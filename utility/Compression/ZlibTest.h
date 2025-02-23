#pragma once
#include "Compression_intf.h"
#include "gmock/gmock.h"

namespace test
{
    class MockZlib : public util::Compression_intf
    {
    public:
        MOCK_METHOD((Result<std::vector<uint8_t>, ErrorCode>), compress, (std::span<uint8_t> data), (override));
        MOCK_METHOD((Result<std::vector<uint8_t>, ErrorCode>), decompress, (std::span<const uint8_t> data), (override));
    };

}