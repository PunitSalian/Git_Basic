#include "InitCommand.h"
#include <iostream>

Result<std::monostate, ErrorCode> git::InitCommand::execute()
{
    auto res = f_.createDirectory(g_.getGitDir());

    if (res.is_err())
    {
        std::cerr << "Init Command failed " << std::endl;
        return res.error();
    }

    return std::monostate();
}