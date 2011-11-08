#ifndef __PLUGINSYSTEM_PLUGINOBJECT_H__
#define __PLUGINSYSTEM_PLUGINOBJECT_H__

#include <QObject>


class PluginSpec;


class PluginObject : public QObject
{
	Q_OBJECT

public:
	PluginObject();

private:
	bool operator == (const PluginObject &) const;
	PluginObject &operator = (const PluginObject &);
};


#endif //__PLUGINSYSTEM_PLUGINSPEC_H__
