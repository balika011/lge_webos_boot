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
#include "hw_ldmspi.h"
#include "drv_ldmspi.h"

#define AMBILIGHT_DEBUG_SW_REG 0
//====================================================================================
//                      Function Prototype For Command Set
//====================================================================================
sLdmSpiCommandSet sAmbiLitSpiFunc = 
{
	SPI_NON_DEF_CMD_0_PARAM,        //  void (*vSpiBatchReadCmd)(void);
	SPI_NON_DEF_CMD_0_PARAM,           //  void (*vSpiBatchWriteCmd)(void);
    SPI_NON_DEF_CMD_0_PARAM,              //  void (*vSpiLightUpCmd)(void);
	SPI_NON_DEF_CMD_0_PARAM,        //	void (*vSpiLogOffCmd)(void);    
	SPI_NON_DEF_CMD_0_PARAM,        //	void (*vSpiRepairCmd)(void);		
	SPI_NON_DEF_CMD_1_PARAM,        //	void (*vSpiRead1BCmd)(UINT8);	
	SPI_NON_DEF_CMD_2_PARAM,         //	void (*vSpiWrite1BCmd)(UINT8, UINT8);
	SPI_NON_DEF_CMD_1_PARAM          //    void (*vSpiSetScanningCmd)(UINT8);			
};

//====================================================================================
//                           LGD SPI Command Interface
//====================================================================================

void vDrvAmbiLitBatchWriteCmd(UINT16 u2CmdLength, UINT8 *u1AmbilCmd, UINT8 u1ICType)
{    
#ifndef CC_MTK_LOADER       
    vDrvLcdimSpiSendDataSWMode(u1AmbilCmd, u2CmdLength);     
#endif
}

void vDrvAmbiLitLightUpCmd(UINT8 u1ICType)
{    
#ifndef CC_MTK_LOADER    
    UNUSED(u1ICType);
    //-------------------------------------------------------
    // 0. set sw / hw mode
    //-------------------------------------------------------    
    vIO32WriteFldAlign(LED_CON, SV_FALSE, SRAM_HW_MODE);
    vIO32WriteFldAlign(LED_CON, SV_FALSE, HW_TRIG_EN);   

    //-------------------------------------------------------
    // 1. set clock
    //-------------------------------------------------------    
    vIO32WriteFldAlign(LED_CKCFG, 0x33, CLK_DIV);       //set clock divide value
    vIO32WriteFldAlign(LED_CKCFG, SV_FALSE, CPOL);        //set clock polarity
    vIO32WriteFldAlign(LED_CKCFG, SV_FALSE, CPHA);        //set clock phase    

    //-------------------------------------------------------
    // 2. set vsync
    //-------------------------------------------------------    
    vIO32WriteFldAlign(LED_VSACC, 0xFFF, VSYNC_HOLD_LATENCY);     //set Vsync high time
    vIO32WriteFldAlign(LED_VSACC, 0xFFF, WAIT_LATENCY);         //set wait time from Vsync low to CS low

    //-------------------------------------------------------
    // 3. set cs                                                
    //-------------------------------------------------------    
    vIO32WriteFldAlign(LED_CSACC2, 0xF, CS_HOLD_LATENCY);	    //set CS high time
    vIO32WriteFldAlign(LED_CSACC, 0xF, CS_LEAD_LATENCY);	    //set setup time from CS low to CLK edge
    vIO32WriteFldAlign(LED_CSACC, 0xF, CS_LAG_LATENCY);	    //set hold time from CLK edge to CS high
    vIO32WriteFldAlign(LED_CSACC, 2, CS_LEAD_LATENCY);	        //set setup time from CS low to CLK edge
    vIO32WriteFldAlign(LED_CSACC, 2, CS_LAG_LATENCY);	        //set hold time from CLK edge to CS high


    vIO32WriteFldAlign(LED_CSCFG, SV_FALSE, MHEAD_SINGLE_EN); 
    vIO32WriteFldAlign(LED_CSCFG, SV_FALSE, MDATA_SINGLE_EN); 
    vIO32WriteFldAlign(LED_CSCFG, SV_FALSE, MFOOT_SINGLE_EN);     
    vIO32WriteFldAlign(LED_CSCFG, SV_FALSE, NHEAD_SINGLE_EN);
    vIO32WriteFldAlign(LED_CSCFG, SV_FALSE, NDATA_SINGLE_EN);
    vIO32WriteFldAlign(LED_CSCFG, SV_FALSE, NFOOT_SINGLE_EN);    

    //-------------------------------------------------------
    // 4. set frame rate setting
    //-------------------------------------------------------    
    vIO32WriteFldAlign(LED_CON, 1, SW_SET_DATA);    // write buffered setting into register

    vIO32WriteFldAlign(LED_VSCFG, 0, FRAME_RATE);             //set frame rate 
    vIO32WriteFldAlign(LED_VSCFG, 0, VSYNC_LATENCY);           //set vsync latency    

    vIO32WriteFldAlign(LED_CON, 0, SW_SET_DATA);       
    //-------------------------------------------------------
    // 5. misc default setting
    //-------------------------------------------------------
    vIO32WriteFldAlign(LED_FMT, 0, LSB_MODE);       //set LSB mode : (0) msb first  (1) lsb first
    vIO32WriteFldAlign(LED_FMT, 0, SPI_RGB_MODE);       //set RGB mode : (0) white mode (1) rgb repeat mode
    vIO32WriteFldAlign(LED_FMT, 0, LED_TYPE);       //set LED type : (0) default no special mode

    vIO32WriteFldAlign(LED_CON, 1, SW_SET_DATA);    // write buffered setting into register
    vIO32WriteFldAlign(LED_CON, 0, SW_SET_DATA);   

    vIO32WriteFldAlign(LED_NOB, 28, MDATA_LEN);     // 224 x 2 bits
    vIO32WriteFldAlign(LED_NOB, 1, NDATA_LEN);          

    vIO32WriteFldAlign(LED_CON, 1, OP_MODE);  

    //--------------------------------------------------------
    // 6. register initial setting
    //--------------------------------------------------------    
#if AMBILIGHT_DEBUG_SW_REG    
    vIO32WriteFldAlign(AMBILIGHT_SPI_FC, 0, AMBILIGHT_SPI_FC_BLANK);      
    vIO32WriteFldAlign(AMBILIGHT_SPI_FC, 1, AMBILIGHT_SPI_FC_DSPRPT);  
    vIO32WriteFldAlign(AMBILIGHT_SPI_FC, 0, AMBILIGHT_SPI_FC_TMGRST);      
    vIO32WriteFldAlign(AMBILIGHT_SPI_FC, 1, AMBILIGHT_SPI_FC_EXTGCK);  
    vIO32WriteFldAlign(AMBILIGHT_SPI_FC, 1, AMBILIGHT_SPI_FC_OUTTMG);     

    vIO32WriteFldAlign(AMBILIGHT_SPI_BC, 0x7F, AMBILIGHT_SPI_BC_R);      
    vIO32WriteFldAlign(AMBILIGHT_SPI_BC, 0x7F, AMBILIGHT_SPI_BC_G);      
    vIO32WriteFldAlign(AMBILIGHT_SPI_BC, 0x7F, AMBILIGHT_SPI_BC_B);     

    vIO32WriteFldAlign(AMBILIGHT_SPI_FC, 0x25, AMBILIGHT_SPI_MAGIC);        
    
    for(i = 0; i < 12; i++)
    {
        vIO32WriteFldAlign(AMBILIGHT_SPI_COLOR_01 + (i*4), 0xFF, AMBILIGHT_SPI_COLOR_001);
        vIO32WriteFldAlign(AMBILIGHT_SPI_COLOR_01 + (i*4), 0xFF, AMBILIGHT_SPI_COLOR_002);       
    }
#endif    
#endif
}
