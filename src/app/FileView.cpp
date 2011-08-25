#include <QPainter>
#include <QSettings>
#include <QScrollBar>
#include <QResizeEvent>

#include "Application.h"
#include "FileView.h"


FileView::FileView(HeaderView *header, QWidget *parent)
    : QWidget(parent), e_header(header)
{
	m_scroll = new QScrollBar(Qt::Vertical, this);
	m_scroll->setFixedWidth(16);
	m_scroll->hide();

	m_start = 0;
	m_current = 0;

	readSettings();
}


FileView::~FileView()
{
	writeSettings();
}


void FileView::paintEvent(QPaintEvent *)
{
	QPainter painter(this);

	paintBackground(painter);
}


void FileView::resizeEvent(QResizeEvent *e)
{
	m_scroll->setGeometry(width() - m_scroll->width(), 0, m_scroll->width(), e->size().height());
}


void FileView::keyPressEvent(QKeyEvent *)
{

}


inline void FileView::paintBackground(QPainter &painter)
{
	painter.fillRect(rect(), m_baseColor[0]);

	// if m_start odd then start from alternate color
	int y = (m_start & 1) ? m_itemHeight : 0;
	int i = (m_start & 1) ? m_start : m_start + 1;

	while (i < m_fileList.size() && y < height()) {
		QRect rect(0, y, width(), m_itemHeight);
		painter.fillRect(rect, m_baseColor[1]);

		y += 2*m_itemHeight;
		i += 2;
	}
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
	m_curentTextColor.setRgb((QRgb) sets->value("CurentTextColor", (uint) palette().color(QPalette::Text).rgb()).toUInt(&ok));
	Q_ASSERT(ok);
	m_textColor.setRgb((QRgb) sets->value("TextColor", (uint) palette().color(QPalette::Text).rgb()).toUInt(&ok));
	Q_ASSERT(ok);
	m_baseColor[0].setRgb((QRgb) sets->value("BaseColor.0", (uint) palette().color(QPalette::Base).rgb()).toUInt(&ok));
	Q_ASSERT(ok);
	m_baseColor[1].setRgb((QRgb) sets->value("BaseColor.1", (uint) palette().color(QPalette::AlternateBase).rgb()).toUInt(&ok));
	Q_ASSERT(ok);
	m_selectTextColor.setRgb((QRgb) sets->value("SelectTextColor",
	                                            (uint) palette().color(QPalette::HighlightedText).rgb()).toUInt(&ok));
	Q_ASSERT(ok);
	m_selectBaseColor.setRgb((QRgb) sets->value("SelectBaseColor",
	                                            (uint) palette().color(QPalette::Highlight).rgb()).toUInt(&ok));
	Q_ASSERT(ok);
	m_itemHeight = sets->value("ItemHeight", 16).toInt(&ok);
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
	sets->setValue("BaseColor.0", (uint) m_baseColor[0].rgb());
	sets->setValue("BaseColor.1", (uint) m_baseColor[1].rgb());
	sets->setValue("SelectTextColor", (uint) m_selectTextColor.rgb());
	sets->setValue("SelectBaseColor", (uint) m_selectBaseColor.rgb());
	sets->setValue("ItemHeight", m_itemHeight);

	sets->endGroup();
}
