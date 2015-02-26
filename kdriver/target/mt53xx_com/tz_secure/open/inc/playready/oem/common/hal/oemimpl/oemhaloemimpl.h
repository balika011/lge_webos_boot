/**@@@+++@@@@******************************************************************
**
** Microsoft (r) PlayReady (r)
** Copyright (c) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/

/******************************************************************************
**
** This file declares the interface between the CoreAPI portion of the secure crypto core
**      (which the OEM should not need to change [except to get it to compile properly])
**      and the OEM-hardware-specific portion of the secure crypto core.
** If the device supports a secure crypto core:
**      This file should declare the interface for code the OEM needs to re-implement
**      from scratch with OEM-hardware-specific functionality.  The types defined in this
**      file are reference types only and are likely not representative of
**      any realistic OEM-hardware-specific types.
** If the device does not support a secure crypto core:
**      OEMs should not need to change this file in any way (except to get it to compile properly).
**
** OEM_MANDATORY:
** If the device supports a secure crypto core, all functions in this file MUST be implemented by the OEM.
**
*******************************************************************************
*/
#ifndef __OEM_HALOEMIMPL_H__
#define __OEM_HALOEMIMPL_H__

#include <oemhalbasic.h>
#include <oemeccp256.h>
#include <oemsha256.h>
#include <oemaeskey.h>
#include <oemcipher.h>

#include <drmlicense.h>
#include <drmxmr.h>
#include <drmrevocationtypes.h>
#include <drmkeyfiletypes.h>
#include <drmbcert.h>

ENTER_PK_NAMESPACE;

/******************************************************************************
**
** This section deals with revocation.
**
******************************************************************************/
DRM_API DRM_RESULT DRM_CALL Oem_HalOemImpl_ReadStoredRIV(
    __out                                           DRM_DWORD                           *f_pdwStoredRIV );

DRM_API DRM_RESULT DRM_CALL Oem_HalOemImpl_WriteStoredRIV(
    __in                                            DRM_DWORD                            f_dwStoredRIV );

DRM_API DRM_RESULT DRM_CALL Oem_HalOemImpl_ReadStoredRequiredCRLVersionFromRevInfo(
    __out                                           DRM_DWORD                           *f_pdwStoredRequiredCRLVersionFromRevInfo );

DRM_API DRM_RESULT DRM_CALL Oem_HalOemImpl_WriteStoredRequiredCRLVersionFromRevInfo(
    __in                                            DRM_DWORD                            f_dwStoredRequiredCRLVersionFromRevInfo );

DRM_API DRM_RESULT DRM_CALL Oem_HalOemImpl_ReadStoredCRLVersion(
    __out                                           DRM_DWORD                           *f_pdwStoredCRLVersion );

DRM_API DRM_RESULT DRM_CALL Oem_HalOemImpl_WriteStoredCRLVersion(
    __in                                            DRM_DWORD                            f_dwStoredCRLVersion );

DRM_API DRM_RESULT DRM_CALL Oem_HalOemImpl_ReadStoredCRLEntryCount(
    __out                                           DRM_DWORD                           *f_pdwStoredCRLEntryCount );

DRM_API DRM_RESULT DRM_CALL Oem_HalOemImpl_ReadStoredCRLEntry(
    __in                                            DRM_DWORD                            f_dwEntryIndex,
    __out                                           DRM_RevocationEntry                 *f_pCRLEntry );

DRM_API DRM_RESULT DRM_CALL Oem_HalOemImpl_WriteStoredCRLEntries(
    __in                                            DRM_DWORD                            f_cCRLEntries,
    __in_ecount_opt(f_cCRLEntries)                  DRM_RevocationEntry                 *f_pCRLEntries );

/******************************************************************************
**
** This section deals with key registers.
**
******************************************************************************/
typedef enum
{
    eOEM_HAL_KEY_REGISTER_STATE_UNALLOCATED = 0,
    eOEM_HAL_KEY_REGISTER_STATE_ALLOCATED   = 1,
    eOEM_HAL_KEY_REGISTER_STATE_PRELOADED   = 2,
} OEM_HAL_KEY_REGISTER_STATE;

typedef struct __tagOEM_HAL_ECC256_KEY_PAIR
{
    PRIVKEY_P256                 privateKey;
    PUBKEY_P256                  publicKey;
    DRM_BOOL                     fPublicKeyPresent;
} OEM_HAL_ECC256_KEY_PAIR;

typedef struct __tagOEM_HAL_EXPANDED_AES_KEY
{
    DRM_AES_KEY                  rgExpandedKey;
    DRM_BOOL                     fExpanded;
} OEM_HAL_EXPANDED_AES_KEY;

typedef struct __tagOEM_HAL_AES_KEY_PAIR
{
    DRM_BYTE                     rgbKeys[2][__CB_DECL( DRM_AES_KEYSIZE_128 )];
    OEM_HAL_EXPANDED_AES_KEY     rgExpandedKeys[2];
    DRM_DWORD                    dwSecurityLevelRequiredToWrapThisKey;
} OEM_HAL_AES_KEY_PAIR;

typedef struct __tagOEM_HAL_DERIVED_ROOT_CHECKSUM
{
    DRM_BYTE                     rgbChecksumData[__CB_DECL( DRM_MAX_CHECKSUM )];
    DRM_KID                      KID;
} OEM_HAL_DERIVED_ROOT_CHECKSUM;

typedef struct __tagOEM_HAL_DERIVED_ROOT_KEY
{
    OEM_HAL_DERIVED_ROOT_CHECKSUM checksumInfo;
    DRM_BYTE                     rgbSecondaryKey[__CB_DECL( DRM_AES_KEYSIZE_128 )];
    DRM_XMR_AUX_KEY              AuxKey;

    /*
    ** Only a single DRM_XMR_AUX_KEY_ENTRY is declared. However, during allocation of the
    ** OEM_HAL_DERIVED_ROOT_KEY structure, sufficient memory must be allocated after this
    ** to hold AuxKey.cEntries elements

    */
    DRM_XMR_AUX_KEY_ENTRY        rgAuxKeyEntries[1];
} OEM_HAL_DERIVED_ROOT_KEY;

typedef struct __tagOEM_HAL_AES_ROOT_KEY_PAIR
{
    OEM_HAL_AES_KEY_PAIR         keyPair;
    OEM_HAL_DERIVED_ROOT_KEY    *pDerivedKey;
} OEM_HAL_AES_ROOT_KEY_PAIR;

typedef struct __tagOEM_HAL_PUBLIC_KEY_VALIDATION_INFO
{
    OEM_HAL_PUBKEY_VERIFICATION_STATE eVerificationState;
    DRM_DWORD                         cDigests;
    DRM_SHA256_Digest                 certificateDigests[DRM_BCERT_MAX_CERTS_PER_CHAIN];
} OEM_HAL_PUBLIC_KEY_VALIDATION_INFO;

typedef struct __tagOEM_HAL_ECC256_PUBLIC_KEY
{
    PUBKEY_P256                             key;
    DRM_DWORD                               dwSecurityLevel;
    DRM_DWORD                               dwKeyUsageBitmap;
    DRM_BOOL                                fRootKey;
    OEM_HAL_PUBLIC_KEY_VALIDATION_INFO      validationInfo;
} OEM_HAL_ECC256_PUBLIC_KEY;

typedef struct __tagOEM_HAL_AES_KEY
{
    DRM_BYTE                            rgbKey[__CB_DECL( DRM_AES_KEYSIZE_128 )];
    OEM_HAL_EXPANDED_AES_KEY            expandedKey;
} OEM_HAL_AES_KEY;

/*
** A legacy key buffer must be able to contain the AES CI key, an RC4 cipherkey and a size byte
*/
#define OEM_HAL_LEGACY_KEY_BUFFER_SIZE ( DRM_AES_KEYSIZE_128 + DRMCIPHERKEYLEN_RC4 + 1 )

typedef struct __tagOEM_HAL_LEGACY_KEY
{
    DRM_BYTE                 rgbKeyBuffer[__CB_DECL( OEM_HAL_LEGACY_KEY_BUFFER_SIZE )];
    DRM_DWORD                dwKeyLength;
    DRM_WORD                 wLegacyCryptoType;
    OEM_HAL_EXPANDED_AES_KEY integrityKey;
} OEM_HAL_LEGACY_KEY;

typedef struct __tagOEM_HAL_PRELOADED_KEY_ID
{
    OEM_HAL_KEY_REGISTER_INDEX  index;
    OEM_HAL_KEY_TYPE            ekeyType;
    DRM_GUID                    id;
} OEM_HAL_PRELOADED_KEY_ID;

typedef DRM_OBFUS_FIXED_ALIGN struct __tagOEM_HAL_REGISTER_STATE_TABLE
{
    const DRM_LONG              cState;
    OEM_HAL_KEY_REGISTER_STATE *pfState;
} OEM_HAL_REGISTER_STATE_TABLE;

typedef struct __tagOEM_HAL_ENCRYPTION_CONTEXT_INTERNAL
{
    DRM_BOOL                       fInUse;
    OEM_HAL_SAMPLE_ENCRYPTION_MODE eSampleEncryptionMode;
    OEM_HAL_KEY_TYPE               contentKeyType;
    OEM_HAL_KEY_REGISTER_INDEX     contentKeyIndex;
    DRM_AES_COUNTER_MODE_CONTEXT   counterContext;
    DRM_BOOL                       fIvSet;
} OEM_HAL_ENCRYPTION_CONTEXT_INTERNAL;

#define OEM_HAL_KEY_PAIR_CI_INDEX 0
#define OEM_HAL_KEY_PAIR_CK_INDEX 1

typedef enum
{
    eOEM_HAL_KEYDATA_TYPE_INVALID           = 0,
    eOEM_HAL_KEYDATA_TYPE_DRM_ENCRYPTED_KEY = 1,
    eOEM_HAL_KEYDATA_TYPE_ENCRYPTED_BYTES   = 2,
    eOEM_HAL_KEYDATA_TYPE_XMR_CERT          = 3,
    eOEM_HAL_KEYDATA_TYPE_XMR_LICENSE       = 4,

#if DRM_TEST_EXPOSE_TEST_FUNCTIONS
    eOEM_HAL_KEYDATA_TYPE_TEST_CLEAR_BYTES  = 253,
#endif  /* DRM_TEST_EXPOSE_TEST_FUNCTIONS */
} OEM_HAL_KEYDATA_TYPE;

typedef enum
{
    eOEM_HAL_KEYREGISTER_TYPE_INVALID          = 0,
    eOEM_HAL_KEYREGISTER_TYPE_P256_PUBLIC      = 1,
    eOEM_HAL_KEYREGISTER_TYPE_P256_KEYPAIR     = 2,
    eOEM_HAL_KEYREGISTER_TYPE_AES128           = 3,
    eOEM_HAL_KEYREGISTER_TYPE_AES128_PAIR      = 4,
    eOEM_HAL_KEYREGISTER_TYPE_AES128_ROOT_PAIR = 5,
    eOEM_HAL_KEYREGISTER_TYPE_LEGACY           = 6,
} OEM_HAL_KEYREGISTER_TYPE;

typedef enum
{
    eOEM_HAL_CRYPTO_TYPE_DEFAULT                = 0,
    eOEM_HAL_CRYPTO_TYPE_DERIVED_SYMMETRIC_KEY  = 1,
    eOEM_HAL_CRYPTO_TYPE_AES_KEY_PAIR           = 2,
    eOEM_HAL_CRYPTO_TYPE_TWO_STAGE_ECB          = 3,
    eOEM_HAL_CRYPTO_TYPE_CHAINED_LICENSE_ECC256 = 4,
    eOEM_HAL_CRYPTO_TYPE_SIMPLE_LICENSE_ECC256  = 5,
    eOEM_HAL_CRYPTO_TYPE_AES_ECB                = 6,
    eOEM_HAL_CRYPTO_TYPE_AES_CBC                = 7,
    eOEM_HAL_CRYPTO_TYPE_AES_PADDED_KEY         = 8,
    eOEM_HAL_CRYPTO_TYPE_ECC256_PUBLIC_KEY      = 9,
    eOEM_HAL_CRYPTO_TYPE_ECC256_PRIVATE_KEY     = 10,
    eOEM_HAL_CRYPTO_TYPE_PASSTHROUGH            = 255,
} OEM_HAL_CRYPTO_TYPE;

typedef struct __tagOEM_HAL_RAW_KEY_REGISTER
{
    OEM_HAL_KEYREGISTER_TYPE eType;

    union
    {
        OEM_HAL_ECC256_KEY_PAIR   *pEcc256Pair;
        OEM_HAL_ECC256_PUBLIC_KEY *pEcc256Public;
        OEM_HAL_AES_KEY           *pAes128;
        OEM_HAL_AES_KEY_PAIR      *pAes128Pair;
        OEM_HAL_AES_ROOT_KEY_PAIR *pAes128RootPair;
        OEM_HAL_LEGACY_KEY        *pLegacy;
    } keyBuffer;
} OEM_HAL_RAW_KEY_REGISTER;

typedef struct __tagOEM_HAL_UNWRAP_CONTEXT
{
    OEM_HAL_KEYDATA_TYPE     eKeyDataType;
    OEM_HAL_CRYPTO_TYPE      eDecryptionType;
    OEM_HAL_RAW_KEY_REGISTER wrappingKey;
    OEM_HAL_RAW_KEY_REGISTER targetKey;
} OEM_HAL_UNWRAP_CONTEXT;

typedef struct __tagOEM_HAL_WRAP_CONTEXT
{
    OEM_HAL_CRYPTO_TYPE      eEncryptionType;
    OEM_HAL_RAW_KEY_REGISTER wrappingKey;
    OEM_HAL_RAW_KEY_REGISTER targetKey;
} OEM_HAL_WRAP_CONTEXT;

DRM_API DRM_RESULT DRM_CALL Oem_HalOemImpl_VerifyHalInitialized();

DRM_API DRM_RESULT DRM_CALL Oem_HalOemImpl_VerifyValidKeyRegister(
    __in                                            OEM_HAL_KEY_TYPE                     f_eType,
    __in                                            OEM_HAL_KEY_REGISTER_INDEX           f_keyRegisterIndex );

DRM_API DRM_VOID DRM_CALL Oem_HalOemImpl_ZeroKeyRegister(
    __in                                            OEM_HAL_KEY_TYPE                     f_eType,
    __in                                            OEM_HAL_KEY_REGISTER_INDEX           f_keyRegisterIndex );

DRM_API DRM_RESULT DRM_CALL Oem_HalOemImpl_AllocateDerivedRootKeyInfo(
    __inout_ecount(1)                               OEM_HAL_AES_ROOT_KEY_PAIR           *f_pRootKeyRegister,
    __in_ecount(1)                            const DRM_XMR_AUX_KEY                     *f_pAuxKey );

DRM_API DRM_RESULT DRM_CALL Oem_HalOemImpl_VerifyOMAC1Signature(
    __in_bcount( f_cbData )                   const DRM_BYTE                            *f_rgbData,
    __in                                            DRM_DWORD                            f_cbData,
    __in_bcount( f_cbSignature )              const DRM_BYTE                            *f_rgbSignature,
    __in                                            DRM_DWORD                            f_cbSignature,
    __in                                            OEM_HAL_KEY_TYPE                     f_eKeyType,
    __in                                            OEM_HAL_KEY_REGISTER_INDEX           f_indexKey );

DRM_API DRM_RESULT DRM_CALL Oem_HalOemImpl_CreateOMAC1Signature(
    __in_bcount( f_cbData )                   const DRM_BYTE                            *f_rgbData,
    __in                                            DRM_DWORD                            f_cbData,
    __out_bcount_opt( *f_pcbSignature )             DRM_BYTE                            *f_rgbSignature,
    __inout                                         DRM_DWORD                           *f_pcbSignature,
    __in                                            OEM_HAL_KEY_TYPE                     f_eKeyType,
    __in                                            OEM_HAL_KEY_REGISTER_INDEX           f_indexKey );

DRM_API DRM_RESULT DRM_CALL Oem_HalOemImpl_EncryptDataBlock(
    __in_bcount( f_cbData )                   const DRM_BYTE                            *f_rgbBlock,
    __in                                            DRM_DWORD                            f_cbBlock,
    __out_bcount_opt( *f_pcbEncryptedBlock )        DRM_BYTE                            *f_rgbEncryptedBlock,
    __inout                                         DRM_DWORD                           *f_pcbEncryptedBlock,
    __in                                            OEM_HAL_KEY_TYPE                     f_eKeyType,
    __in                                            OEM_HAL_KEY_REGISTER_INDEX           f_indexMessageEncryptKey );

DRM_API DRM_RESULT DRM_CALL Oem_HalOemImpl_GenerateEcc256KeyPair(
    __inout_ecount( 1 )                             DRM_CRYPTO_CONTEXT                  *f_pContext,
    __in                                            OEM_HAL_KEY_TYPE                     f_eKeyType,
    __in                                            OEM_HAL_KEY_REGISTER_INDEX           f_indexKey );

DRM_API DRM_RESULT DRM_CALL Oem_HalOemImpl_GenerateAes128Key(
    __in                                            DRM_VOID                            *f_pOEMContext,
    __in                                            OEM_HAL_KEY_TYPE                     f_eKeyType,
    __in                                            OEM_HAL_KEY_REGISTER_INDEX           f_indexKey );

DRM_API DRM_RESULT DRM_CALL Oem_HalOemImpl_GenerateAes128KeyPair(
    __in                                            DRM_VOID                            *f_pOEMContext,
    __inout_ecount( 1 )                             DRM_CRYPTO_CONTEXT                  *f_pCryptoContext,
    __in                                            OEM_HAL_KEY_TYPE                     f_eKeyType,
    __in                                            OEM_HAL_KEY_REGISTER_INDEX           f_indexKey );

DRM_API DRM_RESULT DRM_CALL Oem_HalOemImpl_GenerateAes128NewLicenseKeyPair(
    __in                                            DRM_VOID                            *f_pOEMContext,
    __inout_ecount( 1 )                             DRM_CRYPTO_CONTEXT                  *f_pCryptoContext,
    __in                                            OEM_HAL_KEY_TYPE                     f_eKeyType,
    __in                                            OEM_HAL_KEY_REGISTER_INDEX           f_indexKey,
    __in                                            DRM_DWORD                            f_dwRequestedSecurityLevel );

DRM_API DRM_RESULT DRM_CALL Oem_HalOemImpl_AllocateKeyRegister(
    __in                                            OEM_HAL_KEY_TYPE                     f_eKeyType,
    __in                                            DRM_BOOL                             f_fPreloaded,
    __out_ecount( 1 )                               OEM_HAL_KEY_REGISTER_INDEX          *f_pKeyRegisterIndex );

DRM_API DRM_RESULT DRM_CALL Oem_HalOemImpl_EncryptSampleData(
    __in                                            DRM_VOID                            *f_pOEMContext,
    __in                                            OEM_ENCRYPTION_HANDLE                f_hEncryptionContext,
    __out_ecount_opt(1)                             DRM_UINT64                          *f_pqwInitializationVector,
    __out_ecount_opt(1)                             OEM_HAL_SAMPLE_METADATA             *f_pMetadata,
    __in_opt                                        OEM_OPAQUE_BUFFER_HANDLE             f_hInBuf,
    __in_opt                                        OEM_OPAQUE_BUFFER_HANDLE             f_hOutBuf,
    __in                                            DRM_DWORD                            f_cbData,
    __inout_bcount_opt( f_cbData )                  DRM_BYTE                            *f_pbData );

PREFAST_PUSH_DISABLE_EXPLAINED(__WARNING_NONCONST_PARAM_25004, "Prefast Noise: Real implementation will require non-const parameters.")
DRM_API DRM_RESULT DRM_CALL Oem_HalOemImpl_QuerySampleMetadata(
    __in                                            OEM_ENCRYPTION_HANDLE                f_hEncryptionContext,
    __out_ecount(1)                                 OEM_HAL_SAMPLE_METADATA             *f_pMetadata );
PREFAST_POP /* __WARNING_NONCONST_PARAM_25004 */

DRM_API DRM_RESULT DRM_CALL Oem_HalOemImpl_DestroySampleEncryptionContext(
    __in                                            OEM_ENCRYPTION_HANDLE                f_hEncryptionContext );

DRM_API DRM_RESULT DRM_CALL Oem_HalOemImpl_FreeRegister(
    __in                                            OEM_HAL_KEY_TYPE                     f_eKeyType,
    __in                                            OEM_HAL_KEY_REGISTER_INDEX           f_keyRegisterIndex );

DRM_API DRM_RESULT DRM_CALL Oem_HalOemImpl_RegisterCount(
    __in                                            OEM_HAL_KEY_TYPE                     f_eKeyType,
    __out_ecount(1)                                 DRM_DWORD                           *f_pdwTotalRegisters,
    __out_ecount_opt(1)                             DRM_DWORD                           *f_pdwAllocatedRegisters );

DRM_API DRM_RESULT DRM_CALL Oem_HalOemImpl_LookupPreloadedKey(
    __in                                            OEM_HAL_KEY_TYPE                     f_eType,
    __in_ecount( 1 )                          const DRM_GUID                            *f_pKeyId,
    __out_ecount( 1 )                               OEM_HAL_KEY_REGISTER_INDEX          *f_pIndexKey );

DRM_API DRM_RESULT DRM_CALL Oem_HalOemImpl_VerifyPreloadedKeyInfoSlotIsAvailable();

DRM_API DRM_RESULT DRM_CALL Oem_HalOemImpl_SetAesKeyfileExpandedKey(
    __in                                            OEM_HAL_KEY_REGISTER_INDEX           f_keyRegisterIndex,
    __in                                      const DRM_AES_KEY                         *f_pExpandedKey );

DRM_API DRM_RESULT DRM_CALL Oem_HalOemImpl_SetModelSigningKey(
    __in                                            OEM_HAL_KEY_REGISTER_INDEX           f_keyRegisterIndex,
    __in                                      const PRIVKEY_P256                        *f_pModelSigningKey );

DRM_API DRM_RESULT DRM_CALL Oem_HalOemImpl_SetEcc256PublicKeyRoot(
    __in                                            OEM_HAL_KEY_REGISTER_INDEX           f_keyRegisterIndex,
    __in                                      const PUBKEY_P256                         *f_pEcc256PublicKey,
    __in                                            DRM_DWORD                            f_dwKeyUsageBitmap );

DRM_API DRM_RESULT DRM_CALL Oem_HalOemImpl_SavePreloadedKeyInfo(
    __in                                            OEM_HAL_KEY_TYPE                     f_eType,
    __in_ecount( 1 )                          const DRM_GUID                            *f_pKeyId,
    __in                                            OEM_HAL_KEY_REGISTER_INDEX           f_indexKey );

DRM_API DRM_RESULT DRM_CALL Oem_HalOemImpl_Initialize();

DRM_API DRM_RESULT DRM_CALL Oem_HalOemImpl_Deinitialize();

DRM_API DRM_RESULT DRM_CALL Oem_HalOemImpl_VerifyMessageSignature(
    __in_bcount( f_cbMessage )                const DRM_BYTE                            *f_rgbMessage,
    __in                                            DRM_DWORD                            f_cbMessage,
    __in_bcount( f_cbSignature )              const DRM_BYTE                            *f_rgbSignature,
    __in                                            DRM_DWORD                            f_cbSignature,
    __in                                            OEM_HAL_HASH_TYPE                    f_eHashType,
    __in                                            OEM_HAL_SIGNATURE_SCHEME             f_eSignatureScheme,
    __in                                            OEM_HAL_KEY_REGISTER_INDEX           f_indexIntegrityKey );

DRM_API DRM_RESULT DRM_CALL Oem_HalOemImpl_CreateMessageSignature(
    __inout_ecount( 1 )                             DRM_CRYPTO_CONTEXT                  *f_pContext,
    __in_bcount( f_cbMessage )                const DRM_BYTE                            *f_rgbMessage,
    __in                                            DRM_DWORD                            f_cbMessage,
    __out_bcount_opt( *f_pcbSignature )             DRM_BYTE                            *f_rgbSignature,
    __inout                                         DRM_DWORD                           *f_pcbSignature,
    __in                                            OEM_HAL_HASH_TYPE                    f_eHashType,
    __in                                            OEM_HAL_SIGNATURE_SCHEME             f_eSignatureScheme,
    __in                                            OEM_HAL_KEY_TYPE                     f_eKeyType,
    __in                                            OEM_HAL_KEY_REGISTER_INDEX           f_indexIntegrityKey );

DRM_API DRM_RESULT DRM_CALL Oem_HalOemImpl_DecryptContent(
    __inout_bcount( f_cbData )                      DRM_BYTE                            *f_pbData,
    __in                                            DRM_DWORD                            f_cbData,
    __in                                            OEM_HAL_KEY_TYPE                     f_eKeyType,
    __in                                            OEM_HAL_KEY_REGISTER_INDEX           f_indexContentKey,
    __in                                            DRM_UINT64                           f_qwInitializationVector,
    __in                                            DRM_UINT64                           f_qwSampleByteOffset );

PREFAST_PUSH_DISABLE_EXPLAINED(__WARNING_NONCONST_PARAM_25004, "Prefast Noise: Real implementation will require non-const parameters.")
DRM_API DRM_RESULT DRM_CALL Oem_HalOemImpl_DecryptContentOpaque(
    __in                                            OEM_OPAQUE_BUFFER_HANDLE             f_hInBuf,
    __in                                            OEM_OPAQUE_BUFFER_HANDLE             f_hOutBuf,
    __in                                            DRM_DWORD                            f_cbData,
    __in                                            OEM_HAL_KEY_TYPE                     f_eKeyType,
    __in                                            OEM_HAL_KEY_REGISTER_INDEX           f_indexContentKey,
    __in                                            DRM_UINT64                           f_qwInitializationVector,
    __in                                            DRM_UINT64                           f_qwSampleByteOffset );
PREFAST_POP /* __WARNING_NONCONST_PARAM_25004 */

DRM_API DRM_RESULT DRM_CALL Oem_HalOemImpl_CreateSampleEncryptionContext(
    __in                                            OEM_HAL_KEY_TYPE                     f_eKeyType,
    __in                                            OEM_HAL_KEY_REGISTER_INDEX           f_indexKey,
    __in                                            OEM_HAL_SAMPLE_ENCRYPTION_MODE       f_eSampleEncryptionMode,
    __out_ecount(1)                                 OEM_ENCRYPTION_HANDLE               *f_phEncryptionContext );

DRM_API DRM_RESULT DRM_CALL Oem_HalOemImpl_GetEcc256PublicKeyUsageBitmap(
    __in                                            OEM_HAL_KEY_REGISTER_INDEX           f_keyRegisterIndex,
    __out_ecount(1)                                 DRM_DWORD                           *f_pdwKeyUsageBitmap );

DRM_API DRM_RESULT DRM_CALL Oem_HalOemImpl_GetEcc256PublicKeyCertDigests(
    __in                                                            OEM_HAL_KEY_REGISTER_INDEX           f_keyRegisterIndex,
    __out_ecount_part(DRM_BCERT_MAX_CERTS_PER_CHAIN,*f_pcDigests)   DRM_SHA256_Digest                    f_rgCertDigests[DRM_BCERT_MAX_CERTS_PER_CHAIN],
    __out                                                           DRM_DWORD                           *f_pcDigests );

DRM_API DRM_RESULT DRM_CALL Oem_HalOemImpl_SetEcc256PublicKeyVerificationState(
    __in                                            OEM_HAL_KEY_REGISTER_INDEX           f_keyRegisterIndex,
    __in                                            OEM_HAL_PUBKEY_VERIFICATION_STATE    f_eVerificationState );

DRM_API DRM_RESULT DRM_CALL Oem_HalOemImpl_GetEcc256PublicKeyVerificationState(
    __in                                            OEM_HAL_KEY_REGISTER_INDEX           f_keyRegisterIndex,
    __in                                            OEM_HAL_PUBKEY_VERIFICATION_STATE   *f_peVerificationState );

DRM_API DRM_RESULT DRM_CALL Oem_HalOemImpl_ECDSA_Verify_P256(
    __inout_ecount( 1 )                             DRM_CRYPTO_CONTEXT                  *f_pCryptoContext,
    __in                                            OEM_HAL_KEY_REGISTER_INDEX           f_indexSigningKey,
    __in_ecount( f_cbMessageLen )            const  DRM_BYTE                             f_rgbMessage[],
    __in                                     const  DRM_DWORD                            f_cbMessageLen,
    __in                                     const  SIGNATURE_P256                      *f_pSignature );

/*
** Oem_HalOemImpl_InitializeUnwrapContext_* functions
*/
DRM_API DRM_RESULT DRM_CALL Oem_HalOemImpl_InitializeUnwrapContext(
    __in                                            OEM_HAL_KEY_TYPE                     f_eKeyType,
    __in                                            OEM_HAL_KEY_REGISTER_INDEX           f_indexKey,
    __in                                            OEM_HAL_KEY_TYPE                     f_eWrappingKeyType,
    __in                                            OEM_HAL_KEY_REGISTER_INDEX           f_indexWrappingKey,
    __out_ecount( 1 )                               OEM_HAL_UNWRAP_CONTEXT              *f_pUnwrapContext,
    __in_bcount_opt( f_cbParameterData )      const DRM_BYTE                            *f_pbParameterData,
    __in                                            DRM_DWORD                            f_cbParameterData );

/*
** Oem_HalOemImpl_InitializeWrapContext_* functions
*/
DRM_API DRM_RESULT DRM_CALL Oem_HalOemImpl_InitializeWrapContext(
    __in                                            OEM_HAL_KEY_TYPE                     f_eKeyType,
    __in                                            OEM_HAL_KEY_REGISTER_INDEX           f_indexKey,
    __in                                            OEM_HAL_KEY_TYPE                     f_eWrappingKeyType,
    __in                                            OEM_HAL_KEY_REGISTER_INDEX           f_indexWrappingKey,
    __out_ecount( 1 )                               OEM_HAL_WRAP_CONTEXT                *f_pWrapContext );

/*
** Oem_HalOemImpl_WrapOrUnwrap* functions
*/
DRM_API DRM_RESULT DRM_CALL Oem_HalOemImpl_WrapOrUnwrapWithSymmetricCipherRaw(
    __in                                             DRM_BOOL                            f_fEncrypt,
    __in_opt                                   const OEM_HAL_UNWRAP_CONTEXT             *f_pUnwrapContext,
    __in_opt                                   const OEM_HAL_WRAP_CONTEXT               *f_pWrapContext,
    __in_bcount( f_cbData )                    const DRM_BYTE                           *f_pbDataIn,
    __out_bcount( f_cbData )                         DRM_BYTE                           *f_pbDataOut,
    __in                                             DRM_DWORD                           f_cbData );

/*
** Oem_HalOemImpl_Unwrap* functions
*/
DRM_API DRM_RESULT DRM_CALL Oem_HalOemImpl_UnwrapContextGetMetaData(
    __in                                      const OEM_HAL_UNWRAP_CONTEXT              *f_pUnwrapContext,
    __out_ecount_opt( 1 )                           OEM_HAL_KEYDATA_TYPE                *f_peKeyDataType,
    __out_ecount_opt( 1 )                           OEM_HAL_CRYPTO_TYPE                 *f_peDecryptionType,
    __out_ecount_opt( 1 )                           OEM_HAL_KEYREGISTER_TYPE            *f_peWrappingKeyType,
    __out_ecount_opt( 1 )                           OEM_HAL_KEYREGISTER_TYPE            *f_peTargetKeyType );

DRM_API DRM_RESULT DRM_CALL Oem_HalOemImpl_UnwrapEncryptedKey(
    __in                                            OEM_HAL_KEY_TYPE                     f_eKeyType,
    __in                                      const OEM_HAL_UNWRAP_CONTEXT              *f_pUnwrapContext,
    __in                                      const DRM_ENCRYPTED_KEY                   *f_pEncryptedKey );

DRM_API DRM_RESULT DRM_CALL Oem_HalOemImpl_UnwrapWithSymmetricCipher(
    __in                                            OEM_HAL_UNWRAP_CONTEXT              *f_pUnwrapContext,
    __in_bcount( f_cbWrappedKeyData )         const DRM_BYTE                            *f_pbWrappedKeyData,
    __in                                            DRM_DWORD                            f_cbWrappedKeyData );

DRM_API DRM_RESULT DRM_CALL Oem_HalOemImpl_UnwrapAesKeyPairWithEcc256PrivKey(
    __inout_ecount( 1 )                             DRM_CRYPTO_CONTEXT                  *f_pCryptoContext,
    __in                                            OEM_HAL_UNWRAP_CONTEXT              *f_pUnwrapContext,
    __in_bcount( f_cbWrappedKeyData )         const DRM_BYTE                            *f_pbWrappedKeyData,
    __in                                            DRM_DWORD                            f_cbWrappedKeyData );

DRM_API DRM_RESULT DRM_CALL Oem_HalOemImpl_UnwrapXmrAesKeyPairWithEcc256PrivKey(
    __inout_ecount( 1 )                             DRM_CRYPTO_CONTEXT                  *f_pCryptoContext,
    __in                                      const OEM_HAL_UNWRAP_CONTEXT              *f_pUnwrapContext,
    __in_ecount( SIZEOF( CIPHERTEXT_P256 ) )  const DRM_BYTE                            *f_pbEncKeys,
    __out_ecount( SIZEOF( PLAINTEXT_P256 ) )        DRM_BYTE                            *f_pbDecKeys );

DRM_API DRM_RESULT DRM_CALL Oem_HalOemImpl_UnwrapEcc256WithAesKeyPair(
    __in                                            OEM_HAL_UNWRAP_CONTEXT              *f_pUnwrapContext,
    __in_bcount( f_cbWrappedKeyData )         const DRM_BYTE                            *f_pbWrappedKeyData,
    __in                                            DRM_DWORD                            f_cbWrappedKeyData );

DRM_API DRM_RESULT DRM_CALL Oem_HalOemImpl_UnwrapAesPaddedKeyWithEcc256PrivKey(
    __inout_ecount( 1 )                             DRM_CRYPTO_CONTEXT                  *f_pCryptoContext,
    __in                                            OEM_HAL_UNWRAP_CONTEXT              *f_pUnwrapContext,
    __in_bcount( f_cbWrappedKeyData )         const DRM_BYTE                            *f_pbWrappedKeyData,
    __in                                            DRM_DWORD                            f_cbWrappedKeyData );

DRM_API DRM_RESULT DRM_CALL Oem_HalOemImpl_UnwrapAesKeyWithAesEcb(
    __in                                            OEM_HAL_UNWRAP_CONTEXT              *f_pUnwrapContext,
    __in_bcount( f_cbWrappedKeyData )         const DRM_BYTE                            *f_pbWrappedKeyData,
    __in                                            DRM_DWORD                            f_cbWrappedKeyData );

DRM_API DRM_RESULT DRM_CALL Oem_HalOemImpl_UnwrapXmrAesKeyPair(
    __inout                                         OEM_HAL_UNWRAP_CONTEXT              *f_pUnwrapContext,
    __in_ecount( DRM_AES_KEYSIZE_128_X2 )     const DRM_BYTE                            *f_pbDecKeys,
    __in                                            DRM_WORD                             f_wSecurityLevel );

DRM_API DRM_RESULT DRM_CALL Oem_HalOemImpl_UnwrapXmrLegacyKey(
    __inout                                         OEM_HAL_UNWRAP_CONTEXT              *f_pUnwrapContext,
    __in                                            DRM_WORD                             f_wLegacyCryptoType,
    __in_ecount( f_cbDecKeys )                const DRM_BYTE                            *f_pbDecKeys,
    __in                                            DRM_DWORD                            f_cbDecKeys );

DRM_API DRM_RESULT DRM_CALL Oem_HalOemImpl_UnwrapAuxKeyMaterial(
    __inout_ecount( 1 )                             OEM_HAL_UNWRAP_CONTEXT              *f_pUnwrapContext,
    __in_ecount( 1 )                          const DRM_XMR_LICENSE                     *f_pXmrLicense,
    __in_ecount( DRM_MAX_CHECKSUM )           const DRM_BYTE                            *f_pbChecksumData,
    __in                                      const DRM_KID                             *f_pKID );

DRM_API DRM_RESULT DRM_CALL Oem_HalOemImpl_UnwrapGetDerivedAuxKey(
    __in_ecount( 1 )                          const OEM_HAL_UNWRAP_CONTEXT              *f_pUnwrapContext,
    __out_ecount( 1 )                               DRM_XMR_AUX_KEY                     *f_poAuxKey );

DRM_API DRM_RESULT DRM_CALL Oem_HalOemImpl_UnwrapGetRootCK(
    __in_ecount( 1 )                          const OEM_HAL_UNWRAP_CONTEXT              *f_pUnwrapContext,
    __out_ecount( DRM_AES_KEYSIZE_128 )             DRM_BYTE                            *f_pbRootCK );

DRM_API DRM_RESULT DRM_CALL Oem_HalOemImpl_UnwrapGetKeyDerivationData(
    __in_ecount( 1 )                          const OEM_HAL_UNWRAP_CONTEXT              *f_pUnwrapContext,
    __out_ecount( DRM_AES_KEYSIZE_128 )             DRM_BYTE                            *f_pbSecondaryKey,
    __out_ecount_opt( 1 )                           DRM_KID                             *f_pKID,
    __out_ecount_opt( DRM_AES_CK_CHECKSUM_LENGTH )  DRM_BYTE                            *f_pbChecksumData );

DRM_API DRM_RESULT DRM_CALL Oem_HalOemImpl_UnwrapLeafContentKey(
    __in_ecount( 1 )                          const OEM_HAL_UNWRAP_CONTEXT              *f_pUnwrapContext,
    __out_ecount( DRM_AES_KEYSIZE_128_X2 )          DRM_BYTE                            *f_pbDecKeys );

DRM_API DRM_RESULT DRM_CALL Oem_HalOemImpl_UnwrapGetWrappingEcc256PublicKeyData(
    __in_ecount( 1 )                          const OEM_HAL_UNWRAP_CONTEXT              *f_pUnwrapContext,
    __out_ecount_opt( 1 )                           PUBKEY_P256                         *f_pKey,
    __out_ecount_opt( 1 )                           DRM_DWORD                           *f_pdwSecurityLevel,
    __out_ecount_opt( 1 )                           DRM_DWORD                           *f_pdwKeyUsageBitmap,
    __out_ecount_opt( 1 )                           DRM_BOOL                            *f_pfRootKey );

DRM_API DRM_RESULT DRM_CALL Oem_HalOemImpl_UnwrapSetTargetEcc256PublicKeyData(
    __inout_ecount( 1 )                             OEM_HAL_UNWRAP_CONTEXT              *f_pUnwrapContext,
    __in_ecount( 1 )                          const PUBKEY_P256                         *f_pKey,
    __in                                            DRM_DWORD                            f_dwSecurityLevel,
    __in                                            DRM_DWORD                            f_dwKeyUsageBitmap,
    __in_ecount( 1 )                          const DRM_SHA256_Digest                   *f_pDigest,
    __in                                            OEM_HAL_PUBKEY_VERIFICATION_STATE    f_eVerificationState );

/*
** Oem_HalOemImpl_Wrap* functions
*/
DRM_API DRM_RESULT DRM_CALL Oem_HalOemImpl_WrapGetEncryptionType(
    __in                                      const OEM_HAL_WRAP_CONTEXT                *f_pWrapContext,
    __out_ecount(1)                                 OEM_HAL_CRYPTO_TYPE                 *f_peEncryptionType );

DRM_API DRM_RESULT DRM_CALL Oem_HalOemImpl_WrapWithSymmetricCipher(
    __in                                            OEM_HAL_WRAP_CONTEXT                *f_pWrapContext,
    __out_bcount_opt( *f_pcbWrappedKey )            DRM_BYTE                            *f_pbWrappedKey,
    __inout_ecount(1)                               DRM_DWORD                           *f_pcbWrappedKey );

DRM_API DRM_RESULT DRM_CALL Oem_HalOemImpl_WrapWithPassThrough(
    __in                                            OEM_HAL_WRAP_CONTEXT                *f_pWrapContext,
    __out_bcount_opt( *f_pcbWrappedKey )            DRM_BYTE                            *f_pbWrappedKey,
    __inout_ecount(1)                               DRM_DWORD                           *f_pcbWrappedKey );

DRM_API DRM_RESULT DRM_CALL Oem_HalOemImpl_WrapEncryptedKey(
    __in                                            DRM_VOID                            *f_pOEMContext,
    __in                                            OEM_HAL_KEY_TYPE                     f_eKeyType,
    __in                                      const OEM_HAL_WRAP_CONTEXT                *f_pWrapContext,
    __out_ecount(1)                                 DRM_ENCRYPTED_KEY                   *f_pEncryptedKey );

DRM_API DRM_RESULT DRM_CALL Oem_HalOemImpl_WrapWithAesEcb(
    __in                                            OEM_HAL_WRAP_CONTEXT                *f_pWrapContext,
    __out_bcount_opt( *f_pcbWrappedKey )            DRM_BYTE                            *f_pbWrappedKey,
    __inout_ecount(1)                               DRM_DWORD                           *f_pcbWrappedKey );

DRM_API DRM_RESULT DRM_CALL Oem_HalOemImpl_WrapWithPublicKey(
    __inout_ecount( 1 )                             DRM_CRYPTO_CONTEXT                  *f_pCryptoContext,
    __in                                      const OEM_HAL_WRAP_CONTEXT                *f_pWrapContext,
    __out_bcount_opt( *f_pcbWrappedKey )            DRM_BYTE                            *f_pbWrappedKey,
    __inout_ecount(1)                               DRM_DWORD                           *f_pcbWrappedKey );

EXIT_PK_NAMESPACE;

#endif  /* __OEM_HALOEMIMPL_H__ */

