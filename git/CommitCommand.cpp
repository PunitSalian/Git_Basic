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

    // TODO get this from git config
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

    Result<std::string, ErrorCode> commit_hash = b_.storeObject(std::move(raw_entry), storage::Objecttype::COMMIT);

    std::cout << "commit hash val " << commit_hash.value() << std::endl;

    // TODO helper method for HEAD in gitconfig
    auto size = f_.getFilesize(std::filesystem::path(".git/HEAD"));

    if (size.is_err())
    {
        std::cerr << "CommitCommand execute error " << static_cast<uint16_t>(size.error()) << std::endl;
        return size.error();
    }

    std::vector<uint8_t> buff;
    buff.resize(size.value());

    auto ret = f_.readFile(std::filesystem::path(".git/HEAD"), buff);
    if (ret.is_err())
    {
        std::cerr << "CommitCommand execute read file error " << std::endl;
        return res.error();
    }

    std::string view(reinterpret_cast<const char *>(buff.data()), buff.size());
    std::string prefix = "ref: ";

    if (!view.starts_with(prefix))
    {
        std::cerr << "CommitCommand execute ref: prefix not found " << std::endl;
        return ErrorCode::NotFound;
    }
    std::span<const uint8_t> byteSpanConst(reinterpret_cast<const uint8_t *>(commit_hash.value().data()), commit_hash.value().size());

    ret = f_.createFile(std::filesystem::path(g_.getGitDir() + "/" + view.substr(prefix.length())));

    if (ret.is_err())
    {
        std::cerr << "CommitCommand execute create file error " << static_cast<uint16_t>(ret.error()) << std::endl;
        return ret.error();
    }

    auto val = f_.writeFile(std::filesystem::path(g_.getGitDir() + "/" + view.substr(prefix.length())), byteSpanConst);

    if (val.is_err())
    {
        std::cerr << "CommitCommand execute ref: write file error " << static_cast<uint16_t>(val.error()) << std::endl;

        return val.error();
    }

    return std::monostate();
}