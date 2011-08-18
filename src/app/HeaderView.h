#ifndef __HEADERVIEW_H__
#define __HEADERVIEW_H__

#include <QStyleOptionHeader>
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
	void paintEvent(QPaintEvent *);
	void mousePressEvent(QMouseEvent *);
	void mouseReleaseEvent(QMouseEvent *);

private:
	void initStyleOption(QStyleOption *option);
	void paintSection(int index, QPainter &painter);

	void readSettings();
	void writeSettings();

private:
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
	static HeaderItem s_items[];

	int m_itemCount;
	int m_showItemCount;
	ItemFlag m_showItems;
	ItemFlag m_selectItem;
	ItemFlag m_pressedItem;
	bool m_reverseSorting;
};


#endif //__HEADERVIEW_H__
