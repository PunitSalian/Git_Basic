#pragma once
#include "GitConfig_intf.h"

namespace git
{
    class GitConfig : public GitConfig_intf
    {
    private:
        GitConfig(){};
        std::string gitDir;
        void storeConfig();
        void loadConfig();

    public:
        static GitConfig &getGitConfigInstance();
        virtual void setGitDir(std::string &&dir) override;
        virtual std::string getGitDir() const override;
    };
}