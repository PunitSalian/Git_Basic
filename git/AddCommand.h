#pragma once
#include "Command_intf.h"
#include "core/GitIndex_intf.h"
#include "storage/Objectstore/Blob_intf.h"
#include "storage/FileManager/FileManagerInterface.h"
#include "config/GitConfig.h"
#include "storage/FileManager/FileManagerInterface.h"
#include "storage/Objectstore/Blob_intf.h"
#include <filesystem>

namespace git
{
    class AddCommand : public Command_intf
    {
    public:
        AddCommand(storage::FileManagerInterface<std::filesystem::path> &f, git::GitConfig &g, storage::Blob_intf &b, GitIndex_intf &gi) : f_(f), g_(g), b_(b), gi_(gi) {}
        virtual Result<std::monostate, ErrorCode> execute(std::vector<std::string> &&args) override;

    private:
        storage::FileManagerInterface<std::filesystem::path> &f_;
        git::GitConfig &g_;
        storage::Blob_intf &b_;
        GitIndex_intf &gi_;
    };
}