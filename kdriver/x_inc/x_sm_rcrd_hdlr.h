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
/*! @file x_sm_rcrd_hdlr.h 
 *  $RCSfile: $
 *  $Revision: #1 $
 *  $Date: 2015/01/09 $
 *  $Author: dtvbm11 $
 *  
 *  @par Description:
 *         This header file contains Stream Manager's record stream exported  
 *         definitions.
 */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*! @defgroup groupMW_STRM_MNGR_RCRD_STRM_HDLR Record Stream Handler
 *
 *  @ingroup groupMW_STRM_MNGR
 *  @brief Provides record stream external functions for TV timeshift or simple
 *   PVR Application
 *
 *  %MW Application API would be used by TV Application, lot of these APIs would
 *   be wrapped from Exported API
 *
 *  @see groupMW_STRM_MNGR
 *  @{ 
 */
/*----------------------------------------------------------------------------*/
#ifndef _X_SM_RCRD_HDLR_H_
#define _X_SM_RCRD_HDLR_H_

/*-----------------------------------------------------------------------------
                    include files
-----------------------------------------------------------------------------*/
#include "u_sm_rcrd_hdlr.h"
#include "u_sm_event.h"


/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
-----------------------------------------------------------------------------*/

#define SM_RCRD_STRM_HDLR_NAME      "record_strm_hdlr"
#define SM_RCRD_UTIL_NAME_LEN       SYS_NAME_LEN


/*Event main subgroups: APP, PVR STRM, PM Record Utilty*/
/*The first sub group 0 is for APP which defined in u_sm_rcrd_hdlr.h */
#define SM_EVN_RSH_GRP_PVR   SM_MAKE_EVN_GRP(SM_EVN_GRP_RSH, 1)
#define SM_EVN_RSH_GRP_PM    SM_MAKE_EVN_GRP(SM_EVN_GRP_RSH, 2)
#define SM_EVN_RSH_GRP_RUTI  SM_MAKE_EVN_GRP(SM_EVN_GRP_RSH, 3)
#define SM_EVN_RSH_GRP_TM    SM_MAKE_EVN_GRP(SM_EVN_GRP_RSH, 4)


/*PVR events state machine: 
    |-------------------------------------------------
    |        ----------------------------            |
    |        |                           |           |
    |        |                           V           V
 OPENED->CONNECTED->PLAYED<->STOPPED->DISCONNECTED->CLOSED
    |      |          |       |        ^             |
    |      V          V       V        |             |
    |----->             Error          ------------->
    Note: 1)CONNECTED to record demux
          2)Played means enable PVR stream,Stopped means disable enable
          3)PVR stream default is disable
*/
#define SM_EVN_RSH_PVR_OPENED         SM_MAKE_EVN(SM_EVN_RSH_GRP_PVR, 0)
#define SM_EVN_RSH_PVR_PLAYED         SM_MAKE_EVN(SM_EVN_RSH_GRP_PVR, 1)
#define SM_EVN_RSH_PVR_STOPPED        SM_MAKE_EVN(SM_EVN_RSH_GRP_PVR, 2)
#define SM_EVN_RSH_PVR_CONNECTED      SM_MAKE_EVN(SM_EVN_RSH_GRP_PVR, 3)
#define SM_EVN_RSH_PVR_DISCONNECTED   SM_MAKE_EVN(SM_EVN_RSH_GRP_PVR, 4)
#define SM_EVN_RSH_PVR_CLOSED         SM_MAKE_EVN(SM_EVN_RSH_GRP_PVR, 5)
#define SM_EVN_RSH_PVR_ERR            SM_MAKE_EVN(SM_EVN_RSH_GRP_PVR, 6)


/*PM events state machine:
    |-----------------|
    |                 V
 STOPPED<->PLAYED->CLOSED
    |      |          ^
    |      V          |
    |----->  Error --->
    Note: State is acording to PM APIs
*/
/*#define SM_EVN_RSH_PM_COND_OPENED     SM_MAKE_EVN(SM_EVN_RSH_GRP_PM, 0)*/
#define SM_EVN_RSH_PM_COND_PLAYED     SM_MAKE_EVN(SM_EVN_RSH_GRP_PM, 1)
#define SM_EVN_RSH_PM_COND_STOPPED    SM_MAKE_EVN(SM_EVN_RSH_GRP_PM, 2)
#define SM_EVN_RSH_PM_COND_CLOSED     SM_MAKE_EVN(SM_EVN_RSH_GRP_PM, 3)
#define SM_EVN_RSH_PM_COND_ERR        SM_MAKE_EVN(SM_EVN_RSH_GRP_PM, 4)


/*Record utility  events state machine: 
    |------------------------|
    |                        V
 OPENED->CLOSED
    |       ^
    V       |
      Error 
*/
#define SM_EVN_RSH_RUTI_OPENED         SM_MAKE_EVN(SM_EVN_RSH_GRP_RUTI, 0)
#define SM_EVN_RSH_RUTI_READY          SM_MAKE_EVN(SM_EVN_RSH_GRP_RUTI, 1)
#define SM_EVN_RSH_RUTI_TRANSITION     SM_MAKE_EVN(SM_EVN_RSH_GRP_RUTI, 2)
#define SM_EVN_RSH_RUTI_CLOSED         SM_MAKE_EVN(SM_EVN_RSH_GRP_RUTI, 3)
#define SM_EVN_RSH_RUTI_ERR            SM_MAKE_EVN(SM_EVN_RSH_GRP_RUTI, 4)

#define SM_EVN_RSH_TM_PAT_SWAPED   SM_MAKE_EVN(SM_EVN_RSH_GRP_TM, 0)
#define SM_EVN_RSH_TM_PMT_SWAPED   SM_MAKE_EVN(SM_EVN_RSH_GRP_TM, 1)


/*-----------------------------------------------------------------------------
                    Functions declarations
-----------------------------------------------------------------------------*/
/*------------------------------------------------------------------*/
/*! @brief
 *  @param [in]
 *  @param [out]
 *  @note
 *  @see
 *  @return
 *  @retval      -
 */
/*------------------------------------------------------------------*/
extern INT32 x_sm_rcrd_strm_hldr_init(VOID);

#endif /*end of _X_SM_RCRD_HDLR_H_ */
/*----------------------------------------------------------------------------*/
/*! @} */                                              /* end of groupMW_STRM_MNGR_RCRD_STRM_HDLR */
/*----------------------------------------------------------------------------*/



