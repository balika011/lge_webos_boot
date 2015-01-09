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
 * $RCSfile: ostg_if.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

#include "x_lint.h"
LINT_EXT_HEADER_BEGIN
#include "general.h"
#include "drv_video.h"
#include "drv_lvds.h"
#include "panel.h"
#include "x_os.h"
#include "x_assert.h"
#include "x_timer.h"
LINT_EXT_HEADER_END
#include "ostg_if.h"
#include "drv_lvds.h"
#include "drv_display.h"
#include "hw_tcon.h"
#include "drv_tcon.h"
#include "drvcust_if.h"
#include "x_ckgen.h"
#include "drv_pwm.h"
#include "x_gpio.h"
#include "nptv_debug.h"
#ifdef DRV_SUPPORT_EXTMJC
#include "extmjc_if.h"
#endif
#include "drv_vbyone.h"
#include "drv_scpos.h"
#include "drv_scaler.h"
#include "pmx_drvif.h"
#include "mute_if.h"
#include "drv_default.h"

#ifdef CC_SCPOS_3DTV_SUPPORT
BOOL bInitPWM2 = FALSE;
#endif

#ifdef CC_MT5399
#include "drv_mjc.h"
#include "drv_mjc_if.h"
#endif
/*lint -e717*/
/*
e717 : do ... while(0)
*/

//---------------------------------------------------------------------------
// Macro definitions
//---------------------------------------------------------------------------
void vDrvGpioSet(INT32 dwNum, INT32 dwVal)
{
    do
    {
        dwVal = GPIO_Output(dwNum, &dwVal);
    }
    while(0);
}

// backlight power-------------------------------------------------------------
void vDrvTurnOnBacklt(void)
{
    do
    {
        UINT32 gpio, value;

        if(DRVCUST_PanelQuery(eBackLightGpio, &gpio) == 0 &&
           DRVCUST_PanelQuery(eBackLightOnVal, &value) == 0)
        {
            vDrvGpioSet((INT32)gpio, (INT32)value);

            if(DRVCUST_PanelQuery(eBackLightGpio2, &gpio) == 0)
            {
                vDrvGpioSet((INT32)gpio, (INT32)value);
            }

#ifndef CC_MTK_LOADER
            DRVCUST_UpdateBacklightState(BL_STATE_POWERON, 0);
#endif
        }
    }
    while(0);
}

void vDrvTurnOffBacklt(void)
{
    do
    {
        UINT32 gpio, value;

        if(DRVCUST_PanelQuery(eBackLightGpio, &gpio) == 0 &&
           DRVCUST_PanelQuery(eBackLightOnVal, &value) == 0)
        {
            vDrvGpioSet((INT32)gpio, (INT32)!value);

            if(DRVCUST_PanelQuery(eBackLightGpio2, &gpio) == 0)
            {
                vDrvGpioSet((INT32)gpio, (INT32)!value);
            }

#ifndef CC_MTK_LOADER
            DRVCUST_UpdateBacklightState(BL_STATE_POWEROFF, 0);
#endif
        }
    }
    while(0);
}

// PDP VsON------------------------------------------------------------------
void vDrvTurnOnPDPVs(void)
{
    do
    {
        UINT32 gpio, value;

        if(DRVCUST_PanelQuery(ePanelPdpVsGpio, &gpio) == 0)
        {
            value = DRVCUST_PanelGet(ePanelPdpVsOnVal);
            vDrvGpioSet((INT32)gpio, (INT32)value);
        }
    }
    while(0);
}

void vDrvTurnOffPDPVs(void)
{
    do
    {
        UINT32 gpio, value;

        if(DRVCUST_PanelQuery(ePanelPdpVsGpio, &gpio) == 0)
        {
            value = DRVCUST_PanelGet(ePanelPdpVsOnVal);
            vDrvGpioSet((INT32)gpio, (INT32)!value);
        }
    }
    while(0);
}



//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
#define PWM_DUTY_ALL 255
#define PWM_DUTY_LESS 254

#ifdef THE_3RD_PARTY_SW_SUPPORT
#define DEFAULT_DIMMING_FREQUENCY	180
#else
#define DEFAULT_DIMMING_FREQUENCY	1300
#endif
#define DEFAULT_BOOSTING_FREQUENCY	10000

static BOOL _fgPanelOn = FALSE;
static BOOL _fgTCONEPIOn = FALSE;
static BOOL _fgTCONMINIOn = FALSE;

//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------
BOOL	_fgIsSync = (BOOL)SV_OFF;
UINT16	_wFreq    = (UINT16)2169;
UINT16	_wDuty    = (UINT16)128;

/** Set LVDS driving current
 *  @param bDriving: 0~15
 *  @return
 */
void vApiLVDSSetDriving(UINT8 bDriving)
{
    vDrvLVDSSetDriving(bDriving);
}

/**
 * Set Panel output frame rate
 */
void vApiLCDSetFrameRate(UINT16 wFrameRate)
{
    vDrvLCDSetFrame(wFrameRate);
}

UINT8 bApiLCDGetFrameRate(void)
{
    return vDrvGetLCDFreq();
}

//-----------------------------------------------------------------------------
/** event handler before update output frame rate
 *  @param u4FrameRate: output frame rate
 *  @return
 */
//-----------------------------------------------------------------------------
#ifndef SUPPORT_PANEL_SCAN_PWM
UINT32 _u4BrightLevel = 0x80; // 0:dark --> 255:bright
#else
UINT32 _u4BrightLevel = 50;   // 50 means 50%; 0%:dark --> 100%:bright
#endif

//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

//UINT32 BACKLT_GetCurrentPanelBright(void)
//{
//    return _u4BrightLevel;
//}
#ifdef CC_SCPOS_3DTV_SUPPORT
void vApiSetScanningPWM(UINT16 u2HTotal, UINT16 u2VTotal, UINT32 u4Start, UINT32 u4High)
{
    if(DRVCUST_PanelGet(ePanelBacklightPwmPort) == 2)  //pwm2
    {
        vIO32WriteFldAlign(PWM_SCAN_03, u4Start*u2HTotal, REG_PWM_SCAN2_START);
        vIO32WriteFldAlign(PWM_SCAN_04, u4High*u2HTotal, REG_PWM_SCAN2_HIGH);
        vIO32WriteFldAlign(PWM_SCAN_05, (u2VTotal-u4Start-u4High)*u2HTotal, REG_PWM_SCAN2_LOW);
    }
    else //pwm0/1
    {
        vIO32WriteFldAlign(PWM_SCAN_00, u4Start*u2HTotal, REG_PWM_SCAN1_START);
        vIO32WriteFldAlign(PWM_SCAN_01, u4High*u2HTotal, REG_PWM_SCAN1_HIGH);
        vIO32WriteFldAlign(PWM_SCAN_02, (u2VTotal-u4Start-u4High)*u2HTotal, REG_PWM_SCAN1_LOW);
    }

    LOG(1, "Start: %x, High: %x, Low: %x\n", u4Start, u4High, (u2VTotal-u4Start-u4High));
}
#endif

void BACKLT_SetDimming(UINT32 u4Frequency, UINT32 u4Dimming)
{
#ifndef SUPPORT_PANEL_SCAN_PWM
    if(u4Frequency == 0)
    {
        u4Frequency = DEFAULT_DIMMING_FREQUENCY;
    }

    // Invert PWM dimming signal. If pin output with a BJT invert circuit.
    if(DRVCUST_PanelGet(eFlagBacklightDimmingInvert))
    {
        if(u4Dimming >= 256)
        {
            u4Dimming = 0;
        }
        else
        {
            u4Dimming =  256 - u4Dimming;
        }
    }
    else
    {
        if(u4Dimming >= 256)
        {
            u4Dimming = 256;
        }
    }

    if((u4Dimming == 256)||(u4Dimming == 255))
    {
        vDrvSetPWM(
            DRVCUST_PanelGet(ePanelBacklightPwmPort),
            ((BSP_GetDomainClock(SRC_BUS_CLK)) / (PWM_DUTY_LESS+1)) / u4Frequency,
            0xFF,
            PWM_DUTY_LESS);
    }
    else
    {
        vDrvSetPWM(DRVCUST_PanelGet(ePanelBacklightPwmPort),
                   (BSP_GetDomainClock(SRC_BUS_CLK) >> 8) / u4Frequency, u4Dimming, PWM_DUTY_ALL);
    }
#else
	UINT32 u4Devider=1;
	UINT32 u4High=0;
	UINT32 u4Low=0;
	if(u4Frequency == 0)
	{
		u4Frequency = BACKLT_GetDimmingFrequency();
	}
	
	if((vDrvGetLCDFreq() == 25) || (vDrvGetLCDFreq() == 50))
    {
        u4Devider = u4Frequency/50;
    }
	#ifndef CC_MTK_LOADER	
    else if(SUPPORT_PANEL_48HZ && ((vDrvGetLCDFreq() == 24) || (vDrvGetLCDFreq() == 48)) ) 
    {
        u4Devider = u4Frequency/48;
    }
	#endif
    else if ((vDrvGetLCDFreq() == 30) || (vDrvGetLCDFreq() == 60))
    {
        u4Devider = u4Frequency/60;
    }
    else if (vDrvGetLCDFreq() == 100)
    {
	    u4Devider = u4Frequency/100;
    }
    else if (vDrvGetLCDFreq() == 120)
    {
	    u4Devider = u4Frequency/120;
    }

    if ((u4Dimming>0)&&(u4Dimming<1000))
    {
		u4High = u4Dimming/u4Devider;
		u4Low = (1000-u4Dimming)/u4Devider;
    }
    else if (u4Dimming>=1000)
    {
		u4High = 1000;
    }
    LOG(3, "_|-|_ BACKLT_SetDimming freq:%dHz,dev:%d,start(permillage):%d%,high(permillage):%d%,low(permillage):%d%\n", u4Frequency, u4Devider, SCANPWM_START, u4High, u4Low);

	if(!fgIsScanPWMSetDataLatch())
	{
		vDrvSetScanPWMLatchMode(SCAN_PWM_LATCH_MODE_SETDATA_OFF,SCAN_PWM_LATCH_MODE_OFF,SCAN_PWM_ALIGN_MODE_VSYNC);  // Aligned Vsync
	}
		
	// Invert PWM dimming signal
    if(DRVCUST_PanelGet(eFlagBacklightDimmingInvert))
    {
		vDrvSetScanPWMPolarity(DRVCUST_PanelGet(ePanelBacklightPwmPort), 1);
    }
    
    vDrvSetScanPWM(DRVCUST_PanelGet(ePanelBacklightPwmPort),SCANPWM_START,u4High,u4Low);
#ifdef CC_LGE_PROTO_PCBA//shift for 1ms
	if(120 == u4Frequency) //120hz
	vDrvSetScanPWM(DRVCUST_PanelGet(ePanelBacklightPwmPort)+1,SCANPWM_START+62,u4High,u4Low);
	else  //100hz
	vDrvSetScanPWM(DRVCUST_PanelGet(ePanelBacklightPwmPort)+1,SCANPWM_START+50,u4High,u4Low);
#endif

    vDrvScanPWMDataFire();
#endif  // SUPPORT_PANEL_SCAN_PWM
}


UINT32 BACKLT_GetDimmingFrequency(void)
{
    UINT32 u4Freq = 0;

    if((vDrvGetLCDFreq() == 25) || (vDrvGetLCDFreq() == 50) || (vDrvGetLCDFreq() == 100))
    {
        u4Freq = DRVCUST_PanelGet(eDimmingFreq50);

        if(u4Freq == 0)
        {
            u4Freq = PANEL_GetDimmingFrequency50Hz();
        }
    }  
	#ifndef CC_MTK_LOADER	
    else if(SUPPORT_PANEL_48HZ && ((vDrvGetLCDFreq() == 24) || (vDrvGetLCDFreq() == 48) || (vDrvGetLCDFreq() == 96))) 
    {
        u4Freq = PANEL_GetDimmingFrequency48Hz();
    }
	#endif
    else
    {
        u4Freq = DRVCUST_PanelGet(eDimmingFreq60);

        if(u4Freq == 0)
        {
            u4Freq = PANEL_GetDimmingFrequency60Hz();
        }
    }

    if(u4Freq == 0)
    {
        u4Freq = DEFAULT_DIMMING_FREQUENCY;
    }

    return u4Freq;
}

//-----------------------------------------------------------------------------
/** set panel backlight to a brightness level.
 *  @param u1BrightLevel: Brightness level.
 *                        PWM(0:dark --> 255:bright)
 *                        Scan PWM(0%:dark --> 100%:bright)
 *  @return The brightness level
 */
//-----------------------------------------------------------------------------
void vApiSetPanelBright(UINT8 u1BrightLevel)
{
    UINT32 ret = 0;
    ret = (UINT32)DRVCUST_PanelQuery(ePanelDisableDimming, &ret);

    if((DRVCUST_PanelQuery(ePanelDisableDimming, &ret) == 0) && (ret == 1))
    {
        return;
    }

    // if panel is down, just save backlight level
    _u4BrightLevel = (UINT32)u1BrightLevel;

    if(!_fgPanelOn)
    {
        return;
    }

    LOG(3, "_|-|_ vApiSetPanelBright Bright:%d%\n", u1BrightLevel);
    BACKLT_SetDimming(BACKLT_GetDimmingFrequency(), (UINT32)u1BrightLevel);
}

/**
 * @brief Set backlight dimming.
 * @param fgIsSync <SV_ON/SV/OFF>: Synchronize to output v-sync or not.
 * @param bFreq: physical PWM frequency.
 * @param bDUty: duty cycle.
 */
void vApiSetDimming(UINT16 u2Freq, UINT16 u2Duty)
{
    _fgIsSync = (BOOL)SV_ON;
    _wFreq = u2Freq;
    _wDuty = u2Duty; // No need to +1.
    BACKLT_SetDimming(_wFreq, _wDuty);
}

void vApiSetBooster(UINT16 wLevel)
{
    if(!DRVCUST_PanelGet(eFlagBacklightBoostCtrl))
    {
        return;
    }

    // Invert PWM boost signal. If pin output with a BJT invert circuit.
    if(DRVCUST_PanelGet(eFlagBacklightBoostInvert))
    {
        if(wLevel >= 256)
        {
            wLevel = 0;
        }
        else
        {
            wLevel = 256 - wLevel;
        }
    }
    else
    {
        if(wLevel >= 256)
        {
            wLevel = 256;
        }
    }

    // Enable PWM1 for panel backlight boosting.
    IO_WRITE32MSK(IO_ADDR(0xd400), 0, 1 << 28, 1 << 28);

    if((wLevel == 256) || (wLevel == 255))
    {
        vDrvSetPWM(
            SrcPWM1,
            ((BSP_GetDomainClock(SRC_BUS_CLK)) / (PWM_DUTY_LESS+1)) / DEFAULT_BOOSTING_FREQUENCY,
            0xFF,
            PWM_DUTY_LESS);
    }
    else
    {
        vDrvSetPWM(
            SrcPWM1,
            (BSP_GetDomainClock(SRC_BUS_CLK) >> 8) / DEFAULT_BOOSTING_FREQUENCY,
            wLevel,
            PWM_DUTY_ALL);
    }
}


//-----------------------------------------------------------------------------
/** turn on/off panel backlight.
 *  @param fgEnable: Control mode. It could be PANEL_ON or PANEL_OFF
 *  @return
 */
//-----------------------------------------------------------------------------

void vApiBackltONOFF(BOOL fgEnable)
{
    if(fgEnable == PANEL_ON)
    {
        vDrvTurnOnBacklt();
    }
    else
    {
        vDrvTurnOffBacklt();
    }
}


void vApiTCONPower(VIDEO_TCON_POWER_SEQ_T eTconPowSeq, BOOL fgEnable)
{
    if(PANEL_GetDisplayType()== PANEL_DISPLAY_TYPE_EPI)
    {
        if ((fgEnable == TRUE)&&(_fgTCONEPIOn == FALSE)) 
        {
            u1DrvTconInit();                
            _fgTCONEPIOn = TRUE;
        }
        printf("[EPI]vApiTCONEPIPanelPower: %d \n",fgEnable);
        u1DrvEPITgOnOff(eTconPowSeq, fgEnable);
    }	
}

void vApiTCONMINIPower(VIDEO_TCON_MINI_POWER_SEQ_T eTconPowSeq, BOOL fgEnable)
{
    if(PANEL_GetDisplayType()== PANEL_DISPLAY_TYPE_MLVDS)
    {
        if ((fgEnable == TRUE)&&(_fgTCONMINIOn == FALSE)) 
        {
            u1DrvTconInit();                
            _fgTCONMINIOn = TRUE;
        }
        printf("[Mini-LVDS]vApiTCONMINIPanelPower: %d \n",fgEnable);
        vDrvMiniTgOnOff(eTconPowSeq, fgEnable);
    }	
}

//-----------------------------------------------------------------------------
/** turn on/off panel lvds power and backlight.
 *  @param fgEnable: Control mode. It could be PANEL_ON or PANEL_OFF
 *  @return
 */
//-----------------------------------------------------------------------------

#if defined( CC_SUPPORT_4K2K)||defined(CC_SUPPORT_HDMI_4K2K30)
#ifdef CC_MTK_LOADER
#define CC_CMI_MST_FRC_SUPPORT 1 
#define I2C_CMD_LEN 7
#ifdef CC_MT5882
#define I2C_CHN	1
#else
#define I2C_CHN	2
#endif
#define I2C_ADDR 0x38	
void com4k2kSendCmd(UINT8 *pBuf, UINT16 u2Len)
{
    UINT8 u1Checksum = 0, i, *pTmp, u1CmdLen, k ;
    UINT32 iret ;


	pTmp = pBuf ;
	u1CmdLen = I2C_CMD_LEN;
	for(k=0; k < u2Len ; k=k+u1CmdLen)
	{
		u1Checksum = 0;
#ifdef CC_CMI_MST_FRC_SUPPORT		
		{
		    for(  i=1 ; i< u1CmdLen ; i++)
		    {
				//skip data 0
					u1Checksum += pTmp[i] ;
		    }
		}    
#endif		
#ifndef NDEBUG			    
		Printf("\n[4k2k] I2c Data :");
#endif
#ifdef CC_CMI_MST_FRC_SUPPORT
		{
	    	pTmp[0] = (pTmp[0] &0xF) + ((u1Checksum & 0xF) << 4) ; //upper 4 bit ;
	    }	
#endif	    
#ifndef NDEBUG			 
		 for(i=0 ; i < u1CmdLen ; i++)
		 {
			Printf("0x%x ", pTmp[i]);
		 }
		 Printf("\n");
#endif		 
	     iret = SIF_X_Write(I2C_CHN, 0x100 , I2C_ADDR, 0 ,0, pTmp, u1CmdLen);
	     if(iret <=0)
	     {
			Printf("[4k2k] i2c send error\n");

	     }
	     pTmp = pTmp + u1CmdLen ;
	     //command delay
	     x_thread_delay(50);
     }
}

static UINT8 _au1cmiMstFHD60VB1Buf[] = {	    //60Hz
                                            0x40, 0x01, 0x02, 0x00, 0x00, 0x11, 0x00,
                                            0x30, 0x02, 0x00, 0x00, 0x00, 0x01, 0x00,
                                            0x61, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00,
                                            
                                            0x23, 0x1b, 0x10, 0x27, 0xc2, 0xee, 0xd0,  // add delay for CDR training
                                            0x33, 0x1b, 0x10, 0x27, 0x40, 0x71, 0xd0,                                            
                                            0x33, 0x1b, 0x10, 0x27, 0xc3, 0xee, 0xd0, 
                                            
                                            0x81, 0x05, 0x07, 0x80, 0x04, 0x38, 0x00,                                            
                                            0x13, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00,// video mode
                                            0x3D, 0x02, 0x01, 0x00, 0x00, 0x00, 0x00,//color engine bypass
                                            0x33, 0x1c, 0x13, 0x00, 0x04, 0x40, 0x40,//reset
                                            0x33, 0x1c, 0x13, 0x00, 0x04, 0x00, 0x40}; 
                                            
static UINT8 _au1cmiMstFHD120VB1Buf[] = {      //120Hz
                                            0x80, 0x01, 0x02, 0x00, 0x00, 0x55, 0x00,
                                            0x80, 0x02, 0x00, 0x00, 0x02, 0x01, 0x03,                                            
                                            0x61, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00,
                                            
                                            0x23, 0x1b, 0x10, 0x27, 0xc2, 0xee, 0xd0,  // add delay for CDR training
                                            0x33, 0x1b, 0x10, 0x27, 0x40, 0x71, 0xd0,                                            
                                            0x33, 0x1b, 0x10, 0x27, 0xc3, 0xee, 0xd0, 
                                            
                                            0x81, 0x05, 0x07, 0x80, 0x04, 0x38, 0x00,
                                            0x13, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00,  // video mode
                                            0x3D, 0x02, 0x01, 0x00, 0x00, 0x00, 0x00,  //color engine bypass  
                                            0x33, 0x1c, 0x13, 0x00, 0x04, 0x40, 0x40,  //reset
                                            0x33, 0x1c, 0x13, 0x00, 0x04, 0x00, 0x40};
#endif
#endif

#ifdef CC_SUPPORT_BL_DLYTIME_CUT
UINT32 _BIM_GetCurTime(void)
{
   UINT32 u4Time;
   UINT32 u4Val;
   u4Time = BIM_READ32(REG_RW_TIMER2_LOW);
   u4Val = ((~u4Time)/(GET_XTAL_CLK()/1000000));  // us time.
   //Printf("BSP_GetCurTime %d us \n", u4Val);
   return u4Val;
}
#endif /* end of CC_SUPPORT_BL_DLYTIME_CUT */

void vApiPanelPowerSequence(BOOL fgEnable)
{
    PFN_USR_POWER_SEQUENCE pfnUsrPowerSequence = NULL;
    static UINT8 u1PowerSequenceGoing = FALSE;
#if defined(CC_SUPPORT_BL_DLYTIME_CUT)&&defined(CC_MTK_LOADER)
		UINT32 u4DeltaTime = 0xffffffff;
		UINT32 u4BLOnDelay;
		UINT32 gpio, value, temp;
#endif /* end of CC_SUPPORT_BL_DLYTIME_CUT */
#ifndef CC_MTK_LOADER
	static HANDLE_T _hPanelSeqMutex = NULL_HANDLE;
#endif

    //static BOOL b_inited = FALSE;

    //if ( IS_COMPANION_CHIP_ON() && !b_inited )
    //{
    //    _fgPanelOn = TRUE;
    //    b_inited = TRUE;
    //}
#ifndef CC_MTK_LOADER
	if(_hPanelSeqMutex == NULL_HANDLE)
	{
		VERIFY(x_sema_create(&_hPanelSeqMutex, X_SEMA_TYPE_MUTEX, X_SEMA_STATE_UNLOCK) == OSR_OK);
	}
	VERIFY(x_sema_lock(_hPanelSeqMutex , X_SEMA_OPTION_WAIT) == OSR_OK);
#endif

    if(_fgPanelOn == fgEnable)
    {
	#ifndef CC_MTK_LOADER
		VERIFY(x_sema_unlock(_hPanelSeqMutex) == OSR_OK);
	#endif	
        return;
    }

    if(fgEnable == PANEL_ON)  //panel power on sequence
    {
        LOG(0, "vApiPanelPowerSequence= TRUE (START)\n");

        if(bIsPanelOn())
        {
            // already on.
            _fgPanelOn = TRUE;
		#ifndef CC_MTK_LOADER
			VERIFY(x_sema_unlock(_hPanelSeqMutex) == OSR_OK);
		#endif
            return;
        }

        // Check if there is customer-defined power on sequence
        UNUSED(DRVCUST_InitQuery(eCustomerPowerOnSequence,
                                 (UINT32 *)(void *)&(pfnUsrPowerSequence)));

        if(pfnUsrPowerSequence)
        {
            pfnUsrPowerSequence();
            _fgPanelOn = TRUE;
		#ifndef CC_MTK_LOADER
			VERIFY(x_sema_unlock(_hPanelSeqMutex) == OSR_OK);
		#endif
            return;
        }

        // normal power sequence, lvds power -> lvds signal -> backlight power
        vDrvTurnOnPanel();

        // aviod loader and system re-entrant before backlight on
        if(u1PowerSequenceGoing == TRUE)
        {
            LOG(0, "vApiPanelPowerSequence = TRUE (Going)\n");
		#ifndef CC_MTK_LOADER
			VERIFY(x_sema_unlock(_hPanelSeqMutex) == OSR_OK);
		#endif
            return;
        }

        u1PowerSequenceGoing = TRUE;
        /*
        #if defined(CC_MT5368) || defined(CC_MT5389)
           if(GetCurrentPanelIndex()== PANEL_MLVDS_SS_MB7)
            {
             //#ifdef CC_SMALL_TCON_BOARD
                x_thread_delay(100);
               vTconCustTGLoadEEP();
               //  #ifndef CC_MTK_LOADER
               // DRVCUST_EepromLoadODTable();
               // DRVCUST_SetODTable();

               // vHalVideoGammaTblInit();
               //#endif
             //#endif
            }
        #endif
        */
        // the magic 1 makes this delay > 10 * PANEL_GetLvdsOnDalay()
        // otherwise it will less than 10 * PANEL_GetLvdsOnDalay()
        x_thread_delay((10 * PANEL_GetLvdsOnDalay()) + 1);
#ifdef DRV_SUPPORT_EXTMJC

        if(IS_COMPANION_CHIP_ON())
        {
            UINT8 u1MsgErr;
            d_custom_extmjc_set_fastmute(TRUE, &u1MsgErr);
            //drv_extmjc_set_lvds_on_delay(PANEL_GetLvdsOnDalay());
            drv_extmjc_set_lvds_enable(SV_ON, &u1MsgErr);
            d_custom_extmjc_set_fastmute_timeout(0x20,&u1MsgErr);
        }

#endif
#ifdef PANEL_LG_26_WX2

        // special for LG26 PWM, lvds power -> dim -> lvds signal
        if(GetCurrentPanelIndex() == PANEL_LG_26_WX2)
        {
            BACKLT_SetDimming(BACKLT_GetDimmingFrequency(),
                              (IsPwmLowPanelBright() == PWM_HIGH_PANEL_BRIGHT)?100:0);
#ifndef CC_MTK_LOADER
            DRVCUST_UpdateBacklightState(BL_STATE_POWERON, (IsPwmLowPanelBright() == PWM_HIGH_PANEL_BRIGHT)?100:0);
#endif
        }

#endif
        vDrvTurnOnPDPVs();

        switch(PANEL_GetDisplayType())
        {
            default:
            case PANEL_DISPLAY_TYPE_MAX:
                // error display type, defaultly do nothing
                break;

            case PANEL_DISPLAY_TYPE_LVDS:
                vDrvLVDSOff();
                vDrvLVDSConfig();
                vDrvLVDSOn();
                break;

            case PANEL_DISPLAY_TYPE_MLVDS:
                vDrvLVDSConfig();
                vApiTCONMINIPower(VIDEO_TCON_MINISIGNAL,TRUE);
 				vApiTCONMINIPower(VIDEO_TCON_MINI_ALL,TRUE);
                break;
#if defined(CC_MT5399) || defined(CC_MT5882)

            case PANEL_DISPLAY_TYPE_VB1:
                vDrvLVDSOff();
                vDrvLVDSConfig();
                x_thread_delay(1200);
        		vVByOneInit();		
        		vDrvVB1LockCtrl(SV_ON,1);
                vDrvVByOneOn();
                break;
#endif

            case PANEL_DISPLAY_TYPE_EPI:
//                u1DrvTconInit();
#ifndef CC_USE_DDI
                vApiTCONPower(VIDEO_TCON_GST, TRUE);
                vApiTCONPower(VIDEO_TCON_MCLK, TRUE);
                vApiTCONPower(VIDEO_TCON_GCLK, TRUE);
                vApiTCONPower(VIDEO_TCON_EO, TRUE);
                vApiTCONPower(VIDEO_TCON_VCOM_DYN, TRUE);
                vApiTCONPower(VIDEO_TCON_EPISIGNAL, TRUE);
                vApiTCONPower(VIDEO_TCON_ALL, TRUE);
#endif
                break;
        }

#ifdef DRV_SUPPORT_EXTMJC

        if(IS_COMPANION_CHIP_ON())
        {
            UINT8 u1MsgErr;
            drv_extmjc_set_RX_reset(&u1MsgErr);
        }

#endif
        // set pwm before enable backlight power supply, so that backlight would be expected pwm level once we supply the power
    #ifdef SUPPORT_PANEL_SCAN_PWM
		_u4BrightLevel = PANEL_GetScanPWMDuty();
    #endif
	    LOG(3, "_|-|_ vApiPanelPowerSequence Bright:%d%\n", _u4BrightLevel);
        BACKLT_SetDimming(BACKLT_GetDimmingFrequency(), _u4BrightLevel);
#if defined(CC_SUPPORT_BL_DLYTIME_CUT)&&defined(CC_MTK_LOADER)
		if (IS_DISP_4K2K_TYPE2)
		{
			u4DeltaTime = 0;
			Printf("[LVDS][backlight] DeltaTime=%d us-->no need to enable BL_DLYTIME_CUT!!\n", u4DeltaTime);
			BIM_WRITE32((REG_RW_GPRB0 + (4 << 2)), u4DeltaTime);

		#ifndef __MODEL_slt__ // save booting time
        	x_thread_delay(10 * PANEL_GetBacklightOnDelay());
		#endif

        #if defined( CC_SUPPORT_4K2K)||defined(CC_SUPPORT_HDMI_4K2K30)
            if(GetCurrentPanelIndex() == PANEL_CMO_V580DK1_LS1_INX_60_VB1)
		    {	
		        #ifndef CC_MTK_LOADER
		        Printf("[4K2K][VB1]------send FHD_60 CMD #2\n");
		        vSend4k2kFrcPanelTypeCmd(PANEL_4K2K_FHD60);
		        #else
		        Printf("[4K2K][VB1]------send FHD_60 CMD #1\n");
		        com4k2kSendCmd(_au1cmiMstFHD60VB1Buf, sizeof(_au1cmiMstFHD60VB1Buf));
		        #endif
				vDrvVB1LockCtrl(SV_OFF,0);
		    }
		    else if (GetCurrentPanelIndex() == PANEL_CMO_V580DK1_LS1_INX_VB1)
			{
				#ifndef CC_MTK_LOADER
		        Printf("[4K2K][VB1]------send FHD_120 CMD #2\n");
		        vSend4k2kFrcPanelTypeCmd(PANEL_4K2K_FHD120);
		        #else
		        Printf("[4K2K][VB1]------send FHD_120 CMD #1\n");
				com4k2kSendCmd(_au1cmiMstFHD120VB1Buf, sizeof(_au1cmiMstFHD120VB1Buf));
				#endif

				vDrvVB1LockCtrl(SV_OFF,0);
			}
		#endif

			// turn on backlight GPIO, supply backlight power
        	vDrvTurnOnBacklt();
		}
		else
		{
			u4BLOnDelay = 10 * PANEL_GetBacklightOnDelay() * 1000;
			u4DeltaTime = _BIM_GetCurTime() + u4BLOnDelay;
			Printf("[LVDS][backlight] DeltaTime=%d us, BLOnDelay=%d us\n", u4DeltaTime, u4BLOnDelay);
			BIM_WRITE32((REG_RW_GPRB0 + (4 << 2)), u4DeltaTime);
			if(DRVCUST_PanelQuery(eBackLightGpio, &gpio) == 0 &&
				DRVCUST_PanelQuery(eBackLightOnVal, &value) == 0)
			{
				temp = ((UINT16)value << 16)|(UINT16)gpio;
				Printf("[LVDS][backlight] GPIO:%d, value=%d, temp=0x%x\n", gpio, value, temp);
				BIM_WRITE32((REG_RW_GPRB0 + (5 << 2)), temp);
			}
		}
#else        
#ifndef __MODEL_slt__ // save booting time
        x_thread_delay(10 * PANEL_GetBacklightOnDelay());
#endif

#if defined( CC_SUPPORT_4K2K)||defined(CC_SUPPORT_HDMI_4K2K30)
		if(GetCurrentPanelIndex() == PANEL_CMO_V580DK1_LS1_INX_60_VB1)
	    {	
	        #ifndef CC_MTK_LOADER
	        Printf("[4K2K][VB1]------send FHD_60 CMD #2\n");
	        vSend4k2kFrcPanelTypeCmd(PANEL_4K2K_FHD60);
	        #else
	        Printf("[4K2K][VB1]------send FHD_60 CMD #1\n");
	        com4k2kSendCmd(_au1cmiMstFHD60VB1Buf, sizeof(_au1cmiMstFHD60VB1Buf));
	        #endif
			vDrvVB1LockCtrl(SV_OFF,0);
	    }
	    else if (GetCurrentPanelIndex() == PANEL_CMO_V580DK1_LS1_INX_VB1)
		{
			#ifndef CC_MTK_LOADER
	        Printf("[4K2K][VB1]------send FHD_120 CMD #2\n");
	        vSend4k2kFrcPanelTypeCmd(PANEL_4K2K_FHD120);
	        #else
	        Printf("[4K2K][VB1]------send FHD_120 CMD #1\n");
			com4k2kSendCmd(_au1cmiMstFHD120VB1Buf, sizeof(_au1cmiMstFHD120VB1Buf));
			#endif

			vDrvVB1LockCtrl(SV_OFF,0);
		}
#endif

        // turn on backlight GPIO, supply backlight power
        vDrvTurnOnBacklt();
#ifdef PANEL_LG_26_WX2

        if(GetCurrentPanelIndex() == PANEL_LG_26_WX2)
        {
            // keep 100% duty at least 1000ms after backlight on,
            x_thread_delay(1000);
        }

#endif
#endif /* end of CC_SUPPORT_BL_DLYTIME_CUT */
        _fgPanelOn = TRUE;
        // after panel on, we could set backlight
#if 0

        if(DRVCUST_InitGet(eFlagControlBacklightPWM))
        {
            vApiSetPanelBright(_u4BrightLevel);
        }

#endif
#ifndef CC_MTK_LOADER
        DRVCUST_UpdateBacklightState(BL_STATE_POWERON, _u4BrightLevel);
#endif

#if 0// don't set pwm lock, otherwise, the frequency can'be changed. [DTV00612343]
        if(DRVCUST_PanelGet(eFlagBacklightDimmingLock))
        {
            vDrvSetLock(DRVCUST_PanelGet(ePanelBacklightPwmPort), SV_ON);
        }
        else
        {
            vDrvSetLock(DRVCUST_PanelGet(ePanelBacklightPwmPort), SV_OFF);
        }
#endif

        u1PowerSequenceGoing = FALSE;		
		LOG(0, "vApiPanelPowerSequence= TRUE (END)\n");		
    }
    else // panel power off sequence
    {
        // Check if there is customer-defined power on sequence
        UNUSED(DRVCUST_InitQuery(eCustomerPowerOffSequence,
                                 (UINT32 *)(void *)&(pfnUsrPowerSequence)));

        if(pfnUsrPowerSequence)
        {
            pfnUsrPowerSequence();
            _fgPanelOn = FALSE;
		#ifndef CC_MTK_LOADER
			VERIFY(x_sema_unlock(_hPanelSeqMutex) == OSR_OK);
		#endif			
            return;
        }

        // backlight power -> lvds sugnal -> lvds power
        vDrvTurnOffBacklt(); // Backlight off
        x_thread_delay(10 * PANEL_GetBacklightOffDelay());
#ifdef DRV_SUPPORT_EXTMJC

        if(IS_COMPANION_CHIP_ON())
        {
            UINT8 u1MsgErr;
            drv_extmjc_set_lvds_enable(SV_OFF, &u1MsgErr);
        }

#endif

        switch(PANEL_GetDisplayType())
        {
            default:
            case PANEL_DISPLAY_TYPE_MAX:
                // error display type, defaultly do nothing
                break;

            case PANEL_DISPLAY_TYPE_LVDS:
                vDrvLVDSOff();
                break;

            case PANEL_DISPLAY_TYPE_MLVDS:
				vApiTCONMINIPower(VIDEO_TCON_MINISIGNAL,FALSE);
 				vApiTCONMINIPower(VIDEO_TCON_MINI_ALL,FALSE);
                break;
		#if defined(CC_MT5399) || defined(CC_MT5882)
            case PANEL_DISPLAY_TYPE_VB1:
				vVByOneAnalogOffSetting();
                break;
		#endif
            case PANEL_DISPLAY_TYPE_EPI:
				#ifndef CC_USE_DDI
                   vApiTCONPower(VIDEO_TCON_GST, FALSE);
                   vApiTCONPower(VIDEO_TCON_MCLK, FALSE);
                   vApiTCONPower(VIDEO_TCON_GCLK, FALSE);
                   vApiTCONPower(VIDEO_TCON_EO, FALSE);
                   vApiTCONPower(VIDEO_TCON_VCOM_DYN, FALSE);
                   vApiTCONPower(VIDEO_TCON_EPISIGNAL, FALSE);
                   vApiTCONPower(VIDEO_TCON_ALL, FALSE);
                #endif
                break;

        }

        vDrvTurnOffPDPVs();
        x_thread_delay(10 * PANEL_GetLvdsOffDalay());
#ifdef PANEL_LG_26_WX2

        // special for LG26 PWM
        if(GetCurrentPanelIndex() == PANEL_LG_26_WX2)
        {
            BACKLT_SetDimming(BACKLT_GetDimmingFrequency(),
                              (IsPwmLowPanelBright() == PWM_HIGH_PANEL_BRIGHT)? 0 : 100);
#ifndef CC_MTK_LOADER
            DRVCUST_UpdateBacklightState(BL_STATE_POWERON, (IsPwmLowPanelBright() == PWM_HIGH_PANEL_BRIGHT)? 0 : 100);
#endif
        }

#endif
        vDrvTurnOffPanel(); // Panel off
        _fgPanelOn = FALSE;
    }
	
#ifndef CC_MTK_LOADER
	VERIFY(x_sema_unlock(_hPanelSeqMutex) == OSR_OK);
#endif			
}

#ifdef CC_SUPPORT_STR
extern void vDrvTCONIntrDisable(void);
void vDrvLVDSSuspend(void)
{
	vApiPanelPowerSequence(FALSE);
	vDrvTCONIntrDisable();
}

extern void vDrvTCONInitISR(void);
void vDrvLVDSResume(void)
{
	vApiPanelPowerSequence(TRUE);
	vDrvTCONInitISR();
}
#endif

void BeforeOutputFrameRateChange(UINT32 u4FrameRate)
{
    LOG(7, "BeforeOutputFrameRateChange ->%d\n", u4FrameRate);
    // some panel will crash when changing frame rate,
    // so we reset panel this moment to prevent it
    /*
        if (WFB_PANEL_RESET == WFB_PANEL_RESET_ON)
        {
            vApiPanelPowerSequence(FALSE);
        }
    */

    // update dimming frequency
    if(DRVCUST_InitGet(eFlagControlBacklightPWM))
    {
        vApiSetPanelBright(_u4BrightLevel);
    }

#ifndef CC_MTK_LOADER
    DRVCUST_UpdateBacklightState(BL_STATE_OUTPUT_FR_CHANGE, _u4BrightLevel);
#endif
}


//-----------------------------------------------------------------------------
/** event handler after update output frame rate
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------

void AfterOutputFrameRateChange(void)
{
}

//-----------------------------------------------------------------------------
/** check if panel is already lighted on
 *  @param none
 *  @return 0 if not light yet, otherwise already lighted on
 */
//-----------------------------------------------------------------------------
UINT8 bIsPanelOn(void)
{
    UINT32 gpio, value;

    if(DRVCUST_PanelQuery(eBackLightGpio, &gpio) == 0 &&
       DRVCUST_PanelQuery(eBackLightOnVal, &value) == 0)
    {
        if((GPIO_Enable(gpio, NULL) == 1) && (GPIO_Output(gpio, NULL) == value))
        {
            LOG(0, "[LVDS]panel backlight power on!\n");
            return 1;
        }
    }
#if (!defined( CC_SUPPORT_4K2K))&&(!defined(CC_SUPPORT_HDMI_4K2K30))  
    else if(DRVCUST_PanelQuery(ePanelPowerGpio, &gpio) == 0 &&
            DRVCUST_PanelQuery(ePanelPowerOnVal, &value) == 0)
    {
        if((GPIO_Enable(gpio, NULL) == 1) && (GPIO_Output(gpio, NULL) == value))
        {
            LOG(0, "[LVDS]LVDS power on!\n");
            return 1;
        }
    }
#endif
    if(_fgPanelOn != FALSE)
    {
        return 1;
    }

    return 0;
}

#ifdef CC_FAST_INIT
void ostg_set_fgPanelOn(BOOL onoff)
{
    _fgPanelOn = onoff;
}
#endif

void vDrvOstgHWInit_LOADER(void)
{

#ifdef OS_OPTION_POS
	vIO32WriteFldAlign(OSTG_01, OS_OPTION_POS, REG_OP_SEL);
#else
	vIO32WriteFldAlign(OSTG_01, 0, REG_OP_SEL);
#endif // #ifdef OS_OPTION_POS    
}

#include "hw_ospe.h"

void LDR_PmxDisplayHWInit(void)
{

#ifndef CC_MTK_LOADER  
	vDrvVideoMuteColor(SV_VP_MAIN,0);
    _vDrvVideoSetMute(MUTE_MODULE_MODECHG, SV_VP_MAIN, FOREVER_MUTE, FALSE);
    _vDrvVideoSetMute(MUTE_MODULE_MODECHG, SV_VP_PIP , FOREVER_MUTE, FALSE);
#endif  

#ifdef CC_MT5399        
    // MJC mute for  red screen issue when panel on    
    vIO32WriteFldAlign(MUTE_04, 1, R_MUTE_MJC_EN) ;
#endif

#ifndef CC_MT5399        
    // For non-MJC nodel, need to set 0 at loader to prevent logo display shift
    vIO32WriteFldAlign(OSTG_00, 0, C_BYPASS_TTD);
#endif

#ifdef CC_MT5399        
	//Selet OSTG input data path from MJC
	vIO32WriteFldAlign(OSTG_01, 1, REG_MJC_IN_EN);
#endif


	vScpipHwInitOnPanelInit(SV_VP_MAIN);
	vScpipHwInitOnPanelInit(SV_VP_PIP);
	vApiPanelPowerSequence(FALSE);
	vDrvLVDSInit();
	vErrorHandleInit();
	vDrvDisplayInit();
	vDrvOstgHWInit_LOADER();	
	vApiPanelPowerSequence(TRUE);
}

#if 1//LOADER_MULTIPLE_LOGO_PARTITION || LOADER_LOGO_NUMBER
static const UINT32 g_au4PlaneArray[5] = { PMX_OSD2, PMX_OSD3, PMX_OSD1,PMX_PIP,PMX_MAIN};
#else
static const UINT32 g_au4PlaneArray[4] = { PMX_OSD2, PMX_MAX_INPORT_NS, PMX_MAX_INPORT_NS, PMX_MAX_INPORT_NS };
#endif

//-----------------------------------------------------------------------------
/** LDR_PmxDisplay(): Setup Panel Pmx/LVDS driver.
 *  This function is going to setup panel pmx and lvds driver with background
 *  color.
 *
 *  @param  u4Background    This is background color setting. 0xRRGGBB.
 *  @retval 0               Success.
 *  @retval otherwise       Failed.
 */
//-----------------------------------------------------------------------------
UINT32 LDR_PmxDisplay(UINT32 u4Background)
{
    static UINT32 _fgInit = 0;
    UINT32 i;

    if(_fgInit)
    {
        PMX_SetBg(u4Background, FALSE);
        return 0;
    }

    _fgInit = 1;

    PMX_Init();
    LoadPanelIndex();
#ifdef CC_USE_DDI
    PANEL_SetDisplayType(LCD_DISPLAY);
#endif
#ifdef CC_MT5399        
#ifdef CC_MTK_LOADER 	  
	vIO32WriteFldAlign(MUTE_06, ((u4Background >> 16) & 0xFF) << 2, R_MUTE_MAIN_MJC);
	vIO32WriteFldAlign(MUTE_05, ((u4Background >>  8) & 0xFF) << 2, G_MUTE_MAIN_MJC);
	vIO32WriteFldAlign(MUTE_05, ((u4Background >>  0) & 0xFF) << 2, B_MUTE_MAIN_MJC);
	vIO32WriteFldAlign(MUTE_04, 1, R_MUTE_MJC_EN) ; 	
#endif			
#endif

    // set background color
    PMX_SetBg(u4Background, FALSE);
    // remove vdp plane
    PMX_SetPlaneOrderArray(g_au4PlaneArray);

    for(i=0; i<=PMX_UPDATE_DELAY; i++)
    {
        // Use OutputVSync to update PlaneOrder.
        PMX_OnOutputVSync();
    }
	LDR_PmxDisplayHWInit();


    return 0;
}
