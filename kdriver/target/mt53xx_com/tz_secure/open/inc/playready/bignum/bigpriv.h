/**@@@+++@@@@******************************************************************
**
** Microsoft PlayReady
** Copyright (c) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/
#ifndef __BIGPRIV_H
#define __BIGPRIV_H
/*
      File bigpriv.h.   Version 20 September, 2002

      Declarations accessible to bignum library but invisible to application.
      Also see fieldpriv.h
*/

#include "bignum.h"


#if defined (_MSC_VER)

    #pragma warning(disable: 4146 4514)  // TBD
         /* 4146 -- unary minus operator applied
            to unsigned type, result still unsigned.
            4514 -- unreferenced inline function
          */
#endif



/*
     Some routines allow the caller to supply temps, but
     accept a NULL argument to say "Allocate them yourself!".
     possible_digit_allocate assists in doing the allocate
     if the caller passed NULL.
*/

typedef struct digit_tempinfo_t {
    digit_t *address;       // Address supplied by user.
                            // Updated to specify address
                            // to use for temporaries.
    CRYPTO_DWORD nelmt;         // Number of digit_t elements needed
    CRYPTO_BOOL  need_to_free;  // Should be set FALSE by application.
                            // Changed to TRUE if a free is required.
} digit_tempinfo_t;

CRYPTO_BOOL possible_digit_allocate(digit_tempinfo_t*,
                                    const CRYPTO_CHAR*);

CRYPTO_BOOL modmul_choices1(mp_modulus_t*, CRYPTO_INT*);

CRYPTO_BOOL modmul_from_right_default( 
            const digit_t      *a,
            const digit_t      *b,
            digit_t      *c,
            const mp_modulus_t *pmodulo,   
            digit_t      *temps );

CRYPTO_BOOL modmul_from_right_default_modulo8( 
            const digit_t      *a,
            const digit_t      *b,
            digit_t      *c,
            const mp_modulus_t *pmodulo,   
            digit_t      *temps );


#endif  //  __BIGPRIV_H
