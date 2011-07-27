#ifndef __FS_FILEINFO_H__
#define __FS_FILEINFO_H__

#include <sys/stat.h>


/*!
  The FileInfo class provides file system information about a file.
  */
class FileInfo
{
public:
	FileInfo();
	FileInfo(const FileInfo &);
	explicit FileInfo(const char *fileName);
	~FileInfo();

	void setFile(const char *fileName);

	const char *fileName() const;
	const char *absoluteFilePath() const;
	const char *absolutePath() const;
	const char *suffix() const;
	const char *baseName() const;
	int permissions() const;
	const char *owner() const;
	unsigned int ownerId() const;
	const char *group() const;
	unsigned int groupId() const;
	long long size() const;
	bool exists() const;
	time_t created() const;
	time_t lastModified() const;
	time_t lastRead() const;

	bool isDir() const;
	bool isRegularFile() const;
	bool isSymLink() const;
	bool isSocket() const;
	bool isFifo() const;
	bool isSpecFile() const;
	bool isBlockSpecFile() const;
	bool isCharSpecFile() const;

	FileInfo &operator= (const FileInfo &item);

private:
	void clear();
	void clearForConstructor();
	void createFileName(const char *name);

private:
	struct stat m_stat;
	char *m_fullName;
	char *m_fileName;
	char *m_suffix;
	char *m_baseName;
	char *m_path;
	mutable char *m_owner;
	mutable char *m_group;
	bool m_exists;
};


/*!
  Constructs an empty FileInfo object.
  */
inline FileInfo::FileInfo() { clearForConstructor(); }


/*!
  Constructs a new FileInfo object. <i>file</i> is absolute or relative path.
  */
inline FileInfo::FileInfo(const char *fileName)
{
	clearForConstructor();
	setFile(fileName);
}


/*!
  Destroys the FileInfo object and frees its resources.
  */
inline FileInfo::~FileInfo() { clear(); }


/*!
  Returns the file name without the path.
  */
inline const char *FileInfo::fileName() const { return m_fileName; }


/*!
  Returns the absolute file path.
  */
inline const char *FileInfo::absoluteFilePath() const { return m_fullName; }


/*!
  Returns the absolute path without the file name.
  */
inline const char *FileInfo::absolutePath() const { return m_path; }


/*!
  Returns the suffix of the file. Directories don't contain a suffix.

  If file name is "archive.tar.gz" then the method returns "tar.gz".
  If file name is ".bashrc" then the method returns "".

  \see baseName()
  */
inline const char *FileInfo::suffix() const { return m_suffix; }

/*!
  Returns the base file name of the file, i.e. file name without a suffix.

  If file name is "archive.tar.gz" then base name is "archive". But if file
  name is ".bashrc" then base name is ".bashrc".

  \see suffix(), fileName()
  */
inline const char *FileInfo::baseName() const { return m_baseName; }


/*!
  Returns permissions of the file. Permissions is 9 bits in format "rwxrwxrwx".
  */
inline int FileInfo::permissions() const { return (int) m_stat.st_mode & 0777; }


/*! Returns the owner id of the file. */
inline unsigned int FileInfo::ownerId() const { return (int) m_stat.st_uid; }


/*! Returns the group id of the file. */
inline unsigned int FileInfo::groupId() const { return (int) m_stat.st_gid; }


/*! Returns the size of the file (in bytes). */
inline long long FileInfo::size() const { return (long long) m_stat.st_size; }


/*!
  Returns <i>true</i> if the file exists, but <i>false</i> if the file doesn't
  exists or the FileInfo object is empty.
  */
inline bool FileInfo::exists() const { return m_exists; }


/*! Returns the time where the file was create. */
inline time_t FileInfo::created() const { return m_stat.st_ctime; }

/*! Returns the time where the file was modify. */
inline time_t FileInfo::lastModified() const { return m_stat.st_mtime; }

/*! Returns the time where the file was read. */
inline time_t FileInfo::lastRead() const { return m_stat.st_atime; }


/*! Returns <i>true</i> if the file is a directory. */
inline bool FileInfo::isDir() const { return m_stat.st_mode & S_IFDIR; }

/*! Returns <i>true</i> if the file is a regular file. */
inline bool FileInfo::isRegularFile() const{ return m_stat.st_mode & S_IFREG; }

/*! Returns <i>true</i> if the file is a symbolic link. */
inline bool FileInfo::isSymLink() const { return m_stat.st_mode & S_IFLNK; }

/*! Returns <i>true</i> if the file is a socket. */
inline bool FileInfo::isSocket() const { return m_stat.st_mode & S_IFSOCK; }

/*! Returns <i>true</i> if the file is a fifo. */
inline bool FileInfo::isFifo() const { return m_stat.st_mode & S_IFIFO; }

/*! Returns <i>true</i> if the file is a block special file. */
inline bool FileInfo::isBlockSpecFile() const { return m_stat.st_mode & S_IFBLK; }

/*! Returns <i>true</i> if the file is a character spesial file. */
inline bool FileInfo::isCharSpecFile() const { return m_stat.st_mode & S_IFCHR; }

/*! Returns <i>true</i> if the file is a special file. */
inline bool FileInfo::isSpecFile() const { return isBlockSpecFile() || isCharSpecFile(); }


#endif //__FS_FILEINFO_H__
