#include <QMenu>
#include <QAction>
#include <QMenuBar>
#include <QSettings>
#include <QKeyEvent>
#include <QMessageBox>

#include <optionsystem/OptionDialog.h>
#include <core/Application.h>
#include <core/FileSystemView.h>
#include <core/MainWindow.h>


MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	initMenuBar();

	m_fsView = new FileSystemView;
	setCentralWidget(m_fsView);

	resize(640, 460);
	readSettings();
}


MainWindow::~MainWindow()
{
	writeSettings();
}


void MainWindow::showSettings()
{
	static OptionDialog *dialog = new OptionDialog(this);
	dialog->show();
}


void MainWindow::about()
{
	QMessageBox::about(this, tr("About ") + kApp->applicationName(),
	                   tr("<h3>Kirawnik %1</h3>"
	                      "<p>Built on %2 at %3.</p>"
	                      "<p>Kirawnik is a dual-panel file manager for *nix systems (may be and others). "
	                      "It uses Qt4 Framework.</p>"
	                      "<p>You can find the source code at "
	                      "<a href=\"https://github.com/QuendiMax/kirawnik\">"
	                      "https://github.com/QuendiMax/kirawnik</a>.</p>"
	                      ).arg(kApp->applicationVersion()).arg(__DATE__).arg(__TIME__));
}


void MainWindow::initMenuBar()
{
	// File menu
	QMenu *fileMenu = menuBar()->addMenu(tr("&File"));

	QAction *exitAction = new QAction(tr("E&xit"), this);
	exitAction->setShortcut(QKeySequence::Quit);
	exitAction->setIcon(QIcon::fromTheme("application-exit"));
	connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));

	fileMenu->addAction(exitAction);


	// Option menu
	QMenu *optionsMenu = menuBar()->addMenu(tr("&Options"));

	QAction *settingsAction = new QAction(tr("&Settings..."), this);
	connect(settingsAction, SIGNAL(triggered()), this, SLOT(showSettings()));

	optionsMenu->addAction(settingsAction);

	// Help menu
	QMenu *helpMenu = menuBar()->addMenu(tr("&Help"));

	QAction *aboutAction = new QAction(tr("&About..."), this);
	aboutAction->setIcon(QIcon::fromTheme("help-about"));
	connect(aboutAction, SIGNAL(triggered()), this, SLOT(about()));

	QAction *aboutQtAction = new QAction(tr("About &Qt..."), this);
	connect(aboutQtAction, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

	helpMenu->addAction(aboutAction);
	helpMenu->addAction(aboutQtAction);
}


void MainWindow::readSettings()
{
	QSettings *sets = kApp->settings();

	sets->beginGroup("MainWindow");
	restoreState(sets->value("state").toByteArray());
	restoreGeometry(sets->value("geometry").toByteArray());
	setWindowState((Qt::WindowState) sets->value("windowState", (uint) Qt::WindowNoState).toUInt());
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
