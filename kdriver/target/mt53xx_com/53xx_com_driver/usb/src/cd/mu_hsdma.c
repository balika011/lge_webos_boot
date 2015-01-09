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

/*
 * DMA implementation for high-speed controllers.
 * $Revision: #1 $
 */

#include "mu_impl.h"

#include "mu_hsdcf.h"

#include "mu_diag.h"
/* NASTY: */
#include "mu_hdrdf.h"
#include "mu_pippr.h"

/****************************** CONSTANTS ********************************/
#if !defined (CONFIG_ARCH_MT85XX)
#include "x_hal_arm.h"
#else
/* MT8530 driver config file */
#include "drv_config.h"
#include "x_hal_ic.h"
#include <mach/cache_operation.h>
extern int printk(const char *format, ...);
#endif

#if !defined (CONFIG_ARCH_MT85XX)
#ifndef MUSB_QMU
#define MGC_O_HSDMA_BASE    0x200
#define MGC_O_HSDMA_INTR    0x200
#endif
#define MGC_O_HSDMA_INTR_MASK    0x201   
#define MGC_O_HSDMA_INTR_CLEAR   0x202
#define MGC_O_HSDMA_INTR_SET     0x203
#else //#if !defined (CONFIG_ARCH_MT85XX)
/****************************** CONSTANTS ********************************/
#define MGC_O_HSDMA_BASE    0x200
#define MGC_O_HSDMA_INTR    0x200
#ifdef UNIFIED_USB
#ifdef CC_MT8560
#define MGC_O_HSDMA_INTR_MASK    0x202
#elif  defined(CC_MT8580)||defined(CC_MT8561) || defined(CC_MT8563)
#define MGC_O_HSDMA_INTR_MASK    0x201
#define MGC_O_HSDMA_INTR_UNMASK_CLR   0x202
#define MGC_O_HSDMA_INTR_UNMASK_SET   0x203
#endif
#endif
#endif //#if !defined (CONFIG_ARCH_MT85XX)

#define MGC_O_HSDMA_CONTROL 4
#define MGC_O_HSDMA_ADDRESS 8
#define MGC_O_HSDMA_COUNT   0xc

#define MGC_HSDMA_CHANNEL_OFFSET(_bChannel, _bOffset) ((MGC_O_HSDMA_BASE + (_bChannel << 4) + _bOffset))

/* control register (16-bit): */
#define MGC_S_HSDMA_ENABLE 0
#define MGC_S_HSDMA_TRANSMIT   1
#define MGC_S_HSDMA_MODE1   2
#define MGC_S_HSDMA_IRQENABLE  3
#define MGC_S_HSDMA_ENDPOINT 4
#define MGC_S_HSDMA_BUSERROR 8
#define MGC_S_HSDMA_BURSTMODE 9
#define MGC_M_HSDMA_BURSTMODE    (3 << MGC_S_HSDMA_BURSTMODE)
#define MGC_HSDMA_BURSTMODE_UNSPEC  0
#define MGC_HSDMA_BURSTMODE_INCR4   1
#define MGC_HSDMA_BURSTMODE_INCR8   2
#define MGC_HSDMA_BURSTMODE_INCR16  3

#if !defined (CONFIG_ARCH_MT85XX)
#define MGC_HSDMA_MIN_DMA_LEN        64
#define MGC_HSDMA_MAX_DMA_LEN        100000
#else
/* Note: Fixed by Ben 2009/07/20 
         For usb rx data pipe could sometimes stops 
         receiving data when wifi is used.
         Change len from 64 to 512.
 */
#define MGC_HSDMA_MIN_DMA_LEN        1 //512//64
#define SMP_CACHE_OPERATION
#endif

/******************************** TYPES **********************************/

#ifdef MUSB_QMU
uint8_t bADataEnable = 0;
#endif
#ifdef MUSB_DATA_COMPARE
uint8_t bMgcDmaEnable = 1;
#endif
#if defined(MUSB_DMA) && defined(MUSB_HSDMA) && (MUSB_HSDMA_CHANNELS > 0)

struct _MGC_HsDmaController;

typedef struct
{
    MUSB_DmaChannel Channel;
    struct _MGC_HsDmaController *pController;
    uint32_t dwStartAddress;
    uint32_t dwStartPhyAddress;
    uint32_t dwCount;
    uint16_t wMaxPacketSize;
    uint8_t bIndex;
    uint8_t bEnd;
    uint8_t bProtocol;
    uint8_t bTransmit;
    uint8_t bMode;
} MGC_HsDmaChannel;

typedef struct _MGC_HsDmaController
{
    MUSB_DmaController Controller;
    MGC_HsDmaChannel aChannel[MUSB_HSDMA_CHANNELS];
    MUSB_pfSystemToBusAddress pfSystemToBusAddress;
    MUSB_pfDmaChannelStatusChanged pfDmaChannelStatusChanged;
    void *pDmaPrivate;
    uint8_t *pCoreBaseIsr;
    uint8_t *pCoreBaseBsr;
    uint8_t bChannelCount;
    uint8_t bmUsedChannels;
} MGC_HsDmaController;
#endif /* MUSB_HSDMA && have at least one channel */

#if !defined (CONFIG_ARCH_MT85XX)
#ifdef MUSB_QMU
extern void _MUSB_QMU_Transfer(uint8_t end_num, uint32_t u4Type, 
	                             uint32_t framenum, uint8_t isHost, uint8_t num_of_packet);
#endif
#endif

/******************************* FORWARDS ********************************/

#if defined(MUSB_DMA) && defined(MUSB_HSDMA) && (MUSB_HSDMA_CHANNELS > 0)
static uint8_t MGC_HsDmaStartController(void *pPrivateData);

#ifndef MUSB_CODE_SHRINK
static uint8_t MGC_HsDmaStopController(void *pPrivateData);
#endif
static MUSB_DmaChannel *MGC_HsDmaAllocateChannel(void *pPrivateData, uint8_t bLocalEnd,
                                                 uint8_t bTransmit, uint8_t bProtocol,
                                                 uint16_t wMaxPacketSize);

static void MGC_HsDmaReleaseChannel(MUSB_DmaChannel *pChannel);
#ifdef MUSB_DMA_BUF

static uint8_t *MGC_HsDmaAllocateBuffer(MUSB_DmaChannel *pChannel, uint32_t dwLength);

static uint8_t MGC_HsDmaReleaseBuffer(MUSB_DmaChannel *pChannel, uint8_t *pBuffer);
#endif

static uint8_t MGC_HsDmaProgramChannel(MUSB_DmaChannel *pChannel, uint16_t wPacketSize,
                                       uint8_t bMode, const uint8_t *pBuffer,
                                       uint32_t dwLength);

static MUSB_DmaChannelStatus MGC_HsDmaGetChannelStatus(MUSB_DmaChannel *pChannel);

static uint8_t MGC_HsDmaControllerIsr(void *pPrivateData);
#endif /* MUSB_HSDMA && have at least one channel */

static MUSB_DmaController *MGC_HsNewDmaController(
    MUSB_pfDmaChannelStatusChanged pfDmaChannelStatusChanged, void *pDmaPrivate,
    MUSB_pfSystemToBusAddress pfSystemToBusAddress, uint8_t *pCoreBaseIsr, 
    uint8_t *pCoreBaseBsr);

#ifndef MUSB_CODE_SHRINK
static void MGC_HsDestroyDmaController(MUSB_DmaController *pController);
#endif
/******************************* GLOBALS *********************************/
#if defined(MUSB_DMA) && defined(MUSB_HSDMA) && (MUSB_HSDMA_CHANNELS > 0)
static MGC_HsDmaController _arMGC_HsDmaController[MUSB_MAX_CONTROLLERS];
#endif

MUSB_DmaControllerFactory MUSB_HdrcDmaControllerFactory =
{
    MGC_HsNewDmaController
#ifndef MUSB_CODE_SHRINK     
    , MGC_HsDestroyDmaController
#endif    
};

/****************************** FUNCTIONS ********************************/

#if defined(MUSB_DMA) && defined(MUSB_HSDMA) && (MUSB_HSDMA_CHANNELS > 0)

static uint8_t MGC_HsDmaStartController(void *pPrivateData)
{
    /* nothing to do */
    UNUSED(pPrivateData);
    return TRUE;
}

#ifndef MUSB_CODE_SHRINK
static uint8_t MGC_HsDmaStopController(void *pPrivateData)
{
    uint8_t bChannel;

    MGC_HsDmaChannel *pImplChannel;
    MGC_HsDmaController *pController;
    uint8_t *pBase;

    MUSB_ASSERT(pPrivateData);
    pController = (MGC_HsDmaController *)pPrivateData;
    pBase = pController->pCoreBaseBsr;
    MUSB_ASSERT(pBase);    
#ifdef UNIFIED_USB
    for (bChannel = 0; bChannel < pController->bChannelCount; bChannel++)
#else
    for (bChannel = 0; bChannel < MUSB_HSDMA_CHANNELS; bChannel++)
#endif		
    {
        MGC_Write32(pBase, MGC_HSDMA_CHANNEL_OFFSET(bChannel, MGC_O_HSDMA_CONTROL), 0);

        pImplChannel = (MGC_HsDmaChannel *)&(pController->aChannel[bChannel]);
        pImplChannel->Channel.bStatus = MUSB_DMA_STATUS_FREE;
        pImplChannel->Channel.dwActualLength = 0;
    }

    pController->bmUsedChannels = 0;
    return TRUE;
}
#endif
static MUSB_DmaChannel *MGC_HsDmaAllocateChannel(void *pPrivateData, uint8_t bLocalEnd,
                                                 uint8_t bTransmit, uint8_t bProtocol,
                                                 uint16_t wMaxPacketSize)
{
    uint8_t bBit;

    MUSB_DmaChannel *pChannel = NULL;
    MGC_HsDmaChannel *pImplChannel = NULL;
    MGC_HsDmaController *pController;

    MUSB_ASSERT(pPrivateData);
    pController = (MGC_HsDmaController *)pPrivateData;

    if (bLocalEnd && (MUSB_ENDPOINT_XFER_BULK == bProtocol))
    {
#if !defined (CONFIG_ARCH_MT85XX)
        for (bBit = 0; (bBit < pController->bChannelCount) && (bBit < MUSB_HSDMA_CHANNELS); bBit++)
#else
    #ifdef UNIFIED_USB
        for (bBit = 0; bBit < pController->bChannelCount; bBit++)
	#else
        for (bBit = 0; bBit < MUSB_HSDMA_CHANNELS; bBit++)
	#endif		
#endif
        {
            if (!(pController->bmUsedChannels&(1 << bBit)))
            {
                pController->bmUsedChannels |= (1 << bBit);

                pImplChannel = &(pController->aChannel[bBit]);
                pImplChannel->pController = pController;
                pImplChannel->wMaxPacketSize = wMaxPacketSize;
                pImplChannel->bIndex = bBit;
                pImplChannel->bEnd = bLocalEnd;
                pImplChannel->bProtocol = bProtocol;
                pImplChannel->bTransmit = bTransmit;
                pChannel = &(pImplChannel->Channel);
                pChannel->pPrivateData = pImplChannel;
                pChannel->bStatus = MUSB_DMA_STATUS_FREE;
                //  MTK Notice: Max Liao, 2006/11/16.
                //  v2.303 bug: max DMA length always set to 4G Mbytes - 1.
                pChannel->dwMaxLength = 0xFFFFFFFF;
                /* Tx => mode 1; Rx => mode 0 */
                pChannel->bDesiredMode = bTransmit;
                break;
            }
        }
    }

    return pChannel;
}

static void MGC_HsDmaReleaseChannel(MUSB_DmaChannel *pChannel)
{
    MGC_HsDmaChannel *pImplChannel;
    MGC_HsDmaController *pController;
    uint8_t *pBase;

    MUSB_ASSERT(pChannel);
    pImplChannel = (MGC_HsDmaChannel *)pChannel->pPrivateData;
    MUSB_ASSERT(pImplChannel);
    pController = pImplChannel->pController;
    MUSB_ASSERT(pController);
    pBase = pController->pCoreBaseBsr;
    MUSB_ASSERT(pBase);    

    MGC_Write32(pBase, MGC_HSDMA_CHANNEL_OFFSET(pImplChannel->bIndex, MGC_O_HSDMA_CONTROL), 0);

    pImplChannel->pController->bmUsedChannels &= ~(uint8_t)(1 << pImplChannel->bIndex);
    pImplChannel->Channel.bStatus = MUSB_DMA_STATUS_FREE;
}

#ifdef MUSB_DMA_BUF
static uint8_t *MGC_HsDmaAllocateBuffer(MUSB_DmaChannel *pChannel, uint32_t dwLength)
{
    uint8_t *pBuf;

    pBuf = MUSB_MemAlloc(dwLength);
    /* TODO: this may require platform-specific implementation */
    return pBuf;
}

static uint8_t MGC_HsDmaReleaseBuffer(MUSB_DmaChannel *pChannel, uint8_t *pBuffer)
{
    /* TODO: this may require platform-specific implementation */
    MUSB_MemFree(pBuffer);

    return TRUE;
}
#endif

#if defined (CONFIG_ARCH_MT85XX)
void MGC_DMAUnmap(MUSB_DmaChannel* pDmaChannel, uint8_t bIsTx)
{
    MGC_HsDmaChannel* pImplChannel = (MGC_HsDmaChannel*)pDmaChannel->pPrivateData;
	if(bIsTx)
	{
	    //Printf("Tx DMA unmap dwStartPhyAddress = 0x%x,ActualLength = 0x%x\n",pImplChannel->dwStartPhyAddress,pDmaChannel->dwActualLength);
        MUSB_DMA_UNMAP_TO_DEV((UINT32)(pImplChannel->dwStartPhyAddress),(UINT32)pDmaChannel->dwActualLength);
	}
	else
	{
	    //Printf("Rx DMA unmap dwStartPhyAddress = 0x%x,ActualLength = 0x%x\n",pImplChannel->dwStartPhyAddress,pDmaChannel->dwActualLength);
        MUSB_DMA_UNMAP_FROM_DEV((UINT32)(pImplChannel->dwStartPhyAddress),(UINT32)pDmaChannel->dwActualLength);
	}
}

UINT32 _u4MgcHsDmaMinDmaLen = 1;
#endif
static uint8_t MGC_HsDmaProgramChannel(MUSB_DmaChannel *pChannel, uint16_t wPacketSize,
                                       uint8_t bMode, const uint8_t *pBuffer,
                                       uint32_t dwLength)
{
    MGC_HsDmaChannel *pImplChannel;
    MGC_HsDmaController *pController;
    uint8_t *pBase;
    uint32_t dwCsr;
    uint16_t wCsr = 0;
    uint8_t bChannel;
    MGC_Port *pPort;
    MGC_EndpointResource *pEnd;
#if defined(CONFIG_ARCH_MT85XX)
	uint32_t dwPhyAddr = 0;
#endif
   
#ifdef MUSB_DATA_COMPARE
	if(!bMgcDmaEnable)
		return FALSE;
#endif
    /* reject below threshold (packet size) */
	#if !defined (CONFIG_ARCH_MT85XX)
    if (dwLength < MGC_HSDMA_MIN_DMA_LEN)
    {
        return FALSE;
    }
    #else
    if (dwLength < _u4MgcHsDmaMinDmaLen)
    {
        return FALSE;
    }
    #endif

    MUSB_ASSERT(pChannel);
    pImplChannel = (MGC_HsDmaChannel *)pChannel->pPrivateData;
    MUSB_ASSERT(pImplChannel);
    pController = pImplChannel->pController;
    MUSB_ASSERT(pController);   
    pBase = bMode ? pController->pCoreBaseBsr : pController->pCoreBaseIsr;
    MUSB_ASSERT(pBase);   
    dwCsr = (pImplChannel->bEnd << MGC_S_HSDMA_ENDPOINT) | (1 << MGC_S_HSDMA_ENABLE)
                         | (1 << MGC_S_HSDMA_IRQENABLE);
    bChannel = pImplChannel->bIndex;
    pPort = (MGC_Port *)pController->pDmaPrivate;
    MUSB_ASSERT(pPort);   
    pEnd = (MGC_EndpointResource *)MUSB_ArrayFetch(&(pPort->LocalEnds), pImplChannel->bEnd);
    MUSB_ASSERT(pEnd);   
    if (!pEnd)
    {
        LOG(0, "pEnd = NULL.\n", NULL);    
        return FALSE;        
    }

#ifdef USB_FIFO_ACCESS_TEST
    #ifdef USB_FIFO_ACCESS_TEST_TX_DMA_ONLY
    if(!pEnd->bIsTx)
    {
        return FALSE;
    }
    #else
    if(pEnd->bIsTx)
    {
        return FALSE;
    }
    #endif
#endif

    /* remember the core's mode */
    pImplChannel->bMode = bMode;

    /* we always use the DMAC's mode 1 */
    if (bMode)
    {
#if defined(CONFIG_ARCH_MT85XX)
        dwCsr |= 1 << MGC_S_HSDMA_MODE1;
#else
        dwCsr |= 1 << MGC_S_HSDMA_MODE1 | MGC_M_HSDMA_BURSTMODE;
#endif
    }

#if !defined(CONFIG_ARCH_MT85XX)
    //  MTK Notice: Max Liao, 2006/12/08.
    //  flush data cache before dma.
    if (pImplChannel->bTransmit)
	{
	    HalDmaBufferStart((void *)pBuffer, dwLength, HAL_DMA_TO_DEVICE);
	}
    else
	{
	    HalDmaBufferStart((void *)pBuffer, dwLength, HAL_DMA_FROM_DEVICE);
	}
#else
    if (pImplChannel->bTransmit)
	{
        dwPhyAddr = MUSB_DMA_MAP_TO_DEV((uint32_t)pBuffer, dwLength);
	}
    else
	{
        dwPhyAddr = MUSB_DMA_MAP_FROM_DEV((uint32_t)pBuffer, dwLength);
	}
#endif
    if (pImplChannel->bTransmit)
    {
        //  MTK Notice: Max Liao, 2006/12/10.
        //  prevent client task and USB HISR race condition, set csr in MGC_HsDmaProgramChannel().
        wCsr = MGC_ReadCsr16(pBase, MGC_O_HDRC_TXCSR, pEnd->bLocalEnd);

        if (bMode)
        {
            wCsr |= (MGC_M_TXCSR_AUTOSET | MGC_M_TXCSR_DMAENAB | MGC_M_TXCSR_DMAMODE);
        }
        else
        {
            wCsr &= (uint16_t)(~(MGC_M_TXCSR_AUTOSET | MGC_M_TXCSR_DMAENAB | MGC_M_TXCSR_DMAMODE));
        }
#ifdef UNIFIED_USB //2010.05.18
        MGC_WriteCsr16(pBase, MGC_O_HDRC_TXCSR, pEnd->bLocalEnd, wCsr);
#else
        MGC_WriteCsr16(pBase, MGC_O_HDRC_TXCSR, pEnd->bLocalEnd, wCsr | MGC_M_TXCSR_MODE);
#endif

        dwCsr |= 1 << MGC_S_HSDMA_TRANSMIT;
    }
    else
    {
        wCsr = MGC_ReadCsr16(pBase, MGC_O_HDRC_RXCSR, pEnd->bLocalEnd);
        if (bMode)
        {
            wCsr |= (MGC_M_RXCSR_AUTOCLEAR | MGC_M_RXCSR_DMAENAB);
            if (pPort->bIsHost)
            {
                //  MTK Notice: Max Liao, 2006/09/25.
                //  Request the actual number of packet to be received.
                MUSB_ASSERT(pEnd->wRxPacketSize > 0);
                MGC_DMA_Write32(pBase, M_REG_REQPKT(pEnd->bLocalEnd),
                                (((dwLength + pEnd->wRxPacketSize) - 1) /pEnd->wRxPacketSize));

                wCsr &= ~MGC_M_RXCSR_RXPKTRDY;
                // host use MGC_M_RXCSR_DMAMODE.
                wCsr |= (MGC_M_RXCSR_H_AUTOREQ | MGC_M_RXCSR_DMAMODE | MGC_M_RXCSR_H_REQPKT);
                MGC_WriteCsr16(pBase, MGC_O_HDRC_RXCSR, pEnd->bLocalEnd, wCsr);
            }
            else
            {
                wCsr |= (MGC_M_RXCSR_DMAMODE);
                MGC_WriteCsr16(pBase, MGC_O_HDRC_RXCSR, pEnd->bLocalEnd, wCsr);
            }
        }
        else
        {
            wCsr &= (uint16_t)(~(MGC_M_RXCSR_AUTOCLEAR | MGC_M_RXCSR_H_AUTOREQ | MGC_M_RXCSR_DMAENAB
                          | MGC_M_RXCSR_DMAMODE));
            MGC_WriteCsr16(pBase, MGC_O_HDRC_RXCSR, pEnd->bLocalEnd, wCsr);
        }
    }

#if defined(CONFIG_ARCH_MT85XX)
    LOG(5, "%s DMA@0x%08X , Len=%d\n", pImplChannel->bTransmit?"TX":"RX",
        (uint32_t)dwPhyAddr, dwLength);
#endif

    /* address/count */
    MGC_Write32(pBase, MGC_HSDMA_CHANNEL_OFFSET(bChannel, MGC_O_HSDMA_ADDRESS),
#if !defined(CONFIG_ARCH_MT85XX)
                (uint32_t)pController->pfSystemToBusAddress(pBuffer));
#else
                (uint32_t)dwPhyAddr);
#endif
    MGC_Write32(pBase, MGC_HSDMA_CHANNEL_OFFSET(bChannel, MGC_O_HSDMA_COUNT), dwLength);

    /* control (this should start things) */
    pChannel->dwActualLength = 0L;
    pImplChannel->dwStartAddress = (uint32_t)pBuffer;
#if defined(CONFIG_ARCH_MT85XX)
	pImplChannel->dwStartPhyAddress = (uint32_t)dwPhyAddr;
#endif
    pImplChannel->dwCount = dwLength;

	MU_MB();
	
    MGC_Write32(pBase, MGC_HSDMA_CHANNEL_OFFSET(bChannel, MGC_O_HSDMA_CONTROL), dwCsr);

    UNUSED(wPacketSize);
    
    return TRUE;
}

static MUSB_DmaChannelStatus MGC_HsDmaGetChannelStatus(MUSB_DmaChannel *pChannel)
{
    uint32_t dwCount;

    MGC_HsDmaChannel *pImplChannel;
    MGC_HsDmaController *pController;
    uint8_t *pBase;
    uint8_t bChannel;
    uint32_t dwCsr;
    uint32_t dwCsrMask;

    MUSB_ASSERT(pChannel);
    pImplChannel = (MGC_HsDmaChannel *)pChannel->pPrivateData;
    MUSB_ASSERT(pImplChannel);
    pController = pImplChannel->pController;
    MUSB_ASSERT(pController);   
    pBase = pController->pCoreBaseIsr;
    MUSB_ASSERT(pBase);   
    bChannel = pImplChannel->bIndex;
    dwCsr = MGC_Read32(pBase, MGC_HSDMA_CHANNEL_OFFSET(bChannel, MGC_O_HSDMA_CONTROL));

    if (dwCsr&(1 << MGC_S_HSDMA_BUSERROR))
    {
        return MUSB_DMA_STATUS_BUS_ABORT;
    }

    dwCount = MGC_Read32(pBase, MGC_HSDMA_CHANNEL_OFFSET(bChannel, MGC_O_HSDMA_COUNT));
    pChannel->dwActualLength = pImplChannel->dwCount - dwCount;

    //  MTK Notice: Max Liao, 2006/12/15.
    //  handle last short packet in multiple packet DMA RX mode 1.  
    dwCsrMask = (uint32_t)
        ((1 << MGC_S_HSDMA_ENABLE) | (1 << MGC_S_HSDMA_MODE1) | (1 << MGC_S_HSDMA_IRQENABLE));

    if ((dwCsr & dwCsrMask) == dwCsrMask)
    {
        MGC_Write32(pBase, MGC_HSDMA_CHANNEL_OFFSET(bChannel, MGC_O_HSDMA_CONTROL), 0);
        return MUSB_DMA_STATUS_FREE;
    }

    if (dwCount)
    {
        return MUSB_DMA_STATUS_BUSY;
    }

    return MUSB_DMA_STATUS_FREE;
}

static uint8_t MGC_HsDmaControllerIsr(void *pPrivateData)
{
    uint8_t bAllowDma = FALSE;

    MGC_BsrItem qItem;
    MUSB_Irp *pIrp = NULL;
#ifdef MUSB_ISO

    MUSB_IsochIrp *pIsochIrp;
#endif

    MGC_EndpointResource *pEnd;
    uint8_t bChannel;
    uint32_t dwCsr;
    uint32_t dwCount;
    MGC_HsDmaChannel *pImplChannel;
    MGC_HsDmaController *pController;
    uint8_t *pBuffer = NULL;
    uint32_t dwReqLength = 0;
    uint8_t bComplete = FALSE;
    uint32_t dwStatus = MUSB_STATUS_OK;
    MGC_Port *pPort;
    MUSB_SystemServices *pServices;
    uint8_t *pBase;
	#ifdef UNIFIED_USB
    uint8_t bIntr,bIntrMask;
	#else
    uint32_t dwIntr;
	#endif
    uint8_t bRet;
    
    MUSB_ASSERT(pPrivateData);
    pController = (MGC_HsDmaController *)pPrivateData;    
    pBase = pController->pCoreBaseIsr;
    MUSB_ASSERT(pBase);
	#ifdef UNIFIED_USB
    bIntr = MGC_Read8(pBase, MGC_O_HSDMA_INTR);
	bIntrMask = MGC_Read8(pBase, MGC_O_HSDMA_INTR_MASK);
	bIntr = (bIntr&bIntrMask);
	#else
    dwIntr = MGC_Read32(pBase, MGC_O_HSDMA_INTR);   
	#endif

#ifdef UNIFIED_USB
    if (!bIntr)
#else
    if (!dwIntr)
#endif		
    {
        return FALSE;
    }
    //  MTK Notice: Max Liao, 2006/10/01.
    //  write clear interrupt register value.
    #ifdef UNIFIED_USB
    MGC_Write8(pBase, MGC_O_HSDMA_INTR, bIntr);  
	#else
    MGC_Write32(pBase, MGC_O_HSDMA_INTR, dwIntr);    
	#endif
    pPort = (MGC_Port *)pController->pDmaPrivate;
    MUSB_ASSERT(pPort);
    MUSB_ASSERT(pPort->pController);    
    pServices = pPort->pController->pSystemServices;
    MUSB_ASSERT(pServices);    

#if defined(CONFIG_ARCH_MT85XX)
	#ifdef UNIFIED_USB
    for (bChannel = 0; bChannel < pController->bChannelCount; bChannel++)
	#else
    for (bChannel = 0; bChannel < MUSB_HSDMA_CHANNELS; bChannel++)
	#endif		
#else
	for (bChannel = 0; (bChannel < pController->bChannelCount) && (bChannel < MUSB_HSDMA_CHANNELS); bChannel++)
#endif		
    {
    #ifdef UNIFIED_USB
	    if (bIntr&(1 << bChannel))
	#else
        if (dwIntr&(1 << bChannel))
	#endif		
        {
            pImplChannel = (MGC_HsDmaChannel *)&(pController->aChannel[bChannel]);

            dwCsr = MGC_Read32(pBase, MGC_HSDMA_CHANNEL_OFFSET(bChannel, MGC_O_HSDMA_CONTROL));
            dwCount = MGC_Read32(pBase, MGC_HSDMA_CHANNEL_OFFSET(bChannel, MGC_O_HSDMA_COUNT));
            pImplChannel->Channel.dwActualLength = pImplChannel->dwCount - dwCount;
#if defined(CONFIG_ARCH_MT85XX)
            MGC_DMAUnmap(&(pImplChannel->Channel), pImplChannel->bTransmit);
#endif		

            if (dwCsr&(1 << MGC_S_HSDMA_BUSERROR))
            {
                pImplChannel->Channel.bStatus = MUSB_DMA_STATUS_BUS_ABORT;
                dwStatus = (uint32_t)MUSB_STATUS_DMA_ERROR;
            }
            else
            {
                pImplChannel->Channel.bStatus = MUSB_DMA_STATUS_FREE;
            }

            MGC_SelectEnd(pBase, pImplChannel->bEnd);
            if (pImplChannel->bTransmit)
            {

#ifndef MUSB_DISABLE_DB_DYNFIFO                                   
                /* Notice: double packet buffer fifo need to check fifo empty */  
                while ((MGC_ReadCsr16(pBase, MGC_O_HDRC_TXCSR, pImplChannel->bEnd)) & 
                    (MGC_M_TXCSR_FIFONOTEMPTY|MGC_M_TXCSR_TXPKTRDY))
                {
                    /*
                    LOG(0, "DMA FIFO not empty 0x%X.\n", 
                        (MGC_ReadCsr16(pBase, MGC_O_HDRC_TXCSR, pImplChannel->bEnd)));
                    */
                    ;
                }                    
#endif

                //  MTK Notice: Max Liao, 2006/12/14.
                //  send last short packet in multiple packet transfer and tx single packet.
                if ((pImplChannel->dwCount %pImplChannel->wMaxPacketSize)
                        || (pImplChannel->dwCount <= pImplChannel->wMaxPacketSize))
                {
                    MUSB_DIAG_STRING(3, "DMA: txpktrdy.");
                    MGC_WriteCsr16(pBase, MGC_O_HDRC_TXCSR, pImplChannel->bEnd,
                                   MGC_M_TXCSR_TXPKTRDY);
                }
                else
                {
                    MUSB_DIAG_STRING(3, "DMA: tx status change.");
#if !defined(CONFIG_ARCH_MT85XX)
                    HalDmaBufferDone((void *)pImplChannel->dwStartAddress,pImplChannel->dwCount, HAL_DMA_TO_DEVICE);
#endif
                    bRet = pController->pfDmaChannelStatusChanged(
                        pController->pDmaPrivate, pImplChannel->bEnd, pImplChannel->bTransmit);
                    UNUSED(bRet);                        
                }
            }
            else
            {
                /* 
                 * NOTE: assume mode 0 implies the residual handling case,
                 * and therefore use our NASTY hack, because this is all
                 * specific to the Inventra DMAC
                 */
                pEnd = (MGC_EndpointResource *)
                           MUSB_ArrayFetch(&(pPort->LocalEnds), pImplChannel->bEnd);
                if (!pEnd)
                {
                    LOG(0, "pEnd = NULL.\n", NULL);    
                    return FALSE;        
                }

                MUSB_ASSERT(pEnd->pRxIrp);

                pEnd->dwRxOffset += pImplChannel->dwCount;
                //pEnd->bDmaResidual = FALSE;

                switch (pEnd->bRxTrafficType)
                {
                case MUSB_ENDPOINT_XFER_BULK:
                case MUSB_ENDPOINT_XFER_INT:
                    pIrp = (MUSB_Irp *)pEnd->pRxIrp;
                    bAllowDma = pIrp->bAllowDma;
                    pBuffer = pIrp->pBuffer + pEnd->dwRxOffset;
                    dwReqLength = pIrp->dwLength;
                    if (pEnd->dwRxOffset > pIrp->dwLength)
                    {
                        LOG(1, "Error: pEnd->dwRxOffset=%d > pIrp->dwLength=%d.\n", 
                            pEnd->dwRxOffset, pIrp->dwLength);
                    }
                    if ((pEnd->dwRxOffset >= pIrp->dwLength)
                            || (pImplChannel->dwCount < pEnd->wRxPacketSize))
                    {
                        bComplete = TRUE;
                    }
                    break;
#ifdef MUSB_ISO

                case MUSB_ENDPOINT_XFER_ISOC:
                    pIsochIrp = (MUSB_IsochIrp *)pEnd->pRxIrp;
                    bAllowDma = pIsochIrp->bAllowDma;
                    pBuffer = pIsochIrp->pBuffer + pEnd->dwRxOffset;
#ifdef MUSB_ISO_STREAM                    
                    // under develop.
                    bComplete = FALSE;
#else
                    bComplete = (pIsochIrp->wCurrentFrame >= pIsochIrp->wFrameCount) ? TRUE : FALSE;
#endif
                    break;
#endif

                default:
                    break;
                }

                if (bComplete)
                {
                    MUSB_DIAG_STRING(3, "DMA: rx complete.");
#if !defined(CONFIG_ARCH_MT85XX)
                    HalDmaBufferDone((void *)pImplChannel->dwStartAddress,pImplChannel->dwCount, HAL_DMA_FROM_DEVICE);
#endif

//Printf("\n$$%x\n",pImplChannel->dwCount);
					//  MTK Notice: Max Liao, 2006/12/08.
                    //  release RX DMA channel usage before handle MGC_CompleteIrp().
                    if (pEnd->pRxDmaChannel)
                    {
                        MGC_HsDmaReleaseChannel(pEnd->pRxDmaChannel);
                        pEnd->pRxDmaChannel = NULL;
                    }

                    //  MTK Notice: Max Liao, 2006/12/14.
                    //  clear RXPKTRDY when DMA size is not the multiple of max packet size.
                    //  And receive single packet.
                    if ((pImplChannel->dwCount %pImplChannel->wMaxPacketSize)
                            || (pImplChannel->dwCount <= pImplChannel->wMaxPacketSize))
                    {
                        MGC_WriteCsr16(pBase, MGC_O_HDRC_RXCSR, pImplChannel->bEnd, 0);
                    }
                    else
                    {
                        //  MTK Notice: Max Liao, 2006/12/21.
                        //  Clear DMA control bits. This is tricky. 
                        //  Because in peripheral mode, RXPKTRDY will be set by H/W and 
                        //  it be clear by S/W when S/W want to clear DMA control bits. 		    
                        //  So to prevent it by writing RXPKTRDY(r/clear field) at S/W side.
                        MGC_WriteCsr16(pBase, MGC_O_HDRC_RXCSR, pImplChannel->bEnd,
                                       MGC_M_RXCSR_RXPKTRDY);
                    }

                    if (MGC_CompleteIrp(&qItem, pEnd, dwStatus, FALSE, pEnd->pRxIrp))
                    {
                        //  MTK Notice: Max Liao, 2006/12/08.
                        //  handle complete in USBBSR task when MGC_CompleteIrp() return TRUE.		    
                        pPort->bDmaCompleted = TRUE;
                        VERIFY(pServices->pfQueueBackgroundItem(pServices->pPrivateData, &qItem));
                    }
                    MUSB_ASSERT(pEnd->dwRxOffset == 0);
                }
                else
                {
#ifdef USB_WIFI_DEBUG
                    //2009.04.15
                    if(pIrp->dwLength > 512 && pIrp->dwLength < 1024)
                    {
                        LOG(3, "Break Here!");
                    }
#endif
                    dwReqLength = dwReqLength - pEnd->dwRxOffset;
                    if (pPort->bIsHost)
                    {
                        dwStatus = pPort->pfProgramStartReceive(
                            pPort, pEnd, pBuffer, dwReqLength, pEnd->pRxIrp, bAllowDma);
                        UNUSED(dwStatus);
                    }
                    else
                    {
                        if (dwReqLength > pEnd->wRxPacketSize)
                        {
                            VERIFY(MGC_HsDmaProgramChannel(
                                pEnd->pRxDmaChannel, pEnd->wRxPacketSize, 1, pBuffer, dwReqLength));
                        }
                        else
                        {
                            //  MTK Notice: Max Liao, 2006/12/19.
                            //  release dma channel when rx short packet in peripheral mode.
                            if (pEnd->pRxDmaChannel)
                            {
                                MGC_HsDmaReleaseChannel(pEnd->pRxDmaChannel);
                                pEnd->pRxDmaChannel = NULL;
                            }
                        }

						MU_MB();
                        /* ack recv, clear MGC_M_RXCSR_RXPKTRDY */
                        MGC_WriteCsr8(pBase, MGC_O_HDRC_RXCSR, pEnd->bLocalEnd, 0);
                    }
                }
            }
        }
    }

    return TRUE;
}
#endif /* MUSB_HSDMA && have at least one channel */

#if defined(CONFIG_ARCH_MT85XX)
#ifdef UNIFIED_USB
uint8_t MGC_DMAChannelNumConfig[]=
{
#if defined(CC_MT8580)
 4,4,4
#elif defined(CC_MT8563)
 4,4,4
#endif 
};
#endif
#endif

static MUSB_DmaController *MGC_HsNewDmaController(
    MUSB_pfDmaChannelStatusChanged pfDmaChannelStatusChanged, void *pDmaPrivate,
    MUSB_pfSystemToBusAddress pfSystemToBusAddress, uint8_t *pCoreBaseIsr, 
    uint8_t *pCoreBaseBsr)
{
    MUSB_DmaController *pResult = NULL;
#ifdef UNIFIED_USB
    uint8_t intrmask = 0;
    uint8_t i;
#endif

#if defined(MUSB_DMA) && defined(MUSB_HSDMA) && (MUSB_HSDMA_CHANNELS > 0)

//  MTK Notice: Max Liao, 2007/04/04.
//  change to static allocation.
/*
    MGC_HsDmaController *
        pController = (MGC_HsDmaController *)MUSB_MemAlloc(sizeof(MGC_HsDmaController));
*/
    MGC_HsDmaController *pController;

    MUSB_ASSERT(MGC_bSystemCount < MUSB_MAX_CONTROLLERS);
    if (MGC_bSystemCount >= (uint8_t)MUSB_MAX_CONTROLLERS)    
    {
        return pResult;
    }
    pController = &_arMGC_HsDmaController[MGC_bSystemCount];
    //if (pController)
    {
        MUSB_MemSet(pController, 0, sizeof(MGC_HsDmaController));

        pController->pfSystemToBusAddress = pfSystemToBusAddress;
        pController->pfDmaChannelStatusChanged = pfDmaChannelStatusChanged;
        pController->pDmaPrivate = pDmaPrivate;
        UNUSED(pDmaPrivate);
        pController->pCoreBaseIsr = pCoreBaseIsr;
        UNUSED(pCoreBaseIsr);
        pController->pCoreBaseBsr = pCoreBaseBsr;
        UNUSED(pCoreBaseBsr);        

#if !defined(CONFIG_ARCH_MT85XX)
        pController->bChannelCount = MUSB_HSDMA_CHANNELS;  
    	for(i=0; i<pController->bChannelCount; i++)
    	{
    	    intrmask += (1<<i); 
    	}	
    	MGC_Write8(pController->pCoreBaseIsr, MGC_O_HSDMA_INTR_SET, intrmask);
#else //#if !defined(CONFIG_ARCH_MT85XX)
	#ifdef UNIFIED_USB
		pController->bChannelCount = MGC_DMAChannelNumConfig[MGC_bSystemCount];
		
		for(i=0; i<pController->bChannelCount; i++)
		{
			intrmask += (1<<i); 
		}	
        MGC_Write8(pController->pCoreBaseIsr, MGC_O_HSDMA_INTR_UNMASK_SET, intrmask);
	#else
		pController->bChannelCount = MUSB_HSDMA_CHANNELS;
	#endif
#endif //#if !defined(CONFIG_ARCH_MT85XX)

        pController->Controller.pPrivateData = pController;
        pController->Controller.pfDmaStartController = MGC_HsDmaStartController;
#ifndef MUSB_CODE_SHRINK        
        pController->Controller.pfDmaStopController = MGC_HsDmaStopController;
#endif        
        pController->Controller.pfDmaAllocateChannel = MGC_HsDmaAllocateChannel;
        pController->Controller.pfDmaReleaseChannel = MGC_HsDmaReleaseChannel;
#ifdef MUSB_DMA_BUF

        pController->Controller.pfDmaAllocateBuffer = MGC_HsDmaAllocateBuffer;
        pController->Controller.pfDmaReleaseBuffer = MGC_HsDmaReleaseBuffer;
#endif

        pController->Controller.pfDmaProgramChannel = MGC_HsDmaProgramChannel;
        pController->Controller.pfDmaGetChannelStatus = MGC_HsDmaGetChannelStatus;
        pController->Controller.pfDmaControllerIsr = MGC_HsDmaControllerIsr;
        pResult = &(pController->Controller);
    }
#endif /* MUSB_HSDMA && have at least one channel */

    return pResult;
}

#ifndef MUSB_CODE_SHRINK 
static void MGC_HsDestroyDmaController(MUSB_DmaController *pController)
{
#if defined(MUSB_DMA) && defined(MUSB_HSDMA) && (MUSB_HSDMA_CHANNELS > 0)    
    MGC_HsDmaController *pHsController;

    MUSB_ASSERT(pController);
    
    pHsController = (MGC_HsDmaController *)pController->pPrivateData;
    UNUSED(pController);
    
    if (pHsController)
    {
        pHsController->Controller.pPrivateData = NULL;
        MUSB_MemFree(pHsController);
    }
#endif /* MUSB_HSDMA && have at least one channel */

}
#endif

//#ifdef MUSB_FUNCTION
#if 0

/*
* Send data in polling mode in CDC device mode.
*/
uint8_t MGC_DevPollTx(MGC_Port *pPort, MGC_EndpointResource *pEnd, 
    const uint8_t *pBuffer, uint32_t dwTotalBytes)
{
    uint8_t bIntrUsbValue;
    uint16_t wIntrTxValue;
    uint32_t dwDmaIntr;
    uint8_t *pBase;
    MGC_Controller *pController;
    MUSB_SystemServices *pServices;
    uint32_t dwIrqStatus;
    uint8_t bEnd;
    uint8_t bIndex;
    uint16_t wCsr;
    uint8_t bStatus = TRUE;
        
    MUSB_ASSERT(pPort);
    pController = pPort->pController;
    MUSB_ASSERT(pController);
    pServices = pController->pSystemServices;
    MUSB_ASSERT(pServices);        
    pBase = (uint8_t *)pController->pControllerAddressIst;
    MUSB_ASSERT(pBase);
    MUSB_ASSERT(pEnd);

    // disable USB interrupt.   
    dwIrqStatus = pServices->pfIrqLock(pServices->pPrivateData);      

    /* save index */
    bIndex = MGC_Read8(pBase, MGC_O_HDRC_INDEX);  

    bEnd = pEnd->bLocalEnd;
       
    // select ep
    MGC_SelectEnd(pBase, bEnd);  

     //  flush data cache before dma.
    //HalFlushInvalidateDCache();
    // set multiple packet tx mode.    
    wCsr = MGC_ReadCsr16(pBase, MGC_O_HDRC_TXCSR, bEnd);
    wCsr |= (MGC_M_TXCSR_AUTOSET | MGC_M_TXCSR_DMAENAB | MGC_M_TXCSR_DMAMODE);
    MGC_WriteCsr16(pBase, MGC_O_HDRC_TXCSR, bEnd, wCsr | MGC_M_TXCSR_MODE);
    
    // set channel 1 dma address.
    MGC_Write32(pBase, 
        MGC_HSDMA_CHANNEL_OFFSET(1, MGC_O_HSDMA_ADDRESS), (uint32_t)pBuffer);

    // set data size.
    MGC_Write32(pBase, 
        MGC_HSDMA_CHANNEL_OFFSET(1, MGC_O_HSDMA_COUNT), dwTotalBytes);

    // set multiple packet tx mode, trigger dma
    MGC_Write32(pBase, 
        MGC_HSDMA_CHANNEL_OFFSET(1, MGC_O_HSDMA_CONTROL), (0xF |(bEnd << 4)));

    // Check channel 2 DMA happen.
    dwDmaIntr = 0;
    while ((dwDmaIntr & 0x2) == 0)
    {
        // check disconnect.
        bIntrUsbValue = MGC_Read8(pBase, MGC_O_HDRC_INTRUSB);    
        bIntrUsbValue &= MGC_Read8(pBase, MGC_O_HDRC_INTRUSBE);
        if (bIntrUsbValue > 0)
        {
            MGC_Write8(pBase, MGC_O_HDRC_INTRUSB, bIntrUsbValue);
            bStatus = FALSE;
            goto out;            
        }

        dwDmaIntr = MGC_Read32(pBase, MGC_O_HSDMA_INTR);   
    }
    MGC_Write32(pBase, MGC_O_HSDMA_INTR, 0x2);

    // trigger to send last packet.
    MGC_WriteCsr16(pBase, MGC_O_HDRC_TXCSR, bEnd, MGC_M_TXCSR_TXPKTRDY);

    wIntrTxValue = 0;
    while (wIntrTxValue == 0)
    {
        // check disconnect.
        bIntrUsbValue = MGC_Read8(pBase, MGC_O_HDRC_INTRUSB);    
        bIntrUsbValue &= MGC_Read8(pBase, MGC_O_HDRC_INTRUSBE);
        if (bIntrUsbValue > 0)
        {
            MGC_Write8(pBase, MGC_O_HDRC_INTRUSB, bIntrUsbValue);
            bStatus = FALSE;
            goto out;            
        }
        
        wIntrTxValue = MGC_Read16(pBase, MGC_O_HDRC_INTRTX);
        wIntrTxValue &= MGC_Read16(pBase, MGC_O_HDRC_INTRTXE);
    }

    // handle tx ep interrupt.
    if ((wIntrTxValue & (1<<bEnd)) == 0)
    {
        bStatus = FALSE;
        goto out;
    }

    //  write clear interrupt register value.
    MGC_Write16(pBase, MGC_O_HDRC_INTRTX, (1<<bEnd));

out:

    /* restore index */
    MGC_Write8(pBase, MGC_O_HDRC_INDEX, bIndex);
    
    // Enable USB interrupt.         
    pServices->pfIrqUnlock(pServices->pPrivateData, dwIrqStatus);

    return bStatus;
}
#endif

#if defined(CONFIG_ARCH_MT85XX)
#ifdef MUSB_ISO_EMULATION
/*
 *  MGC_Host_SendISOPkt 
 *  TEST command: send ISO packet out
 */
void MGC_Host_SendISOPkt(uint32_t bPortNum, uint32_t bEnd, uint32_t pktsize, uint8_t *buf, uint32_t framenum, uint32_t balance, uint32_t interval)
{
    uint32_t   intr;
    uint32_t   uframe = 0;
    uint32_t  u4MaxPktSz = 0;
    uint8_t *pBase;
    MGC_Port *pPort;
    MUSB_Port *pUsbPort = NULL;
    MGC_Controller *pController;
    const MUSB_Device* pDevice = NULL;
    uint16_t wCount;
#ifdef MTK_MHDRC
    uint8_t bHubAddr = 0;
    uint8_t bHubPort = 0;
    uint8_t bIsMulti = FALSE;
    uint8_t reg;    
#endif
    uint32_t u4UsbVector = 0;
    uint32_t phyaddr = 0;

    switch(bPortNum)
    {
        case 0:
            u4UsbVector = VECTOR_USB;
        break;
        
        case 1:
            u4UsbVector = VECTOR_USB2;
        break;

#if defined(UNIFIED_USB)
        case 2:
            u4UsbVector = VECTOR_USB3;
        break;
#endif

        default:
            LOG(0, "u4UsbVector: %d is invalid.\n", u4UsbVector);
            return;    
        break;
    }

    pUsbPort = MUSB_GetPort(bPortNum);
    if(!pUsbPort)
    {
        LOG(0, "Wrong Port number[%d].\n", bPortNum);
        return;
    }
    //MUSB_ASSERT(pUsbPort);        
    pPort = (MGC_Port *)pUsbPort->pPrivateData;
    MUSB_ASSERT(pPort);
    pController = pPort->pController;
    MUSB_ASSERT(pController);
    pBase = (uint8_t *)pController->pControllerAddressIst;
    MUSB_ASSERT(pBase);

    // disable USB interrupt.   
    BIM_DisableIrq(u4UsbVector); 

    // select ep
    MGC_SelectEnd(pBase, bEnd);

    // get the last device to test iso.
    wCount = MUSB_ListLength(&(pPort->ConnectedDeviceList));

    MUSB_ASSERT(wCount > 0);
    pDevice = (MUSB_Device *)MUSB_ListFindItem(&(pPort->ConnectedDeviceList), (wCount-1));
	MUSB_ASSERT(pDevice); //klock

#ifdef MTK_MHDRC

    /* target addr & hub addr/port */
    if(pDevice->pParentUsbDevice)
    {
        bHubAddr = pDevice->pParentUsbDevice->bBusAddress;
        bHubPort = pDevice->bHubPort;
        if(2 == pDevice->pParentUsbDevice->DeviceDescriptor.bDeviceProtocol)
        {
            bIsMulti = TRUE;
        }
    }

    reg = (0x10 |bEnd);
    /* speed field in proto reg */
    switch(pDevice->ConnectionSpeed)
    {
        case MUSB_CONNECTION_SPEED_LOW:
            // iso do not support low speed.
            MUSB_ASSERT(0);
            break;
        case MUSB_CONNECTION_SPEED_FULL:
            reg |= 0x80;
            break;
        default:
            reg |= 0x40;
    }
    // set tx ep type.
    MGC_WriteCsr8(pBase, MGC_O_HDRC_TXTYPE, bEnd, reg);

    //  set special device address to test.
    MGC_Write8(pBase, MGC_BUSCTL_OFFSET(bEnd, MGC_O_MHDRC_TXFUNCADDR), 
        100);
    MGC_Write8(pBase, MGC_BUSCTL_OFFSET(bEnd, MGC_O_MHDRC_TXHUBADDR), 
        (bIsMulti ? (0x80 | bHubAddr) : bHubAddr));
    MGC_Write8(pBase, MGC_BUSCTL_OFFSET(bEnd, MGC_O_MHDRC_TXHUBPORT), 
        bHubPort);
#else
    // set tx ep type.
    MGC_WriteCsr8(pBase, MGC_O_HDRC_TXTYPE, bEnd, (0x10 |bEnd));        

    MGC_Write32(pBase, M_REG_EPXADDR(bEnd), 100);
#endif

    // set fifo size and address.
#ifdef UNIFIED_USB
    MGC_Write8(pBase, MGC_O_HDRC_TXFIFOSZ, 0x9);
#else
    MGC_Write8(pBase, MGC_O_HDRC_TXFIFOSZ, 0xF);
#endif

    MGC_Write16(pBase, MGC_O_HDRC_TXFIFOADD, 8);
    
    // set tx maximum data size.  
    if (pktsize <= 1024)
    {
        if (pDevice->ConnectionSpeed == MUSB_CONNECTION_SPEED_FULL)
        {
            MGC_WriteCsr16(pBase, MGC_O_HDRC_TXMAXP, bEnd, 1023);        
        }
        else
        {
            MGC_WriteCsr16(pBase, MGC_O_HDRC_TXMAXP, bEnd, 0x400);
        }
    }
    else if ((pktsize > 1024) && (pktsize < 2048))
    {
        // balance data in two packet: MData, Data1.
        if (balance)
        {
            u4MaxPktSz = (pktsize + 1)/2;
            MGC_WriteCsr16(pBase, MGC_O_HDRC_TXMAXP, bEnd, (0x800 |u4MaxPktSz));            
        }
        else
        {
            MGC_WriteCsr16(pBase, MGC_O_HDRC_TXMAXP, bEnd, 0xC00);            
        }
    }
    else
    {
        // balance data in two packet: MData, Data1.
        if (balance)
        {    
            u4MaxPktSz = (pktsize + 2)/3;    
            MGC_WriteCsr16(pBase, MGC_O_HDRC_TXMAXP, bEnd, (0x1000 |u4MaxPktSz));
        }
        else
        {
            MGC_WriteCsr16(pBase, MGC_O_HDRC_TXMAXP, bEnd, 0x1400);
        }        
    }     
    
    // set tx polling interval.
    MGC_WriteCsr8(pBase, MGC_O_HDRC_TXINTERVAL, bEnd, interval);

    // set ep tx mode.
    MGC_WriteCsr8(pBase, MGC_O_HDRC_TXCSR2, bEnd, 0x20);
    
    while (uframe < framenum)
    {    
        //  flush data cache before dma.
#if defined (CONFIG_ARCH_MT85XX)
#ifdef SMP_CACHE_OPERATION
        phyaddr = MUSB_DMA_MAP_TO_DEV(U_CACHE_ADDR(buf),U_CACHE_LEN(buf,pktsize));
		phyaddr = U_DMA_MAP_ADDR(phyaddr,buf);
#endif
#else
        HalFlushInvalidateDCache();
        //HalFlushInvalidateDCacheMultipleLine((UINT32)buf,pktsize);
#endif
        // set dma address.
        MGC_Write32(pBase, MGC_HSDMA_CHANNEL_OFFSET(0, MGC_O_HSDMA_ADDRESS), (uint32_t)phyaddr);

        // set data size.
        MGC_Write32(pBase, MGC_HSDMA_CHANNEL_OFFSET(0, MGC_O_HSDMA_COUNT), pktsize);
        
        // trigger dma
        //MGC_Write32(pBase, MGC_HSDMA_CHANNEL_OFFSET(0, MGC_O_HSDMA_CONTROL), (0x60B |(bEnd << 4)));
        MGC_Write32(pBase, MGC_HSDMA_CHANNEL_OFFSET(0, MGC_O_HSDMA_CONTROL), (0xB |(bEnd << 4)));
        
        // wait irq.    
        while (!BIM_IsIrqPending(u4UsbVector));

        // handle dma interrupt.
        #ifdef UNIFIED_USB
        intr = MGC_Read8(pBase, MGC_O_HSDMA_INTR);
		#else
        intr = MGC_Read32(pBase, MGC_O_HSDMA_INTR);   
		#endif

        MUSB_ASSERT(intr);

        if(intr)
        {
        #ifdef UNIFIED_USB
            MGC_Write8(pBase, MGC_O_HSDMA_INTR, intr);
		#else
            MGC_Write32(pBase, MGC_O_HSDMA_INTR, intr);
		#endif
 #ifdef SMP_CACHE_OPERATION
        phyaddr = U_DMA_UNMAP_ADDR(phyaddr,buf);
        MUSB_DMA_UNMAP_TO_DEV(phyaddr,U_CACHE_LEN(buf,pktsize));
 #endif
            // trigger packet send.
            MGC_WriteCsr8(pBase, MGC_O_HDRC_TXCSR, bEnd, MGC_M_TXCSR_TXPKTRDY);            
        }

        BIM_ClearIrq(u4UsbVector);

        // wait irq.    
        while (!BIM_IsIrqPending(u4UsbVector));

        // handle ep interrupt.
        intr = MGC_Read16(pBase, MGC_O_HDRC_INTRTX);
        
        MUSB_ASSERT(intr);
        if(intr)
        {
            MGC_Write16(pBase, MGC_O_HDRC_INTRTX, intr);            
        }

        BIM_ClearIrq(u4UsbVector);

        buf += pktsize;
        uframe ++;        
    }

    // clear ep tx mode.
    MGC_WriteCsr8(pBase, MGC_O_HDRC_TXCSR2, bEnd, 0x0);
    
    //BIM_EnableIrq(u4UsbVector);
}


/*
 *  MGC_Host_GetISOPkt 
 *  TEST command: receive ISO packet from device.
 */
void MGC_Host_GetISOPkt(uint32_t bPortNum, uint32_t bEnd, uint32_t pktsize, uint8_t *buf, uint32_t framenum, uint32_t balance, uint32_t interval)
{
    uint32_t   intr;
    uint32_t   count = 0;
    uint32_t   rxcount = 0;
    uint32_t csr;    
    uint32_t  u4MaxPktSz = 0;
    uint8_t *pBase;
    MGC_Port *pPort;
    MUSB_Port *pUsbPort = NULL;
    MGC_Controller *pController;
    const MUSB_Device* pDevice = NULL;
    uint16_t wCount;
#ifdef MTK_MHDRC
    uint8_t bHubAddr = 0;
    uint8_t bHubPort = 0;
    uint8_t bIsMulti = FALSE;
    uint8_t reg;    
#endif
    uint32_t u4UsbVector = 0;

#ifdef UNIFIED_USB
    uint8_t bIntrUsbValue;
    uint16_t wIntrTxValue, wIntrRxValue;	
#endif

	uint32_t phyaddr = 0;

    switch(bPortNum)
    {
        case 0:			
            u4UsbVector = VECTOR_USB;		
        break;
        
        case 1:
            u4UsbVector = VECTOR_USB2;
        break;

#if defined(UNIFIED_USB)
        case 2:
            u4UsbVector = VECTOR_USB3;
        break;
#endif
        
        default:
            LOG(0, "u4UsbVector: %d is invalid.\n", u4UsbVector);
            return;    
        break;
    }

    pUsbPort = MUSB_GetPort(bPortNum);
    if(!pUsbPort)
    {
        LOG(0, "Wrong Port number[%d].\n", bPortNum);
        return;
    }
    //MUSB_ASSERT(pUsbPort);        
    pPort = (MGC_Port *)pUsbPort->pPrivateData;
    MUSB_ASSERT(pPort);
    pController = pPort->pController;
    MUSB_ASSERT(pController);
    pBase = (uint8_t *)pController->pControllerAddressIst;
    MUSB_ASSERT(pBase);

    // disable USB interrupt.   
    BIM_DisableIrq(u4UsbVector); 

    // select ep
    MGC_SelectEnd(pBase, bEnd);    

    // get the last device to test iso.
    wCount = MUSB_ListLength(&(pPort->ConnectedDeviceList));
    MUSB_ASSERT(wCount > 0);
    pDevice = (MUSB_Device *)MUSB_ListFindItem(&(pPort->ConnectedDeviceList), (wCount-1));
    MUSB_ASSERT(pDevice);//klock

#ifdef MTK_MHDRC

    /* target addr & hub addr/port */
    if(pDevice->pParentUsbDevice)
    {
        bHubAddr = pDevice->pParentUsbDevice->bBusAddress;
        bHubPort = pDevice->bHubPort;
        if(2 == pDevice->pParentUsbDevice->DeviceDescriptor.bDeviceProtocol)
        {
            bIsMulti = TRUE;
        }
    }

    reg = (0x10 |bEnd);
    /* speed field in proto reg */
    switch(pDevice->ConnectionSpeed)
    {
        case MUSB_CONNECTION_SPEED_LOW:
            // iso do not support low speed.
            MUSB_ASSERT(0);
            break;
        case MUSB_CONNECTION_SPEED_FULL:
            reg |= 0x80;
            break;
        default:
            reg |= 0x40;
    }
    // set rx ep type.
    MGC_WriteCsr8(pBase, MGC_O_HDRC_RXTYPE, bEnd, reg);        

    //  set special device address to test.
    MGC_Write8(pBase, MGC_BUSCTL_OFFSET(bEnd, MGC_O_MHDRC_RXFUNCADDR), 
        100);
    MGC_Write8(pBase, MGC_BUSCTL_OFFSET(bEnd, MGC_O_MHDRC_RXHUBADDR), 
        (bIsMulti ? (0x80 | bHubAddr) : bHubAddr));
    MGC_Write8(pBase, MGC_BUSCTL_OFFSET(bEnd, MGC_O_MHDRC_RXHUBPORT), 
        bHubPort);
#else
    // set rx ep type.
    MGC_WriteCsr8(pBase, MGC_O_HDRC_RXTYPE, bEnd, (0x10 |bEnd));        

    MGC_Write32(pBase, M_REG_EPXADDR(bEnd), 100);
#endif

    // set fifo size and address.
#ifdef UNIFIED_USB
    MGC_Write8(pBase, MGC_O_HDRC_RXFIFOSZ, 0x9);
#else
    MGC_Write8(pBase, MGC_O_HDRC_RXFIFOSZ, 0xF);
#endif

    MGC_Write16(pBase, MGC_O_HDRC_RXFIFOADD, 8);
    
    // set rx maximum data size.   
    if (pktsize <= 1024)
    {
        if (pDevice->ConnectionSpeed == MUSB_CONNECTION_SPEED_FULL)
        {
            MGC_WriteCsr16(pBase, MGC_O_HDRC_RXMAXP, bEnd, 1023);
        }
        else
        {
            MGC_WriteCsr16(pBase, MGC_O_HDRC_RXMAXP, bEnd, 0x400);
        }
    }
    else if ((pktsize > 1024) && (pktsize < 2048))
    {
        // balance data in two packet: Data1, Data0.
        if (balance)
        {
            u4MaxPktSz = (pktsize + 1)/2;
            Printf("Balance 2 pkts: %d\n", u4MaxPktSz);
            MGC_WriteCsr16(pBase, MGC_O_HDRC_RXMAXP, bEnd, (0x800 |u4MaxPktSz));       	
        }
        else
        {
            Printf("2 pkts: %d\n", pktsize);
            MGC_WriteCsr16(pBase, MGC_O_HDRC_RXMAXP, bEnd, 0xC00);
        }
    }
    else
    {
        // balance data in two packet: Data2, Data1, Data0.
        if (balance)
        {    
            u4MaxPktSz = (pktsize + 2)/3;    
            Printf("Balance 3 pkts: %d\n", u4MaxPktSz);
            MGC_WriteCsr16(pBase, MGC_O_HDRC_RXMAXP, bEnd, (0x1000 |u4MaxPktSz));            
        }
        else
        {
            Printf("3 pkts: %d\n", pktsize);
            MGC_WriteCsr16(pBase, MGC_O_HDRC_RXMAXP, bEnd, 0x1400);            
        }        
    }     

    // set rx polling interval.
    MGC_WriteCsr8(pBase, MGC_O_HDRC_RXINTERVAL, bEnd, interval);       	

    while (count < framenum)
    {
        // request pkt from device.
        MGC_WriteCsr8(pBase, MGC_O_HDRC_RXCSR, bEnd, MGC_M_RXCSR_H_REQPKT);       	

        // wait irq.    
        while (!BIM_IsIrqPending(u4UsbVector));

#ifdef UNIFIED_USB
	    /* read registers */
	    bIntrUsbValue = MGC_Read8(pBase, MGC_O_HDRC_INTRUSB);
	    wIntrTxValue = MGC_Read16(pBase, MGC_O_HDRC_INTRTX);
	    wIntrRxValue = MGC_Read16(pBase, MGC_O_HDRC_INTRRX);

		LOG(6,"level1 0x%08x 0x%x 0x%x 0x%x 0x%x \n", MGC_Read32(pBase,M_REG_INTRLEVEL1),
		bIntrUsbValue,wIntrTxValue,wIntrRxValue,
		MGC_Read8(pBase, 0x200));
#endif	

        // check if IncompRX happen.
        csr = MGC_ReadCsr16(pBase, MGC_O_HDRC_RXCSR, bEnd);
        if (csr & MGC_M_RXCSR_INCOMPRX)
        {
            MUSB_ASSERT((csr & MGC_M_RXCSR_INCOMPRX) == 0);
        }
                
        // handle usb interrupt.
        intr = MGC_Read8(pBase, MGC_O_HDRC_INTRUSB);      
        if (intr)
        {
            MGC_Write8(pBase, MGC_O_HDRC_INTRUSB, intr);        
        }

        // handle ep interrupt.
        intr = MGC_Read16(pBase, MGC_O_HDRC_INTRRX);
        
        MUSB_ASSERT(intr);
        if(intr)
        {
            MGC_Write16(pBase, MGC_O_HDRC_INTRRX, intr);                
        }

        BIM_ClearIrq(u4UsbVector);

         //  flush data cache before dma.
#if defined (CONFIG_ARCH_MT85XX)
#ifdef SMP_CACHE_OPERATION
        phyaddr = MUSB_DMA_MAP_FROM_DEV(U_CACHE_ADDR(buf),U_CACHE_LEN(buf,pktsize));
        phyaddr = U_DMA_MAP_ADDR(phyaddr,buf);
#endif
#else
        //HalFlushInvalidateDCache();
        HalFlushInvalidateDCacheMultipleLine((UINT32)buf,MGC_ReadCsr16(pBase, MGC_O_HDRC_RXCOUNT, bEnd));
#endif
        // set dma address.  //2010.05.18
        //MGC_Write32(pBase, MGC_HSDMA_CHANNEL_OFFSET(0, MGC_O_HSDMA_ADDRESS), (uint32_t)buf);
        MGC_Write32(pBase, MGC_HSDMA_CHANNEL_OFFSET(0, MGC_O_HSDMA_ADDRESS), (uint32_t)phyaddr);

        rxcount = MGC_ReadCsr16(pBase, MGC_O_HDRC_RXCOUNT, bEnd);

        Printf("rxcount: %d\n", rxcount);

        // set data size.   //2010.05.18
        MGC_Write32(pBase, MGC_HSDMA_CHANNEL_OFFSET(0, MGC_O_HSDMA_COUNT), rxcount);
        
        // trigger dma    //2010.05.18
        //MGC_Write32(pBase, MGC_HSDMA_CHANNEL_OFFSET(1, MGC_O_HSDMA_CONTROL), (0x609 |(bEnd << 4)));
        MGC_Write32(pBase, MGC_HSDMA_CHANNEL_OFFSET(0, MGC_O_HSDMA_CONTROL), (0x9 |(bEnd << 4)));

        // wait irq.    
        while (!BIM_IsIrqPending(u4UsbVector));

        // handle dma interrupt.
        #ifdef UNIFIED_USB
		intr = MGC_Read8(pBase, MGC_O_HSDMA_INTR); 
		#else 
        intr = MGC_Read32(pBase, MGC_O_HSDMA_INTR);   
		#endif

        MUSB_ASSERT(intr);

        if(intr)
        {
            #ifdef UNIFIED_USB
            MGC_Write8(pBase, MGC_O_HSDMA_INTR, intr);
			#else
            MGC_Write32(pBase, MGC_O_HSDMA_INTR, intr);
			#endif
#ifdef SMP_CACHE_OPERATION
        phyaddr = U_DMA_UNMAP_ADDR(phyaddr,buf);
        MUSB_DMA_UNMAP_FROM_DEV(phyaddr,U_CACHE_LEN(buf,pktsize));
#endif
            // clear rx status.
	    csr = MGC_ReadCsr8(pBase, MGC_O_HDRC_RXCSR, bEnd);
            MGC_WriteCsr8(pBase, MGC_O_HDRC_RXCSR, bEnd, (csr & ~MGC_M_RXCSR_RXPKTRDY));       	
        }

        BIM_ClearIrq(u4UsbVector);

        buf += rxcount;
        count ++;        
    }
    
    //BIM_EnableIrq(u4UsbVector);
}

/*
 *  MGC_Dev_SendISOPkt 
 *  TEST command: send ISO packet out
 */
void MGC_Dev_SendISOPkt(uint32_t bPortNum, uint32_t bEnd, uint32_t pktsize, uint8_t *buf, uint32_t framenum, uint32_t balance)
{
    uint32_t   intr;
    uint32_t   uframe = 0;
    uint32_t  u4MaxPktSz = 0;
    uint8_t *pBase;
    MGC_Port *pPort;
    MUSB_Port *pUsbPort = NULL;
    MGC_Controller *pController;
    uint32_t u4UsbVector = 0;
	uint32_t phyaddr = 0;

    switch(bPortNum)
    {
        case 0:
            u4UsbVector = VECTOR_USB;           
        break;
        
        case 1:
            u4UsbVector = VECTOR_USB2;
        break;

#if defined(UNIFIED_USB)
        case 2:
            u4UsbVector = VECTOR_USB3;
        break;
#endif
        
        default:
            LOG(0, "u4UsbVector: %d is invalid.\n", u4UsbVector);
            return;    
        break;
    }

    pUsbPort = MUSB_GetPort(bPortNum);
    if(!pUsbPort)
    {
        LOG(0, "Wrong Port number[%d].\n", bPortNum);
        return;
    }
    //MUSB_ASSERT(pUsbPort);        
    pPort = (MGC_Port *)pUsbPort->pPrivateData;
    MUSB_ASSERT(pPort);
    pController = pPort->pController;
    MUSB_ASSERT(pController);
    pBase = (uint8_t *)pController->pControllerAddressIst;
    MUSB_ASSERT(pBase);

    // disable USB interrupt.   
    BIM_DisableIrq(u4UsbVector); 

    // select ep
    MGC_SelectEnd(pBase, bEnd);    

    //  set special device address to test.
    MGC_Write8(pBase, MGC_O_HDRC_FADDR, 100);
    
    // set fifo size and address.
#ifdef UNIFIED_USB
    MGC_Write8(pBase, MGC_O_HDRC_TXFIFOSZ, 0x9);
#else
    MGC_Write8(pBase, MGC_O_HDRC_TXFIFOSZ, 0xF);
#endif

    MGC_Write16(pBase, MGC_O_HDRC_TXFIFOADD, 8);

    // set tx maximum data size.  
    if (pktsize <= 1024)
    {
        MGC_WriteCsr16(pBase, MGC_O_HDRC_TXMAXP, bEnd, 0x400);       
    }
    else if ((pktsize > 1024) && (pktsize < 2048))
    {
        // balance data in two packet: MData, Data1.
        if (balance)
        {
            u4MaxPktSz = (pktsize + 1)/2;
            Printf("Balance 2 Pkts %d\n", u4MaxPktSz);
            MGC_WriteCsr16(pBase, MGC_O_HDRC_TXMAXP, bEnd, (0x800 |u4MaxPktSz));            
        }
        else
        {
            Printf("2 Pkts %d\n", u4MaxPktSz);
            MGC_WriteCsr16(pBase, MGC_O_HDRC_TXMAXP, bEnd, 0xC00);            
        }
    }
    else
    {
        // balance data in two packet: MData, Data1.
        if (balance)
        {    
            u4MaxPktSz = (pktsize + 2)/3;    
            Printf("Balance 3 Pkts %d\n", u4MaxPktSz);
            MGC_WriteCsr16(pBase, MGC_O_HDRC_TXMAXP, bEnd, (0x1000 |u4MaxPktSz));
        }
        else
        {
            Printf("3 Pkts %d\n", u4MaxPktSz);
            MGC_WriteCsr16(pBase, MGC_O_HDRC_TXMAXP, bEnd, 0x1400);
        }        
    }     

    // set tx ep type.
    MGC_WriteCsr8(pBase, MGC_O_HDRC_TXTYPE, bEnd, (0x10 |bEnd));

    // set ep tx mode.
    MGC_WriteCsr8(pBase, MGC_O_HDRC_TXCSR2, bEnd, 0x60);
    Printf("Start Dev Send ISO packet. pktsize = 0x%x, framenum =%d\n", pktsize, framenum);  
    while (uframe < framenum)
    {
        //  flush data cache before dma.
#if defined (CONFIG_ARCH_MT85XX)
	#ifdef SMP_CACHE_OPERATION
        phyaddr = MUSB_DMA_MAP_TO_DEV(U_CACHE_ADDR(buf),U_CACHE_LEN(buf,pktsize));
        phyaddr = U_DMA_MAP_ADDR(phyaddr,buf);
	#endif    
#else
        //HalFlushInvalidateDCache();
        HalFlushInvalidateDCacheMultipleLine((UINT32)buf,pktsize);
#endif    
    
        // set dma address.
        MGC_Write32(pBase, MGC_HSDMA_CHANNEL_OFFSET(0, MGC_O_HSDMA_ADDRESS), (uint32_t)buf);

        // set data size.
        MGC_Write32(pBase, MGC_HSDMA_CHANNEL_OFFSET(0, MGC_O_HSDMA_COUNT), pktsize);
        
        // trigger dma
        //MGC_Write32(pBase, MGC_HSDMA_CHANNEL_OFFSET(0, MGC_O_HSDMA_CONTROL), (0x60B |(bEnd << 4)));
        MGC_Write32(pBase, MGC_HSDMA_CHANNEL_OFFSET(0, MGC_O_HSDMA_CONTROL), (0xB |(bEnd << 4)));

        // wait irq.    
        while (!BIM_IsIrqPending(u4UsbVector));
        Printf("Handel DMA iterrupt pktsize = 0x%x.\n", pktsize);
        // handle dma interrupt.
        #ifdef UNIFIED_USB
        intr = MGC_Read8(pBase, MGC_O_HSDMA_INTR);
		#else
        intr = MGC_Read32(pBase, MGC_O_HSDMA_INTR);   
		#endif

        MUSB_ASSERT(intr);

        if(intr)
        {
        #ifdef UNIFIED_USB
           MGC_Write8(pBase, MGC_O_HSDMA_INTR, intr);
		#else
            MGC_Write32(pBase, MGC_O_HSDMA_INTR, intr);
		#endif
#ifdef SMP_CACHE_OPERATION
        phyaddr = U_DMA_UNMAP_ADDR(phyaddr,buf);
        MUSB_DMA_UNMAP_TO_DEV(phyaddr,U_CACHE_LEN(buf,pktsize));
#endif
            // trigger packet send.
            MGC_WriteCsr8(pBase, MGC_O_HDRC_TXCSR, bEnd, MGC_M_TXCSR_TXPKTRDY);            
        }

        BIM_ClearIrq(u4UsbVector);

        // wait irq.    
        while (!BIM_IsIrqPending(u4UsbVector));

        // handle ep interrupt.
        intr = MGC_Read16(pBase, MGC_O_HDRC_INTRTX);
        
        MUSB_ASSERT(intr);
        if(intr)
        {
            MGC_Write16(pBase, MGC_O_HDRC_INTRTX, intr);            
        }

        BIM_ClearIrq(u4UsbVector);

        buf += pktsize;
        uframe ++;        
    }

    // clear ep tx mode.
    MGC_WriteCsr8(pBase, MGC_O_HDRC_TXCSR2, bEnd, 0);
    
    //BIM_EnableIrq(u4UsbVector);
}


/*
 *  MGC_Dev_GetISOPkt 
 *  TEST command: receive ISO packet from host.
 */
uint32_t MGC_Dev_GetISOPkt(uint32_t bPortNum, uint32_t bEnd, uint8_t *buf, uint32_t framenum)
{
    uint32_t   intr;
    uint32_t   count = 0;
    uint32_t   rxcount = 0;
    uint32_t csr;    
    uint8_t *pBase;
    MGC_Port *pPort;
    MUSB_Port *pUsbPort = NULL;
    MGC_Controller *pController;
    uint32_t u4UsbVector = 0;
	uint32_t phyaddr = 0;

    switch(bPortNum)
    {
        case 0:
            u4UsbVector = VECTOR_USB;
        break;
        
        case 1:
            u4UsbVector = VECTOR_USB2;
            break;
		
#if defined(UNIFIED_USB) 
        case 2:
            u4UsbVector = VECTOR_USB3;
            break;
#endif
        
        default:
            LOG(0, "u4UsbVector: %d is invalid.\n", u4UsbVector);
            return 0;    
        break;
    }

    pUsbPort = MUSB_GetPort(bPortNum);

    if(!pUsbPort)
    {
        LOG(0, "Wrong Port number[%d].\n", pUsbPort);
        return 0;
    }
    //MUSB_ASSERT(pUsbPort);        

    pPort = (MGC_Port *)pUsbPort->pPrivateData;
    MUSB_ASSERT(pPort);
    pController = pPort->pController;
    MUSB_ASSERT(pController);
    pBase = (uint8_t *)pController->pControllerAddressIst;
    MUSB_ASSERT(pBase);

    // disable USB interrupt.   
    BIM_DisableIrq(u4UsbVector); 

    // select ep
    MGC_SelectEnd(pBase, bEnd);    

    //  set special device address to test.
    MGC_Write8(pBase, MGC_O_HDRC_FADDR, 100);

    // set fifo size and address.
#ifdef UNIFIED_USB
    MGC_Write8(pBase, MGC_O_HDRC_RXFIFOSZ, 0x9);
#else
    MGC_Write8(pBase, MGC_O_HDRC_RXFIFOSZ, 0xF);
#endif

    MGC_Write16(pBase, MGC_O_HDRC_RXFIFOADD, 8);
	
    // set rx maximum data size.   
    MGC_WriteCsr16(pBase, MGC_O_HDRC_RXMAXP, bEnd, 0x1400);       	

    // set rx ep type.
    MGC_WriteCsr16(pBase, MGC_O_HDRC_RXTYPE, bEnd, (0x10 |bEnd));
    
    // set ISO mode in device mode.
    MGC_WriteCsr8(pBase, MGC_O_HDRC_RXCSR2, bEnd, 0x40);
    
    Printf("Start Dev GetISOPacket count = %d, framenum = %d:\n", count, framenum);    

    while (count < framenum)
    {
        // wait irq.    
        while (!BIM_IsIrqPending(u4UsbVector));

        // handle usb interrupt.
        intr = MGC_Read8(pBase, MGC_O_HDRC_INTRUSB);      
        Printf(" Dev Get USB Interrupt count=%d:\n", count);
		Printf("interrrupt=0%x, 0%x, 0%x.\n", intr, MGC_Read16(pBase, MGC_O_HDRC_INTRTX), MGC_Read16(pBase, MGC_O_HDRC_INTRRX));
		#ifdef MUSB_QMU
		if((count + 1) >= framenum) 
		    break;
		#endif

        if (intr)
        {
            MGC_Write8(pBase, MGC_O_HDRC_INTRUSB, intr);        
        }

        // handle ep interrupt.
        intr = MGC_Read16(pBase, MGC_O_HDRC_INTRRX);
        
        MUSB_ASSERT(intr);
        if(intr)
        {
            MGC_Write16(pBase, MGC_O_HDRC_INTRRX, intr);                
        }

        BIM_ClearIrq(u4UsbVector);

        //  flush data cache before dma.
#if defined (CONFIG_ARCH_MT85XX)
	#ifdef SMP_CACHE_OPERATION
        phyaddr = MUSB_DMA_MAP_FROM_DEV(U_CACHE_ADDR(buf),U_CACHE_LEN(buf,rxcount));
        phyaddr = U_DMA_MAP_ADDR(phyaddr,buf);
	#endif    
#else
        //HalFlushInvalidateDCache();
        HalFlushInvalidateDCacheMultipleLine((UINT32)buf,MGC_ReadCsr16(pBase, MGC_O_HDRC_RXCOUNT, bEnd));
#endif    
        // set dma address.
        MGC_Write32(pBase, MGC_HSDMA_CHANNEL_OFFSET(0, MGC_O_HSDMA_ADDRESS), (uint32_t)buf);

        rxcount = MGC_ReadCsr16(pBase, MGC_O_HDRC_RXCOUNT, bEnd);

        // set data size.
        MGC_Write32(pBase, MGC_HSDMA_CHANNEL_OFFSET(0, MGC_O_HSDMA_COUNT), rxcount);
        
        // trigger dma
        //MGC_Write32(pBase, MGC_HSDMA_CHANNEL_OFFSET(0, MGC_O_HSDMA_CONTROL), (0x609 |(bEnd << 4)));
        MGC_Write32(pBase, MGC_HSDMA_CHANNEL_OFFSET(0, MGC_O_HSDMA_CONTROL), (0x9 |(bEnd << 4)));

        // wait irq.    
        while (!BIM_IsIrqPending(u4UsbVector));

        // handle dma interrupt.
        #ifdef UNIFIED_USB
         intr = MGC_Read8(pBase, MGC_O_HSDMA_INTR);  
		#else
        intr = MGC_Read32(pBase, MGC_O_HSDMA_INTR);   
		#endif

        MUSB_ASSERT(intr);

        if(intr)
        {
        #ifdef UNIFIED_USB
		MGC_Write8(pBase, MGC_O_HSDMA_INTR, intr);

		#else
            MGC_Write32(pBase, MGC_O_HSDMA_INTR, intr);
		#endif
#ifdef SMP_CACHE_OPERATION
        phyaddr = U_DMA_UNMAP_ADDR(phyaddr,buf);
        MUSB_DMA_UNMAP_FROM_DEV(phyaddr,U_CACHE_LEN(buf,rxcount));
#endif
            // clear rx status.
	    csr = MGC_ReadCsr8(pBase, MGC_O_HDRC_RXCSR, bEnd);
            MGC_WriteCsr8(pBase, MGC_O_HDRC_RXCSR, bEnd, (csr & ~MGC_M_RXCSR_RXPKTRDY));       	
        }

        BIM_ClearIrq(u4UsbVector);

        buf += rxcount;
        count ++;        
    }
    
    //BIM_EnableIrq(u4UsbVector);

    return rxcount;
}

#endif /* MUSB_ISO_EMULATION enabled */

#define DEV_BULK_TXEP (1)
#define DEV_BULK_RXEP (2)
#define DEV_BULK_RXEP_MSG (3)

extern unsigned int random32(void);

void MGC_Host_Bulk_Test(uint32_t bPortNum, uint8_t *buf,uint8_t *tx_buf)
{
		uint32_t   intr = 0;
	   // uint32_t	 uframe = 0;
		//uint32_t	u4MaxPktSz = 0;
		uint8_t *pBase;
		MGC_Port *pPort;
		MUSB_Port *pUsbPort = NULL;
		MGC_Controller *pController;
		const MUSB_Device* pDevice = NULL;
		uint16_t wCount;
#ifdef MTK_MHDRC
		uint8_t bHubAddr = 0;
		uint8_t bHubPort = 0;
		uint8_t bIsMulti = FALSE;
		uint8_t reg;	
#endif
	
		
		
#ifdef UNIFIED_USB
		uint16_t wCsr = 0;
				
#else
		uint16_t wCsr = MGC_M_TXCSR_MODE;
#endif
	
		uint32_t u4UsbVector = 0;
		uint32_t i;
		uint8_t bEnd = 1;
	
		switch(bPortNum)
		{
			case 0:
				u4UsbVector = VECTOR_USB;	
			break;
			
            case 1:
                u4UsbVector = VECTOR_USB2;
            break;
					
#if defined(UNIFIED_USB)
            case 2:
                u4UsbVector = VECTOR_USB3;
            break;
#endif

	
			default:
				LOG(0, "u4UsbVector: %d is invalid.\n", u4UsbVector);
				return;    
			break;
		}
	
		pUsbPort = MUSB_GetPort(bPortNum);
		if(!pUsbPort)
		{
			LOG(0, "Wrong Port number[%d].\n", bPortNum);
			return;
		}
		
		//MUSB_ASSERT(pUsbPort);		
		pPort = (MGC_Port *)pUsbPort->pPrivateData;
		MUSB_ASSERT(pPort);
		pController = pPort->pController;
		MUSB_ASSERT(pController);
		pBase = (uint8_t *)pController->pControllerAddressIst;
		MUSB_ASSERT(pBase);
	
		// disable USB interrupt.	
		BIM_DisableIrq(u4UsbVector); 
	
		// select ep
		MGC_SelectEnd(pBase, bEnd);
	
		// get the last device to test iso.
		wCount = MUSB_ListLength(&(pPort->ConnectedDeviceList));
	
		MUSB_ASSERT(wCount > 0);
		pDevice = (MUSB_Device *)MUSB_ListFindItem(&(pPort->ConnectedDeviceList), (wCount-1));
	
#ifdef MTK_MHDRC
	
		/* target addr & hub addr/port */
		if(pDevice->pParentUsbDevice)
		{
			bHubAddr = pDevice->pParentUsbDevice->bBusAddress;
			bHubPort = pDevice->bHubPort;
			if(2 == pDevice->pParentUsbDevice->DeviceDescriptor.bDeviceProtocol)
			{
				bIsMulti = TRUE;
			}
		}
	//device endpoint 2 tx
		reg = (0x20 |DEV_BULK_RXEP);
		/* speed field in proto reg */
		switch(pDevice->ConnectionSpeed)
		{
			case MUSB_CONNECTION_SPEED_LOW:
				// iso do not support low speed.
				MUSB_ASSERT(0);
				break;
			case MUSB_CONNECTION_SPEED_FULL:
				reg |= 0x80;
				break;
			default:
				reg |= 0x40;
		}
	
#else
		// set tx ep type.
	  //  MGC_WriteCsr8(pBase, MGC_O_HDRC_TXTYPE, bEnd, (0x10 |bEnd));		  
	
	  //  MGC_Write32(pBase, M_REG_EPXADDR(bEnd), 100);
#endif
{
			for(i=0; i<6; i++)
			{
			  *(tx_buf+512*i) = (uint8_t)random32() ;
			}
			
			MGC_WriteCsr8(pBase, MGC_O_HDRC_RXTYPE, bEnd, 0x20|DEV_BULK_TXEP|0x40);
			
			MGC_Write8(pBase, MGC_BUSCTL_OFFSET(bEnd, MGC_O_MHDRC_RXFUNCADDR), 
				1);
			MGC_Write8(pBase, MGC_BUSCTL_OFFSET(bEnd, MGC_O_MHDRC_RXHUBADDR), 
				(bIsMulti ? (0x80 | bHubAddr) : bHubAddr));
			MGC_Write8(pBase, MGC_BUSCTL_OFFSET(bEnd, MGC_O_MHDRC_RXHUBPORT), 
				bHubPort);
			
			MGC_WriteCsr16(pBase, MGC_O_HDRC_RXMAXP, bEnd, 0x200); 
			MGC_WriteCsr8(pBase, MGC_O_HDRC_RXINTERVAL, bEnd, 0);
			
			//MGC_WriteCsr16(pBase, MGC_O_HDRC_RXCSR, bEnd,
			//			   MGC_M_RXCSR_FLUSHFIFO);
			//MGC_WriteCsr16(pBase, MGC_O_HDRC_RXCSR, bEnd,
			//			   MGC_M_RXCSR_FLUSHFIFO);
	
	
			MGC_DMA_Write32(pBase, M_REG_REQPKT(bEnd),
				 6);
		   
					// set dma address.
			MGC_Write32(pBase, MGC_HSDMA_CHANNEL_OFFSET(1, MGC_O_HSDMA_ADDRESS), (uint32_t)buf);
	
			// set data size.
			MGC_Write32(pBase, MGC_HSDMA_CHANNEL_OFFSET(1, MGC_O_HSDMA_COUNT), 6*512);
			MGC_Write32(pBase, MGC_HSDMA_CHANNEL_OFFSET(1, MGC_O_HSDMA_CONTROL), (0xd |(bEnd << 4)));
			MGC_WriteCsr16(pBase, MGC_O_HDRC_RXCSR, bEnd, 
					   (MGC_M_RXCSR_H_AUTOREQ | MGC_M_RXCSR_DMAMODE | MGC_M_RXCSR_H_REQPKT|MGC_M_RXCSR_AUTOCLEAR | MGC_M_RXCSR_DMAENAB));
	
		//	Printf("wait \n");
				
	
	
			//x_thread_delay((random32()%5));
	
		//	Printf("wait done\n");
	
			
			// set tx ep type.
			MGC_WriteCsr8(pBase, MGC_O_HDRC_TXTYPE, bEnd, reg);
		
			//	set special device address to test.
			MGC_Write8(pBase, MGC_BUSCTL_OFFSET(bEnd, MGC_O_MHDRC_TXFUNCADDR), 
				1);
			MGC_Write8(pBase, MGC_BUSCTL_OFFSET(bEnd, MGC_O_MHDRC_TXHUBADDR), 
				(bIsMulti ? (0x80 | bHubAddr) : bHubAddr));
			MGC_Write8(pBase, MGC_BUSCTL_OFFSET(bEnd, MGC_O_MHDRC_TXHUBPORT), 
				bHubPort);
	
			MGC_WriteCsr16(pBase, MGC_O_HDRC_TXCSR, bEnd,
					MGC_M_TXCSR_FLUSHFIFO);
			MGC_WriteCsr16(pBase, MGC_O_HDRC_TXCSR, bEnd,
					MGC_M_TXCSR_FLUSHFIFO);
			
			MGC_WriteCsr16(pBase, MGC_O_HDRC_TXMAXP, bEnd, 0x200); 
			
			
			// set tx polling interval.
			MGC_WriteCsr8(pBase, MGC_O_HDRC_TXINTERVAL, bEnd, 0);
	
	
			MGC_WriteCsr16(pBase, MGC_O_HDRC_TXCSR, bEnd, wCsr);
			
			wCsr =
				(MGC_M_TXCSR_AUTOSET | MGC_M_TXCSR_DMAENAB |
				 MGC_M_TXCSR_DMAMODE);
	
#ifdef UNIFIED_USB
		   MGC_WriteCsr16(pBase, MGC_O_HDRC_TXCSR, bEnd, 
		   wCsr);
	
#else
			MGC_WriteCsr16(pBase, MGC_O_HDRC_TXCSR, bEnd, 
			(wCsr| MGC_M_TXCSR_MODE));
#endif
		
			// set dma address.
			MGC_Write32(pBase, MGC_HSDMA_CHANNEL_OFFSET(0, MGC_O_HSDMA_ADDRESS), (uint32_t)tx_buf);
	
			// set data size.
			MGC_Write32(pBase, MGC_HSDMA_CHANNEL_OFFSET(0, MGC_O_HSDMA_COUNT), 512*6);
			
			// trigger dma
			//MGC_Write32(pBase, MGC_HSDMA_CHANNEL_OFFSET(0, MGC_O_HSDMA_CONTROL), (0x60B |(bEnd << 4)));
			MGC_Write32(pBase, MGC_HSDMA_CHANNEL_OFFSET(0, MGC_O_HSDMA_CONTROL), (0xf |(bEnd << 4)));
		
			// wait irq.	
			while (!BIM_IsIrqPending(u4UsbVector));
	
			// handle dma interrupt.
        #ifdef UNIFIED_USB
			intr = MGC_Read8(pBase, MGC_O_HSDMA_INTR);
		#else
			intr = MGC_Read32(pBase, MGC_O_HSDMA_INTR);   
		#endif
		//	Printf("finish %x\n",intr);
	
			MUSB_ASSERT(intr);

			if(intr)
			{
        #ifdef UNIFIED_USB
				MGC_Write8(pBase, MGC_O_HSDMA_INTR, intr);
		#else
				MGC_Write32(pBase, MGC_O_HSDMA_INTR, intr);
		#endif
	
				// trigger packet send.
		  //	  MGC_WriteCsr8(pBase, MGC_O_HDRC_TXCSR, bEnd, MGC_M_TXCSR_TXPKTRDY);			 
			}

			if(intr&0x02)
			{
			
			  MGC_Write32(pBase, 
              MGC_HSDMA_CHANNEL_OFFSET(1, MGC_O_HSDMA_CONTROL), 0);
 			  wCsr = MGC_ReadCsr16(pBase, MGC_O_HDRC_RXCSR, bEnd);

              if (wCsr &
                  (MGC_M_RXCSR_AUTOCLEAR | MGC_M_RXCSR_H_AUTOREQ |
                   MGC_M_RXCSR_DMAENAB | MGC_M_RXCSR_DMAMODE))
              {
                  wCsr &=
                      ~(MGC_M_RXCSR_AUTOCLEAR | MGC_M_RXCSR_H_AUTOREQ |
                        MGC_M_RXCSR_DMAENAB | MGC_M_RXCSR_DMAMODE);
      
                  MGC_WriteCsr16(pBase, MGC_O_HDRC_RXCSR, bEnd, wCsr);
              }

			  if(MGC_Read32(pBase, 
              MGC_HSDMA_CHANNEL_OFFSET(1, MGC_O_HSDMA_COUNT)))
              {MUSB_ASSERT(0);}
			  else
			  {
			    Printf("transfer 512*6 packets\n");
			  }
		

			}

			if(intr&0x01)
			{
			
			  MGC_Write32(pBase, 
              MGC_HSDMA_CHANNEL_OFFSET(0, MGC_O_HSDMA_CONTROL), 0);
			  wCsr = MGC_ReadCsr16(pBase, MGC_O_HDRC_TXCSR, bEnd);
			  
			  if (wCsr &
				  (MGC_M_TXCSR_AUTOSET | MGC_M_TXCSR_DMAENAB |
				   MGC_M_TXCSR_DMAMODE))
			  {
				  wCsr &=
					  ~(MGC_M_TXCSR_AUTOSET | MGC_M_TXCSR_DMAENAB |
						MGC_M_TXCSR_DMAMODE);
			  
				  MGC_WriteCsr16(pBase, MGC_O_HDRC_TXCSR, bEnd, wCsr);
			  }

			}			
				
	
			BIM_ClearIrq(u4UsbVector);
	
			if(intr != 3)
			{
				// wait irq.	
					   while (!BIM_IsIrqPending(u4UsbVector));
				
					   // handle dma interrupt.
	   #ifdef UNIFIED_USB
					   intr = MGC_Read8(pBase, MGC_O_HSDMA_INTR);
	   #else
					   intr = MGC_Read32(pBase, MGC_O_HSDMA_INTR);	 
	   #endif
	//				   Printf("finish %x\n",intr);


				
					   MUSB_ASSERT(intr);
				
					   if(intr)
					   {
	   #ifdef UNIFIED_USB
						   MGC_Write8(pBase, MGC_O_HSDMA_INTR, intr);
	   #else
						   MGC_Write32(pBase, MGC_O_HSDMA_INTR, intr);
	   #endif
				
						   // trigger packet send.
					 // 	 MGC_WriteCsr8(pBase, MGC_O_HDRC_TXCSR, bEnd, MGC_M_TXCSR_TXPKTRDY);			
					   }
      
      				if(intr&0x02)
      				{
      				
      				  MGC_Write32(pBase, 
      				  MGC_HSDMA_CHANNEL_OFFSET(1, MGC_O_HSDMA_CONTROL), 0);
      				  wCsr = MGC_ReadCsr16(pBase, MGC_O_HDRC_RXCSR, bEnd);
      				
      				  if (wCsr &
      					  (MGC_M_RXCSR_AUTOCLEAR | MGC_M_RXCSR_H_AUTOREQ |
      					   MGC_M_RXCSR_DMAENAB | MGC_M_RXCSR_DMAMODE))
      				  {
      					  wCsr &=
      						  ~(MGC_M_RXCSR_AUTOCLEAR | MGC_M_RXCSR_H_AUTOREQ |
      							MGC_M_RXCSR_DMAENAB | MGC_M_RXCSR_DMAMODE);
      				
      					  MGC_WriteCsr16(pBase, MGC_O_HDRC_RXCSR, bEnd, wCsr);
      				  }
      				
      							  if(MGC_Read32(pBase, 
                                MGC_HSDMA_CHANNEL_OFFSET(1, MGC_O_HSDMA_COUNT)))
                                {MUSB_ASSERT(0);}
                  			  else
                  			  {
                  			   // Printf("transfer %x packets\n",*((uint32_t*)(buf + 1024*128 -4)));
                  			  }
      				}
      				
      				if(intr&0x01)
      				{
      				
      				  MGC_Write32(pBase, 
      				  MGC_HSDMA_CHANNEL_OFFSET(0, MGC_O_HSDMA_CONTROL), 0);
      				  wCsr = MGC_ReadCsr16(pBase, MGC_O_HDRC_TXCSR, bEnd);
      				  
      				  if (wCsr &
      					  (MGC_M_TXCSR_AUTOSET | MGC_M_TXCSR_DMAENAB |
      					   MGC_M_TXCSR_DMAMODE))
      				  {
      					  wCsr &=
      						  ~(MGC_M_TXCSR_AUTOSET | MGC_M_TXCSR_DMAENAB |
      							MGC_M_TXCSR_DMAMODE);
      				  
      					  MGC_WriteCsr16(pBase, MGC_O_HDRC_TXCSR, bEnd, wCsr);
      				  }
      				
      				}								   
				
					   BIM_ClearIrq(u4UsbVector);
			  
			}
	
          for(i=0; i<6*512; i++)
          {
            if(*(buf+i) != *(tx_buf+i))
			Printf("data error \n");	
          }
		}
	
		BIM_EnableIrq(u4UsbVector);
	}

typedef enum{
	STS_INIT,
	STS_DATAIN,
	STS_DATAOUT,
}BUF_STS;	

typedef struct{
uint8_t * buf;
BUF_STS	buf_sts;
uint32_t length;
uint32_t rx_actual;
uint32_t tx_actual;
}BUF_DESC;	

uint32_t tx_index,rx_index,tx_pkt_going;

#define GPD_BASIC_NUM 20
BUF_DESC buf_desc[GPD_BASIC_NUM];
uint32_t first_count = 8192 ;


void MGC_Dev_Bulk_Test(uint32_t bPortNum, uint8_t *buf,uint8_t *rx_buf)
{
		uint32_t   intr_tx, intr_rx;
		uint8_t *pBase;
		MGC_Port *pPort;
		MUSB_Port *pUsbPort = NULL;
		MGC_Controller *pController;
		uint32_t u4UsbVector = 0;
		uint32_t bEnd;
		uint16_t wVal;
		uint32_t rxcount,txcount;
		uint8_t *buf_cur;
		uint8_t bstart =0;
		uint32_t* msg_buf = NULL;

		//uint32_t k = 0;
		uint32_t j = 0;
		//uint32_t error =0;
	
		switch(bPortNum)
		{
			case 0:
				u4UsbVector = VECTOR_USB;	
			break;
			
            case 1:
                u4UsbVector = VECTOR_USB2;
            break;
					
#if defined(UNIFIED_USB)
            case 2:
                u4UsbVector = VECTOR_USB3;
            break;
#endif
			
			default:
				LOG(0, "u4UsbVector: %d is invalid.\n", u4UsbVector);
				return;    
			break;
		}
	
		pUsbPort = MUSB_GetPort(bPortNum);
		if(!pUsbPort)
		{
			LOG(0, "Wrong Port number[%d].\n", bPortNum);
			return;
		}
		//MUSB_ASSERT(pUsbPort);		
		pPort = (MGC_Port *)pUsbPort->pPrivateData;
		MUSB_ASSERT(pPort);
		pController = pPort->pController;
		MUSB_ASSERT(pController);
		pBase = (uint8_t *)pController->pControllerAddressIst;
		MUSB_ASSERT(pBase);
	
		// disable USB interrupt.	
		BIM_DisableIrq(u4UsbVector); 
	
  
	
		//	set special device address to test.
		MGC_Write8(pBase, MGC_O_HDRC_FADDR, 1);
		// select ep
		bEnd = DEV_BULK_TXEP;
		MGC_SelectEnd(pBase, bEnd);  		

		MGC_WriteCsr16(pBase, MGC_O_HDRC_TXMAXP, bEnd, 0x200);		
		MGC_WriteCsr8(pBase, MGC_O_HDRC_TXTYPE, bEnd, (0x20 |bEnd));

				// select ep
		bEnd = DEV_BULK_RXEP;
		MGC_SelectEnd(pBase, bEnd); 		
		
		MGC_WriteCsr16(pBase, MGC_O_HDRC_RXMAXP, bEnd, 0x200);	
		MGC_WriteCsr8(pBase, MGC_O_HDRC_RXTYPE, bEnd, (0x20 |bEnd));

		bEnd = DEV_BULK_RXEP_MSG;
		MGC_SelectEnd(pBase, bEnd); 		
		
		MGC_WriteCsr16(pBase, MGC_O_HDRC_RXMAXP, bEnd, 0x200);	
		MGC_WriteCsr8(pBase, MGC_O_HDRC_RXTYPE, bEnd, (0x20 |bEnd));		

	
		bEnd = DEV_BULK_TXEP;
		MGC_SelectEnd(pBase, bEnd);
		
		wVal = MGC_ReadCsr16(pBase, MGC_O_HDRC_TXCSR, bEnd);
		if (wVal & MGC_M_TXCSR_P_UNDERRUN)
		{
			wVal &= ~MGC_M_TXCSR_P_UNDERRUN;
			MGC_WriteCsr16(pBase, MGC_O_HDRC_TXCSR, bEnd, wVal);
		}
		
		for(j=0; j<GPD_BASIC_NUM; j++)
		{
		 // buf_desc[j].length = first_count+j;
		 // buf_desc[j].rx_actual = 0;
		 // buf_desc[j].tx_actual = 0;
		//  buf_desc[j].buf_sts = STS_INIT;
		  buf_desc[j].buf = MUSB_MemAlloc(first_count);
		//  tx_index = 0; 
		 // rx_index = 0; 
		}

		tx_pkt_going = FALSE;

		msg_buf = (uint32_t*)MUSB_MemAlloc(512);


		while(1)
		{
			// wait irq.	
			while (!BIM_IsIrqPending(u4UsbVector));
			
			intr_tx = MGC_Read16(pBase, MGC_O_HDRC_INTRTX);
			intr_rx = MGC_Read16(pBase, MGC_O_HDRC_INTRRX);

			MGC_Write16(pBase, MGC_O_HDRC_INTRTX, intr_tx);
			MGC_Write16(pBase, MGC_O_HDRC_INTRRX, intr_rx);
            BIM_ClearIrq(u4UsbVector);

			if(intr_rx&0x08)
			{ 
				bEnd = DEV_BULK_RXEP_MSG;
		        MGC_SelectEnd(pBase, bEnd);
				rxcount = MGC_ReadCsr16(pBase, MGC_O_HDRC_RXCOUNT, bEnd);

				if(rxcount != GPD_BASIC_NUM*4)
				{
				  Printf("error msg length\n");
				}
				pPort->pfUnloadFifo(pPort,bEnd,rxcount,(uint8_t*)msg_buf);
				MGC_WriteCsr16(pBase, MGC_O_HDRC_RXCSR, bEnd, 0);


				
			  for(j=0; j<GPD_BASIC_NUM; j++)
			  {
				buf_desc[j].length = *(msg_buf+j);
				buf_desc[j].rx_actual = 0;
				buf_desc[j].tx_actual = 0;
				buf_desc[j].buf_sts = STS_INIT;
			  }
			  
				tx_index = 0; 
				rx_index = 0; 
			}
			
            if(intr_rx&0x04)
			{
				//Printf("rx intr\n");	
				
                  if(rx_index == GPD_BASIC_NUM)
                  {
                    MUSB_ASSERT(0);
                  }
				bEnd = DEV_BULK_RXEP;
		        MGC_SelectEnd(pBase, bEnd);
				rxcount = MGC_ReadCsr16(pBase, MGC_O_HDRC_RXCOUNT, bEnd);
				if(buf_desc[rx_index].rx_actual < buf_desc[rx_index].length)
				{
				  if(buf_desc[rx_index].rx_actual + rxcount > buf_desc[rx_index].length)
				  {
				    MUSB_ASSERT(0);
				  }
				  buf_cur = buf_desc[rx_index].buf + buf_desc[rx_index].rx_actual;
				  
				  pPort->pfUnloadFifo(pPort,bEnd,rxcount,buf_cur);
			      MGC_WriteCsr16(pBase, MGC_O_HDRC_RXCSR, bEnd, 0);
				  
				  buf_desc[rx_index].rx_actual += rxcount;
				  if(buf_desc[rx_index].rx_actual == buf_desc[rx_index].length)
				  {
				    buf_desc[rx_index].buf_sts = STS_DATAIN;
					rx_index++;
				  }				  
				  
				}
				else
				{
				   MUSB_ASSERT(0);
				}
				
			}
			
			if(intr_tx)
			{ 
			    if(bstart)
                {
             //     bstart = 0;
                }

				tx_pkt_going = FALSE;
	
			}

			//need tx 

			if(tx_index<rx_index && tx_pkt_going == FALSE)
			{
				do{
				bEnd = DEV_BULK_TXEP;
		        MGC_SelectEnd(pBase, bEnd);
				wVal = MGC_ReadCsr16(pBase, MGC_O_HDRC_TXCSR, bEnd);
				if (wVal & MGC_M_TXCSR_P_UNDERRUN)
				{
				    //Printf("req pkt\n");
					wVal &= ~MGC_M_TXCSR_P_UNDERRUN;
					MGC_WriteCsr16(pBase, MGC_O_HDRC_TXCSR, bEnd, wVal);
				}
				if(buf_desc[tx_index].buf_sts == STS_INIT)
				break;	
				
				if((buf_desc[tx_index].rx_actual - buf_desc[tx_index].tx_actual) >=512)
				{
				  txcount = 512;
				}
				else
				{
				 txcount = buf_desc[tx_index].rx_actual - buf_desc[tx_index].tx_actual;
				}

				buf_cur = buf_desc[tx_index].buf + buf_desc[tx_index].tx_actual;
				//Printf("tx %d \n",txcount);
				if(txcount != 512 && txcount != (buf_desc[tx_index].length % 512))
				{
				  MUSB_ASSERT(0);
				}

				
			    tx_pkt_going = TRUE;
			    pPort->pfLoadFifo(pPort, bEnd, txcount,buf_cur);
				
				buf_desc[tx_index].tx_actual +=  txcount;

		        MGC_WriteCsr16(pBase, MGC_O_HDRC_TXCSR, bEnd, MGC_M_TXCSR_TXPKTRDY);

				if(buf_desc[tx_index].tx_actual >buf_desc[tx_index].rx_actual)
				{
				  MUSB_ASSERT(0);
				}
				else if(buf_desc[tx_index].tx_actual == buf_desc[tx_index].rx_actual)
				{
				  	tx_index++;
				}
				
				}while(0);			  
			  
			}
					

			if(tx_index == GPD_BASIC_NUM)
			{
			    for(j=0; j<GPD_BASIC_NUM; j++)
        		{
        		
			    if(buf_desc[j].rx_actual != buf_desc[j].tx_actual
					|| buf_desc[j].rx_actual != buf_desc[j].length)
			    {
			      Printf("length error !\n");
			    }
        		 // buf_desc[j].length = first_count+j;
        		 // buf_desc[j].rx_actual = 0;
        		 // buf_desc[j].tx_actual = 0;
        		 // buf_desc[j].buf_sts = STS_INIT;
        		 // tx_index = 0; 
        		 // rx_index = 0; 
        		}		  
			}
			
			
		}		
		
		BIM_EnableIrq(u4UsbVector);

}
#else //#if defined(CONFIG_ARCH_MT85XX)
#ifdef MUSB_ISO_EMULATION
/*
 *  MGC_Host_SendISOPkt 
 *  TEST command: send ISO packet out
 */
void MGC_Host_SendISOPkt(uint32_t bEnd, uint32_t pktsize, uint8_t *buf, uint32_t framenum, uint32_t balance, uint32_t interval)
{
    uint32_t   intr;
    uint32_t   uframe = 0;
    uint32_t  u4MaxPktSz = 0;
    uint8_t *pBase;
    MGC_Port *pPort;
    MUSB_Port *pUsbPort = NULL;
    MGC_Controller *pController;
    const MUSB_Device* pDevice = NULL;
    uint16_t wCount;
#ifdef MTK_MHDRC
    uint8_t bHubAddr = 0;
    uint8_t bHubPort = 0;
    uint8_t bIsMulti = FALSE;
    uint8_t reg;    
#endif

    pUsbPort = MUSB_GetPort(0);
    if(!pUsbPort)
    {
        LOG(0, "Wrong Port number[%d].\n", bPortNum);
        return;
    }
    MUSB_ASSERT(pUsbPort);        
    pPort = (MGC_Port *)pUsbPort->pPrivateData;
    MUSB_ASSERT(pPort);
    pController = pPort->pController;
    MUSB_ASSERT(pController);
    pBase = (uint8_t *)pController->pControllerAddressIst;
    MUSB_ASSERT(pBase);

    // disable USB interrupt.   
    BIM_DisableIrq(VECTOR_USB); 

    // select ep
    MGC_SelectEnd(pBase, bEnd);

    // get the last device to test iso.
    wCount = MUSB_ListLength(&(pPort->ConnectedDeviceList));
    MUSB_ASSERT(wCount > 0);
    pDevice = (MUSB_Device *)MUSB_ListFindItem(&(pPort->ConnectedDeviceList), (wCount-1));
	MUSB_ASSERT(pDevice); //klock

#ifdef MTK_MHDRC

    /* target addr & hub addr/port */
    if(pDevice->pParentUsbDevice)
    {
        bHubAddr = pDevice->pParentUsbDevice->bBusAddress;
        bHubPort = pDevice->bHubPort;
        if(2 == pDevice->pParentUsbDevice->DeviceDescriptor.bDeviceProtocol)
        {
            bIsMulti = TRUE;
        }
    }

    reg = (0x10 |bEnd);
    /* speed field in proto reg */
    switch(pDevice->ConnectionSpeed)
    {
        case MUSB_CONNECTION_SPEED_LOW:
            // iso do not support low speed.
            MUSB_ASSERT(0);
            break;
        case MUSB_CONNECTION_SPEED_FULL:
            reg |= 0x80;
            break;
        default:
            reg |= 0x40;
    }
    // set tx ep type.
    MGC_WriteCsr8(pBase, MGC_O_HDRC_TXTYPE, bEnd, reg);

    //  set special device address to test.
    MGC_Write8(pBase, MGC_BUSCTL_OFFSET(bEnd, MGC_O_MHDRC_TXFUNCADDR), 
        100);
    MGC_Write8(pBase, MGC_BUSCTL_OFFSET(bEnd, MGC_O_MHDRC_TXHUBADDR), 
        (bIsMulti ? (0x80 | bHubAddr) : bHubAddr));
    MGC_Write8(pBase, MGC_BUSCTL_OFFSET(bEnd, MGC_O_MHDRC_TXHUBPORT), 
        bHubPort);
#else
    // set tx ep type.
    MGC_WriteCsr8(pBase, MGC_O_HDRC_TXTYPE, bEnd, (0x10 |bEnd));        

    MGC_Write32(pBase, M_REG_EPXADDR(bEnd), 100);
#endif

    // set fifo size and address.
#ifdef UNIFIED_USB
    MGC_Write8(pBase, MGC_O_HDRC_TXFIFOSZ, 0x9);
#else
    MGC_Write8(pBase, MGC_O_HDRC_TXFIFOSZ, 0xF);
#endif

    MGC_Write16(pBase, MGC_O_HDRC_TXFIFOADD, 8);
    
    // set tx maximum data size.  
    if (pktsize <= 1024)
    {
        if (pDevice->ConnectionSpeed == MUSB_CONNECTION_SPEED_FULL)
        {
            MGC_WriteCsr16(pBase, MGC_O_HDRC_TXMAXP, bEnd, 1023);        
        }
        else
        {
            MGC_WriteCsr16(pBase, MGC_O_HDRC_TXMAXP, bEnd, 0x400);
        }
    }
    else if ((pktsize > 1024) && (pktsize < 2048))
    {
        // balance data in two packet: MData, Data1.
        if (balance)
        {
            u4MaxPktSz = (pktsize + 1)/2;
            MGC_WriteCsr16(pBase, MGC_O_HDRC_TXMAXP, bEnd, (0x800 |u4MaxPktSz));            
        }
        else
        {
            MGC_WriteCsr16(pBase, MGC_O_HDRC_TXMAXP, bEnd, 0xC00);            
        }
    }
    else
    {
        // balance data in two packet: MData, Data1.
        if (balance)
        {    
            u4MaxPktSz = (pktsize + 2)/3;    
            MGC_WriteCsr16(pBase, MGC_O_HDRC_TXMAXP, bEnd, (0x1000 |u4MaxPktSz));
        }
        else
        {
            MGC_WriteCsr16(pBase, MGC_O_HDRC_TXMAXP, bEnd, 0x1400);
        }        
    }     
    
    // set tx polling interval.
    MGC_WriteCsr8(pBase, MGC_O_HDRC_TXINTERVAL, bEnd, interval);

    // set ep tx mode.
    MGC_WriteCsr8(pBase, MGC_O_HDRC_TXCSR2, bEnd, 0x20);
    
    while (uframe < framenum)
    {    
         //  flush data cache before dma.
#if defined (CONFIG_ARCH_MT85XX)
#ifdef SMP_CACHE_OPERATION
        phyaddr = MUSB_DMA_MAP_TO_DEV(U_CACHE_ADDR(buf),U_CACHE_LEN(buf,pktsize));
		phyaddr = U_DMA_MAP_ADDR(phyaddr,buf);
#endif
#else
        HalFlushInvalidateDCache();
        //HalFlushInvalidateDCacheMultipleLine((UINT32)buf,pktsize);
#endif
        // set dma address.
        MGC_Write32(pBase, MGC_HSDMA_CHANNEL_OFFSET(0, MGC_O_HSDMA_ADDRESS), (uint32_t)buf);

        // set data size.
        MGC_Write32(pBase, MGC_HSDMA_CHANNEL_OFFSET(0, MGC_O_HSDMA_COUNT), pktsize);
        
        // trigger dma
        MGC_Write32(pBase, MGC_HSDMA_CHANNEL_OFFSET(0, MGC_O_HSDMA_CONTROL), (0xB |(bEnd << 4)|MGC_M_HSDMA_BURSTMODE));
        
        // wait irq.    
        while (!BIM_IsIrqPending(VECTOR_USB));

        // handle dma interrupt.
        #ifdef UNIFIED_USB
        intr = MGC_Read8(pBase, MGC_O_HSDMA_INTR);
		#else
        intr = MGC_Read32(pBase, MGC_O_HSDMA_INTR);   
		#endif

        MUSB_ASSERT(intr);

        if(intr)
        {
        #ifdef UNIFIED_USB
            MGC_Write8(pBase, MGC_O_HSDMA_INTR, intr);
		#else
            MGC_Write32(pBase, MGC_O_HSDMA_INTR, intr);
		#endif
 #ifdef SMP_CACHE_OPERATION
        phyaddr = U_DMA_UNMAP_ADDR(phyaddr,buf);
        MUSB_DMA_UNMAP_TO_DEV(phyaddr,U_CACHE_LEN(buf,pktsize));
 #endif
            // trigger packet send.
            MGC_WriteCsr8(pBase, MGC_O_HDRC_TXCSR, bEnd, MGC_M_TXCSR_TXPKTRDY);            
        }

        BIM_ClearIrq(VECTOR_USB);

        // wait irq.    
        while (!BIM_IsIrqPending(VECTOR_USB));

        // handle ep interrupt.
        intr = MGC_Read16(pBase, MGC_O_HDRC_INTRTX);
        
        MUSB_ASSERT(intr);
        if(intr)
        {
            MGC_Write16(pBase, MGC_O_HDRC_INTRTX, intr);            
        }

        BIM_ClearIrq(VECTOR_USB);

        buf += pktsize;
        uframe ++;        
    }

    // clear ep tx mode.
    MGC_WriteCsr8(pBase, MGC_O_HDRC_TXCSR2, bEnd, 0x0);
    
    BIM_EnableIrq(VECTOR_USB);
}


/*
 *  MGC_Host_GetISOPkt 
 *  TEST command: receive ISO packet from device.
 */
void MGC_Host_GetISOPkt(uint32_t bEnd, uint32_t pktsize, uint8_t *buf, uint32_t framenum, uint32_t balance, uint32_t interval)
{
    uint32_t   intr;
    uint32_t   count = 0;
    uint32_t   rxcount = 0;
    uint32_t csr;    
    uint32_t  u4MaxPktSz = 0;
    uint8_t *pBase;
    MGC_Port *pPort;
    MUSB_Port *pUsbPort = NULL;
    MGC_Controller *pController;
    const MUSB_Device* pDevice = NULL;
    uint16_t wCount;
#ifdef MTK_MHDRC
    uint8_t bHubAddr = 0;
    uint8_t bHubPort = 0;
    uint8_t bIsMulti = FALSE;
    uint8_t reg;    
#endif

    pUsbPort = MUSB_GetPort(0);
    if(!pUsbPort)
    {
        LOG(0, "Wrong Port number[%d].\n", bPortNum);
        return;
    }
    //MUSB_ASSERT(pUsbPort);        
    pPort = (MGC_Port *)pUsbPort->pPrivateData;
    MUSB_ASSERT(pPort);
    pController = pPort->pController;
    MUSB_ASSERT(pController);
    pBase = (uint8_t *)pController->pControllerAddressIst;
    MUSB_ASSERT(pBase);

    // disable USB interrupt.   
    BIM_DisableIrq(VECTOR_USB); 

    // select ep
    MGC_SelectEnd(pBase, bEnd);    

    // get the last device to test iso.
    wCount = MUSB_ListLength(&(pPort->ConnectedDeviceList));
    MUSB_ASSERT(wCount > 0);
    pDevice = (MUSB_Device *)MUSB_ListFindItem(&(pPort->ConnectedDeviceList), (wCount-1));
    MUSB_ASSERT(pDevice);//klock

#ifdef MTK_MHDRC

    /* target addr & hub addr/port */
    if(pDevice->pParentUsbDevice)
    {
        bHubAddr = pDevice->pParentUsbDevice->bBusAddress;
        bHubPort = pDevice->bHubPort;
        if(2 == pDevice->pParentUsbDevice->DeviceDescriptor.bDeviceProtocol)
        {
            bIsMulti = TRUE;
        }
    }

    reg = (0x10 |bEnd);
    /* speed field in proto reg */
    switch(pDevice->ConnectionSpeed)
    {
        case MUSB_CONNECTION_SPEED_LOW:
            // iso do not support low speed.
            MUSB_ASSERT(0);
            break;
        case MUSB_CONNECTION_SPEED_FULL:
            reg |= 0x80;
            break;
        default:
            reg |= 0x40;
    }
    // set rx ep type.
    MGC_WriteCsr8(pBase, MGC_O_HDRC_RXTYPE, bEnd, reg);        

    //  set special device address to test.
    MGC_Write8(pBase, MGC_BUSCTL_OFFSET(bEnd, MGC_O_MHDRC_RXFUNCADDR), 
        100);
    MGC_Write8(pBase, MGC_BUSCTL_OFFSET(bEnd, MGC_O_MHDRC_RXHUBADDR), 
        (bIsMulti ? (0x80 | bHubAddr) : bHubAddr));
    MGC_Write8(pBase, MGC_BUSCTL_OFFSET(bEnd, MGC_O_MHDRC_RXHUBPORT), 
        bHubPort);
#else
    // set rx ep type.
    MGC_WriteCsr8(pBase, MGC_O_HDRC_RXTYPE, bEnd, (0x10 |bEnd));        

    MGC_Write32(pBase, M_REG_EPXADDR(bEnd), 100);
#endif

    // set fifo size and address.
#ifdef UNIFIED_USB
    MGC_Write8(pBase, MGC_O_HDRC_RXFIFOSZ, 0x9);
#else
    MGC_Write8(pBase, MGC_O_HDRC_RXFIFOSZ, 0xF);
#endif

    MGC_Write16(pBase, MGC_O_HDRC_RXFIFOADD, 8);
    
    // set rx maximum data size.   
    if (pktsize <= 1024)
    {
        if (pDevice->ConnectionSpeed == MUSB_CONNECTION_SPEED_FULL)
        {
            MGC_WriteCsr16(pBase, MGC_O_HDRC_RXMAXP, bEnd, 1023);
        }
        else
        {
            MGC_WriteCsr16(pBase, MGC_O_HDRC_RXMAXP, bEnd, 0x400);
        }
    }
    else if ((pktsize > 1024) && (pktsize < 2048))
    {
        // balance data in two packet: Data1, Data0.
        if (balance)
        {
            u4MaxPktSz = (pktsize + 1)/2;
            Printf("Balance 2 pkts: %d\n", u4MaxPktSz);
            MGC_WriteCsr16(pBase, MGC_O_HDRC_RXMAXP, bEnd, (0x800 |u4MaxPktSz));       	
        }
        else
        {
            Printf("2 pkts: %d\n", pktsize);
            MGC_WriteCsr16(pBase, MGC_O_HDRC_RXMAXP, bEnd, 0xC00);
        }
    }
    else
    {
        // balance data in two packet: Data2, Data1, Data0.
        if (balance)
        {    
            u4MaxPktSz = (pktsize + 2)/3;    
            Printf("Balance 3 pkts: %d\n", u4MaxPktSz);
            MGC_WriteCsr16(pBase, MGC_O_HDRC_RXMAXP, bEnd, (0x1000 |u4MaxPktSz));            
        }
        else
        {
            Printf("3 pkts: %d\n", pktsize);
            MGC_WriteCsr16(pBase, MGC_O_HDRC_RXMAXP, bEnd, 0x1400);            
        }        
    }     

    // set rx polling interval.
    MGC_WriteCsr8(pBase, MGC_O_HDRC_RXINTERVAL, bEnd, interval);       	

    while (count < framenum)
    {
        // request pkt from device.
        MGC_WriteCsr8(pBase, MGC_O_HDRC_RXCSR, bEnd, MGC_M_RXCSR_H_REQPKT);       	

        // wait irq.    
        while (!BIM_IsIrqPending(VECTOR_USB));
#ifdef UNIFIED_USB
	    /* read registers */
	    bIntrUsbValue = MGC_Read8(pBase, MGC_O_HDRC_INTRUSB);
	    wIntrTxValue = MGC_Read16(pBase, MGC_O_HDRC_INTRTX);
	    wIntrRxValue = MGC_Read16(pBase, MGC_O_HDRC_INTRRX);

		LOG(6,"level1 0x%08x 0x%x 0x%x 0x%x 0x%x \n", MGC_Read32(pBase,M_REG_INTRLEVEL1),
		bIntrUsbValue,wIntrTxValue,wIntrRxValue,
		MGC_Read8(pBase, 0x200));
#endif	

        // check if IncompRX happen.
        csr = MGC_ReadCsr16(pBase, MGC_O_HDRC_RXCSR, bEnd);
        if (csr & MGC_M_RXCSR_INCOMPRX)
        {
            MUSB_ASSERT((csr & MGC_M_RXCSR_INCOMPRX) == 0);
        }
                
        // handle usb interrupt.
        intr = MGC_Read8(pBase, MGC_O_HDRC_INTRUSB);      
        if (intr)
        {
            MGC_Write8(pBase, MGC_O_HDRC_INTRUSB, intr);        
        }

        // handle ep interrupt.
        intr = MGC_Read16(pBase, MGC_O_HDRC_INTRRX);
        
        MUSB_ASSERT(intr);
        if(intr)
        {
            MGC_Write16(pBase, MGC_O_HDRC_INTRRX, intr);                
        }

        BIM_ClearIrq(VECTOR_USB);

         //  flush data cache before dma.
#if defined (CONFIG_ARCH_MT85XX)
#ifdef SMP_CACHE_OPERATION
        phyaddr = MUSB_DMA_MAP_FROM_DEV(U_CACHE_ADDR(buf),U_CACHE_LEN(buf,pktsize));
        phyaddr = U_DMA_MAP_ADDR(phyaddr,buf);
#endif
#else
        //HalFlushInvalidateDCache();
        HalFlushInvalidateDCacheMultipleLine((UINT32)buf,MGC_ReadCsr16(pBase, MGC_O_HDRC_RXCOUNT, bEnd));
#endif
        // set dma address.
        MGC_Write32(pBase, MGC_HSDMA_CHANNEL_OFFSET(0, MGC_O_HSDMA_ADDRESS), (uint32_t)buf);

        rxcount = MGC_ReadCsr16(pBase, MGC_O_HDRC_RXCOUNT, bEnd);

        Printf("rxcount: %d\n", rxcount);

        // set data size.
        MGC_Write32(pBase, MGC_HSDMA_CHANNEL_OFFSET(0, MGC_O_HSDMA_COUNT), rxcount);
        
        // trigger dma
        MGC_Write32(pBase, MGC_HSDMA_CHANNEL_OFFSET(0, MGC_O_HSDMA_CONTROL), (0x9 |(bEnd << 4) |MGC_M_HSDMA_BURSTMODE));

        // wait irq.    
        while (!BIM_IsIrqPending(VECTOR_USB));

        // handle dma interrupt.
        #ifdef UNIFIED_USB
		intr = MGC_Read8(pBase, MGC_O_HSDMA_INTR); 
		#else 
        intr = MGC_Read32(pBase, MGC_O_HSDMA_INTR);   
		#endif

        MUSB_ASSERT(intr);

        if(intr)
        {
            #ifdef UNIFIED_USB
            MGC_Write8(pBase, MGC_O_HSDMA_INTR, intr);
			#else
            MGC_Write32(pBase, MGC_O_HSDMA_INTR, intr);
			#endif
#ifdef SMP_CACHE_OPERATION
        phyaddr = U_DMA_UNMAP_ADDR(phyaddr,buf);
        MUSB_DMA_UNMAP_FROM_DEV(phyaddr,U_CACHE_LEN(buf,pktsize));
#endif
            // clear rx status.
		    csr = MGC_ReadCsr8(pBase, MGC_O_HDRC_RXCSR, bEnd);
            MGC_WriteCsr8(pBase, MGC_O_HDRC_RXCSR, bEnd, (csr & ~MGC_M_RXCSR_RXPKTRDY));       	
        }

        BIM_ClearIrq(VECTOR_USB);

        buf += rxcount;
        count ++;        
    }
    
    BIM_EnableIrq(VECTOR_USB);
}

/*
 *  MGC_Dev_SendISOPkt 
 *  TEST command: send ISO packet out
 */
void MGC_Dev_SendISOPkt(uint32_t bEnd, uint32_t pktsize, uint8_t *buf, uint32_t framenum, uint32_t balance)
{
    uint32_t   intr;
    uint32_t   uframe = 0;
    uint32_t  u4MaxPktSz = 0;
    uint8_t *pBase;
    MGC_Port *pPort;
    MUSB_Port *pUsbPort = NULL;
    MGC_Controller *pController;
    Printf("Dev_SendISOPkt:\n");
    pUsbPort = MUSB_GetPort(0);
    if(!pUsbPort)
    {
        LOG(0, "Wrong Port number[%d].\n", bPortNum);
        return;
    }
    //MUSB_ASSERT(pUsbPort);        
    pPort = (MGC_Port *)pUsbPort->pPrivateData;
    MUSB_ASSERT(pPort);
    pController = pPort->pController;
    MUSB_ASSERT(pController);
    pBase = (uint8_t *)pController->pControllerAddressIst;
    MUSB_ASSERT(pBase);

    // disable USB interrupt.   
    BIM_DisableIrq(VECTOR_USB); 

    // select ep
    MGC_SelectEnd(pBase, bEnd);    

    //  set special device address to test.
    MGC_Write8(pBase, MGC_O_HDRC_FADDR, 100);
    
    // set fifo size and address.
#ifdef UNIFIED_USB
    MGC_Write8(pBase, MGC_O_HDRC_TXFIFOSZ, 0x9);
#else
    MGC_Write8(pBase, MGC_O_HDRC_TXFIFOSZ, 0xF);
#endif

    MGC_Write16(pBase, MGC_O_HDRC_TXFIFOADD, 8);

    // set tx maximum data size.  
    if (pktsize <= 1024)
    {
        MGC_WriteCsr16(pBase, MGC_O_HDRC_TXMAXP, bEnd, 0x400);       
    }
    else if ((pktsize > 1024) && (pktsize < 2048))
    {
        // balance data in two packet: MData, Data1.
        if (balance)
        {
            u4MaxPktSz = (pktsize + 1)/2;
            Printf("Balance 2 Pkts %d\n", u4MaxPktSz);
            MGC_WriteCsr16(pBase, MGC_O_HDRC_TXMAXP, bEnd, (0x800 |u4MaxPktSz));            
        }
        else
        {
            Printf("2 Pkts %d\n", u4MaxPktSz);
            MGC_WriteCsr16(pBase, MGC_O_HDRC_TXMAXP, bEnd, 0xC00);            
        }
    }
    else
    {
        // balance data in two packet: MData, Data1.
        if (balance)
        {    
            u4MaxPktSz = (pktsize + 2)/3;    
            Printf("Balance 3 Pkts %d\n", u4MaxPktSz);
            MGC_WriteCsr16(pBase, MGC_O_HDRC_TXMAXP, bEnd, (0x1000 |u4MaxPktSz));
        }
        else
        {
            Printf("3 Pkts %d\n", u4MaxPktSz);
            MGC_WriteCsr16(pBase, MGC_O_HDRC_TXMAXP, bEnd, 0x1400);
        }        
    }     

    // set tx ep type.
    MGC_WriteCsr8(pBase, MGC_O_HDRC_TXTYPE, bEnd, (0x10 |bEnd));

    // set ep tx mode.
    MGC_WriteCsr8(pBase, MGC_O_HDRC_TXCSR2, bEnd, 0x60);
    Printf("Start Dev Send ISO packet. pktsize = 0x%x, framenum =%d\n", pktsize, framenum);  
    while (uframe < framenum)
    {
         //  flush data cache before dma.
#if defined (CONFIG_ARCH_MT85XX)
#ifdef SMP_CACHE_OPERATION
        phyaddr = MUSB_DMA_MAP_TO_DEV(U_CACHE_ADDR(buf),U_CACHE_LEN(buf,pktsize));
        phyaddr = U_DMA_MAP_ADDR(phyaddr,buf);
#endif    
#else
        //HalFlushInvalidateDCache();
        HalFlushInvalidateDCacheMultipleLine((UINT32)buf,pktsize);
#endif    

        // set dma address.
        MGC_Write32(pBase, MGC_HSDMA_CHANNEL_OFFSET(0, MGC_O_HSDMA_ADDRESS), (uint32_t)buf);

        // set data size.
        MGC_Write32(pBase, MGC_HSDMA_CHANNEL_OFFSET(0, MGC_O_HSDMA_COUNT), pktsize);
        
        // trigger dma
        MGC_Write32(pBase, MGC_HSDMA_CHANNEL_OFFSET(0, MGC_O_HSDMA_CONTROL), (0xB |(bEnd << 4)|MGC_M_HSDMA_BURSTMODE));

        // wait irq.    
        while (!BIM_IsIrqPending(VECTOR_USB));
        Printf("Handel DMA iterrupt pktsize = 0x%x.\n", pktsize);
        // handle dma interrupt.
        #ifdef UNIFIED_USB
        intr = MGC_Read8(pBase, MGC_O_HSDMA_INTR);
		#else
        intr = MGC_Read32(pBase, MGC_O_HSDMA_INTR);   
		#endif

        MUSB_ASSERT(intr);

        if(intr)
        {
        #ifdef UNIFIED_USB
           MGC_Write8(pBase, MGC_O_HSDMA_INTR, intr);
		#else
            MGC_Write32(pBase, MGC_O_HSDMA_INTR, intr);
		#endif
#ifdef SMP_CACHE_OPERATION
        phyaddr = U_DMA_UNMAP_ADDR(phyaddr,buf);
        MUSB_DMA_UNMAP_TO_DEV(phyaddr,U_CACHE_LEN(buf,pktsize));
#endif
            // trigger packet send.
            MGC_WriteCsr8(pBase, MGC_O_HDRC_TXCSR, bEnd, MGC_M_TXCSR_TXPKTRDY);            
        }

        BIM_ClearIrq(VECTOR_USB);

        // wait irq.    
        while (!BIM_IsIrqPending(VECTOR_USB));

        // handle ep interrupt.
        intr = MGC_Read16(pBase, MGC_O_HDRC_INTRTX);
        
        MUSB_ASSERT(intr);
        if(intr)
        {
            MGC_Write16(pBase, MGC_O_HDRC_INTRTX, intr);            
        }

        BIM_ClearIrq(VECTOR_USB);

        buf += pktsize;
        uframe ++;        
    }

    // clear ep tx mode.
    MGC_WriteCsr8(pBase, MGC_O_HDRC_TXCSR2, bEnd, 0);
    
    BIM_EnableIrq(VECTOR_USB);
}


/*
 *  MGC_Dev_GetISOPkt 
 *  TEST command: receive ISO packet from host.
 */
uint32_t MGC_Dev_GetISOPkt(uint32_t bEnd, uint8_t *buf, uint32_t framenum)
{
    uint32_t   intr;
    uint32_t   count = 0;
    uint32_t   rxcount = 0;
    uint32_t csr;    
    uint8_t *pBase;
    MGC_Port *pPort;
    MUSB_Port *pUsbPort = NULL;
    MGC_Controller *pController;

    pUsbPort = MUSB_GetPort(0);

    if(!pUsbPort)
    {
        LOG(0, "Wrong Port number[%d].\n", pUsbPort);
        return 0;
    }
    //MUSB_ASSERT(pUsbPort);        

    pPort = (MGC_Port *)pUsbPort->pPrivateData;
    MUSB_ASSERT(pPort);
    pController = pPort->pController;
    MUSB_ASSERT(pController);
    pBase = (uint8_t *)pController->pControllerAddressIst;
    MUSB_ASSERT(pBase);

    // disable USB interrupt.   
    BIM_DisableIrq(VECTOR_USB); 

    // select ep
    MGC_SelectEnd(pBase, bEnd);    

    //  set special device address to test.
    MGC_Write8(pBase, MGC_O_HDRC_FADDR, 100);

    // set fifo size and address.
#ifdef UNIFIED_USB
    MGC_Write8(pBase, MGC_O_HDRC_RXFIFOSZ, 0x9);
#else
    MGC_Write8(pBase, MGC_O_HDRC_RXFIFOSZ, 0xF);
#endif

    MGC_Write16(pBase, MGC_O_HDRC_RXFIFOADD, 8);
	
    // set rx maximum data size.   
    MGC_WriteCsr16(pBase, MGC_O_HDRC_RXMAXP, bEnd, 0x1400);       	

    // set rx ep type.
    MGC_WriteCsr16(pBase, MGC_O_HDRC_RXTYPE, bEnd, (0x10 |bEnd));
    
    // set ISO mode in device mode.
    MGC_WriteCsr8(pBase, MGC_O_HDRC_RXCSR2, bEnd, 0x40);
    
    Printf("Start Dev GetISOPacket count = %d, framenum = %d:\n", count, framenum);    

    while (count < framenum)
    {
        // wait irq.    
        while (!BIM_IsIrqPending(VECTOR_USB));
        // handle usb interrupt.
        intr = MGC_Read8(pBase, MGC_O_HDRC_INTRUSB);      
        Printf(" Dev Get USB Interrupt count=%d:\n", count);
	Printf("interrrupt=0%x, 0%x, 0%x.\n", intr, MGC_Read16(pBase, MGC_O_HDRC_INTRTX), MGC_Read16(pBase, MGC_O_HDRC_INTRRX));
	#ifdef MUSB_QMU
	if((count + 1) >= framenum) 
	    break;
	#endif

        if (intr)
        {
            MGC_Write8(pBase, MGC_O_HDRC_INTRUSB, intr);        
        }

        // handle ep interrupt.
        intr = MGC_Read16(pBase, MGC_O_HDRC_INTRRX);
        
        MUSB_ASSERT(intr);
        if(intr)
        {
            MGC_Write16(pBase, MGC_O_HDRC_INTRRX, intr);                
        }

        BIM_ClearIrq(VECTOR_USB);

         //  flush data cache before dma.
#if defined (CONFIG_ARCH_MT85XX)
#ifdef SMP_CACHE_OPERATION
        phyaddr = MUSB_DMA_MAP_FROM_DEV(U_CACHE_ADDR(buf),U_CACHE_LEN(buf,rxcount));
        phyaddr = U_DMA_MAP_ADDR(phyaddr,buf);
#endif    
#else
        //HalFlushInvalidateDCache();
        HalFlushInvalidateDCacheMultipleLine((UINT32)buf,MGC_ReadCsr16(pBase, MGC_O_HDRC_RXCOUNT, bEnd));
#endif    
        // set dma address.
        MGC_Write32(pBase, MGC_HSDMA_CHANNEL_OFFSET(0, MGC_O_HSDMA_ADDRESS), (uint32_t)buf);

        rxcount = MGC_ReadCsr16(pBase, MGC_O_HDRC_RXCOUNT, bEnd);

        // set data size.
        MGC_Write32(pBase, MGC_HSDMA_CHANNEL_OFFSET(0, MGC_O_HSDMA_COUNT), rxcount);
        
        // trigger dma
        MGC_Write32(pBase, MGC_HSDMA_CHANNEL_OFFSET(0, MGC_O_HSDMA_CONTROL), (0x9 |(bEnd << 4)|MGC_M_HSDMA_BURSTMODE));

        // wait irq.    
        while (!BIM_IsIrqPending(VECTOR_USB));

        // handle dma interrupt.
        #ifdef UNIFIED_USB
         intr = MGC_Read8(pBase, MGC_O_HSDMA_INTR);  
		#else
        intr = MGC_Read32(pBase, MGC_O_HSDMA_INTR);   
		#endif

        MUSB_ASSERT(intr);

        if(intr)
        {
        #ifdef UNIFIED_USB
		MGC_Write8(pBase, MGC_O_HSDMA_INTR, intr);

		#else
            MGC_Write32(pBase, MGC_O_HSDMA_INTR, intr);
		#endif
#ifdef SMP_CACHE_OPERATION
        phyaddr = U_DMA_UNMAP_ADDR(phyaddr,buf);
        MUSB_DMA_UNMAP_FROM_DEV(phyaddr,U_CACHE_LEN(buf,rxcount));
#endif
            // clear rx status.
		    csr = MGC_ReadCsr8(pBase, MGC_O_HDRC_RXCSR, bEnd);
            MGC_WriteCsr8(pBase, MGC_O_HDRC_RXCSR, bEnd, (csr & ~MGC_M_RXCSR_RXPKTRDY));       	
        }

        BIM_ClearIrq(VECTOR_USB);

        buf += rxcount;
        count ++;        
    }
    
    BIM_EnableIrq(VECTOR_USB);

    return rxcount;
}

#endif /* MUSB_ISO_EMULATION enabled */

#ifdef MUSB_QMU
void MGC_QMU_Host_SendPkt(uint8_t btxEnd, uint32_t u4Type,uint32_t framenum, 
                                     uint8_t ishost, uint8_t num_of_packet)
{
    uint8_t *pBase;
    MGC_Port *pPort;
    MUSB_Port *pUsbPort = NULL;
    MGC_Controller *pController;
    const MUSB_Device* pDevice = NULL;
    uint16_t wCount;
    int temp;
    uint8_t reg;    
    uint32_t bEnd = btxEnd;
    pUsbPort = MUSB_GetPort(0);
		if(!pUsbPort)
		{
			LOG(0, "Wrong Port number[%d].\n", bPortNum);
			return;
		}
		
		//MUSB_ASSERT(pUsbPort);		
    pPort = (MGC_Port *)pUsbPort->pPrivateData;
    MUSB_ASSERT(pPort);
    pController = pPort->pController;
    MUSB_ASSERT(pController);
    pBase = (uint8_t *)pController->pControllerAddressIst;
    MUSB_ASSERT(pBase);

    // disable USB interrupt.   
    BIM_DisableIrq(VECTOR_USB); 

    // select ep
    MGC_SelectEnd(pBase, bEnd);

    // get the last device to test iso.
    wCount = MUSB_ListLength(&(pPort->ConnectedDeviceList));
    MUSB_ASSERT(wCount > 0);
    pDevice = (MUSB_Device *)MUSB_ListFindItem(&(pPort->ConnectedDeviceList), (wCount-1));

    reg = (0x10 |bEnd);
    /* speed field in proto reg */
    switch(pDevice->ConnectionSpeed)
    {
        case MUSB_CONNECTION_SPEED_LOW:
            // iso do not support low speed.
            MUSB_ASSERT(0);
            break;
        case MUSB_CONNECTION_SPEED_FULL:
            reg |= 0x80;
            break;
        default:
            reg |= 0x40;
    }
    // set tx ep type.
    MGC_WriteCsr8(pBase, MGC_O_HDRC_TXTYPE, bEnd, reg);

    // set ep tx mode.
    MGC_WriteCsr8(pBase, MGC_O_HDRC_TXCSR2, bEnd, 0x20);

    BIM_EnableIrq(VECTOR_USB);
    _MUSB_QMU_Transfer(bEnd, u4Type, framenum, ishost, num_of_packet);

        // clear ep tx mode.
    MGC_WriteCsr8(pBase, MGC_O_HDRC_TXCSR2, bEnd, 0x0);
    temp = 0xff;
    MGC_QMU_Write8 (MGC_O_HDRC_INTRUSBE, temp);
    MGC_QMU_Write16 (MGC_O_HDRC_INTRTXE, 0xff);
    MGC_QMU_Write16 (MGC_O_HDRC_INTRRXE, 0xff);
    return;
}


void MGC_QMU_Host_GetPkt(uint8_t rxend, uint32_t u4Type, 
			uint32_t framenum, uint8_t ishost, uint8_t num_of_packet)
{

    uint32_t csr;    
    uint8_t *pBase;
    MGC_Port *pPort;
    MUSB_Port *pUsbPort = NULL;
    MGC_Controller *pController;
    const MUSB_Device* pDevice = NULL;
    uint16_t wCount;
    uint8_t reg;    

    uint32_t bEnd = rxend;
    
    pUsbPort = MUSB_GetPort(0);
    MUSB_ASSERT(pUsbPort);        
    pPort = (MGC_Port *)pUsbPort->pPrivateData;
    MUSB_ASSERT(pPort);
    pController = pPort->pController;
    MUSB_ASSERT(pController);
    pBase = (uint8_t *)pController->pControllerAddressIst;
    MUSB_ASSERT(pBase);

    // disable USB interrupt.   
    BIM_DisableIrq(VECTOR_USB); 

    // select ep
    MGC_SelectEnd(pBase, bEnd);    

    // get the last device to test iso.
    wCount = MUSB_ListLength(&(pPort->ConnectedDeviceList));
    MUSB_ASSERT(wCount > 0);
    pDevice = (MUSB_Device *)MUSB_ListFindItem(&(pPort->ConnectedDeviceList), (wCount-1));

    reg = (0x10 |bEnd);
    /* speed field in proto reg */
    switch(pDevice->ConnectionSpeed)
    {
        case MUSB_CONNECTION_SPEED_LOW:
            // iso do not support low speed.
            MUSB_ASSERT(0);
            break;
        case MUSB_CONNECTION_SPEED_FULL:
            reg |= 0x80;
            break;
        default:
            reg |= 0x40;
    }
    // set rx ep type.
    MGC_WriteCsr8(pBase, MGC_O_HDRC_RXTYPE, bEnd, reg);        

    BIM_EnableIrq(VECTOR_USB);
    _MUSB_QMU_Transfer(bEnd,u4Type, framenum, ishost, num_of_packet);
        
    // clear rx status.
    csr = MGC_ReadCsr8(pBase, MGC_O_HDRC_RXCSR, bEnd);
    MGC_WriteCsr8(pBase, MGC_O_HDRC_RXCSR, bEnd, (csr & ~MGC_M_RXCSR_RXPKTRDY));        
   
    return;
}



/*
 *  MGC_Dev_GetISOPkt 
 *  TEST command: receive ISO packet from host.
 */
uint32_t MGC_QMU_Device_GetPkt(uint8_t rxend, uint32_t u4Type, 
									uint32_t framenum, uint8_t ishost, uint8_t num_of_packet)
{
    uint32_t   count = 0;
    uint32_t   rxcount = 0;
	uint8_t *pBase;
	uint8_t bEnd;
    MGC_Port *pPort;
    MUSB_Port *pUsbPort = NULL;
    MGC_Controller *pController;

    pUsbPort = MUSB_GetPort(0);
		if(!pUsbPort)
		{
			LOG(0, "Wrong Port number[%d].\n", bPortNum);
			return;
		}
		//MUSB_ASSERT(pUsbPort);		
    pPort = (MGC_Port *)pUsbPort->pPrivateData;
    MUSB_ASSERT(pPort);
    pController = pPort->pController;
    MUSB_ASSERT(pController);
    pBase = (uint8_t *)pController->pControllerAddressIst;
    MUSB_ASSERT(pBase);

	bEnd = rxend;
	
    // disable USB interrupt.   
    BIM_DisableIrq(VECTOR_USB); 

    // select ep
    MGC_SelectEnd(pBase, bEnd);    

    //  set special device address to test.
    MGC_Write8(pBase, MGC_O_HDRC_FADDR, 100);

    // set fifo size and address.
    MGC_Write8(pBase, MGC_O_HDRC_RXFIFOSZ, 0x9);

    MGC_Write16(pBase, MGC_O_HDRC_RXFIFOADD, 8);
	
    // set rx maximum data size.   
    MGC_WriteCsr16(pBase, MGC_O_HDRC_RXMAXP, bEnd, 0x1400);       	

    // set rx ep type.
    MGC_WriteCsr16(pBase, MGC_O_HDRC_RXTYPE, bEnd, (0x10 |bEnd));
    
    // set ISO mode in device mode.
    MGC_WriteCsr8(pBase, MGC_O_HDRC_RXCSR2, bEnd, 0x40);
    
    Printf("Start Dev GetISOPacket count = %d, framenum = %d:\n", count, framenum);    

	BIM_EnableIrq(VECTOR_USB);
	_MUSB_QMU_Transfer(bEnd,u4Type, framenum, ishost, num_of_packet);

    return rxcount;
}


#endif

#endif //#if defined(CONFIG_ARCH_MT85XX)

