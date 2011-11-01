#include <QListWidgetItem>

#include <core/Application.h>

#include "ui_PluginOptionWidget.h"
#include "PluginOptionWidget.h"


PluginOptionWidget::PluginOptionWidget(QWidget *parent)
	: OptionWidget(parent), ui(new Ui::PluginOptionWidget())
{
	ui->setupUi(this);
	setMinimumHeight(400);

	ui->detailsButton->setDisabled(true);
}


PluginOptionWidget::~PluginOptionWidget()
{
	delete ui;
}


QListWidgetItem *PluginOptionWidget::createListWidgetItem() const
{
	QListWidgetItem *item = new QListWidgetItem(QIcon(kApp->resourceFile("pixmaps/PluginOptions.png")),
												tr("Plugins", "Name of plugin option widget"));
	return item;
}
