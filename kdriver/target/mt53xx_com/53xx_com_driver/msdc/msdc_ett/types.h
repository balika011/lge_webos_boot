#ifndef _TYPES_H_
#define _TYPES_H_


typedef signed char         s8;
typedef unsigned char       u8;

typedef signed short        s16;
typedef unsigned short      u16;

typedef signed int          s32;
typedef unsigned int        u32;

typedef signed long long    s64;
typedef unsigned long long  u64;

/* bsd */
typedef unsigned char		u_char;
typedef unsigned short		u_short;
typedef unsigned int		u_int;
typedef unsigned long		u_long;

/* sysv */
typedef unsigned char		unchar;
typedef unsigned char		uchar;
typedef unsigned short		ushort;
typedef unsigned int		uint;
typedef unsigned long		ulong;

typedef unsigned char       uint8;
typedef unsigned short      uint16;
typedef unsigned int        uint32;
typedef signed char         int8;
typedef signed short        int16;
typedef signed int          int32;
#if !(defined(__KERNEL__) && !defined(CC_MTK_PRELOADER))
typedef unsigned char       bool;
#endif

#endif /* _TYPES_H_ */

