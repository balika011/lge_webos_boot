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
 * $Author: dtvbm11 $
 * $Date: 2015/01/09 $
 * $RCSfile: tz_tee.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file tz_tee.h
 *  Demux driver - common definitions
 */


#ifndef TZ_TEE_H
#define TZ_TEE_H

#define TZCTL_TEE_BASE                              ((UINT32)0x6000)
#define TZCTL_TEE_SECURESTORAGE_BASE                ((UINT32)0x6100)
#define TZCTL_TEE_SECURESTORAGE_M2M_ENC             ((UINT32)0x6101)
#define TZCTL_TEE_SECURESTORAGE_M2M_DEC             ((UINT32)0x6102)
#define TZCTL_TEE_NFLX_BASE                         ((UINT32)0x6200)
#define TZCTL_TEE_NFLX_READ_IDFILE                  ((UINT32)0x6201)
#define TZCTL_TEE_NFLX_KPE_CIPHER_ENCRYPT           ((UINT32)0x6202)
#define TZCTL_TEE_NFLX_KPE_CIPHER_DECRYPT           ((UINT32)0x6203)
#define TZCTL_TEE_NFLX_KPH_HMAC                     ((UINT32)0x6204)
#define TZCTL_TEE_NFLX_KCE_CIPHER_ENCRYPT           ((UINT32)0x6205)
#define TZCTL_TEE_NFLX_KCE_CIPHER_DECRYPT           ((UINT32)0x6206)
#define TZCTL_TEE_NFLX_KCH_HMAC                     ((UINT32)0x6207)
#define TZCTL_TEE_NFLX_DH_PUBKEY_GEN                ((UINT32)0x6208)
#define TZCTL_TEE_NFLX_DH_SECRET2KCEKCH             ((UINT32)0x6209)
#define TZCTL_TEE_NFLX_GET_RANDOM_BYTES             ((UINT32)0x620a)
#define TZCTL_TEE_NFLX_DH_PUBKEY_GEN_V2             ((UINT32)0x620b)
#define TZCTL_TEE_NFLX_DH_SECRET2KCEKCH_V2          ((UINT32)0x620c)
#define TZCTL_TEE_PLAYREADY_BASE                    ((UINT32)0x6300)
#define TZCTL_TEE_PLAYREADY_PROVISIONED_DATA_GET    ((UINT32)0x6301)
#define TZCTL_TEE_PLAYREADY_ECCP256_KEYPAIR_GEN     ((UINT32)0x6302)
#define TZCTL_TEE_PLAYREADY_ECCP256_KEY_SET         ((UINT32)0x6303)
#define TZCTL_TEE_PLAYREADY_ECDSAP256_SIGN          ((UINT32)0x6304)
#define TZCTL_TEE_PLAYREADY_ECCP256_DECRYPT         ((UINT32)0x6305)
#define TZCTL_TEE_PLAYREADY_OMAC1_KEY_SET           ((UINT32)0x6306)
#define TZCTL_TEE_PLAYREADY_OMAC1_VERIFY            ((UINT32)0x6307)
#define TZCTL_TEE_PLAYREADY_OMAC1_SIGN              ((UINT32)0x6308)
#define TZCTL_TEE_PLAYREADY_COPYBYTE                ((UINT32)0x6309)
#define TZCTL_TEE_PLAYREADY_CONTENTKEY_AESCTR_SET   ((UINT32)0x630a)
#define TZCTL_TEE_PLAYREADY_CONTENT_AESCTR_DECRYPT  ((UINT32)0x630b)
#define TZCTL_TEE_PLAYREADY_AESECB_KEY_SET          ((UINT32)0x630c)
#define TZCTL_TEE_PLAYREADY_AESECB_ENCRYPT          ((UINT32)0x630d)
#define TZCTL_TEE_PLAYREADY_AESECB_DECRYPT          ((UINT32)0x630e)
#define TZCTL_TEE_PLAYREADY_MULTI_CONTENT_AESCTR_DECRYPT    ((UINT32)0x630f)
#define TZCTL_TEE_PLAYREADY_CONTENTKEY_AESCTR_DMX_SET       ((UINT32)0x6310)
#define TZCTL_TEE_PLAYREADY_GET_KFKEY      ((UINT32)0x6311)
#define TZCTL_TEE_PLAYREADY_AESCBC_KEY_SET          ((UINT32)0x6312)
#define TZCTL_TEE_PLAYREADY_AESCBC_ENCRYPT          ((UINT32)0x6313)
#define TZCTL_TEE_PLAYREADY_AESCBC_DECRYPT          ((UINT32)0x6314)
#define TZCTL_TEE_PLAYREADY_UNSHUFFLE_CONTENTKEY    ((UINT32)0x6315)
#define TZCTL_TEE_PLAYREADY_PREPARECONTENTKEY_PRIME ((UINT32)0x6316)
#define TZCTL_TEE_PLAYREADY_LINKBYTE ((UINT32)0x6317)
#define TZCTL_TEE_PLAYREADY_DASH_SAMPLE_AESCTR_DECRYPT ((UINT32)0x6320)
#define TZCTL_TEE_NAUTILUS_CONTENTKEY_AESCTR_SET ((UINT32)0x6321)
#define TZCTL_TEE_PLAYREADY_DASH_SAMPLE_AESCTR_DECRYPT_AUDIO ((UINT32)0x6322)
#define TZCTL_TEE_NAUTILUS_AUDIOCONTENTKEY_AESCTR_SET ((UINT32)0x6323)
#define TZCTL_TEE_PLAYREADY_AUDIOCONTENTKEY_AESCTR_SET ((UINT32)0x6324)

#if 1 //antirollback_feature
#define TZCTL_TEE_PLAYREADY_ANTIROLLBACK_LOADIN ((UINT32)0x6331)
#define TZCTL_TEE_PLAYREADY_ANTIROLLBACK_UPDATE ((UINT32)0x6332)
#define TZCTL_TEE_PLAYREADY_ANTIROLLBACK_GETTIME ((UINT32)0x6333)

#endif


#define TZCTL_TEE_PLAYREADY_SECURESTOP_INITSS                         ((UINT32)0x6360)
#define TZCTL_TEE_PLAYREADY_SECURESTOP_GETSESSION                        ((UINT32)0x6361)
#define TZCTL_TEE_PLAYREADY_SECURESTOP_GETSESSIONBYKID                        ((UINT32)0x6362)
#define TZCTL_TEE_PLAYREADY_SECURESTOP_ADDSESSION                        ((UINT32)0x6363)
#define TZCTL_TEE_PLAYREADY_SECURESTOP_CANCELSESSION                        ((UINT32)0x6364)
#define TZCTL_TEE_PLAYREADY_SECURESTOP_BINDLICENSE                        ((UINT32)0x6365)
#define TZCTL_TEE_PLAYREADY_SECURESTOP_BINDSESSION                        ((UINT32)0x6366)
#define TZCTL_TEE_PLAYREADY_SECURESTOP_REMOVESESSION                        ((UINT32)0x6367)
#define TZCTL_TEE_PLAYREADY_SECURESTOP_RESETSESSION                        ((UINT32)0x6368)
#define TZCTL_TEE_PLAYREADY_SECURESTOP_WRITE_SECURESTOP                        ((UINT32)0x6369)
#define TZCTL_TEE_PLAYREADY_SECURESTOP_INIT_SECURESTOPS                        ((UINT32)0x636a)
#define TZCTL_TEE_PLAYREADY_SECURESTOP_GET_SECURESTOPS                        ((UINT32)0x636b)
#define TZCTL_TEE_PLAYREADY_SECURESTOP_GET_SECURESTOP_IDS                        ((UINT32)0x636c)
#define TZCTL_TEE_PLAYREADY_SECURESTOP_GET_SECURESTOP                        ((UINT32)0x636d)
#define TZCTL_TEE_PLAYREADY_SECURESTOP_UPDATE_SECURESTOP                        ((UINT32)0x636e)
#define TZCTL_TEE_PLAYREADY_SECURESTOP_COMMIT_SECURESTOP                        ((UINT32)0x636f)
#define TZCTL_TEE_PLAYREADY_SECURESTOP_RESET_SECURESTOPS                        ((UINT32)0x6370)
#define TZCTL_TEE_PLAYREADY_SECURESTOP_UNLOADSECRETKEY                             ((UINT32)0x6371)








#define TZCTL_TEE_DDI_BASE                          ((UINT32)0x6400)
#define TZCTL_TEE_DDI_PREENC_A1_DEC                 ((UINT32)0x6401)
#define TZCTL_TEE_DDI_PREENC_A2_DEC                 ((UINT32)0x6402)
#define TZCTL_TEE_ULI_OBJECT_DECRYPT                ((UINT32)0x6500)
#define TZCTL_TEE_ULI_CHK_POTK                      ((UINT32)0x6501)

#define TZCTL_TEE_CIPLUS_BASE                       ((UINT32)0x6600)
#define TZCTL_TEE_CIPLUS_EXTRACT_CRED               ((UINT32)0x6601)
#define TZCTL_TEE_CIPLUS_GET_ENCRY_AKH              ((UINT32)0x6602)
#define TZCTL_TEE_CIPLUS_SET_ENCRY_AKH              ((UINT32)0x6603)

#define TZCTL_TEE_CIPLUS_DRM_INIT                   ((UINT32)0x6604)
#define TZCTL_TEE_CIPLUS_DRM_REMOVE                 ((UINT32)0x6605)
#define TZCTL_TEE_CIPLUS_DRM_GET_HostDevCert        ((UINT32)0x6606)
#define TZCTL_TEE_CIPLUS_DRM_Get_HostBrandCert      ((UINT32)0x6607)
#define TZCTL_TEE_CIPLUS_DRM_Verify_ModuleCert      ((UINT32)0x6608)
#define TZCTL_TEE_CIPLUS_DRM_Sign_Message           ((UINT32)0x6609)
#define TZCTL_TEE_CIPLUS_DRM_Verify_SignatureB      ((UINT32)0x660a)
#define TZCTL_TEE_CIPLUS_DRM_Get_Dhph               ((UINT32)0x660b)
#define TZCTL_TEE_CIPLUS_DRM_Get_HostId             ((UINT32)0x660c)
#define TZCTL_TEE_CIPLUS_DRM_Get_CamId              ((UINT32)0x660d)
#define TZCTL_TEE_CIPLUS_DRM_Get_CamBrandId         ((UINT32)0x660e)
#define TZCTL_TEE_CIPLUS_DRM_Generate_Akh           ((UINT32)0x660f)
#define TZCTL_TEE_CIPLUS_DRM_Generate_DHSK          ((UINT32)0x6610)
#define TZCTL_TEE_CIPLUS_DRM_Generate_SacKeys       ((UINT32)0x6611)
#define TZCTL_TEE_CIPLUS_DRM_Get_Ns                 ((UINT32)0x6612)
#define TZCTL_TEE_CIPLUS_DRM_Encrypt_MessageSac     ((UINT32)0x6613)
#define TZCTL_TEE_CIPLUS_DRM_Decrypt_MessageSac     ((UINT32)0x6614)
#define TZCTL_TEE_CIPLUS_DRM_GET_CCKId              ((UINT32)0x6615)
#define TZCTL_TEE_CIPLUS_DRM_Data_Confirm           ((UINT32)0x6616)
#define TZCTL_TEE_CIPLUS_DRM_Get_NextAuthContext    ((UINT32)0x6617)
#define TZCTL_TEE_CIPLUS_DRM_UpdateAuthContext      ((UINT32)0x6618)
#define TZCTL_TEE_CIPLUS_VERIFY_CRED                ((UINT32)0x6619)
#define TZCTL_TEE_CIPLUS_CHECK_CRED_BY_CUSTOMER_CODE ((UINT32)0x661a)
#define TZCTL_TEE_CIPLUS_DRM_Set_UTC_time         ((UINT32)0x661b)

#define TZCTL_TEE_DTCP_HCI_ENCRYPT                  ((UINT32)0x6700)
#define TZCTL_TEE_DTCP_HCI_DECRYPT                  ((UINT32)0x6701)
#define TZCTL_TEE_DTCP_HCI_ENCRYPTSWAP              ((UINT32)0x6702)
#define TZCTL_TEE_DTCP_SETLOCALKEY                  ((UINT32)0x6703)
#define TZCTL_TEE_DTCP_DECRYPTHDD                   ((UINT32)0x6704)
#define TZCTL_TEE_DTCP_ENCRYPTSWAPHDD               ((UINT32)0x6705)

#define TZCTL_TEE_NFLX_MSL_BASE						((UINT32)0x6800)
#define TZCTL_TEE_NFLX_MSL_STOREIDFILE				((UINT32)0x6801)	


#endif  // TZ_TEE_H

