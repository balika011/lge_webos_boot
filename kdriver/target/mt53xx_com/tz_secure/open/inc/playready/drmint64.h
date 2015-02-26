/**@@@+++@@@@******************************************************************
**
** Microsoft PlayReady
** Copyright (c) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/

#ifndef __CRYPTOINT64_H__
#define __CRYPTOINT64_H__

//#include <drmnamespace.h>   /* defining proper namespace (if used) */


#if CRYPTO_NO_NATIVE_64_TYPES

/* CRYPTO_INT64 and CRYPTO_UINT64 keeps 2 32 bit values
** val[0] keeps low 32 bit 
** val[1] keeps high 32 bit
** This is valid for both big and little endian CPUs
*/

typedef struct _CRYPTO_INT64 { 
    unsigned long val[2];
} CRYPTO_INT64;

typedef struct _CRYPTO_UINT64 {   
    unsigned long val[2];
} CRYPTO_UINT64;


extern CRYPTO_INT64 CRYPTO_I64Add(const CRYPTO_INT64 a, const CRYPTO_INT64 b);
extern CRYPTO_INT64 CRYPTO_I64Sub(const CRYPTO_INT64 a, const CRYPTO_INT64 b);
extern CRYPTO_INT64 CRYPTO_I64Mul(const CRYPTO_INT64 a, const CRYPTO_INT64 b);
extern CRYPTO_INT64 CRYPTO_I64Div(const CRYPTO_INT64 a, const CRYPTO_INT64 b);
extern CRYPTO_INT64 CRYPTO_I64Mod(const CRYPTO_INT64 a, const CRYPTO_INT64 b);
extern CRYPTO_INT64 CRYPTO_I64And(const CRYPTO_INT64 a, const CRYPTO_INT64 b);
extern CRYPTO_INT64 CRYPTO_I64ShR(const CRYPTO_INT64 a, const int unsigned b);
extern CRYPTO_INT64 CRYPTO_I64ShL(const CRYPTO_INT64 a, const int unsigned b);
extern int       CRYPTO_I64Eql(const CRYPTO_INT64 a, const CRYPTO_INT64 b);
extern int       CRYPTO_I64Les(const CRYPTO_INT64 a, const CRYPTO_INT64 b);
extern CRYPTO_INT64 CRYPTO_I64(const long int b);
extern CRYPTO_INT64 CRYPTO_I64Asgn(const long int a, const long int b);
extern CRYPTO_INT64 CRYPTO_UI2I64(const CRYPTO_UINT64 b);
extern unsigned long int CRYPTO_I64ToUI32(const CRYPTO_INT64 b);

extern CRYPTO_UINT64 CRYPTO_UI64Add(const CRYPTO_UINT64 a, const CRYPTO_UINT64 b);
extern CRYPTO_UINT64 CRYPTO_UI64Sub(const CRYPTO_UINT64 a, const CRYPTO_UINT64 b);
extern CRYPTO_UINT64 CRYPTO_UI64Mul(const CRYPTO_UINT64 a, const CRYPTO_UINT64 b);
extern CRYPTO_UINT64 CRYPTO_UI64Div(const CRYPTO_UINT64 a, const CRYPTO_UINT64 b);
extern CRYPTO_UINT64 CRYPTO_UI64Mod(const CRYPTO_UINT64 a, const CRYPTO_UINT64 b);
extern CRYPTO_UINT64 CRYPTO_UI64And(const CRYPTO_UINT64 a, const CRYPTO_UINT64 b);
extern CRYPTO_UINT64 CRYPTO_UI64ShR(const CRYPTO_UINT64 a, const unsigned int b);
extern CRYPTO_UINT64 CRYPTO_UI64ShL(const CRYPTO_UINT64 a, const unsigned int b);
extern int        CRYPTO_UI64Eql(const CRYPTO_UINT64 a, const CRYPTO_UINT64 b);
extern int        CRYPTO_UI64Les(const CRYPTO_UINT64 a, const CRYPTO_UINT64 b);
extern CRYPTO_UINT64 CRYPTO_UI64(const unsigned long int b);

extern CRYPTO_UINT64 CRYPTO_UI64HL(const unsigned long int a, const unsigned long int b);
extern unsigned long int CRYPTO_UI64High32( CRYPTO_UINT64 a );
extern unsigned long int CRYPTO_UI64Low32( CRYPTO_UINT64 a );

/* Low 32 bit are stored first in 
** CRYPTO_UINT64 structure.
** Thus we put "b" in first place
*/
#define CRYPTO_UI64LITERAL(a,b) { b, a  }
#define CRYPTO_I64LITERAL(a,b) { b, a  }

#define FILETIME_TO_UI64( ft, ui64 ) {ui64=CRYPTO_UI64HL((ft).dwHighDateTime,(ft).dwLowDateTime);}
#define UI64_TO_FILETIME( ui64, ft ) {(ft).dwLowDateTime=CRYPTO_UI64Low32(ui64);(ft).dwHighDateTime=CRYPTO_UI64High32((ui64));}
#define NATIVE64_TO_NONNATIVE64( ui64 ) {ui64=CRYPTO_UI64HL(CRYPTO_UI64Low32(ui64),CRYPTO_UI64High32(ui64));} /* Reverse the high and low */

#else

#if defined (_MSC_VER)

typedef          __int64 CRYPTO_INT64;
typedef unsigned __int64 CRYPTO_UINT64;

#elif defined (__GNUC__)

typedef          long long int CRYPTO_INT64;
typedef unsigned long long int CRYPTO_UINT64;

#endif

#define CRYPTO_I64LITERAL(a,b) (CRYPTO_INT64)(((CRYPTO_INT64)a)<<32|b)
#define CRYPTO_UI64LITERAL(a,b)(CRYPTO_UINT64)(((CRYPTO_UINT64)a)<<32|b)

#define FILETIME_TO_UI64( ft, ui64 ) { (ui64) = *((CRYPTO_UINT64*)&(ft)); } 
#define UI64_TO_FILETIME( ui64, ft ) { (ft)   = *((CRYPTOFILETIME*)&(ui64)); }
#define NATIVE64_TO_NONNATIVE64( ui64 )  

#define CRYPTO_I64Add(a, b) ( (a) + (b) )
#define CRYPTO_I64Sub(a, b) ( (a) - (b) )
#define CRYPTO_I64Mul(a, b) ( (a) * (b) )
#define CRYPTO_I64Div(a, b) ( (a) / (b) )
#define CRYPTO_I64Mod(a, b) ( (a) % (b) )
#define CRYPTO_I64And(a, b) ( (a) & (b) )
#define CRYPTO_I64ShR(a, b) ( (a) >> (b) )
#define CRYPTO_I64ShL(a, b) ( (a) << (b) )
#define CRYPTO_I64Eql(a, b) ( (a) == (b) )
#define CRYPTO_I64Les(a, b) ( (a) < (b) )
#define CRYPTO_I64(b) ( (CRYPTO_INT64) (b) )
#define CRYPTO_I64Asgn(a, b) (((CRYPTO_INT64)(a)<<32) | (b))
#define CRYPTO_UI2I64(b) ((CRYPTO_INT64)(b))

#define CRYPTO_I64ToUI32(b) ((CRYPTO_DWORD)(b))



#define CRYPTO_UI64Add(a, b) ( (a) + (b) )
#define CRYPTO_UI64Sub(a, b) ( (a) - (b) )
#define CRYPTO_UI64Mul(a, b) ( (a) * (b) )
#define CRYPTO_UI64Div(a, b) ( (a) / (b) )
#define CRYPTO_UI64Mod(a, b) ( (a) % (b) )
#define CRYPTO_UI64And(a, b) ( (a) & (b) )
#define CRYPTO_UI64ShR(a, b) ( (a) >> (b) )
#define CRYPTO_UI64ShL(a, b) ( (a) << (b) )
#define CRYPTO_UI64Eql(a, b) ( (a) == (b) )
#define CRYPTO_UI64Les(a, b) ( (a) < (b) )
#define CRYPTO_UI64(b) ( (CRYPTO_UINT64) (b) )

#define CRYPTO_UI64HL(a,b) (((CRYPTO_UINT64)(a)<<32) | (b))
#define CRYPTO_UI64High32(a) ((unsigned long int)(a>>32))
#define CRYPTO_UI64Low32(a) ((unsigned long int)(a&CRYPTO_UI64LITERAL(0,0xFFFFFFFF)))

#endif

/* 
** This conversion is common for both native support of 64 bit number and representation as struct
*/
#define CRYPTO_I2UI64(b) (*((CRYPTO_UINT64*)(&(b))))

#define CRYPTO_UI64Add32(a, b) CRYPTO_UI64Add( CRYPTO_UI64(a), CRYPTO_UI64(b) )


#endif /* __CRYPTOINT64_H__ */
