/**@@@+++@@@@******************************************************************
**
** Microsoft (r) PlayReady (r)
** Copyright (c) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/

/******************************************************************************
**
** This file declares a minimal set of key derivation functions for inside the secure crypto core.
** OEMs should not need to change this file in any way (except to get it to compile properly).
** If the device supports a secure crypto core, this file should compile ONLY for that core.
**
*******************************************************************************
*/

#ifndef __OEMDERIVEDKEYS_H__
#define __OEMDERIVEDKEYS_H__

#include <oemhal.h>
#include <drmxmr.h>

ENTER_PK_NAMESPACE;

DRM_RESULT _DecryptContentKey_Derived(
    __in_bcount( DRM_AES_KEYSIZE_128 )              const DRM_BYTE                      *f_pbSecondaryKey,
    __in                                            const DRM_XMR_UPLINKX               *f_poUplinkX,
    __in_opt                                        const DRM_KID                       *f_pKID,
    __in_ecount_opt( DRM_AES_CK_CHECKSUM_LENGTH )   const DRM_BYTE                      *f_pbChecksumData,
    __inout_bcount( DRM_AES_KEYSIZE_128_X2 )              DRM_BYTE                      *f_pbKey );

DRM_RESULT _CalculateKeys_Derived(
    __in_bcount( DRM_AES_KEYSIZE_128 )  const DRM_BYTE              *f_pbRootContentKey,
    __in                                const DRM_XMR_AUX_KEY       *f_poAuxKey,
    __inout                                   DRM_XMR_UPLINKX       *f_poUplinkX );

EXIT_PK_NAMESPACE;

#endif /* __OEMDERIVEDKEYS_H__ */
