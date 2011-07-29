#include "Application.h"


Application *kApp = 0;


Application::Application(int &argc, char **argv)
    : QApplication(argc, argv)
{
	QApplication::setApplicationName("Kirawnik");
	QApplication::setApplicationVersion("pre");
	QApplication::setOrganizationName("Violators Software");

	m_settings = new QSettings(QSettings::NativeFormat, QSettings::UserScope,
	                           QApplication::organizationName(),
	                           QApplication::applicationName().toLower());

	kApp = this;
}
