#ifndef __PLUGINSYSTEM_PLUGINMANAGER__
#define __PLUGINSYSTEM_PLUGINMANAGER__

#include <QMap>
#include <QList>
#include <QObject>
#include <QStringList>
#include <QPluginLoader>
#include <QSharedPointer>

#include "PluginSpec.h"
#include "PluginObject.h"


class PluginManager : public QObject
{
	Q_OBJECT

public:
	PluginManager(QObject *parent = 0);
	~PluginManager();

	void addPluginPath(const QString &path);

	inline QList<PluginSpec> pluginSpecs() const { return m_plugins.values(); }
	template<class Interface> QList<Interface *> getPlugins() const;

public slots:
	bool enablePlugin(const QString &pluginName, bool on);

signals:
	void pluginTurnedOn(const QString &pluginName, bool on);

private:
	void getPluginList();
	void loadPlugins();
	void unloadPlugins();
	void readPaths();
	void writePaths() const;

	bool checkPluginDependency(const QString &pluginName);
	void turnOnPluginDependency(const QString &pluginName);
	void turnOffDependencyPlugins(const QString &pluginName);

private:
	QStringList m_pluginPaths;
	QMap<QString, PluginSpec> m_plugins;	//!> QString - the plugin name, PluginSpec - the plugin
};


template<class Interface> QList<Interface *> PluginManager::getPlugins() const
{
	QList<Interface *> resultList;

	for (const auto &entry : m_plugins.values()) {
		if (entry.state() == PluginSpec::Loaded)
			if (Interface *inf = qobject_cast<Interface *>(entry.plugin()))
				resultList.append(inf);
	}

	return resultList;
}


#endif //__PLUGINSYSTEM_PLUGINMANAGER__
