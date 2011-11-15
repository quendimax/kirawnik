#ifndef __PLUGINSYSTEM_PLUGINSPEC_H__
#define __PLUGINSYSTEM_PLUGINSPEC_H__

#include <QList>
#include <QSharedPointer>
#include <QString>


class QPluginLoader;
class PluginObject;


struct PluginDependency
{
	enum Type {
		Required,
		Optional
	};

	QString name;
	QString version;
	Type type;

	bool operator == (const PluginDependency &) const;
};


class PluginSpec
{
public:
	enum State { Loaded, Unloaded, Invalid };

	PluginSpec();

	inline QString name() const { return m_name; }
	inline QString version() const { return m_version; }
	inline QString vendor() const { return m_vendor; }
	inline QString license() const { return m_license; }
	inline QString author() const { return m_author; }
	inline QString description() const { return m_description; }
	inline QString category() const { return m_category; }
	inline QString url() const { return m_url; }
	inline QString path() const { return m_path; }

	inline QList<PluginDependency> dependencies() const { return m_dependecies; }
	inline PluginObject *plugin() const { return m_pluginObject; }
	inline State state() const { return m_state; }
	inline bool willLoad() const { return m_willLoad; }

private:
	QString m_name;
	QString m_version;
	QString m_vendor;
	QString m_license;
	QString m_author;
	QString m_description;
	QString m_category;
	QString m_url;
	QString m_path;
	QList<PluginDependency> m_dependecies;
	PluginObject *m_pluginObject;
	QSharedPointer<QPluginLoader> m_loader;
	State m_state;
	bool m_willLoad;

	friend class PluginSpecHandler;
	friend class PluginManager;
};


#endif //__PLUGINSYSTEM_PLUGINSPEC_H__
