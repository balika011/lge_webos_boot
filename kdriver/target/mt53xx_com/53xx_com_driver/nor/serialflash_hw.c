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
 * $RCSfile: serialflash_hw.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file serialflash_hw.c
 *  Brief of file serialflash_hw.c.
 *  Details of file serialflash_hw.c (optional).
 */


//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#include "nor_if.h"
#ifndef CC_NOR_DISABLE

#include "x_typedef.h"
#include "x_os.h"
#include "x_bim.h"
#include "x_assert.h"
#include "x_timer.h"
#include "serialflash_hw.h"
#ifdef BD_NOR_ENABLE
#include "x_debug.h"
#include <mach/cache_operation.h>
#include "x_ckgen.h"
#else
#include "nor_debug.h"
#include "x_hal_arm.h"
#include "x_printf.h"
#endif

//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------
//#define USE_SECOND_FLASH
//#define ENABLE_DUALREAD
//#define ENABLE_CONTROLLER_STATUS
//#define ENABLE_AAIWRITE
//#define USE_FLASH_ERASE_ISR			//5881 5399 feasure
//#define USE_FASTREAD

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
#define SFLASH_POLLINGREG_COUNT     500000
#define SFLASH_WRITECMD_TIMEOUT     100000
#define SFLASH_WRITEBUSY_TIMEOUT    500000
#define SFLASH_ERASESECTOR_TIMEOUT  200000
#define SFLASH_CHIPERASE_TIMEOUT    500000

#define SFLASH_HW_ALIGNMENT         4
#ifndef SFLASH_WRITE_PROTECTION_VAL
  #define SFLASH_WRITE_PROTECTION_VAL 0x00
#endif
//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------
#ifdef REPLACE_EEPROM_WITH_NOR
static SFLASHHW_VENDOR_T _aVendorFlash[] =
{
    { 0xC2, 0x20, 0x17, 0x0, 0x800000,	0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0x00, 0x3B, SFLASH_WRITE_PROTECTION_VAL,0x34, "MXIC(25L640)" },
	  { 0xC2, 0x20, 0x18, 0x0, 0x1000000, 0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0x00, 0x00, SFLASH_WRITE_PROTECTION_VAL,0x00, "MXIC(25L128)" },

    { 0xEF, 0x40, 0x17, 0x0, 0x800000,  0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0x00, 0xBB, SFLASH_WRITE_PROTECTION_VAL,0x08, "WINBOND(W25Q64FV)" },
    { 0xEF, 0x40, 0x18, 0x0, 0x1000000,  0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0x00, 0x00,SFLASH_WRITE_PROTECTION_VAL, 0x04,"WINBOND(W25Q128BV)" },

    { 0x1C, 0x20, 0x17, 0x0, 0x800000,	0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0x00, 0x00, SFLASH_WRITE_PROTECTION_VAL,0x00, "EON(EN25P64)" },
    { 0x1C, 0x30, 0x17, 0x0, 0x800000,	0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0x00, 0x00, SFLASH_WRITE_PROTECTION_VAL,0x0C, "EON(EN25Q64)" },//0x08  protect 0-125 block

	  { 0xC8, 0x40, 0x18, 0x0, 0x1000000, 0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0x00, 0x00,SFLASH_WRITE_PROTECTION_VAL, 0x04,"GD(GD25Q128BSIG)" },
	  { 0xC8, 0x40, 0x17, 0x0, 0x800000, 0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0x00, 0x00,SFLASH_WRITE_PROTECTION_VAL, 0x08,"GD(GD25Q64BSIG)" },

	  { 0x00, 0x00, 0x00, 0x0, 0x000000,	0x00000, 0x000, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, SFLASH_WRITE_PROTECTION_VAL,0x00, "NULL Device" },

};

#else
static SFLASHHW_VENDOR_T _aVendorFlash[] =
{
    { 0x01, 0x02, 0x12, 0x0,  0x80000,  0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0x00, 0x00, SFLASH_WRITE_PROTECTION_VAL, "Spansion(S25FL004A)" },
    { 0x01, 0x02, 0x13, 0x0, 0x100000, 0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0x00, 0x00, SFLASH_WRITE_PROTECTION_VAL, "Spansion(S25FL008A)" },
    { 0x01, 0x02, 0x14, 0x0, 0x200000, 0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0x00, 0x00, SFLASH_WRITE_PROTECTION_VAL, "Spansion(S25FL016A)" },
    { 0x01, 0x02, 0x15, 0x0, 0x400000, 0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0x00, 0x00, SFLASH_WRITE_PROTECTION_VAL, "Spansion(S25FL032A)" },
    { 0x01, 0x02, 0x16, 0x0, 0x800000, 0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0x00, 0x00, SFLASH_WRITE_PROTECTION_VAL, "Spansion(S25FL064A)" },
    { 0x01, 0x20, 0x18, 0x0,0x1000000, 0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0x00, 0x00, SFLASH_WRITE_PROTECTION_VAL, "Spansion(S25FL128P)" },
    { 0x01, 0x02, 0x19, 0x0,0x2000000, 0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0x00, 0x00, SFLASH_WRITE_PROTECTION_VAL, "Spansion(S25FL256S)" },
	{ 0x01, 0x40, 0x15, 0x0, 0x200000,	0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0x00, 0x00, SFLASH_WRITE_PROTECTION_VAL, "Spansion(S25FL016A)" },

    { 0xC2, 0x20, 0x13, 0x0,  0x80000,  0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0x00, 0x00, SFLASH_WRITE_PROTECTION_VAL, "MXIC(25L400)" },
    { 0xC2, 0x20, 0x14, 0x0, 0x100000, 0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0x00, 0x00, SFLASH_WRITE_PROTECTION_VAL, "MXIC(25L80)" },
	  { 0xC2, 0x20, 0x15, 0x0, 0x200000, 0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0x00, 0x00, SFLASH_WRITE_PROTECTION_VAL, "MXIC(25L160)" },
    { 0xC2, 0x24, 0x15, 0x0, 0x200000,  0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0x00, 0x00, SFLASH_WRITE_PROTECTION_VAL, "MXIC(25L1635D)" },
    { 0xC2, 0x20, 0x16, 0x0, 0x400000, 0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0x00, 0x00, SFLASH_WRITE_PROTECTION_VAL, "MXIC(25L320)" },
    { 0xC2, 0x20, 0x17, 0x0, 0x800000, 0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0x00, 0x00, SFLASH_WRITE_PROTECTION_VAL, "MXIC(25L640)" },
    { 0xC2, 0x20, 0x18, 0x0, 0x1000000, 0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0x00, 0x00, SFLASH_WRITE_PROTECTION_VAL, "MXIC(25L1280)" },
    { 0xC2, 0x5E, 0x16, 0x0, 0x400000,	0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0x00, 0x00, SFLASH_WRITE_PROTECTION_VAL, "MXIC(25L3235D)" },
	  { 0xC2, 0x20, 0x19, 0x0, 0x2000000, 0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0x00, 0x3b, SFLASH_WRITE_PROTECTION_VAL, "MXIC(25L256)" },

    { 0x20, 0x20, 0x14, 0x0, 0x100000,  0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0x00, 0x00, SFLASH_WRITE_PROTECTION_VAL, "ST(M25P80)" },
    { 0x20, 0x20, 0x15, 0x0, 0x200000, 0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0x00, 0x00, SFLASH_WRITE_PROTECTION_VAL, "ST(M25P16)" },
    { 0x20, 0x20, 0x16, 0x0, 0x400000, 0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0x00, 0x00, SFLASH_WRITE_PROTECTION_VAL, "ST(M25P32)" },
    { 0x20, 0x20, 0x17, 0x0, 0x800000, 0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0x00, 0x00, SFLASH_WRITE_PROTECTION_VAL, "ST(M25P64)" },
    { 0x20, 0x20, 0x18, 0x0, 0x1000000, 0x40000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0x00, 0x00, SFLASH_WRITE_PROTECTION_VAL, "ST(M25P128)" },
    { 0x20, 0x71, 0x16, 0x0, 0x400000,	0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0x00, 0x00, SFLASH_WRITE_PROTECTION_VAL, "ST(M25PX32)" },
    { 0x20, 0x71, 0x17, 0x0, 0x800000,	0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0x00, 0x00, SFLASH_WRITE_PROTECTION_VAL, "ST(M25PX64)" },
    { 0x20, 0xBA, 0x17, 0x0, 0x800000,	0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0x00, 0x00, SFLASH_WRITE_PROTECTION_VAL, "ST(M25P64)" },

    { 0xBF, 0x25, 0x41, 0x0, 0x200000, 0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0xAD, 0x00, SFLASH_WRITE_PROTECTION_VAL, "SST(25VF016B)" },

    { 0xEF, 0x30, 0x13, 0x0, 0x80000,  0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0x00, 0x00,SFLASH_WRITE_PROTECTION_VAL, "WINBOND(W25X40)" },
    { 0xEF, 0x30, 0x14, 0x0, 0x100000, 0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0x00, 0x00, SFLASH_WRITE_PROTECTION_VAL, "WINBOND(W25X80)" },
    { 0xEF, 0x30, 0x15, 0x0, 0x200000, 0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0x00, 0x00, SFLASH_WRITE_PROTECTION_VAL, "WINBOND(W25X16)" },
    { 0xEF, 0x30, 0x16, 0x0, 0x400000, 0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0x00, 0x00, SFLASH_WRITE_PROTECTION_VAL, "WINBOND(W25X32)" },
    { 0xEF, 0x30, 0x17, 0x0, 0x800000,	0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0x00, 0x00, SFLASH_WRITE_PROTECTION_VAL, "WINBOND(W25X64)" },
	  { 0xEF, 0x40, 0x19, 0x0, 0x2000000, 0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0x00, 0x00, SFLASH_WRITE_PROTECTION_VAL, "WINBOND(W25Q256FV)" },
    { 0xEF, 0x40, 0x16, 0x0, 0x400000,  0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0x00, 0x00, SFLASH_WRITE_PROTECTION_VAL, "WINBOND(W25Q32BV)" },
    { 0xEF, 0x40, 0x17, 0x0, 0x800000,  0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0x00, 0xBB, SFLASH_WRITE_PROTECTION_VAL, "WINBOND(W25Q64BV)" },
    { 0xEF, 0x40, 0x18, 0x0, 0x1000000,  0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0x00, 0x00,SFLASH_WRITE_PROTECTION_VAL, "WINBOND(W25Q128BV)" },

    { 0xC8, 0x40, 0x15, 0x0, 0x200000,  0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0x00, 0x00,SFLASH_WRITE_PROTECTION_VAL, "GD(GD25Q16BSIG)" },
	{ 0xC8, 0x40, 0x16, 0x0, 0x400000,	0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0x00, 0x00,SFLASH_WRITE_PROTECTION_VAL, "GD(GD25Q32BSIG)" },
	{ 0xC8, 0x40, 0x17, 0x0, 0x800000,	0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0x00, 0x00,SFLASH_WRITE_PROTECTION_VAL, "GD(GD25Q64BSIG)" },
	{ 0xC8, 0x40, 0x18, 0x0, 0x1000000,	0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0x00, 0x00,SFLASH_WRITE_PROTECTION_VAL, "GD(GD25Q128BSIG)" },
#if (defined(CC_MT5360B) || defined(CC_MT5387) ||defined(CC_MT5363) ||defined(ENABLE_AAIWRITE))
    { 0xBF, 0x43, 0x10, 0x0,  0x40000,  0x8000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0x52, 0x60, 0x02, 0xAF, 0x00,  SFLASH_WRITE_PROTECTION_VAL,"SST(SST25VF020)" },
    { 0xBF, 0x25, 0x8D, 0x0,  0x80000, 0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0xAD, 0x00,  SFLASH_WRITE_PROTECTION_VAL,"SST(SST25VF040B)" },
    { 0xBF, 0x25, 0x8E, 0x0, 0x100000, 0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0xAD, 0x00,  SFLASH_WRITE_PROTECTION_VAL,"SST(SST25VF080B)" },
    { 0xBF, 0x25, 0x41, 0x0, 0x200000, 0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0xAD, 0x00,  SFLASH_WRITE_PROTECTION_VAL,"SST(SST25VF016B)" },
    { 0xBF, 0x25, 0x4A, 0x0, 0x400000, 0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0xAD, 0x00,  SFLASH_WRITE_PROTECTION_VAL,"SST(SST25VF032B)" },
    { 0xBF, 0x25, 0x4B, 0x0, 0x800000, 0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0xAD, 0x00,  SFLASH_WRITE_PROTECTION_VAL,"SST(SST25VF064C)" },
#else
    { 0xBF, 0x43, 0x10, 0x1,  0x40000,  0x8000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0x52, 0x60, 0x02, 0xAF, 0x00,  SFLASH_WRITE_PROTECTION_VAL,"SST(SST25VF020)" },
    { 0xBF, 0x25, 0x8D, 0x1,  0x80000, 0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0xAD, 0x00,  SFLASH_WRITE_PROTECTION_VAL,"SST(SST25VF040B)" },
    { 0xBF, 0x25, 0x8E, 0x1, 0x100000, 0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0xAD, 0x00,  SFLASH_WRITE_PROTECTION_VAL,"SST(SST25VF080B)" },
    { 0xBF, 0x25, 0x41, 0x1, 0x200000, 0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0xAD, 0x00,  SFLASH_WRITE_PROTECTION_VAL,"SST(SST25VF016B)" },
    { 0xBF, 0x25, 0x4A, 0x1, 0x400000, 0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0xAD, 0x00,  SFLASH_WRITE_PROTECTION_VAL,"SST(SST25VF032B)" },
    { 0xBF, 0x25, 0x4B, 0x1, 0x800000, 0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0xAD, 0x00,  SFLASH_WRITE_PROTECTION_VAL,"SST(SST25VF064C)" },
#endif

    { 0x1F, 0x47, 0x00, 0x0, 0x400000,  0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0x00, 0x00, SFLASH_WRITE_PROTECTION_VAL, "ATMEL(AT25DF321)" },
    { 0x1F, 0x48, 0x00, 0x0, 0x800000,  0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0x00, 0x00, SFLASH_WRITE_PROTECTION_VAL, "ATMEL(AT25DF641)" },
	{ 0x1F, 0x45, 0x01, 0x0, 0x100000, 0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0x00, 0x00, SFLASH_WRITE_PROTECTION_VAL, "ATMEL(AT26DF081A)" },
    { 0x1C, 0x20, 0x13, 0x0,  0x80000,  0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0x00, 0x00, SFLASH_WRITE_PROTECTION_VAL, "EON(EN25B40)" },
    { 0x1C, 0x31, 0x14, 0x0, 0x100000,  0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0x00, 0x00, SFLASH_WRITE_PROTECTION_VAL, "EON(EN25F80)" },
    { 0x1C, 0x20, 0x15, 0x0, 0x200000,  0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0x00, 0x00, SFLASH_WRITE_PROTECTION_VAL, "EON(EN25P16)" },
    { 0x1C, 0x20, 0x16, 0x0, 0x400000,  0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0x00, 0x00, SFLASH_WRITE_PROTECTION_VAL, "EON(EN25P32)" },
    { 0x1C, 0x20, 0x17, 0x0, 0x800000,  0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0x00, 0x00, SFLASH_WRITE_PROTECTION_VAL, "EON(EN25P64)" },
    { 0x1C, 0x30, 0x17, 0x0, 0x800000,  0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0x00, 0x00, SFLASH_WRITE_PROTECTION_VAL, "EON(EN25Q64)" },
    { 0x1C, 0x20, 0x14, 0x0, 0x100000, 0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0x00, 0x00, SFLASH_WRITE_PROTECTION_VAL, "EON(EN25P80/EN25B80)" },
    { 0x7F, 0x37, 0x20, 0x0, 0x200000,  0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0x00, 0x00, SFLASH_WRITE_PROTECTION_VAL, "AMIC(A25L40P)" },
    { 0x37, 0x30, 0x13, 0x0, 0x100000,  0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0x00, 0x00, SFLASH_WRITE_PROTECTION_VAL, "AMIC(A25L040)" },
    { 0x37, 0x30, 0x16, 0x0, 0x400000,  0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0x00, 0x00, SFLASH_WRITE_PROTECTION_VAL, "AMIC(A25L032)" },

    { 0xFF, 0xFF, 0x10, 0x0,  0x10000,  0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xC7, 0xC7, 0x02, 0x00, 0x00, SFLASH_WRITE_PROTECTION_VAL, "512Kb" },
//    { 0xFF, 0xFF, 0x11, 0x0,  0x20000,  0x8000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0x00, 0x00, SFLASH_WRITE_PROTECTION_VAL, "1Mb" },
    { 0xFF, 0xFF, 0x12, 0x0,  0x40000,  0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0x00, 0x00, SFLASH_WRITE_PROTECTION_VAL, "2Mb" },
    { 0xFF, 0xFF, 0x13, 0x0,  0x80000,  0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0x00, 0x00, SFLASH_WRITE_PROTECTION_VAL, "4Mb" },
    { 0xFF, 0xFF, 0x14, 0x0, 0x100000,  0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0x00, 0x00, SFLASH_WRITE_PROTECTION_VAL, "8Mb" },
    { 0xFF, 0xFF, 0x15, 0x0, 0x200000,  0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0x00, 0x00, SFLASH_WRITE_PROTECTION_VAL, "16Mb" },
    { 0xFF, 0xFF, 0x16, 0x0, 0x400000,  0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0x00, 0x00, SFLASH_WRITE_PROTECTION_VAL, "32Mb" },
    { 0xFF, 0xFF, 0x17, 0x0, 0x800000,  0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0x00, 0x00, SFLASH_WRITE_PROTECTION_VAL, "64Mb" },
    { 0xFF, 0xFF, 0x18, 0x0, 0x1000000, 0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0x00, 0x00, SFLASH_WRITE_PROTECTION_VAL, "128Mb" },

    { 0x00, 0x00, 0x00, 0x0, 0x000000, 0x00000, 0x0000, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, SFLASH_WRITE_PROTECTION_VAL, "NULL Device" },
};
#endif
//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------
#define SFLASH_WREG8(offset, value)       IO_WRITE8(SFLASH_REG_BASE, (offset), (value))
#define SFLASH_RREG8(offset)               IO_READ8(SFLASH_REG_BASE, (offset))

#define SFLASH_WREG32(offset, value)       IO_WRITE32(SFLASH_REG_BASE, (offset), (value))
#define SFLASH_RREG32(offset)               IO_READ32(SFLASH_REG_BASE, (offset))


#define LoWord(d)     ((UINT16)(d & 0x0000ffffL))
#define HiWord(d)     ((UINT16)((d >> 16) & 0x0000ffffL))
#define LoByte(w)     ((UINT8)(w & 0x00ff))
#define HiByte(w)     ((UINT8)((w >> 8) & 0x00ff))
#define MidWord(d)    ((UINT16)((d >>8) & 0x00ffff))

#define BYTE0(arg)    (*(UINT8 *)&arg)
#define BYTE1(arg)    (*((UINT8 *)&arg + 1))
#define BYTE2(arg)    (*((UINT8 *)&arg + 2))
#define BYTE3(arg)    (*((UINT8 *)&arg + 3))


//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------
static SFLASHHW_VENDOR_T _arFlashChip[MAX_FLASHCOUNT];

static UINT32 _u4ChipCount;

#if !defined (CC_MTK_LOADER)&& !defined(CC_MTK_LOADER)
static HANDLE_T _hSFLASHSema;
#endif /* CC_MTK_LOADER */

static UINT32 _u4SFIsrStatus;
static BOOL _fgSFIsrEn = TRUE;
#ifdef REPLACE_EEPROM_WITH_NOR
BOOL _fgDoProtect = FALSE;
#else
static BOOL _fgDoProtect = FALSE;
BOOL _fgWriteProtect = TRUE;
#endif
#if (defined(ENABLE_DUALREAD))
static UINT8 _u1DualReadCmd = 0;
#endif


#if (defined(ENABLE_AAIWRITE))
static BOOL _fgAAIWrite = TRUE;
#else
static BOOL _fgAAIWrite = FALSE;
#endif


#if !defined(CC_MTK_LOADER) && !defined(BD_NOR_ENABLE)
UINT32 _fgNorPartialProtection = 1;
#else
UINT32 _fgNorPartialProtection = 0;
#endif

#ifdef BD_NOR_ENABLE
UINT32 SZ_OPT_UNIT_SIZE = 0x10000;
static UINT8 u1MenuID, u1DevID1, u1DevID2;
#endif
//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------

static void _SendFlashCommand(UINT8 u1Val)
{
#if !defined (CC_MTK_LOADER)&& !defined(CC_MTK_LOADER)
	CRIT_STATE_T csState;

	csState = x_crit_start();
#endif
    SFLASH_WREG8(SFLASH_CMD_REG, u1Val);
#if !defined (CC_MTK_LOADER)&& !defined(CC_MTK_LOADER)
    x_crit_end(csState);
#endif
}

//-----------------------------------------------------------------------------
static INT32 _PollingReg(UINT8 u1RegOffset, UINT8 u8Compare)
{
    UINT32 u4Polling;
    UINT8 u1Reg;

    u4Polling = 0;
    while(1)
    {
        u1Reg = SFLASH_RREG8(u1RegOffset);
        if (0x00 == (u1Reg & u8Compare))
        {
            break;
        }

        u4Polling++;
        if(u4Polling > SFLASH_POLLINGREG_COUNT)
        {
            return 1;
        }
    }

    return 0;
}


#if !defined (CC_MTK_LOADER)&& !defined(CC_MTK_LOADER)
//-----------------------------------------------------------------------------
/** _SFLASHHW_Isr()
 */
//-----------------------------------------------------------------------------
static void _SFLASHHW_Isr(UINT16 u2VctId)
{
    UINT32 u4Val;

    UNUSED(u2VctId);
    VERIFY(BIM_ClearIrq(VECTOR_FLASH));

    u4Val = SFLASH_RREG32(SFLASH_SF_INTRSTUS_REG);
#ifdef BD_NOR_ENABLE
    if (u4Val & 0x7F)
#else
    if (u4Val & 0x3F)
#endif
    {
        // Clear interrupt
        SFLASH_WREG32(SFLASH_SF_INTRSTUS_REG, u4Val);
        _u4SFIsrStatus = u4Val;
        VERIFY(x_sema_unlock(_hSFLASHSema) == OSR_OK);
    }
}
#endif /* CC_MTK_LOADER */

#ifndef BD_NOR_ENABLE
//-----------------------------------------------------------------------------
static void _SetFirstFlashSizeReg(void)
{
    UINT8 u1Reg;

    u1Reg = SFLASH_RREG8(SFLASH_CFG1_REG);
    u1Reg &= (~0xC);

#ifdef USE_SECOND_FLASH
        switch(_arFlashChip[0].u4ChipSize)
        {
        case 0x200000:
            u1Reg |= 0xC;
            break;
        case 0x400000:
            u1Reg |= 0x8;
            break;
		case 0x1000000:
            u1Reg |= 0x14;
			break;
        case 0x800000:
        default:
            u1Reg |= 0x4;
            break;
        }
#endif
	SFLASH_WREG8(SFLASH_CFG1_REG, u1Reg);
}
#endif
//-----------------------------------------------------------------------------
static void _SetFlashCountReg(UINT32 u4Count)
{
    UINT8 u1Reg;

    u1Reg = SFLASH_RREG8(SFLASH_CFG1_REG);
    u1Reg &= (~0xC);

    if(u4Count == 1)
    {
        SFLASH_WREG8(SFLASH_CFG1_REG, u1Reg);
    }
#ifdef USE_SECOND_FLASH
    else if(u4Count == 2)
    {
        switch(_arFlashChip[0].u4ChipSize)
        {
        case 0x200000:
            u1Reg |= 0xC;
            break;
        case 0x400000:
            u1Reg |= 0x8;
            break;
        case 0x800000:
        default:
            u1Reg |= 0x4;
            break;
        }

        SFLASH_WREG8(SFLASH_CFG1_REG, u1Reg);
    }
#endif
    else
    {
        ASSERT(0);
    }
}

//-----------------------------------------------------------------------------
static INT32 _DoIdentify(UINT32 u4Index, UINT32 *pu4Vendor)
{
#ifndef BD_NOR_ENABLE
   UINT8 u1MenuID = 0, u1DevID1 = 0, u1DevID2 = 0;
#endif
    UINT32 i;

    if(pu4Vendor == NULL)
    {
        return 1;
    }

    *pu4Vendor = 0xFFFFFFFF;

    if(SFLASHHW_GetID(u4Index, &u1MenuID, &u1DevID1, &u1DevID2) != 0)
    {
        return 1;
    }
	LOG(0, "Flash index :%d,MenuID: 0x%X, DeviceID1: 0x%X, DeviceID2: 0x%X\n",\
            u4Index,u1MenuID, u1DevID1, u1DevID2);
    i = 0;

    while(_aVendorFlash[i].u1MenuID != (UINT8)0x0)
    {
        if( (_aVendorFlash[i].u1MenuID == u1MenuID) &&
            (_aVendorFlash[i].u1DevID1 == u1DevID1) &&
            (_aVendorFlash[i].u1DevID2 == u1DevID2))
        {
            *pu4Vendor = i;

#if(defined(ENABLE_DUALREAD))
            if (_aVendorFlash[i].u1DualREADCmd)
            {
                _u1DualReadCmd = _aVendorFlash[i].u1DualREADCmd;
                LOG(0, "dualread flash _u1DualReadCmd = 0x%x\n",_u1DualReadCmd);
            }
#endif
		#if (defined(ENABLE_AAIWRITE))
            if ( _aVendorFlash[i].u1AAIPROGRAMCmd)
            {
                _fgAAIWrite = TRUE;
                LOG(0, "AAIwrite flash\n");
            }
#endif
            return 0;
        }
#ifndef BD_NOR_ENABLE
        if((_aVendorFlash[i].u1MenuID == ((UINT8)0xFF)) && (_aVendorFlash[i].u1DevID1 == ((UINT8)0xFF)))
        {
           if(_aVendorFlash[i].u1DevID2 == u1DevID2)
           {
              *pu4Vendor = i;
              return 0;
           }
        }
#endif
        i++;
    }

    return 1;
}

//-----------------------------------------------------------------------------
static INT32 _WBEnable(void)
{
    UINT32 u4Polling;
    UINT8 u1Reg;
    UINT8 u1Tmp;
#ifdef BD_NOR_ENABLE
    u1Tmp = 0x01;
#else
    u1Tmp = 0x0D;
#endif
    if (_fgAAIWrite == TRUE)
    {
        if(_arFlashChip[0].u1AAIPROGRAMCmd != 0xAF)
        {
          u1Tmp |= 0x40;
        }
    }
    SFLASH_WREG8(SFLASH_CFG2_REG, u1Tmp);

    u4Polling = 0;
    while(1)
    {
        u1Reg = SFLASH_RREG8(SFLASH_CFG2_REG);
        if (0x01 == (u1Reg & 0x1))
        {
            break;
        }

        u4Polling++;
        if(u4Polling > SFLASH_POLLINGREG_COUNT)
        {
            return 1;
        }
    }

    return 0;
}

//-----------------------------------------------------------------------------
static INT32 _WBDisable(void)
{
    UINT32 u4Polling;
    UINT8 u1Reg;
	UINT8 u1Tmp;
#ifdef BD_NOR_ENABLE
    u1Tmp = 0x00;
#else
    u1Tmp = 0x0C;
#endif

    SFLASH_WREG8(SFLASH_CFG2_REG, u1Tmp);
    u4Polling = 0;
    while(1)    // Wait for finish write buffer
    {
        u1Reg = SFLASH_RREG8(SFLASH_CFG2_REG);
        if (u1Tmp == (u1Reg & 0xF))
        {
            break;
        }

        u4Polling++;
        if(u4Polling > SFLASH_POLLINGREG_COUNT)
        {
            return 1;
        }
    }

    return 0;
}


//-----------------------------------------------------------------------------
static INT32 _WaitForWriteBusy(UINT32 u4Index, UINT32 u4Timeout)
{
    UINT32 u4Count;
    UINT8 u1Reg;

    u4Count = 0;
    while(1)
    {
        if(SFLASHHW_ReadFlashStatus(u4Index, &u1Reg) != 0)
        {
            return 1;
        }

        if(0 == (u1Reg & 0x1))
        {
            break;
        }

        u4Count++;
        if(u4Count > u4Timeout)
        {
            return 1;
        }

        //x_thread_delay(5);
    }

    return 0;
}

//-----------------------------------------------------------------------------
static INT32 _SetFlashWriteEnable(UINT32 u4Index)
{

    SFLASH_WREG8(SFLASH_PRGDATA5_REG, _arFlashChip[u4Index].u1WRENCmd);
    SFLASH_WREG8(SFLASH_CNT_REG,8); // Write SF Bit Count

	if(u4Index==0)
    {
        _SendFlashCommand(0x4);	// Write PRG

    }
#ifdef USE_SECOND_FLASH
    else if(u4Index == 1)
    {
        _SendFlashCommand(0x44);	// Write PRG
    }
#endif
    else
    {
        ASSERT(0);
    }

    if( _PollingReg(SFLASH_CMD_REG, 0x04) !=0) // timeout
    {
        return 1;
    }

    return 0;
}



//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/** Brief of SFLASHHW_Init.
 *  Details of SFLASHHW_Init (optional).
 *  @retval TRUE    Success
 *  @retval FALSE   Fail
 */
//-----------------------------------------------------------------------------

INT32 SFLASHHW_Init(void)
{
    UINT32 u4Loop, u4VendorIdx;
#if !defined (CC_MTK_LOADER)&& !defined(CC_MTK_LOADER)
    void (* pfnOldIsr)(UINT16);
#endif /* CC_MTK_LOADER */

#ifndef BD_NOR_ENABLE
    UINT32 u4Val;
    // Set to NOR flash
    u4Val = BIM_READ32(0x3B0);
    u4Val &= ~0x4;
    BIM_WRITE32(0x3B0, u4Val);

    // MT5391
    if(SFLASH_RREG32(SFLASH_CFG1_REG) & 0x01)
    {
        SFLASH_WREG32(SFLASH_CFG1_REG, 0x21);
    }
    else
    {
        SFLASH_WREG32(SFLASH_CFG1_REG, 0x20);
    }
#endif
    x_memset((VOID*)_arFlashChip, 0x0, MAX_FLASHCOUNT*sizeof(SFLASHHW_VENDOR_T));

    _u4ChipCount = 0;
    for(u4Loop = 0; u4Loop < MAX_FLASHCOUNT; u4Loop++)
    {
        if(_DoIdentify(u4Loop, &u4VendorIdx) != 0)
        {
            continue;
        }

        x_memcpy((VOID*)&(_arFlashChip[_u4ChipCount]),
                 (VOID*)&(_aVendorFlash[u4VendorIdx]),
                 sizeof(SFLASHHW_VENDOR_T));

        //VERIFY(NULL != x_strncpy(_arFlashChip[_u4ChipCount].pcFlashStr, _aVendorFlash[u4VendorIdx].pcFlashStr, x_strlen(_aVendorFlash[u4VendorIdx].pcFlashStr)));
#ifndef BD_NOR_ENABLE
        //--------------------------------------------------
        // must set the first flash size otherwise don't identify the second flash id.
        //--------------------------------------------------
        _SetFirstFlashSizeReg();
#endif
        _u4ChipCount++;
        LOG(0, "Detect flash #%d: %s\n", u4Loop, _arFlashChip[u4Loop].pcFlashStr);
    }

    if(_u4ChipCount == 0)
    {
        LOG(1, "There is no flash!\n");
        return 1;
    }

    //--------------------------------------------------
    // Set flash number register
    //--------------------------------------------------
    _SetFlashCountReg(_u4ChipCount);

#ifdef BD_NOR_ENABLE
	//set clock
    CKGEN_WRITE32(REG_RW_CLK_CFG0, CKGEN_READ32(REG_RW_CLK_CFG0) | 0x3000);//54mhz
#endif
#if !defined (CC_MTK_LOADER)&& !defined(CC_MTK_LOADER)
    //--------------------------------------------------
    // Register ISR
    //--------------------------------------------------
    VERIFY(x_sema_create(&_hSFLASHSema, X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK) == OSR_OK);
#ifdef BD_NOR_ENABLE
    SFLASH_WREG32(SFLASH_SF_INTREN_REG, 0x50); //enable pp Interrupt & aai
#else
		SFLASH_WREG32(SFLASH_SF_INTREN_REG, 0x0);
#endif
    VERIFY(BIM_ClearIrq(VECTOR_FLASH));

    VERIFY(x_reg_isr(VECTOR_FLASH, _SFLASHHW_Isr, &pfnOldIsr) == OSR_OK);
#endif /* CC_MTK_LOADER */

    _fgSFIsrEn = FALSE;
    _u4SFIsrStatus = 0;

	//Guarantee every time nor init can WriteProtect
#ifdef REPLACE_EEPROM_WITH_NOR
    _fgDoProtect = TRUE;
    if(_fgDoProtect)
    {
    	for(u4Loop=0; u4Loop < _u4ChipCount; u4Loop++)
    	{
    		if(SFLASHHW_WriteProtect(u4Loop, TRUE) != 0)
    		{
    			return -1;
    		}
    	}
    }
#else
	_fgWriteProtect = TRUE;

	if(_fgWriteProtect)
	{
        _fgDoProtect = TRUE;
        if(_fgDoProtect)
        {
            for(u4Loop=0; u4Loop < _u4ChipCount; u4Loop++)
            {
                if(SFLASHHW_WriteProtect(u4Loop, TRUE) != 0)
                {
                    return -1;
                }
            }
        }
	}
#endif
    return 0;
}

//-----------------------------------------------------------------------------
/** Brief of SFLASHHW_ReadFlashStatus.
 *  Details of SFLASHHW_ReadFlashStatus (optional).
 *  @retval 1    Success
 *  @retval 0   Fail
 */
//-----------------------------------------------------------------------------
INT32 SFLASHHW_ReadFlashStatus(UINT32 u4Index, UINT8 *pu1Val)
{
    if(pu1Val == NULL)
    {
        return 1;
    }
#ifdef REPLACE_EEPROM_WITH_NOR
	if((0xEF ==_arFlashChip[u4Index].u1MenuID) && (0x40 ==_arFlashChip[u4Index].u1DevID1) && (0x17 ==_arFlashChip[u4Index].u1DevID2))
	{
		//cmd 05 read nor first status register
		SFLASH_WREG8(SFLASH_PRGDATA5_REG, 0x05);	// Write
		SFLASH_WREG8(SFLASH_PRGDATA4_REG, 0x00);	// Write
		SFLASH_WREG8(SFLASH_PRGDATA3_REG, 0x00);	// Write
		SFLASH_WREG8(SFLASH_CNT_REG, 24);			// Write SF Bit Count

		if(u4Index == 0)
		{
			_SendFlashCommand(0x4);
		}
		if( _PollingReg(SFLASH_CMD_REG, 0x04) != 0) // timeout
		{
			return -1;
		}
		*pu1Val = SFLASH_RREG8(SFLASH_SHREG0_REG);
	}
	else
	{
		if(u4Index==0)
		{
			_SendFlashCommand(0x2);
		}
		else
		{
			ASSERT(0);
		}

		if(_PollingReg(SFLASH_CMD_REG, 0x2) != 0)
		{
			return -1;
		}

		*pu1Val = SFLASH_RREG8(SFLASH_RDSR_REG);
	}
#else

    if(u4Index==0)
    {
        _SendFlashCommand(0x2);
    }
#ifdef USE_SECOND_FLASH
    else if(u4Index == 1)
    {
        _SendFlashCommand(0x42);
    }
#endif
    else
    {
        ASSERT(0);
    }

    if(_PollingReg(SFLASH_CMD_REG, 0x2) != 0)
    {
        return 1;
    }

    *pu1Val = SFLASH_RREG8(SFLASH_RDSR_REG);
#endif
    return 0;
}

//-----------------------------------------------------------------------------
/** Brief of SFLASHHW_SetIsr.
 *  Details of SFLASHHW_SetIsr (optional).
 *  @retval TRUE    Success
 *  @retval FALSE   Fail
 */
//-----------------------------------------------------------------------------
void SFLASHHW_SetIsr(BOOL fgSet)
{
    if (fgSet)
    {
        /* enable irq. */
        SFLASH_WREG32(SFLASH_SF_INTRSTUS_REG, 0x3F);
        SFLASH_WREG32(SFLASH_SF_INTREN_REG, 0x3F);
        VERIFY(BIM_ClearIrq(VECTOR_FLASH));
        VERIFY(BIM_EnableIrq(VECTOR_FLASH));
        _fgSFIsrEn = TRUE;
    }
    else
    {
        /* disable irq. */
        VERIFY(BIM_DisableIrq(VECTOR_FLASH));
        SFLASH_WREG32(SFLASH_SF_INTREN_REG, 0);
        SFLASH_WREG32(SFLASH_SF_INTRSTUS_REG, 0x3F);
        VERIFY(BIM_ClearIrq(VECTOR_FLASH));
        _fgSFIsrEn = FALSE;
    }
}
//-----------------------------------------------------------------------------
/** Brief of SFLASHHW_Ewsr.
 *  Details
 *  @retval TRUE    Success
 *  @retval FALSE   Fail
 */
//-----------------------------------------------------------------------------
INT32 SFLASHHW_Send2Byte(UINT32 u4Index, UINT8 u1Data1, UINT8 u1Data2)
{
    SFLASH_WREG8(SFLASH_PRGDATA5_REG, u1Data1);
    SFLASH_WREG8(SFLASH_PRGDATA4_REG, u1Data2);
    SFLASH_WREG8(SFLASH_CNT_REG,0x10); // Write SF Bit Count

#ifdef BD_NOR_ENABLE
    SFLASHHW_SwitchChip(u4Index);
#endif
    if(u4Index==0)
    {
        _SendFlashCommand(0x4); // Write PRG

    }
#ifdef USE_SECOND_FLASH
    else if(u4Index == 1)
    {
        _SendFlashCommand(0x44);    // Write PRG
    }
#endif
    else
    {
        ASSERT(0);
    }

    if( _PollingReg(SFLASH_CMD_REG, 0x04) != 0) // timeout
    {
#ifdef BD_NOR_ENABLE
			SFLASHHW_SwitchChip(0);
#endif

        return -1;
    }
#ifdef BD_NOR_ENABLE
		SFLASHHW_SwitchChip(0);
#endif

    return 0;

}

//-----------------------------------------------------------------------------
/** Brief of SFLASHHW_SendByte.
 *  Details
 *  @retval TRUE    Success
 *  @retval FALSE   Fail
 */
//-----------------------------------------------------------------------------
INT32 SFLASHHW_SendByte(UINT32 u4Index, UINT8 u1Data)
{
    SFLASH_WREG8(SFLASH_PRGDATA5_REG, u1Data);
    SFLASH_WREG8(SFLASH_CNT_REG,8); // Write SF Bit Count

#ifdef BD_NOR_ENABLE
		SFLASHHW_SwitchChip(u4Index);
#endif

    if(u4Index==0)
    {
        _SendFlashCommand(0x4); // Write PRG

    }
#ifdef USE_SECOND_FLASH
    else if(u4Index == 1)
    {
        _SendFlashCommand(0x44);    // Write PRG
    }
#endif
    else
    {
        ASSERT(0);
    }

    if( _PollingReg(SFLASH_CMD_REG, 0x04) != 0) // timeout
    {
#ifdef BD_NOR_ENABLE
			SFLASHHW_SwitchChip(0);
#endif

        return -1;
    }
#ifdef BD_NOR_ENABLE
		SFLASHHW_SwitchChip(0);
#endif

    return 0;

}


//-----------------------------------------------------------------------------
/** Brief of SFLASHHW_GetSstID.
 *  Details of SFLASHHW_Identify (optional).
 *  @retval TRUE    Success
 *  @retval FALSE   Fail
 */
//-----------------------------------------------------------------------------
INT32 SFLASHHW_GetSstID(UINT32 u4Index, UINT8 *pu1MenuID, UINT8 *pu1DevID1, UINT8 *pu1DevID2)
{
    SFLASH_WREG8(SFLASH_PRGDATA5_REG, 0x90);    // Write
    SFLASH_WREG8(SFLASH_PRGDATA4_REG, 0x00);    // Write
    SFLASH_WREG8(SFLASH_PRGDATA3_REG, 0x00);    // Write
    SFLASH_WREG8(SFLASH_PRGDATA2_REG, 0x00);    // Write
    SFLASH_WREG8(SFLASH_PRGDATA1_REG, 0x00);    // Write
    SFLASH_WREG8(SFLASH_PRGDATA0_REG, 0x00);    // Write
    SFLASH_WREG8(SFLASH_CNT_REG, 0x30);           // Write SF Bit Count

    if(u4Index == 0)
    {
        _SendFlashCommand(0x4);
    }
#ifdef USE_SECOND_FLASH
    else if(u4Index == 1)
    {
        _SendFlashCommand(0x44);
    }
#else
    else
    {
        ASSERT(0);
    }
#endif

    if( _PollingReg(SFLASH_CMD_REG, 0x04) != 0) // timeout
    {
        return -1;
    }

    if(pu1DevID2 != NULL)
    {
        *pu1DevID2 = 0x10;
    }

    if(pu1DevID1 != NULL)
    {
        *pu1DevID1 = SFLASH_RREG8(SFLASH_SHREG0_REG);
    }

    if(pu1MenuID != NULL)
    {
        *pu1MenuID = SFLASH_RREG8(SFLASH_SHREG1_REG);
    }

    _SendFlashCommand(0x0);
    return 0;
}


//-----------------------------------------------------------------------------
/** Brief of SFLASHHW_GetID.
 *  Details of SFLASHHW_Identify (optional).
 *  @retval TRUE    Success
 *  @retval FALSE   Fail
 */
//-----------------------------------------------------------------------------
INT32 SFLASHHW_GetID(UINT32 u4Index, UINT8 *pu1MenuID, UINT8 *pu1DevID1, UINT8 *pu1DevID2)
{

	LOG(0, "Try SFLASHHW_GetID\n");
    SFLASH_WREG8(SFLASH_PRGDATA5_REG, 0x9F);    // Write
    SFLASH_WREG8(SFLASH_PRGDATA4_REG, 0x00);    // Write
    SFLASH_WREG8(SFLASH_PRGDATA3_REG, 0x00);    // Write
    SFLASH_WREG8(SFLASH_PRGDATA2_REG, 0x00);    // Write
    SFLASH_WREG8(SFLASH_CNT_REG, 32);           // Write SF Bit Count

    if(u4Index == 0)
    {
        _SendFlashCommand(0x4);
    }
#ifdef USE_SECOND_FLASH
    else if(u4Index == 1)
    {
        _SendFlashCommand(0x44);
    }
#else
    else
    {
        ASSERT(0);
    }
#endif

    if( _PollingReg(SFLASH_CMD_REG, 0x04) !=0) // timeout
    {
        return 1;
    }
    if(pu1DevID2 != NULL)
    {
        *pu1DevID2 = SFLASH_RREG8(SFLASH_SHREG0_REG);
    }

    if(pu1DevID1 != NULL)
    {
        *pu1DevID1 = SFLASH_RREG8(SFLASH_SHREG1_REG);
    }

    if(pu1MenuID != NULL)
    {
        *pu1MenuID = SFLASH_RREG8(SFLASH_SHREG2_REG);
    }

    _SendFlashCommand(0x0);
    if((pu1MenuID != NULL) && (pu1DevID1 != NULL))
    {
    if((*pu1MenuID == 0xFF) || (*pu1DevID1 == 0x00))
    {
       LOG(7, "Try not JEDEC ID\n");
       if(SFLASHHW_GetSstID(u4Index, pu1MenuID, pu1DevID1, pu1DevID2) != 0)
      {
          return -1;
      }
    }
    }
    return 0;
}


//-----------------------------------------------------------------------------
/** Brief of SFLASHHW_GetFlashInfo.
 *  Details of SFLASHHW_GetInfo (optional).
 *  @retval TRUE    Success
 *  @retval FALSE   Fail
 */
//-----------------------------------------------------------------------------
void SFLASHHW_GetFlashInfo(SFLASH_INFO_T *prInfo)
{
    UINT32 i;

    if(prInfo == NULL)
    {
        return;
    }

    prInfo->u1FlashCount = (UINT8)(_u4ChipCount & 0xFF);
    for(i=0; i<_u4ChipCount; i++)
    {
        prInfo->arFlashInfo[i].u1MenuID = _arFlashChip[i].u1MenuID;
        prInfo->arFlashInfo[i].u1DevID1 = _arFlashChip[i].u1DevID1;
        prInfo->arFlashInfo[i].u1DevID2 = _arFlashChip[i].u1DevID2;
        prInfo->arFlashInfo[i].u4ChipSize = _arFlashChip[i].u4ChipSize;
        prInfo->arFlashInfo[i].u4SecSize = _arFlashChip[i].u4SecSize;
        prInfo->arFlashInfo[i].u4SecCount =
                                _arFlashChip[i].u4ChipSize / _arFlashChip[i].u4SecSize;

        //VERIFY(NULL != x_strcpy(prInfo->arFlashInfo[i].pcFlashStr,
                            //_arFlashChip[i].pcFlashStr));
        #if !defined(CC_MT5890) && !defined(CC_MT5882)
		x_memcpy(prInfo->arFlashInfo[i].pcFlashStr, _arFlashChip[i].pcFlashStr, SFLASHHWNAME_LEN);
		#endif // !defined(CC_MT5890) && !defined(CC_MT5882)
    }

}


//-----------------------------------------------------------------------------
/** Brief of SFLASHHW_Read.
 *  Details of SFLASHHW_Read (optional).
 *  @retval TRUE    Success
 *  @retval FALSE   Fail
 */
//-----------------------------------------------------------------------------
INT32 SFLASHHW_Read(UINT32 u4Addr, UINT32 u4Len, UINT8* pu1buf)
{
    UINT32 i;
#ifdef BD_NOR_ENABLE
	UINT8 _u1FlagVaule;
	HAL_TIME_T time1={0}, time2={0}, result_time={0};
	HAL_GetTime(&time1);
#endif
    if(pu1buf == NULL)
    {
        return 1;
    }
    SFLASH_WREG8(SFLASH_CFG2_REG, 0x0C);    // Disable pretch write buffer
#ifndef BD_NOR_ENABLE
#if (defined(ENABLE_DUALREAD))
    if (_u1DualReadCmd == 0x3B)
    {
        SFLASH_WREG8(SFLASH_PRGDATA3_REG, _u1DualReadCmd);
        SFLASH_WREG8(SFLASH_DUAL_REG, 0x1);
    }
    else if (_u1DualReadCmd == 0xBB)
    {
        SFLASH_WREG8(SFLASH_PRGDATA3_REG, _u1DualReadCmd);
        SFLASH_WREG8(SFLASH_DUAL_REG, 0x3);
    }
    else
    {
        SFLASH_WREG8(SFLASH_DUAL_REG, 0x0);
    }
#endif
#else
    SFLASH_WREG8(SFLASH_READ_DUAL_REG, SFLASH_RREG8(SFLASH_READ_DUAL_REG) & 0x10);//clear read mode setting
    _u1FlagVaule = IO_READ8(0xCA000000, 0);
    if(_u1FlagVaule == 1)
    {
        LOG(0, "...cdd SFLASHHW_Read _u1DualReadCmd is 0x3B u4Len = 0x%x!\n",u4Len);
        SFLASH_WREG8(SFLASH_PRGDATA3_REG, 0x3b);
        SFLASH_WREG8(SFLASH_READ_DUAL_REG, SFLASH_RREG8(SFLASH_READ_DUAL_REG) | 0x1);
    }
    else if(_u1FlagVaule == 2)
    {
        LOG(0, "...cdd SFLASHHW_Read _u1DualReadCmd is 0xBB u4Len = 0x%x!\n",u4Len);
        SFLASH_WREG8(SFLASH_PRGDATA3_REG, 0xbb);
        SFLASH_WREG8(SFLASH_READ_DUAL_REG, SFLASH_RREG8(SFLASH_READ_DUAL_REG) | 0x3);
    }
    else if(_u1FlagVaule == 3)
    {
        _SetFlashEnter4BitRead();
        LOG(0, "...cdd SFLASHHW_Read _u1QuadReadCmd is 0x6B u4Len = 0x%x!\n",u4Len);
        SFLASH_WREG8(SFLASH_PRGDATA4_REG, 0x6B);
        SFLASH_WREG8(SFLASH_READ_DUAL_REG, SFLASH_RREG8(SFLASH_READ_DUAL_REG) | 0x4);
    }
    else if(_u1FlagVaule == 4)
    {
        _SetFlashEnter4BitRead();
        LOG(0, "...cdd SFLASHHW_Read _u1QuadReadCmd is 0xEB u4Len = 0x%x!\n",u4Len);
        SFLASH_WREG8(SFLASH_PRGDATA4_REG, 0xEB);
        SFLASH_WREG8(SFLASH_READ_DUAL_REG, SFLASH_RREG8(SFLASH_READ_DUAL_REG) | 0xc);
    }
    else
    {
        //LOG(0, "...cdd SFLASHHW_Read NULL _u1DualReadCmd!\n");
        SFLASH_WREG8(SFLASH_READ_DUAL_REG, SFLASH_RREG8(SFLASH_READ_DUAL_REG) | 0x0);
    }
#endif

    SFLASH_WREG8(SFLASH_RADR2_REG, LoByte(HiWord(u4Addr))); // Set Addr
    SFLASH_WREG8(SFLASH_RADR1_REG, HiByte(LoWord(u4Addr)));
    SFLASH_WREG8(SFLASH_RADR0_REG, LoByte(LoWord(u4Addr)));

    for (i=0; i<u4Len; i++)
    {
#ifdef USE_SECOND_FLASH
		if(u4Addr >= _arFlashChip[0].u4ChipSize)
		{
        	_SendFlashCommand(0xC1);    // Read and autoinc address, use second flash
		}
		else
		{
			_SendFlashCommand(0x81);  // Read and autoinc address
		}
#else
        _SendFlashCommand(0x81);  // Read and autoinc address
#endif

        if( _PollingReg(SFLASH_CMD_REG, 0x01) !=0) // timeout
        {
            LOG(0, "...cdd SFLASHHW_Read timeout !\n");
            return 1;
        }

		#ifdef ENABLE_CONTROLLER_STATUS
		printf("Serial flash control starus:0x87b4=0x%08x!\n",SFLASH_RREG32(SFLASH_SAMPLE_EDGE_REG));
		#endif
        pu1buf[i] = SFLASH_RREG8(SFLASH_RDATA_REG); // Get data
    }

#ifdef BD_NOR_ENABLE
	HAL_GetTime(&time2);
    HAL_GetDeltaTime(&result_time, &time1, &time2);
    LOG(0, "read 0x%XBytes elapse time: %d sec, %d usec\n", u4Len,
                                    result_time.u4Seconds, result_time.u4Micros);

	SFLASH_WREG8(SFLASH_READ_DUAL_REG, SFLASH_RREG8(SFLASH_READ_DUAL_REG) & 0x10);//clear read mode setting
#else
    _SendFlashCommand(0x0);
#endif
    return 0;
}


#ifdef BD_NOR_ENABLE
//-----------------------------------------------------------------------------
/** Brief of SFLASHHW_4BitRead.
 *  Details of SFLASHHW_4BitRead (optional) -----support 4 bit mode.
 *  @retval TRUE    Success
 *  @retval FALSE   Fail
 */
//-----------------------------------------------------------------------------
INT32 SFLASHHW_4BitRead(UINT32 u4Addr, UINT32 u4Len, UINT8* pu1buf)
{
    UINT32 i;
	UINT8 _u1FlagVaule;
	HAL_TIME_T time1={0}, time2={0}, result_time={0};
	HAL_GetTime(&time1);

    if(pu1buf == NULL)
    {
        return 1;
    }
    SFLASH_WREG8(SFLASH_CFG2_REG, 0x0C);    // Disable pretch write buffer
#if 0
#if (defined(ENABLE_DUALREAD))
			if (_u1DualReadCmd == 0x3B)
			{
				LOG(0, "SFLASHHW_Read _u1DualReadCmd is 0x3B!\n");
				SFLASH_WREG8(SFLASH_PRGDATA3_REG, _u1DualReadCmd);
				SFLASH_WREG8(SFLASH_READ_DUAL_REG, 0x1);
			}
			else if (_u1DualReadCmd == 0xBB)
			{
				LOG(0, "SFLASHHW_Read _u1DualReadCmd is 0xBB!\n");
				SFLASH_WREG8(SFLASH_PRGDATA3_REG, _u1DualReadCmd);
				SFLASH_WREG8(SFLASH_READ_DUAL_REG, 0x3);
			}
			else
			{
				LOG(0, "SFLASHHW_Read NULL _u1DualReadCmd!\n");
				SFLASH_WREG8(SFLASH_READ_DUAL_REG, 0x0);
			}
#endif
#else
        SFLASH_WREG8(SFLASH_READ_DUAL_REG, SFLASH_RREG8(SFLASH_READ_DUAL_REG) & 0x10);//clear read mode setting
		_u1FlagVaule = IO_READ8(0xCA000000, 0);
		if(_u1FlagVaule == 1)
		{
			LOG(0, "...cdd SFLASHHW_Read _u1DualReadCmd is 0x3B u4Len = 0x%x!\n",u4Len);
			SFLASH_WREG8(SFLASH_PRGDATA3_REG, 0x3b);
			SFLASH_WREG8(SFLASH_READ_DUAL_REG, SFLASH_RREG8(SFLASH_READ_DUAL_REG) | 0x1);
		}
		else if(_u1FlagVaule == 2)
		{
			LOG(0, "...cdd SFLASHHW_Read _u1DualReadCmd is 0xBB u4Len = 0x%x!\n",u4Len);
			SFLASH_WREG8(SFLASH_PRGDATA3_REG, 0xbb);
			SFLASH_WREG8(SFLASH_READ_DUAL_REG, SFLASH_RREG8(SFLASH_READ_DUAL_REG) | 0x3);
		}
		else if(_u1FlagVaule == 3)
		{
			_SetFlashEnter4BitRead();
			LOG(0, "...cdd SFLASHHW_Read _u1QuadReadCmd is 0x6B u4Len = 0x%x!\n",u4Len);
			SFLASH_WREG8(SFLASH_PRGDATA4_REG, 0x6B);
			SFLASH_WREG8(SFLASH_READ_DUAL_REG, SFLASH_RREG8(SFLASH_READ_DUAL_REG) | 0x4);
		}
		else if(_u1FlagVaule == 4)
		{
			_SetFlashEnter4BitRead();
			LOG(0, "...cdd SFLASHHW_Read _u1QuadReadCmd is 0xEB u4Len = 0x%x!\n",u4Len);
			SFLASH_WREG8(SFLASH_PRGDATA4_REG, 0xEB);
			SFLASH_WREG8(SFLASH_READ_DUAL_REG, SFLASH_RREG8(SFLASH_READ_DUAL_REG) | 0xc);
		}
		else
		{
			//LOG(0, "...cdd SFLASHHW_Read NULL _u1DualReadCmd!\n");
			SFLASH_WREG8(SFLASH_READ_DUAL_REG, SFLASH_RREG8(SFLASH_READ_DUAL_REG) | 0x0);
		}
#endif

    SFLASH_WREG8(SFLASH_RADR3_REG, HiByte(HiWord(u4Addr))); // Write
    SFLASH_WREG8(SFLASH_RADR2_REG, LoByte(HiWord(u4Addr))); // Set Addr
    SFLASH_WREG8(SFLASH_RADR1_REG, HiByte(LoWord(u4Addr)));
    SFLASH_WREG8(SFLASH_RADR0_REG, LoByte(LoWord(u4Addr)));
	LOG(0, "...cdd SFLASHHW_4BitRead u4Addr = 0x%x!\n", u4Addr);

    for (i=0; i<u4Len; i++)
    {
        _SendFlashCommand(0x81);  // Read and auto inc address

        if( _PollingReg(SFLASH_CMD_REG, 0x01) == 1) // timeout
        {
            return 1;
        }

        pu1buf[i] = SFLASH_RREG8(SFLASH_RDATA_REG); // Get data
		if(( i==0 )||(i == 5))
		LOG(0, "...cdd SFLASHHW_4BitRead pu1buf[%d] = 0x%x\n",i,pu1buf[i]);
    }
	HAL_GetTime(&time2);
    HAL_GetDeltaTime(&result_time, &time1, &time2);
    LOG(0, "read 0x%XBytes elapse time: %d sec, %d usec\n", u4Len,
                                    result_time.u4Seconds, result_time.u4Micros);

    SFLASH_WREG8(SFLASH_READ_DUAL_REG, SFLASH_RREG8(SFLASH_READ_DUAL_REG) & 0x10);//clear read mode setting
    return 0;
}


//-----------------------------------------------------------------------------
/** Brief of SFLASHHW_4BBRead.
 *  Details of SFLASHHW_4BBRead (optional) -----support 4 byte mode and 4 bit mode.
 *  @retval TRUE    Success
 *  @retval FALSE   Fail
 */
//-----------------------------------------------------------------------------
INT32 SFLASHHW_4Byte4BitRead(UINT32 u4Addr, UINT32 u4Len, UINT8* pu1buf)
{
    UINT32 i;
	UINT8 _u1FlagVaule;
	HAL_TIME_T time1={0}, time2={0}, result_time={0};
	HAL_GetTime(&time1);

    if(pu1buf == NULL)
    {
        return 1;
    }
    SFLASH_WREG8(SFLASH_CFG2_REG, 0x0C);    // Disable pretch write buffer
#if 0
#if (defined(ENABLE_DUALREAD))
			if (_u1DualReadCmd == 0x3B)
			{
				LOG(0, "SFLASHHW_Read _u1DualReadCmd is 0x3B!\n");
				SFLASH_WREG8(SFLASH_PRGDATA3_REG, _u1DualReadCmd);
				SFLASH_WREG8(SFLASH_READ_DUAL_REG, 0x1);
			}
			else if (_u1DualReadCmd == 0xBB)
			{
				LOG(0, "SFLASHHW_Read _u1DualReadCmd is 0xBB!\n");
				SFLASH_WREG8(SFLASH_PRGDATA3_REG, _u1DualReadCmd);
				SFLASH_WREG8(SFLASH_READ_DUAL_REG, 0x3);
			}
			else
			{
				LOG(0, "SFLASHHW_Read NULL _u1DualReadCmd!\n");
				SFLASH_WREG8(SFLASH_READ_DUAL_REG, 0x0);
			}
#endif
#else
        SFLASH_WREG8(SFLASH_READ_DUAL_REG, SFLASH_RREG8(SFLASH_READ_DUAL_REG) & 0x10);//clear read mode setting
		_u1FlagVaule = IO_READ8(0xCA000000, 0);
		if(_u1FlagVaule == 1)
		{
			LOG(0, "...cdd SFLASHHW_Read _u1DualReadCmd is 0x3B u4Len = 0x%x!\n",u4Len);
			SFLASH_WREG8(SFLASH_PRGDATA3_REG, 0x3b);
			SFLASH_WREG8(SFLASH_READ_DUAL_REG, SFLASH_RREG8(SFLASH_READ_DUAL_REG) | 0x1);
		}
		else if(_u1FlagVaule == 2)
		{
			LOG(0, "...cdd SFLASHHW_Read _u1DualReadCmd is 0xBB u4Len = 0x%x!\n",u4Len);
			SFLASH_WREG8(SFLASH_PRGDATA3_REG, 0xbb);
			SFLASH_WREG8(SFLASH_READ_DUAL_REG, SFLASH_RREG8(SFLASH_READ_DUAL_REG) | 0x3);
		}
		else if(_u1FlagVaule == 3)
		{
			_SetFlashEnter4BitRead();
			LOG(0, "...cdd SFLASHHW_Read _u1QuadReadCmd is 0x6B u4Len = 0x%x!\n",u4Len);
			SFLASH_WREG8(SFLASH_PRGDATA4_REG, 0x6B);
			SFLASH_WREG8(SFLASH_READ_DUAL_REG, SFLASH_RREG8(SFLASH_READ_DUAL_REG) | 0x4);
		}
		else if(_u1FlagVaule == 4)
		{
			_SetFlashEnter4BitRead();
			LOG(0, "...cdd SFLASHHW_Read _u1QuadReadCmd is 0xEB u4Len = 0x%x!\n",u4Len);
			SFLASH_WREG8(SFLASH_PRGDATA4_REG, 0xEB);
			SFLASH_WREG8(SFLASH_READ_DUAL_REG, SFLASH_RREG8(SFLASH_READ_DUAL_REG) | 0xc);
		}
		else
		{
			//LOG(0, "...cdd SFLASHHW_Read NULL _u1DualReadCmd!\n");
			SFLASH_WREG8(SFLASH_READ_DUAL_REG, SFLASH_RREG8(SFLASH_READ_DUAL_REG) | 0x0);
		}
#endif

	if(_SetFlashEnter4Byte()!=0)
		return -1;

    SFLASH_WREG8(SFLASH_RADR3_REG, HiByte(HiWord(u4Addr))); // Write
    SFLASH_WREG8(SFLASH_RADR2_REG, LoByte(HiWord(u4Addr))); // Set Addr
    SFLASH_WREG8(SFLASH_RADR1_REG, HiByte(LoWord(u4Addr)));
    SFLASH_WREG8(SFLASH_RADR0_REG, LoByte(LoWord(u4Addr)));
	LOG(0, "...cdd SFLASHHW_4Byte4BitRead u4Addr = 0x%x!\n", u4Addr);

    for (i=0; i<u4Len; i++)
    {
        _SendFlashCommand(0x81);  // Read and auto inc address

        if( _PollingReg(SFLASH_CMD_REG, 0x01) == 1) // timeout
        {
            return 1;
        }

        pu1buf[i] = SFLASH_RREG8(SFLASH_RDATA_REG); // Get data
		if(( i==0 )||(i == 5))
		LOG(0, "...cdd SFLASHHW_4Byte4BitRead pu1buf[%d] = 0x%x\n",i,pu1buf[i]);
    }
	HAL_GetTime(&time2);
    HAL_GetDeltaTime(&result_time, &time1, &time2);
    LOG(0, "read 0x%XBytes elapse time: %d sec, %d usec\n", u4Len,
                                    result_time.u4Seconds, result_time.u4Micros);
    SFLASH_WREG8(SFLASH_READ_DUAL_REG, SFLASH_RREG8(SFLASH_READ_DUAL_REG) & 0x10);//exit read mode setting
    return 0;
}

#else
//-----------------------------------------------------------------------------
/** Brief of SFLASHHW_ReadProgram.
 *  Details of SFLASHHW_ReadProgram (optional).
 *  @retval TRUE    Success
 *  @retval FALSE   Fail
 */
//-----------------------------------------------------------------------------
INT32 SFLASHHW_ReadProgram(UINT32 u4Addr, UINT32 u4Len, UINT8* pu1buf)
{
    UINT32 i;
	UINT32 u4Index = 0;
    if(pu1buf == NULL)
    {
        return -1;
    }
    SFLASH_WREG8(SFLASH_CFG2_REG, 0x0C);    // Disable pretch write buffer

	if(u4Len == 2)
	{
#ifdef USE_FASTREAD
		SFLASH_WREG8(SFLASH_PRGDATA5_REG, _arFlashChip[u4Index].u1FASTREADCmd);
#else
		SFLASH_WREG8(SFLASH_PRGDATA5_REG, _arFlashChip[u4Index].u1READCmd);
#endif
		SFLASH_WREG8(SFLASH_PRGDATA4_REG, LoByte(HiWord(u4Addr)));
		SFLASH_WREG8(SFLASH_PRGDATA3_REG, HiByte(LoWord(u4Addr)));
		SFLASH_WREG8(SFLASH_PRGDATA2_REG, LoByte(LoWord(u4Addr)));
		SFLASH_WREG8(SFLASH_PRGDATA1_REG, 0x00);
		SFLASH_WREG8(SFLASH_PRGDATA0_REG, 0x00);
#ifdef USE_FASTREAD
		SFLASH_WREG8(SFLASH_PRGDATA6_REG, 0x00);
		SFLASH_WREG8(SFLASH_CNT_REG, 56);           // Write SF Bit Count
#else
		SFLASH_WREG8(SFLASH_CNT_REG, 48);           // Write SF Bit Count
#endif
	    if(u4Index == 0)
	    {
	        _SendFlashCommand(0x4);
	    }
#ifdef USE_SECOND_FLASH
	    else if(u4Index == 1)
	    {
	        _SendFlashCommand(0x44);
	    }
#else
	    else
	    {
	        ASSERT(0);
	    }
#endif
	    if( _PollingReg(SFLASH_CMD_REG, 0x04) != 0) // timeout
	    {
	        return -1;
	    }

		i = 0;
        pu1buf[i] = SFLASH_RREG8(SFLASH_SHREG1_REG); // Get data
        i++;
        pu1buf[i] = SFLASH_RREG8(SFLASH_SHREG0_REG); // Get data
		for(i = 0; i < u4Len; i++)
		{
			printf("pu1buf[%d] = 0x%x \n",i, pu1buf[i]);
		}
	    _SendFlashCommand(0x0);

	}

	else if(u4Len == 3)
	{
#ifdef USE_FASTREAD
		SFLASH_WREG8(SFLASH_PRGDATA5_REG, _arFlashChip[u4Index].u1FASTREADCmd);
#else
		SFLASH_WREG8(SFLASH_PRGDATA5_REG, _arFlashChip[u4Index].u1READCmd);
#endif
		SFLASH_WREG8(SFLASH_PRGDATA4_REG, LoByte(HiWord(u4Addr)));
		SFLASH_WREG8(SFLASH_PRGDATA3_REG, HiByte(LoWord(u4Addr)));
		SFLASH_WREG8(SFLASH_PRGDATA2_REG, LoByte(LoWord(u4Addr)));
		SFLASH_WREG8(SFLASH_PRGDATA1_REG, 0x00);
		SFLASH_WREG8(SFLASH_PRGDATA0_REG, 0x00);
		SFLASH_WREG8(SFLASH_PRGDATA6_REG, 0x00);
#ifdef USE_FASTREAD
		SFLASH_WREG8(SFLASH_PRGDATA7_REG, 0x00);
		SFLASH_WREG8(SFLASH_CNT_REG, 64);			// Write SF Bit Count
#else
		SFLASH_WREG8(SFLASH_CNT_REG, 56);			// Write SF Bit Count
#endif
		if(u4Index == 0)
		{
			_SendFlashCommand(0x4);
		}
		else
		{
			ASSERT(0);
		}
		if( _PollingReg(SFLASH_CMD_REG, 0x04) != 0) // timeout
		{
			return -1;
		}
		i = 0;
		pu1buf[i] = SFLASH_RREG8(SFLASH_SHREG2_REG); // Get data
		i++;
		pu1buf[i] = SFLASH_RREG8(SFLASH_SHREG1_REG); // Get data
		i++;
		pu1buf[i] = SFLASH_RREG8(SFLASH_SHREG0_REG); // Get data
		for(i = 0; i < u4Len; i++)
		{
			printf("pu1buf[%d] = 0x%x \n",i, pu1buf[i]);
		}
		_SendFlashCommand(0x0);

	}
	else if(u4Len == 4)
	{
#ifdef USE_FASTREAD
		SFLASH_WREG8(SFLASH_PRGDATA5_REG, _arFlashChip[u4Index].u1FASTREADCmd);
#else
		SFLASH_WREG8(SFLASH_PRGDATA5_REG, _arFlashChip[u4Index].u1READCmd);
#endif
		SFLASH_WREG8(SFLASH_PRGDATA4_REG, LoByte(HiWord(u4Addr)));
		SFLASH_WREG8(SFLASH_PRGDATA3_REG, HiByte(LoWord(u4Addr)));
		SFLASH_WREG8(SFLASH_PRGDATA2_REG, LoByte(LoWord(u4Addr)));
		SFLASH_WREG8(SFLASH_PRGDATA1_REG, 0x00);
		SFLASH_WREG8(SFLASH_PRGDATA0_REG, 0x00);
		SFLASH_WREG8(SFLASH_PRGDATA6_REG, 0x00);
		SFLASH_WREG8(SFLASH_PRGDATA7_REG, 0x00);
#ifdef USE_FASTREAD
		SFLASH_WREG8(SFLASH_CNT_REG, 72);			// Write SF Bit Count
#else
		SFLASH_WREG8(SFLASH_CNT_REG, 64);			// Write SF Bit Count
#endif
		if(u4Index == 0)
		{
			_SendFlashCommand(0x4);
		}
		else
		{
			ASSERT(0);
		}

		if( _PollingReg(SFLASH_CMD_REG, 0x04) != 0) // timeout
		{
			return -1;
		}
		i = 0;
		pu1buf[i] = SFLASH_RREG8(SFLASH_SHREG3_REG); // Get data
		i++;
		pu1buf[i] = SFLASH_RREG8(SFLASH_SHREG2_REG); // Get data
		i++;
		pu1buf[i] = SFLASH_RREG8(SFLASH_SHREG1_REG); // Get data
		i++;
		pu1buf[i] = SFLASH_RREG8(SFLASH_SHREG0_REG); // Get data
		for(i = 0; i < u4Len; i++)
		{
			printf("pu1buf[%d] = 0x%x \n",i, pu1buf[i]);
		}
		_SendFlashCommand(0x0);
	}
	else
	{
		printf("the u4Len = 0x%x,it is not 2, 3 or 4\n",u4Len);
		return -1;
	}
    return 0;
}

//-----------------------------------------------------------------------------
/** Brief of SFLASHHW_ReadRISCProgram.
 *  Details of SFLASHHW_ReadRISCProgram (optional).
 *  @retval TRUE    Success
 *  @retval FALSE   Fail
 */
//-----------------------------------------------------------------------------
INT32 SFLASHHW_ReadRISCProgram(UINT32 u4Addr, UINT32 u4Len, UINT8* pu1buf)
	{
		UINT32 i, u4BufIdx, u4Data;
		UINT32 u4Index = 0;
		if(pu1buf == NULL)
		{
			return -1;
		}
		SFLASH_WREG8(SFLASH_CFG2_REG, 0x0C);	// Disable pretch write buffer


		if(u4Len == 2)
		{
#ifdef USE_FASTREAD
			u4Data = (((UINT32)_arFlashChip[u4Index].u1FASTREADCmd) << 24)|u4Addr;
#else
			u4Data = (((UINT32)_arFlashChip[u4Index].u1READCmd) << 24)|u4Addr;
#endif

			printf("the u4Data = 0x%x,u4Addr = 0x%x, _arFlashChip[u4Index].u1PAGEPROGRAMCmd = 0x%x\n", u4Data, u4Addr, _arFlashChip[u4Index].u1PAGEPROGRAMCmd);
			SFLASH_WREG32(SFLASH_PRGDATA_RISC1, u4Data);
			SFLASH_WREG32(SFLASH_PRGDATA_RISC0, 0x00);
#ifdef USE_FASTREAD
			SFLASH_WREG8(SFLASH_CNT_REG, 56);			// Write SF Bit Count
#else
			SFLASH_WREG8(SFLASH_CNT_REG, 48);			// Write SF Bit Count
#endif
			if(u4Index == 0)
			{
				_SendFlashCommand(0x4);
			}
#ifdef USE_SECOND_FLASH
			else if(u4Index == 1)
			{
				_SendFlashCommand(0x44);
			}
#else
			else
			{
				ASSERT(0);
			}
#endif
			if( _PollingReg(SFLASH_CMD_REG, 0x04) != 0) // timeout
			{
				return -1;
			}
			u4Data = SFLASH_RREG32(SFLASH_SHREG_RISC0);
			u4BufIdx = 0;
			for(i=0; i<u4Len; i++)
			{
				pu1buf[u4BufIdx] = *((UINT8 *)&u4Data + (u4Len - i -1));
				printf("the u4Data = 0x%x,pu1buf[%d] = 0x%x\n", u4Data, u4BufIdx, pu1buf[u4BufIdx]);
				 u4BufIdx++;
			}
			_SendFlashCommand(0x0);
		}
		else if(u4Len == 3)
		{
#ifdef USE_FASTREAD
			u4Data = (((UINT32)_arFlashChip[u4Index].u1FASTREADCmd) << 24)|u4Addr;
#else
			u4Data = (((UINT32)_arFlashChip[u4Index].u1READCmd) << 24)|u4Addr;
#endif

			printf("the u4Data = 0x%x,u4Addr = 0x%x, _arFlashChip[u4Index].u1READCmd = 0x%x\n", u4Data, u4Addr, _arFlashChip[u4Index].u1PAGEPROGRAMCmd);
			SFLASH_WREG32(SFLASH_PRGDATA_RISC1, u4Data);

			SFLASH_WREG32(SFLASH_PRGDATA_RISC0, 0x00);
#ifdef USE_FASTREAD
			SFLASH_WREG8(SFLASH_CNT_REG, 64);			// Write SF Bit Count
#else
			SFLASH_WREG8(SFLASH_CNT_REG, 56);			// Write SF Bit Count
#endif
			if(u4Index == 0)
			{
				_SendFlashCommand(0x4);
			}
			else
			{
				ASSERT(0);
			}
			if( _PollingReg(SFLASH_CMD_REG, 0x04) != 0) // timeout
			{
				return -1;
			}
			u4Data = SFLASH_RREG32(SFLASH_SHREG_RISC0);
			u4BufIdx = 0;
			for(i=0; i<u4Len; i++)
			{
				pu1buf[u4BufIdx] = *((UINT8 *)&u4Data + (u4Len - i -1));
				printf("the u4Data = 0x%x,pu1buf[%d] = 0x%x\n", u4Data, u4BufIdx, pu1buf[u4BufIdx]);
				 u4BufIdx++;
			}
			_SendFlashCommand(0x0);
		}
		else if(u4Len == 4)
		{
#ifdef USE_FASTREAD
			u4Data = (((UINT32)_arFlashChip[u4Index].u1FASTREADCmd) << 24)|u4Addr;
#else
			u4Data = (((UINT32)_arFlashChip[u4Index].u1READCmd) << 24)|u4Addr;
#endif
			//printf("the u4Data = 0x%x,u4Addr = 0x%x, _arFlashChip[u4Index].u1READCmd = 0x%x\n", u4Data, u4Addr, _arFlashChip[u4Index].u1PAGEPROGRAMCmd);
			SFLASH_WREG32(SFLASH_PRGDATA_RISC1, u4Data);
			SFLASH_WREG32(SFLASH_PRGDATA_RISC0, 0x00);
#ifdef USE_FASTREAD
			SFLASH_WREG8(SFLASH_CNT_REG, 72);			// Write SF Bit Count
#else
			SFLASH_WREG8(SFLASH_CNT_REG, 64);			// Write SF Bit Count
#endif
			if(u4Index == 0)
			{
				_SendFlashCommand(0x4);
			}
			else
			{
				ASSERT(0);
			}
			if( _PollingReg(SFLASH_CMD_REG, 0x04) != 0) // timeout
			{
				return -1;
			}
			u4Data = SFLASH_RREG32(SFLASH_SHREG_RISC0);
			u4BufIdx = 0;
			for(i=0; i<u4Len; i++)
			{
				pu1buf[u4BufIdx] = *((UINT8 *)&u4Data + (u4Len - i -1));
				printf("the u4Data = 0x%x,pu1buf[%d] = 0x%x\n", u4Data, u4BufIdx, pu1buf[u4BufIdx]);
				 u4BufIdx++;
			}
			_SendFlashCommand(0x0);
		}
		else
		{
			printf("the u4Len = 0x%x,it is not 2, 3 or 4\n",u4Len);
			return -1;
		}
		return 0;
	}



#endif
//-----------------------------------------------------------------------------
static INT32 _ExecuteWriteCmd(UINT32 u4Index)
{
    HAL_TIME_T rOrgTime = {0}, rNewTime = {0}, rDeltaTime = {0};
    UINT8 u1Reg;

    if(_fgSFIsrEn)
    {
        _u4SFIsrStatus = 0;

    #ifdef BD_NOR_ENABLE
		if (_fgAAIWrite == FALSE)
        {
          SFLASH_WREG32(SFLASH_SF_INTRSTUS_REG, 0x10);    // Clear interrupt
          SFLASH_WREG32(SFLASH_SF_INTREN_REG, 0x10);
		}
		else
	    {
          SFLASH_WREG32(SFLASH_SF_INTRSTUS_REG, 0x40);    // Clear interrupt
          SFLASH_WREG32(SFLASH_SF_INTREN_REG, 0x40);
		}
	#else
		{
			SFLASH_WREG32(SFLASH_SF_INTRSTUS_REG, 0x10);    // Clear interrupt
             SFLASH_WREG32(SFLASH_SF_INTREN_REG, 0x10);
		}
	#endif
    }
#ifdef BD_NOR_ENABLE
		SFLASHHW_SwitchChip(u4Index);
#endif

    if(u4Index == 0)
    {
        if (_fgAAIWrite == FALSE)
        {
           _SendFlashCommand(0x10);
        }
        else
        {
        	#ifdef BD_NOR_ENABLE
            SFLASH_WREG8(SFLASH_CMD2_REG, 0x01);
            #else
            SFLASH_WREG8(SFLASH_SAMPLE_EDGE_REG, SFLASH_RREG8(SFLASH_SAMPLE_EDGE_REG)|0x80);
			#endif
        }
    }
#ifdef USE_SECOND_FLASH
    else if(u4Index == 1)
    {
        if (_fgAAIWrite == FALSE)
        {
            _SendFlashCommand(0x50);
        }
        else
        {
			#ifdef BD_NOR_ENABLE
            SFLASH_WREG8(SFLASH_CMD2_REG, 0x01);
			#else
			_SendFlashCommand(0x40);
            SFLASH_WREG8(SFLASH_SAMPLE_EDGE_REG, SFLASH_RREG8(SFLASH_SAMPLE_EDGE_REG)|0x80);
			#endif

        }
    }
#endif
    else
    {
        ASSERT(0);
    }

    //-------------------------
#ifdef BD_NOR_ENABLE
    if(_fgSFIsrEn)
    {
        while(((_fgAAIWrite == FALSE)&&(!(_u4SFIsrStatus & 0x10)))
			  || ((_fgAAIWrite == TRUE)&&(!(_u4SFIsrStatus & 0x40))))
        {
#ifndef CC_5381_LOADER
            if(x_sema_lock_timeout(_hSFLASHSema, SFLASH_WRITECMD_TIMEOUT) == OSR_TIMEOUT)
            {
				SFLASHHW_SwitchChip(0);

                return 1;
            }
#endif /* CC_5381_LOADER */
        }
    }
#else //end #ifdef BD_NOR_ENABLE
	    if(_fgSFIsrEn)
    {
        while(!(_u4SFIsrStatus & 0x10))
        {
#if !defined (CC_MTK_LOADER)&& !defined(CC_MTK_LOADER)
            if(x_sema_lock_timeout(_hSFLASHSema, SFLASH_WRITECMD_TIMEOUT) == OSR_TIMEOUT)
            {
                return -1;
            }
#endif /* CC_MTK_LOADER */
        }
    }
#endif
    else
    {
        HAL_GetTime(&rOrgTime);
        while(1)
        {
            if (_fgAAIWrite == FALSE)
            {
               u1Reg = SFLASH_RREG8(SFLASH_CMD_REG);
               if (0x0 == (u1Reg & 0x10))
               {
                  break;
               }
            }
            else
            {
            #ifdef BD_NOR_ENABLE
                u1Reg = SFLASH_RREG8(SFLASH_CMD2_REG);
                if (0x0 == (u1Reg & 0x01))
                {
                    break;
                }
			#else
			    u1Reg = SFLASH_RREG8(SFLASH_SAMPLE_EDGE_REG);
                if (0x0 == (u1Reg & 0x80))
                {
                    break;
                }
			#endif
            }

            HAL_GetTime(&rNewTime);
            HAL_GetDeltaTime(&rDeltaTime, &rOrgTime, &rNewTime);
            if (rDeltaTime.u4Seconds > SFLASH_WRITECMD_TIMEOUT)
            {
#ifdef BD_NOR_ENABLE
					SFLASHHW_SwitchChip(0);
#endif

                return 1;
            }
            //x_thread_delay(10);
        }
    }

    if(_fgSFIsrEn)
    {
        SFLASH_WREG32(SFLASH_SF_INTREN_REG, 0x0);   // Disable interrupt
    }

    _SendFlashCommand(0x0);
#ifdef BD_NOR_ENABLE
		SFLASHHW_SwitchChip(0);
#endif

    return 0;
}

//-----------------------------------------------------------------------------
static INT32 _WriteBuffer(UINT32 u4Index, UINT32 u4Addr, UINT32 u4Len, const UINT8* pu1Buf)
{
    UINT32 i, j, u4BufIdx, u4Data;

    if(pu1Buf == NULL)
    {
        return 1;
    }

    ASSERT(u4Len <= SFLASH_WRBUF_SIZE);
    ASSERT((u4Addr%SFLASH_HW_ALIGNMENT) == 0);
    ASSERT((u4Len%SFLASH_HW_ALIGNMENT) == 0);


    SFLASH_WREG8(SFLASH_RADR2_REG, LoByte(HiWord(u4Addr))); // Write
    SFLASH_WREG8(SFLASH_RADR1_REG, HiByte(LoWord(u4Addr))); // Write
    SFLASH_WREG8(SFLASH_RADR0_REG, LoByte(LoWord(u4Addr))); // Write

    u4BufIdx = 0;
    for(i=0; i<u4Len; i+=4)
    {
        for(j=0; j<4; j++)
        {
            (*((UINT8 *)&u4Data + j)) = pu1Buf[u4BufIdx];
            u4BufIdx++;
        }
        SFLASH_WREG32(SFLASH_PP_DATA_REG, u4Data);
     }

    if(_ExecuteWriteCmd(u4Index) != 0)
    {
        return 1;
    }

    //--------------------------------------------------------------
	if(_WaitForWriteBusy(u4Index, SFLASH_WRITEBUSY_TIMEOUT) != 0)
	{
	    return 1;
	}


    return 0;
}


//-----------------------------------------------------------------------------
static INT32 _WriteSingleByte(UINT32 u4Index, UINT32 u4Addr, UINT8 u1Data)
{

    SFLASH_WREG8(SFLASH_RADR2_REG, LoByte(HiWord(u4Addr)));
    SFLASH_WREG8(SFLASH_RADR1_REG, HiByte(LoWord(u4Addr)));
    SFLASH_WREG8(SFLASH_RADR0_REG, LoByte(LoWord(u4Addr)));

    SFLASH_WREG8(SFLASH_WDATA_REG, u1Data);

    if(_ExecuteWriteCmd(u4Index) != 0)
    {
        return 1;
    }

    //--------------------------------------------------------------
    if(_WaitForWriteBusy(u4Index, SFLASH_WRITEBUSY_TIMEOUT) != 0)
    {
        return 1;
    }

    return 0;
}

#ifndef BD_NOR_ENABLE

//-----------------------------------------------------------------------------
/*
u1Pagecount Total page in program buffer
u1Pagesize  page size,count in 4Bytes, max 255*4

*/
static INT32 _WritePageProgram(UINT32 u4Index, UINT8 u1Pagecount, UINT8 u1Pagesize, UINT32 u4Addr, UINT32 u4Len, const UINT8* pu1Buf)
{
    UINT32 u4MemPtr, u4MemSrc;
    UINT8 u1Reg;
    if(pu1Buf == NULL)
    {
        return 1;
    }

    ASSERT(u4Len <= PP_SIZE*PAGE_COUNT);
    ASSERT((u4Addr%SFLASH_HW_ALIGNMENT) == 0);
    ASSERT((u4Len%SFLASH_HW_ALIGNMENT) == 0);
	u1Reg = SFLASH_RREG8(SFLASH_SF_INTRSTUS_REG);
	SFLASH_WREG8(SFLASH_SF_INTRSTUS_REG, u1Reg);	 // Clear interrupt
	SFLASH_WREG8(SFLASH_SF_INTREN_REG, 0x10);
	//printf("SFLASH_SF_INTRSTUS_REG = 0x%x\n",SFLASH_RREG8(SFLASH_SF_INTRSTUS_REG));
	//printf("SFLASH_SF_INTREN_REG = 0x%x\n",SFLASH_RREG8(SFLASH_SF_INTREN_REG));

    SFLASH_WREG8(SFLASH_RADR2_REG, LoByte(HiWord(u4Addr))); // Write
    SFLASH_WREG8(SFLASH_RADR1_REG, HiByte(LoWord(u4Addr))); // Write
    SFLASH_WREG8(SFLASH_RADR0_REG, LoByte(LoWord(u4Addr))); // Write


	//the RAM start address for flash
	u4MemPtr = SFLASH_RAM_BASE;
	u4MemSrc = (UINT32)pu1Buf;

	//make sure SRAM clock is not the nor clock
#if 0
	u1Reg = SFLASH_RREG8(SFLASH_CFG2_REG);
	SFLASH_WREG8(SFLASH_CFG2_REG,u1Reg & 0x7e );
#else
	if(_WBDisable() != 0)
	{
		return -1;
	}
#endif
	x_memcpy((void *)(u4MemPtr),(void *)(u4MemSrc), u4Len);

	//borrow bim 8k ram as program buf, switch buf to page program mode
	u1Reg = SFLASH_RREG8(SFLASH_CFG2_REG);
	SFLASH_WREG8(SFLASH_CFG2_REG,u1Reg | 0x81 );

	//total page in program buffer
	SFLASH_WREG8((UINT8)0x90,u1Pagecount);

	//page size,count in 4Bytes, max 255*4
	SFLASH_WREG8((UINT8)0x94,u1Pagesize);
	//printf("u4Pagesize = 0x%x 0x90 = 0x%x 0x94 = 0x%x", u1Pagesize ,SFLASH_RREG8((UINT8)0x90), SFLASH_RREG8((UINT8)0x94));
	//write 0x98 to trigger program
	_SendFlashCommand(0x98);

	//wait for interrupt

#if !defined (CC_MTK_LOADER)&& !defined(CC_MTK_LOADER)
		printf("_WritePageProgram interrupt\n");
		if(x_sema_lock_timeout(_hSFLASHSema, SFLASH_WRITECMD_TIMEOUT) == OSR_TIMEOUT)
		{
			return -1;
		}

#else
		UINT32 u4Polling;
		u4Polling = 0;
		while(1)
		{
			u1Reg = SFLASH_RREG8(SFLASH_SF_INTRSTUS_REG);
			printf("u1Reg = 0%d",u1Reg);
			if(0x10 == (u1Reg & 0x10))
			{
				break;
			}

			u4Polling++;
			printf("u4Polling = %d\n", u4Polling);
			if(u4Polling > SFLASH_ERASESECTOR_TIMEOUT)
			{
				printf("u4Polling = 0%d...............",u4Polling);

				return -1;
			}

			x_thread_delay(100);
		}

#endif


/*
    if(_ExecuteWriteCmd(u4Index) != 0)
    {
        return -1;
    }

    //--------------------------------------------------------------
    if(_WaitForWriteBusy(u4Index, SFLASH_WRITEBUSY_TIMEOUT) != 0)
    {
        return -1;
    }*/
	_SendFlashCommand(0x0);

    return 0;
}

//-----------------------------------------------------------------------------
/** Brief of SFLASHHW_WriteMutilPP.
 *  Details of SFLASHHW_WriteMutil_Page_Program (optional).
 *  @retval TRUE    Success
 *  @retval FALSE   Fail
 */
//-----------------------------------------------------------------------------
INT32 SFLASHHW_WriteMutilPP(UINT32 u4Index, UINT32 u4Addr, UINT32 u4Len,
                           const UINT8* pu1Buf)
{
    UINT32 i, u4Count, u4PgAlign;

	  if(u4Index >= MAX_FLASHCOUNT)
	  {
       LOG(1, "Nor Flash Index is out fo Max Flash Count\n");
       return -1;
	  }

    ASSERT(_arFlashChip[u4Index].u1MenuID != 0x00);
    ASSERT(u4Len <= _arFlashChip[u4Index].u4SecSize);

    if(u4Len == 0)
    {
        return 0;
    }

    if(pu1Buf == NULL)
    {
        return -1;
    }

    if(_SetFlashWriteEnable(u4Index) != 0)
    {
        return -1;
    }

    // Handle cross page
    //sst don't use AAI write singlebyte,so if handle cross page,disable AAIwrite,
    u4PgAlign = u4Addr % PP_SIZE;
    if(u4PgAlign != 0)
    {
    	_fgAAIWrite = FALSE;
        for(i=0; i<(PP_SIZE - u4PgAlign); i++)
        {
            if(_WriteSingleByte(u4Index, u4Addr, *pu1Buf) != 0)
            {
                return -1;
            }
            u4Addr++;
            pu1Buf++;
            u4Len--;

            if(u4Len == 0)
            {
                return 0;
            }
        }
    }

	//After handle cross page,we must reenble AAIwrite
	#if (defined(ENABLE_AAIWRITE))
	_fgAAIWrite = TRUE;
    #endif


    if(_WBEnable() != 0)
    {
        return -1;
    }
    if(_arFlashChip[u4Index].u1PPType ==0)
    {
		while((INT32)u4Len > 0)
		{
			if(u4Len >= PP_SIZE*PAGE_COUNT)
			{
				u4Count = PP_SIZE*PAGE_COUNT;
			}
			else
			{
				// Not write-buffer alignment
				break;
			}
			//printf("_WritePageProgram PAGE_COUNT = 0x%x, faddr = 0x%x, len = 0x%x\n", PAGE_COUNT, u4Addr, u4Count);

			if(_WritePageProgram(u4Index, PAGE_COUNT, (PP_SIZE/4), u4Addr, u4Count, pu1Buf) != 0)
			{
				LOG(1, "Write flash error!! faddr = 0x%x, len = 0x%x\n", u4Addr, u4Count);
				if(_WBDisable() != 0)
				{
					return -1;
				}
				return -1;
			}
			u4Len -= u4Count;
			u4Addr += u4Count;
			pu1Buf += u4Count;

		}


	    if(_WBDisable() != 0)
	    {
	        return -1;
	    }
    }
    if((INT32)u4Len > 0)
    {
    	_fgAAIWrite = FALSE;
        for(i=0; i<u4Len; i++)
        {
            if(_WriteSingleByte(u4Index, u4Addr, *pu1Buf) != 0)
            {
                if(_WBDisable() != 0)
                {
                    return -1;
                }
                return -1;
            }
            u4Addr++;
            pu1Buf++;
        }
    }
	//After handle cross page,we must reenble AAIwrite
	#if (defined(ENABLE_AAIWRITE))
	_fgAAIWrite = TRUE;
    #endif

    return 0;
}

//-----------------------------------------------------------------------------
/*
u1Pagecount Total page in program buffer
u1Pagesize  page size,count in 4Bytes, max 255*4

*/
static INT32 _WriteCountPageProgram(UINT32 u4Index, UINT8 u1Pagecount, UINT8 u1Pagesize, UINT32 u4Addr, UINT32 u4Len, const UINT8* pu1Buf)
{
    UINT32 u4MemPtr, u4MemSrc;
    UINT8 u1Reg;
	UINT32 u4Status;
	UINT32 u4Polling = 0;

    if(pu1Buf == NULL)
    {
        printf("_WriteCountPageProgram pu1Buf is NUL\n");
		return 1;
    }
	printf(" _WriteCountPageProgram u4Addr = 0x%x u4Len = 0x%x u1Pagecount = 0x%x \n",u4Addr,u4Len,u1Pagecount);
    ASSERT(u4Len <= PP_SIZE*u1Pagecount);
    ASSERT((u4Addr%SFLASH_HW_ALIGNMENT) == 0);
    ASSERT((u4Len%SFLASH_HW_ALIGNMENT) == 0);

    SFLASH_WREG8(SFLASH_RADR2_REG, LoByte(HiWord(u4Addr))); // Write
    SFLASH_WREG8(SFLASH_RADR1_REG, HiByte(LoWord(u4Addr))); // Write
    SFLASH_WREG8(SFLASH_RADR0_REG, LoByte(LoWord(u4Addr))); // Write

	//the RAM start address for flash
	u4MemPtr = SFLASH_RAM_BASE ;
	u4MemSrc = (UINT32)pu1Buf;

	//make sure SRAM clock is not the nor clock
	if(_WBDisable() != 0)
	{
		printf("_WBDisable fail before write\n");
		return -1;
	}

	//make sure the clock is SRAM clock nor DRAM clock
	u4Status = *((volatile unsigned int*)0xf00081d0);
	if(0x10 == (u4Status & 0x10))
	{
		printf("The clock is not sram clock 0xf00081d0 = 0x%x\n" ,u4Status);
		u4Status = (u4Status & 0xef);
		*((volatile unsigned int*)0xf00081d0) = u4Status;
	}
	if(_WBEnable() != 0)
	{
		printf("_WBEnable fail before memxpy\n");
		return -1;
	}
	x_memcpy((void *)(u4MemPtr),(void *)(u4MemSrc), u4Len);

	u1Reg = SFLASH_RREG8(SFLASH_SF_INTRSTUS_REG);
	SFLASH_WREG8(SFLASH_SF_INTRSTUS_REG, u1Reg);	 // Clear interrupt

	//borrow bim 8k ram as program buf, switch buf to page program mode
	//u1Reg = SFLASH_RREG8(SFLASH_CFG2_REG);
	//SFLASH_WREG8(SFLASH_CFG2_REG,u1Reg | 0x80);

	//total page in program buffer
	SFLASH_WREG8((UINT8)0x90,u1Pagecount);

	//page size,count in 4Bytes, max 255*4
	SFLASH_WREG8((UINT8)0x94,u1Pagesize);


//#if !defined (CC_MTK_LOADER)&& !defined(CC_MTK_LOADER)
#if 0

	SFLASH_WREG8(SFLASH_SF_INTREN_REG, 0x10);
#endif

	u1Reg = SFLASH_RREG8(SFLASH_CFG2_REG);
	SFLASH_WREG8(SFLASH_CFG2_REG,u1Reg | 0x80);

	//write 0x98 to trigger program
	_SendFlashCommand(0x98);


	//wait for interrupt
//#if !defined (CC_MTK_LOADER)&& !defined(CC_MTK_LOADER)
#if 0
		//printf("_WriteCountPageProgram interrupt\n");
		//printf("............ _WriteCountPageProgram _hSFLASHSema lock \n");
		if(x_sema_lock_timeout(_hSFLASHSema, SFLASH_WRITECMD_TIMEOUT) == OSR_TIMEOUT)
		{
			printf("x_sema_lock_timeout...............\n");
			return -1;
		}
#else
		u4Polling = 0;
		while(1)
		{
			u1Reg = SFLASH_RREG8(SFLASH_SF_INTRSTUS_REG);
			if(0x10 == (u1Reg & 0x10))
			{
				break;
			}

			u4Polling++;
			//if(u4Polling > SFLASH_ERASESECTOR_TIMEOUT)
			if(u4Polling > SFLASH_WRITEBUSY_TIMEOUT)
			{
				printf("u4Polling = %d timeout\n",u4Polling);
				return -1;
			}
		}
#endif

/*
    if(_ExecuteWriteCmd(u4Index) != 0)
    {
        return -1;
    }

    //--------------------------------------------------------------
    if(_WaitForWriteBusy(u4Index, SFLASH_WRITEBUSY_TIMEOUT) != 0)
    {
        return -1;
    }*/

 	//make sure SRAM clock is not the nor clock
	if(_WBDisable() != 0)
	{
		printf("_WBDisable fail after write\n");
		return -1;
	}
	_SendFlashCommand(0x0);
	return 0;
}

//-----------------------------------------------------------------------------
/** Brief of SFLASHHW_WriteCountMutilPP.
 *  Details of SFLASHHW_WriteMutil_Page_Program (optional).
 *  @retval TRUE    Success
 *  @retval FALSE   Fail
 */
//-----------------------------------------------------------------------------
INT32 SFLASHHW_WriteCountMutilPP(UINT32 u4Index, UINT32 u4Addr, UINT32 u4Len,
                           const UINT8* pu1Buf, UINT8 u1PageCount)
{
    UINT32 i, u4Count, u4PgAlign;

	if(u4Index >= MAX_FLASHCOUNT)
	{
       LOG(1, "Nor Flash Index is out fo Max Flash Count\n");
       return -1;
	}

    ASSERT(_arFlashChip[u4Index].u1MenuID != 0x00);
    ASSERT(u4Len <= _arFlashChip[u4Index].u4SecSize);

    if(u4Len == 0)
    {
        return 0;
    }

    if(pu1Buf == NULL)
    {
        return -1;
    }

    if(_SetFlashWriteEnable(u4Index) != 0)
    {
        return -1;
    }

    // Handle cross page
    //sst don't use AAI write singlebyte,so if handle cross page,disable AAIwrite,
    u4PgAlign = u4Addr % PP_SIZE;
    if(u4PgAlign != 0)
    {
    	_fgAAIWrite = FALSE;
        for(i=0; i<(PP_SIZE - u4PgAlign); i++)
        {
            if(_WriteSingleByte(u4Index, u4Addr, *pu1Buf) != 0)
            {
                return -1;
            }
            u4Addr++;
            pu1Buf++;
            u4Len--;

            if(u4Len == 0)
            {
                return 0;
            }
        }
    }

	//After handle cross page,we must reenble AAIwrite
	#if (defined(ENABLE_AAIWRITE))
	_fgAAIWrite = TRUE;
    #endif


    //if(_WBEnable() != 0)
    //{
    //    return -1;
    //}
    if(_arFlashChip[u4Index].u1PPType ==0)
    {
		while((INT32)u4Len > 0)
		{
			if(u4Len >= PP_SIZE*u1PageCount)
			{
				u4Count = PP_SIZE*u1PageCount;
			}
			else
			{
				// Not write-buffer alignment
				break;
			}

			if(_WriteCountPageProgram(u4Index, u1PageCount, (PP_SIZE/4), u4Addr, u4Count, pu1Buf) != 0)
			{
				LOG(1, "Write flash error!! faddr = 0x%x, len = 0x%x\n", u4Addr, u4Count);
				/*if(_WBDisable() != 0)
				{
					return -1;
				}*/
				return -1;
			}
			u4Len -= u4Count;
			u4Addr += u4Count;
			pu1Buf += u4Count;
			//_u4PpWriteCnt += 1;

		}


	    //if(_WBDisable() != 0)
	    //{
	      //  return -1;
	    //}
    }
    if((INT32)u4Len > 0)
    {
    	_fgAAIWrite = FALSE;
		printf(" _WriteSingleByte u4Addr = 0x%x u4Len  = 0x%x...............\n",u4Addr,u4Len);
        for(i=0; i<u4Len; i++)
        {
            if(_WriteSingleByte(u4Index, u4Addr, *pu1Buf) != 0)
            {
                //if(_WBDisable() != 0)
                //{
                //    return -1;
               // }
                return -1;
            }
            u4Addr++;
            pu1Buf++;
        }
    }
	//After handle cross page,we must reenble AAIwrite
	#if (defined(ENABLE_AAIWRITE))
	_fgAAIWrite = TRUE;
    #endif

    return 0;
}

//-----------------------------------------------------------------------------
/** Brief of SFLASHHW_WriteProgram. write 2/3/4 bytes through program function, this is only for verigy
 *  Details of SFLASHHW_WriteProgram (optional).
 *  @pram u4Len is 2/3/4
 *  @retval TRUE    Success
 *  @retval FALSE   Fail
 */
//-----------------------------------------------------------------------------
INT32 SFLASHHW_WriteProgram(UINT32 u4Index, UINT32 u4Addr, UINT32 u4Len,
                           const UINT8* pu1Buf)
{
	if(u4Index >= MAX_FLASHCOUNT)
	{
       LOG(1, "Nor Flash Index is out fo Max Flash Count\n");
       return -1;
	}
    ASSERT(_arFlashChip[u4Index].u1MenuID != 0x00);
    ASSERT(u4Len <= _arFlashChip[u4Index].u4SecSize);
    if(u4Len == 0)
    {
        return 0;
    }
    if(pu1Buf == NULL)
    {
        return -1;
    }
    if(_SetFlashWriteEnable(u4Index) != 0)
    {
        return -1;
    }
	if(u4Len == 2)
	{

		SFLASH_WREG8(SFLASH_PRGDATA5_REG, _arFlashChip[u4Index].u1PAGEPROGRAMCmd);
		SFLASH_WREG8(SFLASH_PRGDATA4_REG, LoByte(HiWord(u4Addr)));
		SFLASH_WREG8(SFLASH_PRGDATA3_REG, HiByte(LoWord(u4Addr)));
		SFLASH_WREG8(SFLASH_PRGDATA2_REG, LoByte(LoWord(u4Addr)));
		SFLASH_WREG8(SFLASH_PRGDATA1_REG, pu1Buf[0]);
		SFLASH_WREG8(SFLASH_PRGDATA0_REG, pu1Buf[1]);
		//printf("u4Addr = 0x%x, _arFlashChip[u4Index].u1PAGEPROGRAMCmd = 0x%x\n", u4Addr, _arFlashChip[u4Index].u1PAGEPROGRAMCmd);
		printf("pu1Buf[0] = 0x%x,pu1Buf[1] = 0x%x\n", pu1Buf[0], pu1Buf[1]);
		SFLASH_WREG8(SFLASH_CNT_REG, 48);           // Write SF Bit Count
	    if(u4Index == 0)
	    {
	        _SendFlashCommand(0x4);
	    }
#ifdef USE_SECOND_FLASH
	    else if(u4Index == 1)
	    {
	        _SendFlashCommand(0x44);
	    }
#else
	    else
	    {
	        ASSERT(0);
	    }
#endif
	    if( _PollingReg(SFLASH_CMD_REG, 0x04) != 0) // timeout
	    {
	        return -1;
	    }
	    _SendFlashCommand(0x0);

	}
	else if(u4Len == 3)
	{
		SFLASH_WREG8(SFLASH_PRGDATA5_REG, _arFlashChip[u4Index].u1PAGEPROGRAMCmd);
		SFLASH_WREG8(SFLASH_PRGDATA4_REG, LoByte(HiWord(u4Addr)));
		SFLASH_WREG8(SFLASH_PRGDATA3_REG, HiByte(LoWord(u4Addr)));
		SFLASH_WREG8(SFLASH_PRGDATA2_REG, LoByte(LoWord(u4Addr)));
		SFLASH_WREG8(SFLASH_PRGDATA1_REG, pu1Buf[0]);
		SFLASH_WREG8(SFLASH_PRGDATA0_REG, pu1Buf[1]);
		SFLASH_WREG8(SFLASH_PRGDATA6_REG, pu1Buf[2]);
		//printf("u4Addr = 0x%x, _arFlashChip[u4Index].u1PAGEPROGRAMCmd = 0x%x\n", u4Addr, _arFlashChip[u4Index].u1PAGEPROGRAMCmd);
		printf("pu1Buf[0] = 0x%x,pu1Buf[1] = 0x%x,pu1Buf[2] = 0x%x\n", pu1Buf[0], pu1Buf[1], pu1Buf[2]);
		SFLASH_WREG8(SFLASH_CNT_REG, 56);			// Write SF Bit Count
		if(u4Index == 0)
		{
			_SendFlashCommand(0x4);
		}
		else
		{
			ASSERT(0);
		}
		if( _PollingReg(SFLASH_CMD_REG, 0x04) != 0) // timeout
		{
			return -1;
		}
		_SendFlashCommand(0x0);

	}
	else if(u4Len == 4)
	{
		SFLASH_WREG8(SFLASH_PRGDATA5_REG, _arFlashChip[u4Index].u1PAGEPROGRAMCmd);
		SFLASH_WREG8(SFLASH_PRGDATA4_REG, LoByte(HiWord(u4Addr)));
		SFLASH_WREG8(SFLASH_PRGDATA3_REG, HiByte(LoWord(u4Addr)));
		SFLASH_WREG8(SFLASH_PRGDATA2_REG, LoByte(LoWord(u4Addr)));
		SFLASH_WREG8(SFLASH_PRGDATA1_REG, pu1Buf[0]);
		SFLASH_WREG8(SFLASH_PRGDATA0_REG, pu1Buf[1]);
		SFLASH_WREG8(SFLASH_PRGDATA6_REG, pu1Buf[2]);
		SFLASH_WREG8(SFLASH_PRGDATA7_REG, pu1Buf[3]);
		//printf("u4Addr = 0x%x, _arFlashChip[u4Index].u1PAGEPROGRAMCmd = 0x%x\n", u4Addr, _arFlashChip[u4Index].u1PAGEPROGRAMCmd);
		printf("pu1Buf[0] = 0x%x, pu1Buf[1] = 0x%x, pu1Buf[2] = 0x%x, pu1Buf[3] = 0x%x\n", pu1Buf[0], pu1Buf[1], pu1Buf[2], pu1Buf[3]);
		SFLASH_WREG8(SFLASH_CNT_REG, 64);			// Write SF Bit Count
		if(u4Index == 0)
		{
			_SendFlashCommand(0x4);
		}
		else
		{
			ASSERT(0);
		}
		if( _PollingReg(SFLASH_CMD_REG, 0x04) != 0) // timeout
		{
			return -1;
		}
		_SendFlashCommand(0x0);
	}
	else
	{
		printf("the u4Len = 0x%x,it is not 2, 3 or 4\n",u4Len);
		return -1;
	}
    return 0;
}


//-----------------------------------------------------------------------------
/** Brief of SFLASHHW_WriteRISCProgram.write 2/3/4 bytes through RISC0/1 program function, this is only for verigy
 *  Details of SFLASHHW_WriteRISCProgram (optional).
 *  @pram u4Len is 2/3/4
 *  @retval TRUE    Success
 *  @retval FALSE   Fail
 */
//-----------------------------------------------------------------------------
INT32 SFLASHHW_WriteRISCProgram(UINT32 u4Index, UINT32 u4Addr, UINT32 u4Len,
                           const UINT8* pu1Buf)
{
    UINT32 i, u4BufIdx, u4Data;

	if(u4Index >= MAX_FLASHCOUNT)
	{
       LOG(1, "Nor Flash Index is out fo Max Flash Count\n");
       return -1;
	}
    ASSERT(_arFlashChip[u4Index].u1MenuID != 0x00);
    ASSERT(u4Len <= _arFlashChip[u4Index].u4SecSize);
    if(u4Len == 0)
    {
        return 0;
    }
    if(pu1Buf == NULL)
    {
        return -1;
    }
    if(_SetFlashWriteEnable(u4Index) != 0)
    {
        return -1;
    }
	if(u4Len == 2)
	{
		u4Data = (((UINT32)_arFlashChip[u4Index].u1PAGEPROGRAMCmd) << 24)|u4Addr;
		printf("the u4Data = 0x%x,u4Addr = 0x%x, _arFlashChip[u4Index].u1PAGEPROGRAMCmd = 0x%x\n", u4Data, u4Addr, _arFlashChip[u4Index].u1PAGEPROGRAMCmd);
		SFLASH_WREG32(SFLASH_PRGDATA_RISC1, u4Data);
		u4BufIdx = 0;
		for(i=0; i<u4Len; i++)
		{
			(*((UINT8 *)&u4Data + (3 - i))) = pu1Buf[u4BufIdx];
			printf("the u4Data = 0x%x,pu1Buf[%d] = 0x%x\n", u4Data, u4BufIdx, pu1Buf[u4BufIdx]);
			u4BufIdx++;
		}
		//printf("the u4Data = 0x%x\n", u4Data);
		SFLASH_WREG32(SFLASH_PRGDATA_RISC0, u4Data);
		SFLASH_WREG8(SFLASH_CNT_REG, 48);			// Write SF Bit Count
		if(u4Index == 0)
		{
			_SendFlashCommand(0x4);
		}
	#ifdef USE_SECOND_FLASH
		else if(u4Index == 1)
		{
			_SendFlashCommand(0x44);
		}
	#else
		else
		{
			ASSERT(0);
		}
	#endif
		if( _PollingReg(SFLASH_CMD_REG, 0x04) != 0) // timeout
		{
			return -1;
		}
		_SendFlashCommand(0x0);
	}
	else if(u4Len == 3)
	{
		u4Data = (((UINT32)_arFlashChip[u4Index].u1PAGEPROGRAMCmd) << 24)|u4Addr;
		printf("the u4Data = 0x%x,u4Addr = 0x%x, _arFlashChip[u4Index].u1PAGEPROGRAMCmd = 0x%x\n", u4Data, u4Addr, _arFlashChip[u4Index].u1PAGEPROGRAMCmd);
		SFLASH_WREG32(SFLASH_PRGDATA_RISC1, u4Data);
		u4BufIdx = 0;
		for(i=0; i<u4Len; i++)
		{
			(*((UINT8 *)&u4Data + (3 - i))) = pu1Buf[u4BufIdx];
			printf("the u4Data = 0x%x,pu1Buf[%d] = 0x%x\n", u4Data, u4BufIdx, pu1Buf[u4BufIdx]);
			 u4BufIdx++;
		}
		printf("the u4Data = 0x%x\n", u4Data);
		SFLASH_WREG32(SFLASH_PRGDATA_RISC0, u4Data);
		SFLASH_WREG8(SFLASH_CNT_REG, 56);			// Write SF Bit Count
		if(u4Index == 0)
		{
			_SendFlashCommand(0x4);
		}
		else
		{
			ASSERT(0);
		}
		if( _PollingReg(SFLASH_CMD_REG, 0x04) != 0) // timeout
		{
			return -1;
		}
		_SendFlashCommand(0x0);
	}
	else if(u4Len == 4)
	{
		u4Data = (((UINT32)_arFlashChip[u4Index].u1PAGEPROGRAMCmd) << 24)|u4Addr;
		printf("the u4Data = 0x%x,u4Addr = 0x%x, _arFlashChip[u4Index].u1PAGEPROGRAMCmd = 0x%x\n", u4Data, u4Addr, _arFlashChip[u4Index].u1PAGEPROGRAMCmd);
		SFLASH_WREG32(SFLASH_PRGDATA_RISC1, u4Data);
		u4BufIdx = 0;
		for(i=0; i<u4Len; i++)
		{
			(*((UINT8 *)&u4Data + (3 - i))) = pu1Buf[u4BufIdx];
			printf("the u4Data = 0x%x,pu1Buf[%d] = 0x%x\n", u4Data, u4BufIdx, pu1Buf[u4BufIdx]);
			u4BufIdx++;
		}
		//printf("the u4Data = 0x%x\n", u4Data);
		SFLASH_WREG32(SFLASH_PRGDATA_RISC0, u4Data);
		SFLASH_WREG8(SFLASH_CNT_REG, 64);			// Write SF Bit Count
		if(u4Index == 0)
		{
			_SendFlashCommand(0x4);
		}
		else
		{
			ASSERT(0);
		}
		if( _PollingReg(SFLASH_CMD_REG, 0x04) != 0) // timeout
		{
			return -1;
		}
		_SendFlashCommand(0x0);
	}
	else
	{
		printf("the u4Len = 0x%x,it is not 2, 3 or 4\n",u4Len);
		return -1;
	}
    return 0;
}

#endif
//-----------------------------------------------------------------------------
/** Brief of SFLASHHW_WriteSector.
 *  Details of SFLASHHW_WriteSector (optional).
 *  @retval TRUE    Success
 *  @retval FALSE   Fail
 */
//-----------------------------------------------------------------------------
INT32 SFLASHHW_WriteSector(UINT32 u4Index, UINT32 u4Addr, UINT32 u4Len,
                           const UINT8* pu1Buf)
{
    UINT32 i, u4Count, u4PgAlign;
	  if(u4Index >= MAX_FLASHCOUNT)
	  {
       LOG(1, "Nor Flash Index is out fo Max Flash Count\n");
       return -1;
	  }
    ASSERT(_arFlashChip[u4Index].u1MenuID != 0x00);
    ASSERT(u4Len <= _arFlashChip[u4Index].u4SecSize);

    if(u4Len == 0)
    {
        return 0;
    }

    if(pu1Buf == NULL)
    {
        return 1;
    }

    if(_SetFlashWriteEnable(u4Index) != 0)
    {
        return 1;
    }

    // Handle cross page
    //sst don't use AAI write singlebyte,so if handle cross page,disable AAIwrite,

    u4PgAlign = u4Addr % SFLASH_WRBUF_SIZE;
    if(u4PgAlign != 0)
    {
    	_fgAAIWrite = FALSE;
        for(i=0; i<(SFLASH_WRBUF_SIZE - u4PgAlign); i++)
        {
            if(_WriteSingleByte(u4Index, u4Addr, *pu1Buf) != 0)
            {
                return 1;
            }
            u4Addr++;
            pu1Buf++;
            u4Len--;

            if(u4Len == 0)
            {
                return 0;
            }
        }
    }

	//After handle cross page,we must reenble AAIwrite
	#if (defined(ENABLE_AAIWRITE))
	_fgAAIWrite = TRUE;
    #endif
    if(_WBEnable() != 0)
    {
        return 1;
    }
    if(_arFlashChip[u4Index].u1PPType ==0)
    {
    while((INT32)u4Len > 0)
    {
        if(u4Len >= SFLASH_WRBUF_SIZE)
        {
            u4Count = SFLASH_WRBUF_SIZE;
        }
        else
        {
            // Not write-buffer alignment
            break;
        }

        if(_WriteBuffer(u4Index, u4Addr, u4Count, pu1Buf) != 0)
        {
            LOG(3, "Write flash error!! faddr = 0x%x, len = 0x%x\n", u4Addr, u4Count);
            if(_WBDisable() != 0)
            {
                return 1;
            }
            return 1;
        }
        u4Len -= u4Count;
        u4Addr += u4Count;
        pu1Buf += u4Count;

    }

    if(_WBDisable() != 0)
    {
        return 1;
    }
    }
    if((INT32)u4Len > 0)
    {
    	_fgAAIWrite = FALSE;
        for(i=0; i<u4Len; i++)
        {
            if(_WriteSingleByte(u4Index, u4Addr, *pu1Buf) != 0)
            {
                if(_WBDisable() != 0)
                {
                    return 1;
                }
                return 1;
            }
            u4Addr++;
            pu1Buf++;
        }
    }
	//After handle cross page,we must reenble AAIwrite
	#if (defined(ENABLE_AAIWRITE))
	_fgAAIWrite = TRUE;
    #endif

    return 0;
}

//-----------------------------------------------------------------------------
/** Brief of SFLASHHW_EraseSector.
 *  Details of SFLASHHW_EraseSector (optional).
 *  @retval TRUE    Success
 *  @retval FALSE   Fail
 */
//-----------------------------------------------------------------------------
INT32 SFLASHHW_EraseSector(UINT32 u4Index, UINT32 u4Addr)
{
    UINT32 u4Polling;
    UINT8 u1Reg;

	  if(u4Index >= MAX_FLASHCOUNT)
	  {
       LOG(1, "Nor Flash Index is out fo Max Flash Count\n");
       return -1;
	  }
    ASSERT(_arFlashChip[u4Index].u1MenuID != 0x00);

    //--------------------------------------------------------------
    if(_WaitForWriteBusy(u4Index, SFLASH_WRITEBUSY_TIMEOUT) != 0)
    {
        return 1;
    }

    //--------------------------------------------------------------
    if(_SetFlashWriteEnable(u4Index) != 0)
    {
        return 1;
    }

    // Execute sector erase command
    SFLASH_WREG8(SFLASH_PRGDATA5_REG, _arFlashChip[u4Index].u1SECERASECmd);
    SFLASH_WREG8(SFLASH_PRGDATA4_REG, LoByte(HiWord(u4Addr))); // Write
    SFLASH_WREG8(SFLASH_PRGDATA3_REG, HiByte(LoWord(u4Addr))); // Write
    SFLASH_WREG8(SFLASH_PRGDATA2_REG, LoByte(LoWord(u4Addr))); // Write
    SFLASH_WREG8(SFLASH_CNT_REG, 32);       // Write SF Bit Count

#ifdef USE_FLASH_ERASE_ISR	//use program interrupt for erase
	//printf("SFLASHHW_EraseSector USE_FLASH_ERASE_ISR......... ");
	u1Reg = SFLASH_RREG8(SFLASH_SF_INTRSTUS_REG);
	SFLASH_WREG8(SFLASH_SF_INTRSTUS_REG, u1Reg);	// Clear interrupt
	SFLASH_WREG8(SFLASH_SF_INTREN_REG, 0x04);
	if(u4Index==0)
	{
		_SendFlashCommand(0xc);
	}
#ifdef USE_SECOND_FLASH
	else if(u4Index == 1)
	{
		_SendFlashCommand(0x4c);
	}
#endif
	else
	{
		ASSERT(0);
	}
		u4Polling = 0;
#if !defined (CC_MTK_LOADER)&& !defined(CC_MTK_LOADER)
	if(x_sema_lock_timeout(_hSFLASHSema, SFLASH_WRITECMD_TIMEOUT) == OSR_TIMEOUT)
	{
		return -1;
	}

#else

	while(1)
	{
		u1Reg = SFLASH_RREG8(SFLASH_SF_INTRSTUS_REG);
		if(4 == (u1Reg & 0x4))
		{
			break;
		}

		u4Polling++;
		if(u4Polling > SFLASH_ERASESECTOR_TIMEOUT)
		{
			return -1;
		}

		x_thread_delay(100);
	}
#endif

#else					//use cpu polling
    if(u4Index==0)
    {
        _SendFlashCommand(0x4);
    }
#ifdef USE_SECOND_FLASH
    else if(u4Index == 1)
    {
        _SendFlashCommand(0x44);
    }
#endif
    else
    {
        ASSERT(0);
    }

    // Can not use ISR mode, because there is not erase sector interrupt
    u4Polling = 0;
    while(1)
    {
        if(SFLASHHW_ReadFlashStatus(u4Index, &u1Reg) != 0)
        {
#ifdef BD_NOR_ENABLE
				SFLASHHW_SwitchChip(0);
#endif

            return 1;
        }

        if(0 == (u1Reg & 0x1))
        {
            break;
        }

        u4Polling++;
        if(u4Polling > SFLASH_ERASESECTOR_TIMEOUT)
        {
#ifdef BD_NOR_ENABLE
				SFLASHHW_SwitchChip(0);
#endif

            return 1;
        }

        x_thread_delay(10);
    }

#endif
    _SendFlashCommand(0x0);
#ifdef BD_NOR_ENABLE
		SFLASHHW_SwitchChip(0);
#endif

    return 0;
}

#ifdef BD_NOR_ENABLE
//-----------------------------------------------------------------------------
/** Brief of SFLASHHW_EraseChip.
 *  Details of SFLASHHW_EraseChip (optional).
 *  @retval TRUE    Success
 *  @retval FALSE   Fail
 */
//-----------------------------------------------------------------------------
INT32 SFLASHHW_EraseChip(UINT32 u4Index)
{
    HAL_TIME_T rOrgTime, rNewTime, rDeltaTime;
    UINT8 u1Reg;

    ASSERT(_arFlashChip[u4Index].u1MenuID != 0x00);

    if(u4Index >= _u4ChipCount)
    {
        LOG(3, "Flash chip not exist for %d\n", u4Index);
        return 1;
    }

    //--------------------------------------------------------------
    if(_WaitForWriteBusy(u4Index, SFLASH_WRITEBUSY_TIMEOUT) != 0)
    {
        return 1;
    }

    //--------------------------------------------------------------
    if(_SetFlashWriteEnable(u4Index) != 0)
    {
        return 1;
    }

    if(_fgSFIsrEn)
    {
        _u4SFIsrStatus = 0;
        SFLASH_WREG32(SFLASH_SF_INTRSTUS_REG, 0x8);    // Clear interrupt
        SFLASH_WREG32(SFLASH_SF_INTREN_REG, 0x8);
    }

    // Execute bulk erase command
    if(u4Index == 0)
    {
        _SendFlashCommand(0x8);
    }
#ifdef USE_SECOND_FLASH
    else if(u4Index == 1)
    {
        _SendFlashCommand(0x48);
    }
#endif
    else
    {
        ASSERT(0);
    }

    if(_fgSFIsrEn)
    {
        while(!(_u4SFIsrStatus & 0x8))
        {
#ifndef CC_5381_LOADER
            if(x_sema_lock_timeout(_hSFLASHSema, SFLASH_CHIPERASE_TIMEOUT) == OSR_TIMEOUT)
            {
                return 1;
            }
#endif /* CC_5381_LOADER */
        }
    }
    else
    {
        HAL_GetTime(&rOrgTime);
        while(1)
        {
            u1Reg = SFLASH_RREG8(SFLASH_CMD_REG);
            if (0x0 == (u1Reg & 0x8))
            {
                break;
            }

            HAL_GetTime(&rNewTime);
            HAL_GetDeltaTime(&rDeltaTime, &rOrgTime, &rNewTime);
            if (rDeltaTime.u4Seconds > (SFLASH_CHIPERASE_TIMEOUT/1000))
            {
                return 1;
            }

            x_thread_delay(1000);
        }
    }

    if(_fgSFIsrEn)
    {
        SFLASH_WREG32(SFLASH_SF_INTREN_REG, 0x0);   // Disable interrupt
    }

    _SendFlashCommand(0x0);


    return 0;
}
#else
//-----------------------------------------------------------------------------
/** Brief of SFLASHHW_EraseChip.
 *  Details of SFLASHHW_EraseChip (optional).
 *  @retval TRUE    Success
 *  @retval FALSE   Fail
 */
//-----------------------------------------------------------------------------
INT32 SFLASHHW_EraseChip(UINT32 u4Index)
{
    UINT32 u4Polling;
    UINT8 u1Reg;
    ASSERT(u4Index < MAX_FLASHCOUNT);
    ASSERT(_arFlashChip[u4Index].u1MenuID != 0x00);

    //--------------------------------------------------------------
    if(_WaitForWriteBusy(u4Index, SFLASH_WRITEBUSY_TIMEOUT) != 0)
    {
        return -1;
    }

    //--------------------------------------------------------------
    if(_SetFlashWriteEnable(u4Index) != 0)
    {
        return -1;
    }

    // Execute sector erase command
    SFLASH_WREG8(SFLASH_PRGDATA5_REG, _arFlashChip[u4Index].u1CHIPERASECmd);
    SFLASH_WREG8(SFLASH_CNT_REG, 8);       // Write SF Bit Count

    if(u4Index==0)
    {
        _SendFlashCommand(0x4);
    }
#ifdef USE_SECOND_FLASH
    else if(u4Index == 1)
    {
        _SendFlashCommand(0x44);
    }
#endif
    else
    {
        ASSERT(0);
    }

    // Can not use ISR mode, because there is not erase sector interrupt
    u4Polling = 0;
    while(1)
    {
        if(SFLASHHW_ReadFlashStatus(u4Index, &u1Reg) != 0)
        {
            return -1;
        }

        if(0 == (u1Reg & 0x1))
        {
            break;
        }

        u4Polling++;
        if(u4Polling > SFLASH_ERASESECTOR_TIMEOUT)
        {
            return -1;
        }

        x_thread_delay(10);
    }

    _SendFlashCommand(0x0);


    return 0;
}


#endif

//-----------------------------------------------------------------------------
/** Brief of SFLASHHW_CalCRC.
 *  Details of SFLASHHW_CalCRC (optional).
 *  @retval TRUE    Success
 *  @retval FALSE   Fail
 */
//-----------------------------------------------------------------------------
static UINT8 _u1ReadData = 0;
INT32 SFLASHHW_CalCRC(UINT32 u4Addr, UINT32 u4Len, UINT32* pu4CheckSum)
{
	UINT32 u4Loop;
    UINT8 *pu1Buf;
	//if _u1ReadData defined here will be fail. becaust the variable will not real use, the system will optimizer the code, and then will not do the operation read data from nor
	//UINT8  _u1ReadData;
	UNUSED(_u1ReadData);
    if(pu4CheckSum == NULL)
    {
        return 1;
    }

	SFLASH_WREG8(SFLASH_CFG2_REG, 0x0C);
    SFLASH_WREG8(SFLASH_CHKSUM_CTL_REG, 0x00);
	SFLASH_WREG32(SFLASH_CHECKSUM_REG, 0x0);

    SFLASH_WREG8(SFLASH_CHKSUM_CTL_REG, 0x1);  // start caculate CRC

	pu1Buf = (UINT8*)(u4Addr + SFLASH_MEM_BASE);
	for(u4Loop=0; u4Loop<u4Len; u4Loop++)
    {
		_u1ReadData = *pu1Buf;
        pu1Buf++;
    }

    SFLASH_WREG8(SFLASH_CHKSUM_CTL_REG, 0x0);   // stop calculate CRC

    *pu4CheckSum = SFLASH_RREG32(SFLASH_CHECKSUM_REG);


    return 0;
}

#ifdef BD_NOR_ENABLE

//-----------------------------------------------------------------------------
/*
*Enter 4 bytes address mode
*return 0 success, return others fial
*/
INT32 _SetFlashEnter4Byte(void)
{
	UINT32 u4Index, u4DualReg, u4Polling;
	u4Index = 0;
	if(_SetFlashWriteEnable(u4Index) != 0)
    {
        return 1;
    }

    if( (u1MenuID == 0x01) &&(u1DevID1 == 0x02) &&(u1DevID2 == 0x19) )	//for spansion S25FL256s flash
    {
        SFLASH_WREG8(SFLASH_PRGDATA5_REG, 0x17);	//Enter EN4B cmd
        SFLASH_WREG8(SFLASH_PRGDATA4_REG, 0x80);
        SFLASH_WREG8(SFLASH_CNT_REG,16); 			// Write SF Bit Count
    }
    else
    {
        SFLASH_WREG8(SFLASH_PRGDATA5_REG, 0xb7);	//Enter EN4B cmd
        SFLASH_WREG8(SFLASH_CNT_REG,8); 			// Write SF Bit Count
    }
    _SendFlashCommand(0x4);	// Write PRG
    if( _PollingReg(SFLASH_CMD_REG, 0x04) == 1) // timeout
    {
        return 1;
    }

	u4DualReg = SFLASH_RREG32(SFLASH_READ_DUAL_REG);
	u4DualReg |= 0x10;
	SFLASH_WREG32(SFLASH_READ_DUAL_REG, u4DualReg);

    u4Polling = 0;
    while(1)
    {
        u4DualReg = SFLASH_RREG32(SFLASH_READ_DUAL_REG);
        if (0x10 == (u4DualReg & 0x10))
        {
            break;
        }
        u4Polling++;
        if(u4Polling > SFLASH_POLLINGREG_COUNT)
        {
            return 1;
        }
    }
	LOG(0, "...cdd Enter 4 bytes address!\n");
	return 0;
}


//-----------------------------------------------------------------------------
/*
*Enter 4 bit read mode
*return 0 success, return others fial
*/
INT32 _SetFlashEnter4BitRead(void)
{
	UINT32 u4Index;
	UINT8  u1status, u1flag, u1count = 0;
	u4Index = 0;
	if(_SetFlashWriteEnable(u4Index) != 0)
    {
        return 1;
    }
	LOG(0, "... u1MenuID = 0x%x u1DevID1 = 0x%x u1DevID2 = 0x%x!\n",u1MenuID,u1DevID1,u1DevID2);
	//if( (u1MenuID == 0x01) &&(u1DevID1 == 0x20) &&(u1DevID2 == 0x18))
	if( (u1MenuID == 0x01) &&(u1DevID1 == 0x20))	//for spansion S25FL128s flash
	{
		do
		{
			SFLASH_WREG8(SFLASH_PRGDATA5_REG, 0x01);	//write status cmd
			SFLASH_WREG8(SFLASH_PRGDATA4_REG, 0x1c);
			SFLASH_WREG8(SFLASH_PRGDATA3_REG, 0x0e);
			SFLASH_WREG8(SFLASH_CNT_REG,24);			// Write SF Bit Count

			_SendFlashCommand(0x4); // Write PRG
			if( _PollingReg(SFLASH_CMD_REG, 0x04) == 1) // timeout
			{
				return 1;
			}

			//read config status
			SFLASH_WREG8(SFLASH_PRGDATA5_REG, 0x35);	//read config cmd
			SFLASH_WREG8(SFLASH_PRGDATA4_REG, 0x00);
			SFLASH_WREG8(SFLASH_PRGDATA3_REG, 0x00);
			SFLASH_WREG8(SFLASH_CNT_REG,24);			// Write SF Bit Count
			_SendFlashCommand(0x4); // Write PRG
			if( _PollingReg(SFLASH_CMD_REG, 0x04) == 1) // timeout
			{
				return 1;
			}

			u1flag = (SFLASH_RREG8(SFLASH_SHREG0_REG)) & 0x2;	//make sure the quad read flag is enable
			//LOG(0, "...cdd flash config register status   = 0x%x!\n",u1flag);
			u1count++ ;
			if( u1count >= 200)
			{
				LOG(0, "...setting config register fail!\n");
				return 1;
			}
		}while(!u1flag);

	}
	else		////for mxic  flash
	{
		if(_SetFlashWriteEnable(u4Index) != 0)
		{
		    return 1;
		}
		//read config status
		SFLASH_WREG8(SFLASH_PRGDATA5_REG, 0x05);	//read status cmd
		SFLASH_WREG8(SFLASH_PRGDATA4_REG, 0x00);
		SFLASH_WREG8(SFLASH_PRGDATA3_REG, 0x00);
		SFLASH_WREG8(SFLASH_CNT_REG,24);			// Write SF Bit Count
		_SendFlashCommand(0x4); // Write PRG
		if( _PollingReg(SFLASH_CMD_REG, 0x04) == 1) // timeout
		{
			return 1;
		}
		u1status = SFLASH_RREG8(SFLASH_SHREG0_REG);	//make sure the quad read flag is enable
		u1status = u1status | 0x40;					////quad read enable bit
		LOG(0, "...cdd flash config register u1status   = 0x%x!\n",u1status);
		do
		{
		    SFLASH_WREG8(SFLASH_PRGDATA5_REG, 0x01);	//write status cmd
		    SFLASH_WREG8(SFLASH_PRGDATA4_REG, u1status);
			SFLASH_WREG8(SFLASH_PRGDATA3_REG, 0x00);
		    SFLASH_WREG8(SFLASH_CNT_REG,24); 			// Write SF Bit Count

		    _SendFlashCommand(0x4);	// Write PRG
		    if( _PollingReg(SFLASH_CMD_REG, 0x04) == 1) // timeout
		    {
		        return 1;
		    }
			if(_SetFlashWriteEnable(u4Index) != 0)
		    {
		        return 1;
		    }
			//read config status
			SFLASH_WREG8(SFLASH_PRGDATA5_REG, 0x05);	//read status cmd
			SFLASH_WREG8(SFLASH_PRGDATA4_REG, 0x00);
			SFLASH_WREG8(SFLASH_PRGDATA3_REG, 0x00);
			SFLASH_WREG8(SFLASH_CNT_REG,24);			// Write SF Bit Count
			_SendFlashCommand(0x4); // Write PRG
			if( _PollingReg(SFLASH_CMD_REG, 0x04) == 1) // timeout
			{
				return 1;
			}
			u1flag = (SFLASH_RREG8(SFLASH_SHREG0_REG)) & 0x40;	//make sure the quad read flag is enable
			LOG(0, "...cdd flash config register    = 0x%x!\n",(SFLASH_RREG8(SFLASH_SHREG0_REG)));
			u1count++ ;
			if( u1count >= 200)
			{
				LOG(0, "...setting status register fail!\n");
				return 1;
			}

		}while(!u1flag);
		 	//SFLASH_WREG8(SFLASH_PRGDATA5_REG, 0x35);
		    //SFLASH_WREG8(SFLASH_CNT_REG,08); 			// Write SF Bit Count
		    //_SendFlashCommand(0x4);	// Write PRG
	}
	//LOG(0, "...cdd Enter 4 bit read mode!\n");
	return 0;
}

//-----------------------------------------------------------------------------
/*
*Exit 4 bit read mode
*return 0 success, return others fial
*/
INT32 _SetFlashExit4BitRead(void)
{
	UINT32 u4Index;
	UINT8  u1status, u1flag, u1count = 0;
	u4Index = 0;
	if(_SetFlashWriteEnable(u4Index) != 0)
    {
        return 1;
    }
	LOG(0, "... u1MenuID = 0x%x u1DevID1 = 0x%x u1DevID2 = 0x%x!\n",u1MenuID,u1DevID1,u1DevID2);
	//if( (u1MenuID == 0x01) &&(u1DevID1 == 0x20) &&(u1DevID2 == 0x18))
	if( (u1MenuID == 0x01) &&(u1DevID1 == 0x20))	//for spansion S25FL128s flash
	{
		do
		{
			SFLASH_WREG8(SFLASH_PRGDATA5_REG, 0x01);	//write status cmd
			SFLASH_WREG8(SFLASH_PRGDATA4_REG, 0x1c);
			SFLASH_WREG8(SFLASH_PRGDATA3_REG, 0x00);
			SFLASH_WREG8(SFLASH_CNT_REG,24);			// Write SF Bit Count

			_SendFlashCommand(0x4); // Write PRG
			if( _PollingReg(SFLASH_CMD_REG, 0x04) == 1) // timeout
			{
				return 1;
			}

			//read config status
			SFLASH_WREG8(SFLASH_PRGDATA5_REG, 0x35);	//read config cmd
			SFLASH_WREG8(SFLASH_PRGDATA4_REG, 0x00);
			SFLASH_WREG8(SFLASH_PRGDATA3_REG, 0x00);
			SFLASH_WREG8(SFLASH_CNT_REG,24);			// Write SF Bit Count
			_SendFlashCommand(0x4); // Write PRG
			if( _PollingReg(SFLASH_CMD_REG, 0x04) == 1) // timeout
			{
				return 1;
			}

			u1flag = (SFLASH_RREG8(SFLASH_SHREG0_REG)) & 0x2;	//make sure the quad read flag is enable
			//LOG(0, "...cdd flash config register status   = 0x%x!\n",u1flag);
			u1count++ ;
			if( u1count >= 200)
			{
				LOG(0, "...setting config register fail!\n");
				return 1;
			}
		}while(!u1flag);

	}
	else		////for mxic  flash
	{
		if(_SetFlashWriteEnable(u4Index) != 0)
		{
		    return 1;
		}
		//read config status
		SFLASH_WREG8(SFLASH_PRGDATA5_REG, 0x05);	//read status cmd
		SFLASH_WREG8(SFLASH_PRGDATA4_REG, 0x00);
		SFLASH_WREG8(SFLASH_PRGDATA3_REG, 0x00);
		SFLASH_WREG8(SFLASH_CNT_REG,24);			// Write SF Bit Count
		_SendFlashCommand(0x4); // Write PRG
		if( _PollingReg(SFLASH_CMD_REG, 0x04) == 1) // timeout
		{
			return 1;
		}
		u1status = SFLASH_RREG8(SFLASH_SHREG0_REG);	//make sure the quad read flag is enable
		u1status = u1status & 0xBF;					////quad read disable bit
		LOG(0, "...cdd flash config register u1status   = 0x%x!\n",u1status);
		do
		{
		    SFLASH_WREG8(SFLASH_PRGDATA5_REG, 0x01);	//write status cmd
		    SFLASH_WREG8(SFLASH_PRGDATA4_REG, u1status);
			SFLASH_WREG8(SFLASH_PRGDATA3_REG, 0x00);
		    SFLASH_WREG8(SFLASH_CNT_REG,24); 			// Write SF Bit Count

		    _SendFlashCommand(0x4);	// Write PRG
		    if( _PollingReg(SFLASH_CMD_REG, 0x04) == 1) // timeout
		    {
		        return 1;
		    }
			if(_SetFlashWriteEnable(u4Index) != 0)
		    {
		        return 1;
		    }
			//read config status
			SFLASH_WREG8(SFLASH_PRGDATA5_REG, 0x05);	//read status cmd
			SFLASH_WREG8(SFLASH_PRGDATA4_REG, 0x00);
			SFLASH_WREG8(SFLASH_PRGDATA3_REG, 0x00);
			SFLASH_WREG8(SFLASH_CNT_REG,24);			// Write SF Bit Count
			_SendFlashCommand(0x4); // Write PRG
			if( _PollingReg(SFLASH_CMD_REG, 0x04) == 1) // timeout
			{
				return 1;
			}
			u1flag = (SFLASH_RREG8(SFLASH_SHREG0_REG)) & 0x40;	//make sure the quad read flag is disable
			LOG(0, "...cdd flash config register    = 0x%x!\n",(SFLASH_RREG8(SFLASH_SHREG0_REG)));
			u1count++ ;
			if( u1count >= 200)
			{
				LOG(0, "...setting status register fail!\n");
				return 1;
			}

		}while(u1flag);
		 	//SFLASH_WREG8(SFLASH_PRGDATA5_REG, 0x35);
		    //SFLASH_WREG8(SFLASH_CNT_REG,08); 			// Write SF Bit Count
		    //_SendFlashCommand(0x4);	// Write PRG
	}
	//LOG(0, "...cdd Enter 4 bit read mode!\n");
	return 0;
}

//-----------------------------------------------------------------------------
/*
*Exit 4 bytes address mode
*return 0 success, return others fial
*/
INT32 _SetFlashExit4Byte(void)
{
	UINT32 u4Index, u4DualReg;
	u4Index = 0;
    if(_SetFlashWriteEnable(u4Index) != 0)
    {
        return 1;
    }

	u4DualReg = SFLASH_RREG32(SFLASH_READ_DUAL_REG);
	u4DualReg &= 0xffffffef;
	SFLASH_WREG32(SFLASH_READ_DUAL_REG, u4DualReg);
	if( _PollingReg(SFLASH_READ_DUAL_REG, 0x10) == 1) // timeout
	{
		return 1;
	}
    if( (u1MenuID == 0x01) &&(u1DevID1 == 0x02) &&(u1DevID2 == 0x19) )	//for spansion S25FL256s flash
    {
        SFLASH_WREG8(SFLASH_PRGDATA5_REG, 0x17);	//Enter EN4B cmd
        SFLASH_WREG8(SFLASH_PRGDATA4_REG, 0x00);
        SFLASH_WREG8(SFLASH_CNT_REG,16);			// Write SF Bit Count
    }
    else
    {

        SFLASH_WREG8(SFLASH_PRGDATA5_REG, 0xe9);	//EXIT EN4B cmd
        SFLASH_WREG8(SFLASH_CNT_REG,8); 			// Write SF Bit Count
    }
	_SendFlashCommand(0x4); // Write PRG
	if( _PollingReg(SFLASH_CMD_REG, 0x04) == 1) // timeout
	{
		return 1;
	}
    LOG(0, "...cdd Exit 4 bytes address!\n");
	return 0;
}

//-----------------------------------------------------------------------------
static INT32 _4BWriteBuffer(UINT32 u4Index, UINT32 u4Addr, UINT32 u4Len, const UINT8* pu1Buf)
{
    UINT32 i, j, u4BufIdx, u4Data;

    if(pu1Buf == NULL)
    {
        return 1;
    }

    ASSERT(u4Len <= SFLASH_WRBUF_SIZE);
    ASSERT((u4Addr%SFLASH_HW_ALIGNMENT) == 0);
    ASSERT((u4Len%SFLASH_HW_ALIGNMENT) == 0);
	LOG(1, "...cdd _4BWriteBuffer u4Addr = 0x%x!\n", u4Addr);

	SFLASH_WREG8(SFLASH_RADR3_REG, HiByte(HiWord(u4Addr))); // Write
    SFLASH_WREG8(SFLASH_RADR2_REG, LoByte(HiWord(u4Addr))); // Write
    SFLASH_WREG8(SFLASH_RADR1_REG, HiByte(LoWord(u4Addr))); // Write
    SFLASH_WREG8(SFLASH_RADR0_REG, LoByte(LoWord(u4Addr))); // Write

    u4BufIdx = 0;
	LOG(1, "...cdd u4Data = \n");
    for(i=0; i<u4Len; i+=4)
    {
        for(j=0; j<4; j++)
        {
            (*((UINT8 *)&u4Data + j)) = pu1Buf[u4BufIdx];
            u4BufIdx++;
        }
		LOG(1, "0x%x  ", u4Data);
		if(i % 8 == 0)
		LOG(1, "\n");
        SFLASH_WREG32(SFLASH_PP_DATA_REG, u4Data);
     }

    if(_ExecuteWriteCmd(u4Index) != 0)
    {
        return 1;
    }
#if 0 //the following code can't be used for Atmel flash.
    //--------------------------------------------------------------
    if(!_fgSFIsrEn)
    {
	    if(_WaitForWriteBusy(u4Index, SFLASH_WRITEBUSY_TIMEOUT) != 0)
	    {
	        return 1;
	    }
    }
#else
	if(_WaitForWriteBusy(u4Index, SFLASH_WRITEBUSY_TIMEOUT) != 0)
	{
	    return 1;
	}
#endif


    return 0;
}


//-----------------------------------------------------------------------------
static INT32 _4BWriteSingleByte(UINT32 u4Index, UINT32 u4Addr, UINT8 u1Data)
{
	LOG(1, "...cdd _4BWriteSingleByte u4Addr = 0x%x!\n", u4Addr);
	SFLASH_WREG8(SFLASH_RADR3_REG, HiByte(HiWord(u4Addr))); // Write
	SFLASH_WREG8(SFLASH_RADR2_REG, LoByte(HiWord(u4Addr)));
    SFLASH_WREG8(SFLASH_RADR1_REG, HiByte(LoWord(u4Addr)));
    SFLASH_WREG8(SFLASH_RADR0_REG, LoByte(LoWord(u4Addr)));

	LOG(1, "...cdd _4BWriteSingleByte u1Data = 0x%x!\n", u1Data);
    SFLASH_WREG8(SFLASH_WDATA_REG, u1Data);

    if(_ExecuteWriteCmd(u4Index) != 0)
    {
        return 1;
    }

    //--------------------------------------------------------------
    if(_WaitForWriteBusy(u4Index, SFLASH_WRITEBUSY_TIMEOUT) != 0)
    {
        return 1;
    }

    return 0;
}

//-----------------------------------------------------------------------------
/** Brief of SFLASHHW_4BWriteSector.
 *  Details of SFLASHHW_4BWriteSector (optional).
 *  @retval TRUE    Success
 *  @retval FALSE   Fail
 */
//-----------------------------------------------------------------------------
INT32 SFLASHHW_4BWriteSector(UINT32 u4Index, UINT32 u4Addr, UINT32 u4Len,
                           const UINT8* pu1Buf)
{
    UINT32 i, u4Count, u4PgAlign;
    ASSERT(u4Index < MAX_FLASHCOUNT);
    ASSERT(_arFlashChip[u4Index].u1MenuID != 0x00);
    ASSERT(u4Len <= _arFlashChip[u4Index].u4SecSize);

    if(u4Len == 0)
    {
        return 0;
    }

    if(pu1Buf == NULL)
    {
        return 1;
    }

    if(_SetFlashWriteEnable(u4Index) != 0)
    {
        return 1;
    }

    // Handle cross page
    u4PgAlign = u4Addr % SFLASH_WRBUF_SIZE;
    if(u4PgAlign != 0)
    {
        for(i=0; i<(SFLASH_WRBUF_SIZE - u4PgAlign); i++)
        {
            if(_4BWriteSingleByte(u4Index, u4Addr, *pu1Buf) != 0)
            {
                return 1;
            }
            u4Addr++;
            pu1Buf++;
            u4Len--;

            if(u4Len == 0)
            {
                return 0;
            }
        }
    }

    if(_WBEnable() != 0)
    {
        return 1;
    }

    while((INT32)u4Len > 0)
    {
        if(u4Len >= SFLASH_WRBUF_SIZE)
        {
            u4Count = SFLASH_WRBUF_SIZE;
        }
        else
        {
            // Not write-buffer alignment
            break;
        }

        if(_4BWriteBuffer(u4Index, u4Addr, u4Count, pu1Buf) != 0)
        {
            LOG(3, "Write flash error!! faddr = 0x%x, len = 0x%x\n", u4Addr, u4Count);
            if(_WBDisable() != 0)
            {
                return 1;
            }
            return 1;
        }
        u4Len -= u4Count;
        u4Addr += u4Count;
        pu1Buf += u4Count;

    }

    if(_WBDisable() != 0)
    {
        return 1;
    }

    if((INT32)u4Len > 0)
    {
        for(i=0; i<u4Len; i++)
        {
            if(_4BWriteSingleByte(u4Index, u4Addr, *pu1Buf) != 0)
            {
                if(_WBDisable() != 0)
                {
                    return 1;
                }
                return 1;
            }
            u4Addr++;
            pu1Buf++;
        }
    }

    return 0;
}



INT32 SFLASHHW_4BEraseSector(UINT32 u4Index, UINT32 u4Addr)
{
    UINT32 u4Polling;
    UINT8 u1Reg;

    ASSERT(u4Index < MAX_FLASHCOUNT);
    ASSERT(_arFlashChip[u4Index].u1MenuID != 0x00);

	LOG(0, "...cdd SFLASHHW_4BEraseSector u4Addr = 0x%x!\n", u4Addr);

    //--------------------------------------------------------------
    if(_WaitForWriteBusy(u4Index, SFLASH_WRITEBUSY_TIMEOUT) != 0)
    {
        return 1;
    }

    //--------------------------------------------------------------
    if(_SetFlashWriteEnable(u4Index) != 0)
    {
        return 1;
    }


    // Execute sector erase command
    SFLASH_WREG8(SFLASH_PRGDATA5_REG, _arFlashChip[u4Index].u1SECERASECmd);
	SFLASH_WREG8(SFLASH_PRGDATA4_REG, HiByte(HiWord(u4Addr))); // Write
    SFLASH_WREG8(SFLASH_PRGDATA3_REG, LoByte(HiWord(u4Addr))); // Write
    SFLASH_WREG8(SFLASH_PRGDATA2_REG, HiByte(LoWord(u4Addr))); // Write
    SFLASH_WREG8(SFLASH_PRGDATA1_REG, LoByte(LoWord(u4Addr))); // Write
    SFLASH_WREG8(SFLASH_CNT_REG, 40);       // Write SF Bit Count
    SFLASHHW_SwitchChip(u4Index);
    if(u4Index==0)
    {
        _SendFlashCommand(0x4);
    }
#ifdef USE_SECOND_FLASH
    else if(u4Index == 1)
    {
        _SendFlashCommand(0x44);
    }
#endif
    else
    {
        ASSERT(0);
    }

    // Can not use ISR mode, because there is not erase sector interrupt
    u4Polling = 0;
    while(1)
    {
        if(SFLASHHW_ReadFlashStatus(u4Index, &u1Reg) != 0)
        {
            SFLASHHW_SwitchChip(0);
            return 1;
        }

        if(0 == (u1Reg & 0x1))
        {
            break;
        }

        u4Polling++;
        if(u4Polling > SFLASH_ERASESECTOR_TIMEOUT)
        {
            SFLASHHW_SwitchChip(0);
            return 1;
        }

        x_thread_delay(10);
    }

    _SendFlashCommand(0x0);
    SFLASHHW_SwitchChip(0);
    return 0;
}

//-----------------------------------------------------------------------------
/** Brief of SFLASHHW_Init.
 *  Details of SFLASHHW_Init (optional).
 *  @retval TRUE    Success
 *  @retval FALSE   Fail
 */
//-----------------------------------------------------------------------------
INT32 SFLASHHW_DeInit(void)
{
#ifndef CC_5381_LOADER
    void (* pfnOldIsr)(UINT16);
    //--------------------------------------------------
    // Un-Register ISR
    //--------------------------------------------------
    VERIFY(x_sema_delete(_hSFLASHSema) == OSR_OK);

    SFLASH_WREG32(SFLASH_SF_INTREN_REG, 0x0);
    VERIFY(BIM_ClearIrq(VECTOR_FLASH));

    VERIFY(x_reg_isr(VECTOR_FLASH, NULL, &pfnOldIsr) == OSR_OK);
#endif /* CC_5381_LOADER */

  return 0;
}
//-----------------------------------------------------------------------------
/** Brief of SFLASHHW_WriteSfProtect.
 *  Details of SFLASHHW_WriteSfProtect (u4Val).
 */
//-----------------------------------------------------------------------------
void SFLASHHW_WriteSfProtect(UINT32 u4Val)
{
	LOG(3, "...cdd SFLASHHW_WriteSfProtect u4Val = 0x%x!\n", u4Val);

	SFLASH_WREG32(SFLASH_WRPROT_REG, u4Val);
}
//-----------------------------------------------------------------------------
/** Brief of SFLASHHW_ReadSfProtect.
 *  Details of SFLASHHW_ReadSfProtect ().
 */
//-----------------------------------------------------------------------------
UINT32 SFLASHHW_ReadSfProtect(void)
{
    return SFLASH_RREG32(SFLASH_WRPROT_REG);
}
//-----------------------------------------------------------------------------
/** Brief of SFLASHHW_WriteReg.
 *  Details of SFLASHHW_WriteReg .
 */
//-----------------------------------------------------------------------------
void SFLASHHW_WriteReg(UINT32 uAddr, UINT32 u4Val)
{
    SFLASH_WREG32(uAddr, u4Val);
}
//-----------------------------------------------------------------------------
/** Brief of SFLASHHW_ReadReg.
 *  Details of SFLASHHW_ReadReg ().
 */
//-----------------------------------------------------------------------------
UINT32 SFLASHHW_ReadReg(UINT32 uAddr)
{
    return SFLASH_RREG32(uAddr);
}
//-----------------------------------------------------------------------------
/** Brief of SFLASHHW_SwitchChip.
 *  Details of SFLASHHW_SwitchChip ().
 */
//-----------------------------------------------------------------------------
UINT32 SFLASHHW_SwitchChip(UINT32 uAddr)
{
UINT32 uDualReg = SFLASH_RREG32(SFLASH_READ_DUAL_REG);
    if(uAddr == 0)
    {
       uDualReg &= 0xFFFFFF7F;
       SFLASH_WREG32(SFLASH_READ_DUAL_REG, uDualReg);
    }
    else if(uAddr == 1)
    {
       uDualReg |= 0x80;
       SFLASH_WREG32(SFLASH_READ_DUAL_REG, uDualReg);
    }
    else
    {
       return SFLASH_RREG32(uAddr);
    }
    return 0;
}

#else

//-----------------------------------------------------------------------------
/** Brief of SFLASHHW_SetClk.
 *  Details of SFLASHHW_SetClk (optional).
 */
//-----------------------------------------------------------------------------
void SFLASHHW_SetClk(UINT32 u4Val)
{
    SFLASH_WREG32(SFLASH_CKGEN_REG, u4Val);
}


//-----------------------------------------------------------------------------
/** Brief of SFLASHHW_SetReadType.
 *  Details of SFLASHHW_SetReadType (optional).
 */
//-----------------------------------------------------------------------------
void SFLASHHW_SetReadType(UINT32 u4Val)
{
    UINT32 u4Reg;

    u4Reg = SFLASH_RREG32(SFLASH_CFG1_REG);

    u4Reg &= (~0x1);
    u4Reg |= (u4Val&0x1);

    SFLASH_WREG32(SFLASH_CFG1_REG, u4Reg);
}

//-----------------------------------------------------------------------------
/** Brief of SFLASHHW_SetClkDelay.
 *  Details of SFLASHHW_SetClkDelay (optional).
 */
//-----------------------------------------------------------------------------
void SFLASHHW_SetClkDelay(UINT32 u4Val)
{
    UINT32 u4Reg;

    u4Reg = SFLASH_RREG32(SFLASH_DELAY_SELECTION);

    u4Reg &= (~0xE0);
    u4Reg |= (u4Val << 5);

    SFLASH_WREG32(SFLASH_DELAY_SELECTION, u4Reg);
}

//-----------------------------------------------------------------------------
/** Brief of SFLASHHW_SetDataDelay.
 *  Details of SFLASHHW_SetDataDelay (optional).
 */
//-----------------------------------------------------------------------------
void SFLASHHW_SetDataDelay(UINT32 u4Val)
{
    UINT32 u4Reg;

    u4Reg = SFLASH_RREG32(SFLASH_DELAY_SELECTION);

    u4Reg &= (~0xE);
    u4Reg |= (u4Val << 1);

    SFLASH_WREG32(SFLASH_DELAY_SELECTION, u4Reg);
}

//-----------------------------------------------------------------------------
/** Brief of SFLASHHW_SetEnaleClkDelay.
 *  Details of SFLASHHW_SetEnaleClkDelay.
 */
//-----------------------------------------------------------------------------
void SFLASHHW_SetEnableClkDelay(BOOL fgEnable)
{
	UINT32 u4Reg;

	if(fgEnable)
	{
	   u4Reg = SFLASH_RREG32(SFLASH_DELAY_SELECTION);
	   u4Reg &= (~0x10);
	   u4Reg = u4Reg |(1 << 4);
	   SFLASH_WREG32(SFLASH_DELAY_SELECTION, u4Reg);
	}
	else
	{
	   u4Reg = SFLASH_RREG32(SFLASH_DELAY_SELECTION);
	   u4Reg &= (~0x10);
	   SFLASH_WREG32(SFLASH_DELAY_SELECTION, u4Reg);

	}
}

//-----------------------------------------------------------------------------
/** Brief of SFLASHHW_SetEnaleClkDataDelay.
 *  Details of SFLASHHW_SetEnaleClkDataDelay.
 */
//-----------------------------------------------------------------------------
void SFLASHHW_SetEnableDataDelay(BOOL fgEnable)
{
	UINT32 u4Reg;

	if(fgEnable)
	{
	   u4Reg = SFLASH_RREG32(SFLASH_DELAY_SELECTION);
	   u4Reg &= (~0x1);
	   u4Reg = u4Reg | 0x1;
	   SFLASH_WREG32(SFLASH_DELAY_SELECTION, u4Reg);
	}
	else
	{
	   u4Reg = SFLASH_RREG32(SFLASH_DELAY_SELECTION);
	   u4Reg &= (~0x1);
	   SFLASH_WREG32(SFLASH_DELAY_SELECTION, u4Reg);

	}
}


//-----------------------------------------------------------------------------
/** Brief of SFLASHHW_SetSampleEdge.
 *  Details of SFLASHHW_SetSampleEdge (optional).
 */
//-----------------------------------------------------------------------------
void SFLASHHW_SetSampleEdge(UINT32 u4Val)
{
    UINT32 u4Reg;

    u4Reg = SFLASH_RREG32(SFLASH_SAMPLE_EDGE_REG);

    u4Reg &= (~0x1);
    u4Reg |= (u4Val&0x1);

    SFLASH_WREG32(SFLASH_SAMPLE_EDGE_REG, u4Reg);

}


//-----------------------------------------------------------------------------
/** Brief of SFLASHHW_GetClk.
 *  Details of SFLASHHW_GetClk (optional).
 */
//-----------------------------------------------------------------------------
UINT32 SFLASHHW_GetClk()
{
    return (SFLASH_RREG32(SFLASH_CKGEN_REG) & 0xF);
}

//-----------------------------------------------------------------------------
/** Brief of SFLASHHW_GetClkDelay.
 *  Details of SFLASHHW_GetClkDelay (optional).
 */
//-----------------------------------------------------------------------------
UINT32 SFLASHHW_GetClkDelay()
{
	UINT32 u4Reg;
	u4Reg = SFLASH_RREG32(SFLASH_DELAY_SELECTION) & 0xE0;
	u4Reg = u4Reg >> 5;
	return u4Reg;
}

//-----------------------------------------------------------------------------
/** Brief of SFLASHHW_GetDataDelay.
 *  Details of SFLASHHW_GetDataDelay (optional).
 */
//-----------------------------------------------------------------------------
UINT32 SFLASHHW_GetDataDelay()
{
    UINT32 u4Reg;
	u4Reg = SFLASH_RREG32(SFLASH_DELAY_SELECTION) & 0xE;
	u4Reg = u4Reg >> 1;
	return u4Reg;
}


//-----------------------------------------------------------------------------
/** Brief of SFLASHHW_GetReadType.
 *  Details of SFLASHHW_GetReadType (optional).
 */
//-----------------------------------------------------------------------------
UINT32 SFLASHHW_GetReadType()
{
    return (SFLASH_RREG32(SFLASH_CFG1_REG) & 0x1);
}


//-----------------------------------------------------------------------------
/** Brief of SFLASHHW_GetSampleEdge.
 *  Details of SFLASHHW_GetSampleEdge (optional).
 */
//-----------------------------------------------------------------------------
UINT32 SFLASHHW_GetSampleEdge()
{
    return (SFLASH_RREG32(SFLASH_SAMPLE_EDGE_REG) & 0x1);
}
//-----------------------------------------------------------------------------
/** Brief of SFLASHHW_PinMux.
 *  Details of SFLASHHW_PinMux (optional).
 */
//-----------------------------------------------------------------------------
UINT32 SFLASHHW_PinMux(UINT32 u4Val)
{
    BIM_WRITE32(0x3B0, u4Val);
    return (UINT32)(BIM_READ32(0x3B0));
}

#endif

//-----------------------------------------------------------------------------
/** Brief of SFLASHHW_WriteProtect.
 *  Details of SFLASHHW_WriteProtect (optional).
 */
//-----------------------------------------------------------------------------
INT32 SFLASHHW_WriteProtect(UINT32 u4Index, BOOL fgEnable)
{

	  if(u4Index >= MAX_FLASHCOUNT)
	  {
       LOG(1, "Nor Flash Index is out fo Max Flash Count\n");
       return -1;
	  }
#ifdef REPLACE_EEPROM_WITH_NOR
    if(!_fgDoProtect)
    {
        return 0;
    }

    if(_WaitForWriteBusy(u4Index, SFLASH_WRITEBUSY_TIMEOUT) != 0)
    {
        return -1;
    }

    if(_SetFlashWriteEnable(u4Index) != 0)
    {
        return -1;
    }

	if((0xEF ==_arFlashChip[u4Index].u1MenuID) || (0xC8 ==_arFlashChip[u4Index].u1MenuID) )
	{
		if(fgEnable)
		{
			//Write status cmd 0x01 + status register 1 +  status register 2
			SFLASH_WREG8(SFLASH_PRGDATA5_REG, _arFlashChip[u4Index].u1WRSRCmd);
			SFLASH_WREG8(SFLASH_PRGDATA4_REG, _arFlashChip[u4Index].u1Protection_Eeprom);

			//SFLASH_WREG8(SFLASH_PRGDATA4_REG, 0x08);
			SFLASH_WREG8(SFLASH_PRGDATA3_REG, 0x40);

			SFLASH_WREG8(SFLASH_CNT_REG,24); // Write SF Bit Count
			if(u4Index==0)
			{
				_SendFlashCommand(0x04);
			}
			else
			{
				ASSERT(0);
			}
			if( _PollingReg(SFLASH_CMD_REG, 0x04) != 0) // timeout
			{
				return -1;
			}
			if(_WaitForWriteBusy(u4Index, SFLASH_WRITEBUSY_TIMEOUT) != 0)
			{
				return -1;
			}
			_SendFlashCommand(0x0);
			return 0;

		}
		else
		{
			//printf("....fgdisable\n");
			//Write status cmd 0x01 + status register 1 +  status register 2
			SFLASH_WREG8(SFLASH_PRGDATA5_REG, _arFlashChip[u4Index].u1WRSRCmd);
			SFLASH_WREG8(SFLASH_PRGDATA4_REG, _arFlashChip[u4Index].u1Protection);
			SFLASH_WREG8(SFLASH_PRGDATA3_REG, 0x00);
			SFLASH_WREG8(SFLASH_CNT_REG,24); // Write SF Bit Count
			if(u4Index==0)
			{
				_SendFlashCommand(0x04);
			}
			else
			{
				ASSERT(0);
			}
			if( _PollingReg(SFLASH_CMD_REG, 0x04) != 0) // timeout
			{
				return -1;
			}

			if(_WaitForWriteBusy(u4Index, SFLASH_WRITEBUSY_TIMEOUT) != 0)
			{
				return -1;
			}
			_SendFlashCommand(0x0);
			return 0;
		}
	}
    if(fgEnable)
    {
	    SFLASH_WREG8(SFLASH_PRGDATA5_REG, _arFlashChip[u4Index].u1Protection_Eeprom);
    }
    else
    {
        if(_fgNorPartialProtection)
        {
            SFLASH_WREG32(SFLASH_PRGDATA5_REG, _arFlashChip[u4Index].u1Protection);
        }
        else
        {
            SFLASH_WREG32(SFLASH_PRGDATA5_REG, 0x0);
        }
    }

    SFLASH_WREG8(SFLASH_CNT_REG,8); // Write SF Bit Count

    if(u4Index==0)
    {
        _SendFlashCommand(0x20);
    }
    else
    {
        ASSERT(0);
    }
    if(_PollingReg(SFLASH_CMD_REG, 0x20) != 0)
    {
        return -1;
    }
    if(_WaitForWriteBusy(u4Index, SFLASH_WRITEBUSY_TIMEOUT) != 0)
    {
        return -1;
    }

#else		//not  REPLACE_EEPROM_WITH_NOR
	if(_fgWriteProtect)
	{

        if(!_fgDoProtect)
        {
            return 0;
        }

        if(_WaitForWriteBusy(u4Index, SFLASH_WRITEBUSY_TIMEOUT) != 0)
        {
            return -1;
        }

        if(_SetFlashWriteEnable(u4Index) != 0)
        {
            return -1;
        }
        if(_fgAAIWrite)
        {
            LOG(0, "AAI Write Protect Enable flag %d\n", fgEnable);
            SFLASHHW_SendByte(u4Index, 0x50); // EWSR
            if(fgEnable)
            {
                SFLASHHW_Send2Byte(u4Index, _arFlashChip[u4Index].u1WRSRCmd, 0x3C); // WRSR
            }
            else
            {
                if(_fgNorPartialProtection)
                {
                  SFLASHHW_Send2Byte(u4Index, _arFlashChip[u4Index].u1WRSRCmd, _arFlashChip[u4Index].u1Protection); // WRSR
                }
                else
                {
                SFLASHHW_Send2Byte(u4Index, _arFlashChip[u4Index].u1WRSRCmd, 0x00); // WRSR
            	}
            }
            if(_WaitForWriteBusy(u4Index, SFLASH_WRITEBUSY_TIMEOUT) != 0)
            {
                return -1;
            }
        }
        if(fgEnable)
        {
            SFLASH_WREG8(SFLASH_PRGDATA5_REG, 0x3C);
        }
        else
        {
            if(_fgNorPartialProtection)//_fgNorPartialProtection is TV used only
            {
                SFLASH_WREG32(SFLASH_PRGDATA5_REG, _arFlashChip[u4Index].u1Protection);
            }
            else
            {
                SFLASH_WREG32(SFLASH_PRGDATA5_REG, 0x0);
            }
        }

        SFLASH_WREG8(SFLASH_CNT_REG,8); // Write SF Bit Count

        if(u4Index==0)
        {
            _SendFlashCommand(0x20);
        }

    #ifdef USE_SECOND_FLASH
        else if(u4Index == 1)
        {
            _SendFlashCommand(0x60);
        }
    #endif

        else
        {
            ASSERT(0);
        }

        if(_PollingReg(SFLASH_CMD_REG, 0x20) != 0)
        {
            return -1;
        }

        if(_WaitForWriteBusy(u4Index, SFLASH_WRITEBUSY_TIMEOUT) != 0)
        {
            return -1;
        }
	}
else
{
    UNUSED(u4Index);
    UNUSED(fgEnable);
}
#endif
    return 0;
}


//-----------------------------------------------------------------------------
/** Brief of SFLASHHW_GetFlashName.
 *  Details of SFLASHHW_GetFlashName (optional).
 */
//-----------------------------------------------------------------------------
CHAR* SFLASHHW_GetFlashName(UINT8 u1MenuID, UINT8 u1DevID1, UINT8 u1DevID2)
{
    CHAR *pStr;
    UINT32 i;

    i = 0;
    while(_aVendorFlash[i].u1MenuID != (UINT8)0x0)
    {
        if( (_aVendorFlash[i].u1MenuID == u1MenuID) &&
            (_aVendorFlash[i].u1DevID1 == u1DevID1) &&
            (_aVendorFlash[i].u1DevID2 == u1DevID2))
        {
            pStr = _aVendorFlash[i].pcFlashStr;
            return pStr;
        }
        i++;
    }

    return NULL;
}

//-----------------------------------------------------------------------------
/** Brief of SFLASHHW_GetFlashSize.
 *  Details of SFLASHHW_GetFlashSize (optional).
 */
//-----------------------------------------------------------------------------
UINT32 SFLASHHW_GetFlashSize(UINT8 u1MenuID, UINT8 u1DevID1, UINT8 u1DevID2)
{
    UINT32 u4Size;
    UINT32 i;

    i = 0;
    while(_aVendorFlash[i].u1MenuID != (UINT8)0x0)
    {
        if( (_aVendorFlash[i].u1MenuID == u1MenuID) &&
            (_aVendorFlash[i].u1DevID1 == u1DevID1) &&
            (_aVendorFlash[i].u1DevID2 == u1DevID2))
        {
            u4Size = _aVendorFlash[i].u4ChipSize;
            return u4Size;
        }
        i++;
    }

    return 0;
}


//-----------------------------------------------------------------------------
/** Brief of SFLASHHW_DMA.
 *  Details of SFLASHHW_DMA (optional).
 */
//-----------------------------------------------------------------------------

INT32 SFLASHHW_EnableDMA(UINT32 u4SrcAddr, UINT32 u4DestAddr, UINT32 u4Size)
{
    UINT32 u4DRAMStart, u4DRAMEnd;
    UINT32 u4Reg, u4BinReg, u4ResetVal;
#ifdef BD_NOR_ENABLE
	HAL_TIME_T time1={0}, time2={0}, result_time={0};
	HAL_GetTime(&time1);
    u4BinReg   = 0x718;
	u4ResetVal = 0x4;
#else
    u4BinReg   = 0x5A0;
    u4ResetVal = 0;
#endif

    if(((u4DestAddr%16)!=0) || ((u4Size%16)!=0))
    {
        LOG(5, "Address or size is not 16-byte alignment!\n");
        return 1;
    }
	LOG(0, "...cdd SFLASHHW_EnableDMA u4SrcAddr = 0x%x u4DestAddr = 0x%x\n", u4SrcAddr, u4DestAddr);

    if(u4SrcAddr > PBI_A_BASE)
    {
        u4SrcAddr -= PBI_A_BASE;
    }

	// Do reset
	BIM_WRITE32(u4BinReg, 0);
	BIM_WRITE32(u4BinReg, 0x2);
	BIM_WRITE32(u4BinReg, u4ResetVal);

	// Flash source address
	BIM_WRITE32(u4BinReg + 4, u4SrcAddr);

	// DRAM destination address
	u4DRAMStart = u4DestAddr & (~(0xF));
	u4DRAMEnd = (u4DestAddr + u4Size) & (~(0xF));
	BIM_WRITE32(u4BinReg + 8, u4DRAMStart);
	BIM_WRITE32(u4BinReg + 12, u4DRAMEnd);

	// Enable
#if defined(CC_MT5391) || defined(BD_NOR_ENABLE)
    BIM_WRITE32(u4BinReg, 0x5);	// MT5391
#else
    BIM_WRITE32(0x5A0, 0x1);
#endif

	while(1)
	{
		u4Reg = BIM_READ32(u4BinReg);
        if((u4Reg&0x1) == 0)
        {
            break;
        }
	}
#if !defined (CC_MTK_LOADER)&& !defined(CC_MTK_LOADER) && !defined(BD_NOR_ENABLE)
    HalFlushInvalidateDCache();
#endif
#ifdef BD_NOR_ENABLE
	HAL_GetTime(&time2);
    HAL_GetDeltaTime(&result_time, &time1, &time2);
    LOG(0, "dma read 0x%XBytes elapse time: %d sec, %d usec\n", u4Size,
                                    result_time.u4Seconds, result_time.u4Micros);
#endif
	return 0;
}


#endif/*CC_NOR_DISABLE*/

