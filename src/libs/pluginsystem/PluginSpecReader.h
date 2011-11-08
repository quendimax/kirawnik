#ifndef __PLUGINSYSTEM_PLUGINSPECFILEHANDLER_H__
#define __PLUGINSYSTEM_PLUGINSPECFILEHANDLER_H__


class PluginSpec;


class PluginSpecReader
{
public:
	PluginSpecReader(PluginSpec *);
	bool read();

private:
	PluginSpec *m_pluginSpec;
};


#endif //__PLUGINSYSTEM_PLUGINSPECFILEHANDLER_H__
