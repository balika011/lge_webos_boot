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

#ifndef __MUSB_UCOS_AFS_MEMORY_H__
#define __MUSB_UCOS_AFS_MEMORY_H__

#if defined(CONFIG_ARCH_MT85XX)
#include "sys_config.h"
#if !CONFIG_SYS_MEM_PHASE3
#include "x_mem_phase2.h"
#else
#include "x_rtos.h"
#endif
#endif
/*
 * uC/OS/AFS-specific memory abstraction
 * $Revision: #1 $
 */

//  Memory allocation/free debug tracer.
//#define MUSB_MEMDEBUG


//  add for debug memory allocation and free.
/**
 * MGC_AfsMem.
 * memory allocation detail information.
 * @field pAddr     x_mem_alloc() return pointer.
 * @field dwSize   x_mem_alloc() size.
 * @field dwLine   source code line number.
 * @field bFunc     function name.
 * @field bFile       file name.
 */

#ifdef MUSB_MEMDEBUG
 
typedef struct
{
    uint32_t dwSize;
    uint32_t dwLine;
    uint8_t bFunc[30];
    //uint8_t bFile[11];
    void* pAddr;
} MGC_AfsMem;

#define MGC_MEM_ALLOC_ITEM  (200)
extern MGC_AfsMem MGC_Mem[];
extern uint32_t   MGC_Mem_Index;
extern uint32_t   MGC_Mem_Size;

#ifdef CONFIG_DRV_LINUX
int sprintf(char * buf, const char *fmt, ...);
#define SPrintf	sprintf
#endif

#define MUSB_MemAlloc(x) (MGC_Mem[MGC_Mem_Index].pAddr = x_mem_alloc((x)));\
                                                 MUSB_ASSERT(MGC_Mem[MGC_Mem_Index].pAddr); \
                                                 MGC_Mem[MGC_Mem_Index].dwSize = (x); \
                                                 MGC_Mem_Size += (x); \
                                                 MGC_Mem[MGC_Mem_Index].dwLine = __LINE__; \
                                                 SPrintf((CHAR *)MGC_Mem[MGC_Mem_Index].bFunc, "%s", __FUNCTION__); \
                                                 /*SPrintf((CHAR *)MGC_Mem[MGC_Mem_Index].bFile, "%s", __FILE__);*/ \
                                                 { uint32_t m_i;\
                                                    for (m_i = 0; m_i < MGC_Mem_Index; m_i ++) \
                                                    { \
                                                        if (MGC_Mem[m_i].pAddr == NULL) \
                                                        { \
                                                            x_memcpy(&MGC_Mem[m_i], &MGC_Mem[MGC_Mem_Index], sizeof(MGC_AfsMem));  \
                                                            x_memset(&MGC_Mem[MGC_Mem_Index], 0, sizeof(MGC_AfsMem)); \
                                                            break; \
                                                        } \
                                                    } \
                                                    if (m_i >= MGC_Mem_Index) \
                                                    { \
                                                        MGC_Mem_Index ++; \
                                                        MUSB_ASSERT(MGC_Mem_Index < MGC_MEM_ALLOC_ITEM); \
                                                    } \
                                                }                                                
                                                 
#define MUSB_MemRealloc(x, size) (MGC_Mem[MGC_Mem_Index].pAddr = x_mem_realloc((x), (size))); \
                                                MUSB_ASSERT(MGC_Mem[MGC_Mem_Index].pAddr); \
                                                { uint32_t m_i;\
                                                    for (m_i = 0; m_i < MGC_Mem_Index; m_i ++) \
                                                    { \
                                                        if (MGC_Mem[m_i].pAddr == (x)) \
                                                        { \
                                                            MGC_Mem_Size -=  MGC_Mem[m_i].dwSize; \
                                                            MGC_Mem_Size += (size); \
                                                            x_memcpy(&MGC_Mem[m_i], &MGC_Mem[MGC_Mem_Index], sizeof(MGC_AfsMem));  \
                                                            x_memset(&MGC_Mem[MGC_Mem_Index], 0, sizeof(MGC_AfsMem)); \
                                                            break; \
                                                        } \
                                                    } \
                                                    if (m_i >= MGC_Mem_Index) \
                                                    { \
                                                        LOG(1, "MUSB_MemRealloc address error !"); \
                                                        MUSB_ASSERT(0); \
                                                    } \
                                                }

#define MUSB_MemFree(x) MUSB_ASSERT((x)); \
                                                { uint32_t m_i;\
                                                    for (m_i = 0; m_i < MGC_Mem_Index; m_i ++) \
                                                    { \
                                                        if (MGC_Mem[m_i].pAddr == (x)) \
                                                        { \
                                                            MGC_Mem_Size -=  MGC_Mem[m_i].dwSize; \
                                                            x_mem_free((x)); (x) = NULL; \
                                                            x_memset(&MGC_Mem[m_i], 0, sizeof(MGC_AfsMem)); \
                                                            break; \
                                                        } \
                                                    } \
                                                    if (m_i >= MGC_Mem_Index) \
                                                    { \
                                                        LOG(1, "MUSB_MemFree address error !"); \
                                                        MUSB_ASSERT(0); \
                                                    } \
                                                }

#else

#if !defined(CONFIG_ARCH_MT85XX)
#define MUSB_MemAlloc(x)                    x_mem_alloc((x))
#define MUSB_MemRealloc(x, size)        x_mem_realloc((x), (size))
#define MUSB_MemFree(x)                     x_mem_free((x)); (x) = NULL
#else //#if !defined(CONFIG_ARCH_MT85XX)
 #if !CONFIG_SYS_MEM_PHASE3
	#define MUSB_MemAlloc(x)					x_alloc_aligned_mem((x), 32) //x_alloc_aligned_dma_mem
		//#define MUSB_MemRealloc(x, size)		  x_mem_realloc((x), (size))
	#define MUSB_MemFree(x) 					x_free_aligned_mem((x)); (x) = NULL
 #else
    #define MUSB_MemAlloc(x)                    x_alloc_aligned_ch1_mem((x), 32) //x_alloc_aligned_dma_mem
		//#define MUSB_MemRealloc(x, size)		  x_mem_realloc((x), (size))
    #define MUSB_MemFree(x)                     x_free_aligned_ch1_ch2_mem((x)); (x) = NULL
 #endif
#endif // #if !defined(CONFIG_ARCH_MT85XX)

#endif

#define MUSB_MemCopy x_memcpy
#define MUSB_MemSet x_memset

#endif	/* multiple inclusion protection */
