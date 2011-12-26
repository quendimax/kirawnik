#include <QPushButton>
#include <QLayout>
#include <QSplitter>

#include "FilePanel.h"
#include "FileSystemView.h"


FilePanel *FileSystemView::m_leftPanel = nullptr;
FilePanel *FileSystemView::m_rightPanel = nullptr;
FilePanel *FileSystemView::m_activePanel = nullptr;
FilePanel *FileSystemView::m_inactivePanel = nullptr;


FileSystemView::FileSystemView(QWidget *parent)
    : QWidget(parent)
{
	setFocusPolicy(Qt::NoFocus);
	QHBoxLayout *barLayout = new QHBoxLayout;
	barLayout->setMargin(2);
	barLayout->addWidget(new QPushButton("F2 Rename"));
	barLayout->addWidget(new QPushButton("F3 View"));
	barLayout->addWidget(new QPushButton("F4 Edit"));
	barLayout->addWidget(new QPushButton("F5 Copy"));
	barLayout->addWidget(new QPushButton("F6 Move"));
	barLayout->addWidget(new QPushButton("F8 Remove"));
	barLayout->addWidget(new QPushButton("F10 Exit"));
	for (int i = 0; i < 7; i++)
		barLayout->itemAt(i)->widget()->setFocusPolicy(Qt::NoFocus);

	m_functionalBar = new QWidget;
	m_functionalBar->setLayout(barLayout);
	m_functionalBar->adjustSize();
	m_functionalBar->setFixedHeight(m_functionalBar->height());

	m_leftPanel = new FilePanel("Left");
	m_rightPanel = new FilePanel("Right");

	QSplitter *splitter = new QSplitter(Qt::Horizontal);
	splitter->setContentsMargins(0, 0, 0, 0);
	splitter->setFrameStyle(QFrame::NoFrame);
	splitter->addWidget(m_leftPanel);
	splitter->addWidget(m_rightPanel);

	QVBoxLayout *vLayout= new QVBoxLayout;
	vLayout->setMargin(2);

	vLayout->addWidget(splitter);
	vLayout->addWidget(m_functionalBar);

	setLayout(vLayout);
}


FileSystemView::~FileSystemView()
{
}
