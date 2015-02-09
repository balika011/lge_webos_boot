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
 * $Author: p4admin $
 * $Date  $
 * $RCSfile: drv_pwm.c,v $
 * $Revision: #4 $
 *
 *---------------------------------------------------------------------------*/


//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "drv_pwm.h"
#include "x_hal_5381.h"
#include "x_pinmux.h"
#include "x_assert.h"
#include "nptv_debug.h"
#include "drvcust_if.h"
#include "drv_tdtv_drvif.h"
#include "hw_tcon.h"
#include "hw_mlvds.h"
#include "drv_display.h"
#include "sv_const.h"

//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
EXTERN UINT8 bSI_DISPLAY_DCLK_TYPE;
UINT8 _bPWMSrc;
extern BOOL fgIsVsyncIsrStart;
static UINT8 u1FlagSetPwm =0;


#define PWMSCAN_MAX_CH SrcPWM2
typedef struct
{
	UINT8 u1Initialed;	
	UINT32 u4Start;        // permillage
	UINT32 u4High;         // permillage
	UINT32 u4Low;          // permillage
	UINT32 u4High_target;  // permillage
	UINT32 u4Low_target;   // permillage 
	UINT32 u4LineTotalPre;
	UINT8 u1CrossZone;	
}SCANPWM_INFO_T;

static SCANPWM_INFO_T PWMSCAN_SETDATA[PWMSCAN_MAX_CH+1];

#define SCANPWM_DEBUGLOG 0
UINT8 _bScanPWMDbgLogLev=3;

UINT8 fgApplyScanPWMSetData=0;
UINT8 u1ScanPwmCrossFlg=0;
UINT8 fgScanPWMSetpControl=0;
UINT16 u2ScanPWMSetp=2;

BOOL _fgScanPWMAuto;
UINT8 _u1ScanPWMTestChg;
UINT32 _u4ScanPWMTesFreq;
INT32 _i4ScanPWMTestStart;
UINT32 _u4ScanPWMTestHigh;
UINT32 _u4ScanPWMTestLow;
UINT32 _u4ScanPWMTestBottom;
UINT32 _u4ScanPWMTestTop;
UINT8 _u1ScanPWMTestSrc;

static DRV_PWM_PARAM_T _gPWMSetting[PWM_DEV_MAX];

void vDrvPWM_Init(void)
{
	memset((void *)_gPWMSetting,0,sizeof(_gPWMSetting));
	u1FlagSetPwm=0;
}
void vDrvPWM_ApplyParam(DRV_PWM_PIN_SEL_T pwmIndex)
{
	UINT32 u4BusClk;
	UINT32 u4PwmP,u4PwmH,u4PwmRsn,u4Frequency;
	if (u1FlagSetPwm==0) return;
	
	u4BusClk = BSP_GetDomainClock(SRC_BUS_CLK);
	u4PwmRsn = 0xff;
	if(_gPWMSetting[pwmIndex].pwm_enable && (!_gPWMSetting[pwmIndex].pwm_scanning_enable))//OPWM
	{
		if(_gPWMSetting[pwmIndex].pwm_adapt_freq_param.pwm_adapt_freq_enable)
		{
			switch(vDrvGetLCDFreq())
		    {
		        case 24:
		        case 48:
		        case 96:
		            u4Frequency = _gPWMSetting[pwmIndex].pwm_adapt_freq_param.pwmfreq_48nHz;
		            break;
		        case 25:
		        case 50:
		        case 100:
		            u4Frequency = _gPWMSetting[pwmIndex].pwm_adapt_freq_param.pwmfreq_50nHz;
		            break;
		        case 30:
		        case 60:
		        case 120:
		            u4Frequency = _gPWMSetting[pwmIndex].pwm_adapt_freq_param.pwmfreq_60nHz;
					break;
		        default:
		        	Printf("Error , get wrong LCD freq : %d .\n",vDrvGetLCDFreq());
		        	break;
			}
		}
		else
		{
			u4Frequency = _gPWMSetting[pwmIndex].pwm_frequency;
		}
		LOG(0, "_|-|_  opwm[%d] freq:%dHz,duty:%d, lock:%d, adaptive:%d\n",pwmIndex, u4Frequency, _gPWMSetting[pwmIndex].pwm_duty,_gPWMSetting[pwmIndex].pwm_lock,_gPWMSetting[pwmIndex].pwm_adapt_freq_param.pwm_adapt_freq_enable);
		if(u4Frequency == 0)
		{
			u4PwmP = 0;
			u4PwmH = 0;
		}
		else
		{
		    u4PwmP = ((u4BusClk >> 8)/ u4Frequency);
			u4PwmH = _gPWMSetting[pwmIndex].pwm_duty;
		}
		vDrvSetPWM((SrcPWM0 + pwmIndex),u4PwmP,u4PwmH,u4PwmRsn);
		vDrvSetLock((SrcPWM0 + pwmIndex), _gPWMSetting[pwmIndex].pwm_lock);
	}
	else if(_gPWMSetting[pwmIndex].pwm_enable && (_gPWMSetting[pwmIndex].pwm_scanning_enable))
	{
	UINT32 u4Devider=1;
	UINT32 u4High=0;
	UINT32 u4Low=0;
	UINT32 u4Port=0;
	UINT32 u4Start=0;
	
	if(_gPWMSetting[pwmIndex].pwm_duty>255)_gPWMSetting[pwmIndex].pwm_duty=255;
	if(_gPWMSetting[pwmIndex].pwm_pos_start>255)_gPWMSetting[pwmIndex].pwm_pos_start=255;
	
	u4Start=_gPWMSetting[pwmIndex].pwm_pos_start*1000/255;
	u4High=_gPWMSetting[pwmIndex].pwm_duty*1000/255;
	u4Low = 1000-u4High;
	u4Port = pwmIndex+1;

	switch(vDrvGetLCDFreq())
		    {
		        case 24:
		        case 48:
		            u4Frequency = _gPWMSetting[pwmIndex].pwm_adapt_freq_param.pwmfreq_48nHz;
					u4Devider=u4Frequency/48;
		            break;
		        case 25:
		        case 50:
		            u4Frequency = _gPWMSetting[pwmIndex].pwm_adapt_freq_param.pwmfreq_50nHz;
					u4Devider=u4Frequency/50;
		            break;
		        case 30:
		        case 60:
		            u4Frequency = _gPWMSetting[pwmIndex].pwm_adapt_freq_param.pwmfreq_60nHz;
					u4Devider=u4Frequency/60;
					break;
				case 96:
					u4Frequency = _gPWMSetting[pwmIndex].pwm_adapt_freq_param.pwmfreq_48nHz;
					u4Devider=u4Frequency/96;
		            break;
				case 100:
					u4Frequency = _gPWMSetting[pwmIndex].pwm_adapt_freq_param.pwmfreq_50nHz;
					u4Devider=u4Frequency/100;
		            break;
				case 120:
					u4Frequency = _gPWMSetting[pwmIndex].pwm_adapt_freq_param.pwmfreq_60nHz;
					u4Devider=u4Frequency/120;
					break;
		        default:
		        	Printf("Error , get wrong LCD freq : %d .\n",vDrvGetLCDFreq());
		        	break;
			}
    LOG(0, "_|-|_  scanpwm[%d] freq:%dHz,dev:%d,start(permillage):%d%,high(permillage):%d%,low(permillage):%d%\n",u4Port, u4Frequency, u4Devider, u4Start, u4High, u4Low);

   if(u4Devider==0)
		 u4Devider=1; 
   u4Start=u4Start/u4Devider;
   u4High=u4High/u4Devider;
   u4Low=u4Low/u4Devider;

	if(!fgIsScanPWMSetDataLatch())
	{
		vDrvSetScanPWMLatchMode(SCAN_PWM_LATCH_MODE_SETDATA_OFF,SCAN_PWM_LATCH_MODE_OFF,SCAN_PWM_ALIGN_MODE_VSYNC);  // Aligned Vsync
	}
  
    vDrvSetScanPWM(u4Port,u4Start,u4High,u4Low);

    vDrvScanPWMDataFire();
	}
	else
	{
		GPIO_SetOut(79+pwmIndex, 0);

	}
	
}
void vDrvPWM_SetParam(DRV_PWM_PIN_SEL_T pwmIndex,DRV_PWM_PARAM_T *prPwmSetting)
{
	 u1FlagSetPwm = 1;  

	 memcpy(&_gPWMSetting[pwmIndex], prPwmSetting, sizeof(DRV_PWM_PARAM_T));
	 vDrvPWM_ApplyParam(pwmIndex);
}
void vDrvPWM_ApplyParamSet()
{
	INT32 i;
	if (u1FlagSetPwm==0) return;                                                        ;
	for(i=1;i<=SrcPWM2;i++)
	{	if(_gPWMSetting[i].pwm_enable && (_gPWMSetting[i].pwm_scanning_enable))
			vDrvSetScanPWMLatchMode(SCAN_PWM_LATCH_MODE_SETDATA,SCAN_PWM_LATCH_MODE_OFF,SCAN_PWM_ALIGN_MODE_VSYNC);  // Set data
		vDrvPWM_ApplyParam(PWM_DEV_PIN0+i);
	}
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
/** Brief
 * This driver control the PWM0, PWM1 & PWM2 output frequence and duty cycle
 *
 * duty cycle = bDutyON/(bDutyAll+1);
 * PWM freq.(in 5368/96...) = (BUS Clock) / ((PWMP+1)x(PWMRSN+1))
 *
 * @param bSrc      PWM source (SrcPWM0/1/2)
 * @param bTimeBase PWMP, PWM basic unit = (0~0xfffff) * BUS Clock
 * @param bDutyON   PWMH, ON duty width = (0~0xfff) * bTimeBase
 * @param bDutyAll  PWMRSN, Period width = (0~0xffe) * bTimeBase
 * @return void
 *
 * @example  vDrvSetPWM(SrcPWM0, 6, u4DutyON, 254);
 *           	 Duty Cycle = u4DutyON/(254+1)
 */

void vDrvSetPWM(UINT32 u4Src, UINT32 u4TimeBase, UINT32 u4DutyON,
                UINT32 u4DutyAll)
{
    UINT32 u4Reg = REG_RW_PWM1, u4Value;
    _bPWMSrc = u4Src;

    if(u4DutyON > (u4DutyAll+1))
    {
        Printf("PWM Error: u4DutyON and u4DutyAll= [0x %x,0x %x], u4DutyON is too large\n", u4DutyON, u4DutyAll);
    }
#if defined(CC_MT5880)
		switch(u4Src)
		{
			case SrcPWM0:
				u4Reg = REG_RW_PWM0;
	
				if(DRVCUST_InitGet(ePWM0Port)==PAD_PWM0_PWM0)
				{
					UNUSED(BSP_PinSet(PIN_OPWM0, PINMUX_FUNCTION1));
				}
				else if(DRVCUST_InitGet(ePWM0Port)==PAD_PWM0_GPIO4)		//gpio 61
				{
					UNUSED(BSP_PinSet(PIN_GPIO4, PINMUX_FUNCTION1));
				}
		
				break;
	
			case SrcPWM1:
				u4Reg = REG_RW_PWM1;
	
				if(DRVCUST_InitGet(ePWM1Port)==PAD_PWM1_PWM1)
				{
					UNUSED(BSP_PinSet(PIN_OPWM1, PINMUX_FUNCTION1)); //Tuner 30V Boost-up PWM in BGA
				}
				else if(DRVCUST_InitGet(ePWM1Port)==PAD_PWM1_GPIO5)		//gpio62
				{
					UNUSED(BSP_PinSet(PIN_GPIO5, PINMUX_FUNCTION1));
				}
	
				break;
	
			case SrcPWM2:
				u4Reg = REG_RW_PWM2;
	
				if(DRVCUST_InitGet(ePWM2Port)==PAD_PWM2_PWM2)
				{
					UNUSED(BSP_PinSet(PIN_OPWM2, PINMUX_FUNCTION1)); //LED_CTL in BGA
				}
	
				break;
	
			default:
				ASSERT(u4Src <= SrcPWM2);
		}

#elif defined(CC_MT5399)

    switch(u4Src)
    {
        case SrcPWM0:
            u4Reg = REG_RW_PWM0;

            if(DRVCUST_InitGet(ePWM0Port)==PAD_PWM0_PWM0)
            {
                UNUSED(BSP_PinSet(PIN_OPWM0, PINMUX_FUNCTION2));
            }

            break;

        case SrcPWM1:
            u4Reg = REG_RW_PWM1;

            if(DRVCUST_InitGet(ePWM1Port)==PAD_PWM1_PWM1)
            {
                UNUSED(BSP_PinSet(PIN_OPWM1, PINMUX_FUNCTION2)); //Tuner 30V Boost-up PWM in BGA
            }
            else if(DRVCUST_InitGet(ePWM1Port)==PAD_PWM1_GPIO48)
            {
                UNUSED(BSP_PinSet(PIN_GPIO48, PINMUX_FUNCTION1));
            }

            break;

        case SrcPWM2:
            u4Reg = REG_RW_PWM2;

            if(DRVCUST_InitGet(ePWM2Port)==PAD_PWM2_PWM2)
            {
                UNUSED(BSP_PinSet(PIN_OPWM2, PINMUX_FUNCTION2)); //LED_CTL in BGA
            }

            break;

        default:
            ASSERT(u4Src <= SrcPWM2);
    }

#elif defined(CC_MT5882)

    switch(u4Src)
    {
        case SrcPWM0:
            u4Reg = REG_RW_PWM0;

            UNUSED(BSP_PinSet(PIN_OPWM0, PINMUX_FUNCTION2));
            break;

        case SrcPWM1:
            u4Reg = REG_RW_PWM1;
            UNUSED(BSP_PinSet(PIN_OPWM1, PINMUX_FUNCTION2));

            break;

        case SrcPWM2:
            u4Reg = REG_RW_PWM2;

            UNUSED(BSP_PinSet(PIN_OPWM2, PINMUX_FUNCTION2));
            break;

        default:
            ASSERT(u4Src <= SrcPWM2);
    }
#endif

    // workaround for full duty drop issue
    if(u4DutyAll==0xff && u4DutyON==0xff)	//we used 0xff at normal
    {
        u4DutyAll = 0xfe;
    }

    if(u4DutyAll==0xfff && u4DutyON==0xfff)
    {
        u4DutyAll = 0xffe;
    }

    IO_WRITE32MSK(HPDS_BASE, u4Reg+4, u4TimeBase, 0xFFFFF);
    u4Value = (((UINT32)u4DutyAll << 20) |
               ((UINT32)(u4DutyON & 0xFFF) << 4) | 1);
    IO_WRITE32(HPDS_BASE, u4Reg, u4Value);
}

/**
 * Set PWM sync to LVDS output v-sync signal.
 *
 * @param u4Src      PWM source (SrcPWM0/1/2)
 * @param u4LockON   Lock PWM on LVDS v-sync or not.
 */

void vDrvSetLock(UINT32 u4Src, UINT32 u4LockON)
{
    switch(u4Src)
    {
        default:
        case SrcPWM0:
            IO_WRITE32MSK(
                HPDS_BASE,
                REG_RW_PWM0VSYNC,
                u4LockON? 0x80000000 : 0,
                0xC0000000);
            break;

        case SrcPWM1:
            IO_WRITE32MSK(
                HPDS_BASE,
                REG_RW_PWM1VSYNC,
                u4LockON? 0x80000000 : 0,
                0xC0000000);
            break;

        case SrcPWM2:
            IO_WRITE32MSK(
                HPDS_BASE,
                REG_RW_PWM2VSYNC,
                u4LockON? 0x80000000 : 0,
                0xC0000000);
            break;
    }
}

//-----------------------------------------------------------------------------
/** Brief
 * This driver initialize pwm
 */
void vDrvInitSCANPWM(UINT8 u1Src)
{
    vDrvSetScanPWMOnOff(u1Src, SV_OFF);	
    vDrvSetScanPWMOutBL(u1Src, SV_OFF); // SV_OFF: PWM_SCAN, SV_ON: TCON_3DBL

    //vDrvSetScanPWMLatchMode(SCAN_PWM_LATCH_MODE_SETDATA_OFF,SCAN_PWM_LATCH_MODE_OFF,SCAN_PWM_ALIGN_MODE_VSYNC);  // Aligned Vsync
    vDrvSetScanPWMLatchMode(SCAN_PWM_LATCH_MODE_SETDATA,SCAN_PWM_LATCH_MODE_OFF,SCAN_PWM_ALIGN_MODE_VSYNC);  // Set data

	vDrvSetScanStartModeOnOff(u1Src,SV_OFF);

    vDrvSetScanBLOnOff(SV_OFF);
    vDrvSetScanPWMPolarity(u1Src, 0); 
    vDrvSetScanPWMStartPhase(u1Src,0);

	#ifdef CC_MT5882
	//if(IS_IC_5882_ES1())  // not need eco, cover by sw setting
	{
		
		vIO32WriteFldAlign(PWM_SCAN_32, 0xF0000, REG_PWM_SCAN7_LOW);
		vIO32WriteFldAlign(PWM_SCAN_39, 0xF0000, REG_PWM_SCAN8_LOW);
		
		vIO32WriteFldAlign(PWM_SCAN_3D, 0x1F, REG_PWM_SCAN8_HIGH_ENABLE);
		vIO32WriteFldAlign(PWM_SCAN_3D, 0x1FF, REG_PWM_SCAN8_START_HIGH);
		vIO32WriteFldAlign(PWM_SCAN_3D, 0x1F, REG_PWM_SCAN7_HIGH_ENABLE);
		vIO32WriteFldAlign(PWM_SCAN_3D, 0x1FF, REG_PWM_SCAN7_START_HIGH);
	}
	#endif

    if (u1Src <= PWMSCAN_MAX_CH)
    {
        PWMSCAN_SETDATA[u1Src].u1Initialed =0;	
        PWMSCAN_SETDATA[u1Src].u4LineTotalPre =0;
		PWMSCAN_SETDATA[u1Src].u4High = 0;
		PWMSCAN_SETDATA[u1Src].u4Low = 0;
		PWMSCAN_SETDATA[u1Src].u4Start = 0;
		PWMSCAN_SETDATA[u1Src].u4High_target = 0;
		PWMSCAN_SETDATA[u1Src].u4Low_target = 0;
    }	
	
    //vDrvSetScanPWM(u1Src,0,0,0);
}

//-----------------------------------------------------------------------------
/** Brief
 * This driver calculates the pwm duty cycle period ratio
 * @return ratio
 */
UINT8 vDrvGetPWMDutyCycleRatio(void)
{
    UINT8 u1Ratio, u1FrameRate;
    u1FrameRate = (bSI_DISPLAY_DCLK_TYPE == SV_DCLK_50HZ) ? 50 : 60;
    u1Ratio = (vDrvGetLCDFreq() / u1FrameRate);
    return u1Ratio;
}


/** Brief
 * This driver control the scanning PWM1 & PWM2
 * duty cycle = u4DutyON/u4DutyAll;
 * @param u1Src  PWM source (ScanPWM1/2/3/4/5/6)
 * @param u4Start  start_period (%) ex: 0 --> 0%
 * @param u4High  high_period (%) ex: 40 --> 40%
 * @param u4Low  low_period (%) ex: 60 --> 60%
 * @u4Start + u4High + u4Low = 100%
 * @return void
 */
static void vDrvUpdataPWMCrossZoneStatus(void)
{
	if ((PWMSCAN_SETDATA[1].u1CrossZone == 2)||(PWMSCAN_SETDATA[2].u1CrossZone == 2))
	{
		u1ScanPwmCrossFlg = 2;
	}	
	else if ((PWMSCAN_SETDATA[1].u1CrossZone == 1)||(PWMSCAN_SETDATA[2].u1CrossZone ==1))
	{
		u1ScanPwmCrossFlg = 1;
	}	
	else
	{
		u1ScanPwmCrossFlg = 0;
	}
}

static void vDrvUpdatePWMCrossZoneStatus2(void)
{
	if ((PWMSCAN_SETDATA[1].u1CrossZone == 0)||(PWMSCAN_SETDATA[2].u1CrossZone == 0))
	{
		u1ScanPwmCrossFlg = 0;
	}	
	else
	{
		u1ScanPwmCrossFlg = 2;
	}	
}

UINT32 u4DrvGetScanPWMStructData_Start(UINT8 u1Src)
{
	UINT32 u4Start;
	if (u1Src <= PWMSCAN_MAX_CH)
	{
		u4Start = PWMSCAN_SETDATA[u1Src].u4Start;
		return (u4Start);
	}
	else
	{
		return 0;
	}
}

UINT32 u4DrvGetScanPWMStructData_High(UINT8 u1Src)
{
	UINT32 u4High;
	if (u1Src <= PWMSCAN_MAX_CH)
	{
		u4High = PWMSCAN_SETDATA[u1Src].u4High;
		return (u4High);
	}
	else
	{
		return 0;
	}
}

UINT32 u4DrvGetScanPWMStructData_Low(UINT8 u1Src)
{
	UINT32 u4Low;
	if (u1Src <= PWMSCAN_MAX_CH)
	{
		u4Low = PWMSCAN_SETDATA[u1Src].u4Low;
		return (u4Low);
	}
	else
	{
		return 0;
	}
}

void vDrvSetScanPWMStructData(UINT8 u1Src, UINT32 u4Start, UINT32 u4High, UINT32 u4Low)
{
	if (u1Src <= PWMSCAN_MAX_CH)
	{
		PWMSCAN_SETDATA[u1Src].u4High = u4High;
		PWMSCAN_SETDATA[u1Src].u4Low = u4Low;
		PWMSCAN_SETDATA[u1Src].u4Start= u4Start;			
		Printf("[PWMSCAN_SETDATA]Start=%d, High=%d, Low=%d\n", u4Start, u4High, u4Low);
	}
}

#ifndef CC_MTK_LOADER
static void vDrvSetScanPWMStruct_target(UINT8 u1Src, UINT32 u4High, UINT32 u4Low)
{
	if (u1Src <= PWMSCAN_MAX_CH)
	{
		 PWMSCAN_SETDATA[u1Src].u4High_target = u4High;
		 PWMSCAN_SETDATA[u1Src].u4Low_target = u4Low;	  
	}
}
#endif

UINT8 u1DrvGetScanPWMStruct_Ready(void)
{
	UINT8 u1Ch = 0, u1Ready = 0;;

	for (u1Ch = SrcPWM1; u1Ch <= PWMSCAN_MAX_CH; u1Ch++)
	{
	  if((PWMSCAN_SETDATA[u1Ch].u4High_target == PWMSCAN_SETDATA[u1Ch].u4High)
	  	&& (PWMSCAN_SETDATA[u1Ch].u4Low_target == PWMSCAN_SETDATA[u1Ch].u4Low))
	  {
	   u1Ready |= 1;
	  }
	}
	#if SCANPWM_DEBUGLOG
	LOG(6, "[ScanPWM]u1Ready = %d\n", u1Ready);
	if(!u1Ready)
	{
	 vDrvSetScanPWMSW_SETDATA_Status();	
	}
	#endif
	return(u1Ready);
}

void vDrvSetScanPWMStepControlEn(UINT8 fgEnable)
{
	fgScanPWMSetpControl = fgEnable;
}

void vDrvSetScanPWMStepControl(UINT8 fgEnable, UINT16 u2Step)
{
	vDrvSetScanPWMStepControlEn(fgEnable);
	u2ScanPWMSetp = u2Step;
}

void vDrvSetScanPWMStepTarget(UINT32 u4High, UINT32 u4Low)
{
	PWMSCAN_SETDATA[1].u4High_target= u4High;
	PWMSCAN_SETDATA[2].u4High_target= u4High;	
	PWMSCAN_SETDATA[1].u4Low_target = u4Low;
	PWMSCAN_SETDATA[2].u4Low_target = u4Low;	
	fgApplyScanPWMSetData = 1;
}

void vDrvGetScanPWMStepControlStatus(void)
{
	Printf("fgScanPWMSetpControl=%d\n", fgScanPWMSetpControl);
	Printf("u2ScanPWMSetp=%d\n", u2ScanPWMSetp);	
}

UINT16 u2PWMCrossZoneup = 81; // 36 (sync widthe + VBp) + 45

void vDrvSetCrozzUpLimit(UINT16 u2Value)
{
	if (u2Value >= 10)
		u2PWMCrossZoneup = u2Value-10;
	else
		u2PWMCrossZoneup = 81;
}

UINT16 u2DrvGetCrozzUpLimit(void)
{
	return u2PWMCrossZoneup;
}

static void vDrvSetScanPWMStruct(UINT8 u1Src, UINT32 u4Start, UINT32 u4High, UINT32 u4Low)
{
   UINT8 u1DoubleFrame=0;
   UINT32 u4LineTotalCur = 0,u4LineTotalCur2 = 0;
   UINT16 u2CrossLine = 0, u2CrossLine_top = 0;      

   #if SCANPWM_DEBUGLOG
   LOG(_bScanPWMDbgLogLev, "[ScanPWM]u1Src=%d, u4Start=%d, u4High=%d, u4Low=%d \n",u1Src,u4Start, u4High, u4Low);
   #endif
   if (u1Src <= PWMSCAN_MAX_CH)
   {
   		if (PWMSCAN_SETDATA[u1Src].u1Initialed != 1)
   		{
			PWMSCAN_SETDATA[u1Src].u4High_target= u4High;
			PWMSCAN_SETDATA[u1Src].u4Low_target = u4Low;	 
   		}
   		PWMSCAN_SETDATA[u1Src].u1Initialed = 1;
   		PWMSCAN_SETDATA[u1Src].u4Start = u4Start;
   		PWMSCAN_SETDATA[u1Src].u4High = u4High;
   		PWMSCAN_SETDATA[u1Src].u4Low = u4Low;    
   
   		if ((u4High+u4Low)<510)
   		{
     		u1DoubleFrame =1;
   		}  

   		if(u1DoubleFrame)
  		{
     		u4LineTotalCur = (u4Start + u4High + u4Low + u4High);
			u4LineTotalCur2 = u4LineTotalCur;
			if(u4LineTotalCur>1000)
			{
				#if SCANPWM_DEBUGLOG
				LOG(6, "[ScanPWM]DoubleFrame #1\n");
				#endif
				u4LineTotalCur = (u4LineTotalCur - 1000);
			}
			else
			{
				#if SCANPWM_DEBUGLOG
				LOG(6, "[ScanPWM]DoubleFrame #2\n");
				#endif
				u4LineTotalCur = (u4Start + u4High);
			}
   		}
   		else
   		{
     		u4LineTotalCur = (u4Start + u4High);
			u4LineTotalCur2 = u4LineTotalCur;
			if(u4LineTotalCur>1000)
			{
				#if SCANPWM_DEBUGLOG
				LOG(6, "[ScanPWM]SingleFrame #0\n");
				#endif
				u4LineTotalCur = (u4LineTotalCur - 1000);
			}			
   		}   
		#if SCANPWM_DEBUGLOG
   		LOG(_bScanPWMDbgLogLev, "[ScanPWM]u4LineTotalCur=%d\n",u4LineTotalCur);
   		#endif
		u2CrossLine = wDrvGetOutputVTotal()-wDrvGetOutputVActive()-wDrvGetVsyncFp(); // [Apply point1 position] VDE Rising
		#if SCANPWM_DEBUGLOG
   		LOG(_bScanPWMDbgLogLev, "[ScanPWM]u2CrossLine=%d(line)\n",u2CrossLine);
   		#endif
		u2CrossLine = u2CrossLine * 1000 / wDrvGetOutputVTotal();
		#if SCANPWM_DEBUGLOG
   		LOG(_bScanPWMDbgLogLev, "[ScanPWM]u2CrossLine=%d(%)\n",u2CrossLine);
   		#endif
		u2CrossLine_top = u2CrossLine + 45;
		#if SCANPWM_DEBUGLOG
   		LOG(_bScanPWMDbgLogLev, "[ScanPWM]u2CrossLine_top=%d(%)\n",u2CrossLine_top);
   		#endif
		vDrvSetCrozzUpLimit(u2CrossLine_top);

		if(fgScanPWMSetpControl)
		{
		// Use apply point 2 instead of apply point 1 to avoid VDE rising set  next step transient
		// if((u4LineTotalCur >= 0) && (u4LineTotalCur <= u2CrossLine_top))  // pointless comparison of unsigned integer with zero
		if((u4LineTotalCur >= 0) && (u4LineTotalCur <= u2CrossLine_top))
		{

 			PWMSCAN_SETDATA[u1Src].u1CrossZone = 1;
			#if SCANPWM_DEBUGLOG
	   		LOG(_bScanPWMDbgLogLev, "[ScanPWM][Step_ctrl]VDE_R_ZONE->VDE_F\n");
	   		#endif
		}
		else
   		{
     		PWMSCAN_SETDATA[u1Src].u1CrossZone = 0;
			#if SCANPWM_DEBUGLOG
	   		LOG(_bScanPWMDbgLogLev, "[ScanPWM][Step_ctrl]VDE_F_ZONE->TCON_ISR\n");
	   		#endif
   		}   

   		PWMSCAN_SETDATA[u1Src].u4LineTotalPre = u4LineTotalCur2;
   		vDrvUpdataPWMCrossZoneStatus();
		}
		else
		{
			if((u4LineTotalCur < 1000) && (PWMSCAN_SETDATA[u1Src].u4LineTotalPre > 1000))
			  {
			 	PWMSCAN_SETDATA[u1Src].u1CrossZone = 0;
				#if SCANPWM_DEBUGLOG
		   		LOG(_bScanPWMDbgLogLev, "[ScanPWM][Step_ctrl]VDE_F_ZONE->TCON_ISR\n");
		   		#endif
			  }
			  else
			  {
			  	PWMSCAN_SETDATA[u1Src].u1CrossZone = 2;
				#if SCANPWM_DEBUGLOG
		   		LOG(_bScanPWMDbgLogLev, "[ScanPWM][Step_ctrl]OutVsyncISR setdata\n");
		   		#endif
			  }
			  
	     PWMSCAN_SETDATA[u1Src].u4LineTotalPre = u4LineTotalCur;		
		 vDrvUpdatePWMCrossZoneStatus2(); 
		}
   }
}

void vDrvSetScanPWMSW_StepControl(void)
{
	UINT32 u4HighChg=0, u4LowChg=0;

	UINT8 u1Ch = 0;

	for (u1Ch = SrcPWM1; u1Ch <= PWMSCAN_MAX_CH; u1Ch++)
	{
		if((PWMSCAN_SETDATA[u1Ch].u4High_target != PWMSCAN_SETDATA[u1Ch].u4High)
		|| (PWMSCAN_SETDATA[u1Ch].u4Low_target != PWMSCAN_SETDATA[u1Ch].u4Low))	
		{
			#if SCANPWM_DEBUGLOG
			LOG(6, "[ScanPWM]High_step=%d, Low_step=%d \n",PWMSCAN_SETDATA[u1Ch].u4High,PWMSCAN_SETDATA[u1Ch].u4Low);
			#endif
			
			if (PWMSCAN_SETDATA[u1Ch].u4High_target >= PWMSCAN_SETDATA[u1Ch].u4High)
			{
				u4HighChg = PWMSCAN_SETDATA[u1Ch].u4High + u2ScanPWMSetp;
				if (u4HighChg > PWMSCAN_SETDATA[u1Ch].u4High_target)
				{
				 u4HighChg = PWMSCAN_SETDATA[u1Ch].u4High_target;	
				} 
			}
			else
			{
				u4HighChg = PWMSCAN_SETDATA[u1Ch].u4High - u2ScanPWMSetp;				
				if (u4HighChg < PWMSCAN_SETDATA[u1Ch].u4High_target)
				{
				 u4HighChg = PWMSCAN_SETDATA[u1Ch].u4High_target;	
				} 				
			}
		
			if (PWMSCAN_SETDATA[u1Ch].u4Low_target >= PWMSCAN_SETDATA[u1Ch].u4Low)
			{
				u4LowChg = PWMSCAN_SETDATA[u1Ch].u4Low + u2ScanPWMSetp;
				if (u4LowChg > PWMSCAN_SETDATA[u1Ch].u4Low_target)
				{
				 u4LowChg = PWMSCAN_SETDATA[u1Ch].u4Low_target;	
				} 				
			}
			else
			{
				u4LowChg = PWMSCAN_SETDATA[u1Ch].u4Low - u2ScanPWMSetp; 			
				if (u4LowChg < PWMSCAN_SETDATA[u1Ch].u4Low_target)
				{
				 u4LowChg = PWMSCAN_SETDATA[u1Ch].u4Low_target;	
				} 								
			}
			#if SCANPWM_DEBUGLOG
			LOG(6, "[ScanPWM]u4HighChg=%d, u4LowChg=%d \n",u4HighChg,u4LowChg);
			#endif
			
			vDrvSetScanPWMStruct(u1Ch,PWMSCAN_SETDATA[u1Ch].u4Start,u4HighChg,u4LowChg);			
		}
	}
}


void vDrvSetScanPWMSW(UINT8 u1Src, UINT32 u4Start, UINT32 u4High, UINT32 u4Low)
{
#ifndef CC_MTK_LOADER 
    // CRIT_STATE_T csState;
    // csState = x_crit_start();
#endif     

#ifndef CC_MTK_LOADER
	{
	    vDrvSetScanPWMStruct_target(u1Src,u4High,u4Low);
		
		if (!fgScanPWMSetpControl)
		{
 		 vDrvSetScanPWMStruct(u1Src,u4Start,u4High,u4Low);
		}
		
         fgApplyScanPWMSetData = 1;        
		
        vDrvSetScanPWMLatchMode(SCAN_PWM_LATCH_MODE_SETDATA,SCAN_PWM_LATCH_MODE_OFF,SCAN_PWM_ALIGN_MODE_VSYNC);  // Set data
	}	
#else   
    {
    	vDrvSetScanPWM(u1Src, u4Start, u4High, u4Low);
		vDrvSetScanPWMStruct(u1Src,u4Start,u4High,u4Low);
        vDrvSetScanPWMLatchMode(SCAN_PWM_LATCH_MODE_SETDATA_OFF,SCAN_PWM_LATCH_MODE_OFF,SCAN_PWM_ALIGN_MODE_ONCE);  // Aligned at once
        vDrvScanPWMDataFire();
    }
#endif



#ifndef CC_MTK_LOADER    
    // x_crit_end(csState);
#endif    

}

void vDrvSetScanPWMSW_SETDATA(void)
{
	if(PWMSCAN_SETDATA[SrcPWM1].u1Initialed)
	{
    	vDrvSetScanPWM(SrcPWM1, PWMSCAN_SETDATA[SrcPWM1].u4Start, PWMSCAN_SETDATA[SrcPWM1].u4High, PWMSCAN_SETDATA[SrcPWM1].u4Low );    
		#if SCANPWM_DEBUGLOG		
		LOG(_bScanPWMDbgLogLev, "[ScanPWM]SrcPWM1 S=%d H=%d L=%d", PWMSCAN_SETDATA[SrcPWM1].u4Start, PWMSCAN_SETDATA[SrcPWM1].u4High, PWMSCAN_SETDATA[SrcPWM1].u4Low);
		#endif		
	}	
	
	if(PWMSCAN_SETDATA[SrcPWM2].u1Initialed)
	{
    	vDrvSetScanPWM(SrcPWM2, PWMSCAN_SETDATA[SrcPWM2].u4Start, PWMSCAN_SETDATA[SrcPWM2].u4High, PWMSCAN_SETDATA[SrcPWM2].u4Low );
		#if SCANPWM_DEBUGLOG
		LOG(_bScanPWMDbgLogLev, "[ScanPWM]SrcPWM2 S=%d H=%d L=%d\n", PWMSCAN_SETDATA[SrcPWM2].u4Start, PWMSCAN_SETDATA[SrcPWM2].u4High, PWMSCAN_SETDATA[SrcPWM2].u4Low);		
		#endif		
	}
}

void vDrvSetScanPWMSW_SETDATA_Status(void)
{
	UINT8 u1Ch = 0;

	for (u1Ch = SrcPWM1; u1Ch <= PWMSCAN_MAX_CH; u1Ch++)
	{
	 Printf("[SCAN_PWM_%d] u1Initialed=%d, u4Start=%d, u4High=%d,  u4Low=%d , u4High_target=%d,  u4Low_target=%d\n",u1Ch,PWMSCAN_SETDATA[u1Ch].u1Initialed, PWMSCAN_SETDATA[u1Ch].u4Start, PWMSCAN_SETDATA[u1Ch].u4High, PWMSCAN_SETDATA[u1Ch].u4Low, PWMSCAN_SETDATA[u1Ch].u4High_target, PWMSCAN_SETDATA[u1Ch].u4Low_target);	
	 Printf("[SCAN_PWM_%d] u4LineTotalPre=%d, u1CrossZone=%d\n",u1Ch,PWMSCAN_SETDATA[u1Ch].u4LineTotalPre, PWMSCAN_SETDATA[u1Ch].u1CrossZone);		 
	}
	Printf("[ScanPWM]u2PWMCrossZoneup = %d \n",u2DrvGetCrozzUpLimit());
}

void vDrvSetScanPWM(UINT8 u1Src, UINT32 u4Start, UINT32 u4High, UINT32 u4Low)
{
    UINT16 u2HTotal, u2VTotal;
    UINT32 u4Val;

	vDrvSetScanPWMOnOff(u1Src,SV_ON);
	
	u4Val = u4High + u4Low;
	
	if (u4Val > 1000)
	{
		return;
	}
    
    u2HTotal = wDrvGetOutputHTotal();
    u2VTotal = wDrvGetOutputVTotal();
    u4Val = u2HTotal * u2VTotal / 1000;

	LOG(3, "_|-|_ vDrvSetScanPWM [SRC:scanPWM%d,%d*%d,start:%d%,high:%d%,low:%d%]\n", u1Src, u2HTotal, u2VTotal, u4Start, u4High, u4Low);

    switch(u1Src)
    {
        case SrcPWM1:
            BSP_PinSet(PIN_OPWM0, PINMUX_FUNCTION1);
            if (u4High == 1000)            
            {			 
	            vIO32WriteFldAlign(PWM_SCAN_06, 1, REG_PWM_SCAN1_FIXED_HIGH);			 
	            vIO32WriteFldAlign(PWM_SCAN_06, 0, REG_PWM_SCAN1_FIXED_LOW);		 			 			 			             
            }			
            else if (u4High == 0)			
            {			 
	            vIO32WriteFldAlign(PWM_SCAN_06, 0, REG_PWM_SCAN1_FIXED_HIGH);			 
	            vIO32WriteFldAlign(PWM_SCAN_06, 1, REG_PWM_SCAN1_FIXED_LOW);					 			 			 			
            }
            else
            {
	            vIO32WriteFldAlign(PWM_SCAN_06, 0, REG_PWM_SCAN1_FIXED_HIGH);			 
	            vIO32WriteFldAlign(PWM_SCAN_06, 0, REG_PWM_SCAN1_FIXED_LOW);			             			
            }
            if (u4Start==0)
            {
				//vDrvSetScan1Start(0x0);
				vDrvSetScan1Phase(0x0);
            }
            else
            {
            	//vDrvSetScan1Start(u4Start*u4Val);
            	vDrvSetScan1Phase(u4Start*u4Val);
            }
            vDrvSetScan1High((u4High*u4Val));
            vDrvSetScan1Low(u4Low*u4Val);
            break;

        case SrcPWM2:
            BSP_PinSet(PIN_OPWM1, PINMUX_FUNCTION3);
            if (u4High == 1000)            
            {			 
	            vIO32WriteFldAlign(PWM_SCAN_06, 1, REG_PWM_SCAN2_FIXED_HIGH);			 
	            vIO32WriteFldAlign(PWM_SCAN_06, 0, REG_PWM_SCAN2_FIXED_LOW);		 			 			 			             
            }			
            else if (u4High == 0)			
            {			 
				vIO32WriteFldAlign(PWM_SCAN_06, 0, REG_PWM_SCAN2_FIXED_HIGH);			 
				vIO32WriteFldAlign(PWM_SCAN_06, 1, REG_PWM_SCAN2_FIXED_LOW);					 			 			 			
			}			
			else			
			{			 
				vIO32WriteFldAlign(PWM_SCAN_06, 0, REG_PWM_SCAN2_FIXED_HIGH);			 
				vIO32WriteFldAlign(PWM_SCAN_06, 0, REG_PWM_SCAN2_FIXED_LOW);			             			
			}
			if (u4Start==0)
			{
				//vDrvSetScan2Start(0x0);
				vDrvSetScan2Phase(0x0);
			}
			else
			{
            	//vDrvSetScan2Start(u4Start*u4Val);
            	vDrvSetScan2Phase(u4Start*u4Val);
            }
            vDrvSetScan2High((u4High*u4Val));
            vDrvSetScan2Low(u4Low*u4Val);
            break;

        case SrcPWM3:
        	BSP_PinSet(PIN_OPWM2, PINMUX_FUNCTION3);
            if (u4High == 100)            
            {			 
	            vIO32WriteFldAlign(PWM_SCAN_10, 1, REG_PWM_SCAN3_FIXED_HIGH);			 
	            vIO32WriteFldAlign(PWM_SCAN_10, 0, REG_PWM_SCAN3_FIXED_LOW);		 			 			 			             
            }			
            else if (u4High == 0)			
            {			 
				vIO32WriteFldAlign(PWM_SCAN_10, 0, REG_PWM_SCAN3_FIXED_HIGH);			 
				vIO32WriteFldAlign(PWM_SCAN_10, 1, REG_PWM_SCAN3_FIXED_LOW);					 			 			 			
			}			
			else			
			{			 
				vIO32WriteFldAlign(PWM_SCAN_10, 0, REG_PWM_SCAN3_FIXED_HIGH);			 
				vIO32WriteFldAlign(PWM_SCAN_10, 0, REG_PWM_SCAN3_FIXED_LOW);			             			
			}
			if (u4Start==0)
			{
				//vDrvSetScan3Start(0x0);
				vDrvSetScan3Phase(0x0);
			}
			else
			{
            	//vDrvSetScan3Start(u4Start*u4Val);
            	vDrvSetScan3Phase(u4Start*u4Val);
            }
            vDrvSetScan3High((u4High*u4Val));
            vDrvSetScan3Low(u4Low*u4Val);
            break;

        case SrcPWM4:
            // JTDI PINMUX --> FUNCTION4
            if (u4High == 1000)            
            {			 
	            vIO32WriteFldAlign(PWM_SCAN_10, 1, REG_PWM_SCAN4_FIXED_HIGH);			 
	            vIO32WriteFldAlign(PWM_SCAN_10, 0, REG_PWM_SCAN4_FIXED_LOW);		 			 			 			             
            }			
            else if (u4High == 0)			
            {			 
				vIO32WriteFldAlign(PWM_SCAN_10, 0, REG_PWM_SCAN4_FIXED_HIGH);			 
				vIO32WriteFldAlign(PWM_SCAN_10, 1, REG_PWM_SCAN4_FIXED_LOW);					 			 			 			
			}			
			else			
			{			 
				vIO32WriteFldAlign(PWM_SCAN_10, 0, REG_PWM_SCAN4_FIXED_HIGH);			 
				vIO32WriteFldAlign(PWM_SCAN_10, 0, REG_PWM_SCAN4_FIXED_LOW);			             			
			}
			if (u4Start==0)
			{
				//vDrvSetScan4Start(0x0);
				vDrvSetScan4Phase(0x0);
			}
			else
			{
            	//vDrvSetScan4Start(u4Start*u4Val);
            	vDrvSetScan4Phase(u4Start*u4Val);
            }
            vDrvSetScan4High((u4High*u4Val));
            vDrvSetScan4Low(u4Low*u4Val);
            break;

        case SrcPWM5:
            // JTRST PINMUX --> FUNCTION4
            if (u4High == 1000)            
            {			 
	            vIO32WriteFldAlign(PWM_SCAN_11, 1, REG_PWM_SCAN5_FIXED_HIGH);			 
	            vIO32WriteFldAlign(PWM_SCAN_11, 0, REG_PWM_SCAN5_FIXED_LOW);		 			 			 			             
            }			
            else if (u4High == 0)			
            {			 
				vIO32WriteFldAlign(PWM_SCAN_11, 0, REG_PWM_SCAN5_FIXED_HIGH);			 
				vIO32WriteFldAlign(PWM_SCAN_11, 1, REG_PWM_SCAN5_FIXED_LOW);					 			 			 			
			}			
			else			
			{			 
				vIO32WriteFldAlign(PWM_SCAN_11, 0, REG_PWM_SCAN5_FIXED_HIGH);			 
				vIO32WriteFldAlign(PWM_SCAN_11, 0, REG_PWM_SCAN5_FIXED_LOW);			             			
			}
			if (u4Start==0)
			{
				//vDrvSetScan5Start(0x0);
				vDrvSetScan5Phase(0x0);
			}
			else
			{
            	//vDrvSetScan5Start(u4Start*u4Val);
            	vDrvSetScan5Phase(u4Start*u4Val);
            }
            vDrvSetScan5High((u4High*u4Val));
            vDrvSetScan5Low(u4Low*u4Val);
            break;

		case SrcPWM6:
			// OPWM0/1/2 PINMUX --> FUNCTION1
            if (u4High == 1000)            
            {			 
	            vIO32WriteFldAlign(PWM_SCAN_11, 1, REG_PWM_SCAN6_FIXED_HIGH);			 
	            vIO32WriteFldAlign(PWM_SCAN_11, 0, REG_PWM_SCAN6_FIXED_LOW);		 			 			 			             
            }			
            else if (u4High == 0)			
            {			 
				vIO32WriteFldAlign(PWM_SCAN_11, 0, REG_PWM_SCAN6_FIXED_HIGH);			 
				vIO32WriteFldAlign(PWM_SCAN_11, 1, REG_PWM_SCAN6_FIXED_LOW);					 			 			 			
			}			
			else			
			{			 
				vIO32WriteFldAlign(PWM_SCAN_11, 0, REG_PWM_SCAN6_FIXED_HIGH);			 
				vIO32WriteFldAlign(PWM_SCAN_11, 0, REG_PWM_SCAN6_FIXED_LOW);			             			
			}
			if (u4Start==0)
			{
				//vDrvSetScan6Start(0x0);
				vDrvSetScan6Phase(0x0);
			}
			else
			{
            	//vDrvSetScan6Start(u4Start*u4Val);
            	vDrvSetScan6Phase(u4Start*u4Val);
            }
            vDrvSetScan6High((u4High*u4Val));
            vDrvSetScan6Low(u4Low*u4Val);
            break;
			
		case SrcPWM7:
			// TCON6 PINMUX --> FUNCTION3
            if (u4High == 1000)            
            {			 
	            vIO32WriteFldAlign(PWM_SCAN_30, 1, REG_PWM_SCAN7_FIXED_HIGH);			 
	            vIO32WriteFldAlign(PWM_SCAN_30, 0, REG_PWM_SCAN7_FIXED_LOW);		 			 			 			             
            }			
            else if (u4High == 0)			
            {			 
				vIO32WriteFldAlign(PWM_SCAN_30, 0, REG_PWM_SCAN7_FIXED_HIGH);			 
				vIO32WriteFldAlign(PWM_SCAN_30, 1, REG_PWM_SCAN7_FIXED_LOW);					 			 			 			
			}			
			else			
			{			 
				vIO32WriteFldAlign(PWM_SCAN_30, 0, REG_PWM_SCAN7_FIXED_HIGH);			 
				vIO32WriteFldAlign(PWM_SCAN_30, 0, REG_PWM_SCAN7_FIXED_LOW);			             			
			}
			if (u4Start==0)
			{
				//vDrvSetScan7Start(0x0);
				vDrvSetScan7Phase(0x0);
			}
			else
			{
            	//vDrvSetScan7Start(u4Start*u4Val);
            	vDrvSetScan7Phase(u4Start*u4Val);
            }
            vDrvSetScan7High((u4High*u4Val));
            vDrvSetScan7Low(u4Low*u4Val);
            break;
			
		case SrcPWM8:
			// TCON7 PINMUX --> FUNCTION3
            if (u4High == 1000)            
            {			 
	            vIO32WriteFldAlign(PWM_SCAN_37, 1, REG_PWM_SCAN8_FIXED_HIGH);			 
	            vIO32WriteFldAlign(PWM_SCAN_37, 0, REG_PWM_SCAN8_FIXED_LOW);		 			 			 			             
            }			
            else if (u4High == 0)			
            {			 
				vIO32WriteFldAlign(PWM_SCAN_37, 0, REG_PWM_SCAN8_FIXED_HIGH);			 
				vIO32WriteFldAlign(PWM_SCAN_37, 1, REG_PWM_SCAN8_FIXED_LOW);					 			 			 			
			}			
			else			
			{			 
				vIO32WriteFldAlign(PWM_SCAN_37, 0, REG_PWM_SCAN8_FIXED_HIGH);			 
				vIO32WriteFldAlign(PWM_SCAN_37, 0, REG_PWM_SCAN8_FIXED_LOW);			             			
			}
			if (u4Start==0)
			{
				//vDrvSetScan8Start(0x0);
				vDrvSetScan8Phase(0x0);
			}
			else
			{
            	//vDrvSetScan8Start(u4Start*u4Val);
            	vDrvSetScan8Phase(u4Start*u4Val);
            }
            vDrvSetScan8High((u4High*u4Val));
            vDrvSetScan8Low(u4Low*u4Val);
            break;

        default:
            ASSERT(u1Src <= SrcPWM8);
    }

}

void vDrvSetScanPWMStartPhase(UINT32 u4Src, UINT32 u4Start_Phase)
{
    UINT16 u2HTotal, u2VTotal;
    UINT32 u4Val;

	u2HTotal = wDrvGetOutputHTotal();
    u2VTotal = wDrvGetOutputVTotal();

	u4Val = u2HTotal * u2VTotal / 1000;

    switch(u4Src)
    {
        case SrcPWM1:
			vDrvSetScan1Phase(u4Val * u4Start_Phase);
			break;

		case SrcPWM2:
			vDrvSetScan2Phase(u4Val * u4Start_Phase);
			break;

		case SrcPWM3:
			vDrvSetScan3Phase(u4Val * u4Start_Phase);
			break;

		case SrcPWM4:
			vDrvSetScan4Phase(u4Val * u4Start_Phase);
			break;

		case SrcPWM5:
			vDrvSetScan5Phase(u4Val * u4Start_Phase);
			break;

		case SrcPWM6:
			vDrvSetScan6Phase(u4Val * u4Start_Phase);
			break;

		case SrcPWM7:
			vDrvSetScan7Phase(u4Val * u4Start_Phase);
			break;
			
		case SrcPWM8:
			vDrvSetScan8Phase(u4Val * u4Start_Phase);
			break;
			
		default:
            ASSERT(u4Src <= SrcPWM8);
    }
}

/** Brief
 * This driver control the scanning backlight enable
 * @param u1OnOff
 * @return void
 */
void vDrvSetScanBLOnOff(UINT8 u1OnOff)
{
    vIO32WriteFldAlign(PWM_HS_01, u1OnOff, TCON_BL_EN);
}

/** Brief
 * This driver control the scanning backlight polarify
 * @param bSrc  PWM source (ScanPWM1/2)
 * @param u1blPol  0: Normal/1: Reverse
 * @return void
 */
void vDrvSetScanPWMPolarity(UINT32 u4Src, UINT8 u1blPol)
{
    switch(u4Src)
    {
        case SrcPWM1:
            vIO32WriteFldAlign(PWM_SCAN_06, u1blPol, REG_PWM_SCAN1_POL);
            break;

        case SrcPWM2:
            vIO32WriteFldAlign(PWM_SCAN_06, u1blPol, REG_PWM_SCAN2_POL);
            break;

        case SrcPWM3:
            vIO32WriteFldAlign(PWM_SCAN_10, u1blPol, REG_PWM_SCAN3_POL);
            break;

        case SrcPWM4:
            vIO32WriteFldAlign(PWM_SCAN_10, u1blPol, REG_PWM_SCAN4_POL);
            break;

        case SrcPWM5:
            vIO32WriteFldAlign(PWM_SCAN_11, u1blPol, REG_PWM_SCAN5_POL);
            break;

		case SrcPWM6:
			vIO32WriteFldAlign(PWM_SCAN_11, u1blPol, REG_PWM_SCAN6_POL);
            break;

		case SrcPWM7:
			vIO32WriteFldAlign(PWM_SCAN_30, u1blPol, REG_PWM_SCAN7_POL);
            break;
			
		case SrcPWM8:
			vIO32WriteFldAlign(PWM_SCAN_37, u1blPol, REG_PWM_SCAN8_POL);
            break;
			
        default:
            ASSERT(u4Src <= SrcPWM8);
    }
}

/** Brief
 * This driver control the scanning PWM enable
 * @param u1OnOff
 * @return void
 */
void vDrvSetScanPWMOnOff(UINT32 u4Src, UINT8 u1OnOff)
{
	switch(u4Src)    
	{        
		case SrcPWM1:            
			vIO32WriteFldAlign(PWM_SCAN_12, u1OnOff, REG_PWM_SCAN1_EN);            
			break;        
		case SrcPWM2:            
			vIO32WriteFldAlign(PWM_SCAN_12, u1OnOff, REG_PWM_SCAN2_EN);            
			break;        
		case SrcPWM3:            
			vIO32WriteFldAlign(PWM_SCAN_12, u1OnOff, REG_PWM_SCAN3_EN);           
			break;        
		case SrcPWM4:            
			vIO32WriteFldAlign(PWM_SCAN_12, u1OnOff, REG_PWM_SCAN4_EN);            
			break;        
		case SrcPWM5:            
			vIO32WriteFldAlign(PWM_SCAN_12, u1OnOff, REG_PWM_SCAN5_EN);            
			break;		
		case SrcPWM6:			
			vIO32WriteFldAlign(PWM_SCAN_12, u1OnOff, REG_PWM_SCAN6_EN);            
			break;
		case SrcPWM7:			
			vIO32WriteFldAlign(PWM_SCAN_2F, u1OnOff, REG_PWM_SCAN7_EN);            
			break;
		case SrcPWM8:			
			vIO32WriteFldAlign(PWM_SCAN_36, u1OnOff, REG_PWM_SCAN8_EN);            
			break;
		default:		    
			ASSERT(u4Src <= SrcPWM8);    
	}
}

BOOL fgIsScanPWMSetDataLatch(void)
{
	if ((IO32ReadFldAlign(PWM_HS_01,REG_PWM_SCAN_SET_DATA_EN)==1)
	   &&(IO32ReadFldAlign(PWM_HS_01,REG_PWM_SCAN_SET_DATA_EN2)==1))
	{
		return (TRUE);
	}
	return (FALSE);
}

/** Brief
 * This driver control the scanning PWM latch mode
 * @param u1Mode
 * @param 0: Latch until set_data assert
 * @param 1: Update at once
 * @return void
 */
void vDrvSetScanPWMLatchMode(UINT8 u1SetDataMode,UINT8 u1Mode, UINT8 u1AlignMode)
{
    vIO32WriteFldAlign(PWM_HS_01, u1SetDataMode, REG_PWM_SCAN_SET_DATA_EN);
    vIO32WriteFldAlign(PWM_HS_01, u1SetDataMode, REG_PWM_SCAN_SET_DATA_EN2);
    vIO32WriteFldAlign(PWM_HS_01, u1Mode, REG_PWM_SCAN_LATCH_OFF);
    vIO32WriteFldAlign(PWM_HS_01, u1Mode, REG_PWM_SCAN_LATCH_OFF2);
	vIO32WriteFldAlign(PWM_HS_01, u1AlignMode, REG_PWM_SCAN_LATCH_SEL);
}

void vDrvScanPWMDataFire(void)
{
	vIO32WriteFldAlign(PWM_HS_01, 1, REG_PWM_SCAN_LATCH_OFF);
    vIO32WriteFldAlign(PWM_HS_01, 0, REG_PWM_SCAN_LATCH_OFF);
    
    vIO32WriteFldAlign(PWM_HS_01, 1, REG_PWM_SCAN_LATCH_OFF2);
    vIO32WriteFldAlign(PWM_HS_01, 0, REG_PWM_SCAN_LATCH_OFF2);
}

/** Brief
 * This driver control the output mux for scanning pwm
 * @param bSrc  PWM source (ScanPWM1/2)
 * @param u1OutBL
 * @param 0: output SCAN_PWM
 * @param 1: output TCON_3dbl_out which is from Panel_intf
 * @return void
 */
void vDrvSetScanPWMOutBL(UINT32 u4Src, UINT8 u1OutBL)
{
    switch(u4Src)
    {
        case SrcPWM1:
            vIO32WriteFldAlign(PWM_SCAN_06, u1OutBL, REG_TCON_3DBL_OUT_EN1);
            break;

        case SrcPWM2:
            vIO32WriteFldAlign(PWM_SCAN_06, u1OutBL, REG_TCON_3DBL_OUT_EN2);
            break;

        case SrcPWM3:
            vIO32WriteFldAlign(PWM_SCAN_10, u1OutBL, REG_TCON_3DBL_OUT_EN3);
            break;

        case SrcPWM4:
            vIO32WriteFldAlign(PWM_SCAN_10, u1OutBL, REG_TCON_3DBL_OUT_EN4);
            break;

        case SrcPWM5:
            vIO32WriteFldAlign(PWM_SCAN_11, u1OutBL, REG_TCON_3DBL_OUT_EN5);
            break;

		case SrcPWM6:
            vIO32WriteFldAlign(PWM_SCAN_11, u1OutBL, REG_TCON_3DBL_OUT_EN6);
            break;

		case SrcPWM7:
            vIO32WriteFldAlign(PWM_SCAN_2F, u1OutBL, REG_TCON_3DBL_OUT_EN7);
            break;

		case SrcPWM8:
            vIO32WriteFldAlign(PWM_SCAN_36, u1OutBL, REG_TCON_3DBL_OUT_EN8);
            break;

        default:
            ASSERT(u4Src <= SrcPWM8);
    }

}

void vDrvSetScanStartModeOnOff(UINT32 u4Src, UINT8 u1OnOff)
{
     switch(u4Src)
    {
        case SrcPWM1:
            vIO32WriteFldAlign(PWM_SCAN_12, u1OnOff, REG_PWM_SCAN1_START_MODE);
            break;

        case SrcPWM2:
            vIO32WriteFldAlign(PWM_SCAN_12, u1OnOff, REG_PWM_SCAN2_START_MODE);
            break;

        case SrcPWM3:
            vIO32WriteFldAlign(PWM_SCAN_12, u1OnOff, REG_PWM_SCAN3_START_MODE);
            break;

        case SrcPWM4:
            vIO32WriteFldAlign(PWM_SCAN_12, u1OnOff, REG_PWM_SCAN4_START_MODE);
            break;

        case SrcPWM5:
            vIO32WriteFldAlign(PWM_SCAN_12, u1OnOff, REG_PWM_SCAN5_START_MODE);
            break;

		case SrcPWM6:
			vIO32WriteFldAlign(PWM_SCAN_12, u1OnOff, REG_PWM_SCAN6_START_MODE);
            break;

		case SrcPWM7:
			vIO32WriteFldAlign(PWM_SCAN_2F, u1OnOff, REG_PWM_SCAN7_START_MODE);
            break;

		case SrcPWM8:
			vIO32WriteFldAlign(PWM_SCAN_36, u1OnOff, REG_PWM_SCAN8_START_MODE);
            break;
		default:
            ASSERT(u4Src <= SrcPWM8);
    }

	vIO32WriteFldAlign(PWM_HS_01, 1, REG_PWM_SCAN_LATCH_OFF);
	vIO32WriteFldAlign(PWM_HS_01, 0, REG_PWM_SCAN_LATCH_OFF);	
}

/** Brief
 * This driver control the scanning backlight polarify
 * @param u1blPol  0: Normal/1: Reverse
 * @return void
 */
void vDrvSetScanBLPolarity(UINT8 u1blPol)
{
    vIO32WriteFldAlign(PWM_HS_01, u1blPol, TCON_BL_IN_POL);
}

/** Brief
 * This driver control the scanning backlight latch mode
 * @param 0: Latch BL into a whole frame
 * @param 1: Direct use BL info to mask PWM
 * @return void
 */
void vDrvSetScanBLLatchMode(UINT8 u1Mode)
{
    vIO32WriteFldAlign(PWM_SCAN_06, u1Mode, REG_PWM_SCAN_BL_DIS_LATCH);
}

void vDrvSetScanFixHighLow(UINT8 channel, UINT8 mode)
{
	switch(channel)
    {
        case SrcPWM1:
			if (mode==1)            
            {			 
	            vIO32WriteFldAlign(PWM_SCAN_06, 1, REG_PWM_SCAN1_FIXED_HIGH);			 
	            vIO32WriteFldAlign(PWM_SCAN_06, 0, REG_PWM_SCAN1_FIXED_LOW);		 			 			 			             
            }			
            else if (mode==2)			
            {			 
	            vIO32WriteFldAlign(PWM_SCAN_06, 0, REG_PWM_SCAN1_FIXED_HIGH);			 
	            vIO32WriteFldAlign(PWM_SCAN_06, 1, REG_PWM_SCAN1_FIXED_LOW);					 			 			 			
            }
			else
			{
				vIO32WriteFldAlign(PWM_SCAN_06, 0, REG_PWM_SCAN1_FIXED_HIGH);			 
	            vIO32WriteFldAlign(PWM_SCAN_06, 0, REG_PWM_SCAN1_FIXED_LOW);
			}
			break;
		case SrcPWM2:
			if (mode==1)            
            {			 
	            vIO32WriteFldAlign(PWM_SCAN_06, 1, REG_PWM_SCAN2_FIXED_HIGH);			 
	            vIO32WriteFldAlign(PWM_SCAN_06, 0, REG_PWM_SCAN2_FIXED_LOW);		 			 			 			             
            }			
            else if (mode==2)			
            {			 
				vIO32WriteFldAlign(PWM_SCAN_06, 0, REG_PWM_SCAN2_FIXED_HIGH);			 
				vIO32WriteFldAlign(PWM_SCAN_06, 1, REG_PWM_SCAN2_FIXED_LOW);					 			 			 			
			}	
			else
			{
				vIO32WriteFldAlign(PWM_SCAN_06, 0, REG_PWM_SCAN2_FIXED_HIGH);			 
				vIO32WriteFldAlign(PWM_SCAN_06, 0, REG_PWM_SCAN2_FIXED_LOW);
			}
			break;
		case SrcPWM3:
			if (mode==1)            
            {			 
	            vIO32WriteFldAlign(PWM_SCAN_10, 1, REG_PWM_SCAN3_FIXED_HIGH);			 
	            vIO32WriteFldAlign(PWM_SCAN_10, 0, REG_PWM_SCAN3_FIXED_LOW);		 			 			 			             
            }			
            else if (mode==2)		
            {			 
				vIO32WriteFldAlign(PWM_SCAN_10, 0, REG_PWM_SCAN3_FIXED_HIGH);			 
				vIO32WriteFldAlign(PWM_SCAN_10, 1, REG_PWM_SCAN3_FIXED_LOW);					 			 			 			
			}
			else
			{
				vIO32WriteFldAlign(PWM_SCAN_10, 0, REG_PWM_SCAN3_FIXED_HIGH);			 
				vIO32WriteFldAlign(PWM_SCAN_10, 0, REG_PWM_SCAN3_FIXED_LOW);
			}
			break;
		case SrcPWM4:
			if (mode==1)            
            {			 
	            vIO32WriteFldAlign(PWM_SCAN_10, 1, REG_PWM_SCAN4_FIXED_HIGH);			 
	            vIO32WriteFldAlign(PWM_SCAN_10, 0, REG_PWM_SCAN4_FIXED_LOW);		 			 			 			             
            }			
            else if (mode==2)		
            {			 
				vIO32WriteFldAlign(PWM_SCAN_10, 0, REG_PWM_SCAN4_FIXED_HIGH);			 
				vIO32WriteFldAlign(PWM_SCAN_10, 1, REG_PWM_SCAN4_FIXED_LOW);					 			 			 			
			}	
			else
			{
				vIO32WriteFldAlign(PWM_SCAN_10, 0, REG_PWM_SCAN4_FIXED_HIGH);			 
				vIO32WriteFldAlign(PWM_SCAN_10, 0, REG_PWM_SCAN4_FIXED_LOW);
			}
			break;
		case SrcPWM5:
			if (mode==1)            
            {			 
	            vIO32WriteFldAlign(PWM_SCAN_11, 1, REG_PWM_SCAN5_FIXED_HIGH);			 
	            vIO32WriteFldAlign(PWM_SCAN_11, 0, REG_PWM_SCAN5_FIXED_LOW);		 			 			 			             
            }			
            else if (mode==2)			
            {			 
				vIO32WriteFldAlign(PWM_SCAN_11, 0, REG_PWM_SCAN5_FIXED_HIGH);			 
				vIO32WriteFldAlign(PWM_SCAN_11, 1, REG_PWM_SCAN5_FIXED_LOW);					 			 			 			
			}
			else
			{
				vIO32WriteFldAlign(PWM_SCAN_11, 0, REG_PWM_SCAN5_FIXED_HIGH);			 
				vIO32WriteFldAlign(PWM_SCAN_11, 0, REG_PWM_SCAN5_FIXED_LOW);
			}
			break;
		case SrcPWM6:
			if (mode==1)            
            {			 
	            vIO32WriteFldAlign(PWM_SCAN_11, 1, REG_PWM_SCAN6_FIXED_HIGH);			 
	            vIO32WriteFldAlign(PWM_SCAN_11, 0, REG_PWM_SCAN6_FIXED_LOW);		 			 			 			             
            }			
            else if (mode==2)			
            {			 
				vIO32WriteFldAlign(PWM_SCAN_11, 0, REG_PWM_SCAN6_FIXED_HIGH);			 
				vIO32WriteFldAlign(PWM_SCAN_11, 1, REG_PWM_SCAN6_FIXED_LOW);					 			 			 			
			}
			else
			{
				vIO32WriteFldAlign(PWM_SCAN_11, 0, REG_PWM_SCAN6_FIXED_HIGH);			 
				vIO32WriteFldAlign(PWM_SCAN_11, 0, REG_PWM_SCAN6_FIXED_LOW);
			}
			break;
		case SrcPWM7:
			if (mode==1)            
            {			 
	            vIO32WriteFldAlign(PWM_SCAN_30, 1, REG_PWM_SCAN7_FIXED_HIGH);			 
	            vIO32WriteFldAlign(PWM_SCAN_30, 0, REG_PWM_SCAN7_FIXED_LOW);		 			 			 			             
            }			
            else if (mode==2)			
            {			 
				vIO32WriteFldAlign(PWM_SCAN_30, 0, REG_PWM_SCAN7_FIXED_HIGH);			 
				vIO32WriteFldAlign(PWM_SCAN_30, 1, REG_PWM_SCAN7_FIXED_LOW);					 			 			 			
			}
			else
			{
				vIO32WriteFldAlign(PWM_SCAN_30, 0, REG_PWM_SCAN7_FIXED_HIGH);			 
				vIO32WriteFldAlign(PWM_SCAN_30, 0, REG_PWM_SCAN7_FIXED_LOW);
			}
			break;
		case SrcPWM8:
			if (mode==1)            
            {			 
	            vIO32WriteFldAlign(PWM_SCAN_37, 1, REG_PWM_SCAN8_FIXED_HIGH);			 
	            vIO32WriteFldAlign(PWM_SCAN_37, 0, REG_PWM_SCAN8_FIXED_LOW);		 			 			 			             
            }			
            else if (mode==2)			
            {			 
				vIO32WriteFldAlign(PWM_SCAN_37, 0, REG_PWM_SCAN8_FIXED_HIGH);			 
				vIO32WriteFldAlign(PWM_SCAN_37, 1, REG_PWM_SCAN8_FIXED_LOW);					 			 			 			
			}
			else
			{
				vIO32WriteFldAlign(PWM_SCAN_37, 0, REG_PWM_SCAN8_FIXED_HIGH);			 
				vIO32WriteFldAlign(PWM_SCAN_37, 0, REG_PWM_SCAN8_FIXED_LOW);
			}
			break;
		default:
            ASSERT(channel <= SrcPWM8);
	}
}

UINT8 u1DrvGetScanFixHighLow(UINT8 channel)
{
	UINT8 mode;
	mode = 0;
	
	switch(channel)
    {
        case SrcPWM1:
			if (IO32ReadFldAlign(PWM_SCAN_06,REG_PWM_SCAN1_FIXED_HIGH))            
            {	
            	mode = 1;		 			 			 			             
            }			
            else if (IO32ReadFldAlign(PWM_SCAN_06,REG_PWM_SCAN1_FIXED_LOW))			
            {			 
	            mode = 2;					 			 			 			
            }
			else
			{
            	mode = 0;
			}
			break;
		case SrcPWM2:
			if (IO32ReadFldAlign(PWM_SCAN_06,REG_PWM_SCAN2_FIXED_HIGH))            
            {	
            	mode = 1;		 			 			 			             
            }			
            else if (IO32ReadFldAlign(PWM_SCAN_06,REG_PWM_SCAN2_FIXED_LOW))			
            {			 
	            mode = 2;					 			 			 			
            }
			else
			{
            	mode = 0;
			}
			break;
		case SrcPWM3:
			if (IO32ReadFldAlign(PWM_SCAN_10,REG_PWM_SCAN3_FIXED_HIGH))            
            {	
            	mode = 1;		 			 			 			             
            }			
            else if (IO32ReadFldAlign(PWM_SCAN_10,REG_PWM_SCAN3_FIXED_LOW))			
            {			 
	            mode = 2;					 			 			 			
            }
			else
			{
            	mode = 0;
			}
			break;
		case SrcPWM4:
			if (IO32ReadFldAlign(PWM_SCAN_10,REG_PWM_SCAN4_FIXED_HIGH))            
            {	
            	mode = 1;		 			 			 			             
            }			
            else if (IO32ReadFldAlign(PWM_SCAN_10,REG_PWM_SCAN4_FIXED_LOW))			
            {			 
	            mode = 2;					 			 			 			
            }
			else
			{
            	mode = 0;
			}
			break;
		case SrcPWM5:
			if (IO32ReadFldAlign(PWM_SCAN_11,REG_PWM_SCAN5_FIXED_HIGH))            
            {	
            	mode = 1;		 			 			 			             
            }			
            else if (IO32ReadFldAlign(PWM_SCAN_11,REG_PWM_SCAN5_FIXED_LOW))			
            {			 
	            mode = 2;					 			 			 			
            }
			else
			{
            	mode = 0;
			}
			break;
		case SrcPWM6:
			if (IO32ReadFldAlign(PWM_SCAN_11,REG_PWM_SCAN6_FIXED_HIGH))            
            {	
            	mode = 1;		 			 			 			             
            }			
            else if (IO32ReadFldAlign(PWM_SCAN_11,REG_PWM_SCAN6_FIXED_LOW))			
            {			 
	            mode = 2;					 			 			 			
            }
			else
			{
            	mode = 0;
			}
			break;
		case SrcPWM7:
			if (IO32ReadFldAlign(PWM_SCAN_30,REG_PWM_SCAN7_FIXED_HIGH))            
            {	
            	mode = 1;		 			 			 			             
            }			
            else if (IO32ReadFldAlign(PWM_SCAN_30,REG_PWM_SCAN7_FIXED_LOW))			
            {			 
	            mode = 2;					 			 			 			
            }
			else
			{
            	mode = 0;
			}
			break;
		case SrcPWM8:
			if (IO32ReadFldAlign(PWM_SCAN_37,REG_PWM_SCAN8_FIXED_HIGH))            
            {	
            	mode = 1;		 			 			 			             
            }			
            else if (IO32ReadFldAlign(PWM_SCAN_37,REG_PWM_SCAN8_FIXED_LOW))			
            {			 
	            mode = 2;					 			 			 			
            }
			else
			{
            	mode = 0;
			}
			break;
		default:
            ASSERT(channel <= SrcPWM8);
	}
	return (mode);
}

void vDrvSetISOnOff(UINT8 u1Src, UINT8 u1OnOff)
{
#if defined(CC_MT5396) || defined(CC_MT5368)

    switch(u1Src)
    {
        case MINILVDS_IS_HSYNC:
        case MINILVDS_IS_VSYNC:
            vIO32WriteFldAlign(VINTF_P20, u1OnOff, VINTF_INV_SYNC_START);
            break;

        default:
            ASSERT(u1Src <= MINILVDS_IS_VSYNC);
    }

#endif
}

void vDrvScanPWMAutoTest(void)
{
	UINT32 u4Devider=1;
	static UINT32 u4High1 ;
	u4Devider = _u4ScanPWMTesFreq;

	if (_fgScanPWMAuto==TRUE)
	{
		if (_u1ScanPWMTestChg==1)
		{
			if (_u4ScanPWMTestHigh>_u4ScanPWMTestBottom)
			{
				u4High1 = _u4ScanPWMTestHigh/u4Devider;
				_u4ScanPWMTestLow = (1000 - (_u4ScanPWMTestHigh))/u4Devider;
				LOG(6, "#1 u4Devider=%d, u4High1=%d, Low=%d\n", u4Devider,u4High1,_u4ScanPWMTestLow);
				vDrvSetScanPWMSW(_u1ScanPWMTestSrc,_i4ScanPWMTestStart,u4High1,_u4ScanPWMTestLow);
				_u4ScanPWMTestHigh--;
			}
			if (_u4ScanPWMTestHigh == _u4ScanPWMTestBottom)
			{
				_u4ScanPWMTestHigh = _u4ScanPWMTestTop;
				vDrvSetScanPWMStructData(_u1ScanPWMTestSrc,_i4ScanPWMTestStart,_u4ScanPWMTestHigh/u4Devider,((1000 - (_u4ScanPWMTestHigh))/u4Devider));
			}
		}
		else if (!_u1ScanPWMTestChg)
		{
			if (_u4ScanPWMTestHigh<_u4ScanPWMTestTop)
			{
				u4High1 = _u4ScanPWMTestHigh/u4Devider;
				_u4ScanPWMTestLow = (1000 - (_u4ScanPWMTestHigh))/u4Devider;
				LOG(6, "#0 u4Devider=%d, u4High1=%d, Low=%d\n", u4Devider,u4High1,_u4ScanPWMTestLow);
				vDrvSetScanPWMSW(_u1ScanPWMTestSrc,_i4ScanPWMTestStart,u4High1,_u4ScanPWMTestLow);
				_u4ScanPWMTestHigh++;
			}
			if (_u4ScanPWMTestHigh == _u4ScanPWMTestTop)
			{
				_u4ScanPWMTestHigh = _u4ScanPWMTestBottom;
				vDrvSetScanPWMStructData(_u1ScanPWMTestSrc,_i4ScanPWMTestStart,_u4ScanPWMTestHigh/u4Devider,((1000 - (_u4ScanPWMTestHigh))/u4Devider));
			}
		}
	}
}

void vDrvSetISMaskNum(UINT8 u1Src, UINT8 u1MaskNum)
{
#if defined(CC_MT5396) || defined(CC_MT5368)

    switch(u1Src)
    {
        case MINILVDS_IS_HSYNC:
            vIO32WriteFldAlign(VINTF_P21, u1MaskNum, VINTF_IS_MASK_LEN_H);
            break;

        case MINILVDS_IS_VSYNC:
            vIO32WriteFldAlign(VINTF_P23, u1MaskNum, VINTF_IS_MASK_LEN_V);
            break;

        default:
            ASSERT(u1Src <= MINILVDS_IS_VSYNC);
    }

#endif
}

void vDrvSetISInputInverse(UINT8 u1Src, UINT8 u1Enable)
{
#if defined(CC_MT5396) || defined(CC_MT5368)

    switch(u1Src)
    {
        case MINILVDS_IS_HSYNC:
            vIO32WriteFldAlign(VINTF_P20, u1Enable, VINTF_IS_POL_H);
            break;

        case MINILVDS_IS_VSYNC:
            vIO32WriteFldAlign(VINTF_P22, u1Enable, VINTF_IS_POL_V);
            break;

        default:
            ASSERT(u1Src <= MINILVDS_IS_VSYNC);
    }

#endif
}

void vDrvSetISDelayOnOff(UINT8 u1Src, UINT8 u1OnOff)
{
#if defined(CC_MT5396) || defined(CC_MT5368)

    switch(u1Src)
    {
        case MINILVDS_IS_HSYNC:
        case MINILVDS_IS_VSYNC:
            vIO32WriteFldAlign(VINTF_P20, u1OnOff, VINTF_IS_FORCE_DELAY);
            break;

        default:
            ASSERT(u1Src <= MINILVDS_IS_VSYNC);
    }

#endif
}

void vDrvSetISDelayNum(UINT8 u1Src, UINT16 u2DelayNum)
{
#if defined(CC_MT5396) || defined(CC_MT5368)

    switch(u1Src)
    {
        case MINILVDS_IS_HSYNC:
            vIO32WriteFldAlign(VINTF_P20, u2DelayNum, VINTF_IS_DELAY_H);
            break;

        case MINILVDS_IS_VSYNC:
            vIO32WriteFldAlign(VINTF_P22, u2DelayNum, VINTF_IS_DELAY_V);
            break;

        default:
            ASSERT(u1Src <= MINILVDS_IS_VSYNC);
    }

#endif
}

void vDrvSetISHoldOnOff(UINT8 u1Src, UINT8 u1OnOff)
{
#if defined(CC_MT5396) || defined(CC_MT5368)

    switch(u1Src)
    {
        case MINILVDS_IS_HSYNC:
            vIO32WriteFldAlign(VINTF_P20, u1OnOff, VINTF_IS_HOLD_EN_H);
            break;

        case MINILVDS_IS_VSYNC:
            vIO32WriteFldAlign(VINTF_P22, u1OnOff, VINTF_IS_HOLD_EN_V);
            break;

        default:
            ASSERT(u1Src <= MINILVDS_IS_VSYNC);
    }

#endif
}

void vDrvSetISRelation(UINT8 u1Src, UINT8 u1SyncCnt, UINT8 u1ISSyncCnt)
{
#if defined(CC_MT5396) || defined(CC_MT5368)
    UINT32 u4Total =0, u4ISTotal=0;
    UINT8 u1Scatter =0;

    if(u1SyncCnt >= 15) u1SyncCnt = 15;

    if(u1ISSyncCnt >= 15) u1ISSyncCnt = 15;

    if(u1SyncCnt == 0) u1SyncCnt = 1;

    if(u1ISSyncCnt == 0) u1ISSyncCnt = 1;

    switch(u1Src)
    {
        case MINILVDS_IS_HSYNC:
            u4Total = wDrvGetOutputHTotal();
            break;

        case MINILVDS_IS_VSYNC:
            u4Total = wDrvGetOutputVTotal();
            break;

        default:
            ASSERT(u1Src <= MINILVDS_IS_VSYNC);
    }

    u4ISTotal = (u4Total * u1SyncCnt) / u1ISSyncCnt;
    u1Scatter = (u4Total * u1SyncCnt) % u1ISSyncCnt;

    switch(u1Src)
    {
        case MINILVDS_IS_HSYNC:
            vIO32WriteFldAlign(VINTF_P20, u1ISSyncCnt, VINTF_IS_DIV_H);
            vIO32WriteFldAlign(VINTF_P20, u1SyncCnt, VINTF_IS_MUL_H);
            vIO32WriteFldAlign(VINTF_P20, u1Scatter, VINTF_SYNC_TOTAL_PLUS_1_H);
            vIO32WriteFldAlign(VINTF_P21, u4ISTotal, VINTF_IS_PWM_SYNC_TOTAL_H);
            break;

        case MINILVDS_IS_VSYNC:
            vIO32WriteFldAlign(VINTF_P22, u1ISSyncCnt, VINTF_IS_DIV_V);
            vIO32WriteFldAlign(VINTF_P22, u1SyncCnt, VINTF_IS_MUL_V);
            vIO32WriteFldAlign(VINTF_P23, u1Scatter, VINTF_IS_LEN_PLUS_1_V);
            vIO32WriteFldAlign(VINTF_P23, u4ISTotal, VINTF_IS_PWM_SYNC_TOTAL_V);
            break;

        default:
            ASSERT(u1Src <= MINILVDS_IS_VSYNC);
    }

#endif
}

#ifdef CC_SUPPORT_STR
//-----------------------------------------------------------------------------
/** PWM_pm_str_suspend
 */
//-----------------------------------------------------------------------------
void PWM_pm_str_suspend(void)
{
	// power off
	CKGEN_WRITE32(0x1CC, CKGEN_READ32(0x1CC) & ~(1U << 10));

}


//-----------------------------------------------------------------------------
/** PWM_pm_str_resume
 */
//-----------------------------------------------------------------------------
void PWM_pm_str_resume(void)
{

	CKGEN_WRITE32(0x1CC, CKGEN_READ32(0x1CC) | (1U << 10));

}
#endif // CC_SUPPORT_STR


