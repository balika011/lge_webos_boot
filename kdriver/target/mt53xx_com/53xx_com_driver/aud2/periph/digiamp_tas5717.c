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
 * $RCSfile: digiamp_tas5717.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file adac_apogee.c
 *  Brief of file adac_apogee.c.
 *  Details of file adac_apogee.c (optional).
 */


//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------

#include "x_lint.h"
LINT_EXT_HEADER_BEGIN

#include "digiamp_tas5717.h"
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

static void _TAS5717_RegWrite(UINT8 u1Addr, UINT8 u1Data);
static UINT8 _TAS5717_RegRead(UINT8 u1Addr);
static void _TAS5717_Init(void);
static void _TAS5717_Mute(BOOL fgEnable);
void _TAS5717_Headphone_Mute_Setting(BOOL fgEnable); // Raul 20101026, for HP plug in/out setting.
void _TAS5717_SetDRC(AUD_AMP_ID_T u1AmpID, AUD_AQ_AMP_DRC2_T eDrcData);
void _TAS5717_SetPostScale(AUD_AMP_ID_T u1AmpID, UINT32 u4Value);
void _TAS5717_SetPreScale(AUD_AMP_ID_T u1AmpID, UINT32 u4Value);
static void _TAS5717_DisableEffect (void);

//---------------------------------------------------------------------------
// Static variables
//---------------------------------------------------------------------------

static AMP_DRV_CB_T _rAmpTAS5717DrvCB = 
{
    _TAS5717_DisableEffect // pfnDisableEffect, disable sound effect   
};

static AMP_FUNCTBL_T _rAmpTAS5717FunTbl =
{
    _TAS5717_Init,							//pfnInit
    _TAS5717_Mute,							//pfnMute
    NULL,//_TAS5717_DacFmtCfg,  			   //pfnSetDacInFmt;
    NULL,						 	     		   //pfnSetDacGain;
    _TAS5717_RegWrite, 					//pfnWrite;
    _TAS5717_RegRead,					//pfnRead;
    &_rAmpTAS5717DrvCB                          // pfnDrvCB;    
};

static AMP_T _rAmpTAS5717 =
{
    "TAS5717",				// szName[32];
    0,							// i2MaxGain
    0,						// i2MaxGain
    0,								// u1DacNum
    &_rAmpTAS5717FunTbl		// rCodecFuncTbl
};


//---------------------------------------------------------------------------
// Static functions
//---------------------------------------------------------------------------
static void ADAC_ResetAmp(VOID)
{
    UINT32 u4AmpResetGPIO;
    u4AmpResetGPIO = DRVCUST_OptGet(eAudioDigiAMPResetGpio);
    if (u4AmpResetGPIO != 0xffffffff)
    {
        UNUSED(GPIO_SetOut((INT32)u4AmpResetGPIO, 0));
        x_thread_delay(5);
        UNUSED(GPIO_SetOut((INT32)u4AmpResetGPIO, 1));
        x_thread_delay(50);
    }
}

static void _TAS5717_RegWrite(UINT8 u1Addr, UINT8 u1Data)
{
    UINT32 u4WriteCount = 0;
    u4WriteCount = SIF_X_Write(0, SIF_CLK_DIV, TAS5717_ADDR, 1 ,u1Addr, &u1Data, 1);
    LOG( 9,"I2C write (address,data) = (0x %x,0x %x)\n",u1Addr, u1Data);
    if (u4WriteCount > 0)
    {
        LOG(11, "5717 write 1 byte ok!\n");
    }
    else
    {
        Printf("5717 write 1 byte fail=0x%2x, 0x%2x!\n", u1Addr, u1Data );
        Printf("%c WRITE FAIL\n",_rAmpTAS5717.szName);
    }
}

static UINT8 _TAS5717_RegRead(UINT8 u1Addr)
{
    UINT32 u4ReadCount = 0;
    UINT8 u1Data;
    u4ReadCount = SIF_X_Read(0, SIF_CLK_DIV, TAS5717_ADDR, 1 , u1Addr, &u1Data, 1);
    LOG( 9,"I2C read (address,data) = (0x%x, 0x%x)\n",u1Addr, u1Data);
    if (u4ReadCount > 0)
    {
        LOG(11, "5717 read 1 byte ok=0x%2x!\n", u1Data);
        return u1Data;
    }
    else
    {
        Printf("5717 read 1 byte fail!\n");
        Printf("%c READ FAIL\n",_rAmpTAS5717.szName);
        return 0;
    }
}

void _TAS5717_RegWriteN(UINT8 u1WordAddr, UINT8 *pu1Buf, UINT16 u2ByteCnt)
{
    UINT32 u4WriteCount = 0;
    u4WriteCount = SIF_X_Write(0, SIF_CLK_DIV, TAS5717_ADDR, 1 ,u1WordAddr, pu1Buf, u2ByteCnt);
    //LOG( 9,"I2C write (address,data) = (0x %x,0x %x)\n",u1Addr, u1Data);
    if (u4WriteCount > 0)
    {
        LOG(11, "5717 write n byte ok!\n");
    }
    else
    {
        Printf("5717 write n byte fail=0x%2x!\n", u1WordAddr );
        Printf("%c WRITE FAIL\n",_rAmpTAS5717.szName);
    }
}

#if 0   // Unused
BOOL _TAS5717_RegReadN(UINT8 u1Addr, UINT8 *pu1Buf, UINT16 u2ByteCnt)
{
    UINT32 u4ReadCount = 0;
    //UINT8 u1Data[20];
    u4ReadCount = SIF_X_Read(0, SIF_CLK_DIV, TAS5717_ADDR, 1 , u1Addr, pu1Buf, u2ByteCnt);
    if (u4ReadCount > 0)
    {
        LOG(11, "5717 read ok!\n");
        return TRUE;
    }
    else
    {
        Printf("5717 read 1 byte fail!\n");
        Printf("%c READ FAIL\n",_rAmpTAS5717.szName);
        return 0;
    }
}
#endif

static void _TAS5717_Init(void)
{
    UINT8 u1Array[8];
    AUD_AQ_AMP_DRC2_T eDrcData;
    
    //Reset TAS5717
    ADAC_ResetAmp();
    // Make sure I2c is init
    SIF_Init();
      
    _TAS5717_RegWrite(TAS5717_TRIM, 0xC0);
    x_thread_delay(60);

    //DRC control, Disable
    u1Array[0] = 0x00;
    u1Array[1] = 0x02;
    u1Array[2] = 0x00;
    u1Array[3] = 0x03;      // Bit1, Bit0 enable DRC2 and DRC1. (set 0x3)
    _TAS5717_RegWriteN(TAS5717_DRC_CONTROL, u1Array, 4);

    //Bank switch control no automatic bank switching)
    u1Array[0] = 0x0f;
    u1Array[1] = 0x70;
    u1Array[2] = 0x80;
    u1Array[3] = 0x10;      // D7 set 0 -> EQ On, D4 set 1 -> L and R are ganged for EQ biquads;
    _TAS5717_RegWriteN(TAS5717_BANK_SWITCH, u1Array, 4);

    //Input Mux Register(BD mode,opt.)
    u1Array[0] = 0x00;
    u1Array[1] = 0x01;
    u1Array[2] = 0x77;
    u1Array[3] = 0x72;      ///[Joel] 0x7A
    _TAS5717_RegWriteN(TAS5717_INPUT_MUX, u1Array, 4);

    // PWM Output MUX Register (Note: Writes to this register affect Inter-Channel Delay)
    u1Array[0] = 0x01;
    u1Array[1] = 0x02;     
    u1Array[2] = 0x13;
    u1Array[3] = 0x45;
    _TAS5717_RegWriteN(TAS5717_PWM_OUT_MUX, u1Array, 4);


    //Set Master, CH1, CH3, Headphone Volume to 0dB (0x30).
    u1Array[0] = 0x00;
    u1Array[1] = 0x30;      
    _TAS5717_RegWriteN(TAS5717_MASTER_VOLUME, u1Array, 2);
    u1Array[0] = 0x00;
    u1Array[1] = 0x59;   
    _TAS5717_RegWriteN(TAS5717_CH1_VOLUME, u1Array, 2);
    _TAS5717_RegWriteN(TAS5717_CH2_VOLUME, u1Array, 2);
    _TAS5717_RegWriteN(TAS5717_CH3_VOLUME, u1Array, 2);

    //_TAS5717_RegWrite(TAS5717_HP_VOLUME, 0x30);
    
    _TAS5717_RegWrite(TAS5717_SOFT_MUTE, 0x00);
    _TAS5717_RegWrite(TAS5717_SYS_CONTROL1, 0x80);      ///[Joel] 0xA0
    _TAS5717_RegWrite(TAS5717_SERIAL_DATA, 0x05);
    //_TAS5717_RegWrite(TAS5717_MAX_DUTY_CYCLE, 0x97);    ///Max duty bonding select
    _TAS5717_RegWrite(TAS5717_SYS_CONTROL2, 0x02);
    x_thread_delay(20);
    //_TAS5717_RegWrite(TAS5717_VOLUME_CONFIG, FADE_SLEW_85MS);
    _TAS5717_RegWrite(TAS5717_VOLUME_CONFIG, 0xf1); // for FBC??
    _TAS5717_RegWrite(TAS5717_MODULATION_LIMIT, 0x01);  ///[Joel] 0x01
    _TAS5717_RegWrite(TAS5717_START_STOP_PERIOD, 0x68); ///[Joel] 0x0a);
    //Back-end reset 5707 set 0x02(299ms), 5717 suggest 0x07 (1047ms)
    _TAS5717_RegWrite(TAS5717_BKND_ERR, 0x07);

    //High impact on audio performance (e.g., dynamic range, THD, crosstalk etc.)
    //Change by AD or BD mode, TAS5707 select BD mode.
    _TAS5717_RegWrite(TAS5717_IC_DELAY_CH1, 0x77);  ///[Joel]0x4c);
    _TAS5717_RegWrite(TAS5717_IC_DELAY_CH2, 0x7F);  ///[Joel]0x34);
    _TAS5717_RegWrite(TAS5717_IC_DELAY_CH3, 0x77);  ///[Joel]0x1c);
    _TAS5717_RegWrite(TAS5717_IC_DELAY_CH4, 0x7F);  ///[Joel]0x64);
    
    _TAS5717_RegWrite(TAS5717_Device_ID, 0xC1);
    _TAS5717_RegWrite(TAS5717_ERR_STATUS, 0x80);
    
    eDrcData.u4DRC1FiltAlpha = 0x00010000;
    eDrcData.u4DRC1FiltOmega = 0x007f0000;
    eDrcData.u4DRC1AttackRate = 0x00000100;
    eDrcData.u4DRC1ReleaseRate = 0xffffff00;
    eDrcData.u4DRC1AttackTh = 0x11fc0000;
    eDrcData.u4DRC2FiltAlpha = 0x00000100;
    eDrcData.u4DRC2FiltOmega = 0x00800000;
    eDrcData.u4DRC2AttackRate = 0x00080000;
    eDrcData.u4DRC2ReleaseRate = 0xfff80000;
    eDrcData.u4DRC2AttackTh = 0x0d480000;
    _TAS5717_SetDRC(AUD_AMP_ID_LR, eDrcData);
    
    _TAS5717_SetPostScale(AUD_AMP_ID_LR, 0x00400000);
    
    u1Array[0] = 0xA5;
    u1Array[1] = 0xA5;     
    u1Array[2] = 0xA5;
    u1Array[3] = 0xA5;
    _TAS5717_RegWriteN(TAS5717_UPDATE_DEV_ADDR_KEY, u1Array, 4);
    
    u1Array[0] = 0x00;
    u1Array[1] = 0x00;     
    u1Array[2] = 0x00;
    u1Array[3] = 0x56;
    _TAS5717_RegWriteN(TAS5717_UPDATE_DEV_ADDR, u1Array, 4);
}

static void _TAS5717_Mute(BOOL fgEnable)
{
    if (fgEnable)
    {
        //_TAS5717_RegWrite(TAS5717_SOFT_MUTE, (SW_MUTE_CH1|SW_MUTE_CH2|SW_MUTE_CH3));
        _TAS5717_RegWrite(TAS5717_SOFT_MUTE, (SW_MUTE_CH1|SW_MUTE_CH2));
    }
    else
    {
        _TAS5717_RegWrite(TAS5717_SOFT_MUTE, 0x00);
    }
}

static void _TAS5717_DisableEffect (void)
{
    // FIX ME!!!
    // Please add disable sound effect commands in this function.
    // Let the freq. response the same after amp.
}


AMP_T* _AMP_GetAmpTAS5717FunTbl(void)
{
    return &_rAmpTAS5717;
}

void _TAS5717_SetDRC(AUD_AMP_ID_T u1AmpID, AUD_AQ_AMP_DRC2_T eDrcData)
{
    UINT8 u1Array[8];

    u1Array[3] = (eDrcData.u4DRC1FiltAlpha & 0xff);
    u1Array[2] = ((eDrcData.u4DRC1FiltAlpha >> 8) & 0xff);     
    u1Array[1] = ((eDrcData.u4DRC1FiltAlpha >> 16) & 0xff);     
    u1Array[0] = ((eDrcData.u4DRC1FiltAlpha >> 24) & 0xff);       
    u1Array[7] = (eDrcData.u4DRC1FiltOmega & 0xff);
    u1Array[6] = ((eDrcData.u4DRC1FiltOmega>> 8) & 0xff);  
    u1Array[5] = ((eDrcData.u4DRC1FiltOmega>> 16) & 0xff);  
    u1Array[4] = ((eDrcData.u4DRC1FiltOmega>> 24) & 0xff);   
    _TAS5717_RegWriteN(TAS5717_DRC1_SOFTFILT_ALPHA_OMEGA, u1Array, 8);

    u1Array[3] = (eDrcData.u4DRC1AttackRate & 0xff);
    u1Array[2] = ((eDrcData.u4DRC1AttackRate >> 8) & 0xff);     
    u1Array[1] = ((eDrcData.u4DRC1AttackRate >> 16) & 0xff);     
    u1Array[0] = ((eDrcData.u4DRC1AttackRate >> 24) & 0xff);   
    u1Array[7] = (eDrcData.u4DRC1ReleaseRate & 0xff);
    u1Array[6] = ((eDrcData.u4DRC1ReleaseRate>> 8) & 0xff);  
    u1Array[5] = ((eDrcData.u4DRC1ReleaseRate>> 16) & 0xff);  
    u1Array[4] = ((eDrcData.u4DRC1ReleaseRate>> 24) & 0xff);   
    _TAS5717_RegWriteN(TAS5717_DRC1_ATTACK_RELEASE_RATE, u1Array, 8);

    u1Array[3] = (eDrcData.u4DRC1AttackTh & 0xff);
    u1Array[2] = ((eDrcData.u4DRC1AttackTh >> 8) & 0xff);     
    u1Array[1] = ((eDrcData.u4DRC1AttackTh >> 16) & 0xff);     
    u1Array[0] = ((eDrcData.u4DRC1AttackTh >> 24) & 0xff);   
    _TAS5717_RegWriteN(TAS5717_DRC1_ATTACK_THRESHOLD, u1Array, 4);

    u1Array[3] = (eDrcData.u4DRC2FiltAlpha & 0xff);
    u1Array[2] = ((eDrcData.u4DRC2FiltAlpha >> 8) & 0xff);     
    u1Array[1] = ((eDrcData.u4DRC2FiltAlpha >> 16) & 0xff);     
    u1Array[0] = ((eDrcData.u4DRC2FiltAlpha >> 24) & 0xff);   
    u1Array[7] = (eDrcData.u4DRC2FiltOmega & 0xff);
    u1Array[6] = ((eDrcData.u4DRC2FiltOmega>> 8) & 0xff);  
    u1Array[5] = ((eDrcData.u4DRC2FiltOmega>> 16) & 0xff);  
    u1Array[4] = ((eDrcData.u4DRC2FiltOmega>> 24) & 0xff);   
    _TAS5717_RegWriteN(TAS5717_DRC2_SOFTFILT_ALPHA_OMEGA, u1Array, 8);

    u1Array[3] = (eDrcData.u4DRC2AttackRate & 0xff);
    u1Array[2] = ((eDrcData.u4DRC2AttackRate >> 8) & 0xff);     
    u1Array[1] = ((eDrcData.u4DRC2AttackRate >> 16) & 0xff);     
    u1Array[0] = ((eDrcData.u4DRC2AttackRate >> 24) & 0xff);   
    u1Array[7] = (eDrcData.u4DRC2ReleaseRate & 0xff);
    u1Array[6] = ((eDrcData.u4DRC2ReleaseRate>> 8) & 0xff);  
    u1Array[5] = ((eDrcData.u4DRC2ReleaseRate>> 16) & 0xff);  
    u1Array[4] = ((eDrcData.u4DRC2ReleaseRate>> 24) & 0xff);   
    _TAS5717_RegWriteN(TAS5717_DRC2_ATTACK_RELEASE_RATE, u1Array, 8);

    u1Array[3] = (eDrcData.u4DRC2AttackTh & 0xff);
    u1Array[2] = ((eDrcData.u4DRC2AttackTh >> 8) & 0xff);     
    u1Array[1] = ((eDrcData.u4DRC2AttackTh >> 16) & 0xff);     
    u1Array[0] = ((eDrcData.u4DRC2AttackTh >> 24) & 0xff);   
    _TAS5717_RegWriteN(TAS5717_DRC2_ATTACK_THRESHOLD, u1Array, 4);
}

void _TAS5717_SetPostScale(AUD_AMP_ID_T u1AmpID, UINT32 u4Value)
{
    UINT8 u1Array[4];

    u1Array[0] = ((u4Value >> 24) & 0xff);
    u1Array[1] = ((u4Value >> 16) & 0xff);
    u1Array[2] = ((u4Value >> 8) & 0xff);    
    u1Array[3] = (u4Value & 0xff);       
    _TAS5717_RegWriteN(TAS5717_POSTSCALE, u1Array, 4);
}

#if 0   // Unused
void _TAS5717_Headphone_Mute_Setting(BOOL fgEnable)
{
    if (fgEnable)
    {
        _TAS5717_RegWrite(TAS5717_SYS_CONTROL2, 0x00);
        _TAS5717_RegWrite(TAS5717_SOFT_MUTE, 0x00);            
    }
    else
    {
        _TAS5717_RegWrite(TAS5717_SYS_CONTROL2, 0x10);
        _TAS5717_RegWrite(TAS5717_SOFT_MUTE, 0x00);
        x_thread_delay(1000);
    }
}

void _TAS5717_SetChannelVol(AUD_AMP_ID_T u1AmpID, UINT8 u1ChId, UINT16 u2Value)
{
    UINT8 u1Array[8];

    u1Array[0] = ((u2Value >> 8) & 0xff);     
    u1Array[1] = (u2Value & 0xff);

    switch (u1ChId)
    {
        case 0: //Master volume
            _TAS5717_RegWriteN(TAS5717_MASTER_VOLUME, u1Array,2);
            break;
        case 1: //Channel 1
            _TAS5717_RegWriteN(TAS5717_CH1_VOLUME, u1Array,2);
            break;        
        case 2: //Channel 2
            _TAS5717_RegWriteN(TAS5717_CH2_VOLUME, u1Array,2);
            break;
        case 3: //Channel HP
            _TAS5717_RegWriteN(TAS5717_HP_VOLUME, u1Array,2);
            break;
    };
}

void _TAS5717_SetEQ(AUD_AMP_ID_T u1AmpID, AUD_AQ_EXTPEQ_DATA_T *rAQExtPeqData)
{
    UINT8 u1Array[20];
    UINT8 u1Idx, i;
    UNUSED(u1Array);

    //Jack fixed continue change sound mode can hear pop sound issue     
    x_thread_delay(200);

    u1Idx= 0;    
    for (i=0; i<7; i++) 
    {        
        u1Array[0] = (UINT8)(((rAQExtPeqData->u4Data[u1Idx]) & 0xFF000000) >> 24);   
        u1Array[1] = (UINT8)(((rAQExtPeqData->u4Data[u1Idx]) & 0xFF0000) >> 16);
        u1Array[2] = (UINT8)(((rAQExtPeqData->u4Data[u1Idx]) & 0xFF00) >> 8);
        u1Array[3] = (UINT8)(((rAQExtPeqData->u4Data[u1Idx]) & 0xFF));
        LOG(8, "EXT_PEQ BAND [%d] b1 byte3~byte0 = 0x%x, 0x%x, 0x%x, 0x%x\n", u1Idx, u1Array[0], u1Array[1], u1Array[2], u1Array[3]);
        u1Idx += 1;

        u1Array[4] = (UINT8)(((rAQExtPeqData->u4Data[u1Idx]) & 0xFF000000) >> 24);    
        u1Array[5] = (UINT8)(((rAQExtPeqData->u4Data[u1Idx]) & 0xFF0000) >> 16);
        u1Array[6] = (UINT8)(((rAQExtPeqData->u4Data[u1Idx]) & 0xFF00) >> 8);
        u1Array[7] = (UINT8)(((rAQExtPeqData->u4Data[u1Idx]) & 0xFF));
        LOG(8, "EXT_PEQ BAND [%d] b2 byte3~byte0 = 0x%x, 0x%x, 0x%x, 0x%x\n", u1Idx, u1Array[4], u1Array[5], u1Array[6], u1Array[7]);
        u1Idx += 1;

        u1Array[8] = (UINT8)(((rAQExtPeqData->u4Data[u1Idx]) & 0xFF000000) >> 24); 
        u1Array[9] = (UINT8)(((rAQExtPeqData->u4Data[u1Idx]) & 0xFF0000) >> 16);
        u1Array[10] = (UINT8)(((rAQExtPeqData->u4Data[u1Idx]) & 0xFF00) >> 8);
        u1Array[11] = (UINT8)(((rAQExtPeqData->u4Data[u1Idx]) & 0xFF));
        LOG(8, "EXT_PEQ BAND [%d] a1 byte3~byte0 = 0x%x, 0x%x, 0x%x, 0x%x\n", u1Idx, u1Array[8], u1Array[9], u1Array[10], u1Array[11]);
        u1Idx += 1;

        u1Array[12] = (UINT8)(((rAQExtPeqData->u4Data[u1Idx]) & 0xFF000000) >> 24);    
        u1Array[13] = (UINT8)(((rAQExtPeqData->u4Data[u1Idx]) & 0xFF0000) >> 16);
        u1Array[14] = (UINT8)(((rAQExtPeqData->u4Data[u1Idx]) & 0xFF00) >> 8);
        u1Array[15] = (UINT8)(((rAQExtPeqData->u4Data[u1Idx]) & 0xFF));
        LOG(8, "EXT_PEQ BAND [%d] a2 byte3~byte0 = 0x%x, 0x%x, 0x%x, 0x%x\n", u1Idx, u1Array[12], u1Array[13], u1Array[14], u1Array[15]);
        u1Idx += 1;

        u1Array[16] = (UINT8)(((rAQExtPeqData->u4Data[u1Idx]) & 0xFF000000) >> 24);    
        u1Array[17] = (UINT8)(((rAQExtPeqData->u4Data[u1Idx]) & 0xFF0000) >> 16);
        u1Array[18] = (UINT8)(((rAQExtPeqData->u4Data[u1Idx]) & 0xFF00) >> 8);
        u1Array[19] = (UINT8)(((rAQExtPeqData->u4Data[u1Idx]) & 0xFF));
        LOG(8, "EXT_PEQ BAND [%d] b0 byte3~byte0 = 0x%x, 0x%x, 0x%x, 0x%x\n", u1Idx, u1Array[16], u1Array[17], u1Array[18], u1Array[19]);
        u1Idx += 1;

        _TAS5717_RegWriteN(TAS5717_PEQ_CH1BQ1+i, u1Array, 20);
    }
}

void _TAS5717_SetDRCRest(void)
{
    UINT8 u1Array[20];

    u1Array[0] = 0x00;
    u1Array[1] = 0x7D;     
    u1Array[2] = 0x22;     
    u1Array[3] = 0x0D;   
    u1Array[4] = 0x0F;
    u1Array[5] = 0x82;         
    u1Array[6] = 0xDD;  
    u1Array[7] = 0xF3;   
    u1Array[8] = 0x00; 
    u1Array[9] = 0x00;  
    u1Array[10] = 0x00;   
    u1Array[11] = 0x00; 
    u1Array[12] = 0x00;   
    u1Array[13] = 0x7A; 
    u1Array[14] = 0x44;   
    u1Array[15] = 0x1B; 
    u1Array[16] = 0x00;   
    u1Array[17] = 0x00; 
    u1Array[18] = 0x00;   
    u1Array[19] = 0x00; 
    _TAS5717_RegWriteN(0x58, u1Array, 20);
    
    u1Array[0] = 0x00;         
    u1Array[1] = 0x02;     
    u1Array[2] = 0xDD;     
    u1Array[3] = 0xF2;   
    u1Array[4] = 0x00;
    u1Array[5] = 0x02;         
    u1Array[6] = 0xDD;  
    u1Array[7] = 0xF2;   
    u1Array[8] = 0x00; 
    u1Array[9] = 0x00;  
    u1Array[10] = 0x00;   
    u1Array[11] = 0x00; 
    u1Array[12] = 0x00;   
    u1Array[13] = 0x7A; 
    u1Array[14] = 0x44;   
    u1Array[15] = 0x1B; 
    u1Array[16] = 0x00;   
    u1Array[17] = 0x00; 
    u1Array[18] = 0x00;   
    u1Array[19] = 0x00; 
    _TAS5717_RegWriteN(0x5A, u1Array, 20);
            
    u1Array[0] = 0x00;         
    u1Array[1] = 0x7D;     
    u1Array[2] = 0x22;     
    u1Array[3] = 0x0D;   
    u1Array[4] = 0x0F;
    u1Array[5] = 0x82;         
    u1Array[6] = 0xDD;  
    u1Array[7] = 0xF3;   
    u1Array[8] = 0x00; 
    u1Array[9] = 0x00;  
    u1Array[10] = 0x00;   
    u1Array[11] = 0x00; 
    u1Array[12] = 0x00;   
    u1Array[13] = 0x7A; 
    u1Array[14] = 0x44;   
    u1Array[15] = 0x1B; 
    u1Array[16] = 0x00;   
    u1Array[17] = 0x00; 
    u1Array[18] = 0x00;   
    u1Array[19] = 0x00; 
    _TAS5717_RegWriteN(0x5C, u1Array, 20);
    
    u1Array[0] = 0x00;         
    u1Array[1] = 0x02;     
    u1Array[2] = 0xDD;     
    u1Array[3] = 0xF2;   
    u1Array[4] = 0x00;
    u1Array[5] = 0x02;         
    u1Array[6] = 0xDD;  
    u1Array[7] = 0xF2;   
    u1Array[8] = 0x00; 
    u1Array[9] = 0x00;  
    u1Array[10] = 0x00;   
    u1Array[11] = 0x00; 
    u1Array[12] = 0x00;   
    u1Array[13] = 0x7A; 
    u1Array[14] = 0x44;   
    u1Array[15] = 0x1B; 
    u1Array[16] = 0x00;   
    u1Array[17] = 0x00; 
    u1Array[18] = 0x00;   
    u1Array[19] = 0x00; 
    _TAS5717_RegWriteN(0x5E, u1Array, 20);
    
    u1Array[0] = 0x00;         
    u1Array[1] = 0x80;     
    u1Array[2] = 0x00;     
    u1Array[3] = 0x00;   
    u1Array[4] = 0x00;
    u1Array[5] = 0x80;         
    u1Array[6] = 0x00;  
    u1Array[7] = 0x00;   
    _TAS5717_RegWriteN(0x51, u1Array, 8);
    
    u1Array[0] = 0x00;         
    u1Array[1] = 0x80;     
    u1Array[2] = 0x00;     
    u1Array[3] = 0x00;   
    u1Array[4] = 0x00;
    u1Array[5] = 0x80;         
    u1Array[6] = 0x00;  
    u1Array[7] = 0x00;   
    _TAS5717_RegWriteN(0x52, u1Array, 8);
}

void _TAS5717_SetPreScale(AUD_AMP_ID_T u1AmpID, UINT32 u4Value)
{
    UINT8 u1Array[4];

    u1Array[0] = ((u4Value >> 24) & 0xff);
    u1Array[1] = ((u4Value >> 16) & 0xff);
    u1Array[2] = ((u4Value >> 8) & 0xff);    
    u1Array[3] = (u4Value & 0xff);       
    _TAS5717_RegWriteN(TAS5717_PRESCALE, u1Array, 4);
}

void _TAS5717_Register_Dump(void)
{
    UINT8 u1Addr, i;
    UINT8 u1Data[20];
    UINT8 u1Count;

    // register 0x00~0x06, 1 bytes
    u1Count = 1;
    for (u1Addr=0x00; u1Addr<=0x06; u1Addr++)
    {
        if (_TAS5717_RegReadN(u1Addr, u1Data, u1Count))
        {
            Printf("Read Amp Addr = 0x%x, Data: ", u1Addr);
            for (i=0; i<u1Count; i++)
            {
                Printf("0x%x ",u1Data[i]);
            }
            Printf("\n");
        }
        else
        {
            Printf("Read (Addr) = 0x%x fail\n", u1Addr);
    }
}

    // register 0x07~0x0A, 2 bytes
    u1Count = 2;
    for (u1Addr=0x07; u1Addr<=0x0A; u1Addr++)
    {
        if (_TAS5717_RegReadN(u1Addr, u1Data, u1Count))
{
            Printf("Read Amp Addr = 0x%x, Data: ", u1Addr);
            for (i=0; i<u1Count; i++)
            {
                Printf("0x%x ",u1Data[i]);
            }
            Printf("\n");
        }
        else
        {
            Printf("Read (Addr) = 0x%x fail\n", u1Addr);
        }    
}

    // register 0x0B~0x1F, 1 bytes
    u1Count = 1;
    for (u1Addr=0x0B; u1Addr<=0x1F; u1Addr++)
    {
        if (_TAS5717_RegReadN(u1Addr, u1Data, u1Count))
        {
            Printf("Read Amp Addr = 0x%x, Data: ", u1Addr);
            for (i=0; i<u1Count; i++)
            {
                Printf("0x%x ",u1Data[i]);
            }
            Printf("\n");
        }
        else
        {
            Printf("Read (Addr) = 0x%x fail\n", u1Addr);
        }    
    }
    
    // register 0x20~0x25, 4 bytes
    u1Count = 4;
    for (u1Addr=0x20; u1Addr<=0x25; u1Addr++)
    {
        if (_TAS5717_RegReadN(u1Addr, u1Data, u1Count))
        {
            Printf("Read Amp Addr = 0x%x, Data: ", u1Addr);
            for (i=0; i<u1Count; i++)
            {
                Printf("0x%x ",u1Data[i]);
            }
            Printf("\n");
        }
        else
        {
            Printf("Read (Addr) = 0x%x fail\n", u1Addr);
        }    
    } 
    
    // register 0x26~0x39, 20 bytes
    u1Count = 20;
    for (u1Addr=0x26; u1Addr<=0x39; u1Addr++)
    {
        if (_TAS5717_RegReadN(u1Addr, u1Data, u1Count))
        {
            Printf("Read Amp Addr = 0x%x, Data: ", u1Addr);
            for (i=0; i<u1Count; i++)
            {
                Printf("0x%x ",u1Data[i]);
            }
            Printf("\n");
        }
        else
        {
            Printf("Read (Addr) = 0x%x fail\n", u1Addr);
        }    
    }
    
    // register 0x3A     , 4 bytes
    u1Count = 4;
    for (u1Addr=0x3A; u1Addr<=0x3A; u1Addr++)
    {
        if (_TAS5717_RegReadN(u1Addr, u1Data, u1Count))
        {
            Printf("Read Amp Addr = 0x%x, Data: ", u1Addr);
            for (i=0; i<u1Count; i++)
            {
                Printf("0x%x ",u1Data[i]);
            }
            Printf("\n");
        }
        else
        {
            Printf("Read (Addr) = 0x%x fail\n", u1Addr);
        }    
    }
    
    // register 0x3B~0x3F, 8 bytes
    u1Count = 8;
    for (u1Addr=0x3B; u1Addr<=0x3F; u1Addr++)
    {
        if (_TAS5717_RegReadN(u1Addr, u1Data, u1Count))
        {
            Printf("Read Amp Addr = 0x%x, Data: ", u1Addr);
            for (i=0; i<u1Count; i++)
            {
                Printf("0x%x ",u1Data[i]);
            }
            Printf("\n");
        }
        else
        {
            Printf("Read (Addr) = 0x%x fail\n", u1Addr);
        }    
    }
    
    // register 0x40~0x50, 4 bytes
    u1Count = 4;
    for (u1Addr=0x40; u1Addr<=0x50; u1Addr++)
    {
        if (_TAS5717_RegReadN(u1Addr, u1Data, u1Count))
        {
            Printf("Read Amp Addr = 0x%x, Data: ", u1Addr);
            for (i=0; i<u1Count; i++)
            {
                Printf("0x%x ",u1Data[i]);
            }
            Printf("\n");
        }
        else
        {
            Printf("Read (Addr) = 0x%x fail\n", u1Addr);
        }    
    }
    
    // register 0x51~0x52, 8 bytes
    u1Count = 8;
    for (u1Addr=0x51; u1Addr<=0x52; u1Addr++)
    {
        if (_TAS5717_RegReadN(u1Addr, u1Data, u1Count))
        {
            Printf("Read Amp Addr = 0x%x, Data: ", u1Addr);
            for (i=0; i<u1Count; i++)
            {
                Printf("0x%x ",u1Data[i]);
            }
            Printf("\n");
        }
        else
        {
            Printf("Read (Addr) = 0x%x fail\n", u1Addr);
        }    
    }
    
    // register 0x53~0x54, 16 bytes
    u1Count = 16;
    for (u1Addr=0x53; u1Addr<=0x54; u1Addr++)
    {
        if (_TAS5717_RegReadN(u1Addr, u1Data, u1Count))
        {
            Printf("Read Amp Addr = 0x%x, Data: ", u1Addr);
            for (i=0; i<u1Count; i++)
            {
                Printf("0x%x ",u1Data[i]);
            }
            Printf("\n");
        }
        else
        {
            Printf("Read (Addr) = 0x%x fail\n", u1Addr);
        }    
    }
    
    // register 0x55     , ??
    
    // register 0x56~0x57, 4 bytes
    u1Count = 4;
    for (u1Addr=0x56; u1Addr<=0x57; u1Addr++)
    {
        if (_TAS5717_RegReadN(u1Addr, u1Data, u1Count))
        {
            Printf("Read Amp Addr = 0x%x, Data: ", u1Addr);
            for (i=0; i<u1Count; i++)
            {
                Printf("0x%x ",u1Data[i]);
            }
            Printf("\n");
        }
        else
        {
            Printf("Read (Addr) = 0x%x fail\n", u1Addr);
        }    
    }
    
    // register 0x58~0x5F, 20 bytes
    u1Count = 20;
    for (u1Addr=0x58; u1Addr<=0x5F; u1Addr++)
    {
        if (_TAS5717_RegReadN(u1Addr, u1Data, u1Count))
        {
            Printf("Read Amp Addr = 0x%x, Data: ", u1Addr);
            for (i=0; i<u1Count; i++)
            {
                Printf("0x%x ",u1Data[i]);
            }
            Printf("\n");
        }
        else
        {
            Printf("Read (Addr) = 0x%x fail\n", u1Addr);
        }    
    }
    
    // register 0x60~0x62, 4 bytes
    u1Count = 4;
    for (u1Addr=0x60; u1Addr<=0x62; u1Addr++)
    {
        if (_TAS5717_RegReadN(u1Addr, u1Data, u1Count))
        {
            Printf("Read Amp Addr = 0x%x, Data: ", u1Addr);
            for (i=0; i<u1Count; i++)
            {
                Printf("0x%x ",u1Data[i]);
            }
            Printf("\n");
        }
        else
        {
            Printf("Read (Addr) = 0x%x fail\n", u1Addr);
        }    
    }
    
    // register 0x63~0x6A, ??
    
    // register 0x6B~0x6C, 4 bytes
    u1Count = 4;
    for (u1Addr=0x6B; u1Addr<=0x6C; u1Addr++)
    {
        if (_TAS5717_RegReadN(u1Addr, u1Data, u1Count))
        {
            Printf("Read Amp Addr = 0x%x, Data: ", u1Addr);
            for (i=0; i<u1Count; i++)
            {
                Printf("0x%x ",u1Data[i]);
            }
            Printf("\n");
        }
        else
        {
            Printf("Read (Addr) = 0x%x fail\n", u1Addr);
        }    
    }
    
    // register 0x6D~0x6F, ??
    
    // register 0x70~0x77, 4 bytes
    u1Count = 4;
    for (u1Addr=0x70; u1Addr<=0x77; u1Addr++)
    {
        if (_TAS5717_RegReadN(u1Addr, u1Data, u1Count))
        {
            Printf("Read Amp Addr = 0x%x, Data: ", u1Addr);
            for (i=0; i<u1Count; i++)
            {
                Printf("0x%x ",u1Data[i]);
            }
            Printf("\n");
        }
        else
{
            Printf("Read (Addr) = 0x%x fail\n", u1Addr);
        }    
}

    // register 0x78~0xF7, ??
    
    // register 0xF8~0xFF, 4 bytes
    u1Count = 4;
    for (u1Addr=0xF8; u1Addr<=0xF9; u1Addr++)
    {
        if (_TAS5717_RegReadN(u1Addr, u1Data, u1Count))
        {
            Printf("Read Amp Addr = 0x%x, Data: ", u1Addr);
            for (i=0; i<u1Count; i++)
{
                Printf("0x%x ",u1Data[i]);
}
            Printf("\n");
        }
        else
        {
            Printf("Read (Addr) = 0x%x fail\n", u1Addr);
        }    
    }
    
    return;

}
#endif
