#pragma once

#include <string>
#include <span>
#include <openssl/sha.h>
#include <iomanip>
#include <sstream>

namespace util
{
        inline std::string toHexString(std::span<const uint8_t, SHA_DIGEST_LENGTH> hash)
        {
                std::ostringstream oss;
                for (uint8_t byte : hash)
                {
                        oss << std::hex << std::setw(2) << std::setfill('0') << (int)byte;
                }
                return oss.str();
        }

        inline std::string toHexString(const std::array<uint8_t, SHA_DIGEST_LENGTH> &hash)
        {
                std::ostringstream oss;
                for (uint8_t byte : hash)
                {
                        oss << std::hex << std::setw(2) << std::setfill('0') << (int)byte;
                }
                return oss.str();
        }

        inline std::array<uint8_t, SHA_DIGEST_LENGTH> fromHexString(const std::string &hexString)
        {
                if (hexString.length() != SHA_DIGEST_LENGTH * 2)
                {
                        throw std::invalid_argument("Invalid SHA1 hex string length");
                }

                std::array<uint8_t, SHA_DIGEST_LENGTH> hash{};
                for (size_t i = 0; i < SHA_DIGEST_LENGTH; i++)
                {
                        hash[i] = static_cast<uint8_t>(std::stoul(hexString.substr(i * 2, 2), nullptr, 16));
                }
                return hash;
        }

        class Hash_intf
        {
        public:
                virtual ~Hash_intf() {}

                virtual std::array<uint8_t, SHA_DIGEST_LENGTH> computeHash(std::span<uint8_t> data) = 0;
        };
}