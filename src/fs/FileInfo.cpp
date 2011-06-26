#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <sys/types.h>
#include <pwd.h>
#include <grp.h>

#include "FileInfo.h"


/*!
  Constructs a FileInfo object that is a copy the FileInfo object
  <i>fileInfo</i>
  */
FileInfo::FileInfo(const FileInfo &fileInfo)
{
	clearForConstructor();
	*this = fileInfo;
}


/*!
  Sets the file which the FileInfo object provides. <br>
  If the object is not empty then old information deletes and creates
  information about new file.
  */
void FileInfo::setFile(const char *fileName)
{
	assert(fileName > 0);
	assert(strlen(fileName) > 0);

	clear();

	if (access(fileName, F_OK)) return;
	else m_exists = true;

	// lstat must be call befor createFileName!!!
	// becouse createFileName uses m_stat
	lstat(fileName, &m_stat);

	createFileName(fileName);
}


/*!
  Returns owner's name. <br>
  Note, first time the call of this method is slowly.
  */
const char *FileInfo::owner() const
{
	if (m_owner == NULL) {
		struct passwd *pw;
		pw = getpwuid(m_stat.st_uid);
		int ownerLen = strlen(pw->pw_name);
		m_owner = (char *) malloc(ownerLen + 1);
		strcpy(m_owner, pw->pw_name);
	}

	return m_owner;
}


/*!
  Returns group's name. <br>
  Note, first time the call of this method is slowly.
  */
const char *FileInfo::group() const
{
	if (m_group == NULL) {
		struct group *gr;
		gr = getgrgid(m_stat.st_gid);
		int groupLen = strlen(gr->gr_name);
		m_group = (char *) malloc(groupLen + 1);
		strcpy(m_group, gr->gr_name);
	}

	return m_group;
}


/*!
  Makes a copy of the given FileInfo object.
  */
FileInfo &FileInfo::operator= (const FileInfo &item)
{
	m_exists = item.m_exists;
	m_stat = item.m_stat;

	m_owner = item.m_owner ? strdup(item.m_owner) : 0;
	m_group = item.m_group ? strdup(item.m_group) : 0;

	m_fullName = strdup(item.m_fullName);
	m_baseName = strdup(item.m_baseName);
	m_path = strdup(item.m_path);
	m_suffix = m_fullName + (item.m_suffix - item.m_fullName);
	m_fileName = m_fullName + (item.m_fileName - item.m_fullName);

	return *this;
}


/*!
  Clears the FileInfo object and frees resources.
  */
void FileInfo::clear()
{
	if (m_owner) free(m_owner);
	if (m_group) free(m_group);
	if (m_fullName) free(m_fullName);
	if (m_path) free(m_path);
	if (m_baseName) free(m_baseName);

	m_exists = false;
	m_fileName = NULL;
	m_suffix = NULL;
	m_fullName = NULL;
	m_baseName = NULL;
	m_path = NULL;
	m_owner = NULL;
	m_group = NULL;
	memset(&m_stat, 0, sizeof m_stat);
}


/*!
  Clear the FileInfo object but doesn't free resources.
  \note This method must be called in costructor only!
  */
void FileInfo::clearForConstructor()
{
	m_exists = false;
	memset(&m_stat, 0, sizeof m_stat);
	m_fullName = NULL;
	m_fileName = NULL;
	m_suffix = NULL;
	m_path = NULL;
	m_baseName = NULL;
	m_owner = NULL;
	m_group = NULL;
}


/*!
  Creates file name, base name, suffix, path and others.

  If <i>name</i> is relative path name then uses current path.
  */
void FileInfo::createFileName(const char *name)
{
	// create absolute name
	if (name[0] != '/') {
		char *cwd = getcwd(NULL, 0);
		int cwdLen = strlen(cwd);
		int nameLen = strlen(name);

		if (cwd[cwdLen-1] == '/') {
			m_fullName = (char *) malloc(cwdLen + nameLen + 1);
			strcpy(m_fullName, cwd);
			strcpy(m_fullName + cwdLen, name);
		}
		else {
			m_fullName = (char *) malloc(cwdLen + nameLen + 2);
			strcpy(m_fullName, cwd);
			m_fullName[cwdLen] = '/';
			strcpy(m_fullName + cwdLen + 1, name);
		}

		free(cwd);
	}
	else {
		int nameLen = strlen(name);
		m_fullName = (char *) malloc(nameLen + 1);
		strcpy(m_fullName, name);
	}
	int fullNameLen = strlen(m_fullName);
	if (fullNameLen > 1 && m_fullName[fullNameLen - 1] == '/')
		m_fullName[fullNameLen - 1] = '\0';

	// create file name
	char *p = m_fullName + fullNameLen - 1;
	if (*p == '/')
		p--;

	while (p >= m_fullName) {
		if (*p == '/')
			break;
		p--;
	}
	m_fileName = p + 1;

	// create suffix
	if (isDir()) {
		m_suffix = m_fullName + fullNameLen;
	}
	else {
		p = m_fileName + 1;					// +1 for files which begin from '.'
		while (*p != '.' && *p != '\0')
			p++;
		if (*p == '.')
			m_suffix = p + 1;
		else
			m_suffix = m_fullName + fullNameLen;
	}
	int suffixLen = strlen(m_suffix);

	// create base name
	int baseLen = (m_suffix - m_fileName) - (suffixLen == 0 ? 0 : 1);
	if (baseLen == 0) baseLen = 1;
	m_baseName = (char *) malloc(baseLen + 1);
	memcpy(m_baseName, m_fileName, baseLen);
	m_baseName[baseLen] = '\0';

	// create absolute path
	int pathLen = m_fileName - m_fullName;
	if (pathLen == 0)
		pathLen = 1;
	m_path = (char *) malloc(pathLen + 1);
	memcpy(m_path, m_fullName, pathLen);
	m_path[pathLen] = '\0';
}
