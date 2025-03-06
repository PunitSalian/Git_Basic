#include "AddCommand.h"

Result<std::monostate, ErrorCode> git::AddCommand::execute(std::vector<std::string> &&args)
{
    for (auto &f : args)
    {
        auto res = f_.getFilesize(f);
        std::vector<uint8_t> data;
        data.reserve(res.value());
        f_.readFile(f, data);
        auto ret = b_.storeObject(std::move(data), storage::Objecttype::BLOB);
    }

    return std::monostate();
}