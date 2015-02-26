/**@@@+++@@@@******************************************************************
**
** Microsoft PlayReady
** Copyright (c) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/
/*
      File fieldpriv.h.   Version 20 September 2002

      Declarations for field routines which the application program
      doesn't need to know about.
*/

#ifndef __FIELDPRIV_H
#define __FIELDPRIV_H 1
#include "bigpriv.h"
#include "field.h"


CRYPTO_BOOL Kequaler_default (const digit_t *, const digit_t *,
                                     const CRYPTO_DWORD, const field_desc_t*, struct bigctx_t *f_pBigCtx);

CRYPTO_BOOL Kfdesc_initialize(field_desc_t*, const CRYPTO_DWORD, struct bigctx_t *f_pBigCtx);

CRYPTO_BOOL Kiszeroer_default(const digit_t *, const CRYPTO_DWORD,
                                     const field_desc_t*, struct bigctx_t *f_pBigCtx);

CRYPTO_BOOL Kzeroizer_default(digit_t *, const CRYPTO_DWORD,
                                     const field_desc_t*, struct bigctx_t *f_pBigCtx);



#endif /* __FIELDPRIV_H */
