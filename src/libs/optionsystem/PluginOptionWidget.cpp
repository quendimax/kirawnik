#include <QListWidgetItem>

#include <core/Application.h>
#include <pluginsystem/PluginSpec.h>
#include <pluginsystem/PluginManager.h>

#include "PluginDetailsView.h"
#include "PluginOptionWidget.h"


PluginOptionWidget::PluginOptionWidget(QWidget *parent)
	: OptionWidget(parent), ui(new Ui::PluginOptionWidget())
{
	ui->setupUi(this);
	setMinimumHeight(400);

	ui->detailsButton->setDisabled(true);
	ui->pluginTreeWidget->header()->setResizeMode(QHeaderView::ResizeToContents);

	initPluginTree();

	connect(ui->pluginTreeWidget, SIGNAL(pressed(QModelIndex)), this, SLOT(enableDetailsButton(QModelIndex)));
	connect(ui->pluginTreeWidget, SIGNAL(itemChanged(QTreeWidgetItem *, int)),
	        this, SLOT(enablePlugin(QTreeWidgetItem *, int)));
	connect(ui->detailsButton, SIGNAL(clicked()), this, SLOT(showPluginDetailsView()));
}


QListWidgetItem *PluginOptionWidget::createListWidgetItem() const
{
	QListWidgetItem *item = new QListWidgetItem(QIcon(kApp->resourceFile("pixmaps/PluginOptions.png")),
	                                            tr("Plugins", "Name of plugin option widget"));
	return item;
}


void PluginOptionWidget::showPluginDetailsView()
{
	PluginSpec plugSpec = findPluginSpec(ui->pluginTreeWidget->currentItem()->text(0));
	PluginDetailsView *view = new PluginDetailsView(plugSpec);
	view->show();
}


void PluginOptionWidget::enablePlugin(QTreeWidgetItem *, int)
{
}


void PluginOptionWidget::enableDetailsButton(const QModelIndex &)
{
	ui->detailsButton->setEnabled(true);
}


void PluginOptionWidget::initPluginTree()
{
	const auto &plugins = kApp->pluginManager()->pluginSpecs();
	for (const auto &plugin : plugins) {
		QStringList list;
		list << plugin.name() << plugin.version() << plugin.author();

		QTreeWidgetItem *item = new QTreeWidgetItem(ui->pluginTreeWidget, list);
		item->setCheckState(0, Qt::Checked);
		ui->pluginTreeWidget->addTopLevelItem(item);
	}
}


PluginSpec PluginOptionWidget::findPluginSpec(const QString &pluginName)
{
	for (const PluginSpec &plugin : kApp->pluginManager()->pluginSpecs())
		if (pluginName == plugin.name())
			return plugin;
	return PluginSpec();
}
