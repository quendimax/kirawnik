#ifndef __APP_APPLICATION_H__
#define __APP_APPLICATION_H__

#include <QApplication>

class QSettings;
class PluginManager;


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
	PluginManager *pluginManager() const;

	static QString resourceFile(const QString &shortFileName);

private:
	QSettings *m_settings;
	PluginManager *m_pluginManager;
};


/*! Returns application's the Settings class object */
inline QSettings *Application::settings() const { return m_settings; }
inline PluginManager *Application::pluginManager() const { return m_pluginManager; }


/*! Returns the global pointer to the object of the Application class */
#define kApp static_cast<Application *>(QCoreApplication::instance())


#endif //__APP_APPLICATION_H__
