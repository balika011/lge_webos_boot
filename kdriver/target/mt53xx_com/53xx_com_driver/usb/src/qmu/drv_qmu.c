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
*  @file           ts_transfer.c
*  @brief          unit test register default value, read/write
*  @author
*  @description    test cases to test unified USB registers
*  @date           2009.10.12
*********************************************************************/

/*******************************************************************************


 * Include files 


******************************************************************************/


#include "mu_mem.h"
#if defined(CONFIG_ARCH_MT85XX)
#include "sys_config.h"
#if !CONFIG_SYS_MEM_PHASE3
#include "x_mem_phase2.h"
#else
#include "x_rtos.h"
#endif
#include "cache_operation.h"
#endif
#include "dt_debug.h"
#include "x_bim.h"


extern unsigned char isFreeGPD;
extern char ep_ioc_dis;
extern char gpd_tx_ioc_en;
extern char gpd_rx_ioc_en;
extern char ioc_test;
#if defined(CONFIG_ARCH_MT85XX)
extern char zlp_dis;
#endif


BYTE PDU_calcCksum(BYTE *data, int len);

//irqreturn_t HdrcBsrQmu(int irq_no, void* pParam);
void HdrcBsrQmu(unsigned short wInterruptNumber);
//irqreturn_t HdrcBsrDevQmu(int irq_no, void* pParam);
void HdrcBsrDevQmu(unsigned short wInterruptNumber);

void HdrcIsrMultiIxiaQMU(unsigned short wInterruptNumber);

void ixia_Qmu_handler(unsigned int wQmuVal);
void DevQmu_handler(unsigned int wQmuVal);
//VOID ep_error_check(unsigned char ep_num, USB_DIR dir);
VOID ep_error_dev_check(unsigned char ep_num, USB_DIR dir);
TGPD* GetEndGpd(unsigned char ep_num, TGPD** LastGpd, USB_DIR dir);
TGPD* GpdTxPrepare(BYTE*pBuf, unsigned int data_len, unsigned char ep_num, unsigned char target_ep, unsigned char _is_bdp);
TGPD* Prepare_Tx_GPD(TGPD* gpd, BYTE*pBuf, unsigned int data_length, unsigned char ep_num, unsigned char target_ep, unsigned char _is_bdp, unsigned char isHWO);
TGPD* GpdRxPrepare(BYTE*pBuf, unsigned int data_len, unsigned char ep_num, unsigned char target_ep, unsigned char _is_bdp);
TGPD* Prepare_Rx_GPD(TGPD*gpd, BYTE*pBuf, unsigned int data_len, unsigned char ep_num, unsigned char target_ep, unsigned char _is_bdp, unsigned char isHWO);	
void usb_start_transfer_QMU(int ep_num, int target_ep, USB_DIR dir);
void usb_insert_transfer_QMU(int ep_num, int target_ep, USB_DIR dir, unsigned char* buf, unsigned int count, unsigned char isHWO);
void usb_start_QMU(int Q_num,  USB_DIR dir, unsigned char isMT);
void usb_stop_QMU(int Q_num,  USB_DIR dir);
void usb_resume_QMU(int Q_num,  USB_DIR dir);
void Flush_QMU(int Q_num,  USB_DIR dir);
void proc_qmu_rx(int ep_num);
void proc_qmu_tx(int ep_num);
void proc_devqmu_rx(int ep_num);
void proc_devqmu_tx(int ep_num);
void proc_ixia_qmu_rx(int ep_num);
void proc_ixia_qmu_tx(int ep_num);
void proc_ixia_rx_empty(int ep_num);
void Free_BD(TGPD* gpd);

void Qmu_handler(unsigned int wIntrQMUValue);
//void DevQmu_handler(unsigned int wIntrQMUValue);
VOID ep_error_check(unsigned char ep_num, USB_DIR dir);
//VOID ep_error_dev_check(unsigned char ep_num, USB_DIR dir);
//void ixia_Qmu_handler(unsigned int wIntrQMUValue);
//void proc_qmu_tx(int ep_num);
//void proc_qmu_rx(int ep_num);
//void proc_devqmu_tx(int ep_num);
//void proc_devqmu_rx(int ep_num);
//void proc_ixia_qmu_tx(int ep_num);
//void proc_ixia_qmu_rx(int ep_num);
//void proc_ixia_rx_empty(int ep_num);
//void Free_BD(TGPD* gpd);
TGPD* GpdTxPrepare(BYTE*pBuf, unsigned int data_len, unsigned char ep_num, unsigned char target_ep, unsigned char _is_bdp);
TGPD* GpdRxPrepare(BYTE*pBuf, unsigned int data_len, unsigned char ep_num, unsigned char target_ep, unsigned char _is_bdp);
	
#define _HAL_DELAY_MS_GPTIDX    1
extern unsigned char is_bdp;
extern unsigned int gpd_num;
unsigned int ori_gpd_num;
extern PGPD Rx_gpd_end[15];
extern PGPD Tx_gpd_end[15];
extern PGPD Rx_gpd_last[15];
extern PGPD Tx_gpd_last[15];
extern UINT8P ixia_buf[4][5000]; 
unsigned int ixia_isr_count=0;
unsigned int ixia_empty_count=0;

BYTE PDU_calcCksum(BYTE *data, int len)
{
        BYTE *uDataPtr, ckSum;
        int i;
	 *(data + 1) = 0x0;
        uDataPtr = data;
        ckSum = 0;
        for (i = 0; i < len; i++)
                ckSum += *(uDataPtr + i);
        return 0xFF - ckSum;
}

/*******************************************************************
*        externed function                                                                                   *
*******************************************************************/

#if defined(CONFIG_ARCH_MT85XX)
extern void ep0_handler(void);
#endif

//irqreturn_t HdrcBsrQmu(int irq_no, void* pParam){
void HdrcBsrQmu(unsigned short wInterruptNumber){
    //hdrc_t* pCtrl = (hdrc_t*)pParam;
#if defined(CONFIG_ARCH_MT85XX)
    BsrItem item;
#endif
    unsigned char bIntrUsbValue;
    unsigned int dwDmaIntrValue;
    unsigned short wIntrTxValue;
    unsigned short wIntrRxValue;
    unsigned int   wIntrQMUValue;
   // unsigned int orgMask;
    unsigned int ep_num, channel;
    static unsigned int BsrCount = 0;
      
    {
    	 //Printf("into BSR!!![%x]\r\n", item.wIntrQMUValue);
		#if defined(CONFIG_ARCH_MT85XX)
        item.bIntrUsbValue = 0;
        item.dwDmaIntrValue = 0;
        item.wIntrTxValue = 0;
        item.wIntrRxValue = 0;
	     item.wIntrQMUValue = 0;			 
		 #endif    	 

        bIntrUsbValue = MGC_QREG_Read8(MGC_O_HDRC_INTRUSB) & MGC_QREG_Read8(MGC_O_HDRC_INTRUSBE);
        dwDmaIntrValue = MGC_QREG_Read32(MGC_O_HSDMA_INTR);
        wIntrTxValue = MGC_QREG_Read16(MGC_O_HDRC_INTRTX) & MGC_QREG_Read16(MGC_O_HDRC_INTRTXE);
        wIntrRxValue = MGC_QREG_Read16(MGC_O_HDRC_INTRRX) & MGC_QREG_Read16(MGC_O_HDRC_INTRRXE);
        wIntrQMUValue = MGC_ReadQIRQ32(MGC_O_QIRQ_QISAR) & (~ (MGC_ReadQIRQ32(MGC_O_QIRQ_QIMR)));
        //Printf("Interrupt: IntrUsb [%x] IntrTx[%x] IntrRx [%x] IntrDMA[%x] IntrQMU [%x]\r\n", bIntrUsbValue, wIntrTxValue, wIntrRxValue, dwDmaIntrValue, wIntrQMUValue);        
        if( !(bIntrUsbValue && wIntrTxValue && wIntrRxValue && dwDmaIntrValue && wIntrQMUValue) )
        {
            //Printf("[NULL INTR] REG_INTRL1 = 0x%08X\n", MGC_QREG_Read32(M_REG_INTRLEVEL1));
        }

        MGC_WriteQIRQ32(MGC_O_QIRQ_QISAR, wIntrQMUValue);
        MGC_QMU_Write16(MGC_O_HDRC_INTRTX, wIntrTxValue);
        MGC_QMU_Write16(MGC_O_HDRC_INTRRX, wIntrRxValue);
        MGC_QMU_Write8(MGC_O_HDRC_INTRUSB, bIntrUsbValue);
        MGC_QREG_Write32(MGC_O_HSDMA_INTR, dwDmaIntrValue);        

        BsrCount++;   

        /* handle CONN interrupt */
        if(bIntrUsbValue & MGC_M_INTR_CONNECT){
            MGC_ResetPort();
            //return;// IRQ_HANDLED;
            goto HDRC_BSR_EXIT;
        }
        if(bIntrUsbValue & MGC_M_INTR_BABBLE){
            nBabble++;
            Printf("Babble (%d)\r\n", nBabble);
        }
        if(bIntrUsbValue & MGC_M_INTR_VBUSERROR){
            bError = TRUE;
            Printf("VBus Error \r\n");
        }
      	if(dwDmaIntrValue){
            for(channel = 0; channel < DMA_CHANNEL_NUM; channel++){
                if(dwDmaIntrValue & (1<<channel)){
                    //dma_handler(channel+1);
		      //Printf("dma interrupt trigger (channel : %d)\r\n", channel+1);
                }
            }
        }

		if(wIntrTxValue & 1){
		#if defined(CONFIG_ARCH_MT85XX)
			ep0_handler();
		#endif
		}

        if(wIntrTxValue){
            for(ep_num = 1; ep_num <= TX_FIFO_NUM; ep_num++){
                if(wIntrTxValue & (1<<ep_num)){
                    tx_handler(ep_num);
                }
            }
        }
        if(wIntrRxValue){
            for(ep_num = 1; ep_num <= RX_FIFO_NUM; ep_num++){
                if(wIntrRxValue & (1<<ep_num)){
                    rx_handler(ep_num);
                }
            }
        }
	 if(wIntrQMUValue){
		Qmu_handler(wIntrQMUValue);
	 }
    }


HDRC_BSR_EXIT:

    //ben, avoid empty interrupt
    BIM_ClearIrq(USB_IRQ_NO);
    //~ben

    return; //IRQ_HANDLED;
}

void HdrcBsrDevQmu(unsigned short wInterruptNumber)
{
    //hdrc_t* pCtrl = (hdrc_t*)pParam;
	#if defined(CONFIG_ARCH_MT85XX)
    BsrItem item;
	#endif
    unsigned char bIntrUsbValue;
    unsigned int dwDmaIntrValue;
    unsigned short wIntrTxValue;
    unsigned short wIntrRxValue;
    unsigned int   wIntrQMUValue;
   // unsigned int orgMask;
    unsigned int ep_num, channel;
    static unsigned int BsrCount = 0;

    {
        //Printf("into BSR!!![%x]\r\n", item.wIntrQMUValue);
		#if defined(CONFIG_ARCH_MT85XX)
        item.bIntrUsbValue = 0;
        item.dwDmaIntrValue = 0;
        item.wIntrTxValue = 0;
        item.wIntrRxValue = 0;
        item.wIntrQMUValue = 0;			 
        #endif

        bIntrUsbValue = MGC_QREG_Read8(MGC_O_HDRC_INTRUSB) & MGC_QREG_Read8(MGC_O_HDRC_INTRUSBE);
        dwDmaIntrValue = MGC_QREG_Read32(MGC_O_HSDMA_INTR);
        wIntrTxValue = MGC_QREG_Read16(MGC_O_HDRC_INTRTX) & MGC_QREG_Read16(MGC_O_HDRC_INTRTXE);
        wIntrRxValue = MGC_QREG_Read16(MGC_O_HDRC_INTRRX) & MGC_QREG_Read16(MGC_O_HDRC_INTRRXE);
        wIntrQMUValue = MGC_ReadQIRQ32(MGC_O_QIRQ_QISAR) & (~ (MGC_ReadQIRQ32(MGC_O_QIRQ_QIMR)));
        Printf("Interrupt: IntrUsb [%x] IntrTx[%x] IntrRx [%x] IntrDMA[%x] IntrQMU [%x]\r\n", bIntrUsbValue, wIntrTxValue, wIntrRxValue, dwDmaIntrValue, wIntrQMUValue);        

        MGC_WriteQIRQ32(MGC_O_QIRQ_QISAR, wIntrQMUValue);
        MGC_QMU_Write16(MGC_O_HDRC_INTRTX, wIntrTxValue);
        MGC_QMU_Write16(MGC_O_HDRC_INTRRX, wIntrRxValue);
        MGC_QMU_Write8(MGC_O_HDRC_INTRUSB, bIntrUsbValue);
        MGC_QREG_Write32(MGC_O_HSDMA_INTR, dwDmaIntrValue);          
        
        //ben, avoid empty interrupt
        BIM_ClearIrq(USB_IRQ_NO);
        //~ben
    
        //ben for debug
        if(BsrCount < 10)
        {
            Printf("[ISR %d] bIntrUsbValue:  0x%08X\n", BsrCount, bIntrUsbValue);
            Printf("[ISR %d] dwDmaIntrValue: 0x%08X\n", BsrCount, dwDmaIntrValue);
            Printf("[ISR %d] wIntrTxValue:   0x%08X\n", BsrCount, wIntrTxValue);
            Printf("[ISR %d] wIntrRxValue:   0x%08X\n", BsrCount, wIntrRxValue);
            Printf("[ISR %d] wIntrQMUValue:  0x%08X\n", BsrCount, wIntrQMUValue);
        }
        BsrCount++;   
        //~ben

            
        /* handle CONN interrupt */
        if(dwDmaIntrValue){
            for(channel = 0; channel < DMA_CHANNEL_NUM; channel++){
                if(dwDmaIntrValue & (1<<channel)){
                    //dma_handler(channel+1);
		      //Printf("dma interrupt trigger (channel : %d)\r\n", channel+1);
                }
            }
        }
       
        if(bIntrUsbValue & MGC_M_INTR_RESET){
            Printf("Reset interrupt\r\n");
            if(MGC_QREG_Read8(MGC_O_HDRC_POWER) & MGC_M_POWER_HSMODE){
                Printf("Device: high-speed mode\r\n");
            }
            else{
                Printf("Device: full-speed mode\r\n");
            }
	     isReset = 1;
        }
        
      	if(bIntrUsbValue & MGC_M_INTR_SUSPEND){
      		Printf("suspend interrupt\r\n");
      		isSuspend = TRUE;
      		//MGC_QMU_Write8(MGC_O_HDRC_INTRUSBE, MGC_QREG_Read8(MGC_O_HDRC_INTRUSBE) & ~MGC_M_INTR_SUSPEND);
      	}
      	if(bIntrUsbValue & MGC_M_INTR_RESUME){
      		Printf("resume interrupt\r\n");
      		isSuspend = FALSE;
      	}
      	
      	if(bIntrUsbValue & MGC_M_INTR_DISCONNECT){
      		Printf("disconnect\r\n");
      	}
        if(wIntrTxValue & 1){
            ep0_handler();
        }
        if(wIntrTxValue){
            for(ep_num = 1; ep_num <= TX_FIFO_NUM; ep_num++){
                if(wIntrTxValue & (1<<ep_num)){
                    tx_handler(ep_num);
                }
            }
        }
        if(wIntrRxValue){
            for(ep_num = 1; ep_num <= RX_FIFO_NUM; ep_num++){
                if(wIntrRxValue & (1<<ep_num)){
                    rx_handler(ep_num);
                }
            }
        }
	 if(wIntrQMUValue){
		DevQmu_handler(wIntrQMUValue);
	 }
    }
}


#define QMU_LAST 0
#define QMU_CURR 1

#if defined(CONFIG_ARCH_MT85XX)
unsigned int _u4TxCount[2][2] = {{0},{0}};
unsigned int _u4RxCount[2][2] = {{0},{0}};
#else
unsigned int _u4TxCount[4][2] = {{0},{0},{0},{0}};
unsigned int _u4RxCount[4][2] = {{0},{0},{0},{0}};
#endif

void HdrcIsrMultiIxiaQMU(unsigned short wInterruptNumber){
   // hdrc_t* pCtrl = (hdrc_t*)pParam;

    unsigned int wIntrQMUValue;
   // unsigned int ep_num, channel;

	//Printf("hdrcisrmultiixia\r\n");

    ixia_isr_count++;
    /*
    for(i=1; i<=2; i++)
    {
        Printf("Q#%d\n", i);
        Printf("RQCSR 0x%08X\n", MGC_ReadQMU16(MGC_O_QMU_RQCSR(i)));
        Printf("RQSAR 0x%08X\n", MGC_ReadQMU16(MGC_O_QMU_RQSAR(i)));
        Printf("RQCPR 0x%08X\n", MGC_ReadQMU16(MGC_O_QMU_RQCPR(i)));
    }

    bIntrUsbValue = MGC_QREG_Read8(MGC_O_HDRC_INTRUSB) & MGC_QREG_Read8(MGC_O_HDRC_INTRUSBE);
    dwDmaIntrValue = MGC_QREG_Read32(MGC_O_HSDMA_INTR);
    wIntrTxValue = MGC_QREG_Read16(MGC_O_HDRC_INTRTX) & MGC_QREG_Read16(MGC_O_HDRC_INTRTXE);
    wIntrRxValue = MGC_QREG_Read16(MGC_O_HDRC_INTRRX) & MGC_QREG_Read16(MGC_O_HDRC_INTRRXE);
    */
    wIntrQMUValue = MGC_ReadQIRQ32(MGC_O_QIRQ_QISAR) & (~ (MGC_ReadQIRQ32(MGC_O_QIRQ_QIMR)));
    /*
    MGC_QMU_Write8(MGC_O_HDRC_INTRUSB, (bIntrUsbValue));
    MGC_QREG_Write32(MGC_O_HSDMA_INTR, (dwDmaIntrValue));
    MGC_QMU_Write16(MGC_O_HDRC_INTRTX, (wIntrTxValue));
    MGC_QMU_Write16(MGC_O_HDRC_INTRRX, (wIntrRxValue));
    */
    MGC_WriteQIRQ32(MGC_O_QIRQ_QISAR, (wIntrQMUValue));
    //ben, avoid empty interrupt
    BIM_ClearIrq(USB_IRQ_NO);
    //~ben
    //Printf("Interrupt: IntrUsb [%x] IntrTx[%x] IntrRx [%x] IntrDMA[%x] IntrQMU [%x]\r\n", bIntrUsbValue, wIntrTxValue, wIntrRxValue, dwDmaIntrValue, wIntrQMUValue);
    /*
    if(bIntrUsbValue & MGC_M_INTR_CONNECT){
        MGC_ResetPort();
        return;
    }
   
    if(dwDmaIntrValue){
        for(channel = 0; channel < DMA_CHANNEL_NUM; channel++){
            if(dwDmaIntrValue & (1<<channel)){
                multi_ixia_dma_handler(channel+1);
            }
        }
    }
    if(wIntrTxValue){
        for(ep_num = 1; ep_num <= TX_FIFO_NUM; ep_num++){
            if(wIntrTxValue & (1<<ep_num)){
                multi_ixia_tx_handler(ep_num);
            }
        }
    }
    if(wIntrRxValue){
        for(ep_num = 1; ep_num <= RX_FIFO_NUM; ep_num++){
            if(wIntrRxValue & (1<<ep_num)){
                multi_ixia_rx_handler(ep_num);
            }
        }
    }
    */
    /*
    int i;
    if(wIntrQMUValue){
	    for(i=1; i<4; i++){	
			proc_ixia_qmu_rx(i);
			proc_ixia_qmu_tx(i);
	    }
    }
    */
    
    if(wIntrQMUValue){
	ixia_Qmu_handler(wIntrQMUValue);
	
    }


    return;
}

void ixia_Qmu_handler(unsigned int wQmuVal){
//    int ep_num;    
    int i;
    //readTxCSR
    //Printf("into QMU_handler!\r\n");
    wQmuVal = MGC_ReadQIRQ32(MGC_O_QIRQ_QISAR);
    //Printf("QMU handler %x\r\n",wQmuVal);
    for(i=1; i<5; i++){
        if(wQmuVal & DQMU_M_RX_DONE(i)){
        	proc_ixia_qmu_rx(i);
        }
        if(wQmuVal & DQMU_M_TX_DONE(i)){
        	proc_ixia_qmu_tx(i);
        }
    }
    /*
    if(wQmuVal & DQMU_M_RXEP_ERR){ 	
 	unsigned int wEpErrVal=MGC_ReadQIRQ32(MGC_O_QIRQ_REPEIR);
 	Printf("Rx endpoint error in QMU mode![0x%x]\r\n", wEpErrVal);
 	for(i=1; i<9; i++){
 		if(wEpErrVal &DQMU_M_RX_EP_ERR(i) ){
 			Printf("Rx %d Ep Error!\r\n", i); 
 		}		  
 	}
 	MGC_WriteQIRQ32(MGC_O_QIRQ_REPEIR, wEpErrVal);
    }
    if(wQmuVal & DQMU_M_TXEP_ERR){ 	
 	unsigned int wEpErrVal=MGC_ReadQIRQ32(MGC_O_QIRQ_TEPEIR);
 	Printf("Tx endpoint error in QMU mode![0x%x]\r\n", wEpErrVal);
 	for(i=1; i<9; i++){
 		if(wEpErrVal &DQMU_M_TX_EP_ERR(i) ){
 			Printf("Tx %d Ep Error!\r\n", i); 
 		}		  
 	}
 	MGC_WriteQIRQ32(MGC_O_QIRQ_TEPEIR, wEpErrVal);
    }
    if(wQmuVal & DQMU_M_RXQ_ERR){ 	
 	unsigned int wErrVal=MGC_ReadQIRQ32(MGC_O_QIRQ_RQEIR);
 	Printf("Rx Queue error in QMU mode![0x%x]\r\n", wErrVal);
 	for(i=1; i<9; i++){ 			  
 		if(wErrVal &DQMU_M_RX_GPDCS_ERR(i) ){
 			Printf("Rx %d GPD checksum error!\r\n", i); 
 		}	
 		if(wErrVal &DQMU_M_RX_ZLP_ERR(i) ){
 			Printf("Rx %d recieve an zlp packet!\r\n", i); 
 		}	
 		if(wErrVal &DQMU_M_RX_LEN_ERR(i) ){
 			Printf("Rx %d recieve length error!\r\n", i); 
 		}
 	}
 	MGC_WriteQIRQ32(MGC_O_QIRQ_RQEIR, wErrVal);
    }
    if(wQmuVal & DQMU_M_TXQ_ERR){ 	
 	unsigned int wErrVal=MGC_ReadQIRQ32(MGC_O_QIRQ_TQEIR);
 	Printf("Tx Queue error in QMU mode![0x%x]\r\n", wErrVal);
 	for(i=1; i<9; i++){
 		if(wErrVal &DQMU_M_TX_BDCS_ERR(i) ){
 			Printf("Tx %d BD checksum error!\r\n", i); 
 		}		  
 		if(wErrVal &DQMU_M_TX_GPDCS_ERR(i) ){
 			Printf("Tx %d GPD checksum error!\r\n", i); 
 		}	
 		if(wErrVal &DQMU_M_TX_LEN_ERR(i) ){
 			Printf("Tx %d buffer length error!\r\n", i); 
 		}	
 	}
 	MGC_WriteQIRQ32(MGC_O_QIRQ_TQEIR, wErrVal); 	
    }
    */
    /*
    if(wQmuVal & DQMU_M_TQ_EMPTY){ 	
 	unsigned int wEmptyVal=MGC_ReadQIRQ32(MGC_O_QIRQ_TEPEMPR);
 	//Printf("Tx Empty in QMU mode![0x%x]\r\n", wEmptyVal);
 	for(i=1; i<5; i++){
 		if(wEmptyVal &DQMU_M_TX_EMPTY(i) ){
 			Printf("Tx %d Empty!%d \r\n", i, ++ixia_empty_count); 
 		}		  
 	}
 	MGC_WriteQIRQ32(MGC_O_QIRQ_TEPEMPR, wEmptyVal);
    }   
    
    if(wQmuVal & DQMU_M_RQ_EMPTY){ 	
 	unsigned int wEmptyVal=MGC_ReadQIRQ32(MGC_O_QIRQ_REPEMPR);
 	//Printf("Rx Empty in QMU mode![0x%x]\r\n", wEmptyVal);
 	for(i=1; i<5; i++){
 		if(wEmptyVal &DQMU_M_RX_EMPTY(i) ){
 			Printf("Rx %d Empty %d!\r\n", i, ++ixia_empty_count); 
 			//proc_ixia_rx_empty(i);
 		}		  
 	}
 	MGC_WriteQIRQ32(MGC_O_QIRQ_REPEMPR, wEmptyVal);
 	//MGC_WriteQIRQ32(MGC_O_QIRQ_TQEIMCR, DQMU_M_TX_LEN_ERR(ep_num)|DQMU_M_TX_BDCS_ERR(ep_num)|DQMU_M_TX_CPDCS_ERR(ep_num));
	//MGC_WriteQIRQ32(MGC_O_QIRQ_TEPEIR, DQMU_M_TX_EP_ERR(ep_num));
    }    
    */
}

void Qmu_handler(unsigned int wQmuVal)
{ 
    int i;

    LOG(6, "QMU handler %x\r\n",wQmuVal);
    for(i=1; i<9; i++)
    {
    	if(wQmuVal & DQMU_M_RX_DONE(i))
    	{
    		proc_qmu_rx(i);
    	}
    	if(wQmuVal & DQMU_M_TX_DONE(i))
    	{
    		proc_qmu_tx(i);
    	}
    }
    
    if(wQmuVal & DQMU_M_RXEP_ERR)
    { 	
        unsigned int wEpErrVal=MGC_ReadQIRQ32(MGC_O_QIRQ_REPEIR);
        LOG(6, "Rx endpoint error in QMU mode![0x%x]\r\n", wEpErrVal);
		MUSB_ASSERT(1);
        for(i=1; i<9; i++)
        {
        	if(wEpErrVal &DQMU_M_RX_EP_ERR(i) )
        	{
        		Printf("Rx %d Ep Error!\r\n", i); 
        		ep_error_check(i, USB_RX);
        	}		  
        }
     	MGC_WriteQIRQ32(MGC_O_QIRQ_REPEIR, wEpErrVal);
    }
    
    if(wQmuVal & DQMU_M_TXEP_ERR)
    { 	
     	unsigned int wEpErrVal=MGC_ReadQIRQ32(MGC_O_QIRQ_TEPEIR);
     	LOG(6, "Tx endpoint error in QMU mode![0x%x]\r\n", wEpErrVal);
		MUSB_ASSERT(1);
     	for(i=1; i<9; i++)
     	{
     		if(wEpErrVal &DQMU_M_TX_EP_ERR(i) )
     		{
     			Printf("Tx %d Ep Error!\r\n", i); 
     			ep_error_check(i, USB_TX);
     		}		  
     	}
     	MGC_WriteQIRQ32(MGC_O_QIRQ_TEPEIR, wEpErrVal);
    }
    
    if(wQmuVal & DQMU_M_RXQ_ERR)
    { 	
     	unsigned int wErrVal=MGC_ReadQIRQ32(MGC_O_QIRQ_RQEIR);
     	LOG(6, "Rx Queue error in QMU mode![0x%x]\r\n", wErrVal);
		MUSB_ASSERT(1);
     	for(i=1; i<9; i++)
     	{ 			  
     		if(wErrVal &DQMU_M_RX_GPDCS_ERR(i) )
     		{
     			LOG(6, "Rx %d GPD checksum error!\r\n", i); 
     		}	
     		if(wErrVal &DQMU_M_RX_ZLP_ERR(i) )
     		{
     			LOG(6, "Rx %d recieve an zlp packet!\r\n", i); 
     		}	
     		if(wErrVal &DQMU_M_RX_LEN_ERR(i) )
     		{
     			LOG(6, "Rx %d recieve length error!\r\n", i); 
     		}
     	}
     	MGC_WriteQIRQ32(MGC_O_QIRQ_RQEIR, wErrVal);
    }
    if(wQmuVal & DQMU_M_TXQ_ERR){ 	
 	unsigned int wErrVal=MGC_ReadQIRQ32(MGC_O_QIRQ_TQEIR);
 	LOG(6, "Tx Queue error in QMU mode![0x%x]\r\n", wErrVal);
 	for(i=1; i<9; i++){
 		if(wErrVal &DQMU_M_TX_BDCS_ERR(i) ){
 			LOG(6, "Tx %d BD checksum error!\r\n", i); 
 		}		  
 		if(wErrVal &DQMU_M_TX_GPDCS_ERR(i) ){
 			LOG(6, "Tx %d GPD checksum error!\r\n", i); 
 		}	
 		if(wErrVal &DQMU_M_TX_LEN_ERR(i) ){
 			LOG(6, "Tx %d buffer length error!\r\n", i); 
 		}	
 	}
 	MGC_WriteQIRQ32(MGC_O_QIRQ_TQEIR, wErrVal);
    }
    if(wQmuVal & DQMU_M_RQ_EMPTY){ 	
 	unsigned int wEmptyVal=MGC_ReadQIRQ32(MGC_O_QIRQ_REPEMPR);
 	LOG(6, "Rx Empty in QMU mode![0x%x]\r\n", wEmptyVal);
 	for(i=1; i<9; i++){
 		if(wEmptyVal &DQMU_M_RX_EMPTY(i) ){
 			LOG(6, "Rx %d Empty!\r\n", i); 
 		}		  
 	}
 	MGC_WriteQIRQ32(MGC_O_QIRQ_REPEMPR, wEmptyVal);
    }
    if(wQmuVal & DQMU_M_TQ_EMPTY){ 	
 	unsigned int wEmptyVal=MGC_ReadQIRQ32(MGC_O_QIRQ_TEPEMPR);
 	LOG(6, "Tx Empty in QMU mode![0x%x]\r\n", wEmptyVal);
 	for(i=1; i<9; i++){
 		if(wEmptyVal &DQMU_M_TX_EMPTY(i) ){
 			LOG(6, "Tx %d Empty!\r\n", i); 
 		}		  
 	}
 	MGC_WriteQIRQ32(MGC_O_QIRQ_TEPEMPR, wEmptyVal);
    wEmptyVal=MGC_ReadQIRQ32(MGC_O_QIRQ_TEPEMPR);
    LOG(6, "TX Empty clean [0%x]\n", wEmptyVal);
    }   
}

void DevQmu_handler(unsigned int wQmuVal){
//    int ep_num;    
    int i;
    
    for(i=1; i<9; i++){
	if(wQmuVal & DQMU_M_RX_DONE(i)){
		proc_devqmu_rx(i);
	}
	if(wQmuVal & DQMU_M_TX_DONE(i)){
		proc_devqmu_tx(i);
	}
    }
    if(wQmuVal & DQMU_M_RXEP_ERR){ 	
 	unsigned int wEpErrVal=MGC_ReadQIRQ32(MGC_O_QIRQ_REPEIR);
 	Printf("Rx endpoint error in QMU mode![0x%x]\r\n", wEpErrVal);
 	for(i=1; i<9; i++){
 		if(wEpErrVal &DQMU_M_RX_EP_ERR(i) ){
 			Printf("Rx %d Ep Error!\r\n", i); 
 			ep_error_dev_check(i, USB_RX);
 		}		  
 	}
 	MGC_WriteQIRQ32(MGC_O_QIRQ_REPEIR, wEpErrVal);
    }
    if(wQmuVal & DQMU_M_TXEP_ERR){ 	
 	unsigned int wEpErrVal=MGC_ReadQIRQ32(MGC_O_QIRQ_TEPEIR);
 	Printf("Tx endpoint error in QMU mode![0x%x]\r\n", wEpErrVal);
 	for(i=1; i<9; i++){
 		if(wEpErrVal &DQMU_M_TX_EP_ERR(i) ){
 			Printf("Tx %d Ep Error!\r\n", i);
 			ep_error_dev_check(i, USB_TX);
 		}		  
 	}
 	MGC_WriteQIRQ32(MGC_O_QIRQ_TEPEIR, wEpErrVal);
    }
    if(wQmuVal & DQMU_M_RXQ_ERR){ 	
 	unsigned int wErrVal=MGC_ReadQIRQ32(MGC_O_QIRQ_RQEIR);
 	Printf("Rx Queue error in QMU mode![0x%x]\r\n", wErrVal);
 	for(i=1; i<9; i++){ 			  
 		if(wErrVal &DQMU_M_RX_GPDCS_ERR(i) ){
 			Printf("Rx %d GPD checksum error!\r\n", i); 
 		}	
 		if(wErrVal &DQMU_M_RX_ZLP_ERR(i) ){
 			Printf("Rx %d recieve an zlp packet!\r\n", i); 
 		}	
 		if(wErrVal &DQMU_M_RX_LEN_ERR(i) ){
 			Printf("Rx %d recieve length error!\r\n", i); 
 		}
 	}
 	MGC_WriteQIRQ32(MGC_O_QIRQ_RQEIR, wErrVal);
    }
    if(wQmuVal & DQMU_M_TXQ_ERR){ 	
 	unsigned int wErrVal=MGC_ReadQIRQ32(MGC_O_QIRQ_TQEIR);
 	Printf("Tx Queue error in QMU mode![0x%x]\r\n", wErrVal);
 	for(i=1; i<9; i++){
 		if(wErrVal &DQMU_M_TX_BDCS_ERR(i) ){
 			Printf("Tx %d BD checksum error!\r\n", i); 
 		}		  
 		if(wErrVal &DQMU_M_TX_GPDCS_ERR(i) ){
 			Printf("Tx %d GPD checksum error!\r\n", i); 
 		}	
 		if(wErrVal &DQMU_M_TX_LEN_ERR(i) ){
 			Printf("Tx %d buffer length error!\r\n", i); 
 		}	
 	}
 	MGC_WriteQIRQ32(MGC_O_QIRQ_TQEIR, wErrVal);
    }
    if(wQmuVal & DQMU_M_RQ_EMPTY){ 	
 	unsigned int wEmptyVal=MGC_ReadQIRQ32(MGC_O_QIRQ_REPEMPR);
 	Printf("Rx Empty in QMU mode![0x%x]\r\n", wEmptyVal);
 	for(i=1; i<9; i++){
 		if(wEmptyVal &DQMU_M_RX_EMPTY(i) ){
 			Printf("Rx %d Empty!\r\n", i); 
 		}		  
 	}
 	MGC_WriteQIRQ32(MGC_O_QIRQ_REPEMPR, wEmptyVal);
    }
    if(wQmuVal & DQMU_M_TQ_EMPTY){ 	
 	unsigned int wEmptyVal=MGC_ReadQIRQ32(MGC_O_QIRQ_TEPEMPR);
 	Printf("Tx Empty in QMU mode![0x%x]\r\n", wEmptyVal);
 	for(i=1; i<9; i++){
 		if(wEmptyVal &DQMU_M_TX_EMPTY(i) ){
 			Printf("Tx %d Empty!\r\n", i); 
 		}		  
 	}
 	MGC_WriteQIRQ32(MGC_O_QIRQ_TEPEMPR, wEmptyVal);
    }   
}

VOID ep_error_check(unsigned char ep_num, USB_DIR dir){
	unsigned char wType ;
	unsigned short wCsrVal;
	unsigned char isISO=FALSE;
	if(dir==USB_TX){
		wType = MGC_QReadCsr16(MGC_O_HDRC_TXTYPE, ep_num);
		wCsrVal = MGC_QReadCsr16(MGC_O_HDRC_TXCSR, ep_num);
	}else{
		wType = MGC_QReadCsr16(MGC_O_HDRC_RXTYPE, ep_num);
		wCsrVal = MGC_QReadCsr16(MGC_O_HDRC_RXCSR, ep_num);
	}
	wType=wType>>4;
	if(wType==USB_ISO){
		isISO=TRUE;
	}
	if(isISO){
		if(dir==USB_TX){
			if(wCsrVal & MGC_M_TXCSR_INCOMPTX){
				Printf("Tx Incomplete Error[%d]\r\n", ep_num);	        	
			}
		}else{
			if(wCsrVal & MGC_M_RXCSR_INCOMPRX){
				Printf("Rx Incomplete Error[%d]\r\n", ep_num);	        	
			}
			if(wCsrVal & MGC_M_RXCSR_PIDERROR){
				Printf("Rx PID Error[%d]\r\n", ep_num);			
			}
			if(wCsrVal & MGC_M_RXCSR_DATAERR){
				Printf("Rx DATA Error[%d]\r\n", ep_num);			
			}
		}
	}else{
		if(dir==USB_TX){
			if(wCsrVal & MGC_M_TXCSR_H_RXSTALL){
				Printf("Tx RxStall Error[%d]\r\n", ep_num);			
			}
			if(wCsrVal & MGC_M_TXCSR_H_ERROR){
				Printf("Tx Strikeout Error[%d]\r\n", ep_num);			
			}
			if(wCsrVal & MGC_M_TXCSR_H_NAKTIMEOUT){
				Printf("Tx NAK TimeOut Error[%d]\r\n", ep_num);			
			}
		}else{
			if(wCsrVal & MGC_M_RXCSR_H_RXSTALL){
				Printf("Rx RxStall Error[%d]\r\n", ep_num);			
			}
			if(wCsrVal & MGC_M_RXCSR_H_ERROR){
				Printf("Rx Strikeout Error[%d]\r\n", ep_num);			
			}
			if(wCsrVal & MGC_M_RXCSR_H_NAKTIMEOUT){
				Printf("Rx NAK TimeOut Error[%d]\r\n", ep_num);			
			}
		}
	}	
}

VOID ep_error_dev_check(unsigned char ep_num, USB_DIR dir){
	unsigned short wCsrVal;
	unsigned char isISO=FALSE;
	if(dir==USB_TX){		
		wCsrVal = MGC_QReadCsr16(MGC_O_HDRC_TXCSR, ep_num);
	}else{		
		wCsrVal = MGC_QReadCsr16(MGC_O_HDRC_RXCSR, ep_num);
	}
	Printf("wCsr: 0x%x\r\n", wCsrVal);
	
	if(wCsrVal&MGC_M_TXCSR_ISO){
		isISO=TRUE;
	}
	if(isISO){
		if(dir==USB_TX){
			if(wCsrVal & MGC_M_TXCSR_INCOMPTX){
				Printf("Tx Incomplete Error[%d]\r\n", ep_num);	        	
			}
			if(wCsrVal & MGC_M_TXCSR_P_UNDERRUN){
				Printf("Tx Underrun Error[%d]\r\n", ep_num);			
			}
		}else{
			if(wCsrVal & MGC_M_RXCSR_INCOMPRX){
				Printf("Rx Incomplete Error[%d]\r\n", ep_num);	        	
			}
			if(wCsrVal & MGC_M_RXCSR_PIDERROR){
				Printf("Rx PID Error[%d]\r\n", ep_num);			
			}
			if(wCsrVal & MGC_M_RXCSR_DATAERR){
				Printf("Rx DATA Error[%d]\r\n", ep_num);			
			}
			if(wCsrVal & MGC_M_RXCSR_P_OVERRUN){
				Printf("Rx Overrun Error[%d]\r\n", ep_num);			
			}
		}
	}
}

TGPD* GetEndGpd(unsigned char ep_num, TGPD** LastGpd, USB_DIR dir){
	TGPD* gpd_end;
	if(dir==USB_TX){
		gpd_end=(TGPD*)(MGC_ReadQMU32(MGC_O_QMU_TQSAR(ep_num)));
	}else{
		gpd_end=(TGPD*)(MGC_ReadQMU32(MGC_O_QMU_RQSAR(ep_num)));
	}
	*LastGpd=NULL;
	while(TGPD_IS_FLAGS_HWO(gpd_end)){
		*LastGpd = gpd_end;
		gpd_end = TGPD_GET_NEXT(gpd_end);
	}
	return gpd_end;

}

TGPD* GpdTxPrepare(BYTE*pBuf, unsigned int data_len, unsigned char ep_num, unsigned char target_ep, unsigned char _is_bdp){
    unsigned int data_length, offset;
    int i; // int seed;
    TGPD* gpd;

    TBD * bd_head;
    TBD *bd;
    BYTE* pBuffer; // mhzhang
    unsigned int length;
    TBD * bd_next;
    TGPD* gpd_last;
    TGPD * gpd_end;
    BYTE *_tmp;
	
    gpd_num--;
    data_length= gpd_num?data_len/2 : data_len;
    gpd =(TGPD*)MUSB_MemAlloc(sizeof(TGPD)+AT_GPD_EXT_LEN);//, 32);
    if(NULL ==gpd)
    {
	return NULL;
	}
    MUSB_MemSet(gpd, 0, sizeof(TGPD)+AT_GPD_EXT_LEN);   
    //Printf("GPD %d buffer: 0x%x _is_bdp %d\r\n", gpd_num, gpd, _is_bdp);
    if(!_is_bdp)
    {
        TGPD_SET_DATA(gpd, pBuf+AT_GPD_EXT_LEN);
        TGPD_CLR_FORMAT_BDP(gpd);
    }
    else
    {
		bd_head=(TBD*)MUSB_MemAlloc(sizeof(TBD));//, 32);
		 if(NULL ==bd_head)
		{
		return NULL;
		}
		bd=bd_head;
		MUSB_MemSet(bd, 0, sizeof(TBD));		

        length=data_length-AT_GPD_EXT_LEN;		 
        pBuffer= (BYTE*)(pBuf+AT_GPD_EXT_LEN);

		for(i=0; i<5; i++){
					#if defined(CONFIG_ARCH_MT85XX)
                    offset=random32()%length;
                    #else
                    offset = 0x10;
                    #endif
			if(!offset){
				offset=length;
			}			
			TBD_SET_EXT_LEN(bd, 0);
			TBD_SET_BUF_LEN(bd, offset);
			TBD_SET_DATA(bd, pBuffer);
			if(offset==length){
				TBD_SET_FLAGS_EOL(bd);
				TBD_SET_NEXT(bd, 0);
				TBD_SET_CHKSUM(bd, 16);	
				Printf("BD number %d\r\n", i+1);
				length -= offset;
				break;
			}else{				
				TBD_CLR_FLAGS_EOL(bd);
				// TBD * bd_next=(TBD*)x_alloc_aligned_mem(sizeof(TBD), 32);
				bd_next=(TBD*)MUSB_MemAlloc(sizeof(TBD));//, 32); // mhzhang
				if(NULL == bd_next)
				{
					return NULL;
				}
                            MUSB_MemSet(bd_next, 0, sizeof(TBD));
				TBD_SET_NEXT(bd, bd_next);
				TBD_SET_CHKSUM(bd, 16);	
				length -= offset;
				pBuffer += offset;
				bd=bd_next;
			}
		 }			  
		 if(length){
		  	if(AT_BD_EXT_LEN && AT_BD_EXT_LEN<=length){
		  		//free(bd);
		  		
		  		#if !PORTING //how to do realloc?
		  		bd=(TBD*)realloc(bd, sizeof(TBD)+AT_BD_EXT_LEN);
				#endif
				
				MUSB_MemSet(bd, 0, sizeof(TBD)+AT_BD_EXT_LEN);
		  		TBD_SET_EXT_LEN(bd, AT_BD_EXT_LEN);
		  		TBD_SET_BUF_LEN(bd, length-AT_BD_EXT_LEN);
				TBD_SET_DATA(bd, pBuffer+AT_BD_EXT_LEN);

				_tmp=TBD_GET_EXT(bd);
				
				MUSB_MemCopy(_tmp, pBuffer, AT_BD_EXT_LEN);
		  	}else{
				TBD_SET_EXT_LEN(bd, 0);
				TBD_SET_BUF_LEN(bd, length);
				TBD_SET_DATA(bd, pBuffer);
			}
			TBD_SET_FLAGS_EOL(bd);
			TBD_SET_NEXT(bd, 0);
			TBD_SET_CHKSUM(bd, 16);				
			Printf("BD number %d %d\r\n", gpd_num,  i+1);
		}
		TGPD_SET_DATA(gpd, bd_head);
		TGPD_SET_FORMAT_BDP(gpd);
	}
	TGPD_SET_BUF_LEN(gpd, data_length-AT_GPD_EXT_LEN);			
	//TGPD_SET_BUF_LEN(gpd, 0);			
	TGPD_SET_EXT_LEN(gpd, AT_GPD_EXT_LEN);
	if(AT_GPD_EXT_LEN){
		BYTE *_tmp=TGPD_GET_EXT(gpd);
		MUSB_MemCopy(_tmp, pBuf, AT_GPD_EXT_LEN);
	}
		  
	TGPD_SET_FORMAT_ZLP(gpd);
	TGPD_CLR_FORMAT_TGL(gpd);
	TGPD_SET_FORMAT_IOC(gpd);
	TGPD_SET_EPaddr(gpd, target_ep);	

	// TGPD* gpd_last;
	// TGPD* gpd_end=GetEndGpd(ep_num, &gpd_last, USB_TX);
	gpd_end=GetEndGpd(ep_num, &gpd_last, USB_TX); // mhzhang
	
	//MGC_ReadQMU32(MGC_O_QMU_RQSAR(ep_num));		
	if(!gpd_num){
		TGPD_SET_NEXT(gpd, gpd_end);		 
	}else{
		//Printf("GPD_NUM:%d\r\n",gpd_num);
		TGPD* l_gpd;
		l_gpd = GpdTxPrepare(pBuf+data_length, data_len-data_length, ep_num, target_ep, (!_is_bdp));
		TGPD_SET_NEXT(gpd, l_gpd);
	}
		  
	TGPD_SET_FLAGS_HWO(gpd);
	TGPD_SET_CHKSUM(gpd, 16);
	
	//TGPD_CLR_FLAGS_HWO(gpd);
	return gpd;	
}

TGPD* Prepare_Tx_GPD(TGPD* gpd, BYTE*pBuf, unsigned int data_length, unsigned char ep_num, unsigned char target_ep, unsigned char _is_bdp, unsigned char isHWO){
	unsigned int  offset;
	int i; // int seed;

    TBD * bd_next; 
    TBD * bd_head, *bd;  // mhzhang
    unsigned int length;
    BYTE* pBuffer;
    BYTE *_tmp;
    extern unsigned char _fgTxErrChkSumTestCase;
#if defined(CONFIG_ARCH_MT85XX)
    BYTE *pPhyBuf;
#endif

    //TGPD* gpd =(TGPD*)x_alloc_aligned_mem(sizeof(TGPD)+AT_GPD_EXT_LEN, 32);
    MUSB_MemSet(gpd, 0, sizeof(TGPD)+AT_GPD_EXT_LEN);   
    LOG(6, "GPD %x buffer: 0x%x _is_bdp %d\r\n", gpd, pBuf, _is_bdp);
#if defined(CONFIG_ARCH_MT85XX)
    pPhyBuf = (BYTE *)MUSB_DMA_MAP_TO_DEV((UINT32)pBuf,data_length);
#endif
	
    if(!_is_bdp)
    {
#if defined(CONFIG_ARCH_MT85XX)
        PHY_TGPD_SET_DATA(gpd, pPhyBuf+AT_GPD_EXT_LEN);
#else
        TGPD_SET_DATA(gpd, pBuf+AT_GPD_EXT_LEN);
#endif
        TGPD_CLR_FORMAT_BDP(gpd);
    }
    else
    {
		bd_head=(TBD*)MUSB_MemAlloc(sizeof(TBD));//, 32);
		if(NULL ==bd_head)
		{
		return NULL;
		}
		bd=bd_head;
		MUSB_MemSet(bd, 0, sizeof(TBD));		
        length=data_length-AT_GPD_EXT_LEN;
        pBuffer= (BYTE*)(pBuf+AT_GPD_EXT_LEN);

		for(i=0; i<5; i++)
		{
#if defined(CONFIG_ARCH_MT85XX)
            offset=random32()%length;
#else
            offset = 0;
            //offset=random32()%length;
#endif
			if(!offset){
				offset=length;
			}			
			TBD_SET_EXT_LEN(bd, 0);
			TBD_SET_BUF_LEN(bd, offset);
			TBD_SET_DATA(bd, pBuffer);
			if(offset==length){
				TBD_SET_FLAGS_EOL(bd);
				TBD_SET_NEXT(bd, 0);
				TBD_SET_CHKSUM(bd, 16);	
				Printf("BD number %d\r\n", i+1);
				length -= offset;
				break;
			}else{				
				TBD_CLR_FLAGS_EOL(bd);
                bd_next = (TBD*)MUSB_MemAlloc(sizeof(TBD));//, 32);
  		if(NULL ==bd_next)
		{
			return NULL;
		}              
                MUSB_MemSet(bd_next, 0, sizeof(TBD));
				TBD_SET_NEXT(bd, bd_next);
				TBD_SET_CHKSUM(bd, 16);	
				length -= offset;
				pBuffer += offset;
				bd=bd_next;
			}
		 }			  
		 if(length){
		  	if(AT_BD_EXT_LEN && AT_BD_EXT_LEN<=length){
		  		//free(bd);
#if defined(CONFIG_ARCH_MT85XX)
#if !PORTING
		  		bd=(TBD*)realloc(bd, sizeof(TBD)+AT_BD_EXT_LEN);
#endif
#endif
				MUSB_MemSet(bd, 0, sizeof(TBD)+AT_BD_EXT_LEN);
		  		TBD_SET_EXT_LEN(bd, AT_BD_EXT_LEN);
		  		TBD_SET_BUF_LEN(bd, length-AT_BD_EXT_LEN);
				TBD_SET_DATA(bd, pBuffer+AT_BD_EXT_LEN);
				// BYTE *_tmp=TBD_GET_EXT(bd);
				_tmp=TBD_GET_EXT(bd); // mhzhang
				MUSB_MemCopy(_tmp, pBuffer, AT_BD_EXT_LEN);
		  	}else{
				TBD_SET_EXT_LEN(bd, 0);
				TBD_SET_BUF_LEN(bd, length);
				TBD_SET_DATA(bd, pBuffer);
			}
			TBD_SET_FLAGS_EOL(bd);
			TBD_SET_NEXT(bd, 0);
			TBD_SET_CHKSUM(bd, 16);				
			LOG(6, "BD number %d %d\r\n", gpd_num,  i+1);
		}
		TGPD_SET_DATA(gpd, bd_head);
		TGPD_SET_FORMAT_BDP(gpd);
	}
    
	TGPD_SET_BUF_LEN(gpd, data_length-AT_GPD_EXT_LEN);			
	//TGPD_SET_BUF_LEN(gpd, 0);			
	TGPD_SET_EXT_LEN(gpd, AT_GPD_EXT_LEN);
	if(AT_GPD_EXT_LEN){
		_tmp=TGPD_GET_EXT(gpd);
		MUSB_MemCopy(_tmp, pBuf, AT_GPD_EXT_LEN);
	}
		  
	TGPD_SET_FORMAT_ZLP(gpd);
	TGPD_CLR_FORMAT_TGL(gpd);
	if(gpd_tx_ioc_en)
	{
	TGPD_SET_FORMAT_IOC(gpd);
    }
	else
	{
	  TGPD_CLR_FORMAT_IOC(gpd);	 
	}
	TGPD_SET_EPaddr(gpd, target_ep);

    //Create next GPD
    Tx_gpd_end[ep_num]=(TGPD*)MUSB_MemAlloc(sizeof(TGPD)+AT_GPD_EXT_LEN);//, 32);
    MUSB_MemSet(Tx_gpd_end[ep_num], 0 , sizeof(TGPD)+AT_GPD_EXT_LEN);
	TGPD_CLR_FLAGS_HWO(Tx_gpd_end[ep_num]);    
	TGPD_SET_NEXT(gpd, Tx_gpd_end[ep_num]); 	
	if(isHWO){
		TGPD_SET_FLAGS_HWO(gpd);
		TGPD_SET_CHKSUM(gpd, 16);		
	}else{
		TGPD_CLR_FLAGS_HWO(gpd);
        TGPD_SET_CHKSUM_HWO(gpd, 16);
	}

	if(_fgTxErrChkSumTestCase){ //Let check sum error for error test case.
	    Printf("*** MAKE QMU TX CHECK SUM ERROR *** \n");
	    gpd->chksum--;
	}

    #if !defined(CONFIG_ARCH_MT85XX)//defined(USB_RISC_CACHE_ENABLED)  
    HalFlushInvalidateDCache();
    #endif

	LOG(6, "[USB TX GPD@0x%08X] 0x%08X\n", ((unsigned int)gpd), *(unsigned int*)((unsigned int)gpd));
	LOG(6, "[USB TX GPD@0x%08X] 0x%08X\n", ((unsigned int)gpd), *(unsigned int*)((unsigned int)gpd+4));
	LOG(6, "[USB TX GPD@0x%08X] 0x%08X\n", ((unsigned int)gpd), *(unsigned int*)((unsigned int)gpd+8));
	LOG(6, "[USB TX GPD@0x%08X] 0x%08X\n", ((unsigned int)gpd), *(unsigned int*)((unsigned int)gpd+12));
	
	//Tx_gpd_last[ep_num]=gpd;
	//TGPD_CLR_FLAGS_HWO(gpd);
	return gpd;	
}

TGPD* GpdRxPrepare(BYTE*pBuf, unsigned int data_len, unsigned char ep_num, unsigned char target_ep, unsigned char _is_bdp){
	// unsigned int data_length, offset;
	// int i, seed;
	TGPD* gpd_last, *gpd_end;
	
	TGPD* gpd =(TGPD*)MUSB_MemAlloc(sizeof(TGPD));//, 32);
	 if(NULL ==gpd)
    	{
		return NULL;
	}
       MUSB_MemSet(gpd, 0, sizeof(TGPD));
        Printf("Rx GPD 0x%x\r\n", (unsigned int)gpd);
	TGPD_SET_DATA(gpd, pBuf);
	if(data_len&&data_len!=65535){
		data_len++;
	}
	//Printf("data_len 0x%x\r\n", data_len);
	TGPD_SET_DataBUF_LEN(gpd, data_len);
	//TGPD_SET_DataBUF_LEN(gpd, 0);
	TGPD_SET_BUF_LEN(gpd, 0);		
	TGPD_SET_FORMAT_ZLP(gpd);
	TGPD_CLR_FORMAT_TGL(gpd);
	TGPD_SET_FORMAT_IOC(gpd);
	TGPD_SET_EPaddr(gpd, target_ep);		  
	//TGPD* gpd_last;
	//TGPD* gpd_end=GetEndGpd(ep_num, &gpd_last, USB_RX);		  
	gpd_end=GetEndGpd(ep_num, &gpd_last, USB_RX);	// mhzhang
	TGPD_SET_NEXT(gpd, gpd_end);	  
	TGPD_SET_FLAGS_HWO(gpd);
	TGPD_SET_CHKSUM(gpd, 16);	

    #if !defined(CONFIG_ARCH_MT85XX)
    #if defined(USB_RISC_CACHE_ENABLED)  
    HalFlushInvalidateDCache();
    #endif
    #endif

	Printf("[USB RX GPD] 0x%08X\n", *(unsigned int*)((unsigned int)gpd));
	Printf("[USB RX GPD] 0x%08X\n", *(unsigned int*)((unsigned int)gpd+4));
	Printf("[USB RX GPD] 0x%08X\n", *(unsigned int*)((unsigned int)gpd+8));
	Printf("[USB RX GPD] 0x%08X\n", *(unsigned int*)((unsigned int)gpd+12));

	return gpd;	
}

TGPD* Prepare_Rx_GPD(TGPD*gpd, BYTE*pBuf, unsigned int data_len, unsigned char ep_num, unsigned char target_ep, unsigned char _is_bdp, unsigned char isHWO){
	extern unsigned char _fgRxErrChkSumTestCase;
#if defined(CONFIG_ARCH_MT85XX)
	BYTE *pPhyBuf;
#endif
	// unsigned int offset;
	// int i, seed;
	//TGPD* gpd =(TGPD*)x_alloc_aligned_mem(sizeof(TGPD),32);
       MUSB_MemSet(gpd, 0, sizeof(TGPD));
       //Printf("Rx GPD 0x%x\r\n", gpd);

#if defined(CONFIG_ARCH_MT85XX)
	pPhyBuf = (BYTE *)MUSB_DMA_MAP_FROM_DEV((UINT32)pBuf,data_len);
	PHY_TGPD_SET_DATA(gpd, pPhyBuf);
#else
	TGPD_SET_DATA(gpd, pBuf);
#endif
	if(data_len&&data_len!=65535){
		//data_len++;
	}
	//Printf("data_len 0x%x\r\n", data_len);
	TGPD_SET_DataBUF_LEN(gpd, data_len);
	//TGPD_SET_DataBUF_LEN(gpd, 0);
	TGPD_SET_BUF_LEN(gpd, 0);		
	TGPD_SET_FORMAT_ZLP(gpd);
	TGPD_CLR_FORMAT_TGL(gpd);
	if(gpd_rx_ioc_en)
	{
	TGPD_SET_FORMAT_IOC(gpd);
	}
	else
	{
	  TGPD_CLR_FORMAT_IOC(gpd);
	}
	TGPD_SET_EPaddr(gpd, target_ep);
	Rx_gpd_end[ep_num]=(TGPD*)MUSB_MemAlloc(sizeof(TGPD));//, 32);
	MUSB_MemSet(Rx_gpd_end[ep_num], 0 , sizeof(TGPD));
	//Printf("Rx Next GPD 0x%x\r\n", Rx_gpd_end[ep_num]);
	TGPD_CLR_FLAGS_HWO(Rx_gpd_end[ep_num]);    
	TGPD_SET_NEXT(gpd, Rx_gpd_end[ep_num]); 	
	if(isHWO){
		TGPD_SET_FLAGS_HWO(gpd);
		TGPD_SET_CHKSUM(gpd, 16);
	}else{
		TGPD_CLR_FLAGS_HWO(gpd);
		TGPD_SET_CHKSUM_HWO(gpd, 16);
	}

	if(_fgRxErrChkSumTestCase){ //Let check sum error for error test case.
	    Printf("*** MAKE QMU RX CHECK SUM ERROR *** \n");
	    gpd->chksum--;
	}

	//Rx_gpd_last[ep_num]=gpd;
	
	return gpd;	
}
	
void usb_start_transfer_QMU(int ep_num, int target_ep, USB_DIR dir){// int target_ep,
    USB_REQ *req = usb_get_req(ep_num, dir);
    // USB_EP_SETTING *setting = usb_get_ep_setting(ep_num, dir);
		
	TGPD* gpd_head; // mhzhang
    unsigned int QCR;
 
    // int i, j, seed, offset;
    //Printf("GPD_index: %d\r\n", GPD_Index);
    if(dir == USB_TX){
        if(req->isDma){
        	
                //set DMAReqEnab=0
                MGC_QWriteCsr16(MGC_O_HDRC_TXCSR, ep_num, MGC_QReadCsr16(MGC_O_HDRC_TXCSR, ep_num) |(MGC_M_TXCSR_DMAENAB));
                //Set DMA Ctrl: , direction=1, interrupt_enable=1, dma_mode=0, burst_mode=3
		  ori_gpd_num = gpd_num;
		  Printf("prepare GPD!!! %d\r\n", gpd_num);
		  // TGPD* gpd_head =GpdTxPrepare(req->buf, req->count, ep_num, target_ep, is_bdp);
		  gpd_head =GpdTxPrepare(req->buf, req->count, ep_num, target_ep, is_bdp); // mhzhang
		  //TGPD_SET_FLAGS_HWO(gpd_head);
                gpd_num = ori_gpd_num;
                
		 // unsigned int QCR = MGC_ReadQMU32(MGC_O_QMU_QCR0);
                  QCR = MGC_ReadQMU32(MGC_O_QMU_QCR0);
		  MGC_WriteQMU32(MGC_O_QMU_QCR0, QCR|DQMU_TQCS_EN(ep_num));
		  QCR = MGC_ReadQMU32(MGC_O_QMU_QCR2);
		  MGC_WriteQMU32(MGC_O_QMU_QCR2, QCR|DQMU_TX_ZLP(ep_num));
		  MGC_WriteQMU32(MGC_O_QMU_TQSAR(ep_num), (unsigned int)gpd_head);

		  MGC_WriteQIRQ32(MGC_O_QIRQ_TEPEMPMCR, DQMU_M_TX_EMPTY(ep_num));		 
		  MGC_WriteQIRQ32(MGC_O_QIRQ_TQEIMCR, DQMU_M_TX_LEN_ERR(ep_num)|DQMU_M_TX_BDCS_ERR(ep_num)|DQMU_M_TX_GPDCS_ERR(ep_num));//|DQMU_M_TX_GPDCS_ERR(ep_num)
		  MGC_WriteQIRQ32(MGC_O_QIRQ_TEPEIR, DQMU_M_TX_EP_ERR(ep_num));
		  
		  if(MGC_ReadQMU16(MGC_O_QMU_TQCSR(ep_num))&DQMU_QUE_ACTIVE){
		  	Printf("Active Now!\r\n");
		  	return;
		  }
		  MGC_WriteQMU16(MGC_O_QMU_TQCSR(ep_num), DQMU_QUE_START);
        }
        else{
            proc_epx_tx(ep_num);
            MGC_QWriteCsr16 (MGC_O_HDRC_TXCSR, ep_num, MGC_M_TXCSR_TXPKTRDY);
        }
    }
    else if(dir == USB_RX){
        if(req->isDma){        
		  MGC_QWriteCsr16(MGC_O_HDRC_RXCSR, ep_num, MGC_QReadCsr16(MGC_O_HDRC_RXCSR, ep_num) |(MGC_M_RXCSR_DMAENAB));
		  // TGPD*gpd_head= GpdRxPrepare(req->buf, req->count, ep_num, target_ep, is_bdp);
		  gpd_head= GpdRxPrepare(req->buf, req->count, ep_num, target_ep, is_bdp); // mhzhang
								
		  // unsigned int QCR = MGC_ReadQMU32(MGC_O_QMU_QCR0);
		  QCR = MGC_ReadQMU32(MGC_O_QMU_QCR0);
								
		  MGC_WriteQMU32(MGC_O_QMU_QCR0, QCR|DQMU_RQCS_EN(ep_num));
		  QCR = MGC_ReadQMU32(MGC_O_QMU_QCR3);
		  MGC_WriteQMU32(MGC_O_QMU_QCR3, QCR|DQMU_RX_ZLP(ep_num));

		  MGC_WriteQMU32(MGC_O_QMU_RQSAR(ep_num), (unsigned int)(gpd_head));
		  
		  MGC_WriteQIRQ32(MGC_O_QIRQ_REPEMPMCR, DQMU_M_RX_EMPTY(ep_num));
		  MGC_WriteQIRQ32(MGC_O_QIRQ_RQEIMCR, DQMU_M_RX_LEN_ERR(ep_num) |DQMU_M_RX_ZLP_ERR(ep_num)|DQMU_M_RX_GPDCS_ERR(ep_num));
		  MGC_WriteQIRQ32(MGC_O_QIRQ_REPEIR, DQMU_M_RX_EP_ERR(ep_num));
		  
		  if(MGC_ReadQMU16(MGC_O_QMU_RQCSR(ep_num))&DQMU_QUE_ACTIVE){
		  	Printf("Active Now!\r\n");
		  	return;
		  }
		  MGC_WriteQMU16(MGC_O_QMU_RQCSR(ep_num), DQMU_QUE_START);		  
                      
        }
        else{
            MGC_QWriteCsr16 (MGC_O_HDRC_RXCSR, ep_num, MGC_M_RXCSR_H_REQPKT);
        }
    }
}

void usb_insert_transfer_QMU(int ep_num, int target_ep, USB_DIR dir, unsigned char* buf, unsigned int count, unsigned char isHWO){
   // int i, j, seed, offset;
   
    TGPD* gpd; // mhzhang
	 
    Printf("(%d)into usb_insert_transfer_QMU dir=%d \r\n", count,dir);
    if(dir == USB_TX){  
		  LOG(6, "Tx_gpd_end[%d]:0x%x ep_num %d\r\n", ep_num, Tx_gpd_end[ep_num], ep_num);
		  gpd = Tx_gpd_end[ep_num];
		  Prepare_Tx_GPD(gpd, buf, count, ep_num, target_ep, is_bdp, isHWO);
    }
    else if(dir == USB_RX){               
           gpd = Rx_gpd_end[ep_num];
		  Prepare_Rx_GPD(gpd, buf, count, ep_num, target_ep, is_bdp, isHWO);
    }
}

void usb_start_QMU(int Q_num,  USB_DIR dir, unsigned char isMT){// int target_ep,    
    // int i, j, seed, offset;
    unsigned int QCR; // mhzhang
    unsigned int Temp; // mhzhang

    Printf("usb_start_qmu: Q_num= %d, dir=%d, isMT=%d, ioc_test = %d, ep_ioc_dis = %d\n", Q_num, dir, isMT, ioc_test, ep_ioc_dis);
    if(dir == USB_TX){           	
    	  //set DMAReqEnab=0
        MGC_QWriteCsr16(MGC_O_HDRC_TXCSR, Q_num, MGC_QReadCsr16(MGC_O_HDRC_TXCSR, Q_num) |(MGC_M_TXCSR_DMAENAB));
                
      // unsigned int QCR = MGC_ReadQMU32(MGC_O_QMU_QCR0);
      QCR = MGC_ReadQMU32(MGC_O_QMU_QCR0); // mhzhang
			
      MGC_WriteQMU32(MGC_O_QMU_QCR0, QCR|DQMU_TQCS_EN(Q_num));
      QCR = MGC_ReadQMU32(MGC_O_QMU_QCR2);
      if(isMT){
	QCR = QCR | DQMU_TX_MULTIPLE(Q_num);
      }
#if defined(CONFIG_ARCH_MT85XX)
		  if(zlp_dis)
		  {
		    MGC_WriteQMU32(MGC_O_QMU_QCR2, QCR);
		  }
		  else
#endif
		  {
      MGC_WriteQMU32(MGC_O_QMU_QCR2, QCR|DQMU_TX_ZLP(Q_num));
		  }
      if(ioc_test)
      {
        MGC_WriteQIRQ32(MGC_O_QIRQ_TEPEMPMCR, MGC_ReadQIRQ32(MGC_O_QIRQ_TEPEMPMCR) & ~ DQMU_M_TX_EMPTY(Q_num));
      }
      else
      {
	  MGC_WriteQIRQ32(MGC_O_QIRQ_TEPEMPMCR, DQMU_M_TX_EMPTY(Q_num));		 
    }
      MGC_WriteQIRQ32(MGC_O_QIRQ_TQEIMCR, DQMU_M_TX_LEN_ERR(Q_num)|DQMU_M_TX_BDCS_ERR(Q_num)|DQMU_M_TX_GPDCS_ERR(Q_num));
      MGC_WriteQIRQ32(MGC_O_QIRQ_TEPEIMCR, DQMU_M_TX_EP_ERR(Q_num));

    if(ep_ioc_dis)
    {
        MGC_WriteQIRQ32(MGC_O_QIRQ_IOCDISR, MGC_ReadQIRQ32(MGC_O_QIRQ_IOCDISR) | (MGC_O_QMU_TXIOC_DIS(Q_num)));
    }
    else
    {
        MGC_WriteQIRQ32(MGC_O_QIRQ_IOCDISR, MGC_ReadQIRQ32(MGC_O_QIRQ_IOCDISR) & ~(MGC_O_QMU_TXIOC_DIS(Q_num)));
    } 

      Temp = MGC_ReadQMU16(MGC_O_QMU_TQCSR(Q_num));
      Printf("MGC_O_QMU_TQCSR:0x%08X\n", Temp);

    if(MGC_ReadQMU16(MGC_O_QMU_TQCSR(Q_num))&DQMU_QUE_ACTIVE){
        
        LOG(6, "Tx %d Active Now!\r\n", Q_num);
        return;
    }

            //ben
            //BSP_CleanDCacheRange();
            //BSP_InvDCacheRange();	  
            //~ben
          #if defined(USB_RISC_CACHE_ENABLED)  
			#if !defined(CONFIG_ARCH_MT85XX)
	          HalFlushInvalidateDCache();
			#else
          //HalFlushInvalidateDCache();

		  	{
	        
    	        PGPD gpd = Tx_gpd_last[Q_num];
                do {
					//Printf("Flush TX gpd @0x%x\n",(unsigned int)gpd);
                    //BSP_FlushDCacheRange((UINT32)gpd,sizeof(TGPD));
                    MUSB_DMA_MAP_BIDIR((UINT32)gpd,sizeof(TGPD));
                    gpd = TGPD_GET_NEXT(gpd);
                } while ((gpd != Tx_gpd_end[Q_num])&&U_PHY_ADDR((UINT32)gpd)); 
				if(U_PHY_ADDR((UINT32)gpd)&&(Tx_gpd_last[Q_num] != Tx_gpd_end[Q_num]))
				{
    			    //Printf("Flush TX gpd @0x%x\n",(unsigned int)gpd);
    			    //BSP_FlushDCacheRange((UINT32)gpd,sizeof(TGPD));
                    MUSB_DMA_MAP_BIDIR((UINT32)gpd,sizeof(TGPD));
				}
	      }
			#endif // #if !defined(CONFIG_ARCH_MT85XX)
          #endif// #if defined(USB_RISC_CACHE_ENABLED)  
	  MGC_WriteQMU16(MGC_O_QMU_TQCSR(Q_num), DQMU_QUE_START);	         
    }
    else if(dir == USB_RX){               
		  MGC_QWriteCsr16(MGC_O_HDRC_RXCSR, Q_num, MGC_QReadCsr16(MGC_O_HDRC_RXCSR, Q_num) |(MGC_M_RXCSR_DMAENAB));

               // unsigned int QCR = MGC_ReadQMU32(MGC_O_QMU_QCR0);
               QCR = MGC_ReadQMU32(MGC_O_QMU_QCR0);  // mhzhang
		#if defined(CONFIG_ARCH_MT85XX)
		  MGC_WriteQMU32(MGC_O_QMU_QCR0, QCR|DQMU_RQCS_EN(Q_num));
		#endif
		  QCR = MGC_ReadQMU32(MGC_O_QMU_QCR3);
		  if(isMT){
			QCR = QCR | DQMU_RX_MULTIPLE(Q_num);
		  }
		  
		#if defined(CONFIG_ARCH_MT85XX)
		  if(zlp_dis)
		  {
		    MGC_WriteQMU32(MGC_O_QMU_QCR3, QCR);
		  }
		  else
		#endif
		  {
		  MGC_WriteQMU32(MGC_O_QMU_QCR3, QCR|DQMU_RX_ZLP(Q_num));
		  }
		  if(ioc_test)
		  {
		    MGC_WriteQIRQ32(MGC_O_QIRQ_REPEMPMCR, MGC_ReadQIRQ32(MGC_O_QIRQ_REPEMPMCR) & ~ DQMU_M_RX_EMPTY(Q_num));
		  }
		  else
		  {
		  MGC_WriteQIRQ32(MGC_O_QIRQ_REPEMPMCR, DQMU_M_RX_EMPTY(Q_num));
		  }
		  MGC_WriteQIRQ32(MGC_O_QIRQ_RQEIMCR,DQMU_M_RX_ZLP_ERR(Q_num)|DQMU_M_RX_LEN_ERR(Q_num) | DQMU_M_RX_LEN_ERR(Q_num)|DQMU_M_RX_GPDCS_ERR(Q_num));
		  MGC_WriteQIRQ32(MGC_O_QIRQ_REPEIMCR, DQMU_M_RX_EP_ERR(Q_num));
//#if defined(CC_MT8580)
          if(ep_ioc_dis)
          {
		     MGC_WriteQIRQ32(MGC_O_QIRQ_IOCDISR, MGC_ReadQIRQ32(MGC_O_QIRQ_IOCDISR) | (MGC_O_QMU_RXIOC_DIS(Q_num)));            
          }
          else
          {
		     MGC_WriteQIRQ32(MGC_O_QIRQ_IOCDISR, MGC_ReadQIRQ32(MGC_O_QIRQ_IOCDISR) & ~(MGC_O_QMU_RXIOC_DIS(Q_num)));
          } 
//#endif		  
		  if(MGC_ReadQMU16(MGC_O_QMU_RQCSR(Q_num))&DQMU_QUE_ACTIVE){
		  	LOG(6, "Rx %d Active Now!\r\n", Q_num);
		  	return;
		  }
          #if defined(USB_RISC_CACHE_ENABLED)  
			#if !defined(CONFIG_ARCH_MT85XX)
	          HalFlushInvalidateDCache();
			#else
          //HalFlushInvalidateDCache();
          
		  	{
	        
    	        PGPD gpd = Rx_gpd_last[Q_num];
                do {
                    //Printf("Flush RX gpd @0x%x\n",(unsigned int)gpd);
                    //BSP_FlushDCacheRange((UINT32)gpd,sizeof(TGPD));
                    MUSB_DMA_MAP_BIDIR((UINT32)gpd,sizeof(TGPD));
                    gpd = TGPD_GET_NEXT(gpd);
                } while ((gpd != Rx_gpd_end[Q_num])&&U_PHY_ADDR((UINT32)gpd));
				if(U_PHY_ADDR((UINT32)gpd)&&(Rx_gpd_last[Q_num] != Rx_gpd_end[Q_num]))
				{
                    //Printf("Flush TX gpd @0x%x\n",(unsigned int)gpd);
                    //BSP_FlushDCacheRange((UINT32)gpd,sizeof(TGPD));
                    MUSB_DMA_MAP_BIDIR((UINT32)gpd,sizeof(TGPD));
				}
	      }
			#endif // #if defined(CONFIG_ARCH_MT85XX)
          #endif // #if defined(USB_RISC_CACHE_ENABLED)  
	  MGC_WriteQMU16(MGC_O_QMU_RQCSR(Q_num), DQMU_QUE_START);	     
    }
}

void usb_stop_QMU(int Q_num,  USB_DIR dir){// int target_ep,    
  //  int i, j, seed, offset;
    //Printf("GPD_index: %d\r\n", GPD_Index);
    if(dir == USB_TX){           	  
		  if( !(MGC_ReadQMU16(MGC_O_QMU_TQCSR(Q_num)) & (DQMU_QUE_ACTIVE)) ){  
		  	Printf("Tx %d inActive Now!\r\n", Q_num);
		  	return;
		  }
		  MGC_WriteQMU16(MGC_O_QMU_TQCSR(Q_num), DQMU_QUE_STOP);	         
		  Printf("Tx %d stop Now!\r\n", Q_num);
    }
    else if(dir == USB_RX){	  
		  if(!(MGC_ReadQMU16(MGC_O_QMU_RQCSR(Q_num))&DQMU_QUE_ACTIVE)){
		  	Printf("Rx %d inActive Now!\r\n", Q_num);
		  	return;
		  }
		  MGC_WriteQMU16(MGC_O_QMU_RQCSR(Q_num), DQMU_QUE_STOP);
		  Printf("Rx %d stop Now!\r\n", Q_num);
    }
}

void usb_resume_QMU(int Q_num,  USB_DIR dir){// int target_ep,    
   // int i, j, seed, offset;
    //Printf("GPD_index: %d\r\n", GPD_Index);
    #if !defined(CONFIG_ARCH_MT85XX)//defined(USB_RISC_CACHE_ENABLED)  
    #if defined(USB_RISC_CACHE_ENABLED)  
    HalFlushInvalidateDCache();
    #endif
    #endif

    if(dir == USB_TX){      
			#if defined(CONFIG_ARCH_MT85XX)
			if(MGC_ReadQMU16(MGC_O_QMU_TQCSR(Q_num))&DQMU_QUE_ACTIVE)
			{
			    Printf("Tx %d Active Now!\r\n", Q_num);
			    return;
			}

		            
		  	{
	        
    	        PGPD gpd = Tx_gpd_last[Q_num];
                do {
                    //Printf("Flush TX gpd @0x%x\n",(unsigned int)gpd);
                    //BSP_FlushDCacheRange((UINT32)gpd,sizeof(TGPD));
                    MUSB_DMA_MAP_BIDIR((UINT32)gpd,sizeof(TGPD));
                    gpd = TGPD_GET_NEXT(gpd);
                } while ((gpd != Tx_gpd_end[Q_num])&&U_PHY_ADDR((UINT32)gpd));  
    			if(U_PHY_ADDR((UINT32)gpd)&&(Tx_gpd_last[Q_num] != Tx_gpd_end[Q_num]))
    			{
                    //Printf("Flush TX gpd @0x%x\n",(unsigned int)gpd);
                    //BSP_FlushDCacheRange((UINT32)gpd,sizeof(TGPD));
                    MUSB_DMA_MAP_BIDIR((UINT32)gpd,sizeof(TGPD));
    			}
	      }
			#endif // #if defined(CONFIG_ARCH_MT85XX)
		MGC_WriteQMU32(MGC_O_QMU_TQCSR(Q_num), DQMU_QUE_RESUME);	         
        if(!MGC_ReadQMU32( MGC_O_QMU_TQCSR(Q_num))){  //judge if Queue is still inactive
            Printf("Tx %d is not InActive Now!\r\n", Q_num);

            MGC_WriteQMU32( MGC_O_QMU_TQCSR(Q_num), DQMU_QUE_RESUME);
        }

		LOG(6, "Tx %d resume Now!\r\n", Q_num);

    }
    else if(dir == USB_RX){
			#if defined(CONFIG_ARCH_MT85XX)
	        if(MGC_ReadQMU16(MGC_O_QMU_RQCSR(Q_num))&DQMU_QUE_ACTIVE)
			{
		  	    Printf("Rx %d Active Now!\r\n", Q_num);
		  	    return;
		    }
	              
		  	{
	        
    	        PGPD gpd = Rx_gpd_last[Q_num];
                do {
                    //Printf("Flush RX gpd @0x%x\n",(unsigned int)gpd);
                    //BSP_FlushDCacheRange((UINT32)gpd,sizeof(TGPD));
                    MUSB_DMA_MAP_BIDIR((UINT32)gpd,sizeof(TGPD));
                    gpd = TGPD_GET_NEXT(gpd);
                } while ((gpd != Rx_gpd_end[Q_num])&&U_PHY_ADDR((UINT32)gpd));  
    			if(U_PHY_ADDR((UINT32)gpd)&&(Rx_gpd_last[Q_num] != Rx_gpd_end[Q_num]))
    			{
                    //Printf("Flush RX gpd @0x%x\n",(unsigned int)gpd);
                    //BSP_FlushDCacheRange((UINT32)gpd,sizeof(TGPD));
                    MUSB_DMA_MAP_BIDIR((UINT32)gpd,sizeof(TGPD));
    			}
	      }
			#endif// #if defined(CONFIG_ARCH_MT85XX)
        MGC_WriteQMU32(MGC_O_QMU_RQCSR(Q_num), DQMU_QUE_RESUME);
		//Printf("Rx %d resume Now!\r\n", Q_num);
        if(!MGC_ReadQMU32( MGC_O_QMU_RQCSR(Q_num))){   //judge if Queue is still inactive
            MGC_WriteQMU32( MGC_O_QMU_RQCSR(Q_num), DQMU_QUE_RESUME);
        }
    }
}

void usb_restart_QMU(int Q_num,  USB_DIR dir){// int target_ep,    
    //int i, j, seed, offset;
    //Printf("GPD_index: %d\r\n", GPD_Index);
    if(dir == USB_TX){       	
		#if defined(CONFIG_ARCH_MT85XX)
		  if(Tx_gpd_last[Q_num] == Tx_gpd_end[Q_num])
		  {
	        
    	        PGPD gpd = Tx_gpd_last[Q_num];
                do {
                    //Printf("Flush TX gpd @0x%x\n",(unsigned int)gpd);
                    //BSP_FlushDCacheRange((UINT32)gpd,sizeof(TGPD));
                    MUSB_DMA_MAP_BIDIR((UINT32)gpd,sizeof(TGPD));
                    gpd = TGPD_GET_NEXT(gpd);
                } while ((gpd != Tx_gpd_end[Q_num])&&U_PHY_ADDR((UINT32)gpd)); 
				if(U_PHY_ADDR((UINT32)gpd)&&(Tx_gpd_last[Q_num] != Tx_gpd_end[Q_num]))
				{
                    //Printf("Flush TX gpd @0x%x\n",(unsigned int)gpd);
                    //BSP_FlushDCacheRange((UINT32)gpd,sizeof(TGPD));
                    MUSB_DMA_MAP_BIDIR((UINT32)gpd,sizeof(TGPD));
				}
	      }
		#endif
		  MGC_WriteQMU16(MGC_O_QMU_TQCSR(Q_num), DQMU_QUE_START);	         
		  //Printf("Tx %d resume Now!\r\n", Q_num);
    }
    else if(dir == USB_RX){    	
		#if defined(CONFIG_ARCH_MT85XX)
		  if(Rx_gpd_last[Q_num] == Rx_gpd_end[Q_num])
		  {
	        
    	        PGPD gpd = Rx_gpd_last[Q_num];
                do {
                    //Printf("Flush RX gpd @0x%x\n",(unsigned int)gpd);
                    //BSP_FlushDCacheRange((UINT32)gpd,sizeof(TGPD));
                    MUSB_DMA_MAP_BIDIR((UINT32)gpd,sizeof(TGPD));
                    gpd = TGPD_GET_NEXT(gpd);
                } while ((gpd != Rx_gpd_end[Q_num])&&U_PHY_ADDR((UINT32)gpd));
				if(U_PHY_ADDR((UINT32)gpd)&&(Rx_gpd_last[Q_num] != Rx_gpd_end[Q_num]))
				{
                    //Printf("Flush TX gpd @0x%x\n",(unsigned int)gpd);
                    //BSP_FlushDCacheRange((UINT32)gpd,sizeof(TGPD));
                    MUSB_DMA_MAP_BIDIR((UINT32)gpd,sizeof(TGPD));
				}
	      }
		#endif
		  MGC_WriteQMU16(MGC_O_QMU_RQCSR(Q_num), DQMU_QUE_START);
		  //Printf("Rx %d resume Now!\r\n", Q_num);
    }
}

void Flush_QMU(int Q_num,  USB_DIR dir){// int target_ep,    
    // int i, j, seed, offset;

    UINT16 wVal; // mhzhang
    TGPD* gpd_current;

    USB_REQ *req = usb_get_req(Q_num, dir); 

    if(!req)
    {
        Printf("Flush_QMU: usb_get_req fail\n");
        return;
    }
    
    //Printf("GPD_index: %d\r\n", GPD_Index);
    if(dir == USB_TX)
    {           	  
        // UINT16 wVal = MGC_QReadCsr16(MGC_O_HDRC_TXCSR, Q_num);
        wVal = MGC_QReadCsr16(MGC_O_HDRC_TXCSR, Q_num);
        if(!(wVal&MGC_M_TXCSR_TXPKTRDY))
        {
        	MGC_QWriteCsr16(MGC_O_HDRC_TXCSR, Q_num, (wVal&0xFFF0)|MGC_M_TXCSR_FLUSHFIFO|MGC_M_TXCSR_TXPKTRDY);
        }

        wVal=(wVal&0xFFF0)|MGC_M_TXCSR_FLUSHFIFO;

        MGC_QWriteCsr16(MGC_O_HDRC_TXCSR, Q_num, wVal);
        MGC_QWriteCsr16(MGC_O_HDRC_TXCSR, Q_num, wVal);
		
		gpd_current = (TGPD*)(MGC_ReadQMU32(MGC_O_QMU_TQCPR(Q_num)));
		#if defined(CONFIG_ARCH_MT85XX)
        MUSB_DMA_UNMAP_BIDIR((UINT32)gpd_current,sizeof(TGPD));
        gpd_current = (TGPD*)U_VIRT_ADDR((UINT32)gpd_current);
		#endif
		
		if(gpd_current == Tx_gpd_end[Q_num])
		{
			Tx_gpd_last[Q_num] = gpd_current;
			MGC_WriteQMU32(MGC_O_QMU_TQSAR(Q_num), (unsigned int)(U_PHY_ADDR((unsigned int)Tx_gpd_last[Q_num])));
			req->complete=TRUE;
		}
		else if(TGPD_GET_NEXT(gpd_current)==Tx_gpd_end[Q_num])
		{
			Tx_gpd_last[Q_num]=Tx_gpd_end[Q_num];
			Printf("Flush_QMU(%d): free@0x%08X\n", __LINE__, (unsigned int)gpd_current);
			if (isFreeGPD)			
			{
		#if defined(CONFIG_ARCH_MT85XX)
    			MUSB_MemFree(gpd_current);
		#endif
    			gpd_current = NULL;
			}
			MGC_WriteQMU32(MGC_O_QMU_TQSAR(Q_num), (unsigned int)(U_PHY_ADDR((unsigned int)Tx_gpd_last[Q_num])));
			req->complete=TRUE;
		}
		else
		{
			Tx_gpd_last[Q_num]=TGPD_GET_NEXT(gpd_current);			
			Printf("Flush_QMU(%d): free@0x%08X\n", __LINE__, (unsigned int)gpd_current);
			if (isFreeGPD)			
			{
		#if defined(CONFIG_ARCH_MT85XX)
    			MUSB_MemFree(gpd_current); //2010.05.20 for test
		#endif
    			gpd_current = NULL;
			}

			MGC_WriteQMU32(MGC_O_QMU_TQSAR(Q_num), (unsigned int)(U_PHY_ADDR((unsigned int)Tx_gpd_last[Q_num])));
		}
		Printf("TxQ %d Flush Now!\r\n", Q_num);
    }
    else if(dir == USB_RX)
    {	  
		// UINT16 wVal = MGC_QReadCsr16(MGC_O_HDRC_RXCSR, Q_num);    		
		wVal = MGC_QReadCsr16(MGC_O_HDRC_RXCSR, Q_num);    		
    		wVal=(wVal&0xFFFE)|MGC_M_RXCSR_FLUSHFIFO;
		MGC_QWriteCsr16(MGC_O_HDRC_RXCSR, Q_num, wVal);
		MGC_QWriteCsr16(MGC_O_HDRC_RXCSR, Q_num, wVal);
		
		gpd_current = (TGPD*)(MGC_ReadQMU32(MGC_O_QMU_RQCPR(Q_num)));
		#if defined(CONFIG_ARCH_MT85XX)
        MUSB_DMA_UNMAP_BIDIR((UINT32)gpd_current,sizeof(TGPD));
        gpd_current = (TGPD*)U_VIRT_ADDR((UINT32)gpd_current);
		#endif

		if(gpd_current == Rx_gpd_end[Q_num])
		{
			//Printf(">>>>>>>>>>>>>>>>>>>>>>11111111111\r\n");
			MGC_WriteQMU32(MGC_O_QMU_RQSAR(Q_num), (unsigned int)(U_PHY_ADDR((unsigned int)Rx_gpd_last[Q_num])));
			Rx_gpd_last[Q_num] = gpd_current;
			req->complete=TRUE;
		}
		else if(TGPD_GET_NEXT(gpd_current)==Rx_gpd_end[Q_num])
	    {
			//Printf(">>>>>>>>>>>>>>>>>>>>>>2222222222\r\n");
			Rx_gpd_last[Q_num]=Rx_gpd_end[Q_num];
			MGC_WriteQMU32(MGC_O_QMU_RQSAR(Q_num), (unsigned int)(U_PHY_ADDR((unsigned int)Rx_gpd_last[Q_num])));
			req->complete=TRUE;
			Printf("Flush_QMU(%d): free@0x%08X\n", __LINE__, (unsigned int)gpd_current);
		#if defined(CONFIG_ARCH_MT85XX)
			MUSB_MemFree(gpd_current);
		#endif
			gpd_current = NULL;
		}
		else
		{
			//Printf(">>>>>>>>>>>>>>>>>>>>>>3333333333\r\n");
			Rx_gpd_last[Q_num]=TGPD_GET_NEXT(gpd_current);			
			MGC_WriteQMU32(MGC_O_QMU_RQSAR(Q_num), (unsigned int)(U_PHY_ADDR((unsigned int)Rx_gpd_last[Q_num])));
			Printf("Flush_QMU(%d): free@0x%08X\n", __LINE__, (unsigned int)gpd_current);
		#if defined(CONFIG_ARCH_MT85XX)
			MUSB_MemFree(gpd_current);
		#endif
			gpd_current = NULL;
		}
		Printf("RxQ %d Flush Now!\r\n", Q_num);
    }
}

void proc_qmu_rx(int ep_num)
{
    //int i;   
	#if defined(CONFIG_ARCH_MT85XX)
    TGPD*gpd_free; // mhzhang
	#endif
    USB_REQ *req = usb_get_req(ep_num, USB_RX);
    TGPD* gpd=Rx_gpd_last[ep_num];
    TGPD* gpd_current = (TGPD*)(U_VIRT_ADDR(MGC_ReadQMU32(MGC_O_QMU_RQCPR(ep_num))));		
    //TGPD* gpd_last = (TGPD*) MGC_ReadQMU32(MGC_O_QMU_RQLDPR(ep_num));

    //Printf("Rx_gpd_last 0x%x, gpd_current 0x%x, gpd_end 0x%x,  gpd_last 0x%x\r\n",  gpd, gpd_current, Rx_gpd_end[ep_num], gpd_last);
    if(gpd==gpd_current)
    {
    	return;
    }

    while(gpd != gpd_current && !TGPD_IS_FLAGS_HWO(gpd))
    {
	#if defined(CONFIG_ARCH_MT85XX)
        //BSP_InvDCacheRange((UINT32)gpd,sizeof(TGPD));
        MUSB_DMA_UNMAP_BIDIR(U_PHY_ADDR((UINT32)gpd),sizeof(TGPD));
	#endif

		Printf("Rx gpd info { HWO %d, Next_GPD %x ,DataBufferLength %d, DataBuffer %x, Recived Len %d, Endpoint %d, TGL %d, ZLP %d}\r\n", 
		        (unsigned int)TGPD_GET_FLAG(gpd), (unsigned int)TGPD_GET_NEXT(gpd), 
		        (unsigned int)TGPD_GET_DataBUF_LEN(gpd), (unsigned int)TGPD_GET_DATA(gpd), 
		        (unsigned int)TGPD_GET_BUF_LEN(gpd), (unsigned int)TGPD_GET_EPaddr(gpd), 
		        (unsigned int)TGPD_GET_TGL(gpd), (unsigned int)TGPD_GET_ZLP(gpd));
		// TGPD*gpd_free=gpd;
	#if defined(CONFIG_ARCH_MT85XX)
		gpd_free=gpd;
        //BSP_InvDCacheRange((UINT32)TGPD_GET_DATA(gpd),gpd->bufLen);
        MUSB_DMA_UNMAP_FROM_DEV((UINT32)PHY_TGPD_GET_DATA(gpd),gpd->bufLen);
	#endif
		gpd=TGPD_GET_NEXT(gpd);	
	#if defined(CONFIG_ARCH_MT85XX)
		MUSB_MemFree(gpd_free);
    	gpd_free = NULL;
	#endif
    }    
    //MGC_WriteQMU32(MGC_O_QMU_RQSAR(ep_num), gpd);
    Rx_gpd_last[ep_num]=gpd;
    if(Rx_gpd_last[ep_num]==Rx_gpd_end[ep_num])
    {
        //if(!TGPD_IS_FLAGS_HWO(gpd_current)){
    	req->complete = TRUE;
    	Printf("Rx %d complete\r\n", ep_num);    	
    }        
}

void proc_qmu_tx(int ep_num){
	
    TGPD*gpd_free; // mhzhang
	
    USB_REQ *req = usb_get_req(ep_num, USB_TX);
   // unsigned dataCount;
   // int i=0;    

	
    TGPD* gpd=Tx_gpd_last[ep_num];
    TGPD* gpd_current = (TGPD*)(U_VIRT_ADDR(MGC_ReadQMU32(MGC_O_QMU_TQCPR(ep_num))));

    LOG(6, "Tx_gpd_last 0x%x, gpd_current 0x%x, gpd_end 0x%x, \r\n",  gpd, gpd_current, Tx_gpd_end[ep_num]);
    if(gpd==gpd_current){
	return;
    }    
    while(gpd!=gpd_current && !TGPD_IS_FLAGS_HWO(gpd)){
		 LOG(6, "Tx gpd %x info { HWO %d, BPD %d, Next_GPD %x , DataBuffer %x, BufferLen %d, Endpoint %d}\r\n", gpd, TGPD_GET_FLAG(gpd), TGPD_GET_FORMAT(gpd), TGPD_GET_NEXT(gpd), TGPD_GET_DATA(gpd), TGPD_GET_BUF_LEN(gpd), TGPD_GET_EPaddr(gpd));
	#if defined(CONFIG_ARCH_MT85XX)
        //BSP_InvDCacheRange((UINT32)gpd,sizeof(TGPD));
        MUSB_DMA_UNMAP_BIDIR(U_PHY_ADDR((UINT32)gpd),sizeof(TGPD));
	#endif
		 // TGPD*gpd_free=gpd;
		gpd_free=gpd;
	#if defined(CONFIG_ARCH_MT85XX)
        MUSB_DMA_UNMAP_TO_DEV((UINT32)PHY_TGPD_GET_DATA(gpd),gpd->bufLen);
	#endif
		gpd=TGPD_GET_NEXT(gpd);			
		if (isFreeGPD)		
		{
    		if(TGPD_IS_FORMAT_BDP(gpd_free))
    		{
		        LOG(6, "Free bd:");
    			Free_BD(gpd_free);
    		}
            LOG(6, "Free gpd");
	#if defined(CONFIG_ARCH_MT85XX)
    		MUSB_MemFree(gpd_free);				
	#endif
        	gpd_free = NULL;
		}
    }
    //MGC_WriteQMU32(MGC_O_QMU_TQSAR(ep_num), gpd);
    Tx_gpd_last[ep_num]=gpd;   		
    if(Tx_gpd_last[ep_num]==Tx_gpd_end[ep_num]){
    //if(!TGPD_IS_FLAGS_HWO(gpd_current)){
    	req->complete = TRUE;
    	Printf("Tx %d complete\r\n", ep_num);    	
    }       
}

void proc_devqmu_rx(int ep_num){    
    // int i;
    
	#if defined(CONFIG_ARCH_MT85XX)
    TGPD*gpd_free; // mhzhang
	#endif
		
    USB_REQ *req = usb_get_req(ep_num, USB_RX);    

	
    TGPD* gpd=Rx_gpd_last[ep_num];
    TGPD* gpd_current = (TGPD*)(U_VIRT_ADDR(MGC_ReadQMU32(MGC_O_QMU_RQCPR(ep_num))));


    // TGPD* gpd_last = MGC_ReadQMU32(MGC_O_QMU_RQLDPR(ep_num));
    //Printf("Rx_gpd_last 0x%x, gpd_current 0x%x, gpd_end 0x%x,  gpd_last 0x%x\r\n",  gpd, gpd_current, Rx_gpd_end[ep_num], gpd_last);
    if(gpd==gpd_current){
	return;
    }    
    while(gpd!=gpd_current&& !TGPD_IS_FLAGS_HWO(gpd)){
		LOG(6, "Rx gpd info { HWO %d, Next_GPD %x , Data buffer length %d,  DataBuffer %x, Recived Len %d, Endpoint %d, TGL %d, ZLP %d}\r\n", (unsigned int)TGPD_GET_FLAG(gpd), (unsigned int)TGPD_GET_NEXT(gpd), (unsigned int)TGPD_GET_DataBUF_LEN(gpd), (unsigned int)TGPD_GET_DATA(gpd), (unsigned int)TGPD_GET_BUF_LEN(gpd), (unsigned int)TGPD_GET_EPaddr(gpd), (unsigned int)TGPD_GET_TGL(gpd), (unsigned int)TGPD_GET_ZLP(gpd));
		usb_insert_transfer_QMU(2, 2, USB_TX, TGPD_GET_DATA(gpd), TGPD_GET_BUF_LEN(gpd), TRUE);
		// TGPD*gpd_free=gpd;
	#if defined(CONFIG_ARCH_MT85XX)
		gpd_free=gpd;
	#endif
		gpd=TGPD_GET_NEXT(gpd);	
	#if defined(CONFIG_ARCH_MT85XX)
		MUSB_MemFree(gpd_free);
	#endif
    }    
    //MGC_WriteQMU32(MGC_O_QMU_RQSAR(ep_num), gpd);    
    Rx_gpd_last[ep_num]=gpd;
    if(Rx_gpd_last[ep_num]==Rx_gpd_end[ep_num]){
    //if(!TGPD_IS_FLAGS_HWO(gpd_current)){
    	req->complete = TRUE;
    	Printf("Rx %d complete\r\n", ep_num);    	
    }        
}

void proc_devqmu_tx(int ep_num){

	 TGPD*gpd_free; // mhzhang
	 
    USB_REQ *req = usb_get_req(ep_num, USB_TX);
    // unsigned dataCount;
    // int i=0;    

#if 0 // mhzhang    ????
    TGPD* gpd=Tx_gpd_last[ep_num];
    TGPD* gpd_current = MGC_ReadQMU32(MGC_O_QMU_TQCPR(ep_num));
#else		
    TGPD* gpd=Tx_gpd_last[ep_num];
    TGPD* gpd_current = (TGPD*)(U_VIRT_ADDR(MGC_ReadQMU32(MGC_O_QMU_TQCPR(ep_num))));		
#endif

    //Printf("Tx_gpd_last 0x%x, gpd_current 0x%x, gpd_end 0x%x, \r\n",  gpd, gpd_current, Tx_gpd_end[ep_num]);
    if(gpd==gpd_current){
	return;
    }    
    while(gpd!=gpd_current && !TGPD_IS_FLAGS_HWO(gpd)){
		// Printf("Tx gpd %x info { HWO %d, BPD %d, Next_GPD %x , DataBuffer %x, BufferLen %d, Endpoint %d}\r\n", gpd, TGPD_GET_FLAG(gpd), TGPD_GET_FORMAT(gpd), TGPD_GET_NEXT(gpd), TGPD_GET_DATA(gpd), TGPD_GET_BUF_LEN(gpd), TGPD_GET_EPaddr(gpd));
		// TGPD*gpd_free=gpd;
		gpd_free=gpd;
		gpd=TGPD_GET_NEXT(gpd);			
		if(TGPD_IS_FORMAT_BDP(gpd_free)){
			Free_BD(gpd_free);
		}
	#if defined(CONFIG_ARCH_MT85XX)
		MUSB_MemFree(gpd_free);
	#endif
    	gpd_free = NULL;
    }    
    Tx_gpd_last[ep_num]=gpd;   		
    if(Tx_gpd_last[ep_num]==Tx_gpd_end[ep_num]){   
    	req->complete = TRUE;
    	//Printf("Tx %d complete\r\n", ep_num);    	
    } else{
	//TGPD_SET_FLAGS_HWO(Tx_gpd_last[ep_num]);
	//usb_resume_QMU(ep_num, USB_TX);
    }
}

void proc_ixia_qmu_rx(int ep_num){    
//    int distance;
    unsigned int from;
    unsigned int to;
    TGPD* gpd=Rx_gpd_last[ep_num];
    TGPD* gpd_current=(TGPD*)(U_VIRT_ADDR(MGC_ReadQMU32(MGC_O_QMU_RQCPR(ep_num))));

    //Printf("Rx_gpd_last 0x%08X, gpd_current 0x%08X, gpd_end 0x%08X,\r\n",  (int)gpd, (int)gpd_current, (int)Rx_gpd_end[ep_num]);
    if(gpd==gpd_current)
    {
        Printf("*** Rx_gpd_last==gpd_current ***\n");
    }
    
    while (
        (!TGPD_IS_FLAGS_HWO(gpd))
        && (gpd!=gpd_current)){	//&&i<1000	
		//Printf("Rx gpd info { HWO %d, Next_GPD %x ,DataBufferLength %d, DataBuffer %x, Recived Len %d, Endpoint %d}\r\n", TGPD_GET_FLAG(gpd), TGPD_GET_NEXT(gpd), TGPD_GET_DataBUF_LEN(gpd), TGPD_GET_DATA(gpd), TGPD_GET_BUF_LEN(gpd), TGPD_GET_EPaddr(gpd));
		TGPD_SET_FLAGS_HWO(Tx_gpd_end[ep_num]);
		Tx_gpd_end[ep_num]=TGPD_GET_NEXT(Tx_gpd_end[ep_num]);

		TGPD_SET_FLAGS_HWO(Rx_gpd_end[ep_num]);
		Rx_gpd_end[ep_num]=TGPD_GET_NEXT(Rx_gpd_end[ep_num]);

		gpd=TGPD_GET_NEXT(gpd);		

		_u4RxCount[ep_num-1][QMU_CURR]++;
    }    

    from = (unsigned int)Rx_gpd_last[ep_num];
    to = (unsigned int)gpd;
    
    if ((_u4RxCount[ep_num-1][QMU_LAST]%GPD_LIST_LEN) < (_u4RxCount[ep_num-1][QMU_CURR]%GPD_LIST_LEN))
    {
        if (!(from < to))
        {
            Printf("(!(from < to)) %d %d\n", from, to);
        }
    }
    else if ((_u4RxCount[ep_num-1][QMU_LAST]%GPD_LIST_LEN) > (_u4RxCount[ep_num-1][QMU_CURR]%GPD_LIST_LEN))
    {
        if (!(from > to))
        {
            Printf("(!(from > to)) %d %d\n", from, to);
        }
    }

    Printf("-> Rx %d (%d) (%d, %d) (+%d)\r\n", (ep_num-1),
        (int)(_u4RxCount[ep_num-1][QMU_LAST]),        
        (int)(_u4RxCount[ep_num-1][QMU_LAST]%GPD_LIST_LEN),
        (int)(_u4RxCount[ep_num-1][QMU_CURR]%GPD_LIST_LEN),
        (int)(_u4RxCount[ep_num-1][QMU_CURR]-_u4RxCount[ep_num-1][QMU_LAST]));

    usb_resume_QMU(ep_num, USB_TX);
    usb_resume_QMU(ep_num, USB_RX);	

    Rx_gpd_last[ep_num]=gpd;    
    _u4RxCount[ep_num-1][QMU_LAST] = 
        _u4RxCount[ep_num-1][QMU_CURR];
}

void proc_ixia_qmu_tx(int ep_num){
    TGPD* gpd=Tx_gpd_last[ep_num];
    TGPD* gpd_current=(TGPD*)(U_VIRT_ADDR(MGC_ReadQMU32(MGC_O_QMU_TQCPR(ep_num))));

    //Printf("Tx_gpd_last 0x%08X, gpd_current 0x%08X, gpd_end 0x%08X, \r\n", (int) gpd, (int)gpd_current, (int)Tx_gpd_end[ep_num]);
    while (
        (!TGPD_IS_FLAGS_HWO(gpd))
        &&(gpd!=gpd_current)){//&&i<1000
		//Printf("Tx gpd %x info { HWO %d, BPD %d, Next_GPD %x , DataBuffer %x, BufferLen %d, Endpoint %d}\r\n", gpd, TGPD_GET_FLAG(gpd), TGPD_GET_FORMAT(gpd), TGPD_GET_NEXT(gpd), TGPD_GET_DATA(gpd), TGPD_GET_BUF_LEN(gpd), TGPD_GET_EPaddr(gpd));

		gpd=TGPD_GET_NEXT(gpd);	
		//TGPD_SET_FLAGS_HWO(Rx_gpd_end[ep_num]);
		//Rx_gpd_end[ep_num]=TGPD_GET_NEXT(Rx_gpd_end[ep_num]);

		_u4TxCount[ep_num-1][QMU_CURR]++;
    }

    Tx_gpd_last[ep_num]=gpd;       

    //usb_resume_QMU(ep_num, USB_RX);	

    Printf("<- Tx %d (%d) (%d, %d) (+%d)\r\n", ep_num-1, 
        (int)_u4TxCount[ep_num-1][QMU_LAST],
        (int)(_u4TxCount[ep_num-1][QMU_LAST]%GPD_LIST_LEN),
        (int)(_u4TxCount[ep_num-1][QMU_CURR]%GPD_LIST_LEN),
        (int)(_u4TxCount[ep_num-1][QMU_CURR]-_u4TxCount[ep_num-1][QMU_LAST]));

    _u4TxCount[ep_num-1][QMU_LAST] = 
        _u4TxCount[ep_num-1][QMU_CURR];    
}

void proc_ixia_rx_empty(int ep_num){    
    int i=0;
    //unsigned char *buf = (unsigned char*)x_alloc_aligned_mem(2048,32);    
    //usb_insert_transfer_QMU(ep_num, 3, USB_RX, ixia_buf[ep_num-1][i], 2048, false);
    for(i=0; i<100; i++){
    	//buf = (unsigned char*)x_alloc_aligned_mem(2048,32);    
	usb_insert_transfer_QMU(ep_num, 3, USB_RX, (unsigned char*)ixia_buf[ep_num-1][i], 2048, TRUE);
    	usb_resume_QMU(ep_num, USB_RX);
    }
    //TGPD_SET_FLAGS_HWO(Rx_gpd_last[ep_num]);    
}

void Free_BD(TGPD* gpd){
	TBD* bd_current=(TBD*)TGPD_GET_DATA(gpd);
	#if defined(CONFIG_ARCH_MT85XX)
	TBD* bd_pre;
	#endif
	while(!TBD_IS_FLAGS_EOL(bd_current)){
	#if defined(CONFIG_ARCH_MT85XX)
		bd_pre=bd_current;
	#endif
		bd_current=TBD_GET_NEXT(bd_current);		
	#if defined(CONFIG_ARCH_MT85XX)
		MUSB_MemFree(bd_pre);
    	bd_pre = NULL;
	#endif
	}
	#if defined(CONFIG_ARCH_MT85XX)
	MUSB_MemFree(bd_current);
	#endif
    bd_current = NULL;
}   

