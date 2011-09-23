#ifndef __APP_HEADERVIEW_H__
#define __APP_HEADERVIEW_H__

#include <QStyleOptionHeader>
#include <QBitArray>
#include <QList>
#include <QWidget>

#include "AbstractHeaderItem.h"

class QMenu;
class QPainter;
class QStyleOption;


class HeaderView : public QWidget
{
	Q_OBJECT

public:
	explicit HeaderView(QWidget *parent = 0);
	virtual ~HeaderView();

	AbstractHeaderItem *headerItem(int index) const;
	bool sectionIsShowing(int index) const;

	bool isReverseSorting() const;
	int sortingId() const;

	int logicalIndex(int id) const;
	int count() const;
	int hiddenCount() const;

signals:
	void geometryChanged();
	void sortingChanged(int id, bool reverse);

protected:
	void contextMenuEvent(QContextMenuEvent *);
	void mousePressEvent(QMouseEvent *);
	void mouseReleaseEvent(QMouseEvent *);
	void mouseMoveEvent(QMouseEvent *);
	void paintEvent(QPaintEvent *);

private:
	enum HeaderState {
		HS_Free,           //!< mouse buttons is raisen
		HS_Pressing,       //!< mouse left button is pressing on header item
		HS_OutPressing,    //!< mouse left button is pressing out of headers
		HS_Resizing,       //!< mouse left button is pressing on resize item
		HS_Moving          //!< mouse left button is pressing and moving header item
	};

private:
	static void initMenu();
	static void destroyMenu();
	static void updateAll();

	void initStyleOption(QStyleOption *option);
	void paintSection(int index, QPainter &painter);
	void paintMovableSection(QPainter &painter);

	int indexAt(const QPoint &pos, bool *isResize = 0) const;
	int indexAt(int id) const;
	int realyIndex(int logicalIndex) const;

	void readSettings();
	void writeSettings();

private:
	static QList<HeaderView *> s_headerViews;
	static QList<AbstractHeaderItem *> s_items;
	static QBitArray s_showItems;
	static int s_showItemCount;
	static int s_objectCount;
	static QMenu *s_menu;
	static AbstractHeaderItem s_movableItem;

	int m_objectNumber;
	QPoint m_oldPos;
	HeaderState m_headerState;
	int m_sortingItemId;
	int m_pressedItemId;
	int m_resizeItemId;
	bool m_reverseSorting;
};


inline AbstractHeaderItem *HeaderView::headerItem(int index) const { return s_items[realyIndex(index)]; }
inline bool HeaderView::sectionIsShowing(int index) const { return s_showItems.at(s_items[realyIndex(index)]->id()); }

inline int HeaderView::sortingId() const { return m_sortingItemId; }
inline bool HeaderView::isReverseSorting() const { return m_reverseSorting; }

inline int HeaderView::logicalIndex(int id) const { return indexAt(id) - (s_items.size() - s_showItemCount); }
inline int HeaderView::count() const { return s_showItemCount; }
inline int HeaderView::hiddenCount() const { return s_items.size() - s_showItemCount; }

inline int HeaderView::realyIndex(int logicalIndex) const { return s_items.size() - s_showItemCount + logicalIndex; }


#endif //__APP_HEADERVIEW_H__
