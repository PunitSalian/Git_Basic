#include "Sha1.h"
#include <openssl/sha.h>

std::array<uint8_t, SHA_DIGEST_LENGTH> util::Sha1::computeHash(std::span<uint8_t> data)
{
    std::array<uint8_t, SHA_DIGEST_LENGTH> hash{};
    SHA1(reinterpret_cast<const unsigned char *>(data.data()), data.size(), hash.data());
    return hash;
}