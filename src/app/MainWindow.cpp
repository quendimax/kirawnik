#include <QtCore/QSettings>

#include "Application.h"
#include "MainWindow.h"


MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
{
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
