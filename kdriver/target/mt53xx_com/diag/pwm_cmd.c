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
 * $RCSfile: pwm_cmd.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file pwm_cmd.c
 *  PWM module CLI test function.
 */

//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------
#ifdef __MW_CLI_DEF__
#include "x_mid_cli.h"
#else
#include "x_drv_cli.h"
#endif

#include "drvcust_if.h"
#define DEFINE_IS_LOG      PWM_IsLog
#include "x_debug.h"
#include "x_util.h"
#include "x_hal_5381.h"

#include "x_os.h"
#include "x_printf.h"
#include "x_stl_lib.h"
#include "x_pinmux.h"
#include "x_assert.h"
#include "drv_pwm.h"
#include "x_bim.h"
//---------------------------------------------------------------------------
// Configurations
//---------------------------------------------------------------------------
/* Declare the debug on/off/level and RegTest functions */
CLIMOD_DEBUG_FUNCTIONS(PWM);
/* End of Declare */
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
#define PWM_I_DUTY_C 0
#define PWM_I_PEAK_C 1
#define PWM_RETURN_ERROR_C 0xFFFF


typedef enum
{
    eNormal          = 0,
    eHigh_Fix        = 1,
    eLow_Fix         = 2
} Out_Mode_C;

typedef struct
{
    Out_Mode_C Mode;
    UINT16 u2duty;
} PWM_Status_C;

PWM_Status_C arPWM_Status_C[2] = {{eNormal, 1024}, {eNormal, 1024}};   //PWM Status

/**************************************************************************/
// Private function : To get PWM Addr.
static UINT32 Backlight_PwmGetAddr_C(UINT8 u1Channel)
{
    UINT32 u4Reg = 0;
    switch (u1Channel)
    {
        case PWM_I_DUTY_C:
            u4Reg = REG_RW_PWM0;
            break;

        case PWM_I_PEAK_C:
            u4Reg = REG_RW_PWM1;
            break;

        default:
            LOG(0, "PWM NOT support (%d) Channel", u1Channel);
    }

    return u4Reg;
}

// Frequency = 216MHz / (TimeBase + 1) / (1024 + 1) // DutyAll is fix to 1024.
// timeBase is (4 ~ 4400)
// Func: set TimeBase, it is another way of saying PreScaler, it is PWM P
void DRVCUST_SetBLPwmFrequency_C(UINT8 u1Channel, UINT32 u4TimeBase)
{
    UINT32 u4Reg;
    UINT32 u4Frequency;
    UINT32 u4BusClock = 192000000;

    //Check timeBase
    if (u4TimeBase >= 4 && u4TimeBase <= 4400)
    {
        u4TimeBase *= 192;
        u4TimeBase /= 216;

        u4Frequency = u4BusClock / ((u4TimeBase + 1) * (1024 + 1));

        Printf("SONY_PWM SetFrequency_C(%d): PWMP [0x%lx], Frequency[%luHz]\n", u1Channel, u4TimeBase, u4Frequency);
        u4Reg = Backlight_PwmGetAddr_C(u1Channel);
        if (u4Reg != 0)
        {
            IO_WRITE32MSK(HPDS_BASE, u4Reg, 0x40000000, 0xFFF00000);// PWMRSN fix to 1024
            IO_WRITE32MSK(HPDS_BASE, u4Reg + 4, u4TimeBase, 0xFFFFF);// Set TimeBase
            IO_WRITE32_SET(HPDS_BASE, u4Reg, 0);// Enable PWM
        }
    }
    else
    {
        LOG(0, "PWM timeBase(%d) is too large", u4TimeBase);
    }
}

// Return : timeBase [4, 4400]
// Return OK:[4, 4400] ; Fail : 0xFFFF
UINT32 DRVCUST_GetBLPwmFrequency_C(UINT8 u1Channel)
{
    UINT32 u4Reg;
    UINT32 u4RVal = 0;
    u4Reg = Backlight_PwmGetAddr_C(u1Channel);
    if (u4Reg != 0)
    {
        u4RVal = IO_READ32(HPDS_BASE, u4Reg + 4);
        u4RVal &= 0xFFFFF;

        return u4RVal;
    }
    else// means error
    {
        return PWM_RETURN_ERROR_C;
    }
}

// Duty All is RSN, so Duty cycle is Duty On, is PWM H.
// Paramters :
// u2Ratio : Duty cycle (0 ~ 1024 : 0-100%)
void DRVCUST_SetBLPwmDuty_C(UINT8 u1Channel, UINT16 u2Ratio)
{
    UINT32 u4Value;
    UINT32 u4Reg;
    UINT32 u4DutyCycle;

    if ((u1Channel > 1) || (u2Ratio > 1024))
    {
        LOG(0, "DRVCUST_SetBLPwmDuty_C,Parameter Incorrect! [Channel(%d),Ratio(%d)]\n",
            u1Channel, u2Ratio);
        return;
    }

    arPWM_Status_C[u1Channel].u2duty = u2Ratio;
    if (arPWM_Status_C[u1Channel].Mode == eHigh_Fix)
    {
        u2Ratio = 1024;
        LOG(0, "DRVCUST_SetBLPwmDuty_C high\n");
    }
    else if (arPWM_Status_C[u1Channel].Mode == eLow_Fix)
    {
        u2Ratio = 0;
        LOG(0, "DRVCUST_SetBLPwmDuty_C low\n");
    }
    u4Reg = Backlight_PwmGetAddr_C(u1Channel);
    if (u4Reg != 0)
    {
        if (u2Ratio <= 1024)   // 0~1024
        {
            if (u2Ratio == 1024)// fix when duty is 1024. duty cycle = 99.9%. duty cycle = duty/(1024 + 1)
            {
                u2Ratio = 1025;
            }
            // PWMRSN fix to 1024 and enable
            u4Value = (((UINT32)0x400 << 20) |
                       ((UINT32)(u2Ratio & 0xFFF) << 4) | 1);
            IO_WRITE32(HPDS_BASE, u4Reg, u4Value);

            u4DutyCycle = (u2Ratio * 100) / (1024 + 1);
            Printf("SONY_PWM SetDuty_C(%d): PWMH [0x%x], DutyCycle[%lu%%]\n", u1Channel, u2Ratio, u4DutyCycle);
        }
    }
}

// Return : Duty cycle (0 ~ 1024 : 1-100%)
// Return OK:[0, 1024] ; Fail : 0xFFFF
UINT16 DRVCUST_GetBLPwmDuty_C(UINT8 u1Channel)
{
    UINT32 u4Reg;
    UINT32 u4RVal = 0;
    u4Reg = Backlight_PwmGetAddr_C(u1Channel);
    if (u4Reg != 0)
    {
        u4RVal = IO_READ32(HPDS_BASE, u4Reg);
        u4RVal &= 0xFFF0;
        u4RVal = u4RVal >> 4;

        if (u4RVal == 1025)
        {
            u4RVal = 1024;
        }

        return u4RVal;
    }
    else// means error
    {
        return PWM_RETURN_ERROR_C;
    }
}

//---------------------------------------------------------------------------
// Static function forward declarations
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Static variables
//---------------------------------------------------------------------------
static INT32 _PWM_Set_PWM_Frequency(INT32 i4Argc, const CHAR** szArgv)
{
    UINT8 u1Channel;
    UINT32 u4TimeBase;

    if (i4Argc < 2)
    {
        Printf("%s [Channel] [TimeBase]\n", szArgv[0]);
        return 1;
    }

    u1Channel = StrToInt(szArgv[1]);
    u4TimeBase = StrToInt(szArgv[2]);
    DRVCUST_SetBLPwmFrequency_C(u1Channel, u4TimeBase);

    return 0;
}

static INT32 _PWM_Get_PWM_Frequency(INT32 i4Argc, const CHAR** szArgv)
{
    UINT8 u1Channel;
    UINT32 u4Ret;

    if (i4Argc < 1)
    {
        Printf("%s [Channel]\n", szArgv[0]);
        return 1;
    }

    u1Channel = StrToInt(szArgv[1]);
    u4Ret = DRVCUST_GetBLPwmFrequency_C(u1Channel);

    Printf("Channel(%d) TimeBase is(%d)\n", u1Channel, u4Ret);

    return 0;
}

static INT32 _PWM_Set_PWM_Duty(INT32 i4Argc, const CHAR** szArgv)
{
    UINT8 u1Channel;
    UINT32 u4Duty;

    if (i4Argc < 2)
    {
        Printf("%s [Channel] [Duty[0, 1024]]\n", szArgv[0]);
        return 1;
    }

    u1Channel = StrToInt(szArgv[1]);
    u4Duty = StrToInt(szArgv[2]);
    DRVCUST_SetBLPwmDuty_C(u1Channel, u4Duty);

    return 0;
}

static INT32 _PWM_Get_PWM_Duty(INT32 i4Argc, const CHAR** szArgv)
{
    UINT8 u1Channel;
    UINT32 u4Ret;

    if (i4Argc < 1)
    {
        Printf("%s [Channel]\n", szArgv[0]);
        return 1;
    }

    u1Channel = StrToInt(szArgv[1]);
    u4Ret = DRVCUST_GetBLPwmDuty_C(u1Channel);

    Printf("Channel(%d) duty is(%d)\n", u1Channel, u4Ret);

    return 0;
}

static INT32 _PWM_StressTest(INT32 i4Argc, const CHAR** szArgv)
{
    UINT8 u1Channel;
    UINT32 u4DutyRange = 0;// [0, 1024]
    UINT32 u4Value = 0;

    if (i4Argc < 1)
    {
        Printf("%s [Channel]\n", szArgv[0]);
        return 1;
    }

    u1Channel = StrToInt(szArgv[1]);

    for (u4DutyRange = 0; u4DutyRange < 1025; u4DutyRange++)
    {
        DRVCUST_SetBLPwmDuty_C(u1Channel, u4DutyRange);
        Printf("Set Duty(%d)", u4DutyRange);
        x_thread_delay(2);
        u4Value = DRVCUST_GetBLPwmDuty_C(u1Channel);
        Printf("Get Duty(%d)", u4Value);
    }

    Printf("Channel(%d) tested\n", u1Channel);

    return 0;
}

static INT32 _PWM_MtkSet(INT32 i4Argc, const CHAR** szArgv)
{
    UINT8 u1Channel;
    UINT32 u4PWMP = 0;
    UINT32 u4PWMH = 0;
    UINT32 u4PWMRSN = 0;
    UINT32 u4Frequency = 0;
    UINT32 u4DutyCycle = 0;
    UINT32 u4BusClock = 192000000;

    if (i4Argc < 4)
    {
        Printf("%s [Channel] [PWMP] [PWMH] [PWMRSN]\n", szArgv[0]);
        return 1;
    }

    u1Channel = StrToInt(szArgv[1]);
    u4PWMP = StrToInt(szArgv[2]);
    u4PWMH = StrToInt(szArgv[3]);
    u4PWMRSN = StrToInt(szArgv[4]);

    u4Frequency = u4BusClock / ((u4PWMP + 1) * (u4PWMRSN + 1));
    u4DutyCycle = (u4PWMH * 100) / (u4PWMRSN + 1);

    Printf("Channel(%d) PWMP [0x%lx],PWMH [0x%lx], PWMRSN [0x%lx], Frequency[%luHz], DutyCycle[%lx%%]\n",
           u1Channel, u4PWMP, u4PWMH, u4PWMRSN, u4Frequency, u4DutyCycle);

    vDrvSetPWM(u1Channel, u4PWMP, u4PWMH, u4PWMRSN);

    Printf("Channel(%d) seted\n", u1Channel);

    return 0;
}

static CLI_EXEC_T _arPWMCmdTbl [] =
{
    {"SetFrequency",     "sf", 	_PWM_Set_PWM_Frequency,     NULL, "PWM", CLI_GUEST},
    {"GetFrequency",     "gf", 	_PWM_Get_PWM_Frequency,     NULL, "PWM", CLI_GUEST},
    {"SetDuty",     "sd", 	_PWM_Set_PWM_Duty,     NULL, "PWM", CLI_GUEST},
    {"GetDuty",     "gd", 	_PWM_Get_PWM_Duty,     NULL, "PWM", CLI_GUEST},
    {"Stress",     "s", 	_PWM_StressTest,     NULL, "PWM", CLI_GUEST},
    {"mtk_set",     "ms", 	_PWM_MtkSet,     NULL, "PWM", CLI_GUEST},
    // Register test.
    CLIMOD_DEBUG_CLIENTRY(PWM),

    { NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR }
};


CLI_MAIN_COMMAND_ITEM(Pwm)
{
    "pwm", NULL, NULL, _arPWMCmdTbl, "Pwm command", CLI_GUEST
};




