#ifndef __CORE_ABSTRACTHEADERITEM_H__
#define __CORE_ABSTRACTHEADERITEM_H__

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

	static const QString &nothingId();

	inline QString id() const { return m_id; }
	inline QString title() const { return m_title; }
	inline QString description() const { return m_description; }
	inline LessThan lessThanFunction() const { return m_lessThan; }
	inline int offset() const { return m_offset; }
	inline int width() const { return m_width; }

	virtual void drawFileItem(const QFileInfo &, const PaintOption &, QPainter &) {}

protected:
	QString m_id;		//!< must be unique
	QString m_title;
	QString m_description;
	LessThan m_lessThan;

private:
	int m_offset;
	int m_width;
};


#endif //__CORE_ABSTRACTHEADERITEM_H__
