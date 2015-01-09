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
 *-----------------------------------------------------------------------------
 * $Author: dtvbm11 $
 * $Date: 2015/01/09 $
 * $RCSfile: nptv_hvpos_cmd.c,v $
 * $Revision: #1 $
 *
 * Description:
 *         This file contains CLI implementation of NPTV Video.
 *---------------------------------------------------------------------------*/

#include "x_os.h"
#include "x_stl_lib.h"
#include "video_def.h"

#ifdef __MW_CLI_DEF__
#include "x_mid_cli.h"
#else
#include "x_drv_cli.h"
#endif

#include "x_assert.h"
#include "x_pinmux.h"
#include "x_debug.h"
#include "drv_pwm.h"
#include "x_ckgen.h"

#ifndef CC_CLI
#define CC_CLI
#endif

#ifdef CC_CLI
#define PWM_DUTY_ALL 255
#define PWM_DUTY_LESS 254

/* Functions for H/V position */

static INT32 _CLIGetPWMValue(INT32 i4Argc, const CHAR **szArgv);
static INT32 _CLIPwmSetCmd(INT32 i4Argc, const CHAR **szArgv);
static INT32 _CLIPwmSetDimmingCmd(INT32 i4Argc, const CHAR **szArgv);

#endif /*#ifdef CC_CLI*/
/******************************************************************************
* CLI Command Table
******************************************************************************/

CLI_EXEC_T arPWMCmdTbl[] = {
  {"GetPWMValue", "gpv", _CLIGetPWMValue, NULL, "get pwm value", CLI_GUEST},
  {"PWMSetRegster", "srg", _CLIPwmSetCmd, NULL, "PWM set register", CLI_GUEST},
  {"PWMSetDimming", "sdm", _CLIPwmSetDimmingCmd, NULL, "PWM set Dim", CLI_GUEST},
  {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};

#ifdef CC_CLI
/******************************************************************************
* Local Function
******************************************************************************/

static INT32 _CLIGetPWMValue(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4Src,u4PwmP,u4PwmH,u4PwmRSN,u4Frequency,u4Dimming;
   
    if(i4Argc < 1)
    {
		Printf("Arg: u4Src \n");
		return 0;
    }

	u4Src = StrToInt(szArgv[0]);
	if(0 == u4Src)
	{
		u4PwmRSN = (UINT32)(IO_READ32(HPDS_BASE,REG_RW_PWM0) & 0xfff00000) >> 20;
		u4PwmH = (UINT32)(IO_READ32(HPDS_BASE,REG_RW_PWM0) & 0xfff0) >> 4;
		u4PwmP = (UINT32)(IO_READ32(HPDS_BASE,REG_RW_PWM0VSYNC) & 0xfffff);
	    Printf("u4Src = 0x%x, u4PwmP = 0x%x, u4PwmH = 0x%x, u4PwmRSN = 0x%x.\n",
				   u4Src,
	               u4PwmP,
	               u4PwmH,
	               u4PwmRSN);
		u4Dimming = u4PwmH;
		if(u4Dimming >= 255)
		{
			u4Dimming = 255;
		}
		if(u4Dimming == 255)
		{
			u4Frequency = (BSP_GetDomainClock(SRC_BUS_CLK)/0xff)/255;

		}
		else
		{
			u4Frequency = (BSP_GetDomainClock(SRC_BUS_CLK)/PWM_DUTY_ALL)/(u4Dimming + 1);	
		} 
	    Printf("u4Frequency = 0x%x, u4Dimming = 0x%x.\n",u4Frequency,u4Dimming);		
	}
	else if(1 == u4Src)
	{
		u4PwmRSN = (UINT32)(IO_READ32(HPDS_BASE,REG_RW_PWM0) & 0xfff00000) >> 20;
		u4PwmH = (UINT32)(IO_READ32(HPDS_BASE,REG_RW_PWM0) & 0xfff0) >> 4;
		u4PwmP = (UINT32)(IO_READ32(HPDS_BASE,REG_RW_PWM0VSYNC) & 0xfffff);
	    Printf("u4Src = 0x%x, u4PwmP = 0x%x, u4PwmH = 0x%x, u4PwmRSN = 0x%x.\n",
				   u4Src,
	               u4PwmP,
	               u4PwmH,
	               u4PwmRSN);
		u4Dimming = u4PwmH;
		if(u4Dimming >= 255)
		{
			u4Dimming = 255;
		}
		if(u4Dimming == 255)
		{
			u4Frequency = (BSP_GetDomainClock(SRC_BUS_CLK)/0xff)/255;

		}
		else
		{
			u4Frequency = (BSP_GetDomainClock(SRC_BUS_CLK)/PWM_DUTY_ALL)/(u4Dimming + 1);	
		} 
	    Printf("u4Frequency = 0x%x, u4Dimming = 0x%x.\n",u4Frequency,u4Dimming);	
	}
	else if(2 == u4Src)
	{
		u4PwmRSN = (UINT32)(IO_READ32(HPDS_BASE,REG_RW_PWM0) & 0xfff00000) >> 20;
		u4PwmH = (UINT32)(IO_READ32(HPDS_BASE,REG_RW_PWM0) & 0xfff0) >> 4;
		u4PwmP = (UINT32)(IO_READ32(HPDS_BASE,REG_RW_PWM0VSYNC) & 0xfffff);
	    Printf("u4Src = 0x%x, u4PwmP = 0x%x, u4PwmH = 0x%x, u4PwmRSN = 0x%x.\n",
				   u4Src,
	               u4PwmP,
	               u4PwmH,
	               u4PwmRSN);
		u4Dimming = u4PwmH;
		if(u4Dimming >= 255)
		{
			u4Dimming = 255;
		}
		if(u4Dimming == 255)
		{
			u4Frequency = (BSP_GetDomainClock(SRC_BUS_CLK)/0xff)/255;

		}
		else
		{
			u4Frequency = (BSP_GetDomainClock(SRC_BUS_CLK)/PWM_DUTY_ALL)/(u4Dimming + 1);	
		} 
	    Printf("u4Frequency = 0x%x, u4Dimming = 0x%x.\n",u4Frequency,u4Dimming);		
	}
	else 
	{
		Printf("src can not > 2\n");
		return 0;
	}
    return 0;
}
static INT32 _CLIPwmSetCmd(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4Src,u4PwmP,u4PwmH,u4PwmRSN;
   
    if(i4Argc < 4)
    {
		Printf("Arg: u4Src, u4PwmP, u4PwmH, u4PwmRSN\n");
		return 0;
    }

	u4Src = StrToInt(szArgv[0]);
    u4PwmP = StrToInt(szArgv[1]);  
    u4PwmH = StrToInt(szArgv[2]);
    u4PwmRSN = StrToInt(szArgv[3]);

    Printf("u4Src = 0x%x, u4PwmP = 0x%x, u4PwmH = 0x%x, u4PwmRSN = 0x%x.\n",
			   u4Src,
               u4PwmP,
               u4PwmH,
               u4PwmRSN);

	vDrvSetPWM(u4Src,u4PwmP,u4PwmH,u4PwmRSN);  
    return 0;

}


static INT32 _CLIPwmSetDimmingCmd(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4Src, u4Frequency,u4Dimming;
   
    if(i4Argc < 3)
    {
		Printf("Arg: u4Pin, u4Frequency, u4Dimming\n");
		return 0;
    }
	u4Src = StrToInt(szArgv[0]);
	u4Frequency = StrToInt(szArgv[1]);
    u4Dimming = StrToInt(szArgv[2]);  


    Printf("u4Src = 0x%x, u4Frequency = 0x%x, u4Dimming = 0x%x.\n",
			   u4Src,
			   u4Frequency,
               u4Dimming);

     if(u4Frequency == 0)
    {
		Printf("u4Frequency can not be zero.\n");
		return 0;
    }

    if(u4Dimming >= 256)
    {
        u4Dimming = 256;
    }

    if((u4Dimming == 256)||(u4Dimming == 255))
    {
        vDrvSetPWM(u4Src,
            ((BSP_GetDomainClock(SRC_BUS_CLK)) / (PWM_DUTY_LESS+1)) / u4Frequency,
            0xFF,
            PWM_DUTY_LESS);
    }
    else
    {
        vDrvSetPWM(u4Src,
                   (BSP_GetDomainClock(SRC_BUS_CLK) >> 8) / u4Frequency, u4Dimming, PWM_DUTY_ALL);
    }   
    return 0;

}

#endif //#ifdef CC_CLI

