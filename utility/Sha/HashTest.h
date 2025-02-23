#pragma once
#include "Hash_intf.h"
#include "gmock/gmock.h"

namespace test
{
    class MockHash : public util::Hash_intf
    {
    public:
        MOCK_METHOD(std::string, computeHash, (std::span<uint8_t> data), (override));
    };
}