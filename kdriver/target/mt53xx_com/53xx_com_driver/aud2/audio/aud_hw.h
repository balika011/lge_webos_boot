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
 * $RCSfile: aud_hw.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file aud_hw.h
 *  Define the audio environment registers and register access macro
 */

#ifndef _AUD_HW_H_
#define _AUD_HW_H_

//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------

#include "x_typedef.h"
#include "x_bim.h"

//---------------------------------------------------------------------------
// Constant definitions
//---------------------------------------------------------------------------

#define MCLK0                   (0)
#define MCLK1                   (1)

#define REG_ACK_CFG             0x0000
    #define BCK_DIV_MASK            (0x0f << 0)
      #define BCK_DIV_8               (0x4 << 0)      //512fs/2*8
      #define BCK_DIV_6               (0x3 << 0)      //384fs/2*6
      #define BCK_DIV_4               (0x2 << 0)      //256fs/2*4     default=32fs
      #define BCK_DIV_2               (0x1 << 0)      //128fs/2*2
    #define DEC2_BCK_DIV_MASK       (0xf << 16)
      #define DEC2_BCK_DIV_8          (0x4 << 16)     //512fs/2*8
      #define DEC2_BCK_DIV_6          (0x3 << 16)     //384fs/2*6
      #define DEC2_BCK_DIV_4          (0x2 << 16)     //256fs/2*4     default=32fs
      #define DEC2_BCK_DIV_2          (0x1 << 16)     //128fs/2*2
    #define IEC_DIV_MASK            (0x7 << 4)
      #define IEC_DIV_4               (0x4 << 4)
      #define IEC_DIV_2               (0x2 << 4)
      #define IEC_DIV_1               (0x1 << 4)
    #define DEC2_IEC_DIV_MASK       (0x7 << 20)
      #define DEC2_IEC_DIV_4          (0x4 << 20)
      #define DEC2_IEC_DIV_2          (0x2 << 20)
      #define DEC2_IEC_DIV_1          (0x1 << 20)
    #define DEC0_MTRI               (0x1 << 8)
    #define DEC1_MTRI               (0x1 << 24)
    #define LIN_MS                  (0x1 <<  9)
    #define LIN3_MS                 (0x1 << 14)     /// CC_AUD_LINE_IN3
    #define LIN2_MS                 (0x1 << 25)
    #define APLL_SEL_MASK           (0x04000400 << 0)
    #define CLK_SEL_APLL0           (0x0 << 10)
    #define CLK_SEL_APLL1           (0x1 << 10)
    #define DEC2_CLK_SEL_APLL0      (0x0 << 26)
    #define DEC2_CLK_SEL_APLL1      (0x1 << 26)
    #define MPHONE_SEL              (0x1 << 11)
    #define DEC2_MPHONE_SEL         (0x1 << 27)

#define REG_ACK2_CFG            0x0004
    #define AUD3_ACK_BCK_DIV_MASK   (0x0f << 0)
      #define AUD3_ACK_BCK_DIV_8      (0x4 << 0)
      #define AUD3_ACK_BCK_DIV_6      (0x3 << 0)
      #define AUD3_ACK_BCK_DIV_4      (0x2 << 0)
      #define AUD3_ACK_BCK_DIV_2      (0x1 << 0)
    #define AUD3_LRCK_CYC_SEL_MASK  (0x3 << 4)
      #define AUD3_LRCK_CYC16         (0x0 << 4)
      #define AUD3_LRCK_CYC24         (0x1 << 4)
      #define AUD3_LRCK_CYC32         (0x2 << 4)
    #define SDADC_AUD3_CLK_SEL      (0x1 << 8)
    #define SDADC_AUD3_LRCK_SEL     (0x1 << 9)
    #define LIN_CLK_AUD3_SEL        (0x1 << 10)
    #define DEC2_LIN_CLK_AUD3_SEL   (0x1 << 11)
    #define IEC_AUD3_CLK_SEL        (0x1 << 12)
    #define DEC3_LIN_CLK_AUD3_SEL   (0x1 << 13) /// CC_AUD_LINE_IN3
    #define AOMCLK_DEC_SEL          (0x1 << 22)
    #define AOMCLK2_DEC_SEL         (0x1 << 23)
    #define APLL1_REF_CLK_SEL_MASK  (0x0f << 24)
      #define APLL1_EXT_MPBCK_IN      (0x08 << 24)
      #define APLL1_EXT_HDMI_BCK      (0x09 << 24)
      #define APLL1_EXT_AUD_CLK       (0x0a << 24)
      #define APLL1_INT_HDMI_MCLK     (0x0c << 24)
      #define APLL1_HDMI_BCK          (0x00 << 24)
    #define APLL2_REF_CLK_SEL_MASK  (0x0f << 28)
      #define APLL2_EXT_MPBCK_IN      (0x08 << 28)
      #define APLL2_EXT_HDMI_BCK      (0x09 << 28)
      #define APLL2_EXT_AUD2_CLK      (0x0a << 28)
      #define APLL2_INT_HDMI_MCLK     (0x0c << 28)
      #define APLL2_HDMI_BCK          (0x00 << 28)

#define REG_EXTDAC_CFG0         0x0008
#define REG_EXTDAC_CFG1         0x000c
    #define AUD_OUT_CFG_MASK        (0x3fff)
    #define LRCK_CYC_MASK           (0x3 << 0)
      #define LRCK_CYC16              (0x0 << 0)      // 16 cycles
      #define LRCK_CYC24              (0x1 << 0)      // 24 cycles
      #define LRCK_CYC32              (0x2 << 0)      // 32 cycles
    #define AOFMT_MASK              (0x3 << 2)
      #define AOFMT_RJ                (0x0 << 2)      // Right aligned with LRCK
      #define AOFMT_LJ                (0x2 << 2)      // Left aligned with LRCK
      #define AOFMT_I2S               (0x3 << 2)      // I2S interface
    #define INV_BCK                 (0x1 << 4)      // Invert bit clock
    #define INV_LRCK                (0x1 << 5)      // Invert LRCK
    #define INV_SDATA               (0x1 << 6)      // Invert SDATA
    #define DAC_BNUM_MASK           (0x3F << 8)     // Audio DAC bit number
      #define DAC_16BIT               (0x10 << 8)     // Audio DAC 16-Bit
      #define DAC_18BIT               (0x12 << 8)     // Audio DAC 18-Bit
      #define DAC_20BIT               (0x14 << 8)     // Audio DAC 20-Bit
      #define DAC_24BIT               (0x18 << 8)     // Audio DAC 24-Bit

#define REG_EXTADC_CFG          0x0010
    #define AUD_IN_CFG_MASK         (0x1fff)
    #define SPDIF_CYC_MASK          (0x3 << 0)      // mask of cycles
      #define SPDIF_CYC16             (0x0 << 0)      // 16 cycles
      #define SPDIF_CYC24             (0x1 << 0)      // 24 cycles
      #define SPDIF_CYC32             (0x2 << 0)      // 32 cycles
    #define SPLIN_FMT_MASK          (0x3 << 2)
      #define SPLIN_RJ                (0x0 << 2)      // Right aligned with LRCK
      #define SPLIN_LJ                (0x2 << 2)      // Left aligned with LRCK
      #define SPLIN_I2S               (0x3 << 2)      // I2S interface
    #define SPLIN_BCK_INVT          (0x1 << 4)      // Inver LR
    #define SPLIN_LR_INVT           (0x1 << 5)      // Inver LR
    #define SPLIN_BNUM_MASK         (0x1f << 8)     // (number of bits - 1) for line in ADC or SPDIF
      #define ADC_16BIT               (0xf << 8)      // ADC 16-Bit
      #define ADC_18BIT               (0x11 << 8)     // ADC 18-Bit
      #define ADC_20BIT               (0x13 << 8)     // ADC 20-Bit
      #define ADC_24BIT               (0x17 << 8)     // ADC 24-Bit

#define REG_EXTADC_CFG2         0x0014

#define REG_LIN_CFG0            0x0018
    #define SPDIF_FMT_MASK          (0x8fe << 0)
    #define SPDIF_LINE_IN_ENABLE    (0x1 << 0)      // Enable SPDIF/Line-In buffering data to DRAM
    #define SPDIF_LINE_IN_DISABLE   (0x0 << 0)      // Disable SPDIF/Line-In buffering data to DRAM
    #define DATA_RAW_16_BITS        (0x0 << 1)      // Store 16 bits data per sample, RAW data
    #define DATA_PCM_24_BITS        (0x1 << 1)      // Store 24 bits data per sample, PCM data
    #define DATA_LSB_8_BITS         (0x1 << 2)      // LSB 8 bits nonzero
    #define DATA_LSB_SYNC           (0x0 << 2)      //PCM data desided when sync pattern not found
    #define DATA_SWAP               (0x1 << 3)      // Swap data
    #define DATA_NO_SWAP            (0x0 << 3)      // No swap data
    #define PCM_CH_NUM_POS          (4)             /// CC_HDMI_PCM_MULT_CHANNEL
    #define PCM_CH_NUM_MASK         (UINT32)(0x3 << 4)
      #define PCM_CH_NUM_2_CH         (0x0 << 4)
      #define PCM_CH_NUM_4_CH         (0x1 << 4)
      #define PCM_CH_NUM_6_CH         (0x2 << 4)
      #define PCM_CH_NUM_8_CH         (0x3 << 4)
    #define RISC_INT_PERIOD_MASK    (0x3 << 6)
      #define RISC_INT_PERIOD_32      (0x0 << 6)      // RISC interrupt generating period per 32*4 bytes
      #define RISC_INT_PERIOD_64      (0x1 << 6)      // RISC interrupt generating period per 64*4 bytes
      #define RISC_INT_PERIOD_128     (0x2 << 6)      // RISC interrupt generating period per 128*4 bytes
      #define RISC_INT_PERIOD_256     (0x3 << 6)      // RISC interrupt generating period per 256*4 bytes
    #define PSU_BS_DETEC            (0x0 << 8)
    #define PSU_BS_NO_DETEC         (0x1 << 8)
    #define ADDR_CFG_SL_ALE         (0x0 << 9)
    #define ADDR_CFG_SL_INPUT       (0x1 << 9)
    #define SR_CFG_MUTE_EN          (0x1 << 10)
    #define SR_CFG_MUTE_UNEN        (0x0 << 10)
    #define INT_ADC_EXTLIN          (0x0 << 11)
    #define INT_ADC_INTLIN          (0x1 << 11)
    #define LIN_CLK_SEL_MASK        (0x3 << 12)
      #define AOUT_DEC0               (0x0 << 12)
      #define AOUT_DEC2               (0x1 << 12)
      #define AOUT_HDMI               (0x2 << 12)
    #define RAW_LONG_DETECT         (0x1U << 28)
    #define LR_DETECT_ON            (0x1U << 31)

#define REG_DEC2_LIN_CFG0            0x001C

#define REG_LINBLK              0x0020

#define REG_DEC2_LINBLK         0x0024

#define REG_SPDFTYP             0x0028
    #define TYPE_DETAIL             (0x1f << 0)
      #define TYPE_AC3                (0x01 << 0)
      #define TYPE_PAUSE              (0x03 << 0)
      #define TYPE_MPEG1_L1           (0x04 << 0)
      #define TYPE_MPEG1_L2L3         (0x05 << 0)
      #define TYPE_MPEG2_EXT          (0x06 << 0)
      #define TYPE_AAC                (0x07 << 0)
      #define TYPE_MPEG2_L1LSF        (0x08 << 0)
      #define TYPE_MPEG2_L2LSF        (0x09 << 0)
      #define TYPE_MPEG2_L3LSF        (0x0a << 0)
      #define TYPE_DTS1               (0x0b << 0)
      #define TYPE_DTS2               (0x0c << 0)
      #define TYPE_DTS3               (0x0d << 0)
      #define TYPE_EAC3               (0x15 << 0)
    #define TYPE_BSNUM              (0x7 << 5)
    #define TYPE_ROUGH              (0x3 << 8)
      #define TYPE_PCM                (0 << 8)
      #define TYPE_RAW                (1 << 8)
      #define TYPE_DTS_16BITS         (2 << 8)
      #define TYPE_DTS_14BITS         (3 << 8)
    #define TYPE_DETECT             (0x1 << 10)
    
#define REG_SPDFS               0x002c
    #define SPDF_LRCK_DIV       (0x1fff)    

#define REG_SPOCFG0             0x0030
    #define SPOUT_BASIC_MASK        (0x000001ff)
    #define SPOUT_BIT_NUM_POS       (0)                 // Bit[1:0]
      #define SPOUT_BIT_NUM_24        (0x0 << SPOUT_BIT_NUM_POS)
      #define SPOUT_BIT_NUM_20        (0x2 << SPOUT_BIT_NUM_POS)
      #define SPOUT_BIT_NUM_16        (0x3 << SPOUT_BIT_NUM_POS)
    #define SPOUT_CHSEL_POS         (2)                 // Bit[4:2]
      #define SPOUT_CHSEL_MASK        (0x7 << SPOUT_CHSEL_POS)
    #define SPOUT_MUTE_POS          (5)                 // Bit[5]
      #define SPOUT_MUTE              (0x1 << SPOUT_MUTE_POS)
    #define SPOUT_DOWNSAMPLE_POS    (6)                 // Bit[7:6]
    #define SPOUT_DOWNSAMPLE_MASK   (0x03 << SPOUT_DOWNSAMPLE_POS)
      #define SPOUT_DOWNSAMPLE_0      (0x0)
      #define SPOUT_DOWNSAMPLE_2      (0x1)
      #define SPOUT_DOWNSAMPLE_4      (0x3)
    #define SPOUT_SPDF_SEL_POS      (8)
    #define SPOUT_SPDF_SEL_MASK     (0x01 << SPOUT_SPDF_SEL_POS)
      #define SPOUT_SPDF_SEL_MAIN     (0)
      #define SPOUT_SPDF_SEL_AUX      (1)
    #define HDMI_RX_SPDIF           (0x1 << 12)         // write "1" will output spdif data from HDMI-RX to SPDF pin

//#define REG_BS_CFG              0x0040
#define REG_BS_CFG(dspid)               (dspid ? 0x0640: 0x0040)
    #define CLR_BS                  (0x1 << 4)
    #define NO_HOLD_GBS             (0x1 << 7)
    #define GBCD                    (0x1 << 9)
    #define PAR_SEL                 (0x1 << 10)
    #define DEC3_PAR_SEL            (0x1 << 13)
    #define DEC4_PAR_SEL            (0x1 << 14)
    #define PTR_SEL_BANK0_POS       (16)
    #define PTR_SEL_BANK0_MASK          (0x7 << PTR_SEL_BANK0_POS)
      #define PTR_SEL_BANK0_PSR           (0 << PTR_SEL_BANK0_POS)
      #define PTR_SEL_BANK0_SPDIF_LINE_IN (1 << PTR_SEL_BANK0_POS)
      #define PTR_SEL_BANK0_DMX           (2 << PTR_SEL_BANK0_POS)
      #define PTR_SEL_BANK0_DEC2_SPDIF_LINE_IN (3 << PTR_SEL_BANK0_POS)
      #define PTR_SEL_BANK0_DEFINED0      (4 << PTR_SEL_BANK0_POS)
      #define PTR_SEL_BANK0_DEFINED1      (5 << PTR_SEL_BANK0_POS)
      #define PTR_SEL_BANK0_DEC3_SPDIF_LINE_IN (6 << PTR_SEL_BANK0_POS) 
    #define PTR_SEL_BANK1_POS       (20)
    #define PTR_SEL_BANK1_MASK          (0x7 << PTR_SEL_BANK1_POS)
      #define PTR_SEL_BANK1_PSR           (0 << PTR_SEL_BANK1_POS)
      #define PTR_SEL_BANK1_SPDIF_LINE_IN (1 << PTR_SEL_BANK1_POS)
      #define PTR_SEL_BANK1_DMX           (2 << PTR_SEL_BANK1_POS)
      #define PTR_SEL_BANK1_DEC2_SPDIF_LINE_IN (3 << PTR_SEL_BANK1_POS)
      #define PTR_SEL_BANK1_DEFINED0      (4 << PTR_SEL_BANK1_POS)
      #define PTR_SEL_BANK1_DEFINED1      (5 << PTR_SEL_BANK1_POS)
      #define PTR_SEL_BANK1_DEC3_SPDIF_LINE_IN (6 << PTR_SEL_BANK1_POS)
    #define PTR_SEL_BANK2_POS       (24)
    #define PTR_SEL_BANK2_MASK          (0x7 << PTR_SEL_BANK2_POS)
      #define PTR_SEL_BANK2_PSR           (0 << PTR_SEL_BANK2_POS)
      #define PTR_SEL_BANK2_SPDIF_LINE_IN (1 << PTR_SEL_BANK2_POS)
      #define PTR_SEL_BANK2_DMX           (2 << PTR_SEL_BANK2_POS)
      #define PTR_SEL_BANK2_DEC2_SPDIF_LINE_IN (3 << PTR_SEL_BANK2_POS)
      #define PTR_SEL_BANK2_DEFINED0      (4 << PTR_SEL_BANK2_POS)
      #define PTR_SEL_BANK2_DEFINED1      (5 << PTR_SEL_BANK2_POS)
      #define PTR_SEL_BANK2_DEC3_SPDIF_LINE_IN (6 << PTR_SEL_BANK2_POS)
    #define PTR_SEL_BANK3_POS       (28)
    #define PTR_SEL_BANK3_MASK          (0x7 << PTR_SEL_BANK3_POS)
      #define PTR_SEL_BANK3_PSR           (0 << PTR_SEL_BANK3_POS)
      #define PTR_SEL_BANK3_SPDIF_LINE_IN (1 << PTR_SEL_BANK3_POS)
      #define PTR_SEL_BANK3_DMX           (2 << PTR_SEL_BANK3_POS)
      #define PTR_SEL_BANK3_DEC2_SPDIF_LINE_IN (3 << PTR_SEL_BANK3_POS)
      #define PTR_SEL_BANK3_DEFINED0      (4 << PTR_SEL_BANK3_POS)
      #define PTR_SEL_BANK3_DEFINED1      (5 << PTR_SEL_BANK3_POS)
      #define PTR_SEL_BANK3_DEC3_SPDIF_LINE_IN (6 << PTR_SEL_BANK3_POS)

#define REG_ABUF_PNT(dspid)               (dspid ? 0x0644: 0x0044)
//#define REG_ABUF_PNT                        (0x0044)
    #define LINE_IN_SEL_WP          0x1
    #define LINE_IN_SEL_RP          0x0
#define REG_ABUF_PNT_DEC2(dspid)      (dspid ? 0x0648: 0x0048)
//#define REG_ABUF_PNT_DEC2                (0x0048)

#define REG_DMX_WRITE_PNT       0x004c
#define REG_DMX_STR             0x0050
#define REG_DMX_END             0x0054

#define REG_ABUF_PNT_DEC3(dspid)      (dspid ? 0x064C: 0x0058)
#define REG_ABUF_PNT_DEC4(dspid)      (dspid ? 0x0650: 0x005C)
//#define REG_ABUF_PNT_DEC3               (0x0058)
//#define REG_ABUF_PNT_DEC4               (0x005c)

#define REG_DEC3_LIN_CFG0       0x0060
#define REG_DEC3_LINBLK         0x0064

#define REG_RISC_INT            0x0068
    #define DSPA2RC_INTR            (0x01 << 8)
    #define DSPB2RC_INTR            (0x01 << 9)

#define REG_PUTBS_BLK0_RISC     0x0078
#define REG_PUTBS_BLK1_RISC     0x007c

#define REG_ADSP_CFG            0x0080
    #define ADSP_CFG_ICACHE_FLUSH   (0x01 << 2)
    #define ADSP_CFG_RST_B          (0x01 << 4)
    #define ADSP_CFG_CLK_ENA        (0x01 << 5)
    #define ADSP_BOOT_FROM_RAM      (0x01 << 6)
    #define ADSP_CFG_RST_B_DSPB            (0x01 << 7)
    #define ADSP_BOOT_FROM_RAM_DSPB   (0x01 << 9)    
    #define ADSP_CACHE_SIZE_POS     (10)                    // Bit[12:10]
    #define ADSP_CACHE_SIZE_MASK    (0x07 << ADSP_CACHE_SIZE_POS)
    #define CACHE_SIZE_256          (0x00)
    #define CACHE_SIZE_2K           (0x02)
    #define CACHE_SIZE_1K           (0x04)
      #define ADSP_CACHE_SIZE_POS_DSPB         (13)   
    #define ADSP_CACHE_SIZE_MASK_DSPB       (0x07 << ADSP_CACHE_SIZE_POS_DSPB)
    #define ADSP_CACHE_INST_SIZE_POS    (16)                    // Bit[17:16]
    #define ADSP_CACHE_INST_SIZE_MASK   (0x03 << ADSP_CACHE_INST_SIZE_POS)
      #define CBUF_SIZE_16X128        (0x0)
      #define CBUF_SIZE_8X128         (0x01)
      #define CBUF_SIZE_4X128         (0x02)
    #define ADSP_CMPTRD_THRES_POS   (18)                    // Bit[19:18]
    #define ADSP_CMPTRD_THRES_MASK  (0x03 << ADSP_CMPTRD_THRES_POS)
      #define THRES_UNFULL            (0x0)
      #define THRES_HALF              (0x01)
      #define THRES_3_4               (0x02)
    #define ADSP_AUDIO_BLOCK_POS    (20)
    #define ADSP_AUDIO_BLOCK_MASK   (0x7F << ADSP_AUDIO_BLOCK_POS)
    #define ADSP_DRAM_TIMEOUT_RESET (0x01 << 28)
    #define ADSP_DRAM_TIMEOUT_FLUSH (0x01 << 29)

//#define ADSP_REG_OFST                (0x6c0 >> 2)  //(0x1b0)
#define AUD_DSPB_REG_OFST            (0x0400)
#define AUD_REG_OFST                            (0x00)

#define REG_SD_ADR              0x0084


#define REG_ADSP_CFG_DSPB                    (AUD_REG_OFST + AUD_DSPB_REG_OFST +0x0080)
#if 0
#define REG_SD_ADR_INTR         0x0088
#define REG_LD                  0x008c

#define REG_R2D_STA             0x0090
    #define  DSP_B                  (0x01)

#define REG_R2D_RD              0x0094

#define REG_D2R_STA             0x0098
    #define  DREQ                   (0x01)
    #define  D2RDAV                 (0x01 << 1)

#define REG_D2R_SDA             0x009c
#define REG_D2R_RD              0x00a0

#define REG_DSP_DRAM_SBLK01     0x00b0
#define REG_DSP_DRAM_SBLK23     0x00b4
#define REG_DSP_DRAM_SBLK45     0x04b0
#define REG_DSP_DRAM_SBLK67     0x04b4

#define REG_DSP_CMPT_DRAM_SBLK01    0x00c0
#define REG_DSP_CMPT_DRAM_SBLK23    0x00c4
#define REG_DSP_CMPT_DRAM_SBLK45    0x04c0
#define REG_DSP_CMPT_DRAM_SBLK67    0x04c4

#define REG_DSP_IC_DRAM_SBLK01  0x00c8
#define REG_DSP_IC_DRAM_SBLK23  0x00cc
#define REG_DSP_IC_DRAM_SBLK45  0x00d0
#define REG_DSP_IC_DRAM_SBLK67  0x00d4

#define REG_DSP_IPGSET0         0x0380
#define REG_DSP_IPGSET1         0x0384

#define REG_CACHE_HIT_CNT       (0xE0)
#define REG_CACHE_MISS_CNT      (0xE4)

#else
#define REG_SD_ADR_INTR(dspid)     (dspid ?(AUD_REG_OFST + AUD_DSPB_REG_OFST + (0x0088>>0)):(AUD_REG_OFST +(0x0088>>0)))
#define REG_LD(dspid)                     (dspid ?(AUD_REG_OFST + AUD_DSPB_REG_OFST + (0x008C>>0)):(AUD_REG_OFST + (0x008C>>0)))

#define RREG_RC2DSP_STA_DSPB      (AUD_REG_OFST + AUD_DSPB_REG_OFST+ (0x0090>>0))


#define REG_R2D_STA(dspid)            (dspid ?(AUD_REG_OFST + AUD_DSPB_REG_OFST+ (0x0090>>0)):(AUD_REG_OFST + (0x0090>>0)))
    #define  DSP_B                                  (0x01)

#define REG_R2D_RD                        (AUD_REG_OFST + (0x0094>>0))

#define REG_D2R_STA(dspid)             (dspid ?(AUD_REG_OFST + AUD_DSPB_REG_OFST + (0x0098>>0)):(AUD_REG_OFST + (0x0098>>0)))
    #define  DREQ                                   (0x01)
    #define  D2RDAV                               (0x01 << 1)


#define REG_D2R_SDA(dspid)                                  (dspid ? (AUD_REG_OFST +AUD_DSPB_REG_OFST + (0x009C>>0)):(AUD_REG_OFST + (0x009C>>0)))
#define REG_D2R_RD(dspid)                                    (dspid ? (AUD_REG_OFST + AUD_DSPB_REG_OFST +(0x00A0>>0)): (AUD_REG_OFST + (0x00A0>>0)))
#define REG_DSP_DRAM_SBLK01(dspid)                    (dspid ? 0x04c8: 0x00b0)
#define REG_DSP_DRAM_SBLK23(dspid)                    (dspid ?0x04cc: 0x00b4)
#define REG_DSP_DRAM_SBLK45(dspid)                    (dspid ? 0x04d0: 0x04b0)
#define REG_DSP_DRAM_SBLK67(dspid)                    (dspid ?0x04d4: 0x04b4)
#define REG_DSP_CMPT_DRAM_SBLK01(dspid)          (dspid ? 0x04d8: 0x00c0)
#define REG_DSP_CMPT_DRAM_SBLK23(dspid)          (dspid ? 0x04dc: 0x00c4)
#define REG_DSP_CMPT_DRAM_SBLK45(dspid)          (dspid ? 0x04e0:  0x04c0)
#define REG_DSP_CMPT_DRAM_SBLK67(dspid)          (dspid ? 0x04e4: 0x04c4)
#define REG_DSP_IC_DRAM_SBLK01(dspid)               (dspid ? 0x0600:  0x00c8)
#define REG_DSP_IC_DRAM_SBLK23(dspid)               (dspid ? 0x0604: 0x00cc)
#define REG_DSP_IC_DRAM_SBLK45(dspid)               (dspid ? 0x0608: 0x00d0)
#define REG_DSP_IC_DRAM_SBLK67(dspid)               (dspid ? 0x060c: 0x00d4)
//#define REG_DSP_IC_DRAM_SBLK01(dspid)               (dspid ? 0x04c8:  0x00c8)
//#define REG_DSP_IC_DRAM_SBLK23(dspid)               (dspid ? 0x04cc: 0x00cc)
//#define REG_DSP_IC_DRAM_SBLK45(dspid)               (dspid ? 0x04d0: 0x00d0)
//#define REG_DSP_IC_DRAM_SBLK67(dspid)               (dspid ? 0x04d4: 0x00d4)

#define REG_DSP_IPGSET0(dspid)                             (dspid ?0x0610:0x0380)
#define REG_DSP_IPGSET1(dspid)                             (dspid ?0x0614:0x0384)

#define REG_CACHE_HIT_CNT(dspid)                            (dspid ?0x618:0xE0)
#define REG_CACHE_MISS_CNT(dspid)                          (dspid ?0x61C:0xE4)

#endif

#define REG_DSP_PC              0x015c
#define REG_DSP_PC_DSPB              0x055c

// PROBE_RELATED_API
#define REG_DSP_CTRL_R          0x0100

#define REG_DSP_RB_BASE         0x0114
#define REG_DSP_PB_CMSK         0x0114
#define REG_DSP_PB_CTAR         0x0118
#define REG_DSP_PB_TAR          0x011C
#define REG_DSP_PB_CTRL         0x0120
#define REG_DSP_PB_INDEX        0x0124
#define REG_DSP_PB_DATA         0x0128
#define REG_DSP_PB_STOP         0x012c
#define REG_DSP_PB_STP0         0x0130
#define REG_DSP_PB_STP1         0x0134
#define REG_DSP_PB_STP2         0x0138
#define REG_DSP_PB_STP3         0x013c
#define REG_DSP_PB_WMSK         0x0140
#define REG_DSP_PB_WTAR         0x0144
#define REG_DSP_PB_WRDX         0x0148
#define REG_DSP_PB_WR           0x014c
#define REG_DSP_PB_HIDX         0x0150

#define REG_DSP_CTRL_R_DSPB          0x0500
#define REG_DSP_RB_BASE_DSPB         0x0514
#define REG_DSP_PB_CMSK_DSPB         0x0514
#define REG_DSP_PB_CTAR_DSPB         0x0518
#define REG_DSP_PB_TAR_DSPB          0x051C
#define REG_DSP_PB_CTRL_DSPB         0x0520
#define REG_DSP_PB_INDEX_DSPB        0x0524
#define REG_DSP_PB_DATA_DSPB         0x0528
#define REG_DSP_PB_STOP_DSPB         0x052c
#define REG_DSP_PB_STP0_DSPB         0x0530
#define REG_DSP_PB_STP1_DSPB         0x0534
#define REG_DSP_PB_STP2_DSPB         0x0538
#define REG_DSP_PB_STP3_DSPB         0x053c
#define REG_DSP_PB_WMSK_DSPB         0x0540
#define REG_DSP_PB_WTAR_DSPB         0x0544
#define REG_DSP_PB_WRDX_DSPB         0x0548
#define REG_DSP_PB_WR_DSPB           0x054c
#define REG_DSP_PB_HIDX_DSPB         0x0550

#define REG_CH_CFG              0x0200
    #define AOUTDATA0_MAIN          (0)
    #define AOUTDATA1_MAIN          (1)
    #define AOUTDATA2_MAIN          (2)
    #define AOUTDATA3_MAIN          (3)
    #define AOUTDATA4_MAIN			(4)
    #define AOUTDATA5_MAIN			(5)
    #define AOUTDATA6_MAIN			(6)
    #define AOUTDATA0_AUX           (7)

    #define CH0_SEL_POS             (0)
        #define CH_SEL_MASK             (0xFFFFFF)
        #define CH_SEL                  ((CH_SEL_MASK) << CH0_SEL_POS)

        #define CH0_AOUTDATA0           (0 << CH0_SEL_POS)
        #define CH0_AOUTDATA1           (1 << CH0_SEL_POS)
        #define CH0_AOUTDATA2           (2 << CH0_SEL_POS)
        #define CH0_AOUTDATA3           (3 << CH0_SEL_POS)
        #define CH0_AOUTDATA4			(4 << CH0_SEL_POS)
        #define CH0_AOUTDATA5			(5 << CH0_SEL_POS)
        #define CH0_AOUTDATA6			(6 << CH0_SEL_POS)
		
    #define CH1_SEL_POS             (3)
        #define CH1_AOUTDATA0           (0 << CH1_SEL_POS)
        #define CH1_AOUTDATA1           (1 << CH1_SEL_POS)
        #define CH1_AOUTDATA2           (2 << CH1_SEL_POS)
        #define CH1_AOUTDATA3           (3 << CH1_SEL_POS)
        #define CH1_AOUTDATA4           (4 << CH1_SEL_POS)
        #define CH1_AOUTDATA5			(5 << CH1_SEL_POS)
        #define CH1_AOUTDATA6			(6 << CH1_SEL_POS)

    #define CH2_SEL_POS             (6)
        #define CH2_AOUTDATA0           (0 << CH2_SEL_POS)
        #define CH2_AOUTDATA1           (1 << CH2_SEL_POS)
        #define CH2_AOUTDATA2           (2 << CH2_SEL_POS)
        #define CH2_AOUTDATA3           (3 << CH2_SEL_POS)
        #define CH2_AOUTDATA4           (4 << CH2_SEL_POS)
        #define CH2_AOUTDATA5			(5 << CH2_SEL_POS)
        #define CH2_AOUTDATA6			(6 << CH2_SEL_POS)

    #define CH3_SEL_POS             (9)
        #define CH3_AOUTDATA0           (0 << CH3_SEL_POS)
        #define CH3_AOUTDATA1           (1 << CH3_SEL_POS)
        #define CH3_AOUTDATA2           (2 << CH3_SEL_POS)
        #define CH3_AOUTDATA3           (3 << CH3_SEL_POS)
        #define CH3_AOUTDATA4           (4 << CH3_SEL_POS)
        #define CH3_AOUTDATA5			(5 << CH3_SEL_POS)
        #define CH3_AOUTDATA6			(6 << CH3_SEL_POS)

    #define CH4_SEL_POS             (12)
        #define CH4_AOUTDATA0           (0 << CH4_SEL_POS)
        #define CH4_AOUTDATA1           (1 << CH4_SEL_POS)
        #define CH4_AOUTDATA2           (2 << CH4_SEL_POS)
        #define CH4_AOUTDATA3           (3 << CH4_SEL_POS)
        #define CH4_AOUTDATA4           (4 << CH4_SEL_POS)
        #define CH4_AOUTDATA5			(5 << CH4_SEL_POS)
        #define CH4_AOUTDATA6			(6 << CH4_SEL_POS)

    #define DEC2_CH0_SEL_POS        (15)
        #define DEC2_CH0_AOUTDATA0      (0 << DEC2_CH0_SEL_POS)
        #define DEC2_CH0_AOUTDATA1      (1 << DEC2_CH0_SEL_POS)
        #define DEC2_CH0_AOUTDATA2      (2 << DEC2_CH0_SEL_POS)

    #define DEC2_CH1_SEL_POS        (18)
        #define DEC2_CH1_AOUTDATA0      (0 << DEC2_CH1_SEL_POS)
        #define DEC2_CH1_AOUTDATA1      (1 << DEC2_CH1_SEL_POS)
        #define DEC2_CH1_AOUTDATA2      (2 << DEC2_CH1_SEL_POS)

    #define DEC2_CH2_SEL_POS        (21)
        #define DEC2_CH2_AOUTDATA0      (0 << DEC2_CH2_SEL_POS)
        #define DEC2_CH2_AOUTDATA1      (1 << DEC2_CH2_SEL_POS)
        #define DEC2_CH2_AOUTDATA2      (2 << DEC2_CH2_SEL_POS)

    #define SDATA_MCLK_SYNC_POS     (24)
    #define SDATA_MCLK_SYNC_MASK    (0x3F)
    #define SDATA_MCLK_SYNC         (SDATA_MCLK_SYNC_MASK << SDATA_MCLK_SYNC_POS)

    #define AOUTDATA0_AUX_TO_SDATA0 (1 << 24)
    #define AOUTDATA0_AUX_TO_SDATA1 (1 << 25)
    #define AOUTDATA0_AUX_TO_SDATA2 (1 << 26)
    #define AOUTDATA0_AUX_TO_SDATA3 (1 << 27)
    #define AOUTDATA0_AUX_TO_SDATA4 (1 << 28)
    #define AOUTDATA0_AUX_TO_SDATA5 (1 << 29)
    #define AO_BCK_LRCK_SEL            (1 << 30)
    #define BO_BCK_LRCK_SEL            (1 << 31)

#define REG_IDACHCG    0x0204

#define REG_IEC_DIV    0x0208
    #define IEC_AUDCK_SEL    (1<<16)
    #define IEC_UPDATE_SEL_0T    (0<<14)  // Sync from [DTV00094645] (IEC_UPDATE_SEL fine tune)
    #define IEC_UPDATE_SEL_1T    (1<<14)
    #define IEC_UPDATE_SEL_2T    (2<<14)
    #define IEC_UPDATE_SEL_3T    (3<<14)
    #define IEC_CLK_INDEPENDENT_SEL    (1<<12)
    #define IEC_LRCK_CYCLE    (0x3f)    //??

#define REG_AOUT_DRAM_CFG       0x0210
    #define DRAM_ACCESS_128_BITS    (1 << 4)
    #define AOUT_VOL_CTRL_BY_DSP    (1<<21)
    #define AOUT2_VOL_CTRL_BY_DSP   (1<<20)
    #define ADD_DEC2_POS            (6)
    #define ADD_DEC2_MASK           (0x1F<<ADD_DEC2_POS)
    #define AOUT2_USE_AOUT1_CLK     (1<<23)
    #define ADD_DEC2_MIX_POS        (24)
    #define ADD_DEC2_MIX_MASK       (0x1F<<ADD_DEC2_MIX_POS)

//-----------------------------------------------------------------------------
// Ain Module Register Map
//-----------------------------------------------------------------------------
#define REG_AUDIN_CFG           0x0220
    #define REG_AUDIN_CFG_MASK      (0x91003F1FU)
    #define APGA_GAIN_POS           (0)                 // Bit[4:0]
    #define APGA_GAIN_MASK          (0x1FU << APGA_GAIN_POS)
      #define APGA_GAIN_0DB           (0x00U << APGA_GAIN_POS)
    #define APGA_OFFSET_POS         (8)                  // Bit[13:8]
    #define APGA_OFFSET_MASK        (0x3FU << APGA_OFFSET_POS)
      #define APGA_OFFSET_0V          (0x00U << APGA_OFFSET_POS)
    #define APGA_GAIN_BY_DSP        (0x00U << 24)
    #define APGA_GAIN_BY_RISC       (0x01U << 24)
    #define ADCK_INV_RISING         (0x00U << 28)
    #define ADCK_INV_FALLING        (0x01U << 28)
    #define ADC_SEL_SIF             (0x00U << 31)
    #define ADC_SEL_DEMOD           (0x01U << 31)

    #if defined(CC_MT5365) || defined(CC_MT5395)
#define REG_ATVCFG0             0x0228
    #define REG_ATVCFG0_MASK        (0x7771F3FF)
    #define RG_SIF_SEL_POS          (28)          // Bit[8:0]
    #define RG_SIF_SEL_MASK         (0x3 << RG_PGA_SIF_PWD_POS)
    #define SIF_SEL_OFF             (0x00 << RG_SIF_SEL_POS)    // default
    #define SIF_SEL_AF              (0x01 << RG_SIF_SEL_POS)
    #define SIF_SEL_SIF             (0x02 << RG_SIF_SEL_POS)
    #define RG_PGA_SIF_PWD_POS      (0)          // Bit[8:0]
    #define RG_PGA_SIF_PWD_MASK     (0x17F << RG_PGA_SIF_PWD_POS)
    #define RG_PGA_SIF_PWD_ON       (0x080 << RG_PGA_SIF_PWD_POS)
    #define RG_PGA_SIF_PWD_DOWN     (0x1FF << RG_PGA_SIF_PWD_POS)    // default

#define REG_ATVCFG1             0x022c
    #define REG_ATVCFG1_MASK        (0xFF333F73)
    #define RG_SIF_CHSEL_POS        (20)          // Bit[21:20]
    #define RG_SIF_CHSEL_MASK       (0x3 << RG_SIF_CHSEL_POS)
    #define RG_SIF_CHSEL_TEST       (0x00 << RG_SIF_CHSEL_POS)
    #define RG_SIF_CHSEL_SIF        (0x01 << RG_SIF_CHSEL_POS)    // default
    #define RG_SIF_CHSEL_T_IN       (0x10 << RG_SIF_CHSEL_POS)
    #define RG_SIF_CHSEL_NA         (0x11 << RG_SIF_CHSEL_POS)
    #define RG_SIF_ADC_PWD_POS      (17)          // Bit[17]
    #define RG_SIF_ADC_PWD_MASK     (0x1<< RG_SIF_ADC_PWD_POS)
    #define RG_SIF_ADC_PWD_ON       (0x00 << RG_SIF_ADC_PWD_POS)
    #define RG_SIF_ADC_PWD_DOWN     (0x1 << RG_SIF_ADC_PWD_POS)    // default
    #define RG_SIF_SIF_EN_POS       (16)          // Bit[17]
    #define RG_SIF_SIF_EN_MASK      (0x1<< RG_SIF_SIF_EN_POS)
    #define RG_SIF_SIF_EN_ON        (0x01 << RG_SIF_SIF_EN_POS)     // default
    #define RG_SIF_SIF_EN_DOWN      (0x0 << RG_SIF_SIF_EN_POS)
    #define RG_BIAS_SIF_PWD_POS     (6)          // Bit[6]
    #define RG_BIAS_SIF_PWD_MASK    (0x1<< RG_BIAS_SIF_PWD_POS)
    #define RG_BIAS_SIF_PWD_ON      (0x00 << RG_BIAS_SIF_PWD_POS)
    #define RG_BIAS_SIF_PWD_DOWN    (0x1 << RG_BIAS_SIF_PWD_POS)    // default
    #endif

//-----------------------------------------------------------------------------
// MISC
//-----------------------------------------------------------------------------
#define REG_TEST_CFG            0x0250
    #define HDMI_SPDIF_SEL      (0x1 << 0)
    #define DTVSLT              (0x03 << 1)

#define REG_USER0_WP    0x260
#define REG_USER0_ST    0x264   // USE_FIFO3 : dec3 use fifo3
#define REG_USER0_END   0x268
#define REG_USER1_WP    0x26c
#define REG_USER1_ST    0x270
#define REG_USER1_END   0x274

//---------------------------------------------------------------------------
// AOUT CRC
//---------------------------------------------------------------------------
#define REG_AOUT_CRC_CTL        0x2c0

//---------------------------------------------------------------------------
// ROM Timing Config
//---------------------------------------------------------------------------
#define REG_DSPROM_CFG          0x16C
#define REG_AINROM_CFG          0x288

//---------------------------------------------------------------------------
// Line ADC Module Register Map
//---------------------------------------------------------------------------
#define REG_IDACHCG             0x0204
    #define DAC0_SEL_POS        (0)
    #define DAC1_SEL_POS        (3)
    #define DAC2_SEL_POS        (6)
    #define DAC3_SEL_POS        (16)

    #define DAC_SEL_MASK        (0x7U)
    #define DAC_SEL_0_LR        (0x0U)
    #define DAC_SEL_0_SLR       (0x1U)
    #define DAC_SEL_0_CLFE      (0x2U)
    #define DAC_SEL_0_CH78      (0x3U)
    #define DAC_SEL_0_CH90      (0x4U)
    #define DAC_SEL_0_CH1112    (0x5U)
    #define DAC_SEL_0_CH1314    (0x6U)        
    #define DAC_SEL_1_LR        (0x7U)

#define REG_INTADC_SDADC0       0x0240
    #define RST_FILTER          (0x1 << 0)

#define REG_INTADC_SDADC1       0x0244

#if defined(CC_MT5365) || defined(CC_MT5395)
#define REG_INTADC_SDADC2       0x0248
    /* ADC left channel input mixer control bit */
    #define ADC_GPIO_MASK       (0xff << 23)
    #define ADC_GPIO_EN         (0xff << 23)
    #define ADC_GPIO_DIS        (0x00 << 23)
    #define ADC_GPIO0           (0x1  << 23)
    #define ADC_GPIO1           (0x1  << 24)
    #define ADC_GPIO2           (0x1  << 25)
    #define ADC_GPIO3           (0x1  << 26)
    #define ADC_GPIO4           (0x1  << 27)
    #define ADC_GPIO5           (0x1  << 28)
    #define ADC_GPIO6           (0x1  << 29)

    #define ADC_INPUT_MUX_MASK  (0x7  << 20)
    #define ADC_IN0L0R          (0x0  << 20)
    #define ADC_IN1L1R          (0x1  << 20)
    #define ADC_IN2L2R          (0x2  << 20)
    #define ADC_IN3L3R          (0x3  << 20)
    #define ADC_IN4L4R          (0x4  << 20)
    #define ADC_IN5L5R          (0x5  << 20)
    #define ADC_IN6L6R          (0x6  << 20)
    #define ADC_L_PD            (0x1 << 12)
    #define ADC_R_PD            (0x1 << 13)
    #define LEFT_PD             (0x1 << 14)
    #define RIGHT_PD            (0x1 << 15)

#define REG_INTADC_SDADC3       0x024c
#endif

#if defined(CC_MT5399) || defined(CC_MT5890) || defined(CC_MT5882)
#define REG_INTADC_SDADC3       0x024C
#endif

//---------------------------------------------------------------------------
// HADDS1
//---------------------------------------------------------------------------
#define REG_HADDS1_CFG0         0x03d4
#define REG_HADDS1_CFG1         0x03d8

//---------------------------------------------------------------------------
// RISC AOUT Volume Gain Control
//---------------------------------------------------------------------------
#define REG_AOUT_VOL_GAIN_CTRL  0x0580
    #define AOUT_VOL_GAIN_MARK      (0xFFFF)
    #define AOUT_VOL_GAIN_0DB       (0x1000)
    #define AOUT_VOL_GAIN_MUTE      (0x0)
    #define CH_GAIN_SEL_POS         (16)
    #define CH_GAIN_SEL_MARK        (0xF<<CH_GAIN_SEL_POS)
    #define CH_GAIN_SEL_ALL         (0x0<<CH_GAIN_SEL_POS)
    #define CH_GAIN_SEL_L           (0x1<<CH_GAIN_SEL_POS)
    #define CH_GAIN_SEL_R           (0x2<<CH_GAIN_SEL_POS)
    #define CH_GAIN_SEL_LS          (0x3<<CH_GAIN_SEL_POS)
    #define CH_GAIN_SEL_RS          (0x4<<CH_GAIN_SEL_POS)
    #define CH_GAIN_SEL_C           (0x5<<CH_GAIN_SEL_POS)
    #define CH_GAIN_SEL_LFE         (0x6<<CH_GAIN_SEL_POS)
    #define CH_GAIN_SEL_CH7         (0x7<<CH_GAIN_SEL_POS)
    #define CH_GAIN_SEL_CH8         (0x8<<CH_GAIN_SEL_POS)
    #define CH_GAIN_SEL_CH9         (0x9<<CH_GAIN_SEL_POS)
    #define CH_GAIN_SEL_CH10        (0xa<<CH_GAIN_SEL_POS)
    #define CH_GAIN_SEL_LOAD        (0xf<<CH_GAIN_SEL_POS)
#define REG_AOUT_VOL_GAIN_SETUP  0x0584
    #define AOUT_GAIN_BYPASS        (0x1<<8)
    #define AOUT_VOL_STEP_MARK      (0xFF)
    #define AOUT_VOL_STEP_1DB       (0x1)
    #define AOUT_VOL_SETP_4DB       (0x4)      //we use 4db step from 0x1000 to 0..that means 0x1000/4 = 1024 point fade in/out
    #define AOUT_VOL_SETP_16DB      (0x10)      //256 point fade in/out
#define REG_AOUT_VOL_READ_INFO   0x0588
#define REG_AOUT_VOL_BUSY        0x058C

#define REG_AOUT2_VOL_GAIN_CTRL  0x05c0
    #define AOUT2_VOL_GAIN_MARK     (0xFFFF)
    #define AOUT2_VOL_GAIN_0DB      (0x1000)
    #define AOUT2_VOL_GAIN_MUTE     (0x0)
    #define AOUT2_CH_GAIN_SEL_POS   (16)

#define REG_AOUT2_VOL_GAIN_SETUP     0x05C4
    #define AOUT2_GAIN_BYPASS       (0x1<<8)
    #define AOUT2_VOL_STEP_MARK     (0xFF)
    #define AOUT2_VOL_STEP_1DB      (0x1)

#define REG_AOUT2_VOL_READ_INFO   0x05C8
#define REG_AOUT2_VOL_BUSY        0x05CC

//---------------------------------------------------------------------------
// Aenv module control 
//---------------------------------------------------------------------------
#define REG_MODULE_CTRL                0x0748
    #define AOUT1_SEL_POS                 (0)
    #define AOUT1_SEL_MASK               (0x03 << AOUT1_SEL_POS)
    #define AOUT1_SEL_DSPA               (0 << AOUT1_SEL_POS)
    #define AOUT1_SEL_DSPB               (1 << AOUT1_SEL_POS)
    #define AOUT1_SEL_ARM11             (2 << AOUT1_SEL_POS)
    #define AOUT2_SEL_POS                 (2)
    #define AOUT2_SEL_MASK               (0x03 << AOUT2_SEL_POS)
    #define AOUT2_SEL_DSPA               (0 << AOUT2_SEL_POS)
    #define AOUT2_SEL_DSPB               (1 << AOUT2_SEL_POS)
    #define AOUT2_SEL_ARM11             (2 << AOUT2_SEL_POS)    
    #define IEC_SEL_POS                      (4)
    #define IEC_SEL_MASK                   (0x03 << IEC_SEL_POS)
    #define IEC_SEL_DSPA                   (0 << IEC_SEL_POS)
    #define IEC_SEL_DSPB                   (1 << IEC_SEL_POS)
    #define IEC_SEL_ARM11                 (2 << IEC_SEL_POS)    
    #define PUTBS_SEL_POS                 (6)
    #define PUTBS_SEL_MASK               (0x03 << PUTBS_SEL_POS)
    #define PUTBS_SEL_DSPA               (0 << PUTBS_SEL_POS)
    #define PUTBS_SEL_DSPB               (1 << PUTBS_SEL_POS)    
    #define ASRC1_SEL_POS                 (8)
    #define ASRC1_SEL_MASK               (0x03 << ASRC1_SEL_POS)
    #define ASRC1_SEL_DSPA               (0 << ASRC1_SEL_POS)
    #define ASRC1_SEL_DSPB               (1 << ASRC1_SEL_POS)
    #define ASRC1_SEL_ARM11             (2 << ASRC1_SEL_POS)     
    #define ASRC2_SEL_POS                 (10)
    #define ASRC2_SEL_MASK               (0x03 << ASRC2_SEL_POS)
    #define ASRC2_SEL_DSPA               (0 << ASRC2_SEL_POS)
    #define ASRC2_SEL_DSPB               (1 << ASRC2_SEL_POS)
    #define ASRC2_SEL_ARM11             (2 << ASRC2_SEL_POS)      
    #define ASRC3_SEL_POS                 (12)
    #define ASRC3_SEL_MASK               (0x03 << ASRC3_SEL_POS) 
    #define ASRC3_SEL_DSPA               (0 << ASRC3_SEL_POS)
    #define ASRC3_SEL_DSPB               (1 << ASRC3_SEL_POS)
    #define ASRC3_SEL_ARM11             (2 << ASRC3_SEL_POS)  

//---------------------------------------------------------------------------
// Parser definitions
//---------------------------------------------------------------------------

#define REG_A0STA               0x0014      // Audio buffer 0 start address
#define REG_A0END               0x0018      // Audio buffer 0 end address

#define REG_V_STC_1             0x004c
#define REG_A_STC_1             0x01c4
#define REG_PAR_CTRL            0x01f0
#define REG_STC_CTRL            0x01f4

//---------------------------------------------------------------------------
// Macro definitions
//---------------------------------------------------------------------------
#define AUD_READ32(Reg)                IO_READ32(AUDIO_BASE, (Reg))
#define AUD_WRITE32(Reg, Val)          IO_WRITE32(AUDIO_BASE, (Reg), (Val))

#ifdef CC_AUD_HDMI_PARSER_2_0_SUPPORT
#define AUD_READIEC16(offset)          IO_READ16(0,(offset))
#endif

#define AUD_SET_BIT(Reg, Bit)          AUD_WRITE32(Reg, AUD_READ32(Reg) | (Bit))
#define AUD_CLR_BIT(Reg, Bit)          AUD_WRITE32(Reg, AUD_READ32(Reg) & (~(Bit)))

#define AUD_READ32_SPECIAL(offset)          ((AUD_READ32(offset) & 0xffff) | ((AUD_READ32(offset) & 0x1800000) << 1))
#define AUD_CLR_BIT_SPECIAL(Reg, Bit)       AUD_WRITE32(Reg, AUD_READ32_SPECIAL(Reg) & (~(Bit)))

#define AUD_WRITE32_MSK(Reg,Val,Msk)   AUD_WRITE32(Reg, (AUD_READ32(Reg)&(~(Msk))) | (Val))

#define PARSER_READ32(Reg)             IO_READ32(PARSER0_BASE, (Reg))
#define PARSER_WRITE32(Reg, Val)       IO_WRITE32(PARSER0_BASE, (Reg), (Val))

#define SendDSPInt(dspid, arg1, arg2)         AUD_WRITE32(REG_SD_ADR_INTR(dspid), (((UINT32) arg1) + (((UINT32) arg2) << 8)))
#define SendDSPLInt(dspid, arg1, arg2)        AUD_WRITE32(REG_SD_ADR_INTR(dspid), (((UINT32) arg1) + (((UINT32) arg2) << 16)))

//---------------------------------------------------------------------------
// ANA definitions (New after 5368/96)
//---------------------------------------------------------------------------
#if defined(CC_MT5880) || defined(CC_MT5881) //may be fixed, paul
    #define REG_AADC_CFG0           0x0000
#elif defined(CC_MT5882)
    #define REG_AADC_CFG0           0x0900
#else
    #define REG_AADC_CFG0           0x0800
#endif

#if defined(CC_MT5890)
    //[31:24] RG_GPIO_EN[7:0]
    //Changed by Remy
    #define ADC_BGVMID_PWD_ON   (0x1 << 31) ///Power ON VMID generation circuit refering to bandgap 
    #define ADC_BGVMID_PWD_OFF  (0x0 << 31) ///Power Off VMID generation circuit refering to bandgap 
    #define ADC_VMID_REF_BG     (0x1 << 30)
    #define ADC_BGVMID_TUNE_EN  (0x1 << 29) /// Enable tuning for VMID refering to bandgap
    #define ADC_BGVMID_TUNE_DIS (0x0 << 29) /// Disable tuning for VMID refering to bandgap
    #define ADC_VMID_REF_GND    (0x1 << 28) /// 0:AVSS33_CLN, 1:AVSS12_DEMOD

    #define ADC_FBKDAC_AVSS33_CLN  (0x03  << 26)
    #define ADC_FBKDAC_AVSS33      (0x02  << 26)
    #define ADC_FBKDAC_VRN_01      (0x01  << 26)
    #define ADC_FBKDAC_VRN_00      (0x00  << 26)
        
    #define ADC_VREFGEN_11      (0x03  << 24)///0.95/0.05
    #define ADC_VREFGEN_10      (0x02  << 24)///0.925/0.075
    #define ADC_VREFGEN_01      (0x01  << 24)///0.9/0.1
    #define ADC_VREFGEN_00      (0x00  << 24)///0.875/0.125
    
#elif defined(CC_MT5881) || defined(CC_MT5399) || defined(CC_MT5882)
    //[31:24] RG_GPIO_EN[7:0]
    //Changed by Remy
    #define ADC_EN_5M_VMID      (0x1 << 31) /// when RG_VREFGEN_LPF = 1.
    #define ADC_VMID_REF_BG     (0x1 << 30)
    #define ADC_VMID_LVL        (0x1 << 29) /// when VMID_REF_BG=1 (0:1.6, 1:1.675)
    #define ADC_VMID_REF_GND    (0x1 << 28) /// 0:AVSS33_CLN, 1:AVSS12_DEMOD
    // RG_GPIO_EN[3:0], Remy ADC only have 4 pairs.
    #define ADC_GPIO_MASK       (0x0f << 24)
    #define ADC_GPIO_EN         (0x0f << 24)
    #define ADC_GPIO_DIS        (0xf0 << 24)
    #define ADC_GPIO0           (0x01  << 24)
    #define ADC_GPIO1           (0x01  << 25)
    #define ADC_GPIO2           (0x01  << 26)
    #define ADC_GPIO3           (0x01  << 27)
#else 
    //[31:24] RG_GPIO_EN
    /* ADC left channel input mixer control bit */
    #define ADC_GPIO_MASK       (0xff << 24)
    #define ADC_GPIO_EN         (0xff << 24)
    #define ADC_GPIO_DIS        (0x00 << 24)
    #define ADC_GPIO0           (0x1  << 24)
    #define ADC_GPIO1           (0x1  << 25)
    #define ADC_GPIO2           (0x1  << 26)
    #define ADC_GPIO3           (0x1  << 27)
    #define ADC_GPIO4           (0x1  << 28)
    #define ADC_GPIO5           (0x1  << 29)
    #define ADC_GPIO6           (0x1  << 30)
#endif

#if defined(CC_MT5890) 
    #define ADC_CKEXT_EN        (0x1  << 22) ///Enable clock from DMAD's test pins
    //[21:20] RG_ADCINSEL
    #define ADC_INPUT_MUX_MASK  (0x03  << 20)
    #define ADC_IN0L0R          (0x00  << 20)
    #define ADC_IN1L1R          (0x01  << 20)
    #define ADC_IN2L2R          (0x02  << 20)
    #define ADC_IN3L3R          (0x03  << 20)

#else
    //[22:20] RG_ADCINSEL
    #define ADC_INPUT_MUX_MASK  (0x07  << 20)
    #define ADC_IN0L0R          (0x00  << 20)
    #define ADC_IN1L1R          (0x01  << 20)
    #define ADC_IN2L2R          (0x02  << 20)
    #define ADC_IN3L3R          (0x03  << 20)
    #define ADC_IN4L4R          (0x04  << 20)
    #define ADC_IN5L5R          (0x05  << 20)
    #define ADC_IN6L6R          (0x06  << 20)
#endif

#if defined(CC_MT5890) 
    #define ADC_MUTE_SDM        (0x1  << 11) ///MUTE SDM:0_disable;1_enable
    #define ADC_REF_BIAS_SEL    (0x1  << 8)  ///)0:un-trimmed;1:trimmed;
    #define ADC_SEL_PLL         (0x1  << 3)  ///0:IFPLL 432Mhz;1:HADDS 147.456Mhz;
    #define ADC_CKEDGE          (0x1  << 0)  ///0:Rising edge of DA_CK48M;1:Falling edge of DA_CK48M;
#else
    #define ADC_PWD_PGA         (0x1  << 11) ///Power down PGA,and connect sampling metwork inputs to VCM(This function is gated by RG_MUTE_RIGHT/LEFT)
    #define RG_SELCK            (0x1 << 3) /// Channel clock, 0:ori-clock, 1:shift 180 degree
    #define RG_CK256EDGE        (0x1 << 0) /// Channel clock, 0:ori-clock, 1:shift 90 degree (if set 1, 60004[31],[29] need set 1 too)
#endif

    #define ADC_R_MUTE          (0x1 << 17)
    #define ADC_L_MUTE          (0x1 << 16)
    #define ADC_L_PD            (0x1 << 12)
    #define ADC_R_PD            (0x1 << 13)
    
    #define VCMGEN_LP_DIS       (0x00 << 4) ///For Oryx & Mustang setting_BIT[4]_Local RC LPF control:disable;BIT[5]_Power control fro VMID generation:power off
    #define VCMGEN_LP_2M        (0x01 << 4) ///For Oryx & Mustang setting_BIT[4]_Local RC LPF control:enable;BIT[5]_Power control fro VMID generation:power off
    #define VCMGEN_LP_1M        (0x02 << 4) ///For Oryx & Mustang setting_BIT[4]_Local RC LPF control:disable;BIT[5]_Power control fro VMID generationPower control fro VMID generation:power on
    #define VCMGEN_LP_500K      (0x03 << 4) ///For Oryx & Mustang setting_BIT[4]_Local RC LPF control:enable;BIT[5]_Power control fro VMID generationPower control fro VMID generation:power on

    #define RG_CKDLY0           (0x00 << 1) /// no delay
    #define RG_CKDLY1           (0x01 << 1) /// 8ns
    #define RG_CKDLY2           (0x02 << 1) /// 16ns
    #define RG_CKDLY3           (0x03 << 1) /// max
    
#if defined(CC_MT5880) || defined(CC_MT5881) // may be  fixed, paul
    #define REG_AADC_CFG1           0x0004
#elif defined(CC_MT5882)
    #define REG_AADC_CFG1           0x0904
#else
    #define REG_AADC_CFG1           0x0804
#endif


#if defined(CC_MT5881) || defined(CC_MT5399) || defined(CC_MT5882)
    #define L_RESAMPLE_EN       (0x1 << 31)
    #define R_RESAMPLE_EN       (0x1 << 30)
    #define L_RESAMPLE_FALL     (0x1 << 29) /// 0:rising, 1:falling
    #define R_RESAMPLE_FALL     (0x1 << 28) /// 0:rising, 1:falling

    #define RG_SELCK_R          (0x1 << 27) /// Channel clock, 0:ori-clock, 1:shift 180 degree
    #define RG_CKDLY0_R         (0x00 << 25) /// no delay
    #define RG_CKDLY1_R         (0x01 << 25) /// 8ns
    #define RG_CKDLY2_R         (0x02 << 25) /// 16ns
    #define RG_CKDLY3_R         (0x03 << 25) /// max
    #define RG_CK256EDGE_R      (0x1 << 24) /// Channel clock, 0:ori-clock, 1:shift 90 degree (if set 1, [30],[28] need set 1 too)
    ///Reduce ADC's interference for other module.
    #define DE_SENSE_DWA        (0x1 << 20)
    ///Remy DE_SENSE RG_REV0[3:0], Viper DE_SENSE RG_REV[2:0].
    #define DE_SENSE_EN_LVL15   (0xF << 16)
    ///VREFGEN Bias Current
    #define RG_VREFGEN_IB_15UA  (0x00 << 0)
    #define RG_VREFGEN_IB_20UA  (0x01 << 0)
    #define RG_VREFGEN_IB_25UA  (0x02 << 0)
    #define RG_VREFGEN_IB_30UA  (0x03 << 0)
    
#elif defined(CC_MT5890) 
    #define RG_COEF_G           (0xc0 << 24) ///always set 1100 0000
    #define RG_CLAMPBIAS        (0x1 << 22)  ///always set 10
    
    #define RG_SARBIAS_0X5      (0x03 << 20)
    #define RG_SARBIAS_1X       (0x02 << 20)  ///always set 10
    #define RG_SARBIAS_1X5      (0x01 << 20)
    #define RG_SARBIAS_2X       (0x00 << 20)
    
    #define RG_SAR_CK           (0x0 << 11)  ///0:from CKGEN;1:from FBKDAC's level shift
    #define RG_CK_RS_EDGE       (0x0 << 5)   ///0:rising;1:falling
    #define RG_CK_RS_EN         (0x0 << 4)   ///0:Disable;1:enable
    #define RG_DO_RS_EN         (0x1 << 3)   ///0:Disable;1:enable
    #define RG_CK_RS_SEL        (0x0 << 2)   ///0:by L CH clock;1:by R CH clock;
        ///VREFGEN Bias Current
    #define RG_VREFGEN_BIAS_0X5  (0x03 << 0)
    #define RG_VREFGEN_BIAS_1X   (0x02 << 0)
    #define RG_VREFGEN_BIAS_1X5  (0x01 << 0)
    #define RG_VREFGEN_BIAS_2X   (0x00 << 0)
#endif

#if defined(CC_MT5890)
    #define RG_VMIDBIAS_0X5     (0x03 << 18)
    #define RG_VMIDBIAS_1X      (0x02 << 18)  ///always set 10
    #define RG_VMIDBIAS_1X5     (0x01 << 18)
    #define RG_VMIDBIAS_2X      (0x00 << 18)
    #define RG_DESENSE_CTRL     (0x3 << 16)  ///always set 11(AVDD33_LS_L/R;100ohm)
    #define RG_VREG_G           (0x1 << 13) ///Enable QS_ABIST_DC_AADC,0:Disable;1:enable
#else
    #define DE_SENSE_DIS        (0x00 << 16)
    #define DE_SENSE_EN_LVL1    (0x01 << 16)
    #define DE_SENSE_EN_LVL7    (0x07 << 16)
    #define DEMOD_GROUND_ON     (0x1 << 13)
#endif
    #define PGA_CLAMP_ON        (0x1 << 12)
    #define PGA_EN              (0x1 << 14)
    #define ADC_VCMGEN_1M_R_ON  (0x1 << 15)


    //[5:3] ABIST AADC input mux selection <=> 0x524c[2:0] before 5365/66/95
    //[2] AADC ABIST Enable <=> 0x5248[31] before 5365/66/95
#if defined(CC_MT5880) || defined(CC_MT5881) // may be fixed, paul
    #define REG_AADC_CFG2           0x0008
#elif defined(CC_MT5882)
    #define REG_AADC_CFG2           0x0908
#else
    #define REG_AADC_CFG2           0x0808
#endif

#if defined(CC_MT5881)
#define REG_ADAC_CFG0           0x0100
    ///[29:24]
    #define APLL_CLK_APLL1      ((UINT32)0x00 << 24)
    #define APLL_CLK_APLL2      ((UINT32)0x3F << 24)
    ///[21:16]
    #define CLK_SEL_APLL        ((UINT32)0x00 << 16)
    #define CLK_SEL_432M        ((UINT32)0x3F << 16)
    ///[13:8]
    #define CLK_NORMAL          ((UINT32)0x00 << 8)
    #define CLK_INVERSE         ((UINT32)0x3F << 8)
    ///[5:0]
    #define RELATCH_DIS         ((UINT32)0x00 << 0)
    #define RELATCH_EN          ((UINT32)0x3F << 8)

#define REG_ADAC_CFG1           0x0104
    ///[29:24]
    #define PS_APLL_CLK_APLL1      ((UINT32)0x00 << 24)
    #define PS_APLL_CLK_APLL2      ((UINT32)0x3F << 24)
    ///[21:16]
    #define PS_CLK_SEL_APLL        ((UINT32)0x00 << 16)
    #define PS_CLK_SEL_432M        ((UINT32)0x3F << 16)
    ///[13:8]
    #define PS_CLK_NORMAL          ((UINT32)0x00 << 8)
    #define PS_CLK_INVERSE         ((UINT32)0x3F << 8)
    ///[5:0]
    #define PS_RELATCH_DIS         ((UINT32)0x00 << 0)
    #define PS_RELATCH_EN          ((UINT32)0x3F << 8)

#define REG_ADAC_CFG2           0x0108
    #define ACT_UP                 ((UINT32)0x1 << 16)
    #define ACT_ENDP_CH2           ((UINT32)0x1 << 11)
    #define ACT_ENVO_CH2           ((UINT32)0x1 << 10)
    #define ACT_SLEW_CH2           ((UINT32)0x1 << 9)
    #define ACT_PLAY_CH2           ((UINT32)0x1 << 8)
    #define ACT_ENDP_CH1           ((UINT32)0x1 << 7)
    #define ACT_ENVO_CH1           ((UINT32)0x1 << 6)
    #define ACT_SLEW_CH1           ((UINT32)0x1 << 5)
    #define ACT_PLAY_CH1           ((UINT32)0x1 << 4)
    #define ACT_ENDP_CH0           ((UINT32)0x1 << 3)
    #define ACT_ENVO_CH0           ((UINT32)0x1 << 2)
    #define ACT_SLEW_CH0           ((UINT32)0x1 << 1)
    #define ACT_PLAY_CH0           ((UINT32)0x1 << 0)

#define REG_ADAC_CFG3           0x010C
    #define DAC_HIZ                ((UINT32)0x1 << 24)
    #define DAC_DFC_CH2            ((UINT32)0x1 << 22)
    #define DAC_DFC_CH1            ((UINT32)0x1 << 21)
    #define DAC_DFC_CH0            ((UINT32)0x1 << 20)
    #define DAC_INV_CH2            ((UINT32)0x1 << 18)
    #define DAC_INV_CH1            ((UINT32)0x1 << 17)
    #define DAC_INV_CH0            ((UINT32)0x1 << 16)
    ///[9:8]
    #define DAC_FS_CH2_P36         ((UINT32)0x00 << 8)
    #define DAC_FS_CH2_P40         ((UINT32)0x01 << 8)
    #define DAC_FS_CH2_P42         ((UINT32)0x02 << 8)
    #define DAC_FS_CH2_P45         ((UINT32)0x03 << 8)
    ///[5:4]
    #define DAC_FS_CH1_P36         ((UINT32)0x00 << 4)
    #define DAC_FS_CH1_P40         ((UINT32)0x01 << 4)
    #define DAC_FS_CH1_P42         ((UINT32)0x02 << 4)
    #define DAC_FS_CH1_P45         ((UINT32)0x03 << 4)
    ///[1:0]
    #define DAC_FS_CH0_P36         ((UINT32)0x00 << 0)
    #define DAC_FS_CH0_P40         ((UINT32)0x01 << 0)
    #define DAC_FS_CH0_P42         ((UINT32)0x02 << 0)
    #define DAC_FS_CH0_P45         ((UINT32)0x03 << 0)

#define REG_ADAC_CFG4           0x0110
    ///[31:30]
    #define IB_PDET_20UA            ((UINT32)0x00 << 30)
    #define IB_PDET_15UA            ((UINT32)0x01 << 30)
    #define IB_PDET_10UA            ((UINT32)0x02 << 30)
    #define IB_PDET_05UA            ((UINT32)0x03 << 30)
    ///[29:28]
    #define IB_OPA_20UA             ((UINT32)0x00 << 28)
    #define IB_OPA_15UA             ((UINT32)0x01 << 28)
    #define IB_OPA_10UA             ((UINT32)0x02 << 28)
    #define IB_OPA_05UA             ((UINT32)0x03 << 28)
    ///[27:26]
    #define IB_PWMBUF_20UA          ((UINT32)0x00 << 26)
    #define IB_PWMBUF_15UA          ((UINT32)0x01 << 26)
    #define IB_PWMBUF_10UA          ((UINT32)0x02 << 26)
    #define IB_PWMBUF_05UA          ((UINT32)0x03 << 26)
    ///[25:24]
    #define IB_STEP_20UA            ((UINT32)0x00 << 24)
    #define IB_STEP_15UA            ((UINT32)0x01 << 24)
    #define IB_STEP_10UA            ((UINT32)0x02 << 24)
    #define IB_STEP_05UA            ((UINT32)0x03 << 24)
    ///[20]
    #define AC_OFF_PWRDET           ((UINT32)0x1 << 20)
    ///[10:8]
    #define RAMP_STEP_53MV          ((UINT32)0x00 << 8)
    #define RAMP_STEP_112MV         ((UINT32)0x01 << 8)
    #define RAMP_STEP_170MV         ((UINT32)0x02 << 8)
    #define RAMP_STEP_226MV         ((UINT32)0x03 << 8)
    #define RAMP_STEP_284MV         ((UINT32)0x04 << 8)
    #define RAMP_STEP_340MV         ((UINT32)0x05 << 8)
    #define RAMP_STEP_398MV         ((UINT32)0x06 << 8)
    #define RAMP_STEP_453MV         ((UINT32)0x07 << 8)
    ///[6:4]
    #define RAMP_STEP_ST_53MV       ((UINT32)0x00 << 4)
    #define RAMP_STEP_ST_112MV      ((UINT32)0x01 << 4)
    #define RAMP_STEP_ST_170MV      ((UINT32)0x02 << 4)
    #define RAMP_STEP_ST_226MV      ((UINT32)0x03 << 4)
    #define RAMP_STEP_ST_284MV      ((UINT32)0x04 << 4)
    #define RAMP_STEP_ST_340MV      ((UINT32)0x05 << 4)
    #define RAMP_STEP_ST_398MV      ((UINT32)0x06 << 4)
    #define RAMP_STEP_ST_453MV      ((UINT32)0x07 << 4)
    ///[2:0]
    #define CKF_DIV_512             ((UINT32)0x00 << 0)
    #define CKF_DIV_1024            ((UINT32)0x01 << 0)
    #define CKF_DIV_2048            ((UINT32)0x02 << 0)
    #define CKF_DIV_4096            ((UINT32)0x03 << 0)
    #define CKF_DIV_8192            ((UINT32)0x04 << 0)
    #define CKF_DIV_16384           ((UINT32)0x05 << 0)
    #define CKF_DIV_32768           ((UINT32)0x06 << 0)
    #define CKF_DIV_65536           ((UINT32)0x07 << 0)

#define REG_ADAC_CFG5           0x0114
    ///[30:28]
    #define CKF_ST_DIV_512             ((UINT32)0x00 << 28)
    #define CKF_ST_DIV_1024            ((UINT32)0x01 << 28)
    #define CKF_ST_DIV_2048            ((UINT32)0x02 << 28)
    #define CKF_ST_DIV_4096            ((UINT32)0x03 << 28)
    #define CKF_ST_DIV_8192            ((UINT32)0x04 << 28)
    #define CKF_ST_DIV_16384           ((UINT32)0x05 << 28)
    #define CKF_ST_DIV_32768           ((UINT32)0x06 << 28)
    #define CKF_ST_DIV_65536           ((UINT32)0x07 << 28)
    ///[21:16]
    #define AL0_GPI_EN                  ((UINT32)0x1 << 16)
    #define AR0_GPI_EN                  ((UINT32)0x1 << 17)
    #define AL1_GPI_EN                  ((UINT32)0x1 << 18)
    #define AR1_GPI_EN                  ((UINT32)0x1 << 19)
    #define AL2_GPI_EN                  ((UINT32)0x1 << 20)
    #define AR2_GPI_EN                  ((UINT32)0x1 << 21)
    ///[1]
    #define RAMP_INV_DIS                ((UINT32)0x1 << 1)
    ///[0]
    #define DAC_DESENSE_CH1             ((UINT32)0x1 << 0)

#define REG_ADAC_CFG6           0x0118
    ///[31]
    #define ACOFF_CKT_CH0               ((UINT32)0x1 << 31)
    ///[30]
    #define ACOFF_CKT_CH1               ((UINT32)0x1 << 30)
    ///[25]
    #define RAMP_OUT_EN                 ((UINT32)0x1 << 25)
    ///[0]
    #define DAC_DESENSE_CH0             ((UINT32)0x1 << 16)
#elif defined(CC_MT5399)
#define REG_ADAC_CFG0           0x0900
    ///[29:24]
    #define APLL_CLK_APLL1      ((UINT32)0x00 << 24)
    #define APLL_CLK_APLL2      ((UINT32)0x3F << 24)
    ///[21:16]
    #define CLK_SEL_APLL        ((UINT32)0x00 << 16)
    #define CLK_SEL_432M        ((UINT32)0x3F << 16)
    ///[13:8]
    #define CLK_NORMAL          ((UINT32)0x00 << 8)
    #define CLK_INVERSE         ((UINT32)0x3F << 8)
    ///[5:0]
    #define RELATCH_DIS         ((UINT32)0x00 << 0)
    #define RELATCH_EN          ((UINT32)0x3F << 8)

#define REG_ADAC_CFG1           0x0904
    ///[29:24]
    #define PS_APLL_CLK_APLL1      ((UINT32)0x00 << 24)
    #define PS_APLL_CLK_APLL2      ((UINT32)0x3F << 24)
    ///[21:16]
    #define PS_CLK_SEL_APLL        ((UINT32)0x00 << 16)
    #define PS_CLK_SEL_432M        ((UINT32)0x3F << 16)
    ///[13:8]
    #define PS_CLK_NORMAL          ((UINT32)0x00 << 8)
    #define PS_CLK_INVERSE         ((UINT32)0x3F << 8)
    ///[5:0]
    #define PS_RELATCH_DIS         ((UINT32)0x00 << 0)
    #define PS_RELATCH_EN          ((UINT32)0x3F << 8)

#define REG_ADAC_CFG2           0x0908
    #define ACT_UP                 ((UINT32)0x1 << 16)
    #define ACT_ENDP_CH2           ((UINT32)0x1 << 11)
    #define ACT_ENVO_CH2           ((UINT32)0x1 << 10)
    #define ACT_SLEW_CH2           ((UINT32)0x1 << 9)
    #define ACT_PLAY_CH2           ((UINT32)0x1 << 8)
    #define ACT_ENDP_CH1           ((UINT32)0x1 << 7)
    #define ACT_ENVO_CH1           ((UINT32)0x1 << 6)
    #define ACT_SLEW_CH1           ((UINT32)0x1 << 5)
    #define ACT_PLAY_CH1           ((UINT32)0x1 << 4)
    #define ACT_ENDP_CH0           ((UINT32)0x1 << 3)
    #define ACT_ENVO_CH0           ((UINT32)0x1 << 2)
    #define ACT_SLEW_CH0           ((UINT32)0x1 << 1)
    #define ACT_PLAY_CH0           ((UINT32)0x1 << 0)

#define REG_ADAC_CFG3           0x090C
    #define DAC_HIZ                ((UINT32)0x1 << 24)
    #define DAC_DFC_CH2            ((UINT32)0x1 << 22)
    #define DAC_DFC_CH1            ((UINT32)0x1 << 21)
    #define DAC_DFC_CH0            ((UINT32)0x1 << 20)
    #define DAC_INV_CH2            ((UINT32)0x1 << 18)
    #define DAC_INV_CH1            ((UINT32)0x1 << 17)
    #define DAC_INV_CH0            ((UINT32)0x1 << 16)
    ///[9:8]
    #define DAC_FS_CH2_P36         ((UINT32)0x00 << 8)
    #define DAC_FS_CH2_P40         ((UINT32)0x01 << 8)
    #define DAC_FS_CH2_P42         ((UINT32)0x02 << 8)
    #define DAC_FS_CH2_P45         ((UINT32)0x03 << 8)
    ///[5:4]
    #define DAC_FS_CH1_P36         ((UINT32)0x00 << 4)
    #define DAC_FS_CH1_P40         ((UINT32)0x01 << 4)
    #define DAC_FS_CH1_P42         ((UINT32)0x02 << 4)
    #define DAC_FS_CH1_P45         ((UINT32)0x03 << 4)
    ///[1:0]
    #define DAC_FS_CH0_P36         ((UINT32)0x00 << 0)
    #define DAC_FS_CH0_P40         ((UINT32)0x01 << 0)
    #define DAC_FS_CH0_P42         ((UINT32)0x02 << 0)
    #define DAC_FS_CH0_P45         ((UINT32)0x03 << 0)

#define REG_ADAC_CFG4           0x0910
    ///[31:30]
    #define IB_PDET_20UA            ((UINT32)0x00 << 30)
    #define IB_PDET_15UA            ((UINT32)0x01 << 30)
    #define IB_PDET_10UA            ((UINT32)0x02 << 30)
    #define IB_PDET_05UA            ((UINT32)0x03 << 30)
    ///[29:28]
    #define IB_OPA_20UA             ((UINT32)0x00 << 28)
    #define IB_OPA_15UA             ((UINT32)0x01 << 28)
    #define IB_OPA_10UA             ((UINT32)0x02 << 28)
    #define IB_OPA_05UA             ((UINT32)0x03 << 28)
    ///[27:26]
    #define IB_PWMBUF_20UA          ((UINT32)0x00 << 26)
    #define IB_PWMBUF_15UA          ((UINT32)0x01 << 26)
    #define IB_PWMBUF_10UA          ((UINT32)0x02 << 26)
    #define IB_PWMBUF_05UA          ((UINT32)0x03 << 26)
    ///[25:24]
    #define IB_STEP_20UA            ((UINT32)0x00 << 24)
    #define IB_STEP_15UA            ((UINT32)0x01 << 24)
    #define IB_STEP_10UA            ((UINT32)0x02 << 24)
    #define IB_STEP_05UA            ((UINT32)0x03 << 24)
    ///[20]
    #define AC_OFF_PWRDET           ((UINT32)0x0 << 20)
    ///[10:8]
    #define RAMP_STEP_53MV          ((UINT32)0x00 << 8)
    #define RAMP_STEP_112MV         ((UINT32)0x01 << 8)
    #define RAMP_STEP_170MV         ((UINT32)0x02 << 8)
    #define RAMP_STEP_226MV         ((UINT32)0x03 << 8)
    #define RAMP_STEP_284MV         ((UINT32)0x04 << 8)
    #define RAMP_STEP_340MV         ((UINT32)0x05 << 8)
    #define RAMP_STEP_398MV         ((UINT32)0x06 << 8)
    #define RAMP_STEP_453MV         ((UINT32)0x07 << 8)
    ///[6:4]
    #define RAMP_STEP_ST_53MV       ((UINT32)0x00 << 4)
    #define RAMP_STEP_ST_112MV      ((UINT32)0x01 << 4)
    #define RAMP_STEP_ST_170MV      ((UINT32)0x02 << 4)
    #define RAMP_STEP_ST_226MV      ((UINT32)0x03 << 4)
    #define RAMP_STEP_ST_284MV      ((UINT32)0x04 << 4)
    #define RAMP_STEP_ST_340MV      ((UINT32)0x05 << 4)
    #define RAMP_STEP_ST_398MV      ((UINT32)0x06 << 4)
    #define RAMP_STEP_ST_453MV      ((UINT32)0x07 << 4)
    ///[2:0]
    #define CKF_DIV_512             ((UINT32)0x00 << 0)
    #define CKF_DIV_1024            ((UINT32)0x01 << 0)
    #define CKF_DIV_2048            ((UINT32)0x02 << 0)
    #define CKF_DIV_4096            ((UINT32)0x03 << 0)
    #define CKF_DIV_8192            ((UINT32)0x04 << 0)
    #define CKF_DIV_16384           ((UINT32)0x05 << 0)
    #define CKF_DIV_32768           ((UINT32)0x06 << 0)
    #define CKF_DIV_65536           ((UINT32)0x07 << 0)

#define REG_ADAC_CFG5           0x0914
    ///[30:28]
    #define CKF_ST_DIV_512             ((UINT32)0x00 << 28)
    #define CKF_ST_DIV_1024            ((UINT32)0x01 << 28)
    #define CKF_ST_DIV_2048            ((UINT32)0x02 << 28)
    #define CKF_ST_DIV_4096            ((UINT32)0x03 << 28)
    #define CKF_ST_DIV_8192            ((UINT32)0x04 << 28)
    #define CKF_ST_DIV_16384           ((UINT32)0x05 << 28)
    #define CKF_ST_DIV_32768           ((UINT32)0x06 << 28)
    #define CKF_ST_DIV_65536           ((UINT32)0x07 << 28)
    ///[21:16]
    #define AL0_GPI_EN                  ((UINT32)0x1 << 16)
    #define AR0_GPI_EN                  ((UINT32)0x1 << 17)
    #define AL1_GPI_EN                  ((UINT32)0x1 << 18)
    #define AR1_GPI_EN                  ((UINT32)0x1 << 19)
    #define AL2_GPI_EN                  ((UINT32)0x1 << 20)
    #define AR2_GPI_EN                  ((UINT32)0x1 << 21)
    ///[1]
    #define RAMP_INV_DIS                ((UINT32)0x1 << 1)
    ///[0]
    #define DAC_DESENSE_CH1             ((UINT32)0x1 << 0)

#define REG_ADAC_CFG6           0x0918
    ///[31]
    #define ACOFF_CKT_CH0               ((UINT32)0x1 << 31)
    ///[30]
    #define ACOFF_CKT_CH1               ((UINT32)0x1 << 30)
    ///[25]
    #define RAMP_OUT_EN                 ((UINT32)0x1 << 25)
    ///[0]
    #define DAC_DESENSE_CH0             ((UINT32)0x1 << 16)
#elif defined(CC_MT5882)
#define REG_ADAC_CFG0           0x0a00
    ///[29:24]
    #define APLL_CLK_APLL1      ((UINT32)0x00 << 24)
    #define APLL_CLK_APLL2      ((UINT32)0x3F << 24)
    ///[21:16]
    #define CLK_SEL_APLL        ((UINT32)0x00 << 16)
    #define CLK_SEL_432M        ((UINT32)0x3F << 16)
    ///[13:8]
    #define CLK_NORMAL          ((UINT32)0x00 << 8)
    #define CLK_INVERSE         ((UINT32)0x3F << 8)
    ///[5:0]
    #define RELATCH_DIS         ((UINT32)0x00 << 0)
    #define RELATCH_EN          ((UINT32)0x3F << 8)

#define REG_ADAC_CFG1           0x0a04
    ///[29:24]
    #define PS_APLL_CLK_APLL1      ((UINT32)0x00 << 24)
    #define PS_APLL_CLK_APLL2      ((UINT32)0x3F << 24)
    ///[21:16]
    #define PS_CLK_SEL_APLL        ((UINT32)0x00 << 16)
    #define PS_CLK_SEL_432M        ((UINT32)0x3F << 16)
    ///[13:8]
    #define PS_CLK_NORMAL          ((UINT32)0x00 << 8)
    #define PS_CLK_INVERSE         ((UINT32)0x3F << 8)
    ///[5:0]
    #define PS_RELATCH_DIS         ((UINT32)0x00 << 0)
    #define PS_RELATCH_EN          ((UINT32)0x3F << 8)

#define REG_ADAC_CFG2           0x0a08
    #define ACT_UP                 ((UINT32)0x1 << 16)
    #define ACT_ENDP_CH2           ((UINT32)0x1 << 11)
    #define ACT_ENVO_CH2           ((UINT32)0x1 << 10)
    #define ACT_SLEW_CH2           ((UINT32)0x1 << 9)
    #define ACT_PLAY_CH2           ((UINT32)0x1 << 8)
    #define ACT_ENDP_CH1           ((UINT32)0x1 << 7)
    #define ACT_ENVO_CH1           ((UINT32)0x1 << 6)
    #define ACT_SLEW_CH1           ((UINT32)0x1 << 5)
    #define ACT_PLAY_CH1           ((UINT32)0x1 << 4)
    #define ACT_ENDP_CH0           ((UINT32)0x1 << 3)
    #define ACT_ENVO_CH0           ((UINT32)0x1 << 2)
    #define ACT_SLEW_CH0           ((UINT32)0x1 << 1)
    #define ACT_PLAY_CH0           ((UINT32)0x1 << 0)

#define REG_ADAC_CFG3           0x0a0C
    #define DAC_HIZ                ((UINT32)0x1 << 24)
    #define DAC_DFC_CH2            ((UINT32)0x1 << 22)
    #define DAC_DFC_CH1            ((UINT32)0x1 << 21)
    #define DAC_DFC_CH0            ((UINT32)0x1 << 20)
    #define DAC_INV_CH2            ((UINT32)0x1 << 18)
    #define DAC_INV_CH1            ((UINT32)0x1 << 17)
    #define DAC_INV_CH0            ((UINT32)0x1 << 16)
    ///[9:8]
    #define DAC_FS_CH2_P36         ((UINT32)0x00 << 8)
    #define DAC_FS_CH2_P40         ((UINT32)0x01 << 8)
    #define DAC_FS_CH2_P42         ((UINT32)0x02 << 8)
    #define DAC_FS_CH2_P45         ((UINT32)0x03 << 8)
    ///[5:4]
    #define DAC_FS_CH1_P36         ((UINT32)0x00 << 4)
    #define DAC_FS_CH1_P40         ((UINT32)0x01 << 4)
    #define DAC_FS_CH1_P42         ((UINT32)0x02 << 4)
    #define DAC_FS_CH1_P45         ((UINT32)0x03 << 4)
    ///[1:0]
    #define DAC_FS_CH0_P36         ((UINT32)0x00 << 0)
    #define DAC_FS_CH0_P40         ((UINT32)0x01 << 0)
    #define DAC_FS_CH0_P42         ((UINT32)0x02 << 0)
    #define DAC_FS_CH0_P45         ((UINT32)0x03 << 0)

#define REG_ADAC_CFG4           0x0a10
    ///[31:30]
    #define IB_PDET_20UA            ((UINT32)0x00 << 30)
    #define IB_PDET_15UA            ((UINT32)0x01 << 30)
    #define IB_PDET_10UA            ((UINT32)0x02 << 30)
    #define IB_PDET_05UA            ((UINT32)0x03 << 30)
    ///[29:28]
    #define IB_OPA_20UA             ((UINT32)0x00 << 28)
    #define IB_OPA_15UA             ((UINT32)0x01 << 28)
    #define IB_OPA_10UA             ((UINT32)0x02 << 28)
    #define IB_OPA_05UA             ((UINT32)0x03 << 28)
    ///[27:26]
    #define IB_PWMBUF_20UA          ((UINT32)0x00 << 26)
    #define IB_PWMBUF_15UA          ((UINT32)0x01 << 26)
    #define IB_PWMBUF_10UA          ((UINT32)0x02 << 26)
    #define IB_PWMBUF_05UA          ((UINT32)0x03 << 26)
    ///[25:24]
    #define IB_STEP_20UA            ((UINT32)0x00 << 24)
    #define IB_STEP_15UA            ((UINT32)0x01 << 24)
    #define IB_STEP_10UA            ((UINT32)0x02 << 24)
    #define IB_STEP_05UA            ((UINT32)0x03 << 24)
    ///[20]
    #define AC_OFF_PWRDET           ((UINT32)0x0 << 20)
    ///[10:8]
    #define RAMP_STEP_53MV          ((UINT32)0x00 << 8)
    #define RAMP_STEP_112MV         ((UINT32)0x01 << 8)
    #define RAMP_STEP_170MV         ((UINT32)0x02 << 8)
    #define RAMP_STEP_226MV         ((UINT32)0x03 << 8)
    #define RAMP_STEP_284MV         ((UINT32)0x04 << 8)
    #define RAMP_STEP_340MV         ((UINT32)0x05 << 8)
    #define RAMP_STEP_398MV         ((UINT32)0x06 << 8)
    #define RAMP_STEP_453MV         ((UINT32)0x07 << 8)
    ///[6:4]
    #define RAMP_STEP_ST_53MV       ((UINT32)0x00 << 4)
    #define RAMP_STEP_ST_112MV      ((UINT32)0x01 << 4)
    #define RAMP_STEP_ST_170MV      ((UINT32)0x02 << 4)
    #define RAMP_STEP_ST_226MV      ((UINT32)0x03 << 4)
    #define RAMP_STEP_ST_284MV      ((UINT32)0x04 << 4)
    #define RAMP_STEP_ST_340MV      ((UINT32)0x05 << 4)
    #define RAMP_STEP_ST_398MV      ((UINT32)0x06 << 4)
    #define RAMP_STEP_ST_453MV      ((UINT32)0x07 << 4)
    ///[2:0]
    #define CKF_DIV_512             ((UINT32)0x00 << 0)
    #define CKF_DIV_1024            ((UINT32)0x01 << 0)
    #define CKF_DIV_2048            ((UINT32)0x02 << 0)
    #define CKF_DIV_4096            ((UINT32)0x03 << 0)
    #define CKF_DIV_8192            ((UINT32)0x04 << 0)
    #define CKF_DIV_16384           ((UINT32)0x05 << 0)
    #define CKF_DIV_32768           ((UINT32)0x06 << 0)
    #define CKF_DIV_65536           ((UINT32)0x07 << 0)

#define REG_ADAC_CFG5           0x0a14
    ///[30:28]
    #define CKF_ST_DIV_512             ((UINT32)0x00 << 28)
    #define CKF_ST_DIV_1024            ((UINT32)0x01 << 28)
    #define CKF_ST_DIV_2048            ((UINT32)0x02 << 28)
    #define CKF_ST_DIV_4096            ((UINT32)0x03 << 28)
    #define CKF_ST_DIV_8192            ((UINT32)0x04 << 28)
    #define CKF_ST_DIV_16384           ((UINT32)0x05 << 28)
    #define CKF_ST_DIV_32768           ((UINT32)0x06 << 28)
    #define CKF_ST_DIV_65536           ((UINT32)0x07 << 28)
    ///[21:16]
    #define AL0_GPI_EN                  ((UINT32)0x1 << 16)
    #define AR0_GPI_EN                  ((UINT32)0x1 << 17)
    #define AL1_GPI_EN                  ((UINT32)0x1 << 18)
    #define AR1_GPI_EN                  ((UINT32)0x1 << 19)
    #define AL2_GPI_EN                  ((UINT32)0x1 << 20)
    #define AR2_GPI_EN                  ((UINT32)0x1 << 21)
    ///[1]
    #define RAMP_INV_DIS                ((UINT32)0x1 << 1)
    ///[0]
    #define DAC_DESENSE_CH1             ((UINT32)0x1 << 0)

#define REG_ADAC_CFG6           0x0a18
    ///[31]
    #define ACOFF_CKT_CH0               ((UINT32)0x1 << 31)
    ///[30]
    #define ACOFF_CKT_CH1               ((UINT32)0x1 << 30)
    ///[25]
    #define RAMP_OUT_EN                 ((UINT32)0x1 << 25)
    ///[0]
    #define DAC_DESENSE_CH0             ((UINT32)0x1 << 16)    
#elif defined(CC_MT5890) 
#define REG_ADAC_CFG0           0x0900
        ///[29:24]
    #define APLL_CLK_APLL1      ((UINT32)0x00 << 24)
    #define APLL_CLK_APLL2      ((UINT32)0x3F << 24)
        ///[21:16]
    #define CLK_SEL_APLL        ((UINT32)0x00 << 16)
    #define CLK_SEL_432M        ((UINT32)0x3F << 16)
        ///[13:8]
    #define CLK_NORMAL          ((UINT32)0x00 << 8)
    #define CLK_INVERSE         ((UINT32)0x3F << 8)
        ///[5:0]
    #define RELATCH_DIS         ((UINT32)0x00 << 0)
    #define RELATCH_EN          ((UINT32)0x3F << 8)
    
#define REG_ADAC_CFG1           0x0904
        ///[29:24]
    #define PS_APLL_CLK_APLL1      ((UINT32)0x00 << 24)
    #define PS_APLL_CLK_APLL2      ((UINT32)0x3F << 24)
        ///[21:16]
    #define PS_CLK_SEL_APLL        ((UINT32)0x00 << 16)
    #define PS_CLK_SEL_432M        ((UINT32)0x3F << 16)
        ///[13:8]
    #define PS_CLK_NORMAL          ((UINT32)0x00 << 8)
    #define PS_CLK_INVERSE         ((UINT32)0x3F << 8)
        ///[5:0]
    #define PS_RELATCH_DIS         ((UINT32)0x00 << 0)
    #define PS_RELATCH_EN          ((UINT32)0x3F << 8)
    
#define REG_ADAC_CFG2           0x0908
    #define ACT_UP                 ((UINT32)0x1 << 16)
    //#define ACT_ENDP_CH3           ((UINT32)0x1 << 15)
    //#define ACT_ENVO_CH3           ((UINT32)0x1 << 14)
    //#define ACT_SLEW_CH3           ((UINT32)0x1 << 13)
    //#define ACT_PLAY_CH3           ((UINT32)0x1 << 12)
    #define ACT_ENDP_CH2           ((UINT32)0x1 << 11)
    #define ACT_ENVO_CH2           ((UINT32)0x1 << 10)
    #define ACT_SLEW_CH2           ((UINT32)0x1 << 9)
    #define ACT_PLAY_CH2           ((UINT32)0x1 << 8)
    #define ACT_ENDP_CH1           ((UINT32)0x1 << 7)
    #define ACT_ENVO_CH1           ((UINT32)0x1 << 6)
    #define ACT_SLEW_CH1           ((UINT32)0x1 << 5)
    #define ACT_PLAY_CH1           ((UINT32)0x1 << 4)
    #define ACT_ENDP_CH0           ((UINT32)0x1 << 3)
    #define ACT_ENVO_CH0           ((UINT32)0x1 << 2)
    #define ACT_SLEW_CH0           ((UINT32)0x1 << 1)
    #define ACT_PLAY_CH0           ((UINT32)0x1 << 0)
    
#define REG_ADAC_CFG3           0x090C
    #define DAC_HIZ_EN             ((UINT32)0x1 << 28)
    #define DAC_HIZ_DIS            ((UINT32)0x0 << 28)
#define REG_ADAC_CFG4           0x0910
    #define DAC_OC_DIS             ((UINT32)0x0 << 28)
    #define DAC_OC_EN              ((UINT32)0x1 << 28)
    #define DAC_OC_HIGH_SEL10      ((UINT32)0x02 << 24)//default value
    #define DAC_OC_HIGH_SEL00      ((UINT32)0x00 << 24)
    #define DAC_OC_LOW_SEL10       ((UINT32)0x02 << 20)//default value
    #define DAC_OC_LOW_SEL11       ((UINT32)0x03 << 20)

    #define DAC_DFC_CH2            ((UINT32)0x1 << 6)
    #define DAC_DFC_CH1            ((UINT32)0x1 << 5)
    #define DAC_DFC_CH0            ((UINT32)0x1 << 4)
    #define DAC_INV_CH2            ((UINT32)0x1 << 2)
    #define DAC_INV_CH1            ((UINT32)0x1 << 1)
    #define DAC_INV_CH0            ((UINT32)0x1 << 0)
       
#define REG_ADAC_CFG5           0x0914
    ///[25:24]
    #define DAC_FS_CH3_P36         ((UINT32)0x00 << 28)
    #define DAC_FS_CH3_P40         ((UINT32)0x01 << 28)
    #define DAC_FS_CH3_P42         ((UINT32)0x02 << 28)
    #define DAC_FS_CH3_P45         ((UINT32)0x03 << 28)

    ///[25:24]
    #define DAC_FS_CH2_P36         ((UINT32)0x00 << 24)
    #define DAC_FS_CH2_P40         ((UINT32)0x01 << 24)
    #define DAC_FS_CH2_P42         ((UINT32)0x02 << 24)
    #define DAC_FS_CH2_P45         ((UINT32)0x03 << 24)
        ///[21:20]
    #define DAC_FS_CH1_P36         ((UINT32)0x00 << 20)
    #define DAC_FS_CH1_P40         ((UINT32)0x01 << 20)
    #define DAC_FS_CH1_P42         ((UINT32)0x02 << 20)
    #define DAC_FS_CH1_P45         ((UINT32)0x03 << 20)
        ///[17:16]
    #define DAC_FS_CH0_P36         ((UINT32)0x00 << 16)
    #define DAC_FS_CH0_P40         ((UINT32)0x01 << 16)
    #define DAC_FS_CH0_P42         ((UINT32)0x02 << 16)
    #define DAC_FS_CH0_P45         ((UINT32)0x03 << 16)

     ///[11:10]
    #define DAC_IB_IBGEN_20UA      ((UINT32)0x00 << 10)//have not update to driver,need to study application note
    #define DAC_IB_IBGEN_15UA      ((UINT32)0x01 << 10)//have not update to driver,need to study application note
    #define DAC_IB_IBGEN_10UA      ((UINT32)0x02 << 10)//have not update to driver,need to study application note
    #define DAC_IB_IBGEN_5UA       ((UINT32)0x03 << 10)//have not update to driver,need to study application note

    ///[9:8]
    #define IB_PDET_20UA           ((UINT32)0x00 << 8)
    #define IB_PDET_15UA           ((UINT32)0x01 << 8)
    #define IB_PDET_10UA           ((UINT32)0x02 << 8)
    #define IB_PDET_05UA           ((UINT32)0x03 << 8)

     ///[7:6]
    #define IB_HPA_20UA             ((UINT32)0x00 << 6)
    #define IB_HPA_15UA             ((UINT32)0x01 << 6)
    #define IB_HPA_10UA             ((UINT32)0x02 << 6)
    #define IB_HPA_05UA             ((UINT32)0x03 << 6)

    ///[5:4]
    #define IB_LOA_20UA             ((UINT32)0x00 << 4)
    #define IB_LOA_15UA             ((UINT32)0x01 << 4)
    #define IB_LOA_10UA             ((UINT32)0x02 << 4)
    #define IB_LOA_05UA             ((UINT32)0x03 << 4)
    ///[3:2]
    #define IB_PWMBUF_20UA          ((UINT32)0x00 << 2)
    #define IB_PWMBUF_15UA          ((UINT32)0x01 << 2)
    #define IB_PWMBUF_10UA          ((UINT32)0x02 << 2)
    #define IB_PWMBUF_05UA          ((UINT32)0x03 << 2)
    ///[1:0]
    #define IB_STEP_20UA            ((UINT32)0x00 << 0)
    #define IB_STEP_15UA            ((UINT32)0x01 << 0)
    #define IB_STEP_10UA            ((UINT32)0x02 << 0)
    #define IB_STEP_05UA            ((UINT32)0x03 << 0)
    
#define REG_ADAC_CFG6           0x0918
    #define AC_OFF_PWRDET           ((UINT32)0x1 << 28)

    ///[27:24] Always=0000
    //#define RG_OFFTH_D           ((UINT32)0x1 << 26)
    //#define RG_OFFTH_A           ((UINT32)0x1 << 24)

    ///[20] always=0
    //#define RG_HB_OFF           ((UINT32)0x1 << 20)
    
        ///[18:16]
    #define RAMP_STEP_53MV          ((UINT32)0x00 << 16)
    #define RAMP_STEP_112MV         ((UINT32)0x01 << 16)
    #define RAMP_STEP_170MV         ((UINT32)0x02 << 16)
    #define RAMP_STEP_226MV         ((UINT32)0x03 << 16)
    #define RAMP_STEP_284MV         ((UINT32)0x04 << 16)
    #define RAMP_STEP_340MV         ((UINT32)0x05 << 16)
    #define RAMP_STEP_398MV         ((UINT32)0x06 << 16)
    #define RAMP_STEP_453MV         ((UINT32)0x07 << 16)
        ///[14:12]
    #define RAMP_STEP_ST_53MV       ((UINT32)0x00 << 12)
    #define RAMP_STEP_ST_112MV      ((UINT32)0x01 << 12)
    #define RAMP_STEP_ST_170MV      ((UINT32)0x02 << 12)
    #define RAMP_STEP_ST_226MV      ((UINT32)0x03 << 12)
    #define RAMP_STEP_ST_284MV      ((UINT32)0x04 << 12)
    #define RAMP_STEP_ST_340MV      ((UINT32)0x05 << 12)
    #define RAMP_STEP_ST_398MV      ((UINT32)0x06 << 12)
    #define RAMP_STEP_ST_453MV      ((UINT32)0x07 << 12)
        ///[10:8]
    #define CKF_DIV_512             ((UINT32)0x00 << 8)
    #define CKF_DIV_1024            ((UINT32)0x01 << 8)
    #define CKF_DIV_2048            ((UINT32)0x02 << 8)
    #define CKF_DIV_4096            ((UINT32)0x03 << 8)
    #define CKF_DIV_8192            ((UINT32)0x04 << 8)
    #define CKF_DIV_16384           ((UINT32)0x05 << 8)
    #define CKF_DIV_32768           ((UINT32)0x06 << 8)
    #define CKF_DIV_65536           ((UINT32)0x07 << 8)
    
       ///[6:4]
    #define CKF_ST_DIV_512             ((UINT32)0x00 << 4)
    #define CKF_ST_DIV_1024            ((UINT32)0x01 << 4)
    #define CKF_ST_DIV_2048            ((UINT32)0x02 << 4)
    #define CKF_ST_DIV_4096            ((UINT32)0x03 << 4)
    #define CKF_ST_DIV_8192            ((UINT32)0x04 << 4)
    #define CKF_ST_DIV_16384           ((UINT32)0x05 << 4)
    #define CKF_ST_DIV_32768           ((UINT32)0x06 << 4)
    #define CKF_ST_DIV_65536           ((UINT32)0x07 << 4)

#define REG_ADAC_CFG7           0x091C
    #define DAC_DESENSE_CH3             ((UINT32)0x1 << 31)
    #define DAC_DESENSE_CH2             ((UINT32)0x1 << 30)
    #define DAC_DESENSE_CH1             ((UINT32)0x1 << 29)
    #define DAC_DESENSE_CH0             ((UINT32)0x1 << 28)

#define REG_ADAC_CFG8           0x0920
    #define RAMP_OUT_MON                ((UINT32)0x01 << 24)
#define REG_ADAC_CFG9           0x0924
    #define DAC_432M_EN                 ((UINT32)0x1 << 16)
    #define DAC_HADDS1_EN               ((UINT32)0x1 << 17)


#define REG_ADAC_CFG10           0x0928
    #define DAC_RAMP_R2R_EN             ((UINT32)0x01 << 0)
    #define DAC_RAMP_R2R_DIS            ((UINT32)0x00 << 0)
    #define DAC_RAMP_DELAY_EN           ((UINT32)0x01 << 1)
    #define DAC_RAMP_DELAY_DSI          ((UINT32)0x00 << 1)
    #define DAC_HPA_BUFFER_ST           ((UINT32)0x01 << 3)
    #define DAC_RAMP2HPA                ((UINT32)0x01 << 5)
    #define DAC_RAMP2HPA_BUFFER         ((UINT32)0x01 << 6)
        ///[1]
    //#define RAMP_INV_DIS                ((UINT32)0x1 << 1)

    
//#define REG_ADAC_CFG6           0x0918
        ///[31]
    //#define ACOFF_CKT_CH0               ((UINT32)0x1 << 31)
        ///[30]
    //#define ACOFF_CKT_CH1               ((UINT32)0x1 << 30)
        ///[25]
    //#define RAMP_OUT_EN                 ((UINT32)0x1 << 25)


#endif

/// To Do
/// PWMDAC_CFGX need remove at CC_MT5881 case

#if defined(CC_MT5880) || defined(CC_MT5881) // may be  fixed, paul
#define REG_PWMDAC_CFG0         0x0100
#define REG_PWMDAC_CFG1         0x0104
#define REG_PWMDAC_CFG2         0x0108
#else
#define REG_PWMDAC_CFG0         0x0900     // <=> 0x51ec before 5365/66/95
    //[31:24] RG_APLL_CLK_SEL
    //[23:16] RG_CLK_432M_SEL
    //[15:8]  RG_APLL_CLK_INV
    //[7:0]   RG_PWM_RELATCH_SEL
#define REG_PWMDAC_CFG1         0x0904
    //[31:24] RG_GPIO_PWM_E4
    //[23:16] RG_GPIO_PWM_E8
    //[15:8]  RG_GPIO_PWM_PD
    //[7:0]   RG_GPIO_WPM_PU
#define REG_PWMDAC_CFG2         0x0908
#endif
    #define AL0_CAB_00          ((UINT32)0x0 << 13)
    #define AL0_CAB_01          ((UINT32)0x1 << 13)
    #define AL0_CAB_02          ((UINT32)0x2 << 13)
    #define AL0_CAB_03          ((UINT32)0x3 << 13)
    #define AL0_ILDO_EN         ((UINT32)0x1 << 12)
    #define LDO_CAB_CUR_250U    ((UINT32)0 << 9)
    #define LDO_CAB_CUR_1000U    ((UINT32)1 << 9)
    #define LDO_CAB_CUR_1250U    ((UINT32)2 << 9)
    #define LDO_CAB_CUR_2000U    ((UINT32)3 << 9)
    #define LDO_CAB_CUR_2250U    ((UINT32)4 << 9)
    #define LDO_CAB_CUR_3000U    ((UINT32)5 << 9)
    #define LDO_CAB_CUR_3250U    ((UINT32)6 << 9)
    #define LDO_CAB_CUR_4000U    ((UINT32)7 << 9)
    #define AR0_CAB_00          ((UINT32)0x0 << 5)
    #define AR0_CAB_01          ((UINT32)0x1 << 5)
    #define AR0_CAB_02          ((UINT32)0x2 << 5)
    #define AR0_CAB_03          ((UINT32)0x3 << 5)
    #define AR0_ILDO_EN         ((UINT32)0x1 << 4)

    //[31:24] RG_GPIO_PWM_SR
    //[23:16] RG_GPIO_PWM_SMT
#if defined(CC_MT5880) || defined(CC_MT5881) // may be fixed, paul
#define REG_PWMDAC_CFG3         0x010C
#else
#define REG_PWMDAC_CFG3         0x090C
#endif
    #define AL1_CAB_00          ((UINT32)0x0 << 29)
    #define AL1_CAB_01          ((UINT32)0x1 << 29)
    #define AL1_CAB_02          ((UINT32)0x2 << 29)
    #define AL1_CAB_03          ((UINT32)0x3 << 29)
    #define AL1_ILDO_EN         ((UINT32)0x1 << 28)
    #define AR1_CAB_00          ((UINT32)0x0 << 21)
    #define AR1_CAB_01          ((UINT32)0x1 << 21)
    #define AR1_CAB_02          ((UINT32)0x2 << 21)
    #define AR1_CAB_03          ((UINT32)0x3 << 21)
    #define AR1_ILDO_EN         ((UINT32)0x1 << 20)
    #define AL2_CAB_00          ((UINT32)0x0 << 13)
    #define AL2_CAB_01          ((UINT32)0x1 << 13)
    #define AL2_CAB_02          ((UINT32)0x2 << 13)
    #define AL2_CAB_03          ((UINT32)0x3 << 13)
    #define AL2_ILDO_EN         ((UINT32)0x1 << 12)
    #define AR2_CAB_00          ((UINT32)0x0 << 5)
    #define AR2_CAB_01          ((UINT32)0x1 << 5)
    #define AR2_CAB_02          ((UINT32)0x2 << 5)
    #define AR2_CAB_03          ((UINT32)0x3 << 5)
    #define AR2_ILDO_EN         ((UINT32)0x1 << 4)

#if defined(CC_MT5880) || defined(CC_MT5881) // may be fixed, paul
#define REG_PWMDAC_CFG4         0x0110
#else
#define REG_PWMDAC_CFG4         0x0910
#endif
    #define V_TO_I_DIS          ((UINT32)0x1 << 31)
    #define V_TO_I_EN           ((UINT32)0x0 << 31)
    #define CUR_MORR_EN         ((UINT32)0x1 << 30)
    #define REG_1P2V_DIS        ((UINT32)0x1 << 29)
    #define REG_1P2V_EN         ((UINT32)0x0 << 29)
    #define VREF_DBG_EN         ((UINT32)0x1 << 28)
    #define I_10UA              ((UINT32)0x1 << 27)
    #define I_5UA               ((UINT32)0x0 << 27)
    #define ALR0_CAP_P25PF       ((UINT32)0x0 << 22)
    #define ALR0_CAP_P36PF       ((UINT32)0x1 << 22)
    #define ALR0_CAP_P60PF       ((UINT32)0x2 << 22)
    #define ALR0_CAP_P73PF       ((UINT32)0x3 << 22)
    #define ALR1_CAP_P25PF       ((UINT32)0x0 << 20)
    #define ALR1_CAP_P36PF       ((UINT32)0x1 << 20)
    #define ALR1_CAP_P60PF       ((UINT32)0x2 << 20)
    #define ALR1_CAP_P73PF       ((UINT32)0x3 << 20)
    #define ALR2_CAP_P25PF       ((UINT32)0x0 << 18)
    #define ALR2_CAP_P36PF       ((UINT32)0x1 << 18)
    #define ALR2_CAP_P60PF       ((UINT32)0x2 << 18)
    #define ALR2_CAP_P73PF       ((UINT32)0x3 << 18)
    #define ALR0_1K_MODE        ((UINT32)0x1 << 17)
    #define ALR1_1K_MODE        ((UINT32)0x1 << 16)
    #define ALR2_1K_MODE        ((UINT32)0x1 << 15)

#if defined(CC_MT5880) || defined(CC_MT5881) // may be fixed, paul
#define REG_PWMDAC_CFG5         0x0114
#else
#define REG_PWMDAC_CFG5         0x0914
#endif

#if defined(CC_MT5881)
#define REG_PWMDAC_CFG6         0x0118
#elif defined(CC_MT5399) || defined(CC_MT5890) || defined(CC_MT5882)
#define REG_PWMDAC_CFG6         0x0918
#else ///defined(CC_MT5880)
#define REG_PWMDAC_CFG6         0x0118
    #define RG_GPIO_PWM_G           ((UINT32)0x3C << 26) //[31:26]
    #define RG_HB_ENDP_EN           ((UINT32)0x3 << 19) //[20:19]
    #define RG_HB_ENDP_DIS          ((UINT32)0x0 << 19) ///CC_AUD_HP_DEPOP, ///For Remy, need remove "CC_DAC_DE_POP".
    #define RG_HB_ENVO_EN           ((UINT32)0x3 << 17) //[18:17]
    #define RG_HB_ENVO_DIS          ((UINT32)0x0 << 17)
    #define RG_HB_SLEW_EN           ((UINT32)0x3 << 15) //[16:15]
    #define RG_HB_SLEW_DIS          ((UINT32)0x0 << 15)
    #define RG_HB_PLAY_EN           ((UINT32)0x3 << 13) //[14:13]
    #define RG_HB_PLAY_DIS          ((UINT32)0x0 << 13)
    #define RG_HB_UP_EN             ((UINT32)0x1 << 9) //[9]
    #define RG_HB_UP_DIS            ((UINT32)0x0 << 9)  //[9]
    #define RG_HB_DFC_EN            ((UINT32)0x3 << 7) //[8:7]
    #define RG_HB_DFC_DIS           ((UINT32)0x0 << 7)
    #define RG_HB_INV_EN            ((UINT32)0x3 << 5)  //[6:5]
    #define RG_HB_INV_DIS           ((UINT32)0x0 << 5)
    #define RG_HB_FS_1_036V         ((UINT32)0x0 << 3) //[4:3]
    #define RG_HB_FS_1_04V          ((UINT32)0x1 << 3)
    #define RG_HB_FS_1_042V         ((UINT32)0x2 << 3)
    #define RG_HB_FS_1_045V         ((UINT32)0x3 << 3)
    #define RG_HB_FS_0_036V         ((UINT32)0x0 << 1) //[2:1]
    #define RG_HB_FS_0_04V          ((UINT32)0x1 << 1)
    #define RG_HB_FS_0_042V         ((UINT32)0x2 << 1)
    #define RG_HB_FS_0_045V         ((UINT32)0x3 << 1)
#define REG_PWMDAC_CFG7         0x011c
    #define RG_HB_HIZ_EN            ((UINT32)0x1 << 28) //[28]
    #define RG_HB_HIZ_DIS           ((UINT32)0x0 << 28)
    #define RG_HB_IB_PDET_20U       ((UINT32)0x0 << 26) //[27:26]
    #define RG_HB_IB_PDET_15U       ((UINT32)0x1 << 26)
    #define RG_HB_IB_PDET_10U       ((UINT32)0x2 << 26)
    #define RG_HB_IB_PDET_5U        ((UINT32)0x3 << 26)
    #define RG_HB_IB_OPA_20U        ((UINT32)0x0 << 24) //[25:24]
    #define RG_HB_IB_OPA_15U        ((UINT32)0x1 << 24)
    #define RG_HB_IB_OPA_10U        ((UINT32)0x2 << 24)
    #define RG_HB_IB_OPA_5U         ((UINT32)0x3 << 24)
    #define RG_HB_IB_PWMBUF_20U     ((UINT32)0x0 << 22) //[23:22]
    #define RG_HB_IB_PWMBUF_15U     ((UINT32)0x1 << 22)
    #define RG_HB_IB_PWMBUF_10U     ((UINT32)0x2 << 22)
    #define RG_HB_IB_PWMBUF_5U      ((UINT32)0x3 << 22)
    #define RG_HB_IB_STEP_20U       ((UINT32)0x0 << 20) //[21:20]
    #define RG_HB_IB_STEP_15U       ((UINT32)0x1 << 20)
    #define RG_HB_IB_STEP_10U       ((UINT32)0x2 << 20)
    #define RG_HB_IB_STEP_5U        ((UINT32)0x3 << 20)
    #define RG_HB_ENPWRDET_EN       ((UINT32)0x1 << 16) //[16]
    #define RG_HB_OFF_EN            ((UINT32)0x1 << 8) //[8]
    #define RG_HB_OFF_DIS           ((UINT32)0x0 << 8)
    #define RG_HB_STEP_53M          ((UINT32)0x0 << 5)  //[7:5]
    #define RG_HB_STEP_112M         ((UINT32)0x1 << 5)
    #define RG_HB_STEP_398M         ((UINT32)0x6 << 5)
    #define RG_HB_STEP_452M         ((UINT32)0x7 << 5)
    #define RG_HB_STEP_ST_53M       ((UINT32)0x0 << 2)  //[4:2]
    #define RG_HB_STEP_ST_112M      ((UINT32)0x1 << 2)
    #define RG_HB_STEP_ST_398M      ((UINT32)0x6 << 2)
    #define RG_HB_STEP_ST_452M      ((UINT32)0x7 << 2)
#define REG_PWMDAC_CFG8         0x0120
    #define RG_HB_CKF_512           ((UINT32)0x0 << 29) //[31:29]
    #define RG_HB_CKF_2048          ((UINT32)0x2 << 29) //[31:29]
    #define RG_HB_CKF_4096          ((UINT32)0x3 << 29)
    #define RG_HB_CKF_8192          ((UINT32)0x4 << 29)
    #define RG_HB_CKF_16384         ((UINT32)0x5 << 29)
    #define RG_HB_CKF_32768         ((UINT32)0x6 << 29)
    #define RG_HB_CKF_65536         ((UINT32)0x7 << 29)
    #define RG_HB_CKF_ST_2048       ((UINT32)0x2 << 26) //[28:26]
    #define RG_HB_CKF_ST_4096       ((UINT32)0x3 << 26)
    #define RG_HB_CKF_ST_8192       ((UINT32)0x4 << 26)
    #define RG_HB_CKF_ST_16384      ((UINT32)0x5 << 26)
    #define RG_HB_CKF_ST_32768      ((UINT32)0x6 << 26)
    #define RG_HB_CKF_ST_65536      ((UINT32)0x7 << 26)
    #define RG_HB_STEP_170MV        ((UINT32)0x2 << 5)
    #define RG_HB_STEP_453MV        ((UINT32)0x7 << 5)
    #define RG_HB_STEP_112MV        ((UINT32)0x1 << 5)
#define REG_PWMDAC_CFG9         0x0124
#endif

#if defined(CC_MT5880) || defined(CC_MT5881) // may be fixed, paul    //check the register from hw_vfe.h  0xf0060210...
    #define REG_CVBS_CFG4           0x0210     
    #define REG_CVBS_CFG5           0x0214
    #define REG_CVBS_CFG6           0x0218
    #define REG_CVBS_CFG7           0x021C
#else
    #define REG_CVBS_CFG4           0x0110
    //[31:24] RG_AISEL_2    => 8'b01000000
    //[7] RG_C_SELA_2       => 1
    #define REG_CVBS_CFG5           0x0114
    #define REG_CVBS_CFG6           0x0118
    //[29] RG_SIFA_EN_2     => 1
    #define REG_CVBS_CFG7           0x011C
    //[23] RG_CVBSADC_SEL_CKPLL_2 => 0
    //[21] RG_CVBSADC_PWD_2 => 0
    //[15] RG_CVBS_PWD_2 => 0
    //[13] RG_INMUX_PWD_2 => 0
    //[12] RG_PROT_PWD_2 => 0
    //[11] RG_PGABUFNA_PWD_2 => 0
    //[9] RG_SHIFTA_PWD_2 => 0
    //[7] RG_OFFCURA_PWD_2 => 0
#endif

#ifdef  CC_MT5881 //Remy spdif in
#define REG_SPDIFIN_CFG0            0x0680
    #define SPDIF_RX_ENABLE         (0x1 << 0)      // Enable SPDIFRX
    #define SPDIF_RX_DISABLE        (0x0 << 0)      // Disable SPDIFRX
    #define SPDIF_RX_DATA_FLIP      (0x1 << 1)      // Decoder data flip
    #define SPDIF_RX_INTEN          (0x1 << 6)      // Spdifrx interrupt enable
    #define SPDIF_RX_TIMEOUT_FLAG   (0x1 << 7)      // Spdifrx timeout flag
    #define SPDIF_RX_DECNT          (0x4 << 8)      // Input threshold counter
    #define SPDIF_RX_DESEL          (0x3 << 13)     // 
    #define SPDIF_RX_MAXLEN         (0xa4 << 16)    // Max number sampled by spdifin_clk

#define REG_SPDIFIN_CFG1            0x0684
    #define SPDIF_RX_SEL            (0x1 << 0)      // Select spdifin decoded data into line in buffer for decoder0
    #define SPDIF_RX_SEL_CLK        (0x1 << 1)      // Select spdifin decoded data into line in buffer for decoder1
    #define SPDIF_RX_DEC2_SEL       (0x1 << 2)      // Select spdifin recovery BITCLK and LRCK as LINEIN source for decoder 0
    #define SPDIF_RX_DEC2_DEL_CLK   (0x1 << 3)      // Select spdifin recovery BITCLK and LRCK as LINEIN source for decoder 1
    #define SPDIF_RX_BYPASS         (0x1 << 10)     // Bypass spdifin input to spdif output
    #define SPDIF_RX_LOOPBACK       (0x1 << 14)     // Loop back (spdif out to spdif in), test only
    #define SPDIF_RX_PREAMBLE_ERROR (0x1 << 20)     // Preamble error no matter what the symbol is
    #define SPDIF_RX_TIMEOUT_INT    (0x1 << 28)     // Time out interrupt (plug in stage to plug out detection used)
    #define SPDIF_RX_INVALID_BITCNT (0x1 << 24)     // Decoding error: invalid bitcnd
    #define SPDIF_RX_PARITY_CHKERR  (0x1 << 25)     // Decoding error: parity check error
    
#define REG_SPDIFIN_ROUGH_FREQ      0x06dc
    #define SPDIF_RX_MODE_594M_EN   (0x1 << 17)     // Rough frequency detection bit in system clk =594M mode
    #define SPDIF_RX_MODE_432M_EN   (0x1 << 16)     // Rough frequency detection bit in system clk =432M mode
    #define SPDIF_RX_ROUGH_FREQ     (0xffU << 28)  // Rough frequency index
#define REG_SPDIFIN_FREQ_STATUS     0x06ec
    #define SPDIF_RX_FREQ_IDX       (0xffU << 28)  // The rough frequency preview index

#define REG_SPDIFIN_EDGE_INTERRUPT  0x06d8
    #define SPDIF_RX_EDGE_CNT       (0x8 << 0)      // Default is 0x8 means transfer 8 bits call input detect success
    #define SPDIF_RX_EDGE_DET_EN    (0x1 << 8)      // Optical input interrupt
    #define SPDIF_RX_INPUT_SEL      (0x00 << 14)    // Input Selection
    #define SPDIF_RX_EDGE_FLAG_INT  (0x1 << 28)     // Optical input interrupt send means data avtive
    
#define REG_SPDIFIN_DEBUG3          0x06a8
    #define SPDIF_RX_PREAMBLE_ERR   (0x1 << 0)      // Preamble error
    #define SPDIF_RX_PREAMBLE_BERR  (0x1 << 1)      // Preamble B error
    #define SPDIF_RX_PREAMBLE_MERR  (0x1 << 2)      // Preamble M error
    #define SPDIF_RX_PREAMBLE_WERR  (0x1 << 3)      // Preamble W error
    #define SPDIF_RX_PARITY_ERR     (0x1 << 5)      // Parity check error
    #define SPDIF_RX_SMP_COUNTER_ERR (0x1 << 4)     // Sample counter error
    #define SPDIF_RX_TIMEOUT_ERR    (0x1 << 6)      // Time out error

#define REG_SPDIFIN_BR              0x06c0
    #define SPDIF_RX_BC_LOWBOUND    (0x11 << 12)    // Number of bitcell lower bound counted by spdifin sys_clk
#endif

#if defined(CC_MT5880) || defined(CC_MT5881) // may be fixed, paul
    #define ANA_BASE                0xf0060000
#else
    #define ANA_BASE                0xf0061000
#endif

#define ANA_READ32(Reg)                IO_READ32(ANA_BASE, (Reg))
#define ANA_WRITE32(Reg, Val)          IO_WRITE32(ANA_BASE, (Reg), (Val))

#define ANA_SET_BIT(Reg, Bit)          ANA_WRITE32(Reg, ANA_READ32(Reg) | (Bit))
#define ANA_CLR_BIT(Reg, Bit)          ANA_WRITE32(Reg, ANA_READ32(Reg) & (~(Bit)))

#define ANA_WRITE32_MSK(Reg,Val,Msk)   ANA_WRITE32(Reg, (ANA_READ32(Reg)&(~(Msk))) | (Val))

#endif /* _AUD_HW_H_ */

