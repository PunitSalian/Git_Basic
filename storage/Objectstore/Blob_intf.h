#pragma once
#include <string>
#include "common/Result.h"
#include "common/Error.h"
#include <variant>
#include <vector>
#include <span>
#include <unordered_map>
namespace storage
{
    enum class Objecttype
    {
        BLOB,
        TREE,
        COMMIT,
    };
    static const std::unordered_map<Objecttype, std::string> objectToString = {
        {Objecttype::BLOB, "blob"},
        {Objecttype::TREE, "tree"},
        {Objecttype::COMMIT, "commit"},
    };
    static const std::unordered_map<std::string, Objecttype> stringToObject = {
        {"blob", Objecttype::BLOB},
        {"tree", Objecttype::TREE},
        {"commit", Objecttype::COMMIT},
    };
    class Blob_intf
    {
    public:
        virtual ~Blob_intf() {}

        virtual Result<std::pair<storage::Objecttype, std::vector<uint8_t>>, ErrorCode> getObject(const std::string &hash) = 0;
        virtual Result<std::string, ErrorCode> storeObject(std::vector<uint8_t> &&data, storage::Objecttype blobtype) = 0;
    };

} // namespace storage