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

	QVBoxLayout *layout = new QVBoxLayout;
	layout->addWidget(m_listWidget);
	layout->addWidget(m_buttonBox);
	setLayout(layout);
}
