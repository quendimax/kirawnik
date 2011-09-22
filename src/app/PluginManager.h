#ifndef __APP_PLUGINMANAGER__
#define __APP_PLUGINMANAGER__

#include <QObject>
#include <QStringList>

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
};


#endif //__APP_PLUGINMANAGER__
