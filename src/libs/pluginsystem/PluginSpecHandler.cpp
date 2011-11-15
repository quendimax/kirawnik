#include <QXmlAttributes>

#include "PluginSpec.h"
#include "PluginSpecHandler.h"


PluginSpecHandler::PluginSpecHandler(PluginSpec *pluginSpec)
    : e_pluginSpec(pluginSpec)
{
}


bool PluginSpecHandler::startElement(const QString &, const QString &, const QString &qName, const QXmlAttributes &attrs)
{
	if (qName == "plugin") {
		e_pluginSpec->m_name = attrs.value("name");
		e_pluginSpec->m_version = attrs.value("version");
	}
	else if (qName == "dependency") {
		PluginDependency dep;
		dep.name = attrs.value("name");
		dep.version = attrs.value("version");
		QString type = attrs.value("type");
		if (type == "optional") dep.type = PluginDependency::Optional;
		else if (type == "required") dep.type = PluginDependency::Required;
		e_pluginSpec->m_dependecies.append(dep);
	}

	m_text.clear();
	return true;
}


bool PluginSpecHandler::endElement(const QString &, const QString &, const QString &qName)
{
	if (qName == "vendor")
		e_pluginSpec->m_vendor = m_text;
	else if (qName == "license")
		e_pluginSpec->m_license = m_text;
	else if (qName == "author")
		e_pluginSpec->m_author = m_text;
	else if (qName == "description")
		e_pluginSpec->m_description = m_text;
	else if (qName == "category")
		e_pluginSpec->m_category = m_text;
	else if (qName == "url")
		e_pluginSpec->m_url = m_text;

	return true;
}


bool PluginSpecHandler::characters(const QString &ch)
{
	m_text.append(ch);
	return true;
}
