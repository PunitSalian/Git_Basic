#include "CommitCommand.h"

Result<std::monostate, ErrorCode> git::CommitCommand::execute(std::vector<std::string> &&args)
{
    return std::monostate();
}