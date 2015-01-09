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
#ifndef _U_SBTL_IPCC_H_
#define _U_SBTL_IPCC_H_

#ifndef __KERNEL__

/*-----------------------------------------------------------------------------
                    include files
-----------------------------------------------------------------------------*/
//#include "x_lnk_list.h"
//#include "x_handle.h"
//#include "x_sm.h"
//#include "u_wgl_common.h"
//#include "u_wgl_cc.h"
//#include "u_common.h"
#include "x_lnk_list.h"
#include "x_handle.h"
#include "x_sm.h"
#include "u_wgl_common.h"
#include "u_wgl_cc.h"
#include "u_common.h"



/*-----------------------------------------------------------------------------
                    macros, defines, enums
-----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*! @enum   SBTL_IPCC_CAPTION_MODE
 *  @brief  SBTL_IPCC_CAPTION_MODE
 *  @code
 *  typedef enum _SBTL_IPCC_CAPTION_MODE
 *  {
 *      SBTL_IPCC_CAPTION_MODE_POP_ON = 0,       
 *      SBTL_IPCC_CAPTION_MODE_PAINT_ON,
 *      SBTL_IPCC_CAPTION_MODE_ROLL_UP
 *  } SBTL_IPCC_CAPTION_MODE;
 *  @endcode
 *   
 *  @li@c SBTL_IPCC_CAPTION_MODE_POP_ON                 - Pop-on
 *  @li@c SBTL_IPCC_CAPTION_MODE_PAINT_ON               - Paint-on
 *  @li@SBTL_IPCC_CAPTION_MODE_ROLL_UP                  - Roll-up
 */
/*----------------------------------------------------------------------------*/
typedef enum _SBTL_IPCC_CAPTION_MODE
{
    SBTL_IPCC_CAPTION_MODE_POP_ON = 0,
    SBTL_IPCC_CAPTION_MODE_PAINT_ON,
    SBTL_IPCC_CAPTION_MODE_ROLL_UP
} SBTL_IPCC_CAPTION_MODE;


/*----------------------------------------------------------------------------*/
/*! @enum   TTML_LENGTH_TYPE
 *  @brief  TTML_LENGTH_TYPE
 *  @code
 *  typedef enum _TTML_LENGTH_TYPE
 *  {
 *      TTML_LENGTH_TYPE_SCALAR = 0,       
 *      TTML_LENGTH_TYPE_PERCENTAGE              
 *  } TTML_LENGTH_TYPE;
 *  @endcode
 *   
 *  @li@c TTML_LENGTH_TYPE_SCALAR                   - Number units
 *  @li@c TTML_LENGTH_TYPE_PERCENTAGE               - Number "%"
 */
/*----------------------------------------------------------------------------*/
typedef enum _TTML_LENGTH_TYPE
{
    TTML_LENGTH_TYPE_SCALAR   = 0,
    TTML_LENGTH_TYPE_PERCENTAGE
}TTML_LENGTH_TYPE;


/*----------------------------------------------------------------------------*/
/*! @enum   TTML_LENGTH_UNIT_TYPE
 *  @brief  TTML_LENGTH_UNIT_TYPE
 *  @code
 *  typedef enum _TTML_LENGTH_UNIT_TYPE
 *  {
 *     TTML_LENGTH_UNIT_TYPE_UNKNOW = 0,
 *     TTML_LENGTH_UNIT_TYPE_PX,
 *     TTML_LENGTH_UNIT_TYPE_PRCNT,
 *     TTML_LENGTH_UNIT_TYPE_C
 *  } TTML_LENGTH_UNIT_TYPE;
 *  @endcode
 *   
 *  @li@c TTML_LENGTH_UNIT_TYPE_UNKNOW         - unknown
 *  @li@c TTML_LENGTH_UNIT_TYPE_PX             - pixel
 *  @li@c TTML_LENGTH_UNIT_TYPE_PRCNT          - %
 *  @li@c TTML_LENGTH_UNIT_TYPE_C              - cell
 */
/*----------------------------------------------------------------------------*/
typedef enum _TTML_LENGTH_UNIT_TYPE
{
    TTML_LENGTH_UNIT_TYPE_UNKNOW = 0,
    TTML_LENGTH_UNIT_TYPE_PX,
    TTML_LENGTH_UNIT_TYPE_PRCNT,
    TTML_LENGTH_UNIT_TYPE_C   
}TTML_LENGTH_UNIT_TYPE;


/*----------------------------------------------------------------------------*/
/*! @enum   WGT_ATTR_TYPE
 *  @brief  WGT_ATTR_TYPE
 *  @code
 *  typedef enum _WGT_ATTR_TYPE
 *  {
 *     WGT_ATTR_TYPE_SET_WIN_ANCHOR = 0,
 *     WGT_ATTR_TYPE_SET_WIN_SIZE,
 *     WGT_ATTR_TYPE_SET_WIN_ATTR,
 *     WGT_ATTR_TYPE_SET_PEN_FONT,
 *     WGT_ATTR_TYPE_SET_FONT_NAME,
 *     WGT_ATTR_TYPE_SET_PEN_COLOR,
 *     WGT_ATTR_TYPE_SET_TEXT_TAG
 *  } WGT_ATTR_TYPE;
 *  @endcode
 *   
 *  @li@c WGT_ATTR_TYPE_SET_WIN_ANCHOR        - Window anchor
 *  @li@c WGT_ATTR_TYPE_SET_WIN_SIZE          - Window size
 *  @li@c WGT_ATTR_TYPE_SET_WIN_ATTR          - Window attribute
 *  @li@c WGT_ATTR_TYPE_SET_PEN_FONT          - Pen font
 *  @li@c WGT_ATTR_TYPE_SET_FONT_NAME         - Font name
 *  @li@c WGT_ATTR_TYPE_SET_PEN_COLOR         - Pen color
 *  @li@c WGT_ATTR_TYPE_SET_TEXT_TAG          - Text tag
 */
/*----------------------------------------------------------------------------*/
typedef enum _WGT_ATTR_TYPE
{
    WGT_ATTR_TYPE_SET_WIN_ANCHOR = 0,
    WGT_ATTR_TYPE_SET_WIN_SIZE,  
    WGT_ATTR_TYPE_SET_WIN_ATTR,
    WGT_ATTR_TYPE_SET_PEN_FONT,
    WGT_ATTR_TYPE_SET_FONT_NAME,
    WGT_ATTR_TYPE_SET_PEN_COLOR,
    WGT_ATTR_TYPE_SET_TEXT_TAG
}WGT_ATTR_TYPE;


/*----------------------------------------------------------------------------*/
/*! @enum   WGT_ATTR_COLOR_TYPE
 *  @brief  WGT_ATTR_COLOR_TYPE
 *  @code
 *  typedef enum _WGT_ATTR_COLOR_TYPE
 *  {
 *     WGT_ATTR_COLOR_TYPE_FG = 0,
 *     WGT_ATTR_COLOR_TYPE_BG,
 *     WGT_ATTR_COLOR_TYPE_EDGE
 *  } WGT_ATTR_COLOR_TYPE;
 *  @endcode
 *   
 *  @li@c WGT_ATTR_COLOR_TYPE_FG            - Foreground color
 *  @li@c WGT_ATTR_COLOR_TYPE_BG            - Background color
 *  @li@c WGT_ATTR_COLOR_TYPE_EDGE          - Edge color
 */
/*----------------------------------------------------------------------------*/
typedef enum _WGT_ATTR_COLOR_TYPE
{
    WGT_ATTR_COLOR_TYPE_FG = 0,
    WGT_ATTR_COLOR_TYPE_BG,
    WGT_ATTR_COLOR_TYPE_EDGE
}WGT_ATTR_COLOR_TYPE;


/*----------------------------------------------------------------------------*/
/*! @enum   WGL_CC_WIN_ANCHOR_T
 *  @brief  This struct contains the window start position
 *  @code
 *  typedef struct _WGL_CC_WIN_ANCHOR_T
 *  {
 *     UINT8                   ui1_anchor;
 *     UINT16                  ui2_anchor_v;
 *     UINT16                  ui2_anchor_h;
 *  } WGL_CC_WIN_ANCHOR_T;
 *  @endcode
 *   
 *  @li@c ui1_anchor            - Window anchor number
 *  @li@c ui2_anchor_v          - Window anchor vertical position
 *  @li@c ui2_anchor_h          - Window anchor horizon position
 */
/*----------------------------------------------------------------------------*/
typedef struct _WGL_CC_WIN_ANCHOR_T
{
    UINT8                   ui1_anchor;
    UINT16                  ui2_anchor_v;
    UINT16                  ui2_anchor_h;
}WGL_CC_WIN_ANCHOR_T;


/*----------------------------------------------------------------------------*/
/*! @enum   WGL_CC_WIN_SIZE_T
 *  @brief  This struct contains the window size
 *  @code
 *  typedef struct _WGL_CC_WIN_SIZE_T
 *  {
 *     UINT16                  ui2_row_count;
 *     UINT16                  ui2_col_count;
 *  } WGL_CC_WIN_SIZE_T;
 *  @endcode
 *   
 *  @li@c ui2_row_count          - Window row count
 *  @li@c ui2_col_count          - Window coloumn count
 */
/*----------------------------------------------------------------------------*/
typedef struct _WGL_CC_WIN_SIZE_T
{
    UINT16                  ui2_row_count;
    UINT16                  ui2_col_count;
}WGL_CC_WIN_SIZE_T;


/*------------------------------------------------------------------*/
/*! @struct WGT_ATTR_CMD_T
 *  @brief This struct contains the attr.
 *  @code
 *  typedef struct _WGT_ATTR_CMD_T
 *  {
 *      WGT_ATTR_TYPE                   e_attr_type;
 *      union
 *      {
 *          WGL_CC_PEN_COLOR_T          t_attr_pen_color;
 *          WGL_CC_WIN_ATTR_T           t_attr_win;
 *          WGL_CC_PEN_FONT_T           t_pen_font;
 *          WGL_CC_WIN_ANCHOR_T         t_win_anchor;
 *          WGL_CC_WIN_SIZE_T           t_win_size;
 *          CHAR                        s_font_name[WGL_MAX_FONT_NAME];
 *          UINT8                       ui1_text_tag;
 *      }u;
 *      DLIST_ENTRY_T(_WGT_ATTR_CMD_T)  t_attr_link;
 *  }WGT_ATTR_CMD_T;
 *  @endcode
 *  @li@c  e_attr_type              - Attribute type
 *  @li@c  t_attr_pen_color         - Pen color        
 *  @li@c  t_attr_win               - Window
 *  @li@c  t_win_anchor             - Window anchor 
 *  @li@c  t_win_size               - Window size
 *  @li@c  s_font_name              - Font name
 *  @li@c  ui1_text_tag             - Text tag
 *  @li@c  t_attr_link              - Attribute entry 
 */
/*------------------------------------------------------------------*/
typedef struct _WGT_ATTR_CMD_T
{
    WGT_ATTR_TYPE                   e_attr_type;
    union
    {
        WGL_CC_PEN_COLOR_T          t_attr_pen_color;
        WGL_CC_WIN_ATTR_T           t_attr_win;
        WGL_CC_PEN_FONT_T           t_pen_font;
        WGL_CC_WIN_ANCHOR_T         t_win_anchor;
        WGL_CC_WIN_SIZE_T           t_win_size;
        CHAR                        s_font_name[WGL_MAX_FONT_NAME];
        UINT8                       ui1_text_tag;
    }u;
    DLIST_ENTRY_T(_WGT_ATTR_CMD_T)  t_attr_link;
}WGT_ATTR_CMD_T;


/*------------------------------------------------------------------*/
/*! @struct WGT_ELEMENT_SPAN_CMD_T
 *  @brief This struct contains the <span> element.
 *  @code
 *  typedef struct _WGT_ELEMENT_SPAN_CMD_T
 *  {
 *      DLIST_ENTRY_T(_WGT_ELEMENT_SPAN_CMD_T)      t_element_span_link;
 *      DLIST_T(_WGT_ATTR_CMD_T)                    t_attr_list;
 *      SM_CC_NTSC_CMD_T                            t_content_cmd;
 *  }WGT_ELEMENT_SPAN_CMD_T;
 *  @endcode
 *  @li@c  t_element_span_link        -the pointer of <span> list
 *  @li@c  t_attr_list                -the entry of attr list
 *  @li@c  t_content_cmd              -the cmd of text which need to show
 */
/*------------------------------------------------------------------*/
typedef struct _WGT_ELEMENT_SPAN_CMD_T
{
    DLIST_ENTRY_T(_WGT_ELEMENT_SPAN_CMD_T)      t_element_span_link;
    DLIST_T(_WGT_ATTR_CMD_T)                    t_attr_list;
    CHAR*                                       ps_content;
}WGT_ELEMENT_SPAN_CMD_T;




/*------------------------------------------------------------------*/
/*! @struct WGT_ELEMENT_P_CMD_T
 *  @brief This struct contains the <p> element.
 *  @code
 *  typedef struct _WGT_ELEMENT_P_CMD_T
 *  {
 *      DLIST_ENTRY_T(_WGT_ELEMENT_P_CMD_T)   t_element_p_link;
 *      DLIST_T(_WGT_ELEMENT_SPAN_CMD_T)      t_element_span_list;
 *      UINT8                                 ui1_region_id;
 *      SBTL_IPCC_SET_TYPE_CAPTION_MODE       e_caption_mode;
 *  }WGT_ELEMENT_P_CMD_T;
 *  @endcode
 *  @li@c  t_element_p_link         -the pointer of <p> list
 *  @li@c  t_element_span_list      -the entry of <span> list
 *  @li@c  ui1_region_id            -the id of <region>
 *  @li@c  e_caption_mode           -the caption mode of <p>
 */
/*------------------------------------------------------------------*/
typedef struct _WGT_ELEMENT_P_CMD_T
{
    DLIST_T(_WGT_ELEMENT_SPAN_CMD_T)      t_element_span_list;
    DLIST_ENTRY_T(_WGT_ELEMENT_P_CMD_T)   t_element_p_link;
    UINT8                                 ui1_region_id;
    SBTL_IPCC_CAPTION_MODE                e_caption_mode;  
    GL_COLOR_T                            t_win_bgcolor;
}WGT_ELEMENT_P_CMD_T;


/*------------------------------------------------------------------*/
/*! @struct WGT_TTML_DATA_CMD_T
 *  @brief This struct contains the <p> list.
 *  @code
 *  typedef struct _WGT_TTML_DATA_CMD_T
 *  {
 *      DLIST_T(_WGT_ELEMENT_P_CMD_T)   t_element_p_list;
 *      INT32                           i4_ele_p_count;
 *  }WGT_TTML_DATA_CMD_T;
 *  @endcode
 *  @li@c  t_element_p_list            -the entry of <p> list
 *  @li@c  i4_ele_p_count              -<p> count
 */
/*------------------------------------------------------------------*/
typedef struct _WGT_TTML_DATA_CMD_T
{
    DLIST_T(_WGT_ELEMENT_P_CMD_T)   t_element_p_list;
    INT32                           i4_ele_p_count;
}WGT_TTML_DATA_CMD_T;




/*------------------------------------------------------------------*/
/*! @struct TTML_EXTENT_T
 *  @brief This struct contains the tts:extent.
 *  @code
 *  typedef struct _TTML_EXTENT_T
 *  {
 *      UINT8 ui1_column;
 *      UINT8 ui1_row;
 *  }TTML_EXTENT_T;
 *  @endcode
 *  @li@c  ui1_column                              -column
 *  @li@c  ui1_row                                 -row
 */
/*------------------------------------------------------------------*/
typedef struct _TTML_EXTENT_T
{
    UINT8   ui1_column;
    UINT8   ui1_row;
}TTML_EXTENT_T;



/*------------------------------------------------------------------*/
/*! @struct SBTL_IPCC_CONFIG_T
 *  @brief This struct contains the size of screen.
 *  @code
 *  typedef struct _SBTL_IPCC_CONFIG_T
 *  {
 *        UINT32  ui4_screen_width;
 *        UINT32  ui4_screen_height;
 *  }SBTL_IPCC_CONFIG_T;
 *  @endcode
 *  @li@c  ui4_screen_width             -The width of screen width
 *  @li@c  ui4_screen_height            -The width of screen height

 */
/*------------------------------------------------------------------*/
typedef struct _SBTL_IPCC_CONFIG_T
{
    UINT32  ui4_screen_width;
    UINT32  ui4_screen_height;
}SBTL_IPCC_CONFIG_T;


/*----------------------------------------------------------------------------*/
/*! @enum   SBTL_IPCC_VIDEO_TYPE
 *  @brief  SBTL_IPCC_VIDEO_TYPE
 *  @code
 *  typedef enum _SBTL_IPCC_VIDEO_TYPE
 *  {
 *      SBTL_IPCC_VIDEO_TYPE_HD = 0,       
 *      SBTL_IPCC_VIDEO_TYPE_SD
 *      
 *  } SBTL_IPCC_VIDEO_TYPE;
 *  @endcode
 *   
 *  @li@c SBTL_IPCC_VIDEO_TYPE_HD                             - HD
 *  @li@c SBTL_IPCC_VIDEO_TYPE_SD                             - SD
 */
/*----------------------------------------------------------------------------*/
typedef enum _SBTL_IPCC_VIDEO_TYPE
{
    SBTL_IPCC_VIDEO_TYPE_HD = 0,
    SBTL_IPCC_VIDEO_TYPE_SD
}SBTL_IPCC_VIDEO_TYPE;


/*----------------------------------------------------------------------------*/
/*! @enum   SBTL_IPCC_FILE_TYPE
 *  @brief  SBTL_IPCC_FILE_TYPE
 *  @code
 *  typedef enum _SBTL_IPCC_FILE_TYPE
 *  {
 *      SBTL_IPCC_FILE_TYPE_SENVU = 0,       
 *      SBTL_IPCC_FILE_TYPE_CRACKLE
 *      
 *  } SBTL_IPCC_FILE_TYPE;
 *  @endcode
 *   
 *  @li@c SBTL_IPCC_FILE_TYPE_SENVU                 - m4t type
 *  @li@c SBTL_IPCC_FILE_TYPE_CRACKLE               - Other type, except m4t
 */
/*----------------------------------------------------------------------------*/
typedef enum _SBTL_IPCC_FILE_TYPE
{
    SBTL_IPCC_FILE_TYPE_SENVU = 0,
    SBTL_IPCC_FILE_TYPE_CRACKLE
}SBTL_IPCC_FILE_TYPE;


/*----------------------------------------------------------------------------*/
/*! @enum   SBTL_IPCC_FILE_MODE
 *  @brief  SBTL_IPCC_FILE_MODE
 *  @code
 *  typedef enum _SBTL_IPCC_FILE_MODE
 *  {
 *      SBTL_IPCC_FILE_MODE_PRESERVED = 0,       
 *      SBTL_IPCC_FILE_MODE_ENHANCED
 *  } SBTL_IPCC_FILE_MODE;
 *  @endcode
 *   
 *  @li@c SBTL_IPCC_FILE_MODE_PRESERVED              - Preserved, in xml/m4t file, search {mode="Preserved"}
 *  @li@c SBTL_IPCC_FILE_MODE_ENHANCED               - Enhanced, in xml/m4t file, search {mode="Enhanced"}
 */
/*----------------------------------------------------------------------------*/
typedef enum _SBTL_IPCC_FILE_MODE
{
    SBTL_IPCC_FILE_MODE_PRESERVED = 0,
    SBTL_IPCC_FILE_MODE_ENHANCED
}SBTL_IPCC_FILE_MODE;



/*----------------------------------------------------------------------------*/
/*! @struct SBTL_IPCC_VIDEO_T
 *  @brief  This struct contains video info
 *  @code
 *  typedef struct _SBTL_IPCC_VIDEO_T
 *  {
 *      SBTL_IPCC_VIDEO_TYPE    e_video_type;
 *      UINT16                  ui2_video_width;
 *      UINT16                  ui2_video_height;
 *      SBTL_IPCC_FILE_TYPE     e_file_type;
 *      SBTL_IPCC_FILE_MODE     e_file_mode; 
 *  } SBTL_IPCC_VIDEO_T;
 *  @endcode
 *   
 *  @li@c e_video_type        - Video type
 *  @li@c ui2_video_width     - Video width
 *  @li@c ui2_video_height    - Video height
 *  @li@c e_file_type         - File type
 *  @li@c e_file_mode         - File mode
 */
/*----------------------------------------------------------------------------*/
typedef struct _SBTL_IPCC_VIDEO_T
{
    SBTL_IPCC_VIDEO_TYPE    e_video_type;
    UINT16                  ui2_video_width;
    UINT16                  ui2_video_height;
    SBTL_IPCC_FILE_TYPE     e_file_type;
    SBTL_IPCC_FILE_MODE     e_file_mode;
}SBTL_IPCC_VIDEO_T;


/*----------------------------------------------------------------------------*/
/*! @struct SBTL_IPCC_OPEN_CFG_T
 *  @brief  This struct contains configuration args
 *  @code
 *  typedef struct _SBTL_IPCC_OPEN_CFG_T
 *  {
 *      CHAR*               ps_extent;       
 *      CHAR*               ps_text;
 *      UINT8               ui1_row_count;
 *      SBTL_IPCC_VIDEO_T   t_ipcc_video; 
 *      
 *  } SBTL_IPCC_OPEN_CFG_T;
 *  @endcode
 *   
 *  @li@c ps_extent            - tts:extent
 *  @li@c ps_text              - Text
 *  @li@c ui1_row_count        - Row count of text
 *  @li@c t_ipcc_video         - Video args
 */
/*----------------------------------------------------------------------------*/
typedef struct _SBTL_IPCC_OPEN_CFG_T
{
    CHAR*               ps_extent;
    CHAR*               ps_aspect_ratio;
    UINT8               ui1_row_count;
    SBTL_IPCC_VIDEO_T   t_ipcc_video;   
        
}SBTL_IPCC_OPEN_CFG_T;



#endif /* __KERNEL__ */
#endif/* _U_SBTL_IPCC_H_ */
/*----------------------------------------------------------------------------*/
/*! @} */   /* end of  groupMW_STRM_MNGR_MM_SBTL_HDLR*/
/*----------------------------------------------------------------------------*/

