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


bool PluginManager::enablePlugin(const QString &pluginName, bool on)
{
	Q_ASSERT(m_plugins.contains(pluginName));
	PluginSpec &plugin = m_plugins[pluginName];

	if (!on) {
		plugin.m_willLoad = false;
		emit pluginTurnedOn(plugin.name(), false);
		return true;
	}

	// else if (on)

	if (checkPluginDependency(pluginName)) {
		turnOnPluginDependency(pluginName);
	}

	return true;
}


void PluginManager::getPluginList()
{
	QStringList filters;
	filters << "*.pluginspec";

	for (const QString &path : m_pluginPaths) {
		QDir dir(path);
		dir.setNameFilters(filters);

		for (const QFileInfo &pluginSpecFile : dir.entryInfoList()) {
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
				continue;
			}

			pluginSpec.m_path = pluginSpecFile.canonicalPath();

			PluginSpec plugin = m_plugins.value(pluginSpec.name());
			if (plugin.name() != "Kirawnik-Unknown") {
				qDebug("The %s plugin (%s) is duplicating\nthe %s plugin (%s)",
				       qPrintable(pluginSpec.name()), qPrintable(pluginSpec.path()),
				       qPrintable(plugin.name()), qPrintable(plugin.path()));
				continue;
			}

			//TODO: have to add setting up of willLoad
			QSettings *sets = kApp->settings();
			pluginSpec.m_willLoad = sets->value("Plugins/" + pluginSpec.name() + ".WillLoad", true).toBool();

			m_plugins[pluginSpec.name()] = pluginSpec;
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

	QMutableMapIterator<QString, PluginSpec> it(m_plugins);
	while (it.hasNext()) {
		PluginSpec &plugin = it.next().value();

		if (!plugin.m_willLoad)
			continue;

		QString pluginFileName = plugin.path() + "/" + prefix + plugin.name() + postfix;
		plugin.m_loader->setFileName(pluginFileName);

		if (plugin.m_loader->load()) {
			plugin.m_pluginObject = qobject_cast<PluginObject *>(plugin.m_loader->instance());
			Q_ASSERT(plugin.m_pluginObject);
			plugin.m_state = PluginSpec::Loaded;
		}
		else {
			plugin.m_state = PluginSpec::Invalid;
			qWarning("Cannot load the \"%s\" plugin", qPrintable(plugin.name()));
		}
	}

	sets->endGroup();
}


void PluginManager::unloadPlugins()
{
	QSettings *sets = kApp->settings();
	QMutableMapIterator<QString, PluginSpec> it(m_plugins);
	while (it.hasNext()) {
		PluginSpec &plugin = it.next().value();
		if (plugin.m_loader->isLoaded())
			plugin.m_loader->unload();
		sets->setValue("Plugins/" + plugin.name() + ".WillLoad", plugin.willLoad());
	}
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


bool PluginManager::checkPluginDependency(const QString &pluginName)
{
	if (!m_plugins.contains(pluginName)) return false;

	const PluginSpec &plugin = m_plugins[pluginName];
	for (const PluginDependency &dependency : plugin.dependencies()) {
		if (!checkPluginDependency(dependency.name))
			return false;
	}
	return true;
}


void PluginManager::turnOnPluginDependency(const QString &pluginName)
{
	Q_ASSERT(m_plugins.contains(pluginName));
	PluginSpec &plugin = m_plugins[pluginName];

	plugin.m_willLoad = true;
	emit pluginTurnedOn(pluginName, true);

	for (const PluginDependency &dependency : plugin.dependencies())
		turnOnPluginDependency(dependency.name);
}


void PluginManager::turnOffDependencyPlugins(const QString &pluginName)
{
	Q_ASSERT(m_plugins.contains(pluginName));
	PluginSpec &plugin = m_plugins[pluginName];

	plugin.m_willLoad = false;
}
