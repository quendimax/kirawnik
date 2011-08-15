#ifndef __GLOBAL_H__
#define __GLOBAL_H__


enum HeaderFlags {
	HF_Name = 0x01,
	HF_Suffix = 0x02,
	HF_Size = 0x04,
	HF_TextPerms = 0x08,
	HF_DigitPerms = 0x10,
	HF_Owner = 0x20,
	HF_Group = 0x40,
	HF_Modified = 0x80
};


#endif //__GLOBAL_H__
