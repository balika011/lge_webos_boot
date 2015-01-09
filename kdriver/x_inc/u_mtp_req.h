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
/*-----------------------------------------------------------------------------
 * $RCSfile: mtp_req.h,v $
 * $Revision: #1 $
 * $Date: 2015/01/09 $
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_HQ_int/DTV_X_ATSC/1 $
 * $SWAuthor: Yuchien Chen $
 * $MD5HEX: 34076dbf72c1e756a741024f89fac47f $
 *
 * Description: 
 *         This file contains MTP request type & structues.
 *---------------------------------------------------------------------------*/
#ifndef _MTP_REQUEST_H_
#define _MTP_REQUEST_H_
/*-----------------------------------------------------------------------------
                    include files
-----------------------------------------------------------------------------*/
#include "x_common.h"
#include "x_lnk_list.h"
#include "u_mtp.h"
#include "libmtp.h"

/*--------------------------------------------------------------------------
 Constants, enumerations, and macros
---------------------------------------------------------------------------*/
typedef enum _MTP_DEV_REQ_TYPE_T
{
    MTP_DEV_REQ_NORMAL,
    MTP_DEV_REQ_INTERRUPT
}MTP_DEV_REQ_TYPE_T;

/** basic mtp protocol request handle
 *
 */    
#define MTP_DEV_HANDLE                                  (HT_GROUP_MTP + (HANDLE_TYPE_T) 1 )    
#define MTP_REQ_TYPE_GET_DEVICE_INFO                    (HT_GROUP_MTP + (HANDLE_TYPE_T) 2 )
#define MTP_REQ_TYPE_OPEN_SESSION                       (HT_GROUP_MTP + (HANDLE_TYPE_T) 3 )
#define MTP_REQ_TYPE_CLOSE_SESSION                      (HT_GROUP_MTP + (HANDLE_TYPE_T) 4 )
#define MTP_REQ_TYPE_GET_STORAGE_IDS                    (HT_GROUP_MTP + (HANDLE_TYPE_T) 5 )
#define MTP_REQ_TYPE_GET_STORAGE_INFO                   (HT_GROUP_MTP + (HANDLE_TYPE_T) 6 )
#define MTP_REQ_TYPE_GET_NUM_OBJECTS                    (HT_GROUP_MTP + (HANDLE_TYPE_T) 7 )
#define MTP_REQ_TYPE_GET_OBJECT_HANDLES                 (HT_GROUP_MTP + (HANDLE_TYPE_T) 8 )
#define MTP_REQ_TYPE_GET_OBJECT_INFO                    (HT_GROUP_MTP + (HANDLE_TYPE_T) 9 )
#define MTP_REQ_TYPE_SEND_OBJECT_INFO                   (HT_GROUP_MTP + (HANDLE_TYPE_T) 10)
#define MTP_REQ_TYPE_DELETE_OBJECT                      (HT_GROUP_MTP + (HANDLE_TYPE_T) 11)
#define MTP_REQ_TYPE_MOVE_OBJECT                        (HT_GROUP_MTP + (HANDLE_TYPE_T) 12)
#define MTP_REQ_TYPE_COPY_OBJECT                        (HT_GROUP_MTP + (HANDLE_TYPE_T) 13)
#define MTP_REQ_TYPE_GET_OBJECT_REFERENCES              (HT_GROUP_MTP + (HANDLE_TYPE_T) 14)
#define MTP_REQ_TYPE_SET_OBJECT_REFERENCES              (HT_GROUP_MTP + (HANDLE_TYPE_T) 15)
#define MTP_REQ_TYPE_INITIATE_CAPTURE                   (HT_GROUP_MTP + (HANDLE_TYPE_T) 16)
#define MTP_REQ_TYPE_INITIATE_OPEN_CAPTURE              (HT_GROUP_MTP + (HANDLE_TYPE_T) 17)
#define MTP_REQ_TYPE_TERMINATE_OPEN_CAPTURE             (HT_GROUP_MTP + (HANDLE_TYPE_T) 18)
#define MTP_REQ_TYPE_SKIP                               (HT_GROUP_MTP + (HANDLE_TYPE_T) 19)
#define MTP_REQ_TYPE_FORMAT_STORE                       (HT_GROUP_MTP + (HANDLE_TYPE_T) 20)
#define MTP_REQ_TYPE_RESET_DEVICE                       (HT_GROUP_MTP + (HANDLE_TYPE_T) 21)
#define MTP_REQ_TYPE_SELF_TEST                          (HT_GROUP_MTP + (HANDLE_TYPE_T) 22)
#define MTP_REQ_TYPE_SET_OBJECT_PROTECTION              (HT_GROUP_MTP + (HANDLE_TYPE_T) 23)
#define MTP_REQ_TYPE_POWER_DOWN                         (HT_GROUP_MTP + (HANDLE_TYPE_T) 24)
#define MTP_REQ_TYPE_ISSESSIONOPEN                      (HT_GROUP_MTP + (HANDLE_TYPE_T) 25)

/** mtp protocol request handle related to object data
 *
 */    
#define MTP_REQ_TYPE_GET_THUMB                          (HT_GROUP_MTP + (HANDLE_TYPE_T) 27)
#define MTP_REQ_TYPE_GET_PARTIAL_OBJECT                 (HT_GROUP_MTP + (HANDLE_TYPE_T) 28)
#define MTP_REQ_TYPE_GET_OBJECT                         (HT_GROUP_MTP + (HANDLE_TYPE_T) 29)
#define MTP_REQ_TYPE_BEGIN_GET_OBJECT                   (HT_GROUP_MTP + (HANDLE_TYPE_T) 30)
#define MTP_REQ_TYPE_GET_OBJECT_SKIP                    (HT_GROUP_MTP + (HANDLE_TYPE_T) 31)
#define MTP_REQ_TYPE_GET_NEXT_DATA                      (HT_GROUP_MTP + (HANDLE_TYPE_T) 32)
#define MTP_REQ_TYPE_SEND_OBJECT_T                      (HT_GROUP_MTP + (HANDLE_TYPE_T) 33)
#define MTP_REQ_TYPE_BEGIN_SEND_OBJECT_T                (HT_GROUP_MTP + (HANDLE_TYPE_T) 34)
#define MTP_REQ_TYPE_SEND_NEXT_DATA_T                   (HT_GROUP_MTP + (HANDLE_TYPE_T) 35)


/** mtp protocol request handle related to object prop and device prop
 *
 */
#define MTP_REQ_TYPE_GET_DEVICE_PROP_DESC               (HT_GROUP_MTP + (HANDLE_TYPE_T) 36)
#define MTP_REQ_TYPE_GET_DEVICE_PROP_VALUE              (HT_GROUP_MTP + (HANDLE_TYPE_T) 37)
#define MTP_REQ_TYPE_SET_DEVICE_PROP_VALUE              (HT_GROUP_MTP + (HANDLE_TYPE_T) 38)
#define MTP_REQ_TYPE_RESET_DEVICE_PROP_VALUE            (HT_GROUP_MTP + (HANDLE_TYPE_T) 39)
#define MTP_REQ_TYPE_GET_OBJECT_PROPERTIES_SUPPORTED    (HT_GROUP_MTP + (HANDLE_TYPE_T) 40)
#define MTP_REQ_TYPE_GET_OBJECT_PROP_DESC               (HT_GROUP_MTP + (HANDLE_TYPE_T) 41)
#define MTP_REQ_TYPE_GET_OBJECT_PROP_VALUE              (HT_GROUP_MTP + (HANDLE_TYPE_T) 42)
#define MTP_REQ_TYPE_SET_OBJECT_PROP_VALUE              (HT_GROUP_MTP + (HANDLE_TYPE_T) 43)
#define MTP_REQ_TYPE_RESET_OBJECT_PROP_VALUE            (HT_GROUP_MTP + (HANDLE_TYPE_T) 44)
#define MTP_REQ_TYPE_SIMPLE_GET_OBJECT_PROP_LIST        (HT_GROUP_MTP + (HANDLE_TYPE_T) 45)
#define MTP_REQ_TYPE_BEGIN_GET_OBJECT_PROP_LIST         (HT_GROUP_MTP + (HANDLE_TYPE_T) 46)
#define MTP_REQ_TYPE_GET_NEXT_OBJECT_PROP_LIST          (HT_GROUP_MTP + (HANDLE_TYPE_T) 47)
#define MTP_REQ_TYPE_BEGIN_GET_OBJECT_PROP_LIST_ENH     (HT_GROUP_MTP + (HANDLE_TYPE_T) 48)
#define MTP_REQ_TYPE_GET_NEXT_OBJECT_PROP_LIST_ENH      (HT_GROUP_MTP + (HANDLE_TYPE_T) 49)
#define MTP_REQ_TYPE_SET_OBJECT_PROP_LIST               (HT_GROUP_MTP + (HANDLE_TYPE_T) 50)
#define MTP_REQ_TYPE_GET_INTERDEPENDENT_PROP_DESC       (HT_GROUP_MTP + (HANDLE_TYPE_T) 51)
#define MTP_REQ_TYPE_SEND_OBJECT_PROP_LIST              (HT_GROUP_MTP + (HANDLE_TYPE_T) 52)


/** general mtp protocol request handle
 *
 */
#define MTP_REQ_TYPE_EXECUTE_COMMAND                    (HT_GROUP_MTP + (HANDLE_TYPE_T) 53)
#define MTP_REQ_TYPE_EXECUTE_COMMAND_GET_DATA           (HT_GROUP_MTP + (HANDLE_TYPE_T) 54)
#define MTP_REQ_TYPE_EXECUTE_COMMAND_BEGIN_GET_DATA     (HT_GROUP_MTP + (HANDLE_TYPE_T) 55)
#define MTP_REQ_TYPE_EXECUTE_COMMAND_GET_NEXT_DATA      (HT_GROUP_MTP + (HANDLE_TYPE_T) 56)
#define MTP_REQ_TYPE_EXECUTE_COMMAND_SEND_DATA          (HT_GROUP_MTP + (HANDLE_TYPE_T) 57)
#define MTP_REQ_TYPE_EXECUTE_COMMAND_BEGIN_SEND_DATA    (HT_GROUP_MTP + (HANDLE_TYPE_T) 58)
#define MTP_REQ_TYPE_EXECUTE_COMMAND_SEND_NEXT_DATA     (HT_GROUP_MTP + (HANDLE_TYPE_T) 59)

 
/** Protocol layer request ???
 *
 */
#define MTP_REQ_TYPE_ABORT_TRANSFER                     (HT_GROUP_MTP + (HANDLE_TYPE_T) 60)
#define MTP_REQ_TYPE_RESET_IO                           (HT_GROUP_MTP + (HANDLE_TYPE_T) 61)
#define MTP_REQ_TYPE_NO_EXPECTED                        (HT_GROUP_MTP + (HANDLE_TYPE_T) 62)

typedef HANDLE_TYPE_T MTP_REQ_TYPE_T;

#define MTP_REQ_SUPPORT_MAX_NUM  (MTP_REQ_TYPE_NO_EXPECTED - MTP_DEV_HANDLE)


/*--------------------------------------------------------------------------
 basic mtp protocol request body structure
---------------------------------------------------------------------------*/ 

typedef struct _MTP_GET_DEVICE_INFO_RECEIVE_T
{
    MTP_DEVICE_INFO_T*      pt_device_info;
    VOID*                   pv_this;
    mtp_obj_free_fct        pf_free;
}MTP_GET_DEVICE_INFO_RECEIVE_T;

/**  struct for MTP_REQ_TYPE_GET_DEVICE_INFO
 * 
 */
typedef struct _MTP_GET_DEVICE_INFO_T
{
    /* output member */
    MTP_GET_DEVICE_INFO_RECEIVE_T t_receive;
}MTP_GET_DEVICE_INFO_T;


/**  struct for MTP_REQ_TYPE_OPEN_SESSION
 * 
 */
typedef struct _MTP_OPEN_SESSION_T
{
    /* input member */
    UINT32 ui4_session_id;
}MTP_OPEN_SESSION_T;


/**  struct for MTP_REQ_TYPE_CLOSE_SESSION
 * 
 */
typedef struct _MTP_CLOSE_SESSION_T
{
    /* invalid member for build*/
    UINT32 ui4_invalid_value;
}MTP_CLOSE_SESSION_T;

typedef struct _MTP_GET_STORAGE_IDS_RECEIVE_T
{
    UINT32              ui4_num;
    UINT32*             aui4_storage_ids;
    VOID*               pv_this;
    mtp_obj_free_fct    pf_free;
}MTP_GET_STORAGE_IDS_RECEIVE_T;

/** struct of MTP_REQ_TYPE_GET_STORAGE_IDS
 *
 */
typedef struct _MTP_GET_STORAGE_IDS_T
{
    /* output member */
    MTP_GET_STORAGE_IDS_RECEIVE_T   t_receive;
}MTP_GET_STORAGE_IDS_T;


typedef struct _MTP_GET_STORAGE_INFO_RECEIVE_T
{
    MTP_STORAGE_INFO_T*     pt_storage_info;
    VOID*                   pv_this;
    mtp_obj_free_fct        pf_free;
}MTP_GET_STORAGE_INFO_RECEIVE_T;

/** struct of MTP_REQ_TYPE_GET_STORAGE_INFO
 *
 */
typedef struct _MTP_GET_STORAGE_INFO_T
{
    /* input member */
    UINT32 ui4_storage_id;
    
    /* output member */
    MTP_GET_STORAGE_INFO_RECEIVE_T t_receive;
}MTP_GET_STORAGE_INFO_T;

/** struct of MTP_REQ_TYPE_GET_NUM_OBJECTS
 * 
 */
typedef struct _MTP_GET_NUM_OBJECTS_T
{
    /* input member */
    UINT32  ui4_storage_id;
    UINT16  ui2_format_code; /*_FORMATCODE*/
    UINT32  ui4_parent_object_handle;

    /* output member */
    UINT32  ui4_num_objects;
}MTP_GET_NUM_OBJECTS_T;

typedef struct _MTP_GET_OBJECT_HANDLES_RECEIVE_T
{
    UINT32              ui4_num;
    UINT32*             aui4_object_handles;
    VOID*               pv_this;
    mtp_obj_free_fct    pf_free;
}MTP_GET_OBJECT_HANDLES_RECEIVE_T;

/** struct of MTP_REQ_TYPE_GET_OBJECT_HANDLES
 *
 */
typedef struct _MTP_GET_OBJECT_HANDLES_T
{
    /* input member */
    UINT32                              ui4_storage_id;
    UINT16                              ui2_format_code; /*_FORMATCODE*/
    UINT32                              ui4_parent_object_handle;

    /* output member */
    MTP_GET_OBJECT_HANDLES_RECEIVE_T    t_receive;
}MTP_GET_OBJECT_HANDLES_T;

typedef struct _MTP_GET_OBJECT_INFO_RECEIVE_T
{
    UINT32                  ui4_curr_get_num;
    MTP_OBJECT_INFO_T**     apt_object_info;
    UINT16*                 apui2_response_code;
    VOID*                   pv_this;
    mtp_obj_free_fct        pf_free;
}MTP_GET_OBJECT_INFO_RECEIVE_T;

/** struct of MTP_REQ_TYPE_GET_OBJECT_INFO
 *
 */
typedef struct _MTP_GET_OBJECT_INFO_T
{
    /* input member */
    UINT32                          ui4_num;
    UINT32*                         aui4_object_handle;
    
    /* output member */
    MTP_GET_OBJECT_INFO_RECEIVE_T   t_receive;
}MTP_GET_OBJECT_INFO_T;

/** struct of MTP_REQ_TYPE_SEND_OBJECT_INFO
 *
 */ 
typedef struct _MTP_SEND_OBJECT_INFO_T
{
    /* input member */
    UINT32                  ui4_storage_id;
    UINT32                  ui4_parent_object_handle;
    MTP_OBJECT_INFO_T*      pt_object_info;

    /* output member */
    UINT32                  ui4_result_storage_id;
    UINT32                  ui4_result_parent_object_handle;
    UINT32                  ui4_result_object_handle;
}MTP_SEND_OBJECT_INFO_T;


/** struct of MTP_REQ_TYPE_DELETE_OBJECT
 *
 */
typedef struct _MTP_DELETE_OBJECT_T
{
    /* input member */
    UINT32      ui4_object_handle;
    UINT16      ui2_format_code; /*_FORMATCODE*/
}MTP_DELETE_OBJECT_T;

/** struct of MTP_REQ_TYPE_MOVE_OBJECT
 *
 */
typedef struct _MTP_MOVE_OBJECT_T
{
    /* input member */
    UINT32      ui4_object_handle;
    UINT32      ui4_storage_id;
    UINT32      ui4_parent_object_handle;
}MTP_MOVE_OBJECT_T;

/** struct of MTP_REQ_TYPE_COPY_OBJECT
 *
 */
typedef struct _MTP_COPY_OBJECT_T
{
    /* input member */
    UINT32      ui4_object_handle;
    UINT32      ui4_storage_id;
    UINT32      ui4_parent_object_handle;
    
    /* output member */
    UINT32      ui4_result_object_handle;
}MTP_COPY_OBJECT_T;

typedef struct _MTP_GET_OBJECT_REFERENCES_RECEIVE_T
{
    UINT32              ui4_num;
    UINT32*             aui4_references;
    VOID*               pv_this;
    mtp_obj_free_fct    pf_free;  
}MTP_GET_OBJECT_REFERENCES_RECEIVE_T;

/** struct of MTP_REQ_TYPE_GET_OBJECT_REFERENCES_DESC
 *
 */
typedef struct _MTP_GET_OBJECT_REFERENCES_T
{
    /* input member */
    UINT32                              ui4_object_handle;
    
    /* output member */
    MTP_GET_OBJECT_REFERENCES_RECEIVE_T t_receive;
}MTP_GET_OBJECT_REFERENCES_T;


/** struct of MTP_REQ_TYPE_SET_OBJECT_REFERENCES
 *
 */
typedef struct _MTP_SET_OBJECT_REFERENCES_T
{
    /* input member */
    UINT32      ui4_object_handle;
    UINT32      ui4_num;
    UINT32*     aui4_references;
}MTP_SET_OBJECT_REFERENCES_T;


/** struct of MTP_REQ_TYPE_INITIATE_CAPTURE
 *
 */
typedef struct _MTP_INITIATE_CAPTURE_T
{
    /* input member */
    UINT32      ui4_storage_id;
    UINT16      ui2_format_code; /*_FORMATCODE*/
}MTP_INITIATE_CAPTURE_T;

/** struct of MTP_REQ_TYPE_TERMINATE_OPEN_CAPTURE
 *
 */
typedef struct _MTP_TERMINATE_OPEN_CAPTURE_T
{
    /* input member */
    UINT32      ui4_transaction_id;
}MTP_TERMINATE_OPEN_CAPTURE_T;

/** struct of MTP_REQ_TYPE_INITIATE_OPEN_CAPTURE
 *
 */
typedef struct _MTP_INITIATE_OPEN_CAPTURE_T
{
    /* input member */
    UINT32      ui4_storage_id;
    UINT16      ui2_format_code; /*_FORMATCODE*/
    
    /* output member */
    UINT32      ui4_transaction_id;
}MTP_INITIATE_OPEN_CAPTURE_T;


/** struct of MTP_REQ_TYPE_SKIP
 *
 */
typedef struct _MTP_SKIP_T
{
    /* input member */
    UINT32      ui4_skip_index;
}MTP_SKIP_T;

/** struct of MTP_REQ_TYPE_FORMAT_STORE
 *
 */
typedef struct _MTP_FORMAT_STORE_T
{
    /* input member */
    UINT32      ui4_storage_id;
    UINT16      ui2_file_system_format; /*_FILESYSTEMTYPE*/
}MTP_FORMAT_STORE_T;

/** struct of MTP_REQ_TYPE_RESET_DEVICE
 *
 */
typedef struct _MTP_RESET_DEVICE_T
{ 
    /* invalid member for build*/
    UINT32      ui4_invalid_value;
}MTP_RESET_DEVICE_T;

/** struct of MTP_REQ_TYPE_SELF_TEST
 *
 */
typedef struct _MTP_SELF_TEST_T
{
    /* input member */
    UINT16      ui2_self_test_type;
}MTP_SELF_TEST_T;

/** struct of MTP_REQ_TYPE_SET_OBJECT_PROTECTION
 *
 */
typedef struct _MTP_SET_OBJECT_PROTECTION_T
{
    /* input member */
    UINT32      ui4_object_handle;                                                               
    UINT16      ui2_protection_status; /*_PROTECTIONSTATUS*/
}MTP_SET_OBJECT_PROTECTION_T;

/** struct of MTP_REQ_TYPE_POWER_DOWN
 *
 */
typedef struct _MTP_POWER_DOWN_T
{
    /* invalid member for build*/
    UINT32      ui4_invalid_value;
}MTP_POWER_DOWN_T;


/** struct of MTP_REQ_TYPE_ISSESSIONPEN
 *
 */
typedef struct _MTP_ISSESSIONOPEN_T
{ 
    /* output member */
    BOOL        b_session_open;
    UINT32      ui4_session_id;
}MTP_ISSESSIONOPEN_T;


/*--------------------------------------------------------------------------
 mtp protocol request related to object data body structure
---------------------------------------------------------------------------*/ 

/** struct of MTP_REQ_TYPE_GET_THUMB
 *
 */
typedef struct _MTP_GET_THUMB_T
{
    /* input member */
    UINT32      ui4_object_handle;
    UINT8*      pui1_buffer;
    UINT32      ui4_buffer_len;
    
    /* output member */
    UINT32      ui4_bytes_read;
}MTP_GET_THUMB_T;

/** struct of MTP_REQ_TYPE_GET_PARTIAL_OBJECT
 *
 */
typedef struct _MTP_GET_PARTIAL_OBJECT_T
{
    /* input member */
    UINT32      ui4_object_handle;
    UINT32      ui4_offset;
    UINT8*      pui1_buffer;
    UINT32      ui4_buffer_len;

    /* output member */
    UINT32      ui4_bytes_read;
}MTP_GET_PARTIAL_OBJECT_T;


/** struct of MTP_REQ_TYPE_GET_OBJECT
 *
 */
typedef struct _MTP_GET_OBJECT_T
{
    /* input member */
    UINT32      ui4_object_handle;
    UINT32      ui4_offset;  
    UINT8*      pui1_buffer;
    UINT32      ui4_buffer_len;
    
    /* output member */
    UINT32      ui4_bytes_read;
}MTP_GET_OBJECT_T;

/** struct of MTP_REQ_TYPE_BEGIN_GET_OBJECT
 *
 */
typedef struct _MTP_BEGIN_GET_OBJECT_T
{
    /* input member */
    UINT32      ui4_object_handle;
    UINT32      ui4_offset; /*mono increase*/
    UINT8*      pui1_buffer;
    UINT32      ui4_buffer_len;
        
    /* output member */
    UINT32      ui4_bytes_read;
}MTP_BEGIN_GET_OBJECT_T;

/** struct of MTP_REQ_TYPE_GET_OBJECT_SKIP
 *
 */
typedef struct _MTP_GET_OBJECT_SKIP_T
{
    /* input member */
    UINT32      ui4_offset; /*mono increase*/
}MTP_GET_OBJECT_SKIP_T;

/** struct of MTP_REQ_TYPE_GET_NEXT_DATA
 *
 */
typedef struct _MTP_GET_NEXT_DATA_T
{
    /* input member */
    UINT8*      pui1_buffer;
    UINT32      ui4_buffer_len;
    
    /* output member */
    UINT32      ui4_bytes_read;
}MTP_GET_NEXT_DATA_T;

/** struct of MTP_REQ_TYPE_SEND_OBJECT_T
 *
 */
typedef struct _MTP_SEND_OBJECT_T
{
    /* input member */
    UINT8*      pui1_buffer;
    UINT32      ui4_buffer_len;
    
    /* output member */
    UINT32      ui4_send_num;
}MTP_SEND_OBJECT_T;

/** struct of MTP_REQ_TYPE_BEGIN_SEND_OBJECT_T
 *
 */
typedef struct _MTP_BEGIN_SEND_OBJECT_T
{
    /* input member */
    UINT8*      pui1_buffer;
    UINT32      ui4_buffer_len;
    UINT32      ui4_object_size;

    /* output member */
    UINT32      ui4_send_num;
}MTP_BEGIN_SEND_OBJECT_T;

/** struct of MTP_REQ_TYPE_SEND_NEXT_DATA_T
 *
 */
typedef struct _MTP_SEND_NEXT_DATA_T
{
    /* input member */
    UINT8*      pui1_buffer;
    UINT32      ui4_buffer_len;
    
    /* output member */
    UINT32      ui4_send_num;
}MTP_SEND_NEXT_DATA_T;


/*--------------------------------------------------------------------------
 mtp protocol request related to prop code body structure
---------------------------------------------------------------------------*/ 
typedef struct _MTP_GET_DEVICE_PROP_DESC_RECEIVE_T
{
    MTP_DEVICE_PROP_DESC_T* pt_prop_desc; 
    VOID*                   pv_this;
    mtp_obj_free_fct        pf_free;
}MTP_GET_DEVICE_PROP_DESC_RECEIVE_T;

/** struct of MTP_REQ_TYPE_GET_DEVICE_PROP_VALUE
 *
 */
typedef struct _MTP_GET_DEVICE_PROP_DESC_T
{
    /* input member */
    UINT16                              ui2_prop_code; /*_DEVICEPROPCODE*/
    
    /* output member */
    MTP_GET_DEVICE_PROP_DESC_RECEIVE_T  t_receive;
}MTP_GET_DEVICE_PROP_DESC_T;

typedef struct _MTP_GET_DEVICE_PROP_VALUE_RECEIVE_T
{
    MTP_VARIANT_T*          pt_prop_value; 
    VOID*                   pv_this;
    mtp_obj_free_fct        pf_free;
}MTP_GET_DEVICE_PROP_VALUE_RECEIVE_T;

/** struct of MTP_REQ_TYPE_GET_DEVICE_PROP_DESC
 *
 */
typedef struct _MTP_GET_DEVICE_PROP_VALUE_T
{
    /* input member */
    UINT16                              ui2_prop_code; /*_DEVICEPROPCODE*/
    UINT16                              ui2_mtp_data_type;
    
    /* output member */
    MTP_GET_DEVICE_PROP_VALUE_RECEIVE_T t_receive;
}MTP_GET_DEVICE_PROP_VALUE_T;


/** struct of MTP_REQ_TYPE_SET_DEVICE_PROP_VALUE
 *
 */
typedef struct _MTP_SET_DEVICE_PROP_VALUE_T
{
    /* input member */
    UINT16          ui2_prop_code; /*_DEVICEPROPCODE*/
    MTP_VARIANT_T   t_new_prop_value;
}MTP_SET_DEVICE_PROP_VALUE_T;


/** struct of MTP_REQ_TYPE_RESET_DEVICE_PROP_VALUE
 *
 */
typedef struct _MTP_RESET_DEVICE_PROP_VALUE_T
{
    /* input member */
    UINT16  ui2_prop_code; /*_DEVICEPROPCODE*/
}MTP_RESET_DEVICE_PROP_VALUE_T;


typedef struct _MTP_GET_INTERDEPENDENT_PROP_DESC_RECEIVE_T
{
    MTP_INTERDEPENDENT_PROP_DESC_LIST_T*    pt_interdependent_prop_desc_list;
    VOID*                                   pv_this;
    mtp_obj_free_fct                        pf_free;
}MTP_GET_INTERDEPENDENT_PROP_DESC_RECEIVE_T;

/** struct of MTP_REQ_TYPE_GET_INTERDEPENDENT_PROP_DESC
 *
 */
typedef struct _MTP_GET_INTERDEPENDENT_PROP_DESC_T
{
    /* input member */
    UINT16                                      ui2_format_code; /*_FORMATCODE*/
    
    /* output member */
    MTP_GET_INTERDEPENDENT_PROP_DESC_RECEIVE_T  t_receive;
}MTP_GET_INTERDEPENDENT_PROP_DESC_T;


typedef struct _MTP_GET_OBJECT_PROP_DESC_RECEIVE_T
{
    MTP_OBJECT_PROP_DESC_T*                 pt_prop_desc;

    VOID*                                   pv_this;
    mtp_obj_free_fct                        pf_free;
}MTP_GET_OBJECT_PROP_DESC_RECEIVE_T;

/** struct of MTP_REQ_TYPE_GET_OBJECT_PROP_DESC
 *
 */
typedef struct _MTP_GET_OBJECT_PROP_DESC_T
{
    /* input member */
    UINT16                              ui2_prop_code;   /*_OBJECTPROPCODE*/
    UINT16                              ui2_format_code; /*_FORMATCODE*/

    /* output member */
    MTP_GET_OBJECT_PROP_DESC_RECEIVE_T  t_receive;
}MTP_GET_OBJECT_PROP_DESC_T;

typedef struct _MTP_GET_OBJECT_PROPERTIES_SUPPORTED_RECEIVE_T
{
    UINT32              ui4_num;
    UINT16*             aui2_prop_codes; /*_OBJECTPROPCODE*/

    VOID*               pv_this;
    mtp_obj_free_fct    pf_free;
}MTP_GET_OBJECT_PROPERTIES_SUPPORTED_RECEIVE_T;

/** struct of MTP_REQ_TYPE_GET_OBJECT_PROPERTIES_SUPPORTED
 *
 */
typedef struct _MTP_GET_OBJECT_PROPERTIES_SUPPORTED_T
{
    /* input member */
    UINT16                                          ui2_format_code; /*_FORMATCODE*/
    
    /* output member */
    MTP_GET_OBJECT_PROPERTIES_SUPPORTED_RECEIVE_T   t_receive;
}MTP_GET_OBJECT_PROPERTIES_SUPPORTED_T;

typedef struct _MTP_GET_OBJECT_PROP_VALUE_RECEIVE_T
{
    MTP_VARIANT_T*      pt_prop_value;

    VOID*               pv_this;
    mtp_obj_free_fct    pf_free;
}MTP_GET_OBJECT_PROP_VALUE_RECEIVE_T;

/** struct of MTP_REQ_TYPE_GET_OBJECT_PROP_VALUE
 *
 */
typedef struct _MTP_GET_OBJECT_PROP_VALUE_T
{
    /* input member */
    UINT32                              ui4_object_handle;
    UINT16                              ui2_prop_code;     /*_OBJECTPROPCODE*/
    UINT16                              ui2_mtp_data_type; /*_DATATYPE*/

    /* output member */
    MTP_GET_OBJECT_PROP_VALUE_RECEIVE_T t_receive;
}MTP_GET_OBJECT_PROP_VALUE_T;

/** struct of MTP_REQ_TYPE_SET_OBJECT_PROP_VALUE
 *
 */
typedef struct _MTP_SET_OBJECT_PROP_VALUE_T
{
    /* input member */
    UINT32          ui4_object_handle;
    UINT16          ui2_prop_code; /*_OBJECTPROPCODE*/
    MTP_VARIANT_T   t_new_prop_value;
}MTP_SET_OBJECT_PROP_VALUE_T;

/** struct of MTP_REQ_TYPE_RESET_OBJECT_PROP_VALUE
 *
 */
typedef struct _MTP_RESET_OBJECT_PROP_VALUE_T
{
    /* input member */
    UINT32          ui4_object_handle;
    UINT16          ui2_prop_code; /*_OBJECTPROPCODE*/
}MTP_RESET_OBJECT_PROP_VALUE_T;

typedef struct _MTP_GET_OBJ_PROP_LIST_POINT_T
{
    PROPLIST_ITEM_T*                        pt_prop_values_list;
    struct _MTP_GET_OBJ_PROP_LIST_POINT_T*  pt_next;
}MTP_GET_OBJ_PROP_LIST_POINT_T;

typedef struct _MTP_GET_OBJECT_PROP_LIST_RECEIVE_T
{
    UINT32              ui4_total_num;
    UINT32              ui4_num;    /*free after every get request*/
    PROPLIST_ITEM_T**   apt_prop_values_list;
 
    VOID*               pv_this;
    mtp_obj_free_fct    pf_free;
}MTP_GET_OBJECT_PROP_LIST_RECEIVE_T;

typedef struct _MTP_SIMPLE_GET_OBJECT_PROP_LIST_RECEIVE_T
{
    UINT32              ui4_total_num;
    PROPLIST_ITEM_T**   apt_prop_values_list;

    VOID*               pv_this;
    mtp_obj_free_fct    pf_free;
}MTP_SIMPLE_GET_OBJECT_PROP_LIST_RECEIVE_T;

/** struct of MTP_REQ_TYPE_SIMPLE_GET_OBJECT_PROP_LIST
 *
 */
typedef struct _MTP_SIMPLE_GET_OBJECT_PROP_LIST_T
{
    /* input member */
    UINT32                                      ui4_object_handle;
    UINT16                                      ui2_format_code; /*_FORMATCODE*/
    UINT16                                      ui2_prop_code;   /*_OBJECTPROPCODE*/
    UINT32                                      ui4_prop_group;  /*_PROPERTYGROUP */
    UINT32                                      ui4_depth;

    /* output member */
    MTP_SIMPLE_GET_OBJECT_PROP_LIST_RECEIVE_T   t_receive;
}MTP_SIMPLE_GET_OBJECT_PROP_LIST_T;

/** struct of MTP_REQ_TYPE_BEGIN_GET_OBJECT_PROP_LIST
 *
 */
typedef struct _MTP_BEGIN_GET_OBJECT_PROP_LIST_T
{
    /* input member */
    UINT32                              ui4_object_handle;
    UINT16                              ui2_format_code; /*_FORMATCODE*/
    UINT16                              ui2_prop_code;   /*_OBJECTPROPCODE*/
    UINT32                              ui4_prop_group;  /*_PROPERTYGROUP */
    UINT32                              ui4_depth;
    UINT32                              ui4_need_count;
    UINT32                              ui4_prefetch_count;

    /* output member */
    MTP_GET_OBJECT_PROP_LIST_RECEIVE_T  t_receive;
}MTP_BEGIN_GET_OBJECT_PROP_LIST_T;

/** struct of MTP_REQ_TYPE_GET_NEXT_OBJECT_PROP_LIST
 *
 */
typedef struct _MTP_GET_NEXT_OBJECT_PROP_LIST_T
{
    UINT32                              ui4_need_count;
    UINT32                              ui4_prefetch_count;
    MTP_GET_OBJECT_PROP_LIST_RECEIVE_T  t_receive;
}MTP_GET_NEXT_OBJECT_PROP_LIST_T;

/** struct of MTP_REQ_TYPE_GET_OBJECT_PROP_LIST_ENH
 *
 */
typedef struct _MTP_BEGIN_GET_OBJECT_PROP_LIST_ENH_T
{
    /* input member */
    /*must to have and combined with depth (OBJECTHANDLE_ALL & depth=0)*/
    UINT32                              ui4_object_handle; 
    UINT16                              ui2_format_code;    /*_FORMATCODE*/
    UINT32                              ui4_prop_code_num;
    UINT16*                             aui2_prop_code;     /*_OBJECTPROPCODE*/
    UINT32                              ui4_prop_group;     /*_PROPERTYGROUP */
    UINT32                              ui4_depth;

    UINT32                              ui4_need_count;
    UINT32                              ui4_prefetch_count;

    /* output member */
    MTP_GET_OBJECT_PROP_LIST_RECEIVE_T  t_receive;
}MTP_BEGIN_GET_OBJECT_PROP_LIST_ENH_T;

/** struct of MTP_REQ_TYPE_GET_NEXT_OBJECT_PROP_LIST_ENH
 *
 */
typedef struct _MTP_GET_NEXT_OBJECT_PROP_LIST_ENH_T
{
    /* input member */
    UINT32                              ui4_need_count;
    UINT32                              ui4_prefetch_count;

    /* output member */
    MTP_GET_OBJECT_PROP_LIST_RECEIVE_T  t_receive;
}MTP_GET_NEXT_OBJECT_PROP_LIST_ENH_T;

/** struct of MTP_REQ_TYPE_SET_OBJECT_PROP_LIST
 *
 */
typedef struct _MTP_SET_OBJECT_PROP_LIST_T
{
    /* input member */
    UINT32              ui4_num;
    PROPLIST_ITEM_T**   apt_prop_values_list;

    /* output member */
    UINT32              ui4_failed_prop_index;
}MTP_SET_OBJECT_PROP_LIST_T;

/** struct of MTP_REQ_TYPE_SEND_OBJECT_PROP_LIST
 *
 */
typedef struct _MTP_SEND_OBJECT_PROP_LIST_T
{
    /* input member */
    UINT32              ui4_storage_id;
    UINT32              ui4_parent_object_handle;
    UINT16              ui2_format_code; /*_FORMATCODE*/
    UINT64              ui8_object_size;
    UINT32              ui4_num;
    PROPLIST_ITEM_T**   apt_prop_values_list;

    /* output member */
    UINT32              ui4_result_storage_id;
    UINT32              ui4_result_parent_object_handle;
    UINT32              ui4_result_object_handle;
    UINT32              ui4_failed_prop_index;
}MTP_SEND_OBJECT_PROP_LIST_T;



/*--------------------------------------------------------------------------
 general mtp protocol request body structure
---------------------------------------------------------------------------*/ 
/** struct of MTP_REQ_TYPE_EXECUTE_COMMAND
 *
 */
typedef struct _MTP_EXECUTE_COMMAND_T
{
    /* input member, malloc by client, free by client */
    MTP_COMMAND_T   t_command;
    
    /* output member, malloc by client, free by client  */
    MTP_RESPONSE_T  t_response;
}MTP_EXECUTE_COMMAND_T;

/** struct of MTP_REQ_TYPE_EXECUTE_COMMAND_GET_DATA
 *
 */
typedef struct _MTP_EXECUTE_COMMAND_GET_DATA_T
{
    /* input member, malloc by client, free by client */
    MTP_COMMAND_T       t_command;
    UINT8*              pui1_buf; /* malloc by client, free by client */
    UINT32              ui4_buf_len;
    
    /* output member, malloc by client, free by client  */
    MTP_RESPONSE_T      t_response;
    UINT32              ui4_bytes_read;
}MTP_EXECUTE_COMMAND_GET_DATA_T;

/** struct of MTP_REQ_TYPE_EXECUTE_COMMAND_BEGIN_GET_DATA
 *
 */
typedef struct _MTP_EXECUTE_COMMAND_BEGIN_GET_DATA_T
{
    /* input member, malloc by client, free by client */
    MTP_COMMAND_T       t_command;
    UINT8*              pui1_buf;  /* malloc by client, free by client */
    UINT32              ui4_buf_len;
    
    /* output member */
    UINT32              ui4_bytes_read;
}MTP_EXECUTE_COMMAND_BEGIN_GET_DATA_T;

/** struct of MTP_REQ_TYPE_EXECUTE_COMMAND_GET_NEXT_DATA
 *
 */
typedef struct _MTP_EXECUTE_COMMAND_GET_NEXT_DATA_T
{
    /* input member, malloc by client, free by client */
    UINT8*      pui1_buf;  /* malloc by client, free by client */
    UINT32      ui4_buf_len;
    
    /* output member */
    UINT32      ui4_bytes_read;
}MTP_EXECUTE_COMMAND_GET_NEXT_DATA_T;

/** struct of MTP_REQ_TYPE_EXECUTE_COMMAND_SEND_DATA
 *
 */
typedef struct _MTP_EXECUTE_COMMAND_SEND_DATA_T
{
    /* input member */ 
    MTP_COMMAND_T       t_command; /* malloc by client, free by client */
    UINT8*              pui1_buf; /* malloc by client, free by client */
    UINT32              ui4_buf_len;
    
    /* output member, malloc by client, free by client  */
    MTP_RESPONSE_T      t_response;
}MTP_EXECUTE_COMMAND_SEND_DATA_T;

/** struct of MTP_REQ_TYPE_EXECUTE_COMMAND_BEGIN_SEND_DATA
 *
 */
typedef struct _MTP_EXECUTE_COMMAND_BEGIN_SEND_DATA_T
{
    /* input member */ 
    MTP_COMMAND_T   t_command; /* malloc by client, free by client */
    UINT32          ui4_total_data_size;
    UINT8*          pui1_buf; /* malloc by client, free by client */
    UINT32          ui4_buf_len;
    
    /* output member, malloc by client, free by client  */
    MTP_RESPONSE_T  t_response;
}MTP_EXECUTE_COMMAND_BEGIN_SEND_DATA_T;

/** struct of MTP_REQ_TYPE_EXECUTE_COMMAND_SEND_NEXT_DATA
 *
 */
typedef struct _MTP_EXECUTE_COMMAND_SEND_NEXT_DATA_T
{
    /* input member */ 
    UINT8*              pui1_buf; /* malloc by client, free by client */
    UINT32              ui4_buf_len; 
    BOOL                b_last_data;
    
    /* output member */
    MTP_RESPONSE_T      t_response;
}MTP_EXECUTE_COMMAND_SEND_NEXT_DATA_T;


/*--------------------------------------------------------------------------
 other mtp protocol request body structure
---------------------------------------------------------------------------*/
typedef struct _MTP_ABORT_TRANSFER_T
{
    HANDLE_T h_mtp;
}MTP_ABORT_TRANSFER_T;

/*--------------------------------------------------------------------------
 extern request structure
---------------------------------------------------------------------------*/
typedef VOID (*x_mtp_async_fct)(HANDLE_T            h_req,
                                VOID*               pv_tag,
                                MTP_ASYNC_COND_T    e_async_cond,
                                VOID*               pv_data);

typedef struct _MTP_REQUEST_T
{
    BOOL                            b_used;
    UINT8                           ui1_type;
    UINT8                           ui1_pri;
    HANDLE_T                        h_mtp_req;
    UINT16*                         pui2_response_code;
    x_mtp_async_fct                 pf_nfy_fct;
    VOID*                           pv_tag;
    VOID*                           pv_data;
    DLIST_ENTRY_T(_MTP_REQUEST_T)   t_qlink;
}MTP_REQUEST_T;

#endif /* _MTP_REQUEST_H_ */
