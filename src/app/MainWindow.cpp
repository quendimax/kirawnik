#include <QtCore/QSettings>
#include <QtGui/QKeyEvent>

#include "Application.h"
#include "FileSystemView.h"
#include "MainWindow.h"


MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	fsView = new FileSystemView;
	setCentralWidget(fsView);

	readSettings();
}


MainWindow::~MainWindow()
{
	writeSettings();
}


void MainWindow::readSettings()
{
	QSettings *sets = kApp->settings();

	sets->beginGroup("MainWindow");
	restoreState(sets->value("state").toByteArray());
	setWindowState((Qt::WindowState) sets->value("windowState", (uint) Qt::WindowNoState).toUInt());
	if (!isMaximized())
		restoreGeometry(sets->value("geometry").toByteArray());
	sets->endGroup();
}


void MainWindow::writeSettings()
{
	QSettings *sets = kApp->settings();

	sets->beginGroup("MainWindow");
	sets->setValue("state", saveState());
	sets->setValue("windowState", (uint) windowState());
	if (!isMaximized())
		sets->setValue("geometry", saveGeometry());
	sets->endGroup();
}
