#pragma once
#include "Hash_intf.h"
#include "gmock/gmock.h"

namespace test
{
    class MockHash : public util::Hash_intf
    {
    public:
        MOCK_METHOD(std::array<uint8_t, SHA_DIGEST_LENGTH>, computeHash, (std::span<uint8_t> data), (override));
    };
}