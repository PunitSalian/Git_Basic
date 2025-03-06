#pragma once
#include <string>
namespace git
{
    class GitConfig_intf
    {
    public:
        virtual ~GitConfig_intf() {}
        virtual void setGitDir(std::string &&dir) = 0;
        virtual std::string getGitDir() const = 0;
        virtual void setGitIndexDir(std::string &&dir) = 0;
        virtual std::string getGitIndexDir() const = 0;
    };
} // namespace git
