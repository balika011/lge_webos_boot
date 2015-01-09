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
 * $RCSfile: evctx_slctr.h,v $
 * $Revision: #1 $
 * $Date: 2015/01/09 $
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_HQ_int/4 $
 * $SWAuthor: Clear Case Administrator $
 * $MD5HEX: 4d43a6d2b67a7d98c00a94263455ea99 $
 *
 * Description:
 *         This contains Event Context library macros, definitons, structures,
 *         and API's, which can be shared with other middleware modules, but 
 *         not exported to the 3rd parties.
 *---------------------------------------------------------------------------*/

#ifndef _EVCTX_SLCTR_H_
#define _EVCTX_SLCTR_H_

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include "x_evctx.h"
#include "x_evctx_eng.h"

/*----------------------------------------------------------------------------*/
/*! @addtogroup groupMW_EVCTX
 *  @{
 */
/*----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
/*#define PS_EVCTX_ENG_MSG_Q_PREFIX      "ECE_"*/




#define _EVCTX_ERROR                    "{EVCTX} ERR: "
#define _EVCTX_API                      "{EVCTX} "
#define _EVCTX_INFO                     "{EVCTX} "

/*-----------------------------------------------------------------------------
                    functions declarations
 ----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*! @brief	initialization the evctx
 *
 *    @param  [in] pt_evctx_thread_descr          	-pt_evctx_thread_descr References 
 *	@note	
 *
 *	@see
 *    @return		INT32
 *    @retval		EVCTXR_OK   - Success
 */
/*----------------------------------------------------------------------------*/
extern INT32 evctx_init (
                const THREAD_DESCR_T*   pt_evctx_thread_descr);

/*----------------------------------------------------------------------------*/
/*! @brief	delete thevctx
 *
 *	@note	
 *
 *	@see
 *    @return		INT32
 *    @retval		EVCTXR_OK   - Success
 */
/*----------------------------------------------------------------------------*/
extern INT32 evctx_exit (VOID);


/*----------------------------------------------------------------------------*/
/*! @brief	the initialziation status of EVCTX component
 *
 *	@note	
 *
 *	@see
 *    @return		BOOL
 *    @retval		TRUE        - The EVCTX component had been initialized.
 *         	 		FALSE       - The EVCTX component is not initialized.
 */
/*----------------------------------------------------------------------------*/
extern BOOL evctx_is_inited (VOID);

/*----------------------------------------------------------------------------*/
/*! @brief	return the handle of the message queue
 *
 *	@note	
 *
 *	@see
 *    @return		HANDLE_T
 *    @retval		return the handle of the message queue
 */
/*----------------------------------------------------------------------------*/

extern HANDLE_T evctx_get_msg_q_hdl (VOID);

/*----------------------------------------------------------------------------*/
/*! @brief	returns the current setting of debug level.
 *
 *	@note	
 *
 *	@see
 *    @return		UINT16
 *    @retval		the current setting of debug level.
 */
/*----------------------------------------------------------------------------*/

extern UINT16 evctx_get_dbg_level (VOID);

extern CHAR*  evctx_get_module_name(VOID);
/*----------------------------------------------------------------------------*/
/*! @brief	changes the setting of debug level to new one.
 *
 *    @param  [in] ui2_level     -new setting of debug level. 
 *	@note	
 *
 *	@see
 *    @return		BOOL
 *    @retval		TRUE        - Routine successful.
 *         	 		FALSE       - Routine failed.
 */
/*----------------------------------------------------------------------------*/

extern BOOL evctx_set_dbg_level (UINT16  ui2_level);

/*----------------------------------------------------------------------------*/
/*! @brief	the TS of evctx object is same or not.
 *
 *    @param  [in] pt_ts_desc     -the TS descriptor. 
 *	@note	
 *
 *	@see
 *    @return		BOOL
 *    @retval		TRUE        - same TS.
 *         	 		FALSE       - not same TS.
 */
/*----------------------------------------------------------------------------*/
extern BOOL evctx_is_same_ts (TS_DESCR_T* pt_ts_desc);

#endif /* _EVCTX_SLCTR_H_ */

/*----------------------------------------------------------------------------*/
/*! @} */                                             /* end of groupMW_EVCTX */
/*----------------------------------------------------------------------------*/

