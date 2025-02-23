#include "Zlib.h"
#include <zlib.h>
#include <vector>
#include <stdexcept>
#include <cstring>

Result<std::vector<uint8_t>, ErrorCode> util::Zlib::compress(std::span<uint8_t> data)
{
    uLong sourceSize = data.size();
    uLong destSize = compressBound(sourceSize); // Get max compressed size
    std::vector<uint8_t> compressedData(sizeof(uLong) + destSize);
    std::memcpy(compressedData.data(), &sourceSize, sizeof(uLong));

    int result = ::compress(compressedData.data() + sizeof(uLong), &destSize, data.data(), sourceSize);
    if (result != Z_OK)
    {
        throw std::runtime_error("Compression failed");
    }

    compressedData.resize(sizeof(uLong) + destSize); // Resize to actual compressed size
    return compressedData;
}

Result<std::vector<uint8_t>, ErrorCode> util::Zlib::decompress(std::span<const uint8_t> compressedData)
{

    uLong originalSize = 0;
    std::memcpy(&originalSize, compressedData.data(), sizeof(uLong));
    std::vector<uint8_t> decompressedData(originalSize);

    int result = ::uncompress(decompressedData.data(), &originalSize, compressedData.data() + sizeof(uLong), compressedData.size() - sizeof(uLong));
    if (result != Z_OK)
    {
        throw std::runtime_error("Decompression failed");
    }

    decompressedData.resize(originalSize); // Resize to actual decompressed size
    return decompressedData;
}