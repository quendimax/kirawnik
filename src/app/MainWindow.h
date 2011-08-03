#ifndef __MAINWINDOW_H__
#define __MAINWINDOW_H__

#include <QtGui/QMainWindow>

class FileSystemView;


/*!
  Contains general central widget and saves window geometry settings
  */
class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	virtual ~MainWindow();

private:
	void readSettings();
	void writeSettings();

private:
	FileSystemView *fsView;
};


#endif //__MAINWINDOW_H__
