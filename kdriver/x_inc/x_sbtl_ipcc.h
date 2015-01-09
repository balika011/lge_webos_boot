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
/*! @addtogroup groupMW_STRM_MNGR_MM_SBTL_HDLR
 *  @{ 
 */
/*----------------------------------------------------------------------------*/
#ifndef _X_SBTL_IPCC_H_
#define _X_SBTL_IPCC_H_
#ifndef __KERNEL__

/*-----------------------------------------------------------------------------
                    include files
-----------------------------------------------------------------------------*/
//#include "x_handle.h"
//#include "x_sm.h"
//#include "u_mm_sbtl_engine.h"
#include "u_sbtl_ipcc.h"
#include "x_handle.h"
#include "x_sm.h"
#include "u_mm_sbtl_engine.h"


/*-----------------------------------------------------------------------------
                    function declaration
-----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------*/
/*! @brief  This API is used to open ipcc
 *
 * @param [in]      h_canvas        - The handle of canvas
 * @param [in]      pv_cmd_sets     - The pointer of arg array
 * @param [out]     None
 * @return return the error code
 * @retval MMSBTLR_OK               - Routine successful.
 * @retval MMSBTLR_WGL_FAIL         - Routine Failed.
 * @retval MMSBTLR_OUT_OF_HANDLE    - Handle is invalid
 */
/* ----------------------------------------------------------------------------*/
extern INT32 x_sbtl_ipcc_open_fct(HANDLE_T h_canvas, VOID* pv_cmd_sets);




/*-----------------------------------------------------------------------------*/
/*! @brief  This API is used to close ipcc
 *
 * @param [in]      None        
 * @param [out]     None
 * @return return the error code
 * @retval MMSBTLR_OK               - Routine successful.
 * @retval MMSBTLR_WGL_FAIL         - Routine Failed.
 *
 */
/* ----------------------------------------------------------------------------*/
extern INT32 x_sbtl_ipcc_close_fct(VOID);




/*-----------------------------------------------------------------------------*/
/*! @brief  This API is used to set attributes of ipcc
 *
 * @param [in]      e_set_type          - The type of attribute
 * @param [in]      pv_set_info         - The info of attribute
 * @param [in]      z_set_info_len      - The length of attribute
 * @param [out]     None
 * @return return the error code
 * @retval MMSBTLR_OK                   - Routine successful.
 * @retval MMSBTLR_WGL_FAIL             - Routine Failed.
 * @retval MMSBTLR_INV_ARG              - Invalid argument.
 * @retval MMSBTLR_INSUFFICIENT_SIZE    - Memory is insufficient.
 */
/* ----------------------------------------------------------------------------*/
extern INT32 x_sbtl_ipcc_set_fct(SBTL_IPCC_SET_TYPE e_set_type, VOID* pv_set_info, SIZE_T z_set_info_len);




/*-----------------------------------------------------------------------------*/
/*! @brief  This API is used to get attributes of ipcc
 *
 * @param [in]      e_get_type          - The type of attribute
 * @param [in]      pv_get_info         - The info of attribute
 * @param [in]      z_get_info_len      - The length of attribute
 * @param [out]     None
 * @return return the error code
 * @retval MMSBTLR_OK                   - Routine successful.
 */
/* ----------------------------------------------------------------------------*/
extern INT32 x_sbtl_ipcc_get_fct(SBTL_IPCC_GET_TYPE e_get_type, VOID* pv_get_info, SIZE_T z_get_info_len);




/*-----------------------------------------------------------------------------*/
/*! @brief  This API is used to remove window of ipcc
 *
 * @param [in]      ps_crnt_wnd         - The id of <region>
 * @param [out]     None
 * @return return the error code
 * @retval MMSBTLR_OK                   - Routine successful.
 * @retval MMSBTLR_INV_ARG              - Invalid argument.
 */
/* ----------------------------------------------------------------------------*/
extern INT32 x_sbtl_ipcc_remove(CHAR* ps_crnt_wnd);




/*-----------------------------------------------------------------------------*/
/*! @brief  This API is used to init IPCC
 *
 * @param [in]      pt_ipcc_config      - The config of IPCC
 * @param [out]     None
 * @return return the error code
 * @retval MMSBTLR_OK                   - Routine successful.
 * @retval MMSBTLR_INV_ARG              - Invalid argument.
 */
/* ----------------------------------------------------------------------------*/
extern INT32 x_sbtl_ipcc_init(SBTL_IPCC_CONFIG_T* pt_ipcc_config);






#endif/* _U_SBTL_IPCC_H_ */
#endif
/*----------------------------------------------------------------------------*/
/*! @} */   /* end of  groupMW_STRM_MNGR_MM_SBTL_HDLR*/
/*----------------------------------------------------------------------------*/




