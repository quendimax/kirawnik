#ifndef __APP_FILESYSTEMVIEW_H__
#define __APP_FILESYSTEMVIEW_H__

#include <QWidget>


class QPushButton;
class FilePanel;


/*!
  The FileSystemView class shows two file panel, and row of
  functional buttons.

  It must exist the single copy.
  */
class FileSystemView : public QWidget
{
	Q_OBJECT

public:
	static FilePanel *leftPanel();
	static FilePanel *rightPanel();
	static FilePanel *activePanel();
	static FilePanel *inactivePanel();

public:
	explicit FileSystemView(QWidget *parent = 0);
	virtual ~FileSystemView();

private:
	static FilePanel *m_leftPanel;
	static FilePanel *m_rightPanel;
	static FilePanel *m_activePanel;
	static FilePanel *m_inactivePanel;

private:
	QWidget *m_functionalBar;
	QPushButton *m_funcButtons;
};


inline FilePanel *FileSystemView::leftPanel() { return m_leftPanel; }
inline FilePanel *FileSystemView::rightPanel() { return m_rightPanel; }
inline FilePanel *FileSystemView::activePanel() { return m_activePanel; }
inline FilePanel *FileSystemView::inactivePanel() { return m_inactivePanel; }

#endif //__APP_FILESYSTEMVIEW_H__
