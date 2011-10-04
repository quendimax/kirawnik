#ifndef __APP_OPTIONS_OPTIONWIDGET_H__
#define __APP_OPTIONS_OPTIONWIDGET_H__

#include <QWidget>

class QListWidgetItem;


class OptionWidget : public QWidget
{
	Q_OBJECT

public:
	OptionWidget(QWidget *parent = 0) : QWidget(parent) {}

	QListWidgetItem *listWidgetItem() const = 0;
};


#endif //__APP_OPTIONS_OPTIONWIDGET_H__
