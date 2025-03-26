#pragma once
#include <string>
#include <vector>
#include <variant>
#include <filesystem>
#include "common/Error.h"
#include "common/Result.h"
namespace git
{
#pragma pack(push, 1) // Ensure no padding in struct
    struct IndexEntry
    {
        uint32_t ctime_sec;
        uint32_t ctime_nsec;
        uint32_t mtime_sec;
        uint32_t mtime_nsec;
        uint32_t dev;
        uint32_t ino;
        uint32_t mode;
        uint32_t uid;
        uint32_t gid;
        uint32_t size;
        unsigned char sha1[20];
        uint16_t flags;
        std::string path; // Variable-length, null-terminated
    };
    struct IndexHeader
    {
        char sign[4];
        uint32_t version;
        uint32_t num_entries;
        char extension_sign[4];
    };
#pragma pack(pop) // Restore default alignment

    class GitIndex_intf
    {
    public:
        virtual ~GitIndex_intf() = default;
        virtual Result<std::monostate, ErrorCode> indexInitFile() = 0;
        virtual Result<std::monostate, ErrorCode> saveStagedFile(const std::variant<std::filesystem::path, std::string> &param, std::string &&sha) = 0;
        virtual Result<std::vector<IndexEntry>, ErrorCode> getStagedFile() = 0;
    };
}