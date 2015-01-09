/*----------------------------------------------------------------------------*
 * No Warranty                                                                *
 * Except as may be otherwise agreed to in writing, no warranties of any      *
 * kind, whether express or implied, are given by MTK with respect to any MTK *
 * Deliverables or any use thereof, and MTK Deliverables are provided on an   *
 * "AS IS" basis.  MTK hereby expressly disclaims all such warranties,        *
 * including any implied warranties of merchantability, non-infringement and  *
 * fitness for a particular purpose and any warranties arising out of course  *
 * of performance, course of dealing or usage of trade.  Parties further      *
 * acknowledge that Company may, either presently and/or in the future,       *
 * instruct MTK to assist it in the development and the implementation, in    *
 * accordance with Company's designs, of certain softwares relating to        *
 * Company's product(s) (the "Services").  Except as may be otherwise agreed  *
 * to in writing, no warranties of any kind, whether express or implied, are  *
 * given by MTK with respect to the Services provided, and the Services are   *
 * provided on an "AS IS" basis.  Company further acknowledges that the       *
 * Services may contain errors, that testing is important and Company is      *
 * solely responsible for fully testing the Services and/or derivatives       *
 * thereof before they are used, sublicensed or distributed.  Should there be *
 * any third party action brought against MTK, arising out of or relating to  *
 * the Services, Company agree to fully indemnify and hold MTK harmless.      *
 * If the parties mutually agree to enter into or continue a business         *
 * relationship or other arrangement, the terms and conditions set forth      *
 * hereunder shall remain effective and, unless explicitly stated otherwise,  *
 * shall prevail in the event of a conflict in the terms in any agreements    *
 * entered into between the parties.                                          *
 *---------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * Copyright (c) 2005, MediaTek, Inc.
 * All rights reserved.
 *
 * Unauthorized use, practice, perform, copy, distribution, reproduction,
 * or disclosure of this information in whole or in part is prohibited.
 *-----------------------------------------------------------------------------
 *
 * $Author: dtvbm11 $
 * $Date: 2015/01/09 $
 * $RCSfile: drv_ycproc.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/


#ifndef _DRV_LDMSPI_H_
#define _DRV_LDMSPI_H_

#include "x_typedef.h"
#include "x_os.h"
#include "x_assert.h"
#include "x_printf.h"
#include "x_hal_io.h"
#include "panel.h"
#include "x_ckgen.h"

#ifdef CC_MT5399
#define LDM_BUS_CLOCK 216
#else
#define LDM_BUS_CLOCK 144
#endif

#define MAX_SPI_CMD_SIZE 514
#define HSYNC_SRC SrcPWM2

typedef enum
{
    E_SPI_PAGE_WRITE,
    E_SPI_READ_FAULT,
    E_SPI_LIGHT_UP,
    E_SPI_LOG_OFF,
    E_SPI_FAULT_REPAIR,
    E_SPI_SET_SCANNING,
    E_SPI_SET_HSYNC_FREQ
} ENUM_SPI_COMMAND_SET;

typedef enum
{
    E_SPI_NORMAL_STATUS=0,
    E_SPI_OPEN_STATUS=1,
    E_SPI_SHORT_STATUS=2,
    E_SPI_UNCERTAIN_SHORT_STATUS=3,
    E_SPI_SHORT_TOLERANCE=180   
} ENUM_SPI_FAULT_STATUS_SET;

typedef enum
{
    E_SPI_LGD_PROTOCOL,
    E_SPI_AMS_PROTOCOL,        
    E_SPI_AMS_382X_PROTOCOL,
    E_SPI_AATI_PROTOCOL,
    E_SPI_RT_8302_PROTOCOL,
    E_SPI_FXC_PROTOCOL,
    E_SPI_LEPOWER_PROTOCOL,
    E_SPI_NONE_PROTOCOL
} ENUM_SPI_PROTOCOL_SET;

/* Named size of panel */
typedef enum
{
	INCH_15=0,
    INCH_19,
    INCH_22,
    INCH_26,
    INCH_32,
    INCH_37,
    INCH_42,
    INCH_46,
    INCH_47,
    INCH_50,
    INCH_52,
    INCH_55,
    INCH_65,
    INCH_60,
    INCH_70,
    INCH_72,
    INCH_BASE
} INCH_TYPE_T;

/* Backlight type */
typedef enum
{
	BL_CCFL = 0,
	BL_NOR_LED,
	BL_EDGE_LED,
	BL_IOP_LED,
	BL_ALEF_LED,
	BL_IOL_LED,
	BL_ALEF_LGD,
	BL_ALEF_FPR,
}BACKLIGHT_TYPE_T;

extern UINT8 u1DriverOrder[200]; 
extern UINT8 u1BlockDimGain[256]; 

BOOL bDrvLcdimSpiInit(void);
BOOL bDrvLcdimSpiSWInit(void);
BOOL bDrvLcdimSpiHWInit(void);

void vDrvLcdimSpiResume(void);
void vDrvLcdimSpiSuspend(void);
void vDrvLcdimSpiCommandGen(UINT8 u1CommandSet, UINT32 u4CommandParam);
void vDrvLcdimSpiGetSystemInfo(void);
void vDrvLcdimSpiSetLoopAllOnOff(UINT8 u1OnOff);
void vDrvLcdimSpiSetGlobalDimming(UINT16 u2Duty, UINT8 u1Freq);
void vDrvLcdimSpi1BWrite(UINT8 u1Addr, UINT8 u1Data);
void vDrvLcdimSpi1BRead(UINT8 u1Addr);
void vDrvLcdimSpiSendDataSWMode(UINT8 *u1DataArray, UINT16 u2Datalen);
void vDrvLcdimSpiSendDataSWModeMbyN(UINT8 *u1DataArray, UINT16 u2MDatalen, UINT16 u2NDatalen);
void vDrvLcdimSpiSendDataHWMode(UINT8 *u1DataArray, UINT16 u2Datalen);
void vDrvLcdimSpiSendDataHWModeMbyN(UINT8 *u1DataArray, UINT16 u2MDatalen, UINT16 u2NDatalen);
void vDrvLcdimSpiSetDoubleVsyncFreq(UINT32 u4Double);
UINT8 u1DrvLcdimSpiRWProtect(void);
UINT8 vDrvLcdimSpiGetGlobalDimming(void);
UINT16 u2DrvLcdimSpiGetBlockNum(void);
UINT32 vDrvLcdimSpiWaitReady(UINT32 u4WaitCount);
void vDrvLcdimSpiSetHeader(UINT8 u1Mask, UINT8 u1OnOff);

//-----------------------------------------------------------
//   Function Prototype For Command Set
//-----------------------------------------------------------
void SPI_NON_DEF_CMD_0_PARAM(void);
void SPI_NON_DEF_CMD_1_PARAM(UINT8 u1Param1);
void SPI_NON_DEF_CMD_2_PARAM(UINT8 u1Param1, UINT8 u1Param2);

typedef struct sLdmSpiFuncSet {    
	void (*vSpiBatchReadCmd)(void);
	void (*vSpiBatchWriteCmd)(void);
	void (*vSpiLightUpCmd)(void);
	void (*vSpiLogOffCmd)(void);
	void (*vSpiRepairCmd)(void);	
	void (*vSpiRead1BCmd)(UINT8);
	void (*vSpiWrite1BCmd)(UINT8, UINT8);	
    void (*vSpiSetScanningCmd)(UINT8);	
} sLdmSpiCommandSet;

UINT16 u2DrvGetTotalBadBlocks(void);

//-----------------------------------------------------------
//                Driver For AMS Driver IC
//-----------------------------------------------------------
void vDrvAmsBatchReadCmd(void);
void vDrvAmsBatchWriteCmd(void);
void vDrvAmsLightUpCmd(void);
void vDrvAmsLogOffCmd(void);
void vDrvAmsRepairCmd(void);
void vDrvAmsRead1BCmd(UINT8 u1Addr);
void vDrvAmsWrite1BCmd(UINT8 u1Addr, UINT8 u1Data);
void vDrvAmsSetScanningCmd(UINT8 u1ScanningIdx);
void vDrvAmsSetFaultState(UINT8 u1BlkIdx, UINT8 u1FaultStatus);
UINT8 u1DrvAmsGetFaultState(void);
void vDrvAmsSetScanning(UINT8 u1ScanningIdx);

//-----------------------------------------------------------
//                Driver For AMS 382X Driver IC
//-----------------------------------------------------------
void vDrvAms382XBatchReadCmd(void);
void vDrvAms382XBatchWriteCmd(void);
void vDrvAms382XLightUpCmd(void);
void vDrvAms382XLogOffCmd(void);
void vDrvAms382XRepairCmd(void);
void vDrvAms382XRead1BCmd(UINT8 u1Addr);
void vDrvAms382XWrite1BCmd(UINT8 u1Addr, UINT8 u1Data);
void vDrvAms382XSetScanningCmd(UINT8 u1ScanningIdx);
void vDrvAm382XSetFaultState(UINT8 u1BlkIdx, UINT8 u1FaultStatus);
UINT8 u1DrvAms382XGetFaultState(void);
void vDrvAms382XSetScanning(UINT8 u1ScanningIdx);

//-----------------------------------------------------------
//                Driver For RT8302 Driver IC
//-----------------------------------------------------------
void vDrvRT8302BatchReadCmd(void);
void vDrvRT8302BatchWriteCmd(void);
void vDrvRT8302LightUpCmd(void);
void vDrvRT8302LogOffCmd(void);
void vDrvRT8302RepairCmd(void);
void vDrvRT8302Read1BCmd(UINT8 u1Addr);
void vDrvRT8302Write1BCmd(UINT8 u1Addr, UINT8 u1Data);
void vDrvRT8302SetScanningCmd(UINT8 u1ScanningIdx);
void vDrvRT8302SetFaultState(UINT8 u1BlkIdx, UINT8 u1FaultStatus);
UINT8 u1DrvRT8302GetFaultState(void);
void vDrvRT8302SetScanning(UINT8 u1ScanningIdx);

//-----------------------------------------------------------
//                Driver For AATI Driver IC
//-----------------------------------------------------------
void vDrvAatiBatchReadCmd(void);
void vDrvAatiBatchWriteCmd(void);
void vDrvAatiLightUpCmd(void);
void vDrvAatiLogOffCmd(void);
void vDrvAatiRepairCmd(void);
void vDrvAatiRead1BCmd(UINT8 u1Addr);
void vDrvAatiWrite1BCmd(UINT8 u1Addr, UINT8 u1Data);
void vDrvAatiSetScanning(void);

//-----------------------------------------------------------
//                Driver For LGD Driver IC
//-----------------------------------------------------------
void vDrvLgdBatchWriteCmd(void);
void vDrvLgdLightUpCmd(void);

//-----------------------------------------------------------
//                Driver For FXC Driver IC
//-----------------------------------------------------------
void vDrvFxcBatchWriteCmd(void);
void vDrvFxcLightUpCmd(void);
void vDrvFxcSetScanningCmd(UINT8 u1Index);

//-----------------------------------------------------------
//                Driver For LEPOWER Driver IC
//-----------------------------------------------------------
void vDrvLePowerWrite1BCmd(UINT8 u1Addr, UINT8 u1Data);
void vDrvLePowerBatchWriteCmd(void);
void vDrvLePowerLightUpCmd(void);
void vDrvLePowerLogOffCmd(void);
void vDrvLePowerSetDelayOrder(UINT8 *u1CustDelayOrder);
void vDrvLePowerSetScanning(UINT8 u1ScanningIdx);


//-----------------------------------------------------------
//                Driver For Ambilight Driver IC
//-----------------------------------------------------------
void vDrvAmbiLitBatchWriteCmd(UINT16 u2CmdLength, UINT8 *u1AmbilCmd, UINT8 u1ICType);
void vDrvAmbiLitLightUpCmd(UINT8 u1ICType);

void vDrvSPIBatchWriteCmd(UINT16 u2CmdLength, UINT8 *u1AmbilCmd, UINT8 u1ICType);
void vDrvSPILightUpCmd(UINT8 u1ICType);
UINT8 DRVCUST_SetBLHeaderFooter(UINT32 u4CmdHeader, UINT32 u4CmdFooter);

#endif //_DRV_LDMSPI_H_
