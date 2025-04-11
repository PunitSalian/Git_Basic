#pragma once
#include "Command_intf.h"
#include "config/GitConfig.h"
#include "storage/FileManager/FileManagerInterface.h"
#include "storage/Objectstore/Blob_intf.h"
#include <filesystem>
#include "Tree.h"

namespace git
{
    class CommitCommand : public Command_intf
    {
    public:
        CommitCommand(git::TreeBuilder &t, git::GitIndex_intf &gi) : t_(t), gi_(gi) {}
        virtual Result<std::monostate, ErrorCode> execute(std::vector<std::string> &&args) override;

    private:
        git::TreeBuilder &t_;
        git::GitIndex_intf &gi_;
    };
}