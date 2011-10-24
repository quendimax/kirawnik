#include <QDir>
#include <QSettings>

#include "Application.h"
#include "PluginManager.h"


PluginManager::PluginManager(QObject *parent)
    : QObject(parent)
{
	readPaths();
	loadPlugins();
}


PluginManager::~PluginManager()
{
	unloadPlugins();
	writePaths();
}


void PluginManager::addPluginPath(const QString &path)
{
	m_pluginPaths.append(path);
}


void PluginManager::loadPlugins()
{
	QSettings *sets = kApp->settings();
	sets->beginGroup("Plugins");

	QStringList filters;
#ifdef Q_OS_LINUX
	filters << "libkplugin_*.so";
#elif Q_OS_WIN32
	filters << "kplugin_*.dll";
#endif

	foreach (QString path, m_pluginPaths)
	{
		QDir dir(path);
		dir.setNameFilters(filters);
		foreach (QFileInfo file, dir.entryInfoList()) {
			PluginEntry plugin;
			plugin.loader = QSharedPointer<QPluginLoader>(new QPluginLoader);
			plugin.loader->setFileName(file.canonicalFilePath());
			plugin.fileName = file.canonicalFilePath();

			if (!sets->contains(file.canonicalFilePath())) {
				if (plugin.loader->load()) {
					sets->setValue(file.canonicalFilePath(), true);
					plugin.on = true;
					m_pluginList.append(plugin);
				}
				else
					qWarning("Plugin \"%s\" cannot load", qPrintable(file.canonicalFilePath()));
			}
			else {
				plugin.on = sets->value(file.canonicalFilePath()).toBool();
				if (plugin.on) {
					if (!plugin.loader->load())
						qWarning("Plugin \"%s\" cannot load", qPrintable(file.canonicalFilePath()));
				}
				m_pluginList.append(plugin);
			}
		}
	}

	sets->endGroup();
}


void PluginManager::unloadPlugins()
{
	foreach (PluginEntry plugin, m_pluginList)
		plugin.loader->unload();
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


void PluginManager::writePaths() const
{
	QSettings *sets = kApp->settings();
	sets->beginGroup("Plugins");
	sets->setValue("Paths", m_pluginPaths);
	sets->endGroup();
}
