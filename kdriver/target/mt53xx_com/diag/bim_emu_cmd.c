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
#include "x_ldr_env.h"
#include "loader_imghdr.h"

#ifndef CPU_TEST_ENABLE
#define CPU_TEST_ENABLE     1
#endif

#if CPU_TEST_ENABLE
//=====================================================================
// extern functions
//=====================================================================
EXTERN void run_QuickSort(void);
EXTERN void run_HeapSort(void);
EXTERN void TestMemoryAccess_R(void);
EXTERN void TestMemoryAccess(void);
EXTERN void TestPrimes(void);
EXTERN void TestBits(void);
EXTERN void fbench(int niter);
EXTERN void cache_test(void);
EXTERN void MEM_Test(UINT32 u4Round, UINT32 u4Addr, UINT32 u4Size);
EXTERN int vfp_test(void);
EXTERN int pmu_test(void);

#define PAGE_SIZE	 4096
typedef unsigned int volatile ulv;

int TEST_Memwrite(ulv * pDest, unsigned int nCount)
{
	register unsigned int val = 0xA5A4B5B4;
	ulv *p2 = NULL;
	unsigned int    nTest, i;
	for (nTest = 0; nTest < 10; nTest++)
	{
		p2 = (ulv *) pDest;
		for (i = 0; i < nCount; i++)
			*p2++ = val;
	}
	return nTest;
}

int TEST_Memread(ulv * pSrc, unsigned int nCount)
{
	register unsigned int val;
	ulv *p1 = NULL;
	unsigned int    nTest, i;
	for (nTest = 0; nTest < 10; nTest++)
	{
		p1 = (ulv *) pSrc;
		for (i = 0; i < nCount; i++)
			val = *p1++;
	}
	return nTest;
}

int TEST_Memcpy(ulv * pSrc, ulv * pDest, unsigned int nCount)
{
	int nTest;
	for (nTest = 0; nTest < 10; nTest++)
		memcpy((void*)pDest, (void*)pSrc, nCount*sizeof(unsigned int));
	return nTest;
}

int TEST_MemBandWidth(ulv * pSrc, ulv * pDest, unsigned int nCount)
{
	ulv *p1 = NULL;
	ulv *p2 = NULL;
	unsigned int	i;
	unsigned int	nTest;

	for (nTest = 0; nTest < 10; nTest++)
	{
		p1 = (ulv *) pSrc;
		p2 = (ulv *) pDest;

		for (i = 0; i < nCount; i++)
			*p2++ = *p1++;
	}

	return nTest;
}


void CheckMem(unsigned int uMemSize)
{
	unsigned int	nLoop;
	unsigned int	nAllocBytes;
	unsigned int	nBufSize;
	unsigned int	nCount;
	unsigned int	PAGE_MASK;
	void volatile	* pBuf = NULL;
	void volatile	* pAlignedBuf = NULL;
	ulv				* pSrc;
	ulv				* pDest;
	HAL_TIME_T t1, t2, td;
	//struct timespec	tss, tse;
	unsigned int				nDelay;
	unsigned int				nTestCount;
	unsigned int				nSize;

	PAGE_MASK = (unsigned int) ~(PAGE_SIZE - 1);
	nBufSize	= (unsigned int) (uMemSize << 20);
	nAllocBytes = nBufSize + PAGE_SIZE;

	pBuf = (void volatile *) x_mem_alloc(nAllocBytes);
	if (!pBuf)
	{
		printf(" Error :: malloc failed\n");
		return;
	}

	printf(" Allocated %d bytes at 0x%08x\n", nAllocBytes, (unsigned int) pBuf);

	if ((unsigned int) pBuf % PAGE_SIZE)
	{
		pAlignedBuf = (void volatile *) (((unsigned int) pBuf + PAGE_SIZE) & PAGE_MASK);
		printf(" Aligned at 0x%08x\n", (unsigned int) pAlignedBuf);
	}
	else
	{
		pAlignedBuf = pBuf;
	}

	/* Try memlock */
	printf(" Trying mlock ...");
	#if 0
//	fflush(stdout);
	if (mlock((void *) pBuf, nAllocBytes) < 0)
	{
		printf(" Error :: failed to lock memory\n");
		return;
	}
	printf("locked.\n");
    #endif
	/* Show information */
	nCount = (nBufSize / 2) / sizeof(unsigned int);

	pSrc = (ulv *) pAlignedBuf;
	pDest = (ulv *) ((unsigned int) pAlignedBuf + (nBufSize / 2));

	printf(" Read from : %p\n", pSrc);
	printf(" Write to  : %p\n", pDest);

	nSize = nCount * sizeof(unsigned int);
	printf("\n(1) Memory read/write test through 32-bit pointer access\n");
	nLoop  = 0;
	while (nLoop < 5)
	{
		nLoop++;

		//clock_gettime(CLOCK_MONOTONIC, &tss);
		HAL_GetTime(&t1);
		nTestCount = TEST_MemBandWidth(pSrc, pDest, nCount);
		HAL_GetTime(&t2);
		//clock_gettime(CLOCK_MONOTONIC, &tse);
       	HAL_GetDeltaTime(&td, &t1, &t2);

	//	nDelay = (tse.tv_sec - tss.tv_sec)*1000 + tse.tv_nsec/1000000 - tss.tv_nsec/1000000;
        nDelay = td.u4Seconds*1000+td.u4Micros/1000;
		printf("Read/Write %3d: %d times, %8d bytes, %4d msec => %6d KB/sec\n",
				nLoop, nTestCount, nSize, nDelay, (((nSize * nTestCount) / 1024) * 1000) / nDelay);
	}

	printf("\n(2) Memory read/write test through memcpy()\n");
	nLoop  = 0;
	while (nLoop < 5)
	{
		nLoop++;

		HAL_GetTime(&t1);
		nTestCount = TEST_Memcpy(pSrc, pDest, nCount);
		HAL_GetTime(&t2);
		//clock_gettime(CLOCK_MONOTONIC, &tse);
       	HAL_GetDeltaTime(&td, &t1, &t2);

	//	nDelay = (tse.tv_sec - tss.tv_sec)*1000 + tse.tv_nsec/1000000 - tss.tv_nsec/1000000;
        nDelay = td.u4Seconds*1000+td.u4Micros/1000;

		printf("Read/Write %3d: %d times, %8d bytes, %4d msec => %6d KB/sec\n",
				nLoop, nTestCount, nSize, nDelay, (((nSize * nTestCount) / 1024) * 1000) / nDelay);
	}

	printf("\n(3) Memory read test\n");
	nLoop  = 0;
	while (nLoop < 5)
	{
		nLoop++;

		HAL_GetTime(&t1);
		nTestCount = TEST_Memread(pSrc, nCount);
		HAL_GetTime(&t2);
		//clock_gettime(CLOCK_MONOTONIC, &tse);
       	HAL_GetDeltaTime(&td, &t1, &t2);

	//	nDelay = (tse.tv_sec - tss.tv_sec)*1000 + tse.tv_nsec/1000000 - tss.tv_nsec/1000000;
        nDelay = td.u4Seconds*1000+td.u4Micros/1000;

		printf("Read  %3d: %d times, %8d bytes, %4d msec => %6d KB/sec\n",
				nLoop, nTestCount, nSize, nDelay, (((nSize * nTestCount) / 1024) * 1000) / nDelay);
	}

	printf("\n(4) Memory write test\n");
	nLoop  = 0;
	while (nLoop < 5)
	{
		nLoop++;

		HAL_GetTime(&t1);
		nTestCount = TEST_Memwrite(pDest, nCount);
		HAL_GetTime(&t2);
				//clock_gettime(CLOCK_MONOTONIC, &tse);
		HAL_GetDeltaTime(&td, &t1, &t2);

			//	nDelay = (tse.tv_sec - tss.tv_sec)*1000 + tse.tv_nsec/1000000 - tss.tv_nsec/1000000;
				nDelay = td.u4Seconds*1000+td.u4Micros/1000;


		printf("Write %3d: %d times, %8d bytes, %4d msec => %6d KB/sec\n",
				nLoop, nTestCount, nSize, nDelay, (((nSize * nTestCount) / 1024) * 1000) / nDelay);
	}

	//munlock((void *) pBuf, nAllocBytes);
	free((void *) pBuf);
}

static void _ShowCacheMeterUsage(void)
{
    Printf("cm");
}

static INT32 _CmdCacheMeter(INT32 i4Argc, const CHAR** aszArgv)
{
    UINT32 u4IReq, u4IMiss, u4IPenalty, u4DReq, u4DMiss, u4DPenalty;

    if (i4Argc != 1)
    {
        _ShowCacheMeterUsage();
        return 0;
    }

    //Printf("Please wait for calculation....\n\n");

    //CacheMeterQuery(&u4IReq, &u4IMiss, &u4IPenalty, &u4DReq, &u4DMiss, &u4DPenalty);

    return 0;
}
static INT32 _CmdCpuTest(INT32 i4Argc, const CHAR** aszArgv)
{
    UINT32 u4Action = 0, u4Flag;
    HAL_TIME_T t1, t2, td;
    FLOAT f;
    DOUBLE d;

    u4Flag = HalCriticalStart();

    if (i4Argc > 1)
    {
        u4Action = StrToInt(aszArgv[1]);
    }

    if (u4Action == 9)
    {
        pmu_test();
    }

    if (u4Action == 8)
    {
        cache_test();
    }

    if(u4Action ==10)
    {
    UINT32* ptr;
    UINT32 v = 0;
	UINT32 numBytes=0x1000000,val,i,j;
	HAL_GetTime(&t1);
    for ( i = 0; i < (numBytes / 4) / 8; i++) {
         for ( j = 0; j < 8; j++) {
            v += ptr[i*8+j];
        }
       }
    val += v;
	HAL_GetTime(&t2);
	HAL_GetDeltaTime(&td, &t1, &t2);
    Printf("Seq 4-byte write: %d.%06d sec.\n", td.u4Seconds, td.u4Micros);
     }
#if 0
    if(u4Action ==11)
    {
    UINT32* ptr;
    UINT32 v = 0;
	UINT32 numBytes=0x1000000,val,i,j;
	HAL_GetTime(&t1);
	__builtin_prefetch(&ptr[0], 0, 3);
    __builtin_prefetch(&ptr[8], 0, 3);
    __builtin_prefetch(&ptr[16], 0, 3);
    __builtin_prefetch(&ptr[24], 0, 3);
    for ( i = 0; i < (numBytes / 4) / 8; i++) {
         for ( j = 0; j < 8; j++) {
            v += ptr[i*8+j];
        }
         __builtin_prefetch(&ptr[(i+4)*8], 0, 3);
       }
    val += v;
	HAL_GetTime(&t2);
	HAL_GetDeltaTime(&td, &t1, &t2);
    Printf("&&&&&&: %d.%06d sec.\n", td.u4Seconds, td.u4Micros);
     }
#endif
	if(u4Action ==12)
	{
		CheckMem(10);
	}
    if (u4Action == 7)
    {
        f = 1.1;
        d = 0.01;
        Printf("expected string : float=1.10, ffffffff, 0.010, 123.12\n");
        Printf("real string : float=%2.2f, %x, %.3f, %2.2f\n", f, 0xffffffff, d, 123.123);
        HAL_GetTime(&t1);
        fbench(1000);
        HAL_GetTime(&t2);
        HAL_GetDeltaTime(&td, &t1, &t2);
        Printf("fbench: %d.%06d sec.\n", td.u4Seconds, td.u4Micros);
        vfp_test();
    }

    if ((u4Action == 0) || (u4Action == 1))
    {
        HAL_GetTime(&t1);
        run_QuickSort();
        HAL_GetTime(&t2);
        HAL_GetDeltaTime(&td, &t1, &t2);
        Printf("run_QuickSort: %d.%06d sec.\n", td.u4Seconds, td.u4Micros);
    }

    if ((u4Action == 0) || (u4Action == 2))
    {
        HAL_GetTime(&t1);
        run_HeapSort();
        HAL_GetTime(&t2);
        HAL_GetDeltaTime(&td, &t1, &t2);
        Printf("run_HeapSort: %d.%06d sec.\n", td.u4Seconds, td.u4Micros);
    }

    if ((u4Action == 0) || (u4Action == 3))
    {
        HAL_GetTime(&t1);
        TestMemoryAccess_R();
        HAL_GetTime(&t2);
        HAL_GetDeltaTime(&td, &t1, &t2);
        Printf("TestMemoryAccess_R: %d.%06d sec.\n", td.u4Seconds, td.u4Micros);
    }

    if ((u4Action == 0) || (u4Action == 4))
    {
        HAL_GetTime(&t1);
        TestMemoryAccess();
        HAL_GetTime(&t2);
        HAL_GetDeltaTime(&td, &t1, &t2);
        Printf("TestMemoryAccess: %d.%06d sec.\n", td.u4Seconds, td.u4Micros);
    }

    if ((u4Action == 0) || (u4Action == 5))
    {
        HAL_GetTime(&t1);
        TestPrimes();
        HAL_GetTime(&t2);
        HAL_GetDeltaTime(&td, &t1, &t2);
        Printf("TestPrimes: %d.%06d sec.\n", td.u4Seconds, td.u4Micros);
    }

    if ((u4Action == 0) || (u4Action == 6))
    {
        HAL_GetTime(&t1);
        TestBits();
        HAL_GetTime(&t2);
        HAL_GetDeltaTime(&td, &t1, &t2);
        Printf("TestBits: %d.%06d sec.\n", td.u4Seconds, td.u4Micros);
    }

    HalCriticalEnd(u4Flag);

    return 0;
}

static void _TimerIsr(UINT16 u2Vector)
{
    Printf("irq enable=0x%08x, irq status=0x%08x\n", BIM_READ32(REG_IRQEN), BIM_READ32(REG_IRQST));
    if (u2Vector==VECTOR_T1)
    {
        Printf("VECTOR_T1, 4 second timeout\n");
    }
    else if (u2Vector==VECTOR_T2)
    {
        Printf("VECTOR_T2, 2 second timeout\n");
    }
    BIM_ClearIrq(u2Vector);
}

#define DRAM_BUF0 0x4000000
#define DRAM_BUF1 (DRAM_BUF0+0x1000000)
#define DRAM_BUF2 (DRAM_BUF1+0x1000000)

static INT32 _CmdBimTest( INT32 i4Argc, const CHAR ** aszArgv )
{
    UINT32 i, j;
    UINT32 cpi0, cpi1, cpi2, cpi3;
    LDR_ENV_T* prLdrEnv = (LDR_ENV_T*)CC_LDR_ENV_OFFSET;
    UINT32 u4ImageAddr;
    UINT8 u1Checksum;
	UINT32 u4imagesize,u4Plainsize;
    void (* pfnOldIsr)(UINT16);
    BOOL fgPass;

    // read/write general purpose register---------------------------------------------
    fgPass = TRUE;
    for (i=REG_RW_GPRB0; i<=REG_RW_GPRDW3; i+=4)
    {
        BIM_WRITE32(i, 0xffffffff);
    }
    for (i=REG_RW_GPRB0; i<=REG_RW_GPRDW3; i+=4)
    {
        if (BIM_READ32(i)!=0xffffffff)
        {
            fgPass = FALSE;
            Printf("r/w register 0x%08x fail\n", i);
            break;
        }
    }
    if (fgPass)
    {
        Printf("r/w register 0x%08x--0x%08x pass\n", REG_RW_GPRB0, REG_RW_GPRDW3);
    }
    for (i=REG_RW_GPRDW4; i<=REG_RW_GPRDW5; i+=4)
    {
        BIM_WRITE32(i, 0xffffffff);
    }
    for (i=REG_RW_GPRDW4; i<=REG_RW_GPRDW5; i+=4)
    {
        if (BIM_READ32(i)!=0xffffffff)
        {
            fgPass = FALSE;
            Printf("r/w register 0x%08x fail\n", i);
            break;
        }
    }
    if (fgPass)
    {
        Printf("r/w register 0x%08x--0x%08x pass\n", REG_RW_GPRDW4, REG_RW_GPRDW5);
    }


    LZHS_SetIsrEnable(TRUE);
    // lzhs dram to dram decode---------------------------------------------
    u4ImageAddr = LOADER_MAX_SIZE+ 0x50 + 0xf8000000;
    u1Checksum = *((UINT8 *)(u4ImageAddr - 8));
    u4imagesize= *((UINT32 *)(u4ImageAddr -12));
	u4Plainsize= *((UINT32 *)(u4ImageAddr -0x10));
    Printf("Test DRAM2DRAM LZHS decode, plain size=0x%x, lzhs size=0x%x\n",
        u4Plainsize,u4imagesize);
    x_memcpy((VOID*)DRAM_BUF0, (VOID*)u4ImageAddr, u4imagesize);
    if (LZHS_Dec(DRAM_BUF0, DRAM_BUF1, u4Plainsize, &u1Checksum)==0)
    {
        Printf("LZHS decode ok, u1Checksum=0x%02x\n", u1Checksum);
    }
    else
    {
        Printf("LZHS decode fail, u1Checksum=0x%02x\n", u1Checksum);
    }

    Printf("Test NOR2DRAM LZHS decode...\n");
    if (LZHS_Dec(u4ImageAddr, DRAM_BUF2, u4Plainsize, &u1Checksum)==0)
    {
        Printf("LZHS decode ok, u1Checksum=0x%02x\n", u1Checksum);
    }
    else
    {
        Printf("LZHS decode fail, u1Checksum=0x%02x\n", u1Checksum);
    }

    if (x_memcmp((VOID*)DRAM_BUF1, (VOID*)DRAM_BUF2, prLdrEnv->u4LoaderPlainSize)==0)
    {
        Printf("LZHS compare ok\n");
    }
    else
    {
        Printf("LZHS compare fail\n");
    }
    LZHS_SetIsrEnable(FALSE);

    SerTransparent();

    // timer and interrupt test----------------------------------------------
    //27M=0x19BFCC0=1 second
    x_reg_isr(VECTOR_T1, _TimerIsr, &pfnOldIsr);
    x_reg_isr(VECTOR_T2, _TimerIsr, &pfnOldIsr);

    // only the first time x_reg_isr will enable, so we manually enable it
    BIM_EnableIrq(VECTOR_T1);
    BIM_EnableIrq(VECTOR_T2);

    // Setup Timer 1 as 4 second counter, not repeat
    BIM_WRITE32(REG_RW_TIMER1_LLMT, 2);
    BIM_WRITE32(REG_RW_TIMER1_HLMT, 0x19BFCC0);
    BIM_WRITE32(REG_RW_TIMER1_LOW , 2);
    BIM_WRITE32(REG_RW_TIMER1_HIGH, 0x19BFCC0);

    // Setup Timer 2 as 2 second counter, repeat
    BIM_WRITE32(REG_RW_TIMER2_LLMT, 0x19BFCC0);
    BIM_WRITE32(REG_RW_TIMER2_HLMT, 0x1);
    BIM_WRITE32(REG_RW_TIMER2_LOW , 0x19BFCC0);
    BIM_WRITE32(REG_RW_TIMER2_HIGH, 0x1);

    i = BIM_READ32(REG_RW_TIMER_CTRL);
    i |= ((TMR0_CNTDWN_EN) | (TMR0_AUTOLD_EN) |
              (TMR1_CNTDWN_EN) |
              (TMR2_CNTDWN_EN) | (TMR2_AUTOLD_EN));
    BIM_WRITE32(REG_RW_TIMER_CTRL, i);

    x_thread_delay(10000);
    Printf("VECTOR_T0, 10 second timeout, disable ISR\n");
    BIM_DisableIrq(VECTOR_T1);
    BIM_DisableIrq(VECTOR_T2);
    // only remain nucleus timer
    BIM_WRITE32(REG_RW_TIMER_CTRL, (TMR0_CNTDWN_EN) | (TMR0_AUTOLD_EN));
    Printf("T1 ISR enable:%d\n", BIM_IsIrqEnabled(VECTOR_T1));
    Printf("T2 ISR enable:%d\n", BIM_IsIrqEnabled(VECTOR_T2));


    return 0;
}

static INT32 _CmdSendEvent( INT32 i4Argc, const CHAR ** aszArgv )
{
    ARM_SendEvent();
}

static INT32 _CmdFlushInvalideDCache( INT32 i4Argc, const CHAR ** aszArgv )
{
    HalFlushInvalidateDCache();
}

#ifdef CC_MT5890
#define SAMPLE_COUNT 10
static INT32 _CmdTrng( INT32 i4Argc, const CHAR ** aszArgv )
{
    int i = 0;
    int j = 0;
    UINT32 skipped_data = 0x12345678;
    UINT32 rand_data[SAMPLE_COUNT];

    //Get random data for SAMPLE_COUNT times
    TRNG_WRITE32(TRNG_CTRL, TRNG_READ32(TRNG_CTRL) | TRNG_CTRL_START);
    //*((volatile unsigned int*)TRNG_CTRL) |= TRNG_CTRL_START;  //start TRNG

    for(i=SAMPLE_COUNT-1; i>=0; i--)
    {
        while (0 == (TRNG_READ32(TRNG_CTRL) & TRNG_CTRL_RDY)) ; //wait until TRNG ready
        rand_data[i] = TRNG_READ32(TRNG_DATA);
        Printf("TRNG data [%d] = 0x%x\n", i, rand_data[i]);
    }
    TRNG_WRITE32(TRNG_CTRL, TRNG_READ32(TRNG_CTRL) & ~TRNG_CTRL_START); //stop TRNG

    //Check data correctness
    for(i=SAMPLE_COUNT-1; i>0; i--)
    {
        //Compare ==> data can't be all 0 or all 1
        if(rand_data[i]==0 || rand_data[i]==0xFFFFFFFF)
        {
            Printf("TRNG data can't be all 0 or all 1 (0x%x)\n", rand_data[i]);
            return 1;
        }
        for(j=i-1; j>=0; j--)
        {
            //Compare ==> data can't be the same
            if(rand_data[i] == rand_data[j])
            {
                Printf("TRNG data can't be the same (0x%x, 0x%x)\n", rand_data[i], rand_data[j]);
                return 2;
            }
        }
    }

    return 0;
}
#endif

static CLI_EXEC_T _arBIMEMUCmdTbl[] =
{
#if CPU_TEST_ENABLE
    {
        "CacheMeter",
        "cm",
        _CmdCacheMeter,
        NULL,
        "Cache Meter",
        CLI_SUPERVISOR
    },
    {
        "cpu",
        "c",
        _CmdCpuTest,
        NULL,
        "google CPU test",
        CLI_SUPERVISOR
    },
    {
        "bimtest",
        "bimtest",
        _CmdBimTest,
        NULL,
        "bim test",
        CLI_SUPERVISOR
    },
    {
        "sev",
        "sev",
        _CmdSendEvent,
        NULL,
        "issue SEV",
        CLI_SUPERVISOR
    },
    {
        "flushinvdacahe",
        "fd",
        _CmdFlushInvalideDCache,
        NULL,
        "flush inv dacahe",
        CLI_SUPERVISOR
    },
#ifdef CC_MT5890
    {
        "trng",
        "trng",
        _CmdTrng,
        NULL,
        "trng",
        CLI_SUPERVISOR
    },
#endif // CC_MT5890
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
CLI_MAIN_COMMAND_ITEM(BIMEMU)
{
    "bimtest",
    NULL,
    NULL,
    _arBIMEMUCmdTbl,
    "BIM module test",
    CLI_GUEST
};


#endif /* CPU_TEST_ENABLE */

