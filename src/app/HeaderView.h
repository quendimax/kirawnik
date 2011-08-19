#ifndef __HEADERVIEW_H__
#define __HEADERVIEW_H__

#include <QStyleOptionHeader>
#include <QList>
#include <QWidget>

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
	void initStyleOption(QStyleOption *option);
	void paintSection(int index, QPainter &painter);
	void updateAll();

	int indexAt(const QPoint &pos, bool *isResize = 0);

	void readSettings();
	void writeSettings();

private:
	enum HeaderState {
		HS_Free,
		HS_Pressing,
		HS_Resizing,
		HS_Moving
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
	static ItemFlag s_showItems;
	static int s_showItemCount;
	static int s_itemCount;

	int m_objectNumber;

	QPoint m_pos;
	HeaderState m_headerState;
	ItemFlag m_sortingItem;
	int m_pressedItem;
	int m_resizeItem;
	bool m_reverseSorting;
};


#endif //__HEADERVIEW_H__
