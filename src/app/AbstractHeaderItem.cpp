#include "AbstractHeaderItem.h"


AbstractHeaderItem::AbstractHeaderItem()
{
	m_name = QObject::tr("Unknown");
	m_description = QObject::tr("I don't know :(");
	m_lessThan = 0;
	m_id = -1;
	m_offset = 0;
	m_width = 0;
}
