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
/*! @file 3d_photo_playback.h 
 *  $RCSfile: $
 *  $Revision: #1 $
 *  $Date: 2015/01/09 $
 *  $Author: dtvbm11 $
 *  
 *  @par Description:
 *         This file exports all image library API and constant for 
 *         other module using.
 */
/*----------------------------------------------------------------------------*/

#ifndef _MPO_PHOTO_PLAYBACK_H_
#define _MPO_PHOTO_PLAYBACK_H_


#include "x_gl.h"
#include "x_img.h"
#include "x_wgl.h"
#include "x_dbg.h"
#include "x_img_tfx.h"


#define IMG_RC_DISPLAY_NAME                 SN_PRES_MAIN_DISPLAY


#define GUI_PLANE_ID                        0

#define _IMG_DRC_FLAG_READY               ((UINT8) 0x01)
#define _IMG_DRC_FLAG_USED                   ((UINT8) 0x02)


#define _IMG_PE_ACCURACY_CONST              ((UINT32)10000)
#define ZOOM_CONST  100
#define ZOOM_MIN    100
#define ZOOM_MAX    400

#define PAN_CONST   100
#define PAN_MIN     -100
#define PAN_MAX     100


typedef struct _IMG_CROP_RECT_T
{
    UINT32  ui4_left_offset;
    UINT32  ui4_right_offset;
    UINT32  ui4_top_offset;
    UINT32  ui4_bottom_offset;
}IMG_CROP_RECT_T;


typedef struct _IMG_DRC_BUF_T
{
    GL_HVIEW_T                  h_view;                         /*view handle*/
    GL_HSURFACE_T               h_surf;                         /*surface handle*/
    IMG_ROTATE_OP_TYPE_T        e_rotate;                       /*rotation of this display*/
    UINT16                      ui2_zoom;
    GL_POINT_T                  t_center_offset;
    UINT32                      ui4_scale;                       /*scale of this display*/
    UINT32                      ui4_x;
    UINT32                      ui4_y;
    UINT32                      ui4_w;
    UINT32                      ui4_h;
    UINT32                      ui4_buf_width;
    UINT32                      ui4_buf_height;

    IMG_CROP_RECT_T             t_surface_crop_rect;
}   IMG_DRC_BUF_T;

typedef struct _IMG_DRC_DISPLAY_RES_T
{
    IMG_DRC_BUF_T               t_drc_buf;
    UINT32                      ui4_viewport;     /*viewport of this display*/
    UINT8                       ui1_flag;
}   IMG_DRC_DISPLAY_RES_T;
typedef enum{
    E_3D_JPS,
    E_3D_PNS,
    E_3D_BMS,
    E_3D_MPO,
    E_3D_UNKNOWN
}E_3D_IMG_TYPE;

typedef struct _IMG_METADATA_T
{
    UINT32                      ui4_src_width;
    UINT32                      ui4_src_height;
}IMG_METADATA_T;


typedef struct
{
    HANDLE_T                    h_img;
    IMG_DRC_BUF_T*              pt_buf_crnt_decode;
    IMG_DRC_BUF_T*              pt_buf_crnt_rotate;
    IMG_DRC_BUF_T*              pt_buf_crnt_zoom;
    IMG_DRC_BUF_T*              pt_buf_animation_gif;
    IMG_DRC_BUF_T*              pt_buf_tfx;
    BOOL                        b_is_4K2K;
    E_3D_IMG_TYPE               e_file_type;
}IMG_FILE_BUF_T;


#define FILE_CURRENT_IDX    0
#define FILE_NEXT_IDX       1
#define FILE_TOTAL_IDX      2

#define SVCTX_NAME_MAIN     "main_svctx"


#define _IMG_DRC_DISPLAY_RES_MAX_COUNT       ((UINT8) 4)


typedef struct _IMG_DRC_DATA_T
{
    GL_HSCREEN_T                h_screen;
    GL_HPLANE_T                 h_plane;
    UINT32                      ui4_screen_w;
    UINT32                      ui4_screen_h;
    UINT32                      ui4_canvas_w;
    UINT32                      ui4_canvas_h;
    INT8                        i1_img_z_order;
    UINT32                      ui4_flag;
    GL_POINT_T                  t_screen_offset;
    IMG_DRC_DISPLAY_RES_T      at_disp_pool[_IMG_DRC_DISPLAY_RES_MAX_COUNT];
    IMG_DRC_BUF_T*              pt_buf_crnt_disp;
    IMG_FILE_BUF_T              at_file_info[FILE_TOTAL_IDX];

    BOOL                        b_init;

    IMG_METADATA_T              t_image_metadata;

    HANDLE_T                    h_scc_main_vid;
    BOOL                        b_enable_3d;
    HANDLE_T                    h_svctx;

    HANDLE_T                    h_tfx;
    INT32                       i4_status;
    HANDLE_T                    h_tfx_lock;
    BOOL                        b_effect;
    HANDLE_T                    h_mutex_lock;
    BOOL                        b_is_4k2k_panel;
}   IMG_DRC_DATA_T;

typedef struct
{
    INT32 i4_status;
    VOID* pv_data;
}IMG_TFX_MSG_T;

#define TYPE_OFFSET 100
typedef enum
{
    IMG_NFY_DEC_HANDLE_CLOSED_TYPE = TYPE_OFFSET,
    IMG_NFY_DEC_LOAD_COMPLETE_TYPE,
    IMG_NFY_DEC_LOAD_COMPLETE_WITH_TAG,
    IMG_NFY_DEC_GIF_ONE_FRM_LOAD_COMPLETE_TYPE,
    IMG_NTY_DEC_GIF_LOAD_STOP
} IMG_DEC_NFY_EVENT_TYPE_T;


extern INT32 x_img_playback_rc_init(VOID);
extern INT32 x_img_playback_rc_deinit(VOID);
extern INT32 x_img_playback_open(const CHAR* ps_file_name, x_img_nfy_fct   pf_nfy, const VOID* pv_tag, HANDLE_T* ph_img);
extern INT32 x_img_playback_decode(HANDLE_T h_img);
extern INT32 x_img_playback_display(HANDLE_T h_img);
extern INT32 x_img_playback_close(HANDLE_T h_img);
extern INT32 x_img_playback_get(HANDLE_T h_img, IMG_INFO_TYPE_T e_info_type, VOID* pv_info, UINT32* pui4_len);
extern INT32 x_img_playback_set_3d(HANDLE_T h_img, BOOL b_enable_3d);
extern INT32 x_img_playback_rotate(HANDLE_T h_img, IMG_ROTATE_OP_TYPE_T e_rotate_type);
extern INT32  x_img_playback_zoom(HANDLE_T h_img, UINT16 ui2_zoom);
extern INT32  x_img_playback_pan(HANDLE_T h_img, INT8 i1_horzontal, INT8  i1_vertical);
extern INT32 x_img_playback_tfx_play(HANDLE_T h_img, IMG_TFX_EFFECT_T    e_effect);
extern INT32 x_img_playback_tfx_stop(VOID);
extern INT32 x_img_playback_get_thumbnail(
    const CHAR*         ps_file_name,
    UINT32              ui4_width, 
    UINT32              ui4_height, 
    IMG_COLORMODE_T     e_cm, 
    VOID*               pv_addr,
    UINT32              ui4_buf_len);

#endif

