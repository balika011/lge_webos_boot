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
/*! @file u_sm_bypass_hdlr.h 
 *  $RCSfile: $
 *  $Revision: #1 $
 *  $Date: 2015/01/09 $
 *  $Author: dtvbm11 $
 *  
 *  @par Description:
 *         This header file contains Bypass Stream handler specific 
 *         definitions, which are exported to all.
 */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*! @defgroup groupMW_STRM_MNGR_BYPASS_HDLR Bypass Handler
 *
 *  @ingroup groupMW_STRM_MNGR
 *  @brief Provides video bypass stream external function for TV Application
 *
 *  %MW Application API would be used by TV Application, lot of these APIs would
 *   be wrapped from Exported API
 *
 *  @see groupMW_STRM_MNGR
 *  @{ 
 */
/*----------------------------------------------------------------------------*/

#ifndef _U_SM_BYPASS_HDLR_H_
#define _U_SM_BYPASS_HDLR_H_


/*-----------------------------------------------------------------------------
                    include files
-----------------------------------------------------------------------------*/

#include "u_common.h"
#include "u_handle.h"
#include "u_sm.h"
#include "u_sm_video_hdlr.h"

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
-----------------------------------------------------------------------------*/

/* command sets *************************************************************/
/* primary commands */
#define SM_BYPASS_CMD_CODE_SET_DEST_GRP_NAME                    \
        ((SM_CMD_CODE_T)(SM_CMD_CODE_PRIMARY_BIT_FLAG | 0))    /**<        */
#define SM_BYPASS_CMD_CODE_SET_VBIF_SRC                         \
        ((SM_CMD_CODE_T)(SM_CMD_CODE_PRIMARY_BIT_FLAG | 1))    /**<        */
#define SM_BYPASS_CMD_CODE_SET_FORCE_PID                        \
        ((SM_CMD_CODE_T)(SM_CMD_CODE_PRIMARY_BIT_FLAG | 2))    /**<        */
#define SM_BYPASS_CMD_CODE_SET_FORCE_CODEC                      \
        ((SM_CMD_CODE_T)(SM_CMD_CODE_PRIMARY_BIT_FLAG | 3))    /**<        */
#define SM_BYPASS_CMD_CODE_SET_FORCE_BLANK                      \
        ((SM_CMD_CODE_T)(SM_CMD_CODE_PRIMARY_BIT_FLAG | 4))    /**<        */

#define SM_BYPASS_CMD_CODE_SET_FORCE_NO_CA_REQ                        \
        ((SM_CMD_CODE_T)(SM_CMD_CODE_PRIMARY_BIT_FLAG | 5))    /**<        */

#define SM_GRP_BSH    (( SM_GRP_LAST_ENTRY + 14 ) << SM_CODE_BITS)    /**<        */

/* set types *****************************************************************/
#define SM_BSH_SET_TYPE_IGNORE               ((SM_SET_TYPE_T)(SM_GRP_BSH | 0))    /**<        */
#define SM_BSH_SET_TYPE_VBIF_SRC             ((SM_SET_TYPE_T)(SM_GRP_BSH | 1))    /**<        */
#define SM_BSH_SET_TYPE_VBIF_CTRL            ((SM_SET_TYPE_T)(SM_GRP_BSH | 2))    /**<        */
#define SM_BSH_SET_TYPE_VIDEO_OUT_CTRL       ((SM_SET_TYPE_T)(SM_GRP_BSH | 3))    /**<        */
#define SM_BSH_SET_TYPE_TV_ENC_CTRL          ((SM_SET_TYPE_T)(SM_GRP_BSH | 4))    /**<        */

/* get types *****************************************************************/
#define SM_BSH_GET_TYPE_IGNORE               ((SM_GET_TYPE_T)(SM_GRP_BSH | 0))    /**<        */
#define SM_BSH_GET_TYPE_VIDEO_SRC_RESOLUTION ((SM_GET_TYPE_T)(SM_GRP_BSH | 1))    /**<        */
#define SM_BSH_GET_TYPE_VIDEO_OUT_CTRL       ((SM_GET_TYPE_T)(SM_GRP_BSH | 2))    /**<        */
#define SM_BSH_GET_TYPE_TV_ENC_CTRL          ((SM_GET_TYPE_T)(SM_GRP_BSH | 3))    /**<        */


/* SM_BSH_SET_TYPE_VBIF_SRC settings ****************************************/
/*------------------------------------------------------------------*/
/*! @enum BSH_VBIF_SRC_TYPE_T
 *  @brief  Specify VBIF source type 
 *  @code
 *  typedef enum _BSH_VBIF_SRC_TYPE_T
 *  {
 *      BSH_VBIF_SRC_TYPE_UNKNOWN = 0,
 *      BSH_VBIF_SRC_TYPE_DIG_VBI_ES,
 *      BSH_VBIF_SRC_TYPE_DIG_TTX_ES
 *  }   BSH_VBIF_SRC_TYPE_T;   
 *  @endcode
 *  @li@c  BSH_VBIF_SRC_TYPE_UNKNOWN                         -VBIF source type unknown
 *  @li@c  BSH_VBIF_SRC_TYPE_DIG_VBI_ES                      -VBIF source type digital VBI ES
 *  @li@c  BSH_VBIF_SRC_TYPE_DIG_TTX_ES                      -VBIF source type digital TTX ES
 */
/*------------------------------------------------------------------*/
typedef enum _BSH_VBIF_SRC_TYPE_T
{
    BSH_VBIF_SRC_TYPE_UNKNOWN = 0,
    BSH_VBIF_SRC_TYPE_DIG_VBI_ES,
    BSH_VBIF_SRC_TYPE_DIG_TTX_ES
}   BSH_VBIF_SRC_TYPE_T;   

/*------------------------------------------------------------------*/
/*! @struct BSH_VBIF_SRC_INFO_T
 *  @brief  Contain VBIF source info
 *  @code
 *  typedef struct _BSH_VBIF_SRC_INFO_T
 *  {
 *      BSH_VBIF_SRC_TYPE_T    e_src_type;
 *      UINT16                 ui2_pid;
 *  }   BSH_VBIF_SRC_INFO_T;
 *  @endcode
 *  @li@c  e_src_type                                     -VBIF source type 
 *  @li@c  ui2_pid                                           -VBIF source PID value
 */
/*------------------------------------------------------------------*/
typedef struct _BSH_VBIF_SRC_INFO_T
{
    BSH_VBIF_SRC_TYPE_T    e_src_type;
    UINT16                 ui2_pid;
}   BSH_VBIF_SRC_INFO_T;

/* SM_BSH_SET_TYPE_VBIF_CTRL settings ****************************************/
/*------------------------------------------------------------------*/
/*! @enum BSH_VBIF_CTRL_T
 *  @brief  Specify VBIF control type
 *  @code
 *  typedef enum
 *  {
 *      BSH_VBIF_CTRL_RESET = 0,
 *      BSH_VBIF_CTRL_STOP,
 *      BSH_VBIF_CTRL_START
 *  }   BSH_VBIF_CTRL_T;
 *  @endcode
 *  @li@c  BSH_VBIF_CTRL_RESET                              -VBIF control type reset
 *  @li@c  BSH_VBIF_CTRL_STOP                                -VBIF control type stop
 *  @li@c  BSH_VBIF_CTRL_START                              -VBIF control type start
 */
/*------------------------------------------------------------------*/
typedef enum
{
    BSH_VBIF_CTRL_RESET = 0,
    BSH_VBIF_CTRL_STOP,
    BSH_VBIF_CTRL_START
}   BSH_VBIF_CTRL_T;

/* SM_BSH_SET_TYPE_VIDEO_OUT_CTRL settings ***********************************/
/*------------------------------------------------------------------*/
/*! @enum BSH_VIDEO_OUT_CTRL_T
 *  @brief  Specify video out control type
 *  @code
 *  typedef enum
 *  {
 *      BSH_VIDEO_OUT_CTRL_DISABLE = 0x0,
 *      BSH_VIDEO_OUT_CTRL_ENABLE  = 0x1
 *  }   BSH_VIDEO_OUT_CTRL_T;
 *  @endcode
 *  @li@c  BSH_VIDEO_OUT_CTRL_DISABLE                        -video out control type disable
 *  @li@c  BSH_VIDEO_OUT_CTRL_ENABLE                         -video out control type enable
 */
/*------------------------------------------------------------------*/
typedef enum
{
    BSH_VIDEO_OUT_CTRL_DISABLE = 0x0,
    BSH_VIDEO_OUT_CTRL_ENABLE  = 0x1
}   BSH_VIDEO_OUT_CTRL_T;

/* SM_BSH_SET_TYPE_TV_ENC_CTRL settings **************************************/
/*------------------------------------------------------------------*/
/*! @enum BSH_TV_ENC_CTRL_T
 *  @brief  Specify TV encoder control type
 *  @code
 *  typedef enum
 *  {
 *      BSH_TV_ENC_CTRL_DISABLE = 0x0,
 *      BSH_TV_ENC_CTRL_ENABLE  = 0x1
 *  }   BSH_TV_ENC_CTRL_T;
 *  @endcode
 *  @li@c  BSH_TV_ENC_CTRL_DISABLE                           -TV encoder control type disable
 *  @li@c  BSH_TV_ENC_CTRL_ENABLE                            -TV encoder control type enable
 */
/*------------------------------------------------------------------*/
typedef enum
{
    BSH_TV_ENC_CTRL_DISABLE = 0x0,
    BSH_TV_ENC_CTRL_ENABLE  = 0x1
}   BSH_TV_ENC_CTRL_T;

/*-----------------------------------------------------------------------------
                   functions declarations
-----------------------------------------------------------------------------*/

#endif /* _U_SM_BYPASS_HDLR_H_ */

/*----------------------------------------------------------------------------*/
/*! @} */   /* end of groupMW_STRM_MNGR_BYPASS_HDLR */
/*----------------------------------------------------------------------------*/
