#ifndef __FILEPANEL_H__
#define __FILEPANEL_H__

#include <QDir>
#include <QWidget>

class HeaderView;
class FileView;


/*!
  The FilePanel object shows information about curent disk part,
  and curent directory.
  */
class FilePanel : public QWidget
{
	Q_OBJECT

public:
	explicit FilePanel(const QString &objectName, QWidget *parent = 0);
	virtual ~FilePanel();

protected:
	void keyPressEvent(QKeyEvent *);
	void mouseDoubleClickEvent(QMouseEvent *);

private:
	void changeDir(const QString &dirName);
	void openFile(const QFileInfo &fileInfo);

	void readSettings();
	void writeSettings();

private:
	HeaderView *m_header;
	FileView *m_fileView;
	QDir m_currentDir;
	QFileInfoList m_fileList;
};


#endif //__FILEPANEL_H__
