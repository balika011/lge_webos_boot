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

#include "adac_sta381.h"
#include "sif_if.h"  // for i2c
#include "aud_if.h"
#include "aud_debug.h"
#include "drvcust_if.h"
#include "aud_cfg.h"
#include "aud_dsp_cfg.h"
#include "x_assert.h"

LINT_EXT_HEADER_END

//---------------------------------------------------------------------------
// Configurations
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Constant definitions
//---------------------------------------------------------------------------
#define SAMPLE_RATE_NUM         3
#define SOURCE_NUM              3
    #define SOURCE_FROM_AV      0
    #define SOURCE_FROM_ATV     1
    #define SOURCE_FROM_DTV     2
#define EQ_BIQUAD_NUM           4
#define HL_BIQUAD_NUM           2

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

static void _STA381_RegWrite(UINT8 u1Addr, UINT8 u1Data);
static UINT8 _STA381_RegRead(UINT8 u1Addr);
static void _STA381_Init(void);
static void _STA381_Mute(BOOL fgEnable);
static void _STA381_DacFmtCfg(DATA_FORMAT_T eFormat, MCLK_FREQUENCY_T eMclk);
void _STA381_RegWriteEx(UINT8 u1Addr, UINT8 u1Data, AUD_AMP_ID_T u1AmpID);
UINT8 _STA381_RegReadEx(UINT8 u1Addr, AUD_AMP_ID_T u1AmpID);
static void _STA381_DisableEffect (void);

void ADAC_STA381_SetDualDRC(AUD_AMP_ID_T u1AmpID, AUD_AQ_AMP_DRC_T eDrcData);

//---------------------------------------------------------------------------
// Static variables
//---------------------------------------------------------------------------
static BOOL fgSTA381Mute=FALSE;

static AMP_DRV_CB_T _rAmpSTA381DrvCB = 
{
    _STA381_DisableEffect // pfnDisableEffect, disable sound effect   
};

static AMP_FUNCTBL_T _rAmpFunTbl = 
{
	_STA381_Init,							//pfnInit
	_STA381_Mute,							//pfnMute 
	_STA381_DacFmtCfg,  			   //pfnSetDacInFmt;
	NULL,						 	     		   //pfnSetDacGain;
	_STA381_RegWrite, 					//pfnWrite;
	_STA381_RegRead,					//pfnRead;
	&_rAmpSTA381DrvCB                          // pfnDrvCB;
};

static AMP_T _rAmpSTA381 = 
{
	"STA381",				// szName[32];
	256,							// i2MaxGain
	-256,						// i2MaxGain
	2,								// u1DacNum
	&_rAmpFunTbl		// rCodecFuncTbl
};
#define AMP_NUM 3
static UINT8 u1MasterVolume[AMP_NUM] = {0x00, 0x00, 0x00};
static HANDLE_T _hSemaSta381MVol = NULL_HANDLE;
static HANDLE_T _hSemaSta381PEQ = NULL_HANDLE;

//---------------------------------------------------------------------------
// Static functions
//---------------------------------------------------------------------------

static void _STA381_RegWrite(UINT8 u1Addr, UINT8 u1Data)
{
    UINT32 u4WriteCount = 0; 

    u4WriteCount = SIF_X_Write(AUD_AMP_BUS_ID, SIF_CLK_DIV, STA381BW_ADDR, 1 ,u1Addr, &u1Data, 1);

    LOG( 9,"I2C write (address,data) = (0x %x,0x %x)\n",u1Addr, u1Data);

    if (u4WriteCount <= 0)
    {
        LOG(0, "STA381 WRITE FAIL (address,data) = (0x %x,0x %x)\n", u1Addr, u1Data);
    }	
}

static UINT8 _STA381_RegRead(UINT8 u1Addr) 
{
    UINT32 u4ReadCount = 0;
    UINT8 u1Data;

    u4ReadCount = SIF_X_Read(AUD_AMP_BUS_ID, SIF_CLK_DIV, STA381BW_ADDR, 1 , u1Addr, &u1Data, 1);
    LOG( 9,"I2C read (address,data) = (0x%x, 0x%x)\n",u1Addr, u1Data);

    if (u4ReadCount > 0)
    {
        return u1Data;
    }	
    else
    {
        LOG(0, "STA381 READ FAIL address = 0x%x\n", u1Addr);
        return 0;
    }	
}

static void _STA381_Init(void)
{
    AOUT_CFG_T*		prAudOutFmt;
    UINT32 u4reset,u4polarity;
    AUD_AQ_AMP_DRC_T eDrcData;
    AUD_AQ_EXTPEQ_DATA_T rPEQData;
    AUD_AQ_AMP_RAM_DATA_T rAmpData;

    LOG(0, " Start _STA381_Init...\n");
    // Make sure I2c is init
    SIF_Init(); 
    
    UNUSED(DRVCUST_OptQuery(eAudioDigiAMPResetGpio, &u4reset));
    UNUSED(DRVCUST_OptQuery(eAudioDigiAMPResetGpioPolarity, &u4polarity));
    if (u4reset != 0xffffffff)
    {
        u4polarity = !u4polarity;
        AUD_GPIOOutput(u4reset, (INT32)u4polarity);    //digital ampliferT
        x_thread_delay(50);
        u4polarity = !u4polarity;
        AUD_GPIOOutput(u4reset, (INT32)u4polarity);    //digital ampliferT
        LOG(0, " GPIO(%d) = %d\n", u4reset, u4polarity);
    }
    
    if (_hSemaSta381MVol == NULL_HANDLE)
    {
        VERIFY(x_sema_create(&_hSemaSta381MVol, X_SEMA_TYPE_BINARY,
                                        X_SEMA_STATE_UNLOCK) == OSR_OK);
    }
    if (_hSemaSta381PEQ == NULL_HANDLE)
    {
        VERIFY(x_sema_create(&_hSemaSta381PEQ, X_SEMA_TYPE_BINARY,
                                        X_SEMA_STATE_UNLOCK) == OSR_OK);        
    }    
    //Set as ST map(STA339BWS compatible map)
    _STA381_RegWrite(STA381BW_MISC4, 0x00);            // 00-> ST map, 80->SEC map

    //Initial flow for power on sequence 
    _STA381_RegWrite(STA381BW_MASTER_VOL_CRL, 0xFE);//0x00 for hard mute 
    _STA381_RegWrite(STA381BW_CH1CONFIG, 0x00); 
    _STA381_RegWrite(STA381BW_CH2CONFIG, 0x40); 
    _STA381_RegWrite(STA381BW_CH3CONFIG, 0x80); 

    _STA381_RegWrite(STA381BW_CXT41, 0x00); 
    _STA381_RegWrite(STA381BW_CXT75, 0x00); 
    _STA381_RegWrite(STA381BW_CONFD, 0x00); 
    //support 2 channel	
    _STA381_RegWrite(STA381BW_CONFF, 0xdc);
    _STA381_RegWrite(STA381BW_MASMUTE, 0x40); 

    // Format config
    UNUSED(DRVCUST_OptQuery(eAudioOutFmtTable, (UINT32 *)(void *)&prAudOutFmt));
    _STA381_RegWrite(STA381BW_CONFA, 0x63); // 48K and 256fs
    _STA381_DacFmtCfg(prAudOutFmt->eFormat, (MCLK_FREQUENCY_T)DRVCUST_OptGet(eAudioDacMclk));
    _STA381_RegWrite(STA381BW_CONFC, 0xc2); //change default for HDMI & DTV 48Khz& 44.1Khz sasa background noise
			
			
   // Volume setup
    _STA381_RegWrite(STA381BW_MASTER_VOL_CRL, 0x00);	//Oliver 0x20 -> 0x00 /*MASTER_VOL_0DB=0x00*/

    _STA381_RegWrite(STA381BW_CH1_VOL_CRL, 0x3f);
    _STA381_RegWrite(STA381BW_CH2_VOL_CRL, 0x3f);
    _STA381_RegWrite(STA381BW_CH3_VOL_CRL, 0x22);     
    _STA381_RegWrite(STA381BW_CH1CONFIG, 0x11); 
    _STA381_RegWrite(STA381BW_CH2CONFIG, 0x61); 
    
    _STA381_RegWrite(STA381BW_LIMITER, 0x6f); 
    _STA381_RegWrite(STA381BW_LIMITER_THRESHOLD, 0x6f); 
    
    //HP/Line-out related
    _STA381_RegWrite(STA381BW_HPCFG, 0x88);
    _STA381_RegWrite(STA381BW_F3XCFG2, 0x2e);
    _STA381_RegWrite(STA381BW_MAGIC1, 0x10);

    //DualBand DRC Init for Wistron
    eDrcData.u1Cutoff = 0;
    eDrcData.u1LfeGain = 0x22;
    eDrcData.u1AttackHi = 0xb8;
    eDrcData.u1ReleaseHi = 0xb8;
    eDrcData.u1RateHi = 0x8;
    eDrcData.u1AttackLo = 0xb8;
    eDrcData.u1ReleaseLo = 0xb8;
    eDrcData.u1RateLo = 0x8;
    ADAC_STA381_SetDualDRC(AUD_AMP_ID_LR, eDrcData); 
    
    rPEQData.u4Data[0] = 0x90659c;
    rPEQData.u4Data[1] = 0x63da95;
    rPEQData.u4Data[2] = 0x6f9a64;
    rPEQData.u4Data[3] = 0x98eb83;
    rPEQData.u4Data[4] = 0x419cf4;
    
    rPEQData.u4Data[5] = 0x80fd95;
    rPEQData.u4Data[6] = 0x7e03d4;
    rPEQData.u4Data[7] = 0x7f026b;
    rPEQData.u4Data[8] = 0x81def4;
    rPEQData.u4Data[9] = 0x400e9c;

    rPEQData.u4Data[10] = 0x856407;
    rPEQData.u4Data[11] = 0x75e2ca;
    rPEQData.u4Data[12] = 0x7a9bf9;
    rPEQData.u4Data[13] = 0x8911e2;
    rPEQData.u4Data[14] = 0x4085aa;

    rPEQData.u4Data[15] = 0x9e7181;
    rPEQData.u4Data[16] = 0x569ff8;
    rPEQData.u4Data[17] = 0x618e7f;
    rPEQData.u4Data[18] = 0xa2c36e;
    rPEQData.u4Data[19] = 0x434e4d;  

    rPEQData.u4Data[20] = 0xb606ca;
    rPEQData.u4Data[21] = 0x3fb632;
    rPEQData.u4Data[22] = 0x49f936;
    rPEQData.u4Data[23] = 0xb54b91;
    rPEQData.u4Data[24] = 0x457f1f;    
    
    rPEQData.u4Data[25] = 0x8059d1;
    rPEQData.u4Data[26] = 0x7f19a4;
    rPEQData.u4Data[27] = 0x7fa62f;
    rPEQData.u4Data[28] = 0x80b23c;
    rPEQData.u4Data[29] = 0x401a10; 
    
    rPEQData.u4Data[30] = 0x806b0e;
    rPEQData.u4Data[31] = 0x7f94f2;
    rPEQData.u4Data[32] = 0x7f94c6;
    rPEQData.u4Data[33] = 0x80d5c4;
    rPEQData.u4Data[34] = 0x3fca79; 
    
    ADAC_STA381_SetPEQ(AUD_AMP_ID_LR, &rPEQData);     
    
    rAmpData.u1Data[0] = 0;
    rAmpData.u1Data[1] = 0;
    rAmpData.u1Data[2] = 0;    
    ADAC_STA381_WriteRam(AUD_AMP_ID_LR,0,0x74,rAmpData, FALSE); 
    
}

static void _STA381_MVOL_CTRL(AUD_AMP_ID_T u1AmpID)
{
    if (u1AmpID == AUD_AMP_ID_CSw)
    {
        //if ((!fgApogeeMute) && (!fgApogeeSwMute))
        //{
        //    _APOGEE_RegWriteEx(REG_APOGEE_MASTER_VOL_CRL, u1MasterVolume[u1AmpID] , u1AmpID);    
        //}   
    }
    else if (u1AmpID == AUD_AMP_ID_LR)
    {
        if (!fgSTA381Mute)
        {
            _STA381_RegWriteEx(STA381BW_MASTER_VOL_CRL, u1MasterVolume[AUD_AMP_ID_LR] , AUD_AMP_ID_LR);
        }
    }
}

static void _STA381_Mute(BOOL fgEnable)
{
    UNUSED(fgSTA381Mute);
    if (_hSemaSta381MVol == NULL_HANDLE)
    {
        VERIFY(x_sema_create(&_hSemaSta381MVol, X_SEMA_TYPE_BINARY,
                                        X_SEMA_STATE_UNLOCK) == OSR_OK);
    }        
    VERIFY(x_sema_lock(_hSemaSta381MVol, X_SEMA_OPTION_WAIT) == OSR_OK);           
    fgSTA381Mute = fgEnable;    
    
    if (fgEnable)
    {
       //DSP_AoutWaitMute();
       // Don't use DSP_AoutWaitMute, delay for too long.
       // use the pseudo fade out only takes 10ms delay.       
        _STA381_RegWriteEx(STA381BW_MASTER_VOL_CRL, 0xFE, (AUD_AMP_ID_T)AUD_AMP_ID_LR);
    }
    else
    {        
        _STA381_MVOL_CTRL(AUD_AMP_ID_LR);
    }
    VERIFY(x_sema_unlock(_hSemaSta381MVol) == OSR_OK);
}

static void _STA381_DacFmtCfg(DATA_FORMAT_T eFormat, MCLK_FREQUENCY_T eMclk)
{
    UNUSED(eMclk);
    
    switch (eFormat)
    {
        case FORMAT_LJ:
        	_STA381_RegWrite(STA381BW_CONFB, 0x81);
        	break;
        case FORMAT_I2S:
        	_STA381_RegWrite(STA381BW_CONFB, 0x80);
        	break;
        case FORMAT_RJ:
        	_STA381_RegWrite(STA381BW_CONFB, 0x82);
        	break;
        default:
        	_STA381_RegWrite(STA381BW_CONFB, 0x81);
        	break;
    }
}

static void _STA381_DisableEffect (void)
{
    // FIX ME!!!
    // Please add disable sound effect commands in this function.
    // Let the freq. response the same after amp.
}

AMP_T* _AMP_GetAmpSTA381FunTbl(void)
{
	return &_rAmpSTA381;
}

void _STA381_RegWriteEx(UINT8 u1Addr, UINT8 u1Data, AUD_AMP_ID_T u1AmpID)
{
    UINT32 u4WriteCount = 0;
    UINT8 u1BusID;
    UINT8 u1DevID;

    UNUSED(u4WriteCount);
    UNUSED(u1BusID);
    UNUSED(u1DevID);    
    // BUS2: 0x3A
    // BUS0: 0x3A Ls/Rs
    // BUS0: 0x38 Center:8, Sub:9    
    switch (u1AmpID)
    {
    case AUD_AMP_ID_LR:
        u1BusID = 0;
        u1DevID = STA381BW_ADDR;             
        u4WriteCount = SIF_X_Write(u1BusID, SIF_CLK_DIV, u1DevID, 1 ,u1Addr, &u1Data, 1);          
        break;        
#ifdef CC_AUD_AMP_STA333BW_SA_1
    case AUD_AMP_ID_LsRs:
        u1BusID = 0;
        u1DevID = APOGEE_STA333BW_ADDR;
        u4WriteCount = SIF_X_Write(u1BusID, SIF_CLK_DIV, u1DevID, 1 ,u1Addr, &u1Data, 1);
        break;        
    case AUD_AMP_ID_CSw:
        u1BusID = 0;
        u1DevID = APOGEE_STA333BW_ADDR_1;
        u4WriteCount = SIF_X_Write(u1BusID, SIF_CLK_DIV, u1DevID, 1 ,u1Addr, &u1Data, 1);
        break;        
#endif        
    default:
        break;
    };
}

UINT8 _STA381_RegReadEx(UINT8 u1Addr, AUD_AMP_ID_T u1AmpID) 
{
    UINT32 u4ReadCount = 0;
    UINT8 u1Data;
    UINT8 u1BusID;
    UINT8 u1DevID;

    UNUSED(u1BusID);
    UNUSED(u1DevID);
    UNUSED(u4ReadCount);
    UNUSED(u1Data);

    switch (u1AmpID)
    {
    case AUD_AMP_ID_LR:
        u1BusID = 0;
        u1DevID = STA381BW_ADDR;          
        break;        
#ifdef CC_AUD_AMP_STA333BW_SA_1
    case AUD_AMP_ID_LsRs:
        u1BusID = 0;
        u1DevID = APOGEE_STA333BW_ADDR;
        break;        
    case AUD_AMP_ID_CSw:
        u1BusID = 0;
        u1DevID = APOGEE_STA333BW_ADDR_1;
        break;        
#endif
    default:
        u1BusID = 0;
        u1DevID = STA381BW_ADDR;  
        break;
    };

    u4ReadCount = SIF_X_Read(u1BusID, SIF_CLK_DIV, u1DevID, 1 , u1Addr, &u1Data, 1);
    if (u4ReadCount > 0)
    {
    	return u1Data;
    }	
    else
    {
        LOG(5, "STA381 READ FAIL\n");
    	return 0;
    }
}


void ADAC_STA381_SetDualDRC(AUD_AMP_ID_T u1AmpID, AUD_AQ_AMP_DRC_T eDrcData)    
{
    UINT8 u1Temp;

    //CufOff Reg: 0xC,  Bit[7:4]
    u1Temp = _STA381_RegReadEx(STA381BW_AUTOMODE,u1AmpID);
    u1Temp = (u1Temp & 0xF) | (eDrcData.u1Cutoff << 4);
    _STA381_RegWriteEx(STA381BW_AUTOMODE, u1Temp, u1AmpID); 

    //Low Freq Gain Reg: 0xA,  Bit[7:0]
    _STA381_RegWriteEx(STA381BW_CH3_VOL_CRL, (eDrcData.u1LfeGain), u1AmpID);

    //Attack Hi Reg: 0x32,  Bit[6:0]
    u1Temp = _STA381_RegReadEx(STA339_ATTACK1_THRESHOLD, u1AmpID);
    u1Temp = (u1Temp & 0x80) | (eDrcData.u1AttackHi);
    _STA381_RegWriteEx(STA339_ATTACK1_THRESHOLD, u1Temp, u1AmpID); 
    //Attack Lo Reg: 0x34,  Bit[6:0]
    u1Temp = _STA381_RegReadEx(STA339_ATTACK2_THRESHOLD, u1AmpID);
    u1Temp = (u1Temp & 0x80) | (eDrcData.u1AttackLo);
    _STA381_RegWriteEx(STA339_ATTACK2_THRESHOLD, u1Temp, u1AmpID);

    //Release Hi Reg: 0x33,  Bit[6:0]
    u1Temp = _STA381_RegReadEx(STA339_RELEASE1_THRESHOLD, u1AmpID);
    u1Temp = (u1Temp & 0x80) | (eDrcData.u1ReleaseHi);
    _STA381_RegWriteEx(STA339_RELEASE1_THRESHOLD, u1Temp, u1AmpID);
    //Release Lo Reg: 0x35,  Bit[6:0]
    u1Temp = _STA381_RegReadEx(STA339_RELEASE2_THRESHOLD, u1AmpID);
    u1Temp = (u1Temp & 0x80) | (eDrcData.u1ReleaseLo);
    _STA381_RegWriteEx(STA339_RELEASE2_THRESHOLD, u1Temp, u1AmpID);

    //Rate Hi Reg: 0x12,  Bit[7:4]
    u1Temp = _STA381_RegReadEx(STA339_ATTACK_RATE1,u1AmpID);
    u1Temp = (u1Temp & 0xF) | (eDrcData.u1RateHi << 4);
    _STA381_RegWriteEx(STA339_ATTACK_RATE1, u1Temp, u1AmpID);
    //Rate Lo Reg: 0x14,  Bit[7:4]
    u1Temp = _STA381_RegReadEx(STA339_ATTACK_RATE2,u1AmpID);
    u1Temp = (u1Temp & 0xF) | (eDrcData.u1RateLo << 4);
    _STA381_RegWriteEx(STA339_ATTACK_RATE2, u1Temp, u1AmpID);    
}

void ADAC_STA381_SetPEQ(AUD_AMP_ID_T u1AmpID, AUD_AQ_EXTPEQ_DATA_T *rAQExtPeqData)
{
    UINT8 u1Temp;
    AUD_AQ_AMP_RAM_DATA_T rAmpData;
    UINT8 i,j;

    UNUSED(u1AmpID);
    UNUSED(rAQExtPeqData);
    UNUSED(ADAC_STA381_SetPEQ);    

    // Set 1 to bit4 in reg 0x3
    u1Temp = _STA381_RegReadEx(STA381BW_CONFD, u1AmpID);     
    u1Temp |= 0x10;
    _STA381_RegWriteEx(STA381BW_CONFD, u1Temp, u1AmpID);

    // Write Coeffs to Ram. (Band 0 ~ 7 , 5 coeffs each.)
    for (i=0; i<7; i++) // 7 Bands
    {
        for (j=0; j<15; j+=3) //For one band, there are 5 coeffs, each are 3 byte.
        {
            rAmpData.u1Data[j] = ((rAQExtPeqData ->u4Data[i*5+j/3]) & 0xFF0000) >> 16;
            rAmpData.u1Data[j+1] = ((rAQExtPeqData ->u4Data[i*5+j/3]) & 0xFF00)>> 8;
            rAmpData.u1Data[j+2] = ((rAQExtPeqData ->u4Data[i*5+j/3]) & 0xFF);
            LOG(10,"EXT_PEQ - rAmpData.u1Data[%d] = 0x%x\n", j,rAmpData.u1Data[j]);
            LOG(10,"EXT_PEQ - rAmpData.u1Data[%d] = 0x%x\n", j+1,rAmpData.u1Data[j+1]);
            LOG(10,"EXT_PEQ - rAmpData.u1Data[%d] = 0x%x\n", j+2,rAmpData.u1Data[j+2]);
        }
        ADAC_STA381_WriteRam(u1AmpID, 0, (i*5), rAmpData, TRUE);
        LOG(10,"EXT_PEQ - Write PEQ to band %d, Start Addr %d\n", i, i*5);
    }
}

BOOL ADAC_STA381_WriteRam(AUD_AMP_ID_T u1AmpID, UINT8 u1Bank, UINT8 u1Addr, AUD_AQ_AMP_RAM_DATA_T rAmpRamData, BOOL fgAllSet)
{
    UINT8 u1Temp;
        
    VERIFY(x_sema_lock(_hSemaSta381PEQ, X_SEMA_OPTION_WAIT) == OSR_OK);    
    _STA381_RegWriteEx(STA339_EQDRC_CFG, u1Bank, u1AmpID); // Set EQ Cfg, select Bank.
    _STA381_RegWriteEx(STA339_CFADDR, u1Addr, u1AmpID);    // Write Addr to reg 0x16

    _STA381_RegWriteEx(STA339_B1CF1, rAmpRamData.u1Data[0], u1AmpID);     //0x17 : Top 8 bits of Data.
    _STA381_RegWriteEx(STA339_B1CF2, rAmpRamData.u1Data[1], u1AmpID);     //0x18 : Mid 8 bits of Data.
    _STA381_RegWriteEx(STA339_B1CF3, rAmpRamData.u1Data[2], u1AmpID);     //0x19 : Last 8 bits of Data.

    if (fgAllSet)        
    {
        _STA381_RegWriteEx(STA339_B2CF1, rAmpRamData.u1Data[3], u1AmpID); 
        _STA381_RegWriteEx(STA339_B2CF2, rAmpRamData.u1Data[4], u1AmpID); 
        _STA381_RegWriteEx(STA339_B2CF3, rAmpRamData.u1Data[5], u1AmpID); 

        _STA381_RegWriteEx(STA339_A1CF1, rAmpRamData.u1Data[6], u1AmpID); 
        _STA381_RegWriteEx(STA339_A1CF2, rAmpRamData.u1Data[7], u1AmpID); 
        _STA381_RegWriteEx(STA339_A1CF3, rAmpRamData.u1Data[8], u1AmpID); 

        _STA381_RegWriteEx(STA339_A2CF1, rAmpRamData.u1Data[9], u1AmpID); 
        _STA381_RegWriteEx(STA339_A2CF2, rAmpRamData.u1Data[10], u1AmpID); 
        _STA381_RegWriteEx(STA339_A2CF3, rAmpRamData.u1Data[11], u1AmpID); 

        _STA381_RegWriteEx(STA339_B0CF1, rAmpRamData.u1Data[12], u1AmpID); 
        _STA381_RegWriteEx(STA339_B0CF2, rAmpRamData.u1Data[13], u1AmpID); 
        _STA381_RegWriteEx(STA339_B0CF3, rAmpRamData.u1Data[14], u1AmpID);              
    }

    u1Temp = _STA381_RegReadEx(STA339_CFUD, u1AmpID);      // Write 1 ro W1 bit in reg 0x26
    if (fgAllSet)
    {
        u1Temp |= 0x2; //bit 1 means write all.
    }
    else
    {
        u1Temp |= 0x1; //bit 0 means write one.
    }
    _STA381_RegWriteEx(STA339_CFUD, u1Temp, u1AmpID);
    VERIFY(x_sema_unlock(_hSemaSta381PEQ) == OSR_OK);

    return TRUE;
}


#if 0   // Unused
static UINT8 u1RegConfdBak = 0;
static UINT8 u1RegCH3VolBak = 0; 
static BOOL fgSta381Reset = FALSE; 

BOOL ADAC_STA381_ReadRam(AUD_AMP_ID_T u1AmpID, UINT8 u1Bank, UINT8 u1Addr, UINT8* u1Data, BOOL fgAllSet)
{
    UINT8 u1Temp;
        
    VERIFY(x_sema_lock(_hSemaSta381PEQ, X_SEMA_OPTION_WAIT) == OSR_OK);    
    _STA381_RegWriteEx(STA339_EQDRC_CFG, u1Bank, u1AmpID); // Set EQ Cfg, select Bank.
    _STA381_RegWriteEx(STA339_CFADDR, u1Addr, u1AmpID);    // Write Addr to reg 0x16
    u1Temp = _STA381_RegReadEx(STA339_CFUD, u1AmpID);      // Write 1 ro R1 bit in reg 0x26
    u1Temp |= 0x4;
    _STA381_RegWriteEx(STA339_CFUD, u1Temp, u1AmpID);

    u1Temp = _STA381_RegReadEx(STA339_B1CF1, u1AmpID);     //0x17 : Top 8 bits of Data.
    u1Data[0] = u1Temp;
    u1Temp = _STA381_RegReadEx(STA339_B1CF2, u1AmpID);     //0x18 : Mid 8 bits of Data.
    u1Data[1] = u1Temp;
    u1Temp = _STA381_RegReadEx(STA339_B1CF3, u1AmpID);     //0x19 : Last 8 bits of Data.
    u1Data[2] = u1Temp;    

    if (fgAllSet)        
    {
        u1Temp = _STA381_RegReadEx(STA339_B2CF1, u1AmpID);     
        u1Data[3] = u1Temp;
        u1Temp = _STA381_RegReadEx(STA339_B2CF2, u1AmpID);     
        u1Data[4] = u1Temp;
        u1Temp = _STA381_RegReadEx(STA339_B2CF3, u1AmpID);     
        u1Data[5] = u1Temp;

        u1Temp = _STA381_RegReadEx(STA339_A1CF1, u1AmpID);     
        u1Data[6] = u1Temp;
        u1Temp = _STA381_RegReadEx(STA339_A1CF2, u1AmpID);     
        u1Data[7] = u1Temp;
        u1Temp = _STA381_RegReadEx(STA339_A1CF3, u1AmpID);     
        u1Data[8] = u1Temp;

        u1Temp = _STA381_RegReadEx(STA339_A2CF1, u1AmpID);     
        u1Data[9] = u1Temp;
        u1Temp = _STA381_RegReadEx(STA339_A2CF2, u1AmpID);     
        u1Data[10] = u1Temp;
        u1Temp = _STA381_RegReadEx(STA339_A2CF3, u1AmpID);     
        u1Data[11] = u1Temp;        

        u1Temp = _STA381_RegReadEx(STA339_B0CF1, u1AmpID);     
        u1Data[12] = u1Temp;
        u1Temp = _STA381_RegReadEx(STA339_B0CF2, u1AmpID);     
        u1Data[13] = u1Temp;
        u1Temp = _STA381_RegReadEx(STA339_B0CF3, u1AmpID);     
        u1Data[14] = u1Temp;                
    }

    VERIFY(x_sema_unlock(_hSemaSta381PEQ) == OSR_OK);   
    return TRUE;
}

void ADAC_STA381_SetChannelVol(AUD_AMP_ID_T u1AmpID, UINT8 u1ChId, UINT8 u1Value)
{
    switch (u1ChId)
    {
    case 0: //Master volume
        VERIFY(x_sema_lock(_hSemaSta381MVol, X_SEMA_OPTION_WAIT) == OSR_OK);    
        u1MasterVolume[u1AmpID] = u1Value;
        _STA381_MVOL_CTRL(u1AmpID);
        VERIFY(x_sema_unlock(_hSemaSta381MVol) == OSR_OK);        
        //_STA381_RegWriteEx(STA381BW_MASTER_VOL_CRL, u1Value, u1AmpID);
        break;
    case 1: //Channel 1
        _STA381_RegWriteEx(STA381BW_CH1_VOL_CRL, u1Value, u1AmpID);    
        break;        
    case 2: //Channel 2
        _STA381_RegWriteEx(STA381BW_CH2_VOL_CRL, u1Value, u1AmpID);
        break;
    default:
        LOG(0,"Incorrect Channel volume control of STA381, u1ChId = %d!!\n", u1ChId);
        break;    
    };
}

void ADAC_STA381_SetPostScale(AUD_AMP_ID_T u1AmpID, UINT32 u4PostSclCh1, UINT32 u4PostSclCh2)
{
    AUD_AQ_AMP_RAM_DATA_T rAmpData;
    // set -PostScale to Cope with AMP output phase inverse problem.
    UINT32 u4PostSclCh1_NEG;
    UINT32 u4PostSclCh2_NEG;
    u4PostSclCh1_NEG = ~u4PostSclCh1;
    u4PostSclCh2_NEG = ~u4PostSclCh2;
    UNUSED(ADAC_STA381_SetPostScale);        
    UNUSED(u4PostSclCh1);
    UNUSED(u4PostSclCh2);    
    UNUSED(u4PostSclCh1_NEG);
    UNUSED(u4PostSclCh2_NEG);    
        
    rAmpData.u1Data[0] = (u4PostSclCh1_NEG & 0xFF0000)>> 16;
    rAmpData.u1Data[1] = (u4PostSclCh1_NEG & 0xFF00)>> 8;
    rAmpData.u1Data[2] = (u4PostSclCh1_NEG & 0xFF);    
    ADAC_STA381_WriteRam(u1AmpID,0,0x34,rAmpData, FALSE);

    rAmpData.u1Data[0] = (u4PostSclCh2_NEG & 0xFF0000)>> 16;
    rAmpData.u1Data[1] = (u4PostSclCh2_NEG & 0xFF00)>> 8;
    rAmpData.u1Data[2] = (u4PostSclCh2_NEG & 0xFF);     
    ADAC_STA381_WriteRam(u1AmpID,0,0x35,rAmpData, FALSE);
}

void ADAC_STA381_SetSTCCoeff(AUD_AMP_ID_T u1AmpID, AUD_AQ_AMP_STC_COEFF_T *rAQAmpStcCoeff)
{
    AUD_AQ_AMP_RAM_DATA_T rAmpData;

    UNUSED(u1AmpID);
    
    // band 0, DRC 0
    rAmpData.u1Data[0] = (rAQAmpStcCoeff->u1ReleaseRate0Hi);
    rAmpData.u1Data[1] = (rAQAmpStcCoeff->u1ReleaseRate0Mid);
    rAmpData.u1Data[2] = (rAQAmpStcCoeff->u1ReleaseRate0Lo);    
    ADAC_STA381_WriteRam(u1AmpID,0,0x54,rAmpData, FALSE);
    
    rAmpData.u1Data[0] = (rAQAmpStcCoeff->u1AttackRate0Hi);
    rAmpData.u1Data[1] = (rAQAmpStcCoeff->u1AttackRate0Mid);
    rAmpData.u1Data[2] = (rAQAmpStcCoeff->u1AttackRate0Lo);    
    ADAC_STA381_WriteRam(u1AmpID,0,0x55,rAmpData, FALSE);    
    
    rAmpData.u1Data[0] = (rAQAmpStcCoeff->u1LimiterTH0Hi);
    rAmpData.u1Data[1] = (rAQAmpStcCoeff->u1LimiterTH0Mid);
    rAmpData.u1Data[2] = (rAQAmpStcCoeff->u1LimiterTH0Lo);    
    ADAC_STA381_WriteRam(u1AmpID,0,0x56,rAmpData, FALSE);     
    
    rAmpData.u1Data[0] = (rAQAmpStcCoeff->u1CompressorRate0Hi);
    rAmpData.u1Data[1] = (rAQAmpStcCoeff->u1CompressorRate0Mid);
    rAmpData.u1Data[2] = (rAQAmpStcCoeff->u1CompressorRate0Lo);    
    ADAC_STA381_WriteRam(u1AmpID,0,0x57,rAmpData, FALSE);             

    rAmpData.u1Data[0] = (rAQAmpStcCoeff->u1CompressorTH0Hi);
    rAmpData.u1Data[1] = (rAQAmpStcCoeff->u1CompressorTH0Mid);
    rAmpData.u1Data[2] = (rAQAmpStcCoeff->u1CompressorTH0Lo);    
    ADAC_STA381_WriteRam(u1AmpID,0,0x58,rAmpData, FALSE);    
    
    // band 0, DRC 2
    rAmpData.u1Data[0] = (rAQAmpStcCoeff->u1ReleaseRate1Hi);
    rAmpData.u1Data[1] = (rAQAmpStcCoeff->u1ReleaseRate1Mid);
    rAmpData.u1Data[2] = (rAQAmpStcCoeff->u1ReleaseRate1Lo);    
    ADAC_STA381_WriteRam(u1AmpID,0,0x5e,rAmpData, FALSE);
    
    rAmpData.u1Data[0] = (rAQAmpStcCoeff->u1AttackRate1Hi);
    rAmpData.u1Data[1] = (rAQAmpStcCoeff->u1AttackRate1Mid);
    rAmpData.u1Data[2] = (rAQAmpStcCoeff->u1AttackRate1Lo);    
    ADAC_STA381_WriteRam(u1AmpID,0,0x5f,rAmpData, FALSE);    
    
    rAmpData.u1Data[0] = (rAQAmpStcCoeff->u1LimiterTH1Hi);
    rAmpData.u1Data[1] = (rAQAmpStcCoeff->u1LimiterTH1Mid);
    rAmpData.u1Data[2] = (rAQAmpStcCoeff->u1LimiterTH1Lo);    
    ADAC_STA381_WriteRam(u1AmpID,0,0x60,rAmpData, FALSE);     
    
    rAmpData.u1Data[0] = (rAQAmpStcCoeff->u1CompressorRate1Hi);
    rAmpData.u1Data[1] = (rAQAmpStcCoeff->u1CompressorRate1Mid);
    rAmpData.u1Data[2] = (rAQAmpStcCoeff->u1CompressorRate1Lo);    
    ADAC_STA381_WriteRam(u1AmpID,0,0x61,rAmpData, FALSE);             

    rAmpData.u1Data[0] = (rAQAmpStcCoeff->u1CompressorTH1Hi);
    rAmpData.u1Data[1] = (rAQAmpStcCoeff->u1CompressorTH1Mid);
    rAmpData.u1Data[2] = (rAQAmpStcCoeff->u1CompressorTH1Lo);    
    ADAC_STA381_WriteRam(u1AmpID,0,0x62,rAmpData, FALSE);  
    
    // band 1, DRC 1
    rAmpData.u1Data[0] = (rAQAmpStcCoeff->u1ReleaseRate2Hi);
    rAmpData.u1Data[1] = (rAQAmpStcCoeff->u1ReleaseRate2Mid);
    rAmpData.u1Data[2] = (rAQAmpStcCoeff->u1ReleaseRate2Lo);    
    ADAC_STA381_WriteRam(u1AmpID,0,0x59,rAmpData, FALSE);
    
    rAmpData.u1Data[0] = (rAQAmpStcCoeff->u1AttackRate2Hi);
    rAmpData.u1Data[1] = (rAQAmpStcCoeff->u1AttackRate2Mid);
    rAmpData.u1Data[2] = (rAQAmpStcCoeff->u1AttackRate2Lo);    
    ADAC_STA381_WriteRam(u1AmpID,0,0x5a,rAmpData, FALSE);    
    
    rAmpData.u1Data[0] = (rAQAmpStcCoeff->u1LimiterTH2Hi);
    rAmpData.u1Data[1] = (rAQAmpStcCoeff->u1LimiterTH2Mid);
    rAmpData.u1Data[2] = (rAQAmpStcCoeff->u1LimiterTH2Lo);    
    ADAC_STA381_WriteRam(u1AmpID,0,0x5b,rAmpData, FALSE);     
    
    rAmpData.u1Data[0] = (rAQAmpStcCoeff->u1CompressorRate2Hi);
    rAmpData.u1Data[1] = (rAQAmpStcCoeff->u1CompressorRate2Mid);
    rAmpData.u1Data[2] = (rAQAmpStcCoeff->u1CompressorRate2Lo);    
    ADAC_STA381_WriteRam(u1AmpID,0,0x5c,rAmpData, FALSE);             

    rAmpData.u1Data[0] = (rAQAmpStcCoeff->u1CompressorTH2Hi);
    rAmpData.u1Data[1] = (rAQAmpStcCoeff->u1CompressorTH2Mid);
    rAmpData.u1Data[2] = (rAQAmpStcCoeff->u1CompressorTH2Lo);    
    ADAC_STA381_WriteRam(u1AmpID,0,0x5d,rAmpData, FALSE);    
    
    // band 1, DRC 3
    rAmpData.u1Data[0] = (rAQAmpStcCoeff->u1ReleaseRate3Hi);
    rAmpData.u1Data[1] = (rAQAmpStcCoeff->u1ReleaseRate3Mid);
    rAmpData.u1Data[2] = (rAQAmpStcCoeff->u1ReleaseRate3Lo);    
    ADAC_STA381_WriteRam(u1AmpID,0,0x63,rAmpData, FALSE);
    
    rAmpData.u1Data[0] = (rAQAmpStcCoeff->u1AttackRate3Hi);
    rAmpData.u1Data[1] = (rAQAmpStcCoeff->u1AttackRate3Mid);
    rAmpData.u1Data[2] = (rAQAmpStcCoeff->u1AttackRate3Lo);    
    ADAC_STA381_WriteRam(u1AmpID,0,0x64,rAmpData, FALSE);    
    
    rAmpData.u1Data[0] = (rAQAmpStcCoeff->u1LimiterTH3Hi);
    rAmpData.u1Data[1] = (rAQAmpStcCoeff->u1LimiterTH3Mid);
    rAmpData.u1Data[2] = (rAQAmpStcCoeff->u1LimiterTH3Lo);    
    ADAC_STA381_WriteRam(u1AmpID,0,0x65,rAmpData, FALSE);     
    
    rAmpData.u1Data[0] = (rAQAmpStcCoeff->u1CompressorRate3Hi);
    rAmpData.u1Data[1] = (rAQAmpStcCoeff->u1CompressorRate3Mid);
    rAmpData.u1Data[2] = (rAQAmpStcCoeff->u1CompressorRate3Lo);    
    ADAC_STA381_WriteRam(u1AmpID,0,0x66,rAmpData, FALSE);             

    rAmpData.u1Data[0] = (rAQAmpStcCoeff->u1CompressorTH3Hi);
    rAmpData.u1Data[1] = (rAQAmpStcCoeff->u1CompressorTH3Mid);
    rAmpData.u1Data[2] = (rAQAmpStcCoeff->u1CompressorTH3Lo);    
    ADAC_STA381_WriteRam(u1AmpID,0,0x67,rAmpData, FALSE);
    
    // offset DRC0~3      
    rAmpData.u1Data[0] = (rAQAmpStcCoeff->u1OffsetDRC0Hi);
    rAmpData.u1Data[1] = (rAQAmpStcCoeff->u1OffsetDRC0Mid);
    rAmpData.u1Data[2] = 0;    
    ADAC_STA381_WriteRam(u1AmpID,0,0x68,rAmpData, FALSE);     
    
    rAmpData.u1Data[0] = (rAQAmpStcCoeff->u1OffsetDRC1Hi);
    rAmpData.u1Data[1] = (rAQAmpStcCoeff->u1OffsetDRC1Mid);
    rAmpData.u1Data[2] = 0;    
    ADAC_STA381_WriteRam(u1AmpID,0,0x69,rAmpData, FALSE);    
    
    rAmpData.u1Data[0] = (rAQAmpStcCoeff->u1OffsetDRC2Hi);
    rAmpData.u1Data[1] = (rAQAmpStcCoeff->u1OffsetDRC2Mid);
    rAmpData.u1Data[2] = 0;    
    ADAC_STA381_WriteRam(u1AmpID,0,0x6a,rAmpData, FALSE);    
    
    rAmpData.u1Data[0] = (rAQAmpStcCoeff->u1OffsetDRC3Hi);
    rAmpData.u1Data[1] = (rAQAmpStcCoeff->u1OffsetDRC3Mid);
    rAmpData.u1Data[2] = 0;    
    ADAC_STA381_WriteRam(u1AmpID,0,0x6b,rAmpData, FALSE); 
    
    _STA381_RegWrite(STA381BW_STCCFG0, rAQAmpStcCoeff->u1STCCFG0);  
    _STA381_RegWrite(STA381BW_STCCFG1, rAQAmpStcCoeff->u1STCCFG1);
    //2 reserved bytes, no used  
    
    //Dynamic Attack Rate    
    rAmpData.u1Data[0] = (rAQAmpStcCoeff->u1DynamicAttackRateHi);
    rAmpData.u1Data[1] = (rAQAmpStcCoeff->u1DynamicAttackRateMid);
    rAmpData.u1Data[2] = (rAQAmpStcCoeff->u1DynamicAttackRateLo);    
    ADAC_STA381_WriteRam(u1AmpID,0,0x71,rAmpData, FALSE);   
    
    return;  
}    


void ADAC_STA381_SetSTCFilter(AUD_AMP_ID_T u1AmpID, AUD_AQ_EXTPEQ_DATA_T *rAQExtPeqData)
{
    AUD_AQ_AMP_RAM_DATA_T rAmpData;
    UINT8 i,j;
    
    UNUSED(u1AmpID);
    UNUSED(rAQExtPeqData);
    UNUSED(ADAC_STA381_SetSTCFilter);    
    // Write Coeffs to Ram. (filter 0 ~ 3 , 5 coeffs each.)
    for (i=0; i<4; i++) // 4 filters
    {
        for (j=0; j<15; j+=3) //For one band, there are 5 coeffs, each are 3 byte.
        {
            rAmpData.u1Data[j] = ((rAQExtPeqData ->u4Data[i*5+j/3]) & 0xFF0000) >> 16;
            rAmpData.u1Data[j+1] = ((rAQExtPeqData ->u4Data[i*5+j/3]) & 0xFF00)>> 8;
            rAmpData.u1Data[j+2] = ((rAQExtPeqData ->u4Data[i*5+j/3]) & 0xFF);
            LOG(10,"STC_Filter - rAmpData.u1Data[%d] = 0x%x\n", j,rAmpData.u1Data[j]);
            LOG(10,"STC_Filter - rAmpData.u1Data[%d] = 0x%x\n", j+1,rAmpData.u1Data[j+1]);
            LOG(10,"STC_Filter - rAmpData.u1Data[%d] = 0x%x\n", j+2,rAmpData.u1Data[j+2]);
        }
        ADAC_STA381_WriteRam(u1AmpID, 0, 0x40+(i*5), rAmpData, TRUE);
        LOG(10,"STC_Filter - Write STC_Filter[%d], Start Addr %d\n", i, 0x40+i*5);
    }        
}

void ADAC_STA381_Reset(void)
{
    if (!fgSta381Reset)
    {
        u1RegConfdBak = _STA381_RegReadEx(STA381BW_CONFD, AUD_AMP_ID_LR);
        u1RegCH3VolBak = _STA381_RegReadEx(STA381BW_CH3_VOL_CRL, AUD_AMP_ID_LR);
        _STA381_RegWriteEx(0x6e, 0x04, AUD_AMP_ID_LR);
        fgSta381Reset = TRUE;
        LOG(1, "[AC DETECT]Reset STA381\n");    
    }
}

void ADAC_STA381_Recover(void)
{

    if (fgSta381Reset)
    {
        fgSta381Reset = FALSE; 
        _STA381_Init();
        _STA381_Mute(fgSTA381Mute);
        _STA381_RegWriteEx(STA381BW_CONFD, u1RegConfdBak,AUD_AMP_ID_LR);
        _STA381_RegWriteEx(STA381BW_CH3_VOL_CRL, u1RegCH3VolBak, AUD_AMP_ID_LR);    
#ifdef CC_AUD_USE_FLASH_AQ    
        _AUD_FlashAQPeriphRecover();
#endif
        LOG(1, "[AC_DETECT]Recover STA381 settings\n");
    }
}
#endif
