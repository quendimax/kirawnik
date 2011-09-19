#ifndef __PLUGINMANAGER__
#define __PLUGINMANAGER__

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


#endif //__PLUGINMANAGER__
