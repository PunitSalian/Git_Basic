#pragma once
#include "common/Result.h"
#include "common/Error.h"
#include <variant>
#include <vector>

namespace git
{
    class Command_intf
    {
    public:
        virtual Result<std::monostate, ErrorCode> execute(std::vector<std::string> &&args) = 0;
    };
}