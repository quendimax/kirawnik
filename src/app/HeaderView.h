#ifndef __HEADERVIEW_H__
#define __HEADERVIEW_H__

#include <QStyleOptionHeader>
#include <QList>
#include <QWidget>

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
	void initMenu();
	void destroyMenu();
	void initStyleOption(QStyleOption *option);
	void paintSection(int index, QPainter &painter);
	void updateAll();

	int indexAt(const QPoint &pos, bool *isResize = 0);

	void readSettings();
	void writeSettings();

private:
	enum HeaderState {
		HS_Free,           //!< mouse buttons is raisen
		HS_Pressing,       //!< mouse left button is pressing on not control items
		HS_OutPressing,    //!< mouse left button is pressing out of headers
		HS_Resizing,       //!< mouse left button is pressing on resize item
		HS_Moving          //!< mouse left button is pressing on header item
	};

	enum ItemFlag {
		Item_Name = 0x01,
		Item_Suffix = 0x02,
		Item_Size = 0x04,
		Item_TextPerms = 0x08,
		Item_DigitPerms = 0x10,
		Item_Owner = 0x20,
		Item_Group = 0x40,
		Item_Modified = 0x80,

		Item_None = 0,
		Item_All = 0xFF
	};
	typedef QFlags<ItemFlag> ItemFlags;

	struct HeaderItem
	{
		int offset;
		int width;
		HeaderView::ItemFlag type;
		QString name;

		inline bool operator < (const HeaderItem &item) const { return offset < item.offset; }
	};

private:
	static QList<HeaderView *> s_headerViews;
	static HeaderItem s_items[];
	static ItemFlags s_showItems;
	static int s_showItemCount;
	static int s_itemCount;
	static int s_objectCount;
	static QMenu *s_menu;

	int m_objectNumber;

	QPoint m_oldPos;
	HeaderState m_headerState;
	ItemFlag m_sortingItem;
	int m_pressedItem;
	int m_resizeItem;
	bool m_reverseSorting;
};


#endif //__HEADERVIEW_H__
