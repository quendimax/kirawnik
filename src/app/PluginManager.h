#ifndef __APP_PLUGINMANAGER__
#define __APP_PLUGINMANAGER__

#include <QMap>
#include <QList>
#include <QObject>
#include <QStringList>
#include <QPluginLoader>

#include "plugins/interfaces/PluginInterface.h"


class PluginManager : public QObject
{
	Q_OBJECT

public:
	PluginManager(QObject *parent = 0);
	~PluginManager();

	void addPluginPath(const QString &path);
	PluginInterface *loadPlugin(const QString &pluginName);

	template<class Interface> QList<Interface *> getPlugin() const;

private:
	QStringList m_paths;
	QList<QPluginLoader> m_pluginLoaders;
};


template<class Interface> QList<Interface *> PluginManager::getPlugin() const
{
	QList<Interface *> resultList;

	foreach (QPluginLoader &loader, m_pluginLoaders) {
		if (Interface *inf = qobject_cast<Interface *>(loader.instance()))
			resultList.append(inf);
	}

	return resultList;
}


#endif //__APP_PLUGINMANAGER__
