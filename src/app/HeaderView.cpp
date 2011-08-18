#include <QCursor>
#include <QPainter>
#include <QSettings>
#include <QMouseEvent>

#include "Application.h"
#include "HeaderView.h"


int HeaderView::s_showItemCount;
int HeaderView::s_itemCount;
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
	QFontMetrics metrics(font());
	setFixedHeight(metrics.height()+6);

	setMouseTracking(true);

	readSettings();
}


HeaderView::~HeaderView()
{
	writeSettings();
}


void HeaderView::mousePressEvent(QMouseEvent *e)
{
	if (e->button() != Qt::LeftButton) {
		QWidget::mousePressEvent(e);
		return;
	}

	int index = indexAt(e->pos());
	if (index < 0)
		return;

	m_pressedItem = s_items[index].type;

	update();
}


void HeaderView::mouseReleaseEvent(QMouseEvent *e)
{
	if (e->button() != Qt::LeftButton) {
		QWidget::mouseReleaseEvent(e);
		return;
	}

	int index = indexAt(e->pos());
	if (index < 0)
		return;

	ItemFlag newSelectItem = s_items[index].type;
	if (newSelectItem == m_selectItem) {
		m_reverseSorting = !m_reverseSorting;
	}
	else {
		m_selectItem = newSelectItem;
		m_reverseSorting = false;
	}

	m_pressedItem = Item_None;

	update();
}


void HeaderView::mouseMoveEvent(QMouseEvent *e)
{
	int x = e->pos().x();

	int index = 0;
	while (s_items[index].offset < 0)
		index++;

	static const Qt::CursorShape defaultCursorShape = cursor().shape();

	for (int i = index; i < s_itemCount; i++) {
		int rightBoard = s_items[i].offset + s_items[i].width;
		if (rightBoard - 3 <= x && x < rightBoard) {
			setCursor(QCursor(Qt::SplitHCursor));
			return;
		}
	}
	if (cursor().shape() != defaultCursorShape)
		setCursor(QCursor(defaultCursorShape));
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
			if (s_items[index + 1].type & m_selectItem)
				option.selectedPosition = QStyleOptionHeader::NextIsSelected;
		}
		else if (index == s_itemCount - 1) {
			option.position = QStyleOptionHeader::End;
			if (s_items[index - 1].type & m_selectItem)
				option.selectedPosition = QStyleOptionHeader::PreviousIsSelected;
		}
		else {
			option.position = QStyleOptionHeader::Middle;
		}
	}

	if (s_items[index].type & m_selectItem) {
		if (m_reverseSorting)
			option.sortIndicator = QStyleOptionHeader::SortUp;
		else
			option.sortIndicator = QStyleOptionHeader::SortDown;
	}

	if (s_items[index].type == m_pressedItem)
		option.state |= QStyle::State_Sunken;

	option.rect = this->rect();
	option.rect.setLeft(s_items[index].offset);
	option.rect.setWidth(s_items[index].width);

	style()->drawControl(QStyle::CE_Header, &option, &painter, this);
}


int HeaderView::indexAt(const QPoint &pos)
{
	int index = 0;
	while (s_items[index].offset < 0)
		index++;

	for (int i = index; i < s_itemCount; i++) {
		if (s_items[i].offset <= pos.x() && pos.x() < s_items[i].offset + s_items[i].width)
			return i;
	}

	return -1;
}


void HeaderView::readSettings()
{
	QSettings *sets = kApp->settings();
	sets->beginGroup("HeaderView");

	m_selectItem = (ItemFlag) sets->value("SelectItem", Item_Name).toUInt();
	m_reverseSorting = sets->value("ReverseSorting", false).toBool();

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
	sets->setValue("SelectItem", (uint) m_selectItem);
	sets->setValue("ReverseSorting", m_reverseSorting);

	for (int i = 0; i < s_itemCount; i++) {
		sets->setValue(QString::number(s_items[i].type, 16) + ".Offset", s_items[i].offset);
		sets->setValue(QString::number(s_items[i].type, 16) + ".Width", s_items[i].width);
	}

	sets->endGroup();
}
