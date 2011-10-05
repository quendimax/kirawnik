#ifndef __APP_OPTIONS_COLORBUTTON_H__
#define __APP_OPTIONS_COLORBUTTON_H__

#include <QFrame>


class ColorButton : public QFrame
{
	Q_OBJECT

public:
	ColorButton(QWidget *parent = 0);

	inline QColor color() const { return m_color; }
	inline void setColor(const QColor &color) { m_color = color; }

signals:
	void colorChanged(const QColor &newColor);

protected:
	void paintEvent(QPaintEvent *);
	void mousePressEvent(QMouseEvent *);
	void mouseReleaseEvent(QMouseEvent *);

private:
	QColor m_color;
};


#endif //__APP_OPTIONS_COLORBUTTON_H__
