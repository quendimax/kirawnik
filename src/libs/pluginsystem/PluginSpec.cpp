#include "PluginSpec.h"


bool PluginDependency::operator == (const PluginDependency &t) const
{
	return name == t.name  &&  version == t.version  &&  type == t.type;
}
