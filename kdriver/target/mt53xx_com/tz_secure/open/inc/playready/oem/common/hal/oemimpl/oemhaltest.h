/**@@@+++@@@@******************************************************************
**
** Microsoft (r) PlayReady (r)
** Copyright (c) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/

/******************************************************************************
**
** This file declares TEST-ONLY functions for the secure crypto core.
** If the device supports a secure crypto core:
**      This file should declare the interface for TEST-ONLY code the OEM needs to re-implement
**      from scratch with OEM-hardware-specific functionality.
** If the device does not support a secure crypto core:
**      OEMs should not need to change this file in any way (except to get it to compile properly).
**
** Notes: This header file is only intended for use by test code - the functions
**        it exposes must not be made available in production code
**
*******************************************************************************
*/


#ifndef __OEMHALTEST_H__
#define __OEMHALTEST_H__

#include <oemhaloemimpl.h>

ENTER_PK_NAMESPACE;

DRM_API DRM_BOOL DRM_CALL OEM_HAL_TEST_CompareKeyMaterial(
    __in                                 OEM_HAL_KEY_TYPE            f_eKeyType,
    __in                                 OEM_HAL_KEY_REGISTER_INDEX  f_keyRegisterIndex,
    __in_bcount( f_cbKeyMaterial ) const DRM_BYTE                   *f_pbKeyMaterial,
    __in                                 DRM_DWORD                   f_cbKeyMaterial );


DRM_API DRM_BOOL DRM_CALL OEM_HAL_TEST_CompareKeys(
    __in                                 OEM_HAL_KEY_TYPE            f_eKeyType,
    __in                                 OEM_HAL_KEY_REGISTER_INDEX  f_keyRegisterIndex1,
    __in                                 OEM_HAL_KEY_REGISTER_INDEX  f_keyRegisterIndex2 );


DRM_API DRM_DWORD DRM_CALL OEM_HAL_TEST_GetUnwrapCount();


DRM_API DRM_RESULT DRM_CALL OEM_HAL_TEST_AddPublicKeyUsage(
    __in OEM_HAL_KEY_TYPE            f_eKeyType,
    __in OEM_HAL_KEY_REGISTER_INDEX  f_keyRegisterIndex,
    __in DRM_DWORD                   f_dwUsageAdd );


DRM_API DRM_VOID DRM_CALL OEM_HAL_TEST_EnableTestWrapUnwrap(
    __in DRM_BOOL f_fEnable );

DRM_API DRM_RESULT DRM_CALL OEM_HAL_TEST_InitTestKeysUnwrap(
    __in                                    OEM_HAL_KEY_TYPE            f_eKeyType,
    __in                                    OEM_HAL_KEY_REGISTER_INDEX  f_indexKey,
    __in                                    OEM_HAL_KEY_TYPE            f_eWrappingKeyType,
    __in                                    OEM_HAL_KEY_REGISTER_INDEX  f_indexWrappingKey,
    __in                                    DRM_DWORD                   f_cbWrappedKeyData,
    __out_ecount( 1 )                       OEM_HAL_UNWRAP_CONTEXT     *f_pUnwrapContext );

DRM_API DRM_RESULT DRM_CALL OEM_HAL_TEST_InjectCleartextKey(
    __in                                    OEM_HAL_UNWRAP_CONTEXT *f_pUnwrapContext,
    __in_bcount( f_cbWrappedKeyData ) const DRM_BYTE               *f_pbWrappedKeyData,
    __in                                    DRM_DWORD               f_cbWrappedKeyData );

DRM_API DRM_RESULT DRM_CALL OEM_HAL_TEST_GenerateEcc256KeyPair(
    __inout_ecount( 1 )                             DRM_CRYPTO_CONTEXT                  *f_pContext,
    __in                                            OEM_HAL_KEY_TYPE                     f_eKeyType,
    __in                                            OEM_HAL_KEY_REGISTER_INDEX           f_indexKey );

DRM_API DRM_RESULT DRM_CALL OEM_HAL_TEST_GenerateAes128Key(
    __in                                            DRM_VOID                            *f_pOEMContext,
    __inout_ecount( 1 )                             DRM_CRYPTO_CONTEXT                  *f_pCryptoContext,
    __in                                            OEM_HAL_KEY_TYPE                     f_eKeyType,
    __in                                            OEM_HAL_KEY_REGISTER_INDEX           f_indexKey );

DRM_API DRM_RESULT DRM_CALL OEM_HAL_TEST_GenerateAes128KeyPair(
    __in                                            DRM_VOID                            *f_pOEMContext,
    __inout_ecount( 1 )                             DRM_CRYPTO_CONTEXT                  *f_pCryptoContext,
    __in                                            OEM_HAL_KEY_TYPE                     f_eKeyType,
    __in                                            OEM_HAL_KEY_REGISTER_INDEX           f_indexKey );

DRM_API DRM_RESULT DRM_CALL OEM_HAL_TEST_OpaqueBufferCreate(
    __out                                           OEM_OPAQUE_BUFFER_HANDLE            *f_phBuf );

DRM_API DRM_RESULT DRM_CALL OEM_HAL_TEST_OpaqueBufferCreateWithData(
    __in_bcount( f_cbData )                   const DRM_BYTE                            *f_pbData,
    __in                                            DRM_DWORD                            f_cbData,
    __out                                           OEM_OPAQUE_BUFFER_HANDLE            *f_phBuf );

DRM_API DRM_RESULT DRM_CALL OEM_HAL_TEST_OpaqueBufferDestroy(
    __out                                           OEM_OPAQUE_BUFFER_HANDLE            *f_phBuf );

PREFAST_PUSH_DISABLE_EXPLAINED(__WARNING_NONCONST_PARAM_25004, "Prefast Noise: f_hBuf already const")
DRM_API DRM_RESULT DRM_CALL OEM_HAL_TEST_OpaqueBufferRead(
    __in                                      const OEM_OPAQUE_BUFFER_HANDLE             f_hBuf,
    __out_bcount( f_cbData )                        DRM_BYTE                            *f_pbData,
    __in                                            DRM_DWORD                            f_cbData );
PREFAST_POP /* __WARNING_NONCONST_PARAM_25004 */

EXIT_PK_NAMESPACE;

#endif /* __OEMHALTEST_H__ */
