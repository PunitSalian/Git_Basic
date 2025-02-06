#include "gtest/gtest.h"
#include "FileManagerInterface.h"
#include "FileManager.h"
#include <iostream>


TEST(FileCreate, FileCreateSuccessAndRemove)
{
    storage::FileManager<std::filesystem::path> f;
    std::filesystem::path file = "/tmp/output.txt";
    f.createFile(file);
    // getFilesize should check the existance of file and return 0
    EXPECT_EQ(f.getFilesize(file).value(), 0);

    f.removeFile(file);

    EXPECT_EQ(f.getFilesize(file).is_err(), true);

}

TEST(DirectoryCreate, DirectoryCreateSuccessAndRemove)
{
    storage::FileManager<std::filesystem::path> f;
    std::filesystem::path dir = "/tmp/folder";
    EXPECT_EQ(f.createDirectory(dir).is_err(), false);
    EXPECT_EQ(std::filesystem::exists(dir) && std::filesystem::is_directory(dir), true);

    EXPECT_EQ(f.removeDirectory(dir).is_err(), false);

    EXPECT_EQ(std::filesystem::exists(dir), false);

}

TEST(FileReadWrite, WriteAndReadFile)
{
    storage::FileManager<std::filesystem::path> f;
    std::filesystem::path file = "/tmp/output.txt";
    f.createFile(file);
    // getFilesize should check the existance of file and return 0
    EXPECT_EQ(f.getFilesize(file).value(), 0);

    std::vector<uint8_t> wr = {1,2,3};
    f.writeFile(file, wr);

    EXPECT_EQ(f.getFilesize(file).value(), 3);

    std::vector<uint8_t> rb;

    rb.resize(f.getFilesize(file).value());

    EXPECT_EQ(f.readFile(file, rb).is_err(), false);

    EXPECT_EQ(wr, rb);
}