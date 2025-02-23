#pragma once
#include "Compression_intf.h"
#include <span>
#include <cstdint>

namespace util
{
    class Zlib : public Compression_intf
    {
    public:
        Zlib() {}
        virtual Result<std::vector<uint8_t>, ErrorCode> compress(std::span<uint8_t> data) override;
        virtual Result<std::vector<uint8_t>, ErrorCode> decompress(std::span<const uint8_t> data) override;
    };
}