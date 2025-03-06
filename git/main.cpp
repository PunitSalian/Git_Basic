#include "Command_intf.h"
#include "InitCommand.h"
#include "AddCommand.h"
#include "storage/FileManager/FileManager.h"
#include "config/GitConfig.h"
#include "storage/Objectstore/Blob.h"
#include "utility/Sha/Sha1.h"
#include "utility/Compression/Zlib.h"
#include <tclap/CmdLine.h>

int main(int argc, char **argv)
{
    util::Sha1 h;
    util::Zlib z;
    storage::FileManager<std::filesystem::path> f;

    git::GitConfig &g = git::GitConfig::getGitConfigInstance();
    std::filesystem::path currentPath = std::filesystem::current_path() / ".git";
    g.setGitDir(std::move(currentPath.string()));
    currentPath = std::filesystem::current_path() / ".git" / "index";
    g.setGitIndexDir(std::move(currentPath.string()));

    storage::Blob b(h, f, g, z);

    TCLAP::CmdLine cmd("my_git utility", ' ', "1.0", false);
    TCLAP::UnlabeledValueArg<std::string> firstArg("command", "Git command", true, "", "string", cmd);
    TCLAP::UnlabeledMultiArg<std::string> argsArg(
        "args", "Additional arguments", false, "strings", cmd);
    cmd.parse(argc, argv);

    auto &command = firstArg.getValue();
    std::vector<std::string> args = std::move(argsArg.getValue());
    git::Command_intf *c = nullptr;

    if (command == "init")
    {
        git::InitCommand init_comm(f, g);
        c = &init_comm;
    }
    else if (command == "add")
    {
        git::AddCommand add_comm(f, g, b);
        c = &add_comm;
    }
    if (c)
        c->execute(std::move(args));
    else
        std::cerr << "Not a valid command" << std::endl;
}