#include <QListWidgetItem>
#include <QFormLayout>
#include <QGroupBox>
#include <QLayout>

#include "ColorButton.h"
#include "Application.h"
#include "ColorOptionWidget.h"


ColorOptionWidget::ColorOptionWidget(QWidget *parent)
    : OptionWidget(parent)
{
	QGroupBox *leftGroupBox = new QGroupBox(tr("General colors"));
	QFormLayout *genColorLayout = new QFormLayout;
	leftGroupBox->setLayout(genColorLayout);

	genColorLayout->addRow(tr("Background:"), new ColorButton);
	genColorLayout->addRow(tr("Alternate background:"), new ColorButton);
	genColorLayout->addRow(tr("Select background:"), new ColorButton);
	genColorLayout->addRow(tr("Text:"), new ColorButton);
	genColorLayout->addRow(tr("Select text:"), new ColorButton);

	QGridLayout *mainLayout = new QGridLayout;
	mainLayout->addWidget(leftGroupBox, 0, 0);

	setLayout(mainLayout);
}


QListWidgetItem *ColorOptionWidget::createListWidgetItem() const
{
	QListWidgetItem *item = new QListWidgetItem(QIcon(kApp->resourceFile("icons/ColorOptions.png")),
	                                            tr("Colors", "Title of color option widget"));
	return item;
}
