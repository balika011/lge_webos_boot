#ifndef WRAPPED_PR25_HAL_INTERFACE
#define WRAPPED_PR25_HAL_INTERFACE
#include <drmcompiler.h>
#include "tee_common.h"
#include <drmresults.h>
#include <drmtypes.h>
DRM_BOOL Wrapped_OEM_HAL_IsHalDevCertValidationSupported();
DRM_BOOL Wrapped_OEM_HAL_IsHalDevCertValidationUnsupported();
DRM_API DRM_RESULT DRM_CALL Wrapped_Oem_Hal_AllocateRegister(TEE_COMMON_ARG_T* ptCommonArg);
DRM_API DRM_RESULT DRM_CALL Wrapped_Oem_Hal_FreeRegister(TEE_COMMON_ARG_T* ptCommonArg);
DRM_API DRM_RESULT DRM_CALL Wrapped_Oem_Hal_RegisterCount(TEE_COMMON_ARG_T* ptCommonArg);
DRM_API DRM_RESULT DRM_CALL Wrapped_Oem_Hal_GetPreloadedIndex(TEE_COMMON_ARG_T* ptCommonArg );
DRM_API DRM_RESULT DRM_CALL Wrapped_Oem_Hal_Initialize(TEE_COMMON_ARG_T* ptCommonArg);
DRM_API DRM_RESULT DRM_CALL Wrapped_Oem_Hal_Deinitialize(TEE_COMMON_ARG_T* ptCommonArg);
DRM_API DRM_RESULT DRM_CALL Wrapped_Oem_Hal_VerifyMessageSignature(TEE_COMMON_ARG_T* ptCommonArg);
DRM_API DRM_RESULT DRM_CALL Wrapped_Oem_Hal_CreateMessageSignature(TEE_COMMON_ARG_T* ptCommonArg );
DRM_API DRM_RESULT DRM_CALL Wrapped_Oem_Hal_VerifyOMAC1Signature(TEE_COMMON_ARG_T* ptCommonArg );
DRM_API DRM_RESULT DRM_CALL Wrapped_Oem_Hal_CreateOMAC1Signature(TEE_COMMON_ARG_T* ptCommonArg );
DRM_API DRM_RESULT DRM_CALL Wrapped_Oem_Hal_EncryptDataBlock(TEE_COMMON_ARG_T* ptCommonArg);
DRM_API DRM_BOOL DRM_CALL Wrapped_Oem_Hal_IsDevelopmentPlatform();
DRM_API DRM_RESULT DRM_CALL Wrapped_Oem_Hal_UnwrapKey(TEE_COMMON_ARG_T* ptCommonArg );
DRM_API DRM_RESULT DRM_CALL Wrapped_Oem_Hal_WrapKey(TEE_COMMON_ARG_T* ptCommonArg );
DRM_API DRM_RESULT DRM_CALL Wrapped_Oem_Hal_GenerateKey( TEE_COMMON_ARG_T* ptCommonArg );
DRM_API DRM_RESULT DRM_CALL Wrapped_Oem_Hal_DecryptContent(TEE_COMMON_ARG_T* ptCommonArg);
DRM_API DRM_RESULT DRM_CALL Wrapped_Oem_Hal_DecryptContentOpaque(TEE_COMMON_ARG_T* ptCommonArg );
DRM_API DRM_RESULT DRM_CALL Wrapped_Oem_Hal_CreateSampleEncryptionContext( TEE_COMMON_ARG_T* ptCommonArg );
DRM_API DRM_RESULT DRM_CALL Wrapped_Oem_Hal_EncryptSampleData(TEE_COMMON_ARG_T* ptCommonArg );
DRM_API DRM_RESULT DRM_CALL Wrapped_Oem_Hal_QuerySampleMetadata(TEE_COMMON_ARG_T* ptCommonArg );
DRM_API DRM_RESULT DRM_CALL Wrapped_Oem_Hal_DestroySampleEncryptionContext(TEE_COMMON_ARG_T* ptCommonArg);
DRM_API DRM_RESULT DRM_CALL Wrapped_Oem_Hal_LoadPlayReadyRevocationInfo(TEE_COMMON_ARG_T* ptCommonArg );
DRM_API DRM_RESULT DRM_CALL Wrapped_Oem_Hal_LoadPlayReadyCrl(TEE_COMMON_ARG_T* ptCommonArg);
DRM_API DRM_RESULT DRM_CALL Wrapped_Wrapped_Get_EncryptionType(TEE_COMMON_ARG_T* ptCommonArg );
DRM_API DRM_RESULT DRM_CALL Wrapped_Wrapped_Get_KeyDataType(TEE_COMMON_ARG_T* ptCommonArg );

#endif


