#ifndef __HEADERVIEW_H__
#define __HEADERVIEW_H__

#include <QStyleOptionHeader>
#include <QBitArray>
#include <QList>
#include <QWidget>

#include "Global.h"

class QMenu;
class QPainter;
class QStyleOption;


class HeaderView : public QWidget
{
	Q_OBJECT

	friend class FilePanel;

public:
	explicit HeaderView(QWidget *parent = 0);
	virtual ~HeaderView();

	int sectionOffset(int index) const;
	int sectionSize(int index) const;
	Krw::SortingType sectionType(int index) const;
	QString sectionName(int index) const;
	bool sectionIsShowing(int index) const;

	bool isReverseSorting() const;
	Krw::SortingType sortingType() const;

	int logicalIndex(Krw::SortingType f) const;
	int count() const;
	int hiddenCount() const;

signals:
	void geometryChanged();
	void sortingChanged(Krw::SortingType flag, bool reverse);

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

	struct HeaderItem
	{
		QString name;
		Krw::SortingType type;
		int offset;
		int width;

		inline bool operator < (const HeaderItem &item) const { return offset < item.offset; }
	};

private:
	static void initMenu();
	static void destroyMenu();
	static void updateAll();

	void initStyleOption(QStyleOption *option);
	void paintSection(int index, QPainter &painter);
	void paintMovableSection(QPainter &painter);

	int indexAt(const QPoint &pos, bool *isResize = 0) const;
	int indexAt(Krw::SortingType flag) const;
	int realyIndex(int logicalIndex) const;

	void readSettings();
	void writeSettings();

private:
	static QList<HeaderView *> s_headerViews;
	static HeaderItem s_items[];
	static QBitArray s_showItems;
	static int s_showItemCount;
	static int s_itemCount;
	static int s_objectCount;
	static QMenu *s_menu;
	static HeaderItem s_movableItem;

	int m_objectNumber;
	QPoint m_oldPos;
	HeaderState m_headerState;
	Krw::SortingType m_sortingItem;
	Krw::SortingType m_pressedItem;
	int m_resizeItem;
	bool m_reverseSorting;
};


inline int HeaderView::sectionOffset(int index) const { return s_items[realyIndex(index)].offset; }
inline int HeaderView::sectionSize(int index) const { return s_items[realyIndex(index)].width; }
inline Krw::SortingType HeaderView::sectionType(int index) const { return s_items[realyIndex(index)].type; }
inline QString HeaderView::sectionName(int index) const { return s_items[realyIndex(index)].name; }
inline bool HeaderView::sectionIsShowing(int index) const { return s_showItems.at(s_items[realyIndex(index)].type); }

inline bool HeaderView::isReverseSorting() const { return m_reverseSorting; }
inline Krw::SortingType HeaderView::sortingType() const { return m_sortingItem; }

inline int HeaderView::logicalIndex(Krw::SortingType f) const { return indexAt(f) - (s_itemCount - s_showItemCount); }
inline int HeaderView::count() const { return s_showItemCount; }
inline int HeaderView::hiddenCount() const { return s_itemCount - s_showItemCount; }

inline int HeaderView::realyIndex(int logicalIndex) const { return s_itemCount - s_showItemCount + logicalIndex; }


#endif //__HEADERVIEW_H__
