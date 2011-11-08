#ifndef __PLUGINS_STANDARTHEADERSPLUGIN_H__
#define __PLUGINS_STANDARTHEADERSPLUGIN_H__

#include <pluginsystem/PluginObject.h>
#include <plugins/interfaces/HeaderPluginInterface.h>

class AbstractHeaderItem;


class StandartHeadersPlugin : public PluginObject, public HeaderPluginInterface
{
	Q_OBJECT
	Q_INTERFACES(HeaderPluginInterface)

public:
	StandartHeadersPlugin();
	~StandartHeadersPlugin();

	void initPluginSpec(PluginSpec *plugSpec) const;
	QList<AbstractHeaderItem *> getHeaderItems();

private:
	QList<AbstractHeaderItem *> m_headers;
};


#endif //__PLUGINS_STANDARTHEADERSPLUGIN_H__
