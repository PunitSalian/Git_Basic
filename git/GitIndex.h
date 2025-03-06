#pragma once
#include "GitIndex_intf.h"

namespace git
{

    class GitIndex : public GitIndex_intf
    {
    public:
        GitIndex() {}
        Result<std::string, ErrorCode> saveIndex() override;
        Result<std::string, ErrorCode> getIndex() override;
    };

} // namespace git