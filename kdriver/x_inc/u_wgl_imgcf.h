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
#ifndef _U_WGL_IMGCF_H_
#define _U_WGL_IMGCF_H_

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include "u_wgl.h"
#include "u_wgl_tools.h"
#include "u_handle.h"
#include "u_lnk_list.h"

#ifdef __cplusplus
extern "C" {
#endif


/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/

/*Specific Command*/
#define WGL_CMD_IMGCF_SET_KEY_MAP        WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_IMGCF, 1)    /**< This command changes the keys for operating a imgcf.

                                                                                                                                                        related parameters in c_wgl_do_cmd(),

                                                                                                                                                        pv_param1:   Pointer to a WGL_IMGCF_KEY_MAP_T that contains the new key map. If it is NULL, it means to use the imgcf's default setting. 

                                                                                                                                                        pv_param2:   Reserved. It must be NULL.
                                                                                                                                                        */
#define WGL_CMD_IMGCF_GET_KEY_MAP        WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_IMGCF, 2)    /**<This command returns the key map of a imgcf.

                                                                                                                                                        related parameters in c_wgl_do_cmd(),

                                                                                                                                                        pv_param1:   Pointer to a WGL_IMGCF_KEY_MAP_T variable to receive the key map. It cannot be NULL.

                                                                                                                                                        pv_param2:   Reserved. It must be NULL.
                                                                                                                                                        */
#define WGL_CMD_IMGCF_SET_CNT_INSET      WGL_MAKE_CONTENT_CMD(HT_WGL_WIDGET_IMGCF, 3)    /**< This command is used to set the display margin of imgcf Static's foreground. The i4_left is for the left margin, the i4_right is for the right margin, the i4_top is for the top margin, and the i4_bottom is for the bottom margin. All of them cannot be less than 0.

                                                                                                                                                        related parameters in c_wgl_do_cmd(),

                                                                                                                                                        pv_param1:   Pointer to a WGL_INSET_T value to specify the display margin. It cannot be NULL.

                                                                                                                                                        pv_param2:   Reserved. It must be NULL.
                                                                                                                                                        */
#define WGL_CMD_IMGCF_GET_CNT_INSET      WGL_MAKE_CONTENT_CMD(HT_WGL_WIDGET_IMGCF, 4)    /**<This command returns the display margin of imgcf Static's foreground. The i4_left is the left margin, the i4_right is the right margin, the i4_top is the top margin, and the i4_bottom is the bottom margin. 

                                                                                                                                                        related parameters in c_wgl_do_cmd(),

                                                                                                                                                        pv_param1:   Pointer to a WGL_INSET_T value to receive the display margin. It cannot be NULL.

                                                                                                                                                        pv_param2:   Reserved. It must be NULL.
                                                                                                                                                        */
#define WGL_CMD_IMGCF_GET_CNT_WIDTH     WGL_MAKE_CONTENT_CMD(HT_WGL_WIDGET_IMGCF, 5) /**<  This command get  the width of the imgcf.       

                                                                                                                                                            related parameters in c_wgl_do_cmd(),

                                                                                                                                                            pv_param1:   A UIN16  point to the width of wgl

                                                                                                                                                            pv_param2:   NULL
                                                                                                                                                            */
#define WGL_CMD_IMGCF_ADD_ITEM          WGL_MAKE_CONTENT_CMD(HT_WGL_WIDGET_IMGCF, 6) /**<  This command add a image item to the imgcf.       

                                                                                                                                                            related parameters in c_wgl_do_cmd(),

                                                                                                                                                            pv_param1:   Pointer to a WGL_IMGCF_ITEM_T value to receive a image item. It cannot be NULL.

                                                                                                                                                            pv_param2:   NULL
                                                                                                                                                            */
#define WGL_CMD_IMGCF_BEGIN_PLAY        WGL_MAKE_CONTENT_CMD(HT_WGL_WIDGET_IMGCF, 7) /**<  This command begine to play the imgcf.       

                                                                                                                                                            related parameters in c_wgl_do_cmd(),

                                                                                                                                                            pv_param1:   Point to the delay time after which the timer pops

                                                                                                                                                            pv_param2:   NULL
                                                                                                                                                            */
#define WGL_CMD_IMGCF_PAUSE_REUME_TIMER     WGL_MAKE_CONTENT_CMD(HT_WGL_WIDGET_IMGCF, 8) /**<  This command get  the width of the imgcf.       

                                                                                                                                                            related parameters in c_wgl_do_cmd(),

                                                                                                                                                            pv_param1:   NULL

                                                                                                                                                            pv_param2:   NULL
                                                                                                                                                            */
#define WGL_CMD_IMGCF_SPEED_UP_PLAYE     WGL_MAKE_CONTENT_CMD(HT_WGL_WIDGET_IMGCF, 9) /**<  This command get  the width of the imgcf.       

                                                                                                                                                            related parameters in c_wgl_do_cmd(),

                                                                                                                                                            pv_param1:   NULL

                                                                                                                                                            pv_param2:   NULL
                                                                                                                                                            */
#define WGL_CMD_IMGCF_REGULAR_PLAYE     WGL_MAKE_CONTENT_CMD(HT_WGL_WIDGET_IMGCF, 10) /**<  This command get  the width of the imgcf.       

                                                                                                                                                            related parameters in c_wgl_do_cmd(),

                                                                                                                                                            pv_param1:   NULL

                                                                                                                                                            pv_param2:   NULL
                                                                                                                                                            */
#define WGL_CMD_IMGCF_PLAY_TO_LEFT      WGL_MAKE_CONTENT_CMD(HT_WGL_WIDGET_IMGCF, 11) /**<  This command get  the width of the imgcf.       

                                                                                                                                                            related parameters in c_wgl_do_cmd(),

                                                                                                                                                            pv_param1:   NULL

                                                                                                                                                            pv_param2:   NULL
                                                                                                                                                            */
#define WGL_CMD_IMGCF_PLAY_TO_RIGHT     WGL_MAKE_CONTENT_CMD(HT_WGL_WIDGET_IMGCF, 12) /**<  This command get  the width of the imgcf.       

                                                                                                                                                            related parameters in c_wgl_do_cmd(),

                                                                                                                                                            pv_param1:   NULL

                                                                                                                                                            pv_param2:   NULL
                                                                                                                                                            */

                                                                                                                                                           
#define IMGCF_CNT_PRIVATE_START          13    

/*Widget Style*/
#define WGL_IMGCF_FIVE_SHOWN            ((UINT32) WGL_STL_WGT_SPF_START)          /**< Indicates five images shown at the same time */
#define WGL_IMGCF_SEVEN_SHOWN           ((UINT32) WGL_STL_WGT_SPF_START << 1)    /**<Indicates seven images shown at the same time */
#define WGL_IMGCF_NIGHT_SHOWN           ((UINT32) WGL_STL_WGT_SPF_START << 2)    /**<Indicates night images shown at the same time */
#define IMGCF_BK_CLR_NOT_INIT           ((UINT32) WGL_STL_WGT_SPF_START << 3)    
#define IMGCF_BK_IMG_NOT_INIT           ((UINT32) WGL_STL_WGT_SPF_START << 4)    
#define IMGCF_NO_BK                     ((UINT32) WGL_STL_WGT_SPF_START << 5)    
#define IMGCF_NOT_CLEAN_BK              ((UINT32) WGL_STL_WGT_SPF_START << 6)    
#define IMGCF_EDGE_CLR_NOT_INIT         ((UINT32) WGL_STL_WGT_SPF_START << 7)    
#define IMGCF_IMGCF_BG_CLR_NOT_INIT     ((UINT32) WGL_STL_WGT_SPF_START << 8)    

/************************************************************************/
/* Error code                                                           */
/************************************************************************/
#define IMGCFR_ALREADY_STOP           ((INT32)     1)    /**< Imgcf stop       */
#define IMGCFR_OK                     ((INT32)     0)    /**< Imgcf ok       */
#define IMGCFR_OPEND_ENOUGH           ((INT32)    -1)    /**< the count of opened image items are enough */
#define IMGCFR_TEMPLATE_ERR           ((INT32)    -2)    /**< out of memory       */
#define IMGCFR_MEMORY_ERR             ((INT32)    -3)    /**< memory opration error       */
#define IMGCFR_SURFACE_ERR            ((INT32)    -4)    /**< surface opration error       */
#define IMGCFR_FILE_ERR               ((INT32)    -5)    /**< file opration error       */
#define IMGCFR_OUT_OF_MEMORY          ((INT32)    -6)    /**< out of memory       */
#define IMGCFR_EXCEED_MAX_SUPPORT     ((INT32)    -7)    /**< exceed imgcf MAX Support       */


/*-----------------------------------------------------------------------------
                    structures
 ----------------------------------------------------------------------------*/

typedef enum{
	WGL_IMGCF_DIRECTION_TO_LEFT,
	WGL_IMGCF_DIRECTION_TO_RIGHT
	
}WGL_IMGCF_DIRECTION_TYPE;

/*Control Key Map*/
typedef struct _WGL_IMGCF_KEY_MAP_T
{
    UINT32          ui4_to_right;
    UINT32          ui4_to_left;
    UINT32          ui4_speed_up;
    UINT32          ui4_speed_down;
	
} WGL_IMGCF_KEY_MAP_T;


/*-----------------------------------------------------------------------------
                    typedefs, enums
 ----------------------------------------------------------------------------*/

typedef enum{
	WGL_IMGCF_ITEM_TYPE_IMAGE_TEMPLATE,
	WGL_IMGCF_ITEM_TYPE_MEMORY,
	WGL_IMGCF_ITEM_TYPE_FILE,
	WGL_IMGCF_ITEM_TYPE_SURFACE,
	WGL_IMGCF_ITEM_TYPE_MAX
	
}WGL_IMGCF_ITEM_TYPE;


typedef struct _WGL_IMGCF_ITEM_TEMPLATE_T{
	WGL_HIMG_TPL_T		   h_img_tpl;
	
}WGL_IMGCF_ITEM_TEMPLATE_T;


typedef struct _WGL_MEMORY_META_DATA_T{
	VOID*				      buffer;
	UINT32				  zSize;
	UINT32 	              ui4_surf_width;
	UINT32 	              ui4_surf_height;
	
}WGL_IMGCF_ITEM_MEMORY_T;


typedef struct _WGL_IMGCF_ITEM_META_DATA_T{
	CHAR*				    Filename;
	UINT32               ui4_surf_width;
	UINT32               ui4_surf_height;
	
}WGL_IMGCF_ITEM_FILE_T;


typedef struct _WGL_IMGCF_ITEM_SURFACE_T{
	GL_HSURFACE_T			   h_surf;
	
}WGL_IMGCF_ITEM_SURFACE_T;


typedef struct _WGL_IMGCF_ITEM_T
{
	WGL_IMGCF_ITEM_TYPE 		e_type;
	union{
		WGL_IMGCF_ITEM_TEMPLATE_T	             image_item;
		WGL_IMGCF_ITEM_MEMORY_T                 memory_item;
		WGL_IMGCF_ITEM_FILE_T	                  file_item;
		WGL_IMGCF_ITEM_SURFACE_T                surf_item;		
   }u;
	
}WGL_IMGCF_ITEM_T;


typedef struct _WGL_IMGCF_ITEM_INTERNAL_T
{
	DLIST_ENTRY_T(_WGL_IMGCF_ITEM_INTERNAL_T) t_item_lnk;
	WGL_IMGCF_ITEM_T  item;	
	UINT8				     ui1_disp_index;/* if it is larger than 0, this item is being played */
	
}WGL_IMGCF_ITEM_INTERNAL_T;


typedef struct _WGL_IMGCF_ITEM_INFO_T
{
	DLIST_T(_WGL_IMGCF_ITEM_INTERNAL_T) t_item_list;
	
}WGL_IMGCF_ITEM_INFO_T;


#ifdef __cplusplus
}
#endif

#endif
/*----------------------------------------------------------------------------*/
/*! @} */   /* end of groupMW_WGL_IMGCF*/
/*----------------------------------------------------------------------------*/
