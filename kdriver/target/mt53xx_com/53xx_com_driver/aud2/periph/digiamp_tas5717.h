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
 * $RCSfile: digiamp_others.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file digiamp_others.h
 *  Brief of file digiamp_others.h. \n
 */

#ifndef DIGIAMP_TAS5717_H
#define DIGIAMP_TAS5717_H

//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------

#include "x_typedef.h"
#include "aud_if.h"

//---------------------------------------------------------------------------
// Constant definitions
//---------------------------------------------------------------------------

//TAS5717
#define SIF_CLK_DIV							0x100

// The 7-bit address for TAS5717 is 0101 010 (0x54) 
// or 0101 011 (0x56) defined by A_SEL (external pulldown for
// 0x54 and pullup for 0x56).
// Stero device with Headphone should use 0x54 as its device address.

#ifdef CC_AUD_AMP_TYPE_TAS5721  //TAS5721 driver is almost the same as TAS5717 except Slave Address
#define TAS5717_ADDR                        0x36    // A_SEL=0
#else
#define TAS5717_ADDR                        0x54    // A_SEL=0
#endif
#define TAS5717_ADDR_1                      0x56    // A_SEL=1

#define TAS5717_Device_ID				    0x01
#define TAS5717_ERR_STATUS				    0x02
#define TAS5717_SYS_CONTROL1				0x03
#define TAS5717_SERIAL_DATA				0x04
#define TAS5717_SYS_CONTROL2				0x05
    #define ALL_CH_SHDN                     0x40
    #define ALL_CH_EN                       0x00
#define TAS5717_SOFT_MUTE					0x06
    #define SW_MUTE_CH1                     (1<<0)
    #define SW_MUTE_CH2                     (1<<1)
    #define SW_MUTE_CH3                     (1<<2)

#define TAS5717_MASTER_VOLUME               0x07
#define TAS5717_CH1_VOLUME                  0x08
#define TAS5717_CH2_VOLUME                  0x09
#define TAS5717_CH3_VOLUME                  0x0A
#define TAS5717_HP_VOLUME                   0x0C

#define TAS5717_VOLUME_CONFIG				0x0E
    #define FADE_SLEW_43MS                   0x90
    #define FADE_SLEW_85MS                   0x91
    #define FADE_SLEW_171MS                  0x92
    #define FADE_SLEW_21MS                   0x93
#define TAS5717_MAX_DUTY_CYCLE              0x0F
#define TAS5717_MODULATION_LIMIT			0x10
#define TAS5717_IC_DELAY_CH1				0x11
#define TAS5717_IC_DELAY_CH2				0x12
#define TAS5717_IC_DELAY_CH3				0x13
#define TAS5717_IC_DELAY_CH4				0x14
#define TAS5717_START_STOP_PERIOD			0x1a
#define TAS5717_TRIM						0x1b
#define TAS5717_BKND_ERR					0x1c

#define TAS5717_INPUT_MUX                   0x20
#define TAS5717_CH4_SRC_SEL                 0x21
#define TAS5717_PWM_OUT_MUX                 0x25

#define TAS5717_PEQ_CH1BQ0                 0x26
#define TAS5717_PEQ_CH1BQ1                 0x27
#define TAS5717_PEQ_CH1BQ2                 0x28
#define TAS5717_PEQ_CH1BQ3                 0x29
#define TAS5717_PEQ_CH1BQ4                 0x2a
#define TAS5717_PEQ_CH1BQ5                 0x2b
#define TAS5717_PEQ_CH1BQ6                 0x2c

#define TAS5717_PEQ_CH2BQ0                 0x30
#define TAS5717_PEQ_CH2BQ1                 0x31
#define TAS5717_PEQ_CH2BQ2                 0x32
#define TAS5717_PEQ_CH2BQ3                 0x33
#define TAS5717_PEQ_CH2BQ4                 0x34
#define TAS5717_PEQ_CH2BQ5                 0x35
#define TAS5717_PEQ_CH2BQ6                 0x36

#define TAS5717_DRC1_SOFTFILT_ALPHA_OMEGA			0x3B
#define TAS5717_DRC1_ATTACK_RELEASE_RATE			0x3C
#define TAS5717_DRC2_SOFTFILT_ALPHA_OMEGA			0x3E
#define TAS5717_DRC2_ATTACK_RELEASE_RATE			0x3F
#define TAS5717_DRC1_ATTACK_THRESHOLD	            0x40
#define TAS5717_DRC2_ATTACK_THRESHOLD	            0x43
#define TAS5717_DRC_CONTROL				0x46

#define TAS5717_BANK_SWITCH				0x50

#define TAS5717_POSTSCALE               0x56
#define TAS5717_PRESCALE                0x57

#define TAS5717_UPDATE_DEV_ADDR_KEY     0xF8
#define TAS5717_UPDATE_DEV_ADDR         0xF9

//---------------------------------------------------------------------------
// Public functions
//---------------------------------------------------------------------------

extern AMP_T* _AMP_GetAmpTAS5717FunTbl(void);
extern void _TAS5717_Headphone_Mute_Setting(BOOL fgEnable);

#endif /* DIGIAMP_OTHERS_H */

