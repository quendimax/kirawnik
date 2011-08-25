#include <QVBoxLayout>

#include "HeaderView.h"
#include "FileView.h"
#include "FilePanel.h"


FilePanel::FilePanel(QWidget *parent)
    : QWidget(parent)
{
	setFocusPolicy(Qt::StrongFocus);

	m_header = new HeaderView;
	m_fileView = new FileView(m_header);

	QVBoxLayout *layout = new QVBoxLayout;
	layout->setMargin(0);
	layout->setSpacing(0);
	layout->addWidget(m_header);
	layout->addWidget(m_fileView);

	setLayout(layout);
}
