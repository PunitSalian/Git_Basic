#pragma once
#include "FileManagerInterface.h"

namespace storage
{
    template <typename T>
    class FileManager : public FileManagerInterface<T>
    {
    public:
        FileManager()
        {
            static_assert(std::is_same_v<T, std::string> || std::is_same_v<T, std::filesystem::path>,
                          "T must be string or std::filesystem::path");
        }

        virtual Result<std::monostate, ErrorCode> createDirectory(const T &param) override;

        virtual Result<std::monostate, ErrorCode> removeDirectory(const T &param) override;

        virtual Result<std::monostate, ErrorCode> createFile(const T &param) override;

        virtual Result<std::monostate, ErrorCode> removeFile(const T &param) override;

        virtual Result<size_t, ErrorCode> getFilesize(const T &param) override;

        virtual Result<std::monostate, ErrorCode> readFile(const T &param, std::span<uint8_t> data) override;

        virtual Result<size_t, ErrorCode> writeFile(const T &param, std::span<const uint8_t> data) override;

        virtual Result<bool, ErrorCode> isFile(const T &param) override;

        virtual Result<struct stat, ErrorCode> fileStat(const T &param) override;
    };
}

#include "FileManager.inl"