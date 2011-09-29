#ifndef __APP_ABSTRACTHEADERITEM_H__
#define __APP_ABSTRACTHEADERITEM_H__

#include <QFont>
#include <QRect>
#include <QString>

class QPainter;
class QFileInfo;


class AbstractHeaderItem
{
	friend class HeaderView;

public:
	struct PaintOption {
		QRect rect;
		int margin;
	};

	typedef bool (*LessThan)(const QFileInfo &, const QFileInfo &);

public:
	AbstractHeaderItem();

	inline QString name() const { return m_name; }
	inline QString description() const { return m_description; }
	inline int id() const { return m_id; }
	inline int offset() const { return m_offset; }
	inline int width() const { return m_width; }

	inline LessThan lessThanFunction() const { return m_lessThan; }

	virtual void drawFileItem(const QFileInfo &, const PaintOption &, QPainter &) {};

protected:
	LessThan m_lessThan;
	QString m_name;
	QString m_description;

private:
	int m_id;
	int m_offset;
	int m_width;
};


#endif //__APP_ABSTRACTHEADERITEM_H__
