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
/*! @file u_ci.h 
 *  $RCSfile: $
 *  $Revision: #1 $
 *  $Date: 2015/01/09 $
 *  $Author: dtvbm11 $
 *  
 *  @par Description:
 *         This header file contains Common Interface (CI) specific
 *         definitions, which are exported.
 */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*! @defgroup groupMW_CI  CI Engine
 *
 *  @ingroup groupMW
 *  @brief Provides constant,type definitions and function of the CI Engine.. CI Engine is a middleware component that is responsible for communication between Host and CICAM and  descrambling the scrambled the stream components before passing them to demux
 *
 *  %MW Application API would be used by TV Application, lot of these APIs would
 *   be wrapped from Exported API
 *
 *  @see groupMW
 *  @{ 
 */
/*----------------------------------------------------------------------------*/

#ifndef _U_CI_H_
#define _U_CI_H_

/*-------------------------------------------------------------------------
                    include files
-------------------------------------------------------------------------*/
#include "u_common.h"
#include "u_handle.h"
#include "u_lnk_list.h"
#include "u_prc_ca.h"
/*-------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ------------------------------------------------------------------------*/
/*------------------------------------------------------------------*/
/*! @enum CI_NFY_COND_T
 *  @brief Used in x_ci_nfy_fct() callback function to notify message received from Card
 *  @code
 *  typedef enum
 *  {
 *      CI_NFY_COND_CARD_INSERT = 0,
 *      CI_NFY_COND_CARD_NAME,
 *      CI_NFY_COND_CARD_REMOVE,
 *      CI_NFY_COND_MMI_ENQUIRY,
 *      CI_NFY_COND_MMI_MENU,
 *      CI_NFY_COND_MMI_LIST,
 *      CI_NFY_COND_MMI_CLOSE,
 *      CI_NFY_COND_HOST_TUNE,
 *      CI_NFY_COND_HOST_REPLACE,
 *      CI_NFY_COND_HOST_CLEAR_REPLACE,
 *      CI_NFY_COND_HOST_SESS_CLOSE,
 *      CI_NFY_COND_CA_SYSTEM_ID_WAIT,
 *      CI_NFY_COND_CA_SYSTEM_ID_READY,
 *      CI_NFY_COND_CA_SYSTEM_ID_MATCH,
 *      CI_NFY_COND_FIRMWARE_UPGRADE,
 *      CI_NFY_COND_FIRMWARE_UPGRADE_PROGRESS,        
 *      CI_NFY_COND_REQUEST_START,
 *      CI_NFY_COND_APP_ABORT_REQUEST,
 *      CI_NFY_COND_CIPLUS_CAM_STATUS,
 *      CI_NFY_COND_ERROR,
 *      CI_NFY_COND_PROFILE_SEARCH_REQUEST,
 *      CI_NFY_COND_PROFILE_SEARCH_SCHEDULE,
 *      CI_NFY_COND_PROFILE_SEARCH_STARTED,
 *      CI_NFY_COND_PROFILE_SEARCH_ENDED,
 *      CI_NFY_COND_PROFILE_SEARCH_CANCELED,
 *      CI_NFY_COND_PROFILE_TYPE,
 *      CI_NFY_COND_PIN_CAP_CHANGE,
 *      CI_NFY_COND_RECORD_START_STATUS,
 *      CI_NFY_COND_PIN_EVENT,
 *      CI_NFY_COND_PIN_REPLY,
 *      CI_NFY_COND_RECORD_LICENSE,
 *      CI_NFY_COND_RECORD_BLOCK_SVC,
 *      CI_NFY_COND_SAS_SESS_OPENED,
 *      CI_NFY_COND_SAS_CONNECT_CONFIRM,
 *      CI_NFY_COND_SAS_SGUI_STATE_EVENT,
 *      CI_NFY_COND_SAS_SESS_CLOSED
 *  } CI_NFY_COND_T;
 *  @endcode
 *  @li@c  CI_NFY_COND_CARD_INSERT                           -notify that the Card is insterted, 
 *                                                                                     pv_data in x_ci_nfy_fct is NULL 
 *  @li@c  CI_NFY_COND_CARD_NAME                             -notify that  the Card's name is ready
 *                                                                                     pv_data in x_ci_nfy_fct is NULL 
 *  @li@c  CI_NFY_COND_CARD_REMOVE                           -notify that the Card is removed
 *                                                                                     pv_data in x_ci_nfy_fct is NULL 
 *  @li@c  CI_NFY_COND_MMI_ENQUIRY                           -notify the MMI Enq object 
 *                                                                                     pv_data in x_ci_nfy_fct is CI_MMI_ENQ_T pointer 
 *  @li@c  CI_NFY_COND_MMI_MENU                              -notify the MMI Menu object
 *                                                                                     pv_data in x_ci_nfy_fct is CI_MMI_MENU_T pointer 
 *  @li@c  CI_NFY_COND_MMI_LIST                              -notify the MMI List object
 *                                                                                     pv_data in x_ci_nfy_fct is CI_MMI_MENU_T pointer 
 *  @li@c  CI_NFY_COND_MMI_CLOSE                             -notify that  the MMI Menu or List  is required to close
 *                                                                                     pv_data in x_ci_nfy_fct is CI_MMI_ENQ_CLOSE_T pointer 
 *  @li@c  CI_NFY_COND_HOST_SESS_OPEN                        -notify the host control session is opened
 *                                                                                     pv_data in x_ci_nfy_fct is UINT32 value indicate slot id
 *  @li@c  CI_NFY_COND_HOST_TUNE                             -notify that  the Card requires to tune service
 *                                                                                     pv_data in x_ci_nfy_fct is CI_HC_TUNE_T pointer 
 *  @li@c  CI_NFY_COND_HOST_REPLACE                          -notify that  the Card requires to replace one stream component for current service
 *                                                                                     pv_data in x_ci_nfy_fct is CI_HC_REPLACE_T pointer 
 *  @li@c  CI_NFY_COND_HOST_CLEAR_REPLACE                    -notify that the Card requires to recover the replaced  stream component for current service
 *                                                                                     pv_data in x_ci_nfy_fct is CI_HC_CLR_REPLACE_T pointer 
 *  @li@c  CI_NFY_COND_HOST_SESS_CLOSE                       -notify the session is closed
 *                                                                                     pv_data in x_ci_nfy_fct is UINT32 value indicate slot id
 *  @li@c  CI_NFY_COND_CA_SYSTEM_ID_WAIT                     -notify the ca system id is not ready while selecting program
 *                                                                                     pv_data in x_ci_nfy_fct is UINT32 value indicate slot id
 *  @li@c  CI_NFY_COND_CA_SYSTEM_ID_READY                    -notify that the CA system ID is ready
 *                                                                                     pv_data in x_ci_nfy_fct is UINT8 value indicate slot id
 *  @li@c  CI_NFY_COND_CA_SYSTEM_ID_MATCH                    -notify that the CA system ID is math with pmt's ca system id for current service
 *                                                                                     pv_data in x_ci_nfy_fct is CI_CA_SYSTEM_ID_MATCH_T* value indicates if is match.
 *  @li@c  CI_NFY_COND_FIRMWARE_UPGRADE                    -notify that the CAM request to upgrade software on Host
 *                                                                                     pv_data in x_ci_nfy_fct is CI_CAM_UPGRADE_T* value indicates upgrade type.
 *  @li@c  CI_NFY_COND_FIRMWARE_UPGRADE_PROGRESS    -notify that the CAM upgrade progress
 *                                                                                     pv_data in x_ci_nfy_fct is CI_CAM_UPGRADE_PROGRESS_T* value indicates upgrade progress.
 *  @li@c  CI_NFY_COND_REQUEST_START                            -notify that the CAM request to start APP
 *                                                                                     pv_data in x_ci_nfy_fct is CI_REQUEST_START_T* value indicates APP's infomation.
 *  @li@c  CI_NFY_COND_APP_ABORT_REQUEST                     -notify that the CAM request to abort APP
 *                                                                                     pv_data in x_ci_nfy_fct is CI_ABORT_REQUEST_T* value indicates APP abort reason.
 *  @li@c  CI_NFY_COND_CIPLUS_CAM_STATUS                      -notify that the CAM request to abort APP
 *                                                                                     pv_data in x_ci_nfy_fct is UINT8 value indicates slot id.
 *  @li@c  CI_NFY_COND_ERROR                                        -notify that the error occured
 *  @li@c  CI_NFY_COND_HOST_TUNE_BROADCAST                    -notify that the Card requires to recover the replaced  stream component for current service
 *                                                                                     pv_data in x_ci_nfy_fct is CI_HC_TUNE_BROADCAST_T pointer 
 *  @li@c  CI_NFY_COND_ASK_RELEASE_REPLY                   -notify that the error occured
 *                                                                                     pv_data in x_ci_nfy_fct is CI_NFY_ERROR_DATA_T* value indicates error code.
 *  @li@c  CI_NFY_COND_HOST_TUNE_BROADCAST             -notify that the error occured
 *                                                                                     pv_data in x_ci_nfy_fct is CI_NFY_ERROR_DATA_T* value indicates error code.
 *  @li@c  CI_NFY_COND_HOST_TUNE_BROADCAST_FAILED     -notify that the error occured
 *                                                                                     pv_data in x_ci_nfy_fct is CI_NFY_ERROR_DATA_T* value indicates error code.
 *  @li@c   CI_NFY_COND_PROFILE_SEARCH_REQUEST          -notify that cam profile search request
 *                                                                                     pv_data in x_ci_nfy_fct is CI_CAM_SCAN_REQ_T* value indicates profile search request data.
 *  @li@c   CI_NFY_COND_PROFILE_SEARCH_STARTED          -notify that cam profile search is started
 *                                                                                     pv_data in x_ci_nfy_fct is UINT32 value indicate slot id
 *  @li@c   CI_NFY_COND_PROFILE_SEARCH_ENDED          -notify that cam profile search is ended
 *                                                                                     pv_data in x_ci_nfy_fct is UINT32 value indicate slot id
 *  @li@c   CI_NFY_COND_PROFILE_SEARCH_CANCELED    -notify that cam profile search is canceld
 *                                                                                     pv_data in x_ci_nfy_fct is UINT32 value indicate slot id
  *  @li@c   CI_NFY_COND_PROFILE_NIT_UPDATE_REQ      -notify that cam profile search is canceld
  *                                                                                     pv_data in x_ci_nfy_fct is private data used in CI_SET_TYPE_PROFILE_NIT_UPDATE_START 
  *  @li@c   CI_NFY_COND_PROFILE_NAME_CHG                -notify that cam profile name is changed
  *                                                                                     pv_data in x_ci_nfy_fct is UINT32 value indicate slot id
  *  @li@c   CI_NFY_COND_PROFILE_ISO_639_LANG_CODE_CHG    -notify that cam iso 639 language code is changed
  *                                                                                     pv_data in x_ci_nfy_fct is UINT32 value indicate slot id
   *  @li@c   CI_NFY_COND_PROFILE_TYPE                        -notify that cam profile type changed
   *                                                                                     pv_data in x_ci_nfy_fct is CI_PROFILE_TYPE_T value 
  *  @li@c   CI_NFY_COND_PIN_CAP_CHANGE                    -notify that cam pin capability change
  *                                                                                     pv_data in x_ci_nfy_fct is CI_PIN_CAPS_T* value 
  *  @li@c   CI_NFY_COND_RECORD_START_STATUS            -notify that record start status
  *                                                                                     pv_data in x_ci_nfy_fct is CI_RECORD_START_STATUS_T* value 
  *  @li@c   CI_NFY_COND_RECORD_MODE_CHG_STATUS     -notify that record mode change status
  *                                                                                     pv_data in x_ci_nfy_fct is CI_RECORDING_STATUS_T value 
  *  @li@c   CI_NFY_COND_PIN_EVENT                               -notify that rating event
 *                                                                                     pv_data in x_ci_nfy_fct is CI_PIN_EVENT_T* value 
 *  @li@c   CI_NFY_COND_PIN_REPLY                                -notify that pin status
 *                                                                                     pv_data in x_ci_nfy_fct is CI_PIN_STATUS_T value 
 *  @li@c   CI_NFY_COND_RECORD_LICENSE                      -notify that cam license
 *                                                                                     pv_data in x_ci_nfy_fct is CI_LICENSE_EVENT_T* value 
 *  @li@c   CI_NFY_COND_RECORD_BLOCK_SVC                  -notify that service need to be block/unblock for recording resource
 *                                                                                     pv_data in x_ci_nfy_fct is BOOL* value  true means need block service,false means unblock service
 *  @li@c  CI_NFY_COND_SAS_SESS_OPENED                   -notify that the SAS connection is opened
 *                                                                                     pv_data in x_ci_nfy_fct is NULL.
 *  @li@c  CI_NFY_COND_SAS_CONNECT_CONFIRM                -notify that SAS connection is confirmed by cam
 *                                                                                     pv_data in x_ci_nfy_fct is CI_PLUS_SAS_CONNECT_STATUS_T* value indicates sas connect confrim data
 *  @li@c  CI_NFY_COND_SAS_SGUI_STATE_EVENT                -notify that SAS Sgui state
 *                                                                                     pv_data in x_ci_nfy_fct is CI_PLUS_SAS_SGUI_STATE_T* value indicates sas sgui state data
 *  @li@c  CI_NFY_COND_SAS_SESS_CLOSED                   -notify that the SAS connection is closed
 *                                                                                     pv_data in x_ci_nfy_fct is NULL.
 *  @li@c  CI_NFY_COND_HC_ASK_RELEASE_TIMEOUT         -notify the host control session is opened
*                                                                                     pv_data in x_ci_nfy_fct is UINT32 value indicate slot id
 */
/*------------------------------------------------------------------*/
typedef enum
{
    CI_NFY_COND_CARD_INSERT = 0,
    CI_NFY_COND_CARD_NAME,
    CI_NFY_COND_CARD_REMOVE,
    CI_NFY_COND_MMI_ENQUIRY,
    CI_NFY_COND_MMI_MENU,
    CI_NFY_COND_MMI_LIST,
    CI_NFY_COND_MMI_CLOSE,
    CI_NFY_COND_HOST_SESS_OPEN,
    CI_NFY_COND_HOST_TUNE,
    CI_NFY_COND_HOST_REPLACE,
    CI_NFY_COND_HOST_CLEAR_REPLACE,
    CI_NFY_COND_HOST_SESS_CLOSE,
    CI_NFY_COND_CA_SYSTEM_ID_WAIT,
    CI_NFY_COND_CA_SYSTEM_ID_READY,
    CI_NFY_COND_CA_SYSTEM_ID_MATCH,
    CI_NFY_COND_FIRMWARE_UPGRADE,
    CI_NFY_COND_FIRMWARE_UPGRADE_PROGRESS,  
    CI_NFY_COND_FIRMWARE_UPGRADE_COMPLETE,
    CI_NFY_COND_REQUEST_START,
    CI_NFY_COND_APP_ABORT_REQUEST,
    CI_NFY_COND_CIPLUS_CAM_STATUS,
    CI_NFY_COND_ERROR,
    CI_NFY_COND_CARD_NEED_RESET,    
    CI_NFY_COND_ASK_RELEASE_REPLY,
    CI_NFY_COND_CICAM_SHUNNING_STATUS,
    CI_NFY_COND_HOST_TUNE_BROADCAST,    
    CI_NFY_COND_HOST_TUNE_BROADCAST_FAILED,
    CI_NFY_COND_PROFILE_SEARCH_REQUEST,
    CI_NFY_COND_PROFILE_SEARCH_STARTED,
    CI_NFY_COND_PROFILE_SEARCH_ENDED,
    CI_NFY_COND_PROFILE_SEARCH_CANCELED,
    CI_NFY_COND_PROFILE_NIT_UPDATE_REQ,
    CI_NFY_COND_PROFILE_NAME_CHG,
    CI_NFY_COND_PROFILE_ISO_639_LANG_CODE_CHG,
    CI_NFY_COND_PROFILE_TYPE,
    CI_NFY_COND_PIN_CAP_CHANGE,
    CI_NFY_COND_RECORD_START_STATUS,
    CI_NFY_COND_RECORD_MODE_CHG_STATUS,
    CI_NFY_COND_PIN_EVENT,
    CI_NFY_COND_PIN_REPLY,
    CI_NFY_COND_RECORD_LICENSE,
    CI_NFY_COND_RECORD_BLOCK_SVC,
    CI_NFY_COND_CA_SYSTEM_ID_PMT_LIST,
    CI_NFY_COND_CA_REPLY_DESCAMBLE_CAPABILITY,
    CI_NFY_COND_SAS_SESS_OPENED,
    CI_NFY_COND_SAS_CONNECT_CONFIRM,
    CI_NFY_COND_SAS_SGUI_STATE_EVENT,
    CI_NFY_COND_SAS_SESS_CLOSED,
    CI_NFY_COND_HC_ASK_RELEASE_TIMEOUT

} CI_NFY_COND_T;

/*------------------------------------------------------------------*/
/*! @enum CA_SYSTEM_ID_STATUS_T
 *  @brief Specify the current CA resouce status
 *  @code
 *  typedef enum
 *  {
 *     CA_SYSTEM_ID_INVALID = 0, 
 *     CA_SYSTEM_ID_WAIT,
 *     CA_SYSTEM_ID_READY,
 *     CA_SYSTEM_ID_MATCH,
 *     CA_SYSTEM_ID_NOT_MATCH
 *  } CA_SYSTEM_ID_STATUS_T;
 *  @endcode
 *  @li@c  CA_SYSTEM_ID_INVALID           - the initial status
 *  @li@c  CA_SYSTEM_ID_WAIT              - can inserted, wait for ca resouce from CAM
 *  @li@c  CA_SYSTEM_ID_READY             - Get CA system id from ca_info command
 *  @li@c  CA_SYSTEM_ID_MATCH             - Current PMT's ca system id match ca_info 
 *  @li@c  CA_SYSTEM_ID_NOT_MATCH         - Current PMT's ca system id match ca_info 
 *
 */
/*------------------------------------------------------------------*/
typedef enum
{
    CA_SYSTEM_ID_INVALID = 0, 
    CA_SYSTEM_ID_WAIT,
    CA_SYSTEM_ID_READY,
    CA_SYSTEM_ID_MATCH,
    CA_SYSTEM_ID_NOT_MATCH
} CA_SYSTEM_ID_STATUS_T;

/*------------------------------------------------------------------*/
/*! @enum CI_CAM_STATUS_T
 *  @brief Specify the current slot status
 *  @code
 *  typedef enum
 *  {
 *     CI_CAM_STATUS_NOT_INSTALLED,
 *     CI_CAM_STATUS_INITIALIZING,
 *     CI_CAM_STATUS_OPERATIONAL,
 *  } CI_CAM_STATUS_T;
 *  @endcode
 *  @li@c  CI_STATUS_NOT_INSTALLED           - the initial status
 *  @li@c  CI_STATUS_INITIALIZING              - can inserted, wait for ca resouce from CAM
 *  @li@c  CI_STATUS_OPERATIONAL             - Get CA system id from ca_info command
 *
 */
/*------------------------------------------------------------------*/

typedef enum {
    CI_CAM_STATUS_NOT_INSTALLED,
    CI_CAM_STATUS_INITIALIZING,
    CI_CAM_STATUS_OPERATIONAL
} CI_CAM_STATUS_T;
/*------------------------------------------------------------------*/
/*! @struct CI_STATUS_T
 *  @brief This structure contains slot status. 
 *  @code
 *  typedef struct _CI_STATUS_T
 *  {
 *      UINT8 ui1_slot_id;
 *      CI_CAM_STATUS_T  e_cam_status;
 *  } CI_STATUS_T;
 *  @endcode
 *  @li@c  ui1_slot_id                           -slot id, value range[0,slot num-1].
 *  @li@c  e_cam_status                    -current slot status
 */
/*------------------------------------------------------------------*/
typedef struct _CI_STATUS_T
{
    UINT8 ui1_slot_id;
    CI_CAM_STATUS_T	e_cam_status;
}CI_STATUS_T;

/*------------------------------------------------------------------*/
/*! @enum CI_QUERY_TYPE_T
 *  @brief  specifies query type
 *  @code
 * typedef enum
 * {
 *   CI_QUERY_TYPE_HOST_COUNTY = 0,
 *   CI_QUERY_TYPE_HOST_LANGUAGE,
 *   CI_QUERY_TYPE_HOST_REGION_ID,
 *   CI_QUERY_TYPE_HOST_CABLE_BRDCSTER,
 *   CI_QUERY_TYPE_HOST_BRDCST_MEDIUM,
 *   CI_QUERY_TYPE_HOST_SAT_INFO,
 *   CI_QUERY_TYPE_HOST_SAT_BRDCSTER
 * } CI_QUERY_TYPE_T;
 *  @endcode
 *  @li@c  CI_QUERY_TYPE_HOST_COUNTY                              - This type means query host country code
 *  @li@c  CI_QUERY_TYPE_HOST_LANGUAGE                          - This type means query host language code
 *  @li@c  CI_QUERY_TYPE_HOST_REGION_ID                         - This type means query host region id
 *  @li@c  CI_QUERY_TYPE_HOST_CABLE_BRDCSTER                    - This type means query host cable profile settings
 *  @li@c  CI_QUERY_TYPE_HOST_BRDCST_MEDIUM                     - This type means query host broadcast medium settings
 *  @li@c  CI_QUERY_TYPE_HOST_SAT_INFO                     - This type means query host sat settings
 *  @li@c  CI_QUERY_TYPE_HOST_SAT_BRDCSTER                     - This type means query host sat profile settings
 */
/*------------------------------------------------------------------*/
typedef enum
{
    CI_QUERY_TYPE_HOST_COUNTY = 0,
    CI_QUERY_TYPE_HOST_LANGUAGE,
    CI_QUERY_TYPE_HOST_REGION_ID,
    CI_QUERY_TYPE_HOST_CABLE_BRDCSTER,
    CI_QUERY_TYPE_HOST_BRDCST_MEDIUM,
    CI_QUERY_TYPE_HOST_SAT_INFO,
    CI_QUERY_TYPE_HOST_SAT_BRDCSTER
} CI_QUERY_TYPE_T;

/*------------------------------------------------------------------*/
/*! @enum CI_NFY_ERROR_CODE_T
 *  @brief  specifies CI Notify error code 
 *  @code
 *  typedef enum
 * {
 *   CI_NFY_ERR_FIRMWARE_UPGRADE_PROGRESS_TIMEOUT = 0,
 *   CI_NFY_ERR_FIRMWARE_UPGRADE_CANCEL = 1
 * }CI_NFY_ERROR_CODE_T;
 
 *  @endcode
 *  @li@c  CI_NFY_ERR_FIRMWARE_UPGRADE_PROGRESS_TIMEOUT                              - Indicate error reason is firmware upgrade progress timeout
 *  @li@c  CI_NFY_ERR_FIRMWARE_UPGRADE_CANCEL                              - Indicate error reason is firmware upgrade progress is canceled
 */
/*------------------------------------------------------------------*/
typedef enum
{
    CI_NFY_ERR_FIRMWARE_UPGRADE_PROGRESS_TIMEOUT = 0,
    CI_NFY_ERR_FIRMWARE_UPGRADE_CANCEL = 1
}CI_NFY_ERROR_CODE_T;
/*------------------------------------------------------------------*/
/*! @enum CI_PLUS_SAS_CONNECT_STATUS_T
 *  @brief This structure defines ci plus SAS connection status info.
 *  @code
 *  typedef enum
 *  {
 *      CI_PLUS_SAS_CONNECTION_ESTABLISHED,
 *      CI_PLUS_SAS_CONNECTION_NO_APPLICATION,
 *      CI_PLUS_SAS_CONNECTION_NOT_AVAILABLE
 *   }CI_PLUS_SAS_CONNECT_STATUS_T;
 
 *  @endcode
 *  @li@c  CI_PLUS_SAS_CONNECTION_ESTABLISHED          - Connection established
 *  @li@c  CI_PLUS_SAS_CONNECTION_NO_APPLICATION       - Connection denied-no associated vendor-specific Card application found.
 *  @li@c  CI_PLUS_SAS_CONNECTION_NOT_AVAILABLE          -Connection denied-no more connections available.
 */
/*------------------------------------------------------------------*/
typedef enum
{
  CI_PLUS_SAS_CONNECTION_ESTABLISHED = 0,
  CI_PLUS_SAS_CONNECTION_NO_APPLICATION,
  CI_PLUS_SAS_CONNECTION_NOT_AVAILABLE
}CI_PLUS_SAS_CONNECT_STATUS_T;

/*------------------------------------------------------------------*/
/*! @enum CI_PLUS_SAS_EVENT_T
 *  @brief This structure defines ci plus SAS connection status info.
 *  @code
 *  typedef enum
 *  {
 *   CI_PLUS_SAS_EVT_VID_DECODING_ERROR          = 1,
 *   CI_PLUS_SAS_EVT_VID_NO_DATA,
 *   CI_PLUS_SAS_EVT_VID_DECODING_ISSUE_RECOVERED,
 *   CI_PLUS_SAS_EVT_AUD_DECODING_ERROR,
 *   CI_PLUS_SAS_EVT_AUD_NO_DATA,
 *   CI_PLUS_SAS_EVT_AUD_DECODING_ISSUE_RECOVERED,
 *   CI_PLUS_SAS_EVT_TUNER_ISSUE,
 *   CI_PLUS_SAS_EVT_TUNER_ISSUE_RECOVERED,
 *   }CI_PLUS_SAS_EVENT_T;
 
 *  @endcode
 *  @li@c  CI_PLUS_SAS_EVT_VID_DECODING_ERROR       - VIDEO Decoding issue
 *  @li@c  CI_PLUS_SAS_EVT_VID_NO_DATA       - VIDEO NO DATA issue
 *  @li@c  CI_PLUS_SAS_EVT_VID_DECODING_ISSUE_RECOVERED       - VIDEO Decoding error and no data issue recovered
 *  @li@c  CI_PLUS_SAS_EVT_AUD_DECODING_ERROR       - Audio Decoding issue
 *  @li@c  CI_PLUS_SAS_EVT_AUD_NO_DATA       - Audio NO DATA issue
 *  @li@c  CI_PLUS_SAS_EVT_AUD_DECODING_ISSUE_RECOVERED       - Audio Decoding error and no data issue recovered
 *  @li@c  CI_PLUS_SAS_EVT_TUNER_ISSUE          - Signal issue(no signal or bad quality)
 *  @li@c  CI_PLUS_SAS_EVT_TUNER_ISSUE_RECOVERED          - Signal issue recovered
 */
/*------------------------------------------------------------------*/
typedef enum
{
    CI_PLUS_SAS_EVT_VID_DECODING_ERROR          = 1,
    CI_PLUS_SAS_EVT_VID_NO_DATA,
    CI_PLUS_SAS_EVT_VID_DECODING_ISSUE_RECOVERED,
    CI_PLUS_SAS_EVT_AUD_DECODING_ERROR,
    CI_PLUS_SAS_EVT_AUD_NO_DATA,
    CI_PLUS_SAS_EVT_AUD_DECODING_ISSUE_RECOVERED,
    CI_PLUS_SAS_EVT_TUNER_ISSUE,
    CI_PLUS_SAS_EVT_TUNER_ISSUE_RECOVERED,
}CI_PLUS_SAS_EVENT_T;
/*------------------------------------------------------------------*/
/*! @enum CI_PLUS_SAS_MSG_STATUS_T
 *  @brief This structure defines ci plus SAS connection status info.
 *  @code
 *  typedef enum
 *  {
 *      CI_PLUS_SAS_SGUI_STATE_NORMAL,
 *      CI_PLUS_SAS_SGUI_STATE_LL,
 *   }CI_PLUS_SAS_SGUI_STATE_T;
 
 *  @endcode
 *  @li@c  CI_PLUS_SAS_SGUI_STATE_NORMAL       - The iCAM on linearir mode or VOD stream mode
 *  @li@c  CI_PLUS_SAS_SGUI_STATE_LL          - The iCAM in SGUI mode requires low latency mode of TV
 */
/*------------------------------------------------------------------*/
typedef enum
{
    CI_PLUS_SAS_SGUI_STATE_NORMAL          = 0,
    CI_PLUS_SAS_SGUI_STATE_LL
}CI_PLUS_SAS_SGUI_STATE_T;

typedef UINT8 CI_REQUEST_START_ACK_CODE_T;/**< Defines APP start request acknowledge code    */
#define CI_REQUEST_START_ACK_CODE_OK         ((CI_REQUEST_START_ACK_CODE_T) 0x01)/**<   Start request is ok   */
#define CI_REQUEST_START_ACK_CODE_WRONG_API ((CI_REQUEST_START_ACK_CODE_T) 0x02)/**<   Start request is failed because using wrong api  */
#define CI_REQUEST_START_ACK_CODE_API_BUSY ((CI_REQUEST_START_ACK_CODE_T) 0x03)/**<   Start request is failed because host is busy now  */

typedef UINT8 CI_ABORT_REQUEST_CODE_T;/**< Defines APP abort request code    */
#define CI_ABORT_CODE_USER_CANCEL ((CI_ABORT_REQUEST_CODE_T)    0x01)/**<   Abort  request is trigged by user cancel   */
#define CI_ABORT_CODE_SYSTEM_CANCEL ((CI_ABORT_REQUEST_CODE_T)    0x02)/**<   Abort request is trigged by system cancel  */

typedef UINT8 CI_ABORT_ACK_CODE_T;/**<Defines APP abort ack code. It is defines by host now. */
#define CI_ABORT_ACK_CODE_CAM_REMOVED ((CI_ABORT_ACK_CODE_T)    0x01)/**<   APP abort is because of CAM removed   */
#define CI_ABORT_ACK_CODE_CAM_ABORT_APP ((CI_ABORT_ACK_CODE_T)    0x02)/**<   APP abort is ok   */

typedef UINT8 CI_FILE_SEARCH_MODE_T;/**<This structure contains the file name search mode to update data from usb.     */
#define CI_FILE_SEARCH_MODE_INVALID    ((CI_FILE_SEARCH_MODE_T) 0x00)/**<  search mode invalid */
#define CI_FILE_SEARCH_MODE_FULL       ((CI_FILE_SEARCH_MODE_T) 0x01)/**<  search through full file name */
#define CI_FILE_SEARCH_MODE_PREFIX_EXT ((CI_FILE_SEARCH_MODE_T) 0x02)/**<  search through file extention and prefix file name*/
#define CI_FILE_SEARCH_MODE_PREFIX     ((CI_FILE_SEARCH_MODE_T) 0x03)/**<  search through file prefix name */
#define CI_FILE_SEARCH_MODE_EXT        ((CI_FILE_SEARCH_MODE_T) 0x04)/**<  search through file extention name*/

typedef UINT32 CI_SET_TYPE_CI_ON_OFF_T;    /**< indicates ci on/off set type       */
#define CI_SET_TYPE_CI_OFF              ((CI_SET_TYPE_CI_ON_OFF_T)    0)    /**<   turn off CI driver     */
#define CI_SET_TYPE_CI_ON               ((CI_SET_TYPE_CI_ON_OFF_T)    1)    /**<   turn on CI driver    */

typedef UINT32 CI_SET_TYPE_TS_ON_OFF_T;    /**< indicates ts on/off set type       */
#define CI_SET_TYPE_TS_OFF              ((CI_SET_TYPE_TS_ON_OFF_T)    0)  /**<disble TS, do not go through CAM        */
#define CI_SET_TYPE_TS_ON               ((CI_SET_TYPE_TS_ON_OFF_T)    1)  /**< enable TS to go through CAM       */

typedef UINT32 CI_SET_TYPE_INP_SRC_T;    /**< indicates input source set type */
#define CI_SET_TYPE_INVALID             ((CI_SET_TYPE_INP_SRC_T)    0)  /**   initial value   */
#define CI_SET_TYPE_NON_DTV             ((CI_SET_TYPE_INP_SRC_T)    1)  /**   other input source, not DTV    */
#define CI_SET_TYPE_DTV                 ((CI_SET_TYPE_INP_SRC_T)    2)  /**   DTV   */

typedef UINT32 CI_SET_TYPE_FORCE_THROUGH_T;    /**< indicates force TS path to go through CAM or not */
#define CI_SET_TYPE_THROUGH_CAM_OFF     ((CI_SET_TYPE_FORCE_THROUGH_T)    0)  /**   go through CAM   */
#define CI_SET_TYPE_THROUGH_CAM_ON      ((CI_SET_TYPE_FORCE_THROUGH_T)    1)  /**   not go through CAM    */

typedef UINT32 CI_GET_TYPE_T;    /**<  get info type used in  c_ci_get() function     */
typedef UINT32 CI_SET_TYPE_T;    /**<   set info type used in  c_ci_set() function     */
typedef SLIST_T(_CI_MMI_MENU_ITEM_T) CI_MMI_MENU_ITEM_LIST_T;    /**<        */

typedef UINT8 CI_SVC_SEL_STATE_T;      /**< Typedef for CI host shunning mode */
#define CI_SVC_SEL_STATE_UNKNOWN        ((CI_SVC_SEL_STATE_T)0)    /* bypass mode */
#define CI_SVC_SEL_STATE_PMT_NOT_FOUND  ((CI_SVC_SEL_STATE_T)1)    /* bypass mode */
#define CI_SVC_SEL_STATE_TUNE_FAILED    ((CI_SVC_SEL_STATE_T)2)    /* pass-through mode */
#define CI_SVC_SEL_STATE_TUNE_CONNECTED ((CI_SVC_SEL_STATE_T)3)    /* pass-through mode */

typedef UINT8 CI_SIGNAL_STATE_T;      /**< Typedef for signal state */
#define CI_SIGNAL_STATE_UNKNOWN         ((CI_SIGNAL_STATE_T)0)    /* signal loss */
#define CI_SIGNAL_STATE_LOSS            ((CI_SIGNAL_STATE_T)1)    /* signal loss */
#define CI_SIGNAL_STATE_ACQUIRED        ((CI_SIGNAL_STATE_T)2)    /* signal get */


typedef UINT8 CI_SVC_DESCRIPTOR_TYPE_T;      /**< Typedef for Host Control v2 service descriptor type */
#define CI_SVC_DESCRIPTOR_TYPE_EVENT  ((CI_SVC_DESCRIPTOR_TYPE_T)0)    /* event descriptor */
#define CI_SVC_DESCRIPTOR_TYPE_COMP    ((CI_SVC_DESCRIPTOR_TYPE_T)1)    /* component descriptor */
#define CI_SVC_DESCRIPTOR_TYPE_RATING    ((CI_SVC_DESCRIPTOR_TYPE_T)2)    /* rating descriptor */
#define CI_SVC_DESCRIPTOR_TYPE_CONTENT    ((CI_SVC_DESCRIPTOR_TYPE_T)3)    /* content descriptor */
#define CI_SVC_DESCRIPTOR_TYPE_COUNT     ((CI_SVC_DESCRIPTOR_TYPE_T)4)    /* descriptor type number */


typedef UINT8 CI_ASK_RELEASE_REPLY_CODE_T;      /**< Typedef for CI host shunning mode */
#define CI_ASK_RELEASE_REPLY_CODE_OK        ((CI_ASK_RELEASE_REPLY_CODE_T)0)    /* bypass mode */
#define CI_ASK_RELEASE_REPLY_CODE_REFUSE    ((CI_ASK_RELEASE_REPLY_CODE_T)1)    /* pass-through mode */


typedef UINT8 CI_SVC_TYPE_SUPPORT_T; /*structure contains the service type that the system supported.     */
#define CI_SVC_TYPE_MPEG2_SUPPORT            ((CI_SVC_TYPE_SUPPORT_T) 0x01)/**<  MPEG-2 television services supported*/
#define CI_SVC_TYPE_MPEG1_RADIO_SUPPORT      ((CI_SVC_TYPE_SUPPORT_T) 0x02)/**<  MPEG-1, Layer-II radio services supported*/
#define CI_SVC_TYPE_TELETEXT_SUPPORT         ((CI_SVC_TYPE_SUPPORT_T) 0x03)/**<  Teletext services supported*/
#define CI_SVC_TYPE_ADV_RADIO_SUPPORT        ((CI_SVC_TYPE_SUPPORT_T) 0x0a)/**<Advanced codec radio services supported */
#define CI_SVC_TYPE_DATA_SUPPORT             ((CI_SVC_TYPE_SUPPORT_T) 0x0c)/**<  data services identified by the application_capability_byte field (0x0c) supported. */
#define CI_SVC_TYPE_MHP_SUPPORT              ((CI_SVC_TYPE_SUPPORT_T) 0x10)/**<  MHP services supported*/
#define CI_SVC_TYPE_ADV_VID_SD_SUPPORT       ((CI_SVC_TYPE_SUPPORT_T) 0x16)/**<  Advanced codec SD video services supported*/
#define CI_SVC_TYPE_ADV_VID_HD_SUPPORT       ((CI_SVC_TYPE_SUPPORT_T) 0x19)/**<  Advanced codec HD video services supported */

typedef UINT8 CI_DELIVERY_SYS_SUPPORT_T; /*structure contains the delivery system type that the system supported.     */
#define CI_DELIVERY_SYS_DVB_T_SUPPORT       ((CI_DELIVERY_SYS_SUPPORT_T) 0x5a)/**< DVB-T delivery system supported*/
#define CI_DELIVERY_SYS_DVB_EXT_SUPPORT     ((CI_DELIVERY_SYS_SUPPORT_T) 0x7f)/**<  extended descriptor tag*/
#define CI_DELIVERY_SYS_DVB_T2_SUPPORT      ((CI_DELIVERY_SYS_SUPPORT_T) 0x04)/**<  DVB-T2 delivery system supported*/
#define CI_DELIVERY_SYS_DVB_C_SUPPORT       ((CI_DELIVERY_SYS_SUPPORT_T) 0x44)/**<  DVB-C delivery system supported*/
#define CI_DELIVERY_SYS_DVB_S_SUPPORT       ((CI_DELIVERY_SYS_SUPPORT_T) 0x43)/**<  DVB-S delivery system supported. */
#define CI_DELIVERY_SYS_DVB_S2_SUPPORT      ((CI_DELIVERY_SYS_SUPPORT_T) 0x79)/**<  DVB-S2 delivery system supported*/

typedef UINT16 CI_APP_TYPE_SUPPORT_T; /*structure contains the application type that the system supported.     */
#define CI_APP_TYPE_MHP_SUPPORT            ((CI_APP_TYPE_SUPPORT_T) 0xf000)/**<  broadcast MHP application supported*/
#define CI_APP_TYPE_MHEG_5_SUPPORT         ((CI_APP_TYPE_SUPPORT_T) 0x0601)/**<  broadcast MHEG_5 application supported*/
#define CI_APP_TYPE_HBB_TV_SUPPORT         ((CI_APP_TYPE_SUPPORT_T) 0x2301)/**<  broadcast Hbb TV application supported*/
#define CI_APP_TYPE_OPEN_TV_SUPPORT        ((CI_APP_TYPE_SUPPORT_T) 0x0701)/**<  broadcast Open TV application supported */

typedef UINT32 CI_SET_TYPE_REPLY_CODE;    /**< indicates tune reply code to cam */
#define CI_SET_TYPE_TUNE_OK                      ((CI_SET_TYPE_REPLY_CODE)    0)  
#define CI_SET_TYPE_TUNE_UNSUPPORTED_SYSTEM      ((CI_SET_TYPE_REPLY_CODE)    1)  
#define CI_SET_TYPE_TUNE_TUNER_NOT_LOCKING       ((CI_SET_TYPE_REPLY_CODE)    2)  
#define CI_SET_TYPE_TUNER_BUSY                   ((CI_SET_TYPE_REPLY_CODE)    3) 
#define CI_SET_TYPE_TUNE_BAD_PARAMETER           ((CI_SET_TYPE_REPLY_CODE)    4)  
#define CI_SET_TYPE_TUNE_SERVICE_NOT_FOUND       ((CI_SET_TYPE_REPLY_CODE)    5) 
#define CI_SET_TYPE_TUNE_UNDEFINED_ERROR         ((CI_SET_TYPE_REPLY_CODE)    6)  

typedef UINT32 CI_SET_TYPE_PROFILE_SEARCH_STATUS_T;    /**< indicates ts on/off set type       */
#define CI_SET_TYPE_PROFILE_SEARCH_START              ((CI_SET_TYPE_PROFILE_SEARCH_STATUS_T)    0)  /**<disble TS, do not go through CAM        */
#define CI_SET_TYPE_PROFILE_SEARCH_END               ((CI_SET_TYPE_PROFILE_SEARCH_STATUS_T)    1)  /**< enable TS to go through CAM       */


#define CI_GET_TYPE_APP_NAME            ((CI_GET_TYPE_T)    0)    /**<  get Card's name, related arguments in c_ci_get(),

                                                                                                                             pv_get_info:input UINT8 value means slot id,

                                                                                                                             pv_data:input&output UTF_16* value means Card's name,

                                                                                                                             pz_data_len:input&output SIZE_T* value means pv_data's length.
                                                                                                                    */
#define CI_GET_TYPE_SLOT_NUM            ((CI_GET_TYPE_T)    1)    /**<  get slot number, related arguments in c_ci_get(),

                                                                                                                             pv_get_info:input NULL,

                                                                                                                             pv_data:input&output UINT8* value means slot number,

                                                                                                                             pz_data_len:input&output SIZE_T*  value means pv_data's length.
                                                                                                                    */
#define CI_GET_TYPE_SLOT_INFO           ((CI_GET_TYPE_T)    2)    /**< get slot info, related arguments in c_ci_get(),

                                                                                                                             pv_get_info:input UINT8 means slot id,

                                                                                                                             pv_data:input&output CI_SLOT_INFO_T* value,

                                                                                                                             pz_data_len:input&output SIZE_T* value means pv_data's length.
                                                                                                                    */
#define CI_GET_TYPE_PREVIOUS_MMI        ((CI_GET_TYPE_T)    3)    /**<  get MMI menu/list object which is previously notified through x_ci_nfy_fct(),

                                                                                                                             related arguments in c_ci_get(),

                                                                                                                             pv_get_info:input NULL,

                                                                                                                             pv_data:input&output CI_MMI_MENU_T* value,

                                                                                                                             pz_data_len:input&output SIZE_T* value means pv_data's length.
                                                                                                                    */
#define CI_GET_TYPE_PREVIOUS_MMI_ENQ    ((CI_GET_TYPE_T)    4)    /**<  get MMI enq object which is previously notified through x_ci_nfy_fct(),

                                                                                                                             related arguments in c_ci_get(),

                                                                                                                             pv_get_info:input NULL,

                                                                                                                             pv_data:input&output CI_MMI_ENQ_T* value,

                                                                                                                             pz_data_len:input&output SIZE_T* value means pv_data's length.
                                                                                                                    */
#define CI_GET_TYPE_CA_SYSTEM_ID        ((CI_GET_TYPE_T)    5)    /**<  get CA system id array, related arguments in c_ci_get(),

                                                                                                                             pv_get_info:input UINT8 value means slot id,

                                                                                                                             pv_data:input&output UINT16* value means system id array,

                                                                                                                             pz_data_len:input&output  SIZE_T* value means pv_data's length.
                                                                                                                    */
#define CI_GET_TYPE_CA_SYSTEM_ID_NUM    ((CI_GET_TYPE_T)    6)    /**<  get CA system id number, related arguments in c_ci_get(),

                                                                                                                             pv_get_info:input UINT8 means slot id,

                                                                                                                             pv_data:input&output UINT16* value means CA system id number ,

                                                                                                                             pz_data_len:input&output  SIZE_T* value, means pv_data's length.
                                                                                                                    */
#define CI_GET_TYPE_MENU_LIST_ID        ((CI_GET_TYPE_T)    7)    /**<  get id of menu or list related arguments in c_ci_get(),

                                                                                                                             pv_get_info:input UINT8 means slot id,

                                                                                                                             pv_data:input&output UINT16* value means the id ,

                                                                                                                             pz_data_len:input&output  SIZE_T* value, means pv_data's length.
                                                                                                                    */
#define CI_GET_TYPE_ENQ_ID              ((CI_GET_TYPE_T)    8)    /**<  get id of enquiry related arguments in c_ci_get(),

                                                                                                                             pv_get_info:input UINT8 means slot id,

                                                                                                                             pv_data:input&output UINT16* value means the id ,

                                                                                                                             pz_data_len:input&output  SIZE_T* value, means pv_data's length.
                                                                                                                    */
#define CI_GET_TYPE_CIPLUS_CAM_STATUS   ((CI_GET_TYPE_T)    9)    /**<  get CAM type if is CI plus or not related arguments in c_ci_get(),

                                                                                                                             pv_get_info:input UINT8 means slot id,

                                                                                                                             pv_data:input&output BOOL* value means the id ,

                                                                                                                             pz_data_len:input&output  BOOL* value, means pv_data's length.
                                                                                                                    */
#define CI_GET_TYPE_CA_SYSTEM_ID_MATCH   ((CI_GET_TYPE_T)   10)    /**<  get id of enquiry related arguments in c_ci_get(),

                                                                                                                             pv_get_info:input UINT8 means slot id,

                                                                                                                             pv_data:input&output UINT16* value means the id ,

                                                                                                                             pz_data_len:input&output  SIZE_T* value, means pv_data's length.
                                                                                                                    */

#define CI_GET_TYPE_CA_SYSTEM_ID_STATUS   ((CI_GET_TYPE_T)   11)    /**<  get id of enquiry related arguments in c_ci_get(),

                                                                                                                             pv_get_info:input UINT8 means slot id,

                                                                                                                             pv_data:input&output UINT16* value means the id ,

                                                                                                                             pz_data_len:input&output  SIZE_T* value, means pv_data's length.
                                                                                                                    */

#define CI_GET_TYPE_HC_ASK_RELEASE_STATUS   ((CI_GET_TYPE_T)   12)    /**<  get id of enquiry related arguments in c_ci_get(),
    
                                                                                                                                 pv_get_info:input UINT8 means slot id,

                                                                                                                                 pv_data:input&output BOOL* value means tuner release status,
                                                                                                                                 TRUE: means tuner release request is accepted by CAM
                                                                                                                                 FALSE: means tuner release request is denied by CAM,

                                                                                                                                 pz_data_len:input&output BOOL* value, means pv_data's length.
                                                                                                                       */          

#define CI_GET_TYPE_PROFILE_VALID    ((CI_SET_TYPE_T)   13)       /**<  Query profile releated to slot is if is valid, related arguments in c_ci_get(),

                                                                                                                             pv_get_info:input UINT8 means slot id that channel list profile belong to,

                                                                                                                             pv_data:input&output BOOLEAN* value means if profile is valid or not ,

                                                                                                                             pz_data_len:input&output  SIZE_T* value, means pv_data's length.
                                                                                                                    */
#define CI_GET_TYPE_PROFILE_TYPE    ((CI_SET_TYPE_T)   14)       /**<  Query profile type to slot, related arguments in c_ci_get(),
    
                                                                                                                                 pv_get_info:input UINT8 means slot id that channel list profile belong to,
    
                                                                                                                                 pv_data:input&output CI_PROFILE_TYPE_T* value means profile type,
    
                                                                                                                                 pz_data_len:input&output  SIZE_T* value, means pv_data's length.
                                                                                                                        */
    
#define CI_GET_TYPE_PROFILE_NAME            ((CI_GET_TYPE_T)    15)    /**<  get profile's name, related arguments in c_ci_get(),

                                                                                                                             pv_get_info:input UINT8 value means slot id,

                                                                                                                             pv_data:input&output UTF_16* value means profile's name,

                                                                                                                             pz_data_len:input&output SIZE_T* value means pv_data's length.
                                                                                                                    */
#define CI_GET_TYPE_PROFILE_SUPPORT    ((CI_SET_TYPE_T)   16)       /**<  Query profile if match current current tuner mode, related arguments in c_ci_get(),
        
                                                                                                                                     pv_get_info:input CI_PROFILE_SUPPORT_INFO_T* means needed cam profile support infomation,
        
                                                                                                                                     pv_data:input&output BOOLEAN* value means if cam profile match input tuner mode ,
        
                                                                                                                                     pz_data_len:input&output  SIZE_T* value, means pv_data's length.
                                                                                                                            */
#define CI_GET_TYPE_CA_PIN_CAPS   ((CI_GET_TYPE_T)   17)         /**<    get CAM PIN capability related arguments in c_ci_get(),

                                                                                                                                       pv_get_info:NULL,

                                                                                                                                       pv_data:input&output CI_PIN_CAPS_T* value means the capabilities ,

                                                                                                                                       pz_data_len:input&output  SIZE_T* value, means pv_data's length.
                                                                                                                  */
#define CI_GET_TYPE_CA_CAM_ID   ((CI_GET_TYPE_T)   18)         /**<    get CAM ID related arguments in c_ci_get(),
            
                                                                                                                         pv_get_info:NULL,
                                            
                                                                                                                         pv_data:input&output UINT8* value means the cam id array ,
                                            
                                                                                                                         pz_data_len:input&output  SIZE_T* value, means pv_data's length should be CI_PLUS_CAM_ID_LEN*sizeof(UINT8).
                                                                                                                */
#define CI_GET_TYPE_PROFILE_RESOURCE_IS_OPEN   ((CI_GET_TYPE_T)   19) /**<    get CAM ID related arguments in c_ci_get(),
                                                                                                                             */

#define CI_GET_TYPE_PROFILE_ISO_639_LANG_CODE   ((CI_GET_TYPE_T)   20)   /**<    Get the default ISO 639 language code of  unlabelled text fields and elementary stream components. related arguments in c_ci_get(),
            
                                                                                                                         pv_get_info:NULL,
                                            
                                                                                                                         pv_data:input&output UINT8* value means language code array ,
                                            
                                                                                                                         pz_data_len:input&output  SIZE_T* value, means pv_data's length should be CI_PLUS_PROFILE_LANGUAGE_CODE_LEN*sizeof(UINT8).
                                                                                                                */
#define CI_GET_TYPE_RECORD_SUPPORT   ((CI_GET_TYPE_T)   21)         /**<    query whether cam support recording resource, related arguments in c_ci_get(),
                
                                                                                                                             pv_get_info:NULL,,
                                                
                                                                                                                             pv_data:input&output BOOLEAN* value means whether cam support recording resource ,
                                                
                                                                                                                             pz_data_len:input&output  SIZE_T* value, means pv_data's length should be BOOLEAN*sizeof(UINT8).
                                                                                                                    */
#define CI_GET_TYPE_PROFILE_UPDATE_SUPPORT    ((CI_GET_TYPE_T)   22)/**<  user query whether can update cam profile through menu scan page,for example query will return false if cam profile status is waiting schedule update, related arguments in c_ci_get(),
        
                                                                                                                               pv_get_info:input UINT8 value means slot id,
        
                                                                                                                               pv_data:input&output BOOLEAN* value means whether cam support profile update ,
                                                                                                                               
                                                                                                                               pz_data_len:input&output  SIZE_T* value, means pv_data's length should be BOOLEAN*sizeof(UINT8).
                                                                                                                      */ 
#define CI_GET_TYPE_PROFILE_INIT_STATE   ((CI_GET_TYPE_T)   23)   /**<    Get CAM PROFILE initialized status . related arguments in c_ci_get(),
                
                                                                                                                             pv_get_info:input UINT8 means slot id that channel list profile belong to,
                                                
                                                                                                                             pv_data:input&output BOOLEAN* value means language code array ,
                                                
                                                                                                                             pz_data_len:input&output  SIZE_T* value, means pv_data's length .
                                                                                                                    */
                                                                                                                    
#define CI_GET_TYPE_SLOT_STATUS   ((CI_GET_TYPE_T)   24)   /**<    Get CAM slot status . related arguments in c_ci_get(),
                
		                                                                                           pv_get_info:input UINT8 means slot id,

		                                                                                           pv_data:input&output CI_STATUS_T* value,

		                                                                                           pz_data_len:input&output SIZE_T* value means pv_data's length.
                                                                                                       */

                                                                                                                    
           
#define CI_GET_TYPE_COPY_CONTROL_FLAG   ((CI_GET_TYPE_T)   25)   /**<    Get CAM EMI value in URI information . related arguments in c_ci_get(),
                
                                                                                                                             pv_get_info:NULL,,,
                                                
                                                                                                                             pv_data:input&output EMI_TYPE_T* value means EMI value ,
                                                
                                                                                                                             pz_data_len:input&output  SIZE_T* value, means pv_data's length .
                                                                                                                    */
#define CI_GET_CAM_SER_NO               ((CI_GET_TYPE_T)    26)    /*Get cam serial no for hisense & smit private protocolrelated arguments in c_ci_get(),

                                                                                                                    pv_get_info:input UINT8 means slot id,
                                                                                                                    
                                                                                                                    pv_data:
                                                                                                                    
                                                                                                                    pz_data_len:.

                                                                                                                    */
    
#define CI_GET_CAM_USER_ATTRIBUTE               ((CI_GET_TYPE_T)    27)    /*Get cam serial no for hisense & smit private protocol
                                                                                                                                    pv_get_info:input UINT8 means slot id,

                                                                                                                                    pv_data:,

                                                                                                                                    pz_data_len:.

                                                                                                                                    */

#define CI_GET_TYPE_SAS_CONNECT_STATE   ((CI_GET_TYPE_T)   28)   /**<    Get SAS is connected . related arguments in c_ci_get(),
                
                                                                                                                             pv_get_info:input UINT32 value means slot number,
                                                
                                                                                                                             pv_data:input&output BOOLEAN* value,true means SAS session is connected. ,
                                                
                                                                                                                             pz_data_len:input&output  SIZE_T* value, means pv_data's length .
                                                                                                                    */
           
#define CI_GET_TYPE_SAS_OPEN_STATE   ((CI_GET_TYPE_T)   29)   /**<    Get SAS session is opened  . related arguments in c_ci_get(),
                
                                                                                                                             pv_get_info:input UINT32 value means slot number,
                                                
                                                                                                                             pv_data:input&output BOOLEAN* value,true means SAS session is opened. ,
                                                
                                                                                                                             pz_data_len:input&output  SIZE_T* value, means pv_data's length .
                                                                                                                    */
#define CI_GET_TYPE_PROFILE_INFO   ((CI_GET_TYPE_T)   30)   /**<    Get operator profile information . related arguments in c_ci_get(),
                    
                                                                                                                                 pv_get_info:input UINT8 means slot id that channel list profile belong to,
                                                    
                                                                                                                                 pv_data:input&output CI_PROFILE_INFO_T* value ,
                                                    
                                                                                                                                 pz_data_len:input&output  SIZE_T* value, means pv_data's length .
                                                                                                                        */
    
#define CI_GET_TYPE_LAST_ENTRY          ((CI_GET_TYPE_T)    32)    /**<        */

#define CI_SET_TYPE_CLOSE               ((CI_SET_TYPE_T)    0)    /**<  close MMI, related arguments in c_ci_set(),

                                                                                                                             pv_set_info:input UINT32 value means slot number

                                                                                                                             pv_data:NULL.
                                                                                                                    */
#define CI_SET_TYPE_ENTER_MENU          ((CI_SET_TYPE_T)    1)     /**<  enter MMI menu, related arguments in c_ci_set(),

                                                                                                                             pv_set_info:input UINT32 value means slot number

                                                                                                                             pv_data:NULL.
                                                                                                                    */
#define CI_SET_TYPE_MMI_ENQ_ANS         ((CI_SET_TYPE_T)    2)     /**<  answer ENQ object, related arguments in c_ci_set(),

                                                                                                                             pv_set_info:NULL,

                                                                                                                             pv_data:input CI_MMI_ENQ_ANS_T * value.
                                                                                                                    */
#define CI_SET_TYPE_MMI_MENU_ANS        ((CI_SET_TYPE_T)    3)    /**<  answer Menu object, related arguments in c_ci_set(),

                                                                                                                             pv_set_info:NULL,

                                                                                                                             pv_data:input CI_MMI_MENU_ANS_T * value.
                                                                                                                    */
#define CI_SET_TYPE_HC_ASK_RELEASE      ((CI_SET_TYPE_T)    4)     /**<  ask CAM to release control, related arguments in c_ci_set(),

                                                                                                                             pv_set_info:input UINT32 value means slot number

                                                                                                                             pv_data:NULL.
                                                                                                                    */
#define CI_SET_TYPE_CI_ON_OFF           ((CI_SET_TYPE_T)    5)     /**<  control CI driver, related arguments in c_ci_set(),

                                                                                                                             pv_set_info:input CI_SET_TYPE_CI_ON_OFF_T value,

                                                                                                                             pv_data:NULL.
                                                                                                                    */
#define CI_SET_TYPE_NFY_COLSE_DONE      ((CI_SET_TYPE_T)    6)     /**<  notify MMI close done, related arguments in c_ci_set(),

                                                                                                                             pv_set_info:input UINT32 value means slot number,

                                                                                                                             pv_data:NULL.
                                                                                                                    */
#define CI_SET_TYPE_TS_ON_OFF           ((CI_SET_TYPE_T)    7)     /**<  control TS path, related arguments in c_ci_set(),

                                                                                                                             pv_set_info:input CI_SET_TYPE_TS_ON_OFF_T value,

                                                                                                                             pv_data:NULL.
                                                                                                                    */
#define CI_SET_TYPE_CAM_UPGRADE_REPLY   ((CI_SET_TYPE_T)    8)/**<  notify CAM upgrade reply, related arguments in c_ci_set(),

                                                                                                                       pv_set_info:NULL,

                                                                                                                       pv_data:input CI_CAM_UPGRADE_PROGRESS_REPLY_T* data contains reply data.
                                                                                                                */
#define CI_SET_TYPE_REQUEST_START_ACK   ((CI_SET_TYPE_T)    9)/**<  notify APP about start request reply, related arguments in c_ci_set(),

                                                                                                                       pv_set_info:NULL,

                                                                                                                       pv_data:input CI_REQUEST_START_ACK_T* data contains reply data.
                                                                                                                */
#define CI_SET_TYPE_APP_ABORT_REQUEST   ((CI_SET_TYPE_T)    10)/**<  request CAM to abort APP, related arguments in c_ci_set(),

                                                                                                                       pv_set_info:NULL,

                                                                                                                       pv_data:input CI_ABORT_REQUEST_T* data means abort data.
                                                                                                                */
#define CI_SET_TYPE_APP_ABORT_ACK       ((CI_SET_TYPE_T)    11)/**<  notify CAM that APP abort finished, related arguments in c_ci_set(),

                                                                                                                       pv_set_info:NULL,

                                                                                                                       pv_data:NULL
                                                                                                                */
#define CI_SET_TYPE_HOST_LANGUAGE       ((CI_SET_TYPE_T)    12)/**<  notify CAM host language code, related arguments in c_ci_set(),
        
                                                                                                                               pv_set_info:NULL,
        
                                                                                                                               pv_data:input ISO_639_LANG_T* data contains host language code.
                                                                                                                        */
#define CI_SET_TYPE_SCAN_END_AND_DT_SYNC       ((CI_SET_TYPE_T)    13)/**<  notify that scan is complete and dt is sync status,
    
                                                                                                                           pv_set_info:NULL,
    
                                                                                                                           pv_data:NULL.
                                                                                                                    */
#define CI_SET_TYPE_NEED_RESET_CAM            ((CI_SET_TYPE_T)    14)/**<  notify CI driver to reset CI plus CAM to do authentication again ,
        
                                                                                                                               pv_set_info:NULL,
        
                                                                                                                               pv_data:NULL.
                                                                                                                        */
#define CI_SET_TYPE_HOST_COUNTRY        ((CI_SET_TYPE_T)    15)/**<  notify CAM host country code, related arguments in c_ci_set(),

                                                                                                                       pv_set_info:NULL,

                                                                                                                       pv_data:input ISO_3166_COUNT_T* data contains host country code.
                                                                                                                */
#define CI_SET_TYPE_INP_SRC              ((CI_SET_TYPE_T)   16)/**<  notify CAM about current source setting, DTV or not, related arguments in c_ci_set(),

                                                                                                                       pv_set_info: CI_SET_TYPE_INP_SRC_T

                                                                                                                       pv_data:. NULL
                                                                                                                */

#define CI_SET_TYPE_FORCE_THROUGH_CAM    ((CI_SET_TYPE_T)   17)/**<  notify CI driver to rough TS path no matter CAM insert or not. , related arguments in c_ci_set(),

                                                                                                                       pv_set_info: CI_SET_TYPE_FORCE_THROUGH_T

                                                                                                                       pv_data:. NULL
                                                                                                                */
#define CI_SET_TYPE_HOST_REGION_ID        ((CI_SET_TYPE_T)    18)/**<  notify CI MW host region id, related arguments in c_ci_set(),
    
                                                                                                                           pv_set_info:NULL,
    
                                                                                                                           pv_data:input data contains host region id.
                                                                                                                    */
#define CI_SET_TYPE_HOST_CABLE_BRDCSTER   ((CI_SET_TYPE_T)    19)/**<  notify  CI MW host cable profile settings, related arguments in c_ci_set(),
        
                                                                                                                               pv_set_info:NULL,
        
                                                                                                                               pv_data:input data contains host cable profile settings.
                                                                                                                        */
#define CI_SET_TYPE_HOST_BRDCST_MEDIUM    ((CI_SET_TYPE_T)    20)/**<  notify  CI MW host brdcst settings, related arguments in c_ci_set(),
            
                                                                                                                                   pv_set_info:NULL,
            
                                                                                                                                   pv_data:input data contains host brdcst settings.
                                                                                                                            */
                                                                                                                

#define CI_SET_TYPE_STOP_TUNE_BRDCST_SVC      ((CI_SET_TYPE_T)    21)/**<  host control broadcast tune serivce is failed to started  ,
                
                                                                                                                                       pv_set_info:NULL,
                
                                                                                                                                       pv_data:NULL.
                                                                                                                                */
#define CI_SET_TYPE_PROFILE_SEARCH_REQ_START    ((CI_SET_TYPE_T)   22)/**<  request cam profile search to started , related arguments in c_ci_set(),

                                                                                                                       pv_set_info: UINT32 value indicates slot id

                                                                                                                       pv_data: BOOLEAN value indicates whether current tv mode is in BGM mode
                                                                                                                */            
#define CI_SET_TYPE_PROFILE_SEARCH_STARTED    ((CI_SET_TYPE_T)   23) /**<  notify cam profile search is started , related arguments in c_ci_set(),

                                                                                                                       pv_set_info: UINT32 value indicates slot id

                                                                                                                       pv_data:. NULL
                                                                                                                */           
#define CI_SET_TYPE_PROFILE_SEARCH_CANCELED   ((CI_SET_TYPE_T)   24) /**<  notify cam profile search is canceled by user , related arguments in c_ci_set(),
    
                                                                                                                           pv_set_info: UINT32 value indicates slot id
    
                                                                                                                           pv_data:. NULL
                                                                                                                    */           
#define CI_SET_TYPE_PROFILE_START   ((CI_SET_TYPE_T)   25)/**<  request to enter ci plus channel list profile , related arguments in c_ci_set(),

                                                                                                                       pv_set_info: UINT32 value indicates slot id

                                                                                                                       pv_data:NULL
                                                                                                                */
#define CI_SET_TYPE_PROFILE_END   ((CI_SET_TYPE_T)   26)/**<  request to exit ci plus channel list profile , related arguments in c_ci_set(),
    
                                                                                                                           pv_set_info: UINT32 value indicates slot id
    
                                                                                                                           pv_data:NULL
                                                                                                                    */
#define CI_SET_TYPE_RECORD_START    ((CI_SET_TYPE_T)   27)/**<  notify cam to start record , related arguments in c_ci_set(),

                                                                                                                   pv_set_info: NULL

                                                                                                                    pv_data:input CI_RECORD_START_T* data 
                                                                                                                */
                                                                                                                  
#define CI_SET_TYPE_RECORD_STOP    ((CI_SET_TYPE_T)   28)/**<  notify cam to stop record , related arguments in c_ci_set(),

                                                                                                                   pv_set_info: NULL

                                                                                                                    pv_data:input UINT16 data indicates program number 
                                                                                                                  */
#define CI_SET_TYPE_PIN_PLAYBACK    ((CI_SET_TYPE_T)   29)/**<  send pin event to cam when playback which is received when recording , related arguments in c_ci_set(),

                                                                                                                    pv_set_info: NULL

                                                                                                                     pv_data:input CI_PIN_PLAYBACK_T* data indicates playback data 
                                                                                                                  */
#define CI_SET_TYPE_LICENSE_PLAYBACK    ((CI_SET_TYPE_T)   30)/**<  send license to cam when playback which is received when recording , related arguments in c_ci_set(),
            
                                                                                                                      pv_set_info: NULL
        
                                                                                                                       pv_data:input CI_LICENSE_Playback_T* data indicates playback data 
                                                                                                              */
#define CI_SET_TYPE_TUNE_REPLY  ((CI_SET_TYPE_T)   31)/**<  request to change cam profile , related arguments in c_ci_set(),

                                                                                                    pv_set_info: UINT32 value indicates slot id

                                                                                                   pv_data:NULL 
                                                                                                   */
#define CI_SET_TYPE_PROFILE_NIT_UPDATE_START    ((CI_SET_TYPE_T)   32)/**<  request to start cam update nit table , related arguments in c_ci_set(),          
                                                                                                                                    pv_set_info: UINT32 value indicates slot id                                                   
                                                                                                                                    pv_data: private data from  CI_NFY_COND_PROFILE_NIT_UPDATE_REQ          
                                                                                                                                    */    
#define CI_SET_TYPE_PROFILE_SEARCH_STATUS    ((CI_SET_TYPE_T)   33)/**<  update profile search status , related arguments in c_ci_set(),          
                                                                                                                                        pv_set_info: CI_SET_TYPE_PROFILE_SEARCH_STATUS_T value indicates search status                                                   
                                                                                                                                        pv_data: null         
                                                                                                                                        */    
#define CI_SET_TYPE_PIN_CMD                      ((CI_SET_TYPE_T)   34)/**<  send pin code to cam to check if pin code is correct, The CAM should respond to this call by notify with code:CI_NFY_COND_PIN_REPLY,related arguments in c_ci_set(),
        
                                                                                                                            pv_set_info: NULL
        
                                                                                                                             pv_data:input UINT8* data indicates PIN data, need add '/0' to end of pin code 
                                                                                                                      */
#define CI_SET_TYPE_RECORD_CHG_MODE    ((CI_SET_TYPE_T)   35)/**<  notify cam to change record mode , related arguments in c_ci_set(),
    
                                                                                                                       pv_set_info: NULL
    
                                                                                                                        pv_data:input CI_RECORD_START_T* data ,pin_data in CI_RECORD_START_T not used
                                                                                                                    */                                                                                                  
#define CI_SET_TYPE_RESET_AUTH_KEY    ((CI_SET_TYPE_T)   36)                                                                                            
#define CI_SET_TYPE_BLOCK_SERVICE    ((CI_SET_TYPE_T)   37)
#define CI_SET_TYPE_PIN_CODE_VERIFY    ((CI_SET_TYPE_T) 38)
#define CI_SET_TYPE_NETWORK_SUPPORT    ((CI_SET_TYPE_T) 39)/**<  notify cam whether support network  , related arguments in c_ci_set(),*/                                                                                                       
#define CI_SET_TYPE_EMPTY_PMT    ((CI_SET_TYPE_T)   40)                                                                                            

#define CI_SET_TYPE_NETCARD_PLUG          ((CI_SET_TYPE_T)   41)

#define CI_SET_TYPE_SAS_CONNECT    ((CI_SET_TYPE_T)   42)/**<  send SAS session connect request to cam , related arguments in c_ci_set(),
                                                                                                                pv_set_info: NULL
                                                                                                                pv_data:input UINT8* data ,indicates applicaion id and length is 8 byte,need add '/0' to end,for example: The value of ziggo_APPLICATION_ID is 0x5A6967676F695456
                                                                                                                */
#define CI_SET_TYPE_SAS_CLOSE    ((CI_SET_TYPE_T)   43)/**<  send SAS session close request to cam , related arguments in c_ci_set(), 
                                                                                                            pv_set_info: NULL
                                                                                                            pv_data: NULL
                                                                                                            */

#define CI_SET_TYPE_SAS_DTID_EVENT    ((CI_SET_TYPE_T)   44)/**<  send SAS message to cam , related arguments in c_ci_set(),
                                                                                                                pv_set_info: NULL
                                                                                                                pv_data: CI_PLUS_SAS_EVENT_T* data,indicates sas dtid error event.
                                                                                                                */
#define CI_SET_TYPE_SELECT_TUNER_FOR_CAM    ((CI_SET_TYPE_T)   45)  /*select which tuner pass though the CAM*/ 
     
#define CI_SET_TYPE_UPDATE_PROFILE_INFO   ((CI_GET_TYPE_T)   46)   /**<    Update operator profile information . related arguments in c_ci_get(),
                
                                                                                                                             pv_set_info:UINT32 value indicates slot id,
                                                
                                                                                                                             pv_data:input CI_PROFILE_INFO_T* value ,
                                                
                                                                                                                    */

#define CI_SET_TYPE_LAST_ENTRY          ((CI_SET_TYPE_T)    64)    /**<        */

#define CIR_OK                      ((INT32)    0)    /**< The routine is successful     */
#define CIR_FAILED                  ((INT32)    -1)    /**<The routine is failed         */
#define CIR_ALREADY_INIT            ((INT32)    -2)    /**<  CI engine has been initialized       */
#define CIR_NOT_INIT                ((INT32)    -3)    /**< CI engine has not initialized       */
#define CIR_INV_ARG                 ((INT32)    -4)    /**< Indicates at least one argument is invalid. */
#define CIR_OUT_OF_HANDLES          ((INT32)    -5)    /**<There isn't enough handles or the caller's  handle resource is exhausted.        */
#define CIR_INV_GET_TYPE            ((INT32)    -6)    /**< e_get_type value is invalid in c_ci_get()       */
#define CIR_INV_GET_INFO            ((INT32)    -7)    /**< pv_get_info value is invalid in c_ci_get()      */
#define CIR_INV_GET_DATA            ((INT32)    -8)    /**<  pv_data value is invalid in c_ci_get()      */
#define CIR_INV_SET_TYPE            ((INT32)    -9)    /**< e_set_type value is invalid in c_ci_set()       */
#define CIR_INV_SET_INFO            ((INT32)    -10)    /**<  pv_set_info value is invalid in c_ci_set()      */
#define CIR_INV_SET_DATA            ((INT32)    -11)    /**<   pv_data value is invalid in c_ci_set()          */
#define CIR_NOT_ENOUGH_SPACE        ((INT32)    -12)    /**<There is not enough memory to complete the operation.        */
#define CIR_CLI_ERROR               ((INT32)    -13)    /**< cli command occur error       */
#define CIR_INIT_FAILED             ((INT32)    -14)    /**<CI engine initialized failed     */
#define CIR_NOT_REGISTERED          ((INT32)    -15)    /**<No related callback function registered        */
#define CIR_ALREADY_REGISTERED      ((INT32)    -16)    /**<Related callback function already registered        */
#define CIR_INV_NAME                ((INT32)    -17)    /**< CI engine's name is invalid       */
#define CIR_ALREADY_SET_ENTER_MENU  ((INT32)    -18)    /**< MMI Menu has been already entered       */
#define CIR_ENC_DEC_FAILED          ((INT32)    -19)    /**< Encryption or decryption fail     */
#define CIR_SESSION_NOT_ESTABLISHED ((INT32)    -20)    /**< Session not open     */
#define CIR_CHECKSUM_ERROR          ((INT32)    -21)    /**< checksum error     */
#define CIR_NO_TUNE_BRDCST_DATA     ((INT32)    -22)    /**< checksum error     */
#define CIR_INV_HANDLE                ((INT32)    -23)    /**< Invalid handle error     */
#define CIR_NOT_SUPPORT                ((INT32)    -24)    /**< Invalid handle error     */



typedef UINT32 CI_APPEAR_COND_T;
#define CI_APPEAR_COND_INSERT     ((CI_APPEAR_COND_T) 0)  /**< Card is insterted     */
#define CI_APPEAR_COND_CARD_NAME  ((CI_APPEAR_COND_T) 1)  /**< Card's name is ready     */
#define CI_APPEAR_COND_REMOVE     ((CI_APPEAR_COND_T) 2)  /**< Card is removed    */
#define CI_APPEAR_COND_MAX        ((CI_APPEAR_COND_T) 32) /**< Max condition, debug usage, not valid status   */

#define MAX_MENU_ITEMS 64
#define MAX_STRING 256
/*Command Category Prefix*/
#define CI_CMD_NOTIFY            ((UINT32)0x10000000)    /**< NOTIFY Command Category Prefix       */
#define CI_CMD_QUERY             ((UINT32)0x20000000)    /**< QUERY Command Category Prefix      */

/* Notify functions */
#define CI_REG_EVENT_NFY_MENU          ((UINT32)(CI_CMD_NOTIFY | MAKE_BIT_MASK_32(0)))  /**< notify tag from menu application */
#define CI_REG_EVENT_NFY_NAV_CORE      ((UINT32)(CI_CMD_NOTIFY | MAKE_BIT_MASK_32(1)))  /**< notify tag from nav core application */
#define CI_REG_EVENT_NFY_NAV_CI        ((UINT32)(CI_CMD_NOTIFY | MAKE_BIT_MASK_32(2)))  /**< notify tag from nav ci application */
#define CI_REG_EVENT_NFY_EPG           ((UINT32)(CI_CMD_NOTIFY | MAKE_BIT_MASK_32(3)))  /**< notify tag from epg application */
#define CI_REG_EVENT_NFY_NAV_MHEG      ((UINT32)(CI_CMD_NOTIFY | MAKE_BIT_MASK_32(4)))  /**< notify tag from mheg application */
#define CI_REG_EVENT_NFY_ANDROID       ((UINT32)(CI_CMD_NOTIFY | MAKE_BIT_MASK_32(5)))  /**< notify tag from android application */
#define CI_REG_EVENT_NFY_MM       ((UINT32)(CI_CMD_NOTIFY | MAKE_BIT_MASK_32(6)))  /**< notify tag from MM application */
#define CI_REG_EVENT_NFY_REC       ((UINT32)(CI_CMD_NOTIFY | MAKE_BIT_MASK_32(7)))  /**< notify tag from REC application */
#define CI_REG_EVENT_NFY_BGM_CAM_SCAN      ((UINT32)(CI_CMD_NOTIFY | MAKE_BIT_MASK_32(8)))  /**< notify tag from REC application */
#define CI_REG_EVENT_NFY_SUBTITLE      ((UINT32)(CI_CMD_NOTIFY | MAKE_BIT_MASK_32(9)))  /**< notify tag from SUBTITLE application */
#define CI_REG_EVENT_NFY_PWD            ((UINT32)(CI_CMD_NOTIFY | MAKE_BIT_MASK_32(10))) /**< notify tag from PWD application */
#define CI_REG_EVENT_NFY_MENU_CA_PIN ((UINT32)(CI_CMD_NOTIFY | MAKE_BIT_MASK_32(11)))  /**< notify tag from menu ca pin */

#define CI_REG_EVENT_NFY_LAST          ((UINT32)(CI_CMD_NOTIFY | MAKE_BIT_MASK_32(23)))  /**< last index, not used  */

/* Query functions */
#define CI_REG_EVENT_QUERY_NAV_CI      ((UINT32)(CI_CMD_QUERY | MAKE_BIT_MASK_32(0)))  /**< query tag from nav ci application  */
#define CI_REG_EVENT_QUERY_LAST        ((UINT32)(CI_CMD_NOTIFY | MAKE_BIT_MASK_32(23)))  /**< last index, not used  */

/*------------------------------------------------------------------*/
/*! @struct CI_MMI_ENQ_T
 *  @brief This structure contains the MMI enquiry object information. 
 *  @code
 *  typedef struct _CI_MMI_ENQ_T
 *  {
 *      UINT32      ui4_id;
 *      UINT8       ui1_ans_txt_len;
 *      BOOL        b_blind_ans;
 *      UTF16_T*    w2s_text;
 *  } CI_MMI_ENQ_T;
 *  @endcode
 *  @li@c  ui4_id                                            -enqery object  id that will be used in CI_MMI_ENQ_ANS_T
 *  @li@c  ui1_ans_txt_len                                   -expected length of the answer. Set to hex 'FF' if unknown by the Application.
 *  @li@c  b_blind_ans                                       -set to 1 means that the user input has not to be displayed when entered. The host has the
*                                                                       choice of the replacement character used (star, ...).
 *  @li@c  w2s_text                                          -Text information is coded using the character sets and methods described in [4].
 */
/*------------------------------------------------------------------*/
typedef struct _CI_MMI_ENQ_T
{
    UINT32      ui4_id;
    UINT8       ui1_ans_txt_len;
    BOOL        b_blind_ans;
    UTF16_T*    w2s_text;
} CI_MMI_ENQ_T;

/*------------------------------------------------------------------*/
/*! @struct CI_MMI_ENQ_ANS_T
 *  @brief This structure contains the answer information of MMI enquiry object. 
 *  @code
 *  typedef struct _CI_MMI_ENQ_ANS_T
 *  {
 *      UINT32      ui4_id;
 *      BOOL        b_answer;
 *      UTF16_T*    w2s_text;
 *  } CI_MMI_ENQ_ANS_T;
 *  @endcode
 *  @li@c  ui4_id                                            -indicate answer which Enq object got before
 *  @li@c  b_answer                                          -true means that the object contains the user input (which may be of zero length).
 *                                                                      false means that the user wishes to abort the dialogue.
 *  @li@c  w2s_text                                          -answer content which shall be coded using the same character coding scheme as that used its associated Enq object.
 */
/*------------------------------------------------------------------*/
typedef struct _CI_MMI_ENQ_ANS_T
{
    UINT32      ui4_id;
    BOOL        b_answer;
    UTF16_T*    w2s_text;
} CI_MMI_ENQ_ANS_T;

/*------------------------------------------------------------------*/
/*! @struct CI_MMI_ENQ_CLOSE_T
 *  @brief This structure contains the answer information of MMI enquiry close object. 
 *  @code
 *  typedef struct _CI_MMI_ENQ_CLOSE_T
 *  {
 *      UINT8       ui1_ci_slot;
 *      UINT8       ui1_mmi_close_delay;
 *  } CI_MMI_ENQ_CLOSE_T;
 *  @endcode
 *  @li@c  ui1_ci_slot                                       -slot id, value range[0,slot num-1]
 *  @li@c  ui1_mmi_close_delay                               -MMI close delay time,0 means close immediately
 */
/*------------------------------------------------------------------*/
typedef struct _CI_MMI_ENQ_CLOSE_T
{
    UINT8       ui1_ci_slot;
    UINT8       ui1_mmi_close_delay;
} CI_MMI_ENQ_CLOSE_T;

/*------------------------------------------------------------------*/
/*! @struct  CI_MMI_MENU_ITEM_T
 *  @brief his structure contains the MMI menu item information. 
 *  @code
 *  typedef struct _CI_MMI_MENU_ITEM_T
 *  {
 *      SLIST_ENTRY_T(_CI_MMI_MENU_ITEM_T)  t_link;
 *      UTF16_T*                            w2s_item;
 *  } CI_MMI_MENU_ITEM_T;
 *  @endcode
 *  @li@c  t_link                                            -next menu item pointer
 *  @li@c  w2s_item                                          -menu item content, max length is 256
 */
/*------------------------------------------------------------------*/
typedef struct _CI_MMI_MENU_ITEM_T
{
    SLIST_ENTRY_T(_CI_MMI_MENU_ITEM_T)  t_link; 
    UTF16_T*                            w2s_item;
} CI_MMI_MENU_ITEM_T;

/* This structure contains the MMI menu or list object information. */
/*------------------------------------------------------------------*/
/*! @struct CI_MMI_MENU_T
 *  @brief This structure contains the MMI menu or list object information.
 *  @code
 *  typedef struct _CI_MMI_MENU_T
 *  {
 *      UINT32              ui4_id;
 *      UTF16_T*            w2s_title;
 *      UTF16_T*            w2s_subtitle;
 *      UTF16_T*            w2s_bottom;
 *      UINT8               ui1_choice_nb;
 *      CI_MMI_MENU_ITEM_LIST_T  t_list;
 *  } CI_MMI_MENU_T;
 *  @endcode
 *  @li@c  ui4_id                                            -menu/list id
 *  @li@c  w2s_title                                         -title content,max length is 256
 *  @li@c  w2s_subtitle                                      -subtitle content,max length is 256
 *  @li@c  w2s_bottom                                        -bottom content,max length is 256
 *  @li@c  ui1_choice_nb                                     -choice number,'FF' means that this field does not carry the number of choices information.
 *  @li@c  t_list                                            -menu item list 
 */
/*------------------------------------------------------------------*/
typedef struct _CI_MMI_MENU_T
{
    UINT32              ui4_id;
    UTF16_T*            w2s_title;
    UTF16_T*            w2s_subtitle;
    UTF16_T*            w2s_bottom;
    UINT8               ui1_choice_nb;
    UTF16_T*            t_list_android[MAX_MENU_ITEMS];
    CI_MMI_MENU_ITEM_LIST_T  t_list;
} CI_MMI_MENU_T;

/* This structure contains the answer information of MMI menu object. */
/*------------------------------------------------------------------*/
/*! @struct CI_MMI_MENU_ANS_T
 *  @brief This structure contains the answer information of MMI menu object.
 *  @code
 *  typedef struct _CI_MMI_MENU_ANS_T
 *  {
 *      UINT32              ui4_id;
 *      UINT8               ui1_answer;
 *  } CI_MMI_MENU_ANS_T;
 *  @endcode
 *  @li@c  ui4_id                                            -menu id used to indicate which menu object to answer.
 *  @li@c  ui1_answer                                    -the number of the choice selected by the user. 
 *                                                                  01 corresponds to the first choice that had been presented by the application in that object 
 *                                                                  02 corresponds to the second choice text presented by the application.
 *                                                                  00 indicates that the user has cancelled the preceding menu or list object without making a choice.
 */
/*------------------------------------------------------------------*/
typedef struct _CI_MMI_MENU_ANS_T
{
    UINT32              ui4_id;
    UINT8               ui1_answer;
} CI_MMI_MENU_ANS_T;

/*------------------------------------------------------------------*/
/*! @struct CI_SLOT_INFO_T
 *  @brief This structure contains the slot information. 
 *  @code
 *  typedef struct _CI_SLOT_INFO_T
 *  {
 *      BOOL                b_active;
 *      UINT32              ui4_app_id;
 *  } CI_SLOT_INFO_T;
 *  @endcode
 *  @li@c  b_active                                          -true means CAM is plugged in the slot and CAM's name is ready,otherwise false
 *  @li@c  ui4_app_id                                        -slot id, value range[0,slot num-1].
 */
/*------------------------------------------------------------------*/
typedef struct _CI_SLOT_INFO_T
{
    BOOL                b_active;
    UINT32              ui4_app_id;
} CI_SLOT_INFO_T;

/*------------------------------------------------------------------*/
/*! @struct CI_HC_TUNE_T
 *  @brief This structure contains the Tune object information.
 *  @code
 *  typedef struct _CI_HC_TUNE_T
 *  {
 *      UINT32              ui4_id;
 *      UINT16              ui2_network_id;
 *      UINT16              ui2_orig_network_id;
 *      UINT16              ui2_ts_id;
 *      UINT16              ui2_service_id;
 *  } CI_HC_TUNE_T;
 *  @endcode
 *  @li@c  ui4_id                                            -slot id, value range[0,slot num-1].
 *  @li@c  ui2_network_id                               -Network ID
 *  @li@c  ui2_orig_network_id                        -Original Network ID
 *  @li@c  ui2_ts_id                                         -Transport Stream ID
 *  @li@c  ui2_service_id                                 -Service ID
 */
/*------------------------------------------------------------------*/
typedef struct _CI_HC_TUNE_T
{
    UINT32              ui4_id;
    UINT16              ui2_network_id;
    UINT16              ui2_orig_network_id;
    UINT16              ui2_ts_id;
    UINT16              ui2_service_id;
} CI_HC_TUNE_T;

/*------------------------------------------------------------------*/
/*! @struct CI_HC_REPLACE_T
 *  @brief This structure contains the replace object information
 *  @code
 *  typedef struct _CI_HC_REPLACE_T
 *  {
 *      UINT32              ui4_id;
 *      UINT8               ui1_ref;
 *      UINT16              ui2_replaced_pid;
 *      UINT16              ui2_replacement_pid;
 *  } CI_HC_REPLACE_T;
 *  @endcode
 *  @li@c  ui4_id                                            -slot id, value range[0,slot num-1].
 *  @li@c  ui1_ref                                           -replace object id,which is used to match a Clear Replace object with one or more previous Replace objects.
 *                                                                   Several Replace objects can use the same value of ui1_ref.
 *  @li@c  ui2_replaced_pid                                  -Stream PID: video, audio, teletext or subtitles,
                                                                           replaced by the component being transmitted in the ui2_replacement_pid
 *  @li@c  ui2_replacement_pid                               -replacement stream PID.
 */
/*------------------------------------------------------------------*/
typedef struct _CI_HC_REPLACE_T
{
    UINT32              ui4_id;
    UINT8               ui1_ref;
    UINT16              ui2_replaced_pid;
    UINT16              ui2_replacement_pid;
} CI_HC_REPLACE_T;

/*------------------------------------------------------------------*/
/*! @struct CI_HC_CLR_REPLACE_T
 *  @brief This structure contains the clear replace object information 
 *  @code
 *  typedef struct _CI_HC_CLR_REPLACE_T
 *  {
 *      UINT32              ui4_id;
 *      UINT8               ui1_ref;
 *  } CI_HC_CLR_REPLACE_T;
 *  @endcode
 *  @li@c  ui4_id                                            -slot id, value range[0,slot num-1].
 *  @li@c  ui1_ref                                           -replace object id which will be cleared
 */
/*------------------------------------------------------------------*/
typedef struct _CI_HC_CLR_REPLACE_T
{
    UINT32              ui4_id;
    UINT8               ui1_ref;
} CI_HC_CLR_REPLACE_T;

/*------------------------------------------------------------------*/
/*! @struct CI_CA_SYSTEM_ID_MATCH_T
 *  @brief This structure contains the ca system id match information. 
 *  @code
 *  typedef struct _CI_CA_SYSTEM_ID_MATCH_T
 *  {
 *      UINT8 ui1_slot_id;
 *      BOOL b_match;
 *  } CI_CA_SYSTEM_ID_MATCH_T;
 *  @endcode
 *  @li@c  ui1_slot_id                                       -slot id, value range[0,slot num-1].
 *  @li@c  b_match                                           -ture if ca system id in CAM match pmt's
 */
/*------------------------------------------------------------------*/
typedef struct _CI_CA_SYSTEM_ID_MATCH_T
{
    UINT8    ui1_slot_id;
    BOOL     b_match;
} CI_CA_SYSTEM_ID_MATCH_T;
/*! @struct CI_CA_SYSTEM_ID_MATCH_T
 *  @brief This structure contains the ca system id match information. 
 *  @code
 typedef struct _CI_CA_SYSTEM_ID_PMT_LIST_T
 {
	 UINT8	  ui1_slot_id;
	 BOOL	  b_match;
 } CI_CA_SYSTEM_ID_PMT_LIST_T;
 *  @endcode
 *  @li@c  ui1_slot_id                                       -slot id, value range[0,slot num-1].
 *  @li@c  b_match                                           -ture if ca system id in CAM match pmt's
 */
/*------------------------------------------------------------------*/
typedef struct _CI_CA_SYSTEM_ID_PMT_LIST_T
{
    UINT8    ui1_slot_id;
    BOOL     b_match;
} CI_CA_SYSTEM_ID_PMT_LIST_T;

/*------------------------------------------------------------------*/

/*------------------------------------------------------------------*/
/*! @struct CI_CA_SYSTEM_ID_STATUS_T
 *  @brief This structure contains current ca resouce status. 
 *  @code
 *  typedef struct _CI_CA_SYSTEM_ID_STATUS_T
 *  {
 *      UINT8 ui1_slot_id;
 *      CA_SYSTEM_ID_STATUS_T e_ca_sys_id_status;
 *  } CI_CA_SYSTEM_ID_STATUS_T;
 *  @endcode
 *  @li@c  ui1_slot_id                           -slot id, value range[0,slot num-1].
 *  @li@c  e_ca_sys_id_status                    -current ca resouce status
 */
/*------------------------------------------------------------------*/
typedef struct _CI_CA_SYSTEM_ID_STATUS_T
{
    UINT8    ui1_slot_id;
    CA_SYSTEM_ID_STATUS_T e_ca_sys_id_status;
} CI_CA_SYSTEM_ID_STATUS_T;
/*------------------------------------------------------------------*/
/*! @struct S_SUB_ATTRIBUTE
 *  @brief This structure contains one of user attribute. 
 *  @code
 * typedef struct sub_user_attribute
 * {
 * UINT8 at_ui1_operator_id[4];
 * UINT16 single_attribute_len;
 * UINT8* pui1_single_user_attribute;    
 * }S_SUB_ATTRIBUTE;
 *  @endcode
 *  @li@c  at_ui1_operator_id                           -operator id
 *  @li@c  single_attribute_len               -user attribute length
 *  @li@c   pui1_single_user_attribute            -user attribute
 */
/*------------------------------------------------------------------*/

typedef struct sub_user_attribute
{
  UINT8 at_ui1_operator_id[4];
  UINT16 ui2_single_attribute_len;
  UINT8* pui1_single_user_attribute;    
}S_SUB_ATTRIBUTE;
/*------------------------------------------------------------------*/
/*! @struct S_USER_ATTRIBUTE
 *  @brief This structure contains user attribute. 
 *  @code
 *typedef struct user_attribute
 *{  
 * S_SUB_ATTRIBUTE*   pt_cam_user_attribute;
 * UINT16             ui2_attribute_num;
 *}S_USER_ATTRIBUTE;

 *  @endcode
 *  @li@c  cam_user_attribute               -user attribue
 *  @li@c   attribute_num                      -indicate number of attribute cam send to mw. 
 */
/*------------------------------------------------------------------*/

typedef struct user_attribute
{  
  S_SUB_ATTRIBUTE*   pt_cam_user_attribute;
  UINT16              ui2_attribute_num;
}S_USER_ATTRIBUTE;

/*------------------------------------------------------------------*/
/*! @struct CI_HOST_COUNTRY_T
 *  @brief This structure contains the host country message.
 *  @code
 *typedef struct _CI_HOST_COUNTRY_T
 *{
 *   UINT8 ui1_slot_id;
 *   ISO_3166_COUNT_T    s3166_country_code;
 *} CI_HOST_COUNTRY_T;
 *  @endcode
 *  @li@c  ui1_slot_id                                            -slot id, value range[0,slot num-1].
 *  @li@c  s3166_country_code                -define country code
 */
/*------------------------------------------------------------------*/
typedef struct _CI_HOST_COUNTRY_T
{
    UINT8 ui1_slot_id;
    ISO_3166_COUNT_T    s3166_country_code;
} CI_HOST_COUNTRY_T;
/*------------------------------------------------------------------*/
/*! @struct CI_HOST_LANGUAGE_T
 *  @brief This structure contains the host language message.
 *  @code
 *typedef struct _CI_HOST_LANGUAGE_T
 *{
 *   UINT8 ui1_slot_id;
 *   ISO_639_LANG_T      s639_lang_code;
 * } CI_HOST_LANGUAGE_T;
 *  @endcode
 *  @li@c  ui1_slot_id                                            -slot id, value range[0,slot num-1].
 *  @li@c  s639_lang_code                -define host language code
 */
/*------------------------------------------------------------------*/
typedef struct _CI_HOST_LANGUAGE_T
{
    UINT8 ui1_slot_id;
    ISO_639_LANG_T      s639_lang_code;
} CI_HOST_LANGUAGE_T;

/*------------------------------------------------------------------*/
/*! @struct CI_HOST_REGION_ID_T
 *  @brief This structure contains the host region id message.
 *  @code
 *typedef struct CI_HOST_REGION_ID_T
 *{
 *   UINT8 ui1_slot_id;
 *   UINT32 ui4_region_id;
 *} CI_HOST_REGION_ID_T;
 *  @endcode
 *  @li@c  ui1_slot_id                            -slot id, value range[0,slot num-1].
 *  @li@c  ui4_region_id                          -define region_id
 */
/*------------------------------------------------------------------*/
typedef struct _CI_HOST_REGION_ID_T
{
    UINT8 ui1_slot_id;
    UINT32 ui4_region_id;
} CI_HOST_REGION_ID_T;

/*------------------------------------------------------------------*/
/*! @struct _CI_HOST_CABLE_BRDCSTER_T
 *  @brief This structure contains the host cable profile settings.
 *  @code
 *typedef struct _CI_HOST_CABLE_BRDCSTER_T
 *{
 *   UINT8 ui1_slot_id;
 *   UINT32 ui4_cable_brdcster;
 *} _CI_HOST_CABLE_BRDCSTER_T;
 *  @endcode
 *  @li@c  ui1_slot_id                            -slot id, value range[0,slot num-1].
 *  @li@c  ui4_cable_brdcster                     -define cable profile settings
 */
/*------------------------------------------------------------------*/
typedef struct _CI_HOST_CABLE_BRDCSTER_T
{
    UINT8 ui1_slot_id;
    UINT32 ui4_cable_brdcster;
} CI_HOST_CABLE_BRDCSTER_T;
/*------------------------------------------------------------------*/
/*! @struct _CI_HOST_SAT_BRDCSTER_T
 *  @brief This structure contains the host sat profile settings.
 *  @code
 *typedef struct _CI_HOST_SAT_BRDCSTER_T
 *{
 *   UINT8 ui1_slot_id;
 *   UINT32 ui4_sat_brdcster;
 *} CI_HOST_SAT_BRDCSTER_T;
 *  @endcode
 *  @li@c  ui1_slot_id                            -slot id, value range[0,slot num-1].
 *  @li@c  ui4_sat_brdcster                     -define cable profile settings
 */
/*------------------------------------------------------------------*/
typedef struct _CI_HOST_SAT_BRDCSTER_T
{
    UINT8 ui1_slot_id;
    UINT32 ui4_sat_brdcster;
} CI_HOST_SAT_BRDCSTER_T;


/*------------------------------------------------------------------*/
/*! @struct _CI_HOST_SAT_T
 *  @brief This structure contains the host sat settings.
 *  @code
 *typedef struct _CI_HOST_SAT_T
 *{
 *     UINT16          ui2_satl_id;        
 *     UINT16          ui2_satl_rec_id;    
 *} CI_HOST_SAT_T;
 *  @endcode
 *  @li@c  ui2_satl_id                          .
 *  @li@c  ui2_satl_rec_id                      
 */
/*------------------------------------------------------------------*/
typedef struct _CI_HOST_SAT_T
{
    UINT16          ui2_satl_id;        /* SATL id */
    UINT16          ui2_satl_rec_id;    /* SATL record id*/
} CI_HOST_SAT_T;

/*------------------------------------------------------------------*/
/*! @struct _CI_HOST_BRDCST_MEDIUM_T
 *  @brief This structure contains the host broadcast media settings.
 *  @code
 *typedef struct _CI_BRDCST_MEDIUM_T
 *{
 *   UINT8 ui1_slot_id;
 *   BRDCST_TYPE_T t_brdcst_type;
 *   BRDCST_MEDIUM_T t_brdcst_media;
 *} CI_HOST_BRDCST_MEDIUM_T;
 *  @endcode
 *  @li@c  ui1_slot_id                          -slot id, value range[0,slot num-1].
 *  @li@c  t_brdcst_type                        -define brdcster type, dvb, atsc, isdb..or others
 *  @li@c  t_brdcst_type                        -define brdcster media, TERRESTRIAL, CABLE, SATELLITE...
 */
/*------------------------------------------------------------------*/
typedef struct _CI_HOST_BRDCST_MEDIUM_T
{
    UINT8 ui1_slot_id;
    BRDCST_TYPE_T t_brdcst_type;
    BRDCST_MEDIUM_T t_brdcst_media;
} CI_HOST_BRDCST_MEDIUM_T;

/*------------------------------------------------------------------*/
/*! @struct CI_CAM_UPGRADE_T
 *  @brief This structure contains the cam upgrade message. 
 *  @code
 *  typedef struct _CI_CAM_UPGRADE_T
 *{
 *   UINT8 ui1_slot_id;
 *   UINT8 ui1_upgrade_type;
 *   UINT16 ui2_download_time;
 * } CI_CAM_UPGRADE_T;
 *  @endcode
 *  @li@c  ui1_slot_id                                            -slot id, value range[0,slot num-1].
 *  @li@c  ui1_upgrade_type                                  -this parameter identifies the type of CAM firmware upgrade requested:
 *                                                                          0x00: Delayed Upgrade mode
 *                                                                          0x01: Immediate Upgrade mode
 *  @li@c  ui2_download_time                                -The time in seconds, estimated to complete the firmware upgrade process. If the value is 0x0000 then the duration is unknown.
 */
/*------------------------------------------------------------------*/

typedef struct _CI_CAM_UPGRADE_T
{
    UINT8 ui1_slot_id;
    UINT8 ui1_upgrade_type;
    UINT16 ui2_download_time;
} CI_CAM_UPGRADE_T;

/*------------------------------------------------------------------*/
/*! @struct CI_CAM_UPGRADE_PROGRESS_T
 *  @brief This structure contains the cam upgrade progress message.
 *  @code
 * typedef struct _CI_CAM_UPGRADE_PROGRESS_T
 * {
 *    UINT8 ui1_slot_id;
 *    UINT8 ui1_download_progress_status;
 * } CI_CAM_UPGRADE_PROGRESS_T;
 *  @endcode
 *  @li@c  ui1_slot_id                                            -slot id, value range[0,slot num-1].
 *  @li@c  ui1_download_progress_status                -The percentage value of the CAM upgrade progress,in the range 0 to 100.
 */
/*------------------------------------------------------------------*/
typedef struct _CI_CAM_UPGRADE_PROGRESS_T
{
    UINT8 ui1_slot_id;
    UINT8 ui1_download_progress_status;
} CI_CAM_UPGRADE_PROGRESS_T;

/*------------------------------------------------------------------*/
/*! @struct CI_NFY_ERROR_DATA_T
 *  @brief This structure contains the error notification related data.
 *  @code
 *typedef struct _CI_NFY_ERROR_DATA_T
 *{
 *   UINT8 ui1_slot_id;
 *   CI_NFY_ERROR_CODE_T     e_nfy_err_code;
 *}CI_NFY_ERROR_DATA_T;
 *  @endcode
 *  @li@c  ui1_slot_id                                            -slot id, value range[0,slot num-1].
 *  @li@c  e_nfy_err_code                -error code.
 */
/*------------------------------------------------------------------*/
typedef struct _CI_NFY_ERROR_DATA_T
{
    UINT8 ui1_slot_id;
    CI_NFY_ERROR_CODE_T     e_nfy_err_code;
}CI_NFY_ERROR_DATA_T;

/*------------------------------------------------------------------*/
/*! @struct CI_CAM_UPGRADE_PROGRESS_REPLY_T
 *  @brief This structure contains the answer information of MMI menu object. 
 *  @code
 *typedef struct _CI_CAM_UPGRADE_PROGRESS_REPLY_T
 *{
 *   UINT32              ui4_id;
 *   UINT8               ui1_reply;
 *} CI_CAM_UPGRADE_PROGRESS_REPLY_T;
 *  @endcode
 *  @li@c  ui1_slot_id                                            -slot id, value range[0,slot num-1].
 *  @li@c  e_nfy_err_code                -0x00:means NO,0x01:means YES,0x02:means ASK the user
 */
/*------------------------------------------------------------------*/
typedef struct _CI_CAM_UPGRADE_PROGRESS_REPLY_T
{
    UINT32              ui4_id;
    UINT8               ui1_reply;
} CI_CAM_UPGRADE_PROGRESS_REPLY_T;



/*------------------------------------------------------------------*/
/*! @struct CI_APP_DOMAIN_ID_T
 *  @brief This structure defines the application identifier value sent from RequestStart. 
 *  @code
 * typedef struct _CI_APP_DOMAIN_ID_T
 *{
 *    UINT8 *pu1_appDomainID;
 *    UINT8 ui1_ssm; 
 * } CI_APP_DOMAIN_ID_T ; 
 *  @endcode
 *  @li@c  pu1_appDomainID      -is a null-terminated string. It should be "CIMHEGP1".
 *  @li@c  ui1_ssm                     -Refer to ci plus spec Table12.8 to handle
 */
/*------------------------------------------------------------------*/
typedef struct _CI_APP_DOMAIN_ID_T
{
    UINT8 *pu1_appDomainID;
    UINT8 ui1_ssm; 
} CI_APP_DOMAIN_ID_T ;


/*------------------------------------------------------------------*/
/*! @struct CI_REQUEST_START_T
 *  @brief This structure defines App Start Request message 
 *  @code
 * typedef struct _CI_REQUEST_START_T
 *{
 *   UINT8 *filename;
 *   CI_APP_DOMAIN_ID_T t_appDomainID ;
* } CI_REQUEST_START_T;
 *  @endcode
 *  @li@c  filename      -is a null-terminated string. 
 *  @li@c  t_appDomainID                     -contains app domain id
 */
/*------------------------------------------------------------------*/
typedef struct _CI_REQUEST_START_T
{
    UINT8 *filename;
    CI_APP_DOMAIN_ID_T t_appDomainID ;
} CI_REQUEST_START_T;


/*------------------------------------------------------------------*/
/*! @struct CI_REQUEST_START_ACK_T
 *  @brief This structure defines App Start Request ackowledge message, used in c_ci_set() with set type: CI_SET_TYPE_REQUEST_START_ACK
 *  @code
 *typedef struct _CI_REQUEST_START_ACK_T
 *{
 *   CI_REQUEST_START_ACK_CODE_T  t_ack_code;
 *} CI_REQUEST_START_ACK_T;
 *  @endcode
 *  @li@c  t_ack_code      -contains start app request ackowledge code. 
 */
/*------------------------------------------------------------------*/
typedef struct _CI_REQUEST_START_ACK_T
{
    CI_REQUEST_START_ACK_CODE_T  t_ack_code;
} CI_REQUEST_START_ACK_T;

/*------------------------------------------------------------------*/
/*! @struct CI_ABORT_REQUEST_T
 *  @brief This structure defines app abort request message,set data by CI_SET_TYPE_ABORT_REQUEST or CI_NFY_COND_APP_ABORT_REQUEST.
 *  @code
 *typedef struct _CI_ABORT_REQUEST_T
 *{
 *   CI_ABORT_REQUEST_CODE_T t_abort_code;
 *} CI_ABORT_REQUEST_T;
 *  @endcode
 *  @li@c  t_abort_code      -contains abort app request code. 
 */
/*------------------------------------------------------------------*/
typedef struct _CI_ABORT_REQUEST_T
{
    CI_ABORT_REQUEST_CODE_T t_abort_code;
} CI_ABORT_REQUEST_T;

/*------------------------------------------------------------------*/
/*!@struct CI_ABORT_ARC_T
 *  @brief This structure contains abort ack code. It is used to indicate that the MHEG5 CI+ application is stopped successfully.
 *typedef struct _CI_ABORT_ACK_T
 *{
 *   CI_ABORT_ACK_CODE_T t_abort_ack_code;
 *} CI_ABORT_ARC_T;
 *  @endcode
 *  @li@c  t_abort_ack_code      -contains abort app request ackowledge code. 
 */
/*------------------------------------------------------------------*/
typedef struct _CI_ABORT_ACK_T
{
    CI_ABORT_ACK_CODE_T t_abort_ack_code;
} CI_ABORT_ARC_T;


/*------------------------------------------------------------------*/
/*!@struct CI_FILE_REQUEST_T
 *  @brief This structure defines the data sent from RequestFile.
 * typedef struct _CI_FILE_REQUEST_T
 *{
 *    UINT8 *pu1fileReq;
 *    UINT32 ui4_length; 
 *} CI_FILE_REQUEST_T;
 
 *  @endcode
 *  @li@c  pu1fileReq      -contains request data buffer pointer.  
 *  @li@c  ui4_length      -contains request data length 
 */
/*------------------------------------------------------------------*/
typedef struct _CI_FILE_REQUEST_T
{
    UINT8 *pu1fileReq;
    UINT32 ui4_length; 
} CI_FILE_REQUEST_T;

/*------------------------------------------------------------------*/
/*!@struct CI_FILE_ACK_T
 *  @brief This structure defines the data sent from RequestFileAck.
 * typedef struct _CI_FILE_ACK_T
 *{
 *   BOOL b_file_ok;
 *   UINT8 *pu1fileAck;
 *   UINT32 ui4_length; 
 *} CI_FILE_ACK_T;
 
 *  @endcode
 *  @li@c  b_file_ok      -A 1 bit field is set to "1" if the file is available or this is an acknowledgement response to a FileRequest
 *                                 message with a RequestType of data, otherwise it shall be "0". 
 *  @li@c  pu1fileReq      -contains request ack data  buffer pointer.  
 *  @li@c  ui4_length      -contains request ack data length 
 */
/*------------------------------------------------------------------*/
typedef struct _CI_FILE_ACK_T
{
    BOOL b_file_ok;
    UINT8 *pu1fileAck;
    UINT32 ui4_length; 
} CI_FILE_ACK_T;


/*------------------------------------------------------------------*/
/*!@struct CI_FREE_ACK_DATA_T
 *  @brief This structure defines set data by CI_SET_TYPE_FREE_ACK_DATA.
 * typedef struct _CI_FREE_ACK_DATA_T
 *{
 *   UINT8 *pu1fileAck;
 *} CI_FREE_ACK_DATA_T; 
 *  @endcode
 *  @li@c  pu1fileAck      -contains request ack data  buffer pointer to be free.  If is NULL then means free all request ack data buffers.
 */
/*------------------------------------------------------------------*/
typedef struct _CI_FREE_ACK_DATA_T
{
    UINT8 *pu1fileAck;
} CI_FREE_ACK_DATA_T;

/*------------------------------------------------------------------*/
/*!@struct CI_SEARCH_MODE_FULL_T
 *  @brief This structure defines full file name search mode data.
 * typedef struct _CI_SEARCH_MODE_FULL_T
 *{
 *  UINT16  ui2_filename_len;
 *   CHAR    *pc_filename;
 *} CI_SEARCH_MODE_FULL_T;
 *  @endcode
 *  @li@c  ui2_filename_len      -contains file full name's length.
 *  @li@c  pc_filename      -contains file full name string.
 */
/*------------------------------------------------------------------*/
typedef struct _CI_SEARCH_MODE_FULL_T
{
    UINT16  ui2_filename_len;
    CHAR    *pc_filename;
} CI_SEARCH_MODE_FULL_T;


/*------------------------------------------------------------------*/
/*!@struct CI_SEARCH_MODE_PREFIX_EXT_T
 *  @brief This structure defines prefix and ext file name search mode data.
 * typedef struct _CI_SEARCH_MODE_PREFIX_EXT_T
 *{
 *   UINT16  ui2_prefix_len;
 *   CHAR    *pc_prefix;
 *   UINT16  ui2_extension_len;    
 *   CHAR    *pc_extension;
 *} CI_SEARCH_MODE_PREFIX_EXT_T;
 *  @endcode
 *  @li@c  ui2_prefix_len      -contains prefix of file name length.
 *  @li@c  pc_prefix      -contains prefix string.
  *  @li@c  ui2_extension_len      -contains file name extention name length.
 *  @li@c  pc_extension      -contains file extention name.
 */
/*------------------------------------------------------------------*/
typedef struct _CI_SEARCH_MODE_PREFIX_EXT_T
{
    UINT16  ui2_prefix_len;
    CHAR    *pc_prefix;
    UINT16  ui2_extension_len;    
    CHAR    *pc_extension;
} CI_SEARCH_MODE_PREFIX_EXT_T;


/*------------------------------------------------------------------*/
/*!@struct CI_SEARCH_MODE_PREFIX_T
 *  @brief This structure defines  file prefix name search mode data.
 * typedef struct _CI_SEARCH_MODE_PREFIX_T
 *{
 *   UINT16  ui2_prefix_len;
 *   CHAR    *pc_prefix;
 *} CI_SEARCH_MODE_PREFIX_T;
 *  @endcode
 *  @li@c  ui2_prefix_len      -contains prefix of file name length.
 *  @li@c  pc_prefix      -contains prefix string.
 */
/*------------------------------------------------------------------*/
typedef struct _CI_SEARCH_MODE_PREFIX_T
{
    UINT16  ui2_prefix_len;
    CHAR    *pc_prefix;
} CI_SEARCH_MODE_PREFIX_T;

/*------------------------------------------------------------------*/
/*!@struct CI_SEARCH_MODE_EXT_T
 *  @brief This structure defines  file extention name search mode data.
 * typedef struct _CI_SEARCH_MODE_EXT_T
 *{
 *   UINT16  ui2_extension_len;
 *    CHAR    *pc_extension;
 *} CI_SEARCH_MODE_EXT_T;
 *  @endcode
  *  @li@c  ui2_extension_len      -contains file name extention name length.
 *  @li@c  pc_extension      -contains file extention name.
 */
/*------------------------------------------------------------------*/
typedef struct _CI_SEARCH_MODE_EXT_T
{
    UINT16  ui2_extension_len;
    CHAR    *pc_extension;
} CI_SEARCH_MODE_EXT_T;

/*------------------------------------------------------------------*/
/*!@struct CI_SEARCH_MODE_DATA_T
 *  @brief This structure defines file search mode and its return data to update data from USB.
 * typedef struct _CI_SEARCH_MODE_DATA_T
 * {
 *   CI_FILE_SEARCH_MODE_T t_search_mode;
 *   union
 *   {
 *       CI_SEARCH_MODE_FULL_T          t_fullname;
 *       CI_SEARCH_MODE_PREFIX_EXT_T    t_prefix_ext;
 *       CI_SEARCH_MODE_PREFIX_T        t_prefix;
 *       CI_SEARCH_MODE_EXT_T           t_ext;            
 *   }u_para;
 * } CI_SEARCH_MODE_DATA_T;
 *  @endcode
  *  @li@c  t_search_mode      -contains file search mode.
 *  @li@c  t_fullname      -contains full file name search data.
 *  @li@c  t_prefix_ext      -contains prefix and extention  file name search data.
 *  @li@c  t_prefix      -contains  file prefix name search data.
 *  @li@c  t_ext      -contains file extention name search data.
 */
/*------------------------------------------------------------------*/
typedef struct _CI_SEARCH_MODE_DATA_T
{
    CI_FILE_SEARCH_MODE_T t_search_mode;
    union
    {
        CI_SEARCH_MODE_FULL_T          t_fullname;
        CI_SEARCH_MODE_PREFIX_EXT_T    t_prefix_ext;
        CI_SEARCH_MODE_PREFIX_T        t_prefix;
        CI_SEARCH_MODE_EXT_T           t_ext;            
    }u_para;
} CI_SEARCH_MODE_DATA_T;


/*------------------------------------------------------------------*/
/*!@struct CI_PLUS_CRED_INFO_T
 *  @brief This structure defines ci plus credential infomation data.
 *typedef struct _CI_PLUS_CRED_INFO_T
 *{
 *   BOOL    b_valid;                
 *   UINT16  ui2_custom_code_len;    
 *   CHAR    *pc_custom_code;        
 *   UINT16  ui2_serial_num_len;    
 *   CHAR    *pc_serial_num;        
 *   UINT8   ui1_device_id_len;     
 *   CHAR    ac_device_id[16];       
 * } CI_PLUS_CRED_INFO_T;
 *  @endcode
  *  @li@c  b_valid      -contains the flag it is true if valid.
 *  @li@c  ui2_custom_code_len      -contains length of custom code .
 *  @li@c  pc_custom_code      -contains pointer of custom code buffer .
 *  @li@c  ui2_serial_num_len      -contains length of serial number, it is addition info for remember the filename when key is input through USB .
 *  @li@c  pc_serial_num      -contains pointer of serial number.
 *  @li@c  ui1_device_id_len      -contains length of device id.
 *  @li@c  ac_device_id      -contains pointer of device id .
 */
/*------------------------------------------------------------------*/
typedef struct _CI_PLUS_CRED_INFO_T
{
    BOOL    b_valid;                
    UINT16  ui2_custom_code_len;    
    CHAR    *pc_custom_code;        
    UINT16  ui2_serial_num_len;    
    CHAR    *pc_serial_num;        
    UINT8   ui1_device_id_len;     
    CHAR    ac_device_id[16];       
} CI_PLUS_CRED_INFO_T;
/*------------------------------------------------------------------*/
/*! @brief This notify function is called by the CI Abstraction Layer to notify 
 * the status or condition change.
 *  @param [in] h_ci   -handle returned in c_ci_reg_nfy()
 *  @param [in] pv_nfy_tag   -private data set  in c_ci_reg_nfy()
 *  @param [in] e_nfy_cond   -notify condition
 *  @param [in] pv_data   -notify info,concrete data type is related to CI_NFY_COND_T e_nfy_cond
 *  @param [out] NONE
 *  @note
 *  @see c_ci_reg_nfy(),CI_NFY_COND_T
 *  @return None
 *  @retval      -
 */
/*------------------------------------------------------------------*/
typedef VOID (*x_ci_nfy_fct)(
    HANDLE_T            h_ci,
    VOID*               pv_nfy_tag,
    CI_NFY_COND_T       e_nfy_cond,
    VOID*               pv_data);

/*------------------------------------------------------------------*/
/*! @brief This query function is called by the CI Abstraction Layer to query 
 *  the data of system
 *  @param [in] h_ci   -handle returned in c_ci_reg_query_nfy()
 *  @param [in] pv_tag   -private data set  in c_ci_reg_query_nfy()
 *  @param [in] e_query_type   -query type
 *  @param[in] [out] pv_data   -query data buffer,concrete data type is related to CI_QUERY_TYPE_T e_query_type
 *  @param [in][out] pz_data_len - query data len
 *  @note
 *  @see c_ci_reg_query_nfy(),CI_QUERY_TYPE_T
 *  @return None
 *  @retval      -
 */
/*------------------------------------------------------------------*/
typedef VOID (*x_ci_query_info_fct)(
    HANDLE_T            h_ci,
    VOID*               pv_tag,
    CI_QUERY_TYPE_T     e_query_type,
    VOID*               pv_data,
    SIZE_T*             pz_data_len);

/******************************************************************************
structures  between MHEG5 module and CI module
********************************************************************************/
/*------------------------------------------------------------------*/
/*! @enum CI_MHEG5_NFY_COND_T
 *  @brief  specifies MHEG5 Notify error code 
 *  @code
 *  typedef enum
 *{
 *   CI_NFY_COND_FILE_ACK
 *} CI_MHEG5_NFY_COND_T;
 *  @endcode
 *  @li@c  CI_NFY_COND_FILE_ACK            -  mheg5 notify file acklowdge
 */
/*------------------------------------------------------------------*/
typedef enum
{
    CI_NFY_COND_FILE_ACK
} CI_MHEG5_NFY_COND_T;


/*------------------------------------------------------------------*/
/*! @enum CI_MHEG5_SET_TYPE
 *  @brief  specifies mheg5 set type message
 *  @code
 * typedef enum
 *{
 *   CI_MHEG5_SET_TYPE_FILE_REQUEST,
 *   CI_MHEG5_SET_TYPE_FREE_ACK_DATA
 *} CI_MHEG5_SET_TYPE ;
 
 *  @endcode
 *  @li@c  CI_MHEG5_SET_TYPE_FILE_REQUEST                    - Indicate file request set type
 *  @li@c  CI_MHEG5_SET_TYPE_FREE_ACK_DATA                - Indicate free file data acknowledge set type
 */
/*------------------------------------------------------------------*/
typedef enum
{
    CI_MHEG5_SET_TYPE_FILE_REQUEST,
    CI_MHEG5_SET_TYPE_FREE_ACK_DATA
} CI_MHEG5_SET_TYPE ;

/*------------------------------------------------------------------*/
/*! @brief  The CI engine will use this API to notify information to AP. Argument e_nfy_cond indicates the type of notification. 
 *  @param [in] h_ci   -handle returned in c_ci_reg_query_nfy()
 *  @param [in] pv_nfy_tag   -private data set  in x_ci_mheg5_reg_nfy()
 *  @param [in] e_nfy_cond   -notify condition
 *  @param[in] [out] pv_data   -pv_data data set in x_ci_mheg5_set
 *  @note
 *  @see x_ci_mheg5_reg_nfy(),CI_MHEG5_NFY_COND_T
 *  @return None
 *  @retval      -
 */
/*------------------------------------------------------------------*/
typedef VOID (*x_ci_mheg5_nfy_fct)(   
    HANDLE_T            h_ci,
    VOID*               pv_nfy_tag,
    CI_MHEG5_NFY_COND_T e_nfy_cond,
    VOID*               pv_data);

#define CI_PLUS_CAM_ID_LEN 8
#define CI_PLUS_PROFILE_LANGUAGE_CODE_LEN 3
/**************host control v2 resource******************/
/*------------------------------------------------------------------*/
/*! @struct CI_HC_TUNE_BROADCAST_T
 *  @brief This structure contains the Tune object information.
 *  @code
 *  typedef struct _CI_HC_TUNE_BROADCAST_T
 *  {
 *      UINT16              ui4_tsl_id;
 *      UINT16              ui4_svl_id;
 *      UINT16              ui2_tsl_rec_id;
 *      UINT16              ui2_svl_rec_id;
 *  } CI_HC_TUNE_BROADCAST_T;
 *  @endcode
 *  @li@c  ui4_tsl_id                              -TSL id
 *  @li@c  ui4_svl_id                             -SVL id
 *  @li@c  ui2_tsl_rec_id                        -TSL rec id
 *  @li@c  ui2_svl_rec_id                       -SVL rec id
 */
/*------------------------------------------------------------------*/
typedef struct _CI_HC_TUNE_BROADCAST_T
{
    UINT16              ui4_tsl_id;
    UINT16              ui4_svl_id;
    UINT16              ui2_tsl_rec_id;
    UINT16              ui2_svl_rec_id;
} CI_HC_TUNE_BROADCAST_T;

/**************operator profile resource******************/
/*------------------------------------------------------------------*/
/*! @struct CI_DATE_T
 *  @brief This structure contains date infomation.
 *  @code
 *  typedef struct 
 *  {
 *      UINT16 mjd;
 *     UINT16 year;
 *      UINT8 month;
 *      UINT8 day;
 *   } CI_DATE_T;
 *  @endcode
 *  @li@c  mjd                                      -is specified as UTC Modified Julian Date (MJD) as defined in EN 300 468 [10]
 *  @li@c  year                                     -year
 *  @li@c  month                                  -month
 *  @li@c  day                                      -day
 */
/*------------------------------------------------------------------*/
typedef struct 
{
   UINT16 mjd;
   UINT16 year;
   UINT8 month;
   UINT8 day;
} CI_DATE_T;

/*------------------------------------------------------------------*/
/*! @struct CI_TIME_T
 *  @brief This structure contains the time information.
 *  @code
 *  
 *  typedef struct 
 *  {
 *     UINT8 hour;
 *     UINT8 minute;
 *     UINT8 second;
 *     UINT8 hundredth;
 *   }CI_TIME_T;
 *  @endcode
 *  @li@c  hour                           -hour
 *  @li@c  minute                        -minute
 *  @li@c  second                        -second
 *  @li@c  hundredth                    -hundredth
 */
/*------------------------------------------------------------------*/
typedef struct 
{
   UINT8 hour;
   UINT8 minute;
   UINT8 second;
   UINT8 hundredth;
} CI_TIME_T;

/*------------------------------------------------------------------*/
/*! @enum CI_NETCARD_TYPE_T
 *  @brief  specifies net card type
 *  @code
 *  typedef enum
 *  {
 *      NETCARD_UNKNOW,
 *      ETHERNET_0,
 *      WIRELESS_0,
 *      WIRELESS_1,
 *   }CI_NETCARD_TYPE_T;
 *  @li@c  ETHERNET_0                          -eth0
 *  @li@c  WIRELESS_0                      - wifi0
 *  @li@c  WIRELESS_1                          -wifi1
 *  @endcode
 */
/*------------------------------------------------------------------*/
typedef enum
{
    NETCARD_UNKNOW,
    ETHERNET_0,
    WIRELESS_0,
    WIRELESS_1,
}CI_NETCARD_TYPE_T;

/*------------------------------------------------------------------*/

/*! @enum CI_PROFILE_TYPE_T
 *  @brief  specifies operator profle type
 *  @code
 *  typedef enum
 *  {
 *      CI_PROFILR_TYPE_0,
 *      CI_PROFILR_TYPE_1,
 *   }CI_PROFILE_TYPE_T;
 
 *  @endcode
 *  @li@c  CI_PROFILR_TYPE_0          - A non-profiled CICAM for a DVB network that follows normal DVB SI rules. The network service list is determined from the broadcast Service Information by the Host. This profile allows the CICAM to collect entitlement rights information. Additional information about the network behaviour may be defined and conveyed to the Host in the resource.
 *  @li@c  CI_PROFILR_TYPE_1          - Profiled operation where the Host constructs a local channel list explicitly for the service operator and the CICAM delivers an alternative CICAM NIT to the Host which defines the network. The Host does not interrogate the broadcast network to determine the logical channel line up.
 */
/*------------------------------------------------------------------*/
typedef enum
{
    CI_PROFILR_TYPE_0,
    CI_PROFILR_TYPE_1,
}CI_PROFILE_TYPE_T;

/*------------------------------------------------------------------*/
/*! @enum CI_PROFILE_SEARCH_REQ_TYPE_T
 *  @brief  specifies operator profle search request type
 *  @code
 * typedef enum
 *{
 *   CI_PROFILR_SEARCH_REQ_TYPE_WARNING,
 *   CI_PROFILR_SEARCH_REQ_TYPE_URGENT,
 *   CI_PROFILR_SEARCH_REQ_TYPE_NOT_INIT,
 *   CI_PROFILE_SEARCH_REQ_TYPE_SCHEDULE
 *} CI_MHEG5_SET_TYPE ;
 
 *  @endcode
 *  @li@c  CI_PROFILR_SEARCH_REQ_TYPE_WARNING          - Hint user with text"CI Plus Network change,you may search to update network!"
 *  @li@c  CI_PROFILR_SEARCH_REQ_TYPE_URGENT             - Hint user with text"CI Plus Network unavailable,you must search to update network!"
 *  @li@c  CI_PROFILR_SEARCH_REQ_TYPE_NOT_INIT           - Hint user with text"CI Plus Network not init,you must search to update network!"
 *  @li@c  CI_PROFILE_SEARCH_REQ_TYPE_SCHEDULE          - Hint user with text"CI Plus Network search schedule time reached,you may search to update network!"
 */
/*------------------------------------------------------------------*/
typedef enum
{
    CI_PROFILR_SEARCH_REQ_TYPE_WARNING,
    CI_PROFILR_SEARCH_REQ_TYPE_URGENT,
    CI_PROFILR_SEARCH_REQ_TYPE_NOT_INIT,
    CI_PROFILE_SEARCH_REQ_TYPE_SCHEDULE,
}CI_PROFILE_SEARCH_REQ_TYPE_T;

/*------------------------------------------------------------------*/
/*! @struct CI_CAM_SCAN_REQ_T
 *  @brief This structure contains the profile search infomation.
 *  @code
 *  typedef struct _CI_CAM_SCAN_REQ_T
 *  {
 *     UINT8          ui1_slot_id;
 *      CI_PROFILE_SEARCH_REQ_TYPE_T  t_scan_req_type;
 *     BOOL           b_req_query;
 *     CI_DATE_T      t_scan_req_date;
 *     CI_TIME_T      t_scan_req_time;
 * } CI_CAM_SCAN_REQ_T; 
 *  @endcode
 *  @li@c  ui1_slot_id                              -slot id
 *  @li@c  t_scan_req_type                      - search type
 *  @li@c  b_req_query                           - indicate whether need query user to start search
 *  @li@c  t_scan_req_date                      -if t_scan_req_type == CI_PROFILE_SEARCH_REQ_TYPE_SCHEDULE, this value indicates schedule date 
 *  @li@c  t_scan_req_time                      -if t_scan_req_type == CI_PROFILE_SEARCH_REQ_TYPE_SCHEDULE, this value indicates schedule time 
 */
/*------------------------------------------------------------------*/
typedef struct _CI_CAM_SCAN_REQ_T
{
    UINT8          ui1_slot_id;
    CI_PROFILE_SEARCH_REQ_TYPE_T  t_scan_req_type;
    BOOL           b_req_query;
    CI_DATE_T      t_scan_req_date;
    CI_TIME_T      t_scan_req_time;
} CI_CAM_SCAN_REQ_T; 

/*------------------------------------------------------------------*/
/*! @enum CI_PROFILE_TUNER_MODE_T
 *  @brief  specifies tuner mode that cam profile need support
 *  @code
 * typedef enum
 *{
 *   CI_PROFILR_TUNE_AIR,
 *   CI_PROFILR_TUNE_CABLE,
 *   CI_PROFILR_TUNE_SAT,
 *} CI_PROFILE_TUNER_MODE_T ;
 
 *  @endcode
 *  @li@c  CI_PROFILR_TUNE_AIR          - Tuner mode is Air
 *  @li@c  CI_PROFILR_TUNE_CABLE      - Tuner mode is Cable
 *  @li@c  CI_PROFILR_TUNE_SAT          - Tuner mode is Satelite
 */
/*------------------------------------------------------------------*/
typedef enum
{
    CI_PROFILR_TUNE_AIR,
    CI_PROFILR_TUNE_CABLE,    
    CI_PROFILR_TUNE_SAT
}CI_PROFILE_TUNER_MODE_T;

/*------------------------------------------------------------------*/
/*! @struct CI_PROFILE_SUPPORT_INFO_T
 *  @brief This structure contains the profile capability support infomation
 *  @code
 *  typedef struct _CI_PROFILE_SUPPORT_INFO_T
 *  {
 *     UINT8          ui1_slot_id;
 *     CI_PROFILE_TUNER_MODE_T  t_tune_mode;
 * } CI_PROFILE_SUPPORT_INFO_T; 
 *  @endcode
 *  @li@c  ui1_slot_id                          -slot id
 *  @li@c  t_tune_mode                      - tuner mode that cam need support
 */
/*------------------------------------------------------------------*/
typedef struct _CI_PROFILE_SUPPORT_INFO_T
{
    UINT8          ui1_slot_id;
    CI_PROFILE_TUNER_MODE_T  t_tune_mode;
} CI_PROFILE_SUPPORT_INFO_T; 
#define MAX_OP_NAME_LEN 256

typedef struct _CI_PROFILE_INFO_T
{
   UINT16 cicam_original_network_id;
   UINT32 cicam_identifier;
   UINT8 character_code_table;
   UINT8 encoding_type_id;
   UINT8 second_byte_value;
   UINT8 third_byte_value;
   BOOL  sdt_running_status_trusted;
   BOOL  eit_running_status_trusted;
   UINT8 eit_present_following_usage;
   UINT8 eit_schedule_usage;
   UINT8 extended_event_usage;
   BOOL sdt_other_trusted;
   BOOL eit_event_trigger;
   UINT8 nit_version;   
   UINT8 iso_639_language_code[3];
   UINT8 profile_name_length;
   UINT8 profile_name[MAX_OP_NAME_LEN];
} CI_PROFILE_INFO_T;

/**************Parental Control resource******************/
/*------------------------------------------------------------------*/
/*! @enum CI_PIN_CAPS_CODE_T
 *  @brief  specifies cam's pin capability infomation
 *  @code
 *  typedef enum 
 *  {
 *     CI_PIN_CAPS_NONE                 = 0x00,
 *     CI_PIN_CAPS_CAS_ONLY             = 0x01,
 *     CI_PIN_CAPS_CAS_AND_FTA          = 0x02,
 *     CI_PIN_CAPS_CAS_ONLY_CACHED      = 0x03,
 *     CI_PIN_CAPS_CAS_AND_FTA_CACHED   = 0x04
 *  } CI_PIN_CAPS_CODE_T;
 
 *  @endcode
 *  @li@c  CI_PIN_CAPS_NONE                    - The CICAM does not perform any parental rating on any services (FTA and CAS).
 *  @li@c  CI_PIN_CAPS_CAS_ONLY             - The CICAM does not perform any rating management on FTA services. The CICAM performs the CAS PIN code management for services that the CICAM is capable of descrambling (e.g. matching CA_system_ID).
 *  @li@c  CI_PIN_CAPS_CAS_AND_FTA           - The Host may request the CICAM to display a MMI screen for FTA services and CA services where the parental rating set in the Host is lower than the rating provided in the CICAM capabilities.
 *  @li@c  CI_PIN_CAPS_CAS_ONLY_CACHED   - The CICAM does not perform any rating management on FTA services. The CICAM performs the CAS PIN code management for services that the CICAM is capable of descrambling (e.g. matching CA_system_ID).The Host may use the Record Start Message to provide the CICAM with the CICAM PIN. In this case the CICAM shall use the cached PIN code for enabling uninterrupted recording.
 *  @li@c  CI_PIN_CAPS_CAS_AND_FTA_CACHED  -The Host may request the CICAM to display a MMI screen for FTA services and CA services where the parental rating set in the Host is lower than the rating provided in the CICAM capabilities. The Host may use the Record Start Message to provide the CICAM with the CICAM PIN. In this case the CICAM shall use the cached PIN code for enabling uninterrupted recording.
 */
/*------------------------------------------------------------------*/
typedef enum 
{
   CI_PIN_CAPS_NONE                 = 0x00,
   CI_PIN_CAPS_CAS_ONLY             = 0x01,
   CI_PIN_CAPS_CAS_AND_FTA          = 0x02,
   CI_PIN_CAPS_CAS_ONLY_CACHED      = 0x03,
   CI_PIN_CAPS_CAS_AND_FTA_CACHED   = 0x04
} CI_PIN_CAPS_CODE_T;
/*------------------------------------------------------------------*/
/*! @enum CI_PIN_STATUS_T
 *  @brief  specifies operator profle search request type
 *  @code
 *  typedef enum 
 *  {
 *     CI_PIN_BAD_CODE                  = 0x00,
 *     CI_PIN_CICAM_BUSY                = 0x01,
 *     CI_PIN_CODE_CORRECT              = 0x02,
 *     CI_PIN_CODE_UNCONFIRMED          = 0x03,
 *     CI_PIN_BLANK_NOT_REQUIRED        = 0x04,
 *     CI_PIN_CONTENT_SCRAMBLED         = 0x05
 *   } CI_PIN_STATUS_T;
 
 *  @endcode
 *  @li@c  CI_PIN_BAD_CODE                   - the PIN entered by the end-user is incorrect.
 *  @li@c  CI_PIN_CICAM_BUSY                - the CICAM is busy.
 *  @li@c  CI_PIN_CODE_CORRECT           - the PIN entered by the end-user is correct.
 *  @li@c  CI_PIN_CODE_UNCONFIRMED    - when multiple CA Systems are present and it is not known which would be used for the event to be recorded. This error response occurs when the Host sends a cc_PIN_cmd() to the CICAM when booking a recording if the CA system associated with the event is not known.
 *  @li@c  CI_PIN_BLANK_NOT_REQUIRED   - confirms that a PIN is not required by the CICAM to view the content, e.g. the Host is not required to blank the AV for CAS Controlled content.
 *  @li@c  CI_PIN_CONTENT_SCRAMBLED    - This error response occurs when the CICAM is not able to descramble the content, meaning that the content is not available for playback.
 */
/*------------------------------------------------------------------*/
typedef enum 
{
   CI_PIN_BAD_CODE                  = 0x00,
   CI_PIN_CICAM_BUSY                = 0x01,
   CI_PIN_CODE_CORRECT              = 0x02,
   CI_PIN_CODE_UNCONFIRMED          = 0x03,
   CI_PIN_BLANK_NOT_REQUIRED        = 0x04,
   CI_PIN_CONTENT_SCRAMBLED         = 0x05
} CI_PIN_STATUS_T;

/*------------------------------------------------------------------*/
/*! @enum CI_OPERATING_MODE_T
 *  @brief  specifies operator profle search request type
 *  @code
 * typedef enum
 *{
 *   CI_MODE_WATCH_AND_BUFFER         = 0x00,
 *   CI_MODE_TIMESHIFT                = 0x01,
 *   CI_MODE_UNATTENDED_RECORDING     = 0x02
 *   CI_MODE_WATCH_ONLY                        = 0x03
 *} CI_OPERATING_MODE_T ;
 
 *  @endcode
 *  @li@c  CI_MODE_WATCH_AND_BUFFER          - the user is watching live content that is also being recorded
 *  @li@c  CI_MODE_TIMESHIFT                          - the user is watching previously recorded content while the Host continues recording the live content.
 *  @li@c  CI_MODE_UNATTENDED_RECORDING  - the user has programmed the Host to record content in an unattended mode.
 *  @li@c  CI_MODE_WATCH_ONLY                    - the user is watching live content.
 */
/*------------------------------------------------------------------*/
typedef enum 
{
   CI_MODE_WATCH_AND_BUFFER         = 0x00,
   CI_MODE_TIMESHIFT                = 0x01,
   CI_MODE_UNATTENDED_RECORDING     = 0x02,
   CI_MODE_WATCH_ONLY               = 0x03
} CI_OPERATING_MODE_T;

/*------------------------------------------------------------------*/
/*! @enum CI_LICENSE_STATUS_T
 *  @brief  specifies operator profle search request type
 *  @code
 * typedef enum
 *{
 *    CI_LICENSE_OK              = 0x00,
 *    CI_LICENSE_NO_ENTITLEMENT  = 0x01,
 *    CI_LICENSE_UNDEFINED_ERROR = 0x02,
 *    CI_LICENSE_ENTITLEMENT_EXPIRED = 0x03,
 *    CI_LICENSE_PLAYCOUNT_EXCEED = 0x04,
 *    CI_LICENSE_RETENTION_EXCEED = 0x05,
 *    CI_LICENSE_INVALID = 0x06
 *} CI_LICENSE_STATUS_T ;
 
 *  @endcode
 *  @li@c  CI_LICENSE_OK          - license is ok
 *  @li@c  CI_LICENSE_NO_ENTITLEMENT               - Descrambling not possible, no entitlement (record only)
 *  @li@c  CI_LICENSE_UNDEFINED_ERROR            - Descrambling not possible, undefined error (record only)
 *  @li@c  CI_LICENSE_ENTITLEMENT_EXPIRED      - Entitlement rights expired (playback & status check)
 *  @li@c  CI_LICENSE_PLAYCOUNT_EXCEED          - Play count exceeded (playback & status check)
 *  @li@c  CI_LICENSE_RETENTION_EXCEED          - Retention limit exceeded (playback & status check)
 *  @li@c  CI_LICENSE_INVALID                            - Invalid license (playback & status check)
 */
/*------------------------------------------------------------------*/
typedef enum 
{
   CI_LICENSE_OK              = 0x00,
   CI_LICENSE_NO_ENTITLEMENT  = 0x01,
   CI_LICENSE_UNDEFINED_ERROR = 0x02,
   CI_LICENSE_ENTITLEMENT_EXPIRED = 0x03,
   CI_LICENSE_PLAYCOUNT_EXCEED = 0x04,
   CI_LICENSE_RETENTION_EXCEED = 0x05,
   CI_LICENSE_INVALID = 0x06
} CI_LICENSE_STATUS_T;

/*------------------------------------------------------------------*/
/*! @enum CI_RECORDING_STATUS_T
 *  @brief  specifies operator profle search request type
 *  @code
 * typedef enum
 *{
 *   CI_RECORDING_OK              = 0x00,
 *   CI_RECORDING_NO_CC_SUPPORT  = 0x01,
 *   CI_RECORDING_HOST_BUSY = 0x02,
 *    CI_RECORDING_AUTH_FAILED = 0x03,
 *    CI_RECORDING_CICAM_BUSY = 0x04,
 *    CI_RECORDING_MODE_ERROR = 0x05
 *} CI_RECORDING_STATUS_T ;
 
 *  @endcode
 *  @li@c  CI_RECORDING_OK                        - recording is ok
 *  @li@c  CI_RECORDING_NO_CC_SUPPORT   - no content control resource support
 *  @li@c  CI_RECORDING_HOST_BUSY           - host is busy
 *  @li@c  CI_RECORDING_AUTH_FAILED          - cam authentication process is failed
 *  @li@c  CI_RECORDING_CICAM_BUSY          - cam is busy
 *  @li@c  CI_RECORDING_MODE_ERROR          - recording mode error
 */
/*------------------------------------------------------------------*/
typedef enum 
{
   CI_RECORDING_OK              = 0x00,
   CI_RECORDING_NO_CC_SUPPORT  = 0x01,
   CI_RECORDING_HOST_BUSY = 0x02,
   CI_RECORDING_AUTH_FAILED = 0x03,
   CI_RECORDING_CICAM_BUSY = 0x04,
   CI_RECORDING_MODE_ERROR = 0x05
} CI_RECORDING_STATUS_T;

/*------------------------------------------------------------------*/
/*! @struct CI_RECORD_START_STATUS_T
 *  @brief This structure contains the record start status
 *  @code
 *  typedef struct _CI_RECORD_START_STATUS_T
 *  {
 *      CI_RECORDING_STATUS_T t_status;
 *      CP_INFO_LIST_T t_cp_info;
 *      UINT8 aui1_cam_id[CI_PLUS_CAM_ID_LEN];
 *  } CI_RECORD_START_STATUS_T;
 *  @endcode
 *  @li@c  t_status                          -record start status
 *  @li@c  t_cp_info                        -uri info when record start
 *  @li@c  aui1_cam_id                   -cam id
 */
/*------------------------------------------------------------------*/
typedef struct _CI_RECORD_START_STATUS_T
{
    CI_RECORDING_STATUS_T t_status;    
    CP_INFO_LIST_T t_cp_info;
    UINT8 aui1_cam_id[CI_PLUS_CAM_ID_LEN];
} CI_RECORD_START_STATUS_T;


/*------------------------------------------------------------------*/
/*! @struct CI_PIN_CAPS_T
 *  @brief This structure contains the cam pin capability  information.
 *  @brief this infomation is returned in response to the cc_PIN_capabilities_req and shall also be sent to the Host unsolicited whenever the PIN capabilities change including when the effective age rating at which the CICAM starts managing the PIN code is changed in the CICAM.
 *  @code
 *  typedef struct _CI_PIN_CAPS_T
 *  {
 *      CI_PIN_CAPS_CODE_T t_cap_code;
 *      CI_DATE_T t_pin_change_date;
 *      CI_TIME_T t_pin_change_time;    
 *      UINT8 rating;
 *  } CI_PIN_CAPS_T;
 *  @endcode
 *  @li@c  t_cap_code                              -cam pin capability code
 *  @li@c  t_pin_change_date                        -returns the date when the CICAM PIN was last changed.
 *  @li@c  t_pin_change_time                       -returns the time when the CICAM PIN was last changed.
 *  @li@c  rating                       -is coded as DVB rating (3+years). Rating is defined in EN 300 468 [10] parental rating descriptor. This is the current rating as set in the CICAM.
 */
/*------------------------------------------------------------------*/
typedef struct _CI_PIN_CAPS_T
{
    CI_PIN_CAPS_CODE_T t_cap_code;
    CI_DATE_T t_pin_change_date;
    CI_TIME_T t_pin_change_time;    
    UINT8 rating;
}CI_PIN_CAPS_T;

/*------------------------------------------------------------------*/
/*! @struct CI_RECORD_START_T
 *  @brief This structure contains record start infomation.
 *  @code
 *  typedef struct _CI_RECORD_START_T
 *  {
 *      CI_OPERATING_MODE_T mode;
 *      UINT16 program_number; 
 *      UINT8 *pin_data;
 *  } CI_RECORD_START_T;
 *  @endcode
 *  @li@c  mode                              -record mode
 *  @li@c  program_number            -recorded program number
 *  @li@c  pin_data                        -pin code data use in cam capability is CI_PIN_CAPS_CAS_ONLY_CACHED or CI_PIN_CAPS_CAS_AND_FTA_CACHED,need add '/0' to the end
 */
/*------------------------------------------------------------------*/
typedef struct _CI_RECORD_START_T
{
    CI_OPERATING_MODE_T mode;
    UINT16 program_number; 
    UINT8 *pin_data;
}CI_RECORD_START_T;

/*------------------------------------------------------------------*/
/*! @struct CI_PIN_EVENT_T
 *  @brief  This infomation  is sent by the CICAM for CA protected content only and performs two operations:
 *  @brief To notify the Host of a parental rating change.
 *  @brief To confirm the validity of the PIN sent in the record start protocol.
 *  @code
 * typedef struct _CI_PIN_EVENT_T
 * {
 *      UINT16 program_number;  
 *     CI_PIN_STATUS_T status;
 *      UINT8 rating;
 *      CI_DATE_T event_date;
 *      CI_TIME_T event_time;
 *      UINT8 private_data[15];
 * }CI_PIN_EVENT_T;
 *  @endcode
 *  @li@c  program_number       -the program number of the associated Record Start protocol for this recording.
 *  @li@c  status                        -the status of the previously submitted PIN code
 *  @li@c  rating                        -coded as DVB rating (3+years). Rating is defined in EN 300 468 [10] parental rating descriptor. It represents the rating of the broadcasted content item that triggered the cc_PIN_event() APDU.
 *  @li@c  event_date                -This field returns the date when the parental rating changed requiring the entry of a PIN.
 *  @li@c  event_time                -This field returns the time when the parental rating changed requiring the entry of a PIN.
 *  @li@c  private_data              -These private data bytes provide the CICAM with the option to include additional CAS specific information stored with the parental control rating in the recording. The private_data is returned to the CICAM on playback using the CI_PIN_PLAYBACK_T structure in CI_SET_TYPE_PIN_PLAYBACK.
 */
/*------------------------------------------------------------------*/
typedef struct _CI_PIN_EVENT_T
{
    UINT16 program_number;	
    CI_PIN_STATUS_T status;
    UINT8 rating;
    CI_DATE_T event_date;
    CI_TIME_T event_time;
    UINT8 private_data[15];
}CI_PIN_EVENT_T;

/*------------------------------------------------------------------*/
/*! @struct CI_PIN_PLAYBACK_T
 *  @brief This structure contains the Tune object information.
 *  @code
 * typedef struct _CI_PIN_PLAYBACK_T
 * {
 *     UINT8 rating;
 *    UINT8 private_data[15];
 *  }CI_PIN_PLAYBACK_T;
 *  @endcode
 *  @li@c  program_number       -the program number of the associated Record Start protocol for this recording.
 *  @li@c  private_data              -These private data bytes provide the CICAM with the option to include additional CAS specific information stored with the parental control rating in the recording. The private_data should be same as CI_PIN_EVENT_T info in CI_NFY_COND_PIN_EVENT
 */
/*------------------------------------------------------------------*/
typedef struct _CI_PIN_PLAYBACK_T
{
    UINT8 rating;
    UINT8 private_data[15];
}CI_PIN_PLAYBACK_T;

/*------------------------------------------------------------------*/
/*! @struct CI_LICENSE_EVENT_T
 *  @brief This structure contains the license event infomation
 *  @code
 * typedef struct _CI_LICENSE_EVENT_T
 * {
 *      UINT16 program_number;
 *      CI_LICENSE_STATUS_T license_status;
 *      CP_INFO_LIST_T t_cp_info;
 *      UINT8 *license;
 *      UINT8 license_len;
 *  }CI_LICENSE_EVENT_T;
 *  @endcode
 *  @li@c  program_number              -program number
 *  @li@c  license_status                   -license status
 *  @li@c  t_cp_info                          -uri info
 *  @li@c  license                             -license data array
 *  @li@c  license_len                       -lincense data length
 */
/*------------------------------------------------------------------*/
typedef struct _CI_LICENSE_EVENT_T
{
    UINT16 program_number;
    CI_LICENSE_STATUS_T license_status;
    CP_INFO_LIST_T t_cp_info;
    UINT8 *license;
    UINT16 license_len;
}CI_LICENSE_EVENT_T;

/*------------------------------------------------------------------*/
/*! @struct CI_LICENSE_Playback_T
 *  @brief This structure contains license playback infomation.
 *  @code
 *  typedef struct _CI_LICENSE_Playback_T
 *  {
 *      UINT16 program_number;
 *      UINT8 *license;
 *      UINT8 license_len;
 *   }CI_LICENSE_Playback_T;
 *  @endcode
 *  @li@c  program_number                  -program number
 *  @li@c  license                             -license data array
 *  @li@c  license_len                        -lincense data length
 */
/*------------------------------------------------------------------*/
typedef struct _CI_LICENSE_Playback_T
{
    UINT16 program_number;
    UINT8 *license;
    UINT16 license_len;
}CI_LICENSE_Playback_T;

typedef enum
{
    CI_TUNER_TS_EXT1 = 0,
    CI_TUNER_TS_EXT2,
    CI_TUNER_TS_NONE
} CI_TUNER_TS_T;

/*------------------------------------------------------------------*/
/*! @enum CI_DESCRAMBLE_CAPABILITY_T
 *  @brief  specifies cam descramble capability
 *  @code
 *  typedef enum 
 * {
 *    CI_DESCRAMBLE_POSSIBLE                                         = 0x01,
 *    CI_DESCRAMBLE_POSSIBLE_PURCHASE_DIALOG         = 0x02,
 *    CI_DESCRAMBLE_POSSIBLE_TECHNICAL_DIALOG        = 0x03,
 *    CI_DESCRAMBLE_POSSIBLE_NO_ENTITLEMENT            = 0x71,
 *    CI_DESCRAMBLE_POSSIBLE_TECHNICAL                      = 0x73,
 *    CI_DESCRAMBLE_POSSIBLE_UNKNOWN                       = 0x80
 * }  CI_DESCRAMBLE_CAPABILITY_T;
 *
 *  @endcode
 *  @li@c  CI_DESCRAMBLE_POSSIBLE                                   - can descramble
 *  @li@c  CI_DESCRAMBLE_POSSIBLE_PURCHASE_DIALOG   -
 *  @li@c  CI_DESCRAMBLE_POSSIBLE_TECHNICAL_DIALOG  - 
 *  @li@c  CI_DESCRAMBLE_POSSIBLE_NO_ENTITLEMENT      - 
 *  @li@c  CI_DESCRAMBLE_POSSIBLE_TECHNICAL                - 
 *  @li@c  CI_DESCRAMBLE_POSSIBLE_UNKNOWN                 - capability unknown
 */
/*------------------------------------------------------------------*/
typedef enum 
{
   CI_DESCRAMBLE_POSSIBLE                             = 0x01,
   CI_DESCRAMBLE_POSSIBLE_PURCHASE_DIALOG             = 0x02,
   CI_DESCRAMBLE_POSSIBLE_TECHNICAL_DIALOG            = 0x03,
   CI_DESCRAMBLE_NOT_POSSIBLE_NO_ENTITLEMENT          = 0x71,
   CI_DESCRAMBLE_NOT_POSSIBLE_TECHNICAL               = 0x72,
   CI_DESCRAMBLE_POSSIBLE_UNKNOWN                     = 0x80
} CI_DESCRAMBLE_CAPABILITY_T;

/*------------------------------------------------------------------*/
/*! @struct CI_REPLY_DESCRAMBLE_CAPABILITY_T
 *  @brief This structure contains descramble capability infomation.
 *  @code
 *  typedef struct _CI_REPLY_DESCRAMBLE_CAPABILITY_T
 *  {
 *      UINT8    ui1_slot_id;
 *     CI_DESCRAMBLE_CAPABILITY_T ci_desc_cap_t;
 *   }CI_REPLY_DESCRAMBLE_CAPABILITY_T;
 *  @endcode
 *  @li@c  ui1_slot_id                       -ui1_slot_id (0,1,2....)
 *  @li@c  license                             -capability

 */
/*------------------------------------------------------------------*/
typedef struct _CI_REPLY_DESCRAMBLE_CAPABILITY_T
{
    UINT8    ui1_slot_id;
    CI_DESCRAMBLE_CAPABILITY_T ci_desc_cap_t;
}CI_REPLY_DESCRAMBLE_CAPABILITY_T;

#endif /* _U_CI_H_ */

/*----------------------------------------------------------------------------*/
/*! @} */   /* end of  groupMW_CI*/
/*----------------------------------------------------------------------------*/
