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
#ifndef _U_SBTL_IPCC_WGT_H_
#define _U_SBTL_IPCC_WGT_H_
#ifndef __KERNEL__

/*-----------------------------------------------------------------------------
                    include files
-----------------------------------------------------------------------------*/
#include "x_lnk_list.h"
#include "u_handle.h"
#include "x_wgl.h"
#include "x_os.h"
#include "u_wgl.h"
#include "u_wgl_cc.h"


/*-----------------------------------------------------------------------------
                macros, defines, enums
-----------------------------------------------------------------------------*/
#define IPCC_MAX_WGT_NUM        ((UINT32) 8)
#define IPCC_MAX_PEN_STL_NUM    ((UINT32) 8)

/* Default colors                A      R       G       B                    */
#define CC_COLOR_BLACK         { 0xFF, {0x00}, {0x00}, {0x00} }
#define CC_COLOR_BLACK_TLC     { 0x7F, {0x00}, {0x00}, {0x00} }
#define CC_COLOR_WHITE         { 0xFF, {0xFF}, {0xFF}, {0xFF} }
#define CC_COLOR_WHITE_TLC     { 0x7F, {0xFF}, {0xFF}, {0xFF} }
#define CC_COLOR_GREEN         { 0xFF, {0x00}, {0xFF}, {0x00} }
#define CC_COLOR_GREEN_TLC     { 0x7F, {0x00}, {0xFF}, {0x00} }
#define CC_COLOR_BLUE          { 0xFF, {0x00}, {0x00}, {0xFF} }
#define CC_COLOR_BLUE_TLC      { 0x7F, {0x00}, {0x00}, {0xFF} }
#define CC_COLOR_RED           { 0xFF, {0xFF}, {0x00}, {0x00} }
#define CC_COLOR_RED_TLC       { 0x7F, {0xFF}, {0x00}, {0x00} }
#define CC_COLOR_CYAN          { 0xFF, {0x00}, {0xFF}, {0xFF} }
#define CC_COLOR_CYAN_TLC      { 0x7F, {0x00}, {0xFF}, {0xFF} }
#define CC_COLOR_YELLOW        { 0xFF, {0xFF}, {0xFF}, {0x00} }
#define CC_COLOR_YELLOW_TLC    { 0x7F, {0xFF}, {0xFF}, {0x00} }
#define CC_COLOR_MAGENTA       { 0xFF, {0xFF}, {0x00}, {0xFF} }
#define CC_COLOR_MAGENTA_TLC   { 0x7F, {0xFF}, {0x00}, {0xFF} }
#define CC_COLOR_GRAY          { 0xFF, {0x20}, {0x20}, {0x20} }
#define CC_COLOR_GRAY_TLC      { 0x7F, {0x20}, {0x20}, {0x20} }
#define CC_COLOR_TRANSPARENT   { 0x00, {0x00}, {0x00}, {0x00} }


/* Command - set pen color ***************************************************/
typedef struct _CAP_CMD_SET_PEN_LOC_T
{
    UINT8    ui1_b1;
    
#define CAP_CMD_SPL_RSRV1( pt_cmd )        ((UINT8)( pt_cmd->ui1_b1 & 0xF0 ) >> 4)
#define CAP_CMD_SPL_ROW( pt_cmd )          ((UINT8)( pt_cmd->ui1_b1 & 0x0F ))

    UINT8    ui1_b2;
    
#define CAP_CMD_SPL_RSRV2( pt_cmd )        ((UINT8)( pt_cmd->ui1_b2 & 0xC0 ) >> 6)
#define CAP_CMD_SPL_COLUMN( pt_cmd )       ((UINT8)( pt_cmd->ui1_b2 & 0x3F ))

} CAP_CMD_SET_PEN_LOC_T;



/* Border type */
#define CAP_CMD_TEXT_TAG_DIALOG             ((UINT8) 0 )
#define CAP_CMD_TEXT_TAG_SRC_SPEAKER_ID     ((UINT8) 1 )
#define CAP_CMD_TEXT_TAG_ELEC_REPROD_VOICE  ((UINT8) 2 )
#define CAP_CMD_TEXT_TAG_DIALOG_NOT_PRIM    ((UINT8) 3 )
#define CAP_CMD_TEXT_TAG_VOICEOVER          ((UINT8) 4 )
#define CAP_CMD_TEXT_TAG_AUDIBLE_TRANSLATE  ((UINT8) 5 )
#define CAP_CMD_TEXT_TAG_SUBTITLE_TRANSLATE ((UINT8) 6 )
#define CAP_CMD_TEXT_TAG_VOICE_QA_DESC      ((UINT8) 7 )
#define CAP_CMD_TEXT_TAG_SONG_LYRICS        ((UINT8) 8 )
#define CAP_CMD_TEXT_TAG_SND_EFF_DESC       ((UINT8) 9 )
#define CAP_CMD_TEXT_TAG_MUSICAL_SCORE_DESC ((UINT8) 10 )
#define CAP_CMD_TEXT_TAG_EXPLETIVE          ((UINT8) 11 )
#define CAP_CMD_TEXT_TAG_NOT_TO_BE_DISP     ((UINT8) 15 )



/*-----------------------------------------------------------------------------
                            struct
-----------------------------------------------------------------------------*/


#endif

#endif
