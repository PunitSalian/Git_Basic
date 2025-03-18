#pragma once
#include "storage/FileManager/FileManagerInterface.h"
#include "config/GitConfig.h"
#include <filesystem>
#include "GitIndex_intf.h"
#include <span>
#include <iostream>
#include "utility/Sha/Hash_intf.h"

namespace git
{
    class GitIndex : public GitIndex_intf
    {
    public:
        GitIndex(storage::FileManagerInterface<std::filesystem::path> &f, GitConfig_intf &g, util::Hash_intf &sha) : f_(f), g_(g), sha_(sha)
        {
        }
        Result<std::monostate, ErrorCode> indexInitFile() override;
        virtual Result<std::monostate, ErrorCode> saveStagedFile(const std::variant<std::filesystem::path, std::string> &param, std::string &&sha) override;
        virtual Result<std::vector<std::string>, ErrorCode> getStagedFile() override;
        storage::FileManagerInterface<std::filesystem::path> &f_;
        GitConfig_intf &g_;
        util::Hash_intf &sha_;
    };

} // namespace git