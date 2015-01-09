/*
 * (C) Copyright 2001
 * Denis Peter, MPL AG Switzerland
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 *
 * Note: Part of this code has been derived from linux
 *
 */
#ifndef _USB_H_
#define _USB_H_

#include "usb_defs.h"
#include "part.h"

#define size_t UINT32
#define LITTLEENDIAN            1       /* used by usb.h  */


/* Everything is aribtrary */
#define USB_ALTSETTINGALLOC		4
#define USB_MAXALTSETTING		128	/* Hard limit */

#define USB_MAX_DEVICE			4
#define USB_MAXCONFIG			8
#define USB_MAXINTERFACES		8
#define USB_MAXENDPOINTS		16
#define USB_MAXCHILDREN			8	/* This is arbitrary */
#define USB_MAX_HUB			16

#define USB_CNTL_TIMEOUT 100 /* 100ms timeout */

typedef enum 
{
	USB_SPEED_UNKNOWN = 0,			/* enumerating */
	USB_SPEED_LOW, USB_SPEED_FULL,		/* usb 1.1 */
	USB_SPEED_HIGH,				/* usb 2.0 */
	USB_SPEED_WIRELESS,			/* wireless (usb 2.5) */
	USB_SPEED_SUPER,			/* usb 3.0 */
} usb_device_speed;

/* String descriptor */
__packed struct usb_string_descriptor {
	unsigned char  bLength;
	unsigned char  bDescriptorType;
	unsigned short wData[1];
}; //__attribute__ ((packed));

/* device request (setup) */
__packed struct devrequest {
	unsigned char requesttype;
	unsigned char request;
	unsigned short value;
	unsigned short index;
	unsigned short length;
}; //__attribute__ ((packed));


/* All standard descriptors have these 2 fields in common */
__packed struct usb_descriptor_header {
	unsigned char  bLength;
	unsigned char  bDescriptorType;
}; //__attribute__ ((packed));

/* Device descriptor */
__packed struct usb_device_descriptor {
	unsigned char  bLength;
	unsigned char  bDescriptorType;
	unsigned short bcdUSB;
	unsigned char  bDeviceClass;
	unsigned char  bDeviceSubClass;
	unsigned char  bDeviceProtocol;
	unsigned char  bMaxPacketSize0;
	unsigned short idVendor;
	unsigned short idProduct;
	unsigned short bcdDevice;
	unsigned char  iManufacturer;
	unsigned char  iProduct;
	unsigned char  iSerialNumber;
	unsigned char  bNumConfigurations;
}; //__attribute__ ((packed));


/* Endpoint descriptor */
__packed struct usb_endpoint_descriptor {
	unsigned char  bLength;
	unsigned char  bDescriptorType;
	unsigned char  bEndpointAddress;
	unsigned char  bmAttributes;
	unsigned short wMaxPacketSize;
	unsigned char  bInterval;
	unsigned char  bRefresh;
	unsigned char  bSynchAddress;

}; //__attribute__ ((packed));
/* Interface descriptor */
__packed struct usb_interface_descriptor {
	unsigned char  bLength;
	unsigned char  bDescriptorType;
	unsigned char  bInterfaceNumber;
	unsigned char  bAlternateSetting;
	unsigned char  bNumEndpoints;
	unsigned char  bInterfaceClass;
	unsigned char  bInterfaceSubClass;
	unsigned char  bInterfaceProtocol;
	unsigned char  iInterface;

	unsigned char  no_of_ep;
	unsigned char  num_altsetting;
	unsigned char  act_altsetting;
	struct usb_endpoint_descriptor ep_desc[USB_MAXENDPOINTS];
}; //__attribute__ ((packed));


/* Configuration descriptor information.. */
__packed struct usb_config_descriptor {
	unsigned char  bLength;
	unsigned char  bDescriptorType;
	unsigned short wTotalLength;
	unsigned char  bNumInterfaces;
	unsigned char  bConfigurationValue;
	unsigned char  iConfiguration;
	unsigned char  bmAttributes;
	unsigned char  MaxPower;

	unsigned char  no_of_if;		/* number of interfaces */
	struct usb_interface_descriptor if_desc[USB_MAXINTERFACES];
}; //__attribute__ ((packed));


struct usb_device {
	int devnum;			/* Device number on USB bus */
	int slow;			/* Slow device? */
	char mf[32];			/* manufacturer */
	char prod[32];			/* product */
	char serial[32];		/* serial number */

	int maxpacketsize;		/* Maximum packet size; encoded as 0,1,2,3 = 8,16,32,64 */
	unsigned int toggle[2];		/* one bit for each endpoint ([0] = IN, [1] = OUT) */
	unsigned int halted[2];		/* endpoint halts; one bit per endpoint # & direction; */
			    /* [0] = IN, [1] = OUT */
	int epmaxpacketin[16];		/* INput endpoint specific maximums */
	int epmaxpacketout[16];		/* OUTput endpoint specific maximums */

	int configno;			/* selected config number */
	struct usb_device_descriptor descriptor; /* Device Descriptor */
	struct usb_config_descriptor config; /* config descriptor */

	int have_langid;		/* whether string_langid is valid yet */
	int string_langid;		/* language ID for strings */
	int (*irq_handle)(struct usb_device *dev);
	unsigned long irq_status;
	int irq_act_len;		/* transfered bytes */
	void *privptr;
	/*
	 * Child devices -  if this is a hub device
	 * Each instance needs its own set of data structures.
	 */
	unsigned long status;
	int act_len;			/* transfered bytes */
	int maxchild;			/* Number of ports if hub */
	struct usb_device *parent;
	struct usb_device *children[USB_MAXCHILDREN];

	int portnum;			/* device on which port ?*/	

	usb_device_speed speed;
	int slot_id;
};

/**********************************************************************
 * this is how the lowlevel part communicate with the outer world
 */

int usb_lowlevel_init(void);
int usb_lowlevel_stop(void);
int submit_bulk_msg(struct usb_device *dev, unsigned long pipe, void *buffer,int transfer_len);
int submit_control_msg(struct usb_device *dev, unsigned long pipe, void *buffer,
			int transfer_len,struct devrequest *setup);
int submit_int_msg(struct usb_device *dev, unsigned long pipe, void *buffer,
			int transfer_len, int interval);
int usb_silent_reset(void);

/* Defines */
#define USB_UHCI_VEND_ID 0x8086
#define USB_UHCI_DEV_ID	 0x7112

#define USB_MAX_STOR_DEV 5
block_dev_desc_t *usb_stor_get_dev(int index);
int usb_stor_scan(int mode);
int usb_stor_info(void);
int usb_stor_max_dev(void);

#ifdef CONFIG_USB_PL2303
int PL2303_scan(struct usb_device *dev);
void PL2303_serial_putc (const char c);
void PL2303_serial_puts (const char *s);
int PL2303_serial_getc (void);
int PL2303_serial_tstc (void);
extern struct usb_device *pPL2303Dev;
#endif

#ifdef CONFIG_USB_KEYBOARD

int drv_usb_kbd_init(void);
int usb_kbd_deregister(void);

#endif
/* routines */
int usb_electrical_test(char *argv); /* USB Electrical test */
int usb_init(int iPortNum); /* initialize the USB Controller */
int usb_silent_init(void);
int usb_stop(void); /* stop the USB Controller */
int usb_speed(int speed);   /* initialize the USB Controller speed */
int usb_sendse0nak(void);     /* Set usb SE0 NAK */
int usb_sendtestj(void);     /* Set usb Test J */
int usb_sendtestk(void);     /* Set usb Test K */
int usb_sendtestpacket(void); /* Send usb test packet */

int usb_set_protocol(struct usb_device *dev, int ifnum, int protocol);
int usb_set_idle(struct usb_device *dev, int ifnum, int duration, int report_id);
struct usb_device * usb_get_dev_index(int index);
int usb_control_msg(struct usb_device *dev, unsigned int pipe,
			unsigned char request, unsigned char requesttype,
			unsigned short value, unsigned short index,
			void *data, unsigned short size, int timeout);
int usb_bulk_msg(struct usb_device *dev, unsigned int pipe,
			void *data, int len, int *actual_length, int timeout);
int usb_submit_int_msg(struct usb_device *dev, unsigned long pipe,
			void *buffer,int transfer_len, int interval);
void usb_disable_asynch(int disable);
int usb_maxpacket(struct usb_device *dev,unsigned long pipe);
void __inline__ wait_ms(unsigned long ms);
int usb_get_configuration_no(struct usb_device *dev,unsigned char *buffer,int cfgno);
int usb_get_report(struct usb_device *dev, int ifnum, unsigned char type, unsigned char id, void *buf, int size);
int usb_get_class_descriptor(struct usb_device *dev, int ifnum,
		unsigned char type, unsigned char id, void *buf, int size);
int usb_clear_halt(struct usb_device *dev, int pipe);
int usb_string(struct usb_device *dev, int index, char *buf, size_t size);
int usb_set_interface(struct usb_device *dev, int interface, int alternate);

/* big endian -> little endian conversion */
/* some CPUs are already little endian e.g. the ARM920T */
#define __swap_16(x) \
	({ unsigned short x_ = (unsigned short)x; \
	 (unsigned short)( \
		((x_ & 0x00FFU) << 8) | ((x_ & 0xFF00U) >> 8) ); \
	})
#define __swap_32(x) \
	({ unsigned long x_ = (unsigned long)x; \
	 (unsigned long)( \
		((x_ & 0x000000FFUL) << 24) | \
		((x_ & 0x0000FF00UL) <<	 8) | \
		((x_ & 0x00FF0000UL) >>	 8) | \
		((x_ & 0xFF000000UL) >> 24) ); \
	})

#ifdef LITTLEENDIAN
# define swap_16(x) (x)
# define swap_32(x) (x)
#else
# define swap_16(x) __swap_16(x)
# define swap_32(x) __swap_32(x)
#endif /* LITTLEENDIAN */

/*
 * Calling this entity a "pipe" is glorifying it. A USB pipe
 * is something embarrassingly simple: it basically consists
 * of the following information:
 *  - device number (7 bits)
 *  - endpoint number (4 bits)
 *  - current Data0/1 state (1 bit)
 *  - direction (1 bit)
 *  - speed (1 bit)
 *  - max packet size (2 bits: 8, 16, 32 or 64)
 *  - pipe type (2 bits: control, interrupt, bulk, isochronous)
 *
 * That's 18 bits. Really. Nothing more. And the USB people have
 * documented these eighteen bits as some kind of glorious
 * virtual data structure.
 *
 * Let's not fall in that trap. We'll just encode it as a simple
 * unsigned int. The encoding is:
 *
 *  - max size:		bits 0-1	(00 = 8, 01 = 16, 10 = 32, 11 = 64)
 *  - direction:	bit 7		(0 = Host-to-Device [Out], 1 = Device-to-Host [In])
 *  - device:		bits 8-14
 *  - endpoint:		bits 15-18
 *  - Data0/1:		bit 19
 *  - speed:		bit 26		(0 = Full, 1 = Low Speed)
 *  - pipe type:	bits 30-31	(00 = isochronous, 01 = interrupt, 10 = control, 11 = bulk)
 *
 * Why? Because it's arbitrary, and whatever encoding we select is really
 * up to us. This one happens to share a lot of bit positions with the UHCI
 * specification, so that much of the uhci driver can just mask the bits
 * appropriately.
 */
/* Create various pipes... */
#define create_pipe(dev,endpoint) \
		(((dev)->devnum << 8) | (endpoint << 15) | ((dev)->slow << 26) | (dev)->maxpacketsize)
#define default_pipe(dev) ((dev)->slow <<26)

#define usb_sndctrlpipe(dev,endpoint)	((PIPE_CONTROL << 30) | create_pipe(dev,endpoint))
#define usb_rcvctrlpipe(dev,endpoint)	((PIPE_CONTROL << 30) | create_pipe(dev,endpoint) | USB_DIR_IN)
#define usb_sndisocpipe(dev,endpoint)	((PIPE_ISOCHRONOUS << 30) | create_pipe(dev,endpoint))
#define usb_rcvisocpipe(dev,endpoint)	((PIPE_ISOCHRONOUS << 30) | create_pipe(dev,endpoint) | USB_DIR_IN)
#define usb_sndbulkpipe(dev,endpoint)	((PIPE_BULK << 30) | create_pipe(dev,endpoint))
#define usb_rcvbulkpipe(dev,endpoint)	((PIPE_BULK << 30) | create_pipe(dev,endpoint) | USB_DIR_IN)
#define usb_sndintpipe(dev,endpoint)	((PIPE_INTERRUPT << 30) | create_pipe(dev,endpoint))
#define usb_rcvintpipe(dev,endpoint)	((PIPE_INTERRUPT << 30) | create_pipe(dev,endpoint) | USB_DIR_IN)
#define usb_snddefctrl(dev)		((PIPE_CONTROL << 30) | default_pipe(dev))
#define usb_rcvdefctrl(dev)		((PIPE_CONTROL << 30) | default_pipe(dev) | USB_DIR_IN)

/* The D0/D1 toggle bits */
#define usb_gettoggle(dev, ep, out) (((dev)->toggle[out] >> ep) & 1)
#define usb_dotoggle(dev, ep, out)  ((dev)->toggle[out] ^= (1 << ep))
#define usb_settoggle(dev, ep, out, bit) ((dev)->toggle[out] = ((dev)->toggle[out] & ~(1 << ep)) | ((bit) << ep))

/* Endpoint halt control/status */
#define usb_endpoint_out(ep_dir)	(((ep_dir >> 7) & 1) ^ 1)
#define usb_endpoint_halt(dev, ep, out) ((dev)->halted[out] |= (1 << (ep)))
#define usb_endpoint_running(dev, ep, out) ((dev)->halted[out] &= ~(1 << (ep)))
#define usb_endpoint_halted(dev, ep, out) ((dev)->halted[out] & (1 << (ep)))

#define usb_packetid(pipe)	(((pipe) & USB_DIR_IN) ? USB_PID_IN : USB_PID_OUT)

#define usb_pipeout(pipe)	((((pipe) >> 7) & 1) ^ 1)
#define usb_pipein(pipe)	(((pipe) >> 7) & 1)
#define usb_pipedevice(pipe)	(((pipe) >> 8) & 0x7f)
#define usb_pipe_endpdev(pipe)	(((pipe) >> 8) & 0x7ff)
#define usb_pipeendpoint(pipe)	(((pipe) >> 15) & 0xf)
#define usb_pipedata(pipe)	(((pipe) >> 19) & 1)
#define usb_pipeslow(pipe)	(((pipe) >> 26) & 1)
#define usb_pipetype(pipe)	(((pipe) >> 30) & 3)
#define usb_pipeisoc(pipe)	(usb_pipetype((pipe)) == PIPE_ISOCHRONOUS)
#define usb_pipeint(pipe)	(usb_pipetype((pipe)) == PIPE_INTERRUPT)
#define usb_pipecontrol(pipe)	(usb_pipetype((pipe)) == PIPE_CONTROL)
#define usb_pipebulk(pipe)	(usb_pipetype((pipe)) == PIPE_BULK)


/*************************************************************************
 * Hub Stuff
 */
__packed struct usb_port_status {
	unsigned short wPortStatus;
	unsigned short wPortChange;
}; //__attribute__ ((packed));

__packed struct usb_hub_status {
	unsigned short wHubStatus;
	unsigned short wHubChange;
}; //__attribute__ ((packed));


/* Hub descriptor */
__packed struct usb_hub_descriptor {
	unsigned char  bLength;
	unsigned char  bDescriptorType;
	unsigned char  bNbrPorts;
	unsigned short wHubCharacteristics;
	unsigned char  bPwrOn2PwrGood;
	unsigned char  bHubContrCurrent;
	unsigned char  DeviceRemovable[(USB_MAXCHILDREN+1+7)/8];
	unsigned char  PortPowerCtrlMask[(USB_MAXCHILDREN+1+7)/8];
		/* DeviceRemovable and PortPwrCtrlMask want to be variable-length
	   bitmaps that hold max 255 entries. (bit0 is ignored) */
}; //__attribute__ ((packed));


struct usb_hub_device {
	struct usb_device *pusb_dev;
	struct usb_hub_descriptor desc;
};

__packed struct usb_device_id {
	/* Used for product specific matches; range is inclusive */
	unsigned short	idVendor;
	unsigned short	idProduct;
}; //__attribute__ ((packed));

#define USB_DEVICE(vend, prod) \
	.idVendor = (vend), \
	.idProduct = (prod)
/*
__packed struct usb_ctrlrequest {
	__u8 bRequestType;
	__u8 bRequest;
	__le16 wValue;
	__le16 wIndex;
	__le16 wLength;
}; // __attribute__ ((packed));
*/

#define roundup(x, y) ((((x) + ((y) - 1)) / (y)) * (y))
#define DIV_ROUND_UP(n,d) (((n) + (d) - 1) / (d))
#define clamp_val(val, min, max) ({             \
        typeof(val) __val = (val);              \
        typeof(val) __min = (min);              \
        typeof(val) __max = (max);              \
        __val = __val < __min ? __min: __val;   \
        __val > __max ? __max: __val; })


/*-------------------------------------------------------------------------*/

/**
 * usb_endpoint_num - get the endpoint's number
 * @epd: endpoint to be checked
 *
 * Returns @epd's number: 0 to 15.
 */
static inline int usb_endpoint_num(const struct usb_endpoint_descriptor *epd)
{
	return epd->bEndpointAddress & USB_ENDPOINT_NUMBER_MASK;
}

/**
 * usb_endpoint_type - get the endpoint's transfer type
 * @epd: endpoint to be checked
 *
 * Returns one of USB_ENDPOINT_XFER_{CONTROL, ISOC, BULK, INT} according
 * to @epd's transfer type.
 */
static inline int usb_endpoint_type(const struct usb_endpoint_descriptor *epd)
{
	return epd->bmAttributes & USB_ENDPOINT_XFERTYPE_MASK;
}

/**
 * usb_endpoint_dir_in - check if the endpoint has IN direction
 * @epd: endpoint to be checked
 *
 * Returns true if the endpoint is of type IN, otherwise it returns false.
 */
static inline int usb_endpoint_dir_in(const struct usb_endpoint_descriptor *epd)
{
	return ((epd->bEndpointAddress & USB_ENDPOINT_DIR_MASK) == USB_DIR_IN);
}

/**
 * usb_endpoint_dir_out - check if the endpoint has OUT direction
 * @epd: endpoint to be checked
 *
 * Returns true if the endpoint is of type OUT, otherwise it returns false.
 */
static inline int usb_endpoint_dir_out(
				const struct usb_endpoint_descriptor *epd)
{
	return ((epd->bEndpointAddress & USB_ENDPOINT_DIR_MASK) == USB_DIR_OUT);
}

/**
 * usb_endpoint_xfer_bulk - check if the endpoint has bulk transfer type
 * @epd: endpoint to be checked
 *
 * Returns true if the endpoint is of type bulk, otherwise it returns false.
 */
static inline int usb_endpoint_xfer_bulk(
				const struct usb_endpoint_descriptor *epd)
{
	return ((epd->bmAttributes & USB_ENDPOINT_XFERTYPE_MASK) ==
		USB_ENDPOINT_XFER_BULK);
}

/**
 * usb_endpoint_xfer_control - check if the endpoint has control transfer type
 * @epd: endpoint to be checked
 *
 * Returns true if the endpoint is of type control, otherwise it returns false.
 */
static inline int usb_endpoint_xfer_control(
				const struct usb_endpoint_descriptor *epd)
{
	return ((epd->bmAttributes & USB_ENDPOINT_XFERTYPE_MASK) ==
		USB_ENDPOINT_XFER_CONTROL);
}

/**
 * usb_endpoint_xfer_int - check if the endpoint has interrupt transfer type
 * @epd: endpoint to be checked
 *
 * Returns true if the endpoint is of type interrupt, otherwise it returns
 * false.
 */
static inline int usb_endpoint_xfer_int(
				const struct usb_endpoint_descriptor *epd)
{
	return ((epd->bmAttributes & USB_ENDPOINT_XFERTYPE_MASK) ==
		USB_ENDPOINT_XFER_INT);
}

/**
 * usb_endpoint_xfer_isoc - check if the endpoint has isochronous transfer type
 * @epd: endpoint to be checked
 *
 * Returns true if the endpoint is of type isochronous, otherwise it returns
 * false.
 */
static inline int usb_endpoint_xfer_isoc(
				const struct usb_endpoint_descriptor *epd)
{
	return ((epd->bmAttributes & USB_ENDPOINT_XFERTYPE_MASK) ==
		USB_ENDPOINT_XFER_ISOC);
}

/**
 * usb_endpoint_is_bulk_in - check if the endpoint is bulk IN
 * @epd: endpoint to be checked
 *
 * Returns true if the endpoint has bulk transfer type and IN direction,
 * otherwise it returns false.
 */
static inline int usb_endpoint_is_bulk_in(
				const struct usb_endpoint_descriptor *epd)
{
	return usb_endpoint_xfer_bulk(epd) && usb_endpoint_dir_in(epd);
}

/**
 * usb_endpoint_is_bulk_out - check if the endpoint is bulk OUT
 * @epd: endpoint to be checked
 *
 * Returns true if the endpoint has bulk transfer type and OUT direction,
 * otherwise it returns false.
 */
static inline int usb_endpoint_is_bulk_out(
				const struct usb_endpoint_descriptor *epd)
{
	return usb_endpoint_xfer_bulk(epd) && usb_endpoint_dir_out(epd);
}

/**
 * usb_endpoint_is_int_in - check if the endpoint is interrupt IN
 * @epd: endpoint to be checked
 *
 * Returns true if the endpoint has interrupt transfer type and IN direction,
 * otherwise it returns false.
 */
static inline int usb_endpoint_is_int_in(
				const struct usb_endpoint_descriptor *epd)
{
	return usb_endpoint_xfer_int(epd) && usb_endpoint_dir_in(epd);
}

/**
 * usb_endpoint_is_int_out - check if the endpoint is interrupt OUT
 * @epd: endpoint to be checked
 *
 * Returns true if the endpoint has interrupt transfer type and OUT direction,
 * otherwise it returns false.
 */
static inline int usb_endpoint_is_int_out(
				const struct usb_endpoint_descriptor *epd)
{
	return usb_endpoint_xfer_int(epd) && usb_endpoint_dir_out(epd);
}

/**
 * usb_endpoint_is_isoc_in - check if the endpoint is isochronous IN
 * @epd: endpoint to be checked
 *
 * Returns true if the endpoint has isochronous transfer type and IN direction,
 * otherwise it returns false.
 */
static inline int usb_endpoint_is_isoc_in(
				const struct usb_endpoint_descriptor *epd)
{
	return usb_endpoint_xfer_isoc(epd) && usb_endpoint_dir_in(epd);
}

/**
 * usb_endpoint_is_isoc_out - check if the endpoint is isochronous OUT
 * @epd: endpoint to be checked
 *
 * Returns true if the endpoint has isochronous transfer type and OUT direction,
 * otherwise it returns false.
 */
static inline int usb_endpoint_is_isoc_out(
				const struct usb_endpoint_descriptor *epd)
{
	return usb_endpoint_xfer_isoc(epd) && usb_endpoint_dir_out(epd);
}

/*-------------------------------------------------------------------------*/


#endif /*_USB_H_ */
