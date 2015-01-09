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
 * $RCSfile: nandhw_reg.c,v $
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

UINT16 u2LePowerDriverOrder[256];

UINT8 u1LePowerDelayOrder[16] = {0, 0, 1, 2, 2, 3, 4, 4, 4, 4, 3, 2, 2, 1, 0, 0};

extern UINT8 u1SpiCmdArray[MAX_SPI_CMD_SIZE];
extern UINT8 u1ReadFlg;

extern PANEL_LOCAL_DIMMING_ATTRIBUTE_T *pParam;

//====================================================================================
//                        Function Prototype For Command Set
//====================================================================================
sLdmSpiCommandSet sLePowerSpiFunc = 
{
	SPI_NON_DEF_CMD_0_PARAM,            //  void (*vSpiBatchReadCmd)(void);
	vDrvLePowerBatchWriteCmd,           //  void (*vSpiBatchWriteCmd)(void);
    vDrvLePowerLightUpCmd,              //  void (*vSpiLightUpCmd)(void);
	vDrvLePowerLogOffCmd,               //	void (*vSpiLogOffCmd)(void);
	SPI_NON_DEF_CMD_0_PARAM,	        //	void (*vSpiRepairCmd)(void);	
	SPI_NON_DEF_CMD_1_PARAM,	        //	void (*vSpiRead1BCmd)(UINT8);
	vDrvLePowerWrite1BCmd,              //	void (*vSpiWrite1BCmd)(UINT8, UINT8);	
	vDrvLePowerSetScanning
};

//====================================================================================
//                     AMS Spec Implementation (General Behavior)
//====================================================================================
void vDrvLePowerWrite1BCmd(UINT8 u1Addr, UINT8 u1Data)
{
    // 0. set header (device id and register address)
    u1SpiCmdArray[0] = (UINT8)((LDM_LP1087_ADDR1 << 1) | LDM_LP1087_WRITE_IDX);;
    u1SpiCmdArray[1] = u1Addr;
    u1SpiCmdArray[2] = u1Data;

    vDrvLcdimSpiSendDataSWMode(u1SpiCmdArray, 3);   
}

void vDrvLePowerBatchWriteCmd(void)
{
#ifndef CC_MTK_LOADER      
    UINT16 u2DataLen = 2 + u2DrvLcdimSpiGetBlockNum()*2;
    UINT16 i;    
    UINT8 u1DimGain = vDrvLcdimSpiGetGlobalDimming();
    UINT16 u2TempBuf;    
    static UINT8 u1Buffer[256];

    vDrvGetLcDimDimmingResult(u1Buffer);

    // 0. set header (device id and register address)
    u1SpiCmdArray[0] = (UINT8)((LDM_LP1087_ADDR1 << 1) | LDM_LP1087_WRITE_IDX);;
    u1SpiCmdArray[1] = LDM_LP1087_PWM_HTIME1;
    
    // 1. set data body (16 bit per block, but each block use 9 bit to represent data actually)
    for(i=0; i < u2DrvLcdimSpiGetBlockNum(); i++)
    {    
        u2TempBuf = (UINT16)(((UINT32)((UINT32)u1Buffer[u2LePowerDriverOrder[i]]*(UINT32)u1DimGain)>>8) << 1);
        u1SpiCmdArray[2+i*2] = (((u2IO32Read2B(ADAP_LCDIM_SPI_SEP) >> u2LePowerDriverOrder[i]) & 0x1) == 1) ? 0xFF : (UINT8)(u2TempBuf & 0xff);      
        u1SpiCmdArray[2+i*2+1] = (((u2IO32Read2B(ADAP_LCDIM_SPI_SEP) >> u2LePowerDriverOrder[i]) & 0x1) == 1) ? 1 : (UINT8)((u2TempBuf>>8) & 0xff);
    }        
    vDrvLcdimSpiSendDataHWMode(u1SpiCmdArray, u2DataLen);        
#endif      
}

void vDrvLePowerLightUpCmd(void)
{
    //-------------------------------------
    // Customization Part For Different Panel Design
    //-------------------------------------
    UINT16 u2DataLen = 2 + u2DrvLcdimSpiGetBlockNum()*2;
    UINT16 i;

    vDrvLcdimSpiSetLoopAllOnOff(SV_OFF);    
    
    vIO32WriteFldAlign(LED_CON, 1, OP_MODE);  
    //-------------------------------------
    // 0. set basic config
    //-------------------------------------    
    u1SpiCmdArray[0] = (UINT8)((LDM_LP1087_ADDR1 << 1) | LDM_LP1087_WRITE_IDX);;
    u1SpiCmdArray[1] = LDM_LP1087_BASIC_CFG;
    u1SpiCmdArray[2] = 0x0;
    
    vDrvLcdimSpiSendDataSWMode(u1SpiCmdArray, 3);       

    //-------------------------------------
    // 1. set period = 0x200
    //-------------------------------------    
    u1SpiCmdArray[0] = (UINT8)((LDM_LP1087_ADDR1 << 1) | LDM_LP1087_WRITE_IDX);;
    u1SpiCmdArray[1] = LDM_LP1087_PWM_PERIOD_LSB;
    u1SpiCmdArray[2] = 0x00;
    u1SpiCmdArray[3] = 0x02;    
    
    vDrvLcdimSpiSendDataSWMode(u1SpiCmdArray, 4);       
    
    //-------------------------------------
    // 2. set all dimming to full
    //-------------------------------------    

    // 2.0. set header (device id and register address)
    u1SpiCmdArray[0] = (UINT8)((LDM_LP1087_ADDR1 << 1) | LDM_LP1087_WRITE_IDX);;
    u1SpiCmdArray[1] = LDM_LP1087_PWM_HTIME1;

    // 2.1. set data body (16 bit per block)
    for(i=0; i < u2DrvLcdimSpiGetBlockNum(); i++)
    {
            // set default order here
            u2LePowerDriverOrder[i] = i;    

            Printf("u2LePowerDriverOrder[%d] = %d", u2LePowerDriverOrder[i], i);

            u1SpiCmdArray[2+i*2] = 0xFF;
            u1SpiCmdArray[2+i*2+1] = 1;

    }        

    vDrvLcdimSpiSendDataSWMode(u1SpiCmdArray, u2DataLen);   

    vDrvLcdimSpiSetLoopAllOnOff(SV_ON);
}

void vDrvLePowerLogOffCmd(void)
{
    UINT16 u2DataLen = 2 + u2DrvLcdimSpiGetBlockNum()*2;
    UINT16 i;

    if(u2DataLen > MAX_SPI_CMD_SIZE)
    {
        u2DataLen = MAX_SPI_CMD_SIZE;    // fix klockwork
    }

    vDrvLcdimSpiSetLoopAllOnOff(SV_OFF);    

    vIO32WriteFldAlign(LED_CON, 1, OP_MODE);  
    
    // 1. set all block dimming to 0
    // 1.0. set header (device id and register address)
    u1SpiCmdArray[0] = (UINT8)((LDM_LP1087_ADDR1 << 1) | LDM_LP1087_WRITE_IDX);;
    u1SpiCmdArray[1] = LDM_LP1087_PWM_HTIME1;

    // 1.1. set data body (16 bit per block)
    for(i=1; i < u2DrvLcdimSpiGetBlockNum(); i++)
    {
        u1SpiCmdArray[2+i*2] = 0;
        u1SpiCmdArray[2+i*2+1] = 0;
    }        

    vDrvLcdimSpiSendDataSWMode(u1SpiCmdArray, u2DataLen); 

    LOG(0, "SPI Init [LogOff]\n");
}

//====================================================================================
//                 AMS Spec Implementation (Customized Behavior)
//====================================================================================
// For Delay Order Customization
void vDrvLePowerSetDelayOrder(UINT8 *u1CustDelayOrder)
{
    UINT8 i;
    
    for(i = 0; i < 16; i++)
    {
        u1LePowerDelayOrder[i] = u1CustDelayOrder[i];
    }    
}

// Customization Part For Customer Spec (5 Dimming Area)
void vDrvLePowerSetScanning(UINT8 u1ScanningIdx)
{

}

