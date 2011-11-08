#ifndef __PLUGINSYSTEM_PLUGINOBJECT_H__
#define __PLUGINSYSTEM_PLUGINOBJECT_H__

#include <QObject>


class PluginSpec;


class PluginObject : public QObject
{
	Q_OBJECT

public:
	PluginObject();

	virtual void initPluginSpec (PluginSpec *plugSpec) const = 0;

private:
	bool operator == (const PluginObject &) const;
	PluginObject &operator = (const PluginObject &);
};


#endif //__PLUGINSYSTEM_PLUGINSPEC_H__
