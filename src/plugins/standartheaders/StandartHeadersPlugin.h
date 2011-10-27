#ifndef __PLUGINS_STANDARTHEADERSPLUGIN_H__
#define __PLUGINS_STANDARTHEADERSPLUGIN_H__

#include <core/AbstractHeaderItem.h>
#include <plugins/interfaces/HeaderPluginInterface.h>


class StandartHeadersPlugin : public QObject, public HeaderPluginInterface
{
	Q_OBJECT
	Q_INTERFACES(HeaderPluginInterface)

public:
	StandartHeadersPlugin();
	~StandartHeadersPlugin();

	QList<AbstractHeaderItem *> getHeaderItems();

	void initialize() {}

private:
	QList<AbstractHeaderItem *> m_headers;
};


#endif //__PLUGINS_STANDARTHEADERSPLUGIN_H__
