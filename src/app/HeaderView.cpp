#include <QPainter>
#include <QSettings>
#include <QMouseEvent>

#include "HeaderView.h"


HeaderView::HeaderItem HeaderView::s_items[] = {
	{ 0, 0, Item_Name, tr("Name") },
	{ 0, 0, Item_Suffix, tr("Ext") },
	{ 0, 0, Item_Size, tr("Name") },
	{ 0, 0, Item_TextPerms, tr("Perms (rwx)") },
	{ 0, 0, Item_DigitPerms, tr("Perms") },
	{ 0, 0, Item_Owner, tr("Owner") },
	{ 0, 0, Item_Group, tr("Group") },
	{ 0, 0, Item_Modified, tr("Modified") }
};



HeaderView::HeaderView(QWidget *parent)
	: QWidget(parent)
{
	setFixedHeight(22);
	m_showItemCount = m_itemCount = sizeof s_items / sizeof s_items[0];

	readSettings();
}


HeaderView::~HeaderView()
{
	//writeSettings();
}


void HeaderView::mousePressEvent(QMouseEvent *e)
{
	if (e->button() == Qt::LeftButton)
		m_pressedItem = Item_Name;

	update();
}


void HeaderView::mouseReleaseEvent(QMouseEvent *)
{
	m_pressedItem = Item_None;
	update();
}


void HeaderView::paintEvent(QPaintEvent *)
{
	QStyleOption option;
	initStyleOption(&option);
	option.rect = rect();

	QPainter painter(this);
	style()->drawControl(QStyle::CE_HeaderEmptyArea, &option, &painter, this);

	for (int i = 0; i < m_itemCount; i++) {
		if (s_items[i].offset < 0)
			continue;
		if (s_items[i].type & m_showItems)
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
	option.section = index - (m_itemCount - m_showItemCount);
	option.selectedPosition = QStyleOptionHeader::NotAdjacent;

	if (m_showItemCount == 1) {
		option.position = QStyleOptionHeader::OnlyOneSection;
	}
	else {
		if (index == m_itemCount - m_showItemCount) {
			option.position = QStyleOptionHeader::Beginning;
			if (s_items[index + 1].type & m_selectItem)
				option.selectedPosition = QStyleOptionHeader::NextIsSelected;
		}
		else if (index == m_itemCount - 1) {
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


void HeaderView::readSettings()
{
	static bool wasCalled = false;
	if (wasCalled) return;
	else wasCalled = true;

	QSettings *sets = new QSettings;
	sets->beginGroup("HeaderView");

	m_showItems = (ItemFlag) sets->value("ShowFlags", Item_All).toUInt();
	m_selectItem = (ItemFlag) sets->value("SelectFlag", Item_Name).toUInt();
	m_reverseSorting = sets->value("ReverseSorting", false).toBool();

	int offset = 0, width = 70;
	for (int i = 0; i < m_itemCount; i++) {
		s_items[i].offset = sets->value(QString::number(s_items[i].type, 16) + ".Offset", offset).toInt();
		s_items[i].width = sets->value(QString::number(s_items[i].type, 16) + ".Width", width).toInt();
		offset += width;
	}

	sets->endGroup();
	delete sets;
}


void HeaderView::writeSettings()
{
	QSettings *sets = new QSettings;
	sets->beginGroup("HeaderView");

	sets->setValue("ShowFlags", (uint) m_showItems);
	sets->setValue("SelectFlag", (uint) m_selectItem);
	sets->setValue("ReverseSorting", m_reverseSorting);

	for (int i = 0; i < m_itemCount; i++) {
		sets->setValue(QString::number(s_items[i].type, 16) + ".Offset", s_items[i].offset);
		sets->setValue(QString::number(s_items[i].type, 16) + ".Width", s_items[i].width);
	}

	sets->endGroup();
	delete sets;
}
