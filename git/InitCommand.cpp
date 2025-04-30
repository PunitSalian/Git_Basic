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

    res = f_.createDirectory(g_.getGitDir() + "/refs/heads/");

    if (res.is_err())
    {
        std::cerr << "Init Command failed createDirectory refs/heads" << std::endl;
        return res.error();
    }

    res = f_.createFile(g_.getGitDir() + "/index");

    if (res.is_err())
    {
        std::cerr << "Init Command failed createFile index" << std::endl;
        return res.error();
    }

    res = gi_.indexInitFile();

    res = f_.createFile(g_.getGitDir() + "/HEAD");

    if (res.is_err())
    {
        std::cerr << "Init Command failed createFile HEAD" << std::endl;
        return res.error();
    }

    std::string branchReference = "ref: refs/heads/temp-branch";

    std::span<uint8_t> branchSpan(reinterpret_cast<uint8_t *>(branchReference.data()), branchReference.size());

    auto res_wr = f_.writeFile(g_.getGitDir() + "/HEAD", branchSpan);

    if (res_wr.is_err())
    {
        std::cerr << "Init Command failed writeFile HEAD" << std::endl;
        return res_wr.error();
    }

    return std::monostate();
}