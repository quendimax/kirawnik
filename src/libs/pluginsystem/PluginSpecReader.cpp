#include <QXmlDefaultHandler>

#include "PluginSpec.h"
#include "PluginSpecReader.h"


class PluginSpecFileHandler : public QXmlDefaultHandler
{
public:
	PluginSpecFileHandler(PluginSpec *pluginSpec) : m_pluginSpec(pluginSpec) {}

	bool startElement(const QString &namespaceURI, const QString &localName, const QString &qName, const QXmlAttributes &atts);
	bool endElement(const QString &namespaceURI, const QString &localName, const QString &qName);
	bool characters(const QString &ch);

private:
	PluginSpec *m_pluginSpec;
};


PluginSpecReader::PluginSpecReader(PluginSpec *pluginSpec)
    : m_pluginSpec(pluginSpec)
{
}
