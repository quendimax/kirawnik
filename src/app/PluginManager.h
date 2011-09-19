#ifndef __PLUGINMANAGER__
#define __PLUGINMANAGER__

#include <QObject>
#include <QStringList>


class PluginManager : public QObject
{
	Q_OBJECT

public:
	PluginManager(QObject *parent = 0);

	void addPluginPath(const QString &path);
	void *loadPlugin(const QString &pluginName);

private:
	QStringList m_paths;
};


#endif //__PLUGINMANAGER__
