#include <gtest/gtest.h>
#include <fs/FileInfo.h>


TEST(FileInfoTest, FileNames)
{
	{
	FileInfo i("/");
	ASSERT_STREQ("/", i.fileName());
	ASSERT_STREQ("/", i.absoluteFilePath());
	ASSERT_STREQ("/", i.absolutePath());
	ASSERT_TRUE(i.suffix() != 0);
	ASSERT_STREQ("", i.suffix());
	ASSERT_STREQ("/", i.baseName());
	}
	{
	FileInfo i("/bin");
	ASSERT_STREQ("bin", i.fileName());
	ASSERT_STREQ("/bin", i.absoluteFilePath());
	ASSERT_STREQ("/", i.absolutePath());
	ASSERT_STREQ("", i.suffix());
	ASSERT_STREQ("bin", i.baseName());
	}
	{
	FileInfo i("/bin/");
	ASSERT_STREQ("bin", i.fileName());
	ASSERT_STREQ("/bin", i.absoluteFilePath());
	ASSERT_STREQ("/", i.absolutePath());
	ASSERT_STREQ("", i.suffix());
	ASSERT_STREQ("bin", i.baseName());
	}
	{
	FileInfo i("/usr/bin/gcc");
	ASSERT_STREQ("gcc", i.fileName());
	ASSERT_STREQ("/usr/bin/gcc", i.absoluteFilePath());
	ASSERT_STREQ("/usr/bin/", i.absolutePath());
	ASSERT_STREQ("", i.suffix());
	ASSERT_STREQ("gcc", i.baseName());
	}
	{
	FileInfo i("/usr/bin/");
	ASSERT_STREQ("bin", i.fileName());
	ASSERT_STREQ("/usr/bin", i.absoluteFilePath());
	ASSERT_STREQ("/usr/", i.absolutePath());
	ASSERT_STREQ("", i.suffix());
	ASSERT_STREQ("bin", i.baseName());
	}
}


TEST(FileInfoTest, FileSize)
{
	FileInfo info("../data/file0bytes");
	ASSERT_TRUE(info.exists());
	ASSERT_EQ(0, info.size());

	FileInfo info2("../data/file14bytes");
	ASSERT_TRUE(info2.exists());
	ASSERT_EQ(14, info2.size());
}

