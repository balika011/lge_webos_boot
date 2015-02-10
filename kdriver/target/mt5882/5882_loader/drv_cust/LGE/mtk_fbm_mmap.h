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

#ifndef MTK_FBM_MMAP_H
#define MTK_FBM_MMAP_H

UINT8 gStaticMapFBM_Pools[] = 
{

    FBM_POOL_TYPE_DMX,
    FBM_POOL_TYPE_TDC,
    FBM_POOL_TYPE_VBI,
#ifndef CC_VDEC_FBM_DYNAMIC_MAPPING    
    FBM_POOL_TYPE_MPEG,
#endif    
//   FBM_POOL_TYPE_MJC_1,
#ifndef CC_VDEC_FBM_DYNAMIC_MAPPING   
    FBM_POOL_TYPE_MPEG_TV,
    FBM_POOL_TYPE_MPEG_NET,
#endif    
    //FBM_POOL_TYPE_SCPOS_MAIN,//
//    FBM_POOL_TYPE_SCPOS_PIP,
    FBM_POOL_TYPE_FEEDER,
    FBM_POOL_TYPE_FEEDER_MMP,
    FBM_POOL_TYPE_TDC_DYN,
    FBM_POOL_TYPE_DSP,
#ifndef CC_JPEG_FBM_DYNAMIC_MAPPING    
    FBM_POOL_TYPE_JPEG,
    FBM_POOL_TYPE_JPEG_OSD1,
    FBM_POOL_TYPE_JPEG_OSD2,
    FBM_POOL_TYPE_JPEG_OSD3,
    FBM_POOL_TYPE_JPEG_OSD4,
#endif    
    FBM_POOL_TYPE_PQ_TOOL,//
//    FBM_POOL_TYPE_OD,
    FBM_POOL_TYPE_VENC,
    FBM_POOL_TYPE_SWDMX,
#ifndef CC_VDEC_FBM_DYNAMIC_MAPPING    
    FBM_POOL_TYPE_MPEG_FREE,
#endif    
    FBM_POOL_TYPE_DIVA,
    FBM_POOL_TYPE_FEEDER2,
    FBM_POOL_TYPE_DMX2,
#ifndef CC_VDEC_FBM_DYNAMIC_MAPPING    
    FBM_POOL_TYPE_MPEG2,
#endif    
    FBM_POOL_TYPE_PVR,
    FBM_POOL_TYPE_PVR2,
#ifndef CC_JPEG_FBM_DYNAMIC_MAPPING    
    FBM_POOL_TYPE_JPG_VDP,
#endif    
#ifndef CC_JPEG_FBM_DYNAMIC_MAPPING
    FBM_POOL_TYPE_PNG,
#endif    
    FBM_POOL_TYPE_DIVX,
#ifndef CC_VDEC_FBM_DYNAMIC_MAPPING    
    FBM_POOL_TYPE_MPEG3,//
#endif    
    FBM_POOL_TYPE_VSS,
    FBM_POOL_TYPE_DMXPID,
    FBM_POOL_TYPE_DRM,
    FBM_POOL_TYPE_PQ_3DC,
    FBM_POOL_TYPE_SUBTITLE,
    FBM_POOL_TYPE_DMX3,
    FBM_POOL_TYPE_FEEDER3,
    FBM_POOL_TYPE_EDB,
    FBM_POOL_TYPE_DSP_BIN,
    FBM_POOL_TYPE_PHOTO_SHARE,
#ifndef CC_VDEC_FBM_DYNAMIC_MAPPING    
    FBM_POOL_TYPE_MPEG4,//
#endif    
    FBM_POOL_TYPE_FEEDER4,
    FBM_POOL_TYPE_DMX4,
#ifndef CC_VDEC_FBM_DYNAMIC_MAPPING    
    FBM_POOL_TYPE_MPEG_C,
    FBM_POOL_TYPE_MPEG2_C,
    FBM_POOL_TYPE_MPEG3_C,
    FBM_POOL_TYPE_MPEG4_C,
#endif 
    FBM_POOL_TYPE_SECURE_FEEDER,
    FBM_POOL_TYPE_SRS,
};

#endif /* MTK_FBM_MMAP_H */
