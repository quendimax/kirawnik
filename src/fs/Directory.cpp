#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>

#include "Directory.h"


/*!
  Constructs a Directory object. If <i>dirName</i> is "", then use "."
  (current directory).
  */
Directory::Directory(const char *dirName)
    : m_files(0)
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


/*! Clears a dynamic data. */
Directory::~Directory()
{
	clear();
}


/*!
  Change directory to <i>newFolder</i>. <br>
  Returns <i>true</i> if the new directory exists and is readable. Else returns
  <i>false</i>.
  */
bool Directory::cd(const char *newFolder)
{
	clear();

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
	char *fileName = strdup(m_info.absoluteFilePath());
	int i = strlen(fileName);

	while (i > 0 && fileName[i] == '/')
		i--;
	while (i > 0 && fileName[i] != '/')
		i--;
	fileName[i+1] = '\0';

	clear();

	m_info.setFile(fileName);
	if (!m_info.exists())
		return false;

	return getFileList();
}


/*!
  Makes a copy of the given FileInfo object.
  */
Directory &Directory::operator=(const Directory &dir)
{
	this->m_count = dir.m_count;
	this->m_info = dir.m_info;

	m_files = (char **) malloc(sizeof (char *) * m_count);
	for (int i = 0; i < m_count; i++)
		m_files[i] = strdup(dir.m_files[i]);

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
	m_files = 0;
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


/*! Clears a dynamic data. */
void Directory::clear()
{
	for (int i = 0; i < m_count; i++)
		free(m_files[i]);

	free(m_files);
	m_files = 0;
	m_count = 0;
}
