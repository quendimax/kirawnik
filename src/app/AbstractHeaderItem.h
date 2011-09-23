#ifndef __APP_ABSTRACTHEADERITEM_H__
#define __APP_ABSTRACTHEADERITEM_H__

#include <QString>

class QRect;
class QPainter;
class QFileInfo;


class AbstractHeaderItem
{
	friend class HeaderView;

public:
	AbstractHeaderItem(const QString &name, int id, int offset = -1, int width = 10);

	inline QString name() const { return m_name; }
	inline int id() const { return m_id; }
	inline int offset() const { return m_offset; }
	inline int width() const { return m_width; }

	inline bool operator < (const AbstractHeaderItem &item) const { return m_offset < item.m_offset; }

	virtual void drawFileItem(const QFileInfo &, const QRect &, QPainter &) {};

private:
	QString m_name;
	int m_id;
	int m_offset;
	int m_width;
};


#endif //__APP_ABSTRACTHEADERITEM_H__
