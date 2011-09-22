#ifndef __PLUGINS_PLUGININTERFACE_H__
#define __PLUGINS_PLUGININTERFACE_H__


class PluginInterface
{
public:
	virtual ~PluginInterface() = 0;

	virtual void initialize() = 0;
};


Q_DECLARE_INTERFACE(PluginInterface, "org.violatorsoftware.kirawnik.PluginInterface/0.1")


#endif //__PLUGINS_PLUGININTERFACE_H__
