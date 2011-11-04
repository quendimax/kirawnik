#ifndef __OPTIONSYSTEM_PLUGINOPTIONWIDGET_H__
#define __OPTIONSYSTEM_PLUGINOPTIONWIDGET_H__

#include <QScopedPointer>

#include "OptionWidget.h"
#include "ui_PluginOptionWidget.h"

class QTreeWidgetItem;


class PluginOptionWidget : public OptionWidget
{
	Q_OBJECT

public:
	PluginOptionWidget(QWidget *parent = 0);

	QListWidgetItem *createListWidgetItem() const;

private slots:
	void showPluginDetailsView(QTreeWidgetItem *, int);

private:
	void initPluginTree();

private:
	QScopedPointer<Ui::PluginOptionWidget> ui;
};


#endif //__OPTIONSYSTEM_PLUGINOPTIONWIDGET_H__
