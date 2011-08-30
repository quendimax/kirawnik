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
	setFocusPolicy(Qt::StrongFocus);
	m_scroll = new QScrollBar(Qt::Vertical, this);
	m_scroll->setFixedWidth(16);
	m_scroll->hide();

	m_start = 0;
	m_current = 0;

	readSettings();

	QDir dir("/");
	setFileInfoList(dir.entryInfoList());
}


FileView::~FileView()
{
	writeSettings();
}


void FileView::setFileInfoList(const QFileInfoList &list)
{
	m_fileList = list;
	m_selectItems.resize(list.size());
	m_selectItems.fill(false);
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

	painter.setPen(m_textColor);
	for (int i = 0; i < e_header->count(); i++) {
		if (e_header->sectionIsShowing(i)) {
			void (FileView::*drawMethod)(QPainter &) = drawPart[e_header->sectionType(i)];
			(this->*drawMethod)(painter);
		}
	}

	if (hasFocus())
		paintCursor(painter);
}


void FileView::resizeEvent(QResizeEvent *e)
{
	m_scroll->setGeometry(width() - m_scroll->width(), 0, m_scroll->width(), e->size().height());
}


void FileView::keyPressEvent(QKeyEvent *e)
{
	if (e->modifiers() == Qt::ShiftModifier) {
		switch (e->key()) {
		case Qt::Key_Down:
			m_selectItems.setBit(m_current, !m_selectItems.at(m_current));
			m_current++;
			break;
		case Qt::Key_Up:
			m_selectItems.setBit(m_current, !m_selectItems.at(m_current));
			m_current--;
			break;
		}
	}
	else {
		switch (e->key()) {
		case Qt::Key_Down:
			m_current++;
			break;
		case Qt::Key_Up:
			m_current--;
			break;
		case Qt::Key_Home:
			m_current = 0;
			break;
		case Qt::Key_End:
			m_current = m_fileList.size() - 1;
			break;
		case Qt::Key_PageDown:
			m_current += height() / m_itemHeight;
			break;
		case Qt::Key_PageUp:
			m_current -= height() / m_itemHeight;
			break;
		case Qt::Key_Space:
			m_selectItems.setBit(m_current, !m_selectItems.at(m_current));
			break;
		}
	}

	if (m_current < 0)
		m_current = 0;
	if (m_fileList.size() <= m_current)
		m_current = m_fileList.size() - 1;

	update();
}


inline void FileView::paintBackground(QPainter &painter)
{
	painter.fillRect(rect(), m_baseColor[0]);

	if (m_baseColor[0] != m_baseColor[1]) {
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

	if (!(m_baseColor[0] == m_baseColor[1] && m_baseColor[0] == m_selectBaseColor)) {
		for (int i = m_start, y = 0; i < m_fileList.size() && y < height(); i++, y += m_itemHeight) {
			if (m_selectItems.at(i)) {
				QRect rect(0, y, width(), m_itemHeight);
				painter.fillRect(rect, m_selectBaseColor);
			}
		}
	}

	if (hasFocus() && m_cursorIsFull) {
		int y = (m_current - m_start) * m_itemHeight;
		QRect rect(0, y, width()-1, m_itemHeight);
		painter.fillRect(rect, m_cursorColor);
	}
}


inline void FileView::paintCursor(QPainter &painter)
{
	painter.setPen(m_cursorColor);

	if (m_current < m_start || height() / m_itemHeight + 1 < m_start)
		return;

	int y = (m_current - m_start) * m_itemHeight;
	QRect rect(0, y, width()-1, m_itemHeight);
	painter.drawRect(rect);
}


void FileView::drawNamePart(QPainter &painter)
{
	int y = 0;
	int i = m_start;
	int sectionIndex = e_header->logicalIndex(Krw::Sort_Name);

	while (i < m_fileList.size() && y < height())
	{
		QRect rect = makeRectForSection(sectionIndex, y);
		rect.setLeft(rect.left() + m_itemHeight + 2*Margin);

		QFontMetrics metrics(font());
		QString name;
		QString addName;
		if (m_fileList[i].isDir()) {
			name = m_fileList[i].fileName();
			addName += "[]";	// len == 2
		}
		else {
			name = m_fileList[i].baseName();
			addName = "";
		}

		if (metrics.width(name) + metrics.width(addName) > rect.width())
			addName += "..";	// len == 4

		while (metrics.width(name) + metrics.width(addName) > rect.width() && name.length() > 0)
			name.remove(name.length() - 1, 1);

		if (m_fileList[i].isDir()) {
			if (addName.length() == 2)
				name = "[" + name + "]";
			else
				name = "[" + name + "..]";
		}
		else if (addName.length() == 2)
			name += "..";

		if (m_selectItems.at(i))
			painter.setPen(m_selectTextColor);
		else
			painter.setPen(m_textColor);
		painter.drawText(rect, Qt::AlignLeft | Qt::AlignVCenter, name);

		QIcon icon = m_iconProvider.icon(m_fileList[i]);
		icon.paint(&painter, e_header->sectionOffset(sectionIndex) + Margin, y, m_itemHeight, m_itemHeight);

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
		QRect rect = makeRectForSection(sectionIndex, y);

		if (m_selectItems.at(i))
			painter.setPen(m_selectTextColor);
		else
			painter.setPen(m_textColor);
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
		QRect rect = makeRectForSection(sectionIndex, y);
		QString strSize("");
		int size = m_fileList[i].size();

		do {
			if (size >= 1000)
				strSize = QString(" %1").arg(size % 1000, 3, 10, QLatin1Char('0')) + strSize;
			else
				strSize = QString("%1").arg(size % 1000) + strSize;
			size /= 1000;
		}
		while (size);
		rect.setLeft(0 + Margin);

		if (m_selectItems.at(i))
			painter.setPen(m_selectTextColor);
		else
			painter.setPen(m_textColor);
		painter.drawText(rect, Qt::AlignRight | Qt::AlignVCenter, strSize);

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
		QRect rect = makeRectForSection(sectionIndex, y);

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

		if (m_selectItems.at(i))
			painter.setPen(m_selectTextColor);
		else
			painter.setPen(m_textColor);
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
		QRect rect = makeRectForSection(sectionIndex, y);
		uint perms = m_fileList[i].permissions();

		perms = (perms & 0x00f)  |  ((perms & 0x0f0) >> 1)  |  ((perms & 0xf00) >> 2);

		if (m_selectItems.at(i))
			painter.setPen(m_selectTextColor);
		else
			painter.setPen(m_textColor);
		painter.drawText(rect, Qt::AlignRight | Qt::AlignVCenter, QString("0%1").arg(perms, 3, 8, QLatin1Char('0')));

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
		QRect rect = makeRectForSection(sectionIndex, y);

		if (m_selectItems.at(i))
			painter.setPen(m_selectTextColor);
		else
			painter.setPen(m_textColor);
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
		QRect rect = makeRectForSection(sectionIndex, y);

		if (m_selectItems.at(i))
			painter.setPen(m_selectTextColor);
		else
			painter.setPen(m_textColor);
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
		QRect rect = makeRectForSection(sectionIndex, y);

		if (m_selectItems.at(i))
			painter.setPen(m_selectTextColor);
		else
			painter.setPen(m_textColor);
		painter.drawText(rect, Qt::AlignLeft | Qt::AlignVCenter, m_fileList[i].lastModified().toString());

		y += m_itemHeight;
		i++;
	}
}


inline QRect FileView::makeRectForSection(int index, int top) const
{
	QRect rect;
	rect.setTop(top);
	rect.setLeft(e_header->sectionOffset(index) + Margin);
	rect.setWidth(e_header->sectionSize(index) - 2*Margin);
	rect.setHeight(m_itemHeight);

	return rect;
}


void FileView::readSettings()
{
	bool ok;
	QSettings *sets = kApp->settings();
	sets->beginGroup("FileView");

	setFont(sets->value("Font", font().family()).toString());
	m_cursorIsFull = sets->value("CursorIsFull", false).toBool();
	m_cursorColor.setRgb((QRgb) sets->value("CursorColor", (uint) QColor(Qt::green).rgb()).toUInt(&ok));
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

	sets->setValue("Font", font().family());
	sets->setValue("CursorIsFull", m_cursorIsFull);
	sets->setValue("CursorColor", (uint) m_cursorColor.rgb());
	sets->setValue("TextColor", (uint) m_textColor.rgb());
	sets->setValue("BaseColor.0", (uint) m_baseColor[0].rgb());
	sets->setValue("BaseColor.1", (uint) m_baseColor[1].rgb());
	sets->setValue("SelectTextColor", (uint) m_selectTextColor.rgb());
	sets->setValue("SelectBaseColor", (uint) m_selectBaseColor.rgb());
	sets->setValue("ItemHeight", m_itemHeight);

	sets->endGroup();
}
