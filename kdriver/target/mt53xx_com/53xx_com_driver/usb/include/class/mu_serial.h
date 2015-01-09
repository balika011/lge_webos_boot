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
 * MUSBStack-S USB-Serial  definitions.
 * $Revision: #1 $
 */

#ifndef __MUSB_SERIAL_H__
#define __MUSB_SERIAL_H__

#include "mu_dev.h"
#include "mu_tools.h"
#include "mu_serial_if.h"
#include "mu_hfi_if.h"

#define MUSB_SERIAL_SUPPORT_AUTO_INIT
#define MUSB_SERIAL_DEVICE_NUM   (1)
#define MUSB_SERIAL_CTRL_LENGTH                7
#define MUSB_SERIAL_LIST_NUM                   3


/* Ctrl + q */
#define USB_UART_MAGIC_QUERY         (UINT8)(17)
/* Ctrl + o */
#define USB_UART_MAGIC_OUTPUT_CHANGE (UINT8)(15)
#define USB_UART_MAGIC_OUT_NUMBER    (UINT8)(3)
#define USB_UART_OUTPUT_CLI     (UINT8)(3)
#define USB_UART_OUTPUT_MW      (UINT8)(2)
#define USB_UART_OUTPUT_ALL     (UINT8)(1)

/* Ctrl + i */
#define USB_UART_INPUT_CHANGE  (UINT8)(9)
#define USB_UART_INPUT_NUMBER  (UINT8)(2)
#define USB_UART_INPUT_FACTORY  (UINT8)(3)
#define USB_UART_INPUT_CLI      (UINT8)(2)
#define USB_UART_INPUT_MW       (UINT8)(1)

#define USB_UART_MIN_FACTORY_KEY    (0x20)
#define USB_UART_MAX_FACTORY_KEY    (0x80)



/** State of USB-Serial driver       */
#define MUSB_SERIAL_STATE_FREE                          ((uint8_t)0x00)
#define MUSB_SERIAL_STATE_CONNECT                  	    ((uint8_t)0x01)
#define MUSB_HID_STATE_CONFIGURED                       ((uint8_t)0x02)

#define MUSB_SERIAL_STATE_VENDOR_1                 		((uint8_t)0x05)
#define MUSB_SERIAL_STATE_VENDOR_2                 		((uint8_t)0x06)
#define MUSB_SERIAL_STATE_VENDOR_3                 		((uint8_t)0x07)
#define MUSB_SERIAL_STATE_VENDOR_4                 		((uint8_t)0x08)
#define MUSB_SERIAL_STATE_VENDOR_5                 		((uint8_t)0x09)
#define MUSB_SERIAL_STATE_VENDOR_6                 		((uint8_t)0x0a)
#define MUSB_SERIAL_STATE_VENDOR_7                 		((uint8_t)0x0b)
#define MUSB_SERIAL_STATE_VENDOR_8                 		((uint8_t)0x0c)
#define MUSB_SERIAL_STATE_VENDOR_9                 		((uint8_t)0x0d)
#define MUSB_SERIAL_STATE_VENDOR_10                 	((uint8_t)0x0e)
#define MUSB_SERIAL_STATE_VENDOR_11                 	((uint8_t)0x0f)
#define MUSB_SERIAL_STATE_SET_TERMIOS_1                 ((uint8_t)0x10)
#define MUSB_SERIAL_STATE_SET_TERMIOS_2                 ((uint8_t)0x11)
#define MUSB_SERIAL_STATE_SET_TERMIOS_3                 ((uint8_t)0x12)
#define MUSB_SERIAL_STATE_CONFIG_DONE					((uint8_t)0x13)
#define MUSB_SERIAL_STATE_START_IN                      ((uint8_t)0x14)


#define SET_LINE_REQUEST_TYPE		0x21
#define SET_LINE_REQUEST		0x20

#define SET_CONTROL_REQUEST_TYPE	0x21
#define SET_CONTROL_REQUEST		0x22
#define CONTROL_DTR			0x01
#define CONTROL_RTS			0x02

#define BREAK_REQUEST_TYPE		0x21
#define BREAK_REQUEST			0x23
#define BREAK_ON			0xffff
#define BREAK_OFF			0x0000

#define GET_LINE_REQUEST_TYPE		0xa1
#define GET_LINE_REQUEST		0x21

#define VENDOR_WRITE_REQUEST_TYPE	0x40
#define VENDOR_WRITE_REQUEST		0x01

#define VENDOR_READ_REQUEST_TYPE	0xc0
#define VENDOR_READ_REQUEST		0x01

#define UART_STATE			0x08
#define UART_STATE_TRANSIENT_MASK	0x74
#define UART_DCD			0x01
#define UART_DSR			0x02
#define UART_BREAK_ERROR		0x04
#define UART_RING			0x08
#define UART_FRAME_ERROR		0x10
#define UART_PARITY_ERROR		0x20
#define UART_OVERRUN_ERROR		0x40
#define UART_CTS			0x80

/* Macro to prepare setup packet for USB-Serial  Class driver*/
#define MGC_PREPARE_SETUP_PACKET(pSetup,\
                                     bmActualRequestType,\
                                     bActualRequest,\
                                     wActualValue,\
                                     wActualIndex,\
                                     wActualLength)\
{\
    (pSetup)->bmRequestType = (uint8_t) bmActualRequestType;\
    (pSetup)->bRequest      = (uint8_t) bActualRequest;\
    (pSetup)->wValue        = (uint16_t) MUSB_SWAP16(wActualValue);\
    (pSetup)->wIndex        = (uint16_t) MUSB_SWAP16(wActualIndex);\
    (pSetup)->wLength       = (uint16_t) MUSB_SWAP16(wActualLength);\
}

/* Macro to fill control Irp for USB-Serial  Class driver */
#define MGC_FILL_CONTROL_IRP(pTargetDevice,\
                                 pControlIrp,\
                                 pActualOutBuffer,\
                                 dwRequestedOutLength,\
                                 pActualInBuffer,\
                                 dwRequestedInLength,\
                                 pfControlIrpComplete)\
{\
    (pControlIrp)->pDevice           = pTargetDevice->pUsbDevice;\
    (pControlIrp)->pOutBuffer        = pActualOutBuffer;\
    (pControlIrp)->dwOutLength       = dwRequestedOutLength;\
    (pControlIrp)->pInBuffer         = pActualInBuffer;\
    (pControlIrp)->dwInLength        = dwRequestedInLength;\
    (pControlIrp)->dwStatus          = 0;\
    (pControlIrp)->dwActualOutLength = 0;\
    (pControlIrp)->dwActualInLength  = 0;\
    (pControlIrp)->pfIrpComplete     = pfControlIrpComplete;\
    (pControlIrp)->pCompleteParam    = (void *) pTargetDevice;\
}

typedef struct _MGC_SerialDevice
{
#ifndef MUSB_NONEOS   
    HANDLE_T hTransferDone;
    HANDLE_T hPutsTransfer;
    HANDLE_T hGetsTransfer;
#endif
    /* USB-Serial  device basic information object */
    uint8_t bDeviceIndex;
    MUSB_BusHandle hBus;   /* MGC_Port* pImplPort */
    MUSB_Device *pUsbDevice;
    MUSB_Port* pPort;
    MUSB_DeviceDriver *pDriver;
    MUSB_ConfigurationDescriptor *pConfig;
    MUSB_InterfaceDescriptor *pIface;
    MUSB_EndpointDescriptor *pEnd;

    /* USB transfer related object */
    uint8_t aSetupTx[8+MUSB_SERIAL_CTRL_LENGTH];
    MUSB_ControlIrp ControlIrp;
    MUSB_Irp intrInIrp;
    MUSB_Pipe hIntInPipe;
    uint8_t aIntInBuf[8];

    MUSB_Irp BulkOutIrp;
    MUSB_Pipe hBulkOutPipe;
    uint8_t *aBulkOutBuf;
    uint16_t bBulkOutSize;

    MUSB_Irp BulkInIrp;
    MUSB_Pipe hBulkInPipe;
    uint8_t *aBulkInBuf;
    uint16_t bBulkInSize;

    uint8_t bState;
    uint8_t bNextState; 

    uint8_t bSerialDriverNum;  /*Num of driver on the same serial device.*/
    void *pSerialDriverTag;
	
	USB_RS_232_NFY_INFO_T *pt_info_nfy;
    /* Device manager plug in/out handling*/
    MUSB_HOTSWAP_NFY_INFO_T *pt_socket_nfy_info; /* IN:  DM set hot swap notify function. */     
	
} MGC_SerialDevice;

typedef struct __MUSB_READ_BUFFER_INFO
{
	uint32_t u4Read;
	uint32_t u4Write;
	uint32_t u4length;
	uint8_t CharBuffer[MUSB_SERIAL_FIFO_SZ];
}MUSB_READ_BUFFER_INFO;

/**
 * Fill an array with the targetted peripheral list entry appropriate
 * for the USB-Serial  class driver, ending with the MUSB_TARGET_ACCEPT.
 * @param pList array
 * @param wListLength how many bytes are available in the array
 * @return how many bytes were filled in the array.
 * If this equals bListLength, the caller should assume there is insufficient space
 * in the array and the list entry is incomplete.
 */
extern uint16_t MUSB_FillSerialClassPeripheralList(uint8_t* pList, uint16_t wListLength, uint8_t bIndex);

/**
 * Get a pointer to the USB-Serial class driver
 */
extern MUSB_DeviceDriver* MUSB_GetSerialClassDriver(void);


/**
 * PL2303 internal function.
 */
extern void MUSB_PL2303Init(void); 
extern void MUSB_PL2303Register(MGC_SerialDevice *pSerialDevice);
extern void MUSB_PL2303Release(MGC_SerialDevice *pSerialDevice);


/**
 * PL2303 external function.
 */
extern void MUSB_PL2303SetInsertNotify(void *pBuf);
extern uint8_t MUSB_PL2303GetInsert(uint8_t bIndex);

extern BOOL MUSB_Serial_Cli_Support(void);	

#endif	/* multiple inclusion protection */
