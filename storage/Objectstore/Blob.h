#pragma once
#include "Blob_intf.h"
#include "utility/Sha/Hash_intf.h"
#include "storage/FileManager/FileManagerInterface.h"
#include "utility/Compression/Compression_intf.h"
#include "config/GitConfig_intf.h"
#include <memory>
#include <filesystem>

namespace storage
{
    class Blob : public Blob_intf
    {
    public:
        Blob(util::Hash_intf &h, storage::FileManagerInterface<std::filesystem::path> &f,
             git::GitConfig_intf &g, util::Compression_intf &z) : hash_(h), gitconfig_(g), filemgr_(f), compress_handle_(z) {}
        Result<std::pair<storage::Objecttype, std::vector<uint8_t>>, ErrorCode> getObject(const std::string &hash) override;
        Result<std::string, ErrorCode> storeObject(std::vector<uint8_t> &&datam, storage::Objecttype blobtype) override;

    private:
        util::Hash_intf &hash_;
        git::GitConfig_intf &gitconfig_;
        storage::FileManagerInterface<std::filesystem::path> &filemgr_;
        util::Compression_intf &compress_handle_;
    };
}