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
 * $RCSfile: digiamp_others.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file adac_apogee.c
 *  Brief of file adac_apogee.c.
 *  Details of file adac_apogee.c (optional).
 */

#define AUD_EXTERN_FILE

//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------

#include "x_lint.h"
LINT_EXT_HEADER_BEGIN

#include "codec_NE3201.h"
#include "sif_if.h"  // for i2c
#include "aud_if.h"
#include "aud_debug.h"
#include "drvcust_if.h"
#include "x_os.h"

LINT_EXT_HEADER_END

//---------------------------------------------------------------------------
// Configurations
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Constant definitions
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Type definitions
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Macro definitions
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Imported variables
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Imported functions
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Static function forward declarations
//---------------------------------------------------------------------------

static void _NE3201_RegWrite(UINT8 u1Addr, UINT8 u1Data);
static UINT8 _NE3201_RegRead(UINT8 u1Addr);
static void _NE3201_Init(void);
static void _NE3201_Mute(BOOL fgEnable);
static void _NE3201_DacFmtCfg(DATA_FORMAT_T eFormat, MCLK_FREQUENCY_T eMclk);
static void _NE3201_DisableEffect (void);
static void _NE3201_RAM_Access(void);

//---------------------------------------------------------------------------
// Static variables
//---------------------------------------------------------------------------
/*static UINT8 NE3201_DEF[NE3201_REG_LEN] = {
    0x6C, 0x10, 0x00, 0xE0, 0x03, 0x00, 0xC8, 0x30, 
    0x00, 0x00, 0x00, 0x00, 0x40, 0x40, 0x00, 0x00,
    0x8A, 0x38, 0x48, 0xB8, 0xC8, 0x00, 0x00, 0x00, 
    0x00, 0x44, 0x20, 0x22, 0x0C, 0x8C, 0x00, 0x00,
    0x00, 0xC2, 0xC3, 0xC4, 0xC5, 0x00, 0x00, 0x00, 
    0x00, 0x17, 0x5F, 0x80, 0x00, 0x00, 0x00, 0x00
};*/

static INT32 NE3201_RAM_Data[NE3201_CRAM_LEN]= 
{
    0x0,0x0,0x0,0x0,0x200000,0x0,0x0,0x0,0x0,0x200000,0x0,0x0,0x0,0x0,0x200000,0x200000,
    0x0,0x0,0x0,0x0,0x200000,0x0,0x0,0x0,0x0,0x200000,0x0,0x0,0x0,0x0,0x200000,
    0x0,0x0,0x0,0x0,0x200000,0x0,0x0,0x0,0x0,0x200000,0x200000,0x200000,0x0,0x0,0x0,0x0,
    0x200000,0x0,0x0,0x0,0x0,0x200000,0x0,0x0,0x0,0x0,0x200000,0x0,0x0,0x0,0x0,0x200000,
    0x0,0x0,0x0,0x0,0x200000,0x200000,0x200000,0x0,0x0,0x0,0x0,0x200000,0x0,0x0,0x0,0x0,
    0x200000,0x0,0x0,0x0,0x0,0x200000,0x0,0x0,0x0,0x0,0x200000,0x0,0x0,0x0,0x0,0x200000,
    0x200000,0x0,0x0,0x0,0x0,0x200000,0x0,0x0,0x0,0x0,0x200000,0x0,0x0,0x0,0x0,0x200000,
    0x200000,0x0,0x0,0x0,0x0,0x200000,0x0,0x0,0x0,0x0,0x200000,0x0,0x0,0x0,0x0,0x200000,
    0x0,0x0,0x0,0x0,0x200000,0x0,0x0,0x0,0x0,0x200000,0x200000,0x200000,0x0,0x0,0x0,0x0,
    0x200000,0x0,0x0,0x0,0x0,0x200000,0x0,0x0,0x0,0x0,0x200000,0x0,0x0,0x0,0x0,0x200000,
    0x0,0x0,0x0,0x0,0x200000,0x200000,0x200000,0x0,0x0,0x0,0x0,0x200000,0x0,0x0,0x0,0x0,
    0x200000,0x0,0x0,0x0,0x0,0x200000,0x0,0x0,0x0,0x0,0x200000,0x0,0x0,0x0,0x0,0x200000,
    0x200000,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x200000,0x3,0x0,
    0x0,0x0,0x0,0x1D1E26,0x170ED,0x170ED,0xFFE00000,0xA3D7,0x200000,0x1062,0x200000,
    0x200000,0x200000,0x200000,0x200000,0x200000,0x200000,0xC,0x200000,0x4,0x1543,
    0x200000,0x200000,0x200000,0x200000,0x0,0x0,0x186A0,0x0,0x1FFFFF,0xE0,0x0,0x0,0x0,
    0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0
};

static AMP_DRV_CB_T _rAmpNE3201DrvCB =
{
    _NE3201_DisableEffect // pfnDisableEffect, disable sound effect
};

static AMP_FUNCTBL_T _rAmpNE3201FunTbl =
{
    _NE3201_Init,                           //pfnInit
    _NE3201_Mute,                           //pfnMute
    _NE3201_DacFmtCfg,                 //pfnSetDacInFmt;
    NULL,                                          //pfnSetDacGain;
    _NE3201_RegWrite,                   //pfnWrite;
    _NE3201_RegRead,                    //pfnRead;
    &(_rAmpNE3201DrvCB)              // pfnDrvCB;
};

static AMP_T _rAmpNE3201 =
{
    "NE3201",               // szName[32];
    0,                          // i2MaxGain
    0,                      // i2MaxGain
    0,                              // u1DacNum
    &(_rAmpNE3201FunTbl)        // rCodecFuncTbl
};

//---------------------------------------------------------------------------
// Static functions
//---------------------------------------------------------------------------

static void _NE3201_RegWrite(UINT8 u1Addr, UINT8 u1Data)
{
    UINT32 u4WriteCount = 0;
    
    u4WriteCount = SIF_X_Write(AUD_AMP_BUS_ID, SIF_CLK_DIV, NE3201_ADDR, 1, u1Addr, &u1Data, 1); 

    LOG( 9,"I2C write (address,data) = (0x%x,0x%x)\n",u1Addr, u1Data); 

    if (u4WriteCount <= 0)
    {
        LOG(0, "NE3201 write fail!!!(address,data) = (0x%x,0x%x)\n", u1Addr, u1Data);
    }
}

static UINT8 _NE3201_RegRead(UINT8 u1Addr)
{
    UINT32 u4ReadCount = 0;
    UINT8 u1Data;

    u4ReadCount = SIF_X_Read(AUD_AMP_BUS_ID, SIF_CLK_DIV, NE3201_ADDR, 1, u1Addr, &u1Data, 1);
    LOG( 9,"I2C read (address,data) = (0x%x, 0x%x)\n",u1Addr, u1Data);

    if (u4ReadCount > 0)
    {
        return u1Data;
    }   
    else
    {
        LOG(0, "NE3201 read fail address = 0x%x\n", u1Addr);
        return 0;
    }   
}

static void _NE3201_Init(void)
{
    AOUT_CFG_T*     prAudOutFmt;

    // Make sure I2c is init
    LOG(0, "NE3201 Init Start\n");
    SIF_Init();
    //Power on GPIO
    //ADAC_GpioAmpEnable(TRUE);
    //x_thread_delay(10);

    //FS_48K, MCLK_256FS
    _NE3201_RegWrite(NE3201_CLK_CTL, 0x6C);  //Reg0x00[7:0] = 0x6C
    //No LR reverse, I2S, 24bits
    _NE3201_RegWrite(NE3201_SDATAIF, 0x03);  //Reg0x04[7:0] = 0x03
    // Format config: I2S
    UNUSED(DRVCUST_OptQuery(eAudioOutFmtTable, (UINT32 *)(void *)&prAudOutFmt));
    _NE3201_DacFmtCfg(prAudOutFmt->eFormat, (MCLK_FREQUENCY_T)DRVCUST_OptGet(eAudioDacMclk));  //Reg0x04[7:0] = 0x03

#ifdef NE3201_TERNARY_MODE
    //Ternary mode
    LOG(0, "Config NE3201 as Ternary mode\n");
    _NE3201_RegWrite(NE3201_RESVD_11, 0x00);  //Reg0x11[7:0] = 0x00
    _NE3201_RegWrite(NE3201_RESVD_12, 0x18);  //Reg0x12[7:0] = 0x18
    _NE3201_RegWrite(NE3201_RESVD_13, 0xC8);  //Reg0x13[7:0] = 0xC8
    _NE3201_RegWrite(NE3201_RESVD_14, 0xD0);  //Reg0x14[7:0] = 0xD0

    _NE3201_RegWrite(NE3201_CH1A_CFG, 0x42);  //Reg0x21[7:0] = 0x42
    _NE3201_RegWrite(NE3201_CH1B_CFG, 0x43);  //Reg0x22[7:0] = 0x43
    _NE3201_RegWrite(NE3201_CH2A_CFG, 0x44);  //Reg0x23[7:0] = 0x44
    _NE3201_RegWrite(NE3201_CH2B_CFG, 0x45);  //Reg0x24[7:0] = 0x45
    _NE3201_RegWrite(NE3201_RESVD_0F, 0x00);  //Reg0x0F[7:0] = 0x00
#else
    //Low EMI mode
    LOG(0, "Config NE3201 as Low EMI mode\n");
    _NE3201_RegWrite(NE3201_RESVD_11, 0x00);  //Reg0x11[7:0] = 0x00
    _NE3201_RegWrite(NE3201_RESVD_12, 0x04);  //Reg0x12[7:0] = 0x04 
    _NE3201_RegWrite(NE3201_RESVD_13, 0x10);  //Reg0x13[7:0] = 0x10
    _NE3201_RegWrite(NE3201_RESVD_14, 0x14);  //Reg0x14[7:0] = 0x14     

    _NE3201_RegWrite(NE3201_CH1A_CFG, 0x52);  //Reg0x21[7:0] = 0x52
    _NE3201_RegWrite(NE3201_CH1B_CFG, 0x53);  //Reg0x22[7:0] = 0x53
    _NE3201_RegWrite(NE3201_CH2A_CFG, 0x54);  //Reg0x23[7:0] = 0x54
    _NE3201_RegWrite(NE3201_CH2B_CFG, 0x55);  //Reg0x24[7:0] = 0x55
    _NE3201_RegWrite(NE3201_RESVD_0F, 0x0C);  //Reg0x0F[7:0] = 0x0C
#endif
    _NE3201_RegWrite(NE3201_PROTECTION_MODE, _NE3201_RegRead(NE3201_PROTECTION_MODE) & 0xDF);
    _NE3201_RegWrite(NE3201_PROTECTION_MODE, _NE3201_RegRead(NE3201_PROTECTION_MODE) | 0x20);
    x_thread_delay(13); 
    _NE3201_RegWrite(NE3201_SYS_CTL, _NE3201_RegRead(NE3201_SYS_CTL) & SHUTDOWNALL_Mask);  //Reg0x03[6] = 0
    x_thread_delay(1);
    _NE3201_RegWrite(NE3201_CLR_RAM, _NE3201_RegRead(NE3201_CLR_RAM) & ClearRAM_Mask);  //Reg0x1A[5] = 0
    
    _NE3201_RegWrite(NE3201_MUTE_CTL, _NE3201_RegRead(NE3201_MUTE_CTL) & PULL_DC_Mask);  //Reg0x06[7] = 0
    x_thread_delay(10);
    _NE3201_RAM_Access();
    _NE3201_RegWrite(NE3201_MOD_LIMIT, _NE3201_RegRead(NE3201_MOD_LIMIT) & ForceZeroDis);  //Reg0x10[3] = 0
    _NE3201_Mute(FALSE);   //Reg0x06[3] = 0
    LOG(0, "NE3201 Init End\n");
    
}

static void _NE3201_Mute(BOOL fgEnable)
{
    if (fgEnable)
    {
        _NE3201_RegWrite(NE3201_MUTE_CTL, _NE3201_RegRead(NE3201_MUTE_CTL) | SoftMuteAll);
        LOG(5, "[AUD_AMP] NE3201 Mute\n");
    }
    else
    {
        _NE3201_RegWrite(NE3201_MUTE_CTL, _NE3201_RegRead(NE3201_MUTE_CTL) & SoftMute_Mask);
        LOG(5, "[AUD_AMP] NE3201 Unute\n");
    }
    x_thread_delay(25); //wait for unmute process finished,delay 50ms 
}

static void _NE3201_DacFmtCfg(DATA_FORMAT_T eFormat, MCLK_FREQUENCY_T eMclk)
{
    UNUSED(eMclk);

    switch (eFormat)
    {
        case FORMAT_LJ:
            _NE3201_RegWrite(NE3201_SDATAIF, ((_NE3201_RegRead(NE3201_SDATAIF) & (~SD_FMT_MASK)) | FMT_LJ));
            break;
        case FORMAT_I2S:
            _NE3201_RegWrite(NE3201_SDATAIF, ((_NE3201_RegRead(NE3201_SDATAIF) & (~SD_FMT_MASK)) | FMT_I2S));
            break;
        case FORMAT_RJ:
            _NE3201_RegWrite(NE3201_SDATAIF, ((_NE3201_RegRead(NE3201_SDATAIF) & (~SD_FMT_MASK)) | FMT_RJ));
            break;
        default:
            _NE3201_RegWrite(NE3201_SDATAIF, ((_NE3201_RegRead(NE3201_SDATAIF) & (~SD_FMT_MASK)) | FMT_I2S));
            break;
    }
}

static void _NE3201_DisableEffect (void)
{
    // FIX ME!!!
    // Please add disable sound effect commands in this function.
    // Let the freq. response the same after amp.
}

static void _NE3201_RAM_Access(void)
{
    UINT32 u4Idx;
    UINT8 u1Addr1, u1Addr2;
    UNUSED(_NE3201_RAM_Access);
  
    //stop DSP 
    _NE3201_RegWrite(NE3201_DSP_CTL, _NE3201_RegRead(NE3201_DSP_CTL)| DSP_Stop);
    _NE3201_RegWrite(NE3201_SYS_CTL, _NE3201_RegRead(NE3201_SYS_CTL) | SHUTDOWN_ALL);

    //enter RAM mode
    _NE3201_RegWrite(NE3201_PROG_RAM, _NE3201_RegRead(NE3201_PROG_RAM) | RAM_Mode_On);
    
    //send the Coefficients to IC RAM
    //send the start address of Coefficients RAM
    //Start address = $800*4
    u1Addr1 = 0x20; //Start address = $800*4
    u1Addr2 = 0x00;
    _NE3201_RegWrite(NE3201_RAMADDR0, u1Addr1);  //Reg0x1E[7:0] = 0x20
    _NE3201_RegWrite(NE3201_RAMADDR1, u1Addr2);  //Reg0x1F[7:0] = 0x00
    //send the data of Coefficients RAM
    for (u4Idx = 0; u4Idx < NE3201_CRAM_LEN; u4Idx++)
    { 
        _NE3201_RegWrite(NE3201_DATA, NE3201_RAM_Data[u4Idx] & 0xFF);
        _NE3201_RegWrite(NE3201_DATA, (NE3201_RAM_Data[u4Idx]>>8) & 0xFF);
        _NE3201_RegWrite(NE3201_DATA, (NE3201_RAM_Data[u4Idx]>>16) & 0xFF);
        _NE3201_RegWrite(NE3201_DATA, 0x00);
    }
        
    //exit RAM Mode
    _NE3201_RegWrite(NE3201_PROG_RAM,_NE3201_RegRead(NE3201_PROG_RAM) & RAM_Mode_Mask);
    
    //DSP run
    _NE3201_RegWrite(NE3201_DSP_CTL,_NE3201_RegRead(NE3201_DSP_CTL) & DSP_Run_Mask);
    
    _NE3201_RegWrite(NE3201_SYS_CTL,_NE3201_RegRead(NE3201_SYS_CTL) & SHUTDOWNALL_Mask);
}

AMP_T* _AMP_GetAmpNE3201FunTbl(void)
{
    return &_rAmpNE3201;
}


