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
 * $Author: p4admin $
 * $Date: 2015/03/03 $
 * $RCSfile: drv_tdc.c,v $
 * $Revision: #8 $
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

#include "hw_di_int.h"
#include "hw_di.h"
#include "hw_ycproc.h"
#ifndef  CC_UP8032_ATV
#include "fbm_drvif.h"
EXTERN UINT32 TDC_DRAM_BASE;
#else
extern UINT32 TDC_DRAM_BASE;
#endif

#define SUPPORT_PQ_FINETUNE 1
#if SUPPORT_PQ_FINETUNE 
UINT8 _bK2ValueByCSS;
UINT8 _bK2ValueByWeakC;
UINT8 _bK4ChromaThByCans;
UINT8 _bPatchInPattern107;
UINT8 u1fgPatternHammock;
UINT8 _fgMovingcolorCharacter;
UINT8 _fgCornPattern;
UINT8 _fgMovingCans;
EXTERN UINT16 u2MajorCnt0;
EXTERN UINT8  u1MajorMvX0;
EXTERN UINT8  u1MajorMvY0;
EXTERN UINT16 u2TotalCnt;
EXTERN UINT8 u1IF_MAX_MOTION_C_Value;
static void vTdc3DStable(void);
static void vTdcRNRSetting(void);
static void vTdcCrossColorReduce2(void);
void vTdcDotCrawlReduce(void);
void vTdcAdaptiveFW2DSoftswitch(void);
void CheckSPCAndWeakC(void);
void vTdcCCSPatch(void);
void vTdcMainMenuTransition(void);
void vTdcSharpness(void);
#endif
UINT8 _bTDCEnable;

REGTBL_T const CODE REGTBL_COMB_NTSC_AV[] = {
	{COMB_CTRL_00, 0x000B101F, 0x1FFFFFFF},
	{COMB_CTRL_01, 0x00000000, 0xFE000000},
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
	{COMB2D_00, 0x304859C8, 0xFFFFFFFE},   //bypass y2d_cshap_en
	{COMB2D_01, 0x809E2001, 0xFFFFFFFF},
	{COMB2D_02, 0x08051020, 0x0FFFFFFF},
	{COMB2D_03, 0x08081E03, 0xFFFFFF7F},
	{COMB2D_04, 0x00100008, 0xFFFF03FF},
	{COMB2D_05, 0x14061210, 0xFFFFFFFF},
	{COMB2D_06, 0x10101022, 0xFFFFFFFF},
	{COMB2D_07, 0x20502214, 0x7FFFFFFF},
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
	{COMB2D_08, 0x33110029, 0xFFFFFFFF},
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
        vIO32WriteFldAlign(TDC_FW_01, 0x4000, COLOR_EDGE_TH);
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

    vIO32WriteFldAlign(TDC_FW_00, SV_ON, TDC_CCS_ADAP_EN);
	if((_rTvd3dStatus.eSourceType==SV_ST_TV)&&(bHwTvdMode()==SV_CS_NTSC358))  // for ATV FSC change color bar issue.
	{		
		vIO32WriteFldAlign(COMB2D_00, SV_OFF, ENFWEAKC);
		
		vIO32WriteFldAlign(COMB2D_07, SV_ON, REG_ENBOUND);
		vIO32WriteFldAlign(COMB2D_07, SV_ON, REG_ENUNIFORM_SPCLR);
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
#if SUPPORT_PQ_FINETUNE
	if((_rTvd3dStatus.eSourceType==SV_ST_AV))
	{
		vTdc3DStable();
		vTdcRNRSetting();
		vTdcCrossColorReduce2();
		vTdcDotCrawlReduce();
		vTdcAdaptiveFW2DSoftswitch();
		CheckSPCAndWeakC();
		vTdcCCSPatch();
		vTdcMainMenuTransition();
	}	
#endif

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

#if SUPPORT_PQ_FINETUNE
static void vTdc3DStable(void)
{
    UINT32 u4MotionPixelCnt = RegReadFldAlign(STA_COMB_05, MOPIXCNTSTA);        
    UINT32 dwTdc3dLumasum = IO32ReadFldAlign(STA_COMB_07, LUMASUMSTA);
    UINT32 dwTdc3dMBPixCnt = IO32ReadFldAlign(STA_COMB_06, MBPIXCNTSTA);
    UINT32 dwTdc3dColorEdgeSum = IO32ReadFldAlign(STA_COMB_0B, COLOREDGESTA);
    UINT32 dwTdc3dColorSum = IO32ReadFldAlign(STA_COMB_08, COLORSUMSTA);
    //UINT32 dwTdc3dLumaEdgeSum = IO32ReadFldAlign(STA_COMB_09, LUMAEDGESTA);

    if (bHwTvdMode()==SV_CS_PAL)
    {
        if (((dwTdc3dMBPixCnt>0x600)&&(dwTdc3dMBPixCnt<0x800))
            &&((dwTdc3dLumasum>0xD800000)&&(dwTdc3dLumasum<0xE800000))
            &&((dwTdc3dColorSum>0x1200000)&&(dwTdc3dColorSum<0x1400000))
            &&((dwTdc3dColorEdgeSum>0x180000)&&(dwTdc3dColorEdgeSum<0x280000))
        )//7
        {
            vRegWriteFldAlign(COMB3D_0B, MIN(0xB0, RegReadFldAlign(COMB3D_0B, REG_D3BIGTH)+0x4), REG_D3BIGTH);
        }
        else
        {
            vRegWriteFldAlign(COMB3D_0B, MAX(0x8A, RegReadFldAlign(COMB3D_0B, REG_D3BIGTH)-0x4), REG_D3BIGTH);
        }

    }
    if (bHwTvdMode()==SV_CS_NTSC358)//some still pattern have flicker
    {
        if (u4MotionPixelCnt <= 0x120)
        {
            vRegWriteFldAlign(COMB3D_03, MIN(20, 0x120 - u4MotionPixelCnt)+0xAA, REG_CVBS_DIFF_TH); // need to check the register*****
        }
        else
        {
            vRegWriteFldAlign(COMB3D_03, 0xAA, REG_CVBS_DIFF_TH);
        }
    }
}

static void vTdcRNRSetting(void)
{
    //UINT32 u4MotionPixelCnt = RegReadFldAlign(STA_COMB_05, MOPIXCNTSTA);        
    UINT32 dwTdc3dLumasum = IO32ReadFldAlign(STA_COMB_07, LUMASUMSTA);
    UINT32 dwTdc3dMBPixCnt = IO32ReadFldAlign(STA_COMB_06, MBPIXCNTSTA);
    UINT32 dwTdc3dColorEdgeSum = IO32ReadFldAlign(STA_COMB_0B, COLOREDGESTA);
    UINT32 dwTdc3dColorSum = IO32ReadFldAlign(STA_COMB_08, COLORSUMSTA);
    UINT32 dwTdc3dLumaEdgeSum = IO32ReadFldAlign(STA_COMB_09, LUMAEDGESTA);

    if (bHwTvdMode()==SV_CS_PAL)
    {
        if (((dwTdc3dMBPixCnt>0x350)&&(dwTdc3dMBPixCnt<0x480))
            &&((dwTdc3dLumasum>0xD000000)&&(dwTdc3dLumasum<0xD900000))
            &&((dwTdc3dColorSum>0x1C00000)&&(dwTdc3dColorSum<0x1E00000))
            &&((dwTdc3dLumaEdgeSum>0x100000)&&(dwTdc3dLumaEdgeSum<0x128000))
            &&((dwTdc3dColorEdgeSum>0x700000)&&(dwTdc3dColorEdgeSum<0x8C0000))
        )//46
        {
            //vIO32WriteFldAlign(NR_CTRL_00, 1, EN_RNR_TH_ADAP);
        }
        else
        {
            //vIO32WriteFldAlign(NR_CTRL_00, 0, EN_RNR_TH_ADAP);
        }
    }

    
}

static void vTdcCrossColorReduce2(void)
{
    UINT32 u4MotionPixelCnt = RegReadFldAlign(STA_COMB_05, MOPIXCNTSTA);        
    UINT32 dwTdc3dLumasum = IO32ReadFldAlign(STA_COMB_07, LUMASUMSTA);
    UINT32 dwTdc3dMBPixCnt = IO32ReadFldAlign(STA_COMB_06, MBPIXCNTSTA);
    UINT32 dwTdc3dColorEdgeSum = IO32ReadFldAlign(STA_COMB_0B, COLOREDGESTA);
    UINT32 dwTdc3dLumaEdgeSum = IO32ReadFldAlign(STA_COMB_09, LUMAEDGESTA);
    UINT32 dwTdc3dColorSum = IO32ReadFldAlign(STA_COMB_08, COLORSUMSTA);
	UINT16 u2DIHistCnt = IO32ReadFldAlign(MCVP_CS_27,HIST_CNT);
	UINT16 u2DIHistCntMax = IO32ReadFldAlign(MCVP_CS_27,STA_MAX_CNT1);
	UINT16 u2DIHistCntDiff;
   /// UINT16 u2MjcMvZeroCnt = IO32ReadFldAlign(MJC_STA_MH_1A, MJC_STA_MV_ZERO);  need find another status.
    static UINT8 bPre2DGain = 0x4;
    UINT8 bTarget2DGain;
    UINT8 b3DGainC;
    UINT8 u1IsHammock = 0;
	
	u2DIHistCntDiff = (u2DIHistCnt >= u2DIHistCntMax) ? (u2DIHistCnt - u2DIHistCntMax) :(u2DIHistCntMax - u2DIHistCnt);

	if(IO32ReadFldAlign(TDC_FW_00, TDC_CCRD2_DEBUG))
	{
		Printf("\n***vTdcCrossColorR u4MotionPixelCnt=%x,dwTdc3dLumasum=%x,dwTdc3dMBPixCnt=%x ,dwTdc3dColorEdgeSum=%x\n",
			u4MotionPixelCnt,dwTdc3dLumasum,dwTdc3dMBPixCnt,dwTdc3dColorEdgeSum);
		Printf("dwTdc3dColorSum=%x, u2DIHistCnt=%x, u2DIHistCntMax=%x,u2DIHistCntDiff=%x\n",
			dwTdc3dColorSum,u2DIHistCnt,u2DIHistCnt,u2DIHistCntDiff);
	}
	
	if (bHwTvdMode()==SV_CS_PAL)
    {
        b3DGainC = 4;
        if((u4MotionPixelCnt<=0xE00)
           //  &&(u2MjcMvZeroCnt > 0x6500)  need to check.
           &&(u2DIHistCnt>=1650)&&(u2DIHistCnt<=2250)&&(u2DIHistCntDiff>=80)&&(u2DIHistCntDiff<=500)
            &&((dwTdc3dLumasum>=0xF400000)&&(dwTdc3dLumasum<=0x10200000))
            //&&((dwTdc3dMBPixCnt>0x1200)&&(dwTdc3dMBPixCnt<0x1300))
            &&((dwTdc3dColorEdgeSum>1700000)&&(dwTdc3dColorEdgeSum<5800000))
            &&((dwTdc3dColorSum>5000000)&&(dwTdc3dColorSum<9200000)))//for hammock
        {
            //vRegWriteFldAlign (COMB_3D_0E, MIN(0x10, (0x2000-u4MotionPixelCnt)>>7), CB_3D_FW_2D_COST);
            //vRegWriteFldAlign(MCVP_FUSION_21, 0, IF_MAX_MOTION_C);
            u1fgPatternHammock = 1;
            u1IF_MAX_MOTION_C_Value = 0;
            vRegWriteFldAlign(COMB2D_07, 0x20, VWY2CHROMATH);
            vRegWriteFldAlign(COMB3D_0A, 0x8A, REG_D3BIGTH1);
            
        }
        else if (_bK4ChromaThByCans == 4)
        {
            u1fgPatternHammock = 0;
            u1IF_MAX_MOTION_C_Value = 0;
            vRegWriteFldAlign(COMB2D_07, 0x12, VWY2CHROMATH);
            vRegWriteFldAlign(COMB3D_0A, 0x90, REG_D3BIGTH1);
        }
        else if (_fgMovingcolorCharacter == 1)
        {
            u1fgPatternHammock = 0;
            u1IF_MAX_MOTION_C_Value = 0;
            vRegWriteFldAlign(COMB2D_07, 0xD, VWY2CHROMATH);
            vRegWriteFldAlign(COMB3D_0A, 0xA0, REG_D3BIGTH1);
        }
        else if (_bPatchInPattern107 == 1)
        {
            u1fgPatternHammock = 0;
            b3DGainC = 1;
            u1IF_MAX_MOTION_C_Value = 8;
            vRegWriteFldAlign(COMB2D_07, 0xD, VWY2CHROMATH);
            vRegWriteFldAlign(COMB3D_0A, 0x71, REG_D3BIGTH1);
            
        }
        else
        {
            //vRegWriteFldAlign (COMB_3D_0E, 0, CB_3D_FW_2D_COST);
            //vRegWriteFldAlign(MCVP_FUSION_21, 8, IF_MAX_MOTION_C);
            u1fgPatternHammock = 0;
            if(IO32ReadFldAlign(PSCAN_FWCS_02, FAVOR_CS_STATE)>=4)
            {
                u1IF_MAX_MOTION_C_Value = 8 - IO32ReadFldAlign(PSCAN_FWCS_02, FAVOR_CS_STATE);
            }
            else
            {
                u1IF_MAX_MOTION_C_Value = 8;
            }
            
            vRegWriteFldAlign(COMB2D_07, 0xD, VWY2CHROMATH);
            vRegWriteFldAlign(COMB3D_0A, 0x71, REG_D3BIGTH1);
            
        }
        
        if ((dwTdc3dColorEdgeSum>26000000)
            &&(dwTdc3dColorSum>48000000)
            )
        {
            vRegWriteFldAlign(COMB3D_06, 0, REG_UNIFORMTH);
        }
        else
        {
            vRegWriteFldAlign(COMB3D_06, 0x10, REG_UNIFORMTH);
        }
        vRegWriteFldAlign (COMB3D_0F, b3DGainC, REG_D3GAINC);
    }
    else if (_rTvd3dStatus.bTvdMode==SV_CS_NTSC358)
    {
        if ((u4MotionPixelCnt<=0x4F00)//&&(u2MjcMvZeroCnt > 0x6500)   ++++++++  need to find another register. +++++
			&&(u2DIHistCnt>=1650)&&(u2DIHistCnt<=2250)&&(u2DIHistCntDiff>=80)&&(u2DIHistCntDiff<=500)
            //&&((dwTdc3dLumasum>=0xA800000)&&(dwTdc3dLumasum<=0xB000000))
            &&((dwTdc3dLumasum>=0xA000000)&&(dwTdc3dLumasum<=0xB700000))
            &&((dwTdc3dMBPixCnt>0x300)&&(dwTdc3dMBPixCnt<0x900))
            //&&((dwTdc3dColorEdgeSum>0x1A0000)&&(dwTdc3dColorEdgeSum<0x310000))
            &&((dwTdc3dColorEdgeSum>0x100000)&&(dwTdc3dColorEdgeSum<0x250000))
            //&&((dwTdc3dColorSum>0x500000)&&(dwTdc3dColorSum<0x700000))
            &&((dwTdc3dColorSum>0x400000)&&(dwTdc3dColorSum<0x580000)))//for hammock
        {
            //vRegWriteFldAlign (C_COMB_1B, 3, REG_D3GAINY);
            //vRegWriteFldAlign (C_COMB_1B, 4-MIN(((0x4800 - u4MotionPixelCnt)>>9),4), REG_D3GAINCV);
            u1IsHammock = 1;
            vRegWriteFldAlign (COMB3D_0C, 8, CHROMA3D_OFFSET);
            vRegWriteFldAlign (COMB3D_0C, 0xA, CHROMA3D_SLOP);
            
            bTarget2DGain = MIN(((0x4800 - u4MotionPixelCnt)>>8)+4, 0xF);

            vRegWriteFldAlign (COMB3D_0B, 0x84 - MIN((0x4800 - u4MotionPixelCnt)>>4, 0x84), REG_D3BIGTH);
            vRegWriteFldAlign (COMB3D_0A, 0x78 - MIN((0x4800 - u4MotionPixelCnt)>>4, 0x78), REG_D3BIGTH1);

            vRegWriteFldAlign (COMB3D_02, 1, REG_ENCVMO);
            vRegWriteFldAlign (COMB3D_0A, 1, REG_ENLOCALSTILLG_B);

            vRegWriteFldAlign (COMB3D_0F, 4, REG_D3GAINCV);
            vRegWriteFldAlign (COMB3D_0F, 8, REG_D3GAINC);
            vRegWriteFldAlign (COMB3D_0F, 4, REG_D3GAINY);
            vRegWriteFldAlign (COMB3D_0F, 4, REG_D2GAINCV);
            vRegWriteFldAlign (COMB3D_0F, 4, REG_D2GAINC);
            vRegWriteFldAlign (COMB3D_0F, 4, REG_D2GAINY);
            vRegWriteFldAlign (COMB3D_0F, 4, REG_D2GAINCV_MB);  //need to check register.
        }
       
        else 
        {
            if ((dwTdc3dColorSum>38000000)
                &&(dwTdc3dColorEdgeSum>20000000)
                &&(u4MotionPixelCnt<4000)// //resolution still color mulitburst pattern not still
                )

            {
                vRegWriteFldAlign (COMB3D_0F, 0, REG_D3GAINY);
                vRegWriteFldAlign (COMB3D_02, 0, REG_ENCVMO);
                vRegWriteFldAlign (COMB3D_0B, 0x84, REG_D3BIGTH);
                vRegWriteFldAlign (COMB3D_0A, 0x40, REG_D3BIGTH1);
                vRegWriteFldAlign (COMB3D_0A, 0, REG_ENLOCALSTILLG_B);
                bTarget2DGain = 0x9;
                
            }
            else if(((dwTdc3dLumasum>=0xA000000)&&(dwTdc3dLumasum<=0xAA00000))
            &&(dwTdc3dMBPixCnt<0x800)
            &&((dwTdc3dLumaEdgeSum>0x2C000)&&(dwTdc3dLumaEdgeSum<0x45000))
            &&((dwTdc3dColorEdgeSum>0x3A0000)&&(dwTdc3dColorEdgeSum<0x4A0000))
            &&((dwTdc3dColorSum>0x3500000)&&(dwTdc3dColorSum<0x4100000)))
                {
                vRegWriteFldAlign (COMB3D_0F, 4, REG_D3GAINY);
                vRegWriteFldAlign (COMB3D_02, 1, REG_ENCVMO);
                vRegWriteFldAlign (COMB3D_0B, 0x84, REG_D3BIGTH);
                vRegWriteFldAlign (COMB3D_0A, 0xA0, REG_D3BIGTH1);
                vRegWriteFldAlign (COMB3D_0A, 1, REG_ENLOCALSTILLG_B);
                bTarget2DGain = 4;
            }
            else
            {
                vRegWriteFldAlign (COMB3D_0F, 4, REG_D3GAINY);
                vRegWriteFldAlign (COMB3D_02, 1, REG_ENCVMO);
                vRegWriteFldAlign (COMB3D_0B, 0x84, REG_D3BIGTH);
                vRegWriteFldAlign (COMB3D_0A, 0x78, REG_D3BIGTH1);
                vRegWriteFldAlign (COMB3D_0A, 1, REG_ENLOCALSTILLG_B);
                bTarget2DGain = 4;
            }
            
            //vRegWriteFldAlign (C_COMB_1B, 4, REG_D3GAINCV);

            vRegWriteFldAlign (COMB3D_0C, 0x1E, CHROMA3D_OFFSET);
            vRegWriteFldAlign (COMB3D_0C, 0xF, CHROMA3D_SLOP);
        }
        //.................................................................
        if (((IO32ReadFldAlign(PSCAN_FWCS_02, FAVOR_CS_STATE)>=4)
            &&(u4MotionPixelCnt<=0x6500)
            //&&((dwTdc3dLumasum>=0xB000000)&&(dwTdc3dLumasum<=0xC000000))
            &&((dwTdc3dLumasum>=0xA000000)&&(dwTdc3dLumasum<=0xC000000))
            //&&((dwTdc3dMBPixCnt>0xB00)&&(dwTdc3dMBPixCnt<0xC00))
            &&((dwTdc3dMBPixCnt>0xA00)&&(dwTdc3dMBPixCnt<0xD00))
            &&((dwTdc3dColorEdgeSum>0x150000)&&(dwTdc3dColorEdgeSum<0x400000))
            //&&((dwTdc3dColorSum>0xF80000)&&(dwTdc3dColorSum<0x1180000)))
            &&((dwTdc3dColorSum>0xE00000)&&(dwTdc3dColorSum<0x1100000)))
            ||(u1IsHammock == 1) || _fgMovingCans)//pattern 107 need more 3d
        {
            vRegWriteFldAlign (COMB3D_0F, 2, REG_D3GAINCV);
			vRegWriteFldAlign (COMB3D_0F, 8, REG_D3GAINC);
        }
        else
        {
            vRegWriteFldAlign (COMB3D_0F, 4, REG_D3GAINCV);
			vRegWriteFldAlign (COMB3D_0F, 1, REG_D3GAINC);
        }

        if (bPre2DGain < bTarget2DGain)
        {
            bPre2DGain++;
        }
        else if (bPre2DGain > bTarget2DGain)
        {
            bPre2DGain--;
        }

        if(u1IsHammock == 0)
        {
            vRegWriteFldAlign (COMB3D_0F, bPre2DGain, REG_D2GAINY);
            vRegWriteFldAlign (COMB3D_0F, bPre2DGain, REG_D2GAINC);
            vRegWriteFldAlign (COMB3D_0F, bPre2DGain, REG_D2GAINCV);
            vRegWriteFldAlign (COMB3D_0F, bPre2DGain, REG_D2GAINCV_MB);
        }
    }
}

// for moving can dot carwl redction
void vTdcDotCrawlReduce(void)
{
    UINT8 u1DCR_En = IO32ReadFldAlign(TDC_FW_00, TDC_FW_DOTCRAWL_RD);
    static UINT8 _u1PreDCR_En;
    static UINT8 K2THValue = 7;
    UINT8 bcs_state = IO32ReadFldAlign(PSCAN_FWCS_02, FAVOR_CS_STATE);
    UINT32 dwTdc3dColorEdgeSum = IO32ReadFldAlign(STA_COMB_0B, COLOREDGESTA);
    UINT32 dwTdc3dLumasum = IO32ReadFldAlign(STA_COMB_07, LUMASUMSTA);
    UINT32 dwTdc3dColorSum = IO32ReadFldAlign(STA_COMB_08, COLORSUMSTA);
    UINT32 dwTdc3dMBPixCnt = IO32ReadFldAlign(STA_COMB_06, MBPIXCNTSTA);
    UINT32 u4MotionPixelCnt = RegReadFldAlign(STA_COMB_05, MOPIXCNTSTA); 
    UINT32 dwTdc3dLumaEdgeSum = IO32ReadFldAlign(STA_COMB_09, LUMAEDGESTA);    
    UINT8 bEdgeSum = MIN(0xFF, dwTdc3dLumaEdgeSum>>14);
    UINT8 bK4_V_Gain;
    UINT8 bK1Gain = 0x8;
    UINT8 bK4_HDGAIN_HFY2 = 0x2;
    UINT8 bK2_TBTH_CVAR = 0x0;
		
	if(IO32ReadFldAlign(TDC_FW_00, TDC_CCRD_DEBUG))
	{
		Printf("\n vTdcDotCrawlReduce bcs_state=%x dwTdc3dColorEdgeSum=%xdwTdc3dColorSum=%x\n",bcs_state,dwTdc3dColorEdgeSum,dwTdc3dColorSum);
		Printf("dwTdc3dMBPixCnt=%x u4MotionPixelCnt=%x dwTdc3dLumaEdgeSum=%x bEdgeSum=%x\n",
			dwTdc3dMBPixCnt,u4MotionPixelCnt,dwTdc3dLumaEdgeSum,bEdgeSum);
	}
    if ((bHwTvdMode()==SV_CS_NTSC358))
    {
        _fgCornPattern = 0;
		_fgMovingCans = 0;
        if ((bcs_state>=4)
            && ((dwTdc3dColorEdgeSum >= 2200000)&&(dwTdc3dColorEdgeSum <= 4000000))
            && ((dwTdc3dColorSum >= 11000000)&&(dwTdc3dColorSum <= 18500000))
            && ((dwTdc3dMBPixCnt >= 500)&&(dwTdc3dMBPixCnt <= 3400))//cans pattern
            )
        {
            if (u4MotionPixelCnt>50000)//cans pattern
            {
                vRegWriteFldAlign(COMB2D_07, MAX(0x28, RegReadFldAlign(COMB2D_07, VWY2CHROMATH)-0x10), VWY2CHROMATH);
                vRegWriteFldAlign(COMB2D_09, 0x4, HDGAIN_HFY2);
                vRegWriteFldAlign(COMB2D_05, 0x1, TBTH_CVAR);

                vRegWriteFldAlign (COMB3D_0B, 0x8, REG_D2D3SMALLTH);
                vRegWriteFldAlign(COMB2D_06, 0x10, PVCVBSVERTH);				
                
                _fgMovingCans = 1;
            }
            else //107
            {
                vRegWriteFldAlign(COMB2D_07, MAX(0x28, RegReadFldAlign(COMB2D_07, VWY2CHROMATH)-0x10), VWY2CHROMATH);
                vRegWriteFldAlign(COMB2D_09, MAX(0x2, RegReadFldAlign(COMB2D_09, HDGAIN_HFY2)-0x1), HDGAIN_HFY2);
                vRegWriteFldAlign(COMB2D_05, 3, TBTH_CVAR); 
                
                vRegWriteFldAlign (COMB3D_0B, 0x8, REG_D2D3SMALLTH);
                vRegWriteFldAlign(COMB2D_06, 0x10, PVCVBSVERTH);
                
            }
            
        }
        else if (((dwTdc3dColorEdgeSum >= 2000000)&&(dwTdc3dColorEdgeSum <= 12000000))
            && ((dwTdc3dColorSum >=12000000)&&(dwTdc3dColorSum <= 34000000))
            && ((dwTdc3dMBPixCnt >= 100)&&(dwTdc3dMBPixCnt <= 2400))// Corn(sorghum) and sunflower
            && (u4MotionPixelCnt>4500)
            )
        {
            vRegWriteFldAlign(COMB2D_05, 0x05, TBTH_CVAR);
            vRegWriteFldAlign(COMB2D_07, MIN(0x88, RegReadFldAlign(COMB2D_07, VWY2CHROMATH)+0x10), VWY2CHROMATH);
            vRegWriteFldAlign(COMB2D_09, MIN(0x6, RegReadFldAlign(COMB2D_09, HDGAIN_HFY2)+0x1), HDGAIN_HFY2);

            vRegWriteFldAlign (COMB3D_0B, 0x8, REG_D2D3SMALLTH);
            vRegWriteFldAlign(COMB2D_06, 0x10, PVCVBSVERTH);
            
            _fgCornPattern = 1;
        }
        else if(((dwTdc3dLumasum>=0xA000000)&&(dwTdc3dLumasum<=0xAA00000))
        &&(dwTdc3dMBPixCnt<0x800)
        &&((dwTdc3dLumaEdgeSum>0x2C000)&&(dwTdc3dLumaEdgeSum<0x45000))
        &&((dwTdc3dColorEdgeSum>0x3A0000)&&(dwTdc3dColorEdgeSum<0x4A0000))
        &&((dwTdc3dColorSum>0x3500000)&&(dwTdc3dColorSum<0x4100000))
        )
            {
            vRegWriteFldAlign(COMB2D_05, 0x2, TBTH_CVAR);
            vRegWriteFldAlign(COMB2D_07, MAX(0x50, RegReadFldAlign(COMB2D_07, VWY2CHROMATH)-0x10), VWY2CHROMATH);
            vRegWriteFldAlign(COMB2D_09, MIN(0x6, RegReadFldAlign(COMB2D_09, HDGAIN_HFY2)+0x1), HDGAIN_HFY2);
            
            vRegWriteFldAlign (COMB3D_0B, 0x4, REG_D2D3SMALLTH);
            vRegWriteFldAlign(COMB2D_06, 0x10, PVCVBSVERTH);
            
        }
        else if(IO32ReadFldAlign(STA_COMB_0C, NONVCR3D) == 0)
            {
            vRegWriteFldAlign(COMB2D_05, 2, TBTH_CVAR);
            vRegWriteFldAlign(COMB2D_07, MAX(0x50, RegReadFldAlign(COMB2D_07, VWY2CHROMATH)-0x10), VWY2CHROMATH);
            vRegWriteFldAlign(COMB2D_09, MIN(0x6, RegReadFldAlign(COMB2D_09, HDGAIN_HFY2)+0x1), HDGAIN_HFY2);
            
            vRegWriteFldAlign (COMB3D_0B, 0x8, REG_D2D3SMALLTH);
            vRegWriteFldAlign(COMB2D_06, 0x17, PVCVBSVERTH);
        }
        else
        {
            vRegWriteFldAlign(COMB2D_05, 4, TBTH_CVAR);
            vRegWriteFldAlign(COMB2D_07, MAX(0x50, RegReadFldAlign(COMB2D_07, VWY2CHROMATH)-0x10), VWY2CHROMATH);
            vRegWriteFldAlign(COMB2D_09, MIN(0x6, RegReadFldAlign(COMB2D_09, HDGAIN_HFY2)+0x1), HDGAIN_HFY2);
           // vRegWriteFldAlign(COMB2D_00, 0x0, ENSHARP);// check register address. not found?
           //vIO32Write4B(COMB2D_08, 0x1444000A);
            vRegWriteFldAlign (COMB3D_0B, 0x8, REG_D2D3SMALLTH);
            vRegWriteFldAlign(COMB2D_06, 0x10, PVCVBSVERTH);
        }
    }
    else if ((bHwTvdMode()==SV_CS_PAL))//Cans pattern check
    {
    
        UINT32 u4SCEHueHist0 = IO32ReadFldAlign(HUE_HIST_1_0_MAIN, HUE_HIST_0);
        UINT32 u4SCEHueHist2 = IO32ReadFldAlign(HUE_HIST_3_2_MAIN, HUE_HIST_2);
        UINT32 u4SCEHueHist5 = IO32ReadFldAlign(HUE_HIST_5_4_MAIN, HUE_HIST_5);
        _bPatchInPattern107 = 0;
        _bK4ChromaThByCans =0;
        _fgMovingcolorCharacter =0;
        bK4_V_Gain = 3;
        if (IO32ReadFldAlign(PSCAN_FWCS_02, FAVOR_CS_STATE) > 4)
        {
            bEdgeSum = bEdgeSum - MIN(bEdgeSum, (RegReadFldAlign(PSCAN_FWCS_02, FAVOR_CS_STATE)-4)<<3);
        }
        
        if (u1fgPatternHammock == 1)
        {
            bK4_V_Gain = 3;
            K2THValue = 6;
        }
        else if ((u1MajorMvX0>=0)&&(u1MajorMvY0 == 0)
            &&((dwTdc3dColorEdgeSum >= 0x800000)&&(dwTdc3dColorEdgeSum <= 0x10F0000))
            && ((dwTdc3dColorSum >= 0x3000000)&&(dwTdc3dColorSum <= 0x3600000))
            && ((dwTdc3dMBPixCnt >= 0xA00)&&(dwTdc3dMBPixCnt <= 0x2000))
            && ((u4MotionPixelCnt >= 0x2B0)&&(u4MotionPixelCnt <= 0x18000))
            && (u4SCEHueHist0<=0x50)
            && ((u4SCEHueHist2<=0x1600)&&(u4SCEHueHist2>=0x1300))
            && ((u4SCEHueHist5<=0x2500)&&(u4SCEHueHist5>=0x2000))
            )//leaf pattern
        {
            
            bK4_V_Gain = 3;
            K2THValue = 3;
            _fgMovingcolorCharacter = 1;
        }
        else if ((bcs_state>=4) && (u1MajorMvX0>=1)
            && ((dwTdc3dColorEdgeSum >= 2800000)&&(dwTdc3dColorEdgeSum <= 5950000))
            && ((dwTdc3dColorSum >= 15000000)&&(dwTdc3dColorSum <= 25500000))
            && ((dwTdc3dMBPixCnt >= 500)&&(dwTdc3dMBPixCnt <= 3500))//cans
            )
        {
            K2THValue = (K2THValue>3)?(K2THValue-1):3;
            bK4_V_Gain = 3;
            _bK4ChromaThByCans = 4;
        }
        else if((bEdgeSum>0x68) && 
            (dwTdc3dColorSum > 0x1000000) && (dwTdc3dColorSum < 0x3500000)
            && (u4MotionPixelCnt > 0x800))//corn and sunflower
        {
            K2THValue = (K2THValue<8)?(K2THValue+1):8;
            bK1Gain = 0xF;
            bK4_HDGAIN_HFY2 = 0x8;
            bK2_TBTH_CVAR = 0x5; 
            _fgCornPattern = 1;            
        }
        else if ((dwTdc3dColorSum>29000000)&&(dwTdc3dColorSum<40000000)
            &&(dwTdc3dLumaEdgeSum>800000)&&(dwTdc3dLumaEdgeSum<2000000)
            &&(dwTdc3dColorEdgeSum>8200000)&&(dwTdc3dColorEdgeSum<13000000)
            &&(dwTdc3dMBPixCnt>500)&&(dwTdc3dMBPixCnt<1200)//Thailand woman
            )
        {
            K2THValue = 2;
        }
        else if ((bcs_state>=4) &&(u4MotionPixelCnt<16000)
            && ((dwTdc3dColorSum >=17000000)&&(dwTdc3dColorSum <= 29000000))
            && ((dwTdc3dColorEdgeSum >= 2100000)&&(dwTdc3dColorEdgeSum <= 4100000))
            && ((dwTdc3dMBPixCnt >= 2700)&&(dwTdc3dMBPixCnt <= 4800))//107
            )
        {
            //K2THValue = (K2THValue>1)?(K2THValue-1):1;
            K2THValue = (K2THValue<3)?(K2THValue+1):3;
            _bPatchInPattern107 =1;
        }

        else
        {
            K2THValue = (K2THValue<3)?(K2THValue+1):3;
        }
        vRegWriteFldAlign(COMB2D_0A, MIN(0x8, MAX(1, _bK2ValueByCSS+K2THValue-_bK2ValueByWeakC)), TBTH_K2B);
        vRegWriteFldAlign(COMB2D_08, bK4_V_Gain, REG_HFY_VYD_GAIN);
        vRegWriteFldAlign(COMB2D_08, bK4_V_Gain, REG_HFY_VCD_GAIN);
        vRegWriteFldAlign(COMB2D_03, bK1Gain, MK1);
        vRegWriteFldAlign(COMB2D_09, bK4_HDGAIN_HFY2, HDGAIN_HFY2);
        vRegWriteFldAlign(COMB2D_05, bK2_TBTH_CVAR, TBTH_CVAR);
    }
    else if (u1DCR_En !=_u1PreDCR_En)
    {
        vRegWriteFldAlign(COMB2D_00, 0, ENMK2);
        vRegWriteFldAlign(COMB2D_03, 0, MK2);
    }
    _u1PreDCR_En = u1DCR_En;
}

void vTdcAdaptiveFW2DSoftswitch(void)  // check begin  here.
{
    //UINT32 u4MotionPixelCnt = RegReadFldAlign(STA_COMB_05, MOPIXCNTSTA);        
    UINT32 dwTdc3dColorEdgeSum = IO32ReadFldAlign(STA_COMB_0B, COLOREDGESTA);
    UINT32 dwTdc3dLumaEdgeSum = IO32ReadFldAlign(STA_COMB_09, LUMAEDGESTA);
    //UINT8 bK2THValue;
    UINT8 bK4HGain;
    UINT8 bEdgeSum;	

    if (bHwTvdMode()==SV_CS_PAL)
    {
        //bEdgeSum = MIN(0xFF, MIN(dwTdc3dColorEdgeSum>>16, dwTdc3dLumaEdgeSum>>14));
        bEdgeSum = MIN(0xFF, dwTdc3dLumaEdgeSum>>14);
        if (IO32ReadFldAlign(PSCAN_FWCS_02, FAVOR_CS_STATE) > 4)
        {
            bEdgeSum = bEdgeSum - MIN(bEdgeSum, (IO32ReadFldAlign(PSCAN_FWCS_02, FAVOR_CS_STATE)-4)<<3);
        }
        /*
        if ((RegReadFldAlign(COMB_3D_15, CB_3D_CHROMA_EDGE_SUM)<0x5)&&(RegReadFldAlign(COMB_3D_15, CB_3D_MOTION_PIXEL_CNT)>=0x3000))// for pattern 152 flicker
        {
            bUsefulBandSum = bUsefulBandSum + ((0x5 - RegReadFldAlign(COMB_3D_15, CB_3D_CHROMA_EDGE_SUM))<<4);
        }
            */

        if (u1fgPatternHammock == 1)//hammock
        {
            bK4HGain = 0x5;
        }
        else if(_fgMovingcolorCharacter == 1)
        {
            bK4HGain = 0x0;
        }
        else if (bEdgeSum>0x68)//for corn and sunflower
        {
            //bK2THValue = 0xA;
            bK4HGain = 0x8;
        }
        else if (_bK4ChromaThByCans == 4)
        {
            bK4HGain = 1;
        }
        else if (dwTdc3dColorEdgeSum < 0x500000)//shop
        {
            bK4HGain = 1;
        }
        else if (bEdgeSum<0x58)
        {
            //bK2THValue = 7;
            bK4HGain = 0x0;
        }
        else
        {
            //bK2THValue = 7;///2 + ((bEdgeSum - 0x60)>>2);
            bK4HGain = (bEdgeSum - 0x58)>>2;
        }

        bK4HGain = MIN(bK4HGain, 0x8);
        vRegWriteFldAlign(COMB2D_08, bK4HGain+1, REG_HFY_HDGAIN);
        vRegWriteFldAlign(COMB2D_08, bK4HGain, REG_HFY_HYD_GAIN);
        vRegWriteFldAlign(COMB2D_08, bK4HGain, REG_HFY_HCD_GAIN);
        
    }
    
}

void CheckSPCAndWeakC(void)// Test Pattern
{
    UINT32 dwTdc3dColorEdgeSum = IO32ReadFldAlign(STA_COMB_0B, COLOREDGESTA);
    UINT32 dwTdc3dColorSum = IO32ReadFldAlign(STA_COMB_08, COLORSUMSTA);
    UINT32 u4MotionPixelCnt = RegReadFldAlign(STA_COMB_05, MOPIXCNTSTA); 
    UINT32 dwTdc3dMBPixCnt = IO32ReadFldAlign(STA_COMB_06, MBPIXCNTSTA);
    UINT32 dwColorStatus = (dwTdc3dColorEdgeSum>>14) + (dwTdc3dColorSum>>16);
    UINT8 bSpcHorSmoothTH;
    UINT8 bWeakC_Tbth_K2;
    UINT8 bWeakC_Tbth_K1V;
    UINT8 bWeakC_Tbth_K1H;
    UINT8 b3D_D3Gain_CV;
    UINT8 bK3Value;
	
    if (bHwTvdMode()==SV_CS_NTSC358)
    {
        if (dwTdc3dColorSum>=0x2000000)
        {
            bSpcHorSmoothTH = 0x88;
            bWeakC_Tbth_K2 = 0x7F;

            //bypass weakC
            vRegWriteFldAlign(COMB2D_01, 1, DISCFIRINCBG);
            //K4
            //vRegWriteFldAlign(COMB2D_01, 0, REG_MHFYK);
            //vRegWriteFldAlign(COMB2D_01, 1, REG_ENMHFY);
        }
        else if (dwTdc3dColorSum<0x1C00000)
        {
            bSpcHorSmoothTH = 0x8;
            bWeakC_Tbth_K2 = 0x2;

            vRegWriteFldAlign(COMB2D_01, 0, DISCFIRINCBG);
            //vRegWriteFldAlign(COMB2D_01, 0, REG_ENMHFY);
        }
        else
        {
            bSpcHorSmoothTH = 0x8+ ((dwTdc3dColorSum-0x1C00000)>>15);
            bWeakC_Tbth_K2 = MIN(0x7F, 0x2+ ((dwTdc3dColorSum-0x1C00000)>>15));

            vRegWriteFldAlign(COMB2D_01, 0, DISCFIRINCBG);
            //vRegWriteFldAlign(COMB2D_01, 0, REG_ENMHFY);
        }
    
        vRegWriteFldAlign(COMB2D_02, bSpcHorSmoothTH, REG_VDGYHORSMOOTHTH);
        vRegWriteFldAlign(COMB2D_0E, bWeakC_Tbth_K2, TBTH_K2);
    
    }
    else if (bHwTvdMode()==SV_CS_PAL)
    {
        bSpcHorSmoothTH = 0x10;
        bWeakC_Tbth_K2 = 0x3;
        bWeakC_Tbth_K1V = 0xA;
        bWeakC_Tbth_K1H = 0x10;
        b3D_D3Gain_CV = 0x4;
        bK3Value = 0x8;
        
        _bK2ValueByWeakC =0;
        if ((u4MotionPixelCnt<10000)||(dwTdc3dMBPixCnt>200))
        {
            //do nothing
        }
        else if (dwColorStatus >= 0x980)
        {
            b3D_D3Gain_CV = 9;
            bK3Value = 0;
            _bK2ValueByWeakC = 2;
            
        }
        else if (dwColorStatus<=0x700)
        {   
            //do nothing
        }
        else if(_fgCornPattern == 1)
        {
            bK3Value = 0xF;
        }
        else
        {
            
            b3D_D3Gain_CV = MIN(9, b3D_D3Gain_CV + ((dwColorStatus-0x700)>>7));
            bK3Value = MIN(8, (980-dwColorStatus)>>6);
        }
        //......................................................................................
        if (dwTdc3dColorSum>=0x3000000)//pattern moving char bars in leaf disc
        {
            bSpcHorSmoothTH = 0x60;
            bWeakC_Tbth_K2 = 0x53;
            bWeakC_Tbth_K1V = 0;
            bWeakC_Tbth_K1H = 0;
            
        }
        else if (dwTdc3dColorSum <=0x2800000 )
        {
            
            //do nothing
        }
        else
        {
            bSpcHorSmoothTH = bSpcHorSmoothTH + ((dwTdc3dColorSum-0x2800000)>>17);
            bWeakC_Tbth_K2 = bWeakC_Tbth_K2 + ((dwTdc3dColorSum-0x2800000)>>17);
            bWeakC_Tbth_K1V = bWeakC_Tbth_K1V - MIN((dwTdc3dColorSum-0x2800000)>>19, bWeakC_Tbth_K1V);
            bWeakC_Tbth_K1H = bWeakC_Tbth_K1H - MIN((dwTdc3dColorSum-0x2800000)>>19, bWeakC_Tbth_K1H);
            
        }

        vRegWriteFldAlign(COMB2D_02, bSpcHorSmoothTH, REG_VDGYHORSMOOTHTH);
        vRegWriteFldAlign(COMB2D_0E, bWeakC_Tbth_K2, TBTH_K2);
        vRegWriteFldAlign(COMB2D_0E, bWeakC_Tbth_K1V, TBTH_K1V);
        vRegWriteFldAlign(COMB2D_0E, bWeakC_Tbth_K1H, TBTH_K1H);
        vRegWriteFldAlign(COMB2D_03, bK3Value, MK3);
        vRegWriteFldAlign(COMB3D_0F, b3D_D3Gain_CV, REG_D3GAINCV);
        
    }
    
}

void vTdcCCSPatch(void)  // not use 
{
    //UINT16 u2Sat = IO32ReadFldAlign(GLOBAL_ADJ_02, SAT_DUMMY);
    UINT32 u4CCState = IO32ReadFldAlign(METER_INFO_17, METER_CC_STATE);
    UINT32 u4MoPix = IO32ReadFldAlign(MCVP_STATUS_0F, OLD_FRAME_MO_Q_STATUS);
    UINT32 u4Rto = 16;
    static UINT8 _u1Cnt = 0;

    u4MoPix = ((MAX(u4MoPix, 6000) - 6000) >> 10);
    u4MoPix = MIN(u4MoPix, 8);
    u4CCState = u4CCState * u4MoPix;

    if (u4CCState >= 768)
    {
        _u1Cnt = (_u1Cnt < 32) ? _u1Cnt+1 : _u1Cnt;
    }
    else
    {
        UINT8 u1Dec = ((768 - u4CCState) >> 6);
        _u1Cnt = _u1Cnt - MIN(_u1Cnt, u1Dec);
    }

    u4Rto = MIN((MAX(_u1Cnt,12) - 12), 16);
    _bK2ValueByCSS = u4Rto>>2;
    u4Rto = 16 - u4Rto;
        
    //u2Sat = (u4Rto*u2Sat) >> 4;

    if (IO32ReadFldAlign(METER_CFG_00, METER_CC_ADAP_EN))
    {
        vIO32WriteFldAlign(METER_INFO_17, u4Rto, METER_CC_RATIO);
    }
    else
    {
        vIO32WriteFldAlign(METER_INFO_17, u4Rto, METER_CC_RATIO);
    }
}

void vTdcMainMenuTransition(void)
{
    UINT32 dwTdc3dColorEdgeSum = IO32ReadFldAlign(STA_COMB_0B, COLOREDGESTA);
    UINT32 dwThresholdColorEdge = IO32ReadFldAlign(TDC_FW_01, COLOR_EDGE_TH);
    static UINT8 bPreMbVTh = 0;
    UINT8 bMbVTh =0;
    dwTdc3dColorEdgeSum = dwTdc3dColorEdgeSum>>8;
    
    if (bHwTvdMode()==SV_CS_PAL)
    {
        if (dwTdc3dColorEdgeSum < dwThresholdColorEdge)
        {
            bMbVTh = MIN(0x20,(dwThresholdColorEdge-dwTdc3dColorEdgeSum)>>6);
            //vRegWriteFldAlign(COMB2D_00, MIN(0x20,(dwThresholdColorEdge-dwTdc3dColorEdgeSum)>>6),REG_HFY_MBVTH);
        }
        else 
        {
            bMbVTh = 0;
            //vRegWriteFldAlign(COMB2D_00, 0,REG_HFY_MBVTH);
        }

        if ((RegReadFldAlign(STA_COMB_05, MOPIXCNTSTA)< 0x180)&&(bPreMbVTh < bMbVTh))
        {
            bPreMbVTh = bPreMbVTh+1;
        }
        else if(bPreMbVTh>0)
        {
            bPreMbVTh = bPreMbVTh - 1;
        }
        vRegWriteFldAlign(COMB2D_08, bPreMbVTh, REG_HFY_MBVTH);
    }
}
void vTdcSharpness(void)
{
    if(_fgMovingCans == 1)
    {
       // vRegWriteFldAlign(COMB2D_07, 0x1, ENSHARP);        check register.
      //  vIO32Write4B(COMB2D_08, 0x44446666); 
    }
    else
    {
       // vRegWriteFldAlign(COMB2D_07, 0x0, ENSHARP);        
       // vIO32Write4B(COMB2D_08, 0x1234444); 
    }
}

#endif


