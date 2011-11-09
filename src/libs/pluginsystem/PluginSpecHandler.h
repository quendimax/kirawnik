#ifndef __PLUGINSYSTEM_PLUGINSPECHANDLER_H__
#define __PLUGINSYSTEM_PLUGINSPECHANDLER_H__

#include <QString>
#include <QXmlDefaultHandler>

class PluginSpec;


class PluginSpecHandler : public QXmlDefaultHandler
{
public:
	PluginSpecHandler(PluginSpec *pluginSpec);

	bool startElement(const QString &, const QString &, const QString &qName, const QXmlAttributes &attrs);
	bool endElement(const QString &, const QString &, const QString &qName);
	bool characters(const QString &ch);

private:
	PluginSpec *e_pluginSpec;
	QString m_text;
};


#endif //__PLUGINSYSTEM_PLUGINSPECHANDLER_H__
