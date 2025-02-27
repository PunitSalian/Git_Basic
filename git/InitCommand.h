#pragma once
#include "Command_intf.h"
#include "storage/FileManager/FileManagerInterface.h"
#include "config/GitConfig.h"
#include <filesystem>

namespace git
{
    class InitCommand : public Command_intf
    {
    public:
        InitCommand(storage::FileManagerInterface<std::filesystem::path> &f, GitConfig_intf &g) : f_(f), g_(g) {}
        virtual Result<std::monostate, ErrorCode> execute() override;

    private:
        storage::FileManagerInterface<std::filesystem::path> &f_;
        GitConfig_intf &g_;
    };
}