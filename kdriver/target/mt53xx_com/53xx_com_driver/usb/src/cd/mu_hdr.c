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
 * MUSBStack-S HDRC implementation.
 * $Revision: #2 $
 */
#if defined(CONFIG_ARCH_MT85XX) //#ifdef CONFIG_DRV_LINUX
#include <linux/delay.h>
#endif    

#include "mu_impl.h"

#if defined (CONFIG_ARCH_MT85XX)
//#ifdef CC_MT8530 
#include "x_hal_ic.h"
//#endif
#endif    
#ifdef MUSB_HDRC

/** 
 * Undefine this to avoid embedded HSET overheads 
 * (but this would preclude USB-IF high-speed OTG certification)
 */
//#define MUSB_EHSET

/*
* Implementations of HDRC-specific functions
*/

//#include "hdrc_cnf.h"

#include "mu_diag.h"

#include "mu_hdrpr.h"
#include "mu_hdrdf.h"
#include "mu_pippr.h"
#include "mu_funpr.h"
#include "mu_hstpr.h"

#ifdef MUSB_HUB
#include "mu_hub.h"
#include "mu_hdef.h"
#include "mu_hcreq.h"
#include "mu_hcore.h"
#include "mu_hapi.h"
#endif

#ifndef MUSB_NONEOS
#include "mu_sys.h"
#else
#include "mu_none.h"
#endif

#if defined(CONFIG_ARCH_MT85XX)
#define TZ_TEST
#endif

#ifdef TZ_TEST
#include "tz_protect.h"
#include <linux/slab.h>
extern unsigned int enableTzProtectTest;
TZ_PROT_PERMIT_T tzUsb0Permit = {
    .u4Permit_MainID = TZ_DRAM_AGENT(4) | TZ_DRAM_AGENT(12),
    .u4Permit_SubID = TZ_PROT_ALL_SUBID,
    .u4Prot_RW= TZ_PROT_BIT_MODE_READ|TZ_PROT_BIT_MODE_WRITE //read-protected and write-protected
};

static char* tzProtectZone0 = NULL;
static char* tzProtectZone1 = NULL;
static char* tzProtectZone2 = NULL;

#define TZ_PROTECTZONG_LEN 1024
#define TZ_USB0_ID		   0
#define TZ_USB1_ID		   1
#define TZ_USB2_ID		   2
#endif
#ifdef MUSB_BC12
extern UINT32 IS_SupportBC12(VOID);
#endif


#ifdef MTK_MHDRC
#ifdef UNIFIED_USB
#define MGC_FIFO_CNT    MGC_Write32
#else
#define MGC_FIFO_CNT    MGC_MISC_Write32
#endif
#else
#define MGC_FIFO_CNT    MGC_Write32
#endif

#if !defined (CONFIG_ARCH_MT85XX)
	#ifdef MUSB_LOADER
	#define MGC_CHECK_INSERT_DEBOUNCE   10
	#else
	#define MGC_CHECK_INSERT_DEBOUNCE   500
	#endif
#else
	#define MGC_CHECK_INSERT_DEBOUNCE   500
#endif

//#define TX_RX_VERIFICATION

static uint32_t MGC_dwCheckInsert = 0;
#if defined(MUSB_FUNCTION) || defined(MUSB_EHSET)

static const uint8_t MGC_aTestPacket [] =
{
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa,
    0xaa, 0xee, 0xee, 0xee, 0xee, 0xee, 0xee, 0xee, 
    0xee, 0xfe, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0x7f, 0xbf, 0xdf, 
    0xef, 0xf7, 0xfb, 0xfd, 0xfc, 0x7e, 0xbf, 0xdf,
    0xef, 0xf7, 0xfb, 0xfd, 0x7e
};
#endif

#if !defined (CONFIG_ARCH_MT85XX)
static uint8_t MGC_PortEpNumConfig[] = 
{
 MGC_USB_PORT0_ENDS,MGC_USB_PORT1_ENDS,MGC_USB_PORT2_ENDS,MGC_USB_PORT3_ENDS
};
#endif

#ifdef MUSB_C_DYNFIFO_DEF
//  MTK Notice: Max Liao, 2007/04/04.
//  change to static allocation.
static MGC_EndpointResource _arMGC_EndpointResource[MUSB_MAX_CONTROLLERS][MUSB_C_NUM_EPS];
#endif
static uint8_t bRetries = 0;

#ifndef MUSB_DISABLE_DB_DYNFIFO            
static uint8_t MGC_bDBFifoEnable = FALSE;
#endif
#ifdef MUSB_POWERDOWN
static uint32_t MGC_POWERDWN_CASE = 0;
static uint32_t MGC_WAKEUP_CASE = 0;
static uint32_t MGC_INT_SHOW_ENABLE = FALSE;
#endif


#if !defined (CONFIG_ARCH_MT85XX)
extern uint8_t MGC_Ep_No(void *pBase);

uint8_t MGC_Ep_No(void *pBase)
{
	uint8_t bEp_No = 0;
	if((uint32_t)pBase == MUSB_BASE)
		bEp_No = MGC_PortEpNumConfig[0];
	else if((uint32_t)pBase == MUSB1_BASE)
		bEp_No = MGC_PortEpNumConfig[1];
	else if((uint32_t)pBase == MUSB2_BASE)
		bEp_No = MGC_PortEpNumConfig[2];
	else if((uint32_t)pBase == MUSB3_BASE)
		bEp_No = MGC_PortEpNumConfig[3];
	return bEp_No;
}
#endif

static void MGC_HdrcIrpStatusChanged(MGC_Port *pPort, MGC_EndpointResource *pEnd, uint8_t bTransmit)
{
#ifdef MUSB_DMA
    MUSB_ASSERT(pPort);
    MUSB_ASSERT(pEnd);

    /* TODO: something more specific might be better */
    if ((bTransmit) && (pEnd->pDmaChannel))
    {
        if (MUSB_DMA_STATUS_FREE == pPort->pDmaController->pfDmaGetChannelStatus(pEnd->pDmaChannel))
        {
            pEnd->dwTxOffset = pEnd->pDmaChannel->dwActualLength;

            pPort->pDmaController->pfDmaReleaseChannel(pEnd->pDmaChannel);
            pEnd->pDmaChannel = NULL;            
        }
    }
    else if ((!bTransmit) && (pEnd->pRxDmaChannel))
    {
        if (MUSB_DMA_STATUS_FREE == pPort->pDmaController->pfDmaGetChannelStatus(pEnd->pRxDmaChannel))
        {
            pEnd->dwRxOffset = pEnd->pRxDmaChannel->dwActualLength;
            pPort->pDmaController->pfDmaReleaseChannel(pEnd->pRxDmaChannel);
            pEnd->pRxDmaChannel = NULL;        
        }
    }    
#endif
}

/*
 * Discover HDRC FIFO configuration
 */

#if defined (CONFIG_ARCH_MT85XX)
#ifdef UNIFIED_USB
uint8_t MGC_PortEpNumConfig[] = 
{
#if defined(CC_MT8580)
 11,11,11,
#elif defined(CC_MT8563)
6,6,6
#endif
};
#endif
#endif //#if defined (CONFIG_ARCH_MT85XX)

uint8_t MGC_HdrcInit(MGC_Port *pPort)
{
#ifdef MUSB_C_DYNFIFO_DEF

    MGC_EndpointResource end;

    uint8_t bEnd;
#endif
    uint8_t config;
    MGC_Controller *pImplController;
    uint8_t *pBase;
    uint32_t dwFifoAddr = 0;

    MUSB_ASSERT(pPort);
    pImplController = pPort->pController;
    MUSB_ASSERT(pImplController);
    pBase = (uint8_t *)pImplController->pControllerAddressIst;
    MUSB_ASSERT(pBase);

    //  MTK Notice: Max Liao, 2006/11/17.
    //  disable ping in control ep operation.
    pPort->bHasDisablePing = TRUE;

    /* read config data */
    MGC_SelectEnd(pBase, 0);
    config = MGC_ReadCsr8(pBase, MGC_O_HDRC_CONFIGDATA, 0);
#ifdef MUSB_C_DYNFIFO_DEF

    if (!(config & MGC_M_CONFIGDATA_DYNFIFO))
    {
        MGC_DIAG(1, pImplController,
                 "Dynamic FIFO support built but not present in hardware; please rebuild");
        return FALSE;
    }

//  MTK Notice: Max Liao, 2007/04/03.
//  change to static allocation.    
//    pPort->pPrivateData = MUSB_MemAlloc(MUSB_C_NUM_EPS *sizeof(MGC_EndpointResource));
    MUSB_ASSERT(MGC_bSystemCount < MUSB_MAX_CONTROLLERS);
    if (MGC_bSystemCount >= (uint8_t)MUSB_MAX_CONTROLLERS)
    {
        return FALSE;
    }

#ifdef UNIFIED_USB
    pPort->bEndCount = MGC_PortEpNumConfig[MGC_bSystemCount];
    if(pPort->bEndCount > MUSB_C_NUM_EPS)
    {
      LOG(0,"endpoint static mem nick\n");
	  return FALSE;
    }
#else
    pPort->bEndCount = MUSB_C_NUM_EPS;
#endif
	
    pPort->wEndMask = 1;

    pPort->pPrivateData = (void*)&_arMGC_EndpointResource[MGC_bSystemCount][0];

    if (pPort->pPrivateData)
    {
#if defined (CONFIG_ARCH_MT85XX)
        if (MUSB_ArrayInit(&(pPort->LocalEnds), sizeof(MGC_EndpointResource), MUSB_C_NUM_EPS,
#else
        if (MUSB_ArrayInit(&(pPort->LocalEnds), sizeof(MGC_EndpointResource), pPort->bEndCount,
#endif
                           pPort->pPrivateData))
        {
            /* add endpoint 0 */
            MUSB_MemSet((void *)&end, 0, sizeof(end));

            end.bIsFifoShared = TRUE;
            end.bTrafficType = MUSB_ENDPOINT_XFER_CONTROL;
            end.wMaxPacketSizeTx = MGC_END0_FIFOSIZE;
            end.wMaxPacketSizeRx = MGC_END0_FIFOSIZE;
            end.bIsClaimed = TRUE;
            MUSB_ArrayAppend(&(pPort->LocalEnds), &end);
            MGC_SelectEnd(pBase, 0);
            MGC_Write8(pBase, MGC_O_HDRC_RXFIFOSZ, 3);
            MGC_Write8(pBase, MGC_O_HDRC_TXFIFOSZ, 3);
            MGC_Write16(pBase, MGC_O_HDRC_RXFIFOADD, 0);
            MGC_Write16(pBase, MGC_O_HDRC_TXFIFOADD, 0);
            dwFifoAddr = MGC_END0_FIFOSIZE;
            /* add others, but don't set sizes */

#ifdef TX_RX_VERIFICATION //for MT53XX
            // Set fifo size = 512 bytes per endpoint.             
            // TX and RX are shared the same fifo.
            for (bEnd = 1; bEnd < (uint8_t)(pPort->bEndCount & 0xff); bEnd++)
            {
            if((bEnd>=1)&&(bEnd<=5)) //endpoint0~5 contain both TX RX
            	{
                MUSB_MemSet((void *)&end, 0, sizeof(end));
                end.bIsFifoShared = TRUE;
                end.bLocalEnd = bEnd;
                end.wMaxPacketSizeTx = 512;
                end.wMaxPacketSizeRx = 512;              
                MUSB_ArrayAppend(&(pPort->LocalEnds), &end);
                MGC_SelectEnd(pBase, bEnd);
                MGC_Write8(pBase, MGC_O_HDRC_RXFIFOSZ, 6);
                MGC_Write8(pBase, MGC_O_HDRC_TXFIFOSZ, 6);
                MGC_Write16(pBase, MGC_O_HDRC_RXFIFOADD, dwFifoAddr/8);
                MGC_Write16(pBase, MGC_O_HDRC_TXFIFOADD, dwFifoAddr/8);
                dwFifoAddr += 512;                
                pPort->wEndMask |= (1 << bEnd);
            	}
	    else 
		{
			MUSB_MemSet((void *)&end, 0, sizeof(end));
			 end.bIsFifoShared = FALSE;
			 end.bLocalEnd = bEnd;
			 end.wMaxPacketSizeTx = 512;
			 end.wMaxPacketSizeRx = 512;			  
			 MUSB_ArrayAppend(&(pPort->LocalEnds), &end);
			 MGC_SelectEnd(pBase, bEnd);
			 MGC_Write8(pBase, MGC_O_HDRC_RXFIFOSZ, 6);
			
			 MGC_Write16(pBase, MGC_O_HDRC_RXFIFOADD, dwFifoAddr/8);
			 dwFifoAddr += 512; 			   
			
			 pPort->wEndMask |= (1 << bEnd);

		}
            }
#else
          #if 1
	      // Single Packet Buffer  set to if 0 when test Double packet buffer
            // Set fifo size = 512 bytes per endpoint.             
            // TX and RX are not shared the same fifo.
            #ifdef UNIFIED_USB
                for (bEnd = 1; bEnd < (uint8_t)(pPort->bEndCount & 0xff); bEnd++)
            #else
                for (bEnd = 1; bEnd < (uint8_t)(MUSB_C_NUM_EPS & 0xff); bEnd++)
            #endif	
            {
                MUSB_MemSet((void *)&end, 0, sizeof(end));
                end.bIsFifoShared = FALSE;
                end.bLocalEnd = bEnd;
		 #if (!defined (CC_MT5881)) || (defined (CONFIG_ARCH_MT85XX))
                end.wMaxPacketSizeTx = 512;
                end.wMaxPacketSizeRx = 512;  
		#else //MT5881 port 0: 1TX 2RX set port1 to RX with 64 bytes FIFO
		 if((2 ==bEnd)&&(MUSB_BASE == (uint32_t)pBase))
		 {
			end.wMaxPacketSizeTx = 0;
			end.wMaxPacketSizeRx = 64; 
		 }
		else
		{
			end.wMaxPacketSizeTx = 512;
			end.wMaxPacketSizeRx = 512;  
		}
		#endif
                MUSB_ArrayAppend(&(pPort->LocalEnds), &end);
                MGC_SelectEnd(pBase, bEnd);
		 #if (!defined (CC_MT5881)) || (defined (CONFIG_ARCH_MT85XX))
                MGC_Write8(pBase, MGC_O_HDRC_RXFIFOSZ, 6);
                MGC_Write8(pBase, MGC_O_HDRC_TXFIFOSZ, 6);
                MGC_Write16(pBase, MGC_O_HDRC_RXFIFOADD, dwFifoAddr/8);
                dwFifoAddr += 512;                
                MGC_Write16(pBase, MGC_O_HDRC_TXFIFOADD, dwFifoAddr/8);
                dwFifoAddr += 512;
		#else //MT5881 port 0: 1TX 2RX set port1 to RX with 64 bytes FIFO
		if((2 == bEnd)&&(MUSB_BASE == (uint32_t)pBase))
		{
			MGC_Write8(pBase, MGC_O_HDRC_RXFIFOSZ, 3);
			MGC_Write16(pBase, MGC_O_HDRC_RXFIFOADD, dwFifoAddr/8);
			dwFifoAddr += 64;				 
		}
		else
		{
                	MGC_Write8(pBase, MGC_O_HDRC_RXFIFOSZ, 6);
                	MGC_Write8(pBase, MGC_O_HDRC_TXFIFOSZ, 6);
                	MGC_Write16(pBase, MGC_O_HDRC_RXFIFOADD, dwFifoAddr/8);
                	dwFifoAddr += 512;                
               	 MGC_Write16(pBase, MGC_O_HDRC_TXFIFOADD, dwFifoAddr/8);
                	dwFifoAddr += 512;
		}
		#endif
                pPort->wEndMask |= (1 << bEnd);
            }
	#else
	// Double Packet Buffer
	 // Set fifo size = 512 bytes per endpoint.			  
	 // TX and RX are shared the same fifo.
	for (bEnd = 1; bEnd < (uint8_t)(pPort->bEndCount & 0xff); bEnd++)
	{
	       MUSB_MemSet((void *)&end, 0, sizeof(end));
              end.bIsFifoShared = FALSE;
              end.bLocalEnd = bEnd;
              end.wMaxPacketSizeTx = 512;
              end.wMaxPacketSizeRx = 512;              
              MUSB_ArrayAppend(&(pPort->LocalEnds), &end);
		MGC_SelectEnd(pBase, bEnd);
		MGC_Write8(pBase, MGC_O_HDRC_RXFIFOSZ, 0x16);
		MGC_Write8(pBase, MGC_O_HDRC_TXFIFOSZ, 0x16);
		MGC_Write16(pBase, MGC_O_HDRC_RXFIFOADD, dwFifoAddr/8);
		MGC_Write16(pBase, MGC_O_HDRC_TXFIFOADD, dwFifoAddr/8);
		dwFifoAddr += 1024;
		pPort->wEndMask |= (1 << bEnd);
	}

	#endif
#endif
            return TRUE;
        }
    }

    return FALSE;
#else

    if (config & MGC_M_CONFIGDATA_DYNFIFO)
    {
        MGC_DIAG(1, pImplController,
                 "Dynamic FIFOs detected but support not built; please rebuild");
        return FALSE;
    }

    return MGC_DrcInit(pPort);
#endif

}

#ifdef MUSB_C_DYNFIFO_DEF
MGC_EndpointResource *MGC_HdrcBindDynamicEndpoint(MGC_Port *pPort,
                                                  const MUSB_DeviceEndpoint *pUsbEnd,
                                                  uint8_t bBind,
                                                  uint8_t *pbIsTx)
{
    return MGC_AllocateDynamicFifo(pPort, pUsbEnd, bBind, pbIsTx);
}
#endif /* dynamic FIFOs */

#ifdef MUSB_EHSET
static void MGC_HdrcResumeOff(void *pParam, uint16_t wTimer)
{
    MGC_Controller *pController;
    MGC_Port *pPort;
    
    UNUSED(wTimer);
    MUSB_ASSERT(pParam);
    pController = (MGC_Controller *)pParam;
    MUSB_ASSERT(pController);
    pPort = (MGC_Port *)pController->pPort;
    MUSB_ASSERT(pPort);

    pPort->bWantResume = FALSE;
    MGC_HdrcProgramBusState(pPort);

    LOG(5, "HS_HOST_CLEAR_RESUME.\n", NULL);
}
#endif

static void MGC_HdrcCheckConnect(void *pParam, uint16_t wTimer)
{
    MGC_Controller *pController;

    MGC_Port *pPort;
    MUSB_SystemServices *pServices;
    uint32_t dwLineState;
    MGC_BsrItem qItem;
    uint8_t bRet;
    uint8_t *pBase;
    MUSB_ASSERT(pParam);    
    pController = (MGC_Controller *)pParam;
    pPort = pController->pPort;
    MUSB_ASSERT(pPort);    
    MUSB_ASSERT(pPort->pController);    
    pBase = (uint8_t *)pPort->pController->pControllerAddressIst;
    MUSB_ASSERT(pBase);
    pServices = pPort->pController->pSystemServices;
    MUSB_ASSERT(pServices);    
    #ifdef MUSB_BC12
    #if defined(CC_MT5881) || defined(CC_MT5890) || defined(CC_MT5882)
    uint32_t dwReg=0;	
    #endif
    #endif
#if !defined (CONFIG_ARCH_MT85XX)
    dwLineState = MGC_PHY_Read32(pBase, M_REG_U2PHYDMON1);
#else
#ifdef UNIFIED_USB
    dwLineState = MGC_Read32(pBase,M_REG_DEBUG_PROBE);
	//LOG(0,"EMULATION: software set ok linestate directly\n");
#else
    dwLineState = MGC_PHY_Read32(pBase, M_REG_PHYC5);
#endif
#endif

    dwLineState &= M_REG_LINESTATE_MASK;	

    if (dwLineState == LINESTATE_DISCONNECT)
    {
#ifdef MUSB_LOADER
        pPort->bInsert = FALSE;   
#endif
        return;
    }

    if (dwLineState == LINESTATE_HWERROR)
    {      
#ifdef MUSB_LOADER
        pPort->bInsert = FALSE;    
#endif
        
        MGC_dwCheckInsert++;

        if (MGC_dwCheckInsert > 5)
        {
            LOG(1, "LINESTATE_HWERROR !!!\n", NULL);               
            return;
        }

        MUSB_ASSERT(pParam);
        MUSB_ASSERT(pController->pSystemServices);
        MUSB_ASSERT(pController->pSystemServices->pfArmTimer);

        //Timer Index 0 is reserved for EP0 device configuration. 
        bRet = pController->pSystemServices->pfArmTimer(
            pController->pSystemServices->pPrivateData, 
            MUSB_DEV_CONFIG_TIMER_IDX /*bTimerIndex*/,
            MGC_CHECK_INSERT_DEBOUNCE, FALSE,
            MGC_HdrcCheckConnect, pController);
        UNUSED(bRet);            
        return;
    }
#ifdef MUSB_BC12
#if defined(CC_MT5881) || defined(CC_MT5890) || defined(CC_MT5882)
if(IS_SupportBC12())
{
	#if defined(CC_MT5881)
	if(MUSB1_BASE == pBase)
	#elif defined(CC_MT5890) || defined(CC_MT5882)
	if(MUSB_BASE == pBase)
	#endif
	{
		LOG(0, "Disable BC12 function.\n");
		
		#ifndef defined(CC_MT5890)
		dwReg = MGC_PHY_Read32(pBase,M_REG_U2PHYACR5);
		#else
		dwReg = MGC_PHY_Read32(pBase,0x40); //5890 IC issue, read 0x40 instead of M_REG_U2PHYACR5(0x24)
		#endif
		dwReg &= ~0x1;
		MGC_PHY_Write32(pBase,M_REG_U2PHYACR5,dwReg);
	}
}
#endif
#endif
    pPort->bInsert = TRUE;

    LOG(5, "Confirm connect interrupt, pBase = 0x%X.\n", (uint32_t)pBase);
    // add connect event to queue.
    pPort->bIsHost = TRUE;
    qItem.bCause = MGC_BSR_CAUSE_CONNECT;
    VERIFY(pServices->pfQueueBackgroundItem(pServices->pPrivateData, &qItem));
    UNUSED(pBase);
    UNUSED(wTimer);    
}

/* Check device is attached */
uint8_t MGC_IsDevAttached(uint8_t *pBase)
{
    uint8_t devctl;

    MUSB_ASSERT(pBase);
    /* bConnectorId, bIsSession, bIsHost */
    devctl = MGC_Read8(pBase, MGC_O_HDRC_DEVCTL);

    if (devctl & (MGC_M_DEVCTL_FSDEV | MGC_M_DEVCTL_LSDEV))
    {
        return TRUE;
    }

    return FALSE;
}

static uint8_t MGC_IsNoiseStillSof(uint8_t *pBase)
{
    uint32_t reg;

    MUSB_ASSERT(pBase);
#ifdef UNIFIED_USB
    reg = MGC_Read8(pBase,M_REG_PERFORMANCE3);
    return (uint8_t)(reg & 0x80);/* Check if Host set SOF_FORCE bit */
#else
    reg = MGC_MISC_Read32(pBase, 0xE0);
    reg >>= 12;   /* Check if Host set SOF_FORCE bit */

    return (uint8_t)(reg & 1);
#endif	
}

/*
* HDRC Interrupt Service Routine
*/
#ifdef USB_WIFI_DEBUG
UINT32 _u4UsbIsrCnt = 0;
#endif
//extern BOOL _fgUsbSupportCentiflyHub;

#if defined (CONFIG_ARCH_MT85XX)
static void mem_set(char* str, unsigned int len, int val)
{
	unsigned int i;
	for(i=0; i< len; i++){
		str[i] = val;
	}
}
#endif
int32_t MGC_HdrcIsr(void *pParam)
{
    int32_t result;

    uint8_t bIntrUsbValue;
    uint16_t wIntrTxValue, wIntrRxValue;

#ifdef UNIFIED_USB
#if USB_SUSPEND_TEST
    uint32_t u4IntrL1ints, u4Reg;
#elif defined(MUSB_BC12)
	uint32_t u4Reg;
#endif
#endif
    #ifdef MUSB_POWENDOWN
    uint32_t dwPoweDown;
    #endif
    MUSB_Controller *pController;
    MGC_Controller *pControllerImpl;
    MGC_Port *pPort;
    uint8_t *pBase;
    /* save index */
    uint8_t bIndex;
    uint32_t dwLineState;
    uint8_t bRet;
#if defined (CONFIG_ARCH_MT85XX)
    MUSB_UcosController *pControllerInfo = NULL;
    uint32_t u4IrqVector = 8;
#endif
    MUSB_ASSERT(pParam);
    pController = (MUSB_Controller *)pParam;
    pControllerImpl = (MGC_Controller *)pController->pPrivateData;
    MUSB_ASSERT(pControllerImpl);
    pPort = pControllerImpl->pPort;
    MUSB_ASSERT(pPort);
    pBase = (uint8_t *)pControllerImpl->pControllerAddressIst;
    MUSB_ASSERT(pBase);
    /* save index */
    bIndex = MGC_Read8(pBase, MGC_O_HDRC_INDEX);  
#ifdef MUSB_POWERDOWN
	if((MGC_POWERDWN_CASE) &(MGC_INT_SHOW_ENABLE))
	{
		Printf("====Device Detect====\n");
		MGC_INT_SHOW_ENABLE = 0;
		MGC_WAKEUP_CASE = MGC_POWERDWN_CASE;
	}
#endif

#if !defined (CONFIG_ARCH_MT85XX)
    /* read registers */
    bIntrUsbValue = MGC_Read8(pBase, MGC_O_HDRC_INTRUSB);
    wIntrTxValue = MGC_Read16(pBase, MGC_O_HDRC_INTRTX);
    wIntrRxValue = MGC_Read16(pBase, MGC_O_HDRC_INTRRX);

    //  write clear interrupt register value.
    MGC_Write8(pBase, MGC_O_HDRC_INTRUSB, bIntrUsbValue);
    MGC_Write16(pBase, MGC_O_HDRC_INTRTX, wIntrTxValue);
    MGC_Write16(pBase, MGC_O_HDRC_INTRRX, wIntrRxValue);

    //  Only handle enabled interrupt.
    bIntrUsbValue &= MGC_Read8(pBase, MGC_O_HDRC_INTRUSBE);
    wIntrTxValue &= MGC_Read16(pBase, MGC_O_HDRC_INTRTXE);
    wIntrRxValue &= MGC_Read16(pBase, MGC_O_HDRC_INTRRXE);
#else //#if !defined (CONFIG_ARCH_MT85XX)
    /* read registers */
    bIntrUsbValue = MGC_Read8(pBase, MGC_O_HDRC_INTRUSB);
    wIntrTxValue = MGC_Read16(pBase, MGC_O_HDRC_INTRTX);
    wIntrRxValue = MGC_Read16(pBase, MGC_O_HDRC_INTRRX);
	#ifdef UNIFIED_USB
    #if USB_SUSPEND_TEST
        u4IntrL1ints = MGC_Read32(pBase,M_REG_INTRLEVEL1);
		LOG(6,"level1 0x%08x 0x%x 0x%x 0x%x 0x%x \n", u4IntrL1ints,
                                    bIntrUsbValue,wIntrTxValue,wIntrRxValue,
                            		MGC_Read8(pBase, 0x200));
    #endif
	#endif	

    //  write clear interrupt register value.
    MGC_Write8(pBase, MGC_O_HDRC_INTRUSB, bIntrUsbValue);
    MGC_Write16(pBase, MGC_O_HDRC_INTRTX, wIntrTxValue);
    MGC_Write16(pBase, MGC_O_HDRC_INTRRX, wIntrRxValue);
	#ifdef UNIFIED_USB
    #if USB_SUSPEND_TEST
        //MGC_Write32(pBase, M_REG_INTRLEVEL1, u4IntrL1ints);    
    #endif
	#endif

    //  Only handle enabled interrupt.
    bIntrUsbValue &= MGC_Read8(pBase, MGC_O_HDRC_INTRUSBE);
    wIntrTxValue &= MGC_Read16(pBase, MGC_O_HDRC_INTRTXE);
    wIntrRxValue &= MGC_Read16(pBase, MGC_O_HDRC_INTRRXE);
	#ifdef UNIFIED_USB
    #if USB_SUSPEND_TEST
        u4IntrL1ints &= MGC_Read32(pBase, M_REG_INTRLEVEL1EN);    
    #endif
	#endif

	#ifdef UNIFIED_USB
	#if USB_SUSPEND_TEST
    if(MGC_M_L1INTS_DPDM_INT & u4IntrL1ints)
    {
        u4Reg = MGC_Read32(pBase,(uint32_t)M_REG_INTRLEVEL1EN);
        u4Reg &= ~((uint32_t)0x80);
        MGC_Write32(pBase, M_REG_INTRLEVEL1EN, u4Reg);
    }
	#endif
	#endif

    //LOG(0, "BIM_IRQ_STATS: 0x%08X\n", *(unsigned int *)(0xF0008030));
    //Clear system interrupt status.
    for(bIndex = 0; bIndex < MUSB_MAX_CONTROLLERS; bIndex++)
    {
        pControllerInfo = &(MUSB_aUcosController[bIndex]);
        if(pBase == pControllerInfo->pBase)
        {
            u4IrqVector = pControllerInfo->dwInfo;
            BIM_ClearIrq(u4IrqVector);
            break;
        }
    }

    //LOG(0, "BIM_IRQ_STATS: 0x%08X\n", *(unsigned int *)(0xF0008030));

	#ifdef UNIFIED_USB
	#if USB_SUSPEND_TEST
    if(MGC_M_L1INTS_DPDM_INT & u4IntrL1ints)
    {       
        LOG(0, "[USB INTR] DPDM_INT\n");
        u4Reg = MGC_PHY_Read32(pBase,(uint32_t)0x68);
        u4Reg &=  ~0x00040000; 
        MGC_PHY_Write32(pBase, (uint32_t)0x68, u4Reg);

        if( bIntrUsbValue || wIntrTxValue || wIntrRxValue)
        {
            LOG(0, "Resume PHY\n");       
            LOG(0, "bIntrUsbValue: 0x%08X\n", bIntrUsbValue);       
            LOG(0, "wIntrTxValue : 0x%08X\n", wIntrTxValue);       
            LOG(0, "wIntrRxValue : 0x%08X\n", wIntrRxValue);       
        }
        else
        {
            LOG(0, "Resume PHY ONLY\n");       
            return 0;
        }
    }
	#endif
	#endif

	#ifdef USB_WIFI_DEBUG
	_u4UsbIsrCnt++;
	if(wIntrRxValue == 0x0004)
	{
	    LOG(5, "Stop here!");
	}
	#endif

#endif // #if !defined (CONFIG_ARCH_MT85XX)

    if(bIntrUsbValue)
    {
     LOG(5, "usb interrupt = 0x%X, pBase = 0x%X.\n", bIntrUsbValue, (uint32_t)pBase);   
    }
    // handle babble interrupt.   
    if ((pPort->bIsHost) && (bIntrUsbValue & MGC_M_INTR_BABBLE))
    {
        /* If device is still attached, we ignore the babble interrupt
           when NOISE_STIFF_SOF is enabled, which is used to avoid host 
           controller stop issue SOF during ESD attack.
         */
        LOG(0, "Babble interrupt = 0x%X, pBase = 0x%X.\n", bIntrUsbValue, (uint32_t)pBase);   
        if (MGC_IsDevAttached(pBase))
        {
            LOG(0, "Device is still attached.\n", NULL);

            if (MGC_IsNoiseStillSof(pBase))
            {
                LOG(0, "SOF is still transmit during babble.\n", NULL);
                // Ignore this babble.
                bIntrUsbValue &= ~MGC_M_INTR_BABBLE;
            }

            // turn off babble interrupt.
            bRet = MGC_Read8(pBase, MGC_O_HDRC_INTRUSBE);
            bRet &= ~(MGC_M_INTR_BABBLE);
            MGC_Write8(pBase, MGC_O_HDRC_INTRUSBE, bRet);
        }
    }

    //  read PHY line state to confirm connection status.
    if ((bIntrUsbValue & (MGC_M_INTR_CONNECT | MGC_M_INTR_DISCONNECT)) ==
         (uint8_t)(MGC_M_INTR_CONNECT | MGC_M_INTR_DISCONNECT))
    {   
        LOG(0, "USB unstable interrupt: 0x%X, pBase=0x%X.\n", bIntrUsbValue, (uint32_t)pBase);
        // Do not handle this interrupt.
        bIntrUsbValue &= ~(MGC_M_INTR_CONNECT | MGC_M_INTR_DISCONNECT);
#ifdef MUSB_LOADER
        pPort->bInsert = TRUE;
#endif        
    }

    if (bIntrUsbValue & MGC_M_INTR_CONNECT)
    {
#if !defined (CONFIG_ARCH_MT85XX)
	 	dwLineState = MGC_PHY_Read32(pBase, M_REG_U2PHYDMON1);
#else
        #ifdef UNIFIED_USB
            dwLineState = MGC_Read32(pBase,M_REG_DEBUG_PROBE);
            //LOG(0,"EMULATION: software set ok linestate directly\n");
        #else
            dwLineState = MGC_PHY_Read32(pBase, M_REG_PHYC5);
		#endif
#endif

        dwLineState &= M_REG_LINESTATE_MASK;

        LOG(0, "Connect interrupt = 0x%X, pBase = 0x%X.\n", bIntrUsbValue, (uint32_t)pBase);
       
        //kill timer.
        bRet = pControllerImpl->pSystemServices->pfCancelTimer(
            pControllerImpl->pSystemServices->pPrivateData, 0);
        UNUSED(bRet);
        
        if (dwLineState != LINESTATE_DISCONNECT)
        {
#ifdef MUSB_LOADER
            pPort->bInsert = TRUE;
#endif        

            MGC_dwCheckInsert = 0;

            //Timer Index 0 is reserved for EP0 device configuration. 
            bRet = pControllerImpl->pSystemServices->pfArmTimer(
                pControllerImpl->pSystemServices->pPrivateData, 
                MUSB_DEV_CONFIG_TIMER_IDX /*bTimerIndex*/, 
                MGC_CHECK_INSERT_DEBOUNCE, FALSE,
                MGC_HdrcCheckConnect, pControllerImpl);
            UNUSED(bRet);    
        }
	#ifdef TZ_TEST		
		if(enableTzProtectTest){
			if(!tzProtectZone0){
				tzProtectZone0 = (char*)kmalloc(TZ_PROTECTZONG_LEN, GFP_KERNEL);
				tzProtectZone1 = (char*)kmalloc(TZ_PROTECTZONG_LEN, GFP_KERNEL);
				tzProtectZone2 = (char*)kmalloc(TZ_PROTECTZONG_LEN, GFP_KERNEL);
				mem_set(tzProtectZone0, TZ_PROTECTZONG_LEN, 1);
				mem_set(tzProtectZone1, TZ_PROTECTZONG_LEN, 1);
				mem_set(tzProtectZone2, TZ_PROTECTZONG_LEN, 1);
				TZ_ProtEnable(TZ_USB0_ID, virt_to_phys(tzProtectZone0), virt_to_phys(tzProtectZone0+TZ_PROTECTZONG_LEN-1) , &tzUsb0Permit);
				#if 0
				TZ_ProtEnable(TZ_USB1_ID, virt_to_phys(tzProtectZone1), virt_to_phys(tzProtectZone1+TZ_PROTECTZONG_LEN-1) , &tzUsb1Permit);
				TZ_ProtEnable(TZ_USB2_ID, virt_to_phys(tzProtectZone2), virt_to_phys(tzProtectZone2+TZ_PROTECTZONG_LEN-1) , &tzUsb2Permit);
				#endif
			}
		}else{
			if(tzProtectZone0){
				TZ_ProtDisable(TZ_USB0_ID);
				#if 0
				TZ_ProtDisable(TZ_USB1_ID);
				TZ_ProtDisable(TZ_USB2_ID);
				#endif
				kfree(tzProtectZone0);
				kfree(tzProtectZone1);
				kfree(tzProtectZone2);
			}
		}
	#endif
        // When connect and disconnet happen at the same time,
        // we only handle connect event. This will happen at Vbus not stable when cable is  inserted.
        bIntrUsbValue &= ~(MGC_M_INTR_CONNECT);
    }
    else if (bIntrUsbValue & MGC_M_INTR_DISCONNECT) // disconnect event.   
    {
        if (!pPort->bWantReset)
        {
            LOG(0, "Disconnect interrupt = 0x%X, pBase = 0x%X.\n", bIntrUsbValue, (uint32_t)pBase);
            //kill timer.
            bRet = pControllerImpl->pSystemServices->pfCancelTimer(
                pControllerImpl->pSystemServices->pPrivateData, 0);

            if (!pPort->bInsert)
            {
                // connect interrupt is not confirmed, ignore disconnect interrupt.
                bIntrUsbValue &= ~(MGC_M_INTR_DISCONNECT);
            }
            else
            {
                pPort->bInsert = FALSE;
            }
			
			#ifdef MUSB_BC12
			#if defined(CC_MT5881) || defined(CC_MT5890) || defined(CC_MT5882)
			if(IS_SupportBC12())
			{
				#if defined(CC_MT5881)
				if(MUSB1_BASE == pBase)
				#elif defined(CC_MT5890) || defined(CC_MT5882)
				if(MUSB_BASE == pBase)
				#endif
				{
					uint8_t power = MGC_Read8(pBase,MGC_O_HDRC_POWER);
					uint8_t nPower= power ;
			
					LOG(0, "[USB]Re-Pull up RG_USB20_BGR_EN RG_USB20_CHP_EN on port1.\n");
					u4Reg = MGC_PHY_Read32(pBase, M_REG_PHYC0);
					u4Reg |= 0x3;
					MGC_PHY_Write32(pBase, M_REG_PHYC0, u4Reg);
					LOG(0, "[USB]Re-Enable BC12 function on port1 when init.\n");
					LOG(0, "[USB]Reset port\n");
			
					nPower |= MGC_POWER_RESET ;
					if(nPower != power)
					{
						MGC_Write8(pBase,MGC_O_HDRC_POWER,nPower);
					}
			
					//pull up RG_CHGDT_EN
					#ifndef defined(CC_MT5890)
					u4Reg = MGC_PHY_Read32(pBase,M_REG_U2PHYACR5);
					#else
					u4Reg = MGC_PHY_Read32(pBase,0x40); //5890 IC issue, read 0x40 instead of M_REG_U2PHYACR5(0x24)
					#endif
					u4Reg |= 0x1;
					MGC_PHY_Write32(pBase,M_REG_U2PHYACR5,u4Reg);
				}
			}
			#endif
			#endif
        }
	#ifdef TZ_TEST
		if(enableTzProtectTest){
			unsigned int i;
			unsigned int is_protected;
			if(tzProtectZone0){
				is_protected = 1;
				for(i=0; i< TZ_PROTECTZONG_LEN-1; i++){
					if(tzProtectZone0[i] != 1){
						Printf("[USB]i num:%d\n", i);
						is_protected = 0;
						break;
					}
				}
				if(is_protected){
					Printf("[USB]TZ0 is proteced.\n");
				}else{
					Printf("[USB]TZ0 not proteced.\n");
				}
			}
			if(tzProtectZone1){
				is_protected = 1;
				for(i=0; i< TZ_PROTECTZONG_LEN-1; i++){
					if(tzProtectZone1[i] != 1){
						Printf("[USB]i num:%d\n", i);
						is_protected = 0;
						break;
					}
				}
				if(is_protected){
					Printf("[USB]TZ1 is proteced.\n");
				}else{
					Printf("[USB]TZ1 not proteced.\n");
				}
			}
			if(tzProtectZone2){
				is_protected = 1;
				for(i=0; i< TZ_PROTECTZONG_LEN-1; i++){
					if(tzProtectZone2[i] != 1){
						Printf("[USB]i num:%d\n", i);
						is_protected = 0;
						break;
					}
				}
				if(is_protected){
					Printf("[USB]TZ2 is proteced.\n");
				}else{
					Printf("[USB]TZ2 not proteced.\n");
				}
			}
		}
	#endif
    }


    /* call common ISR */
    result = MGC_DrcIsr(pControllerImpl, bIntrUsbValue, wIntrTxValue, wIntrRxValue);

    /* restore index */
    MGC_Write8(pBase, MGC_O_HDRC_INDEX, bIndex);

    return result;
}

/*
* Program the HDRC to start (enable interrupts, etc.).
*/
uint32_t MGC_HdrcStart(MGC_Controller *pController)
{
    uint32_t val;
    uint8_t val2;
    uint8_t *pBase;
    MGC_Port *pPort;

    MUSB_ASSERT(pController);    
    pBase = (uint8_t *)pController->pControllerAddressIsr;
    MUSB_ASSERT(pBase);    
    pPort = pController->pPort;
    MUSB_ASSERT(pPort);    

    /*  Set INT enable registers */
    val = (uint32_t)((0xffff & pPort->wEndMask) << 16);
    MGC_DIAG1(3, pController, "Setting IntrTxE to ", val, 16, 4);
    MGC_Write32(pBase, MGC_O_HDRC_INTRRX, val);
    val =(uint32_t)(0xfffe & pPort->wEndMask);
    MGC_DIAG1(3, pController, "Setting IntrRxE to ", val, 16, 4);
    //MGC_Write16(pBase, MGC_O_HDRC_INTRRXE, val);
#ifdef MUSB_SCHEDULER
    /* TODO: enable SOF interrupts only when needed (maybe in ProgramBusState when the schedule is non-empty) */

    val2 = 0xff;
#else

    val2 = (uint8_t)~MGC_M_INTR_SOF;
#endif

    MGC_DIAG1(3, pController, "Setting IntrUsbE to ", (val2 & 0xff), 16, 2);
    //MGC_Write8(pBase, MGC_O_HDRC_INTRUSBE, (val & 0xff));
    val |= (((uint32_t)val2) << 24);
    MGC_Write32(pBase, MGC_O_HDRC_INTRRXE, val);

#ifdef UNIFIED_USB
   val = MGC_Read32(pBase, M_REG_INTRLEVEL1EN);

	#if defined (CONFIG_ARCH_MT85XX)
   val |= 0x0f; 
	#else
	val |= 0x0f;
	#endif

   MGC_Write32(pBase, M_REG_INTRLEVEL1EN, val);
   LOG(3,"Setting level1En to 0x%08x\n",val);
#endif	

    /* TODO: always set ISOUPDATE in POWER (periph mode) and leave it on! */
    pPort->bOtgState = (uint8_t)MUSB_AB_IDLE;

    return 0;
}

/*
* Program the HDRC to stop (disable interrupts, etc.).
*/
uint32_t MGC_HdrcStop(MGC_Controller *pController)
{
    uint16_t temp;
    uint8_t *pBase;
    MGC_Port *pPort;
    MUSB_Port* pUsbPort;
#ifdef UNIFIED_USB
    uint32_t u4Reg;
#endif

#ifndef MUSB_DISABLE_DB_DYNFIFO            
    uint8_t bEnd;
    uint32_t dwFifoAddr = MGC_END0_FIFOSIZE;
#endif    
    MUSB_ASSERT(pController);    
    pBase = (uint8_t *)pController->pControllerAddressIsr;
    MUSB_ASSERT(pBase);    
    pPort = pController->pPort;
    MUSB_ASSERT(pPort);
    pUsbPort = pPort->pInterfacePort;
    MUSB_ASSERT(pUsbPort);

#ifdef UNIFIED_USB
    //  reset Vbus.
    #if !(USB_SUSPEND_TEST)
    u4Reg = MGC_PHY_Read32(pBase,(uint32_t)0x6C);
    u4Reg &= ~0x3f3f;
    u4Reg |=  0x3e10;
    MGC_PHY_Write32(pBase, (uint32_t)0x6C, u4Reg);   	 
    #endif
#else
    //  reset Vbus.
    MGC_PHY_Write32(pBase, M_REG_PHYC5, 0xF04);            
#endif

#ifdef CONFIG_DRV_LINUX
    mdelay(10);
#else
    (void)MUSB_Sleep(10);
#endif
    
    if (pUsbPort->Type == MUSB_PORT_TYPE_HOST)
    {    
        #ifdef UNIFIED_USB
        //how to reset hw?
        u4Reg = MGC_PHY_Read32(pBase,(uint32_t)0x6C);
        u4Reg &= ~0x3f3f;
        u4Reg |=  0x3e2c;
        MGC_PHY_Write32(pBase, (uint32_t)0x6C, u4Reg); 
        #else
            MGC_PHY_Write32(pBase, M_REG_PHYC5, 0x0F03);  
        #endif

#ifndef MTK_MHDRC
        // turn on device address control at host mode.        
        MGC_MISC_Write32(pBase, M_REG_ACCESSUNIT, 0x12);
#endif 

#ifndef MUSB_DISABLE_DB_DYNFIFO         
        /*
            Host mode use single packet buffer.
        */
        if (MGC_bDBFifoEnable)
        {
            MGC_bDBFifoEnable = FALSE;

#if 0       // Double Packet Buffer
            // Set fifo size = 512 bytes per endpoint.             
            // TX and RX are shared the same fifo.
            #ifdef UNIFIED_USB
                for (bEnd = 1; bEnd < (uint8_t)(pUsbPort->bUsbPort & 0xff); bEnd++)
            #else
                for (bEnd = 1; bEnd < (uint8_t)(MUSB_C_NUM_EPS & 0xff); bEnd++)
            #endif	
            {
                MGC_SelectEnd(pBase, bEnd);
                MGC_Write8(pBase, MGC_O_HDRC_RXFIFOSZ, 0x16);
                MGC_Write8(pBase, MGC_O_HDRC_TXFIFOSZ, 0x16);
                MGC_Write16(pBase, MGC_O_HDRC_RXFIFOADD, dwFifoAddr/8);
                MGC_Write16(pBase, MGC_O_HDRC_TXFIFOADD, dwFifoAddr/8);
                dwFifoAddr += 1024;
            }
#else       // Single Packet Buffer
            // Set fifo size = 512 bytes per endpoint.             
            // TX and RX are not shared the same fifo.
            #ifdef UNIFIED_USB
                for (bEnd = 1; bEnd < (uint8_t)(pUsbPort->bUsbPort & 0xff); bEnd++)
            #else
                for (bEnd = 1; bEnd < (uint8_t)(MUSB_C_NUM_EPS & 0xff); bEnd++)
            #endif	
            {
                MGC_SelectEnd(pBase, bEnd);
                MGC_Write8(pBase, MGC_O_HDRC_RXFIFOSZ, 0x6);
                MGC_Write8(pBase, MGC_O_HDRC_TXFIFOSZ, 0x6);
                MGC_Write16(pBase, MGC_O_HDRC_RXFIFOADD, dwFifoAddr/8);
                dwFifoAddr += 512;                
                MGC_Write16(pBase, MGC_O_HDRC_TXFIFOADD, dwFifoAddr/8);
                dwFifoAddr += 512;
            }
#endif
        }
#endif        
    }
    else
    {
        #ifdef UNIFIED_USB
        //how to reset hw?
        LOG(0,"should set tm1 for device mode ?\n");
        u4Reg = MGC_PHY_Read32(pBase,(uint32_t)0x6C);
        u4Reg &= ~0x3f3f;
        u4Reg |=  0x3e2e;
        MGC_PHY_Write32(pBase, (uint32_t)0x6C, u4Reg); 
        #else
        MGC_PHY_Write32(pBase, M_REG_PHYC5, 0x0F0B);
        #endif
#ifndef MTK_MHDRC
        // turn off device address control at device mode.
        MGC_MISC_Write32(pBase, M_REG_ACCESSUNIT, 0x2);
#endif        

#ifndef MUSB_DISABLE_DB_DYNFIFO         
        /*
            Device mode use double packet buffer to get better performance.
        */
        if (!MGC_bDBFifoEnable)
        {
            MGC_bDBFifoEnable = TRUE;

#if 1       // Double Packet Buffer
            /* Notice: double packet buffer fifo need to check fifo empty */  
            // Set fifo size = 512 bytes and open double-buffering per endpoint.
            // TX and RX are shared the same fifo.
            #ifdef UNIFIED_USB
            for (bEnd = 1; bEnd < (uint8_t)(pUsbPort->bUsbPort & 0xff); bEnd++)
            #else
            for (bEnd = 1; bEnd < (uint8_t)(MUSB_C_NUM_EPS & 0xff); bEnd++)
		    #endif	
            {
                MGC_SelectEnd(pBase, bEnd);
                MGC_Write8(pBase, MGC_O_HDRC_RXFIFOSZ, 0x16);
                MGC_Write8(pBase, MGC_O_HDRC_TXFIFOSZ, 0x16);
                MGC_Write16(pBase, MGC_O_HDRC_RXFIFOADD, dwFifoAddr/8);
                MGC_Write16(pBase, MGC_O_HDRC_TXFIFOADD, dwFifoAddr/8);
                dwFifoAddr += 1024;
            }
#else       // Single Packet Buffer
            // Set fifo size = 512 bytes per endpoint.             
            // TX and RX are not shared the same fifo.
            for (bEnd = 1; bEnd < (uint8_t)(pUsbPort->bUsbPort & 0xff); bEnd++)
            {
                MGC_SelectEnd(pBase, bEnd);
                MGC_Write8(pBase, MGC_O_HDRC_RXFIFOSZ, 0x6);
                MGC_Write8(pBase, MGC_O_HDRC_TXFIFOSZ, 0x6);
                MGC_Write16(pBase, MGC_O_HDRC_RXFIFOADD, dwFifoAddr/8);
                dwFifoAddr += 512;                
                MGC_Write16(pBase, MGC_O_HDRC_TXFIFOADD, dwFifoAddr/8);
                dwFifoAddr += 512;                
            }
#endif
        }
#endif        
    }

    /* disable interrupts */
    MGC_Write8(pBase, MGC_O_HDRC_INTRUSBE, 0);
    MGC_Write16(pBase, MGC_O_HDRC_INTRTXE, 0);
    MGC_Write16(pBase, MGC_O_HDRC_INTRRXE, 0);
#ifdef UNIFIED_USB
    u4Reg = MGC_Read32(pBase, M_REG_INTRLEVEL1EN);
	#if defined (CONFIG_ARCH_MT85XX)
   u4Reg &= ~((uint32_t)0x0f); 
	#else
    u4Reg &= ~((uint32_t)0x0f);
	#endif
    MGC_Write32(pBase, M_REG_INTRLEVEL1EN, u4Reg);
#endif

    /*  flush pending interrupts */
    temp = MGC_Read8(pBase, MGC_O_HDRC_INTRUSB);
    temp = MGC_Read16(pBase, MGC_O_HDRC_INTRTX);
    temp = MGC_Read16(pBase, MGC_O_HDRC_INTRRX);

    UNUSED(temp);
    return 0;
}
#ifndef MUSB_CODE_SHRINK 
uint32_t MGC_HdrcDestroy(MGC_Controller *pController)
{
    MGC_Port *pPort;

    MUSB_ASSERT(pController);    
    pPort = pController->pPort;
    MUSB_ASSERT(pPort);    

    VERIFY(MGC_HostDestroy(pPort));

    if (pPort->pPrivateData)
    {
        MUSB_MemFree(pPort->pPrivateData);
    }
#ifdef MUSB_FUNCTION

    return MGC_FunctionDestroy(pPort);
#else

    return 0;
#endif

}
#endif
/* Fill the bus state info from the controller */
uint32_t MGC_HdrcReadBusState(MGC_Port *pPort)
{
    uint8_t devctl;
    uint8_t power;
#ifdef MUSB_OTG_FUNC      
    uint8_t vbus;
#endif
    MGC_Controller *pController;
    uint8_t *pBase;
    MUSB_Port *pUsbPort = NULL;

    MUSB_ASSERT(pPort);
    pController = pPort->pController;
    MUSB_ASSERT(pController);
    pBase = (uint8_t *)pController->pControllerAddressIst;
    MUSB_ASSERT(pBase);
    pUsbPort = pPort->pInterfacePort;
    MUSB_ASSERT(pUsbPort);


    /* bConnectorId, bIsSession, bIsHost */
    devctl = MGC_Read8(pBase, MGC_O_HDRC_DEVCTL);

#ifdef MUSB_OTG_FUNC
    if (devctl & MGC_M_DEVCTL_SESSION)
    {
        pPort->bConnectorId = (devctl & MGC_M_DEVCTL_BDEVICE) >> 7;
    }
    else
    {
        pPort->bConnectorId = MGC_CID_UNKNOWN;
    }
#endif

    pPort->bIsSession = (devctl & MGC_M_DEVCTL_SESSION) ? TRUE : FALSE;

    //pPort->bIsHost = (devctl & MGC_M_DEVCTL_HM) ? TRUE : FALSE;
    pPort->bIsHost = (pUsbPort->Type == MUSB_PORT_TYPE_HOST) ? TRUE: FALSE;    

    pPort->bIsLowSpeed = (devctl & MGC_M_DEVCTL_LSDEV) ? TRUE : FALSE;

    /* bIsHighSpeed, bIsSuspend, bIsReset, bIsResume, bIsSession, bBusVoltage */
    power = MGC_Read8(pBase, MGC_O_HDRC_POWER);
    pPort->bIsHighSpeed = (power & MGC_M_POWER_HSMODE) ? TRUE : FALSE;
    pPort->bIsSuspend = (power & MGC_M_POWER_SUSPENDM) ? TRUE : FALSE;
    pPort->bIsReset = (power & MGC_M_POWER_RESET) ? TRUE : FALSE;
    pPort->bIsResume = (power & MGC_M_POWER_RESUME) ? TRUE : FALSE;

#ifdef MUSB_OTG_FUNC   
    vbus = (devctl & MGC_VBUS_MASK) >> 3;

    switch (vbus)
    {
    case 0:
        pPort->bBusVoltage = MGC_VBUS_BELOW_SESSION_END;
        break;

    case 1:
        pPort->bBusVoltage = MGC_VBUS_ABOVE_SESSION_END;
        break;

    case 2:
        pPort->bBusVoltage = MGC_VBUS_ABOVE_AVALID;
        break;

    case 3:
        pPort->bBusVoltage = MGC_VBUS_ABOVE_VBUS_VALID;
        break;

    default:
        pPort->bBusVoltage = MGC_VBUS_ERROR;
    }
#endif
    /* dwFrame */
    /* if some one is using this, then have protection for updating it */
#if defined(MUSB_ISO) || defined(MUSB_SCHEDULER)
    //pPort->dwFrame = MGC_Read16(pBase, MGC_O_HDRC_FRAME);
#endif
    return 0;
}

/* Program the bus state from the port (see MGC_Port) */
void MGC_HdrcProgramBusState(MGC_Port *pPort)
{
    MGC_Controller *pController;
    uint8_t *pBase;
    uint8_t power;
    uint8_t nPower;
    uint8_t devctl;
    uint8_t nDevCtl;
    uint8_t bFuncAddr;
#if defined(CC_MT8580)||defined(CC_MT8563)
    uint32_t dwPhydtm0;
#endif
#ifdef MUSB_EHSET
    uint8_t bRet;
#endif
    
    MUSB_ASSERT(pPort);
    pController = pPort->pController;
    MUSB_ASSERT(pController);
    pBase = (uint8_t *)pController->pControllerAddressIst;
    MUSB_ASSERT(pBase);

    power = MGC_Read8(pBase, MGC_O_HDRC_POWER);
    nPower = power;
    devctl = MGC_Read8(pBase, MGC_O_HDRC_DEVCTL);
    nDevCtl = devctl;
    bFuncAddr = MGC_Read8(pBase, MGC_O_HDRC_FADDR);

    /* reset? */
    if (pPort->bWantReset)
    {
        nPower |= MGC_M_POWER_RESET;
        //  MTK Notice: Max Liao, 2006/10/04.
        //  address must = 0 at reset.
        if (pPort->bFuncAddr)
        {
            pPort->bFuncAddr = 0;
        }
    }
    else
    {
        nPower &= ~MGC_M_POWER_RESET;
    }

    /* resume? */
    if (pPort->bWantResume)
    {
        nPower |= MGC_M_POWER_RESUME;
#ifdef MUSB_EHSET
        bRet = pController->pSystemServices->pfArmTimer(
            pController->pSystemServices->pPrivateData, 0, 10, 
            FALSE, MGC_HdrcResumeOff, pController);     
        UNUSED(bRet);            
#endif        
    }
    else
    {
        nPower &= ~MGC_M_POWER_RESUME;
    }

    /* suspend? */
    if (pPort->bWantSuspend && MGC_bWantSuspend)
    {   
    #if defined(CC_MT8580) ||defined(CC_MT8563)
		//Printf("Base is 0x%08x\n",(unsigned int)pBase);
        if ((uint32_t)pBase == MUSB2_BASE)
	    {
            dwPhydtm0 = MGC_PHY_Read32(pBase,(uint32_t)0x68);
			//Printf("before Phydtm0 is 0x%08x\n",dwPhydtm0);
			dwPhydtm0 |= 0x00040008;
			//Printf("after Phydtm0 is 0x%08x\n",dwPhydtm0);
			MGC_PHY_Write32(pBase,(uint32_t)0x68,dwPhydtm0);
		}
	#endif
        if (pPort->bIsHost)
        {
            nPower |= MGC_M_POWER_SUSPENDM;
        }

        // Set UTMI+PHY to low power mode and stop XCLK.
        nPower |= MGC_M_POWER_ENSUSPEND;
    }
    else
    {
        nPower &= ~MGC_M_POWER_SUSPENDM;
        nPower &= ~MGC_M_POWER_ENSUSPEND;
    }

    /* high-speed? */
    if (pPort->bWantHighSpeed)
    {
        nPower |= MGC_M_POWER_HSENAB;
    }
    else
    {
        nPower &= ~MGC_M_POWER_HSENAB;
    }

    /* session? */
    if (pPort->bWantSession)
    {
        LOG(5, "bWantSession\n");
        nDevCtl |= MGC_M_DEVCTL_SESSION;
    }
    else
    {
        LOG(5, "!bWantSession\n");
        nDevCtl &= ~MGC_M_DEVCTL_SESSION;
    }

#ifdef MUSB_OTG_FUNC
    /* host? */
    if (pPort->bWantHost)
    {
        nDevCtl |= MGC_M_DEVCTL_HR;
    }
    else
    {
        nDevCtl &= ~MGC_M_DEVCTL_HR;
    }
#endif

#ifdef MTK_MHDRC    
    if ((!pPort->bIsHost) && (pPort->bFuncAddr != bFuncAddr))
#else
    if (pPort->bFuncAddr != bFuncAddr)
#endif
    {
        MGC_DIAG1(3, pController, "Updating FADDR to ", pPort->bFuncAddr, 10, 0);

        MGC_Write8(pBase, MGC_O_HDRC_FADDR, pPort->bFuncAddr);

        //  MTK Notice: Max Liao, 2006/11/28.
        //  pPort->bFuncAddr as ep0 address for supporting Hub.
        MGC_Write32(pBase, M_REG_EP0ADDR, pPort->bFuncAddr);
    }    

    if (nPower != power)
    {
        MGC_DIAG1(3, pController, "Updating POWER to ", nPower, 16, 2);
        MGC_Write8(pBase, MGC_O_HDRC_POWER, nPower);
    }

    if (nDevCtl != devctl)
    {
        MGC_DIAG1(3, pController, "Updating DevCtl to ", nDevCtl, 16, 2);
        LOG(5, "Updating DevCtl to 0x%02X\n", nDevCtl);
        MGC_Write8(pBase, MGC_O_HDRC_DEVCTL, nDevCtl);
        nDevCtl = MGC_Read8(pBase, MGC_O_HDRC_DEVCTL);
        LOG(5, "DevCtl = 0x%02X\n", nDevCtl);        
    }
}

/*
* Find the HDRC's first (host) or required (function) 
* local endpoint resource to communicate with the given remote endpoint (host)
* or to be identified with the given endpoint descriptor (function).
*/
MGC_EndpointResource *MGC_HdrcBindEndpoint(MGC_Port *pPort, const MUSB_DeviceEndpoint *pUsbEnd,
                                           MUSB_EndpointResource *pRequest, uint8_t bBind)
{
    uint8_t bEnd, bIsTx, bTrafficType;
#ifdef CC_MT5881
    uint8_t bIsIn;
#endif
    uint8_t bInterval;
    MGC_EndpointResource *pEnd;
    uint8_t reg = 0;
    uint16_t csr = 0;
    MGC_Controller *pController;
    uint8_t *pBase;
    MUSB_SystemServices* pServices;    
    uint8_t bStatus = 0;
	
#if !defined(MUSB_LOADER)
    CRIT_STATE_T rState = 0;
#endif

    MUSB_ASSERT(pPort);
    MUSB_ASSERT(pUsbEnd);    
    bInterval = pUsbEnd->UsbDescriptor.bInterval;    
    pController = pPort->pController;
    MUSB_ASSERT(pController);
    pServices = pController->pSystemServices;

#if !defined(MUSB_LOADER)
#if 0
    if (pPort->bIsHost)
    {
        bStatus = pServices->pfIrpLock(pServices->pPrivateData, 0);
    }    
#else
    rState = x_crit_start();
#endif
#endif

    pBase = (uint8_t *)pController->pControllerAddressIst;
    MUSB_ASSERT(pBase);
  
#ifdef MUSB_C_DYNFIFO_DEF
#ifdef CC_MT5881 //MT5881 bind hub to endpoint 2 on port 0
	if(((uint32_t)pBase == MUSB_BASE)&&(pUsbEnd->pDevice->bDeviceClass == 0x09))
	{
		pEnd = (MGC_EndpointResource *)MUSB_ArrayFetch(&(pPort->LocalEnds), 2);
		if(NULL == pEnd)  
		{
			return NULL;
		}
		bIsIn = (pUsbEnd->UsbDescriptor.bEndpointAddress &MUSB_ENDPOINT_DIR_MASK) ? TRUE : FALSE;
		bIsTx = pPort->bIsHost ? !bIsIn : bIsIn;
		pEnd->bRxClaimed = TRUE;
		pEnd->bIsTx = bIsTx;
		pEnd->bIsHalted = FALSE;
		pEnd->bRxBusAddress = pUsbEnd->pDevice->bBusAddress;
		pEnd->bRxBusEnd = pUsbEnd->UsbDescriptor.bEndpointAddress;
		pEnd->bRxInterval = pUsbEnd->UsbDescriptor.bInterval;
		pEnd->bRxTrafficType = pUsbEnd->UsbDescriptor.bmAttributes;
		pEnd->wRxPacketSize = MUSB_SWAP16P((uint8_t *)&(pUsbEnd->UsbDescriptor.wMaxPacketSize));
	}		   
else
#endif
{
    pEnd = MGC_HdrcBindDynamicEndpoint(pPort, pUsbEnd, bBind, &bIsTx);
    UNUSED(pRequest);
}
#else

    pEnd = MGC_DrcBindEndpoint(pPort, pUsbEnd, pRequest, bBind, &bIsTx);
#endif

    if (pEnd && bBind)
    {
        /* prepare endpoint registers according to flags */
        bTrafficType = bIsTx ? pEnd->bTrafficType : pEnd->bRxTrafficType;
        bTrafficType &= MUSB_ENDPOINT_XFERTYPE_MASK;        

        bEnd = pEnd->bLocalEnd;
        MGC_SelectEnd(pBase, bEnd);

        LOG(5, "H/W %s Endpoint = %d, TrafficType=%d.\n", 
            ((bIsTx) ? "Tx": "Rx"), bEnd, bTrafficType);
        
        /* proto reg */
        reg |= bIsTx ? (pEnd->bBusEnd & MUSB_ENDPOINT_NUMBER_MASK)
                   : (pEnd->bRxBusEnd & MUSB_ENDPOINT_NUMBER_MASK);

        if (bEnd)
        {
            reg |= (bTrafficType << 4);
        }

        //  MTK Notice: Max Liao, 2006/11/29.
        //  Set interrupt polling interval must depend on high, or low/full speed.
        if (MUSB_ENDPOINT_XFER_INT == bTrafficType)
        {
            if (pPort->bIsHighSpeed)
            {
                bInterval = MUSB_MIN(bInterval, 6);
            }
            else
            {
                bInterval = MUSB_MIN(bInterval, 32);
            }            
        }
        else if (MUSB_ENDPOINT_XFER_BULK == bTrafficType)
        {
#if !defined (CONFIG_ARCH_MT85XX)
            //  MTK Notice: Max Liao, 2007/07/17.
            //  turn on nak limit when process bulk transfer.
            //20121022 LeonLin when usb-serial catch null buffer, 
            // need about 1.2s to return NAK timeout irq.
            bInterval = (uint8_t)((pPort->bIsHighSpeed) ? 14 : 2);
#else
            if(pUsbEnd->wNakLimit == 1)
            {
                LOG(5, "Set NAK Limit to %d.\n", pUsbEnd->wNakLimit);
                bInterval = 1;
            }
            else if(pUsbEnd->wNakLimit == 2)
            {
                LOG(5, "Set NAK Limit to %d.\n", pUsbEnd->wNakLimit);
                bInterval = 2;
            }
            else if(pUsbEnd->wNakLimit == 5)
            {
                LOG(5, "Set NAK Limit to %d.\n", pUsbEnd->wNakLimit);
                bInterval = 5;
            }
            else
            {
                //  MTK Notice: Max Liao, 2007/07/17.
                //  turn on nak limit when process bulk transfer.
                bInterval = (uint8_t)((pPort->bIsHighSpeed) ? 14 : 11);
            }                
#endif
        }
        else if (MUSB_ENDPOINT_XFER_ISOC== bTrafficType)
        {
            LOG(0, "ISOC Ep#%d, bInterval: %d\n", bEnd, bInterval);
        }
        if (bIsTx)
        {
            /* transmit */
            if (pPort->bIsHost)
            {
                /* protocol/endpoint/interval/NAKlimit */
                if (bEnd)
                {
#ifndef MTK_MHDRC               
                    //  MTK Notice: Max Liao, 2006/11/28.
                    //  set device address in ep basis for supporting Hub.
                    MGC_Write32(pBase, M_REG_EPXADDR(bEnd), pEnd->bBusAddress);
                    MGC_WriteCsr8(pBase, MGC_O_HDRC_TXTYPE, bEnd, reg);
#endif
                    /* 
                     * TODO: at least for bulk, setting the upper 4 interval bits causes the core 
                     * to continuously assert Rx interrupts with RxError set
                     */
                    MGC_WriteCsr8(pBase, MGC_O_HDRC_TXINTERVAL, bEnd, bInterval);
                }
                else
                {
                    MGC_WriteCsr8(pBase, MGC_O_HDRC_NAKLIMIT0, 0, 0x10);
                }
            }

            /* CSR */
            if (bEnd)
            {
                /* normal case: set up for packet size */
                MGC_WriteCsr16(pBase, MGC_O_HDRC_TXMAXP, bEnd, pEnd->wPacketSize);
                #ifdef UNIFIED_USB
                LOG(5,"MGC_M_TXCSR_MODE not exist\n");
                #else
                csr = MGC_M_TXCSR_MODE;
	        #endif
#ifdef MUSB_ISO

                if (!pPort->bIsHost && (pEnd->bTrafficType == MUSB_ENDPOINT_XFER_ISOC))
                {
                    csr |= MGC_M_TXCSR_ISO;
                }
#endif
                /* flush twice in case of double packet buffering */

                MGC_WriteCsr16(pBase, MGC_O_HDRC_TXCSR, bEnd,
                               MGC_M_TXCSR_FLUSHFIFO | MGC_M_TXCSR_CLRDATATOG);
                MGC_WriteCsr16(pBase, MGC_O_HDRC_TXCSR, bEnd,
                               MGC_M_TXCSR_FLUSHFIFO | MGC_M_TXCSR_CLRDATATOG);
                /* final CSR setup */
                MGC_WriteCsr16(pBase, MGC_O_HDRC_TXCSR, bEnd, csr);
            }
            else
            {
                /* endpoint 0: just flush */
                MGC_WriteCsr16(pBase, MGC_O_HDRC_CSR0, 0, MGC_M_CSR0_FLUSHFIFO);
                MGC_WriteCsr16(pBase, MGC_O_HDRC_CSR0, 0, MGC_M_CSR0_FLUSHFIFO);
            }
        }
        else
        {
            /* receive */
            if (pPort->bIsHost)
            {
                /* protocol/endpoint/interval/NAKlimit */
                if (bEnd)
                {
#ifndef MTK_MHDRC                              
                    //  MTK Notice: Max Liao, 2006/11/28.
                    //  set device address in ep basis for supporting Hub.
                    MGC_Write32(pBase, M_REG_EPXADDR(bEnd), pEnd->bRxBusAddress);
                    MGC_WriteCsr8(pBase, MGC_O_HDRC_RXTYPE, bEnd, reg);
#endif
                    /* first, ensure Rx mode */
                    MGC_WriteCsr16(pBase, MGC_O_HDRC_TXCSR, bEnd, 0);

                    /* 
                     * TODO: at least for bulk, setting the upper 4 interval bits causes the core 
                     * to continuously assert Rx interrupts with RxError set
                     */
                    MGC_WriteCsr8(pBase, MGC_O_HDRC_RXINTERVAL, bEnd, bInterval);
                }
                else
                {
                    MGC_WriteCsr8(pBase, MGC_O_HDRC_NAKLIMIT0, 0, 0x10);
                    MGC_WriteCsr8(pBase, MGC_O_HDRC_TYPE0, 0, reg & 0xc0);
                }
            }

            /* CSR */
            if (bEnd)
            {
                /* packet size */
                MGC_WriteCsr16(pBase, MGC_O_HDRC_RXMAXP, bEnd, pEnd->wRxPacketSize);

                if (pPort->bIsHost)
                {
                    csr = 0;
                }
                else
                {
#ifdef MUSB_ISO
                    if (pEnd->bTrafficType == MUSB_ENDPOINT_XFER_ISOC)
                    {
                        csr = MGC_M_RXCSR_P_ISO;
                    }
#endif
                    if (MUSB_ENDPOINT_XFER_INT == pEnd->bRxTrafficType)
                    {
                        csr |= MGC_M_RXCSR_DISNYET;
                    }
                }
                
                /* twice in case of double packet buffering */
                MGC_WriteCsr16(pBase, MGC_O_HDRC_RXCSR, bEnd,
                               MGC_M_RXCSR_FLUSHFIFO | MGC_M_RXCSR_CLRDATATOG);
                MGC_WriteCsr16(pBase, MGC_O_HDRC_RXCSR, bEnd,
                               MGC_M_RXCSR_FLUSHFIFO | MGC_M_RXCSR_CLRDATATOG);

                /* use AutoClear in peripheral mode */
                MGC_WriteCsr16(pBase, MGC_O_HDRC_RXCSR, bEnd, csr);
            }
        }
    }

#if !defined(MUSB_LOADER)
#if 0
    if ((pPort->bIsHost) && (bStatus))
    {
        (void)pServices->pfIrpUnlock(pServices->pPrivateData, 0);
    }    
#else
    x_crit_end(rState);  
#endif
#endif

    UNUSED(pServices);
    UNUSED(bStatus);

    return pEnd;
}

#if (!defined (CONFIG_ARCH_MT85XX))
extern void HalFlushDCacheMultipleLine(UINT32 u4Addr, UINT32 u4Size);
#endif

#ifdef USB_WIFI_DEBUG
#define USB_RX_DEBUG_CNT    16

MGC_Port *_pUsbRxPort[USB_RX_DEBUG_CNT] = {0};
MGC_EndpointResource *_pUsbRxEnd[USB_RX_DEBUG_CNT] = {0};
uint8_t *_pUsbRxBuffer[USB_RX_DEBUG_CNT] = {0};
uint32_t _dwUsbRxTotalBytes[USB_RX_DEBUG_CNT] = {0};
void *_pUsbRxCurrentIrp[USB_RX_DEBUG_CNT] = {0};
uint8_t _bUsbRxAllowDma[USB_RX_DEBUG_CNT] = {0};
uint32_t _i4UsbRxDebugIdx = 0;
uint32_t _i4UsbStartRxCnt = 0;
uint8_t *_pUsbRxBufLast = NULL;
UINT32 _u4UsbIsrLastCnt = 0;
extern uint8_t _aUsbRxTempBuf[];
extern uint8_t _fgUsbWifiSwapBuf;

extern UINT32 _u4UsbIsrCnt;
#endif

/*
* Program the HDRC to initiate reception.
*/
uint32_t MGC_HdrcStartRx(MGC_Port *pPort, MGC_EndpointResource *pEnd, uint8_t *pBuffer,
                         uint32_t dwTotalBytes, void *pCurrentIrp, uint8_t bAllowDma)
{
#ifdef MUSB_DMA
    MUSB_DmaController *pDmaController;
    MUSB_DmaChannel *pDmaChannel;
    uint8_t bDmaOk;
#endif
    uint16_t wCsr;
    uint8_t reg = 0;
    MGC_Controller *pController;
    uint8_t *pBase;
    MUSB_SystemServices *pServices;
    uint8_t bEnd;
    uint8_t bIntrUsbValue;
#ifdef MUSB_FUNCTION         
    uint32_t dwIntrFlag;
    uint8_t bResult;
    MGC_BsrItem qItem;
    uint16_t wRxCount = 0;
#endif
    MUSB_ControlIrp* pControlIrp;
    MUSB_Irp* pIrp;
    MUSB_IsochIrp* pIsochIrp;
    MGC_Pipe* pPipe;
    const MUSB_Device* pDevice = NULL;
    uint8_t bHubPort = 0;
    uint8_t bSSHubPort = 0;
    uint8_t bSSHubAddr = 0; /* SS, CS packet must use for high speed to full/low speed change.*/    
#ifdef MUSB_HUB    
    MUSB_Hub *pHubDevice = NULL;    
    MUSB_Device* pParentUsbDevice = NULL;           
#endif    
#ifdef MTK_MHDRC	
    uint8_t bIsMulti = FALSE;
#endif
    uint32_t dwStatus = 0;    
    uint8_t bStatus = 0;
	
#if !defined(MUSB_LOADER)
    CRIT_STATE_T rState = 0;
#endif

    MUSB_ASSERT(pPort);
    MUSB_ASSERT(pCurrentIrp);    
    pController = pPort->pController;
    MUSB_ASSERT(pController);
    pBase = (uint8_t *)pController->pControllerAddressIst;
    MUSB_ASSERT(pBase);   
    pServices = pController->pSystemServices;
    MUSB_ASSERT(pServices);
    MUSB_ASSERT(pEnd);    
    bEnd = pEnd->bLocalEnd;
    bIntrUsbValue = MGC_Read8(pBase, MGC_O_HDRC_INTRUSB);

#ifdef USB_WIFI_DEBUG
{
    extern MUSB_Pipe _pUsbBulkInPipe;
    MUSB_Irp *pDebugIrp = (MUSB_Irp *)pCurrentIrp;    

    if(_pUsbBulkInPipe == pDebugIrp->hPipe)
    {
        _i4UsbRxDebugIdx %= USB_RX_DEBUG_CNT;
        _pUsbRxPort[_i4UsbRxDebugIdx] = pPort;
        _pUsbRxEnd[_i4UsbRxDebugIdx] = pEnd;
        _pUsbRxBufLast = _pUsbRxBuffer[_i4UsbRxDebugIdx] = pBuffer;
        _dwUsbRxTotalBytes[_i4UsbRxDebugIdx] = dwTotalBytes;
        _pUsbRxCurrentIrp[_i4UsbRxDebugIdx] = pCurrentIrp;
        _bUsbRxAllowDma[_i4UsbRxDebugIdx] = bAllowDma;
        _i4UsbRxDebugIdx++;
        _i4UsbStartRxCnt++;
        _u4UsbIsrLastCnt = _u4UsbIsrCnt;
    
        if(_fgUsbWifiSwapBuf)
        {
            pDebugIrp->pBuffer = pBuffer = _aUsbRxTempBuf;

            if( ((UINT32)_pUsbRxBufLast & 0xFFFF0000) ==  
                ((UINT32)_aUsbRxTempBuf & 0xFFFF0000))
            {
                LOG(0, "Stop here");
            }

            *(uint32_t*)_pUsbRxBufLast = 0;
        }
    }
}
#endif

    //  MTK Notice: Max Liao, 2006/10/30.
    // check device remove before tx/rx.
    if ((!pPort->bInsert) || (bIntrUsbValue & MGC_M_INTR_DISCONNECT))
    {
        // clear IRP.
        MUSB_DIAG_STRING(1, "MGC_HdrcStartRx: device removed !");
        return (uint32_t)MUSB_STATUS_PEER_REMOVED;
    }

#if !defined(MUSB_LOADER)
#if 0
    if (pPort->bIsHost)
    {
        bStatus = pServices->pfIrpLock(pServices->pPrivateData, 0);
    }
#else
    rState = x_crit_start();
#endif
#endif

    MGC_SelectEnd(pBase, bEnd);

    /* clear mode bit in TxCSR */
    wCsr = MGC_ReadCsr16(pBase, MGC_O_HDRC_TXCSR, bEnd);

    /* receive */
    if (pEnd->bIsFifoShared)
    {
        if(wCsr & MGC_M_TXCSR_TXPKTRDY)
        {
            LOG(0, "WARNING: TX is on-going\n", bEnd);
        }
        
        if(wCsr & MGC_M_TXCSR_FIFONOTEMPTY)
        {
            LOG(0, "*** switching end %d to Rx but Tx FIFO not empty\n",
                bEnd);
        }

        LOG(0, "WARNING: reprogramming ep%d for rx\n", bEnd);
        
        //  MTK Notice: Max Liao, 2006/11/16.
        //  v2.303 bug: Must clear MGC_M_TXCSR_MODE before set RX mode.  
        #ifdef UNIFIED_USB
        if ((wCsr & MGC_M_TXCSR_MODE) != 0)
        {
          LOG(5,"MGC_M_TXCSR_MODE case not exist\n");
        }
        #else
        if ((wCsr & MGC_M_TXCSR_MODE) != 0)
        {
            wCsr &= ~MGC_M_TXCSR_MODE;    
            MGC_WriteCsr16(pBase, MGC_O_HDRC_TXCSR, bEnd, wCsr);
        }
        #endif
    }

    if (pPort->bIsHost)
    {
        /* read RxCSR for preparation */
        wCsr = MGC_ReadCsr16(pBase, MGC_O_HDRC_RXCSR, bEnd);

        /* prepare for the non-DMA case */
        wCsr &= ~(MGC_M_RXCSR_DMAENAB | MGC_M_RXCSR_DMAMODE);

        wCsr |= MGC_M_RXCSR_H_REQPKT;
        wCsr &= ~MGC_M_RXCSR_RXPKTRDY;

        /* write protocol/endpoint/speed */
	switch(pEnd->bRxTrafficType)
	{
	case MUSB_ENDPOINT_XFER_CONTROL:
	    pControlIrp = (MUSB_ControlIrp*)pCurrentIrp;
	    pDevice = pControlIrp->pDevice;
	    break;
	case MUSB_ENDPOINT_XFER_ISOC:
	    pIsochIrp = (MUSB_IsochIrp*)pCurrentIrp;
	    pPipe = (MGC_Pipe*)pIsochIrp->hPipe;
	    pDevice = pPipe->pDevice;
	    break;
	default:
	    pIrp = (MUSB_Irp*)pCurrentIrp;
	    pPipe = (MGC_Pipe*)pIrp->hPipe;
	    pDevice = pPipe->pDevice;
#if defined(CONFIG_ARCH_MT85XX)
	    pIrp->bIsTx = FALSE;
	    pIrp->pEnd = (void *) pEnd;
#endif
	}

        MUSB_ASSERT(pDevice);

#ifdef MUSB_HUB
        /* target addr & hub addr/port */
        if (pDevice->pParentUsbDevice)
        {
            bHubPort = pDevice->bHubPort;
            pParentUsbDevice = pDevice->pParentUsbDevice;

            if (pParentUsbDevice && 
                (bSSHubAddr == 0) && 
                (pParentUsbDevice->ConnectionSpeed != pDevice->ConnectionSpeed)
                &&(pParentUsbDevice->ConnectionSpeed == MUSB_CONNECTION_SPEED_HIGH))
            {
                bSSHubAddr = pParentUsbDevice->bBusAddress;
				/*2009.10.29,dexiao,should set pDevice->bHubPort as port num*/
				#if 0
                bSSHubPort = pParentUsbDevice->bHubPort;
                #else
                bSSHubPort = pDevice->bHubPort;
                #endif
            }
            
            // check if device is still on hub ?
            while(pParentUsbDevice)
            {
                dwStatus = MGC_HubFindHubDevice(pParentUsbDevice, &pHubDevice);
                // Device still on hub port: hub status not change and aHubChildPortMap present.
                if ((MUSB_SUCCESS == dwStatus) && 
                     (pHubDevice) && 
                     (bHubPort <= MUSB_HUB_MAX_PORTS) &&
                     (bHubPort >= 1) &&
                     (
                     #if (!defined (MUSB_LOADER)) || (defined (CONFIG_ARCH_MT85XX))
                     (pHubDevice->dwHubPortStatusChange & (1 << bHubPort)) ||
                     #endif
                       (pHubDevice->aHubChildPortMap[bHubPort-1] == MUSB_HUB_CHILD_FREE)))
                {
                    LOG(1, "MGC_HdrcStartRx: HubPort-%d device removed.\n", bHubPort);
                    dwStatus = (uint32_t)MUSB_STATUS_PEER_REMOVED;
                    goto out;
                }

                // check upper layer.
                bHubPort = pParentUsbDevice->bHubPort;

                if(pParentUsbDevice->pParentUsbDevice)
                {
                if ((bSSHubAddr == 0) && 
                    (pParentUsbDevice->pParentUsbDevice->ConnectionSpeed != 
                        pParentUsbDevice->ConnectionSpeed)
                        &&(pParentUsbDevice->pParentUsbDevice->ConnectionSpeed == MUSB_CONNECTION_SPEED_HIGH)
                        &&(pDevice->ConnectionSpeed !=MUSB_CONNECTION_SPEED_HIGH))
                {
                    bSSHubAddr = pParentUsbDevice->pParentUsbDevice->bBusAddress;
                    bSSHubPort = pParentUsbDevice->pParentUsbDevice->bHubPort;
                    break;
                }
                }
                
                pParentUsbDevice =  pParentUsbDevice->pParentUsbDevice;                
            }

#ifdef MTK_MHDRC	
            if(2 == pDevice->pParentUsbDevice->DeviceDescriptor.bDeviceProtocol)
            {
                    bIsMulti = TRUE;
            }
#endif            
        }
#endif	
#ifdef MTK_MHDRC	
        reg = pEnd->bRxTrafficType << MGC_S_TYPE_PROTO;
        reg |= pEnd->bRxBusEnd & MGC_M_TYPE_REMOTE_END;	
        /* speed field in proto reg */
        switch(pDevice->ConnectionSpeed)
        {
        case MUSB_CONNECTION_SPEED_LOW:
            reg |= 0xc0;
            break;
        case MUSB_CONNECTION_SPEED_FULL:
            reg |= 0x80;
            break;
        default:
            reg |= 0x40;
        }
        MGC_WriteCsr8(pBase, MGC_O_HDRC_RXTYPE, bEnd, reg);
        /* target addr & hub addr/port */
#ifdef UNIFIED_USB
       if(!bEnd)
       {
        MGC_Write8(pBase, MGC_BUSCTL_OFFSET(bEnd, MGC_O_MHDRC_TXFUNCADDR), 
            pDevice->bBusAddress);
        MGC_Write8(pBase, MGC_BUSCTL_OFFSET(bEnd, MGC_O_MHDRC_TXHUBADDR), 
            (bIsMulti ? (uint8_t)(0x80 | bSSHubAddr) : bSSHubAddr));
        MGC_Write8(pBase, MGC_BUSCTL_OFFSET(bEnd, MGC_O_MHDRC_TXHUBPORT), 
            bSSHubPort);  		
       }
	   else
	   {
        MGC_Write8(pBase, MGC_BUSCTL_OFFSET(bEnd, MGC_O_MHDRC_RXFUNCADDR), 
            pDevice->bBusAddress);
        MGC_Write8(pBase, MGC_BUSCTL_OFFSET(bEnd, MGC_O_MHDRC_RXHUBADDR), 
            (bIsMulti ? (uint8_t)(0x80 | bSSHubAddr) : bSSHubAddr));
        MGC_Write8(pBase, MGC_BUSCTL_OFFSET(bEnd, MGC_O_MHDRC_RXHUBPORT), 
            bSSHubPort);
	   }

#else
{
        MGC_Write8(pBase, MGC_BUSCTL_OFFSET(bEnd, MGC_O_MHDRC_RXFUNCADDR), 
            pDevice->bBusAddress);
        MGC_Write8(pBase, MGC_BUSCTL_OFFSET(bEnd, MGC_O_MHDRC_RXHUBADDR), 
            (bIsMulti ? (uint8_t)(0x80 | bSSHubAddr) : bSSHubAddr));
        MGC_Write8(pBase, MGC_BUSCTL_OFFSET(bEnd, MGC_O_MHDRC_RXHUBPORT), 
            bSSHubPort);
}
#endif		
#else
        /* write protocol/endpoint */
        reg = pEnd->bRxTrafficType << MGC_S_TYPE_PROTO;
        reg |= pEnd->bRxBusEnd & MGC_M_TYPE_REMOTE_END;
        MGC_WriteCsr8(pBase, MGC_O_HDRC_RXTYPE, bEnd, reg);
#endif

        if (bEnd)
        {
            /* normal case: set up for packet size */
            MGC_WriteCsr16(pBase, MGC_O_HDRC_RXMAXP, bEnd, pEnd->wRxPacketSize);
        }

        /* no residual; set current IRP */
        pEnd->pRxIrp = pCurrentIrp;
#ifdef MUSB_DMA

        if (dwTotalBytes > pEnd->wRxPacketSize)
        {
            pDmaController = pPort->pDmaController;
            MUSB_ASSERT(pDmaController);
            pDmaChannel = pEnd->pRxDmaChannel;

            if (!bAllowDma && pDmaChannel)
            {
                /* release previously-allocated channel */
                pDmaController->pfDmaReleaseChannel(pDmaChannel);
                pEnd->pRxDmaChannel = NULL;
            }
#if defined (CONFIG_ARCH_MT85XX)
            //if (bAllowDma && pServices->pfSystemToBusAddress(pBuffer))
            if (bAllowDma && pBuffer)
#else
            if (bAllowDma && pServices->pfSystemToBusAddress(pBuffer))
#endif
            {
                /* candidate for DMA */
                if (!pDmaChannel)
                {
                    pDmaChannel = pDmaController->pfDmaAllocateChannel(
                    pDmaController->pPrivateData, bEnd, FALSE, pEnd->bRxTrafficType,
                    pEnd->wRxPacketSize);
                    pEnd->pRxDmaChannel = pDmaChannel;
                }
                if (pDmaChannel)
                {
                    #if (!defined (CONFIG_ARCH_MT85XX))
					HalFlushDCacheMultipleLine((UINT32)pBuffer,MGC_MIN(dwTotalBytes,pDmaChannel->dwMaxLength));
					#endif
                    pEnd->bDmaTx = FALSE;

                    pDmaChannel->dwActualLength = 0L;
                    /* Program DMA channel */

                    //  MTK Notice: Max Liao, 2006/12/07.
                    //  set DMAReqMode = 1 in RXCSR when DMA size is not known.	    
                    pDmaChannel->bDesiredMode = 1;

                    MUSB_ASSERT(pDmaController->pfDmaProgramChannel);
                    bDmaOk = pDmaController->pfDmaProgramChannel(pDmaChannel,
                                                                 pEnd->wRxPacketSize,
                                                                 pDmaChannel->bDesiredMode,
                                                                 pBuffer,
                                                                 MGC_MIN(dwTotalBytes,
                                                                         pDmaChannel->dwMaxLength));

					MU_MB();
					
                    if (bDmaOk)
                    {
                        //  MTK Notice: Max Liao, 2006/12/10.
                        //  prevent client task and USB HISR race condition, set csr in MGC_HsDmaProgramChannel().
                        dwStatus = 0;
                        goto out;
                    }
                    else
                    {
                        pDmaController->pfDmaReleaseChannel(pDmaChannel);
                        pEnd->pRxDmaChannel = NULL;
                    }
                }
            }
        }
#endif

        MGC_WriteCsr16(pBase, MGC_O_HDRC_RXCSR, bEnd, wCsr);
    }
#ifdef MUSB_FUNCTION     
    else
    {
        //  MTK Notice: Max Liao, 2006/12/19.
        //  disable interrupt to protect share data: pEnd->dwFifoRemain with USB ISR.    
        dwIntrFlag = pServices->pfIrqLock(pServices->pPrivateData);

        if (pEnd->dwFifoRemain == 0)
        {
            pEnd->pRxIrp = pCurrentIrp;

            pServices->pfIrqUnlock(pServices->pPrivateData, dwIntrFlag);
            dwStatus = 0;
            goto out;
        }
        else
        {
            wRxCount = pEnd->dwFifoRemain;
            pEnd->dwFifoRemain = 0;
        }

        pServices->pfIrqUnlock(pServices->pPrivateData, dwIntrFlag);        
        if (wRxCount > 0)
        {
            /* handle residual if any */
            MUSB_DIAG1(3, "StartRx: residual byte count=", wRxCount, 16, 0);

            //  MTK Notice: Max Liao, 2006/12/07.
            //  support RX by DMA.
#ifdef MUSB_DMA

            pDmaController = pPort->pDmaController;
            MUSB_ASSERT(pDmaController);
            pDmaChannel = pEnd->pRxDmaChannel;

            if (!bAllowDma && pDmaChannel)
            {
                /* release previously-allocated channel */
                pDmaController->pfDmaReleaseChannel(pDmaChannel);
                pEnd->pRxDmaChannel = NULL;
            }
#if defined (CONFIG_ARCH_MT85XX)
    //else if (bAllowDma && pServices->pfSystemToBusAddress(pBuffer))
	else if (bAllowDma && pBuffer)
#else
    else if (bAllowDma && pServices->pfSystemToBusAddress(pBuffer))
#endif
            {
                /* candidate for DMA */
                if (!pDmaChannel)
                {
                    pDmaChannel = pDmaController->pfDmaAllocateChannel(
                    pDmaController->pPrivateData, bEnd, FALSE, pEnd->bRxTrafficType,
                    pEnd->wRxPacketSize);
                    pEnd->pRxDmaChannel = pDmaChannel;
                }
                if (pDmaChannel)
                {
                    pDmaChannel->dwActualLength = 0L;

                    pEnd->bDmaTx = FALSE;

                    /*
                    * Speculate successful DMA programming and clear these fields if not.
                    * This is because although third-party DMACs obey the interface, 
                    * the Inventra DMAC in some cases will immediately activate, finish,
                    * and interrupt us before the RxCSR write below, and these fields
                    * are needed by its ISR.
                    */
                    //pEnd->bDmaResidual = TRUE;
                    pEnd->pRxIrp = pCurrentIrp;

                    pDmaChannel->bDesiredMode = 0;
                    
                    MUSB_ASSERT(pDmaController->pfDmaProgramChannel);
                    /* attempt to program the channel */
                    bDmaOk = pDmaController->pfDmaProgramChannel(pDmaChannel,
                                                                 pEnd->wRxPacketSize,
                                                                 pDmaChannel->bDesiredMode,
                                                                 pBuffer,
                                                                 MGC_MIN(wRxCount,
                                                                         pDmaChannel->dwMaxLength));
                    if (bDmaOk)
                    {
                        MGC_DIAG(3, pPort->pController, "DMA unload - StartRx]");
                        dwStatus = 0;
                        goto out;
                    }
                    else
                    {
                        //pEnd->bDmaResidual = FALSE;

                        pEnd->pRxIrp = NULL;

                        pDmaController->pfDmaReleaseChannel(pDmaChannel);
                        pEnd->pRxDmaChannel = NULL;
                    }
                }
            }
#endif

            bResult = MGC_PipePacketReceived(pPort, pEnd, wRxCount, FALSE, pCurrentIrp);

            /* clear recv condition if necessary */
            if (wRxCount < pEnd->wRxPacketSize)
            {
                MGC_WriteCsr16(pBase, MGC_O_HDRC_RXCSR, bEnd, 0);
            }
            /* complete IRP if necessary */
            if (bResult)
            {
                if (MGC_CompleteIrp(&qItem, pEnd, (uint8_t)MUSB_STATUS_OK, FALSE, pCurrentIrp))
                {
                    VERIFY(pServices->pfQueueBackgroundItem(pServices->pPrivateData, &qItem));
                }

                MGC_DIAG(3, pPort->pController, "IRP Complete - StartRx]");
                dwStatus = 0;
                goto out;
            }
        }
    }
#else
    else
    {
        MUSB_ASSERT(pPort->bIsHost);
    }    
#endif

out:

    MGC_DIAG(3, pPort->pController, "StartRx]");

#if !defined(MUSB_LOADER)
#if 0
    if ((pPort->bIsHost) && (bStatus))
    {
        (void)pServices->pfIrpUnlock(pServices->pPrivateData, 0);
    }  
#else
    x_crit_end(rState);  
#endif
	UNUSED(rState);
#endif

    UNUSED(bStatus);
    UNUSED(pDevice);
    UNUSED(bHubPort);    
    UNUSED(bSSHubPort);
    UNUSED(bSSHubAddr);
    UNUSED(pBuffer);
    
    return dwStatus;
}

/*
* Program the HDRC to initiate transmit.
*/
uint32_t MGC_HdrcStartTx(MGC_Port *pPort, MGC_EndpointResource *pEnd, const uint8_t *pBuffer,
                         uint32_t dwTotalBytes, void *pGenIrp)
{
    uint32_t dwLoadCount;
    uint16_t wCsr = 0;
    uint8_t reg = 0;
    MGC_Controller *pController;
#ifdef MUSB_DMA
    MUSB_DmaController *pDmaController;
    MUSB_DmaChannel *pDmaChannel;
    uint8_t bDmaOk = FALSE;
    uint8_t bAllowDma = FALSE;
    MUSB_Irp *pIrp = NULL;
    MUSB_SystemServices *pServices;
#endif
    uint8_t *pBase;
    uint8_t bEnd;
    uint8_t bIntrUsbValue;
    MUSB_ControlIrp* pControlIrp;
#ifdef MUSB_ISO
    MUSB_IsochIrp *pIsochIrp = NULL;
#endif
    MGC_Pipe* pPipe;
    const MUSB_Device* pDevice = NULL;   
    uint8_t bHubPort = 0;
    uint8_t bSSHubPort = 0;
    uint8_t bSSHubAddr = 0; /* SS, CS packet must use for high speed to full/low speed change.*/
#ifdef MUSB_HUB    
    MUSB_Hub *pHubDevice = NULL;    
    MUSB_Device* pParentUsbDevice = NULL;
#endif    
#ifdef MTK_MHDRC	
    uint8_t bIsMulti = FALSE;
#endif
    uint32_t dwStatus = 0;    
    uint8_t bStatus = 0;
	
#if !defined(MUSB_LOADER)
    CRIT_STATE_T rState = 0;
#endif

    MUSB_ASSERT(pPort);
    MUSB_ASSERT(pGenIrp);    
    pController = pPort->pController;
    MUSB_ASSERT(pController);
    pBase = (uint8_t *)pController->pControllerAddressIst;
    MUSB_ASSERT(pBase);   
    MUSB_ASSERT(pGenIrp);
#ifdef MUSB_DMA
    pServices = pController->pSystemServices;
    MUSB_ASSERT(pServices);
#endif
    MUSB_ASSERT(pEnd);
    bEnd = pEnd->bLocalEnd;
    bIntrUsbValue = MGC_Read8(pBase, MGC_O_HDRC_INTRUSB);

    //  MTK Notice: Max Liao, 2006/10/30.
    // check device remove before tx/rx.
    if ((!pPort->bInsert) || (bIntrUsbValue & MGC_M_INTR_DISCONNECT))
    {
        MUSB_DIAG_STRING(1, "MGC_HdrcStartTx: device removed !");
        return (uint32_t)MUSB_STATUS_PEER_REMOVED;
    }

#if !defined(MUSB_LOADER)
#if 0
    if (pPort->bIsHost)
    {
        bStatus = pServices->pfIrpLock(pServices->pPrivateData, 0);    
    }    
#else
    rState = x_crit_start();
#endif
#endif

    MGC_SelectEnd(pBase, bEnd);

    if(pPort->bIsHost)
    {
        /* host mode; program protocol/speed and destination */
	switch(pEnd->bTrafficType)
	{
	case MUSB_ENDPOINT_XFER_CONTROL:
	    pControlIrp = (MUSB_ControlIrp*)pGenIrp;
	    pDevice = pControlIrp->pDevice;
	    break;
#ifdef MUSB_ISO
	case MUSB_ENDPOINT_XFER_ISOC:
	    pIsochIrp = (MUSB_IsochIrp*)pGenIrp;
	    pPipe = (MGC_Pipe*)pIsochIrp->hPipe;
	    pDevice = pPipe->pDevice;
	    break;
#endif
	default:
	    pIrp = (MUSB_Irp*)pGenIrp;
	    pPipe = (MGC_Pipe*)pIrp->hPipe;
	    pDevice = pPipe->pDevice;
#if defined(CONFIG_ARCH_MT85XX)
	    pIrp->bIsTx = TRUE;
	    pIrp->pEnd = (void *)pEnd;
#endif
	}

        MUSB_ASSERT(pDevice);

#ifdef MUSB_HUB
        if (pDevice->pParentUsbDevice)
        {
            bHubPort = pDevice->bHubPort;
            pParentUsbDevice = pDevice->pParentUsbDevice;

            if (pParentUsbDevice && 
                (bSSHubAddr == 0) && 
                (pParentUsbDevice->ConnectionSpeed != pDevice->ConnectionSpeed)
               &&(pParentUsbDevice->ConnectionSpeed == MUSB_CONNECTION_SPEED_HIGH ) )
            {
                bSSHubAddr = pParentUsbDevice->bBusAddress;
				/*2009.10.29,dexiao,should set pDevice->bHubPort as port num*/
				#if 0
                bSSHubPort = pParentUsbDevice->bHubPort;
				#else
                bSSHubPort = pDevice->bHubPort;
				#endif
            }
            
            // check if device is still on hub ?
            while(pParentUsbDevice)
            {
                dwStatus = MGC_HubFindHubDevice(pParentUsbDevice, &pHubDevice);
                // Device still on hub port: hub status not change and aHubChildPortMap present.
                if ((MUSB_SUCCESS == dwStatus) && 
                     (pHubDevice) && 
                     (bHubPort <= MUSB_HUB_MAX_PORTS) &&
                     (bHubPort >= 1) &&
                     (
#if (!defined (MUSB_LOADER)) || (defined (CONFIG_ARCH_MT85XX))
                    (pHubDevice->dwHubPortStatusChange & (1 << bHubPort)) ||
#endif
                     
                       (pHubDevice->aHubChildPortMap[bHubPort-1] == MUSB_HUB_CHILD_FREE)))
                {
                    LOG(1, "MGC_HdrcStartTx: HubPort-%d device removed.\n", bHubPort);
                    
                    dwStatus = (uint32_t)MUSB_STATUS_PEER_REMOVED;
                    goto out;
                }

                // check upper layer.
                bHubPort = pParentUsbDevice->bHubPort;

                if(pParentUsbDevice->pParentUsbDevice)
                {
                if ((bSSHubAddr == 0) && 
                    (pParentUsbDevice->pParentUsbDevice->ConnectionSpeed != 
                        pParentUsbDevice->ConnectionSpeed)
                        &&(pParentUsbDevice->pParentUsbDevice->ConnectionSpeed== MUSB_CONNECTION_SPEED_HIGH)
                        &&(pDevice->ConnectionSpeed != MUSB_CONNECTION_SPEED_HIGH))
                {
                    bSSHubAddr = pParentUsbDevice->pParentUsbDevice->bBusAddress;
                    bSSHubPort = pParentUsbDevice->pParentUsbDevice->bHubPort;
                    break;
                }
                }                
                
                pParentUsbDevice =  pParentUsbDevice->pParentUsbDevice;                
            }

#ifdef MTK_MHDRC	
            if(2 == pDevice->pParentUsbDevice->DeviceDescriptor.bDeviceProtocol)
            {
                bIsMulti = TRUE;
            }
#endif
        }
#endif        
#ifdef MTK_MHDRC	
        reg = pEnd->bTrafficType << MGC_S_TYPE_PROTO;
        reg |= pEnd->bBusEnd & MGC_M_TYPE_REMOTE_END;
	/* speed field in proto reg */
	switch(pDevice->ConnectionSpeed)
	{
	case MUSB_CONNECTION_SPEED_LOW:
	    reg |= 0xc0;
	    break;
	case MUSB_CONNECTION_SPEED_FULL:
	    reg |= 0x80;
	    break;
	default:
	    reg |= 0x40;
	}
	if(bEnd)
	{
	    MGC_WriteCsr8(pBase, MGC_O_HDRC_TXTYPE, bEnd, reg);
	}
	else
	{
	    MGC_WriteCsr8(pBase, MGC_O_HDRC_TYPE0, 0, reg & 0xc0);
	}
	/* target addr & hub addr/port */
	MGC_Write8(pBase, MGC_BUSCTL_OFFSET(bEnd, MGC_O_MHDRC_TXFUNCADDR), 
	    pDevice->bBusAddress);
	MGC_Write8(pBase, MGC_BUSCTL_OFFSET(bEnd, MGC_O_MHDRC_TXHUBADDR), 
	    (bIsMulti ? (uint8_t)(0x80 | bSSHubAddr) : bSSHubAddr));
	MGC_Write8(pBase, MGC_BUSCTL_OFFSET(bEnd, MGC_O_MHDRC_TXHUBPORT), 
	    bSSHubPort);	
	
	#ifndef UNIFIED_USB
	if(!bEnd)
	{
	    MGC_Write8(pBase, MGC_BUSCTL_OFFSET(bEnd, MGC_O_MHDRC_RXFUNCADDR), 
		pDevice->bBusAddress);
	    MGC_Write8(pBase, MGC_BUSCTL_OFFSET(bEnd, MGC_O_MHDRC_RXHUBADDR), 
		(bIsMulti ? (uint8_t)(0x80 | bSSHubAddr) : bSSHubAddr));
	    MGC_Write8(pBase, MGC_BUSCTL_OFFSET(bEnd, MGC_O_MHDRC_RXHUBPORT), 
		bSSHubPort);
	}
	#endif
#else    
        /* host mode; program protocol/endpoint */
        reg = pEnd->bTrafficType << MGC_S_TYPE_PROTO;

        reg |= pEnd->bBusEnd & MGC_M_TYPE_REMOTE_END;
        MGC_WriteCsr8(pBase, MGC_O_HDRC_TXTYPE, bEnd, reg);
#endif
    }
    /* determine how much to load into FIFO for non-DMA case */
    dwLoadCount = MGC_MIN(pEnd->wPacketSize, dwTotalBytes);

    if (bEnd)
    {
        /* normal case: set up for packet size */
        MGC_WriteCsr16(pBase, MGC_O_HDRC_TXMAXP, bEnd, pEnd->wPacketSize);

        /* read CSR for preparation */
        wCsr = MGC_ReadCsr16(pBase, MGC_O_HDRC_TXCSR, bEnd) & ~MGC_M_TXCSR_P_UNDERRUN;
    }
    else
    {
        /* ensure FIFO ready for writing */
        MGC_WriteCsr16(pBase, MGC_O_HDRC_CSR0, 0, 0);
//  MTK Notice: Max Liao, 2007/07/17.            
//  turn on nak limit when process control transfer.        
#if defined (CONFIG_ARCH_MT85XX)
        MGC_WriteCsr8(pBase, MGC_O_HDRC_NAKLIMIT0, 0, ((pPort->bIsHighSpeed) ? 16 : 13));
#else
//emulation board. can't set 16
        MGC_WriteCsr8(pBase, MGC_O_HDRC_NAKLIMIT0, 0, ((pPort->bIsHighSpeed) ? 15 : 13));
#endif
    }

    //  MTK Notice: Max Liao, 2006/12/07.
    //  register txirp before set dma.
    /* write CSR */
    pEnd->pTxIrp = pGenIrp;
#ifdef MUSB_DMA

    switch (pEnd->bTrafficType)
    {
#ifdef MUSB_ISO

    case MUSB_ENDPOINT_XFER_ISOC:
        pIsochIrp = (MUSB_IsochIrp *)pGenIrp;
        bAllowDma = pIsochIrp->bAllowDma;
        break;
#endif

    case MUSB_ENDPOINT_XFER_CONTROL:
        bAllowDma = FALSE;
        break;

    default:
        pIrp = (MUSB_Irp *)pGenIrp;
        bAllowDma = pIrp->bAllowDma;
        break;
    }

    pDmaController = pPort->pDmaController;
    MUSB_ASSERT(pDmaController);
    pDmaChannel = pEnd->pDmaChannel;

    if (!bAllowDma && pDmaChannel)
    {
        /* release previously-allocated channel */
        pDmaController->pfDmaReleaseChannel(pDmaChannel);
        pEnd->pDmaChannel = NULL;
    }
#if defined (CONFIG_ARCH_MT85XX)
    //else if (bAllowDma && pServices->pfSystemToBusAddress(pBuffer))
	else if (bAllowDma && pBuffer)
#else
    else if (bAllowDma && pServices->pfSystemToBusAddress(pBuffer))
#endif
    {
        /* candidate for DMA */
        if (!pDmaChannel)
        {
            pDmaChannel = pDmaController->pfDmaAllocateChannel(
            pDmaController->pPrivateData, bEnd, TRUE, pEnd->bTrafficType, pEnd->wPacketSize);
            pEnd->pDmaChannel = pDmaChannel;
        }
        if (pDmaChannel)
        {
            pEnd->bDmaTx = TRUE;

            pDmaChannel->dwActualLength = 0L;

            //  MTK Notice: Max Liao, 2006/12/07.
            //  set DMAReqMode by sending data size.	    	    
            pDmaChannel->bDesiredMode = (uint8_t)((dwTotalBytes > pEnd->wPacketSize) ? 1 : 0);
            MUSB_ASSERT(pDmaController->pfDmaProgramChannel);
            bDmaOk = pDmaController->pfDmaProgramChannel(
                         pDmaChannel, pEnd->wPacketSize, pDmaChannel->bDesiredMode, pBuffer,
                         MGC_MIN(dwTotalBytes, pDmaChannel->dwMaxLength));

			MU_MB();
			
            if (bDmaOk)
            {
                //  MTK Notice: Max Liao, 2006/12/10.
                //  prevent client task and USB HISR race condition, set csr in MGC_HsDmaProgramChannel().	
                dwStatus = 0;
                goto out;
            }
            else
            {
                MUSB_ASSERT(pDmaController->pfDmaReleaseChannel);
                pDmaController->pfDmaReleaseChannel(pDmaChannel);
                pEnd->pDmaChannel = NULL;
            }
        }
    }
#endif
    /* load FIFO */

    pPort->pfLoadFifo(pPort, pEnd->bLocalEnd, dwLoadCount, pBuffer);
    pEnd->dwTxSize = dwLoadCount;  
    /* prepare CSR */
    if (bEnd)
    {
        /* since we cannot clear DMAMODE before/during clearing DMAENAB, never clear DMAMODE */
        wCsr &= (uint16_t)(~(MGC_M_TXCSR_AUTOSET | MGC_M_TXCSR_DMAENAB));
        wCsr |= MGC_M_TXCSR_TXPKTRDY;
    }
    else
    {
        /* endpoint 0 is a fixed case */
        wCsr = (uint16_t)(MGC_M_CSR0_H_SETUPPKT | MGC_M_CSR0_TXPKTRDY);
        if (pPort->bHasDisablePing)
        {
            wCsr |= MGC_M_CSR0_H_NO_PING;
        }
    }

    if (bEnd)
    {
    #ifdef UNIFIED_USB
            MGC_WriteCsr16(pBase, MGC_O_HDRC_TXCSR, bEnd, wCsr);
	#else
            MGC_WriteCsr16(pBase, MGC_O_HDRC_TXCSR, bEnd, wCsr | MGC_M_TXCSR_MODE);
	#endif
    }
    else
    {
        MGC_WriteCsr16(pBase, MGC_O_HDRC_CSR0, 0, wCsr);
    }


out:

    MGC_DIAG(3, pPort->pController, "StartTx]");

#if !defined(MUSB_LOADER)
#if 0
    if ((pPort->bIsHost) && (bStatus))
    {
        (void)pServices->pfIrpUnlock(pServices->pPrivateData, 0);
    }    
#else
    x_crit_end(rState);  
#endif
#endif

    UNUSED(bStatus);
    UNUSED(pDevice);
    UNUSED(bHubPort);
    UNUSED(bSSHubPort);
    UNUSED(bSSHubAddr);
    
    return dwStatus;
}

/* Program the HDRC to flush the given endpoint wrt the given USB direction */
uint32_t MGC_HdrcFlushEndpoint(MGC_Port *pPort, MGC_EndpointResource *pEnd, uint8_t bDir,
                               uint8_t bReuse)
{
    uint16_t wCsr;

    MGC_Controller *pController;
    MUSB_SystemServices *pServices;    
    uint8_t *pBase;
    uint8_t bTx = FALSE;
    uint8_t bEnd;
    uint8_t bStatus = 0;
	
#if !defined(MUSB_LOADER)
    CRIT_STATE_T rState = 0;
#endif

    MUSB_ASSERT(pPort);
    pController = pPort->pController;
    MUSB_ASSERT(pController);
    pServices = pController->pSystemServices;

#if !defined(MUSB_LOADER)
#if 0
    if (pPort->bIsHost)
    {
        bStatus = pServices->pfIrpLock(pServices->pPrivateData, 0);
    }
#else
    rState = x_crit_start();
#endif
#endif

    pBase = (uint8_t *)pController->pControllerAddressIst;
    MUSB_ASSERT(pBase);
    MUSB_ASSERT(pEnd);
    bEnd = pEnd->bLocalEnd;

    /* select hw ep */
    MGC_SelectEnd(pBase, bEnd);

    /* determine TX/RX */
    if (bDir & MUSB_DIR_IN)
    {
        if (!pPort->bIsHost)
        {
            bTx = TRUE;
        }
    }
    else
    {
        if (pPort->bIsHost)
        {
            bTx = TRUE;
        }
    }

    if (bTx)
    {
        MGC_DIAG1(2, pController, "Flushing Tx on ", bEnd, 16, 2);
#ifdef MUSB_DMA

        if (pEnd->pDmaChannel && pEnd->bDmaTx)
        {
            pPort->pDmaController->pfDmaReleaseChannel(pEnd->pDmaChannel);
            pEnd->pDmaChannel = NULL;
        }
#endif
        /*  flush the xmit fifo, reset the data tog to 0 */
        //  MTK Notice: Max Liao, 2006/11/27.
        //  clear dma control and stall status when flush ep.	
        //	wCsr = MGC_ReadCsr16(pBase, MGC_O_HDRC_TXCSR, bEnd);

        wCsr = 0;
        MGC_WriteCsr16(pBase, MGC_O_HDRC_TXCSR, bEnd,
                       wCsr | (MGC_M_TXCSR_FLUSHFIFO | MGC_M_TXCSR_CLRDATATOG));
        MGC_WriteCsr16(pBase, MGC_O_HDRC_TXCSR, bEnd, wCsr | MGC_M_TXCSR_FLUSHFIFO);
    }
    else
    {
        MGC_DIAG1(2, pController, "Flushing Rx on ", bEnd, 16, 2);

        if (pPort->bIsHost)
        {
            if (bEnd)
            {
                wCsr = MGC_ReadCsr16(pBase, MGC_O_HDRC_RXCSR, bEnd);

                wCsr &= ~MGC_M_RXCSR_H_REQPKT;
                MGC_WriteCsr16(pBase, MGC_O_HDRC_RXCSR, bEnd, wCsr);
            }
            else
            {
                wCsr = MGC_ReadCsr16(pBase, MGC_O_HDRC_CSR0, 0);

                wCsr &= ~MGC_M_CSR0_H_REQPKT;
                MGC_WriteCsr16(pBase, MGC_O_HDRC_CSR0, 0, wCsr);
            }
        }
#ifdef MUSB_DMA

        if (pEnd->pRxDmaChannel && !pEnd->bDmaTx)
        {
            pPort->pDmaController->pfDmaReleaseChannel(pEnd->pRxDmaChannel);
            pEnd->pRxDmaChannel = NULL;
        }
#endif
        /*  flush the recv fifo, reset the data tog to 0 */
        //  MTK Notice: Max Liao, 2006/11/27.
        //  clear dma control and stall status when flush ep.	
        //	wCsr = MGC_ReadCsr16(pBase, MGC_O_HDRC_RXCSR, bEnd);

        wCsr = 0;
        MGC_WriteCsr16(pBase, MGC_O_HDRC_RXCSR, bEnd,
                       wCsr | (MGC_M_RXCSR_FLUSHFIFO | MGC_M_RXCSR_CLRDATATOG));
        MGC_WriteCsr16(pBase, MGC_O_HDRC_RXCSR, bEnd, wCsr | MGC_M_RXCSR_FLUSHFIFO);
    }


#if !defined(MUSB_LOADER)
#if 0
    if ((pPort->bIsHost) && (bStatus))
    {
        (void)pServices->pfIrpUnlock(pServices->pPrivateData, 0);
    }    
#else
    x_crit_end(rState);  
#endif
#endif

    UNUSED(pServices);
    UNUSED(bStatus);
    UNUSED(bReuse);

    return 0;
}

/* Program the HDRC to set/clear the halting (stall) of an endpoint */
#ifdef MUSB_FUNCTION    
uint32_t MGC_HdrcHaltEndpoint(MGC_Port *pPort, MGC_EndpointResource *pEnd, uint8_t bDir,
                              uint8_t bHalt)
{
    uint16_t val;

    uint8_t bTx = FALSE;
    MGC_Controller *pController;
    uint8_t *pBase;

    MUSB_ASSERT(pPort);
    pController = pPort->pController;
    MUSB_ASSERT(pController);
    pBase = (uint8_t *)pController->pControllerAddressIst;
    MUSB_ASSERT(pBase);
    MUSB_ASSERT(pEnd);

    /* determine TX/RX */
    if (bDir & MUSB_DIR_IN)
    {
        if (!pPort->bIsHost)
        {
            bTx = TRUE;
        }
    }
    else
    {
        if (pPort->bIsHost)
        {
            bTx = TRUE;
        }
    }

    MGC_SelectEnd(pBase, pEnd->bLocalEnd);

    if (bTx)
    {
        /* Tx */
        val = MGC_ReadCsr16(pBase, MGC_O_HDRC_TXCSR, pEnd->bLocalEnd);

        if (bHalt)
        {
            if (pPort->bIsHost)
            {
                /* mark for stopping */
                pEnd->bStopTx = TRUE;
                if (!pEnd->pTxIrp)
                {
                    /* if already stopped, indicate it */
                    pEnd->bIsHalted = TRUE;
                }
            }
            else
            {
                val &= ~(MGC_M_TXCSR_P_UNDERRUN | MGC_M_TXCSR_P_INCOMPTX);
                val |= MGC_M_TXCSR_P_SENDSTALL;
            }
        }
        else
        {
            if (pPort->bIsHost)
            {
                //  MTK Notice: Max Liao, 2006/11/09.
                //  turn off bStopTx when clear halt.
                pEnd->bStopTx = FALSE;

                /* restart Tx traffic */
                pEnd->bIsHalted = FALSE;
                /* start next IRP (if any) */
                MGC_StartNextIrp(pPort, pEnd, TRUE);
            }
            else
            {
                val &= ~(MGC_M_TXCSR_P_SENDSTALL | MGC_M_TXCSR_P_UNDERRUN | MGC_M_TXCSR_P_INCOMPTX);
                /* apparently the hardware is not doing this: */
                val |= MGC_M_TXCSR_CLRDATATOG;
            }
        }
        MGC_WriteCsr16(pBase, MGC_O_HDRC_TXCSR, pEnd->bLocalEnd, val);
    }
    else
    {
        /* Rx */
        val = MGC_ReadCsr16(pBase, MGC_O_HDRC_RXCSR, pEnd->bLocalEnd);

        if (bHalt)
        {
            if (pPort->bIsHost)
            {
                /* stop Rx traffic */
                pEnd->bIsRxHalted = TRUE;
                val &= ~(MGC_M_RXCSR_H_REQPKT | MGC_M_RXCSR_H_AUTOREQ | MGC_M_RXCSR_DMAENAB);
            }
            else
            {
                val |= MGC_M_RXCSR_P_SENDSTALL;
            }
        }
        else
        {
            if (pPort->bIsHost)
            {
                /* restart Rx traffic */
                pEnd->bIsRxHalted = FALSE;
                if (pEnd->pRxIrp)
                {
                    /* restart pending IRP */
#ifdef MUSB_DMA

                    val |= (pEnd->pDmaChannel && !pEnd->bDmaTx)
                               ? (MGC_M_RXCSR_H_AUTOREQ | MGC_M_RXCSR_DMAENAB)
                               : MGC_M_RXCSR_H_REQPKT;
#else

                    val |= MGC_M_RXCSR_H_REQPKT;
#endif

                }
                else
                {
                    /* start next IRP (if any) */
                    MGC_StartNextIrp(pPort, pEnd, FALSE);
                }
            }
            else
            {
                val &= ~MGC_M_RXCSR_P_SENDSTALL;
                /* apparently the hardware is not doing this: */
                val |= MGC_M_RXCSR_CLRDATATOG;
            }
        }
        MGC_WriteCsr16(pBase, MGC_O_HDRC_RXCSR, pEnd->bLocalEnd, val);
    }

    return 0;
}

/*
* Respond on default endpoint
*/
uint32_t MGC_HdrcDefaultEndResponse(MGC_Port *pPort, uint8_t bStall)
{
    uint16_t wSize, wCsrVal;

    uint8_t bVal = MGC_M_CSR0_P_SVDRXPKTRDY;
    MGC_Controller *pController;
    uint8_t *pBase;

    MUSB_ASSERT(pPort);
    pController = pPort->pController;
    MUSB_ASSERT(pController);
    pBase = (uint8_t *)pController->pControllerAddressIst;
    MUSB_ASSERT(pBase);

    MGC_SelectEnd(pBase, 0);
    wCsrVal = MGC_ReadCsr16(pBase, MGC_O_HDRC_CSR0, 0);

    if (wCsrVal & MGC_M_CSR0_P_SETUPEND)
    {
        bVal |= MGC_M_CSR0_P_SVDSETUPEND;
    }
    /* prepare stall response if required */
    if (bStall)
    {
        bVal |= MGC_M_CSR0_P_SENDSTALL;
    }
    else if (pPort->wSetupDataSize)
    {
        /* we need to transmit; ack RxPktRdy BEFORE loading FIFO */
        MGC_WriteCsr8(pBase, MGC_O_HDRC_CSR0, 0, bVal);

        bVal = 0;
        /* start transmit */
        wSize = (uint16_t)MUSB_MIN(64, pPort->wSetupDataSize);
        MGC_HdrcLoadFifo(pPort, 0, wSize, pPort->pSetupData);
        pPort->wSetupDataIndex = wSize;
        bVal |= MGC_M_CSR0_TXPKTRDY;
        if (wSize == pPort->wSetupDataSize)
        {
            bVal |= MGC_M_CSR0_P_DATAEND;

            pPort->bEnd0Status = TRUE;
            pPort->wSetupDataIndex = 0;
            pPort->wSetupDataSize = 0;
        }
    }
    else
    {
        /* just acknowledge end of data */
        bVal |= MGC_M_CSR0_P_DATAEND;
        pPort->bEnd0Status = TRUE;
    }

    /* write CSR0 */
    MGC_DIAG1(3, pPort->pController, "DefaultEndResponse: Setting CSR0 to ", bVal, 16, 2);
    MGC_WriteCsr8(pBase, MGC_O_HDRC_CSR0, 0, bVal);

    return 0;
}
#endif
/*
 * Handle default endpoint interrupt
 */

uint8_t MGC_HdrcServiceDefaultEnd(MGC_Port *pPort, MGC_BsrItem *pItem)
{
    uint16_t wCsrVal, wCount;
#ifdef MUSB_FUNCTION

    uint8_t bTestVal;
#endif

    uint8_t bError = FALSE;
    uint8_t bResult = FALSE;
    uint8_t status = MUSB_STATUS_OK;
    MGC_EndpointResource *pEnd;
    uint8_t bIntrUsbValue;
    MGC_Controller *pController;
    uint8_t *pBase;

    MUSB_ASSERT(pPort);
    pController = pPort->pController;
    MUSB_ASSERT(pController);
    pBase = (uint8_t *)pController->pControllerAddressIst;
    MUSB_ASSERT(pBase);
    pEnd = MUSB_ArrayFetch(&(pPort->LocalEnds), 0);
    if (!pEnd)
    {
        LOG(0, "pEnd = NULL.\n", NULL);    
        return bResult;        
    }
    
    bIntrUsbValue = MGC_Read8(pBase, MGC_O_HDRC_INTRUSB);

    MGC_SelectEnd(pBase, 0);
    wCsrVal = MGC_ReadCsr16(pBase, MGC_O_HDRC_CSR0, 0);
    wCount = MGC_ReadCsr8(pBase, MGC_O_HDRC_COUNT0, 0);
    MGC_DIAG1(3, pPort->pController, "CSR0=", wCsrVal, 16, 2);
    MGC_DIAG1(3, pPort->pController, "Count0=", wCount, 16, 2);

    //  MTK Notice: Max Liao, 2006/11/02.
    // check device remove before handling ep0 interrupt.
    if ((!pPort->bInsert) || (bIntrUsbValue & MGC_M_INTR_DISCONNECT))
    {
        // clear CSR0.
        MGC_WriteCsr16(pBase, MGC_O_HDRC_CSR0, 0, MGC_M_CSR0_FLUSHFIFO);

        MGC_WriteCsr16(pBase, MGC_O_HDRC_CSR0, 0, MGC_M_CSR0_FLUSHFIFO);
        MUSB_DIAG_STRING(1, "MGC_HdrcServiceDefaultEnd: device removed !");
        return bResult;
    }

    if (pPort->bIsHost)
    {
        if (wCsrVal & MGC_M_CSR0_H_RXSTALL)
        {
            LOG(0, "End 0 stall\n", NULL);

            status = (uint8_t)MUSB_STATUS_STALLED;
            bError = TRUE;
        }
        else if (wCsrVal & MGC_M_CSR0_H_ERROR)
        {
            LOG(0, "end 0: no response (error)\n", NULL);

            status = (uint8_t)MUSB_STATUS_ACK_TIMEOUT;
            bError = TRUE;
        }
        else if (wCsrVal & MGC_M_CSR0_H_NAKTIMEOUT)
        {
            LOG(0, "end 0: no response (NAK timeout)\n", NULL);

            status = (uint8_t)MUSB_STATUS_NACK_LIMIT;
            bError = TRUE;
        }

        if ((uint8_t)MUSB_STATUS_NACK_LIMIT == status)
        {
            /* use the proper sequence to abort the transfer */
            if (wCsrVal & MGC_M_CSR0_H_REQPKT)
            {
                wCsrVal &= ~MGC_M_CSR0_H_REQPKT;

                MGC_WriteCsr16(pBase, MGC_O_HDRC_CSR0, 0, wCsrVal);
                wCsrVal &= ~MGC_M_CSR0_H_NAKTIMEOUT;
                MGC_WriteCsr16(pBase, MGC_O_HDRC_CSR0, 0, wCsrVal);
            }
            else
            {
                wCsrVal |= MGC_M_CSR0_FLUSHFIFO;

                MGC_WriteCsr16(pBase, MGC_O_HDRC_CSR0, 0, wCsrVal);
                MGC_WriteCsr16(pBase, MGC_O_HDRC_CSR0, 0, wCsrVal);
                wCsrVal &= ~MGC_M_CSR0_H_NAKTIMEOUT;
                MGC_WriteCsr16(pBase, MGC_O_HDRC_CSR0, 0, wCsrVal);
            }
        }
        if (bError)
        {
            /* clear it */
            MGC_WriteCsr16(pBase, MGC_O_HDRC_CSR0, 0, 0);
        }
    }
#ifdef MUSB_FUNCTION

    else if (wCsrVal & MGC_M_CSR0_P_SENTSTALL)
    {
        bError = TRUE;

        wCsrVal &= ~MGC_M_CSR0_P_SENTSTALL;
        MGC_WriteCsr16(pBase, MGC_O_HDRC_CSR0, 0, wCsrVal);
    }
    else if (wCsrVal & MGC_M_CSR0_P_SETUPEND)
    {
        //pPort->wSetupDataIndex = 0;

        pPort->wSetupDataSize = 0;
    }
    else if (!wCount && pPort->bEnd0Status)
    {
        /* this is the "acknowledge" interrupt */
        pPort->bEnd0Status = FALSE;

        MGC_WriteCsr16(pBase, MGC_O_HDRC_CSR0, 0, 0);

        /* set address if required */
        if (pPort->bSetAddress)
        {
            MGC_HdrcProgramBusState(pPort);
            pPort->bSetAddress = FALSE;
        }

        /* set TESTMODE register if required */
        if (pPort->bWantTestMode)
        {
            pPort->bWantTestMode = FALSE;

            bTestVal = 0;

            switch (pPort->bTestMode)
            {
            case MUSB_TESTMODE_TEST_J:
                bTestVal = MGC_M_TEST_J;
                break;

            case MUSB_TESTMODE_TEST_K:
                bTestVal = MGC_M_TEST_K;
                break;

            case MUSB_TESTMODE_TEST_SE0_NAK:
                bTestVal = MGC_M_TEST_SE0_NAK;
                break;

            case MUSB_TESTMODE_TEST_PACKET:
                bTestVal = MGC_M_TEST_PACKET;
                MGC_HdrcLoadFifo(pPort, 0, sizeof(MGC_aTestPacket), MGC_aTestPacket);
                /* despite explicit instruction, we still must kick-start */
                MGC_WriteCsr16(pBase, MGC_O_HDRC_CSR0, 0, MGC_M_CSR0_TXPKTRDY);
                break;
                
            default:
                break;
            }
            MGC_Write8(pBase, MGC_O_HDRC_TESTMODE, bTestVal);
        }
        return FALSE;
    }
#endif

    if (bError)
    {
        if (pPort->bIsHost)
        {
            if (MGC_CompleteIrp(pItem, pEnd, status, TRUE, pEnd->pTxIrp))
            {
                bResult = TRUE;
            }
            /* start next transfer if any */
            MGC_StartNextControlTransfer(pPort);
        }
    }
    else
    {
        /* call common logic */
        bResult = MGC_DrcServiceDefaultEnd(pPort, pItem, &wCsrVal, wCount);

        /* write CSR0 */
        if (wCsrVal)
        {
            if (pPort->bIsHost && pPort->bHasDisablePing)
            {
                wCsrVal |= MGC_M_CSR0_H_NO_PING;
            }

            MGC_DIAG1(3, pPort->pController, "ServiceDefaultEnd: Setting CSR0 to ", wCsrVal, 16, 2);
            MGC_WriteCsr16(pBase, MGC_O_HDRC_CSR0, 0, wCsrVal);
        }
    }

    return bResult;
}

#if defined(CONFIG_ARCH_MT85XX)
extern void MGC_DMAUnmap(MUSB_DmaChannel* pDmaChannel, uint8_t bIsTx);
#endif

uint8_t MGC_HdrcServiceTransmitAvail(MGC_Port *pPort, uint16_t wEndIndex, MGC_BsrItem *pItem)
{
    uint16_t wVal;

    MGC_EndpointResource *pEnd;
    uint8_t bResult = FALSE;
    uint32_t status = MUSB_STATUS_OK;
    uint8_t bEnd = (uint8_t)wEndIndex;
    MGC_Controller *pController;
    uint8_t *pBase;
    uint8_t bIntrUsbValue;
    uint32_t u4Ret;

    MUSB_ASSERT(pPort);
    MUSB_ASSERT(pItem);
    pController = pPort->pController;
    MUSB_ASSERT(pController);
    pBase = (uint8_t *)pController->pControllerAddressIst;
    MUSB_ASSERT(pBase);
    bIntrUsbValue = MGC_Read8(pBase, MGC_O_HDRC_INTRUSB);

    pItem->bStatus = MUSB_STATUS_OK;
    pItem->bCause = MGC_BSR_CAUSE_NONE;

    MGC_SelectEnd(pBase, bEnd);
    wVal = MGC_ReadCsr16(pBase, MGC_O_HDRC_TXCSR, bEnd);

    pEnd = (MGC_EndpointResource *)MUSB_ArrayFetch(&(pPort->LocalEnds), wEndIndex);
    if (!pEnd)
    {
        LOG(0, "pEnd = NULL.\n", NULL);    
        return bResult;        
    }

    //  MTK Notice: Max Liao, 2006/11/02.
    // check device remove before handling tx ep interrupt.
    if ((!pPort->bInsert) || (bIntrUsbValue &MGC_M_INTR_DISCONNECT))
    {
        /* flush twice in case of double packet buffering */
        MGC_WriteCsr16(pBase, MGC_O_HDRC_TXCSR, bEnd,
                       MGC_M_TXCSR_FLUSHFIFO | MGC_M_TXCSR_CLRDATATOG);

        MGC_WriteCsr16(pBase, MGC_O_HDRC_TXCSR, bEnd,
                       MGC_M_TXCSR_FLUSHFIFO | MGC_M_TXCSR_CLRDATATOG);
        MUSB_DIAG_STRING(1, "TX device removed !");
        return bResult;
    }

#ifndef MUSB_DISABLE_DB_DYNFIFO
    if (MGC_bDBFifoEnable)
    {
        /* Notice: double packet buffer fifo need to check fifo empty */  
         while ((MGC_ReadCsr16(pBase, MGC_O_HDRC_TXCSR, bEnd)) & 
            (MGC_M_TXCSR_FIFONOTEMPTY|MGC_M_TXCSR_TXPKTRDY))
         {

            /*
            LOG(0, "TxCSR FIFO not empty 0x%X.\n", 
                (MGC_ReadCsr16(pBase, MGC_O_HDRC_TXCSR, bEnd)));
            */
            return bResult;
        }
    }
#endif

    if (pPort->bIsHost)
    {
        /* check for stall */
        if (wVal & MGC_M_TXCSR_H_RXSTALL)
        {
            status = (uint8_t)MUSB_STATUS_STALLED;
            /* reset error bits */
            
            //  MTK Notice: Max Liao, 2006/11/27.
            //  clear ep resource including DMA when stall.	
            u4Ret = MGC_HdrcFlushEndpoint(pPort, pEnd, MUSB_DIR_OUT, FALSE);
            UNUSED(u4Ret);            
        }
        /* check for tx error*/
        if (wVal & MGC_M_TXCSR_H_ERROR)
        {        
            LOG(0, "TX error.\n", NULL);
            MGC_HdrcIrpStatusChanged(pPort, pEnd, TRUE);
            
            status = (uint8_t)MUSB_STATUS_TRANSMIT_ERROR;

            /* reset error bits by flush fifo */
            MGC_WriteCsr16(pBase, MGC_O_HDRC_TXCSR, bEnd, MGC_M_TXCSR_FLUSHFIFO);            
            MGC_WriteCsr16(pBase, MGC_O_HDRC_TXCSR, bEnd, MGC_M_TXCSR_FLUSHFIFO);    
            #ifndef UNIFIED_USB
                MGC_WriteCsr16(pBase, MGC_O_HDRC_TXCSR, bEnd, MGC_M_TXCSR_MODE);
            #endif
        }
        /* check for NAK timeout */
        if (wVal &MGC_M_TXCSR_H_NAKTIMEOUT)
        {
            LOG(0, "TX NAK error.\n", NULL);
            MGC_HdrcIrpStatusChanged(pPort, pEnd, TRUE);
            
            status = (uint8_t)MUSB_STATUS_NACK_LIMIT;

            /* reset error bits by flush fifo */
            MGC_WriteCsr16(pBase, MGC_O_HDRC_TXCSR, bEnd, MGC_M_TXCSR_FLUSHFIFO);
            MGC_WriteCsr16(pBase, MGC_O_HDRC_TXCSR, bEnd, MGC_M_TXCSR_FLUSHFIFO);    
            #ifndef UNIFIED_USB
                MGC_WriteCsr16(pBase, MGC_O_HDRC_TXCSR, bEnd, MGC_M_TXCSR_MODE);
            #endif
        }

        if (status)
        {
            if (pEnd->pTxIrp)
            {
                bResult = MGC_CompleteIrp(pItem, pEnd, (uint8_t)status, TRUE, pEnd->pTxIrp);
            }
            /* start next IRP if any */
            if (pEnd->bStopTx)
            {
                pEnd->bIsHalted = TRUE;
            }
        }
        else
        {
            if (pEnd)
            {

#ifdef MUSB_DMA

                if (pEnd->pDmaChannel)
                {
                    if (MUSB_DMA_STATUS_FREE == pPort->pDmaController->pfDmaGetChannelStatus(
                        pEnd->pDmaChannel))
                    {
                        pEnd->dwTxOffset += pEnd->pDmaChannel->dwActualLength;
                    }
                }
                else
#endif

                {
                    pEnd->dwTxOffset += pEnd->dwTxSize;
                }
                bResult = MGC_PipeTransmitReady(pPort, pEnd);
            }
            else
            {
                bResult = TRUE;
                status = (uint8_t)MUSB_STATUS_INTERNAL_ERROR;
            }

            if (bResult)
            {

    #ifdef MUSB_DMA

                MUSB_ASSERT(pEnd);

                if (pEnd->pDmaChannel)
                {
                    MUSB_ASSERT(pPort->pDmaController);
                    MUSB_ASSERT(pPort->pDmaController->pfDmaReleaseChannel);
                    pPort->pDmaController->pfDmaReleaseChannel(pEnd->pDmaChannel);
                    pEnd->pDmaChannel = NULL;
                }
                /* clear DMA bits */
                wVal = MGC_ReadCsr16(pBase, MGC_O_HDRC_TXCSR, bEnd);
                if (wVal & 
                    (MGC_M_TXCSR_AUTOSET | MGC_M_TXCSR_DMAENAB | MGC_M_TXCSR_DMAMODE))
                {
                    wVal &= (uint16_t)(~(MGC_M_TXCSR_AUTOSET | MGC_M_TXCSR_DMAENAB | MGC_M_TXCSR_DMAMODE));

					MU_MB();
					
                    MGC_WriteCsr8(pBase, (MGC_O_HDRC_TXCSR + 1), bEnd, wVal >> 8);
                }                
    #endif

                bResult = MGC_CompleteIrp(pItem, pEnd, (uint8_t)status, TRUE, pEnd->pTxIrp);
                /* start next IRP if any */
                if (pEnd->bStopTx)
                {
                    pEnd->bIsHalted = TRUE;
                }
            }
        }            
    }
#ifdef MUSB_FUNCTION        
    else
    {
        /* Function role */

        if (wVal & MGC_M_TXCSR_P_SENTSTALL)
        {
            wVal &= ~MGC_M_TXCSR_P_SENTSTALL;

            MGC_WriteCsr16(pBase, MGC_O_HDRC_TXCSR, bEnd, wVal);
            return bResult;
        }

        if (wVal & MGC_M_TXCSR_P_UNDERRUN)
        {
            wVal &= ~MGC_M_TXCSR_P_UNDERRUN;
            MGC_WriteCsr16(pBase, MGC_O_HDRC_TXCSR, bEnd, wVal);
        }
        if (pEnd)
        {

#ifdef MUSB_DMA

            if (pEnd->pDmaChannel)
            {
                if (MUSB_DMA_STATUS_FREE == pPort->pDmaController->pfDmaGetChannelStatus(
                    pEnd->pDmaChannel))
                {
                    pEnd->dwTxOffset += pEnd->pDmaChannel->dwActualLength;
                }

                pPort->pDmaController->pfDmaReleaseChannel(pEnd->pDmaChannel);
                pEnd->pDmaChannel = NULL;
            }
            else
#endif

            {
                pEnd->dwTxOffset += pEnd->dwTxSize;
            }
            if (pEnd->pTxIrp)
            {
                bResult = MGC_PipeTransmitReady(pPort, pEnd);
                if (bResult)
                {

#ifdef MUSB_DMA
                    /* clear DMA bits */

                    MGC_WriteCsr16(pBase, MGC_O_HDRC_TXCSR, bEnd, 0);
#endif

                    bResult = MGC_CompleteIrp(pItem, pEnd, (uint8_t)status, TRUE, pEnd->pTxIrp);
                }
            }
        }
    }
#endif    
    return bResult;
}


uint8_t MGC_HdrcServiceReceiveReady(MGC_Port *pPort, uint16_t wEndIndex, MGC_BsrItem *pItem)
{
    uint16_t wVal;

    MGC_EndpointResource *pEnd;
    uint16_t wRxCount;
    uint8_t bEnd = (uint8_t)wEndIndex;
    uint8_t bResult = FALSE;
    uint32_t status = MUSB_STATUS_OK;
    MGC_Controller *pController;
    uint8_t *pBase;
    uint8_t bIntrUsbValue;
    MUSB_Irp *pIrp = NULL;
    uint32_t u4Ret;
#ifdef MUSB_DMA
    MUSB_DmaChannelStatus eDmastatus;
    MUSB_DmaController *pDmaController = pPort->pDmaController;
    MUSB_DmaChannel *pDmaChannel;
    uint8_t bDmaOk;
    MUSB_SystemServices *pServices;
    uint8_t *pBuffer = NULL;
    uint8_t bAllowDma = FALSE;
#endif
#ifdef MUSB_ISO
    MUSB_IsochIrp *pIsochIrp = NULL;
#endif

    MUSB_ASSERT(pPort);
    MUSB_ASSERT(pItem);
    pController = pPort->pController;
    MUSB_ASSERT(pController);
    pBase = (uint8_t *)pController->pControllerAddressIst;
    MUSB_ASSERT(pBase);
    bIntrUsbValue = MGC_Read8(pBase, MGC_O_HDRC_INTRUSB);

#ifdef MUSB_DMA

    pServices = pController->pSystemServices;
#endif

    pItem->bStatus = MUSB_STATUS_OK;
    pItem->bCause = MGC_BSR_CAUSE_NONE;

    MGC_SelectEnd(pBase, bEnd);
    wVal = MGC_ReadCsr16(pBase, MGC_O_HDRC_RXCSR, bEnd);

    pEnd = (MGC_EndpointResource *)MUSB_ArrayFetch(&(pPort->LocalEnds), wEndIndex);
    if (!pEnd)
    {
        LOG(0, "pEnd = NULL.\n", NULL);    
        return bResult;        
    }

    //  MTK Notice: Max Liao, 2006/11/02.
    // check device remove before handling rx ep interrupt.
    if ((!pPort->bInsert) || (bIntrUsbValue & MGC_M_INTR_DISCONNECT))
    {
        /* twice in case of double packet buffering */
        MGC_WriteCsr16(pBase, MGC_O_HDRC_RXCSR, bEnd,
                       MGC_M_RXCSR_FLUSHFIFO | MGC_M_RXCSR_CLRDATATOG);

        MGC_WriteCsr16(pBase, MGC_O_HDRC_RXCSR, bEnd,
                       MGC_M_RXCSR_FLUSHFIFO | MGC_M_RXCSR_CLRDATATOG);
        MUSB_DIAG_STRING(1, "RX device removed !");
        return bResult;
    }

    if (pPort->bIsHost)
    {
        /* check for stall */
        if (wVal & MGC_M_RXCSR_H_RXSTALL)
        {
            status = (uint32_t)MUSB_STATUS_STALLED;
            /* avoid a race with next StartRx by clearing ReqPkt */
            wVal &= ~MGC_M_RXCSR_H_REQPKT;
            MGC_WriteCsr16(pBase, MGC_O_HDRC_RXCSR, bEnd, wVal);

            //  MTK Notice: Max Liao, 2006/11/27.
            //  clear ep resource including DMA when stall.	
            u4Ret = MGC_HdrcFlushEndpoint(pPort, pEnd, MUSB_DIR_IN, FALSE);
            UNUSED(u4Ret);            
        }
        /* check for RX error */
        if (wVal&(MGC_M_RXCSR_H_ERROR))
        {
            LOG(0, "RX error.\n", NULL);

            MGC_HdrcIrpStatusChanged(pPort, pEnd, FALSE);

            status = (uint32_t)MUSB_STATUS_RECEIVE_ERROR;
            /* do the proper sequence to abort the transfer */
            wVal &= ~MGC_M_RXCSR_H_REQPKT;
            MGC_WriteCsr16(pBase, MGC_O_HDRC_RXCSR, bEnd, wVal);
        } 
        /* check for RX NAK timeout */
        else if (wVal&(MGC_M_RXCSR_DATAERR))
        {
            //LOG(0, "RX NAK timeout.\n", NULL);

			//20130310. Leon.Lin PL2303 don't need retry for data.
			if(!pPort->bIsHighSpeed)
				bRetries = MUSB_MAX_RETRIES;
			
			if(++bRetries < MUSB_MAX_RETRIES)
			{
				//LOG(0, "RX NAK retry %d.\n", bRetries);
				/* silently ignore it */
				wVal &= ~MGC_M_RXCSR_DATAERR;
				wVal &= ~MGC_M_RXCSR_RXPKTRDY;
				MGC_WriteCsr16(pBase, MGC_O_HDRC_RXCSR, bEnd, wVal | MGC_M_RXCSR_H_REQPKT);		
			}
			else
			{
				//LOG(0, "RX NAK retry Done, still fail %d.\n", bRetries);
				bRetries = 0;
	            status = (uint32_t)MUSB_STATUS_NACK_LIMIT;
	            /* do the proper sequence to abort the transfer */
	            wVal &= ~MGC_M_RXCSR_H_REQPKT;
	            MGC_WriteCsr16(pBase, MGC_O_HDRC_RXCSR, bEnd, wVal);
			}			
        }
        /* be sure a packet is ready for unloading */
        if (!status && !(wVal & MGC_M_RXCSR_RXPKTRDY))
        {
            //  MTK Notice: Max Liao, 2006/11/24.
            //  use the original error cause, not generic MUSB_STATUS_INTERNAL_ERROR. 	    
            //status = MUSB_STATUS_INTERNAL_ERROR;
            /* do the proper sequence to abort the transfer */
            wVal &= ~MGC_M_RXCSR_H_REQPKT;
            MGC_WriteCsr16(pBase, MGC_O_HDRC_RXCSR, bEnd, wVal);
        }
        if (status)
        {
            /* reset error bits */
            wVal &= ~(MGC_M_RXCSR_H_ERROR | MGC_M_RXCSR_DATAERR | MGC_M_RXCSR_H_RXSTALL
                          | MGC_M_RXCSR_RXPKTRDY);
            MGC_WriteCsr16(pBase, MGC_O_HDRC_RXCSR, bEnd, wVal);
            
            if (pEnd->pRxIrp)
            {
                bResult = MGC_CompleteIrp(pItem, pEnd, (uint8_t)status, FALSE, pEnd->pRxIrp);
            }
        }
        else
        {
	        bRetries = 0;
            if (pEnd && pEnd->pRxIrp)
            {
                /* read FIFO count and see if current IRP is satisfied */
                wRxCount = MGC_ReadCsr16(pBase, MGC_O_HDRC_RXCOUNT, bEnd);

#ifdef USB_WIFI_DEBUG          
{
        extern MUSB_Pipe _pUsbBulkInPipe;
        pIrp = (MUSB_Irp *)pEnd->pRxIrp;
        if (_pUsbBulkInPipe == pIrp->hPipe && wRxCount < 64)
        {
            if ((*((uint8_t *)pIrp->pBuffer)) > 23)
            {
                LOG(5, "STOP Here!\n");
            }
        }
}        
#endif
               
#ifdef MUSB_DMA
                /* if we were using DMA, update actual byte count */
                if (pEnd->pRxDmaChannel)
                {
                    eDmastatus = pPort->pDmaController->pfDmaGetChannelStatus(pEnd->pRxDmaChannel);
                    if (MUSB_DMA_STATUS_BUS_ABORT != eDmastatus)
                    {
                        pEnd->dwRxOffset += pEnd->pRxDmaChannel->dwActualLength;
                    }
                }
#endif
                switch (pEnd->bRxTrafficType)
                {
            #ifdef MUSB_ISO
                case MUSB_ENDPOINT_XFER_ISOC:                   
                    pIsochIrp = (MUSB_IsochIrp *)pEnd->pRxIrp;
                    bAllowDma = pIsochIrp->bAllowDma;
                    pBuffer = pIsochIrp->pBuffer + pEnd->dwRxOffset;                    
                    break;
            #endif

                case MUSB_ENDPOINT_XFER_BULK:
                case MUSB_ENDPOINT_XFER_INT:
                    pIrp = (MUSB_Irp *)pEnd->pRxIrp;
                    bAllowDma = pIrp->bAllowDma;
                    pBuffer = pIrp->pBuffer + pEnd->dwRxOffset;
                    
                    // Check if device return data length is more than user's request.
                    if ((wRxCount + pEnd->dwRxOffset) > pIrp->dwLength)
                    {                    
                        bAllowDma = FALSE;

                        if (pIrp->dwLength > pEnd->dwRxOffset)
                        {
                            wRxCount = pIrp->dwLength - pEnd->dwRxOffset;
                        }
                        else
                        {
                            LOG(1, "Device return more than expected: wRxCount = %d.\n", wRxCount);
                            LOG(1, "pEnd->dwRxOffset = %d,  pIrp->dwLength = %d.\n", 
                                pEnd->dwRxOffset,  pIrp->dwLength);

                            wRxCount = 0;
                        }
                    }                    
                    break;

                default:
                    MUSB_ASSERT(0);
                    break;
                }

#ifdef MUSB_DMA

                if ((wRxCount > pEnd->wRxPacketSize) && (bAllowDma))
                {
                    LOG(3, "wRxCount = %d > pEnd->wRxPacketSize = %d.\n", 
                        wRxCount,  pEnd->wRxPacketSize);
                    // disable DMA in this irp.
                    bAllowDma = FALSE;
                }
				
#if defined (CONFIG_ARCH_MT85XX)
                if (bAllowDma && pBuffer)
#else
                if (bAllowDma && pServices->pfSystemToBusAddress(pBuffer))
#endif
                {
                    pDmaController = pPort->pDmaController;
                    MUSB_ASSERT(pDmaController);
                    if (!pDmaController)
                    {
                        LOG(0, "pDmaController = NULL.\n", NULL);    
                        return FALSE;        
                    }
                    
                    pDmaChannel = pEnd->pRxDmaChannel;

                    /* candidate for DMA */
                    if (!pDmaChannel)
                    {
                        pDmaChannel =
                            pDmaController->pfDmaAllocateChannel(pDmaController->pPrivateData, bEnd,
                                                                 FALSE, pEnd->bRxTrafficType,
                                                                 pEnd->wRxPacketSize);
                        pEnd->pRxDmaChannel = pDmaChannel;
                    }
                    if (pDmaChannel)
                    {
						#if (!defined (CONFIG_ARCH_MT85XX))
						HalFlushDCacheMultipleLine((UINT32)pBuffer,MGC_MIN(wRxCount,pDmaChannel->dwMaxLength));
                        #endif
                        pDmaChannel->dwActualLength = 0L;

                        pEnd->bDmaTx = FALSE;

                        /*
                        * Speculate successful DMA programming and clear these fields if not.
                        * This is because although third-party DMACs obey the interface, 
                        * the Inventra DMAC in some cases will immediately activate, finish,
                        * and interrupt us before the RxCSR write below, and these fields
                        * are needed by its ISR.
                        */
                        //pEnd->bDmaResidual = TRUE;

                        /* attempt to program the channel */
                        MUSB_ASSERT(pDmaController->pfDmaProgramChannel);
                        bDmaOk =
                            pDmaController->pfDmaProgramChannel(pDmaChannel,
                                                                pEnd->wRxPacketSize,
                                                                0,
                                                                pBuffer,
                                                                MGC_MIN(wRxCount,
                                                                        pDmaChannel->dwMaxLength));
                        if (bDmaOk)
                        {
                            MGC_DIAG(3, pPort->pController, "DMA unload - StartRx]");
                            return 0;
                        }
                        else
                        {
                            //pEnd->bDmaResidual = FALSE;
                            MUSB_ASSERT(pDmaController->pfDmaReleaseChannel);

                            pDmaController->pfDmaReleaseChannel(pDmaChannel);
                            pEnd->pRxDmaChannel = NULL;
                        }
                    }
                }
#endif

                bResult = MGC_PipePacketReceived(pPort, pEnd, wRxCount, TRUE, pEnd->pRxIrp);
                if (bResult)
                {
                    /* curent IRP is satisfied */

#ifdef MUSB_DMA

                    if (pEnd->pRxDmaChannel)
                    {
                        MUSB_ASSERT(pDmaController);
                        MUSB_ASSERT(pDmaController->pfDmaReleaseChannel);
                        if ((pDmaController) && (pDmaController->pfDmaReleaseChannel))
                        {
                            pDmaController->pfDmaReleaseChannel(pEnd->pRxDmaChannel);
                            pEnd->pRxDmaChannel = NULL;
                        }
                    }
#endif
                    /* ack recv */
                    MU_MB();

                    MGC_WriteCsr16(pBase, MGC_O_HDRC_RXCSR, bEnd, 0);
                    /* complete IRP */
                    bResult = MGC_CompleteIrp(pItem, pEnd, (uint8_t)status, FALSE, pEnd->pRxIrp);
                }
            }
            else
            {
                bResult = TRUE;
                status = (uint32_t)MUSB_STATUS_INTERNAL_ERROR;
            }
        }
    }
#ifdef MUSB_FUNCTION         
    else
    {
        /*
        *  PERIPHERAL RX ( HOST OUT )
        */
        if (wVal & MGC_M_RXCSR_P_SENTSTALL)
        {
            wVal &= ~MGC_M_RXCSR_P_SENTSTALL;

            MGC_WriteCsr16(pBase, MGC_O_HDRC_RXCSR, bEnd, wVal);
            return bResult;
        }

        if (wVal & MGC_M_RXCSR_P_OVERRUN)
        {
            wVal &= ~MGC_M_RXCSR_P_OVERRUN;

            MGC_WriteCsr16(pBase, MGC_O_HDRC_RXCSR, bEnd, wVal);
            return bResult;
        }
        /*
        *  Fresh packet received
        */
        if (!pEnd || pEnd->bIsHalted)
        {
            /* couldn't find endpoint or it is halted */
            status = (uint32_t)(pEnd ? 0 : MUSB_STATUS_INTERNAL_ERROR);

            MGC_WriteCsr16(pBase, MGC_O_HDRC_RXCSR, bEnd, MGC_M_RXCSR_P_SENDSTALL);
            return bResult;
        }

        //MUSB_WriteConsole('%');

        //  MTK Notice: Max Liao, 2006/12/12.
        //  Peripheral mode execute pEnd->pRxIrp from MGC_HdrcStartRx().
        /* read FIFO count and see if current IRP is satisfied */
        wRxCount = MGC_ReadCsr16(pBase, MGC_O_HDRC_RXCOUNT, bEnd);
        if (pEnd->pRxIrp)
        {
            //  MTK Notice: Max Liao, 2006/12/07.
            //  support RX by DMA.

#ifdef MUSB_DMA
            /* if we were using DMA, update actual byte count */

            if (pEnd->pRxDmaChannel)
            {
                if (MUSB_DMA_STATUS_FREE == pPort->pDmaController->pfDmaGetChannelStatus(
                                                pEnd->pRxDmaChannel))
                {
                    pEnd->dwRxOffset += pEnd->pRxDmaChannel->dwActualLength;
                //pEnd->bDmaResidual = FALSE;
                }

                pPort->pDmaController->pfDmaReleaseChannel(pEnd->pRxDmaChannel);
                pEnd->pRxDmaChannel = NULL;
            }
            /*
            if(pEnd->bDmaResidual)
            {
            return FALSE;
            }
                    */
            // rx by DMA only when receive full packet.
            if (wRxCount == pEnd->wRxPacketSize)
            {
                pIrp = (MUSB_Irp *)pEnd->pRxIrp;

                pDmaController = pPort->pDmaController;
                MUSB_ASSERT(pDmaController);
                pDmaChannel = pEnd->pRxDmaChannel;
                pBuffer = pIrp->pBuffer + pEnd->dwRxOffset;
#if defined (CONFIG_ARCH_MT85XX)
                //if (pIrp->bAllowDma && pServices->pfSystemToBusAddress(pBuffer))
                if (pIrp->bAllowDma && pBuffer)
#else
                if (pIrp->bAllowDma && pServices->pfSystemToBusAddress(pBuffer))
#endif
                {
                    /* candidate for DMA */
                    if (!pDmaChannel)
                    {
                        pDmaChannel =
                            pDmaController->pfDmaAllocateChannel(pDmaController->pPrivateData, bEnd,
                                                                 FALSE, pEnd->bRxTrafficType,
                                                                 pEnd->wRxPacketSize);
                        pEnd->pRxDmaChannel = pDmaChannel;
                    }
                    if (pDmaChannel)
                    {
                        pDmaChannel->dwActualLength = 0L;

                        pEnd->bDmaTx = FALSE;

                        /*
                        * Speculate successful DMA programming and clear these fields if not.
                        * This is because although third-party DMACs obey the interface, 
                        * the Inventra DMAC in some cases will immediately activate, finish,
                        * and interrupt us before the RxCSR write below, and these fields
                        * are needed by its ISR.
                        */
                        //pEnd->bDmaResidual = TRUE;

                        /* attempt to program the channel */
                        // rx dma mode 1.
                        pDmaChannel->bDesiredMode = 0;
                        MUSB_ASSERT(pDmaController->pfDmaProgramChannel);
                        bDmaOk =
                            pDmaController->pfDmaProgramChannel(pDmaChannel,
                                                                pEnd->wRxPacketSize,
                                                                pDmaChannel->bDesiredMode,
                                                                pBuffer,
                                                                MGC_MIN(wRxCount,
                                                                        pDmaChannel->dwMaxLength));
                        if (bDmaOk)
                        {
                            MGC_DIAG(3, pPort->pController, "DMA unload - StartRx]");
                            return 0;
                        }
                        else
                        {
                            //pEnd->bDmaResidual = FALSE;
                            MUSB_ASSERT(pDmaController->pfDmaReleaseChannel);

                            pDmaController->pfDmaReleaseChannel(pDmaChannel);
                            pEnd->pRxDmaChannel = NULL;
                        }
                    }
                }
            }
#endif

            bResult = MGC_PipePacketReceived(pPort, pEnd, wRxCount, FALSE, pEnd->pRxIrp);

            //  MTK Notice: Max Liao, 2006/12/20.
            //  Because our system is 32 bits reg write.
            //  when MGC_WriteCsr16/MGC_WriteCsr8 may clear RXPKTRDY in peripheral mode.
            //  Should clear RXPKTRDY after clearing all other bit in the same 32 bits field.
            /* clear recv condition if necessary */
            if (wRxCount < pEnd->wRxPacketSize)
            {
                MGC_WriteCsr16(pBase, MGC_O_HDRC_RXCSR, bEnd, 0);
            }
            if (bResult)
            {
                /* current IRP is satisfied */
                bResult = MGC_CompleteIrp(pItem, pEnd, (uint8_t)status, FALSE, pEnd->pRxIrp);
            }
        }
        else
        {
            // If host send null packet, just clear RXPKTRDY.
            if (wRxCount == 0)
            {
                MGC_WriteCsr16(pBase, MGC_O_HDRC_RXCSR, bEnd, 0);
            }
            else
            {
                pEnd->dwFifoRemain = wRxCount;
            }
        }
    } /* END: PERIPHERAL RX ( HOST OUT )  */    
#endif

    return bResult;
}

#ifdef MUSB_EHSET

static void MGC_HdrcResetOff(void *pParam, uint16_t wTimer)
{
    uint8_t bReg;
    MGC_Controller *pController;
    uint8_t *pBase;

    UNUSED(wTimer);
    MUSB_ASSERT(pParam);
    pController = (MGC_Controller *)pParam;
    MUSB_ASSERT(pController);
    pBase = (uint8_t *)pController->pControllerAddressIst;
    MUSB_ASSERT(pBase);

    bReg = MGC_Read8(pBase, MGC_O_HDRC_POWER);
    MGC_Write8(pBase, MGC_O_HDRC_POWER, bReg & ~MGC_M_POWER_RESET);
}

void MGC_HdrcSetPortTestMode(MGC_Port *pPort, MUSB_HsetPortMode eMode)
{
    uint8_t bReg;
    MGC_Controller *pController;
    uint8_t *pBase;
    uint16_t wReg;
    uint8_t bRet = FALSE;
    //volatile uint32_t u4Reg;

    MUSB_ASSERT(pPort);
    pController = pPort->pController;
    MUSB_ASSERT(pController);
    pBase = (uint8_t *)pController->pControllerAddressIst;
    MUSB_ASSERT(pBase);
    wReg = MGC_Read16(pBase, MGC_O_HDRC_INTRTXE);

    switch (eMode)
    {
    case MUSB_HSET_PORT_NONE:
        MGC_Write8(pBase, MGC_O_HDRC_TESTMODE, 0);
        /* re-enable endpoint 0 interrupts */
        MGC_Write16(pBase, MGC_O_HDRC_INTRTXE, wReg | 1);
        break;

    case MUSB_HSET_PORT_TEST_J:
        MGC_Write8(pBase, MGC_O_HDRC_TESTMODE, MGC_M_TEST_J);
        break;

    case MUSB_HSET_PORT_TEST_K:
        MGC_Write8(pBase, MGC_O_HDRC_TESTMODE, MGC_M_TEST_K);
        break;

    case MUSB_HSET_PORT_TEST_SE0_NAK:
        MGC_Write8(pBase, MGC_O_HDRC_TESTMODE, MGC_M_TEST_SE0_NAK);
        break;

    case MUSB_HSET_PORT_TEST_PACKET:
        MGC_SelectEnd(pBase, 0);
        // 53 bytes.
        MGC_HdrcLoadFifo(pPort, 0, sizeof(MGC_aTestPacket), MGC_aTestPacket);
        MGC_Write8(pBase, MGC_O_HDRC_TESTMODE, MGC_M_TEST_PACKET);
        MGC_WriteCsr16(pBase, MGC_O_HDRC_CSR0, 0, MGC_M_CSR0_TXPKTRDY);
        break;

    case MUSB_HSET_PORT_RESET:
        bReg = MGC_Read8(pBase, MGC_O_HDRC_POWER);
        MGC_Write8(pBase, MGC_O_HDRC_POWER, bReg | MGC_M_POWER_RESET);
        bRet = pController->pSystemServices->pfArmTimer(
            pController->pSystemServices->pPrivateData, 0, 60, FALSE, MGC_HdrcResetOff, pController);
        break;

    case MUSB_HSET_PORT_SUSPEND:
        bReg = MGC_Read8(pBase, MGC_O_HDRC_POWER);
        MGC_Write8(pBase, MGC_O_HDRC_POWER, bReg | MGC_M_POWER_SUSPENDM);
        break;

    case MUSB_HSET_PORT_RESUME:
        bReg = MGC_Read8(pBase, MGC_O_HDRC_POWER) & ~MGC_M_POWER_SUSPENDM;
        MGC_Write8(pBase, MGC_O_HDRC_POWER, bReg | MGC_M_POWER_RESUME);
        bRet = pController->pSystemServices->pfArmTimer(
            pController->pSystemServices->pPrivateData, 0, 10, FALSE, MGC_HdrcResumeOff, pController);
        break;

    case MUSB_HSET_PORT_SETUP_START:
        /* disable endpoint 0 interrupts as a cheat for single-step mode */
        MGC_Write16(pBase, MGC_O_HDRC_INTRTXE, wReg & ~0xfffe);
        MGC_SelectEnd(pBase, 0);
        MGC_WriteCsr16(pBase, MGC_O_HDRC_CSR0, 0, MGC_M_CSR0_TXPKTRDY | MGC_M_CSR0_H_SETUPPKT);
        break;

    case MUSB_HSET_PORT_SETUP_IN:
        /* disable endpoint 0 interrupts as a cheat for single-step mode */
        MGC_Write16(pBase, MGC_O_HDRC_INTRTXE, wReg & ~0xfffe);
        MGC_SelectEnd(pBase, 0);
        MGC_WriteCsr16(pBase, MGC_O_HDRC_CSR0, 0, MGC_M_CSR0_H_REQPKT);
        break;

    case MUSB_HSET_PORT_SETUP_STATUS:
        /* disable endpoint 0 interrupts as a cheat for single-step mode */
        MGC_Write16(pBase, MGC_O_HDRC_INTRTXE, wReg & ~0xfffe);
        MGC_SelectEnd(pBase, 0);
        MGC_WriteCsr16(pBase, MGC_O_HDRC_CSR0, 0, MGC_M_CSR0_H_REQPKT | MGC_M_CSR0_H_STATUSPKT);
        break;
    }
    UNUSED(bRet);
}
#endif

#ifdef CC_MT8563
uint32_t MGC_HdrcPdwncPort(MGC_Port *pPort, uint32_t powerState)
{
	MGC_Controller* pController = pPort->pController;
	uint8_t* pBase = (uint8_t*)pController->pControllerAddressIst;
	uint16_t val;
	uint32_t reg;

	if(powerState){//power on
		Printf("[USB]MTCMOS power on.\n");
		if((uint32_t)pBase == MUSB_BASE){
			Printf("[USB]MTCMOS port0 on.\n");
			reg = 0x24098;
			val = MGC_VIRT_Read32(reg);
			val |= 0x2;
			MGC_VIRT_Write32(reg, val);
			udelay(1);
			while(1){
				val = MGC_VIRT_Read32(0x2409c);
				if(val & 0x1)
					break;
			}
			Printf("[USB]MTCMOS port0 0x2409c bit1 on.\n");
			
			val = MGC_VIRT_Read32(reg);
			val |= 0x1;
			MGC_VIRT_Write32(reg, val);
			udelay(1);
			
			while(1){
				val = MGC_VIRT_Read32(0x2409c);
				if(val & 0x4)
					break;
			}
			udelay(1);
			val = MGC_VIRT_Read32(reg);
			val &= ~0x10;
			MGC_VIRT_Write32(reg, val);

			val = MGC_VIRT_Read32(reg);
			val &= ~0x8;
			MGC_VIRT_Write32(reg, val);
			
			val = MGC_VIRT_Read32(reg);
			val |= 0x4;
			MGC_VIRT_Write32(reg, val);
			
			Printf("[USB]MTCMOS port0 0x2409c bit3 on.\n");
			val = MGC_VIRT_Read32(reg);
			val &= ~0x400;
			MGC_VIRT_Write32(reg, val);

		}else if((uint32_t)pBase == MUSB_BASE2){
			Printf("[USB]MT8563 PORT1 dont has power down control.\n");
		}else if((uint32_t)pBase == MUSB_BASE3){
			Printf("[USB]MTCMOS port2 on.\n");
			reg = 0x38c;
			val = MGC_VIRT_Read32(reg);
			val |= 0x1;
			MGC_VIRT_Write32(reg,val);
			udelay(1);
			
			val = MGC_VIRT_Read32(reg);
			val |= 0x2;
			MGC_VIRT_Write32(reg,val);
			udelay(1);
			
			reg = 0x3ac;
			val = MGC_VIRT_Read32(reg);
			val &= ~0x1;
			MGC_VIRT_Write32(reg,val);
			udelay(1);
			
			reg = 0x38c;
			val = MGC_VIRT_Read32(reg);
			val |= 0x4;
			MGC_VIRT_Write32(reg,val);
			
			val = MGC_VIRT_Read32(reg);
			val &= ~0x8;
			MGC_VIRT_Write32(reg,val);
			
			val = MGC_VIRT_Read32(reg);
			val &= ~0x10;
			MGC_VIRT_Write32(reg,val);
		}else{
			Printf("[USB]error port.\n");
		}
	}else{
		Printf("[USB]MTCMOS power off.\n");
		if((uint32_t)pBase == MUSB_BASE){
			Printf("[USB]MTCMOS port0 off.\n");
			reg = 0x24098;
			val = MGC_VIRT_Read32(reg);
			val |= 0x400;
			MGC_VIRT_Write32(reg, val);
			
			val = MGC_VIRT_Read32(reg);
			val |= 0x10;
			MGC_VIRT_Write32(reg, val);

			val = MGC_VIRT_Read32(reg);
			val |= 0x8;
			MGC_VIRT_Write32(reg, val);

			val = MGC_VIRT_Read32(reg);
			val &= ~0x4;
			MGC_VIRT_Write32(reg, val);

			val = MGC_VIRT_Read32(reg);
			val &= ~0x1;
			MGC_VIRT_Write32(reg, val);
			
			val = MGC_VIRT_Read32(reg);
			val &= ~0x2;
			MGC_VIRT_Write32(reg, val);
		}else if((uint32_t)pBase == MUSB_BASE2){
			Printf("[USB]MT8563 PORT1 dont has power down control.\n");
		}else if((uint32_t)pBase == MUSB_BASE3){
			Printf("[USB]MTCMOS port2 off.\n");
			reg = 0x38c;
			val = MGC_VIRT_Read32(reg);
			val |= 0x10;
			MGC_VIRT_Write32(reg,val);
			
			val = MGC_VIRT_Read32(reg);
			val |= 0x8;
			MGC_VIRT_Write32(reg,val);
			
			val = MGC_VIRT_Read32(reg);
			val &= ~0x4;
			MGC_VIRT_Write32(reg,val);
			
			reg = 0x3ac;
			val = MGC_VIRT_Read32(reg);
			val |= 0x1;
			MGC_VIRT_Write32(reg,val);
			
			reg = 0x38c;
			val = MGC_VIRT_Read32(reg);
			val &= ~0x2;
			MGC_VIRT_Write32(reg,val);
			
			val = MGC_VIRT_Read32(reg);
			val &= ~0x1;
			MGC_VIRT_Write32(reg,val);
		}else{
			Printf("[USB]error port.\n");
		}
	}
	
	return 0;
}
#endif
void MGC_HdrcLoadFifo(MGC_Port *pPort, uint8_t bEnd, uint32_t dwCount, const uint8_t *pSource)
{
    uint32_t dwDatum = 0;
	MGC_EnumerationData *pEnumData;
	MUSB_Device *pDevice;
    MGC_Controller *pController;
    uint8_t *pBase;
    uint8_t bFifoOffset = MGC_FIFO_OFFSET(bEnd);
    uint32_t dwBufSize = 4;

    MUSB_ASSERT(pPort);
    pController = pPort->pController;
    MUSB_ASSERT(pController);
    pBase = (uint8_t *)pController->pControllerAddressIst;
    MUSB_ASSERT(pBase);
	pEnumData = &(pPort->EnumerationData);
	MUSB_ASSERT(pEnumData);	
	pDevice = (pEnumData->pDevice);
	//MUSB_ASSERT(pDevice);

    //  MTK Notice: Max Liao, 2006/10/31.
    //  do not handle zero length data.
    if (dwCount == 0)
    {
        return;
    }

    LOG(6, "[USB FIFO]@0x%08X Load   %d\n", (UINT32)pSource, dwCount);    

#if !defined (CONFIG_ARCH_MT85XX)
	#ifdef CC_SUPPORT_STR
	// 20130310 Leon.Lin, bEnd = 0 can't add it, the USB pl2303 tx will be cleared when need bulk out 
	// data, there is not BULK OUT command from Host
	if((bEnd) \
		&& ((pDevice->DeviceDescriptor.idVendor != 0x067B) \
		&& (pDevice->DeviceDescriptor.idProduct != 0x2303))\
		)
	{
		//flush FIFO before write data to FIFO
		MGC_WriteCsr16(pBase, MGC_O_HDRC_TXCSR, bEnd, MGC_M_TXCSR_FLUSHFIFO);			 
		MGC_WriteCsr16(pBase, MGC_O_HDRC_TXCSR, bEnd, MGC_M_TXCSR_FLUSHFIFO);  
	}
	#endif
#endif //#if !defined (CONFIG_ARCH_MT85XX)

    MUSB_ASSERT(pSource);    

    /* byte access for unaligned */
    if ((dwCount > 0) && ((uint32_t)pSource & 3))
    {
        while (dwCount)
        {
         #ifdef UNIFIED_USB
     		if (3 == dwCount || 2 == dwCount)
     		{
	 			dwBufSize = 2;     		
     		    MU_MB();
     			// set FIFO byte count.
     			MGC_FIFO_CNT(pBase, M_REG_FIFOBYTECNT, 1);
     		}
			else if(1 == dwCount)
			{
	 			dwBufSize = 1;	 		
	 			// set FIFO byte count.
	 			MGC_FIFO_CNT(pBase, M_REG_FIFOBYTECNT, 0);			   
				MU_MB();
			}
     		
     		MUSB_MemCopy((VOID *)&dwDatum, (const VOID *)pSource, dwBufSize);
			MU_MB();
     		
     		MGC_Write32(pBase, bFifoOffset, dwDatum);
     		
     		dwCount -= dwBufSize;
     		pSource += dwBufSize;
         #else
            if (dwCount < 4)
            {
                dwBufSize = dwCount;

                // set FIFO byte count.
                MGC_FIFO_CNT(pBase, M_REG_FIFOBYTECNT, dwCount);
            }

            MUSB_MemCopy((VOID *)&dwDatum, (const VOID *)pSource, dwBufSize);

            MGC_Write32(pBase, bFifoOffset, dwDatum);

            dwCount -= dwBufSize;
            pSource += dwBufSize;
         #endif	
        }
    }
    else /* word access if aligned */
    {
        while ((dwCount > 3) && !((uint32_t)pSource & 3))
        {
            MU_MB();
            MGC_Write32(pBase, bFifoOffset, *((uint32_t *)((void *)pSource)));

            pSource += 4;
            dwCount -= 4;
        }
        #ifdef UNIFIED_USB
        if (3 == dwCount || 2 == dwCount)
        {
            MUSB_ASSERT(dwCount < 4);

            MU_MB();
			
            // set FIFO byte count.
            MGC_FIFO_CNT(pBase, M_REG_FIFOBYTECNT, 1);

			MU_MB();

            MGC_Write32(pBase, bFifoOffset, *((uint32_t *)((void *)pSource)));
			
			dwCount -= 2;
			pSource += 2;
        }
		
		if(1 == dwCount)
		{
          MU_MB();
		  
		  MGC_FIFO_CNT(pBase, M_REG_FIFOBYTECNT, 0);
		  if((uint32_t)pSource & 3)
		  {
     		
     		MUSB_MemCopy((VOID *)&dwDatum, (const VOID *)pSource, 1);

			MU_MB();
     		
     		MGC_Write32(pBase, bFifoOffset, dwDatum);		    
		  }
		  else
		  {
		    MU_MB();
			
            MGC_Write32(pBase, bFifoOffset, *((uint32_t *)((void *)pSource)));		    
		  }
		}

        #else
        if (dwCount > 0)
        {
            MUSB_ASSERT(dwCount < 4);

            // set FIFO byte count.
            MGC_FIFO_CNT(pBase, M_REG_FIFOBYTECNT, dwCount);

            MGC_Write32(pBase, bFifoOffset, *((uint32_t *)((void *)pSource)));
        }
        #endif
    }

	MU_MB();
	
#ifdef UNIFIED_USB
    MGC_FIFO_CNT(pBase, M_REG_FIFOBYTECNT, 2);
#else 
    MGC_FIFO_CNT(pBase, M_REG_FIFOBYTECNT, 4);
#endif

    MU_MB();

    UNUSED(pDevice);

    return;
}

void MGC_HdrcUnloadFifo(MGC_Port *pPort, uint8_t bEnd, uint32_t dwCount, uint8_t *pDest)
{
    uint32_t dwDatum = 0;

    MGC_Controller *pController;
    uint8_t *pBase;
    uint8_t bFifoOffset = MGC_FIFO_OFFSET(bEnd);
    uint32_t i;

    //  MTK Notice: Max Liao, 2006/10/31.
    //  do not handle zero length data.
    if (dwCount == 0)
    {
        return;
    }

    LOG(6, "[USB FIFO]@0x%08X Unload %d\n", (UINT32)pDest, dwCount);    

    MUSB_ASSERT(pPort);
    MUSB_ASSERT(pDest);
    pController = pPort->pController;
    MUSB_ASSERT(pController);
    pBase = (uint8_t *)pController->pControllerAddressIst;
    MUSB_ASSERT(pBase);

    if (((uint32_t)pDest) & 3)
    {
        /* byte access for unaligned */
        while (dwCount > 0)
        {
            if (dwCount < 4)
            {
   #ifdef  UNIFIED_USB
               if(3 == dwCount || 2 == dwCount)
               {
                   MU_MB();
			   
				   MGC_FIFO_CNT(pBase, M_REG_FIFOBYTECNT, 1);

                   MU_MB();
				   
				   dwDatum = MGC_Read32(pBase, bFifoOffset);
				   
				   for (i = 0; i < 2; i++)
				   {
					   *pDest++ = ((dwDatum >> (i *8)) & 0xFF);
				   }
				   
                   dwCount -=2;
              }

			  if(1 == dwCount) 
			  {
			      MU_MB();
				  
			    MGC_FIFO_CNT(pBase, M_REG_FIFOBYTECNT, 0);

				  MU_MB();
				  
                dwDatum = MGC_Read32(pBase, bFifoOffset);
				*pDest++ = (dwDatum  & 0xFF);
				 dwCount -= 1;
			  }
			   
               MU_MB();
			   
			   // set FIFO byte count = 4 bytes.
			   MGC_FIFO_CNT(pBase, M_REG_FIFOBYTECNT, 2);
			   
			   dwCount = 0;

   #else

   
                // set FIFO byte count.
                MGC_FIFO_CNT(pBase, M_REG_FIFOBYTECNT, dwCount);

                dwDatum = MGC_Read32(pBase, bFifoOffset);

                for (i = 0; i < dwCount; i++)
                {
                    *pDest++ = ((dwDatum >> (i *8)) & 0xFF);
                }

                // set FIFO byte count = 4 bytes.
                MGC_FIFO_CNT(pBase, M_REG_FIFOBYTECNT, 4);
                dwCount = 0;

	#endif			
            }
            else
            {
                MU_MB();
                dwDatum = MGC_Read32(pBase, bFifoOffset);

                for (i = 0; i < 4; i++)
                {
                    *pDest++ = ((dwDatum >> (i *8)) & 0xFF);
                }
                dwCount -= 4;
            }
        }
    }
    else
    {
        /* word access if aligned */
        while (dwCount >= 4)
        {
            MU_MB();
            *((uint32_t *)((void *)pDest)) = MGC_Read32(pBase, bFifoOffset);

            pDest += 4;
            dwCount -= 4;
        }
        if (dwCount > 0)
        {
        #ifdef UNIFIED_USB
		    if(3 == dwCount ||2 == dwCount )
		    {
               MU_MB();
			   
               MGC_FIFO_CNT(pBase, M_REG_FIFOBYTECNT, 1);

			   MU_MB();
			   
			   dwDatum = MGC_Read32(pBase, bFifoOffset);
			    for (i = 0; i < 2; i++)
         		{
         			*pDest++ = ((dwDatum >> (i *8)) & 0xFF);
         		}
				dwCount -= 2;
			}

			if(1 == dwCount)
			{
			   MU_MB();
			   
               MGC_FIFO_CNT(pBase, M_REG_FIFOBYTECNT,0);

			   MU_MB();
			   
			   dwDatum = MGC_Read32(pBase, bFifoOffset);
			   
			    *pDest++ = (dwDatum & 0xFF);
				dwCount -= 1;			   
			}
			
    		// set FIFO byte count = 4 bytes.
    		MU_MB();
			
    		MGC_FIFO_CNT(pBase, M_REG_FIFOBYTECNT, 2);

		#else
            // set FIFO byte count.
            MGC_FIFO_CNT(pBase, M_REG_FIFOBYTECNT, dwCount);

            dwDatum = MGC_Read32(pBase, bFifoOffset);

            for (i = 0; i < dwCount; i++)
            {
                *pDest++ = ((dwDatum >> (i *8)) & 0xFF);
            }

            // set FIFO byte count = 4 bytes.
            MGC_FIFO_CNT(pBase, M_REG_FIFOBYTECNT, 4);

		#endif	
        }
    }
	
	MU_MB();

    return;
}

uint8_t MGC_HdrcDmaChannelStatusChanged(void *pPrivateData, uint8_t bLocalEnd, uint8_t bTransmit)
{
#ifdef MUSB_DMA

    MGC_BsrItem qItem;

    uint8_t bQueue = FALSE;
    MGC_EndpointResource *pEnd;
    MGC_Port *pPort;
    MGC_Controller *pController;
    MUSB_SystemServices *pServices;

    MUSB_ASSERT(pPrivateData);
    pPort = (MGC_Port *)pPrivateData;
    pController = pPort->pController;
    MUSB_ASSERT(pController);
    pServices = pController->pSystemServices;
    MUSB_ASSERT(pServices);

    pEnd = (MGC_EndpointResource *)MUSB_ArrayFetch(&(pPort->LocalEnds), bLocalEnd);
    if (!pEnd)
    {
        LOG(0, "pEnd = NULL.\n", NULL);    
        return FALSE;        
    }

    if ((bTransmit && !pEnd->pDmaChannel) || (!bTransmit && !pEnd->pRxDmaChannel))
    {
        return FALSE;
    }

    /* TODO: something more specific might be better */
    if (bTransmit)
    {
        bQueue = MGC_HdrcServiceTransmitAvail(pPort, bLocalEnd, &qItem);
        if (bQueue)
        {
            qItem.bLocalEnd = bLocalEnd;
            VERIFY(pServices->pfQueueBackgroundItem(pServices->pPrivateData, &qItem));
        }
    }
    else
    {
        //  MTK Notice: Max Liao, 2006/12/20.
        //  MGC_HdrcDmaChannelStatusChanged() is not use in RX.    
        MUSB_ASSERT(0);
    }

    pPort->bDmaCompleted = bQueue;

    /* restore index */
    //MGC_Write8(pBase, MGC_O_HDRC_INDEX, bIndex);

    return bQueue;
#else

    return FALSE;
#endif

}
#ifdef MUSB_POWERDOWN
uint32_t MUSB_PowerDownSet(uint32_t u4Case)
{
	uint32_t u4Reg;
	uint32_t pBase;
	pBase = 0xf0050000;
	
	switch(u4Case)
	{
		case 0x01:
		case 0x02:
			if(MGC_POWERDWN_CASE == 0)
			{
				if(u4Case == 0x01)
				{
					// enable POWERDWN_INT_UNMASK
					u4Reg = MGC_Read8(pBase,M_REG_INTRLEVEL1EN);
					u4Reg |=  0x800;
					MGC_Write8(pBase, M_REG_INTRLEVEL1EN, (uint8_t)u4Reg);
				}
				// Enter USB PHY Suspend mode
				u4Reg = MGC_PHY_Read32(pBase,(uint32_t)0x68);
				u4Reg |=  0x00040000; 
				MGC_PHY_Write32(pBase, (uint32_t)0x68, u4Reg);

				// Turn Off USB Clk			
				u4Reg = IO_READ32(0xf000d278,0);
				u4Reg &= ( ~0x0f000000);
				IO_WRITE32(0xf000d278, 0, u4Reg);

				Printf("--USB Power Down State Case %d--\n", u4Case);
				MGC_POWERDWN_CASE = u4Case;
			}
			else
			{
				if(MGC_WAKEUP_CASE)
				{
					// Turn On USB Clk			
					u4Reg = IO_READ32(0xf000d278,0);
					u4Reg |= (0x0f000000);
					IO_WRITE32(0xf000d278, 0, u4Reg);	

					// Exit USB PHY Suspend mode
					u4Reg = MGC_PHY_Read32(pBase,(uint32_t)0x68);
					u4Reg |=  0x00040000; 
					MGC_PHY_Write32(pBase, (uint32_t)0x68, u4Reg);

					MGC_WAKEUP_CASE = 0;
					MGC_POWERDWN_CASE = 0;
					Printf("--USB Power Down exit--\n");
				}
			}
			break;
		case 0x03:
			if(MGC_POWERDWN_CASE == 0)
			{
				// enable POWERDWN_INT 0 
				u4Reg = MGC_Read8(pBase,0xa8);
				u4Reg |=  0x800;
				MGC_Write8(pBase, 0xa8, (uint8_t)u4Reg);

				// enable POWERDWN_INT_UNMASK
				u4Reg = MGC_Read8(pBase,M_REG_INTRLEVEL1EN);
				u4Reg |=  0x800;
				MGC_Write8(pBase, M_REG_INTRLEVEL1EN, (uint8_t)u4Reg);


				// Turn Off USB Clk			
				u4Reg = IO_READ32(0xf000d278,0);
				u4Reg &= ( ~0x0f000000);
				IO_WRITE32(0xf000d278, 0, u4Reg);

				Printf("--USB Power Down State Case: %d--\n", u4Case);
				MGC_POWERDWN_CASE = u4Case;
			}
			else
			{
				if(MGC_WAKEUP_CASE)
				{
					// Turn On USB Clk			
					u4Reg = IO_READ32(0xf000d278,0);
					u4Reg |= (0x0f000000);
					IO_WRITE32(0xf000d278, 0, u4Reg);	

					// Exit USB PHY Suspend mode
					u4Reg = MGC_PHY_Read32(pBase,(uint32_t)0x68);
					u4Reg |=  0x00040000; 
					MGC_PHY_Write32(pBase, (uint32_t)0x68, u4Reg);

					MGC_WAKEUP_CASE = 0;
					MGC_POWERDWN_CASE = 0;
					Printf("--USB Power Down exit--\n");
				}
			}
			break;			
	}
	return 0;
}
#endif

#endif /* HDRC enabled */
