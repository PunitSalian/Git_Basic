#pragma once
#include <cstdint>
#include <span>
#include <vector>
#include "common/Error.h"
#include "common/Result.h"

namespace util
{
    class Compression_intf
    {
    public:
        virtual ~Compression_intf() {}
        virtual Result<std::vector<uint8_t>, ErrorCode> compress(std::span<uint8_t> data) = 0;
        virtual Result<std::vector<uint8_t>, ErrorCode> decompress(std::span<const uint8_t> data) = 0;
    };
}