#ifndef __HEADER_H__
#define __HEADER_H__

#include <QtGui/QWidget>

class QLabel;


class Header : public QWidget
{
	Q_OBJECT

public:
	explicit Header(QWidget *parent = 0);

private:
	const int m_count = 5;
	QLabel *m_labels[m_count];
};


#endif //__HEADER_H__
