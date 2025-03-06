#include "InitCommand.h"
#include <iostream>

Result<std::monostate, ErrorCode> git::InitCommand::execute(std::vector<std::string> &&args)
{
    auto res = f_.createDirectory(g_.getGitDir() + "/objects/");

    if (res.is_err())
    {
        std::cerr << "Init Command failed createDirectory objects" << std::endl;
        return res.error();
    }

    res = f_.createDirectory(g_.getGitDir() + "/refs/");

    if (res.is_err())
    {
        std::cerr << "Init Command failed createDirectory refs" << std::endl;
        return res.error();
    }

    res = f_.createFile(g_.getGitDir() + "/index");

    if (res.is_err())
    {
        std::cerr << "Init Command failed createFile index" << std::endl;
        return res.error();
    }

    return std::monostate();
}