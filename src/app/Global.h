#ifndef __GLOBAL_H__
#define __GLOBAL_H__

#include <QFlags>

#define KRW_NAMESPACE_BEGIN namespace Krw {
#define KRW_NAMESPACE_END }


KRW_NAMESPACE_BEGIN


enum SortFlag {
	Sort_Name = 0x01,
	Sort_Suffix = 0x02,
	Sort_Size = 0x04,
	Sort_TextPerms = 0x08,
	Sort_DigitPerms = 0x10,
	Sort_Owner = 0x20,
	Sort_Group = 0x40,
	Sort_Modified = 0x80,

	Sort_None = 0,
	Sort_All = 0xFF
};

typedef QFlags<SortFlag> SortFlags;


KRW_NAMESPACE_END


#endif //__GLOBAL_H__
