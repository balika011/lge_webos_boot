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
/*! @file u_nwl.h 
 *  $File: //DTV/RELEASE_BR/r_DTV_X_IDTV1401_002306_12_001_37_001/boot/kdriver/x_inc/u_nwl.h $
 *  $Revision: #1 $
 *  $Date: 2015/01/09 $
 *  $Author: dtvbm11 $
 *  
 *  @par Description:
 *         This header file contains Network list related API definitions,
 *         which are exported to other Middleware components.
 *
 */
/*----------------------------------------------------------------------------*/

#ifndef _U_NWL_H_
#define _U_NWL_H_
/*----------------------------------------------------------------------------*/
/*! @defgroup   groupMW_NWL Network List
 *  @ingroup    groupMW
 *  @brief      The NWL module is used to store and retrieve information about
 *              networks in DVB broadcasting.
 *  @see        groupMW_CDB
 *  @{
 */
/*----------------------------------------------------------------------------*/

#include "u_common.h"
#include "u_handle.h"
#include "u_handle_grp.h"

/*--------------------------------------------------------------------------
 Constants, enumerations, and macros
---------------------------------------------------------------------------*/

/* Return code status from API */
#define         NWLR_OK                    ((INT32)  0)    /**< Operation succeeded. */
#define         NWLR_FAIL                  ((INT32) -1)    /**< Operation failed. */
#define         NWLR_OUT_OF_HANDLE         ((INT32) -2)    /**< Handle allocation failed. */
#define         NWLR_OUT_OF_MEM            ((INT32) -3)    /**< Memory exhausted. */
#define         NWLR_INV_HANDLE            ((INT32) -4)    /**< Bad handle. */
#define         NWLR_INV_ARG               ((INT32) -5)    /**< Invalid argument. */
#define         NWLR_REC_NOT_FOUND         ((INT32) -6)    /**< Record not found. */
#define         NWLR_NO_WRITE_LOCK         ((INT32) -7)    /**< Failed initializing RW Lock. */
#define         NWLR_NOT_FOUND             ((INT32) -8)    /**< Specified NWL object not found. */
#define         NWLR_END_OF_ITERATION      ((INT32) -9)    /**< Iteration reached end. */
#define         NWLR_MODIFIED              ((INT32) -10)    /**< Database modified since last iteration. */
#define         NWLR_DUPLICATE_REC         ((INT32) -11)    /**< Existing record with the same key value is found. */
#define         NWLR_INV_FILE_PATH         ((INT32) -12)    /**< Invalid file path. */
#define         NWLR_OUT_OF_SPACE          ((INT32) -13)    /**< Failed to allocate space on file system. */
#define         NWLR_AEE_OUT_OF_RESOURCES  ((INT32) -14)    /**< Can not allocate AEE resources. */
#define         NWLR_AEE_NO_RIGHTS         ((INT32) -15)    /**< No right for the AEE resources. */
#define         NWLR_FILE_READ_FAIL        ((INT32) -16)    /**< Failed to read from file. */
#define         NWLR_FILE_WRITE_FAIL       ((INT32) -17)    /**< Failed to write to file. */
#define         NWLR_INV_FILE_VER          ((INT32) -18)    /**< Incompatible file version. */

/* maximum length of network name in NIT table. */
#define         MAX_NETWORK_NAME_LEN            32    /**< Maximum length of network name in NIT table. */

/* handle group type for the Network list handle. */
#define         NWLT_NETWORK_LIST          (HT_GROUP_NWL + ((HANDLE_TYPE_T) 0))    /**< Handle group type for the Network list handle. */

/* Notification conditions. */
/*------------------------------------------------------------------*/
/*! @brief  Notification conditions.
 *  @code
 *  typedef enum
 *  {
 *      NWL_UPDATING,   
 *      NWL_UPDATED,    
 *      NWL_CLOSED,     
 *      NWL_UNKNOWN
 *  }   NWL_COND_T;
 *  @endcode
 *  @li@c  NWL_UPDATING                  - When nwl_lock is called.
 *  @li@c  NWL_UPDATED                   - When nwl_unlock is called.
 *  @li@c  NWL_CLOSED                    - When nwl_delete is called.
 *  @li@c  NWL_UNKNOWN                   - Unknown condition.
 */
/*------------------------------------------------------------------*/
typedef enum
{
    
    NWL_UPDATING,   /* When nwl_lock() is called. */
    NWL_UPDATED,    /* When nwl_unlock() is called. */
    NWL_CLOSED,     /* When nwl_delete() is called. */
    NWL_UNKNOWN
    
}   NWL_COND_T;

/* prototype for the notification. */
/*------------------------------------------------------------------*/
/*! @brief  Prototype for the notification.
 *  @param  [in]  h_nwl             - Handle to the NWL object.
 *  @param  [in]  e_cond            - The status flag for the NWL object.
 *  @param  [in]  ui4_reason        - The reason for the notification.
 *  @param  [in]  pv_tag            - Private tag for the client.
 *  @param  [in]  ui4_data          - Data pass back to the application.
 *  @note   It is very important that the client MUST NOT try to read the NWL in
 *          the context of notification function.  Otherwise, a `Dead lock'
 *          situation would arise.  The client should only send a message to
 *          itself from the notification function or set a condition flag, and
 *          execute the read operation outside the context of the notification
 *          function.
 *          @par
 *          Client should also avoid doing CPU intensive task in the
 *          notification function.  If CPU intensive task needs to be performed,
 *          then the notification function should send a message to the client
 *          thread, and have the CPU intensive task executed in the client
 *          thread context.
 *  @see    c_nwl_create, c_nwl_open, c_nwl_open_by_name, c_nwl_fs_load
 *  @return None
 */
/*------------------------------------------------------------------*/
typedef VOID (*x_nwl_nfy_fct) (HANDLE_T          h_nwl,
                               NWL_COND_T        e_cond,
                               UINT32            ui4_reason,
                               VOID*             pv_tag,
                               UINT32            ui4_data);

/* Notification reasons */
#define NWL_REASON_UNKNOWN  ((UINT32)( 0x0000 ))   /**< Reason unknown */
#define NWL_RECORD_ADD      MAKE_BIT_MASK_32(1)    /**< Reason for adding records */
#define NWL_RECORD_DEL      MAKE_BIT_MASK_32(2)    /**< Reason for deleting records */
#define NWL_RECORD_MOD      MAKE_BIT_MASK_32(3)    /**< Reason for modifying records */

/* Maximum length for a name specifying a Network List object. */
#define     MAX_NWL_NAME_LEN    32    /**< Maximum length for a name specifying a Network List object. */

#define     NWL_NULL_ID         (0)    /**< NULL NWL ID value. */

/* specified null record id value.  Note:
   valid network record id is from 1 to 2**16
*/
#define     NWL_NULL_REC_ID     ((UINT16)0)    /**< NULL record ID value. Note: valid network record id is from 1 to 2**16 */

#define     NWL_NULL_VER_ID     ((UINT32)0)    /**< Null NWL database version id. */


/* Typedef and constants for NWL record id query flag. */
typedef     UINT8                 NWL_REC_ID_QRY_T;    /**< Typedef for NWL record id query flag. */

#define     NWL_FIRST_REC_ID      ((NWL_REC_ID_QRY_T) 1)    /**< First record ID. */
#define     NWL_LAST_REC_ID       ((NWL_REC_ID_QRY_T) 2)    /**< Last record ID. */
#define     NWL_NEXT_REC_ID       ((NWL_REC_ID_QRY_T) 3)    /**< Next record ID. */


/*------------------------------------------------------
  data structure and typedef.
------------------------------------------------------*/

/* Structure for the NWL data record */
/*------------------------------------------------------------------*/
/*!  @brief  Structure for the NWL data record
 *  @code
 *  typedef struct _NWL_REC_T
 *  {
 *      UINT32        ui4_nw_mask;
 *  	UINT16        ui2_nwl_rec_id;
 *      UINT16        ui2_nw_id; 
 *      UINT8         ui1_nit_ver;
 *      CHAR          s_nw_name[MAX_NETWORK_NAME_LEN];
 *  } NWL_REC_T;
 *  @endcode
 *  @li@c  ui4_nw_mask                   - Network mask (not used).
 *  @li@c  ui2_nwl_rec_id                - NWL record id.
 *  @li@c  ui2_nw_id                     - Network id.
 *  @li@c  ui1_nit_ver                   - NIT version ID.
 *  @li@c  s_nw_name                     - Network name.
 */
/*------------------------------------------------------------------*/
typedef struct _NWL_REC_T
{
    /* Mask */
    UINT32        ui4_nw_mask;

    /* NWL record id */
	UINT16        ui2_nwl_rec_id;

    /*SATL id & record id*/
    UINT16        ui2_satl_id;
    UINT16        ui2_satl_rec_id;

    /* Network id */
    UINT16        ui2_nw_id; 

    /* NIT version ID */
    UINT8         ui1_nit_ver;
  
    /* Network name */
    CHAR          s_nw_name[MAX_NETWORK_NAME_LEN];

} NWL_REC_T;

#define  NWL_REC_SIZE (4+2+2*2+2+4+MAX_NETWORK_NAME_LEN)    /**< Pre-set size of the NWL record structure. */


typedef  UINT8       NWL_MEM_POLICY_T;    /**< Typedef for NWL memory mangement policy. */

#define  NWL_MEM_STATIC   ((NWL_MEM_POLICY_T) 0)    /**< Static memory policy. */
#define  NWL_MEM_DYNAMIC  ((NWL_MEM_POLICY_T) 1)    /**< Dynamic memory policy. */


/*
 * Structure for memory dump.
 */
/*------------------------------------------------------------------*/
/*!  @brief  Structure for memory dump.
 *  @code
 *  typedef struct _NWL_MEM_DUMP_T_
 *  {
 *      UINT16      ui2_nwl_id;
 *      CHAR        ps_nwl_name[MAX_NWL_NAME_LEN+1];
 *      VOID*       pv_data;
 *      UINT32      ui4_data_len;
 *  }  NWL_MEM_DUMP_T ;
 *  @endcode
 *  @li@c  ui2_nwl_id                    - ID of the Network List
 *  @li@c  ps_nwl_name                   - Name of the Network List
 *  @li@c  pv_data                       - Memory dump of NWL database
 *  @li@c  ui4_data_len                  - Size of memory allocated for pv_data
 */
/*------------------------------------------------------------------*/
typedef struct _NWL_MEM_DUMP_T_
{
    UINT16      ui2_nwl_id;
    CHAR        ps_nwl_name[MAX_NWL_NAME_LEN+1];
    VOID*       pv_data;
    UINT32      ui4_data_len;
}  NWL_MEM_DUMP_T ;

/*
  typedef for NWL user defined compare function that implement customized
  sorting order of NWL record.  This function is called during the
  sorting operation.
*/
/*------------------------------------------------------------------*/
/*! @brief  Typedef for NWL user defined compare function that implement
 *          customized sorting order of NWL record.  This function is called
 *          during the sorting operation.
 *  @param  [in]  pt_nwl_rec_1      - Reference a NWL record structure.
 *  @param  [in]  pt_nwl_rec_2      - Reference a NWL record structure.
 *  @param  [in]  pv_tag            - Private tag info pass to the comparison
 *                                    function.
 *  @return Comparison result
 *  @retval RC_SMALLER          - `pt_nwl_rec_1' is less than `pt_nwl_rec_2'.
 *  @retval RC_EQUAL            - `pt_nwl_rec_1' is equal to  `pt_nwl_rec_2'.
 *  @retval RC_GREATER          - `pt_nwl_rec_1' is greater than `pt_nwl_rec_2'.
 */
/*------------------------------------------------------------------*/
typedef INT32 (*x_nwl_sort_cmp)
(
    const NWL_REC_T*     pt_nwl_rec_1,
    const NWL_REC_T*     pt_nwl_rec_2,
    VOID*                pv_tag
);

/*
  typedef for NWL user defined search compare function. This
  function will be called by the NWL library when performing
  binary search operation on sorted NWL records.
  For binary search operation, this function must implement the
  same logic order as implemented by the x_nwl_sort_cmp function.
*/
/*------------------------------------------------------------------*/
/*! @brief  Typedef for NWL user defined search compare function. This
 *          function will be called by the NWL library when performing
 *          binary search operation on sorted NWL records.
 *          For binary search operation, this function must implement the
 *          same logic order as implemented by the x_nwl_sort_cmp function.
 *  @param  [in]  pt_nwl_rec        - Reference a NWL record structure.
 *  @param  [in]  pv_data_to_compare - Value parameter to compare with NWL
 *                                     record `pt_nwl_rec'.
 *  @param  [in]  pv_tag            - Private tag info pass to the comparison
 *                                    function.
 *  @return Comparison result
 *  @retval RC_SMALLER      - `pt_nwl_rec_1' is less than `pv_data_to_compare'.
 *  @retval RC_EQUAL        - `pt_nwl_rec_1' is equal to `pv_data_to_compare'.
 *  @retval RC_GREATER      - `pt_nwl_rec_1' is greater than `pt_nwl_rec_2'.
 */
/*------------------------------------------------------------------*/
typedef INT32 (*x_nwl_search_cmp)
(
    const NWL_REC_T*     pt_nwl_rec,
    const VOID*          pv_data_to_compare,
    VOID*                pv_tag
);


/*----------------------------------------------------------------------------*/
/*! @} */
/*----------------------------------------------------------------------------*/
#endif  /* _U_NWL_H_ */
