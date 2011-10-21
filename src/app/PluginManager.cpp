#include <QSettings>

#include "Application.h"
#include "PluginManager.h"


PluginManager::PluginManager(QObject *parent)
    : QObject(parent)
{
	readPaths();
}


PluginManager::~PluginManager()
{
	writePaths();
}


void PluginManager::addPluginPath(const QString &path)
{
	m_pluginPaths.append(path);
}


void PluginManager::loadPlugins()
{

}


void PluginManager::unloadPlugins()
{

}


void PluginManager::readPaths()
{
	QSettings *sets = kApp->settings();
	sets->beginGroup("Plugins");

	QStringList defaultPaths;
	defaultPaths << kApp->applicationDirPath() + "/../lib/kirawnik/";

	m_pluginPaths = sets->value("Paths", defaultPaths).toStringList();

	sets->endGroup();
}


void PluginManager::writePaths()
{
	QSettings *sets = kApp->settings();
	sets->beginGroup("Plugins");
	sets->setValue("Paths", m_pluginPaths);
	sets->endGroup();
}
