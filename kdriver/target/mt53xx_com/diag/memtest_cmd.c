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

#define DEFINE_IS_LOG       MEMTEST_IsLog
#include "x_debug.h"

#include "x_stl_lib.h"
#include "x_assert.h"

#include "x_timer.h"
#include "x_bim.h"
#include "drvcust_if.h"
#include "c_model.h"
#include "x_dram.h"
#include "x_ckgen.h"
#include "x_os.h"
#include "x_hal_arm.h"
//---------------------------------------------------------------------------
// Configurations
//---------------------------------------------------------------------------
//#define DRAM_HWONLINECALDQS

#define DRAM_MEMORY_CHECK
#ifdef CC_MT5399
//#define DRAM_MEMORY_AGENTTEST 
#endif
//#define HW_rDQS_OVER_SAMPLE
//---------------------------------------------------------------------------
// Constant definitions
//---------------------------------------------------------------------------
#define MEMTEST_DEFAULT_ADDR        0x2000000
#define MEMTEST_DEFAULT_SIZE        0x10000
#define MEMTEST_DEFAULT_LOOP        100
#define RECORD_DQS_REG              (IO_VIRT + 0x8100)

#define MEMTEST_BUFFERSIZE     7808

#define HWCALI_BUFFERSIZE     8192
#define HWCALI_ALIGNBITS     6
// reserve address align and 3 bytes check data.
#define HWCALI_ACTUALBUFFERSIZE (HWCALI_BUFFERSIZE + (1<<HWCALI_ALIGNBITS))
#define HWCALI_START 0
#define HWCALI_PROCESS 1

//---------------------------------------------------------------------------
// Type definitions
//---------------------------------------------------------------------------
#define BINARY2GRAY(u4Bin)   ((u4Bin) ^ ((u4Bin) >> 1))

//---------------------------------------------------------------------------
// Macro definitions
//---------------------------------------------------------------------------
#ifndef IS_DDR_ASYNCON
#define IS_DDR_ASYNCON()        (0)
#endif

#ifndef IS_DDR_CTSMODE
#define IS_DDR_CTSMODE()        (0)
#endif

#ifndef DDR_MIN_VAL
#define DDR_MIN_VAL     0
#endif
#ifndef SDR_x1
#define SDR_x1                1
#endif
#ifndef DDR_I_x1
#define DDR_I_x1            2
#endif
#ifndef DDR_I_x2
#define DDR_I_x2            3
#endif
#ifndef DDR_II_x1
#define DDR_II_x1          4
#endif
#ifndef DDR_II_x2
#define DDR_II_x2          5
#endif
#ifndef DDR_II_x3
#define DDR_II_x3          6
#endif
#ifndef DDR_II_x4
#define DDR_II_x4          7
#endif
#ifndef DDR_III_x1
#define DDR_III_x1          8
#endif
#ifndef DDR_III_x2
#define DDR_III_x2          9
#endif
#ifndef DDR_III_x3
#define DDR_III_x3          10
#endif
#ifndef DDR_III_x4
#define DDR_III_x4          11
#endif
#ifndef DDR_MAX_VAL
#define DDR_MAX_VAL     12
#endif

#define DQS_GATING_STEP 3

//---------------------------------------------------------------------------
// Imported variables
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Imported functions
//---------------------------------------------------------------------------
extern void VDP_Memory_Test(UINT32 u4Color);
#if defined(DRAM_MEMORY_CHECK)
extern void MEM_Test( UINT32 u4Loop, UINT32 u4Addr, UINT32 u4Size );
extern int memLineBounceTest(unsigned int u4Start, unsigned int u4Size,
    unsigned int verbose);
extern int memButterflyTest(unsigned int u4Start, unsigned int u4Size,
    unsigned int verbose);
#endif /* #if defined(DRAM_MEMORY_CHECK) */
//---------------------------------------------------------------------------
// Static function forward declarations
//---------------------------------------------------------------------------
#ifdef DRAM_HWONLINECALDQS
static void _DDR_DQSCalDisable(void);
#endif
static void _DDR_ThreadInit(void);

//---------------------------------------------------------------------------
// Static variables
//---------------------------------------------------------------------------
#ifdef DRAM_HWONLINECALDQS
//base[29:6] (unit:byte)
static UINT32 au4HwCaliBuffer[HWCALI_ACTUALBUFFERSIZE]; 
static UINT32 u4HwCalDQSEnable = 0;
static UINT32 u4HWCaliState = HWCALI_START;
#endif
static UINT32 u4Cmemrw = 0;
static BOOL fgCmemrwAbort = TRUE;
static BOOL fgDRAMThreadInit = FALSE;
static UINT32 *pu4M8K = NULL;
static UINT32 *pu4ChAM8K = NULL;
#ifdef __KERNEL__
static UINT32 *pu4ChBM8K = (UINT32 *)(0xF3000000);   /*Linux: Map logical addr = 0xF3000000 to physical addr =0x2FFFE180*/ 
#else
static UINT32 *pu4ChBM8K = (UINT32 *)(0x30000000 - MEMTEST_BUFFERSIZE);
#endif
static HAL_TIME_T rOrgTime, rNewTime, rDiffTime;
static UINT32 u4InitSec;
static HANDLE_T phDRAMThreadHdl;

//---------------------------------------------------------------------------
// Static functions
//---------------------------------------------------------------------------
#if !defined(CC_MT5890) 
static UINT32 _DDR_4BitTo4Byte( UINT32 i ) 
{ 
    return (((i & 8) ? 0xFF000000 : 0) | ((i & 4) ? 0x00FF0000 : 0) | 
                  ((i & 2) ? 0x0000FF00 : 0) | ((i & 1) ? 0x000000FF : 0));
}

static INT32 _DDR_NextIter( INT32 iter )
{
    INT32 new_iter;

    INT32 i = (iter >> 8) & 255;
    INT32 j = iter & 255;

    if (i<=j)
    {
        new_iter = (j << 8) | i;

        if (j == (i + 1))
        {
            new_iter = ((i + 1) << 8) | 15;

            if (iter == 0x0E0F)
            {
                new_iter = 0x0F0E;
            }
        }

        if (i == j)
        {
            new_iter = 0x000F; //iter must be 0 now.
        }
    }
    else
    {
        new_iter = (j << 8) | (i - 1);

        if (i == (j + 1))
        {
            new_iter = ((i - 1) << 8) | (j - 1);

            if (j == 0)
            {
                new_iter = 0;
            }
        }
    }

    return new_iter;
}

#if defined(DRAM_MEMORY_CHECK)
static void _DDR_CpuDspMemTest(void)
{
    UINT32 i;

    if (!pu4M8K)
    {
        return;
    }
    
    for (i=0; i<10; i=i+2)
    {
        if (memLineBounceTest((unsigned int)pu4M8K, MEMTEST_BUFFERSIZE, 0) == 0)
        {
            // Test error, show red at whole panel.
            VDP_Memory_Test(0);
            while(1);
        }

        if (memButterflyTest((unsigned int)pu4M8K, MEMTEST_BUFFERSIZE, 0) == 0)
        {
            // Test error, show red at whole panel.
            VDP_Memory_Test(0);
            while(1);
        }

        x_thread_delay(100);
    }
}
#endif /* #if defined(DRAM_MEMORY_CHECK)  */

#if defined(DRAM_MEMORY_AGENTTEST)
#if defined(CC_MT5399)||defined(CC_MT5882)
static void _DDR_TestAgentMemTest(void)
{
	U32 u4result = 0x0;
    u4result = DramcEngine2(TE_OP_WRITE_READ_CHECK, DEFAULT_TEST2_0_CAL, 0x33000000, DEFAULT_TEST2_2_CAL);
	if(u4result != 0)
	{
		Printf("Test agent fail, error code is 0x%08x \n", u4result);
		while(1);
	}
}
#else
static void _DDR_TestAgentMemTest(void)
{

	UINT32 u4Val,u4Reg280_ori,u4Reg280,u4Reg284;
#if defined(CC_MT5398) || defined(CC_MT5880)
	DRAM_WRITE32(0x18,DRAM_READ32(0x18) & (~0x00800000));  //extenal Dram agent 15 disable
#endif

	
	//Enable test agent-2
    u4Val = DRAM_READ32(0x218);
    u4Val &= ~0xF0000000;
    DRAM_WRITE32(0x218, u4Val |0x80000000);
	//Printf("\n-----Test agent2 DQ Calibration .\n");
    do
    {
        u4Reg280_ori= DRAM_READ32(0x280);
        u4Reg280 = u4Reg280_ori & 0x444;
    }while((u4Reg280 != 0x404) && (u4Reg280 != 0x444));
    
    if(u4Reg280 == 0x444)
    {
    	u4Reg284 = DRAM_READ32(0x284);
    	Printf("W. -----Test agent2 DQ Calibration Error,u4Reg284 = 0x%08X .\n",u4Reg284);
        u4Val = DRAM_READ32(0x218);
        u4Val &= ~0xF0000000;
        DRAM_WRITE32(0x218, u4Val);
        
        u4Val = DRAM_READ32(0x218);
        u4Val &= ~0xF0000000;
        DRAM_WRITE32(0x218, u4Val | 0x40000000);

        do
        {
            u4Reg280_ori= DRAM_READ32(0x280);
            u4Reg280 = u4Reg280_ori & 0x444;
        }while((u4Reg280 != 0x404) && (u4Reg280 != 0x444));

        u4Reg284 = DRAM_READ32(0x284);
        Printf("R.-----Test agent2 DQ Calibration Error,u4Reg284 = 0x%08X .\n",u4Reg284);
		while(1);
    }
    
    //Disable test agent2
    u4Val = DRAM_READ32(0x218);
    u4Val &= ~0xF0000000;
    DRAM_WRITE32(0x218, u4Val);
#if defined(CC_MT5398) || defined(CC_MT5880)
	DRAM_WRITE32(0x18,DRAM_READ32(0x18) | 0x00800000 );  //extenal Dram agent 15 enabling
#endif

}
#endif//#ifdef CC_MT5399
#endif//#if defined(DRAM_MEMORY_AGENTTEST) 

static UINT32 _DDR_IsDQSFail(void)
{
    UINT32 u4Addr = (UINT32)pu4M8K;
    UINT32 u4Val;
    UINT32 ib = 17;
    INT32 iter, c, iter_mod_3 = 0;
    UINT32 u4temp, i;

    for (c = 0; c<32; c++)
    {
        // 1 c is 4Byte*241, 8 c is nearly (<) 8K Bytes
        if ((c & 7) == 0)
        {
            //start of write or read
            u4Addr = (UINT32)pu4M8K;

            HalFlushInvalidateDCacheMultipleLine(u4Addr, MEMTEST_BUFFERSIZE);

            ib = 17;
            iter_mod_3 = 0;
        }

        iter = 0;

        do
        {
            if (c & 16)
            {
                ib = 32;
            }

            u4Val = _DDR_4BitTo4Byte(iter & 255) ^ (1 << ib);

            if (c & 8)
            {
                u4temp = IO_READ32(0, u4Addr);

                if (u4temp ^ u4Val)
                {
                    Printf("Iter:%d, Read Addr=0x%X.\n", iter, u4Addr);
                    Printf("Expect Val=0x%08X, Read Val=0x%08X.\n", u4Val, u4temp);

                    Printf("Try to read 200 times...\n");
                    for (i = 0; i<200; i++)
                    {
                        u4temp = IO_READ32(0, u4Addr);
                        if (u4temp ^ u4Val)
                        {
                            Printf("(%03d) Read Val = 0x%08X, XOR Value=0x%X.\n", 
                                i, u4temp, (u4temp ^ u4Val));
                        }
                    }
                    return TRUE;
                }
            }

            IO_WRITE32(0, u4Addr, u4Val);

            if (iter_mod_3 == 0)
            {
                ib = (ib + 13) & 31;

                iter_mod_3 = 3;
            }

            iter_mod_3--;
            u4Addr += 4;
            iter = _DDR_NextIter(iter);
        } while (iter != 0);
    }

    return FALSE;
}

static INT32 _DDR_Cmdmemoryrw( INT32 i4Argc, const CHAR ** aszArgv )
{
    if (_DDR_IsDQSFail() == FALSE)
    {
        Printf("memrw ok\n");
    }
    else
    {
        Printf("memrw fail\n");
    }

    return 0;
}

#endif //#if !defined(CC_MT5890) 

static INT32 _DDR_ChSelect( INT32 i4Argc, const CHAR ** aszArgv )
{
#if defined(CC_MT5890)
    Printf("memtest.ch [0: channel A, 1: channel B, 2: channel C]\n");

    if ((UINT32)StrToInt(aszArgv[1]) == 1)
    {
        if (IS_DRAM_CHANNELB_SUPPORT())
        {
            TCMSET_CHANNELB_ACTIVE();
            Printf("Channel B is selected.\n");
            pu4M8K = pu4ChBM8K;
        }
        else
        {
            Printf("Channel B is not support.\n");
        }
    }
	else if ((UINT32)StrToInt(aszArgv[1]) == 2)
    {
        if (IS_DRAM_CHANNELC_SUPPORT())
        {
            TCMSET_CHANNELC_ACTIVE();
            Printf("Channel C is selected.\n");
        }
        else
        {
            Printf("Channel C is not support.\n");
        }
    }
    else
    {
        TCMSET_CHANNELA_ACTIVE();
        Printf("Channel A is selected.\n");        
        pu4M8K = pu4ChAM8K;
    }
#else
     Printf("memtest.ch [0: channel A, 1: channel B]\n");

    if ((UINT32)StrToInt(aszArgv[1]) == 1)
    {
        if (IS_DRAM_CHANNELB_SUPPORT())
        {
            TCMSET_CHANNELB_ACTIVE();
            Printf("Channel B is selected.\n");
            pu4M8K = pu4ChBM8K;
        }
        else
        {
            Printf("Channel B is not support.\n");
        }
    }
    else
    {
        TCMSET_CHANNELA_ACTIVE();
        Printf("Channel A is selected.\n");        
        pu4M8K = pu4ChAM8K;
    }
#endif// defined(CC_MT5890)
    return 0;
}

#if !defined(CC_MT5890) 
#if defined(DRAM_MEMORY_CHECK)
static INT32 _DDR_CmdMemoryTest( INT32 i4Argc, const CHAR ** aszArgv )
{
    UINT32 u4Loop, u4Addr, u4Size;

    u4Loop = MEMTEST_DEFAULT_LOOP;
    u4Addr = MEMTEST_DEFAULT_ADDR;
    u4Size = MEMTEST_DEFAULT_SIZE;

    if (i4Argc<2 || i4Argc>4)
    {
        Printf("run loop [addr = 0x%x] [size = 0x%x]\n", 
            MEMTEST_DEFAULT_ADDR, MEMTEST_DEFAULT_SIZE);

        return 0;
    }

    u4Loop = (UINT32)StrToInt(aszArgv[1]);

    if (i4Argc>2)
    {
        u4Addr = (UINT32)StrToInt(aszArgv[2]);
    }

    if (i4Argc>3)
    {
        u4Size = (UINT32)StrToInt(aszArgv[3]);
    }

    MEM_Test(u4Loop, u4Addr, u4Size);

    return 0;
}
#endif /* #if defined(DRAM_MEMORY_CHECK) */
#endif 

#if defined(CC_MT5890)
static INT32 _DDR_BandWidthMontorTest( INT32 i4Argc, const CHAR ** aszArgv )
{   
    UINT32 u4AgentId = 0x20;  /* 0x20 = all system include Group1, Group2, Group3*/
    UINT32 u4value, u4Val = 0;

    Printf("DRAM Channel %s is selected.\n", (IS_DRAM_CHANNELA_ACTIVE() ? "A": (IS_DRAM_CHANNELB_ACTIVE() ? "B": "C")));
    
    if (i4Argc != 2)
    {
		Printf("bm [Agent ID].\n");
		Printf("Agent ID   : 0 ~ 31.\n");
		Printf("All System : 32 or blank.\n");
        u4AgentId = 0x20; /* 0x20 = all system include Group1, Group2, Group3*/
    }
    else
    {
        u4AgentId = (UINT32)StrToInt(aszArgv[1]);
    }
   //Check 32bit or 16bit
   u4value = ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x004));

   u4AgentId = (u4AgentId > 0x1F) ? 0x20: u4AgentId;

   // Set monitor cycle.
   DRAM_ARB_WRITE32(0x8C, 0x0FFFFFFF);

   if ((u4AgentId <= 15))                           // Group 1.
   {
	   DRAM_ARB_WRITE32(0x80, (u4AgentId << 8));
	   if (mcTEST_BIT(u4value, 0))
	   DRAM_ARB_WRITE32(0x80, 0x08008000 | (u4AgentId << 8));
	   else
	   DRAM_ARB_WRITE32(0x80, 0x00008000 | (u4AgentId << 8));
   }
   else if ((u4AgentId > 15) && (u4AgentId <= 22))	// Group 2, Agent 16 ~ Agent 22.
   {
	   DRAM_ARB_WRITE32(0x80, ((u4AgentId-16) << 16));
	   if (mcTEST_BIT(u4value, 0))
	   DRAM_ARB_WRITE32(0x80, 0x08080000 | ((u4AgentId-16) << 16));
	   else
	   DRAM_ARB_WRITE32(0x80, 0x00080000 | ((u4AgentId-16) << 16));	
   }
   else if ((u4AgentId == 30))                      // Group 2, Agrnt 30.
   {
	   DRAM_ARB_WRITE32(0x80, (7 << 16));
	   if (mcTEST_BIT(u4value, 0))
	   DRAM_ARB_WRITE32(0x80, 0x08080000 | (7 << 16));
	   else
	   DRAM_ARB_WRITE32(0x80, 0x00080000 | (7 << 16));	
   }
   else if ((u4AgentId > 22) && (u4AgentId <= 29))	// Group 3, Agent 23 ~ Agent 29 .
   {
	   DRAM_ARB_WRITE32(0x80, ((u4AgentId-23) << 20));
	   if (mcTEST_BIT(u4value, 0))
	   DRAM_ARB_WRITE32(0x80, 0x08800000 | ((u4AgentId-23) << 20));
	   else
	   DRAM_ARB_WRITE32(0x80, 0x00800000 | ((u4AgentId-23) << 20));
   }
   else if ((u4AgentId == 31))	                    // Group 3, Agent 31 .
   {
	   DRAM_ARB_WRITE32(0x80, (7 << 20));
	   if (mcTEST_BIT(u4value, 0))
	   DRAM_ARB_WRITE32(0x80, 0x08800000 | (7 << 20));
   else
	   DRAM_ARB_WRITE32(0x80, 0x00800000 | (7 << 20));
   }
   else if ((u4AgentId == 32))                     // all system include Group1, Group2, Group3.
   {
	   DRAM_ARB_WRITE32(0x80, (31 << 8));
	   if (mcTEST_BIT(u4value, 0))
	   DRAM_ARB_WRITE32(0x80, 0x08008000 | (31 << 8));
	   else
	   DRAM_ARB_WRITE32(0x80, 0x00008000 | (31 << 8));
   }

   if(u4AgentId == 32)
   	  Printf("Start check All System.....Waiting !\n", u4AgentId);
   else
   Printf("Start check Agent id = 0x%X.....Waiting !\n", u4AgentId);
   
   while (DRAM_ARB_READ32(0x9C) != DRAM_ARB_READ32(0x8C))
   {
	   x_thread_delay(1000);	   
   }
	 
   if ((u4AgentId <= 15) || (u4AgentId == 32))                              // Group 1.
   {
	   u4Val = DRAM_ARB_READ32(0x90);
   }
   else if (((u4AgentId > 15) && (u4AgentId <= 22)) || (u4AgentId == 30))	// Group 2.
   {
	   u4Val = DRAM_ARB_READ32(0x94);
   }
   else if (((u4AgentId > 22) && (u4AgentId <= 29)) || (u4AgentId == 31))	// Group 3.
   {
	   u4Val = DRAM_ARB_READ32(0x98);
   }

   u4Val /= (DRAM_ARB_READ32(0x8C)/1000);

   if(u4AgentId == 32)
      Printf("Result: All System, Bandwidth = %d.%d%%.\n", (u4Val/10), (u4Val%10));
   else
      Printf("Result: Agent ID = 0x%X, Bandwidth = %d.%d%%.\n", u4AgentId, (u4Val/10), (u4Val%10));

    return 0;    
}
#else
static INT32 _DDR_BandWidthMontorTest( INT32 i4Argc, const CHAR ** aszArgv )
{   
    UINT32 u4AgentId = 0x1F;  /* 0x1F = all system */
    UINT32 u4Val = 0;
    UINT32 u4AGVDIV2;

    Printf("DRAM Channel %s is selected.\n", (IS_DRAM_CHANNELB_ACTIVE() ? "B": "A"));
    
    if (i4Argc != 2)
    {
        u4AgentId = 0x1F; /* 0x1F = all system */
    }
    else
    {
        u4AgentId = (UINT32)StrToInt(aszArgv[1]);
    }
#if defined(CC_MT5881)
// Check if AGFZDIV2 is set.
   u4AGVDIV2 = (DRAM_READ32(0x44) & 0x00400000) ? 2 : 1;

   u4AgentId = (u4AgentId > 0x1F) ? 0x1F: u4AgentId;

   // Set monitor cycle.
   DRAM_DMARB_WRITE32(0x8C, 0x0FFFFFFF);

   if ((u4AgentId <= 15) ||(u4AgentId == 31))  // Group 1.
   {
	   DRAM_DMARB_WRITE32(0x80, (u4AgentId << 8));
	   DRAM_DMARB_WRITE32(0x80, 0x8000 | (u4AgentId << 8));
   }
   else if ((u4AgentId > 15) && (u4AgentId <= 22))	// Group 2.
   {
	   DRAM_DMARB_WRITE32(0x80, ((u4AgentId-16) << 16));
	   DRAM_DMARB_WRITE32(0x80, 0x80000 | ((u4AgentId-16) << 16));
   }
   else if ((u4AgentId > 22) && (u4AgentId <= 29))	// Group 3.
   {
	   DRAM_DMARB_WRITE32(0x80, ((u4AgentId-20) << 16));
	   DRAM_DMARB_WRITE32(0x80, 0x800000 | ((u4AgentId-20) << 16));
   }
   else
   {
	   // all system bandwidth.
	   DRAM_DMARB_WRITE32(0x80, (u4AgentId << 8));
	   DRAM_DMARB_WRITE32(0x80, 0x8000 | (u4AgentId << 8));
   }

   Printf("Start check Agent id = 0x%X.....Waiting !\n", u4AgentId);
   
   while (DRAM_DMARB_READ32(0x9C) != DRAM_DMARB_READ32(0x8C))
   {
	   x_thread_delay(1000);	   
   }
	 
   if ((u4AgentId <= 15) ||(u4AgentId == 31))  // Group 1.
   {
	   u4Val = DRAM_DMARB_READ32(0x90);
   }
   else if ((u4AgentId > 15) && (u4AgentId <= 22))	// Group 2.
   {
	   u4Val = DRAM_DMARB_READ32(0x94);
   }
   else if ((u4AgentId > 22) && (u4AgentId <= 29))	// Group 3.
   {
	   u4Val = DRAM_DMARB_READ32(0x98);
   }

   u4Val /= ((DRAM_DMARB_READ32(0x8C)/1000) * u4AGVDIV2);
#elif defined(CC_MT5399)||defined(CC_MT5882)
// Check if AGFZDIV2 is set.
   u4AGVDIV2 = (DRAM_READ32(0x44) & 0x00400000) ? 2 : 1;

   u4AgentId = (u4AgentId > 0x1F) ? 0x1F: u4AgentId;

   // Set monitor cycle.
   DRAM_ARB_WRITE32(0x8C, 0x0FFFFFFF);

   if ((u4AgentId <= 15) ||(u4AgentId == 31))  // Group 1.
   {
	   DRAM_ARB_WRITE32(0x80, (u4AgentId << 8));
	   DRAM_ARB_WRITE32(0x80, 0x08008000 | (u4AgentId << 8));
   }
   else if ((u4AgentId > 15) && (u4AgentId <= 22))	// Group 2.
   {
	   DRAM_ARB_WRITE32(0x80, ((u4AgentId-16) << 16));
	   DRAM_ARB_WRITE32(0x80, 0x08080000 | ((u4AgentId-16) << 16));
   }
   else if ((u4AgentId > 22) && (u4AgentId <= 29))	// Group 3.
   {
	   DRAM_ARB_WRITE32(0x80, ((u4AgentId-20) << 16));
	   DRAM_ARB_WRITE32(0x80, 0x08800000 | ((u4AgentId-20) << 16));
   }
   else
   {
	   // all system bandwidth.
	   DRAM_ARB_WRITE32(0x80, (u4AgentId << 8));
	   DRAM_ARB_WRITE32(0x80, 0x8000 | (u4AgentId << 8));
   }

   Printf("Start check Agent id = 0x%X.....Waiting !\n", u4AgentId);
   
   while (DRAM_ARB_READ32(0x9C) != DRAM_ARB_READ32(0x8C))
   {
	   x_thread_delay(1000);	   
   }
	 
   if ((u4AgentId <= 15) ||(u4AgentId == 31))  // Group 1.
   {
	   u4Val = DRAM_ARB_READ32(0x90);
   }
   else if ((u4AgentId > 15) && (u4AgentId <= 22))	// Group 2.
   {
	   u4Val = DRAM_ARB_READ32(0x94);
   }
   else if ((u4AgentId > 22) && (u4AgentId <= 29))	// Group 3.
   {
	   u4Val = DRAM_ARB_READ32(0x98);
   }

   u4Val /= ((DRAM_ARB_READ32(0x8C)/1000) * u4AGVDIV2);

#else
    // Check if AGFZDIV2 is set.
    u4AGVDIV2 = (DRAM_READ32(0x18) & 0x80) ? 2 : 1;

    u4AgentId = (u4AgentId > 0x1F) ? 0x1F: u4AgentId;

    // Set monitor cycle.
    DRAM_WRITE32(0x18C, 0x0FFFFFFF);

    if ((u4AgentId <= 15) ||(u4AgentId == 31))  // Group 1.
    {
        DRAM_WRITE32(0x180, (u4AgentId << 8));
        DRAM_WRITE32(0x180, 0x8000 | (u4AgentId << 8));
    }
    else if ((u4AgentId > 15) && (u4AgentId <= 22))  // Group 2.
    {
        DRAM_WRITE32(0x180, ((u4AgentId-16) << 16));
        DRAM_WRITE32(0x180, 0x80000 | ((u4AgentId-16) << 16));
    }
    else if ((u4AgentId > 22) && (u4AgentId <= 29))  // Group 3.
    {
        DRAM_WRITE32(0x180, ((u4AgentId-20) << 16));
        DRAM_WRITE32(0x180, 0x800000 | ((u4AgentId-20) << 16));
    }
    else
    {
        // all system bandwidth.
        DRAM_WRITE32(0x180, (u4AgentId << 8));
        DRAM_WRITE32(0x180, 0x8000 | (u4AgentId << 8));
    }

    Printf("Start check Agent id = 0x%X.....Waiting !\n", u4AgentId);
    
    while (DRAM_READ32(0x19C) != DRAM_READ32(0x18C))
    {
        x_thread_delay(1000);       
    }
      
    if ((u4AgentId <= 15) ||(u4AgentId == 31))  // Group 1.
    {
        u4Val = DRAM_READ32(0x190);
    }
    else if ((u4AgentId > 15) && (u4AgentId <= 22))  // Group 2.
    {
        u4Val = DRAM_READ32(0x194);
    }
    else if ((u4AgentId > 22) && (u4AgentId <= 29))  // Group 3.
    {
        u4Val = DRAM_READ32(0x198);
    }

    u4Val /= ((DRAM_READ32(0x18C)/1000) * u4AGVDIV2);
#endif
    Printf("Result: Agent id = 0x%X, bandwidth=%d.%d%%.\n", 
        u4AgentId, (u4Val/10), (u4Val%10));

    return 0;    
}
#endif

static INT32 _DDR_Cmdcm( INT32 i4Argc, const CHAR ** aszArgv )
{
    if (i4Argc < 2)
    {
        Printf("Usage: cm [time in secs] [abort 1:compare fail abort, 0: compare fail not abort].\n");
        Printf("[time=n] n = 0 disable test.\n");
        Printf("[abort=1 or 0] default abort=1.\n");
    }
    else if (i4Argc == 2)
    {
        u4Cmemrw = (UINT32)StrToInt(aszArgv[1]);
        fgCmemrwAbort = TRUE;
    }    
    else if (i4Argc == 3)
    {
        u4Cmemrw = (UINT32)StrToInt(aszArgv[1]);
        fgCmemrwAbort = (BOOL)(StrToInt(aszArgv[2]) > 0);        
    }

    // Check and create DDR thread to do memory test.
    if (u4Cmemrw > 0)
    {
        _DDR_ThreadInit();
    }

    Printf("Current Time = %d secs.\n", u4Cmemrw);
    Printf("Current compare fail abort => %s.\n", ((fgCmemrwAbort) ? "TRUE": "FALSE"));
    return 0;
}

#ifndef CC_MT5881
#if defined(CC_MT5399)||defined(CC_MT5890)||defined(CC_MT5882)
static void _DDR_SetDQDelay(UINT32 u4DQSth, UINT32 u4Bin, UINT32 u4DQInputDelay[])
{
    UINT32 u4Val; 
    UINT32 u4OldBin;
    UINT32 i;
   
    u4OldBin = u4DQInputDelay[(2*u4DQSth)];
    for (i=0; i<4; i++)
    {
		u4Val = (u4OldBin>>(8*i)) & 0x0F;
		mcSET_FIELD(u4OldBin, u4Val+u4Bin, 0x0F<<(8*i), 8*i);
    }
    ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x210 + (8*u4DQSth)), u4OldBin);

    u4OldBin = u4DQInputDelay[(2*u4DQSth)+1];
    for (i=0; i<4; i++)
    {
		u4Val = (u4OldBin>>(8*i)) & 0x0F;
		mcSET_FIELD(u4OldBin, u4Val+u4Bin, 0x0F<<(8*i), 8*i);
    }
    ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x214 + (8*u4DQSth)), u4OldBin);
}

static void _DDR_SetDQSDelay(UINT32 u4DQSth, UINT32 u4Bin)
{
	UINT32 u4Value;
		
	u4Value = ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x18));
	mcSET_FIELD(u4Value, u4Bin, 0x7F << (u4DQSth*8), u4DQSth*8);
	ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(0x18), u4Value);    
}
#else
static void _DDR_SetDQDelay(UINT32 u4DQSth, UINT32 u4Bin, UINT32 u4DQInputDelay[])
{
    UINT32 u4Val; 
    UINT32 u4OldBin;
    UINT32 i;

    /*

        Caution: 
        5365 IS_DDR_SWAP16BIT() do not need to swap DQ input delay mapping.
        But RDQS, WDQS, WDQ need to be swapped.
        
    */
   
    u4OldBin = u4DQInputDelay[(2*u4DQSth)];
    for (i=0; i<4; i++)
    {
        u4Val = (u4OldBin>>(8*i)) & 0xFF;
        u4Val += u4Bin;
        u4OldBin &= ~(0xFF << (8*i));
        u4OldBin |= u4Val << (8*i);
    }
    DRAM_WRITE32(0x3B0 + (8*u4DQSth), u4OldBin);

    u4OldBin = u4DQInputDelay[(2*u4DQSth)+1];
    for (i=0; i<4; i++)
    {
        u4Val = (u4OldBin>>(8*i)) & 0xFF;
        u4Val += u4Bin;
        u4OldBin &= ~(0xFF << (8*i));
        u4OldBin |= u4Val << (8*i);
    }
    DRAM_WRITE32(0x3B4 + (8*u4DQSth), u4OldBin);
}

static void _DDR_SetDQSDelay(UINT32 u4DQSth, UINT32 u4Bin)
{
    UINT32 u4Val;
    UINT32 u4Swap16Bit[4] = {2, 3, 0, 1}; // Mapping when 16 bits DQ swap.
    UINT32 u4HwDQSth;

    // Byte 0 change to byte 2, byte 1 change to byte 3.
    // Byte 2 change to byte 0, byte 3 change to byte 1.
    u4HwDQSth = (IS_DDR_SWAP16BIT()) ? u4Swap16Bit[u4DQSth] : u4DQSth;

    // Setup 7-bit DQS offset
    u4Val = DRAM_READ32(0x3A0 + (4*(u4HwDQSth >> 2)));
    u4Val = (u4Val & (~(0xffU << (8*(u4HwDQSth & 3)))));
    u4Val |= (u4Bin << (8*(u4HwDQSth & 3)));

    // Update manual DQS input delay only while MANUDQSUPD=1 and DLLFRZ.   
    DRAM_WRITE32(0x234, (DRAM_READ32(0x234) | 0x00040000)); 
    
    DRAM_WRITE32(0x3A0+ (4*(u4HwDQSth >> 2)), u4Val);

    // On-line manual DQS input delay adjust enabling.
    DRAM_WRITE32(0x234, (DRAM_READ32(0x234) | 0x00020000)); 

    x_thread_delay(10);

    DRAM_WRITE32(0x234, (DRAM_READ32(0x234) & (~0x00060000)));     
}
#endif//#ifdef CC_MT5399
#endif//#ifndef CC_MT5881

#if defined(CC_MT5399)||defined(CC_MT5882)
static INT32 _DDR_DQSGatingWindowTest( INT32 i4Argc, const CHAR ** aszArgv )
{
    //UINT32 u4OldVal[4];
    UINT32 u4Value;    
    //UINT32 u4SetVal = 0;    
    UINT32 u4Step;
    UINT32 u4IncDelay = 0;
    UINT32 u4TimeGap = 3;
    UINT32 u4SelectByte = 0;
    UINT32 i;
	UINT32 ucdly_coarse_0p5T[4], ucdly_fine_xT[4];
	UINT32 u4PHY_BASE_ADDR,u4PHY_BASE_ADDR1;
	UINT32 ucRX_DLY_DQSIENSTB_LOOP;
	UINT32 ucdly_coarse_0p5T_phy_addr[4], ucdly_fine_xT_phy_addr[4];

    Printf("DRAM Channel %s is selected.\n", (IS_DRAM_CHANNELB_ACTIVE() ? "B": "A"));
    
    if (i4Argc >= 4)
    {
        u4IncDelay = (UINT32)StrToInt(aszArgv[1]);
        u4TimeGap = (UINT32)StrToInt(aszArgv[2]);        
        u4SelectByte = (UINT32)StrToInt(aszArgv[3]);
    }

	if (!IS_DRAM_CHANNELB_ACTIVE())
    {
		u4PHY_BASE_ADDR = mcSET_PHY_REG_ADDR(0x000);
		u4PHY_BASE_ADDR1 = mcSET_PHY_REG_ADDR(0x200);
    }
    else
    {
		u4PHY_BASE_ADDR = mcSET_PHY_REG_ADDR(0x250);
		u4PHY_BASE_ADDR1 = mcSET_PHY_REG_ADDR(0x400);
    }
	
	ucdly_coarse_0p5T_phy_addr[0] = mcSET_PHY_REG_ADDR(u4PHY_BASE_ADDR+0x038);
	ucdly_coarse_0p5T_phy_addr[1] = mcSET_PHY_REG_ADDR(u4PHY_BASE_ADDR+0x03C);
	ucdly_coarse_0p5T_phy_addr[2] = mcSET_PHY_REG_ADDR(u4PHY_BASE_ADDR1+0x038);
	ucdly_coarse_0p5T_phy_addr[3] = mcSET_PHY_REG_ADDR(u4PHY_BASE_ADDR1+0x03C);

	ucdly_fine_xT_phy_addr[0] = mcSET_PHY_REG_ADDR(u4PHY_BASE_ADDR+0x03C);
	ucdly_fine_xT_phy_addr[1] = mcSET_PHY_REG_ADDR(u4PHY_BASE_ADDR+0x040);
	ucdly_fine_xT_phy_addr[2] = mcSET_PHY_REG_ADDR(u4PHY_BASE_ADDR1+0x03C);
	ucdly_fine_xT_phy_addr[3] = mcSET_PHY_REG_ADDR(u4PHY_BASE_ADDR1+0x040);
	
	for(i=0;i<4;i++)
	{
		ucdly_coarse_0p5T[i] =  ucDram_Register_Read(ucdly_coarse_0p5T_phy_addr[i]) & 0xfff;
		ucdly_fine_xT[i] = (ucDram_Register_Read(ucdly_fine_xT_phy_addr[i])>>16) & 0x03f;
	}	
   
	for(i=0;i<4;i++)\
	{
		Printf("current DQS%d Coarse 0.5T = 0x%03x\n", i, ucdly_coarse_0p5T[i]);
	}
    Printf("__________________________________________________\n");
	for(i=0;i<4;i++)
	{
		Printf("current DQS%d fine tune = %d\n", i, ucdly_fine_xT[i]);
	}
    Printf("===============================================================================\n");

    if (i4Argc < 4)
    {
        Printf("g [Inc=1:Dec=0] [Gap in secs] [Selected Byte].\n");
        return 0;
    }

	ucRX_DLY_DQSIENSTB_LOOP = TCMGET_DLYCELL_PERT / 2;
    
    for(u4Step=DQS_GATING_STEP; u4Step<0x60; u4Step=u4Step+DQS_GATING_STEP)
    {
        if (u4IncDelay)
        {
			ucdly_fine_xT[u4SelectByte] += DQS_GATING_STEP;
			if(ucdly_fine_xT[u4SelectByte] > ucRX_DLY_DQSIENSTB_LOOP)
			{
				ucdly_coarse_0p5T[u4SelectByte] =  ucdly_coarse_0p5T[u4SelectByte]<<1;
				ucdly_fine_xT[u4SelectByte] = ucdly_fine_xT[u4SelectByte]-ucRX_DLY_DQSIENSTB_LOOP-1;		
			}

			Printf("\nByte%d: Step=+%d, current Coarse 0.5T = 0x%03x, fine tune = %d.\n", 
                    u4SelectByte, u4Step, ucdly_coarse_0p5T[u4SelectByte], ucdly_fine_xT[u4SelectByte]);

			u4Value = ucDram_Register_Read(ucdly_coarse_0p5T_phy_addr[u4SelectByte]);
			mcSET_FIELD(u4Value, ucdly_coarse_0p5T[u4SelectByte], 0xfff, 0);
			ucDram_Register_Write(ucdly_coarse_0p5T_phy_addr[u4SelectByte], u4Value);
			
			u4Value = ucDram_Register_Read(ucdly_fine_xT_phy_addr[u4SelectByte]);
			mcSET_FIELD(u4Value, ucdly_fine_xT[u4SelectByte], 0x003f0000, 16);
			ucDram_Register_Write(ucdly_fine_xT_phy_addr[u4SelectByte], u4Value);
        }
        else
        {
			if(ucdly_fine_xT[u4SelectByte] < DQS_GATING_STEP)
			{
				ucdly_coarse_0p5T[u4SelectByte] =  ucdly_coarse_0p5T[u4SelectByte]>>1;
				ucdly_fine_xT[u4SelectByte] = ucdly_fine_xT[u4SelectByte]+ucRX_DLY_DQSIENSTB_LOOP-DQS_GATING_STEP+1;		
			}
			else
			{
				ucdly_fine_xT[u4SelectByte] -= DQS_GATING_STEP;
			}

			Printf("\nByte%d: Step=-%d, current Coarse 0.5T = 0x%03x, fine tune = %d.\n", 
                    u4SelectByte, u4Step, ucdly_coarse_0p5T[u4SelectByte], ucdly_fine_xT[u4SelectByte]);

			u4Value = ucDram_Register_Read(ucdly_coarse_0p5T_phy_addr[u4SelectByte]);
			mcSET_FIELD(u4Value, ucdly_coarse_0p5T[u4SelectByte], 0xfff, 0);
			ucDram_Register_Write(ucdly_coarse_0p5T_phy_addr[u4SelectByte], u4Value);
			
			u4Value = ucDram_Register_Read(ucdly_fine_xT_phy_addr[u4SelectByte]);
			mcSET_FIELD(u4Value, ucdly_fine_xT[u4SelectByte], 0x003f0000, 16);
			ucDram_Register_Write(ucdly_fine_xT_phy_addr[u4SelectByte], u4Value);
        }

        Printf("Wait %d secs...\n", u4TimeGap);
        for (i=0; i<u4TimeGap*10; i++)
        {
            Printf("=");
            x_thread_delay(100);
        }
        Printf("End.\n");
    }

    return 0;    
}

#else
#if !defined(CC_MT5890)
static INT32 _DDR_DQSGatingWindowTest( INT32 i4Argc, const CHAR ** aszArgv )
{
    UINT32 u4OldVal[4];
    UINT32 u4Val;    
    UINT32 u4SetVal = 0;    
    UINT32 u4Step;
    UINT32 u4IncDelay = 0;
    UINT32 u4TimeGap = 3;
    UINT32 u4SelectByte = 0;
    UINT32 u4Swap16Bit[4] = {2, 3, 0, 1}; // Mapping when 16 bits DQ swap.
    UINT32 u4HwDQSth = 0;
    UINT32 i;

    Printf("DRAM Channel %s is selected.\n", (IS_DRAM_CHANNELB_ACTIVE() ? "B": "A"));
    
    if (i4Argc >= 4)
    {
        u4IncDelay = (UINT32)StrToInt(aszArgv[1]);
        u4TimeGap = (UINT32)StrToInt(aszArgv[2]);        
        u4SelectByte = (UINT32)StrToInt(aszArgv[3]);
        u4HwDQSth = (IS_DDR_SWAP16BIT()) ? u4Swap16Bit[u4SelectByte] : u4SelectByte; 
    }

    u4Val = DRAM_READ32(0x388);   
    Printf("Current DQSCTL=0x%X, 0x7388=0x%X.\n", (DRAM_READ32(0x380) & 0xFFF), u4Val);   

    for (i=0; i<4; i++)
    {       
        if (IS_DDR_SWAP16BIT())
        {
            u4OldVal[i] = ((u4Val >> 8*u4Swap16Bit[i]) & 0x7F);
        }
        else
        {
            u4OldVal[i] = ((u4Val >> 8*i) & 0x7F);
        }
        Printf("DQS%d: Gating=%d (Binary Coded).\n", i, u4OldVal[i]);
    }

    if (i4Argc < 4)
    {
        Printf("g [Inc=1:Dec=0] [Gap in secs] [Selected Byte].\n");
        return 0;
    }
    
    for(u4Step=0; u4Step<0x60; u4Step=u4Step+3)
    {
        if (u4IncDelay)
        {
            u4Val = (u4OldVal[u4SelectByte] + u4Step) & 0x7F;
            u4SetVal = DRAM_READ32(0x388);
            u4SetVal &= ~ (0x7F << (8*u4HwDQSth));
            u4SetVal |= (u4Val << (8*u4HwDQSth));
            Printf("\nByte%d: Step=+%d, Value=%d, 0x388=0x%X.\n", 
                u4SelectByte, u4Step, u4Val, u4SetVal);                
            DRAM_WRITE32(0x388, u4SetVal); // Need to do early	
        }
        else
        {
            if (u4OldVal[u4SelectByte] > u4Step)
            {
                u4Val = (u4OldVal[u4SelectByte] - u4Step) & 0x7F;
                u4SetVal = DRAM_READ32(0x388);
                u4SetVal &= ~ (0x7F << (8*u4HwDQSth));
                u4SetVal |= (u4Val << (8*u4HwDQSth));
                Printf("\nByte%d: Step=-%d, Value=%d, 0x388=0x%X.\n", 
                    u4SelectByte, u4Step, u4Val, u4SetVal);                                    
                DRAM_WRITE32(0x388, u4SetVal); // Need to do early	
            }
            else
            {
                return 0;
            }
        }

        Printf("Wait %d secs...\n", u4TimeGap);
        for (i=0; i<u4TimeGap*10; i++)
        {
            Printf("=");
            x_thread_delay(100);
        }
        Printf("End.\n");
    }

    return 0;    
}
#endif //#if !defined(CC_MT5890)
#endif //#if defined(CC_MT5399)
static INT32 _DDR_DQSDelayTest( INT32 i4Argc, const CHAR ** aszArgv )
{
#if (defined(CC_MT5368) || defined(CC_MT5396)) || defined(CC_MT5389) || defined(CC_MT5398) || defined(CC_MT5880)
    UINT32 u4Val;    
    UINT32 i;    
    UINT32 u4Rec;
    UINT32 u4Bytes = 4;
    UINT32 u4SelectByte = 0;
    INT32 i4Avg[8]={0};
    INT32 i4Min;
    INT32 i4Max;
    INT32 i4Size;
    UINT32 u4Step;
    UINT32 u4IncDelay = 0;
    UINT32 u4TimeGap = 20;
    UINT32 u4DQInputDelay[8];
#if defined(CC_MT5881) 
	UINT32 u4ValDQS,u4ValDQ;
	UINT32 u4Swap16Bit[2] = {1, 0};  // Mapping when 16 bits DQ swap.
#else
    UINT32 u4Swap16Bit[4] = {2, 3, 0, 1}; // Mapping when 16 bits DQ swap.
#endif
    UINT32 u4HwDQSth;
    UINT32 u4RecByte;
    #ifdef HW_rDQS_OVER_SAMPLE
        UINT32 u4OverSampleErr1,u4OverSampleErr2;
    #endif

    Printf("DRAM Channel %s is selected.\n", (IS_DRAM_CHANNELB_ACTIVE() ? "B": "A"));
#if defined(CC_MT5881)  
	u4Bytes = 2;
	for (i=0; i<4; i++)
    {
        // Byte 0 change to byte 1, byte 1 change to byte 0.
        u4DQInputDelay[i] =  DRAM_READ32(0x210 + 4*i);
    }  
	Printf("DRAM Byte Number = %d, Loader calibration infor:\n", u4Bytes);
	for (i=0; i<u4Bytes; i++)
    {
    	u4HwDQSth = (IS_DDR_SWAP16BIT()) ? u4Swap16Bit[i] : i;
		u4RecByte = (IS_DRAM_CHANNELB_ACTIVE()) ? (i+4) : (i);
		i4Avg[i] = (DRAM_READ32(0x18) >> (u4HwDQSth*8)) & 0xFF;
        u4Rec = HAL_READ32(RECORD_DQS_REG + ((u4RecByte /2)*4));
        u4Val = ((u4Rec >> ((u4RecByte & 0x1)*16)) & 0xFF);
        i4Min = ((UINT32)i4Avg[i] >= u4Val) ? (i4Avg[i] - u4Val) : -(u4Val - i4Avg[i]);
        i4Max = i4Avg[i] + ((u4Rec >> (((u4RecByte & 0x1)*16) + 8)) & 0xFF);
        i4Size = i4Max- i4Min + 1;   
		Printf("Byte %d : DQS(%d ~ %d), Size %d, Middle = %d.\n", i, i4Min, i4Max, i4Size, i4Avg[i]);
	}
	if (i4Argc < 3)
    {
        Printf("dqs [Byte] [Inc=1:Dec=0] [Gap in secs].\n");
        return 0;
    }
    else
    {
        u4SelectByte = (UINT32)StrToInt(aszArgv[1]);        
        u4IncDelay = (UINT32)StrToInt(aszArgv[2]);
        u4TimeGap = (UINT32)StrToInt(aszArgv[3]); 
    }
	
	u4HwDQSth = (IS_DDR_SWAP16BIT()) ? u4Swap16Bit[u4SelectByte] : u4SelectByte;
	for(u4Step=1; u4Step < 32; u4Step++)
	{
		if (u4IncDelay)
		{			
			Printf("\nStep: +%d\n", u4Step);
			u4Val = i4Avg[u4SelectByte]+u4Step;

			Printf("Byte %d, DQS = %d.\n", u4SelectByte, u4Val);
			u4ValDQS = DRAM_READ32(0x18);
			u4ValDQS = (u4ValDQS & (~(0xffU << (8*(u4HwDQSth & 3)))));
			u4ValDQS |= (u4Val << (8*(u4HwDQSth & 3)));
			DRAM_WRITE32(0x18, u4ValDQS);
			//_DDR_SetDQSDelay(u4SelectByte, u4Val);
		}
		else
		{			
			Printf("\nStep: -%d\n", u4Step);

			if (i4Avg[u4SelectByte] >= u4Step)
			{
				u4Val = i4Avg[u4SelectByte]-u4Step;
				Printf("Byte %d, DQS = %d.\n", u4SelectByte, u4Val);
				u4ValDQS = DRAM_READ32(0x18);
				u4ValDQS = (u4ValDQS & (~(0xffU << (8*(u4HwDQSth & 3)))));
				u4ValDQS |= (u4Val << (8*(u4HwDQSth & 3)));
				DRAM_WRITE32(0x18, u4ValDQS);
				//_DDR_SetDQSDelay(u4SelectByte, u4Val);
			}
			else
			{
				u4Val = u4Step - i4Avg[u4SelectByte];

				Printf("Byte %d, DQS = -%d.\n", u4SelectByte, u4Val);

				// Set DQS = 0, DQ start to increase.
				DRAM_WRITE32(0x18, DRAM_READ32(0x18) & ~(0xFF<< (8*(u4HwDQSth & 3))));
				u4ValDQ = (u4Val<<24) | (u4Val<<16) | (u4Val<<8) | u4Val;
				DRAM_WRITE32(0x210+(8*u4SelectByte), u4DQInputDelay[(2*u4SelectByte)] + u4ValDQ);
				DRAM_WRITE32(0x214+(8*u4SelectByte), u4DQInputDelay[(2*u4SelectByte)+1] + u4ValDQ);
				//_DDR_SetDQSDelay(u4SelectByte, 0);
				//_DDR_SetDQDelay(u4SelectByte, u4Val, u4DQInputDelay);
			}			 
		}
		Printf("Wait %d secs...\n", u4TimeGap);
	    for (i=0; i<u4TimeGap*10; i++)
	    {
	        Printf("=");
			x_thread_delay(100);

	    }
	    Printf("End.\n");
	}
	
#else

	if (IS_FORCE32()) 
	{
	    u4Bytes = 2;
	    // Keep DQ input delay.
	    for (i=0; i<4; i++)
	    {
	        // Byte 0 change to byte 2, byte 1 change to byte 3.
	        // Byte 2 change to byte 0, byte 3 change to byte 1.
	        u4DQInputDelay[i] = (IS_DDR_SWAP16BIT()) ? 
	            DRAM_READ32(0x3C0 + 4*i) : DRAM_READ32(0x3B0 + 4*i);
	    }                
	}
	else
	{
	    u4Bytes = 4;
	    // Keep DQ input delay.
	    for (i=0; i<8; i++)
	    {
	        u4DQInputDelay[i] = DRAM_READ32(0x3B0 + 4*i);
	    }
	}

	Printf("DRAM Byte Number = %d, Loader calibration infor:\n", u4Bytes);

	for (i=0; i<u4Bytes; i++)
	{
	    // Byte 0 change to byte 2, byte 1 change to byte 3.
	    // Byte 2 change to byte 0, byte 3 change to byte 1.
	    u4HwDQSth = (IS_DDR_SWAP16BIT()) ? u4Swap16Bit[i] : i;

	    u4RecByte = (IS_DRAM_CHANNELB_ACTIVE()) ? (i+4) : (i);

	    i4Avg[i] = (DRAM_READ32(0x3A0) >> (u4HwDQSth*8)) & 0xFF;
	    u4Rec = HAL_READ32(RECORD_DQS_REG + ((u4RecByte /2)*4));
	    u4Val = ((u4Rec >> ((u4RecByte & 0x1)*16)) & 0xFF);
	    i4Min = ((UINT32)i4Avg[i] >= u4Val) ? (i4Avg[i] - u4Val) : -(u4Val - i4Avg[i]);
	    i4Max = i4Avg[i] + ((u4Rec >> (((u4RecByte & 0x1)*16) + 8)) & 0xFF);
	    i4Size = i4Max- i4Min + 1;    
	    
	    Printf("Byte %d : DQS(%d ~ %d), Size %d, Middle = %d.\n", i, i4Min, i4Max, i4Size, i4Avg[i]);
	}

	if (i4Argc < 3)
	{
	    Printf("dqs [Byte] [Inc=1:Dec=0] [Gap in secs].\n");
	    return 0;
	}
	else
	{
	    u4SelectByte = (UINT32)StrToInt(aszArgv[1]);        
	    u4IncDelay = (UINT32)StrToInt(aszArgv[2]);
	    u4TimeGap = (UINT32)StrToInt(aszArgv[3]); 

#ifdef DRAM_HWONLINECALDQS
	    // Wait h/w calibration stop.
	    while ((u4HwCalDQSEnable) && (u4HWCaliState != HWCALI_START))
	    {
	        x_thread_delay(1000);
	    }
	    // Suspend h/w calibration.
	    u4HwCalDQSEnable = 0;
	    _DDR_DQSCalDisable();
#endif        
	}

	for(u4Step=1; u4Step < 32; u4Step++)
	{
	    if (u4IncDelay)
	    {
	        #ifdef HW_rDQS_OVER_SAMPLE
	            DRAM_WRITE32(0x22C,0x0F00 );
	            DRAM_WRITE32(0x22C,0x1F00 );
	        #endif
	        
	        Printf("\nStep: +%d\n", u4Step);
	        u4Val = i4Avg[u4SelectByte]+u4Step;

	        Printf("Byte %d, DQS = %d.\n", u4SelectByte, u4Val);

	        _DDR_SetDQSDelay(u4SelectByte, u4Val);
	    }
	    else
	    {
	        #ifdef HW_rDQS_OVER_SAMPLE
	            DRAM_WRITE32(0x22C,0x0700 );
	            DRAM_WRITE32(0x22C,0x1700 );
	        #endif
	        
	        Printf("\nStep: -%d\n", u4Step);

	        if (i4Avg[u4SelectByte] >= u4Step)
	        {
	            u4Val = i4Avg[u4SelectByte]-u4Step;

	            Printf("Byte %d, DQS = %d.\n", u4SelectByte, u4Val);

	            _DDR_SetDQSDelay(u4SelectByte, u4Val);
	        }
	        else
	        {
	            u4Val = u4Step - i4Avg[u4SelectByte];

	            Printf("Byte %d, DQS = -%d.\n", u4SelectByte, u4Val);

	            // Set DQS = 0, DQ start to increase.
	            _DDR_SetDQSDelay(u4SelectByte, 0);
	            _DDR_SetDQDelay(u4SelectByte, u4Val, u4DQInputDelay);
	        }            
	    }        

	    Printf("Wait %d secs...\n", u4TimeGap);
	    for (i=0; i<u4TimeGap*10; i++)
	    {
	        Printf("=");
#if defined(DRAM_MEMORY_AGENTTEST)
			_DDR_TestAgentMemTest();
#endif //#if defined(DRAM_MEMORY_AGENTTEST)
				x_thread_delay(100);

	    }
	    Printf("End.\n");

	    #ifdef HW_rDQS_OVER_SAMPLE
	        u4OverSampleErr1=DRAM_READ32(0x1A4);
	        u4OverSampleErr2=DRAM_READ32(0x1A8);
	        if((u4OverSampleErr1!=0)||(u4OverSampleErr2!=0))
	            Printf("[DRAM]!!!!!!Over Sample Error  0x1A4 = 0x%08x ,0x1A8 = 0x%08x  !!!!!!\n", u4OverSampleErr1,u4OverSampleErr2);
	    #endif
	    
	}

#ifdef DRAM_HWONLINECALDQS
// Resume h/w calibration.
u4HwCalDQSEnable = 1;
u4HWCaliState = HWCALI_START;
#endif
#endif

#else
#if (defined(CC_MT5399) || defined(CC_MT5890))||defined(CC_MT5882)
	UINT32 u4Val;    
    UINT32 i;    
    //UINT32 u4Rec;
    UINT32 u4Bytes = (IS_FORCE32()) ? 2 : 4;
    UINT32 u4SelectByte = 0;
    INT32 i4Avg[8];
    //INT32 i4Min;
    //INT32 i4Max;
    //INT32 i4Size;
    UINT32 u4Step;
    UINT32 u4IncDelay = 0;
    UINT32 u4TimeGap = 20;
    UINT32 u4DQInputDelay[8];
    //UINT32 u4HwDQSth;
    //UINT32 u4RecByte;
    
    #if defined(CC_MT5399)||defined(CC_MT5882)
    Printf("DRAM Channel %s is selected.\n", (IS_DRAM_CHANNELB_ACTIVE() ? "B": "A"));
    #else
	Printf("DRAM Channel %s is selected.\n", (IS_DRAM_CHANNELA_ACTIVE() ? "A": (IS_DRAM_CHANNELB_ACTIVE() ? "B": "C")));
    #endif
	
	for (i=0; i<(u4Bytes*2); i++)
    {
        u4DQInputDelay[i] =  ucDram_Register_Read(mcSET_DRAMC_REG_ADDR((0x210 + 4*i)));
    }  
	Printf("DRAM Byte Number = %d.\n", u4Bytes);	

	for (i=0; i<u4Bytes; i++)
	{
	    //u4RecByte = (IS_DRAM_CHANNELB_ACTIVE()) ? (i+4) : (i);

	    i4Avg[i] = (ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x18)) >> (i*8)) & 0x7F;
	    //u4Rec = HAL_READ32(RECORD_DQS_REG + ((u4RecByte /2)*4));
	    //u4Val = ((u4Rec >> ((u4RecByte & 0x1)*16)) & 0xFF);
	    //i4Min = ((UINT32)i4Avg[i] >= u4Val) ? (i4Avg[i] - u4Val) : -(u4Val - i4Avg[i]);
	    //i4Max = i4Avg[i] + ((u4Rec >> (((u4RecByte & 0x1)*16) + 8)) & 0xFF);
	    //i4Size = i4Max- i4Min + 1;    
	    
	    //Printf("Byte %d : DQS(%d ~ %d), Size %d, Middle = %d.\n", i, i4Min, i4Max, i4Size, i4Avg[i]);
	}


	if (i4Argc < 3)
	{
	    Printf("dqs [Byte] [Inc=1:Dec=0] [Gap in secs].\n");
	    return 0;
	}
	else
	{
	    u4SelectByte = (UINT32)StrToInt(aszArgv[1]);        
	    u4IncDelay = (UINT32)StrToInt(aszArgv[2]);
	    u4TimeGap = (UINT32)StrToInt(aszArgv[3]);         
	}

	for(u4Step=1; u4Step < 32; u4Step++)
	{
	    if (u4IncDelay)
	    {        
	        Printf("\nStep: +%d\n", u4Step);
	        u4Val = i4Avg[u4SelectByte]+u4Step;

	        Printf("Byte %d, DQS = %d.\n", u4SelectByte, u4Val);
		
			_DDR_SetDQSDelay(u4SelectByte, u4Val);
	    }
	    else
	    {	        
	        Printf("\nStep: -%d\n", u4Step);

	        if (i4Avg[u4SelectByte] >= u4Step)
	        {
	            u4Val = i4Avg[u4SelectByte]-u4Step;

	            Printf("Byte %d, DQS = %d.\n", u4SelectByte, u4Val);

				_DDR_SetDQSDelay(u4SelectByte, u4Val);
	        }
	        else
	        {
	            u4Val = u4Step - i4Avg[u4SelectByte];

	            Printf("Byte %d, DQS = -%d.\n", u4SelectByte, u4Val);

	            // Set DQS = 0, DQ start to increase.
				_DDR_SetDQSDelay(u4SelectByte, 0);
	            _DDR_SetDQDelay(u4SelectByte, u4Val, u4DQInputDelay);
	        }            
	    }        

	    Printf("Wait %d secs...\n", u4TimeGap);
	    for (i=0; i<u4TimeGap*10; i++)
	    {
	        Printf("=");
#if defined(DRAM_MEMORY_AGENTTEST)
			_DDR_TestAgentMemTest();
#endif //#if defined(DRAM_MEMORY_AGENTTEST)
			x_thread_delay(100);
	    }
	    Printf("End.\n");	    
	}
	
#endif //(defined(CC_MT5399) || defined(CC_MT5890))
#endif//(defined(CC_MT5368) || defined(CC_MT5396)) || defined(CC_MT5389) || defined(CC_MT5398) || defined(CC_MT5880)
return 0;    
}
    
static INT32 _DDR_DQSOutputDelayTest( INT32 i4Argc, const CHAR ** aszArgv )
{
#if (defined(CC_MT5368) || defined(CC_MT5396)) || defined(CC_MT5389)
    UINT32 i;
    UINT32 tmp;
    UINT32 u4SelectByte = 0;
    UINT32 u4DQSori;
    UINT32 u4DQori[4];
    UINT32 u4DQSnew;
    UINT32 u4delay;
    UINT32 u4Step;
    UINT32 u4IncDelay = 0;
    UINT32 u4TimeGap = 20;
    UINT32 u4Bytes;
    UINT32 u4Swap16Bit[4] = {2, 3, 0, 1}; // Mapping when 16 bits DQ swap.
    UINT32 u4HwDQSth;

    Printf("DRAM Channel %s is selected.\n", (IS_DRAM_CHANNELB_ACTIVE() ? "B": "A"));
      
    if (i4Argc < 3)
    {
        Printf("dqso [Byte] [Inc=1:Dec=0] [Gap in secs].\n");
        return 0;
    }
    else
    {
        u4SelectByte = (UINT32)StrToInt(aszArgv[1]);        
        u4IncDelay = (UINT32)StrToInt(aszArgv[2]);
        u4TimeGap = (UINT32)StrToInt(aszArgv[3]);        
        u4HwDQSth = (IS_DDR_SWAP16BIT()) ? u4Swap16Bit[u4SelectByte] : u4SelectByte;         
    }

    u4Bytes = (IS_FORCE32()) ? 2 : 4;

    u4DQSori = DRAM_DDRPHY_READ32(0x354);
    for(i=0; i<u4Bytes; i++)
    {
        tmp = (IS_DDR_SWAP16BIT()) ? u4Swap16Bit[i] : i; 
        u4DQori[i] = DRAM_DDRPHY_READ32(0x360 + tmp*4);
    }

    for(u4Step=1; u4Step < 32; u4Step++)
    {
        if (u4IncDelay)
        {
            Printf("\nStep: +%d\n", u4Step);
        }
        else
        {
            Printf("\nStep: -%d\n", u4Step);
        }        

        if(u4IncDelay)
        {
            tmp = 1;
            for(i=0; i<32; i+=4)    //scan bit
            {                
                if( (0x000f & (DRAM_DDRPHY_READ32(0x360 + (u4HwDQSth*4))>>i)) == 0 )
                {
                    tmp = 0;
                }
            }

            if(tmp==0)      //+DQS
            {
                u4delay = (DRAM_DDRPHY_READ32(0x354)>>(u4HwDQSth*4)) & 0x0000000f;
                if(u4delay==0xf)
                {
                    break;
                }
                else
                {
                    u4delay++;
                }

                u4DQSnew = (DRAM_DDRPHY_READ32(0x354) & ~(0x0000000f << u4HwDQSth*4)) | (u4delay << u4HwDQSth*4) ;
                Printf("DQSdelay(0x58B54)=0x%08X.\n", u4DQSnew);
                DRAM_DDRPHY_WRITE32(0x354, u4DQSnew);                
            }
            else            // -DQ
            {
                u4DQSnew = 0;
                for(i=0; i<32; i+=4)
                {
                    u4delay = 0x000f & (DRAM_DDRPHY_READ32(0x360 + u4HwDQSth*4)>>i) ;
                    u4delay--;
                    u4DQSnew = u4DQSnew | (u4delay<<i);                    
                }

                Printf("DQdelay(0x%05X)=0x%08X.\n", 0x58B60+u4HwDQSth*4, u4DQSnew);
                DRAM_DDRPHY_WRITE32(0x360+u4HwDQSth*4, u4DQSnew);              
            }

        }
        else
        {
            if( 0x000f & (DRAM_DDRPHY_READ32(0x354)>>(u4HwDQSth*4))) 
            {
                tmp = 1;
            }
            else
            {
                tmp = 0;
            }

            if(tmp==1)      //-DQS
            {
                u4delay = (DRAM_DDRPHY_READ32(0x354)>>(u4HwDQSth*4)) & 0x0000000f;
                u4delay--;

                u4DQSnew = (DRAM_DDRPHY_READ32(0x354) & ~(0x0000000f << u4HwDQSth*4)) | (u4delay << u4HwDQSth*4) ;
                Printf("DQSdelay(0x58B54)=0x%08X.\n", u4DQSnew);
                DRAM_DDRPHY_WRITE32(0x354, u4DQSnew);                
            }
            else            // +DQ
            {
                u4DQSnew = 0;
                for(i=0; i<32; i+=4)
                {
                    u4delay = 0x000f & (DRAM_DDRPHY_READ32(0x360 + u4HwDQSth*4)>>i) ;
                    if(u4delay != 0x0f)
                    {
                        u4delay++;
                    }
                    
                    u4DQSnew = u4DQSnew | (u4delay<<i);
                }

                Printf("DQdelay(0x%05X)=0x%08X.\n", (0x58B60+u4HwDQSth*4), u4DQSnew);
                DRAM_DDRPHY_WRITE32(0x360+u4HwDQSth*4, u4DQSnew);
            }

        }

        Printf("Wait %d secs...\n", u4TimeGap);
        x_thread_delay(u4TimeGap*1000);
    }

    Printf("restore...\n");
    DRAM_DDRPHY_WRITE32(0x354, u4DQSori);
    for (i=0; i<u4Bytes; i++)
    {
        DRAM_DDRPHY_WRITE32(0x360 + i*4, u4DQori[i]);
    }


#else //if defined(CC_MT5368) || defined(CC_MT5396)) || defined(CC_MT5389)

#if defined(CC_MT5398) || defined(CC_MT5880)
    UINT32 i;
    UINT32 tmp;
    UINT32 u4SelectByte = 0;
    UINT32 u4DQSori;
    UINT32 u4DQori[8];
    UINT32 u4DQSnew;
	UINT32 u4DQnew0,u4DQnew1;
    UINT32 u4delay;
    UINT32 u4Step;
    UINT32 u4IncDelay = 0;
    UINT32 u4TimeGap = 20;
    UINT32 u4Bytes;
    UINT32 u4Swap16Bit[4] = {2, 3, 0, 1}; // Mapping when 16 bits DQ swap.
    UINT32 u4HwDQSth;

    Printf("DRAM Channel %s is selected.\n", (IS_DRAM_CHANNELB_ACTIVE() ? "B": "A"));
      
    if (i4Argc < 3)
    {
        Printf("dqso [Byte] [Inc=1:Dec=0] [Gap in secs].\n");
        return 0;
    }
    else
    {
        u4SelectByte = (UINT32)StrToInt(aszArgv[1]);        
        u4IncDelay = (UINT32)StrToInt(aszArgv[2]);
        u4TimeGap = (UINT32)StrToInt(aszArgv[3]);        
        u4HwDQSth = (IS_DDR_SWAP16BIT()) ? u4Swap16Bit[u4SelectByte] : u4SelectByte;         
    }

    u4Bytes = (IS_FORCE32()) ? 2 : 4;

    u4DQSori = DRAM_DDRPHY_READ32(0x354);
    for(i=0; i<u4Bytes; i++)
    {
        tmp = (IS_DDR_SWAP16BIT()) ? u4Swap16Bit[i] : i; 
        u4DQori[i*2]   = DRAM_DDRPHY_READ32(0x360 + tmp*8);
		u4DQori[i*2+1] = DRAM_DDRPHY_READ32(0x360 + tmp*8 + 4);
    }

    for(u4Step=1; u4Step < 32; u4Step++)
    {
        if (u4IncDelay)
        {
            Printf("\nStep: +%d\n", u4Step);
        }
        else
        {
            Printf("\nStep: -%d\n", u4Step);
        }        

        if(u4IncDelay)
        {
            tmp = 1;
            for(i=0; i<32; i+=8)    //scan bit
            {                
                if( (0x1f & (DRAM_DDRPHY_READ32(0x360 + (u4HwDQSth*8))>>i)) == 0 )
                {
                    tmp = 0;
					break;
                }
				if( (0x1f & (DRAM_DDRPHY_READ32(0x360 + (u4HwDQSth*8) + 4)>>i)) == 0 )
                {
                    tmp = 0;
					break;
                }
            }

            if(tmp==0)      //+DQS
            {
                u4delay = (DRAM_DDRPHY_READ32(0x354)>>(u4HwDQSth*8)) & 0x0000001f;
                if(u4delay==0x1f)
                {
                    break;
                }
                else
                {
                    u4delay++;
                }

                u4DQSnew = (DRAM_DDRPHY_READ32(0x354) & ~(0x0000001f << u4HwDQSth*8)) | (u4delay << u4HwDQSth*8) ;
                Printf("DQSdelay(0x58B54)=0x%08X.\n", u4DQSnew);
                DRAM_DDRPHY_WRITE32(0x354, u4DQSnew);                
            }
            else            // -DQ
            {
                u4DQnew0 = 0;
				u4DQnew1 = 0;
                for(i=0; i<32; i+=8)
                {
                    u4delay = 0x1f & (DRAM_DDRPHY_READ32(0x360 + u4HwDQSth*8)>>i) ;
                    u4delay--;
                    u4DQnew0 = u4DQnew0 | (u4delay<<i);  
					u4delay = 0x1f & (DRAM_DDRPHY_READ32(0x360 + u4HwDQSth*8 + 4)>>i) ;
                    u4delay--;
                    u4DQnew1 = u4DQnew1 | (u4delay<<i);
                }

                Printf("DQdelay(0x%05X)=0x%08X.\n", 0x58B60+u4HwDQSth*8, u4DQnew0);
				Printf("DQdelay(0x%05X)=0x%08X.\n", 0x58B60+u4HwDQSth*8 + 4, u4DQnew1);
                DRAM_DDRPHY_WRITE32(0x360+u4HwDQSth*8, u4DQnew0); 
				DRAM_DDRPHY_WRITE32(0x360+u4HwDQSth*8 + 4, u4DQnew1);  
            }

        }
        else
        {
            if( 0x0001f & (DRAM_DDRPHY_READ32(0x354)>>(u4HwDQSth*8))) 
            {
                tmp = 1;
            }
            else
            {
                tmp = 0;
            }

            if(tmp==1)      //-DQS
            {
                u4delay = (DRAM_DDRPHY_READ32(0x354)>>(u4HwDQSth*8)) & 0x0000001f;
                u4delay--;

                u4DQSnew = (DRAM_DDRPHY_READ32(0x354) & ~(0x0000001f << u4HwDQSth*8)) | (u4delay << u4HwDQSth*8) ;
                Printf("DQSdelay(0x58B54)=0x%08X.\n", u4DQSnew);
                DRAM_DDRPHY_WRITE32(0x354, u4DQSnew);                
            }
            else            // +DQ
            {
                u4DQnew0 = 0;
				u4DQnew1 = 0;
                for(i=0; i<32; i+=8)
                {
                    u4delay = 0x1f & (DRAM_DDRPHY_READ32(0x360 + u4HwDQSth*8)>>i) ;
                    if(u4delay != 0x1f)
                    {
                        u4delay++;
                    } 
                    u4DQnew0 = u4DQnew0 | (u4delay<<i);
					
					u4delay = 0x1f & (DRAM_DDRPHY_READ32(0x360 + u4HwDQSth*8 + 4)>>i) ;
                    if(u4delay != 0x1f)
                    {
                        u4delay++;
                    } 
                    u4DQnew1 = u4DQnew1 | (u4delay<<i);
                }

                Printf("DQdelay(0x%05X)=0x%08X.\n", (0x58B60+u4HwDQSth*8), u4DQnew0);
				Printf("DQdelay(0x%05X)=0x%08X.\n", (0x58B60+u4HwDQSth*8 + 4), u4DQnew1);
                DRAM_DDRPHY_WRITE32(0x360+u4HwDQSth*8, u4DQnew0);
				DRAM_DDRPHY_WRITE32(0x360+u4HwDQSth*8 + 4, u4DQnew1);
            }

        }
#if defined(DRAM_MEMORY_AGENTTEST)
			_DDR_TestAgentMemTest();
#endif //#if defined(DRAM_MEMORY_AGENTTEST)
        Printf("Wait %d secs...\n", u4TimeGap);
        x_thread_delay(u4TimeGap*1000);
    }

    Printf("restore...\n");
    DRAM_DDRPHY_WRITE32(0x354, u4DQSori);
    for (i=0; i<u4Bytes; i++)
    {
        DRAM_DDRPHY_WRITE32(0x360 + i*8, u4DQori[i*2]);
		DRAM_DDRPHY_WRITE32(0x360 + i*8 + 4, u4DQori[i*2 + 1]);
    }
	
#else
#if defined(CC_MT5881)
	UINT32 i;
    UINT32 tmp;
    UINT32 u4SelectByte = 0;
    UINT32 u4DQSori;
    UINT32 u4DQori[4];
    UINT32 u4DQSnew;
	UINT32 u4DQnew0,u4DQnew1;
    UINT32 u4delay;
    UINT32 u4Step;
    UINT32 u4IncDelay = 0;
    UINT32 u4TimeGap = 20;
    UINT32 u4Swap16Bit[2] = {1, 0}; // Mapping when 16 bits DQ swap.
    UINT32 u4HwDQSth;
	 
    if (i4Argc < 3)
    {
        Printf("dqso [Byte] [Inc=1:Dec=0] [Gap in secs].\n");
        return 0;
    }
    else
    {
        u4SelectByte = (UINT32)StrToInt(aszArgv[1]);        
        u4IncDelay = (UINT32)StrToInt(aszArgv[2]);
        u4TimeGap = (UINT32)StrToInt(aszArgv[3]);        
        u4HwDQSth = (IS_DDR_SWAP16BIT()) ? u4Swap16Bit[u4SelectByte] : u4SelectByte;         
    }

    u4DQSori = DRAM_DDRPHY_READ32(0x354);
    u4DQori[0] = DRAM_DDRPHY_READ32(0x360 + 0x0);
	u4DQori[1] = DRAM_DDRPHY_READ32(0x360 + 0x4);
	u4DQori[2] = DRAM_DDRPHY_READ32(0x360 + 0x8);
	u4DQori[3] = DRAM_DDRPHY_READ32(0x360 + 0xC);

    for(u4Step=1; u4Step < 32; u4Step++)
    {
        if (u4IncDelay)
        {
            Printf("\nStep: +%d\n", u4Step);
        }
        else
        {
            Printf("\nStep: -%d\n", u4Step);
        }        

        if(u4IncDelay)
        {
            tmp = 1;
            for(i=0; i<32; i+=8)    //scan bit
            {                
                if( (0x1f & (DRAM_DDRPHY_READ32(0x360 + (u4HwDQSth*4))>>i)) == 0 )
                {
                    tmp = 0;
					break;
                }
				if( (0x1f & (DRAM_DDRPHY_READ32(0x360 + (u4HwDQSth*4) + 8)>>i)) == 0 )
                {
                    tmp = 0;
					break;
                }
            }

            if(tmp==0)      //+DQS
            {
                u4delay = (DRAM_DDRPHY_READ32(0x354)>>(u4HwDQSth*8)) & 0x0000001f;
                if(u4delay==0x1f)
                {
                    break;
                }
                else
                {
                    u4delay++;
                }

                u4DQSnew = (DRAM_DDRPHY_READ32(0x354) & ~(0x0000001f << u4HwDQSth*8)) | (u4delay << u4HwDQSth*8) ;
                Printf("DQSdelay(0x58B54)=0x%08X.\n", u4DQSnew);
                DRAM_DDRPHY_WRITE32(0x354, u4DQSnew);                
            }
            else            // -DQ
            {
                u4DQnew0 = 0;
				u4DQnew1 = 0;
                for(i=0; i<32; i+=8)
                {
                    u4delay = 0x1f & (DRAM_DDRPHY_READ32(0x360 + u4HwDQSth*4)>>i) ;
                    u4delay--;
                    u4DQnew0 = u4DQnew0 | (u4delay<<i);  
					u4delay = 0x1f & (DRAM_DDRPHY_READ32(0x360 + u4HwDQSth*4 + 8)>>i) ;
                    u4delay--;
                    u4DQnew1 = u4DQnew1 | (u4delay<<i);
                }

                Printf("DQdelay(0x%05X)=0x%08X.\n", 0x58B60+u4HwDQSth*4, u4DQnew0);
				Printf("DQdelay(0x%05X)=0x%08X.\n", 0x58B60+u4HwDQSth*4 + 8, u4DQnew1);
                DRAM_DDRPHY_WRITE32(0x360+u4HwDQSth*4, u4DQnew0); 
				DRAM_DDRPHY_WRITE32(0x360+u4HwDQSth*4 + 8, u4DQnew1);  
            }

        }
        else
        {
            if( 0x0001f & (DRAM_DDRPHY_READ32(0x354)>>(u4HwDQSth*8))) 
            {
                tmp = 1;
            }
            else
            {
                tmp = 0;
            }

            if(tmp==1)      //-DQS
            {
                u4delay = (DRAM_DDRPHY_READ32(0x354)>>(u4HwDQSth*8)) & 0x0000001f;
                u4delay--;

                u4DQSnew = (DRAM_DDRPHY_READ32(0x354) & ~(0x0000001f << u4HwDQSth*8)) | (u4delay << u4HwDQSth*8) ;
                Printf("DQSdelay(0x58B54)=0x%08X.\n", u4DQSnew);
                DRAM_DDRPHY_WRITE32(0x354, u4DQSnew);                
            }
            else            // +DQ
            {
                u4DQnew0 = 0;
				u4DQnew1 = 0;
                for(i=0; i<32; i+=8)
                {
                    u4delay = 0x1f & (DRAM_DDRPHY_READ32(0x360 + u4HwDQSth*4)>>i) ;
                    if(u4delay != 0x1f)
                    {
                        u4delay++;
                    } 
                    u4DQnew0 = u4DQnew0 | (u4delay<<i);
					
					u4delay = 0x1f & (DRAM_DDRPHY_READ32(0x360 + u4HwDQSth*4 + 8)>>i) ;
                    if(u4delay != 0x1f)
                    {
                        u4delay++;
                    } 
                    u4DQnew1 = u4DQnew1 | (u4delay<<i);
                }

                Printf("DQdelay(0x%05X)=0x%08X.\n", (0x58B60+u4HwDQSth*4), u4DQnew0);
				Printf("DQdelay(0x%05X)=0x%08X.\n", (0x58B60+u4HwDQSth*4 + 8), u4DQnew1);
                DRAM_DDRPHY_WRITE32(0x360+u4HwDQSth*4, u4DQnew0);
				DRAM_DDRPHY_WRITE32(0x360+u4HwDQSth*4 + 8, u4DQnew1);
            }

        }
        Printf("Wait %d secs...\n", u4TimeGap);
        x_thread_delay(u4TimeGap*1000);
    }

    Printf("restore...\n");
    DRAM_DDRPHY_WRITE32(0x354, u4DQSori);

    DRAM_DDRPHY_WRITE32(0x360 + 0x0, u4DQori[0]);
	DRAM_DDRPHY_WRITE32(0x360 + 0x4, u4DQori[1]);
	DRAM_DDRPHY_WRITE32(0x360 + 0x8, u4DQori[2]);
	DRAM_DDRPHY_WRITE32(0x360 + 0xC, u4DQori[3]);

	
#else
#if defined(CC_MT5399)||defined(CC_MT5882)
    UINT32 u4SelectByte = 0;
    UINT32 u4DQori[8];
    UINT32 u4IncDelay = 0;
    UINT32 u4TimeGap = 20;
	UINT32 u4value;
    int ii,i;
	
    Printf("DRAM Channel %s is selected.\n", (IS_DRAM_CHANNELB_ACTIVE() ? "B": "A"));

	if (!IS_DRAM_CHANNELB_ACTIVE())
	{
		//A1 byte A&B
		u4DQori[0] = 0x3f & (ucDram_Register_Read(0xF00580d0)>>24);
		u4DQori[1] = 0x3f & (ucDram_Register_Read(0xF00580d0)>>8);
	    //A2 byte A
	    u4DQori[2] = 0x3f & (ucDram_Register_Read(0xF00582c8)>>8);
		//A2 byte B
		u4DQori[3] = 0x3f & (ucDram_Register_Read(0xF00582cc)>>24);	
		Printf("Output Delay:\n Byte0 = %d\n Byte1 = %d\n Byte2 = %d\n Byte3 = %d\n", 
		u4DQori[0], u4DQori[1], u4DQori[2], u4DQori[3]);
	}
	else
	{
		//B1 byte A
		u4DQori[4] = 0x3f & (ucDram_Register_Read(0xF00582d4)>>8);
		//B1 byte B
		u4DQori[5] = 0x3f & (ucDram_Register_Read(0xF00582d8)>>24);
		//B2 byte A&B
		u4DQori[6] = 0x3f & (ucDram_Register_Read(0xF00584d0)>>24);
		u4DQori[7] = 0x3f & (ucDram_Register_Read(0xF00584d0)>>8);	
		Printf("Output Delay:\n Byte0 = %d\n Byte1 = %d\n Byte2 = %d\n Byte3 = %d\n", 
		u4DQori[4], u4DQori[5], u4DQori[6], u4DQori[7]);
	}
	
    if (i4Argc < 3)
    {
        Printf("dqso [Byte] [Inc=1:Dec=0] [Gap in secs].\n");
        return 0;
    }
    else
    {
        u4SelectByte = (UINT32)StrToInt(aszArgv[1]);        
        u4IncDelay = (UINT32)StrToInt(aszArgv[2]);
        u4TimeGap = (UINT32)StrToInt(aszArgv[3]);        
        //u4HwDQSth = (IS_DDR_SWAP16BIT()) ? u4Swap16Bit[u4SelectByte] : u4SelectByte;         
    }

    for (ii = 0; ii <=25; ii++)
    {		  
       if (u4IncDelay)
	   {
	       //Move DQ (total 0x00~0x3F) up from 0x10 (default: 90 degree) to 0x28 (225 degree) to find the right boundary
           Printf("\nStep: +%d\n", ii);
		
		   if (!IS_DRAM_CHANNELB_ACTIVE())
           {
              switch (u4SelectByte)
              {
                 case 0:
                    //A1 byte A
                    u4value = ucDram_Register_Read(0xF00580d0);
					u4value = (~0x3f000000) & u4value;
					u4value = u4value | ((u4DQori[0] + ii) << 24);
                    ucDram_Register_Write(0xF00580d0, u4value);
					Printf("DQ00~DQ07 TX PI Delay 0xF00580d0[29:24] = %d.\n", (u4DQori[0] + ii));
			     break;
				 case 1:
					//A1 byte B
					u4value = ucDram_Register_Read(0xF00580d0);
					u4value = (~0x00003f00) & u4value;
					u4value = u4value | ((u4DQori[1] + ii) << 8);
					ucDram_Register_Write(0xF00580d0, u4value);
					Printf("DQ08~DQ15 TX PI Delay 0xF00580d0[13:08] = %d.\n", (u4DQori[1] + ii));
                 break;
				 case 2:
				    //A2 byte A
                    u4value = ucDram_Register_Read(0xF00582c8);
                    u4value = (~0x00003f00) & u4value;
					u4value = u4value | ((u4DQori[2] + ii) << 8);
                    ucDram_Register_Write(0xF00582c8, u4value);
					Printf("DQ16~DQ23 TX PI Delay 0xF00582c8[13:08] = %d.\n", (u4DQori[2] + ii));
				 break;
                 case 3:
					 //A2 byte B
					 u4value = ucDram_Register_Read(0xF00582cc);
                     u4value = (~0x3f000000) & u4value;
					 u4value = u4value | ((u4DQori[3] + ii) << 24);				
					 ucDram_Register_Write(0xF00582cc, u4value);  
					 Printf("DQ24~DQ31 TX PI Delay 0xF00582cc[29:24] = %d.\n", (u4DQori[3] + ii));
				 break;
	             default:
                     Printf("Invalid Channel!!! \n");
					 return 0;
              }
		   }
           else
           {
			  switch (u4SelectByte)
			  {
				 case 0:
                     //B1 byte A
                     u4value = ucDram_Register_Read(0xF00582d4);
					 u4value = (~0x00003f00) & u4value;
					 u4value = u4value | ((u4DQori[4] + ii) << 8);	
                     ucDram_Register_Write(0xF00582d4, u4value);
					 Printf("CHB_DQ00~DQ07 TX PI Delay 0xF00582d4[13:08] = %d.\n", (u4DQori[4] + ii));
			     break;
				 case 1:
                     //B1 byte B
                     u4value = ucDram_Register_Read(0xF00582d8);
					 u4value = (~0x3f000000) & u4value;
					 u4value = u4value | ((u4DQori[5] + ii) << 24);	
                     ucDram_Register_Write(0xF00582d8, u4value);  
					 Printf("CHB_DQ08~DQ15 TX PI Delay 0xF00582d8[29:24] = %d.\n", (u4DQori[5] + ii));
                 break;
				 case 2:
                     //B2 byte A
                     u4value = ucDram_Register_Read(0xF00584d0);
					 u4value = (~0x3f000000) & u4value;
					 u4value = u4value | ((u4DQori[6] + ii) << 24);	
                     ucDram_Register_Write(0xF00584d0, u4value);       
					 Printf("CHB_DQ16~DQ23 TX PI Delay 0xF00584d0[29:24] = %d.\n", (u4DQori[6] + ii));
			     break;
				 case 3:
				 	 //B2 byte B
                     u4value = ucDram_Register_Read(0xF00584d0);
					 u4value = (~0x00003f00) & u4value;
					 u4value = u4value | ((u4DQori[7] + ii) << 8);	
                     ucDram_Register_Write(0xF00584d0, u4value);    
					 Printf("CHB_DQ24~DQ31 TX PI Delay 0xF00584d0[13:08] = %d.\n", (u4DQori[7] + ii));
				 break;
				 default:
                     Printf("Invalid Channel!!! \n");
					 return 0;
              }
           }
       	}
	    else
	    {
           //Move DQ (total 0x00~0x3F) down from 0x10 (default: 90 degree) to 0x38 (-45 degree) to find the left boundary
           Printf("\nStep: -%d\n", ii); 
           
     	   if (!IS_DRAM_CHANNELB_ACTIVE())
	       {
	          i =(ii <= u4DQori[u4SelectByte]) ? ii : (ii - 64);
			  
	          switch (u4SelectByte)
              {
                 case 0:
		             //A1 byte A&B
		             u4value = ucDram_Register_Read(0xF00580d0);
					 u4value = (~0x3f000000) & u4value;
					 u4value = u4value | ((u4DQori[0] - i) << 24);
					 ucDram_Register_Write(0xF00580d0, u4value);
					 Printf("DQ00~DQ07 TX PI Delay 0xF00580d0[29:24] = %d.\n", (u4DQori[0] - i));
	             break;
				 case 1:
		             //A1 byte B
		             u4value = ucDram_Register_Read(0xF00580d0);
					 u4value = (~0x00003f00) & u4value;
					 u4value = u4value | ((u4DQori[1] - i) << 8);
					 ucDram_Register_Write(0xF00580d0, u4value);
					 Printf("DQ08~DQ15 TX PI Delay 0xF00580d0[13:08] = %d.\n", (u4DQori[1] - i));
				 break;
				 case 2:					
					 //A2 byte A
					 u4value = ucDram_Register_Read(0xF00582c8);
					 u4value = (~0x00003f00) & u4value;
					 u4value = u4value | ((u4DQori[2] - i) << 8);
					 ucDram_Register_Write(0xF00582c8, u4value);
					 Printf("DQ16~DQ23 TX PI Delay 0xF00582c8[13:08] = %d.\n", (u4DQori[2] - i));
				 break;
				 case 3: 
		             //A2 byte B
		             u4value = ucDram_Register_Read(0xF00582cc);
					 u4value = (~0x3f000000) & u4value;
					 u4value = u4value | ((u4DQori[3] - i) << 24);
		             ucDram_Register_Write(0xF00582cc, u4value);	
					 Printf("DQ24~DQ31 TX PI Delay 0xF00582cc[29:24] = %d.\n", (u4DQori[3] - i));
			     break;
				 default:
                     Printf("Invalid Channel!!! \n");
					 return 0;
	          }
     	   }
	       else
	       {
	          i =(ii <= u4DQori[u4SelectByte+4]) ? ii : (ii - 64);
			  
	       	  switch (u4SelectByte)
              {
                 case 0:
		             //B1 byte A
		             u4value = ucDram_Register_Read(0xF00582d4);
					 u4value = (~0x00003f000) & u4value;
					 u4value = u4value | ((u4DQori[4] - i) << 8);
		             ucDram_Register_Write(0xF00582d4, u4value);
					 Printf("CHB_DQ00~DQ07 TX PI Delay 0xF00582d4[13:08] = %d.\n", (u4DQori[4] - i));
				 break;
				 case 1:
		             //B1 byte B
		             u4value = ucDram_Register_Read(0xF00582d8);
					 u4value = (~0x3f000000) & u4value;
					 u4value = u4value | ((u4DQori[5] - i) << 24);
		             ucDram_Register_Write(0xF00582d8, u4value);
					 Printf("CHB_DQ08~DQ15 TX PI Delay 0xF00582d8[29:24] = %d.\n", (u4DQori[5] - i));
	             break;
				 case 2:
		             //B2 byte A
		             u4value = ucDram_Register_Read(0xF00584d0);
					 u4value = (~0x3f000000) & u4value;
					 u4value = u4value | ((u4DQori[6] - i) << 24);
		             ucDram_Register_Write(0xF00584d0, u4value);	
					 Printf("CHB_DQ16~DQ23 TX PI Delay 0xF00584d0[29:24] = %d.\n", (u4DQori[6] - i));
				break;	 
				case 3:
		             //B2 byte A&B
		             u4value = ucDram_Register_Read(0xF00584d0);
					 u4value = (~0x00003f00) & u4value;
					 u4value = u4value | ((u4DQori[7] - i) << 8);
		             ucDram_Register_Write(0xF00584d0, u4value);
					 Printf("CHB_DQ24~DQ31 TX PI Delay 0xF00584d0[13:08] = %d.\n", (u4DQori[7] - i));
				break;	
				default:
                     Printf("Invalid Channel!!! \n");
					 return 0;
	          }
	       }
	    }
	    Printf("Wait %d secs...\n", u4TimeGap);
        for (i=0; i<u4TimeGap*10; i++)
        {
            Printf("=");
#if defined(DRAM_MEMORY_AGENTTEST)
			_DDR_TestAgentMemTest();
#endif //#if defined(DRAM_MEMORY_AGENTTEST)
            x_thread_delay(100);
        }
		Printf("End.\n");
     }
#else
#if defined(CC_MT5890)
			UINT32 u4SelectByte = 0;
			UINT32 u4DQori[10];
			UINT32 u4IncDelay = 0;
			UINT32 u4TimeGap = 20;
			UINT32 u4value;
			int ii,i;
			
			Printf("DRAM Channel %s is selected.\n", (IS_DRAM_CHANNELA_ACTIVE() ? "A": (IS_DRAM_CHANNELB_ACTIVE() ? "B": "C")));
	
			// set PI DQ/DQM delay to registers
			if (IS_DRAM_CHANNELA_ACTIVE())
			{
					// A1 byteA/B
					u4DQori[0] = 0x1f & (ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x30c))>>8);
					u4DQori[1] = 0x1f & (ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x30c))>>0);
	
					// A2 byteA/B
					u4DQori[2] = 0x1f & (ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x40c))>>8);
					u4DQori[3] = 0x1f & (ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x40c))>>0); 
					Printf("Output Delay:\n Byte0 = %d\n Byte1 = %d\n Byte2 = %d\n Byte3 = %d\n", 
					u4DQori[0], u4DQori[1], u4DQori[2], u4DQori[3]);
			}
			else if (IS_DRAM_CHANNELB_ACTIVE())
			{
					// B1 byteA/B (NOTE: register is located on channel A bank)
					u4DQori[4] = 0x1f & (ucDram_Register_Read(mcSET_PHY_REG_ADDR_CHA(0x410))>>24);
					u4DQori[5] = 0x1f & (ucDram_Register_Read(mcSET_PHY_REG_ADDR_CHA(0x410))>>16);
	
					// B2 byteA/B
					u4DQori[6] = 0x1f & (ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x30c))>>8);
					u4DQori[7] = 0x1f & (ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x30c))>>0);
					Printf("Output Delay:\n Byte0 = %d\n Byte1 = %d\n Byte2 = %d\n Byte3 = %d\n", 
					u4DQori[4], u4DQori[5], u4DQori[6], u4DQori[7]);
			}
			else
			{
					// C1 byte A/B (channel C only has 16 bits)
					u4DQori[8] = 0x1f & (ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x30c))>>8);
					u4DQori[9] = 0x1f & (ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x30c))>>0);
					Printf("Output Delay:\n Byte0 = %d\n Byte1 = %d\n", 
				    u4DQori[8], u4DQori[8]);
			}
	
			if (i4Argc < 3)
			{
				Printf("dqso [Byte] [Inc=1:Dec=0] [Gap in secs].\n");
				return 0;
			}
			else
			{
				u4SelectByte = (UINT32)StrToInt(aszArgv[1]);		
				u4IncDelay = (UINT32)StrToInt(aszArgv[2]);
				u4TimeGap = (UINT32)StrToInt(aszArgv[3]);		 
				//u4HwDQSth = (IS_DDR_SWAP16BIT()) ? u4Swap16Bit[u4SelectByte] : u4SelectByte;		   
			}
	
			for (ii = 0; ii <=13; ii++)
			{		  
			   if (u4IncDelay)
			   {
				   //Move DQ (total 0x00~0x3F) up from 0x10 (default: 90 degree) to 0x28 (225 degree) to find the right boundary
				   Printf("\nStep: +%d\n", ii);

				   if (IS_DRAM_CHANNELA_ACTIVE())
				   {

					  switch (u4SelectByte)
					  {
						 case 0:
							//A1 byte A
							u4value = ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x30c));
							u4value = (~0x00001f00) & u4value;
							u4value = u4value | ((u4DQori[0] + ii) << 8);
							ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x30c), u4value);
							Printf("DQ00~DQ07 TX PI Delay = %d.\n", (u4DQori[0] + ii));
						 break;
						 case 1:
							//A1 byte B
							u4value = ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x30c));
							u4value = (~0x0000001f) & u4value;
							u4value = u4value | ((u4DQori[1] + ii) << 0);
							ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x30c), u4value);
				
							Printf("DQ08~DQ15 TX PI Delay = %d.\n", (u4DQori[1] + ii));
						 break;
						 case 2:
							//A2 byte A
							u4value = ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x40c));
							u4value = (~0x00001f00) & u4value;
							u4value = u4value | ((u4DQori[2] + ii) << 8);
							ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x40c), u4value);
							Printf("DQ16~DQ23 TX PI Delay = %d.\n", (u4DQori[2] + ii));
						 break;
						 case 3:
							 //A2 byte B
							 u4value = ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x40c));
							 u4value = (~0x0000001f) & u4value;
							 u4value = u4value | ((u4DQori[3] + ii) << 0);			
							 ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x40c), u4value);
							 Printf("DQ24~DQ31 TX PI Delay = %d.\n", (u4DQori[3] + ii));
						 break;
						 default:
							 Printf("Invalid Channel!!! \n");
							 return 0;
					  }
				   }
				   else if (IS_DRAM_CHANNELB_ACTIVE())
				   {
					  switch (u4SelectByte)
					  {
						 case 0:
							 //B1 byte A
							 u4value = ucDram_Register_Read(mcSET_PHY_REG_ADDR_CHA(0x410));
							 u4value = (~0x1f000000) & u4value;
							 u4value = u4value | ((u4DQori[4] + ii) << 24); 
							 ucDram_Register_Write(mcSET_PHY_REG_ADDR_CHA(0x410), u4value);
							 Printf("CHB_DQ00~DQ07 TX PI Delay = %d.\n", (u4DQori[4] + ii));
						 break;
						 case 1:
							 //B1 byte B
							 u4value = ucDram_Register_Read(mcSET_PHY_REG_ADDR_CHA(0x410));
							 u4value = (~0x001f0000) & u4value;
							 u4value = u4value | ((u4DQori[5] + ii) << 16); 
							 ucDram_Register_Write(mcSET_PHY_REG_ADDR_CHA(0x410), u4value);  
							 Printf("CHB_DQ08~DQ15 TX PI Delay = %d.\n", (u4DQori[5] + ii));
						 break;
						 case 2:
							 //B2 byte A
							 u4value = ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x30c));
							 u4value = (~0x00001f00) & u4value;
							 u4value = u4value | ((u4DQori[6] + ii) << 8); 
							 ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x30c), u4value);    
							 Printf("CHB_DQ16~DQ23 TX PI Delay = %d.\n", (u4DQori[6] + ii));
						 break;
						 case 3:
							 //B2 byte B
							 u4value = ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x30c));
							 u4value = (~0x0000001f) & u4value;
							 u4value = u4value | ((u4DQori[7] + ii) << 0);	
							 ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x30c), u4value); 
							 Printf("CHB_DQ24~DQ31 TX PI Delay = %d.\n", (u4DQori[7] + ii));
						 break;
						 default:
							 Printf("Invalid Channel!!! \n");
							 return 0;
					  }
				   }
				   else
				   {
					  switch (u4SelectByte)
					  {
						 case 0:
							 //C1 byte A
							 u4value = ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x30c));
							 u4value = (~0x00001f00) & u4value;
							 u4value = u4value | ((u4DQori[8] + ii) << 8);	
							 ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x30c), u4value);
							 Printf("CHC_DQ00~DQ07 TX PI Delay = %d.\n", (u4DQori[8] + ii));
						 break;
						 case 1:
							 //C1 byte B
							 u4value = ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x30c));
							 u4value = (~0x0000001f) & u4value;
							 u4value = u4value | ((u4DQori[9] + ii) << 0); 
							 ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x30c), u4value);
							 Printf("CHC_DQ08~DQ15 TX PI Delay = %d.\n", (u4DQori[9] + ii));
						 break;
						 default:
							 Printf("Invalid Channel!!! \n");
							 return 0;
					  }
					}
				}
				else
				{
				   //Move DQ (total 0x00~0x3F) down from 0x10 (default: 90 degree) to 0x38 (-45 degree) to find the left boundary
				   Printf("\nStep: -%d\n", ii); 
				   
				   if (IS_DRAM_CHANNELA_ACTIVE())
				   {
					  i =(ii <= u4DQori[u4SelectByte]) ? ii : (ii - 32);
					  
					  switch (u4SelectByte)
					  {
						 case 0:
							//A1 byte A
							u4value = ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x30c));
							u4value = (~0x00001f00) & u4value;
							u4value = u4value | ((u4DQori[0] - i) << 8);
							ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x30c), u4value);
							Printf("DQ00~DQ07 TX PI Delay = %d.\n", (u4DQori[0] - i));
						 break;
						 case 1:
							//A1 byte B
							u4value = ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x30c));
							u4value = (~0x0000001f) & u4value;
							u4value = u4value | ((u4DQori[1] - i) << 0);
							ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x30c), u4value);
				
							Printf("DQ08~DQ15 TX PI Delay = %d.\n", (u4DQori[1] - i));
						 break;
						 case 2:
							//A2 byte A
							u4value = ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x40c));
							u4value = (~0x00001f00) & u4value;
							u4value = u4value | ((u4DQori[2] - i) << 8);
							ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x40c), u4value);
							Printf("DQ16~DQ23 TX PI Delay = %d.\n", (u4DQori[2] - i));
						 break;
						 case 3:
							 //A2 byte B
							 u4value = ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x40c));
							 u4value = (~0x0000001f) & u4value;
							 u4value = u4value | ((u4DQori[3] - i) << 0);			
							 ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x40c), u4value);
							 Printf("DQ24~DQ31 TX PI Delay = %d.\n", (u4DQori[3] - i));
						 break;
						 default:
							 Printf("Invalid Channel!!! \n");
							 return 0;
					  }
				   }
				   else if (IS_DRAM_CHANNELB_ACTIVE())
				   {
					  i =(ii <= u4DQori[u4SelectByte+4]) ? ii : (ii - 32);
					  
					  switch (u4SelectByte)
					  {
						 case 0:
							 //B1 byte A
							 u4value = ucDram_Register_Read(mcSET_PHY_REG_ADDR_CHA(0x410));
							 u4value = (~0x1f000000) & u4value;
							 u4value = u4value | ((u4DQori[4] - i) << 24);	
							 ucDram_Register_Write(mcSET_PHY_REG_ADDR_CHA(0x410), u4value);
							 Printf("CHB_DQ00~DQ07 TX PI Delay = %d.\n", (u4DQori[4] - i));
						 break;
						 case 1:
							 //B1 byte B
							 u4value = ucDram_Register_Read(mcSET_PHY_REG_ADDR_CHA(0x410));
							 u4value = (~0x001f0000) & u4value;
							 u4value = u4value | ((u4DQori[5] - i) << 16); 
							 ucDram_Register_Write(mcSET_PHY_REG_ADDR_CHA(0x410), u4value);  
							 Printf("CHB_DQ08~DQ15 TX PI Delay = %d.\n", (u4DQori[5] - i));
						 break;
						 case 2:
							 //B2 byte A
							 u4value = ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x30c));
							 u4value = (~0x00001f00) & u4value;
							 u4value = u4value | ((u4DQori[6] - i) << 8); 
							 ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x30c), u4value);    
							 Printf("CHB_DQ16~DQ23 TX PI Delay = %d.\n", (u4DQori[6] - i));
						 break;
						 case 3:
							 //B2 byte B
							 u4value = ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x30c));
							 u4value = (~0x0000001f) & u4value;
							 u4value = u4value | ((u4DQori[7] - i) << 0);	
							 ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x30c), u4value); 
							 Printf("CHB_DQ24~DQ31 TX PI Delay = %d.\n", (u4DQori[7] - i));
						 break;
						 default:
							 Printf("Invalid Channel!!! \n");
							 return 0;
					  }
				   }
				   else
				   {
					  i =(ii <= u4DQori[u4SelectByte+8]) ? ii : (ii - 32);
	
					  switch (u4SelectByte)
					  {
						 case 0:
							 //C1 byte A
							 u4value = ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x30c));
							 u4value = (~0x00001f00) & u4value;
							 u4value = u4value | ((u4DQori[8] - i) << 8);	
							 ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x30c), u4value);
							 Printf("CHC_DQ00~DQ07 TX PI Delay = %d.\n", (u4DQori[8] - i));
						 break;
						 case 1:
							 //C1 byte B
							 u4value = ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x30c));
							 u4value = (~0x0000001f) & u4value;
							 u4value = u4value | ((u4DQori[9] - i) << 0); 
							 ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x30c), u4value);
							 Printf("CHC_DQ08~DQ15 TX PI Delay = %d.\n", (u4DQori[9] - i));
						 break;
						 default:
							 Printf("Invalid Channel!!! \n");
							 return 0;
					  }
				   }
				}

				Printf("Wait %d secs...\n", u4TimeGap);
				for (i=0; i<u4TimeGap*10; i++)
				{
					Printf("=");
#if defined(DRAM_MEMORY_AGENTTEST)
					_DDR_TestAgentMemTest();
#endif //#if defined(DRAM_MEMORY_AGENTTEST)
					x_thread_delay(100);
				}
				Printf("End.\n");
			}
#else
    UINT32 i;
    UINT32 tmp;
    UINT32 u4SelectByte = 0;
    UINT32 u4DQSori;
    UINT32 u4DQori[4];
    UINT32 u4DQSnew;
    UINT32 u4delay;
    UINT32 u4Step;
    UINT32 u4IncDelay = 0;
    UINT32 u4TimeGap = 20;
    UINT32 u4Bytes;
    UINT32 u4Swap16Bit[4] = {2, 3, 0, 1}; // Mapping when 16 bits DQ swap.
    UINT32 u4HwDQSth;

    Printf("DRAM Channel %s is selected.\n", (IS_DRAM_CHANNELB_ACTIVE() ? "B": "A"));
      
    if (i4Argc < 3)
    {
        Printf("dqso [Byte] [Inc=1:Dec=0] [Gap in secs].\n");
        return 0;
    }
    else
    {
        u4SelectByte = (UINT32)StrToInt(aszArgv[1]);        
        u4IncDelay = (UINT32)StrToInt(aszArgv[2]);
        u4TimeGap = (UINT32)StrToInt(aszArgv[3]);        
        u4HwDQSth = (IS_DDR_SWAP16BIT()) ? u4Swap16Bit[u4SelectByte] : u4SelectByte;         
    }

    u4Bytes = (IS_FORCE32()) ? 2 : 4;

    u4DQSori = DRAM_READ32(0x354);
    for(i=0; i<u4Bytes; i++)
    {
        tmp = (IS_DDR_SWAP16BIT()) ? u4Swap16Bit[i] : i; 
        u4DQori[i] = DRAM_READ32(0x360 + tmp*4);
    }

    for(u4Step=1; u4Step < 32; u4Step++)
    {
        if (u4IncDelay)
        {
            Printf("\nStep: +%d\n", u4Step);
        }
        else
        {
            Printf("\nStep: -%d\n", u4Step);
        }        

        if(u4IncDelay)
        {
            tmp = 1;
            for(i=0; i<32; i+=4)    //scan bit
            {                
                if( (0x000f & (DRAM_READ32(0x360 + (u4HwDQSth*4))>>i)) == 0 )
                {
                    tmp = 0;
                }
            }

            if(tmp==0)      //+DQS
            {
                u4delay = (DRAM_READ32(0x354)>>(u4HwDQSth*4)) & 0x0000000f;
                if(u4delay==0xf)
                {
                    break;
                }
                else
                {
                    u4delay++;
                }

                u4DQSnew = (DRAM_READ32(0x354) & ~(0x0000000f << u4HwDQSth*4)) | (u4delay << u4HwDQSth*4) ;
                Printf("DQSdelay(0x7354)=0x%08X.\n", u4DQSnew);
                DRAM_WRITE32(0x354, u4DQSnew);                
            }
            else            // -DQ
            {
                u4DQSnew = 0;
                for(i=0; i<32; i+=4)
                {
                    u4delay = 0x000f & (DRAM_READ32(0x360 + u4HwDQSth*4)>>i) ;
                    u4delay--;
                    u4DQSnew = u4DQSnew | (u4delay<<i);                    
                }

                Printf("DQdelay(0x%04X)=0x%08X.\n", 0x7360+u4HwDQSth*4, u4DQSnew);
                DRAM_WRITE32(0x360+u4HwDQSth*4, u4DQSnew);              
            }

        }
        else
        {
            if( 0x000f & (DRAM_READ32(0x354)>>(u4HwDQSth*4))) 
            {
                tmp = 1;
            }
            else
            {
                tmp = 0;
            }

            if(tmp==1)      //-DQS
            {
                u4delay = (DRAM_READ32(0x354)>>(u4HwDQSth*4)) & 0x0000000f;
                u4delay--;

                u4DQSnew = (DRAM_READ32(0x354) & ~(0x0000000f << u4HwDQSth*4)) | (u4delay << u4HwDQSth*4) ;
                Printf("DQSdelay(0x7354)=0x%08X.\n", u4DQSnew);
                DRAM_WRITE32(0x354, u4DQSnew);                
            }
            else            // +DQ
            {
                u4DQSnew = 0;
                for(i=0; i<32; i+=4)
                {
                    u4delay = 0x000f & (DRAM_READ32(0x360 + u4HwDQSth*4)>>i) ;
                    if(u4delay != 0x0f)
                    {
                        u4delay++;
                    }
                    
                    u4DQSnew = u4DQSnew | (u4delay<<i);
                }

                Printf("DQdelay(0x%04X)=0x%08X.\n", (0x7360+u4HwDQSth*4), u4DQSnew);
                DRAM_WRITE32(0x360+u4HwDQSth*4, u4DQSnew);
            }

        }

        Printf("Wait %d secs...\n", u4TimeGap);
        x_thread_delay(u4TimeGap*1000);
    }

    Printf("restore...\n");
    DRAM_WRITE32(0x354, u4DQSori);
    for (i=0; i<u4Bytes; i++)
    {
        DRAM_WRITE32(0x360 + i*4, u4DQori[i]);
    }
#endif //defined(CC_MT5890)
#endif //defined(CC_MT5399)
#endif //defined(CC_MT5881)
#endif //defined(CC_MT5398)   
#endif //if defined(CC_MT5368) || defined(CC_MT5396)) || defined(CC_MT5389)

	return 0;
}

#ifdef DRAM_HWONLINECALDQS
static void _DDR_DQSCalDisable(void) 
{ 
    DRAM_WRITE32(0x220, 0);
}

static void _DDR_DQSCalStart(void)
{
    UINT32 u4addr;

    u4addr = ((UINT32)(au4HwCaliBuffer)) & 0xF;

    u4addr = ((UINT32)(au4HwCaliBuffer) +(0x10-u4addr)+0x10);
    
    //audio pattern; loop=4, post write enabling for test agent 2.
    DRAM_WRITE32(0x218, 0x001F110D);
    DRAM_WRITE32(0x220, 0x804);
    //test agent2 address
    DRAM_WRITE32(0x210, u4addr);
    //test agent2 offset
    DRAM_WRITE32(0x214, HWCALI_BUFFERSIZE);
    //enable DQS cal.
    DRAM_WRITE32(0x220, 0x824);
}

static BOOL _DDR_IsDQSCalDone(void)
{
    // CALI_DONE_MON: calibration result is updated, SW can disable calibration.
    // From 0 - > 1 : Calibration done.
    // From 1 - > 0 : Calibration is turned off at the internal of DRAMC.
    return ((DRAM_READ32(0x280) & 0x80000000) == 0) ? FALSE : TRUE;
}

static void _DDR_UpdateDQS(void)
{
    UINT32 dq, dqs_min, dqs_max, dqs_avg, manualdqs, newmanualdqs;
    UINT32 i;
    UINT32 u4OldVal; 
    UINT32 u4NewVal;
    UINT32 u4Bytes;

    u4Bytes = (IS_FORCE32()) ? 2 : 4;
       
    dq = DRAM_READ32(0x290);
    dqs_min = DRAM_READ32(0x294);
    dqs_max = DRAM_READ32(0x298);
    dqs_avg = DRAM_READ32(0x29c);  // binary coded.
   
    manualdqs = DRAM_READ32(0x3A0); // binary coded.
    newmanualdqs = manualdqs;

    for (i = 0; i<u4Bytes; i++)
    {
        u4NewVal = (dqs_avg >> (8 * i)) & 0xFF;
        u4OldVal = (manualdqs >> (8 * i)) & 0xFF;

        if (u4NewVal > (u4OldVal + 1))
        {
            newmanualdqs &= ~(0xFF << (8 * i));
            newmanualdqs |= (u4OldVal + 1) << (8 * i);
            LOG(5, "DRAM: Byte %d, h/w NewVal=%d > OldVal=%d, Set=%d.\n", 
                i, u4NewVal, u4OldVal, (u4OldVal + 1));            
        }
        else if (u4NewVal < (u4OldVal - 1))
        {
            newmanualdqs &= ~(0xFF << (8 * i));        
            newmanualdqs |= (u4OldVal - 1) << (8 * i);
            LOG(5, "DRAM: Byte %d, h/w NewVal=%d < OldVal=%d, Set=%d.\n", 
                i, u4NewVal, u4OldVal, (u4OldVal - 1));            
        }
    }

    // Need to update center value of DQS.
    if (newmanualdqs != manualdqs)
    {   
        for (i=0; i<u4Bytes; i++)
        {
            dq = (INT32)DRAM_READ32(0x290);
            dq = (dq >> (8*i)) & 0xFF;
            
            dqs_min = (INT32)DRAM_READ32(0x294);
            dqs_min = (dqs_min >> (8*i)) & 0xFF;
            
            dqs_max = (INT32)DRAM_READ32(0x298);
            dqs_max = (dqs_max >> (8*i)) & 0xFF;            
            
            dqs_avg = (INT32)DRAM_READ32(0x29C);
            dqs_avg = (dqs_avg >> (8*i)) & 0xFF;            
            
            if (dq != 0)
            {
                dqs_min = dqs_min - dq;
            }

            LOG(1, "DRAM: HW Byte %d : DQS(%d ~ %d), Size %d, Middle = %d.\n", 
                i, dqs_min, dqs_max, (dqs_max- dqs_min + 1), dqs_avg);
        }
        
        do 
        {
            // Note: This delay can not be removed.
            // After disable h/w calibration, it will actually being disabled at next refresh cycle.
            // At this critical section, we can not turn on MANUDQSUPD=1.
            x_thread_delay(10);

        // Check if Calibration is turned off at the internal of DRAMC.
        } while(_DDR_IsDQSCalDone());
            
        // Update manual DQS input delay only while MANUDQSUPD=1 and DLLFRZ.   
        DRAM_WRITE32(0x234, (DRAM_READ32(0x234) | 0x00040000)); 
        
        DRAM_WRITE32(0x3A0, newmanualdqs);

        // On-line manual DQS input delay adjust enabling.
        DRAM_WRITE32(0x234, (DRAM_READ32(0x234) | 0x00020000)); 

        x_thread_delay(10);

        DRAM_WRITE32(0x234, (DRAM_READ32(0x234) & (~0x00060000)));                 
    }
}

static INT32 _DDR_OnlineDQSCal( INT32 i4Argc, const CHAR ** aszArgv )
{
    if (i4Argc != 2)
    {
        Printf("  [0:Disable /1:Enable].\n");
    }
    else
    {
        u4HwCalDQSEnable = (UINT32)StrToInt(aszArgv[1]);
    }

    if (u4HwCalDQSEnable == 0)
    {
        Printf("Current Setting: Disable\n");
    }
    else
    {
        Printf("Current Setting: Enable\n");
    }

    return 0;
}
#endif // #ifdef DRAM_HWONLINECALDQS

#if defined(CC_MT5396) || defined(CC_MT5368) || defined(CC_MT5389)

static INT32 _DDR_SSCTest(INT32 i4Argc, const CHAR** aszArgv)
{
    UINT32 range = 0;
    UINT32 modulation;
    UINT32 u4Val;	
    UINT32 u4SysPLL = 648; // Unit in Mhz.

    if(i4Argc >= 2)
    {
        range = StrToInt(aszArgv[1]);
        if (range == 0)
        {
            Printf("DRAM: Disable SSC.\n");
            /* Disable Spectrum */
            CKGEN_WRITE32(0x194, CKGEN_READ32(0x194) & ~0x4000 );
            return 0;        
        }
    }    	

    if(i4Argc < 3)
    {
        Printf("ssc n m.\n");
        Printf("  ,n: +-n/10000 percent spreading range\n");
        Printf("  ,m: modulation freq Khz, m must >=1\n");        
        return 0;
    }    	

    modulation = StrToInt(aszArgv[2]);

    Printf("DRAM: SSC range=%d, mod_freq=%d.\n", range, modulation);
    
    if (modulation == 0)
    {		
        Printf("Modulation freq Khz, m must >=1\n");
        return 0;
    }
   
    /* Disable Spectrum */
    CKGEN_WRITE32(0x194, CKGEN_READ32(0x194) & ~0x4000 );

    /* Setup Step. */	
    u4Val =(u4SysPLL/54)*range*2*2;
    u4Val =(u4Val*16 *1024)/(54000/(modulation*16));
    u4Val =u4Val /10000;
    u4Val= ((u4Val/2)<<16)|u4Val;
    CKGEN_WRITE32(0x198, u4Val);//set DELTA & center spread    
    
    /* Setup Period. */
    u4Val = (27000 ) / modulation;
    CKGEN_WRITE32(0x194, (CKGEN_READ32(0x194) & 0xFFFF) | (u4Val<<16)|0x0000A003);

    //enable ssc
    CKGEN_WRITE32(0x194, CKGEN_READ32(0x194) | 0x4000 );

    return 0;
}
#else //defined(CC_MT5396) || defined(CC_MT5368) || defined(CC_MT5389)
#if defined(CC_MT5398)
static INT32 _DDR_SSCTest(INT32 i4Argc, const CHAR** aszArgv)
{
    UINT32 range = 0;
    UINT32 modulation;
    UINT32 u4Val;	
    UINT32 u4SysPLL = 648; // Unit in Mhz.

    if(i4Argc >= 2)
    {
        range = StrToInt(aszArgv[1]);
        if (range == 0)
        {
            Printf("DRAM: Disable SSC.\n");
            /* Disable Spectrum */
            CKGEN_WRITE32(0x94, CKGEN_READ32(0x94) & ~0x4000 );
            return 0;        
        }
    }    	

    if(i4Argc < 3)
    {
        Printf("ssc n m.\n");
        Printf("  ,n: +-n/10000 percent spreading range\n");
        Printf("  ,m: modulation freq Khz, m must >=1\n");        
        return 0;
    }    	

    modulation = StrToInt(aszArgv[2]);

    Printf("DRAM: SSC range=%d, mod_freq=%d.\n", range, modulation);
    
    if (modulation == 0)
    {		
        Printf("Modulation freq Khz, m must >=1\n");
        return 0;
    }
   
    /* Disable Spectrum */
    CKGEN_WRITE32(0x94, CKGEN_READ32(0x94) & ~0x4000 );
	if(IS_XTAL_24MHZ())
	{
	    /* Setup Step. */	
	    u4Val =(u4SysPLL/48)*range*2*2;
	    u4Val =(u4Val*16 *1024)/(48000/(modulation*16));
	    u4Val =u4Val /10000;
	    u4Val= ((u4Val/2)<<16)|u4Val;
	    CKGEN_WRITE32(0x98, u4Val);//set DELTA & center spread    
	    
	    /* Setup Period. */
	    u4Val = (24000 ) / modulation;
	    CKGEN_WRITE32(0x94, (CKGEN_READ32(0x94) & 0xFFFF) | (u4Val<<16)|0x0000A003);
	}
	else
	{
		/* Setup Step. */	
	    u4Val =(u4SysPLL/54)*range*2*2;
	    u4Val =(u4Val*16 *1024)/(54000/(modulation*16));
	    u4Val =u4Val /10000;
	    u4Val= ((u4Val/2)<<16)|u4Val;
	    CKGEN_WRITE32(0x98, u4Val);//set DELTA & center spread    
	    
	    /* Setup Period. */
	    u4Val = (27000 ) / modulation;
	    CKGEN_WRITE32(0x94, (CKGEN_READ32(0x94) & 0xFFFF) | (u4Val<<16)|0x0000A003);
	}
	//enable ssc
    CKGEN_WRITE32(0x94, CKGEN_READ32(0x94) | 0x4000 );

    return 0;
}
#else  //defined(CC_MT5398)
#if defined(CC_MT5880) || defined(CC_MT5881)
static INT32 _DDR_SSCTest(INT32 i4Argc, const CHAR** aszArgv)
{
    UINT32 range = 0;
    UINT32 modulation;
    UINT32 u4Val;	
    UINT32 u4SysPLL = 648; // Unit in Mhz.
    UINT32 u4DMSS;
    
    if(i4Argc >= 2)
    {
        range = StrToInt(aszArgv[1]);
        if (range == 0)
        {
            Printf("DRAM: Disable SSC.\n");
            /* Disable Spectrum */
            CKGEN_WRITE32(0x94, CKGEN_READ32(0x94) & ~0x4000 );
            return 0;        
        }
    }    	

    if(i4Argc < 3)
    {
        Printf("ssc n m.\n");
        Printf("  ,n: +-n/10000 percent spreading range\n");
        Printf("  ,m: modulation freq Khz, m must >=1\n");        
        return 0;
    }    	

    modulation = StrToInt(aszArgv[2]);

    Printf("DRAM: SSC range=%d, mod_freq=%d.\n", range, modulation);
    
    if (modulation == 0)
    {		
        Printf("Modulation freq Khz, m must >=1\n");
        return 0;
    }
   
    
    /* Disable Spectrum */
    CKGEN_WRITE32(0x94, CKGEN_READ32(0x94) & ~0x4000 );
    //wait the dram clock freq go back to the center
    x_thread_delay(200);

    u4DMSS = (IS_XTAL_24MHZ()) ? 24 : 27;
        
	u4Val =(u4SysPLL/u4DMSS)*range*2*2;
	u4Val =(u4Val*16 *1024)/(u4DMSS*1000/(modulation*16));
	u4Val =u4Val /10000;
    u4Val= ((u4Val/2)<<16)|u4Val;
    CKGEN_WRITE32(0x98, u4Val);//set DELTA & center spread

    u4Val = CKGEN_READ32(0x90);
    CKGEN_WRITE32(0x90,u4Val|0x80000000);//after change DMSS setting, DE suggest to trggle 0xD230[31]=1

    /* Setup Period. For 24.0mhz dmss output (sysplll:648)*/
    //turn on NCPO and FIFO state machine & DMSS_PRD & SSC triangle wave
	u4Val = (u4DMSS*1000/2) / modulation;
    CKGEN_WRITE32(0x94, (CKGEN_READ32(0x94) & 0xFFFF) | (u4Val<<16)|0x0000A003);


	//enable ssc
    CKGEN_WRITE32(0x94, CKGEN_READ32(0x94) | 0x4000 );

    return 0;
}

#else
#if defined(CC_MT5399)||defined(CC_MT5882)
static INT32 _DDR_SSCTest(INT32 i4Argc, const CHAR** aszArgv)
{
	UINT32 range = 0;
    UINT32 modulation;
    UINT32 u4Val;
	U32 u4mempll_prediv_hex;
	U32 u4mempll_predivider[3] = {1, 2, 4}; //caution: u4mempll_predivider = 2^u4mempll_prediv_hex;
	U16 u2frequency1;
	U32 u4ssc_prd, u4ssc_delta;

	if(i4Argc >= 2)
    {
        range = StrToInt(aszArgv[1]);
        if (range == 0)
        {
            Printf("DRAM: Disable SSC.\n");
            /* Disable Spectrum */
            //RG_MEMPLL_SSC_EN (12ACH[29]) from 0 to 1 (DDS SSC enable). 20us after RG_MEMPLL_BIAS_RST from FNPLL AN (It is enough here)
			u4Val = ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x2ac));
			mcCLR_BIT(u4Val, 29);
			ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x2ac), u4Val);
            return 0;        
        }
    }

	if(i4Argc < 3)
    {
        Printf("ssc n m.\n");
        Printf("  ,n: +-n/10000 percent spreading range\n");
        Printf("  ,m: modulation freq Khz, m must >=1\n");        
        return 0;
    }

	modulation = StrToInt(aszArgv[2]);

    Printf("DRAM: SSC range=%d, mod_freq=%d.\n", range, modulation);
    
    if (modulation == 0)
    {		
        Printf("Modulation freq Khz, m must >=1\n");
        return 0;
    }

	/* Disable Spectrum */
	//RG_MEMPLL_SSC_EN (12ACH[29]) from 0 to 1 (DDS SSC enable). 20us after RG_MEMPLL_BIAS_RST from FNPLL AN (It is enough here)
	u4Val = ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x2ac));
	mcCLR_BIT(u4Val, 29);
	ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x2ac), u4Val);
	x_thread_delay(200);

	u2frequency1 = ((TCMGET_DDR_CLK()/BASE_DDR_CLK)/2)/2;
	u4Val = ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x2a0));
	u4mempll_prediv_hex = u4Val&0x03;

	ASSERT(u4mempll_prediv_hex <=2);
	u4ssc_prd = (((XTAL_MHZ/u4mempll_predivider[u4mempll_prediv_hex])*1000)/modulation)/2;
	u4ssc_delta = ((u2frequency1*262144)/XTAL_MHZ)*range*2/(10000*u4ssc_prd);
	Printf("u4ssc_prd =%08x, u4ssc_delta =%08x\n", u4ssc_prd, u4ssc_delta);
	
	if(u4ssc_delta>0xFF)
	{
		Printf("u4ssc_delta is overflow, please change the ssc setting. \n");
	}
	else
	{
		//set ssc for down spread
		u4Val = ((u4ssc_delta)<<16)|(u4ssc_delta<<24);
		u4Val = (ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x2b0))&0x0000FFFF) | u4Val;
		ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x2b0), u4Val);

		u4Val = (ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x2ac))&0xFFFF0000)|(u4ssc_prd&0xFFFF);
		ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x2ac), u4Val);
	}
	//enable ssc
	//RG_MEMPLL_SSC_EN (12ACH[29]) from 0 to 1 (DDS SSC enable). 20us after RG_MEMPLL_BIAS_RST from FNPLL AN (It is enough here)
	u4Val = ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x2ac));
	mcSET_BIT(u4Val, 29);
	ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x2ac), u4Val);
	
	return 0;
}
#else
#if defined(CC_MT5890)
static INT32 _DDR_SSCTest(INT32 i4Argc, const CHAR** aszArgv)
{
	UINT32 u4Permillage = 0;
    UINT32 u4Frequency;
    UINT32 u4Val;
	U32 u4mempll_postdiv_hex = 2;
	U32 u4mempll_postdivider[3] = {1, 2, 4}; //caution: u4mempll_postdivider = 2^u4mempll_postdiv_hex;
	U16 u4dramc_clock;
	U32 u4ssc_prd, u4ssc_delta;

	if(i4Argc >= 2)
    {
        u4Permillage = StrToInt(aszArgv[1]);
        if (u4Permillage == 0)
        {
            Printf("DRAM: Disable SSC.\n");
            /* Disable Spectrum */
            //RG_MEMPLL_SDM_SSC_EN (0X7B414[30]) 
			u4Val = ucDram_Register_Read(mcSET_PHY_REG_ADDR_CHB(0x414));
			mcCLR_BIT(u4Val, 30);
			ucDram_Register_Write(mcSET_PHY_REG_ADDR_CHB(0x414), u4Val);
            return 0;        
        }
    }

	if(i4Argc < 3)
    {
        Printf("ssc n m.\n");
        Printf("  ,n: +-n/10000 percent spreading range\n");
        Printf("  ,m: modulation freq Khz, m must >=1\n");        
        return 0;
    }

    if (u4Permillage > 100)
    {		
        Printf("Spreading range (+-n/10000), n must <=100.\n");
        return 0;
    }

	u4Frequency = StrToInt(aszArgv[2]);

    Printf("DRAM: SSC range = %d, mod_freq = %d.\n", u4Permillage, u4Frequency);
    
    if (u4Frequency == 0)
    {		
        Printf("Modulation freq Khz, m must >=1\n");
        return 0;
    }

	/* Disable Spectrum */
	//RG_MEMPLL_SDM_SSC_EN (0X7B414[30]) 
	u4Val = ucDram_Register_Read(mcSET_PHY_REG_ADDR_CHB(0x414));
	mcCLR_BIT(u4Val, 30);
	ucDram_Register_Write(mcSET_PHY_REG_ADDR_CHB(0x414), u4Val);
	x_thread_delay(200);
	
	u4dramc_clock = ((TCMGET_DDR_CLK()/BASE_DDR_CLK)/2)/2;
	u4ssc_prd = 1000*XTAL_MHZ/u4Frequency/2;
	u4ssc_delta = u4Permillage*2*u4dramc_clock*u4mempll_postdivider[u4mempll_postdiv_hex]*1024/u4ssc_prd/XTAL_MHZ*256/10000;

	//Printf("u4ssc_prd =%08x, u4ssc_delta =%08x\n", u4ssc_prd, u4ssc_delta);
	

	if(u4ssc_delta>0xFFFF)
	{
		Printf("u4ssc_delta is overflow, please change the ssc setting. \n");
	}
	else
	{
		u4Val = (ucDram_Register_Read(mcSET_PHY_REG_ADDR_CHB(0x40c))&0xFFFF0000) | u4ssc_delta;
		mcSET_BIT(u4Val, 24);
		ucDram_Register_Write(mcSET_PHY_REG_ADDR_CHB(0x40c), u4Val);

        //center  spread
		//u4Val = ((u4ssc_delta/2)<<16) | u4ssc_prd;
		
		//down  spread
		u4Val = ((u4ssc_delta)<<16) | u4ssc_prd;
		ucDram_Register_Write(mcSET_PHY_REG_ADDR_CHB(0x410), u4Val);
	}

	//enable ssc
	//RG_MEMPLL_SDM_SSC_EN (0X7B414[30]) from 0 to 1 (DDS SSC enable).
	u4Val = ucDram_Register_Read(mcSET_PHY_REG_ADDR_CHB(0x414));
	mcSET_BIT(u4Val, 30);
	ucDram_Register_Write(mcSET_PHY_REG_ADDR_CHB(0x414), u4Val);

	return 0;
}
#else
static INT32 _DDR_SSCTest(INT32 i4Argc, const CHAR** aszArgv)
{
    UINT32 range = 0;
    UINT32 modulation;
    UINT32 u4Val;	
    UINT32 u4SysPLL = 459; // Unit in Mhz.

    if(i4Argc >= 2)
    {
        range = StrToInt(aszArgv[1]);
        if (range == 0)
        {
            Printf("DRAM: Disable SSC.\n");
            /* Disable Spectrum */
            CKGEN_WRITE32(0x190, CKGEN_READ32(0x190) | 0x40 );
            return 0;        
        }
    }    	

    if(i4Argc < 3)
    {
        Printf("ssc n m.\n");
        Printf("  ,n: +-n/10000 percent spreading range\n");
        Printf("  ,m: modulation freq Khz, m must >=1\n");        
        return 0;
    }    	

    modulation = StrToInt(aszArgv[2]);

    Printf("DRAM: SSC range=%d, mod_freq=%d.\n", range, modulation);
    
    if (modulation == 0)
    {		
        Printf("Modulation freq Khz, m must >=1\n");
        return 0;
    }
   
    /* Disable Spectrum */
    CKGEN_WRITE32(0x190, CKGEN_READ32(0x190) | 0x40 );

    /* Setup Step. */	       
    u4Val = (u4SysPLL*360)/1000;
    u4Val = ((modulation*range*u4Val)/10000) & 0xFFFF;
    CKGEN_WRITE32(0x194, (CKGEN_READ32(0x194) & ~0xFFFF) | u4Val);

    /* Setup Period. */
    u4Val = (27000 /4) / modulation;
    CKGEN_WRITE32(0x190, (CKGEN_READ32(0x190) & 0xFFFF) | (u4Val<<16));

    //enable ssc
    CKGEN_WRITE32(0x190, CKGEN_READ32(0x190) & ~0x40 );

    /* Set Direct Mempll reference clock(Bypass Dmpll) from Xtal to DMSS */
    CKGEN_WRITE32(0x124, CKGEN_READ32(0x124) | 0x0800);

    return 0;
}
#endif	//defined(CC_MT5890)
#endif	//defined(CC_MT5399)
#endif	//defined(CC_MT5880) || defined(CC_MT5881)
#endif  //defined(CC_MT5398)
#endif //defined(CC_MT5396) || defined(CC_MT5368) || defined(CC_MT5389)


#if defined(CC_MT5890)
static INT32 _DDR_InforQuery(INT32 i4Argc, const CHAR** aszArgv)
{
    UINT32 u4DramType = TCMGET_DRAMTYPE();

    UINT32 u4Val; 
    UINT32 u4WODT;    
	
    UINT32 range;
    UINT32 modulation;

	U32 u4mempll_postdiv_hex=2;
	U32 u4mempll_postdivider[3] = {1, 2, 4}; //caution: u4mempll_postdivider = 2^u4mempll_postdiv_hex;
	U32 u2frequency1;
	U32 u4ssc_prd, u4ssc_delta;
        
    Printf("DRAM Type = %s.\n", ((u4DramType == DDR_III_x1) ? "DDR_III_x1" : 
                               ((u4DramType == DDR_III_x2) ? "DDR_III_x2" : 
			                   ((u4DramType == DDR_III_x3) ? "DDR_III_x3" : 
			                   ((u4DramType == DDR_III_x4) ? "DDR_III_x4" : "DDR_III_x5")))));
			              
	Printf("DRAM Data Rate = %d Mbps, CL = %d.\n", (TCMGET_DDR_CLK()/1000000), TCMGET_DDR_CL());

	Printf("ChA Size = %d MBytes, ChB Size = %d MBytes, ChC Size = %d MBytes.\n", TCMGET_CHANNELA_SIZE(), TCMGET_CHANNELB_SIZE(), TCMGET_CHANNELC_SIZE());

    // Reduce/Full strength
    u4Val = (((U32)TCMGET_MR1>>1) & 0x1);
	// Write ODT
    u4WODT = ((U32)TCMGET_MR1 & 0x244);

    Printf("DRAM Output Driver = %s, DRAM ODT = %s.\n", (u4Val ? "RZQ/7": "RZQ/6"), ((u4WODT == 0x0004) ? "60 ohm" : 
                                                                                     ((u4WODT == 0x0040) ? "120 ohm" : 
                                                                                     ((u4WODT == 0x0044) ? "40 ohm" : "Disable"))));

    //SSC Setting
	u4Val = ucDram_Register_Read(mcSET_PHY_REG_ADDR_CHB(0x414));
	if(mcCHK_BIT1(u4Val, 30)!=0)
	{
		u2frequency1 = ((TCMGET_DDR_CLK()/BASE_DDR_CLK)/2)/2;
		
		u4Val = ucDram_Register_Read(mcSET_PHY_REG_ADDR_CHB(0x410));
		u4ssc_prd = u4Val & 0xFFFF;
		modulation = (XTAL_MHZ*1000/(u4ssc_prd*2));

        u4Val = ucDram_Register_Read(mcSET_PHY_REG_ADDR_CHB(0x40c));
		u4ssc_delta = u4Val & 0xFFFF;
		range = (10000*u4ssc_prd*XTAL_MHZ/u2frequency1*u4ssc_delta)/u4mempll_postdivider[u4mempll_postdiv_hex]/262144/2;
		range = range+1;
		
		Printf("DRAM: SSC range=%d, mod_freq=%d.\n", range, modulation); 
	}
	else
	{
		Printf("DRAM: SSC Disable.\n");
	}		
	
    return 0;    
}

#else 
static INT32 _DDR_InforQuery(INT32 i4Argc, const CHAR** aszArgv)
{
    UINT32 u4Val; 
    UINT32 u4Mux;
    UINT32 u4WODT;    
    UINT32 u4RODT;    
    UINT32 i;    
    UINT32 u4Rec;
    UINT32 u4Bytes;
    INT32 i4Avg;
#ifdef DRAM_HWONLINECALDQS    
    INT32 i4DQMax;
#endif
    INT32 i4Min;
    INT32 i4Max;
    INT32 i4Size;
    UINT32 u4TotalSecs = 0;
    UINT32 u4Secs = 0;   
    UINT32 u4Mins = 0;
    UINT32 u4Hrs = 0;
    UINT32 u4DMSS = 27;
    UINT32 range;
    UINT32 modulation;
    UINT32 u4DramType;
#if defined(CC_MT5881)
	UINT32 u4Swap16Bit[4] = {1, 0, 2, 3};  // Mapping when 16 bits DQ swap.
#else
    UINT32 u4Swap16Bit[4] = {2, 3, 0, 1}; // Mapping when 16 bits DQ swap.
#endif
    UINT32 u4DQSth;
    UINT32 u4SysPLL = 648; // Unit in Mhz.
    UINT32 u4RecByte;
	
#if defined(CC_MT5399)||defined(CC_MT5882)
	U32 u4mempll_prediv_hex;
	U32 u4mempll_predivider[3] = {1, 2, 4}; //caution: u4mempll_predivider = 2^u4mempll_prediv_hex;
	U16 u2frequency1;
	U32 u4ssc_prd, u4ssc_delta;
#endif

    u4Bytes = (IS_FORCE32()) ? 2 : 4;
   
    u4DramType = TCMGET_DRAMTYPE();
        
    if ((u4DramType == DDR_II_x1) ||(u4DramType == DDR_II_x2) || (u4DramType == DDR_II_x3) || (u4DramType == DDR_II_x4))
    {
        Printf("DRAM: Type=%s, Clock=%d MHz, CL=%d, ChASize=%d MBytes, ChBSize=%d MBytes.\n", 
            ((u4DramType == DDR_II_x1) ? "DDR_II_x1" : ((u4DramType == DDR_II_x2) ? "DDR_II_x2" :((u4DramType == DDR_II_x3) ? "DDR_II_x3" : "DDR_II_x4"))), 
            (TCMGET_DDR_CLK()/1000000), 
            TCMGET_DDR_CL(), 
            TCMGET_CHANNELA_SIZE(), 
            TCMGET_CHANNELB_SIZE());

        // Reduce/Full strength, Write ODT, Read ODT setting.
        u4Val = DRAM_READ32(0x10);
        u4WODT = ((u4Val & 0x00440000) >> 16) & 0xFF;
        u4RODT = (DRAM_READ32(0x310) & 0xF);    
        Printf("DRAM: %s strength, DRAM ODT=%s, MainChip ODT=%s.\n", 
            ((u4Val & 0x00020000) ? "Reduce": "Full"), 
            ((u4WODT == 0x0004) ? "75 ohm" : 
            ((u4WODT == 0x0040) ? "150 ohm" : 
            ((u4WODT == 0x0044) ? "50 ohm" : "Disable"))),
            ((u4RODT ==  1) ? "75 ohm" : 
            (u4RODT ==  2) ? "150 ohm" : 
            (u4RODT ==  3) ? "50 ohm" : 
            (u4RODT ==  4) ? "37.5 ohm" : 
            (u4RODT ==  5) ? "25 ohm" : 
            (u4RODT ==  6) ? "30 ohm" : 
            (u4RODT ==  7) ? "21.5 ohm" : "Disable"));
    }
    else
    {
        Printf("DRAM: Type=%s, Clock=%d MHz, CL=%d, ChASize=%d MBytes, ChBSize=%d MBytes.\n",
            ((u4DramType == DDR_III_x1) ? "DDR_III_x1" : ((u4DramType == DDR_III_x2) ? "DDR_III_x2" : ((u4DramType == DDR_III_x3) ? "DDR_III_x3" : "DDR_III_x4"))), 
            (TCMGET_DDR_CLK()/1000000), 
            TCMGET_DDR_CL(), 
            TCMGET_CHANNELA_SIZE(), 
            TCMGET_CHANNELB_SIZE());

        // Reduce/Full strength, Write ODT, Read ODT setting.
#if defined(CC_MT5881)
		u4Val = 0x00002040;//!
        u4WODT = (u4Val & 0x0244) & 0x3FF;
		u4Val = (u4Val & 0x0002);
        u4RODT = (DRAM_DDRPHY_READ32(0x310) & 0xF);
#else
        u4Val = DRAM_READ32(0x10);
        u4WODT = ((u4Val & 0x02440000) >> 16) & 0x3FF;
        u4RODT = (DRAM_READ32(0x310) & 0xF);
		u4Val = (u4Val & 0x00020000);
#endif
        Printf("DRAM: Output driver=%s, DRAM ODT=%s, MainChip ODT=%s.\n", 
            (u4Val ? "RZQ/7": "RZQ/6"), 
            ((u4WODT == 0x0004) ? "60 ohm" : 
            ((u4WODT == 0x0040) ? "120 ohm" : 
            ((u4WODT == 0x0044) ? "40 ohm" : "Disable"))),
            ((u4RODT ==  1) ? "75 ohm" : 
            (u4RODT ==  2) ? "150 ohm" : 
            (u4RODT ==  3) ? "50 ohm" : 
            (u4RODT ==  4) ? "37.5 ohm" : 
            (u4RODT ==  5) ? "25 ohm" : 
            (u4RODT ==  6) ? "30 ohm" : 
            (u4RODT ==  7) ? "21.5 ohm" : "Disable"));
    }

    for (i=0; i<u4Bytes; i++)
    {
        // Byte 0 change to byte 2, byte 1 change to byte 3.
        // Byte 2 change to byte 0, byte 3 change to byte 1.
        u4DQSth = (IS_DDR_SWAP16BIT()) ? u4Swap16Bit[i] : i;

        u4RecByte = (IS_DRAM_CHANNELB_ACTIVE()) ? (i+4) : (i);
    
        i4Avg = (DRAM_READ32(0x3A0) >> (u4DQSth*8)) & 0xFF;
        u4Rec = HAL_READ32(RECORD_DQS_REG + ((u4RecByte /2)*4));
        u4Val = ((u4Rec >> ((u4RecByte & 0x1)*16)) & 0xFF);
        i4Min = ((UINT32)i4Avg >= u4Val) ? (i4Avg - u4Val) : -(u4Val - i4Avg);        
        i4Max = i4Avg + ((u4Rec >> (((u4RecByte & 0x1)*16) + 8)) & 0xFF);
        i4Size = i4Max- i4Min + 1;    
       
        Printf("DRAM: SW Byte %d : DQS(%d ~ %d), Size %d, Middle = %d.\n", 
            i, i4Min, i4Max, i4Size, i4Avg);
    }

#ifdef DRAM_HWONLINECALDQS
    Printf("DRAM: HW calibration %s.\n", (u4HwCalDQSEnable) ? "Enable":"Disable");

    if (u4HwCalDQSEnable)
    {   
        for (i=0; i<u4Bytes; i++)
        {
            // Byte 0 change to byte 2, byte 1 change to byte 3.
            // Byte 2 change to byte 0, byte 3 change to byte 1.
            u4DQSth = (IS_DDR_SWAP16BIT()) ? u4Swap16Bit[i] : i;
        
            i4DQMax = (INT32)DRAM_READ32(0x290);
            i4DQMax = (i4DQMax >> (8*u4DQSth)) & 0xFF;
            
            i4Min = (INT32)DRAM_READ32(0x294);
            i4Min = (i4Min >> (8*u4DQSth)) & 0xFF;
            
            i4Max = (INT32)DRAM_READ32(0x298);
            i4Max = (i4Max >> (8*u4DQSth)) & 0xFF;            
            
            i4Avg = (INT32)DRAM_READ32(0x29C);
            i4Avg = (i4Avg >> (8*u4DQSth)) & 0xFF;            
            
            if (i4DQMax != 0)
            {
                i4Min = i4Min - i4DQMax;
            }

            i4Size = i4Max- i4Min + 1;

            u4Val = DRAM_READ32(0x3A0);
            u4Val = (u4Val >> (8 * u4DQSth)) & 0x7F;
            
            Printf("DRAM: HW Byte %d : DQS(%d ~ %d), Size %d, Middle = %d, 0x73A0=%d.\n", 
                i, i4Min, i4Max, i4Size, i4Avg, u4Val);
        }
   
        u4Val = (UINT32)au4HwCaliBuffer;
        Printf("DRAM: HW calibration au4HwCaliBuffer addr = 0x%08X.\n", u4Val);

        u4Val = u4Val >> HWCALI_ALIGNBITS;

        Printf("DRAM: HW calibration Test addr=0x%08X ~ 0x%08X.\n", 
            ((u4Val + 2) << HWCALI_ALIGNBITS), 
            (((u4Val + 2) << HWCALI_ALIGNBITS) + (HWCALI_BUFFERSIZE*4)));        
    }
#endif

    Printf("DRAM: Memrw u4Cmemrw = %d secs.\n", u4Cmemrw);
    Printf("DRAM: Memrw compare fail abort => %s.\n", ((fgCmemrwAbort) ? "TRUE": "FALSE"));
    u4Val = MEMTEST_BUFFERSIZE;
    Printf("DRAM: Memrw au4M8K: 0x%08X ~ 0x%08X, length=%d Bytes.\n", 
        (UINT32)pu4M8K, ((UINT32)pu4M8K) + u4Val -1, u4Val);

    if (u4Cmemrw > 0)
    {    
        HAL_GetTime(& rNewTime);
        HAL_GetDeltaTime(& rDiffTime, & rOrgTime, & rNewTime);
        rOrgTime = rNewTime;
       
        u4TotalSecs = rNewTime.u4Seconds - u4InitSec;
        u4Hrs = u4TotalSecs/(3600);
        u4Mins = (u4TotalSecs - (u4Hrs*3600))/60;
        u4Secs = u4TotalSecs - (u4Hrs*3600) - (u4Mins*60);
        Printf("DRAM: Memrw Total Time=%d hrs:%d mins:%d secs.\n", 
            u4Hrs, u4Mins, u4Secs);
    }    
    
#if defined(CC_MT5396) || defined(CC_MT5368) || defined(CC_MT5389)

    UNUSED(u4Mux); 
    u4Val = CKGEN_READ32(0x194);  
    if (u4Val & 0x4000) 
    {
        Printf("DRAM: SSC Enable.\n");

        u4Val = CKGEN_READ32(0x194);
        u4Val = (u4Val >> 16) & 0xFFFF;
        modulation = (u4Val > 0) ? ((27000)/u4Val) : 0;

        u4Val = CKGEN_READ32(0x198);
        u4Val = u4Val & 0xFFFF;
        u4Val = u4Val * 10000;
        u4Val = u4Val*(54000/(modulation*16));
        u4Val = u4Val/(16*1024);
        range = (u4Val > 0) ? ((u4Val*54 )/(u4SysPLL*4)) : 0;
        range = range+1;
        
        Printf("DRAM: SSC range=%d, mod_freq=%d.\n", range, modulation);        
    }
    else
    {
        Printf("DRAM: SSC Disable.\n");
    }
#else //defined(CC_MT5396) || defined(CC_MT5368) || defined(CC_MT5389)

#if defined(CC_MT5398) 
	UNUSED(u4Mux); 
    u4Val = CKGEN_READ32(0x94);  
    if (u4Val & 0x4000) 
    {
        Printf("DRAM: SSC Enable.\n");

        u4Val = CKGEN_READ32(0x94);
        u4Val = (u4Val >> 16) & 0xFFFF;
		if(IS_XTAL_24MHZ())
		{
			modulation = (u4Val > 0) ? ((24000)/u4Val) : 0;
	        u4Val = CKGEN_READ32(0x98);
	        u4Val = u4Val & 0xFFFF;
	        u4Val = u4Val * 10000;
	        u4Val = u4Val*(48000/(modulation*16));
	        u4Val = u4Val/(16*1024);
	        range = (u4Val > 0) ? ((u4Val*48 )/(u4SysPLL*4)) : 0;
	        range = range+1;			
		}
		else
		{
			modulation = (u4Val > 0) ? ((27000)/u4Val) : 0;
	        u4Val = CKGEN_READ32(0x98);
	        u4Val = u4Val & 0xFFFF;
	        u4Val = u4Val * 10000;
	        u4Val = u4Val*(54000/(modulation*16));
	        u4Val = u4Val/(16*1024);
	        range = (u4Val > 0) ? ((u4Val*54 )/(u4SysPLL*4)) : 0;
	        range = range+1;
		}     
        Printf("DRAM: SSC range=%d, mod_freq=%d.\n", range, modulation);        
    }
    else
    {
        Printf("DRAM: SSC Disable.\n");
    }

#else //defined(CC_MT5398)
#if defined(CC_MT5880) || defined(CC_MT5881)
    UNUSED(u4Mux); 
    u4Val = CKGEN_READ32(0x94);  
    if (u4Val & 0x4000) 
    {
        Printf("DRAM: SSC Enable.\n");

        u4Val = CKGEN_READ32(0x94);
        u4Val = (u4Val >> 16) & 0xFFFF;

        u4DMSS = IS_XTAL_24MHZ() ? 24 : 27;
        modulation = (u4Val > 0) ? ((u4DMSS*1000/2)/u4Val) : 0;
        u4Val = CKGEN_READ32(0x98);
        u4Val = u4Val & 0xFFFF;

		u4Val = u4Val * 10000;
	    u4Val = u4Val*(u4DMSS*1000/(modulation*16));
	    u4Val = u4Val/(16*1024);
	    range = (u4Val > 0) ? ((u4Val*u4DMSS )/(u4SysPLL*4)) : 0;
	    range = range+1;

        Printf("DRAM: SSC range=%d, mod_freq=%d.\n", range, modulation);        
    }
    else
    {
        Printf("DRAM: SSC Disable.\n");
    }
    
#else
#if defined(CC_MT5399)||defined(CC_MT5882)
		u4Val = ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x2ac));
		if(mcCHK_BIT1(u4Val, 29)!=0)
		{
			u4Val = ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x2a0));
			u4mempll_prediv_hex = u4Val&0x03;
			ASSERT(u4mempll_prediv_hex <=2);
			u2frequency1 = ((TCMGET_DDR_CLK()/BASE_DDR_CLK)/2)/2;
			u4Val = ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x2ac));
			u4ssc_prd = u4Val & 0xFFFF;
			modulation = ((XTAL_MHZ/u4mempll_predivider[u4mempll_prediv_hex])*1000)/(u4ssc_prd*2);
			u4Val = ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x2b0));
			u4ssc_delta = (u4Val>>24)&0xFF;
			range = (10000*u4ssc_prd*u4ssc_delta)/(((u2frequency1*262144)/XTAL_MHZ)*2);
			range = range +1;
			Printf("DRAM: SSC range=%d, mod_freq=%d.\n", range, modulation); 
		}
		else
		{
			Printf("DRAM: SSC Disable.\n");
		}
#else
    u4SysPLL = 459; // Unit in Mhz.   
    // SSC parameter.    
    u4Val = CKGEN_READ32(0x190);    
    u4Mux = CKGEN_READ32(0x124);
    if (((u4Val & 0x40) == 0) && (u4Mux & 0x0800)) 
    {
        Printf("DRAM: SSC Enable.\n");

        u4Val = CKGEN_READ32(0x190);
        u4Val = (u4Val >> 16) & 0xFFF;
        modulation = (u4Val > 0) ? ((27000 /4)/u4Val) : 0;

        u4Val = CKGEN_READ32(0x194);
        u4Val = u4Val & 0xFFFF;
        range = (u4Val > 0) ? ((u4Val * 10000)/(modulation * (u4SysPLL*360)/1000)) : 0;
        Printf("DRAM: SSC range=%d, mod_freq=%d.\n", range, modulation);        
    }
    else
    {
        Printf("DRAM: SSC Disable.\n");
    }

#endif //defined(CC_MT5399)
#endif //defined(CC_MT5880) || defined(CC_MT5881)
#endif //defined(CC_MT5398)
#endif //defined(CC_MT5396) || defined(CC_MT5368) || defined(CC_MT5389)


    Printf("DRAM: Clk Domain ASYNC: %s\n", (IS_DDR_ASYNCON() ? "ON" : "OFF"));
    
    Printf("DRAM: Mempll input clk source is : %s\n", (IS_DDR_CTSMODE() ? "CTS mode" : "Current mode"));

    UNUSED(u4DMSS);
	UNUSED(u4SysPLL);
	UNUSED(u4Mux);
	UNUSED(u4Bytes);
	
    return 0;    
}
#endif //defined(CC_MT5890)

#if defined(CC_MT5890)
static void _DDR_Thread( void * pvArgs )
{
    UINT32 u4TotalSecs = 0;
    UINT32 u4Secs = 0;   
    UINT32 u4Mins = 0;
    UINT32 u4Hrs = 0;    
    UINT32 u4ThreadDelay = 5; // Unit in ms.
	UINT32 u4RingCounterError = 0;

    UNUSED(pvArgs);

    // Wait 300 ms to avoid race condition with CLI thread.
    x_thread_delay(300); 
    Printf("DRAM: _DDR_Thread is created.\n");

    HAL_GetTime(& rOrgTime);
    u4InitSec = rOrgTime.u4Seconds;

    while (1)
    {
        u4ThreadDelay = (u4Cmemrw > 0) ? 5 : 500;
        x_thread_delay(u4ThreadDelay); 
		
        // Check CHA DQS ring counter error.
        if ((DRAM_READ32(0x320) != 0) || (DRAM_READ32(0x324) != 0))
        {
            Printf("CHA Ring Counter Error 0x320=0x%08X, 0x324=0x%08X.\n", DRAM_READ32(0x320), DRAM_READ32(0x324));
            u4RingCounterError = 1;
        }

        // Check CHB DQS ring counter error.
        if (IS_DRAM_CHANNELB_SUPPORT())
        {
            TCMSET_CHANNELB_ACTIVE();
            if ((DRAM_READ32(0x320) != 0) || (DRAM_READ32(0x324) != 0))
            {
                Printf("CHB Ring Counter Error 0x320=0x%08X, 0x324=0x%08X.\n", DRAM_READ32(0x320), DRAM_READ32(0x324));
				u4RingCounterError = 1;
            }
            TCMSET_CHANNELA_ACTIVE();
        }

		// Check CHC DQS ring counter error.
        if (IS_DRAM_CHANNELC_SUPPORT())
        {
            TCMSET_CHANNELC_ACTIVE();
            if ((DRAM_READ32(0x320) != 0) || (DRAM_READ32(0x324) != 0))
            {
                Printf("CHC Ring Counter Error 0x320=0x%08X, 0x324=0x%08X.\n", DRAM_READ32(0x320), DRAM_READ32(0x324));
                u4RingCounterError = 1;
            }
            TCMSET_CHANNELA_ACTIVE();
        }
		
        if (u4RingCounterError)
        {
		    while(1);
        }
		
        if (u4Cmemrw)
        {                
            HAL_GetTime(& rNewTime);
            HAL_GetDeltaTime(& rDiffTime, & rOrgTime, & rNewTime);
            
            if (rDiffTime.u4Seconds >= u4Cmemrw)
            {                
                u4TotalSecs = rNewTime.u4Seconds - u4InitSec;
                u4Hrs = u4TotalSecs/(3600);
                u4Mins = (u4TotalSecs - (u4Hrs*3600))/60;
                u4Secs = u4TotalSecs - (u4Hrs*3600) - (u4Mins*60);
                Printf("DRAM: Total Time=%d hrs:%d mins:%d secs.\n", u4Hrs, u4Mins, u4Secs);
                                
                rOrgTime = rNewTime;
            }
        }        
    }
}
#else
static void _DDR_Thread( void * pvArgs )
{
    UINT32 u4run = 0;
    UINT32 u4TotalSecs = 0;
    UINT32 u4Secs = 0;   
    UINT32 u4Mins = 0;
    UINT32 u4Hrs = 0;    
    UINT32 u4ThreadDelay = 5; // Unit in ms.
#ifdef DRAM_HWONLINECALDQS   
    UINT32 u4HWCaliGap = 0;
    UINT32 u4HWCaliLoop = 0;
#endif
#ifdef HW_rDQS_OVER_SAMPLE
    UINT32 u4OverSampleErr1,u4OverSampleErr2,u4OverSamplePoint=0;
#endif

    UNUSED(pvArgs);

    // Wait 300 ms to avoid race condition with CLI thread.
    x_thread_delay(300); 
    Printf("DRAM: _DDR_Thread is created.\n");

    HAL_GetTime(& rOrgTime);
    u4InitSec = rOrgTime.u4Seconds;

    while (1)
    {
        // Thread delay is depended on memory read/write test being enabled or not.
        u4ThreadDelay = (u4Cmemrw > 0) ? 5 : 500;
        x_thread_delay(u4ThreadDelay); 

        // Check DQS ring counter error.
        if ((DRAM_READ32(0x2A8) != 0) || (DRAM_READ32(0x2AC) != 0))
        {
            Printf("Ring Counter error 0x2A8=0x%08X, 0x2AC=0x%08X.\n", 
                DRAM_READ32(0x2A8), DRAM_READ32(0x2AC));
            while(1);
        }

        // Check DQS ring counter error.
        if (IS_DRAM_CHANNELB_SUPPORT())
        {
            TCMSET_CHANNELB_ACTIVE();
            if ((DRAM_READ32(0x2A8) != 0) || (DRAM_READ32(0x2AC) != 0))
            {
                Printf("Ring Counter error 0x2A8=0x%08X, 0x2AC=0x%08X.\n", 
                    DRAM_READ32(0x2A8), DRAM_READ32(0x2AC));
                while(1);
            }
            TCMSET_CHANNELA_ACTIVE();
        }

        /*
            Memory test part.
        */
        if (u4Cmemrw)
        {
            if (!pu4M8K)
            {
                pu4M8K = (UINT32 *)x_mem_alloc(MEMTEST_BUFFERSIZE);
                if (pu4M8K == NULL)
                {
                    Printf("pu4M8K allocate fail.\n");
                    u4Cmemrw = 0;
                    continue;
                }
                pu4ChAM8K = pu4M8K;
            }
                
            HAL_GetTime(& rNewTime);
            HAL_GetDeltaTime(& rDiffTime, & rOrgTime, & rNewTime);
            
            if (rDiffTime.u4Seconds >= u4Cmemrw)
            {                
                u4TotalSecs = rNewTime.u4Seconds - u4InitSec;
                u4Hrs = u4TotalSecs/(3600);
                u4Mins = (u4TotalSecs - (u4Hrs*3600))/60;
                u4Secs = u4TotalSecs - (u4Hrs*3600) - (u4Mins*60);
                Printf("DRAM: Memrw Run=%d, Total Time=%d hrs:%d mins:%d secs.\n", 
                    u4run, u4Hrs, u4Mins, u4Secs);
                                
                rOrgTime = rNewTime;
                #ifdef HW_rDQS_OVER_SAMPLE

                    u4OverSampleErr1=DRAM_READ32(0x1A4);
                    u4OverSampleErr2=DRAM_READ32(0x1A8);
                    if((u4OverSampleErr1!=0)||(u4OverSampleErr2!=0))
                        Printf("[DRAM]!!!!!!Over Sample Error  0x1A4 = 0x%08x ,0x1A8 = 0x%08x  !!!!!!\n", u4OverSampleErr1,u4OverSampleErr2);

                    if(u4OverSamplePoint==0)
                    {
                        DRAM_WRITE32(0x22C,0x0F00 );
                        DRAM_WRITE32(0x22C,0x1F00 );
                        u4OverSamplePoint=1;
                    }
                    else
                    {
                         DRAM_WRITE32(0x22C,0x0700 );
                         DRAM_WRITE32(0x22C,0x1700 );
                         u4OverSamplePoint=0;
       	      }
                #endif

            }

#if defined(DRAM_MEMORY_CHECK)
            // LineBounce, ButterflyTest, and DSP PC counter test.
            _DDR_CpuDspMemTest();
#endif /* #if defined(DRAM_MEMORY_CHECK) */
#if defined(DRAM_MEMORY_AGENTTEST)
			_DDR_TestAgentMemTest();
#endif // #if defined(DRAM_MEMORY_AGENTTEST)
            u4run++;
            if (_DDR_IsDQSFail())
            {
                u4TotalSecs = rNewTime.u4Seconds - u4InitSec;
                u4Hrs = u4TotalSecs/(3600);
                u4Mins = (u4TotalSecs - (u4Hrs*3600))/60;
                u4Secs = u4TotalSecs - (u4Hrs*3600) - (u4Mins*60);
                Printf("DRAM: Memrw Run=%d, Total Time=%d hrs:%d mins:%d secs.\n", 
                    u4run, u4Hrs, u4Mins, u4Secs);

                if (fgCmemrwAbort)
                {
                    // Test error, show red at whole panel.
                    VDP_Memory_Test(0);
                    while(1);
                }
            }
        }

#ifdef DRAM_HWONLINECALDQS
        /*
            h/w calibration part.
        */
        if (u4HwCalDQSEnable)
        {
            ASSERT(u4ThreadDelay != 0);
            
            // Wait 1000 ms to start h/w calibration.               
            if (u4HWCaliGap ++ > (1000/u4ThreadDelay))
            {
                u4HWCaliGap = 0;

                switch(u4HWCaliState)
                {
                    case HWCALI_START:
                        u4HWCaliLoop++;
                        LOG(7, "DRAM: (%d) H/W Cal DQS start...\n", u4HWCaliLoop);

                        // Fill some information to check if memory violation by h/w calibration.
                        au4HwCaliBuffer[0] = (UINT32)au4HwCaliBuffer;
                        au4HwCaliBuffer[1] = (UINT32)0x53875363;
                        au4HwCaliBuffer[2] = (UINT32)0x12345678;
                        au4HwCaliBuffer[HWCALI_ACTUALBUFFERSIZE-3] = (UINT32)au4HwCaliBuffer;
                        au4HwCaliBuffer[HWCALI_ACTUALBUFFERSIZE-2] = (UINT32)0x53875363;
                        au4HwCaliBuffer[HWCALI_ACTUALBUFFERSIZE-1] = (UINT32)0x12345678;
                        
                        _DDR_DQSCalStart();                        
                        u4HWCaliState = HWCALI_PROCESS;
                        break;
                        
                    case HWCALI_PROCESS:
                        if (!_DDR_IsDQSCalDone())
                        {
                            LOG(0, "DRAM: (%d) Fail: H/W Cal DQS not finish.\n", u4HWCaliLoop);                            
                        }
                        else
                        {                        
                            LOG(7, "DRAM: (%d) Pass: H/W Cal DQS done.\n", u4HWCaliLoop);
                            _DDR_DQSCalDisable();

                            // Check if memory violation by h/w calibration.
                            if ((au4HwCaliBuffer[0] != (UINT32)au4HwCaliBuffer) ||
                                 (au4HwCaliBuffer[1] != (UINT32)0x53875363) ||
                                 (au4HwCaliBuffer[2] != (UINT32)0x12345678) ||
                                 (au4HwCaliBuffer[HWCALI_ACTUALBUFFERSIZE-3] != (UINT32)au4HwCaliBuffer) ||
                                 (au4HwCaliBuffer[HWCALI_ACTUALBUFFERSIZE-2] != (UINT32)0x53875363) ||
                                 (au4HwCaliBuffer[HWCALI_ACTUALBUFFERSIZE-1] != (UINT32)0x12345678))
                            {
                                Printf("DRAM: Memory violation !!\n");
                                ASSERT(0);
                            }
                            
                            _DDR_UpdateDQS();
                            u4HWCaliState = HWCALI_START;
                        }
                        break;

                    default:
                        break;
                }                
            }
        }
        
#endif // #ifdef DRAM_HWONLINECALDQS
        
    }
}

#endif //#if defined(CC_MT5890)

static void _DDR_ThreadInit(void)
{
   /*
        Memory Test Thread.
    */
    if (fgDRAMThreadInit)    
    {
        return;
    }
    
    fgDRAMThreadInit = TRUE;

    /* Dram Thread */
    (void)x_thread_create(&phDRAMThreadHdl, 
        "DDRThread", 1024,  128, _DDR_Thread, 0, NULL);
}

#if defined(CC_MT5399)||defined(CC_MT5882)
static INT32 _DDR_JitterCheck( INT32 i4Argc, const CHAR ** aszArgv )
{
	U8 ucsearch_state, ucdqs_dly, fgcurrent_value, fginitial_value, ucstart_period, ucend_period;
    U32 u4value, u4addr_array[4], u4sample_cnt, u4ones_cnt, u4frequency_hex;
    U16 u2real_freq, u2real_period, u2delay_cell_ps;
	U32 u4mempll_prediv_hex; 
	U32 u4mempll_predivider[3] = {1, 2, 4};

	fginitial_value = 0;
	ucstart_period = 0;
	ucend_period = 0;


    if (!IS_DRAM_CHANNELB_ACTIVE())
	{
	    u4addr_array[0] = mcSET_PHY_REG_ADDR(0x048);
	    u4addr_array[1] = mcSET_PHY_REG_ADDR(0x04c);
	}
	else
	{
	    u4addr_array[0] = mcSET_PHY_REG_ADDR(0x298);
	    u4addr_array[1] = mcSET_PHY_REG_ADDR(0x29c);
	}

	u4addr_array[2] = mcSET_DRAMC_REG_ADDR(DRAMC_REG_TOGGLE_CNT);
	u4addr_array[3] = mcSET_DRAMC_REG_ADDR(DRAMC_REG_DQS_ERR_CNT);

    //Enable DQ eye scan
    //RG_??_RX_EYE_SCAN_EN
    //RG_??_RX_VREF_EN 
    //RG_??_RX_VREF_OP_EN 
    //RG_??_RX_SMT_EN
    u4value = ucDram_Register_Read(u4addr_array[0]);
    u4value = u4value | 0x0000e000;
    ucDram_Register_Write(u4addr_array[0], u4value);

    u4value = ucDram_Register_Read(u4addr_array[1]);
    u4value = u4value | 0xffff0000;
    ucDram_Register_Write(u4addr_array[1], u4value);

    //Enable MIOCK jitter meter mode (RG_??_RX_DQS_MIOCK_SEL=1, RG_RX_MIOCK_JIT_EN=1)
    //RG_??_RX_DQS_MIOCK_SEL=1
    u4value = ucDram_Register_Read(u4addr_array[0]);
    u4value = u4value | 0x00001000;
    ucDram_Register_Write(u4addr_array[0], u4value);

    //RG_RX_MIOCK_JIT_EN=1
    u4value = ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DCBLN));
    mcSET_BIT(u4value, POS_DCBLN_RX_MIOCK_JIT_EN);
    ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DCBLN), u4value);

    ucsearch_state = 0;

    //Disable DQ eye scan (b'1), for counter clear
    u4value = ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DCBLN));
    mcCLR_BIT(u4value, POS_DCBLN_RX_EYE_SCAN_EN);
    ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DCBLN), u4value);

    for (ucdqs_dly=0; ucdqs_dly<128; ucdqs_dly++)
    {
        //Set DQS delay (RG_??_RX_DQS_EYE_DLY)
        u4value = ucDram_Register_Read(u4addr_array[0]);
        mcSET_FIELD(u4value, ucdqs_dly, 0x007f0000, 16);
        ucDram_Register_Write(u4addr_array[0], u4value);

        //Reset eye scan counters (reg_sw_rst): 1 to 0
        u4value = ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DCBLN));
        mcSET_BIT(u4value, POS_DCBLN_REG_SW_RST);
        ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DCBLN), u4value);
        mcCLR_BIT(u4value, POS_DCBLN_REG_SW_RST);
        ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DCBLN), u4value);

        //Enable DQ eye scan (b'1)
        u4value = ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DCBLN));
        mcSET_BIT(u4value, POS_DCBLN_RX_EYE_SCAN_EN);
        ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DCBLN), u4value);

        // 2ns/sample, here we delay 1ms about 500 samples
        HAL_Delay_us(1000);

        //Disable DQ eye scan (b'1), for counter latch
        u4value = ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DCBLN));
        mcCLR_BIT(u4value, POS_DCBLN_RX_EYE_SCAN_EN);
        ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DCBLN), u4value);

        //Read the counter values from registers (toggle_cnt*, dqs_err_cnt*)
        u4sample_cnt = ucDram_Register_Read(u4addr_array[2]);
        u4ones_cnt = ucDram_Register_Read(u4addr_array[3]);

        Printf("%3d | %8x --%8x \n", ucdqs_dly, u4sample_cnt, u4ones_cnt); 

        //change to boolean value
        if (u4ones_cnt < (u4sample_cnt/2))
        {
            fgcurrent_value = 0;
        }
        else
        {
            fgcurrent_value = 1;
        }
        
        if (ucsearch_state==0)
        {
            //record initial value at the beginning
            fginitial_value = fgcurrent_value;            
            ucsearch_state = 1;
        }
        else if (ucsearch_state==1)
        {
            // check if change value
            if (fgcurrent_value != fginitial_value)
            {
                // start of the period
                fginitial_value = fgcurrent_value;
                ucstart_period = ucdqs_dly;
                ucsearch_state = 2;
            }
        }
        else if (ucsearch_state==2)
        {
            // check if change value
            if (fgcurrent_value != fginitial_value)
            {
                fginitial_value = fgcurrent_value;
                ucsearch_state = 3;
            }
        }
        else if (ucsearch_state==3)
        {
            // check if change value
            if (fgcurrent_value != fginitial_value)
            {
                // end of the period, break the loop
                ucend_period = ucdqs_dly;
                ucsearch_state = 4;
                break;
            }
        }
        else
        {
            //nothing
        }
    }

    //Calculate 1 delay cell = ? ps
    // 1T = ? delay cell
	TCMSET_DLYCELL_PERT(ucend_period-ucstart_period);
    // 1T = ? ps
    u4value = ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x2a0));
    u4frequency_hex = mcGET_FIELD(u4value, 0x00007f00, 8);
	u4mempll_prediv_hex = mcGET_FIELD(u4value, 0x00000003, 0);
    u2real_freq = (U16) ((u4frequency_hex+1)*(XTAL_MHZ/u4mempll_predivider[u4mempll_prediv_hex])*2);
    u2real_period = (U16) ((U32)1000000/(U32)u2real_freq);
    //calculate delay cell time
    u2delay_cell_ps = u2real_period / TCMGET_DLYCELL_PERT;

    //RG_RX_MIOCK_JIT_EN=0
    //u4value = ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DCBLN), &u4value);
    //mcCLR_BIT(u4value, POS_DCBLN_RX_MIOCK_JIT_EN);
    //ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DCBLN), u4value);

    Printf("===============================================================================\n");
    Printf("    MIOCK jitter meter \n");
    Printf("===============================================================================\n");
    Printf("1T = (%d-%d) = %d delay cells\n", ucend_period, ucstart_period, TCMGET_DLYCELL_PERT);
    Printf("Clock frequency = %d MHz, Clock period = %d ps, 1 delay cell = %d ps\n", u2real_freq, u2real_period, u2delay_cell_ps);

	UNUSED(u2delay_cell_ps);

	return 0;
}
#else
#ifdef CC_MT5890
static INT32 _DDR_JitterCheck( INT32 i4Argc, const CHAR ** aszArgv )
{
    U8 ucsearch_state, ucdqs_dly, fgcurrent_value, fginitial_value, ucstart_period, ucend_period;
    U32 u4value, u4addr_array[3], u4sample_cnt, u4ones_cnt;
    U16 u2real_freq, u2real_period, u2delay_cell_ps;
    U32 /*u4clock,u4frequency1,*/ u4counter;
    U8 ii, ucloop;

	fginitial_value = 0;
	ucstart_period = 0;
	ucend_period = 0;

    // [R] to check every 2-byte, apply gating. Check default value...(17.5ps)
    //TBD (give default values)
    TCMSET_DLYCELL_PERT(86); // 86 is for DDR-1333    

#if 0 // for process check, every 2-bytes to see if variation
    if (IS_DRAM_CHANNELC_ACTIVE()) // chc
	{
        ucloop = DQS_NUMBER_CHC / 2;
	}
    else // cha
    {
        ucloop = DQS_NUMBER / 2;
    }
#else
    ucloop = 1;
#endif

  for (ii=0; ii<ucloop; ii++) 
  {
  //============================================
    if (ii==0) // byte 0/1
    {
        u4addr_array[0] = mcSET_PHY_REG_ADDR(0x134);
        u4addr_array[1] = mcSET_PHY_REG_ADDR(0x130);
        u4addr_array[2] = mcSET_PHY_REG_ADDR(0x138);
    }
    else // byte 2/3
	{
        u4addr_array[0] = mcSET_PHY_REG_ADDR(0x234);
        u4addr_array[1] = mcSET_PHY_REG_ADDR(0x230);
        u4addr_array[2] = mcSET_PHY_REG_ADDR(0x238);
	}

    // Bypass DQS glitch-free mode
    // RG_RX_*RDQ_EYE_DLY_DQS_BYPASS_B** : [20]
    u4value = ucDram_Register_Read(u4addr_array[2]);
    mcSET_BIT(u4value, 20);
    ucDram_Register_Write(u4addr_array[2], u4value);    

    //Enable DQ eye scan (2-byte base)
    //RG_RX_*RDQ_EYE_EN_B** : [22]
    //RG_RX_*RDQ_VREF_EN_B** : [8]
    //RG_RX_*RDQ_VREF_OP_EN_B** : [7]    
    u4value = ucDram_Register_Read(u4addr_array[0]);
    //u4value = u4value | 0x00400180;
    mcSET_BIT(u4value, 22);
    mcSET_BIT(u4value, 8);
    mcSET_BIT(u4value, 7);
    ucDram_Register_Write(u4addr_array[0], u4value);

    //RG_RX_*RDQ_SMT_EN_B** : [10]
    u4value = ucDram_Register_Read(u4addr_array[1]);
    //u4value = u4value | 0x00000400;
    mcSET_BIT(u4value, 10);
    ucDram_Register_Write(u4addr_array[1], u4value);

    //Enable MIOCK jitter meter mode (RG_RX_*RDQ_EYE_MCK_SEL_B**=1, RG_RX_MIOCK_JIT_EN=1)
    //RG_RX_*RDQ_EYE_MCK_SEL_B**=1 : [9]
    u4value = ucDram_Register_Read(u4addr_array[0]);
    //u4value = u4value | 0x00000200;
    mcSET_BIT(u4value, 9);
    ucDram_Register_Write(u4addr_array[0], u4value);

    //RG_RX_MIOCK_JIT_EN=1
    u4value = ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DCBLN));
    mcSET_BIT(u4value, POS_DCBLN_RX_MIOCK_JIT_EN);
    ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DCBLN), u4value);

    ucsearch_state = 0;

    //Disable DQ eye scan (b'1), for counter clear
    u4value = ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DCBLN));
    mcCLR_BIT(u4value, POS_DCBLN_RX_EYE_SCAN_EN);
    ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DCBLN), u4value);

    for (ucdqs_dly=0; ucdqs_dly<128; ucdqs_dly++)
    {
        //Set DQS delay (RG_RX_*RDQ_EYE_DQS_DLY_B**)
        u4value = ucDram_Register_Read(u4addr_array[0]);
        mcSET_FIELD(u4value, ucdqs_dly, 0x003f8000, 15);
        ucDram_Register_Write(u4addr_array[0], u4value);

        u4counter=0;
        
        do
        {
        //Reset eye scan counters (reg_sw_rst): 1 to 0
        u4value = ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DCBLN));
        mcSET_BIT(u4value, POS_DCBLN_REG_SW_RST);
        ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DCBLN), u4value);
        mcCLR_BIT(u4value, POS_DCBLN_REG_SW_RST);
        ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DCBLN), u4value);

        //Enable DQ eye scan (b'1)
        u4value = ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DCBLN));
        mcSET_BIT(u4value, POS_DCBLN_RX_EYE_SCAN_EN);
        ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DCBLN), u4value);

        // 2ns/sample, here we delay 1ms about 500 samples
		HAL_Delay_us(1000);

        //Disable DQ eye scan (b'1), for counter latch
        u4value = ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DCBLN));
        mcCLR_BIT(u4value, POS_DCBLN_RX_EYE_SCAN_EN);
        ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DCBLN), u4value);

		//mcDELAY_us(1000);

        //Read the counter values from registers (toggle_cnt*, dqs_err_cnt*)
            u4sample_cnt = ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_TOGGLE_CNT));
            u4ones_cnt = ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DQS_ERR_CNT));

        Printf("%3d | %8x --%8x \n", ucdqs_dly, u4sample_cnt, u4ones_cnt); 

            u4counter++;
            if(u4counter>5)
            {
                Printf("Toggle_cnt has mistake in function DramcMiockJmeter()!\n");
                while(1);
            }
        }while(((u4sample_cnt+10)<u4ones_cnt) || (u4sample_cnt==0));

        //change to boolean value
        if (u4ones_cnt < (u4sample_cnt/2))
        {
            fgcurrent_value = 0;
        }
        else
        {
            fgcurrent_value = 1;
        }
        
        if (ucsearch_state==0)
        {
            //record initial value at the beginning
            fginitial_value = fgcurrent_value;            
            ucsearch_state = 1;
        }
        else if (ucsearch_state==1)
        {
            // check if change value
            if (fgcurrent_value != fginitial_value)
            {
                // start of the period
                fginitial_value = fgcurrent_value;
                ucstart_period = ucdqs_dly;
                ucsearch_state = 2;
            }
        }
        else if (ucsearch_state==2)
        {
            // check if change value
            if (fgcurrent_value != fginitial_value)
            {
                fginitial_value = fgcurrent_value;
                ucend_period = ucdqs_dly;
                ucsearch_state = 3;
            }
        }
        else if (ucsearch_state==3)
        {
            // check if change value
            if (fgcurrent_value != fginitial_value)
            {
                // end of the period, break the loop
                ucend_period = ucdqs_dly;
                ucsearch_state = 4;
                break;
            }
        }
        else
        {
            //nothing
        }
    }

    //Calculate 1 delay cell = ? ps
    // 1T = ? delay cell
    if (ucsearch_state != 4) // not cover full 1T for all delay cells, only calculate by 0.5T
    {
        Printf("WARNING: Not cover full 1T for all delay cells.\n");        
        TCMSET_DLYCELL_PERT((ucend_period-ucstart_period)*2);        
    }
    else
    {
	TCMSET_DLYCELL_PERT(ucend_period-ucstart_period);
    }

    // 1T = ? ps
#if 0
    u4value = ucDram_Register_Read(mcSET_PHY_REG_ADDR_CHB(0x408));
    u4frequency_hex = mcGET_FIELD(u4value, 0x000007f0, 4); // get only integer part
    
    u2real_freq = (U16) ((u4frequency_hex+1)*(XTAL_MHZ*2);

    u4clock = (TCMGET_DDR_CLK()/BASE_DDR_CLK)/2;
    u4frequency1 = u4clock / 2;
    u2real_freq = u2real_freq + (U16)((u4frequency1-(u4frequency_hex+1)*XTAL_MHZ)*2);
#else
    u2real_freq = (U16) ((TCMGET_DDR_CLK()/BASE_DDR_CLK)/2);
#endif

    u2real_period = (U16) ((U32)1000000/(U32)u2real_freq);

    //calculate delay cell time
    u2delay_cell_ps = u2real_period / TCMGET_DLYCELL_PERT;

    //??
    //RG_RX_MIOCK_JIT_EN=0
    //u4value = ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DCBLN), &u4value);
    //mcCLR_BIT(u4value, POS_DCBLN_RX_MIOCK_JIT_EN);
    //ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DCBLN), u4value);

    // Disable SMT_EN
    //RG_RX_*RDQ_SMT_EN_B** : [10]
    u4value = ucDram_Register_Read(u4addr_array[1]);
    u4value = u4value & 0xfffffbff;
    ucDram_Register_Write(u4addr_array[1], u4value);

    Printf("===============================================================================\n");
    Printf("    MIOCK jitter meter \n");
    Printf("===============================================================================\n");
    Printf("1T = (%d-%d) = %d delay cells\n", ucend_period, ucstart_period, TCMGET_DLYCELL_PERT);
    Printf("Clock frequency = %d MHz, Clock period = %d ps, 1 delay cell = %d ps\n", u2real_freq, u2real_period, u2delay_cell_ps);

	UNUSED(u2delay_cell_ps);
  }
  return 0;
}
#else
static INT32 _DDR_JitterCheck( INT32 i4Argc, const CHAR ** aszArgv )
{    
    INT32 i4PLL, i4MCK;
    UINT32 u4Val;
    UINT16 u2PllJitter;
    UINT16 u2MckJitter;
    
    Printf("Jitter checking...\n");  

    // check PLLCK first.
    for (i4PLL=0x5F; i4PLL >= 0; i4PLL--)
    {       
        u4Val = 0x8000FFFF;
        u4Val |= (UINT32)(i4PLL << 24);
        DRAM_WRITE32(0x228, u4Val);
        DRAM_WRITE32(0x228, u4Val | 0x00800000);

        x_thread_delay(10);

        u2PllJitter = (UINT16)(DRAM_READ32(0x2B4) & 0xFFFF);
        DRAM_WRITE32(0x228, 0);
        //if ((u2PllJitter > 2) && (u2PllJitter < 0xFFFD)) 
        {
           Printf("PLL=%03d, MCK=000, Jitter=0x%04X.\n", i4PLL, u2PllJitter);
        }
    }  

    // check MCK.
    for (i4MCK=1; i4MCK<=0x5F; i4MCK++)
    {        
        u4Val = 0x8000FFFF;
        u4Val |= (UINT32)(i4MCK << 16);
        DRAM_WRITE32(0x228, u4Val);
        DRAM_WRITE32(0x228, u4Val | 0x00800000);

        x_thread_delay(10);

        u2MckJitter = (UINT16)(DRAM_READ32(0x2B4) & 0xFFFF);
        DRAM_WRITE32(0x228, 0);
        //if ((u2MckJitter > 2) && (u2MckJitter < 0xFFFD))
        {
            Printf("PLL=000, MCK=%03d, Jitter=0x%04X.\n", i4MCK, u2MckJitter);
        }
    }     
    
    return 0;
}
#endif //#ifdef CC_MT5890
#endif //#ifdef CC_MT5399

#if defined(CC_MT5399)||defined(CC_MT5882)
//-------------------------------------------------------------------------
/** DramcRxEyeScan
 *  start the rx dq eye scan.
 *  @param p                Pointer of context created by DramcCtxCreate.
 *  @param dq_no            (U8): 0~31.
 *  @retval status          (DRAM_STATUS_T): DRAM_OK or DRAM_FAIL 
 */
//-------------------------------------------------------------------------
DRAM_STATUS_T DramcRxEyeScan(U8 dq_no)
{
    U8 ucidx_dqs, ucidx_dq, ucdqs_dly;
    S8 s1vref, s1dq_dly;
    U32 u4value, u4err_value = 0xffffffff, u4addr_array[4], u4sample_cnt, u4error_cnt;
	U32 ii;

    if (dq_no <= 15)
    {
        if (!IS_DRAM_CHANNELB_ACTIVE())
        {
            u4addr_array[0] = mcSET_PHY_REG_ADDR(0x048);
            u4addr_array[1] = mcSET_PHY_REG_ADDR(0x04c);
        }
        else
        {
            u4addr_array[0] = mcSET_PHY_REG_ADDR(0x298);
            u4addr_array[1] = mcSET_PHY_REG_ADDR(0x29c);
        }

        u4addr_array[2] = mcSET_DRAMC_REG_ADDR(DRAMC_REG_TOGGLE_CNT);
        u4addr_array[3] = mcSET_DRAMC_REG_ADDR(DRAMC_REG_DQ_ERR_CNT);
        ucidx_dqs =  dq_no / 8;
    }
    else
    {
        if (!IS_DRAM_CHANNELB_ACTIVE())
        {
            u4addr_array[0] = mcSET_PHY_REG_ADDR(0x248);
            u4addr_array[1] = mcSET_PHY_REG_ADDR(0x24c);
        }
        else
        {
            u4addr_array[0] = mcSET_PHY_REG_ADDR(0x448);
            u4addr_array[1] = mcSET_PHY_REG_ADDR(0x44c);
        }

        u4addr_array[2] = mcSET_DRAMC_REG_ADDR(DRAMC_REG_TOGGLE_CNT_2);
        u4addr_array[3] = mcSET_DRAMC_REG_ADDR(DRAMC_REG_DQ_ERR_CNT_2);
        ucidx_dqs =  (dq_no-16) / 8;
    }

    // check if SoC platform has "%" operation?!
    ucidx_dq =  dq_no % 16;
    	
    //Enable DQ eye scan
    //RG_??_RX_EYE_SCAN_EN
    //RG_??_RX_VREF_EN 
    //RG_??_RX_VREF_OP_EN 
    //RG_??_RX_SMT_EN
    u4value = ucDram_Register_Read(u4addr_array[0]);
    u4value = u4value | 0x0000e000;
    ucDram_Register_Write(u4addr_array[0], u4value);

    u4value = ucDram_Register_Read(u4addr_array[1]);
    u4value = u4value | 0xffff0000;
    ucDram_Register_Write(u4addr_array[1], u4value);

    //Disable MIOCK jitter meter mode (RG_??_RX_DQS_MIOCK_SEL=0, RG_RX_MIOCK_JIT_EN=0)
    //RG_??_RX_DQS_MIOCK_SEL=0
    u4value = ucDram_Register_Read(u4addr_array[0]);
    u4value = u4value & 0xffffefff;
    ucDram_Register_Write(u4addr_array[0], u4value);

    //RG_RX_MIOCK_JIT_EN=0
    u4value = ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DCBLN));
    mcCLR_BIT(u4value, POS_DCBLN_RX_MIOCK_JIT_EN);
    ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DCBLN), u4value);

    //select DQ to be scanned (0~15)
    //DDRPHY: RG_??_RX_DQ_EYE_SEL
    u4value = ucDram_Register_Read(u4addr_array[0]);
    mcSET_FIELD(u4value, ucidx_dq, 0x0f000000, 24);
    ucDram_Register_Write(u4addr_array[0], u4value); 
    //DRAMC: RG_RX_DQ_EYE_SEL (0~15)
    u4value = ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DCBLN));
    mcSET_FIELD(u4value, ucidx_dq, 0x000000f0, 4);
    ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DCBLN), u4value); 
 	
    //Select DQS to be scanned (0 or 1)
    //RG_??_RX_DQS_EYE_SEL
    u4value = ucDram_Register_Read(u4addr_array[0]);
    mcSET_FIELD(u4value, ucidx_dqs, 0x00800000, 23);
    ucDram_Register_Write(u4addr_array[0], u4value); 	

    Printf("===============================================================================\n");
    Printf("    DQ RX eye scan (channel=%d(2:cha, 3:chb), dq_%d)\n", IS_DRAM_CHANNELB_ACTIVE(), dq_no);
    Printf("===============================================================================\n");

	//Set Vref voltage
    u4value = ucDram_Register_Read(u4addr_array[0]);
    mcSET_FIELD(u4value, 8, 0x0000000f, 0);
    ucDram_Register_Write(u4addr_array[0], u4value);
	//Set DQ delay (RG_??_RX_DQ_EYE_DLY)
    u4value = ucDram_Register_Read(u4addr_array[0]);
    mcSET_FIELD(u4value, 0, 0x00000f00, 8);
    ucDram_Register_Write(u4addr_array[0], u4value);
	//set dqs delay
	u4value = ucDram_Register_Read(u4addr_array[0]);
	mcSET_FIELD(u4value, 9, 0x007f0000, 16);
	ucDram_Register_Write(u4addr_array[0], u4value);

	Printf("vref, dqs_delay, dq_delay: reg 0x%08x=0x%08x \n", u4addr_array[0], ucDram_Register_Read(u4addr_array[0]));

	for(ii=0;ii<10;ii++)
	{
		//Reset eye scan counters (reg_sw_rst): 1 to 0
		u4value = ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DCBLN));
		mcSET_BIT(u4value, POS_DCBLN_REG_SW_RST);
		ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DCBLN), u4value);
		mcCLR_BIT(u4value, POS_DCBLN_REG_SW_RST);
		ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DCBLN), u4value);

		//Enable DQ eye scan (b'1)
		u4value = ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DCBLN));
		mcSET_BIT(u4value, POS_DCBLN_RX_EYE_SCAN_EN);
		ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DCBLN), u4value);

		// enable TE2, audio pattern
		//u4err_value = DramcEngine2(TE_OP_WRITE_READ_CHECK, 0xaa550000, 0x30000000, 0x00000100);                        
		HAL_Delay_us(100);
		//Disable DQ eye scan (b'1), for counter latch
		u4value = ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DCBLN));
		mcCLR_BIT(u4value, POS_DCBLN_RX_EYE_SCAN_EN);
		ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DCBLN), u4value);

		//Read the counter values from registers (toggle_cnt*, dqs_err_cnt*); *_cnt is for (DQS0,1) and *_cnt_2 is for (DQS2,3)
		u4sample_cnt = ucDram_Register_Read(u4addr_array[2]);
		u4error_cnt = ucDram_Register_Read(u4addr_array[3]);

		Printf("%4d,", u4error_cnt);         	
	}

	Printf("\n");

	for (s1vref=15; s1vref>=0; s1vref--)
    {
        //Set Vref voltage
        u4value = ucDram_Register_Read(u4addr_array[0]);
        mcSET_FIELD(u4value, s1vref, 0x0000000f, 0);
        ucDram_Register_Write(u4addr_array[0], u4value); 

        //Wait for Vref settles down, 1ms is enough
        HAL_Delay_us(10);

        //Set DQS delay (RG_??_RX_DQS_EYE_DLY) to 0
        u4value = ucDram_Register_Read(u4addr_array[0]);
        mcSET_FIELD(u4value, 0, 0x007f0000, 16);
        ucDram_Register_Write(u4addr_array[0], u4value);  		
 		
        //Disable DQ eye scan (b'1), for counter clear
        u4value = ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DCBLN));
        mcCLR_BIT(u4value, POS_DCBLN_RX_EYE_SCAN_EN);
        ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DCBLN), u4value);

        for (s1dq_dly=15; s1dq_dly>=0; s1dq_dly--)
        {
            //Set DQ delay (RG_??_RX_DQ_EYE_DLY)
            u4value = ucDram_Register_Read(u4addr_array[0]);
            mcSET_FIELD(u4value, s1dq_dly, 0x00000f00, 8);
            ucDram_Register_Write(u4addr_array[0], u4value);

            //Reset eye scan counters (reg_sw_rst): 1 to 0
            u4value = ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DCBLN));
            mcSET_BIT(u4value, POS_DCBLN_REG_SW_RST);
            ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DCBLN), u4value);
            mcCLR_BIT(u4value, POS_DCBLN_REG_SW_RST);
            ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DCBLN), u4value);

            //Enable DQ eye scan (b'1)
            u4value = ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DCBLN));
            mcSET_BIT(u4value, POS_DCBLN_RX_EYE_SCAN_EN);
            ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DCBLN), u4value);

            // enable TE2, audio pattern
            //u4err_value = DramcEngine2(TE_OP_WRITE_READ_CHECK, 0xaa550000, 0x30000000, 0x00000100);                        
			HAL_Delay_us(100);
            //Disable DQ eye scan (b'1), for counter latch
            u4value = ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DCBLN));
            mcCLR_BIT(u4value, POS_DCBLN_RX_EYE_SCAN_EN);
            ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DCBLN), u4value);

            //Read the counter values from registers (toggle_cnt*, dqs_err_cnt*); *_cnt is for (DQS0,1) and *_cnt_2 is for (DQS2,3)
            u4sample_cnt = ucDram_Register_Read(u4addr_array[2]);
            u4error_cnt = ucDram_Register_Read(u4addr_array[3]);

            Printf("%4d,", u4error_cnt);         
        }

        //Set DQ delay (RG_??_RX_DQ_EYE_DLY) to 0
        u4value = ucDram_Register_Read(u4addr_array[0]);
        mcSET_FIELD(u4value, 0, 0x00000f00, 8);
        ucDram_Register_Write(u4addr_array[0], u4value);

        //Disable DQ eye scan (b'1), for counter clear
        u4value = ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DCBLN));
        mcCLR_BIT(u4value, POS_DCBLN_RX_EYE_SCAN_EN);
        ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DCBLN), u4value);

        //for eye scan, dqs only need to scan 0~31
        for (ucdqs_dly=0; ucdqs_dly<32; ucdqs_dly++)
        {
            //Set DQS delay (RG_??_RX_DQS_EYE_DLY)
            u4value = ucDram_Register_Read(u4addr_array[0]);
            mcSET_FIELD(u4value, ucdqs_dly, 0x007f0000, 16);
            ucDram_Register_Write(u4addr_array[0], u4value);

            //Reset eye scan counters (reg_sw_rst): 1 to 0
            u4value = ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DCBLN));
            mcSET_BIT(u4value, POS_DCBLN_REG_SW_RST);
            ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DCBLN), u4value);
            mcCLR_BIT(u4value, POS_DCBLN_REG_SW_RST);
            ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DCBLN), u4value);

            //Enable DQ eye scan (b'1)
            u4value = ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DCBLN));
            mcSET_BIT(u4value, POS_DCBLN_RX_EYE_SCAN_EN);
            ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DCBLN), u4value);

            //reset phy R_DMPHYRST: 0xf0[28] 
            // 0x0f0[28] = 1 -> 0
            //u4value = ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_PHYCTL1));
            //mcSET_BIT(u4value, POS_PHYCTL1_PHYRST);
            //ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_PHYCTL1), u4value);
            //delay 10ns, 1ms here
            //HAL_Delay_us(1);
            //mcCLR_BIT(u4value, POS_PHYCTL1_PHYRST);
            //ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_PHYCTL1), u4value);

            // read data counter reset
            // 0x0f4[25] = 1 -> 0
            //u4value = ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_GDDR3CTL1));
            //mcSET_BIT(u4value, POS_GDDR3CTL1_RDATRST);
            //ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_GDDR3CTL1), u4value);
            //delay 10ns, 1ms here
            //HAL_Delay_us(1);                
            //mcCLR_BIT(u4value, POS_GDDR3CTL1_RDATRST);
            //ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_GDDR3CTL1), u4value);

			HAL_Delay_us(100);
            // enable TE2, audio pattern
            //u4err_value = DramcEngine2(TE_OP_WRITE_READ_CHECK, 0xaa550000, 0x30000000, 0x00000100);

            //Disable DQ eye scan (b'1), for counter latch
            u4value = ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DCBLN));
            mcCLR_BIT(u4value, POS_DCBLN_RX_EYE_SCAN_EN);
            ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DCBLN), u4value);

            //Read the counter values from registers (toggle_cnt*, dqs_err_cnt*); *_cnt is for (DQS0,1) and *_cnt_2 is for (DQS2,3)
            u4sample_cnt = ucDram_Register_Read(u4addr_array[2]);
            u4error_cnt = ucDram_Register_Read(u4addr_array[3]);

            Printf("%4d,", u4error_cnt);             
        }
        Printf("\n");
    }

	UNUSED(u4err_value);
	UNUSED(u4sample_cnt);
	UNUSED(u4error_cnt);

    //RG_RX_MIOCK_JIT_EN=0
    //u4value = ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DCBLN), &u4value);
    //mcCLR_BIT(u4value, POS_DCBLN_RX_MIOCK_JIT_EN);
    //ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DCBLN), u4value);

	return DRAM_OK;
	
    // log example
    /*
 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 504, 502, 500, 495, 494, 227,  61,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   3, 133, 226, 354, 378, 378, 381, 424, 480, 504, 506, 506, 506, 506, 506,
 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 504, 502, 500, 500, 429, 429,  53,  18,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  73, 197, 307, 378, 378, 385, 422, 493, 506, 506, 506, 506, 506, 506,
 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 502, 500, 500, 500, 225, 228,  13,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   9, 182, 200, 378, 378, 411, 450, 506, 506, 506, 506, 506, 506, 506,
 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 500, 500, 500, 463,  59,  56,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 102, 193, 348, 382, 436, 476, 506, 506, 506, 506, 506, 506, 506,
 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 500, 500, 484, 343,  16,  15,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  17, 129, 268, 384, 444, 503, 506, 506, 506, 506, 506, 506, 506,
 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 500, 500, 401, 177,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   1,  84, 238, 372, 467, 506, 506, 506, 506, 506, 506, 506, 506,
 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 504, 500, 223,  38,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  15, 261, 365, 499, 506, 506, 506, 506, 506, 506, 506, 506,
 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 496,  81,   7,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  11, 264, 372, 506, 506, 506, 506, 506, 506, 506, 506, 506,
 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 478, 121,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   1,  45, 241, 322, 475, 506, 506, 506, 506, 506, 506, 506, 506,
 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 492, 220,  37,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  10,  80, 195, 308, 470, 506, 506, 506, 506, 506, 506, 506, 506,
 512, 512, 512, 512, 512, 512, 512, 512, 506, 506, 506, 506, 466, 324, 103,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   2,  41, 156, 205, 315, 494, 506, 506, 506, 506, 506, 506, 506, 506,
 512, 512, 512, 512, 512, 512, 512, 512, 512, 506, 506, 496, 409, 364, 228,  17,  13,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   5,  74, 187, 245, 349, 502, 506, 506, 506, 506, 506, 506, 506, 506,
 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 506, 469, 386, 373, 320,  89,  85,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   6, 135, 189, 316, 373, 506, 506, 506, 506, 506, 506, 506, 506, 506,
 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 412, 378, 378, 345, 178, 181,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   9, 188, 193, 365, 397, 506, 506, 506, 506, 506, 506, 506, 506, 506,
 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 389, 378, 378, 368, 299, 297,  37,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  38, 191, 225, 376, 446, 506, 506, 506, 506, 506, 506, 506, 506, 506,
 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 508, 435, 378, 378, 374, 331, 332, 133,  24,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   5,  96, 192, 264, 404, 498, 506, 506, 506, 506, 506, 506, 506, 506, 506,
   */
}
#else
#if 0 // #if defined(CC_MT5890)
DRAM_STATUS_T DramcRxEyeScan(U8 dq_no)
{
    U8 ucidx_dqs, ucidx_dq, ucdqs_dly;
    U8 s1vref, s1dq_dly;
    U32 u4value, u4err_value = 0xffffffff, u4addr_array[4], u4sample_cnt, u4error_cnt;
	
    if (dq_no <= 15)
    {
        // assign address array
	    u4addr_array[0] = mcSET_PHY_REG_ADDR(0x134);
		u4addr_array[1] = mcSET_PHY_REG_ADDR(0x130);

        u4addr_array[2] = mcSET_DRAMC_REG_ADDR(DRAMC_REG_TOGGLE_CNT);
        u4addr_array[3] = mcSET_DRAMC_REG_ADDR(DRAMC_REG_DQ_ERR_CNT);
        ucidx_dqs =  dq_no / 8;
    }
    else
    {
        if (!IS_DRAM_CHANNELC_ACTIVE()) // chA/B
        {
		// assign address array
		u4addr_array[0] = mcSET_PHY_REG_ADDR(0x234);
		u4addr_array[1] = mcSET_PHY_REG_ADDR(0x230);
        }

        u4addr_array[2] = mcSET_DRAMC_REG_ADDR(DRAMC_REG_TOGGLE_CNT_2);
        u4addr_array[3] = mcSET_DRAMC_REG_ADDR(DRAMC_REG_DQ_ERR_CNT_2);
        ucidx_dqs =  (dq_no-16) / 8;
    }

    // check if SoC platform has "%" operation?!
    ucidx_dq =  dq_no % 16;
    	
    //Enable DQ eye scan
    //RG_RX_*RDQ_EYE_EN
    //RG_RX_*RDQ_VREF_EN 
    //RG_RX_*RDQ_VREF_OP_EN 
    //RG_RX_*RDQ_SMT_EN
    u4value = ucDram_Register_Read(u4addr_array[0]);
    u4value = u4value | 0x00400180;
    ucDram_Register_Write(u4addr_array[0], u4value);

    u4value = ucDram_Register_Read(u4addr_array[1]);
    mcSET_BIT (u4value,10);
    ucDram_Register_Write(u4addr_array[1], u4value);

    //Disable MIOCK jitter meter mode (RG_RX_*RDQ_EYE_MCK_SEL=0, RG_RX_MIOCK_JIT_EN=0)
    //RG_RX_*RDQ_EYE_MCK_SEL=0
    u4value = ucDram_Register_Read(u4addr_array[0]);
    mcCLR_BIT(u4value,9);
    ucDram_Register_Write(u4addr_array[0], u4value);

    //RG_RX_MIOCK_JIT_EN=0
    u4value = ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DCBLN));
    mcCLR_BIT(u4value, POS_DCBLN_RX_MIOCK_JIT_EN);
    ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DCBLN), u4value);

    //select DQ to be scanned (0~15)
    //DDRPHY: RG_RX_*RDQ_EYE_DQ_SEL
    u4value = ucDram_Register_Read(u4addr_array[0]);
    mcSET_FIELD(u4value, ucidx_dq, 0xf0000000, 28);
    ucDram_Register_Write(u4addr_array[0], u4value); 
    //DRAMC: RG_RX_DQ_EYE_SEL (0~15)
    u4value = ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DCBLN));
    mcSET_FIELD(u4value, ucidx_dq, 0x000000f0, 4);
    ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DCBLN), u4value); 
 	
    //Select DQS to be scanned (0 or 1)
    //RG_RX_*RDQ_EYE_DQSI_SEL
    u4value = ucDram_Register_Read(u4addr_array[0]);
    mcSET_FIELD(u4value, ucidx_dqs, 0x00000400, 10);
    ucDram_Register_Write(u4addr_array[0], u4value); 	

    Printf("===============================================================================\n");
    Printf("    DQ RX eye scan (channel=%d(2:cha, 3:chb), dq_%d)\n", IS_DRAM_CHANNELB_ACTIVE(), dq_no);
    Printf("===============================================================================\n");
#if 0
	//Set Vref voltage
    u4value = ucDram_Register_Read(u4addr_array[0]);
    mcSET_FIELD(u4value, 8, 0x0000000f, 0);
    ucDram_Register_Write(u4addr_array[0], u4value);
	//Set DQ delay (RG_??_RX_DQ_EYE_DLY)
    u4value = ucDram_Register_Read(u4addr_array[0]);
    mcSET_FIELD(u4value, 0, 0x00000f00, 8);
    ucDram_Register_Write(u4addr_array[0], u4value);
	//set dqs delay
	u4value = ucDram_Register_Read(u4addr_array[0]);
	mcSET_FIELD(u4value, 9, 0x007f0000, 16);
	ucDram_Register_Write(u4addr_array[0], u4value);

	Printf("vref, dqs_delay, dq_delay: reg 0x%08x=0x%08x \n", u4addr_array[0], ucDram_Register_Read(u4addr_array[0]));

	for(ii=0;ii<10;ii++)
	{
		//Reset eye scan counters (reg_sw_rst): 1 to 0
		u4value = ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DCBLN));
		mcSET_BIT(u4value, POS_DCBLN_REG_SW_RST);
		ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DCBLN), u4value);
		mcCLR_BIT(u4value, POS_DCBLN_REG_SW_RST);
		ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DCBLN), u4value);

		//Enable DQ eye scan (b'1)
		u4value = ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DCBLN));
		mcSET_BIT(u4value, POS_DCBLN_RX_EYE_SCAN_EN);
		ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DCBLN), u4value);

		// enable TE2, audio pattern
		//u4err_value = DramcEngine2(TE_OP_WRITE_READ_CHECK, 0xaa550000, 0x30000000, 0x00000100);                        
		HAL_Delay_us(100);
		//Disable DQ eye scan (b'1), for counter latch
		u4value = ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DCBLN));
		mcCLR_BIT(u4value, POS_DCBLN_RX_EYE_SCAN_EN);
		ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DCBLN), u4value);

		//Read the counter values from registers (toggle_cnt*, dqs_err_cnt*); *_cnt is for (DQS0,1) and *_cnt_2 is for (DQS2,3)
		u4sample_cnt = ucDram_Register_Read(u4addr_array[2]);
		u4error_cnt = ucDram_Register_Read(u4addr_array[3]);

		Printf("%4d,", u4error_cnt);         	
	}

	Printf("\n");
#endif
    for (s1vref=15; s1vref>=0; s1vref--)
    {
        //Set Vref voltage
        u4value = ucDram_Register_Read(u4addr_array[0]);
        mcSET_FIELD(u4value, s1vref, 0x00000078, 3);
        ucDram_Register_Write(u4addr_array[0], u4value); 

        //Wait for Vref settles down, 1ms is enough
        HAL_Delay_us(1000);

        //Set DQS delay (RG_RX_*RDQ_EYE_DQS_DLY) to 0
        u4value = ucDram_Register_Read(u4addr_array[0]);
        mcSET_FIELD(u4value, 0, 0x003f8000, 15);
        ucDram_Register_Write(u4addr_array[0], u4value);  		
 		
        //Disable DQ eye scan (b'1), for counter clear
        u4value = ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DCBLN));
        mcCLR_BIT(u4value, POS_DCBLN_RX_EYE_SCAN_EN);
        ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DCBLN), u4value);

        for (s1dq_dly=15; s1dq_dly>=0; s1dq_dly--)
        {
            //Set DQ delay (RG_RX_*RDQ_EYE_DQ_DLY)
            u4value = ucDram_Register_Read(u4addr_array[0]);
            mcSET_FIELD(u4value, s1dq_dly, 0x0f800000, 23);
            ucDram_Register_Write(u4addr_array[0], u4value);

            //Reset eye scan counters (reg_sw_rst): 1 to 0
            u4value = ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DCBLN));
            mcSET_BIT(u4value, POS_DCBLN_REG_SW_RST);
            ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DCBLN), u4value);
            mcCLR_BIT(u4value, POS_DCBLN_REG_SW_RST);
            ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DCBLN), u4value);

            //Enable DQ eye scan (b'1)
            u4value = ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DCBLN));
            mcSET_BIT(u4value, POS_DCBLN_RX_EYE_SCAN_EN);
            ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DCBLN), u4value);

            // enable TE2, audio pattern
            //u4err_value = DramcEngine2(TE_OP_WRITE_READ_CHECK, 0xaa550000, 0x30000000, 0x00000100);                        

            //Disable DQ eye scan (b'1), for counter latch
            u4value = ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DCBLN));
            mcCLR_BIT(u4value, POS_DCBLN_RX_EYE_SCAN_EN);
            ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DCBLN), u4value);

            //Read the counter values from registers (toggle_cnt*, dqs_err_cnt*); *_cnt is for (DQS0,1) and *_cnt_2 is for (DQS2,3)
            u4sample_cnt = ucDram_Register_Read(u4addr_array[2]);
            u4error_cnt = ucDram_Register_Read(u4addr_array[3]);

            Printf("%4d,", u4error_cnt);         
        }

        //Set DQ delay (RG_RX_*RDQ_EYE_DQ_DLY) to 0
        u4value = ucDram_Register_Read(u4addr_array[0]);
        mcSET_FIELD(u4value, 0, 0x0f800000, 23);
        ucDram_Register_Write(u4addr_array[0], u4value);

        //Disable DQ eye scan (b'1), for counter clear
        u4value = ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DCBLN));
        mcCLR_BIT(u4value, POS_DCBLN_RX_EYE_SCAN_EN);
        ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DCBLN), u4value);

        //for eye scan, dqs only need to scan 0~31
        for (ucdqs_dly=0; ucdqs_dly<32; ucdqs_dly++)
        {
            //Set DQS delay (RG_RX_*RDQ_EYE_DQS_DLY)
            u4value = ucDram_Register_Read(u4addr_array[0]);
            mcSET_FIELD(u4value, ucdqs_dly, 0x003f8000, 15);
            ucDram_Register_Write(u4addr_array[0], u4value);

            //Reset eye scan counters (reg_sw_rst): 1 to 0
            u4value = ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DCBLN));
            mcSET_BIT(u4value, POS_DCBLN_REG_SW_RST);
            ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DCBLN), u4value);
            mcCLR_BIT(u4value, POS_DCBLN_REG_SW_RST);
            ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DCBLN), u4value);

            //Enable DQ eye scan (b'1)
            u4value = ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DCBLN));
            mcSET_BIT(u4value, POS_DCBLN_RX_EYE_SCAN_EN);
            ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DCBLN), u4value);

            //reset phy R_DMPHYRST: 0xf0[28] 
            // 0x0f0[28] = 1 -> 0
            //u4value = ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_PHYCTL1));
            //mcSET_BIT(u4value, POS_PHYCTL1_PHYRST);
            //ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_PHYCTL1), u4value);
            //delay 10ns, 1ms here
            //mcDELAY_us(1);
            //mcCLR_BIT(u4value, POS_PHYCTL1_PHYRST);
            //ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_PHYCTL1), u4value);

            // read data counter reset
            // 0x0f4[25] = 1 -> 0
            //u4value = ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_GDDR3CTL1));
            //mcSET_BIT(u4value, POS_GDDR3CTL1_RDATRST);
            //ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_GDDR3CTL1), u4value);
            //delay 10ns, 1ms here
            //mcDELAY_us(1);                
            //mcCLR_BIT(u4value, POS_GDDR3CTL1_RDATRST);
            //ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_GDDR3CTL1), u4value);

            // enable TE2, audio pattern
            //u4err_value = DramcEngine2(TE_OP_WRITE_READ_CHECK, 0xaa550000, 0x30000000, 0x00000100);

            //Disable DQ eye scan (b'1), for counter latch
            u4value = ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DCBLN));
            mcCLR_BIT(u4value, POS_DCBLN_RX_EYE_SCAN_EN);
            ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DCBLN), u4value);

            //Read the counter values from registers (toggle_cnt*, dqs_err_cnt*); *_cnt is for (DQS0,1) and *_cnt_2 is for (DQS2,3)
            u4sample_cnt = ucDram_Register_Read(u4addr_array[2]);
            u4error_cnt = ucDram_Register_Read(u4addr_array[3]);

            Printf("%4d,", u4error_cnt);             
        }
        Printf("\n");
    }

	UNUSED(u4err_value);
	UNUSED(u4sample_cnt);
	UNUSED(u4error_cnt);

    //RG_RX_MIOCK_JIT_EN=0
    //u4value = ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DCBLN), &u4value);
    //mcCLR_BIT(u4value, POS_DCBLN_RX_MIOCK_JIT_EN);
    //ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DCBLN), u4value);

	return DRAM_OK;
	
    // log example
    /*
 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 504, 502, 500, 495, 494, 227,  61,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   3, 133, 226, 354, 378, 378, 381, 424, 480, 504, 506, 506, 506, 506, 506,
 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 504, 502, 500, 500, 429, 429,  53,  18,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  73, 197, 307, 378, 378, 385, 422, 493, 506, 506, 506, 506, 506, 506,
 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 502, 500, 500, 500, 225, 228,  13,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   9, 182, 200, 378, 378, 411, 450, 506, 506, 506, 506, 506, 506, 506,
 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 500, 500, 500, 463,  59,  56,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 102, 193, 348, 382, 436, 476, 506, 506, 506, 506, 506, 506, 506,
 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 500, 500, 484, 343,  16,  15,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  17, 129, 268, 384, 444, 503, 506, 506, 506, 506, 506, 506, 506,
 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 500, 500, 401, 177,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   1,  84, 238, 372, 467, 506, 506, 506, 506, 506, 506, 506, 506,
 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 504, 500, 223,  38,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  15, 261, 365, 499, 506, 506, 506, 506, 506, 506, 506, 506,
 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 496,  81,   7,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  11, 264, 372, 506, 506, 506, 506, 506, 506, 506, 506, 506,
 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 478, 121,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   1,  45, 241, 322, 475, 506, 506, 506, 506, 506, 506, 506, 506,
 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 492, 220,  37,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  10,  80, 195, 308, 470, 506, 506, 506, 506, 506, 506, 506, 506,
 512, 512, 512, 512, 512, 512, 512, 512, 506, 506, 506, 506, 466, 324, 103,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   2,  41, 156, 205, 315, 494, 506, 506, 506, 506, 506, 506, 506, 506,
 512, 512, 512, 512, 512, 512, 512, 512, 512, 506, 506, 496, 409, 364, 228,  17,  13,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   5,  74, 187, 245, 349, 502, 506, 506, 506, 506, 506, 506, 506, 506,
 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 506, 469, 386, 373, 320,  89,  85,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   6, 135, 189, 316, 373, 506, 506, 506, 506, 506, 506, 506, 506, 506,
 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 412, 378, 378, 345, 178, 181,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   9, 188, 193, 365, 397, 506, 506, 506, 506, 506, 506, 506, 506, 506,
 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 389, 378, 378, 368, 299, 297,  37,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  38, 191, 225, 376, 446, 506, 506, 506, 506, 506, 506, 506, 506, 506,
 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 508, 435, 378, 378, 374, 331, 332, 133,  24,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   5,  96, 192, 264, 404, 498, 506, 506, 506, 506, 506, 506, 506, 506, 506,
   */
}
#endif //defined(CC_MT5890)
#endif //defined(CC_MT5399)

#if defined(CC_MT5399)||defined(CC_MT5882) // || defined(CC_MT5890)
static INT32 _DDR_RxEyeScan(INT32 i4Argc, const CHAR ** aszArgv)
{
	U8 dq_no=0;

	Printf("dram rx eye scan...\n");
	if (i4Argc < 2)
    {
        Printf("Usage: reye [dq no] .\n");
		return 0;
    }
    else if (i4Argc == 2)
    {
        dq_no = (UINT32)StrToInt(aszArgv[1]);
    }

	DramcRxEyeScan(dq_no);
	
	return 0;
}
#endif

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
 CLIMOD_DEBUG_FUNCTIONS(MEMTEST)

static CLI_EXEC_T _arMemoryTestCmdTbl [] =
{
    CLIMOD_DEBUG_CLIENTRY(MEMTEST),
#if defined(CC_MT5890) 
	{ "ch", "ch", _DDR_ChSelect, NULL, "DRAM channel select", CLI_GUEST },
	{ "bm", "bm", _DDR_BandWidthMontorTest, NULL, "DRAM Bandwidth monitor test", CLI_GUEST },
	{ "cm", "cm", _DDR_Cmdcm, NULL, "Confirm DDR test time", CLI_GUEST },	  
	{ "dqs", "dqs", _DDR_DQSDelayTest, NULL, "DRAM DQS delay test", CLI_GUEST },
	{ "dqso", "dqso", _DDR_DQSOutputDelayTest, NULL, "DRAM DQS output delay test", CLI_GUEST },
	{ "ssc", "ssc", _DDR_SSCTest, NULL, "DRAM spread spectrum",  CLI_GUEST },
	{ "jitter", "j", _DDR_JitterCheck, NULL, "DRAM Jitter check",  CLI_SUPERVISOR },
	{ "query", "q", _DDR_InforQuery, NULL, "DRAM Information Query",  CLI_GUEST },
	//{ "rxeyescan", "reye", _DDR_RxEyeScan, NULL, "DRAM Rx Eye Scan",  CLI_GUEST},
#else
  #if defined(DRAM_MEMORY_CHECK)
    { "run", "r", _DDR_CmdMemoryTest, NULL, "DRAM Memory test", CLI_SUPERVISOR },
  #endif /* #if defined(DRAM_MEMORY_CHECK) */    
    { "ch", "ch", _DDR_ChSelect, NULL, "DRAM channel select", CLI_GUEST },
    { "memrw", "memrw", _DDR_Cmdmemoryrw, NULL, "DRAM memory test only once", CLI_SUPERVISOR },
    { "bm", "bm", _DDR_BandWidthMontorTest, NULL, "DRAM Bandwidth monitor test", CLI_GUEST },
    { "cm", "cm", _DDR_Cmdcm, NULL, "DRAM continuous memory test", CLI_GUEST },
    { "dqsg", "g", _DDR_DQSGatingWindowTest, NULL, "DRAM DQS gating window test", CLI_GUEST },    
    { "dqs", "dqs", _DDR_DQSDelayTest, NULL, "DRAM DQS delay test", CLI_GUEST },
    { "dqso", "dqso", _DDR_DQSOutputDelayTest, NULL, "DRAM DQS output delay test", CLI_GUEST },
  #ifdef DRAM_HWONLINECALDQS
    { "cal", "cal", _DDR_OnlineDQSCal, NULL, "DRAM Online DQS Calibration",  CLI_SUPERVISOR },
  #endif
    { "ssc", "ssc", _DDR_SSCTest, NULL, "DRAM spread spectrum",  CLI_GUEST },
    { "jitter", "j", _DDR_JitterCheck, NULL, "DRAM Jitter check",  CLI_SUPERVISOR },
    { "query", "q", _DDR_InforQuery, NULL, "DRAM Information Query",  CLI_GUEST },
  #if defined(CC_MT5399)||defined(CC_MT5882)
	{"rxeyescan", "reye", _DDR_RxEyeScan, NULL, "DRAM Rx Eye Scan",  CLI_GUEST},
  #endif
#endif //defined(CC_MT5890)

    { NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR }
};

//---------------------------------------------------------------------------
// Inter-file functions
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Public functions
//---------------------------------------------------------------------------
#if 0
void Drv_DRAMInit(void)
{
    // Check and create DDR thread to do memory test.
    if (u4Cmemrw > 0)
    {
        _DDR_ThreadInit();
        return;
    }

#ifdef DRAM_HWONLINECALDQS
    _DDR_ThreadInit();
#endif
}
#endif

CLI_MAIN_COMMAND_ITEM(MemTest)
{
    "memtest", NULL, NULL, _arMemoryTestCmdTbl, "Memory test", CLI_GUEST
};


