#include "CommitCommand.h"
#include "Tree.h"
#include <iostream>

Result<std::monostate, ErrorCode> git::CommitCommand::execute(std::vector<std::string> &&args)
{
    auto staged_files = gi_.getStagedFile();

    if (staged_files.is_err())
    {
        std::cerr << "Commit command error " << static_cast<uint16_t>(staged_files.error()) << std::endl;
    }

    t_.WriteTree(t_.buildTree(staged_files.value()).value());

    return std::monostate();
}