#pragma once
#include "Blob_intf.h"
#include "gmock/gmock.h"

namespace test
{
    class MockBlob : public storage::Blob_intf
    {
    public:
        MOCK_METHOD((Result<std::pair<storage::Objecttype, std::vector<uint8_t>>, ErrorCode>), getObject, (const std::string &hash), (override));
        MOCK_METHOD((Result<std::string, ErrorCode>), storeObject, (std::vector<uint8_t> && data, storage::Objecttype blobtype), (override));
    };
}