#ifndef __PLUGINOPTIONWIDGET_H__
#define __PLUGINOPTIONWIDGET_H__

#include "OptionWidget.h"

namespace Ui {
	class PluginOptionWidget;
}


class PluginOptionWidget : public OptionWidget
{
	Q_OBJECT

public:
	PluginOptionWidget(QWidget *parent = 0);
	~PluginOptionWidget();

	QListWidgetItem *createListWidgetItem() const;

private:
	Ui::PluginOptionWidget *ui;
};


#endif //__PLUGINOPTIONWIDGET_H__
