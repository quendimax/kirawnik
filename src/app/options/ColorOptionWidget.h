#ifndef __APP_OPTIONS_COLOROPTIONWIDGET_H__
#define __APP_OPTIONS_COLOROPTIONWIDGET_H__

#include "OptionWidget.h"


class ColorOptionWidget : public OptionWidget
{
	Q_OBJECT

public:
	ColorOptionWidget(QWidget *parent = 0);

	QListWidgetItem *createListWidgetItem() const;

private slots:
	void setColor(const QString &key, const QColor &color);
	void setShowSelectBackground(bool);
};


#endif //__APP_OPTIONS_COLOROPTIONWIDGET_H__
