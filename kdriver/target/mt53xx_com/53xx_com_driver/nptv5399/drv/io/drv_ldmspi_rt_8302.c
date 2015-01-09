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
 * $RCSfile: drv_ldmspi_rt_8302.c,v $
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
extern UINT16 u2DimmingLowBound;
extern PANEL_LOCAL_DIMMING_ATTRIBUTE_T *pParam;

//====================================================================================
//                        Function Prototype For Command Set
//====================================================================================
sLdmSpiCommandSet sRT8302SpiFunc = 
{
	vDrvRT8302BatchReadCmd,            //  void (*vSpiBatchReadCmd)(void);
	vDrvRT8302BatchWriteCmd,           //  void (*vSpiBatchWriteCmd)(void);
    vDrvRT8302LightUpCmd,              //  void (*vSpiLightUpCmd)(void);
	vDrvRT8302LogOffCmd,               //	void (*vSpiLogOffCmd)(void);
	vDrvRT8302RepairCmd,	            //	void (*vSpiRepairCmd)(void);	
	vDrvRT8302Read1BCmd,	            //	void (*vSpiRead1BCmd)(UINT8);
	vDrvRT8302Write1BCmd,               //	void (*vSpiWrite1BCmd)(UINT8, UINT8);	
	vDrvRT8302SetScanningCmd            //    void (*vSpiSetScanningCmd)(UINT8);		
};

//====================================================================================
//               RT 8302 Spec Implementation (General Behavior)
//====================================================================================
void vDrvRT8302Read1BCmd(UINT8 u1Addr)
{
    UINT32 u4ReadBack;

    vDrvLcdimSpiSetLoopAllOnOff(SV_OFF);    

    vIO32WriteFldAlign(LED_CON, 2, OP_MODE);
    vIO32WriteFldAlign(LED_NOB2, 1, RBYTE_LEN);
    vIO32WriteFldAlign(LED_NOB2, 1, RBACK_LEN);

    u1SpiCmdArray[0] = (UINT8)((u1Addr << 1) | LDM_RT8302_READ_IDX);
    vDrvLcdimSpiSendDataSWMode(u1SpiCmdArray, 1);
    LOG(1, "[SPI] Send command: 0x%x\n", u1SpiCmdArray[0]);

    if(vDrvLcdimSpiWaitReady(10000000)==SV_FAIL)
    {
        Printf("[SPI] Reading Address %d Fail\n", u1Addr);
        vIO32WriteFldAlign(LED_CON, 1, OP_MODE);          
        return;
    }

    vIO32WriteFldAlign(LED_NOB2, 0, RBYTE_LEN);
    vDrvLcdimSpiSendDataSWMode(u1SpiCmdArray, 1);
    u4ReadBack = IO32ReadFldAlign(LED_RBUF, READ_BUF0) & 0xFF;
    Printf("[SPI] Current Reading Address 0x%x, Readback = 0x%x\n", u1Addr, u4ReadBack);

    vIO32WriteFldAlign(LED_CON, 1, OP_MODE);  

    vDrvRT8302Write1BCmd(0x3, 0x0);    // dummy command
    vDrvLcdimSpiSetLoopAllOnOff(SV_ON);
}

void vDrvRT8302Write1BCmd(UINT8 u1Addr, UINT8 u1Data)
{
    vIO32WriteFldAlign(LED_CON, 1, OP_MODE);  

    u1SpiCmdArray[0] = (UINT8)((u1Addr << 1) | LDM_RT8302_WRITE_IDX);
    u1SpiCmdArray[1] = u1Data;

    LOG(1, "[SPI] Send command: 0x%x 0x%x\n", u1SpiCmdArray[0], u1SpiCmdArray[1]);
    vDrvLcdimSpiSendDataSWMode(u1SpiCmdArray, 2);
}

BOOL _bDrvRT8302ReadCmd(void)
{
    return 0;
}

void vDrvRT8302BatchReadCmd(void)
{

}

void vDrvRT8302BatchWriteCmd(void)
{
#ifndef CC_MTK_LOADER
    UINT8 u1DimGain = vDrvLcdimSpiGetGlobalDimming();
    UINT8 u1SpiCmdArrayDelay[9];
    UINT16 i, u2TempBuf, u2TempDelay;
    static UINT8 u1Buffer[LCDIM_MAX_BLK_NUM];
    static UINT32 u4DimCnt = 0;

    vDrvGetLcDimDimmingResult(u1Buffer); 

    // 0. set header (device id and register address)
    u1SpiCmdArray[0] = (UINT8)((LDM_RT8302_PWM_HTIME1<<1) | LDM_RT8302_WRITE_IDX);
    u1SpiCmdArrayDelay[0] = (UINT8)((LDM_RT8302_DELAY1<<1) | LDM_RT8302_WRITE_IDX);

    // 1. set data body (16 bit per block, but each block use 9 bit to represent data actually)
    for(i = 0; i < u2DrvLcdimSpiGetBlockNum(); i++)
    {
        i = MIN(i, LCDIM_MAX_BLK_NUM-1);

        // Apply dimming value
        u2TempBuf =   (((u2IO32Read2B(ADAP_LCDIM_SPI_SEP) >> u1DriverOrder[i]) & 0x1) == 1) ? 
                            0 : (UINT8)(CLIP(((UINT32)((UINT32)u1Buffer[i]*(UINT32)u1DimGain*(UINT32)u1BlockDimGain[i]+32767)>>15), u2DimmingLowBound, 0xFF));
        u2TempBuf = (u2TempBuf == 255) ? 4095 : CLIP(u2TempBuf*16, 0, 4095);
        u1SpiCmdArray[1+u1DriverOrder[i]*2] = u2TempBuf & 0xFF;
        u1SpiCmdArray[1+u1DriverOrder[i]*2+1] = (u2TempBuf >> 8) & 0xFF;

        // Check & apply delay value
        u2TempDelay = MIN(u1PanelDelay[u1DriverOrder[i]] * 16, 0xFFF);
        if ((u2TempBuf + u2TempDelay) > 0xFFF)
        {
            u2TempDelay = 0xFFF - u2TempBuf;
        }
        u1SpiCmdArrayDelay[1+u1DriverOrder[i]*2] = u2TempDelay & 0xFF;
        u1SpiCmdArrayDelay[1+u1DriverOrder[i]*2+1] = u2TempDelay >> 8;
    }

    // 2. send dimming SPI command
    vDrvLcdimSpiSendDataHWMode(u1SpiCmdArrayDelay, 9);   
    vDrvLcdimSpiSendDataHWMode(u1SpiCmdArray, 9);

    if (IO32ReadFldAlign(ADAP_LCDIM_SPI, SPI_DIM_LOG) && ((u4DimCnt++) % 120 == 0))
    {
        Printf("DLY CMD: ");
        for(i = 0; i < 9; i++)
        {
            Printf("0x%2x ", u1SpiCmdArrayDelay[i]);
        }

        Printf("\nDIM CMD: ");
        for(i = 0; i < 9; i++)
        {
            Printf("0x%2x ", u1SpiCmdArray[i]);
        }
        Printf("\n");
    }
#endif      
}

void vDrvRT8302LightUpCmd(void)
{
    UINT16 u2RegulatorTurnOff, i;

    vDrvLcdimSpiSetLoopAllOnOff(SV_OFF);    
    vIO32WriteFldAlign(LED_CON, 1, OP_MODE);  

    // 1. Set PWM control mode
    vDrvRT8302Write1BCmd(LDM_RT8302_PWM_MODE, 0x10);    // sync mode

    // 2. Set feedback control mode
    vDrvRT8302Write1BCmd(LDM_RT8302_FEEDBACK_CTRL, 0x31);   // enable feedback

    // 3. Set regulation current 
    vDrvRT8302Write1BCmd(LDM_RT8302_CUR, 0x6B);   // Set current = 150mA

    // 4. Set all delay to zero
    u1SpiCmdArray[0] = (UINT8)((LDM_RT8302_DELAY1<<1) | LDM_RT8302_WRITE_IDX);

    for(i = 0; i < 4; i++)
    {
        u1SpiCmdArray[1+i*2] = 0;
        u1SpiCmdArray[1+i*2+1] = 0;
    }
    vDrvLcdimSpiSendDataSWMode(u1SpiCmdArray, 9);   

    // 5. Set all dimming to full
    u1SpiCmdArray[0] = (UINT8)((LDM_RT8302_PWM_HTIME1<<1) | LDM_RT8302_WRITE_IDX);

    for(i=0; i < 4; i++)
    {
        u1SpiCmdArray[1+i*2] = 0xFF;
        u1SpiCmdArray[1+i*2+1] = 0xF;
    }        
    vDrvLcdimSpiSendDataSWMode(u1SpiCmdArray, 9);   

    // 7. Turn on all current regulator
    u2RegulatorTurnOff = 0;
    for(i = 0; i < u2DrvLcdimSpiGetBlockNum(); i++)
    {
        u2RegulatorTurnOff |= (1 << i);
    }

    u2RegulatorTurnOff |= (LDM_RT8302_IC_VER << 4);
    vDrvRT8302Write1BCmd(LDM_RT8302_CUR_REGULATOR, u2RegulatorTurnOff & 0xFF);

    // 8. Enable SPI loop
    vDrvLcdimSpiSetLoopAllOnOff(SV_ON);
    vIO32WriteFldAlign(LED_CON, SV_ON, HW_TRIG_EN);  
}

void vDrvRT8302LogOffCmd(void)
{

}

void vDrvRT8302SetFaultState(UINT8 u1BlkIdx, UINT8 u1FaultStatus)
{
   
}

UINT8 u1DrvRT8302GetFaultState(void)
{
    return 0;
}

void vDrvRT8302RepairCmd(void)
{

}

void vDrvRT8302SetScanning(UINT8 u1ScanningIdx)
{
    UINT16 u2Temp, i;
        
    if(pParam->u1ScanningOnoff)
    {
        u1SpiCmdArray[0] = (UINT8)((LDM_RT8302_DELAY1<<1) | LDM_RT8302_WRITE_IDX);

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

        for(i=0; i < 4; i++)
        {
            u2Temp = MIN(u1PanelDelay[i] * 16, 0xFFF);
            u1SpiCmdArray[1+i*2] = u2Temp & 0xFF;
            u1SpiCmdArray[1+i*2+1] = u2Temp >> 8;
        }        

        vDrvLcdimSpiSendDataSWMode(u1SpiCmdArray, 9);   
    }    
    else
    {
        LOG(0, "Not Support Scanning\n");
    }     
}

void vDrvRT8302SetScanningCmd(UINT8 u1ScanningIdx)
{
    vDrvLcdimSpiSetLoopAllOnOff(SV_OFF);
    vIO32WriteFldAlign(LED_CON, 1, OP_MODE);  
    vDrvRT8302SetScanning(u1ScanningIdx);
    vDrvLcdimSpiSetLoopAllOnOff(SV_ON);
}

