/*
 * Copyright (c) 2012 MediaTek Inc.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *  * Redistributions of source code must retain the above copyright
 *	notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *	notice, this list of conditions and the following disclaimer in
 *	the documentation and/or other materials provided with the
 *	distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */
#include "../udc.h"

/* hardware spec */
#define MT_EP_NUM 4

#define EP0_MAX_PACKET_SIZE	64

 /* USB transfer directions */
#define UDC_DIR_IN	DEVICE_WRITE	/* val: 0x80 */
#define UDC_DIR_OUT	DEVICE_READ	/* val: 0x00 */

 /* endpoint data - mt_ep */
 struct udc_endpoint {
     /* rx side */
     struct urb *rcv_urb;    /* active urb */
 
     /* tx side */
     struct urb *tx_urb; /* active urb */
 
     /* info from hsusb */
     struct udc_request *req;
     unsigned int bit;   /* EPT_TX/EPT_RX */
     unsigned char num;
     unsigned char in;
     unsigned short maxpkt;
     int status; /* status for error handling */
 
     unsigned int sent;      /* data already sent */
     unsigned int last;      /* data sent in last packet XXX do we need this */
     unsigned char mode; /* double buffer */
 };

 struct urb {
     struct udc_endpoint *endpoint;
     struct udc_device *device;
     struct setup_packet device_request;
 
     u8 *buffer;
     unsigned int actual_length;
 };
 
 /* mt_usb defines */
 typedef enum
 {
     EP0_IDLE = 0,
     EP0_RX,
     EP0_TX,
 } EP0_STATE;

