#include <QCursor>
#include <QPainter>
#include <QSettings>
#include <QMouseEvent>
#include <QMenu>
#include <QAction>

#include "Application.h"
#include "HeaderView.h"


int HeaderView::s_showItemCount;
int HeaderView::s_itemCount;
QList<HeaderView *> HeaderView::s_headerViews;

HeaderView::ItemFlag HeaderView::s_showItems;
HeaderView::HeaderItem HeaderView::s_items[] = {
	{ 0, 0, Item_Name, tr("Name") },
	{ 0, 0, Item_Suffix, tr("Ext") },
	{ 0, 0, Item_Size, tr("Suffix") },
	{ 0, 0, Item_TextPerms, tr("Perms (rwx)") },
	{ 0, 0, Item_DigitPerms, tr("Perms") },
	{ 0, 0, Item_Owner, tr("Owner") },
	{ 0, 0, Item_Group, tr("Group") },
	{ 0, 0, Item_Modified, tr("Modified") }
};



HeaderView::HeaderView(QWidget *parent)
	: QWidget(parent)
{
	static int s_objectCount = 0;
	m_objectNumber = s_objectCount;
	s_objectCount++;

	s_headerViews.append(this);

	m_headerState = HS_Free;
	m_pressedItem = -1;		// nothings is pressed
	m_resizeItem = -1;

	QFontMetrics metrics(font());
	setFixedHeight(metrics.height() + 6);

	setMouseTracking(true);

	readSettings();
}


HeaderView::~HeaderView()
{
	writeSettings();
	s_headerViews.removeAll(this);
}


void HeaderView::contextMenuEvent(QContextMenuEvent *e)
{
	static QMenu *menu = 0;

	if (menu == 0) {
		menu = new QMenu(this);

		for (int i = 0; i < s_itemCount; i++) {
			QAction *action = menu->addAction(s_items[i].name);
			action->setCheckable(true);
			if (s_items[i].type & s_showItems)
				action->setChecked(true);
		}
	}

	if (menu->exec(e->globalPos()))
		updateAll();
}


void HeaderView::mousePressEvent(QMouseEvent *e)
{
	if (e->button() != Qt::LeftButton) {
		QWidget::mousePressEvent(e);
		return;
	}

	m_pos = e->pos();

	bool isResize;
	int index = indexAt(e->pos(), &isResize);

	if (index < 0) {
		m_headerState = HS_Pressing;
		return;
	}
	if (isResize) {
		m_headerState = HS_Resizing;
		m_resizeItem = index;
	}
	else {
		m_headerState = HS_Moving;
		m_pressedItem = index;
	}

	update();
}


void HeaderView::mouseReleaseEvent(QMouseEvent *e)
{
	if (e->button() != Qt::LeftButton) {
		QWidget::mouseReleaseEvent(e);
		return;
	}

	if (m_pressedItem > -1) {
		int index = indexAt(e->pos());

		if (index != m_pressedItem) {
			m_pressedItem = -1;
		}
		else {
			ItemFlag newSortingItem = s_items[index].type;
			if (newSortingItem == m_sortingItem) {
				m_reverseSorting = !m_reverseSorting;
			}
			else {
				m_sortingItem = newSortingItem;
				m_reverseSorting = false;
			}
		}
		m_pressedItem = -1;
	}
	m_resizeItem = -1;
	m_headerState = HS_Free;

	update();
}


void HeaderView::mouseMoveEvent(QMouseEvent *e)
{
	switch (m_headerState) {
	case HS_Free: {
		static const Qt::CursorShape defaultCursorShape = cursor().shape();
		bool isResize;
		indexAt(e->pos(), &isResize);
		if (isResize)
			setCursor(QCursor(Qt::SplitHCursor));
		else if (cursor().shape() != defaultCursorShape)
			setCursor(QCursor(defaultCursorShape));
		}
		break;

	case HS_Resizing: {
		int delta = e->pos().x() - m_pos.x();
		if (s_items[m_resizeItem].width < 32 && delta < 0)
			break;

		s_items[m_resizeItem].width += delta;
		for (int i = m_resizeItem + 1; i < s_itemCount; i++)
			s_items[i].offset += delta;
		m_pos = e->pos();
		updateAll();
		}
		break;

	case HS_Pressing: {
		}
		break;

	case HS_Moving: {
		}
		break;
	}
}


void HeaderView::paintEvent(QPaintEvent *)
{
	QStyleOption option;
	initStyleOption(&option);
	option.rect = rect();

	QPainter painter(this);
	style()->drawControl(QStyle::CE_HeaderEmptyArea, &option, &painter, this);

	for (int i = 0; i < s_itemCount; i++) {
		if (s_items[i].offset < 0)
			continue;
		if (s_items[i].type & s_showItems)
			paintSection(i, painter);
	}
}


void HeaderView::initStyleOption(QStyleOption *option)
{
	option->initFrom(this);
	option->state =  QStyle::State_None | QStyle::State_Raised | QStyle::State_Horizontal;

	if (isActiveWindow()) option->state |= QStyle::State_Active;
	if (isEnabled()) option->state |= QStyle::State_Enabled;
}


void HeaderView::paintSection(int index, QPainter &painter)
{
	QStyleOptionHeader option;
	initStyleOption(&option);

	option.text = s_items[index].name;
	option.section = index - (s_itemCount - s_showItemCount);
	option.selectedPosition = QStyleOptionHeader::NotAdjacent;

	if (s_showItemCount == 1) {
		option.position = QStyleOptionHeader::OnlyOneSection;
	}
	else {
		if (index == s_itemCount - s_showItemCount) {
			option.position = QStyleOptionHeader::Beginning;
			if (s_items[index + 1].type & m_sortingItem)
				option.selectedPosition = QStyleOptionHeader::NextIsSelected;
		}
		else if (index == s_itemCount - 1) {
			option.position = QStyleOptionHeader::End;
			if (s_items[index - 1].type & m_sortingItem)
				option.selectedPosition = QStyleOptionHeader::PreviousIsSelected;
		}
		else {
			option.position = QStyleOptionHeader::Middle;
		}
	}

	if (s_items[index].type & m_sortingItem) {
		if (m_reverseSorting)
			option.sortIndicator = QStyleOptionHeader::SortUp;
		else
			option.sortIndicator = QStyleOptionHeader::SortDown;
	}

	if (index == m_pressedItem)
		option.state |= QStyle::State_Sunken;

	option.rect = this->rect();
	option.rect.setLeft(s_items[index].offset);
	option.rect.setWidth(s_items[index].width);

	style()->drawControl(QStyle::CE_Header, &option, &painter, this);
}


void HeaderView::updateAll()
{
	foreach (HeaderView *view, s_headerViews)
		view->update();
}


int HeaderView::indexAt(const QPoint &pos, bool *isResize)
{
	int index = 0;
	while (s_items[index].offset < 0)
		index++;

	if (0 <= pos.y() && pos.y() < height()) {
		for (int i = index; i < s_itemCount; i++) {
			if (s_items[i].offset <= pos.x() && pos.x() < s_items[i].offset + s_items[i].width) {
				if (isResize) {
					*isResize = false;
					int rightBorder = s_items[i].offset + s_items[i].width;
					if (rightBorder - 3 <= pos.x() && pos.x() < rightBorder)
						*isResize = true;
				}
				return i;
			}
		}
	}

	return -1;
}


void HeaderView::readSettings()
{
	QSettings *sets = kApp->settings();
	sets->beginGroup("HeaderView");

	m_sortingItem = (ItemFlag) sets->value("SelectItem." + QString::number(m_objectNumber), Item_Name).toUInt();
	m_reverseSorting = sets->value("ReverseSorting." + QString::number(m_objectNumber), false).toBool();

	static bool wasCalled = false;
	if (!wasCalled) {
		wasCalled = true;

		s_showItems = (ItemFlag) sets->value("ShowItems", Item_All).toUInt();
		s_showItemCount = s_itemCount = sizeof s_items / sizeof s_items[0];

		int offset = 0, width = 60;
		for (int i = 0; i < s_itemCount; i++) {
			s_items[i].offset = sets->value(QString::number(s_items[i].type, 16) + ".Offset", offset).toInt();
			s_items[i].width = sets->value(QString::number(s_items[i].type, 16) + ".Width", width).toInt();
			offset += width;
		}
	}

	sets->endGroup();
}


void HeaderView::writeSettings()
{
	QSettings *sets = kApp->settings();
	sets->beginGroup("HeaderView");

	sets->setValue("ShowItems", (uint) s_showItems);
	sets->setValue("SelectItem." + QString::number(m_objectNumber), (uint) m_sortingItem);
	sets->setValue("ReverseSorting." + QString::number(m_objectNumber), m_reverseSorting);

	for (int i = 0; i < s_itemCount; i++) {
		sets->setValue(QString::number(s_items[i].type, 16) + ".Offset", s_items[i].offset);
		sets->setValue(QString::number(s_items[i].type, 16) + ".Width", s_items[i].width);
	}

	sets->endGroup();
}
