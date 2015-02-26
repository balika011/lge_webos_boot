/**@@@+++@@@@******************************************************************
**
** Microsoft (r) PlayReady (r)
** Copyright (c) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/

/******************************************************************************
**
** This file declares a minimal set of revocation functions for inside the secure crypto core.
** OEMs should not need to change this file in any way (except to get it to compile properly).
** If the device supports a secure crypto core, this file should compile ONLY for that core.
**
*******************************************************************************
*/

#ifndef __OEM_HAL_DEVICE_CERTS_H__
#define __OEM_HAL_DEVICE_CERTS_H__

#include <oemhalbasic.h>
#include <oemsha256.h>
#include <oemcryptoctx.h>

ENTER_PK_NAMESPACE;

DRM_RESULT _ProcessRevocationInfo(
    __inout_ecount( 1 )                     DRM_CRYPTO_CONTEXT         *f_pCryptoContext,
    __in_bcount( f_cbRevocationInfo ) const DRM_BYTE                   *f_pbRevocationInfo,
    __in                                    DRM_DWORD                   f_cbRevocationInfo,
    __in                                    OEM_HAL_KEY_REGISTER_INDEX  f_indexSigningKey );

DRM_RESULT _ProcessCRL(
    __inout_ecount( 1 )                     DRM_CRYPTO_CONTEXT         *f_pCryptoContext,
    __in_bcount( f_cbCRL )            const DRM_BYTE                   *f_pbCRL,
    __in                                    DRM_DWORD                   f_cbCRL,
    __in                                    OEM_HAL_KEY_REGISTER_INDEX  f_indexSigningKey );

DRM_NO_INLINE DRM_RESULT _CheckCertificateRevocation(
    __in_ecount(f_cDigests)           const DRM_SHA256_Digest                   *f_pCertDigests,
    __in                                    DRM_DWORD                            f_cDigests,
    __out_ecount( 1 )                       OEM_HAL_PUBKEY_VERIFICATION_STATE   *f_peVerificationState ) DRM_NO_INLINE_ATTRIBUTE;

DRM_RESULT _CheckValidPublicKey(
    __in                                    OEM_HAL_KEY_REGISTER_INDEX  f_indexPublicKey );


EXIT_PK_NAMESPACE;

#endif  /* __OEM_HAL_DEVICE_CERTS_H__ */

