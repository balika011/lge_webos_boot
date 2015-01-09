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
 * $RCSfile: codec_NE3201.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file codec_NE3201.h
 *  Brief of file codec_NE3201.h. \n
 */

#ifndef CODEC_NE3201_H
#define CODEC_NE3201_H

//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------

#include "x_typedef.h"
#include "aud_if.h"

//---------------------------------------------------------------------------
// Constant definitions
//---------------------------------------------------------------------------

#ifdef CC_AUD_AMP_IN_I2C_BUS_2
#define AUD_AMP_BUS_ID          2
#elif defined (CC_AUD_AMP_IN_I2C_BUS_1)
#define AUD_AMP_BUS_ID          1
#else
#define AUD_AMP_BUS_ID          0
#endif
#define NE3201_TERNARY_MODE

#define NE3201_ADDR             0x36
#define SIF_CLK_DIV             0x100

#define NE3201_REG_LEN          48  //NE3201 total register length
/* NE3201 register definition */
#define NE3201_CLK_CTL          0x00
#define NE3201_ERR_STATUS       0x02
#define NE3201_SYS_CTL          0x03
#define NE3201_SDATAIF          0x04
    #define SD_FMT_MASK        (0x03 << 2)
    #define FMT_I2S             0x00
    #define FMT_RJ              0x04
    #define FMT_LJ              0x08
#define NE3201_RESVD_05         0x05
#define NE3201_MUTE_CTL         0x06
#define NE3201_MASTER_VOL       0x07
#define NE3201_RESVD_08         0x08
#define NE3201_RESVD_09         0x09
#define NE3201_RESVD_0A         0x0A
#define NE3201_RESVD_0B         0x0B
#define NE3201_RESVD_0C         0x0C
#define NE3201_RESVD_0D         0x0D
#define NE3201_RESVD_0E         0x0E
#define NE3201_RESVD_0F         0x0F
#define NE3201_MOD_LIMIT        0x10
#define NE3201_RESVD_11         0x11
#define NE3201_RESVD_12         0x12
#define NE3201_RESVD_13         0x13
#define NE3201_RESVD_14         0x14
#define NE3201_RESVD_15         0x15
#define NE3201_RESVD_16         0x16
#define NE3201_RESVD_17         0x17
#define NE3201_RESVD_18         0x18
#define NE3201_RESVD_19         0x19
#define NE3201_CLR_RAM          0x1A
#define NE3201_PROTECTION_MODE  0x1B
#define NE3201_WARN_MODE        0x1C
#define NE3201_DSP_CTL          0x1D
#define NE3201_RAMADDR0         0x1E
#define NE3201_RAMADDR1         0x1F
#define NE3201_DATA             0x20
#define NE3201_CH1A_CFG         0x21
#define NE3201_CH1B_CFG         0x22
#define NE3201_CH2A_CFG         0x23
#define NE3201_CH2B_CFG         0x24
#define NE3201_RESVD_25         0x25
#define NE3201_RESVD_26         0x26
#define NE3201_RESVD_27         0x27
#define NE3201_RESVD_28         0x28
#define NE3201_RESVD_2B         0x2B
#define NE3201_RESVD_2C         0x2C
#define NE3201_PROG_RAM         0x2D
#define NE3201_I2C_PREAMBLE     0x2E
#define NE3201_I2C_ADDR_SEL     0x2F

/* Register bit definitions*/
/*===Reg00===*/
#define Input_Rate_Mask 0x1F
#define Set_Input_Rate_32K 0x20
#define Set_Input_Rate_441K 0x40
#define Set_Input_Rate_48K 0x60
#define Set_Input_Rate_882K 0x80
#define Set_Input_Rate_96K 0xA0
#define Set_Input_Rate_176K 0xC0
#define Set_Input_Rate_192K 0xE0
#define MCLK_Ratio_Mask 0xE3
#define Set_MCLK_Ratio_64 0x0
#define Set_MCLK_Ratio_128 0x4
#define Set_MCLK_Ratio_192 0x8
#define Set_MCLK_Ratio_256 0xC
#define Set_MCLK_Ratio_320 0x10
#define Set_MCLK_Ratio_384 0x14
#define Set_MCLK_Ratio_448 0x18
#define Set_MCLK_Ratio_512 0x1C
/*===Reg02===*/
#define MCLK_ERR 0x80
#define PLL_ERR 0x40
#define SCLK_ERR 0x20
#define LRCK_ERR 0x10
#define OTW_ERR 0x8
#define OTP_ERR 0x4
#define OCW_ERR 0x2
#define OCP_ERR 0x1
/*===Reg03===*/
#define DC_HPF_Mask 0x7F
#define DC_HPF_EN 0x80
#define DC_HPF_DIS 0x00
#define SHUTDOWNALL_Mask 0xBF
#define SHUTDOWN_ALL 0x40
#define UNSHUTDOWM_ALL 0x00
/*===Reg04===*/
#define LRCK_Mask 0x7F
#define LRCK_0_LCH 0x0
#define LRCK_1_LCH 0x80
#define DSPCHSWAP_Mask 0x9F
#define DSP_NotCHSWAP 0x0
#define DSP_CHSWAP 0x20
#define SCLK_Latch_Mask 0xEF
#define SCLK_Latch_RSEDGE 0x0
#define SCLK_Latch_FALLEDGE 0x10
//#define SD_FMT_Mask 0xF3
//#define FMT_I2S 0x00
//#define FMT_RJ 0x4
//#define FMT_LJ 0x8
#define SD_WL_Mask 0xFC
#define WL_16B 0x0
#define WL_18B 0x1
#define WL_20B 0x2
#define WL_24B 0x3
/*===Reg06===*/
#define PULL_DC_Mask 0x7F
#define PULL_DC_UP 0x0
#define PULL_DC_DOWN 0x80
#define AutoMute_MCLKERR_Mask 0xBF
#define AutoMute_MCLKERR 0x0
#define NotAutoMute_MCLKERR 0x40
#define SoftMute_Mask 0xF7
#define SoftUnmuteAll 0x0
#define SoftMuteAll 0x8
/*===Reg10===*/
#define ForceZeroEn 0x8
#define ForceZeroDis 0xF7
/*===Reg1A===*/
#define ClearRAM_Mask 0xDF
#define Clear_RAM 0x20
#define Clear_RAM_Done 0x00
/*===Reg1B===*/
#define PMOSState_At_FAULT_Mask 0x7F
#define NotTriStatePMOS_At_FAULT 0x80
#define TriStatePMOS_At_FAULT 0x00
#define AutoRecovery_AT_IC_SHDN_Mask 0xBF
#define NotAutoRecovery_AT_IC_SHDN 0x40
#define AutoRecovery_AT_IC_SHDN 0x00
#define Clear_Fault_Flag 0xDF
#define Set_AutoRecoveryTime_Mask 0xF0
#define Set_AutoRecoveryTime_0MS 0x0
#define Set_AutoRecoveryTime_170MS 0x1
#define Set_AutoRecoveryTime_340MS 0x2
#define Set_AutoRecoveryTime_510MS 0x3
#define Set_AutoRecoveryTime_680MS 0x4
#define Set_AutoRecoveryTime_850MS 0x5
#define Set_AutoRecoveryTime_1020MS 0x6
#define Set_AutoRecoveryTime_1190MS 0x7
#define Set_AutoRecoveryTime_1360MS 0x8
#define Set_AutoRecoveryTime_1530MS 0x9
#define Set_AutoRecoveryTime_1700MS 0xA
#define Set_AutoRecoveryTime_1870MS 0xB
#define Set_AutoRecoveryTime_2040MS 0xC
#define Set_AutoRecoveryTime_2210MS 0xD
#define Set_AutoRecoveryTime_2380MS 0xE
#define Set_AutoRecoveryTime_2550MS 0xF
/*===Reg1C===*/
#define OTW_AutoRecovery_Mask 0x7F
#define OTW_AutoRecovery_EN 0x00
#define OTW_AutoRecovery_DIS 0x80
#define OTW_ADJ_Mask 0xBF
#define OTW_ADJ_EN 0x00
#define OTW_ADJ_DIS 0x40
#define OCW_AutoRecovery_Mask 0xDF
#define OCW_AutoRecovery_EN 0x00
#define OCW_AutoRecovery_DIS 0x20
#define OCW_ADJ_Mask 0xEF
#define OCW_ADJ_EN 0x00
#define OCW_ADJ_DIS 0x10
/*===Reg1D===*/
#define DSP_Run_Mask 0xFD
#define DSP_Run 0x0
#define DSP_Stop 0x2
/*===Reg21~24===*/
#define OUT1A_OutputState_Mask 0x3F
#define OUT1A_Output_Tristate 0x00
#define OUT1A_Output_Force_On 0x40
#define OUT1A_Default 0xC0
#define OUT1A_PWM_Mask 0xCF
#define OUT1A_PWM_Ternary 0x0
#define OUT1A_PWM_Lowemi 0x10
#define OUT1A_OUT_SRC_Mask 0xF0
#define OUT1A_OUT_SRC_LA 0x2
#define OUT1A_OUT_SRC_LB 0x3
#define OUT1A_OUT_SRC_RA 0x4
#define OUT1A_OUT_SRC_RB 0x5
#define OUT1B_OutputState_Mask 0x3F
#define OUT1B_Output_Tristate 0x00
#define OUT1B_Output_Force_On 0x40
#define OUT1B_Default 0xC0
#define OUT1B_PWM_Mask 0xCF
#define OUT1B_PWM_Ternary 0x0
#define OUT1B_PWM_Lowemi 0x10
#define OUT1B_OUT_SRC_Mask 0xF0
#define OUT1B_OUT_SRC_LA 0x2
#define OUT1B_OUT_SRC_LB 0x3
#define OUT1B_OUT_SRC_RA 0x4
#define OUT1B_OUT_SRC_RB 0x5
#define OUT2A_OutputState_Mask 0x3F
#define OUT2A_Output_Tristate 0x00
#define OUT2A_Output_Force_On 0x40
#define OUT2A_Default 0xC0
#define OUT2A_PWM_Mask 0xCF
#define OUT2A_PWM_Ternary 0x0
#define OUT2A_PWM_Lowemi 0x10
#define OUT2A_OUT_SRC_Mask 0xF0
#define OUT2A_OUT_SRC_LA 0x2
#define OUT2A_OUT_SRC_LB 0x3
#define OUT2A_OUT_SRC_RA 0x4
#define OUT2A_OUT_SRC_RB 0x5
#define OUT2B_OutputState_Mask 0x3F
#define OUT2B_Output_Tristate 0x00
#define OUT2B_Output_Force_On 0x40
#define OUT2B_Default 0xC0
#define OUT2B_PWM_Mask 0xCF
#define OUT2B_PWM_Ternary 0x0
#define OUT2B_PWM_Lowemi 0x10
#define OUT2B_OUT_SRC_Mask 0xF0
#define OUT2B_OUT_SRC_LA 0x2
#define OUT2B_OUT_SRC_LB 0x3
#define OUT2B_OUT_SRC_RA 0x4
#define OUT2B_OUT_SRC_RB 0x5
/*===Reg2D===*/
#define RAM_Mode_Mask 0xFE
#define RAM_Mode_On 0x1
#define RAM_Mode_OFF 0x0
/*===Reg2E===*/
#define I2C_Preamble 0x5A


#define NE3201_CRAM_LEN 256
#define TOTAL_RAM_CNT  (NE3201_CRAM_LEN * 4)

//---------------------------------------------------------------------------
// Public functions
//---------------------------------------------------------------------------

extern AMP_T* _AMP_GetAmpNE3201FunTbl(void);

#endif /* CODEC_NE3201_H */

