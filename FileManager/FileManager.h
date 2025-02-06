#pragma once
#include "FileManagerInterface.h"

namespace storage
{
    template <typename T>
    class FileManager : public FileManagerInterface<T>
    {
    public:
        FileManager() {}

        virtual Result<std::monostate, ErrorCode> createDirectory(const T &param) override;

        virtual Result<std::monostate, ErrorCode> createFile(const T &param) override;

        virtual Result<std::monostate, ErrorCode> removeFile(const T &param) override;

        virtual Result<size_t, ErrorCode> getFilesize(const T &param) override;

        virtual Result<std::monostate, ErrorCode> readFile(const T &param, std::span<uint8_t> data) override;

        virtual Result<size_t, ErrorCode> writeFile(const T &param, std::span<uint8_t> data) override;
    };
}

#include "FileManager.inl"