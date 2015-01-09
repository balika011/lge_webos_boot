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

#ifndef _MUSB_SERIAL_IF_H_
#define _MUSB_SERIAL_IF_H_

#define MUSB_SERIAL_FIFO_SZ    2048
#define MUSB_SERIAL_GET_FIFO_SZ       256

/** 
 * MUSB_HidIoctlCmdType.
 * HFI Ioctl command Type.
 */
typedef enum
{
	MUSB_SERIAL_IOCTL_GET_DEVICE_NUM,	  // total number of SERIAL devices on our system.
	MUSB_SERIAL_IOCTL_GET_DEVICE_PORT,	 // number of ports on the each SERIAL device.
	MUSB_SERIAL_IOCTL_GET_DEVICE_STATUS,

	MUSB_SERIAL_IOCTL_HOTSWAP_NFY,						// set insert notify
	MUSB_SERIAL_IOCTL_SWITCH_REC_DATA_ON_OFF,			// set get data on/off
	MUSB_SERIAL_IOCTL_SET_REC_DATA_START,               // start get data.

	MUSB_SERIAL_IOCTL_CLI_REC_DATA_START,               // cli debug start get data.

	MUSB_SERIAL_IOCTL_SET_INFO_NFY,
	MUSB_SERIAL_IOCTL_GET_REC_DATA_LEN,

	
	MUSB_SERIAL_IOCTL_END
} MUSB_SerialIoctlCmdType;


typedef void (*MUSB_pfSerialPL2303InsertNfy)(uint8_t bPL2303Id, uint8_t bInsert);

struct _MGC_SerialDevice;

typedef struct 
{
    uint8_t bIndex;
    uint8_t bSerialDevId; /* The id index in total id */
    uint8_t *rPL2303;
    struct _MGC_SerialDevice *pSerialDevice;
} MGC_SerialPL2303;


typedef enum
{
    MUSB_PL2303_TYPE_0,  /* don't know the difference between type 0 and */  
    MUSB_PL2303_TYPE_1,   /* type 1, until someone from prolific tells us... */
    MUSB_PL2303_HX       /* HX version of the pl2303 chip */
} MUSB_PL2303_TYPE;


struct _MUSB_SerialIrp;

typedef struct _MUSB_SerialIrp
{
    uint8_t* pBuffer;                
    uint32_t dwBufSize;
} MUSB_SerialIrp;


/**
 * Element Meaning
 * 
 * @param MaxUnits Number of maximum units the driver can handle.
 * @param pfIoCtl Pointer to the device IoCtl function.
 * @param pfSerialPuts Pointer to the usb-serial write data to uart function. (optional)
 * @param pfSerialBufGets Pointer to the usb-serial get data from receiver buffer function. (optional)
 * @param pfSerialBufPut Pointer to the write buffer function and start write to usb-serial if buffer data lenght = 1. (optional)
 * @param pfInitDevice Pointer to the usb-serial driver initialization function. (optional)
*/
typedef struct 
{
    uint32_t MaxUnits;
    uint32_t (*pfIoCtl) (uint32_t u4Index, uint32_t u4Cmd, void *pBuffer);
	uint32_t (*pfSerialPuts) (uint32_t u4Index, uint8_t *pBuffer, uint32_t u4Length);
	uint32_t (*pfSerialBufGets) (uint32_t u4Index, uint8_t *pBuffer, uint32_t u4Length);
	uint32_t (*pfSerialBufPut) (uint8_t u1Char);
    BOOL (*pfInitDevice) (uint32_t u4Index);
} MU_SERIAL_DEVICE_TYPE;

extern MU_SERIAL_DEVICE_TYPE *MUSB_SerialDriverGet(void);

/* Notify condition */
typedef enum
{
    USB_RS_232_COND_REC_DATA = 0,
    USB_RS_232_COND_REC_BUFFER,
    USB_RS_232_COND_REC_FRAME_ERROR,
    USB_RS_232_COND_REC_OVERFLOW,
    USB_RS_232_COND_XMT,
    USB_RS_232_COND_XMT_EMPTY,
    USB_RS_232_COND_CTRL_CHG,
    USB_RS_232_COND_HOTPLUG   // factory rs232 notify plut in/out
}   USB_RS_232_COND_T;

/* Notify function */
typedef VOID (*x_usb_rs_232_nfy_fct) (VOID*          pv_nfy_tag,
                                  USB_RS_232_COND_T  e_nfy_cond,
                                  UINT32         ui4_data);


/* Data xfer structure */
typedef struct _USB_RS_232_MULTI_DATA_INFO_T
{
    VOID*   pv_tag;
    UINT32  z_data_len;
    UINT8*  pui1_data;
}   USB_RS_232_MULTI_DATA_INFO_T;


/* Notify setting structure */
typedef struct _USB_RS_232_NFY_INFO_T
{
    VOID*  pv_tag;

    x_usb_rs_232_nfy_fct  pf_usb_rs_232_nfy;
}   USB_RS_232_NFY_INFO_T;
#endif /* _MUSB_SERILA_IF_H_ */
