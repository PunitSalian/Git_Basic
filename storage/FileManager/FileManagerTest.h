#pragma once
#include "gmock/gmock.h"
#include "FileManagerInterface.h"
namespace test
{
    template <typename T>
    class MockFileManager : public storage::FileManagerInterface<T>
    {
    public:
        MOCK_METHOD((Result<std::monostate, ErrorCode>), createDirectory, (const T &param), (override));
        MOCK_METHOD((Result<std::monostate, ErrorCode>), removeDirectory, (const T &param), (override));
        MOCK_METHOD((Result<std::monostate, ErrorCode>), createFile, (const T &param), (override));
        MOCK_METHOD((Result<std::monostate, ErrorCode>), removeFile, (const T &param), (override));
        MOCK_METHOD((Result<size_t, ErrorCode>), getFilesize, (const T &param), (override));
        MOCK_METHOD((Result<std::monostate, ErrorCode>), readFile, (const T &param, std::span<uint8_t> data), (override));
        MOCK_METHOD((Result<size_t, ErrorCode>), writeFile, (const T &param, std::span<uint8_t> data), (override));
    };
}
