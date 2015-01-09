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
 * $RCSfile: pdwnc_gpio.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file pdwnc_gpio.c
 *  pdwnc_gpio.c The PDWNC (Power DoWN Controller) GPIO functions
 */


//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

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
#include "pdwnc_srvad.h"
#include "pdwnc_if.h"
#include "sif_if.h"
#ifndef CC_MTK_LOADER
#ifdef __KERNEL__
#include <linux/module.h>
#ifdef CONFIG_MT53XX_NATIVE_GPIO
#include <mach/mtk_gpio.h>
#else
#include "cb_data.h"
#include "mtperipheral.h"
#endif
#endif
#endif

#undef PDWNC_WRITE32
#undef PDWNC_READ32

//-----------------------------------------------------------------------------
// Configuration
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
#if defined(CC_MT5399)
#define PDWNC_GPIO_INT0_BASE 0
#define PDWNC_GPIO_INT_GROUP_0_NUM 8

#define PDWNC_GPIO_INT1_BASE 16
#define PDWNC_GPIO_INT_GROUP_1_NUM 4

#define PDWNC_GPIO_INT2_BASE 26
#define PDWNC_GPIO_INT_GROUP_2_NUM 2

#define PDWNC_GPIO_INT3_BASE 32
#define PDWNC_GPIO_INT_GROUP_3_NUM 6
#endif
//-----------------------------------------------------------------------------
// Macros
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------

#ifndef CC_MTK_LOADER
// PDWNC module sub-interrupt routine list.
static PFN_GPIO_CALLBACK _afnPDWNCGpioCallback[TOTAL_PDWNC_GPIO_INT_NUM];

#if defined(CC_MT5399)
static const INT8 _ai1PdwncInt2Gpio[MAX_PDWNC_INT_ID_2] =
{//check 2804c[7:0] [18] and [24:31], translate them to bit offset within 280A8(offset of sysytem gpio number), only 17 gpio can issue interrupt
    0,  1,  2,  3,  4,  5,  6,  7,
   -1, -1, -1, -1, -1, -1, -1, -1,
    8,  9, 10, 11, -1, -1, -1, -1,
   -1, -1, 13, 15, -1, -1, -1, -1,
   52, 53, 54, 55, 56, 57
};
#endif
#if defined(CC_MT5882)
static const INT8 _ai1PdwncInt2Gpio[TOTAL_PDWNC_GPIO_INT_NUM] =
{
   	0,  1,  2,  3,  4,  5,  6,  7,  8,  9,  //0~9
   10, 11, 12, 14, 16, 28, 29, 30, 31, 32,  //10~19
   33, 34, 								    //20~21
};
#endif
#endif

#if defined(CC_MT5399)
static const INT8 _ai1PdwncGpio2Int[TOTAL_PDWNC_GPIO_NUM] =
{
	0,  1,  2,  3,  4,  5,  6,  7,
   16, 17, 18, 19, -1, 26, -1, 27,
   -1, -1, -1, -1, -1, -1, -1, -1,
   -1, -1, -1, -1, -1, -1, -1, -1,
   -1, -1, -1, -1, -1, -1, -1, -1,
   -1, -1, -1, -1, -1, -1, -1, -1,
   -1, -1, -1, -1, 32, 33, 34, 35,
   36, 37, -1, -1
};

#if !defined(CC_MTK_LOADER) || (defined(__KERNEL__) && !defined(CONFIG_MT53XX_NATIVE_GPIO))
static const INT8 _ai1PdwncInt2Callback[MAX_PDWNC_INT_ID_2] =
{
    0,  1,  2,  3,  4,  5,  6,  7,
   -1, -1, -1, -1, -1, -1, -1, -1,
    8,  9, 10, 11, -1, -1, -1, -1,
   -1, -1, 12, 13, -1, -1, -1, -1,
   14, 15, 16, 17, 18, 19
};
#endif
#endif

#if defined(CC_MT5882)
static const INT8 _ai1PdwncGpio2Int[TOTAL_PDWNC_GPIO_NUM] =
{
	  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, //0~9
	 10, 11, 12, -1, 13, -1, 14, -1, -1, -1, //10~19
	 -1, -1, -1, -1, -1, -1, -1, -1, 15, 16, //20~29
	 17, 18, 19, 20, 21, -1, -1, -1, -1, -1, //30~39
	 -1, -1, 		 						 //40~41
   
};
static const INT8 _ai1PdwncGpio2Wak[TOTAL_PDWNC_GPIO_NUM] =
{
	  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, //0~9    , OPCTRL 0~7
	 10, 11, 12, -1, -1, -1, -1, -1, -1, -1, //10~19  , OPCTRL 8~20
	 -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, //20~29  
	 -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, //30~39  
	 -1, -1, 								 //40~41
};
#endif

#if defined(__KERNEL__) && !defined(CONFIG_MT53XX_NATIVE_GPIO)
static UINT32 _au4PdwncGpioCallBackArg[TOTAL_PDWNC_GPIO_INT_NUM];
static UINT8 _au4PdwncCallBackFromLinuxAP[TOTAL_PDWNC_GPIO_INT_NUM];
#endif
//-----------------------------------------------------------------------------
// Prototypes 
//-----------------------------------------------------------------------------
//INT32 PDWNC_GpioEnable(INT32 i4Gpio, INT32 *pfgSet);
//INT32 PDWNC_GpioOutput(INT32 i4Gpio, INT32 *pfgSet);
//INT32 PDWNC_GpioInput(INT32 i4Gpio);
#ifndef CC_MTK_LOADER
INT32 PDWNC_GpioIntrq(INT32 i4Gpio, INT32 *pfgSet);
INT32 PDWNC_GpioQuery(INT32 i4Gpio, INT32 *pi4Intr, INT32 *pi4Fall, INT32 *pi4Rise);
INT32 PDWNC_GpioReg(INT32 i4Gpio, GPIO_TYPE eType, PFN_GPIO_CALLBACK pfnCallback);
INT32 PDWNC_T8032GpioReg(INT32 i4Gpio, GPIO_TYPE eType, PFN_GPIO_CALLBACK pfnCallback);
#endif


//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------
#ifndef CC_MTK_LOADER

#if defined(__KERNEL__) && !defined(CONFIG_MT53XX_NATIVE_GPIO)

extern INT32 _CB_PutEvent(CB_FCT_ID_T eFctId, INT32 i4TagSize, void *pvTag);

#endif

//-----------------------------------------------------------------------------
/** _PdwncGpioIsr() is the interrupt routine for GPIO pins of PDWNC module.
 *  @param u2IntIndex is the gpio number in PDWNC module to trigger interrupt.
 */
//-----------------------------------------------------------------------------
void _PdwncGpioIsr(UINT16 u2IntIndex)
#if defined(CC_MT5399)
{
#if !defined(CONFIG_MT53XX_NATIVE_GPIO)
    BOOL fgStatus;
    UNUSED(_ai1PdwncInt2Gpio);
    if (
        ((u2IntIndex < PDWNC_GPIO_INT_GROUP_0_NUM) \
        || ((u2IntIndex >= PDWNC_GPIO_INT1_BASE) && (u2IntIndex < PDWNC_GPIO_INT1_BASE + PDWNC_GPIO_INT_GROUP_1_NUM)) \
        || ((u2IntIndex >= PDWNC_GPIO_INT2_BASE) && (u2IntIndex < PDWNC_GPIO_INT2_BASE + PDWNC_GPIO_INT_GROUP_2_NUM)) \
        || ((u2IntIndex >= PDWNC_GPIO_INT3_BASE) && (u2IntIndex < PDWNC_GPIO_INT3_BASE + PDWNC_GPIO_INT_GROUP_3_NUM)) \
        )
        &&
        (_afnPDWNCGpioCallback[_ai1PdwncInt2Callback[u2IntIndex]]!=NULL))
    {
        fgStatus = PDWNC_GpioInput((INT32)_ai1PdwncInt2Gpio[u2IntIndex]);
		#ifdef __KERNEL__					 
		if(!_au4PdwncCallBackFromLinuxAP[_ai1PdwncInt2Callback[u2IntIndex]])
		{
			_afnPDWNCGpioCallback[_ai1PdwncInt2Callback[u2IntIndex]]((INT32)(_ai1PdwncInt2Gpio[u2IntIndex] + OPCTRL(0)), fgStatus);
		}
		else
		{
			MTGPIO_CB_T rGpioCb;
			rGpioCb.i4Gpio = (INT32)(_ai1PdwncInt2Gpio[u2IntIndex] + OPCTRL(0));
			rGpioCb.fgStatus = fgStatus;
			rGpioCb.u4Pfn = (UINT32)_afnPDWNCGpioCallback[_ai1PdwncInt2Callback[u2IntIndex]];
			rGpioCb.u4Arg = _au4PdwncGpioCallBackArg[_ai1PdwncInt2Callback[u2IntIndex]];
			//printf("_PdwncGpioIsr: gpio_%d, fgStatus: %d\n", rGpioCb.i4Gpio, rGpioCb.fgStatus);
			_CB_PutEvent(CB_MTAL_GPIO_TRIGGER, sizeof(MTGPIO_CB_T), &rGpioCb);
		}
		#else
		_afnPDWNCGpioCallback[_ai1PdwncInt2Callback[u2IntIndex]]((INT32)(_ai1PdwncInt2Gpio[u2IntIndex] + OPCTRL(0)), fgStatus); 
		#endif	
    }
	
#else
    UNUSED(_ai1PdwncInt2Gpio);
		if (
			(
			(u2IntIndex < PDWNC_GPIO_INT_GROUP_0_NUM) \
			|| ((u2IntIndex >= PDWNC_GPIO_INT1_BASE) && (u2IntIndex < PDWNC_GPIO_INT1_BASE + PDWNC_GPIO_INT_GROUP_1_NUM)) \
			|| ((u2IntIndex >= PDWNC_GPIO_INT2_BASE) && (u2IntIndex < PDWNC_GPIO_INT2_BASE + PDWNC_GPIO_INT_GROUP_2_NUM)) \
			|| ((u2IntIndex >= PDWNC_GPIO_INT3_BASE) && (u2IntIndex < PDWNC_GPIO_INT3_BASE + PDWNC_GPIO_INT_GROUP_3_NUM)) \
			)
			&&
			(_ai1PdwncInt2Gpio[u2IntIndex] != -1))
		{
			mtk_pdwnc_gpio_isr_func(u2IntIndex);
		}
#endif/* CONFIG_MT53XX_NATIVE_GPIO */

    // Still need to clear this non-hook interrupt.
    vIO32Write4B(PDWNC_INTCLR,  _PINT(u2IntIndex));
}
#elif defined(CC_MT5882) 
{
#if !defined(CONFIG_MT53XX_NATIVE_GPIO)
    BOOL fgStatus;
    //UNUSED(_ai1PdwncInt2Gpio);
    if (((u2IntIndex < TOTAL_PDWNC_GPIO_INT_NUM)) && (_afnPDWNCGpioCallback[_ai1PdwncInt2Gpio[u2IntIndex]]!=NULL))
    {
        fgStatus = PDWNC_GpioInput((INT32)_ai1PdwncInt2Gpio[u2IntIndex]);
		#ifdef __KERNEL__					 
		if(!_au4PdwncCallBackFromLinuxAP[_ai1PdwncInt2Gpio[u2IntIndex]])
		{
			_afnPDWNCGpioCallback[_ai1PdwncInt2Gpio[u2IntIndex]]((INT32)(_ai1PdwncInt2Gpio[u2IntIndex] + OPCTRL(0)), fgStatus);
		}
		else
		{
			MTGPIO_CB_T rGpioCb;
			rGpioCb.i4Gpio = (INT32)(_ai1PdwncInt2Gpio[u2IntIndex] + OPCTRL(0));
			rGpioCb.fgStatus = fgStatus;
			rGpioCb.u4Pfn = (UINT32)_afnPDWNCGpioCallback[_ai1PdwncInt2Gpio[u2IntIndex]];
			rGpioCb.u4Arg = _au4PdwncGpioCallBackArg[_ai1PdwncInt2Gpio[u2IntIndex]];
			//printf("_PdwncGpioIsr: gpio_%d, fgStatus: %d\n", rGpioCb.i4Gpio, rGpioCb.fgStatus);
			_CB_PutEvent(CB_MTAL_GPIO_TRIGGER, sizeof(MTGPIO_CB_T), &rGpioCb);
		}
		#else
		_afnPDWNCGpioCallback[_ai1PdwncInt2Gpio[u2IntIndex]]((INT32)(_ai1PdwncInt2Gpio[u2IntIndex] + OPCTRL(0)), fgStatus); 
		#endif	
    }
#else
    //UNUSED(_ai1PdwncInt2Gpio);
	if (_ai1PdwncGpio2Int[_ai1PdwncInt2Gpio[u2IntIndex]] != -1)
	{
		mtk_pdwnc_gpio_isr_func(u2IntIndex);
	}
#endif/* CONFIG_MT53XX_NATIVE_GPIO */
    // Still need to clear this non-hook interrupt.
    vIO32Write4B(PDWNC_INTCLR_2,_PINT(u2IntIndex));
}
#endif//CC_MT5399
#endif

//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/** _PDWNC_InitGpio(). Initialize the interrupt routine for GPIO irq.
 *  @param void
 */
//-----------------------------------------------------------------------------

void PDWNC_GPIO_Suspend(void)
{

}

void PDWNC_GPIO_Resume(void)
{	
    vIO32Write4B(PDWNC_ARM_INTEN, u4IO32Read4B(PDWNC_ARM_INTEN) & (~(REG_RW_PDWNC_INTEN_GPIO_MASK)) );
    vIO32Write4B(PDWNC_EXINT2ED, 0);
    vIO32Write4B(PDWNC_EXINTLEV, 0);
    vIO32Write4B(PDWNC_EXINTPOL, 0);
    vIO32Write4B(PDWNC_INTCLR, REG_RW_PDWNC_INTEN_GPIO_MASK); //david: just clean gpio int.
}

#if defined(__KERNEL__) && !defined(CONFIG_MT53XX_NATIVE_GPIO)
INT32 PDWNC_GpioSetIntExtParam(INT32 i4Gpio, UINT32 u4Arg)
{
	#if defined(CC_MT5399)
    _au4PdwncCallBackFromLinuxAP[_ai1PdwncInt2Callback[_ai1PdwncGpio2Int[i4Gpio]]] = 1;
    if(_ai1PdwncGpio2Int[i4Gpio] != -1)
    {
        _au4PdwncGpioCallBackArg[_ai1PdwncInt2Callback[_ai1PdwncGpio2Int[i4Gpio]]] = u4Arg;
        return 0;
    }
	#elif defined(CC_MT5882)
	if(_ai1PdwncGpio2Int[i4Gpio] != -1)
    {
    	_au4PdwncCallBackFromLinuxAP[_ai1PdwncGpio2Int[i4Gpio]] = 1;
        _au4PdwncGpioCallBackArg[_ai1PdwncGpio2Int[i4Gpio]] = u4Arg;
        return 0;
    }
	#endif
    else
    {
        return -1;        
    }
}

INT32 PDWNC_GpioGetIntExtParam(INT32 i4Gpio, UINT32 *pu4Arg)
{
    if(_ai1PdwncGpio2Int[i4Gpio] != -1)
    {
    #if defined(CC_MT5399)
        *pu4Arg = _au4PdwncGpioCallBackArg[_ai1PdwncInt2Callback[_ai1PdwncGpio2Int[i4Gpio]]];
	#elif defined(CC_MT5882)
		*pu4Arg = _au4PdwncGpioCallBackArg[_ai1PdwncGpio2Int[i4Gpio]];
	#endif
        return 0;
    }
    else
    {
        return -1;        
    }
}
#endif

#ifndef CC_MTK_LOADER
//-----------------------------------------------------------------------------
/** _PDWNC_InitGpio(). Initialize the interrupt routine for GPIO irq.
 *  @param void
 */
//-----------------------------------------------------------------------------
INT32 _PDWNC_InitGpio(void)
{ 
#if defined(CC_MT5399)
	UINT32 i;
    INT32 i4Val;
    void (* pfnOldIsr)(UINT16);
	// Hook PDWNC Gpio interrupt routine.
    for (i=0; i < MAX_PDWNC_INT_ID_2; i++)
    {
        if(_ai1PdwncInt2Callback[i] >= 0)
        {
            i4Val = PDWNC_RegIsr((UINT16)i, _PdwncGpioIsr, &pfnOldIsr);
            if (i4Val != PDWNC_OK)
            {
                return -1;
            }
        }
    }
#endif
#if !defined(CONFIG_MT53XX_NATIVE_GPIO)
#if defined(CC_MT5882)
	UINT32 i;
#endif
    // Clear callback function array.
    for (i=0; i<TOTAL_PDWNC_GPIO_INT_NUM;i++)
    {
        _afnPDWNCGpioCallback[i] = NULL;
		
		#ifdef __KERNEL__  
        _au4PdwncGpioCallBackArg[i] = 0;
        _au4PdwncCallBackFromLinuxAP[i] = 0;
		#endif
    }
    // Initialize interupt register.
    vIO32Write4B(PDWNC_ARM_INTEN, u4IO32Read4B(PDWNC_ARM_INTEN) & (~(REG_RW_PDWNC_INTEN_GPIO_MASK)));
	vIO32Write4B(PDWNC_ARM_INTEN_2,0);
    vIO32Write4B(PDWNC_EXINT2ED, 0);
    vIO32Write4B(PDWNC_EXINTLEV, 0);
    vIO32Write4B(PDWNC_EXINTPOL, 0);    
    vIO32Write4B(PDWNC_INTCLR, REG_RW_PDWNC_INTEN_GPIO_MASK); //david: just clean gpio int.
    #if defined(CC_MT5882)
	vIO32WriteFldAlign(PDWNC_INTCLR_2,0x3FFFFF,FLD_EXTIO_INTCLR);
	#else
	vIO32WriteFldAlign(PDWNC_INTCLR_2,0x7f,FLD_EXTIO_INTCLR);
	#endif
#endif
    return 0;
}
#endif


//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

INT32 PDWNC_GpioRangeCheck(INT32 i4Gpio)
{
		if ((i4Gpio >= OPCTRL(0)) && (i4Gpio <= (TOTAL_PDWNC_GPIO_NUM + OPCTRL(0))))
		{
			return 1;
		}
		else
		{	
			return 0;
		}	
}
//-----------------------------------------------------------------------------
/** PDWNC_GpioEnable() The GPIO input/output mode setting functions. It will
 *  check the i4Gpio and set to related register bit as 0 or 1.  In this 
 *  function, 0 is input mode and 1 is output mode.
 *  @param i4Gpio the gpio number to set or read.
 *  @param pfgSet A integer pointer.  If it's NULL, this function will return
 *  the current mode of gpio number (0 or 1).  If it's not NULL, it must
 *  reference to a integer.  If the integer is 0, this function will set the
 *  mode of the gpio number as input mode, otherwise set as output mode.
 *  @retval If pfgSet is NULL, it return 0 or 1 (0 is input mode, 1 is output
 *          mode.)  Otherwise, return (*pfgSet).
 */
//-----------------------------------------------------------------------------
INT32 PDWNC_GpioEnable(INT32 i4Gpio, INT32 *pfgSet)
{
    UINT32 u4Val;
    INT32 i4Idx;
#ifndef CC_MTK_LOADER
        CRIT_STATE_T rCrit;
#endif
    if ((i4Gpio < 0) || (i4Gpio >= TOTAL_PDWNC_GPIO_NUM))
    {
        return -1;
    }

    // Translate GPIO number to pad setting register offset
    i4Idx = i4Gpio;
    
#ifndef CC_MTK_LOADER
        rCrit = x_crit_start();
#endif /* CC_MTK_LOADER */


    if (i4Gpio <= 31)
    {
        u4Val = u4IO32Read4B(PDWNC_GPIOEN0);
    }
    else
    {
        u4Val = u4IO32Read4B(PDWNC_GPIOEN1);
    }

	i4Idx &= 0x1f;
	
	//for mustang 
	if(i4Gpio >= 47)
	{
		i4Idx++;
	}	
	
    if (pfgSet == NULL)
    {
#ifndef CC_MTK_LOADER
        x_crit_end(rCrit);         
#endif /* CC_MTK_LOADER */
        return ((u4Val & (1U << i4Idx)) ? 1 : 0);
    }
  	
	
    u4Val = (*pfgSet) ?
                (u4Val | (1U << i4Idx)) :
                (u4Val & ~(1U << i4Idx));
				

    if (i4Gpio <= 31)
    {
        vIO32Write4B(PDWNC_GPIOEN0, u4Val);
    }
    else
    {
    	#if defined(CC_MT5882)
		vIO32Write4B(PDWNC_GPIOEN1, u4Val | (1<<15)); //uart enable bit.
		#else
        vIO32Write4B(PDWNC_GPIOEN1, u4Val);
		#endif
    }


#ifndef CC_MTK_LOADER
        x_crit_end(rCrit);         
#endif /* CC_MTK_LOADER */ 
    return (*pfgSet);
}

//-----------------------------------------------------------------------------
/** PDWNC_GpioOutput() The GPIO output value setting functions. It will check
 *  the i4Gpio and set to related register bit as 0 or 1.
 *  @param i4Gpio the gpio number to set or read.
 *  @param pfgSet A integer pointer.  If it's NULL, this function will return
 *          the bit of gpio number (0 or 1).  If it's not NULL, it must 
 *          reference to a integer.  If the integer is 0, this function 
 *          will set the bit of the gpio number as 0, otherwise set as 1.
 *  @retval If pfgSet is NULL, it return 0 or 1 (the bit value of the gpio
 *          number of output mode.  Otherwise, return (*pfgSet).
 */
//-----------------------------------------------------------------------------
INT32 PDWNC_GpioOutput(INT32 i4Gpio, INT32 *pfgSet)
{
    UINT32 u4Val,u4Val1;
    INT32 i4Idx,i4Val;
#ifndef CC_MTK_LOADER
        CRIT_STATE_T rCrit;
#endif 
    if ((i4Gpio < 0) || (i4Gpio >= TOTAL_PDWNC_GPIO_NUM))
    {
        return -1;
    }

    // Translate GPIO number to pad setting register offset
    i4Idx = i4Gpio;	
	
#ifndef CC_MTK_LOADER
        rCrit = x_crit_start();
#endif /* CC_MTK_LOADER */

	BSP_PinGpioSel(i4Gpio +  OPCTRL(0));
	
	i4Idx &= 0x1f;
	
    if (pfgSet == NULL)	//NULL: for query gpio status, must be GPIO output , but not change pin level
    {
#ifndef CC_MTK_LOADER
        x_crit_end(rCrit);         
#endif /* CC_MTK_LOADER */
		//if input mode, get input value and set the value for output valeu ,then change to be output
		if(PDWNC_GpioEnable(i4Gpio, NULL) == 0)
		{
			//get pin level		
		    if (i4Gpio <= 31)
		    {
		        u4Val1 = IO32ReadFldAlign(PDWNC_GPIOIN0, FLD_PD_GPIO_IN0);		
		    }
		    else
		    {
		        u4Val1 = IO32ReadFldAlign(PDWNC_GPIOIN1, FLD_PD_GPIO_IN1);
		    }
			//get current out register setting
			if (i4Gpio <= 31)
			{
				u4Val = u4IO32Read4B(PDWNC_GPIOOUT0);
			}
			else
			{
				u4Val = u4IO32Read4B(PDWNC_GPIOOUT1);
			}
			u4Val = (u4Val1 & (1U << i4Idx)) ?
						(u4Val | (1U << i4Idx)) :
						(u4Val & ~(1U << i4Idx));
			if (i4Gpio <= 31)
			{
				vIO32Write4B(PDWNC_GPIOOUT0, u4Val);
			}
			else
			{
				vIO32Write4B(PDWNC_GPIOOUT1, u4Val);
			}
		
			// Set the output mode.
			i4Val = 1;
			VERIFY(1 == PDWNC_GpioEnable(i4Gpio, &i4Val));

		}
		
	//get out value
		if (i4Gpio <= 31)
		{
			u4Val = u4IO32Read4B(PDWNC_GPIOOUT0);
		}
		else
		{
			u4Val = u4IO32Read4B(PDWNC_GPIOOUT1);
		}

        return ((u4Val & (1U << i4Idx)) ? 1 : 0);
    }

//get out value
	if (i4Gpio <= 31)
	{
		u4Val = u4IO32Read4B(PDWNC_GPIOOUT0);
	}
	else
	{
		u4Val = u4IO32Read4B(PDWNC_GPIOOUT1);
	}

    u4Val = (*pfgSet) ?
                (u4Val | (1U << i4Idx)) :
                (u4Val & ~(1U << i4Idx));
				
    if (i4Gpio <= 31)
    {
        vIO32Write4B(PDWNC_GPIOOUT0, u4Val);
    }
    else
    {
        vIO32Write4B(PDWNC_GPIOOUT1, u4Val);
    }

#ifndef CC_MTK_LOADER
        x_crit_end(rCrit);         
#endif /* CC_MTK_LOADER */

    // Set the output mode.
    i4Val = 1;
    VERIFY(1 == PDWNC_GpioEnable(i4Gpio, &i4Val));
 
    return (*pfgSet);
}

//-----------------------------------------------------------------------------
/** PDWNC_GpioInput()  The GPIO input reading functions. It will check the 
 *  i4Gpio and read related register bit to return.
 *  @param i4Gpio the gpio number to read.
 *  @retval 0 or 1.  (GPIO input value.)
 */
//-----------------------------------------------------------------------------
INT32 PDWNC_GpioInput(INT32 i4Gpio)
{
    UINT32 u4Val;
    INT32 i4Idx = 0;

    if ((i4Gpio < 0) || (i4Gpio >= TOTAL_PDWNC_GPIO_NUM))
    {
        return -1;
    }

    // Translate GPIO number to pad setting register offset
    i4Idx = i4Gpio;
    
//    _PdwncSetGpioPin(i4Gpio);
    BSP_PinGpioSel(i4Gpio +  OPCTRL(0));

    if (i4Gpio <= 31)
    {
        u4Val = IO32ReadFldAlign(PDWNC_GPIOIN0, FLD_PD_GPIO_IN0);		
		return ((u4Val & (1U << i4Idx)) ? 1U : 0);	
    }
    else
    {
        u4Val = IO32ReadFldAlign(PDWNC_GPIOIN1, FLD_PD_GPIO_IN1);
		i4Idx &= 0x1f;
		return ((u4Val & (1U << i4Idx)) ? 1U : 0);	
    }
}

INT32 PDWNC_GpioStatus(INT32 i4Gpio, INT32* pi4Pinmux, INT32* pi4Enable, INT32* pi4Output, INT32* pi4Input)
{
    UINT32 u4Val;
    INT32 i4Idx = 0;


    if ((i4Gpio < 0) || (i4Gpio >= TOTAL_PDWNC_GPIO_NUM))
    {
        return -1;
    }
    
    *pi4Pinmux = BSP_PinGpioGet(i4Gpio +  OPCTRL(0), &u4Val);

    // Translate GPIO number to pad setting register offset
    i4Idx = i4Gpio;
	i4Idx &= 0x1f;

    if (i4Gpio <= 31)
    {
        u4Val = u4IO32Read4B(PDWNC_GPIOEN0);
    }
    else
    {
        u4Val = u4IO32Read4B(PDWNC_GPIOEN1);
    }
  	
	//for mustang 
	if(i4Gpio >= 47)
	{
		i4Idx++;
	}	
	
    *pi4Enable = ((u4Val & (1U << i4Idx)) ? 1 : 0);

    if (i4Gpio <= 31)
    {
        u4Val = u4IO32Read4B(PDWNC_GPIOOUT0);
    }
    else
    {
        u4Val = u4IO32Read4B(PDWNC_GPIOOUT1);
    }
    *pi4Output = ((u4Val & (1U << i4Idx)) ? 1 : 0);

    if (i4Gpio <= 31)
    {
        u4Val = IO32ReadFldAlign(PDWNC_GPIOIN0, FLD_PD_GPIO_IN0);		
    }
    else
    {
        u4Val = IO32ReadFldAlign(PDWNC_GPIOIN1, FLD_PD_GPIO_IN1);
    }
    *pi4Input = ((u4Val & (1U << i4Idx)) ? 1U : 0);
	
    return 0;
}


#ifndef CC_MTK_LOADER
//-----------------------------------------------------------------------------
/** PDWNC_GpioIntrq() The GPIO interrupt enable setting functions. It will
 *  check the i4Gpio and set to related register bit as 0 or 1.  In this 
 *  function, 0 is interrupt disable mode and 1 is interrupt enable mode.
 *  @param i4Gpio the gpio number to set or read.
 *  @param pfgSet A integer pointer.  If it's NULL, this function will return
 *  the current setting of gpio number (0 or 1).  If it's not NULL, it must
 *  reference to a integer.  If the integer is 0, this function will set the
 *  mode of the gpio number as interrupt disable mode, otherwise set as
 *  interrupt enable mode.
 *  @retval If pfgSet is NULL, it return 0 or 1 (0 is interrupt disable mode,
 *          1 is interrupt enable mode.)  Otherwise, return (*pfgSet).
 */
//-----------------------------------------------------------------------------
INT32 PDWNC_GpioIntrq(INT32 i4Gpio, INT32 *pfgSet)
{
    UINT32 u4Val;
    INT32 i4Int;
    UNUSED(_ai1PdwncGpio2Int);
    if ((i4Gpio < 0) || (i4Gpio >= TOTAL_PDWNC_GPIO_NUM))
    {
        return -1;
    }

    if((i4Int = _ai1PdwncGpio2Int[i4Gpio]) < 0)
    {
        return -1;    
    }

	#if defined(CC_MT5882)
	u4Val = u4IO32Read4B(PDWNC_ARM_INTEN_2);
	#else //MT5399
	if(_ai1PdwncGpio2Int[i4Gpio] >= MAX_PDWNC_INT_ID)
	{
		u4Val = u4IO32Read4B(PDWNC_ARM_INTEN_2);
		i4Int = i4Int - MAX_PDWNC_INT_ID;
	}
	else
	{
	    u4Val = u4IO32Read4B(PDWNC_ARM_INTEN);
	}
	#endif //CC_MT5882
    if (pfgSet == NULL)
    {
        return ((u4Val & (1U << i4Int)) ? 1 : 0);
    }
    u4Val = (*pfgSet) ?
            (u4Val | OPCTRL_INTEN(i4Int)) :
            (u4Val & ~ OPCTRL_INTEN(i4Int));

	#if defined(CC_MT5882)
	vIO32Write4B(PDWNC_INTCLR_2, OPCTRL_INTEN(i4Int)); //just clean gpio int.
	vIO32Write4B(PDWNC_ARM_INTEN_2, u4Val);
	#else //MT5399
	if(_ai1PdwncGpio2Int[i4Gpio] >= MAX_PDWNC_INT_ID)
	{
		vIO32Write4B(PDWNC_ARM_INTEN_2, u4Val);
	}
    else 
	{
		vIO32Write4B(PDWNC_ARM_INTEN, u4Val);
	}
	#endif //CC_MT5882
    return (*pfgSet);
}

//-----------------------------------------------------------------------------
/* PDWNC_GpioQuery() to query PDWNC Gpio settings.
 *  @param i4Gpio should be between 0~7.
 *  @param pi4Intr to get interrupt enable/disable status.
 *  @param pi4Fall to get falling callback enable/disable status.
 *  @param pi4Rise to get rising callback enable/disable status.
 *  @retval return 0 if successful, otherwise return -1.
 */
//-----------------------------------------------------------------------------
INT32 PDWNC_GpioQuery(INT32 i4Gpio, INT32 *pi4Intr, INT32 *pi4Fall, INT32 *pi4Rise)
{
    return 0;
}

//-----------------------------------------------------------------------------
/* PDWNC_GpioReg() to setup the PDWNC Gpio interrupt callback function, type,
 *      and state.
 *  @param i4Gpio should be between 0~7.
 *  @param eType should be one of enum GPIO_TYPE.
 *  @param pfnCallback the callback function.
 *  @retval 0 successful, -1 failed.
 */
//-----------------------------------------------------------------------------

INT32 PDWNC_T8032GpioReg(INT32 i4Gpio, GPIO_TYPE eType, PFN_GPIO_CALLBACK pfnCallback)
{

    UINT32 u4Val;
    INT32 i4Int;

	if(i4Gpio >= 200)
		i4Gpio -= 200;
    if ((i4Gpio < 0) || (i4Gpio >= TOTAL_PDWNC_GPIO_NUM))
    {
        return -1;
    }

    if((i4Int = _ai1PdwncGpio2Int[i4Gpio]) < 0)    
    {
        return -1;        
    }
	//for mustang
	#if defined(CC_MT5399)
	if(_ai1PdwncGpio2Int[i4Gpio] >= MAX_PDWNC_INT_ID)
	{
		i4Int = i4Int -12; //gpio50: init 32~37---contrld bit 20~25
	}
	#endif
   /* Set the register and callback function. */
    switch(eType)
    {
        case GPIO_TYPE_INTR_FALL:
            u4Val = u4IO32Read4B(PDWNC_EXINT2ED);
            u4Val &= ~(1U << i4Int); // Disable double edge trigger.
            vIO32Write4B(PDWNC_EXINT2ED, u4Val);
            u4Val = u4IO32Read4B(PDWNC_EXINTLEV);
            u4Val &= ~(1U << i4Int);  // Set Edge trigger.
            vIO32Write4B(PDWNC_EXINTLEV, u4Val);
            u4Val = u4IO32Read4B(PDWNC_EXINTPOL);
            u4Val &= ~(1U << i4Int);       // Set Falling Edge interrupt.
            vIO32Write4B(PDWNC_EXINTPOL, u4Val);
            break;
            
        case GPIO_TYPE_INTR_RISE:            
            u4Val = u4IO32Read4B(PDWNC_EXINT2ED);
            u4Val &= ~(1U << i4Int); // Disable double edge trigger.
            vIO32Write4B(PDWNC_EXINT2ED, u4Val);
            u4Val = u4IO32Read4B(PDWNC_EXINTLEV);
            u4Val &= ~(1U << i4Int);  // Set Edge trigger.
            vIO32Write4B(PDWNC_EXINTLEV, u4Val);
            u4Val = u4IO32Read4B(PDWNC_EXINTPOL);
            u4Val |= (1U << i4Int);       // Set rasing Edge interrupt.
            vIO32Write4B(PDWNC_EXINTPOL, u4Val);
            break;               
            
        case GPIO_TYPE_INTR_BOTH:            
            u4Val = u4IO32Read4B(PDWNC_EXINT2ED);
            u4Val |= (1U << i4Int); // enable double edge trigger.
            vIO32Write4B(PDWNC_EXINT2ED, u4Val);
            u4Val = u4IO32Read4B(PDWNC_EXINTLEV);
            u4Val &= ~(1U << i4Int);  // Set Edge trigger.
            vIO32Write4B(PDWNC_EXINTLEV, u4Val);
            break;
            
        case GPIO_TYPE_INTR_LEVEL_LOW:            
            u4Val = u4IO32Read4B(PDWNC_EXINT2ED);
            u4Val &= ~(1U << i4Int); // Disable double edge trigger.
            vIO32Write4B(PDWNC_EXINT2ED, u4Val);
            u4Val = u4IO32Read4B(PDWNC_EXINTLEV);
            u4Val |= (1U << i4Int);  // Set Level trigger.
            vIO32Write4B(PDWNC_EXINTLEV, u4Val);
            u4Val = u4IO32Read4B(PDWNC_EXINTPOL);
            u4Val &= ~(1U << i4Int);       // Set level low interrupt.
            vIO32Write4B(PDWNC_EXINTPOL, u4Val);
            break;
            
        case GPIO_TYPE_INTR_LEVEL_HIGH:            
            u4Val = u4IO32Read4B(PDWNC_EXINT2ED);
            u4Val &= ~(1U << i4Int); // Disable double edge trigger.
            vIO32Write4B(PDWNC_EXINT2ED, u4Val);
            u4Val = u4IO32Read4B(PDWNC_EXINTLEV);
            u4Val |= (1U << i4Int);  // Set level trigger.
            vIO32Write4B(PDWNC_EXINTLEV, u4Val);
            u4Val = u4IO32Read4B(PDWNC_EXINTPOL);
            u4Val |= (1U << i4Int);       // Set level high interrupt.
            vIO32Write4B(PDWNC_EXINTPOL, u4Val);
            break;
            
        default:
			#if defined(CC_MT5882)
			u4Val = u4IO32Read4B(PDWNC_T8032_INTEN_2);//PDWNC_T8032_INTEN
			u4Val &= ~(1U << i4Int);	   // Disable interrupt.
			vIO32Write4B(PDWNC_T8032_INTEN_2, u4Val);
			#else//MT5399
			if(_ai1PdwncGpio2Int[i4Gpio] >= MAX_PDWNC_INT_ID)
			{
				u4Val = u4IO32Read4B(PDWNC_T8032_INTEN_2);//PDWNC_T8032_INTEN
				u4Val &= ~(1U << (_ai1PdwncGpio2Int[i4Gpio] - MAX_PDWNC_INT_ID));	   // disable interrupt.
				vIO32Write4B(PDWNC_T8032_INTEN_2, u4Val);
			}
			else
			{	
				u4Val = u4IO32Read4B(PDWNC_T8032_INTEN);//PDWNC_T8032_INTEN
				u4Val &= ~(1U << i4Int);	   // Disable interrupt.
				vIO32Write4B(PDWNC_T8032_INTEN, u4Val);
			}
			#endif//CC_MT5882
            return 0;
        }
	
	BSP_PinGpioSel(i4Gpio + OPCTRL(0));//pinmux 

	#if defined(CC_MT5882)
	u4Val = u4IO32Read4B(PDWNC_T8032_INTEN_2);//PDWNC_T8032_INTEN
	u4Val &= ~(1U << i4Int);	   // Disable interrupt.
	vIO32Write4B(PDWNC_T8032_INTEN_2, u4Val);
	#else //MT5399
	if(_ai1PdwncGpio2Int[i4Gpio] >= MAX_PDWNC_INT_ID)
	{
		u4Val = u4IO32Read4B(PDWNC_T8032_INTEN_2);//PDWNC_T8032_INTEN
		u4Val |= (1U << (_ai1PdwncGpio2Int[i4Gpio] - MAX_PDWNC_INT_ID)); 	   // Enable interrupt.
		vIO32Write4B(PDWNC_T8032_INTEN_2, u4Val);
	}
	else
	{	
		u4Val = u4IO32Read4B(PDWNC_T8032_INTEN);//PDWNC_T8032_INTEN
		u4Val |= (1U << i4Int); 	   // Enable interrupt.
		vIO32Write4B(PDWNC_T8032_INTEN, u4Val);
	}
	#endif //CC_MT5882
	UNUSED(pfnCallback);
	#if 0
    if (pfnCallback)
    {
        //_afnPDWNCGpioCallback[i4Gpio] = pfnCallback;
		_afnPDWNCGpioCallback[_ai1PdwncInt2Callback[_ai1PdwncGpio2Int[i4Gpio]]] = pfnCallback;
    }
	#endif
    return 0;
}


INT32 PDWNC_GpioReg(INT32 i4Gpio, GPIO_TYPE eType, PFN_GPIO_CALLBACK pfnCallback)
{
    UINT32 u4Val;
    INT32 i4Int;

	if(i4Gpio >= 200)
		i4Gpio -= 200;
	
    if ((i4Gpio < 0) || (i4Gpio >= TOTAL_PDWNC_GPIO_NUM))
    {
        return -1;
    }
	
#ifndef CC_FAST_INIT
    if((eType != GPIO_TYPE_NONE) && (pfnCallback == NULL))
    {
        return -1;        
    }
#endif

    if((i4Int = _ai1PdwncGpio2Int[i4Gpio]) < 0)    
    {
        return -1;        
    }
	
	//for mustang
	#if defined(CC_MT5399)
	if(_ai1PdwncGpio2Int[i4Gpio] >= MAX_PDWNC_INT_ID)
	{
		i4Int = i4Int -12; //gpio50: init 32~37---contrld bit 20~25
	}
	#endif
     /* Set the register and callback function. */
    switch(eType)
    {
        case GPIO_TYPE_INTR_FALL:
            u4Val = u4IO32Read4B(PDWNC_EXINT2ED);
            u4Val &= ~(1U << i4Int); // Disable double edge trigger.
            vIO32Write4B(PDWNC_EXINT2ED, u4Val);
            u4Val = u4IO32Read4B(PDWNC_EXINTLEV);
            u4Val &= ~(1U << i4Int);  // Set Edge trigger.
            vIO32Write4B(PDWNC_EXINTLEV, u4Val);
            u4Val = u4IO32Read4B(PDWNC_EXINTPOL);
            u4Val &= ~(1U << i4Int);       // Set Falling Edge interrupt.
            vIO32Write4B(PDWNC_EXINTPOL, u4Val);
            break;
            
        case GPIO_TYPE_INTR_RISE:            
            u4Val = u4IO32Read4B(PDWNC_EXINT2ED);
            u4Val &= ~(1U << i4Int); // Disable double edge trigger.
            vIO32Write4B(PDWNC_EXINT2ED, u4Val);
            u4Val = u4IO32Read4B(PDWNC_EXINTLEV);
            u4Val &= ~(1U << i4Int);  // Set Edge trigger.
            vIO32Write4B(PDWNC_EXINTLEV, u4Val);
            u4Val = u4IO32Read4B(PDWNC_EXINTPOL);
            u4Val |= (1U << i4Int);       // Set rasing Edge interrupt.
            vIO32Write4B(PDWNC_EXINTPOL, u4Val);
            break;               
            
        case GPIO_TYPE_INTR_BOTH:            
            u4Val = u4IO32Read4B(PDWNC_EXINT2ED);
            u4Val |= (1U << i4Int); // enable double edge trigger.
            vIO32Write4B(PDWNC_EXINT2ED, u4Val);
            u4Val = u4IO32Read4B(PDWNC_EXINTLEV);
            u4Val &= ~(1U << i4Int);  // Set Edge trigger.
            vIO32Write4B(PDWNC_EXINTLEV, u4Val);
            break;
            
        case GPIO_TYPE_INTR_LEVEL_LOW:            
            u4Val = u4IO32Read4B(PDWNC_EXINT2ED);
            u4Val &= ~(1U << i4Int); // Disable double edge trigger.
            vIO32Write4B(PDWNC_EXINT2ED, u4Val);
            u4Val = u4IO32Read4B(PDWNC_EXINTLEV);
            u4Val |= (1U << i4Int);  // Set Level trigger.
            vIO32Write4B(PDWNC_EXINTLEV, u4Val);
            u4Val = u4IO32Read4B(PDWNC_EXINTPOL);
            u4Val &= ~(1U << i4Int);       // Set level low interrupt.
            vIO32Write4B(PDWNC_EXINTPOL, u4Val);
            break;
            
        case GPIO_TYPE_INTR_LEVEL_HIGH:            
            u4Val = u4IO32Read4B(PDWNC_EXINT2ED);
            u4Val &= ~(1U << i4Int); // Disable double edge trigger.
            vIO32Write4B(PDWNC_EXINT2ED, u4Val);
            u4Val = u4IO32Read4B(PDWNC_EXINTLEV);
            u4Val |= (1U << i4Int);  // Set level trigger.
            vIO32Write4B(PDWNC_EXINTLEV, u4Val);
            u4Val = u4IO32Read4B(PDWNC_EXINTPOL);
            u4Val |= (1U << i4Int);       // Set level high interrupt.
            vIO32Write4B(PDWNC_EXINTPOL, u4Val);
            break;
            
        default:
			#if defined(CC_MT5882)
			u4Val = 0;
			PDWNC_GpioIntrq(i4Int,(INT32 *)&u4Val);// Disable interrupt.
			#else//MT5399
			if(_ai1PdwncGpio2Int[i4Gpio] >= MAX_PDWNC_INT_ID)
			{
				u4Val = u4IO32Read4B(PDWNC_ARM_INTEN_2);//PDWNC_T8032_INTEN
				u4Val &= ~(1U << (_ai1PdwncGpio2Int[i4Gpio] - MAX_PDWNC_INT_ID));	   // disable interrupt.
				vIO32Write4B(PDWNC_ARM_INTEN_2, u4Val);
			}
			else
			{	
				u4Val = u4IO32Read4B(PDWNC_ARM_INTEN);
				u4Val &= ~(1U << i4Int);	   // Disable interrupt.
				vIO32Write4B(PDWNC_ARM_INTEN, u4Val);
			}
			#endif//CC_MT5882
			return 0;
        }

	BSP_PinGpioSel(i4Gpio + OPCTRL(0));//pinmux 

	#if defined(CC_MT5882)
	u4Val = 1;
	PDWNC_GpioIntrq(i4Gpio,(INT32 *)&u4Val);// Enable interrupt.
	#else//MT5399
	if(_ai1PdwncGpio2Int[i4Gpio] >= MAX_PDWNC_INT_ID)
	{
		u4Val = u4IO32Read4B(PDWNC_ARM_INTEN_2);//PDWNC_T8032_INTEN
		u4Val |= (1U << (_ai1PdwncGpio2Int[i4Gpio] - MAX_PDWNC_INT_ID));	   // Enable interrupt.
		vIO32Write4B(PDWNC_ARM_INTEN_2, u4Val);
	}
	else
	{	
		u4Val = u4IO32Read4B(PDWNC_ARM_INTEN);
		u4Val |= (1U << i4Int); 	   // Enable interrupt.
		vIO32Write4B(PDWNC_ARM_INTEN, u4Val);
	}
	#endif//CC_MT5882
    if (pfnCallback)
    {
    	#if defined(CC_MT5882)
        _afnPDWNCGpioCallback[i4Gpio] = pfnCallback;
		#else//MT5399
		_afnPDWNCGpioCallback[_ai1PdwncInt2Callback[_ai1PdwncGpio2Int[i4Gpio]]] = pfnCallback;
		#endif
    }
//    printf("Interrupt seting is:0x%8x,0x%8x,0x%8x\n",u4IO32Read4B(PDWNC_EXINT2ED),u4IO32Read4B(PDWNC_EXINTLEV),u4IO32Read4B(PDWNC_EXINTPOL));
    
    return 0;
}
#if 0
INT32 PDWNC_GpioResumeINTEN(INT32 i4Gpio, GPIO_TYPE eType)
{
//note: At resume stage, pass NULL will not overwrite _afnPDWNCGpioCallback[] which setup at initial stage    
    return PDWNC_GpioReg(i4Gpio, eType, NULL);
}
#endif
#else

INT32 PDWNC_GpioIntrq(INT32 i4Gpio, INT32 *pfgSet)
{
	
	return -1;
}
INT32 PDWNC_GpioQuery(INT32 i4Gpio, INT32 *pi4Intr, INT32 *pi4Fall, INT32 *pi4Rise)
{
	return -1;
}
INT32 PDWNC_GpioReg(INT32 i4Gpio, GPIO_TYPE eType, PFN_GPIO_CALLBACK pfnCallback)
{
	return -1;
}
#if 0
INT32 PDWNC_T8032GpioReg(INT32 i4Gpio, GPIO_TYPE eType, PFN_GPIO_CALLBACK pfnCallback)
{
	return -1;
}
INT32 PDWNC_GpioResumeINTEN(INT32 i4Gpio, GPIO_TYPE eType)
{
	return -1;
}
#endif
#endif /* CC_MTK_LOADER */

void _PDWNC_SetupGpioWakeup(UINT32 u4Gpio, UINT32 u4Pol,UINT32 *pu4PowerDownEn)
{
	INT32 i4Val;
	UINT32 u4Val;
	// Set to input mode.
    i4Val = 0;
    UNUSED(GPIO_Enable((INT32)u4Gpio, &i4Val));
    // Set the wakeup polarity.
    //i4Val = (INT32)u4Pol;
    u4Gpio = u4Gpio - OPCTRL(0);
	#if defined(CC_MT5882)
	if(u4Gpio <= 12)
	#else
    if(u4Gpio <= TOTAL_PDWNC_DEDICATE_GPI_NUM)
	#endif
    {                
        u4Val =  IO32ReadFldAlign(PDWNC_PDIO,FLD_EXTIO_POL);
        if (u4Pol)
        {
            u4Val |= (GPIOX_POL(u4Gpio));
        }
        else
        {
            u4Val &= ~(GPIOX_POL(u4Gpio));
        }
        vIO32WriteFldAlign(PDWNC_PDIO, u4Val,FLD_EXTIO_POL);
    }

	if(u4Gpio <= 7)
	{
		*pu4PowerDownEn |= GPIO_WAKEN(WAK_GPIOX(u4Gpio));
	}
	#if defined(CC_MT5882)
	else if((u4Gpio >= 8) && (u4Gpio <= 12))
	#else
	else if((u4Gpio >= 8) && (u4Gpio <= 11))
	#endif
	{
		*pu4PowerDownEn |= (1U << (u4Gpio + 2));
	}
}

//-----------------------------------------------------------------------------
/** _PdwncSetupGPIO() to setup PDWNC module GPIO wakeup settings
 *  @param pu4PowerDownEn is the power down wakeup enable register value.
 */
//-----------------------------------------------------------------------------
void _PDWNC_SetupGPIO(UINT32 *pu4PowerDownEn)
{
    INT32 i, i4Val;
    UINT32 u4Val, u4Gpio, u4DkBit;
    SGL_GPIO_T   *prSingleGpioKeyList = NULL;
    GREED_KEY_T  *prGreedGpioKeyList = NULL;

    ASSERT(pu4PowerDownEn != NULL);

    if (!PDWNC_SetupFpWakeup(NULL))
    {
        LOG(1,"FP wakeup disable\n");
        return;
    }

    u4DkBit = DRVCUST_InitGet(eKeyPadWakeupBit);
    i = DRVCUST_InitQuery(eSingleGpioKeyList, (UINT32 *)(void *)&prSingleGpioKeyList);
    if (i < 0)
    {
        return;
    }
    if (!((*pu4PowerDownEn) & WAK_IRRX))
    {
        u4DkBit = DKBIT_POWER;
    }

    // Do the GPIO wakeup initialization
    i = 0;
    while ((prSingleGpioKeyList) &&
            (prSingleGpioKeyList[i].u4Key != 0) &&
            (prSingleGpioKeyList[i].u4Key != DKBIT_NONE))
    {
        if (prSingleGpioKeyList[i].u4Key & u4DkBit)
        {
        	#if 0
            u4Gpio = prSingleGpioKeyList[i].u4Gpio - OPCTRL(0);
            if (u4Gpio > TOTAL_PDWNC_DEDICATE_GPI_NUM || _ai1PdwncGpio2Int[u4Gpio] < 0) //only use first 8 gpio
            {
                LOG(1, "Error Power button setting\n");
                i++;
                continue;
            }
            u4Val = u4IO32Read4B(PDWNC_PDIO);
            if (prSingleGpioKeyList[i].u4Val)
            {
                u4Val |= (GPIOX_POL(u4Gpio));
            }
            else
            {
                u4Val &= ~(GPIOX_POL(u4Gpio));
            }
            vIO32Write4B(PDWNC_PDIO, u4Val);
			#endif
            *pu4PowerDownEn |= GPIO_WAKEN(WAK_GPIOX(u4Gpio));
			_PDWNC_SetupGpioWakeup(prSingleGpioKeyList[i].u4Gpio,prSingleGpioKeyList[i].u4Val,pu4PowerDownEn);
        }
        i++;
    }
    i = DRVCUST_InitQuery(eGreedGpioKeyList, (UINT32 *)(void *)&prGreedGpioKeyList);
    if (i < 0)
    {
        return;
    }

    // Do the GPIO wakeup initialization
    i = 0;
    while ((prGreedGpioKeyList) &&
            (prGreedGpioKeyList[i].u4Key != 0) &&
            (prGreedGpioKeyList[i].u4Key != DKBIT_NONE))
    {
        if (prGreedGpioKeyList[i].u4Key & u4DkBit)
        {
            u4Gpio = prGreedGpioKeyList[i].u4GpioOut;
            i4Val = (INT32)prGreedGpioKeyList[i].u4Val;
            UNUSED(GPIO_Output((INT32)u4Gpio, &i4Val));
            u4Gpio = prGreedGpioKeyList[i].u4GpioIn - OPCTRL(0);
            if (u4Gpio > TOTAL_PDWNC_DEDICATE_GPI_NUM) //|| _ai1PdwncGpio2Int[u4Gpio] < 0)
            {
                LOG(1, "Error Power button setting\n");
                i++;
                continue;
            }
            u4Val = u4IO32Read4B(PDWNC_PDIO);
            if (prGreedGpioKeyList[i].u4Val)
            {
                u4Val |= (GPIOX_POL(u4Gpio));
            }
            else
            {
                u4Val &= ~(GPIOX_POL(u4Gpio));
            }
            vIO32Write4B(PDWNC_PDIO, u4Val);
            *pu4PowerDownEn |= GPIO_WAKEN(WAK_GPIOX(u4Gpio));
        }
        i++;
    }

    // Do the special GPIO wakeup initialization
    i = DRVCUST_InitQuery(eSpecialWakeupGpio, (UINT32 *)&u4Gpio);
    if (i < 0)
    {
        return;
    }
    i = DRVCUST_InitQuery(eSpecialWakeupOnVal, (UINT32 *)&u4Val);
    if (i < 0)
    {
        return;
    }
    // Set to input mode.
    i4Val = 0;
    UNUSED(GPIO_Enable((INT32)u4Gpio, &i4Val));
    // Set the wakeup polarity.
    i4Val = (INT32)u4Val;
    u4Gpio = u4Gpio - OPCTRL(0);

    if(u4Gpio < TOTAL_PDWNC_DEDICATE_GPI_NUM)
    {                
        u4Val =  u4IO32Read4B(PDWNC_PDIO);
        if (i4Val)
        {
            u4Val |= (GPIOX_POL(u4Gpio));
        }
        else
        {
            u4Val &= ~(GPIOX_POL(u4Gpio));
        }
        vIO32Write4B(PDWNC_PDIO, u4Val);
    }
    
    *pu4PowerDownEn |= GPIO_WAKEN(WAK_GPIOX(u4Gpio));
}


//-----------------------------------------------------------------------------
/** _PDWNC_RegLedBlinker() Register/Unregister an GPIO as LED blinker
 *  @param i4Gpio the gpio idx to be used as LED blinker.
 *  @param fgEnable enable/disable the gpio as LED blinker. 
 *  @param u1RoundPeriod blinking dark/light cycle time (unit: 0.5 sec).  
 *  @param u1LightPeriod blinking dark time in a cycle time (unit: 0.5 sec).   
 *  @retval blinker gpio idx if success; -1 if fail
 */
//-----------------------------------------------------------------------------
INT32 _PDWNC_RegLedBlinker(INT32 i4Gpio, BOOL fgEnable, UINT8 u1RoundPeriod, UINT8 u1LightPeriod)
{
	#if 0 //todo I don't what is this for 
	UINT32 u4Val;
    INT32 i4Val = 1;
    if(i4Gpio > OPCTRL(5) - OPCTRL(0) )
    {
        return -1;
    }

    if(fgEnable)
    {
        UNUSED(PDWNC_GpioEnable(i4Gpio, &i4Val));
        u4Val =  u4IO32Read4B(PDWNC_PDMISC) | (1U << i4Gpio);
        if(u1RoundPeriod == 4 && u1LightPeriod == 2)
        {
            u4Val |= (0x1 << 8);
        }
        else
        {
            u4Val &= ~(0x1 << 8);            
        }
    }
    else
    {
        u4Val = u4IO32Read4B(PDWNC_PDMISC) & ~(1U << i4Gpio);
    }
      
    vIO32Write4B(PDWNC_PDMISC, u4Val);
	#endif
	return i4Gpio;
}


