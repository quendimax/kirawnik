#include <QSettings>

#include "Application.h"
#include "FileView.h"


FileView::FileView(HeaderView *header, QWidget *parent)
    : QWidget(parent), e_header(header)
{
	readSettings();
}


FileView::~FileView()
{
	writeSettings();
}


void FileView::readSettings()
{
	bool ok;
	QSettings *sets = kApp->settings();
	sets->beginGroup("FileView");

	m_font.setFamily(sets->value("Font", font().family()).toString());
	m_cursorIsFull = sets->value("CursorIsFull", false).toBool();
	m_cursorColor.setRgb((QRgb) sets->value("CursorColor", (uint) QColor(Qt::green).rgb()).toUInt(&ok));
	Q_ASSERT(ok);
	m_curentTextColor.setRgb((QRgb) sets->value("CurentTextColor", (uint) QColor(Qt::green).rgb()).toUInt(&ok));
	Q_ASSERT(ok);
	m_textColor.setRgb((QRgb) sets->value("TextColor", (uint) QColor(Qt::green).rgb()).toUInt(&ok));
	Q_ASSERT(ok);
	m_backgroundColor[0].setRgb((QRgb) sets->value("BackgroundColor.0", (uint) QColor(Qt::green).rgb()).toUInt(&ok));
	Q_ASSERT(ok);
	m_backgroundColor[1].setRgb((QRgb) sets->value("BackgroundColor.1", (uint) QColor(Qt::green).rgb()).toUInt(&ok));
	Q_ASSERT(ok);
	m_selectTextColor.setRgb((QRgb) sets->value("SelectTextColor", (uint) QColor(Qt::green).rgb()).toUInt(&ok));
	Q_ASSERT(ok);
	m_selectBackgroundColor[0].setRgb((QRgb) sets->value("SelectBackgroundColor.0", (uint) QColor(Qt::green).rgb()).toUInt(&ok));
	Q_ASSERT(ok);
	m_selectBackgroundColor[1].setRgb((QRgb) sets->value("SelectBackgroundColor.1", (uint) QColor(Qt::green).rgb()).toUInt(&ok));
	Q_ASSERT(ok);

	sets->endGroup();
}


void FileView::writeSettings()
{
	QSettings *sets = kApp->settings();
	sets->beginGroup("FileView");

	sets->setValue("Font", m_font.family());
	sets->setValue("CursorIsFull", m_cursorIsFull);
	sets->setValue("CursorColor", (uint) m_cursorColor.rgb());
	sets->setValue("CurentTextColor", (uint) m_curentTextColor.rgb());
	sets->setValue("TextColor", (uint) m_textColor.rgb());
	sets->setValue("BackgroundColor.0", (uint) m_backgroundColor[0].rgb());
	sets->setValue("BackgroundColor.1", (uint) m_backgroundColor[1].rgb());
	sets->setValue("SelectTextColor", (uint) m_selectTextColor.rgb());
	sets->setValue("SelectBackgroundColor.0", (uint) m_selectBackgroundColor[0].rgb());
	sets->setValue("SelectBackgroundColor.1", (uint) m_selectBackgroundColor[1].rgb());

	sets->endGroup();
}
