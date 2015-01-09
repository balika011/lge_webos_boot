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
/*! @file u_sm_video_hdlr.h 
 *  $RCSfile: $
 *  $Revision: #1 $
 *  $Date: 2015/01/09 $
 *  $Author: dtvbm11 $
 *  
 *  @par Description:
 *         This header file contains Video Stream handler specific 
 *         definitions, which are exported to all.
 */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*! @addtogroup groupMW_STRM_MNGR_VIDEO_HDLR
 *  @{ 
 */
/*----------------------------------------------------------------------------*/

#ifndef _U_SM_VIDEO_HDLR_H_
#define _U_SM_VIDEO_HDLR_H_

/*-----------------------------------------------------------------------------
                    include files
-----------------------------------------------------------------------------*/
#include "u_common.h"
#include "u_handle.h"
#include "u_sm.h"

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
-----------------------------------------------------------------------------*/

/* command sets *************************************************************/
/* primary commands */
#define SM_VS_CMD_CODE_SET_VIDEO_DECODER                    \
      ((SM_CMD_CODE_T)(SM_CMD_CODE_PRIMARY_BIT_FLAG | 0))    /**<  Indicate to set video decoder information next */
#define SM_VS_CMD_CODE_SET_TV_DECODER                       \
      ((SM_CMD_CODE_T)(SM_CMD_CODE_PRIMARY_BIT_FLAG | 1))    /**<  Indicate to set tv decoder information next     */
#define SM_VS_CMD_CODE_SET_VIDEO_PLANE                      \
      ((SM_CMD_CODE_T)(SM_CMD_CODE_PRIMARY_BIT_FLAG | 2))    /**<  Indicate to set video plane information next    */
#define SM_VS_CMD_CODE_SET_PLANE_MIXER                      \
      ((SM_CMD_CODE_T)(SM_CMD_CODE_PRIMARY_BIT_FLAG | 3))    /**<  Indicate to set plane mixer information next    */
#define SM_VS_CMD_CODE_SET_TV_ENCODER                       \
      ((SM_CMD_CODE_T)(SM_CMD_CODE_PRIMARY_BIT_FLAG | 4))    /**<  Indicate to set tv encoder information next     */
#define SM_VS_CMD_CODE_SET_DISP_REGION                      \
      ((SM_CMD_CODE_T)(SM_CMD_CODE_PRIMARY_BIT_FLAG | 5))    /**<  Indicate to set display region                          */
#define SM_VS_CMD_CODE_SET_VIDEO_REGION                     \
      ((SM_CMD_CODE_T)(SM_CMD_CODE_PRIMARY_BIT_FLAG | 6))    /**<  Indicate to set video region ( source region)  */
#define SM_VS_CMD_CODE_SET_FORCE_PID                        \
      ((SM_CMD_CODE_T)(SM_CMD_CODE_PRIMARY_BIT_FLAG | 7))    /**<  Indicate to use video pid  by client instead of scdb   */
#define SM_VS_CMD_CODE_SET_FORCE_CODEC                      \
      ((SM_CMD_CODE_T)(SM_CMD_CODE_PRIMARY_BIT_FLAG | 8))    /**<  Indicate to use video codec from client instead of scdb */
#define SM_VS_CMD_CODE_SET_MM_CRYPT_INFO                    \
      ((SM_CMD_CODE_T)(SM_CMD_CODE_PRIMARY_BIT_FLAG | 9))    /**<  Indicate to set mm copy protection information */
#define SM_VS_CMD_CODE_SET_FORCE_NO_CA_REQ                   \
      ((SM_CMD_CODE_T)(SM_CMD_CODE_PRIMARY_BIT_FLAG | 10))    /**< Indicate that pm not to request CA       */
#define SM_VS_CMD_CODE_SET_AV_REC_FLAG                       \
      ((SM_CMD_CODE_T)(SM_CMD_CODE_PRIMARY_BIT_FLAG | 11))    /**< Indicate the video stream is for recording      */
#define SM_VS_CMD_CODE_SET_SRM_MODE_FLAG                       \
      ((SM_CMD_CODE_T)(SM_CMD_CODE_PRIMARY_BIT_FLAG | 12))    /**< Indicate the video stream is for Thumbnail (b_bypass_vdp == TRUE) ,
                                                                                                                         generally, we should use  SM_VS_CMD_CODE_SET_IGNORE_VDP instead,
      
                                                                                                                         this Settype is just for backward compatible.*/
#define SM_VS_CMD_CODE_SET_IMGRSZ_ID                       \
      ((SM_CMD_CODE_T)(SM_CMD_CODE_PRIMARY_BIT_FLAG | 13))    /**< Indicate to set imge resizer ID */

#define SM_VS_CMD_CODE_SET_IGNORE_VDP                           \
      ((SM_CMD_CODE_T)(SM_CMD_CODE_PRIMARY_BIT_FLAG | 14))    /**< Indicate the video stream is NO vdp included (b_bypass_vdp == TRUE)      */
      
#define SM_VS_CMD_CODE_SET_SEAMLESS_MODE                        \
      ((SM_CMD_CODE_T)(SM_CMD_CODE_PRIMARY_BIT_FLAG | 15))    /**< Indicate seamless mode 4K/FHD */


/* subsequence commands */
#define SM_VS_CMD_CODE_SET_LOGICAL_DESC  ((SM_CMD_CODE_T) 1)    /**<  Indicate to set logical name   */
#define SM_VS_CMD_CODE_SET_HANDLE        ((SM_CMD_CODE_T) 2)    /**<  Indicate to set handle for verify the handle is exist or not  */
#define SM_VS_CMD_CODE_SET_COMP_ID       ((SM_CMD_CODE_T) 3)    /**<  Indicate to use component id from client instead of select from rm  */
#define SM_VS_CMD_CODE_SET_IN_PORT       ((SM_CMD_CODE_T) 4)    /**<  Indicate to use in port from client   */


/* set types *****************************************************************/
#define SM_GRP_VSH                   (( SM_GRP_LAST_ENTRY + 0 ) << SM_CODE_BITS)    /**<  video stream handler group   */
#define SM_GRP_TUNL_DEC              (( SM_GRP_LAST_ENTRY + 1 ) << SM_CODE_BITS)    /**<  decoder group           */
#define SM_GRP_TUNL_VIDEO_PLANE      (( SM_GRP_LAST_ENTRY + 2 ) << SM_CODE_BITS)    /**<  video plane group     */
#define SM_GRP_TUNL_PLANE_MIXER      (( SM_GRP_LAST_ENTRY + 3 ) << SM_CODE_BITS)    /**<  plane mixer group     */
#define SM_GRP_TUNL_TV_ENC           (( SM_GRP_LAST_ENTRY + 4 ) << SM_CODE_BITS)    /**<  tv encoder group      */

#define SM_VSH_SET_TYPE_IGNORE               ((SM_SET_TYPE_T)(SM_GRP_VSH | 0))    /**<  For default value     */
#define SM_VSH_SET_TYPE_CAPTURE              ((SM_SET_TYPE_T)(SM_GRP_VSH | 1))    /**<  Set capture video (not support now)  */
#define SM_VSH_SET_TYPE_DISP_REGION          ((SM_SET_TYPE_T)(SM_GRP_VSH | 2))    /**<  Set display region    */
#define SM_VSH_SET_TYPE_VIDEO_REGION         ((SM_SET_TYPE_T)(SM_GRP_VSH | 3))    /**<  Set video region( source region )      */
#define SM_VSH_SET_TYPE_ENHANCEMENT          ((SM_SET_TYPE_T)(SM_GRP_VSH | 4))    /**<  Set video enhancement information for letter box detection  */
#define SM_VSH_SET_TYPE_PLANE_ORDER          ((SM_SET_TYPE_T)(SM_GRP_VSH | 5))    /**<  Set video plane order      */
#define SM_VSH_SET_TYPE_BLENDING             ((SM_SET_TYPE_T)(SM_GRP_VSH | 6))    /**<  Set blending data to PQ  */
#define SM_VSH_SET_TYPE_QV_CTRL              ((SM_SET_TYPE_T)(SM_GRP_VSH | 7))    /**<  Set QV control information for letter box detectionWWW  */
#define SM_VSH_SET_TYPE_AUTO_CLK_PHS_POS     ((SM_SET_TYPE_T)(SM_GRP_VSH | 8))    /**<  Set auto clock, phase and postion    */
#define SM_VSH_SET_TYPE_AUTO_COLOR           ((SM_SET_TYPE_T)(SM_GRP_VSH | 9))    /**<  Set auto color      */
#define SM_VSH_SET_TYPE_VID_CLOCK            ((SM_SET_TYPE_T)(SM_GRP_VSH | 10))   /**<  Set video clock      */
#define SM_VSH_SET_TYPE_VID_PHASE            ((SM_SET_TYPE_T)(SM_GRP_VSH | 11))   /**<  Set video phase      */
#define SM_VSH_SET_TYPE_VID_H_POS            ((SM_SET_TYPE_T)(SM_GRP_VSH | 12))   /**<  Set video h-position     */
#define SM_VSH_SET_TYPE_VID_V_POS            ((SM_SET_TYPE_T)(SM_GRP_VSH | 13))   /**<  Set video v-position      */
#define SM_VSH_SET_TYPE_VBI_NFY              ((SM_SET_TYPE_T)(SM_GRP_VSH | 14))   /**<  Set vbi notify function to video stream for notify vbi data to client   */
#define SM_VSH_SET_TYPE_VBI_REF_INFO         ((SM_SET_TYPE_T)(SM_GRP_VSH | 15))   /**<  Set vbi reference information( for example cc stream handle) to video stream for receive cc data that cc stream parsed  */
#define SM_VSH_SET_TYPE_VGA_TIMING_OPTION    ((SM_SET_TYPE_T)(SM_GRP_VSH | 16))   /**<  Set vga timing option      */
#define SM_VSH_SET_TYPE_COLOR_SYS            ((SM_SET_TYPE_T)(SM_GRP_VSH | 17))   /**<  Set color system       */
#define SM_VSH_SET_TYPE_SEQUENCE_INFO        ((SM_SET_TYPE_T)(SM_GRP_VSH | 18))   /**<  Set sequence information      */
#define SM_VSH_SET_TYPE_SPEED                ((SM_SET_TYPE_T)(SM_GRP_VSH | 19))   /**<  Set video play speed      */
#define SM_VSH_SET_TYPE_FLUSH                ((SM_SET_TYPE_T)(SM_GRP_VSH | 20))   /**<  Set flush data    */
#define SM_VSH_SET_TYPE_START_RENDER_INFO    ((SM_SET_TYPE_T)(SM_GRP_VSH | 21))   /**<  Set start render information for letter box detection    */
#define SM_VSH_SET_TYPE_CHAPTER              ((SM_SET_TYPE_T)(SM_GRP_VSH | 22))   /**<  Set divx chapter information   */
#define SM_VSH_SET_TYPE_VDEC_REPLAY          ((SM_SET_TYPE_T)(SM_GRP_VSH | 23))   /**<  Set video decoder(h.264) to replay     */
#define SM_VSH_SET_TYPE_IFRAME_MODE          ((SM_SET_TYPE_T)(SM_GRP_VSH | 24))   /**<  Set vdieo      */
#define SM_VSH_SET_TYPE_SRM_THUMBNAIL_MODE   ((SM_SET_TYPE_T)(SM_GRP_VSH | 25))   /**<  Set vdieo SRM thumbnail mode to disable vdp&pmx when open video stream    */
#define SM_VSH_SET_TYPE_RESET_WFD_INFO       ((SM_SET_TYPE_T)(SM_GRP_VSH | 26))   /**<  rest Set wfd vdieo decoder info      */

/* get types *****************************************************************/
#define SM_VSH_GET_TYPE_IGNORE               ((SM_GET_TYPE_T)(SM_GRP_VSH | 0))    /**<  For default value   */
#define SM_VSH_GET_TYPE_DISP_REGION          ((SM_GET_TYPE_T)(SM_GRP_VSH | 1))    /**<  Get display region */
#define SM_VSH_GET_TYPE_VIDEO_REGION         ((SM_GET_TYPE_T)(SM_GRP_VSH | 2))    /**<  Get video region( source region)  */
#define SM_VSH_GET_TYPE_ENHANCEMENT          ((SM_GET_TYPE_T)(SM_GRP_VSH | 3))    /**<  Get video enhancement information  */
#define SM_VSH_GET_TYPE_PLANE_ORDER          ((SM_GET_TYPE_T)(SM_GRP_VSH | 4))    /**<  Get video plane order   */
#define SM_VSH_GET_TYPE_BLENDING             ((SM_GET_TYPE_T)(SM_GRP_VSH | 5))    /**<  Get blending data from PQ  */
#define SM_VSH_GET_TYPE_DECODER              ((SM_GET_TYPE_T)(SM_GRP_VSH | 6))    /**<  Get decoder handle for control decoder */
#define SM_VSH_GET_TYPE_VIDEO_PLANE          ((SM_GET_TYPE_T)(SM_GRP_VSH | 7))    /**<  Get video plane hande for control video plane  */
#define SM_VSH_GET_TYPE_PLANE_MIXER          ((SM_GET_TYPE_T)(SM_GRP_VSH | 8))    /**<  Get plane mixer handle for control plane mixer  */
#define SM_VSH_GET_TYPE_TV_ENCODER           ((SM_GET_TYPE_T)(SM_GRP_VSH | 9))    /**<  Get tv encoder handle for control tv encoder   */
#define SM_VSH_GET_TYPE_QV_POSITION          ((SM_GET_TYPE_T)(SM_GRP_VSH | 10))   /**<  Get QV position    */
#define SM_VSH_GET_TYPE_VIDEO_SRC_RESOLUTION ((SM_GET_TYPE_T)(SM_GRP_VSH | 11))   /**<  Get video source resolution information    */
#define SM_VSH_GET_TYPE_VID_CLOCK            ((SM_GET_TYPE_T)(SM_GRP_VSH | 12))   /**<  Get video clock   */
#define SM_VSH_GET_TYPE_VID_PHASE            ((SM_GET_TYPE_T)(SM_GRP_VSH | 13))   /**<  Get video phase */
#define SM_VSH_GET_TYPE_VID_H_POS            ((SM_GET_TYPE_T)(SM_GRP_VSH | 14))   /**<  Get video h-position  */
#define SM_VSH_GET_TYPE_VID_V_POS            ((SM_GET_TYPE_T)(SM_GRP_VSH | 15))   /**<  Get video v-position  */
#define SM_VSH_GET_TYPE_VID_MIN_MAX          ((SM_GET_TYPE_T)(SM_GRP_VSH | 16))   /**<  Get the max and min value with type from client  */
#define SM_VSH_GET_TYPE_FINAL_DISP_REGION    ((SM_GET_TYPE_T)(SM_GRP_VSH | 17))   /**<  Get final display region that driver calculated  */
#define SM_VSH_GET_TYPE_FINAL_VIDEO_REGION   ((SM_GET_TYPE_T)(SM_GRP_VSH | 18))   /**<  Get final video region that driver calculate      */
#define SM_VSH_GET_TYPE_CC_INDICATOR         ((SM_GET_TYPE_T)(SM_GRP_VSH | 19))   /**<  Get whether open cc indicator  */
#define SM_VSH_GET_TYPE_VBI_DATA             ((SM_GET_TYPE_T)(SM_GRP_VSH | 20))   /**<  Get vbi data    */
#define SM_VSH_GET_TYPE_AMBIG_VGA_TIMING     ((SM_GET_TYPE_T)(SM_GRP_VSH | 21))   /**<  Get information of ambiguous vga timing  */
#define SM_VSH_GET_TYPE_COLOR_SYS            ((SM_GET_TYPE_T)(SM_GRP_VSH | 22))   /**<  Get information of color system      */
#define SM_VSH_GET_TYPE_REGION_CAPABILITY    ((SM_GET_TYPE_T)(SM_GRP_VSH | 23))   /**<  Get display region or video region capability   */
#define SM_VSH_GET_TYPE_DIG_DECODE_STATUS    ((SM_GET_TYPE_T)(SM_GRP_VSH | 24))   /**<  Get video decoder current decode status     */
#define SM_VSH_GET_TYPE_PICTURE_INFO         ((SM_GET_TYPE_T)(SM_GRP_VSH | 25))   /**<  Get picture information    */
#define SM_VSH_GET_TYPE_SPEED                ((SM_GET_TYPE_T)(SM_GRP_VSH | 26))   /**<  Get speed of video play   */
#define SM_VSH_GET_TYPE_THUMBNAIL            ((SM_GET_TYPE_T)(SM_GRP_VSH | 27))   /**<  Get thumbnail information  */
#define SM_VSH_GET_TYPE_HDMI_WITH_XVYCC      ((SM_GET_TYPE_T)(SM_GRP_VSH | 28))   /**<  Get whether HDMI with xvYCC   */
#define SM_VSH_GET_TYPE_MEM_BAND_STATUS      ((SM_GET_TYPE_T)(SM_GRP_VSH | 29))   /**<  Get memory bandwidth status   */
#define SM_VSH_GET_TYPE_PROGRESS_SEQ         ((SM_GET_TYPE_T)(SM_GRP_VSH | 30)) 
#define SM_VSH_GET_TYPE_PROFILE_LEVEL        ((SM_GET_TYPE_T)(SM_GRP_VSH | 31))
#define SM_VSH_GET_TYPE_DROP_FRAME_CNT       ((SM_GET_TYPE_T)(SM_GRP_VSH | 32))   /**<  Get video drop frame count   */
#define SM_VSH_GET_TYPE_WFD_ERROR_INFO       ((SM_GET_TYPE_T)(SM_GRP_VSH | 33))


#define SM_VSH_VID_COLCK_UNKNOWN      ((INT16)0x7FFF)     /**<  Video clock default value  */
#define SM_VSH_VID_PHASE_UNKNOWN      ((UINT16)0xFFFF)    /**<  Video phase default value   */
#define SM_VSH_VID_H_POS_UNKNOWN      ((UINT16)0xFFFF)    /**<  Video h-position default value  */
#define SM_VSH_VID_V_POS_UNKNOWN      ((UINT16)0xFFFF)    /**<  Video v-position default value  */


/* SM_VSH_SET_TYPE_VBI_NFY data info *****************************************/
#define MAX_VBI_DATA_SIZE     64      /**<  Max size of vbi data  */

/*------------------------------------------------------------------*/
/*! @enum SM_VSH_VBI_TYPE_T
 *  @brief Specify VSH VBI type.
 *  @code
 *  typedef enum
 *  {
 *      SM_VSH_VBI_TYPE_UNKNOWN = 0,
 *      SM_VSH_VBI_TYPE_NTSC
 *  } SM_VSH_VBI_TYPE_T;
 *  @endcode
 *  @li@c  SM_VSH_VBI_TYPE_UNKNOWN                      -Unknown
 *  @li@c  SM_VSH_VBI_TYPE_NTSC                         -NTSC
 */
/*------------------------------------------------------------------*/
typedef enum
{
    SM_VSH_VBI_TYPE_UNKNOWN = 0,
    SM_VSH_VBI_TYPE_NTSC
} SM_VSH_VBI_TYPE_T;

/*------------------------------------------------------------------*/
/*! @struct SM_VSH_VBI_INFO_T
 *  @brief Contains VSH VBI information.
 *  @code
 *  typedef struct _SM_VSH_VBI_INFO_T
 *  {
 *      SM_VSH_VBI_TYPE_T   e_vbi_type;
 *      VOID*               pv_vbi_data;
 *      SIZE_T              z_vbi_data_size;
 *  } SM_VSH_VBI_INFO_T;
 *  @endcode
 *  @li@c  e_vbi_type                                        -vbi type
 *  @li@c  pv_vbi_data                                       -vbi data
 *  @li@c  z_vbi_data_size                                   -vbo data size
 */
/*------------------------------------------------------------------*/
typedef struct _SM_VSH_VBI_INFO_T
{
    SM_VSH_VBI_TYPE_T   e_vbi_type;
    
    VOID*               pv_vbi_data;
    SIZE_T              z_vbi_data_size;
} SM_VSH_VBI_INFO_T;

/*------------------------------------------------------------------*/
/*! @brief Vbi notify function.
 *  @param [in]  
 *  @param [out]
 *  @note
 *  @see
 *  @return
 *  @retval      -
 */
/*------------------------------------------------------------------*/
typedef INT32 (*x_sm_vbi_nfy_fct)( VOID*               pv_nfy_tag,
                                   SM_VSH_VBI_INFO_T*  pt_vbi );

/*------------------------------------------------------------------*/
/*! @struct SM_VSH_VBI_NFY_T
 *  @brief Contains VSH VBI notify.
 *  @code
 *  typedef struct _SM_VSH_VBI_NFY_T
 *  {
 *      x_sm_vbi_nfy_fct   pf_vbi_nfy;
 *      VOID*              pv_tag;
 *  } SM_VSH_VBI_NFY_T;
 *  @endcode
 *  @li@c  pf_vbi_nfy                                        -VBI notify function
 *  @li@c  pv_tag                                            -notify function tag
 */
/*------------------------------------------------------------------*/
typedef struct _SM_VSH_VBI_NFY_T
{
    x_sm_vbi_nfy_fct   pf_vbi_nfy;
    VOID*              pv_tag;
} SM_VSH_VBI_NFY_T;


/*------------------------------------------------------------------*/
/*! @struct SM_VSH_START_RENDER_INFO_T
 *  @brief Contains VSH start render information.
 *  @code
 *  typedef struct _SM_VSH_START_RENDER_INFO_T
 *  {
 *      PTS_T                     t_pts;
 *      BOOL                      b_auto_pause;
 *  } SM_VSH_START_RENDER_INFO_T;
 *  @endcode
 *  @li@c  t_pts                                             -PTS
 *  @li@c  b_auto_pause                                      -auto pause or not
 */
/*------------------------------------------------------------------*/
typedef struct _SM_VSH_START_RENDER_INFO_T
{
    PTS_T                     t_pts;
    BOOL                      b_auto_pause;
} SM_VSH_START_RENDER_INFO_T;


/* SM_VSH_SET_TYPE_VBI_REF_INFO data info ************************************/
/*------------------------------------------------------------------*/
/*! @enum SM_VSH_VBI_REF_INFO_TYPE_T
 *  @brief Specify VSH VBI reference info type.
 *  @code
 *  typedef enum
 *  {
 *      SM_VSH_VBI_REF_INFO_TYPE_UNKNOWN = 0,
 *      SM_VSH_VBI_REF_INFO_TYPE_CC_STRM_HDL
 *  } SM_VSH_VBI_REF_INFO_TYPE_T;
 *  @endcode
 *  @li@c  SM_VSH_VBI_REF_INFO_TYPE_UNKNOWN                  -unknown
 *  @li@c  SM_VSH_VBI_REF_INFO_TYPE_CC_STRM_HDL              -CC stream handle
 */
/*------------------------------------------------------------------*/
typedef enum
{
    SM_VSH_VBI_REF_INFO_TYPE_UNKNOWN = 0,
    SM_VSH_VBI_REF_INFO_TYPE_CC_STRM_HDL
} SM_VSH_VBI_REF_INFO_TYPE_T;

/*------------------------------------------------------------------*/
/*! @struct SM_VSH_VBI_REF_INFO_T
 *  @brief Contains VSH VBI reference info.
 *  @code
 *  typedef struct _SM_VSH_VBI_REF_INFO_T
 *  {
 *      SM_VSH_VBI_REF_INFO_TYPE_T   e_vbi_ref_type;
 *      union
 *      {
 *          HANDLE_T           h_cc_strm;
 *          VOID*              pv_reserve;
 *      } u;
 *  } SM_VSH_VBI_REF_INFO_T;
 *  @endcode
 *  @li@c  e_vbi_ref_type                                    -VBI reference type
 *  @li@c  h_cc_strm                                         -CC stream handle
 *  @li@c  pv_reserve                                        -reserve
 */
/*------------------------------------------------------------------*/
typedef struct _SM_VSH_VBI_REF_INFO_T
{
    SM_VSH_VBI_REF_INFO_TYPE_T   e_vbi_ref_type;
    
    union
    {
        HANDLE_T           h_cc_strm;
        VOID*              pv_reserve;
    } u;
} SM_VSH_VBI_REF_INFO_T;


/* SM_VSH_SET_TYPE_CAPTURE data info *****************************************/
/*------------------------------------------------------------------*/
/*! @struct VSH_CAPTURE_INFO_T
 *  @brief Contain VSH capture info.
 *  @code
 *  typedef struct _VSH_CAPTURE_INFO_T
 *  {
 *      UINT32       ui4_length;
 *      UINT8*       pui1_frame_buff;
 *  }   VSH_CAPTURE_INFO_T;
 *  @endcode
 *  @li@c  ui4_length                                        -length
 *  @li@c  pui1_frame_buff                                   -frame buffer
 */
/*------------------------------------------------------------------*/
typedef struct _VSH_CAPTURE_INFO_T
{
    UINT32       ui4_length;
    UINT8*       pui1_frame_buff;
}   VSH_CAPTURE_INFO_T;


/* SM_VSH_SET_TYPE_DISP_REGION/SM_VSH_GET_TYPE_DISP_REGION data info *********/
/* SM_VSH_SET_TYPE_VIDEO_REGION/SM_VSH_GET_TYPE_VIDEO_REGION data info *******/
#define VSH_REGION_MAX_WIDTH      ((UINT32) 10000)    /**<   The max width of display region or video region   */
#define VSH_REGION_MAX_HEIGHT     ((UINT32) 10000)    /**<   The max height of display region or video region  */

/*------------------------------------------------------------------*/
/*! @struct VSH_REGION_INFO_T
 *  @brief Contains VSH region info.
 *  @code
 *  typedef struct _VSH_REGION_INFO_T
 *  {
 *      UINT32          ui4_x;
 *      UINT32          ui4_y;
 *      UINT32          ui4_width;
 *      UINT32          ui4_height;
 *  }   VSH_REGION_INFO_T;
 *  @endcode
 *  @li@c  ui4_x                                             -start x
 *  @li@c  ui4_y                                             -start y
 *  @li@c  ui4_width                                         -width
 *  @li@c  ui4_height                                        -height
 */
/*------------------------------------------------------------------*/
typedef struct _VSH_REGION_INFO_T
{
    UINT32          ui4_x;
    UINT32          ui4_y;
    UINT32          ui4_width;
    UINT32          ui4_height;
}   VSH_REGION_INFO_T;

/*------------------------------------------------------------------*/
/*! @struct VSH_CHAP_INFO_T
 *  @brief Contains divx chapter information.
 *  @code
 *  typedef struct _VSH_CHAP_INFO_T
 *  {
 *     BOOL         b_seek;
 *     BOOL         b_first;
 *     UINT64      ui8_start_time;
 *     UINT64      ui8_end_time;
 *  }   VSH_CHAP_INFO_T;
 *  @endcode
 *  @li@c  b_seek                                             -whether seek operation
 *  @li@c  b_first                                               -whether first
 *  @li@c  ui8_start_time                                  -start time
 *  @li@c  ui8_end_time                                    -end time
 */
/*------------------------------------------------------------------*/
typedef struct _VSH_CHAP_INFO_T 
{
    BOOL        b_seek;
    BOOL        b_first;
    UINT64      ui8_start_time;
    UINT64      ui8_end_time;
}   VSH_CHAP_INFO_T;

/* SM_VSH_GET_TYPE_REGION_CAPABILITY *****************************************/
/*------------------------------------------------------------------*/
/*! @struct VSH_REGION_CAPABILITY_INFO_T
 *  @brief Contains VSH region capability info.
 *  @code
 *  typedef struct _VSH_REGION_CAPABILITY_INFO_T
 *  {
 *      UINT32          e_get_type;    
 *      BOOL            b_is_enable;   
 *      UINT32          ui4_x_min;
 *      UINT32          ui4_x_max;
 *      UINT32          ui4_y_min;
 *      UINT32          ui4_y_max;
 *      UINT32          ui4_width_min;
 *      UINT32          ui4_width_max;
 *      UINT32          ui4_height_min;
 *      UINT32          ui4_height_max;
 *  }   VSH_REGION_CAPABILITY_INFO_T;
 *  @endcode
 *  @li@c  e_get_type                                        -get type( display region or video region )
 *  @li@c  b_is_enable                                       -enable or not
 *  @li@c  ui4_x_min                                         -x minimum
 *  @li@c  ui4_x_max                                         -x maximum
 *  @li@c  ui4_y_min                                         -y minimum
 *  @li@c  ui4_y_max                                         -y maximum
 *  @li@c  ui4_width_min                                     -width minimum
 *  @li@c  ui4_width_max                                     -width maximum
 *  @li@c  ui4_height_min                                    -height minimum
 *  @li@c  ui4_height_max                                    -height maximum
 */
/*------------------------------------------------------------------*/
typedef struct _VSH_REGION_CAPABILITY_INFO_T
{
    UINT32          e_get_type;    /* IN */
    BOOL            b_is_enable;   /* is function enable? */
    UINT32          ui4_x_min;
    UINT32          ui4_x_max;
    UINT32          ui4_y_min;
    UINT32          ui4_y_max;
    UINT32          ui4_width_min;
    UINT32          ui4_width_max;
    UINT32          ui4_height_min;
    UINT32          ui4_height_max;
}   VSH_REGION_CAPABILITY_INFO_T;

/* SM_VSH_SET_TYPE_ENHANCEMENT/SM_VSH_GET_TYPE_ENHANCEMENT data info *********/
/*------------------------------------------------------------------*/
/*! @enum VSH_ENHANCE_MODE_T
 *  @brief Specify VSH enhance mode.
 *  @code
 *  typedef enum {
 *      VSH_ENHANCE_MODE_DISABLE,
 *      VSH_ENHANCE_MODE_ENHANCEMENT,
 *      VSH_ENHANCE_MODE_BLUR
 *  } VSH_ENHANCE_MODE_T;
 *  @endcode
 *  @li@c  VSH_ENHANCE_MODE_DISABLE                          -disable
 *  @li@c  VSH_ENHANCE_MODE_ENHANCEMENT                      -enhancement
 *  @li@c  VSH_ENHANCE_MODE_BLUR                             -blur
 */
/*------------------------------------------------------------------*/
typedef enum {
    VSH_ENHANCE_MODE_DISABLE,
    VSH_ENHANCE_MODE_ENHANCEMENT,
    VSH_ENHANCE_MODE_BLUR
} VSH_ENHANCE_MODE_T;

/*------------------------------------------------------------------*/
/*! @struct VSH_ENCHANCE_INFO_T
 *  @brief Contains VSH enhance info.
 *  @code
 *  typedef struct _VSH_ENCHANCE_INFO_T
 *  {
 *      VSH_ENHANCE_MODE_T    e_mode;
 *      UINT8                 ui1_level;
 *  }   VSH_ENCHANCE_INFO_T;
 *  @endcode
 *  @li@c  e_mode                                            -enhance mode
 *  @li@c  ui1_level                                         -level
 */
/*------------------------------------------------------------------*/
typedef struct _VSH_ENCHANCE_INFO_T
{
    VSH_ENHANCE_MODE_T    e_mode;
    UINT8                 ui1_level;
}   VSH_ENCHANCE_INFO_T;


/* SM_VSH_SET_TYPE_PLANE_ORDER data info *************************************/
/*------------------------------------------------------------------*/
/*! @enum VSH_SET_PLANE_ORDER_CTRL_T
 *  @brief Specify VSH set plane order control type.
 *  @code
 *  typedef enum {
 *      VSH_PLANE_ORDER_CTRL_TOP,
 *      VSH_PLANE_ORDER_CTRL_UP,
 *      VSH_PLANE_ORDER_CTRL_DOWN,
 *      VSH_PLANE_ORDER_CTRL_SWAP,
 *      VSH_PLANE_ORDER_CTRL_BOTTOM
 *  } VSH_SET_PLANE_ORDER_CTRL_T;
 *  @endcode
 *  @li@c  VSH_PLANE_ORDER_CTRL_TOP                          -top
 *  @li@c  VSH_PLANE_ORDER_CTRL_UP                           -up
 *  @li@c  VSH_PLANE_ORDER_CTRL_DOWN                         -down
 *  @li@c  VSH_PLANE_ORDER_CTRL_SWAP                         -swap
 *  @li@c  VSH_PLANE_ORDER_CTRL_BOTTOM                       -bottom
 */
/*------------------------------------------------------------------*/
typedef enum {
    VSH_PLANE_ORDER_CTRL_TOP,
    VSH_PLANE_ORDER_CTRL_UP,
    VSH_PLANE_ORDER_CTRL_DOWN,
    VSH_PLANE_ORDER_CTRL_SWAP,
    VSH_PLANE_ORDER_CTRL_BOTTOM
} VSH_SET_PLANE_ORDER_CTRL_T;

/*------------------------------------------------------------------*/
/*! @struct VSH_SET_PLANE_ORDER_INFO_T
 *  @brief Contains VSH set plane order info.
 *  @code
 *  typedef struct _VSH_SET_PLANE_ORDER_INFO_T
 *  {
 *      VSH_SET_PLANE_ORDER_CTRL_T   e_order_ctrl;
 *      union {
 *          UINT8                    ui1_num_layers;  
 *          UINT8                    ui1_layer;       
 *      } u;
 *  }   VSH_SET_PLANE_ORDER_INFO_T;
 *  @endcode
 *  @li@c  e_order_ctrl                                      -order control
 *  @li@c  ui1_num_layers                                    -layers number
 *  @li@c  ui1_layer                                         -layer index
 */
/*------------------------------------------------------------------*/
typedef struct _VSH_SET_PLANE_ORDER_INFO_T
{
    VSH_SET_PLANE_ORDER_CTRL_T   e_order_ctrl;
    
    union {
        UINT8                    ui1_num_layers;  /* up or down */
        UINT8                    ui1_layer;       /* swap */   
    } u;
}   VSH_SET_PLANE_ORDER_INFO_T;


/* SM_VSH_GET_TYPE_PLANE_ORDER data info *************************************/
/*------------------------------------------------------------------*/
/*! @struct VSH_GET_PLANE_ORDER_INFO_T
 *  @brief Contains VSH get plane order info.
 *  @code
 *  typedef struct _VSH_GET_PLANE_ORDER_INFO_T
 *  {
 *      UINT8                ui1_curr_layer;
 *      UINT8                ui1_num_layers;  
 *  }   VSH_GET_PLANE_ORDER_INFO_T;
 *  @endcode
 *  @li@c  ui1_curr_layer                                    -current layer
 *  @li@c  ui1_num_layers                                    -total layers number
 */
/*------------------------------------------------------------------*/
typedef struct _VSH_GET_PLANE_ORDER_INFO_T
{
    UINT8                ui1_curr_layer;
    UINT8                ui1_num_layers;  /* total number of layers */   
}   VSH_GET_PLANE_ORDER_INFO_T;


/* SM_VSH_SET_TYPE_BLANDING/SM_VSH_GET_TYPE_BLANDING data info ***************/
typedef UINT8 VSH_BLENDING_INFO_T;    /**<        */


/* SM_VSH_GET_TYPE_QV_CTRL info **********************************************/
/*------------------------------------------------------------------*/
/*! @enum VSH_QV_POSITION_INFO_T
 *  @brief Specify VSH QV position info.
 *  @code
 *  typedef enum
 *  {
 *      VSH_QV_POSITION_IGNORE =0,
 *      VSH_QV_POSITION_TOP_LEFT,
 *      VSH_QV_POSITION_TOP_RIGHT,
 *      VSH_QV_POSITION_BOTTOM_LEFT,
 *      VSH_QV_POSITION_BOTTOM_RIGHT
 *  } VSH_QV_POSITION_INFO_T;
 *  @endcode
 *  @li@c  VSH_QV_POSITION_IGNORE                            -ignore
 *  @li@c  VSH_QV_POSITION_TOP_LEFT                          -top left
 *  @li@c  VSH_QV_POSITION_TOP_RIGHT                         -top right
 *  @li@c  VSH_QV_POSITION_BOTTOM_LEFT                       -bottom left
 *  @li@c  VSH_QV_POSITION_BOTTOM_RIGHT                      -bottom right
 */
/*------------------------------------------------------------------*/
typedef enum
{
    VSH_QV_POSITION_IGNORE =0,
    VSH_QV_POSITION_TOP_LEFT,
    VSH_QV_POSITION_TOP_RIGHT,
    VSH_QV_POSITION_BOTTOM_LEFT,
    VSH_QV_POSITION_BOTTOM_RIGHT
} VSH_QV_POSITION_INFO_T;

/* SM_VSH_SET_TYPE_QV_CTRL info **********************************************/
/*------------------------------------------------------------------*/
/*! @enum VSH_QV_CTRL_TYPE_T
 *  @brief Specify VSH QV control type.
 *  @code
 *  typedef enum 
 *  {
 *      VSH_QV_CTRL_TYPE_ROTATE,
 *      VSH_QV_CTRL_TYPE_SWAP
 *  } VSH_QV_CTRL_TYPE_T;
 *  @endcode
 *  @li@c  VSH_QV_CTRL_TYPE_ROTATE                           -rotate
 *  @li@c  VSH_QV_CTRL_TYPE_SWAP                             -swap
 */
/*------------------------------------------------------------------*/
typedef enum 
{
    VSH_QV_CTRL_TYPE_ROTATE,
    VSH_QV_CTRL_TYPE_SWAP
} VSH_QV_CTRL_TYPE_T;

/*------------------------------------------------------------------*/
/*! @enum VSH_QV_ROTATE_TYPE_T
 *  @brief Specify VSH QV rotate type.
 *  @code
 *  typedef enum 
 *  {
 *      VSH_QV_ROTATE_TYPE_CLOCKWISE,
 *      VSH_QV_ROTATE_TYPE_ANTI_CLOCKWISE
 *  } VSH_QV_ROTATE_TYPE_T;
 *  @endcode
 *  @li@c  VSH_QV_ROTATE_TYPE_CLOCKWISE                      -clockwise
 *  @li@c  VSH_QV_ROTATE_TYPE_ANTI_CLOCKWISE                 -anti clockwise
 */
/*------------------------------------------------------------------*/
typedef enum 
{
    VSH_QV_ROTATE_TYPE_CLOCKWISE,
    VSH_QV_ROTATE_TYPE_ANTI_CLOCKWISE
} VSH_QV_ROTATE_TYPE_T;

/*------------------------------------------------------------------*/
/*! @struct VSH_QV_ROTATE_T
 *  @brief Contains VSH QV rotate.
 *  @code
 *  typedef struct
 *  {
 *      UINT8                 ui1_steps;     
 *      VSH_QV_ROTATE_TYPE_T  e_rotate_type;
 *  } VSH_QV_ROTATE_T;
 *  @endcode
 *  @li@c  ui1_steps                                         -steps
 *  @li@c  e_rotate_type                                     -rotate type
 */
/*------------------------------------------------------------------*/
typedef struct
{
    UINT8                 ui1_steps;     /* 1-3 */
    VSH_QV_ROTATE_TYPE_T  e_rotate_type;
} VSH_QV_ROTATE_T;

/*------------------------------------------------------------------*/
/*! @struct VSH_QV_CTRL_INFO_T
 *  @brief Contains VSH QV control info.
 *  @code
 *  typedef struct
 *  {
 *      VSH_QV_CTRL_TYPE_T          e_ctrl;
 *      union {
 *          VSH_QV_ROTATE_T         e_rotate;
 *          VSH_QV_POSITION_INFO_T  e_swap_to;
 *      } u;
 *  } VSH_QV_CTRL_INFO_T;
 *  @endcode
 *  @li@c  e_ctrl                                            -control type
 *  @li@c  e_rotate                                          -rotate
 *  @li@c  e_swap_to                                         -position swap to
 */
/*------------------------------------------------------------------*/
typedef struct
{
    VSH_QV_CTRL_TYPE_T          e_ctrl;
    
    union {
        VSH_QV_ROTATE_T         e_rotate;
        VSH_QV_POSITION_INFO_T  e_swap_to;
    } u;
} VSH_QV_CTRL_INFO_T;

/* SM_VSH_GET_TYPE_VIDEO_SRC_RESOLUTION info *********************************/
#define VSH_SRC_RESOLUTION_INFO_FRAME_RATE_UNKNOWN    ((UINT32) 0)    /**<  frame rate default value    */

/*------------------------------------------------------------------*/
/*! @enum VSH_SRC_ASPECT_RATIO_T
 *  @brief Specify VSH source aspect ratio.
 *  @code
 *  typedef enum 
 *  {
 *      VSH_SRC_ASPECT_RATIO_UNKNOWN = 0,
 *      VSH_SRC_ASPECT_RATIO_4_3,         
 *      VSH_SRC_ASPECT_RATIO_16_9,        
 *      VSH_SRC_ASPECT_RATIO_2_21_1,      
 *      VSH_SRC_ASPECT_RATIO_PXL_10_11,   
 *      VSH_SRC_ASPECT_RATIO_PXL_40_33,   
 *      VSH_SRC_ASPECT_RATIO_PXL_16_11,   
 *      VSH_SRC_ASPECT_RATIO_PXL_12_11,   
 *      VSH_SRC_ASPECT_RATIO_PXL_3_2,     
 *      VSH_SRC_ASPECT_RATIO_PXL_1_1      
 *  } VSH_SRC_ASPECT_RATIO_T;
 *  @endcode
 *  @li@c  VSH_SRC_ASPECT_RATIO_UNKNOWN                      -unknown
 *  @li@c  VSH_SRC_ASPECT_RATIO_4_3                          -4:3
 *  @li@c  VSH_SRC_ASPECT_RATIO_16_9                         -16:9
 *  @li@c  VSH_SRC_ASPECT_RATIO_2_21_1                       -2.21:1
 *  @li@c  VSH_SRC_ASPECT_RATIO_PXL_10_11                    -10:11
 *  @li@c  VSH_SRC_ASPECT_RATIO_PXL_40_33                    -40:33
 *  @li@c  VSH_SRC_ASPECT_RATIO_PXL_16_11                    -16:11
 *  @li@c  VSH_SRC_ASPECT_RATIO_PXL_12_11                    -12:11
 *  @li@c  VSH_SRC_ASPECT_RATIO_PXL_3_2                      -3:2
 *  @li@c  VSH_SRC_ASPECT_RATIO_PXL_1_1                      -1:1
 *  @li@c  VSH_SRC_ASPECT_RATIO_USER                           - get aspect ratio from sys_asp_ratio.c
 */
/*------------------------------------------------------------------*/
typedef enum 
{
    VSH_SRC_ASPECT_RATIO_UNKNOWN = 0,
    VSH_SRC_ASPECT_RATIO_PXL_1_1,     /* PAR 1:1 */
    VSH_SRC_ASPECT_RATIO_4_3,         /* DAR 4:3 */
    VSH_SRC_ASPECT_RATIO_16_9,        /* DAR 16:9 */
    VSH_SRC_ASPECT_RATIO_2_21_1,      /* DAR 2.21:1 */
    VSH_SRC_ASPECT_RATIO_PXL_10_11,   /* PAR 10:11 */
    VSH_SRC_ASPECT_RATIO_PXL_40_33,   /* PAR 40:33 */
    VSH_SRC_ASPECT_RATIO_PXL_16_11,   /* PAR 16:11 */
    VSH_SRC_ASPECT_RATIO_PXL_12_11,   /* PAR 12:11 */
    VSH_SRC_ASPECT_RATIO_PXL_3_2,     /* PAR 3:2 */
    VSH_SRC_ASPECT_RATIO_PXL_24_11,
    VSH_SRC_ASPECT_RATIO_PXL_20_11,
    VSH_SRC_ASPECT_RATIO_PXL_32_11,
    VSH_SRC_ASPECT_RATIO_PXL_80_33,
    VSH_SRC_ASPECT_RATIO_PXL_18_11,
    VSH_SRC_ASPECT_RATIO_PXL_15_11,
    VSH_SRC_ASPECT_RATIO_PXL_64_33,
    VSH_SRC_ASPECT_RATIO_PXL_160_99,
    VSH_SRC_ASPECT_RATIO_USR,          /* user */
    VSH_SRC_ASPECT_RATIO_2_1
}VSH_SRC_ASPECT_RATIO_T;
/*--VSH----------------------------------------------------------------*/
/*! @enum VSH_SRC_TIMING_TYPE
 *  @brief Specify VSH source timing type.
 *  @code
 *  typedef enum
 *  {
 *      VSH_SRC_TIMING_UNKNOWN = 0,
 *      VSH_SRC_TIMING_VIDEO,
 *      VSH_SRC_TIMING_GRAPHIC,
 *      VSH_SRC_TIMING_NOT_SUPPORT
 *  } VSH_SRC_TIMING_TYPE;
 *  @endcode
 *  @li@c  VSH_SRC_TIMING_UNKNOWN                            -unknown
 *  @li@c  VSH_SRC_TIMING_VIDEO                              -video timing
 *  @li@c  VSH_SRC_TIMING_GRAPHIC                            -graphic timing  ( PC timing)
 *  @li@c  VSH_SRC_TIMING_NOT_SUPPORT                        -timing no support
 */
/*------------------------------------------------------------------*/
typedef enum
{
    VSH_SRC_TIMING_UNKNOWN = 0,
    VSH_SRC_TIMING_VIDEO,       /* Video timing: 480i, 480p, 576i, 576p, 720p, 1080i, 1080p */
    VSH_SRC_TIMING_GRAPHIC,     /* PC timing: all timing except video timing, and pc timing only in HDMI and VGA source */
    VSH_SRC_TIMING_NOT_SUPPORT  /* Timing not suppot in driver */
} VSH_SRC_TIMING_TYPE;

/*------------------------------------------------------------------*/
/*! @enum VSH_SRC_COLOR_FMT_T
 *  @brief Specify VSH source color format.
 *  @code
 *  typedef enum
 *  {
 *      VSH_SRC_COLOR_FMT_UNKNOWN = 0,
 *      VSH_SRC_COLOR_FMT_VGA_4_4_4,
 *      VSH_SRC_COLOR_FMT_YUV_4_2_2,
 *      VSH_SRC_COLOR_FMT_YUV_4_4_4
 *  } VSH_SRC_COLOR_FMT_T;
 *  @endcode
 *  @li@c  VSH_SRC_COLOR_FMT_UNKNOWN                         -unknown
 *  @li@c  VSH_SRC_COLOR_FMT_VGA_4_4_4                       -VGA 444
 *  @li@c  VSH_SRC_COLOR_FMT_YUV_4_2_2                       -YUV 422
 *  @li@c  VSH_SRC_COLOR_FMT_YUV_4_4_4                       -YUV 444
 */
/*------------------------------------------------------------------*/
typedef enum
{
    VSH_SRC_COLOR_FMT_UNKNOWN = 0,
    VSH_SRC_COLOR_FMT_VGA_4_4_4,
    VSH_SRC_COLOR_FMT_YUV_4_2_2,
    VSH_SRC_COLOR_FMT_YUV_4_4_4
} VSH_SRC_COLOR_FMT_T;

typedef enum
{
    VSH_SEAMLESS_MODE_NONE,
    VSH_SEAMLESS_MODE_SD,
    VSH_SEAMLESS_MODE_HD,
    VSH_SEAMLESS_MODE_FHD,
    VSH_SEAMLESS_MODE_4K2K
} VSH_SEAMLESS_MODE_T;

#define VSH_SRC_RES_EXT_HDMI_SCAN_INFO_MASK           ((UINT32)0x3)    /**<   Hdmi scan information bit mask  */
#define VSH_SRC_RES_EXT_GET_HDMI_SCAN_INFO( ui4_ext )                       \
    ((ui4_ext) & VSH_SRC_RES_EXT_HDMI_SCAN_INFO_MASK)                  /**<   Get hdmi scan information in video source resolution  */

#define VSH_SRC_RES_EXT_HDMI_SCAN_INFO_NONE                 ((UINT32)0x0)    /**<  None  */
#define VSH_SRC_RES_EXT_HDMI_SCAN_INFO_OVERSCAN             ((UINT32)0x1)    /**<  Overscan  */
#define VSH_SRC_RES_EXT_HDMI_SCAN_INFO_UNDERSCAN            ((UINT32)0x2)    /**<  Underscan   */
#define VSH_SRC_RES_EXT_HDMI_SCAN_INFO_FUTURE               ((UINT32)0x3)    /**<  Future use   */

#define VSH_SRC_RES_EXT_HDMI_PIC_ASP_RATIO_MASK       ((UINT32)0xc)    /**<  Hdmi picture aspect ratio bit mask   */
#define VSH_SRC_RES_EXT_GET_HDMI_PIC_ASP_RATIO( ui4_ext )                  \
    (((ui4_ext) & VSH_SRC_RES_EXT_HDMI_PIC_ASP_RATIO_MASK)>>2)        /**<   Get  hdmi picture aspect ratio in video source resolution  */
#define VSH_SRC_RES_EXT_SET_HDMI_PIC_ASP_RATIO( ui4_ext, asp_info )        \
    ( (ui4_ext) = (((UINT32)(ui4_ext) & 0xFFFFFFF3) |                   \
                    (((UINT32)(asp_info<<2)) & VSH_SRC_RES_EXT_HDMI_PIC_ASP_RATIO_MASK)) )  /**<  Set hdmi picture aspect ratio   */

#define VSH_SRC_RES_EXT_HDMI_PIC_ASP_RATIO_NO_DATA          ((UINT32)0x0)    /**<  No data  */
#define VSH_SRC_RES_EXT_HDMI_PIC_ASP_RATIO_4_3              ((UINT32)0x1)    /**<  4:3    */
#define VSH_SRC_RES_EXT_HDMI_PIC_ASP_RATIO_16_9             ((UINT32)0x2)    /**<  16:9  */
#define VSH_SRC_RES_EXT_HDMI_PIC_ASP_RATIO_FUTURE           ((UINT32)0x3)    /**<   Future use  */

#define VSH_SRC_RES_EXT_HDMI_AFAR_MASK                ((UINT32)0xf0)    /**<  Hdmi AFAR? bit mask  */
#define VSH_SRC_RES_EXT_GET_HDMI_AFAR( ui4_ext )                           \
    (((ui4_ext) & VSH_SRC_RES_EXT_HDMI_AFAR_MASK)>>4)                  /**<  Get hdmi AFAR? in video source resolution  */
#define VSH_SRC_RES_EXT_SET_HDMI_AFAR( ui4_ext, afar_info )                \
    ( (ui4_ext) = (((UINT32)(ui4_ext) & 0xFFFFFF0F) |                      \
                    (((UINT32)(afar_info<<4)) & VSH_SRC_RES_EXT_HDMI_AFAR_MASK)) )    /**<  Set hdmi AFAR?   */

#define VSH_SRC_RES_EXT_HDMI_AFAR_SAME_AS_PIC_ASP_RATIO     ((UINT32)0x8)    /**<  Same as picture aspect ratio   */
#define VSH_SRC_RES_EXT_HDMI_AFAR_4_3_CENTER                ((UINT32)0x9)    /**<  4:3 center   */
#define VSH_SRC_RES_EXT_HDMI_AFAR_16_9_CENTER               ((UINT32)0xa)    /**<  16:9 center  */
#define VSH_SRC_RES_EXT_HDMI_AFAR_14_9_CENTER               ((UINT32)0xb)    /**<  14:9 center  */

#define VSH_SRC_RES_EXT_NTSC_WSS_ASP_RATIO_MASK       ((UINT32)0x7000)    /**<  NTSC wss aspect ratio bit mask  */
#define VSH_SRC_RES_EXT_GET_NTSC_WSS_ASP_RATIO( ui4_ext )                  \
    (((ui4_ext) & VSH_SRC_RES_EXT_NTSC_WSS_ASP_RATIO_MASK)>>12)          /**<  Get ntsc wss aspect ratio  */
#define VSH_SRC_RES_EXT_SET_NTSC_WSS_ASP_RATIO( ui4_ext, asp_ratio )       \
    ( (ui4_ext) = (((UINT32)(ui4_ext) & 0xFFFF8FFF) |                   \
                    (((UINT32)(asp_ratio<<12)) & VSH_SRC_RES_EXT_NTSC_WSS_ASP_RATIO_MASK)) )    /**<  Set ntsc wss aspect ratio  */

#define VSH_SRC_RES_EXT_NTSC_WSS_4_3                        ((UINT32)0x0)    /**<  4:3   */
#define VSH_SRC_RES_EXT_NTSC_WSS_16_9                       ((UINT32)0x1)    /**<  16:9  */
#define VSH_SRC_RES_EXT_NTSC_WSS_4_3_LB                     ((UINT32)0x2)    /**<  4:3 letter box  */
#define VSH_SRC_RES_EXT_NTSC_WSS_16_9_LB                    ((UINT32)0x3)    /**<  16:9 letter box  */
#define VSH_SRC_RES_EXT_NTSC_WSS_UNKNOWN                    ((UINT32)0x4)    /**<   default value  */

#define VSH_SRC_RES_EXT_PAL_SECAM_WSS_ASP_RATIO_MASK        ((UINT32)0xf00)  /**<   PAL or SECAM wss aspect  ratio bit mask  */
#define VSH_SRC_RES_EXT_GET_PAL_SECAM_WSS_ASP_RATIO( ui4_ext )                \
    (((ui4_ext) & VSH_SRC_RES_EXT_PAL_SECAM_WSS_ASP_RATIO_MASK)>>8)         /**<   Get pal or secam wss aspect ratio   */
#define VSH_SRC_RES_EXT_SET_PAL_SECAM_WSS_ASP_RATIO( ui4_ext, asp_ratio )     \
    ( (ui4_ext) = (((UINT32)(ui4_ext) & 0xFFFFF0FF) |                      \
                    (((UINT32)(asp_ratio<<8)) & VSH_SRC_RES_EXT_PAL_SECAM_WSS_ASP_RATIO_MASK)) )    /**<  Set pal or secam wss aspect ratio   */

#define VSH_SRC_RES_EXT_PAL_SECAM_WSS_4_3_FF                ((UINT32)0x8)    /**<  4:3 FF?  */
#define VSH_SRC_RES_EXT_PAL_SECAM_WSS_14_9_LB_CENTER        ((UINT32)0x1)    /**<  14:9 letter box center  */
#define VSH_SRC_RES_EXT_PAL_SECAM_WSS_14_9_LB_TOP           ((UINT32)0x2)    /**<  14:9 letter box top  */
#define VSH_SRC_RES_EXT_PAL_SECAM_WSS_16_9_LB_CENTER        ((UINT32)0xb)    /**<  16:9 letter box center  */
#define VSH_SRC_RES_EXT_PAL_SECAM_WSS_16_9_LB_TOP           ((UINT32)0x4)    /**<  16:9 letter box top  */
#define VSH_SRC_RES_EXT_PAL_SECAM_WSS_BIG_16_9_LB_CENTER    ((UINT32)0xd)    /**<  big 16:9 letter box top  *//
#define VSH_SRC_RES_EXT_PAL_SECAM_WSS_4_3_FF_SP_14_9        ((UINT32)0xe)    /**<  4:3 FF? SP? 14:9  */
#define VSH_SRC_RES_EXT_PAL_SECAM_WSS_16_9_FF               ((UINT32)0x7)    /**<  16:9 FF?  */

#define VSH_SRC_RES_EXT_AFD_MASK                      ((UINT32)0xf)    /**<  AFD bit mask   */
#define VSH_SRC_RES_EXT_GET_AFD( ui4_ext )                                 \
    ((ui4_ext) & VSH_SRC_RES_EXT_AFD_MASK)                             /**<  Get afd value    */
#define VSH_SRC_RES_EXT_SET_AFD( ui4_ext, afd_info )                       \
    ( (ui4_ext) = (((UINT32)(ui4_ext) & 0xFFFFFFF0) |                   \
                    (((UINT32)(afd_info)) & VSH_SRC_RES_EXT_AFD_MASK)))    /**<  Set afd value  */

#if 0
/*------------------------------------------------------------------*/
/*! @enum VSH_SCART_PIN_8_TYPE_T
 *  @brief Specify VSH SCART pin 8 type.
 *  @code
 *  typedef enum
 *  {
 *      VSH_SCART_PIN_8_TYPE_UNKNOWN = 0,
 *      VSH_SCART_PIN_8_TYPE_NO_SIGNAL,    
 *      VSH_SCART_PIN_8_TYPE_ASPECT_16_9,
 *      VSH_SCART_PIN_8_TYPE_ASPECT_4_3
 *  }   VSH_SCART_PIN_8_TYPE_T;
 *  @endcode
 *  @li@c  VSH_SCART_PIN_8_TYPE_UNKNOWN                      -unknown
 *  @li@c  VSH_SCART_PIN_8_TYPE_NO_SIGNAL                    -no signal
 *  @li@c  VSH_SCART_PIN_8_TYPE_ASPECT_16_9                  -aspect 16:9
 *  @li@c  VSH_SCART_PIN_8_TYPE_ASPECT_4_3                   -aspect 4:3
 */
/*------------------------------------------------------------------*/
typedef enum
{
    VSH_SCART_PIN_8_TYPE_UNKNOWN = 0,
    VSH_SCART_PIN_8_TYPE_NO_SIGNAL,    
    VSH_SCART_PIN_8_TYPE_ASPECT_16_9,
    VSH_SCART_PIN_8_TYPE_ASPECT_4_3
}   VSH_SCART_PIN_8_TYPE_T;
#endif

#define VSH_HDMI_INFO_FRAME_LEN             (6)    /**<  HDMI AVI Info Frame Data Length   */
#define VSH_SRC_RES_HDMI_CN_GRAPHICS        ((UINT32)0x0)    /**<  Graphic   */
#define VSH_SRC_RES_HDMI_CN_PHOTO           ((UINT32)0x1)    /**<  Photo      */
#define VSH_SRC_RES_HDMI_CN_CINEMA          ((UINT32)0x2)    /**<  Cinema   */
#define VSH_SRC_RES_HDMI_CN_GAME            ((UINT32)0x3)    /**<  Game      */

/*------------------------------------------------------------------*/
/*! @struct VSH_HDMI_AVI_INFOFRAME_INFO_T
 *  @brief Contains VSH HDMI AVI info frame info.
 *  @code
 *  typedef struct _VSH_HDMI_AVI_INFOFRAME_INFO_T
 *  {
 *      BOOL    b_valid; 
 *      UINT8   aui1_info_frame_data [VSH_HDMI_INFO_FRAME_LEN];
 *  } VSH_HDMI_AVI_INFOFRAME_INFO_T;
 *  @endcode
 *  @li@c  b_valid                                           -valid or not
 *  @li@c  aui1_info_frame_data [VSH_HDMI_INFO_FRAME_LEN]    -info frame data
 */
/*------------------------------------------------------------------*/
typedef struct _VSH_HDMI_AVI_INFOFRAME_INFO_T
{
    BOOL    b_valid;  /* TRUE: The following is AVI info frame data; FALSE: no info frame data */
    UINT8   aui1_info_frame_data[VSH_HDMI_INFO_FRAME_LEN];  /* HDMI avi frame data */
} VSH_HDMI_AVI_INFOFRAME_INFO_T;

/*------------------------------------------------------------------*/
/*! @struct VSH_HDMI_SPD_DATA_STRUCT
 *  @brief Contains VSH HDMI SPD info frame info.
 *  @code
 *  typedef struct _VSH_HDMI_SPD_INFOFRAME_INFO_T
 *  {
 *    UINT8 ui1_up_sts;
 *    UINT8 ui1_up_sts_chg;
 *    UINT8 ui1_sv_sts;
 *    UINT8 ui1_sv_sts_chg;
 *    VSH_HDMI_SPD_DATA_STRUCT t_spd_Info_data;
 *  } VSH_HDMI_SPD_INFOFRAME_INFO_T;
 *  @endcode
 *  @li@c  ui1_up_sts            -for UP pattern, if exist,detected,if not,undetected.
 *  @li@c  ui1_up_sts_chg     -for UP pattern, if its existence changed.
 *  @li@c  ui1_sv_sts            -for SV pattern, if exist,detected,if not,undetected.
 *  @li@c  ui1_sv_sts_chg     -for SV pattern, if its existence changed.
 */
/*------------------------------------------------------------------*/

#define VSH_HDMI_SPD_INFO_VNDR_NAME_LEN     (2)    /**<  HDMI SPD Info Frame vender name Length   */
#define VSH_HDMI_SPD_INFO_PRD_DESC_LEN      (4)    /**<  HDMI SPD Info Frame product description Length   */

typedef struct _VSH_HDMI_SPD_DATA_T
{
	UINT32 aui4_vndr_name[VSH_HDMI_SPD_INFO_VNDR_NAME_LEN];//Vendor Name Chacter VN1 TO VN8, 8 bytes.
	UINT32 aui4_product_des[VSH_HDMI_SPD_INFO_PRD_DESC_LEN];//Product Description Chacter PD1 TO PD16,16 bytes.
	UINT8  ui1_src_info;
}VSH_HDMI_SPD_DATA_T;

typedef struct _VSH_HDMI_SPD_INFOFRAME_INFO_T
{
	UINT8 ui1_up_sts;
	UINT8 ui1_up_sts_chg;
	UINT8 ui1_sv_sts;
	UINT8 ui1_sv_sts_chg;
	UINT8 ui1_spd_4k_ps_sts;
	VSH_HDMI_SPD_DATA_T t_spd_Info_data;
}VSH_HDMI_SPD_INFOFRAME_INFO_T;


/*------------------------------------------------------------------*/
/*! @enum VSH_SRC_TAG3D_TYPE_T
 *  @brief Specify VSH tag3D type.
 *  @code
 *  typedef enum
 *  {
 *      VSH_SRC_TAG3D_2D = 0,
 *      VSH_SRC_TAG3D_MVC,    
 *      VSH_SRC_TAG3D_FP,
 *      VSH_SRC_TAG3D_FS,
 *      VSH_SRC_TAG3D_TB, 
 *      VSH_SRC_TAG3D_SBS,
 *      VSH_SRC_TAG3D_REALD,
 *      VSH_SRC_TAG3D_SENSIO,
 *      VSH_SRC_TAG3D_LA,
 *      VSH_SRC_TAG3D_TTDO,
 *      VSH_SRC_TAG3D_NOT_SUPPORT
 *  }   VSH_SRC_TAG3D_TYPE_T;
 *  @endcode
 *  @li@c  VSH_SRC_TAG3D_2D                       -2D
 *  @li@c  VSH_SRC_TAG3D_MVC                     -Multi-View Codec
 *  @li@c  VSH_SRC_TAG3D_FP                      -Frame Packing
 *  @li@c  VSH_SRC_TAG3D_FS                      -Frame Sequential
 *  @li@c  VSH_SRC_TAG3D_TB                      -Top-and-Bottom 
 *  @li@c  VSH_SRC_TAG3D_SBS                     -Side-by-Side
 *  @li@c  VSH_SRC_TAG3D_REALD                   -RealD
 *  @li@c  VSH_SRC_TAG3D_SENSIO                  -Sensio
 *  @li@c  VSH_SRC_TAG3D_LA                      -Line Alternative
 *  @li@c  VSH_SRC_TAG3D_TTDO                    -TTD only
 *  @li@c  VSH_SRC_TAG3D_NOT_SUPPORT             -Not support
 */
/*------------------------------------------------------------------*/
typedef enum
{
    VSH_SRC_TAG3D_2D = 0,
    VSH_SRC_TAG3D_MVC,          /* MVC = Multi-View Codec */
    VSH_SRC_TAG3D_FP,           /* FP = Frame Packing */
    VSH_SRC_TAG3D_FS,           /* FS = Frame Sequential */
    VSH_SRC_TAG3D_TB,           /* TB = Top-and-Bottom */
    VSH_SRC_TAG3D_SBS,          /* SBS = Side-by-Side */
    VSH_SRC_TAG3D_REALD,
    VSH_SRC_TAG3D_SENSIO,
    VSH_SRC_TAG3D_LA,           /* LA = Line Alternative */
    VSH_SRC_TAG3D_TTDO,         /* TTD only. It is 2D mode */
    VSH_SRC_TAG3D_NOT_SUPPORT
}   VSH_SRC_TAG3D_TYPE_T;

/*------------------------------------------------------------------*/
/*! @struct VSH_SRC_RESOLUTION_INFO_T
 *  @brief Contains VSH source resolution info.
 *  @code
 *  typedef struct _VSH_SRC_RESOLUTION_INFO_T
 *  {
 *      UINT32                  ui4_frame_rate;
 *      UINT32                  ui4_width;
 *      UINT32                  ui4_height;
 *      BOOL                    b_is_progressive;
 *      BOOL                    b_overscan_not_applied;
 *      VSH_SRC_ASPECT_RATIO_T  e_src_asp_ratio;
 *      VSH_SRC_TIMING_TYPE     e_timing_type;
 *      VSH_SRC_COLOR_FMT_T     e_color_fmt;
 *      VSH_SRC_COLOR_FMT_T     e_src_color_space;
 *      UINT32                  ui4_res_idx;
 *      UINT32                  ui4_ext;
 *      VSH_HDMI_AVI_INFOFRAME_INFO_T t_hdmi_infoframe;
 *  }   VSH_SRC_RESOLUTION_INFO_T;
 *  @endcode
 *  @li@c  ui4_frame_rate                                    -frame rate
 *  @li@c  ui4_width                                         -width
 *  @li@c  ui4_height                                        -height
 *  @li@c  b_is_progressive                                  -progressive or interlace
 *  @li@c  b_overscan_not_applied                            -overscan or not
 *  @li@c  e_src_asp_ratio                                   -source aspect ratio
 *  @li@c  e_timing_type                                     -timing type
 *  @li@c  e_color_fmt                                       -color format
 *  @li@c  e_src_color_space                                 -source color space
 *  @li@c  ui4_res_idx                                       -resolution index
 *  @li@c  ui4_ext                                           -extern
 *  @li@c  t_hdmi_infoframe                                  -hdmi info frame
 *  @li@c  e_src_tag3d_type                                  -tag3d type
 */
/*------------------------------------------------------------------*/
typedef struct _VSH_SRC_RESOLUTION_INFO_T
{
    UINT32                  ui4_frame_rate;
    UINT32                  ui4_width;
    UINT32                  ui4_height;
    BOOL                    b_is_progressive;
    BOOL                    b_overscan_not_applied;
    VSH_SRC_ASPECT_RATIO_T  e_src_asp_ratio;
    VSH_SRC_TIMING_TYPE     e_timing_type;
    VSH_SRC_COLOR_FMT_T     e_color_fmt;
    VSH_SRC_COLOR_FMT_T     e_src_color_space;
#if 0   /* because driver don't supply the value, and can get scart pin 8 status throught CM avc handler or DSM */
    VSH_SCART_PIN_8_TYPE_T  e_scart_pin_8;
#endif
    UINT32                  ui4_res_idx;
    UINT32                  ui4_ext;
    VSH_HDMI_AVI_INFOFRAME_INFO_T t_hdmi_infoframe;
    VSH_HDMI_SPD_INFOFRAME_INFO_T t_hdmi_spd_infoframe;
    VSH_SRC_TAG3D_TYPE_T    e_src_tag3d_type;
    BOOL                    b_is_display_aspect_ratio;
    UINT32                  ui4_aspect_ratio_w; 
    UINT32                  ui4_aspect_ratio_h;  
    BOOL                    b_is_mhl_signal;
    BOOL                    b_resized;
    UINT16                  ui2_orig_width;
    UINT16                  ui2_orig_height;
    VSH_SRC_ASPECT_RATIO_T  e_orig_src_asp_ratio;
    UINT32                  ui4_orig_aspect_ratio_w; 
    UINT32                  ui4_orig_aspect_ratio_h; 
    BOOL                    b_is_orig_display_aspect_ratio;
}   VSH_SRC_RESOLUTION_INFO_T;

/*VSH_SRC_PROFILE_TYPE_T settings **********************************************/
typedef enum
{
    VSH_SRC_PROFILE_UNKNOWN = 0,
    VSH_SRC_PROFILE_SIMPLE,    //Simple profile
    VSH_SRC_PROFILE_MAIN,    //Main profile
    VSH_SRC_PROFILE_SNR_SCALABLE,    //Snr Scalable profile
    VSH_SRC_PROFILE_SPATIAL_SCALABLE,        //Spatial Scalable profile
    VSH_SRC_PROFILE_HIGH,    //High profile
    VSH_SRC_PROFILE_MULTIVIEW,    //Multiview profile
    VSH_SRC_PROFILE_4_2_2,    //4:2:2 profile
    //MPEG4
    VSH_SRC_PROFILE_ADVANCED_CODING_EFFICIENCY,        //Advanced Coding Efficiency profle
    VSH_SRC_PROFILE_SIMPLE_STUDIO,        //Simple Studio profile
    VSH_SRC_PROFILE_CORE_STUDIO,    //Core Studio profile
    VSH_SRC_PROFILE_FINE_GRANULARITY_SCALABLE,    //Fine Granularity Scalable profile
    VSH_SRC_PROFILE_SIMPLE_FACE_ANIMATION,        //Simple Face Animation profile
    VSH_SRC_PROFILE_SIMPLE_FBA,        //Simple FBA proflie
    VSH_SRC_PROFILE_BASIC_ANIMATED_TEXTURE,    //Basic Animated Texture profile
    VSH_SRC_PROFILE_SCALABLE_TEXTURE,        //Scalable Texture profile
    VSH_SRC_PROFILE_ADVANCED_SCALABLE_TEXTURE,    //Advanced Scalable Texture profile
    VSH_SRC_PROFILE_HYBRID,    //Hybrid profile
    VSH_SRC_PROFILE_SIMPLE_SCALABLE,    //Simple Scalable profile
    VSH_SRC_PROFILE_ADVANCED_SIMPLE,    //Advanced Simple profile
    VSH_SRC_PROFILE_CORE,    //Core profile
    VSH_SRC_PROFILE_CORE_SCALABLE,        //Core Scalable profile
    VSH_SRC_PROFILE_ADVANCED_CORE,        //Advanced Core profile
    VSH_SRC_PROFILE_N_BIT,        //N-Bit profile
    VSH_SRC_PROFILE_REAL_TIME_SIMPLE,    //Real Time profile
    //H264
    VSH_SRC_PROFILE_BASELINE,        //Baseline profile
    VSH_SRC_PROFILE_EXTENDED,        //Extended profile
    VSH_SRC_PROFILE_HIGH_10,        //High 10 profile
    VSH_SRC_PROFILE_HIGH_4_2_2,    //High 4:2:2 profile
    VSH_SRC_PROFILE_HIGH_4_4_4,    //High 4:4:4 profile
    //VC1
    VSH_SRC_PROFILE_ADVANCED,    //Advanced profile
    //wmv
    VSH_SRC_PROFILE_PC    //PC Profile
}    VSH_SRC_PROFILE_TYPE_T;

/*VSH_SRC_LEVEL_TYPE_T settings **********************************************/
typedef enum
{
    VSH_SRC_LEVEL_UNKNOWN = 0,
    VSH_SRC_LEVEL_MAIN,    //Main level
    VSH_SRC_LEVEL_LOW,    //Low level
    VSH_SRC_LEVEL_HIGH,    //High level
    VSH_SRC_LEVEL_HIGH_1440,    //High 1440 level
    VSH_SRC_LEVEL_0,    //level 0
    VSH_SRC_LEVEL_1,    //level 1
    VSH_SRC_LEVEL_2,    //level 2
    VSH_SRC_LEVEL_3,    //level 3
    VSH_SRC_LEVEL_4,    //level 4
    VSH_SRC_LEVEL_5,    //level 5
    VSH_SRC_LEVEL_1_B,    //level 1b
    VSH_SRC_LEVEL_1_1,    //level 1.1
    VSH_SRC_LEVEL_1_2,    //level 1.2
    VSH_SRC_LEVEL_1_3,    //level 1.3
    VSH_SRC_LEVEL_2_1,    //level 2.1
    VSH_SRC_LEVEL_2_2,    //level 2.2
    VSH_SRC_LEVEL_3_1,    //level 3.1
    VSH_SRC_LEVEL_3_2,    //level 3.2
    VSH_SRC_LEVEL_4_1,    //level 4.1
    VSH_SRC_LEVEL_4_2,    //level 4.2
    VSH_SRC_LEVEL_5_1,    //level 5.1
    VSH_SRC_LEVEL_LL,        //level LL
    VSH_SRC_LEVEL_ML,        //level ML
    VSH_SRC_LEVEL_HL,        //level HL
    VSH_SRC_LEVEL_L_0,    //level L0
    VSH_SRC_LEVEL_L_1,    //level L1
    VSH_SRC_LEVEL_L_2,    //level L2
    VSH_SRC_LEVEL_L_3,    //level L3
    VSH_SRC_LEVEL_L_4        //level L4
}    VSH_SRC_LEVEL_TYPE_T;

typedef struct _VSH_SRC_PROFILE_LEVEL_INFO_T
{
    VSH_SRC_PROFILE_TYPE_T  e_profile_type;
    VSH_SRC_LEVEL_TYPE_T    e_level_type;
}   VSH_SRC_PROFILE_LEVEL_INFO_T;

typedef struct _VSH_DEC_WFD_ERR_INFO_T
{
    INT32 i4TimeoutCnt;
    INT32 i4LackSeqHeaderCnt;
    INT32 i4OutOfSpecCnt;
} VSH_DEC_WFD_ERR_INFO_T;



/* SM_VSH_GET_TYPE_VID_MIN_MAX info ******************************************/
/*------------------------------------------------------------------*/
/*! @struct VSH_VID_MIN_MAX_INFO_T
 *  @brief Contains VSH VID minimum maximum info.
 *  @code
 *  typedef struct _VSH_VID_MIN_MAX_INFO_T
 *  {
 *      UINT32                  e_get_type;    
 *      INT32                   i4_min_value;  
 *      INT32                   i4_max_value;  
 *  }   VSH_VID_MIN_MAX_INFO_T;
 *  @endcode
 *  @li@c  e_get_type                                        -type
 *  @li@c  i4_min_value                                      -minimum value
 *  @li@c  i4_max_value                                      -maximum value
 */
/*------------------------------------------------------------------*/
typedef struct _VSH_VID_MIN_MAX_INFO_T
{
    UINT32                  e_get_type;    /* IN */
    INT32                   i4_min_value;  /* OUT */
    INT32                   i4_max_value;  /* OUT */
}   VSH_VID_MIN_MAX_INFO_T;

#define VSH_MIN_MAX_INFO_WIDTH_MASK               ((INT32)0xffff)        /**< min and max width bit mask (0~15)    */
#define VSH_MIN_MAX_INFO_HEIGHT_MASK              ((INT32)0xffff0000)    /**< min and max height bit mask(16~31)   */

/*
#define VSH_MIN_MAX_INFO_GET_WIDTH( i4_value )                             \
    ((i4_value) & VSH_MIN_MAX_INFO_WIDTH_MASK)

#define VSH_MIN_MAX_INFO_GET_HEIGHT( i4_value )                            \
    (((i4_value) & VSH_MIN_MAX_INFO_HEIGHT_MASK)>>16)
*/

/* SM_VSH_GET_TYPE_VID_MIN_MAX info ******************************************/
/*------------------------------------------------------------------*/
/*! @struct VSH_CNT_ADV_INFO_T
 *  @brief Contains VSH content advisory information.
 *  @code
 *  typedef struct _VSH_CNT_ADV_INFO_T
 *  {
 *      UINT8         ui1_char_1;
 *      UINT8         ui1_char_2;
 *  }   VSH_CNT_ADV_INFO_T;
 *  @endcode
 *  @li@c  ui1_char_1                                        - char
 *  @li@c  ui1_char_2                                        - char
 */
/*------------------------------------------------------------------*/
typedef struct _VSH_CNT_ADV_INFO_T
{
    UINT8         ui1_char_1;
    UINT8         ui1_char_2;
}   VSH_CNT_ADV_INFO_T;

/* SM_VSH_SET_TYPE_VGA_TIMING_OPTION *****************************************/
/*------------------------------------------------------------------*/
/*! @struct VSH_VGA_TIMING_OPTION_T
 *  @brief Contains VSH VGA timing option.
 *  @code
 *  typedef struct _VSH_VGA_TIMING_OPTION_T
 *  {
 *      UINT16                    ui2_opt_index;      
 *      BOOL                      b_is_default;       
 *  }   VSH_VGA_TIMING_OPTION_T;
 *  @endcode
 *  @li@c  ui2_opt_index                                     -option index
 *  @li@c  b_is_default                                      -default or not
 */
/*------------------------------------------------------------------*/
typedef struct _VSH_VGA_TIMING_OPTION_T
{
    UINT16                    ui2_opt_index;      /* [IN] option index in group */
    BOOL                      b_is_default;       /* [IN] is default? */
}   VSH_VGA_TIMING_OPTION_T;


/* SM_VSH_GET_TYPE_AMBIG_VGA_TIMING info *************************************/
/*------------------------------------------------------------------*/
/*! @enum VSH_AMBIG_VGA_TIMING_TYPE_T
 *  @brief Specify VSH ambiguous VGA timing type.
 *  @code
 *  typedef enum 
 *  {
 *      VSH_AMBIG_VGA_TIMING_TYPE_NUM_IN_GRP,
 *      VSH_AMBIG_VGA_TIMING_TYPE_OPTION_IN_GRP,
 *      VSH_AMBIG_VGA_TIMING_TYPE_CURR_OPT_IN_GRP
 *  } VSH_AMBIG_VGA_TIMING_TYPE_T;
 *  @endcode
 *  @li@c  VSH_AMBIG_VGA_TIMING_TYPE_NUM_IN_GRP              -number in GRP
 *  @li@c  VSH_AMBIG_VGA_TIMING_TYPE_OPTION_IN_GRP           -option in GRP
 *  @li@c  VSH_AMBIG_VGA_TIMING_TYPE_CURR_OPT_IN_GRP         -current option in GRP 
 */
/*------------------------------------------------------------------*/
typedef enum 
{
    VSH_AMBIG_VGA_TIMING_TYPE_NUM_IN_GRP,
    VSH_AMBIG_VGA_TIMING_TYPE_OPTION_IN_GRP,
    VSH_AMBIG_VGA_TIMING_TYPE_CURR_OPT_IN_GRP
} VSH_AMBIG_VGA_TIMING_TYPE_T;

#define VSH_VGA_TIMING_FREQUENCY_UNKNOWN          ((UINT16)0)    /**<  VGA timing frequency default value  */

/*------------------------------------------------------------------*/
/*! @struct VSH_AMBIG_VGA_TIMING_OPTION_T
 *  @brief Contains VSH AMBIG VGA timing option.
 *  @code
 *  typedef struct _VSH_AMBIG_VGA_TIMING_OPTION_T
 *  {
 *      UINT16                    ui2_opt_index;      
 *      UINT16                    ui2_ver_frequncy;   
 *      UINT32                    ui4_hor_resolution; 
 *      UINT32                    ui4_ver_resolution; 
 *  }   VSH_AMBIG_VGA_TIMING_OPTION_T;
 *  @endcode
 *  @li@c  ui2_opt_index                                     -option index
 *  @li@c  ui2_ver_frequncy                                  -vertical frequency
 *  @li@c  ui4_hor_resolution                                -horizontal resolution
 *  @li@c  ui4_ver_resolution                                -vertical resolution
 */
/*------------------------------------------------------------------*/
typedef struct _VSH_AMBIG_VGA_TIMING_OPTION_T
{
    UINT16                    ui2_opt_index;      /* [IN/OUT] option index in group */
    UINT16                    ui2_ver_frequncy;   /* [OUT] vertical frequency */
    UINT32                    ui4_hor_resolution; /* [OUT] horizontal resolution */
    UINT32                    ui4_ver_resolution; /* [OUT] vertical resolution */
}   VSH_AMBIG_VGA_TIMING_OPTION_T;

/*------------------------------------------------------------------*/
/*! @struct VSH_AMBIG_VGA_TIMING_INFO_T
 *  @brief Contains VSH AMBIG VGA timing info.
 *  @code
 *  typedef struct _VSH_AMBIG_VGA_TIMING_INFO_T
 *  {
 *      VSH_AMBIG_VGA_TIMING_TYPE_T                   e_type;  
 *      union {
 *          UINT16                                    ui2_num; 
 *          VSH_AMBIG_VGA_TIMING_OPTION_T             t_opt;   
 *      } u;
 *  }   VSH_AMBIG_VGA_TIMING_INFO_T;
 *  @endcode
 *  @li@c  e_type                                            -type
 *  @li@c  ui2_num                                           -number
 *  @li@c  t_opt                                             -timing option
 */
/*------------------------------------------------------------------*/
typedef struct _VSH_AMBIG_VGA_TIMING_INFO_T
{
    VSH_AMBIG_VGA_TIMING_TYPE_T                   e_type;  /* [IN] */

    union {
        UINT16                                    ui2_num; /* [OUT] */
        VSH_AMBIG_VGA_TIMING_OPTION_T             t_opt;   /* [OUT] */
    } u;
}   VSH_AMBIG_VGA_TIMING_INFO_T;

/* SM_VSH_SET_TYPE_COLOR_SYS/SM_VSH_GET_TYPE_COLOR_SYS ***********************/
/*------------------------------------------------------------------*/
/*! @enum VSH_COLOR_SYS_TYPE_T
 *  @brief Specify VSH color system type
 *  @code
 *  typedef enum
 *  {
 *      VSH_COLOR_SYS_TYPE_UNKNOWN = -1, 
 *      VSH_COLOR_SYS_TYPE_NTSC,
 *      VSH_COLOR_SYS_TYPE_NTSC_443,
 *      VSH_COLOR_SYS_TYPE_PAL,
 *      VSH_COLOR_SYS_TYPE_PAL_N,
 *      VSH_COLOR_SYS_TYPE_PAL_M,
 *      VSH_COLOR_SYS_TYPE_PAL_60,
 *      VSH_COLOR_SYS_TYPE_SECAM
 *  }   VSH_COLOR_SYS_TYPE_T;
 *  @endcode
 *  @li@c  VSH_COLOR_SYS_TYPE_UNKNOWN                        -unknown
 *  @li@c  VSH_COLOR_SYS_TYPE_NTSC                           -NTSC
 *  @li@c  VSH_COLOR_SYS_TYPE_NTSC_443                       -NTSC 443
 *  @li@c  VSH_COLOR_SYS_TYPE_PAL                            -PAL
 *  @li@c  VSH_COLOR_SYS_TYPE_PAL_N                          -PAL N
 *  @li@c  VSH_COLOR_SYS_TYPE_PAL_M                          -PAL M
 *  @li@c  VSH_COLOR_SYS_TYPE_PAL_60                         -PAL 60
 *  @li@c  VSH_COLOR_SYS_TYPE_SECAM                          -SECAM
 */
/*------------------------------------------------------------------*/
typedef enum
{
    VSH_COLOR_SYS_TYPE_UNKNOWN = -1, /* auto */
    VSH_COLOR_SYS_TYPE_NTSC,
    VSH_COLOR_SYS_TYPE_NTSC_443,
    VSH_COLOR_SYS_TYPE_PAL,
    VSH_COLOR_SYS_TYPE_PAL_N,
    VSH_COLOR_SYS_TYPE_PAL_M,
    VSH_COLOR_SYS_TYPE_PAL_60,
    VSH_COLOR_SYS_TYPE_SECAM
}   VSH_COLOR_SYS_TYPE_T;

/*------------------------------------------------------------------*/
/*! @struct VSH_ANA_TV_SIG_STATUS_T
 *  @brief Contains analog signal status from tvd.
 *  @code
 *  typedef enum
 *  {
 *      VSH_ANA_TV_SIG_UNKNOWN = 0,    
 *      VSH_ANA_TV_SIG_NONE, 
 *      VSH_ANA_TV_SIG_STABLE
 *  }   VSH_ANA_TV_SIG_STATUS_T;
 *  @endcode
 *  @li@c  VSH_ANA_TV_SIG_UNKNOWN               -signal is detecting.
 *  @li@c  VSH_ANA_TV_SIG_NONE                      -no signal.
 *  @li@c  VSH_ANA_TV_SIG_STABLE                    -has signal.
 */
/*------------------------------------------------------------------*/
typedef enum
{
    VSH_ANA_TV_SIG_UNKNOWN = 0,
    VSH_ANA_TV_SIG_NONE,
    VSH_ANA_TV_SIG_STABLE
}   VSH_ANA_TV_SIG_STATUS_T;

/* SM_VSH_GET_TYPE_DIG_DECODE_STATUS *****************************************/
/*------------------------------------------------------------------*/
/*! @enum VSH_DIG_DECODE_STATUS_T
 *  @brief Specify VSH digit decode status.
 *  @code
 *  typedef enum
 *  {
 *      VSH_DIG_DECODE_UNKNOWN = -1,
 *      VSH_DIG_DECODE_NO_DATA,
 *      VSH_DIG_DECODE_NORMAL,
 *      VSH_DIG_DECODE_ERROR,
 *      VSH_DIG_DECODE_HD_NOT_SUPPORT,
 *      VSH_DIG_DECODE_CODEC_NOT_SUPPORT
 *      VSH_DIG_DECODE_BITSTREAM_NOT_SUPPORT,
 *      VSH_DIG_DECODE_CODEC_TYPE_CHG,
 *      VSH_DIG_DECODE_DATA_RECEIVE,
 *      VSH_DIG_DECODE_RES_NOT_SUPPORT
 *  }   VSH_DIG_DECODE_STATUS_T;
 *  @endcode
 *  @li@c  VSH_DIG_DECODE_UNKNOWN                            -unknown
 *  @li@c  VSH_DIG_DECODE_NO_DATA                            -no data
 *  @li@c  VSH_DIG_DECODE_NORMAL                             -decode normal
 *  @li@c  VSH_DIG_DECODE_ERROR                              -decode error
 *  @li@c  VSH_DIG_DECODE_HD_NOT_SUPPORT            -HD not support
 *  @li@c  VSH_DIG_DECODE_CODEC_NOT_SUPPORT      -codec not support
 *  @li@c  VSH_DIG_DECODE_BITSTREAM_NOT_SUPPORT  -bit stream not support
 *  @li@c  VSH_DIG_DECODE_CODEC_TYPE_CHG            -codec type changed
 *  @li@c  VSH_DIG_DECODE_DATA_RECEIVE                  -decoder receive data
 *  @li@c  VSH_DIG_DECODE_RES_NOT_SUPPORT           -resolution not support
 */
/*------------------------------------------------------------------*/
typedef enum
{
    VSH_DIG_DECODE_UNKNOWN = -1,
    VSH_DIG_DECODE_NO_DATA,
    VSH_DIG_DECODE_NORMAL,
    VSH_DIG_DECODE_ERROR,
    VSH_DIG_DECODE_HD_NOT_SUPPORT,
    VSH_DIG_DECODE_CODEC_NOT_SUPPORT,
    VSH_DIG_DECODE_BITSTREAM_NOT_SUPPORT,
    VSH_DIG_DECODE_CODEC_TYPE_CHG,
    VSH_DIG_DECODE_DATA_RECEIVE,
    VSH_DIG_DECODE_RES_NOT_SUPPORT 
}   VSH_DIG_DECODE_STATUS_T;

/* SM_VSH_SET_TYPE_SEQUENCE_INFO *********************************************/
#define VSH_SRC_WIDTH_UNKNOWN                         ((UINT16) 0xFFFF)    /**<  width defaultl value   */
#define VSH_SRC_HEIGHT_UNKNOWN                        ((UINT16) 0xFFFF)    /**<  height defaultl value  */

/*------------------------------------------------------------------*/
/*! @enum VSH_SRC_FRAME_RATE_T
 *  @brief Specify VSH source frame rate.
 *  @code
 *  typedef enum
 *  {
 *      VSH_SRC_FRAME_RATE_UNKNOWN = 0,
 *      VSH_SRC_FRAME_RATE_23_976, 
 *      VSH_SRC_FRAME_RATE_24,
 *      VSH_SRC_FRAME_RATE_25,
 *      VSH_SRC_FRAME_RATE_29_97, 
 *      VSH_SRC_FRAME_RATE_30,
 *      VSH_SRC_FRAME_RATE_50,
 *      VSH_SRC_FRAME_RATE_59_94, 
 *      VSH_SRC_FRAME_RATE_60,
 *      VSH_SRC_FRAME_RATE_120,
 *      VSH_SRC_FRAME_RATE_1,
 *      VSH_SRC_FRAME_RATE_5,
 *      VSH_SRC_FRAME_RATE_8,
 *      VSH_SRC_FRAME_RATE_10,
 *      VSH_SRC_FRAME_RATE_12,
 *      VSH_SRC_FRAME_RATE_15,
 *      VSH_SRC_FRAME_RATE_2,
 *      VSH_SRC_FRAME_RATE_6,
 *      VSH_SRC_FRAME_RATE_48,
 *      VSH_SRC_FRAME_RATE_70,
 *      VSH_SRC_FRAME_RATE_20,
 *      VSH_SRC_FRAME_RATE_VARIABLE
 *  } VSH_SRC_FRAME_RATE_T;
 *  @endcode
 *  @li@c  VSH_SRC_FRAME_RATE_UNKNOWN                        -unknown
 *  @li@c  VSH_SRC_FRAME_RATE_23_976                         -23_976
 *  @li@c  VSH_SRC_FRAME_RATE_24                             -24
 *  @li@c  VSH_SRC_FRAME_RATE_25                             -25
 *  @li@c  VSH_SRC_FRAME_RATE_29_97                          -29_97
 *  @li@c  VSH_SRC_FRAME_RATE_30                             -30
 *  @li@c  VSH_SRC_FRAME_RATE_50                             -50
 *  @li@c  VSH_SRC_FRAME_RATE_59_94                          -59_94
 *  @li@c  VSH_SRC_FRAME_RATE_60                             -60
 *  @li@c  VSH_SRC_FRAME_RATE_120                            -120
 *  @li@c  VSH_SRC_FRAME_RATE_1                              -1
 *  @li@c  VSH_SRC_FRAME_RATE_5                              -5
 *  @li@c  VSH_SRC_FRAME_RATE_8                              -8
 *  @li@c  VSH_SRC_FRAME_RATE_10                             -10
 *  @li@c  VSH_SRC_FRAME_RATE_12                             -12
 *  @li@c  VSH_SRC_FRAME_RATE_15                             -15
 *  @li@c  VSH_SRC_FRAME_RATE_2                              -2
 *  @li@c  VSH_SRC_FRAME_RATE_6                              -6
 *  @li@c  VSH_SRC_FRAME_RATE_48                             -48
 *  @li@c  VSH_SRC_FRAME_RATE_70                             -70
 *  @li@c  VSH_SRC_FRAME_RATE_20                             -20
 *  @li@c  VSH_SRC_FRAME_RATE_VARIABLE                       -variable
 */
/*------------------------------------------------------------------*/
typedef enum
{
    VSH_SRC_FRAME_RATE_UNKNOWN = 0,
    VSH_SRC_FRAME_RATE_23_976, /* 24000/1001 (23.976...) */
    VSH_SRC_FRAME_RATE_24,
    VSH_SRC_FRAME_RATE_25,
    VSH_SRC_FRAME_RATE_29_97, /* 30000/1001 (29.97...) */
    VSH_SRC_FRAME_RATE_30,
    VSH_SRC_FRAME_RATE_50,
    VSH_SRC_FRAME_RATE_59_94, /* 60000/1001 (59.94...) */
    VSH_SRC_FRAME_RATE_60,
    VSH_SRC_FRAME_RATE_120,
    VSH_SRC_FRAME_RATE_1,
    VSH_SRC_FRAME_RATE_5,
    VSH_SRC_FRAME_RATE_8,
    VSH_SRC_FRAME_RATE_10,
    VSH_SRC_FRAME_RATE_12,
    VSH_SRC_FRAME_RATE_15,
    VSH_SRC_FRAME_RATE_2,
    VSH_SRC_FRAME_RATE_6,
    VSH_SRC_FRAME_RATE_48,
    VSH_SRC_FRAME_RATE_70,
    VSH_SRC_FRAME_RATE_20,
    VSH_SRC_FRAME_RATE_VARIABLE
} VSH_SRC_FRAME_RATE_T;

/*------------------------------------------------------------------*/
/*! @struct VSH_ASP_USR_TYPE_T
 *  @brief Contains source or display aspectio.
 *  @code
 *  typedef enum
 *  {
 *      VSH_ASP_USR_TYPE_SRC = 1,    
 *      VSH_ASP_USR_TYPE_DISP, 
 *  }   VSH_ANA_TV_SIG_STATUS_T;
 *  @endcode
 *  @li@c  VSH_ASP_USR_TYPE_SRC               -source aspect ratio.
 *  @li@c  VSH_ASP_USR_TYPE_DISP              -display aspect ratio.
 */
/*------------------------------------------------------------------*/
typedef enum
{    
    VSH_ASP_USR_TYPE_SRC = 1,   
    VSH_ASP_USR_TYPE_DISP    
} VSH_ASP_USR_TYPE_T;

/*------------------------------------------------------------------*/
/*! @struct VSH_SEQUENCE_DATA_T
 *  @brief Contains VSH sequence data.
 *  @code
 *  typedef struct _VSH_SEQUENCE_DATA_T
 *  {
 *      UINT16                     ui2_width;      
 *      UINT16                     ui2_height;     
 *      VSH_SRC_FRAME_RATE_T       e_frame_rate;   
 *      VSH_SRC_ASPECT_RATIO_T     e_src_asp;      
 *      UINT32                     ui4_seq_hdr;
 *      UINT16                     ui2_seq_hdr_sz;        
 *      UINT8                      ui1_sample_id;
 *  }   VSH_SEQUENCE_DATA_T;
 *  @endcode
 *  @li@c  ui2_width                                         -width
 *  @li@c  ui2_height                                        -height
 *  @li@c  e_frame_rate                                      -frame rate
 *  @li@c  e_src_asp                                         -source aspect
 *  @li@c  ui4_seq_hdr                                       -sequence handler
 *  @li@c  ui2_seq_hdr_sz                                    -sequence handler size
 *  @li@c  ui1_sample_id                                     -sample index
 */
/*------------------------------------------------------------------*/
typedef struct _VSH_SEQUENCE_DATA_T
{
    UINT16                     ui2_width;      /* horizontal_size_value */
    UINT16                     ui2_height;     /* vertical_size_value */
    VSH_SRC_FRAME_RATE_T       e_frame_rate;   /* Frame Rate */
    VSH_SRC_ASPECT_RATIO_T     e_src_asp;      /* Display Aspect Ratio (DAR) */
    VSH_ASP_USR_TYPE_T         e_asp_usr_type; /* source or display asp */
    UINT16                     ui2_asp_usr_h;
    UINT16                     ui2_asp_usr_w;      
    UINT32                     ui4_seq_hdr;
    UINT16                     ui2_seq_hdr_sz;        
    UINT8                      ui1_sample_id;
    BOOL                       b_chk_framerate;
    BOOL                       b_divx_plus;
}   VSH_SEQUENCE_DATA_T;

/*------------------------------------------------------------------*/
/*! @struct VSH_SEQUENCE_RAW_T
 *  @brief Contains VSH sequence raw.
 *  @code
 *  typedef struct _VSH_SEQUENCE_RAW_T
 *  {
 *      UCHAR*                     pc_buff;
 *      SIZE_T                     z_buff_size;
 *      VSH_SRC_FRAME_RATE_T       e_frame_rate;   
 *      UINT8                      ui1_sample_id;
 *  }   VSH_SEQUENCE_RAW_T;
 *  @endcode
 *  @li@c  pc_buff                                           -buffer
 *  @li@c  z_buff_size                                       -buffer size
 *  @li@c  e_frame_rate                                      -frame rate
 *  @li@c  ui1_sample_id                                     -sample index
 */
/*------------------------------------------------------------------*/
typedef struct _VSH_SEQUENCE_RAW_T
{
    UCHAR*                     pc_buff;
    SIZE_T                     z_buff_size;
    VSH_SRC_FRAME_RATE_T       e_frame_rate;  
    UINT8                      ui1_sample_id;
}   VSH_SEQUENCE_RAW_T;

/*------------------------------------------------------------------*/
/*! @struct VSH_SEQUENCE_INFO_T
 *  @brief Contains VSH sequence info.
 *  @code
 *  typedef struct _VSH_SEQUENCE_INFO_T
 *  {
 *      BOOL                       b_is_raw;
 *      union
 *      {
 *          VSH_SEQUENCE_RAW_T         t_seq_raw;
 *          VSH_SEQUENCE_DATA_T        t_seq_data;
 *      } u;
 *      struct _VSH_SEQUENCE_INFO_T*  pt_next_scale;
 *  }   VSH_SEQUENCE_INFO_T;
 *  @endcode
 *  @li@c  b_is_raw                                          -raw data or not
 *  @li@c  t_seq_raw                                         -sequence raw data
 *  @li@c  t_seq_data                                        -sequence data
 *  @li@c  pt_next_scale                                     -next scale
 */
/*------------------------------------------------------------------*/

typedef struct _VSH_SEQUENCE_INFO_T
{
    BOOL                       b_is_raw;
    BOOL                       b_is_second_raw;

    union
    {
        VSH_SEQUENCE_RAW_T         t_seq_raw;
        VSH_SEQUENCE_DATA_T        t_seq_data;
    } u;

    struct _VSH_SEQUENCE_INFO_T*  pt_next_scale;
}   VSH_SEQUENCE_INFO_T;


/* SM_VSH_GET_TYPE_PICTURE_INFO **********************************************/
/*------------------------------------------------------------------*/
/*! @enum VSH_PIC_INFO_TYPE_T
 *  @brief Specify VSH picture info type.
 *  @code
 *  typedef enum
 *  {
 *      VSH_PIC_INFO_TYPE_UNKNOWN= 0,
 *      VSH_PIC_INFO_TYPE_GOP,
 *      VSH_PIC_INFO_TYPE_PICTURE
 *  }   VSH_PIC_INFO_TYPE_T;
 *  @endcode
 *  @li@c  VSH_PIC_INFO_TYPE_UNKNOWN                         -unknown
 *  @li@c  VSH_PIC_INFO_TYPE_GOP                             -GOP
 *  @li@c  VSH_PIC_INFO_TYPE_PICTURE                         -picture
 */
/*------------------------------------------------------------------*/
typedef enum
{
    VSH_PIC_INFO_TYPE_UNKNOWN= 0,
    VSH_PIC_INFO_TYPE_GOP,
    VSH_PIC_INFO_TYPE_PICTURE
}   VSH_PIC_INFO_TYPE_T;

/*------------------------------------------------------------------*/
/*! @struct VSH_TIME_CODE_INFO_T
 *  @brief Contains VSH time code info.
 *  @code
 *  typedef struct _VSH_TIME_CODE_INFO_T
 *  {
 *      UINT8                     ui1_hours;    
 *      UINT8                     ui1_minutes;  
 *      UINT8                     ui1_seconds;  
 *      UINT8                     ui1_pictures; 
 *      INT32                     i4_total_dur_adj;    
 *  }   VSH_TIME_CODE_INFO_T;
 *  @endcode
 *  @li@c  ui1_hours                                         -hours
 *  @li@c  ui1_minutes                                       -minutes
 *  @li@c  ui1_seconds                                       -seconds
 *  @li@c  ui1_pictures                                      -pictures
 *  @li@c  i4_total_dur_adj                                  -total duration adjustment
 */
/*------------------------------------------------------------------*/
typedef struct _VSH_TIME_CODE_INFO_T
{
    UINT8                     ui1_hours;    /* 0 - 23 */
    UINT8                     ui1_minutes;  /* 0 - 59 */
    UINT8                     ui1_seconds;  /* 0 - 59 */
    UINT8                     ui1_pictures; /* 0 - 59 */
    INT32                     i4_total_dur_adj;    
}   VSH_TIME_CODE_INFO_T;

/*------------------------------------------------------------------*/
/*! @struct VSH_PICTURE_INFO_T
 *  @brief Contains VSH picture info.
 *  @code
 *  typedef struct _VSH_PICTURE_INFO_T
 *  {
 *      VSH_PIC_INFO_TYPE_T       e_type;
 *      UINT64                    ui8_offset;
 *      union {
 *          VSH_TIME_CODE_INFO_T      t_time_code;
 *          UINT64                    ui8_pts;
 *      } u;
 *  }   VSH_PICTURE_INFO_T;
 *  @endcode
 *  @li@c  e_type                                            -type
 *  @li@c  ui8_offset                                        -offset
 *  @li@c  t_time_code                                       -time code
 *  @li@c  ui8_pts                                           -PTS
 */
/*------------------------------------------------------------------*/
typedef struct _VSH_PICTURE_INFO_T
{
    VSH_PIC_INFO_TYPE_T       e_type;
    UINT64                    ui8_offset;
    union {
        VSH_TIME_CODE_INFO_T      t_time_code;
        UINT64                    ui8_pts;
    } u;
}   VSH_PICTURE_INFO_T;


/*for test*/
/*------------------------------------------------------------------*/
/*! @struct VSH_PROGRESS_SEQ_INFO
 *  @brief VSH_PROGRESS_SEQ_INFO.
 *  @code
 *  typedef struct _VSH_PROGRESS_SEQ_INFO_T
    {
        UINT32              ui4_progress_seq;
    }VSH_PROGRESS_SEQ_INFO_T;


 *  @li@c  e_type                                            -type
 *  @li@c  ui8_offset                                        -offset
 *  @li@c  t_time_code                                       -time code
 *  @li@c  ui8_pts                                           -PTS
 */
/*------------------------------------------------------------------*/
typedef struct _VSH_PROGRESS_SEQ_INFO_T
{
    UINT32              ui4_progress_seq;
}VSH_PROGRESS_SEQ_INFO_T;

/* SM_VSH_GET_TYPE_THUMBNAIL *************************************************/
/* color mode */
/*------------------------------------------------------------------*/
/*! @enum VSH_COLORMODE_T
 *  @brief Specify VSH color mode.
 *  @code
 *  typedef enum
 *  {
 *      VSH_COLORMODE_AYUV_CLUT2     = 0,
 *      VSH_COLORMODE_AYUV_CLUT4     = 1,
 *      VSH_COLORMODE_AYUV_CLUT8     = 2,
 *      VSH_COLORMODE_UYVY_16        = 3,
 *      VSH_COLORMODE_YUYV_16        = 4,
 *      VSH_COLORMODE_AYUV_D8888     = 5,
 *      VSH_COLORMODE_ARGB_CLUT2     = 6,
 *      VSH_COLORMODE_ARGB_CLUT4     = 7,
 *      VSH_COLORMODE_ARGB_CLUT8     = 8,
 *      VSH_COLORMODE_RGB_D565       = 9,
 *      VSH_COLORMODE_ARGB_D1555     = 10,
 *      VSH_COLORMODE_ARGB_D4444     = 11,
 *      VSH_COLORMODE_ARGB_D8888     = 12,
 *      VSH_COLORMODE_YUV_420_BLK    = 13,
 *      VSH_COLORMODE_YUV_420_RS     = 14,
 *      VSH_COLORMODE_YUV_422_BLK    = 15,
 *      VSH_COLORMODE_YUV_422_RS     = 16,
 *      VSH_COLORMODE_YUV_444_BLK    = 17,
 *      VSH_COLORMODE_YUV_444_RS     = 18
 *  } VSH_COLORMODE_T;
 *  @endcode
 *  @li@c  VSH_COLORMODE_AYUV_CLUT2                          -AYUV_CLUT2
 *  @li@c  VSH_COLORMODE_AYUV_CLUT4                          -AYUV_CLUT4
 *  @li@c  VSH_COLORMODE_AYUV_CLUT8                          -AYUV_CLUT8
 *  @li@c  VSH_COLORMODE_UYVY_16                             -UYVY_16
 *  @li@c  VSH_COLORMODE_YUYV_16                             -YUYV_16
 *  @li@c  VSH_COLORMODE_AYUV_D8888                          -AYUV_D8888
 *  @li@c  VSH_COLORMODE_ARGB_CLUT2                          -ARGB_CLUT2
 *  @li@c  VSH_COLORMODE_ARGB_CLUT4                          -ARGB_CLUT4
 *  @li@c  VSH_COLORMODE_ARGB_CLUT8                          -ARGB_CLUT8
 *  @li@c  VSH_COLORMODE_RGB_D565                            -RGB_D565
 *  @li@c  VSH_COLORMODE_ARGB_D1555                          -ARGB_D1555
 *  @li@c  VSH_COLORMODE_ARGB_D4444                          -ARGB_D4444
 *  @li@c  VSH_COLORMODE_ARGB_D8888                          -ARGB_D8888
 *  @li@c  VSH_COLORMODE_YUV_420_BLK                         -YUV_420_BLK
 *  @li@c  VSH_COLORMODE_YUV_420_RS                          -YUV_420_RS
 *  @li@c  VSH_COLORMODE_YUV_422_BLK                         -YUV_422_BLK
 *  @li@c  VSH_COLORMODE_YUV_422_RS                          -YUV_422_RS
 *  @li@c  VSH_COLORMODE_YUV_444_BLK                         -YUV_444_BLK
 *  @li@c  VSH_COLORMODE_YUV_444_RS                          -YUV_444_RS
 */
/*------------------------------------------------------------------*/
typedef enum
{
    VSH_COLORMODE_AYUV_CLUT2     = 0,
    VSH_COLORMODE_AYUV_CLUT4     = 1,
    VSH_COLORMODE_AYUV_CLUT8     = 2,
    VSH_COLORMODE_UYVY_16        = 3,
    VSH_COLORMODE_YUYV_16        = 4,
    VSH_COLORMODE_AYUV_D8888     = 5,
    VSH_COLORMODE_ARGB_CLUT2     = 6,
    VSH_COLORMODE_ARGB_CLUT4     = 7,
    VSH_COLORMODE_ARGB_CLUT8     = 8,
    VSH_COLORMODE_RGB_D565       = 9,
    VSH_COLORMODE_ARGB_D1555     = 10,
    VSH_COLORMODE_ARGB_D4444     = 11,
    VSH_COLORMODE_ARGB_D8888     = 12,
    VSH_COLORMODE_YUV_420_BLK    = 13,
    VSH_COLORMODE_YUV_420_RS     = 14,
    VSH_COLORMODE_YUV_422_BLK    = 15,
    VSH_COLORMODE_YUV_422_RS     = 16,
    VSH_COLORMODE_YUV_444_BLK    = 17,
    VSH_COLORMODE_YUV_444_RS     = 18
} VSH_COLORMODE_T;

/*------------------------------------------------------------------*/
/*! @struct VSH_THUMBNAIL_INFO_T
 *  @brief Contains VSH(video stream handles) thumbnail info.
 *  @code
 *  typedef struct _VSH_THUMBNAIL_INFO_T
 *  {
 *      UCHAR*                    pc_canvas_buffer;
 *      VSH_COLORMODE_T           e_canvas_colormode;
 *      UINT32                    ui4_canvas_width;     
 *      UINT32                    ui4_canvas_height;    
 *      UINT32                    ui4_canvas_pitch;     
 *      UINT32                    ui4_thumbnail_x;      
 *      UINT32                    ui4_thumbnail_y;      
 *      UINT32                    ui4_thumbnail_width;  
 *      UINT32                    ui4_thumbnail_height; 
 *  }   VSH_THUMBNAIL_INFO_T;
 *  @endcode
 *  @li@c  pc_canvas_buffer                                  -canvas buffer
 *  @li@c  e_canvas_colormode                                -canvas color mode
 *  @li@c  ui4_canvas_width                                  -canvas width
 *  @li@c  ui4_canvas_height                                 -canvas height
 *  @li@c  ui4_canvas_pitch                                  -canvas pitch
 *  @li@c  ui4_thumbnail_x                                   -thumbnail start x
 *  @li@c  ui4_thumbnail_y                                   -thumbnail start y
 *  @li@c  ui4_thumbnail_width                               -thumbnail width
 *  @li@c  ui4_thumbnail_height                              -thumbnail height
 */
/*------------------------------------------------------------------*/
typedef struct _VSH_THUMBNAIL_INFO_T
{
    UCHAR*                    pc_canvas_buffer;
    VSH_COLORMODE_T           e_canvas_colormode;
    UINT32                    ui4_canvas_width;     /* in pixels */
    UINT32                    ui4_canvas_height;    /* in pixels */
    UINT32                    ui4_canvas_pitch;     /* in bytes */

    UINT32                    ui4_thumbnail_x;      /* in pixels */
    UINT32                    ui4_thumbnail_y;      /* in pixels */
    UINT32                    ui4_thumbnail_width;  /* in pixels */
    UINT32                    ui4_thumbnail_height; /* in pixels */
}   VSH_THUMBNAIL_INFO_T;

/* For AP Get thumbnail data*/
typedef struct
{
    UCHAR*                          u1CanvasBuffer;
    UINT32                          u4BufLen;
    VSH_COLORMODE_T                 eCanvasColormode;
    UINT32                          u4ThumbnailWidth;  /* in pixels */
    UINT32                          u4ThumbnailHeight; /* in pixels */
}THUMBNAIL_GET_INFO_T;

/* audio protect infomation */
typedef enum
{
    VSH_AUD_PROTECT_INFO_UNKNOWN = -1,
    VSH_AUD_PROTECT_INFO_DMUTE_AMUTE,         /* Digtal mute and analog mute */
    VSH_AUD_PROTECT_INFO_DUNMUTE_AMUTE,       /* Digtal unmute and analog mute */
    VSH_AUD_PROTECT_INFO_DUNMUTE_AUNMUTE,     /* Digtal unmute and analog unmute */
    VSH_AUD_PROTECT_INFO_DMUTE_AUNMUTE        /* Digtal mute and analog unmute */
}   VSH_AUD_PROTECT_INFO_T;

/*-----------------------------------------------------------------------------
                    functions declarations
-----------------------------------------------------------------------------*/

#endif /* _U_SM_VIDEO_HDLR_H_ */
/*----------------------------------------------------------------------------*/
/*! @} */   /* end of groupMW_STRM_MNGR_VIDEO_HDLR */
/*----------------------------------------------------------------------------*/

