#include "AbstractHeaderItem.h"


AbstractHeaderItem::AbstractHeaderItem()
	: m_name(QString()),
	  m_title(QObject::tr("Unknown")),
	  m_description(QObject::tr("I don't know :(")),
	  m_lessThan(nullptr),
	  m_id(-1),
	  m_offset(0),
	  m_width(0)
{}
