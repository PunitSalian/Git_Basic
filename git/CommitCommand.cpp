#include "CommitCommand.h"
#include <iostream>

Result<std::monostate, ErrorCode> git::CommitCommand::execute(std::vector<std::string> &&args)
{
    auto staged_files = gi_.getStagedFile();

    if (staged_files.is_err())
    {
        std::cerr << "Commit command error " << static_cast<uint16_t>(staged_files.error()) << std::endl;
    }

    auto res = t_.WriteTree(t_.buildTree(staged_files.value()).value());

    std::ostringstream oss;

    oss << "tree " << res.value() << "\n";

    oss << "author "
        << "Punit Salian"
        << " <"
        << "salian@gmail.com"
        << "> "
        << "1712719152"
        << " "
        << "+0000"
        << "\n";

    oss << "committer "
        << "Punit Salian"
        << " <"
        << "salian@gmail.com"
        << "> "
        << "1712719152"
        << " "
        << "+0000"
        << "\n\n";

    oss << "Hello World"
        << "\n";

    std::string content = oss.str();

    std::ostringstream full;
    full << content;

    std::string fullCommit = full.str();

    std::vector<uint8_t> raw_entry(fullCommit.begin(), fullCommit.end());

    auto commit_hash = b_.storeObject(std::move(raw_entry), storage::Objecttype::COMMIT);

    std::cout << "commit hash val " << commit_hash.value() << std::endl;
    return std::monostate();
}