#include <QLabel>
#include <QSplitter>
#include <QScrollArea>
#include <QListWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QDialogButtonBox>

#include "ColorOptionWidget.h"
#include "FontOptionWidget.h"
#include "PluginOptionWidget.h"

#include "OptionDialog.h"


OptionDialog::OptionDialog(QWidget *parent)
    : QDialog(parent)
{
	setWindowTitle(tr("Application options"));

	createOptionWidgets();
	createListWidget();

	m_buttonBox = new QDialogButtonBox(QDialogButtonBox::Close);
	m_buttonBox->button(QDialogButtonBox::Close)->setDefault(true);
	connect(m_buttonBox->button(QDialogButtonBox::Close), SIGNAL(clicked()), this, SLOT(hide()));

	m_optionLabel = new QLabel(QString("<h3>%1</h3>").arg(m_listWidget->item(0)->text()));

	m_optionWidget = new QWidget;
	m_optionWidget->setLayout(m_optionLayout = new QVBoxLayout);

	m_optionLayout->addWidget(m_optionLabel);
	m_optionLayout->addWidget(m_optionWidgets.first());

	m_splitter = new QSplitter;
	m_splitter->addWidget(m_listWidget);
	m_splitter->addWidget(m_optionWidget);

	QVBoxLayout *layout = new QVBoxLayout;
	layout->addWidget(m_splitter);
	layout->addWidget(m_buttonBox);
	setLayout(layout);

	m_splitter->setStretchFactor(0, 0);
	m_splitter->setStretchFactor(1, 1);

	setMinimumSize(600, 500);
	m_splitter->setSizes(QList<int>() << 160 << 400);
	resize(640, 500);
}


OptionDialog::~OptionDialog()
{
	if (QLayoutItem *item = m_optionLayout->takeAt(1))
		delete item;
	qDeleteAll(m_optionWidgets);
}


void OptionDialog::setCurrentOption(int index)
{
	if (QLayoutItem *item = m_optionLayout->takeAt(1)) {
		item->widget()->hide();
		delete item;
	}
	m_optionLayout->addWidget(m_optionWidgets[index]);
	m_optionLabel->setText(QString("<h3>%1</h3>").arg(m_listWidget->item(index)->text()));
	m_optionWidgets[index]->show();
}


void OptionDialog::createOptionWidgets()
{
	m_optionWidgets.clear();

	m_optionWidgets.append(new ColorOptionWidget);
	m_optionWidgets.append(new FontOptionWidget);
	m_optionWidgets.append(new PluginOptionWidget);
}


void OptionDialog::createListWidget()
{
	m_listWidget = new QListWidget;
	m_listWidget->setIconSize(QSize(32, 32));
	connect(m_listWidget, SIGNAL(currentRowChanged(int)), this, SLOT(setCurrentOption(int)));

	foreach (OptionWidget *widget, m_optionWidgets) {
		m_listWidget->addItem(widget->createListWidgetItem());
	}
}
