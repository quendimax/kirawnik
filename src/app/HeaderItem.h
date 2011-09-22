#ifndef __APP_HEADERITEM_H__
#define __APP_HEADERITEM_H__

#include <QString>

class QRect;
class QPainter;
class QFileInfo;


class HeaderItem
{
public:
	typedef void (*DrawFileItemFunc)(const QFileInfo &fi, const QRect &, QPainter &);

	inline QString name() const { return m_name; }
	inline QString type() const { return m_type; }
	inline int offset() const { return m_offset; }
	inline int width() const { return m_width; }
	inline DrawFileItemFunc drawFileItemFunc() const { return m_drawFileItemFunc; }

	inline void setName(const QString &name) { m_name = name; }
	inline void setType(const QString &type) { m_type = type; }
	inline void setOffset(int offset) { m_offset = offset; }
	inline void setWidth(int width) { m_width = width; }
	inline void setDrawFileItemFunc(DrawFileItemFunc func) { m_drawFileItemFunc = func; }

	inline bool operator < (const HeaderItem &item) const { return m_offset < item.m_offset; }

private:
	QString m_name;
	QString m_type;
	int m_offset;
	int m_width;
	DrawFileItemFunc m_drawFileItemFunc;
};


#endif //__APP_HEADERITEM_H__
