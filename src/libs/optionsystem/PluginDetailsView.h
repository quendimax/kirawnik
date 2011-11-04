#ifndef __OPTIONSYSTEM_PLUGINDETAILSVIEW_H__
#define __OPTIONSYSTEM_PLUGINDETAILSVIEW_H__

#include <QDialog>


namespace Ui {
	class PluginDetailsView;
}

class PluginObject;


class PluginDetailsView : public QDialog
{
public:
	PluginDetailsView(const PluginObject *plugin, QWidget *parent = 0);

private:
	Ui::PluginDetailsView *ui;
};

#endif //__OPTIONSYSTEM_PLUGINDETAILSVIEW_H__
