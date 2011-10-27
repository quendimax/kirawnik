#ifndef __PLUGINOPTIONWIDGET_H__
#define __PLUGINOPTIONWIDGET_H__

#include "OptionWidget.h"


class PluginOptionWidget : public OptionWidget
{
	Q_OBJECT

public:
	PluginOptionWidget(QWidget *parent = 0);

	QListWidgetItem *createListWidgetItem() const;
};


#endif //__PLUGINOPTIONWIDGET_H__
