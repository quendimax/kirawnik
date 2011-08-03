#ifndef __APPLICATION_H__
#define __APPLICATION_H__

#include <QtGui/QApplication>

class QSettings;


/*!
  The Application class inherits the QApplication class and copies
  its features. Also it contains the global object of the QSettings class.

  Only one the Application object must be in a application.
  */
class Application : public QApplication
{
	Q_OBJECT

public:
	explicit Application(int &argc, char **argv);
	virtual ~Application();

	QSettings *settings() const;

private:
	QSettings *m_settings;
};


/*! Returns application's the Settings class object */
inline QSettings *Application::settings() const { return m_settings; }


/*! Returns the global pointer to the object of the Application class */
#define kApp static_cast<Application *>(QCoreApplication::instance())


#endif //__APPLICATION_H__
