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

	m_current = 0;

	initScroll();
	initPixmap();
}


void FileView::settingsUpdate()
{
	readSettings();
	initPixmap();
	update();
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
	int tmpFinish = m_scroll->value() + m_scroll->pageStep();
	if (!(height() % m_itemHeight))
		tmpFinish--;
	int finish = qMin(tmpFinish, m_fileList.size() - 1);
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


void FileView::resizeEvent(QResizeEvent *)
{
	initScroll();
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
  Paints full pixmap. Must be called at the moment when recreate the m_pixmap variable
  */
void FileView::initPixmap()
{
	m_pixmap = QPixmap(m_width, ((height() + m_itemHeight - 1) / m_itemHeight) * m_itemHeight);

	QPainter painter(&m_pixmap);
	painter.setFont(font());

	int start = m_scroll->value();
	int finish = qMin(m_scroll->value() + m_scroll->pageStep(), m_fileList.size() - 1);

	painter.fillRect(m_pixmap.rect(), m_baseColor[0]);
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
	QPainter painter(&m_pixmap);
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

	QFont nfont;
	nfont.setFamily(sets->value("Font", font().family()).toString());
	nfont.setPointSize(sets->value("FontSize", font().pointSize()).toInt(&ok));
	Q_ASSERT(ok);
	setFont(nfont);
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
}


void FileView::writeSettings()
{
	QSettings *sets = kApp->settings();
	sets->beginGroup("FileView");

	sets->setValue("Font", font().family());
	sets->setValue("FontSize", font().pointSize());
	sets->setValue("CursorIsFull", m_cursorIsFull);
	sets->setValue("ItemHeight", m_itemHeight);

	sets->endGroup();
}
