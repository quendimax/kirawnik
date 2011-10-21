#ifndef __APP_OPTIONS_OPTIONDIALOG_H__
#define __APP_OPTIONS_OPTIONDIALOG_H__

#include <QList>
#include <QDialog>

class QLabel;
class QSplitter;
class QVBoxLayout;
class QListWidget;
class QDialogButtonBox;

class OptionWidget;


class OptionDialog : public QDialog
{
	Q_OBJECT

public:
	OptionDialog(QWidget *parent = 0);

private slots:
	void setCurrentOption(int index);

private:
	void createOptionWidgets();
	void createListWidget();

private:
	QDialogButtonBox *m_buttonBox;
	QListWidget *m_listWidget;
	QWidget *m_optionWidget;
	QLabel *m_optionLabel;
	QSplitter *m_splitter;
	QVBoxLayout *m_optionLayout;
	QList<OptionWidget *> m_optionWidgets;
};


#endif //__APP_OPTIONS_OPTIONDIALOG_H__
