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
/*********************************************************************
* LEGAL DISCLAIMER
*
* (Header of MediaTek Software/Firmware Release or Documentation)
*
* BY OPENING OR USING THIS FILE, BUYER HEREBY UNEQUIVOCALLY
* ACKNOWLEDGES AND AGREES THAT THE SOFTWARE/FIRMWARE AND ITS
* DOCUMENTATIONS ("MEDIATEK SOFTWARE") RECEIVED FROM MEDIATEK AND/OR
* ITS REPRESENTATIVES ARE PROVIDED TO BUYER ON AN "AS-IS" BASIS ONLY.
* MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR
* NONINFRINGEMENT. NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY
* WHATSOEVER WITH RESPECT TO THE SOFTWARE OF ANY THIRD PARTY WHICH
* MAY BE USED BY, INCORPORATED IN, OR SUPPLIED WITH THE MEDIATEK
* SOFTWARE, AND BUYER AGREES TO LOOK ONLY TO SUCH THIRD PARTY FOR ANY
* WARRANTY CLAIM RELATING THERETO. MEDIATEK SHALL ALSO NOT BE
* RESPONSIBLE FOR ANY MEDIATEK SOFTWARE RELEASES MADE TO BUYER'S
* SPECIFICATION OR TO CONFORM TO A PARTICULAR STANDARD OR OPEN FORUM.
*
* BUYER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND
* CUMULATIVE LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED
* HEREUNDER WILL BE, AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE
* MEDIATEK SOFTWARE AT ISSUE, OR REFUND ANY SOFTWARE LICENSE FEES OR
* SERVICE CHARGE PAID BY BUYER TO MEDIATEK FOR SUCH MEDIATEK SOFTWARE
* AT ISSUE.
*
* THE TRANSACTION CONTEMPLATED HEREUNDER SHALL BE CONSTRUED IN
* ACCORDANCE WITH THE LAWS OF THE STATE OF CALIFORNIA, USA, EXCLUDING
* ITS CONFLICT OF LAWS PRINCIPLES.
*********************************************************************/
/*********************************************************************
*  @file      utest-ETH.module
*  @brief	   unit test for USB ETH
*  @author         
*  @description    test cases to verify USB ETH
*  @date           2008.06.16
*********************************************************************/

#if 0
#include <base/utest.h>
#include <stdlib.h> 
#include <time.h>
#include <bsp/arch-mt7118/ddr_uart.h>
//#include <bsp/ddr_uarti.h>
#include <bsp/ddr_misc.h>
#include <bsp/ddr_ahbmon.h>
#include <bsp/config.h>
#include <bsp/ddr_pmu.h>

#if CACHE_ENABLE
#include <bsp/ddr_mpu.h>
#endif

#include <packetgen.h>
#include <debug.h>
#include <bsp/ddr_gpt.h>
#include <ddr_usbhost.h>
#include <musbdefs.h>
#endif



//#include "utest-usb.h"
#define UTEST_UNIUSB_C
#include "dt_debug.h"
#include "utest_uniusb.h"
#undef UTEST_UNIUSB_C

#if defined(CONFIG_ARCH_MT85XX)
#include "x_os.h"
#include "sys_config.h"
#if !CONFIG_SYS_MEM_PHASE3
#include "x_mem_phase2.h"
#else
#include "x_rtos.h"
#endif
#include "x_hal_1176.h"
#endif //#endif

#include "x_bim.h"
//#include "x_hal_1176.h"
#include "mu_mem.h"
//#include "mu_sys.h"


#define _HAL_DELAY_MS_GPTIDX    1

extern uint32_t MUSB_Sleep(uint32_t dwTime);

void USB_hal_delay_ms ( unsigned int msecs )
{   

    if ( msecs )
    {
        (void)MUSB_Sleep(msecs);
    }

    return;
}

void MGC_ResetPort(void){
	unsigned char power;

    MGC_QMU_Write8(MGC_O_HDRC_POWER, (MGC_QREG_Read8(MGC_O_HDRC_POWER) | MGC_M_POWER_RESET));
        USB_hal_delay_ms(40);
        if (MGC_QREG_Read16(MGC_O_HDRC_FRAME) == 0) {
            Printf("Success: Reset success\r\n");
            isReset = 1;
        }  
        else{
            Printf("Error: Reset not success\r\n");
            isReset = 0;
        }
        MGC_QMU_Write8(MGC_O_HDRC_POWER, (MGC_QREG_Read8(MGC_O_HDRC_POWER) & (~MGC_M_POWER_RESET)));
        USB_hal_delay_ms(60);

    power = MGC_QREG_Read8(MGC_O_HDRC_POWER);
		
    if(power & 0x10){
        Printf("HS Mode up, power[%x]\r\n", power);
    }
    else{
        Printf("HS Mode not up, power[%x]\r\n", power);
    }
}
#if !defined(CONFIG_ARCH_MT85XX)
void resetUSB(void){
    unsigned int u4Reg = 0;
    //Reset USB
    Printf("HalDisableDCache\n");
    HalDisableDCache();

    Reg_Write32(0xf0060200, 0x008f0000);

    u4Reg = Reg_Read32(MUSB_BASE + MUSB_PHYBASE + 0x68);
    //u4Reg |=   0x00004000; 
    MGC_PHY_Write32((MUSB_BASE + MUSB_PHYBASE), (uint32_t)0x68, 0x02004000);//u4Reg);
    //u4Reg &=  ~0x00004000; 
    MGC_PHY_Write32((MUSB_BASE + MUSB_PHYBASE), (uint32_t)0x68, 0x02000000);//u4Reg);

    Printf("Read OTG Reg@[0x%08X]\n", (unsigned int)((MUSB_BASE + MUSB_PHYBASE) + MUSB_PORT0_PHYBASE + 0x6C));
    u4Reg = Reg_Read32((unsigned int)((MUSB_BASE + MUSB_PHYBASE) + MUSB_PORT0_PHYBASE + 0x6C));
    u4Reg &= ~0x3f3f;
    u4Reg |=  0x3e2c;
    Printf("Set OTG bit: 0x%08X\n", u4Reg);
    Reg_Write32((unsigned int)((MUSB_BASE + MUSB_PHYBASE) + MUSB_PORT0_PHYBASE + 0x6C),u4Reg);
 		
    //suspendom control
    Printf("Read Suspendom control\n");
    Printf("Read OTG Reg@[0x%08X]\n", (unsigned int)((MUSB_BASE + MUSB_PHYBASE) + MUSB_PORT0_PHYBASE + 0x68));
    u4Reg = Reg_Read32((unsigned int)((MUSB_BASE + MUSB_PHYBASE) + MUSB_PORT0_PHYBASE + 0x68));
	u4Reg &=  ~0x00040000; 
    Printf("Read Suspendom control = %x\n", u4Reg);
    Reg_Write32((unsigned int)((MUSB_BASE + MUSB_PHYBASE) + MUSB_PORT0_PHYBASE + 0x68),u4Reg);

   
    Printf("\n");
}
#else //#if !defined(CONFIG_ARCH_MT85XX)
void resetUSB(void){
#if defined(CC_MT8580)
	unsigned int u4Reg = 0;

    u4Reg = Reg_Read32(0xfd00007C);
	u4Reg |= 0x00010000; 
    Reg_Write32(0xfd00007C, u4Reg);

    //Reset USB
    Reg_Write32(0xfd035868, Reg_Read32(0xfd035868)| 0x00004000);
    Reg_Write32(0xfd035868, Reg_Read32(0xfd035868)& ~ 0x00004000);

    //Printf("HalDisableDCache\n");
    //HalDisableDCache();

    Printf("Read OTG Reg@[0x%08X]\n", (unsigned int)((MUSB2_BASE + MUSB_PWD_PHYBASE) + MUSB_PORT0_PHYBASE + 0x6C));
    u4Reg = Reg_Read32((unsigned int)((MUSB2_BASE + MUSB_PWD_PHYBASE) + MUSB_PORT0_PHYBASE + 0x6C));
	u4Reg &= ~0x3f3f;
    u4Reg |=  0x3e2c;
    Printf("Set OTG bit: 0x%08X\n", u4Reg);
    Reg_Write32((unsigned int)((MUSB2_BASE + MUSB_PWD_PHYBASE) + MUSB_PORT0_PHYBASE + 0x6C),u4Reg);
 		
    //suspendom control
    Printf("Read Suspendom control\n");
    Printf("Read OTG Reg@[0x%08X]\n", (unsigned int)((MUSB2_BASE + MUSB_PWD_PHYBASE) + MUSB_PORT0_PHYBASE + 0x68));
    u4Reg = Reg_Read32((unsigned int)((MUSB2_BASE + MUSB_PWD_PHYBASE) + MUSB_PORT0_PHYBASE + 0x68));
	u4Reg &=  ~0x00040000; 
    Printf("Read Suspendom control = %x\n", u4Reg);
    Reg_Write32((unsigned int)((MUSB2_BASE + MUSB_PWD_PHYBASE) + MUSB_PORT0_PHYBASE + 0x68),u4Reg);
    Printf("\n");
    	
#endif
    //~ben
}

irqreturn_t HdrcIsrReset(int irq_no, void* pParam){
    unsigned char usbIntr = MGC_QREG_Read8(MGC_O_HDRC_INTRUSB);
    MGC_QMU_Write8(MGC_O_HDRC_INTRUSB, (usbIntr));
    if((usbIntr & MGC_M_INTR_CONNECT) == MGC_M_INTR_CONNECT){
        MGC_ResetPort();
    }
	#if !PORTING
    return CISR_CALL_DSR;
	#endif

return 0;
}
#endif //#if !defined(CONFIG_ARCH_MT85XX)

int disableIntr(void){
    //int temp;
    /* disable interrupts */
    MGC_QMU_Write8 (MGC_O_HDRC_INTRUSBE, 0);
    MGC_QMU_Write16 (MGC_O_HDRC_INTRTXE, 0);
    MGC_QMU_Write16 (MGC_O_HDRC_INTRRXE, 0);
    MGC_QREG_Write32(M_REG_INTRLEVEL1EN, 0);


    MGC_QMU_Write8(MGC_O_HDRC_INTRUSB, MGC_QREG_Read8(MGC_O_HDRC_INTRUSB));
    MGC_QMU_Write16(MGC_O_HDRC_INTRTX, MGC_QREG_Read16(MGC_O_HDRC_INTRTX));    
    MGC_QMU_Write16(MGC_O_HDRC_INTRRX, MGC_QREG_Read16(MGC_O_HDRC_INTRRX));    
    MGC_QMU_Write8(MGC_O_HSDMA_INTR, MGC_QREG_Read8(MGC_O_HSDMA_INTR));  
    MGC_QREG_Write32(M_REG_INTRLEVEL1, MGC_QREG_Read32(M_REG_INTRLEVEL1));

    return 0;
}

int enableUsbIntr(void){
#if !defined(CONFIG_ARCH_MT85XX)
    int temp;
    //def Leon
	temp = 0xf7;
	MGC_QMU_Write8 (MGC_O_HDRC_INTRUSBE, temp);
    MGC_QMU_Write16 (MGC_O_HDRC_INTRTXE, 0xFF);
    MGC_QMU_Write16 (MGC_O_HDRC_INTRRXE, 0xFF);
#endif
    MGC_QREG_Write32(M_REG_INTRLEVEL1EN, 0x2F);
    Printf("Enable Level1 IntrEn\n");
    Printf("USB Enable IRQ: %d\n", USB_IRQ_NO);
    BIM_EnableIrq(USB_IRQ_NO);

    return 0;
}



unsigned int getFIFOsz(unsigned int FIFOlen, int isDPB){
    int FIFOsz = 0x0;
    if(isDPB){
        FIFOsz = FIFOsz | (0x1<<4);
    }
    if(FIFOlen <= 8){
        FIFOsz = FIFOsz | 0x0;
    }
    else if(FIFOlen <= 16){
        FIFOsz = FIFOsz | 0x1;
    }
    else if(FIFOlen <= 32){
        FIFOsz = FIFOsz | 0x2;
    }
    else if(FIFOlen <= 64){
        FIFOsz = FIFOsz | 0x3;
    }
    else if(FIFOlen <= 128){
        FIFOsz = FIFOsz | 0x4;
    }
    else if(FIFOlen <= 256){
        FIFOsz = FIFOsz | 0x5;
    }
    else if(FIFOlen <= 512){
        FIFOsz = FIFOsz | 0x6;
    }
    else if(FIFOlen <= 1024){
        FIFOsz = FIFOsz | 0x7;
    }
    else if(FIFOlen <= 2048){
        FIFOsz = FIFOsz | 0x8;
    }
    else if(FIFOlen <= 4096){
        FIFOsz = FIFOsz | 0x9;
    }
    else{
        Printf("FIFO length[%d] not available", FIFOlen);
        FIFOsz = 0x0;
    }
    
    return (char)FIFOsz;
}

void SetAddress(int addr){
    #if defined SEPERATED_FADDR
    int i;
    for(i=0; i<=TX_FIFO_NUM; i++){
        MGC_QMU_Write8(MGC_O_HDRC_TXFADDR(i), addr);
    }
    for(i=1; i<=RX_FIFO_NUM; i++){
        MGC_QMU_Write8(MGC_O_HDRC_RXFADDR(i), addr);
    }
	MGC_QMU_Write8(MGC_O_HDRC_FADDR, addr);
    #endif
    #if defined GLOBAL_FADDR
    MGC_QMU_Write8(MGC_O_HDRC_FADDR, addr);
    #endif
}

void SetHubAddress(int ep_num, int addr, USB_DIR dir){
	if(ep_num == 0){
		MGC_QMU_Write8(MGC_O_HDRC_TXFADDR(0), addr);
	}
	else{
		if(dir == USB_TX){
			MGC_QMU_Write8(MGC_O_HDRC_TXFADDR(ep_num), addr);
		}
		else if(dir == USB_RX){
			MGC_QMU_Write8(MGC_O_HDRC_RXFADDR(ep_num), addr);
		}
	}
}

/***************************************************************************/
/*   EP0 Enumeration                                                                                                          */
/***************************************************************************/
int EP0ControlRequest(PMUSB_DeviceRequest pDevReq)
{
    unsigned short wAddress;

    // Set FAddr Register
    
    //MGC_QMU_Write8 (MGC_O_HDRC_FADDR, wSetAddress);
    SetAddress(wSetAddress);

    MGC_QMU_Write16 (MGC_O_HDRC_INTRTXE, 0x1e);        
    MGC_QWriteCsr16 (MGC_O_HDRC_CSR0, 0, MGC_M_CSR0_FLUSHFIFO);
    MGC_QWriteCsr16 (MGC_O_HDRC_CSR0, 0, MGC_M_CSR0_FLUSHFIFO);
    MGC_QWriteCsr8 (MGC_O_HDRC_NAKLIMIT0, 0, bTxInterval);
    MGC_QMU_Write16 (MGC_O_HDRC_INTRTXE, 0x1f);    

    SetupTxDataBuffer[0] = pDevReq->bmRequestType;
    SetupTxDataBuffer[1] = pDevReq->bRequest;
    SetupTxDataBuffer[2] = pDevReq->wValue & 0xff;
    SetupTxDataBuffer[3] = pDevReq->wValue >> 8;
    SetupTxDataBuffer[4] = pDevReq->wIndex & 0xff;
    SetupTxDataBuffer[5] = pDevReq->wIndex >> 8;
    SetupTxDataBuffer[6] = pDevReq->wLength & 0xff;
    SetupTxDataBuffer[7] = pDevReq->wLength >> 8;
          
    switch (pDevReq->bRequest) {
        case MUSB_REQ_GET_STATUS :
        {  
            QMU_HdrcLoadFifo (0, 8, SetupTxDataBuffer);   
            bEnd0DataStage = MGC_END0_DSTAGE_DATAIN;
            break;
        }
        case MUSB_REQ_CLEAR_FEATURE :
        {
            break;
        }
        case MUSB_REQ_GET_DESCRIPTOR :
        {
            QMU_HdrcLoadFifo (0, 8, SetupTxDataBuffer);   
            bEnd0DataStage = MGC_END0_DSTAGE_DATAIN;
            break;
        }
        case MUSB_REQ_SET_ADDRESS :
        {
            wAddress = wAssignAddress;
            wSetAddress = wAddress;
                
            SetupTxDataBuffer[0] = MUSB_DIR_OUT|MUSB_TYPE_STANDARD|MUSB_RECIP_DEVICE;
            SetupTxDataBuffer[1] = MUSB_REQ_SET_ADDRESS;
            SetupTxDataBuffer[2] = wAddress & 0xff;
            SetupTxDataBuffer[3] = wAddress >> 8;        
            SetupTxDataBuffer[4] = 0x00;
            SetupTxDataBuffer[5] = 0x00;
            SetupTxDataBuffer[6] = 0x00;
            SetupTxDataBuffer[7] = 0x00;
            QMU_HdrcLoadFifo (0, 8, SetupTxDataBuffer);

            bEnd0DataStage = MGC_END0_DSTAGE_NODATA;
            break;
        }
        case MUSB_REQ_GET_CONFIGURATION :
        {
            break;
        }
        case MUSB_REQ_SET_CONFIGURATION :
        {
            QMU_HdrcLoadFifo (0, 8, SetupTxDataBuffer);
            bEnd0DataStage = MGC_END0_DSTAGE_NODATA;
            break;
        }
        case 0xFE :
        {
            QMU_HdrcLoadFifo (0, 8, SetupTxDataBuffer);
            bEnd0DataStage = MGC_END0_DSTAGE_DATAIN;
            break;
        }
        default :
        {
            break;
        }
    }  
    MGC_QWriteCsr16(MGC_O_HDRC_CSR0, 0, MGC_M_CSR0_H_NO_PING | MGC_M_CSR0_H_SETUPPKT | MGC_M_CSR0_TXPKTRDY);
    return 0;
}
unsigned char ProcHostDefaultEnd(unsigned int dwCount)
{
    unsigned char bMore = FALSE;
#if 0
    DBG (6, "===> %s\r\n", __FUNCTION__);
    DBG (3, "decode_ep0stage: 0x%x\r\n", pCtrl->bEnd0Stage);	
#endif
    if (MGC_END0_DATA == bEnd0Stage && bEnd0DataStage == MGC_END0_DSTAGE_DATAIN) {
        QMU_HdrcUnloadFifo (0, dwCount, SetupRxDataBuffer + SetupRxDataCount);
        SetupRxDataCount += dwCount;
            
        if (dwCount == dwMaxPacketSizeRx) {
            bMore = TRUE;
        }
    } 
    else if (MGC_END0_DATA == bEnd0Stage && bEnd0DataStage == MGC_END0_DSTAGE_DATAOUT) {
//       QMU_HdrcLoadFifo (0, dwCount, pCtrl->SetupTxDataBuffer);
//       pCtrl->SetupTxDataCount += dwCount;
            
        if (dwCount == dwMaxPacketSizeTx) {
            bMore = TRUE;
        }
    } 
    else {
        if (MGC_END0_SETUP == bEnd0Stage && bEnd0DataStage == MGC_END0_DSTAGE_DATAIN) {
//            DBG(3, "just did setup, switching to IN\r\n");
            
            /* this means we just did setup; switch to IN */
            bEnd0Stage = MGC_END0_DATA ;
            bMore = TRUE;
        }
        else if (MGC_END0_SETUP == bEnd0Stage && bEnd0DataStage == MGC_END0_DSTAGE_DATAOUT) {           		
            bEnd0Stage = MGC_END0_DATA;            		
            QMU_HdrcLoadFifo(0, SetupTxDataCount, SetupTxDataBuffer);
            bMore = TRUE;
        }
        else if (MGC_END0_SETUP == bEnd0Stage && bEnd0DataStage == MGC_END0_DSTAGE_NODATA) {
            bEnd0Stage = MGC_END0_STATUS;
        }
    }   
    return bMore;
}


#if 0 // mhzhang
void QMU_HdrcLoadFifo(unsigned char nEnd, unsigned int dwCount, const unsigned char* pSource)
{
    unsigned int  dwIndex, dwIndex32;
    unsigned int  dwDatum;
    unsigned int  dwCount32 = ((unsigned int )pSource & 3) ? 0 : (dwCount >> 2);
    unsigned char bFifoOffset = MGC_FIFO_OFFSET(nEnd);

//    DBG (5, "LoadFifo end %d, count=%d, pSrc=%p\r\n", nEnd, dwCount, pSource);

    /* doublewords when possible */
    for(dwIndex = dwIndex32 = 0; dwIndex32 < dwCount32; dwIndex32++, dwIndex += 4)
    {
        dwDatum = *((unsigned int*)&(pSource[dwIndex]));
        MGC_QREG_Write32(bFifoOffset, dwDatum);
    }
    while(dwIndex < dwCount)
    {
        MGC_QMU_Write8(bFifoOffset, pSource[dwIndex]);
        dwIndex += 1;
    }

}

void QMU_HdrcUnloadFifo(unsigned char nEnd, unsigned int dwCount, unsigned char* pDest)
{
    unsigned int  dwIndex, dwIndex32;
    unsigned int  dwCount32 = ((unsigned int )pDest & 3) ? 0 : (dwCount >> 2);
    unsigned char bFifoOffset = MGC_FIFO_OFFSET(nEnd);

    DBG (5, "UnLoadFifo end %d, count=%d, pDest=%p\r\n", nEnd, dwCount, pDest);

    /* doublewords when possible */
    for(dwIndex = dwIndex32 = 0; dwIndex32 < dwCount32; dwIndex32++, dwIndex += 4)
    {
        *((unsigned int*)&(pDest[dwIndex])) = MGC_QREG_Read32(bFifoOffset);
    }
    while(dwIndex < dwCount)
    {
        pDest[dwIndex++] = MGC_QREG_Read8(bFifoOffset);
    }

}
#endif

int main (void)
{

    return 0;
}
