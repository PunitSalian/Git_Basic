#include "Command_intf.h"
#include "InitCommand.h"
#include "storage/FileManager/FileManager.h"
#include "config/GitConfig.h"

int main()
{
    storage::FileManager<std::filesystem::path> f;
    git::GitConfig &g = git::GitConfig::getGitConfigInstance();
    g.setGitDir(".my_git/objects/");
    git::InitCommand init_comm(f, g);
    init_comm.execute();
}