#include "FileManager.h"
#include <filesystem>
#include <iostream>
#include <fstream>

template <typename T>
Result<std::monostate, ErrorCode> storage::FileManager<T>::createDirectory(const T &param)
{
    if (std::filesystem::exists(param)) 
    {
        return std::monostate();
    }

    if (!std::filesystem::create_directory(param))
    {
        return ErrorCode::Unknown;
    }

    return std::monostate();
}

template <typename T>
Result<std::monostate, ErrorCode> storage::FileManager<T>::removeDirectory(const T &dir)
{
    if (!std::filesystem::exists(dir)) 
    {
        return ErrorCode::FailToOpen;
    }

    if (!std::filesystem::remove(dir))
    {
        return ErrorCode::InternalError;
    }

    return std::monostate();
}

template <typename T>
Result<std::monostate, ErrorCode> storage::FileManager<T>::createFile(const T &param)
{
    std::ofstream file(param);

    if (!file.good())
    {
        return ErrorCode::Unknown;
    }

    return std::monostate();
}

template <typename T>
Result<std::monostate, ErrorCode> storage::FileManager<T>::removeFile(const T &param)
{
    if (!std::filesystem::remove(param))
    {
        return ErrorCode::Unknown;
    }

    return std::monostate();
}

template <typename T>
Result<size_t, ErrorCode> storage::FileManager<T>::getFilesize(const T &param)
{
    if (!std::filesystem::exists(param) && !std::filesystem::is_regular_file(param))
    {
        return ErrorCode::Unknown;
    }
    std::error_code e;
    auto filesize = std::filesystem::file_size(param, e);

    if (e)
    {
        std::cerr << "Error getting size for file " << param << " error " << e.message() << std::endl;
        return ErrorCode::Unknown;
    }

    return filesize;
}

template <typename T>
Result<std::monostate, ErrorCode> storage::FileManager<T>::readFile(const T &param, std::span<uint8_t> buffer)
{
    auto f = getFilesize(param);

    if (f.is_err())
    {
        return f.error();
    }

    if (buffer.size() < f.value())
    {
        return ErrorCode::OutOfRange;
    }

    std::ifstream file(param, std::ios::binary);

    if (!file)
    {
        return ErrorCode::FailToOpen;
    }

    file.read(reinterpret_cast<char *>(buffer.data()), f.value());

    return std::monostate();
}

template <typename T>
Result<size_t, ErrorCode> storage::FileManager<T>::writeFile(const T &param, std::span<uint8_t> buffer)
{

    if (!std::filesystem::exists(param) && !std::filesystem::is_regular_file(param))
    {
        return ErrorCode::Unknown;
    }

    std::ofstream file(param, std::ios::binary);

    if (!file.good())
    {
        return ErrorCode::FailToOpen;
    }

    file.write(reinterpret_cast<char *>(buffer.data()), buffer.size());

    return getFilesize(param);
}
