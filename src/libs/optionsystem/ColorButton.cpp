#include <QPainter>
#include <QSettings>
#include <QColorDialog>

#include <core/Application.h>

#include "ColorButton.h"


ColorButton::ColorButton(const QString &settingKey, QWidget *parent)
    : QFrame(parent), m_settingKey(settingKey)
{
	setLineWidth(2);
	setFrameStyle(QFrame::Panel | QFrame::Raised);
	setMinimumSize(64, 24);

	bool ok;
	m_color.setRgb((QRgb) kApp->settings()->value(m_settingKey).toUInt(&ok));
	Q_ASSERT(ok);
}


void ColorButton::paintEvent(QPaintEvent *event)
{
	QFrame::paintEvent(event);

	const int Margin = lineWidth();
	QPainter painter(this);
	QRect colorRect;
	colorRect.setLeft(rect().left() + Margin);
	colorRect.setRight(rect().right() - Margin);
	colorRect.setTop(rect().top() + Margin);
	colorRect.setBottom(rect().bottom() - Margin);

	painter.fillRect(colorRect, m_color);
}


void ColorButton::mousePressEvent(QMouseEvent *)
{
	setFrameStyle(QFrame::Panel | QFrame::Sunken);
}


void ColorButton::mouseReleaseEvent(QMouseEvent *)
{
	setFrameStyle(QFrame::Panel | QFrame::Raised);

	QColor newColor = QColorDialog::getColor(m_color, this);

	if (newColor.isValid())
		if (m_color != newColor) {
			m_color = newColor;
			emit colorChanged(m_settingKey, m_color);
		}
}
