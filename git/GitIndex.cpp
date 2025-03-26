#include "GitIndex.h"
#include <iostream>
#include <cstring>
#include <vector>
#include <utility>
#include <arpa/inet.h>
#include <algorithm>

namespace
{
    uint32_t toUInt32(const uint8_t *data)
    {
        return (data[0] << 24) | (data[1] << 16) | (data[2] << 8) | data[3];
    }

    uint16_t toUInt16(const uint8_t *data)
    {
        return (data[0] << 8) | data[1];
    }
}

Result<std::vector<git::IndexEntry>, ErrorCode> git::GitIndex::readIndexEntries()
{
    std::vector<IndexEntry> entries;

    auto index_size = f_.getFilesize(g_.getGitIndexDir());
    if (index_size.is_err())
    {
        return index_size.error();
    }

    std::vector<uint8_t> buff(index_size.value()); // Reserve buffer with correct size
    auto res = f_.readFile(g_.getGitIndexDir(), buff);
    if (res.is_err())
    {
        return res.error();
    }

    // Check header signature
    if (std::memcmp(buff.data(), "DIRC", 4) != 0)
    {
        std::cerr << "Invalid index file: Signature mismatch\n";
        return ErrorCode::InvalidFile;
    }

    uint32_t version = toUInt32(buff.data() + 4);
    uint32_t entryCount = toUInt32(buff.data() + 8);

    std::cout << "Git Index Version: " << version << "\n";
    std::cout << "Entry Count: " << entryCount << "\n";

    size_t offset = 12;
    for (uint32_t i = 0; i < entryCount; ++i)
    {
        if (offset + 62 > buff.size())
        {
            std::cerr << "Invalid index file: Truncated entry\n";
            return ErrorCode::InvalidFile;
        }

        IndexEntry entry;
        std::memcpy(&entry, buff.data() + offset, 62); // Read raw entry bytes

        offset += 62; // Move to file path

        // Read path (null-terminated)
        size_t pathStart = offset;
        while (offset < buff.size() && buff[offset] != '\0')
        {
            offset++;
        }
        if (offset < buff.size())
        {
            entry.path = std::string(reinterpret_cast<const char *>(buff.data() + pathStart), offset - pathStart);
        }
        offset++; // Skip null terminator

        // Align to 8-byte boundary
        offset = (offset + 7) & ~7;

        entries.push_back(entry);
    }

    return entries;
}

Result<std::monostate, ErrorCode> git::GitIndex::indexInitFile()
{
    std::vector<uint8_t> buffer;
    IndexHeader header = {{'D', 'I', 'R', 'C'}, 2, 0, {'A', 'B', 'C', 'D'}};
    header.version = __builtin_bswap32(header.version);
    header.num_entries = __builtin_bswap32(header.num_entries);
    uint8_t *header_bytes = reinterpret_cast<uint8_t *>(&header);

    std::string sha_res = sha_.computeHash(std::span<uint8_t>(header_bytes, sizeof(IndexHeader)));
    buffer.reserve(sizeof(IndexHeader) + sha_res.size());
    buffer.insert(buffer.end(), header_bytes, header_bytes + sizeof(IndexHeader));
    buffer.insert(buffer.end(), sha_res.begin(), sha_res.end());

    auto res = f_.writeFile(g_.getGitIndexDir(), buffer);
    if (res.is_err())
    {
        std::cerr << "GitIndex error in write file " << static_cast<uint16_t>(res.error()) << std::endl;
        return res.error();
    }

    return std::monostate();
}

Result<std::monostate, ErrorCode> git::GitIndex::saveStagedFile(
    const std::variant<std::filesystem::path, std::string> &param,
    std::string &&sha)
{
    struct stat st;
    size_t size = 0;
    std::string filename;

    // Determine if the param is a file path
    if (std::holds_alternative<std::filesystem::path>(param))
    {
        auto res = f_.fileStat(std::get<std::filesystem::path>(param));
        if (res.is_err())
        {
            std::cerr << "saveStagedFile error reading stats " << static_cast<uint16_t>(res.error()) << std::endl;
            return ErrorCode::FailToOpen;
        }

        st = res.value();
        size = f_.getFilesize(std::get<std::filesystem::path>(param)).value();
        filename = std::get<std::filesystem::path>(param).string();
    }
    else
    {
        std::cerr << "Error: std::string path not implemented" << std::endl;
        return ErrorCode::FailToOpen;
    }

    // 🔹 Read raw index entries from file
    auto rawEntries = readIndexEntries();
    if (rawEntries.is_err())
    {
        std::cerr << "Error reading existing index file\n";
        return rawEntries.error();
    }

    std::vector<IndexEntry> entries = rawEntries.value();

    // 🔹 Remove existing entry if it's already staged
    auto it = std::remove_if(entries.begin(), entries.end(),
                             [&](const IndexEntry &entry)
                             { return entry.path == filename; });
    entries.erase(it, entries.end());

    IndexEntry newEntry = {
        (uint32_t)st.st_ctime, 0, // ctime
        (uint32_t)st.st_mtime,    // mtime
        0,                        // mtime
        (uint32_t)st.st_dev,      // dev
        (uint32_t)st.st_ino,      // ino
        0100644,                  // mode (regular file)
        (uint32_t)st.st_uid,      // uid
        (uint32_t)st.st_gid,      // gid
        (uint32_t)st.st_size,     // size
        {},                       // sha1
        (uint16_t)size,           // flags (14-bit path length)
    };

    std::memcpy(newEntry.sha1, sha.c_str(), 20);
    newEntry.path = filename;

    // 🔹 Append new entry
    entries.push_back(newEntry);

    // 🔹 Serialize and write back to index file
    std::vector<uint8_t> data;
    data.insert(data.end(), {'D', 'I', 'R', 'C'}); // Signature
    data.insert(data.end(), {0, 0, 0, 2});         // Version 2
    uint32_t entryCount = htonl(entries.size());
    data.insert(data.end(), reinterpret_cast<uint8_t *>(&entryCount), reinterpret_cast<uint8_t *>(&entryCount) + 4);

    for (const auto &entry : entries)
    {
        // Serialize fields in network byte order (big-endian)
        auto appendUInt32 = [&](uint32_t value)
        {
            uint32_t beValue = htonl(value);
            data.insert(data.end(), reinterpret_cast<uint8_t *>(&beValue), reinterpret_cast<uint8_t *>(&beValue) + 4);
        };

        auto appendUInt16 = [&](uint16_t value)
        {
            uint16_t beValue = htons(value);
            data.insert(data.end(), reinterpret_cast<uint8_t *>(&beValue), reinterpret_cast<uint8_t *>(&beValue) + 2);
        };

        appendUInt32(entry.ctime_sec);
        appendUInt32(entry.ctime_nsec);
        appendUInt32(entry.mtime_sec);
        appendUInt32(entry.mtime_nsec);
        appendUInt32(entry.dev);
        appendUInt32(entry.ino);
        appendUInt32(entry.mode);
        appendUInt32(entry.uid);
        appendUInt32(entry.gid);
        appendUInt32(entry.size);

        data.insert(data.end(), entry.sha1, entry.sha1 + 20);

        appendUInt16(entry.flags);

        // Append file path (null-terminated)
        data.insert(data.end(), entry.path.begin(), entry.path.end());
        data.push_back('\0');

        // Align to 8-byte boundary
        while (data.size() % 8 != 0)
        {
            data.push_back(0);
        }
    }

    auto res = f_.writeFile(g_.getGitIndexDir(), data);
    if (res.is_err())
    {
        std::cerr << "saveStagedFile write error " << static_cast<uint8_t>(res.error()) << std::endl;
    }
    return std::monostate();
}

Result<std::vector<git::IndexEntry>, ErrorCode> git::GitIndex::getStagedFile()
{
    auto rawEntries = readIndexEntries();
    if (rawEntries.is_err())
    {
        return rawEntries.error();
    }

    std::vector<IndexEntry> entries = rawEntries.value();

    for (auto &entry : entries)
    {
        // Convert multi-byte fields from big-endian to little-endian
        entry.ctime_sec = ntohl(entry.ctime_sec);
        entry.ctime_nsec = ntohl(entry.ctime_nsec);
        entry.mtime_sec = ntohl(entry.mtime_sec);
        entry.mtime_nsec = ntohl(entry.mtime_nsec);
        entry.dev = ntohl(entry.dev);
        entry.ino = ntohl(entry.ino);
        entry.mode = ntohl(entry.mode);
        entry.uid = ntohl(entry.uid);
        entry.gid = ntohl(entry.gid);
        entry.size = ntohl(entry.size);
        entry.flags = ntohs(entry.flags);
    }

    return entries;
}