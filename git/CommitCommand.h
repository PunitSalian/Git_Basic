#pragma once
#include "Command_intf.h"
#include "config/GitConfig.h"
#include "storage/FileManager/FileManagerInterface.h"
#include "storage/Objectstore/Blob_intf.h"
#include <filesystem>
#include "core/Tree.h"

namespace git
{
    class CommitCommand : public Command_intf
    {
    public:
        CommitCommand(git::TreeBuilder &t, git::GitIndex_intf &gi, storage::Blob_intf &b) : t_(t), gi_(gi), b_(b) {}
        virtual Result<std::monostate, ErrorCode> execute(std::vector<std::string> &&args) override;

    private:
        git::TreeBuilder &t_;
        git::GitIndex_intf &gi_;
        storage::Blob_intf &b_;
    };
}