#include "Blob.h"
#include "Blob_intf.h"
#include <iostream>
Result<std::pair<storage::Objecttype, std::vector<uint8_t>>, ErrorCode> storage::Blob::getObject(const std::string &hash)
{
    std::pair<storage::Objecttype, std::vector<uint8_t>> object;
    auto file = gitconfig_.getGitDir() + hash.substr(0, 2) + "/" + hash.substr(2);
    auto ret = filemgr_.getFilesize(file);
    if (ret.is_err())
    {
        std::cerr << "getObject readFile error" << static_cast<uint16_t>(ret.error()) << std::endl;
        return ErrorCode::FailToOpen;
    }
    std::vector<uint8_t> buffer(ret.value());
    auto res = filemgr_.readFile(file, buffer);
    if (res.is_err())
    {
        std::cerr << "getObject readFile error" << static_cast<uint16_t>(res.error()) << std::endl;
        return ErrorCode::FailToOpen;
    }
    // Blob type and size is encoded in the begining of the file with null termination
    std::string blob_type = std::string(reinterpret_cast<const char *>(buffer.data()));

    buffer.erase(buffer.begin(), buffer.begin() + blob_type.size() + 1);

    if (blob_type.find(objectToString.at(storage::Objecttype::BLOB)) != std::string::npos)
    {
        object.first = storage::Objecttype::BLOB;
        auto ret = compress_handle_.decompress(std::span<uint8_t>(buffer.data(), buffer.size()));
        if (ret.is_err())
        {
            std::cerr << "getObject decompress failed " << static_cast<uint16_t>(ret.error()) << std::endl;
            return ErrorCode::Unknown;
        }
        object.second = std::move(ret.value());
    }
    else if (blob_type.find(objectToString.at(storage::Objecttype::COMMIT)) != std::string::npos)
    {
        object.first = storage::Objecttype::COMMIT;
        object.second = std::move(buffer);
    }
    else if (blob_type.find(objectToString.at(storage::Objecttype::TREE)) != std::string::npos)
    {
        object.first = storage::Objecttype::TREE;
        object.second = std::move(buffer);
    }

    return object;
}

Result<std::string, ErrorCode> storage::Blob::storeObject(std::vector<uint8_t> &&data, storage::Objecttype blobtype)
{
    // Append the blob header with the size, helps while retriving the blob
    std::string blob_header = storage::objectToString.at(blobtype) + " " + std::to_string(data.size()) + "\0";

    blob_header.push_back('\0');

    std::vector<uint8_t> blob_data(blob_header.begin(), blob_header.end());

    // reserve memory to append the header
    data.reserve(data.size() + blob_data.size());

    data.insert(data.begin(), blob_data.begin(), blob_data.end());

    std::string hash = hash_.computeHash(data);

    std::cout << "storeBlob hash " << hash << std::endl;

    // Create a directory with the first 2 chars of the sha
    std::string new_dir = gitconfig_.getGitDir() + hash.substr(0, 2);

    auto res = filemgr_.createDirectory(new_dir);

    if (res.is_err())
    {
        std::cerr << "storeBlob Creating dir not successful " << static_cast<uint16_t>(res.error()) << std::endl;
        return ErrorCode::FailToOpen;
    }

    // Create a file with the rest of chars of the sha
    auto new_file = new_dir + "/" + hash.substr(2);
    res = filemgr_.createFile(new_file);

    if (res.is_err())
    {
        std::cerr << "storeBlob Creating file not successful " << static_cast<uint16_t>(res.error()) << std::endl;
        return ErrorCode::FailToOpen;
    }
    // File Blob should be compressed.
    if (blobtype == storage::Objecttype::BLOB)
    {
        Result<std::vector<uint8_t>, ErrorCode> compress_data_res = compress_handle_.compress(std::span<uint8_t>(data.data() + blob_data.size(), data.size() - blob_data.size()));

        if (compress_data_res.is_err())
        {
            std::cerr << "storeBlob compression file failed " << static_cast<uint16_t>(compress_data_res.error()) << std::endl;
            return ErrorCode::InternalError;
        }

        data = std::move(compress_data_res.value());
    }

    auto wr_res = filemgr_.writeFile(new_file, std::span<unsigned char>(data.data(), data.size()));

    if (wr_res.is_err())
    {
        std::cerr << "storeBlob write File failed " << static_cast<uint16_t>(wr_res.error()) << std::endl;
        return ErrorCode::FailToOpen;
    }

    return hash;
}