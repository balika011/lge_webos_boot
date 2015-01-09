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

/*****************************************************************************
 *
 * Filename:
 * ---------
 *   $Workfile: drv_rs232.c $ 
 *
 * Project:
 * --------
 *   MT8226
 *
 * Description:
 * ------------
 *   BIM register definition
 *   
 * Author:
 * -------
 *   Daniel Hsiao
 *
 * Last changed:
 * ------------- 
 * $Author: dtvbm11 $ 
 *
 * $Modtime: 04/05/27 10:47p $    
 *
 * $Revision: #1 $
****************************************************************************/
#include "typedef.h"
#include "general.h"
#include "drv_t8032_pwm.h"
#include "Hw_pdwnc.h"




void PDWNC_PwmSetResolution(UINT32 u4Path, UINT32 u4Arg)
{
    vIO32WriteFldAlign(PDWNC_PD_PWM0C0 + u4Path * PWM_ADDR_OFFSET ,u4Arg,FLD_PWM0RSN);
}

void PDWNC_PwmSetHightPeriod(UINT32 u4Path, UINT32 u4Arg)
{
    vIO32WriteFldAlign(PDWNC_PD_PWM0C0 + u4Path * PWM_ADDR_OFFSET ,u4Arg,FLD_PWM0H);
}

void PDWNC_PwmSetRepeatCycle(UINT32 u4Path, UINT32 u4Arg)
{
    vIO32WriteFldAlign(PDWNC_PD_PWM0C0 + u4Path * PWM_ADDR_OFFSET ,u4Arg,FLD_PWM0CYC);
}

void PDWNC_PwmSetAutoEnable(UINT32 u4Path, UINT32 u4Arg)
{
    vIO32WriteFldAlign(PDWNC_PD_PWM0C0 + u4Path * PWM_ADDR_OFFSET ,u4Arg,FLD_PWM0AUTO);
}

void PDWNC_PwmSetPwmEnable(UINT32 u4Path, UINT32 u4Arg)
{
    vIO32WriteFldAlign(PDWNC_PD_PWM0C0 + u4Path * PWM_ADDR_OFFSET ,u4Arg,FLD_PWM0E);
}

void PDWNC_PwmSetPwmPrescaler(UINT32 u4Path, UINT32 u4Arg)
{
    vIO32WriteFldAlign(PDWNC_PD_PWM0C1 + u4Path * PWM_ADDR_OFFSET ,u4Arg,FLD_PWM0P);
}


//-----------------------------------------------------------------------------
/** PDWNC_PwmSetPwm() set pwm agrument
 *  @param u4Path  pwm0 or pwm1
 *  @param u4Enable enable or disable pwm
 *  @param u4Frequence  pwm frequence, 
 *  @param u4DutyCycle  pdwm u4DutyCycle, it will be set as u4DutyCycle% high
 */
//-----------------------------------------------------------------------------
UINT32 PDWNC_PwmSetPwm(UINT32 u4Path, UINT32 u4Enable,UINT32 u4Frequence, UINT32 u4DutyCycle)
{
	UINT32 u4Factor;
    if((PWM_PATH0 != u4Path)&&(PWM_PATH1 != u4Path))
    {
		return FALSE;
	}
	if(FALSE == u4Enable)
	{
		PDWNC_PwmSetPwmEnable(u4Path,FALSE);
		return TRUE;
	}
	PDWNC_PwmSetResolution(u4Path,99); //fix resolution as 99
	PDWNC_PwmSetHightPeriod(u4Path,(u4DutyCycle));
	// system_clock/(1 + pwmrsn)*(1 + pwmp)  = frequence
	if(IO32ReadFldAlign(PDWNC_PDMODE, FLD_DEEP_SLEEP))// 3Mhz
	{
		u4Factor = 3000000/(100 * u4Frequence) - 1;
	}
	else
	{
		u4Factor = (SYS_CLOCK_RATE * 1000000)/(100 * u4Frequence) - 1;
	}
	PDWNC_PwmSetPwmPrescaler(u4Path,u4Factor);
	PDWNC_PwmSetPwmEnable(u4Path,TRUE);
	return TRUE;
}


//-----------------------------------------------------------------------------
/** PDWNC_PwmSetPwm() set pwm agrument
 *  @param u4Path  pwm0 or pwm1 use for breathlight
 *  @param u4Enable enable or disable pwm breathlight
 *  @param u4Cycle  breathlight cycle, set unit as 100 ms
 */
//-----------------------------------------------------------------------------
UINT32 PDWNC_PwmSetBreathLight(UINT32 u4Path, UINT32 u4Enable,UINT32 u4Cycle)
{	
    if((PWM_PATH0 != u4Path)&&(PWM_PATH1 != u4Path))
    {
		return FALSE;
	}
	if(FALSE == u4Enable)
	{
		PDWNC_PwmSetPwmEnable(u4Path,FALSE);
		return TRUE;
	}
	PDWNC_PwmSetRepeatCycle(u4Path,(u4Cycle-1));
	PDWNC_PwmSetPwm(u4Path,u4Enable,1000,50);
	PDWNC_PwmSetAutoEnable(u4Path,TRUE);
	return TRUE;
}


