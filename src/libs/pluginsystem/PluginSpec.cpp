#include <QPluginLoader>

#include "PluginSpec.h"


bool PluginDependency::operator == (const PluginDependency &t) const
{
	return name == t.name  &&  version == t.version  &&  type == t.type;
}


PluginSpec::PluginSpec()
    : m_name("Kirawnik-Unknown"),
      m_pluginObject(0),
      m_loader(new QPluginLoader),
      m_state(Unloaded),
      m_willLoad(false)
{
}
