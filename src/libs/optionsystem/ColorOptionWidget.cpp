#include <QListWidgetItem>
#include <QFormLayout>
#include <QSettings>
#include <QGroupBox>
#include <QCheckBox>
#include <QLayout>

#include <core/FileView.h>
#include <core/Application.h>

#include "ColorButton.h"
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
	sbgCheckBox->setCheckState(kApp->settings()->value("FileView/ShowSelectBackground").toBool() ? Qt::Checked : Qt::Unchecked);
	connect(sbgCheckBox, SIGNAL(toggled(bool)), this, SLOT(setShowSelectBackground(bool)));

	QCheckBox *cursorFullCheckBox = new QCheckBox(tr("Use full cursor"));
	cursorFullCheckBox->setCheckState(kApp->settings()->value("FileView/CursorIsFull").toBool() ? Qt::Checked : Qt::Unchecked);
	connect(cursorFullCheckBox, SIGNAL(toggled(bool)), this, SLOT(setFullCursor(bool)));

	generalGroupLayout->addLayout(colorButtonLayout);
	generalGroupLayout->addWidget(sbgCheckBox);
	generalGroupLayout->addWidget(cursorFullCheckBox);

	QGridLayout *mainLayout = new QGridLayout;
	mainLayout->addWidget(leftGroupBox, 0, 0);

	setLayout(mainLayout);
}


QListWidgetItem *ColorOptionWidget::createListWidgetItem() const
{
	QListWidgetItem *item = new QListWidgetItem(QIcon(kApp->resourceFile("pixmaps/ColorOptions.png")),
	                                            tr("Colors", "Title of color option widget"));
	return item;
}


void ColorOptionWidget::setColor(const QString &key, const QColor &color)
{
	kApp->settings()->setValue(key, (uint) color.rgb());
	updateFileView();
}


void ColorOptionWidget::setShowSelectBackground(bool turn)
{
	kApp->settings()->setValue("FileView/ShowSelectBackground", turn);
	updateFileView();
}


void ColorOptionWidget::setFullCursor(bool turn)
{
	kApp->settings()->setValue("FileView/CursorIsFull", turn);
	updateFileView();
}


void ColorOptionWidget::updateFileView()
{
	foreach (QWidget *w, kApp->allWidgets()) {
		if (FileView *fv = qobject_cast<FileView *>(w)) {
			fv->settingsUpdate();
		}
	}
}
