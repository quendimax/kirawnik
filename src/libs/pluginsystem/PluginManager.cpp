#include <QDir>
#include <QFile>
#include <QMap>
#include <QSettings>
#include <QXmlSimpleReader>

#include <core/Application.h>

#include "PluginSpecHandler.h"
#include "PluginManager.h"


PluginManager::PluginManager(QObject *parent)
    : QObject(parent)
{
	readPaths();
	getPluginList();
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


void PluginManager::turnOnPlugin(const PluginObject *p, bool on)
{
	for (auto &entry : m_pluginList)
		if (entry.plugin() == p) {
			entry.m_willLoad = on;
			break;
		}
}


/*!
  To get the list of unique plugins.
  */
void PluginManager::getPluginList() const
{
	QStringList filters;
	filters << "*.pluginspec";

	for (const QString &path : m_pluginPaths) {
		QDir dir(path);
		dir.setNameFilters(filters);

		for (const auto &pluginSpecFile : dir.entryInfoList()) {
			PluginSpec pluginSpec;
			PluginSpecHandler handler(&pluginSpec);
			QXmlSimpleReader reader;
			reader.setContentHandler(&handler);

			QFile specFile(pluginSpecFile.canonicalFilePath());
			if (!specFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
				qWarning("Cannot open the \"%s\" file.", qPrintable(pluginSpecFile.canonicalFilePath()));
				continue;
			}
			QXmlInputSource source(&specFile);
			if (!reader.parse(source)) {
				qWarning("Cannot parse the \"%s\" file.", qPrintable(pluginSpecFile.canonicalFilePath()));
				pluginSpec.m_state = PluginSpec::Invalid;
				m_pluginList.append(pluginSpec);
				continue;
			}

			m_pluginList.append(pluginSpec);
		}
	}
}


void PluginManager::loadPlugins()
{
	QString prefix, postfix;
#ifdef Q_OS_LINUX
	prefix = "lib";
	#ifdef KIRAWNIK_DEBUG
		postfix = "d.so";
	#else
		postfix = ".so";
	#endif
#elif defined(Q_OS_WIN32)
	prefix = "";
	#ifdef KIRAWNIK_DEBUG
		postfix = "d.dll";
	#else
		postfix = ".dll";
	#endif
#endif

	QSettings *sets = kApp->settings();
	sets->beginGroup("Plugins");

	foreach (const QString &file, pluginList) {
		PluginSpec plugin;
		plugin.loader = QSharedPointer<QPluginLoader>(new QPluginLoader);
		plugin.loader->setFileName(file);
		plugin.fileName = file;
		plugin.instance = nullptr;

		if (!sets->contains(file)) {
			if (plugin.loader->load()) {
				sets->setValue(file, true);
				plugin.on = true;
				plugin.instance = qobject_cast<PluginObject *>(plugin.loader->instance());
				Q_ASSERT(plugin.instance);
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
			plugin.instance = qobject_cast<PluginObject *>(plugin.loader->instance());
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


void PluginManager::readPaths()
{
	QSettings *sets = kApp->settings();
	sets->beginGroup("Plugins");

	QStringList defaultPaths;
	defaultPaths << kApp->applicationDirPath() + "/../lib/kirawnik/plugins/";

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
