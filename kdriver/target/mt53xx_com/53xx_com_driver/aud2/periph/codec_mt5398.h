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
 * $RCSfile: codec_mt5398.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file codec_mt5398.h
 *  Brief of file codec_mt5398.h. \n
 */

#ifndef CODEC_MT5398_H
#define CODEC_MT5398_H

//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------

#include "x_typedef.h"
#include "aud_if.h"
#include "drvcust_if.h"

//---------------------------------------------------------------------------
// Configurations
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Constant definitions
//---------------------------------------------------------------------------

// -+-+- Memory Address Map -+-+-
#define REG_INDIRECT_G1_ADDR 		0x800
#define REG_INDIRECT_G1_DATA 		0x804
#define REG_INDIRECT_G2_ADDR 		0x840
#define REG_INDIRECT_G2_DATA 		0x844
#define REG_INDIRECT_G3_ADDR 		0x880
#define REG_INDIRECT_G3_DATA 		0x884
#define REG_INDIRECT_G4_ADDR 		0x8C0
#define REG_INDIRECT_G4_DATA 		0x8C4


#define REG_PWMCFG0_PG1             0x1C8
#define REG_PWMCFG0_PG2             0x1CC
#define REG_PWMCFG0_PG3             0x1E0
  #define SWAP_LR                        (0x01<< 23) // This bit is used to swap Left/Right channel before pwmIP uses it
  #define SWAP_LR_NO                     (0x00<< 23) // This bit is used to swap Left/Right channel before pwmIP uses it
  #define UPDATE_NUM_0                   (0x00<< 16) // The timing for latching audio data input
  #define UPDATE_NUM_1                   (0x01<< 16) // The timing for latching audio data input
  #define UPDATE_NUM_2                   (0x02<< 16) // The timing for latching audio data input
  #define GHRESETZ                       (0x01<< 15) // Softreset bit (active low) for PWM G1/G2
  #define WRITE_DMAP_DATA_EN             (0x01<< 12) // Feed audio sample data by ADSP
  #define PWM_ADAC_MUTE                  (0x01<< 8)
  #define ADAC_SWAP_LR                   (0x01<< 7)  // input data swapping
  #define SIGEN                          (0x01<<2)   // PWM internal signal generate
  #define CR_MODE                        (0x01<< 1)  // Clock Ratio Mode in top (test only), PWM controller assumes that LJCLK = DCLK
  #define LINE_IN_BYPASS                 (0x01<< 0)  // input data from line-in mode

#define RREG_PWMIP_ABIST_CH                 0x1D0
///51D4,51D8 no use for 5398, change to 51EC REG_PWMIP_PWMPAD_CFG.
///#define REG_PWMIP_PINMUX_PG1                0x1D4
///#define REG_PWMIP_PINMUX_PG2                0x1D8
#define REG_PWMIP_MON_PG1                   0x1DC

///Design add test mode, To Do after IC back.
#define REG_PWMIP_GETIDA_PG1                0x1E4
#define REG_PWMIP_GETIDD_PG1                0x1E8

#define REG_PWMIP_PWMPAD_CFG                0x1EC
#define REG_PWMIP_PWMPAD_CFG2               0x248
    #if defined(CC_MT5399) || defined(CC_MT5890) || defined(CC_MT5882)                   
    #define PWM_PAD_INIT    (PINMUX_AL0 | PINMUX_AR0 | PINMUX_AL1)  /// 0xf00051ec = 0x00543210
    #define PWM_PAD_INIT2   (PINMUX_AR1 | PINMUX_AL2 | PINMUX_AR2)  /// 0xf0005248 = 0x00BA9876

    #define PINMUX_AL0     ((PWMPG1_CH0<<AL0_N)|(PWMPG1_CH1<<AL0_P))
    #define PINMUX_AR0     ((PWMPG1_CH2<<AR0_N)|(PWMPG1_CH3<<AR0_P))
    #define PINMUX_AL1     ((PWMPG2_CH0<<AL1_N)|(PWMPG2_CH1<<AL1_P))
    #define PINMUX_AR1     ((PWMPG2_CH2<<AR1_N)|(PWMPG2_CH3<<AR1_P))
    #define PINMUX_AL2     ((PWMPG3_CH0<<AL2_N)|(PWMPG3_CH1<<AL2_P))
    #define PINMUX_AR2     ((PWMPG3_CH2<<AR2_N)|(PWMPG3_CH3<<AR2_P))

    /// PINMUX Shifter
    #define AL0_N             (0)
    #define AL0_P             (4)
    #define AR0_N             (8)
    #define AR0_P             (12)
    #define AL1_N             (16)
    #define AL1_P             (20)

    #define AR1_N             (0)
    #define AR1_P             (4)
    #define AL2_N             (8)
    #define AL2_P             (12)
    #define AR2_N             (16)
    #define AR2_P             (20)
    /// PWM Channel
    /// PWMG1 DAC select by 0x5204[2:0]
    #define PWMPG1_CH0          (0)
    #define PWMPG1_CH1          (1)
    #define PWMPG1_CH2          (2)
    #define PWMPG1_CH3          (3)
    /// PWMG2 DAC select by 0x5204[5:3]
    #define PWMPG2_CH0          (4)
    #define PWMPG2_CH1          (5)
    #define PWMPG2_CH2          (6)
    #define PWMPG2_CH3          (7)
    /// PWMG2 DAC select by 0x5204[8:6]
    #define PWMPG3_CH0          (8)
    #define PWMPG3_CH1          (9)
    #define PWMPG3_CH2          (0xA)
    #define PWMPG3_CH3          (0xB)
    
    #elif defined(CC_MT5398)
    #define PWM_PAD_INIT (AL0_G3_CH0|AR0_G3_CH1|AL1_G1_CH0|AR1_G1_CH1|AL2_G2_CH0|AR2_G2_CH1)  // when 0x51ec = 0x321054, change to avoid influnce to HDMI port0
    #elif defined(CC_MT5880)
    #define PWM_PAD_INIT (AL0_G1_CH0|AR0_G1_CH1|AL1_G2_CH0|AR1_G2_CH1|AL2_G3_CH0|AR2_G3_CH1)  // 0x51ec = 0x543210
    #elif defined(CC_MT5881)    // 0x51ec = 0x76543210
    #define PWM_PAD_INIT_ES (PINMUX_AL0_ES | PINMUX_AR0_ES \
                          | PINMUX_AL1_ES | PINMUX_AR1_ES)
                          
    #define PWM_PAD_INIT (PINMUX_AL0 | PINMUX_AR0 \
                          | PINMUX_AL1 | PINMUX_AR1)
    #define PINMUX_AL0_ES     ((PWMPG1_CH0<<AL0_N)|(PWMPG2_CH1<<AL0_P))
    #define PINMUX_AR0_ES     ((PWMPG1_CH2<<AR0_N)|(PWMPG2_CH3<<AR0_P))
    #define PINMUX_AL1_ES     ((PWMPG1_CH0<<AL1_N)|(PWMPG2_CH1<<AL1_P))
    #define PINMUX_AR1_ES     ((PWMPG1_CH2<<AR1_N)|(PWMPG2_CH3<<AR1_P))
    
    #define PINMUX_AL0     ((PWMPG1_CH0<<AL0_N)|(PWMPG1_CH1<<AL0_P))
    #define PINMUX_AR0     ((PWMPG1_CH2<<AR0_N)|(PWMPG1_CH3<<AR0_P))
    #define PINMUX_AL1     ((PWMPG2_CH0<<AL1_N)|(PWMPG2_CH1<<AL1_P))
    #define PINMUX_AR1     ((PWMPG2_CH2<<AR1_N)|(PWMPG2_CH3<<AR1_P))

    /// PINMUX Shifter
    #define AL0_N             (0)
    #define AL0_P             (4)
    #define AR0_N             (8)
    #define AR0_P             (12)
    #define AL1_N             (16)
    #define AL1_P             (20)
    #define AR1_N             (24)
    #define AR1_P             (28)
    /// PWM Channel
    /// PWMG1 DAC select by 0x5204[2:0]
    #define PWMPG1_CH0          (0)
    #define PWMPG1_CH1          (1)
    #define PWMPG1_CH2          (2)
    #define PWMPG1_CH3          (3)
    /// PWMG2 DAC select by 0x5204[5:3]
    #define PWMPG2_CH0          (4)
    #define PWMPG2_CH1          (5)
    #define PWMPG2_CH2          (6)
    #define PWMPG2_CH3          (7)
    #endif

    /// AL0 AR0
    #define AL0_G1_CH0      (0<<0)
    #define AL0_G1_CH1      (1<<0)
    #define AL0_G2_CH0      (2<<0)
    #define AL0_G2_CH1      (3<<0)
    #define AL0_G3_CH0      (4<<0)
    #define AL0_G3_CH1      (5<<0)

    #define AR0_G1_CH0      (0<<4)
    #define AR0_G1_CH1      (1<<4)
    #define AR0_G2_CH0      (2<<4)
    #define AR0_G2_CH1      (3<<4)
    #define AR0_G3_CH0      (4<<4)
    #define AR0_G3_CH1      (5<<4)

    /// AL1 AR1
    #define AL1_G1_CH0      (0<<8)
    #define AL1_G1_CH1      (1<<8)
    #define AL1_G2_CH0      (2<<8)
    #define AL1_G2_CH1      (3<<8)
    #define AL1_G3_CH0      (4<<8)
    #define AL1_G3_CH1      (5<<8)

    #define AR1_G1_CH0      (0<<12)
    #define AR1_G1_CH1      (1<<12)
    #define AR1_G2_CH0      (2<<12)
    #define AR1_G2_CH1      (3<<12)
    #define AR1_G3_CH0      (4<<12)
    #define AR1_G3_CH1      (5<<12)

    /// AL2 AR2
    #define AL2_G1_CH0      (0<<16)
    #define AL2_G1_CH1      (1<<16)
    #define AL2_G2_CH0      (2<<16)
    #define AL2_G2_CH1      (3<<16)
    #define AL2_G3_CH0      (4<<16)
    #define AL2_G3_CH1      (5<<16)

    #define AR2_G1_CH0      (0<<20)
    #define AR2_G1_CH1      (1<<20)
    #define AR2_G2_CH0      (2<<20)
    #define AR2_G2_CH1      (3<<20)
    #define AR2_G3_CH0      (4<<20)
    #define AR2_G3_CH1      (5<<20)

///Fade-In/Out Register
#define REG_PWMIP_MISC0_PG1                 0x1F4 // Ramping for G1
#define REG_PWMIP_MISC0_PG2                 0x1F8 // Ramping for G2
#define REG_PWMIP_MISC0_PG3                 0x1F0 // Ramping for G3
  #define RAMP_SPEED_MIN                        ((UINT32)(0x3F))<<26  // Suggested
  #define RAMP_SPEED_MAX                        (0x01)<<26  // Fastest, not suggested
  #define RAMP_STEP_MIN                         (0x01<<8)  // Suggested (smallest)
  #define RAMP_EN_ALL                           (0x3F<<0)  // Enable all cheannels ramp mode
  #define RAMP_DIS_ALL                          (0x00<<0)  // Enable all cheannels ramp mode
  #define RAMP_EN_CH0                           (0x01<<0)  // Enable  cheannel 0 ramp mode
  #define RAMP_EN_CH1                           (0x01<<1)  // Enable  cheannel 0 ramp mode
  #define RAMP_EN_CH2                           (0x01<<2)  // Enable  cheannel 0 ramp mode
  #define RAMP_EN_CH3                           (0x01<<3)  // Enable  cheannel 0 ramp mode
  #define RAMP_EN_CH4                           (0x01<<4)  // Enable  cheannel 0 ramp mode
  #define RAMP_EN_CH5                           (0x01<<5)   // Enable  cheannel 0 ramp mode

#define REG_PWMIP_MISC1                   0x1FC
  #define PWMIP_RAMP_OFF                         0x00003fff
  #define PWMIP_RAMP_ON                          0x00000fff
  #define ONEHEART_ON                            0x80000000 //[31] oneheart for g1&g4
  #define RAMP_UP_AUTO_CLOSE                     (0x01<<13)  
  #define RAMP_UP_EN                             (0x01<<12)
#if defined (CC_MT5881) || defined(CC_MT5399) || defined(CC_MT5890) || defined(CC_MT5882)
  #define PWM_DIFF_PAIR_GP1                      (0x01<<29)
  #define PWM_DIFF_PAIR_GP2                      (0x01<<28)
  #define PWM_DIFF_PAIR_GP3                      (0x01<<27)
#endif

///PWM IP Domain
#define A_P11PIN    0xF1B
#define A_P10PIN    0xF1A
#define A_P9PIN 	0xF19
#define A_P8PIN 	0xF18
#define A_P7PIN 	0xF17
#define A_P6PIN 	0xF16
#define A_P5PIN 	0xF15
#define A_P4PIN 	0xF14

#define A_P1CTRL 	0xF0F
#define A_P0CTRL 	0xF0E

#define A_P3PIN 	0xF0D
#define A_P2PIN 	0xF0C
#define A_P1PIN 	0xF0B
#define A_P0PIN 	0xF0A

#define A_PCDATA 	0xF09
#define A_PCADDR 	0xF08
    #define CH0_GAIN    0x115   /// ALx, bit8=1 for YRAM
    #define CH1_GAIN    0x1D6   /// ARx, bit8=1 for YRAM
#define A_PGDR 		0xF07
#define A_PSTAT 	0xF06
#define A_PIC 		0xF05
	#define DACCIE 	(0x01<<0)
#define A_POE 		0xF04
    #define POZ_5   (0x01<<21)
    #define POZ_4   (0x01<<20)
	#define P5EN 	(0x01<<17)
	#define P4EN 	(0x01<<16)
    #define POZ_3   (0x01<<13)
    #define POZ_2   (0x01<<12)
	#define P3EN 	(0x01<<9)
	#define P2EN 	(0x01<<8)
    /// Cobra/Viper only 2 channels/group
    #define POZ_1   (0x01<<5)       ///set pwmdac output pure duty 50%
    #define POZ_0   (0x01<<4)
	#define P1EN 	(0x01<<1)
	#define P0EN 	(0x01<<0)


#define A_PGCTRL1 	0xF02
    #define C_PGCTRL1   (IDS_3LSB | CLIP_4)
    /// CLIPMODE[23:22]
    #define CLIP_OFF        (0x00<<22)
    #define CLIP_1          (0x01<<22)
    #define CLIP_4          (0x02<<22)
    #define CLIP_8          (0x03<<22)
    /// IDS[21:20]
    #define IDS_DIS         (0x00<<20)
    #define IDS_1LSB        (0x01<<20)
    #define IDS_2LSB        (0x02<<20)
    #define IDS_3LSB        (0x03<<20)
    /// LEVELS[18:8] 0~2048
    #define NUMLEVELS_1125  (0x464<<8)
    #define NUMLEVELS_1000  (0x3E7<<8)
    #define NUMLEVELS_900   (0x383<<8)
    #define NUMLEVELS_750 	(0x2ED<<8)
    #define NUMLEVELS_768 	(0x2FF<<8)
    #define NUMLEVELS_576 	(0x23F<<8)
    #define NUMLEVELS_384	(0x17F<<8)
    #define NUMLEVELS_300   (0x12B<<8)
    #define NUMLEVELS_256   (0x0FF<<8)
    /// INTPBR[5:4]
    #define INTPBR_1X	(0<<4)
    #define INTPBR_2X	(1<<4)
    #define INTPBR_3X	(2<<4)
    #define INTPBR_4X	(3<<4)
    /// INTPFR[3:0]: 1X~16X
    #define INTPFR_1X	(0<<0)
    #define INTPFR_2X	(1<<0)
    #define INTPFR_3X	(2<<0)
    #define INTPFR_4X	(3<<0)
    #define INTPFR_5X   (4<<0)
    #define INTPFR_6X   (5<<0)
    #define INTPFR_8X   (7<<0)
    #define INTPFR_10X  (9<<0)
#define A_PGCTRL0 	0xF01
    #define C_PGCTRL0       (PEN0|PEN1|QEPS|NSDITH|NAI_EN|ADITH_EN|QLEV|ROUND|INF)      /// C_PGCTRL0       0xF542FE
    #define C_PGCTRL0_PEN   (PEN|PEN0|PEN1|QEPS|NSDITH|NAI_EN|ADITH_EN|QLEV|ROUND|INF)  /// C_PGCTRL0_PEN   0xF542FF

    #define PEN             (1<<0)      ///PWM Group Total Switch
    #define PEN0            (1<<1)
    #define PEN1            (1<<2)
    /// QEPS[5:3]
    #define QEPS            (7<<3)
    /// NSDITH[7:6]
    #define NSDITH          (3<<6)
    /// PTM[8]
    #define PTM_EN          (1<<8)  ///Test Mode
    /// NAI[9]
    #define NAI_EN          (1<<9)  ///Enable PCADDR Not-Auto-Increment
    #define NAI_DIS         (0<<9)  ///Enable PCADDR Auto-Increment
    /// ADITH[10]
    #define ADITH_EN        (0<<10)
    /// QLEV[14:12]
    #define QLEV            (4<<12)
    /// ROUNDMODE[17:16]
    #define ROUND           (1<<16)
    #define INF             (1<<18) //input formate
    #define PEN2            (1<<20)
    #define PEN3            (1<<21)
    #define PEN4            (1<<22)
    #define PEN5            (1<<23

#define A_PDATA 	0xF00

#ifdef CC_MT5398
//PG1~3
#define C_P3PIN0 	0x000500
#define C_P2PIN0 	0x000604    //R
#define C_P1PIN0 	0x000500
#define C_P0PIN0 	0x000600    //L
#elif defined(CC_MT5880)
//PG1~3
#define C_P3PIN0 	0x000500             
#define C_P2PIN0 	0x000600    //R
#define C_P1PIN0 	0x000500
#define C_P0PIN0 	0x000600    // L       change from 0x500 to 0x600, because the bit will cause reverse (180 phase) for per channel

//PG4
#define C_P7PIN0 	0x000600
#define C_P6PIN0 	0x000500    //R
#define C_P5PIN0 	0x000600
#define C_P4PIN0 	0x000500    // L

#elif defined(CC_MT5881) || defined(CC_MT5399) || defined(CC_MT5890) || defined(CC_MT5882)
///(IS_IC_5881_ES(), def REMY_VER_A_DAC_WORKAROUND
///Set Both PWM GP1 and GP2 as PMOD: F0A[11:8] = 4'd5, F0C[11:8] = 4'd5.
#define C_P3PIN0_ES1 	0x000000    ///0xF0D             
#define C_P2PIN0_ES1 	0x000500    ///0xF0C
#define C_P1PIN0_ES1 	0x000000    ///0xF0B
#define C_P0PIN0_ES1 	0x000500    ///0xF0A
///(IS_IC_5881_ES2()), ECO IC
///1 PWMGP do different output, F0A must = F0B, F0C must = F0D
#define C_P3PIN0 	    0x000600             
#define C_P2PIN0 	    0x000600
#define C_P1PIN0 	    0x000600
#define C_P0PIN0 	    0x000600
    #ifdef CC_ALR0_ALR1_PS90
#define C_P3PIN0_PS90   0x000613
#define C_P2PIN0_PS90   0x000613
#define C_P1PIN0_PS90   0x000613
#define C_P0PIN0_PS90   0x000613
    #endif
#endif


//////  ADC related   ///////////
#if defined(CC_MT5399) 
///294.912MHz/2/12 -> 12.288MHz
#define ADC_GAIN           0x20F58000
#define ADC_GAIN_N         0x230A8000
#define ADC_GAIN_5399_ECO           0x20F10000
#define ADC_GAIN_N_5399_ECO         0x230f0000
#elif defined CC_MT5890
#define ADC_GAIN          0x20EB0000
#define ADC_GAIN_N        0x23150000
#define ADC_GAIN_5861           0x20E30000
#define ADC_GAIN_N_5861         0x231D0000
#define ADC_GAIN_5890_MPS1           0x20DE0000
#define ADC_GAIN_N_5890_MPS1         0x23220000
#define ADC_GAIN_5890_ES3       0x20E30000
#define ADC_GAIN_N_5890_ES3     0x231D0000
#elif defined CC_MT5882
#define ADC_GAIN           0x20F58000
#define ADC_GAIN_N         0x230A8000
#elif defined CC_MT5398
#define ADC_GAIN           0x20F58000
#define ADC_GAIN_N         0x230A8000
#elif defined CC_MT5880
#define ADC_GAIN           0x20EE0000
#define ADC_GAIN_N         0x23120000
///Python 5860 Viper 5880 ECO - 0x60000[10:8] from 4(-12dB) to 5(-10.5dB).
#define ADC_GAIN_5860_5880_ECO           0x20DD0000
#define ADC_GAIN_N_5860_5880_ECO         0x23230000
#elif defined(CC_MT5881)
///For ADC_VMID_REF_BG 0xf0060000[30]=1 and ADC self VSS 0xf0060004[13]=0 case
#define ADC_GAIN           0x20ED0000
#define ADC_GAIN_N         0x23130000
#else
#define ADC_GAIN           0x20F58000
#define ADC_GAIN_N         0x230A8000
#endif

///////// PWM constant definitions////////
/// 0xD244[3:0], 0xD248[3:0]
#define APLL_D1             1
#define APLL2_D1            4
#define SAWLESSPLL_D1       7

#ifdef CC_AUD_PWM_SAWLESS_PLL_EN
    #if defined(CC_MT5881) || defined(CC_MT5399) || defined(CC_MT5882) || defined(CC_MT5890)
        #define PWMPAD_CFG_INIT         0x003F003F      ///AL0[0],AR0[1],AL1[2],AR1[3] 
        #define PWMPAD_CFG_INIT1         0x00000000      ///AL0[0],AR0[1],AL1[2],AR1[3] 
    #else
    #define PWMPAD_CFG_INIT         0x00FC00FC      ///Kevin Suggest, Sawless + Clock Relatch . because there is only 6ch, not 8ch
    #endif

/// use YRAM_4X, 4*2*48 = 384K
/// PWMDAC divider must even, 1125 is odd for 432MHz, so can't use it.
///#define PGCTRL1_INIT_384        (C_PGCTRL1 | NUMLEVELS_1125 | INTPBR_2X | INTPFR_4X)
/// use YRAM_3X, 3*3*48 = 432K
#define PGCTRL1_INIT_432        (C_PGCTRL1 | NUMLEVELS_1000 | INTPBR_3X | INTPFR_3X)
/// use YRAM_5X, 5*2*48 = 480K
#define PGCTRL1_INIT_480        (C_PGCTRL1 | NUMLEVELS_900 | INTPBR_2X | INTPFR_5X)
/// use YRAM 4X, 4*3*48 = 576K
#define PGCTRL1_INIT_576        (C_PGCTRL1 | NUMLEVELS_750 | INTPBR_3X | INTPFR_4X)     /// use YRAM_4X ///PWM Reg 0xF02, Bigfox suggest
/// use YRAM 10X,10*3*48 = 1440K
#define PGCTRL1_INIT_1440       (C_PGCTRL1 | NUMLEVELS_300 | INTPBR_3X | INTPFR_10X)    /// use YRAM_10X ///PWM Reg 0xF02, Bigfox suggest

/// use YRAM 8X,8*3*48 = 1152K
#define PGCTRL1_INIT_1152       (C_PGCTRL1 | NUMLEVELS_256 | INTPBR_3X | INTPFR_8X)    /// use YRAM_8X ///PWM Reg 0xF02, Benzhai suggest

/// use YRAM 5X,5*3*96 = 1440K
#define PGCTRL1_INIT_1440_96K   (C_PGCTRL1 | NUMLEVELS_300 | INTPBR_3X | INTPFR_5X)     /// CC_SUPPORT_96K_AOUT
#define PWMIP_MON_PG1_INIT      (0x3FF << 16)   /// 0x51DC
#define PWM_CK_SEL              SAWLESSPLL_D1   ///0xD220[3:0] & 0xD224[3:0] &0xD23c[3:0]
#define PWM_CK_SEL_APLL              APLL_D1         ///0xD220[3:0] & 0xD224[3:0] &0xD23c[3:0]
///Reduce pwm digital gain for external gain 2.11X (for internal LDO use)
    #ifdef CC_MT5398
#define PWM_DIGITAL_INTERNALLDO_3X_GAIN         0x0B0000  // for g1_432k/g2_576k       ///PWM YRAM 0x115, 0x1D6
#define PWM_DIGITAL_INTERNALLDO_3X_GAIN_N       0xF50000  // if (up-sample = 3x),then G_3x = ~ G_n3x * 8/9.  to make output at the same level
#define PWM_DIGITAL_INTERNALLDO_N3X_GAIN        0x0C8000  // for g3_480k
#define PWM_DIGITAL_INTERNALLDO_N3X_GAIN_N      0xF38000

#define PWM_DIGITAL_3X_GAIN        0x0C8000        ///PWM YRAM 0x115, 0x1D6
#define PWM_DIGITAL_3X_GAIN_N      0xF38000
#define PWM_DIGITAL_N3X_GAIN       0x0D8000        ///PWM YRAM 0x115, 0x1D6
#define PWM_DIGITAL_N3X_GAIN_N     0xF28000
//temp for MT5399, 20130122 
#define PWM_DIGITAL_10X_GAIN        0x0B8000        ///PWM YRAM 0x115, 0x1D6
#define PWM_DIGITAL_10X_GAIN_N      0xF48000
    #elif defined (CC_MT5880)
    #ifndef CC_MT5565
#define PWM_DIGITAL_INTERNALLDO_3X_GAIN         0x0A8000 //0x0B0000  // for g1_432k/g2_576k       ///PWM YRAM 0x115, 0x1D6
#define PWM_DIGITAL_INTERNALLDO_3X_GAIN_N       0xF58000 //0xF50000  // if (up-sample = 3x),then G_3x = ~ G_n3x * 8/9.  to make output at the same level
    #else
#define PWM_DIGITAL_INTERNALLDO_3X_GAIN         0x0C0000 //Viper-E M1V1 SCART1 OPA enlargement factor is 51K/20K, increase the gain
#define PWM_DIGITAL_INTERNALLDO_3X_GAIN_N       0xF40000 
    #endif
#define PWM_DIGITAL_INTERNALLDO_N3X_GAIN        0x0C0000 //0x0C8000  // for g3_480k
#define PWM_DIGITAL_INTERNALLDO_N3X_GAIN_N      0xF40000 //0xF38000

#define PWM_DIGITAL_INTERNALLDO_HP_GAIN         0x0D4000 //for analog HP output, Viper_LRTP_180 & Python is lower, so need to increase the gain
#define PWM_DIGITAL_INTERNALLDO_HP_GAIN_N       0xF2C000
   
#define PWM_DIGITAL_3X_GAIN        0x0C8000        ///PWM YRAM 0x115, 0x1D6
#define PWM_DIGITAL_3X_GAIN_N      0xF38000
#define PWM_DIGITAL_N3X_GAIN       0x0D8000        ///PWM YRAM 0x115, 0x1D6
#define PWM_DIGITAL_N3X_GAIN_N     0xF28000
    #elif defined(CC_MT5881) || defined(CC_MT5399) || defined(CC_MT5890) || defined(CC_MT5882) /// PWM output clock = 1440KHz
///for PWMGP1/GP2/GP3
#define PWM_DIGITAL_10X_GAIN        0x0B8000        ///PWM YRAM 0x115, 0x1D6
#define PWM_DIGITAL_10X_GAIN_N      0xF48000
    #endif

#else
#define PWMPAD_CFG_INIT         0x0000FF        ///APLL1 + Sel APLL + APLL non-inverse + Clock Relatch
#define PGCTRL1_INIT            (C_PGCTRL1 | NUMLEVELS_768 | INTPBR_2X | INTPFR_4X) ///Use
#define PWMIP_MON_PG1_INIT      (0x3FF << 16)   /// 0x51DC
#define PWM_CK_SEL              APLL_D1         ///0xD244[3:0]
#define PWM_DIGITAL_GAIN        0x0F0600        ///PWM YRAM 0x115, 0x1D6
#define PWM_DIGITAL_GAIN_N      0xF0FA00
#endif

///////// GPIO constant definition for AIN0~AIN6////////
#if 0 ///defined(CC_MT5396)
#define GPIO_AIN0_L 170
#define GPIO_AIN0_R 169
#define GPIO_AIN1_L 172
#define GPIO_AIN1_R 171
#define GPIO_AIN2_L 174
#define GPIO_AIN2_R 173
#define GPIO_AIN3_L 176
#define GPIO_AIN3_R 175
#define GPIO_AIN4_L 178
#define GPIO_AIN4_R 177
#define GPIO_AIN5_L 180
#define GPIO_AIN5_R 179
#define GPIO_AIN6_L 182
#define GPIO_AIN6_R 181
#endif

//---------------------------------------------------------------------------
// Macro definitions
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
// Public functions
//---------------------------------------------------------------------------
extern CODEC_T* _CODEC_GetMT5398Tbl(void);

void _MT5398_Init(void);
void _MT5398_Mute(UINT8 u1DacId,BOOL fgMute);
void _MT5398_MuteALRx(UINT8 u1ALRx,BOOL fgMute);
void _MT5398_InterPWMEnable(BOOL fgEnable);
void _MT5398_InterPWMGainSet(INT8 pgroup,INT32 pgain);
UINT32 _MT5398_InterPWMGainGet(INT8 pgroup);
void _MT5398_AdcFmtCfg(DATA_FORMAT_T eFormat, MCLK_FREQUENCY_T eMclk);
void _MT5398_DacFmtCfg(DATA_FORMAT_T eFormat, MCLK_FREQUENCY_T eMclk);
void _MT5398_ChlSel(UINT8 u1Indx);
extern void _MT5398_PWM_SetSamplingRate(UINT8 u1ClkFmt);
extern void _MT5398_ADCDOWN(BOOL fgEnalbe);
#ifdef CC_SUPPORT_STR
extern void _MT5398_DACDOWN(BOOL fgEnable);
#endif
extern void _MT5398_MuteCLK(UINT8 u1DacId,BOOL fgMute);
void _MT5398_DacChlSel(UINT8 u1DacId,AUD_CHL_DEF_T eChl);
void _MT5398_DacChlSelQuery(void);

#endif /* CODEC_MT5398_H */


