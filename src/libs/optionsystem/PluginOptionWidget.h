#ifndef __OPTIONSYSTEM_PLUGINOPTIONWIDGET_H__
#define __OPTIONSYSTEM_PLUGINOPTIONWIDGET_H__

#include <QMap>
#include <QScopedPointer>

#include "OptionWidget.h"
#include "ui_PluginOptionWidget.h"

class QTreeWidgetItem;
class PluginSpec;


class PluginOptionWidget : public OptionWidget
{
	Q_OBJECT

public:
	PluginOptionWidget(QWidget *parent = 0);

	QListWidgetItem *createListWidgetItem() const;

private slots:
	void showPluginDetailsView();
	void enablePlugin(QTreeWidgetItem *, int);
	void enableDetailsButton(const QModelIndex &);
	void turnOnPlugin(const QString &pluginName, bool on);

signals:
	void pluginStateChanged(const QString &pluginName, bool willLoad);

private:
	void initPluginTree();
	PluginSpec findPluginSpec(const QString &pluginName);

private:
	QScopedPointer<Ui::PluginOptionWidget> ui;
	QMap<QString, QTreeWidgetItem *> m_items;
};


#endif //__OPTIONSYSTEM_PLUGINOPTIONWIDGET_H__
