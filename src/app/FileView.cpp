#include <QDir>
#include <QIcon>
#include <QPainter>
#include <QSettings>
#include <QDateTime>
#include <QScrollBar>
#include <QResizeEvent>
#include <QFileIconProvider>

#include "Application.h"
#include "HeaderView.h"
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

	QDir dir("/usr/bin");
	setFileInfoList(dir.entryInfoList());
}


FileView::~FileView()
{
	writeSettings();
}


void FileView::paintEvent(QPaintEvent *)
{
	QPainter painter(this);

	paintBackground(painter);

	static void (FileView::*drawPart[Krw::Sort_End])(QPainter &) = {
		&FileView::drawNamePart,
		&FileView::drawSuffixPart,
		&FileView::drawSizePart,
		&FileView::drawTextPermsPart,
		&FileView::drawDigitPermsPart,
		&FileView::drawOwnerPart,
		&FileView::drawGroupPart,
		&FileView::drawModifiedPart
	};

	for (int i = 0; i < e_header->count(); i++) {
		if (e_header->sectionIsShowing(i)) {
			void (FileView::*drawMethod)(QPainter &) = drawPart[e_header->sectionType(i)];
			(this->*drawMethod)(painter);
		}
	}
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
	int y = (m_start & 1) ? 0 : m_itemHeight;
	int i = (m_start & 1) ? m_start : m_start + 1;

	while (i < m_fileList.size() && y < height()) {
		QRect rect(0, y, width(), m_itemHeight);
		painter.fillRect(rect, m_baseColor[1]);

		y += 2*m_itemHeight;
		i += 2;
	}
}


void FileView::drawNamePart(QPainter &painter)
{
	int y = 0;
	int i = m_start;
	int sectionIndex = e_header->logicalIndex(Krw::Sort_Name);

	while (i < m_fileList.size() && y < height()) {
		QRect rect;
		rect.setTop(y);
		rect.setLeft(e_header->sectionOffset(sectionIndex) + m_itemHeight + 4);
		rect.setWidth(e_header->sectionSize(sectionIndex) - (m_itemHeight + 4));
		rect.setHeight(m_itemHeight);

		QIcon icon = m_iconProvider.icon(m_fileList[i]);
		icon.paint(&painter, 1, y, m_itemHeight, m_itemHeight);

		painter.drawText(rect, Qt::AlignLeft | Qt::AlignVCenter, m_fileList[i].baseName());

		y += m_itemHeight;
		i++;
	}
}


void FileView::drawSuffixPart(QPainter &painter)
{
	int y = 0;
	int i = m_start;
	int sectionIndex = e_header->logicalIndex(Krw::Sort_Suffix);

	while (i < m_fileList.size() && y < height()) {
		QRect rect;
		rect.setTop(y);
		rect.setLeft(e_header->sectionOffset(sectionIndex));
		rect.setWidth(e_header->sectionSize(sectionIndex));
		rect.setHeight(m_itemHeight);

		painter.drawText(rect, Qt::AlignLeft | Qt::AlignVCenter, m_fileList[i].suffix());

		y += m_itemHeight;
		i++;
	}
}


void FileView::drawSizePart(QPainter &painter)
{
	int y = 0;
	int i = m_start;
	int sectionIndex = e_header->logicalIndex(Krw::Sort_Size);

	while (i < m_fileList.size() && y < height()) {
		QRect rect;
		rect.setTop(y);
		rect.setLeft(e_header->sectionOffset(sectionIndex));
		rect.setWidth(e_header->sectionSize(sectionIndex));
		rect.setHeight(m_itemHeight);

		painter.drawText(rect, Qt::AlignRight | Qt::AlignVCenter, QString::number(m_fileList[i].size()));

		y += m_itemHeight;
		i++;
	}
}


void FileView::drawTextPermsPart(QPainter &painter)
{
	int y = 0;
	int i = m_start;
	int sectionIndex = e_header->logicalIndex(Krw::Sort_TextPerms);

	while (i < m_fileList.size() && y < height()) {
		QRect rect;
		rect.setTop(y);
		rect.setLeft(e_header->sectionOffset(sectionIndex));
		rect.setWidth(e_header->sectionSize(sectionIndex));
		rect.setHeight(m_itemHeight);

		QIcon icon = m_iconProvider.icon(m_fileList[i]);
		icon.paint(&painter, 1, y, m_itemHeight, m_itemHeight);

		QFile::Permissions p = m_fileList[i].permissions();
		QString perms("");
		perms += p.testFlag(QFile::ReadUser) ? "r" : "-";
		perms += p.testFlag(QFile::WriteUser) ? "w" : "-";
		perms += p.testFlag(QFile::ExeUser) ? "x" : "-";
		perms += p.testFlag(QFile::ReadGroup) ? "r" : "-";
		perms += p.testFlag(QFile::WriteGroup) ? "w" : "-";
		perms += p.testFlag(QFile::ExeGroup) ? "x" : "-";
		perms += p.testFlag(QFile::ReadOther) ? "r" : "-";
		perms += p.testFlag(QFile::WriteOther) ? "w" : "-";
		perms += p.testFlag(QFile::ExeOther) ? "x" : "-";
		painter.drawText(rect, Qt::AlignRight | Qt::AlignVCenter, perms);

		y += m_itemHeight;
		i++;
	}
}


void FileView::drawDigitPermsPart(QPainter &painter)
{
	int y = 0;
	int i = m_start;
	int sectionIndex = e_header->logicalIndex(Krw::Sort_DigitPerms);

	while (i < m_fileList.size() && y < height()) {
		QRect rect;
		rect.setTop(y);
		rect.setLeft(e_header->sectionOffset(sectionIndex));
		rect.setWidth(e_header->sectionSize(sectionIndex) - 2);
		rect.setHeight(m_itemHeight);

		QIcon icon = m_iconProvider.icon(m_fileList[i]);
		icon.paint(&painter, 1, y, m_itemHeight, m_itemHeight);

		painter.drawText(rect, Qt::AlignRight | Qt::AlignVCenter, QString::number(m_fileList[i].permissions() & 0777, 8));

		y += m_itemHeight;
		i++;
	}
}


void FileView::drawOwnerPart(QPainter &painter)
{
	int y = 0;
	int i = m_start;
	int sectionIndex = e_header->logicalIndex(Krw::Sort_Owner);

	while (i < m_fileList.size() && y < height()) {
		QRect rect;
		rect.setTop(y);
		rect.setLeft(e_header->sectionOffset(sectionIndex) + 2);
		rect.setWidth(e_header->sectionSize(sectionIndex));
		rect.setHeight(m_itemHeight);

		painter.drawText(rect, Qt::AlignLeft | Qt::AlignVCenter, m_fileList[i].owner());

		y += m_itemHeight;
		i++;
	}
}


void FileView::drawGroupPart(QPainter &painter)
{
	int y = 0;
	int i = m_start;
	int sectionIndex = e_header->logicalIndex(Krw::Sort_Group);

	while (i < m_fileList.size() && y < height()) {
		QRect rect;
		rect.setTop(y);
		rect.setLeft(e_header->sectionOffset(sectionIndex) + 2);
		rect.setWidth(e_header->sectionSize(sectionIndex));
		rect.setHeight(m_itemHeight);

		painter.drawText(rect, Qt::AlignLeft | Qt::AlignVCenter, m_fileList[i].group());

		y += m_itemHeight;
		i++;
	}
}


void FileView::drawModifiedPart(QPainter &painter)
{
	int y = 0;
	int i = m_start;
	int sectionIndex = e_header->logicalIndex(Krw::Sort_Modified);

	while (i < m_fileList.size() && y < height()) {
		QRect rect;
		rect.setTop(y);
		rect.setLeft(e_header->sectionOffset(sectionIndex) + 2);
		rect.setWidth(e_header->sectionSize(sectionIndex));
		rect.setHeight(m_itemHeight);

		QIcon icon = m_iconProvider.icon(m_fileList[i]);
		icon.paint(&painter, 1, y, m_itemHeight, m_itemHeight);

		painter.drawText(rect, Qt::AlignLeft | Qt::AlignVCenter, m_fileList[i].lastModified().toString());

		y += m_itemHeight;
		i++;
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
