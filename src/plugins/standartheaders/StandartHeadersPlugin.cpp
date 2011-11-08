#include <QtPlugin>

#include <libs/core/Application.h>
#include <libs/pluginsystem/PluginSpec.h>

#include "StandartHeaders.h"
#include "StandartHeadersPlugin.h"


StandartHeadersPlugin::StandartHeadersPlugin()
{
	m_headers.append(new NameHeaderItem);
	m_headers.append(new SuffixHeaderItem);
	m_headers.append(new SizeHeaderItem);
	m_headers.append(new TextPermsHeaderItem);
	m_headers.append(new DigitPermsHeaderItem);
	m_headers.append(new OwnerHeaderItem);
	m_headers.append(new GroupHeaderItem);
	m_headers.append(new ModifiedHeaderItem);
}


StandartHeadersPlugin::~StandartHeadersPlugin()
{
	qDeleteAll(m_headers);
}


QList<AbstractHeaderItem *> StandartHeadersPlugin::getHeaderItems()
{
	return m_headers;
}


Q_EXPORT_PLUGIN2(kplugin_standartheaders, StandartHeadersPlugin)
