#include <QMap>
#include <QDir>
#include <QFile>
#include <QSettings>
#include <QCryptographicHash>

#include <core/Application.h>

#include "PluginManager.h"


PluginManager::PluginManager(QObject *parent)
    : QObject(parent)
{
	readPaths();
	QStringList pluginList = getPluginList();
	loadPlugins(pluginList);
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


/*!
  To get the list of unique plugins.
  */
QStringList PluginManager::getPluginList() const
{
	QStringList filters;
#ifdef Q_OS_LINUX
	#ifdef KIRAWNIK_DEBUG
		filters << "libkplugin_*d.so";
	#else
		filters << "libkplugin_*.so";
	#endif
#elif Q_OS_WIN32
	#ifdef KIRAWNIK_DEBUG
		filters << "kplugin_*d.dll";
	#else
		filters << "kplugin_*.dll";
	#endif
#endif

	QMap<QByteArray, QString> originalPlugins;
	foreach (QString path, m_pluginPaths) {
		QDir dir(path);
		dir.setNameFilters(filters);

		foreach (const QFileInfo &info, dir.entryInfoList()) {
			QString pluginName = info.canonicalFilePath();
			QByteArray pluginHash = getPluginHash(pluginName);
			originalPlugins[pluginHash] = pluginName;
		}
	}

	QStringList pluginNameList;
	foreach (const QByteArray &key, originalPlugins.uniqueKeys())
		pluginNameList.append(originalPlugins[key]);

	return pluginNameList;
}


void PluginManager::loadPlugins(const QStringList &pluginList)
{
	QSettings *sets = kApp->settings();
	sets->beginGroup("Plugins");

	foreach (const QString &file, pluginList) {
		PluginEntry plugin;
		plugin.loader = QSharedPointer<QPluginLoader>(new QPluginLoader);
		plugin.loader->setFileName(file);
		plugin.fileName = file;

		if (!sets->contains(file)) {
			if (plugin.loader->load()) {
				sets->setValue(file, true);
				plugin.on = true;
				m_pluginList.append(plugin);
			}
			else
				qWarning("Plugin \"%s\" cannot load", qPrintable(file));
		}
		else {
			plugin.on = sets->value(file).toBool();
			if (plugin.on) {
				if (!plugin.loader->load())
					qWarning("Plugin \"%s\" cannot load", qPrintable(file));
			}
			m_pluginList.append(plugin);
		}
	}

	sets->endGroup();
}


void PluginManager::unloadPlugins()
{
	foreach (PluginEntry plugin, m_pluginList)
		if (plugin.loader->isLoaded())
			plugin.loader->unload();
}


QByteArray PluginManager::getPluginHash(const QString &fileName) const
{
	QCryptographicHash hash(QCryptographicHash::Md5);
	QFile file(fileName);
	file.open(QIODevice::ReadOnly);
	hash.addData(file.read(0x7FFFFFFF));
	file.close();
	return hash.result();
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
