#pragma once
#include "common/Error.h"
#include "common/Result.h"
#include <variant>
#include <span>
#include <sys/stat.h>
namespace storage
{
    template <typename T>
    class FileManagerInterface
    {
    public:
        virtual ~FileManagerInterface() = default;

        virtual Result<std::monostate, ErrorCode> createDirectory(const T &param) = 0;

        virtual Result<std::monostate, ErrorCode> removeDirectory(const T &param) = 0;

        virtual Result<std::monostate, ErrorCode> createFile(const T &param) = 0;

        virtual Result<std::monostate, ErrorCode> removeFile(const T &param) = 0;

        virtual Result<size_t, ErrorCode> getFilesize(const T &param) = 0;

        virtual Result<std::monostate, ErrorCode> readFile(const T &param, std::span<uint8_t> data) = 0;

        virtual Result<size_t, ErrorCode> writeFile(const T &param, std::span<uint8_t> data) = 0;

        virtual Result<bool, ErrorCode> isFile(const T &param) = 0;

        virtual Result<struct stat, ErrorCode> fileStat(const T &param) = 0;
    };
}