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

	void addPluginPath(const QString &path);
	PluginInterface *loadPlugin(const QString &pluginName);

private:
	QStringList m_paths;
	QList<QPluginLoader> m_pluginLoaders;
};


#endif //__APP_PLUGINMANAGER__
