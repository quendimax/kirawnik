#ifndef __FILEVIEW_H__
#define __FILEVIEW_H__

#include <QWidget>

class QScrollBar;
class HeaderView;


class FileView : public QWidget
{
	Q_OBJECT

public:
	explicit FileView(HeaderView *, QWidget *parent = 0);
	virtual ~FileView();

private:
	HeaderView *e_header;
	QFont m_font;
	bool m_cursorIsFull;
	QColor m_cursorColor;
	QColor m_textColor;
	QColor m_backgroundColor[2]; // main and alternate
	QColor m_selectTextColor;
	QColor m_selectBackgroundColor[2];
	int m_scrollPosition;
	QScrollBar *m_scroll;
};


#endif //__FILEVIEW_H__
