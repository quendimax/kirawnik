#ifndef __PLUGINS_PLUGININTERFACE_H__
#define __PLUGINS_PLUGININTERFACE_H__


class PluginInterface
{
public:
	virtual void initialize() = 0;
	virtual ~PluginInterface() = 0;
};


Q_DECLARE_INTERFACE(PluginInterface, "org.violatorsoftware.kirawnik.PluginInterface/1.0")


#endif //__PLUGINS_PLUGININTERFACE_H__
