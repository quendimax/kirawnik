#include <gtest/gtest.h>
#include <fs/FileInfo.h>


TEST(FileInfo, fileName)
{
	FileInfo i("/");
	ASSERT_STREQ("/", i.fileName());
	i.setFile("/bin");
	ASSERT_STREQ("bin", i.fileName());
	i.setFile("/bin/");
	ASSERT_STREQ("bin", i.fileName());
	i.setFile("/usr/bin");
	ASSERT_STREQ("bin", i.fileName());
	i.setFile("/usr/bin/");
	ASSERT_STREQ("bin", i.fileName());
	i.setFile("/usr/bin/gcc");
	ASSERT_STREQ("gcc", i.fileName());
	i.setFile("../test.data");
	ASSERT_STREQ("test.data", i.fileName());
	i.setFile("../test.data/");
	ASSERT_STREQ("test.data", i.fileName());
	i.setFile("../test.data/file.suffix");
	ASSERT_STREQ("file.suffix", i.fileName());
	i.setFile("../test.data/file.suf.fix");
	ASSERT_STREQ("file.suf.fix", i.fileName());
	i.setFile("../test.data/.hidefile");
	ASSERT_STREQ(".hidefile", i.fileName());
	i.setFile("../test.data/.hidefile.suffix");
	ASSERT_STREQ(".hidefile.suffix", i.fileName());
}


TEST(FileInfo, absoluteFilePath)
{
	FileInfo i("/");
	ASSERT_STREQ("/", i.absoluteFilePath());
	i.setFile("/bin");
	ASSERT_STREQ("/bin", i.absoluteFilePath());
	i.setFile("/bin/");
	ASSERT_STREQ("/bin", i.absoluteFilePath());
	i.setFile("/usr/bin");
	ASSERT_STREQ("/usr/bin", i.absoluteFilePath());
	i.setFile("/usr/bin/");
	ASSERT_STREQ("/usr/bin", i.absoluteFilePath());
	i.setFile("/usr/bin/gcc");
	ASSERT_STREQ("/usr/bin/gcc", i.absoluteFilePath());
}


TEST(FileInfo, absolutePath)
{
	FileInfo i("/");
	ASSERT_STREQ("/", i.absolutePath());
	i.setFile("/bin");
	ASSERT_STREQ("/", i.absolutePath());
	i.setFile("/bin/");
	ASSERT_STREQ("/", i.absolutePath());
	i.setFile("/usr/bin");
	ASSERT_STREQ("/usr/", i.absolutePath());
	i.setFile("/usr/bin/");
	ASSERT_STREQ("/usr/", i.absolutePath());
	i.setFile("/usr/bin/gcc");
	ASSERT_STREQ("/usr/bin/", i.absolutePath());
}


TEST(FileInfo, suffix)
{
	FileInfo i("/");
	ASSERT_STREQ("", i.suffix());
	i.setFile("/bin");
	ASSERT_STREQ("", i.suffix());
	i.setFile("/bin/");
	ASSERT_STREQ("", i.suffix());
	i.setFile("/usr/bin");
	ASSERT_STREQ("", i.suffix());
	i.setFile("/usr/bin/");
	ASSERT_STREQ("", i.suffix());
	i.setFile("/usr/bin/gcc");
	ASSERT_STREQ("", i.suffix());
	i.setFile("../test.data");
	ASSERT_STREQ("", i.suffix());
	i.setFile("../test.data/");
	ASSERT_STREQ("", i.suffix());
	i.setFile("../test.data/file.suffix");
	ASSERT_STREQ("suffix", i.suffix());
	i.setFile("../test.data/file.suf.fix");
	ASSERT_STREQ("suf.fix", i.suffix());
	i.setFile("../test.data/.hidefile");
	ASSERT_STREQ("", i.suffix());
	i.setFile("../test.data/.hidefile.suffix");
	ASSERT_STREQ("suffix", i.suffix());
}


TEST(FileInfo, baseName)
{
	FileInfo i("/");
	ASSERT_STREQ("/", i.baseName());
	i.setFile("/bin");
	ASSERT_STREQ("bin", i.baseName());
	i.setFile("/bin/");
	ASSERT_STREQ("bin", i.baseName());
	i.setFile("/usr/bin");
	ASSERT_STREQ("bin", i.baseName());
	i.setFile("/usr/bin/");
	ASSERT_STREQ("bin", i.baseName());
	i.setFile("/usr/bin/gcc");
	ASSERT_STREQ("gcc", i.baseName());
	i.setFile("../test.data");
	ASSERT_STREQ("test.data", i.baseName());
	i.setFile("../test.data/");
	ASSERT_STREQ("test.data", i.baseName());
	i.setFile("../test.data/file.suffix");
	ASSERT_STREQ("file", i.baseName());
	i.setFile("../test.data/file.suf.fix");
	ASSERT_STREQ("file", i.baseName());
	i.setFile("../test.data/.hidefile");
	ASSERT_STREQ(".hidefile", i.baseName());
	i.setFile("../test.data/.hidefile.suffix");
	ASSERT_STREQ(".hidefile", i.baseName());
}


TEST(FileInfo, size)
{
	FileInfo info("../test.data/file0bytes");
	ASSERT_TRUE(info.exists());
	ASSERT_EQ(0, info.size());

	FileInfo info2("../test.data/file14bytes");
	ASSERT_TRUE(info2.exists());
	ASSERT_EQ(14, info2.size());
}
