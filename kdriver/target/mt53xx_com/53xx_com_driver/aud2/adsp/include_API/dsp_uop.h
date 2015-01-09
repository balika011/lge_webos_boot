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
 * $RCSfile: dsp_uop.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file dsp_uop.h
 *  Brief of file dsp_uop.h.
 *  Details of file dsp_uop.h (optional).
 */

#ifndef DSP_UOP_H
#define DSP_UOP_H

/* PLEASE NOTE !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/
/*                                                                            */
/* Strict rules - NO replacement of share memory for backward compatible      */
/*                Until new version for architecture is established           */
/*                                                                            */
/* PLEASE NOTE !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/

#include "x_lint.h"
LINT_EXT_HEADER_BEGIN
#define UOP_DSP                                 0x00U

#define ADSPTASK_MSG_COMMAND                    0x00U
#define ADSPTASK_MSG_NO_COMMAND                 (ADSPTASK_MSG_COMMAND)
#define ADSPTASK_MSG_INTERRUPT                  (ADSPTASK_MSG_COMMAND+(0x01<<8))
#define ADSPTASK_MSG_POWER_ON                   (ADSPTASK_MSG_COMMAND+(0x02<<8))
#define ADSPTASK_MSG_POWER_OFF                  (ADSPTASK_MSG_COMMAND+(0x03<<8))
#define ADSPTASK_MSG_AOUT_ENABLE                (ADSPTASK_MSG_COMMAND+(0x04<<8))
#define ADSPTASK_MSG_AOUT_ENABLE_DEC2           (ADSPTASK_MSG_COMMAND+(0x05<<8))
#define ADSPTASK_MSG_UPDATE_EFFECT              (ADSPTASK_MSG_COMMAND+(0x06<<8))
#define ADSPTASK_MSG_INT_STOP                   (ADSPTASK_MSG_COMMAND+(0x07<<8))
#define ADSPTASK_MSG_INT_STOP_DEC2              (ADSPTASK_MSG_COMMAND+(0x08<<8))
#define ADSPTASK_MSG_INT_AOUT_ENABLE            (ADSPTASK_MSG_AOUT_ENABLE)
#define ADSPTASK_MSG_INT_AOUT_ENABLE_DEC2       (ADSPTASK_MSG_AOUT_ENABLE_DEC2)
#define ADSPTASK_MSG_INT_SAMPLE_RATE            (ADSPTASK_MSG_COMMAND+(0x09<<8))
#define ADSPTASK_MSG_INT_SAMPLE_RATE_DEC2       (ADSPTASK_MSG_COMMAND+(0x0A<<8))
#define ADSPTASK_MSG_INT_UPDATE_EFFECT          (ADSPTASK_MSG_UPDATE_EFFECT)
#define ADSPTASK_MSG_INT_MODE_CHANGE            (ADSPTASK_MSG_COMMAND+(0x0B<<8))
#define ADSPTASK_MSG_INT_MODE_CHANGE_DEC2       (ADSPTASK_MSG_COMMAND+(0x0C<<8))
#define ADSPTASK_MSG_INT_MTS_MODE               (ADSPTASK_MSG_COMMAND+(0x0D<<8))
#define ADSPTASK_MSG_INT_PAL_MODE               (ADSPTASK_MSG_COMMAND+(0x0E<<8))
#define ADSPTASK_MSG_INT_JPN_MODE               (ADSPTASK_MSG_COMMAND+(0x0F<<8))
#define ADSPTASK_MSG_INT_MINER_NOTIFY           (ADSPTASK_MSG_COMMAND+(0x10<<8))
#define ADSPTASK_MSG_INT_DETECTOR_NOTIFY        (ADSPTASK_MSG_COMMAND+(0x11<<8))
//#ifdef CC_MT5391_AUD_3_DECODER
#define ADSPTASK_MSG_INT_STOP_DEC3              (ADSPTASK_MSG_COMMAND+(0x12<<8))
#define ADSPTASK_MSG_INT_AOUT_ENABLE_DEC3       (ADSPTASK_MSG_COMMAND+(0x13<<8))
#define ADSPTASK_MSG_INT_SAMPLE_RATE_DEC3       (ADSPTASK_MSG_COMMAND+(0x14<<8))
#define ADSPTASK_MSG_INT_MODE_CHANGE_DEC3       (ADSPTASK_MSG_COMMAND+(0x15<<8))
//#endif
#define ADSPTASK_MSG_INT_ATV_CHANGE             (ADSPTASK_MSG_COMMAND+(0x16<<8))
#define ADSPTASK_MSG_INT_ATV_HDEV_SWITCH        (ADSPTASK_MSG_COMMAND+(0x17<<8))
#define ADSPTASK_MSG_INT_SAMPLING_RATE_CHANGE   (ADSPTASK_MSG_COMMAND+(0x18<<8))
#define ADSPTASK_MSG_INT_SAMPLING_RATE_CHANGE_DEC2  (ADSPTASK_MSG_COMMAND+(0x19<<8))
#define ADSPTASK_MSG_INT_FM_RADIO_DET           (ADSPTASK_MSG_COMMAND+(0x1A<<8))

#define ADSPTASK_MSG_INT_STOP_DEC4              (ADSPTASK_MSG_COMMAND+(0x1B<<8))
#define ADSPTASK_MSG_INT_AOUT_ENABLE_DEC4       (ADSPTASK_MSG_COMMAND+(0x1C<<8))
#define ADSPTASK_MSG_INT_SAMPLE_RATE_DEC4       (ADSPTASK_MSG_COMMAND+(0x1D<<8))
#define ADSPTASK_MSG_INT_MODE_CHANGE_DEC4       (ADSPTASK_MSG_COMMAND+(0x1E<<8))

#ifdef CC_AUD_EFFECT_MUTE_PROTECT
#define ADSPTASK_MSG_UPDATE_EFFECT_END          (ADSPTASK_MSG_COMMAND+(0x1F<<8))
#define ADSPTASK_MSG_INT_UPDATE_EFFECT_END      (ADSPTASK_MSG_UPDATE_EFFECT_END)
#endif

#define ADSPTASK_MSG_INTERRUPT_DATA             (ADSPTASK_MSG_COMMAND+(0x20<<8)) //ADSP Data Queue.

#define ADSPTASK_MSG_INT_DETECTOR_DEC2_NOTIFY        (ADSPTASK_MSG_COMMAND+(0x21<<8))
#define ADSPTASK_MSG_INT_DETECTOR_DEC3_NOTIFY        (ADSPTASK_MSG_COMMAND+(0x22<<8))
// Volume UOP
//Volume of first decoder
#define DSP_UOPID1                              (UOP_DSP + 0x01)
#define UOP_DSP_MASTER_VOLUME                   (DSP_UOPID1 + (0x01 << 8))
#define UOP_DSP_TRIM_C                          (DSP_UOPID1 + (0x02 << 8))
#define UOP_DSP_TRIM_L                          (DSP_UOPID1 + (0x03 << 8))
#define UOP_DSP_TRIM_R                          (DSP_UOPID1 + (0x04 << 8))
#define UOP_DSP_TRIM_LS                         (DSP_UOPID1 + (0x05 << 8))
#define UOP_DSP_TRIM_RS                         (DSP_UOPID1 + (0x06 << 8))
#define UOP_DSP_TRIM_CH7                        (DSP_UOPID1 + (0x07 << 8))
#define UOP_DSP_TRIM_CH8                        (DSP_UOPID1 + (0x08 << 8))
#define UOP_DSP_TRIM_SUBWOOFER                  (DSP_UOPID1 + (0x09 << 8))
#define UOP_DSP_TRIM_LFE                        (DSP_UOPID1 + (0x0A << 8))
#define UOP_DSP_DIALOGUE_GAIN                   (DSP_UOPID1 + (0x0B << 8))
#define UOP_DSP_RAW_MUTE                        (DSP_UOPID1 + (0x0C << 8))
#define UOP_DSP_RAW_UNMUTE                      (DSP_UOPID1 + (0x0D << 8))
#define UOP_DSP_TRIM_CH9                        (DSP_UOPID1 + (0x0E << 8))
#define UOP_DSP_TRIM_CH10                       (DSP_UOPID1 + (0x0F << 8))
#define UOP_DSP_INPUT_SRC_GAIN                  (DSP_UOPID1 + (0x10 << 8))
//Volume of second decoder
#define UOP_DSP_MASTER_VOLUME_DEC2              (DSP_UOPID1 + (0x11 << 8))
#define UOP_DSP_TRIM_L_DEC2                     (DSP_UOPID1 + (0x12 << 8))
#define UOP_DSP_TRIM_R_DEC2                     (DSP_UOPID1 + (0x13 << 8))
#define UOP_DSP_TRIM_LFE_DEC2                   (DSP_UOPID1 + (0x14 << 8))
#define UOP_DSP_DIALOGUE_GAIN_DEC2              (DSP_UOPID1 + (0x15 << 8))
#define UOP_DSP_RAW_MUTE_DEC2                   (DSP_UOPID1 + (0x16 << 8))
#define UOP_DSP_RAW_UNMUTE_DEC2                 (DSP_UOPID1 + (0x17 << 8))
#define UOP_DSP_INPUT_SRC_GAIN_DEC2             (DSP_UOPID1 + (0x18 << 8))
//Volume of thidd decoder
//#ifdef CC_MT5391_AUD_3_DECODER
#define UOP_DSP_MASTER_VOLUME_DEC3              (DSP_UOPID1+(0x19<<8))
#define UOP_DSP_INPUT_SRC_GAIN_DEC3             (DSP_UOPID1+(0x1A<<8))
#define UOP_DSP_TRIM_L_DEC3                     (DSP_UOPID1 + (0x1B << 8))
#define UOP_DSP_TRIM_R_DEC3                     (DSP_UOPID1 + (0x1C << 8))
#define UOP_DSP_AD_FADE                         (DSP_UOPID1 + (0x1D << 8))
#define UOP_DSP_PANFADE_DSP_ENABLE              (DSP_UOPID1 + (0x1E << 8))
//#endif
//#ifdef CC_AUD_4_DECODER_SUPPORT
#define UOP_DSP_MASTER_VOLUME_DEC4              (DSP_UOPID1 + (0x1F << 8))
#define UOP_DSP_INPUT_SRC_GAIN_DEC4             (DSP_UOPID1 + (0x20 << 8))
//#endif
#define UOP_DSP_BLUETOOTH_VOL                   (DSP_UOPID1 + (0x21 << 8))

// For multi volume curve
#define UOP_DSP_VOL_L                           (DSP_UOPID1 + (0x30 << 8))
#define UOP_DSP_VOL_R                           (DSP_UOPID1 + (0x31 << 8))

// Mixsound UOP
#define DSP_UOPID2                              (UOP_DSP + 0x02)
#define UOP_DSP_MIXSOUND_FLAG                   (DSP_UOPID2 + (0x01 << 8))
// Limiter UOP
#define UOP_DSP_LIMITER_CONFIG                  (DSP_UOPID2 + (0x02 << 8))
#define UOP_DSP_MIXSOUND_DEC3                  (DSP_UOPID2 + (0x03 << 8))

// Flow Control UOP
#define DSP_UOPID3                              (UOP_DSP + 0x03)
#define DSP_PLAY                                (DSP_UOPID3 + (0x01 << 8))
#define DSP_STOP                                (DSP_UOPID3 + (0x02 << 8))
#define DSP_FLUSH                               (DSP_UOPID3 + (0x03 << 8))
#define DSP_PAUSE                               (DSP_UOPID3 + (0x04 << 8))
#define DSP_RESUME                              (DSP_UOPID3 + (0x05 << 8))
#define DSP_RESET                               (DSP_UOPID3 + (0x06 << 8))
#define DSP_WAKEUP                              (DSP_UOPID3 + (0x07 << 8))
#define DSP_GETBITRATE                          (DSP_UOPID3 + (0x08 << 8))
#define DSP_GETMPEG_TYPE                        (DSP_UOPID3 + (0x09 << 8))
//Secondary decoder
#define DSP_PLAY_DEC2                           (DSP_UOPID3 + (0x11 << 8))
#define DSP_STOP_DEC2                           (DSP_UOPID3 + (0x12 << 8))
#define DSP_FLUSH_DEC2                          (DSP_UOPID3 + (0x13 << 8))
#define DSP_PAUSE_DEC2                          (DSP_UOPID3 + (0x14 << 8))
#define DSP_RESUME_DEC2                         (DSP_UOPID3 + (0x15 << 8))
#define DSP_RESET_DEC2                          (DSP_UOPID3 + (0x16 << 8))
      //reserved                                (DSP_UOPID3 + (0x17 << 8))
#define DSP_GETBITRATE_DEC2                     (DSP_UOPID3 + (0x18 << 8))
#define DSP_GETMPEG_TYPE_DEC2                   (DSP_UOPID3 + (0x19 << 8))

#define DSP_CLEAR_AOUT_WAIT                     (DSP_UOPID3 + (0x1B << 8))
#define DSP_CLEAR_AOUT_WAIT_DEC2                (DSP_UOPID3 + (0x1C << 8))
#define DSP_CLEAR_AOUT_WAIT_DEC3                (DSP_UOPID3 + (0x1D << 8)) //parson MM3


// Third decoder
//#ifdef CC_MT5391_AUD_3_DECODER              //for build error when not define 3 decoder.
#define DSP_PLAY_DEC3                           (DSP_UOPID3 + (0x21 << 8))
#define DSP_STOP_DEC3                           (DSP_UOPID3 + (0x22 << 8))
#define DSP_FLUSH_DEC3                          (DSP_UOPID3 + (0x23 << 8))
#define DSP_PAUSE_DEC3                          (DSP_UOPID3 + (0x24 << 8))
#define DSP_RESUME_DEC3                         (DSP_UOPID3 + (0x25 << 8))
#define DSP_RESET_DEC3                          (DSP_UOPID3 + (0x26 << 8))
#define DSP_GETBITRATE_DEC3                     (DSP_UOPID3 + (0x28 << 8))
#define DSP_GETMPEG_TYPE_DEC3                   (DSP_UOPID3 + (0x29 << 8))
//#endif

//#ifdef CC_AUD_4_DECODER_SUPPORT
#define DSP_PLAY_DEC4                           (DSP_UOPID3 + (0x31 << 8))
#define DSP_STOP_DEC4                           (DSP_UOPID3 + (0x32 << 8))
#define DSP_FLUSH_DEC4                          (DSP_UOPID3 + (0x33 << 8))
#define DSP_PAUSE_DEC4                          (DSP_UOPID3 + (0x34 << 8))
#define DSP_RESUME_DEC4                         (DSP_UOPID3 + (0x35 << 8))
#define DSP_RESET_DEC4                          (DSP_UOPID3 + (0x36 << 8))
#define DSP_GETBITRATE_DEC4                     (DSP_UOPID3 + (0x38 << 8))
#define DSP_GETMPEG_TYPE_DEC4                   (DSP_UOPID3 + (0x39 << 8))
#define DSP_RESET_READ_POINTER_DEC4             (DSP_UOPID3 + (0x3A << 8))
#define DSP_MP3ENC_HIGHSPEED_DEC4               (DSP_UOPID3 + (0x3B << 8))  ///MP3ENC_SUPPORT

//#endif

// Bass Management UOP
// Channel Configuration
#define DSP_UOPID4                              (UOP_DSP + 0x04)
#define UOP_DSP_CONFIG_SPEAKER                  (DSP_UOPID4 + (0x01 << 8))
#define UOP_DSP_CONFIG_DELAY_C                  (DSP_UOPID4 + (0x02 << 8))
#define UOP_DSP_CONFIG_DELAY_L                  (DSP_UOPID4 + (0x03 << 8))
#define UOP_DSP_CONFIG_DELAY_R                  (DSP_UOPID4 + (0x04 << 8))
#define UOP_DSP_CONFIG_DELAY_LS                 (DSP_UOPID4 + (0x05 << 8))
#define UOP_DSP_CONFIG_DELAY_RS                 (DSP_UOPID4 + (0x06 << 8))
#define UOP_DSP_CONFIG_DELAY_CH7                (DSP_UOPID4 + (0x07 << 8))
#define UOP_DSP_CONFIG_DELAY_CH8                (DSP_UOPID4 + (0x08 << 8))
#define UOP_DSP_CONFIG_DELAY_SUBWOOFER          (DSP_UOPID4 + (0x09 << 8))
#define UOP_DSP_CONFIG_DELAY_CH9                (DSP_UOPID4 + (0x0A << 8))
#define UOP_DSP_CONFIG_DELAY_CH10               (DSP_UOPID4 + (0x0B << 8))
#define UOP_DSP_CHANGE_SPK_SIZE                 (DSP_UOPID4 + (0x0C << 8))
#define UOP_DSP_CONFIG_DELAY_ALL                (DSP_UOPID4 + (0x0D << 8)) //Andrew Wen 2007/6/6 simplify all channel delay
// Channel configuration for second decoder
#define UOP_DSP_CONFIG_SPEAKER_DEC2             (DSP_UOPID4 + (0x11 << 8))
#define UOP_DSP_CONFIG_DELAY_C_DEC2             (DSP_UOPID4 + (0x12 << 8))
#define UOP_DSP_CONFIG_DELAY_L_DEC2             (DSP_UOPID4 + (0x13 << 8))
#define UOP_DSP_CONFIG_DELAY_R_DEC2             (DSP_UOPID4 + (0x14 << 8))
// Channel configuration for third decoder
#define UOP_DSP_CONFIG_SPEAKER_DEC3             (DSP_UOPID4 + (0x15 << 8))
#define UOP_DSP_CONFIG_DELAY_DEC3               (DSP_UOPID4 + (0x16 << 8))

// IEC, PTS, STC and Speed UOP
#define DSP_UOPID5                              (UOP_DSP + 0x05)
// first decoder
#define UOP_DSP_IEC_FLAG                        (DSP_UOPID5 + (0x01 << 8))
#define UOP_DSP_SPEED                           (DSP_UOPID5 + (0x02 << 8))
#define UOP_DSP_PROCESSING_MODE                 (DSP_UOPID5 + (0x03 << 8))
#define UOP_DSP_IEC_DOWN_SAMPLE                 (DSP_UOPID5 + (0x04 << 8))
#define UOP_DSP_IEC_FLAG_DOWNMIX                (DSP_UOPID5 + (0x05 << 8))
#define UOP_DSP_AOUT_REINIT                     (DSP_UOPID5 + (0x06 << 8))

// Light added for AVC
#define UOP_DSP_AVC_CONTROL                     (DSP_UOPID5 + (0x07 << 8))
//-----------------
//Andrew Wen add for CLI control bypass group in PROCESSING_MODE 061027
#define UOP_DSP_BYPASS_PROC_MODE                (DSP_UOPID5 + (0x08 << 8))
// ML added for downmix channel(CH9/10) position
#define UOP_DSP_DOWNMIX_POSITION                (DSP_UOPID5 + (0x09 << 8))
// sunman added for play speed
#define UOP_DSP_FRAC_SPEED                      (DSP_UOPID5 + (0x0a << 8))
#define UOP_DSP_IEC_SET_DATA                    (DSP_UOPID5 + (0x0b << 8))



// second decoder
#define UOP_DSP_IEC_FLAG_DEC2                   (DSP_UOPID5 + (0x11 << 8))
#define UOP_DSP_SPEED_DEC2                      (DSP_UOPID5 + (0x12 << 8))
#define UOP_DSP_PROCESSING_MODE_DEC2            (DSP_UOPID5 + (0x13 << 8))
#define UOP_DSP_IEC_DOWN_SAMPLE_DEC2            (DSP_UOPID5 + (0x14 << 8))
#define UOP_DSP_IEC_FLAG_DOWNMIX_DEC2           (DSP_UOPID5 + (0x15 << 8))
#define UOP_DSP_AOUT_REINIT_DEC2                (DSP_UOPID5 + (0x16 << 8))

// Light added for AVC
#define UOP_DSP_AVC_CONTROL_DEC2                (DSP_UOPID5 + (0x17 << 8))
//-----------------
#define UOP_DSP_BYPASS_PROC_MODE_DEC2           (DSP_UOPID5 + (0x18 << 8))
#define UOP_DSP_SET_STC_DIFF_BOUND              (DSP_UOPID5 + (0x19 << 8))
// third decoder
//#ifdef CC_MT5391_AUD_3_DECODER
#define UOP_DSP_IEC_FLAG_DEC3                   (DSP_UOPID5 + (0x21 << 8))
#define UOP_DSP_SPEED_DEC3                      (DSP_UOPID5 + (0x22 << 8))
#define UOP_DSP_PROCESSING_MODE_DEC3            (DSP_UOPID5 + (0x23 << 8))
#define UOP_DSP_IEC_DOWN_SAMPLE_DEC3            (DSP_UOPID5 + (0x24 << 8))
#define UOP_DSP_IEC_FLAG_DOWNMIX_DEC3           (DSP_UOPID5 + (0x25 << 8))
#define UOP_DSP_AOUT_REINIT_DEC3                (DSP_UOPID5 + (0x26 << 8))
#define UOP_DSP_BYPASS_PROC_MODE_DEC3           (DSP_UOPID5 + (0x28 << 8))
//#endif
//#ifdef CC_AUD_4_DECODER_SUPPORT
#define UOP_DSP_BYPASS_PROC_MODE_DEC4           (DSP_UOPID5 + (0x29 <<8))
//#endif

#define UOP_DSP_SET_STC_ID                      (DSP_UOPID5 + (0x30 <<8)) // 2_STC_Task
#define UOP_DSP_SET_STC_ID_DEC2                 (DSP_UOPID5 + (0x31 <<8))
#define UOP_DSP_SET_STC_ID_DEC3                 (DSP_UOPID5 + (0x32 <<8))
#define UOP_DSP_SET_STC_ID_DEC4                 (DSP_UOPID5 + (0x35 <<8))


// Pink Noise UOP
#define DSP_UOPID6                              (UOP_DSP + 0x06)
#define UOP_DSP_PINK_NOISE_ON                   (DSP_UOPID6 + (0x01 << 8))
#define UOP_DSP_PINK_NOISE_OFF                  (DSP_UOPID6 + (0x02 << 8))
#define UOP_DSP_PINK_NOISE_CHANGE               (DSP_UOPID6 + (0x03 << 8))
#define UOP_DSP_PINK_NOISE_PLAY                 (DSP_UOPID6 + (0x04 << 8))

// LR selection UOP
#define DSP_UOPID7                              (UOP_DSP + 0x07)
#define UOP_DSP_KARAOKE_FLAG                    (DSP_UOPID7 + (0x01 << 8))
#define UOP_DSP_LR_MIX_RATIO                    (DSP_UOPID7 + (0x02 << 8))
#ifdef CC_VOCAL_MUTE
#define UOP_DSP_KARAOKE_FLAG_NO_INT             (DSP_UOPID7 + (0x03 << 8))
#endif
#define UOP_DSP_KARAOKE_FLAG_DEC2               (DSP_UOPID7 + (0x11 << 8))
#define UOP_DSP_LR_MIX_RATIO_DEC2               (DSP_UOPID7 + (0x12 << 8))

// Equalizer UOP
#define DSP_UOPID8                              (UOP_DSP + 0x08)
#define UOP_DSP_EQUALIZER_FLAG                  (DSP_UOPID8 + (0x01 << 8))
#define UOP_DSP_EQUALIZER_CHANNEL_GAIN          (DSP_UOPID8 + (0x02 << 8))
#define UOP_DSP_EQUALIZER_CHANNEL_CHANGE        (DSP_UOPID8 + (0x03 << 8))
#define UOP_DSP_SUPER_BASS_DELAY                (DSP_UOPID8 + (0x04 << 8))
#define UOP_DSP_SUPER_BASS_BOOST_GAIN           (DSP_UOPID8 + (0x05 << 8))
#define UOP_DSP_SUPER_BASS_CLEAR_GAIN           (DSP_UOPID8 + (0x06 << 8))
#define UOP_DSP_CDNOTCH_FLAG                    (DSP_UOPID8 + (0x07 << 8))
#define UOP_DSP_CDNOTCH_CFG                     (DSP_UOPID8 + (0x08 << 8))
#define UOP_DSP_KEYSHIFT_FLAG                   (DSP_UOPID8 + (0x09 << 8))
#define UOP_DSP_KEYSHIFT_KEY                    (DSP_UOPID8 + (0x0A << 8))
#ifdef CC_AUD_FY12_LOUDNESS
#define UOP_DSP_FY12_LOUDNESS_FLAG                  (DSP_UOPID8 + (0x0B << 8))
#define UOP_DSP_FY12_LOUDNESS_BASS_ATTN_GAIN       (DSP_UOPID8 + (0x0C << 8))
#define UOP_DSP_FY12_LOUDNESS_TREBLE_ATTN_GAIN    (DSP_UOPID8 + (0x0D << 8))
#endif
// Secondary decoder
#define UOP_DSP_EQUALIZER_FLAG_DEC2             (DSP_UOPID8 + (0x11 << 8))
#define UOP_DSP_EQUALIZER_CHANNEL_GAIN_DEC2     (DSP_UOPID8 + (0x12 << 8))
#define UOP_DSP_EQUALIZER_CHANNEL_CHANGE_DEC2   (DSP_UOPID8 + (0x13 << 8))
#define UOP_DSP_SUPER_BASS_DELAY_DEC2           (DSP_UOPID8 + (0x14 << 8))
#define UOP_DSP_SUPER_BASS_BOOST_GAIN_DEC2      (DSP_UOPID8 + (0x15 << 8))
#define UOP_DSP_SUPER_BASS_CLEAR_GAIN_DEC2      (DSP_UOPID8 + (0x16 << 8))
#define UOP_DSP_CDNOTCH_FLAG_DEC2               (DSP_UOPID8 + (0x17 << 8))
#define UOP_DSP_CDNOTCH_CFG_DEC2                (DSP_UOPID8 + (0x18 << 8))
#define UOP_DSP_KEYSHIFT_FLAG_DEC2              (DSP_UOPID8 + (0x19 << 8))
#define UOP_DSP_KEYSHIFT_KEY_DEC2               (DSP_UOPID8 + (0x1A << 8))

// 3D Processing UOP
#define DSP_UOPID9                              (UOP_DSP + 0x09)
#define UOP_DSP_PRO_LOGICII_CONFIG              (DSP_UOPID9 + (0x01 << 8))
#define UOP_DSP_PRO_LOGICII_MODE                (DSP_UOPID9 + (0x02 << 8))
#define UOP_DSP_VIRTUAL_SURROUND_FLAG           (DSP_UOPID9 + (0x03 << 8))
#define UOP_DSP_VIRTUAL_SURROUND_GAIN           (DSP_UOPID9 + (0x04 << 8))
#define UOP_DSP_VIRTUAL_SURROUND_WIDE           (DSP_UOPID9 + (0x05 << 8))
#define UOP_DSP_VIRTUAL_SURROUND_DELAY          (DSP_UOPID9 + (0x06 << 8))
#define UOP_DSP_REVERB_FLAG                     (DSP_UOPID9 + (0x07 << 8))
#define UOP_DSP_REVERB_GAIN                     (DSP_UOPID9 + (0x08 << 8))
// SRS
#define UOP_DSP_SRS_WOW_FLAG			        (DSP_UOPID9 + (0x09 << 8))
#define UOP_DSP_SRS_TSXT_FLAG		            (DSP_UOPID9 + (0x0A << 8))
#define UOP_DSP_BBE_MODE                        (DSP_UOPID9 + (0x0B << 8))
#define UOP_DSP_NEW_MTK_VSURR                   (DSP_UOPID9 + (0x0C << 8))
#define UOP_DSP_CV                              (DSP_UOPID9 + (0x0D << 8))

// Sencodary decoder
#define UOP_DSP_VIRTUAL_SURROUND_FLAG_DEC2      (DSP_UOPID9 + (0x13 << 8))
#define UOP_DSP_VIRTUAL_SURROUND_GAIN_DEC2      (DSP_UOPID9 + (0x14 << 8))
#define UOP_DSP_VIRTUAL_SURROUND_WIDE_DEC2      (DSP_UOPID9 + (0x15 << 8))
#define UOP_DSP_VIRTUAL_SURROUND_DELAY_DEC2     (DSP_UOPID9 + (0x16 << 8))
#define UOP_DSP_REVERB_FLAG_DEC2                (DSP_UOPID9 + (0x17 << 8))
#define UOP_DSP_REVERB_GAIN_DEC2                (DSP_UOPID9 + (0x18 << 8))

#define UOP_DSP_AD_FLAG                         (DSP_UOPID9 + (0x19 << 8))  //For Audio Description
#define UOP_DSP_SPKCAL_FLAG                     (DSP_UOPID9 + (0x1A << 8))
#define UOP_DSP_AUX_MIX_MAIN                    (DSP_UOPID9 + (0x1B << 8))  //For Skype Application
#define UOP_DSP_FIR_CHANGE                      (DSP_UOPID9 + (0x1C << 8))

// Decoding option UOP
#define DSP_UOPIDA                              (UOP_DSP + 0x0A)
//DOLBY AC-3 configuration for first decoder
#define UOP_DSP_AC3_KARAOKE_MODE                (DSP_UOPIDA + (0x01 << 8))
#define UOP_DSP_AC3_DUAL_MONO_MODE              (DSP_UOPIDA + (0x02 << 8))
#define UOP_DSP_AC3_COMPRESSION_MODE            (DSP_UOPIDA + (0x03 << 8))
#define UOP_DSP_AC3_DYNAMIC_LOW                 (DSP_UOPIDA + (0x04 << 8))
#define UOP_DSP_AC3_DYNAMIC_HIGH                (DSP_UOPIDA + (0x05 << 8))
#define UOP_DSP_AC3_AC3AUTODNMIX                (DSP_UOPIDA + (0x06 << 8))

// ATV related UOPIDs are released!!!!

#define UOP_DSP_PCM_INPUT_CH_ASSIGNMENT         (DSP_UOPIDA + (0x0E << 8))
#define UOP_DSP_PCM_INPUT_CHANNEL_NUM           (DSP_UOPIDA + (0x0F << 8))
//DOLBY AC-3 configuration for secodn decoder
#define UOP_DSP_AC3_KARAOKE_MODE_DEC2           (DSP_UOPIDA + (0x11 << 8))
#define UOP_DSP_AC3_DUAL_MONO_MODE_DEC2         (DSP_UOPIDA + (0x12 << 8))
#define UOP_DSP_AC3_COMPRESSION_MODE_DEC2       (DSP_UOPIDA + (0x13 << 8))
#define UOP_DSP_AC3_DYNAMIC_LOW_DEC2            (DSP_UOPIDA + (0x14 << 8))
#define UOP_DSP_AC3_DYNAMIC_HIGH_DEC2           (DSP_UOPIDA + (0x15 << 8))
#define UOP_DSP_AC3_AC3AUTODNMIX_DEC2           (DSP_UOPIDA + (0x16 << 8))
//DOLBY AC-3 configuration for third decoder
#define UOP_DSP_AC3_KARAOKE_MODE_DEC3           (DSP_UOPIDA + (0x17 << 8))
#define UOP_DSP_AC3_DUAL_MONO_MODE_DEC3         (DSP_UOPIDA + (0x18 << 8))
#define UOP_DSP_AC3_COMPRESSION_MODE_DEC3       (DSP_UOPIDA + (0x19 << 8))
#define UOP_DSP_AC3_DYNAMIC_LOW_DEC3            (DSP_UOPIDA + (0x1A << 8))
#define UOP_DSP_AC3_DYNAMIC_HIGH_DEC3           (DSP_UOPIDA + (0x1B << 8))
#define UOP_DSP_AC3_AC3AUTODNMIX_DEC3           (DSP_UOPIDA + (0x1C << 8))

// ATV related UOPIDs are released!!!!

#define UOP_DSP_PCM_INPUT_CH_ASSIGNMENT_DEC2    (DSP_UOPIDA + (0x1E << 8))
#define UOP_DSP_PCM_INPUT_CHANNEL_NUM_DEC2      (DSP_UOPIDA + (0x1F << 8))
// G726 decoder
#define UOP_DSP_G726_CFG                        (DSP_UOPIDA + (0x20 << 8))
#define UOP_DSP_G726_CFG_DEC2                   (DSP_UOPIDA + (0x21 << 8))
#define UOP_DSP_G726_CFG_DEC3                   (DSP_UOPIDA + (0x22 << 8))

// third decoder
//#ifdef CC_MT5391_AUD_3_DECODER
#define UOP_DSP_PCM_INPUT_CH_ASSIGNMENT_DEC3    (DSP_UOPIDA + (0x2E << 8))
#define UOP_DSP_PCM_INPUT_CHANNEL_NUM_DEC3      (DSP_UOPIDA + (0x2F << 8))
//#endif
#ifdef CC_AUD_SUPPORT_MS10
#define UOP_DSP_DDC_CONTROL                 (DSP_UOPIDA + (0x30 << 8))
#define UOP_DSP_DDC_STRMID                 (DSP_UOPIDA + (0x31 << 8))
#define UOP_DSP_DDC_ASSOC_MIX           (DSP_UOPIDA + (0x32 << 8))
#define UOP_DSP_DDC_CONTROL_DEC2                 (DSP_UOPIDA + (0x33 << 8))
#define UOP_DSP_DDC_STRMID_DEC2                 (DSP_UOPIDA + (0x34 << 8))
#define UOP_DSP_DDC_ASSOC_MIX_DEC2           (DSP_UOPIDA + (0x35 << 8))

#define UOP_DSP_DDT_DUAL_MODE                   (DSP_UOPIDA + (0x36 << 8))
#define UOP_DSP_DDT_COMPVAL                    (DSP_UOPIDA + (0x37 << 8))
#define UOP_DSP_DDT_COMPMODE                    (DSP_UOPIDA + (0x38 << 8))
#define UOP_DSP_DDT_DUAL_MODE_DEC2              (DSP_UOPIDA + (0x39 << 8))
#define UOP_DSP_DDT_COMPVAL_DEC2               (DSP_UOPIDA + (0x3A << 8))
#define UOP_DSP_DDT_COMPMODE_DEC2               (DSP_UOPIDA + (0x3B << 8))
#define UOP_DSP_DDT_DUAL_MODE_DEC3              (DSP_UOPIDA + (0x3C << 8))
#define UOP_DSP_DDT_COMPVAL_DEC3               (DSP_UOPIDA + (0x3D << 8))
#define UOP_DSP_DDT_COMPMODE_DEC3               (DSP_UOPIDA + (0x3E << 8))
#endif

// Decoding option UOP
#define DSP_UOPIDB                              (UOP_DSP + 0x0B)

// ATV related UOPIDs are released!!!!

#if 1///def CC_VORBIS_SUPPORT ///def DSP_VORBIS_SUPPORT
// vorbis
#define UOP_DSP_MAKE_VORBIS_TAB                 (DSP_UOPIDB + (0x02 << 8))
#define UOP_DSP_MAKE_VORBIS_TAB_DEC2            (DSP_UOPIDB + (0x12 << 8))
#define UOP_DSP_MAKE_VORBIS_TAB_DEC3            (DSP_UOPIDB + (0x22 << 8))
#endif

// Post Processing UOP
#define DSP_UOPIDC                              (UOP_DSP + 0x0C)
#define UOP_DSP_HPF_ENABLE                      (DSP_UOPIDC + (0x01 << 8))
#define UOP_DSP_HPF_FC                          (DSP_UOPIDC + (0x02 << 8))

#define UOP_DSP_CLIPPER_ENABLE                  (DSP_UOPIDC + (0x06 << 8))
#define UOP_DSP_CLIPPER_VAL                     (DSP_UOPIDC + (0x07 << 8))
#define UOP_DSP_CLIPPER_CONFIG_2                (DSP_UOPIDC + (0x08 << 8))

#define UOP_DSP_PEQ_ENABLE                      (DSP_UOPIDC + (0x0A << 8))
#define UOP_DSP_PEQ_CONFIG                      (DSP_UOPIDC + (0x0B << 8))

#define UOP_DSP_VIR_BASS_ENABLE                 (DSP_UOPIDC + (0x0C << 8))
#define UOP_DSP_VIR_BASS_CONFIG                 (DSP_UOPIDC + (0x0D << 8))


#define UOP_DSP_DDCO_ENABLE                     (DSP_UOPIDC + (0x0E << 8))
#define UOP_DSP_DDCO_AGC_ENABLE                 (DSP_UOPIDC + (0x0F << 8))
#define UOP_DSP_DDCO_LFE_LPF_ENABLE             (DSP_UOPIDC + (0x10 << 8))

#define UOP_DSP_AOUT_REINIT_FOR_SAMPLING_RATE_CHANGE    (DSP_UOPIDC + (0x11 << 8))
#define UOP_DSP_AOUT_REINIT_FOR_SAMPLING_RATE_CHANGE_DEC2    (DSP_UOPIDC + (0x12 << 8))

#define UOP_DSP_DUAL_MONO_MODE                  (DSP_UOPIDC + (0x13 << 8))
#define UOP_DSP_DUAL_MONO_MODE_DEC2             (DSP_UOPIDC + (0x14 << 8))

#ifdef CC_AUD_SPEAKER_HEIGHT
#define UOP_DSP_SPH_ENABLE                      (DSP_UOPIDC + (0x15 << 8))
#ifdef CC_AUD_SPEAKER_HEIGHT_MODE
#define UOP_DSP_SPH_CHANGE_MODE                 (DSP_UOPIDC + (0x16 << 8))
#endif
#endif
#ifdef CC_AUD_DSP_SUPPORT_AUDYSSEY
#define UOP_DSP_ADV_ENABLE                      (DSP_UOPIDC + (0x17 << 8))
#define UOP_DSP_ADV_SET_chCalbGain              (DSP_UOPIDC + (0x18 << 8))
#define UOP_DSP_ABX_FLAG                        (DSP_UOPIDC + (0x19 << 8))
#define UOP_DSP_AEQ_FLAG                        (DSP_UOPIDC + (0x1A << 8))
#endif
#ifdef CC_AUD_DATA_UPLOAD_SUPPORT
#define UOP_DSP_UPLOAD_DATA_ENABLE              (DSP_UOPIDC + (0x1B << 8))
#endif
#if defined(DSP_SUPPORT_SRS_SSHD) || defined(DSP_SUPPORT_SRS_PURESOUND) || defined(DSP_SUPPORT_SRS_SS3D)
#define UOP_DSP_SRS_FLAG                        (DSP_UOPIDC + (0x1C << 8))
#endif

//zy_dts
#define UOP_DSP_DTS_ENCODER_AMODE	            (DSP_UOPIDC + (0x1D << 8))
#define UOP_DSP_DTS_ENCODER_ENABLE	            (DSP_UOPIDC + (0x1E << 8))
#define UOP_DSP_IS_WITH_VIDEO					(DSP_UOPIDC + (0x1F << 8))
#define UOP_DSP_IS_WITH_VIDEO_DEC2  			(DSP_UOPIDC + (0x20 << 8))

#ifdef CC_AUD_FIR_SUPPORT
#define UOP_DSP_FIR_ENABLE                      (DSP_UOPIDC + (0x21 << 8))
#endif
#ifdef CC_AUD_DRC_V20
#define UOP_DSP_DRC_CONTROL                      (DSP_UOPIDC + (0x22 << 8))
#endif
#define UOP_DSP_BLUETOOTH_DATA_ENABLE           (DSP_UOPIDC + (0x23 << 8))

#if defined(DSP_SUPPORT_TCL_VBASS)
#define UOP_DSP_TCL_VBASS                        (DSP_UOPIDC + (0x24 << 8))
#endif

// ATV, detector, Fm radio related
#define DSP_UOPIDD                              (UOP_DSP + 0x0D)
#ifdef DSP_SUPPORT_NPTV
//NPTV Demodulation Related
// Decoder 1
#define UOP_DSP_TTXACC_CONFIG                   (DSP_UOPIDD + (0x01 << 8))
#define UOP_DSP_DEMOD_CONFIG                    (DSP_UOPIDD + (0x02 << 8))
#define UOP_DSP_DETECTOR_CONFIG                 (DSP_UOPIDD + (0x03 << 8))
#define UOP_DSP_DETECTION_CONFIG                (DSP_UOPIDD + (0x04 << 8))
#define UOP_DSP_CANON_PLUS                      (DSP_UOPIDD + (0x05 << 8))
#define UOP_DSP_AM_MUTE_CONFIG                  (DSP_UOPIDD + (0x06 << 8))
#define UOP_DSP_A2_DETECTOR_THRES               (DSP_UOPIDD + (0x07 << 8))
#define UOP_DSP_DEMOD_FINE_TUNE_VOLUME          (DSP_UOPIDD + (0x08 << 8))
// Decoder 2
#define UOP_DSP_TTXACC_CONFIG_DEC2              (DSP_UOPIDD + (0x11 << 8))
#define UOP_DSP_DEMOD_CONFIG_DEC2               (DSP_UOPIDD + (0x12 << 8))
#define UOP_DSP_DETECTOR_CONFIG_DEC2            (DSP_UOPIDD + (0x13 << 8))
#define UOP_DSP_DETECTION_CONFIG_DEC2           (DSP_UOPIDD + (0x14 << 8))
#define UOP_DSP_CANON_PLUS_DEC2                 (DSP_UOPIDD + (0x15 << 8))
#define UOP_DSP_AM_MUTE_CONFIG_DEC2             (DSP_UOPIDD + (0x16 << 8))
#define UOP_DSP_A2_DETECTOR_THRES_DEC2          (DSP_UOPIDD + (0x17 << 8))
#define UOP_DSP_DEMOD_FINE_TUNE_VOLUME_DEC2     (DSP_UOPIDD + (0x18 << 8))
#endif

#define DSP_UOPIDE                              (UOP_DSP + 0x0E)
#ifdef CC_AUD_ARM_SUPPORT
#define UOP_DSP_APROC_POSTPROC_VOL              (DSP_UOPIDE + (0x01 << 8))
#endif

//General Purpose
//#define DSP_UOPIDF                              (UOP_DSP + 0x0F)
LINT_EXT_HEADER_END

#endif //DSP_UOP_H

