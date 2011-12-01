#include <QPushButton>

#include <pluginsystem/PluginSpec.h>

#include "PluginDetailsView.h"


PluginDetailsView::PluginDetailsView(const PluginSpec &plugin, QWidget *parent)
	: QDialog(parent), ui(new Ui::PluginDetailsView)
{
	setAttribute(Qt::WA_DeleteOnClose);

	ui->setupUi(this);
	setWindowTitle(tr("Details of the %1 plugin").arg(plugin.name()));

	if (QPushButton *button = ui->buttonBox->button(QDialogButtonBox::Close))
		button->setDefault(true);

	ui->nameLabel->setText(plugin.name());
	ui->versionLabel->setText(plugin.version());
	ui->vendorLabel->setText(plugin.vendor());
	ui->authorLabel->setText(plugin.author());
    ui->pathLabel->setText(QString("<a href=\"%1\">%1</a>").arg(plugin.path()));
	ui->categoryLabel->setText(plugin.category());
	ui->urlLabel->setText(QString("<a href=\"%1\">%1</a>").arg(plugin.url()));
	ui->licenseTextEdit->setPlainText(plugin.license());
	ui->descriptionTextEdit->setPlainText(plugin.description());

	foreach (const PluginDependency &dep, plugin.dependencies()) {
		QString dependItem = tr("%1 [%2]").arg(dep.name).arg(dep.version);
		if (dep.type == PluginDependency::Optional)
			dependItem.append(tr(", optional"));
		ui->dependListWidget->addItem(dependItem);
	}

	connect(this, SIGNAL(accepted()), SLOT(close()));
	connect(this, SIGNAL(rejected()), SLOT(close()));
}
