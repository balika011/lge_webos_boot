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
 * $Date: 2015/02/21 $
 * $RCSfile: nor_part.c,v $
 * $Revision: #2 $
 *
 *---------------------------------------------------------------------------*/

/** @file msdc_part.c
 *  msdc_part.c provides functions of norflash physical/virutal partitions.
 *  In msdc_part.c, there are functions to calculate norflash physical/virtual
 *  partitions, such as  size, offset, sector map, and other necessary
 *  functions.
 */

//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------

#include "msdc_debug.h"
#include "msdc_if.h"
#include "msdc_drv.h"
#include "x_lint.h"
#include "drvcust_if.h"
#include "x_pinmux.h"
#include "x_debug.h"

#ifdef CC_MSDC_ENABLE

typedef struct _MSDC_PART_T
{
    UINT32 fgInitial;
    UINT32 u4PartSize[MSDCPART_PARTNUM];
    UINT32 u4PartOffset[MSDCPART_PARTNUM];
} MSDC_PART_T;

// virtual eeprom
typedef enum
{
    MSDC_EEPROM_NONE,
    MSDC_EEPROM_PARTA,
    MSDC_EEPROM_PARTB,
} MSDC_EEPROM_PART_T;

typedef struct _MSDC_EEPROM_T
{
    BOOL fgInitial;
    UINT32 u4SDMPartId;
    UINT32 u4EEPPartIdA;
    UINT32 u4EEPPartIdB;
    UINT32 u4EEPPartOffA;
    UINT32 u4EEPPartOffB;
    UINT32 u4EEPBlkNum;
    UINT32 u4EEPSize;
    
    UINT32 u4WriteCnt;
    
    UINT32 u4EEPBlkA;
    UINT32 u4EEPBlkB;
    
    #ifdef CC_EEP2EMMC_DUAL_BANK_SUPPORT
	UINT32 u4EEPInitHeaderSize;
	UINT32 u4EEPHeaderSize;
	UINT32 u4EEPHeaderOffset;
    UINT32 u4EEPBankAOffset;
    UINT32 u4EEPBankBOffset;	
	UINT8  u1fgEEPAccessProtect; //0x00:fully 0x01:A prohibit 0x10:B prohibit 0x11:access prohibit
    #endif
	
    MSDC_EEPROM_PART_T eEEPPart;

    UINT8 *pu1EEPBuf;
} MSDC_EEPROM_T;

static MSDC_EEPROM_T _rMSDCEEPROM;

#ifdef CC_PARTITION_WP_SUPPORT
UINT32 wpConfig = 0;
#endif
#define MSDC_EEPBUF_WRITECNT    (*((UINT32 *)(_rMSDCEEPROM.pu1EEPBuf + _rMSDCEEPROM.u4EEPSize - 4)))

static HANDLE_T _hMSDCRWSema   = NULL_HANDLE;
static HANDLE_T _hMSDCInitSema = NULL_HANDLE;

#define MSDCPART_RW_ENTRY       {  MSDC_ApiLock(&_hMSDCRWSema);      }
#define MSDCPART_RW_EXIT        {  MSDC_ApiUnLock(&_hMSDCRWSema);    }
#define MSDCPART_INIT_ENTRY     {  MSDC_ApiLock(&_hMSDCInitSema);    }
#define MSDCPART_INIT_EXIT      {  MSDC_ApiUnLock(&_hMSDCInitSema);  }

#ifdef CC_EEP2EMMC_DUAL_BANK_SUPPORT
#define EEP2MSDC_MASK_ACCESS_PROHIBIT       (0x11)
#define EEP2MSDC_BANKA_ACCESS_PROHIBIT      (1<<0)
#define EEP2MSDC_BANKB_ACCESS_PROHIBIT      (1<<4)
#endif

void MSDCPART_ShowPartition()
{
    UINT32 u4PartId, u4Encryped;
    UINT64 u8Offset, u8PartSize;
    
    for (u4PartId = 0; u4PartId < MSDCPART_PARTNUM; u4PartId++)
    {
        u8PartSize = DRVCUST_InitGet64((QUERY_TYPE_T)(eNANDFlashPartSize0 + u4PartId));
        u8Offset   = DRVCUST_InitGet64((QUERY_TYPE_T)(eNANDFlashPartOffset0 + u4PartId));
		
        if (u8PartSize == 0)
        {
            LOG(0, "Part:#%02d, offset:0x%08x, size:0x%08x,  none\n", u4PartId, 0, 0);
        }
        else
        {
#ifdef CC_MTD_ENCRYPT_SUPPORT
            u4Encryped = DRVCUST_InitGet((QUERY_TYPE_T)(eNANDFlashPartEncypt0 + u4PartId));
#else
            u4Encryped = 0;
#endif
            if(u4Encryped)
            {
                LOG(0, "Part:#%02d, offset:0x%08x%08x, size:0x%08x%08x,  ENC\n", u4PartId,
					(UINT32)(u8Offset>>32), (UINT32)u8Offset, (UINT32)(u8PartSize>>32), (UINT32)u8PartSize);
            }
            else
            {
                LOG(0, "Part:#%02d, offset:0x%08x%08x, size:0x%08x%08x,  none\n", u4PartId,
					(UINT32)(u8Offset>>32), (UINT32)u8Offset, (UINT32)(u8PartSize>>32), (UINT32)u8PartSize);
            }
        }
    }
}

#if defined(__KERNEL__)
#include <linux/mmc/mtk_msdc_part.h>
#endif

static INT32 _MSDC_EEPROM_Init(void)
{
    MSDC_DevInfo_T rMSDCDev;
#ifdef CC_EEP2EMMC_DUAL_BANK_SUPPORT
    UINT8 u1InitHeader[4]={0};
#endif

    //LOG(3, "_MSDC_EEPROM_Init start!\n");
    MSDCPART_INIT_ENTRY

    if (_rMSDCEEPROM.fgInitial)
    {
        MSDCPART_INIT_EXIT
        return 0;
    }

    if (MsdcGetDevInfo(&rMSDCDev) != 0)
    {
        MSDCPART_INIT_EXIT
        LOG(0, "MSDC  info get error!\n");
        return -1;
    }
        
    _rMSDCEEPROM.u4EEPSize    = DRVCUST_InitGet64(eSystemEepromSize);
    _rMSDCEEPROM.u4SDMPartId  = DRVCUST_InitGet64(eNANDFlashDynamicPartitionID); 
    _rMSDCEEPROM.u4EEPPartIdA = DRVCUST_InitGet64(eNANDFlashPartIdEepromA);
    _rMSDCEEPROM.u4EEPPartIdB = DRVCUST_InitGet64(eNANDFlashPartIdEepromB);  
    _rMSDCEEPROM.pu1EEPBuf = (UINT8 *)x_mem_alloc_virtual(_rMSDCEEPROM.u4EEPSize);
    
    if (_rMSDCEEPROM.pu1EEPBuf == NULL)
    {
        MSDCPART_INIT_EXIT
        LOG(0, "_MSDC_EEPROM_Init Can't allocate memory!\n");
        ASSERT(0);
        return -1;
    }
    
    _rMSDCEEPROM.u4EEPPartOffA = DRVCUST_InitGet64((QUERY_TYPE_T)(eNANDFlashPartOffset0 + _rMSDCEEPROM.u4EEPPartIdA));
    _rMSDCEEPROM.u4EEPPartOffB = DRVCUST_InitGet64((QUERY_TYPE_T)(eNANDFlashPartOffset0 + _rMSDCEEPROM.u4EEPPartIdB));

    _rMSDCEEPROM.u4WriteCnt = 0xFFFFFFFF;
    _rMSDCEEPROM.u4EEPBlkNum = (_rMSDCEEPROM.u4EEPSize < 512) ? 1 : _rMSDCEEPROM.u4EEPSize / 512;
    _rMSDCEEPROM.eEEPPart = MSDC_EEPROM_PARTA;
    _rMSDCEEPROM.u4EEPBlkA = 0xFFFFFFFF;
    _rMSDCEEPROM.u4EEPBlkB = 0xFFFFFFFF;
    
#ifdef CC_EEP2EMMC_DUAL_BANK_SUPPORT
    _rMSDCEEPROM.u4EEPInitHeaderSize = DRVCUST_InitGet64(eSystemEepInitHeaderSize);
    _rMSDCEEPROM.u4EEPHeaderSize = DRVCUST_InitGet64(eSystemEepHeaderSize);
    _rMSDCEEPROM.u4EEPHeaderOffset = _rMSDCEEPROM.u4EEPInitHeaderSize;	
    _rMSDCEEPROM.u4EEPBankAOffset = _rMSDCEEPROM.u4EEPHeaderOffset + _rMSDCEEPROM.u4EEPHeaderSize;
    _rMSDCEEPROM.u4EEPBankBOffset = _rMSDCEEPROM.u4EEPBankAOffset + _rMSDCEEPROM.u4EEPHeaderSize;;
	//read init header first
	if (Loader_ReadMsdc(_rMSDCEEPROM.u4EEPPartIdA, 0, (void*)(u1InitHeader), 4) != 4)
	{
#ifdef __KERNEL__
	    printk(KERN_ERR "_MSDC_EEPROM_Init header(%d) fail!\n",_rMSDCEEPROM.u4EEPPartIdA);
#else
		LOG(0, "_MSDC_EEPROM_Init header(%d) fail!\n",_rMSDCEEPROM.u4EEPPartIdA);
#endif
        ASSERT(0);
		return -1;
    }
    if((u1InitHeader[0]==0x58) && (u1InitHeader[1]==0x90) &&
		(u1InitHeader[2]==0x58) && (u1InitHeader[3]==0x90))
    {
	    _rMSDCEEPROM.u1fgEEPAccessProtect = 0x00;
    }
	else
	{
	    _rMSDCEEPROM.u1fgEEPAccessProtect = 0x11;
	}
#endif
    
    x_memset((void *)_rMSDCEEPROM.pu1EEPBuf, 0xFF, _rMSDCEEPROM.u4EEPSize);
    
    _rMSDCEEPROM.fgInitial = TRUE;
   MSDCPART_INIT_EXIT
   //LOG(3, "_MSDC_EEPROM_Init end!\n");
    return 0;
}

INT32 MSDC_EEPROM_Read(UINT32 u4Offset, UINT32 u4MemPtr, UINT32 u4MemLen)
{
    INT32 i4Ret = 0;
#ifdef CC_EEP2EMMC_DUAL_BANK_SUPPORT
    UINT32 u4HeaderIdx;
    UINT8 u1HeadderCount;
#endif
    
    VERIFY(_MSDC_EEPROM_Init() == 0);

    if ((u4Offset >= _rMSDCEEPROM.u4EEPSize) || ((u4Offset + u4MemLen) > _rMSDCEEPROM.u4EEPSize))
    {
        LOG(0, "MSDC_EEPROM_Read: out of boundry! u4Offset = 0x%x, u4MemLen = 0x%x\n", u4Offset, u4MemLen);
        
        ASSERT(0);
        return -1;
    }
    
#ifdef CC_EEP2EMMC_DUAL_BANK_SUPPORT
	if ((_rMSDCEEPROM.u1fgEEPAccessProtect & EEP2MSDC_MASK_ACCESS_PROHIBIT) != 0)
    {
#ifdef __KERNEL__
            printk(KERN_ERR "MSDC_EEPROM_Read access prohibit!\n");
#else
            LOG(0, "Msdc_Read_EEPROM access prohibit!\n");
#endif
            return -1;
    }

    u4HeaderIdx = u4Offset + _rMSDCEEPROM.u4EEPHeaderOffset;
	if (Loader_ReadMsdc(_rMSDCEEPROM.u4EEPPartIdA, u4HeaderIdx,(void*)(&u1HeadderCount), 1) != 1)
    {
#ifdef __KERNEL__
            printk(KERN_ERR "Msdc_Read_EEPROMHeader fail!\n");
#else
            LOG(0, "Msdc_Read_EEPROMHeader fail!\n");
#endif
            return -1;
    }
    if(u1HeadderCount == 0)
    {
        u4Offset = u4Offset + _rMSDCEEPROM.u4EEPBankAOffset;
		LOG(3, "Msdc_Read_EEPROMData from bankA!\n");
    }
	else
	{
	    u4Offset = u4Offset + _rMSDCEEPROM.u4EEPBankBOffset;
		LOG(3, "Msdc_Read_EEPROMData from bankB!\n");
	}
		
    if (Loader_ReadMsdc(_rMSDCEEPROM.u4EEPPartIdA, u4Offset,(void*)u4MemPtr, u4MemLen) != u4MemLen)
    {
#ifdef __KERNEL__
            printk(KERN_ERR "Msdc_Read_EEPROMData fail!\n");
#else
            LOG(0, "Msdc_Read_EEPROMData fail!\n");
#endif
            return -1;
    }
	
#else
    if (Loader_ReadMsdc(_rMSDCEEPROM.u4EEPPartIdA, u4Offset,(void*)u4MemPtr, u4MemLen) != u4MemLen)
    {
#ifdef __KERNEL__
            printk(KERN_ERR "Msdc_Read_EEPROMData fail!\n");
#else
            LOG(0, "Msdc_Read_EEPROMData fail!\n");
#endif
            return -1;
    }
    
#endif    
    //LOG(3, "MSDC_EEPROM_Read end!\n");
    return i4Ret;
}

INT32 MSDC_EEPROM_Write(UINT32 u4Offset, UINT32 u4MemPtr, UINT32 u4MemLen)
{
    INT32 i4Ret = 0;
#ifdef CC_EEP2EMMC_DUAL_BANK_SUPPORT
    UINT32 u4HeaderIdx;
    UINT8 u1HeadderCount;
#endif

    VERIFY(_MSDC_EEPROM_Init() == 0);
    
    if ((u4Offset >= _rMSDCEEPROM.u4EEPSize) || ((u4Offset + u4MemLen) > _rMSDCEEPROM.u4EEPSize))
    {
        LOG(0, "MSDC_EEPROM_Write: out of boundry! u4Offset = 0x%x, u4MemLen = 0x%x\n", u4Offset, u4MemLen);
        
        ASSERT(0);
        return -1;
    }

#ifdef CC_EEP2EMMC_DUAL_BANK_SUPPORT
    if ((_rMSDCEEPROM.u1fgEEPAccessProtect & EEP2MSDC_MASK_ACCESS_PROHIBIT) != 0)
    {
#ifdef __KERNEL__
            printk(KERN_ERR "MSDC_EEPROM_Write access prohibit!\n");
#else
            LOG(0, "Msdc_Weite_EEPROM access prohibit!\n");
#endif
            return -1;
    }
    //read header first
    u4HeaderIdx = u4Offset + _rMSDCEEPROM.u4EEPHeaderOffset;
	if (Loader_ReadMsdc(_rMSDCEEPROM.u4EEPPartIdA, u4HeaderIdx,(void*)(&u1HeadderCount), 1) != 1)
    {
#ifdef __KERNEL__
            printk(KERN_ERR "Msdc_Write_Read_EEPROMHeader fail!\n");
#else
            LOG(0, "Msdc_Write_Read_EEPROMHeader fail!\n");
#endif
            return -1;
    }
	//decide which bank to write to
    if(u1HeadderCount == 0)
    {
        u4Offset = u4Offset + _rMSDCEEPROM.u4EEPBankBOffset;
		LOG(3, "Msdc_Write_EEPROMData to bankB!\n");
    }
	else
	{
	    u4Offset = u4Offset + _rMSDCEEPROM.u4EEPBankAOffset;
		LOG(3, "Msdc_Write_EEPROMData to bankA!\n");
	}
	//data write
    if (Loader_WriteMsdc(_rMSDCEEPROM.u4EEPPartIdA, u4Offset,(void*)u4MemPtr, u4MemLen) != u4MemLen)
    {
#ifdef __KERNEL__
        printk(KERN_ERR "MSDC_EEPROM_Write fail!\n");
#else
        LOG(0, "MSDC_EEPROM_Write fail!\n");
#endif
        return -1;
    }
    //update header count
    if(u1HeadderCount == 0) {
        u1HeadderCount = 1;
    }
	else {
        u1HeadderCount = 0;
    }
    if (Loader_WriteMsdc(_rMSDCEEPROM.u4EEPPartIdA, u4HeaderIdx,(void*)(&u1HeadderCount), 1) != 1)
    {
#ifdef __KERNEL__
            printk(KERN_ERR "Msdc_Write_Read_EEPROMHeader fail!\n");
#else
            LOG(0, "Msdc_Write_Read_EEPROMHeader fail!\n");
#endif
            return -1;
    }
	
#else

    if (Loader_WriteMsdc(_rMSDCEEPROM.u4EEPPartIdA, u4Offset,(void*)u4MemPtr, u4MemLen) != u4MemLen)
    {
#ifdef __KERNEL__
        printk(KERN_ERR "MSDC_EEPROM_Write fail!\n");
#else
        LOG(0, "MSDC_EEPROM_Write fail!\n");
#endif
        return -1;
    }
	
#endif
    
    return i4Ret;
}

#ifdef CC_EEP2EMMC_DUAL_BANK_SUPPORT
INT32 MSDC_EEP_Header_Read(UINT32 u4Offset, UINT32 u4MemPtr, UINT32 u4MemLen)
{
    INT32 i4Ret = 0;
    
    VERIFY(_MSDC_EEPROM_Init() == 0);
    
    if ((u4Offset >= _rMSDCEEPROM.u4EEPSize) || ((u4Offset + u4MemLen) > _rMSDCEEPROM.u4EEPSize))
    {
        LOG(0, "MSDC_EEP_Header_Read: out of boundry! u4Offset = 0x%x, u4MemLen = 0x%x\n", u4Offset, u4MemLen);
        
        ASSERT(0);
        return -1;
    }
    
	u4Offset = u4Offset + _rMSDCEEPROM.u4EEPHeaderOffset;		
    if (Loader_ReadMsdc(_rMSDCEEPROM.u4EEPPartIdA, u4Offset,(void*)u4MemPtr, u4MemLen) != u4MemLen)
    {
#ifdef __KERNEL__
            printk(KERN_ERR "Msdc_Read_EEPROMData fail!\n");
#else
            LOG(0, "Msdc_Read_EEPROMData fail!\n");
#endif
            return -1;
    }

    //LOG(3, "MSDC_EEPROM_Read end!\n");
    return i4Ret;
}


INT32 MSDC_EEP_Header_Write(UINT32 u4Offset, UINT32 u4MemPtr, UINT32 u4MemLen)
{
    INT32 i4Ret = 0;
    UINT32 u4HeaderIdx;
    UINT8 u1HeadderCount;

    VERIFY(_MSDC_EEPROM_Init() == 0);
    
    if ((u4Offset >= _rMSDCEEPROM.u4EEPSize) || ((u4Offset + u4MemLen) > _rMSDCEEPROM.u4EEPSize))
    {
        LOG(0, "MSDC_EEP_Header_Write: out of boundry! u4Offset = 0x%x, u4MemLen = 0x%x\n", u4Offset, u4MemLen);
        
        ASSERT(0);
        return -1;
    }

    u4Offset = u4Offset + _rMSDCEEPROM.u4EEPHeaderOffset;
	//data write
    if (Loader_WriteMsdc(_rMSDCEEPROM.u4EEPPartIdA, u4Offset,(void*)u4MemPtr, u4MemLen) != u4MemLen)
    {
#ifdef __KERNEL__
        printk(KERN_ERR "MSDC_EEPROM_Write fail!\n");
#else
        LOG(0, "MSDC_EEPROM_Write fail!\n");
#endif
        return -1;
    }
	
    return i4Ret;
}

INT32 MSDC_EEP_Init_Header_Read(UINT32 u4Offset, UINT32 u4MemPtr, UINT32 u4MemLen)
{
    INT32 i4Ret = 0;
    
    VERIFY(_MSDC_EEPROM_Init() == 0);
    
    if ((u4Offset >= _rMSDCEEPROM.u4EEPSize) || ((u4Offset + u4MemLen) > _rMSDCEEPROM.u4EEPSize))
    {
        LOG(0, "MSDC_EEP_Header_Read: out of boundry! u4Offset = 0x%x, u4MemLen = 0x%x\n", u4Offset, u4MemLen);
        
        ASSERT(0);
        return -1;
    }
    	
    if (Loader_ReadMsdc(_rMSDCEEPROM.u4EEPPartIdA, u4Offset,(void*)u4MemPtr, u4MemLen) != u4MemLen)
    {
#ifdef __KERNEL__
            printk(KERN_ERR "Msdc_Read_EEPROMData fail!\n");
#else
            LOG(0, "Msdc_Read_EEPROMData fail!\n");
#endif
            return -1;
    }

    //LOG(3, "MSDC_EEPROM_Read end!\n");
    return i4Ret;
}


INT32 MSDC_EEP_Init_Header_Write(UINT32 u4Offset, UINT32 u4MemPtr, UINT32 u4MemLen)
{
    INT32 i4Ret = 0;

    VERIFY(_MSDC_EEPROM_Init() == 0);
    
    if ((u4Offset >= _rMSDCEEPROM.u4EEPInitHeaderSize) || ((u4Offset + u4MemLen) > _rMSDCEEPROM.u4EEPInitHeaderSize))
    {
        LOG(0, "MSDC_EEP_Init_Header_Write: out of boundry! u4Offset = 0x%x, u4MemLen = 0x%x\n", u4Offset, u4MemLen);
        
        ASSERT(0);
        return -1;
    }
	//data write
    if (Loader_WriteMsdc(_rMSDCEEPROM.u4EEPPartIdA, u4Offset,(void*)u4MemPtr, u4MemLen) != u4MemLen)
    {
#ifdef __KERNEL__
        printk(KERN_ERR "MSDC_EEPROM_Write fail!\n");
#else
        LOG(0, "MSDC_EEPROM_Write fail!\n");
#endif
        return -1;
    }
	
    return i4Ret;
}

INT32 MSDC_EEP_Init_Data_Read(BOOL fgBankIndex, UINT32 u4Offset, UINT32 u4MemPtr, UINT32 u4MemLen)
{
    INT32 i4Ret = 0;
    
    VERIFY(_MSDC_EEPROM_Init() == 0);

    if ((u4Offset >= _rMSDCEEPROM.u4EEPSize) || ((u4Offset + u4MemLen) > _rMSDCEEPROM.u4EEPSize))
    {
        LOG(0, "MSDC_EEP_Init_Data_Read: out of boundry! u4Offset = 0x%x, u4MemLen = 0x%x\n", u4Offset, u4MemLen);
        
        ASSERT(0);
        return -1;
    }

    if(fgBankIndex)
    {
        u4Offset = u4Offset + _rMSDCEEPROM.u4EEPBankAOffset;
    }
	else
	{
	    u4Offset = u4Offset + _rMSDCEEPROM.u4EEPBankBOffset;
	}
    	
    if (Loader_ReadMsdc(_rMSDCEEPROM.u4EEPPartIdA, u4Offset,(void*)u4MemPtr, u4MemLen) != u4MemLen)
    {
#ifdef __KERNEL__
            printk(KERN_ERR "Msdc_Read_EEPROMData fail!\n");
#else
            LOG(0, "Msdc_Read_EEPROMData fail!\n");
#endif
            return -1;
    }	
    //LOG(3, "MSDC_EEPROM_Read end!\n");
    return i4Ret;
}


INT32 MSDC_EEP_Init_Data_Write(BOOL fgBankIndex, UINT32 u4Offset, UINT32 u4MemPtr, UINT32 u4MemLen)
{
    INT32 i4Ret = 0;

    VERIFY(_MSDC_EEPROM_Init() == 0);
    
    if ((u4Offset >= _rMSDCEEPROM.u4EEPSize) || ((u4Offset + u4MemLen) > _rMSDCEEPROM.u4EEPSize))
    {
        LOG(0, "MSDC_EEP_Init_Data_Write: out of boundry! u4Offset = 0x%x, u4MemLen = 0x%x\n", u4Offset, u4MemLen);
        
        ASSERT(0);
        return -1;
    }

    if(fgBankIndex)
    {
        u4Offset = u4Offset + _rMSDCEEPROM.u4EEPBankAOffset;//data A write
    }
	else
	{
	    u4Offset = u4Offset + _rMSDCEEPROM.u4EEPBankBOffset;//data B write
	}	
	 
	if (Loader_WriteMsdc(_rMSDCEEPROM.u4EEPPartIdA, u4Offset,(void*)u4MemPtr, u4MemLen) != u4MemLen)
    {
#ifdef __KERNEL__
        printk(KERN_ERR "MSDC_EEP_Init_Data_Write fail!\n");
#else
        LOG(0, "MSDC_EEP_Init_Data_Write fail!\n");
#endif
        return -1;
    }

    return i4Ret;
}


#endif

BOOL MSDCPART_IsEEPROM(UINT32 u4PartId)
{
    UINT32 u4DynPartId = DRVCUST_InitGet64(eNANDFlashDynamicPartitionID);
    
    if (u4PartId >= u4DynPartId)
    {
        UINT32 u4EEPPartIdA, u4EEPPartIdB;
        
        u4EEPPartIdA = DRVCUST_InitGet64(eNANDFlashPartIdEepromA);
        u4EEPPartIdB = DRVCUST_InitGet64(eNANDFlashPartIdEepromB);

        if ((u4PartId == u4EEPPartIdA) || (u4PartId == u4EEPPartIdB))
        {
        
            return TRUE;
        }
    }

    return FALSE;
}

//-------------------------------------------------------------------------
/** MSDCPART_WP_enable
*  MSDCPART_WP_enable() to read data from emmc flash.
*  @param  u8Offset        Lower 32 bits: offset to read data. Higher 32 bits: partition ID.
*  @param  u4MemPtr      User's read buffer address.
*  @param  u4MemLen     read length (bytes).
*  @retval 0   Success.
*  @retval others  Fail.
*/
//-------------------------------------------------------------------------
#if 0
MSDC_DevInfo_T MsdcDevInfo;

INT32 MSDCPART_WP_enable(UINT32 u4PartId)
{
    UINT32 PartWp = 0;
    UINT64 u8Offset = 0;
    UINT32 u4length = 0;
    INT32  i4Ret = MSDC_SUCCESS;
    //UINT32 blklen =512;
   // UINT32 WPUnitSize = MsdcDevInfo.WP_GrpSz;
 
    i4Ret = MsdcGetDevInfo((MSDC_DevInfo_T *)(&MsdcDevInfo));
    if(i4Ret == MSDC_FAILED)
    {
        LOG(0, "Get Dev Info failed !\n");  
    }
    
    PartWp = DRVCUST_InitGet64((QUERY_TYPE_T)(eNANDFlashPartWp0 + u4PartId));
    
    if(PartWp)
    {
       u8Offset = DRVCUST_InitGet64((QUERY_TYPE_T)(eNANDFlashPartOffset0 + u4PartId)); 
       u4length = DRVCUST_InitGet64((QUERY_TYPE_T)(eNANDFlashPartSize0 + u4PartId));
       i4Ret = MsdcWPCard(1,1,1,u8Offset);
    }
    else
    {   
       LOG(0, "The partition %d didn't need write protect !\n",u4PartId);        
       i4Ret = MSDC_FAILED;
    }
    return i4Ret;
}


INT32 MSDCPART_WP_disable(UINT32 u4PartId)
{
    UINT32 PartWp = 0;
    UINT64 u8Offset = 0;
    UINT32 u4length = 0;    
    INT32  i4Ret = MSDC_SUCCESS;
   // UINT32 blklen =512;
   // UINT32 WPUnitSize = MsdcDevInfo.WP_GrpSz;
    i4Ret = MsdcGetDevInfo((MSDC_DevInfo_T *)(&MsdcDevInfo));
    if(i4Ret == MSDC_FAILED)
    {
        LOG(0, "Get Dev Info failed !\n");  
    }
    
    PartWp = DRVCUST_InitGet64((QUERY_TYPE_T)(eNANDFlashPartWp0 + u4PartId));
    
    if(PartWp)
    {
       u8Offset = DRVCUST_InitGet64((QUERY_TYPE_T)(eNANDFlashPartOffset0 + u4PartId)); 
       u4length = DRVCUST_InitGet64((QUERY_TYPE_T)(eNANDFlashPartSize0 + u4PartId));
       i4Ret =  MsdcWPCard(1,1,0,u8Offset);
    }
    else
    {   
       LOG(0, "The partition %d didn't need write protect function!\n",u4PartId);        
       i4Ret =  MSDC_FAILED;; 
    }
    return i4Ret;
}


INT32 MSDCPART_WP_get_status(UINT32 u4PartId)
{
    UINT32 PartWp = 0;
    UINT64 u8Offset = 0;
    UINT32 u4length = 0;
    //UINT32 blklen =512;
    UINT32 pWP;
    UINT64 pWPType;
    INT32  i4Ret = MSDC_SUCCESS;
    //UINT32 WPUnitSize = MsdcDevInfo.WP_GrpSz;
 
    PartWp = DRVCUST_InitGet64((QUERY_TYPE_T)(eNANDFlashPartWp0 + u4PartId));
    
    if(PartWp)
    {
        u8Offset = DRVCUST_InitGet64((QUERY_TYPE_T)(eNANDFlashPartOffset0 + u4PartId)); 
        u4length = DRVCUST_InitGet64((QUERY_TYPE_T)(eNANDFlashPartSize0 + u4PartId));

       i4Ret = MsdcGetWPStatus(u8Offset,&pWP,&pWPType);
       LOG(0, "MSDCPART_WP_get_status, pWP :%x,pWPType :%x!\n",pWP,pWPType);        
    }
    else
    {   
       LOG(0, "The partition %d didn't need write protect function!\n",u4PartId);        
       i4Ret =  MSDC_FAILED;; 
    }
    
    return i4Ret;

}

#endif

//-------------------------------------------------------------------------
/** MSDCPART_Read
*  MSDCPART_Read() to read data from emmc flash.
*  @param  u8Offset        Lower 32 bits: offset to read data. Higher 32 bits: partition ID.
*  @param  u4MemPtr      User's read buffer address.
*  @param  u4MemLen     read length (bytes).
*  @retval 0   Success.
*  @retval others  Fail.
*/
//-------------------------------------------------------------------------
INT32 MSDCPART_Read(UINT64 u8Offset, UINT32 u4MemPtr, UINT32 u4MemLen)
{
    UINT16 u2PartId = 0;
    UINT32 u4Offset = 0;
    INT32  i4Ret = MSDC_SUCCESS;

    MSDCPART_RW_ENTRY;
    u2PartId = (u8Offset >> (UINT64)32) & 0xFFFF;
    u4Offset = (UINT32)u8Offset & 0xFFFFFFFF;

    // the high 32 bits as partition id.
    if(u2PartId >= (UINT32)MSDCPART_PARTNUM)
    {
        MSDCPART_RW_EXIT;
        return 1;
    }
    
#if defined(__KERNEL__)
    // The partno in kernel is start from 1, while mtkdriver is start from 0.
    i4Ret = msdc_partread(u2PartId+1, u4Offset, u4MemLen, (void *)u4MemPtr);
    MSDCPART_RW_EXIT;

    return i4Ret;
#else
    // get physical address by partition id.
    u8Offset = (UINT32)u8Offset & 0xFFFFFFFF;
    u8Offset += DRVCUST_InitGet64((QUERY_TYPE_T)(eNANDFlashPartOffset0 + u2PartId));

    i4Ret = eMMC2DRAM(u8Offset, (UINT32 *)u4MemPtr, u4MemLen, 0);
    MSDCPART_RW_EXIT;
    
    return i4Ret;
#endif
}

//-------------------------------------------------------------------------
/** MSDCPART_Write
*  MSDCPART_Write() to write data to emmc flash.
*  @param  u8Offset        Lower 32 bits: offset to write data. Higher 32 bits: partition ID.
*  @param  u4MemPtr      User's write buffer address.
*  @param  u4MemLen     Write length (bytes).
*  @retval 0   Success.
*  @retval others  Fail.
*/
//-------------------------------------------------------------------------
INT32 MSDCPART_Write(UINT64 u8Offset, UINT32 u4MemPtr, UINT32 u4MemLen)
{
    UINT16 u2PartId = 0;
    UINT32 u4Offset = 0;
    INT32  i4Ret = MSDC_SUCCESS;

    MSDCPART_RW_ENTRY;
    u2PartId = (u8Offset >> (UINT64)32) & 0xFFFF;
    u4Offset = (UINT32)u8Offset & 0xFFFFFFFF;
    
#ifdef CC_PARTITION_WP_SUPPORT
    if(wpConfig)
    {
        if(DRVCUST_InitGet((QUERY_TYPE_T)(eNANDFlashPartWp0 + u2PartId)))
        {
            LOG(0, "partition %d was write protected!\n",u2PartId);  
            MSDCPART_RW_EXIT;
            return 1;
        }    
    }
#endif
    // the high 32 bits as partition id.
    if(u2PartId >= (UINT32)MSDCPART_PARTNUM)
    {
        MSDCPART_RW_EXIT;
        return 1;
    }
    
#if defined(__KERNEL__)
    // The partno in kernel is start from 1, while mtkdriver is start from 0.
    i4Ret = msdc_partwrite(u2PartId+1, u4Offset, u4MemLen, (void *)u4MemPtr);
    MSDCPART_RW_EXIT;

    return i4Ret;
#else
    // get physical address by partition id.
    u8Offset = (UINT32)u8Offset & 0xFFFFFFFF;
    u8Offset += DRVCUST_InitGet64((QUERY_TYPE_T)(eNANDFlashPartOffset0 + u2PartId));

    i4Ret = eMMC2DRAM(u8Offset, (UINT32 *)u4MemPtr, u4MemLen, 1);
    MSDCPART_RW_EXIT;
    
    return i4Ret;
#endif
}

//-------------------------------------------------------------------------
/** MSDCPART_Erase
 *  Erase selected partition, and allocate new physical block to this partition.
 *  @param  u8Offset        Selected partition and block start index to be erased.
 *  @param  u4SectNum    sector number.
 *  @retval 0	        Success
 *  @retval Others	Fail
 */
//-------------------------------------------------------------------------
INT32 MSDCPART_Erase(UINT64 u8Offset, UINT32 u4SectNum)
{
    INT32  i4Ret = MSDC_SUCCESS;
    UINT16 u2PartId = (u8Offset >> (UINT64)32) & 0xFFFF;
#ifdef CC_PARTITION_WP_SUPPORT
    if(wpConfig)
    {
        if(DRVCUST_InitGet((QUERY_TYPE_T)(eNANDFlashPartWp0 + u2PartId)))
        {
            LOG(0, "partition %d was write protected!\n",u2PartId);  
            MSDCPART_RW_EXIT;
            return 1;
        }  
    }
#endif
    // the high 32 bits as partition id.
    if(u2PartId >= (UINT32)MSDCPART_PARTNUM)
    {
        return 1;
    }

    // get physical address by partition id.
    u8Offset = (UINT32)u8Offset & 0xFFFFFFFF;
    u8Offset += DRVCUST_InitGet64((QUERY_TYPE_T)(eNANDFlashPartOffset0 + u2PartId));
    
    return i4Ret;
}
#ifdef CC_PARTITION_WP_SUPPORT
//-------------------------------------------------------------------------
/** MSDCPART_WpConfig
 *  Enable the write protect of partitions
 *  @param  wp:0 disable write protect 1:enable write protect
 */
//-------------------------------------------------------------------------

void MSDCPART_WpConfig(UINT32 wp)
{
    if(wp)
    {
      wpConfig = 1;
      LOG(0, "partition write protect was enabled!\n");  
    }
    else
    {
      wpConfig = 0;
      LOG(0, "partition write protect was disabled!\n");  
    }
}
#endif

INT32 Loader_GetMsdcEraseSize(void)
{
/* the erase size must be aligned with partition size */
#if defined(CC_EMMC_ERASE_SIZE_ALIGNMENT_1M)
    UINT32 u4EraseSize = 1024*1024; 
#else
    UINT32 u4EraseSize = 256*1024;
#endif    
    return (INT32)u4EraseSize;
}

INT32 Snapshot_ReadMsdc(UINT32 u4PartId, UINT64 u8Offset, void *pvMemPtr, UINT32 u4MemLen)
{
	
    INT32  i4Ret = MSDC_SUCCESS;
#if defined(__KERNEL__)
    i4Ret = msdc_partread(u4PartId, u8Offset, u4MemLen, (void *)pvMemPtr);
#endif
	return i4Ret;
}


INT32 Snapshot_WriteMsdc(UINT32 u4PartId, UINT64 u8Offset, void *pvMemPtr, UINT32 u4MemLen)
{
	
    INT32  i4Ret = MSDC_SUCCESS;	
#if defined(__KERNEL__)
    i4Ret = msdc_partwrite(u4PartId, u8Offset, u4MemLen, (void *)pvMemPtr);
#endif
	return i4Ret;
}



INT32 Loader_ReadMsdc(UINT32 u4PartId, UINT32 u4Offset, void *pvMemPtr, UINT32 u4MemLen)
{
    UINT64 u8Offset = ((((UINT64)u4PartId)<<32) | u4Offset);
    
    if (MSDCPART_Read(u8Offset, (UINT32)pvMemPtr, u4MemLen) == 0)
    {
        return (INT32)u4MemLen;
    }
    else
    {
        return -1;
    }
}

INT32 Loader_WriteMsdc(UINT32 u4PartId, UINT32 u4Offset, void *pvMemPtr, UINT32 u4MemLen)
{
    UINT64 u8Offset = ((((UINT64)u4PartId)<<32) | u4Offset);
    
    if (MSDCPART_Write(u8Offset, (UINT32)pvMemPtr, u4MemLen) == 0)
    {
        return (INT32)u4MemLen;
    }
    else
    {
        return -1;
    }
}

INT32 Loader_WriteToBootArea1(void *pvMemPtr)
{

	UINT8 default_value = 0,new_value = 121;
	UINT64 u8Offset = 0;
    UINT32 u4MemLen = 40*1024;

    default_value = (UINT8)MsdcGetEXTCSD_BootPartition(179);
    if(0 != default_value)
    {
	      MsdcSetEXTCSD(179,new_value);
	
        if (MSDCPART_Write(u8Offset, (UINT32)pvMemPtr, u4MemLen) == 0)
        {
            MsdcSetEXTCSD(179,default_value);
            return (INT32)u4MemLen;
        }
        else
        {
            MsdcSetEXTCSD(179,default_value);
            return -1;
        }
    }
    else
    {
        return -1;
    }
  
}


#endif
