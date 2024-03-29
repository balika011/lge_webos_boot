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
 * MHDRC-specific definitions
 * $Revision: #1 $
 */

#ifndef __MUSB_MHDRC_DEFS_H__
#define __MUSB_MHDRC_DEFS_H__

#include "mu_drcdf.h"

/*
 * MHDRC-specific definitions
 */

/*
 *     MUSBMHDRC Register map 
 */

/* Common USB registers */

#define MGC_O_MHDRC_FADDR	0x00	/* 8-bit */
#define MGC_O_MHDRC_POWER	0x01	/* 8-bit */

#define MGC_O_MHDRC_INTRTX	0x02	/* 16-bit */
#define MGC_O_MHDRC_INTRRX       0x04
#define MGC_O_MHDRC_INTRTXE      0x06  
#define MGC_O_MHDRC_INTRRXE      0x08  
#define MGC_O_MHDRC_INTRUSB      0x0A   /* 8 bit */
#define MGC_O_MHDRC_INTRUSBE     0x0B   /* 8 bit */
#define MGC_O_MHDRC_FRAME        0x0C  
#define MGC_O_MHDRC_INDEX        0x0E   /* 8 bit */
#define MGC_O_MHDRC_TESTMODE     0x0F   /* 8 bit */

/* Additional Control Registers */

#define	MGC_O_MHDRC_DEVCTL	0x60	/* 8 bit */    

#define MGC_O_MHDRC_HWVERS	0x6C	/* 16-bit */

/* offsets to registers in flat model */
#define MGC_O_MHDRC_TXMAXP	0x00
#define MGC_O_MHDRC_TXCSR	0x02
#define MGC_O_MHDRC_CSR0	MGC_O_MHDRC_TXCSR	/* re-used for EP0 */
#define MGC_O_MHDRC_RXMAXP	0x04
#define MGC_O_MHDRC_RXCSR	0x06
#define MGC_O_MHDRC_RXCOUNT	0x08
#define MGC_O_MHDRC_COUNT0	MGC_O_MHDRC_RXCOUNT	/* re-used for EP0 */
#define MGC_O_MHDRC_TXTYPE	0x0A
#define MGC_O_MHDRC_TYPE0	MGC_O_MHDRC_TXTYPE	/* re-used for EP0 */
#define MGC_O_MHDRC_TXINTERVAL	0x0B
#define MGC_O_MHDRC_NAKLIMIT0	MGC_O_MHDRC_TXINTERVAL	/* re-used for EP0 */
#define MGC_O_MHDRC_RXTYPE	0x0C
#define MGC_O_MHDRC_RXINTERVAL	0x0D
#define MGC_O_MHDRC_FIFOSIZE	0x0F
#define MGC_O_MHDRC_CONFIGDATA	MGC_O_MHDRC_FIFOSIZE	/* re-used for EP0 */

/* "bus control" registers */
#define MGC_O_MHDRC_TXFUNCADDR	0x00
#define MGC_O_MHDRC_TXHUBADDR	0x02
#define MGC_O_MHDRC_TXHUBPORT	0x03

#define MGC_O_MHDRC_RXFUNCADDR	0x04
#define MGC_O_MHDRC_RXHUBADDR	0x06
#define MGC_O_MHDRC_RXHUBPORT	0x07

/*
 *     MUSBHDRC Register bit masks
 */

/* POWER */

#define MGC_M_POWER_ISOUPDATE   0x80 
#define	MGC_M_POWER_SOFTCONN    0x40
#define	MGC_M_POWER_HSENAB	0x20
#define	MGC_M_POWER_HSMODE	0x10
#define MGC_M_POWER_RESET       0x08
#define MGC_M_POWER_RESUME      0x04
#define MGC_M_POWER_SUSPENDM    0x02
#define MGC_M_POWER_ENSUSPEND   0x01

/* TESTMODE */

#define MGC_M_TEST_FORCE_HOST   0x80
#define MGC_M_TEST_FIFO_ACCESS  0x40
#define MGC_M_TEST_FORCE_FS     0x20
#define MGC_M_TEST_FORCE_HS     0x10
#define MGC_M_TEST_PACKET       0x08
#define MGC_M_TEST_K            0x04
#define MGC_M_TEST_J            0x02
#define MGC_M_TEST_SE0_NAK      0x01

/* CSR0 in Peripheral and Host mode */

#define	MGC_M_CSR0_FLUSHFIFO      0x0100

/* CSR0 in Host mode */

#define MGC_M_CSR0_H_WR_DATATOGGLE   0x0400	/* set to allow setting: */
#define MGC_M_CSR0_H_DATATOGGLE	    0x0200	/* data toggle control */

/* New in 15-July-2005 (v1.4?) */
#define MGC_M_CSR0_H_NO_PING	0x0800

/* TxType/RxType */
#define MGC_M_TYPE_SPEED	0xc0
#define MGC_S_TYPE_SPEED	6
#define MGC_TYPE_SPEED_HIGH	1
#define MGC_TYPE_SPEED_FULL	2
#define MGC_TYPE_SPEED_LOW	3
#define MGC_M_TYPE_PROTO	0x30
#define MGC_S_TYPE_PROTO	4
#define MGC_M_TYPE_REMOTE_END	0xf

/* CONFIGDATA */

#define MGC_M_CONFIGDATA_MPRXE      0x80	/* auto bulk pkt combining */
#define MGC_M_CONFIGDATA_MPTXE      0x40	/* auto bulk pkt splitting */
#define MGC_M_CONFIGDATA_BIGENDIAN  0x20
#define MGC_M_CONFIGDATA_HBRXE      0x10	/* HB-ISO for RX */
#define MGC_M_CONFIGDATA_HBTXE      0x08	/* HB-ISO for TX */
#define MGC_M_CONFIGDATA_DYNFIFO    0x04	/* dynamic FIFO sizing */
#define MGC_M_CONFIGDATA_SOFTCONE   0x02	/* SoftConnect */
#define MGC_M_CONFIGDATA_UTMIDW     0x01   /* data width 0 => 8bits, 1 => 16bits */

/* TXCSR in Peripheral and Host mode */

#define MGC_M_TXCSR_AUTOSET       0x8000
#define MGC_M_TXCSR_ISO           0x4000
#define MGC_M_TXCSR_MODE          0x2000
#define MGC_M_TXCSR_DMAENAB       0x1000
#define MGC_M_TXCSR_FRCDATATOG    0x0800
#define MGC_M_TXCSR_DMAMODE       0x0400
#define MGC_M_TXCSR_CLRDATATOG    0x0040
#define MGC_M_TXCSR_FLUSHFIFO     0x0008
#define MGC_M_TXCSR_FIFONOTEMPTY  0x0002
#define MGC_M_TXCSR_TXPKTRDY      0x0001

/* TXCSR in Peripheral mode */

#define MGC_M_TXCSR_P_INCOMPTX    0x0080
#define MGC_M_TXCSR_P_SENTSTALL   0x0020
#define MGC_M_TXCSR_P_SENDSTALL   0x0010
#define MGC_M_TXCSR_P_UNDERRUN    0x0004

/* TXCSR in Host mode */

#define MGC_M_TXCSR_H_WR_DATATOGGLE   0x0200
#define MGC_M_TXCSR_H_DATATOGGLE      0x0100
#define MGC_M_TXCSR_H_NAKTIMEOUT  0x0080
#define MGC_M_TXCSR_H_RXSTALL     0x0020
#define MGC_M_TXCSR_H_ERROR       0x0004

/* RXCSR in Peripheral and Host mode */

#define MGC_M_RXCSR_AUTOCLEAR     0x8000
#define MGC_M_RXCSR_DMAENAB       0x2000
#define MGC_M_RXCSR_DISNYET       0x1000
#define MGC_M_RXCSR_DMAMODE       0x0800
#define MGC_M_RXCSR_INCOMPRX      0x0100
#define MGC_M_RXCSR_CLRDATATOG    0x0080
#define MGC_M_RXCSR_FLUSHFIFO     0x0010
#define MGC_M_RXCSR_DATAERROR     0x0008
#define MGC_M_RXCSR_FIFOFULL      0x0002
#define MGC_M_RXCSR_RXPKTRDY      0x0001

/* RXCSR in Peripheral mode */

#define MGC_M_RXCSR_P_ISO         0x4000
#define MGC_M_RXCSR_P_SENTSTALL   0x0040
#define MGC_M_RXCSR_P_SENDSTALL   0x0020
#define MGC_M_RXCSR_P_OVERRUN     0x0004

/* RXCSR in Host mode */

#define MGC_M_RXCSR_H_AUTOREQ     0x4000
#define MGC_M_RXCSR_H_WR_DATATOGGLE   0x0400
#define MGC_M_RXCSR_H_DATATOGGLE        0x0200
#define MGC_M_RXCSR_H_RXSTALL     0x0040
#define MGC_M_RXCSR_H_REQPKT      0x0020
#define MGC_M_RXCSR_H_ERROR       0x0004

/* HUBADDR */
#define MGC_M_HUBADDR_MULTI_TT		0x80

/* HWVERS */
#define MGC_M_HWVERS_RC		0x8000
#define MGC_S_HWVERS_RC		15
#define MGC_M_HWVERS_MAJOR	0x7c00
#define MGC_S_HWVERS_MAJOR	10
#define MGC_M_HWVERS_MINOR	0x03ff

/* TXCSR in Peripheral and Host mode */

#define MGC_M_TXCSR2_AUTOSET       0x80
#define MGC_M_TXCSR2_ISO           0x40
#define MGC_M_TXCSR2_MODE          0x20
#define MGC_M_TXCSR2_DMAENAB       0x10
#define MGC_M_TXCSR2_FRCDATATOG    0x08
#define MGC_M_TXCSR2_DMAMODE       0x04

#define MGC_M_TXCSR1_CLRDATATOG    0x40
#define MGC_M_TXCSR1_FLUSHFIFO     0x08
#define MGC_M_TXCSR1_FIFONOTEMPTY  0x02
#define MGC_M_TXCSR1_TXPKTRDY      0x01

/* TXCSR in Peripheral mode */

#define MGC_M_TXCSR1_P_INCOMPTX    0x80
#define MGC_M_TXCSR1_P_SENTSTALL   0x20
#define MGC_M_TXCSR1_P_SENDSTALL   0x10
#define MGC_M_TXCSR1_P_UNDERRUN    0x04

/* TXCSR in Host mode */

#define MGC_M_TXCSR1_H_NAKTIMEOUT  0x80
#define MGC_M_TXCSR1_H_RXSTALL     0x20
#define MGC_M_TXCSR1_H_ERROR       0x04

/* RXCSR in Peripheral and Host mode */

#define MGC_M_RXCSR2_AUTOCLEAR     0x80
#define MGC_M_RXCSR2_DMAENAB       0x20
#define MGC_M_RXCSR2_DISNYET       0x10
#define MGC_M_RXCSR2_DMAMODE       0x08
#define MGC_M_RXCSR2_INCOMPRX      0x01

#define MGC_M_RXCSR1_CLRDATATOG    0x80
#define MGC_M_RXCSR1_FLUSHFIFO     0x10
#define MGC_M_RXCSR1_DATAERROR     0x08
#define MGC_M_RXCSR1_FIFOFULL      0x02
#define MGC_M_RXCSR1_RXPKTRDY      0x01

/* RXCSR in Peripheral mode */

#define MGC_M_RXCSR2_P_ISO         0x40
#define MGC_M_RXCSR1_P_SENTSTALL   0x40
#define MGC_M_RXCSR1_P_SENDSTALL   0x20
#define MGC_M_RXCSR1_P_OVERRUN     0x04

/* RXCSR in Host mode */

#define MGC_M_RXCSR2_H_AUTOREQ     0x40
#define MGC_M_RXCSR1_H_RXSTALL     0x40
#define MGC_M_RXCSR1_H_REQPKT      0x20
#define MGC_M_RXCSR1_H_ERROR       0x04


/* 
 *  DRC register access macros
 */

/* Get offset for a given FIFO */
#define MGC_FIFO_OFFSET(_bEnd) (MGC_M_FIFO_EP0 + (_bEnd * 4))

#define MGC_END_OFFSET(_bEnd, _bOffset)	(0x100 + (0x10*_bEnd) + _bOffset)

#define MGC_BUSCTL_OFFSET(_bEnd, _bOffset)	(0x80 + (8*_bEnd) + _bOffset)

/* indexed vs. flat register model */
#ifdef MUSB_FLAT_REG
#define MGC_SelectEnd(_pBase, _bEnd)
#define MGC_ReadCsr8(_pBase, _bOffset, _bEnd) \
    MGC_Read8(_pBase, MGC_END_OFFSET(_bEnd, _bOffset))
#define MGC_ReadCsr16(_pBase, _bOffset, _bEnd) \
    MGC_Read16(_pBase, MGC_END_OFFSET(_bEnd, _bOffset))
#define MGC_WriteCsr8(_pBase, _bOffset, _bEnd, _bData) \
    MGC_Write8(_pBase, MGC_END_OFFSET(_bEnd, _bOffset), _bData)
#define MGC_WriteCsr16(_pBase, _bOffset, _bEnd, _bData) \
    MGC_Write16(_pBase, MGC_END_OFFSET(_bEnd, _bOffset), _bData)
#else
#define MGC_SelectEnd(_pBase, _bEnd) \
    MGC_Write8(_pBase, MGC_O_MHDRC_INDEX, _bEnd)
#define MGC_ReadCsr8(_pBase, _bOffset, _bEnd) \
    MGC_Read8(_pBase, (_bOffset + 0x10))
#define MGC_ReadCsr16(_pBase, _bOffset, _bEnd) \
    MGC_Read16(_pBase, (_bOffset + 0x10))
#define MGC_WriteCsr8(_pBase, _bOffset, _bEnd, _bData) \
    MGC_Write8(_pBase, (_bOffset + 0x10), _bData)
#define MGC_WriteCsr16(_pBase, _bOffset, _bEnd, _bData) \
    MGC_Write16(_pBase, (_bOffset + 0x10), _bData)
#endif

#define MGC_ReadRxCount(_pController) \
    MGC_Read16((uint8_t*)_pController->pControllerAddressIst, M_REG_RXCOUNT);

#define MGC_VBUS_MASK            0x18    /* DevCtl D4 - D3 */

#endif	/* multiple inclusion protection */
