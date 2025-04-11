#include "Zlib.h"
#include <zlib.h>
#include <vector>
#include <stdexcept>
#include <cstring>
#include <iostream>
#include <iomanip>

Result<std::vector<uint8_t>, ErrorCode> util::Zlib::compress(std::span<uint8_t> data)
{
    if (data.empty())
    {
        return ErrorCode::InvalidArgument;
    }

    z_stream zs;
    zs.zalloc = Z_NULL;
    zs.zfree = Z_NULL;
    zs.opaque = Z_NULL;
    zs.avail_in = static_cast<uInt>(data.size());
    zs.next_in = const_cast<Bytef *>(data.data());
    zs.avail_out = 0;
    zs.next_out = Z_NULL;

    if (deflateInit(&zs, Z_BEST_COMPRESSION) != Z_OK)
    {
        throw std::runtime_error("deflateInit failed while compressing.");
    }

    std::vector<unsigned char> compressedData;
    unsigned char outBuffer[4096];

    do
    {
        zs.avail_out = sizeof(outBuffer);
        zs.next_out = outBuffer;

        if (deflate(&zs, Z_FINISH) == Z_STREAM_ERROR)
        {
            deflateEnd(&zs);
            throw std::runtime_error("deflate failed while compressing.");
        }

        size_t compressedSize = sizeof(outBuffer) - zs.avail_out;
        if (compressedSize > 0)
        {
            compressedData.insert(compressedData.end(), outBuffer, outBuffer + compressedSize);
        }
    } while (zs.avail_out == 0);

    if (deflateEnd(&zs) != Z_OK)
    {
        throw std::runtime_error("deflateEnd failed while compressing.");
    }

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