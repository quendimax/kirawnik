#ifndef __OPTIONSYSTEM_PLUGINDETAILSVIEW_H__
#define __OPTIONSYSTEM_PLUGINDETAILSVIEW_H__

#include <QScopedPointer>
#include <QDialog>

#include "ui_PluginDetailsView.h"


class PluginObject;


class PluginDetailsView : public QDialog
{
public:
	PluginDetailsView(const PluginObject *plugin, QWidget *parent = 0);

private:
	QScopedPointer<Ui::PluginDetailsView> ui;
};

#endif //__OPTIONSYSTEM_PLUGINDETAILSVIEW_H__
