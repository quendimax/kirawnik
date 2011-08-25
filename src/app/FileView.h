#ifndef __FILEVIEW_H__
#define __FILEVIEW_H__

#include <QFileInfoList>
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

protected:
	void paintEvent(QPaintEvent *);
	void resizeEvent(QResizeEvent *);
	void keyPressEvent(QKeyEvent *);

private:
	void paintBackground(QPainter &);

	void readSettings();
	void writeSettings();

private:
	HeaderView *e_header;
	QScrollBar *m_scroll;
	QFileInfoList m_fileList;

	int m_current;
	int m_start;             //!< start item index for painting

	QFont m_font;
	QColor m_cursorColor;
	QColor m_textColor;
	QColor m_curentTextColor;
	QColor m_selectTextColor;
	QColor m_baseColor[2];  // main and alternate
	QColor m_selectBaseColor;
	int m_itemHeight;
	bool m_cursorIsFull;
};


inline int FileView::current() const { return m_current; }


#endif //__FILEVIEW_H__
