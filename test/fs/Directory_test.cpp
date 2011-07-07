#include <gtest/gtest.h>
#include <fs/Directory.h>


TEST(Directory, exists)
{
	Directory dir("/");
	ASSERT_TRUE(dir.exists());
	dir.cd("/bin");
	ASSERT_TRUE(dir.exists());
	dir.cd("..");
	ASSERT_TRUE(dir.exists());
	dir.cd("/bin");
	dir.cdUp();
	ASSERT_TRUE(dir.exists());
	dir.cd("/asdgdfgh");
	ASSERT_FALSE(dir.exists());
/*	dir.cd("../test.data");
	ASSERT_TRUE(dir.exists());
	dir.cd("../test.data/file.suffix");
	ASSERT_FALSE(dir.exists());*/
}
