#ifndef __APPLICATION_H__
#define __APPLICATION_H__

#include <QtGui/QApplication>

class QSettings;


class Application : public QApplication
{
	Q_OBJECT

public:
	Application(int &argc, char **argv);

	inline QSettings *settings() { return m_settings; }

private:
	QSettings *m_settings;
};


extern Application *kApp;


#endif //__APPLICATION_H__
