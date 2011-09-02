#ifndef __FILEVIEW_H__
#define __FILEVIEW_H__

#include <QBitArray>
#include <QFileInfoList>
#include <QFileIconProvider>
#include <QWidget>

class QScrollBar;
class HeaderView;


class FileView : public QWidget
{
	Q_OBJECT

public:
	explicit FileView(HeaderView *, QWidget *parent = 0);
	virtual ~FileView();

	int current() const;

	void setFileInfoList(const QFileInfoList &list);

protected:
	void paintEvent(QPaintEvent *);
	void resizeEvent(QResizeEvent *);
	void keyPressEvent(QKeyEvent *);
	void mousePressEvent(QMouseEvent *);
	void wheelEvent(QWheelEvent *);

private:
	void paintItem(int index, QPainter &);
	void paintBackground(QPainter &);
	void paintCursor(QPainter &);

	void drawName(int index, const QRect &, QPainter &);
	void drawNameColumn(QPainter &);
	void drawSuffix(int index, const QRect &, QPainter &);
	void drawSuffixColumn(QPainter &);
	void drawSize(int index, const QRect &, QPainter &);
	void drawSizeColumn(QPainter &);
	void drawTextPerms(int index, const QRect &, QPainter &);
	void drawTextPermsColumn(QPainter &);
	void drawDigitPerms(int index, const QRect &, QPainter &);
	void drawDigitPermsColumn(QPainter &);
	void drawOwner(int index, const QRect &, QPainter &);
	void drawOwnerColumn(QPainter &);
	void drawGroup(int index, const QRect &, QPainter &);
	void drawGroupColumn(QPainter &);
	void drawModified(int index, const QRect &, QPainter &);
	void drawModifiedColumn(QPainter &);

	QRect makeRectForSection(int index, int top) const;

	void readSettings();
	void writeSettings();

private:
	static const int Margin = 3;

	HeaderView *e_header;
	QScrollBar *m_scroll;
	QFileInfoList m_fileList;
	QFileIconProvider m_iconProvider;
	QBitArray m_selectItems;

	int m_width;             //!< fileview width without scrollbar width
	int m_current;

	QColor m_cursorColor;
	QColor m_textColor;
	QColor m_selectTextColor;
	QColor m_baseColor[2];   // main and alternate
	QColor m_selectBaseColor;
	int m_itemHeight;
	bool m_cursorIsFull;
};


inline int FileView::current() const { return m_current; }


#endif //__FILEVIEW_H__
