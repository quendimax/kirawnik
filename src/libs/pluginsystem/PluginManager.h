#ifndef __PLUGINSYSTEM_PLUGINMANAGER__
#define __PLUGINSYSTEM_PLUGINMANAGER__

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

	QList<PluginSpec> pluginSpecs() const;
	template<class Interface> QList<Interface *> getPlugins() const;

public slots:
	void turnOnPlugin(const PluginObject *p, bool on);

private:
	void getPluginList() const;
	void loadPlugins();
	void unloadPlugins();
	void readPaths();
	void writePaths() const;

private:
	QStringList m_pluginPaths;
	QList<PluginSpec> m_pluginList;
};


template<class Interface> QList<Interface *> PluginManager::getPlugins() const
{
	QList<Interface *> resultList;

	for (const auto &entry : m_pluginList) {
		if (entry.state() == PluginSpec::Loaded)
			if (Interface *inf = qobject_cast<Interface *>(entry.plugin()))
				resultList.append(inf);
	}

	return resultList;
}


#endif //__PLUGINSYSTEM_PLUGINMANAGER__
