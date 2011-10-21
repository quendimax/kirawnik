#include "Application.h"
#include "MainWindow.h"


int main(int argc, char *argv[])
{
	Application app(argc, argv);

	MainWindow window;
	window.setWindowTitle(app.applicationName() + "-" + app.applicationVersion());
	window.show();

	return app.exec();
}
