
#ifndef __CRYPTO_TYPES_H__
#define __CRYPTO_TYPES_H__


/*
#------------------------------------------------------------------------------
# Set this flag to '1' to enable pre-computation of G table to enable speed-up
# in ECC operations at a cost of higher memory footprint
#------------------------------------------------------------------------------
*/
#ifndef CRYPTO_PRECOMPUTE_GTABLE
#define CRYPTO_PRECOMPUTE_GTABLE           0
#endif

#define VERSION_LEN             4

/*
#------------------------------------------------------------------------------
# DRM_USE_ASSEMBLY
# Set to 1 to enable assembly optimized routines where available
#------------------------------------------------------------------------------
*/
#ifndef DRM_USE_ASSEMBLY
#define DRM_USE_ASSEMBLY                0
#endif

#define CRYPTO_INLINING_SUPPORTED  1
/*
** This option specifies that assembly routines will be used, if they exist for the given target
** The RMSDK disables this option for use in managed C++ components
*/
#ifndef CRYPTO_USE_ASSEMBLY
#define CRYPTO_USE_ASSEMBLY 1
#endif

#define digit_getbit(iword, ibit) (((iword) >> (ibit)) & 1)

#define CB_NATIVE_BYTE 1
#ifndef SIZEOF
#define SIZEOF(x) (sizeof(x)*CB_NATIVE_BYTE)
#endif


#define CHAR_BIT            8                /* number of bits in a char    */
typedef UINT16              CRYPTO_WORD;   
typedef UINT32              CRYPTO_DWORD;              /* 4 bytes             2 bytes */
typedef INT32               CRYPTO_LONG;               /* 4 bytes             2 bytes */
typedef UINT32              CRYPTO_UINT;               /* 4 bytes               n/a   */
typedef void      	        CRYPTO_VOID;   
typedef INT32               CRYPTO_INT;                /* 4 bytes               n/a   */
typedef CHAR                CRYPTO_CHAR;               /* 1 byte              1 byte */
typedef INT32               CRYPTO_BOOL;               /* 1 bytes             1 bytes */
typedef UCHAR               CRYPTO_BYTE;

typedef INT32               CRYPTO_RESULT;
typedef CRYPTO_DWORD        digit_t;
typedef CRYPTO_LONG         sdigit_t;

typedef CRYPTO_DWORD        CRYPTO_DWORD_PTR;

#define DIGIT_ONE           ((digit_t)1)
#define DIGIT_ZERO          ((digit_t)0)

#define GET_BYTE(pb,ib)     (pb)[(ib)]
#define PUT_BYTE(pb,ib,b)   (pb)[(ib)]=(b)
/*
**      Test whether a (possibly negative) number is odd or even.
*/
#define IS_EVEN(n) (~(CRYPTO_UINT)(n) & 1)
#define IS_ODD(n) ((CRYPTO_UINT)(n) & 1)

struct bigctx_t;

/*      Multiple-precision data is normally represented
**      in radix 2^RADIX_BITS, with RADIX_BITS bits per word.
**      Here ``word'' means type digit_t.  RADIX_BITS
**      should be 32 for all supported architectures */
#define RADIX_BITS      32
#define RADIX_BYTES     4
#define RADIX_HALF (DIGIT_ONE << (RADIX_BITS - 1))

#define RADIXM1 (DIGIT_ZERO - DIGIT_ONE)

#define DWORD_BITS      32
#define DWORDS_PER_DIGIT (RADIX_BITS/DWORD_BITS)
#define DWORD_LEFT_BIT 0x80000000UL

/* DWORDS_TO_DIGITS(lng_dwords) computes the number of digit_t
** elements required to store an array with -lng_dwords- DWORDs.
** DIGITS_TO_DWORDS converts in the opposite direction. */
#define DWORDS_TO_DIGITS(lng_dwords) \
                ( ((lng_dwords) + DWORDS_PER_DIGIT - 1)/DWORDS_PER_DIGIT)
                
#define BITS_TO_DIGITS(nb) (((nb) + RADIX_BITS - 1)/RADIX_BITS)

#ifndef TRUE
#define TRUE 1
#endif
    
#ifndef FALSE
#define FALSE 0
#endif



/* ============================================================
**
** Standard Success values
**
** ============================================================
*/

#define CRYPTO_SUCCESS                             ((CRYPTO_RESULT)0x00000000L)
#define CRYPTO_S_FALSE                             ((CRYPTO_RESULT)0x00000001L)
#define CRYPTO_S_MORE_DATA                         ((CRYPTO_RESULT)0x00000002L)


#if SIXTEEN_BIT_ADDRESSING
#define CB_NATIVE_BYTE 2
#define __CB_DECL(x) ((x)/CB_NATIVE_BYTE+(((x)%CB_NATIVE_BYTE)?1:0))
#else
#define CB_NATIVE_BYTE 1
#define __CB_DECL(x) (x)
#endif

#if TARGET_LITTLE_ENDIAN
typedef struct _tagCRYPTOFILETIME
{
    CRYPTO_DWORD dwLowDateTime;
    CRYPTO_DWORD dwHighDateTime;
} CRYPTOFILETIME;
#else
typedef struct _tagCRYPTOFILETIME
{
    CRYPTO_DWORD dwHighDateTime;
    CRYPTO_DWORD dwLowDateTime;
} CRYPTOFILETIME;
#endif


CRYPTO_VOID Crypto_Debug_Assert(
     CRYPTO_BOOL f_fAssert, 
     CRYPTO_CHAR *f_assertcmd, 
     CRYPTO_CHAR *f_file, 
     CRYPTO_INT f_line );

#define DRMASSERT(x)     Crypto_Debug_Assert((x), (CRYPTO_CHAR *)#x, (CRYPTO_CHAR *)__FILE__, __LINE__)


#define TRACE(x) 	
#define DRMCASSERT( x )
      
#endif
