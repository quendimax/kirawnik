#include <gtest/gtest.h>
#include <fs/FileInfo.h>


TEST(FileInfoTest, FileSize)
{
	FileInfo info("file0size");
	EXPECT_EQ(0, info.size());
}

