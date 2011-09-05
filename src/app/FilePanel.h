#ifndef __FILEPANEL_H__
#define __FILEPANEL_H__

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
	explicit FilePanel(QWidget *parent = 0);

private:
	HeaderView *m_header;
	FileView *m_fileView;
};


#endif //__FILEPANEL_H__
