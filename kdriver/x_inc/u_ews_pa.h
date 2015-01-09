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
/*! @file u_mheg5.h 
 *  $RCSfile: $
 *  $Revision: #1 $
 *  $Date: 2015/01/09 $
 *  $Author: dtvbm11 $
 *  
 *  @par Description:This header file contains MHEG5 exported constants, macros, and types.
 */
/*----------------------------------------------------------------------------*/
#ifndef _U_EWS_PA_H_
#define _U_EWS_PA_H_
/*-----------------------------------------------------------------------------
                    include files
-----------------------------------------------------------------------------*/
#include "u_common.h"
#include "u_handle.h"
#include "x_dbg.h"


/*!
 * @addtogroup    groupMW_EWS_PA
 * @brief    This header file contains EWS_PA exported constants, macros, and types.
 * 
 * @see groupMW_EWS_PA
 *
 * @{
 * */


/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
-----------------------------------------------------------------------------*/
#define EWSPAR_ASYNC_RESPONSE       ((INT32)    1)    /**<Indicate the request will be responsed asynchronously        */

#define EWSPAR_OK                   ((INT32)    0)    /**<OK        */
#define EWSPAR_INV_ARG              ((INT32) -  1)    /**<Invalid arguments        */
#define EWSPAR_INV_HANDLE           ((INT32) -  2)    /**<Invalid handle        */
#define EWSPAR_OUT_OF_MEMORY        ((INT32) -  3)    /**<Out of memory        */
#define EWSPAR_OUT_OF_HANDLE        ((INT32) -  4)    /**<Out of handle        */
#define EWSPAR_INTERNAL_ERROR       ((INT32) -  5)    /**<Internal error        */
#define EWSPAR_NOT_INIT             ((INT32) -  6)    /**<Not initialized       */
#define EWSPAR_ALREADY_INIT         ((INT32) -  7)    /**<Already initialized        */
#define EWSPAR_INIT_FAIL            ((INT32) -  8)    /**<Failed to initialized        */
#define EWSPAR_INV_STATE            ((INT32) -  9)    /**<Invalid state        */
#define EWSPAR_SVC_NOT_FOUND        ((INT32) - 10)    /**<Service is not found       */
#define EWSPAR_ENG_NOT_FOUND        ((INT32) - 11)    /**<Engine is not found        */
#define EWSPAR_CONNECT_FAIL         ((INT32) - 12)    /**<Failed to connect        */
#define EWSPAR_LOAD_NIT_FAIL        ((INT32) - 13)    /**<Failed to load NIT        */
#define EWSPAR_LOAD_SDT_FAIL        ((INT32) - 14)    /**<Failed to load SDT        */
#define EWSPAR_LOAD_PAT_FAIL        ((INT32) - 15)    /**<Failed to load PAT        */
#define EWSPAR_LOAD_PMT_FAIL        ((INT32) - 16)    /**<Failed to LOAD PMT        */
#define EWSPAR_LOAD_TRDW_FAIL       ((INT32) - 17)    /**<Failed to LOAD TRDW        */
#define EWSPAR_LOAD_TCDW_FAIL       ((INT32) - 18)    /**<Failed to LOAD TCDW        */
#define EWSPAR_LOAD_TMDW_FAIL       ((INT32) - 19)    /**<Failed to LOAD TMDW        */
#define EWSPAR_OUT_OF_RESOURCES     ((INT32) - 20)    /**<Out of semaphores        */
#define EWSPAR_PARSE_PMT_FAIL       ((INT32) - 21)    /**<Failed to parse PMT        */
#define EWSPAR_PARSE_SDT_FAIL       ((INT32) - 22)    /**<Failed to parse SDT        */
#define EWSPAR_PARSE_PAT_FAIL       ((INT32) - 23)    /**<Failed to parse PAT        */
#define EWSPAR_PARSE_TRDW_FAIL      ((INT32) - 24)    /**<Failed to parse TRDW        */
#define EWSPAR_PARSE_TCDW_FAIL      ((INT32) - 25)    /**<Failed to parse TCDW        */
#define EWSPAR_PARSE_TMDW_FAIL      ((INT32) - 26)    /**<Failed to parse TMDW        */
#define EWSPAR_COMP_ALREADY_STARTED ((INT32) - 27)    /**<The EWS_PA component has already been started by a call to tmMHEG5Start()        */
#define EWSPAR_CREATE_TIMER_FAIL    ((INT32) - 28)    /**<Failed to create a timer        */
#define EWSPAR_START_TIMER_FAIL     ((INT32) - 29)    /**<Failed to start a timer        */
#define EWSPAR_MISMATCHED_RUN       ((INT32) - 30)    /**<Mismatched run        */
#define EWSPAR_SVC_UNAVAILABLE      ((INT32) - 31)    /**<Service is unavailable        */
#define EWSPAR_COMP_NOT_OPEN        ((INT32) - 32)    /**<The EWS_PA component has not been opened by a call to tmMHEG5Open()        */
#define EWSPAR_COMP_ALREADY_OPEN    ((INT32) - 33)    /**<The EWS_PA component has already been opened by a call to tmMHEG5Open()        */
#define EWSPAR_COMP_NOT_STARTED     ((INT32) - 34)    /**<The EWS_PA component has not been started by a call to tmMHEG5Start()         */
#define EWSPAR_INV_SET_INFO         ((INT32) - 35)    /**<Invalid set info*/
#define EWSPAR_INV_GET_INFO         ((INT32) - 36)    /**<Invalid get info*/


typedef UINT32 EWS_PA_SET_TYPE_T;    /**< EWS_PA set type. */
typedef UINT32 EWS_PA_GET_TYPE_T;    /**< EWS_PA get type. */

#define EWS_PA_SET_TYPE_IGNORE                      ((EWS_PA_SET_TYPE_T)( 0)) /**< Ignore         */
#define EWS_PA_SET_LOCATION_CODE                    ((EWS_PA_SET_TYPE_T)( 1)) /**< type = UINT8*  */


#define EWS_PA_GET_TYPE_IGNORE                      ((EWS_PA_GET_TYPE_T)( 0)) /**< Ignore */
#define EWS_PA_GET_TYPE_SVC_INFO                    ((EWS_PA_GET_TYPE_T)( 1)) /**< type = EWS_PA_SVC_INFO_T*,             size = sizeof(DEVICE_TYPE_T)               */    
#define EWS_PA_GET_TYPE_INFO_MSG_NUM                ((EWS_PA_GET_TYPE_T)( 2)) /**< type = EWS_PA_SVC_INFO_T*,             size = sizeof(DEVICE_TYPE_T)               */    

#define MIN(x, y)                       (((x) < (y)) ? (x) : (y))

#define EWS_PA_ASSERT(cond)                                              \
{                                                                       \
    if (!(cond))                                                        \
    {                                                                   \
        DBG_ERROR(("\n File : %s, Line : %d ", __FILE__, __LINE__));     \
        DBG_ABORT(DBG_MOD_EWS_PA);                                       \
    }                                                                   \
}   

/*------------------------------------------------------------------*/
/*! @struct EWS_PA_TMDW_INFO_T
 *  @brief EWS_PA_TMDW_INFO message structure
 *  @code
 *  typedef struct _EWS_PA_TMDW_INFO_T
 *  {
 *      UINT8        ui1_info_msg_len;
 *      CHAR*        ps_info_msg;
 *  } EWS_PA_TMDW_INFO_T;
 *  @endcode
 *  @li@c  ui1_info_msg_len                           - TMDW info message length
 *  @li@c  ps_info_msg                                - TMDW info message
 */
/*------------------------------------------------------------------*/
typedef struct _EWS_PA_TMDW_INFO_T
{
    #if 0
    UINT8                           ui1_info_msg_len;
    #endif
    CHAR*                           ps_info_msg; 
    
}EWS_PA_TMDW_INFO_T;

/*------------------------------------------------------------------*/
/*! @struct EWS_PA_SVC_INFO_T
 *  @brief EWS_PA_SVC_INFO message structure
 *  @code
 *typedef struct _EWS_PA_SVC_INFO_T
 *{
 *    UINT16                          ui2_disaster_code;
 *    UINT8                           ui1_package_id;
 *    UINT8                           ui1_location_type_code;
 *    
 *    UINT8                           aui1_location_code[3];
 *    
 *    UINT8                           ui1_authority;
 *    #if 0
 *    UINT8                           ui1_disaster_code_len;
 *    UINT8                           ui1_disaster_date_len;
 *    UINT8                           ui1_disaster_position_len;
 *    UINT8                           ui1_disaster_characterstic_len;
 *    UINT8                           ui1_location_code_len;
 *    #endif
 *
 *    UINT8                           ui2_info_msg_num;
 *
 *    CHAR*                           ps_location_code;
 *    CHAR*                           ps_disaster_code;
 *    CHAR*                           ps_disaster_position;    
 *    CHAR*                           ps_disaster_date;    
 *    CHAR*                           ps_disaster_characterstic; 
 *
 *    _EWS_PA_TMDW_INFO_T*            pt_info_msg;
 *    
 *  @endcode
 *  @li@c  ui2_disaster_code           - 
 *  @li@c  ui1_package_id              -  
 *  @li@c  ui1_location_type_code      -  
 *  @li@c  aui1_location_code[3]       -  
 *  @li@c  ui1_authority               -  
 *  @li@c  ui2_info_msg_num            -  
 *  @li@c  ps_location_code            -  
 *  @li@c  ps_disaster_code            -  
 *  @li@c  ps_disaster_position        -  
 *  @li@c  ps_disaster_characterstic   -  
 *  @li@c  pt_info_msg                 -    
 */
/*------------------------------------------------------------------*/

typedef struct _EWS_PA_SVC_INFO_T
{
    UINT16                          ui2_disaster_code;
    UINT8                           ui1_package_id;
    UINT8                           ui1_location_type_code;
    
    UINT8                           aui1_location_code[3];
    
    UINT8                           ui1_authority;
    UINT8                           ui1_trdw_version;
    UINT8                           ui1_tcdw_version;
    UINT8                           ui1_tmdw_version;
    #if 0
    UINT8                           ui1_disaster_code_len;
    UINT8                           ui1_disaster_date_len;
    UINT8                           ui1_disaster_position_len;
    UINT8                           ui1_disaster_characterstic_len;
    UINT8                           ui1_location_code_len;
    #endif

    UINT16                          ui2_info_msg_num;

    CHAR*                           ps_location_code;
    CHAR*                           ps_disaster_code;
    CHAR*                           ps_disaster_position;    
    CHAR*                           ps_disaster_date;    
    CHAR*                           ps_disaster_characterstic; 

    EWS_PA_TMDW_INFO_T*             pt_info_msg;
    
}EWS_PA_SVC_INFO_T;

/*------------------------------------------------------------------*/
/*! @struct EWS_PA_MSG_T
 *  @brief EWS_PA message structure
 *  @code
 *  typedef struct _EWS_PA_MSG_T
 *  {
 *      UINT32        ui4_inst_id;
 *      UINT16        ui2_msg_type;
 *      VOID*         pv_nfy_tag;
 *  } EWS_PA_MSG_T;
 *  @endcode
 *  @li@c  ui4_inst_id                                   - Instance ID
 *  @li@c  ui2_msg_type                               - Message type
 *  @li@c  pv_nfy_tag                                  - Tag
 */
/*------------------------------------------------------------------*/
typedef struct _EWS_PA_MSG_T
{
    UINT16         ui2_msg_type;
    HANDLE_T       h_tuner;
    HANDLE_T       h_obj;
    UINT16         ui2_seq_num;
    VOID*          pv_nfy_tag;    
} EWS_PA_MSG_T;

/*------------------------------------------------------------------*/
/*! @enum EWS_PA_NFY_COND_T
 *  @brief EWS_PA notify condition
 *  @code
 *  typedef enum _EWS_PA_NFY_COND_T
 *  {
 *      MHEG5_NFY_COND_TABLE_LOADED,
 *      MHEG5_NFY_COND_TABLE_FAILED,    
 *      MHEG5_NFY_COND_DATA_BROADCASD_ID_DESC_RETURNS,
 *      MHEG5_NFY_COND_DATA_BROADCASD_ID_DESC_GONE,
 *      MHEG5_NFY_COND_NB_ACTION_INITIATE_AUTOBOOT,
 *      MHEG5_NFY_COND_CAROUSEL_ID_CHANGED,
 *      MHEG5_NFY_COND_SERVICE_GONE,
 *      MHEG5_NFY_COND_SERVICE_RETURNS,
 *      MHEG5_NFY_COND_SERVICE_CHANGED
 *  } MHEG5_NFY_COND_T;
 *  @endcode
 *  @li@c  MHEG5_NFY_COND_TABLE_LOADED                       - SI table loaded well
 *  @li@c  MHEG5_NFY_COND_TABLE_FAILED                        - SI table loaded failed
 *  @li@c  MHEG5_NFY_COND_DATA_BROADCASD_ID_DESC_RETURNS     - Broadcast ID descriptor returns
 *  @li@c  MHEG5_NFY_COND_DATA_BROADCASD_ID_DESC_GONE        - Broadcast ID descriptor gone
 *  @li@c  MHEG5_NFY_COND_NB_ACTION_INITIATE_AUTOBOOT        - initiate autoboot
 *  @li@c  MHEG5_NFY_COND_CAROUSEL_ID_CHANGED                - Carousel ID chanaged
 *  @li@c  MHEG5_NFY_COND_SERVICE_GONE                       - Service gone
 *  @li@c  MHEG5_NFY_COND_SERVICE_RETURNS                    - Service returns
 *  @li@c  MHEG5_NFY_COND_SERVICE_CHANGED                    - Service changed
 */
/*------------------------------------------------------------------*/
typedef enum _EWS_PA_NFY_COND_T
{
    EWS_PA_NFY_COND_TABLE_ERROR,
    EWS_PA_NFY_COND_SERVICE_GONE,
    EWS_PA_NFY_COND_SERVICE_FOUND,
    EWS_PA_NFY_COND_SERVICE_CHANGED
} EWS_PA_NFY_COND_T;

/*------------------------------------------------------------------*/
/*! @brief Callback to notify the SI loading status
 *  @param [in] e_cond - EWS_PA notify condition
 *  @param [in] pv_tag - User's tag
 *  @param [out] None.
 */
/*------------------------------------------------------------------*/
typedef INT32 (*x_ews_pa_eng_load_si_nfy)(
    EWS_PA_NFY_COND_T    e_cond,
    VOID*                pv_tag);


/*------------------------------------------------------------------*/
/*! @struct EWS_PA_FUNC_TBL_T
 *  @brief  EWS_PA Instance function table
 *  @code
 *  typedef struct _EWS_PA_FUNC_TBL_T
 *  {
 *      x_ews_pa_eng_load_si_nfy                 pf_load_si_nfy;
 *  } EWS_PA_FUNC_TBL_T;
 *  @endcode
 *  @li@c  pf_load_si_nfy                       - EWS_PA COMP function: Load SI NFY
 *------------------------------------------------------------------*/
typedef struct _EWS_PA_FUNC_TBL_T
{
    x_ews_pa_eng_load_si_nfy                     pf_load_si_nfy;
} EWS_PA_FUNC_TBL_T;



/*! @}  */

#endif /* _U_EWS_PA_H_ */


