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

	initScroll();
	m_buffer = QPixmap(m_width, ((height() + m_itemHeight - 1) / m_itemHeight) * m_itemHeight);
	initBuffer();
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

	m_current = 0;

	initScroll();
	initBuffer();
}


void FileView::settingsUpdate()
{
	readSettings();
	initBuffer();
	update();
}


void FileView::updateAll()
{
	foreach (FileView *view, s_fileViewes) {
		view->initBuffer();
		view->update();
	}
}


void FileView::paintEvent(QPaintEvent *)
{
	QPainter painter;
	painter.begin(&m_buffer);
	painter.setFont(font());

	int start = m_scroll->value();
	int tmpFinish = m_scroll->value() + m_scroll->pageStep();
	if (!(height() % m_itemHeight))
		tmpFinish--;
	int finish = qMin(tmpFinish, m_fileList.size() - 1);
	int sub = m_scroll->value() - m_prevScrollValue;

	if (0 <= qAbs(sub) && qAbs(sub) < m_scroll->pageStep()) {
		QRect targetRect = m_buffer.rect();
		QRect sourceRect = m_buffer.rect();

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

		if (targetRect.top() != sourceRect.top()) {
			QPixmap localBuffer(m_buffer.size());
			QPainter bufferPainter(&localBuffer);
			bufferPainter.drawPixmap(0, 0, m_buffer);
			painter.drawPixmap(targetRect, localBuffer, sourceRect);
		}
	}

	paintBackground(start, finish, painter);
	paintForeground(start, finish, painter);

	painter.end();

	painter.begin(this);
	painter.drawPixmap(0, 0, m_buffer);
	if (hasFocus()) {
		if (m_scroll->value() <= m_current && m_current <= m_scroll->value() + m_scroll->pageStep())
			paintCursor(painter);
	}
	painter.end();

	m_prevScrollValue = m_scroll->value();
}


void FileView::resizeEvent(QResizeEvent *)
{
	initScroll();
	initBuffer();
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

	e->ignore();
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
		if (m_showSelectBackground && m_selectItems.at(i))
			painter.fillRect(rect, m_selectBaseColor);
		else if (i & 1)
			painter.fillRect(rect, m_baseColor[1]);
		else
			painter.fillRect(rect, m_baseColor[0]);

		rect.moveTop(rect.top() + m_itemHeight);
	}

	if (finish == m_fileList.size()-1)
		painter.fillRect(rect, m_baseColor[0]);
}


void FileView::paintForeground(int start, int finish, QPainter &painter)
{
	Q_ASSERT(start >= m_scroll->value());

	if (finish < start) return;

	painter.setPen(m_textColor);
	for (int sectionIndex = 0; sectionIndex < e_header->count(); sectionIndex++) {
		int y = (start - m_scroll->value()) * m_itemHeight;
		AbstractHeaderItem::PaintOption op;
		op.margin = Margin;
		op.rect = makeRectForSection(sectionIndex, y);

		for (int i = start; i <= finish; i++) {
			if (m_selectItems.at(i))
				painter.setPen(m_selectTextColor);
			else
				painter.setPen(m_textColor);

			e_header->headerItem(sectionIndex)->drawFileItem(m_fileList[i], op, painter);

			op.rect.moveTop(op.rect.top() + m_itemHeight);
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

		painter.setPen(m_textColor);
		for (int sectionIndex = 0; sectionIndex < e_header->count(); sectionIndex++) {
			AbstractHeaderItem::PaintOption op;
			op.margin = Margin;
			op.rect = makeRectForSection(sectionIndex, y);
			e_header->headerItem(sectionIndex)->drawFileItem(m_fileList[m_current], op, painter);
		}
	}
	else {
		int y = (m_current - m_scroll->value()) * m_itemHeight;
		QRect rect(0, y, m_width-1, m_itemHeight-1);
		painter.drawRect(rect);
	}
}


/*!
  Paints full buffer pixmap. Must be called at the moment when recreate the m_buffer variable
  */
void FileView::initBuffer()
{
	m_buffer = QPixmap(m_width, ((height() + m_itemHeight - 1) / m_itemHeight) * m_itemHeight);

	QPainter painter(&m_buffer);
	painter.setFont(font());

	int start = m_scroll->value();
	int finish = qMin(m_scroll->value() + m_scroll->pageStep(), m_fileList.size() - 1);

	painter.fillRect(m_buffer.rect(), m_baseColor[0]);
	paintBackground(start, finish, painter);
	paintForeground(start, finish, painter);
}


void FileView::initScroll()
{
	if (m_fileList.size()*m_itemHeight > height())
		m_scroll->show();
	else
		m_scroll->hide();

	m_prevScrollValue = 0;
	m_scroll->setMinimum(0);
	m_scroll->setValue(0);
	m_scroll->setSingleStep(1);
	m_scroll->setPageStep(height() / m_itemHeight);
	m_scroll->setMaximum(qMax(m_fileList.size(), m_scroll->pageStep()) - m_scroll->pageStep());
	m_scroll->setGeometry(width() - m_scroll->width(), 0, m_scroll->width(), height());

	m_width = width();
	if (!m_scroll->isHidden())
		m_width -= m_scroll->width();
}


void FileView::updateItem(int index)
{
	QPainter painter(&m_buffer);
	painter.setFont(font());

	paintBackground(index, index, painter);
	paintForeground(index, index, painter);
}


inline QRect FileView::makeRectForSection(int index, int top) const
{
	QRect rect;
	rect.setTop(top);
	rect.setLeft(e_header->headerItem(index)->offset() + Margin);
	rect.setWidth(e_header->headerItem(index)->width() - 2*Margin);
	rect.setHeight(m_itemHeight);

	return rect;
}


void FileView::readSettings()
{
	bool ok;
	QSettings *sets = kApp->settings();
	sets->beginGroup("FileView");

	QFont newFont;
	newFont.setFamily(sets->value("FontFamily", font().family()).toString());
	newFont.setPointSize(sets->value("FontSize", font().pointSize()).toInt(&ok));
	Q_ASSERT(ok);
	newFont.setWeight(sets->value("FontWeight", font().weight()).toInt(&ok));
	Q_ASSERT(ok);
	newFont.setItalic(sets->value("FontItalic", font().italic()).toBool());
	setFont(newFont);

	m_cursorIsFull = sets->value("CursorIsFull", false).toBool();
	m_showSelectBackground = sets->value("ShowSelectBackground", true).toBool();
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

	// This call needs for initializating of the Settings object
	writeSettings();
}


void FileView::writeSettings()
{
	QSettings *sets = kApp->settings();
	sets->beginGroup("FileView");

	sets->setValue("CursorColor", (uint) m_cursorColor.rgb());
	sets->setValue("TextColor", (uint) m_textColor.rgb());
	sets->setValue("BaseColor.0", (uint) m_baseColor[0].rgb());
	sets->setValue("BaseColor.1", (uint) m_baseColor[1].rgb());
	sets->setValue("SelectTextColor", (uint) m_selectTextColor.rgb());
	sets->setValue("SelectBaseColor", (uint) m_selectBaseColor.rgb());
	sets->setValue("FontFamily", font().family());
	sets->setValue("FontSize", font().pointSizeF());
	sets->setValue("FontWeight", font().weight());
	sets->setValue("FontItalic", font().italic());
	sets->setValue("CursorIsFull", m_cursorIsFull);
	sets->setValue("ItemHeight", m_itemHeight);
	sets->setValue("ShowSelectBackground", m_showSelectBackground);

	sets->endGroup();
}
