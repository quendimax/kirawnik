#ifndef __APP_OPTIONS_FONTOPTINOWIDGET_H__
#define __APP_OPTIONS_FONTOPTINOWIDGET_H__

#include "OptionWidget.h"

class QLabel;
class QTextEdit;
class QPushButton;


class FontOptionWidget : public OptionWidget
{
	Q_OBJECT

public:
	FontOptionWidget(QWidget *parent = 0);

	QListWidgetItem *createListWidgetItem() const;

private slots:
	void changeFilePanelFont();

private:
	void initItems();
	void updateFileView();

private:
	QLabel *m_filePanelLabel;
	QPushButton *m_filePanelChooseButton;
	QTextEdit *m_fontExampleTextEdit;
};


#endif //__APP_OPTIONS_FONTOPTINOWIDGET_H__
