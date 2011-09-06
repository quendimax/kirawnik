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


QList<FileView *> FileView::s_fileViewes;


FileView::FileView(HeaderView *header, QWidget *parent)
	: QWidget(parent), e_header(header)
{
	s_fileViewes.append(this);
	setFocusPolicy(Qt::StrongFocus);

	m_prevScrollValue = 0;
	m_scroll = new QScrollBar(Qt::Vertical, this);
	m_scroll->setFixedWidth(16);
	m_scroll->hide();
	m_scroll->setValue(m_prevScrollValue);
	connect(m_scroll, SIGNAL(valueChanged(int)), this, SLOT(update()));

	m_current = 0;

	readSettings();

	m_width = width() - m_scroll->width();
	m_pixmap = QPixmap(m_width, ((height() + m_itemHeight - 1) / m_itemHeight) * m_itemHeight);
	initPixmap();
}


FileView::~FileView()
{
	writeSettings();
	s_fileViewes.removeAll(this);
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

	initPixmap();
}


void FileView::updateAll()
{
	foreach (FileView *view, s_fileViewes) {
		view->initPixmap();
		view->update();
	}
}


void FileView::paintEvent(QPaintEvent *)
{
	QPainter painter;
	painter.begin(&m_pixmap);
	painter.setFont(font());

	int start = m_scroll->value();
	int finish = qMin(m_scroll->value() + m_scroll->pageStep(), m_fileList.size() - 1);
	int sub = m_scroll->value() - m_prevScrollValue;

	if (0 <= qAbs(sub) && qAbs(sub) < m_scroll->pageStep()) {
		QRect targetRect = m_pixmap.rect();
		QRect sourceRect = m_pixmap.rect();

		if (sub > 0) {
			sourceRect.setTop(sourceRect.top() + sub*m_itemHeight);
			targetRect.setBottom(targetRect.bottom() - sub*m_itemHeight);
			start = finish - sub + 1;
		}
		else if (sub < 0){
			targetRect.setTop(targetRect.top() - sub*m_itemHeight);
			sourceRect.setBottom(sourceRect.bottom() + sub*m_itemHeight);
			finish = start - sub - 1;
		}
		else {
			finish = start - 1;
		}
		painter.drawPixmap(targetRect, m_pixmap, sourceRect);
	}

	paintBackground(start, finish, painter);
	paintForeground(start, finish, painter);

	painter.end();

	painter.begin(this);
	painter.drawPixmap(0, 0, m_pixmap);
	if (hasFocus()) {
		if (m_scroll->value() <= m_current && m_current <= m_scroll->value() + m_scroll->pageStep())
			paintCursor(painter);
	}
	painter.end();

	m_prevScrollValue = m_scroll->value();
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

	m_width = width();
	if (!m_scroll->isHidden())
		m_width -= m_scroll->width();

	m_pixmap = QPixmap(m_width, ((height() + m_itemHeight - 1) / m_itemHeight) * m_itemHeight);
	initPixmap();
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
			updateItem(m_current);
			break;
		case Qt::Key_Insert:
			m_selectItems.setBit(m_current, !m_selectItems.at(m_current));
			updateItem(m_current);
			m_current++;
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

	e->ignore();
}


void FileView::mousePressEvent(QMouseEvent *e)
{
	int newCurrent = e->pos().y() / m_itemHeight;
	newCurrent += m_scroll->value();
	if (newCurrent < m_fileList.size())
		m_current = newCurrent;

	if (m_current - m_scroll->value() >= m_scroll->pageStep())
		m_scroll->setValue(m_current - m_scroll->pageStep() + 1);

	update();
}


void FileView::wheelEvent(QWheelEvent *e)
{
	m_scroll->setValue(m_scroll->value() - e->delta()/40);
}


void FileView::paintBackground(int start, int finish, QPainter &painter)
{
	Q_ASSERT(start >= m_scroll->value());

	if (finish < start ) return;

	int top = (start - m_scroll->value()) * m_itemHeight;
	QRect rect(0, top, m_width, m_itemHeight);

	for (int i = start; i <= finish; i++) {
		if (m_selectItems.at(i))
			painter.fillRect(rect, m_selectBaseColor);
		else if (i & 1)
			painter.fillRect(rect, m_baseColor[1]);
		else
			painter.fillRect(rect, m_baseColor[0]);

		rect.moveTop(rect.top() + m_itemHeight);
	}
}


void FileView::paintForeground(int start, int finish, QPainter &painter)
{
	Q_ASSERT(start >= m_scroll->value());

	if (finish < start) return;

	static void (FileView::*drawPart[Krw::Sort_End])(int, const QRect &, QPainter &) = {
		&FileView::drawName,
		&FileView::drawSuffix,
		&FileView::drawSize,
		&FileView::drawTextPerms,
		&FileView::drawDigitPerms,
		&FileView::drawOwner,
		&FileView::drawGroup,
		&FileView::drawModified
	};

	painter.setPen(m_textColor);
	for (int sectionIndex = 0; sectionIndex < e_header->count(); sectionIndex++) {
		int y = (start - m_scroll->value()) * m_itemHeight;
		QRect rect = makeRectForSection(sectionIndex, y);

		for (int i = start; i <= finish; i++) {
			void (FileView::*drawMethod)(int, const QRect &, QPainter &) = drawPart[e_header->sectionType(sectionIndex)];
			(this->*drawMethod)(i, rect, painter);

			rect.moveTop(rect.top() + m_itemHeight);
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

	rect.setLeft(rect.left() + 2*Margin + m_itemHeight);
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

	painter.drawText(rect, Qt::AlignLeft | Qt::AlignVCenter, name);
}


void FileView::drawSuffix(int index, const QRect &rect, QPainter &painter)
{
	if (!m_fileList[index].isDir()) {
		if (m_selectItems.at(index))
			painter.setPen(m_selectTextColor);
		else
			painter.setPen(m_textColor);
		painter.drawText(rect, Qt::AlignLeft | Qt::AlignVCenter, m_fileList[index].suffix());
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
	QRect r = rect;
	r.setLeft(Margin);
	painter.drawText(r, Qt::AlignRight | Qt::AlignVCenter, strSize);
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


void FileView::drawOwner(int index, const QRect &rect, QPainter &painter)
{
	if (m_selectItems.at(index))
		painter.setPen(m_selectTextColor);
	else
		painter.setPen(m_textColor);
	painter.drawText(rect, Qt::AlignLeft | Qt::AlignVCenter, m_fileList[index].owner());
}


void FileView::drawGroup(int index, const QRect &rect, QPainter &painter)
{
	if (m_selectItems.at(index))
		painter.setPen(m_selectTextColor);
	else
		painter.setPen(m_textColor);
	painter.drawText(rect, Qt::AlignLeft | Qt::AlignVCenter, m_fileList[index].group());
}


void FileView::drawModified(int index, const QRect &rect, QPainter &painter)
{
	if (m_selectItems.at(index))
		painter.setPen(m_selectTextColor);
	else
		painter.setPen(m_textColor);
	QDateTime dt = m_fileList[index].lastModified();
	painter.drawText(rect, Qt::AlignLeft | Qt::AlignVCenter, dt.toString("yyyy.MM.dd hh:mm:ss"));
}


/*!
  Paints full pixmap. Must be called at the moment when recreate the m_pixmap variable
  */
void FileView::initPixmap()
{
	QPainter painter(&m_pixmap);
	painter.setFont(font());

	int start = m_scroll->value();
	int finish = qMin(m_scroll->value() + m_scroll->pageStep(), m_fileList.size() - 1);

	painter.fillRect(m_pixmap.rect(), m_baseColor[0]);
	paintBackground(start, finish, painter);
	paintForeground(start, finish, painter);
}


void FileView::updateItem(int index)
{
	QPainter painter(&m_pixmap);
	painter.setFont(font());

	paintBackground(index, index, painter);
	paintForeground(index, index, painter);
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

	QFont nfont;
	nfont.setFamily(sets->value("Font", font().family()).toString());
	nfont.setPointSize(sets->value("FontSize", font().pointSize()).toInt(&ok));
	Q_ASSERT(ok);
	setFont(nfont);
	m_cursorIsFull = sets->value("CursorIsFull", false).toBool();
	m_cursorColor.setRgb((QRgb) sets->value("CursorColor", (uint) QColor(Qt::green).rgb()).toUInt(&ok));
	Q_ASSERT(ok);
	m_textColor.setRgb((QRgb) sets->value("TextColor", (uint) palette().color(QPalette::Text).rgb()).toUInt(&ok));
	Q_ASSERT(ok);
	m_baseColor[0].setRgb((QRgb) sets->value("BaseColor.0", (uint)palette().color(QPalette::Base).rgb()).toUInt(&ok));
	Q_ASSERT(ok);
	m_baseColor[1].setRgb((QRgb) sets->value("BaseColor.1", (uint)palette().color(QPalette::AlternateBase).rgb()).toUInt(&ok));
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
	sets->setValue("FontSize", font().pointSize());
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
