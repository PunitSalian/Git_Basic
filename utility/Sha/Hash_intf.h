#pragma once

#include <string>
#include <span>
namespace util
{
        class Hash_intf
        {
        public:
                virtual ~Hash_intf() {}

                virtual std::string computeHash(std::span<uint8_t> data) = 0;
        };
}