#ifndef __PLUGINSYSTEM_PLUGINSPEC_H__
#define __PLUGINSYSTEM_PLUGINSPEC_H__

#include <QString>
#include <QList>


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
	enum State { Loaded, Unloaded };

	inline QString name() const { return m_name; }
	inline QString version() const { return m_version; }
	inline QString vendor() const { return m_vendor; }
	inline QString license() const { return m_license; }
	inline QString author() const { return m_author; }
	inline QString description() const { return m_description; }
	inline QString url() const { return m_url; }

	inline QList<PluginDependency> dependencies() const { return m_dependecies; }
	inline PluginObject *plugin() const { return m_pluginObject; }
	inline State state() const { return m_state; }

private:
	QString m_name;
	QString m_version;
	QString m_vendor;
	QString m_license;
	QString m_author;
	QString m_description;
	QString m_url;
	QList<PluginDependency> m_dependecies;
	PluginObject *m_pluginObject;
	State m_state;
	bool m_willLoad;

	friend class PluginSpecHandler;
};


#endif //__PLUGINSYSTEM_PLUGINSPEC_H__
