#include <QListWidgetItem>
#include <QFormLayout>
#include <QSettings>
#include <QGroupBox>
#include <QCheckBox>
#include <QLayout>

#include "FileView.h"
#include "ColorButton.h"
#include "Application.h"
#include "ColorOptionWidget.h"


ColorOptionWidget::ColorOptionWidget(QWidget *parent)
    : OptionWidget(parent)
{
	QGroupBox *leftGroupBox = new QGroupBox(tr("General colors"));
	QFormLayout *colorButtonLayout = new QFormLayout;
	QVBoxLayout *generalGroupLayout = new QVBoxLayout;
	leftGroupBox->setLayout(generalGroupLayout);

	ColorButton *cursorButton = new ColorButton("FileView/CursorColor");
	colorButtonLayout->addRow(tr("Cursor:"), cursorButton);
	connect(cursorButton, SIGNAL(colorChanged(QString,QColor)), this, SLOT(setColor(QString,QColor)));

	ColorButton *textButton = new ColorButton("FileView/TextColor");
	colorButtonLayout->addRow(tr("Text:"), textButton);
	connect(textButton, SIGNAL(colorChanged(QString,QColor)), this, SLOT(setColor(QString,QColor)));

	ColorButton *stextButton = new ColorButton("FileView/SelectTextColor");
	colorButtonLayout->addRow(tr("Select text:"), stextButton);
	connect(stextButton, SIGNAL(colorChanged(QString,QColor)), this, SLOT(setColor(QString,QColor)));

	ColorButton *backgroundButton = new ColorButton("FileView/BaseColor.0");
	colorButtonLayout->addRow(tr("Background:"), backgroundButton);
	connect(backgroundButton, SIGNAL(colorChanged(QString,QColor)), this, SLOT(setColor(QString,QColor)));

	ColorButton *abackgroundButton = new ColorButton("FileView/BaseColor.1");
	colorButtonLayout->addRow(tr("Alternate background:"), abackgroundButton);
	connect(abackgroundButton, SIGNAL(colorChanged(QString,QColor)), this, SLOT(setColor(QString,QColor)));

	ColorButton *sbackgroundButton = new ColorButton("FileView/SelectBaseColor");
	colorButtonLayout->addRow(tr("Select background:"), sbackgroundButton);
	connect(sbackgroundButton, SIGNAL(colorChanged(QString,QColor)), this, SLOT(setColor(QString,QColor)));

	QCheckBox *sbgCheckBox = new QCheckBox(tr("Use select background"));
	connect(sbgCheckBox, SIGNAL(toggled(bool)), this, SLOT(setShowSelectBackground(bool)));

	generalGroupLayout->addLayout(colorButtonLayout);
	generalGroupLayout->addWidget(sbgCheckBox);

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


void ColorOptionWidget::setShowSelectBackground(bool turn)
{
	kApp->settings()->setValue("FileView/ShowSelectBackground", turn);
	foreach (QWidget *w, kApp->allWidgets()) {
		if (FileView *fv = qobject_cast<FileView *>(w)) {
			fv->settingsUpdate();
		}
	}
}
