#include "GitConfig.h"

git::GitConfig &git::GitConfig::getGitConfigInstance()
{
    static GitConfig inst;

    return inst;
}

void git::GitConfig::setGitDir(std::string &&dir)
{
    gitDir = std::move(dir);
}

std::string git::GitConfig::getGitDir() const
{
    return gitDir;
}

void git::GitConfig::storeConfig()
{
    // TODO store config in proto format
}

void git::GitConfig::loadConfig()
{
    // TODO load config in proto format
}