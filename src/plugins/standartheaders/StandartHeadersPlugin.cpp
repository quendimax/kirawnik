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


void StandartHeadersPlugin::initPluginSpec(PluginSpec *plugSpec) const
{
	class StandartHeaderPluginSpec : public PluginSpec
	{
	public:
		StandartHeaderPluginSpec()
		{
			setName("Standart Headers");
			setVersion("0.0.0");
			setVendor(tr("Maksim Tamkovich"));
			setAuthor(tr("Maksim Tamkovich"));
			setLicense("GPL");
			setUrl("https://github.com/QuendiMax/kirawnik/");
			setDescription(tr("This is a plugin with classical head items."));
		}
	};

	StandartHeaderPluginSpec spec;
	*plugSpec = spec;
}


Q_EXPORT_PLUGIN2(kplugin_standartheaders, StandartHeadersPlugin)
