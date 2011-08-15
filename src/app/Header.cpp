#include <QtGui/QLabel>

#include "Header.h"


Header::Header(QWidget *parent)
	: QWidget(parent)
{
	m_labels[0] = new QLabel(tr("Name"), this);
}
