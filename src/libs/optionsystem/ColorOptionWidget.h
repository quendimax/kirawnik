#ifndef __OPTIONSYSTEM_COLOROPTIONWIDGET_H__
#define __OPTIONSYSTEM_COLOROPTIONWIDGET_H__

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
	void setFullCursor(bool);

private:
	void updateFileView();
};


#endif //__OPTIONSYSTEM_COLOROPTIONWIDGET_H__
