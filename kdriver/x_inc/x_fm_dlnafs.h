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
#ifndef _X_FM_DLNAFS_H_
#define _X_FM_DLNAFS_H_

#include "u_fm_dlnafs.h"


/*-----------------------------------------------------------------------------
                    include files
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
-----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
                    data declarations
 ----------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
                    functions declarations
 ----------------------------------------------------------------------------*/
INT32 x_fm_dlna_dms_init(VOID*   pv_data);

INT32 x_dlna_dmp_fm_get_media_info(CHAR *ps_filename, DLNA_MEDIA_INFO_T *pt_info);

INT32 x_fm_dlna_orderby(CHAR * ps_filename, DLNA_ORDERBY_TYPE_T e_orderby, BOOL b_override);

INT32  x_fm_dlna_set_browse_recursive(CHAR* ps_filename, BOOL b_recursive);

INT32  x_fm_dlna_set_browse_file_ext(CHAR * ps_filename, CHAR * ps_ext_name);

INT32  x_fm_dlna_set_content_filter(CHAR * ps_filename, DLNA_FILE_CONTENT_TYPE_T e_browse_type);

BOOL x_fm_dlna_is_folder_sorting_supported(CHAR * ps_filename);

BOOL x_fm_dlna_is_folder_changed(CHAR * ps_dir_name);

CHAR * x_fm_dlna_dmr_get_file_path(CHAR* ps_uri, CHAR* ps_protocol);

extern BOOL x_fm_dlna_is_pause_supported(CHAR * ps_media_name);

BOOL x_fm_dlna_is_multiconnection_supported(CHAR * ps_media_name);


/**
 * Get DLNA file seek type
 * There are two seek method in DLNA: Byte-based and Time-based seek
 * Byte-based seek: Seek offset is measured by byte - x_fm_lseek
 * Time-based seek: Seek offset is measured by time (second) - x_fm_dlna_time_seek
 * DLNA file may support only one of them or both of them
 *
 * @param ps_filepath, file full path
 * @param pt_seek, seek ablity
 *
 * @return INT32
 * FMR_OK, ok
 * 
*/

INT32 x_fm_dlna_get_seek_type(const CHAR *ps_filepath, DLNAFS_SEEK_CAP_T * pt_seek);

/**
 * Seek current file access time position as user specified
 * 
 * @param i4_fd, file handle
 * @param i8_time_start, time seek start time, eg. 5s
 * @param i8_time_length, time seek length, eg. 60s, or 0 (to the end of file). It's time length, not end time.
 * 
 * @return INT32
 * FMR_OK, ok
 * FMR_HANDLE, Fail to get the handle object.
 * FMR_INVAL, error
 */

INT32 x_fm_dlna_time_seek(HANDLE_T h_file, UINT64 ui8_time_start, UINT64 ui8_time_length);

/**
 * Check if Connection-Stalling is supported for this file
 * if result is true, that is good, no need to do anything
 * if result is false, we need to close file handle during pause and slow forward operation, then open a new handle when resume
 *
 * @param ps_filepath, file full path
 * @param pb_stalling, result
 *
 * @return INT32
 * FMR_OK, ok
 * 
*/

INT32 x_fm_dlna_is_stalling_supported(const CHAR *ps_filepath, BOOL *pb_stalling);

#endif /* _X_FM_DLNAFS_H_ */
