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
 * Description:
 *         The CLI commands for gpio driver
 *
 *---------------------------------------------------------------------------*/

#include "x_serial.h"
#include "x_printf.h"
#include "x_os.h"
#include "x_stl_lib.h"
#include "x_assert.h"
#include "x_pinmux.h"
#include "drvcust_if.h"
#ifdef __MW_CLI_DEF__
#include "x_mid_cli.h"
#else
#include "x_drv_cli.h"
#endif
#define DEFINE_IS_LOG	GPIO_IsLog
#include "x_debug.h"
#include "x_timer.h"
#include "x_pdwnc.h"
#include "u_dbg_drv.h"

CLIMOD_DEBUG_FUNCTIONS(GPIO);

//===========================================================================
// Configuration
#define FPD_TEST_ENABLE             0

//---------------------------------------------------------------------

static INT32 _GPIOCLI_Init(INT32 i4Argc, const CHAR** aszArgv)
{
    GPIO_Init();
    return 0;
}

static INT32 _GPIOCLI_Stop(INT32 i4Argc, const CHAR** aszArgv)
{
    GPIO_Stop();
    return 0;
}

static INT32 _GPIOCLI_Reset(INT32 i4Argc, const CHAR** aszArgv)
{
    GPIO_Stop();
    GPIO_Init();
    return 0;
}

static INT32 _GPIOCLI_List(INT32 i4Argc, const CHAR** aszArgv)
{
    INT32 i, i4Intr, i4Fall, i4Rise;

    for (i = 0; i < TOTAL_GPIO_NUM; i++)
    {
        GPIO_Query(i, &i4Intr, &i4Fall, &i4Rise);
        if (i4Intr || i4Fall || i4Rise)
        {
            Printf("GPIO%ld(%c,%c,%c)\n", i, (i4Intr ? 'I' : 'X'),
                   (i4Fall ? 'F' : 'X'), (i4Rise ? 'R' : 'X'));
        }
    }
    return 0;
}

static void _GPIO_Callback(INT32 i4Gpio, BOOL fgStatus)
{
    Printf("GPIO:%ld fgStatus:%ld\n", i4Gpio, (INT32)fgStatus);
}

static INT32 _GPIOCLI_Enable(INT32 i4Argc, const CHAR** aszArgv)
{
    INT32 i, i4Gpio, i4Intr, i4Rise, i4Fall, i4Low, i4High;

    if (i4Argc < 2)
    {
        Printf("%s [-i -r -f -L -H] [gpio num]\n", aszArgv[0]);
        return 1;
    }

    i4Gpio = i4Intr = i4Rise = i4Fall = i4High = i4Low = 0;
    for (i = 1; i < i4Argc; i++)
    {
        switch (aszArgv[i][0])
        {
            case '-':
                if (aszArgv[i][1] == 'i')
                {
                    i4Intr = 1;
                }
                else if (aszArgv[i][1] == 'r')
                {
                    i4Rise = 1;
                }
                else if (aszArgv[i][1] == 'f')
                {
                    i4Fall = 1;
                }
                else if (aszArgv[i][1] == 'L')
                {
                    i4Low = 1;
                }
                else if (aszArgv[i][1] == 'H')
                {
                    i4High = 1;
                }
                break;
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                i4Gpio = StrToInt(aszArgv[i]);
                break;
            default:
                Printf("%s [-i -r -f -L -H] [gpio num]\n", aszArgv[0]);
                return 1;
        }
    }
    if (!i4Intr && !i4Rise && !i4Fall && !i4High && !i4Low)
    {
        Printf("%s [-i -r -f -L -H] [gpio num]\n", aszArgv[0]);
        return 1;
    }

    BSP_PinGpioSel(i4Gpio);
    GPIO_Reg(i4Gpio, GPIO_TYPE_NONE, NULL);
    Printf("GPIO %ld - register a", i4Gpio);
    if (i4Intr)
    {
        GPIO_Reg(i4Gpio, GPIO_TYPE_INTR_BOTH, _GPIO_Callback);
        Printf(" (wave fail and rise) ");
    }
    if (i4Fall)
    {
        GPIO_Reg(i4Gpio, GPIO_TYPE_INTR_FALL, _GPIO_Callback);
        Printf(" (wave fail) ");
    }
    if (i4Rise)
    {
        GPIO_Reg(i4Gpio, GPIO_TYPE_INTR_RISE, _GPIO_Callback);
        Printf(" (wave rise) ");
    }
    if (i4Low)
    {
        GPIO_Reg(i4Gpio, GPIO_TYPE_INTR_LEVEL_LOW, _GPIO_Callback);
        Printf(" (wave low) ");
    }
    if (i4High)
    {
        GPIO_Reg(i4Gpio, GPIO_TYPE_INTR_LEVEL_HIGH, _GPIO_Callback);
        Printf(" (wave high) ");
    }

    Printf("to trigger an interrupt.\n");
    return 0;
}

static INT32 _GPIOCLI_Disable(INT32 i4Argc, const CHAR** aszArgv)
{
    INT32 i4Gpio;

    if (i4Argc < 2)
    {
        Printf("%s [gpio num]\n", aszArgv[0]);
        return 1;
    }

    i4Gpio = StrToInt(aszArgv[1]);
    GPIO_Reg(i4Gpio, GPIO_TYPE_NONE, NULL);
    Printf("Disable all gpio function in gpio %ld\n", i4Gpio);
    return 0;
}

static INT32 _GPIOCLI_PinSel(INT32 i4Argc, const CHAR** aszArgv)
{
    INT32 i4GpioNum, i4FuncNo;
    UINT32 u4PinSel;

    if (i4Argc < 2)
    {
        Printf("%s [gpio num] [[func no.]]\n", aszArgv[0]);
        return 1;
    }

    i4GpioNum = StrToInt(aszArgv[1]);
    if (i4Argc < 3)
    {
        i4FuncNo = BSP_PinGpioGet(i4GpioNum, &u4PinSel);
        //Printf("PinSel:0x%08x", u4PinSel);
        Printf("GPIO %ld Current function is %ld\n", i4GpioNum, i4FuncNo);
        return 0;
    }
    i4FuncNo = StrToInt(aszArgv[2]);
    BSP_PinGpioGet(i4GpioNum, &u4PinSel);
    BSP_PinSet(u4PinSel, i4FuncNo);
    Printf("GPIO %ld is set to Func%ld\n", i4GpioNum, i4FuncNo);
    //i4FuncNo = BSP_PinGet(u4PinSel);
    //Printf("PinSel:0x%08x GpioNum:%d Current function is %d\n", u4PinSel, i4GpioNum, i4FuncNo);
    //Printf("\tLoRegIdx:%d LoMask:%d LoBits:%d\n", LO_MUX_REG(u4PinSel), LO_MASK(u4PinSel), LO_BIT_OFFSET(u4PinSel));
    //Printf("\tHiRegIdx:%d HiMask:%d HiBits:%d\n", HI_MUX_REG(u4PinSel), HI_MASK(u4PinSel), HI_BIT_OFFSET(u4PinSel));
    //Printf("\tMaxFunc:%d\n", MAX_FUNC(u4PinSel));
    return 0;
}

static INT32 _GPIOCLI_Output(INT32 i4Argc, const CHAR** aszArgv)
{
    INT32 i4Gpio, i4Val, i4Set;

    if (i4Argc < 2)
    {
        Printf("%s [gpio num] [[0|1]] (0 is low; 1 is high)\n", aszArgv[0]);
        return 1;
    }
    i4Gpio = StrToInt(aszArgv[1]);

    if (i4Argc == 2)   /* Query only. */
    {
        i4Set = GPIO_Output(i4Gpio, NULL);
        Printf("GPIO %ld output mode is set as %ld\n", i4Gpio, i4Set);
        return 0;
    }
    i4Set = StrToInt(aszArgv[2]);

    i4Val = 0;
    GPIO_Enable(i4Gpio, &i4Val); /* Change to input mode first. */
    
    GPIO_Output(i4Gpio, &i4Set);
    
    i4Val = 1;
    GPIO_Enable(i4Gpio, &i4Val); /* Change to output mode. */

    if (i4Set == 0)
    {
        Printf("GPIO %ld output mode is set as [low]\n", i4Gpio);
    }
    if (i4Set == 1)
    {
        Printf("GPIO %ld output mode is set as [high]\n", i4Gpio);
    }
    return 0;
}

static INT32 _GPIOCLI_Output_Verify(INT32 i4Argc, const CHAR** aszArgv)
{
    INT32 i4Gpio_start, i4Gpio_end, i4Val, i;
    INT32 i4TsetCount = 0;

    if (i4Argc > 3)
    {
        Printf("%s gpio.out_verify [gpio_start] [gpio_end]", aszArgv[0]);
        return 1;
    }
    else if (i4Argc == 3)
    {
        i4Gpio_start = StrToInt(aszArgv[1]);
        i4Gpio_end = StrToInt(aszArgv[2]);
    }
    else
    {
        i4Gpio_start = 0;
        i4Gpio_end = NORMAL_GPIO_NUM;
    }

    for (i = i4Gpio_start; i <= i4Gpio_end; i++)
    {
        GPIO_Enable(i, 0);    /* Change to input mode first. */
    }

    while (i4TsetCount < 10000)// 20s
    {
        for (i = i4Gpio_start; i <= i4Gpio_end; i++)
        {
            i4Val = 0;
            GPIO_Output(i, &i4Val);
            i4Val = 1;
            GPIO_Enable(i, &i4Val); /* Change to output mode. */
            GPIO_Output(i, &i4Val);
        }
        x_thread_delay(1000);
        for (i = i4Gpio_start; i <= i4Gpio_end; i++)
        {
            i4Val = 0;
            GPIO_Output(i, &i4Val);
            i4Val = 1;
            GPIO_Enable(i, &i4Val); /* Change to output mode. */
            GPIO_Output(i, &i4Val);
            x_thread_delay(1000);
        }
        x_thread_delay(1000);

        i4TsetCount++;
    }

    return 0;
}

static INT32 _GPIOCLI_Input(INT32 i4Argc, const CHAR** aszArgv)
{
    INT32 i4Gpio, i4Val;

    if (i4Argc < 2)
    {
        Printf("%s [gpio num]\n", aszArgv[0]);
        return 1;
    }

    i4Gpio = StrToInt(aszArgv[1]);
    i4Val = 0;
    GPIO_Enable(i4Gpio, &i4Val); /* Change to input mode first. */
    i4Val = GPIO_Input(i4Gpio);
    if (i4Val == 0)
    {
        Printf("Read GPIO %ld input value is low\n", i4Gpio);
    }
    if (i4Val == 1)
    {
        Printf("Read GPIO %ld input value is high\n", i4Gpio);
    }
    return 0;
}
static INT32 _GPIOCLI_VoltageLvl(INT32 i4Argc, const CHAR** aszArgv)
{
    INT32 i4Gpio, i4Val;
    INT32 i, j;
    if (i4Argc < 2)
    {
        //Printf ALL
        Printf("%s [gpio num]\t(list specific gpio level)\n", aszArgv[0]);
        Printf("%s \t\t(list all)\n", aszArgv[0]);

        Printf("\r   ");
        for (j = 0; j < 10; j++)
        {
            Printf("%2ld", j);
        }
        Printf("\n");
        for (i = 0; i < 13 ; i++)
        {
            Printf("%02ld ", i);
            for (j = 0; j < 10; j++)
            {
                i4Val = GPIO_Input( i * 10 + j );
                Printf(" %s", (i4Val) ? "H" : "L" );
            }
            Printf("\n");
        }
        return 0;
    }

    //Print specific GPIO
    i4Gpio = StrToInt(aszArgv[1]);
    i4Val = 0;

    i4Val = GPIO_Input( i4Gpio );

    Printf("Read GPIO %ld value is %ld\n", i4Gpio, i4Val);
    return 0;
}

#if FPD_TEST_ENABLE
extern INT32 MW_FpdSet7Seg(UINT8 u1Digit);
#endif /* FPD_TEST_ENABLE */

static INT32 _GPIOCLI_Seg7(INT32 i4Argc, const CHAR** aszArgv)
{
#if FPD_TEST_ENABLE
    INT32 i4Gpio, i4Disable, i4Enable;

    if (i4Argc > 1)
    {
        MW_FpdSet7Seg(aszArgv[1][0]);
        return 0;
    }

    i4Disable = 1;
    i4Enable = 0;
    UNUSED(i4Enable);
    for (i4Gpio = 115; i4Gpio < 128; i4Gpio++)
    {
        GPIO_Output(i4Gpio, &i4Disable);
    }
#endif /* FPD_TEST_ENABLE */
    return 0;
}

#if FPD_TEST_ENABLE
extern INT32 MW_FpdSetBuf(UINT32 u4Offset, UINT8* pu1Digit);
extern INT32 FPD_DevInit(void);
#endif /* FPD_TEST_ENABLE */

static INT32 _GPIOCLI_Time(INT32 i4Argc, const CHAR** aszArgv)
{
#if FPD_TEST_ENABLE
    INT32 i;
    UINT8 szBuf[10];

    FPD_DevInit();
    if ((i4Argc >= 2) && aszArgv[1][0] == '-' && aszArgv[1][1] == 'l')
    {
        for (i = 0; i < 9999; i++)
        {
            x_sprintf((CHAR*)szBuf, "%04ld", i);
            MW_FpdSetBuf(0, szBuf);
            x_thread_delay(50);
        }
        return 0;
    }

    if (i4Argc >= 2)
    {
        MW_FpdSetBuf(0, (UINT8*)aszArgv[1]);
        return 0;
    }

    x_sprintf((CHAR*)szBuf, "%04ld", 1234);
    MW_FpdSetBuf(0, szBuf);
#endif /* FPD_TEST_ENABLE */
    return 0;
}

#define KEY_ESCAPE                      0x1b
static INT32 _i4Idx;
static UCHAR _aucArray[256];

static INT32 _GPIOCLI_Test(INT32 i4Argc, const CHAR** aszArgv)
{
    INT32 i;
    UCHAR uc;

    if ((i4Argc == 1) || ((i4Argc > 1) && (aszArgv[1][0] != '-')))
    {
        for (i = 0; i < _i4Idx; i++)
        {
            if (i % 16 == 15)
            {
                Printf("0x%02x\n", _aucArray[i]);
            }
            else
            {
                Printf("0x%02x ", _aucArray[i]);
            }
        }
        return 0;
    }

    switch (aszArgv[1][1])
    {
        case 'i':
        case 'I':
            _i4Idx = 0;
            do
            {
                // SerInByte(&uc);
                uc = SerPollGetChar();
                if (uc != KEY_ESCAPE)
                {
                    _aucArray[_i4Idx] = uc;
                    if ( _i4Idx < 127)
                    {
                        _i4Idx++;
                    }
                    _i4Idx &= 0xff;
                }
            }
            while (uc != KEY_ESCAPE);
            break;
        case '0':
        default:
            _i4Idx = 0;
            for (i = 0; i < 128; i++)
            {
                _aucArray[i] = 0;
            }
            break;
    }
    return 0;
}

#ifndef __KERNEL__
#ifndef CC_MINI_DRIVER
extern void BSP_DoCoreDump(void* prCtx);
#endif /* CC_MINI_DRIVER */
extern void BSP_ShowReg(void);

static INT32 _GPIOCLI_CoreDump(INT32 i4Argc, const CHAR** aszArgv)
{
#ifndef CC_MINI_DRIVER
    BSP_DoCoreDump(NULL);
#endif /* CC_MINI_DRIVER */
    return 0;
}

static INT32 _GPIOCLI_ShowReg(INT32 i4Argc, const CHAR** aszArgv)
{
    BSP_ShowReg();
    return 0;
}
#endif /* __KERNEL__ code not in Linux */

static INT32 _GPIOCLI_Assert(INT32 i4Argc, const CHAR** aszArgv)
{
    ASSERT(0);
    return 0;
}

typedef void (* PFN_VOID)(void);
static UINT32 u4Data0xffff = 0xffffffff;

static INT32 _GPIOCLI_Abort(INT32 i4Argc, const CHAR** aszArgv)
{
    PFN_VOID pfnAbort;

    if (i4Argc != 2)
    {
        Printf("gpio.abort [p|d|u]\n");
        Printf("\tPrefetch abort|Data abort|Undefined Instruction\n");
        return 1;
    }

    switch (aszArgv[1][0])
    {
        case 'p':
        case 'P':
            pfnAbort = (PFN_VOID)0xabcdef12;
            pfnAbort();
            break;
        case 'd':
        case 'D':
            u4Data0xffff = *((UINT32*) 0xabcdef12);
            break;
        case 'u':
        case 'U':
            pfnAbort = (PFN_VOID) & u4Data0xffff;
            pfnAbort();
            break;
        default:
            Printf("gpio.abort [p|d|u]\n");
            Printf("\tPrefetch abort|Data abort|Undefined Instruction\n");
            break;
    }
    return 0;
}

static INT32 _GPIOCLI_Servo(INT32 i4Argc, const CHAR** aszArgv)
{
    UINT32 u4Val;

    if (i4Argc < 2)
    {
        Printf("%s [0-4] to read servo adc channel value.\n", aszArgv[0]);
        return -1;
    }

    u4Val = StrToInt(aszArgv[1]);
    Printf("Channel %lu value is %lu\n", u4Val, PDWNC_ReadServoADCChannelValue(u4Val));
    return 0;
}

static void _GpioCLI_WatchdogThread(void* pvArgv)
{
    UINT32 u4Seconds;
    HAL_TIME_T rPrevTime, rTime, rDelta;

    u4Seconds = *((UINT32*)pvArgv);
    Printf("u4Seconds:%lu\n", u4Seconds);

    HAL_GetTime(&rPrevTime);

    do
    {
        HAL_GetTime(&rTime);
        HAL_GetDeltaTime(&rDelta, &rPrevTime, &rTime);
    }
    while (rDelta.u4Seconds < u4Seconds);

    Printf("Finish %lu seconds\n", u4Seconds);
}

typedef void (*PFN_TESTFUNC)(void);
const UINT32 _au4Swi[] = { 0xef000045, 0xef000046 };

static INT32 _GPIOCLI_TestWatchdog(INT32 i4Argc, const CHAR** aszArgv)
{
    PFN_TESTFUNC pfnTest;
    UINT32 u4Priority = 0;
    UINT32 u4Seconds;
    INT32 i4Ret;
    HANDLE_T hWatchdogThread;

    if (i4Argc < 2)
    {
        Printf("%s s|m|n|t [priority] [seconds]\n", aszArgv[0]);
        return 0;
    }
    else if (aszArgv[1][0] == 's')
    {
        pfnTest = (PFN_TESTFUNC)&_au4Swi;
        pfnTest();
        return 0;
    }
    else if (aszArgv[1][0] == 'n')
    {
        pfnTest = (PFN_TESTFUNC)0x30000000;//david for klocwork warning (PFN_TESTFUNC)0;
        pfnTest();
    }
    else if (aszArgv[1][0] == 'm')
    {
        x_dbg_abort(__LINE__, 0x12345678);
    }
    else if (aszArgv[1][0] == 't')
    {
        if (i4Argc >= 4)
        {
            u4Priority = StrToInt(aszArgv[2]);
            u4Seconds = StrToInt(aszArgv[3]);
        }
        if ((i4Argc < 4) || (u4Priority == 0) || (u4Seconds == 0))
        {
            Printf("%s s|m|n|t [priority] [seconds]\n", aszArgv[0]);
            return 0;
        }

        u4Priority = (u4Priority > 250) ? 250 : u4Priority;
        u4Seconds = (u4Seconds > 10) ? 10 : u4Seconds;

        i4Ret = x_thread_create(&hWatchdogThread, "wtest", 4096, u4Priority,
                                _GpioCLI_WatchdogThread, 4, &u4Seconds);
        if (i4Ret != OSR_OK)
        {
            Printf("Create thread to test watchdog failed.\n");
            return 1;
        }
    }

    return 0;
}

static INT32 _GPIOCLI_Status(INT32 i4Argc, const CHAR** aszArgv)
{

    UINT32 i4GpioNum;
    INT32 i4Pinmux, i4Enable, i4Output, i4Input;

    if (i4Argc < 2)
    {
        Printf("%s GpioNum\n", aszArgv[0]);
        return 1;
    }

    i4GpioNum = StrToInt(aszArgv[1]);

    if (GPIO_Status(i4GpioNum, &i4Pinmux, &i4Enable, &i4Output, &i4Input) != 0)
    {
        Printf("Status Error\n");
        return -1;
    }

    Printf("Pinmux Function: %d\n", i4Pinmux);

    Printf("GPIO %lu(%s): %ld\n", i4GpioNum, i4Enable ? "OUPUT" : "INPUT", i4Enable ? i4Output : i4Input);

    return 0;

}
#ifdef CC_POWER_MERGE_TEST
static HANDLE_T _GpioThread0 = NULL;
static HANDLE_T _GpioThread1 = NULL;
static HANDLE_T _GpioThread2 = NULL;

#define GPIO_T_DELAY_BASE 50
static void _GPIOPowerMergeProc0(void* pvArg)
{
    UINT32 i;

    i = 0;
    do
    {
        if (i >= (GPIO_T_DELAY_BASE - 1))
        {
            i = 0;
        }
        else
        {
            i++;
        }
        x_thread_delay(GPIO_T_DELAY_BASE);
        GPIO_SetOut(GPIO(1), 1);
        x_thread_delay(GPIO_T_DELAY_BASE - i);
        GPIO_SetOut(GPIO(11), 0);
        x_thread_delay(GPIO_T_DELAY_BASE + i);
        GPIO_SetOut(GPIO(0), 1);
        x_thread_delay(GPIO_T_DELAY_BASE - i);
        GPIO_SetOut(GPIO(8), 0);
        x_thread_delay(GPIO_T_DELAY_BASE);
        GPIO_SetOut(GPIO(12), 1);
        x_thread_delay(GPIO_T_DELAY_BASE - i);
        GPIO_SetOut(GPIO(9), 0);
        x_thread_delay(GPIO_T_DELAY_BASE + i);
        GPIO_SetOut(GPIO(10), 1);
        x_thread_delay(GPIO_T_DELAY_BASE - i);
        GPIO_SetOut(GPIO(3), 0);
        x_thread_delay(GPIO_T_DELAY_BASE - i);
        GPIO_SetOut(GPIO(4), 1);
        x_thread_delay(GPIO_T_DELAY_BASE);
        GPIO_SetOut(GPIO(1), 0);
        x_thread_delay(GPIO_T_DELAY_BASE - i);
        GPIO_SetOut(GPIO(11), 1);
        x_thread_delay(GPIO_T_DELAY_BASE + i);
        GPIO_SetOut(GPIO(0), 0);
        x_thread_delay(GPIO_T_DELAY_BASE - i);
        GPIO_SetOut(GPIO(8), 1);
        x_thread_delay(GPIO_T_DELAY_BASE);
        GPIO_SetOut(GPIO(12), 0);
        x_thread_delay(GPIO_T_DELAY_BASE - i);
        GPIO_SetOut(GPIO(9), 1);
        x_thread_delay(GPIO_T_DELAY_BASE + i);
        GPIO_SetOut(GPIO(10), 0);
        x_thread_delay(GPIO_T_DELAY_BASE - i);
        GPIO_SetOut(GPIO(3), 1);
        x_thread_delay(GPIO_T_DELAY_BASE - i);
        GPIO_SetOut(GPIO(4), 0);
    }
    while (1);
}
static void _GPIOPowerMergeProc1(void* pvArg)
{
    UINT32 i;
    i = 0;

    do
    {
        if (i >= (GPIO_T_DELAY_BASE - 1))
        {
            i = 0;
        }
        else
        {
            i++;
        }
        x_thread_delay(GPIO_T_DELAY_BASE);
        GPIO_SetOut(GPIO(208), 1);
        x_thread_delay(GPIO_T_DELAY_BASE - i);
        GPIO_SetOut(GPIO(209), 0);
        x_thread_delay(GPIO_T_DELAY_BASE + i);
        GPIO_SetOut(GPIO(200), 1);
        x_thread_delay(GPIO_T_DELAY_BASE - i);
        GPIO_SetOut(GPIO(201), 0);
#if 0 //OPWRSB
        x_thread_delay(GPIO_T_DELAY_BASE);
        GPIO_SetOut(GPIO(210), 1);
#endif
        x_thread_delay(GPIO_T_DELAY_BASE - i);
        GPIO_SetOut(GPIO(211), 0);
#if 0 //U0RX/U0TX
        x_thread_delay(GPIO_T_DELAY_BASE + i);
        GPIO_SetOut(GPIO(214), 1);
        x_thread_delay(GPIO_T_DELAY_BASE - i);
        GPIO_SetOut(GPIO(215), 0);
#endif
        x_thread_delay(GPIO_T_DELAY_BASE - i);
        GPIO_SetOut(GPIO(202), 1);
        x_thread_delay(GPIO_T_DELAY_BASE);
        GPIO_SetOut(GPIO(203), 0);
        x_thread_delay(GPIO_T_DELAY_BASE - i);
        GPIO_SetOut(GPIO(204), 1);
        x_thread_delay(GPIO_T_DELAY_BASE);
        GPIO_SetOut(GPIO(208), 0);
        x_thread_delay(GPIO_T_DELAY_BASE - i);
        GPIO_SetOut(GPIO(209), 1);
        x_thread_delay(GPIO_T_DELAY_BASE + i);
        GPIO_SetOut(GPIO(200), 0);
        x_thread_delay(GPIO_T_DELAY_BASE - i);
        GPIO_SetOut(GPIO(201), 1);
#if 0 //OPWRSB
        x_thread_delay(GPIO_T_DELAY_BASE);
        GPIO_SetOut(GPIO(210), 0);
#endif
        x_thread_delay(GPIO_T_DELAY_BASE - i);
        GPIO_SetOut(GPIO(211), 1);
#if 0 //U0RX/U0TX
        x_thread_delay(GPIO_T_DELAY_BASE + i);
        GPIO_SetOut(GPIO(214), 0);
        x_thread_delay(GPIO_T_DELAY_BASE - i);
        GPIO_SetOut(GPIO(215), 1);
#endif
        x_thread_delay(GPIO_T_DELAY_BASE - i);
        GPIO_SetOut(GPIO(202), 0);
        x_thread_delay(GPIO_T_DELAY_BASE);
        GPIO_SetOut(GPIO(203), 1);
        x_thread_delay(GPIO_T_DELAY_BASE - i);
        GPIO_SetOut(GPIO(204), 0);

        //HDMI pin gpio funciton
        x_thread_delay(GPIO_T_DELAY_BASE);
        GPIO_SetOut(GPIO(205), 0);
        x_thread_delay(GPIO_T_DELAY_BASE - i);
        GPIO_SetOut(GPIO(206), 1);
        x_thread_delay(GPIO_T_DELAY_BASE);
        GPIO_SetOut(GPIO(207), 0);
        x_thread_delay(GPIO_T_DELAY_BASE - i);
        GPIO_SetOut(GPIO(232), 1);
        x_thread_delay(GPIO_T_DELAY_BASE + i);
        GPIO_SetOut(GPIO(233), 0);
        x_thread_delay(GPIO_T_DELAY_BASE - i);
        GPIO_SetOut(GPIO(234), 1);
        x_thread_delay(GPIO_T_DELAY_BASE);
        GPIO_SetOut(GPIO(235), 0);
        x_thread_delay(GPIO_T_DELAY_BASE - i);
        GPIO_SetOut(GPIO(236), 1);
        x_thread_delay(GPIO_T_DELAY_BASE + i);
        GPIO_SetOut(GPIO(237), 0);
        x_thread_delay(GPIO_T_DELAY_BASE - i);
        GPIO_SetOut(GPIO(238), 1);
        x_thread_delay(GPIO_T_DELAY_BASE - i);
        GPIO_SetOut(GPIO(239), 0);
        x_thread_delay(GPIO_T_DELAY_BASE);
        GPIO_SetOut(GPIO(205), 1);
        x_thread_delay(GPIO_T_DELAY_BASE - i);
        GPIO_SetOut(GPIO(206), 0);
        x_thread_delay(GPIO_T_DELAY_BASE);
        GPIO_SetOut(GPIO(207), 1);
        x_thread_delay(GPIO_T_DELAY_BASE - i);
        GPIO_SetOut(GPIO(232), 0);
        x_thread_delay(GPIO_T_DELAY_BASE + i);
        GPIO_SetOut(GPIO(233), 1);
        x_thread_delay(GPIO_T_DELAY_BASE - i);
        GPIO_SetOut(GPIO(234), 0);
        x_thread_delay(GPIO_T_DELAY_BASE);
        GPIO_SetOut(GPIO(235), 1);
        x_thread_delay(GPIO_T_DELAY_BASE - i);
        GPIO_SetOut(GPIO(236), 0);
        x_thread_delay(GPIO_T_DELAY_BASE + i);
        GPIO_SetOut(GPIO(237), 1);
        x_thread_delay(GPIO_T_DELAY_BASE - i);
        GPIO_SetOut(GPIO(238), 0);
        x_thread_delay(GPIO_T_DELAY_BASE - i);
        GPIO_SetOut(GPIO(239), 1);

        //vga_sda/scl pin gpio function
        x_thread_delay(GPIO_T_DELAY_BASE);
        GPIO_SetOut(GPIO(212), 1);
        x_thread_delay(GPIO_T_DELAY_BASE - i);
        GPIO_SetOut(GPIO(213), 0);
        x_thread_delay(GPIO_T_DELAY_BASE + i);
        GPIO_SetOut(GPIO(213), 1);
        x_thread_delay(GPIO_T_DELAY_BASE - i);
        GPIO_SetOut(GPIO(212), 0);
    }
    while (1);
}
static void _GPIOPowerMergeProc2(void* pvArg)
{
    UINT32 i;
    i = 0;

    do
    {
        if (i >= (GPIO_T_DELAY_BASE - 1))
        {
            i = 0;
        }
        else
        {
            i++;
        }
        x_thread_delay(GPIO_T_DELAY_BASE);
        GPIO_SetOut(GPIO(2), 1);
        x_thread_delay(GPIO_T_DELAY_BASE - i);
        GPIO_SetOut(GPIO(5), 0);
        x_thread_delay(GPIO_T_DELAY_BASE + i);
        GPIO_SetOut(GPIO(5), 1);
        x_thread_delay(GPIO_T_DELAY_BASE - i);
        GPIO_SetOut(GPIO(2), 0);
    }
    while (1);
}

static INT32 _GPIOCLI_PowerMergeTest(INT32 i4Argc, const CHAR** aszArgv)
{

    UINT32 TestCase;

    if (i4Argc < 2)
    {
        Printf("please select power merge test for: 0-MEMPLL 1-PLLGP 2-USB\n");
        return 1;
    }

    TestCase = StrToInt(aszArgv[1]);

    switch (TestCase)
    {
        case 0:	//for MEMPLL
            if (x_thread_create(&_GpioThread0, "GPIO_Thread0", 4096, 200, _GPIOPowerMergeProc0, 0, NULL) == OSR_OK)
            {
                Printf("Create thread toggle for MEMPLL power merge test success!\n");
            }
            break;

        case 1:	//for PLLGP
            if (x_thread_create(&_GpioThread1, "GPIO_Thread1", 4096, 200, _GPIOPowerMergeProc1, 0, NULL) == OSR_OK)
            {
                Printf("Create thread toggle for PLLGP power merge test success!\n");
            }

            break;

        case 2:	//for USB
            if (x_thread_create(&_GpioThread2, "GPIO_Thread2", 4096, 200, _GPIOPowerMergeProc2, 0, NULL) == OSR_OK)
            {
                Printf("Create thread toggle gpio2/5 for USB power merge test success!\n");
            }
            break;

        default:

            break;
    }

    return 0;

}
#endif
//===========================================================================

static CLI_EXEC_T arGpioCmdTbl[] =
{
    CLIMOD_DEBUG_CLIENTRY(GPIO),
    DECLARE_CMD(_GPIOCLI_Init, init, init, "gpio.init"),
    DECLARE_CMD(_GPIOCLI_Stop, stop, stop, "gpio.stop"),
    DECLARE_CMD(_GPIOCLI_Reset, reset, reset, "gpio.reset"),
    DECLARE_CMD(_GPIOCLI_List, list, ls, "gpio.ls"),
    DECLARE_CMD(_GPIOCLI_Enable, enable, en, "gpio.en [-i/-r/-f/-L/-H][gpio num]"),
    DECLARE_CMD(_GPIOCLI_Disable, disable, dis, "gpio.dis [gpio num]"),
    DECLARE_CMD(_GPIOCLI_PinSel, pinsel, ps, "gpio.ps [sel num] [[func no.]]"),
    DECLARE_G_CMD(_GPIOCLI_Output, output, out, "gpio.out [gpio num] [[0|1]]"),
    DECLARE_G_CMD(_GPIOCLI_Status, status, st, "gpio.status [gpio num]"),
    DECLARE_G_CMD(_GPIOCLI_Output_Verify, output_verify, out_v, "gpio.out_verify [gpio_start] [gpio_end]"),
    DECLARE_G_CMD(_GPIOCLI_Input, input, in, "gpio.in [gpio num]"),
    DECLARE_G_CMD(_GPIOCLI_VoltageLvl, vl , vl, "gpio.vl [gpio num]"),
    DECLARE_CMD(_GPIOCLI_Seg7, seg7, seg7, "gpio.seg7"),
    DECLARE_CMD(_GPIOCLI_Time, time, time, "gpio.time"),
    DECLARE_CMD(_GPIOCLI_Test, test, test, "gpio.test"),
#ifndef __KERNEL__
    DECLARE_CMD(_GPIOCLI_CoreDump, coredump, co, "gpio.co or gpio.coredump"),
    DECLARE_CMD(_GPIOCLI_ShowReg, showreg, sr, "gpio.sr or gpio.showreg"),
#endif /* __KERNEL__ code not in Linux */
    DECLARE_CMD(_GPIOCLI_Assert, assert, assert, "gpio.assert"),
    DECLARE_CMD(_GPIOCLI_Abort, abort, abort, "gpio.abort"),
    DECLARE_G_CMD(_GPIOCLI_Servo, servo, servo, "gpio.servo [0-4]"),
    DECLARE_CMD(_GPIOCLI_TestWatchdog, testwatchdog, twg, "gpio.twg - to test watchdog"),
#ifdef CC_POWER_MERGE_TEST
    DECLARE_CMD(_GPIOCLI_PowerMergeTest, pwrmergetest, pmt, "gpio.pmt"),
#endif
    DECLARE_END_ITEM(),
};

CLI_MAIN_COMMAND_ITEM(Gpio)
{
    "gpio",
    NULL,
    NULL,
    arGpioCmdTbl,
    "Gpio interface",
    CLI_GUEST
};

