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
/*! @file x_wgl_sbtl.h 
 *  $RCSfile: $
 *  $Revision: #1 $
 *  $Date: 2015/01/09 $
 *  $Author: dtvbm11 $
 *  
 *  @par Description:
 *         This header file contains public interfaces, constant and type 
 *         definitions of the Widget Library.
 */
/*----------------------------------------------------------------------------*/

#ifndef _X_WGL_SBTL_H_
#define _X_WGL_SBTL_H_

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include "u_wgl_sbtl.h"

/*----------------------------------------------------------------------------*/
/*! @addtogroup groupMW_WGL_SBTL
*  @{  
*/
/*----------------------------------------------------------------------------*/


#ifdef __cplusplus
extern "C" {
#endif

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/

#define WGL_SBTL_MAX_PALETTE_COLOR_NUM      64    /**< the subtitle palette size */


/*-----------------------------------------------------------------------------
                    structures
 ----------------------------------------------------------------------------*/
/* system init structure */
#define WGL_SBTL_SYS_INIT_FLAG_SOFTWARE_SCALER      MAKE_BIT_MASK_32(0)    /**< flag to enable software scaler */
#define WGL_SBTL_SYS_INIT_FLAG_PALETTE_MODE         MAKE_BIT_MASK_32(1)    /**< flag to make subtitle widget work in palette mode */
/*------------------------------------------------------------------*/
/*! @struct WGL_SBTL_SYS_INIT_T
 *  @brief  This structure contains the configuration data for subtitle stream widget
 *  @code
 *  typedef struct _WGL_SBTL_SYS_INIT_T
 *  {
 *      UINT32                      ui4_flag;
 *      GL_COLORMODE_T              e_clr_mode_frm_buf;
 *      UINT8                       ui1_cnvs_plt_sbtl_start;
 *      UINT8                       ui1_cnvs_plt_sbtl_end;
 *  } WGL_SBTL_SYS_INIT_T;
 *  @endcode
 *  @li@c  ui4_flag                                          - initialization flags
 *  @li@c  e_clr_mode_frm_buf                                - color mode
 *  @li@c  ui1_cnvs_plt_sbtl_start                           - start position in palette
 *  @li@c  ui1_cnvs_plt_sbtl_end                             - end position in palette
 */
/*------------------------------------------------------------------*/
typedef struct _WGL_SBTL_SYS_INIT_T
{
    UINT32                      ui4_flag;
    GL_COLORMODE_T              e_clr_mode_frm_buf;
    
    /* canvas palette entry range for subtitle - start */
    /* valid only when WGL_SBTL_SYS_INIT_FLAG_PALETTE_MODE is ON */
    UINT8                       ui1_cnvs_plt_sbtl_start;
    
    /* canvas palette entry range for subtitle - end */
    /* valid only when WGL_SBTL_SYS_INIT_FLAG_PALETTE_MODE is ON */
    /* (ui1_cnvs_plt_sbtl_end - ui1_cnvs_plt_sbtl_start + 1) must be WGL_SBTL_MAX_PALETTE_COLOR_NUM */
    UINT8                       ui1_cnvs_plt_sbtl_end;
    
} WGL_SBTL_SYS_INIT_T;

 

/*-----------------------------------------------------------------------------
                    functions
 ----------------------------------------------------------------------------*/
/*------------------------------------------------------------------*/
/*! @brief Initialize the DVB subtitle widget configuration
 *  @param [in]     pt_sys_init                 - Configuration context
 *  @param [out]
 *  @note
 *  @see
 *  @return     Return the error code.
 *  @retval      WGLR_OK                - The routine was successful.
 *  @retval      WGLR_INV_ARG           - The routine was fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_wgl_sbtl_init(
    WGL_SBTL_SYS_INIT_T                 *pt_sys_init);


#ifdef __cplusplus
}
#endif


#endif

/*----------------------------------------------------------------------------*/
/*! @addtogroup groupMW_WGL_SBTL
*  @{  
*/
/*----------------------------------------------------------------------------*/


