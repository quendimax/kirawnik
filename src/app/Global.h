#ifndef __GLOBAL_H__
#define __GLOBAL_H__

#define KRW_NAMESPACE_BEGIN namespace Krw {
#define KRW_NAMESPACE_END }


KRW_NAMESPACE_BEGIN


enum SortingType {
	Sort_Begin = 0,

	Sort_Name = Sort_Begin,
	Sort_Suffix,
	Sort_Size,
	Sort_TextPerms,
	Sort_DigitPerms,
	Sort_Owner,
	Sort_Group,
	Sort_Modified,

	Sort_End,
	Sort_None = Sort_End
};


KRW_NAMESPACE_END


#endif //__GLOBAL_H__
