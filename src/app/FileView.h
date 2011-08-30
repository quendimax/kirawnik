#ifndef __FILEVIEW_H__
#define __FILEVIEW_H__

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

private:
	void paintItem(int index, QPainter &);
	void paintBackground(QPainter &);

	void drawNamePart(QPainter &);
	void drawSuffixPart(QPainter &);
	void drawSizePart(QPainter &);
	void drawTextPermsPart(QPainter &);
	void drawDigitPermsPart(QPainter &);
	void drawOwnerPart(QPainter &);
	void drawGroupPart(QPainter &);
	void drawModifiedPart(QPainter &);

	QRect makeRectForSection(int index, int top) const;

	void readSettings();
	void writeSettings();

private:
	static const int Margin = 2;

	HeaderView *e_header;
	QScrollBar *m_scroll;
	QFileInfoList m_fileList;
	QFileIconProvider m_iconProvider;

	int m_current;
	int m_start;             //!< start item index for painting

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
inline void FileView::setFileInfoList(const QFileInfoList &list) { m_fileList = list; }


#endif //__FILEVIEW_H__
