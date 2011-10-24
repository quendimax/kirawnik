#include <QtPlugin>

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
	foreach (AbstractHeaderItem *item, m_headers)
		delete item;
}


QList<AbstractHeaderItem *> StandartHeadersPlugin::getHeaderItems()
{
	return m_headers;
}


Q_EXPORT_PLUGIN2(kplugin_standartheaders, StandartHeadersPlugin)
