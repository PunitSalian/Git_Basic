#pragma once
#include "common/Result.h"
#include "common/Error.h"
#include <variant>

namespace git
{
    class Command_intf
    {
        virtual Result<std::monostate, ErrorCode> execute() = 0;
    };
}