#ifndef __APP_HEADERITEM_H__
#define __APP_HEADERITEM_H__

#include <QString>

class QRect;
class QPainter;
class QFileInfo;


class AbstractHeaderItem
{
	friend class HeaderView;

public:
	AbstractHeaderItem(const QString &name, int id, int offset = -1, int width = 10);

	inline void setName(const QString &name) { m_name = name; }
	inline QString name() const { return m_name; }
	inline int id() const { return m_id; }
	inline int offset() const { return m_offset; }
	inline int width() const { return m_width; }

	inline bool operator < (const AbstractHeaderItem &item) const { return m_offset < item.m_offset; }

	virtual void drawFileItem(const QFileInfo &, const QRect &, QPainter &) {};

private:
	inline void setOffset(int offset) { m_offset = offset; }
	inline void setWidth(int width) { m_width = width; }
	inline void setId(int id) { m_id = id; }

private:
	QString m_name;
	int m_id;
	int m_offset;
	int m_width;
};


#endif //__APP_HEADERITEM_H__
