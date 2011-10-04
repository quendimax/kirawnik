#include <QSplitter>
#include <QListWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QDialogButtonBox>

#include "OptionDialog.h"


OptionDialog::OptionDialog(QWidget *parent)
    : QDialog(parent)
{
	setWindowTitle(tr("Application options"));

	m_buttonBox = new QDialogButtonBox(QDialogButtonBox::Close);
	m_buttonBox->button(QDialogButtonBox::Close)->setDefault(true);
	connect(m_buttonBox->button(QDialogButtonBox::Close), SIGNAL(clicked()), this, SLOT(hide()));

	m_listWidget = new QListWidget;

	m_splitter = new QSplitter;
	m_splitter->addWidget(m_listWidget);
	m_splitter->addWidget(new QWidget);

	QVBoxLayout *layout = new QVBoxLayout;
	layout->addWidget(m_splitter);
	layout->addWidget(m_buttonBox);
	setLayout(layout);

	m_splitter->setStretchFactor(0, 0);
	m_splitter->setStretchFactor(1, 1);
}
