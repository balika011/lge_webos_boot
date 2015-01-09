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
 *
 * $Author: dtvbm11 $
 * $Date: 2015/01/09 $
 * $RCSfile: vdp_mw.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file vdp_mt53fb_mw.c
 *  Brief of file vdp_mt53fb_mw.c.
 *  Details of file vdp_photo_mw.c (optional).
 */


//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#ifdef CC_DFB_SUPPORT_VDP_LAYER

typedef struct _mt53fb_vdp_init
{
    unsigned int width;
    unsigned int height;
}mt53fb_vdp_init;

typedef struct mt53fb_vdp_flip
{
    unsigned char ucFbgId;
	unsigned int  ui4_pitch;
    unsigned char* pu1SrcYAddr;
    unsigned char* pu1SrcCAddr;
    unsigned char fgIs3DMode;
    unsigned int u4CropBottomOffset;
    unsigned int u4CropTopOffset;
    unsigned int u4CropLeftOffset;
    unsigned int u4CropRightOffset;          
}mt53fb_vdp_flip;

typedef struct mt53fb_vdp_set
{
    unsigned char ucEnable;
}mt53fb_vdp_set;

#define MT53FB_VDP_ROTATE_90 0 
#define MT53FB_VDP_ROTATE_270 1
#define MT53FB_VDP_ROTATE_180 2

typedef struct mt53fb_vdp_rotate
{
    unsigned int u4Src;
    unsigned int u4Dst;
    unsigned int eSrcCm;
    unsigned int eDstCm;       
    unsigned int u4SrcPitch;
    unsigned int u4DstPitch;    
    unsigned int u4SrcX;
    unsigned int u4SrcY;
    unsigned int u4Width;    
    unsigned int u4Height;        
    unsigned int u4DstX;       
    unsigned int u4DstY;
    unsigned int u4RotateOp;
    unsigned int u4SrcCbCrOffset;    
    unsigned int u4DstCbCrOffset;        
}mt53fb_vdp_rotate; 

typedef struct _mt53fb_vdp_crop_rect
{
    unsigned int                ui4_layer;
	unsigned int                ui4_3d_mode;
    unsigned int                crop_left_offset;       /* pixel width */
    unsigned int                crop_right_offset;      /* pixel height */    
    unsigned int                crop_top_offset;        /* pixel width */
    unsigned int                crop_bottom_offset;     /* pixel height */                                                                                                                      
}mt53fb_vdp_crop_rect;

typedef struct _mt53fb_vdp_stretch_blit
{
    unsigned int src1_buffer_Y_addr;
    unsigned int src1_buffer_C_addr;
    unsigned int src1_buffer_Y_width;
    unsigned int src1_buffer_Y_height;       
    unsigned int src1_buffer_Y_pitch;
    unsigned int src2_buffer_Y_addr;    
    unsigned int src2_buffer_C_addr;
    unsigned int dest_buffer_Y_addr;
    unsigned int dest_buffer_C_addr;    
    unsigned int dest_buffer_Y_width;        
    unsigned int dest_buffer_Y_height;       
    unsigned int dest_buffer_Y_pitch;  
	unsigned int composition_level;
}mt53fb_vdp_stretch_blit;

typedef struct _mt53fb_imagebuffer
{
    unsigned int u4Size;
    unsigned int u4VirtAddr;
    unsigned int u4PhyAddr;
}mt53fb_imagebuffer;


extern UINT32 VDP_SetInput(UCHAR ucVdpId, UCHAR ucEsId, UCHAR ucPort);

extern INT32 _VdpPhotoSetCommand(DRV_COMP_ID_T* prCompId, DRV_SET_TYPE_T rSetType,
                            const VOID* pvSetInfo, SIZE_T zSetInfoLen);

#endif


