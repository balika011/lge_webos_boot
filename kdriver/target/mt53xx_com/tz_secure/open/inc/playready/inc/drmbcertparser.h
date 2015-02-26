/**@@@+++@@@@******************************************************************
**
** Microsoft (r) PlayReady (r)
** Copyright (c) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/

#ifndef __DRMBCERTPARSER_H__
#define __DRMBCERTPARSER_H__

#include <oemparsers.h>

ENTER_PK_NAMESPACE;

DRM_API DRM_RESULT DRM_CALL DRM_BCert_ParseCertificateChain(
    __in_bcount(f_cbCertData) const DRM_BYTE                      *f_pbCertData,
    __in                      const DRM_DWORD                     f_cbCertData,
    __inout                         DRM_BCERT_VERIFICATIONCONTEXT *f_pVerificationContext );

DRM_API DRM_RESULT DRM_CALL DRM_BCert_GetChainHeader(
    __in_bcount(f_cbCertData) const DRM_BYTE         *f_pbCertData,
    __in                      const DRM_DWORD        f_cbCertData,
    __inout_ecount(1)         DRM_DWORD              *f_bOffset,
    __inout_ecount(1)         DRM_BCERT_CHAIN_HEADER *f_poChain );

DRM_API DRM_NO_INLINE DRM_RESULT DRM_CALL DRM_BCert_GetPublicKeyByUsage(
    __in_bcount(f_cbCertData)           const DRM_BYTE    *f_pbCertData,
    __in                                const DRM_DWORD    f_cbCertData,
    __in                                const DRM_DWORD    f_dwCertIndex,
    __in                                const DRM_DWORD    f_dwKeyUsage,
    __inout_ecount(1)                   PUBKEY_P256       *f_pPubkey,
    __out_opt                           DRM_DWORD         *f_pdwKeyUsageSet,
    __out_ecount_opt(1)                 DRM_DWORD         *f_pbCertStartOffset,
    __out_ecount_opt(1)                 DRM_DWORD         *f_pdwCertKeyIndex ) DRM_NO_INLINE_ATTRIBUTE;

DRM_API DRM_NO_INLINE DRM_RESULT DRM_CALL DRM_BCert_GetPublicKey(
    __in_bcount(f_cbCertData) const DRM_BYTE      *f_pbCertData,
    __in                      const DRM_DWORD     f_cbCertData,
    __in                      const DRM_DWORD     f_dwCertIndex,
    __inout_ecount(1)         PUBKEY_P256         *f_pPubkey ) DRM_NO_INLINE_ATTRIBUTE;

DRM_API DRM_NO_INLINE DRM_RESULT DRM_CALL DRM_BCert_GetSecurityVersion(
    __in_bcount(f_cbCertData) const DRM_BYTE      *f_pbCertData,
    __in                      const DRM_DWORD      f_cbCertData,
    __out_ecount(1)                 DRM_DWORD     *f_pdwSecurityVersion,
    __out_ecount(1)                 DRM_DWORD     *f_pdwPlatformIdentifier ) DRM_NO_INLINE_ATTRIBUTE;

DRM_API DRM_RESULT DRM_CALL DRM_BCert_FindObjectInCertByType(
    __in_bcount(f_cbCertData) const DRM_BYTE    *f_pbCertData,
    __in                      const DRM_DWORD   f_cbCertData,
    __inout_ecount(1)         DRM_DWORD         *f_pbOffset,
    __in                      const DRM_DWORD   f_dwObjectType );


DRM_API DRM_RESULT DRM_CALL DRM_BCert_VerifySignature(
    __in_ecount(__CB_DECL( f_cbCertificate )) const DRM_BYTE *f_pbCertificateBegin,
    __in              DRM_DWORD                f_cbCertificate,
    __in              DRM_DWORD                f_dwSignedDataLength,
    __inout_ecount(1) DRM_BCERT_SIGNATURE_INFO *f_poSignatureInfo,
    __inout_ecount(1) DRM_CRYPTO_CONTEXT       *f_pCryptoCtx,
    __in_opt          DRM_BB_CONTEXT           *f_pcontextBBX,
    __in_opt          DRM_SECSTORE_CONTEXT     *f_pcontextSST,
    __in_opt          DRM_DST                  *f_pDatastore ) DRM_NO_INLINE_ATTRIBUTE;

DRM_API DRM_RESULT DRM_CALL DRM_BCert_GetManufacturerStrings(
    __in_bcount(f_cbCertData) const DRM_BYTE      *f_pbCertData,
    __in                      const DRM_DWORD      f_cbCertData,
    __out_ecount(1)                 DRM_BCERT_MANUFACTURER_STRINGS *f_pManufacturerStrings );

DRM_API DRM_NO_INLINE DRM_RESULT DRM_CALL DRM_BCert_LoadPropertiesCache(
    __in_bcount(f_cbCertData) const DRM_BYTE                             *f_pbCertData,
    __in                      const DRM_DWORD                             f_cbCertData,
    __inout                         DRM_BINARY_DEVICE_CERT_CACHED_VALUES *f_pCachedValues,
    __in_ecount_opt(1)        const DRM_BB_KEYHANDLE_CALLBACKS           *f_pKeyHandleCallbacks,
    __inout_ecount(1)               DRM_BB_CONTEXT                       *f_pBlackboxContext ) DRM_NO_INLINE_ATTRIBUTE;

DRM_API DRM_VOID DRM_CALL DRM_BCert_CleanupPropertiesCache(
   __inout                   DRM_BINARY_DEVICE_CERT_CACHED_VALUES *f_pCachedValues,
   __in_ecount_opt(1)  const DRM_BB_KEYHANDLE_CALLBACKS           *f_pKeyHandleCallbacks,
   __inout_ecount(1)         DRM_BB_CONTEXT                       *f_pBlackboxContext );

DRM_NO_INLINE DRM_RESULT DRM_BCert_GetCertChainInfo(
    __in_bcount(f_cbCertData) const DRM_BYTE          *f_pbCertData,
    __in                      const DRM_DWORD          f_cbCertData,
    __in                            DRM_DWORD          f_bOffset,
    __in                            DRM_DWORD          f_dwNumCertficates,
    __in                            DRM_DWORD          f_idxResolveUpTo,
    __in                            DRM_DWORD          f_dwRequiredLeafType,
    __out                           DRM_DWORD          f_rgdwCertOffsets[DRM_BCERT_MAX_CERTS_PER_CHAIN],
    __out                           DRM_DWORD          f_rgdwKeyIndices[DRM_BCERT_MAX_CERTS_PER_CHAIN],
    __out                           DRM_BCERT_PUBKEY  *f_rgpPublicKeys[DRM_BCERT_MAX_CERTS_PER_CHAIN] );

#define DRM_BCert_GetLeafSecurityLevel(secLevel, chain) \
    NETWORKBYTES_TO_DWORD( (secLevel), (chain), SIZEOF( DRM_BCERT_CHAIN_HEADER )  \
                                              + SIZEOF( DRM_BCERT_HEADER )        \
                                              + SIZEOF( DRM_BCERT_OBJECT_HEADER ) \
                                              + SIZEOF( DRM_BCERT_CERT_ID ) );

EXIT_PK_NAMESPACE;

#endif /* __DRMBCERTPARSER_H__ */

