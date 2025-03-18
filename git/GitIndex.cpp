#include "GitIndex.h"
#include <iostream>
#include <cstring>
#include <vector>
#include <utility>

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

Result<std::monostate, ErrorCode> git::GitIndex::saveStagedFile(const std::variant<std::filesystem::path, std::string> &param, std::string &&sha)
{
    struct stat st;
    size_t size = 0;
    std::string filename;
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

        // std::unreachable();
    }

    // Create an index entry
    IndexEntry entry = {
        (uint32_t)st.st_ctime, 0, // ctime
        (uint32_t)st.st_mtime,
        0,                    // mtime
        (uint32_t)st.st_dev,  // dev
        (uint32_t)st.st_ino,  // ino
        0100644,              // mode (regular file, non-executable)
        (uint32_t)st.st_uid,  // uid
        (uint32_t)st.st_gid,  // gid
        (uint32_t)st.st_size, // size
        {},                   // sha1 (filled below)
        (uint16_t)size,       // flags (14-bit path length)
    };
    std::memcpy(entry.sha1, sha.c_str(), 20);

    std::vector<uint8_t> data;

    const uint8_t *entry_bytes = reinterpret_cast<const uint8_t *>(&entry);

    data.insert(data.end(), entry_bytes, entry_bytes + sizeof(IndexEntry));

    data.insert(data.end(), filename.begin(), filename.end());
    data.push_back('\0');
    while (data.size() % 8 != 0)
    {
        data.push_back(0);
    }
    auto res = f_.writeFile(g_.getGitIndexDir(), data);
    std::cout << "Index file " << g_.getGitIndexDir() << std::endl;
    if (res.is_err())
    {
        std::cerr << "saveStagedFile write error " << static_cast<uint8_t>(res.error()) << std::endl;
    }
    return std::monostate();
}

Result<std::vector<std::string>, ErrorCode> git::GitIndex::getStagedFile()
{
    std::vector<std::string> v;
    return v;
}