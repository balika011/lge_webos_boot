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
 * $Author: p4admin $
 * $Date: 2015/01/20 $
 * $RCSfile: hal_1176.c,v $
 * $Revision: #2 $
 *
 *---------------------------------------------------------------------------*/

/** @file hal_1176.c
 *  hal_1176.c provides functions of IRQ/FIQ, MMU, and Cache.
 */


//lint --e{950} No ANSI reserved word (__asm)
//lint --e{529} Symbol not subsequently referenced
//lint --e{530} Symbol not initialized
//lint --e{715} Symbol not referenced
//lint --e{506} Constant value Boolean
//lint --e{641} Converting enum to int


//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "x_typedef.h"
#include "x_hal_arm.h"
#include "x_assert.h"
#include "x_hal_5381.h"
#include "x_bim.h"
#include "x_dram.h"

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------

//---------------------------------------------------------------------
// IRQ/FIQ related definitions
//---------------------------------------------------------------------

// PSR bit definitions
#define PSR_IRQ_DISABLE             (1 << 7)
#define PSR_FIQ_DISABLE             (1 << 6)

//---------------------------------------------------------------------
// Cache related definitions
//---------------------------------------------------------------------

// CP15 control register (c1) bit definitions
#define DATA_UNALIGN_ENABLE         (1 << 22)
#define CP10_PRI_ONLY               (1 << 20)
#define ICACHE_ENABLE               (1 << 12)
#define DCACHE_ENABLE               (1 << 2)
#define MMU_ENABLE                  (1 << 0)

#define XOR_CACHE                   (1 << 7)
#define SMP                         (1 << 6)
#define FOZ                         (1 << 3)
#define L1C_PREFETCH                (1 << 2)
#define L2C_PREFETCH_HINT           (1 << 1)

// CP15 cache type register (c0) bit definitions
#define CTYPE_MASK                  0xf
#define CTYPE_SHIFT                 0x25
#define S_MASK                      1
#define S_SHIFT                     24
#define DSIZE_MASK                  0xf
#define DSIZE_SHIFT                 (6 + 12)
#define ISIZE_MASK                  0xf
#define ISIZE_SHIFT                 6

// Translate VA (Virtual Address) to MVA (Modified Virtual Address)
#define MVA(addr)                   ((addr) & 0xffffffc0)
#define L2MVA(addr)                   addr
#ifdef CC_MT5882
// Cache parameters (in bytes)
#define ICACHE_LINE_SIZE            32
#define DCACHE_LINE_SIZE            64
#define DCACHE_LINE_SIZE_MSK        (DCACHE_LINE_SIZE - 1)
#define DCACHE_FLUSH_THRESHOLD      (DCACHE_LINE_SIZE * 256)
#define L1_SIZE                     32*1024
#define L2_SIZE                     512*1024
#else
// Cache parameters (in bytes)
#define ICACHE_LINE_SIZE            64
#define DCACHE_LINE_SIZE            64
#define L1_SIZE                     32*1024
#define L2_SIZE                     2*1024*1024
#define DCACHE_LINE_SIZE_MSK        (DCACHE_LINE_SIZE - 1)
#define DCACHE_FLUSH_THRESHOLD      (L2_SIZE)
#endif

UINT32 g_u4Dcachelinesize=DCACHE_LINE_SIZE;
//---------------------------------------------------------------------
// Page table related definitions
//---------------------------------------------------------------------

// Domain access control definitions of CP15 register r3
enum DOMAIN_ACCESS
{
    DENIED = 0,
    CLIENT = 1,
    RESERVED = 2,
    MANAGER = 3
};

enum ACCESS_PERMISSION //bit[11:10]
{
    NO_ACCESS,
    USER_NO_ACCESS,
    USER_READ_ONLY,
    USER_ACCESS,
};

enum PAGE_TABLE_ENTRY_TYPE
{
    INVALID = 0,
    COARSE_PAGE = 1,
    SECTION = 2,
    FINE_PAGE = 3,
};


#define TEX_STRONGLY_ORDERED    0 //C,B=0,0
#define TEX_SHARED_DEVICE       0 //C,B=0,1
#define TEX_OUTER_NONCACHE      4 //C,B=0,1
#define TEX_NON_SHARED_DEVICE   2 //C,B=0,0
#define TEX_WRITE_ALLOCATE      1 //C,B=1,1

// Bit definitions of page table
#define L2_XN_BIT       (1 << 0)    // for small page
#define B_BIT           (1 << 2)    // Bufferable
#define C_BIT           (1 << 3)    // Cachable
#define L1_XN_BIT       (1 << 4)    // The execute-never bit. Determines whether the region is executable
#define S_BIT           (1 << 16)   // shared memory region
#define NS_BIT          (1 << 19)   // non-secure bit

//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------

// Macro definition for single level 1 entry of page table
#define L1Entry(type, addr, tex, ucb, acc) \
    ((type == SECTION) \
        ? (((addr) & 0xfff00000) | ((acc) << 10) | ((tex) << 12) | (ucb) | (type)) \
        : ((type == COARSE_PAGE) ? (((addr) &0xfffffc00) | (type)) : 0))

// for small page (0x2)
#define L2Entry(addr, tex, ucb, acc) \
    (((addr) & 0xfffff000) | ((acc) << 4) | ((tex) << 6) | (ucb) | 0x2)

#ifndef __GCC__
#pragma arm
#endif


//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

#ifndef CC_MTK_LOADER
EXTERN UINT32 BSP_GetMemSize(void);
#endif /* CC_MTK_LOADER */

static UINT32 _u4MMUTableAddress = 0;

// We use ISB and DSB to ensure the instruction and data order after cp15 operation.
// NOP is not safe as ARM core improving its pipeline, e.x. CA15 has 15-24 stage pipeline
#ifndef __GCC__
__asm void __DSB(void)
{
    DSB
    BX lr
}

__asm void __DMB(void)
{
    DMB
    BX lr
}
__asm void __ISB(void)
{
    ISB
    BX lr
}
#endif

inline void _DMB(void)
{
#ifdef __GCC__
    __asm__ __volatile__ ("dmb" : : : "memory");
#else
    __DMB();
#endif
}

inline void _DSB(void)
{
#ifdef __GCC__
    __asm__ __volatile__ ("dsb" : : : "memory");
#else
    __DSB();
#endif
}

inline void _ISB(void)
{
#ifdef __GCC__
    __asm__ __volatile__ ("isb" : : : "memory");
#else
    __ISB();
#endif
}


inline void _HalSelectDCacheLevel(UINT32 u4Level)
{
	UINT32 r;
	ASSERT(u4Level >= 1);

	r = (u4Level - 1) << 1;
#ifdef __GCC__
            __asm__ ("MCR   p15, 2, %0, c0, c0, 0" : : "r" (r));
           	_ISB();
    		__asm__ ("MRC   p15, 1, %0, c0, c0, 0" : "=r" (r));
#else
            __asm
            {
                MCR p15,2,r,c0,c0,0
            }

            _ISB();

            __asm
            {
                MRC p15,1,r,c0,c0,0
            }
#endif

	//Printf("Cache size: 0x%x now1 now1 code.\n", r);
}


#if !defined(CC_MTK_PRELOADER)

//===========================================================================
// IRQ/FIQ related functions
//===========================================================================

//-----------------------------------------------------------------------------
/** HalEnableIRQ() Enable IRQ
 */
//-----------------------------------------------------------------------------
void HalEnableIRQ(void)
{
    register UINT32 r;

#ifdef __GCC__
    __asm__ ("MRS     %0, CPSR" : "=r" (r));
    __asm__ ("BIC     %0, %1, %2" : "=r" (r) : "r" (r), "r" (PSR_IRQ_DISABLE));
    __asm__ ("MSR     CPSR_c, %0" : : "r" (r));
#else
    __asm
    {
        MRS     r, CPSR
        BIC     r, r, #PSR_IRQ_DISABLE
        MSR     CPSR_c, r
    }
#endif
}

//-----------------------------------------------------------------------------
/** HalDisableIRQ() Disable IRQ
 */
//-----------------------------------------------------------------------------
void HalDisableIRQ(void)
{
    register UINT32 r;

#ifdef __GCC__
    __asm__ ("MRS     %0, CPSR" : "=r" (r));
    __asm__ ("BIC     %0, %1, %2" : "=r" (r) : "r" (r), "r" (PSR_IRQ_DISABLE));
    __asm__ ("MSR     CPSR_c, %0" : : "r" (r));
#else
    __asm
    {
        MRS     r, CPSR
        ORR     r, r, #PSR_IRQ_DISABLE
        MSR     CPSR_c, r
    }
#endif
}

//-----------------------------------------------------------------------------
/** HalEnableFIQ() Enable FIQ
 */
//-----------------------------------------------------------------------------
void HalEnableFIQ(void)
{
    register UINT32 r;

#ifdef __GCC__
    __asm__ ("MRS     %0, CPSR" : "=r" (r));
    __asm__ ("BIC     %0, %1, %2" : "=r" (r) : "r" (r), "r" (PSR_FIQ_DISABLE));
    __asm__ ("MSR     CPSR_c, %0" : : "r" (r));
#else
    __asm
    {
        MRS     r, CPSR
        BIC     r, r, #PSR_FIQ_DISABLE
        MSR     CPSR_c, r
    }
#endif
}

//-----------------------------------------------------------------------------
/** HalCriticalStart() Enter critical section, disable IRQ and FIQ
 *  @return The current processor status, which must be carried back to \n
 *               HalCritialEnd()
 */
//-----------------------------------------------------------------------------
UINT32 HalCriticalStart(void)
{
    register UINT32 r, s;

#ifdef __GCC__
    __asm__ ("MRS     %0, CPSR" : "=r" (r));
    __asm__ ("ORR     %0, %1, %2" : "=r" (s) : "r" (r), "r" (PSR_IRQ_DISABLE));
    __asm__ ("ORR     %0, %1, %2" : "=r" (s) : "r" (r), "r" (PSR_FIQ_DISABLE));
    __asm__ ("MSR     CPSR_c, %0" : : "r" (s));
#else
    __asm
    {
        MRS     r, CPSR
        ORR     s, r, #(PSR_IRQ_DISABLE | PSR_FIQ_DISABLE)
        MSR     CPSR_c, s
    }
#endif
    return r;
}

//-----------------------------------------------------------------------------
/** HalCritialSemiStart() Enter critical section, disable IRQ (FIQ is still \n
 *                      enabled)
 *  @return The current processor status, which must be carried back to \n
 *               HalCritialEnd()
 */
//-----------------------------------------------------------------------------
UINT32 HalCriticalSemiStart(void)
{
    register UINT32 r, s;

#ifdef __GCC__
    __asm__ ("MRS     %0, CPSR" : "=r" (r));
    __asm__ ("ORR     %0, %1, %2" : "=r" (s) : "r" (r), "r" (PSR_IRQ_DISABLE));
    __asm__ ("MSR     CPSR_c, %0" : : "r" (s));
#else
    __asm
    {
        MRS     r, CPSR
        ORR     s, r, #PSR_IRQ_DISABLE
        MSR     CPSR_c, s
    }
#endif
    return r;
}

//-----------------------------------------------------------------------------
/** HalCritialEnd() Leave critical section, restore IRQ and/or FIQ status
 *  @param u4Flags - The return value of the corresponding \n
 *                     HalCriticalStart() or HalCriticalSemiStart()
 */
//-----------------------------------------------------------------------------
void HalCriticalEnd(UINT32 u4Flags)
{
#ifdef __GCC__
    __asm__ ("MSR     CPSR_c, %0" : : "r" (u4Flags));
#else
    __asm
    {
        MSR     CPSR_c, u4Flags
    }
#endif
}

//-----------------------------------------------------------------------------
/** HalDisableFIQ() Disable FIQ
 */
//-----------------------------------------------------------------------------
void HalDisableFIQ(void)
{
    register UINT32 r;

#ifdef __GCC__
    __asm__ ("MRS     %0, CPSR" : "=r" (r));
    __asm__ ("ORR     %0, %1, %2" : "=r" (r) : "r" (r), "r" (PSR_FIQ_DISABLE));
    __asm__ ("MSR     CPSR_c, %0" : : "r" (r));
#else
    __asm
    {
        MRS     r, CPSR
        ORR     r, r, #PSR_FIQ_DISABLE
        MSR     CPSR_c, r
    }
#endif
}


//===========================================================================
// Cache related functions
//===========================================================================

//-----------------------------------------------------------------------------
/** HalIsICacheEnabled() Check if I-cache enabled
 * @return      : TRUE if I-cache enabled, FALSE if I-cache disabled
 */
//-----------------------------------------------------------------------------
BOOL HalIsICacheEnabled(void)
{
    register UINT32 r;

#ifdef __GCC__
    __asm__ ("MRC   p15, 0, %0, c1, c0, 0" : "=r" (r));
#else
    __asm
    {
        MRC     p15, 0, r, c1, c0, 0
    }
#endif

    return ((r & ICACHE_ENABLE) ? TRUE : FALSE);
}

//-----------------------------------------------------------------------------
/** HalIsDCacheEnabled() Check if D-cache enabled
 *  @return      : TRUE if D-cache enabled, FALSE if D-cache disabled
 */
//-----------------------------------------------------------------------------
BOOL HalIsDCacheEnabled(void)
{
    register UINT32 r;

#ifdef __GCC__
    __asm__ ("MRC   p15, 0, %0, c1, c0, 0" : "=r" (r));
#else
    __asm
    {
        MRC     p15, 0, r, c1, c0, 0
    }
#endif

    return ((r & DCACHE_ENABLE) ? TRUE : FALSE);
}

//-----------------------------------------------------------------------------
/** HalGetICacheSize() Get I-cache size
 *  @return      : The size in byte of I-cache
 */
//-----------------------------------------------------------------------------
UINT32 HalGetICacheSize(void)
{
    return L1_SIZE;
}

//-----------------------------------------------------------------------------
/** HalGetDCacheSize() Get D-cache size
 *  @return      : The size in byte of D-cache
 */
//-----------------------------------------------------------------------------
UINT32 HalGetDCacheSize(void)
{
    return L1_SIZE;
}

//-----------------------------------------------------------------------------
/** HalGetICacheLineSize() Get I-cache line size
 *  @return      : The size in byte
 */
//-----------------------------------------------------------------------------
UINT32 HalGetICacheLineSize(void)
{
    return ICACHE_LINE_SIZE;
}

//-----------------------------------------------------------------------------
/** HalGetDCacheLineSize() Get D-cache line size
 *  @return      : The size in byte
 */
//-----------------------------------------------------------------------------
UINT32 HalGetDCacheLineSize(void)
{
    return DCACHE_LINE_SIZE;
}

//-----------------------------------------------------------------------------
/** HalEnableDCache() Enable D-cache
 */
//-----------------------------------------------------------------------------
void HalEnableDCache(void)
{
    register UINT32 r;

    HalInvalidateDCache();

#ifdef __GCC__
    __asm__ ("MRC     p15, 0, %0, c1, c0, 0" : "=r" (r));
    __asm__ ("ORR     %0, %1, %2" : "=r" (r) : "r" (r), "r" (DCACHE_ENABLE));
    __asm__ ("MCR     p15, 0, %0, c1, c0, 0" : : "r" (r));
    __asm__ ("NOP");
    __asm__ ("NOP");
    __asm__ ("NOP");
    __asm__ ("NOP");
    __asm__ ("NOP");
    __asm__ ("NOP");
    __asm__ ("NOP");
    __asm__ ("NOP");
#else
    __asm
    {
        MRC     p15, 0, r, c1, c0, 0
        ORR     r, r, #DCACHE_ENABLE
        MCR     p15, 0, r, c1, c0, 0
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
    }
#endif
}

//-----------------------------------------------------------------------------
/** HalDisableDCache() Disable D-cache
 */
//-----------------------------------------------------------------------------
void HalDisableDCache(void)
{
    register UINT32 r;

#ifdef __GCC__
    __asm__ ("MRC     p15, 0, %0, c1, c0, 0" : "=r" (r));
    __asm__ ("BIC     %0, %1, %2" : "=r" (r) : "r" (r), "r" (DCACHE_ENABLE));
    __asm__ ("MCR     p15, 0, %0, c1, c0, 0" : : "r" (r));
#else
    __asm
    {
        MRC     p15, 0, r, c1, c0, 0
        BIC     r, r, #DCACHE_ENABLE
        MCR     p15, 0, r, c1, c0, 0
    }
#endif
    // D-cache must be cleaned of dirty data before it is disabled
    HalFlushDCache();
}
#else
    #define HalCriticalStart() 0
    #define HalCriticalEnd(X) UNUSED(X)
#endif //CC_MTK_PRELOADER

//-----------------------------------------------------------------------------
/** HalDisableICache() Disable I-cache
 */
//-----------------------------------------------------------------------------
void HalDisableICache(void)
{
    register UINT32 r;

#ifdef __GCC__
    __asm__ ("MRC     p15, 0, %0, c1, c0, 0" : "=r" (r));
    __asm__ ("BIC     %0, %1, %2" : "=r" (r) : "r" (r), "r" (ICACHE_ENABLE));
    __asm__ ("MCR     p15, 0, %0, c1, c0, 0" : : "r" (r));
#else
    __asm
    {
        MRC     p15, 0, r, c1, c0, 0
        BIC     r, r, #ICACHE_ENABLE
        MCR     p15, 0, r, c1, c0, 0
    }
#endif
}


//-----------------------------------------------------------------------------
/** HalEnableCaches() Enable both I-cache and D-cache
 */
//-----------------------------------------------------------------------------
#ifdef CC_MT5396
//#define CC_ENABLE_XOR_CACHE
#endif
#define CC_ENABLE_FOZ
#define CC_ENABLE_PREFETCH
#ifndef CC_MTK_PRELOADER
void HalEnableCaches(void)
{
    register UINT32 r;

#ifndef CC_FPGA_SIM
#ifndef CC_SECURE_WORLD
    HalInvalidateAllCaches();
#endif
#endif

#ifdef __GCC__
    __asm__ ("MRC     p15, 0, %0, c1, c0, 0" : "=r" (r));
    __asm__ ("ORR     %0, %1, %2" : "=r" (r) : "r" (r), "r" (ICACHE_ENABLE));
    __asm__ ("ORR     %0, %1, %2" : "=r" (r) : "r" (r), "r" (DCACHE_ENABLE));
    __asm__ ("MCR     p15, 0, %0, c1, c0, 0" : : "r" (r));
    __asm__ ("NOP");
    __asm__ ("NOP");
    __asm__ ("NOP");
    __asm__ ("NOP");
    __asm__ ("NOP");
    __asm__ ("NOP");
    __asm__ ("NOP");
    __asm__ ("NOP");
#else
    __asm
    {
        MRC     p15, 0, r, c1, c0, 0
        ORR     r, r, #ICACHE_ENABLE
        ORR     r, r, #DCACHE_ENABLE
        MCR     p15, 0, r, c1, c0, 0
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
    }
#endif

#if (!defined(CC_MTK_LOADER) || defined(CC_FPGA_SIM))
#if defined(CC_ENABLE_PREFETCH)
#ifdef __GCC__
    __asm__ ("MRC     p15, 0, %0, c1, c0, 1" : "=r" (r));
    __asm__ ("ORR     %0, %1, %2" : "=r" (r) : "r" (r), "r" (L1C_PREFETCH));
    __asm__ ("ORR     %0, %1, %2" : "=r" (r) : "r" (r), "r" (L2C_PREFETCH_HINT));
    __asm__ ("MCR     p15, 0, %0, c1, c0, 1" : : "r" (r));
#else
    __asm
    {
        MRC     p15, 0, r, c1, c0, 1
        ORR     r, r, #L1C_PREFETCH
        ORR     r, r, #L2C_PREFETCH_HINT
        MCR     p15, 0, r, c1, c0, 1
    }
#endif
#endif

#if defined(CC_ENABLE_XOR_CACHE)
#ifdef __GCC__
    __asm__ ("MRC     p15, 0, %0, c1, c0, 1" : "=r" (r));
    __asm__ ("ORR     %0, %1, %2" : "=r" (r) : "r" (r), "r" (XOR_CACHE));
    __asm__ ("MCR     p15, 0, %0, c1, c0, 1" : : "r" (r));
#else
    __asm
    {
        MRC     p15, 0, r, c1, c0, 1
        ORR     r, r, #XOR_CACHE
        MCR     p15, 0, r, c1, c0, 1
    }
#endif
#endif //defined(CC_ENABLE_XOR_CACHE)

/*
[30] Early BRESP enable 0 = Early BRESP disabled. This is the default. 1 = Early BRESP enabled.
[29] Instruction prefetch enable 0 = Instruction prefetching disabled. This is the default. 1 = Instruction prefetching enabled.
[28] Data prefetch enable 0 = Data prefetching disabled. This is the default. 1 = Data prefetching enabled.
[24:23] Force write allocate b00 = Use AWCACHE attributes for WA. This is the default.
b01 = Force no allocate, set WA bit always 0.
b10 = Override AWCACHE attributes, set WA bit always 1, all cacheable write misses become write allocated.
b11 = Internally mapped to 00.
[22]Clearing bit 22 in the PL310 Auxiliary Control register (shared attribute override enable) has the side effect of transforming Normal
    Shared Non-cacheable reads into Cacheable no-allocate reads.
[12] Exclusive cache configuration 0 = Disabled. This is the default. 1 = Enabled
[0] Full Line of Zero Enable 0 = Full line of write zero behavior disabled. This is the default. 1 = Full line of write zero behavior
normal:0xf2000104=0x73041001
benchmark:0xf2000104=0x72040001
*/

#if defined(CC_ENABLE_FOZ)
#ifdef __GCC__
    __asm__ ("MRC     p15, 0, %0, c1, c0, 1" : "=r" (r));
    __asm__ ("ORR     %0, %1, %2" : "=r" (r) : "r" (r), "r" (FOZ));
    __asm__ ("MCR     p15, 0, %0, c1, c0, 1" : : "r" (r));
#else
    __asm
    {
        MRC     p15, 0, r, c1, c0, 1
        ORR     r, r, #FOZ
        MCR     p15, 0, r, c1, c0, 1
    }
#endif
#endif //CC_ENABLE_FOZ


#endif
}

//-----------------------------------------------------------------------------
/** HalDisableCaches() Disable both I-cache and D-cache
 */
//-----------------------------------------------------------------------------
void HalDisableCaches(void)
{
    register UINT32 r;

    // D-cache must be cleaned of dirty data before it is disabled
    HalFlushDCache();

#ifdef __GCC__
    __asm__ ("MRC     p15, 0, %0, c1, c0, 0" : "=r" (r));
    __asm__ ("BIC     %0, %1, %2" : "=r" (r) : "r" (r), "r" (ICACHE_ENABLE));
    __asm__ ("BIC     %0, %1, %2" : "=r" (r) : "r" (r), "r" (DCACHE_ENABLE));
    __asm__ ("MCR     p15, 0, %0, c1, c0, 0" : : "r" (r));
#else
    __asm
    {
        MRC     p15, 0, r, c1, c0, 0
        BIC     r, r, #ICACHE_ENABLE
        BIC     r, r, #DCACHE_ENABLE
        MCR     p15, 0, r, c1, c0, 0
    }
#endif
}

//-----------------------------------------------------------------------------
/** HalFlushWriteBuffer() Flush write buffer
 */
//-----------------------------------------------------------------------------
inline void HalFlushWriteBuffer(void)
{
	_DSB();
}


//-----------------------------------------------------------------------------
/** HalInvalidateAllCaches() Invalidate I-cache and D-cache
 */
//-----------------------------------------------------------------------------
void HalInvalidateAllCaches(void)
{
    HalInvalidateICache();
    HalInvalidateDCache();
}


//-----------------------------------------------------------------------------
/** HalInvalidateICacheSingleLine() Invalidate single line (8 words or 32 \n
 *                                  bytes) of I-cache
 *  @param u4Addr  [in] - The address to be invalidated
 */
//-----------------------------------------------------------------------------
void HalInvalidateICacheSingleLine(UINT32 u4Addr)
{
    u4Addr = MVA(u4Addr);

#ifdef __GCC__
    __asm__ ("MCR     p15, 0, %0, c7, c5, 1\n"
             "ISB" : : "r" (u4Addr) : "memory");
#else
    __asm
    {
        MCR     p15, 0, u4Addr, c7, c5, 1
    }
    __ISB();
#endif
}

//-----------------------------------------------------------------------------
/** HalInvalidateDCacheSingleLine() Invalidate single line of D-cache
 *  @param u4Addr  [in] - The address to be invalidated
 */
//-----------------------------------------------------------------------------
void HalInvalidateDCacheSingleLine(UINT32 u4Addr)
{
    u4Addr = MVA(u4Addr);

#ifdef __GCC__
    __asm__ ("MCR     p15, 0, %0, c7, c6, 1" : : "r" (u4Addr));
#else
    __asm
    {
        MCR     p15, 0, u4Addr, c7, c6, 1
    }
#endif
    _DSB();
}

//-----------------------------------------------------------------------------
/** HalFlushDCacheSingleLine() Flush (clean) single line of D-cache
 *  @param u4Addr  [in] - The address to be flushed
 */
//-----------------------------------------------------------------------------
void HalFlushDCacheSingleLine(UINT32 u4Addr)
{
    u4Addr = MVA(u4Addr);

#ifdef __GCC__
    __asm__ ("MCR     p15, 0, %0, c7, c10, 1" : : "r" (u4Addr));
#else
    __asm
    {
        MCR     p15, 0, u4Addr, c7, c10, 1
    }
#endif
    _DSB();
}

//-----------------------------------------------------------------------------
/** HalFlushInvalidateDCacheSingleLine() Flush (clean) and invalidate single \n
 *                  line of D-cache
 *  @param u4Addr  [in] - The address to be flushed
 */
//-----------------------------------------------------------------------------
void HalFlushInvalidateDCacheSingleLine(UINT32 u4Addr)
{
    u4Addr = MVA(u4Addr);

#ifdef __GCC__
    __asm__ ("MCR     p15, 0, %0, c7, c14, 1" : : "r" (u4Addr));
#else
    __asm
    {
        MCR     p15, 0, u4Addr, c7, c14, 1
    }
#endif
    _DSB();
}

//-----------------------------------------------------------------------------
/** HalInvalidateDCacheMultipleLine() Invalidate multiple line of D-cache
 *  @param u4Addr  [in] - The start address to be invalidated
 *  @param u4Size  [in] - The memory size to be invalidated
 */
//-----------------------------------------------------------------------------
void HalInvalidateDCacheMultipleLine(UINT32 u4Addr, UINT32 u4Size)
{
    UINT32 i, j;
    if (u4Size == 0)
    {
        return;
    }
    else
    {
        UINT32 start, end;
        start = u4Addr& ~DCACHE_LINE_SIZE_MSK;
        end = (u4Addr + u4Size + DCACHE_LINE_SIZE_MSK) & ~DCACHE_LINE_SIZE_MSK;
        u4Size = end - start;

        // do L1
        j = MVA(u4Addr);
        for (i=0; i<(u4Size/DCACHE_LINE_SIZE); i++)
        {
#ifdef __GCC__
            __asm__ ("MCR     p15, 0, %0, c7, c6, 1" : : "r" (j));
#else
            __asm
            {
                MCR     p15, 0, j, c7, c6, 1
            }
#endif
            j += DCACHE_LINE_SIZE;
        }
        _DSB();
    }
}

//-----------------------------------------------------------------------------
/** HalFlushDCacheMultipleLine() Flush (clean) multiple line of D-cache
 *  @param u4Addr  [in] - The start address to be flushed
 *  @param u4Size  [in] - The memory size to be invalidated
 */
//-----------------------------------------------------------------------------
void HalFlushDCacheMultipleLine(UINT32 u4Addr, UINT32 u4Size)
{
    UINT32 i, j;
    if (u4Size == 0)
    {
        return;
    }
#ifndef CC_SECURE_WORLD
    else if (u4Size > DCACHE_FLUSH_THRESHOLD)
    {
        HalFlushDCache();
    }
#endif
    else
    {
        UINT32 start, end;
        start = u4Addr& ~DCACHE_LINE_SIZE_MSK;
        end = (u4Addr + u4Size + DCACHE_LINE_SIZE_MSK) & ~DCACHE_LINE_SIZE_MSK;
        u4Size = end - start;

        // do L1
        j = MVA(u4Addr);
        for (i=0; i<(u4Size/DCACHE_LINE_SIZE); i++)
        {
#ifdef __GCC__
            __asm__ ("MCR     p15, 0, %0, c7, c10, 1" : : "r" (j));
#else
            __asm
            {
                MCR     p15, 0, j, c7, c10, 1
            }
#endif
            j += DCACHE_LINE_SIZE;
        }
        _DSB();
    }
}

//-----------------------------------------------------------------------------
/** HalFlushInvalidateDCacheMultipleLine() Flush (clean) and invalidate multiple \n
 *                  line of D-cache
 *  @param u4Addr  [in] - The start address to be flushed
 *  @param u4Size  [in] - The memory size to be invalidated
 */
//-----------------------------------------------------------------------------
void HalFlushInvalidateDCacheMultipleLine(UINT32 u4Addr, UINT32 u4Size)
{
    UINT32 i, j;
    if (u4Size == 0)
    {
        return;
    }
#ifndef CC_SECURE_WORLD
    else if (u4Size > DCACHE_FLUSH_THRESHOLD)
    {
        HalFlushInvalidateDCache();
    }
#endif
    else
    {
        UINT32 start, end;
        start = u4Addr& ~DCACHE_LINE_SIZE_MSK;
        end = (u4Addr + u4Size + DCACHE_LINE_SIZE_MSK) & ~DCACHE_LINE_SIZE_MSK;
        u4Size = end - start;

        // do L1
        j = MVA(u4Addr);
        for (i=0; i<(u4Size/DCACHE_LINE_SIZE); i++)
        {
#ifdef __GCC__
            __asm__ ("MCR     p15, 0, %0, c7, c14, 1" : : "r" (j));
#else
            __asm
            {
                MCR     p15, 0, j, c7, c14, 1
            }
#endif
            j += DCACHE_LINE_SIZE;
        }
        _DSB();

    }
}


//lint -save -e* Don't lint the following two embedded assembly functions

//---------------------------------------------------------------------
/** _FlushDCache Flush (clean) entire D-cache
 */
//-----------------------------------------------------------------------------
void _FlushDCache(void)
// Note: __asm function cannot be static
{
#ifdef __GCC__
		asm volatile("push {r0, r1, r2, r3, r4, r5, r6, r7, r9, r10, r11}\n\t");
        asm volatile(
            "dmb\n\t"
            "mrc p15, 1, r0, c0, c0, 1\n\t"
            "ands    r3, r0, #0x7000000\n\t"
            "mov r3, r3, lsr #23\n\t"
            "beq 5f\n\t" // finished
            "mov r10, #0\n\t"
        "1:\n\t" // flush_levels
            "add r2, r10, r10, lsr #1\n\t"
            "mov r1, r0, lsr r2\n\t"
            "and r1, r1, #7\n\t"
            "cmp r1, #2\n\t"
            "blt 4f\n\t" // skip
#ifdef CONFIG_PREEMPT
//            "save_and_disable_irqs_notrace r9\n\t"
#endif
            "mcr p15, 2, r10, c0, c0, 0\n\t"
            "isb\n\t"
            "mrc p15, 1, r1, c0, c0, 0\n\t"
#ifdef CONFIG_PREEMPT
//            "restore_irqs_notrace r9\n\t"
#endif
            "and r2, r1, #7\n\t"
            "add r2, r2, #4\n\t"
            "ldr r4, =0x3ff\n\t"
            "ands    r4, r4, r1, lsr #3\n\t"
            "clz r5, r4\n\t"
            "ldr r7, =0x7fff\n\t"
            "ands    r7, r7, r1, lsr #13\n\t"
        "2:\n\t" // loop1
            "mov r9, r4\n\t"
        "3:\n\t" // loop2
            "orr r11, r10, r9, lsl r5\n\t"
            "orr r11, r11, r7, lsl r2\n\t"
            "mcr p15, 0, r11, c7, c10, 2\n\t"
            "subs    r9, r9, #1\n\t"
            "bge 3b\n\t" // loop2
            "subs    r7, r7, #1\n\t"
            "bge 2b\n\t" //loop1
        "4:\n\t" // skip
            "add r10, r10, #2\n\t"
            "cmp r3, r10\n\t"
            "bgt 1b\n\t" // flush_levels
        "5:\n\t" // finished
            "mov r10, #0\n\t"
            "mcr p15, 2, r10, c0, c0, 0\n\t"
            "dsb\n\t"
            "isb\n\t"
        ::: "r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7", "r9", "r10", "r11", "cc", "memory");
		asm volatile("pop {r0, r1, r2, r3, r4, r5, r6, r7, r9, r10, r11}\n\t");
#else
    UINT32 vr0, vr1, vr2, vr3, vr4, vr5, vr6, vr7, vr9, vr10, vr11;
    __asm
    {
        bl __DMB
        MRC p15, 1, vr0, c0, c0, 1
        ands    vr3, vr0, 0x7000000
        mov vr3, vr3, lsr 23
        beq finished
        mov vr10, 0
    flush_levels:
        add vr2, vr10, vr10, lsr 1
        mov vr1, vr0, lsr vr2
        and vr1, vr1, 7
        cmp vr1, 2
        blt skip
        mcr p15, 2, vr10, c0, c0, 0
        bl __ISB
        mrc p15, 1, vr1, c0, c0, 0
        and vr2, vr1, 7
        add vr2, vr2, 4
        mov vr4, 0x3ff
        ands    vr4, vr4, vr1, lsr 3
        clz vr5, vr4
        mov vr7, 0x7fff
        ands    vr7, vr7, vr1, lsr 13
    loop1:
        mov vr9, vr4
    loop2:
        orr vr11, vr10, vr9, lsl vr5
        orr vr11, vr11, vr7, lsl vr2
        mcr p15, 0, vr11, c7, c10, 2
        subs    vr9, vr9, 1
        bge loop2
        subs    vr7, vr7, 1
        bge loop1
    skip:
        add vr10, vr10, 2
        cmp vr3, vr10
        bgt flush_levels
    finished:
        mov vr10, 0
        mcr p15, 2, vr10, c0, c0, 0
        bl __DSB
        bl __ISB
    }
#endif
}

//-----------------------------------------------------------------------------
/** _FlushInvalidateDCache() Flush (clean) and invalidate entire D-cache
 */
//-----------------------------------------------------------------------------
void _FlushInvalidateDCache(void)
// Note: __asm function cannot be static
{
#ifdef __GCC__
		asm volatile("push {r0, r1, r2, r3, r4, r5, r6, r7, r9, r10, r11}\n\t");
        asm volatile(
            "dmb\n\t"
            "mrc p15, 1, r0, c0, c0, 1\n\t"
            "ands    r3, r0, #0x7000000\n\t"
            "mov r3, r3, lsr #23\n\t"
            "beq 5f\n\t" // finished
            "mov r10, #0\n\t"
        "1:\n\t" // flush_levels
            "add r2, r10, r10, lsr #1\n\t"
            "mov r1, r0, lsr r2\n\t"
            "and r1, r1, #7\n\t"
            "cmp r1, #2\n\t"
            "blt 4f\n\t" // skip
#ifdef CONFIG_PREEMPT
//            "save_and_disable_irqs_notrace r9\n\t"
#endif
            "mcr p15, 2, r10, c0, c0, 0\n\t"
            "isb\n\t"
            "mrc p15, 1, r1, c0, c0, 0\n\t"
#ifdef CONFIG_PREEMPT
//            "restore_irqs_notrace r9\n\t"
#endif
            "and r2, r1, #7\n\t"
            "add r2, r2, #4\n\t"
            "ldr r4, =0x3ff\n\t"
            "ands    r4, r4, r1, lsr #3\n\t"
            "clz r5, r4\n\t"
            "ldr r7, =0x7fff\n\t"
            "ands    r7, r7, r1, lsr #13\n\t"
        "2:\n\t" // loop1
            "mov r9, r4\n\t"
        "3:\n\t" // loop2
            "orr r11, r10, r9, lsl r5\n\t"
            "orr r11, r11, r7, lsl r2\n\t"
            "mcr p15, 0, r11, c7, c14, 2\n\t"
            "subs    r9, r9, #1\n\t"
            "bge 3b\n\t" // loop2
            "subs    r7, r7, #1\n\t"
            "bge 2b\n\t" //loop1
        "4:\n\t" // skip
            "add r10, r10, #2\n\t"
            "cmp r3, r10\n\t"
            "bgt 1b\n\t" // flush_levels
        "5:\n\t" // finished
            "mov r10, #0\n\t"
            "mcr p15, 2, r10, c0, c0, 0\n\t"
            "dsb\n\t"
            "isb\n\t"
        ::: "r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7", "r9", "r10", "r11", "cc", "memory");
		asm volatile("pop {r0, r1, r2, r3, r4, r5, r6, r7, r9, r10, r11}\n\t");
#else
    UINT32 vr0, vr1, vr2, vr3, vr4, vr5, vr6, vr7, vr9, vr10, vr11;
    __asm
    {
        bl __DMB
        MRC p15, 1, vr0, c0, c0, 1
        ands    vr3, vr0, 0x7000000
        mov vr3, vr3, lsr 23
        beq finished
        mov vr10, 0
    flush_levels:
        add vr2, vr10, vr10, lsr 1
        mov vr1, vr0, lsr vr2
        and vr1, vr1, 7
        cmp vr1, 2
        blt skip
        mcr p15, 2, vr10, c0, c0, 0
        bl __ISB
        mrc p15, 1, vr1, c0, c0, 0
        and vr2, vr1, 7
        add vr2, vr2, 4
        mov vr4, 0x3ff
        ands    vr4, vr4, vr1, lsr 3
        clz vr5, vr4
        mov vr7, 0x7fff
        ands    vr7, vr7, vr1, lsr 13
    loop1:
        mov vr9, vr4
    loop2:
        orr vr11, vr10, vr9, lsl vr5
        orr vr11, vr11, vr7, lsl vr2
        mcr p15, 0, vr11, c7, c14, 2
        subs    vr9, vr9, 1
        bge loop2
        subs    vr7, vr7, 1
        bge loop1
    skip:
        add vr10, vr10, 2
        cmp vr3, vr10
        bgt flush_levels
    finished:
        mov vr10, 0
        mcr p15, 2, vr10, c0, c0, 0
        bl __DSB
        bl __ISB
    }
#endif
}

//-----------------------------------------------------------------------------
/** HalFlushDCache() Flush (clean) entire D-cache
 */
//-----------------------------------------------------------------------------
void HalFlushDCache(void)
{
    UINT32 flag = HalCriticalStart();
    _FlushDCache();
    HalCriticalEnd(flag);
}

//-----------------------------------------------------------------------------
/** HalFlushInvalidateDCache() Flush (clean) and invalidate entire D-cache
 */
//-----------------------------------------------------------------------------
void HalFlushInvalidateDCache(void)
{
    UINT32 flag = HalCriticalStart();
    _FlushInvalidateDCache();
    HalCriticalEnd(flag);
}

//-----------------------------------------------------------------------------
/** HalWaitForInterrupt() Put the CPU into a low-power sleep state until an \n
 *                          interrupt
 */
//-----------------------------------------------------------------------------
void HalWaitForInterrupt(void)
{
    register UINT32 r = 0;

#ifdef __GCC__
    __asm__ ("MCR     p15, 0, %0, c7, c0, 4" : : "r" (r));
#else
    __asm
    {
        MCR     p15, 0, r, c7, c0, 4
    }
#endif
}

//-----------------------------------------------------------------------------
/** HalInstructionMemoryBarrier() IMB that ensures consistency between the \n
 *                                  data and instruction streams
 */
//-----------------------------------------------------------------------------
void HalInstructionMemoryBarrier(void)
{
    volatile UINT32* p;
    UINT32 i;

    // 1. Clean D-cache and drain write buffer
    HalFlushDCache();

    // 2. Synchronize data and instruction streams in level two AHB subsystems
    // by using a nonbuffered store (STR) or a noncached load (LDR)
    p = (volatile UINT32*)DRAM_A_BASE;        // DRAM-A, noncached address
    i = *p;
    UNUSED(i);

    // 3. Invalidate the I-cache
    HalInvalidateICache();
}

//===========================================================================
// MMU related functions
//===========================================================================

//-----------------------------------------------------------------------------
/** HalIsMMUEnabled() Check if MMU enabled
 *  @return TRUE if MMU enabled, FALSE if MMU disabled
 */
//-----------------------------------------------------------------------------
BOOL HalIsMMUEnabled(void)
{
    register UINT32 r;

#ifdef __GCC__
    __asm__ ("MRC   p15, 0, %0, c1, c0, 0" : "=r" (r));
#else
    __asm
    {
        MRC     p15, 0, r, c1, c0, 0
    }
#endif
    return ((r & MMU_ENABLE) ? TRUE : FALSE);
}

//-----------------------------------------------------------------------------
/** HalEnableMMU() Enable MMU
 */
//-----------------------------------------------------------------------------
void HalEnableMMU(void)
{
    register UINT32 r;

    // TLB content is preserved while enabling MMU, thus a TLB invalidation
    // is necessary.
    HalInvalidateTLB();

#ifdef __GCC__
    __asm__ ("MRC     p15, 0, %0, c1, c0, 1" : "=r" (r));
    __asm__ ("ORR     %0, %1, %2" : "=r" (r) : "r" (r), "r" (SMP));
    __asm__ ("MCR     p15, 0, %0, c1, c0, 1" : : "r" (r));
    __asm__ ("MRC     p15, 0, %0, c1, c0, 0" : "=r" (r));
    __asm__ ("ORR     %0, %1, %2" : "=r" (r) : "r" (r), "r" (MMU_ENABLE));
    __asm__ ("MCR     p15, 0, %0, c1, c0, 0" : : "r" (r));
#else
    __asm
    {
        MRC     p15, 0, r, c1, c0, 1
        ORR     r, r, #SMP
        MCR     p15, 0, r, c1, c0, 1
        MRC     p15, 0, r, c1, c0, 0
        ORR     r, r, #MMU_ENABLE
        MCR     p15, 0, r, c1, c0, 0
    }
#endif
    _ISB();
    _DSB();
}


//-----------------------------------------------------------------------------
/** HalDisableMMU() Disable MMU
 */
//-----------------------------------------------------------------------------
void HalDisableMMU(void)
{
    register UINT32 r;

#ifdef __GCC__
    __asm__ ("MRC     p15, 0, %0, c1, c0, 0" : "=r" (r));
    __asm__ ("BIC     %0, %1, %2" : "=r" (r) : "r" (r), "r" (MMU_ENABLE));
    __asm__ ("MCR     p15, 0, %0, c1, c0, 0" : : "r" (r));
    __asm__ ("MRC     p15, 0, %0, c1, c0, 1" : "=r" (r));
    __asm__ ("BIC     %0, %1, %2" : "=r" (r) : "r" (r), "r" (SMP));
    __asm__ ("MCR     p15, 0, %0, c1, c0, 1" : : "r" (r));
#else
    __asm
    {
        MRC     p15, 0, r, c1, c0, 0
        BIC     r, r, #MMU_ENABLE
        MCR     p15, 0, r, c1, c0, 0
        MRC     p15, 0, r, c1, c0, 1
        BIC     r, r, #SMP
        MCR     p15, 0, r, c1, c0, 1
    }
#endif
    _ISB();
    _DSB();
}

//-----------------------------------------------------------------------------
/** HalInvalidateTLB() Invalidate entire TLB
 */
//-----------------------------------------------------------------------------
void HalInvalidateTLB(void)
{
    register UINT32 r = 0;

#ifdef __GCC__
    __asm__ ("MCR     p15, 0, %0, c8, c7, 0" : : "r" (r));
#else
    __asm
    {
        MCR     p15, 0, r, c8, c7, 0
    }
#endif
}

//-----------------------------------------------------------------------------
/**  HalCountLeadingZero() Leave critical section, restore IRQ and/or FIQ status
 *  @param u4Flags - The return value of the corresponding \n
 *                     HalCriticalStart() or HalCriticalSemiStart()
 */
//-----------------------------------------------------------------------------
UINT32 HalCountLeadingZero(UINT32 r1)
{
    register UINT32 r0;
#ifdef __GCC__
    __asm__ ("CLZ     %0, %1" : "=r" (r0) : "r"(r1));
#else
    __asm
    {
        CLZ     r0, r1
    }
#endif
    return r0;
}

//-----------------------------------------------------------------------------
/** SetDomainAccess() Set access privilege of the given doman
 *  @param u4Domain    [in] - Domain number (0 - 15)
 *  @pram  da          [in] - Access privilege
 *  @return      : TRUE on success, FALSE otherwise
 */
//-----------------------------------------------------------------------------
static BOOL SetDomainAccess(UINT32 u4Domain, UINT32 u4Da)
{
    //lint --e{550} Symbol not accessed

    UINT32 u4Mask;
    UINT32 u4Access;
    register UINT32 r;

    // Check if domain out of range
    if (u4Domain > 15)
    {
        return FALSE;
    }

    u4Mask = 3 << (u4Domain * 2);
    u4Access = u4Da << (u4Domain * 2);

#ifdef __GCC__
    __asm__ ("MRC     p15, 0, %0, c3, c0, 0" : "=r" (r));
    __asm__ ("BIC     %0, %1, %2" : "=r" (r) : "r" (r), "r" (u4Mask));
    __asm__ ("ORR     %0, %1, %2" : "=r" (r) : "r" (r), "r" (u4Access));
    __asm__ ("MCR     p15, 0, %0, c3, c0, 0" : : "r" (r));
#else
    __asm
    {
        MRC     p15, 0, r, c3, c0, 0
        BIC     r, r, u4Mask
        ORR     r, r, u4Access
        MCR     p15, 0, r, c3, c0, 0
    }
#endif
    return TRUE;
}

//-----------------------------------------------------------------------------
/** SetPageTableBase() Set base address of page table
 *  @param u4Address   [in] - The base address of page table
 */
//-----------------------------------------------------------------------------
static void SetPageTableBase(UINT32 u4Address)
{
#ifdef __GCC__
    __asm__ ("MCR     p15, 0, %0, c2, c0, 0" : : "r" (u4Address));
#else
    __asm
    {
        MCR     p15, 0, u4Address, c2, c0, 0
    }
#endif
}

//===========================================================================


/**********************************************************************

Memory mapping of MT5371

00000000 - 0fffffff     C   B       0 - 256M        256M    DRAM-B
10000000 - 117fffff     C   B       256M - 280M     24M     PBI-B (Flash)
11800000 - 1fffffff     -   -       280M - 512M     232M    -
20000000 - 200fffff     -   -       512M - 513M     1M      IO
20100000 - 27ffffff     -   -       513M - 640M     127M    -
28000000 - 297fffff     -   -       640M - 664M     24M     PBI-A (Flash)
29800000 - 2fffffff     -   -       664M - 768M     104M    -
30000000 - 3fffffff     -   -       768M - 1024M    256M    DRAM-A
40000000 - 4fffffff     -   B       1024M - 1280M   256M    DRAM-C
50000000 - 5fffffff     C   -       1280M - 1536M   256M    DRAM-D
60000000 - ffffffff     -   -       1536M - 4096M   2560M   -

**********************************************************************/

#ifdef CC_SECURE_WORLD
extern UINT32 TZ_DRAM_START, TZ_DRAM_END;
#endif

//-----------------------------------------------------------------------------
/** CreatePageTable() Create page table
 *  @param u4Addr[in] - Address of page table, shall be aligned to 16K boundary
 */
//-----------------------------------------------------------------------------
static void CreatePageTable(UINT32 u4Addr)
{
    UINT32 i;
    UINT32 *pu4Table = (UINT32*)u4Addr;
#if defined(CC_MTK_LOADER) || defined(CC_FPGA)
    UINT32 u4DramSize = TCMGET_CHANNELA_SIZE() * 0x100000;
#else
#ifdef CC_MT5890
    UINT32 u4DramSize = BSP_GetMemSize() + BSP_GetChBMemSize()+ BSP_GetChCMemSize();
#else
    UINT32 u4DramSize = BSP_GetMemSize();
#endif
#endif
    UINT32 u4SramPageStart = SRAM_START / (1 * 1024 * 1024);

    // Note
    // 1. Unused entries must be reserved.
    // 2. All accessible regions are set to domain 0

    // init all
    for (i = 0; i < 0x1000; i++)
    {
        pu4Table[i] = L1Entry(SECTION, i << 20, 0, L1_XN_BIT, NO_ACCESS);
    }

    // DRAM B 0-0x800M(1G), cachable/bufferable
    #ifdef CC_MT5890
    for (i = 0; i < 0xa00; i++)
    #else
    for (i = 0; i < 0x800; i++)
    #endif //  CC_MT5890
    {
        if ((i << 20) < u4DramSize)
        {
#ifdef CC_SECURE_WORLD
            UINT32 u4NsBit = NS_BIT;

            if ( TZ_DRAM_START != 0 && TZ_DRAM_END != 0)
            {
                u4NsBit = (i >= (TZ_DRAM_START >> 20) && i < (TZ_DRAM_END >> 20)) ? 0 : NS_BIT;
            }
#ifdef CC_DISABLE_SMP
	     // (no smp) - secure/outer non-cache mode
	    pu4Table[i] = L1Entry(SECTION, i << 20, TEX_OUTER_NONCACHE, C_BIT | B_BIT | u4NsBit, USER_NO_ACCESS);
#else
       		// (dual core) - shared/secure/outer non-cache mode
           pu4Table[i] = L1Entry(SECTION, i << 20, TEX_OUTER_NONCACHE, C_BIT | B_BIT | S_BIT | u4NsBit, USER_NO_ACCESS);
#endif
#else
            pu4Table[i] = L1Entry(SECTION, i << 20, TEX_WRITE_ALLOCATE, C_BIT | B_BIT, USER_ACCESS);
#endif
        }
    }
#ifdef CC_MT5882
    // DRAM A 0x800-0xC00M, non-cachable/non-bufferable
    for (i = 0x800; i < 0xc00; i++)
    {
        if (((i-0x800) << 20) < u4DramSize)
        {
            pu4Table[i] = L1Entry(SECTION, i << 20, 0, 0, USER_ACCESS);
        }
    }
#else
	// DRAM A 0xA00-0xE00M, non-cachable/non-bufferable
	   for (i = 0xa00; i < 0xe00; i++)
	   {
		   if (((i-0xa00) << 20) < u4DramSize)
		   {
			   pu4Table[i] = L1Entry(SECTION, i << 20, 0, 0, USER_ACCESS);
		   }
	   }
#endif

    // PBI B 32MB
    for (i = 0xe00; i < (0xe00+32); i++)
    {
        pu4Table[i] = L1Entry(SECTION, i << 20, 0, 0, USER_ACCESS);
    }

    // IO 0xf00-0xf01M, from strongly ordered to non-shared device in CA9
    for (i = 0xf00; i < 0xf01; i++)
    {
        pu4Table[i] = L1Entry(SECTION, i << 20, TEX_NON_SHARED_DEVICE, 0, USER_ACCESS);
    }
#ifdef CC_MT5882
    // CPU register - GIC
    for (i = 0xf10; i < 0xf11; i++)
    {
        pu4Table[i] = L1Entry(SECTION, i << 20, 0, 0, USER_ACCESS);
    }

	 // coresight
    for (i = 0xf20; i < 0xf22; i++)
    {
        pu4Table[i] = L1Entry(SECTION, i << 20, TEX_NON_SHARED_DEVICE, 0, USER_ACCESS);
    }
#else
// coresight
   for (i = 0xf10; i < 0xf12; i++)
   {
	   pu4Table[i] = L1Entry(SECTION, i << 20, TEX_NON_SHARED_DEVICE, 0, USER_ACCESS);
   }

   // CPU register - GIC
   for (i = 0xf20; i < 0xf21; i++)
   {
	   pu4Table[i] = L1Entry(SECTION, i << 20, 0, 0, USER_ACCESS);
   }

#endif
    // Rom code
    for (i = 0xf40; i < 0xf41; i++)
    {
        pu4Table[i] = L1Entry(SECTION, i << 20, 0, 0, USER_ACCESS);
    }

    // PBI A 32MB
    for (i = 0xf80; i < (0xf80+32); i++)
    {
        pu4Table[i] = L1Entry(SECTION, i << 20, 0, 0, USER_ACCESS);
    }

    // SRAM, 0xfb000000 ~ 0xfb006800
    for (i = u4SramPageStart; i < (u4SramPageStart+1); i++)
    {
        pu4Table[i] = L1Entry(SECTION, i << 20, 0, 0, USER_ACCESS);
    }
}

void HalSet1MSectionTable(UINT32 u4PhyAddr, UINT32 u4VirtAdr)
{
    UINT32 *pu4Table = (UINT32*)_u4MMUTableAddress;
    pu4Table[(u4VirtAdr>>20)] = L1Entry(SECTION, u4PhyAddr, TEX_WRITE_ALLOCATE, C_BIT | B_BIT, MANAGER);
}

void HalSet4KPageTable(UINT32 u4PhyAddr, UINT32 u4VirtAdr, UINT32* pu4PageTableAddress)
{
    UINT32 *pu4Table = (UINT32*)_u4MMUTableAddress;
    pu4Table[(u4VirtAdr>>20)] = L1Entry(COARSE_PAGE, (UINT32)pu4PageTableAddress, 0, 0, 0);
    pu4PageTableAddress[(u4VirtAdr>>12)&0xff] = L2Entry(u4PhyAddr, TEX_WRITE_ALLOCATE, C_BIT | B_BIT, MANAGER);
}

UINT32 HalGetMMUTableAddress(void)
{
    return _u4MMUTableAddress;
}

//-----------------------------------------------------------------------------
/** HalInitMMU() Initialize MMU
 *  @param u4Addr[in] - Address of page table, shall be aligned to 16K boundary
 *  @return      : TRUE on success, FALSE otherwise
 */
//-----------------------------------------------------------------------------
BOOL HalResumeMMU(UINT32 u4Addr)
{

	HalInitMMU(u4Addr);
	
	return TRUE;
}

BOOL HalInitMMU(UINT32 u4Addr)
{
    // Check 16K Boundary
    if(u4Addr & 0x3fff)
    {
        return FALSE;
    }

    _u4MMUTableAddress = u4Addr;
#ifndef CC_FPGA_SIM
    // Flush and invalidate caches and TLB

#ifndef CC_SECURE_WORLD
    HalInvalidateAllCaches();
#endif

    HalDisableCaches();
    HalDisableMMU();

    // Setup page table
    CreatePageTable(u4Addr);
#endif
    SetPageTableBase(u4Addr);

    // Enable domain 0 only, set it with access permission enable.
    VERIFY(SetDomainAccess(0, CLIENT));

    // Enable MMU
    HalEnableMMU();
    HalEnableCaches();

// IOMMU test
#if 0
    // this example shows virtual address is continuous but physical address is not
    HalSet1MSectionTable(0x8100000, 0x9000000); //phy=129M, virt=144M
    HalSet1MSectionTable(0x8000000, 0x9100000); //phy=128M, virt=145M
    HalSet1MSectionTable(0x8300000, 0x9200000); //phy=131M, virt=146M
    HalSet1MSectionTable(0x8200000, 0x9300000); //phy=130M, virt=147M
    HalSet1MSectionTable(0x8500000, 0x9400000); //phy=133M, virt=148M
    HalSet1MSectionTable(0x8400000, 0x9500000); //phy=132M, virt=149M

    // For every 1M, you must create a 1K size L2 page table which can contains 256 entries.
    // Each entry present a 4K alignment address mapping.
    // In this example, it use hardcode address for the 2 page table.
    // Please use memory allocate in your verification to avoid the memory overwrite problem.
    // The first table is at 0xa0000000=160M, the second one is at 0xa1000000=161M
    x_memset((void*)0xa000000, 0, 0x400);
    HalSet4KPageTable(0x8500000, 0x9600000, (UINT32*)0xa000000); //phy=133M0K, virt=150M0K, page table[0x96], entry[0]
    HalSet4KPageTable(0x8501000, 0x9601000, (UINT32*)0xa000000); //phy=133M4K, virt=150M4K, page table[0x96], entry[1]
    x_memset((void*)0xa100000, 0, 0x400);
    HalSet4KPageTable(0x8502000, 0x9700000, (UINT32*)0xa100000); //phy=133M8K, virt=151M0K, page table[0x96], entry[0]
    HalSet4KPageTable(0x8503000, 0x9701000, (UINT32*)0xa100000); //phy=133M12K, virt=151M4K, page table[0x96], entry[1]

    // this example shows virtual address is out of physical dram address
    HalSet1MSectionTable(0x8500000, 0x98500000); //phy=133M, virt=out of dram space
    HalSet1MSectionTable(0x8600000, 0x98600000); //phy=134M, virt=out of dram space
    x_memset((void*)0xa200000, 0, 0x400);
    HalSet4KPageTable(0x8700000, 0x98700000, (UINT32*)0xa200000); //phy=135M0K, virt=out of dram space, page table[0x987], entry[0]
    HalSet4KPageTable(0x8701000, 0x98701000, (UINT32*)0xa200000); //phy=135M4K, virt=out of dram space, page table[0x987], entry[1]
#endif

    return TRUE;
}

#endif // CC_MTK_PRELOADER

//-----------------------------------------------------------------------------
/** HalInvalidateDCache() Invalidate entire D-cache
 */
//-----------------------------------------------------------------------------
void HalInvalidateDCache(void)
{
#ifdef __GCC__
		asm volatile("push {r0, r1, r2, r3, r4, r5, r6, r7, r9, r10, r11}\n\t");
        asm volatile(
            "dmb\n\t"
            "mrc p15, 1, r0, c0, c0, 1\n\t"
            "ands    r3, r0, #0x7000000\n\t"
            "mov r3, r3, lsr #23\n\t"
            "beq 5f\n\t" // finished
            "mov r10, #0\n\t"
        "1:\n\t" // flush_levels
            "add r2, r10, r10, lsr #1\n\t"
            "mov r1, r0, lsr r2\n\t"
            "and r1, r1, #7\n\t"
            "cmp r1, #2\n\t"
            "blt 4f\n\t" // skip
#ifdef CONFIG_PREEMPT
//            "save_and_disable_irqs_notrace r9\n\t"
#endif
            "mcr p15, 2, r10, c0, c0, 0\n\t"
            "isb\n\t"
            "mrc p15, 1, r1, c0, c0, 0\n\t"
#ifdef CONFIG_PREEMPT
//            "restore_irqs_notrace r9\n\t"
#endif
            "and r2, r1, #7\n\t"
            "add r2, r2, #4\n\t"
            "ldr r4, =0x3ff\n\t"
            "ands    r4, r4, r1, lsr #3\n\t"
            "clz r5, r4\n\t"
            "ldr r7, =0x7fff\n\t"
            "ands    r7, r7, r1, lsr #13\n\t"
        "2:\n\t" // loop1
            "mov r9, r4\n\t"
        "3:\n\t" // loop2
            "orr r11, r10, r9, lsl r5\n\t"
            "orr r11, r11, r7, lsl r2\n\t"
            "mcr p15, 0, r11, c7, c6, 2\n\t"
            "subs    r9, r9, #1\n\t"
            "bge 3b\n\t" // loop2
            "subs    r7, r7, #1\n\t"
            "bge 2b\n\t" //loop1
        "4:\n\t" // skip
            "add r10, r10, #2\n\t"
            "cmp r3, r10\n\t"
            "bgt 1b\n\t" // flush_levels
        "5:\n\t" // finished
            "mov r10, #0\n\t"
            "mcr p15, 2, r10, c0, c0, 0\n\t"
            "dsb\n\t"
            "isb\n\t"
        ::: "r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7", "r9", "r10", "r11", "cc", "memory");
		asm volatile("pop {r0, r1, r2, r3, r4, r5, r6, r7, r9, r10, r11}\n\t");
#else
    UINT32 vr0, vr1, vr2, vr3, vr4, vr5, vr6, vr7, vr9, vr10, vr11;
    __asm
    {
        bl __DMB
        MRC p15, 1, vr0, c0, c0, 1
        ands    vr3, vr0, 0x7000000
        mov vr3, vr3, lsr 23
        beq finished
        mov vr10, 0
    flush_levels:
        add vr2, vr10, vr10, lsr 1
        mov vr1, vr0, lsr vr2
        and vr1, vr1, 7
        cmp vr1, 2
        blt skip
        mcr p15, 2, vr10, c0, c0, 0
        bl __ISB
        mrc p15, 1, vr1, c0, c0, 0
        and vr2, vr1, 7
        add vr2, vr2, 4
        mov vr4, 0x3ff
        ands    vr4, vr4, vr1, lsr 3
        clz vr5, vr4
        mov vr7, 0x7fff
        ands    vr7, vr7, vr1, lsr 13
    loop1:
        mov vr9, vr4
    loop2:
        orr vr11, vr10, vr9, lsl vr5
        orr vr11, vr11, vr7, lsl vr2
        mcr p15, 0, vr11, c7, c6, 2
        subs    vr9, vr9, 1
        bge loop2
        subs    vr7, vr7, 1
        bge loop1
    skip:
        add vr10, vr10, 2
        cmp vr3, vr10
        bgt flush_levels
    finished:
        mov vr10, 0
        mcr p15, 2, vr10, c0, c0, 0
        bl __DSB
        bl __ISB
    }
#endif
}


#ifndef __GNUC__
//-----------------------------------------------------------------------------
/** Called when DMA on a buffer is about to start.
 *
 *  This function will do proper cache operation.
 *  CPU should not access DMA buffer after this function call.
 *
 *  @param pBuffer:  Virtual address of the buffer
 *  @param size: Size of the buffer.
 *  @param direction: the direction of DMA
 */
//-----------------------------------------------------------------------------
void HalDmaBufferStart(void *pBuffer, UINT32 size, enum HalDmaDirection direction)
{
    switch (direction)
    {
        case HAL_DMA_TO_DEVICE:
            HalFlushDCacheMultipleLine((UINT32)pBuffer, size);
            break;

        case HAL_DMA_FROM_DEVICE:
            HalInvalidateDCacheMultipleLine((UINT32)pBuffer, size);
            break;

        case HAL_DMA_BIDIRECTION:
            HalFlushInvalidateDCacheMultipleLine((UINT32)pBuffer, size);
            break;

        default:
            break;
    }
}

//-----------------------------------------------------------------------------
/**  Called when DMA on a buffer is done.
 *
 *  Device DMA should not access DMA buffer after this function call.
 *
 *  NOTE:
 *     The parameter should match that of BSP_DmaBufferStart
 *
 *  @param pBuffer: Virtual address of the buffer
 *  @param size: Size of the buffer.
 *  @param direction: the direction of DMA
 */
//-----------------------------------------------------------------------------
void HalDmaBufferDone(void *pBuffer, UINT32 size, enum HalDmaDirection direction)
{
    switch (direction)
    {
        case HAL_DMA_FROM_DEVICE:
        case HAL_DMA_BIDIRECTION:
            HalInvalidateDCacheMultipleLine((UINT32)pBuffer, size);
            break;

        case HAL_DMA_TO_DEVICE:
        default:
            break;
    }
}
#endif /* __GNUC__ */

