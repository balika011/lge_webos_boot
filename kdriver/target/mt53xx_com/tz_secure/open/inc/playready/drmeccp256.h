/**@@@+++@@@@******************************************************************
**
** Microsoft PlayReady
** Copyright (c) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/
#ifndef __ECC_P256_H__
#define __ECC_P256_H__

#include "bignum/ecurve.h"


/*
** P256 integer size defines
*/
#define SHA256_DIGEST_SIZE_IN_BYTES  32

#define ECC_P256_INTEGER_SIZE_IN_BITS      ( 256 )
#define ECC_P256_INTEGER_SIZE_IN_BYTES     ( 32 )
#define ECC_P256_INTEGER_SIZE_IN_DIGITS    ( BITS_TO_DIGITS( ECC_P256_INTEGER_SIZE_IN_BITS ) )
#define ECC_P256_INTEGER_SIZE_IN_ECAFFINES ( ECC_P256_INTEGER_SIZE_IN_BYTES / sizeof( digit_t ) )
#define ECC_P256_PRIVKEY_SIZE_IN_BITS      ( ECC_P256_INTEGER_SIZE_IN_BITS ) /* privkeys are 256bit integers */
#define ECC_P256_PRIVKEY_SIZE_IN_BYTES     ( ECC_P256_INTEGER_SIZE_IN_BYTES )

/*
** P256 point size defines
*/
#define ECC_P256_POINT_SIZE_IN_BITS        ( 2 * ECC_P256_INTEGER_SIZE_IN_BITS )
#define ECC_P256_POINT_SIZE_IN_BYTES       ( 2 * ECC_P256_INTEGER_SIZE_IN_BYTES )
#define ECC_P256_POINT_SIZE_IN_DIGITS      ( BITS_TO_DIGITS( ECC_P256_POINT_SIZE_IN_BITS ) )
#define ECC_P256_POINT_SIZE_IN_ECAFFINES   ( 2 * ( ECC_P256_INTEGER_SIZE_IN_BYTES / sizeof( digit_t ) ) ) 
#define ECC_P256_PUBKEY_SIZE_IN_BITS       ( ECC_P256_POINT_SIZE_IN_BITS ) /* pubkeys are EC Points */


/*
** P256 integer size defines
*/
#define ECC_P256_CIPHERTEXT_SIZE_IN_BYTES  ( 2 * ECC_P256_POINT_SIZE_IN_BYTES )
#define ECC_P256_PLAINTEXT_SIZE_IN_BYTES   ( ECC_P256_INTEGER_SIZE_IN_BYTES )
#define ECDSA_P256_SIGNATURE_SIZE_IN_BYTES ( 2 * ECC_P256_INTEGER_SIZE_IN_BYTES )

#define ECC_POINT_ADDITION    ( ( CRYPTO_INT ) 1 )
#define ECC_POINT_SUBTRACTION ( ( CRYPTO_INT ) -1 )

/*
** Supplied temps needed for certain P256 EC functions. Note these are upper bounds,
** some operations may not require as large of temps, this is a potential optimization.
*/
#define ECC_P256_LNGRED2          ( ( ECC_P256_INTEGER_SIZE_IN_DIGITS + 1 ) / 2 )
#define ECC_P256_NDIGTEMPS        ( ( 19 * ECC_P256_INTEGER_SIZE_IN_DIGITS ) + 6 )
#define ECC_P256_VERIFY_TEMPS     ( ECC_P256_NDIGTEMPS )
#define ECC_P256_SIGN_TEMPS       ( ECC_P256_NDIGTEMPS )
#define ECC_P256_ENCRYPT_TEMPS    ( ECC_P256_NDIGTEMPS )
#define ECC_P256_POINTMUL_TEMPS   ( ECC_P256_NDIGTEMPS )
#define ECC_P256_POINTADD_TEMPS   ( ECC_P256_NDIGTEMPS )
#define ECC_P256_INTEGERADD_TEMPS ( ECC_P256_NDIGTEMPS )
#define ECC_P256_DECRYPT_TEMPS    ( ECC_P256_NDIGTEMPS )
#define ECC_P256_GENKEYPAIR_TEMPS ( ECC_P256_NDIGTEMPS )

/*
** Struct Defines for ECC P256 and the AES XMR struct that will be encrypted and decrypted by it.
*/
typedef struct __tagCIPHERTEXT_P256_2POINTS
{
    CRYPTO_BYTE m_rgbC1[ __CB_DECL( ECC_P256_POINT_SIZE_IN_BYTES ) ];
    CRYPTO_BYTE m_rgbC2[ __CB_DECL( ECC_P256_POINT_SIZE_IN_BYTES ) ];
} CIPHERTEXT_P256_2POINTS;

typedef struct __tagPUBKEY_P256
{
    CRYPTO_BYTE m_rgbPubkey[ __CB_DECL( ECC_P256_POINT_SIZE_IN_BYTES ) ];
} PUBKEY_P256;

typedef struct __tagSIGNATURE_P256
{
    CRYPTO_BYTE m_rgbSignature[ __CB_DECL( ECC_P256_POINT_SIZE_IN_BYTES ) ];
} SIGNATURE_P256;

/*
** Function Protos
*/

     
CRYPTO_RESULT CRYPTO_ECC_GenerateHMACKey_P256(
    UINT8   *f_pKeys, 
    struct bigctx_t  *f_pBigCtx );

CRYPTO_RESULT CRYPTO_ECC_CanMapToPoint_P256(
    const  digit_t   f_rgdNumber[], 
    struct bigctx_t *f_pBigCtx );

CRYPTO_RESULT CRYPTO_ECC_MapX2PointP256(
    digit_t          *f_pX,
    digit_t          *f_pY,
    digit_t           f_rgdSuppliedTemps[],
    struct bigctx_t         *f_pBigCtx
);

CRYPTO_RESULT CRYPTO_ECC_Decrypt_P256(
    const  UINT8    *f_pPrivkey,
    const  UINT8    *f_pCiphertext,
    UINT8  *f_pPlaintext) ;

CRYPTO_RESULT CRYPTO_ECC_Encrypt_P256( 
    const  UINT8      *f_pPubkey,
    const  UINT8   *f_pPlaintext,
    UINT8  *f_pCiphertext,
    struct bigctx_t         *f_pBigCtx );

CRYPTO_RESULT CRYPTO_ECC_GenKeyPair_P256( 
    UINT8  *f_pPubKey,
    UINT8 *f_pPrivKey);

CRYPTO_RESULT CRYPTO_ECC_GenKeyPairRestrictedPriv_P256( 
    UINT8  *f_pPubKey,
    UINT8 *f_pPrivKey);

CRYPTO_RESULT CRYPTO_ECDSA_Sign_P256(
    const  UINT8                       f_rgbMessage[],
    const  CRYPTO_DWORD        f_cbMessageLen,
    const  UINT8                      *f_pPrivkey,
    UINT8                                *f_pSignature );

CRYPTO_RESULT CRYPTO_ECDSA_Verify_P256(
    const  UINT8         f_rgbMessage[],
    const  CRYPTO_DWORD        f_cbMessageLen,
    const  UINT8     *f_pPubkey,
    const  UINT8  *f_pSignature,
    struct bigctx_t        *f_pBigCtx );

CRYPTO_RESULT Convert_BigEndianBytesToDigits(
    const UINT8  f_rgbInBytes[],
    digit_t   f_rgdOutDigits[],
    const CRYPTO_DWORD f_cBits );

CRYPTO_RESULT Convert_DigitsToBigEndianBytes(
    const digit_t   f_rgdInDigits[],
    CRYPTO_BYTE  f_rgbOutBytes[],
    const CRYPTO_DWORD f_cBits );

CRYPTO_RESULT Convert_P256_PointToPlaintext(
    const  digit_t          f_rgptPoint[],
    const  ecurve_t        *f_pEcurve,
    UINT8  *f_pPlaintext );


CRYPTO_RESULT Convert_P256_BigEndianBytesToPoint(
    const  CRYPTO_BYTE   f_rgbInBytes[], 
    const  ecurve_t  *f_pEcurve,    
    digit_t    f_rgptOutPoint[],
    struct bigctx_t  *f_pBigCtx );

CRYPTO_RESULT Convert_P256_BigEndianBytesToDigits(
    const CRYPTO_BYTE f_rgdInBytes[], 
    digit_t  f_rgbOutDigits[] );

CRYPTO_RESULT Convert_P256_BigEndianBytesToDigitsModOrder(
    const CRYPTO_BYTE  f_rgbBytes[],
    const ecurve_t *f_pEcurve,
    digit_t   f_rgdDigits[] );

CRYPTO_RESULT Convert_P256_ModularIntToDigitsModOrder(
    const  digit_t   f_rgecInModularInt[],
    const  ecurve_t *f_pEcurve,
    digit_t   f_rgbOutDigits[] );

CRYPTO_RESULT Convert_P256_DigitsToBigEndianBytes(
    const digit_t  f_rgdInDigits[], 
    CRYPTO_BYTE f_rgbOutBytes[] );


#endif /* end ifndef __ECC_P256_H__ */

