#include <QListWidgetItem>

#include <core/Application.h>
#include <pluginsystem/PluginObject.h>
#include <pluginsystem/PluginManager.h>

#include "PluginOptionWidget.h"


PluginOptionWidget::PluginOptionWidget(QWidget *parent)
	: OptionWidget(parent), ui(new Ui::PluginOptionWidget())
{
	ui->setupUi(this);
	setMinimumHeight(400);

	ui->detailsButton->setDisabled(true);
	ui->pluginTreeWidget->header()->setResizeMode(QHeaderView::Stretch);

	initPluginTree();

	connect(ui->pluginTreeWidget, SIGNAL(itemClicked(QTreeWidgetItem*, int)),
	        this, SLOT(showPluginDetailsView(QTreeWidgetItem*, int)));
}


QListWidgetItem *PluginOptionWidget::createListWidgetItem() const
{
	QListWidgetItem *item = new QListWidgetItem(QIcon(kApp->resourceFile("pixmaps/PluginOptions.png")),
	                                            tr("Plugins", "Name of plugin option widget"));
	return item;
}


void PluginOptionWidget::showPluginDetailsView(QTreeWidgetItem *, int)
{

}


void PluginOptionWidget::initPluginTree()
{
/*	QList<PluginObject *> plugins = kApp->pluginManager()->getPlugins<PluginObject>();
	for (const auto plugin : plugins) {
		QStringList list;
		list << "" << plugin->name() << plugin->version() << plugin->author();

		QTreeWidgetItem *item = new QTreeWidgetItem(ui->pluginTreeWidget, list);
		item->setCheckState(0, Qt::Checked);
		ui->pluginTreeWidget->addTopLevelItem(item);
	}*/
}
