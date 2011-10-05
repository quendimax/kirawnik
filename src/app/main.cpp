#include <QDir>

#include "Application.h"
#include "MainWindow.h"


int main(int argc, char *argv[])
{
	Application app(argc, argv);

	QDir::setCurrent(app.applicationDirPath());
	qDebug("Current working dir: %s", qPrintable(QDir::currentPath()));

	MainWindow window;
	window.setWindowTitle(app.applicationName() + "-" + app.applicationVersion());
	window.show();

	return app.exec();
}
