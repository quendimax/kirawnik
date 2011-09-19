#include "PluginManager.h"


PluginManager::PluginManager(QObject *parent)
    : QObject(parent)
{

}


void PluginManager::addPluginPath(const QString &path)
{
	if (!m_paths.contains(path))
		m_paths.append(path);
}
