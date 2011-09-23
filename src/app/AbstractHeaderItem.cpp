#include "AbstractHeaderItem.h"


AbstractHeaderItem::AbstractHeaderItem(const QString &name, int id, int offset, int width)
    : m_name(name),
      m_id(id),
      m_offset(offset),
      m_width(width)
{
}
