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
 *      Memory test commands
 *
 *---------------------------------------------------------------------------*/

#include "x_printf.h"
#ifdef __MW_CLI_DEF__
#include "x_mid_cli.h"
#else
#include "x_drv_cli.h"
#endif
#include "x_stl_lib.h"
#include "x_assert.h"
#include "x_ckgen.h"
#include "x_bim.h"
#include "x_timer.h"
#include "x_os.h"
#include "x_hal_926.h"
#include "x_util.h"
#include "x_ldr_env.h"
#include "drv_pwm.h"
#include "x_ldr_env.h"
#include "../lzma/LzmaEnc.h"
#include "../lzma/LzmaLib.h"
#ifdef CC_TRUSTZONE_SUPPORT
#include "tz_if.h"
#endif
#include "loader_imghdr.h"
#ifndef CPU_TEST_ENABLE
#define CPU_TEST_ENABLE     0
#endif

//=====================================================================
// extern functions
//=====================================================================
EXTERN INT32 CacheMeterQuery(UINT32 *pu4IReq, UINT32 *pu4IMiss, UINT32 *pu4IPenalty,
        UINT32 *pu4DReq, UINT32 *pu4DMiss, UINT32 *pu4DPenalty);
EXTERN INT32 CmdEfusePowerPinOn(INT32 i4Argc, const CHAR** aszArgv);
EXTERN INT32 CmdEfusePowerPinOff(INT32 i4Argc, const CHAR** aszArgv);
EXTERN INT32 CmdEfuseSecureID(INT32 i4Argc, const CHAR** aszArgv);
EXTERN INT32 CmdEfuseSecureData(INT32 i4Argc, const CHAR** aszArgv);
EXTERN INT32 CmdEfuseDump(INT32 i4Argc, const CHAR** aszArgv);
EXTERN INT32 BIM_GetDeviceID(BYTE *pu1DeviceID, BYTE u1Size);
EXTERN INT32 BIM_GetDeviceData(BYTE *pu1DeviceID, BYTE u1Size);

/* Event type */
#define PM_EVENT_INSTR_CACHE_MISS  1
#define PM_EVENT_BRANCH_COUNT      0xc
#define PM_EVENT_BRANCH_MISPREDICT 0x10
#define PM_EVENT_INSTR_EXECUTED    0x8
#define PM_EVENT_DATA_CACHE_ACCESS 0x4
#define PM_EVENT_DATA_CACHE_MISS   0x3
#define PM_EVENT_TOTAL_CYCLES      0x11


static void _DisablePMReg(void)
{
    register UINT32 u4Reg = 3;

#ifdef __KERNEL__
    __asm__ __volatile__("mcr p15, 0, %0, c9, c12,  2 \n\t"   // Disable counter 0,1
                         : "+r" (u4Reg));
#else
    __asm
    {
        mcr p15, 0, u4Reg, c9, c12 , 2
    }
#endif
}

static void _EnablePMReg(void)
{
    register UINT32 u4Reg = 3;

#ifdef __KERNEL__
    __asm__ __volatile__("mcr p15, 0, %0, c9, c12,  1 \n\t"   // Enable counter 0,1
                         "mrc p15, 0, %0, c9, c12,  0 \n\t"   // Read PMCR
                         "orr %0, %0, #1              \n\t"   // Enable
                         "mcr p15, 0, %0, c9, c12,  0 \n\t"   // Write
                         : "+r" (u4Reg));
#else
    __asm
    {
        mcr p15, 0, u4Reg, c9, c12 , 1
        mrc p15, 0, u4Reg, c9, c12,  0
        orr u4Reg, u4Reg, #1
        mcr p15, 0, u4Reg, c9, c12,  0
    }
#endif
}

static void _ResetPMReg(void)
{
    register UINT32 u4Reg = 0, u4Counter = 0;

#ifdef __KERNEL__
    __asm__ __volatile__("mcr p15, 0, %1, c9, c12 , 5 \n\t"   // Select counter 0
                         "mcr p15, 0, %0, c9, c13 , 2 \n\t"   // Clear.
                         "mov %1, #1                  \n\t"
                         "mcr p15, 0, %1, c9, c12 , 5 \n\t"   // Select counter 1
                         "mcr p15, 0, %0, c9, c13 , 2 \n\t"   // Clear.
                         : "+r" (u4Reg), "+r" (u4Counter));
#else
    __asm
    {
        mcr p15, 0, u4Counter, c9, c12 , 5
        mcr p15, 0, u4Reg, c9, c13 , 2
        mov u4Counter, #1
        mcr p15, 0, u4Counter, c9, c12 , 5
        mcr p15, 0, u4Reg, c9, c13 , 2
    }
#endif
}

static void _SetPMRegEvtCnt0(UINT32 u4Evt)
{
    register UINT32 u4Counter = 0;
    register UINT32 u4EvtShifted = (u4Evt & 0xFF);

#ifdef __KERNEL__
    __asm__ __volatile__("mcr p15, 0, %0, c9, c12 , 5 \n\t"   // Select counter 0
                         "mcr p15, 0, %1, c9, c13 , 1 \n\t"   // Set event type
                         : "+r" (u4Counter) : "r" (u4EvtShifted));
#else
    __asm
    {
        mcr p15, 0, u4Counter, c9, c12 , 5
        mcr p15, 0, u4EvtShifted, c9, c13, 1
    }
#endif
}

static void _SetPMRegEvtCnt1(UINT32 u4Evt)
{
        register UINT32 u4Counter = 1;
        register UINT32 u4EvtShifted = (u4Evt & 0xFF);

#ifdef __KERNEL__
        __asm__ __volatile__("mcr p15, 0, %0, c9, c12 , 5 \n\t"   // Select counter 0
                             "mcr p15, 0, %1, c9, c13 , 1 \n\t"   // Set event type
                             : "+r" (u4Counter) : "r" (u4EvtShifted));
#else
        __asm
        {
            mcr p15, 0, u4Counter, c9, c12 , 5
            mcr p15, 0, u4EvtShifted, c9, c13, 1
        }
#endif
}

static UINT32 _GetCr0(void)
{
    register UINT32 u4Reg = 0;

#ifdef __KERNEL__
    __asm__ __volatile__("mcr p15, 0, %0, c9, c12 , 5 \n\t"   // Select counter 0
                         "mrc p15, 0, %0, c9, c13 , 2 \n\t"   // Get counter value
                         : "+r" (u4Reg));
#else
    __asm
    {
        mcr p15, 0, u4Reg, c9, c12 , 5
        mrc p15, 0, u4Reg, c9, c12 , 2
    }
#endif
    return u4Reg;
}

static UINT32 _GetCr1(void)
{
    register UINT32 u4Reg = 1;

#ifdef __KERNEL__
    __asm__ __volatile__("mcr p15, 0, %0, c9, c12 , 5 \n\t"   // Select counter 1
                         "mrc p15, 0, %0, c9, c13 , 2 \n\t"   // Get counter value
                         : "+r" (u4Reg));
#else
    __asm
    {
        mcr p15, 0, u4Reg, c9, c12 , 5
        mrc p15, 0, u4Reg, c9, c12 , 2
    }
#endif
    return u4Reg;
}

typedef struct
{
    UINT32 u4Timer;
	UINT32 u4Val;
	UINT32 u4Offset;
    CHAR * szString;
}BOOTTIME;
#define MAX_BOOTTIME_ITEM 256
static UINT32 u4BoottimeItem=0;
static BOOTTIME _arBootTime[MAX_BOOTTIME_ITEM];
void _CmdStoreTimeItem(UINT32 u4Timer, CHAR *szString)
{
   if(u4BoottimeItem>=MAX_BOOTTIME_ITEM)
	{
	Printf("%s %d: StoreBootTime full\n", __FUNCTION__, __LINE__);
	return;
	}
    _arBootTime[u4BoottimeItem].u4Timer=u4Timer;
    _arBootTime[u4BoottimeItem].szString=szString;
	u4BoottimeItem++;
}
void _CmdArrayTime(void)
{
    UINT32 i,j;
	UINT32 u4Timer;
	CHAR * szString;

    for(i=0;i<u4BoottimeItem-1;i++)
		for(j=i+1;j<u4BoottimeItem;j++)
			{
			if(_arBootTime[i].u4Timer<=_arBootTime[j].u4Timer)
				{
				u4Timer= _arBootTime[i].u4Timer;
				_arBootTime[i].u4Timer =_arBootTime[j].u4Timer;
				_arBootTime[j].u4Timer =u4Timer;
				szString= _arBootTime[i].szString;
				_arBootTime[i].szString =_arBootTime[j].szString;
				_arBootTime[j].szString =szString;
				}
			}
	for(i=0;i<u4BoottimeItem;i++)
		{
		_arBootTime[i].u4Val=((~_arBootTime[i].u4Timer)/(GET_XTAL_CLK()/1000000));
		if(i>0)
		_arBootTime[i].u4Offset =_arBootTime[i].u4Val-_arBootTime[i-1].u4Val;
		else
		_arBootTime[i].u4Offset =0;
	}
 }
void _CmdDisplayTime(void)
{
   UINT32 i;
   for(i=0;i<u4BoottimeItem;i++)
     Printf("0x%08x | %6d.%03d ms  %6d.%03d ms - %s\n",(unsigned int)_arBootTime[i].u4Timer, (int)(_arBootTime[i].u4Val / 1000), (int)(_arBootTime[i].u4Val % 1000), (int)(_arBootTime[i].u4Offset/ 1000), (int)(_arBootTime[i].u4Offset% 1000),_arBootTime[i].szString);
}

#ifdef __KERNEL__
extern TIME_STAMP_T * x_os_drv_get_timestampKernel(UINT32 *pu4Size);
#endif
#ifdef TIME_MEASUREMENT
#include "x_time_msrt.h"
#include "u_time_msrt_name.h"
#endif

static INT32 _CmdBootTime(INT32 i4Argc, const CHAR** aszArgv)
{
    UINT32 *pu4Time;

#ifndef __KERNEL__
    UINT32 i, u4Size;
    TIME_STAMP_T *prTimeStamp;

    u4BoottimeItem=0;
    pu4Time = (UINT32 *)0xf00080e0;
    _CmdStoreTimeItem(*pu4Time, "Before CHIP_NotifyT8032");
    pu4Time++;
    _CmdStoreTimeItem(*pu4Time, "After CHIP_NotifyT8032");
    pu4Time++;
    _CmdStoreTimeItem(*pu4Time, "After dram calibration");
    pu4Time++;
    _CmdStoreTimeItem(*pu4Time, "After loader decompress");
    pu4Time++;
    _CmdStoreTimeItem(*pu4Time, "Before x_drv_init()");
    pu4Time++;
    _CmdStoreTimeItem(*pu4Time, "After x_drv_init()");

    pu4Time = ((LDR_ENV_T*)CC_LDR_ENV_OFFSET)->au4TimeStamp;
    for (i=0; i<LDR_TIMESTAMP_SIZE; i++)
    {
        _CmdStoreTimeItem(*pu4Time, "loader");
        pu4Time++;
    }

    prTimeStamp = x_os_drv_get_timestamp(&u4Size);
    for (i=0; i<u4Size; i++)
    {
        _CmdStoreTimeItem(prTimeStamp[i].u4TimeStamp, prTimeStamp[i].szString);
    }
	_CmdArrayTime();
	_CmdDisplayTime();
#else
    UINT32 i, u4Size;
    TIME_STAMP_T *prTimeStamp;
    pu4Time = (UINT32 *)0xf00080e0;
	u4BoottimeItem=0;
    _CmdStoreTimeItem(*pu4Time, "Preloader Jump to Loader time");
    pu4Time++;
    _CmdStoreTimeItem(*pu4Time, "Loader Jump to Uboot time");
    pu4Time++;
    _CmdStoreTimeItem(*pu4Time, "Uboot board init time");
    pu4Time++;
    _CmdStoreTimeItem(*pu4Time, "Uboot Jump to Kernel time");
    prTimeStamp = x_os_drv_get_timestamp(&u4Size);
    for (i=0; i<u4Size; i++)
    {
        _CmdStoreTimeItem(prTimeStamp[i].u4TimeStamp, prTimeStamp[i].szString);
    }
	prTimeStamp = x_os_drv_get_timestampKernel(&u4Size);
    for (i=0; i<u4Size; i++)
    {
        _CmdStoreTimeItem(prTimeStamp[i].u4TimeStamp, prTimeStamp[i].szString);
    }
    _CmdArrayTime();
	_CmdDisplayTime();
#endif
#ifdef TIME_MEASUREMENT
       Printf("================================================\n");
       _time_msrt_dump_detail(NULL);
#endif

    return 0;
}
#ifndef __KERNEL__
static INT32 _CmdLzhsDec(INT32 i4Argc, const CHAR** aszArgv)
{
    UINT32 u4SrcAddr, u4DestAddr, u4Len;
    UINT8 u1Checksum;

    u4SrcAddr = StrToInt(aszArgv[1]);
    u4DestAddr = StrToInt(aszArgv[2]);
    u4Len = StrToInt(aszArgv[3]);
    u1Checksum = StrToInt(aszArgv[4]);
    if (LZHS_Dec(u4SrcAddr, u4DestAddr, u4Len, &u1Checksum)!=0)
    {
        Printf("LZHS decode failed\n");
        return 1;
    }
    Printf("LZHS decode ok\n");
    return 0;
}

static INT32 _CmdLzmaDec(INT32 i4Argc, const CHAR** aszArgv)
{
#if CPU_TEST_ENABLE
    /*
    b.memcpy 0x8000000 0xf8030000 0x300000
    bim.lzhs 0x8000040 0x9000000
    bim.lzhs 0xf8030040 0x9000000
    */
    UINT32 u4DstSize, u4DstAddr, u4SrcSize, u4SrcAddr;
    UINT32 u4ImageAddr = StrToInt(aszArgv[1]);
    LZMA_FILE_HEADER* prLzmaHdr = (LZMA_FILE_HEADER*)u4ImageAddr;

    u4DstSize = prLzmaHdr->org_size;
    u4DstAddr = StrToInt(aszArgv[2]);
    u4SrcAddr = u4ImageAddr + sizeof(LZMA_FILE_HEADER);
    u4SrcSize = prLzmaHdr->compressed_size;

    Printf("LZMA decode, src{addr=0x%08x, len=%d(0x%08x)}, dst{addr=0x%08x, len=%d(0x%08x)}\n",
        u4SrcAddr, u4SrcSize, u4SrcSize, u4DstAddr, u4DstSize, u4DstSize);
    UINT32 u4Ret = (UINT32)LzmaUncompress((unsigned char*)u4DstAddr, (size_t*)&u4DstSize,
        (unsigned char*)u4SrcAddr, (size_t*)&u4SrcSize,
        (unsigned char *)&prLzmaHdr->props, sizeof(CLzmaEncProps));

    if (u4Ret != SZ_OK)
    {
        Printf("LZMA fail, code=%d\n", u4Ret);
        return 1;
    }
    Printf("LZMA decode ok\n");
#endif
    return 0;
}
#endif

static INT32 _CmdPwmTest(INT32 i4Argc, const CHAR** aszArgv)
{
    UINT32 u4Src, u4DutyAll, u4DutyON,u4TimeBase;

    u4Src = StrToInt(aszArgv[1]);
    u4DutyAll = StrToInt(aszArgv[2]);
    u4DutyON = StrToInt(aszArgv[3]);
    u4TimeBase = StrToInt(aszArgv[4]);
    vDrvSetPWM(u4Src, u4TimeBase, u4DutyON,
                 u4DutyAll);
    return 0;
}
#if defined(CC_MT5890)
static INT32 _CmdCPUClock(INT32 i4Argc, const CHAR** aszArgv)
{
    UINT32 u4Clock,u4Value,u4Change;

    u4Clock = StrToInt(aszArgv[1]);
	u4Value = u4Clock/12;
	
    u4Change = u4IO32Read4B(CKGEN_ANA_PLLGP_CFG0);
	//u4Change = u4Change&(0x80000000);
    vIO32WriteFldAlign(CKGEN_ANA_PLLGP_CFG0, u4Value, FLD_ANA_HPCPLL_SDM_PCW_INTEGER);
	if(u4Change & 0x80000000)
	 {
	vIO32Write4B(CKGEN_ANA_PLLGP_CFG0,(u4IO32Read4B(CKGEN_ANA_PLLGP_CFG0) & 0x7fffffff));
	 }
	else
	 {
	vIO32Write4B(CKGEN_ANA_PLLGP_CFG0,(u4IO32Read4B(CKGEN_ANA_PLLGP_CFG0) & 0x7fffffff)|0x80000000);
	 }
    return 0;
}
#endif
static INT32 _CmdCacheMonitor(INT32 i4Argc, const CHAR** aszArgv)
{
    UINT32 u4Type, u4Period, u4Req, u4Val, u4Cr0, u4Cr1;
    UINT64 u8Req, u8Val, u8Cr0, u8Cr1;
    INT32 i4Loop;
    BOOL fReg = FALSE;

    if (i4Argc==1)
    {
        Printf("args: event_type(2:data read, 4:data write, 7:ins. read), period(ms), loop\n");
        return 0;
    }
    u4Type = (UINT32)StrToInt(aszArgv[1]);
    u4Period = (UINT32)StrToInt(aszArgv[2]);
    i4Loop = StrToInt(aszArgv[3]) - 1;

    do
    {
        //1. reset and disable counter
        L2C_WRITE32(L2C_REG_EV_COUNTER_CTRL, L2C_REG_EV_COUNTER1_RST | L2C_REG_EV_COUNTER0_RST);
        while (L2C_READ32(L2C_REG_EV_COUNTER_CTRL)) {};
        _DisablePMReg();
        _ResetPMReg();

        //2. set the event source
        L2C_WRITE32(L2C_REG_EV_COUNTER0_CFG, u4Type << L2C_REG_EV_TYPE_BIT_SHIFT);

        if (u4Type==2)
        {
            L2C_WRITE32(L2C_REG_EV_COUNTER1_CFG, L2C_REG_EV_TYPE_DRREQ);

            _SetPMRegEvtCnt0(PM_EVENT_DATA_CACHE_ACCESS);
            _SetPMRegEvtCnt1(PM_EVENT_DATA_CACHE_MISS);
            //CLI_Parser("timeprofile.epm dcmr");
            fReg = TRUE;
        }
        else if (u4Type==4)
        {
            L2C_WRITE32(L2C_REG_EV_COUNTER1_CFG, L2C_REG_EV_TYPE_DWREQ);

            _SetPMRegEvtCnt0(PM_EVENT_DATA_CACHE_ACCESS);
            _SetPMRegEvtCnt1(PM_EVENT_DATA_CACHE_MISS);
            //CLI_Parser("timeprofile.epm dcmr");
            fReg = TRUE;
        }
        else if (u4Type==7)
        {
            L2C_WRITE32(L2C_REG_EV_COUNTER1_CFG, L2C_REG_EV_TYPE_IRREQ);

            _SetPMRegEvtCnt0(PM_EVENT_INSTR_EXECUTED);
            _SetPMRegEvtCnt1(PM_EVENT_INSTR_CACHE_MISS);
            //CLI_Parser("timeprofile.epm icmr");
            fReg = TRUE;
        }

//#define CACHE_TEST
#ifdef CACHE_TEST
        HalFlushInvalidateDCache();
#endif

        //3. enable
        L2C_WRITE32(L2C_REG_EV_COUNTER_CTRL, L2C_REG_EV_COUNTING_EN);
        _EnablePMReg();

#ifdef CACHE_TEST
        #define BASE_ADDRESS 0xc000000 // here should be FMB memory
        #define NONCACHE(addr) (((addr) & 0x0fffffff) | 0x40000000)
        #define L2C_SIZE (128*1024)
        #define L2C_LINE_SIZE 32
        #define TEST_LINE 4
        do
        {
            UINT32 i, j;
            volatile UINT8* pu1Start = (UINT8*)BASE_ADDRESS;
            // NOTE: L1 is noWA when write miss
            u4Val = *(UINT8*)(pu1Start);
            for (j=0; j<10; j++)
            {
                for (i=0; i<L2C_SIZE; i++)
                {
                    *pu1Start++ = ((UINT32)pu1Start & 0xff);
                }
            }
        }
        while (0);
#else
        x_thread_delay(u4Period);
#endif

        //4. disable
        L2C_WRITE32(L2C_REG_EV_COUNTER_CTRL, 0);
        _DisablePMReg();

        //5. read result
        u4Val = L2C_READ32(L2C_REG_EV_COUNTER0);
        u4Cr0 = _GetCr0();
        u4Cr1 = _GetCr1();
        u8Cr0 = (UINT64)u4Cr0;
        u8Cr1 = (UINT64)u4Cr1;

        if (fReg)
        {
            //CLI_Parser("timeprofile.gpm");
            //CLI_Parser("timeprofile.spm");

            u4Req = L2C_READ32(L2C_REG_EV_COUNTER1);
            if (u4Req>0)
            {
                u8Req = (UINT64)u4Req;
                u8Val = (UINT64)u4Val;
                Printf("L2 cache miss rate: %d (o/oo) [hit=%d/req=%d]\n", (INT32)u8Div6432((u8Req-u8Val)*1000, u8Req, NULL),  (INT32)u4Val, (INT32)u4Req);
            }
            else
            {
                Printf("L2 cache miss rate: 0 [hit=0/req=0]\n");
            }
        }
        else
        {
            Printf("L2 cache event count: %d\n", u4Val);
        }
        Printf("L1 cache miss rate: %d (o/oo) [miss=%d/req=%d]\n\n", (INT32)u8Div6432(u8Cr1*1000, u8Cr0 ? u8Cr0 : 1, NULL),  (INT32)u4Cr1,  (INT32)u4Cr0);
    }
    while (i4Loop--);

    return 0;
}

#if defined(CC_TRUSTZONE_SUPPORT) && defined(__KERNEL__)

static INT32 _CmdTzEnableDebug(INT32 i4Argc, const CHAR** aszArgv)
{
    TZ_EnableDebug(StrToInt(aszArgv[1]),i4Argc>2?StrToInt(aszArgv[2]):0,i4Argc>3?StrToInt(aszArgv[3]):0);
    return 0;
}

static INT32 _CmdTzRunUT(INT32 i4Argc, const CHAR** aszArgv)
{
    TZ_RunUT(StrToInt(aszArgv[1]));
    return 0;
}

static void PASS_TH1(void *pvArg)
{
    UINT32 u4Cnt_Pass_TH1 = 0;

    Printf("PASS_TH1\n");

    while (1)
    {
        TZ_RunST();
        u4Cnt_Pass_TH1++;

        if ((u4Cnt_Pass_TH1 % 1000) == 0)
        {
            Printf("u4Cnt_PASS_TH1 = %d\n", u4Cnt_Pass_TH1);
        }
        x_thread_delay(1);
    }
}

static void PASS_TH2(void *pvArg)
{
    HANDLE_T hdl;
    UINT32 u4Cnt_Pass_TH2 = 0;

    Printf("PASS_TH2\n");

    x_thread_self(&hdl);
    if (hdl)
    {
        x_thread_set_affinity(hdl, 2);
    }

    while (1)
    {
        TZ_RunST();
        u4Cnt_Pass_TH2++;

        if ((u4Cnt_Pass_TH2 % 1000) == 0)
        {
            Printf("u4Cnt_PASS_TH2 = %d\n", u4Cnt_Pass_TH2);
        }
        x_thread_delay(1);
    }
}

static void PASS_TH3(void *pvArg)
{
    UINT32 u4Cnt_Pass_TH3 = 0;

    Printf("PASS_TH3\n");

    while (1)
    {
        TZ_RunST();
        u4Cnt_Pass_TH3++;

        if ((u4Cnt_Pass_TH3 % 1000) == 0)
        {
            Printf("u4Cnt_PASS_TH3 = %d\n", u4Cnt_Pass_TH3);
        }
        x_thread_delay(1);
    }
}

static void PASS_TH4(void *pvArg)
{
    HANDLE_T hdl;
    UINT32 u4Cnt_Pass_TH4 = 0;

    Printf("PASS_TH4\n");
    x_thread_self(&hdl);
    if (hdl)
    {
        x_thread_set_affinity(hdl, 2);
    }

    while (1)
    {
        TZ_RunST();
        u4Cnt_Pass_TH4++;

        if ((u4Cnt_Pass_TH4 % 1000) == 0)
        {
            Printf("u4Cnt_PASS_TH4 = %d\n", u4Cnt_Pass_TH4);
        }
        x_thread_delay(1);
    }
}

HANDLE_T h_PASS_TH1;
HANDLE_T h_PASS_TH2;
HANDLE_T h_PASS_TH3;
HANDLE_T h_PASS_TH4;
static INT32 _CmdTzRunST(INT32 i4Argc, const CHAR** aszArgv)
{
    INT32 i4_ret;

    i4_ret = x_thread_create(&h_PASS_TH1, "PASS_TH1", 4096, 200, PASS_TH1, 4, 0);
    if (i4_ret != 0)
    {
        Printf("x_thread_create error (th1)\n");
    }

    i4_ret = x_thread_create(&h_PASS_TH2, "PASS_TH2", 4096, 200, PASS_TH2, 4, 0);
    if (i4_ret != 0)
    {
        Printf("x_thread_create error (th2)\n");
    }

    i4_ret = x_thread_create(&h_PASS_TH3, "PASS_TH3", 4096, 200, PASS_TH3, 4, 0);
    if (i4_ret != 0)
    {
        Printf("x_thread_create error (th3)\n");
    }

    i4_ret = x_thread_create(&h_PASS_TH4, "PASS_TH4", 4096, 200, PASS_TH4, 4, 0);
    if (i4_ret != 0)
    {
        Printf("x_thread_create error (th4)\n");
    }

    return 0;
}

#ifndef CC_NDEBUG
static INT32 _CmdTzRead(INT32 i4Argc, const CHAR** aszArgv)
{
    UINT32 u4Val = 0;

    if(i4Argc != 2)
    {
        Printf("%s base offset\n", aszArgv[0]);
        return 0;
    }

    TZ_IO_READ32(StrToInt(aszArgv[1]), 0, &u4Val);
    Printf("0x%x\n", u4Val);

    return 0;
}

static INT32 _CmdTzWrite(INT32 i4Argc, const CHAR** aszArgv)
{
    if(i4Argc != 3)
    {
        Printf("%s base offset value\n", aszArgv[0]);
        return 0;
    }

    TZ_IO_WRITE32(StrToInt(aszArgv[1]), 0, StrToInt(aszArgv[2]));

    return 0;
}
#endif // CC_NDEBUG

static CLI_EXEC_T _arTzCmdTbl[] =
{
    { "st", "st", _CmdTzRunST, NULL, "run tz stress test", CLI_SUPERVISOR },
    { "ut", "ut", _CmdTzRunUT, NULL, "run tz unit test", CLI_SUPERVISOR },
    #ifndef CC_NDEBUG
    { "read", "r", _CmdTzRead, NULL, "read register in tz", CLI_SUPERVISOR },
    { "write", "w", _CmdTzWrite, NULL, "write register in tz", CLI_SUPERVISOR },
    #endif // CC_NDEBUG
    { "debug", "d_l", _CmdTzEnableDebug, NULL, "enable tz debug message", CLI_SUPERVISOR },
    { NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR }
};
#endif

static CLI_EXEC_T _arBIMCmdTbl[] =
{
    {
        "CacheMonitor",
        "cache",
        _CmdCacheMonitor,
        NULL,
        "show cache statistic",
        CLI_SUPERVISOR
    },
    {
        "BootTime",
        "bt",
        _CmdBootTime,
        NULL,
        "Boot time parsing",
        CLI_GUEST
    },
#ifndef __KERNEL__
    {
        "lzhs",
        "lzhs",
        _CmdLzhsDec,
        NULL,
        "lzhs [image src] [target mem] [dest size] [checksum]",
        CLI_SUPERVISOR
    },
    {
        "lzma",
        "lzma",
        _CmdLzmaDec,
        NULL,
        "lzma [src addr] [target addr]",
        CLI_SUPERVISOR
    },
#endif
#if defined(CC_TRUSTZONE_SUPPORT) && defined(__KERNEL__)
    {
        "trustzone",
        "tz",
        NULL,
        _arTzCmdTbl,
        "trustzone commands",
        CLI_SUPERVISOR
    },
#endif

    {
        "pwm",
        "p",
        _CmdPwmTest,
        NULL,
        "PWM [PWM src][u4DutyAll 0-0xfe][u4DutyOn 0-0xff][u4TimeBase 0-0xfff]",
        CLI_SUPERVISOR
    },
 #if defined(CC_MT5890)
    {
    "ck",
    "ck",
    _CmdCPUClock,
    NULL,
    "cpu clock [value]",
    CLI_SUPERVISOR
    },
 #endif
    // last cli command record, NULL
    {
        NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR
    }
};

/*----------------------------------------------------------------------------
 * Function:
 *
 * Description:
 *
 * Inputs: -
 *
 * Outputs: -
 *
 * Returns:
 *      TRUE: Succeed
 *      FALSE: Fail
 *---------------------------------------------------------------------------*/
CLI_MAIN_COMMAND_ITEM(BIM)
{
    "bim",
    NULL,
    NULL,
    _arBIMCmdTbl,
    "BIM module test",
    CLI_GUEST
};

