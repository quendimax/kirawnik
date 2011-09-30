#include <QMenu>
#include <QAction>
#include <QMenuBar>
#include <QSettings>
#include <QKeyEvent>
#include <QMessageBox>

#include "Application.h"
#include "FileSystemView.h"
#include "MainWindow.h"


MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	initMenuBar();

	fsView = new FileSystemView;
	setCentralWidget(fsView);

	readSettings();
}


MainWindow::~MainWindow()
{
	writeSettings();
}


void MainWindow::showSettings()
{

}


void MainWindow::about()
{
	QMessageBox::information(this, tr("About ") + kApp->applicationName(),
	                         tr("<p>Kirawnik is a dual-panel file manager for *nix-systems (may be and other). "
	                            "It uses Qt4-library.</p>"
	                            "<p>You may find the source code at "
	                            "<a href=\"https://github.com/QuendiMax/kirawnik\">"
	                            "https://github.com/QuendiMax/kirawnik</a>.</p>"));
}


void MainWindow::initMenuBar()
{
	// File menu
	QMenu *fileMenu = menuBar()->addMenu(tr("File"));

	QAction *exitAction = new QAction(tr("Exit"), this);
	exitAction->setShortcut(QKeySequence::Quit);
	exitAction->setIcon(QIcon::fromTheme("application-exit"));
	connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));

	fileMenu->addAction(exitAction);


	// Option menu
	QMenu *optionsMenu = menuBar()->addMenu(tr("Options"));

	QAction *settingsAction = new QAction(tr("Settings..."), this);
	connect(settingsAction, SIGNAL(triggered()), this, SLOT(showSettings()));

	optionsMenu->addAction(settingsAction);

	// Hellp menu
	QMenu *helpMenu = menuBar()->addMenu(tr("Help"));

	QAction *aboutAction = new QAction(tr("About..."), this);
	aboutAction->setIcon(QIcon::fromTheme("help-about"));
	connect(aboutAction, SIGNAL(triggered()), this, SLOT(about()));

	QAction *aboutQtAction = new QAction(tr("About Qt..."), this);
	connect(aboutQtAction, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

	helpMenu->addAction(aboutAction);
	helpMenu->addAction(aboutQtAction);
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
