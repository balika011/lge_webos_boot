/********************************************************************************************
 *     LEGAL DISCLAIMER 
 *
 *     (Header of MediaTek Software/Firmware Release or Documentation)
 *
 *     BY OPENING OR USING THIS FILE, BUYER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES 
 *     THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE") RECEIVED 
 *     FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO BUYER ON AN "AS-IS" BASIS 
 *     ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES, EXPRESS OR IMPLIED, 
 *     INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR 
 *     A PARTICULAR PURPOSE OR NONINFRINGEMENT. NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY 
 *     WHATSOEVER WITH RESPECT TO THE SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, 
 *     INCORPORATED IN, OR SUPPLIED WITH THE MEDIATEK SOFTWARE, AND BUYER AGREES TO LOOK 
 *     ONLY TO SUCH THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. MEDIATEK SHALL ALSO
 *     NOT BE RESPONSIBLE FOR ANY MEDIATEK SOFTWARE RELEASES MADE TO BUYER'S SPECIFICATION 
 *     OR TO CONFORM TO A PARTICULAR STANDARD OR OPEN FORUM.
 *     
 *     BUYER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND CUMULATIVE LIABILITY WITH 
 *     RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE, AT MEDIATEK'S OPTION, 
 *     TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE, OR REFUND ANY SOFTWARE LICENSE 
 *     FEES OR SERVICE CHARGE PAID BY BUYER TO MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE. 
 *     
 *     THE TRANSACTION CONTEMPLATED HEREUNDER SHALL BE CONSTRUED IN ACCORDANCE WITH THE LAWS 
 *     OF THE STATE OF CALIFORNIA, USA, EXCLUDING ITS CONFLICT OF LAWS PRINCIPLES.  
 ************************************************************************************************/
/*----------------------------------------------------------------------------*
 * $RCSfile: fm_mtpfs_linux.c,v $
 * $Revision:
 * $Date:
 * $SWAuthor: Yan Wang $
 *
 * Description: 
 *          This header file contains mtp device structure.
 *---------------------------------------------------------------------------*/

#ifndef _X_FM_MTPFS_LINUX_H_
#define _X_FM_MTPFS_LINUX_H_

/*-----------------------------------------------------------------------------
                    include files
-----------------------------------------------------------------------------*/
#include "libmtp.h"
/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    data declarations
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    functions declarations
 ----------------------------------------------------------------------------*/
extern UINT32 is_mtp_obj_path(
    HANDLE_T        h_dir,
    const CHAR      *ps_path);

extern UINT32 is_mtp_obj(
    HANDLE_T  h_file);

extern UINT32 is_mtp_mdb_obj(
    HANDLE_T  h_file);

extern UINT32 is_mtp_dir_lbl(
    HANDLE_T h_dir);

extern INT32 x_fm_mtp_init(
    VOID*   pv_data);

extern INT32 x_fm_mtp_uninit(VOID);

extern VOID  x_fm_mtp_regist(
    LIBMTP_mtpdevice_t   *pt_mtp_device);

extern INT32 x_fm_mtp_fs_info(
    HANDLE_T h_dir,
    const CHAR * ps_path,
    FM_FS_INFO_T * pt_fs_info);

extern INT32 x_mtpfm_open(
    HANDLE_T        h_dir,
    const CHAR      *ps_path,
    UINT32          ui4_flags,
    UINT32          ui4_mode,
    BOOL            b_ext_buf,
    HANDLE_T        *ph_file);

extern INT32 x_mtpfm_close(
    HANDLE_T        h_file);

extern INT32 x_mtpfm_read(
    HANDLE_T        h_file,
    VOID            *pv_data,
    UINT32          ui4_count,
    UINT32          *pui4_read);

extern INT32 x_mtpfm_feof(
    HANDLE_T        h_file,
    BOOL            *pb_eof);

extern INT32 x_mtpfm_read_async(
    HANDLE_T        h_file,
    VOID            *pv_data,
    UINT32          ui4_count,
    UINT8           ui1_pri,
    x_fm_async_fct  pf_nfy_fct,
    VOID            *pv_tag,
    HANDLE_T        *ph_req);

extern INT32 x_mtpfm_abort_async(
    HANDLE_T h_req);

extern INT32 x_mtpfm_lseek(
    HANDLE_T        h_file,
    INT64           i8_offset,
    UINT8           ui1_whence,
    UINT64          *pui8_cur_pos);

extern INT32 x_mtpfm_get_object_info_by_name(
    HANDLE_T        h_dir,
    const CHAR      *ps_path,
    FM_FILE_INFO_T  *pt_info);

extern INT32 x_mtpfm_get_object_info_by_handle(
    HANDLE_T        h_file,
    FM_FILE_INFO_T  *pt_info);


// for DLL init
typedef struct
{
    INT32 (*mtpfs_init) (VOID* pv_data);
    INT32 (*mtpfs_uninit) (VOID);
    
    INT32 (*x_mtpfm_open)(  HANDLE_T        h_dir,
                            const CHAR      *ps_path,
                            UINT32          ui4_flags,
                            UINT32          ui4_mode,
                            BOOL            b_ext_buf,
                            HANDLE_T        *ph_file);
                            
    INT32 (*x_mtpfm_close)( HANDLE_T        h_file);
    
    INT32 (*x_mtpfm_read)(  HANDLE_T        h_file,
                            VOID            *pv_data,
                            UINT32          ui4_count,
                            UINT32          *pui4_read);
                            
    INT32 (*x_mtpfm_read_async)(
                            HANDLE_T        h_file,
                            VOID            *pv_data,
                            UINT32          ui4_count,
                            UINT8           ui1_pri,
                            x_fm_async_fct  pf_nfy_fct,
                            VOID            *pv_tag,
                            HANDLE_T        *ph_req);
                            
    INT32 (*x_mtpfm_abort_async)(HANDLE_T h_req);
    
    INT32 (*x_mtpfm_lseek)(HANDLE_T        h_file,
                            INT64           i8_offset,
                            UINT8           ui1_whence,
                            UINT64          *pui8_cur_pos);
                            
    INT32 (*x_mtpfm_get_object_info_by_name)(
                            HANDLE_T        h_dir,
                            const CHAR      *ps_path,
                            FM_FILE_INFO_T  *pt_info);  
                            
    INT32 (*x_mtpfm_get_object_info_by_handle)(
                            HANDLE_T        h_file,
                            FM_FILE_INFO_T  *pt_info);
    
} MTPFS_IF_T;

extern INT32 x_mtpfm_set_dir_path(
    HANDLE_T        h_dir,
    const CHAR      *ps_path,
    HANDLE_T        *ph_dir);

extern INT32 x_mtpfs_get_if(VOID **pv_interface);


#endif /* _X_FM_MTPFS_LINUX_H_ */

