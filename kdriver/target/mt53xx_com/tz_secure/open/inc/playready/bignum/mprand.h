/**@@@+++@@@@******************************************************************
**
** Microsoft PlayReady
** Copyright (c) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/

/*      File mprand.h */

#ifndef _MPRAND_H
#define _MPRAND_H

#include <tomcrypt.h>
#include "bignum.h"


CRYPTO_BOOL random_bytes(CRYPTO_BYTE*, const CRYPTO_DWORD);
         // Procedure to be supplied by application.



#if !CRYPTO_INLINING_SUPPORTED

CRYPTO_BOOL     random_digits(digit_t*, const CRYPTO_DWORD);

#endif // CRYPTO_INLINING_SUPPORTED


// Following are extern on all platforms

CRYPTO_BOOL random_digit_interval(const digit_t, const digit_t,
                                    digit_t*);
          /* Generate random digit_t or CRYPTO_DWORD in specified closed interval. */

CRYPTO_BOOL random_mod(const digit_t *, digit_t *,
                        const CRYPTO_DWORD);

/*
** Checked stublib expects the function name to be new_random_mod_nonzero
** Temporary fix.
*/
#define random_mod_nonzero new_random_mod_nonzero
CRYPTO_BOOL random_mod_nonzero(const digit_t *, digit_t *,
                                const CRYPTO_DWORD);
                        // Generate random value modulo another value.
                        // random_mod_nonzero generates a nonzero value.


#if CRYPTO_INLINING_SUPPORTED

// Next four are inline, rather than macros, so compiler can issue
// error when array argument has wrong pointer type.

static inline CRYPTO_BOOL random_digits(digit_t dtArray[],
                                      const CRYPTO_DWORD lng)
{    /* Generate random multiple-precision number */
     /* It may have a leading zero. */
#if 0     
    return random_bytes((CRYPTO_BYTE*)dtArray,
                        (CRYPTO_DWORD)lng*sizeof(digit_t),
                        f_pBigCtx);
#endif
                        
  TzGetRandomBytes((UINT8 *)dtArray, (UINT32)lng*sizeof(digit_t));
  return TRUE;
} /* random digits */



#endif  // CRYPTO_INLINING_SUPPORTED


#endif   // _MPRAND_H
