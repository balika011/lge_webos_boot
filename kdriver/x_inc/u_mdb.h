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
/*----------------------------------------------------------------------------*/
/*! @file u_mdb.h 
 *  $RCSfile: u_mdb.h,v$
 *  $Revision: #1 $
 *  $Date: 2015/01/09 $
 *  $Author: dtvbm11 $
 *  
 *  @par Description:
 *         This file contains File Manager exported API's.
 */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*! @defgroup groupMW_MTP_MNGR_MDB Multimedia Transfer Protocol Database Builder
 *  @ingroup groupMW_MTP_MNGR
 *  @brief Provides a unified interface for MDB
 *  %This module provides query functionality, properties collection function for applications 
 *   and provide application a flexible way to configure directory appearance in File manager.
 *   - including these sub modules:
 *  @see groupMW_MTP_MNGR
 *  @{ 
 */
/*----------------------------------------------------------------------------*/

#ifndef _U_MBD_H_
#define _U_MBD_H_

#include "x_os.h"
#include "u_mtp.h"
#include "x_common.h"

#define MDB_STR_CPY(A, B) x_strncpy((CHAR *)A, (const CHAR *)B,(strlen((const CHAR *)B) +1))
#define MDB_STR_CPY_EX(A, B, maxLen) \
{\
   CHAR *pcDest = NULL;\
   UINT32 u4StrLen =0;\
   pcDest = (CHAR *)A;\
   u4StrLen = strlen((const CHAR *)B);\
   if(u4StrLen> maxLen -1)\
   {\
        x_strncpy((CHAR *)A, (const CHAR *)B, (maxLen -1));\
        pcDest[maxLen -1] =0;\
   }\
   else\
   {\
      x_strncpy((CHAR *)A, (const CHAR *)B, u4StrLen);\
      pcDest[u4StrLen] =0;\
   }\
}
/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
-----------------------------------------------------------------------------*/

/************************************************************************/
/* Error code define                                                    */
/************************************************************************/
#define MDBR_OK             ((INT32)    0)
#define MDBR_INV_HANDLE     ((INT32)   -1)
#define MDBR_ALREADY_EXIST  ((INT32)   -2)
#define MDBR_OUT_OF_HANDLE  ((INT32)   -3)
#define MDBR_OUT_OF_MEMORY  ((INT32)   -4)
#define MDBR_NOT_SUPPORT    ((INT32)   -5)
#define MDBR_CORE           ((INT32)   -6)
#define MDBR_INV_ARG        ((INT32)   -7)
#define MDBR_PARAMETER_ERROR    ((INT32)   -8)
#define MDBR_SERIOUS_ERROR      ((INT32)   -9)
#define MDBR_NULL_POINT         ((INT32)   -10)
/* for virtual tree error code */
#define MDBR_INVALID_TREE       ((INT32)   -11)
#define MDBR_INVALID_NODE       ((INT32)   -12)
#define MDBR_INVALID_NODE_NAME  ((INT32)   -13)
#define MDBR_INVALID_NODE_CODE  ((INT32)   -14)

/************************************************************************/
/* Handle related define                                                */
/************************************************************************/
typedef UINT32 MDB_ITEM_HND_T;
typedef UINT32 MDB_NODE_HND_T;
typedef UINT32 MDB_OBJ_HND_T;
/*
 * MDB_ITEM_HANDLE Define
 *
 *        31-30        29 - 24            23 - 16                16 - 0
 *   FOLDER or FILE   Reserved     query condition index      item index.
 */

#define MDB_HND_NONE               0

#define MDB_MODE_TYPE_MASK       ((UINT32) 0xC0000000)
#define MDB_MODE_FOLDER_MASK     ((UINT32) 0x90000000)
#define MDB_MODE_QUERY_MASK      ((UINT32) 0x00FF0000)
#define MDB_MODE_ITEM_MASK       ((UINT32) 0x0000FFFF)
#define MDB_MODE_FILE_MASK       ((UINT32) 0x00FFFFFF)

#define MDB_MODE_TYPE_FOLDER_DIR ((UINT32) 0x90000000)
#define MDB_MODE_TYPE_NODE       ((UINT32) 0x20000000)
#define MDB_MODE_TYPE_FILE       ((UINT32) 0x40000000)
#define MDB_MODE_TYPE_DIR        ((UINT32) 0x80000000)

#define MDB_MODE_FOLDER_IDX(handle) ((handle) & MDB_MODE_FILE_MASK)
#define MDB_MODE_QUERY_IDX(handle)  (((handle) & MDB_MODE_QUERY_MASK) >> 16)
#define MDB_MODE_ITEM_IDX(handle)   ((handle) & MDB_MODE_ITEM_MASK)

#define IS_MDB_HANDLE_FILE(handle)     \
    (((handle) & MDB_MODE_TYPE_MASK) == MDB_MODE_TYPE_FILE)

#define IS_MDB_HANDLE_DIR(handle)      \
    (((handle) & MDB_MODE_TYPE_MASK) == MDB_MODE_TYPE_DIR)

#define IS_MDB_FOLDER_DIR(handle)      \
    (((handle) & MDB_MODE_FOLDER_MASK) == MDB_MODE_TYPE_FOLDER_DIR)

#define MDB_MAKE_FILE_HANDLE(idx)    \
    (MDB_MODE_TYPE_FILE | (idx & MDB_MODE_FILE_MASK))

#define MDB_MAKE_FOLDER_HANDLE(idx)    \
    (MDB_MODE_TYPE_FOLDER_DIR | (idx & MDB_MODE_FILE_MASK))

#define MDB_NEW_HANDLE(type,query,item) \
    (type | (((UINT32)query << 16) & MDB_MODE_QUERY_MASK) \
          | (item & MDB_MODE_ITEM_MASK))

#define MDB_ROOT_NODE_HANDLE  MDB_NEW_HANDLE(MDB_MODE_TYPE_NODE,0,0)

/* virtual tree limit define */
#define MDB_QUERY_PROPERTY_MAX 5
#define MDB_VIRTUAL_TREE_DEPTH 10
#define MDB_VIRTUAL_TREE_WIDTH 10
#define MDB_VIRTUAL_TREE_NODE_MAX MDB_VIRTUAL_TREE_DEPTH * MDB_VIRTUAL_TREE_WIDTH

/* database limit define */
#define MDB_CATEGORY_SUPPORT_MAX       4
#define MDB_ONE_CATEGORY_FORMATS_MAX   50
#define MDB_PROPERTY_SUPPORT_MAX       10
#define MDB_ITEM_STR_LEN_MAX           255
#define MDB_ITEM_QUERY_MAX_ONE_TIME    100

#define MDB_HANDLE_NAME_LEN   8

#define MDB_INVALID_CATEGORY_IDX       0xFF

/*------------------------------------------------------------------*/
/*! @enum MDB_PROPERTY_CODE_T
 *  @brief  This enumeration specifies the MDB virtual tree property code define
 *  @code
 *  typedef enum
 *  {
 *      MDB_INVALID_PROPERTY_CODE,
 *      MDB_ARTIST = OBJECTPROPCODE_ARTIST,
 *      MDB_GENRE = OBJECTPROPCODE_GENRE,
 *      MDB_ALBUM = OBJECTPROPCODE_ALBUMNAME
 *  }MDB_PROPERTY_CODE_T;
 *  @endcode
 *  @li@c  MDB_INVALID_PROPERTY_CODE      -invalid property.
 *  @li@c  MDB_ARTIST                                   -artist property.
 *  @li@c  MDB_GENRE                                    -genre property.
 *  @li@c  MDB_ALBUM                                    -album property.
 */
/*------------------------------------------------------------------*/
typedef enum
{
    MDB_INVALID_PROPERTY_CODE,
    MDB_ARTIST = OBJECTPROPCODE_ARTIST,
    MDB_GENRE = OBJECTPROPCODE_GENRE,
    MDB_ALBUM = OBJECTPROPCODE_ALBUMNAME
}MDB_PROPERTY_CODE_T;

/*------------------------------------------------------------------*/
/*! @enum MDB_LAYOUT_NODE_TYPE_T
 *  @brief  This enumeration specifies the layout node types.
 *  @code
 *  typedef enum
 *  {
 *      MDB_NONE,
 *      MDB_ROOT,  
 *      MDB_STORAGE,
 *      MDB_CATEGORY,
 *      MDB_LABEL,
 *      MDB_PROPERTY,
 *      MDB_FOLDER,  
 *      MDB_FILE
 *  }MDB_LAYOUT_NODE_TYPE_T;
 *  @endcode
 *  @li@c  MDB_NONE             -type of none 
 *  @li@c  MDB_ROOT             -type of root 
 *  @li@c  MDB_STORAGE       -type of storage 
 *  @li@c  MDB_CATEGORY     -type of category 
 *  @li@c  MDB_LABEL            -type of label 
 *  @li@c  MDB_PROPERTY      -type of property 
 *  @li@c  MDB_FOLDER         -type of folder 
 *  @li@c  MDB_FILE               -type of file 
 */
/*------------------------------------------------------------------*/
typedef enum
{
    MDB_NONE,
    MDB_ROOT,
    MDB_STORAGE,
    MDB_CATEGORY,
    MDB_LABEL,
    MDB_PROPERTY,
    MDB_FOLDER,
    MDB_FILE
}MDB_LAYOUT_NODE_TYPE_T;

/*------------------------------------------------------------------*/
/*! @enum MDB_ASYNC_COND_T
 *  @brief   This enumeration specifies the MDB processing asynchronous code.
 *  @code
 *  typedef enum
 *  {
 *      MDB_COND_INVALID,
 *      MDB_COND_DB_NOT_SUPPORT,
 *      MDB_COND_DB_INITED,
 *      MDB_COND_DB_COMPLETE
 *  }MDB_ASYNC_COND_T;
 *  @endcode
 *  @li@c  MDB_COND_INVALID                    -invalid condition
 *  @li@c  MDB_COND_DB_NOT_SUPPORT     -not support condition
 *  @li@c  MDB_COND_DB_INITED                -initialed condition
 *  @li@c  MDB_COND_DB_COMPLETE           -complete condition
 */
/*------------------------------------------------------------------*/
typedef enum
{
    MDB_COND_INVALID,
    MDB_COND_DB_NOT_SUPPORT,
    MDB_COND_DB_INITED,
    MDB_COND_DB_COMPLETE
}MDB_ASYNC_COND_T;

/*------------------------------------------------------------------*/
/*! @enum MDB_STATUS_T
 *  @brief  This enumeration specifies the MDB processing status.
 *  @code
 *  typedef enum
 *  {
 *      MDB_INVALID_STATUS,
 *      MDB_INIT_STATUS,
 *      MDB_INITED_STATUS,
 *      MDB_CONFIG_STATUS,
 *      MDB_DB_CREATE_STATUS,
 *      MDB_READY_STATUS,
 *      MDB_BUSY_STATUS
 *  }MDB_STATUS_T;
 *  @endcode
 *  @li@c  MDB_INVALID_STATUS          -invalid status
 *  @li@c  MDB_INIT_STATUS                -init status
 *  @li@c  MDB_INITED_STATUS            -initialed status
 *  @li@c  MDB_CONFIG_STATUS           -configure status
 *  @li@c  MDB_DB_CREATE_STATUS     -create status
 *  @li@c  MDB_READY_STATUS             -ready status
 *  @li@c  MDB_BUSY_STATUS               -busy status
 */
/*------------------------------------------------------------------*/
typedef enum
{
    MDB_INVALID_STATUS,
    MDB_INIT_STATUS,
    MDB_INITED_STATUS,
    MDB_CONFIG_STATUS,
    MDB_DB_CREATE_STATUS,
    MDB_READY_STATUS,
    MDB_BUSY_STATUS
}MDB_STATUS_T;

/*------------------------------------------------------------------*/
/*! @brief  This function is called in the context of the MDB. It will notify processing status.
 *  @param [in]     h_mdb   Contains the MDB handle.
 *  @param [in]     pv_tag  References a tag value.
 *  @param [in]     e_async_cond    Contains the asynchronous code.
 *  @param [in]     pv_data     References a data value.
 *  @param [out]   -
 *  @note
 *  @see
 *  @return     -
 *  @retval      -
 */
/*------------------------------------------------------------------*/
typedef VOID (*x_mdb_async_fct) (HANDLE_T           h_mdb,
                                 VOID*              pv_tag,
                                 MDB_ASYNC_COND_T   e_async_cond,
                                 VOID*              pv_data);

/*------------------------------------------------------------------*/
/*! @brief  This function is called in the context of the MDB. It will judge if two property value 
 *              are the same. Not used now.
 *  @param [in]     ui2_data_type   MTP property data type..
 *  @param [in]     pui1_data_src   References a tag value.
 *  @param [in]     pui1_data_dst   Contains the asynchronous code.
 *  @param [out]    -
 *  @note
 *  @see
 *  @return     result of compare.
 *  @retval     TRUE    The two values are the same.
 *  @retval     FALSE   The two values are different.
 */
/*------------------------------------------------------------------*/
typedef BOOL (*x_mdb_compare_fct) (UINT16   ui2_data_type,
                                   UINT8*   pui1_data_src,
                                   UINT8*   pui1_data_dst);

/*------------------------------------------------------------------*/
/*! @struct MDB_LAYOUT_NODE_T
 *  @brief  This type specifies the generic node structure of layout tree.
 *  @code
 *  typedef struct _MDB_LAYOUT_NODE_T
 *  {
 *      MDB_LAYOUT_NODE_TYPE_T  e_type;
 *      UTF16_T*                w2s_name;
 *      UINT32                  ui4_code;
 *      x_mdb_compare_fct       pf_compare;
 *  }MDB_LAYOUT_NODE_T;
 *  @endcode
 *  @li@c  e_type        -Indicate which kind of layout node.
 *  @li@c  w2s_name      -Layout node name string
 *  @li@c  ui4_code      -layout node code index. Now only when (e_type == property node) used, 
 *                               will be assign to the value of MDB_PROPERTY_CODE_T.
 *  @li@c  pf_compare   -user indicate compare function if need. ( not used now.)
 */
/*------------------------------------------------------------------*/
typedef struct _MDB_LAYOUT_NODE_T
{
    MDB_LAYOUT_NODE_TYPE_T  e_type;
    UTF16_T*                w2s_name;
    CHAR*                   s_name;
    UINT32                  ui4_code;
    x_mdb_compare_fct       pf_compare;
}MDB_LAYOUT_NODE_T;


typedef struct _MDB_OPEN_PARAM
{
    VOID* pt_libmtp_dev; //LIBMTP_mtpdevice_t
}MDB_OPEN_PARAM;
/*
        MTP database builder module item info define.
*/
/*------------------------------------------------------------------*/
/*! @struct MDB_ITEM_INFO_T
 *  @brief  This type specifies the item structure of query condition and result.
 *  @code
 *  typedef struct
 *  {
 *      MDB_ITEM_HND_T          t_item_handle;  
 *      MDB_OBJ_HND_T           t_object_handle; 
 *      UINT32                  ui4_storage_id; 
 *      UINT32                  ui4_size;
 *      UTF16_T                 w2s_name[MDB_ITEM_STR_LEN_MAX];
 *      TIME_T                  ui8_create_time;
 *      TIME_T                  ui8_access_time;
 *      TIME_T                  ui8_modify_time;
 *  }MDB_ITEM_INFO_T;
 *  @endcode
 *  @li@c  t_item_handle           -item name string.
 *  @li@c  t_object_handle         -item lie in which storage id.
 *  @li@c  ui4_storage_id           -item handle allocated by the MTP database
 *  @li@c  ui4_size                    -item size count by byte
 *  @li@c  w2s_name[MDB_ITEM_STR_LEN_MAX]    -item object handle by MTP device define.
 *  @li@c  ui8_create_time      -item create time. 
 *  @li@c  ui8_access_time      -item last access time. 
 *  @li@c  ui8_modify_time     -item last modify time. 
 */
/*------------------------------------------------------------------*/
typedef struct
{
    MDB_ITEM_HND_T          t_item_handle;  /* mdb define handle for every item */
    MDB_OBJ_HND_T           t_object_handle; /* actual handle in MTP device. */
    UINT32                  ui4_storage_id; /*= 0 , don't care.*/
    UINT32                  ui4_size;
    UTF16_T                 w2s_name[MDB_ITEM_STR_LEN_MAX];
    CHAR                    s_name[MDB_ITEM_STR_LEN_MAX];
    TIME_T                  ui8_create_time;
    TIME_T                  ui8_access_time;
    TIME_T                  ui8_modify_time;
}MDB_ITEM_INFO_T;

/*------------------------------------------------------------------*/
/*! @struct MDB_ITEM_DETAIL_INFO_T
 *  @brief   This type specifies the item structure of detail info.
 *  @code
 *  typedef struct
 *  {
 *      MDB_ITEM_INFO_T* pt_item_info;
 *      UINT16           ui2_format_code;
 *      UTF16_T          w2s_Title[MDB_ITEM_STR_LEN_MAX];
 *      UTF16_T          w2s_Genre[MDB_ITEM_STR_LEN_MAX];
 *      UTF16_T          w2s_Artist[MDB_ITEM_STR_LEN_MAX];
 *      UTF16_T          w2s_Album[MDB_ITEM_STR_LEN_MAX];
 *      UINT16           ui2_pic_width;
 *      UINT16           ui2_pic_height;
 *      UINT32           ui4_duration;
 *  }MDB_ITEM_DETAIL_INFO_T;
 *  @endcode
 *  @li@c  pt_item_info      -Specify basic info of item.
 *  @li@c  ui2_format_code    -Specify the format code of item.
 *  @li@c  w2s_Title[MDB_ITEM_STR_LEN_MAX]     -Specify the title of item.
 *  @li@c  w2s_Genre[MDB_ITEM_STR_LEN_MAX]   -Specify the genre of item.
 *  @li@c  w2s_Artist[MDB_ITEM_STR_LEN_MAX]     -Specify the artist of item.
 *  @li@c  w2s_Album[MDB_ITEM_STR_LEN_MAX]    -Specify the album of item.
 *  @li@c  ui2_pic_width          -Specify picture width of item.
 *  @li@c  ui2_pic_height         -Specify picture height of item.
 *  @li@c  ui4_duration           -Specify duration of item.
 */
/*------------------------------------------------------------------*/
typedef struct
{
    MDB_ITEM_INFO_T* pt_item_info;
    UINT16           ui2_format_code;
    UTF16_T          w2s_Title[MDB_ITEM_STR_LEN_MAX];
    UTF16_T          w2s_Genre[MDB_ITEM_STR_LEN_MAX];
    UTF16_T          w2s_Artist[MDB_ITEM_STR_LEN_MAX];
    UTF16_T          w2s_Album[MDB_ITEM_STR_LEN_MAX];
    UINT16           ui2_pic_width;
    UINT16           ui2_pic_height;
    UINT32           ui4_duration;
    /* more info if need */
}MDB_ITEM_DETAIL_INFO_T;

/*
        mtp database builder module database information define
*/
/*------------------------------------------------------------------*/
/*! @struct MDB_CONFIG_T
 *  @brief  This type specifies the item structure of configuration.
 *  @code
 *  typedef struct
 *  {    
 *          This is the MTP storage id,
 *          if set to 0, means the builder will use the object in all storage.
 *          else, it means use the special storage.
 *          if the storage id is invalid, 
 *      UINT32      ui4_storage_id;
 *      UINT32      ui4_support_total_item_max;
 *      UINT32      ui4_file_name_len; 
 *      UINT32      ui4_formats_support_num;
 *      UINT16*     pui2_formats;
 *      VOID*       pv_tree;
 *  }MDB_CONFIG_T;
 *  @endcode
 *  @li@c  ui4_storage_id          -This is the MTP storage id.
 *  @li@c  ui4_support_total_item_max   -Specify support max total item.
 *  @li@c  ui4_file_name_len                 -Specify support file name length.
 *  @li@c  ui4_formats_support_num     -Specify the number of support formats.
 *  @li@c  pui2_formats        -Specify the formats. 
 *  @li@c  pv_tree                -Specify the virtual tree. 
 */
/*------------------------------------------------------------------*/
typedef struct
{
    /*
        This is the mtp storage id,
        if set to 0, means the builder will use the object in all storage.
        else, it means use the special storage.
        if the storage id is invalid,
    */
    UINT32      ui4_storage_id;

    /* mtp builder database limit define */
    UINT32      ui4_support_total_item_max;
    UINT32      ui4_file_name_len; /* by character */
    UINT32      ui4_formats_support_num;
    UINT16*     pui2_formats;

    /* virtual tree */
    VOID*       pv_tree;
}MDB_CONFIG_T;

typedef struct _C_MDB_IF_T
{
    INT32 (*c_mdb_open)(
                        VOID*               pt_cfg,
                            HANDLE_T*       ph_mdb);

    INT32 (*c_mdb_register_async)(HANDLE_T              h_mdb,
                                      x_mdb_async_fct       pf_async);

    INT32 (*c_mdb_deregister_async)(HANDLE_T            h_mdb,
                                        x_mdb_async_fct     pf_async);

    INT32 (*c_mdb_config)(HANDLE_T          h_mdb,
                              MDB_CONFIG_T*     pt_cfg);

    INT32 (*c_mdb_close)(HANDLE_T h_mdb);

    INT32 (*c_mdb_status)(HANDLE_T         h_mdb,
                              MDB_STATUS_T*    pe_status);

    INT32 (*c_mdb_start)(HANDLE_T     h_mdb);

    INT32 (*c_mdb_pause)(HANDLE_T     h_mdb);

    INT32 (*c_mdb_resume)(HANDLE_T    h_mdb);

    INT32 (*c_mdb_percentage)(HANDLE_T    h_mdb,
                                  UINT8*      pui1_percentage);

    INT32 (*c_mdb_obj_detail_info)(HANDLE_T                 h_mdb,
                                       MDB_OBJ_HND_T            t_obj,
                                       MDB_ITEM_DETAIL_INFO_T*  pt_info);


    INT32 (*c_mdb_device_info)(HANDLE_T             h_mdb,
                                   MTP_DEVICE_INFO_T**  ppt_mdb_device_info);

    INT32 (*c_mdb_storage_ids)(HANDLE_T     h_mdb,
                                   UINT32*      pui4_storage_num,
                                   UINT32**     ppui4_storage_ids);

    INT32 (*c_mdb_storage_info)(HANDLE_T                h_mdb,
                                    UINT32                  ui4_storage_id,
                                    MTP_STORAGE_INFO_T**    ppt_mdb_storage_info);

    INT32 (*c_mdb_start_define_tree)(VOID** ppv_tree);

    INT32 (*c_mdb_config_category)(VOID*               pv_tree,
                                       UINT8               ui1_category_idx,
                                       UINT32              ui4_max_item,
                                       UINT32              ui4_format_num,
                                       UINT16*             aui2_formats);

    INT32 (*c_mdb_add_layout_node)(VOID*               pv_tree,
                                       MDB_LAYOUT_NODE_T*  pt_node,
                                       MDB_NODE_HND_T      t_parent,
                                       MDB_NODE_HND_T*     pt_new_node);

    INT32 (*c_mdb_finish_define_tree)(VOID*    pv_tree);

    INT32 (*c_mdb_virtual_tree_free)(VOID*    pv_tree);

} C_MDB_IF_T;

#endif /* _U_MBD_H_ */

/*----------------------------------------------------------------------------*/
/*! @} */                                      /* end of groupMW_MTP_MNGR_MDB */
/*----------------------------------------------------------------------------*/

