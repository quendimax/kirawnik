#ifndef __PLUGINSYSTEM_PLUGINSPECFILEHANDLER_H__
#define __PLUGINSYSTEM_PLUGINSPECFILEHANDLER_H__

#include <QXmlDefaultHandler>

class PluginSpec;


class PluginSpecFileHandler : public QXmlDefaultHandler
{
public:
	PluginSpecFileHandler(PluginSpec *);

	bool startElement(const QString &namespaceURI, const QString &localName, const QString &qName, const QXmlAttributes &atts);
	bool endElement(const QString &namespaceURI, const QString &localName, const QString &qName);
	bool characters(const QString &ch);

private:
	PluginSpec *m_pluginSpec;
};


#endif //__PLUGINSYSTEM_PLUGINSPECFILEHANDLER_H__
