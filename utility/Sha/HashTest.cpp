#include "Sha1.h"
#include "gtest/gtest.h"

TEST(HashTest, Sha1algoTest)
{
    util::Sha1 cryp;
    std::string str = "Hello World";
    std::vector<uint8_t> vec(str.begin(), str.end());

    auto res = cryp.computeHash(vec);

    EXPECT_EQ(res, "0a4d55a8d778e5022fab701977c5d840bbc486d0");
}
