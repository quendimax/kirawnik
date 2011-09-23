#ifndef __PLUGINS_HEADERPLUGININTERFACE_H__
#define __PLUGINS_HEADERPLUGININTERFACE_H__

#include "app/AbstractHeaderItem.h"

#include "PluginInterface.h"


class HeaderPluginInterface : public PluginInterface
{
public:
	virtual AbstractHeaderItem *nextHeaderItem() = 0;
};


Q_DECLARE_INTERFACE(HeaderPluginInterface, "org.violatorsoftware.kirawnik.HeaderPluginInterface/0.1")


#endif //__PLUGINS_HEADERVIEWPLUGININTERFACE_H__
