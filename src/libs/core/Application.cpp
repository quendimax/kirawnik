#include <QDir>
#include <QIcon>
#include <QSettings>

#include <pluginsystem/PluginManager.h>

#include "Application.h"


Application::Application(int &argc, char **argv)
    : QApplication(argc, argv)
{
	QApplication::setApplicationName("Kirawnik");
	QApplication::setApplicationVersion(KIRAWNIK_VERSION);
	QApplication::setOrganizationName("Violators Software");

	setWindowIcon(QIcon(kApp->applicationDirPath() + "/../share/icons/hicolor/256x256/apps/kirawnik.png"));

	m_settings = new QSettings(QSettings::IniFormat, QSettings::UserScope,
	                           QApplication::organizationName(),
	                           QApplication::applicationName().toLower());
	m_pluginManager = new PluginManager;
}


Application::~Application()
{
	// warning: the plugin manager must delete before the settings object!!!
	delete m_pluginManager;
	delete m_settings;
}


QString Application::resourceFile(const QString &shortFileName)
{
	return QString("%1/../share/kirawnik/%2").arg(kApp->applicationDirPath()).arg(shortFileName);
}
