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
 * $RCSfile: x_ssum.h,v $
 * $Revision: #1 $
 * $Date: 2015/01/09 $
 * $Author: dtvbm11 $
 *
 * Description:
 *         This file contains all x_ssum function prototypes.
 *---------------------------------------------------------------------------*/

#ifndef _X_SSUM_H_
#define _X_SSUM_H_

/*--------------------------------------------------------------------------
                    include files
--------------------------------------------------------------------------*/
#include "u_ssum.h"


/*--------------------------------------------------------------------------
                    functions declarations
 -------------------------------------------------------------------------*/
extern INT32    x_ssum_init(VOID);

/*------------------------------------------------------------------*/
/*! @brief  Open a SSU Manager to detect if there is
 *          - SSU(System Software Update) or
 *          - OAD(Over Air Download)
 *          being trasmitted in the transport stream by checking NIT Actual.
 *  @param  [in]  s_tuner_name  - Tuner name of the tuner receiving the RF signal.
 *  @param  [in]  ui4_oui       - OUI of the manufacturer's software to detect.
 *  @param  [in]  pv_tag        - Memory pointer of the private tag to be used in
 *                                the notification function.
 *  @param  [in]  pf_nfy        - Notification function to be used.
 *  @param  [out] ph_download   - Handle to the SSU Manager being opened.
 *  @note
 *  @see    x_ssum_open_by_type_nid, x_ssum_open_by_setting, x_cm_close
 *  @return Return the error code
 *  @retval SSUMR_OK            - The routine was successful.
 *  @retval SSUMR_INV_ARG       - Invalid argument, s_tuner_name is NULL.
 *  @retval SSUMR_CORE          - Memory allocation failed.
 *  @retval SSUMR_HANDLE_FAIL   - Handle allocation failed.
 *  @retval SSUMR_SEND_MSG_FAIL - Internal message sent failed.
 */
/*------------------------------------------------------------------*/
extern INT32    x_ssum_open(CHAR*           s_tuner_name,
   			                SSU_OPEN_SETTING_T  *pt_ssu_open_setting,
                            VOID*           pv_tag,
                            x_ssum_nfy_fct  pf_nfy,
                            HANDLE_T*       ph_download);

/*------------------------------------------------------------------*/
/*! @brief  Close a SSU Manager specified by h_download, and free related resources.
 *  @param  [in] h_download     - Handle of the SSU Manager to be closed.
 *  @note
 *  @see    x_ssum_open, x_ssum_open_by_type_nid
 *  @return Return the error code
 *  @retval SSUMR_OK            - The routine was successful.
 *  @retval SSUMR_INV_ARG       - ph_download handle is invalid.
 *  @retval SSUMR_CORE          - Internal error.
 *  @retval SSUMR_SEND_MSG_FAIL - Internal message sent failed.
 */
/*------------------------------------------------------------------*/
extern INT32    x_ssum_close(HANDLE_T h_download);

/*------------------------------------------------------------------*/
/*! @brief  Download file by specifying file ID
 *  @param  [in] h_download - Handle of the SSU Manager to download from
 *  @param  [in] pt_setting - Contains download information, including 
 *                            file ID, buffer length, buffer pointer, and file offset
 *  @param  [in] pv_tag     - Private tag to be used in the download
 *                            progress notification function
 *  @param  [in] pf_nfy     - Download progress notification function
 *  @note
 *  @see    x_ssum_download_by_name
 *  @return Return the error code
 *  @retval SSUMR_OK            - The routine was successful.
 *  @retval SSUMR_INV_ARG       - One of the arguments is invalid.
 *  @retval SSUMR_CORE          - Internal error.
 *  @retval SSUMR_SEND_MSG_FAIL - Internal message sent failed.
 */
/*------------------------------------------------------------------*/
extern INT32    x_ssum_download_by_id(HANDLE_T                h_download,
                                      SSU_DOWNLOAD_SETTING_T  *pt_setting,
                                      VOID                    *pv_tag,
                                      x_ssum_download_nfy_fct pf_nfy);

/*------------------------------------------------------------------*/
/*! @brief  Download file by specifying file ID with two buffers
 *  @param  [in] h_download - Handle of the SSU Manager to download from
 *  @param  [in] pt_setting - Contains download information, including 
 *                            file ID, buffer length, buffer pointer, and file offset
 *  @param  [in] pv_tag     - Private tag to be used in the download
 *                            progress notification function
 *  @param  [in] pf_nfy     - Download progress notification function
 *  @note
 *  @see    x_ssum_download_by_name
 *  @return Return the error code
 *  @retval SSUMR_OK            - The routine was successful.
 *  @retval SSUMR_INV_ARG       - One of the arguments is invalid.
 *  @retval SSUMR_CORE          - Internal error.
 *  @retval SSUMR_SEND_MSG_FAIL - Internal message sent failed.
 */
/*------------------------------------------------------------------*/
extern INT32    x_ssum_download_by_id_ex(HANDLE_T                   h_download,
                                         SSU_DOWNLOAD_SETTING_EX_T  *pt_setting,
                                         VOID                       *pv_tag,
                                         x_ssum_download_nfy_fct    pf_nfy);

/*------------------------------------------------------------------*/
/*! @brief  Download file by specifying file name
 *  @param  [in] h_download - Handle of the SSU Manager to download from
 *  @param  [in] pt_setting - Contains download information, including
 *                            file name, buffer length, buffer pointer, and file offset
 *  @param  [in] pv_tag     - Private tag to be used in the download
 *                            progress notification function
 *  @param  [in] pf_nfy     - Download progress notification function
 *  @note
 *  @see    x_ssum_download_by_id
 *  @return Return the error code
 *  @retval SSUMR_OK            - The routine was successful.
 *  @retval SSUMR_INV_ARG       - One of the arguments is invalid.
 *  @retval SSUMR_CORE          - Internal error.
 *  @retval SSUMR_SEND_MSG_FAIL - Internal message sent failed.
 */
/*------------------------------------------------------------------*/
extern INT32    x_ssum_download_by_name(HANDLE_T                h_download,
                                        SSU_DOWNLOAD_SETTING_T  *pt_setting,
                                        VOID                    *pv_tag,
                                        x_ssum_download_nfy_fct pf_nfy);
/*------------------------------------------------------------------*/
/*! @brief  Get information from SSU Manager
 *  @param  [in]      h_download      - Handle of the SSU Manager to get info from.
 *  @param  [in]      e_ssu_get_type  - Get type.
 *  @param  [in]      pv_get_info     - Memory pointer to addtional information to be
 *                                      used for the get type.
 *  @param  [out]     pv_get_data     - Memory pointer to contain the get data.
 *  @param  [in,out]  pz_get_data_len - [in]  Contains the max size of pv_get_data.
 *                                    - [out] Contains the size of the returned get data.
 *  @note
 *  @see    x_ssum_set
 *  @return Return the error code
 *  @retval SSUMR_OK      - The routine was successful.
 *  @retval SSUMR_INV_ARG - One of the arguments is invalid.
 */
/*------------------------------------------------------------------*/
extern INT32    x_ssum_get(HANDLE_T         h_download,
                           SSU_GET_TYPE_T   e_ssu_get_type,
                           VOID*            pv_get_info,
                           VOID*            pv_get_data,
                           SIZE_T*          pz_get_data_len);

/*------------------------------------------------------------------*/
/*! @brief  Set information to SSU Manager
 *  @param  [in]  h_download      - Handle of the SSU Manager to get info from.
 *  @param  [in]  e_ssu_set_type  - Set type.
 *  @param  [in]  pv_set_info     - Memory pointer to addtional information to be
 *                                  used for the set type.
 *  @param  [in]  pv_set_data     - Memory pointer to contain the set data.
 *  @param  [in]  pz_set_data_len - Contains the size of the set data.
 *  @note
 *  @see    x_ssum_get
 *  @return Return the error code
 *  @retval SSUMR_OK            - The routine was successful.
 *  @retval SSUMR_INV_ARG       - One of the arguments is invalid.
 *  @retval SSUMR_CORE          - Internal error.
 *  @retval SSUMR_SEND_MSG_FAIL - Internal message sent failed.
 */
/*------------------------------------------------------------------*/
extern INT32    x_ssum_set(HANDLE_T         h_download,
                           SSU_SET_TYPE_T   e_ssu_set_type,
                           VOID*            pv_set_info,
                           VOID*            pv_set_data,
                           SIZE_T*          pz_set_data_len);

/*------------------------------------------------------------------*/
/*! @brief  Get debug level
 *  @note
 *  @see    x_ssum_set_dbg_level
 *  @return Return the debug level
 *  @retval DBG_INIT_LEVEL_MW_SSU_MNGR - The routine was successful.
 *  @retval 0                          - The routine was successful.
 */
/*------------------------------------------------------------------*/
extern UINT16   x_ssum_get_dbg_level(VOID);

/*------------------------------------------------------------------*/
/*! @brief  Set debug level
 *  @note
 *  @see    x_ssum_get_dbg_level
 *  @return Return the set result
 *  @retval TRUE     - The routine was successful.
 *  @retval FALSE    - The routine failed.
 */
/*------------------------------------------------------------------*/
extern BOOL     x_ssum_set_dbg_level(UINT16 ui2_level);

/*------------------------------------------------------------------*/
/*! @brief  Get current DDB number
 *  @param  [in] h_download - Handle of the SSU Manager to download from
 *  @param  [in] UINT32     - Current DDB number
 *  @note
 *  @see    x_ssum_get_current_ddb_number
 *  @return Return the error code
 *  @retval SSUMR_OK            - The routine was successful.
 *  @retval SSUMR_INV_ARG       - One of the arguments is invalid.
 *  @retval SSUMR_CORE          - Internal error.
 */
/*------------------------------------------------------------------*/
extern INT32    x_ssum_get_current_ddb_number(HANDLE_T    h_download,
                                              UINT32*     ui4_ddb_number);

#if 0
extern INT32 x_ssum_get_file_info_by_id( HANDLE_T h_download,
    UINT32 ui4_file_id,
    SSU_INFO_TYPE_T e_file_info_type,
    SSU_INFO_T *pt_file_info);
#endif
#endif
