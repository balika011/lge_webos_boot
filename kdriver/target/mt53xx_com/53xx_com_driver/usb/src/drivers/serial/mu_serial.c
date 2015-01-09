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
 * Top-level USB-Serial class driver module
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
#include "x_serial.h"

#include "mu_serial.h"
#include "mu_serial_pl2303.h"

//#define MUSB_SERIAL_DEBUG

#define MGC_SERIAL_GET_DATA_TIME   (10)

#define MGC_SERIAL_GAP_TIME   (20)

#define MGC_SERIAL_LIST_SIZE (7)

/**************************** GLOBALS *****************************/
static uint32_t MGC_SerialIoctl(uint32_t u4Index, uint32_t u4Cmd, void *pBuffer);
static uint32_t MGC_SerialPuts(uint32_t u4Index, uint8_t *pBuffer, uint32_t u4length);
static uint32_t MGC_SerialBufGets(uint32_t u4Index, uint8_t *pBuffer, uint32_t u4length);
static uint32_t MGC_SerialBufPut(uint8_t u1Char);
static BOOL MGC_SerialInit(uint32_t u4Index);

/** Connect handler for USB-Serial class driver */
static uint8_t MGC_SerialConnect(void *pPrivateData, MUSB_BusHandle hBus, 
    MUSB_Device *pUsbDevice, const uint8_t *pPeripheralList);

/** Disconnect Handler for USB-Serial Device Driver */
static uint8_t MGC_SerialDisconnect(void *pPrivateData, 
    MUSB_BusHandle hBus, MUSB_Device *pUsbDevice);

static uint32_t MGC_SerialCtrlConfig(MGC_SerialDevice *pSerialDevice, 
						uint8_t bmActualRequestType, uint8_t bActualRequest, uint16_t wActualValue, 
						uint16_t wActualIndex, uint16_t wActualLength, uint8_t *pBuf);

static void MGC_SerialCtrlConfigCallback(void *pContext, MUSB_ControlIrp *pControlIrp);
static uint32_t MGC_SerialBulkOut(MGC_SerialDevice *pSerialDevice);
static uint32_t MGC_SerialBulkIn(MGC_SerialDevice *pSerialDevice);

static const uint8_t MGC_aSerialPeripheralList[MUSB_SERIAL_LIST_NUM][MGC_SERIAL_LIST_SIZE] =
{
    {(uint8_t)MUSB_TARGET_VID, 0xab, 0x22, MUSB_TARGET_PID, 0x23, 0x03, MUSB_TARGET_ACCEPT},
 	{(uint8_t)MUSB_TARGET_VID, 0x7b, 0x06, MUSB_TARGET_PID, 0x03, 0x23, MUSB_TARGET_ACCEPT},
 	{(uint8_t)MUSB_TARGET_VID, 0x1b, 0x06, MUSB_TARGET_PID, 0x13, 0x23, MUSB_TARGET_ACCEPT}
};

/** MUSB Serial device driver */
static MUSB_DeviceDriver MGC_SerialDeviceDriver =
{
    NULL, MGC_SerialConnect, MGC_SerialDisconnect, NULL, NULL,
};

static MGC_SerialDevice MGC_arSerialDevice[MUSB_SERIAL_DEVICE_NUM];
static uint8_t bMu_serial_type = MUSB_PL2303_HX;	
static uint8_t MGC_bSerialGetData = TRUE;    // usb-serial send bulk-in cmd continously
static uint8_t MGC_bSerialInit = FALSE;
static MUSB_READ_BUFFER_INFO _arMusbRxFIFO[MUSB_SERIAL_DEVICE_NUM];
static uint8_t _arMusbRxBuf[MUSB_SERIAL_DEVICE_NUM][MUSB_SERIAL_FIFO_SZ];
static USB_RS_232_NFY_INFO_T _arMusbSerialNfyInfo[MUSB_SERIAL_DEVICE_NUM];
static uint8_t bBulkOutretry = 0;

typedef struct
{
    uint32_t WriteIndex;     /* @field Current have been saved in buffer data index that need send to usb-serial*/
	uint32_t ReadIndex;      /* @fiels Current had been send to usb-serial index. */
    uint32_t MaxSize;      	 /* @field Length of buffer */
    uint8_t *Buffer; 		 /* @field Start of buffer */
} USB_UART_CLI_QUEUE;

static USB_UART_CLI_QUEUE rUSbUartCliQueue;

static uint8_t *UsbSerialCLIBuffer;
static BOOL _fgUsbUartCliInit = FALSE;
static BOOL  _fgUsbUartCliSupport = FALSE;
static UINT8 u1USBLogMode = 0;

extern PF_DBG_INPUT_T _pfMwCliDbgInput;

extern void MGC_CheckMagicChar(uint8_t ucChar);
extern int32_t MUSB_Uart_InputSwitch(void);
extern void MUSB_Uart_OutputSwitch(void);
extern void MUSB_Uart_InOutQuery(void);
extern int32_t MUSB_Uart_Get_Input_Mode(void);
extern void MUSB_Uart_Set_Input_Mode(int32_t u4Value);
extern uint32_t MUSB_Uart_Get_FactoryFlag(void);
extern uint32_t MUSB_Uart_Set_FactoryFlag(uint32_t u4Value);
extern uint32_t MUSB_Uart_Get_MagicInputForceFactory(uint8_t bMaxfg);
extern void MUSB_Uart_Set_MagicInputForceFactory(uint8_t bMaxfg, uint32_t u4Value);
extern uint32_t MGC_SerialStartRecData(uint32_t u4Index, MUSB_SerialIrp *prSerialIrp);
extern INT32 MUSB_GetLun(void);
extern void SendCharToUART(UINT8 u1Port);

/* Provide this driver to middleware. */
static const MU_SERIAL_DEVICE_TYPE MGC_SerialHandleDriver =
{
    MUSB_SERIAL_DEVICE_NUM,  /* number of units */
    MGC_SerialIoctl,               /* IO control interface */
    MGC_SerialPuts,
    MGC_SerialBufGets,
    MGC_SerialBufPut,
    MGC_SerialInit
};


/*************************** FUNCTIONS ****************************/

/**
 * Allocate per-device data
 */

uint16_t MUSB_FillSerialClassPeripheralList(uint8_t *pList, uint16_t wListLength, uint8_t bIndex)
{
    uint16_t wResult = MUSB_MIN(MGC_SERIAL_LIST_SIZE, wListLength);

	MUSB_ASSERT(bIndex < MUSB_SERIAL_LIST_NUM);
	
    (void)MUSB_MemCopy(pList, &MGC_aSerialPeripheralList[bIndex][0], wResult);
	return wResult;	
}

MUSB_DeviceDriver *MUSB_GetSerialClassDriver()
{
    return &MGC_SerialDeviceDriver;
}

MU_SERIAL_DEVICE_TYPE *MUSB_SerialDriverGet(void)
{
    return (MU_SERIAL_DEVICE_TYPE *)&MGC_SerialHandleDriver;
}

void MGC_Serial_SetNotifyFuc(uint8_t u1Port, const USB_RS_232_NFY_INFO_T *prUsbRs232NfyInfo)
{
    ASSERT(prUsbRs232NfyInfo);

    _arMusbSerialNfyInfo[u1Port].pv_tag = prUsbRs232NfyInfo->pv_tag;
    _arMusbSerialNfyInfo[u1Port].pf_usb_rs_232_nfy = prUsbRs232NfyInfo->pf_usb_rs_232_nfy;
}


void MGC_Serial_SendRcvDataEvent(MGC_SerialDevice *pSerialDevice, uint8_t u1Port, uint32_t u4DataSZ)
{
	uint32_t i, j;
	#ifdef MUSB_SERIAL_DEBUG
	uint8_t bReceData[64] = {};
	#endif
    uint8_t ucCh;
	
	if(u4DataSZ == 0)
	{
		// factory mode, receive the whole data at the one time.
		if(MUSB_Uart_Get_Input_Mode() == USB_UART_INPUT_FACTORY)
		{
			if(_arMusbRxFIFO[u1Port].u4Read != _arMusbRxFIFO[u1Port].u4Write)
			{
				if(_arMusbRxFIFO[u1Port].u4Read > _arMusbRxFIFO[u1Port].u4Write)			
					u4DataSZ = _arMusbRxFIFO[u1Port].u4Read - _arMusbRxFIFO[u1Port].u4Write;
				else
					u4DataSZ = _arMusbRxFIFO[u1Port].u4Write +  MUSB_SERIAL_FIFO_SZ - _arMusbRxFIFO[u1Port].u4Read;
			    if(_arMusbSerialNfyInfo[u1Port].pf_usb_rs_232_nfy)
				{
					_arMusbSerialNfyInfo[u1Port].pf_usb_rs_232_nfy((void *)_arMusbSerialNfyInfo[u1Port].pv_tag, 
	        									USB_RS_232_COND_REC_BUFFER,
	        									u4DataSZ);
				}
			}
		}
		return;
	}



	#ifdef MUSB_SERIAL_DEBUG
	LOG(1, "Rcv DataEvent read addr = %d, write addr = %d",_arMusbRxFIFO[u1Port].u4Read, _arMusbRxFIFO[u1Port].u4Write);
	for(i = 0; i < u4DataSZ; i ++)
	{
		bReceData[i] = *(pSerialDevice->aBulkInBuf + i);
	}
	LOG(1, "%s.\n", (char *)bReceData);
	#endif
	for(i = 0; i < u4DataSZ; i++)
	{
		MGC_CheckMagicChar(*(pSerialDevice->aBulkInBuf + i));
	}

	j = _arMusbRxFIFO[u1Port].u4Read; 

	for(i = 0; ((i < u4DataSZ) && (j + i < MUSB_SERIAL_FIFO_SZ)); i ++)
	{
		_arMusbRxFIFO[u1Port].CharBuffer[j + i]= *(pSerialDevice->aBulkInBuf + i);
	}
	_arMusbRxFIFO[u1Port].u4Read += i;

	
	if(i < u4DataSZ)
	{
		j = i;
		for(; ((i < u4DataSZ) && (i - j < MUSB_SERIAL_FIFO_SZ)); i ++)
		{
			_arMusbRxFIFO[u1Port].CharBuffer[i - j]= *(pSerialDevice->aBulkInBuf + i);
		}
		_arMusbRxFIFO[u1Port].u4Read = i - j;
	}
	
	LOG(1, "Rcv DataEvent length= %d, addr = %d", u4DataSZ, _arMusbRxFIFO[u1Port].u4Read);

	if(MUSB_Uart_Get_Input_Mode() == USB_UART_INPUT_MW)
	{
		for (i = 0; i < u4DataSZ; i++)
		{			
			ucCh = _arMusbRxFIFO[u1Port].CharBuffer[_arMusbRxFIFO[u1Port].u4Write + i];
			_arMusbRxFIFO[u1Port].u4Write += 1;
			_pfMwCliDbgInput((INT32)ucCh);
		}
	}
	else if(MUSB_Uart_Get_Input_Mode() == USB_UART_INPUT_CLI)
	{
		#ifndef MUSB_NONEOS 
        VERIFY(x_sema_unlock(MGC_arSerialDevice[0].hGetsTransfer) == OSR_OK);
		#endif
	}
	return;
}


static MGC_SerialDevice *MGC_SerialDeviceInit(void)
{
    MGC_SerialDevice *pSerialDevice;
	uint8_t bSupport = FALSE;
	uint8_t bIndex;
	
	LOG(0, "Serial MGC_SerialDeviceInit.\n", NULL);
	for(bIndex = 0; bIndex < MUSB_SERIAL_DEVICE_NUM; bIndex ++)
	{
		pSerialDevice = &MGC_arSerialDevice[bIndex];
		if (pSerialDevice->pUsbDevice == NULL)
		{
			pSerialDevice->bDeviceIndex = bIndex;
			bSupport = TRUE;
			break;
		}
	}
    if (!bSupport)
    {
        LOG(0, "Serial Err: No space avaiabl.\n", NULL);
        return (NULL);
    }

    pSerialDevice->pDriver = &MGC_SerialDeviceDriver;
    pSerialDevice->bState = MUSB_SERIAL_STATE_FREE;

	_arMusbRxFIFO[bIndex].u4Read = 0;
	_arMusbRxFIFO[bIndex].u4Write = 0;
    return pSerialDevice;
}


static void MGC_SerialReportConnect(MGC_SerialDevice *pSerialDevice, uint8_t bSupport)
{
    MGC_EnumerationData *pEnumData;
    MGC_Port* pPort;
    MUSB_Device *pUsbDevice;

    MUSB_ASSERT(pSerialDevice);
    MUSB_ASSERT((bSupport == MUSB_STATUS_CHILD_CONNECT) ||
     (bSupport == MUSB_STATUS_CHILD_CONNECT_UNSUPPORT));
    
    pPort = (MGC_Port *)pSerialDevice->hBus;
    MUSB_ASSERT(pPort);
    pUsbDevice = pSerialDevice->pUsbDevice;
    MUSB_ASSERT(pUsbDevice);    

    // Report device to user's call back function.
    pUsbDevice->bPortCount = 1;
    pUsbDevice->bSupport = bSupport;
	// special for the Serial Support. change from 0xff to 0xcc
	//pUsbDevice->bDeviceClass = MUSB_CLASS_SERIAL;
	
    pEnumData = &(pPort->EnumerationData);
    if (pEnumData->pfChildDeviceUpdate)
    {
        pEnumData->pfChildDeviceUpdate(pUsbDevice->pParentUsbDevice,
            pUsbDevice, bSupport);
    }
}


/** Disconnect Handler for Serial Device Driver */
static uint8_t MGC_SerialDisconnect(void *pPrivateData, 
    MUSB_BusHandle hBus, MUSB_Device *pUsbDevice)
{
    MGC_SerialDevice *pSerialDevice;
    MGC_Port *pPort = NULL;
    MGC_EnumerationData *pEnumData;
	uint16_t wIndex;
	
    MUSB_ASSERT(pUsbDevice);
    UNUSED(pPrivateData);

    pSerialDevice = (MGC_SerialDevice *)pUsbDevice->pDriverPrivateData;
    LOG(1, "Serial: Disconnect pUsbDevice = 0x%08X.\n", (uint32_t)pUsbDevice);

    /* Check against the USB device and bus handle */
    if ((!pSerialDevice) || 
        (!hBus) || 
        (hBus != pSerialDevice->hBus) || 
        (pUsbDevice != pSerialDevice->pUsbDevice))
    {
        return FALSE;
    }

    if (pSerialDevice->hIntInPipe)
    {
        MUSB_ClosePipe(pSerialDevice->hIntInPipe);
    }
	if(pSerialDevice->hBulkInPipe)
	{
		MUSB_ClosePipe(pSerialDevice->hBulkInPipe);
	}
	if(pSerialDevice->hBulkOutPipe)
	{
		MUSB_ClosePipe(pSerialDevice->hBulkOutPipe);
	}

	MUSB_PL2303Release(pSerialDevice);
	
    pPort = (MGC_Port *)hBus;
    MUSB_ASSERT(pPort->pController);
	
	for (wIndex=0; wIndex<MUSB_SERIAL_DEVICE_NUM; wIndex++)
    {
        if (&MGC_arSerialDevice[wIndex] == pSerialDevice)
        {
            pSerialDevice->pUsbDevice = NULL;
            break;
        }
    }
	  
    // report to middleware.
    pUsbDevice->bSupport = MUSB_STATUS_CHILD_DISCONNECT;
    pEnumData = &(pPort->EnumerationData);
    if (pEnumData && pEnumData->pfChildDeviceUpdate)
    {
        pEnumData->pfChildDeviceUpdate(pUsbDevice->pParentUsbDevice,
            pUsbDevice, pUsbDevice->bSupport);
    }

    pUsbDevice->pDriverPrivateData = NULL;
	MGC_bSerialInit = FALSE;
    rUSbUartCliQueue.WriteIndex = 0;
    rUSbUartCliQueue.ReadIndex = 0;
	LOG(0, "Musb-Serial Disconnect Successfully.\n", NULL);
   return TRUE;
} 

static uint32_t MGC_Settermios(MGC_SerialDevice *pSerialDevice, int baud, int stopbits, int parity, int databits)
{
	unsigned char buf[7];
	uint8_t i=0;
	uint32_t dwStatus = MUSB_STATUS_OK;
	
	/* For reference buf[0]:buf[3] baud rate value */
	/* NOTE: Only the values defined in baud_sup are supported !
	 *       => if unsupported values are set, the PL2303 seems to use
	 *          9600 baud (at least my PL2303X always does)
	 */
	buf[0] = baud & 0xff;
	buf[1] = (baud >> 8) & 0xff;
	buf[2] = (baud >> 16) & 0xff;
	buf[3] = (baud >> 24) & 0xff;

	/* For reference buf[4]=0 is 1 stop bits */
	/* For reference buf[4]=1 is 1.5 stop bits */
	/* For reference buf[4]=2 is 2 stop bits */
	buf[4] = stopbits;

	/* For reference buf[5]=0 is none parity */
	/* For reference buf[5]=1 is odd parity */
	/* For reference buf[5]=2 is even parity */
	/* For reference buf[5]=3 is mark parity */
	/* For reference buf[5]=4 is space parity */
        buf[5] = parity;

	/* For reference buf[6]=8 is data bits = 8 */
	buf[6] = databits;
    /** Fill user's data */
    for (i=0; i<7; i++)
    {
        pSerialDevice->aSetupTx[8 + i] = buf[i];
    }

	dwStatus = MGC_SerialCtrlConfig(pSerialDevice, SET_LINE_REQUEST_TYPE, SET_LINE_REQUEST, 0x8484, 0, 7, NULL);
	return dwStatus;
}


/** After getting connect callback,  device driver calls this function to configure device */
static uint32_t MGC_SerialCtrlConfig(MGC_SerialDevice *pSerialDevice, 
	uint8_t bmActualRequestType, uint8_t bActualRequest, uint16_t wActualValue, uint16_t wActualIndex, uint16_t wActualLength, uint8_t *pBuf)
{
    MUSB_DeviceRequest *pSetup;
    MUSB_ControlIrp *pControlIrp;
    uint32_t dwStatus = MUSB_STATUS_OK;

    MUSB_ASSERT(pSerialDevice);
    //LOG(0, "Serial MGC_SerialConfigureDevice.\n", NULL);
    pSetup = (MUSB_DeviceRequest *)pSerialDevice->aSetupTx;
    pControlIrp = &(pSerialDevice->ControlIrp);

    /** Prepare the Setup Packet for sending Set Config Request */
    MGC_PREPARE_SETUP_PACKET(pSetup, 
        bmActualRequestType,
        bActualRequest, 
        wActualValue,
        wActualIndex, 
        wActualLength);
    
    /** Fill Control Irp */
    MGC_FILL_CONTROL_IRP(pSerialDevice, 
        pControlIrp, 
        (uint8_t *)pSetup, 
        (8 + wActualLength),  /* Only setup phase data */ 
        pBuf, 
        0, 
        MGC_SerialCtrlConfigCallback);

    dwStatus = MUSB_StartControlTransfer(pSerialDevice->pUsbDevice->pPort, pControlIrp);
    if (dwStatus)
    {
        /* Log an Error and return state */
        LOG(0, "Serial Err4: Set Configuration Request failed = 0x%X.\n", dwStatus);
    }

    return (dwStatus);
} /* End MGC_MsdConfigureDevice () */
void MGC_SetUSBLogMode(UINT8 u1Value)
{
    u1USBLogMode = u1Value;
}
uint32_t MGC_CLIStart(uint32_t u4Lun)
{
    MUSB_SerialIrp rSerialInIrpTmp;

    rSerialInIrpTmp.pBuffer = (uint8_t *)(uint32_t)&_arMusbRxBuf[u4Lun][0];
    rSerialInIrpTmp.dwBufSize = 1;
    if(MGC_SerialStartRecData(u4Lun, &rSerialInIrpTmp))
    {
        return MGC_M_STATUS_ERROR;
    }
    //NOTE: Not necessary to switch to drv cli
    //switch to drv cli and write 1 byte to release uart drv cli
//    MUSB_Uart_Set_Input_Mode(USB_UART_INPUT_CLI);
    SendCharToUART(0);
    return MUSB_STATUS_OK;
}


/** Callback function when device acknowledges set config reqeust. */
static void MGC_SerialCtrlConfigCallback(void *pContext, MUSB_ControlIrp *pControlIrp)
{
    MGC_SerialDevice *pSerialDevice;
    uint16_t wIndex = 0;
    MUSB_ASSERT(pContext);
    MUSB_ASSERT(pControlIrp);

    pSerialDevice = (MGC_SerialDevice *)pContext;

    if ((pSerialDevice->bState >=MUSB_SERIAL_STATE_VENDOR_1) && (MUSB_STATUS_OK != pControlIrp->dwStatus))
    {
        //LOG(0, "Serial Err5: Set Config Callback Status = 0x%X.\n", pControlIrp->dwStatus);
        return;
    }

    pSerialDevice->bState = pSerialDevice->bNextState;
    pSerialDevice->bNextState++;
    LOG(1, "Serial: Set Config Callback Status = 0x%X.\n", pSerialDevice->bState);

    switch(pSerialDevice->bState)
    {
        case MUSB_SERIAL_STATE_VENDOR_1:
            MGC_SerialCtrlConfig(pSerialDevice, VENDOR_READ_REQUEST_TYPE, VENDOR_READ_REQUEST, 0x8484, 0, 1, NULL);
            break;
        case MUSB_SERIAL_STATE_VENDOR_2:
            MGC_SerialCtrlConfig(pSerialDevice, VENDOR_WRITE_REQUEST_TYPE, VENDOR_WRITE_REQUEST, 0x0404, 0, 0, NULL);			
            break;
        case MUSB_SERIAL_STATE_VENDOR_3:
            MGC_SerialCtrlConfig(pSerialDevice, VENDOR_READ_REQUEST_TYPE, VENDOR_READ_REQUEST, 0x8484, 0, 1, NULL);
            break;
        case MUSB_SERIAL_STATE_VENDOR_4:
            MGC_SerialCtrlConfig(pSerialDevice, VENDOR_READ_REQUEST_TYPE, VENDOR_READ_REQUEST, 0x8383, 0, 1, NULL);
            break;
        case MUSB_SERIAL_STATE_VENDOR_5:
            MGC_SerialCtrlConfig(pSerialDevice, VENDOR_READ_REQUEST_TYPE, VENDOR_READ_REQUEST, 0x8484, 0, 1, NULL);
            break;
        case MUSB_SERIAL_STATE_VENDOR_6:			
            MGC_SerialCtrlConfig(pSerialDevice, VENDOR_WRITE_REQUEST_TYPE, VENDOR_WRITE_REQUEST, 0x0404, 1, 0, NULL);
            break;
        case MUSB_SERIAL_STATE_VENDOR_7:				
            MGC_SerialCtrlConfig(pSerialDevice, VENDOR_READ_REQUEST_TYPE, VENDOR_READ_REQUEST, 0x8484, 0, 1, NULL);
            break;
        case MUSB_SERIAL_STATE_VENDOR_8:					
            MGC_SerialCtrlConfig(pSerialDevice, VENDOR_READ_REQUEST_TYPE, VENDOR_READ_REQUEST, 0x8383, 0, 1, NULL);
            break;
        case MUSB_SERIAL_STATE_VENDOR_9:
            MGC_SerialCtrlConfig(pSerialDevice, VENDOR_WRITE_REQUEST_TYPE, VENDOR_WRITE_REQUEST, 0, 1, 0, NULL);						
            break;
        case MUSB_SERIAL_STATE_VENDOR_10:
            MGC_SerialCtrlConfig(pSerialDevice, VENDOR_WRITE_REQUEST_TYPE, VENDOR_WRITE_REQUEST, 1, 0, 0, NULL);						
            break;
        case MUSB_SERIAL_STATE_VENDOR_11:
            if(bMu_serial_type == MUSB_PL2303_HX)
                wIndex = 0x44;
            else
                wIndex = 0x24;
            MGC_SerialCtrlConfig(pSerialDevice, VENDOR_WRITE_REQUEST_TYPE, VENDOR_WRITE_REQUEST, 2, wIndex, 0, NULL);
            break;
        case MUSB_SERIAL_STATE_SET_TERMIOS_1:
            MGC_Settermios(pSerialDevice, 115200, 0, 0, 8);
            break;
        case MUSB_SERIAL_STATE_SET_TERMIOS_2:
            MGC_SerialCtrlConfig(pSerialDevice, SET_CONTROL_REQUEST_TYPE, SET_CONTROL_REQUEST, (CONTROL_DTR | CONTROL_RTS), 0, 0, NULL);
            break;
        case MUSB_SERIAL_STATE_SET_TERMIOS_3:
            MGC_SerialCtrlConfig(pSerialDevice, VENDOR_WRITE_REQUEST_TYPE, VENDOR_WRITE_REQUEST, 0x0505, 0, 0, NULL);			
            break;
        case MUSB_SERIAL_STATE_CONFIG_DONE:
            // report to upper layer device connect.
            MGC_SerialReportConnect(pSerialDevice, MUSB_STATUS_CHILD_CONNECT);
                    
            MUSB_PL2303Init();
            MUSB_PL2303Register(pSerialDevice);
            
            //set usb-tty cli init.
            if (u1USBLogMode)
            {
                MGC_SerialInit(0);
                MGC_CLIStart(0);
            }
            LOG(0, "Serial: Set Config Done.\n", NULL);
            break;
        default:
            LOG(0, "Serial: break\n", NULL);
            break;
    }
}

static uint32_t MGC_SerialSetPL2303Type(MGC_SerialDevice *pSerialDevice)
{
    MUSB_ControlIrp *pControlIrp;
    uint32_t dwStatus = MUSB_STATUS_OK;

    MUSB_ASSERT(pSerialDevice);
    //LOG(0, "Serial MGC_SerialConfigureDevice.\n", NULL);
    pControlIrp = &(pSerialDevice->ControlIrp);
	if(pSerialDevice->pUsbDevice->DeviceDescriptor.bDeviceClass == 0x02)
		bMu_serial_type = MUSB_PL2303_TYPE_0;
	else if(pSerialDevice->pUsbDevice->DeviceDescriptor.bMaxPacketSize0 == 0x40)
		bMu_serial_type = MUSB_PL2303_HX;
	else if(pSerialDevice->pUsbDevice->DeviceDescriptor.bDeviceClass == 0x00)
		bMu_serial_type = MUSB_PL2303_TYPE_1;
	else if(pSerialDevice->pUsbDevice->DeviceDescriptor.bDeviceClass== 0xff)
		bMu_serial_type = MUSB_PL2303_TYPE_1;
	
	MGC_SerialCtrlConfigCallback(pSerialDevice, pControlIrp);
    

    return (dwStatus);
} /* End MGC_MsdConfigureDevice () */

static void MGC_SerialTimerExpired(void *pControllerPrivateData, uint16_t wTimerIndex)
{
    MGC_Port *pPort;
    MUSB_Device *pDevice = NULL;
    uint16_t wCount = 0;
    uint16_t wIndex = 0;
    MGC_SerialDevice *pSerialDevice;
    
    MUSB_ASSERT(pControllerPrivateData);
    MUSB_ASSERT(wTimerIndex == MUSB_SERIAL_TIMER_IDX);

    pPort = (MGC_Port *)pControllerPrivateData;
    MUSB_ASSERT(pPort->pController);

    wCount = MUSB_ListLength(&(pPort->ConnectedDeviceList));

    for (wIndex=0; wIndex < wCount; wIndex++)
    {
        pDevice = (MUSB_Device *)MUSB_ListFindItem(&(pPort->ConnectedDeviceList), wIndex);

        if ((pDevice->bSupport == MUSB_STATUS_CHILD_UNKNOW) &&        
            (pDevice->bDeviceClass == MUSB_CLASS_VENDOR_SPEC) &&
            (pDevice->pDriverPrivateData))
        {
            // temp setting.
            pDevice->bSupport = MUSB_STATUS_CHILD_CONNECT;
        
            pSerialDevice = (MGC_SerialDevice *)pDevice->pDriverPrivateData;    

            if (MUSB_STATUS_OK != MGC_SerialSetPL2303Type(pSerialDevice))
            {
                LOG(1, "Hid Err: MGC_HidSetReportIdle Error.\n", NULL);
                return;
            }
        }
    }
}



/** Callback function when device acknowledges set config reqeust. */
static void MGC_SerialSetConfigCallback(void *pContext, MUSB_ControlIrp *pControlIrp)
{
    MGC_SerialDevice *pSerialDevice;
    MGC_Port *pPort;
    MUSB_SystemServices *pServices;

    MUSB_ASSERT(pContext);
    MUSB_ASSERT(pControlIrp);
   
    if (MUSB_STATUS_OK != pControlIrp->dwStatus)
    {
        LOG(0, "Serial Err: Set Config Callback Status = 0x%X.\n", pControlIrp->dwStatus);
        return;
    }

    pSerialDevice = (MGC_SerialDevice *)pContext;

    /* Since IRP is executed properly hence update hub state to its next state.
     */
    pSerialDevice->bState = pSerialDevice->bNextState;
    
    /* Set the Current Configuration Descriptor to Default as Set Config is Success */
    pSerialDevice->pUsbDevice->pCurrentConfiguration = 
        pSerialDevice->pUsbDevice->apConfigDescriptors[0];

    /* Device is connected */
    LOG(0, "Serial ubPort-%d, UsbDevice = 0x%08X Config OK.\n", 
        pSerialDevice->pUsbDevice->bHubPort, (uint32_t)pSerialDevice->pUsbDevice);

    MUSB_ASSERT(pSerialDevice->hBus);
    pPort = (MGC_Port *)pSerialDevice->hBus;
    MUSB_ASSERT(pPort->pController);
    pServices = pPort->pController->pSystemServices;
    MUSB_ASSERT(pServices);    

    //Timer Index MUSB_HID_TIMER_IDX is reserved for HID class driver.
    /* kill timer */
    (void)pServices->pfCancelTimer(pServices->pPrivateData, 
        MUSB_SERIAL_TIMER_IDX/*bTimerIndex*/);
    
    (void)pServices->pfArmTimer(pServices->pPrivateData, 
        MUSB_SERIAL_TIMER_IDX/*bTimerIndex*/, 
        MGC_SERIAL_GAP_TIME/*dwTime*/, FALSE, MGC_SerialTimerExpired, pPort);    
}


/** After getting connect callback,  device driver calls this function to configure device */
static uint32_t MGC_SerialConfigureDevice(MGC_SerialDevice *pSerialDevice)
{
	MUSB_DeviceRequest *pSetup;
	MUSB_ControlIrp *pControlIrp;
	uint32_t dwStatus;

	MUSB_ASSERT(pSerialDevice);
	LOG(0, "SERIAL MGC_HidConfigureDevice.\n", NULL);	  
	pSetup = (MUSB_DeviceRequest *)pSerialDevice->aSetupTx;
	pControlIrp = &(pSerialDevice->ControlIrp);
	pSerialDevice->bNextState = MUSB_SERIAL_STATE_VENDOR_1;

	/** Prepare the Setup Packet for sending Set Config Request */
	MGC_PREPARE_SETUP_PACKET(pSetup, 
		(MUSB_DIR_OUT | MUSB_TYPE_STANDARD | MUSB_RECIP_DEVICE),
		MUSB_REQ_SET_CONFIGURATION, 
		pSerialDevice->pConfig->bConfigurationValue,
		0, 
		0);
	
	/** Fill Control Irp */
	MGC_FILL_CONTROL_IRP(pSerialDevice, 
		pControlIrp, 
		(uint8_t *)pSetup, 
		8,	/* Only setup phase data */ 
		NULL, 
		0, 
		MGC_SerialSetConfigCallback);

	dwStatus = MUSB_StartControlTransfer(pSerialDevice->pUsbDevice->pPort, pControlIrp);
	if (dwStatus)
	{
		/* Log an Error and return state */
		LOG(0, "Serial Err: Set Configuration Request failed = 0x%X.\n", dwStatus);
	}

	return (dwStatus);
}

/** This function is called when Serial device is connected.*/
static uint8_t MGC_SerialConnect(void *pPrivateData, MUSB_BusHandle hBus, 
    MUSB_Device *pUsbDevice, const uint8_t *pPeripheralList)
{
    MGC_SerialDevice *pSerialDevice;
    uint8_t bIndex;
    MUSB_InterfaceDescriptor *pIface = NULL;
    MUSB_ConfigurationDescriptor *pConfig;
    MUSB_DeviceEndpoint rEnd;
    MUSB_EndpointDescriptor *pEnd = NULL;
    MUSB_Pipe intrInPipe = NULL;
	MUSB_Pipe bulkOutPipe = NULL;
	MUSB_Pipe bulkInPipe = NULL;	
    
    MUSB_ASSERT(pUsbDevice);   
    UNUSED(pPrivateData);
    UNUSED(pPeripheralList);

    /* Device is connected */
    LOG(0, "Serial Device Connected.\n", NULL);
    pConfig = (MUSB_ConfigurationDescriptor *)pUsbDevice->apConfigDescriptors[0];
    if (!pConfig)
    {
        return FALSE;
    }   

	
    /* find first interface with supported subclass/protocol combination */
    for (bIndex = 0; bIndex < pConfig->bNumInterfaces; bIndex++)
    {
        pIface = (MUSB_InterfaceDescriptor *)
            MUSB_FindInterfaceDescriptor(pConfig, bIndex, 0);
	}
	
    if (!pIface)
    {
        return FALSE;
    } 

	LOG(0," Serial EndpointNmuber = %d.\n", pIface->bNumEndpoints);
	
    for (bIndex = 0; bIndex < pIface->bNumEndpoints; bIndex++)
    {
        pEnd = (MUSB_EndpointDescriptor *)MUSB_FindEndpointDescriptor(pConfig, pIface, bIndex);
		//LOG(0, "Serial Endpoint[%d][Proto=%d][Dir=0x%x].\n",bIndex,pEnd->bmAttributes & MUSB_ENDPOINT_XFERTYPE_MASK, pEnd->bEndpointAddress);
		//LOG(0,"Serial legnth = 0%d, type = 0x%x, Address = 0x%x, Attributes = 0x%x, packetsize=0x%x, interval=0x%x.\n", 
			//pEnd->bLength, pEnd->bDescriptorType, pEnd->bEndpointAddress, pEnd->bmAttributes, pEnd->wMaxPacketSize, pEnd->bInterval);
		switch(pEnd->bmAttributes & MUSB_ENDPOINT_XFERTYPE_MASK)
		{
			case MUSB_ENDPOINT_XFER_INT:
                break;
				
			case MUSB_ENDPOINT_XFER_BULK:
				if(MUSB_DIR_IN == (pEnd->bEndpointAddress & MUSB_ENDPOINT_DIR_MASK))
				{
	                MUSB_MemCopy((void *)(&(rEnd.UsbDescriptor)), (void *)pEnd, 0x07);

	                rEnd.pDevice = pUsbDevice;
	                bulkInPipe = MUSB_OpenPipe(hBus, &rEnd, NULL);
				}
				else
				{
	                MUSB_MemCopy((void *)(&(rEnd.UsbDescriptor)), (void *)pEnd, 0x07);

	                rEnd.pDevice = pUsbDevice;
	                bulkOutPipe = MUSB_OpenPipe(hBus, &rEnd, NULL);
				}
				break;
        }
    }
	
    if ((NULL == bulkOutPipe) || (NULL == bulkInPipe))
    {
    	LOG(0, "Serial: Open Pipe fail %d, %d, %d.", (NULL == intrInPipe), (NULL == bulkOutPipe),(NULL == bulkInPipe));
        MUSB_DIAG_STRING(1, "Hid: Open Interrupt Pipe fail.");
        return FALSE;
    }

    pSerialDevice = MGC_SerialDeviceInit();
    if (!pSerialDevice)
    {
        return FALSE;
    }    
	
    pConfig = (MUSB_ConfigurationDescriptor *)pUsbDevice->apConfigDescriptors[0];

	//LOG(0, "Serial End Num = %d.\n", pConfig->bNumInterfaces);
	
    if (!pConfig)
    {
        return FALSE;
    }   
    
    /* Initialize */
    pUsbDevice->pDriverPrivateData = pSerialDevice;
    pSerialDevice->pConfig = pConfig;
    pSerialDevice->pIface = pIface;  
    pSerialDevice->pEnd = pEnd;
    pSerialDevice->pUsbDevice = pUsbDevice;
	pSerialDevice->pPort = pUsbDevice->pPort;
    pSerialDevice->hBus = hBus;
    pSerialDevice->bState = MUSB_SERIAL_STATE_CONNECT;
    pSerialDevice->bNextState = MUSB_HID_STATE_CONFIGURED;
    pSerialDevice->hIntInPipe = intrInPipe;
	pSerialDevice->hBulkOutPipe = bulkOutPipe;
	pSerialDevice->hBulkInPipe = bulkInPipe;

    /* Configure the device */
    if (MUSB_STATUS_OK != MGC_SerialConfigureDevice(pSerialDevice))
    {
        return FALSE;
    }
	
    return TRUE;
} 

static void MGC_SerialGetDataExpired(void *pGetPrivateData, uint16_t wTimerIndex)
{
    MGC_Port *pPort;
    MUSB_Device *pDevice = NULL;
    uint16_t wCount = 0;
    uint16_t wIndex = 0;
    MGC_SerialDevice *pSerialDevice;
	uint32_t dwStatus = 0;
	
    MUSB_ASSERT(pGetPrivateData);
    MUSB_ASSERT(wTimerIndex == MUSB_SERIAL_TIMER_IDX);

    pPort = (MGC_Port *)pGetPrivateData;
    MUSB_ASSERT(pPort->pController);

    wCount = MUSB_ListLength(&(pPort->ConnectedDeviceList));
    for (wIndex=0; wIndex < wCount; wIndex++)
    {
        pDevice = (MUSB_Device *)MUSB_ListFindItem(&(pPort->ConnectedDeviceList), wIndex);

        if ((pDevice->bSupport == MUSB_STATUS_CHILD_CONNECT) &&        
            (pDevice->bDeviceClass == MUSB_CLASS_VENDOR_SPEC) &&
            (pDevice->pDriverPrivateData))
        {        
            pSerialDevice = (MGC_SerialDevice *)pDevice->pDriverPrivateData;   
			dwStatus = MGC_SerialBulkIn(pSerialDevice);
			if (MUSB_STATUS_OK != dwStatus)
			{
				return;
			}
        }
    }	
}


static void MGC_SerialBulkInCallback(void *pContext, const MUSB_Irp *pBulkIrp)
{
    MGC_SerialDevice *pSerialDevice;
    MGC_Port *pPort;
    pSerialDevice = (MGC_SerialDevice *)pContext;
    MUSB_SystemServices *pServices;

    MUSB_ASSERT(pContext);
    MUSB_ASSERT(pBulkIrp);

    pSerialDevice = (MGC_SerialDevice *)pContext;

    MUSB_ASSERT(pSerialDevice);
    pPort = (MGC_Port *)pSerialDevice->hBus;
    MUSB_ASSERT(pPort->pController);
    pServices = pPort->pController->pSystemServices;
    MUSB_ASSERT(pServices);    
	
    MGC_Serial_SendRcvDataEvent(pSerialDevice, 0, pSerialDevice->BulkInIrp.dwActualLength);

    if(MGC_bSerialGetData)
    {
        (void)pServices->pfCancelTimer(pServices->pPrivateData, MUSB_SERIAL_TIMER_IDX);

        pSerialDevice->bBulkInSize = pSerialDevice->pEnd->wMaxPacketSize;
	    (void)pServices->pfArmTimer(pServices->pPrivateData, MUSB_SERIAL_TIMER_IDX, 
	        MGC_SERIAL_GET_DATA_TIME, FALSE, MGC_SerialGetDataExpired, pPort);  
    }
    else
    {		
        (void)pServices->pfCancelTimer(pServices->pPrivateData, MUSB_SERIAL_TIMER_IDX);
    }
    return;
}        

static void MGC_SerialBulkOutCallback(void *pContext, const MUSB_Irp *pBulkIrp)
{
    MGC_SerialDevice *pSerialDevice;
    uint32_t dwActualLength = 0;
	uint32_t bCliListNull;
    pSerialDevice = (MGC_SerialDevice *)pContext;
    MUSB_ASSERT(pContext);
    MUSB_ASSERT(pBulkIrp);

    LOG(1,"SerialBulkOutCallback [len=0x%x]\n", pSerialDevice->BulkOutIrp.dwActualLength);
    if (pBulkIrp->dwStatus)
    {
        if((pSerialDevice->bBulkOutSize) && (bBulkOutretry < 3)) 
        {
            bBulkOutretry ++;
            MGC_SerialBulkOut(pSerialDevice);
        }
        else
        {
            bBulkOutretry = 0;
        }
        //LOG(0, "Serial Err1: pBulkIrp->dwStatus=0x%X\n", pBulkIrp->dwStatus);
        return;
    }
    bBulkOutretry = 0;
    if(MUSB_Uart_Get_Input_Mode()== USB_UART_INPUT_FACTORY)
    {
        dwActualLength= pSerialDevice->BulkOutIrp.dwActualLength;
        
        pSerialDevice->aBulkOutBuf += dwActualLength;
        pSerialDevice->bBulkOutSize -= dwActualLength;
    }
    else
    {
		dwActualLength= pSerialDevice->BulkOutIrp.dwActualLength;
		rUSbUartCliQueue.ReadIndex += dwActualLength;
		if (rUSbUartCliQueue.ReadIndex >= rUSbUartCliQueue.MaxSize)
		{
			rUSbUartCliQueue.ReadIndex -= rUSbUartCliQueue.MaxSize;
		}	
		pSerialDevice->aBulkOutBuf = (uint8_t *)(rUSbUartCliQueue.Buffer + rUSbUartCliQueue.ReadIndex);
		if(rUSbUartCliQueue.WriteIndex >= rUSbUartCliQueue.ReadIndex)
			bCliListNull = rUSbUartCliQueue.WriteIndex - rUSbUartCliQueue.ReadIndex;
		else
			bCliListNull = rUSbUartCliQueue.MaxSize - rUSbUartCliQueue.ReadIndex;
		pSerialDevice->bBulkOutSize = bCliListNull; 
    }
	
	LOG(5, "Serial next bulkOutSize = 0x%x, readindex = 0x%x\n", pSerialDevice->bBulkOutSize, rUSbUartCliQueue.ReadIndex);

    if(pSerialDevice->bBulkOutSize)
        MGC_SerialBulkOut(pSerialDevice);
    else
    {	
        if(_arMusbSerialNfyInfo[0].pf_usb_rs_232_nfy)
            _arMusbSerialNfyInfo[0].pf_usb_rs_232_nfy((void *)_arMusbSerialNfyInfo[0].pv_tag, 
                USB_RS_232_COND_XMT_EMPTY,
                NULL);
    }
    return;
}        


static uint32_t MGC_SerialBulkIn(MGC_SerialDevice *pSerialDevice)
{
    MUSB_Irp *pBulkInIrp;
    uint32_t dwStatus = MUSB_STATUS_OK;

    MUSB_ASSERT(pSerialDevice);

    if (!pSerialDevice->hBulkInPipe)
    {
	    LOG(0, "Serial Err0: BulkIrp transfer status=0x%X\n", dwStatus);
        return MGC_M_STATUS_ERROR;
    }
	
	//LOG(1, "bulk in irp.\n");
    pSerialDevice->bState = MUSB_SERIAL_STATE_START_IN;
    pSerialDevice->bNextState = MUSB_SERIAL_STATE_START_IN;


    pBulkInIrp = &(pSerialDevice->BulkInIrp);

    /* Fill the Bulk IRP */
	
    pBulkInIrp->hPipe = pSerialDevice->hBulkInPipe;
    pBulkInIrp->bAllowShortTransfer = FALSE;
    pBulkInIrp->pBuffer = pSerialDevice->aBulkInBuf;
    pBulkInIrp->dwLength = pSerialDevice->pEnd->wMaxPacketSize;
    pBulkInIrp->pfIrpComplete = MGC_SerialBulkInCallback;
    pBulkInIrp->pCompleteParam = (void *)pSerialDevice;
    pBulkInIrp->bIsrCallback = TRUE;
    dwStatus = MUSB_StartTransfer(pBulkInIrp);
    if (dwStatus)
    {
        LOG(0, "Serial Err3: pBulkInIrp transfer status=0x%X\n", dwStatus);
        return dwStatus;
    }

    return dwStatus;
} /* End of function  MGC_HubFillAndSubmitIntrIrp () */

static uint32_t MGC_SerialBulkOut(MGC_SerialDevice *pSerialDevice)
{
    MUSB_Irp *pBulkOutIrp;
    uint32_t dwStatus = MUSB_STATUS_OK;

    MUSB_ASSERT(pSerialDevice);

    if (!pSerialDevice->hBulkOutPipe)
    {
	    LOG(0, "Serial Err0: BulkIrp transfer status=0x%X\n", dwStatus);
        return MGC_M_STATUS_ERROR;
    }

    pSerialDevice->bState = MUSB_SERIAL_STATE_START_IN;
    pSerialDevice->bNextState = MUSB_SERIAL_STATE_START_IN;


    pBulkOutIrp = &(pSerialDevice->BulkOutIrp);

    pBulkOutIrp->hPipe = pSerialDevice->hBulkOutPipe;
    pBulkOutIrp->bAllowShortTransfer = FALSE;
    pBulkOutIrp->pBuffer = pSerialDevice->aBulkOutBuf;
	if(pSerialDevice->bBulkOutSize > pSerialDevice->pEnd->wMaxPacketSize)
		pBulkOutIrp->dwLength = pSerialDevice->pEnd->wMaxPacketSize;
	else
		pBulkOutIrp->dwLength = pSerialDevice->bBulkOutSize;
    pBulkOutIrp->pfIrpComplete = MGC_SerialBulkOutCallback;
    pBulkOutIrp->pCompleteParam = (void *)pSerialDevice;
    pBulkOutIrp->bIsrCallback = TRUE;

    dwStatus = MUSB_StartTransfer(pBulkOutIrp);
    if (dwStatus)
    {
        LOG(1, "Serial Err3: pBulkOutIrp transfer status=0x%X\n", dwStatus);
        return dwStatus;
    }
    return dwStatus;
} /* End of function  MGC_HubFillAndSubmitIntrIrp () */

uint32_t MGC_SerialStartRecData(uint32_t u4Index, MUSB_SerialIrp *prSerialIrp)
{
    MGC_SerialDevice *pSerialDevice;
    uint32_t dwStatus;

    pSerialDevice = &MGC_arSerialDevice[u4Index];

    if ((u4Index >= MUSB_SERIAL_DEVICE_NUM) ||
         (pSerialDevice == NULL) ||
         (pSerialDevice->bState < MUSB_SERIAL_STATE_CONFIG_DONE))
    {
    	LOG(0, "MUSB Serial Start RecData fail.\n");
        return MGC_M_STATUS_ERROR;
    }

	pSerialDevice->aBulkInBuf = prSerialIrp->pBuffer;
	pSerialDevice->bBulkInSize = prSerialIrp->dwBufSize;
    dwStatus = MGC_SerialBulkIn(pSerialDevice);
   
    if (MUSB_STATUS_OK != dwStatus)
    {
        return MGC_M_STATUS_ERROR;
    }
    return MUSB_STATUS_OK;
}

static uint32_t MGC_SerialIoctl(uint32_t u4Index, uint32_t u4Cmd, void *pBuffer)
{
    MGC_SerialDevice *pSerialDevice;
    MUSB_HOTSWAP_NFY_INFO_T *pt_hotswap_nfy_info;
    USB_RS_232_NFY_INFO_T *pt_info_nfy;
    MUSB_SerialIrp rSerialInIrpTmp;
    uint32_t u4RecDataLen = 0;    

    switch (u4Cmd)
    {
        case MUSB_SERIAL_IOCTL_GET_DEVICE_NUM:
            if (!pBuffer)
            {            
                return MGC_M_STATUS_ERROR;
            }   
            break;

        case MUSB_SERIAL_IOCTL_GET_DEVICE_STATUS:
            if ((!pBuffer) || (u4Index >= MUSB_SERIAL_DEVICE_NUM))
            { 
                return MGC_M_STATUS_ERROR;
            }
            pSerialDevice = &MGC_arSerialDevice[u4Index];
            if (pSerialDevice == NULL)
            { 
                return MGC_M_STATUS_ERROR;
            }

            *((uint32_t *)pBuffer) = 
            (uint32_t)(pSerialDevice->bState >= MUSB_SERIAL_STATE_CONFIG_DONE);
            break;
		
        case MUSB_SERIAL_IOCTL_HOTSWAP_NFY:
            if (!pBuffer)
            { 
                return MGC_M_STATUS_ERROR;
            }

            // Check device is initial ok or not.
            if ((!pBuffer) || (u4Index >= MUSB_SERIAL_DEVICE_NUM))
            { 
                return MGC_M_STATUS_ERROR;
            }

            pSerialDevice = &MGC_arSerialDevice[u4Index];

            pt_hotswap_nfy_info = (MUSB_HOTSWAP_NFY_INFO_T *)pBuffer;
            pSerialDevice->pt_socket_nfy_info = pt_hotswap_nfy_info;

            // call back to upper layer when device already insert.
            if (pSerialDevice->pUsbDevice)
            {
                if (pt_hotswap_nfy_info->pfNfy)
                {
                    pt_hotswap_nfy_info->pvDevice = (void *)pSerialDevice->pUsbDevice;
                    pt_hotswap_nfy_info->u4Status = (uint32_t)HFI_STATUS_READY;
                    pt_hotswap_nfy_info->u4SocketNo = (uint32_t)pSerialDevice->pUsbDevice->bHubPort;
                    pt_hotswap_nfy_info->u4UsbPort = pSerialDevice->pPort->bUsbPort;                        
                    pt_hotswap_nfy_info->pfNfy(pt_hotswap_nfy_info->pvTag);
                    LOG(0, "Serial has been insert.\n", NULL);
                }
            }     
            break;

        case MUSB_SERIAL_IOCTL_SWITCH_REC_DATA_ON_OFF:
            if(MGC_bSerialGetData)
                MGC_bSerialGetData = FALSE;
            else
                MGC_bSerialGetData = TRUE;
            LOG(0, "[USB] Set Get Data = %d.\n", MGC_bSerialGetData);
            break;

        case MUSB_SERIAL_IOCTL_SET_INFO_NFY:
            if (!pBuffer)
            { 
                return MGC_M_STATUS_ERROR;
            }

            // Check device is initial ok or not.
            if ((!pBuffer) || (u4Index >= MUSB_SERIAL_DEVICE_NUM))
            { /* avoid writing into non existing buffers */
                return MGC_M_STATUS_ERROR;
            }
            pSerialDevice = &MGC_arSerialDevice[u4Index];

            pt_info_nfy = (USB_RS_232_NFY_INFO_T *)pBuffer;
            pSerialDevice->pt_info_nfy = pt_info_nfy;

            MGC_Serial_SetNotifyFuc(0,(USB_RS_232_NFY_INFO_T *) pt_info_nfy);
            LOG(0, "[%d]MUSB_SERIAL_IOCTL_SET_INFO_NFY.\n", u4Index);
            break;

        case MUSB_SERIAL_IOCTL_GET_REC_DATA_LEN:
            if (!pBuffer)
            { /* avoid writing into non existing buffers */
                LOG(0, "error buffer.\n");
                return MGC_M_STATUS_ERROR;
            }
            if(_arMusbRxFIFO[u4Index].u4Read >= _arMusbRxFIFO[u4Index].u4Write)			
                u4RecDataLen = _arMusbRxFIFO[u4Index].u4Read - _arMusbRxFIFO[u4Index].u4Write;
            else
                u4RecDataLen = _arMusbRxFIFO[u4Index].u4Read +  MUSB_SERIAL_FIFO_SZ - _arMusbRxFIFO[u4Index].u4Write;
            *((int32_t *)pBuffer) = (int32_t)u4RecDataLen;
            break;

        case MUSB_SERIAL_IOCTL_SET_REC_DATA_START:
            //no need to switch to factory at this moment
            //MUSB_Uart_Set_Input_Mode(USB_UART_INPUT_FACTORY);
            rSerialInIrpTmp.pBuffer = (uint8_t *)(uint32_t)&_arMusbRxBuf[u4Index][0];
            rSerialInIrpTmp.dwBufSize = 10;
            if(MGC_SerialStartRecData(u4Index, &rSerialInIrpTmp))
            {
                return MGC_M_STATUS_ERROR;
            }
            break;

        case MUSB_SERIAL_IOCTL_CLI_REC_DATA_START:
            if (MUSB_STATUS_OK!=MGC_CLIStart(u4Index))
            {
                return MGC_M_STATUS_ERROR;
            }
            break;
        default:
            break;
    }

    return MUSB_STATUS_OK; /* Return with zero if no problems have occurred. */
}

static uint32_t MGC_SerialPuts(uint32_t u4Index, uint8_t *pBuffer, uint32_t u4length)
{
    MGC_SerialDevice *pSerialDevice;
    uint32_t dwStatus = MUSB_STATUS_OK;
	
	if(u1USBLogMode && (!_fgUsbUartCliInit))
		return MUSB_STATUS_CHILD_DISCONNECT;

    pSerialDevice = &MGC_arSerialDevice[u4Index];
	
    if ((u4Index >= MUSB_SERIAL_DEVICE_NUM) ||
         (pSerialDevice == NULL) ||
         (pSerialDevice->bState < MUSB_SERIAL_STATE_CONFIG_DONE))
    {
    	LOG(1, "SerialSet fail.\n");
        return MGC_M_STATUS_ERROR;
    }
	//LOG(1, "Bulk Out length=%d.\n", u4length);

	pSerialDevice->aBulkOutBuf = pBuffer;
	pSerialDevice->bBulkOutSize = u4length;
	//rUSbUartCliQueue.Buffer = pBuffer;
	//rUSbUartCliQueue.WriteIndex += u4length;
    dwStatus = MGC_SerialBulkOut(pSerialDevice);
    if (MUSB_STATUS_OK != dwStatus)
    {
        return MGC_M_STATUS_ERROR;
    }
    return MUSB_STATUS_OK;
}

static uint32_t MGC_SerialBufGets(uint32_t u4Index, uint8_t *pBuffer, uint32_t u4length)
{
	uint32_t i = 0,j,k;
	uint32_t u4DataReal = 0;
	
	j = _arMusbRxFIFO[u4Index].u4Write;
	k = _arMusbRxFIFO[u4Index].u4Read;
	
	LOG(1, "getbufferdatau4 length=%d write=%d, read=%d.\n",u4length, j ,k);
	if(j > k)
	{
		for(i = 0; i < u4length && j < MUSB_SERIAL_FIFO_SZ; i ++)
		{
			(uint8_t)pBuffer[i] = _arMusbRxFIFO[u4Index].CharBuffer[j++];
			u4DataReal ++;
		}

		j = 0;
		for(; i < u4length && j < k; i ++)
		{
			(uint8_t)pBuffer[i] = _arMusbRxFIFO[u4Index].CharBuffer[j++];
			u4DataReal ++;
		}
		_arMusbRxFIFO[u4Index].u4Write = j;
	}
	else if(j < k)
	{
		for(i = 0; i < u4length && j < k; i ++)
		{
			(uint8_t)pBuffer[i] = _arMusbRxFIFO[u4Index].CharBuffer[j++];
			u4DataReal ++;
		}
		_arMusbRxFIFO[u4Index].u4Write = j;
	}
	else
	{
		#ifndef MUSB_NONEOS 
		VERIFY(x_sema_lock(MGC_arSerialDevice[0].hGetsTransfer, X_SEMA_OPTION_WAIT) == OSR_OK);
		#endif
	}
	LOG(1, "getbufferdata done u4DataReal=%d write=%d, read=%d.\n",u4DataReal, j ,k);
	#ifdef MUSB_SERIAL_DEBUG
	pBuffer[i] = 0;
	LOG(1, "%s\n", (char *)pBuffer);
	#endif
	return u4DataReal;
}

void MGC_UartCliInit(void)
{
	if(_fgUsbUartCliInit)
		return;
	// Allocate Memory

	UsbSerialCLIBuffer = (uint8_t*)MUSB_MemAlloc(MUSB_SERIAL_FIFO_SZ);
	ASSERT(UsbSerialCLIBuffer != NULL);
	
    rUSbUartCliQueue.WriteIndex = 0;
    rUSbUartCliQueue.ReadIndex = 0;
    rUSbUartCliQueue.Buffer = UsbSerialCLIBuffer;
    rUSbUartCliQueue.MaxSize = MUSB_SERIAL_FIFO_SZ;
	#ifndef MUSB_NONEOS 
	VERIFY(OSR_OK == x_sema_create((HANDLE_T *)&MGC_arSerialDevice[0].hGetsTransfer, X_SEMA_TYPE_BINARY, X_SEMA_STATE_UNLOCK));
	#endif
	_fgUsbUartCliInit = TRUE;
	_fgUsbUartCliSupport = TRUE;

}

static uint32_t MGC_SerialBufPut(uint8_t u1Char)
{
	uint32_t bCliListNull;
	uint8_t *SrcBuf;
	MU_SERIAL_DEVICE_TYPE *pSerialDriver = MUSB_SerialDriverGet();;
	
	if(!_fgUsbUartCliInit)
		return 0;
	
	if (rUSbUartCliQueue.Buffer)
	{	
		rUSbUartCliQueue.Buffer[rUSbUartCliQueue.WriteIndex++] = u1Char;

		bCliListNull = rUSbUartCliQueue.WriteIndex - rUSbUartCliQueue.ReadIndex;
		if (rUSbUartCliQueue.WriteIndex == rUSbUartCliQueue.MaxSize)
		{
			rUSbUartCliQueue.WriteIndex -= rUSbUartCliQueue.MaxSize;
		}		

		if((bCliListNull == 1) && (rUSbUartCliQueue.WriteIndex != rUSbUartCliQueue.ReadIndex))
		{
			 SrcBuf = (uint8_t *)(rUSbUartCliQueue.Buffer + rUSbUartCliQueue.ReadIndex);
			if(pSerialDriver->pfSerialPuts(0, SrcBuf, bCliListNull))
			{
				LOG(1, "Serial Puts fail.\n", NULL);
			}			
		}
	}

	return 0;
}

static BOOL MGC_SerialInit(uint32_t u4Index)
{
	MUSB_SerialIrp rSerialInIrpTmp;

    if (u1USBLogMode)
    {
        MGC_UartCliInit();
    }
    if (!MGC_bSerialInit)
    {
        MGC_bSerialInit = TRUE;
		#ifdef MUSB_SERIAL_SUPPORT_AUTO_INIT
		//MUSB_Uart_Set_Input_Mode(USB_UART_INPUT_MW);
		rSerialInIrpTmp.pBuffer = (uint8_t *)(uint32_t)&_arMusbRxBuf[u4Index][0];
		rSerialInIrpTmp.dwBufSize = 1;
		if(MGC_SerialStartRecData(u4Index, &rSerialInIrpTmp))
		{
		    return MGC_M_STATUS_ERROR;
		}
		#endif
		MGC_UartCliInit();
    }
    else
    {
        return FALSE;
    }

    UNUSED(u4Index);    
    return TRUE;
}

BOOL MUSB_Serial_Cli_Support(void)
{
	return _fgUsbUartCliSupport;
}
//-------------------------------------------------------------------------
/** MGC_CheckMagicChar
*  The code is copy from uart/uart_drv.c  _Uart_CheckMagicChar().
*  @param  ucChar     uart rx data byte.
*  @retval   void
*/
//-------------------------------------------------------------------------

void MGC_CheckMagicChar(uint8_t ucChar)
{
    // In (1) not debug port (2) factory mode (3) normal mode, do not check ucChar = magic key !
    if (MUSB_Uart_Get_Input_Mode() == USB_UART_INPUT_FACTORY)
    {
        return;
    }
        
	if (ucChar == USB_UART_INPUT_CHANGE)
	{
		UNUSED(MUSB_Uart_InputSwitch());
		MUSB_Uart_InOutQuery();
	}
	else if (ucChar == USB_UART_MAGIC_OUTPUT_CHANGE)
	{
		MUSB_Uart_OutputSwitch();
	}
	else if (ucChar == USB_UART_MAGIC_QUERY)
	{
		MUSB_Uart_InOutQuery();
	}
	else if (((MUSB_Uart_Get_MagicInputForceFactory(TRUE) > USB_UART_MIN_FACTORY_KEY) &&
			  ((ucChar == (uint8_t)(MUSB_Uart_Get_MagicInputForceFactory(TRUE) & 0xff)) ||
			   (ucChar == (uint8_t)((MUSB_Uart_Get_MagicInputForceFactory(TRUE) + 2) & 0xff)) ||
			   (ucChar == (uint8_t)((MUSB_Uart_Get_MagicInputForceFactory(TRUE) + 4) & 0xff)))) ||
			 ((MUSB_Uart_Get_MagicInputForceFactory(FALSE) < USB_UART_MIN_FACTORY_KEY) &&
			  (ucChar == (uint8_t)(MUSB_Uart_Get_MagicInputForceFactory(FALSE) & 0xff))))			 
	{
		// switch to factory mode.
		MUSB_Uart_Set_Input_Mode(USB_UART_INPUT_FACTORY);
	}
	else if (MUSB_Uart_Get_FactoryFlag() == 1)
	{
		MUSB_Uart_Set_Input_Mode(USB_UART_INPUT_FACTORY);
		MUSB_Uart_Set_FactoryFlag(0);
	}
}
#endif

