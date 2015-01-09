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
 * $RCSfile: pdwnc_hw.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file pdwnc_if.c
 *  pdwnc_if.c The PDWNC (Power DoWN Controller) interface
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "ir_if.h"
#include "rtc_if.h"
#include "x_pinmux.h"
#include "x_pdwnc.h"
#include "x_bim.h"
#include "x_assert.h"
#include "x_os.h"
#include "x_ckgen.h"
#include "x_timer.h"
#include "x_hal_arm.h"
#include "x_gpio.h"
#include "drvcust_if.h"
#include "pdwnc_config.h"
#include "pdwnc_debug.h"
#include "pdwnc_hw.h"
#include "pdwnc_t8032.h"
#include "pdwnc_gpio.h"
#include "pdwnc_srvad.h"
#include "pdwnc_entry.h"
#include "pdwnc_drvif.h"
#include "sif_if.h"
#include "eeprom_if.h"
#include "pdwnc_if.h"
#include "drv_adcset.h"
#include "ostg_if.h"
#include "drv_display.h"

#include "../net/ethernet/ether_reg.h"

#ifndef CC_MTK_LOADER
#ifdef TIME_MEASUREMENT
#include "x_time_msrt.h"
#include "u_time_msrt_name.h"
#endif
#endif
//#undef PDWNC_WRITE32
//#undef PDWNC_READ32

//-----------------------------------------------------------------------------
// Configuration
//-----------------------------------------------------------------------------
#ifdef CC_MTK_LOADER
//#undef x_crit_start
//#undef x_crit_end
//#define x_crit_start() 0
//#define x_crit_end(x) UNUSED(x)
#define CRIT_STATE_T UINT8
#else
#include "x_os.h"
#include "sif_if.h"
#endif

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------

#define VGAWAKEUP_VRANGE_U 0xfff
#define VGAWAKEUP_VRANGE_L 0x1c
//todo vga user check this
#if 1//24MHz---after remy
#define CSYNC_CONT_THU_DEFAULT   0x175//(0x01a4/9*8)
#define CSYNC_CONT_THL_DEFAULT   0xe2//(0x00ff/9*8)
#define CSYNC_CONT_THH_DEFAULT	0x16e//(0x19c/9*8)
#define VGAWAKEUP_HRANGE_U	(0x6ff)//24m/13k=1846=0x736
#define VGAWAKEUP_HRANGE_L	(0xbf)//24m/100k=240=0xf0
//this paramter is the time to try for each sync type , should bigger than V_STABLE_TH 
#define VGAWAKEUP_FREERUN_CNT_TH  (0x2a0000)   
#else//27MHz
#define CSYNC_CONT_THU_DEFAULT   (0x01a4)
#define CSYNC_CONT_THL_DEFAULT   (0x00ff)
#define CSYNC_CONT_THH_DEFAULT	(0x19c)
#define VGAWAKEUP_HRANGE_U	(0x700)//1792
#define VGAWAKEUP_HRANGE_L	(0xc0)//192
//this paramter is the time to try for each sync type , should bigger than V_STABLE_TH 
#define VGAWAKEUP_FREERUN_CNT_TH  (0x2fffff)   // almost = 0x4cccc * 9
#endif

//-----------------------------------------------------------------------------
// Macros
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------
#ifndef CC_MTK_LOADER
// PDWNC module sub-interrupt routine list.
static volatile x_os_isr_fct _varIntList[MAX_PDWNC_INT_ID_2+1];
#endif /* CC_MTK_LOADER */

static INT32 _gi4PDWNCInit = 0;
static UINT32 _fgPowerDownByAcCut = 0;

static BOOL _fgIrLedEnable = TRUE;
static PDWNC_SET_POWER_DOWN_REASON_T _grPowerDownReason;

static BOOL _fgSuspendWakeupReasonUp = FALSE;

#if defined(CC_SUPPORT_STR_CORE_OFF)
static UINT32 _u4SuspendEn = 0;
static UINT32 _u4SuspendEvent = 0;
#endif

#ifdef NDEBUG
#ifdef WATCHDOG_DETECT_ENABLE
BOOL _bgWatchdogDetect = TRUE;
#endif /* WATCHDOG_DETECT_ENABLE */        
#endif

// static PDWNC_FuncTbl *pgrPDWNC_Func = NULL;
#define pgrPDWNC_Func   (PDWNC_EXT_GetFunc())


//-----------------------------------------------------------------------------
// Imported functions 
//-----------------------------------------------------------------------------
//EXTERN void HAL_Delay_us(UINT32 u4Micros);
extern void PDWNC_ResumeServoADC(void);

//-----------------------------------------------------------------------------
// Prototypes 
//-----------------------------------------------------------------------------




//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------


#ifndef CC_MTK_LOADER
extern void _PdwncGpioIsr(UINT16 u2IntIndex);
//-----------------------------------------------------------------------------
/** _PdwncHwIsr() is the interrupt routine of PDWNC module.
 *  @param u2VctId is not used, it should be VECTOR_PDWNC.
 */
//-----------------------------------------------------------------------------
static void _PdwncHwIsr(UINT16 u2VctId)
{
    UINT32 i = 0, u4Val;
    UINT32 u4ValGpio;
    UNUSED(u2VctId);
    
    // Should we use this?? XXX discussion this.    
    u4Val = u4IO32Read4B(PDWNC_INTSTA) & u4IO32Read4B(PDWNC_ARM_INTEN);
	// gpio init 
	u4ValGpio = u4IO32Read4B(PDWNC_INTSTA_2) & u4IO32Read4B(PDWNC_ARM_INTEN_2);
	
	if(u4Val)
    for (i = 0 ; i < MAX_PDWNC_INT_ID; i++)
    {
        if (u4Val & _PINT(i))
        {
            if (_varIntList[i])
            {
                _varIntList[i](i);
            }
            else
            {
                LOG(11, "PDWNC(0x%08x) interrupt %d no routine hook\n",
                    u4Val, i);
                // Still need to clear this non-hook interrupt.
            }
			vIO32Write4B(PDWNC_INTCLR, _PINT(i));	
        }
    }

	#if defined(CC_MT5882)
	if(u4ValGpio)
    for (i = 0 ; i < TOTAL_PDWNC_GPIO_INT_NUM; i++)
    {
        if (u4ValGpio & _PINT(i))
        {
			//gpio interrupt always call this function
			_PdwncGpioIsr(i);
        }
    }
	#elif defined(CC_MT5399)
	if(u4ValGpio)
	for (i = MAX_PDWNC_INT_ID ; i < MAX_PDWNC_INT_ID_2 ; i++)
	{
        if (u4ValGpio & _PINT(i - MAX_PDWNC_INT_ID))
        {
            if (_varIntList[i])
            {
                _varIntList[i](i);
            }
            else
            {
                LOG(11, "PDWNC(0x%08x) interrupt %d no routine hook\n",
                    u4ValGpio, i);
                // Still need to clear this non-hook interrupt.
            }
			vIO32Write4B(PDWNC_INTCLR_2, _PINT(i - MAX_PDWNC_INT_ID));	
        }
	}
	#endif
	
}

//#define VECTOR_WDT 7 

static volatile PFN_WDT_CALLBACK _afnWdtCallback[NORMAL_GPIO_NUM] = {NULL, NULL, NULL};//NORMAL_GPIO_NUM for SW gpio polling thread

static void PDWNC_WDT_Callback(INT32 i4Wdt)
{
    Printf("\nPDWNC WDT%d Interrupt!\n", i4Wdt);

    BspDumpSystemTasksState();
    Printf("Dump complete, good-bye\n");

    // Delay to make sure last bits get write to UART.
    HAL_Delay_us(100*1000);

    vIO32Write4B((PDWNC_WDTRST0 + (i4Wdt * 4)), 0x0fffffff);	//set reset wait time to 0 (0x0fffffff - 0x0fffffff), let watchdog reset at once
}

INT32 WDT_ISR_Reg(UINT32 u4Idx, PFN_WDT_CALLBACK pfnCallback)
{
#ifndef CC_MTK_LOADER
	if(u4Idx > 2 )
		return -1;
	
    if (pfnCallback != NULL)
    {
    	vIO32Write4B(PDWNC_WDTINT_EN, u4IO32Read4B(PDWNC_WDTINT_EN) | (1 << u4Idx));
        _afnWdtCallback[u4Idx] = pfnCallback;
    }
	else
	{
    	vIO32Write4B(PDWNC_WDTINT_EN, u4IO32Read4B(PDWNC_WDTINT_EN) & (~(1 << u4Idx)));
        _afnWdtCallback[u4Idx] = NULL;
	}
#endif
	return 0;
}

static void _WDTIsr(UINT16 u2VctId)
{
	UINT32 i, u4Val;
	UNUSED(u2VctId);
	
	u4Val = u4IO32Read4B(PDWNC_WDTINT_STA);
	for(i = 0; i < 3; i++)
	{
		if(u4Val & (1 << i))
			if(_afnWdtCallback[i] != NULL)
				_afnWdtCallback[i](i);
	}
	//vIO32Write4B(PDWNC_WDTINT_CLR, u4Val);	//use this will  clear the int and "stop watchdog reset"!!!
	
	//VERIFY(BIM_ClearIrq(VECTOR_WDT));
}
#define WATCHDOG_SEC_CNT(x)  (0x0fffffffU - (x * 0x00300000))	//x should less than 9.48s, it is for 3M
#define WATCHDOG_MSEC_CNT(x)  (0x0fffffffU - (((x/100) * 0x00300000)/10))

void PDWNC_WatchDogPreInit(void)
{
    UINT32 u4Sec, u4Val, i;

	//set watchdog reset wait time
	u4Sec = DRVCUST_OptGet(eWatchdogResetWaitTime);	
	
	if(IO32ReadFldAlign(PDWNC_PDMODE, FLD_DEEP_SLEEP))// 3Mhz
	{
		u4Val = WATCHDOG_SEC_CNT(u4Sec);
	}
	else	//27M, default
	{
		u4Val = WATCHDOG_SEC_CNT(u4Sec * 9);
	}
	vIO32Write4B(PDWNC_WDTRST0, u4Val);
	vIO32Write4B(PDWNC_WDTRST1, u4Val);
	vIO32Write4B(PDWNC_WDTRST2, u4Val);

	//watchdog callback Reg 
	for(i=0; i<3; i++)
	{
		if((WDT_ISR_Reg(i, PDWNC_WDT_Callback)) < 0)
		{
			Printf("Reg WDT%d interrupt callback fail..\n", i);
			return;
		}
	}
	Printf("Reg WDT interrupt callback successful..\n");
}
//-----------------------------------------------------------------------------
/** _PdwncHwSetIsr() is the interrupt enable/disable routine of PDWNC module.
 *  @param fgSet TRUE to enable interrupt, otherwise disable.
 */
//-----------------------------------------------------------------------------
INT32 _PdwncHwSetIsr(BOOL fgSet)
{
    static BOOL fgCurSet = FALSE;
    INT32 i4Val;
    void (* pfnOldIsr)(UINT16);

    if (fgSet && (fgSet!=fgCurSet))
    {
    	/* enable irq. */
        fgCurSet = fgSet;
        i4Val = x_reg_isr(VECTOR_PDWNC, _PdwncHwIsr, &pfnOldIsr);
        if (i4Val != OSR_OK)
        {
            return FALSE;
        }
        VERIFY(BIM_ClearIrq(VECTOR_PDWNC));
		
        i4Val = x_reg_isr(VECTOR_WDT, _WDTIsr, &pfnOldIsr);
        if (i4Val != OSR_OK)
        {
            return FALSE;
        }
        VERIFY(BIM_ClearIrq(VECTOR_WDT));
    }
    else if (!fgSet && (fgSet!=fgCurSet))
    {  
    	/* disable irq. */
        fgCurSet = fgSet;
        i4Val = x_reg_isr(VECTOR_PDWNC, NULL, &pfnOldIsr);
        if (i4Val != OSR_OK)
        {
            return FALSE;
        }

		i4Val = x_reg_isr(VECTOR_WDT, NULL, &pfnOldIsr);
		if (i4Val != OSR_OK)
		{
			return FALSE;
		}
    }
    return TRUE;
}

#ifdef CC_DC_ONOFF_TEST_ENABLE
#define SYSTEM_SHUTDOWN_DELAY       5000
#define WAKEUP_TIMER                3       // system wake up timer by RTC

static HANDLE_T _hShutdownHandle = (HANDLE_T)NULL;

static void _PDWNC_RebootHandle(void *pvArg)
{
    UINT64 u8Time;
    
    UNUSED(pvArg);
    
    x_thread_delay(SYSTEM_SHUTDOWN_DELAY);
   
    printf("[RebootHandle]Get time date\n");
    RTC_GetTimeDate(&u8Time);
    u8Time += WAKEUP_TIMER;
    RTC_SetAlertTime(u8Time);
        
    // Enable RTC wakeup            
    vIO32WriteFldAlign(PDWNC_WAKEN, 0x1, FLD_RTC_WAKEN);
    
    printf("[RebootHandle]Enter power down\n");
    PDWNC_EnterPowerDown(WAK_IRRX | WAK_RTC, (UINT32)PDWNC_POWER_DOWN_UNKNOWN);
    return;
}
#endif

#ifdef CC_CPU_SPEED_TEST_ENABLE
#include "hw_hpcpll.h"
#define SYSTEM_SHUTDOWN_DELAY       5000 * 60 //  test time : 5 minutes

static HANDLE_T _hShutdownHandle = (HANDLE_T)NULL;

static void _PDWNC_RebootHandle(void *pvArg)
{
    UINT32 u4Clock, u4CPUPLL_FBDIV;
    UNUSED(pvArg);
    
    u4Clock = BSP_GetClock(SRC_CPU_CLK, NULL, NULL, NULL, NULL);
    printf("\n[RebootHandle] CPUPLL = %dHz test start !\n", u4Clock);
    
    x_thread_delay(SYSTEM_SHUTDOWN_DELAY);
   
    printf("\n[RebootHandle] CPUPLL = %dHz test pass !\n", u4Clock);
    u4CPUPLL_FBDIV = IO32ReadFldAlign(REG_HPCPLL_CFG0, FLD_RG_HPCPLL_FBDIV) + 4;
    vIO32Write4B(0xf0008154, u4CPUPLL_FBDIV);
    vIO32Write4B(0xf0008150, 0xFFFFFFFF);       // enable CPU speed test
    printf("[RebootHandle] Next FBDIV value of CPUPLL = %x\n", u4CPUPLL_FBDIV);
    
    printf("[RebootHandle] System Reboot !!\n");
    PDWNC_Reboot();
    return;
}
#endif

#endif /* CC_MTK_LOADER */

static void _PdwncSetupIRRX(UINT32 u4PowerDownEn)
{
    INT32 i4Val;
    UINT32 u4Val;

    if ((u4PowerDownEn & WAK_IRRX) == 0)
    {
        return;
    }
    i4Val = DRVCUST_InitQuery(eIrrxType, &u4Val);
    if (i4Val < 0)
    {
        u4Val = IRRX_RC5_TYPE;
    }

    // To write u4PowerDownEn for Wakeup key setting.

    vIO32Write4B(PDWNC_WAKEN, u4PowerDownEn);

    // Configuration
    i4Val = IRRX_InitMtkIrRegSetup(u4Val);
#ifndef CC_MTK_LOADER
	#ifdef CC_QUIET_HOT_BOOT_SUPPORT
	IRRX_IsQuietHotBootClr();
	#endif
#endif
    UNUSED(i4Val);
}

/*static*/ void _PdwncSetupVGA(UINT32 u4PowerDownEn)
{

	if((u4PowerDownEn & WAK_VGA) == 0)  
    {

        LOG(0,"Disable VGA wakeup\n");
			
        HAL_Delay_us(5); 
        vIO32WriteFldAlign(PDWNC_VGACFG1, 0x0, FLD_RG_HSYNC_EN);  
		

        vIO32WriteFldAlign(PDWNC_VGACFG3, 0x1, FLD_RG_SOGY_SINK_PWD);        
        vIO32WriteFldAlign(PDWNC_VGACFG3, 0x1, FLD_RG_SOGY_SORS_PWD);        
        vIO32WriteFldAlign(PDWNC_VGACFG1, 0x1, FLD_RG_SYNC_PWD);  
		
        vIO32WriteFldMulti(PDWNC_VGACFG1, P_Fld(0,FLD_RG_SOG_EN)|P_Fld(0,FLD_RG_SOY0_EN)|P_Fld(0,FLD_RG_SOY1_EN));
        vIO32WriteFldMulti(PDWNC_VGACFG1, P_Fld(0,FLD_RG_TSOGY_EN)|P_Fld(0,FLD_RG_VSYNC_EN));

			
        HAL_Delay_us(5);          

        // Turn on VGA_TESTBUF_PWD        
        vIO32WriteFldAlign(PDWNC_VGACFG2, 0x1, FLD_RG_VGA_TESTBUF_PWD);
        HAL_Delay_us(5);     

        // Disable VGA sync det clock
        vIO32WriteFldAlign(PDWNC_CLKPDN, 0x1, FLD_VGAPD);
        HAL_Delay_us(5);     

		vIO32WriteFldAlign(PDWNC_VGACFG0, 0x1, FLD_RG_VMUX_PWD);
		
		vIO32WriteFldAlign(PDWNC_VGACFG0, 0x0, FLD_RG_AUTOBIAS_EN);//20100415, sc hwu suggestion		
		vIO32WriteFldAlign(PDWNC_PDMISC, 0x1, FLD_VGA_SYNC_DIS);//20100415, chiahsien liu suggestion		

        return;
    }

#ifndef CC_MTK_LOADER
        vDrvSYNCSlicerWakeup(1);
#endif
        
		// David: force enable VGA sync circuit clock 20090219
		vIO32WriteFldAlign(PDWNC_CLKPDN, 0x0, FLD_VGAPD);
		vIO32WriteFldAlign(PDWNC_PDMISC, 0x0, FLD_VGA_SYNC_DIS);
		vIO32WriteFldAlign(PDWNC_VGACFG1, 0x0, FLD_RG_SYNC_PWD); 
		vIO32WriteFldAlign(PDWNC_VGA_WAKE_UP_CONTROL_0_REGISTER, 0x1, FLD_AUTO_SWITCH_EN);

	 	vIO32WriteFldAlign(PDWNC_VGA_WAKE_UP_CONTROL_0_REGISTER, VGAWAKEUP_HRANGE_U, FLD_HRANGE_U);
	 	vIO32WriteFldAlign(PDWNC_VGA_WAKE_UP_CONTROL_0_REGISTER, VGAWAKEUP_HRANGE_L, FLD_HRANGE_L);
		HAL_Delay_us(5);
		vIO32WriteFldAlign(PDWNC_VGA_WAKE_UP_CONTROL_1_REGISTER, VGAWAKEUP_VRANGE_U, FLD_VRANGE_U);
		vIO32WriteFldAlign(PDWNC_VGA_WAKE_UP_CONTROL_1_REGISTER, VGAWAKEUP_VRANGE_L, FLD_VRANGE_L);
		HAL_Delay_us(5);


#ifdef CC_DISABLE_CSYNC_WAKEUP	
	vIO32WriteFldAlign(PDWNC_VGA_WAKE_UP_CONTROL_1_REGISTER, 0x1, FLD_C_SYNC_WAK_DIS);	
#endif
	HAL_Delay_us(5);
#ifdef CC_DISABLE_SOG_WAKEUP  	
        vIO32WriteFldAlign(PDWNC_VGA_WAKE_UP_CONTROL_1_REGISTER, 0x1, FLD_SOG_WAK_DIS);	
#endif
	HAL_Delay_us(5);
	vIO32WriteFldAlign(PDWNC_VGA_WAKE_UP_CONTROL_3_REGISTER, /*0x06*/0xF, FLD_H_DIFF_TH);
	vIO32WriteFldAlign(PDWNC_VGA_WAKE_UP_CONTROL_3_REGISTER, 0x02, FLD_V_STABLE_TH);
	vIO32WriteFldAlign(PDWNC_VGA_WAKE_UP_CONTROL_3_REGISTER, 0x80, FLD_H_STABLE_TH);
	HAL_Delay_us(5);
	vIO32WriteFldAlign(PDWNC_VGA_WAKE_UP_CONTROL_4_REGISTER, 0x3f, FLD_V_DIFF_TH);
	HAL_Delay_us(5);
	vIO32WriteFldAlign(PDWNC_VGA_WAKE_UP_CONTROL_5_REGISTER, (CSYNC_CONT_THL_DEFAULT & 0xff), FLD_CSYNC_CONT_THL);
	vIO32WriteFldAlign(PDWNC_VGA_WAKE_UP_CONTROL_5_REGISTER, (CSYNC_CONT_THL_DEFAULT >> 8), FLD_CYSNC_CT_THL);	
	vIO32WriteFldAlign(PDWNC_VGA_WAKE_UP_CONTROL_5_REGISTER, (CSYNC_CONT_THU_DEFAULT & 0xff), FLD_CSYNC_CONT_THU);
	vIO32WriteFldAlign(PDWNC_VGA_WAKE_UP_CONTROL_5_REGISTER, (CSYNC_CONT_THU_DEFAULT >> 8), FLD_CSYNC_CT_THU);	
	 HAL_Delay_us(5);
    
	vIO32WriteFldMulti(PDWNC_VGA_WAKE_UP_CONTROL_6_REGISTER, P_Fld(0, FLD_HLEN_USE_VMASKX)| P_Fld(0x208, FLD_VMASK_S)| P_Fld(0xA, FLD_VMASK_E));	

	HAL_Delay_us(5);
	vIO32WriteFldAlign(PDWNC_VGA_WAKE_UP_CONTROL_7_REGISTER, (CSYNC_CONT_THH_DEFAULT & 0xff), FLD_CSYNC_CONT_SLICE_THH);
	HAL_Delay_us(5);
	vIO32WriteFldAlign(PDWNC_VGA_WAKE_UP_CONTROL_8_REGISTER, (CSYNC_CONT_THH_DEFAULT >> 8), FLD_CSYNC_CONT_THH);
	vIO32WriteFldAlign(PDWNC_VGA_WAKE_UP_CONTROL_8_REGISTER, VGAWAKEUP_FREERUN_CNT_TH, FLD_FREERUN_CNT_TH);

	vIO32WriteFldAlign(PDWNC_VGA_WAKE_UP_CONTROL_8_REGISTER, 1, FLD_FREERUN_CNT_UNLOCK);


	LOG(0," set vga pdwnc module\n");
}

#ifndef CC_MTK_LOADER
static void _PdwncSetupRTC(const UINT32* pu4PowerDownEn)
{
    UINT32 u4RtcHw;

    ASSERT(pu4PowerDownEn != NULL);
    if ((*pu4PowerDownEn & WAK_RTC) == 0)
    {
        return;
    }
    
    if(DRVCUST_OptQuery(eRtcHw, &u4RtcHw) != 0)
    {
        return;
    }
    if(u4RtcHw == (INT32)RTC_INTERNAL)
    {    
    }
    else
    {
        RTC_SetWakeup((UINT32 *)pu4PowerDownEn);
    }

}
#endif /* CC_MTK_LOADER */


//-----------------------------------------------------------------------------
/* _PDWNC_SetupStrapping() to setup the PDWNC Gpio interrupt callback function, type,
 *      and state.
 *  @param i4Gpio should be between 0~7.
 *  @param eType should be one of enum GPIO_TYPE.
 *  @param pfnCallback the callback function.
 *  @retval 0 successful, -1 failed.
 */
//-----------------------------------------------------------------------------
void _PDWNC_SetupStrapping(void)
{
    return;
}

//-----------------------------------------------------------------------------
/* PDWNC_ReadWakeupReason() to read the wakeup reason 
 * 
 *  @param u1ErrorCode  Error code
 *  @retval   0		    SUCCESS.		
 *  @retval   1		    FAIL.
 */
//-----------------------------------------------------------------------------
UINT32 _PDWNC_ReadWakeupStatus(void)
{
    static UINT32 u4Status;
    static BOOL fgHasRead = FALSE;

	if(_fgSuspendWakeupReasonUp)//wakeup from suspend
    {
		fgHasRead = FALSE;
		_fgSuspendWakeupReasonUp = FALSE;
    }
    if(fgHasRead)
    {
        return u4Status;
    }
#ifdef CC_PDWNCONOFF_PATCH
	if(u4IO32Read4B(PDWNC_RESRV0) & 0x80000000)
	{
		u4Status = u4IO32Read4B(PDWNC_RESRV1);
	}
	else
	{
		u4Status = u4IO32Read4B(PDWNC_PDSTAT);
	}
#else
    u4Status = u4IO32Read4B(PDWNC_PDSTAT);
#endif
    fgHasRead = TRUE;
    
    return (u4Status);
}

//-----------------------------------------------------------------------------
/* PDWNC_ReadWatchDogStatus() to read watchdog reset status
 * 
 *  @param u1ErrorCode  Error code
 *  @retval   0		    SUCCESS.		
 *  @retval   1		    FAIL.
 */
//-----------------------------------------------------------------------------
UINT32 _PDWNC_ReadWatchDogStatus(void)
{
    static UINT32 u4Status;
    static BOOL fgHasRead = FALSE;

    if(fgHasRead)
    {
        return u4Status;
    }

    u4Status = (u4IO32Read4B(PDWNC_WDTCTL) & FLD_WDTALL_RST);

    fgHasRead = TRUE;
    
    return (u4Status);
}

#ifndef CC_MTK_LOADER
#if defined(NDEBUG) & defined(WATCHDOG_DETECT_ENABLE)
//-----------------------------------------------------------------------------
/* _PDWNC_SetWatchDogDetectState() to disable/enable the watchdog detect function
 * 
 *  @param u1Enable     the option of enable/disable watchdog detect function
 *  @retval   0		    SUCCESS.		
 *  @retval   1		    FAIL.
 */
//-----------------------------------------------------------------------------
UINT32 _PDWNC_SetWatchDogDetectState(BOOL u1Enable)
{
    CRIT_STATE_T rCritState;

    rCritState = x_crit_start();
    _bgWatchdogDetect = u1Enable;
    x_crit_end(rCritState);
    
    return 0;
}

//-----------------------------------------------------------------------------
/* _PDWNC_ReadWatchDogDetectStatus() to read the state of watchdog detection
 * 
 *  @param  void
 *  @retval   0		    Disable.		
 *  @retval   1		    Enable.
 */
//-----------------------------------------------------------------------------
UINT32 _PDWNC_ReadWatchDogDetectStatus(void)
{
    UINT32 u4Status;
    CRIT_STATE_T rCritState;
    
    rCritState = x_crit_start();
    u4Status = (UINT32)_bgWatchdogDetect;
    x_crit_end(rCritState);

    return (u4Status);
}
#endif /* defined(NDEBUG) & defined(WATCHDOG_DETECT_ENABLE) */        
#endif /* CC_MTK_LOADER */

//-----------------------------------------------------------------------------
/* _PDWNC_SetupEthernetWakeup() to set ethernet wakeup
 * 
 */
//-----------------------------------------------------------------------------
void _PDWNC_SetupEthernetWakeup(UINT32 *pu4PowerDownEn)
{
    UINT32 u4Val;
	UINT32 u4Pol,u4Gpio;
    UINT32 MACADDRESS_H;
    UINT32 MACADDRESS_L;
    DTVCFG_T    rDtvCfg;
    INT32       i4_ret;

    i4_ret = EEPDTV_GetCfg(&rDtvCfg);
    
    ASSERT(i4_ret==0); 
    if (i4_ret!=0)
    {
        Printf("_PDWNC_SetupEthernetWakeup: DTV EEPROM read failure.\n");
        LOG(0, "%s(%d) DTV EEPROM read failure.\n", __FILE__, __LINE__);
        return;
    } 
    
    if(rDtvCfg.u1Flags3 & (UINT8)DTVCFG_FLAG3_WAKEUP_ON_LAN )
    {
        MACADDRESS_H = u4IO32Read4B(ETHERNET_BASE + RW_MY_MAC_H);
        MACADDRESS_L = u4IO32Read4B(ETHERNET_BASE + RW_MY_MAC_L);
        if((0 == MACADDRESS_H) && (0 == MACADDRESS_L))
        {
            MACADDRESS_H = 0x0000000C;
            MACADDRESS_L = 0xE7060000;
        }
               
        u4Val = u4IO32Read4B(CKGEN_BASE + 0x1C0);
        u4Val &= ~(0x3 << 25);
        vIO32Write4B(CKGEN_BASE + 0x1C0, u4Val);
        HAL_Delay_us(50);
        u4Val |= 0x3 << 25;
        vIO32Write4B(CKGEN_BASE + 0x1C0, u4Val);

        HAL_Delay_us(50);
        
        u4Val = u4IO32Read4B(ETHERNET_BASE + RW_ETHSYS_CONFIG);
        u4Val |= INT_PHY_SEL;
        vIO32Write4B(ETHERNET_BASE + RW_ETHSYS_CONFIG, u4Val);

        u4Val = u4IO32Read4B(ETHERNET_BASE + RW_SW_RESET_CONTROL);
        u4Val |= PHY_RSTN;
        vIO32Write4B(ETHERNET_BASE + RW_SW_RESET_CONTROL, u4Val);

        HAL_Delay_us(50);       

        vIO32Write4B(ETHERNET_BASE + RW_MY_MAC_H, MACADDRESS_H);
        vIO32Write4B(ETHERNET_BASE + RW_MY_MAC_L, MACADDRESS_L);
     
        u4Val = u4IO32Read4B(ETHERNET_BASE + RW_MAC_CFG);
        u4Val |= WOL;
        vIO32Write4B(ETHERNET_BASE + RW_MAC_CFG, u4Val);

        u4Val = u4IO32Read4B(ETHERNET_BASE + RW_INT_MASK);
        u4Val = ~MAGIC_PAK_REC_INT_MASK;
        vIO32Write4B(ETHERNET_BASE + RW_INT_MASK, u4Val);

        u4Val = u4IO32Read4B(ETHERNET_BASE + ETH_UP_CFG);
        u4Val |= CPUCK_BY_8032;
        vIO32Write4B(ETHERNET_BASE + ETH_UP_CFG, u4Val);

        u4Val = u4IO32Read4B(PDWNC_PDMISC);
        u4Val |= (1<<29);    // ethernet wakeup enable
        vIO32Write4B(PDWNC_PDMISC, u4Val);

        u4Val = u4IO32Read4B(PDWNC_T8032_INTEN);
        u4Val |= (1<<29);    // ethernet interrupt 8032 enable
        vIO32Write4B(PDWNC_T8032_INTEN, u4Val);
    }
	if(rDtvCfg.u1Flags4 & (UINT8)DTVCFG_FLAG4_WAKEUP_ON_WLAN )
    {
	    if(DRVCUST_InitQuery(eWakeupOnWLanGpio, (UINT32 *)&u4Gpio) < 0)
	    {
	        return;
	    }
	    if(DRVCUST_InitQuery(eWakeupOnWLanPol, (UINT32 *)&u4Pol) < 0)
		{
	        return;
	    }	
		_PDWNC_SetupGpioWakeup(u4Gpio,u4Pol,pu4PowerDownEn);
	}
}

//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/* PDWNC_WriteErrorCode() to write the current error code to EEPROM & uP
 * 
 *  @param u1ErrorCode  Error code
 *  @retval   0		    SUCCESS.		
 *  @retval   1		    FAIL.
 */
//-----------------------------------------------------------------------------
INT32 PDWNC_WriteErrorCode(UINT8 u1ErrorCode)
{
    VERIFY(pgrPDWNC_Func);
    VERIFY(pgrPDWNC_Func->pfnWriteErrorCode);
    pgrPDWNC_Func->pfnWriteErrorCode(u1ErrorCode);
    return 0;
}

//-----------------------------------------------------------------------------
/* PDWNC_SetQueryRtcYear() to query RTC year register value
 * 
 *  @param u1ErrorCode  Error code
 *  @retval   0		    SUCCESS.		
 *  @retval   1		    FAIL.
 */
//-----------------------------------------------------------------------------
UINT8 PDWNC_SetQueryRtcYear(const UINT8 *pu1Year)
{
#ifdef CC_FPGA
    return *pu1Year;
#else
    if (pu1Year)
    {
        vIO32WriteFldAlign(PDWNC_PROT, 0x87, FLD_PROT);
        vIO32WriteFldAlign(PDWNC_PROT, 0x78, FLD_PROT);
        vIO32Write4B(PDWNC_YEAR, (UINT32)(*pu1Year));
        vIO32WriteFldAlign(PDWNC_PROT, 0x0, FLD_PROT);
    }
#if defined(CC_MT8223)
    RTC_READ_POLLING();
    #endif
    return(u4IO32Read4B(PDWNC_YEAR));
#endif
}

//-----------------------------------------------------------------------------
/* PDWNC_ReadWakeupReason() to read the wakeup reason 
 * 
 *  @param u1ErrorCode  Error code
 *  @retval   0		    SUCCESS.		
 *  @retval   1		    FAIL.
 */
//-----------------------------------------------------------------------------
UINT32 PDWNC_ReadWakeupReason(void)
{
    UINT32 u4Reason;

    VERIFY(pgrPDWNC_Func);
    VERIFY(pgrPDWNC_Func->pfnReadWakeupReason);
    pgrPDWNC_Func->pfnReadWakeupReason(&u4Reason);
     
    return (u4Reason);
}


//-----------------------------------------------------------------------------
/* PDWNC_PowerDownByAcCut()
 * 
 *  @param u1ErrorCode  Error code
 *  @retval   0		    SUCCESS.		
 *  @retval   1		    FAIL.
 */
//-----------------------------------------------------------------------------
UINT32 PDWNC_PowerDownByAcCut(void)
{
    UNUSED(PDWNC_Init());
    return _fgPowerDownByAcCut;
}
//-----------------------------------------------------------------------------
/* PDWNC_pm_suspend() to initialize PDWNC driver
 * 
 *  @param u1ErrorCode  Error code
 */
//-----------------------------------------------------------------------------
void PDWNC_pm_suspend(void)
{
    Printf("PDWNC_pm_suspend \n");
    _gi4PDWNCInit = 0;
}
//-----------------------------------------------------------------------------
/* PDWNC_pm_resume() to initialize PDWNC driver
 * 
 *  @param u1ErrorCode  Error code
 */
//-----------------------------------------------------------------------------
void PDWNC_pm_resume(void)
{
    UINT32 u4Val;
    PFN_GPIO_POWER_FUNC pfnPowerOnFunc = NULL;

    if (_gi4PDWNCInit)
    {
        Printf("Warning! PDWNC_pm_resume has been executed..\n");
        return ;
    }
    _gi4PDWNCInit = 1;

	LOG(0," PDWNC_pm_resume(void)\n");
    BIM_EnableIrq(VECTOR_PDWNC);
// MT5391 Specific initialization
#ifdef CC_MT5391
    if (IS_XTAL_60MHZ())
    {
        u4Val = PDCK_SEL_XTAL_D20;
    }
    else
    {
        u4Val = PDCK_SEL_XTAL_D9;    
    }    
    _PDWNC_WriteMT8297(REGSIF_RW_CKGEN, &u4Val);
#endif

#ifndef CC_MTK_LOADER    
    // If use internal T8032, need to initialize something...
    if(DRVCUST_InitGet(eT8032uPOffset) != 0)
    {
        // Let ARM control other engine after loading image      
        // Note this should happen before _PDWNC_InitGpio to avoid interrupt before interrupt status/clear could be access
#ifndef CC_MT8223
        UINT32 i = 0, j = 0;              
        PDWNC_T8032CMD_SET_CMD(PDWNC_CMD_ARMT8032_ENGINE_SWITCH | (1 << 8));//        vIO32Write4B(PDWNC_SHREG8,  PDWNC_CMD_ARMT8032_ENGINE_SWITCH | (1 << 8));    
         	vIO32WriteFldAlign(PDWNC_INTCLR, 0x1, FLD_AUX_INTCLR);
        while(1)
        {          
            vIO32WriteFldAlign(PDWNC_ARM_INT, 0x1, FLD_ARM_INT);
            while(i < 100000) // Wait 100 ms for T8032's ack
            {
                if(IO32ReadFldAlign(PDWNC_INTSTA, FLD_AUX_INT_STA))
                {
                    Printf("Preloader T8032 ack !\n");        
                    //if((PDWNC_READ8(REG_RW_AUX_DATA) & 0xff) != PDWNC_CMD_ARMT8032_ENGINE_SWITCH)
                    if(/*(u1IO32Read1B(PDWNC_SHREG0) & 0xff)*/PDWNC_T8032CMD_GET_RESPONSE()  != PDWNC_CMD_ARMT8032_ENGINE_SWITCH)
                    {      
                        Printf("Preloader T8032 ack mismatch!\n");        
                        continue;
                    }
                    HAL_Delay_us(1);
                    break;                
                }            
                i++;
            }
            vIO32WriteFldAlign(PDWNC_INTCLR, 0x1, FLD_AUX_INTCLR);
            if(i < 100000) // T8032 ack within 100 ms, we're done
            {     
                break;
            }
            // Any cleanup before re-issue interrupt ??        
            else
            {
                i = 0;
            }
            if(++j > 2) // Set j to interrupt re-issue retry count
            {
                Printf("T8032 ack timeout!\n");            
                break;
            }   
        }               
        //david: useless ...PDWNC_WRITE32(REG_RW_UP_CFG, PDWNC_READ32(REG_RW_UP_CFG) & ~(ENGINE_EN));    
#endif   
        PDWNC_INTEN(PDWNC_INTNO_AUX_INT);
        // Install GPIO ISR
        PDWNC_GPIO_Resume();

        u4Val = PDWNC_ReadWakeupReason();
        if((u4Val != PDWNC_WAKE_UP_REASON_AC_POWER) && (u4Val != PDWNC_WAKE_UP_REASON_WATCHDOG))
        {           
            LOG(5, "T8032 init wakeup case app stage...\n");                       
            VERIFY(_PDWNC_SetT8032(PDWNC_T8032_SET_NORMAL) == 0);
        }    
        else
        {
            LOG(5, "T8032 init AC on/off case app stage...\n");                            
            VERIFY(_PDWNC_SetT8032(PDWNC_T8032_SET_NORMAL) == 0);
        }
    }
    else
    {
        // Install GPIO ISR
        PDWNC_GPIO_Resume();
        LOG(5, "T8032 is disabled...\n");            
    }
#else
    u4Val = PDWNC_ReadWakeupReason();
    if(u4Val == PDWNC_WAKE_UP_REASON_AC_POWER)
    {   
        UINT8 u1Year = 0xff;
        // To indicate it is last cut off by A/C
        VERIFY(PDWNC_SetQueryRtcYear(&u1Year) == u1Year);                
    }    
#endif /* CC_MTK_LOADER */

#ifdef CC_UP8032_ATV

	if(GPIO_POWER_ON_FUNC!=NULL)
		GPIO_POWER_ON_FUNC();

#else
    if (DRVCUST_InitQuery(eGpioPowerOnFunc, (UINT32 *)(void *)&pfnPowerOnFunc) == 0)
    {
        pfnPowerOnFunc();
    }
#endif	

    _fgPowerDownByAcCut = (PDWNC_SetQueryRtcYear(NULL)==0xff);

// MT5360 Specific initialization
    
    // xtal setting.    
	//vIO32Write4B(PDWNC_XTALCFG, 0x0e000000);
	//#ifdef MT5396_TODO	//defined(CC_MT5368) ||defined(CC_MT5396) 
	//vIO32Write4B(PDWNC_CONCFG0, 0x002e0000);	
	//#endif
	
    // HDMI I2C settings
#ifdef MT5368_TODO    
#if !defined(CC_MT8223)
    #if defined(CC_MT5365) || defined(CC_MT5395)
        u4Val = u4IO32Read4B(PDWNC_PADCFG1);
        u4Val &= ~(FLD_HDMI_SDA_MASK   | FLD_HDMI_SCL_MASK);
        vIO32Write4B(PDWNC_PADCFG1, u4Val);
        vIO32WriteFldAlign(PDWNC_PADCFG1, 0x1, FLD_HDMI_SDA_SMT);
        vIO32WriteFldAlign(PDWNC_PADCFG1, 0x1, FLD_HDMI_SCL_SMT);
    #else    //mt5387 and mt5363
        #if defined(CC_MT5387) && !defined(CC_MT5363)//mt5387 only
        u4Val = u4IO32Read4B(PDWNC_PADCFG1);
        u4Val &= ~(FLD_HDMI_SDA0_MASK	| FLD_HDMI_SCL0_MASK);
        vIO32Write4B(PDWNC_PADCFG1, u4Val);
        vIO32WriteFldAlign(PDWNC_PADCFG1, 0x1, FLD_HDMI_SDA0_SMT);
        vIO32WriteFldAlign(PDWNC_PADCFG1, 0x1, FLD_HDMI_SCL0_SMT);
        #endif
        u4Val = u4IO32Read4B(PDWNC_PADCFG3);
        u4Val &= ~(FLD_HDMI_SDA1_MASK	| FLD_HDMI_SCL1_MASK);
        vIO32Write4B(PDWNC_PADCFG3, u4Val);
        vIO32WriteFldAlign(PDWNC_PADCFG3, 0x1, FLD_HDMI_SDA1_SMT);
        vIO32WriteFldAlign(PDWNC_PADCFG3, 0x1, FLD_HDMI_SCL1_SMT);

        u4Val = u4IO32Read4B(PDWNC_PADCFG4);
        u4Val &= ~(FLD_HDMI_SDA2_MASK	| FLD_HDMI_SCL2_MASK);
        vIO32Write4B(PDWNC_PADCFG4, u4Val);
        vIO32WriteFldAlign(PDWNC_PADCFG4, 0x1, FLD_HDMI_SDA2_SMT);
        vIO32WriteFldAlign(PDWNC_PADCFG4, 0x1, FLD_HDMI_SCL2_SMT);
    #endif        
#endif
#endif

    PDWNC_ResumeServoADC();
    // Set reset value to some registers.
    //vIO32Write4B(PDWNC_VGACFG0, 0xe8884c88);// SYNC slicer current reduction//

	vIO32WriteFldAlign(PDWNC_VGACFG0, 0x0, FLD_RG_VMUX_PWD);

	vIO32WriteFldAlign(PDWNC_VGACFG0, 0x1, FLD_RG_AUTOBIAS_EN);//20100415, sc hwu suggestion		
	vIO32WriteFldAlign(PDWNC_PDMISC, 0x0, FLD_VGA_SYNC_DIS);//20100415, chiahsien liu suggestion		

#ifdef CC_DISABLE_SOG_WAKEUP	
    vIO32WriteFldAlign(PDWNC_VGACFG1, 1, FLD_RG_SOG_EN);
    vIO32WriteFldAlign(PDWNC_VGACFG1, 0, FLD_RG_SYNC_PWD);
#endif
    
#ifndef CC_MTK_LOADER

    // Do the external power down function
    VERIFY(pgrPDWNC_Func->pfnInit);
    pgrPDWNC_Func->pfnInit();
    
#endif /* CC_MTK_LOADER */

    return ;
}

//-----------------------------------------------------------------------------
/* PDWNC_Init() to initialize PDWNC driver
 * 
 *  @param u1ErrorCode  Error code
 *  @retval   0		    SUCCESS.		
 *  @retval   1		    FAIL.
 */
//-----------------------------------------------------------------------------
INT32 PDWNC_Init(void)
{
    UINT32 u4Val;
    PFN_GPIO_POWER_FUNC pfnPowerOnFunc = NULL;

    if (_gi4PDWNCInit)
    {
        return 0;
    }

    _gi4PDWNCInit = 1;
	
	LOG(0,"PDWNC_Init\n");

// MT5391 Specific initialization

#ifndef CC_MTK_LOADER    
    // If use internal T8032, need to initialize something...
    if(DRVCUST_InitGet(eT8032uPOffset) != 0)
    {
        // Let ARM control other engine after loading image      
        // Note this should happen before _PDWNC_InitGpio to avoid interrupt before interrupt status/clear could be access
        UINT32 i = 0, j = 0;              
		vIO32Write4B(PDWNC_INTCLR, 0xffffffff);
        PDWNC_T8032CMD_SET_CMD(PDWNC_CMD_ARMT8032_ENGINE_SWITCH | (1 << 8));//        vIO32Write4B(PDWNC_SHREG8,  PDWNC_CMD_ARMT8032_ENGINE_SWITCH | (1 << 8));    
        while(1)
        {          
            vIO32WriteFldAlign(PDWNC_ARM_INT, 0x1, FLD_ARM_INT);
            while(i < 100000) // Wait 100 ms for T8032's ack
            {
                if(IO32ReadFldAlign(PDWNC_INTSTA, FLD_AUX_INT_STA))
                {
                    Printf("Preloader T8032 ack !\n");        
                    //if((PDWNC_READ8(REG_RW_AUX_DATA) & 0xff) != PDWNC_CMD_ARMT8032_ENGINE_SWITCH)
                    if(/*(u1IO32Read1B(PDWNC_SHREG0) & 0xff)*/PDWNC_T8032CMD_GET_RESPONSE()  != PDWNC_CMD_ARMT8032_ENGINE_SWITCH)
                    {      
                        Printf("Preloader T8032 ack mismatch!\n");        
                        continue;
                    }
                    HAL_Delay_us(1);
                    break;                
                }            
                i++;
            }
            vIO32WriteFldAlign(PDWNC_INTCLR, 0x1, FLD_AUX_INTCLR);
            if(i < 100000) // T8032 ack within 100 ms, we're done
            {     
                break;
            }
            // Any cleanup before re-issue interrupt ??        
            else
            {
                i = 0;
            }
            if(++j > 2) // Set j to interrupt re-issue retry count
            {
                Printf("T8032 ack timeout!\n");            
                break;
            }   
        }               
        //david: useless ...PDWNC_WRITE32(REG_RW_UP_CFG, PDWNC_READ32(REG_RW_UP_CFG) & ~(ENGINE_EN));    
        // Install T8032 ISR
        VERIFY(_PDWNC_RegT8032Isr() == PDWNC_OK);
        // Install GPIO ISR
        VERIFY(_PDWNC_InitGpio() == 0);              

        u4Val = PDWNC_ReadWakeupReason();
        if((u4Val != PDWNC_WAKE_UP_REASON_AC_POWER) && (u4Val != PDWNC_WAKE_UP_REASON_WATCHDOG))
        {           
            LOG(5, "T8032 init wakeup case app stage...\n");                       
            VERIFY(_PDWNC_SetT8032(PDWNC_T8032_SET_NORMAL) == 0);
        }    
        else
        {
            LOG(5, "T8032 init AC on/off case app stage...\n");                            
            VERIFY(_PDWNC_SetT8032(PDWNC_T8032_SET_NORMAL) == 0);
        }
    }
    else
    {
        // Install GPIO ISR
        VERIFY(_PDWNC_InitGpio() == 0);    
        LOG(5, "T8032 is disabled...\n");            
    }
#else
    u4Val = PDWNC_ReadWakeupReason();
    if(u4Val == PDWNC_WAKE_UP_REASON_AC_POWER)
    {   
        UINT8 u1Year = 0xff;
        // To indicate it is last cut off by A/C
        VERIFY(PDWNC_SetQueryRtcYear(&u1Year) == u1Year);                
    }    
#endif /* CC_MTK_LOADER */

    if (DRVCUST_InitQuery(eGpioPowerOnFunc, (UINT32 *)(void *)&pfnPowerOnFunc) == 0)
    {
        pfnPowerOnFunc();
    }

    _fgPowerDownByAcCut = (PDWNC_SetQueryRtcYear(NULL)==0xff);


    PDWNC_InitServoADC();

	// Set reset value to some registers.
	//vIO32Write4B(PDWNC_VGACFG0, 0xe8884c88);// SYNC slicer current reduction

	vIO32WriteFldAlign(PDWNC_VGACFG0, 0x0, FLD_RG_VMUX_PWD);
	
	vIO32WriteFldAlign(PDWNC_VGACFG0, 0x1, FLD_RG_AUTOBIAS_EN);//20100415, sc hwu suggestion		
	vIO32WriteFldAlign(PDWNC_PDMISC, 0x0, FLD_VGA_SYNC_DIS);//20100415, chiahsien liu suggestion		
	vIO32WriteFldAlign(PDWNC_PRST, 0x1, FLD_PRST_EN); //Power reset enable.
#ifdef CC_DISABLE_SOG_WAKEUP	
    vIO32WriteFldAlign(PDWNC_VGACFG1, 1, FLD_RG_SOG_EN);
    vIO32WriteFldAlign(PDWNC_VGACFG1, 0, FLD_RG_SYNC_PWD);
#endif

#ifndef CC_MTK_LOADER

    // Do the external power down function
    VERIFY(pgrPDWNC_Func->pfnInit);
    pgrPDWNC_Func->pfnInit();
    
#endif /* CC_MTK_LOADER */

    return 0;
}

#ifndef CC_MTK_LOADER
//-----------------------------------------------------------------------------
/** PDWNC_RegIsr() is to hook the sub-interrupt routine of PDWNC module.
 *  @param u2IntNo the interrupt number at PDWNC module.
 *  @param pfnIsr the hooked function to register, NULL to disable.
 *  @param ppfnOld the old hooked function.
 *  @retval PDWNC_OK: Succeed
 *  @retval PDWNC_FAIL: Fail
 */
//-----------------------------------------------------------------------------
INT32 PDWNC_RegIsr(UINT16 u2IntNo, x_os_isr_fct pfnIsr, x_os_isr_fct *ppfnOld)
{
    UINT32 u4Val, u4State;

#ifndef CC_UP8032_ATV//prevent recursive call   PDWNC_RegIsr -> PDWNC_Init -> _PDWNC_InitGpio -> PDWNC_RegIsr
    VERIFY(PDWNC_Init()==0);
#endif
    if(u2IntNo > MAX_PDWNC_INT_ID_2)
    {
        return PDWNC_FAIL;
    }

    if (ppfnOld != NULL)
    {
        *ppfnOld = _varIntList[u2IntNo];
    }
   
    if (pfnIsr == NULL)
    {
        u4Val = u4IO32Read4B(PDWNC_ARM_INTEN);
        u4Val &= ~(_PINT(u2IntNo));
        if (u4Val==0)
        {
            VERIFY(_PdwncHwSetIsr(FALSE));
        }
#ifndef CC_MTK_LOADER
        u4State = HalCriticalStart();
#endif
        vIO32Write4B(PDWNC_ARM_INTEN, u4Val);
#ifndef CC_MTK_LOADER
        HalCriticalEnd(u4State);
#endif
        _varIntList[u2IntNo] = pfnIsr;
    }
    else
    {
        
  		_varIntList[u2IntNo] = pfnIsr;
	    u4Val = u4IO32Read4B(PDWNC_ARM_INTEN);
        u4Val |= IS_PINTNO_INTABLE_GPIO(u2IntNo) ? 0 : (_PINT(u2IntNo));		
#ifndef CC_MTK_LOADER
        u4State = HalCriticalStart();
#endif
        VERIFY(_PdwncHwSetIsr(TRUE));
        vIO32Write4B(PDWNC_ARM_INTEN, u4Val);
#ifndef CC_MTK_LOADER
        HalCriticalEnd(u4State);
#endif
    }
    return PDWNC_OK;
}

//-----------------------------------------------------------------------------
/** PDWNC_ClearInt() to clear the interrupt flag in PDWNC status
 *  PDWNC_ClearInt() do bit clear on REG_RW_PDWNC_INTCLR register and the \n
 *  value of REG_RO_PDWNC_INTST will change, too.  If the REG_RO_PDWNC_INTST
 *  is 0, the interrupt status bit of PDWNC on BIM IRQST will be cleared, too.
 *  @param u2IntNo the sub-interrupt number of PDWNC
 *  @retval TRUE: Succeed
 *  @retval FALSE: Fail
 */
//-----------------------------------------------------------------------------
BOOL PDWNC_ClearInt(UINT16 u2IntNo)
{
    UINT32 u4Int, u4State;

    if (u2IntNo > MAX_PDWNC_INT_ID_2)//MAX_PINT_ID)
    {
        return FALSE;
    }

    u4Int = _PINT(u2IntNo);
#ifndef CC_MTK_LOADER
    u4State = HalCriticalStart();
#endif
          vIO32Write4B(PDWNC_INTCLR, u4Int);
#ifndef CC_MTK_LOADER
    HalCriticalEnd(u4State);
#endif
    return TRUE;
}
#endif /* CC_MTK_LOADER */

//-----------------------------------------------------------------------------
/** PDWNC_T8032Reboot() Use T8032 to Enter standby and then boot the system
 */
//-----------------------------------------------------------------------------
void PDWNC_T8032Reboot(UINT32 u4Seconds)
{
#ifndef CC_MTK_LOADER
    UINT32 u4State;

    if (DRVCUST_OptGet(eAudioAmpEnableGpioNum) != 0xffffffff)
    {
        // turn off audio amp
        GPIO_SetOut(DRVCUST_OptGet(eAudioAmpEnableGpioNum), !DRVCUST_OptGet(eAudioAmpGpioPolarity));
    }
    if (DRVCUST_OptGet(eAudioMuteGpioNum) != 0xffffffff)
    {
        // set audio mute
        GPIO_SetOut(DRVCUST_OptGet(eAudioMuteGpioNum), DRVCUST_OptGet(eAudioMuteGpioPolarity));
    }

#ifndef CC_UBOOT
    // turn off backlight
    vDrvTurnOffBacklt();
    // turn off lvds power
    vDrvTurnOffPanel();
#endif
#endif /* CC_MTK_LOADER */
#ifndef CC_MTK_LOADER
    u4State = HalCriticalStart();
#endif
    PDWNC_T8032CMD_SET_CMD(PDWNC_CMD_ARMT8032_COLD_REBOOT | (((u4Seconds > 12) ? (0x7F) : (u4Seconds*10)) << 8));
    PDWNC_T8032CMD_SET_DATA(0);

    vIO32WriteFldAlign(PDWNC_ARM_INT, 0x1, FLD_ARM_INT);
#ifndef CC_MTK_LOADER
    HalCriticalEnd(u4State);
#endif
}

//-----------------------------------------------------------------------------
/** PDWNC_Reboot() Reboot the system
 */
//-----------------------------------------------------------------------------
void PDWNC_Reboot()
{
#ifndef CC_UBOOT
    #ifdef __KERNEL__
        UINT32 u4State;
    #endif

#ifdef CC_UP8032_ATV
	vKrDisableInt();
#else
    #ifdef __KERNEL__
        u4State = HalCriticalStart();
        BIM_WRITE32(REG_MISC_IRQEN, 0);
        BIM_WRITE32(REG_IRQEN, 0);
        HalCriticalEnd(u4State);
    #else
      HalDisableIRQ();
      HalDisableFIQ();
    #endif
#endif
#endif /* CC_UBOOT */
    _PDWNC_SetupStrapping();
    
    Printf("\n                                       ");	
    vIO32Write4B(PDWNC_WDTRST0,0x0fffffff);	
    vIO32Write4B(PDWNC_WDT0,0x7fff0000);//vIO32Write4B(PDWNC_WDT0,0x0fff0000);
	vIO32Write4B(PDWNC_WDTCTL, (u4IO32Read4B(PDWNC_WDTCTL) & 0x00ffffff) | 0x1);//	vIO32WriteFldAlign(PDWNC_WDTCTL, 0x1, FLD_WDT0E);
    while (1) { }
}

//-----------------------------------------------------------------------------
/** PDWNC_Reboot1() Reboot the system
 */
//-----------------------------------------------------------------------------
void PDWNC_Reboot1()
{
#ifndef CC_UBOOT
    #ifdef __KERNEL__
        UINT32 u4State;
    #endif

#ifdef CC_UP8032_ATV
	vKrDisableInt();
#else
    #ifdef __KERNEL__
        u4State = HalCriticalStart();
        BIM_WRITE32(REG_MISC_IRQEN, 0);
        BIM_WRITE32(REG_IRQEN, 0);
        HalCriticalEnd(u4State);
    #else
      HalDisableIRQ();
      HalDisableFIQ();
    #endif
#endif
#endif /* CC_UBOOT */
    _PDWNC_SetupStrapping();
    Printf(" PDWNC_Reboot1 \n                                       ");
    vIO32Write4B(PDWNC_WDTRST1,0x0fffffff);	
    vIO32Write4B(PDWNC_WDT1,0x7fff0000);//vIO32Write4B(PDWNC_WDT0,0x0fff0000);
	vIO32Write4B(PDWNC_WDTCTL, (u4IO32Read4B(PDWNC_WDTCTL) & 0x00ffffff) | 0x100);//	vIO32WriteFldAlign(PDWNC_WDTCTL, 0x1, FLD_WDT0E);
	while (1) { }
}

//-----------------------------------------------------------------------------
/** PDWNC_IsWakeupByPowerButton() to check wakeup status is Power key pad.
 *  @retval 0 not wakeup by power key pad, 1 wakeup by power key pad.
 */
//-----------------------------------------------------------------------------
UINT32 PDWNC_IsWakeupByPowerButton(void)
{
    INT32 i;
    UINT32 u4Val, u4Gpio, u4Servo;
    SGL_GPIO_T   *prSingleGpioKeyList = NULL;
    SGL_SERVO_T  *prSingleServoKeyList = NULL;
    GREED_KEY_T  *prGreedGpioKeyList = NULL;
    
    // For compatible with both main chip servoADC & mcm servoADC
    _PDWNC_ReadServoADCWakeupStatus(&u4Servo);
    u4Val = (_PDWNC_ReadWakeupStatus() & ~(0xff << 20)) | u4Servo;

    i = DRVCUST_InitQuery(eSingleGpioKeyList, (UINT32 *)(void *)&prSingleGpioKeyList);
    if (i == 0)
    {
        while ((prSingleGpioKeyList) &&
            (prSingleGpioKeyList[i].u4Key != 0) &&
            (prSingleGpioKeyList[i].u4Key != DKBIT_NONE))
        {
            if (prSingleGpioKeyList[i].u4Key == DKBIT_POWER)
            {
                u4Gpio = prSingleGpioKeyList[i].u4Gpio - OPCTRL(0);
                if (u4Val & (1U << u4Gpio))
                {
                    return 1;
                }
                else
                {
                    return 0;
                }
            }
            i++;
        }
    }

    i = DRVCUST_InitQuery(eSingleServoKeyList, (UINT32 *)(void *)&prSingleServoKeyList);
    if (i==0)
    {
        while ((prSingleServoKeyList) &&
            (prSingleServoKeyList[i].u4Key != 0) &&
            (prSingleServoKeyList[i].u4Key != DKBIT_NONE))
        {
            if (prSingleServoKeyList[i].u4Key == DKBIT_POWER)
            {
                u4Servo = prSingleServoKeyList[i].u4Servo;
                // In case of old style servo ADC numbering
                if(u4Servo >= ADIN0)
                {
                    u4Servo -= ADIN0;
                }
                if (u4Servo > 31)
                {
                    return 0;
                }                
                //if (u4Val & SRVAD_WAKEN(WAK_SRVADX(u4Servo)))
                if (u4Val & PDWNC_SRVAD_WAKEN(PDWNC_WAK_SRVADX(u4Servo)))                
                {
                    return 1;
                }
                else
                {
                    return 0;
                }
            }
            i++;
        }
    }

    i = DRVCUST_InitQuery(eGreedGpioKeyList, (UINT32 *)(void *)&prGreedGpioKeyList);
    if (i==0)
    {
        while ((prGreedGpioKeyList) && 
            (prGreedGpioKeyList[i].u4Key != 0) &&
            (prGreedGpioKeyList[i].u4Key != DKBIT_NONE))
        {
            if (prGreedGpioKeyList[i].u4Key == DKBIT_POWER)
            {
                u4Gpio = prGreedGpioKeyList[i].u4GpioIn - OPCTRL(0);
                if (u4Val & (1U << u4Gpio))
                {
                    return 1;
                }
                else
                {
                    return 0;
                }
            }
            i++;
        }
    }

    return 0;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void PDWNC_WatchDogReboot(void)
{
    //if(u4IO32Read4B(PDWNC_WDTCTL))
     if (IO32ReadFldAlign(PDWNC_WDTCTL, FLD_WDT0E | FLD_WDT1E | FLD_WDT2E))
    {
        PDWNC_Reboot();
    }
}

//-----------------------------------------------------------------------------
/** PDWNC_CreateWatchDogThread() to enable watch dog and set watch dog thread.
 */
//-----------------------------------------------------------------------------
void PDWNC_CreateWatchDogThread(void)
{

#ifndef CC_MTK_LOADER
#if 0
    static HANDLE_T hWatchDog;

    PDWNC_WRITE32(REG_RW_WATCHDOG_EN, 0);
    if (x_thread_create(&hWatchDog, "WatchDog", 0x800, DRVCUST_OptGet(eReleaseWatchdogPriority),
            _WatchDogBody, 0, NULL) != OSR_OK)
    {
        LOG(1, "\nError: Create watchdog thread fail\n");
        return;
    }
#endif

#if defined(CC_MT5368)||defined(CC_MT5396)||defined(CC_MT5389)||defined(CC_MT5398)
#ifndef NDEBUG
	PDWNC_WatchDogPreInit();
#endif
#endif
    VERIFY(pgrPDWNC_Func);
    VERIFY(pgrPDWNC_Func->pfnWatchDogInit);
    pgrPDWNC_Func->pfnWatchDogInit();

    VERIFY(pgrPDWNC_Func->pfnErrorHandleInit);
    pgrPDWNC_Func->pfnErrorHandleInit();
#endif /* CC_MTK_LOADER */
}

//-----------------------------------------------------------------------------
/** PDWNC_IrLedCheck() to check ir blinking option
 */
//-----------------------------------------------------------------------------
void PDWNC_IrLedCheck(INT32 fgOn)
{
    static HAL_TIME_T rPrevTime = { 0, 0 };
    static INT32 fgPrev = 1;
    static INT32 fgInit = 0;
    static PFN_GPIO_LEDCTRL_FUNC pfnLedCtrlFunc = NULL;
    HAL_TIME_T rTime, rDelta;   

    if (!fgInit)
    {
        UNUSED(DRVCUST_InitQuery(eGpioLedCtrlFunc, (UINT32 *)(void *)&pfnLedCtrlFunc));
        fgInit = 1;
    }

    if (!_fgIrLedEnable && DRVCUST_InitGet(eIrrxFlagBlinking))
    {
        if (DRVCUST_InitGet(eSupportDarkLed))
        {
            pfnLedCtrlFunc(eLedDarkLed);
        }
        else
        {
            pfnLedCtrlFunc(eLedIrOff);
        }
        return;
    }

    if (DRVCUST_InitGet(eIrrxFlagBlinking))
    {
        HAL_GetTime(&rTime);
        HAL_GetDeltaTime(&rDelta, &rPrevTime, &rTime);

        if ((fgOn) && (!fgPrev) && ((rDelta.u4Seconds > 0) || (rDelta.u4Micros > 50000)))
        {
            pfnLedCtrlFunc(eLedIrOn);
            fgPrev = 1;
            HAL_GetTime(&rPrevTime);
        }
        else if ((!fgOn) && (fgPrev) && ((rDelta.u4Seconds > 0) || (rDelta.u4Micros > 100000)))
        {
            if (DRVCUST_InitGet(eSupportDarkLed))
            {
                pfnLedCtrlFunc(eLedDarkLed);
            }
            else
            {
                pfnLedCtrlFunc(eLedIrOff);
            }
            fgPrev = 0;
            HAL_GetTime(&rPrevTime);
        }
    }
}

//-----------------------------------------------------------------------------
/** PDWNC_IrLedCheck() to check ir blinking option
 */
//-----------------------------------------------------------------------------
void PDWNC_EnableIrLed(BOOL fgEnable)
{
    _fgIrLedEnable = fgEnable;
}

//#ifdef CBUS_DRIVER_VERIFY
static void _PDWNC_SetupCbus(BOOL fgEnable)
{
    PDWNC_T8032_CMD_T rCmd;

    rCmd.u1Cmd = PDWNC_CMD_ARMT8032_CTRL_CBUS; //Notify T8032 start/stop control CEC
    rCmd.u1SubCmd = 0;

    if (fgEnable)
    {
        rCmd.au1Data[0] = 1; //T8032 control CEC
    }
    else
    {
        rCmd.au1Data[0] = 0; //T8032 stop control CEC
    }

    (void)PDWNC_T8032Cmd(&rCmd, NULL);
}

#if defined(CC_SUPPORT_STR_CORE_OFF)
// for WDT
static UINT32 u4WDTSuspendRegister;
void WDT_pm_suspend(void)
{
	u4WDTSuspendRegister = u4IO32Read4B(PDWNC_WDTCTL);
}

void WDT_pm_resume(void)
{	
	vIO32Write4B(PDWNC_WDT0,0);
	vIO32Write4B(PDWNC_WDT1,0);
	vIO32Write4B(PDWNC_WDT2,0);
	vIO32Write4B(PDWNC_WDTCTL,u4WDTSuspendRegister);
}


static void _PdwncSetupCEC(BOOL fgCecEnable)
{
	UINT32 u4Val;
	UINT32 u4Val2,u4Val3;
	INT32 i4Val;
	UINT8 au1Data[6] = {0};
	PDWNC_T8032_CMD_T rCmd;

	if (!fgCecEnable)
	{
		rCmd.u1Cmd = PDWNC_CMD_ARMT8032_SET_T8032_CTL_CEC; //Notify T8032 start/stop control CEC
		rCmd.u1SubCmd = 0;
		rCmd.au1Data[0] = 0;	//T8032 stop control CEC.
		PDWNC_T8032Cmd(&rCmd, NULL);		
		return;
	}
	
	// Write CEC configuration to uP
	if((DRVCUST_InitGet(eT8032uPOffset) != 0) && ((u4Val = DRVCUST_InitGet(eEepromCECMenuLanguageOffset)) != 0x0) &&
	((DRVCUST_InitGet(eEepromCECEnableOffset)) == 0x00) && ((DRVCUST_InitGet(eEepromCECOneTouchPlayEnableOffset)) == 0x00))
	{
		i4Val = EEPROM_Read(u4Val, (UINT32)au1Data, 6); 	   
		rCmd.u1Cmd = PDWNC_CMD_ARMT8032_SET_MENU_LANGUAGE;
		rCmd.u1SubCmd = 0;
		x_memset(rCmd.au1Data,0,sizeof(rCmd.au1Data));
		x_memcpy(rCmd.au1Data, au1Data, 4);
		PDWNC_T8032Cmd(&rCmd, NULL);

		rCmd.u1Cmd = PDWNC_CMD_ARMT8032_ENABLE_CEC;
		x_memset(rCmd.au1Data,0,sizeof(rCmd.au1Data));
		x_memcpy(rCmd.au1Data, &au1Data[4], 1);
		PDWNC_T8032Cmd(&rCmd, NULL);

		rCmd.u1Cmd = PDWNC_CMD_ARMT8032_ENABLE_ONE_TOUCH_PLAY;
		x_memset(rCmd.au1Data,0,sizeof(rCmd.au1Data));
		x_memcpy(rCmd.au1Data, &au1Data[5], 1);
		PDWNC_T8032Cmd(&rCmd, NULL);		
		UNUSED(i4Val);
	}
	else if((DRVCUST_InitGet(eT8032uPOffset) != 0) && ((u4Val = DRVCUST_InitGet(eEepromCECMenuLanguageOffset)) != 0x0) &&
	((u4Val2 = DRVCUST_InitGet(eEepromCECEnableOffset)) != 0x00) && ((u4Val3 = DRVCUST_InitGet(eEepromCECOneTouchPlayEnableOffset)) != 0x00))
	{
		i4Val = EEPROM_Read(u4Val, (UINT32)au1Data, 4); 	   
		rCmd.u1Cmd = PDWNC_CMD_ARMT8032_SET_MENU_LANGUAGE;
		rCmd.u1SubCmd = 0;
		x_memset(rCmd.au1Data,0,sizeof(rCmd.au1Data));
		x_memcpy(rCmd.au1Data, au1Data, 4);
		PDWNC_T8032Cmd(&rCmd, NULL);

		i4Val = EEPROM_Read(u4Val2, (UINT32)au1Data, 1);		
		rCmd.u1Cmd = PDWNC_CMD_ARMT8032_ENABLE_CEC;
		x_memset(rCmd.au1Data,0,sizeof(rCmd.au1Data));
		x_memcpy(rCmd.au1Data, au1Data, 1);
		PDWNC_T8032Cmd(&rCmd, NULL);

		i4Val = EEPROM_Read(u4Val3, (UINT32)au1Data, 1);		
		rCmd.u1Cmd = PDWNC_CMD_ARMT8032_ENABLE_ONE_TOUCH_PLAY;
		x_memset(rCmd.au1Data,0,sizeof(rCmd.au1Data));
		x_memcpy(rCmd.au1Data, au1Data, 1);
		PDWNC_T8032Cmd(&rCmd, NULL);		
		UNUSED(i4Val);
	}

	rCmd.u1Cmd = PDWNC_CMD_ARMT8032_SET_T8032_CTL_CEC; //Notify T8032 start/stop control CEC
	rCmd.u1SubCmd = 0;
	rCmd.au1Data[0] = 1;	//T8032 control CEC
	PDWNC_T8032Cmd(&rCmd, NULL);
}

static UINT32 SysSuspendMode=0;
//static UINT8  SysSuspendDelay=0;

#if !defined(CC_MTK_LOADER)
static HANDLE_T _hResumeHandle = (HANDLE_T)NULL;
#ifdef CC_STR_ONOFF_TEST_ENABLE
static HANDLE_T _hTestOnOffHandle = (HANDLE_T)NULL;
UINT32 enableSTROnOffTest = 1;
#endif

static void _PDWNC_ResumeHandle(void *pvArg)
{
    x_thread_delay(300); // delay 300ms until driver/kernel is finished
    Printf("_PDWNC_ResumeHandle\n");
	#ifdef ANDROID
    IRRX_SendMtkIr(BTN_EXIT); // trigger a user-activity
    IRRX_SendMtkIr(BTN_EXIT); // trigger a user-activity
    #endif
    IRRX_StartMtkIr();
    x_os_drv_set_timestamp("driver send key");
}

#ifdef CC_STR_ONOFF_TEST_ENABLE
// refer to _PDWNC_RebootHandle implementation
#define SYSTEM_SUSPEND_DELAY        20000
#define WAKEUP_TIMER                10       // system wake up timer by RTC
UINT32 _u4PDWNC_TestOnOFF = 0;
void _PDWNC_TestOnOFF(void *pvArg)
{
    x_thread_delay(SYSTEM_SUSPEND_DELAY);

    IRRX_SendMtkIr(BTN_MENU); // trigger a user-activity
    x_thread_delay(5000);
    IRRX_SendMtkIr(BTN_MENU); // trigger a user-activity
    x_thread_delay(5000);
    IRRX_SendMtkIr(BTN_POWER); // trigger a user-activity
    IRRX_SendMtkIr(BTN_POWER); // trigger a user-activity
    _u4PDWNC_TestOnOFF = 1;

    return;
}
#endif

void PDWNC_Resume(void)
{
    PDWNC_SetSuspendMode(FALSE);
	// Do the external power resume for customize setting
    VERIFY(pgrPDWNC_Func);
    VERIFY(pgrPDWNC_Func->pfnPowerResume);
    pgrPDWNC_Func->pfnPowerResume();
    IRRX_StopMtkIr();
    //IRRX_SendMtkIr(BTN_POWER);
    //IRRX_SendMtkIr(BTN_SELECT);
    #ifdef ANDROID
    IRRX_SendMtkIr(BTN_EXIT); // trigger a user-activity
    IRRX_SendMtkIr(BTN_EXIT); // trigger a user-activity
    #endif
    x_os_drv_set_timestamp("schedule to send key");
    UNUSED(x_thread_create(&_hResumeHandle, "ResumeHandle", 0x800, 100, _PDWNC_ResumeHandle, 0, NULL));
#ifdef CC_STR_ONOFF_TEST_ENABLE
    if (enableSTROnOffTest)
        UNUSED(x_thread_create(&_hTestOnOffHandle, "TestOnOffHandle", 0x800, 100, _PDWNC_TestOnOFF, 0, NULL));
#endif
}
#endif

void PDWNC_SetSuspendMode(UINT32 SuspendMode)
{
    SysSuspendMode = SuspendMode;
}

UINT32 PDWNC_IsSuspend(void)
{
    return SysSuspendMode;
}
//-----------------------------------------------------------------------------
/** PDWNC_EnterSuspend() Enter Suspend mode.
 */
//-----------------------------------------------------------------------------
UINT32 PDWNC_EnterSuspend(void)
{
    //PFN_GPIO_POWEROFF_FUNC pfnPowerOffFunc = NULL;
    //UINT32 u4Val, i, u4WtdCtrl, u4IRQEN, u4FIQEN;
    //UINT32 u4PowerDownEn = WAK_RTC | WAK_IRRX | WAK_VGA ;
    //UINT32 u4PowerDownEvent = u4SuspendEn;

	_fgSuspendWakeupReasonUp = TRUE;
#ifndef CC_MTK_LOADER
	#if defined(CC_SUPPORT_STR_CORE_OFF)
	IRRX_SuspendClrWakeupKeyFg();
	#endif
#endif
    //Printf("PDWNC_EnterSuspend\n");
	_u4SuspendEn = _u4SuspendEn | WAK_IRRX;
#ifdef CC_STR_ONOFF_TEST_ENABLE
#if !defined(CC_MTK_LOADER)
	if (_u4PDWNC_TestOnOFF == 1)
	{
	    UINT64 u8Time;

	    _u4SuspendEn |= WAK_RTC;

	    RTC_GetTimeDate(&u8Time);
	    u8Time += WAKEUP_TIMER;
	    RTC_SetAlertTime(u8Time);

	    // Enable RTC wakeup            
	    vIO32WriteFldAlign(PDWNC_WAKEN, 0x1, FLD_RTC_WAKEN);
	}
#endif
#endif
	Printf("PDWNC_EnterSuspend(0x%08x,0x%08x) .\n",_u4SuspendEn,_u4SuspendEvent);
#ifdef CC_CUST_CEC_CONFIG_SUPPORT
    // CEC configuration.
    PFN_CEC_CONFIG_QUERY_FUNC pfnQueryCECEnableFunc;
            
    pfnQueryCECEnableFunc = 
        (PFN_CEC_CONFIG_QUERY_FUNC)(DRVCUST_InitGet(eLoaderCECConfigQueryFunc)); 
    
    if (pfnQueryCECEnableFunc != NULL)
    {
        _PdwncSetupCEC(pfnQueryCECEnableFunc());
    }
    else
#endif /* CC_CUST_CEC_CONFIG_SUPPORT */        
    {
        _PdwncSetupCEC(TRUE);
    }

	PDWNC_SetSuspendMode(TRUE);
	//PDWNC_SetSuspendDelay(u1Delay20ms);

	return PDWNC_EnterPowerDown(_u4SuspendEn,_u4SuspendEvent);
}
#endif

//-----------------------------------------------------------------------------
/** PDWNC_RequestSuspendEvent() Request Suspend mode Event .
 */
//-----------------------------------------------------------------------------
UINT32 PDWNC_RequestSuspendEvent(UINT32 u4SuspendEn, UINT32 u4SuspendEvent)
{
#if defined(CC_SUPPORT_STR_CORE_OFF)
	_u4SuspendEn = u4SuspendEn;
	_u4SuspendEvent = u4SuspendEvent;
	//Printf("PDWNC_RequestSuspendEvent(%x,%x) .\n",u4SuspendEn,u4SuspendEvent);
#endif
	return 0;
}

//-----------------------------------------------------------------------------
/** PDWNC_EnterPowerDown() Enter Power Down mode.
 */
//-----------------------------------------------------------------------------
UINT32 PDWNC_EnterPowerDown(UINT32 u4PowerDownEn, UINT32 u4PowerDownEvent)
{
    PFN_GPIO_POWEROFF_FUNC pfnPowerOffFunc = NULL;
    INT32 i4Gpio;
    UINT32 u4Cnt;
    Printf("PDWNC_EnterPowerDown(%x,%x) \n",u4PowerDownEn,u4PowerDownEvent);
	
	//#ifdef CBUS_DRIVER_VERIFY
	_PDWNC_SetupCbus(1);
    // GPIO wakeup setup
    _PDWNC_SetupGPIO(&u4PowerDownEn);

    // ServoADC wakeup setup
    _PDWNC_SetupServoADC(&u4PowerDownEn);

    // IRRX wakeup setup
    _PdwncSetupIRRX(u4PowerDownEn);

    // VGA wakeup setup
    _PdwncSetupVGA(u4PowerDownEn);

#ifndef CC_MTK_LOADER
    // RTC wakeup setup
    _PdwncSetupRTC(&u4PowerDownEn);
#endif /* CC_MTK_LOADER */
    
/*CC_PDWNC_SUPPORT_WOL is defined in the .cfg file of sysbuild directory*/
    Printf("support WOL 						\n");
    _PDWNC_SetupEthernetWakeup(&u4PowerDownEn);

    // Do customization 
    if (DRVCUST_InitQuery(eGpioPowerOffFunc, (UINT32 *)(void *)&pfnPowerOffFunc) == 0)
    {
        pfnPowerOffFunc(u4PowerDownEn);
    }
    
    if(DRVCUST_InitGet(eFlagMt8292GpioSupport))
    {
        INT32 i4Val = 0;
        // If customization option does not set 8292 reset, it must be polled by H/W        
        i4Gpio = (INT32)DRVCUST_InitGet(eMt8292ResetGpio);
        if(i4Gpio != -1)//if(i4Gpio != (INT32)0xFFFFFFFF)
        {
            VERIFY(GPIO_Output(i4Gpio, &i4Val) == 0);
        }
        UNUSED(i4Val);
    }
    
    if (DRVCUST_InitGet(ePdwncPolarity))
    {
        vIO32WriteFldAlign(PDWNC_PDMISC, 0x1, FLD_PDWN_POL);
    }
    else
    {
        vIO32WriteFldAlign(PDWNC_PDMISC, 0x0, FLD_PDWN_POL);    
    }
#ifdef CC_PDWNCONOFF_PATCH
	vIO32Write4B(PDWNC_RESRV0, u4IO32Read4B(PDWNC_RESRV0) & 0x7fffffff); //clear 0x140[31]
#endif
#ifndef CC_MTK_LOADER
#ifdef TIME_MEASUREMENT
    TMS_DIFF_EX(TMS_FLAG_POWER_OFF,TMS_IR_POWER_OFF,"@DRV power off");
    TMS_END_EX(TMS_FLAG_POWER_OFF,TMS_IR_POWER_OFF,"@DRV power off");
#endif
#endif
    //clear watchdog flag
    vIO32Write4B(PDWNC_WDTCTL,0x7000000);

    // Setup Power Down
    Printf("Standby                         \n");
    // SerWaitTxBufClear();
    //for the case of logo standby, T8032 may be hang by CEC driver at this monent...
    if(DRVCUST_InitGet(eT8032uPOffset) != 0)
    {
        u4Cnt = 0;
        while(IS_XDATA_AT_ARM() && (u4Cnt < 3))
        {
            x_thread_delay(100);
            u4Cnt++;
        }
        if(u4Cnt == 3)
        {
            Printf("Warning T8032 is still hang...\n");
        }
    }

    // Do the external power down for customize setting
    VERIFY(pgrPDWNC_Func);
    VERIFY(pgrPDWNC_Func->pfnPowerDown);
    pgrPDWNC_Func->pfnPowerDown(u4PowerDownEvent, &u4PowerDownEn);

    // Enable wakeup devices
    vIO32Write4B(PDWNC_WAKEN,  u4PowerDownEn);
    // To wait the completion of pending SIF transaction and stop all subsequent SIF operations
    SIF_WaitLastTransaction(500);
    PDWNC_SetupPowerDown();
    
    return 0;
}

//-----------------------------------------------------------------------------
/** PDWNC_EnterPowerState() to enter semi power state (Will not actually power down the system).
 */
//-----------------------------------------------------------------------------
UINT32 PDWNC_EnterPowerState(UINT32 u4PowerDownEvent)
{
    UINT32 u4State, u4NextState;

    // Do the external power down for customize setting
    VERIFY(pgrPDWNC_Func);
    if (pgrPDWNC_Func->pfnGetCurrentState &&
        pgrPDWNC_Func->pfnDetermineNextState)
    {
        pgrPDWNC_Func->pfnGetCurrentState(&u4State);
        pgrPDWNC_Func->pfnDetermineNextState(u4State, u4PowerDownEvent, &u4NextState, NULL, NULL);
        pgrPDWNC_Func->pfnEnterNextState(u4NextState);
    }
    return 0;
}

UINT32 PDWNC_SetPowerDownReason(PDWNC_SET_POWER_DOWN_REASON_T rReason)
{
    _grPowerDownReason.u4Reason = rReason.u4Reason;
    _grPowerDownReason.u4Btn = rReason.u4Btn;
    return 0;
}

UINT32 PDWNC_QueryPowerDownReason(PDWNC_SET_POWER_DOWN_REASON_T *prReason)
{
    prReason->u4Reason = _grPowerDownReason.u4Reason;
    prReason->u4Btn = _grPowerDownReason.u4Btn;
    return 0;
}

BOOL PDWNC_SetupFpWakeup(BOOL *pfgEnable)
{
    static BOOL fgFpEnable = TRUE;
    if (pfgEnable)
    {
        fgFpEnable = *pfgEnable;
    }
    return fgFpEnable;
}

#ifndef CC_MTK_LOADER
void PDWNC_CreateDCOnOffTestThread(void)
{
    #if defined(CC_DC_ONOFF_TEST_ENABLE) || defined(CC_CPU_SPEED_TEST_ENABLE)
    // Start DC on/off stress test
    UNUSED(x_thread_create(&_hShutdownHandle, "ShutdownHandle", 0x800, 100, _PDWNC_RebootHandle, 0, NULL));
    #else
    return;
    #endif    
}
#endif /* CC_MTK_LOADER */


