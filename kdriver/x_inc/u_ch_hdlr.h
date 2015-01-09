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
/*! @file u_ch_hdlr.h 
 *  $File: //DTV/RELEASE_BR/r_DTV_X_IDTV1401_002306_12_001_37_001/boot/kdriver/x_inc/u_ch_hdlr.h $
 *  $Revision: #1 $
 *  $Date: 2015/01/09 $
 *  $Author: dtvbm11 $
 *  
 *  @par Description:
 *
 */
/*----------------------------------------------------------------------------*/
#ifndef _U_CH_HDLR_H_
#define _U_CH_HDLR_H_
/*-----------------------------------------------------------------------------
 *  include files
 *---------------------------------------------------------------------------*/
#include "u_common.h"
#include "u_tuner.h"
#include "u_sm.h"
#include "u_sm_cc_hdlr.h"
#include "u_analog_brdcst.h"

/*----------------------------------------------------------------------------*/
/*! @defgroup groupMW_SVCTX_CH_HDLR Channel Handler
 *
 *  @ingroup groupMW_SVCTX
 *  @brief For user change channel.
 *
 *  %MW Application API would be used by Application, lot of these APIs would
 *   be wrapped from Exported API. Using these APIs, application could change channel.
 *
 *  @see groupMW_SVCTX
 *  @{ 
 */
/*----------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
 * structure, constants, macro definitions
 *---------------------------------------------------------------------------*/

#define SVCTX_GRP_CH                                      (2<<SVCTX_TYPE_CODE_BITS)    /**<  channel group mask */
/* channel handler set types *********************************************************/
#define SVCTX_CH_SET_TYPE_IGNORE                          ((SVCTX_SET_TYPE_T)(SVCTX_GRP_CH | 0))    /**< skip this set type */
/* will not do any connection and treat such SVL record as no signal for client */
#define SVCTX_CH_SET_INVALID_SVL_REC_NW_MASK_BIT          ((SVCTX_SET_TYPE_T)(SVCTX_GRP_CH | 1))    /**< use invalid svl rec type */
#define SVCTX_CH_SET_AUD_AV_SYNC_MODE                     ((SVCTX_SET_TYPE_T)(SVCTX_GRP_CH | 2))    /**< use av sync mode type */
#define SVCTX_CH_SET_VID_I_FRAME_MODE                     ((SVCTX_SET_TYPE_T)(SVCTX_GRP_CH | 3))    /**< use video I fream type */
#define SVCTX_CH_SET_FREQ_LIST_UPDATED_CHK                ((SVCTX_SET_TYPE_T)(SVCTX_GRP_CH | 4))    /**< request to check frequency list update for RiksTV */
#define SVCTX_CH_SET_RECOVER_FROM_ALTER_TUNE              ((SVCTX_SET_TYPE_T)(SVCTX_GRP_CH | 5))    /**< request to recover from alternative freq tuning for RiksTV */
#define SVCTX_CH_SET_DELIVERY_SYSTEM_CHANGE               ((SVCTX_SET_TYPE_T)(SVCTX_GRP_CH | 6))    /**< request to notify to AP when delivery system change */
#define SVCTX_CH_SET_IS_MHEG_ACTIVE                       ((SVCTX_SET_TYPE_T)(SVCTX_GRP_CH | 7))    /**< MHEG5 set to notify svctx if it is active */

/* channel handler get types *********************************************************/
#define SVCTX_CH_GET_TYPE_IGNORE                          ((SVCTX_GET_TYPE_T)(SVCTX_GRP_CH | 0))    /**< skip this get type */
#define SVCTX_CH_GET_TYPE_CH_SRC                          ((SVCTX_GET_TYPE_T)(SVCTX_GRP_CH | 1)) /**< type = CH_SRC_TYPE_T*   size = sizeof(CH_SRC_TYPE_T)   */
#define SVCTX_CH_GET_SILENT_STATUS                        ((SVCTX_GET_TYPE_T)(SVCTX_GRP_CH | 2)) /**< type = BOOL*            size = sizeof(BOOL)            */
#define SVCTX_CH_GET_SILENT_SVC_REC_ID                    ((SVCTX_GET_TYPE_T)(SVCTX_GRP_CH | 3)) /**< type = UINT16*          size = sizeof(UINT16)          */
#define SVCTX_CH_GET_AUX_AUDIO_STATUS                     ((SVCTX_GET_TYPE_T)(SVCTX_GRP_CH | 4)) /**< type = SM_COND_T*       size = sizeof(SM_COND_T)       */
#define SVCTX_CH_GET_BRDCST_TYPE                          ((SVCTX_GET_TYPE_T)(SVCTX_GRP_CH | 5)) /**< type = BRDCST_TYPE_T*   size = sizeof(BRDCST_TYPE_T)   */
#define SVCTX_CH_GET_SRC_SVL_INFO                         ((SVCTX_GET_TYPE_T)(SVCTX_GRP_CH | 6)) /**< type = SRC_SVL_INFO_T*  size = sizeof(SRC_SVL_INFO_T)  */
#define SVCTX_CH_GET_SRC_FREQ_INFO                        ((SVCTX_GET_TYPE_T)(SVCTX_GRP_CH | 7)) /**< type = SRC_FREQ_INFO_T* size = sizeof(SRC_FREQ_INFO_T) */
#define SVCTX_CH_GET_FIRST_CH_SURF                        ((SVCTX_GET_TYPE_T)(SVCTX_GRP_CH | 8)) /**< type = BOOL*   size = sizeof(BOOL) */
#define SVCTX_CH_GET_NON_BRDCSTING_SVC                    ((SVCTX_GET_TYPE_T)(SVCTX_GRP_CH | 9)) /**< type = BOOL*   size = sizeof(BOOL) */
/*Notes : the following just for SYS_MHP_SUPPORT */
#define SVCTX_CH_GET_CUR_STC                              ((SVCTX_GET_TYPE_T)(SVCTX_GRP_CH | 10)) /**< type = STC_T*   size = sizeof(STC_T) */ /* cur_STC& svl_ST_ID just SYS_MHP_SUPPORT*/
#define SVCTX_CH_GET_SILENT_SVL_LST_ID                    ((SVCTX_GET_TYPE_T)(SVCTX_GRP_CH | 11)) /**< type = UINT16*  size = sizeof(UINT16)  */
#define SVCTX_CH_GET_CURRENT_TS_ID                        ((SVCTX_GET_TYPE_T)(SVCTX_GRP_CH | 12)) /**< type = UINT16*  size = sizeof(UINT16)  */ /* just SYS_MHP_SUPPORT */
#define SVCTX_CH_GET_AUD_ORIG_SCRAMBLE_STATUS             ((SVCTX_GET_TYPE_T)(SVCTX_GRP_CH | 13)) /**< type = BOOL*    size = sizeof(BOOL)  */
#define SVCTX_CH_GET_VID_ORIG_SCRAMBLE_STATUS             ((SVCTX_GET_TYPE_T)(SVCTX_GRP_CH | 14)) /**< type = BOOL*    size = sizeof(BOOL)  */
#define SVCTX_CH_GET_TUNER_SERIAL_NO                      ((SVCTX_GET_TYPE_T)(SVCTX_GRP_CH | 15)) /**< type = UINT32*    size = sizeof(UINT32)  */

/*------------------------------------------------------------------*/
/*! @enum   SVCTX_AV_SYNC_MODE_T
 *  @brief  av sync type
 *  @code
 *  typedef enum
 *  {
 *      SVCTX_AV_SYNC_FREE_RUN = 0, 
 *      SVCTX_AV_SYNC_SLAVE,        
 *      SVCTX_AV_SYNC_MASTER        
 *  }   SVCTX_AV_SYNC_MODE_T;
 *  @endcode
 *  @li@c  SVCTX_AV_SYNC_FREE_RUN                        SVCTX_AV_SYNC_FREE_RUN- 
 *                                                                                  don't care av sync    
 *  @li@c  SVCTX_AV_SYNC_SLAVE                               SVCTX_AV_SYNC_SLAVE- 
 *                                                                                  slave sync av
 *  @li@c  SVCTX_AV_SYNC_MASTER                             SVCTX_AV_SYNC_MASTER- 
 *                                                                                   master sync av
 */
/*------------------------------------------------------------------*/
typedef enum
{
    SVCTX_AV_SYNC_FREE_RUN = 0, /* No need to sync */
    SVCTX_AV_SYNC_SLAVE,        /* Sync to STC */
    SVCTX_AV_SYNC_MASTER        /* Update A-PTS to A-STC, may use in PCR or HardDisk playback */
}   SVCTX_AV_SYNC_MODE_T;

#endif /* _U_CH_HDLR_H_ */

/*----------------------------------------------------------------------------*/
/*! @} */                                           /* end of groupMW_SVCTX_CH_HDLR */
/*----------------------------------------------------------------------------*/

