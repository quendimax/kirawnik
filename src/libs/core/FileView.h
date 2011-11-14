#ifndef __CORE_FILEVIEW_H__
#define __CORE_FILEVIEW_H__

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

public slots:
	void settingsUpdate();
	void updateAll();

protected:
	void paintEvent(QPaintEvent *);
	void resizeEvent(QResizeEvent *);
	void keyPressEvent(QKeyEvent *);
	void mousePressEvent(QMouseEvent *);
	void wheelEvent(QWheelEvent *);

private:
	void paintBackground(int start, int finish, QPainter &);
	void paintForeground(int start, int finish, QPainter &);
	void paintCursor(QPainter &);

	void initBuffer();
	void initScroll();
	void updateItem(int index);
	QRect makeRectForSection(int index, int top) const;

	void readSettings();
	void writeSettings();

private:
	static const int Margin = 2;
	static QList<FileView *> s_fileViewes;

	HeaderView *e_header;
	QScrollBar *m_scroll;
	QFileInfoList m_fileList;
	QFileIconProvider m_iconProvider;
	QBitArray m_selectItems;
	QPixmap m_buffer;

	int m_width;             //!< fileview width without scrollbar width
	int m_current;
	int m_prevScrollValue;

	QColor m_cursorColor;
	QColor m_textColor;
	QColor m_selectTextColor;
	QColor m_baseColor[2];   // main and alternate
	QColor m_selectBaseColor;
	int m_itemHeight;
	bool m_cursorIsFull;
	bool m_showSelectBackground;
};


inline int FileView::current() const { return m_current; }


#endif //__CORE_FILEVIEW_H__
