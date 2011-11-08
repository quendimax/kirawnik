#include <QPushButton>

#include <pluginsystem/PluginSpec.h>

#include "PluginDetailsView.h"


PluginDetailsView::PluginDetailsView(const PluginSpec *plugin, QWidget *parent)
	: QDialog(parent), ui(new Ui::PluginDetailsView)
{
	if (QPushButton *button = ui->buttonBox->button(QDialogButtonBox::Close))
		button->setDefault(true);

	ui->nameLabel->setText(plugin->name());
	ui->versionLabel->setText(plugin->version());
	ui->vendorLabel->setText(plugin->vendor());
	ui->authorLabel->setText(plugin->author());
	ui->urlLabel->setText(plugin->url());
	ui->licenseTextEdit->setPlainText(plugin->license());
	ui->descriptionTextEdit->setPlainText(plugin->description());

	for (auto &dep : plugin->dependencies()) {
		QString dependItem = QString("%1 - %2").arg(dep.name).arg(dep.version);
		if (dep.type == PluginDependency::Optional)
			dependItem.append(", optional");
		ui->dependListWidget->addItem(dependItem);
	}
}
