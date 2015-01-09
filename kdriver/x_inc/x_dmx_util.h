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
 * $RCSfile: x_dmx_util.h,v $
 * $Revision: #1 $
 * $Date: 2015/01/09 $
 * $Author: dtvbm11 $
 *
 * Description:
 *         
 *---------------------------------------------------------------------------*/

#ifndef _X_DMX_UTIL_H_
#define _X_DMX_UTIL_H_

/*-----------------------------------------------------------------------------
                    include files
-----------------------------------------------------------------------------*/

#include "x_common.h"
#include "u_dmx_util.h"

/*----------------------------------------------------------------------------*/
/*! @addtogroup groupMW_DMX_UT
 *  @{ 
 */
/*----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 * structure, constants, macro definitions
 *---------------------------------------------------------------------------*/
 

/*-----------------------------------------------------------------------------
                    functions declarations
 ----------------------------------------------------------------------------*/

/*------------------------------------------------------------------*/
/*! @brief  This API init dmx util module
 *  @param  [in]  pv_data - Contains initial data.
 *  @return return the error code
 *  @retval DMXR_UT_NOT_INIT - dmx util module not init
 *  @retval DMXR_UT_OK - Routine successful.
 */
/*------------------------------------------------------------------*/ 
extern INT32 x_dmx_ut_init(VOID* pv_data);

/*------------------------------------------------------------------*/
/*! @brief  This API alloc a pid filter
 *  @param  [in]  t_pid - stream or table identify 
 *  @param  [in]  z_buf_size - for this pid cache size
 *  @param  [out]  ph_pidf_ctrl - a handle for pid filter control
 *  @return return the error code
 *  @retval DMXR_UT_NOT_INIT - dmx util module not init
 *  @retval DMXR_UT_OK - Routine successful.
 */
/*------------------------------------------------------------------*/ 
extern INT32 x_dmx_ut_pid_filter_alloc(
            MPEG_2_PID_T        t_pid,
            SIZE_T              z_buf_size,
            HANDLE_T*           ph_pidf_ctrl);

/*------------------------------------------------------------------*/
/*! @brief  This API allow client set a new callback function
 *  @param  [in]  h_pidf_ctrl - pid filter handle 
 *  @param  [in]  pf_dmx_ut_nfy - client impletement this callback function
 *  @param  [out]  ui4_nfy_data - this is a tag
 *  @return return the error code
 *  @retval DMXR_UT_NOT_INIT - dmx util module not init
 *  @retval DMXR_UT_INV_ARGS - pf_dmx_ut_nfy is NULL
 *  @retval DMXR_UT_INV_HANDLE - h_pidf_ctrl is NULL
 *  @retval DMXR_UT_OK - Routine successful.
 */
/*------------------------------------------------------------------*/ 
extern INT32 x_dmx_ut_pid_filter_set_nfy(
            HANDLE_T            h_pidf_ctrl,
            x_dmx_ut_nfy_fct    pf_dmx_ut_nfy,
            UINT32              ui4_nfy_data);


/*------------------------------------------------------------------*/
/*! @brief  This API free a pid filter handle
 *  @param  [in]  h_pidf_ctrl - pid filter handle 
 *  @return return the error code
 *  @retval DMXR_UT_NOT_INIT - dmx util module not init
 *  @retval DMXR_UT_INV_HANDLE - h_pidf_ctrl invalid
 *  @retval DMXR_UT_OK - Routine successful.
 */
/*------------------------------------------------------------------*/ 
extern INT32 x_dmx_ut_pid_filter_free(HANDLE_T h_pidf_ctrl);

/*------------------------------------------------------------------*/
/*! @brief  This API alloc a section filter
 *  @param  [in]  h_pidf_ctrl - pid filter handle 
 *  @param  [out]  ph_secf_ctrl - a handle for section filter control
 *  @return return the error code
 *  @retval DMXR_UT_NOT_INIT - dmx util module not init
 *  @retval DMXR_UT_INV_HANDLE - h_pidf_ctrl invalid
 *  @retval DMXR_UT_OK - Routine successful.
 */
/*------------------------------------------------------------------*/ 
extern INT32 x_dmx_ut_sec_filter_alloc(
            HANDLE_T           h_pidf_ctrl,
            HANDLE_T*          ph_secf_ctrl);

/*------------------------------------------------------------------*/
/*! @brief  This API config a section filter for add table id information
 *  @param  [in]  h_secf_ctrl - section filter handle 
 *  @param  [out]  pt_sf_info - config detail struction info
 *  @return return the error code
 *  @retval DMXR_UT_NOT_INIT - dmx util module not init
 *  @retval DMXR_UT_INV_HANDLE - h_secf_ctrl invalid
 *  @retval DMXR_UT_OK - Routine successful.
 */
/*------------------------------------------------------------------*/ 
extern INT32 x_dmx_ut_sec_filter_set(
            HANDLE_T            h_secf_ctrl,
            DMX_UT_SF_INFO_T*   pt_sf_info);

/*------------------------------------------------------------------*/
/*! @brief  This API control a section filter to active or inactive
 *  @param  [in]  h_secf_ctrl - section filter handle 
 *  @param  [out]  b_enable - open filter or disable. 
 *  @return return the error code
 *  @retval DMXR_UT_NOT_INIT - dmx util module not init
 *  @retval DMXR_UT_INV_HANDLE - h_secf_ctrl invalid
 *  @retval DMXR_UT_OK - Routine successful.
 */
/*------------------------------------------------------------------*/ 
extern INT32 x_dmx_ut_sec_filter_ctrl(
            HANDLE_T    h_secf_ctrl,
            BOOL        b_enable);

/*------------------------------------------------------------------*/
/*! @brief  This API free a section filter handle
 *  @param  [in]  h_secf_ctrl - section filter handle 
 *  @return return the error code
 *  @retval DMXR_UT_NOT_INIT - dmx util module not init
 *  @retval DMXR_UT_INV_HANDLE - h_secf_ctrl invalid
 *  @retval DMXR_UT_OK - Routine successful.
 */
/*------------------------------------------------------------------*/ 
extern INT32 x_dmx_ut_sec_filter_free(HANDLE_T h_secf_ctrl);

/*------------------------------------------------------------------*/
/*! @brief  This API copy section data when dmx utility notify to client
 *  @param  [in]  h_pidf_ctrl - pid filter handle 
 *  @param  [in]  pv_src_tag - a tag from dmx utility notify parament
 *  @param  [in]  z_skip_len - skip some section data 
 *  @param  [in]  z_copy_len - the length for copy 
 *  @param  [in]  pui1_buf - section header
 *  @return return the error code
 *  @retval DMXR_UT_NOT_INIT - dmx util module not init
 *  @retval DMXR_UT_INV_ARGS - pui1_buf is NULL
 *  @retval DMXR_UT_INV_HANDLE - h_pidf_ctrl is NULL
 *  @retval DMXR_UT_OK - Routine successful.
 */
/*------------------------------------------------------------------*/ 
extern INT32 x_dmx_ut_sec_data_read(HANDLE_T   h_pidf_ctrl,
                                    VOID*      pv_src_tag,
                                    UINT32     ui4_dmx_data,
                                    SIZE_T     z_skip_len,
                                    SIZE_T     z_copy_len,
                                    UINT8*     pui1_buf);

/*------------------------------------------------------------------*/
/*! @brief  This API was call after copy section completely.
 *  @param  [in]  h_pidf_ctrl - pid filter handle 
 *  @param  [in]  pv_src_tag - a tag from dmx utility notify parament
 *  @param  [in]  z_sec_size - release size. take small value between Notify section length and Max buffer length.
 *  @return return the error code
 *  @retval DMXR_UT_NOT_INIT - dmx util module not init
 *  @retval DMXR_UT_INV_HANDLE - h_pidf_ctrl is NULL
 *  @retval DMXR_UT_OK - Routine successful.
 */
/*------------------------------------------------------------------*/ 
extern INT32 x_dmx_ut_sec_data_release(HANDLE_T   h_pidf_ctrl,
                                       VOID*      pv_src_tag,
                                       UINT32     ui4_dmx_data,
                                       SIZE_T     z_sec_size);

/*------------------------------------------------------------------*/
/*! @brief  This API was reset demux driver data of all relation PID fitler
 *  @param  [in]  h_pidf_ctrl - pid filter handle 
 *  @return return the error code
 *  @retval DMXR_UT_NOT_INIT - dmx util module not init
 *  @retval DMXR_UT_INV_HANDLE - h_pidf_ctrl is NULL
 *  @retval DMXR_UT_OK - Routine successful.
 */
/*------------------------------------------------------------------*/                                  
extern INT32 x_dmx_ut_sec_buf_flush(HANDLE_T   h_pidf_ctrl);

extern UINT16 dmx_ut_get_dbg_level(VOID);

extern VOID dmx_ut_set_dbg_level(UINT16 ui2_db_level);

#endif /* _X_DMX_UTIL_H_ */

/*----------------------------------------------------------------------------*/
/*! @} */                                           /* end of groupMW_DMX_UT */
/*----------------------------------------------------------------------------*/


