#include "gtest/gtest.h"
#include "Zlib.h"

TEST(ZlibTest, CompressDecompressTest)
{
    util::Zlib handle;
    std::vector<uint8_t> input = {1, 2, 3, 4, 5};

    Result<std::vector<uint8_t>, ErrorCode> res = handle.compress(input);

    std::cout << std::endl;
    Result<std::vector<uint8_t>, ErrorCode> d = handle.decompress(res.value());

    EXPECT_EQ(input, d.value());
}
