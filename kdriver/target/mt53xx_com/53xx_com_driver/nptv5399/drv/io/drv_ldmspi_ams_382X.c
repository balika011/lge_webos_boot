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
 * $RCSfile: drv_ldmspi_ams_382X.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file ldm.c
 *  ldm.c provides local dimming control functions\n
 *  device.
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#include "x_typedef.h"
#include "sv_const.h"
#include "nptv_debug.h"
#include "panel.h"
#include "hw_ldmspi.h"
#include "drv_ldmspi.h"
#include "drv_lcdim_v2.h"
#include "drv_display.h"
#include "x_printf.h"
#include "hw_sw.h"
#include "drv_pwm.h"
#include "pe_if.h"

#define CLIP(val, min, max) ((val>=max) ? max : ((val<=min) ? min : val)) 

extern UINT8 u1SpiCmdArray[MAX_SPI_CMD_SIZE];
extern UINT8 u1DriverOrder[200];
extern UINT8 u1BlockDimGain[256]; 
extern UINT8 u1PanelDelay[200];
extern PANEL_LOCAL_DIMMING_ATTRIBUTE_T *pParam;

//====================================================================================
//                        Function Prototype For Command Set
//====================================================================================
sLdmSpiCommandSet sAms382XSpiFunc = 
{
	vDrvAms382XBatchReadCmd,            //  void (*vSpiBatchReadCmd)(void);
	vDrvAms382XBatchWriteCmd,           //  void (*vSpiBatchWriteCmd)(void);
    vDrvAms382XLightUpCmd,              //  void (*vSpiLightUpCmd)(void);
	vDrvAms382XLogOffCmd,               //	void (*vSpiLogOffCmd)(void);
	vDrvAms382XRepairCmd,	            //	void (*vSpiRepairCmd)(void);	
	vDrvAms382XRead1BCmd,	            //	void (*vSpiRead1BCmd)(UINT8);
	vDrvAms382XWrite1BCmd,               //	void (*vSpiWrite1BCmd)(UINT8, UINT8);	
	vDrvAms382XSetScanningCmd            //    void (*vSpiSetScanningCmd)(UINT8);		
};

//====================================================================================
//                     AMS Spec Implementation (General Behavior)
//====================================================================================

void vDrvAms382XRead1BCmd(UINT8 u1Addr)
{
    UINT32 u4ReadBack;

    vIO32WriteFldAlign(LED_CON, 2, OP_MODE);  
    vIO32WriteFldAlign(LED_NOB2, 2, RBYTE_LEN);     
    vIO32WriteFldAlign(LED_NOB2, 2, RBACK_LEN);

    u1SpiCmdArray[0] = (UINT8)(LDM_AS382X_ADDR1 | (LDM_AS382X_SINGLE_DEV<<7) | (LDM_AS382X_SINGLE_BYTE<<6));
    u1SpiCmdArray[1] = (UINT8)(u1Addr | (LDM_AS382X_READ_IDX << 7));

    vDrvLcdimSpiSendDataSWMode(u1SpiCmdArray, 2);    
    LOG(1, "[SPI] Send command: 0x%x 0x%x\n", u1SpiCmdArray[0], u1SpiCmdArray[1]);

    if(vDrvLcdimSpiWaitReady(10000000)==SV_FAIL)
    {
        Printf("[SPI] Reading Address %d Fail\n", u1Addr);
        vIO32WriteFldAlign(LED_CON, 1, OP_MODE);          
        return;
    }
    
    u4ReadBack = IO32ReadFldAlign(LED_RBUF, READ_BUF0) & 0xFF;
    Printf("[SPI] Current Reading Address 0x%x, Readback = 0x%x\n", u1Addr, u4ReadBack);

    vIO32WriteFldAlign(LED_CON, 1, OP_MODE);  
}

void vDrvAms382XWrite1BCmd(UINT8 u1Addr, UINT8 u1Data)
{
    // 0. set header (device id and register address)
    u1SpiCmdArray[0] = (UINT8)(LDM_AS382X_ADDR1 | (LDM_AS382X_SINGLE_DEV<<7) | (LDM_AS382X_SINGLE_BYTE<<6));
    u1SpiCmdArray[1] = (UINT8)(u1Addr | (LDM_AS382X_WRITE_IDX << 7));
    u1SpiCmdArray[2] = u1Data;

    LOG(1, "[SPI] Send command: 0x%x 0x%x 0x%x\n", u1SpiCmdArray[0], u1SpiCmdArray[1], u1SpiCmdArray[2]);

    vDrvLcdimSpiSendDataSWMode(u1SpiCmdArray, 3);   
}

BOOL _bDrvAms382XReadCmd(void)
{
    return SV_SUCCESS;            
}

void vDrvAms382XBatchReadCmd(void)
{
 
}

void vDrvAms382XBatchWriteCmd(void)
{
#ifndef CC_MTK_LOADER
    UINT16 u2DataLen = 3 + u2DrvLcdimSpiGetBlockNum()*2;
    UINT16 i;    
    UINT8 u1DimGain = vDrvLcdimSpiGetGlobalDimming();
    UINT16 u2TempBuf;    
    static UINT8 u1Buffer[LCDIM_MAX_BLK_NUM];
    static UINT32 u4DimCnt = 0;

    vDrvGetLcDimDimmingResult(u1Buffer); //move local dimming data to buffer

    // 0. set header (device id and register address)
    u1SpiCmdArray[0] = (UINT8)(LDM_AS382X_ADDR1 | (LDM_AS382X_SINGLE_DEV<<7) | (LDM_AS382X_MULTI_BYTE<<6));
    u1SpiCmdArray[1] = 2 * u2DrvLcdimSpiGetBlockNum();
    u1SpiCmdArray[2] = (UINT8)(LDM_AS382X_PWM_HTIME1 | (LDM_AS382X_WRITE_IDX << 7));

    // 1. set data body (16 bit per block, but each block use 9 bit to represent data actually)
    for(i = 0; i < u2DrvLcdimSpiGetBlockNum(); i++)
    {
        i = MIN(i, LCDIM_MAX_BLK_NUM-1);
        u2TempBuf = (((u2IO32Read2B(ADAP_LCDIM_SPI_SEP) >> u1DriverOrder[i]) & 0x1) == 1) ? 
                            0 : (UINT8)(CLIP(((UINT32)((UINT32)u1Buffer[i]*(UINT32)u1DimGain*(UINT32)u1BlockDimGain[i]+32767)>>15), 0, 0xFF));
        u2TempBuf = (u2TempBuf == 255) ? 1023 : CLIP(u2TempBuf*4, 0, 1023);		

        u1SpiCmdArray[3+u1DriverOrder[i]*2] = u2TempBuf & 0xFF;
        u1SpiCmdArray[3+u1DriverOrder[i]*2+1] = (u2TempBuf >> 8) & 0xFF;
    }

    vDrvLcdimSpiSendDataHWMode(u1SpiCmdArray, u2DataLen);   

    if (IO32ReadFldAlign(ADAP_LCDIM_SPI, SPI_DIM_LOG))
    {
        if ((u4DimCnt++) % 240 == 0)
        {
            for(i = 0; i < u2DataLen; i++)
            {
                Printf("0x%2x ", u1SpiCmdArray[i]);
            }
            Printf("\n");
        }
    }
#endif      
}

void vDrvAms382XLightUpCmd(void)
{
    UINT16 u2DataLen, u2RegulatorTurnOff, i;
    UINT16 u2BlockNum = MIN(u2DrvLcdimSpiGetBlockNum(), LCDIM_MAX_BLK_NUM-1);

    vDrvLcdimSpiSetLoopAllOnOff(SV_OFF);    
    vIO32WriteFldAlign(LED_CON, 1, OP_MODE);  

    // 1. Unlock AM382X
    vDrvAms382XWrite1BCmd(LDM_AS382X_UNLOCK, LDM_AS382X_UNLOCK_CMD);

    // 2. Set PWM control mode
    vDrvAms382XWrite1BCmd(LDM_AS382X_PWM_MODE, 0x0);    // bit 0: 0=internal osc, bit 1: 0=register updated with CS

    // 3. Set PWM Period
    u1SpiCmdArray[0] = (UINT8)(LDM_AS382X_ADDR1 | (LDM_AS382X_SINGLE_DEV<<7) | (LDM_AS382X_MULTI_BYTE<<6));
    u1SpiCmdArray[1] = 0x2;     // 2 byte command: LSB + MSB
    u1SpiCmdArray[2] = (UINT8)(LDM_AS382X_PWM_PERIOD_LSB | (LDM_AS382X_WRITE_IDX << 7));
    u1SpiCmdArray[3] = 0xFF;    // PWM period = 1023, hsync freq=120x1023=122820Hz
    u1SpiCmdArray[4] = 0x3;
    vDrvLcdimSpiSendDataSWMode(u1SpiCmdArray, 5);   

    // 4. Set Scanning Timing
    vDrvAms382XSetScanning(0);

    // 5. set all dimming to full
    u1SpiCmdArray[0] = (UINT8)(LDM_AS382X_ADDR1 | (LDM_AS382X_SINGLE_DEV<<7) | (LDM_AS382X_MULTI_BYTE<<6));
    u1SpiCmdArray[1] = 2 * u2BlockNum;
    u1SpiCmdArray[2] = (UINT8)(LDM_AS382X_PWM_HTIME1 | (LDM_AS382X_WRITE_IDX << 7));

    for(i=0; i < u2BlockNum; i++)
    {
        u1SpiCmdArray[3+i*2] = 0xFF;
        u1SpiCmdArray[3+i*2+1] = 0x3;
    }        
    u2DataLen = 3 + u2BlockNum * 2;
    vDrvLcdimSpiSendDataSWMode(u1SpiCmdArray, u2DataLen);   

    // 6. turn on all current regulator
    u1SpiCmdArray[0] = (UINT8)(LDM_AS382X_ADDR1 | (LDM_AS382X_SINGLE_DEV<<7) | (LDM_AS382X_MULTI_BYTE<<6));
    u1SpiCmdArray[1] = 2;   // 2 byte command: Channe 1~8 + Channe 9~16
    u1SpiCmdArray[2] = (UINT8)(LDM_AS382X_CUR_REGULATOR | (LDM_AS382X_WRITE_IDX << 7));

    u2RegulatorTurnOff = 0;
    for(i = 0; i < u2BlockNum; i++)
    {
        u2RegulatorTurnOff |= (1 << i);
    }

    u1SpiCmdArray[3] = u2RegulatorTurnOff & 0xFF;
    u1SpiCmdArray[4] = (u2RegulatorTurnOff >> 8) & 0xFF;
    vDrvLcdimSpiSendDataSWMode(u1SpiCmdArray, 5);

    // 7. Set PWM control mode
    vDrvAms382XWrite1BCmd(LDM_AS382X_PWM_MODE, 0x3);    // bit 0: 1=use HSYNC as clk, bit 1: 0=register updated with vsync

    // 9. Set Feedback function
    u1SpiCmdArray[0] = (UINT8)(LDM_AS382X_ADDR1 | (LDM_AS382X_SINGLE_DEV<<7) | (LDM_AS382X_MULTI_BYTE<<6));
    u1SpiCmdArray[1] = 2;   // 2 byte command: Channe 1~8 + Channe 9~16
    u1SpiCmdArray[2] = (UINT8)(LDM_AS382X_FB_CUR | (LDM_AS382X_WRITE_IDX << 7));

    u2RegulatorTurnOff = 0;
    for(i = 0; i < u2BlockNum; i++)
    {
        u2RegulatorTurnOff |= (1 << i);
    }

    u1SpiCmdArray[3] = u2RegulatorTurnOff & 0xFF;
    u1SpiCmdArray[4] = (u2RegulatorTurnOff >> 8) & 0xFF;
    vDrvLcdimSpiSendDataSWMode(u1SpiCmdArray, 5);

    // 10. Set open/short protection function
    vDrvAms382XWrite1BCmd(LDM_AS382X_PROTECT, 0x87);    // bit 0: 1=auto off at open, bit 1: 1=enable open detection

    // 11. Lock AM382X
    vDrvAms382XWrite1BCmd(LDM_AS382X_UNLOCK, LDM_AS382X_LOCK_CMD);

    vDrvSetPWM(HSYNC_SRC, (BSP_GetDomainClock(SRC_BUS_CLK)/255) / 122880, 0x80, 0xFF); 
    vDrvLcdimSpiSetLoopAllOnOff(SV_ON);
    vIO32WriteFldAlign(LED_CON, SV_ON, HW_TRIG_EN);  
}

void vDrvAms382XLogOffCmd(void)
{
    UINT16 u2BlockNum = u2DrvLcdimSpiGetBlockNum();
    UINT16 u2DataLen = 3 + u2BlockNum*2;
    UINT16 i;

    if(u2DataLen > MAX_SPI_CMD_SIZE)
    {
        u2DataLen = MAX_SPI_CMD_SIZE;    // fix klockwork
    }

    vDrvLcdimSpiSetLoopAllOnOff(SV_OFF);    
    vIO32WriteFldAlign(LED_CON, 1, OP_MODE);  

    // 0. Unlock AM382X
    vDrvAms382XWrite1BCmd(LDM_AS382X_UNLOCK, LDM_AS382X_UNLOCK_CMD);
    
    // 1. set all block dimming to 0
    // 1.0. set header (device id and register address)
    u1SpiCmdArray[0] = (UINT8)(LDM_AS382X_ADDR1 | (LDM_AS382X_SINGLE_DEV<<7) | (LDM_AS382X_MULTI_BYTE<<6));
    u1SpiCmdArray[1] = 2 * u2BlockNum;
    u1SpiCmdArray[2] = (UINT8)(LDM_AS382X_PWM_HTIME1 | (LDM_AS382X_WRITE_IDX << 7));

    // 1.1. set data body (16 bit per block)
    for(i=1; i < u2BlockNum; i++)
    {
        u1SpiCmdArray[3+i*2] = 0;
        u1SpiCmdArray[3+i*2+1] = 0;
    }        

    vDrvLcdimSpiSendDataSWMode(u1SpiCmdArray, u2DataLen); 

    // 2. turn off current regulators    
    // 2.0. set header (device id and register address)
    u1SpiCmdArray[0] = (UINT8)(LDM_AS382X_ADDR1 | (LDM_AS382X_SINGLE_DEV<<7) | (LDM_AS382X_MULTI_BYTE<<6));
    u1SpiCmdArray[1] = 2;   // 2 byte command: Channe 1~8 + Channe 9~16
    u1SpiCmdArray[2] = (UINT8)(LDM_AS382X_CUR_REGULATOR | (LDM_AS382X_WRITE_IDX << 7));

    // 2.1. set data body
    u1SpiCmdArray[3] = 0x00;
    u1SpiCmdArray[4] = 0x00;
    vDrvLcdimSpiSendDataSWMode(u1SpiCmdArray, 5);

    // 3. Lock AM382X
    vDrvAms382XWrite1BCmd(LDM_AS382X_UNLOCK, LDM_AS382X_LOCK_CMD);

    LOG(0, "SPI LogOff\n");
}

//====================================================================================
//                 AMS Spec Implementation (Customized Behavior)
//====================================================================================

void vDrvAms382XSetFaultState(UINT8 u1BlkIdx, UINT8 u1FaultStatus)
{

}

UINT8 u1DrvAms382XGetFaultState(void)
{
    return E_SPI_NORMAL_STATUS;
}

void vDrvAms382XRepairCmd(void)
{

}

void vDrvAms382XSetScanning(UINT8 u1ScanningIdx)
{
    UINT16 u2Temp, i, u2BlockNum, u2DataLen;

    u2BlockNum = MIN(u2DrvLcdimSpiGetBlockNum(), LCDIM_MAX_BLK_NUM-1);

    if(pParam->u1ScanningOnoff)
    {
        u1SpiCmdArray[0] = (UINT8)(LDM_AS382X_ADDR1 | (LDM_AS382X_SINGLE_DEV<<7) | (LDM_AS382X_MULTI_BYTE<<6));
        u1SpiCmdArray[1] = 2 * u2BlockNum;
        u1SpiCmdArray[2] = (UINT8)(LDM_AS382X_DELAY1 | (LDM_AS382X_WRITE_IDX << 7));

#ifndef CC_MTK_LOADER
        // For maunal tuning delay time
        if (IO32ReadFldAlign(ADAP_LCDIM_SPI, SPI_MANUAL_DELAY))
        {
            u1PanelDelay[0] = IO32ReadFldAlign(SPI_SCANNING_DELAY_0, SPI_SCANNING_DELAY_00);  
            u1PanelDelay[1] = IO32ReadFldAlign(SPI_SCANNING_DELAY_0, SPI_SCANNING_DELAY_01);  
            u1PanelDelay[2] = IO32ReadFldAlign(SPI_SCANNING_DELAY_1, SPI_SCANNING_DELAY_02);  
            u1PanelDelay[3] = IO32ReadFldAlign(SPI_SCANNING_DELAY_1, SPI_SCANNING_DELAY_03);
            u1PanelDelay[4] = IO32ReadFldAlign(SPI_SCANNING_DELAY_2, SPI_SCANNING_DELAY_04);
            u1PanelDelay[5] = IO32ReadFldAlign(SPI_SCANNING_DELAY_2, SPI_SCANNING_DELAY_05);            
       }
#endif

        for(i=0; i < u2BlockNum; i++)
        {
            u2Temp = MIN(u1PanelDelay[i] * 4, 0x3FF);
            u1SpiCmdArray[3+i*2] = u2Temp & 0xFF;
            u1SpiCmdArray[3+i*2+1] = u2Temp >> 8;
        }        
        u2DataLen = 3 + 2*u2BlockNum;
        vDrvLcdimSpiSendDataSWMode(u1SpiCmdArray, u2DataLen);   
    }    
    else
    {
        LOG(0, "Not Support Scanning\n");
    }  

}

void vDrvAms382XSetScanningCmd(UINT8 u1ScanningIdx)
{
    vDrvLcdimSpiSetLoopAllOnOff(SV_OFF);
    vIO32WriteFldAlign(LED_CON, 1, OP_MODE);  
    vDrvAms382XSetScanning(u1ScanningIdx);
    vDrvLcdimSpiSetLoopAllOnOff(SV_ON);
}

