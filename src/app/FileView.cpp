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
	m_scroll->setValue(0);
	connect(m_scroll, SIGNAL(valueChanged(int)), this, SLOT(update()));

	m_current = 0;

	readSettings();

	m_width = width() - m_scroll->width();
	m_pixmap = QPixmap(m_width, height());

	QDir dir("/usr/bin/");
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

	if (list.size() * m_itemHeight > height())
		m_scroll->show();
	else
		m_scroll->hide();
	m_scroll->setSingleStep(1);
	m_scroll->setPageStep(height() / m_itemHeight);
	m_scroll->setMinimum(0);
	m_scroll->setMaximum(qMax(m_fileList.size(), m_scroll->pageStep()) - m_scroll->pageStep());
	m_scroll->setValue(0);
}


void FileView::paintEvent(QPaintEvent *)
{
	QPainter painter;
	painter.begin(&m_pixmap);

	paintBackground(painter);

	static void (FileView::*drawPart[Krw::Sort_End])(QPainter &) = {
		&FileView::drawNameColumn,
		&FileView::drawSuffixColumn,
		&FileView::drawSizeColumn,
		&FileView::drawTextPermsColumn,
		&FileView::drawDigitPermsColumn,
		&FileView::drawOwnerColumn,
		&FileView::drawGroupColumn,
		&FileView::drawModifiedColumn
	};
	painter.setPen(m_textColor);
	for (int i = 0; i < e_header->count(); i++) {
		void (FileView::*drawMethod)(QPainter &) = drawPart[e_header->sectionType(i)];
		(this->*drawMethod)(painter);
	}

	painter.end();

	painter.begin(this);
	painter.drawPixmap(0, 0, m_pixmap);
	if (hasFocus()) {
		if (m_scroll->value() <= m_current && m_current < m_scroll->value() + m_scroll->pageStep() + 1)
			paintCursor(painter);
	}
	painter.end();
}


void FileView::resizeEvent(QResizeEvent *e)
{
	if (m_fileList.size()*m_itemHeight > height())
		m_scroll->show();
	else
		m_scroll->hide();

	m_scroll->setPageStep(height() / m_itemHeight);
	m_scroll->setMaximum(qMax(m_fileList.size(), m_scroll->pageStep()) - m_scroll->pageStep());
	m_scroll->setGeometry(width() - m_scroll->width(), 0, m_scroll->width(), e->size().height());
	m_scroll->setValue(0);

	m_width = width();
	if (!m_scroll->isHidden())
		m_width -= m_scroll->width();

	m_pixmap = QPixmap(m_width, height());
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
			m_current += m_scroll->pageStep();
			break;
		case Qt::Key_PageUp:
			m_current -= m_scroll->pageStep();
			break;
		case Qt::Key_Space:
			m_selectItems.setBit(m_current, !m_selectItems.at(m_current));
			break;
		}
	}

	if (m_current < 0)
		m_current = 0;
	else if (m_current >= m_fileList.size())
		m_current = m_fileList.size() - 1;

	update();

	if (m_current < m_scroll->value())
		m_scroll->setValue(m_current);
	else if (m_current - m_scroll->value() >= m_scroll->pageStep())
		m_scroll->setValue(m_current - m_scroll->pageStep() + 1);
}


void FileView::mousePressEvent(QMouseEvent *e)
{
	m_current = e->pos().y() / m_itemHeight;
	m_current += m_scroll->value();
	update();
}


void FileView::wheelEvent(QWheelEvent *e)
{
	m_scroll->setValue(m_scroll->value() - e->delta()/40);
}


void FileView::paintBackground(QPainter &painter)
{
	QRect backgroundRect = rect();
	backgroundRect.setWidth(m_width);
	painter.fillRect(backgroundRect, m_baseColor[0]);

	if (m_baseColor[0] != m_baseColor[1]) {
		// if m_scroll->value() odd then start from alternate color
		int y = (m_scroll->value() & 1) ? 0 : m_itemHeight;
		int i = (m_scroll->value() & 1) ? m_scroll->value() : m_scroll->value() + 1;

		while (i < m_fileList.size() && y < height()) {
			QRect rect(0, y, m_width, m_itemHeight);
			painter.fillRect(rect, m_baseColor[1]);

			y += 2*m_itemHeight;
			i += 2;
		}
	}

	if (!(m_baseColor[0] == m_baseColor[1] && m_baseColor[0] == m_selectBaseColor)) {
		for (int i = m_scroll->value(), y = 0; i < m_fileList.size() && y < height(); i++, y += m_itemHeight) {
			if (m_selectItems.at(i)) {
				QRect rect(0, y, m_width, m_itemHeight);
				painter.fillRect(rect, m_selectBaseColor);
			}
		}
	}
}


void FileView::paintCursor(QPainter &painter)
{
	painter.setPen(m_cursorColor);

	if (m_cursorIsFull) {
		int y = (m_current - m_scroll->value()) * m_itemHeight;
		QRect rect(0, y, m_width-1, m_itemHeight);
		painter.fillRect(rect, m_cursorColor);

		for (int sectionIndex = 0; sectionIndex < e_header->count(); sectionIndex++) {
			switch (e_header->sectionType(sectionIndex)) {
			case Krw::Sort_Name:
				drawName(m_current, makeRectForSection(sectionIndex, y), painter);
				break;
			case Krw::Sort_Suffix:
				drawSuffix(m_current, makeRectForSection(sectionIndex, y), painter);
				break;
			case Krw::Sort_Size:
				drawSize(m_current, makeRectForSection(sectionIndex, y), painter);
				break;
			case Krw::Sort_TextPerms:
				drawTextPerms(m_current, makeRectForSection(sectionIndex, y), painter);
				break;
			case Krw::Sort_DigitPerms:
				drawDigitPerms(m_current, makeRectForSection(sectionIndex, y), painter);
				break;
			case Krw::Sort_Owner:
				drawOwner(m_current, makeRectForSection(sectionIndex, y), painter);
				break;
			case Krw::Sort_Group:
				drawGroup(m_current, makeRectForSection(sectionIndex, y), painter);
				break;
			case Krw::Sort_Modified:
				drawModified(m_current, makeRectForSection(sectionIndex, y), painter);
				break;
			default:
				break;
			}
		}
	}
	else {
		int y = (m_current - m_scroll->value()) * m_itemHeight;
		QRect rect(0, y, m_width-1, m_itemHeight-1);
		painter.drawRect(rect);
	}
}


void FileView::drawName(int index, const QRect &rectangle, QPainter &painter)
{
	QRect rect = rectangle;
	QIcon icon = m_iconProvider.icon(m_fileList[index]);
	icon.paint(&painter, rect.left(), rect.top(), m_itemHeight, m_itemHeight);

	QFontMetrics metrics(font());
	QString name;
	QString addName;
	if (m_fileList[index].isDir()) {
		name = m_fileList[index].fileName();
		addName += "[]";	// len == 2
	}
	else {
		name = m_fileList[index].baseName();
		addName = "";
	}

	if (metrics.width(name) + metrics.width(addName) > rect.width())
		addName += "..";	// len == 4

	while (metrics.width(name) + metrics.width(addName) > rect.width() && name.length() > 0)
		name.remove(name.length() - 1, 1);

	if (m_fileList[index].isDir()) {
		if (addName.length() == 2)
			name = "[" + name + "]";
		else
			name = "[" + name + "..]";
	}
	else if (addName.length() == 2)
		name += "..";

	if (m_selectItems.at(index))
		painter.setPen(m_selectTextColor);
	else
		painter.setPen(m_textColor);

	rect.setLeft(rect.left() + 2*Margin + m_itemHeight);
	painter.drawText(rect, Qt::AlignLeft | Qt::AlignVCenter, name);
}


void FileView::drawNameColumn(QPainter &painter)
{
	int y = 0;
	int i = m_scroll->value();
	int sectionIndex = e_header->logicalIndex(Krw::Sort_Name);

	while (i < m_fileList.size() && y < height())
	{
		QRect rect = makeRectForSection(sectionIndex, y);

		drawName(i, rect, painter);

		y += m_itemHeight;
		i++;
	}
}


void FileView::drawSuffix(int index, const QRect &rect, QPainter &painter)
{
	if (m_selectItems.at(index))
		painter.setPen(m_selectTextColor);
	else
		painter.setPen(m_textColor);
	painter.drawText(rect, Qt::AlignLeft | Qt::AlignVCenter, m_fileList[index].suffix());
}


void FileView::drawSuffixColumn(QPainter &painter)
{
	int y = 0;
	int i = m_scroll->value();
	int sectionIndex = e_header->logicalIndex(Krw::Sort_Suffix);

	while (i < m_fileList.size() && y < height()) {
		QRect rect = makeRectForSection(sectionIndex, y);

		drawSuffix(i, rect, painter);

		y += m_itemHeight;
		i++;
	}
}


void FileView::drawSize(int index, const QRect &rect, QPainter &painter)
{
	QString strSize("");
	int size = m_fileList[index].size();

	do {
		if (size >= 1000)
			strSize = QString(" %1").arg(size % 1000, 3, 10, QLatin1Char('0')) + strSize;
		else
			strSize = QString("%1").arg(size % 1000) + strSize;
		size /= 1000;
	}
	while (size);

	if (m_selectItems.at(index))
		painter.setPen(m_selectTextColor);
	else
		painter.setPen(m_textColor);
	painter.drawText(rect, Qt::AlignRight | Qt::AlignVCenter, strSize);
}


void FileView::drawSizeColumn(QPainter &painter)
{
	int y = 0;
	int i = m_scroll->value();
	int sectionIndex = e_header->logicalIndex(Krw::Sort_Size);

	while (i < m_fileList.size() && y < height()) {
		QRect rect = makeRectForSection(sectionIndex, y);
		rect.setLeft(0 + Margin);

		drawSize(i, rect, painter);

		y += m_itemHeight;
		i++;
	}
}


void FileView::drawTextPerms(int index, const QRect &rect, QPainter &painter)
{
	QFile::Permissions p = m_fileList[index].permissions();
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

	if (m_selectItems.at(index))
		painter.setPen(m_selectTextColor);
	else
		painter.setPen(m_textColor);
	painter.drawText(rect, Qt::AlignRight | Qt::AlignVCenter, perms);
}


void FileView::drawTextPermsColumn(QPainter &painter)
{
	int y = 0;
	int i = m_scroll->value();
	int sectionIndex = e_header->logicalIndex(Krw::Sort_TextPerms);

	while (i < m_fileList.size() && y < height()) {
		QRect rect = makeRectForSection(sectionIndex, y);

		drawTextPerms(i, rect, painter);

		y += m_itemHeight;
		i++;
	}
}


void FileView::drawDigitPerms(int index, const QRect &rect, QPainter &painter)
{
	uint perms = m_fileList[index].permissions();

	perms = (perms & 0x00f)  |  ((perms & 0x0f0) >> 1)  |  ((perms & 0xf00) >> 2);

	if (m_selectItems.at(index))
		painter.setPen(m_selectTextColor);
	else
		painter.setPen(m_textColor);
	painter.drawText(rect, Qt::AlignRight | Qt::AlignVCenter, QString("0%1").arg(perms, 3, 8, QLatin1Char('0')));
}


void FileView::drawDigitPermsColumn(QPainter &painter)
{
	int y = 0;
	int i = m_scroll->value();
	int sectionIndex = e_header->logicalIndex(Krw::Sort_DigitPerms);

	while (i < m_fileList.size() && y < height()) {
		QRect rect = makeRectForSection(sectionIndex, y);

		drawDigitPerms(i, rect, painter);

		y += m_itemHeight;
		i++;
	}
}


void FileView::drawOwner(int index, const QRect &rect, QPainter &painter)
{
	if (m_selectItems.at(index))
		painter.setPen(m_selectTextColor);
	else
		painter.setPen(m_textColor);
	painter.drawText(rect, Qt::AlignLeft | Qt::AlignVCenter, m_fileList[index].owner());
}


void FileView::drawOwnerColumn(QPainter &painter)
{
	int y = 0;
	int i = m_scroll->value();
	int sectionIndex = e_header->logicalIndex(Krw::Sort_Owner);

	while (i < m_fileList.size() && y < height()) {
		QRect rect = makeRectForSection(sectionIndex, y);

		drawOwner(i, rect, painter);

		y += m_itemHeight;
		i++;
	}
}


void FileView::drawGroup(int index, const QRect &rect, QPainter &painter)
{
	if (m_selectItems.at(index))
		painter.setPen(m_selectTextColor);
	else
		painter.setPen(m_textColor);
	painter.drawText(rect, Qt::AlignLeft | Qt::AlignVCenter, m_fileList[index].group());
}


void FileView::drawGroupColumn(QPainter &painter)
{
	int y = 0;
	int i = m_scroll->value();
	int sectionIndex = e_header->logicalIndex(Krw::Sort_Group);

	while (i < m_fileList.size() && y < height()) {
		QRect rect = makeRectForSection(sectionIndex, y);

		drawGroup(i, rect, painter);

		y += m_itemHeight;
		i++;
	}
}


void FileView::drawModified(int index, const QRect &rect, QPainter &painter)
{
	if (m_selectItems.at(index))
		painter.setPen(m_selectTextColor);
	else
		painter.setPen(m_textColor);
	painter.drawText(rect, Qt::AlignLeft | Qt::AlignVCenter, m_fileList[index].lastModified().toString());
}


void FileView::drawModifiedColumn(QPainter &painter)
{
	int y = 0;
	int i = m_scroll->value();
	int sectionIndex = e_header->logicalIndex(Krw::Sort_Modified);

	while (i < m_fileList.size() && y < height()) {
		QRect rect = makeRectForSection(sectionIndex, y);

		drawModified(i, rect, painter);

		y += m_itemHeight;
		i++;
	}
}


QRect FileView::makeRectForSection(int index, int top) const
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
