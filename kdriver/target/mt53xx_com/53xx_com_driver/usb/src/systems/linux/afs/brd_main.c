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
 * uC/OS target-specific code for any target supported by the ARM Firmware Suite
 * $Revision: #1 $
 */

#include "mu_cdi.h"
#include "mu_diag.h"
#include "mu_mem.h"
#include "mu_sys.h"
#include "brd_cnf.h"
#include "brd_main.h"
#include "mu_hdrdf.h"

/***************************** TYPES ******************************/
/**
 * MGC_AfsUds.
 * Board-specific UDS instance data.
 * @field pfIsr ISR
 * @field pIsrParam parameter to pass controller ISR
 * @field hTimerHdl: MTK OSAI handler. 
 * @field wIrq interrupt number
 * @field wTimerCount how many wrappers
 * @field bIndex our index into the global array
 */
typedef struct
{
    MUSB_pfUcosIsr pfIsr;
    void *pIsrParam;
    HANDLE_T hTimerHdl;    
    uint16_t wIrq;
    uint8_t bIndex;
} MGC_AfsUds;

/*************************** FORWARDS *****************************/


/**************************** GLOBALS *****************************/

/** since AFS doesn't allow for instance data on ISRs or timer callbacks */
static MGC_AfsUds _arMGC_apAfsUds[MUSB_MAX_CONTROLLERS];

//  add for debug memory allocation and free.
#ifdef MUSB_MEMDEBUG

MGC_AfsMem MGC_Mem[MGC_MEM_ALLOC_ITEM];
uint32_t MGC_Mem_Index;
uint32_t MGC_Mem_Size;

#endif

/*************************** FUNCTIONS ****************************/
static void MGC_AfsUdsIsr(UINT16 wInterruptNumber)
{
    uint8_t bIndex;
    MGC_AfsUds *pUds;
    int32_t result = -1;

    for (bIndex = 0; bIndex < MGC_bSystemCount; bIndex++)
    {
        MUSB_ASSERT(bIndex < (uint8_t)MUSB_MAX_CONTROLLERS);
        if (bIndex < (uint8_t)MUSB_MAX_CONTROLLERS)
        {
            pUds = &_arMGC_apAfsUds[bIndex];
            if ((uint16_t)wInterruptNumber == pUds->wIrq)
            {
                result = pUds->pfIsr(pUds->pIsrParam);
                UNUSED(result);
                return;
            }
        }
    }
}

static uint8_t MUSB_BoardSoftReset(void* pBase)
{
    uint32_t u4Reg = 0;
    uint8_t nDevCtl = 0;
    MUSB_ASSERT(pBase);

    //USB DMA enable
    #if defined (CC_MT5880)
    u4Reg = 0x00cfff00;
    IO_WRITE32(0xf0061300, 0, u4Reg);
    #elif defined (CC_MT5881)
    u4Reg= IO_READ32(0xf0061200, 0);
    u4Reg |= 0x00cfff00;
    IO_WRITE32(0xf0061200, 0, u4Reg);
    #elif defined(CC_MT5398)
    u4Reg = 0x00cfff00;
    IO_WRITE32(0xf0060200, 0, u4Reg);
    #elif defined(CC_MT5399) || defined(CC_MT5890) || defined(CC_MT5861)
    u4Reg = 0x00cfff00;
    IO_WRITE32(0xf0061a00, 0, u4Reg);
	#elif defined(CC_MT5882)
	u4Reg = 0x00cfff00;
    IO_WRITE32(0xf0061b00, 0, u4Reg);
    #endif
	
	#if !defined(CC_MT5890)
    // VRT insert R enable 
    u4Reg = MGC_PHY_Read32(pBase,M_REG_PHYC0);
    u4Reg |= 0x4000;
    MGC_PHY_Write32(pBase, M_REG_PHYC0, u4Reg);
	#endif

    //TX_Current_EN setting 01 , 25:24 = 01 //enable TX current when
    //EN_HS_TX_I=1 or EN_HS_TERM=1
    u4Reg = MGC_PHY_Read32(pBase, 0x1C);	
    u4Reg |= (uint32_t)0x01000000;	  
    MGC_PHY_Write32(pBase, 0x1C, u4Reg);

    //Soft Reset, RG_RESET for Soft RESET
    u4Reg = MGC_PHY_Read32(pBase,(uint32_t)0x68);
    u4Reg |=   0x00004000; 
    //MGC_PHY_Write32(pBase, (uint32_t)0x68, u4Reg);
    u4Reg &=  ~0x00004000; 
    MGC_PHY_Write32(pBase, (uint32_t)0x68, u4Reg);

	#if defined(CC_MT5890)
	//add for Oryx USB20 PLL fail
	u4Reg = MGC_PHY_Read32(pBase,(uint32_t)0x0); //RG_USB20_USBPLL_FORCE_ON=1b
	u4Reg |=   0x00008000;
	MGC_PHY_Write32(pBase, (uint32_t)0x0, u4Reg);

	u4Reg = MGC_PHY_Read32(pBase,(uint32_t)0x14); //RG_USB20_HS_100U_U3_EN=1b
	u4Reg |=   0x00000800; 
	MGC_PHY_Write32(pBase, (uint32_t)0x14, u4Reg);

    if(IS_IC_5890_ES1())
	{
		//driving adjust
		u4Reg = MGC_PHY_Read32(pBase,(uint32_t)0x04);
		u4Reg &=  ~0x00007700;
		MGC_PHY_Write32(pBase, (uint32_t)0x04, u4Reg);
	}
	#endif
	
    //otg bit setting
    u4Reg = MGC_PHY_Read32(pBase,(uint32_t)0x6C);
    u4Reg &= ~0x3f3f;
    u4Reg |=  0x3f2c;
    MGC_PHY_Write32(pBase, (uint32_t)0x6C, u4Reg);
 		
    //suspendom control
    u4Reg = MGC_PHY_Read32(pBase,(uint32_t)0x68);
    u4Reg &=  ~0x00040000; 
    MGC_PHY_Write32(pBase, (uint32_t)0x68, u4Reg);

    //disconnect threshold,set 620mV
    u4Reg = MGC_PHY_Read32(pBase,(uint32_t)0x18); //RG_USB20_DISCTH
	#if defined(CC_MT5890)
    if(IS_IC_5890_ES1())
	{
	    u4Reg &= ~0x0000000f; //0x18[3:0]
	    u4Reg |=  0x0000000b;
	}
	else
	{
		u4Reg &= ~0x000000f0; //0x18[7:4]
		u4Reg |=  0x000000b0;
	}
	#else
    u4Reg &= ~0x000f0000; //others IC is 0x18[19:16]
    u4Reg |=  0x000b0000;
	#endif
    MGC_PHY_Write32(pBase, (uint32_t)0x18, u4Reg);

    u4Reg = MGC_Read8(pBase,M_REG_PERFORMANCE3);
    u4Reg |=  0x80;
    u4Reg &= ~0x40;
    MGC_Write8(pBase, M_REG_PERFORMANCE3, (uint8_t)u4Reg);

    #if defined(CC_MT5890)
    u4Reg = MGC_Read8(pBase, (uint32_t)0x7C);
    u4Reg |= 0x04;
    MGC_Write8(pBase, 0x7C, (uint8_t)u4Reg);
    #else
    // MT5368/MT5396/MT5389 
    //0xf0050070[10]= 1 for Fs Hub + Ls Device 
    u4Reg = MGC_Read8(pBase, (uint32_t)0x71);
    u4Reg |= 0x04;
    MGC_Write8(pBase, 0x71, (uint8_t)u4Reg);
	#endif

    // This is important: TM1_EN
    // speed up OTG setting for checking device connect event after MUC_Initial().
    u4Reg = MGC_Read32(pBase,0x604);
    u4Reg |= 0x01;
    MGC_Write32(pBase, 0x604, u4Reg);
	
    // open session.
    nDevCtl = MGC_Read8(pBase, MGC_O_HDRC_DEVCTL);   
    nDevCtl |= MGC_M_DEVCTL_SESSION;        
    MGC_Write8(pBase, MGC_O_HDRC_DEVCTL, nDevCtl);

    // FS/LS Slew Rate change
    u4Reg = MGC_PHY_Read32(pBase, 0x10);
    u4Reg &= (uint32_t)(~0x000000ff);
    u4Reg |= (uint32_t)0x00000011;
    MGC_PHY_Write32(pBase, 0x10, u4Reg);

    // HS Slew Rate, RG_USB20_HSTX_SRCTRL
	#if defined(CC_MT5890)
    u4Reg = MGC_PHY_Read32(pBase, 0x14); //bit[14:12]
    if(IS_IC_5890_ES1())
	{
	    u4Reg &= (uint32_t)(~0x00007000);
	    u4Reg |= (uint32_t)0x00001000;
	}
	else
	{
	    u4Reg &= (uint32_t)(~0x00007000);
	    u4Reg |= (uint32_t)0x00004000;
	}
    MGC_PHY_Write32(pBase, 0x14, u4Reg);
	#else
    u4Reg = MGC_PHY_Read32(pBase, 0x10);
    u4Reg &= (uint32_t)(~0x00070000);
    u4Reg |= (uint32_t)0x00040000;
    MGC_PHY_Write32(pBase, 0x10, u4Reg);
	#endif
	#if defined(CC_MT5890) //usb2.0 port3 driving adjust
    u4Reg = MGC_PHY_Read32(pBase, 0x14);
    u4Reg &= ~(uint32_t)0x300000;
    u4Reg |= (uint32_t)0x10200000; //bit[21:20]: RG_USB20_DISCD[1:0]=2'b10. bit[28]: RG_USB20_DISC_FIT_EN=1'b1
    MGC_PHY_Write32(pBase, 0x14, u4Reg);
	#endif

    // This is important: TM1_EN
    // speed up OTG setting for checking device connect event after MUC_Initial().
    u4Reg = MGC_Read32(pBase, 0x604);
    u4Reg &= ~0x01;
    MGC_Write32(pBase, 0x604, u4Reg);

#if (defined(__KERNEL__) && defined(__MODEL_slt__))
	// clear the USB connect/disconnect... interrupt status, this interrupt maybe occur during loader/uboot. write 1 to clear.
	u4Reg = MGC_Read32(pBase, 0x08);	// 0x0a: MGC_O_HDRC_INTRUSB[5:4]
	MGC_Write32(pBase, 0x08, u4Reg);
#endif

    LOG(5, "[USB]Nuclues pBase = 0x%08X init ok.\n", (uint32_t)pBase);

    return TRUE;
}


void *MUSB_BoardInitController(void *pPrivateData, MUSB_pfUcosIsr pfIsr,
                               const MUSB_UcosController *pControllerInfo, uint8_t **ppBaseIsr,
                               uint8_t **ppBaseBsr)
{
    MGC_AfsUds *pUds;
    VOID (*pfnOldIsr)(UINT16);

    MUSB_ASSERT(pControllerInfo);
    UNUSED(ppBaseIsr);       
    MUSB_ASSERT(ppBaseBsr);
    
    if (!MUSB_BoardSoftReset(*ppBaseBsr))
    {
        return NULL;
    }
    
//  change to static allocation.
    MUSB_ASSERT(MGC_bSystemCount < (uint8_t)MUSB_MAX_CONTROLLERS);
    if (MGC_bSystemCount >= (uint8_t)MUSB_MAX_CONTROLLERS)
    {
        return NULL;
    }
    pUds = &_arMGC_apAfsUds[MGC_bSystemCount];

    MUSB_MemSet(pUds, 0, sizeof(MGC_AfsUds));

    pUds->wIrq = pControllerInfo->dwInfo;
    pUds->pfIsr = pfIsr;
    pUds->pIsrParam = pPrivateData;
   
    /* assign the interrupt */
    VERIFY(OSR_OK == x_reg_isr((UINT16)pUds->wIrq, MGC_AfsUdsIsr, &pfnOldIsr));
    UNUSED(pfnOldIsr);

    VERIFY(BIM_DisableIrq(pUds->wIrq));

    pUds->bIndex = MGC_bSystemCount;
    return pUds;
}

uint8_t MUSB_BoardInitTimers(void *pPrivateData)
{
    MGC_AfsUds *pUds;
    
    MUSB_ASSERT(pPrivateData);
    pUds = (MGC_AfsUds *)pPrivateData;

    VERIFY(OSR_OK == x_timer_create(&pUds->hTimerHdl));
   
    return TRUE;
}

#ifndef MUSB_CODE_SHRINK
void MUSB_BoardDestroyController(void *pPrivateData)
{
    UNUSED(pPrivateData);
}
#endif

uint8_t MUSB_BoardArmTimer(void *pPrivateData, uint16_t wIndex, uint32_t dwTime, uint8_t bPeriodic,
                           x_os_timer_cb_fct pfExpireCallback, void *pParam)
{
    MGC_AfsUds *pUds;
    TIMER_FLAG_T e_flags;

    UNUSED(wIndex);
    MUSB_ASSERT(pPrivateData);
    pUds = (MGC_AfsUds *)pPrivateData;

    LOG(9, "Timer:%d Start delay = %d ms.\n", wIndex, dwTime);
    e_flags = (bPeriodic) ? X_TIMER_FLAG_REPEAT : X_TIMER_FLAG_ONCE;
    VERIFY(OSR_OK == x_timer_start(pUds->hTimerHdl, dwTime, e_flags, pfExpireCallback, pParam));
    return TRUE;
}

void MUSB_BoardCancelTimer(void *pPrivateData, uint16_t wIndex)
{
    MGC_AfsUds *pUds;

    UNUSED(wIndex);
    MUSB_ASSERT(pPrivateData);
    pUds = (MGC_AfsUds *)pPrivateData;

    LOG(9, "Timer:%d Stop.\n", wIndex);
    
    VERIFY(OSR_OK == x_timer_stop(pUds->hTimerHdl));
}

/*
* Controller calls this to get a bus address (for DMA) from a system address
*/
void *MUSB_BoardSystemToBusAddress(const void *pSysAddr)
{
    return (void *)PHYSICAL((UINT32)pSysAddr);
}

/*
* Controller calls this to enter a lock irq
*/
uint32_t MUSB_BoardIrqLock(void *pPrivateData)
{
    uint32_t u4Ret;
    MGC_AfsUds* pUds;
    
    MUSB_ASSERT(pPrivateData);
    pUds = (MGC_AfsUds *)pPrivateData;

    // disable USB interrupt.
    u4Ret = (UINT32)BIM_IsIrqEnabled(pUds->wIrq);

    if (u4Ret)
    {
        VERIFY(BIM_DisableIrq(pUds->wIrq));
    }

    return u4Ret;
}

/*
* Controller calls this to enter a unlock irq
*/
void MUSB_BoardIrqUnlock(void *pPrivateData, uint32_t dwPrevIrq)
{
    MGC_AfsUds* pUds;
    
    MUSB_ASSERT(pPrivateData);
    pUds = (MGC_AfsUds *)pPrivateData;
   
    // interrupt is previous enable.
    if (dwPrevIrq)
    {
        // enable USB interrupt.
        VERIFY(BIM_EnableIrq(pUds->wIrq));
    }
}

/*
* Controller calls this to get irq status.
*/
uint32_t MUSB_BoardIsIrqPending(void *pPrivateData)
{
    uint32_t u4Ret;
    MGC_AfsUds* pUds;
    
    MUSB_ASSERT(pPrivateData);
    pUds = (MGC_AfsUds *)pPrivateData;

    // disable USB interrupt.
    u4Ret = (UINT32)BIM_IsIrqPending(pUds->wIrq);

    return u4Ret;
}

