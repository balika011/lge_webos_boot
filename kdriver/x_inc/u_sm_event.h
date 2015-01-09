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
/*! @file u_sm_event.h 
 *  $RCSfile: $
 *  $Revision: #1 $
 *  $Date: 2015/01/09 $
 *  $Author: dtvbm11 $
 *  
 *  @par Description:
 *         This header file contains Stream Manager specific event definitions,
 *         which are exported.
 */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*! @addtogroup groupMW_STRM_MNGR
 *  @{ 
 */
/*----------------------------------------------------------------------------*/

#ifndef _U_SM_EVENT_H_
#define _U_SM_EVENT_H_


/*-----------------------------------------------------------------------------
                    include files
-----------------------------------------------------------------------------*/
#include "u_common.h"

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
-----------------------------------------------------------------------------*/

/* event types ***************************************************************/
typedef UINT32 SM_EVN_TYPE_T;    /**<        */

#define SM_EVN_IGNORE             ((SM_EVN_TYPE_T) 0)    /**<        */

#define SM_EVN_GRP_BIT_MASK       ((SM_EVN_TYPE_T) 0xFFFF0000 )    /**<        */
#define SM_EVN_MAIN_GRP_BIT_MASK  ((SM_EVN_TYPE_T) 0xFF000000 )    /**<        */
#define SM_EVN_SUB_GRP_BIT_MASK   ((SM_EVN_TYPE_T) 0x00FF0000 )    /**<        */
#define SM_EVN_GRP_OPT_HDLR_BEGIN ((SM_EVN_TYPE_T) 0x80000000)    /**<        */

#define SM_MAKE_MAIN_EVN_GRP( id )     ((SM_EVN_TYPE_T) id << 24 )    /**<        */
#define SM_MAKE_MAIN_OPT_EVN_GRP( id )                                        \
    ((SM_EVN_TYPE_T) SM_EVN_GRP_OPT_HDLR_BEGIN + SM_MAKE_MAIN_EVN_GRP(id) )    /**<        */

#define SM_MAKE_EVN_GRP( main_grp, sub_grp_id )  ((SM_EVN_TYPE_T) main_grp | (sub_grp_id<<16) )    /**<        */
#define SM_MAKE_EVN( grp, evn_id )  ((SM_EVN_TYPE_T) (grp&SM_EVN_GRP_BIT_MASK) \
                                     | (evn_id&0x0000FFFF) )    /**<        */

#define SM_EVN_GET_GRP( evn )      ((SM_EVN_TYPE_T) evn & SM_EVN_GRP_BIT_MASK )    /**<        */
#define SM_EVN_GET_MAIN_GRP( evn ) ((SM_EVN_TYPE_T) evn & SM_EVN_MAIN_GRP_BIT_MASK )    /**<        */
#define SM_EVN_GET_SUB_GRP( evn )  ((SM_EVN_TYPE_T) evn & SM_EVN_SUB_GRP_BIT_MASK )    /**<        */

/* Mandatory Event Groups */
#define SM_EVN_GRP_SESS           SM_MAKE_MAIN_EVN_GRP( 1 )    /**<        */
#define SM_EVN_GRP_VS             SM_MAKE_MAIN_EVN_GRP( 2 )    /**<        */
#define SM_EVN_GRP_AS             SM_MAKE_MAIN_EVN_GRP( 3 )    /**<        */
#define SM_EVN_GRP_CA             SM_MAKE_MAIN_EVN_GRP( 4 )    /**<        */
#define SM_EVN_GRP_CCI            SM_MAKE_MAIN_EVN_GRP( 5 )    /**<        */

/* Optional Event Groups */
#define SM_EVN_GRP_CCS            SM_MAKE_MAIN_OPT_EVN_GRP( 1 )    /**<        */
#define SM_EVN_GRP_1394           SM_MAKE_MAIN_OPT_EVN_GRP( 2 )    /**<        */
#define SM_EVN_GRP_SBTL           SM_MAKE_MAIN_OPT_EVN_GRP( 3 )    /**<        */
#define SM_EVN_GRP_TTX            SM_MAKE_MAIN_OPT_EVN_GRP( 4 )    /**<        */
#define SM_EVN_GRP_BYPASS         SM_MAKE_MAIN_OPT_EVN_GRP( 5 )    /**<        */
#define SM_EVN_GRP_SESS_RCRD      SM_MAKE_MAIN_OPT_EVN_GRP( 6 )    /**<        */
#define SM_EVN_GRP_RSH            SM_MAKE_MAIN_OPT_EVN_GRP( 7 )    /**<        */

/* events */
#define SM_EVN_SESS_APP_OPEN_REQ         ((SM_EVN_TYPE_T) SM_EVN_GRP_SESS + 0)    /**<        */
#define SM_EVN_SESS_APP_PLAY_REQ         ((SM_EVN_TYPE_T) SM_EVN_GRP_SESS + 1)    /**<        */
#define SM_EVN_SESS_APP_STOP_REQ         ((SM_EVN_TYPE_T) SM_EVN_GRP_SESS + 2)    /**<        */
#define SM_EVN_SESS_APP_CLOSE_REQ        ((SM_EVN_TYPE_T) SM_EVN_GRP_SESS + 3)    /**<        */
#define SM_EVN_SESS_DRV_OPENED           ((SM_EVN_TYPE_T) SM_EVN_GRP_SESS + 4)    /**<        */
#define SM_EVN_SESS_DRV_PLAYED           ((SM_EVN_TYPE_T) SM_EVN_GRP_SESS + 5)    /**<        */
#define SM_EVN_SESS_DRV_STOPPED          ((SM_EVN_TYPE_T) SM_EVN_GRP_SESS + 6)    /**<        */
#define SM_EVN_SESS_DRV_CLOSED           ((SM_EVN_TYPE_T) SM_EVN_GRP_SESS + 7)    /**<        */
#define SM_EVN_SESS_DRV_ERR              ((SM_EVN_TYPE_T) SM_EVN_GRP_SESS + 8)    /**<        */
#define SM_EVN_SESS_SCDB_CLOSED          ((SM_EVN_TYPE_T) SM_EVN_GRP_SESS + 9)    /**<        */
#define SM_EVN_SESS_SCDB_UPDATE          ((SM_EVN_TYPE_T) SM_EVN_GRP_SESS + 10)    /**<        */
#define SM_EVN_SESS_INTERNAL_ERR         ((SM_EVN_TYPE_T) SM_EVN_GRP_SESS + 11)    /**<        */

#define SM_EVN_VS_APP_OPEN_REQ           ((SM_EVN_TYPE_T) SM_EVN_GRP_VS + 0)    /**<        */
#define SM_EVN_VS_APP_PLAY_REQ           ((SM_EVN_TYPE_T) SM_EVN_GRP_VS + 1)    /**<        */
#define SM_EVN_VS_APP_STOP_REQ           ((SM_EVN_TYPE_T) SM_EVN_GRP_VS + 2)    /**<        */
#define SM_EVN_VS_APP_CLOSE_REQ          ((SM_EVN_TYPE_T) SM_EVN_GRP_VS + 3)    /**<        */
#define SM_EVN_VS_DRV_OPENED             ((SM_EVN_TYPE_T) SM_EVN_GRP_VS + 4)    /**<        */
#define SM_EVN_VS_DRV_PLAYED             ((SM_EVN_TYPE_T) SM_EVN_GRP_VS + 5)    /**<        */
#define SM_EVN_VS_DRV_STOPPED            ((SM_EVN_TYPE_T) SM_EVN_GRP_VS + 6)    /**<        */
#define SM_EVN_VS_DRV_CLOSED             ((SM_EVN_TYPE_T) SM_EVN_GRP_VS + 7)    /**<        */
#define SM_EVN_VS_DRV_ERR                ((SM_EVN_TYPE_T) SM_EVN_GRP_VS + 8)    /**<        */
#define SM_EVN_VS_SCDB_RECORD_DEL        ((SM_EVN_TYPE_T) SM_EVN_GRP_VS + 9)    /**<        */
#define SM_EVN_VS_SCDB_RECORD_MOD        ((SM_EVN_TYPE_T) SM_EVN_GRP_VS + 10)    /**<        */
#define SM_EVN_VS_INTERNAL_ERR           ((SM_EVN_TYPE_T) SM_EVN_GRP_VS + 11)    /**<        */
#define SM_EVN_VS_VIDEO_RESOLUTION_CHG   ((SM_EVN_TYPE_T) SM_EVN_GRP_VS + 12)    /**<        */
#define SM_EVN_VS_AUTO_CLK_POS_PHS_DONE  ((SM_EVN_TYPE_T) SM_EVN_GRP_VS + 13)    /**<        */
#define SM_EVN_VS_AUTO_COLOR_DONE        ((SM_EVN_TYPE_T) SM_EVN_GRP_VS + 14)    /**<        */
#define SM_EVN_VS_ANA_TV_SIGNAL          ((SM_EVN_TYPE_T) SM_EVN_GRP_VS + 15)    /**<        */
#define SM_EVN_VS_ANA_COLOR_SYSTEM       ((SM_EVN_TYPE_T) SM_EVN_GRP_VS + 16)    /**<        */
#define SM_EVN_VS_ANA_CONTENT_ADV_INFO   ((SM_EVN_TYPE_T) SM_EVN_GRP_VS + 17)    /**<        */
#define SM_EVN_VS_CC_INDICATOR           ((SM_EVN_TYPE_T) SM_EVN_GRP_VS + 18)    /**<        */
#define SM_EVN_VS_ANA_TV_SIGNAL_DETECTING ((SM_EVN_TYPE_T) SM_EVN_GRP_VS + 19)    /**<        */
#define SM_EVN_VS_DIG_DECODE_STATUS_CHG  ((SM_EVN_TYPE_T) SM_EVN_GRP_VS + 20)    /**<        */
#define SM_EVN_VS_SPEED_CHG              ((SM_EVN_TYPE_T) SM_EVN_GRP_VS + 21)    /**<        */
#define SM_EVN_VS_EOS                    ((SM_EVN_TYPE_T) SM_EVN_GRP_VS + 22)    /**<        */
#define SM_EVN_VS_RESTART                ((SM_EVN_TYPE_T) SM_EVN_GRP_VS + 23)    /**<        */
#define SM_EVN_VS_CODEC_NOT_SUPPORT      ((SM_EVN_TYPE_T) SM_EVN_GRP_VS + 24)    /**<        */
#define SM_EVN_VS_THUMBNAIL_DONE         ((SM_EVN_TYPE_T) SM_EVN_GRP_VS + 25)    /**<        */
#define SM_EVN_VS_HDCP_CONTENT           ((SM_EVN_TYPE_T) SM_EVN_GRP_VS + 26)    /**<        */
#define SM_EVN_VS_MACROVISION            ((SM_EVN_TYPE_T) SM_EVN_GRP_VS + 27)    /**<        */
#define SM_EVN_RANGE_AB_DONE             ((SM_EVN_TYPE_T) SM_EVN_GRP_VS + 28)    /*     */
#define SM_EVN_VS_DISP_FRAME             ((SM_EVN_TYPE_T) SM_EVN_GRP_VS + 29)    /**<        */
#define SM_EVN_VS_BIT_RATE               ((SM_EVN_TYPE_T) SM_EVN_GRP_VS + 30)    /**<        */
#define SM_EVN_VS_VIDEO_PLAY_DONE        ((SM_EVN_TYPE_T) SM_EVN_GRP_VS + 31)    /**<        */
#define SM_EVN_VS_FRAMERATE_NOT_SUPPORT  ((SM_EVN_TYPE_T) SM_EVN_GRP_VS + 32)    /**<        */
#define SM_EVN_VS_CHAP_DONE              ((SM_EVN_TYPE_T) SM_EVN_GRP_VS + 33)    /* divx chapter done */
#define SM_EVN_VS_AUD_PROTECT_INFO       ((SM_EVN_TYPE_T) SM_EVN_GRP_VS + 34)    /**<        */
#define SM_EVN_VS_REQ_RESTART            ((SM_EVN_TYPE_T) SM_EVN_GRP_VS + 35)    /**<        */
#define SM_EVN_VS_VDP_BIT_RATE_CHG       ((SM_EVN_TYPE_T) SM_EVN_GRP_VS + 36)    /**<        */
#define SM_EVN_VS_THUMBNAIL_PLAY_EOS     ((SM_EVN_TYPE_T) SM_EVN_GRP_VS + 37)    /**<        */
#define SM_EVN_VS_UHD_420_SIG_CHG        ((SM_EVN_TYPE_T) SM_EVN_GRP_VS + 38)    /**<        */
#define SM_EVN_VS_WFD_ERROR              ((SM_EVN_TYPE_T) SM_EVN_GRP_VS + 39)    /**<        */
#define SM_EVN_VS_VDP_MUTE_ALL_FINISHED	 ((SM_EVN_TYPE_T) SM_EVN_GRP_VS + 40)    /**<        */
#define SM_EVN_VS_MJPEG_IMGRSZ_RELEASE	 ((SM_EVN_TYPE_T) SM_EVN_GRP_VS + 41)    /**<        */


#define SM_EVN_AS_APP_OPEN_REQ           ((SM_EVN_TYPE_T) SM_EVN_GRP_AS + 0)    /**<        */
#define SM_EVN_AS_APP_PLAY_REQ           ((SM_EVN_TYPE_T) SM_EVN_GRP_AS + 1)    /**<        */
#define SM_EVN_AS_APP_STOP_REQ           ((SM_EVN_TYPE_T) SM_EVN_GRP_AS + 2)    /**<        */
#define SM_EVN_AS_APP_CLOSE_REQ          ((SM_EVN_TYPE_T) SM_EVN_GRP_AS + 3)    /**<        */
#define SM_EVN_AS_DRV_OPENED             ((SM_EVN_TYPE_T) SM_EVN_GRP_AS + 4)    /**<        */
#define SM_EVN_AS_DRV_PLAYED             ((SM_EVN_TYPE_T) SM_EVN_GRP_AS + 5)    /**<        */
#define SM_EVN_AS_DRV_STOPPED            ((SM_EVN_TYPE_T) SM_EVN_GRP_AS + 6)    /**<        */
#define SM_EVN_AS_DRV_CLOSED             ((SM_EVN_TYPE_T) SM_EVN_GRP_AS + 7)    /**<        */
#define SM_EVN_AS_DRV_ERR                ((SM_EVN_TYPE_T) SM_EVN_GRP_AS + 8)    /**<        */
#define SM_EVN_AS_SCDB_RECORD_DEL        ((SM_EVN_TYPE_T) SM_EVN_GRP_AS + 9)    /**<        */
#define SM_EVN_AS_SCDB_RECORD_MOD        ((SM_EVN_TYPE_T) SM_EVN_GRP_AS + 10)    /**<        */
#define SM_EVN_AS_INTERNAL_ERR           ((SM_EVN_TYPE_T) SM_EVN_GRP_AS + 11)    /**<        */
#define SM_EVN_AS_AUD_INFO_CHG           ((SM_EVN_TYPE_T) SM_EVN_GRP_AS + 12)    /**<        */
#define SM_EVN_AS_RESTART                ((SM_EVN_TYPE_T) SM_EVN_GRP_AS + 13)    /**<        */
#define SM_EVN_AS_DECODE_STATUS_CHG      ((SM_EVN_TYPE_T) SM_EVN_GRP_AS + 14)    /**<        */
#define SM_EVN_AS_EOS                    ((SM_EVN_TYPE_T) SM_EVN_GRP_AS + 15)    /**<        */
#define SM_EVN_AS_CODEC_NOT_SUPPORT      ((SM_EVN_TYPE_T) SM_EVN_GRP_AS + 16)    /**<        */
#define SM_EVN_AS_SIG_MONITOR_TM         ((SM_EVN_TYPE_T) SM_EVN_GRP_AS + 17)    /**<        */
#define SM_EVN_AS_SIG_MONITOR_AUTO_MUTE         ((SM_EVN_TYPE_T) SM_EVN_GRP_AS + 18)    /**<        */
#define SM_EVN_AS_TV_AUD_SYS_DETECTED         ((SM_EVN_TYPE_T) SM_EVN_GRP_AS + 19)    /**<        */
#define SM_EVN_AS_REQ_RESTART         ((SM_EVN_TYPE_T) SM_EVN_GRP_AS + 20)    /**<        */
#define SM_EVN_AS_CODEC_CHG           ((SM_EVN_TYPE_T) SM_EVN_GRP_AS + 21)    /**<        */
#define SM_EVN_AS_BIT_RATE_CHG         ((SM_EVN_TYPE_T) SM_EVN_GRP_AS + 22)    /**<        */

#define SM_EVN_CA_COND_CLOSED            ((SM_EVN_TYPE_T) SM_EVN_GRP_CA + 0)    /**<        */
#define SM_EVN_CA_COND_UNKNOWN           ((SM_EVN_TYPE_T) SM_EVN_GRP_CA + 1)    /**<        */
#define SM_EVN_CA_COND_FREE              ((SM_EVN_TYPE_T) SM_EVN_GRP_CA + 2)    /**<        */
#define SM_EVN_CA_COND_AUTHORIZED        ((SM_EVN_TYPE_T) SM_EVN_GRP_CA + 3)    /**<        */
#define SM_EVN_CA_COND_AUTHORIZED_CA_SYS_ID ((SM_EVN_TYPE_T) SM_EVN_GRP_CA + 4)    /**<        */
#define SM_EVN_CA_COND_NOT_AUTHORIZED    ((SM_EVN_TYPE_T) SM_EVN_GRP_CA + 5)    /**<        */

#define SM_EVN_CCI_INFORMATION           ((SM_EVN_TYPE_T) SM_EVN_GRP_CCI + 0)    /**<        */


#endif /* _U_SM_EVENT_H_ */
/*----------------------------------------------------------------------------*/
/*! @} */                                             /* end of groupMW_STRM_MNGR */
/*----------------------------------------------------------------------------*/

