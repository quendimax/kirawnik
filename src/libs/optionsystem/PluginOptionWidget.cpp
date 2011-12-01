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
			this, SLOT(sendEnablePluginSignal(QTreeWidgetItem *, int)));
	connect(ui->detailsButton, SIGNAL(clicked()), this, SLOT(showPluginDetailsView()));
	connect(this, SIGNAL(pluginStateChanged(QString, bool)), kApp->pluginManager(), SLOT(enablePlugin(QString, bool)));
	connect(kApp->pluginManager(), SIGNAL(pluginEnabled(QString,bool)), this, SLOT(setPluginCheckBox(QString, bool)));
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
	PluginDetailsView *view = new PluginDetailsView(plugSpec, this);
	view->show();
}


void PluginOptionWidget::sendEnablePluginSignal(QTreeWidgetItem *item, int)
{
	emit pluginStateChanged(item->text(0), item->checkState(0) == Qt::Checked);
}


void PluginOptionWidget::enableDetailsButton(const QModelIndex &)
{
	ui->detailsButton->setEnabled(true);
}


void PluginOptionWidget::setPluginCheckBox(const QString &pluginName, bool on)
{
	Q_ASSERT(m_items.contains(pluginName));

	m_items[pluginName]->setCheckState(0, on ? Qt::Checked : Qt::Unchecked);
}


void PluginOptionWidget::initPluginTree()
{
	const QList<PluginSpec> &plugins = kApp->pluginManager()->pluginSpecs();
	foreach (const PluginSpec &plugin, plugins) {
		QStringList list;
		list << plugin.name() << plugin.version() << plugin.author();

		QTreeWidgetItem *item = new QTreeWidgetItem(ui->pluginTreeWidget, list);
		if (plugin.willLoad())
			item->setCheckState(0, Qt::Checked);
		else
			item->setCheckState(0, Qt::Unchecked);
		ui->pluginTreeWidget->addTopLevelItem(item);
		m_items[plugin.name()] = item;
	}
}


PluginSpec PluginOptionWidget::findPluginSpec(const QString &pluginName)
{
	foreach (const PluginSpec &plugin, kApp->pluginManager()->pluginSpecs())
		if (pluginName == plugin.name())
			return plugin;
	return PluginSpec();
}
