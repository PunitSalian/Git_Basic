
#include <filesystem>
#include "Blob.h"
#include "gtest/gtest.h"
#include "utility/Sha/HashTest.h"
#include "utility/Compression/ZlibTest.h"
#include "storage/FileManager/FileManagerTest.h"
#include "config/GitConfig.h"

TEST(BlobTest, BlobObjectStore)
{
    test::MockHash h;
    test::MockFileManager<std::filesystem::path> f;
    test::MockZlib z;
    git::GitConfig &g = git::GitConfig::getGitConfigInstance();
    std::string dir = "/tmp/";
    g.setGitDir(std::move(dir));

    storage::Blob b(h, f, g, z);
    std::vector<uint8_t> vec = {'H', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd'};

    EXPECT_CALL(h, computeHash(testing::ElementsAre('b', 'l', 'o', 'b', ' ', '1', '1', '\0', 'H', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd')))
        .WillOnce(testing::Return("2ef7bde608ce5404e97d5f042f95f89f1c232871"));

    EXPECT_CALL(f, createDirectory(testing::StrEq("/tmp/2e"))).WillOnce(testing::Return(std::monostate()));

    EXPECT_CALL(f, createFile(testing::StrEq("/tmp/2e/f7bde608ce5404e97d5f042f95f89f1c232871"))).WillOnce(testing::Return(std::monostate()));

    EXPECT_CALL(z, compress(testing::ElementsAre('H', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd'))).WillOnce(testing::Return(std::vector<uint8_t>{1, 2, 3}));

    EXPECT_CALL(f, writeFile(testing::StrEq("/tmp/2e/f7bde608ce5404e97d5f042f95f89f1c232871"), testing::ElementsAre(1, 2, 3))).WillOnce(testing::Return(5));

    auto res = b.storeObject(std::move(vec), storage::Objecttype::BLOB);

    EXPECT_EQ(res.value(), "2ef7bde608ce5404e97d5f042f95f89f1c232871");
}

TEST(BlobTest, CommitObjectStore)
{
    test::MockHash h;
    test::MockFileManager<std::filesystem::path> f;
    test::MockZlib z;
    git::GitConfig &g = git::GitConfig::getGitConfigInstance();
    std::string dir = "/tmp/";
    g.setGitDir(std::move(dir));

    storage::Blob b(h, f, g, z);
    std::vector<uint8_t> vec = {'H', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd'};

    EXPECT_CALL(h, computeHash(testing::ElementsAre('c', 'o', 'm', 'm', 'i', 't', ' ', '1', '1', '\0', 'H', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd')))
        .WillOnce(testing::Return("2ef7bde608ce5404e97d5f042f95f89f1c232871"));

    EXPECT_CALL(f, createDirectory(testing::StrEq("/tmp/2e"))).WillOnce(testing::Return(std::monostate()));

    EXPECT_CALL(f, createFile(testing::StrEq("/tmp/2e/f7bde608ce5404e97d5f042f95f89f1c232871"))).WillOnce(testing::Return(std::monostate()));

    EXPECT_CALL(f, writeFile(testing::StrEq("/tmp/2e/f7bde608ce5404e97d5f042f95f89f1c232871"), testing::ElementsAre('c', 'o', 'm', 'm', 'i', 't', ' ', '1', '1', '\0', 'H', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd'))).WillOnce(testing::Return(5));

    auto res = b.storeObject(std::move(vec), storage::Objecttype::COMMIT);

    EXPECT_EQ(res.value(), "2ef7bde608ce5404e97d5f042f95f89f1c232871");
}

TEST(BlobTest, TreeObjectStore)
{
    test::MockHash h;
    test::MockFileManager<std::filesystem::path> f;
    test::MockZlib z;
    git::GitConfig &g = git::GitConfig::getGitConfigInstance();
    std::string dir = "/tmp/";
    g.setGitDir(std::move(dir));

    storage::Blob b(h, f, g, z);
    std::vector<uint8_t> vec = {'H', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd'};

    EXPECT_CALL(h, computeHash(testing::ElementsAre('t', 'r', 'e', 'e', ' ', '1', '1', '\0', 'H', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd')))
        .WillOnce(testing::Return("2ef7bde608ce5404e97d5f042f95f89f1c232871"));

    EXPECT_CALL(f, createDirectory(testing::StrEq("/tmp/2e"))).WillOnce(testing::Return(std::monostate()));

    EXPECT_CALL(f, createFile(testing::StrEq("/tmp/2e/f7bde608ce5404e97d5f042f95f89f1c232871"))).WillOnce(testing::Return(std::monostate()));

    EXPECT_CALL(f, writeFile(testing::StrEq("/tmp/2e/f7bde608ce5404e97d5f042f95f89f1c232871"), testing::ElementsAre('t', 'r', 'e', 'e', ' ', '1', '1', '\0', 'H', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd'))).WillOnce(testing::Return(5));

    auto res = b.storeObject(std::move(vec), storage::Objecttype::TREE);

    EXPECT_EQ(res.value(), "2ef7bde608ce5404e97d5f042f95f89f1c232871");
}

TEST(BlobTest, GetBlobObjectStore)
{
    test::MockHash h;
    test::MockFileManager<std::filesystem::path> f;
    test::MockZlib z;
    git::GitConfig &g = git::GitConfig::getGitConfigInstance();
    std::string dir = "/tmp/";
    g.setGitDir(std::move(dir));

    // Sample data to be read
    std::vector<uint8_t> mockData = {'b', 'l', 'o', 'b', ' ', '1', '1', '\0', 11, 0, 0, 0, 0, 0, 0, 0, 'H', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd'}; // "Hello world"

    storage::Blob b(h, f, g, z);
    EXPECT_CALL(f, getFilesize(testing::StrEq("/tmp/2e/f7bde608ce5404e97d5f042f95f89f1c232871"))).WillOnce(testing::Return(mockData.size()));

    EXPECT_CALL(f, readFile(testing::StrEq("/tmp/2e/f7bde608ce5404e97d5f042f95f89f1c232871"), testing::_)).WillOnce([&](const std::string &, std::span<uint8_t> data)
                                                                                                                    {
            if (data.size() >= mockData.size()) {
                std::copy(mockData.begin(), mockData.end(), data.begin());
                return Result<std::monostate, ErrorCode>(std::monostate{}); // Success
            } else {
                return Result<std::monostate, ErrorCode>(ErrorCode::InternalError); // Failure
            } });

    EXPECT_CALL(z, decompress(testing::ElementsAre(11, 0, 0, 0, 0, 0, 0, 0, 'H', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd'))).WillOnce(testing::Return(std::vector<uint8_t>{1, 2, 3}));

    const std::string hash = "2ef7bde608ce5404e97d5f042f95f89f1c232871";
    auto res = b.getObject(hash);

    EXPECT_EQ(res.value().first, storage::Objecttype::BLOB);
    std::vector<uint8_t> expected = {1, 2, 3};
    EXPECT_EQ(res.value().second, expected);
}

TEST(BlobTest, GetCommitObjectStore)
{
    test::MockHash h;
    test::MockFileManager<std::filesystem::path> f;
    test::MockZlib z;
    git::GitConfig &g = git::GitConfig::getGitConfigInstance();
    std::string dir = "/tmp/";
    g.setGitDir(std::move(dir));

    // Sample data to be read
    std::vector<uint8_t> mockData = {'c', 'o', 'm', 'm', 'i', 't', ' ', '1', '1', '\0', 'H', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd'}; // "Hello world"

    storage::Blob b(h, f, g, z);
    EXPECT_CALL(f, getFilesize(testing::StrEq("/tmp/2e/f7bde608ce5404e97d5f042f95f89f1c232871"))).WillOnce(testing::Return(mockData.size()));

    EXPECT_CALL(f, readFile(testing::StrEq("/tmp/2e/f7bde608ce5404e97d5f042f95f89f1c232871"), testing::_)).WillOnce([&](const std::string &, std::span<uint8_t> data)
                                                                                                                    {
            if (data.size() >= mockData.size()) {
                std::copy(mockData.begin(), mockData.end(), data.begin());
                return Result<std::monostate, ErrorCode>(std::monostate{}); // Success
            } else {
                return Result<std::monostate, ErrorCode>(ErrorCode::InternalError); // Failure
            } });

    const std::string hash = "2ef7bde608ce5404e97d5f042f95f89f1c232871";
    auto res = b.getObject(hash);

    EXPECT_EQ(res.value().first, storage::Objecttype::COMMIT);
    std::vector<uint8_t> expected = {'H', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd'};
    EXPECT_EQ(res.value().second, expected);
}

TEST(BlobTest, GetTreeObjectStore)
{
    test::MockHash h;
    test::MockFileManager<std::filesystem::path> f;
    test::MockZlib z;
    git::GitConfig &g = git::GitConfig::getGitConfigInstance();
    std::string dir = "/tmp/";
    g.setGitDir(std::move(dir));

    // Sample data to be read
    std::vector<uint8_t> mockData = {'t', 'r', 'e', 'e', ' ', '1', '1', '\0', 'H', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd'}; // "Hello world"

    storage::Blob b(h, f, g, z);
    EXPECT_CALL(f, getFilesize(testing::StrEq("/tmp/2e/f7bde608ce5404e97d5f042f95f89f1c232871"))).WillOnce(testing::Return(mockData.size()));

    EXPECT_CALL(f, readFile(testing::StrEq("/tmp/2e/f7bde608ce5404e97d5f042f95f89f1c232871"), testing::_)).WillOnce([&](const std::string &, std::span<uint8_t> data)
                                                                                                                    {
            if (data.size() >= mockData.size()) {
                std::copy(mockData.begin(), mockData.end(), data.begin());
                return Result<std::monostate, ErrorCode>(std::monostate{}); // Success
            } else {
                return Result<std::monostate, ErrorCode>(ErrorCode::InternalError); // Failure
            } });

    const std::string hash = "2ef7bde608ce5404e97d5f042f95f89f1c232871";
    auto res = b.getObject(hash);

    EXPECT_EQ(res.value().first, storage::Objecttype::TREE);
    std::vector<uint8_t> expected = {'H', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd'};
    EXPECT_EQ(res.value().second, expected);
}