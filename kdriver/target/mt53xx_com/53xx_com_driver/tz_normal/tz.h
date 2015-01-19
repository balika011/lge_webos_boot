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
 * $Date: 2015/01/19 $
 * $RCSfile: dmx.h,v $
 * $Revision: #3 $
 *
 *---------------------------------------------------------------------------*/

/** @file tz.h
 *  Demux driver - common definitions
 */


#ifndef TZ_H
#define TZ_H

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#include "tz_if.h"
#include "x_hal_5381.h"


//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
#define TZ_SUCCESS                  ((UINT32)0x00)
#define TZ_PENDING                  ((UINT32)0x01)
#define TZ_ERROR_ACCESS_DENIED      ((UINT32)0x02)
#define TZ_ERROR_BUSY               ((UINT32)0x03)
#define TZ_ERROR_CANCEL             ((UINT32)0x04)
#define TZ_ERROR_COMMUNICATION      ((UINT32)0x05)
#define TZ_ERROR_DECODE_NO_DATA     ((UINT32)0x06)
#define TZ_ERROR_DECODE_TYPE        ((UINT32)0x07)
#define TZ_ERROR_ENCODE_FORMAT      ((UINT32)0x08)
#define TZ_ERROR_ENCODE_MEMORY      ((UINT32)0x09)
#define TZ_ERROR_GENERIC            ((UINT32)0x0a)
#define TZ_ERROR_ILLEGAL_ARGUMENT   ((UINT32)0x0b)
#define TZ_ERROR_ILLEGAL_STATE      ((UINT32)0x0c)
#define TZ_ERROR_MEMORY             ((UINT32)0x0d)
#define TZ_ERROR_NOT_IMPLEMENTED    ((UINT32)0x0e)
#define TZ_ERROR_SECURITY           ((UINT32)0x0f)
#define TZ_ERROR_SERVICE            ((UINT32)0x10)
#define TZ_ERROR_SHORT_BUFFER       ((UINT32)0x11)
#define TZ_ERROR_UNDEFINED          ((UINT32)0x12)
#define TZ_ERROR_ITEM_NOT_FOUND     ((UINT32)0x13)
#define TZ_ERROR_CRYPTO             ((UINT32)0x14)

#define TZCTL_SYSTEM_ECHO                           ((UINT32)0x0000)
#define TZCTL_SYSTEM_ENABLE_DEBUG                   ((UINT32)0x0001)
#define TZCTL_SYSTEM_REVERSE_ARRAY                  ((UINT32)0x0002)

#define TZCTL_SYSTEM_IO_WRITE32                     ((UINT32)0x0010)
#define TZCTL_SYSTEM_IO_WRITE16                     ((UINT32)0x0011)
#define TZCTL_SYSTEM_IO_WRITE8                      ((UINT32)0x0012)
#define TZCTL_SYSTEM_IO_READ32                      ((UINT32)0x0013)
#define TZCTL_SYSTEM_IO_READ16                      ((UINT32)0x0014)
#define TZCTL_SYSTEM_IO_READ8                       ((UINT32)0x0015)

#define TZCTL_MARLIN_BASE                           ((UINT32)0x1000)
#define TZCTL_HCI_INIT                              ((UINT32)0x1001)
#define TZCTL_HCI_FIN                               ((UINT32)0x1002)
#define TZCTL_HCI_KEYREF_CREATE                     ((UINT32)0x1003)
#define TZCTL_HCI_KEYREF_CHECK                      ((UINT32)0x1004)
#define TZCTL_HCI_KEYREF_DESTROY                    ((UINT32)0x1005)
#define TZCTL_HCI_DEVICE_GET_NUM                    ((UINT32)0x1006)
#define TZCTL_HCI_DEVICE_GET_SIZE                   ((UINT32)0x1007)
#define TZCTL_HCI_DEVICE_GET_NAME                   ((UINT32)0x1008)
#define TZCTL_HCI_IMPORT_KEY                        ((UINT32)0x1009)
#define TZCTL_HCI_EXPORT_KEY_SIZE                   ((UINT32)0x100a)
#define TZCTL_HCI_EXPORT_KEY                        ((UINT32)0x100b)
#define TZCTL_HCI_GET_START_FISH_ID_SIZE            ((UINT32)0x100c)
#define TZCTL_HCI_GET_START_FISH_ID                 ((UINT32)0x100d)
#define TZCTL_HCI_GET_START_FISH_KEYDATA_SIZE       ((UINT32)0x100e)
#define TZCTL_HCI_GET_START_FISH_KEYDATA            ((UINT32)0x100f)
#define TZCTL_HCI_IMPORT_START_FISH_KEY             ((UINT32)0x1010)
#define TZCTL_HCI_DATA_SIGN_SIZE                    ((UINT32)0x1011)
#define TZCTL_HCI_DATA_SIGN                         ((UINT32)0x1012)
#define TZCTL_HCI_DATA_DECRYPT_SIZE                 ((UINT32)0x1013)
#define TZCTL_HCI_DATA_DECRYPT                      ((UINT32)0x1014)
#define TZCTL_HCI_STREAM_DECRYPT_SIZE               ((UINT32)0x1015)
#define TZCTL_HCI_STREAM_DECRYP                     ((UINT32)0x1016)
#define TZCTL_HCI_KEYDATA_ENCRYPT_SIZE              ((UINT32)0x1017)
#define TZCTL_HCI_KEYDATA_ENCRYPT                   ((UINT32)0x1018)
#define TZCTL_HCI_HMAC_CHECK                        ((UINT32)0x1019)
#define TZCTL_HCI_HASH_CHECK                        ((UINT32)0x102a)
#define TZCTL_HCI_MOVE_KEY_SIZE                     ((UINT32)0x102b)
#define TZCTL_HCI_MOVE_KEY                          ((UINT32)0x102c)
#define TZCTL_HCI_PERSONALITYKEY_LOAD               ((UINT32)0x102d)
#define TZCTL_HCI_PERSONALITYKEY_CHECK              ((UINT32)0x102e)
#define TZCTL_HCI_IMPORT_SAS_SIZE                   ((UINT32)0x102f)
#define TZCTL_HCI_IMPORT_SAS                        ((UINT32)0x1030)
#define TZCTL_HCI_IMPORT_SAS_IDS                    ((UINT32)0x1031)
#define TZCTL_HCI_FREE_SAS                          ((UINT32)0x1032)
#define TZCTL_HCI_LOCK                              ((UINT32)0x1033)
#define TZCTL_HCI_UNLOCK                            ((UINT32)0x1034)
#define TZCTL_HCI_FREEMEM                           ((UINT32)0x1035)

#define TZCTL_SKB_GET_INSTANCE                      ((UINT32)0x1100)
#define TZCTL_SKB_RELEASE                           ((UINT32)0x1101)
#define TZCTL_SKB_GET_INFO                          ((UINT32)0x1102)
#define TZCTL_SKB_GET_PROPERTIES_SIZE               ((UINT32)0x1103)
#define TZCTL_SKB_GET_PROPERTIES_DATA               ((UINT32)0x1104)
//#define TZCTL_SKB_CREATE_DATA_FROM_WRAPPED           ((UINT32)0x1105)
#define TZCTL_SKB_CREATE_DATA_FROM_WRAPPED          ((UINT32)0x1106)
#define TZCTL_SKB_CREATE_DATA_FROM_EXPORTED         ((UINT32)0x1107)
#define TZCTL_SKB_CREATE_TRANSFORM                  ((UINT32)0x1108)
#define TZCTL_SKB_CREATE_CIPHER                     ((UINT32)0x1109)
#define TZCTL_SKB_SECUREDATA_RELEASE                ((UINT32)0x110a)
#define TZCTL_SKB_SECUREDATA_GETINFO                ((UINT32)0x110b)
#define TZCTL_SKB_SECUREDATA_EXPORT                 ((UINT32)0x110c)
#define TZCTL_SKB_SECUREDATA_DERIVE                 ((UINT32)0x110d)
#define TZCTL_SKB_TRANSFORM_RELEASE                 ((UINT32)0x110e)
#define TZCTL_SKB_TRANSFORM_ADDBYTES                ((UINT32)0x110f)
#define TZCTL_SKB_TRANSFORM_ADDSECUREDATA           ((UINT32)0x1110)
#define TZCTL_SKB_TRANSFORM_GETOUTPUT               ((UINT32)0x1111)
#define TZCTL_SKB_CIPHER_PROCESSBUFFER              ((UINT32)0x1112)
#define TZCTL_SKB_CIPHER_RELEASE                    ((UINT32)0x1113)

#define TZCTL_DRM_TZ_SETSECRETDATATYPE              ((UINT32)0x1200)
#define TZCTL_DRM_TZ_OPEN                           ((UINT32)0x1201)
#define TZCTL_DRM_TZ_CLOSE                          ((UINT32)0x1202)
#define TZCTL_DRM_TZ_DECODEPACKETSVOD               ((UINT32)0x1203)
#define TZCTL_DRM_TZ_LOADSECRETDATATYPE             ((UINT32)0x1204)

#ifdef CC_ENABLE_HDCP2
#define TZCTL_HDCP2_GETCERTINFO                     ((UINT32)0x1300)
#define TZCTL_HDCP2_DECRYPT_RSAESOAEP               ((UINT32)0x1301)
#define TZCTL_HDCP2_KDKEYDERIVATION                 ((UINT32)0x1302)
#define TZCTL_HDCP2_COMPUTE_HPRIME                  ((UINT32)0x1303)
#define TZCTL_HDCP2_COMPUTE_LPRIME                  ((UINT32)0x1304)
#define TZCTL_HDCP2_COMPUTE_KH                      ((UINT32)0x1305)
#define TZCTL_HDCP2_ENCRYPT_KM                      ((UINT32)0x1306)
#define TZCTL_HDCP2_DECRYPT_KM                      ((UINT32)0x1307)
#define TZCTL_HDCP2_DECRYPT_EKS                     ((UINT32)0x1308)
#define TZCTL_HDCP2_KSLC128XOR                      ((UINT32)0x1309)
#define TZCTL_HDCP2_DECRYPT_PES                     ((UINT32)0x1310)
#define TZCTL_HDCP2_SET_ENC_KEY                     ((UINT32)0x1311)
#define TZCTL_HDCP2_SET_ENC_LC128                   ((UINT32)0x1312)
#define TZCTL_HDCP2_SETRIV_PID                      ((UINT32)0x1313)
#define TZCTL_HDCP2_PRESETKSLC128                   ((UINT32)0x1314)
#define TZCTL_HDCP2_GENERATE_KM                     ((UINT32)0x1315)
#define TZCTL_HDCP2_GENERATE_KS                     ((UINT32)0x1316)
#define TZCTL_HDCP2_SET_ENC_KM                      ((UINT32)0x1317)
#define TZCTL_HDCP2_GET_ENC_KM                      ((UINT32)0x1318)

/*for HDCP 2.2 */

#define TZCTL_HDCP2_2_KDKEYDERIVATION                 ((UINT32)0x1319)
#define TZCTL_HDCP2_2_COMPUTE_HPRIME                  ((UINT32)0x131A)
#define TZCTL_HDCP2_2_COMPUTE_LPRIME                  ((UINT32)0x131B)


/*for HDCP2.X ALIGN*/
#define TZCTL_HDCP2_SET_ENC_KEYSET                    ((UINT32)0x131C)

/*for HDCP2.X TX */
#define TZCTL_HDCP2_RETRVE_KH_KM                    	 ((UINT32)0x131D)
#define TZCTL_HDCP2_SAVE_PAIRINGINFO                     ((UINT32)0x131E)
#define TZCTL_HDCP2_CHECK_RXID                   		 ((UINT32)0x131F)
#define TZCTL_HDCP2_COMPUTE_MPRIME                       ((UINT32)0x1320)
#define TZCTL_HDCP2_COMPUTE_VPRIME                       ((UINT32)0x132A)
#define TZCTL_HDCP2_2_COMPUTE_VHPRIME                    ((UINT32)0x132B)
#define TZCTL_HDCP2_VERIFY_SIGNATURE					 ((UINT32)0x132C)
#define TZCTL_HDCP2_GENERATE_KM_FOR_TX                   ((UINT32)0x132D)
#define TZCTL_HDCP2_SET_KEY_TYPE                         ((UINT32)0x132E)
#define TZCTL_HDCP2_ResetPairingInfo                     ((UINT32)0x132F)

#define TZCTL_HDCP2_HMAC_SHA256                  	  ((UINT32)0x1330)
#define TZCTL_HDCP2_SW_DecryptData                  	 ((UINT32)0x1331)

#define TZCTL_HDCP2_ENABLE_TZ_KEY                         ((UINT32)0x1332)

#endif

#define TZCTL_OEMCrypto_Initialize                      ((UINT32)0x1400)
#define TZCTL_OEMCrypto_Terminate                       ((UINT32)0x1401)
#define TZCTL_OEMCrypto_OpenSession                     ((UINT32)0x1402)
#define TZCTL_OEMCrypto_CloseSession                    ((UINT32)0x1403)
#define TZCTL_OEMCrypto_GenerateDerivedKeys             ((UINT32)0x1404)
#define TZCTL_OEMCrypto_GenerateNonce                   ((UINT32)0x1405)
#define TZCTL_OEMCrypto_GenerateSignature               ((UINT32)0x1406)
#define TZCTL_OEMCrypto_LoadKeys                        ((UINT32)0x1407)
#define TZCTL_OEMCrypto_RefreshKeys                     ((UINT32)0x1408)
#define TZCTL_OEMCrypto_SelectKey                       ((UINT32)0x1409)
#define TZCTL_OEMCrypto_DecryptCTR                      ((UINT32)0x1410)
#define TZCTL_OEMCrypto_InstallKeybox                   ((UINT32)0x1411)
#define TZCTL_OEMCrypto_IsKeyboxValid                   ((UINT32)0x1412)
#define TZCTL_OEMCrypto_GetDeviceID                     ((UINT32)0x1413)
#define TZCTL_OEMCrypto_GetKeyData                      ((UINT32)0x1414)
#define TZCTL_OEMCrypto_GetRandom                       ((UINT32)0x1415)
#define TZCTL_OEMCrypto_WrapKeybox                      ((UINT32)0x1416)
#define TZCTL_OEMCrypto_RewrapDeviceRSAKey              ((UINT32)0x1417)
#define TZCTL_OEMCrypto_LoadDeviceRSAKey                ((UINT32)0x1418)
#define TZCTL_OEMCrypto_GenerateRSASignature            ((UINT32)0x1419)
#define TZCTL_OEMCrypto_DeriveKeysFromSessionKey        ((UINT32)0x1421)
#define TZCTL_OEMCrypto_Generic_Encrypt                 ((UINT32)0x1422)
#define TZCTL_OEMCrypto_Generic_Decrypt                 ((UINT32)0x1423)
#define TZCTL_OEMCrypto_Generic_Sign                    ((UINT32)0x1424)
#define TZCTL_OEMCrypto_Generic_Verify                  ((UINT32)0x1425)
#define TZCTL_OEMCrypto_SetEntitlementKey               ((UINT32)0x1426)
#define TZCTL_OEMCrypto_DeriveControlWord               ((UINT32)0x1427)
#define TZCTL_OEMCrypto_DecryptVideo                    ((UINT32)0x1428)
#define TZCTL_OEMCrypto_DecryptAudio                    ((UINT32)0x1429)

#define TZCTL_OEMCrypto_UpdateUsageTable                ((UINT32)0x142a)
#define TZCTL_OEMCrypto_DeactivateUsageEntry            ((UINT32)0x142b)
#define TZCTL_OEMCrypto_ReportUsage                     ((UINT32)0x142c)
#define TZCTL_OEMCrypto_DeleteUsageEntry                ((UINT32)0x142d)
#define TZCTL_OEMCrypto_DeleteUsageTable                ((UINT32)0x142e)

#define TZCTL_OEMCrypto_GetUsageTableWriteRequest       ((UINT32)0x142f)
#define TZCTL_OEMCrypto_VerifyWriteResponse             ((UINT32)0x1430)
#define TZCTL_OEMCrypto_GetUsageTableReadRequest        ((UINT32)0x1431)
#define TZCTL_OEMCrypto_VerifyReadResponse              ((UINT32)0x1432)
   
#define TZCTL_SEC_BUF_OPEN_SESSION                  ((UINT32)0x1800)
#define TZCTL_SEC_BUF_CLOSE_SESSION                 ((UINT32)0x1801)
#define TZCTL_SEC_BUF_ALLOC                         ((UINT32)0x1802)
#define TZCTL_SEC_BUF_FREE                          ((UINT32)0x1803)
#define TZCTL_SEC_BUF_FLUSH                         ((UINT32)0x1804)
#define TZCTL_SEC_BUF_FRAGMENT_ALLOC				((UINT32)0x1805)
#define TZCTL_SEC_BUF_FRAGMENT_FREE                 ((UINT32)0x1806)
#define TZCTL_SEC_BUF_CPB_MOVEDATA                  ((UINT32)0x1807)

#define TZCTL_RPMB_LOAD_MAC_KEY                     ((UINT32)0x1880)
#define TZCTL_RPMB_GET_WCNT_REQ				        ((UINT32)0x1881)
#define TZCTL_RPMB_SET_WCNT_RESP                    ((UINT32)0x1882)





/* sony iptv es*/

/*saq*/
#define TZCTL_IPTVES_SAC_CONSTRUCT                    ((UINT32)0x1900)
#define TZCTL_IPTVES_SAC_DESTRUCT                     ((UINT32)0x1901)
#define TZCTL_IPTVES_SAC_MAKEMSG                      ((UINT32)0x1902)
#define TZCTL_IPTVES_SAC_PROCESSMSG                   ((UINT32)0x1903)



/* sdi */
#define TZCTL_IPTVES_SDI_INIT          			      ((UINT32)0x1904)
#define TZCTL_IPTVES_SDI_FIN          			      ((UINT32)0x1905)
#define TZCTL_IPTVES_SDI_CleanupNVS       		      ((UINT32)0x1906)
#define TZCTL_IPTVES_SDI_GetDeviceID    			  ((UINT32)0x1907)
#define TZCTL_IPTVES_SDI_VerifyServiceMetaData        ((UINT32)0x1908)
#define TZCTL_IPTVES_SDI_JudgeCRLUpdate      		  ((UINT32)0x1909)
#define TZCTL_IPTVES_SDI_GetCRLDstPoint               ((UINT32)0x190a)
#define TZCTL_IPTVES_SDI_SetCRL                       ((UINT32)0x190b)
#define TZCTL_IPTVES_SDI_CheckTrustedTime             ((UINT32)0x190c)
                                                  
                                                  
/* ecm */                                         
#define TZCTL_IPTVES_ECM_ExtractScrambleKey           ((UINT32)0x190d)
#define TZCTL_IPTVES_ECM_GetScrambleKey    		      ((UINT32)0x190e)
                                                  
                                                  
/* drm */                                         
#define TZCTL_IPTVES_DRM_ExtractContentKey            ((UINT32)0x190f)
#define TZCTL_IPTVES_DRM_DelContentKey          	  ((UINT32)0x1910)
#define TZCTL_IPTVES_DRM_DelUncommittedKey       	  ((UINT32)0x1911)
#define TZCTL_IPTVES_DRM_GetKeyInfo   			      ((UINT32)0x1912)
#define TZCTL_IPTVES_DRM_GetKeyNum     			      ((UINT32)0x1913)
#define TZCTL_IPTVES_DRM_GetKeyRefList      	      ((UINT32)0x1914)
#define TZCTL_IPTVES_DRM_GetContentKey                ((UINT32)0x1915)

/* NVS mem */
#define TZCTL_IPTVES_NVS_MEM_Init	                  ((UINT32)0x1916)
#define TZCTL_IPTVES_NVS_MEM_Fin	                  ((UINT32)0x1917)
#define TZCTL_IPTVES_NVS_MEM_GetRWLen                 ((UINT32)0x1918)


/* time */
#define TZCTL_IPTVES_TIME_SetTrustedTime     		   ((UINT32)0x1919)
#define TZCTL_IPTVES_TIME_GetTrustedTime     		   ((UINT32)0x191a)
#define TZCTL_IPTVES_TIME_Cleanup     		   		   ((UINT32)0x191b)


#define TZCTL_HDCP_TEST                             ((UINT32)0x2000)

#define TZCTL_WV_BASE                               ((UINT32)0x3000)
#define TZCTL_WV_SET_ASSET_KEY                      ((UINT32)0x3001)
#define TZCTL_WV_SET_DERIVE_CW                      ((UINT32)0x3002)
#define TZCTL_WV_PASS_KEYBOX                        ((UINT32)0x3003)
///////////////////////////////////for hulu+dash+wv drm only begin//////////////
#define TZCTL_WV_DESTROYSESSION                     ((UINT32)0x3004)
#define TZCTL_WV_CREATESESSION                      ((UINT32)0x3005)
#define TZCTL_WV_GENERATE_NONCE                     ((UINT32)0x3006)
#define TZCTL_WV_GENERATE_DERIVEKEY                 ((UINT32)0x3007)
#define TZCTL_WV_GENERATE_SIGNATURE                 ((UINT32)0x3008)
#define TZCTL_WV_VALIDATEM_MESSAGE                  ((UINT32)0x3009)
#define TZCTL_WV_INSTALL_KEY                        ((UINT32)0x3010)
#define TZCTL_WV_FLUSH_NONCE                        ((UINT32)0x3011)
#define TZCTL_WV_UPDATE_MACKEY                      ((UINT32)0x3012)
#define TZCTL_WV_REFRESH_KEY                        ((UINT32)0x3013)
#define TZCTL_WV_SELECT_CONTENT_KEY                 ((UINT32)0x3014)
#define TZCTL_WV_SAVE_KEYBOX                        ((UINT32)0x3015)
#define TZCTL_WV_GET_RANDOM_NUM                     ((UINT32)0x3016)

#define TZCTL_WV_REWRAP_RSAKEY                      ((UINT32)0x3017)
#define TZCTL_WV_LOAD_RSAKET                        ((UINT32)0x3018)
#define TZCTL_WV_GENERATE_RSASIGN                   ((UINT32)0x3019)
#define TZCTL_WV_RSA_DERIVEKEY                      ((UINT32)0x3020)

#define TZCTL_WV_GENERIC_SIGN                       ((UINT32)0x3021)
#define TZCTL_WV_GENERIC_VERIFY                     ((UINT32)0x3022)
#define TZCTL_WV_CHK_CTR_KEY                     	((UINT32)0x3023)
#define TZCTL_WV_CHK_CBC_KEY                     	((UINT32)0x3024)
#if 0
#define TZCTL_WV_CHK_DEC_CTR 						((UINT32)0x3025)
#define TZCTL_WV_GENERIC_ENC						((UINT32)0x3026)
#define TZCTL_WV_GENERIC_DEC						((UINT32)0x3027)
#endif

#define TZCTL_WV_VALIDATE_KEYBOX                    ((UINT32)0x3028)

///////////////////////////////////for hulu+dash+wv drm only begin//////////////

#define TZCTL_DMX_BASE                              ((UINT32)0x4000)
#define TZCTL_DMX_GETPICHDR                         ((UINT32)0x4001)
#define TZCTL_DMX_GESECUREVFIFO                     ((UINT32)0x4002)
#define TZCTL_DMX_GESECUREVFIFO2                    ((UINT32)0x4003)
#define TZCTL_DMX_RELOADSECUREKEY                   ((UINT32)0x4004)
#define TZCTL_DMX_INIT                              ((UINT32)0x4005)
#define TZCTL_DMX_SET_MMKEY                         ((UINT32)0x4006)
#define TZCTL_DMX_FREESECUREKEY                     ((UINT32)0x4007)
#define TZCTL_DMX_SETCRYPTOKEY                      ((UINT32)0x4009)
#define TZCTL_FVR_SETCRYPTOKEY                      ((UINT32)0x400A)
#define TZCTL_DMX_SETMMSECURITY                     ((UINT32)0x400C)

#define TZCTL_GCPU_BASE                             ((UINT32)0x4100)
#define TZCTL_GCPU_HW_CMDRUN                        ((UINT32)0x4101)
#define TZCTL_GCPU_HW_CMDRETURN                     ((UINT32)0x4102)
#define TZCTL_GCPU_ENABLE_IOMMU                     ((UINT32)0x4103)
#define TZCTL_GCPU_DISABLE_IOMMU                    ((UINT32)0x4104)
#define TZCTL_GCPU_CLEAR_IRQ                        ((UINT32)0x4105)
#define TZCTL_GCPU_CLEAR_IOMMU_IRQ                  ((UINT32)0x4106)
#define TZCTL_GCPU_SET_SLOT                         ((UINT32)0x4107)
#define TZCTL_GCPU_HW_RESET                         ((UINT32)0x4108)
#define TZCTL_GCPU_IRQ_HANDLER                      ((UINT32)0x4109)
#define TZCTL_GCPU_IOMMU_IRQ_HANDLER                ((UINT32)0x410A)
#define TZCTL_GCPU_GET_CHECKSUM                     ((UINT32)0x410B)
#define TZCTL_GCPU_RESTORE_SECURE_KEYS              ((UINT32)0x410C)

#define TZCTL_DRM_DIVX_GETHWSECRETKEY0              ((UINT32)0x5000)
#define TZCTL_DRM_DIVX_GETHWSECRETKEY1              ((UINT32)0x5001)

/* Include TZ_CTL_TEE_XXX defines (0x6xxx) */
#include "tz_tee.h"

#define TZCTL_VDEC_BASE                             ((UINT32)0x7000)
#define TZCTL_VDEC_AVC_SPS                          ((UINT32)0x7001)
#define TZCTL_VDEC_AVC_PPS                          ((UINT32)0x7002)
#define TZCTL_VDEC_AVC_SLICE                        ((UINT32)0x7003)
#define TZCTL_VDEC_AVC_SEI                          ((UINT32)0x7004)
#define TZCTL_VDEC_MPEG4_M4V                        ((UINT32)0x7005)
#define TZCTL_VDEC_MPEG4_H263                       ((UINT32)0x7006)
#define TZCTL_VDEC_MPEG4_FLV                        ((UINT32)0x7007)
#define TZCTL_VDEC_MPEG4_DX311                      ((UINT32)0x7008)
#define TZCTL_VDEC_RV_PRS_HDR                       ((UINT32)0x7009)
#define TZCTL_VDEC_RV_CON_HDR                       ((UINT32)0x700a)
#define TZCTL_VDEC_FB_OPERATION						((UINT32)0x700b)
#define TZCTL_VDEC_CHECK_REALD						((UINT32)0x700c)
#define TZCTL_AU_DELIM_APPEND                       ((UINT32)0x700d)

/* SWDMX Apply MSG ID :0x8500----0x9000 */
#define TZCTL_SWDMX_MPGPARSPTS                      ((UINT32)0x8500)
#define TZCTL_SWDMX_SET_DECRYPT_SECURE_KEY          ((UINT32)0x8501)
#define TZCTL_SWDMX_DECRYPT_DATA                    ((UINT32)0x8502)
#define TZCTL_SWDMX_FREE_DECRYPT_BUF                ((UINT32)0x8503)

#define TZCTL_FBM_INITIALIZATION					((UINT32)0x9000)
#define TZCTL_QUERY_TVP_ENABLED						((UINT32)0x9001)

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------
typedef enum
{
	TZ_VDEC_FB_MEMCPY,
	TZ_VDEC_FB_MEMSET,
	TZ_VDEC_FB_FLUSHMUTILINE,
	TZ_VDEC_FB_HALFLUSH,
	TZ_VDEC_FB_GETVALUE,
	TZ_VDEC_FB_SETVALUE		
} ENUM_TZ_VDEC_FB_OPERATION_T;

typedef struct
{
    UINT32 u4Cmd;
    UINT32 u4Addr;
    UINT32 u4Size;
} TZ_SMC_ARG_T;

typedef struct
{
    UINT8 au1Buf[32];
} TZ_GET_HWSECRETKEY_T;

typedef struct
{
    UINT8 au1Buf[16];
    UINT32 u4Size;
} TZ_GET_SECURE_ID_T;

typedef struct
{
    UINT32 u4Base;
    UINT32 u4Offset;
    UINT32 u4Val;
} TZ_IO_T;

typedef struct
{
    UINT8 au1Ecm[32];
    UINT32 u4Flags;
} TZ_WV_DERIVECW_T;

typedef struct
{
    UINT32 u4Addr;
    UINT32 u4BufStart;
    UINT32 u4BufEnd;
    UINT8 *pu1Buf;
    UINT32 u4Size;
} TZ_DMX_PICHEADER_T;

typedef struct
{
    UINT32 u4Base;
    UINT32 u4Offset;
    UINT8 *pu1Buf;
    UINT32 u4Size;
} TZ_AU_DELIM_APPEND_T;

typedef struct
{
    ENUM_TZ_VDEC_FB_OPERATION_T eOperation;
    UINT32 u4SrcAddr;
    UINT32 u4DesAddr;
    UINT32 u4Size;
    UINT32 u4Value;
} TZ_FB_OPERATION_T;

typedef struct
{
    void* pvYAddr;
    void* pvCAddr;
    BOOL bIsCur2D;
    UINT8 u1QuincunxCnt;
    UINT32 u4W;
    UINT32 u4H;
    UINT32 u4W_Cmp;
    UINT32 u4H_Cmp;
    UINT32 u4UnqualifiedCnt;
} TZ_DISP_PIC_INFO_T;

typedef struct
{
    UINT8 * pu1KeyBuf;
    UINT32  u4KeyLen;
    UINT8 * pu1IvBuf;
    UINT32  u4IvLen;
}TZ_NDDRM_T;

typedef struct
{
    UINT32 u4Addr;
    UINT32 u4Size;
} TZ_DMX_SECURE_VFIFO_T;

typedef struct
{
    UINT32 u4BufStart;                  ///< Buffer start address
    UINT32 u4BufEnd;                    ///< Buffer end address
    UINT32 u4FrameAddr;                 ///< Frame (picture) address
    UINT32 u4PrevFrameAddr;             ///< Prev Frame (picture) address
    UINT32 u4PrevFrameType;             ///< Prev Frame (picture) type
    UINT32 u4Pts;                       ///< PTS
    UINT32 u4Dts;                       ///< DTS
} TZ_DMX_PES_MSG_T;

#define SEC_BUF_ANONYMOUS_SESSION		0x0
#define SEC_BUF_ANONYMOUS_HANDLE		0x0

#define SEC_BUF_FLAG_IS_CYCLIC_BUFFER	0x1 

typedef struct
{
    UINT32 u4Session;
    UINT32 u4Size;        //Size to be alloc
    UINT32 u4Flag;        //Ctrl flag
	UINT32 *u4Handle;     //return handle of the secure buf
} TZ_DRM_SEC_BUF_ALLOC_T;

typedef struct
{
    UINT32 u4Session;
	UINT32 u4Handle;
	UINT32 *u4Fragment;	
	UINT32 u4Size;
} TZ_DRM_SEC_BUF_FRAGMENT_ALLOC_T;

typedef struct
{
    UINT32 u4Session;
	UINT32 u4Handle;
	UINT32 u4Fragment;	
	UINT32 u4Size;
} TZ_DRM_SEC_BUF_FRAGMENT_FREE_T;

typedef struct
{
    UINT32 *pu4Des;
    UINT32 u4Src;        
    UINT32 *pu4Size;         
	UINT32 u4RDP;        //Read Point
} TZ_DRM_SEC_BUF_CPB_MOVEDATA_T;

#define DECRYPT_SECURE_KEY_LEN 16

typedef enum
{
    TZ_ENCRYPT_MODE_CLEAR_DATA,
    TZ_ENCRYPT_MODE_LOCAL_KEY,
    TZ_ENCRYPT_MODE_DTCP_IP_KEY,
    TZ_ENCRYPT_MODE_MAX
} TZ_ENCRYPT_MODE_T;

typedef struct
{
    BOOL fgEOS;             // reserve
    BOOL fgFrameHead;       // handle the first data in trustzone    
    UINT32 u4BufStart;
    UINT32 u4BufEnd;
    UINT32 u4StartAddr;
    UINT32 u4FrameSize;
    TZ_ENCRYPT_MODE_T       eEncryptMode;
    
}TZ_DESCRYPT_DATA_T;
#endif  // TZ_H


