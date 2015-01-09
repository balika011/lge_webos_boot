/*----------------------------------------------------------------------------*
 * Copyright Statement:                                                       *
 *                                                                            *
 *   This software/firmware and related documentation ("MediaTek Software")   *
 * are protected under international and related jurisdictions'copyright laws *
 * as unpublished works. The information contained herein is confidential and *
 * proprietary to MediaTek Inc. Without the prior written permission of       *
 * MediaTek Inc., any reproduction, modification, use or disclosure of        *
 * MediaTek Software, and information contained herein, in whole or in part,  *
 * shall be strictly prohibited.                                              *
 * MediaTek Inc. Copyright (C) 2010. All rights reserved.                     *
 *                                                                            *
 *   BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND     *
 * AGREES TO THE FOLLOWING:                                                   *
 *                                                                            *
 *   1)Any and all intellectual property rights (including without            *
 * limitation, patent, copyright, and trade secrets) in and to this           *
 * Software/firmware and related documentation ("MediaTek Software") shall    *
 * remain the exclusive property of MediaTek Inc. Any and all intellectual    *
 * property rights (including without limitation, patent, copyright, and      *
 * trade secrets) in and to any modifications and derivatives to MediaTek     *
 * Software, whoever made, shall also remain the exclusive property of        *
 * MediaTek Inc.  Nothing herein shall be construed as any transfer of any    *
 * title to any intellectual property right in MediaTek Software to Receiver. *
 *                                                                            *
 *   2)This MediaTek Software Receiver received from MediaTek Inc. and/or its *
 * representatives is provided to Receiver on an "AS IS" basis only.          *
 * MediaTek Inc. expressly disclaims all warranties, expressed or implied,    *
 * including but not limited to any implied warranties of merchantability,    *
 * non-infringement and fitness for a particular purpose and any warranties   *
 * arising out of course of performance, course of dealing or usage of trade. *
 * MediaTek Inc. does not provide any warranty whatsoever with respect to the *
 * software of any third party which may be used by, incorporated in, or      *
 * supplied with the MediaTek Software, and Receiver agrees to look only to   *
 * such third parties for any warranty claim relating thereto.  Receiver      *
 * expressly acknowledges that it is Receiver's sole responsibility to obtain *
 * from any third party all proper licenses contained in or delivered with    *
 * MediaTek Software.  MediaTek is not responsible for any MediaTek Software  *
 * releases made to Receiver's specifications or to conform to a particular   *
 * standard or open forum.                                                    *
 *                                                                            *
 *   3)Receiver further acknowledge that Receiver may, either presently       *
 * and/or in the future, instruct MediaTek Inc. to assist it in the           *
 * development and the implementation, in accordance with Receiver's designs, *
 * of certain softwares relating to Receiver's product(s) (the "Services").   *
 * Except as may be otherwise agreed to in writing, no warranties of any      *
 * kind, whether express or implied, are given by MediaTek Inc. with respect  *
 * to the Services provided, and the Services are provided on an "AS IS"      *
 * basis. Receiver further acknowledges that the Services may contain errors  *
 * that testing is important and it is solely responsible for fully testing   *
 * the Services and/or derivatives thereof before they are used, sublicensed  *
 * or distributed. Should there be any third party action brought against     *
 * MediaTek Inc. arising out of or relating to the Services, Receiver agree   *
 * to fully indemnify and hold MediaTek Inc. harmless.  If the parties        *
 * mutually agree to enter into or continue a business relationship or other  *
 * arrangement, the terms and conditions set forth herein shall remain        *
 * effective and, unless explicitly stated otherwise, shall prevail in the    *
 * event of a conflict in the terms in any agreements entered into between    *
 * the parties.                                                               *
 *                                                                            *
 *   4)Receiver's sole and exclusive remedy and MediaTek Inc.'s entire and    *
 * cumulative liability with respect to MediaTek Software released hereunder  *
 * will be, at MediaTek Inc.'s sole discretion, to replace or revise the      *
 * MediaTek Software at issue.                                                *
 *                                                                            *
 *   5)The transaction contemplated hereunder shall be construed in           *
 * accordance with the laws of Singapore, excluding its conflict of laws      *
 * principles.  Any disputes, controversies or claims arising thereof and     *
 * related thereto shall be settled via arbitration in Singapore, under the   *
 * then current rules of the International Chamber of Commerce (ICC).  The    *
 * arbitration shall be conducted in English. The awards of the arbitration   *
 * shall be final and binding upon both parties and shall be entered and      *
 * enforceable in any court of competent jurisdiction.                        *
 *---------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * $RCSfile: x_ci_storage.h,v $
 * $Revision: #1 $
 * $Date: 2015/01/09 $
 * $Author: dtvbm11 $
 * $CCRevision:$
 * $SWAuthor: $
 * $MD5HEX: $
 *
 * Description: 
 *
 *---------------------------------------------------------------------------*/
 
/*----------------------------------------------------------------------------*/
/*! @addtogroup groupMW_CI
*  @{ 
*/
/*----------------------------------------------------------------------------*/
#ifndef _X_CI_STORAGE_H
#define _X_CI_STORAGE_H

/*-------------------------------------------------------------------------
                    include files
-------------------------------------------------------------------------*/
#include "u_sys_name.h"
#include "u_ci.h"

/*-------------------------------------------------------------------------
                    define, micro, structure
-------------------------------------------------------------------------*/
extern UINT8 ciplus_storage_root_cert[];
extern UINT8 ciplus_storage_dev_cert[];
extern UINT8 ciplus_storage_brand_cert[];
extern UINT8 ciplus_storage_host_prng_key[];
extern UINT8 ciplus_storage_host_dh_g[];
extern UINT8 ciplus_storage_host_dh_p[];
extern UINT8 ciplus_storage_host_dh_q[];
extern UINT8 ciplus_storage_hdq[];
extern UINT8 ciplus_storage_host_siv[];
extern UINT8 ciplus_storage_host_slk[];
extern UINT8 ciplus_storage_host_clk[];

extern UINT16 ciplus_storage_root_cert_size;
extern UINT16 ciplus_storage_dev_cert_size;
extern UINT16 ciplus_storage_brand_cert_size;
extern UINT16 ciplus_storage_host_prng_key_size;
extern UINT16 ciplus_storage_host_dh_g_size;
extern UINT16 ciplus_storage_host_dh_p_size;
extern UINT16 ciplus_storage_host_dh_q_size;
extern UINT16 ciplus_storage_hdq_size;
extern UINT16 ciplus_storage_host_siv_size;
extern UINT16 ciplus_storage_host_slk_size;
extern UINT16 ciplus_storage_host_clk_size; 

#define CI_AUTH_CTX_OFFSET          (0x8000)
#define CI_RAW_CRED_HDR_OFFSET      ((UINT16)4)
#define CI_RAW_CRED_TAG_SIZE        ((UINT16)1)
#define CI_RAW_CRED_ITEM_LEN_SIZE   ((UINT16)2)
#define SAH1_DIGEST_LEN             ((UINT16)20)
#define CI_MAX_KEY_PACK             (0x8000)
#define CI_DEVICE_ID_SIZE           ((UINT16)16)

#define CIPLUS_KEY_SUPPORT_SERIAL_NUMBER 
#define CI_PLUS_AKH_PATH   "/perm/ciplus_akh"
#define CI_PLUS_KEY_RAW_MAX_SIZE   (6*1024)


/*-------------------------------------------------------------------------
                    Functions
 ------------------------------------------------------------------------*/

extern VOID x_ci_sha1 (UINT8 *pi1_buffer, UINT32 ui4_length, UINT8 *pui1_digest);

extern INT32 x_ci_update_ciplus_cred(CHAR *pc_src_file_path_in);

extern INT32 x_ci_erase_ciplus_cred(VOID);

extern INT32 x_ci_erase_ciplus_precred(VOID);

extern INT32 x_ci_write_ciplus_pre_cred(CHAR *aui1_src_buffer, UINT32 ui4_len);

extern INT32 x_ci_read_ciplus_pre_cred(CHAR *aui1_dst_buffer, UINT32 *ui4_len);

extern INT32 x_ci_activate_ciplus_cred(VOID);

extern INT32 x_ci_get_ciplus_cred_info(CI_PLUS_CRED_INFO_T *t_info_data);

extern INT32 x_ci_get_ciplus_cred_pre_info(CI_PLUS_CRED_INFO_T *t_info_data);

extern INT32 x_ci_secure_storage_mem_encryption(CHAR *puc_deviceId, CHAR *puc_deviceData,
                                                  CHAR *pc_src_buf,INT32 l_src_len,
                                                  CHAR *pc_dst_buf, INT32 *pl_dst_len);

extern INT32 x_ci_secure_storage_mem_decryption(CHAR *puc_deviceId, CHAR *puc_deviceData,
                                                  CHAR *pc_src_buf, INT32 l_src_len,
                                                  CHAR *pc_dst_buf, INT32 *pl_dst_len);
extern INT32 x_ci_read_device_random_constant(UINT8 *aui1_data, UINT8 ui1_len);

extern INT32 x_ci_aes128cbc_decrypt(UINT8 *pui1_src_buffer,
                                 UINT32 ui4_src_length,
                                 UINT8 *pui1_dst_buffer,
                                 UINT32 *ui4_dst_length,                                
                                 UINT8 *pui1_key,
                                 UINT8 *pui1_iv);
extern INT32 _ci_storage_read_file_and_decr(UINT8* pui1_data_buffer,UINT16*pui2_dec_len,UINT8* pui1_serial_buffer,UINT16*pui2_serail_len);



#endif /* _X_CI_STORAGE_H */
/*----------------------------------------------------------------------------*/
/*! @} */   /* end of  groupMW_CI*/
/*----------------------------------------------------------------------------*/
