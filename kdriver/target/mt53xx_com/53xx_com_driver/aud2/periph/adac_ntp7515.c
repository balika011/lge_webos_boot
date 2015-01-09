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
 * $RCSfile: adac_apogee.c,v $
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

#include "adac_ntp7515.h"
#include "sif_if.h"  // for i2c
#include "aud_if.h"
#include "aud_debug.h"
#include "drvcust_if.h"
#include "aud_cfg.h"

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
static void _NTP_RegWrite(UINT8 u1Addr, UINT8 u1Data);
void _NTP_RegWriteN(UINT8 u1WordAddr, UINT32 u4Data, UINT16 u2ByteCnt);
static UINT8 _NTP_RegRead(UINT8 u1Addr);

static void _NTP_Init(void);
static void _NTP_Mute(BOOL fgEnable);
static void _NTP_DisableEffect (void);

//static void _NTP_DacFmtCfg(DATA_FORMAT_T eFormat, MCLK_FREQUENCY_T eMclk);

//---------------------------------------------------------------------------
// Static variables
//---------------------------------------------------------------------------
//static BOOL fgSTMute=FALSE;
static AMP_DRV_CB_T _rAmpNTPDrvCB = 
{
    _NTP_DisableEffect // pfnDisableEffect, disable sound effect   
};

static AMP_FUNCTBL_T _rAmpFunTbl = 
{
    _NTP_Init,                  //pfnInit
    _NTP_Mute,                  //pfnMute 
    NULL,                                //pfnSetDacInFmt;
    NULL,                       //pfnSetDacGain;
    _NTP_RegWrite,              //pfnWrite;
    _NTP_RegRead,               //pfnRead;
    &_rAmpNTPDrvCB                          // pfnDrvCB;    
};

static AMP_T _rAmpNTP = 
{
    "NTP-7515",             // szName[32];
    256,                    // i2MaxGain
    -256,               // i2MaxGain
    2,                  // u1DacNum
    &_rAmpFunTbl        // rCodecFuncTbl
};

//---------------------------------------------------------------------------
// Static functions
//---------------------------------------------------------------------------

static void _NTP_RegWrite(UINT8 u1Addr, UINT8 u1Data)
{
    UINT32 u4WriteCount = 0; 

    u4WriteCount = SIF_X_Write(AUD_AMP_BUS_ID, SIF_CLK_DIV, NTP7515_ADDR, 1 ,u1Addr, &u1Data, 1);
    LOG(2, "_NTP_RegWrite (address, data) = (0x%x, 0x%x)\n",u1Addr, u1Data);

    if (u4WriteCount <= 0)
    {
        LOG(0, "NTP WRITE FAIL\n");
    } 
}

static UINT8 _NTP_RegRead(UINT8 u1Addr) 
{
    UINT32 u4ReadCount = 0;
    UINT8 u1Data;

    u4ReadCount = SIF_X_Read(AUD_AMP_BUS_ID, SIF_CLK_DIV, NTP7515_ADDR, 1 , u1Addr, &u1Data, 1);

    LOG(2, "_NTP_RegRead (address,data) = (0x%x, 0x%x)\n",u1Addr, u1Data);

    if (u4ReadCount > 0)
    {
        return u1Data;
    }   
    else
    {
        LOG(0, "NTP READ FAIL\n");
        return 0;
    }   
}

void _NTP_RegWriteN(UINT8 u1WordAddr, UINT32 u4Data, UINT16 u2ByteCnt)
{
    UINT32 u4WriteCount = 0;

    u4WriteCount = SIF_X_Write(AUD_AMP_BUS_ID, SIF_CLK_DIV, NTP7515_ADDR, 1, u1WordAddr, (UINT8 *)&u4Data, u2ByteCnt);
    if (u4WriteCount > 0)
    {
        LOG(2, "_NTP_RegWriteN (addr, data) = (%#x, %#x)\n", u1WordAddr, u4Data);
    }
    else
    {
        LOG(0, "_NTP_RegWriteN fail=0x%2x!\n", u1WordAddr);
    }
}

UINT32 _NTP_RegReadN(UINT8 u1WordAddr, UINT16 u2ByteCnt)
{
    UINT32 u4ReadCount = 0;
    UINT32 u4Data;
    u4ReadCount = SIF_X_Read(AUD_AMP_BUS_ID, SIF_CLK_DIV, NTP7515_ADDR, 1, u1WordAddr, (UINT8 *)&u4Data, u2ByteCnt);
    
    LOG(2, "_NTP_RegReadN (address,data) = (0x%x, 0x%x)\n",u1WordAddr, u4Data);
    
    if (u4ReadCount > 0)
    {
        return u4Data;
    }
    else
    {
        LOG(5, "NTP READ FAIL\n");
        return 0; 
    }
}

static void _NTP_Mute(BOOL fgEnable)
{

    if (fgEnable)  //Mute
    {
        _NTP_RegWrite(NTP_SMC, SMC_SM12);
        _NTP_RegWrite(NTP_PWM_SOOC,PWM_SOOC_POF12);
        _NTP_RegWrite(NTP_PWM_MASK_C0,PWM_MASK_C0_PWMM_FPMLD);
    }
    else  //Unmute
    {      
       _NTP_RegWrite(NTP_PWM_MASK_C0, PWM_MASK_C0_FPMLD);
       _NTP_RegWrite(NTP_PWM_SOOC, PWM_SOOC_POF1);
       _NTP_RegWrite(NTP_SMC, SMC_SM1);
    }

}

static void _NTP_Init(void)
{
    UINT32 u4Buffer;
    UINT32 u4AmpResetGPIO;
    UINT32 u4polarity;

    UNUSED(u4Buffer);
    UNUSED(DRVCUST_OptQuery(eAudioDigiAMPResetGpio, &u4AmpResetGPIO));
    UNUSED(DRVCUST_OptQuery(eAudioDigiAMPResetGpioPolarity, &u4polarity));
    if (u4AmpResetGPIO != 0xffffffff)
    {
        u4polarity = !u4polarity; 
        AUD_GPIOOutput(u4AmpResetGPIO, (INT32)u4polarity);
        x_thread_delay(10);
        u4polarity = !u4polarity;
        AUD_GPIOOutput(u4AmpResetGPIO, (INT32)u4polarity);
        x_thread_delay(10); 
    }
    
    SIF_Init();
    
    _NTP_RegWrite(NTP_BQFCP, BQFCP_NM);
    _NTP_RegWrite(NTP_GF,GF_GW);
    _NTP_RegWrite(NTP_DC,DC_SHDN_PIN_H);

#if 0
    _NTP_RegWrite(NTP_BQFCP,BQFCP_CH12); 

    u4Buffer = 0x0F01C139;
    _NTP_RegWriteN(NTP_L_DRC_LPF1_1,u4Buffer,4);
    u4Buffer = 0x0F01C139;
    _NTP_RegWriteN(NTP_L_DRC_LPF1_2,u4Buffer,4);
    u4Buffer = 0x20000000;
    _NTP_RegWriteN(NTP_L_DRC_LPF1_3,u4Buffer,4);
    u4Buffer = 0x0F7C7D8F;
    _NTP_RegWriteN(NTP_L_DRC_LPF1_4,u4Buffer,4);
    u4Buffer = 0x20000000;
    _NTP_RegWriteN(NTP_L_DRC_LPF1_5,u4Buffer,4);
    
    u4Buffer = 0x0D223A32;
    _NTP_RegWriteN(NTP_L_DRC_LPF2_1,u4Buffer,4);
    u4Buffer = 0x0E223A32;
    _NTP_RegWriteN(NTP_L_DRC_LPF2_2,u4Buffer,4);
    u4Buffer = 0x0D223A32;
    _NTP_RegWriteN(NTP_L_DRC_LPF2_3,u4Buffer,4);
    u4Buffer = 0x111BB474;
    _NTP_RegWriteN(NTP_L_DRC_LPF2_4,u4Buffer,4);
    u4Buffer = 0x10888600;
    _NTP_RegWriteN(NTP_L_DRC_LPF2_5,u4Buffer,4);
    
    u4Buffer = 0x103F1F64;
    _NTP_RegWriteN(NTP_H_DRC_HPF1_1,u4Buffer,4);
    u4Buffer = 0x10BF1F64;
    _NTP_RegWriteN(NTP_H_DRC_HPF1_2,u4Buffer,4);
    u4Buffer = 0x20000000;
    _NTP_RegWriteN(NTP_H_DRC_HPF1_3,u4Buffer,4);
    u4Buffer = 0x0F7C7D8F;
    _NTP_RegWriteN(NTP_H_DRC_HPF1_4,u4Buffer,4);
    u4Buffer = 0x20000000;
    _NTP_RegWriteN(NTP_H_DRC_HPF1_5,u4Buffer,4);    
    
    u4Buffer = 0x102FFBBA;
    _NTP_RegWriteN(NTP_H_DRC_HPF2_1,u4Buffer,4);
    u4Buffer = 0x11AFFBBA;
    _NTP_RegWriteN(NTP_H_DRC_HPF2_2,u4Buffer,4);
    u4Buffer = 0x102FFBBA;
    _NTP_RegWriteN(NTP_H_DRC_HPF2_3,u4Buffer,4);
    u4Buffer = 0x111BB474;
    _NTP_RegWriteN(NTP_H_DRC_HPF2_4,u4Buffer,4);
    u4Buffer = 0x10888600;
    _NTP_RegWriteN(NTP_H_DRC_HPF2_5,u4Buffer,4);
    
    u4Buffer = 0x09551315;
    _NTP_RegWriteN(NTP_S_DRC_SPF1_1,u4Buffer,4);
    u4Buffer = 0x09551315;
    _NTP_RegWriteN(NTP_S_DRC_SPF1_2,u4Buffer,4);
    u4Buffer = 0x20000000;
    _NTP_RegWriteN(NTP_S_DRC_SPF1_3,u4Buffer,4);
    u4Buffer = 0x107CABB4;
    _NTP_RegWriteN(NTP_S_DRC_SPF1_4,u4Buffer,4);
    u4Buffer = 0x20000000;
    _NTP_RegWriteN(NTP_S_DRC_SPF1_5,u4Buffer,4);
    
    u4Buffer = 0x11000000;
    _NTP_RegWriteN(NTP_S_DRC_SPF2_1,u4Buffer,4);
    u4Buffer = 0x20000000;
    _NTP_RegWriteN(NTP_S_DRC_SPF2_2,u4Buffer,4);
    u4Buffer = 0x20000000;
    _NTP_RegWriteN(NTP_S_DRC_SPF2_3,u4Buffer,4);
    u4Buffer = 0x20000000;
    _NTP_RegWriteN(NTP_S_DRC_SPF2_4,u4Buffer,4);
    u4Buffer = 0x20000000;
    _NTP_RegWriteN(NTP_S_DRC_SPF2_5,u4Buffer,4);
 
    //Normal mode
    _NTP_RegWrite(NTP_BQFCP, BQFCP_NM);
#endif   

    _NTP_RegWrite(NTP_DRC_C_0, 0xA8);
    _NTP_RegWrite(NTP_DRC_C_1, 0x10);
    _NTP_RegWrite(NTP_DRC_C_2, 0xA8);
    _NTP_RegWrite(NTP_DRC_C_3, 0x10);
    _NTP_RegWrite(NTP_DRC_C_6, 0xA8);
    _NTP_RegWrite(NTP_DRC_C_7, 0x11);
    _NTP_RegWrite(NTP_DRC_C_10, 0xA8);
    _NTP_RegWrite(NTP_DRC_C_11, 0x41);
    _NTP_RegWrite(NTP_DRC_C_9, 0x11);
    _NTP_RegWrite(NTP_DRC_C_12, 0x01);
    _NTP_RegWrite(NTP_CH1_VC, CVC_V);
    _NTP_RegWrite(NTP_CH2_VC, CVC_V);

    _NTP_RegWrite(NTP_M_PWM_C,M_PWM_C_MD); 
    _NTP_RegWrite(NTP_CPVC,CPVC_PS12);

#if 0
    _NTP_RegWrite(NTP_BQFCP, BQFCP_CH12);

    u4Buffer = 0x11000000;
    _NTP_RegWriteN(NTP_BQ1_1,u4Buffer,4);
    u4Buffer = 0x20000000;
    _NTP_RegWriteN(NTP_BQ1_2,u4Buffer,4);
    u4Buffer = 0x20000000;
    _NTP_RegWriteN(NTP_BQ1_3,u4Buffer,4);
    u4Buffer = 0x20000000;
    _NTP_RegWriteN(NTP_BQ1_4,u4Buffer,4);
    u4Buffer = 0x20000000;
    _NTP_RegWriteN(NTP_BQ1_5,u4Buffer,4);

    u4Buffer = 0x11000000;
    _NTP_RegWriteN(NTP_BQ2_1,u4Buffer,4);
    u4Buffer = 0x20000000;
    _NTP_RegWriteN(NTP_BQ2_2,u4Buffer,4);
    u4Buffer = 0x20000000;
    _NTP_RegWriteN(NTP_BQ2_3,u4Buffer,4);
    u4Buffer = 0x20000000;
    _NTP_RegWriteN(NTP_BQ2_4,u4Buffer,4);
    u4Buffer = 0x20000000;
    _NTP_RegWriteN(NTP_BQ2_5,u4Buffer,4);

    u4Buffer = 0x11000000;
    _NTP_RegWriteN(NTP_BQ3_1,u4Buffer,4);
    u4Buffer = 0x20000000;
    _NTP_RegWriteN(NTP_BQ3_2,u4Buffer,4);
    u4Buffer = 0x20000000;
    _NTP_RegWriteN(NTP_BQ3_3,u4Buffer,4);
    u4Buffer = 0x20000000;
    _NTP_RegWriteN(NTP_BQ3_4,u4Buffer,4);
    u4Buffer = 0x20000000;
    _NTP_RegWriteN(NTP_BQ3_5,u4Buffer,4);

    u4Buffer = 0x11000000;
    _NTP_RegWriteN(NTP_BQ4_1,u4Buffer,4);
    u4Buffer = 0x20000000;
    _NTP_RegWriteN(NTP_BQ4_2,u4Buffer,4);
    u4Buffer = 0x20000000;
    _NTP_RegWriteN(NTP_BQ4_3,u4Buffer,4);
    u4Buffer = 0x20000000;
    _NTP_RegWriteN(NTP_BQ4_4,u4Buffer,4);
    u4Buffer = 0x20000000;
    _NTP_RegWriteN(NTP_BQ4_5,u4Buffer,4);

    u4Buffer = 0x11000000;
    _NTP_RegWriteN(NTP_BQ5_1,u4Buffer,4);
    u4Buffer = 0x20000000;
    _NTP_RegWriteN(NTP_BQ5_2,u4Buffer,4);
    u4Buffer = 0x20000000;
    _NTP_RegWriteN(NTP_BQ5_3,u4Buffer,4);
    u4Buffer = 0x20000000;
    _NTP_RegWriteN(NTP_BQ5_4,u4Buffer,4);
    u4Buffer = 0x20000000;
    _NTP_RegWriteN(NTP_BQ5_5,u4Buffer,4);

    u4Buffer = 0x11000000;
    _NTP_RegWriteN(NTP_BQ6_1,u4Buffer,4);
    u4Buffer = 0x20000000;
    _NTP_RegWriteN(NTP_BQ6_2,u4Buffer,4);
    u4Buffer = 0x20000000;
    _NTP_RegWriteN(NTP_BQ6_3,u4Buffer,4);
    u4Buffer = 0x20000000;
    _NTP_RegWriteN(NTP_BQ6_4,u4Buffer,4);
    u4Buffer = 0x20000000;
    _NTP_RegWriteN(NTP_BQ6_5,u4Buffer,4);

    u4Buffer = 0x11000000;
    _NTP_RegWriteN(NTP_BQ7_1,u4Buffer,4);
    u4Buffer = 0x20000000;
    _NTP_RegWriteN(NTP_BQ7_2,u4Buffer,4);
    u4Buffer = 0x20000000;
    _NTP_RegWriteN(NTP_BQ7_3,u4Buffer,4);
    u4Buffer = 0x20000000;
    _NTP_RegWriteN(NTP_BQ7_4,u4Buffer,4);
    u4Buffer = 0x20000000;
    _NTP_RegWriteN(NTP_BQ7_5,u4Buffer,4);

    u4Buffer = 0x11000000;
    _NTP_RegWriteN(NTP_BQ8_1,u4Buffer,4);
    u4Buffer = 0x20000000;
    _NTP_RegWriteN(NTP_BQ8_2,u4Buffer,4);
    u4Buffer = 0x20000000;
    _NTP_RegWriteN(NTP_BQ8_3,u4Buffer,4);
    u4Buffer = 0x20000000;
    _NTP_RegWriteN(NTP_BQ8_4,u4Buffer,4);
    u4Buffer = 0x20000000;
    _NTP_RegWriteN(NTP_BQ8_5,u4Buffer,4);

    u4Buffer = 0x11000000;
    _NTP_RegWriteN(NTP_BQ9_1,u4Buffer,4);
    u4Buffer = 0x20000000;
    _NTP_RegWriteN(NTP_BQ9_2,u4Buffer,4);
    u4Buffer = 0x20000000;
    _NTP_RegWriteN(NTP_BQ9_3,u4Buffer,4);
    u4Buffer = 0x20000000;
    _NTP_RegWriteN(NTP_BQ9_4,u4Buffer,4);
    u4Buffer = 0x20000000;
    _NTP_RegWriteN(NTP_BQ9_5,u4Buffer,4);

    u4Buffer = 0x11000000;
    _NTP_RegWriteN(NTP_BQ10_1,u4Buffer,4);
    u4Buffer = 0x20000000;
    _NTP_RegWriteN(NTP_BQ10_2,u4Buffer,4);
    u4Buffer = 0x20000000;
    _NTP_RegWriteN(NTP_BQ10_3,u4Buffer,4);
    u4Buffer = 0x20000000;
    _NTP_RegWriteN(NTP_BQ10_4,u4Buffer,4);
    u4Buffer = 0x20000000;
    _NTP_RegWriteN(NTP_BQ10_5,u4Buffer,4);

    u4Buffer = 0x11000000;
    _NTP_RegWriteN(NTP_BQ11_1,u4Buffer,4);
    u4Buffer = 0x20000000;
    _NTP_RegWriteN(NTP_BQ11_2,u4Buffer,4);
    u4Buffer = 0x20000000;
    _NTP_RegWriteN(NTP_BQ11_3,u4Buffer,4);
    u4Buffer = 0x20000000;
    _NTP_RegWriteN(NTP_BQ11_4,u4Buffer,4);
    u4Buffer = 0x20000000;
    _NTP_RegWriteN(NTP_BQ11_5,u4Buffer,4);

    u4Buffer = 0x11000000;
    _NTP_RegWriteN(NTP_BQ12_1,u4Buffer,4);
    u4Buffer = 0x20000000;
    _NTP_RegWriteN(NTP_BQ12_2,u4Buffer,4);
    u4Buffer = 0x20000000;
    _NTP_RegWriteN(NTP_BQ12_3,u4Buffer,4);
    u4Buffer = 0x20000000;
    _NTP_RegWriteN(NTP_BQ12_4,u4Buffer,4);
    u4Buffer = 0x20000000;
    _NTP_RegWriteN(NTP_BQ12_5,u4Buffer,4);

     _NTP_RegWrite(NTP_BQFCP,BQFCP_APEQ);

    u4Buffer = 0x10000000;
    _NTP_RegWriteN(NTP_APEQ_C0_1,u4Buffer,4);
    u4Buffer = 0x10000000;
    _NTP_RegWriteN(NTP_APEQ_C0_2,u4Buffer,4);
    u4Buffer = 0x10000000;
    _NTP_RegWriteN(NTP_APEQ_C0_3,u4Buffer,4);
    u4Buffer = 0x10000000;
    _NTP_RegWriteN(NTP_APEQ_C0_4,u4Buffer,4);
    u4Buffer = 0x10000000;
    _NTP_RegWriteN(NTP_APEQ_C0_5,u4Buffer,4);
    u4Buffer = 0x10000000;
    _NTP_RegWriteN(NTP_APEQ_C0_6,u4Buffer,4);
    
    u4Buffer = 0x11000000;
    _NTP_RegWriteN(NTP_APEQ_C1_1,u4Buffer,4);
    u4Buffer = 0x11000000;
    _NTP_RegWriteN(NTP_APEQ_C1_2,u4Buffer,4);
    u4Buffer = 0x11000000;
    _NTP_RegWriteN(NTP_APEQ_C1_3,u4Buffer,4);
    u4Buffer = 0x11000000;
    _NTP_RegWriteN(NTP_APEQ_C1_4,u4Buffer,4);
    u4Buffer = 0x11000000;
    _NTP_RegWriteN(NTP_APEQ_C1_5,u4Buffer,4);
    u4Buffer = 0x11000000;
    _NTP_RegWriteN(NTP_APEQ_C1_6,u4Buffer,4);

    u4Buffer = 0x11000000;
    _NTP_RegWriteN(NTP_APEQ_C2_1,u4Buffer,4);
    u4Buffer = 0x11000000;
    _NTP_RegWriteN(NTP_APEQ_C2_2,u4Buffer,4);
    u4Buffer = 0x11000000;
    _NTP_RegWriteN(NTP_APEQ_C2_3,u4Buffer,4);
    u4Buffer = 0x11000000;
    _NTP_RegWriteN(NTP_APEQ_C2_4,u4Buffer,4);
    u4Buffer = 0x11000000;
    _NTP_RegWriteN(NTP_APEQ_C2_5,u4Buffer,4);
    u4Buffer = 0x11000000;
    _NTP_RegWriteN(NTP_APEQ_C2_6,u4Buffer,4);

    u4Buffer = 0x20000000;
    _NTP_RegWriteN(NTP_APEQ_C3_1,u4Buffer,4);
    u4Buffer = 0x20000000;
    _NTP_RegWriteN(NTP_APEQ_C3_2,u4Buffer,4);
    u4Buffer = 0x20000000;
    _NTP_RegWriteN(NTP_APEQ_C3_3,u4Buffer,4);
    u4Buffer = 0x20000000;
    _NTP_RegWriteN(NTP_APEQ_C3_4,u4Buffer,4);
    u4Buffer = 0x20000000;
    _NTP_RegWriteN(NTP_APEQ_C3_5,u4Buffer,4);
    u4Buffer = 0x20000000;
    _NTP_RegWriteN(NTP_APEQ_C3_6,u4Buffer,4);

    u4Buffer = 0x11000000;
    _NTP_RegWriteN(NTP_APEQ_C4_1,u4Buffer,4);
    u4Buffer = 0x11000000;
    _NTP_RegWriteN(NTP_APEQ_C4_2,u4Buffer,4);
    u4Buffer = 0x11000000;
    _NTP_RegWriteN(NTP_APEQ_C4_3,u4Buffer,4);
    u4Buffer = 0x11000000;
    _NTP_RegWriteN(NTP_APEQ_C4_4,u4Buffer,4);
    u4Buffer = 0x11000000;
    _NTP_RegWriteN(NTP_APEQ_C4_5,u4Buffer,4);
    u4Buffer = 0x11000000;
    _NTP_RegWriteN(NTP_APEQ_C4_6,u4Buffer,4);
    _NTP_RegWrite(NTP_BQFCP,BQFCP_NM);
#endif

    _NTP_RegWrite(NTP_APEQ_PO,APEQ_PO_V);
    _NTP_RegWrite(NTP_APEQ_FC2,APEQ_FC2_V);
    _NTP_RegWrite(NTP_APEQ_FC3,APEQ_FC3_V);
    _NTP_RegWrite(NTP_APEQ_FC4,APEQ_FC4_V);
    _NTP_RegWrite(NTP_APEQ_FC5,APEQ_FC5_V);
    _NTP_RegWrite(NTP_APEQ_FC6,APEQ_FC6_V);
    _NTP_RegWrite(NTP_APEQ_FC7,APEQ_FC7_V);

    _NTP_RegWrite(NTP_PEQ_FC0_CH1,PEQ_FC0_CH1_V);
    _NTP_RegWrite(NTP_PEQ_FC0_CH2,PEQ_FC0_CH2_V);
    _NTP_RegWrite(NTP_PEQ_FC1_CH1,PEQ_FC1_CH1_V);
    _NTP_RegWrite(NTP_PEQ_FC1_CH2,PEQ_FC1_CH2_V);
    _NTP_RegWrite(NTP_APEQ_FC0_CH1,APEQ_FC0_CH1_V);
    _NTP_RegWrite(NTP_APEQ_FC0_CH2,APEQ_FC0_CH2_V);
    _NTP_RegWrite(NTP_APEQ_FC1_CH1,APEQ_FC1_CH1_V);
    _NTP_RegWrite(NTP_APEQ_FC1_CH2,APEQ_FC1_CH2_V); 
    _NTP_RegWrite(NTP_PWM_PC,PWM_PC_V);
    
    _NTP_RegWrite(NTP_SSRC_C0,SSRC_C0_V); 
    _NTP_RegWrite(NTP_SSC0,SSC0_V); 
    _NTP_RegWrite(NTP_SSC1,SSC1_V); 
    _NTP_RegWrite(NTP_SSRHC1,SSRHC1_V); 
    _NTP_RegWrite(NTP_AD_DC_PC0,AD_DC_PC0_V); 
    _NTP_RegWrite(NTP_D_BTL_DC_PC1,D_BTL_DC_PC1_V); 
    _NTP_RegWrite(NTP_DC_PC2,DC_PC2_V); 
    _NTP_RegWrite(NTP_PWM_MASK_C0,PWM_MASK_C0_FPMLD); 
    _NTP_RegWrite(NTP_PWM_SOOC,PWM_SOOC_POF1); 
    _NTP_RegWrite(NTP_SMC,SMC_SM1); 
    _NTP_RegWrite(NTP_MV_SPK_PWM_SOOC,MV_SPK_PWM_SOOC_V); 
    _NTP_RegWrite(NTP_MVFC,MVFC_V);
    
}

static void _NTP_DisableEffect (void)
{
    // FIX ME!!!
    // Please add disable sound effect commands in this function.
    // Let the freq. response the same after amp.
}

AMP_T* _AMP_GetAmpNTP7515FunTbl(void)
{
    return &_rAmpNTP;
}


