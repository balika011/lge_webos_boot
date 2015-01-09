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
 * $RCSfile: x_dlm.h,v $
 * $Revision: #1 $
 * $Date: 2015/01/09 $
 * $Author: dtvbm11 $
 * $CCRevision:  $
 * $SWAuthor:  $
 * $MD5HEX:  $
 *
 * Description:
 *---------------------------------------------------------------------------*/

#ifndef X_DLM_H
#define X_DLM_H

#include "u_dlm.h"

INT32       x_dlm_init(VOID);
VOID        x_dlm_release(VOID);
INT32       x_dlm_reg_hdr_callback(dlm_hdr_parse_fn fn, UINT32 ui4_hdr_len);
INT32       x_dlm_reg_append_callback(UCHAR *pc_type, UINT32 ui4_buf_size, dlm_append_data_fn fn);
INT32	    x_dlm_reg_append_callback2(UCHAR *pc_type, UINT32 ui4_buf_size, dlm_append_data_fn2 fn);
VOID        x_dlm_reg_version_compare_callback(dlm_ver_chk_fn p_fn);
INT32       x_dlm_unreg_append_callback(UCHAR *pc_type);
INT32        x_dlm_is_version_validate(VOID);
INT32       x_dlm_integrity_check(CHAR *filename, DLM_IO_T *pt_user_io);
INT32       x_dlm_download(CHAR *filename, DLM_IO_T *pt_user_io, INT32 i4_check);
INT32       x_dlm_upload(CHAR *pac_tag,DLM_IO_T *pt_user_io, CHAR *ps_path);
DLM_IO_T*   x_dlm_get_default_io(VOID);
INT32       x_dlm_register_progress_notify(dlm_progress_notify_fn fn);
INT32       x_dlm_register_tag_progress_notify(dlm_progress_notify_fn fn);
INT32       x_dlm_boot_init(CHAR *pac_hdr);
CHAR       *x_dlm_get_download_hdr(VOID);
INT32       x_dlm_set_buffer(UINT32 ui4_k_bytes);
INT32       x_dlm_set_ignore_check_item(UCHAR uc_flag);
Append_T   *x_dlm_get_codefile_tag_list(CHAR *ps_filepath);

INT32       x_dlm_download_by_tag(CHAR *ps_file, UCHAR *puc_tag, DLM_IO_T *pt_user_io);
INT32       x_dlm_download_by_division(UINT32 ui4_len, UCHAR*  puc_buffer, UCHAR* puc_tag);
INT32       x_dlm_tag_test(CHAR *ps_file, UCHAR *puac_tag, DLM_IO_T *pt_user_io, CHAR *pac_buffer, INT32 i4_buf_size);
INT32        x_dlm_set_rsa_public_key(CHAR *ps_file);
INT32        x_dlm_set_rsa_private_key(CHAR *ps_file);
INT32        x_dlm_set_aes_key_iv(VOID* key, INT32 key_len, VOID* iv, INT32 iv_len);

INT32        x_dlm_set_customer_key(UINT32* key);
INT32        x_dlm_set_customer_aes_key(UCHAR* key, INT32 key_len, UCHAR* iv, INT32 iv_len);

#endif
