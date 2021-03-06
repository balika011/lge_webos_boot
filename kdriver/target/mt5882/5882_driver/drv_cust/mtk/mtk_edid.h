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

/*****************************************************************************
 *
 * Filename:
 * ---------
 *   $Workfile: mtk_edid.h
 *
 * Project:
 * --------
 *   MT5365
 *
 * Description:
 * ------------
 *   Edid customization
 *   
 * Author:
 * -------
 *   
 *
 * Unauthorized use, practice, perform, copy, distribution, reproduction,
 * or disclosure of this information in whole or in part is prohibited.
 *-----------------------------------------------------------------------------
 *
 * $Author: dtvbm11 $
 * $Date: 2015/01/09 $
 * $RCSfile: mtk_vga.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/
#ifndef _EDID_CUSTOMIZATION_H_
#define _EDID_CUSTOMIZATION_H_
#include "eeprom_if.h"
#ifndef CC_VGA_EDID_DISABLE
CODE UINT8 VGA_EDID_TABLE[256] =
{
	0x00, 0xFF, 0xFF, 0xFF,  0xFF, 0xFF, 0xFF, 0x00,   0x36, 0x8B, 0x10, 0x00,  0x01, 0x01, 0x01, 0x01, 
	0x07, 0x14, 0x01, 0x03,  0x1F, 0x5D, 0x34, 0x78,   0xCA, 0x31, 0xC3, 0xA3,  0x55, 0x4A, 0x9B, 0x24, 
	0x10, 0x47, 0x4A, 0xAF,  0xCF, 0x00, 0x01, 0x01,   0x01, 0x01, 0x01, 0x01,  0x01, 0x01, 0x01, 0x01, 
	0x01, 0x01, 0x01, 0x01,  0x01, 0x01, 0x1A, 0x36,   0x80, 0xA0, 0x70, 0x38,  0x1F, 0x40, 0x30, 0x20, 
	0x35, 0x00, 0xA2, 0x0B,  0x32, 0x00, 0x00, 0x1A,   0x00, 0x00, 0x00, 0xFD,  0x00, 0x32, 0x55, 0x1F, 
	0x46, 0x0F, 0x00, 0x0A,  0x20, 0x20, 0x20, 0x20,   0x20, 0x20, 0x00, 0x00,  0x00, 0xFF, 0x00, 0x53, 
	0x4B, 0x46, 0x4A, 0x42,  0x4C, 0x30, 0x37, 0x30,   0x32, 0x30, 0x32, 0x39,  0x00, 0x00, 0x00, 0xFC, 
	0x00, 0x53, 0x56, 0x34,  0x32, 0x32, 0x58, 0x56,   0x54, 0x0A, 0x20, 0x20,  0x20, 0x20, 0x00, 0x5D, 
	0x00, 0x00, 0x00, 0x00,  0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00,  0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00,  0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00,  0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00,  0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00,  0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00,  0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00,  0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00,  0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00,  0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00,  0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00,  0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00,  0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00,  0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00,  0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00,  0x00, 0x00, 0x00, 0x00
};
void vDrvLoadVgaEdid2Table(void)
{
#ifdef CC_FPGA
    UINT64 u8Offset;
    UINT8 idx = 0;
    //read form eeprom or flash to VGA_EDID_TABLE

#ifdef CC_MULTI_EDID
    UINT32 u4EdidOffset;
    if(DRVCUST_InitQuery(eEdidVgaIndexOffset, &u4EdidOffset) == 0)
    {
        EEPROM_Read((UINT64)u4EdidOffset, (UINT32)&idx, 1);
    }
#endif

    if((idx+1)*VGA_EDID_SIZE > VGA_EDID_STORAGE_SIZE)
    {
        u8Offset = 0;
    }
    else
    {
        u8Offset = idx*VGA_EDID_SIZE;
    }
    //Printf("read edid from eeprom\n");
#ifndef CC_DISABLE_VGA_EDID_FROM_EEP //wait for eeprom ready
    if(EEPVGAEDID_Read(u8Offset,  (UINT32) VGA_EDID_TABLE, VGA_EDID_SIZE ))
    {
        Printf("read eeprom edid fail\n");
    }
#endif

#endif

}
#endif

#ifndef CC_HDMI_EDID_DISABLE
/*
All value of Port 0 is in HDMIEDID_CEC_192k_DeepColor12B_P0
Arrangement of HDMICEC_SETTING is
Port2 CEC physical address
Port3 CEC physical address
Port4 CEC physical address
*/
UINT8 HDMICEC_SETTING[3]=
{
	0x20, // Port2 CEC physical address
	0x30, // Port2 CEC physical address
	0x40, // Port2 CEC physical address
};

UINT8 HDMIEDID_table[256] =
{
    0x00, 0xFF, 0xFF, 0xFF,  0xFF, 0xFF, 0xFF, 0x00, 0x36, 0x8B, 0x01, 0x00,  0x01, 0x01, 0x01, 0x01,
    0x01, 0x0F, 0x01, 0x03,  0x80, 0x3C, 0x22, 0x78, 0x0A, 0x0D, 0xC9, 0xA0,  0x57, 0x47, 0x98, 0x27,
    0x12, 0x48, 0x4C, 0xBF,  0xEF, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,  0x01, 0x01, 0x01, 0x01,
    0x01, 0x01, 0x01, 0x01,  0x01, 0x01, 0x01, 0x1D, 0x00, 0x72, 0x51, 0xD0,  0x1E, 0x20, 0x6E, 0x28,
    0x55, 0x00, 0xC4, 0x8E,  0x21, 0x00, 0x00, 0x1E, 0x01, 0x1D, 0x80, 0x18,  0x71, 0x1C, 0x16, 0x20,
    0x58, 0x2C, 0x25, 0x00,  0xC4, 0x8E, 0x21, 0x00, 0x00, 0x9E, 0x00, 0x00,  0x00, 0xFC, 0x00, 0x4D,
    0x54, 0x4B, 0x20, 0x4C,  0x43, 0x44, 0x54, 0x56, 0x0A, 0x20, 0x20, 0x20,  0x00, 0x00, 0x00, 0xFD,
    0x00, 0x31, 0x4C, 0x0F,  0x50, 0x0E, 0x00, 0x0A, 0x20, 0x20, 0x20, 0x20,  0x20, 0x20, 0x01, 0x56,
    0x02, 0x03, 0x23, 0x74,  0x4B, 0x84, 0x10, 0x1F, 0x05, 0x13, 0x14, 0x01,  0x02, 0x11, 0x06, 0x15,
    0x26, 0x09, 0x7F, 0x03,  0x11, 0x7F, 0x18, 0x83, 0x01, 0x00, 0x00, 0x67,  0x03, 0x0C, 0x00, 0x10,
    0x00, 0xB8, 0x2D, 0x01,  0x1D, 0x00, 0xBC, 0x52, 0xD0, 0x1E, 0x20, 0xB8,  0x28, 0x55, 0x40, 0xC4,
    0x8E, 0x21, 0x00, 0x00,  0x1E, 0x01, 0x1D, 0x80, 0xD0, 0x72, 0x1C, 0x16,  0x20, 0x10, 0x2C, 0x25,
    0x80, 0xC4, 0x8E, 0x21,  0x00, 0x00, 0x9E, 0x8C, 0x0A, 0xD0, 0x8A, 0x20,  0xE0, 0x2D, 0x10, 0x10,
    0x3E, 0x96, 0x00, 0x13,  0x8E, 0x21, 0x00, 0x00, 0x18, 0x8C, 0x0A, 0xD0,  0x90, 0x20, 0x40, 0x31,
    0x20, 0x0C, 0x40, 0x55,  0x00, 0x13, 0x8E, 0x21, 0x00, 0x00, 0x18, 0x00,  0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  0x00, 0x00, 0x00, 0x52
};

UINT8 HDMIEDID_CEC_192k_DeepColor12B_P0[256] =
{
    0x00, 0xFF, 0xFF, 0xFF,  0xFF, 0xFF, 0xFF, 0x00,   0x36, 0x8B, 0x01, 0x00,  0x01, 0x01, 0x01, 0x01,
    0x01, 0x0F, 0x01, 0x03,  0x80, 0x3C, 0x22, 0x78,   0x0A, 0x0D, 0xC9, 0xA0,  0x57, 0x47, 0x98, 0x27,
    0x12, 0x48, 0x4C, 0xBF,  0xEF, 0x00, 0x01, 0x01,   0x01, 0x01, 0x01, 0x01,  0x01, 0x01, 0x01, 0x01,
    0x01, 0x01, 0x01, 0x01,  0x01, 0x01, 0x01, 0x1D,   0x00, 0x72, 0x51, 0xD0,  0x1E, 0x20, 0x6E, 0x28,
    0x55, 0x00, 0xC4, 0x8E,  0x21, 0x00, 0x00, 0x1E,   0x01, 0x1D, 0x80, 0x18,  0x71, 0x1C, 0x16, 0x20,
    0x58, 0x2C, 0x25, 0x00,  0xC4, 0x8E, 0x21, 0x00,   0x00, 0x9E, 0x00, 0x00,  0x00, 0xFC, 0x00, 0x4D,
    0x54, 0x4B, 0x20, 0x4C,  0x43, 0x44, 0x54, 0x56,   0x0A, 0x20, 0x20, 0x20,  0x00, 0x00, 0x00, 0xFD,
    0x00, 0x31, 0x4C, 0x0F,  0x50, 0x0E, 0x00, 0x0A,   0x20, 0x20, 0x20, 0x20,  0x20, 0x20, 0x01, 0x56,
    0x02, 0x03, 0x23, 0x74,  0x4B, 0x84, 0x10, 0x1F,   0x05, 0x13, 0x14, 0x01,  0x02, 0x11, 0x06, 0x15,
    0x26, 0x09, 0x7F, 0x03,  0x11, 0x7F, 0x18, 0x83,   0x01, 0x00, 0x00, 0x67,  0x03, 0x0C, 0x00, 0x10,
    0x00, 0xB8, 0x2D, 0x01,  0x1D, 0x00, 0xBC, 0x52,   0xD0, 0x1E, 0x20, 0xB8,  0x28, 0x55, 0x40, 0xC4,
    0x8E, 0x21, 0x00, 0x00,  0x1E, 0x01, 0x1D, 0x80,   0xD0, 0x72, 0x1C, 0x16,  0x20, 0x10, 0x2C, 0x25,
    0x80, 0xC4, 0x8E, 0x21,  0x00, 0x00, 0x9E, 0x8C,   0x0A, 0xD0, 0x8A, 0x20,  0xE0, 0x2D, 0x10, 0x10,
    0x3E, 0x96, 0x00, 0x13,  0x8E, 0x21, 0x00, 0x00,   0x18, 0x8C, 0x0A, 0xD0,  0x90, 0x20, 0x40, 0x31,
    0x20, 0x0C, 0x40, 0x55,  0x00, 0x13, 0x8E, 0x21,   0x00, 0x00, 0x18, 0x00,  0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,  0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00,  0x00, 0x00, 0x00, 0x52
};
UINT8 HDMIEDID_3D_CEC_192k_DeepColor12B_P0[256] =
{
	0x00, 0xFF, 0xFF, 0xFF,  0xFF, 0xFF, 0xFF, 0x00, 0x36, 0x8B, 0x01, 0x00,  0x01, 0x01, 0x01, 0x01, 
	0x01, 0x0F, 0x01, 0x03,  0x80, 0x3C, 0x22, 0x78, 0x0A, 0x0D, 0xC9, 0xA0,  0x57, 0x47, 0x98, 0x27, 
	0x12, 0x48, 0x4C, 0xBF,  0xEF, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,  0x01, 0x01, 0x01, 0x01, 
	0x01, 0x01, 0x01, 0x01,  0x01, 0x01, 0x01, 0x1D, 0x00, 0x72, 0x51, 0xD0,  0x1E, 0x20, 0x6E, 0x28, 
	0x55, 0x00, 0xC4, 0x8E,  0x21, 0x00, 0x00, 0x1E, 0x01, 0x1D, 0x80, 0x18,  0x71, 0x1C, 0x16, 0x20, 
	0x58, 0x2C, 0x25, 0x00,  0xC4, 0x8E, 0x21, 0x00, 0x00, 0x9E, 0x00, 0x00,  0x00, 0xFC, 0x00, 0x4D, 
	0x54, 0x4B, 0x20, 0x4C,  0x43, 0x44, 0x54, 0x56, 0x0A, 0x20, 0x20, 0x20,  0x00, 0x00, 0x00, 0xFD, 
	0x00, 0x31, 0x4C, 0x0F,  0x50, 0x0E, 0x00, 0x0A, 0x20, 0x20, 0x20, 0x20,  0x20, 0x20, 0x01, 0x56, 
	0x02, 0x03, 0x26, 0x74,  0x4C, 0x84, 0x10, 0x1F, 0x05, 0x13, 0x14, 0x01,  0x02, 0x11, 0x06, 0x15, 0x20,
	0x26, 0x09, 0x7F, 0x03,  0x11, 0x7F, 0x18, 0x83, 0x01, 0x00, 0x00, 0x6A,  0x03, 0x0C, 0x00, 0x10, 
	0x00, 0xF8, 0x2D, 0x2F,  0x80, 0x00, 0x01, 0x1D, 0x00, 0xBC, 0x52, 0xD0,  0x1E, 0x20, 0xB8, 0x28, 
	0x55, 0x40, 0xC4, 0x8E,  0x21, 0x00, 0x00, 0x1E, 0x01, 0x1D, 0x80, 0xD0,  0x72, 0x1C, 0x16, 0x20, 
	0x10, 0x2C, 0x25, 0x80,  0xC4, 0x8E, 0x21, 0x00, 0x00, 0x9E, 0x8C, 0x0A,  0xD0, 0x8A, 0x20, 0xE0, 
	0x2D, 0x10, 0x10, 0x3E,  0x96, 0x00, 0x13, 0x8E, 0x21, 0x00, 0x00, 0x18,  0x8C, 0x0A, 0xD0, 0x90, 
	0x20, 0x40, 0x31, 0x20,  0x0C, 0x40, 0x55, 0x00, 0x13, 0x8E, 0x21, 0x00,  0x00, 0x18, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  0x00, 0x00, 0x3C//, 0x5D
};
UINT8 HDMIEDID_4K2K_P0[256] =
{
	0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x4d, 0xd9, 0xf3, 0xb4, 0x01, 0x01, 0x01, 0x01,
	0x31, 0x15, 0x01, 0x03, 0x80, 0xe0, 0x77, 0x78, 0x0a, 0x99, 0x55, 0xae, 0x51, 0x49, 0xb2, 0x24,
	0x0d, 0x50, 0x54, 0xa1, 0x08, 0x00, 0x90, 0x40, 0x81, 0x80, 0x81, 0x40, 0x01, 0x01, 0x01, 0x01,
	0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x02, 0x3a, 0x80, 0x18, 0x71, 0x38, 0x2d, 0x40, 0x58, 0x2c,
	0x45, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1e, 0x0e, 0x1f, 0x00, 0x80, 0x51, 0x00, 0x1e, 0x30,
	0x40, 0x80, 0x37, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1c, 0x00, 0x00, 0x00, 0xfd, 0x00, 0x17,
	0x40, 0x0f, 0x44, 0x0f, 0x00, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x00, 0x00, 0x00, 0xfc,
	0x00, 0x56, 0x50, 0x4c, 0x2d, 0x56, 0x57, 0x31, 0x30, 0x30, 0x30, 0x45, 0x53, 0x0a, 0x01, 0xb4,
	0x02, 0x03, 0x33, 0xf0, 0x4c, 0x10, 0x1f, 0x20, 0x05, 0x14, 0x04, 0x13, 0x03, 0x12, 0x07, 0x16,
	0x01, 0x26, 0x09, 0x7f, 0x03, 0x11, 0x7f, 0x18, 0x7a, 0x03, 0x0c, 0x00, 0x10, 0x00, 0xb8, 0x3c,
	0xe0, 0x1c, 0xff, 0x1c, 0xff, 0xc8, 0x88, 0x01, 0x02, 0x03, 0x00, 0x01, 0x40, 0x00, 0x63, 0x38, 
	0x10, 0x48, 0x10, 0x02, 0x3a, 0x80, 0xd0, 0x72, 0x38, 0x2d, 0x40, 0x10, 0x2c, 0x45, 0x80, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x1e, 0x01, 0x1d, 0x80, 0x18, 0x71, 0x1c, 0x16, 0x20, 0x58, 0x2c, 0x25,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x9e, 0x8c, 0x0a, 0xd0, 0x8a, 0x20, 0xe0, 0x2d, 0x10, 0x10,
	0x3e, 0x96, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x8c, 0x0a, 0xd0, 0x90, 0x20, 0x40, 0x31,
	0x20, 0x0c, 0x40, 0x55, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x39
};
void vDrvLoadHdmiEdid2Table(void)
{
#ifndef CC_FPGA
    UINT64 u8Offset;
    UINT8 idx = 0;
    //read form eeprom or flash to VGA_EDID_TABLE

#ifdef CC_MULTI_EDID
    UINT32 u4EdidOffset;
    if(DRVCUST_InitQuery(eEdidHdmiIndexOffset, &u4EdidOffset) == 0)
    {
        EEPROM_Read((UINT64)u4EdidOffset, (UINT32)&idx, 1);
    }
#endif

    if((idx+1)*HDMI_EDID_SIZE > HDMI_EDID_STORAGE_SIZE)
    {
        u8Offset = 0;
    }
    else
    {
        u8Offset = idx*HDMI_EDID_SIZE;
    }
    //read form eeprom or flash to HDMIEDID_CEC_192k_DeepColor12B_P0
    if (EEPHDMIEDID_Read(u8Offset,  (UINT32) HDMIEDID_table, HDMI_EDID_SIZE ))
    {
        Printf("read eeprom edid fail\n");
    }

#endif
}
#endif

#endif // _EDID_CUSTOMIZATION_H_
