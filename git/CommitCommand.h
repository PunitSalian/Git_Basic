#pragma once
#include "Command_intf.h"
#include "config/GitConfig.h"
#include "storage/FileManager/FileManagerInterface.h"
#include "storage/Objectstore/Blob_intf.h"
#include <filesystem>

namespace git
{
    class CommitCommand : public Command_intf
    {
    public:
        CommitCommand(storage::FileManagerInterface<std::filesystem::path> &f, git::GitConfig &g, storage::Blob_intf &b) : f_(f), g_(g), b_(b) {}
        virtual Result<std::monostate, ErrorCode> execute(std::vector<std::string> &&args) override;

    private:
        storage::FileManagerInterface<std::filesystem::path> &f_;
        git::GitConfig &g_;
        storage::Blob_intf &b_;
    };
}