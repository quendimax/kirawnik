#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>

#include "Directory.h"


/*!
  Constructs a Directory object. If <i>dirName</i> is "", then use "."
  (current directory).
  */
Directory::Directory(const char *dirName)
{
	if (dirName[0] == '\0') dirName = ".";

	m_info.setFile(dirName);
	if (!m_info.exists())
		return;

	getFileList();
}


/*!
  Constructs a Directory object as copy of <i>dir</i> object.
  */
Directory::Directory(const Directory &dir)
{
	*this = dir;
}


/*!
  Change directory to <i>newFolder</i>. <br>
  Returns <i>true</i> if the new directory exists and is readable. Else returns
  <i>false</i>.
  */
bool Directory::cd(const char *newFolder)
{
	m_info.setFile(newFolder);

	if (!m_info.exists())
		return false;

	return getFileList();
}


/*!
  Change directory by moving one directory up. <br>
  Returns <i>true</i> if the new directory exists and is readable. Else returns
  <i>false</i>.
  */
bool Directory::cdUp()
{
	return true;
}


/*!
  Makes a copy of the given FileInfo object.
  */
Directory &Directory::operator=(const Directory &dir)
{
	return *this;
}


/*!
  Returns a file name, if it exists. Otherwise returns NULL.
  */
const char *Directory::operator[](int index) const
{
	if (index < 0 || index >= m_count)
		return NULL;
	return m_files[index];
}


/*!
  Gets a file name list.
  */
bool Directory::getFileList()
{
	DIR *dir;
	struct dirent *d;

	dir = opendir(m_info.absoluteFilePath());
	if (!dir) {
		m_info.setFile("");
		return false;
	}

	m_count = 0;
	errno = 0;
	while ((d = readdir(dir)) != NULL) {
		if (m_count % 4 == 0) {
			m_files = (char **) realloc(m_files, (m_count + 4) * sizeof(char **));
		}
		m_files[m_count] = strdup(d->d_name);
		m_count++;
		errno = 0;
	}

	if (errno) {
		perror("readdir");
		closedir(dir);
		return false;
	}

	closedir(dir);
	return true;
}
