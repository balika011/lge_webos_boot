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
 * Device Framework (USB 2.0 chapter 9) definitions.
 * $Revision: #1 $
 */

#ifndef __MUSB_DEVICE_H__
#define __MUSB_DEVICE_H__

/* USB 2.0 device framework definitions */

#include "mu_tools.h"

/*
* Device and/or Interface Class codes
*/
#define MUSB_CLASS_PER_INTERFACE	0	/* for DeviceClass */
#define MUSB_CLASS_AUDIO		1
#define MUSB_CLASS_COMM			2
#define MUSB_CLASS_HID			3
#define MUSB_CLASS_PHYSICAL		5
#define MUSB_CLASS_SICD		        6
#define MUSB_CLASS_PRINTER		7
#define MUSB_CLASS_MASS_STORAGE		8
#define MUSB_CLASS_HUB			9
#define MUSB_CLASS_DATA			10
#define MUSB_CLASS_IPOD         11
#define MUSB_CLASS_DIAGNOSTIC		0xDC
#define MUSB_CLASS_APP_SPEC		0xfe
#define MUSB_CLASS_VENDOR_SPEC		0xff
#define MUSB_CLASS_SERIAL       0xcc       // special for the Serial Support. change from 0xff to 0xcc
/*
The class code (bDeviceClass) is 0xE0 �V Wireless Controller.
The SubClass code (bDeviceSubClass) is 0x01 �V RF Controller.
The Protocol code (bDeviceProtocol) is 0x01 �V Bluetooth programming.
*/
#define MUSB_CLASS_WIRELESS		0xE0
#define MUSB_SUBCLASS_RF		0x01
#define MUSB_PROTOCOL_BT		0x01

/*
* USB request types
*/
#define MUSB_TYPE_MASK			(0x03 << 5)
#define MUSB_TYPE_STANDARD		(0x00 << 5)
#define MUSB_TYPE_CLASS			(0x01 << 5)
#define MUSB_TYPE_VENDOR		(0x02 << 5)
#define MUSB_TYPE_RESERVED		(0x03 << 5)

/*
* USB recipients
*/
#define MUSB_RECIP_MASK			0x1f
#define MUSB_RECIP_DEVICE		0x00
#define MUSB_RECIP_INTERFACE		0x01
#define MUSB_RECIP_ENDPOINT		0x02
#define MUSB_RECIP_OTHER		0x03

/*
* USB directions
*/
#define MUSB_DIR_OUT			0
#define MUSB_DIR_IN			0x80

/*
* Descriptor types
*/
#define MUSB_DT_DEVICE			0x01
#define MUSB_DT_CONFIG			0x02
#define MUSB_DT_STRING			0x03
#define MUSB_DT_INTERFACE		0x04
#define MUSB_DT_ENDPOINT		0x05
#define	MUSB_DT_DEVICE_QUALIFIER	0x06
#define	MUSB_DT_OTHER_SPEED		0X07
#define	MUSB_DT_INTERFACE_POWER		0x08
#define	MUSB_DT_OTG			0x09

/*
* Descriptor sizes per descriptor type
*/
#define MUSB_DT_DEVICE_SIZE		18
#define MUSB_DT_DEVICE_QUALIFIER_SIZE	10
#define MUSB_DT_CONFIG_SIZE		9
#define MUSB_DT_INTERFACE_SIZE		9
#define MUSB_DT_ENDPOINT_SIZE		7
#define MUSB_DT_HUB_NONVAR_SIZE		7
#define	MUSB_DT_OTG_SIZE		3

/*
* Endpoints
*/
#define MUSB_ENDPOINT_NUMBER_MASK	0x0f	/* in bEndpointAddress */
#define MUSB_ENDPOINT_DIR_MASK		0x80

/* Traffic types in bmAttributes */
#define MUSB_ENDPOINT_XFERTYPE_MASK	0x03
#define MUSB_ENDPOINT_XFER_CONTROL	0
#define MUSB_ENDPOINT_XFER_ISOC		1
#define MUSB_ENDPOINT_XFER_BULK		2
#define MUSB_ENDPOINT_XFER_INT		3

/* Synchronization type in bmAttributes (ISO only) */
#define MUSB_M_ENDPOINT_SYNC_TYPE	0x0c
#define MUSB_S_ENDPOINT_SYNC_TYPE	2
#define MUSB_ENDPOINT_TYPE_NONE		0
#define MUSB_ENDPOINT_TYPE_ASYNC	1
#define MUSB_ENDPOINT_TYPE_ADAPTIVE	2
#define MUSB_ENDPOINT_TYPE_SYNC		3

/* Usage type in bmAttributes (ISO only) */
#define MUSB_M_ENDPOINT_USAGE_TYPE	0x30
#define MUSB_S_ENDPOINT_USAGE_TYPE	4
#define MUSB_ENDPOINT_USAGE_DATA	0
#define MUSB_ENDPOINT_USAGE_FEEDBACK	1
#define MUSB_ENDPOINT_USAGE_IMPLICIT	2
#define MUSB_ENDPOINT_USAGE_RESERVED	3

/* wMaxPacketSize */
#define MUSB_M_ENDPOINT_MAX_PACKET_SIZE	    0x07ff
#define MUSB_M_ENDPOINT_PACKETS_PER_FRAME   0x1800  /* high-speed, high-bandwidth */
#define MUSB_S_ENDPOINT_PACKETS_PER_FRAME   11	    /* high-speed, high-bandwidth */

/*
* Standard requests
*/
#define MUSB_REQ_GET_STATUS		0x00
#define MUSB_REQ_CLEAR_FEATURE		0x01
#define MUSB_REQ_SET_FEATURE		0x03
#define MUSB_REQ_SET_ADDRESS		0x05
#define MUSB_REQ_GET_DESCRIPTOR		0x06
#define MUSB_REQ_SET_DESCRIPTOR		0x07
#define MUSB_REQ_GET_CONFIGURATION	0x08
#define MUSB_REQ_SET_CONFIGURATION	0x09
#define MUSB_REQ_GET_INTERFACE		0x0A
#define MUSB_REQ_SET_INTERFACE		0x0B
#define MUSB_REQ_SYNCH_FRAME		0x0C

/*
* OTG requests
*/
#define	MUSB_OTG_SRP			0x01	/* bit 0 of bmAttributes */
#define	MUSB_OTG_HNP			0x02	/* bit 1 of bmAttributes */

/*
* USB Set Features
*/
#define	MUSB_FEATURE_ENDPOINT_HALT		0x00
#define	MUSB_FEATURE_DEVICE_REMOTE_WAKEUP	0x01
#define	MUSB_FEATURE_TEST_MODE			0x02

/*
* OTG Set Feature parameters
*/
#define	MUSB_b_hnp_enable		0x03
#define	MUSB_a_hnp_support		0x04
#define	MUSB_a_alt_hnp_support		0x05

/*
* Test modes
*/
#define MUSB_TESTMODE_TEST_J		0x01
#define MUSB_TESTMODE_TEST_K		0x02
#define MUSB_TESTMODE_TEST_SE0_NAK	0x03
#define MUSB_TESTMODE_TEST_PACKET	0x04
#define MUSB_TESTMODE_TEST_FORCE_ENABLE	0x05

/* USB structures */

/* Standard device request (USB 2.0) */
#include "mu_pkon.h"
typedef struct
{
    uint8_t bmRequestType;
    uint8_t bRequest;
    uint16_t wValue;
    uint16_t wIndex;
    uint16_t wLength;
} MUSB_DeviceRequest;
#include "mu_pkoff.h"

/* All standard descriptors have these 2 fields in common */
#include "mu_pkon.h"
typedef	struct
{
    uint8_t bLength;
    uint8_t bDescriptorType;
} MUSB_DescriptorHeader;
#include "mu_pkoff.h"

/* Device descriptor (USB 2.0) */
#include "mu_pkon.h"
typedef struct
{
    uint8_t bLength;
    uint8_t bDescriptorType;
    uint16_t bcdUSB;
    uint8_t bDeviceClass;
    uint8_t bDeviceSubClass;
    uint8_t bDeviceProtocol;
    uint8_t bMaxPacketSize0;
    uint16_t idVendor;
    uint16_t idProduct;
    uint16_t bcdDevice;
    uint8_t iManufacturer;
    uint8_t iProduct;
    uint8_t iSerialNumber;
    uint8_t bNumConfigurations;
} MUSB_DeviceDescriptor;
#include "mu_pkoff.h"

/* Endpoint descriptor (USB 2.0) */
#include "mu_pkon.h"
typedef	struct
{
    uint8_t bLength;
    uint8_t bDescriptorType;
    uint8_t bEndpointAddress;
    uint8_t bmAttributes;
    uint16_t wMaxPacketSize;
    uint8_t bInterval;
} MUSB_EndpointDescriptor;
#include "mu_pkoff.h"

/* Interface descriptor (USB 2.0) */
#include "mu_pkon.h"
typedef	struct
{
    uint8_t bLength;
    uint8_t bDescriptorType;
    uint8_t bInterfaceNumber;
    uint8_t bAlternateSetting;
    uint8_t bNumEndpoints;
    uint8_t bInterfaceClass;
    uint8_t bInterfaceSubClass;
    uint8_t bInterfaceProtocol;
    uint8_t iInterface;
} MUSB_InterfaceDescriptor; 
#include "mu_pkoff.h"

/* Configuration descriptor (USB 2.0) */
#include "mu_pkon.h"
typedef	struct
{
    uint8_t bLength;
    uint8_t bDescriptorType;
    uint16_t wTotalLength;
    uint8_t bNumInterfaces;
    uint8_t bConfigurationValue;
    uint8_t iConfiguration;
    uint8_t bmAttributes;
    uint8_t bMaxPower;
} MUSB_ConfigurationDescriptor;
#include "mu_pkoff.h"

/* String descriptor (USB 2.0) */
#include "mu_pkon.h"
typedef	struct
{
    uint8_t bLength;
    uint8_t bDescriptorType;
    uint16_t wData[1];
} MUSB_StringDescriptor;
#include "mu_pkoff.h"

/* OTG descriptor (OTG 1.0a) */
#include "mu_pkon.h"
typedef	struct
{
    uint8_t bLength;
    uint8_t bDescriptorType;
    uint8_t bmAttributes;	/* bit 0=SRP; bit 1=HNP */
} MUSB_OtgDescriptor;
#include "mu_pkoff.h"

/* Qualifier descriptor */
#include "mu_pkon.h"
typedef struct
{
    uint8_t bLength;
    uint8_t bDescriptorType;
    uint16_t bcdUSB;
    uint8_t bDeviceClass;
    uint8_t bDeviceSubClass;
    uint8_t bDeviceProtocol;
    uint8_t bMaxPacketSize0;
    uint8_t bNumConfigurations;
    uint8_t bRESERVED;
} MUSB_QualifierDescriptor;
#include "mu_pkoff.h"


#endif	/* multiple inclusion protection */
