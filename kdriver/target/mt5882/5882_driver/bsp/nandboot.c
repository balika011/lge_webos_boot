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
 * $RCSfile: nandboot.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file nandboot.c
 *  nandboot.c provides the low level register access function of nand device.
 */
 
#if defined(CC_NAND_BOOT) && defined(CC_MTK_PRELOADER)

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#include "x_typedef.h"
#include "x_assert.h"
#include "x_bim.h"
#include "nandboot.h"
#include "chip_txt.h"

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
#define NAND_SUCC               (0)
#define NAND_FAIL               (1)
#define NAND_ECC_ERROR          (4)
#define NAND_CMD_ERROR          (8)
#define NAND_INVALID_ARG        (16)
#define POLLING_RETRY           (512*512)
#define NAND_SCRAMBLE_EN            (0)
#define NAND_RANDOM_EN              (1)
#define NAND_AUTORD_DONE            (0)

#if CC_NAND_60BIT_NFI
   #if NAND_RANDOM_EN
   static  BOOL fgRandomizer = FALSE;
   static UINT16 RANDOM_SEED[256]=
   {
    //for page 0~127
        0x576A, 0x05E8, 0x629D, 0x45A3, 0x649C, 0x4BF0, 0x2342, 0x272E,
        0x7358, 0x4FF3, 0x73EC, 0x5F70, 0x7A60, 0x1AD8, 0x3472, 0x3612,
        0x224F, 0x0454, 0x030E, 0x70A5, 0x7809, 0x2521, 0x484F, 0x5A2D,
        0x492A, 0x043D, 0x7F61, 0x3969, 0x517A, 0x3B42, 0x769D, 0x0647,
        0x7E2A, 0x1383, 0x49D9, 0x07B8, 0x2578, 0x4EEC, 0x4423, 0x352F,
        0x5B22, 0x72B9, 0x367B, 0x24B6, 0x7E8E, 0x2318, 0x6BD0, 0x5519,
        0x1783, 0x18A7, 0x7B6E, 0x7602, 0x4B7F, 0x3648, 0x2C53, 0x6B99,
        0x0C23, 0x67CF, 0x7E0E, 0x4D8C, 0x5079, 0x209D, 0x244A, 0x747B,
        0x350B, 0x0E4D, 0x7004, 0x6AC3, 0x7F3E, 0x21F5, 0x7A15, 0x2379,
        0x1517, 0x1ABA, 0x4E77, 0x15A1, 0x04FA, 0x2D61, 0x253A, 0x1302,
        0x1F63, 0x5AB3, 0x049A, 0x5AE8, 0x1CD7, 0x4A00, 0x30C8, 0x3247,
        0x729C, 0x5034, 0x2B0E, 0x57F2, 0x00E4, 0x575B, 0x6192, 0x38F8,
        0x2F6A, 0x0C14, 0x45FC, 0x41DF, 0x38DA, 0x7AE1, 0x7322, 0x62DF,
        0x5E39, 0x0E64, 0x6D85, 0x5951, 0x5937, 0x6281, 0x33A1, 0x6A32,
        0x3A5A, 0x2BAC, 0x743A, 0x5E74, 0x3B2E, 0x7EC7, 0x4FD2, 0x5D28,
        0x751F, 0x3EF8, 0x39B1, 0x4E49, 0x746B, 0x6EF6, 0x44BE, 0x6DB7,
   //for page 128~255
        0x576A, 0x05E8, 0x629D, 0x45A3, 0x649C, 0x4BF0, 0x2342, 0x272E,
        0x7358, 0x4FF3, 0x73EC, 0x5F70, 0x7A60, 0x1AD8, 0x3472, 0x3612,
        0x224F, 0x0454, 0x030E, 0x70A5, 0x7809, 0x2521, 0x484F, 0x5A2D,
        0x492A, 0x043D, 0x7F61, 0x3969, 0x517A, 0x3B42, 0x769D, 0x0647,
        0x7E2A, 0x1383, 0x49D9, 0x07B8, 0x2578, 0x4EEC, 0x4423, 0x352F,
        0x5B22, 0x72B9, 0x367B, 0x24B6, 0x7E8E, 0x2318, 0x6BD0, 0x5519,
        0x1783, 0x18A7, 0x7B6E, 0x7602, 0x4B7F, 0x3648, 0x2C53, 0x6B99,
        0x0C23, 0x67CF, 0x7E0E, 0x4D8C, 0x5079, 0x209D, 0x244A, 0x747B,
        0x350B, 0x0E4D, 0x7004, 0x6AC3, 0x7F3E, 0x21F5, 0x7A15, 0x2379,
        0x1517, 0x1ABA, 0x4E77, 0x15A1, 0x04FA, 0x2D61, 0x253A, 0x1302,
        0x1F63, 0x5AB3, 0x049A, 0x5AE8, 0x1CD7, 0x4A00, 0x30C8, 0x3247,
        0x729C, 0x5034, 0x2B0E, 0x57F2, 0x00E4, 0x575B, 0x6192, 0x38F8,
        0x2F6A, 0x0C14, 0x45FC, 0x41DF, 0x38DA, 0x7AE1, 0x7322, 0x62DF,
        0x5E39, 0x0E64, 0x6D85, 0x5951, 0x5937, 0x6281, 0x33A1, 0x6A32,
        0x3A5A, 0x2BAC, 0x743A, 0x5E74, 0x3B2E, 0x7EC7, 0x4FD2, 0x5D28,
        0x751F, 0x3EF8, 0x39B1, 0x4E49, 0x746B, 0x6EF6, 0x44BE, 0x6DB7
   };
   #endif   
#endif
//-----------------------------------------------------------------------------
// static variables
//-----------------------------------------------------------------------------
static BOOL _fgIsrEn = FALSE;
static BOOL _fgDmaEn = FALSE;
static NAND_ECC_TYPE_T _rECCType = NAND_ECC_SOFT;

static NAND_FLASH_DEV_T *_prFlashDev;
static UINT32 _u4BlkPgCount;
static UINT32 _u4FdmNum;
static UINT32 _u4SectorNum;
static UINT32 _u4PgAddrLen;
static UINT32 _u4TotalAddrLen;
static UINT32 _u4PageSize = 2048;
static UINT32 _u4OOBSize = 16;
static UINT32 _u4SectorSize = 1024;
static UINT32 _u4ECCBits = 0;
BOOL _fgNandInit = FALSE;

UINT8 pu1Spare[NAND_MAX_OOB_SIZE];
UINT8 pu1Data[(NAND_MAX_PAGE_SIZE + NAND_MAX_OOB_SIZE)/8];

#ifndef CC_SECURE_BOOT_SCRAMBLE
#define ROMNANDTAG_0 0x2F4B544D
#define ROMNANDTAG_1 0x2F565444
#define ROMNANDTAG_2 0x434D4F52
#define ROMNANDTAG_3 0x2F45444F
#define ROMNANDTAG_4 0x444E414E
#define ROMNANDTAG_5 0x544F4F42
#define ROMNANDTAG_6 0x3130562F
#define ROMNANDTAG_7 0x0030302E

const static UINT32 _u4Signature[] __attribute__ ((section ("ROMNANDTAG"))) =
{
    ROMNANDTAG_0, ROMNANDTAG_1, ROMNANDTAG_2, ROMNANDTAG_3,     // MTK/DTV/ROMCODE/
    ROMNANDTAG_4, ROMNANDTAG_5, ROMNANDTAG_6, ROMNANDTAG_7,     // NANDBOOT/V01.00
    0,            0,            0,            0,
    0,            0,            0,            0,
    
#if defined(CC_NANDBOOT_SUPPORT_SMALL_BBM0)
    ROMNANDTAG_5, 4,            2,            0x200,
    0x10,         0,            4,            3,
    0x200,        0x10,         0x20,         0,
    0,            0,            0x195B2C7A,   0xEF81A6AF
#else
    ROMNANDTAG_7, 4,            2,            0x200,
    0x10,         0,            5,            3,
    0x800,        0x10,         0,            0,
    0,            0,            0x4D245937,   0x9B628D7C
#endif
};
#else //  CC_SECURE_BOOT_SCRAMBLE
#define ROMNANDTAG_0 0xaf19074c
#define ROMNANDTAG_1 0xaf6d0768
#define ROMNANDTAG_2 0x1f016b31
#define ROMNANDTAG_3 0xaf214744
#define ROMNANDTAG_4 0x030d5341
#define ROMNANDTAG_5 0x43096b71
#define ROMNANDTAG_6 0xd6f50ec4
#define ROMNANDTAG_7 0x12f496c0

const static UINT32 _u4Signature[] __attribute__ ((section ("ROMNANDTAG"))) =
{
    ROMNANDTAG_0, ROMNANDTAG_1, ROMNANDTAG_2, ROMNANDTAG_3,     // MTK/DTV/ROMCODE/
    ROMNANDTAG_4, ROMNANDTAG_5, ROMNANDTAG_6, ROMNANDTAG_7,     // NANDBOOT/V01.00
    0x12345678, 0x12345678, 0x12345678, 0x12345678,
    0x12345678, 0x12345678, 0x12345678, 0x12345678,

#if defined(CC_NANDBOOT_SUPPORT_SMALL_BBM0)
    ROMNANDTAG_5, 0x12345668, 0x12345670, 0x12345e78,
    0x12345638, 0x12345678, 0x12345668, 0x12345674,
    0x12345e78, 0x12345638, 0x123456f8, 0x12345678,
    0x12345678, 0x12345678, 0x7758e790, 0xac32ccc7
#else
    ROMNANDTAG_7, 0x12345668, 0x12345670, 0x12345e78,
    0x12345638, 0x12345678, 0x1234566c, 0x12345674,
    0x12347678, 0x12345638, 0x12345678, 0x12345678,
    0x12345678, 0x12345678, 0x26a532a5, 0x7fbe638a
#endif
};
#endif //  CC_SECURE_BOOT_SCRAMBLE

//-----------------------------------------------------------------------------
// static functions
//-----------------------------------------------------------------------------
void CHIP_DisplayString(const CHAR *szString);
void CHIP_DisplayHex(UINT32);

//-----------------------------------------------------------------------------
/** CHIP_NandShowHex()
 */
//-----------------------------------------------------------------------------
VOID CHIP_NandShowHex(const CHAR *pu1Str, UINT32 u4Hex)
{
    CHIP_DisplayString(pu1Str);
    CHIP_DisplayHex(u4Hex);
    CHIP_DisplayString("\n");
}

//-----------------------------------------------------------------------------
/** _NANDHW_countbits()
 */
//-----------------------------------------------------------------------------
static UINT32 _NANDHW_countbits(UINT32 byte)
{
    UINT32 res = 0;

    for (; byte; byte >>= 1)
    {
        res += (byte & 0x01);
    }
    
    return res;
}
#if defined(CC_MT5399) || defined(CC_MT5882)
//-----------------------------------------------------------------------------
/** _NANDHW_SetEptNum()
 */
//-----------------------------------------------------------------------------
static void _NANDHW_SetEptNum(UINT32 u4EmptyNum)
{
#ifndef TOOLDRIVER
     NAND_WRITE32(NAND_NFI_FIFOSTA,NAND_NFI_EMPTY_BIT_NUM(u4EmptyNum));
#endif
}
#endif
//-----------------------------------------------------------------------------
/** _NANDHW_WaitRead()
 */
//-----------------------------------------------------------------------------
static void _NANDHW_WaitRead(UINT32 u4WaitNum)
{
    UINT32 u4Val;
    
    do
    {
        u4Val = NAND_READ32(NAND_NFI_FIFOSTA);
        u4Val &= NAND_NFI_FIFOSTA_RD_REMAIN;
    } while (u4Val < u4WaitNum);
}

//-----------------------------------------------------------------------------
/** _NANDHW_WaitEccDone() must wait done
 */
//-----------------------------------------------------------------------------
static void _NANDHW_WaitEccDone(UINT32 u4SectIdx)
{
    UINT32 u4Val;
    
    do
    {
        u4Val = NAND_READ32(NAND_NFIECC_DECDONE);
        u4Val = (u4Val >> u4SectIdx) & 0x1;
    } while (u4Val == 0);
}

//-----------------------------------------------------------------------------
/** _NANDHW_WaitBusy()
 */
//-----------------------------------------------------------------------------
static void _NANDHW_WaitBusy(void)
{
    UINT32 u4Val;
    
    do
    {
        u4Val = NAND_READ32(NAND_NFI_STA);
        u4Val &= 0xFF000DFF;  // bit8=1: ready to busy
    } while (u4Val != 0);
}

//-----------------------------------------------------------------------------
/** _NANDHW_NFIECC_WaitBusy()
 */
//-----------------------------------------------------------------------------
static void _NANDHW_NFIECC_WaitBusy(void)
{
    UINT32 u4Val;
    
    do
    {
        u4Val = NAND_READ32(NAND_NFIECC_ENCIDLE);
    } while (u4Val == 0);
    
    do
    {
        u4Val = NAND_READ32(NAND_NFIECC_DECIDLE);
    } while (u4Val == 0);
}

//-----------------------------------------------------------------------------
/** _NANDHW_NFI_WaitSectorCnt()
 */
//-----------------------------------------------------------------------------
static void _NANDHW_NFI_WaitSectorCnt(void)
{
    UINT32 u4Val, u4Cnt;
    
    do
    {
        u4Val = NAND_READ32(NAND_NFI_ADDRCNTR);
#if defined(CC_MT5399) || defined(CC_MT5882)
        u4Val &= 0xF800;
#else
        u4Val &= 0xF000;
#endif
        u4Cnt = NAND_NFI_ADDRCNTR_SEC_CNTR(_u4SectorNum);
    } while (u4Val != u4Cnt);
}

//-----------------------------------------------------------------------------
/** _NANDHW_Command()
 */
//-----------------------------------------------------------------------------
static void _NANDHW_Command(UINT32 u4Cmd)
{
    NAND_WRITE32(NAND_NFI_CMD, u4Cmd);
    _NANDHW_WaitBusy();
}

//-----------------------------------------------------------------------------
/** _NANDHW_Addr()
 */
//-----------------------------------------------------------------------------
static void _NANDHW_Addr(UINT32 u4RowAddr, UINT32 u4ColAddr, UINT32 u4ByteCount)
{
    NAND_WRITE32(NAND_NFI_COLADDR, u4ColAddr);
    NAND_WRITE32(NAND_NFI_ROWADDR, u4RowAddr);
    NAND_WRITE32(NAND_NFI_ADDRNOB, u4ByteCount);
    _NANDHW_WaitBusy();
}

//-----------------------------------------------------------------------------
/** _NANDHW_NFI_Reset()
 */
//-----------------------------------------------------------------------------
static void _NANDHW_NFI_Reset(void)
{
    NAND_WRITE32(NAND_NFI_CNFG, 0);
    NAND_WRITE32(NAND_NFI_CON, NAND_NFI_CON_NFI_RST | NAND_NFI_CON_FIFO_FLUSH);
    _NANDHW_WaitBusy();
}

//-----------------------------------------------------------------------------
/** _NANDHW_NFIECC_Reset()
 */
//-----------------------------------------------------------------------------
static void _NANDHW_NFIECC_Reset(void)
{
    NAND_WRITE32(NAND_NFIECC_ENCON, 0);
    NAND_WRITE32(NAND_NFIECC_DECCON, 0);
    _NANDHW_NFIECC_WaitBusy();
}

//-----------------------------------------------------------------------------
/** _NANDHW_NFI_SetPageFmt()
 */
//-----------------------------------------------------------------------------
static void _NANDHW_NFI_SetPageFmt(void)
{
    UINT32 u4Val = 0;
    _u4FdmNum = 8;
    
#if CC_NAND_60BIT_NFI
    if (_u4OOBSize == 16)
    {
        u4Val = NAND_NFI_PAGEFMT_SPARE_16;
        if (_u4PageSize == 512)
        {
            u4Val |= NAND_NFI_PAGEFMT_PAGE_SIZE_512_2k;
            u4Val |= NAND_NFI_PAGEFMT_SECTOR_SIZE_512_16;
        }
        else if (_u4PageSize == 2048)
        {
            u4Val |= NAND_NFI_PAGEFMT_PAGE_SIZE_512_2k;
            u4Val |= NAND_NFI_PAGEFMT_SECTOR_SIZE_1024_32;
        }
        else if (_u4PageSize == 4096)
        {
            u4Val |= NAND_NFI_PAGEFMT_PAGE_SIZE_4k;
            u4Val |= NAND_NFI_PAGEFMT_SECTOR_SIZE_1024_32;
        }
        else if (_u4PageSize == 8192)
        {
            u4Val |= NAND_NFI_PAGEFMT_PAGE_SIZE_8k;
            u4Val |= NAND_NFI_PAGEFMT_SECTOR_SIZE_1024_32;
        }
        else if (_u4PageSize == 16384)
        {
            u4Val |= NAND_NFI_PAGEFMT_PAGE_SIZE_16k;
            u4Val |= NAND_NFI_PAGEFMT_SECTOR_SIZE_1024_32;
        }
    }
    else if (_u4OOBSize == 26)
    {
        u4Val = NAND_NFI_PAGEFMT_SPARE_26;
        u4Val |= NAND_NFI_PAGEFMT_SECTOR_SIZE_1024_52;
    }
    else if (_u4OOBSize == 32)
    {
        u4Val = NAND_NFI_PAGEFMT_SPARE_32;
        u4Val |= NAND_NFI_PAGEFMT_SECTOR_SIZE_1024_64;
    }
    else if (_u4OOBSize == 36)
    {
        u4Val = NAND_NFI_PAGEFMT_SPARE_36;
        u4Val |= NAND_NFI_PAGEFMT_SECTOR_SIZE_1024_72;
    }
    else if (_u4OOBSize == 40)
    {
        u4Val = NAND_NFI_PAGEFMT_SPARE_40;
        u4Val |= NAND_NFI_PAGEFMT_SECTOR_SIZE_1024_80;
    }
    else if (_u4OOBSize == 64)
    {
        u4Val = NAND_NFI_PAGEFMT_SPARE_64;
        u4Val |= NAND_NFI_PAGEFMT_SECTOR_SIZE_1024_128;
    }

    if (_u4PageSize == 4096)
    {
        u4Val |= NAND_NFI_PAGEFMT_PAGE_SIZE_4k;            
    }
    else if (_u4PageSize == 8192)
    {
        u4Val |= NAND_NFI_PAGEFMT_PAGE_SIZE_8k;
    }
    else if (_u4PageSize == 16384)
    {
        u4Val |= NAND_NFI_PAGEFMT_PAGE_SIZE_16k;
    }  
  
#else
    if (_u4OOBSize == 16)
    {
        u4Val = NAND_NFI_PAGEFMT_SPARE_16;
    }
    else
    {
        u4Val = NAND_NFI_PAGEFMT_SPARE_26;
    }

    if (_u4PageSize == 4096)
    {
        u4Val |= NAND_NFI_PAGEFMT_PAGE_SIZE_4k;
    }
    else if (_u4PageSize == 8192)
    {
        u4Val |= NAND_NFI_PAGEFMT_PAGE_SIZE_8k;
    }
    else
    {
        u4Val |= NAND_NFI_PAGEFMT_PAGE_SIZE_512_2k;
    }
#endif
    u4Val |= NAND_NFI_PAGEFMT_FDM_NUM(8) | NAND_NFI_PAGEFMT_ECC_NUM(8);

    NAND_WRITE32(NAND_NFI_PAGEFMT, u4Val);
}

//-----------------------------------------------------------------------------
/** _NANDHW_NFI_Cfg()
 */
//-----------------------------------------------------------------------------
static void _NANDHW_NFI_Cfg(NAND_ECC_TYPE_T rECCType, BOOL fgDmaEn, UINT32 u4OpMode)
{
    UINT32 u4Val = 0;
    
    if (rECCType == NAND_ECC_HARD)
    {
        u4Val = NAND_NFI_CNFG_AUTO_FMT_EN | NAND_NFI_CNFG_HW_ECC_EN;
    }
    else if (rECCType == NAND_ECC_SOFT)
    {
        u4Val = NAND_NFI_CNFG_HW_ECC_EN;
    }
    else
    {
        u4Val = 0;
    }
    
    if (fgDmaEn)
    {
        u4Val |= (NAND_NFI_CNFG_AUTO_FMT_EN | NAND_NFI_CNFG_AHB_MODE);
    }
    
    switch (u4OpMode)
    {
    case NAND_NFI_CNFG_OP_IDLE:
        NAND_WRITE32(NAND_NFI_CNFG, 0);
        return;
        
    case NAND_NFI_CNFG_OP_SINGLE:
    case NAND_NFI_CNFG_OP_READ:
        u4Val |= (NAND_NFI_CNFG_READ_MODE | u4OpMode);
        break;
        
    case NAND_NFI_CNFG_OP_PROGRAM:
    case NAND_NFI_CNFG_OP_ERASE:
    case NAND_NFI_CNFG_OP_RESET:
    case NAND_NFI_CNFG_OP_CUSTOM:
        u4Val |= u4OpMode;
        break;
        
    default:
        break;
    }
    
    if (_u4PageSize == 512)
    {
        u4Val |= NAND_NFI_CNFG_SEL_SEC_512BYTE;
    }
        
    NAND_WRITE32(NAND_NFI_CNFG, u4Val);
}

//-----------------------------------------------------------------------------
/** _NANDHW_NFIECC_Cfg()
 */
//-----------------------------------------------------------------------------
static void _NANDHW_NFIECC_Cfg(NAND_ECC_TYPE_T rECCType)
{
    UINT32 u4EncMsg = 0, u4DecMsg = 0;

    if (_u4OOBSize == 16)
    {
        if (_u4PageSize == 512)
        {
            _u4ECCBits = 4;
            u4EncMsg = NAND_NFIECC_ENCCNFG_ENC_TNUM_4 | NAND_NFIECC_ENCCNFG_ENC_MS_520;
            u4DecMsg = NAND_NFIECC_DECCNFG_DEC_TNUM_4 | NAND_NFIECC_DECCNFG_DEC_CS_520_4;
        }
        else
        {
            _u4ECCBits = 12;
            u4EncMsg = NAND_NFIECC_ENCCNFG_ENC_TNUM_12 | NAND_NFIECC_ENCCNFG_ENC_MS_1032;
            u4DecMsg = NAND_NFIECC_DECCNFG_DEC_TNUM_12 | NAND_NFIECC_DECCNFG_DEC_CS_1032_12;
        }
    }
    else if(_u4OOBSize == 26)
    {
        if (_u4PageSize == 512)
        {
            _u4ECCBits = 10;
            u4EncMsg = NAND_NFIECC_ENCCNFG_ENC_TNUM_10 | NAND_NFIECC_ENCCNFG_ENC_MS_520;
            u4DecMsg = NAND_NFIECC_DECCNFG_DEC_TNUM_10 | NAND_NFIECC_DECCNFG_DEC_CS_520_10;
        }
        else
        {
            _u4ECCBits = 24;
            u4EncMsg = NAND_NFIECC_ENCCNFG_ENC_TNUM_24 | NAND_NFIECC_ENCCNFG_ENC_MS_1032;
            u4DecMsg = NAND_NFIECC_DECCNFG_DEC_TNUM_24 | NAND_NFIECC_DECCNFG_DEC_CS_1032_24;
        }
    }
#if CC_NAND_60BIT_NFI
    else if(_u4OOBSize == 32)
    {
         _u4ECCBits = 32;
         u4EncMsg = NAND_NFIECC_ENCCNFG_ENC_TNUM_32 | NAND_NFIECC_ENCCNFG_ENC_MS_1032;
         u4DecMsg = NAND_NFIECC_DECCNFG_DEC_TNUM_32 | NAND_NFIECC_DECCNFG_DEC_CS_1032_32;
    }
    else if(_u4OOBSize == 36)
    {
         _u4ECCBits = 36;
         u4EncMsg = NAND_NFIECC_ENCCNFG_ENC_TNUM_36 | NAND_NFIECC_ENCCNFG_ENC_MS_1032;
         u4DecMsg = NAND_NFIECC_DECCNFG_DEC_TNUM_36 | NAND_NFIECC_DECCNFG_DEC_CS_1032_36;
    }
    else if(_u4OOBSize == 40)
    {
         _u4ECCBits = 40;
         u4EncMsg = NAND_NFIECC_ENCCNFG_ENC_TNUM_40 | NAND_NFIECC_ENCCNFG_ENC_MS_1032;
         u4DecMsg = NAND_NFIECC_DECCNFG_DEC_TNUM_40 | NAND_NFIECC_DECCNFG_DEC_CS_1032_40;
    }
    else if(_u4OOBSize == 64)
    {
         _u4ECCBits = 60;
         u4EncMsg = NAND_NFIECC_ENCCNFG_ENC_TNUM_60 | NAND_NFIECC_ENCCNFG_ENC_MS_1032;
         u4DecMsg = NAND_NFIECC_DECCNFG_DEC_TNUM_60 | NAND_NFIECC_DECCNFG_DEC_CS_1032_60;
    }
    /**/
#endif

    u4EncMsg |= NAND_NFIECC_ENCCNFG_ENC_NFI_MODE;
    u4DecMsg |= NAND_NFIECC_DECCNFG_DEC_CS_EMPTY_EN | NAND_NFIECC_DECCNFG_DEC_NFI_MODE;

    if (rECCType == NAND_ECC_HARD)
    {
        u4DecMsg |= NAND_NFIECC_DECCNFG_DEC_CON_AUTO;
    }
    else if (rECCType == NAND_ECC_SOFT)
    {
        u4DecMsg |= NAND_NFIECC_DECCNFG_DEC_CON_SOFT;
    }
    else if (rECCType == NAND_ECC_NONE)
    {
        u4DecMsg |= NAND_NFIECC_DECCNFG_DEC_CON_NONE;
    }
    else
    {
        ASSERT(0);
    }
    
    NAND_WRITE32(NAND_NFIECC_ENCCNFG, u4EncMsg);
    NAND_WRITE32(NAND_NFIECC_DECCNFG, u4DecMsg);
}

//-----------------------------------------------------------------------------
/** _NANDHW_NFI_Trig()
 */
//-----------------------------------------------------------------------------
static void _NANDHW_NFI_Trig(UINT32 u4OpMode)
{
    UINT32 u4Val;
    
    switch (u4OpMode)
    {
    case NAND_NFI_CNFG_OP_SINGLE:
        u4Val = NAND_NFI_CON_NOB(4) | NAND_NFI_CON_SRD;
        break;
        
    case NAND_NFI_CNFG_OP_READ:
#if NAND_AUTORD_DONE
        u4Val = NAND_NFI_CON_SEC_NUM(_u4SectorNum) | NAND_NFI_CON_BRD | NAND_NFI_CON_BRD_HW_EN;
#else
        u4Val = NAND_NFI_CON_SEC_NUM(_u4SectorNum) | NAND_NFI_CON_BRD;
#endif
        break;
        
    case NAND_NFI_CNFG_OP_PROGRAM:
        u4Val = NAND_NFI_CON_SEC_NUM(_u4SectorNum) | NAND_NFI_CON_BWR;
        break;
        
    case NAND_NFI_CNFG_OP_CUSTOM:
        u4Val = NAND_NFI_CON_SEC_NUM(_u4SectorNum) | NAND_NFI_CON_BWR;
        NAND_WRITE32(NAND_NFI_CON, u4Val);
        NAND_WRITE32(NAND_NFI_STRDATA, NAND_NFI_STRDATA_STRDATA);
        return;
        
    default:
        u4Val = 0;
        break;
  }
    
  NAND_WRITE32(NAND_NFI_CON, u4Val);
}

//-----------------------------------------------------------------------------
/** _NANDHW_NFIECC_Trig()
 */
//-----------------------------------------------------------------------------
static void _NANDHW_NFIECC_Trig(UINT32 u4OpMode)
{
    switch (u4OpMode)
    {
    case NAND_NFI_CNFG_OP_READ:
        NAND_WRITE32(NAND_NFIECC_DECCON, 1);
        break;

    case NAND_NFI_CNFG_OP_PROGRAM:
        NAND_WRITE32(NAND_NFIECC_ENCON, 1);
        break;
    
    default:
        break;
    }
}

//-----------------------------------------------------------------------------
/** _NANDHW_NFI_CheckOpMode()
 */
//-----------------------------------------------------------------------------
static INT32 _NANDHW_NFI_CheckOpMode(UINT32 u4OpMode)
{
    UINT32 i, u4Val;
    
    for (i = 0; i < 3; i++)
    {
        switch (u4OpMode)
        {
        case NAND_NFI_CNFG_OP_READ:
            u4Val = NAND_READ32(NAND_NFI_STA);
            u4Val &= NAND_NFI_STA_NFI_FSM;
            if (u4Val == NAND_NFI_STA_NFI_FSM_READ_DATA)
            {
                return NAND_SUCC;
            }
            break;
            
       default:
            return NAND_INVALID_ARG;
       }
   }
    
   return NAND_CMD_ERROR;
}

//-----------------------------------------------------------------------------
/** _NANDHW_NFIECC_GetErrNum()
 */
//-----------------------------------------------------------------------------
static INT32 _NANDHW_NFIECC_GetErrNum(UINT32 u4SectIdx, UINT32 *pu4ErrNum)
{
    UINT32 u4Val;
    ASSERT(pu4ErrNum != NULL);
    
    if (u4SectIdx < 4)
    {
        u4Val = NAND_READ32(NAND_NFIECC_DECENUM0);
#if CC_NAND_60BIT_NFI
        *pu4ErrNum = (u4Val >> (u4SectIdx*8)) & 0x3F;
#else
        *pu4ErrNum = (u4Val >> (u4SectIdx*8)) & 0x1F;
#endif
    }
    else if ((u4SectIdx >= 4) && (u4SectIdx < 8))
    {
        u4Val = NAND_READ32(NAND_NFIECC_DECENUM1);
#if CC_NAND_60BIT_NFI
        *pu4ErrNum = (u4Val >> ((u4SectIdx - 4)*8)) & 0x3F;
#else
        *pu4ErrNum = (u4Val >> ((u4SectIdx - 4)*8)) & 0x1F;
#endif
    }
    else if ((u4SectIdx >= 8) && (u4SectIdx < 12))
    {
        u4Val = NAND_READ32(NAND_NFIECC_DECENUM2);
        *pu4ErrNum = (u4Val >> ((u4SectIdx - 8)*8)) & 0x3F;
    }
    else
    {
        u4Val = NAND_READ32(NAND_NFIECC_DECENUM3);
        *pu4ErrNum = (u4Val >> ((u4SectIdx - 12)*8)) & 0x3F;
    }
#if CC_NAND_60BIT_NFI    
    if (*pu4ErrNum == 0x3F)
    {
        return NAND_ECC_ERROR;
    }
#else
    if (*pu4ErrNum == 0x1F)
    {
        return NAND_ECC_ERROR;
    }
#endif
    else
    {
        return NAND_SUCC;
    }
}

//-----------------------------------------------------------------------------
/** _NANDHW_NFIECC_CorrectEcc()
 */
//-----------------------------------------------------------------------------
static INT32 _NANDHW_NFIECC_CorrectEcc(UINT8 *pu1Buf, UINT8 *pu1Spare, UINT32 u4SectIdx,
                                                             UINT32 u4PgOff, UINT32 u4Len)
{
    UINT32 i, u4Val, u4SpareOff, u4SectOobSz;
    UINT32 u4ErrNum, u4ErrFound, u4ErrByte, u4ErrBit;
    
    _NANDHW_WaitEccDone(u4SectIdx); // must wait done

    if (_NANDHW_NFIECC_GetErrNum(u4SectIdx, &u4ErrNum))
    {
        return NAND_ECC_ERROR;
    }
    
    u4SectOobSz = (_u4OOBSize * _u4SectorSize)>>9;
    u4ErrFound = NAND_READ32(NAND_NFIECC_DECFER);
    if ((u4ErrFound >> u4SectIdx) & 0x01)
    {
        for (i = 0; i < u4ErrNum; i++)
        {
            u4Val = NAND_READ16(NAND_NFIECC_DECEL0 + (i * 2));
            u4ErrByte = u4Val >> 3;
            u4ErrBit = u4Val & 0x7;

            if (u4ErrByte < _u4SectorSize)
            {
                // Data area bit error.
                u4ErrByte += u4SectIdx*_u4SectorSize;
                if (pu1Buf && (u4ErrByte >= u4PgOff) && (u4ErrByte < (u4PgOff + u4Len)))
                {
                    pu1Buf[u4ErrByte - u4PgOff] = pu1Buf[u4ErrByte - u4PgOff] ^ (((UINT32)1) << u4ErrBit);
                }
            }
            else if (u4ErrByte < (u4SectOobSz + _u4SectorSize))
            {
                // Spare area bit error.
                if (pu1Spare)
                {
                    u4SpareOff = u4ErrByte - _u4SectorSize + u4SectOobSz * u4SectIdx;
                    pu1Spare[u4SpareOff] = pu1Spare[u4SpareOff] ^ (((UINT32)1) << u4ErrBit);
                }
            }
            else
            {
                return NAND_ECC_ERROR;
            }
        }
    }

    return NAND_SUCC;
}

//-----------------------------------------------------------------------------
/** _NANDHW_NFIECC_CheckEcc()
 */
//-----------------------------------------------------------------------------
static INT32 _NANDHW_NFIECC_CheckEcc(void)
{
    UINT32 u4SectIdx, u4ErrNum;
    INT32 i4Ret = NAND_SUCC;
    
    for (u4SectIdx = 0; u4SectIdx < _u4SectorNum; u4SectIdx++)
    {
        _NANDHW_WaitEccDone(u4SectIdx);
        i4Ret |= _NANDHW_NFIECC_GetErrNum(u4SectIdx, &u4ErrNum);
    }

    return i4Ret;
}

//-----------------------------------------------------------------------------
/** _NANDHW_DataExchange()
 */
//-----------------------------------------------------------------------------
static INT32 _NANDHW_DataExchange(UINT32 *pu4Buf, UINT32 *pu4Spare, UINT32 u4PgOff, UINT32 u4Len)
{
    UINT32 u4Val, u4Addr;

    ASSERT(pu4Buf != NULL);
    ASSERT(pu4Spare != NULL);
    
    if (_u4OOBSize == 16)
    {
        if (_u4PageSize == 0x800)
        {
            u4Addr = 0x7E0;
        }
        else if (_u4PageSize == 0x1000)
        {
            u4Addr = 0xFA0;
        }
        else if (_u4PageSize == 0x2000)
        {
            u4Addr = 0x1F20;
        }
        else if (_u4PageSize == 0x4000)
        {
            u4Addr = 0x3E20;
        }
        else
        {
            return NAND_SUCC;
        }
    }
    else if (_u4OOBSize == 26)
    {
        if (_u4PageSize == 0x800)
        {
            u4Addr = 0x7CC;
        }
        else if (_u4PageSize == 0x1000)
        {
            u4Addr = 0xF64;
        }
        else if (_u4PageSize == 0x2000)
        {
            u4Addr = 0x1E94;
        }
        else if (_u4PageSize == 0x4000)
        {
            u4Addr = 0x3CF4;
        }
        else
        {
            return NAND_SUCC;
        }
    }
#if CC_NAND_60BIT_NFI
    else if (_u4OOBSize == 32)
    {
        if (_u4PageSize == 0x1000)
        {
            u4Addr = 0xF40;
        }
        else if (_u4PageSize == 0x2000)
        {
            u4Addr = 0x1E40;
        }
        else if (_u4PageSize == 0x4000)
        {
            u4Addr = 0x4000 - 15*64;
        }
        else
        {
            return NAND_SUCC;
        }
    }
    else if (_u4OOBSize == 36)
    {
        if (_u4PageSize == 0x1000)
        {
            u4Addr = 0xF28;
        }
        else if (_u4PageSize == 0x2000)
        {
            u4Addr = 0x1E08;
        }
        else
        {
            return NAND_SUCC;
        }
    }
    else if (_u4OOBSize == 40)
    {
        if (_u4PageSize == 0x1000)
        {
            u4Addr = 0xF10;
        }
        else if (_u4PageSize == 0x2000)
        {
            u4Addr = 0x1DD0;
        }
        else
        {
            return NAND_SUCC;
        }
    }
    else if (_u4OOBSize == 64)
    {
        if (_u4PageSize == 0x1000)
        {
            u4Addr = 0xE80;
        }
        else if (_u4PageSize == 0x2000)
        {
            u4Addr = 0x1C80;
        }
        else
        {
            return NAND_SUCC;
        }
    }
#endif
    else
    {
        return NAND_FAIL;
    }
    
    if ((u4Addr >= u4PgOff) && (u4Addr < (u4PgOff + u4Len)))
    {
        u4Val = pu4Buf[(u4Addr - u4PgOff) >> 2];
        pu4Buf[(u4Addr-u4PgOff) >> 2] = pu4Spare[0];
        pu4Spare[0] = u4Val;
    }

    return NAND_SUCC;
}

//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------
#if CC_NAND_60BIT_NFI
#if NAND_RANDOM_EN

//-----------------------------------------------------------------------------
/** NANDHW_Random_Mizer_Encode()
 */
//-----------------------------------------------------------------------------
void NANDHW_Random_Mizer_Encode(UINT32 u4PgIdx, BOOL fgRandom)
{
    UINT32 u4Val;

    if (fgRandom)
    {
        NAND_WRITE32(NAND_NFI_RANDOM_CFG, 0);
        u4Val = NAND_NFI_ENCODING_RANDON_SEED(RANDOM_SEED[u4PgIdx%_u4BlkPgCount]) | NAND_NFI_ENCODING_RANDON_EN;
        NAND_WRITE32(NAND_NFI_RANDOM_CFG, u4Val);
    }
    else
    {
        NAND_WRITE32(NAND_NFI_RANDOM_CFG, 0);
    }
}
//-----------------------------------------------------------------------------
/** NANDHW_Random_Mizer_Decode()
 */
//-----------------------------------------------------------------------------
void NANDHW_Random_Mizer_Decode(UINT32 u4PgIdx, BOOL fgRandom)
{
    UINT32 u4Val;

    if (fgRandom)
    {
        NAND_WRITE32(NAND_NFI_RANDOM_CFG, 0);
        u4Val = NAND_NFI_DECODING_RANDON_SEED(RANDOM_SEED[u4PgIdx%_u4BlkPgCount]) | NAND_NFI_DECODING_RANDON_EN;
        NAND_WRITE32(NAND_NFI_RANDOM_CFG, u4Val);
    }
    else
    {
        NAND_WRITE32(NAND_NFI_RANDOM_CFG, 0);
    }
}
#endif
#else
#if NAND_SCRAMBLE_EN
#define NAND_SCRAMBLE_PATTERN   0xAA

//-----------------------------------------------------------------------------
/** NANDHW_SetScramble()
 */
//-----------------------------------------------------------------------------
void NANDHW_SetScramble(BOOL fgScramble)
{
    UINT32 u4Val;

    if (fgScramble)
    {
        u4Val = NAND_NFI_SCRAMBLE_PAT(NAND_SCRAMBLE_PATTERN) | NAND_NFI_SCRAMBLE_EN | NAND_NFI_INTERLEAVE_EN;
        NAND_WRITE32(NAND_NFI_SCRAMBLE, u4Val);
    }
    else
    {
        NAND_WRITE32(NAND_NFI_SCRAMBLE, 0);
    }
}
#endif
#endif
//-----------------------------------------------------------------------------
/** NANDHW_ReadID()
 */
//-----------------------------------------------------------------------------
void NANDHW_ReadID(UINT32 *pu4ID)
{
    ASSERT(pu4ID != NULL);

    _NANDHW_NFI_Reset();

    _NANDHW_Command(NAND_CMD_READ_ID);
    _NANDHW_Addr(0, 0, 1);
    _NANDHW_NFI_Cfg(NAND_ECC_NONE, 0, NAND_NFI_CNFG_OP_SINGLE);
    _NANDHW_NFI_Trig(NAND_NFI_CNFG_OP_SINGLE);
    _NANDHW_WaitBusy();
    
    _NANDHW_WaitRead(4);
    *pu4ID = NAND_READ32(NAND_NFI_DATAR);
}
//-----------------------------------------------------------------------------
/** NANDHW_ReadIDEx()
 */
//-----------------------------------------------------------------------------
UINT32 NANDHW_ReadIDEx(UINT32 *pu4ID)
{
    ASSERT(pu4ID != NULL);

    _NANDHW_NFI_Reset();

    _NANDHW_Command(NAND_CMD_READ_ID);
    NAND_WRITE32(NAND_NFI_COLADDR, 0);
    NAND_WRITE32(NAND_NFI_ROWADDR, 0);
    NAND_WRITE32(NAND_NFI_ADDRNOB, 1);
    _NANDHW_WaitBusy();
    _NANDHW_NFI_Cfg(NAND_ECC_NONE, 0, NAND_NFI_CNFG_OP_SINGLE);
	//NAND_WRITE32(NAND_NFI_CNFG, NAND_READ32(NAND_NFI_CNFG)| 0x00000040);
	NAND_WRITE32(NAND_NFI_CON,  NAND_NFI_CON_NOB(6) | NAND_NFI_CON_SRD);
    
    _NANDHW_WaitBusy();
    
    _NANDHW_WaitRead(4);
    *pu4ID  = NAND_READ32(NAND_NFI_DATAR);
	_NANDHW_WaitRead(2);
    return NAND_READ32(NAND_NFI_DATAR);
}

//-----------------------------------------------------------------------------
/** NANDHW_Reset()
 */
//-----------------------------------------------------------------------------
void NANDHW_Reset(void)
{
    _NANDHW_NFI_Reset();
    _NANDHW_Command(NAND_CMD_RESET);

    _NANDHW_WaitBusy();
}

//-----------------------------------------------------------------------------
/** NANDHW_EnablePinmux()
 */
//-----------------------------------------------------------------------------
void NANDHW_EnablePinmux(BOOL fgEnable)
{
    UINT32 u4Val;
    
    if (_fgNandInit == FALSE)
    {
#if defined(CC_MT5365) || defined(CC_MT5395)
        u4Val = BIM_READ32(NAND_NFI_PERMISC);
        u4Val |= NAND_NFI_PERMISC_NFI_SEL;
        BIM_WRITE32(NAND_NFI_PERMISC, u4Val);
#endif
    } 
    
#if defined(CC_MT5399) || defined(CC_MT5882)
#if CC_NAND_60BIT_NFI
		NAND_WRITE32(NAND_SELECT_REGISTER, 0x00000001);// no use in 5399
		NAND_WRITE32(NAND_NFI_MLC_CFG, NAND_CHK_DRAM_WDLE_EN|NAND_CHK_DRAM_WDLE_EN2);
#endif	
#if CC_NAND_DMMERGE_NFI
		NAND_WRITE32(NAND_DMMERGE_CFG, 0xFFFFFFFE);
#endif
#endif

#if CC_NAND_WDLE_CFG
    IO_WRITE32(0,NAND_NFI_MLC_CFG, NAND_CHK_DRAM_WDLE_EN2);
#endif
      
    u4Val = NAND_READ32(NAND_NFI_MISC);
    if (fgEnable)
    {
        u4Val |= NAND_NFI_MISC_FLASH_PMUX;
    }
    else
    {
        u4Val &= (~NAND_NFI_MISC_FLASH_PMUX);
    }
    NAND_WRITE32(NAND_NFI_MISC, u4Val);
    
    if (fgEnable)
    {
        NANDHW_Reset();
    }
}

//-----------------------------------------------------------------------------
/** _NANDHW_NFI_ReadFifo()
 */
//-----------------------------------------------------------------------------
static INT32 _NANDHW_NFI_ReadFifo(UINT8 *pu1Buf, UINT8 *pu1Spare, UINT32 u4PgOff, UINT32 u4Len)
{
    INT32 i4Ret = NAND_SUCC;
    UINT32 u4SectIdx, j, u4Val, u4LoopCnt;
    UINT32 *pu4Buf = (UINT32 *)pu1Buf;
    UINT32 *pu4Spare = (UINT32 *)pu1Spare;
    
    ASSERT(pu1Buf != NULL);
    
    for (u4SectIdx = 0; u4SectIdx < _u4SectorNum; u4SectIdx++)
    {
        u4LoopCnt = _u4SectorSize>>2;
        u4Val = u4SectIdx * u4LoopCnt;
        for (j = 0; j < u4LoopCnt; j++)
        {
            _NANDHW_WaitRead(4);
            if (((u4Val << 2) >= u4PgOff) && ((u4Val << 2) < (u4PgOff + u4Len)))
            {
                pu4Buf[u4Val - (u4PgOff >> 2)] = NAND_READ32(NAND_NFI_DATAR);
            }
            else
            {
                NAND_READ32(NAND_NFI_DATAR);
            }
            u4Val++;
        }
        
        u4LoopCnt = (_u4OOBSize * _u4SectorSize) >> 11;
        u4Val = u4SectIdx * u4LoopCnt;
        for (j = 0; j < u4LoopCnt; j++)
        {
            _NANDHW_WaitRead(4);
            if (pu4Spare)
            {
                pu4Spare[u4Val] = NAND_READ32(NAND_NFI_DATAR);
            }
            else
            {
                NAND_READ32(NAND_NFI_DATAR);
            }
            u4Val ++;
        }

        if (_rECCType == NAND_ECC_SOFT)
        {
            /* ECC correction. */
            if (_NANDHW_NFIECC_CorrectEcc(pu1Buf, pu1Spare, u4SectIdx, u4PgOff, u4Len))
            {
                i4Ret |= NAND_ECC_ERROR;
            }
        }
        
        _NANDHW_WaitBusy();
    }
    
    _NANDHW_NFIECC_WaitBusy();
    return i4Ret;
}

//-----------------------------------------------------------------------------
/** _NANDHW_ReadPage()
 */
//-----------------------------------------------------------------------------
static INT32 _NANDHW_ReadPage(UINT8 *pu1Buf, UINT8 *pu1Spare, UINT32 u4PgIdx, UINT32 u4PgOff, UINT32 u4Len)
{
    INT32 i4Ret = NAND_SUCC;

    ASSERT(pu1Buf != NULL);
    
    _NANDHW_NFI_Reset();
    _NANDHW_NFIECC_Reset();
#if CC_NAND_60BIT_NFI
    #if NAND_RANDOM_EN
    if(fgRandomizer == TRUE)
    {
        NANDHW_Random_Mizer_Decode(u4PgIdx, TRUE);
    }
    #endif
#endif
    _NANDHW_NFI_Cfg(_rECCType, _fgDmaEn, NAND_NFI_CNFG_OP_READ);
    _NANDHW_NFIECC_Cfg(_rECCType);
    _NANDHW_NFIECC_Trig(NAND_NFI_CNFG_OP_READ);
#if defined(CC_MT5399) || defined(CC_MT5882)
    _NANDHW_SetEptNum(40);
#endif
#if NAND_AUTORD_DONE
	NAND_WRITE32(NAND_NFI_INTR_EN, NAND_NFI_INTR_EN_RD_DONE | NAND_NFI_INTR_EN_AHB_DONE);	
	/*Trigger Read . */
	_NANDHW_NFI_Trig(NAND_NFI_CNFG_OP_READ);
#endif
    /* Write command. */
    _NANDHW_Command(NAND_CMD_READ1_00);
    _NANDHW_Addr(u4PgIdx, 0, (_u4PgAddrLen << 4)|(_u4TotalAddrLen - _u4PgAddrLen));
    if (_u4PageSize != 0x200)
    {
        _NANDHW_Command(NAND_CMD_READ_30);
    }

    _NANDHW_WaitBusy();
    i4Ret = _NANDHW_NFI_CheckOpMode(NAND_NFI_CNFG_OP_READ);
    if (i4Ret)
    {
        CHIP_NandShowHex("Read fail 1: page=", u4PgIdx);
        goto HandleExit;
    }
    
#if !(NAND_AUTORD_DONE)
    /*Trigger Read . */
    _NANDHW_NFI_Trig(NAND_NFI_CNFG_OP_READ);
#endif
    _NANDHW_WaitBusy();

    i4Ret = _NANDHW_NFI_ReadFifo(pu1Buf, pu1Spare, u4PgOff, u4Len);
    if (i4Ret)
    {
        CHIP_NandShowHex("Read fail 2: page=", u4PgIdx);
        goto HandleExit;
    }

    _NANDHW_NFI_WaitSectorCnt();

    /* Check ECC */
    if (_rECCType != NAND_ECC_NONE)
    {
        i4Ret = _NANDHW_NFIECC_CheckEcc();
        if (i4Ret)
        {
            CHIP_NandShowHex("Read fail 3: page=", u4PgIdx);
            goto HandleExit;
        }
    }
#if CC_NAND_60BIT_NFI
#if NAND_RANDOM_EN
    if(fgRandomizer == TRUE)
    {
        NANDHW_Random_Mizer_Decode(u4PgIdx, FALSE);
    }
#endif
#endif   
HandleExit:
    _NANDHW_WaitBusy();
    NAND_WRITE32(NAND_NFI_CNFG, 0);
#if CC_NAND_60BIT_NFI
    #if NAND_RANDOM_EN
    if(fgRandomizer == TRUE)
    {
        NANDHW_Random_Mizer_Decode(u4PgIdx, FALSE);
    }
    #endif
#endif
    return i4Ret;
}

//-----------------------------------------------------------------------------
/** _NANDHW_ProbeDev()
 */
//-----------------------------------------------------------------------------
static INT32 _NANDHW_ProbeDev(void)
{
    NAND_FLASH_DEV_T *prDev;
    UINT32 i, u4ID, u4DevCount;

    NANDHW_ReadID(&u4ID);
    CHIP_NandShowHex("NID=", u4ID);
    
    u4DevCount = sizeof(_arNand_DevInfo) / sizeof(NAND_FLASH_DEV_T);
    for (i = 0; i < u4DevCount; i++)
    {
        prDev = (NAND_FLASH_DEV_T *)&_arNand_DevInfo[i];
        if ((u4ID & prDev->u4IDMask) == (prDev->u4IDVal & prDev->u4IDMask))
        {
            _prFlashDev = prDev;
			      if(0x1590DA98 == u4ID)
            {
                if(0x00001676 == NANDHW_ReadIDEx(&u4ID))
                {
                    _prFlashDev->u4OOBSize = 26;
                    _prFlashDev->u4BBMode  = 0x00000008;
                    CHIP_DisplayString("ExNID=0x00001676");
                }
            }
            else if(0x9590DA2C == u4ID)
            {
                if(0x00000004 == NANDHW_ReadIDEx(&u4ID))
                {
                    _prFlashDev->u4OOBSize = 26;
                    _prFlashDev->u4BBMode  = 0x00000008;
                    CHIP_DisplayString("ExNID=0x00000004");
                }
            }
            else if(0x9590DCAD == u4ID)
            {
                if(0x00000056 == (0x000000FF & NANDHW_ReadIDEx(&u4ID)))
                {
                    _prFlashDev->u4OOBSize = 26;
                    _prFlashDev->u4BBMode  = 0x00000008;
                    CHIP_DisplayString("ExNID=0x00000056");
                }
            }
            else if(0x9590DC01 == u4ID)
            {
                if(0x00000056 == (0x000000FF & NANDHW_ReadIDEx(&u4ID)))
                {
                    _prFlashDev->u4OOBSize = 26;
                    _prFlashDev->u4BBMode  = 0x00000008;
                    CHIP_DisplayString("ExNID=0x00000056");
                }
            }
            return NAND_SUCC;
        }
    }

    _prFlashDev = NULL;
    return NAND_FAIL;
}

//-----------------------------------------------------------------------------
/** NANDHW_Init()
 */
//-----------------------------------------------------------------------------
INT32 NANDHW_Init(void)
{
    UINT32 i;

    if (_fgNandInit)
    {
        return NAND_SUCC;
    }
    
    UNUSED(_fgIsrEn);
    UNUSED(_u4FdmNum);
    UNUSED(_u4ECCBits);
    
    _fgIsrEn = FALSE;
    _fgDmaEn = FALSE;
    _rECCType = NAND_ECC_SOFT;
    _u4BlkPgCount = 64;
    _prFlashDev = NULL;
    _u4PgAddrLen  = 2;
    _u4TotalAddrLen = 5;
    _u4PageSize = 2048;
    _u4OOBSize = 16;
    _u4SectorSize = 1024;
	
    IO_WRITE32(0x0, 0xf000d48c, IO_READ32(0x0,0xf000d48c) & 0x8E38E3FF);//WE driving clear
    IO_WRITE32(0x0, 0xf000d490, IO_READ32(0x0,0xf000d490) & 0xE38E38E3);//pin driving clear
    
    IO_WRITE32(0x0, 0xf000d48c, IO_READ32(0x0,0xf000d48c) | 0x10410400);//WE driving setting
    IO_WRITE32(0x0, 0xf000d490, IO_READ32(0x0,0xf000d490) | 0x04104104);//pin driving setting
    
    IO_WRITE32(0x0, 0xf000d540, IO_READ32(0x0,0xf000d540) | 0x1);//POOE driving 
    IO_WRITE32(0x0, 0xf000d550, IO_READ32(0x0,0xf000d550) & 0xFFFFFFFE);//POOE driving 
    if(0x00400000 == (IO_READ32(0x0,0xf0008664)&0x00400000))//5399 randomizer efuse bit
       fgRandomizer = TRUE;
    else
       fgRandomizer = FALSE;		

    NAND_WRITE32(NAND_NFI_ACCCON1, 0x00411888);
    NAND_WRITE32(NAND_NFI_ACCCON2, 0x00080008);

    for (i = 0; i < 3; i++)
    {
        // Reset NFI Controller
        _NANDHW_NFI_Reset();
        NANDHW_Reset();

        if (_NANDHW_ProbeDev() == NAND_SUCC)
        {
            LOG(1, "Detect %s NAND flash: %dMB\n", _prFlashDev->acName, (_prFlashDev->u4ChipSize / 0x100000));
            _u4PageSize = _prFlashDev->u4PageSize;
            _u4OOBSize =  _prFlashDev->u4OOBSize;
            
            _NANDHW_NFI_Cfg(NAND_ECC_NONE, 0, NAND_NFI_CNFG_OP_SINGLE);
            _NANDHW_NFIECC_Cfg(NAND_ECC_NONE);
            
            if (_prFlashDev->u4PageSize == 512)
            {
                _u4PgAddrLen = (UINT32)(_prFlashDev->u1AddrCycle - 1);
                _u4TotalAddrLen = _prFlashDev->u1AddrCycle;
                _u4SectorSize = 512;
            }
            else
            {
                _u4PgAddrLen = (UINT32)(_prFlashDev->u1AddrCycle - 2);
                _u4TotalAddrLen = _prFlashDev->u1AddrCycle;
                _u4SectorSize = 1024;
            }
            _u4BlkPgCount = _prFlashDev->u4BlkPgCount;
            
            break;
        }
        
        LOG(0, "Not detect any flash\n");
        _u4PgAddrLen = 2;
        _u4TotalAddrLen = 5;
        _u4PageSize = 2048;
        _u4OOBSize = 16;
        _u4SectorSize = 1024;
    }
    
    _NANDHW_NFI_SetPageFmt();
    _u4SectorNum = _u4PageSize / _u4SectorSize;

    _fgNandInit = TRUE;
    return NAND_SUCC;
}

//-----------------------------------------------------------------------------
/** _NANDHW_NFI_ReadSpare()
 */
//-----------------------------------------------------------------------------
static INT32 _NANDHW_NFI_ReadSpare(UINT8 *pu1Buf, UINT32 u4Len)
{
    UINT32 i, u4Val = 0;
    UINT32 *pu4Buf = (UINT32 *)pu1Buf;
    INT32 i4Ret = NAND_SUCC;

    ASSERT(pu1Buf != NULL);

    for (i = 0; i < u4Len; i += 4)
    {
        _NANDHW_WaitRead(4);
        pu4Buf[u4Val] = NAND_READ32(NAND_NFI_DATAR);
        u4Val++;
    }
    
    if (_rECCType == NAND_ECC_SOFT)
    {
        if (u4Len == (_u4SectorSize + ((_u4OOBSize *_u4SectorSize)>>9)))
        {
            /* ECC correction. */
            if (_NANDHW_NFIECC_CorrectEcc(pu1Buf, &pu1Buf[_u4SectorSize], 0, 0, u4Len))
            {
                i4Ret |= NAND_ECC_ERROR;
            }
        }
    }

    _NANDHW_WaitBusy();
    return i4Ret;
}

//-----------------------------------------------------------------------------
/** _NANDHW_ReadSpare()
 */
//-----------------------------------------------------------------------------
INT32 _NANDHW_ReadSpare(UINT8 *pu1Buf, UINT32 u4PgIdx, UINT32 u4PgOff, UINT32 u4Len)
{
    INT32 i4Ret = NAND_SUCC;
    ASSERT(pu1Buf != NULL);
    
    _NANDHW_NFI_Reset();
    _NANDHW_NFIECC_Reset();

    _NANDHW_NFI_Cfg(_rECCType, _fgDmaEn, NAND_NFI_CNFG_OP_READ);
    _NANDHW_NFIECC_Cfg(_rECCType);
    _NANDHW_NFIECC_Trig(NAND_NFI_CNFG_OP_READ);

    /* Read command. */
    if ((u4PgOff == 0x200) && (_u4PageSize == 0x200))
    {
        _NANDHW_Command(NAND_CMD_READ2);
        _NANDHW_Addr(u4PgIdx, 0, (_u4PgAddrLen << 4)|(_u4TotalAddrLen - _u4PgAddrLen));
    }
    else
    {
        _NANDHW_Command(NAND_CMD_READ1_00);
        _NANDHW_Addr(u4PgIdx, u4PgOff, (_u4PgAddrLen << 4)|(_u4TotalAddrLen - _u4PgAddrLen));
    }
    
    if (_u4PageSize != 0x200)
    {
        _NANDHW_Command(NAND_CMD_READ_30);
    }

    /*Trigger Read . */
    i4Ret = _NANDHW_NFI_CheckOpMode(NAND_NFI_CNFG_OP_READ);
    if (i4Ret)
    {
        goto HandleExit;
    }
    
    _NANDHW_NFI_Trig(NAND_NFI_CNFG_OP_READ);

    _NANDHW_WaitBusy();
    i4Ret = _NANDHW_NFI_ReadSpare(pu1Buf, u4Len);
    if (i4Ret)
    {
        goto HandleExit;
    }
    
HandleExit:
    _NANDHW_WaitBusy();
    NAND_WRITE32(NAND_NFI_CNFG, 0);
    return i4Ret;
}
static int _NANDHW_Page_Empty(UINT32 *pu4Buf, UINT32 *pu4Spare)
{
    // If all data is 0xff, do not write the page. mtk40109 - 2010/4/23
    UINT32 i, u4PgOobSz;

    for (i = 0; i < _u4PageSize/4; i++)
    {
        if (pu4Buf[i] != 0xFFFFFFFF)
        {
            return 0;
        }
    }
    //for scan bad block in Random mizer, the function is closed to a new nand flash.
#if CC_NAND_60BIT_NFI
#if NAND_RANDOM_EN
    if (fgRandomizer && (!pu4Spare))
    {
        return 1;
    }
#endif
#endif

    u4PgOobSz = (_u4OOBSize * _u4PageSize)>>9;
    for (i = 0; i < u4PgOobSz/4; i++)
    {
        if (pu4Spare[i] != 0xFFFFFFFF)
        {
            return 0;
        }
    }

    return 1;
}

//-----------------------------------------------------------------------------
/** NANDHW_ReadPageAttr()
 */
//-----------------------------------------------------------------------------
INT32 NANDHW_ReadPageAttr(UINT32 *pu4Buf, UINT32 *u4Spare, UINT32 u4PgIdx, UINT32 u4PgOff, UINT32 u4Len)
{
    INT32 i4Ret;
    
    i4Ret = _NANDHW_ReadPage((UINT8 *)pu4Buf, (UINT8 *)u4Spare, u4PgIdx, u4PgOff, u4Len);
    _NANDHW_DataExchange(pu4Buf, u4Spare, u4PgOff, u4Len);
    return i4Ret;
}
//-----------------------------------------------------------------------------
/** _NANDHW_IsBadPage()
 */
//-----------------------------------------------------------------------------
static BOOL _NANDHW_IsBadPage(UINT32 u4PgIdx)
{
    INT32 iRet;
    BOOL isEmpty = TRUE;
    UINT32 u4BitCnt, u4Offset, u4OobLastIdx, i;
    UINT32 u4EccSectorOobSize, u4EccPageOobSize, u4EccSectorSize;
    
    _rECCType = NAND_ECC_NONE;
    
    u4EccSectorOobSize =  (_u4OOBSize * _u4SectorSize)>>9;
    u4EccPageOobSize = (_u4OOBSize *_u4PageSize)>>9;
    u4EccSectorSize = _u4SectorSize + u4EccSectorOobSize;

    // read spare data without ecc
    u4Offset = _u4PageSize;
       
    //for Random mizer, new flash block or erased block cannot be scan by Random mizer mode.
#if CC_NAND_60BIT_NFI
    #if NAND_RANDOM_EN   
    if(fgRandomizer)
    {
       fgRandomizer = FALSE;
       iRet = NANDHW_ReadPageAttr((UINT32 *)pu1Data, (UINT32 *)pu1Spare, u4PgIdx, 0, _u4PageSize);
       if (iRet)
       {
          fgRandomizer = TRUE;
          goto HandleBad;
       }
       else if(!_NANDHW_Page_Empty((UINT32 *)pu1Data, NULL))
       {   
          isEmpty = FALSE;
          fgRandomizer = TRUE;
          iRet = NANDHW_ReadPageAttr((UINT32 *)pu1Data, (UINT32 *)pu1Spare, u4PgIdx, 0, _u4PageSize);
       }
       else
       {
          isEmpty = TRUE;
          fgRandomizer = TRUE;
       }
    }
    #endif
#endif
    if(isEmpty == TRUE)
    {
        if ((_prFlashDev->u4BBMode & NAND_BBMODE_ECCBIT) == 1)
        {
              iRet = _NANDHW_ReadSpare(pu1Spare, u4PgIdx, u4Offset, _u4OOBSize);
        }
        else
        {
              iRet = _NANDHW_ReadSpare(pu1Spare, u4PgIdx, u4Offset, u4EccPageOobSize);
        }
    }
    
    if (iRet)
    {
        goto HandleBad;
    }
    
    if ((_prFlashDev->u4BBMode & NAND_BBMODE_BBMARK) == 0x00000500)
    {
        u4BitCnt = _NANDHW_countbits(pu1Spare[5]) + _NANDHW_countbits(pu1Spare[6]);
    }
    else
    {
        u4BitCnt = _NANDHW_countbits(pu1Spare[0]) + _NANDHW_countbits(pu1Spare[1]);
    }
    
    switch (_prFlashDev->u4BBMode & NAND_BBMODE_ECCBIT)
    {
    case 0:
        ASSERT(0);
        break;
        
    case 1:
        if (u4BitCnt < 15)
        {
            goto HandleBad;
        }
        else
        {
            goto HandleGood;
        }
        
    default:
        u4OobLastIdx = u4EccPageOobSize - u4EccSectorOobSize;
        for (i = 0; i < 6; i++)
        {
           u4BitCnt += _NANDHW_countbits(pu1Spare[u4OobLastIdx + i]);
        }

        if(u4BitCnt > 62)
        {
            goto HandleGood;
        }
        
        if (u4BitCnt < 2)
        {
            goto HandleBad;
        }

        break;
    }
    
    _rECCType = NAND_ECC_SOFT;

    // read last sector data with ecc
    u4Offset = (_u4SectorNum - 1) * u4EccSectorSize;
    iRet = _NANDHW_ReadSpare(pu1Data, u4PgIdx, u4Offset, u4EccSectorSize);
    if (iRet)
    {
        goto HandleBad;
    }

    u4OobLastIdx = u4EccSectorSize - (u4EccSectorOobSize*_u4SectorNum);
    if ((pu1Data[u4OobLastIdx] != 0xFF) || (pu1Data[u4OobLastIdx + 1] != 0xFF))
    {
        goto HandleBad;
    }
      
HandleGood:
    _rECCType = NAND_ECC_SOFT;
    return FALSE;       // Good block
HandleBad:
    _rECCType = NAND_ECC_SOFT;
    return TRUE;        // bad block
}

//-----------------------------------------------------------------------------
/** NANDHW_IsBadBlk()
 */
//-----------------------------------------------------------------------------
BOOL NANDHW_IsBadBlk(UINT32 u4BlkIdx)
{
    UINT32 u4PgIdx = u4BlkIdx * _u4BlkPgCount;
    
    // check the 1st page of the block.
    if (_NANDHW_IsBadPage(u4PgIdx))
    {
        return TRUE;
    }
    // check the 2nd page of the block.
    if (_NANDHW_IsBadPage(u4PgIdx + 1))
    {
        return TRUE;
    }
    // check the last page of the block if MLC.
    if (_u4OOBSize > 16)
    {
        if (_NANDHW_IsBadPage(u4PgIdx + _u4BlkPgCount -1))
        {
            return TRUE;
        }
    }

    return FALSE;
}


// Check for pre bad block of loader.
UINT32 NANDHW_CheckPreBadBlk(UINT32 u4ByteOffset)
{    
    UINT32 i, u4PgIdx, u4GoodBlk, u4Count, u4BlkSize;

    u4PgIdx = u4ByteOffset/_u4PageSize;
    u4BlkSize = _u4PageSize*_u4BlkPgCount;
    
    if ((u4PgIdx & (_u4BlkPgCount - 1)) == 0)
    {
        u4GoodBlk = u4ByteOffset/u4BlkSize;
    }
    else
    {
        u4GoodBlk = u4ByteOffset/u4BlkSize + 1;
    }

    for (i = 0, u4Count = 0; u4Count < u4GoodBlk; i++)
    {
        // The first block must be good.
        if ((i != 0) && NANDHW_IsBadBlk(i))
        {
            CHIP_NandShowHex("Pre  Bad Block:", i);
            
            u4ByteOffset += u4BlkSize;
            continue;
        }

        u4Count++;
    }

    return u4ByteOffset;
}

//-----------------------------------------------------------------------------
/** CHIP_NANDCopyToDRAM
 */
//-----------------------------------------------------------------------------
INT32 CHIP_NANDCopyToDRAM(UINT32 *pu4DstBuf, UINT32 u4ByteOffset, UINT32 u4Len)
{
    INT32  i4Ret = 0;
    UINT32 u4Spare[28*4];
    UINT32 i, u4PgIdx, u4PageOff, u4ReadByte;

    u4ByteOffset = NANDHW_CheckPreBadBlk(u4ByteOffset);
        
    u4PgIdx = u4ByteOffset/_u4PageSize;
    u4PageOff = u4ByteOffset&(_u4PageSize - 1);
    
    if (u4PageOff)
    {
        u4ReadByte = _u4PageSize - u4PageOff;
    }
    else
    {
        u4ReadByte = _u4PageSize;
    }
    
    for (i = 0; i < u4Len; )
    {
        // Check for post bad block of loader.
        if ((u4PgIdx & (_u4BlkPgCount - 1)) == 0)
        {
            if (NANDHW_IsBadBlk(u4PgIdx/_u4BlkPgCount))
            {
                CHIP_NandShowHex("Post Bad Block:", u4PgIdx/_u4BlkPgCount);
                
                u4PgIdx += _u4BlkPgCount;
                continue;
            }
        }
        
        if ((u4ReadByte + i) > u4Len)
        {
            u4ReadByte = u4Len - i;
        }
        
        i4Ret |= NANDHW_ReadPageAttr((UINT32 *)((UINT32)pu4DstBuf+i), u4Spare, u4PgIdx, u4PageOff, u4ReadByte);

        i += u4ReadByte;
        u4PageOff = 0;
        u4ReadByte = _u4PageSize;
        u4PgIdx++;
    }
    
    return i4Ret;
}

// This function is just protect _u4Signature/ROMCODETAG not optimized by compiler.
UINT32 NandDummy(UINT32 i)
{
    return _u4Signature[i];
}

void CHIP_NANDCopyToDRAMLZHS(UINT32 *pu4DstBuf, UINT32 u4ByteOffset, UINT32 u4Len)
{
    NandDummy(0);
    
    NANDHW_EnablePinmux(TRUE);
    NANDHW_Init();
    
    CHIP_NANDCopyToDRAM((UINT32 *)pu4DstBuf, u4ByteOffset, u4Len);
    
    NANDHW_EnablePinmux(FALSE);
}

#endif

