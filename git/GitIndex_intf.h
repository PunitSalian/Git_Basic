#pragma once
#include <string>
#include "common/Error.h"
#include "common/Result.h"
namespace git
{
    class GitIndex_intf
    {
    public:
        virtual ~GitIndex_intf() {}
        virtual Result<std::string, ErrorCode> saveIndex() = 0;
        virtual Result<std::string, ErrorCode> getIndex() = 0;
    };
}