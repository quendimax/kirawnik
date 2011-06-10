#ifndef __FILEINFO_H__
#define __FILEINFO_H__

#include <sys/stat.h>
#include <stdio.h>


class FileInfo
{
public:
	explicit FileInfo();
	explicit FileInfo(const char *fileName);
	explicit FileInfo(int file);
	explicit FileInfo(FILE *file);
	virtual ~FileInfo();

	void setFile(const char *fileName);
	void setFile(int file);
	void setFile(FILE *file);

	const char *fileName() const;
	const char *absoluteName() const;
	const char *path() const;
	const char *suffix() const;
	const char *baseName() const;
	int permissions() const;
	const char *owner() const;
	int ownerId() const;
	const char *group() const;
	int groupId() const;
	long long size() const;
	bool exists() const;
	time_t created() const;
	time_t lastModified() const;
	time_t lastReading() const;

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


inline FileInfo::FileInfo() { clearForConstructor(); }

inline FileInfo::FileInfo(const char *fileName)
{
	clearForConstructor();
	setFile(fileName);
}

inline FileInfo::FileInfo(int file)
{
	clearForConstructor();
	setFile(file);
}

inline FileInfo::FileInfo(FILE *file)
{
	clearForConstructor();
	setFile(file);
}

inline FileInfo::~FileInfo() { clear(); }

inline const char *FileInfo::fileName() const { return m_fileName; }
inline const char *FileInfo::absoluteName() const { return m_fullName; }
inline const char *FileInfo::path() const { return m_path; }
inline const char *FileInfo::suffix() const { return m_suffix; }
inline const char *FileInfo::baseName() const { return m_baseName; }
inline int FileInfo::permissions() const { return (int) m_stat.st_mode & 0777; }
inline int FileInfo::ownerId() const { return (int) m_stat.st_uid; }
inline int FileInfo::groupId() const { return (int) m_stat.st_gid; }
inline long long FileInfo::size() const { return (long long) m_stat.st_size; }
inline bool FileInfo::exists() const { return m_exists; }
inline time_t FileInfo::created() const { return m_stat.st_ctime; }
inline time_t FileInfo::lastModified() const { return m_stat.st_mtime; }
inline time_t FileInfo::lastReading() const { return m_stat.st_atime; }

inline bool FileInfo::isDir() const { return m_stat.st_mode & S_IFDIR; }
inline bool FileInfo::isRegularFile() const{ return m_stat.st_mode & S_IFREG; }
inline bool FileInfo::isSymLink() const { return m_stat.st_mode & S_IFLNK; }
inline bool FileInfo::isSocket() const { return m_stat.st_mode & S_IFSOCK; }
inline bool FileInfo::isFifo() const { return m_stat.st_mode & S_IFIFO; }
inline bool FileInfo::isBlockSpecFile() const { return m_stat.st_mode & S_IFBLK; }
inline bool FileInfo::isCharSpecFile() const { return m_stat.st_mode & S_IFCHR; }
inline bool FileInfo::isSpecFile() const { return isBlockSpecFile() || isCharSpecFile(); }


#endif //__FILEINFO_H__
