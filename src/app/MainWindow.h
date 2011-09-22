#ifndef __APP_MAINWINDOW_H__
#define __APP_MAINWINDOW_H__

#include <QMainWindow>

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


#endif //__APP_MAINWINDOW_H__
