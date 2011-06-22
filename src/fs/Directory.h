#ifndef __FS_DIRECTORY_H__
#define __FS_DIRECTORY_H__

#include "FileInfo.h"


class Directory
{
	Directory();
	Directory(const Directory &);
	explicit Directory(const char *dirName);
	~Directory();

	const FileInfo &fileInfo() const;
	int count() const;

	bool cd(const char *newFolder);
	bool cdUp();
	const char *dirName() const;

	Directory &operator=(const Directory &);
	const char *operator[](int index) const;

};


#endif //__FS_DIRECTORY_H__
