#ifndef __FILEPANEL_H__
#define __FILEPANEL_H__

#include <QWidget>


/*!
  The FilePanel object shows information about curent disk part,
  and curent directory.
  */
class FilePanel : public QWidget
{
	Q_OBJECT

public:
	explicit FilePanel(QWidget *parent = 0);
};


#endif //__FILEPANEL_H__
