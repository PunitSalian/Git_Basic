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

        // Read and convert struct fields to host byte order
        std::memcpy(&entry.stat_entry, buff.data() + offset, 62);

        offset += 62; // Move to file path

        // Read path (null-terminated)
        size_t pathStart = offset;
        while (offset < buff.size() && buff[offset] != '\0')
        {
            offset++;
        }
        if (offset < buff.size())
        {
            entry.file = std::string(reinterpret_cast<const char *>(buff.data() + pathStart), offset - pathStart);
        }

        offset++; // Skip null terminator
        std::cout << "offfset without padding = " << offset << std::endl;

        // Align to 8-byte boundary
        offset += (8 - ((offset - 12) % 8));

        std::cout << "offfset = " << offset << std::endl;
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

    auto sha_res = sha_.computeHash(std::span<uint8_t>(header_bytes, sizeof(IndexHeader)));
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
    std::string filename;

    // Determine if param is a file path
    if (std::holds_alternative<std::filesystem::path>(param))
    {
        auto res = f_.fileStat(std::get<std::filesystem::path>(param));
        if (res.is_err())
        {
            std::cerr << "saveStagedFile error reading stats " << static_cast<uint16_t>(res.error()) << std::endl;
            return ErrorCode::FailToOpen;
        }
        st = res.value();
        filename = std::get<std::filesystem::path>(param).string();
    }
    else
    {
        std::cerr << "Error: std::string path not implemented" << std::endl;
        return ErrorCode::FailToOpen;
    }

    // 🔹 Read existing index entries
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
                             { return entry.file == filename; });
    entries.erase(it, entries.end());

    // 🔹 Create a new index entry
    IndexEntry newEntry;
    newEntry.file = filename;

    newEntry.stat_entry = {
        (uint32_t)st.st_ctime, // ctime_sec
        0,                     // ctime_nsec
        (uint32_t)st.st_mtime, // mtime_sec
        0,                     // mtime_nsec
        (uint32_t)st.st_dev,   // dev
        (uint32_t)st.st_ino,   // ino
        0100644,               // mode (Regular file: 100644)
        (uint32_t)st.st_uid,   // uid
        (uint32_t)st.st_gid,   // gid
        (uint32_t)st.st_size,  // size
        {},                    // sha1 (set below)
        (uint16_t)0xFFF,       // flags
    };

    newEntry.stat_entry.ctime_sec = htonl(newEntry.stat_entry.ctime_sec);
    newEntry.stat_entry.ctime_nsec = htonl(newEntry.stat_entry.ctime_nsec);
    newEntry.stat_entry.mtime_sec = htonl(newEntry.stat_entry.mtime_sec);
    newEntry.stat_entry.mtime_nsec = htonl(newEntry.stat_entry.mtime_nsec);
    newEntry.stat_entry.dev = htonl(newEntry.stat_entry.dev);
    newEntry.stat_entry.ino = htonl(newEntry.stat_entry.ino);
    newEntry.stat_entry.mode = htonl(newEntry.stat_entry.mode);
    newEntry.stat_entry.uid = htonl(newEntry.stat_entry.uid);
    newEntry.stat_entry.gid = htonl(newEntry.stat_entry.gid);
    newEntry.stat_entry.size = htonl(newEntry.stat_entry.size);
    newEntry.stat_entry.flags = htons(newEntry.stat_entry.flags);
    auto sha_hex = util::fromHexString(sha);
    memcpy(sha_hex.data(), newEntry.stat_entry.sha1, sha_hex.size());

    // 🔹 Append the new entry
    entries.push_back(newEntry);

    // 🔹 Serialize and write back to index file
    std::vector<uint8_t> data;

    // Write header (magic signature, version, entry count)
    data.insert(data.end(), {'D', 'I', 'R', 'C'}); // "DIRC"
    data.insert(data.end(), {0, 0, 0, 2});         // Version 2
    uint32_t entryCount = htonl(entries.size());
    data.insert(data.end(), reinterpret_cast<uint8_t *>(&entryCount),
                reinterpret_cast<uint8_t *>(&entryCount) + 4);

    std::cout << "Header size = " << data.size() << ", Entry count = " << entries.size() << std::endl;

    // 🔹 Iterate over entries (without applying htonl again)
    for (const auto &entry : entries)
    {
        // Write IndexStatEntry (already in network byte order)
        const uint8_t *entryBytes = reinterpret_cast<const uint8_t *>(&entry.stat_entry);
        data.insert(data.end(), entryBytes, entryBytes + sizeof(IndexStatEntry));

        // Write file path (null-terminated)
        data.insert(data.end(), entry.file.begin(), entry.file.end());
        data.push_back('\0');

        while ((data.size() - 12) % 8 != 0)
        {
            data.push_back('\0');
        }
    }

    // 🔹 Append Git Extension: 'ABCD'
    uint8_t extName[] = {'A', 'B', 'C', 'D'};
    data.insert(data.end(), extName, extName + 4);

    std::cout << "Header + Data + extension size " << data.size() << std::endl;

    // 🔹 Compute and append SHA-1 checksum for the entire index

    auto sha_res = sha_.computeHash(std::span<uint8_t>(data.data(), data.size()));

    data.insert(data.end(), sha_res.begin(), sha_res.end());

    // 🔹 Write final index file
    auto res = f_.writeFile(g_.getGitIndexDir(), data);
    if (res.is_err())
    {
        std::cerr << "saveStagedFile write error " << static_cast<uint8_t>(res.error()) << std::endl;
        return res.error();
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
        entry.stat_entry.ctime_sec = ntohl(entry.stat_entry.ctime_sec);
        entry.stat_entry.ctime_nsec = ntohl(entry.stat_entry.ctime_nsec);
        entry.stat_entry.mtime_sec = ntohl(entry.stat_entry.mtime_sec);
        entry.stat_entry.mtime_nsec = ntohl(entry.stat_entry.mtime_nsec);
        entry.stat_entry.dev = ntohl(entry.stat_entry.dev);
        entry.stat_entry.ino = ntohl(entry.stat_entry.ino);
        entry.stat_entry.mode = ntohl(entry.stat_entry.mode);
        entry.stat_entry.uid = ntohl(entry.stat_entry.uid);
        entry.stat_entry.gid = ntohl(entry.stat_entry.gid);
        entry.stat_entry.size = ntohl(entry.stat_entry.size);
        entry.stat_entry.flags = ntohs(entry.stat_entry.flags);
    }

    return entries;
}