#include <QListWidgetItem>
#include <QFormLayout>
#include <QSettings>
#include <QGroupBox>
#include <QLayout>

#include "FileView.h"
#include "ColorButton.h"
#include "Application.h"
#include "ColorOptionWidget.h"


ColorOptionWidget::ColorOptionWidget(QWidget *parent)
    : OptionWidget(parent)
{
	QGroupBox *leftGroupBox = new QGroupBox(tr("General colors"));
	QFormLayout *genColorLayout = new QFormLayout;
	leftGroupBox->setLayout(genColorLayout);

	ColorButton *backgroundButton = new ColorButton("FileView/BaseColor.0");
	genColorLayout->addRow(tr("Background:"), backgroundButton);
	connect(backgroundButton, SIGNAL(colorChanged(QString,QColor)), this, SLOT(setColor(QString,QColor)));

	ColorButton *abackgroundButton = new ColorButton("FileView/BaseColor.1");
	genColorLayout->addRow(tr("Alternate background:"), abackgroundButton);
	connect(abackgroundButton, SIGNAL(colorChanged(QString,QColor)), this, SLOT(setColor(QString,QColor)));

	ColorButton *sbackgroundButton = new ColorButton("FileView/SelectBaseColor");
	genColorLayout->addRow(tr("Select background:"), sbackgroundButton);
	connect(sbackgroundButton, SIGNAL(colorChanged(QString,QColor)), this, SLOT(setColor(QString,QColor)));

	ColorButton *textButton = new ColorButton("FileView/TextColor");
	genColorLayout->addRow(tr("Text:"), textButton);
	connect(textButton, SIGNAL(colorChanged(QString,QColor)), this, SLOT(setColor(QString,QColor)));

	ColorButton *stextButton = new ColorButton("FileView/SelectTextColor");
	genColorLayout->addRow(tr("Select text:"), stextButton);
	connect(stextButton, SIGNAL(colorChanged(QString,QColor)), this, SLOT(setColor(QString,QColor)));

	ColorButton *cursorButton = new ColorButton("FileView/CursorColor");
	genColorLayout->addRow(tr("Cursor:"), cursorButton);
	connect(cursorButton, SIGNAL(colorChanged(QString,QColor)), this, SLOT(setColor(QString,QColor)));

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


void ColorOptionWidget::setColor(const QString &key, const QColor &color)
{
	kApp->settings()->setValue(key, (uint) color.rgb());
	foreach (QWidget *w, kApp->allWidgets()) {
		if (FileView *fv = qobject_cast<FileView *>(w)) {
			fv->settingsUpdate();
		}
	}
}
