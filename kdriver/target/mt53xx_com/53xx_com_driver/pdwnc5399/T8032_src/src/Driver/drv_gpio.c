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
#include "general.h"


static INT16 PDWNC_GpioInput(INT16 i4Gpio) reentrant;
static INT16 PDWNC_GpioOutput(INT16 i4Gpio, INT16 *pfgSet) reentrant;
static INT16 PDWNC_GpioEnable(INT16 i4Gpio, INT8 *pfgSet) reentrant;
static INT16 _GPIONumOffset(INT16 i4GpioNum) reentrant;
static INT16 BSP_PinGpioSel(UINT16 i4Gpio) reentrant;	

#if defined(CC_MT5882)
code INT8 _ai1PdwncGpio2Int[TOTAL_OPCTRL_NUM] =
{
	0,  1,  2,  3,  4,  5,  6,  7,  8,  9,  //0~9
	10, 11, 12, -1, 13, -1, 14, -1, -1, -1, //10~19
	-1, -1, -1, -1, -1, -1, -1, -1, 15, 16, //20~29
	17, 18, 19, 20, 21, -1, -1, -1, -1, -1, //30~39
	-1, -1, 		 						//40~41
};
code const UINT32   _aau1OpctrlSelFunc[TOTAL_OPCTRL_NUM][2] =
{
    {PIN_OPCTRL0,	PINMUX_FUNCTION0 },//OPCTRL(200)
    {PIN_OPCTRL1,	PINMUX_FUNCTION0 },//OPCTRL(201)
    {PIN_OPCTRL2,	PINMUX_FUNCTION0 },//OPCTRL(202)
    {PIN_OPCTRL3,	PINMUX_FUNCTION0 },//OPCTRL(203)
    {PIN_OPCTRL4,	PINMUX_FUNCTION0 },//OPCTRL(204)
    {PIN_OPCTRL5,	PINMUX_FUNCTION0 },//OPCTRL(205)
    {PIN_OPCTRL6,	PINMUX_FUNCTION3 },//OPCTRL(206)
    {PIN_OPCTRL7,	PINMUX_FUNCTION3 },//OPCTRL(207)
    {PIN_OPCTRL8,	PINMUX_FUNCTION3 },//OPCTRL(208)
    {PIN_OPCTRL9,	PINMUX_FUNCTION3 },//OPCTRL(209)
    {PIN_OPCTRL10,	PINMUX_FUNCTION0 },//PINMUX_FUNCTION3 - OPCTRL(210)
    {PIN_OPCTRL11,	PINMUX_FUNCTION0 },//OPCTRL(211)
    {PIN_OPCTRL12,	PINMUX_FUNCTION0 },//OPCTRL(212)
    {PIN_OPWRSB,	PINMUX_FUNCTION1 },//OPCTRL(213)
    {PIN_OIRI,	PINMUX_FUNCTION1 },//OPCTRL(214)
    {PIN_LED_PDWM0,	PINMUX_FUNCTION0 },//OPCTRL(215)
    {PIN_LED_PDWM1,	PINMUX_FUNCTION0 },//OPCTRL(216)
    {PIN_HDMI_0_HPD_CBUS,	PINMUX_FUNCTION0 },//OPCTRL(217)
    {PIN_HDMI_CEC,	PINMUX_FUNCTION2 },//OPCTRL(218)
    {PIN_HDMI_0_PWR5V,	PINMUX_FUNCTION0 },//OPCTRL(219)
    {PIN_HDMI_0_SDA,	PINMUX_FUNCTION0 },//OPCTRL(220)
    {PIN_HDMI_0_SCL,	PINMUX_FUNCTION0 },//OPCTRL(221)
    {PIN_HDMI_1_HPD,	PINMUX_FUNCTION1 },//OPCTRL(222)
    {PIN_HDMI_1_SDA,	PINMUX_FUNCTION7 },//OPCTRL(223)
    {PIN_HDMI_1_SCL,	PINMUX_FUNCTION7 },//OPCTRL(224)
    {PIN_HDMI_2_HPD,	PINMUX_FUNCTION0 },//OPCTRL(225)
    {PIN_HDMI_2_SDA,	PINMUX_FUNCTION0 },//OPCTRL(226)
    {PIN_HDMI_2_SCL,	PINMUX_FUNCTION0 },//OPCTRL(227)
    {PIN_ADIN1,	PINMUX_FUNCTION1 },//OPCTRL(228)
    {PIN_ADIN2,	PINMUX_FUNCTION1 },//OPCTRL(229)
    {PIN_ADIN3,	PINMUX_FUNCTION1 },//OPCTRL(230)
    {PIN_ADIN4,	PINMUX_FUNCTION1 },//OPCTRL(231)
    {PIN_ADIN5,	PINMUX_FUNCTION1 },//OPCTRL(232)
    {PIN_ADIN6,	PINMUX_FUNCTION1 },//OPCTRL(233)
    {PIN_ADIN7,	PINMUX_FUNCTION1 },//OPCTRL(234)
    {PIN_VGA_SDA,	PINMUX_FUNCTION0 },//OPCTRL(235)
    {PIN_VGA_SCL,	PINMUX_FUNCTION0 },//OPCTRL(236)
    {PIN_U0TX,	PINMUX_FUNCTION2 },//OPCTRL(237)
    {PIN_U0RX,	PINMUX_FUNCTION2 },//OPCTRL(238)
    {PIN_PHYLED0,	PINMUX_FUNCTION0 },//OPCTRL(239)
    {PIN_PHYLED1,	PINMUX_FUNCTION0 },//OPCTRL(240)
    {PIN_MHL_SENSE,	PINMUX_FUNCTION1 },//OPCTRL(241)
};
code const UINT32 _au4PinmuxReg[TOTAL_PIN_REG+1] = {
    0, // not a pinmux register
	PDWNC_PINMUX0,
	PDWNC_PINMUX1,
	PDWNC_PINMUX2,
};
#endif


#if defined(CC_MT5399)
code INT8 _ai1PdwncGpio2Int[TOTAL_OPCTRL_NUM] =
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
#if 0
code const INT8 _ai1PdwncGpio2Pad[TOTAL_OPCTRL_NUM] =
{//There are some reserved bit field in 28080 of mt536x, so the 'offset of system gpio number' needs to be translated to the correct bit field of 28080
    0, 1, 2, 3, 4, 5, 6, 7,
    8, 9, 10, 11, 12, 13,14, 15, 
    16, 17, 18, 19, 20, 21, 22, 23,
    24, 25, 26, 27, 28, 29, 30, 31,
    32, 33, 34, 35, 36, 37, 38, 39,
    40,41
};
#endif
code const UINT32   _aau1OpctrlSelFunc[TOTAL_OPCTRL_NUM][2] =
{
	{PIN_OPCTRL0,	PINMUX_FUNCTION0 },//OPCTRL(200)
	{PIN_OPCTRL1,	PINMUX_FUNCTION0 },//OPCTRL(201)
	{PIN_OPCTRL2,	PINMUX_FUNCTION0 },//OPCTRL(202)
	{PIN_OPCTRL3,	PINMUX_FUNCTION0 },//OPCTRL(203)
	{PIN_OPCTRL4,	PINMUX_FUNCTION0 },//OPCTRL(204)
	{PIN_OPCTRL5,	PINMUX_FUNCTION0 },//OPCTRL(205)
	{0,0},//{PIN_OPCTRL6,	PINMUX_FUNCTION0 },//OPCTRL(206)
	{0,0},//{PIN_OPCTRL7,	PINMUX_FUNCTION0 },//OPCTRL(207)
	{PIN_OPCTRL8,	PINMUX_FUNCTION0 },//OPCTRL(208)
	{PIN_OPCTRL9,	PINMUX_FUNCTION0 },//OPCTRL(209)
	{PIN_OPCTRL10,	PINMUX_FUNCTION0 },//OPCTRL(210)
	{PIN_OPCTRL11,	PINMUX_FUNCTION0 },//OPCTRL(211)
	{PIN_OPWRSB,	PINMUX_FUNCTION1},//OPCTRL(212)
	{PIN_OIRI,	PINMUX_FUNCTION1 },//OPCTRL(213)
	
	{PIN_LED_PDWN0,	PINMUX_FUNCTION0 },//OPCTRL(214)
	{PIN_LED_PDWN1,	PINMUX_FUNCTION0 },//OPCTRL(215)
	{PIN_HDMI_CEC,	PINMUX_FUNCTION0 },//OPCTRL(216)
	{PIN_HDMI_0_HPD_CBUS,	PINMUX_FUNCTION0 },//OPCTRL(217)
	{0,0},//{PIN_AOLRCK,	PINMUX_FUNCTION0 },//OPCTRL(218)
	{PIN_HDMI_0_SDA,	PINMUX_FUNCTION0 },//OPCTRL(219)
	{PIN_HDMI_0_SCL,	PINMUX_FUNCTION0 },//OPCTRL(220)
	{PIN_HDMI_1_HPD,	PINMUX_FUNCTION0 },//OPCTRL(221)
	{0,0},//{PIN_ADIN0,	PINMUX_FUNCTION1 },//OPCTRL(222)
	{PIN_HDMI_1_SDA,	PINMUX_FUNCTION0 },//OPCTRL(223)
	{PIN_HDMI_1_SCL,	PINMUX_FUNCTION0 },//OPCTRL(224)
	{PIN_HDMI_2_HPD,	PINMUX_FUNCTION0 },//OPCTRL(225)
	{0,0},//{PIN_ADIN4,	PINMUX_FUNCTION1 },//OPCTRL(226)
	{PIN_HDMI_2_SDA,	PINMUX_FUNCTION0 },//OPCTRL(227)
	{PIN_HDMI_2_SCL,	PINMUX_FUNCTION0 },//OPCTRL(228)
	{0,0},//{PIN_ADIN7,	PINMUX_FUNCTION1 },//OPCTRL(229)
	{0,0},//{PIN_IF_AGC,	PINMUX_FUNCTION0 },//OPCTRL(230)
	{PIN_HDMI_3_SDA,	PINMUX_FUNCTION0 },//OPCTRL(231)
	{PIN_HDMI_3_SCL,	PINMUX_FUNCTION0 },//OPCTRL(232)
	{PIN_ADIN4,	PINMUX_FUNCTION1 },//OPCTRL(233)
	{PIN_ADIN5,	PINMUX_FUNCTION1 },//OPCTRL(234)
	{PIN_ADIN6,	PINMUX_FUNCTION1 },//OPCTRL(235)
	{PIN_ADIN7,	PINMUX_FUNCTION1 },//OPCTRL(236)
	{PIN_VGA_SDA,	PINMUX_FUNCTION0 },//OPCTRL(237)
	{PIN_VGA_SCL,	PINMUX_FUNCTION0 },//OPCTRL(238)
	{PIN_STB_SDA,	PINMUX_FUNCTION0 },//OPCTRL(239) 
	{PIN_STB_SCL,	PINMUX_FUNCTION0 },//OPCTRL(240) 
	{PIN_U0TX,	PINMUX_FUNCTION2 },//OPCTRL(241) 
	{PIN_U0RX,	PINMUX_FUNCTION2 },//OPCTRL(242) 
	{PIN_PHYLED0,	PINMUX_FUNCTION0 },//OPCTRL(243) 
	{PIN_PHYLED1,	PINMUX_FUNCTION0 },//OPCTRL(244) 
	{PIN_ADIN2,	PINMUX_FUNCTION1 },//OPCTRL(245) 
	{PIN_ADIN3,	PINMUX_FUNCTION1 },//OPCTRL(246) 
	{PIN_GPIO56,	PINMUX_FUNCTION1 },//OPCTRL(247) 
	{PIN_GPIO59,	PINMUX_FUNCTION1 },//OPCTRL(248) 
	{PIN_GPIO57,	PINMUX_FUNCTION1 },//OPCTRL(249) 
	{PIN_GPIO58,	PINMUX_FUNCTION1 },//OPCTRL(250) 
	{PIN_GPIO60,	PINMUX_FUNCTION1 },//OPCTRL(251) 
	{PIN_GPIO50,	PINMUX_FUNCTION1 },//OPCTRL(252) 
	{PIN_GPIO51,	PINMUX_FUNCTION1 },//OPCTRL(253) 
	{PIN_GPIO52,	PINMUX_FUNCTION1 },//OPCTRL(254) 
	{PIN_GPIO53,	PINMUX_FUNCTION1 },//OPCTRL(255) 
	{PIN_GPIO54,	PINMUX_FUNCTION1 },//OPCTRL(256) 
	{PIN_GPIO55,	PINMUX_FUNCTION1 },//OPCTRL(257) 
	{PIN_MHL_SENSE,	PINMUX_FUNCTION1 },//OPCTRL(258) 
	{PIN_ADIN1,	    PINMUX_FUNCTION1 },//OPCTRL(259) 
};
code const UINT32 _au4PinmuxReg[TOTAL_PIN_REG+1] = {
    0, // not a pinmux register
	PDWNC_PINMUX0,
	PDWNC_PINMUX1,
	PDWNC_PINMUX2,
	//PDWNC_PINMUX3,
};
#endif
static INT16 _GPIONumOffset(INT16 i4GpioNum) reentrant
{
	UINT8 u4Idx;
	
	if ((i4GpioNum >= ADIN0) && (i4GpioNum <= ADIN7))
	{
		u4Idx = GPIO_HANDLER_PDWNC_SRVAD;
	}
	if ((i4GpioNum >= OPCTRL0) && (i4GpioNum < (OPCTRL0+TOTAL_OPCTRL_NUM)))
	{
		u4Idx = GPIO_HANDLER_PDWNC_GPIO;
	}	
	
	switch(u4Idx)
	{
	case GPIO_HANDLER_PDWNC_GPIO:
		return i4GpioNum - OPCTRL(0);
	case GPIO_HANDLER_PDWNC_SRVAD:
		#if defined(CC_MT5881)
		return (i4GpioNum - SERVO_0_ALIAS) + (SERVO_GPIO0 - OPCTRL(0));
		#elif defined(CC_MT5399) || defined(CC_MT5882)
		//for mustang servo AD 
		switch(i4GpioNum)
		{
			case ADIN0:
				#if UART_DBG
				LogS("mustang servoAD 0 have no gpio function");
				#endif
				return -1;
			case ADIN1:
				return SERVO_GPIO1;
			case ADIN2:
				return SERVO_GPIO2;
			case ADIN3:
				return SERVO_GPIO3;
			case ADIN4:
				return SERVO_GPIO4;
			case ADIN5:
				return SERVO_GPIO5;
			case ADIN6:
				return SERVO_GPIO6;
			case ADIN7:
				return SERVO_GPIO7;
			default:
				#if UART_DBG
				LogS("_GPIONumOffset( ) argument error ");
				#endif
				return -1;
		}
		#endif
	}
	return i4GpioNum;
}


//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------
static INT16 PDWNC_GpioEnable(INT16 i4Gpio, INT8 *pfgSet) reentrant
{
    UINT32 u4Val;
    INT32 i4Idx;
#ifdef T8032_GPIO_DEBUG
	if(MGET_ARM_POWER_STATE() != ARM_POWER_OFF)
		return -1;
#endif
    if ((i4Gpio < 0) || (i4Gpio >= TOTAL_OPCTRL_NUM))
    {
        return -1;
    }
	
    // Translate GPIO number to pad setting register offset
    //i4Idx = (INT32)_ai1PdwncGpio2Pad[i4Gpio];
	i4Idx = i4Gpio;
	
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
		return ((u4Val & (1L << i4Idx)) ? 1 : 0);
    }
	

    u4Val = (*pfgSet) ?
		(u4Val | (1L << i4Idx)) :
	(u4Val & ~(1L << i4Idx));
	
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
	
	
    return (*pfgSet);
}



//-----------------------------------------------------------------------------                              
/** BSP_PinSet() to set pinmux data                                                                          
*  @param u4PinSel is the bit group of pinmuxers                                                            
*  @param i4Func is the function we want to select                                                          
*  @retval 0 is successful, otherwise failed.                                                               
*/                                                                                                          
//-----------------------------------------------------------------------------                              
INT16 BSP_PinSet(UINT32 u4PinSel, UINT16 i4Func) reentrant                                                             
{                                                                                                            
	
	UINT32 u4HiReg;
	UINT32 u4LoReg, u4LoMask, u4LoBits;
    UINT32 u4HiMask = 0xffffffff;
    UINT32 u4HiBits = 0; 
#ifdef T8032_GPIO_DEBUG
	if(MGET_ARM_POWER_STATE() != ARM_POWER_OFF)
		return -1;
#endif
	
    if ((!IS_VALID_MUX_REG(u4PinSel)) ||(i4Func < 0) || (i4Func > MAX_FUNC(u4PinSel)))
    {                                                                                                        
        return -1;                                                                                           
    }
	 u4HiReg = (HI_MUX_REG(u4PinSel));
	if (u4HiReg)
		{
 			u4HiReg = HI_MUX_REG(u4PinSel);
		}
    u4LoReg = LO_MUX_REG(u4PinSel);
	if ((u4HiReg > TOTAL_PIN_REG)||  (u4LoReg > TOTAL_PIN_REG))
    {                                                                                                        
		return -1;                                                                                           
	}                                                                                               
	
	if (u4HiReg!=0)    // with high pinmux bits
    {
		u4HiReg = _au4PinmuxReg[u4HiReg];
        u4HiMask = HI_MASK(u4PinSel);
        u4HiBits = HI_BIT_OFFSET(u4PinSel);
    }
	
    u4LoReg = _au4PinmuxReg[u4LoReg];
    u4LoMask = LO_MASK(u4PinSel);
    u4LoBits = LO_BIT_OFFSET(u4PinSel);
	if (u4HiReg != 0)
    {
		vIO32Write4B(u4HiReg, (u4IO32Read4B(u4HiReg) & (~(u4HiMask << u4HiBits))) |
			((UINT32)((i4Func >> (MASK_TO_BITNUM(u4LoMask)+1)) & u4HiMask) << u4HiBits)); 
		
    }
	
    vIO32Write4B(u4LoReg, (u4IO32Read4B(u4LoReg) & (~(u4LoMask << u4LoBits))) |
		((UINT32)(i4Func & u4LoMask) << u4LoBits));    
    return 0;    
    
}                                                                                                            


static INT16 BSP_PinGpioSel(UINT16 i4Gpio) reentrant																			 
{
#ifdef T8032_GPIO_DEBUG
	if(MGET_ARM_POWER_STATE() != ARM_POWER_OFF)
		return -1;
#endif
	
	if ((i4Gpio >= OPCTRL0) && (i4Gpio < (OPCTRL0+TOTAL_OPCTRL_NUM)))
	{
		
		return BSP_PinSet(_aau1OpctrlSelFunc[i4Gpio - OPCTRL0][0], _aau1OpctrlSelFunc[i4Gpio - OPCTRL0][1]); 
	}																										 
	return -1;
}											  



//-----------------------------------------------------------------------------                              
/** BSP_PinGet() to get pinmux data                                                                          
*  @param u4PinSel is the bit group of pinmuxers                                                            
*  @retval the function we select now.                                                                      
*/                                                                                                          
//-----------------------------------------------------------------------------                              
INT16 BSP_PinGet(UINT32 u4PinSel)                                                                             
{                                                                                                            
    UINT32 u4HiReg, u4HiVal;
    UINT32 u4HiMask = 0xffffffff;
	UINT32 u4HiBits = 0;
	UINT32 u4LoReg, u4LoMask, u4LoBits, u4LoVal, u4Func;
	
    if (!IS_VALID_MUX_REG(u4PinSel))
    {                                                                                                        
        return -1;                                                                                           
    }                                                                                                        
	
	 u4HiReg = (HI_MUX_REG(u4PinSel));
	if (u4HiReg)
		{
 			u4HiReg = HI_MUX_REG(u4PinSel);
		}
    u4LoReg = LO_MUX_REG(u4PinSel);

    if ((u4HiReg > TOTAL_PIN_REG) || (u4LoReg > TOTAL_PIN_REG))
    {                                                                                                        
        return -1;                                                                                           
    }                                                                                                        
	
    if (u4HiReg!=0)    // with high pinmux bits
    {
        u4HiReg = _au4PinmuxReg[u4HiReg];
        u4HiMask = HI_MASK(u4PinSel);
        u4HiBits = HI_BIT_OFFSET(u4PinSel);
    }
    u4LoReg = _au4PinmuxReg[u4LoReg];
    u4LoMask = LO_MASK(u4PinSel);
    u4LoBits = LO_BIT_OFFSET(u4PinSel);
	
    u4LoVal = u4IO32Read4B(u4LoReg);
    u4Func = (u4LoVal >> u4LoBits) & u4LoMask;
    if (u4HiReg != 0)
    {
        u4HiVal = u4IO32Read4B(u4HiReg);
        u4Func += ((u4HiVal >> u4HiBits) & u4HiMask) << (MASK_TO_BITNUM(u4LoMask)+1);
    }
    return (INT16)u4Func;
}                                                                                                            
static INT16 PDWNC_GpioOutput(INT16 i4Gpio, INT16 *pfgSet) reentrant
{
	UINT32 u4Val;
	INT8 i4Idx;
	
#ifdef T8032_GPIO_DEBUG
	if(MGET_ARM_POWER_STATE() != ARM_POWER_OFF)
		return -1;
#endif
    if ((i4Gpio < 0) || (i4Gpio >= TOTAL_OPCTRL_NUM))
    {
        return -1;
    }

	// Translate GPIO number to pad setting register offset
	//i4Idx = (INT8)_ai1PdwncGpio2Pad[i4Gpio];
	i4Idx = i4Gpio;
	
	if (i4Gpio <= 31)
    {
        u4Val = u4IO32Read4B(PDWNC_GPIOOUT0);
    }
    else
    {
        u4Val = u4IO32Read4B(PDWNC_GPIOOUT1);
    }
	
	i4Idx &= 0x1f;
	
	if (pfgSet == NULL)
	{		
		return ((u4Val & (1L << i4Idx)) ? 1 : 0);
	}
	
	
	u4Val = (*pfgSet) ?
		(u4Val | (1L << i4Idx)) :
	(u4Val & ~(1L << i4Idx));
	
	if (i4Gpio <= 31)
    {
        vIO32Write4B(PDWNC_GPIOOUT0, u4Val);
    }
    else
    {
        vIO32Write4B(PDWNC_GPIOOUT1, u4Val);
    }
	// Set the output mode.
	i4Idx = 1;
	//VERIFY(1==PDWNC_GpioEnable(i4Gpio, &i4Idx));
	
	UNUSED(PDWNC_GpioEnable(i4Gpio, &i4Idx));
	BSP_PinGpioSel(i4Gpio +  OPCTRL(0));
	return (*pfgSet);
}

static INT16 PDWNC_GpioInput(INT16 i4Gpio) reentrant
{
    UINT32 u4Val;
    INT16 i4Idx = 0;
#ifdef T8032_GPIO_DEBUG
	if(MGET_ARM_POWER_STATE() != ARM_POWER_OFF)
		return -1;
#endif
    if ((i4Gpio < 0) || (i4Gpio >= TOTAL_OPCTRL_NUM))
    {
        return -1;
    }

    // Translate GPIO number to pad setting register offset
    //i4Idx = (INT16)_ai1PdwncGpio2Pad[i4Gpio];
    i4Idx = i4Gpio;
    
    BSP_PinGpioSel(i4Gpio +  OPCTRL(0));
	
	if (i4Gpio <= 31)
	{
		u4Val = IO32ReadFldAlign(PDWNC_GPIOIN0, FLD_PD_GPIO_IN0); 	  
		return ((u4Val & (1L << i4Idx)) ? 1L : 0);  
	}
	else
	{
		u4Val = IO32ReadFldAlign(PDWNC_GPIOIN1, FLD_PD_GPIO_IN1);
		i4Idx &= 0x1f;
		return ((u4Val & (1L << i4Idx)) ? 1L : 0);
	}   
	
}

//-----------------------------------------------------------------------------
/** GPIO_SetOut() to set gpio output value.
*  @param i4GpioNum
*  @param i4Val 0 or 1.
*  @retval to return current gpio out register setting.
*/
//-----------------------------------------------------------------------------
INT16 GPIO_SetOut(INT16 i4GpioNum, INT16 i4Val) reentrant
{
    INT16 fgSet, i4Ret;
	UINT8 stEA;
#ifdef T8032_GPIO_DEBUG
	if(MGET_ARM_POWER_STATE() != ARM_POWER_OFF)
		return -1;
#endif
	stEA = EA;
	EA = 0;//disable all interrupt
    fgSet = i4Val;
	
	i4Ret = PDWNC_GpioOutput(_GPIONumOffset(i4GpioNum), &fgSet);
	if (i4Ret != i4Val)
	{
		return i4Ret;
	}
	i4Ret = PDWNC_GpioOutput(_GPIONumOffset(i4GpioNum), NULL);
	EA = stEA; //restore EA bit
	return i4Ret;
}
//-----------------------------------------------------------------------------
/** GPIO_Input()  The GPIO input reading functions. It will check the 
 *  i4GpioNum and read related register bit to return.
 *  @param i4GpioNum the gpio number to read.
 *  @retval 0 or 1.  (GPIO input value.)
 */
//-----------------------------------------------------------------------------

INT16 GPIO_GetIn(INT16 i4GpioNum) reentrant
{
    INT8 i4Mode;
	INT16 i4Ret;
	UINT8 stEA;
#ifdef T8032_GPIO_DEBUG
	if(MGET_ARM_POWER_STATE() != ARM_POWER_OFF)
		return -1;
#endif
	stEA = EA;
	EA = 0;//disable all interrupt
	
    i4Mode = 0;
	UNUSED(PDWNC_GpioEnable(_GPIONumOffset(i4GpioNum), &i4Mode));
	i4Ret = PDWNC_GpioInput(_GPIONumOffset(i4GpioNum));

	EA = stEA; //restore EA bit
	return i4Ret;
	
}
#if GPIO_INTERRUPT_ENABLE


INT16 PDWNC_GpioIntrq(INT16 i4Gpio, INT32 *pfgSet)
{
    UINT32 u4Val;
    INT32 i4Int;
#ifdef T8032_GPIO_DEBUG
	if(MGET_ARM_POWER_STATE() != ARM_POWER_OFF)
		return -1;
#endif
    //UNUSED(_ai1PdwncGpio2Int);
    if ((i4Gpio < 0) || (i4Gpio >= TOTAL_OPCTRL_NUM))
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
	#endif//CC_MT5882
    if (pfgSet == NULL)
    {
        return ((u4Val & (1L << i4Int)) ? 1 : 0);
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
	#endif
	
    return (*pfgSet);
}
INT16 PDWNC_GpioReg(UINT16 i4Gpio, GPIO_TYPE eType)
{

    UINT32 u4Val;
    //UINT32 u4RegIntCfg;
    INT32 i4Int;
//	UINT32 u4Gpio;
    //INT32 i4Offset = 0;

	if(i4Gpio >= OPCTRL0)
		i4Gpio -= OPCTRL0;
#ifdef T8032_GPIO_DEBUG
	if(MGET_ARM_POWER_STATE() != ARM_POWER_OFF)
		return -1;
#endif
    if ((i4Gpio < 0) || (i4Gpio >= TOTAL_OPCTRL_NUM))
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
	
    if((eType == GPIO_TYPE_INTR_RISE) || (eType == GPIO_TYPE_INTR_FALL) || (eType == GPIO_TYPE_INTR_BOTH) || (eType == GPIO_TYPE_INTR_LEVEL_HIGH) || (eType == GPIO_TYPE_INTR_LEVEL_LOW))
    {    
		BSP_PinGpioSel(i4Gpio + OPCTRL(0));//pinmux 
		
		if(_ai1PdwncGpio2Int[i4Gpio] >= MAX_PDWNC_INT_ID)
		{
			u4Val = u4IO32Read4B(PDWNC_T8032_INTEN_2);//PDWNC_T8032_INTEN
			u4Val |= (1U << (_ai1PdwncGpio2Int[i4Gpio] - MAX_PDWNC_INT_ID));	   // Enable interrupt.
			vIO32Write4B(PDWNC_T8032_INTEN_2, u4Val);
		}
		else
		{	
			u4Val = u4IO32Read4B(PDWNC_T8032_INTEN);//PDWNC_T8032_INTEN
			u4Val |= (1U << i4Int); 	   // Enable interrupt.
			vIO32Write4B(PDWNC_T8032_INTEN, u4Val);
		}
    }
    else
    {
		if(_ai1PdwncGpio2Int[i4Gpio] >= MAX_PDWNC_INT_ID)
		{
			u4Val = u4IO32Read4B(PDWNC_T8032_INTEN_2);//PDWNC_T8032_INTEN
			u4Val &= ~(1U << (_ai1PdwncGpio2Int[i4Gpio] - MAX_PDWNC_INT_ID));	   //disable interrupt.
			vIO32Write4B(PDWNC_T8032_INTEN_2, u4Val);
		}
		else
		{	
			u4Val = u4IO32Read4B(PDWNC_T8032_INTEN);//PDWNC_T8032_INTEN
			u4Val &= ~(1L << i4Int);       // Disable interrupt.
			vIO32Write4B(PDWNC_T8032_INTEN, u4Val);
		}
        return 0;
    }
	#endif
     /* Set the register and callback function. */
//    //i4Int = (i4Int>=24)? (i4Int-16):(i4Int);
//    u4Gpio = _ai1PdwncInt2Gpio[i4Int];
    switch(eType)
    {
        case GPIO_TYPE_INTR_FALL:
            u4Val = u4IO32Read4B(PDWNC_EXINT2ED);
            u4Val &= ~(1L << i4Int); // Disable double edge trigger.
            vIO32Write4B(PDWNC_EXINT2ED, u4Val);
            u4Val = u4IO32Read4B(PDWNC_EXINTLEV);
            u4Val &= ~(1L << i4Int);  // Set Edge trigger.
            vIO32Write4B(PDWNC_EXINTLEV, u4Val);
            u4Val = u4IO32Read4B(PDWNC_EXINTPOL);
            u4Val &= ~(1L << i4Int);       // Set Falling Edge interrupt.
            vIO32Write4B(PDWNC_EXINTPOL, u4Val);
            break;
            
        case GPIO_TYPE_INTR_RISE:            
            u4Val = u4IO32Read4B(PDWNC_EXINT2ED);
            u4Val &= ~(1L << i4Int); // Disable double edge trigger.
            vIO32Write4B(PDWNC_EXINT2ED, u4Val);
            u4Val = u4IO32Read4B(PDWNC_EXINTLEV);
            u4Val &= ~(1L << i4Int);  // Set Edge trigger.
            vIO32Write4B(PDWNC_EXINTLEV, u4Val);
            u4Val = u4IO32Read4B(PDWNC_EXINTPOL);
            u4Val |= (1L << i4Int);       // Set rasing Edge interrupt.
            vIO32Write4B(PDWNC_EXINTPOL, u4Val);
            break;               
            
        case GPIO_TYPE_INTR_BOTH:            
            u4Val = u4IO32Read4B(PDWNC_EXINT2ED);
            u4Val |= (1L << i4Int); // enable double edge trigger.
            vIO32Write4B(PDWNC_EXINT2ED, u4Val);
            u4Val = u4IO32Read4B(PDWNC_EXINTLEV);
            u4Val &= ~(1L << i4Int);  // Set Edge trigger.
            vIO32Write4B(PDWNC_EXINTLEV, u4Val);
            break;
            
        case GPIO_TYPE_INTR_LEVEL_LOW:            
            u4Val = u4IO32Read4B(PDWNC_EXINT2ED);
            u4Val &= ~(1L << i4Int); // Disable double edge trigger.
            vIO32Write4B(PDWNC_EXINT2ED, u4Val);
            u4Val = u4IO32Read4B(PDWNC_EXINTLEV);
            u4Val |= (1L << i4Int);  // Set Level trigger.
            vIO32Write4B(PDWNC_EXINTLEV, u4Val);
            u4Val = u4IO32Read4B(PDWNC_EXINTPOL);
            u4Val &= ~(1L << i4Int);       // Set level low interrupt.
            vIO32Write4B(PDWNC_EXINTPOL, u4Val);
            break;
            
        case GPIO_TYPE_INTR_LEVEL_HIGH:            
            u4Val = u4IO32Read4B(PDWNC_EXINT2ED);
            u4Val &= ~(1L << i4Int); // Disable double edge trigger.
            vIO32Write4B(PDWNC_EXINT2ED, u4Val);
            u4Val = u4IO32Read4B(PDWNC_EXINTLEV);
            u4Val |= (1L << i4Int);  // Set level trigger.
            vIO32Write4B(PDWNC_EXINTLEV, u4Val);
            u4Val = u4IO32Read4B(PDWNC_EXINTPOL);
            u4Val |= (1L << i4Int);       // Set level high interrupt.
            vIO32Write4B(PDWNC_EXINTPOL, u4Val);
            break;
            
        default:
			#if defined(CC_MT5882)
			u4Val = 0;
			PDWNC_GpioIntrq(i4Int,(INT32 *)&u4Val);// Disable interrupt.
			#endif
            return -1;
    }
	#if defined(CC_MT5882)
	u4Val = 1;
	PDWNC_GpioIntrq(i4Gpio,(INT32 *)&u4Val);// Enable interrupt.
	#endif
    return 0;
}
#endif

