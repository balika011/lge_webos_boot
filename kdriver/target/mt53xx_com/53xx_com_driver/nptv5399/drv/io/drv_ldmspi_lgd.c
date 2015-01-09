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

#define CLIP(val, min, max) ((val>=max) ? max : ((val<=min) ? min : val)) 

extern UINT8 u1SupportScanning;
extern UINT8 u1LcdimScanning; 
extern UINT8 u1LcdimCurrentBoost;
extern UINT8 u1LcdimH1BSelect;
extern UINT8 u1LcdimReverse;

extern UINT8 u1LcdimSpiHeader1;

extern UINT8 u1SpiCmdArray[MAX_SPI_CMD_SIZE];
extern UINT8 u1ReadFlg;
extern UINT8 u1DriverOrder[200];
extern UINT8 u1BlockDimGain[256];

extern PANEL_LOCAL_DIMMING_ATTRIBUTE_T *pParam;

#define LGD_PROTOCOL_HDR_0 0xAA
#define LGD_PROTOCOL_HDR_1 0x03

//====================================================================================
//                      Function Prototype For Command Set
//====================================================================================
sLdmSpiCommandSet sLgdSpiFunc = 
{
	SPI_NON_DEF_CMD_0_PARAM,        //  void (*vSpiBatchReadCmd)(void);
	vDrvLgdBatchWriteCmd,           //  void (*vSpiBatchWriteCmd)(void);
    vDrvLgdLightUpCmd,              //  void (*vSpiLightUpCmd)(void);
	SPI_NON_DEF_CMD_0_PARAM,        //	void (*vSpiLogOffCmd)(void);    
	SPI_NON_DEF_CMD_0_PARAM,        //	void (*vSpiRepairCmd)(void);		
	SPI_NON_DEF_CMD_1_PARAM,        //	void (*vSpiRead1BCmd)(UINT8);	
	SPI_NON_DEF_CMD_2_PARAM,         //	void (*vSpiWrite1BCmd)(UINT8, UINT8);
	SPI_NON_DEF_CMD_1_PARAM          //    void (*vSpiSetScanningCmd)(UINT8);		
};


//====================================================================================
//                           LGD SPI Command Interface
//====================================================================================
void vDrvLgdBatchWriteCmd(void)
{    
#ifndef CC_MTK_LOADER      
    UINT32 i;
    UINT16 u2DataLen = 2 + u2DrvLcdimSpiGetBlockNum() + 1;  // header (2) + body (n) + footer (1)
    UINT8 u1Buffer[LCDIM_MAX_BLK_NUM];

    if(u2DataLen > MAX_SPI_CMD_SIZE)
    {
        u2DataLen = MAX_SPI_CMD_SIZE;    // fix klockwork
    }

    vIO32WriteFldAlign(LED_CON, 1, OP_MODE);  
    
    // 1. set header
    u1SpiCmdArray[0] = LGD_PROTOCOL_HDR_0;   
    u1SpiCmdArray[1] = u1LcdimSpiHeader1;

    if(u1SpiCmdArray[1] == 0)
    u1SpiCmdArray[1] = LGD_PROTOCOL_HDR_1;          

    // 2. set data body
//    vDrvGetLcDimDimmingResult(u1SpiCmdArray+2); //move local dimming data to buffer
    if(IO32ReadFldAlign(LCDIM_DEMO_SETTING_0, LCDIM_DEMO_EN) == SV_OFF)
    {
        vDrvGetLcDimDimmingResult(u1Buffer); //move local dimming data to buffer
    }
    else
    {
        vDrvLcdimSetDemoEffect(u1Buffer);
    }

    for(i=0; i < u2DrvLcdimSpiGetBlockNum(); i++)
    {
        // Cobra local dimming V2 supports only up to 200 blocks, fix klock warning
        if(i >= 200)
        {
            i = 199;
        }                
        u1SpiCmdArray[2+u1DriverOrder[i]] = (UINT8)(CLIP((((UINT32)u1Buffer[i]*(UINT32)u1BlockDimGain[i])>>7), 0, 0xFF));
    }      
    // 3. set footer
    u1SpiCmdArray[u2DataLen-1]=u1SpiCmdArray[0]^u1SpiCmdArray[1];

    for(i=2; i<u2DataLen-1; i++)
    {
        u1SpiCmdArray[u2DataLen-1]= u1SpiCmdArray[u2DataLen-1]^u1SpiCmdArray[i];
    }
    
    vDrvLcdimSpiSendDataSWMode(u1SpiCmdArray, u2DataLen);
#endif    
}

void vDrvLgdLightUpCmd(void)
{
    UINT32 i;
    UINT16 u2DataLen = 2 + u2DrvLcdimSpiGetBlockNum() + 1;  // header (2) + body (n) + footer (1)

    if(u2DataLen > MAX_SPI_CMD_SIZE)
    {
        u2DataLen = MAX_SPI_CMD_SIZE;    // fix klockwork
    }

    // 1. set header
    u1SpiCmdArray[0] = LGD_PROTOCOL_HDR_0;   
    u1SpiCmdArray[1] = LGD_PROTOCOL_HDR_1;          

    // 2. set data body (full dimming at light up)
    for(i = 2; i < u2DataLen-1; i++)
    {

        u1SpiCmdArray[i] = 0xFF; 
    }

    // 3. set footer
    u1SpiCmdArray[u2DataLen-1]=u1SpiCmdArray[0]^u1SpiCmdArray[1];

    for(i=2; i<u2DataLen-1; i++)
    {
        u1SpiCmdArray[u2DataLen-1]= u1SpiCmdArray[u2DataLen-1]^u1SpiCmdArray[i];
    }
    
    vDrvLcdimSpiSendDataSWMode(u1SpiCmdArray, u2DataLen);

    vDrvLcdimSpiSetLoopAllOnOff(SV_ON);    
}
