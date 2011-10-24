#ifndef __APP_PLUGINMANAGER__
#define __APP_PLUGINMANAGER__

#include <QList>
#include <QObject>
#include <QStringList>
#include <QPluginLoader>
#include <QSharedPointer>

#include "plugins/interfaces/PluginInterface.h"


class PluginManager : public QObject
{
	Q_OBJECT

public:
	PluginManager(QObject *parent = 0);
	~PluginManager();

	void addPluginPath(const QString &path);

	template<class Interface> QList<Interface *> getPlugins() const;

private:
	struct PluginEntry {
		QSharedPointer<QPluginLoader> loader;
		QString fileName;
		bool on;
	};

private:
	void loadPlugins();
	void unloadPlugins();
	void readPaths();
	void writePaths() const;

private:
	QStringList m_pluginPaths;
	QList<PluginEntry> m_pluginList;
};


template<class Interface> QList<Interface *> PluginManager::getPlugins() const
{
	QList<Interface *> resultList;

	foreach (const PluginEntry &entry, m_pluginList) {
		if (entry.on)
			if (Interface *inf = qobject_cast<Interface *>(entry.loader->instance()))
				resultList.append(inf);
	}

	return resultList;
}


#endif //__APP_PLUGINMANAGER__
