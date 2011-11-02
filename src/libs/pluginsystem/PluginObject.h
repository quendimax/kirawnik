#ifndef __PLUGINSYSTEM_PLUGINOBJECT_H__
#define __PLUGINSYSTEM_PLUGINOBJECT_H__

#include <QList>
#include <QObject>
#include <QString>


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

	bool operator== (const PluginDependency &) const;
};


class PluginObject : public QObject
{
public:
	PluginObject();

	inline QString name() const { return m_name; }
	inline QString version() const { return m_version; }
	inline QString vendor() const { return m_vendor; }
	inline QString license() const { return m_license; }
	inline QString author() const { return m_author; }
	inline QString description() const { return m_description; }
	inline QString url() const { return m_url; }

	inline QList<PluginDependency> dependencies() const { return m_dependecies; }
	inline bool isEnabled() const { return m_enabled; }
	inline void setEnabled(bool value) { m_enabled = value; }

protected:
	inline void setName(const QString &name) { m_name = name; }
	inline void setVersion(const QString &version) { m_version = version; }
	inline void setVendor(const QString &vendor) { m_vendor = vendor; }
	inline void setLicense(const QString &license) { m_license = license; }
	inline void setAuthor(const QString &author) { m_author = author; }
	inline void setDescription(const QString &description) { m_description = description; }
	inline void setUrl(const QString &url) { m_url = url; }

	inline void addPluginDependency(const PluginDependency &dep) { m_dependecies.append(dep); }

private:
	bool operator == (const PluginObject &) const;
	PluginObject &operator=(const PluginObject &);

private:
	QString m_name;
	QString m_version;
	QString m_vendor;
	QString m_license;
	QString m_author;
	QString m_description;
	QString m_url;
	QList<PluginDependency> m_dependecies;
	bool m_enabled;
};


#endif //__PLUGINSYSTEM_PLUGINSPEC_H__
