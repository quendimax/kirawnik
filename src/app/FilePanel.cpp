#include <QVBoxLayout>

#include "HeaderView.h"
#include "FilePanel.h"


FilePanel::FilePanel(QWidget *parent)
    : QWidget(parent)
{
	setFocusPolicy(Qt::StrongFocus);

	QVBoxLayout *layout = new QVBoxLayout;
	layout->setMargin(0);
	layout->setSpacing(0);
	layout->addWidget(new HeaderView);
	QWidget *w = new QWidget;
	w->setAutoFillBackground(true);
	w->setBackgroundRole(QPalette::Dark);
	layout->addWidget(w);

	setLayout(layout);
}
