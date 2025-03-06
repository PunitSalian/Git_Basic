#pragma once
#include "GitConfig_intf.h"

namespace git
{
    class GitConfig : public GitConfig_intf
    {
    private:
        GitConfig(){};
        std::string gitDir;
        std::string gitIndexDir;
        void storeConfig();
        void loadConfig();

    public:
        static GitConfig &getGitConfigInstance();
        virtual void setGitDir(std::string &&dir) override;
        virtual std::string getGitDir() const override;
        virtual void setGitIndexDir(std::string &&dir) override;
        virtual std::string getGitIndexDir() const override;
    };
}