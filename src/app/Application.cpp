#include <QDir>
#include <QSettings>

#include "Application.h"


Application::Application(int &argc, char **argv)
    : QApplication(argc, argv)
{
	QApplication::setApplicationName("Kirawnik");
	QApplication::setApplicationVersion("pre");
	QApplication::setOrganizationName("Violators Software");

	m_settings = new QSettings(QSettings::IniFormat, QSettings::UserScope,
	                           QApplication::organizationName(),
	                           QApplication::applicationName().toLower());
}


Application::~Application()
{
	delete m_settings;
}


QString Application::resourceFile(const QString &shortFileName)
{
	return QString("%1/../share/kirawnik/%2").arg(kApp->applicationDirPath()).arg(shortFileName);
}
