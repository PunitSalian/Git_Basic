#include "gtest/gtest.h"
#include "FileManagerInterface.h"
#include "FileManager.h"


TEST(FileCreate, FileCreateSuccess)
{
    storage::FileManager<std::filesystem::path> f;
    std::filesystem::path file = "/tmp/output.txt";
    f.createFile(file);
    // getFilesize should check the existance of file and return 0
    EXPECT_EQ(f.getFilesize(file).value(), 0);
}
