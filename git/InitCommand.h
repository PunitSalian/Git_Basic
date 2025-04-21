#pragma once
#include "Command_intf.h"
#include "storage/FileManager/FileManagerInterface.h"
#include "config/GitConfig.h"
#include "core/GitIndex_intf.h"
#include <filesystem>

namespace git
{
    class InitCommand : public Command_intf
    {
    public:
        InitCommand(storage::FileManagerInterface<std::filesystem::path> &f, GitConfig_intf &g, GitIndex_intf &gi) : f_(f), g_(g), gi_(gi) {}
        virtual Result<std::monostate, ErrorCode> execute(std::vector<std::string> &&args) override;

    private:
        storage::FileManagerInterface<std::filesystem::path> &f_;
        GitConfig_intf &g_;
        GitIndex_intf &gi_;
    };
}