#pragma once

#include "Hash_intf.h"
namespace util
{
    class Sha1 : public Hash_intf
    {
    public:
        Sha1()
        {
        }

        virtual std::array<uint8_t, SHA_DIGEST_LENGTH> computeHash(std::span<uint8_t> data) override;
    };
}
