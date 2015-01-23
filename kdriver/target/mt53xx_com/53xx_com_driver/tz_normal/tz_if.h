/*----------------------------------------------------------------------------*
 * No Warranty                                                                *
 * Except as may be otherwise agreed to in writing, no warranties of any      *
 * kind, whether express or implied, are given by MTK with respect to any MTK *
 * Deliverables or any use thereof, and MTK Deliverables are provided on an   *
 * "AS IS" basis.  MTK hereby expressly disclaims all such warranties,        *
 * including any implied warranties of merchantability, non-infringement and  *
 * fitness for a particular purpose and any warranties arising out of course  *
 * of performance, course of dealing or usage of trade.  Parties further      *
 * acknowledge that Company may, either presently and/or in the future,       *
 * instruct MTK to assist it in the development and the implementation, in    *
 * accordance with Company's designs, of certain softwares relating to        *
 * Company's product(s) (the "Services").  Except as may be otherwise agreed  *
 * to in writing, no warranties of any kind, whether express or implied, are  *
 * given by MTK with respect to the Services provided, and the Services are   *
 * provided on an "AS IS" basis.  Company further acknowledges that the       *
 * Services may contain errors, that testing is important and Company is      *
 * solely responsible for fully testing the Services and/or derivatives       *
 * thereof before they are used, sublicensed or distributed.  Should there be *
 * any third party action brought against MTK, arising out of or relating to  *
 * the Services, Company agree to fully indemnify and hold MTK harmless.      *
 * If the parties mutually agree to enter into or continue a business         *
 * relationship or other arrangement, the terms and conditions set forth      *
 * hereunder shall remain effective and, unless explicitly stated otherwise,  *
 * shall prevail in the event of a conflict in the terms in any agreements    *
 * entered into between the parties.                                          *
 *---------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * Copyright (c) 2005, MediaTek Inc.
 * All rights reserved.
 *
 * Unauthorized use, practice, perform, copy, distribution, reproduction,
 * or disclosure of this information in whole or in part is prohibited.
 *-----------------------------------------------------------------------------
 *
 * $Author: p4admin $
 * $Date: 2015/01/23 $
 * $RCSfile: dmx.h,v $
 * $Revision: #4 $
 *
 *---------------------------------------------------------------------------*/

/** @file tz_if.h
 *  Interface of trustzone driver
 */


#ifndef TZ_IF_H
#define TZ_IF_H

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#include "x_typedef.h"

//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------
#if defined(CC_MT5882) || defined(CC_MT5890)
  #define TZ_CACHE_LINE_SIZE      64
#else
  #define TZ_CACHE_LINE_SIZE      32
#endif

#define TZ_CACHE_LINE_SIZE_MSK  (TZ_CACHE_LINE_SIZE - 1)
#define TZ_CACHE_ALIGN(x)       (((x) + TZ_CACHE_LINE_SIZE_MSK) & ~TZ_CACHE_LINE_SIZE_MSK)

//-----------------------------------------------------------------------------
// Prototype  of inter-file functions
//-----------------------------------------------------------------------------
EXTERN BOOL TZ_IsTrustzoneEnable(void);

EXTERN void TZ_NWD_BufferStart(UINT32 u4Addr, UINT32 u4Size);
EXTERN void TZ_NWD_BufferEnd(UINT32 u4Addr, UINT32 u4Size);
EXTERN BOOL TZ_SWDMX_MpgParsingPts(void* ptDmxPesMsg);
EXTERN BOOL TZ_SWDMX_DecryptContent(void* pvDmxMmData);
EXTERN BOOL TZ_SWDMX_FreeDecryptBuf(void);
EXTERN BOOL TZ_DMX_Init(void);
EXTERN BOOL TZ_DMX_SetMMKey(UINT32 u4Addr, UINT32 u4Val);
EXTERN BOOL TZ_DMX_GetPicHeader(UINT32 u4Addr, UINT32 u4BufStart, UINT32 u4BufEnd, UINT8 *pu1Buf, UINT32 u4Size);
EXTERN BOOL TZ_DMX_GetSecureVfifo(UINT32 *pu4Addr, UINT32 *pu4Size);
EXTERN BOOL TZ_DMX_GetSecureVfifo2(UINT32 *pu4Addr, UINT32 *pu4Size);
EXTERN BOOL TZ_DMX_ReloadSecureKey(void *prKeyInfo, UINT32 u4Size);
EXTERN BOOL TZ_DMX_FreeSecureKeyIv(UINT8 u1Pidx);
EXTERN BOOL TZ_DMX_SetCryptoKey(void *prKeyInfo, UINT32 u4Size);
EXTERN BOOL TZ_FVR_SetCryptoKey(void *prKeyInfo, UINT32 u4Size);
EXTERN BOOL TZ_DMX_SetMMSecurity(void *prKeyInfo, UINT32 u4Size);

EXTERN BOOL TZ_GCPU_Hw_CmdRun(void *prKernParam, UINT32 u4ParamSize);
EXTERN BOOL TZ_GCPU_Hw_CmdReturn(void *prKernParam, UINT32 u4ParamSize);
EXTERN BOOL TZ_GCPU_EnableIOMMU(void *prKernParam, UINT32 u4ParamSize);
EXTERN BOOL TZ_GCPU_DisableIOMMU(void);
EXTERN BOOL TZ_GCPU_ISR_ClearIRQ(void);
EXTERN BOOL TZ_GCPU_ClearIRQ(void);
EXTERN BOOL TZ_GCPU_ISR_ClearIommuIRQ(void);
EXTERN BOOL TZ_GCPU_ClearIommuIRQ(void);
EXTERN BOOL TZ_GCPU_Hw_SetSlot(void *prKernParam, UINT32 u4ParamSize);
EXTERN BOOL TZ_GCPU_Hw_Reset(void *prKernParam, UINT32 u4ParamSize);
EXTERN BOOL TZ_GCPU_IrqHandle(void *prKernParam, UINT32 u4ParamSize);
EXTERN BOOL TZ_GCPU_IOMMU_IrqHandle(void *prKernParam, UINT32 u4ParamSize);
EXTERN BOOL TZ_GCPU_GetRomCodeChecksum(void *prKernParam, UINT32 u4ParamSize);
EXTERN BOOL TZ_GCPU_Hw_RestoreSecureKeys(void);

EXTERN BOOL TZ_BIM_GetRomBytes(UINT32 u4Offset, UINT8 *pu1Buf, UINT32 u4Size);
EXTERN BOOL TZ_DRM_Divx_GetHwSecretKey0(UINT8 *pu1Buf);
EXTERN BOOL TZ_DRM_Divx_GetHwSecretKey1(UINT8 *pu1Buf);
EXTERN BOOL TZ_BIM_GetSecureID(UINT8 *pu1Buf, UINT32 u4Size);

EXTERN void TZ_RunST(void);
EXTERN void TZ_RunUT(UINT32 u4Count);
EXTERN void TZ_EnableDebug(UINT32 u4Enable,UINT32 sec_start,UINT32 sec_end);

#ifndef CC_NDEBUG
EXTERN BOOL TZ_IO_WRITE32(UINT32 u4Base, UINT32 u4Offset, UINT32 u4Val);
EXTERN BOOL TZ_IO_WRITE16(UINT32 u4Base, UINT32 u4Offset, UINT16 u2Val);
EXTERN BOOL TZ_IO_WRITE8(UINT32 u4Base, UINT32 u4Offset, UINT8 u1Val);
EXTERN BOOL TZ_IO_READ32(UINT32 u4Base, UINT32 u4Offset, UINT32 *pu4Val);
EXTERN BOOL TZ_IO_READ16(UINT32 u4Base, UINT32 u4Offset, UINT16 *pu2Val);
EXTERN BOOL TZ_IO_READ8(UINT32 u4Base, UINT32 u4Offset, UINT8 *pu1Val);
#endif

EXTERN BOOL TZ_HCI_Init(void *prKernParam, UINT32 u4Size);
EXTERN BOOL TZ_HCI_Fin(void *prKernParam, UINT32 u4Size);
EXTERN BOOL TZ_HCI_Lock(void *prKernParam, UINT32 u4Size);
EXTERN BOOL TZ_HCI_Unlock(void *prKernParam, UINT32 u4Size);
EXTERN BOOL TZ_HCI_KeyRefCreate(void *prKernParam, UINT32 u4Size);
EXTERN BOOL TZ_HCI_KeyRefCheck(void *prKernParam, UINT32 u4Size);
EXTERN BOOL TZ_HCI_KeyRefDestroy(void *prKernParam, UINT32 u4Size);
EXTERN BOOL TZ_HCI_DevIdsGetNum(void *prKernParam, UINT32 u4Size);
EXTERN BOOL TZ_HCI_DevIdsGetSize(void *prKernParam, UINT32 u4Size);
EXTERN BOOL TZ_HCI_DevIdsGetName(void *prKernParam, UINT32 u4Size);
EXTERN BOOL TZ_HCI_ImportKey(void *prKernParam, UINT32 u4Size);
EXTERN BOOL TZ_HCI_ExportKeySize(void *prKernParam, UINT32 u4Size);
EXTERN BOOL TZ_HCI_ExportKey(void *prKernParam, UINT32 u4Size);
EXTERN BOOL TZ_HCI_StarfishIdGetSize(void *prKernParam, UINT32 u4Size);
EXTERN BOOL TZ_HCI_StarfishIdGet(void *prKernParam, UINT32 u4Size);
EXTERN BOOL TZ_HCI_KeyDataGetIfStarfishSize(void *prKernParam, UINT32 u4Size);
EXTERN BOOL TZ_HCI_KeyDataGetIfStarfish(void *prKernParam, UINT32 u4Size);
EXTERN BOOL TZ_HCI_ImportStarfishKey(void *prKernParam, UINT32 u4Size);
EXTERN BOOL TZ_HCI_DataSignSize(void *prKernParam, UINT32 u4Size);
EXTERN BOOL TZ_HCI_DataSign(void *prKernParam, UINT32 u4Size);
EXTERN BOOL TZ_HCI_DataDecryptSize(void *prKernParam, UINT32 u4Size);
EXTERN BOOL TZ_HCI_DataDecrypt(void *prKernParam, UINT32 u4Size);
EXTERN BOOL TZ_HCI_StreamDecryptSize(void *prKernParam, UINT32 u4Size);
EXTERN BOOL TZ_HCI_StreamDecrypt(void *prKernParam, UINT32 u4Size);
EXTERN BOOL TZ_HCI_KeyDataEncryptSize(void *prKernParam, UINT32 u4Size);
EXTERN BOOL TZ_HCI_KeyDataEncrypt(void *prKernParam, UINT32 u4Size);
EXTERN BOOL TZ_HCI_HmacCheck(void *prKernParam, UINT32 u4Size);
EXTERN BOOL TZ_HCI_HashCheck(void *prKernParam, UINT32 u4Size);
EXTERN BOOL TZ_HCI_MoveKeySize(void *prKernParam, UINT32 u4Size);
EXTERN BOOL TZ_HCI_MoveKey(void *prKernParam, UINT32 u4Size);
EXTERN BOOL TZ_HCI_FreeMem(void *prKernParam, UINT32 u4Size);
EXTERN BOOL TZ_HCI_PersonalityKeyLoad(void *prKernParam, UINT32 u4Size);
EXTERN BOOL TZ_HCI_PersonalityKeyCheck(void *prKernParam, UINT32 u4Size);
EXTERN BOOL TZ_HCI_ImportSasSize(void *prKernParam, UINT32 u4Size);
EXTERN BOOL TZ_HCI_ImportSas(void *prKernParam, UINT32 u4Size);
EXTERN BOOL TZ_HCI_ImportSasIds(void *prKernParam, UINT32 u4Size);

EXTERN BOOL TZ_SKB_GetInstance(void *prKernParam, UINT32 u4Size);
EXTERN BOOL TZ_SKB_Release(void *prKernParam, UINT32 u4Size);
EXTERN BOOL TZ_SKB_GetInfo(void *prKernParam, UINT32 u4Size);
EXTERN BOOL TZ_SKB_GetProertiesSize(void *prKernParam, UINT32 u4Size);
EXTERN BOOL TZ_SKB_GetProertiesData(void *prKernParam, UINT32 u4Size);
EXTERN BOOL TZ_SKB_DataFromWrapped(void *prKernParam, UINT32 u4Size);
EXTERN BOOL TZ_SKB_DataFromExported(void *prKernParam, UINT32 u4Size);
EXTERN BOOL TZ_SKB_CreateTransform(void *prKernParam, UINT32 u4Size);
EXTERN BOOL TZ_SKB_CreateCipher(void *prKernParam, UINT32 u4Size);
EXTERN BOOL TZ_SKB_SecureData_Release(void *prKernParam, UINT32 u4Size);
EXTERN BOOL TZ_SKB_SecureDataGetInfo(void *prKernParam, UINT32 u4Size);
EXTERN BOOL TZ_SKB_SecureData_Export(void *prKernParam, UINT32 u4Size);
EXTERN BOOL TZ_SKB_SecureData_Derive(void *prKernParam, UINT32 u4Size);
EXTERN BOOL TZ_SKB_Transform_Release(void *prKernParam, UINT32 u4Size);
EXTERN BOOL TZ_SKB_Transform_AddBytes(void *prKernParam, UINT32 u4Size);
EXTERN BOOL TZ_SKB_Transform_AddSecureData(void *prKernParam, UINT32 u4Size);
EXTERN BOOL TZ_SKB_Transform_GetOutput(void *prKernParam, UINT32 u4Size);
EXTERN BOOL TZ_SKB_Cipher_ProcessBuffer(void *prKernParam, UINT32 u4Size);
EXTERN BOOL TZ_SKB_Cipher_Release(void *prKernParam, UINT32 u4Size);

EXTERN BOOL TZ_DRM_WV_SetAssetKey(UINT8 *pu1Key, UINT32 u4KeyLen);
EXTERN BOOL TZ_DRM_WV_DeriveCW(UINT8 *pu1Ecm, UINT32 *pu4Flags);
EXTERN BOOL TZ_DRM_WV_PassKeybox(UINT8 *pu1Keybox, UINT32 u4Size);

EXTERN BOOL TZ_DRM_TZ_SetSecretDataType(void *prKernParam, UINT32 u4Size);
EXTERN BOOL TZ_DRM_TZ_Open(void *prKernParam, UINT32 u4Size);
EXTERN BOOL TZ_DRM_TZ_Close(void *prKernParam, UINT32 u4Size);
EXTERN BOOL TZ_DRM_TZ_DecodePacketsVOD(void *prKernParam, UINT32 u4Size);
EXTERN BOOL TZ_DRM_TZ_LoadSecretDataType(void *prKernParam, UINT32 u4Size);

#ifdef CC_ENABLE_HDCP2
EXTERN BOOL TZ_HDCP2_SetEncLc128(void *prKernParam, UINT32 u4Size);
EXTERN BOOL TZ_HDCP2_SetEncKey(void *prKernParam, UINT32 u4Size);
EXTERN BOOL TZ_HDCP2_GetCertInfo(void *prKernParam, UINT32 u4Size);
EXTERN BOOL TZ_HDCP2_DecryptRSAESOAEP(void *prKernParam, UINT32 u4Size);
EXTERN BOOL TZ_HDCP2_KdKeyDerivation(void *prKernParam, UINT32 u4Size);
EXTERN BOOL TZ_HDCP2_ComputeHprime(void *prKernParam, UINT32 u4Size);
EXTERN BOOL TZ_HDCP2_ComputeLprime(void *prKernParam, UINT32 u4Size);
EXTERN BOOL TZ_HDCP2_ComputeKh(void *prKernParam, UINT32 u4Size);
EXTERN BOOL TZ_HDCP2_EncryptKmUsingKh(void *prKernParam, UINT32 u4Size);
EXTERN BOOL TZ_HDCP2_DecryptKmUsingKh(void *prKernParam, UINT32 u4Size);
EXTERN BOOL TZ_HDCP2_DecryptEKs(void *prKernParam, UINT32 u4Size);
EXTERN BOOL TZ_HDCP2_KsXorLC128(void *prKernParam, UINT32 u4Size);
EXTERN BOOL TZ_HDCP2_SetRiv_Pid(void *prKernParam, UINT32 u4Size);
EXTERN BOOL TZ_HDCP2_PresetKsLC128(void *prKernParam, UINT32 u4Size);
EXTERN BOOL TZ_HDCP2_Generate_Km(void *prKernParam, UINT32 u4Size);
EXTERN BOOL TZ_HDCP2_Generate_Ks(void *prKernParam, UINT32 u4Size);
EXTERN BOOL TZ_HDCP2_GetEncKm(void *prKernParam, UINT32 u4Size);
EXTERN BOOL TZ_HDCP2_SetEncKm(void *prKernParam, UINT32 u4Size);


EXTERN BOOL TZ_HDCP2_HMAC_SHA256(void *prKernParam, UINT32 u4Size);


/*
	for HDCP 2.2
*/


BOOL TZ_HDCP2_2_KdKeyDerivation(void *prKernParam, UINT32 u4Size);
BOOL TZ_HDCP2_2_ComputeHprime(void *prKernParam, UINT32 u4Size);
BOOL TZ_HDCP2_2_ComputeLprime(void *prKernParam, UINT32 u4Size);


/*
	for hdcp align
*/

EXTERN BOOL TZ_HDCP2_SetEncKeySet(void * prKernParam,UINT32 u4Size);



/*
	for HDCP TX
*/

EXTERN BOOL TZ_HDCP2_RetrvEkh_km(void *prKernParam, UINT32 u4Size);
EXTERN BOOL TZ_HDCP2_SavePairingInfo(void *prKernParam, UINT32 u4Size);
EXTERN BOOL TZ_HDCP2_CheckRxID(void *prKernParam, UINT32 u4Size);
EXTERN BOOL TZ_HDCP2_ComputeMprime(void *prKernParam, UINT32 u4Size);
EXTERN BOOL TZ_HDCP2_ComputeVprime(void *prKernParam, UINT32 u4Size);
EXTERN BOOL TZ_HDCP2_2_ComputeVprime(void *prKernParam, UINT32 u4Size);
EXTERN BOOL TZ_HDCP2_VerifySignature(void *prKernParam, UINT32 u4Size);
EXTERN BOOL TZ_HDCP2_Generate_Km_for_tx(void *prKernParam, UINT32 u4Size);
EXTERN BOOL TZ_HDCP2_UseTestKey_InKernel(void *prKernParam, UINT32 u4Size);
EXTERN BOOL TZ_HDCP2_ResetPairingInfo(void *prKernParam, UINT32 u4Size);


/*for android  */
EXTERN BOOL TZ_HDCP2_SW_Decrypt	(void *prKernParam, UINT32 u4Size);

EXTERN BOOL TZ_HDCP2_enable_tz_key(void *prKernParam, UINT32 u4Size);

EXTERN BOOL TZ_HDCP2_GetPDKey(void *prKernParam, UINT32 u4Size);
EXTERN BOOL TZ_HDCP2_SetPDKey(void *prKernParam, UINT32 u4Size);

#endif

EXTERN BOOL TZ_OEMCrypto_Initialize(void *prKernParam, UINT32 u4Size);
EXTERN BOOL TZ_OEMCrypto_Terminate(void *prKernParam, UINT32 u4Size);
EXTERN BOOL TZ_OEMCrypto_OpenSession(void *prKernParam, UINT32 u4Size);
EXTERN BOOL TZ_OEMCrypto_CloseSession(void *prKernParam, UINT32 u4Size);
EXTERN BOOL TZ_OEMCrypto_GenerateDerivedKeys(void *prKernParam, UINT32 u4Size);
EXTERN BOOL TZ_OEMCrypto_GenerateNonce(void *prKernParam, UINT32 u4Size);
EXTERN BOOL TZ_OEMCrypto_GenerateSignature(void *prKernParam, UINT32 u4Size);
EXTERN BOOL TZ_OEMCrypto_LoadKeys(void *prKernParam, UINT32 u4Size);
EXTERN BOOL TZ_OEMCrypto_RefreshKeys(void *prKernParam, UINT32 u4Size);
EXTERN BOOL TZ_OEMCrypto_SelectKey(void *prKernParam, UINT32 u4Size);
EXTERN BOOL TZ_OEMCrypto_DecryptCTR(void *prKernParam, UINT32 u4Size);
EXTERN BOOL TZ_OEMCrypto_InstallKeybox(void *prKernParam, UINT32 u4Size);
EXTERN BOOL TZ_OEMCrypto_IsKeyboxValid(void *prKernParam, UINT32 u4Size);
EXTERN BOOL TZ_OEMCrypto_GetDeviceID(void *prKernParam, UINT32 u4Size);
EXTERN BOOL TZ_OEMCrypto_GetKeyData(void *prKernParam, UINT32 u4Size);
EXTERN BOOL TZ_OEMCrypto_GetRandom(void *prKernParam, UINT32 u4Size);
EXTERN BOOL TZ_OEMCrypto_WrapKeybox(void *prKernParam, UINT32 u4Size);
EXTERN BOOL TZ_OEMCrypto_RewrapDeviceRSAKey(void *prKernParam, UINT32 u4Size);
EXTERN BOOL TZ_OEMCrypto_LoadDeviceRSAKey(void *prKernParam, UINT32 u4Size);
EXTERN BOOL TZ_OEMCrypto_GenerateRSASignature(void *prKernParam, UINT32 u4Size);
EXTERN BOOL TZ_OEMCrypto_DeriveKeysFromSessionKey(void *prKernParam, UINT32 u4Size);
EXTERN BOOL TZ_OEMCrypto_Generic_Encrypt(void *prKernParam, UINT32 u4Size);
EXTERN BOOL TZ_OEMCrypto_Generic_Decrypt(void *prKernParam, UINT32 u4Size);
EXTERN BOOL TZ_OEMCrypto_Generic_Sign(void *prKernParam, UINT32 u4Size);
EXTERN BOOL TZ_OEMCrypto_Generic_Verify(void *prKernParam, UINT32 u4Size);

EXTERN BOOL TZ_OEMCrypto_SetEntitlementKey(void *prKernParam, UINT32 u4Size);
EXTERN BOOL TZ_OEMCrypto_DeriveControlWord(void *prKernParam, UINT32 u4Size); 
EXTERN BOOL TZ_OEMCrypto_DecryptVideo(void *prKernParam, UINT32 u4Size);
EXTERN BOOL TZ_OEMCrypto_DecryptAudio(void *prKernParam, UINT32 u4Size);

EXTERN BOOL TZ_OEMCrypto_UpdateUsageTable(void *arg, UINT32 u4Size);
EXTERN BOOL TZ_OEMCrypto_DeactivateUsageEntry(void *arg, UINT32 u4Size);
EXTERN BOOL TZ_OEMCrypto_ReportUsage(void *arg, UINT32 u4Size);
EXTERN BOOL TZ_OEMCrypto_DeleteUsageEntry(void *arg, UINT32 u4Size);
EXTERN BOOL TZ_OEMCrypto_DeleteUsageTable(void *arg, UINT32 u4Size);

EXTERN BOOL TZ_OEMCrypto_GetUsageTableWriteRequest(void *prKernParam, UINT32 u4Size);
EXTERN BOOL TZ_OEMCrypto_VerifyWriteResponse(void *prKernParam, UINT32 u4Size);

EXTERN BOOL TZ_OEMCrypto_GetUsageTableReadRequest(void *arg, UINT32 u4Size);
EXTERN BOOL TZ_OEMCrypto_VerifyReadResponse(void *arg, UINT32 u4Size);

EXTERN BOOL TZ_SEC_BUF_OPEN_SESSION(void *prKernParam  , UINT32 u4Size);
EXTERN BOOL TZ_SEC_BUF_CLOSE_SESSION(void *prKernParam , UINT32 u4Size);
EXTERN BOOL TZ_SEC_BUF_ALLOC(void *prKernParam         , UINT32 u4Size);
EXTERN BOOL TZ_SEC_BUF_FREE(void *prKernParam          , UINT32 u4Size);
EXTERN BOOL TZ_SEC_BUF_FLUSH(void *prKernParam         , UINT32 u4Size);
EXTERN BOOL TZ_SEC_BUF_FRAGMENT_ALLOC(void *prKernParam , UINT32 u4Size);
EXTERN BOOL TZ_SEC_BUF_FRAGMENT_FREE(void *prKernParam , UINT32 u4Size);
EXTERN BOOL TZ_SEC_BUF_CPB_MOVEDATA(void *prKernParam  , UINT32 u4Size);

EXTERN BOOL TZ_RPMB_LOAD_MAC_KEY(void *prKernParam         , UINT32 u4Size);
EXTERN BOOL TZ_RPMB_GET_WCNT_REQ(void *prKernParam , UINT32 u4Size);
EXTERN BOOL TZ_RPMB_SET_WCNT_RESP(void *prKernParam  , UINT32 u4Size);

/* SONY IPTVES */

/* sac */
EXTERN BOOL TZ_IPTVES_SAC_CONSTRUCT(void *prKernParam, UINT32 u4Size);
EXTERN BOOL TZ_IPTVES_SAC_DESTRUCT(void *prKernParam, UINT32 u4Size);
EXTERN BOOL TZ_IPTVES_SAC_MkMsg(void *prKernParam, UINT32 u4Size);
EXTERN BOOL TZ_IPTVES_SAC_ProcessMsg(void *prKernParam, UINT32 u4Size);




/* sdi */
EXTERN BOOL TZ_IPTVES_SDI_INIT(void *prKernParam, UINT32 u4Size);
          			     
EXTERN BOOL TZ_IPTVES_SDI_FIN(void *prKernParam, UINT32 u4Size);
          			     
EXTERN BOOL TZ_IPTVES_SDI_CleanupNVS(void *prKernParam, UINT32 u4Size);
       		   
EXTERN BOOL TZ_IPTVES_SDI_GetDeviceID(void *prKernParam, UINT32 u4Size);

EXTERN BOOL TZ_IPTVES_SDI_VerifyServiceMetaData(void *prKernParam, UINT32 u4Size);
   
EXTERN BOOL TZ_IPTVES_SDI_JudgeCRLUpdate(void *prKernParam, UINT32 u4Size);
      		 
EXTERN BOOL TZ_IPTVES_SDI_GetCRLDstPoint(void *prKernParam, UINT32 u4Size);
          
EXTERN BOOL TZ_IPTVES_SDI_SetCRL(void *prKernParam, UINT32 u4Size);
                  
EXTERN BOOL TZ_IPTVES_SDI_CheckTrustedTime(void *prKernParam, UINT32 u4Size);
        
                                                 
                                                 
/* ecm */                                        
EXTERN BOOL TZ_IPTVES_ECM_ExtractScrambleKey(void *prKernParam, UINT32 u4Size);
      
EXTERN BOOL TZ_IPTVES_ECM_GetScrambleKey(void *prKernParam, UINT32 u4Size);
    		   
                                                 
                                                 
/* drm */                                        
EXTERN BOOL TZ_IPTVES_DRM_ExtractContentKey(void *prKernParam, UINT32 u4Size);
       
EXTERN BOOL TZ_IPTVES_DRM_DelContentKey(void *prKernParam, UINT32 u4Size);
          	
EXTERN BOOL TZ_IPTVES_DRM_DelUncommittedKey(void *prKernParam, UINT32 u4Size);
       
EXTERN BOOL TZ_IPTVES_DRM_GetKeyInfo(void *prKernParam, UINT32 u4Size);
   			     
EXTERN BOOL TZ_IPTVES_DRM_GetKeyNum(void *prKernParam, UINT32 u4Size);
     			     
EXTERN BOOL TZ_IPTVES_DRM_GetKeyRefList(void *prKernParam, UINT32 u4Size);
      	   
EXTERN BOOL TZ_IPTVES_DRM_GetContentKey(void *prKernParam, UINT32 u4Size);
           


/* nvs */
EXTERN BOOL TZ_IPTVES_NVS_MEM_Init(void *prKernParam, UINT32 u4Size);
EXTERN BOOL TZ_IPTVES_NVS_MEM_Fin(void *prKernParam, UINT32 u4Size);
EXTERN BOOL TZ_IPTVES_NVS_MEM_GetRWLen(void *prKernParam, UINT32 u4Size);



/* time */
EXTERN BOOL TZ_IPTVES_TIME_SetTrustedTime(void *prKernParam, UINT32 u4Size);
EXTERN BOOL TZ_IPTVES_TIME_GetTrustedTime(void *prKernParam, UINT32 u4Size);
EXTERN BOOL TZ_IPTVES_TIME_Cleanup(void *prKernParam, UINT32 u4Size);



#if defined(CC_TRUSTZONE_SUPPORT) && defined(CC_TVP_SUPPORT)
BOOL TZ_VDEC_FB_Operation(VOID* prTzFbOp, UCHAR ucOperation);
BOOL TZ_VDEC_Check_RealD(VOID *prDisp);
#endif

#if defined(CC_TRUSTZONE_SUPPORT) && defined(CC_SVP_SUPPORT)
BOOL TZ_AU_DELIM_APPEND(UINT32 u4Base, UINT32 u4Offset, UINT32 u4DelimSize, UINT32* u4DelimBuf);
#endif

#if defined(CC_TVP_SUPPORT)
BOOL TZ_FBM_Initialization(void *prKernParam  , UINT32 u4Size);
#endif

EXTERN BOOL TZ_TEE_Common_Operate(UINT32 ui4Command, void *ptCommonArg, UINT32 u4Size);
#endif  // TZ_IF_H


