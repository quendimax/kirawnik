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

public:
	explicit HeaderView(QWidget *parent = 0);
	virtual ~HeaderView();

protected:
	void contextMenuEvent(QContextMenuEvent *);
	void mousePressEvent(QMouseEvent *);
	void mouseReleaseEvent(QMouseEvent *);
	void mouseMoveEvent(QMouseEvent *);
	void paintEvent(QPaintEvent *);

private:
	enum HeaderState {
		HS_Free,           //!< mouse buttons is raisen
		HS_Pressing,       //!< mouse left button is pressing on not control items
		HS_OutPressing,    //!< mouse left button is pressing out of headers
		HS_Resizing,       //!< mouse left button is pressing on resize item
		HS_Moving          //!< mouse left button is pressing on header item
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
	void initMenu();
	void destroyMenu();
	void initStyleOption(QStyleOption *option);
	void paintSection(int index, QPainter &painter);
	void paintMovableSection(QPainter &painter);
	void updateAll();

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


#endif //__HEADERVIEW_H__
