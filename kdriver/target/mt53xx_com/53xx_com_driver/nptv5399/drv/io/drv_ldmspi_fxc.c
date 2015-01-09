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

extern UINT8 u1SpiCmdArray[MAX_SPI_CMD_SIZE];
extern UINT8 u1DriverOrder[200];
extern UINT8 u1BlockDimGain[256];
extern UINT8 u1PanelDelay[200];
extern UINT16 u2DimmingLowBound;
extern PANEL_LOCAL_DIMMING_ATTRIBUTE_T *pParam;

#ifndef CC_MTK_LOADER      
static UINT32 u4LogCnt = 0;
#endif

#define FXC_PROTOCOL_HDR                0x1E
#define FXC_PROTOCOL_ITEM_UPDATE        0x23
#define FXC_PROTOCOL_ITEM_SCAN          0x01

#define CLIP(val, min, max) ((val>=max) ? max : ((val<=min) ? min : val)) 

#define EN_FXC_LD_DBG

#ifdef EN_FXC_LD_DBG
#define FXC_DBG_REG_4C                  u4SWReg(0x004C)
#define FXC_DBG_LD_CMD_LOG_EN           Fld(1, 28, AC_MSKB3)    // 28

UINT16  u2CmdLogCntr = 0;
#endif

#define EN_FXC_LD_TEST_X

#ifdef EN_FXC_LD_TEST_X
#define FXC_TST_REG_4C_X                u4SWReg(0x004C)
    #define FXC_TST_LD_BYPASS_X             Fld(1,  31, AC_MSKB3)   // 31
    #define FXC_TST_SINGLE_EN_X             Fld(1,  30, AC_MSKB3)   // 30
    #define FXC_TST_FORCE_LV_EN_X           Fld(1,  29, AC_MSKB3)   // 29
    #define FXC_TST_FORCE_LV_X              Fld(8,  16, AC_FULLB2)  // 23:16
    #define FXC_TST_SINGLE_CH_X             Fld(8,   8, AC_FULLB1)  // 15:8
    #define FXC_TST_GBL_DUTY_X              Fld(8,   0, AC_FULLB0)  //  7:0
#endif

//====================================================================================
//                      Function Prototype For Command Set
//====================================================================================
sLdmSpiCommandSet sFxcSpiFunc = 
{
	SPI_NON_DEF_CMD_0_PARAM,        //  void (*vSpiBatchReadCmd)(void);
	vDrvFxcBatchWriteCmd,           //  void (*vSpiBatchWriteCmd)(void);
    vDrvFxcLightUpCmd,              //  void (*vSpiLightUpCmd)(void);
	SPI_NON_DEF_CMD_0_PARAM,        //	void (*vSpiLogOffCmd)(void);
	SPI_NON_DEF_CMD_0_PARAM,	    //	void (*vSpiRepairCmd)(void);	
	SPI_NON_DEF_CMD_1_PARAM,	    //	void (*vSpiRead1BCmd)(UINT8);
	SPI_NON_DEF_CMD_2_PARAM,        //	void (*vSpiWrite1BCmd)(UINT8, UINT8);	
	vDrvFxcSetScanningCmd           //   void (*vSpiSetScanningCmd)(UINT8);		
};

//====================================================================================
//                           FXC SPI Command Interface
//====================================================================================
void vDrvFxcBatchWriteCmd(void)
{    
#ifndef CC_MTK_LOADER      
    static UINT8 u1Dimming[LCDIM_MAX_BLK_NUM];
    UINT16 u2DataLen;
    UINT32 u4Index, u4Sum, u4BlockNum, u4LowBright, u4GlobalDimming, u4Temp;
	
  #ifdef EN_FXC_LD_TEST_X
    UINT8  bBypassLD = IO32ReadFldAlign(FXC_TST_REG_4C_X, FXC_TST_LD_BYPASS_X) ? 1 : 0;
    UINT8  bSingleCH = IO32ReadFldAlign(FXC_TST_REG_4C_X, FXC_TST_SINGLE_EN_X) ? 1 : 0;
    UINT8  bForceLV  = IO32ReadFldAlign(FXC_TST_REG_4C_X, FXC_TST_FORCE_LV_EN_X) ? 1 : 0;
    UINT8  u1ForceLV = (UINT8)IO32ReadFldAlign(FXC_TST_REG_4C_X, FXC_TST_FORCE_LV_X);
    UINT8  u1TstCh   = (UINT8)IO32ReadFldAlign(FXC_TST_REG_4C_X, FXC_TST_SINGLE_CH_X);
  #endif

    u4BlockNum =  u2DrvLcdimSpiGetBlockNum();
    u2DataLen = MIN(3 + u2DrvLcdimSpiGetBlockNum() + 1, MAX_SPI_CMD_SIZE);  // header (3) + body (n) + footer (1)
    u4LowBright = IsPwmLowPanelBright();
    u4GlobalDimming = vDrvLcdimSpiGetGlobalDimming();

    vIO32WriteFldAlign(LED_CON, 1, OP_MODE);

    // 1. set header
    u1SpiCmdArray[0] = FXC_PROTOCOL_HDR;
    u1SpiCmdArray[1] = FXC_PROTOCOL_ITEM_UPDATE;
    u1SpiCmdArray[2] = u4BlockNum;

    // 2. set data body
    vDrvGetLcDimDimmingResult(u1Dimming); //move local dimming data to buffer
    for (u4Index = 0; u4Index < u4BlockNum; u4Index++)
    {
        u4Index = MIN(u4Index, LCDIM_MAX_BLK_NUM-1);

        u4Temp = (((u2IO32Read2B(ADAP_LCDIM_SPI_SEP) >> u1DriverOrder[u4Index]) & 0x1) == 1) ? 0 :
                                CLIP((u1Dimming[u4Index] * u4GlobalDimming *(UINT32)u1BlockDimGain[u4Index] + 32767)>>15, pParam->u2DimmingLowBound, 0xFF);

#if (1)		
        if (1)
        {
    #ifdef EN_FXC_LD_TEST_X
            if (bSingleCH)
            {
                if ((UINT8)(u4Index+1) == u1TstCh)
                    u1SpiCmdArray[u4Index+3] = 0;
                else
                    u1SpiCmdArray[u4Index+3] = 0xFF;
            }
			else if (bForceLV)
			{
			    u1SpiCmdArray[u4Index+3] = 0xFF - u1ForceLV;
			}
            else if (bBypassLD)
            {
                u1SpiCmdArray[u4Index+3] = 0;
            }
            else 
                //u1SpiCmdArray[u4Index] = 0xFF - (UINT8)(IO32ReadFldAlign(FXC_TST_REG_4C_X, FXC_TST_GBL_DUTY_X));
    #endif
            {
                u1SpiCmdArray[u1DriverOrder[u4Index]+3] = 0xFF - u4Temp;
            }
        }
#else
        if (u4LowBright)
        {
            u1SpiCmdArray[u1DriverOrder[u4Index]+3] = 0xFF - u4Temp;
        }
#endif		
    }

    // 3. set footer
    u4Sum = 0;
    for(u4Index = 0; u4Index < u2DataLen-1; u4Index++)
    {
        u4Sum += u1SpiCmdArray[u4Index];
    }
    u1SpiCmdArray[u2DataLen-1] = 0xFF - (u4Sum & 0xFF);

    // 4. send command    
    vDrvLcdimSpiSendDataHWModeMbyN(u1SpiCmdArray, u2DataLen/5, 5);

    // Command Log
    if (IO32ReadFldAlign(ADAP_LCDIM_SPI, SPI_DIM_LOG) && (u4LogCnt++%90==0))
    {
        for(u4Index = 0; u4Index < u2DataLen; u4Index++)
        {
            Printf("0x%2x ", u1SpiCmdArray[u4Index]);
        }
        Printf("\n");
    }

#ifdef EN_FXC_LD_DBG
    if ((++u2CmdLogCntr % 240) == 0)
    {
        if (IO32ReadFldAlign(FXC_DBG_REG_4C, FXC_DBG_LD_CMD_LOG_EN))
        {
            for(u4Index = 0; u4Index < u2DataLen; u4Index++)
            {
                Printf("0x%2x ", (0xFF - u1SpiCmdArray[u4Index]));
            }
            Printf("\n");
        }
    }
#endif

#endif    
}

void vDrvFxcSetScanningCmd(UINT8 u1Index)
{
#ifndef CC_MTK_LOADER
    UINT32 u4Index, u4Sum, u4BlockNum;
    UINT16 u2DataLen;

    u4BlockNum =  MIN(u2DrvLcdimSpiGetBlockNum(), LCDIM_MAX_BLK_NUM-1);
    u2DataLen = MIN(3 + u2DrvLcdimSpiGetBlockNum() + 1, MAX_SPI_CMD_SIZE);  // header (3) + body (n) + footer (1)
    vIO32WriteFldAlign(LED_CON, 1, OP_MODE);

    // 1. set header
    u1SpiCmdArray[0] = FXC_PROTOCOL_HDR;
    u1SpiCmdArray[1] = FXC_PROTOCOL_ITEM_SCAN;
    u1SpiCmdArray[2] = u4BlockNum;
    
    // 2. set data body
    for (u4Index = 3; u4Index < (3+u4BlockNum); u4Index++)
    {
        u1SpiCmdArray[u4Index] = u1PanelDelay[u4Index-3];
    }

    // 3. set footer
    u4Sum = 0;
    for(u4Index = 0; u4Index < u2DataLen-1; u4Index++)
    {
        u4Sum += u1SpiCmdArray[u4Index];
    }
    u1SpiCmdArray[u2DataLen-1] = 0xFF - (u4Sum & 0xFF);

    // 4. send command    
    vDrvLcdimSpiSendDataSWMode(u1SpiCmdArray, u2DataLen);

    // Command Log
    if (IO32ReadFldAlign(ADAP_LCDIM_SPI, SPI_DIM_LOG))
    {
            for(u4Index = 0; u4Index < u2DataLen; u4Index++)
            {
                Printf("0x%2x ", u1SpiCmdArray[u4Index]);
            }
            Printf("\n");
        }
#endif    
}

void vDrvFxcLightUpCmd(void)
{
    UINT16 u2DataLen;
    UINT32 u4Index, u4Sum, u4BlockNum, u4LowBright, u4Temp;

    u4BlockNum =  u2DrvLcdimSpiGetBlockNum();
    u2DataLen = MIN(3 + u2DrvLcdimSpiGetBlockNum() + 1, MAX_SPI_CMD_SIZE);  // header (3) + body (n) + footer (1)
    u4LowBright = IsPwmLowPanelBright();

    vIO32WriteFldAlign(LED_CON, 1, OP_MODE);

    // 1. set header
    u1SpiCmdArray[0] = FXC_PROTOCOL_HDR;
    u1SpiCmdArray[1] = FXC_PROTOCOL_ITEM_UPDATE;
    u1SpiCmdArray[2] = u4BlockNum;

    // 2. set data body
    for (u4Index = 0; u4Index < u4BlockNum; u4Index++)
    {
        u4Temp = u4LowBright ? 0x0 : 0xFF;
        u1SpiCmdArray[u4Index+3] = u4Temp;
    }

    // 3. set footer
    u4Sum = 0;
    for(u4Index = 0; u4Index < u2DataLen-1; u4Index++)
    {
        u4Sum += u1SpiCmdArray[u4Index];
    }
    u1SpiCmdArray[u2DataLen-1] = 0xFF - (u4Sum & 0xFF);

    // 4. send command    
    vDrvLcdimSpiSendDataSWModeMbyN(u1SpiCmdArray, u2DataLen/5, 5);

#ifndef CC_MTK_LOADER
    vDrvLcdimSpiSetLoopAllOnOff(SV_OFF);
    vDrvFxcSetScanningCmd(0);
    vDrvLcdimSpiSetLoopAllOnOff(SV_ON);
#endif
}


