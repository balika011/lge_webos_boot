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
 * $RCSfile: x_mdb_fs.h,v $
 * $Revision: #1 $
 * $Date: 2015/01/09 $
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_HQ_int/DTV_X_ATSC/2 $
 * $SWAuthor: Bin.liu $
 * $MD5HEX: a72575edb609b7b51f412d6003e7428c $
 *
 * Description: 
 *          This file implements the simulated MDB file system.
 *       
 *---------------------------------------------------------------------------*/

#ifndef X_MDB_MDFS
#define X_MDB_MDFS
#include "x_os.h"
#include "u_mdb.h"
#include "u_mfs.h"

#ifdef ASSERT
#undef ASSERT
#define ASSERT(cond)                                                    \
{                                                                       \
    if (!(cond))                                                        \
    {                                                                   \
        DBG_LOG_PRINT(("[mtpfs] ERR: %s(%u): Assert %s", __FUNCTION__, __LINE__, #cond)); \
        DBG_ASSERT((cond), ~0);                                         \
    }                                                                   \
}
#endif

extern INT32  x_mfs_init(const CHAR *ps_root_path, HANDLE_T h_mdb);

extern INT32  x_mfs_unit(const CHAR *ps_root_path);

extern INT32 x_mfs_open_dir(const CHAR *ps_path,HANDLE_T *ph_dir);
    
extern INT32  x_mfs_seek_dir(HANDLE_T h_dir,UINT8 ui1_whence,INT32  i4_ent_off);

extern INT32 x_mfs_read_dir_entries(
                                                        HANDLE_T        h_dir,
                                                        MFS_DIR_ENTRY_T  *pt_dir_entry,
                                                        UINT32         ui4_count,
                                                        UINT32        *pui4_entries);

extern INT32 x_mfs_get_dir_info(
                                                        const CHAR  *ps_path,
                                                        MFS_DIR_INFO_T *pt_info);

extern INT32 x_mfs_dir_close(HANDLE_T h_file);


typedef struct
{
    INT32 (*x_mfs_init)(const CHAR *ps_root_path, HANDLE_T h_mdb);
    INT32 (*x_mfs_unit)(const CHAR *ps_root_path);
    INT32 (*x_mfs_open_dir)(const CHAR *ps_path,HANDLE_T  *ph_dir);
    INT32 (*x_mfs_seek_dir)(HANDLE_T h_dir,UINT8 ui1_whence,INT32  i4_ent_off);
    INT32 (*x_mfs_read_dir_entries)(
                                                        HANDLE_T        h_dir,
                                                        MFS_DIR_ENTRY_T  *pt_dir_entry,
                                                        UINT32         ui4_count,
                                                        UINT32        *pui4_entries);
    INT32 (*x_mfs_get_dir_info)(
                                                        const CHAR  *ps_path,
                                                        MFS_DIR_INFO_T *pt_info);
    INT32 (*x_mfs_dir_close)(HANDLE_T h_file);


} MFS_IF_T;

#endif //X_MDB_MDFS
