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
/*-----------------------------------------------------------------------------
 *
 * $Author: dtvbm11 $
 * $Date: 2015/01/09 $
 * $RCSfile: sif_hw.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file sif_hw.h
 *  Define SIF hardware information.
 */

#ifndef SIF_HW_H
#define SIF_HW_H

//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------
#include "x_typedef.h"

//---------------------------------------------------------------------------
// Macro definitions
//---------------------------------------------------------------------------
#define SIF_USE_LENGTHER_PAGE            1//To be Del

#define IO_VAL 0x21000
// PERMISC
#define RW_PERMISC_REG                ((UINT32)(IO_VAL+0x0000))
#define SIF1_SW_RST                   ((UINT32)1<<29 )
#define SIF0_SW_RST                   ((UINT32)1<<28 )
#define SIF2_SW_RST                   ((UINT32)1<<26 )
#define SIF0_SW_RST_MSK               (0x10000000)
#define SIF1_SW_RST_MSK               (0x20000000)
#define SIF2_SW_RST_MSK               (0x04000000)

#define SCL_STRETCH                   ((UINT32)1 << 0)
#define SM_EN                         ((UINT32)1 << 1)
// SDA_STATE:	SDA Value (1:high, 0:low)
#define SDA_STATE                     ((UINT32)1 << 2)
// SCL_STATE:	SDA Value (1:high, 0:low)
#define SCL_STATE                     ((UINT32)1 << 3)
// CS_STATUS:	SCL stretching status
#define CS_STATUS                     ((UINT32)1 << 4)
#define DEGLITCH_EN                   ((UINT32)1 << 5)
//deglitch set
#define DEGLITCH_SET(x)                    ((((UINT32)(x)) & 0x00FF) << 8)
#define DEGLITCH_SET_MSK                   (0x00FF<< 8)

// CLK_DIV: Parameter of divider to divide 27M for SCL.
#define CLK_DIV(x)                    ((((UINT32)(x)) & 0x0FFF) << 16)
#define CLK_DIV_MSK                   (0x0FFF << 16)
#define ODRAIN                        ((UINT32)1 << 31)

// TRI: Trigger serial interface. Read back as serial interface busy.
#define TRI                           ((UINT32)1 << 0)
#define TRI_MSK                       ((UINT32)1 << 0)
// PGLEN: Page length of sequential read/write. The maximum is 36 bytes. Set 0 as 1 bytes.
#if SIF_USE_LENGTHER_PAGE
#define PGLEN(x)                      ((((UINT32)(x)) & 0x003f) << 8)
#define PGLEN_MSK                     (0x3f00)
#endif

// SIF_MODE: start ==>data==>stop, start==>data, data==>stop, only data
#define SIF_MODE(x)                   ((((UINT32)(x)) & 0x0007) << 4)
typedef enum
{
    MODE_START        = 0x1,
    MODE_WR_DATA      = 0x2,
    MODE_STOP         = 0x3,
    MODE_RD_DATA_LAST = 0x4,
    MODE_RD_DATA      = 0x5,
} SIF_MODE_T;
#define SIF_MODE_MSK                  ((UINT32)0x70)

// ACK: 	Acknowledge bits.
#if SIF_USE_LENGTHER_PAGE
#define ACK(x)                        ((((UINT32)(x)) & 0xFFFF0000) >> 16)//SMCTL1
#define ACKH(x)                        ((((UINT32)(x)) & 0x7FFFF800) >> 11)//SMCTL3
#endif

/*
* 5399(Mustang) (SIF0, SIF1)       (PD_SIF0, PD_SIF1).
* 5882(Capri)   (SIF0, SIF1)      .(PD_SIF0, PD_SIF1).
* 5890(Oryx)    (SIF0, SIF1, SIF2).(PD_SIF0, PD_SIF1).
* 5861(Gazelle) (SIF0, SIF1, SIF2).(PD_SIF0, PD_SIF1).
*/
// SM0CTL_REG
#define RW_SM0_CTRL0_REG              ((UINT32)(IO_VAL+0x0240))
#define RW_SM0_CTRL1_REG              ((UINT32)(IO_VAL+0x0244))
#define RW_SM0_CTRL2_REG              ((UINT32)(IO_VAL+0x0248))
#define RW_SM0_CTRL3_REG              ((UINT32)(IO_VAL+0x02fc))
#define RW_SM0_DEV_REG                ((UINT32)(IO_VAL+0x024c))

#define RW_SM0_D0_REG                 ((UINT32)(IO_VAL+0x0250))
#define RW_SM0_D1_REG                 ((UINT32)(IO_VAL+0x0254))
#define RW_SM0_D2_REG                 ((UINT32)(IO_VAL+0x02E0))
#define RW_SM0_D3_REG                 ((UINT32)(IO_VAL+0x02E4))
#define RW_SM0_D4_REG                 ((UINT32)(IO_VAL+0x02E8))
#define RW_SM0_D5_REG                 ((UINT32)(IO_VAL+0x02EC))
#define RW_SM0_D6_REG                 ((UINT32)(IO_VAL+0x02F0))
#define RW_SM0_D7_REG                 ((UINT32)(IO_VAL+0x02F4))
#define RW_SM0_D8_REG                 ((UINT32)(IO_VAL+0x02F8))


// PINTEN_REG: Peripheral interrupt enable register.
#define RW_PINTEN_REG                 ((UINT32)(IO_VAL+0x025C))
#define SIF_M0_INTEN                  ((UINT32)1 << 0)
#define SIF_M1_INTEN                  ((UINT32)1 << 1)
#define SIF_M2_INTEN                  ((UINT32)1 << 2)
// PINTST_REG: Peripheral interrupt status register.
#define RW_PINTST_REG                 ((UINT32)(IO_VAL+0x0260))
#define SIF_M0_INTST                  ((UINT32)1 << 0)
#define SIF_M1_INTST                  ((UINT32)1 << 1)
#define SIF_M2_INTST                  ((UINT32)1 << 2)
// PINTCLR_REG: Peripheral interrupt clear register.
#define RW_PINTCLR_REG                ((UINT32)(IO_VAL+0x0264))
#define SIF_M0_INTCLR                 ((UINT32)1 << 0)
#define SIF_M1_INTCLR                 ((UINT32)1 << 1)
#define SIF_M2_INTCLR                 ((UINT32)1 << 2)


// SM1CTL_REG
#define RW_SM1_CTRL0_REG              ((UINT32)(IO_VAL+0x0270))
#define RW_SM1_CTRL1_REG              ((UINT32)(IO_VAL+0x0274))
#define RW_SM1_CTRL2_REG              ((UINT32)(IO_VAL+0x0278))
#define RW_SM1_CTRL3_REG              ((UINT32)(IO_VAL+0x02A8))
#define RW_SM1_DEV_REG                ((UINT32)(IO_VAL+0x027C))

#define RW_SM1_D0_REG                 ((UINT32)(IO_VAL+0x0280))
#define RW_SM1_D1_REG                 ((UINT32)(IO_VAL+0x0284))
#define RW_SM1_D2_REG                 ((UINT32)(IO_VAL+0x0288))
#define RW_SM1_D3_REG                 ((UINT32)(IO_VAL+0x028C))
#define RW_SM1_D4_REG                 ((UINT32)(IO_VAL+0x0290))
#define RW_SM1_D5_REG                 ((UINT32)(IO_VAL+0x0294))
#define RW_SM1_D6_REG                 ((UINT32)(IO_VAL+0x0298))
#define RW_SM1_D7_REG                 ((UINT32)(IO_VAL+0x029C))
#define RW_SM1_D8_REG                 ((UINT32)(IO_VAL+0x02A4))


// SM2CTL_REG
#define RW_SM2_CTRL0_REG              ((UINT32)(IO_VAL+0x0200))
#define RW_SM2_CTRL1_REG              ((UINT32)(IO_VAL+0x0204))
#define RW_SM2_CTRL2_REG              ((UINT32)(IO_VAL+0x0208))
#define RW_SM2_CTRL3_REG              ((UINT32)(IO_VAL+0x0238))
#define RW_SM2_DEV_REG                ((UINT32)(IO_VAL+0x020C))

#define RW_SM2_D0_REG                 ((UINT32)(IO_VAL+0x0210))
#define RW_SM2_D1_REG                 ((UINT32)(IO_VAL+0x0214))
#define RW_SM2_D2_REG                 ((UINT32)(IO_VAL+0x0218))
#define RW_SM2_D3_REG                 ((UINT32)(IO_VAL+0x021C))
#define RW_SM2_D4_REG                 ((UINT32)(IO_VAL+0x0220))
#define RW_SM2_D5_REG                 ((UINT32)(IO_VAL+0x0224))
#define RW_SM2_D6_REG                 ((UINT32)(IO_VAL+0x0228))
#define RW_SM2_D7_REG                 ((UINT32)(IO_VAL+0x022C))
#define RW_SM2_D8_REG                 ((UINT32)(IO_VAL+0x0234))


// ARM_INTEN_REG: Pdwnc interrupt enable register.
#define RW_AINTEN_REG                 ((UINT32)0x0044)
#define SIF_PD_M0_INTEN               ((UINT32)1 << 8)
#define SIF_PD_M1_INTEN               ((UINT32)1 << 20)
// INTSTA_REG: Pdwnc interrupt status register.
#define RW_AINTSTA_REG                ((UINT32)0x0040)
#define SIF_PD_M0_INTST               ((UINT32)1 << 8)
#define SIF_PD_M1_INTST               ((UINT32)1 << 20)
// INTCLR_REG: Pdwnc interrupt clear register.
#define RW_AINTCLR_REG                ((UINT32)0x0048)
#define SIF_PD_M0_INTCLR              ((UINT32)1 << 8)
#define SIF_PD_M1_INTCLR              ((UINT32)1 << 20)


/// WSM0CTL0_REG: 8295B Serial interface master 0 control 0 register.
#define RW_WSM0_CTRL0_REG              ((UINT32)0x7800)

/// WSM0CTL1_REG: 8295B Serial interface master 0 control 1 register.
#define RW_WSM0_CTRL1_REG              ((UINT32)0x7804)

/// WSM0D0_REG:8295B Serial interface master 0 data port 0 register.
#define RW_WSM0_D0_REG                 ((UINT32)0x7808)

/// WSM0D1_REG:8295B Serial interface master 0 data port 1 register.
#define RW_WSM0_D1_REG                 ((UINT32)0x780C)

//WINTEN_REG: 8295b interrupt enable register.
#define RW_WINTEN_REG                  ((UINT32)0x7004)
#define SIF_M4_INTEN                  ((UINT32)1 << 0)
#define SIF_M5_INTEN                  ((UINT32)1 << 1)

/// WINTSTA_REG: 8295B interrupt status register.
#define RW_WINTSTA_REG                 ((UINT32)0x7008)
#define SIF_M4_INTST                  ((UINT32)1 << 0)
#define SIF_M5_INTST                  ((UINT32)1 << 1)

/// WINTCLR_REG: 8295B interrupt clear register.
#define RW_WINTCLR_REG                ((UINT32)0x700c)
#define SIF_M4_INTCLR                  ((UINT32)1 << 0)
#define SIF_M5_INTCLR                  ((UINT32)1 << 1)


#endif // SIF_HW_H
