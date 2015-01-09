/********************************************************************************************
 *     LEGAL DISCLAIMER 
 *
 *     (Header of MediaTek Software/Firmware Release or Documentation)
 *
 *     BY OPENING OR USING THIS FILE, BUYER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES 
 *     THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE") RECEIVED 
 *     FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO BUYER ON AN "AS-IS" BASIS 
 *     ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES, EXPRESS OR IMPLIED, 
 *     INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR 
 *     A PARTICULAR PURPOSE OR NONINFRINGEMENT. NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY 
 *     WHATSOEVER WITH RESPECT TO THE SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, 
 *     INCORPORATED IN, OR SUPPLIED WITH THE MEDIATEK SOFTWARE, AND BUYER AGREES TO LOOK 
 *     ONLY TO SUCH THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. MEDIATEK SHALL ALSO
 *     NOT BE RESPONSIBLE FOR ANY MEDIATEK SOFTWARE RELEASES MADE TO BUYER'S SPECIFICATION 
 *     OR TO CONFORM TO A PARTICULAR STANDARD OR OPEN FORUM.
 *     
 *     BUYER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND CUMULATIVE LIABILITY WITH 
 *     RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE, AT MEDIATEK'S OPTION, 
 *     TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE, OR REFUND ANY SOFTWARE LICENSE 
 *     FEES OR SERVICE CHARGE PAID BY BUYER TO MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE. 
 *     
 *     THE TRANSACTION CONTEMPLATED HEREUNDER SHALL BE CONSTRUED IN ACCORDANCE WITH THE LAWS 
 *     OF THE STATE OF CALIFORNIA, USA, EXCLUDING ITS CONFLICT OF LAWS PRINCIPLES.  
 ************************************************************************************************/

/*
 * Top-level Serial class driver module
 * $Revision: #1 $
 */
#ifdef MUSB_SERIAL

#include "mu_bits.h"
#include "mu_cdi.h"
#include "mu_descs.h"
#include "mu_diag.h"
#include "mu_mem.h"
#include "mu_stdio.h"
#include "mu_impl.h"
#include "mu_serial.h"
#include "mu_serial_pl2303.h"
#include "uart_if.h"

static uint8_t MGC_bPL2303Init = FALSE;

/* PL2303 related object */
static MGC_SerialPL2303 MGC_arPL2303[MUSB_SERIAL_DEVICE_NUM];

static MUSB_pfSerialPL2303InsertNfy MGC_pfSerialPL2303Insert;   

void MUSB_PL2303Init(void)
{
    uint8_t bIndex = 0;

    if (MGC_bPL2303Init)
    {
        return;
    }
    MGC_bPL2303Init = TRUE;    

    for(bIndex=0; bIndex<MUSB_SERIAL_DEVICE_NUM; bIndex++)
    {
        MGC_arPL2303[bIndex].bIndex = bIndex;
    }
}

void MUSB_PL2303Register(MGC_SerialDevice *pSerialDevice)
{
    uint8_t bIndex;
    uint8_t bIdCount = 0;    
    MUSB_HOTSWAP_NFY_INFO_T *pt_hotswap_nfy_info;
	
    MUSB_ASSERT(pSerialDevice);

    for(bIndex=0; bIndex<MUSB_SERIAL_DEVICE_NUM; bIndex++)
    {
        if (MGC_arPL2303[bIndex].pSerialDevice == NULL)
        {
            MGC_arPL2303[bIndex].bSerialDevId = bIdCount;
            MGC_arPL2303[bIndex].pSerialDevice = pSerialDevice;
			// call back to upper layer when device already insert.
		   if ((pSerialDevice->pUsbDevice) &&
			   (pSerialDevice->pt_socket_nfy_info))
		   {
			   pt_hotswap_nfy_info = pSerialDevice->pt_socket_nfy_info;
			   if (pt_hotswap_nfy_info->pfNfy)
			   {
				   pt_hotswap_nfy_info->pvDevice = (void *)pSerialDevice->pUsbDevice;
				   pt_hotswap_nfy_info->u4Status = (uint32_t)HFI_STATUS_READY;
				   pt_hotswap_nfy_info->u4SocketNo = (uint32_t)pSerialDevice->pUsbDevice->bHubPort;
				   pt_hotswap_nfy_info->u4UsbPort = pSerialDevice->pPort->bUsbPort; 					   
				   pt_hotswap_nfy_info->pfNfy(pt_hotswap_nfy_info->pvTag);
				   LOG(0, "SERIAL STATUS READY Notify.\n", NULL);
			   }
		   }
            /*Register the start */
            bIdCount ++;
        }
    }

    pSerialDevice->pSerialDriverTag = MGC_arPL2303;
    return;
}


void MUSB_PL2303Release(MGC_SerialDevice *pSerialDevice)
{
    uint8_t bIndex;
    MUSB_HOTSWAP_NFY_INFO_T *pt_hotswap_nfy_info;
		
    MUSB_ASSERT(pSerialDevice);

    for(bIndex=0; bIndex<MUSB_SERIAL_DEVICE_NUM; bIndex++)
    {    
        if (MGC_arPL2303[bIndex].pSerialDevice == pSerialDevice)
        {
			// call back to upper layer when device already insert.
			if ((pSerialDevice->pUsbDevice) && 				 
				(pSerialDevice->pt_socket_nfy_info))
			{
				pt_hotswap_nfy_info = pSerialDevice->pt_socket_nfy_info;
				if (pt_hotswap_nfy_info->pfNfy)
				{
					pt_hotswap_nfy_info->pvDevice = (void *)pSerialDevice->pUsbDevice;
					pt_hotswap_nfy_info->u4Status = (uint32_t)HFI_STATUS_ABSENT;
					pt_hotswap_nfy_info->u4SocketNo = (uint32_t)pSerialDevice->pUsbDevice->bHubPort;
					pt_hotswap_nfy_info->u4UsbPort = pSerialDevice->pPort->bUsbPort;						
					pt_hotswap_nfy_info->pfNfy(pt_hotswap_nfy_info->pvTag);
				}
			}  


            MGC_arPL2303[bIndex].bSerialDevId = 0;        
            MGC_arPL2303[bIndex].pSerialDevice = NULL;            
        }
    }

    pSerialDevice->pSerialDriverTag = NULL;
    return;
}

void MUSB_PL2303SetInsertNotify(void *pBuf)
{
    MUSB_ASSERT(pBuf);

    MGC_pfSerialPL2303Insert = (MUSB_pfSerialPL2303InsertNfy)((uint32_t)pBuf);

	if (MGC_arPL2303[0].pSerialDevice)
	{
		MGC_pfSerialPL2303Insert(0, TRUE);
	}
	
    return;
}

uint8_t MUSB_PL2303GetInsert(uint8_t bIndex)
{
	if (MGC_arPL2303[0].pSerialDevice)
		return TRUE;
	else
		return FALSE;
}
#endif /* MUSB_SERIAL */

