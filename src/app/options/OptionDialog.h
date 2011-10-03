#ifndef __APP_OPTIONS_OPTIONDIALOG_H__
#define __APP_OPTIONS_OPTIONDIALOG_H__

#include <QDialog>

class QListWidget;
class QDialogButtonBox;


class OptionDialog : public QDialog
{
	Q_OBJECT

public:
	OptionDialog(QWidget *parent = 0);

private:
	QDialogButtonBox *m_buttonBox;
	QListWidget *m_listWidget;
};

#endif //__APP_OPTIONS_OPTIONDIALOG_H__
