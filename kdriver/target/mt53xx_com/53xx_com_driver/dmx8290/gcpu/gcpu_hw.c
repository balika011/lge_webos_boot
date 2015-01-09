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
 * $RCSfile: dmx_api.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file gcpu_hw.c
 *  gcpu driver - hw access
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#include "gcpu.h"
#include "gcpu_dram.h"
#include "x_bim.h"
#include "x_hal_926.h"

#include "gcpu_iram0_90.h"
#include "gcpu_iram1_98.h"
#include "tz_if.h"

#if defined(CC_SUPPORT_STR) && !defined(CC_UBOOT) && !defined(CC_MTK_LOADER)
#include "dmx_drm_drvif.h"
#ifndef CC_TRUSTZONE_SUPPORT
#include "dmx.h"
#endif /* !CC_TRUSTZONE_SUPPORT */
#endif

#ifndef CC_MTK_LOADER
#ifdef __KERNEL__
#include <linux/semaphore.h>
#endif
#endif
//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
#define GCPU_TIMEOUT        1000

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------
#ifndef CC_MTK_LOADER
static HANDLE_T _hGcpuHwSema = NULL_HANDLE;
static HANDLE_T _hGcpuIrqSema = NULL_HANDLE;
#if defined(CC_TRUSTZONE_SUPPORT) && !defined(CC_UBOOT)
static GCPU_TZ_PARAM_T *_prGcpuTzParam = NULL;
static UINT32 _u4GcpuTzParamSize;
static GCPU_HW_CMD_T *_prGcpuHwCmd = NULL;
static UINT32 _u4GcpuHwCmdSize;
#endif /* (CC_TRUSTZONE_SUPPORT && !CC_UBOOT) */
#endif /* !CC_MTK_LOADER */
static BOOL fgPageFault = FALSE;
static UINT32 _u4Mode = 0;

#if defined(CC_SUPPORT_STR) && !defined(CC_UBOOT) && !defined(CC_MTK_LOADER)
#ifndef CC_TRUSTZONE_SUPPORT
static UINT32 _arSecureKeyHoles[GCPU_SECURESLOT_NUM];
static BOOL _GCPU_Hw_SaveSecureKeys(UINT32 u4SlotPtr, UINT32 u4Key);
#endif
static BOOL _GCPU_Hw_RestoreSecureKeys(void);
#endif
//-----------------------------------------------------------------------------
// Extern variables
//-----------------------------------------------------------------------------
#ifdef __KERNEL__
extern struct semaphore aes_api;
extern struct semaphore aes_sema;
extern BOOL fgNativeAESISR;
#endif

//-----------------------------------------------------------------------------
// Extern functions
//-----------------------------------------------------------------------------
extern int x_kmem_sync_table(void *ptr,size_t size);

//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------

static BOOL _Gcpu_Hw_GetRamCode(BYTE **ppbRamCode, UINT32 *pu4RamCodeSize, BYTE **ppbTrapping, UINT32 *pu4TrapSize)
{
    if ((ppbRamCode == NULL) || (ppbTrapping == NULL) || (pu4RamCodeSize == NULL) || (pu4TrapSize == NULL))
    {
        return FALSE;
    }

    // for Oryx 1st verstion
    if(BSP_GetIcVersion() == IC_VER_5890_AA)
    {
        *ppbRamCode = (BYTE*) GCPU_IRAM0_CODE;
        *ppbTrapping = (BYTE*) GCPU_IRAM0_TRAPPING;

        *pu4RamCodeSize = sizeof(GCPU_IRAM0_CODE);
        *pu4TrapSize = sizeof(GCPU_IRAM0_TRAPPING);
    }
    else // B version IC uses the same code
    {
        *ppbRamCode = (BYTE*) GCPU_IRAM1_CODE;
        *ppbTrapping = (BYTE*) GCPU_IRAM1_TRAPPING;

        *pu4RamCodeSize = sizeof(GCPU_IRAM1_CODE);
        *pu4TrapSize = sizeof(GCPU_IRAM1_TRAPPING);
    }
    
    return TRUE;
}

static BOOL _Gcpu_Hw_SetResetParam(GCPU_TZ_PARAM_T *prTzParam)
{
    static UINT32 u4PhyDramCodeAddr = 0;
    static UINT32 u4PhyRamCodeAddr = 0;
    static UINT32 u4PhyTrapAddr = 0;
    static UINT32 u4RamCodeSize = 0, u4TrapSize = 0;
    BYTE *pbRamDstCode = NULL, *pbTrappingDst = NULL;
    BYTE *pbRamSrcCode = NULL, *pbTrappingSrc = NULL;
    BYTE *pbDramCodeAddr = NULL;
    BOOL fgInitRam = FALSE, fgInitDram = FALSE;
    BOOL fgRet = TRUE;

    if (prTzParam == NULL)
    {
        return FALSE;
    }

    x_memset((void *)prTzParam, 0, sizeof(GCPU_TZ_PARAM_T)); 
    if(_u4Mode == 0)
    {
        fgInitRam = TRUE;
    }
    else
    {
        fgInitDram = TRUE;
    }
    
    prTzParam->u4InitRam = (UINT32)fgInitRam;
    prTzParam->u4InitDram = (UINT32)fgInitDram;

    if (fgInitRam)
    {
        if ((0 == u4PhyRamCodeAddr) && (0 == u4PhyTrapAddr))
        {
            fgRet = _Gcpu_Hw_GetRamCode((BYTE **)&pbRamSrcCode, (UINT32 *)&u4RamCodeSize,
                                        (BYTE **)&pbTrappingSrc, (UINT32 *)&u4TrapSize);
            
            if (!fgRet)
            {
                LOG(1, "Can't get ram and grap code\n");
                goto lbExit1;
            }

#if defined(CC_TRUSTZONE_SUPPORT) && !defined(CC_UBOOT) && !defined(CC_MTK_LOADER)
            u4PhyRamCodeAddr = BSP_AllocAlignedDmaMemory(TZ_CACHE_ALIGN(u4RamCodeSize), GCPU_FIFO_ALIGNMENT);
            if (u4PhyRamCodeAddr == 0)
            {
                fgRet = FALSE;
                LOG(1, "Can't allocate memory for ram code\n");
                goto lbExit1;
            }

            u4PhyTrapAddr = BSP_AllocAlignedDmaMemory(TZ_CACHE_ALIGN(u4TrapSize), GCPU_FIFO_ALIGNMENT);
            if (u4PhyTrapAddr == 0)
            {
                fgRet = FALSE;
                LOG(1, "Can't allocate memory for trapping\n");
                goto lbExit1;
            }
            
            pbRamDstCode = (BYTE *)VIRTUAL(u4PhyRamCodeAddr);
            x_memcpy((void *)pbRamDstCode, (void *)pbRamSrcCode, u4RamCodeSize);
            
            pbTrappingDst = (BYTE *)VIRTUAL(u4PhyTrapAddr);
            x_memcpy((void *)pbTrappingDst, (void *)pbTrappingSrc, u4TrapSize);

            HalFlushInvalidateDCacheMultipleLine((UINT32)pbRamDstCode, TZ_CACHE_ALIGN(u4RamCodeSize));
            HalFlushInvalidateDCacheMultipleLine((UINT32)pbTrappingDst, TZ_CACHE_ALIGN(u4TrapSize));
#else /* !(CC_TRUSTZONE_SUPPORT && !CC_UBOOT && !CC_MTK_LOADER) */
            u4PhyRamCodeAddr = (UINT32)pbRamSrcCode;
            u4PhyTrapAddr = (UINT32)pbTrappingSrc;
#endif /* !(CC_TRUSTZONE_SUPPORT && !CC_UBOOT && !CC_MTK_LOADER) */
        }

        prTzParam->pu4RamCode = (UINT32 *)u4PhyRamCodeAddr; 
        prTzParam->pu4Trapping = (UINT32 *)u4PhyTrapAddr ; 
        prTzParam->u4RamCodeSize = u4RamCodeSize / 4;
        prTzParam->u4TrapSize = u4TrapSize / 4;
    }
    
#ifndef CC_MTK_LOADER
    if (fgInitDram)
    {
        if (u4PhyDramCodeAddr == 0)
        { 
            u4PhyDramCodeAddr = BSP_AllocAlignedDmaMemory(GCPU_DRAM_LEN, GCPU_FIFO_ALIGNMENT);
            if (u4PhyDramCodeAddr == 0)
            {
                fgRet = FALSE;
                LOG(1, "Can't allocate memory for Dram code\n");
            }
        }
       
        if (fgRet)
        {
            prTzParam->u4PhyDramCodeAddr = u4PhyDramCodeAddr;
            pbDramCodeAddr = (BYTE *)VIRTUAL(u4PhyDramCodeAddr);
            x_memcpy((void *)pbDramCodeAddr, (void *)GCPU_DRAM_CODE, GCPU_DRAM_LEN);
            HalFlushInvalidateDCacheMultipleLine((UINT32)pbDramCodeAddr, GCPU_DRAM_LEN);
        }
    }
#endif
    UNUSED(u4PhyDramCodeAddr);
    UNUSED(pbDramCodeAddr);
    UNUSED(GCPU_DRAM_CODE);
    UNUSED(u4PhyRamCodeAddr);
    UNUSED(u4PhyTrapAddr);
    UNUSED(pbTrappingDst);
    UNUSED(pbRamDstCode);

    return TRUE;

lbExit1:

#if defined(CC_TRUSTZONE_SUPPORT) && !defined(CC_UBOOT) && !defined(CC_MTK_LOADER)
    if (u4PhyTrapAddr)
    {
        VERIFY(BSP_FreeAlignedDmaMemory(u4PhyTrapAddr));
        u4PhyTrapAddr = 0;
    }

    if (u4PhyRamCodeAddr)
    {
        VERIFY(BSP_FreeAlignedDmaMemory(u4PhyRamCodeAddr));
        u4PhyRamCodeAddr = 0;
    }

    if (u4PhyDramCodeAddr)
    {
        VERIFY(BSP_FreeAlignedDmaMemory(u4PhyDramCodeAddr));
        u4PhyDramCodeAddr = 0;
    }
#endif

    return fgRet;
}

//-----------------------------------------------------------------------------
/** _GcpuHWReset
 *  Reset GCPU HW
 */
//-----------------------------------------------------------------------------
#if !defined(CC_TRUSTZONE_SUPPORT) || defined(CC_UBOOT) || defined(CC_MTK_LOADER) 
static INT32 _Gcpu_Hw_Reset(GCPU_TZ_PARAM_T *prTzParam)
{
    UINT32 u4Value = 0;
    UINT32 *pu4RamCode = NULL, *pu4Trapping = NULL;
    UINT32 u4RamCodeSize = 0, u4TrapSize = 0;
    UINT32 u4PhyDramCodeAddr = 0;
    BOOL fgInitRam, fgInitDram;

    if (prTzParam == NULL)
    {
        return E_GCPU_PARAM_WRONG;
    }

    pu4RamCode = (UINT32 *)VIRTUAL((UINT32)prTzParam->pu4RamCode);
    pu4Trapping = (UINT32 *)VIRTUAL((UINT32)prTzParam->pu4Trapping);
    u4RamCodeSize = prTzParam->u4RamCodeSize;
    u4TrapSize = prTzParam->u4TrapSize;
    u4PhyDramCodeAddr = prTzParam->u4PhyDramCodeAddr;
    fgInitRam = (BOOL)prTzParam->u4InitRam;
    fgInitDram = (BOOL)prTzParam->u4InitDram;

    u4Value = GCPUCMD_READ32(GCPU_REG_CTL);
    u4Value &= ~(0x7);
    GCPUCMD_WRITE32(GCPU_REG_CTL, u4Value);
    u4Value &= ~(0x8);
    GCPUCMD_WRITE32(GCPU_REG_CTL, u4Value);

    u4Value |= 0xF;
    GCPUCMD_WRITE32(GCPU_REG_CTL, u4Value);

    if ((fgInitRam) && 
        (pu4RamCode != NULL) && (u4RamCodeSize != 0) &&
        (pu4Trapping != NULL) && (u4TrapSize != 0))
    {
        //Init Ram Code
        UINT32 i;

        // clear
        GCPUCMD_WRITE32(GCPU_REG_MEM_ADDR, 0x80002000);
        for (i = 0; i < 1024; i++)
        {
            GCPUCMD_WRITE32(GCPU_REG_MEM_DATA, 0);
        }
    
        GCPUCMD_WRITE32(GCPU_REG_MEM_ADDR, 0x80002000);
        for (i = 0; i < u4RamCodeSize; i++)
        {
            GCPUCMD_WRITE32(GCPU_REG_MEM_DATA, *(pu4RamCode + i));
        }

        // trapping
        for (i = 0; i < u4TrapSize; i++)
        {
            GCPUCMD_WRITE32((GCPU_REG_TRAP_START + i), *(pu4Trapping + i));
        }
    }

#ifndef CC_MTK_LOADER
    if ((fgInitDram) && (u4PhyDramCodeAddr != 0))
    {
        //Init Dram Code
        GCPUCMD_WRITE32(GCPU_REG_MSC, GCPUCMD_READ32(GCPU_REG_MSC) | (1 << 12));
        GCPUCMD_WRITE32(GCPU_REG_DRAM_INST_BASE, u4PhyDramCodeAddr);
    }
#else
    UNUSED(fgInitDram);
    UNUSED(u4PhyDramCodeAddr );
#endif

    return S_GCPU_OK;
}
#endif  /* (!CC_TRUSTZONE_SUPPORT || CC_UBOOT || CC_MTK_LOADER) */


void GCPU_HW_LOCK(void)
{
#ifndef CC_MTK_LOADER
    #ifdef __KERNEL__
    if(fgNativeAESISR)
    {
        down(&aes_api);
    }
    else
    #endif
    {
        VERIFY(x_sema_lock(_hGcpuHwSema, X_SEMA_OPTION_WAIT) == OSR_OK);
    }
#endif
}


void GCPU_HW_UNLOCK(void)
{
#ifndef CC_MTK_LOADER
    #ifdef __KERNEL__
    if(fgNativeAESISR)
    {
        up(&aes_api);
    }
    else
    #endif
    {
        VERIFY(x_sema_unlock(_hGcpuHwSema) == OSR_OK);
    }
#endif
}


static void GCPU_IRQ_LOCK(void)
{
#ifndef CC_MTK_LOADER
#ifdef __KERNEL__
#if !defined(CC_TRUSTZONE_SUPPORT) || defined(CC_UBOOT)
    GCPU_TZ_PARAM_T rTzParam;
#endif

    if(fgNativeAESISR)
    {
        if(down_timeout(&aes_sema, GCPU_TIMEOUT/5) != 0)    // 1 tick = 5 ms
        {
            LOG(3, "Timeout\n");
#if defined(CC_TRUSTZONE_SUPPORT) && !defined(CC_UBOOT)
            if (_Gcpu_Hw_SetResetParam(_prGcpuTzParam))
            {
                TZ_GCPU_Hw_Reset(_prGcpuTzParam, _u4GcpuTzParamSize);
            }
            else
            {
                LOG(1, "GCPU invalid reset parameters\n");
            }
#else  /* !(CC_TRUSTZONE_SUPPORT && !CC_UBOOT) */
            if (_Gcpu_Hw_SetResetParam(&rTzParam))
            {
                _Gcpu_Hw_Reset(&rTzParam);
            }
            else
            {
                LOG(1, "GCPU invalid reset parameters\n");
            }
#endif  /* !(CC_TRUSTZONE_SUPPORT && !CC_UBOOT) */
        }
    }
    else
#endif // __KERNEL__
    {
        VERIFY(x_sema_lock(_hGcpuIrqSema, X_SEMA_OPTION_WAIT) == OSR_OK);
        /*
        INT32 i4Ret;
        i4Ret = x_sema_lock_timeout(_hGcpuIrqSema, GCPU_TIMEOUT);
        if(i4Ret != OSR_OK)
        {
            if(i4Ret == OSR_TIMEOUT)
            {
                LOG(3, "Timeout\n");
                _Gcpu_Hw_Reset();
            }
            else
            {
                ASSERT(0);
            }
        }
        */
    }

#else
    while((GCPUCMD_READ32(GCPU_REG_INT_SET) & 0x1) == 0x0);
    GCPUCMD_WRITE32(GCPU_REG_INT_CLR, GCPUCMD_READ32(GCPU_REG_INT_CLR));
    GCPUCMD_WRITE32(GCPU_REG_INT_EN, 0);
#endif //CC_MTK_LOADER
}


#ifndef CC_MTK_LOADER
static void GCPU_IRQ_UNLOCK(void)
{
    VERIFY(x_sema_unlock(_hGcpuIrqSema) == OSR_OK);
}
#endif

//-----------------------------------------------------------------------------
/** _GCPU_IrqHandle()
 *  @param u2Vector: The IRQ vector, must be VECTOR_UP0
 */
//-----------------------------------------------------------------------------
#ifndef CC_MTK_LOADER
static VOID _Gcpu_IrqHandle(UINT16 u2Vector)
{
#if !defined(CC_TRUSTZONE_SUPPORT) || defined(CC_UBOOT)
    GCPU_TZ_PARAM_T rTzParam;
#endif
    
    ASSERT(u2Vector == VECTOR_UP0);
    if (!BIM_IsIrqPending(VECTOR_UP0))
    {
        LOG(3, "Spurious gcpu global interrupt!\n");
        return;
    }

#if !defined(__linux__)
    // Check if it's aes interrupt
    while (BIM_IsIrqPending(VECTOR_UP0))
#endif
    {
#if defined(CC_TRUSTZONE_SUPPORT) && !defined(CC_UBOOT)
        if (_Gcpu_Hw_SetResetParam(_prGcpuTzParam))
        {
            TZ_GCPU_IrqHandle(_prGcpuTzParam, _u4GcpuTzParamSize);
            if (_prGcpuTzParam->u4ReturnedValue != 0)
            {
                LOG(0, "invalid command\n");
            }
            else
            {
                GCPU_IRQ_UNLOCK();
            }
            VERIFY(BIM_ClearIrq(VECTOR_UP0));
        }
        else
        {
            LOG(1, "GCPU invalid reset parameters\n");
            // Clear interrupt
            TZ_GCPU_ISR_ClearIRQ();
            VERIFY(BIM_ClearIrq(VECTOR_UP0));
            GCPU_IRQ_UNLOCK();
        }
#else  /* !(CC_TRUSTZONE_SUPPORT && !CC_UBOOT) */
        if (_Gcpu_Hw_SetResetParam(&rTzParam))
        {
            if(GCPUCMD_READ32(GCPU_REG_INT_CLR) & 0x80000000)
            {
                LOG(0, "invalid command\n");
                _Gcpu_Hw_Reset(&rTzParam);
                VERIFY(BIM_ClearIrq(VECTOR_UP0));

                // Clear/Enable GCPU Interrupt
                GCPUCMD_WRITE32(GCPU_REG_INT_CLR, GCPUCMD_READ32(GCPU_REG_INT_CLR));
                GCPUCMD_WRITE32(GCPU_REG_INT_EN, GCPU_INT_MASK);

                // GCPU PC
                GCPUCMD_WRITE32(GCPU_REG_PC_CTL, 0x0);
            }
            else
            {
                // Clear interrupt
                GCPUCMD_WRITE32(GCPU_REG_INT_CLR, GCPUCMD_READ32(GCPU_REG_INT_CLR));
                GCPUCMD_WRITE32(GCPU_REG_INT_EN, 0);
                VERIFY(BIM_ClearIrq(VECTOR_UP0));

                GCPU_IRQ_UNLOCK();
            }
        }
        else
        {
            LOG(1, "GCPU invalid reset parameters\n");
            // Clear interrupt
            GCPUCMD_WRITE32(GCPU_REG_INT_CLR, GCPUCMD_READ32(GCPU_REG_INT_CLR));
            GCPUCMD_WRITE32(GCPU_REG_INT_EN, 0);
            VERIFY(BIM_ClearIrq(VECTOR_UP0));

            GCPU_IRQ_UNLOCK();
        }
#endif  /* !(CC_TRUSTZONE_SUPPORT && !CC_UBOOT) */
    }
}
#endif

#ifndef CC_MTK_LOADER
static void _Gcpu_IOMMU_IrqHandle(UINT16 u2Vector)
{
#if !defined(CC_TRUSTZONE_SUPPORT) || defined(CC_UBOOT)
    GCPU_TZ_PARAM_T rTzParam;
#endif

    ASSERT(u2Vector == VECTOR_MMU_GCPU);
    if (!BIM_IsIrqPending(VECTOR_MMU_GCPU))
    {
        LOG(3, "Spurious gcpu iommu global interrupt!\n");
        return;
    }

#if !defined(__linux__)
    // Check if it's aes interrupt
    while (BIM_IsIrqPending(VECTOR_MMU_GCPU))
#endif
    {
        fgPageFault = TRUE;
#if defined(CC_TRUSTZONE_SUPPORT) && !defined(CC_UBOOT)
        if (_Gcpu_Hw_SetResetParam(_prGcpuTzParam))
        {
            TZ_GCPU_IOMMU_IrqHandle(_prGcpuTzParam, _u4GcpuTzParamSize);
        }
        else
        {
            LOG(1, "GCPU invalid reset parameters\n");
            TZ_GCPU_ISR_ClearIommuIRQ();
        }
       
        VERIFY(BIM_ClearIrq(u2Vector));

        GCPU_IRQ_UNLOCK();
#else  /* !(CC_TRUSTZONE_SUPPORT && !CC_UBOOT) */
        if (_Gcpu_Hw_SetResetParam(&rTzParam))
        {
            _Gcpu_Hw_Reset(&rTzParam);
        }
        else
        {
            LOG(1, "GCPU invalid reset parameters\n");
        }
        IOMMU_GCPU_WRITE32(REG_IOMMU_CFG4, IOMMU_GCPU_READ32(REG_IOMMU_CFG4) | 0x06000005);
        IOMMU_GCPU_WRITE32(REG_IOMMU_CFG4, IOMMU_GCPU_READ32(REG_IOMMU_CFG4) & ~(0x0610000F));
        
        VERIFY(BIM_ClearIrq(u2Vector));

        GCPU_IRQ_UNLOCK();
#endif  /* !(CC_TRUSTZONE_SUPPORT && !CC_UBOOT) */
    }
}
#endif

//-----------------------------------------------------------------------------
/** _GCPU_InitIsr
*/
//-----------------------------------------------------------------------------
static void _Gcpu_InitIsr(void)
{
#ifndef CC_MTK_LOADER
    static BOOL _fgInit = FALSE;
    x_os_isr_fct pfnOldIsr;

    if(!_fgInit)
    {
        #ifdef __KERNEL__
        if(!fgNativeAESISR) // only initial ISR when native AES did not do it
        #endif
        {
            VERIFY(OSR_OK == x_sema_create(&_hGcpuIrqSema,
                X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK));

            // GCPU interrupt
#if defined(CC_TRUSTZONE_SUPPORT) && !defined(CC_UBOOT)
            TZ_GCPU_ClearIRQ();
#else
            GCPUCMD_WRITE32(GCPU_REG_INT_CLR, GCPUCMD_READ32(GCPU_REG_INT_CLR));
            GCPUCMD_WRITE32(GCPU_REG_INT_EN, 0);
#endif

            BIM_ClearIrq(VECTOR_UP0);
            if (x_reg_isr(VECTOR_UP0, _Gcpu_IrqHandle, &pfnOldIsr) != OSR_OK)
            {
                LOG(0, "Error: fail to register GCPU ISR!\n");
            }

            // GCPU IOMMU interrupt
            BIM_ClearIrq(VECTOR_MMU_GCPU);
            if (x_reg_isr(VECTOR_MMU_GCPU, _Gcpu_IOMMU_IrqHandle, &pfnOldIsr) != OSR_OK)
            {
                LOG(0, "Error: fail to register GCPU IOMMU ISR!\n");
            }
        }

        UNUSED(pfnOldIsr);

        _fgInit = TRUE;
    }
    else
    {
        #ifdef __KERNEL__
        if(!fgNativeAESISR) // only initial ISR when native AES did not do it
        #endif
        {
            INT32 i4Ret;
            // Make sure the semaphore is reset to the locked state.
            i4Ret = x_sema_lock(_hGcpuIrqSema, X_SEMA_OPTION_NOWAIT);
            if (i4Ret == OSR_OK)
            {
                LOG(6, "Reset GCPU semaphore to the Locked state!\n");
            }
            else if (i4Ret == OSR_WOULD_BLOCK)
            {
                LOG(6, "GCPU semaphore is in the Locked state!\n");
            }
            else
            {
                LOG(6, "%d: Semaphore API failed!\n", __LINE__);
            }
        }
    }
#endif
}

static INT32 _GCPU_Hw_RunCmd(GCPU_HW_CMD_T *prHwCmd)
{
    UINT32 i;
    INT32 i4Result = S_GCPU_OK;

#if defined(CC_TRUSTZONE_SUPPORT) && !defined(CC_UBOOT) && !defined(CC_MTK_LOADER)
    *_prGcpuHwCmd = *prHwCmd;

    if (!TZ_GCPU_Hw_CmdRun(_prGcpuHwCmd, _u4GcpuHwCmdSize))
    {
        Printf("%s - TZ run error\n", __FUNCTION__);
        i4Result = E_GCPU_CMD_FAIL;
        goto lbExit1;
    }
#else
    // Setup Parameter
    for(i = 1; i < GCPU_PARAM_NUM; i++)
    {
        GCPUCMD_WRITE32(GCPU_REG_MEM_CMD + i, prHwCmd->u4Param[i]);
    }

    // Clear/Enable GCPU Interrupt
    GCPUCMD_WRITE32(GCPU_REG_INT_CLR, GCPUCMD_READ32(GCPU_REG_INT_CLR));
    GCPUCMD_WRITE32(GCPU_REG_INT_EN, GCPU_INT_MASK);

    // GCPU Decryption Mode
    GCPUCMD_WRITE32(GCPU_REG_MEM_CMD, prHwCmd->u4Param[0]);

    // GCPU PC
    GCPUCMD_WRITE32(GCPU_REG_PC_CTL, 0x0);
#endif

    GCPU_IRQ_LOCK();
    
    if(fgPageFault == TRUE)
    {
        Printf("GCPU IOMMU has page fault!\n");
        fgPageFault = FALSE;
        i4Result = E_GCPU_PAGE_FAULT;
        goto lbExit1;
    }

#if defined(CC_TRUSTZONE_SUPPORT) && !defined(CC_UBOOT) && !defined(CC_MTK_LOADER)
    if (!TZ_GCPU_Hw_CmdReturn(_prGcpuHwCmd, _u4GcpuHwCmdSize))
    {
        Printf("%s - TZ return error\n", __FUNCTION__);
        i4Result = E_GCPU_CMD_FAIL;
        goto lbExit1;
    }

    *prHwCmd = *_prGcpuHwCmd;
#else  /* !(CC_TRUSTZONE_SUPPORT && !CC_UBOOT && !CC_MTK_LOADER) */
    // read back param 0 - 31
    for(i = 0; i < 32; i++)
    {
        prHwCmd->u4Param[i] = GCPUCMD_READ32(GCPU_REG_MEM_CMD + i);
    }

    // read back param 32 - 47
    for(i = GCPU_PARAM_RET_PTR; i < GCPU_PARAM_RET_PTR + GCPU_PARAM_RET_NUM; i++)
    {
        prHwCmd->u4Param[i] = GCPUCMD_READ32(GCPU_REG_MEM_CMD + i);
    }
#endif  /* !(CC_TRUSTZONE_SUPPORT && !CC_UBOOT && !CC_MTK_LOADER) */

    UNUSED(i);

lbExit1:
    return i4Result;
}

//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------
INT32 _GCPU_EnableIOMMU(UINT32 u4Src1Start, UINT32 u4Src1End, UINT32 u4Src2Start, UINT32 u4Src2End)
{
    INT32 i4Ret = S_GCPU_OK;

#ifndef CC_MTK_LOADER
    UINT32 u4MmuTblAddr;
    u4MmuTblAddr = HalGetMMUTableAddress();

#if defined(CC_TRUSTZONE_SUPPORT) && !defined(CC_UBOOT)
    x_memset((void *)_prGcpuTzParam, 0, sizeof(GCPU_TZ_PARAM_T));
    
    _prGcpuTzParam->u4MmuSrc1Start = u4Src1Start;
    _prGcpuTzParam->u4MmuSrc1End = u4Src1End;
    _prGcpuTzParam->u4MmuSrc2Start = u4Src2Start;
    _prGcpuTzParam->u4MmuSrc2End = u4Src2End;
    _prGcpuTzParam->u4MmuTblAddr = u4MmuTblAddr;

    if (!TZ_GCPU_EnableIOMMU(_prGcpuTzParam, _u4GcpuTzParamSize))
    {
        Printf("Enable GCPI fails\n");
        i4Ret = E_GCPU_FAIL;
    }
#else
    // setup IOMMU
    IOMMU_GCPU_WRITE32(REG_IOMMU_CFG4, 0x0603000A);
    IOMMU_GCPU_WRITE32(REG_IOMMU_CFG4, 0x0010000A);
    IOMMU_GCPU_WRITE32(REG_IOMMU_CFG0, 0xFE | (0x01 << 0));
    IOMMU_GCPU_WRITE32(REG_IOMMU_CFG1, u4MmuTblAddr);
    IOMMU_GCPU_WRITE32(REG_IOMMU_CFG2, 0x00130011);

    IOMMU_GCPU_WRITE32(REG_IOMMU_CFG9, (u4Src1Start & 0xfffff000) | 0x3);
    IOMMU_GCPU_WRITE32(REG_IOMMU_CFGA, (u4Src1End - 1) & 0xfffff000);
    if(u4Src2Start == 0 || u4Src2End == 0)
    {
        IOMMU_GCPU_WRITE32(REG_IOMMU_CFGB, (u4Src1Start & 0xfffff000) | 0x1);
        IOMMU_GCPU_WRITE32(REG_IOMMU_CFGC, (u4Src1End - 1) & 0xfffff000);
    }
    else
    {
        IOMMU_GCPU_WRITE32(REG_IOMMU_CFGB, (u4Src2Start & 0xfffff000) | 0x1);
        IOMMU_GCPU_WRITE32(REG_IOMMU_CFGC, (u4Src2End - 1) & 0xfffff000);
    }

    //fire IOMMU cache
    IOMMU_GCPU_WRITE32(REG_IOMMU_CFG4, IOMMU_GCPU_READ32(REG_IOMMU_CFG4) | (1U << 31));
    while((IOMMU_GCPU_READ32(REG_IOMMU_CFG8) & (1 << 29)) != 0);
#endif
#endif  //end of CC_MTK_LOADER
    return i4Ret;
}

INT32 _GCPU_DisableIOMMU(void)
{
#if defined(CC_TRUSTZONE_SUPPORT) && !defined(CC_UBOOT) && !defined(CC_MTK_LOADER)
    TZ_GCPU_DisableIOMMU();
#else
    IOMMU_GCPU_WRITE32(REG_IOMMU_CFG0, 0);
#endif

    return S_GCPU_OK;
}


INT32 _GCPU_GetRomCodeChecksum(void)
{
    INT32 res = 0;

#if defined(CC_TRUSTZONE_SUPPORT) && !defined(CC_UBOOT) && !defined(CC_MTK_LOADER)
    GCPU_HW_LOCK();
    
    if (_Gcpu_Hw_SetResetParam(_prGcpuTzParam))
    {
        TZ_GCPU_GetRomCodeChecksum(_prGcpuTzParam, _u4GcpuTzParamSize);
        res = (INT32)_prGcpuTzParam->u4ReturnedValue;
    }
    
    GCPU_HW_UNLOCK();
#else  /* !(CC_TRUSTZONE_SUPPORT && !CC_UBOOT && !CC_MTK_LOADER) */
    GCPU_TZ_PARAM_T rTzParam;
    INT32 i, tmp, d0, d1, d2, mask;

    GCPU_HW_LOCK();

    if (_Gcpu_Hw_SetResetParam(&rTzParam))
    {
        _Gcpu_Hw_Reset(&rTzParam);
    }
    else
    {
        LOG(1, "GCPU invalid reset parameters\n");
    }

    _GCPU_Lock();

    mask = 255; //0xff
    res = 0;

    GCPUCMD_WRITE32(GCPU_REG_MEM_ADDR, 0x00000000);

    for (i = 1; i <= GCPU_IMEM_ROM_SIZE; i++)
    {
        tmp = GCPUCMD_READ32(GCPU_REG_MEM_DATA);
        Printf("%04d:%08x\n", i, tmp);

        d0 = tmp & mask;
        d1 = (tmp >> 8) & mask;
        d2 = (tmp >> 16) & mask;
        if((i % 4) == 0)
        {
            tmp = (d2 << 16) + (d1 << 8) + d0;
        }
        if((i % 4) == 1)
        {
            tmp = (d1 << 16) + (d0 << 8) + d2;
        }
        if((i % 4) == 2)
        {
            tmp = (d0 << 16) + (d2 << 8) + d1;
        }
        if((i % 4) == 3)
        {
            tmp = (d0 << 16) + (d1 << 8) + d2;
        }

        res = res ^ tmp;
    }

    _GCPU_Unlock();

    LOG(5, "%x\n", res);

    GCPU_HW_UNLOCK();
#endif  /* !(CC_TRUSTZONE_SUPPORT && !CC_UBOOT && !CC_MTK_LOADER) */

    return res;
}


//-----------------------------------------------------------------------------
/** _GCPU_Hw_Init
 *  Init GCPU HW
 */
//-----------------------------------------------------------------------------
INT32 _GCPU_Hw_Init(UINT32 u4Mode, BOOL fgIsLocked)
{
#if defined(CC_TRUSTZONE_SUPPORT) && !defined(CC_UBOOT) && !defined(CC_MTK_LOADER)
    UINT32 u4BufPAddr;
#else
    GCPU_TZ_PARAM_T rTzParam;
#endif

    #ifndef CC_MTK_LOADER
    // create a semaphore
    if (0 == _hGcpuHwSema)
    {
        if(x_sema_create(&_hGcpuHwSema, X_SEMA_TYPE_MUTEX, X_SEMA_STATE_UNLOCK) != OSR_OK)
        {
            LOG(1, "Fail to create HW semaphore\n");
            return E_GCPU_OS_OPERA_FAIL;
        }
    }
    #if defined(CC_TRUSTZONE_SUPPORT) && !defined(CC_UBOOT)
    if (!_prGcpuTzParam)
    {
        _u4GcpuTzParamSize = TZ_CACHE_ALIGN(sizeof(GCPU_TZ_PARAM_T));
        u4BufPAddr = (UINT32)BSP_AllocAlignedDmaMemory(_u4GcpuTzParamSize, TZ_CACHE_LINE_SIZE);
        _prGcpuTzParam = (GCPU_TZ_PARAM_T *)VIRTUAL(u4BufPAddr);
        if (!_prGcpuTzParam)
        {
            LOG(1, "Unable to allocate GCPU_TZ_PARAM_T\n");
            return E_GCPU_OS_OPERA_FAIL;
        }
    }
    
    if (!_prGcpuHwCmd)
    {
        _u4GcpuHwCmdSize = TZ_CACHE_ALIGN(sizeof(GCPU_HW_CMD_T));
        u4BufPAddr = (UINT32)BSP_AllocAlignedDmaMemory(_u4GcpuHwCmdSize, TZ_CACHE_LINE_SIZE);
        _prGcpuHwCmd = (GCPU_HW_CMD_T *)VIRTUAL(u4BufPAddr);
        if (!_prGcpuHwCmd)
        {
            BSP_FreeAlignedDmaMemory(PHYSICAL((UINT32)_prGcpuTzParam));
            LOG(1, "Unable to allocate GCPU_HW_CMD_T\n");
            return E_GCPU_OS_OPERA_FAIL;
        }
    }
    #endif  /* (CC_TRUSTZONE_SUPPORT && !CC_UBOOT) */
    #endif  /* CC_MTK_LOADER */

    if (!fgIsLocked)
    {
        GCPU_HW_LOCK();
    }

    CKGEN_WRITE32(0x1C8, CKGEN_READ32(0x1C8) | (1 << 20));
    #ifdef CC_DMX_A1
    CKGEN_WRITE32(0x39C, 0x1);  // GCPU Max clock
    #else
    CKGEN_WRITE32(0x39C, 0x2);
    #endif

    // Enable GCPU
    _u4Mode = u4Mode;

#if defined(CC_TRUSTZONE_SUPPORT) && !defined(CC_UBOOT) && !defined(CC_MTK_LOADER)
    if (_Gcpu_Hw_SetResetParam(_prGcpuTzParam))
    {
        TZ_GCPU_Hw_Reset(_prGcpuTzParam, _u4GcpuTzParamSize);
    }
    else
    {
        LOG(1, "GCPU invalid reset parameters\n");
    }
#else  /* !(CC_TRUSTZONE_SUPPORT && !CC_UBOOT && !CC_MTK_LOADER) */
    if (_Gcpu_Hw_SetResetParam(&rTzParam))
    {
        _Gcpu_Hw_Reset(&rTzParam);
    }
    else
    {
        LOG(1, "GCPU invalid reset parameters\n");
    }
#endif /* !(CC_TRUSTZONE_SUPPORT && !CC_UBOOT && !CC_MTK_LOADER) */

    _Gcpu_InitIsr();

    if (!fgIsLocked)
    {
        GCPU_HW_UNLOCK();
    }

    return S_GCPU_OK;
}


INT32 _GCPU_Hw_SetSlot(UINT32 u4Idx, UINT32 u4Data)
{
    GCPU_HW_LOCK();

#if defined(CC_SUPPORT_STR) && !defined(CC_UBOOT) && !defined(CC_MTK_LOADER) && !defined(CC_TRUSTZONE_SUPPORT) 
    _GCPU_Hw_SaveSecureKeys(u4Idx, u4Data);
#endif

#if defined(CC_TRUSTZONE_SUPPORT) && !defined(CC_UBOOT) && !defined(CC_MTK_LOADER)
    x_memset((void *)_prGcpuTzParam, 0, sizeof(GCPU_TZ_PARAM_T));
    
    _prGcpuTzParam->u4SlotIdx = u4Idx;
    _prGcpuTzParam->u4SlotData = u4Data;

    TZ_GCPU_Hw_SetSlot(_prGcpuTzParam, _u4GcpuTzParamSize);
#else
    GCPUCMD_WRITE32(GCPU_REG_MEM_CMD + u4Idx, u4Data);
#endif

    GCPU_HW_UNLOCK();

    return S_GCPU_OK;
}


INT32 _GCPU_Hw_Cmd(GCPU_HW_CMD_T *prHwCmd)
{
    INT32 i4Result = S_GCPU_OK;

    if (prHwCmd == NULL)
    {
        return E_GCPU_PARAM_WRONG;
    } 
  
    GCPU_HW_LOCK();
    
    i4Result = _GCPU_Hw_RunCmd(prHwCmd);

    GCPU_HW_UNLOCK();

    return i4Result;
}

INT32 _GCPU_Hw_CmdEx(GCPU_HW_CMD_T *prHwCmd, const GCPU_IO_BUF_T *prBuf, UINT32 u4Flags) 
{
    INT32 i4Result = S_GCPU_OK;
    BOOL fgUsrland;

    if ((prHwCmd == NULL) || (prBuf == NULL))
    {
        return E_GCPU_PARAM_WRONG;
    }

#if !defined(CC_UBOOT) && !defined(CC_MTK_LOADER) && !defined(CC_DRIVER_BUILD)
    fgUsrland = ((prBuf->u4InBufStart > 0) && (prBuf->u4InBufStart < 0xBF000000)) ? TRUE : FALSE;
#else
    fgUsrland = FALSE;
#endif

    GCPU_HW_LOCK();

    if (u4Flags & GCPU_HWCMD_FLAG_IOMMU)
    {
        _GCPU_EnableIOMMU(prBuf->u4IommuSrc1Start, prBuf->u4IommuSrc1End,
                          prBuf->u4IommuSrc2Start, prBuf->u4IommuSrc2End);
    }
    else
    {
        _GCPU_DisableIOMMU();
    }

    if (u4Flags & GCPU_HWCMD_FLAG_CLEAR_CACHE)
    {
        HalFlushInvalidateDCache();
    }
    else if (!fgUsrland && (u4Flags & GCPU_HWCMD_FLAG_DMA_BUF))
    {
        if ((prBuf->u4InBufStart != 0) && (prBuf->u4InBufSize != 0))
        {
#if !defined(CC_UBOOT) && !defined(CC_MTK_LOADER) && !defined(CC_DRIVER_BUILD)
            x_kmem_sync_table((void *)prBuf->u4InBufStart, prBuf->u4InBufSize);
#endif
            HalDmaBufferStart((void *)prBuf->u4InBufStart, prBuf->u4InBufSize, HAL_DMA_BIDIRECTION);
        }
        
        if ((prBuf->u4OutBufStart != 0) && (prBuf->u4OutBufSize != 0))
        {
#if !defined(CC_UBOOT) && !defined(CC_MTK_LOADER) && !defined(CC_DRIVER_BUILD)
            x_kmem_sync_table((void *)prBuf->u4OutBufStart, prBuf->u4OutBufSize);
#endif
            HalDmaBufferStart((void *)prBuf->u4OutBufStart, prBuf->u4OutBufSize, HAL_DMA_BIDIRECTION);
        }
    }
    
    i4Result = _GCPU_Hw_RunCmd(prHwCmd);

    if (u4Flags & GCPU_HWCMD_FLAG_CLEAR_CACHE)
    {
        HalFlushInvalidateDCache();
    }
    else if (!fgUsrland && (u4Flags & GCPU_HWCMD_FLAG_DMA_BUF))
    {
        if ((prBuf->u4InBufStart != 0) && (prBuf->u4InBufSize != 0))
        {
            HalDmaBufferDone((void *)prBuf->u4InBufStart, prBuf->u4InBufSize, HAL_DMA_BIDIRECTION);
        }
        
        if ((prBuf->u4OutBufStart != 0) && (prBuf->u4OutBufSize != 0))
        {
            HalDmaBufferDone((void *)prBuf->u4OutBufStart, prBuf->u4OutBufSize, HAL_DMA_BIDIRECTION);
        }
    }

    GCPU_HW_UNLOCK();

    return i4Result;
}


#if defined(CC_SUPPORT_STR) && !defined(CC_UBOOT) && !defined(CC_MTK_LOADER)
#ifndef CC_TRUSTZONE_SUPPORT
static BOOL _GCPU_Hw_SaveSecureKeys(UINT32 u4SlotPtr, UINT32 u4Key)
{
    UINT32 u4Idx;

    if (u4SlotPtr >= GCPU_SECURESLOT_PTR + GCPU_SECURESLOT_NUM)
    {
        return FALSE;
    }
    
    u4Idx = u4SlotPtr - GCPU_SECURESLOT_PTR;
    _arSecureKeyHoles[u4Idx] = u4Key;

    return TRUE;
}
#endif

static BOOL _GCPU_Hw_RestoreSecureKeys(void)
{
#ifdef CC_TRUSTZONE_SUPPORT
    TZ_GCPU_Hw_RestoreSecureKeys();
#else  /* !CC_TRUSTZONE_SUPPORT */
    INT32 i;
    UINT32 u4Data;
    UINT8 *p;
    UINT8 au1Key[16] = {};

    _DMX_GetMtdSecureKey(au1Key);

    for (i = 0; i < GCPU_SECURESLOT_NUM; i++)
    {
        u4Data = _arSecureKeyHoles[i];
        GCPUCMD_WRITE32(GCPU_REG_MEM_CMD + GCPU_SECURESLOT_PTR + i, u4Data);
    }

    for(i = 0; i < 4; i++)
    {
        p = au1Key + i * 4;
        u4Data = KEY_WRAPPER(p);
        GCPUCMD_WRITE32(GCPU_REG_MEM_CMD + AES_MTD_SECURE_KEY_PTR + i, u4Data);
    }
#endif /* !CC_TRUSTZONE_SUPPORT */
    
    return TRUE;
}

//#define CC_GCPU_SLOW_DOWN_CLOCK     // we don't really power down gcpu, due to NAND/native driver relation
void _GCPU_pm_str_suspend(void)
{
    #ifdef CC_GCPU_SLOW_DOWN_CLOCK
    CKGEN_WRITE32(0x39C, 0x0);  // GCPU XTAL clock
    #else
    // disable IRQ
    GCPU_HW_LOCK();
    GCPUCMD_WRITE32(GCPU_REG_INT_CLR, GCPUCMD_READ32(GCPU_REG_INT_CLR));
    GCPUCMD_WRITE32(GCPU_REG_INT_EN, 0);

    if (!BIM_DisableIrq(VECTOR_UP0))
    {
        LOG(1, "Disable GCPU isr fail\n");
    }

    if (!BIM_DisableIrq(VECTOR_MMU_GCPU))
    {
        LOG(1, "Disable GCPU IOMMU isr fail\n");
    }

    // power off
    CKGEN_WRITE32(0x1C8, CKGEN_READ32(0x1C8) & ~(1U << 20));
    #endif
}

void _GCPU_pm_str_resume(void)
{
    #ifdef CC_GCPU_SLOW_DOWN_CLOCK
    CKGEN_WRITE32(0x39C, 0x2);  // GCPU Max clock
    #else
    // power on
    _GCPU_Hw_Init(0, TRUE);
    
    _GCPU_Hw_RestoreSecureKeys();

    if (!BIM_EnableIrq(VECTOR_UP0))
    {
        LOG(1, "Eanble GCPU isr fail\n");
    }

    if (!BIM_EnableIrq(VECTOR_MMU_GCPU))
    {
        LOG(1, "Eanble GCPU IOMMU isr fail\n");
    }

    GCPU_HW_UNLOCK();

    // IRQ will be enalbed when executing GCPU command
    // do nothing here
    #endif
}
#endif // CC_SUPPORT_STR

