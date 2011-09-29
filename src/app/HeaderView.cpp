#include <QCursor>
#include <QLabel>
#include <QPainter>
#include <QSettings>
#include <QMouseEvent>
#include <QMenu>
#include <QFileIconProvider>
#include <QAction>

#include "Application.h"
#include "HeaderView.h"


int HeaderView::s_showItemCount;
int HeaderView::s_objectCount = 0;
QBitArray HeaderView::s_showItems;
AbstractHeaderItem HeaderView::s_movableItem;
QList<HeaderView *> HeaderView::s_headerViews;
QMenu *HeaderView::s_menu = 0;
QList<AbstractHeaderItem *> HeaderView::s_items;



HeaderView::HeaderView(QWidget *parent)
	: QWidget(parent)
{
	// must be first
	m_objectNumber = s_objectCount;
	s_objectCount++;
	Q_ASSERT(0 < s_objectCount && s_objectCount <= 2);

	s_headerViews.append(this);

	m_headerState = HS_Free;
	m_pressedItemId = -1;		// nothings is pressed
	m_resizeItemIndex = -1;

	QFontMetrics metrics(font());
	setFixedHeight(metrics.height() + 8);

	setMouseTracking(true);

	readSettings();

	// will called with first object only
	if (m_objectNumber == 0) {
		initMenu();
		sortItems();
	}
}


HeaderView::~HeaderView()
{
	// will called with last object only
	if (m_objectNumber == 0) {
		destroyMenu();
	}
	writeSettings();
	s_headerViews.removeAll(this);

	// must be last
	s_objectCount--;
}


void HeaderView::contextMenuEvent(QContextMenuEvent *e)
{
	QAction *action = s_menu->exec(e->globalPos());
	if (action) {
		bool ok;
		int id = action->data().toInt(&ok);
		Q_ASSERT(ok);

		if (s_showItemCount == 1 && id == m_sortingItemId) {
			action->setChecked(true);
			return;
		}

		int index = indexAt(id);
		Q_ASSERT(-1 < index && index < s_items.size());

		if (action->isChecked()) {
			s_showItems.setBit(id);
			s_items[index]->m_offset = 0;
			for (int i = s_items.size() - s_showItemCount; i < s_items.size(); i++)
				s_items[i]->m_offset += s_items[index]->width();

			s_showItemCount++;
		}
		else {
			s_showItems.clearBit(id);
			s_items[index]->m_offset = -1;
			for (int i = index + 1; i < s_items.size(); i++)
				s_items[i]->m_offset -= s_items[index]->width();

			foreach (HeaderView *header, s_headerViews) {
				if (s_items[index]->id() == header->m_sortingItemId) {
					for (int i = s_items.size() - s_showItemCount; i < s_items.size(); i++) {
						if (i != index) {
							header->m_sortingItemId = s_items[i]->id();
							break;
						}
					}
					header->m_reverseSorting = false;
				}
			}

			s_showItemCount--;
		}

		sortItems();
		updateAll();
		emit geometryChanged();
	}
}


void HeaderView::mousePressEvent(QMouseEvent *e)
{
	if (e->button() != Qt::LeftButton) {
		QWidget::mousePressEvent(e);
		return;
	}

	m_oldPos = e->pos();

	bool isResize;
	int index = indexAt(e->pos(), &isResize);

	if (index < 0) {
		m_headerState = HS_OutPressing;
		return;
	}
	if (isResize) {
		m_headerState = HS_Resizing;
		m_resizeItemIndex = index;
	}
	else {
		m_headerState = HS_Pressing;
		m_pressedItemId = s_items[index]->id();
	}

	update();
}


void HeaderView::mouseReleaseEvent(QMouseEvent *e)
{
	if (e->button() != Qt::LeftButton) {
		QWidget::mouseReleaseEvent(e);
		return;
	}

	if (m_pressedItemId >= 0 && m_headerState != HS_Moving) {
		int index = indexAt(e->pos());

		if (s_items[index]->id() == m_pressedItemId) {
			int newSortingItemId = s_items[index]->id();
			if (newSortingItemId == m_sortingItemId) {
				m_reverseSorting = !m_reverseSorting;
			}
			else {
				m_sortingItemId = newSortingItemId;
				m_reverseSorting = false;
			}
			emit sortingChanged(newSortingItemId, m_reverseSorting);
		}
	}
	m_pressedItemId = -1;
	m_resizeItemIndex = -1;
	s_movableItem.m_id = -1;
	m_headerState = HS_Free;

	updateAll();
}


void HeaderView::mouseMoveEvent(QMouseEvent *e)
{
	switch (m_headerState) {
	case HS_Free:
	case HS_OutPressing:{
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
		int delta = e->pos().x() - m_oldPos.x();
		if (s_items[m_resizeItemIndex]->width() < 32 && delta < 0)
			break;

		s_items[m_resizeItemIndex]->m_width += delta;
		for (int i = m_resizeItemIndex + 1; i < s_items.size(); i++)
			s_items[i]->m_offset += delta;
		m_oldPos = e->pos();
		updateAll();
		emit geometryChanged();
		}
		break;

	case HS_Pressing:
		if ((e->pos() - m_oldPos).manhattanLength() > QApplication::startDragDistance()) {
			int index = indexAt(m_oldPos);
			Q_ASSERT(index > -1);
			s_movableItem = *(s_items[index]);
			m_headerState = HS_Moving;
			update();
		}
		break;

	case HS_Moving: {
		s_movableItem.m_offset += e->pos().x() - m_oldPos.x();
		m_oldPos = e->pos();
		int index = indexAt(s_movableItem.id());

		for (int i = s_items.size() - s_showItemCount; i < s_items.size(); i++) {
			if (s_items[i]->id() == s_movableItem.id())
				continue;
			int minWidth = qMin(s_items[i]->width(), s_items[index]->width());
			if (s_items[i]->offset() <= s_movableItem.offset() && s_movableItem.offset() < s_items[i]->offset() + minWidth) {
				qSwap(s_items[index]->m_offset, s_items[i]->m_offset);
				if (s_items[i]->offset() > s_items[index]->offset())
					s_items[i]->m_offset = s_items[index]->offset() + s_items[index]->width();
				else
					s_items[index]->m_offset = s_items[i]->offset() + s_items[i]->width();
				sortItems();
				emit geometryChanged();
				break;
			}
		}
		updateAll();
		}
		break;
	}
}


void HeaderView::paintEvent(QPaintEvent *)
{
	QStyleOption option;
	initStyleOption(&option);
	option.rect = rect();
	if (s_items.isEmpty())
		option.rect.setLeft(0);
	else
		option.rect.setLeft(s_items[s_items.size()-1]->offset() + s_items[s_items.size()-1]->width());

	QPainter painter(this);
	style()->drawControl(QStyle::CE_HeaderEmptyArea, &option, &painter, this);

	for (int i = 0; i < s_items.size(); i++) {
		if (s_items[i]->offset() < 0)
			continue;
		if (s_showItems.at(s_items[i]->id()))
			paintSection(i, painter);
	}

	if (s_movableItem.id() != -1) {
		paintMovableSection(painter);
	}
}


/*!
  It should be called before first sorting of s_items,
  and after reading s_items' settings.
  */
void HeaderView::initMenu()
{
	s_menu = new QMenu;

	for (int i = 0; i < s_items.size(); i++) {
		QAction *action = s_menu->addAction(s_items[i]->name() + " - " + s_items[i]->description());
		action->setData(s_items[i]->id());
		action->setCheckable(true);
		if (s_showItems.at(s_items[i]->id()))
			action->setChecked(true);
		else
			action->setChecked(false);
	}
}


void HeaderView::destroyMenu()
{
	delete s_menu;
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
	if (s_items[index]->id() == s_movableItem.id()) {
		QStyleOptionButton option;
		initStyleOption(&option);

		option.features = QStyleOptionButton::None;
		option.rect = this->rect();
		option.rect.setLeft(s_items[index]->offset());
		option.rect.setWidth(s_items[index]->width());

		style()->drawControl(QStyle::CE_PushButton, &option, &painter, this);
	}
	else {
		QStyleOptionHeader option;
		initStyleOption(&option);

		option.text = s_items[index]->name();
		option.section = index - (s_items.size() - s_showItemCount);
		option.selectedPosition = QStyleOptionHeader::NotAdjacent;

		if (s_showItemCount == 1) {
			option.position = QStyleOptionHeader::OnlyOneSection;
		}
		else {
			if (index == s_items.size() - s_showItemCount) {
				option.position = QStyleOptionHeader::Beginning;
				if (s_items[index + 1]->id() == m_sortingItemId)
					option.selectedPosition = QStyleOptionHeader::NextIsSelected;
			}
			else if (index == s_items.size() - 1) {
				option.position = QStyleOptionHeader::End;
				if (s_items[index - 1]->id() == m_sortingItemId)
					option.selectedPosition = QStyleOptionHeader::PreviousIsSelected;
			}
			else {
				option.position = QStyleOptionHeader::Middle;
			}
		}

		if (s_items[index]->id() == m_sortingItemId) {
			if (m_reverseSorting)
				option.sortIndicator = QStyleOptionHeader::SortUp;
			else
				option.sortIndicator = QStyleOptionHeader::SortDown;
		}

		if (s_items[index]->id() == m_pressedItemId)
			option.state |= QStyle::State_Sunken;

		option.rect = this->rect();
		option.rect.setLeft(s_items[index]->offset());
		option.rect.setWidth(s_items[index]->width());

		style()->drawControl(QStyle::CE_Header, &option, &painter, this);
	}
}


void HeaderView::paintMovableSection(QPainter &painter)
{
	QStyleOptionHeader option;
	initStyleOption(&option);
	option.state |= QStyle::State_Sunken;

	option.text = s_movableItem.name();
	option.section = 0;
	option.selectedPosition = QStyleOptionHeader::NotAdjacent;
	option.position = QStyleOptionHeader::OnlyOneSection;
	option.sortIndicator = QStyleOptionHeader::None;
	if (s_movableItem.id() == m_sortingItemId) {
		if (m_reverseSorting)
			option.sortIndicator = QStyleOptionHeader::SortUp;
		else
			option.sortIndicator = QStyleOptionHeader::SortDown;
	}

	option.rect = this->rect();
	option.rect.setLeft(s_movableItem.offset());
	option.rect.setWidth(s_movableItem.width());

	style()->drawControl(QStyle::CE_Header, &option, &painter, this);
}


void HeaderView::updateAll()
{
	foreach (HeaderView *view, s_headerViews)
		view->update();
}


int HeaderView::indexAt(const QPoint &pos, bool *isResize) const
{
	if (s_items.isEmpty()) return -1;

	int index = 0;
	while (s_items[index]->offset() < 0)
		index++;

	if (0 <= pos.y() && pos.y() < height()) {
		for (int i = index; i < s_items.size(); i++) {
			if (s_items[i]->offset() <= pos.x() && pos.x() < s_items[i]->offset() + s_items[i]->width()) {
				if (isResize) {
					*isResize = false;
					int rightBorder = s_items[i]->offset() + s_items[i]->width();
					if (rightBorder - 3 <= pos.x() && pos.x() < rightBorder)
						*isResize = true;
				}
				return i;
			}
		}
	}

	return -1;
}


int HeaderView::indexAt(int id) const
{
	for (int i = 0; i < s_items.size(); i++)
		if (s_items[i]->id() == id)
			return i;
	return -1;
}


#include "plugins/interfaces/HeaderPluginInterface.h"
#include <QPluginLoader>


void HeaderView::readSettings()
{
	QSettings *sets = kApp->settings();
	sets->beginGroup("HeaderView");

	bool ok;
	m_sortingItemId = sets->value("SortingItemId." + QString::number(m_objectNumber), 0).toInt(&ok);
	Q_ASSERT(ok);
	m_reverseSorting = sets->value("ReverseSorting." + QString::number(m_objectNumber), false).toBool();

	if (s_objectCount == 1) {
		QPluginLoader pluginLoader("../share/kirawnik/plugins/libkplugin_standartheaders.so");
		static int idcount = 0;
		if (HeaderPluginInterface *plugin = qobject_cast<HeaderPluginInterface *>(pluginLoader.instance())) {
			QList<AbstractHeaderItem *> list = plugin->getHeaderItems();
			for (int i = 0; i < list.size(); i++) {
				s_items.append(list[i]);
				s_items.back()->m_id = idcount++;
			}
		}

		s_showItems = sets->value("ShowItems", QBitArray(s_items.size(), true)).toBitArray();

		s_showItemCount = 0;
		int offset = 0, width = 60;
		for (int i = 0; i < s_items.size(); i++) {
			s_items[i]->m_offset = sets->value(QString::number(s_items[i]->id(), 16) + ".Offset", offset).toInt(&ok);
			Q_ASSERT(ok);
			s_items[i]->m_width = sets->value(QString::number(s_items[i]->id(), 16) + ".Width", width).toInt(&ok);
			Q_ASSERT(ok);
			offset += s_items[i]->m_width;

			if (s_items[i]->m_offset >= 0)
				s_showItemCount++;
		}
	}

	sets->endGroup();
}


void HeaderView::writeSettings()
{
	QSettings *sets = kApp->settings();
	sets->beginGroup("HeaderView");

	sets->setValue("SortingItemId." + QString::number(m_objectNumber), m_sortingItemId);
	sets->setValue("ReverseSorting." + QString::number(m_objectNumber), m_reverseSorting);

	if (s_objectCount == 1) {
		sets->setValue("ShowItems", s_showItems);

		for (int i = 0; i < s_items.size(); i++) {
			sets->setValue(QString::number(s_items[i]->id(), 16) + ".Offset", s_items[i]->offset());
			sets->setValue(QString::number(s_items[i]->id(), 16) + ".Width", s_items[i]->width());
		}
	}

	sets->endGroup();
}


void HeaderView::sortItems()
{
	if (s_items.size() > 1)
		sortItems(0, s_items.size() - 1);
}


void HeaderView::sortItems(int left, int right)
{
	int x = s_items[(left + right)/2]->offset();
	int i = left;
	int j = right;

	do {
		while (s_items[i]->offset() < x) i++;
		while (x < s_items[j]->offset()) j--;
		if (i <= j) {
			qSwap(s_items[i], s_items[j]);
			i++, j--;
		}
	}
	while (i <= j);

	if (left < j) sortItems(left, j);
	if (i < right) sortItems(i, right);
}
