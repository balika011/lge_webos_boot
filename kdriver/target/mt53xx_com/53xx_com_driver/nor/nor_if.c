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
 * $RCSfile: nor_if.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file serialflash_if.c
 *  Brief of file nor_serflash.c.
 *  Details of file nor_serflash.c (optional).
 */


//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "nor_if.h"

#ifndef CC_NOR_DISABLE

#include "x_assert.h"
#include "x_typedef.h"
#include "x_os.h"
#include "x_printf.h"
#ifndef BD_NOR_ENABLE
#include "x_hal_5381.h"
#endif
#include "x_timer.h"
#include "nor_if.h"
#include "x_pdwnc.h"
#include "x_serial.h"
#include "x_pinmux.h"

//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------
#if !defined(CC_MTK_LOADER) && !defined(CC_MTK_LOADER)
#define NOR_SHARE_WITH_CI
#endif
//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
#define NOR_FLASH_BASE_ADDR     (IO_VIRT + 0x8000000)

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------
#ifdef BD_NOR_ENABLE
#define NOR_ENTRY    {NOR_SemaLock();}
#else
#define NOR_ENTRY    {NOR_Init();NOR_SemaLock();SFLASH_PinMux(0);}
#endif
#define NOR_EXIT     NOR_SemaUnLock()

//-----------------------------------------------------------------------------
// Imported variables
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Imported functions
//-----------------------------------------------------------------------------
#ifdef BD_NOR_ENABLE
   #ifndef printf
   #define printf      Printf      //
   #endif
#endif

//-----------------------------------------------------------------------------
// Static function forward declarations
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------
#if defined( NOR_SHARE_WITH_CI)
static HANDLE_T _hNOREntrySema;
#endif
static UINT32 _u4NORXDelay = 4*1024;
//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Inter-file functions
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Public functions
//---------------------------------------------------------------------------
//-----------------------------------------------------------------------------
/** _NOR_LockInit()
 *  Lock sempahore for thread safe
 */
//-----------------------------------------------------------------------------
#if defined( NOR_SHARE_WITH_CI)
static VOID _NOR_LockInit(VOID)
{
    static BOOL _fgNORLockInit = FALSE;
    if(_fgNORLockInit)
    {
        return;
    }
        VERIFY(OSR_OK == x_sema_create(&_hNOREntrySema, X_SEMA_TYPE_BINARY
                , X_SEMA_STATE_UNLOCK) );
    _fgNORLockInit = TRUE;
}
#endif

//-----------------------------------------------------------------------------
/** NOR_ApiLock()
 *  Lock sempahore for thread safe
 */
//-----------------------------------------------------------------------------
VOID NOR_SemaLock(VOID)
{

#if defined( NOR_SHARE_WITH_CI)
    _NOR_LockInit();
    VERIFY(OSR_OK == x_sema_lock(_hNOREntrySema, X_SEMA_OPTION_WAIT));
#endif
}

//-------------------------------------------------------------------------
/** NOR_SemaUnLock
 *  Unlock semaphore for thread safe.
 */
//-------------------------------------------------------------------------
VOID NOR_SemaUnLock(VOID)
{
#if defined( NOR_SHARE_WITH_CI)
    VERIFY(OSR_OK == x_sema_unlock(_hNOREntrySema));
#endif
}


//-----------------------------------------------------------------------------
/** NOR_Init() to read data from nor flash.
 *  @return 0 successful, otherwise failed.
 */
//-----------------------------------------------------------------------------
INT32 NOR_Init(VOID)
{
    INT32 i4Ret;
    static BOOL _fgNORInit = FALSE;
    if (_fgNORInit)
    {
        return 0;
    }
    _fgNORInit = TRUE;

    NOR_ENTRY;
    i4Ret = SFLASH_Init();
    NOR_EXIT;

    return i4Ret;
}

EXTERN BOOL _fgPerforTest;
//-----------------------------------------------------------------------------
/** NOR_Read() to read data from nor flash.
 *  @return 0 successful, otherwise failed.
 */
//-----------------------------------------------------------------------------
INT32 NOR_Read(UINT64 u8Offset, UINT32 u4MemPtr, UINT32 u4MemLen)
{
    UINT32 u4Offset = (UINT32) u8Offset;

    NOR_ENTRY;
#if !defined(__KERNEL__)
    #if defined(CC_MT5360B)
    SFLASH_Read( 0, u4MemPtr, 1);
    SFLASH_Read( u4Offset, u4MemPtr, u4MemLen);
    #else
	if(_fgPerforTest)
	{
		SFLASH_Read( 0, u4MemPtr, 1);
		SFLASH_Read( u4Offset, u4MemPtr, u4MemLen);
	}
	else
    {
        x_memcpy((void *)(u4MemPtr),(void *)(NOR_FLASH_BASE_ADDR), 1);
        x_memcpy((void *)(u4MemPtr),(void *)(NOR_FLASH_BASE_ADDR + u4Offset), u4MemLen);
	}
    #endif
#else
    SFLASH_Read( 0, u4MemPtr, 1);
    SFLASH_Read( u4Offset, u4MemPtr, u4MemLen);
#endif

    HAL_Delay_us(200);
    NOR_EXIT;

    return 0;
}

//-----------------------------------------------------------------------------
/** NOR_EraseBlk() to erase the block which the offset is at.
 *  @return 0 successful, otherwise failed.
 */
//-----------------------------------------------------------------------------
INT32 NOR_EraseBlk(UINT32 u4Offset)
{
    INT32 i4Ret;

    NOR_ENTRY;
    i4Ret = SFLASH_EraseOneBlk(u4Offset);
    NOR_EXIT;

    return i4Ret;
}

//-----------------------------------------------------------------------------
/** NOR_EraseAddr() to erase data to 0xff on nor flash.
 *  @return 0 successful, otherwise failed.
 */
//-----------------------------------------------------------------------------
INT32 NOR_EraseAddr(UINT32 u4Offset, UINT32 u4ByteCount)
{
    INT32 i4Ret;

    NOR_ENTRY;
    i4Ret = SFLASH_EraseAddr(u4Offset, u4ByteCount);
    NOR_EXIT;

    return i4Ret;
}
//-----------------------------------------------------------------------------
/** NOR_queue() to show states of nor replace to eeprom.
 *  @return 0 successful, otherwise failed.
 */
//-----------------------------------------------------------------------------
INT32 NOR_Queue(void)
{
    INT32 i4Ret;

	#ifdef REPLACE_EEPROM_WITH_NOR
	i4Ret = Get_NOR_EEPROM_INFO();
	#else
	i4Ret = 0x1;
	#endif

    return i4Ret;
}


//-----------------------------------------------------------------------------
/** NOR_EraseChip() to erase data to 0xff on nor flash.
 *  @return 0 successful, otherwise failed.
 */
//-----------------------------------------------------------------------------
INT32 NOR_EraseChip(UINT32 u4FlashIndex)
{
    INT32 i4Ret;

    NOR_ENTRY;
    i4Ret = SFLASH_EraseChip(u4FlashIndex);
    NOR_EXIT;

    return i4Ret;
}


//-----------------------------------------------------------------------------
/** NOR_Write to write data to nor flash.
 *  @return 0 successful, otherwise failed.
 */
//-----------------------------------------------------------------------------
INT32 NOR_Write(UINT64 u8Offset, UINT32 u4MemPtr, UINT32 u4ByteCount)
{
    INT32 i4Ret;
    UINT32 u4WriteCnt;
    UINT32 u4WriteOffset;
    u4WriteOffset = 0;
    do{
    NOR_ENTRY;
        if (u4ByteCount > _u4NORXDelay)
        {
            u4WriteCnt = _u4NORXDelay;
        }
        else
        {
            u4WriteCnt = u4ByteCount;
        }
        i4Ret = SFLASH_Write((UINT32)(u8Offset+u4WriteOffset), (u4MemPtr+u4WriteOffset), u4WriteCnt);
        u4WriteOffset += u4WriteCnt;
        u4ByteCount -= u4WriteCnt;
    NOR_EXIT;
    #if defined(CC_MT8295)
        x_thread_delay(5);
    #endif
    }while(u4ByteCount);

    return i4Ret;
}

//-----------------------------------------------------------------------------
/** NOR_WriteByte() to write single byte on nor flash.
 *  @return 0 successful, otherwise failed.
 */
//-----------------------------------------------------------------------------
INT32 NOR_WriteByte(UINT32 u4Offset, UINT32 u4Data)
{
    INT32 i4Ret;

    NOR_ENTRY;
    i4Ret = SFLASH_WriteOneByte(u4Offset, (UINT8)(u4Data&0xFF));
    NOR_EXIT;

    return i4Ret;
}


//-----------------------------------------------------------------------------
/** NOR_WrNoE() to write data on nor flash and erase automaticallly if need.
 *  @return 0 successful, otherwise failed.
 */
//-----------------------------------------------------------------------------
INT32 NOR_WrNoE(UINT32 u4Offset, UINT32 u4MemPtr, UINT32 u4MemLen)
{
    INT32 i4Ret;

    NOR_ENTRY;
    i4Ret = SFLASH_WrNoE(u4Offset, u4MemPtr, u4MemLen);
    NOR_EXIT;

    return i4Ret;
}

//-----------------------------------------------------------------------------
/** NOR_Diag() diagnostic function to check norflash status.
 */
//-----------------------------------------------------------------------------
INT32 NOR_CalCRC(UINT32 u4Addr, UINT32 u4Size, UINT32* pu4Checksum)
{
    INT32 i4Ret;

    NOR_ENTRY;
    i4Ret = SFLASH_CalCRC(u4Addr, u4Size, pu4Checksum);
    NOR_EXIT;

    return i4Ret;
}




//-----------------------------------------------------------------------------
/** Brief of NOR_EnableDMA.
 *  Details of NOR_EnableDMA (optional).
 */
//-----------------------------------------------------------------------------
INT32 NOR_EnableDMA(UINT32 u4SrcAddr, UINT32 u4DestAddr, UINT32 u4Size)
{
    INT32 i4Ret;

    NOR_ENTRY;
    i4Ret = SFLASH_EnableDMA(u4SrcAddr, u4DestAddr, u4Size);
    NOR_EXIT;

    return i4Ret;
}
#ifndef BD_NOR_ENABLE

//-----------------------------------------------------------------------------
/** NOR_GetSize() to get the size of norflash by bytes.
 *  @return 0 successful, otherwise failed.
 */
//-----------------------------------------------------------------------------
INT32 NOR_GetSize()
{
    SECTTBL_T rBlkMap;
    UINT32 u4Size;

    NOR_ENTRY;
    VERIFY(SFLASH_EraseBlockMap(&rBlkMap, &u4Size) == 0);
    NOR_EXIT;

    return (INT32)u4Size;
}


//-----------------------------------------------------------------------------
/** NOR_EraseMap() to get the norflash sector map.
 *  @return 0 successful, otherwise failed.
 */
//-----------------------------------------------------------------------------
INT32 NOR_EraseMap(SECTTBL_T *prBlkMap)
{
    INT32 i4Ret;

    NOR_ENTRY;
    i4Ret = SFLASH_EraseBlockMap(prBlkMap, NULL);
    NOR_EXIT;

    return i4Ret;
}

//-----------------------------------------------------------------------------
/** NOR_Diag() diagnostic function to check norflash status.
 */
//-----------------------------------------------------------------------------
INT32 NOR_Diag(VOID)
{
    INT32 i4Ret;

    NOR_ENTRY;
    i4Ret = SFLASH_Diag();
    NOR_EXIT;

    return i4Ret;
}
//-----------------------------------------------------------------------------
/** NOR_ReadProgram() to read data from nor flash.
 *  @return 0 successful, otherwise failed.
 */
//-----------------------------------------------------------------------------
INT32 NOR_ReadProgram(UINT64 u8Offset, UINT32 u4MemPtr, UINT32 u4MemLen)
{
    UINT32 u4Offset = (UINT32) u8Offset;

    NOR_ENTRY;

    SFLASH_Read( 0, u4MemPtr, 1);
    SFLASH_ReadProgram( u4Offset, u4MemPtr, u4MemLen);


    HAL_Delay_us(200);
    NOR_EXIT;

    return 0;
}

//-----------------------------------------------------------------------------
/** NOR_ReadRISCProgram() to read data from nor flash.
 *  @return 0 successful, otherwise failed.
 */
//-----------------------------------------------------------------------------
INT32 NOR_ReadRISCProgram(UINT64 u8Offset, UINT32 u4MemPtr, UINT32 u4MemLen)
{
    UINT32 u4Offset = (UINT32) u8Offset;

	NOR_ENTRY;
    SFLASH_Read( 0, u4MemPtr, 1);
    SFLASH_ReadRISCProgram( u4Offset, u4MemPtr, u4MemLen);


    HAL_Delay_us(200);
    NOR_EXIT;

    return 0;
}
//-----------------------------------------------------------------------------
/** NOR_WriteProgram to write data to nor flash.
 *  @return 0 successful, otherwise failed.
 */
//-----------------------------------------------------------------------------
INT32 NOR_WriteProgram(UINT64 u8Offset, UINT32 u4MemPtr, UINT32 u4ByteCount)
{
    INT32 i4Ret;
    UINT32 u4WriteCnt;
    UINT32 u4WriteOffset;
    u4WriteOffset = 0;
    do{
    NOR_ENTRY;
        if (u4ByteCount > _u4NORXDelay)
        {
            u4WriteCnt = _u4NORXDelay;
        }
        else
        {
            u4WriteCnt = u4ByteCount;
        }
        i4Ret = SFLASH_WriteProgram((UINT32)(u8Offset+u4WriteOffset), (u4MemPtr+u4WriteOffset), u4WriteCnt);
        u4WriteOffset += u4WriteCnt;
        u4ByteCount -= u4WriteCnt;
    NOR_EXIT;
    #if defined(CC_MT8295)
        x_thread_delay(5);
    #endif
    }while(u4ByteCount);

    return i4Ret;
}
//-----------------------------------------------------------------------------
/** NOR_WriteRISCProgram to write data to nor flash.
 *  @return 0 successful, otherwise failed.
 */
//-----------------------------------------------------------------------------
INT32 NOR_WriteRISCProgram(UINT64 u8Offset, UINT32 u4MemPtr, UINT32 u4ByteCount)
{
    INT32 i4Ret;
    UINT32 u4WriteCnt;
    UINT32 u4WriteOffset;
    u4WriteOffset = 0;
    do{
    NOR_ENTRY;
        if (u4ByteCount > _u4NORXDelay)
        {
            u4WriteCnt = _u4NORXDelay;
        }
        else
        {
            u4WriteCnt = u4ByteCount;
        }
        i4Ret = SFLASH_WriteRISCProgram((UINT32)(u8Offset+u4WriteOffset), (u4MemPtr+u4WriteOffset), u4WriteCnt);
        u4WriteOffset += u4WriteCnt;
        u4ByteCount -= u4WriteCnt;
    NOR_EXIT;
    #if defined(CC_MT8295)
        x_thread_delay(5);
    #endif
    }while(u4ByteCount);

    return i4Ret;
}


//-----------------------------------------------------------------------------
/** NOR_PageProgramWrite to write data to nor flash.
 *  @return 0 successful, otherwise failed.
 */
//-----------------------------------------------------------------------------
INT32 NOR_PageProgramWrite(UINT64 u8Offset, UINT32 u4MemPtr, UINT32 u4ByteCount)
{
    INT32 i4Ret;
    UINT32 u4WriteCnt;
    UINT32 u4WriteOffset;
    u4WriteOffset = 0;
    do{
    NOR_ENTRY;
        if (u4ByteCount > _u4NORXDelay)
        {
            u4WriteCnt = _u4NORXDelay;
        }
        else
        {
            u4WriteCnt = u4ByteCount;
        }
	
        i4Ret = SFLASH_PagePragramWrite((UINT32)(u8Offset+u4WriteOffset), (u4MemPtr+u4WriteOffset), u4WriteCnt);
        u4WriteOffset += u4WriteCnt;
        u4ByteCount -= u4WriteCnt;
    NOR_EXIT;
    #if defined(CC_MT8295)
        x_thread_delay(5);
    #endif
    }while(u4ByteCount);

    return i4Ret;
}

//-----------------------------------------------------------------------------
/** NOR_PageProgramCountWrite to write data to nor flash.
 *  @return 0 successful, otherwise failed.
 */
//-----------------------------------------------------------------------------
INT32 NOR_PageProgramCountWrite(UINT64 u8Offset, UINT32 u4MemPtr, UINT32 u4ByteCount, UINT8 u1PageCount)
{
    INT32 i4Ret;
    UINT32 u4WriteCnt;
    UINT32 u4WriteOffset;
    u4WriteOffset = 0;
	printf("...u4ByteCount = 0x%x",u4ByteCount);
    do{
    NOR_ENTRY;
    if (u4ByteCount > (4*_u4NORXDelay))
    {
        u4WriteCnt = (4*_u4NORXDelay);
    }
    else
    {
        u4WriteCnt = u4ByteCount;
    }
	printf("...u4WriteCnt = 0x%x",u4WriteCnt);
    i4Ret = SFLASH_PagePragramCountWrite((UINT32)(u8Offset+u4WriteOffset), (u4MemPtr+u4WriteOffset), u4WriteCnt, u1PageCount);
    u4WriteOffset += u4WriteCnt;
    u4ByteCount -= u4WriteCnt;
    NOR_EXIT;
    #if defined(CC_MT8295)
        x_thread_delay(5);
    #endif
    }while(u4ByteCount);

    return i4Ret;
}
//-----------------------------------------------------------------------------
/** NOR_Erase() to erase data to 0xff on nor flash.
 *  @return 0 successful, otherwise failed.
 */
//-----------------------------------------------------------------------------
INT32 NOR_Erase(UINT32 u4SectIdx, UINT32 u4SectNum)
{
    INT32 i4Ret;

    NOR_ENTRY;
    i4Ret = SFLASH_EraseSectIdx(u4SectIdx, u4SectNum);
    NOR_EXIT;

    return i4Ret;
}

//-----------------------------------------------------------------------------
/** NOR_Backup()
*/
//-----------------------------------------------------------------------------
INT32 NOR_Backup(UINT32 u4Addr, UINT32 u4Size)
{
   UINT8 au1Buf[64]={0};
   UINT32 u4Count, i;

		vIO32Write4B(PDWNC_WDTCTL, 0);//PDWNC_WRITE32(REG_RW_WATCHDOG_EN, 0);

   // Return Key
   SerPollPutChar(0x5A);
   SerPollPutChar(0x35);
   SerPollPutChar(0xA5);
   SerPollPutChar(0x33);
   SerPollPutChar(0x55);
   SerPollPutChar(0x38);
   SerPollPutChar(0xAA);
   SerPollPutChar(0x31);

   // Return Size
   SerPollPutChar((UINT8)((u4Size>>24)&0xFF));
   SerPollPutChar((UINT8)((u4Size>>16)&0xFF));
   SerPollPutChar((UINT8)((u4Size>>8)&0xFF));
   SerPollPutChar((UINT8)(u4Size&0xFF));

   while(u4Size > 0)
   {
       if(u4Size > 16)
       {
           u4Count = 16;
       }
       else
       {
           u4Count = u4Size;
       }

       // Read flash
       if(NOR_Read((UINT64)u4Addr, (UINT32)au1Buf, u4Count) != 0)
       {
           return -1;
       }

       // Send to UART
       for(i=0; i<u4Count; i++)
       {
           SerPollPutChar(au1Buf[i]);
       }

       u4Addr += u4Count;
       u4Size -= u4Count;
   }

   return 0;
}
#else
INT32 NOR_Enter4B(void)
{
   return SFLASH_Enter4B();
}
INT32 NOR_Enter4BitRead(void)
{
   return SFLASH_Enter4BitRead();
}
INT32 NOR_Exit4BitRead(void)
{
   return SFLASH_Exit4BitRead();
}

INT32 NOR_Exit4B(void)
{
   return SFLASH_Exit4B();
}

INT32 NOR_4BitRead(UINT32 u4Offset, UINT32 u4MemPtr, UINT32 u4ByteCount)
{
   return SFLASH_4BitRead(u4Offset, u4MemPtr, u4ByteCount);
}
INT32 NOR_4Byte4BitRead(UINT32 u4Offset, UINT32 u4MemPtr, UINT32 u4ByteCount)
{
   return SFLASH_4Byte4BitRead(u4Offset, u4MemPtr, u4ByteCount);
}

INT32 NOR_4BWrite(UINT32 u4Offset, UINT32 u4MemPtr, UINT32 u4ByteCount)
{
   return SFLASH_4BWrite(u4Offset, u4MemPtr, u4ByteCount);
}	

INT32 NOR_4BEraseAddr(UINT32 u4Offset, UINT32 u4ByteLen)
{
   return SFLASH_4BEraseAddr(u4Offset, u4ByteLen);
}
void NOR_WriteCfg(UINT32 uAddr)
{
   return SFLASH_WriteCfg(uAddr);
}

#endif
#ifdef REPLACE_EEPROM_WITH_NOR
//-----------------------------------------------------------------------------
/** NOR_GetInfo() get NOR chip information.
 */
//-----------------------------------------------------------------------------

INT32 NOR_GetInfo(SFLASH_INFO_T *prNORInfo)
{
	INT32 i4Ret;
	if(i4Ret = NOR_Init())
	{
		return i4Ret;
	}
	
    if (prNORInfo == NULL)
    {
        return 1;
    }

    SFLASHHW_GetFlashInfo(prNORInfo);
    return 0;
}
#endif/*REPLACE_EEPROM_WITH_NOR*/

#ifdef CC_SUPPORT_STR
//-----------------------------------------------------------------------------
/** NOR_pm_str_suspend
 */
//-----------------------------------------------------------------------------
void NOR_pm_str_suspend(void)
{
	//flash_ck clock power down
	CKGEN_WRITE32(0x20C, CKGEN_READ32(0x20C) | (1U << 7));
}


//-----------------------------------------------------------------------------
/** NOR_pm_str_resume
 */
//-----------------------------------------------------------------------------
void NOR_pm_str_resume(void)
{
    //flash_ck clock is normal clock
	CKGEN_WRITE32(0x20C, CKGEN_READ32(0x20C) & ~(1U << 7));
}
#endif // CC_SUPPORT_STR

#else

INT32 NOR_Init(void)
{
	return ERR_NOT_SUPPORT_NOR;
}

INT32 NOR_Read(UINT64 u8Offset, UINT32 u4MemPtr, UINT32 u4MemLen)
{
   return ERR_NOT_SUPPORT_NOR;
}

INT32 NOR_EraseBlk(UINT32 u4Offset)
{
   return ERR_NOT_SUPPORT_NOR;
}

INT32 NOR_Erase(UINT32 u4SectIdx, UINT32 u4SectNum)
{
   return ERR_NOT_SUPPORT_NOR;
}

INT32 NOR_EraseAddr(UINT32 u4Offset, UINT32 u4ByteCount)
{
  return ERR_NOT_SUPPORT_NOR;
}

INT32 NOR_EraseChip(UINT32 u4FlashIndex)
{
	return ERR_NOT_SUPPORT_NOR;
}

INT32 NOR_Write(UINT64 u8Offset, UINT32 u4MemPtr, UINT32 u4ByteCount)
{
  return ERR_NOT_SUPPORT_NOR;
}

INT32 NOR_WriteByte(UINT32 u4Offset, UINT32 u4Data)
{
	return ERR_NOT_SUPPORT_NOR;	
}

INT32 NOR_GetSize(void)
{
  return ERR_NOT_SUPPORT_NOR;	
}

INT32 NOR_CalCRC(UINT32 u4Addr, UINT32 u4Size, UINT32* pu4Checksum)
{
  return ERR_NOT_SUPPORT_NOR;	
}

INT32 NOR_WrNoE(UINT32 u4Offset, UINT32 u4MemPtr, UINT32 u4MemLen)
{
  return ERR_NOT_SUPPORT_NOR;	
}

INT32 NOR_Diag(VOID)
{
	return ERR_NOT_SUPPORT_NOR;
}

INT32 NOR_EraseMap(SECTTBL_T *prBlkMap)
{
	return ERR_NOT_SUPPORT_NOR;
}


//-----------------------------------------------------------------------------
/** NOR_ApiLock()
 *  Lock sempahore for thread safe
 */
//-----------------------------------------------------------------------------
VOID NOR_SemaLock(VOID)
{

}

//-------------------------------------------------------------------------
/** NOR_SemaUnLock
 *  Unlock semaphore for thread safe.
 */
//-------------------------------------------------------------------------
VOID NOR_SemaUnLock(VOID)
{

}

#ifdef REPLACE_EEPROM_WITH_NOR
INT32 NOR_GetInfo(SFLASH_INFO_T *prNORInfo)
{
  	return ERR_NOT_SUPPORT_NOR;   
}
#endif/*REPLACE_EEPROM_WITH_NOR*/
#endif/*CC_NOR_DISABLE*/
