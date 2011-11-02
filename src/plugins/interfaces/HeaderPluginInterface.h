#ifndef __PLUGINS_HEADERPLUGININTERFACE_H__
#define __PLUGINS_HEADERPLUGININTERFACE_H__

#include <QList>

class AbstractHeaderItem;


class HeaderPluginInterface
{
public:
	virtual QList<AbstractHeaderItem *> getHeaderItems() = 0;
};


Q_DECLARE_INTERFACE(HeaderPluginInterface, "org.violatorsoftware.kirawnik.HeaderPluginInterface/1.0")


#endif //__PLUGINS_HEADERVIEWPLUGININTERFACE_H__
