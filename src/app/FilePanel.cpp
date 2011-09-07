#include <QUrl>
#include <QProcess>
#include <QSettings>
#include <QKeyEvent>
#include <QVBoxLayout>
#include <QDesktopServices>

#include "FileView.h"
#include "HeaderView.h"
#include "Application.h"
#include "FilePanel.h"


FilePanel::FilePanel(const QString &name, QWidget *parent)
    : QWidget(parent)
{
	setObjectName(name);

	m_header = new HeaderView;
	m_fileView = new FileView(m_header);
	connect(m_header, SIGNAL(geometryChanged()), m_fileView, SLOT(updateAll()));

	QVBoxLayout *layout = new QVBoxLayout;
	layout->setMargin(0);
	layout->setSpacing(0);
	layout->addWidget(m_header);
	layout->addWidget(m_fileView);

	setLayout(layout);

	m_currentDir.setSorting(QDir::Name | QDir::DirsFirst | QDir::IgnoreCase | QDir::LocaleAware);

	readSettings();

	m_fileList = m_currentDir.entryInfoList();
	m_fileList.pop_front();
	m_fileView->setFileInfoList(m_fileList);
}


FilePanel::~FilePanel()
{
	writeSettings();
}


void FilePanel::keyPressEvent(QKeyEvent *e)
{
	switch (e->key()) {
	case Qt::Key_Return: {
		const QFileInfo &fi = m_fileList[m_fileView->current()];
		if (fi.isDir()) {
			m_currentDir = QDir(fi.canonicalFilePath());
			m_currentDir.setSorting(QDir::Name | QDir::DirsFirst | QDir::IgnoreCase | QDir::LocaleAware);
			m_fileList = m_currentDir.entryInfoList();
			m_fileList.pop_front();
			if (fi.isRoot()) m_fileList.pop_front();
			m_fileView->setFileInfoList(m_fileList);
		}
		else {
			if (fi.isExecutable())
				QProcess::startDetached(fi.canonicalFilePath());
			else
				QProcess::startDetached(QString("xdg-open"), QStringList() << fi.canonicalFilePath());
		}
		}
		break;
	}
}


void FilePanel::mouseDoubleClickEvent(QMouseEvent *)
{
	const QFileInfo &fi = m_fileList[m_fileView->current()];
	if (fi.isDir()) {
		m_currentDir = QDir(fi.canonicalFilePath());
		m_currentDir.setSorting(QDir::Name | QDir::DirsFirst | QDir::IgnoreCase | QDir::LocaleAware);
		m_fileList = m_currentDir.entryInfoList();
		m_fileList.pop_front();
		if (fi.isRoot()) m_fileList.pop_front();
		m_fileView->setFileInfoList(m_fileList);
	}
	else {
		if (fi.isExecutable())
			QProcess::startDetached(fi.canonicalFilePath());
		else
			QProcess::startDetached(QString("xdg-open"), QStringList() << fi.canonicalFilePath());
	}
}


void FilePanel::readSettings()
{
	QSettings *sets = kApp->settings();
	sets->beginGroup("FilePanel." + objectName());

	m_currentDir.setPath(sets->value("CurrentDir", QDir::homePath()).toString());

	sets->endGroup();
}


void FilePanel::writeSettings()
{
	QSettings *sets = kApp->settings();
	sets->beginGroup("FilePanel." + objectName());

	sets->setValue("CurrentDir", m_currentDir.absolutePath());

	sets->endGroup();
}
