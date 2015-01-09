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
 * $RCSfile: digiamp_tas5707.c,v $
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

#include "digiamp_tas5707.h"
#include "sif_if.h"  // for i2c
#include "aud_if.h"
#include "aud_debug.h"
#include "drvcust_if.h"
#include "x_os.h"
#include "adac_if.h"
#include "x_gpio.h"

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

static void _TAS5707_RegWrite(UINT8 u1Addr, UINT8 u1Data);
static UINT8 _TAS5707_RegRead(UINT8 u1Addr);
static void _TAS5707_Init(void);
static void _TAS5707_Mute(BOOL fgEnable);
static void _TAS5707_DisableEffect (void);

//---------------------------------------------------------------------------
// Static variables
//---------------------------------------------------------------------------

static AMP_DRV_CB_T _rAmpTAS5707DrvCB =
{
    _TAS5707_DisableEffect // pfnDisableEffect, disable sound effect
};

static AMP_FUNCTBL_T _rAmpTAS5707FunTbl =
{
    _TAS5707_Init,							//pfnInit
    _TAS5707_Mute,							//pfnMute
    NULL,//_TAS5707_DacFmtCfg,  			   //pfnSetDacInFmt;
    NULL,						 	     		   //pfnSetDacGain;
    _TAS5707_RegWrite, 					//pfnWrite;
    _TAS5707_RegRead,					//pfnRead;
    &(_rAmpTAS5707DrvCB)                          // pfnDrvCB;
};

static AMP_T _rAmpTAS5707 =
{
    "TAS5707",				// szName[32];
    0,							// i2MaxGain
    0,						// i2MaxGain
    0,								// u1DacNum
    &_rAmpTAS5707FunTbl		// rCodecFuncTbl
};



//---------------------------------------------------------------------------
// Static functions
//---------------------------------------------------------------------------
static void ADAC_ResetAmp(VOID)
{
    UINT32 u4AmpResetGPIO;
    u4AmpResetGPIO = DRVCUST_OptGet(eAudioAmpResetGpioNum);
    if (u4AmpResetGPIO != 0xffffffff)
    {
        UNUSED(GPIO_SetOut((INT32)u4AmpResetGPIO, 1));
        x_thread_delay(5);
        UNUSED(GPIO_SetOut((INT32)u4AmpResetGPIO, 0));
        x_thread_delay(10);
        UNUSED(GPIO_SetOut((INT32)u4AmpResetGPIO, 1));
        x_thread_delay(5);
        UNUSED(GPIO_SetOut((INT32)u4AmpResetGPIO, 0));
        x_thread_delay(50);
    }
}

static void _TAS5707_RegWrite(UINT8 u1Addr, UINT8 u1Data)
{
    UINT32 u4WriteCount = 0;
    u4WriteCount = SIF_Write(SIF_CLK_DIV, TAS5707_ADDR, u1Addr, &u1Data, 1);
    LOG( 9,"I2C write (address,data) = (0x %x,0x %x)\n",u1Addr, u1Data);
    if (u4WriteCount > 0)
    {
        Printf("5707 write 1 byte ok!\n");
    }
    else
    {
        Printf("5707 write 1 byte fail=0x%2x, 0x%2x!\n", u1Addr, u1Data );
        LOG(5, "%c WRITE FAIL\n",_rAmpTAS5707.szName);
    }
}

static UINT8 _TAS5707_RegRead(UINT8 u1Addr)
{
    UINT32 u4ReadCount = 0;
    UINT8 u1Data;
    u4ReadCount = SIF_Read(SIF_CLK_DIV, TAS5707_ADDR, u1Addr, &u1Data, 1);
    LOG( 9,"I2C read (address,data) = (0x%x, 0x%x)\n",u1Addr, u1Data);
    if (u4ReadCount > 0)
    {
        Printf("5707 read 1 byte ok=0x%2x!\n", u1Data);
        return u1Data;
    }
    else
    {
        Printf("5707 read 1 byte fail!\n");
        LOG(5, "%c READ FAIL\n",_rAmpTAS5707.szName);
        return 0;
    }
}

static void _TAS5707_RegWriteN(UINT8 u1WordAddr, UINT8 *pu1Buf, UINT16 u2ByteCnt)
{
    UINT32 u4WriteCount = 0;
    u4WriteCount = SIF_Write(SIF_CLK_DIV, TAS5707_ADDR, u1WordAddr, pu1Buf, u2ByteCnt);
    //LOG( 9,"I2C write (address,data) = (0x %x,0x %x)\n",u1Addr, u1Data);
    if (u4WriteCount > 0)
    {
        Printf("5707 write n byte ok!\n");
    }
    else
    {
        Printf("5707 write n byte fail=0x%2x!\n", u1WordAddr );
        LOG(5, "%c WRITE FAIL\n",_rAmpTAS5707.szName);
    }
}

static void _TAS5707_Init(void)
{
    UINT8 u1Array[8];
    //Reset TAS5707
    ADAC_ResetAmp();
    // Make sure I2c is init
    SIF_Init();
    _TAS5707_RegWrite(TAS5707_TRIM, 0x00);
    x_thread_delay(60);

    // DRC energy time
    u1Array[0] = 0x00;
    u1Array[1] = 0x00;
    u1Array[2] = 0x88;
    u1Array[3] = 0x3f;
    u1Array[4] = 0x00;
    u1Array[5] = 0x7f;
    u1Array[6] = 0x77;
    u1Array[7] = 0xc0;
    //SIF_Write(SIF_CLK_DIV, TAS5707_ADDR, TAS5707_DRC_ENERGY, u1Array, 8);
    _TAS5707_RegWriteN(TAS5707_DRC_ENERGY, u1Array, 8);
    //DRC attack time(1ms)
    u1Array[0] = 0x00;
    u1Array[1] = 0x02;
    u1Array[2] = 0xa3;
    u1Array[3] = 0x9a;
    u1Array[4] = 0x00;
    u1Array[5] = 0x7d;
    u1Array[6] = 0x2f;
    u1Array[7] = 0xd2;
    //SIF_Write(SIF_CLK_DIV, TAS5707_ADDR, TAS5707_DRC_ATTACK, u1Array, 8);
    _TAS5707_RegWriteN(TAS5707_DRC_ATTACK, u1Array, 8);

    // DRC decay time(100ms)
    u1Array[0] = 0x00;
    u1Array[1] = 0x00;
    u1Array[2] = 0x06;
    u1Array[3] = 0xd3;
    u1Array[4] = 0x00;
    u1Array[5] = 0x7f;
    u1Array[6] = 0xf9;
    u1Array[7] = 0x2b;
    //SIF_Write(SIF_CLK_DIV, TAS5707_ADDR, TAS5707_DRC_DELAY, u1Array, 8);
    _TAS5707_RegWriteN(TAS5707_DRC_DELAY, u1Array, 8);

    //!DRC threshold 1(-14.7dB,9.1W,THD+N=6%,Gain=0dB,PVCC=12V)
    u1Array[0] = 0xfc;
    u1Array[1] = 0xc9;
    u1Array[2] = 0x39;
    u1Array[3] = 0x90;
    //SIF_Write(SIF_CLK_DIV, TAS5707_ADDR, TAS5707_DRC_THRESHOLD, u1Array, 4);
    _TAS5707_RegWriteN(TAS5707_DRC_THRESHOLD, u1Array, 4);

    //DRC compression ratio��100��
    u1Array[0] = 0x03;
    u1Array[1] = 0x81;
    u1Array[2] = 0x47;
    u1Array[3] = 0xaf;
    //SIF_Write(SIF_CLK_DIV, TAS5708_ADDR, TAS5707_DRC_COMPRESSION_RATIO, u1Array, 4);
    _TAS5707_RegWriteN(TAS5707_DRC_COMPRESSION_RATIO, u1Array, 4);

    //DRC offset��0��
    u1Array[0] = 0x00;
    u1Array[1] = 0x08;
    u1Array[2] = 0x42;
    u1Array[3] = 0x10;
    //SIF_Write(SIF_CLK_DIV, TAS5708_ADDR, TAS5707_DRC_OFFSET, u1Array, 4);
    _TAS5707_RegWriteN(TAS5707_DRC_OFFSET, u1Array, 4);

    //DRC control
    u1Array[0] = 0x00;
    u1Array[1] = 0x00;
    u1Array[2] = 0x00;
    u1Array[3] = 0x01;
    //SIF_Write(SIF_CLK_DIV, TAS5707_ADDR, TAS5708_DRC_CONTROL, u1Array, 4);
    _TAS5707_RegWriteN(TAS5707_DRC_CONTROL, u1Array, 4);

    //Bank switch control ��no automatic bank switching)
    u1Array[0] = 0x0f;
    u1Array[1] = 0x70;
    u1Array[2] = 0x80;
    u1Array[3] = 0x00;
    //SIF_Write(SIF_CLK_DIV, TAS5707_ADDR, TAS5707_BANK_SWITCH, u1Array, 4);
    _TAS5707_RegWriteN(TAS5707_BANK_SWITCH, u1Array, 4);

    //Input Mux Register(BD mode,opt.)
    u1Array[0] = 0x00;
    u1Array[1] = 0x89;
    u1Array[2] = 0x77;
    u1Array[3] = 0x7a;
    //SIF_Write(SIF_CLK_DIV, TAS5707_ADDR, TAS5708_INPUT_MUX, u1Array, 4);
    _TAS5707_RegWriteN(TAS5707_INPUT_MUX, u1Array, 4);

    // PWM Output MUX Register (Note: Writes to this register affect Inter-Channel Delay)
    u1Array[0] = 0x01;
    u1Array[1] = 0x20;
    //u1Array[1] = 0x02;
    u1Array[2] = 0x13;
    u1Array[3] = 0x45;
    //SIF_Write(SIF_CLK_DIV, TAS5707_ADDR, TAS5707_PWM_OUT_MUX, u1Array, 4);
    _TAS5707_RegWriteN(TAS5707_PWM_OUT_MUX, u1Array, 4);

    _TAS5707_RegWrite(TAS5707_MASTER_VOLUME, 0x30);
    _TAS5707_RegWrite(TAS5707_CH1_VOLUME, 0x30);
    _TAS5707_RegWrite(TAS5707_CH2_VOLUME, 0x30);
    _TAS5707_RegWrite(TAS5707_SOFT_MUTE, 0x00);
    _TAS5707_RegWrite(TAS5707_SYS_CONTROL1, 0xa0);
    _TAS5707_RegWrite(TAS5707_SERIAL_DATA, 0x05);
    _TAS5707_RegWrite(TAS5707_SYS_CONTROL2, 0x00);
    x_thread_delay(20);
    //vKrDelay1ms(20);
    _TAS5707_RegWrite(TAS5707_VOLUME_CONFIG, 0x91);
    _TAS5707_RegWrite(TAS5707_MODULATION_LIMIT, 0x02);
    _TAS5707_RegWrite(TAS5707_START_STOP_PERIOD, 0x0a);
    _TAS5707_RegWrite(TAS5707_BKND_ERR, 0x02);
    _TAS5707_RegWrite(TAS5707_IC_DELAY_CH1, 0x4c);
    _TAS5707_RegWrite(TAS5707_IC_DELAY_CH2, 0x34);
    _TAS5707_RegWrite(TAS5707_IC_DELAY_CH3, 0x1c);
    _TAS5707_RegWrite(TAS5707_IC_DELAY_CH4, 0x64);
}

static void _TAS5707_Mute(BOOL fgEnable)
{
    if (fgEnable)
    {
        _TAS5707_RegWrite(TAS5707_SOFT_MUTE, 0x03);
    }
    else
    {
        _TAS5707_RegWrite(TAS5707_SOFT_MUTE, 0x00);
    }
}

static void _TAS5707_DisableEffect (void)
{
    // FIX ME!!!
    // Please add disable sound effect commands in this function.
    // Let the freq. response the same after amp.
}

AMP_T* _AMP_GetAmpTAS5707FunTbl(void)
{
    return &_rAmpTAS5707;
}

/*
void test()
{
    //powr on sequence 1
    vHWADDACRegWriteByte(STA232W_CONFF, 0x5C);
    vHWADDACRegWriteByte(STA232W_MASMUTE, 0x01);
    //initializing
    vHWADDACRegWriteByte(STA232W_CONFA, 0x63); //256fps
    vHWADDACRegWriteByte(STA232W_CONFD, 0x10);
    //vHWADDACRegWrite(STA232W_CONFF, 0xDC); --> overlap, disable
    vHWADDACRegWriteByte(STA232W_MASTER,0x00);
    vHWADDACRegWriteByte(STA232W_MASVOL1,0x44);
    vHWADDACRegWriteByte(STA232W_MASVOL2,0x44);
    vHWADDACRegWriteByte(STA232W_AUTOMODE,0x80);
    vHWADDACRegWriteByte(STA232W_CH1CONFIG,0x00);
    vHWADDACRegWriteByte(STA232W_CH2CONFIG,0x40);
    vHWADDACRegWriteByte(STA232W_LIMITER,0x7F);
    //POWER ON SEQUENCE 2
    vHWADDACRegWriteByte(STA232W_CONFF,0xDC);
    vHWADDACRegWriteByte(STA232W_MASMUTE, 0x00);
}
*/


