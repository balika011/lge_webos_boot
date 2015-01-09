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
 * $RCSfile: drv_tdc.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

#define _DRV_TDC_C_

#include "x_assert.h"
#include "x_os.h"
#include "x_bim.h"
#include "x_util.h"

#include "general.h"
#include "hw_vdoin.h"
#include "nptv_debug.h"

#include "hw_tdc.h"
#include "hw_tvd.h"
#include "hw_nr.h"
#include "hw_vdoin.h"

#include "drv_tdc.h"
#include "drv_tvd.h"

#include "source_select.h"
#include "vdo_misc.h"
#include "video_def.h"

#include "util.h"
#include "drvcust_if.h"

#ifndef  CC_UP8032_ATV
#include "fbm_drvif.h"
EXTERN UINT32 TDC_DRAM_BASE;
#else
extern UINT32 TDC_DRAM_BASE;
#endif

UINT8 _bTDCEnable;

REGTBL_T const CODE REGTBL_COMB_NTSC_AV[] = {
	{COMB_CTRL_00, 0x000B101F, 0x1FFFFFFF},
	{COMB_CTRL_01, 0x08000000, 0xFE000000},
	{COMB_CTRL_02, 0x0A64CADD, 0xFFFFFFFF},
	{COMB_CTRL_03, 0x28FF0210, 0xFFFFFFFF},
	{COMB_CTRL_04, 0x02F70000, 0x8FFFFFFF},
	{COMB_CTRL_05, 0x27020005, 0xFFFFFFFF},
	{COMB_CTRL_06, 0x01A60A0D, 0xFFFF1FFF},
	{COMB_CTRL_07, 0x01FC1500, 0x03FFFFFF},
	{COMB_CTRL_08, 0x0111B106, 0x7F1FF1FF},
	{COMB_CTRL_09, 0x38E3200B, 0x7FF3FFFF},
	{COMB_CTRL_0A, 0x0A074A36, 0xFFFFFFFF},
	{COMB_CTRL_0B, 0x0A074A36, 0xFFFFFFFF},
	{COMB_CTRL_0C, 0x0000CCBD, 0x01FFFFFF},
	{COMB_CTRL_0D, 0x00000004, 0xFFFFFFFF},
	{COMB_CTRL_0E, 0x4E554E55, 0xFFFFFFFF},
	{COMB2D_00, 0x314859C8, 0xFFFFFFFE},
	{COMB2D_01, 0x809E3001, 0xFFFFFFFF},
	{COMB2D_02, 0x08001020, 0x0FFFFFFF},
	{COMB2D_03, 0x08081E03, 0xFFFFFF7F},
	{COMB2D_04, 0x00100008, 0xFFFF03FF},
	{COMB2D_05, 0x14061210, 0xFFFFFFFF},
	{COMB2D_06, 0x10101022, 0xFFFFFFFF},
	{COMB2D_07, 0x20502010, 0x7FFFFFFF},
	{COMB2D_08, 0x3311001A, 0xFFFFFFFF},
	{COMB2D_09, 0x1111046A, 0xFFFFFFFF},
	{COMB2D_0A, 0x8000080A, 0xFC7FFFFF},
	{COMB2D_0B, 0x01245678, 0xFFFFFFFF},
	{COMB2D_0C, 0x02345678, 0xFFFFFFFF},
	{COMB2D_0D, 0x0008300A, 0xCFFFFFFF},
	{COMB2D_0E, 0x00022A34, 0x7F7F7F7F},
	{COMB2D_0F, 0x047C0558, 0xFFFFFF7F},
	{COMB2D_10, 0x00880114, 0x80FF81FF},
	{COMB2D_11, 0x00006A68, 0x7777FFFF},
	{COMB2D_12, 0x000783FF, 0xFFFFFFFF},
	{COMB2D_13, 0x90000000, 0xFFFFFFFF},
	{COMB2D_14, 0x00000000, 0x003FF3FF},
	{COMB2D_15, 0x0F051818, 0x3FFF1F1F},
	{COMB2D_16, 0x00191014, 0xF0FFFF3F},
	{COMB2D_17, 0x23645050, 0xFFFFFFFF},
	{COMB2D_18, 0x64641432, 0xFFFFFFFF},
	{COMB2D_19, 0x1E519696, 0xFFFFFFFF},
	{COMB2D_1A, 0x10169696, 0xFFFFFFFF},
	{COMB2D_1B, 0x00504600, 0xDFFFFFFF},
	{COMB2D_1C, 0x821FFFFF, 0xF31FFFFF},
	{COMB2D_1D, 0x00003359, 0x0000FFFF},
	{COMB3D_00, 0x38081808, 0xFFFFFFFF},
	{COMB3D_01, 0x090A1E05, 0xFFFFFFFF},
	{COMB3D_02, 0x24180810, 0x3FFF1F1F},
	{COMB3D_03, 0x64AA321E, 0xFFFFFFFF},
	{COMB3D_04, 0x32107023, 0xFFFFFFFF},
	{COMB3D_05, 0x0E013F85, 0xFF01FFFF},
	{COMB3D_06, 0x0000B910, 0xFFFFFFFF},
	{COMB3D_07, 0x0064C800, 0x00FFFFFF},
	{COMB3D_08, 0x70680610, 0xFFFFFFFF},
	{COMB3D_09, 0x0000170F, 0xFFFFF7FF},
	{COMB3D_0A, 0x12782880, 0x3FFFFFFF},
	{COMB3D_0B, 0x60084084, 0xFF7FF3FF},
	{COMB3D_0C, 0x1E0F1904, 0xFFFFFFFF},
	{COMB3D_0D, 0x100F0344, 0xFFFF0777},
	{COMB3D_0E, 0x3E505F00, 0xFFFFFFFF},
	{COMB3D_0F, 0x04414444, 0x0FFFFFFF},
	{COMB3D_10, 0x00110333, 0xFFFFFFFF},
	{COMB3D_11, 0x00A404F3, 0x0FFFFFFF},
	{COMB3D_12, 0x45195030, 0xFFFFFFFF},
	{COMB3D_13, 0x0640285C, 0x0FFBFFFF},
	{COMB3D_14, 0x11600811, 0xFFFFF8FF},
	{COMB3D_15, 0x11111111, 0xFFFFFFFF},
	{COMB3D_16, 0x0008FC00, 0xFF08FFFF},
	{COMB3D_17, 0x00000000, 0x33FFFFFF},
	{COMB3D_18, 0xC0031008, 0xC0FFFFFF},
	{COMB3D_19, 0x141420FF, 0xFFFFFFFF},
	{COMB3D_1A, 0x32200060, 0xFF7F3FFF},
	{COMB3D_1B, 0x20906000, 0xFFFFFFFF},
	{COMB3D_1C, 0x0000000D, 0x000000FF},
	{COMB3D_1D, 0x43174537, 0xF7FFFF7F},
	{COMB3D_1E, 0x0045633C, 0x0FFFFFFF},
	{COMB3D_1F, 0xC0001A50, 0xFFF03FFF},
	{COMB3D_20, 0x0414050D, 0x7F7F3F7F},
	{COMB3D_21, 0x00000050, 0xFFFFFFFF},
	{COMB3D_22, 0x64000450, 0xFFCFFFFF},
	{COMB3D_23, 0x410A0028, 0xC73FFFFF},
	{COMB3D_24, 0x00014455, 0x0001FFFF},
	{COMB3D_25, 0x00000000, 0xFFFFFFFF},
    {REGTBL_END,   0x00000000, 0x00000000},
};
REGTBL_T const CODE REGTBL_COMB_NTSC443[] = { 
	{COMB_CTRL_00, 0x000B101F, 0x1FFFFFFF},
	{COMB_CTRL_01, 0x81B55000, 0xFE000000},
	{COMB_CTRL_02, 0x02000000, 0xFFFFFFFF},
	{COMB_CTRL_03, 0x00000213, 0xFFFFFFFF},
	{COMB_CTRL_04, 0x00000000, 0x8FFFFFFF},
	{COMB_CTRL_05, 0x2F040438, 0xFFFFFFFF},
	{COMB_CTRL_06, 0x021E1E74, 0xFFFF1FFF},
	{COMB_CTRL_07, 0x02641900, 0x03FFFFFF},
	{COMB_CTRL_08, 0x01151139, 0x7F1FF1FF},
	{COMB_CTRL_09, 0x46F3C00B, 0x7FF3FFFF},
	{COMB_CTRL_0A, 0x0A09052E, 0xFFFFFFFF},
	{COMB_CTRL_0B, 0x0A09052E, 0xFFFFFFFF},
	{COMB_CTRL_0C, 0x00000000, 0x01FFFFFF},
	{COMB_CTRL_0D, 0x00000004, 0xFFFFFFFF},
	{COMB_CTRL_0E, 0xE7787300, 0xFFFFFFFF},
	{COMB2D_00, 0x314059C0, 0xFFFFFFFE},
	{COMB2D_01, 0x801E0000, 0xFFFFFFFF},
	{COMB2D_02, 0x08001120, 0x0FFFFFFF},
	{COMB2D_03, 0x08081E03, 0xFFFFFF7F},
	{COMB2D_04, 0x00100008, 0xFFFF03FF},
	{COMB2D_05, 0x07060A10, 0xFFFFFFFF},
	{COMB2D_06, 0x10101022, 0xFFFFFFFF},
	{COMB2D_07, 0x20B09000, 0x7FFFFFFF},
	{COMB2D_08, 0x22110A0E, 0xFFFFFFFF},
	{COMB2D_09, 0x1111043A, 0xFFFFFFFF},
	{COMB2D_0A, 0x8000080A, 0xFC7FFFFF},
	{COMB2D_0B, 0x12345678, 0xFFFFFFFF},
	{COMB2D_0C, 0x02345678, 0xFFFFFFFF},
	{COMB2D_0D, 0x10080014, 0xCFFFFFFF},
	{COMB2D_0E, 0x00020A34, 0x7F7F7F7F},
	{COMB2D_0F, 0x047C0558, 0xFFFFFF7F},
	{COMB2D_10, 0x0088011A, 0x80FF81FF},
	{COMB2D_11, 0x00009666, 0x7777FFFF},
	{COMB2D_12, 0x000781FF, 0xFFFFFFFF},
	{COMB2D_13, 0x90000000, 0xFFFFFFFF},
	{COMB2D_14, 0x000073DA, 0x003FF3FF},
	{COMB2D_15, 0x0F051818, 0x3FFF1F1F},
	{COMB2D_16, 0x00191014, 0xF0FFFF3F},
	{COMB2D_17, 0x23645050, 0xFFFFFFFF},
	{COMB2D_18, 0x64641432, 0xFFFFFFFF},
	{COMB2D_19, 0x1E519696, 0xFFFFFFFF},
	{COMB2D_1A, 0x10169696, 0xFFFFFFFF},
	{COMB2D_1B, 0x053C501E, 0xDFFFFFFF},
	{COMB2D_1C, 0x820A325A, 0xF31FFFFF},
	{COMB2D_1D, 0x00003359, 0x0000FFFF},
	{COMB3D_00, 0x38081808, 0xFFFFFFFF},
	{COMB3D_01, 0x090A1E00, 0xFFFFFFFF},
	{COMB3D_02, 0x24180810, 0x3FFF1F1F},
	{COMB3D_03, 0x64AA321E, 0xFFFFFFFF},
	{COMB3D_04, 0x32107023, 0xFFFFFFFF},
	{COMB3D_05, 0x0F013F85, 0xFF01FFFF},
	{COMB3D_06, 0x0000B910, 0xFFFFFFFF},
	{COMB3D_07, 0x0064C800, 0x00FFFFFF},
	{COMB3D_08, 0x00480610, 0xFFFFFFFF},
	{COMB3D_09, 0x0000900F, 0xFFFFF7FF},
	{COMB3D_0A, 0x10782880, 0x3FFFFFFF},
	{COMB3D_0B, 0x60084084, 0xFF7FF3FF},
	{COMB3D_0C, 0x1E0F1904, 0xFFFFFFFF},
	{COMB3D_0D, 0x10080344, 0xFFFF0777},
	{COMB3D_0E, 0x3E505F07, 0xFFFFFFFF},
	{COMB3D_0F, 0x04414444, 0x0FFFFFFF},
	{COMB3D_10, 0x00110333, 0xFFFFFFFF},
	{COMB3D_11, 0x00A404F3, 0x0FFFFFFF},
	{COMB3D_12, 0x45195030, 0xFFFFFFFF},
	{COMB3D_13, 0x0640285C, 0x0FFBFFFF},
	{COMB3D_14, 0x11600811, 0xFFFFF8FF},
	{COMB3D_15, 0x11111111, 0xFFFFFFFF},
	{COMB3D_16, 0x00089C00, 0xFF08FFFF},
	{COMB3D_17, 0x00000000, 0x33FFFFFF},
	{COMB3D_18, 0x00071004, 0xC0FFFFFF},
	{COMB3D_19, 0x141420FF, 0xFFFFFFFF},
	{COMB3D_1A, 0x32200060, 0xFF7F3FFF},
	{COMB3D_1B, 0x20A06000, 0xFFFFFFFF},
	{COMB3D_1C, 0x00000014, 0x000000FF},
	{COMB3D_1D, 0x43174537, 0xF7FFFF7F},
	{COMB3D_1E, 0x0045713C, 0x0FFFFFFF},
	{COMB3D_1F, 0xC0001A50, 0xFFF03FFF},
	{COMB3D_20, 0x4414050D, 0x7F7F3F7F},
	{COMB3D_21, 0x1D7A0020, 0xFFFFFFFF},
	{COMB3D_22, 0x64000450, 0xFFCFFFFF},
	{COMB3D_23, 0x410A0028, 0xC73FFFFF},
	{COMB3D_24, 0x00014455, 0x0001FFFF},
	{COMB3D_25, 0x00000000, 0xFFFFFFFF},
	{REGTBL_END, 0x00000000, 0x00000000},
    };
REGTBL_T const CODE REGTBL_COMB_PAL_60[] = {
	{COMB_CTRL_00, 0x000B101F, 0x1FFFFFFF},
	{COMB_CTRL_01, 0x09B55000, 0xFE000000},
	{COMB_CTRL_02, 0x02000000, 0xFFFFFFFF},
	{COMB_CTRL_03, 0x00000213, 0xFFFFFFFF},
	{COMB_CTRL_04, 0x03980000, 0x8FFFFFFF},
	{COMB_CTRL_05, 0x31044000, 0xFFFFFFFF},
	{COMB_CTRL_06, 0x021E1E74, 0xFFFF1FFF},
	{COMB_CTRL_07, 0x02641900, 0x03FFFFFF},
	{COMB_CTRL_08, 0x01151139, 0x7F1FF1FF},
	{COMB_CTRL_09, 0x46F3C00B, 0x7FF3FFFF},
	{COMB_CTRL_0A, 0x0A09073F, 0xFFFFFFFF},
	{COMB_CTRL_0B, 0x0A09073F, 0xFFFFFFFF},
	{COMB_CTRL_0C, 0x00000000, 0x01FFFFFF},
	{COMB_CTRL_0D, 0x00000007, 0xFFFFFFFF},
	{COMB_CTRL_0E, 0xF37D7800, 0xFFFFFFFF},
	{COMB2D_00, 0x31501A48, 0xFFFFFFFE},
	{COMB2D_01, 0x8C9F0000, 0xFFFFFFFF},
	{COMB2D_02, 0x08001020, 0x0FFFFFFF},
	{COMB2D_03, 0x08081E14, 0xFFFFFF7F},
	{COMB2D_04, 0x0A100008, 0xFFFF03FF},
	{COMB2D_05, 0x14061608, 0xFFFFFFFF},
	{COMB2D_06, 0x0C0A2808, 0xFFFFFFFF},
	{COMB2D_07, 0x0A149000, 0x7FFFFFFF},
	{COMB2D_08, 0x55000019, 0xFFFFFFFF},
	{COMB2D_09, 0x0311041A, 0xFFFFFFFF},
	{COMB2D_0A, 0x8C0A100A, 0xFC7FFFFF},
	{COMB2D_0B, 0x01245678, 0xFFFFFFFF},
	{COMB2D_0C, 0x02345678, 0xFFFFFFFF},
	{COMB2D_0D, 0x10080014, 0xCFFFFFFF},
	{COMB2D_0E, 0x00020A34, 0x7F7F7F7F},
	{COMB2D_0F, 0x047C0558, 0xFFFFFF7F},
	{COMB2D_10, 0x0088011A, 0x80FF81FF},
	{COMB2D_11, 0x00009666, 0x7777FFFF},
	{COMB2D_12, 0x000781FF, 0xFFFFFFFF},
	{COMB2D_13, 0x90000000, 0xFFFFFFFF},
	{COMB2D_14, 0x000073DA, 0x003FF3FF},
	{COMB2D_15, 0x0F051818, 0x3FFF1F1F},
	{COMB2D_16, 0x00191014, 0xF0FFFF3F},
	{COMB2D_17, 0x23645050, 0xFFFFFFFF},
	{COMB2D_18, 0x64641432, 0xFFFFFFFF},
	{COMB2D_19, 0x1E519696, 0xFFFFFFFF},
	{COMB2D_1A, 0x10169696, 0xFFFFFFFF},
	{COMB2D_1B, 0x053C501E, 0xDFFFFFFF},
	{COMB2D_1C, 0x820A325A, 0xF31FFFFF},
	{COMB2D_1D, 0x00003359, 0x0000FFFF},
	{COMB3D_00, 0x4A101030, 0xFFFFFFFF},
	{COMB3D_01, 0x580A1E05, 0xFFFFFFFF},
	{COMB3D_02, 0x2018040C, 0x3FFF1F1F},
	{COMB3D_03, 0xC896321E, 0xFFFFFFFF},
	{COMB3D_04, 0x00107023, 0xFFFFFFFF},
	{COMB3D_05, 0x0F01EF85, 0xFF01FFFF},
	{COMB3D_06, 0x0070500C, 0xFFFFFFFF},
	{COMB3D_07, 0x00641400, 0x00FFFFFF},
	{COMB3D_08, 0x00300500, 0xFFFFFFFF},
	{COMB3D_09, 0x0000230A, 0xFFFFF7FF},
	{COMB3D_0A, 0x1E702808, 0x3FFFFFFF},
	{COMB3D_0B, 0x80064078, 0xFF7FF3FF},
	{COMB3D_0C, 0x54280504, 0xFFFFFFFF},
	{COMB3D_0D, 0x200F0000, 0xFFFF0777},
	{COMB3D_0E, 0x00969600, 0xFFFFFFFF},
	{COMB3D_0F, 0x04044444, 0x0FFFFFFF},
	{COMB3D_10, 0x0C010444, 0xFFFFFFFF},
	{COMB3D_11, 0x00A444F3, 0x0FFFFFFF},
	{COMB3D_12, 0x3E1A5353, 0xFFFFFFFF},
	{COMB3D_13, 0x064B1478, 0x0FFBFFFF},
	{COMB3D_14, 0x1020D831, 0xFFFFF8FF},
	{COMB3D_15, 0x11111111, 0xFFFFFFFF},
	{COMB3D_16, 0xC008FC10, 0xFF08FFFF},
	{COMB3D_17, 0x00000000, 0x33FFFFFF},
	{COMB3D_18, 0xC0001020, 0xC0FFFFFF},
	{COMB3D_19, 0x0F371858, 0xFFFFFFFF},
	{COMB3D_1A, 0x3220008F, 0xFF7F3FFF},
	{COMB3D_1B, 0xA02D0E28, 0xFFFFFFFF},
	{COMB3D_1C, 0x00000054, 0x000000FF},
	{COMB3D_1D, 0x43174537, 0xF7FFFF7F},
	{COMB3D_1E, 0x0045713C, 0x0FFFFFFF},
	{COMB3D_1F, 0xC0001A50, 0xFFF03FFF},
	{COMB3D_20, 0x4414050D, 0x7F7F3F7F},
	{COMB3D_21, 0x1D7A0020, 0xFFFFFFFF},
	{COMB3D_22, 0x64000450, 0xFFCFFFFF},
	{COMB3D_23, 0x410A0028, 0xC73FFFFF},
	{COMB3D_24, 0x00014455, 0x0001FFFF},
	{COMB3D_25, 0x00000000, 0xFFFFFFFF},
	{REGTBL_END, 0x00000000, 0x00000000},
        };

REGTBL_T const CODE REGTBL_COMB_PAL_AV[] = {
	{COMB_CTRL_00, 0x000B101F, 0x1FFFFFFF},
	{COMB_CTRL_01, 0x08000000, 0xFE000000},
	{COMB_CTRL_02, 0x020493B3, 0xFFFFFFFF},
	{COMB_CTRL_03, 0x2DFF0213, 0xFFFFFFFF},
	{COMB_CTRL_04, 0x03A80100, 0x8FFFFFFF},
	{COMB_CTRL_05, 0x3135400A, 0xFFFFFFFF},
	{COMB_CTRL_06, 0x02361E74, 0xFFFF1FFF},
	{COMB_CTRL_07, 0x02441800, 0x03FFFFFF},
	{COMB_CTRL_08, 0x01151139, 0x7F1FF1FF},
	{COMB_CTRL_09, 0x46F3C00B, 0x7FF3FFFF},
	{COMB_CTRL_0A, 0x0A0AD303, 0xFFFFFFFF},
	{COMB_CTRL_0B, 0x0A0AD303, 0xFFFFFFFF},
	{COMB_CTRL_0C, 0x000098C4, 0x01FFFFFF},
	{COMB_CTRL_0D, 0x00000007, 0xFFFFFFFF},
	{COMB_CTRL_0E, 0x4E554E55, 0xFFFFFFFF},
	{COMB2D_00, 0x31581E48, 0xFFFFFFFE},
	{COMB2D_01, 0x8C9B3000, 0xFFFFFFFF},
	{COMB2D_02, 0x08101020, 0x0FFFFFFF},
	{COMB2D_03, 0x08081E14, 0xFFFFFF7F},
	{COMB2D_04, 0x0A100008, 0xFFFF03FF},
	{COMB2D_05, 0x14060208, 0xFFFFFFFF},
	{COMB2D_06, 0x0C0A2808, 0xFFFFFFFF},
	{COMB2D_07, 0x0A0D9226, 0x7FFFFFFF},
	{COMB2D_08, 0x33112029, 0xFFFFFFFF},
	{COMB2D_09, 0x1311042A, 0xFFFFFFFF},
	{COMB2D_0A, 0x8C03100A, 0xFC7FFFFF},
	{COMB2D_0B, 0x12345678, 0xFFFFFFFF},
	{COMB2D_0C, 0x02345678, 0xFFFFFFFF},
	{COMB2D_0D, 0x10081414, 0xCFFFFFFF},
	{COMB2D_0E, 0x00030A10, 0x7F7F7F7F},
	{COMB2D_0F, 0x047C0558, 0xFFFFFF7F},
	{COMB2D_10, 0x0088011A, 0x80FF81FF},
	{COMB2D_11, 0x00009666, 0x7777FFFF},
	{COMB2D_12, 0x000781FF, 0xFFFFFFFF},
	{COMB2D_13, 0x90000000, 0xFFFFFFFF},
	{COMB2D_14, 0x00000000, 0x003FF3FF},
	{COMB2D_15, 0x0F051818, 0x3FFF1F1F},
	{COMB2D_16, 0x00191014, 0xF0FFFF3F},
	{COMB2D_17, 0x23645050, 0xFFFFFFFF},
	{COMB2D_18, 0x64641432, 0xFFFFFFFF},
	{COMB2D_19, 0x1E519696, 0xFFFFFFFF},
	{COMB2D_1A, 0x10169696, 0xFFFFFFFF},
	{COMB2D_1B, 0x053C501E, 0xDFFFFFFF},
	{COMB2D_1C, 0x820A325A, 0xF31FFFFF},
	{COMB2D_1D, 0x00003359, 0x0000FFFF},
	{COMB3D_00, 0x4A101030, 0xFFFFFFFF},
	{COMB3D_01, 0x510A1E05, 0xFFFFFFFF},
	{COMB3D_02, 0x2218040C, 0x3FFF1F1F},
	{COMB3D_03, 0x8296321E, 0xFFFFFFFF},
	{COMB3D_04, 0x00107010, 0xFFFFFFFF},
	{COMB3D_05, 0x0F01EF85, 0xFF01FFFF},
	{COMB3D_06, 0x1070500C, 0xFFFFFFFF},
	{COMB3D_07, 0x00641400, 0x00FFFFFF},
	{COMB3D_08, 0x00300500, 0xFFFFFFFF},
	{COMB3D_09, 0x0000230A, 0xFFFFF7FF},
	{COMB3D_0A, 0x1E712808, 0x3FFFFFFF},
	{COMB3D_0B, 0x8006408A, 0xFF7FF3FF},
	{COMB3D_0C, 0x54280000, 0xFFFFFFFF},
	{COMB3D_0D, 0x200F0000, 0xFFFF0777},
	{COMB3D_0E, 0x00969600, 0xFFFFFFFF},
	{COMB3D_0F, 0x04044444, 0x0FFFFFFF},
	{COMB3D_10, 0x0C010444, 0xFFFFFFFF},
	{COMB3D_11, 0x00A444F3, 0x0FFFFFFF},
	{COMB3D_12, 0x3E1A53D3, 0xFFFFFFFF},
	{COMB3D_13, 0x064F1478, 0x0FFBFFFF},
	{COMB3D_14, 0x1020D831, 0xFFFFF8FF},
	{COMB3D_15, 0x11111111, 0xFFFFFFFF},
	{COMB3D_16, 0xC008FC10, 0xFF08FFFF},
	{COMB3D_17, 0x00000000, 0x33FFFFFF},
	{COMB3D_18, 0xC0001020, 0xC0FFFFFF},
	{COMB3D_19, 0x0F271858, 0xFFFFFFFF},
	{COMB3D_1A, 0x3220008F, 0xFF7F3FFF},
	{COMB3D_1B, 0xA02D0E28, 0xFFFFFFFF},
	{COMB3D_1C, 0x00000054, 0x000000FF},
	{COMB3D_1D, 0x43174537, 0xF7FFFF7F},
	{COMB3D_1E, 0x0045713C, 0x0FFFFFFF},
	{COMB3D_1F, 0xC0001A50, 0xFFF03FFF},
	{COMB3D_20, 0x4414050D, 0x7F7F3F7F},
	{COMB3D_21, 0x068A0020, 0xFFFFFFFF},
	{COMB3D_22, 0x64800450, 0xFFCFFFFF},
	{COMB3D_23, 0x410A0028, 0xC73FFFFF},
	{COMB3D_24, 0x00014455, 0x0001FFFF},
	{COMB3D_25, 0x00000000, 0xFFFFFFFF},
	{REGTBL_END, 0x00000000, 0x00000000},

};

REGTBL_T const CODE REGTBL_COMB_PAL_M_AV[] = {
	{COMB_CTRL_00, 0x020B101F, 0x1FFFFFFF},
	{COMB_CTRL_01, 0x89B55000, 0xFE000000},
	{COMB_CTRL_02, 0x020482F0, 0xFFFFFFFF},
	{COMB_CTRL_03, 0x23040213, 0xFFFFFFFF},
	{COMB_CTRL_04, 0x02F40000, 0x8FFFFFFF},
	{COMB_CTRL_05, 0x28034404, 0xFFFFFFFF},
	{COMB_CTRL_06, 0x01C30A0C, 0xFFFF1FFF},
	{COMB_CTRL_07, 0x01FA1600, 0x03FFFFFF},
	{COMB_CTRL_08, 0x0111D107, 0x7F1FF1FF},
	{COMB_CTRL_09, 0x38D2F80B, 0x7FF3FFFF},
	{COMB_CTRL_0A, 0x0A074829, 0xFFFFFFFF},
	{COMB_CTRL_0B, 0x20074829, 0xFFFFFFFF},
	{COMB_CTRL_0C, 0x00000000, 0x01FFFFFF},
	{COMB_CTRL_0D, 0x00000007, 0xFFFFFFFF},
	{COMB_CTRL_0E, 0xF37F7900, 0xFFFFFFFF},
	{COMB2D_00, 0x31581A40, 0xFFFFFFFE},
	{COMB2D_01, 0x849F0000, 0xFFFFFFFF},
	{COMB2D_02, 0x08001020, 0x0FFFFFFF},
	{COMB2D_03, 0x08081E14, 0xFFFFFF7F},
	{COMB2D_04, 0x0A100008, 0xFFFF03FF},
	{COMB2D_05, 0x50061608, 0xFFFFFFFF},
	{COMB2D_06, 0x0C0A2808, 0xFFFFFFFF},
	{COMB2D_07, 0x0A149000, 0x7FFFFFFF},
	{COMB2D_08, 0x55000019, 0xFFFFFFFF},
	{COMB2D_09, 0x0311041A, 0xFFFFFFFF},
	{COMB2D_0A, 0x8C0A100A, 0xFC7FFFFF},
	{COMB2D_0B, 0x01245678, 0xFFFFFFFF},
	{COMB2D_0C, 0x02345678, 0xFFFFFFFF},
	{COMB2D_0D, 0x10080014, 0xCFFFFFFF},
	{COMB2D_0E, 0x00020A34, 0x7F7F7F7F},
	{COMB2D_0F, 0x047C0558, 0xFFFFFF7F},
	{COMB2D_10, 0x0088011A, 0x80FF81FF},
	{COMB2D_11, 0x00009666, 0x7777FFFF},
	{COMB2D_12, 0x000781FF, 0xFFFFFFFF},
	{COMB2D_13, 0x90000000, 0xFFFFFFFF},
	{COMB2D_14, 0x000073DA, 0x003FF3FF},
	{COMB2D_15, 0x0F051818, 0x3FFF1F1F},
	{COMB2D_16, 0x00191014, 0xF0FFFF3F},
	{COMB2D_17, 0x23645050, 0xFFFFFFFF},
	{COMB2D_18, 0x64641432, 0xFFFFFFFF},
	{COMB2D_19, 0x1E519696, 0xFFFFFFFF},
	{COMB2D_1A, 0x10169696, 0xFFFFFFFF},
	{COMB2D_1B, 0x053C501E, 0xDFFFFFFF},
	{COMB2D_1C, 0x820A325A, 0xF31FFFFF},
	{COMB2D_1D, 0x00003359, 0x0000FFFF},
	{COMB3D_00, 0x4A101030, 0xFFFFFFFF},
	{COMB3D_01, 0x480A1E2A, 0xFFFFFFFF},
	{COMB3D_02, 0x3018040C, 0x3FFF1F1F},
	{COMB3D_03, 0xC896321E, 0xFFFFFFFF},
	{COMB3D_04, 0x00107023, 0xFFFFFFFF},
	{COMB3D_05, 0x0F00EF85, 0xFF01FFFF},
	{COMB3D_06, 0x0070500C, 0xFFFFFFFF},
	{COMB3D_07, 0x00641400, 0x00FFFFFF},
	{COMB3D_08, 0x00300500, 0xFFFFFFFF},
	{COMB3D_09, 0x0000A00A, 0xFFFFF7FF},
	{COMB3D_0A, 0x1E702808, 0x3FFFFFFF},
	{COMB3D_0B, 0x80064078, 0xFF7FF3FF},
	{COMB3D_0C, 0x54280504, 0xFFFFFFFF},
	{COMB3D_0D, 0x20100000, 0xFFFF0777},
	{COMB3D_0E, 0x00969600, 0xFFFFFFFF},
	{COMB3D_0F, 0x04044444, 0x0FFFFFFF},
	{COMB3D_10, 0x0C010444, 0xFFFFFFFF},
	{COMB3D_11, 0x00A444F3, 0x0FFFFFFF},
	{COMB3D_12, 0x3E1A5353, 0xFFFFFFFF},
	{COMB3D_13, 0x064B1478, 0x0FFBFFFF},
	{COMB3D_14, 0x1020D831, 0xFFFFF8FF},
	{COMB3D_15, 0x11111111, 0xFFFFFFFF},
	{COMB3D_16, 0xC008FC10, 0xFF08FFFF},
	{COMB3D_17, 0x00000020, 0x33FFFFFF},
	{COMB3D_18, 0xC0001020, 0xC0FFFFFF},
	{COMB3D_19, 0x0F371858, 0xFFFFFFFF},
	{COMB3D_1A, 0x3220008F, 0xFF7F3FFF},
	{COMB3D_1B, 0xB0210E28, 0xFFFFFFFF},
	{COMB3D_1C, 0x00000014, 0x000000FF},
	{COMB3D_1D, 0x43174537, 0xF7FFFF7F},
	{COMB3D_1E, 0x0045713C, 0x0FFFFFFF},
	{COMB3D_1F, 0xC0001A50, 0xFFF03FFF},
	{COMB3D_20, 0x4414050D, 0x7F7F3F7F},
	{COMB3D_21, 0x1D7A0020, 0xFFFFFFFF},
	{COMB3D_22, 0x64000450, 0xFFCFFFFF},
	{COMB3D_23, 0x410A0028, 0xC73FFFFF},
	{COMB3D_24, 0x00014455, 0x0001FFFF},
	{COMB3D_25, 0x00000000, 0xFFFFFFFF},
	{REGTBL_END, 0x00000000, 0x00000000}
};

REGTBL_T const CODE REGTBL_COMB_PAL_N_AV[] = { 
 {COMB_CTRL_00, 0x020B101F, 0x1FFFFFFF},
 {COMB_CTRL_01, 0x89B63800, 0xFE000000},
 {COMB_CTRL_02, 0x0204E2EA, 0xFFFFFFFF},
 {COMB_CTRL_03, 0x2D360213, 0xFFFFFFFF},
 {COMB_CTRL_04, 0x02F40000, 0x8FFFFFFF},
 {COMB_CTRL_05, 0x31354408, 0xFFFFFFFF},
 {COMB_CTRL_06, 0x01CC1A71, 0xFFFF1FFF},
 {COMB_CTRL_07, 0x02611900, 0x03FFFFFF},
 {COMB_CTRL_08, 0x01151139, 0x7F1FF1FF},
 {COMB_CTRL_09, 0x3953160B, 0x7FF3FFFF},
 {COMB_CTRL_0A, 0x0A08BEC9, 0xFFFFFFFF},
 {COMB_CTRL_0B, 0x2008BEC9, 0xFFFFFFFF},
 {COMB_CTRL_0C, 0x00000000, 0x01FFFFFF},
 {COMB_CTRL_0D, 0x00000007, 0xFFFFFFFF},
 {COMB_CTRL_0E, 0xD8736D00, 0xFFFFFFFF},
 {COMB2D_00, 0x31581A40, 0xFFFFFFFE},
 {COMB2D_01, 0x849F0000, 0xFFFFFFFF},
 {COMB2D_02, 0x08001020, 0x0FFFFFFF},
 {COMB2D_03, 0x08081E14, 0xFFFFFF7F},
 {COMB2D_04, 0x0A100008, 0xFFFF03FF},
 {COMB2D_05, 0x50061608, 0xFFFFFFFF},
 {COMB2D_06, 0x0C0A2808, 0xFFFFFFFF},
 {COMB2D_07, 0x0A149000, 0x7FFFFFFF},
 {COMB2D_08, 0x55000019, 0xFFFFFFFF},
 {COMB2D_09, 0x0311041A, 0xFFFFFFFF},
 {COMB2D_0A, 0x8C0A100A, 0xFC7FFFFF},
 {COMB2D_0B, 0x01245678, 0xFFFFFFFF},
 {COMB2D_0C, 0x02345678, 0xFFFFFFFF},
 {COMB2D_0D, 0x10080014, 0xCFFFFFFF},
 {COMB2D_0E, 0x00020A34, 0x7F7F7F7F},
 {COMB2D_0F, 0x047C0558, 0xFFFFFF7F},
 {COMB2D_10, 0x0088011A, 0x80FF81FF},
 {COMB2D_11, 0x00009666, 0x7777FFFF},
 {COMB2D_12, 0x000781FF, 0xFFFFFFFF},
 {COMB2D_13, 0x90000000, 0xFFFFFFFF},
 {COMB2D_14, 0x000073DA, 0x003FF3FF},
 {COMB2D_15, 0x0F051818, 0x3FFF1F1F},
 {COMB2D_16, 0x00191014, 0xF0FFFF3F},
 {COMB2D_17, 0x23645050, 0xFFFFFFFF},
 {COMB2D_18, 0x64641432, 0xFFFFFFFF},
 {COMB2D_19, 0x1E519696, 0xFFFFFFFF},
 {COMB2D_1A, 0x10169696, 0xFFFFFFFF},
 {COMB2D_1B, 0x053C501E, 0xDFFFFFFF},
 {COMB2D_1C, 0x820A325A, 0xF31FFFFF},
 {COMB2D_1D, 0x00003359, 0x0000FFFF},
 {COMB3D_00, 0x4A101030, 0xFFFFFFFF},
 {COMB3D_01, 0x480A1E2A, 0xFFFFFFFF},
 {COMB3D_02, 0x3018040C, 0x3FFF1F1F},
 {COMB3D_03, 0xC896321E, 0xFFFFFFFF},
 {COMB3D_04, 0x00107023, 0xFFFFFFFF},
 {COMB3D_05, 0x0F00EF85, 0xFF01FFFF},
 {COMB3D_06, 0x0070500C, 0xFFFFFFFF},
 {COMB3D_07, 0x00641400, 0x00FFFFFF},
 {COMB3D_08, 0x00300500, 0xFFFFFFFF},
 {COMB3D_09, 0x0000A00A, 0xFFFFF7FF},
 {COMB3D_0A, 0x1E702808, 0x3FFFFFFF},
 {COMB3D_0B, 0x80064078, 0xFF7FF3FF},
 {COMB3D_0C, 0x54280504, 0xFFFFFFFF},
 {COMB3D_0D, 0x20100000, 0xFFFF0777},
 {COMB3D_0E, 0x00969600, 0xFFFFFFFF},
 {COMB3D_0F, 0x04044444, 0x0FFFFFFF},
 {COMB3D_10, 0x0C010444, 0xFFFFFFFF},
 {COMB3D_11, 0x00A444F3, 0x0FFFFFFF},
 {COMB3D_12, 0x3E1A5353, 0xFFFFFFFF},
 {COMB3D_13, 0x064B1478, 0x0FFBFFFF},
 {COMB3D_14, 0x1020D831, 0xFFFFF8FF},
 {COMB3D_15, 0x11111111, 0xFFFFFFFF},
 {COMB3D_16, 0xC008FC10, 0xFF08FFFF},
 {COMB3D_17, 0x00000020, 0x33FFFFFF},
 {COMB3D_18, 0xC0001020, 0xC0FFFFFF},
 {COMB3D_19, 0x0F371858, 0xFFFFFFFF},
 {COMB3D_1A, 0x3220008F, 0xFF7F3FFF},
 {COMB3D_1B, 0xB0210E28, 0xFFFFFFFF},
 {COMB3D_1C, 0x00000014, 0x000000FF},
 {COMB3D_1D, 0x43174537, 0xF7FFFF7F},
 {COMB3D_1E, 0x0045713C, 0x0FFFFFFF},
 {COMB3D_1F, 0xC0001A50, 0xFFF03FFF},
 {COMB3D_20, 0x4414050D, 0x7F7F3F7F},
 {COMB3D_21, 0x1D7A0020, 0xFFFFFFFF},
 {COMB3D_22, 0x64000450, 0xFFCFFFFF},
 {COMB3D_23, 0x410A0028, 0xC73FFFFF},
 {COMB3D_24, 0x00014455, 0x0001FFFF},
 {COMB3D_25, 0x00000000, 0xFFFFFFFF},
 {REGTBL_END, 0x00000000, 0x00000000},
};

REGTBL_T const CODE REGTBL_COMB_SV[] = {
    {COMB_CTRL_02, 0x0004A2EE, 0x80FFFFFF},

    {COMB2D_02, 0x00000000, 0x0F000000},
    {COMB2D_00, 0x00000000, 0xC0000000},
    {COMB3D_0E, 0x00808300, 0x00FCF900},
    {COMB2D_00, 0x00000000, 0x10000000},

    {COMB2D_00, 0X00000000, 0x00000F00},

    {COMB2D_02, 0x00001130, 0x0000FFFF},
    {COMB2D_02, 0x00000000, 0x0C000000},
 
    {COMB2D_08, 0x00000000, 0x00000003},
    {COMB2D_00, 0x00000000, 0x00020000},
	
    {COMB2D_01, 0x00000000, 0x00800000},
    {COMB2D_01, 0x00000000, 0x00600000},
    {COMB2D_04, 0X00000080, 0x000000FF},
    {COMB2D_06, 0x47000000, 0xFF000000},
    {COMB2D_06, 0x00FF0000, 0x00FF0000},

    {COMB_CTRL_01, 0x80000000, 0x80000000},
    {REGTBL_END, 0x00000000, 0x00000000}
};


/**
 * @brief TDC Initial Routine
 *
 * TDC Module Initial Routine
 *
 * @param None
 * @return None
 * @warning
 * @todo
 *
 * @pre System Initial
 * @post TDC can start
 */
void vDrvTDCInit(void)
{
//      _fgTDCDemo = FALSE;
    _bTDCEnable = FALSE;

    vDrvTDCOnOff(SV_OFF);

}

/**
 * @brief TDC On/Off Routine
 * 	  //mtk01140 20090902 TDC onoff switchh control rule
 *		[condition] [TDC need to be set to]
 *	1.	[SCART mode change done, SVideo is ON(vTvd3dSVInit)]				[OFF]
 * 	2.	[dis-connect from a input with TDC enable(vTvd3dConnect)]			[OFF]
 *	3.	[Hardware init => TDC init(vDrvTDCInit)]					[OFF]
 *	4.	[Connet to SCART Svideo(vScartSvConnect)]					[OFF]
 *	5.	[FIFO error occur (bFIFOERR)]							[->OFF->ON] 
 *	6.	[before change TDC DRAM based(vDrvTDCSetDramBase), and (C_COMB_2A, EN3D) is ON]	[OFF]
 *	7.	[after change TDC DRAM based(vDrvTDCSetDramBase), and (C_COMB_2A, EN3D) is ON]	[ON]
 *	8.	[before Change setting of "TDC Bandwidth saving(vDrvTDCSaveBW)"]		[OFF]
 *	9.	[after Change setting of "TDC Bandwidth saving(vDrvTDCSaveBW)"]			[ON]
 *	10.	[before Change setting of "TDC DRAM bit(vDrvTDCSetDramMode)"]			[OFF]
 *	11.	[after Change setting of "TDC DRAM bit(vDrvTDCSetDramMode)"]			[ON] 
 * Switch TDC On/Off
 *
 * @param bOnOff: On/Off TDC
 * @return None
 * @warning
 * @todo
 *
 * @pre
 * @post
 */
void vDrvTDCOnOff(UINT8 bOnOff)
{
    CRIT_STATE_T csState;
    
#if !SUPPORT_3DCOMB
    bOnOff = SV_OFF;
#endif // #if !SUPPORT_3DCOMB


    if (IO32ReadFldAlign(COMB_CTRL_01, DRAMBASEADR))
    {
		csState = x_crit_start();
        if ((bOnOff == SV_ON) && fgDrvTDCEnableChk())
        {
            _bTDCEnable = TRUE;
            //turn off "force dram fifo idle"
            vIO32WriteFldAlign(COMB_CTRL_03, SV_OFF, FSKBACK);
            vIO32WriteFldAlign(COMB_CTRL_02, SV_ON, EN3D);
        }
        else
        {
            _bTDCEnable = FALSE;
            //force dram fifo idle
            vIO32WriteFldAlign(COMB_CTRL_03, SV_ON, FSKBACK);
            vIO32WriteFldAlign(COMB_CTRL_02, SV_OFF, EN3D);
    	}
	    x_crit_end(csState);
    }
    else
    {
        LOG(1,"Zero TDC memory address\n");
    }
} 


/**
 * @brief TDC active window size
 *
 * Set TDC active window
 *
 * @param bOnOff: Active/de-active TDC
 * @return None
 */
void vDrvTDCActive(UINT8 bOnOff)
{
    if (bOnOff)
    {
        switch (_rTvd3dStatus.bTvdMode)
        {
        case SV_CS_PAL:
            // Set active window for PAL
            vIO32WriteFldAlign(COMB_CTRL_02, ACTIVE_WIN_PAL_X_START, HCNT3D);       // Start pixel
            vIO32WriteFldAlign(COMB_CTRL_02, ACTIVE_WIN_PAL_X_LENGTH, HLEN3D);      // h-length
            vIO32WriteFldAlign(COMB_CTRL_03, ACTIVE_WIN_PAL_Y_START, VLINEST);      // start line
            vIO32WriteFldAlign(COMB_CTRL_03, ACTIVE_WIN_PAL_Y_LENGTH, VLINECNT);    // v-length
            vIO32WriteFldAlign(COMB_CTRL_05, ACTIVE_WIN_PAL_Y_LENGTH, VLEN3D_M);    // v-length
            break;
        case SV_CS_PAL_M:
            // Set active window for PAL
            vIO32WriteFldAlign(COMB_CTRL_02, ACTIVE_WIN_PAL_M_X_START, HCNT3D);     // Start pixel
            vIO32WriteFldAlign(COMB_CTRL_02, ACTIVE_WIN_PAL_M_X_LENGTH, HLEN3D);    // h-length
            vIO32WriteFldAlign(COMB_CTRL_03, ACTIVE_WIN_PAL_M_Y_START, VLINEST);    // start line
            vIO32WriteFldAlign(COMB_CTRL_03, ACTIVE_WIN_PAL_M_Y_LENGTH, VLINECNT);  // v-length
            vIO32WriteFldAlign(COMB_CTRL_05, ACTIVE_WIN_PAL_M_Y_LENGTH, VLEN3D_M);  // v-length
            break;
        case SV_CS_PAL_N:
            // Set active window for PAL
            vIO32WriteFldAlign(COMB_CTRL_02, ACTIVE_WIN_PAL_N_X_START, HCNT3D);     // Start pixel
            vIO32WriteFldAlign(COMB_CTRL_02, ACTIVE_WIN_PAL_N_X_LENGTH, HLEN3D);    // h-length
            vIO32WriteFldAlign(COMB_CTRL_03, ACTIVE_WIN_PAL_N_Y_START, VLINEST);    // start line
            vIO32WriteFldAlign(COMB_CTRL_03, ACTIVE_WIN_PAL_N_Y_LENGTH, VLINECNT);  // v-length
            vIO32WriteFldAlign(COMB_CTRL_05, ACTIVE_WIN_PAL_N_Y_LENGTH, VLEN3D_M);  // v-length
            break;
        case SV_CS_NTSC358:
        default:
            // Set active window for NTSC
            vIO32WriteFldAlign(COMB_CTRL_02, ACTIVE_WIN_NTSC_X_START, HCNT3D);      // Start pixel
            vIO32WriteFldAlign(COMB_CTRL_02, ACTIVE_WIN_NTSC_X_LENGTH, HLEN3D);     // h-length
            vIO32WriteFldAlign(COMB_CTRL_03, ACTIVE_WIN_NTSC_Y_START, VLINEST);     // start line
            vIO32WriteFldAlign(COMB_CTRL_03, ACTIVE_WIN_NTSC_Y_LENGTH, VLINECNT);   // v-length
            vIO32WriteFldAlign(COMB_CTRL_05, ACTIVE_WIN_NTSC_Y_LENGTH, VLEN3D_M);   // v-length
            break;
        }
    }
    else
    {
        vIO32WriteFldAlign(COMB_CTRL_02, 0, HCNT3D);        // Start pixel
        vIO32WriteFldAlign(COMB_CTRL_02, 0, HLEN3D);        // h-length
        vIO32WriteFldAlign(COMB_CTRL_03, 0, VLINEST);       // start line
        vIO32WriteFldAlign(COMB_CTRL_03, 0, VLINECNT);      // v-length
    }
}

/**
 * @brief TDC Check Routine for Enable or not. Internal routine for TDC Module
 *
 * @param None
 * @return BOOL: TRUE/FALSE
 * @warning
 * @todo
 */
BOOL fgDrvTDCEnableChk(void)
{
    UINT8 bTmp;

    bTmp = bHwTvdMode();
    if (fgHwTvdSVID() || ((!fgIsMainTvd3d())&&(!fgIsPipTvd3d()))||
        ((bTmp != SV_CS_NTSC358) && (bTmp != SV_CS_PAL)
         && (bTmp != SV_CS_PAL_M) && (bTmp != SV_CS_PAL_N)))
    {
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}

/**
 * @brief Set DRAM mode
 *
 * TDC can be 10-bit or 9-bit mode for DRAM access.
 *
 * @param b10BitMode SV_ON/SV_OFF
 */
void vDrvTDCSetDramMode(UINT8 b10BitMode)
{
    BOOL fgTDCEnabled = SV_FALSE;

    // Store last TDC on/off status.
    fgTDCEnabled = IO32ReadFldAlign(COMB_CTRL_02, EN3D) ? SV_ON : SV_OFF;

    vDrvTDCOnOff(SV_OFF);

    // Turn on 10-bit 3D comb.
    if (b10BitMode)
    {
        vIO32WriteFldAlign(COMB_CTRL_02, SV_ON, B10MODE);
    }
    // Turn on 9-bit 3D comb.
    else
    {
        vIO32WriteFldAlign(COMB_CTRL_02, SV_OFF, B10MODE);
    }

    // Re-enable TDC.
    if (fgTDCEnabled)
    {
        vDrvTDCOnOff(SV_ON);
    }

}

/**
 * @brief TDC Set Parameter
 *
 * Set TDC Parameter for different TV systems.
 *
 * @param None
 * @return None
 * @warning Remember to Off TDC before change setting
 * @todo Confirm TVD timgen setting for phase lock.
 *
 * @pre
 * @post
 */
void vDrvTDCSet(void)
{
	BOOL fgTDCEnabled = SV_FALSE;
    //AAF Setting---
    #ifdef CUST_LCCT_DRV
    switch (_rTvd3dStatus.bTvdMode)
    {
        case SV_CS_PAL:
        case SV_CS_PAL_N:
        case SV_CS_PAL_60:  
        case SV_CS_PAL_M:    
            vTvd3dSetAAF(4);
            break;
        case SV_CS_SECAM:
            vTvd3dSetAAF(0);
            break;
        case SV_CS_NTSC443:
        case SV_CS_NTSC358:  
            vTvd3dSetAAF(3);
        default:
            break;
    }
	#else
    if(_rTvd3dStatus.eSourceType==SV_ST_TV)
    {
        switch (_rTvd3dStatus.bTvdMode)
        {
            case SV_CS_PAL:
            case SV_CS_PAL_N:
            case SV_CS_PAL_60:  
            case SV_CS_PAL_M:    
                vTvd3dSetAAF(4);
                break;
            case SV_CS_SECAM:
                vTvd3dSetAAF(0);
                break;
            case SV_CS_NTSC443:
            case SV_CS_NTSC358:  
                vTvd3dSetAAF(3);
            default:
                break;
        }
    }
    else
    {
        vTvd3dSetAAF(0);
    }
	#endif
    //---AAF Setting
    //YCDelay
    vTvd3dSetYCDelay(0);


    if (fgHwTvdSVID())
    {
        vDrvLoadRegTbl(REGTBL_COMB_SV);
        return;
    }
    // Store last TDC on/off status.    
    fgTDCEnabled = IO32ReadFldAlign(COMB_CTRL_02, EN3D) ? SV_ON : SV_OFF;        
	vDrvTDCOnOff(SV_OFF);

    switch (_rTvd3dStatus.bTvdMode)
    {
    case SV_CS_PAL:
        vDrvLoadRegTbl(REGTBL_COMB_PAL_AV);
        vIO32WriteFldAlign(CTG_00, SV_OFF, BST_0DEG);     // Change TVD CGen color 45 degree .
        break;
    case SV_CS_PAL_M:
        vDrvLoadRegTbl(REGTBL_COMB_PAL_M_AV);
        vIO32WriteFldAlign(CTG_00, SV_OFF, BST_0DEG);     // Change TVD CGen color 45 degree .
        break;
    case SV_CS_PAL_N:
        vDrvLoadRegTbl(REGTBL_COMB_PAL_N_AV);
        vIO32WriteFldAlign(CTG_00, SV_OFF, BST_0DEG);     // Change TVD CGen color 45 degree .
        break;
    case SV_CS_SECAM:
        //Enable CKill bypass Y/C seperation only under not SECAM (To reduce color fleshing).
        vIO32WriteFldAlign(COMB_CTRL_03, SV_OFF, ENYCCKILL);
        vIO32WriteFldAlign(CTG_00, SV_ON, BST_0DEG); 		// Nomatter
        break;
    case SV_CS_NTSC443:
        vDrvLoadRegTbl(REGTBL_COMB_NTSC443);  
        vIO32WriteFldAlign(CTG_00, SV_ON, BST_0DEG);     // Change TVD CGen color 0 degree .
        break;
    case SV_CS_PAL_60:
        vDrvLoadRegTbl(REGTBL_COMB_PAL_60);
        vIO32WriteFldAlign(CTG_00, SV_OFF, BST_0DEG);     // Change TVD CGen color 45 degree .
        break;
    case SV_CS_NTSC358:
    default:
        vDrvLoadRegTbl(REGTBL_COMB_NTSC_AV);
        vIO32WriteFldAlign(CTG_00, SV_ON, BST_0DEG);     // Change TVD CGen color 0 degree .
        break;
    }
    // Re-enable TDC.        
    if (fgTDCEnabled)    
	{        
		vDrvTDCOnOff(SV_ON);    
	}   
}

#if ADAPTIVE_GOH
static void vAdaptiveGOH(void)
{
    // Only enable go 1D for VCR (2D) mode.
    if (IO32ReadFldAlign(STA_COMB_0C, NONVCR3D))
    {
        vIO32WriteFldAlign(COMB2D_01, SV_OFF, ENGOH);
    }
    else
    {
        vIO32WriteFldAlign(COMB2D_01, SV_ON, ENGOH);
    }
}
#endif

#if GOH_FROM_1D
static void vGOHFrom1D(void)
{
    UINT16 wHorPixelValue;
    UINT16 wHorPixelValueDefault;
    wHorPixelValue = IO32ReadFldAlign(TG_STA_05, STA_LCNT);
    switch(_rTvd3dStatus.bTvdMode)
    {
    case SV_CS_PAL_M:
    case SV_CS_NTSC358:
    case SV_CS_PAL_60:
    case SV_CS_NTSC443:
        wHorPixelValueDefault = 1716;
        break;
    default:
        wHorPixelValueDefault = 1728;
    }

    if ((wHorPixelValue >= wHorPixelValueDefault -IO32ReadFldAlign(COMB2D_04, GOHTH))
        &&(wHorPixelValue <= wHorPixelValueDefault +IO32ReadFldAlign(COMB2D_04, GOHTH)))
    {
        vIO32WriteFldAlign(COMB2D_00, SV_OFF, REG_PURE1D);
    }
    else
    {
        vIO32WriteFldAlign(COMB2D_00, SV_ON, REG_PURE1D);
    }
}
#endif

static void vAdaptive3DCombGain(void)
{
    UINT8  bTVDNoiseLevel;
    UINT32 dwTdc3dMotion;
    UINT32 dwTdc3dColorSum;
    
    bTVDNoiseLevel = bTvdSignalStrengthLevel(1);
    dwTdc3dMotion = IO32ReadFldAlign(STA_COMB_05, MOPIXCNTSTA);
    dwTdc3dColorSum = IO32ReadFldAlign(STA_COMB_08, COLORSUMSTA);
    //New 3D function, 3D PAL CVBS diff is so big, the Fluke output Colorbar with Circle, have some shake on the circle
    if ((_rTvd3dStatus.eSourceType==SV_ST_TV)&&(_rTvd3dStatus.bTvdMode == SV_CS_PAL))
    {
        if ((IO32ReadFldAlign(STA_COMB_0C, STILL_FRAME)==1)
            &&(dwTdc3dColorSum >=0x2000000)
            &&(dwTdc3dMotion>0x6000)&&(dwTdc3dMotion<0xF000)
            )
        {
            vIO32WriteFldAlign(COMB3D_0F, 0, REG_D3GAINCV);
            vIO32WriteFldAlign(COMB3D_0F, 2, REG_D3GAINY);
			vIO32WriteFldAlign(COMB3D_06, 0x50, REG_LOSTI_LUMATH);
			vIO32WriteFldAlign(COMB3D_13, 0x14, REG_MOTIONTH);
        }
        else
        {
			//vIO32WriteFldAlign(C_COMB_1B, 4, D3GAIN_CV);
            vIO32WriteFldAlign(COMB3D_0F, 0, REG_D3GAINY);
            
            if ((bTVDNoiseLevel >= 0x3D)&&(bTVDNoiseLevel <= 0x47))
            {
                vIO32WriteFldAlign(COMB3D_0F, 4, REG_D3GAINCV);
                vIO32WriteFldAlign(COMB3D_06, 0x58 +((0x47 -bTVDNoiseLevel)<<4), REG_LOSTI_LUMATH);
                vIO32WriteFldAlign(COMB3D_13, 0x16 +(0x47 -bTVDNoiseLevel), REG_MOTIONTH);
            }
            else if (bTVDNoiseLevel > 0x47)
            {
                vIO32WriteFldAlign(COMB3D_0F, 4, REG_D3GAINCV);
                vIO32WriteFldAlign(COMB3D_06, 0x50, REG_LOSTI_LUMATH);
                vIO32WriteFldAlign(COMB3D_13, 0x14, REG_MOTIONTH);
            }
            else if ((bTVDNoiseLevel == 0x3C))
            {
                vIO32WriteFldAlign(COMB3D_0F, 2, REG_D3GAINCV);
                vIO32WriteFldAlign(COMB3D_06, 0xFF, REG_LOSTI_LUMATH);
                vIO32WriteFldAlign(COMB3D_13, 0x22, REG_MOTIONTH);
            }
            else if ((bTVDNoiseLevel <= 0x3B))
            {
                vIO32WriteFldAlign(COMB3D_0F, 1, REG_D3GAINCV);
                vIO32WriteFldAlign(COMB3D_06, 0xFF, REG_LOSTI_LUMATH);
                vIO32WriteFldAlign(COMB3D_13, 0x22, REG_MOTIONTH);
            }
        }        
	}
}


/**
 * @brief TDC Interrupt Service Routine
 *
 * Measure video content and adaptive tune TDC parameters.
 *
 * @param None
 * @return None
 */
void vTdc3dProc(void)
{
    UINT32 dwTdc3dLumasum;
    UINT32 dwTdc3dMBPixCnt;
    UINT32 dwTdc3dColorEdgeSum;
    UINT32 dwTdc3dLumaEdgeSum;
    UINT8  bMOTH4MB, bLSMOOTH;
    UINT8  bSweepDet, bFIFOERR;
	BOOL fgTDCEnabled = SV_FALSE;
	
    bSweepDet =  IO32ReadFldAlign(STA_COMB_0C, SWEEP_DETECTION);
    dwTdc3dLumasum = IO32ReadFldAlign(STA_COMB_07, LUMASUMSTA);
    dwTdc3dMBPixCnt = IO32ReadFldAlign(STA_COMB_06, MBPIXCNTSTA);
    dwTdc3dColorEdgeSum = IO32ReadFldAlign(STA_COMB_0B, COLOREDGESTA);
    dwTdc3dLumaEdgeSum = IO32ReadFldAlign(STA_COMB_09, LUMAEDGESTA);

    bFIFOERR = ((IO32ReadFldAlign(STA_COMB_0C, FIFO_EVERFULL))||
                 (IO32ReadFldAlign(STA_COMB_0C, FIFOEMPTY))||
                 (IO32ReadFldAlign(STA_COMB_0C, FIFOEMPTY_A))||
                 (IO32ReadFldAlign(STA_COMB_0C, FIFOEMPTY_AA)));

    //UNUSED(bTVDNoiseLevel);
    UNUSED(dwTdc3dLumaEdgeSum);
    UNUSED(dwTdc3dColorEdgeSum);
    UNUSED(dwTdc3dLumasum);
    UNUSED(dwTdc3dMBPixCnt);
    UNUSED(bMOTH4MB);
    UNUSED(bLSMOOTH);
    UNUSED(bSweepDet);  

    if (fgHwTvdSVID()||
        (!fgIsMainTvd3d() && !fgIsPipTvd3d()) ||
        (!fgIsAdaptiveFlgSet(FLG_ADAPTIVE_COMB)) )
    {
        return;
    }

    //Patch for 3dcomb dram
    if(bFIFOERR&&(_rTvd3dStatus.bSigStatus==(UINT8)SV_VDO_STABLE))
    {
        #ifndef CC_ENABLE_AUTO_CHECK_DRAM_PRIO_UNDERFLOW
		    // Re-enable TDC. 
		fgTDCEnabled = IO32ReadFldAlign(COMB_CTRL_02, EN3D) ? SV_ON : SV_OFF;     
        vDrvTDCOnOff(SV_OFF);
        if (fgTDCEnabled)
		{
        	vDrvTDCOnOff(SV_ON);
        }
        #endif
    }

#if ADAPTIVE_GOH
    vAdaptiveGOH();
#endif // #if ADAPTIVE_GOH

#if GOH_FROM_1D
    vGOHFrom1D();
#endif

	vTdcColorPatch();
	if((IO32ReadFldAlign(STA_CDET_00, MODE_TVD3D) == SV_CS_NTSC358))
	{
		vTdcUS14Patch();
	}
	vTdcCrossColorProc();
    vAdaptive3DCombGain();

}


/**
 * @brief TDC Set Dram Base Routine
 *
 * Set TDC Dram Base
 *
 * @param None
 * @return None
 * @warning
 * @todo
 *
 * @pre System Initial
 * @post TDC can start
 */
void vDrvTDCSetDramBase(void)
{
    BOOL fgTDCEnabled = SV_FALSE;

    // Store last TDC on/off status.
    fgTDCEnabled = IO32ReadFldAlign(COMB_CTRL_02, EN3D) ? SV_ON : SV_OFF;
    
    vDrvTDCOnOff(SV_OFF);


#if 1
	if (TDC_DRAM_BASE & 0x40000000)
    {
		vIO32WriteFldAlign(COMB2D_0D, 1, CHANNEL_B_SEL);
    }
    else
    {
		vIO32WriteFldAlign(COMB2D_0D, 0, CHANNEL_B_SEL);
    }
    if (TDC_DRAM_BASE & 0x20000000)
    {
		vIO32WriteFldAlign(COMB2D_0D, 1, DRAMBASEADR_MSB);
    }
    else
    {
		vIO32WriteFldAlign(COMB2D_0D, 0, DRAMBASEADR_MSB);
    }
    // Update TDC Dram base.
    vIO32WriteFldAlign(COMB_CTRL_01, TDC_DRAM_BASE >> 4, DRAMBASEADR);
#endif


    // Re-enable TDC.    
    if (fgTDCEnabled)
    {
        vDrvTDCOnOff(SV_ON);
    }

}
            

UINT8 u1DrvTDCChkDramOvfUdf(void)
{
    return((IO32ReadFldAlign(COMB_CTRL_00, FULLSTA))|
           (IO32ReadFldAlign(COMB_CTRL_00, EMPTYSTA)));
}
void vDrvTDCClearDramStatus(void)
{
    //Clear Status
    vIO32WriteFldMulti(COMB_CTRL_00,
              P_Fld(1, CLRFULL)|
              P_Fld(1, CLREMPTY));
    
    vIO32WriteFldMulti(COMB_CTRL_00,
              P_Fld(0, CLRFULL)|
              P_Fld(0, CLREMPTY));

}

/**
* @brief TDC callback function
* @param u4Arg1 VDP_1/VDP_2
* @param u4Arg2 configuration
* @param u4SramMode sram mode
*/
void vDrvTDCSrmCb(UINT32 u4Arg1, UINT32 u4Arg2, UINT32 u4PDSize, UINT32 u4SramMode)
{
    FBM_POOL_T* prPool;
    #ifndef CC_SUPPORT_RECORD_AV
    prPool = FBM_GetPoolInfoAuto((UCHAR)FBM_POOL_TYPE_TDC, NULL);
	#else
	prPool = FBM_GetPoolInfo((UINT8)FBM_POOL_TYPE_TDC);
	#endif
    if (prPool != NULL)
    {
        if (!(((u4Arg1 == SV_VP_MAIN)&&fgIsMainTvd3d())||((u4Arg1 == SV_VP_PIP)&&fgIsPipTvd3d())))
        {
            return;
        }
        if (u4Arg2 & FBM_POOL_MODE_10BIT)
        {
            if (prPool->u4Mode & FBM_POOL_MODE_10BIT)
            {
                vDrvTDCSetDramMode(SV_ON);
            }
            else
            {
                vDrvTDCSetDramMode(SV_OFF);
            }
            TDC_DRAM_BASE = prPool->u4Addr;
            vDrvTDCSetDramBase();


		    if(fgDrvTDCEnableChk())
		    {
		    	vDrvTDCOnOff(SV_ON);
	            vDrvTDCActive(SV_ON);
			}

        }
        else
        {
            vDrvTDCOnOff(SV_OFF);
        }
    }
    else
    {
        LOG(1, "vDrvDISrmCb: Fail to get pool\n");
    }
}


void vTdcColorPatch(void)
{
	UINT8 bPatch_En = IO32ReadFldAlign(TDC_FW_00, TDC_COLOR_PATCH_EN);
	
	if(bPatch_En)
	{
		if(IO32ReadFldAlign(STA_COMB_06, PERFIELDCHROMAVARSUM)>130)
		{
			vIO32WriteFldAlign(COMB3D_1F, SV_OFF, CB_3D_CUBE_FILTER_Y_EN);
			vIO32WriteFldAlign(COMB3D_1F, SV_OFF, CB_3D_CUBE_FILTER_C_EN);
		}
		else
		{
			vIO32WriteFldAlign(COMB3D_1F, SV_ON, CB_3D_CUBE_FILTER_Y_EN);
			vIO32WriteFldAlign(COMB3D_1F, SV_ON, CB_3D_CUBE_FILTER_C_EN);
		}
	}
}

void vTdcUS14Patch(void)
{
	UINT8 bPatch_En = IO32ReadFldAlign(TDC_FW_00, TDC_US14_EN);
	
	if(bPatch_En)
	{
		if(IO32ReadFldAlign(STA_COMB_04, INPIXCNTSTA)>0x6000 && 
		   IO32ReadFldAlign(STA_COMB_05, MOPIXCNTSTA)<0xC000)
		{
			vIO32WriteFldAlign(COMB3D_0F, 2, REG_D3GAINCV);
			vIO32WriteFldAlign(COMB3D_0F, 1, REG_D3GAINY);
		}
		else
		{
			vIO32WriteFldAlign(COMB3D_0F, 4, REG_D3GAINCV);
			vIO32WriteFldAlign(COMB3D_0F, 4, REG_D3GAINY);
		}
	}
}

void vTdcCrossColorProc(void)
{
	UINT8 bSAT_SUM = IO32ReadFldAlign(COMB2D_1D, FRM_SAT_SUM);
	UINT8 bCCS_ADAP = IO32ReadFldAlign(TDC_FW_00, TDC_CCS_ADAP_EN);
	UINT8 bPerMotionPixCnt = IO32ReadFldAlign(STA_COMB_05, PERMOTIONPIXCOUNT);
	
	if(bCCS_ADAP)
	{
		if((IO32ReadFldAlign(STA_CDET_00, MODE_TVD3D) == SV_CS_NTSC358))
		{
			//cband_mode
			vIO32WriteFldAlign(COMB2D_1C, ((bSAT_SUM<55 && bPerMotionPixCnt>5)?2:0), Y2D_CBAND_MODE);
			vIO32WriteFldAlign(COMB2D_1B, ((bSAT_SUM<55 && bPerMotionPixCnt>5)?0:1), C2D_CBAND_FORCE_CBWGT);
			vIO32WriteFldAlign(COMB2D_16, ((bSAT_SUM<190)?0:1), C2D_COLOR_FORCE_COLORWGTC);
			vIO32WriteFldAlign(COMB2D_16, ((bSAT_SUM<190)?0:1), C2D_COLOR_FORCE_GRAYWGTC);
			//Saturation limit type & Chroma band weight
			vIO32WriteFldAlign(COMB2D_19, ((bSAT_SUM<30)?2:1), C2D_COLOR_GWGT_SATLIMIT_TYPE);
			

			if(bSAT_SUM<59) // Aggressive
			{
				vIO32WriteFldAlign(COMB2D_15, 24, C2D_COLOR_SIMILAR_TH);
				vIO32WriteFldAlign(COMB2D_15, 20, C2D_COLOR_GSIMILAR_TH);
				vIO32WriteFldAlign(COMB2D_15, 5, C2D_COLOR_SAT_RANGE);
				vIO32WriteFldAlign(COMB2D_15, 20, C2D_COLOR_HUE_RANGE);
				vIO32WriteFldAlign(COMB2D_16, 20, C2D_COLOR_GRAY_RANGE);
				vIO32WriteFldAlign(COMB2D_16, 35, C2D_COLOR_UVVAR_TH);
				vIO32WriteFldAlign(COMB2D_18, 70, C2D_COLOR_GRNG_BSUM);
				vIO32WriteFldAlign(COMB2D_17, 5, C2D_COLOR_CBANDWGT_BSUMTH);
				vIO32WriteFldAlign(COMB2D_17, 60, C2D_COLOR_GBANDWGT_BSUMTH);	
			}
			else // Conservative
			{
				vIO32WriteFldAlign(COMB2D_15, 24, C2D_COLOR_SIMILAR_TH);
				vIO32WriteFldAlign(COMB2D_15, 27, C2D_COLOR_GSIMILAR_TH);
				vIO32WriteFldAlign(COMB2D_15, 5, C2D_COLOR_SAT_RANGE);
				vIO32WriteFldAlign(COMB2D_15, 15, C2D_COLOR_HUE_RANGE);
				vIO32WriteFldAlign(COMB2D_16, 20, C2D_COLOR_GRAY_RANGE);
				vIO32WriteFldAlign(COMB2D_16, 25, C2D_COLOR_UVVAR_TH);
				vIO32WriteFldAlign(COMB2D_18, 150, C2D_COLOR_GRNG_BSUM);
				vIO32WriteFldAlign(COMB2D_17, 50, C2D_COLOR_CBANDWGT_BSUMTH);
				vIO32WriteFldAlign(COMB2D_17, 150, C2D_COLOR_GBANDWGT_BSUMTH);
			}			
		}
		else if((IO32ReadFldAlign(STA_CDET_00, MODE_TVD3D) == SV_CS_PAL))
		{
			//cband_mode
			vIO32WriteFldAlign(COMB2D_1C, ((bSAT_SUM<40 && bPerMotionPixCnt>5)?2:
										   (bSAT_SUM<80 && bPerMotionPixCnt>5)?1:0), Y2D_CBAND_MODE);
			
			vIO32WriteFldAlign(COMB2D_1B, ((bSAT_SUM<88 && bPerMotionPixCnt>5)?0:1), C2D_CBAND_FORCE_CBWGT);
			vIO32WriteFldAlign(COMB2D_16, ((bSAT_SUM<220)?0:1), C2D_COLOR_FORCE_COLORWGTC);
			vIO32WriteFldAlign(COMB2D_16, ((bSAT_SUM<220)?0:1), C2D_COLOR_FORCE_GRAYWGTC);
			//Saturation limit type & Chroma band weight
			vIO32WriteFldAlign(COMB2D_19, ((bSAT_SUM<48)?2:1), C2D_COLOR_GWGT_SATLIMIT_TYPE);
			

			if(bSAT_SUM<94) // Aggressive
			{
				vIO32WriteFldAlign(COMB2D_15, 24, C2D_COLOR_SIMILAR_TH);
				vIO32WriteFldAlign(COMB2D_15, 20, C2D_COLOR_GSIMILAR_TH);
				vIO32WriteFldAlign(COMB2D_15, 5, C2D_COLOR_SAT_RANGE);
				vIO32WriteFldAlign(COMB2D_15, 20, C2D_COLOR_HUE_RANGE);
				vIO32WriteFldAlign(COMB2D_16, 30, C2D_COLOR_GRAY_RANGE);
				vIO32WriteFldAlign(COMB2D_16, 35, C2D_COLOR_UVVAR_TH);
				vIO32WriteFldAlign(COMB2D_18, 70, C2D_COLOR_GRNG_BSUM);
				vIO32WriteFldAlign(COMB2D_17, 10, C2D_COLOR_CBANDWGT_BSUMTH);
				vIO32WriteFldAlign(COMB2D_17, 60, C2D_COLOR_GBANDWGT_BSUMTH);	
			}
			else // Conservative
			{
				vIO32WriteFldAlign(COMB2D_15, 24, C2D_COLOR_SIMILAR_TH);
				vIO32WriteFldAlign(COMB2D_15, 27, C2D_COLOR_GSIMILAR_TH);
				vIO32WriteFldAlign(COMB2D_15, 5, C2D_COLOR_SAT_RANGE);
				vIO32WriteFldAlign(COMB2D_15, 15, C2D_COLOR_HUE_RANGE);
				vIO32WriteFldAlign(COMB2D_16, 30, C2D_COLOR_GRAY_RANGE);
				vIO32WriteFldAlign(COMB2D_16, 25, C2D_COLOR_UVVAR_TH);
				vIO32WriteFldAlign(COMB2D_18, 150, C2D_COLOR_GRNG_BSUM);
				vIO32WriteFldAlign(COMB2D_17, 50, C2D_COLOR_CBANDWGT_BSUMTH);
				vIO32WriteFldAlign(COMB2D_17, 150, C2D_COLOR_GBANDWGT_BSUMTH);
			}
		}
	}
}

