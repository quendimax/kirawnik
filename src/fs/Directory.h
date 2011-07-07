#ifndef __FS_DIRECTORY_H__
#define __FS_DIRECTORY_H__

#include "FileInfo.h"


/*!
  The Directory class contains list of dir's files. The names get with
  operator [].
  */
class Directory
{
public:
	Directory(const char *dirName = "");
	Directory(const Directory &);
	~Directory();

	const FileInfo &fileInfo() const;
	int count() const;
	bool exists() const;

	bool cd(const char *newFolder);
	bool cdUp();
	const char *dirName() const;

	Directory &operator=(const Directory &);
	const char *operator[](int index) const;

private:
	bool getFileList();

private:
	char **m_files;
	int m_count;
	FileInfo m_info;
};


/*! Returns the FileInfo object about the Directory object. */
inline const FileInfo &Directory::fileInfo() const { return m_info; }

/*! Returns count of files in the directory. */
inline int Directory::count() const { return m_count; }

/*! Returns <i>true</i> if directory exists. Else returns <i>false</i>. */
inline bool Directory::exists() const { return m_info.exists(); }


/*!
  Returns short name of directory. For getting full name see the FileInfo
  class.
  \see fileInfo()
  */
inline const char *Directory::dirName() const { return m_info.fileName(); }


#endif //__FS_DIRECTORY_H__
