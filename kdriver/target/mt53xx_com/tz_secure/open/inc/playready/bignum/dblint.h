/**@@@+++@@@@******************************************************************
**
** Microsoft PlayReady
** Copyright (c) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/

#ifndef DBLINT_H
#define DBLINT_H 1

#include "../drmint64.h"

/*
        File: dblint.h.  Supplement to bignum.h.  Version 10 December, 2002.

        This file has declarations related to double-precision integers,
        such as typedefs, constants, and primitive operations.

        The CRYPTO_UINT64 type is unsigned and holds twice as many bits
        as a digit_t datum.  If (CRYPTO_NO_NATIVE_64_TYPES = 0), then use the type
        already in the language.  Otherwise (CRYPTO_NO_NATIVE_64_TYPES = 1)
        construct one of our own, using a struct with two digit_t fields.

        Let u, u1, u2 have type digit_t and
        d, d1, d2 have type CRYPTO_UINT64.
        The following primitives are defined,
        whether we use the built-in type or our own type:
                                        
                DPRODUU(u1, u2) -- Product of u1 and u2, as a CRYPTO_UINT64.
                HPRODUU(u1, u2) -- Most significant half of product
                                   of u1 and u2, as a digit_t.                
*/

#if !(CRYPTO_NO_NATIVE_64_TYPES)

#if defined (_MSC_VER)
/*
        A problem in Visual C/C++ 4.0 (x86 version, 1995)
        prevents proper inlining of the DPRODUU function
        if we code it in a straightforward way.  Specifically,
        if we have two nearby references DPRODUU(x, y)
        and DPRODUU(x, z), where one argument (here x) is
        repeated, then the compiler calls library function
        __allmul rather than emit a MUL instruction.

        x86 version of VC 4.1 adds an __emulu function.
        VC 7.0 (2002) does not recognize it in C+ programs.
*/
#if CRYPTO_INLINING_SUPPORTED
static __forceinline CRYPTO_UINT64 DPRODUU(const digit_t u1, const digit_t u2)
{
#if defined(_M_IX86) && !defined(__cplusplus)
        return __emulu(u1, u2);
#else
        return CRYPTO_UI64(u1) * CRYPTO_UI64(u2);
#endif
}
#else // INLINING
#define DPRODUU(u1, u2) (CRYPTO_UI64(u1) * CRYPTO_UI64(u2))
#endif

#else // _MSC_VER

#define DPRODUU(u1, u2) (CRYPTO_UI64(u1) * CRYPTO_UI64(u2))

#endif // _MSC_VER

#define MULTIPLY_ADD1(d1, d2, d3) \
        CRYPTO_UI64Add(DPRODUU(d1, d2), CRYPTO_UI64(d3))
           /* d1*d2 + d3 */

#define MULTIPLY_ADD2(d1, d2, d3, d4) \
        CRYPTO_UI64Add(CRYPTO_UI64Add(DPRODUU(d1, d2), CRYPTO_UI64(d3)), \
                   CRYPTO_UI64(d4))
          /* d1*d2 + d3 + d4 */

#define HPRODUU(u1, u2) CRYPTO_UI64High32(CRYPTO_UI64Mul(CRYPTO_UI64((u1)), CRYPTO_UI64((u2))))

#else  /* CRYPTO_NO_NATIVE_64_TYPES */

/* No native support for 64-bit types */

#if defined( _M_AMD64_ )
    #pragma intrinsic(__umulh)
    #define HPRODUU(u1, u2) __umulh(u1, u2)
#else
    #define HPRODUU(u1, u2) CRYPTO_UI64High32(CRYPTO_UI64Mul(CRYPTO_UI64((u1)), CRYPTO_UI64((u2))))
#endif

#if !CRYPTO_INLINING_SUPPORTED

CRYPTO_UINT64 DPRODUU(const digit_t, const digit_t);
CRYPTO_UINT64 MULTIPLY_ADD1(const digit_t, const digit_t, const digit_t);
CRYPTO_UINT64 MULTIPLY_ADD2(const digit_t, const digit_t,
                                            const digit_t, const digit_t);


#endif // !CRYPTO_INLINING_SUPPORTED

#if CRYPTO_INLINING_SUPPORTED

inline CRYPTO_UINT64 DPRODUU(const digit_t u1, const digit_t u2)
/*
        Multiply two single-precision operands,
        return double precision product.
        This will normally be replaced by an assembly language routine.
        unless the top half of the product (HPRODUU) is available in C.
*/
{
    return CRYPTO_UI64Mul( CRYPTO_UI64( u1 ), CRYPTO_UI64( u2 ) );
}   // end DPRODUU
 


/*
    The MULTIPLY_ADD1. MULTIPLY_ADD2
    functions take single-length (digit_t) operands and
    return double-length (CRYPTO_UINT64) results.
    Overflow is impossible.
*/


inline CRYPTO_UINT64 MULTIPLY_ADD1(const digit_t d1, const digit_t d2,
                                                 const digit_t d3)
{
    return CRYPTO_UI64Add( CRYPTO_UI64Mul( CRYPTO_UI64(d1), CRYPTO_UI64(d2) ), CRYPTO_UI64(d3) );
} // MULTIPLY_ADD1


inline CRYPTO_UINT64 MULTIPLY_ADD2(const digit_t d1, const digit_t d2,
                                                 const digit_t d3, const digit_t d4)
{
    return CRYPTO_UI64Add( CRYPTO_UI64Add( CRYPTO_UI64Mul( CRYPTO_UI64(d1), CRYPTO_UI64(d2) ), CRYPTO_UI64(d3) ), CRYPTO_UI64(d4) );
} // MULTIPLY_ADD2

#endif // CRYPTO_INLINING_SUPPORTED

#endif // CRYPTO_NO_NATIVE_64_TYPES



#endif // DBLINT_H
