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

private:
	void readSettings();
	void writeSettings();

private:
	HeaderView *m_header;
	FileView *m_fileView;
	QDir m_currentDir;
};


#endif //__FILEPANEL_H__
