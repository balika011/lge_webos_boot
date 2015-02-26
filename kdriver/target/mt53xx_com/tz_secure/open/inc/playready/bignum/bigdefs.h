/**@@@+++@@@@******************************************************************
**
** Microsoft PlayReady
** Copyright (c) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/

#ifndef __BIGDEFS_H__
#define __BIGDEFS_H__


#include "tzcrypto.h"

//       File bigdefs.h     Version 10 February 2004

//       This file declared the fundamental types and constants used
//       within the bignum code.

//           digit_t
//           sdigit_t
//
//        used within the bignum code and headers.
//        We also declare the most fundamental routines for operating on these.
//
//        If you need to perform arithmetic, not just copy data, see bignum.h .

//#include <drmcommon.h>


/*
      Occasionally a struct name is used before the struct itself is
      declared.  The Future_Struct macro avoids a warning message
      with Visual C .NET (2002).
*/
#define Future_Struct(strname) struct strname


#if !CRYPTO_INLINING_SUPPORTED

CRYPTO_DWORD significant_bit_count(const digit_t);
                 /* Has special code on some platforms */

CRYPTO_DWORD mp_significant_bit_count(const digit_t *, const CRYPTO_DWORD);

CRYPTO_DWORD digits_to_dwords(const digit_t *, CRYPTO_DWORD*, const CRYPTO_DWORD );

CRYPTO_DWORD dwords_to_digits(const CRYPTO_DWORD*, digit_t *, const CRYPTO_DWORD );

CRYPTO_VOID mp_extend(const digit_t *, const CRYPTO_DWORD, digit_t *, const CRYPTO_DWORD );

digit_t mp_getbit(const digit_t *, const CRYPTO_DWORD);

CRYPTO_VOID mp_setbit(digit_t *, const CRYPTO_DWORD, const digit_t );

CRYPTO_DWORD significant_digit_count(const digit_t *, const CRYPTO_DWORD);
#endif

#if CRYPTO_INLINING_SUPPORTED
static inline CRYPTO_VOID mp_extend
        (const digit_t  a[],
         const CRYPTO_DWORD lnga,
         digit_t   b[],
         const CRYPTO_DWORD lngb )
/*
        Copy a to b, while changing its length from
        lnga to lngb (zero fill).  Require lngb >= lnga.
*/
{    
    memcpy( b,a,( lnga)*sizeof( digit_t ));
    memset(b + lnga,0, ( lngb - lnga)*sizeof( digit_t ));
}  // mp_extend
/****************************************************************************/
static inline digit_t mp_getbit
        (const digit_t a[],
         const CRYPTO_DWORD ibit)
                /* Extract bit of multiple precision number */
{
    return digit_getbit(a[ibit/RADIX_BITS],  ibit % RADIX_BITS);
}
/****************************************************************************/
static inline CRYPTO_VOID  mp_setbit
        (digit_t   a[],
         const CRYPTO_DWORD ibit,
         const digit_t  new_value)
/*
        Set a bit to 0 or 1,
        when the number is viewed as a bit array.
*/

{
    const CRYPTO_DWORD j       = ibit / RADIX_BITS;
    const CRYPTO_DWORD ishift  = ibit % RADIX_BITS;

    const digit_t mask1 = (DIGIT_ONE &  new_value) << ishift;
    const digit_t mask2 = (DIGIT_ONE & ~new_value) << ishift;
    
    a[j] = (a[j] & ~mask2) | mask1;
} // end mp_setbit
/****************************************************************************/
static inline CRYPTO_DWORD  significant_digit_count
        (const digit_t  a[],
         const CRYPTO_DWORD    lng)
/*
        Return the number of significant digits in a.
        Function value is zero precisely when a == 0.
*/
{
    CRYPTO_DWORD i = lng;

    while (i != 0 && a[i-1] == 0) i--;
    return i;
}  /* significant_digit_count */
/****************************************************************************/
static inline CRYPTO_BOOL digits_to_dwords
        (const digit_t  pdigit[],
         CRYPTO_DWORD     pdword[],
         const CRYPTO_DWORD lng_dwords)
{
    CRYPTO_BOOL OK = TRUE;

#if DWORDS_PER_DIGIT == 1    
    memcpy( (digit_t*)pdword,pdigit,( lng_dwords)*sizeof( digit_t ));
#elif DWORDS_PER_DIGIT == 2
    const CRYPTO_DWORD lng_half = lng_dwords >> 1;
    CRYPTO_DWORD i;
    
    if (IS_ODD(lng_dwords)) {
        pdword[lng_dwords-1] = (CRYPTO_DWORD)pdigit[lng_half];
    }
    for (i = 0; i != lng_half; i++) {
        const digit_t dig = pdigit[i];
        pdword[2*i    ] = (CRYPTO_DWORD)dig;
        pdword[2*i + 1] = (CRYPTO_DWORD)(dig >> DWORD_BITS);
    }
#else
    #error "Unexpected DWORDS_PER_DIGIT"
#endif
    return OK;
}  /* digits_to_dwords */
/****************************************************************************/
static inline CRYPTO_BOOL dwords_to_digits
        (const CRYPTO_DWORD    pdword[],
         digit_t   pdigit[],
         const CRYPTO_DWORD lng_dwords)
{
    CRYPTO_BOOL OK = TRUE;

#if DWORDS_PER_DIGIT == 1    
    memcpy( pdigit,(digit_t*)pdword,( lng_dwords)*sizeof( digit_t ));
#elif DWORDS_PER_DIGIT == 2
    const CRYPTO_DWORD lng_half = lng_dwords >> 1;
    CRYPTO_DWORD i;    

    if (IS_ODD(lng_dwords)) {
        pdigit[lng_half] = (digit_t)pdword[lng_dwords - 1];  // Zero fill
    }
    for (i = 0; i != lng_half; i++) {
        pdigit[i] =    ((digit_t)pdword[2*i+1] << DWORD_BITS)
                     |  (digit_t)pdword[2*i];
    }
#else
    #error "Unexpected DWORDS_PER_DIGIT"
#endif
    return OK;
}  /* dwords_to_digits */

#undef significant_bit_count    // In case a macro version existed
static inline CRYPTO_DWORD significant_bit_count(const digit_t d)
/*
        Compute the number of significant bits in d.
        This is one more than the truncated base 2 logarithm of d.
        significant_bit_count(0)  is undefined.

        For example, significant_bit_count(d) = 8 for 128 <= d <= 255.

        On platforms which have a Count Leading Zeroes or
        similar instruction, UNIFORM_SIGNIFICANT_BIT_COUNT
        should be 1, with special code used.
        For other platforms, use this algorithm.
        The algorithm performs best when the argument is large,
        a feature used by the GCD routines.
*/
{
    digit_t dadj = d | 1;
    CRYPTO_DWORD width = RADIX_BITS;
    CRYPTO_DWORD width_adj;

    while (dadj < (DIGIT_ONE << (RADIX_BITS - 5))) {
        width -= 5;
        dadj <<= 5;
    }
    dadj >>= (RADIX_BITS - 4);     /* From 0 to 15 */
    width_adj = ((CRYPTO_DWORD)0000000011112234 >> dadj) >> (2*dadj);
                     // Shift by 3*dadj, to index into array of octal digits
    return width - (width_adj & 7);
                    // Subtract 5 - significant_bit_count(2*dadj + 1)
} /* significant_bit_count */

static inline CRYPTO_DWORD  mp_significant_bit_count
        (const digit_t  a[],
        const CRYPTO_DWORD  lnga)
/*
        Return the number of significant bits in a, which
        is one more than the truncated base 2 logarithm of a.
        Return 0 if a == 0.
*/
{
    const CRYPTO_DWORD lng_sig = significant_digit_count(a, lnga);
    
    if (lng_sig == 0) {
        return 0;
    } else {
       return    (lng_sig - 1)*RADIX_BITS
               + significant_bit_count(a[lng_sig-1]);
    }
}


/****************************************************************************/
#endif // CRYPTO_INLINING_SUPPORTED

/*
        The application should define the
        following three malloc-like functions.
        Sample definitions appear in bigalloc.h.
*/
#if 0
CRYPTO_VOID* bignum_alloc(const CRYPTO_DWORD cblen, struct bigctx_t *f_pBigCtx);
CRYPTO_VOID  bignum_free(CRYPTO_VOID* pvMem, struct bigctx_t *f_pBigCtx);
#endif

#endif // BIGDEFS_H
