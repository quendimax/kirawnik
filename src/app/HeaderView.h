#ifndef __HEADERVIEW_H__
#define __HEADERVIEW_H__

#include <QStyleOptionHeader>
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
	QString sectionName(int index) const;
	Krw::SortFlag sectionSort(int index) const;
	Krw::SortFlags showSections() const;
	bool isReverseSorting() const;

	int logicalIndex(Krw::SortFlag f) const;
	int count() const;
	int hiddenCount() const;

signals:
	void geometryChanged();
	void sortingChanged(Krw::SortFlag flag, bool reverse);

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
		int offset;
		int width;
		Krw::SortFlag type;
		QString name;

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
	int indexAt(Krw::SortFlag flag) const;

	void readSettings();
	void writeSettings();

private:
	static QList<HeaderView *> s_headerViews;
	static HeaderItem s_items[];
	static Krw::SortFlags s_showItems;
	static int s_showItemCount;
	static int s_itemCount;
	static int s_objectCount;
	static QMenu *s_menu;
	static HeaderItem s_movableItem;

	int m_objectNumber;
	QPoint m_oldPos;
	HeaderState m_headerState;
	Krw::SortFlag m_sortingItem;
	Krw::SortFlag m_pressedItem;
	int m_resizeItem;
	bool m_reverseSorting;
};


inline int HeaderView::sectionOffset(int index) const { return s_items[s_itemCount - s_showItems + index].offset; }
inline int HeaderView::sectionSize(int index) const { return s_items[s_itemCount - s_showItems + index].width; }
inline QString HeaderView::sectionName(int index) const { return s_items[s_itemCount - s_showItems + index].name; }
inline Krw::SortFlag HeaderView::sectionSort(int index) const { return s_items[s_itemCount - s_showItems + index].type; }
inline Krw::SortFlags HeaderView::showSections() const { return s_showItems; }
inline bool HeaderView::isReverseSorting() const { return m_reverseSorting; }

inline int HeaderView::logicalIndex(Krw::SortFlag f) const { return indexAt(f) - (s_itemCount - s_showItemCount); }
inline int HeaderView::count() const { return s_showItemCount; }
inline int HeaderView::hiddenCount() const { return s_itemCount - s_showItemCount; }


#endif //__HEADERVIEW_H__
