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
/******************************************************************************
* [File]			msdc_if.h
* [Version]			v1.0
* [Revision Date]	2011-05-04
* [Author]			Shunli Wang, shunli.wang@mediatek.inc, 82896, 2011-05-04
* [Description]
*	MSDC Driver Header File
* [Copyright]
*	Copyright (C) 2011 MediaTek Incorporation. All Rights Reserved.
******************************************************************************/

#ifndef MSDC_IF_H
#define MSDC_IF_H

//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------

#include "x_lint.h"
#include "x_typedef.h"
#include "x_os.h"
#include "x_assert.h"
#include "x_printf.h"

#define MSDC_NAME_MAX_SIZE              (32)
#ifdef CC_MTD_SUPPORT_64_PARTITION
	#define MSDCPART_PARTNUM                (64)
#else
	#define MSDCPART_PARTNUM                (32)
#endif	

// For SD use
#define MSDC_SD_NONE                    (0)
#define MSDC_SD_INSERT                  (1)
#define MSDC_SD_UNKNOWN                 (2)

typedef struct
{
    UINT8           u1PartId;
    CHAR            cName[MSDC_NAME_MAX_SIZE];
    UINT32          u4Offset;
    UINT32          u4Size;
} MSDCPART_STRUCT_T;

typedef struct
{
    UINT32 Boot_Partition_Sz;	
	
    UINT32 Write_BlkSz;
    UINT32 Erase_GrpSz;
    UINT32 WP_GrpSz;
    
    UINT64 Dev_Sz;	
}MSDC_DevInfo_T;

EXTERN VOID MSDC_HostSet(UINT32 u4Ch);
EXTERN UINT32 MSDC_HostGet(VOID);
EXTERN VOID MSDC_EmmcHostSet(UINT32 u4Ch);
EXTERN VOID  MSDC_ApiLock(HANDLE_T *hhSema);
EXTERN VOID  MSDC_ApiUnLock(HANDLE_T *hhSema);
EXTERN VOID MSDC_PinMux(UINT32 u4Ch);
EXTERN INT32 MsdcCardIdentify(VOID);
EXTERN INT32 MsdcSysInit(VOID);
EXTERN INT32 MsdcSetDataMode(UINT32 fgDataMode);
EXTERN INT32 MsdcSetDataEndMode(UINT32 fgEndMode);
EXTERN INT32 MsdcSetDataBusWidth(UINT32 busWidth);
EXTERN INT32 MsdcSetSpeedMode(UINT32 SpeedMode);
EXTERN INT32 MsdcSetDDRMode(UINT32 fgDDRMode);
EXTERN INT32 MsdcSetBusClock(UINT32 busClock);
EXTERN INT32 MsdcSetContinueClock(UINT32 fgContinueClock);
EXTERN INT32 MsdcPrintConfigInfo(VOID);
EXTERN INT32 MsdcGetRegisters(UINT32 fgWhich);
EXTERN INT32 MsdcGetDevInfo(MSDC_DevInfo_T *MsdcDevInfo);
EXTERN INT32 MsdcIdentyCard(BOOL fgForceIdentCard);
EXTERN INT32 MsdcReadCard(UINT64 u8Addr, UINT32 *pu4Buf, UINT32 u4Length);
EXTERN INT32 MsdcWriteCard(UINT32 *u4SrcAddr, UINT64 u8ByteOffset, UINT32 u4Len);
EXTERN INT32 MsdcEraseCard(UINT64 u8ByteOffset, UINT32 u4Len, UINT32 u4EraseType);
EXTERN INT32 MsdcWPCard(UINT32 u4WPLevel, UINT32 u4WPType, UINT32 u4fgEn, UINT64 wpAddr);
EXTERN VOID MsdcToggleContClock(VOID);
EXTERN INT32 MsdcIdentReadMMC(UINT32 *pu4Buf, UINT64 u8Addr, UINT32 u4Length);
EXTERN INT32 eMMC2DRAM(UINT64 u8Addr, UINT32 *pSrcOrDes, UINT32 u4Length, UINT32 u4Type);

EXTERN INT32 MSDCPART_Read(UINT64 u8Offset, UINT32 u4MemPtr, UINT32 u4MemLen);
EXTERN INT32 MSDCPART_Write(UINT64 u8Offset, UINT32 u4MemPtr, UINT32 u4MemLen);
EXTERN INT32 MSDCPART_Erase(UINT64 u8Offset, UINT32 u4SectNum);
#ifdef CC_PARTITION_WP_SUPPORT
EXTERN void  MSDCPART_WpConfig(UINT32 wp);
#endif
EXTERN VOID  MSDCPART_ShowPartition(VOID);

// For SD use
EXTERN INT32 MSDCDetectSDGPIO(VOID);
EXTERN INT32 MSDCSDSwitchVoltage(INT32 hV);
EXTERN INT32 MSDCSDInit(BOOL fgForce);
EXTERN INT32 MSDCGetSDInsert(BOOL fgForce);
EXTERN INT32 MSDCGetSDDevInfo(MSDC_DevInfo_T *MsdcDevInfo);
EXTERN INT32 SD2DRAM(UINT64 u8Addr, UINT32 *pSrcOrDes, UINT32 u4Length, UINT32 u4Type);

// For loader use
EXTERN INT32 Loader_GetMsdcEraseSize(VOID);
EXTERN INT32 Loader_ReadMsdc(UINT32 u4PartId, UINT32 u4Offset, VOID *pvMemPtr, UINT32 u4MemLen);
EXTERN INT32 Loader_WriteMsdc(UINT32 u4PartId, UINT32 u4Offset, VOID *pvMemPtr, UINT32 u4MemLen);
EXTERN INT32 Loader_EraseMsdc(UINT32 u4PartId, UINT32 u4Offset, UINT32 u4MemLen);
EXTERN INT32 Loader_WriteToBootArea1(VOID *pvMemPtr);
EXTERN INT32 Snapshot_ReadMsdc(UINT32 u4PartId, UINT64 u8Offset, void *pvMemPtr, UINT32 u4MemLen);
EXTERN INT32 Snapshot_WriteMsdc(UINT32 u4PartId, UINT64 u8Offset, void *pvMemPtr, UINT32 u4MemLen);

EXTERN INT32 MSDCTest_HS200Tuning(UINT32 target);
EXTERN INT32 MSDCTest_HS200Calibration(UINT32 u4Offset, UINT32 u4Size);

// For sd/mmc test use
#ifdef CC_MSDC_SDMMC_TEST
EXTERN INT32 MSDCTest_Register(VOID);
EXTERN INT32 MSDCTest_Detection(VOID);
EXTERN INT32 MSDCTest_Protection(VOID);
EXTERN INT32 MSDCTest_DataAutoTest(UINT32 *pSrcWrite,  UINT32 *pDestRead);
EXTERN INT32 MSDCTest_DataEndAutoTest(UINT32 *pSrcWrite,  UINT32 *pDestRead);
EXTERN INT32 MSDCTest_BootModeAutoTest(UINT32 *pSrcWrite, UINT32 *pDestRead);
EXTERN INT32 MSDCTest_BootInt(UINT32 *pSrcWrite, UINT32 *pDestRead, UINT32 test_mode);
EXTERN INT32 MSDCTest_AutoTuning(UINT32 target);
#endif

// for sdio test use
#ifdef CC_MSDC_SDIO_TEST
EXTERN INT32 MSDCSDIO_Init(VOID);
EXTERN INT32 MSDCSDIO_GetCIA(UINT32 fgMode);
EXTERN INT32 MSDCSDIO_ParseCIA(VOID);
EXTERN INT32 MSDCSDIO_SetBusWidth(UINT32 buswidth);
EXTERN INT32 MSDCSDIO_SetBlockLen(UINT32 blklen);
EXTERN INT32 MSDCSDIO_ReadTest(UINT32 buff, UINT32 offset, UINT32 len, UINT32 mode);
EXTERN INT32 MSDCSDIO_ReadAutoTest(VOID);
#endif
// for eeprom replacement

EXTERN INT32 MSDC_EEPROM_Read(UINT32 u4Offset, UINT32 u4MemPtr, UINT32 u4MemLen);
EXTERN INT32 MSDC_EEPROM_Write(UINT32 u4Offset, UINT32 u4MemPtr, UINT32 u4MemLen);

#ifdef CC_EEP2EMMC_DUAL_BANK_SUPPORT
EXTERN INT32 MSDC_EEP_Header_Read(UINT32 u4Offset, UINT32 u4MemPtr, UINT32 u4MemLen);
EXTERN INT32 MSDC_EEP_Header_Write(UINT32 u4Offset, UINT32 u4MemPtr, UINT32 u4MemLen);
EXTERN INT32 MSDC_EEP_Init_Header_Read(UINT32 u4Offset, UINT32 u4MemPtr, UINT32 u4MemLen);
EXTERN INT32 MSDC_EEP_Init_Header_Write(UINT32 u4Offset, UINT32 u4MemPtr, UINT32 u4MemLen);
EXTERN INT32 MSDC_EEP_Init_Data_Read(BOOL fgBankIndex, UINT32 u4Offset, UINT32 u4MemPtr, UINT32 u4MemLen);
EXTERN INT32 MSDC_EEP_Init_Data_Write(BOOL fgBankIndex, UINT32 u4Offset, UINT32 u4MemPtr, UINT32 u4MemLen);
#endif

#endif /* MSDC_IF_H */
