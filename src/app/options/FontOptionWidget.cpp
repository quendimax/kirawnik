#include <QLabel>
#include <QLayout>
#include <QTextEdit>
#include <QGroupBox>
#include <QSettings>
#include <QFontDialog>
#include <QGridLayout>
#include <QPushButton>
#include <QListWidgetItem>

#include "FileView.h"
#include "Application.h"
#include "FontOptionWidget.h"


FontOptionWidget::FontOptionWidget(QWidget *parent)
    : OptionWidget(parent)
{
	QGroupBox *filePanelGroupBox = new QGroupBox(tr("File panel font"));
	QGridLayout *filePanelLayout = new QGridLayout;
	filePanelGroupBox->setLayout(filePanelLayout);

	m_filePanelLabel = new QLabel;
	m_filePanelChooseButton = new QPushButton(tr("Choose...", "Font choose"));
	m_fontExampleTextEdit = new QTextEdit(tr("Text example"));
	filePanelLayout->addWidget(m_filePanelLabel, 0, 0);
	filePanelLayout->addWidget(m_filePanelChooseButton, 0, 1);
	filePanelLayout->addWidget(m_fontExampleTextEdit, 1, 0, 1, 2);

	QVBoxLayout *mainLayout = new QVBoxLayout;
	setLayout(mainLayout);
	mainLayout->addWidget(filePanelGroupBox);

	initItems();

	connect(m_filePanelChooseButton, SIGNAL(clicked()), this, SLOT(changeFilePanelFont()));
}


QListWidgetItem *FontOptionWidget::createListWidgetItem() const
{
	QListWidgetItem *item = new QListWidgetItem(QIcon(kApp->resourceFile("pixmaps/FontOptions.png")),
	                                            tr("Fonts", "Name of option widget"));
	return item;
}


void FontOptionWidget::changeFilePanelFont()
{
	bool ok;
	QFont oldFont(kApp->settings()->value("FileView/Font").toString(),
	              kApp->settings()->value("FileView/FontSize").toInt(),
	              kApp->settings()->value("FileView/FontWeight").toInt(),
	              kApp->settings()->value("FileView/FontItalic").toBool());
	QFont newFont = QFontDialog::getFont(&ok, oldFont, this);

	if (newFont != oldFont) {
		kApp->settings()->setValue("FileView/Font", newFont.family());
		kApp->settings()->setValue("FileView/FontSize", newFont.pointSizeF());
		kApp->settings()->setValue("FileView/FontWeight", newFont.weight());
		kApp->settings()->setValue("FileView/FontItalic", newFont.italic());

		initItems();
		updateFileView();
	}
}


void FontOptionWidget::initItems()
{
	QString labelText = QString("%1, %2")\
	                    .arg(kApp->settings()->value("FileView/Font").toString())\
	                    .arg(kApp->settings()->value("FileView/FontSize").toInt());
	m_filePanelLabel->setText(labelText);

	QFont textFont(kApp->settings()->value("FileView/Font").toString(),
	               kApp->settings()->value("FileView/FontSize").toInt(),
	               kApp->settings()->value("FileView/FontWeight").toInt(),
	               kApp->settings()->value("FileView/FontItalic").toBool());
	m_fontExampleTextEdit->document()->setDefaultFont(textFont);
	m_filePanelLabel->setFont(m_fontExampleTextEdit->font());
}


void FontOptionWidget::updateFileView()
{
	foreach (QWidget *w, kApp->allWidgets()) {
		if (FileView *fv = qobject_cast<FileView *>(w)) {
			fv->settingsUpdate();
		}
	}
}
