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
 * $Author: p4admin $
 * $Date: 2015/04/07 $
 * $RCSfile: aud_if.h,v $
 * $Revision: #20 $
 *
 *---------------------------------------------------------------------------*/

/** @file aud_if.h
 * Declare audio related structure and function table.
 */

#ifndef X_AUD_IF_H
#define X_AUD_IF_H

//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------

#include "x_typedef.h"
#include "dmx_if.h"
#include "feeder_if.h"
#include "sif_if.h"
#include "stc_drvif.h"
#include "u_drv_cust.h"



//-----------------------------------------------------------------------------
// Audio table enum and structure
//-----------------------------------------------------------------------------

#define AUD_ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))

#define DUAL_AUD_DEC_CONNECT 0
#define DUAL_AUD_DEC_DISCONNECT 1

//#ifndef CC_AUD_HPF_SUPPORT
//#define CC_AUD_HPF_SUPPORT
//#endif

//#ifndef DSP_SUPPORT_TCL_VBASS
//#define DSP_SUPPORT_TCL_VBASS
//#endif

#define APROC_POSTBUF_BLOCK_SIZE        64

#define AUD_DSP_NUM                     ((UINT8)2)

#ifdef CC_DUAL_AUD_DEC_SUPPORT
#define AUD_DEC_NUM                     ((UINT8)5)
#elif defined (CC_AUD_4_DECODER_SUPPORT)
#define AUD_DEC_NUM                     ((UINT8)4)
#elif defined (CC_MT5391_AUD_3_DECODER)
#define AUD_DEC_NUM                     ((UINT8)3)
#else
#define AUD_DEC_NUM                     ((UINT8)2)
#endif

#ifdef CC_DUAL_AUD_DEC_SUPPORT
#define AUD_DEC_MAX                     ((UINT8)5)
#else
#define AUD_DEC_MAX                     ((UINT8)4)
#endif

// Decoder ID check macro
#define AUD_DEC_ID_VALIDATE(u1DecId)    if (u1DecId>=AUD_DEC_NUM)\
                                            ASSERT(0)
#define AUD_DEC_ID_VALIDATE_2(u1DecId)  if (u1DecId>AUD_DEC_AUX)\
                                            ASSERT(0)
#define AUD_DEC_ID_VALIDATE_3(u1DecId)  if (u1DecId>AUD_DEC_THIRD)\
                                            ASSERT(0)
#define AUD_DEC_ID_VALIDATE_SUB(u1DecId)  if (u1DecId>AUD_DEC_SUB)\
                                                ASSERT(0)                                            
#define AUD_DSP_ID_VALIDATE(u1DspId)    VERIFY((u1DspId == AUD_DSP0) || (u1DspId == AUD_DSP1))
                                            
#define AUD_ADPAN_CHANNEL_VALIDATE(eChannel)    if (eChannel>=AUD_ADPAN_CH_NUM)\
                                                    ASSERT(0)

#define AUD_OUT_PORT_VALIDATE(eOut)     if ((AUD_OUT_PORT_T)eOut>=AUD_OUT_PORT_NUM)\
                                                ASSERT(0)
#define AUD_MIXSND_ID_VALIDATE(u1MixSndId)    if (u1MixSndId>=AUD_MIXSND_NUM)\
                                            ASSERT(0)

#define AUD_AOUT_NO     ((UINT8)2)

#define APLL_ID_MAIN            0
#define APLL_ID_AUX             1

#define UNUSED_GPIO     (0xffffffff)
#define SW_NO_USED      (0xff)
#define NO_USED         (0xff)

#define MUX_SW_0        1, 1
#define MUX_SW_1        0, 1
#define MUX_SW_2        1, 0
#define MUX_SW_3        0, 0

#define DEFAULT_0_DB    0x20000

#ifdef CC_AUD_MULTI_BUFFER_MEMORY_PLAY
#define MAX_MEM_BUFFER_NUMBER       (16)
#endif

#ifdef CC_AUD_7BAND_EQUALIZER
#define CH_EQ_BAND_NO   7
#define AUD_EQ_IIR_BAND_NUM     7
#else
#define CH_EQ_BAND_NO   5
#define AUD_EQ_IIR_BAND_NUM     8
#endif

#ifdef CC_AUD_DSP_SUPPORT_AUDYSSEY
#define AUD_AEQ_IIR_BAND_NUM    5
#endif

//#define CC_AUD_SOUND_MODE
#define PEQ_SETTING_MAX 9
#define PEQ_GAIN_NUM_FOR_VOL    4

#define EQ_BAND_GAIN_DB_MAX             14

#ifdef CC_AUD_3_5K_TREBLE
// Bass/Treble constant
#define BASS_TREBLE_GAIN_DB_MAX         14
// Bass Treble
#define BASS_TREBLE_GAIN_MAX            ((BASS_TREBLE_GAIN_DB_MAX*4) + 1) // range -14 ~ +14 dB, 0.5dB/Step
#else
// Bass/Treble constant
#define BASS_TREBLE_GAIN_DB_MAX         14
// Bass Treble
#define BASS_TREBLE_GAIN_MAX ( (14*2) + 1) // range -14 ~ +14 dB, 1 dB/Step
#endif

#ifdef CC_AUD_FY12_LOUDNESS
//#define BASS_TREBLE_ATTN_GAIN_DB_MIN         (-10+14) //-10dB ~0dB
#define BASS_TREBLE_ATTN_GAIN_DB_MIN         (-14*2+28) //-14dB ~0dB
#define BASS_TREBLE_ATTN_GAIN_DB_MAX         (14*2+28) //-14dB ~0dB
#define BASS_TREBLE_ATTN_GAIN_MAX            ((14*2*2) + 1) // range -14 ~ +14 dB, 0.5 dB/Step
#endif
#define BBE_LEVEL_MAX                   5
#define BBE_PROCESS_MIN                 (3*2)
#define BBE_PROCESS_MAX                 (12*2)
#define BBE_LO_CONTOUR_MIN              (3*2)
#define BBE_LO_CONTOUR_MAX              (12*2)
#define BBE_3D_GAIN_MAX                 100

#define CDNOTCH_Q_IDX_MIN               0
#define CDNOTCH_Q_IDX_MAX               37
#define CDNOTCH_FC_MIN                  60
#define CDNOTCH_FC_MAX                  15000

#define AUD_VOLUME_OFFSET_IDX_NUM 13
#define AUD_SW_VOLUME_OFFSET_IDX_NUM 25

#ifdef CC_SUPPORT_STR
#define RESUME_BASS_ENABLE             0
#define RESUME_TREBLE_ENABLE           1
#define RESUME_VIRSUR1_ENABLE          2
#define RESUME_VIRSUR2_ENABLE          3
#define RESUME_EQ_ENABLE               4
#define RESUME_AD_SPEAKER_ENABLE       5
#define RESUME_AD_HP_EnABLE            6
#define RESUME_AD_SPEAKER_HP_EnABLE    7
#define RESUME_INDEX_MAX               8
#endif

#if defined(CC_AUD_SM_SUPPORT)
#define INDIA_GAIN_SP_DB   4.5
#define INDIA_GAIN_LO_DB   0
#define INDIA_GAIN_HP_DB   0
#elif defined(CC_AUD_W_SUPPORT)
#define INDIA_GAIN_SP_DB   5
#define INDIA_GAIN_LO_DB   5
#define INDIA_GAIN_HP_DB   5
#else
#define INDIA_GAIN_SP_DB   5
#define INDIA_GAIN_LO_DB   5
#define INDIA_GAIN_HP_DB   5
#endif

#ifdef CC_AUD_AVC_V20
#ifndef CC_AUD_DRC_V20
#define CC_AUD_DRC_V20
#endif  // CC_AUD_DRC_V20
#endif  // CC_AUD_AVC_V20

//#ifndef ARM_SUPPORT_SRS_SS
//#define ARM_SUPPORT_SRS_SS
//#endif

/* SPDIF */

// channel status
typedef struct
{
    UINT8 u1Mode; // [0] 0 = consumer mode
    UINT8 u1SampleMode; // [1] 0 = linear PCM, 1 = non-linear PCM (RAW)
    UINT8 u1Copyright; // [2] 0 = copyright is asserted, 1 = no copyright
    UINT8 u1PreEmphasis; // [3,5] pre-emphasis
    UINT8 u1Category; // [8,15] category code, MSB 1 bit = L-bit
    UINT8 u1SourceNum; // [16, 19] source number
    UINT8 u1CannelNum; // [20, 23] channel number
    UINT8 u1SampleRate; // [24, 27] sample rate
    UINT8 u1ClockAccuray; // [28, 29] clock accuracy
    UINT8 u1WordLength; // [32, 35] word length
    UINT8 u1OrgSampleRate; // [36, 39] original sample rate
}   SPDIF_CHANNEL_STATUS_T;

// sample rate in channel status
#define SPDIF_SAMPLE_RATE_22KHZ 0x4 // 22.05 kHz
#define SPDIF_SAMPLE_RATE_44KHZ 0x0 // 44.1 kHz
#define SPDIF_SAMPLE_RATE_88KHZ 0x8 // 88.2 kHz
#define SPDIF_SAMPLE_RATE_176KHZ 0xc // 176.4 kHz
#define SPDIF_SAMPLE_RATE_24KHZ 0x6 // 24 kHz
#define SPDIF_SAMPLE_RATE_48KHZ 0x2 // 48 kHz
#define SPDIF_SAMPLE_RATE_96KHZ 0xa // 96 kHz
#define SPDIF_SAMPLE_RATE_192KHZ 0xe // 192 kHz
#define SPDIF_SAMPLE_RATE_32KHZ 0x3 // 32 kHz
#define SPDIF_SAMPLE_RATE_NOT_INDICATE 0x1 // not indicated

// word length in channel status
#define SPDIF_WORDLENGTH_MAX24_NOT_INDICATE 0x1 // max 24bits, not indicate
#define SPDIF_WORDLENGTH_MAX24_20BITS 0x3 // max 24bits, 20bits
#define SPDIF_WORDLENGTH_MAX24_22BITS 0x5 // max 24bits, 22bits
#define SPDIF_WORDLENGTH_MAX24_23BITS 0x9 // max 24bits, 23bits
#define SPDIF_WORDLENGTH_MAX24_24BITS 0xb // max 24bits, 24bits
#define SPDIF_WORDLENGTH_MAX24_21BITS 0xd // max 24bits, 21bits
#define SPDIF_WORDLENGTH_MAX20_NOT_INDICATE 0x0 // max 20bits, not indicate
#define SPDIF_WORDLENGTH_MAX20_16BITS 0x2 // max 20bits, 16bits
#define SPDIF_WORDLENGTH_MAX20_18BITS 0x4 // max 20bits, 18bits
#define SPDIF_WORDLENGTH_MAX20_19BITS 0x8 // max 20bits, 19bits
#define SPDIF_WORDLENGTH_MAX20_20BITS 0xa // max 20bits, 20bits
#define SPDIF_WORDLENGTH_MAX20_17BITS 0xc // max 20bits, 17bits


// IEC channel status by
#define IEC_CHANNEL_STATUS_BY_SETUP (0x0)
#define IEC_CHANNEL_STATUS_BY_SOURCE (0x1)

// Copyright bit
// 0 = copyright is asserted
// 1 = no copyright is asserted
#define IEC_COPY_NEVER                 (0x00)
#define IEC_COPY_FREELY                  (0x01)
#define IEC_COPY_NO_MORE            (0x01)
#define IEC_COPY_ONCE                    (0x00)

// IEC category code
#define IEC_ORIGINAL_GENERAL            (0x00)
#define IEC_ORIGINAL_BROAD_CAST         (0x64)
#define IEC_NON_ORIGINAL_BROAD_CAST     (0xe4)
#define IEC_ORIGINAL_BROAD_CAST_EU      (0x0c)
#define IEC_CH_STATUS_BY_SOURCE         (0xff)

// L-bit
#define IEC_CATEGORY_LBIT_NUMBER 7
#define IEC_CATEGORY_LBIT (1 << IEC_CATEGORY_LBIT_NUMBER)
#define IEC_CATEGORY_LBIT_MASK (~IEC_CATEGORY_LBIT)

// IEC word length (defined in register map order)
#define IEC_WORD_LENGTH_24BIT           (0 << 0)
#define IEC_WORD_LENGTH_DEFAULT      (1 << 0)
#define IEC_WORD_LENGTH_20BIT           (2 << 0)
#define IEC_WORD_LENGTH_16BIT           (3 << 0)

typedef enum
{
    BURST_INFO_NULL_DATA = 0,
    BURST_INFO_AC3,
    BURST_INFO_AAC = 7,
    BURST_INFO_DTS = 11,
    BURST_INFO_DTS_512 = 11, 
    BURST_INFO_DTS_1024,
    BURST_INFO_DTS_2048,
    BURST_INFO_NUM
}   SPDIF_BURST_INFO_T;

#if defined(CC_AUD_ARM_SUPPORT) && defined(CC_AUD_ARM_RENDER)
typedef enum {
    APROC_RAW_UNKNOWN = 0,
    APROC_RAW_PCM,
    APROC_RAW_AC3,
    APROC_RAW_AAC,
    APROC_RAW_DTS_512,
    APROC_RAW_DTS_1024,
    APROC_RAW_DTS_2048,
    APROC_RAW_NUM
} APROC_IEC_AUDFMT_T;
#endif
#define IEC_CFG0_CATEGORYCODE_BIT 0
#define IEC_CFG0_CATEGORYCODE_MASK (~0xff)

#define IEC_CFG0_PCM_RAW_BIT 8
#define IEC_CFG0_PCM_RAW_MASK (~0x300)
#define IEC_CFG0_PCM_RAW_ISRAW (0x3 << IEC_CFG0_PCM_RAW_BIT)
#define IEC_CFG0_PCM_RAW_ISPCM (0x0 << IEC_CFG0_PCM_RAW_BIT)

#define IEC_CFG0_WORDLENGTH_BIT 12
#define IEC_CFG0_WORDLENGTH_MASK (~0xf000)
#define IEC_CFG0_WORDLENGTH_24BITS (SPDIF_WORDLENGTH_MAX24_24BITS << IEC_CFG0_WORDLENGTH_BIT)
#define IEC_CFG0_WORDLENGTH_20BITS (SPDIF_WORDLENGTH_MAX24_20BITS << IEC_CFG0_WORDLENGTH_BIT)
#define IEC_CFG0_WORDLENGTH_16BITS (SPDIF_WORDLENGTH_MAX20_16BITS << IEC_CFG0_WORDLENGTH_BIT)
#define IEC_CFG0_WORDLENGTH_DEFAULT (SPDIF_WORDLENGTH_MAX20_NOT_INDICATE << IEC_CFG0_WORDLENGTH_BIT)

#define IEC_WORDLENGTH_24BITS 0x0
#define IEC_WORDLENGTH_DEFAULT 0x1
#define IEC_WORDLENGTH_20BITS 0x2
#define IEC_WORDLENGTH_16BITS 0x3

#define IEC_CFG1_DIGITAL_BIT 8
#define IEC_CFG1_DIGITAL_MASK (~(1 << IEC_CFG1_DIGITAL_BIT))
#define IEC_CFG1_DIGITAL_RAW (0x1 << IEC_CFG1_DIGITAL_BIT)
#define IEC_CFG1_DIGITAL_PCM (0x0 << IEC_CFG1_DIGITAL_BIT)

#define IEC_CFG1_CBIT_BIT 7
#define IEC_CFG1_CBIT_MASK (~(1 << IEC_CFG1_CBIT_BIT))

#define IEC_CFG1_SAMPLERATE_BIT 0
#define IEC_CFG1_SAMPLERATE_MASK (~(0xf))
#define IEC_CFG1_SAMPLERATE_32K (SPDIF_SAMPLE_RATE_32KHZ << IEC_CFG1_SAMPLERATE_BIT)
#define IEC_CFG1_SAMPLERATE_44K (SPDIF_SAMPLE_RATE_44KHZ << IEC_CFG1_SAMPLERATE_BIT)
#define IEC_CFG1_SAMPLERATE_48K (SPDIF_SAMPLE_RATE_48KHZ << IEC_CFG1_SAMPLERATE_BIT)


// PROM ID
#define DspPROMTstAddr   0x39           // PROM test address
#define DspPROMRdAddr    0x3A           // PROM read port address
#define DspPROMMsk       0xFFFF         // PROM intruction mask
#define DspPROMEnTst     0x100000       // PROM test enable
#define DspPROMVerOffset 0x78           // PROM ID start address
// DROM ID
#define DspDROMRdAddr    0x4000         // DROM read port address
#define DspDROMMsk       0xFFFFFF       // DROM data mask
#define DspDROMVerOffset 0x0            // DROM ID start address

#define DSP_WMA_MWIF
#define CC_SET_VOLUME

// Downmix channel position
#define DOWNMIX_CH_DO_ALL_POST_PROC     ((UINT8)0x00)
#define DOWNMIX_CH_BEFORE_AVC           ((UINT8)0x01)
#define DOWNMIX_CH_BYPASS_POST_PROC     ((UINT8)0x03)

#ifdef CC_AUD_HDMI_PARSER_2_0_SUPPORT
#define IEC_SYNCWORD_PA_PB  0xf8724e1f    //pa must equal to 0x72f8,the really pa in spec is 0xf872
#define IEC_SYNCWORD_PA     0xf872        //pa must equal to 0x72f8,the really pa in spec is 0xf872
#define IEC_SYNCWORD_PB   0x4e1f         //pb must equal to 0x1f4e,the really pb in spec is 0x4e1f
#define IEC_SYNCWORD_DTS_14 0x1FFFE800
#define IEC_SYNCWORD_DTS_16 0x7FFE8001
//#define IEC_SYNCWORD_CHECK_PC  0xe0e0  //pc bit 5-7 and bit 13-15 must set 0
#define IEC_MAX_TRANSFER    0x17f8  //0x1800(frame size)-4(sync word)
#define IEC_AC3_FRAME_SIZE 0x1800
#define HDMI_BUFFER_SIZE (0)
#define HDMI_PARSER_PCM_TRANSFER_SIZE 256
#define HDMI_PARSER_PCM_NONZERO_THL 0x2400 //48ms@48KHz 2 channel
#define HDMI_PARSER_NON_DETECT_SIZE 0x8040
#define HDMI_PARSER_DETECT_PCM_SIZE 0x9080  //0x2840
#endif

#define MPEG_FRAME_SIZE 0x870 //1152 sample for 48 kHZ, in 90K Hz Clock.
#define MPEG_AAC_FRAME_SIZE 2700    //30ms, cannot exceed minimum of video set audio delay

/*For Power ON/OFF music flag in EEP*/
#define AUD_POWER_ON_MUSIC_MASK            (0x1 << 0)
#define AUD_POWER_OFF_MUSIC_MASK           (0x1 << 1)

// ASRC definition
#define ASRC_BASE_FREQ_8K           0x000a0000
#define ASRC_BASE_FREQ_16K          0x00140000
#define ASRC_BASE_FREQ_32K          0x00280000

#define ASRC_BASE_FREQ_11K          0x000dc800
#define ASRC_BASE_FREQ_22K          0x001b9000
#define ASRC_BASE_FREQ_44K          0x00372000

#define ASRC_BASE_FREQ_12K          0x000f0000
#define ASRC_BASE_FREQ_24K          0x001e0000
#define ASRC_BASE_FREQ_48K          0x003c0000
/*For Power ON/OFF music flag in EEP*/
#define AUD_POWER_ON_MUSIC_MASK            (0x1 << 0)
#define AUD_POWER_OFF_MUSIC_MASK           (0x1 << 1)
// new SDPFI control
typedef enum
{
    SPDIF_REG_TYPE_DEFAULT = 0,
    SPDIF_REG_TYPE_USER,
    SPDIF_REG_TYPE_CIPLUS,
    SPDIF_REG_TYPE_NUM
}   SPDIF_REG_TYPE_T;

#if 0 //move to drvcust_if.h
typedef enum
{
    FORMAT_RJ,
    FORMAT_LJ,
    FORMAT_I2S
}   DATA_FORMAT_T;

typedef enum
{
    MCLK_128FS,
    MCLK_192FS,
    MCLK_256FS,
    MCLK_384FS,
    MCLK_512FS,
    MCLK_768FS
}   MCLK_FREQUENCY_T;
#endif

typedef enum
{
    LRCK_CYC_16,
    LRCK_CYC_24,
    LRCK_CYC_32
}   LRCK_CYC_T;

typedef enum
{
    DAC_16_BIT,
    DAC_18_BIT,
    DAC_20_BIT,
    DAC_24_BIT
}   DAC_DATA_NUMBER_T;

typedef enum
{
    AUD_BS_SEL_PSR, 
    AUD_BS_SEL_LINE_IN1, 
    AUD_BS_SEL_DMX, 
    AUD_BS_SEL_LINE_IN2, 
    AUD_BS_SEL_USER0, 
    AUD_BS_SEL_USER1, 
    AUD_BS_SEL_LINE_IN3,
    AUD_BS_SEL_NUM
} AUD_BS_SEL_T;

typedef enum
{
    AUD_STREAM_FROM_OTHERS = 0,         ///< ex. Pink Noise Generator
    AUD_STREAM_FROM_DIGITAL_TUNER,      ///< Digital tuner. TS
    AUD_STREAM_FROM_ANALOG_TUNER,       ///< ex. Analog tuner. SIF/AF
    AUD_STREAM_FROM_SPDIF,              ///< SPDIF-in
    AUD_STREAM_FROM_LINE_IN,            ///< Line in
    AUD_STREAM_FROM_HDMI,               ///< HDMI in
    AUD_STREAM_FROM_MEMORY,             ///< Memory
    AUD_STREAM_FROM_BUF_AGT,            ///< Buffer agent
    AUD_STREAM_FROM_FEEDER,             ///< Feeder
    AUD_STREAM_FROM_MULTI_MEDIA,        /// < Multi-Media
    AUD_STREAM_FROM_LINE_IN_2 = 10,          ///< Line in2
    AUD_STREAM_FROM_LINE_IN_3 = 11,          ///< Line in3 for Echo Cancel & BT.
#ifdef CC_ENABLE_AOMX
    AUD_STREAM_FROM_GST,                    /// Gstreamer
#endif
#ifdef CC_AUD_I2S_IN
    AUD_STREAM_FROM_I2S,                ///<I2S in
#endif
    AUD_STREAM_FROM_NUM                 ///< Number of stream source
}   AUD_DEC_STREAM_FROM_T;

typedef struct
{
    DATA_FORMAT_T      eFormat;         ///< format of alignment
    DAC_DATA_NUMBER_T  eBits;           ///< number of bits per sample
    LRCK_CYC_T         eCycle;          ///< cycles per sample
    MCLK_FREQUENCY_T   eSampleFreq;     ///< DAC sampling frequence
    BOOL               fgDataInvert;    ///< Invert audio output for OP phase
    BOOL               fgLRInvert;      ///< Invert L/R audio output
}   AOUT_CFG_T;

typedef enum
{
    AUD_INPUT_ID_COMP_VID_0 = 0,
    AUD_INPUT_ID_COMP_VID_1,
    AUD_INPUT_ID_COMP_VID_2,
    AUD_INPUT_ID_COMP_VID_3,
    AUD_INPUT_ID_S_VID_0,
    AUD_INPUT_ID_S_VID_1,
    AUD_INPUT_ID_S_VID_2,
    AUD_INPUT_ID_YPBPR_0,
    AUD_INPUT_ID_YPBPR_1,
    AUD_INPUT_ID_YPBPR_2,
    AUD_INPUT_ID_VGA_0,
    AUD_INPUT_ID_VGA_1,
    AUD_INPUT_ID_HDMI_0,
    AUD_INPUT_ID_HDMI_1,
    AUD_INPUT_ID_HDMI_2,
    AUD_INPUT_ID_HDMI_3,
    AUD_INPUT_ID_HDMI_4,
    AUD_INPUT_ID_DVI_0,
    AUD_INPUT_ID_DVI_1,
    AUD_INPUT_ID_DVI_2,
    AUD_INPUT_ID_DVI_3,
    AUD_INPUT_ID_DVI_4,
    AUD_INPUT_ID_SCART_0,
    AUD_INPUT_ID_SCART_1,
    AUD_INPUT_ID_SCART_2,
    AUD_INPUT_ID_SCART_3,
    AUD_INPUT_ID_AUXIN_0,
    AUD_INPUT_ID_AUXIN_1,
    AUD_INPUT_ID_MAX
} AUD_INPUT_ID_T;

typedef enum
{
    AUD_CHL_L_R = 0,
    AUD_CHL_LS_RS,
    AUD_CHL_C_SW,
    AUD_CHL_IEC,
    AUD_CHL_DOWNMIX, //HP
    AUD_CHL_BYPASS, //MON
    AUD_CHL_EXTCH,  //CH13,14
    AUD_CHL_AUX     //AUX_LR
}   AUD_CHL_DEF_T;

typedef enum
{
    AUD_AP_COUNTRY_UNKNOWN = 0,
    AUD_AP_COUNTRY_GA,
    AUD_AP_COUNTRY_INDIA,
    AUD_AP_COUNTRY_PHILIPPINE,
    AUD_AP_COUNTRY_CIS,
    AUD_AP_COUNTRY_CHINA,
    AUD_AP_COUNTRY_LATIN
} AUD_APP_COUNTRY_SEL;

typedef enum
{
    AUD_SPEAKER = 0,
    AUD_HEADPHONE,
    AUD_SPDIF,
    AUD_AV_OUT,
    AUD_MONITOR_SCART,
    AUD_TV_SCART,
    AUD_BLUETOOTH,
    AUD_PORT_LGSE,
    AUD_OUT_PORT_NUM
} AUD_OUT_PORT_T;

typedef enum
{
    AUD_ADEC0_AD_ENABLE = 0,
    AUD_ADEC1_AD_ENABLE,
    AUD_AD_DISABLE,
} AUD_ADEC_AD_T;


typedef enum
{
    APROC_OUT_SEL_DEC0 =  0x00000001,
    APROC_OUT_SEL_DEC1 =  0x00000002,
    APROC_OUT_SEL_DEC2 =  0x00000004,
    APROC_OUT_SEL_MIX0 =  0x00000100,
    APROC_OUT_SEL_MIX1 =  0x00000200,
    APROC_OUT_SEL_MIX2 =  0x00000400,
    APROC_OUT_SEL_MIX3 =  0x00000800,
    APROC_OUT_SEL_MIX4 =  0x00001000,
    APROC_OUT_SEL_MIX5 =  0x00002000,
    APROC_OUT_SEL_MIX6 =  0x00004000,
    APROC_OUT_SEL_MIX7 =  0x00008000,
    APROC_OUT_SEL_LGSE =  0x80000000,
} APROC_OUT_SEL_T;

typedef void (*CODEC_INIT)(void);
typedef void (*CODEC_MUTE)(UINT8 u1DacId, BOOL fgMute);
typedef void (*CODEC_MUTE_CLK)(UINT8 u1DacId, BOOL fgMute);
typedef void (*CODEC_SET_DAC_IN_FMT)(DATA_FORMAT_T eFormat, MCLK_FREQUENCY_T eMclk);
typedef void (*CODEC_SET_ADC_OUT_FMT)(DATA_FORMAT_T eFormat, MCLK_FREQUENCY_T eMclk);
typedef void (*CODEC_SET_DAC_GAIN)(UINT8 u1DacId, INT16 i2Gain);
typedef void (*CODEC_GET_DAC_GAIN)(UINT8 u1DacId, INT16* pi2Gain);
typedef void (*CODEC_SET_ADC_GAIN)(UINT8 u1DacId, INT16 i2Gain);
typedef void (*CODEC_GET_ADC_GAIN)(UINT8 u1DacId, INT16* pi2Gain);
typedef void (*CODEC_SET_HP_GAIN)(UINT8 u1HpId, INT16 i2Gain);
typedef void (*CODEC_GET_HP_GAIN)(UINT8 u1HpId, INT16* i2Gain);
typedef void (*CODEC_SET_ADC_MUX_SEL)(UINT8 u1Sel);
typedef void (*CODEC_REG_WRITE)(UINT8 u1Offset, UINT8 u1Data);
typedef UINT8 (*CODEC_REG_READ)(UINT8 u1Offset);
typedef void (*CODEC_SET_DAC_CHL)(UINT8 u1DacId, AUD_CHL_DEF_T eChl);
typedef void (*CODEC_GET_DAC_CHL)(void);
typedef void  (*CODEC_ADC_DOWN)(BOOL fgEnalbe);

typedef void (*ADAC_INIT)(void);
typedef void (*ADAC_MUTE)(BOOL fgMute);
typedef void (*ADAC_ENABLE)(BOOL fgEnable);
typedef void (*ADAC_CHL_SEL)(UINT8 u1DacId, UINT8 eChl);
typedef void (*ADAC_SET_DAC_IN_FMT)(DATA_FORMAT_T eFormat, MCLK_FREQUENCY_T eMclk);
typedef void (*ADAC_SET_DAC_GAIN)(UINT8 u1HpId, INT16 i2Gain);
typedef void (*ADAC_REG_WRITE)(UINT8 u1Offset, UINT8 u1Data);
typedef UINT8 (*ADAC_REG_READ)(UINT8 u1Offset);

#if 0 //move to drvcust_if.h
typedef void (*AMP_INIT)(void);
typedef void (*AMP_MUTE)(BOOL fgMute);
typedef void (*AMP_SET_DAC_IN_FMT)(DATA_FORMAT_T eFormat, MCLK_FREQUENCY_T eMclk);
typedef void (*AMP_SET_DAC_GAIN)(UINT8 u1HpId, INT16 i2Gain);
typedef void (*AMP_REG_WRITE)(UINT8 u1Offset, UINT8 u1Data);
typedef UINT8 (*AMP_REG_READ)(UINT8 u1Offset);
#endif

typedef struct  {
    UINT32 u4Value;
    const CHAR *paName; 
}AUD_ENUM_TO_NAME_T;

/// Audio codec function table.
typedef struct
{
    CODEC_INIT             pfnInit;
    CODEC_MUTE             pfnMute;
    CODEC_MUTE_CLK         pfnMuteClk;
    CODEC_SET_DAC_IN_FMT   pfnSetDacInFmt;
    CODEC_SET_ADC_OUT_FMT  pfnSetAdcOutFmt;
    CODEC_SET_DAC_GAIN     pfnSetDacGain;
    CODEC_GET_DAC_GAIN     pfnGetDacGain;
    CODEC_SET_ADC_GAIN     pfnSetAdcGain;
    CODEC_GET_ADC_GAIN     pfnGetAdcGain;
    CODEC_SET_HP_GAIN      pfnSetHpGain;
    CODEC_GET_HP_GAIN      pfnGetHpGain;
    CODEC_SET_ADC_MUX_SEL  pfnSetAdcMuxSel;
    CODEC_REG_WRITE        pfnRegWrite;
    CODEC_REG_READ         pfnRegRead;
    CODEC_SET_DAC_CHL      pfnSetDacChl;
    CODEC_GET_DAC_CHL      pfnGetDacChl;
    CODEC_ADC_DOWN         pfnADCDown;
} CODEC_FUNCTBL_T;

/// Codec configuration table.
typedef struct
{
    CHAR             szName[32];
    INT16            i2MaxHpGain;
    INT16            i2MinHpGain;
    INT16            i2MaxDacGain;
    INT16            i2MinDacGain;
    UINT8            u1AdcMuxNum;
    UINT8            u1DacNum;
    UINT8            u1HpNum;
    CODEC_FUNCTBL_T* prCodecFuncTbl;
} CODEC_T;

/// Internal dac function table.
typedef struct
{
    ADAC_INIT           pfnInit;
    ADAC_MUTE           pfnMute;
    ADAC_ENABLE         pfnEnable;
    ADAC_CHL_SEL        pfnChlSelect;
    ADAC_SET_DAC_IN_FMT pfnSetDacInFmt;
    ADAC_SET_DAC_GAIN   pfnSetDacGain;
    ADAC_REG_WRITE      pfnRegWrite;
    ADAC_REG_READ       pfnRegRead;
} ADAC_FUNCTBL_T;

/// Internal dac configuration table.
typedef struct
{
    CHAR            szName[32];
    INT16           i2MaxGain;
    INT16           i2MinGain;
    UINT8           u1DacNum;
    BOOL            fInterDac;
    ADAC_FUNCTBL_T* prDacFuncTbl;
} ADAC_T;

#if 0 //move to drvcust_if.h
/// Audio amplify function table.
typedef struct
{
    AMP_INIT           pfnInit;
    AMP_MUTE           pfnMute;
    AMP_SET_DAC_IN_FMT pfnSetDacInFmt;
    AMP_SET_DAC_GAIN   pfnSetDacGain;
    AMP_REG_WRITE      pfnRegWrite;
    AMP_REG_READ       pfnRegRead;
} AMP_FUNCTBL_T;

/// Audio amplify configuration table.
typedef struct
{
    CHAR            szName[32];
    INT16           i2MaxGain;
    INT16           i2MinGain;
    UINT8           u1DacNum;
    AMP_FUNCTBL_T* prAmpFunc;
} AMP_T;
#endif

/// Audio ampify init detection configuration table
typedef struct
{
    UINT8           u1ConditionId;
    UINT8           u1Addr;
    UINT8           u1Data;
    UINT8           u1DataMask;
} AMP_INIT_DETECT_T;

/// Audio codec select table.
typedef enum
{
    AUD_CODEC_MT8291 = 0,
    AUD_CODEC_WM8776,
    AUD_CODEC_MT538xInternal,
    AUD_CODEC_MT536xInternal,
    AUD_CODEC_MT539xInternal,
    AUD_CODEC_MT5387,
    AUD_CODEC_MT8292,
    AUD_CODEC_MT5365,
    AUD_CODEC_MT5398,
    AUD_CODEC_OTHERS,
    AUD_CODEC_NULL
} AUD_CODEC_T;

/// Audio codec select table2.
typedef enum
{
    AUD_ADAC_MT537X_INTERNAL = 0,
    AUD_ADAC_WM8766,
    AUD_ADAC_CSXXX,
    AUD_ADAC_OTHERS,
    AUD_ADAC_NULL
} AUD_ADAC_T;

/// Audio channel table.
typedef enum
{
    AUD_CH_FRONT_LEFT = 0,
    AUD_CH_FRONT_RIGHT,
    AUD_CH_REAR_LEFT,
    AUD_CH_REAR_RIGHT,
    AUD_CH_CENTER,
    AUD_CH_SUB_WOOFER,
    AUD_CH_IEC_LEFT,
    AUD_CH_IEC_RIGHT,
    AUD_CH_DMX_LEFT,
    AUD_CH_DMX_RIGHT,
    AUD_CH_BYPASS_LEFT,
    AUD_CH_BYPASS_RIGHT,
    AUD_CH_CH1314_LEFT,
    AUD_CH_CH1314_RIGHT,
    AUD_CH_AUX_FRONT_LEFT,
    AUD_CH_AUX_FRONT_RIGHT,
    AUD_CH_INPUT_SRC,                   ///< For input source volume control
    AUD_CH_ALL,                         ///< For master volume control
    AUD_CH_LFE,
    AUD_CH_THIRD,
    AUD_CH_NUM
}   AUD_CH_T;

/// Audio digital amplify table.
typedef enum
{
    AUD_AMP_APOGEE_DDX_2051 = 0,
    AUD_AMP_AKM_XXX,
    AUD_AMP_WOLFSON_XXX,
    AUD_AMP_PHILIPS_XXX,
    AUD_AMP_OTHERS,
    AUD_AMP_AD82581,
    AUD_AMP_TAS5707,
    AUD_AMP_STA339,
    AUD_AMP_NTP7400L,
    AUD_AMP_NTP7515,
    AUD_AMP_TAS5717,
    AUD_AMP_STA381,
    AUD_AMP_NE3201,
    AUD_AMP_DRVCUST,  //AMP_DRV_CUST
    AUD_AMP_NULL
} AUD_AMP_T;

typedef enum
{
    AUD_AMP_ID_LR = 0,
    AUD_AMP_ID_LsRs,
    AUD_AMP_ID_CSw,
    AUD_AMP_ID_NUM
} AUD_AMP_ID_T;

/// Audio opctrl polarity table.
typedef enum
{
    AUD_OPCTRL_MUTE_GPIO_LOW_ENALBE = 0,
    AUD_OPCTRL_MUTE_GPIO_HIGH_ENALBE = 1
}   AUD_OPCTRL_MUTE_GPIO_POLARITY_T;


/// Audio codec mutel polarity table.
typedef enum
{
    AUD_MUTE_GPIO_LOW_ENALBE = 0,
    AUD_MUTE_GPIO_HIGH_ENALBE = 1
}   AUD_MUTE_GPIO_POLARITY_T;

/// Audio amplify enable polarity table.
typedef enum
{
    AUD_AMP_GPIO_LOW_ENALBE = 0,
    AUD_AMP_GPIO_HIGH_ENALBE = 1
}   AUD_AMP_GPIO_POLARITY_T;

/// Audio gpio enable polarity table.
typedef enum
{
    AUD_GPIO_LOW_ENALBE = 0,
    AUD_GPIO_HIGH_ENALBE = 1
}   AUD_GPIO_POLARITY_T;

/// Audio DVD gpio enable polarity table.
typedef enum
{
    AUD_DVD_GPIO_LOW_ENALBE = 0,
    AUD_DVD_GPIO_HIGH_ENALBE = 1
}   AUD_DVD_GPIO_POLARITY_T;

/// Audio EQ table.
typedef enum
{
    AUD_EQ_OFF = 0,
    AUD_EQ_ROCK,
    AUD_EQ_POP,
    AUD_EQ_LIVE,
    AUD_EQ_DANCE,
    AUD_EQ_TECHNO,
    AUD_EQ_CLASSIC,
    AUD_EQ_SOFT,
    AUD_EQ_CUST1,
    AUD_EQ_CUST2,
    AUD_EQ_CUST3,
    AUD_EQ_CUST4,
    AUD_EQ_CUST5,
    AUD_EQ_CUST6,
    AUD_EQ_CUST7,
    AUD_EQ_CUST8,
    AUD_EQ_BBE_TV,
    AUD_EQ_BBE_AV,
    AUD_EQ_SBASS,
    AUD_EQ_NUM
}   AUD_EQ_TYPE_T;

/// Audio limiter flag table.
typedef enum
{
    AUD_LIMITER_VOL = 0,
    AUD_LIMITER_VSURR,
    AUD_LIMITER_SBASS,
    AUD_LIMITER_EQ,
    AUD_LIMITER_REVERB,
    AUD_LIMITER_SPEAKER,
    AUD_LIMITER_VOICE,
    AUD_LIMITER_NUM,
} AUD_LIMITER_FLAG_T;

/// Audio MixSnd table.
typedef enum
{
    AUD_MIXSND_CLIP0 = 0,
    AUD_MIXSND_CLIP1,
    AUD_MIXSND_CLIP2,
    AUD_MIXSND_CLIP3,
    AUD_MIXSND_CLIP_NUM
}   AUD_MIXSND_CLIP_T;

#ifdef CC_AUD_HDMI_SPDIF_CFG
typedef enum
{
    DEC_STOP = 0,
    DEC_PLAY = 1,
} HDMI_DEC_STATE;

typedef enum
{
    SPDIF_OFF = 0,
    SPDIF_BYPASS = 1,
    SPDIF_MUTE = 2,
} HDMI_SPDIF_STATE;

typedef enum
{
    ACP_NONE = 0,
    ACP_CD,
    ACP_DVD_AUDIO,
    ACP_SACD
} HDMI_ACPTYPE;

typedef enum
{
    PCMData = 0,
    RAWData = 1,
} HDMI_DATATYPE;

typedef enum
{
    FS_32KHZ = 0x03,
    FS_44KHZ = 0x00,
    FS_48KHZ = 0x02,
    FS_88KHZ = 0x08,
    FS_96KHZ = 0x0A,
    FS_176KHZ = 0x0C,
    FS_192KHZ = 0x0E,
    FS_UNKNOWN = 0x01,
} HDMI_AUD_FS;

typedef enum
{
    DigitalMute_AnalogMute = 0,         ///STOP_ALL = 0,
    DigitalUnMute_AnalogMute = 1,       ///STOP_SPDIF_BYPASS = 1,
    DigitalUnMute_AnalogUnMute = 2,     ///PLAY_SPDIF_BYPASS = 2,
    DigitalMute_AnalogUnMute = 3,       ///PLAY_SPDIF_MUTE = 3,
} AUD_OUTPUT_STATE;

typedef struct
{
    HDMI_ACPTYPE    eACPType;          ///0:ACP Packet is none, 1:CD, 2:DVD-Audio, 3:SACD
    HDMI_DATATYPE   eDataType;          ///0:PCM, 1:Compression (Raw).
    UINT8           u1CopyRight;        ///0: Copyright, 1: No Copyright
    UINT8           u1Category;         ///By HDMI Channel Status each 192 frames
    HDMI_AUD_FS     eSampeFreq;        ///0000:32KHz, 1100:44.1KHz, 0100:48KHz
} AUD_HDMI_CHL_STATUS;
#endif

/// Audio input mux select table.
typedef struct
{
    AUD_INPUT_ID_T  eAudInputId;
    UINT8           u1AdcMuxSel;        ///< ADC mux select, ex. WM8776(0~5)
    UINT8           u1SwGpioNum1Val;    ///< 0: Low, 1: High
    UINT8           u1SwGpioNum2Val;    ///< 0: Low, 1: High
    UINT8           u1SwGpioNum3Val;    ///< 0: Low, 1: High
    UINT8           u1SwGpioNum4Val;    ///< 0: Low, 1: High
} AUD_INPUT_MUX_SEL_T;

/// Audio input mux select table with size table.
typedef struct
{
    UINT32                     u4Size;
    const AUD_INPUT_MUX_SEL_T* prAudInMuxSel;
} AUD_INPUT_MUX_SEL_TABLE_T;

/// Audio input mux select switch table.
typedef struct
{
    UINT32      u4AudInSwGpioNum1;      ///< ex.GPIO 17
    UINT32      u4AudInSwGpioNum2;      ///< ex.GPIO 15
    UINT32      u4AudInSwGpioNum3;      ///< ex.GPIO xx
    UINT32      u4AudInSwGpioNum4;      ///< ex.GPIO yy
}   AUD_INPUT_SW_GPIO_T;

/// Audio dac volume table.
typedef struct
{
    UINT32      u4LineOutVol;
    UINT32      u4HeadphoneVol;
    UINT32      u4SpeakerVol;
} AUD_ADAC_VOL_CFG_T;

/// Audio dac usage table.
typedef struct
{
    AUD_ADAC_T  eAdac1;
    AUD_ADAC_T  eAdac2;
    AUD_ADAC_T  eAdac3;
    AUD_ADAC_T  eAdac4;
    AUD_CODEC_T eCodec1;
    AUD_CODEC_T eCodec2;
    AUD_AMP_T   eAudAmp1;
    AUD_AMP_T   eAudAmp2;
} AUD_ADAC_USAGE_T;

/// Audio aout PAD config table.
typedef struct
{
    AUD_CH_T    ePadSdata0;
    AUD_CH_T    ePadSdata1;
    AUD_CH_T    ePadSdata2;
    AUD_CH_T    ePadSdata3;
    AUD_CH_T    ePadSdata4;
    AUD_CH_T    ePadSdata5;
} AUD_AOUT_PAD_CFG_T;

/// Audio internal dac channel config table.
typedef struct
{
    AUD_CHL_DEF_T eDac0ChlSel;
    AUD_CHL_DEF_T eDac1ChlSel;
    AUD_CHL_DEF_T eDac2ChlSel;
    AUD_CHL_DEF_T eDac3ChlSel;
} AUD_INTER_DAC_CFG_T;

typedef struct
{
    UINT32 u4Size;
    const UINT8* prAudSetupMelody;
} AUD_SETUP_MELODY_TABLE_T;


/// Audio ATV decoder type table.
typedef enum
{
    AUD_ATV_DECODER_PAL,
    AUD_ATV_DECODER_A2,
    AUD_ATV_DECODER_MTS,
    AUD_ATV_DECODER_FMFM,
    AUD_ATV_DECODER_NUM
} AUD_ATV_DECODER_TYPE_T;

/// Audio factory MTS detection item table.
typedef enum
{
    AUD_FACTORY_MTS_NUM_CHECK,          ///< numbers of check
    AUD_FACTORY_MTS_NUM_PILOT,          ///< numbers of pilot
    AUD_FACTORY_MTS_PILOT_HIGH,         ///< pilot higher threshold
    AUD_FACTORY_MTS_PILOT_LOW,          ///< pilot lower threshold
    AUD_FACTORY_MTS_NUM_SAP,            ///< numbers of sap
    AUD_FACTORY_MTS_SAP_HIGH,           ///< sap higher threshold
    AUD_FACTORY_MTS_SAP_LOW             ///< sap lower threshold
} AUD_FACTORY_MTS_DETECTION_ITEM_T;

/// Audio factory FM mute table.
typedef enum
{
    AUD_FACTORY_FM_MUTE,                ///< carrier sense mute mode on/off
    AUD_FACTORY_FM_MUTE_HIGH,           ///< higher carrier sense mute threshold
    AUD_FACTORY_FM_MUTE_LOW,             ///< lower carrier sense mute threshold
    AUD_FACTORY_FM_MUTE_HIGH_HDEV, ///< higher carrier sense mute threshold for hdev mode
    AUD_FACTORY_FM_MUTE_LOW_HDEV  ///< lower carrier sense mute threshold for hdev mode
} AUD_FACTORY_FM_MUTE_ITEM_T;

/// Audio factory A2 detection table.
typedef enum
{
    AUD_FACTORY_A2_NUM_CHECK,           ///< numbers of check
    AUD_FACTORY_A2_NUM_DOUBLE,          ///< numbers of double check
    AUD_FACTORY_A2_MONO_WEIGHT,         ///< mono wieght
    AUD_FACTORY_A2_STEREO_WEIGHT,       ///< stereo wieght
    AUD_FACTORY_A2_DUAL_WEIGHT          ///< dual wieght
} AUD_FACTORY_A2_DETECTION_ITEM_T;

/// Audio factory A2 threshold table.
typedef enum
{
    AUD_FACTORY_A2_MONO2STEREO,
    AUD_FACTORY_A2_STEREO2MONO,
    AUD_FACTORY_A2_WEAK_RF_LVL_MONO2STEREO,
    AUD_FACTORY_A2_WEAK_RF_LVL_STEREO2MONO,
} AUD_FACTORY_A2_THRESHOLD_ITEM_T;

/// Audio factory PAL detection table.
typedef enum
{
    AUD_FACTORY_PAL_CORRECT,            ///< correct threshold
    AUD_FACTORY_PAL_SYNC_LOOP,          ///< total sync loop
    AUD_FACTORY_PAL_ERROR,              ///< error threshold
    AUD_FACTORY_PAL_PARITY_ERROR,       ///< parity error threshold
    AUD_FACTORY_PAL_NUM_FRAMES          ///< every number frames
} AUD_FACTORY_PAL_DETECTION_ITEM_T;

/// Audio factory PAL_AM mute table.
typedef enum
{
    AUD_FACTORY_PAL_AM_MUTE,            ///< AM mute mode on/off
    AUD_FACTORY_PAL_AM_MUTE_HIGH,       ///< higher AM mute threshold
    AUD_FACTORY_PAL_AM_MUTE_LOW         ///< lower AM mute threshold
} AUD_FACTORY_PAL_AM_MUTE_ITEM_T;

/// Audio factory FM_FM detection table.
typedef enum
{
    AUD_FACTORY_FMFM_NUM_CHECK,         ///< numbers of check
    AUD_FACTORY_FMFM_NUM_DOUBLE,        ///< numbers of double check
    AUD_FACTORY_FMFM_MONO_WEIGHT,       ///< mono wieght
    AUD_FACTORY_FMFM_STEREO_WEIGHT,     ///< stereo wieght
    AUD_FACTORY_FMFM_DUAL_WEIGHT,       ///< dual wieght
    AUD_FACTORY_FMFM_DETECT_CONFID
} AUD_FACTORY_FMFM_DETECTION_ITEM_T;

typedef enum
{
    AUD_FACTORY_SAP_MUTE_LOWER,         /* higher AM mute threshold */
    AUD_FACTORY_SAP_MUTE_HIGHER,         /* lower AM mute threshold */
    AUD_FACTORY_SAP_HP_MUTE_HIGHER,  /* higher SAP HPF mute threshold */
    AUD_FACTORY_SAP_HP_MUTE_LOWER,   /* higher SAP HPF mute threshold */
    AUD_FACTORY_SAP_FILTER_SEL,             /* SAP filter selection */
    AUD_FACTORY_SAP_RATIO                      /* SAP SNR  */
} AUD_FACTORY_SAP_MUTE_ITEM_T;

typedef enum
{
    AUD_FACTORY_SATU_MUTE_LOWER,         /* higher satutaion mute threshold */
    AUD_FACTORY_SATU_MUTE_HIGHER         /* lower satutaion mute threshold */
} AUD_FACTORY_SATU_MUTE_ITEM_T;

typedef struct
{
    UINT16 u2PeqFc;
    UINT8  u1PeqQ;         
    INT16 i2PeqGain;
}  AUD_PEQ_CFG_T;

typedef enum
{   
    AUD_PEQ_CASE0 = 0, //Wall Normal & Clear sound on
    AUD_PEQ_CASE1,  //Table Normal & Clear sound on
    AUD_PEQ_CASE2,  // Wall Skype
    AUD_PEQ_CASE3,  // Table Skype
    AUD_PEQ_CASE4,  // Wall Normal & Clear sound off
    AUD_PEQ_CASE5,  // Table Normal & Clear sound off
    AUD_PEQ_CASE_NUM
}AUD_PEQ_CASE_T;

typedef enum
{
    AUD_SM_CASE0 = 0,
    AUD_SM_CASE1,
    AUD_SM_CASE2,
    AUD_SM_CASE3,
    AUD_SM_CASE4,
    AUD_SM_CASE5,
    AUD_SM_CASE6,
    AUD_SM_CASE7,
    AUD_SM_CASE8,
    AUD_SM_CASE9,
    AUD_SM_CASE10,
    AUD_SM_CASE11,
    AUD_SM_CASE12,
    AUD_SM_CASE13,
    AUD_SM_CASE14,
    AUD_SM_CASE15,
    AUD_SM_NUM
}   AUD_POST_SM_T;
typedef enum
{
    AUD_SOUND_MODE_DEFAULT = 0,
    AUD_SOUND_MODE_STANDARD ,
    AUD_SOUND_MODE_DYNAMIC,
    AUD_SOUND_MODE_CLEAR_VOICE,
    AUD_SOUND_MODE_NUM
}   AUD_US_SOUND_MODE_T;

typedef enum
{
    AUD_VS_CASE1 = 0,
    AUD_VS_CASE2,
    AUD_VS_CASE3,
    AUD_VS_CASE4,
    AUD_VS_CASE5,
    AUD_VS_NUM
}   AUD_VS_MODE_T;

typedef struct
{
    UINT16 u2NumCheck;
    UINT16 u2NumPilot;              // stereo detectio
    UINT16 u2PilotHigherThd;
    UINT16 u2PilotLowerThd;
    UINT16 u2NumSap;              // SAP detectio
    UINT16 u2SapHigherThd;
    UINT16 u2SapLowerThd;
    UINT8 u1HdevMode;
    BOOL fgCarrierShift;
    BOOL fgSatuMuteMode;
    UINT8 u1SatuMuteHigherThd;
    UINT8 u1SatuMuteLowerThd;
    BOOL fgCarrierMuteMode;
    UINT8 u1CarrierMuteHigherThd;
    UINT8 u1CarrierMuteLowerThd;
    UINT8 u1MonoStereoPrescale;
    UINT8 u1SapPrescale;
    BOOL fgPilotOffsetDetect;
    UINT8 u1SapNoiseMuteHigh;
    UINT8 u1SapNoiseMuteLow;
    UINT8 u1SAPFilterType;  /// [0:auto/1:manual]
    UINT8 u1SAPFilterMode;  /// [0:big/1:small]
#ifdef CC_AUD_MTS_OUTPUT_LEVEL_GAIN_SUPPORT
    UINT32 u2OutputLevelGain;
#endif
}  AUD_MTS_FAC_T;

typedef struct
{
    UINT16 u2NumCheck;
    UINT16 u2NumDoubleCheck;
    UINT16 u2MonoWeight;
    UINT16 u2StereoWeight;
    UINT16 u2DualWeight;
    BOOL fgHdevMode;
    BOOL fgCarrierShift;
    BOOL fgCarrierMuteMode;
    UINT8 u1CarrierMuteHigherThd;
    UINT8 u1CarrierMuteLowerThd;
    UINT8 u1A2Prescale;
    BOOL fgSatuMuteMode;
    BOOL fgNonEuMode;
    UINT16 u2Mono2StereoEU;
    UINT16 u2Stereo2MonoEU;
    UINT16 u2Mono2StereoM;
    UINT16 u2Stereo2MonoM;
    UINT16 u2WeakRfLvlM2SEU;
    UINT16 u2WeakRfLvlS2MEU;
}  AUD_A2_FAC_T;

typedef struct
{
    UINT16 u2CorrectThd;
    UINT16 u2TotalSyncLoop;
    UINT16 u2ErrorThd;
    UINT16 u2ParityErrorThd;
    UINT16 u2EveryNumFrames;
    BOOL fgHdevMode;
    BOOL fgAmMuteMode;
    UINT8 u1AmMuteHigherThd;
    UINT8 u1AmMuteLowerThd;
    BOOL fgCarrierShift;
    BOOL fgCarrierMuteMode;
    UINT8 u1CarrierMuteHigherThd;
    UINT8 u1CarrierMuteLowerThd;
#ifdef CC_AUD_SX1_FEATURE
    UINT8 u1CarrierMuteHdevHigherThd;
    UINT8 u1CarrierMuteHdevLowerThd;
#endif
    UINT8 u1PalPrescale;
    UINT8 u1AmPrescale;
    UINT8 u1NicamPrescale;
    BOOL fgSatuMuteMode;
    BOOL fgNonEuMode;
    BOOL fgNicamIBoost;
}  AUD_PAL_FAC_T;

typedef struct
{
    BOOL fgCLK;
    BOOL fgAOSDATA0;
    BOOL fgAOSDATA1;
    BOOL fgAOSDATA2;
    BOOL fgAOSDATA3;
    BOOL fgAOSDATA4;
} AUD_DIGITAL_DATA_T;

typedef enum
{
    AUD_PEQ_Q_0_33 = 0,
    AUD_PEQ_Q_0_43,
    AUD_PEQ_Q_0_56,
    AUD_PEQ_Q_0_75,
    AUD_PEQ_Q_1_00,
    AUD_PEQ_Q_1_20,
    AUD_PEQ_Q_1_50,
    AUD_PEQ_Q_1_80,
    AUD_PEQ_Q_2_20,
    AUD_PEQ_Q_2_27,
    AUD_PEQ_Q_3_30,
    AUD_PEQ_Q_3_39,
    AUD_PEQ_Q_4_70,
    AUD_PEQ_Q_5_60,
    AUD_PEQ_Q_6_80,
    AUD_PEQ_Q_8_20,
    AUD_PEQ_Q_MAX
} AUD_PEQ_Q_TYPE_T;

//---------------------------------------------------------------------------
// Global Variable
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Type definitions
//---------------------------------------------------------------------------


/* Note that this enumeration should be consistent with middleware x_aud_dec.h  */
typedef enum
{
    AUD_FMT_UNKNOWN = 0,
    AUD_FMT_MPEG,
    AUD_FMT_AC3,
    AUD_FMT_PCM,
    AUD_FMT_MP3,
    AUD_FMT_AAC,
    AUD_FMT_DTS,
    AUD_FMT_WMA,
    AUD_FMT_RA,
    AUD_FMT_HDCD,
    AUD_FMT_MLP,     // 10
    AUD_FMT_MTS,
    AUD_FMT_EU_CANAL_PLUS,
    AUD_FMT_PAL,
    AUD_FMT_A2,
    AUD_FMT_FMFM,
    AUD_FMT_NICAM,
    AUD_FMT_TTXAAC,
    AUD_FMT_DETECTOR,
    AUD_FMT_MINER,
    AUD_FMT_LPCM,       // 20
    AUD_FMT_FMRDO,
    AUD_FMT_FMRDO_DET,
    AUD_FMT_SBCDEC,
    AUD_FMT_SBCENC,     // 24,
    AUD_FMT_MP3ENC,     // 25, MP3ENC_SUPPORT
    AUD_FMT_G711DEC,    // 26
    AUD_FMT_G711ENC,    // 27
    AUD_FMT_DRA,        // 28
    AUD_FMT_COOK,        // 29
    AUD_FMT_G729DEC,     // 30
    AUD_FMT_VORBISDEC,    //31, CC_VORBIS_SUPPORT
    AUD_FMT_WMAPRO,    //32  please sync number with middleware\res_mngr\drv\x_aud_dec.h
    AUD_FMT_HE_AAC,     //33 Terry Added for S1 UI notification
    AUD_FMT_HE_AAC_V2,  //34
    AUD_FMT_AMR,        //35 amr-nb run in DSP
    AUD_FMT_AWB,        //36 amr-wb run in DSP
    AUD_FMT_APE,        //37 //ian APE decoder
    AUD_FMT_FLAC = 39,  //39, paul_flac
    AUD_FMT_G726 = 40,  //40, G726 decoder 

    AUD_FMT_TV_SYS = 63, //63, sync from x_aud_dec.h
    AUD_FMT_OMX_MSADPCM = 0x80,
    AUD_FMT_OMX_IMAADPCM = 0x81,
    AUD_FMT_OMX_ALAW = 0x82,
    AUD_FMT_OMX_ULAW = 0x83,
    AUD_FMT_WMA10LOSSLESS,//0x84, reserved for future development
    AUD_FMT_WMA10SPEECH,//0x85, reserved for future development    
    AUD_FMT_OMX_LPCM_TS,
    AUD_FMT_OMX_LPCM_PRIVATE1,
    AUD_FMT_OMX_LPCM_TSDVD,
}   AUD_FMT_T;

/* Move AUD_DEC_STREAM_FROM_T to drv_cust/drvcust_if.h */

// Macro to create bit mask
#define MAKE_BIT_MASK(_val)     (((UINT32) 1) << (UINT32)_val)

/* The following enumeration and bitmasks are used to convey */
/* TV-System type information.                               */
/* Note that this enumeration should be consistent with middleware x_aud_dec.h  */
typedef enum
{
    AUD_TV_SYS_UNKNOWN = -1, /* Must be set to '-1' else I loose an entry in the bit mask. */
    AUD_TV_SYS_A = 0,
    AUD_TV_SYS_B = 1,
    AUD_TV_SYS_C = 2,
    AUD_TV_SYS_D = 3,
    AUD_TV_SYS_E = 4,
    AUD_TV_SYS_F = 5,
    AUD_TV_SYS_G = 6,
    AUD_TV_SYS_H = 7,
    AUD_TV_SYS_I = 8,
    AUD_TV_SYS_J = 9,
    AUD_TV_SYS_K = 10,
    AUD_TV_SYS_K_PRIME = 11,
    AUD_TV_SYS_L = 12,
    AUD_TV_SYS_L_PRIME = 13,
    AUD_TV_SYS_M = 14,
    AUD_TV_SYS_N = 15,
    AUD_TV_SYS_A2 = 16,
    AUD_TV_SYS_PAL = 17,
    AUD_TV_SYS_NICAM = 18,
    AUD_TV_SYS_SECAM = 19
}   AUD_TV_SYS_T;

#define AUD_TV_SYS_MASK_A               MAKE_BIT_MASK (AUD_TV_SYS_A)
#define AUD_TV_SYS_MASK_B               MAKE_BIT_MASK(AUD_TV_SYS_B)
#define AUD_TV_SYS_MASK_C               MAKE_BIT_MASK(AUD_TV_SYS_C)
#define AUD_TV_SYS_MASK_D               MAKE_BIT_MASK(AUD_TV_SYS_D)
#define AUD_TV_SYS_MASK_E               MAKE_BIT_MASK(AUD_TV_SYS_E)
#define AUD_TV_SYS_MASK_F               MAKE_BIT_MASK(AUD_TV_SYS_F)
#define AUD_TV_SYS_MASK_G               MAKE_BIT_MASK(AUD_TV_SYS_G)
#define AUD_TV_SYS_MASK_H               MAKE_BIT_MASK(AUD_TV_SYS_H)
#define AUD_TV_SYS_MASK_I               MAKE_BIT_MASK(AUD_TV_SYS_I)
#define AUD_TV_SYS_MASK_J               MAKE_BIT_MASK(AUD_TV_SYS_J)
#define AUD_TV_SYS_MASK_K               MAKE_BIT_MASK(AUD_TV_SYS_K)
#define AUD_TV_SYS_MASK_K_PRIME         MAKE_BIT_MASK(AUD_TV_SYS_K_PRIME)
#define AUD_TV_SYS_MASK_L               MAKE_BIT_MASK(AUD_TV_SYS_L)
#define AUD_TV_SYS_MASK_L_PRIME         MAKE_BIT_MASK(AUD_TV_SYS_L_PRIME)
#define AUD_TV_SYS_MASK_M               MAKE_BIT_MASK(AUD_TV_SYS_M)
#define AUD_TV_SYS_MASK_N               MAKE_BIT_MASK(AUD_TV_SYS_N)

#define AUD_TV_SYS_MASK_A2              MAKE_BIT_MASK(AUD_TV_SYS_A2)
#define AUD_TV_SYS_MASK_PAL             MAKE_BIT_MASK(AUD_TV_SYS_PAL)
#define AUD_TV_SYS_MASK_NICAM           MAKE_BIT_MASK(AUD_TV_SYS_NICAM)
#define AUD_TV_SYS_MASK_SECAM           MAKE_BIT_MASK(AUD_TV_SYS_SECAM)

/* The following enumeration and bitmasks are used to convey */
/* Audio-System type information.                            */
/* Note that this enumeration should be consistent with middleware x_aud_dec.h */
typedef enum
{
    AUD_SYS_UNKNOWN = -1, /* Must be set to '-1' else I loose an entry in the bit mask. */
    AUD_SYS_AM,
    AUD_SYS_FM_MONO,
    AUD_SYS_FM_EIA_J,
    AUD_SYS_FM_A2,
    AUD_SYS_FM_A2_DK1,
    AUD_SYS_FM_A2_DK2,
    AUD_SYS_FM_RADIO,
    AUD_SYS_NICAM,
    AUD_SYS_BTSC
}   AUD_SYS_T;

#define AUD_SYS_MASK_AM                 MAKE_BIT_MASK (AUD_SYS_AM)
#define AUD_SYS_MASK_FM_MONO            MAKE_BIT_MASK (AUD_SYS_FM_MONO)
#define AUD_SYS_MASK_FM_EIA_J           MAKE_BIT_MASK (AUD_SYS_FM_EIA_J)
#define AUD_SYS_MASK_FM_A2              MAKE_BIT_MASK (AUD_SYS_FM_A2)
#define AUD_SYS_MASK_FM_A2_DK1          MAKE_BIT_MASK (AUD_SYS_FM_A2_DK1)
#define AUD_SYS_MASK_FM_A2_DK2          MAKE_BIT_MASK (AUD_SYS_FM_A2_DK2)
#define AUD_SYS_MASK_FM_RADIO           MAKE_BIT_MASK (AUD_SYS_FM_RADIO)
#define AUD_SYS_MASK_NICAM              MAKE_BIT_MASK (AUD_SYS_NICAM)
#define AUD_SYS_MASK_BTSC               MAKE_BIT_MASK (AUD_SYS_BTSC)

/* Note that this enumeration should be consistent with middleware x_aud_dec.h */
/// Audio middleware system table.
typedef struct _MW_TV_AUD_SYS_T
{
    UINT32  u4TvSysMask;
    UINT32  u4AudSysMask;
} MW_TV_AUD_SYS_T;

#ifndef _TV_AUD_SYS_T
#define _TV_AUD_SYS_T
typedef enum
{
    SV_NONE_DETECTED = 0x0,     //0x0
    SV_MTS           = 0x1,     //0x1
    SV_FM_FM         = 0x2,     //0x2
    SV_NTSC_M        = 0x3,     //0x3
    SV_A2_BG         = 0x4,     //0x4
    SV_A2_DK1        = 0x5,     //0x5
    SV_A2_DK2        = 0x6,     //0x6
    SV_A2_DK3        = 0x7,     //0x7
    SV_PAL_I         = 0x8,     //0x8
    SV_PAL_BG        = 0x9,     //0x09
    SV_PAL_DK        = 0xa,     //0x0a
    SV_SECAM_L       = 0xb,     //0x0b
    SV_SECAM_L_PLUM  = 0xc,     //0x0c
    SV_SECAM_BG      = 0xd,     //0x0d, 0x0c is reserved for SECAM L' setting for tuner
    SV_SECAM_DK      = 0xe,      //0x0e
    SV_AUD_SYS_NUM   = 0xf
} TV_AUD_SYS_T;
#endif

/* Note that this enumeration should be consistent with AUD_DEC_MTS_T in middleware x_aud_dec.h */
typedef enum
{
    AUD_SOUND_MODE_UNKNOWN = 0,
    AUD_SOUND_MODE_MONO,
    AUD_SOUND_MODE_STEREO,
    AUD_SOUND_MODE_SUB_LANG,
    AUD_SOUND_MODE_DUAL1,
    AUD_SOUND_MODE_DUAL2,
    AUD_SOUND_MODE_NICAM_MONO,
    AUD_SOUND_MODE_NICAM_STEREO,
    AUD_SOUND_MODE_NICAM_DUAL1,
    AUD_SOUND_MODE_NICAM_DUAL2,
    AUD_SOUND_MODE_A2_DUAL1_DUAL2,
    AUD_SOUND_MODE_NICAM_DUAL1_DUAL2,
    AUD_SOUND_MODE_FM_MONO,
    AUD_SOUND_MODE_FM_STEREO
} AUD_SOUND_MODE_T;

typedef enum
{
    AUD_MATRIX_OUT_OFF = 0,
    AUD_MATRIX_OUT_L_MONO,
    AUD_MATRIX_OUT_R_MONO,
    AUD_MATRIX_OUT_LR_MIX
} AUD_MATRIX_OUT_T;

/// Audio mem buffer information table.
typedef struct
{
    UINT8* pData;
    UINT32 u4Length;
    UINT32 u4LoopCount;
}   MEM_BUFFER_INFO_T;

#ifdef CC_AUD_MULTI_BUFFER_MEMORY_PLAY
typedef struct
{
    UINT8* pData;
    UINT32 u4Length;
    UINT32 u4BufferSize;
    UINT32 u4Bid;   // Block data id
    BOOL   fgValid;
    BOOL   fgEOB;   // End of block data
} MEM_INFO_T;
#endif

typedef enum
{
    AUD_DUAL_DEC_UNINITIALIZED = 0,
    AUD_DUAL_DEC_INIT_AUX_DECODER,
    AUD_DUAL_DEC_PLAY,
    AUD_DUAL_DEC_STOP,
    AUD_DUAL_DEC_IDLE
}   AUD_DUAL_DEC_DRV_STATE_T;

typedef enum
{
    AUD_UNINITIALIZED = 0,
    AUD_TRIGGER_ADSP,
    AUD_WAIT_POWER_ON,
    AUD_INIT,
    AUD_WAIT_CMD,
    AUD_WAIT_PLAY,
    AUD_WAIT_PAUSE,
    AUD_WAIT_RESUME,
    AUD_WAIT_STOP,
    AUD_WAIT_STOPSTREAM,
    AUD_IDLE,
    AUD_ON_PLAY,
    AUD_ON_PAUSE,
    AUD_STOPPED,
    AUD_DECODE_INIT,
    AUD_CHANGE_FORMAT,
    AUD_DSP_ERROR
}   AUD_DRV_STATE_T;

typedef enum
{
    AV_SYNC_FREE_RUN = 0,       // no need to syn
    AV_SYNC_SLAVE,              // syn to STC
    AV_SYNC_AUDIO_MASTER       // update A-PTS to A-STC, may use in PCR or HardDisk playback
}   AV_SYNC_MODE_T;

typedef enum
{
    AUD_MM_MODE_NORMAL = 0,
    AUD_MM_MODE_KARAOKE,
    AUD_MM_MODE_KARAOKE_MIC_PASS,
    AUD_MM_MODE_GAME
}   AUD_MM_MODE_T;

typedef enum
{
    AUD_PATH_NONE,
    AUD_PATH_MAIN,
    AUD_PATH_SUB
}    AUD_PATH_T;

typedef enum
{
    AV_SYNC_STC_A1 = 0,
    AV_SYNC_STC_A2,
    AV_SYNC_STC_V1,
    AV_SYNC_STC_V2
}   AV_SYNC_STC_SELECT_T;

/* Audio command types. */
typedef enum
{
    AUD_CMD_PLAY       = 0,
    AUD_CMD_STOP       = 1,
    AUD_CMD_RESET      = 2,
    AUD_CMD_PAUSE      = 3,
    AUD_CMD_AVSYNC     = 4,
    AUD_CMD_LOADCODE   = 5,
    AUD_CMD_RESUME  = 6,
    AUD_CMD_CHANGE_FORMAT = 7,
    AUD_CMD_STOPSTREAM = 8,
    AUD_CMD_UNMUTE     = 9,
    AUD_CMD_PLAY_SKIP  = 10,
    AUD_CMD_STOP_SKIP  = 11,     
    AUD_CMD_NULL       = 12
}   AUD_DEC_CMD_T;

/* Audio command state.*/
typedef enum
{
    AUD_CMD_ST_PLAY    = 0,
    AUD_CMD_ST_STOP    = 1,
    AUD_CMD_ST_NULL
}   AUD_DEC_CMD_STATE_T;

/* Audio dual decoder command types. */
typedef enum
{
    AUD_DUAL_DEC_CMD_PLAY    = 0,
    AUD_DUAL_DEC_CMD_STOP    = 1
}   AUD_DUAL_DEC_CMD_T;

/* Audio play mute command types. */
typedef enum
{
    AUD_PM_OP_SET_DELAY = 0,
    AUD_PM_OP_UNMUTE,
    AUD_PM_CMD_RESET,
    AUD_PM_CMD_VDP_MODE_CHG,
    AUD_PM_CMD_MUTE,
    AUD_PM_CMD_VDP_MODE_CHG_DONE,
    AUD_PM_CMD_DSP_PLAY,
    AUD_PM_CMD_UNMUTE_WITH_VDP,
    AUD_PM_CMD_UNMUTE_TIME,
    AUD_PM_CMD_SET_PIP_VDO_PATH,
    AUD_PM_CMD_SET_DELAY_READY,
    AUD_PM_CMD_AOUT_ENABLE,
    AUD_PM_CMD_NUM
}   AUD_PLAY_MUTE_CMD_T;

/* IEC types. */
typedef enum
{
    AUD_IEC_CFG_PCM = 0,
    AUD_IEC_CFG_RAW
}   AUD_IEC_T;

/* IEC PCM Channel */
typedef enum
{
    AUD_IEC_PCM_CH_L_R = 0,
    AUD_IEC_PCM_CH_LS_RS,
    AUD_IEC_PCM_CH_C_SW,
    AUD_IEC_PCM_CH_7_8,
    AUD_IEC_LINE_IN,
    AUD_IEC_PCM_CH_9_10,
    AUD_IEC_AUX
}   AUD_IEC_CH_T;

typedef enum
{
    IEC_RAW_SRC_OFF = 0,
    IEC_RAW_SRC_DEC1_ON = 0x10,
    IEC_RAW_SRC_DEC2_ON = 0x20,
    IEC_RAW_SRC_DEC3_ON = 0x40
} AUD_IEC_RAWSRC_T;

typedef struct
{
    // spdif information
    AUD_IEC_T eIecCfg; // user select
    // driver register information
    SPDIF_REG_TYPE_T eSpdifRegType;
    // control information
    AUD_DEC_STREAM_FROM_T eStreamSrc;
    AUD_FMT_T eDecFormat;
} AUD_IECINFO_T;

typedef enum
{
    FS_16K = 0x00,
    FS_22K,
    FS_24K,
    FS_32K,
    FS_44K,
    FS_48K,
    FS_64K,
    FS_88K,
    FS_96K,
    FS_176K,
    FS_192K,
    FS_8K, // appended since 09/10/2007, don't change the order
    FS_11K, // appended since 09/10/2007, don't change the order
    FS_12K, // appended since 09/10/2007, don't change the order
    FS_52K, // appended since 24/02/2010, don't change the order
    FS_56K,
    FS_62K,  // appended since 24/02/2010, don't change the order
    FS_64K_SRC,
    FS_6K,
    FS_10K,
    FS_5K
}   SAMPLE_FREQ_T;

/* Audio types. */
typedef enum
{
    AUD_TYPE_UNKNOWN = 0,
    AUD_TYPE_MONO,                  /* 1/0 */
    AUD_TYPE_MONO_SUB,              /* 1+sub-language */
    AUD_TYPE_DUAL_MONO,             /* 1+1 */
    AUD_TYPE_STEREO,                /* 2/0 */
    AUD_TYPE_STEREO_SUB,            /* 2+sub-language */
    AUD_TYPE_STEREO_DOLBY_SURROUND, /* 2/0, dolby surround */
    AUD_TYPE_SURROUND_2CH,          /* 2/1 */
    AUD_TYPE_SURROUND,              /* 3/1 */
    AUD_TYPE_3_0,                   /* 3/0 */
    AUD_TYPE_4_0,                   /* 2/2 */
    AUD_TYPE_5_1,                   /* 3/2 +SW */
    AUD_TYPE_7_1,                   /* 5/2 */
    AUD_TYPE_FM_MONO_NICAM_MONO,
    AUD_TYPE_FM_MONO_NICAM_STEREO,
    AUD_TYPE_FM_MONO_NICAM_DUAL,
    AUD_TYPE_OTHERS,
    AUD_TYPE_5_0,                    /* 3/2 */
    AUD_TYPE_JOINT_STEREO,
    AUD_TYPE_FMRDO_MONO,
    AUD_TYPE_FMRDO_STEREO,

    AUD_TYPE_MONO_LFE,                 /* 1/0.L */
    AUD_TYPE_DUAL_MONO_LFE,            /* 1+1.L */
    AUD_TYPE_STEREO_LFE,               /* 2/0.L */
    AUD_TYPE_SURROUND_2CH_LFE,         /* 2/1.L */
    AUD_TYPE_SURROUND_LFE,             /* 3/1.L */
    AUD_TYPE_3_1,                  /* 3/0.L */
    AUD_TYPE_4_1,                  /* 2/2.L */

    AUD_TYPE_LEFT_SIDE_STEREO,     //paul_flac
    AUD_TYPE_RIGHT_SIDE_STEREO,
    AUD_TYPE_MID_SIDE_STEREO
}   AUD_TYPE_T;

/* audio decoder types. */
typedef enum
{
    AUD_DECODE_TYPE_UNKNOWN = 0,
    AUD_DECODE_TYPE_AC3,
    AUD_DECODE_TYPE_EAC3,
    AUD_DECODE_TYPE_FLAC,//paul_flac
    AUD_DECODE_TYPE_AAC,
    AUD_DECODE_TYPE_HEAAC,
    AUD_DECODE_TYPE_HEAAC_V2,
    AUD_DECODE_TYPE_LPCM_ALAW,
    AUD_DECODE_TYPE_LPCM_ULAW,
    AUD_DECODE_TYPE_MPEG1_LAYER3,
    AUD_DECODE_TYPE_MPEG1_LAYER2,
    AUD_DECODE_TYPE_MPEG1_LAYER1,
    AUD_DECODE_TYPE_MPEG2_LAYER3,
    AUD_DECODE_TYPE_MPEG2_LAYER2,
    AUD_DECODE_TYPE_MPEG2_LAYER1
} AUD_DECODE_TYPE_T;

/// Audio ain configuration table.
typedef struct
{
    AUD_DEC_STREAM_FROM_T eStrSrc;      ///< Stream source
    DATA_FORMAT_T      eFormat;         ///< format of alignment
    DAC_DATA_NUMBER_T  eBits;           ///< number of bits per sample
    LRCK_CYC_T         eCycle;          ///< cycles per sample
    MCLK_FREQUENCY_T   eSampleFreq;     ///< DAC sampling frequence
    BOOL               fgDataInvert;    ///< Invert audio output for OP phase
    BOOL               fgLRInvert;      ///< Invert L/R audio output
    //========================================================================
    UINT8 uFormat;                      ///< format of alignment
    UINT8 uBits;                        ///< number of bits per sample
    UINT8 uCycle;                       ///< cycles per sample
    BOOL  fgIsSPDIFin;                  ///< TRUE is slave mode
    BOOL  fgLrckInv;                    ///< TRUE while LRCK Low is Left channel
}   AIN_CFG_T;

typedef enum
{
    AVC_COMP_VIDEO_0 = 0,               ///< Composite video (CVBS) # 0
    AVC_COMP_VIDEO_1,                   ///< Composite video (CVBS) # 1
    AVC_COMP_VIDEO_2,                   ///< Composite video (CVBS) # 2
    AVC_S_VIDEO_0,                      ///< S-Video
    AVC_Y_PB_PR_0,                      ///< YPbPr # 0
    AVC_Y_PB_PR_1,                      ///< YPbPr # 1
    AVC_VGA,                            ///< VGA, via headphone jet
    AVC_HDMI,                           ///< HDMI (DVI)
    AVC_S_VIDEO_1,
    AVC_S_VIDEO_2
}   AV_COMPONENT_T;

/// Audio decoder information table.
typedef struct
{
    AUD_FMT_T   e_aud_fmt;
    AUD_TYPE_T  e_aud_type;
    UINT32      ui4_sample_rate;
    UINT32      ui4_data_rate;
    UINT8       ui1_bit_depth;
    AUD_DECODE_TYPE_T e_dec_type;
} AUD_DEC_INFO_T;

typedef enum
{
    AUD_COND_ERROR = -1,
    AUD_COND_CTRL_DONE,
    AUD_COND_AUD_CLIP_DONE,
    AUD_COND_MEM_BUFFER_DONE,
    AUD_COND_FLUSH_DONE,
    AUD_COND_FEED_ME_ON,
    AUD_COND_FEED_ME_OFF,
    AUD_COND_AUD_INFO_CHG,
    AUD_COND_TV_SYS_DETECTED,
    AUD_COND_AUD_DECODE_STATUS_CHG,
    AUD_COND_MM_EOS,
    AUD_COND_FM_RADIO_DET_DONE,
    AUD_COND_PROTECT_INFO,       ///[Joel]20101011, CC_AUD_HDMI_SPDIF_CFG
    AUD_COND_UPLOAD_INFO_CHG,
    AUD_COND_BLUETOOTH_INFO_CHG,
    AUD_COND_AUD_CLIP_PLAY_DONE,
    AUD_COND_AUD_CODEC_CHG,      //audio codec change notify
    AUD_COND_AUD_BIT_RATE_CHG,
    AUD_COND_STREAM_FORMAT_CHG,
    AUD_COND_MUTE_STATE_CHG,
    AUD_COND_AUD_MEM_STATE_CHG,
    AUD_COND_INPUT_SOURCE_STATE_CHG,
}   AUD_COND_NFY_T;

typedef enum
{
    AUD_DECODE_UNKNOWN = -1, ///< initial value; the decode status should be initialized to this value after connect/disconnect or AUD_DEC_CTRL_STOP
    AUD_DECODE_NO_DATA,      ///< audio decoder found no audio data in stream. there will be a timer threashold value for decoder to change the decode status from NORMAL/ERROR to NO_DATA
    AUD_DECODE_NORMAL,     ///< audio decoder decode the first audio frame data successfully after AUD_DEC_CTRL_PLAY
    AUD_DECODE_ERROR,       ///< audio decoder decode the audio data abnormally (e.g. unsupported format)
    AUD_DECODE_NOT_SUPPORT, ///< unsupported format for WMA decoder
    AUD_DECODE_FMT_CONFLICT
}   AUD_DECODE_STATUS_T;

typedef enum
{
    AUD_MEM_UNKNOWN = -1,
    AUD_MEM_DECODE_DONE,
    AUD_MEM_ENDOF_DATA,
    AUD_MEM_NO_DATA,
    AUD_MEM_NORMAL,
    AUD_MEM_ERROR,
    AUD_MEM_NOT_SUPPORT
} AUD_MEM_STATE_T;

typedef enum
{
    AUD_SOURCE_UNKOWN = -1,
    AUD_SOURCE_NO_SIGNAL,
    AUD_SOURCE_STABLE,
    AUD_SOURCE_UNSTABLE,
} AUD_SOURCE_STATE_T;

typedef enum
{
    AUD_HP_COND_HEADPHONE_COND_UNPLUGED = 0,
    AUD_HP_COND_HEADPHONE_COND_PLUGED
} AUD_HP_COND_NFY_T;

typedef enum
{
    AUD_YW_COND_UPLOAD_ERROR = -1,
    AUD_YW_COND_UPLOAD_OK
} AUD_YW_COND_NFY_T;

typedef enum        //Andrew Wen : 2007/2/2 for STC differnece setting
{
    AUD_STC_LO = 0,
    AUD_STC_HI,
    AUD_STC_WS
}   AUD_STC_DIFF_SET;

typedef enum
{
    AUD_NFY_STREAM = 0,
    AUD_NFY_HP,
    AUD_NFY_YW
}   AUD_NFY_TAG_T;

/* Notify function */
typedef void (*AudDecNfyFct) (void*           pvNfyTag,
                              UINT8           u1DecId,
                              AUD_COND_NFY_T  eNfyCond,
                              UINT32          u4Data1,
                              UINT32          u4Data2);

/* Notify setting info. */
/// Audio Notify setting info table.
typedef struct
{
    void*           pvTag;

    AudDecNfyFct    pfAudDecNfy;
}   AUD_NFY_INFO_T;

/// Audio reverb mode table.
typedef struct
{
    UINT32      u4Gain;
    UINT8       u1ReverbBank0;
    UINT8       u1ReverbBank1;
    UINT8       u1ReverbBank2;
    UINT8       u1ReverbBank3;
} REVERB_MODE_T;

/// Audio PAL2 setting table.
typedef struct
{
    UINT16 u2Config;
    UINT16 u2Mode;
} PL2_SETTING_T;

/// Audio EQ flag table.
typedef struct
{
    UINT8       fgChEqOn       : 1;
    UINT8       fgBassBoostOn  : 1;
    UINT8       fgSuperBassOn  : 1;
    UINT8       fgClearBoostOn : 1;
    UINT8       Reserve        : 4;
}   EQ_FLAG_T;

typedef union
{
    UINT8       u1Eq;
    EQ_FLAG_T   rField;
} EQ_FLAG_UNION_T;

/// Audio channel band table.
typedef struct
{
    UINT8       u1Channel;
    UINT8       u1Band;
} CH_BAND_T;

/// Audio EQ band table.
typedef struct
{
    UINT8       u1BandIdx;
    INT8        i1Level;
}   EQ_BAND_CFG;

/// Audio EQ band information table.
typedef struct
{
    UINT8       ui1_band_idx;
    INT8        i1_max_lvl;
    INT8        i1_min_lvl;
    UINT32      ui4_freq_lower_bound;
    UINT32      ui4_freq_upper_bound;
} AUD_EQ_BAND_INFO_T;

// Test tone freqence definition for table selection
typedef enum
{
    TEST_TONE_1K_0dB = 0,
    TEST_TONE_1K_20dB,
    TEST_TONE_1K_60dB,
    TEST_TONE_1K_120dB,
    TEST_TONE_2K_0dB,
    TEST_TONE_3K_0dB,
    TEST_TONE_4K_0dB,
    TEST_TONE_5K_0dB,
    TEST_TONE_8K_0dB,
    TEST_TONE_12_5K_0dB,
    TEST_TONE_20K_0dB,
    TEST_TONE_200_0dB,
    TEST_TONE_200_60dB,
    TEST_TONE_400_0dB,
    TEST_TONE_1K3K_0dB,
    TEST_TONE_1K_0dB_LCH_ONLY,
    TEST_TONE_1K_MIX_0dB
} TEST_TONE_T;

/// Audio clip type table.
typedef struct
{
    BOOL        fgPlayTestTone;
    UINT32      u4OutputChlSelect;
    UINT32      u4RepeatCount;
    UINT32      u4ToneFreq;
    UINT32      u4AudCtrlTypeAddr;

    AUD_FMT_T           eAudFmt;
    MEM_BUFFER_INFO_T   eMemBuf;
} AUD_CLIP_TYPE_T;

/* BBE mode */
typedef enum
{
    AUD_BBE_MODE_OFF = 0,
    AUD_BBE_MODE_BBE,
    AUD_BBE_MODE_BBE_VIVA
}   AUD_BBE_MODE_T;

typedef enum
{
    AUD_FM_RADIO_NON_DETECTED = 0,
    AUD_FM_RADIO_DETECTED
} AUD_FM_RADIO_DET_T;

typedef enum
{
    AUD_VIDEO_IS_STABLE = 0,
    AUD_VIDEO_IS_UNSTABLE = 1
}   AUD_VIDEO_STATE_T;

//-----------------------------------------------------------------------------
// Begin of NVM and Profile Functions
//   for customers use EEPROM and FLASH audio parameters
//-----------------------------------------------------------------------------
// NVM Data Releated
#ifdef CC_AUD_USE_NVM
#define AUD_NVM_LENGTH (0xFF)
#define AUD_NVM_OFFSET (0x3500) /* For NVM layout issue */
#define AUD_NVM_PROFILE_OFFSET1 (0x0520) /* 120913 Isaac */
#define AUD_NVM_PROFILE_OFFSET2 (0x1050) /* 120913 Isaac */
#define AUD_NVM_PROFILE_OFFSET3 (0x1520) /* 120913 Isaac */
#define AUD_FLASH_LENGTH        (1152)    //2k13 v0.3
#define AUD_FLASH_COMMON_LENGTH (996)
//#define AUD_DD_BANNER_OFFSET (0x11D3)
#define AUD_NVM_FLAG        (0xaf)
#define CC_AUD_NVM_CHECK_VALID

typedef enum
{
    AUD_NVM_READ = 0,
    AUD_NVM_WRITE,
    AUD_NVM_INIT,
    AUD_NVM_VOL_INIT,
    AUD_NVM_PROFILE_QRY,
    AUD_NVM_VS_INIT,
    AUD_NVM_PROFILE_OFFSET
} AUD_NVM_OP_T;

typedef enum
{
    AUD_NVM_NOP = 0,
    // MTS
    AUD_NVM_MTS_OBSRV_TIME,
    AUD_NVM_MTS_STEREO_TIME,
    AUD_NVM_MTS_SAP_TIME,
    AUD_NVM_MTS_UP_THR_BTSC,
    AUD_NVM_MTS_LO_THR_BTSC,
    AUD_NVM_MTS_UP_THR_SAP,
    AUD_NVM_MTS_LO_THR_SAP,
    AUD_NVM_MTS_NMUTE_FM_UPTHR,
    AUD_NVM_MTS_NMUTE_FM_LOTHR,
    AUD_NVM_MTS_NMUTE_SAP_UPTHR,
    AUD_NVM_MTS_NMUTE_SAP_LOTHR,
    AUD_NVM_MTS_PRE_BTSC_STMONO,
    AUD_NVM_MTS_PRE_SAP,
    AUD_NVM_MTS_HIGH_DEV,
    AUD_NVM_MTS_CARRIER_SHIFT,
    AUD_NVM_MTS_FMCARR_MUTE,
    // A2
    AUD_NVM_A2_HDEV_MODE,    
    AUD_NVM_A2_CR_SHIFT,    
    AUD_NVM_A2_FM_MUTE,    
    AUD_NVM_A2_UP_THR_FM,    
    AUD_NVM_A2_LO_THR_FM,    
    AUD_NVM_A2_PRE,    
    AUD_NVM_A2_NON_EU,    
    // PAL
    AUD_NVM_PAL_HDEV_MODE,    
    AUD_NVM_PAL_CR_SHIFT,    
    AUD_NVM_PAL_AM_MUTE,    
    AUD_NVM_PAL_UP_THR_AM,    
    AUD_NVM_PAL_LO_THR_AM, 
    AUD_NVM_PAL_FM_MUTE,  
    AUD_NVM_PAL_UP_THR_FM,  
    AUD_NVM_PAL_LO_THR_FM,    
    AUD_NVM_PAL_PRE,    
    AUD_NVM_PAL_AM_PRE,   
    AUD_NVM_PAL_NICAM_PRE,
    AUD_NVM_PAL_NON_EU,      
    AUD_NVM_A2_PRE_DK,   
    AUD_NVM_PAL_PRE_DK,   
    AUD_NVM_PAL_AM_PRE_DK, 
    AUD_NVM_PAL_NICAM_PRE_DK,    
    // AVC
    AUD_NVM_AVC_ADJUST_RATE,
    AUD_NVM_AVC_UI_ADJUST_RATE,
    AUD_NVM_AVC_LEVEL,
    AUD_NVM_AVC_GAIN_UP,
    AUD_NVM_LIPSYNC_A,
    AUD_NVM_LIPSYNC_B,
    AUD_NVM_LIPSYNC_C,
    AUD_NVM_FLAT,
    AUD_NVM_REVERB,
    // AVL
    AUD_NVM_AVL_FC,
    AUD_NVM_AVL_BAND0_TARGET,
    AUD_NVM_AVL_BAND0_ATTACK,
    AUD_NVM_AVL_BAND0_RELEASE,
    AUD_NVM_AVL_BAND0_RATIO,
    AUD_NVM_AVL_BAND0_SILENCE,
    AUD_NVM_AVL_BAND0_EXPAND,
    AUD_NVM_AVL_BAND0_POST_GAIN,
    AUD_NVM_AVL_BAND0_HOLD,
    AUD_NVM_AVL_BAND1_TARGET,
    AUD_NVM_AVL_BAND1_ATTACK,
    AUD_NVM_AVL_BAND1_RELEASE,
    AUD_NVM_AVL_BAND1_RATIO,
    AUD_NVM_AVL_BAND1_SILENCE,
    AUD_NVM_AVL_BAND1_EXPAND,
    AUD_NVM_AVL_BAND1_POST_GAIN,
    AUD_NVM_AVL_BAND1_HOLD,
#ifndef CC_AUD_NVM_LR_USE_DIFFER_VOL_TBL
    // Vol
    AUD_NVM_VOL_MIN,
    AUD_NVM_VOL_MAX,
    AUD_NVM_VOL_STEP1,
    AUD_NVM_VOL_STEP2,
    AUD_NVM_VOL_STEP3,
    AUD_NVM_VOL_STEP4,
    AUD_NVM_VOL_STEP5,
#else
    // Vol (L channel)
    AUD_NVM_VOL_MIN,
    AUD_NVM_L_VOL_MAX,
    AUD_NVM_L_VOL_STEP1,
    AUD_NVM_L_VOL_STEP2,
    AUD_NVM_L_VOL_STEP3,
    AUD_NVM_L_VOL_STEP4,
    AUD_NVM_L_VOL_STEP5,
        // (R channel)
    AUD_NVM_R_VOL_MAX,
    AUD_NVM_R_VOL_STEP1,
    AUD_NVM_R_VOL_STEP2,
    AUD_NVM_R_VOL_STEP3,
    AUD_NVM_R_VOL_STEP4,
    AUD_NVM_R_VOL_STEP5,
        // (L channel for skype)
    AUD_NVM_L_VOL_MAX_SKYPE,
    AUD_NVM_L_VOL_STEP1_SKYPE,
    AUD_NVM_L_VOL_STEP2_SKYPE,
    AUD_NVM_L_VOL_STEP3_SKYPE,
    AUD_NVM_L_VOL_STEP4_SKYPE,
    AUD_NVM_L_VOL_STEP5_SKYPE,
        // (R channel for skype)
    AUD_NVM_R_VOL_MAX_SKYPE,
    AUD_NVM_R_VOL_STEP1_SKYPE,
    AUD_NVM_R_VOL_STEP2_SKYPE,
    AUD_NVM_R_VOL_STEP3_SKYPE,
    AUD_NVM_R_VOL_STEP4_SKYPE,
    AUD_NVM_R_VOL_STEP5_SKYPE,
#endif
    // HP Vol
    AUD_NVM_HP_VOL_MAX,
    AUD_NVM_HP_VOL_STEP1,
    AUD_NVM_HP_VOL_STEP2,
    AUD_NVM_HP_VOL_STEP3,
    AUD_NVM_HP_VOL_STEP4,
    AUD_NVM_HP_VOL_STEP5,
    // Input source gain
    AUD_NVM_PREHDMI,
    AUD_NVM_PREAV,
    AUD_NVM_PREUSB,
    AUD_NVM_PREATV,
    AUD_NVM_PREDTV,
    // Bass
    AUD_NVM_BASS_SUBWOOFER,
    AUD_NVM_BASS_SPKSIZE,
    // VS
    AUD_NVM_VS_CLARITY,
    AUD_NVM_VS_WIDTH,
    AUD_NVM_VS_LRGAIN,
    AUD_NVM_VS_XTALK_GIAN,
    AUD_NVM_VS_OPGAIN,
    AUD_NVM_VS_SWGAIN,
    // 3DVS
    AUD_NVM_3DVS_CLARITY,
    AUD_NVM_3DVS_WIDTH,
    AUD_NVM_3DVS_LRGAIN,
    AUD_NVM_3DVS_XTALK_GIAN,
    AUD_NVM_3DVS_OPGAIN,
    AUD_NVM_3DVS_SWGAIN,
    AUD_NVM_VS_FO,
    // BBE
    AUD_NVM_BBE_LEVEL,
    AUD_NVM_BBE_UPGAIN,
    AUD_NVM_BBE_LOGAIN,
    AUD_NVM_BBE_3DGAIN,
    AUD_NVM_PL2_ON,
    AUD_NVM_PL2_MODE,
    AUD_NVM_MTS_FMSATU_MUTE,
    AUD_NVM_MTS_FMSATU_MUTE_UPTHR,
    AUD_NVM_MTS_FMSATU_MUTE_LOTHR,
    AUD_NVM_PILOT_OFF_DETECT,
    AUD_NVM_AMP_MUTE,
    // Extra volume gain
    AUD_NVM_LR_EXTRA_LINEIN,
    AUD_NVM_LR_EXTRA_OTHERS,
    // Spdif
  //  AUD_NVM_SPDIF_NORMAL,
  //  AUD_NVM_SPDIF_LOW,
    AUD_NVM_SPDIF_MORE,
    AUD_NVM_SPDIF_MEDIUM,
    AUD_NVM_SPDIF_LESS,
    // Limiter
    AUD_NVM_LIMITER_MODE,
    AUD_NVM_LIMITER_FLAG,
    AUD_NVM_LIMITER_THRES,
    AUD_NVM_LIMITER_ATTACK_RATE,
    AUD_NVM_LIMITER_RELEASE_RATE,
    AUD_NVM_LIMITER_RATIO,
    AUD_NVM_CUTOFF_FREQ,
    // Notch
    AUD_NVM_NOTCH_FC,
    AUD_NVM_NOTCH_Q_ON,
    AUD_NVM_NOTCH_Q_OFF,
    // EQ Table & Normal & Clear sound on
    AUD_NVM_EQ_USER1_1_CS_ON,
    AUD_NVM_EQ_USER1_2_CS_ON,
    AUD_NVM_EQ_USER1_3_CS_ON,
    AUD_NVM_EQ_USER1_4_CS_ON,
    AUD_NVM_EQ_USER1_5_CS_ON,
    AUD_NVM_EQ_USER1_6_CS_ON,
    AUD_NVM_EQ_USER1_7_CS_ON,
    // EQ Wall & Normal & Clear sound on
    AUD_NVM_EQ_USER2_1_CS_ON,
    AUD_NVM_EQ_USER2_2_CS_ON,
    AUD_NVM_EQ_USER2_3_CS_ON,
    AUD_NVM_EQ_USER2_4_CS_ON,
    AUD_NVM_EQ_USER2_5_CS_ON,
    AUD_NVM_EQ_USER2_6_CS_ON,
    AUD_NVM_EQ_USER2_7_CS_ON,
    // EQ Table & Normal & Clear sound off
    AUD_NVM_EQ_USER1_1_CS_OFF,
    AUD_NVM_EQ_USER1_2_CS_OFF,
    AUD_NVM_EQ_USER1_3_CS_OFF,
    AUD_NVM_EQ_USER1_4_CS_OFF,
    AUD_NVM_EQ_USER1_5_CS_OFF,
    AUD_NVM_EQ_USER1_6_CS_OFF,
    AUD_NVM_EQ_USER1_7_CS_OFF,
    // EQ Wall & Normal & Clear sound off
    AUD_NVM_EQ_USER2_1_CS_OFF,
    AUD_NVM_EQ_USER2_2_CS_OFF,
    AUD_NVM_EQ_USER2_3_CS_OFF,
    AUD_NVM_EQ_USER2_4_CS_OFF,
    AUD_NVM_EQ_USER2_5_CS_OFF,
    AUD_NVM_EQ_USER2_6_CS_OFF,
    AUD_NVM_EQ_USER2_7_CS_OFF,
    // EQ Table & Skype
    AUD_NVM_EQ_USER3_1,
    AUD_NVM_EQ_USER3_2,
    AUD_NVM_EQ_USER3_3,
    AUD_NVM_EQ_USER3_4,
    AUD_NVM_EQ_USER3_5,
    AUD_NVM_EQ_USER3_6,
    AUD_NVM_EQ_USER3_7,
    // EQ Wall & Skype
    AUD_NVM_EQ_USER4_1,
    AUD_NVM_EQ_USER4_2,
    AUD_NVM_EQ_USER4_3,
    AUD_NVM_EQ_USER4_4,
    AUD_NVM_EQ_USER4_5,
    AUD_NVM_EQ_USER4_6,
    AUD_NVM_EQ_USER4_7,
    // VB
    AUD_NVM_VB_ONOFF,
    AUD_NVM_VB_FC,
    AUD_NVM_VB_GAIN1,
    AUD_NVM_VB_GAIN2,
    // PEQ 0/1 for Bass/Treble
    AUD_NVM_PEQ_ONOFF,
    AUD_NVM_PEQ_FC_USER1_0,
    AUD_NVM_PEQ_Q_USER1_0,
    AUD_NVM_PEQ_GAIN_USER1_0,
    AUD_NVM_PEQ_FC_USER1_1,
    AUD_NVM_PEQ_Q_USER1_1,
    AUD_NVM_PEQ_GAIN_USER1_1,
    // PEQ 2-8 Wall & Normal & Clear sound on
    AUD_NVM_PEQ_FC_USER1_2,
    AUD_NVM_PEQ_Q_USER1_2,
    AUD_NVM_PEQ_GAIN_USER1_2_CS_ON,
    AUD_NVM_PEQ_FC_USER1_3,
    AUD_NVM_PEQ_Q_USER1_3,
    AUD_NVM_PEQ_GAIN_USER1_3_CS_ON,
    AUD_NVM_PEQ_FC_USER1_4,
    AUD_NVM_PEQ_Q_USER1_4,
    AUD_NVM_PEQ_GAIN_USER1_4_CS_ON,
    AUD_NVM_PEQ_FC_USER1_5,
    AUD_NVM_PEQ_Q_USER1_5,
    AUD_NVM_PEQ_GAIN_USER1_5_CS_ON,
    AUD_NVM_PEQ_FC_USER1_6,
    AUD_NVM_PEQ_Q_USER1_6,
    AUD_NVM_PEQ_GAIN_USER1_6_CS_ON,
    AUD_NVM_PEQ_FC_USER1_7,
    AUD_NVM_PEQ_Q_USER1_7,
    AUD_NVM_PEQ_GAIN_USER1_7_CS_ON,
    AUD_NVM_PEQ_FC_USER1_8,
    AUD_NVM_PEQ_Q_USER1_8,
    AUD_NVM_PEQ_GAIN_USER1_8_CS_ON,
    // PEQ Table & Normal & Clear sound on
    AUD_NVM_PEQ_FC_USER2_0,
    AUD_NVM_PEQ_Q_USER2_0,
    AUD_NVM_PEQ_GAIN_USER2_0,
    AUD_NVM_PEQ_FC_USER2_1,
    AUD_NVM_PEQ_Q_USER2_1,
    AUD_NVM_PEQ_GAIN_USER2_1,
    AUD_NVM_PEQ_FC_USER2_2,
    AUD_NVM_PEQ_Q_USER2_2,
    AUD_NVM_PEQ_GAIN_USER2_2_CS_ON,
    AUD_NVM_PEQ_FC_USER2_3,
    AUD_NVM_PEQ_Q_USER2_3,
    AUD_NVM_PEQ_GAIN_USER2_3_CS_ON,
    AUD_NVM_PEQ_FC_USER2_4,
    AUD_NVM_PEQ_Q_USER2_4,
    AUD_NVM_PEQ_GAIN_USER2_4_CS_ON,
    AUD_NVM_PEQ_FC_USER2_5,
    AUD_NVM_PEQ_Q_USER2_5,
    AUD_NVM_PEQ_GAIN_USER2_5_CS_ON,
    AUD_NVM_PEQ_FC_USER2_6,
    AUD_NVM_PEQ_Q_USER2_6,
    AUD_NVM_PEQ_GAIN_USER2_6_CS_ON,
    AUD_NVM_PEQ_FC_USER2_7,
    AUD_NVM_PEQ_Q_USER2_7,
    AUD_NVM_PEQ_GAIN_USER2_7_CS_ON,
    AUD_NVM_PEQ_FC_USER2_8,
    AUD_NVM_PEQ_Q_USER2_8,
    AUD_NVM_PEQ_GAIN_USER2_8_CS_ON,
    // PEQ 2-8 Wall & Normal & Clear sound off
    AUD_NVM_PEQ_GAIN_USER1_2_CS_OFF,
    AUD_NVM_PEQ_GAIN_USER1_3_CS_OFF,
    AUD_NVM_PEQ_GAIN_USER1_4_CS_OFF,
    AUD_NVM_PEQ_GAIN_USER1_5_CS_OFF,
    AUD_NVM_PEQ_GAIN_USER1_6_CS_OFF,
    AUD_NVM_PEQ_GAIN_USER1_7_CS_OFF,
    AUD_NVM_PEQ_GAIN_USER1_8_CS_OFF,
    // PEQ Table & Normal & Clear sound off
    AUD_NVM_PEQ_GAIN_USER2_2_CS_OFF,
    AUD_NVM_PEQ_GAIN_USER2_3_CS_OFF,
    AUD_NVM_PEQ_GAIN_USER2_4_CS_OFF,
    AUD_NVM_PEQ_GAIN_USER2_5_CS_OFF,
    AUD_NVM_PEQ_GAIN_USER2_6_CS_OFF,
    AUD_NVM_PEQ_GAIN_USER2_7_CS_OFF,
    AUD_NVM_PEQ_GAIN_USER2_8_CS_OFF,
    // PEQ Wall & Skype
    AUD_NVM_PEQ_FC_USER3_2,
    AUD_NVM_PEQ_Q_USER3_2,
    AUD_NVM_PEQ_GAIN_USER3_2,
    AUD_NVM_PEQ_FC_USER3_3,
    AUD_NVM_PEQ_Q_USER3_3,
    AUD_NVM_PEQ_GAIN_USER3_3,
    AUD_NVM_PEQ_FC_USER3_4,
    AUD_NVM_PEQ_Q_USER3_4,
    AUD_NVM_PEQ_GAIN_USER3_4,
    AUD_NVM_PEQ_FC_USER3_5,
    AUD_NVM_PEQ_Q_USER3_5,
    AUD_NVM_PEQ_GAIN_USER3_5,
    AUD_NVM_PEQ_FC_USER3_6,
    AUD_NVM_PEQ_Q_USER3_6,
    AUD_NVM_PEQ_GAIN_USER3_6,
    AUD_NVM_PEQ_FC_USER3_7,
    AUD_NVM_PEQ_Q_USER3_7,
    AUD_NVM_PEQ_GAIN_USER3_7,
    AUD_NVM_PEQ_FC_USER3_8,
    AUD_NVM_PEQ_Q_USER3_8,
    AUD_NVM_PEQ_GAIN_USER3_8,
    // PEQ Table & Skype
    AUD_NVM_PEQ_FC_USER4_0,
    AUD_NVM_PEQ_Q_USER4_0,
    AUD_NVM_PEQ_GAIN_USER4_0,
    AUD_NVM_PEQ_FC_USER4_1,
    AUD_NVM_PEQ_Q_USER4_1,
    AUD_NVM_PEQ_GAIN_USER4_1,
    AUD_NVM_PEQ_FC_USER4_2,
    AUD_NVM_PEQ_Q_USER4_2,
    AUD_NVM_PEQ_GAIN_USER4_2,
    AUD_NVM_PEQ_FC_USER4_3,
    AUD_NVM_PEQ_Q_USER4_3,
    AUD_NVM_PEQ_GAIN_USER4_3,
    AUD_NVM_PEQ_FC_USER4_4,
    AUD_NVM_PEQ_Q_USER4_4,
    AUD_NVM_PEQ_GAIN_USER4_4,
    AUD_NVM_PEQ_FC_USER4_5,
    AUD_NVM_PEQ_Q_USER4_5,
    AUD_NVM_PEQ_GAIN_USER4_5,
    AUD_NVM_PEQ_FC_USER4_6,
    AUD_NVM_PEQ_Q_USER4_6,
    AUD_NVM_PEQ_GAIN_USER4_6,
    AUD_NVM_PEQ_FC_USER4_7,
    AUD_NVM_PEQ_Q_USER4_7,
    AUD_NVM_PEQ_GAIN_USER4_7,
    AUD_NVM_PEQ_FC_USER4_8,
    AUD_NVM_PEQ_Q_USER4_8,
    AUD_NVM_PEQ_GAIN_USER4_8,
        // Volume dependent PEQ
    AUD_NVM_PEQ_GAIN_VOL1_7,
    AUD_NVM_PEQ_GAIN_VOL2_7,
    AUD_NVM_PEQ_GAIN_VOL3_7,
    AUD_NVM_PEQ_GAIN_VOL4_7,
    AUD_NVM_PEQ_GAIN_VOL1_8,
    AUD_NVM_PEQ_GAIN_VOL2_8,
    AUD_NVM_PEQ_GAIN_VOL3_8,
    AUD_NVM_PEQ_GAIN_VOL4_8,
    // Beep tone
    AUD_NVM_BEEP_FC,
    AUD_NVM_BEEP_VOLUME,
    AUD_NVM_BEEP_DURATION,
    // Bass management
    AUD_NVM_BASS_MANA_ONOFF,
    // Dual mono
    AUD_NVM_DUAL_MONO,
    #if 1
    // Class D (TAS5711)
    AUD_NVM_D_SYS_CTRL_REG2,
//  AUD_NVM_D_MASTER_VOL,
//  AUD_NVM_D_CH1_VOL,
//  AUD_NVM_D_CH2_VOL,
//  AUD_NVM_D_CH3_VOL,
    AUD_NVM_D_VOL_CONF_REG,
    AUD_NVM_D_ADBD,
    AUD_NVM_D_INPUT_MUX,
    AUD_NVM_D_CH4_SRC_SEL,
    AUD_NVM_D_PWM_MUX,
    
    AUD_NVM_D_CH1_BQ0_1,
    AUD_NVM_D_CH1_BQ0_2,
    AUD_NVM_D_CH1_BQ0_3,
    AUD_NVM_D_CH1_BQ0_4,
    AUD_NVM_D_CH1_BQ0_5,
    AUD_NVM_D_CH1_BQ1_1,
    AUD_NVM_D_CH1_BQ1_2,
    AUD_NVM_D_CH1_BQ1_3,
    AUD_NVM_D_CH1_BQ1_4,
    AUD_NVM_D_CH1_BQ1_5,
    AUD_NVM_D_CH1_BQ2_1,
    AUD_NVM_D_CH1_BQ2_2,
    AUD_NVM_D_CH1_BQ2_3,
    AUD_NVM_D_CH1_BQ2_4,
    AUD_NVM_D_CH1_BQ2_5,
    AUD_NVM_D_CH1_BQ3_1,
    AUD_NVM_D_CH1_BQ3_2,
    AUD_NVM_D_CH1_BQ3_3,
    AUD_NVM_D_CH1_BQ3_4,
    AUD_NVM_D_CH1_BQ3_5,
    AUD_NVM_D_CH1_BQ4_1,
    AUD_NVM_D_CH1_BQ4_2,
    AUD_NVM_D_CH1_BQ4_3,
    AUD_NVM_D_CH1_BQ4_4,
    AUD_NVM_D_CH1_BQ4_5,
    AUD_NVM_D_CH1_BQ5_1,
    AUD_NVM_D_CH1_BQ5_2,
    AUD_NVM_D_CH1_BQ5_3,
    AUD_NVM_D_CH1_BQ5_4,
    AUD_NVM_D_CH1_BQ5_5,
    AUD_NVM_D_CH1_BQ6_1,
    AUD_NVM_D_CH1_BQ6_2,
    AUD_NVM_D_CH1_BQ6_3,
    AUD_NVM_D_CH1_BQ6_4,
    AUD_NVM_D_CH1_BQ6_5,
    AUD_NVM_D_CH2_BQ0_1,
    AUD_NVM_D_CH2_BQ0_2,
    AUD_NVM_D_CH2_BQ0_3,
    AUD_NVM_D_CH2_BQ0_4,
    AUD_NVM_D_CH2_BQ0_5,
    AUD_NVM_D_CH2_BQ1_1,
    AUD_NVM_D_CH2_BQ1_2,
    AUD_NVM_D_CH2_BQ1_3,
    AUD_NVM_D_CH2_BQ1_4,
    AUD_NVM_D_CH2_BQ1_5,
    AUD_NVM_D_CH2_BQ2_1,
    AUD_NVM_D_CH2_BQ2_2,
    AUD_NVM_D_CH2_BQ2_3,
    AUD_NVM_D_CH2_BQ2_4,
    AUD_NVM_D_CH2_BQ2_5,
    AUD_NVM_D_CH2_BQ3_1,
    AUD_NVM_D_CH2_BQ3_2,
    AUD_NVM_D_CH2_BQ3_3,
    AUD_NVM_D_CH2_BQ3_4,
    AUD_NVM_D_CH2_BQ3_5,
    AUD_NVM_D_CH2_BQ4_1,
    AUD_NVM_D_CH2_BQ4_2,
    AUD_NVM_D_CH2_BQ4_3,
    AUD_NVM_D_CH2_BQ4_4,
    AUD_NVM_D_CH2_BQ4_5,
    AUD_NVM_D_CH2_BQ5_1,
    AUD_NVM_D_CH2_BQ5_2,
    AUD_NVM_D_CH2_BQ5_3,
    AUD_NVM_D_CH2_BQ5_4,
    AUD_NVM_D_CH2_BQ5_5,
    AUD_NVM_D_CH2_BQ6_1,
    AUD_NVM_D_CH2_BQ6_2,
    AUD_NVM_D_CH2_BQ6_3,
    AUD_NVM_D_CH2_BQ6_4,
    AUD_NVM_D_CH2_BQ6_5,
    
    AUD_NVM_D_DRC1_AE_1,
    AUD_NVM_D_DRC1_AE_2,
    AUD_NVM_D_DRC1_AA_1,
    AUD_NVM_D_DRC1_AA_2,
    AUD_NVM_D_DRC1_AD_1,
    AUD_NVM_D_DRC1_AD_2,
    AUD_NVM_D_DRC2_AE_1,
    AUD_NVM_D_DRC2_AE_2,
    AUD_NVM_D_DRC2_AA_1,
    AUD_NVM_D_DRC2_AA_2,
    AUD_NVM_D_DRC2_AD_1,
    AUD_NVM_D_DRC2_AD_2,
    
    AUD_NVM_D_DRC1_T,
    AUD_NVM_D_DRC1_K,
    AUD_NVM_D_DRC1_O,
    AUD_NVM_D_DRC2_T,
    AUD_NVM_D_DRC2_K,
    AUD_NVM_D_DRC2_O,
    
    AUD_NVM_D_DRC_CTRL,
    AUD_NVM_D_BANK_SWITCH,
    
    AUD_NVM_D_CH1_OUT_MIX_1,
    AUD_NVM_D_CH1_OUT_MIX_2,
    AUD_NVM_D_CH1_OUT_MIX_3,
    AUD_NVM_D_CH2_OUT_MIX_1,
    AUD_NVM_D_CH2_OUT_MIX_2,
    AUD_NVM_D_CH2_OUT_MIX_3,
    
    AUD_NVM_D_CH1_IN_MIX_1,
    AUD_NVM_D_CH1_IN_MIX_2,
    AUD_NVM_D_CH1_IN_MIX_3,
    AUD_NVM_D_CH1_IN_MIX_4,
    AUD_NVM_D_CH2_IN_MIX_1,
    AUD_NVM_D_CH2_IN_MIX_2,
    AUD_NVM_D_CH2_IN_MIX_3,
    AUD_NVM_D_CH2_IN_MIX_4,
    AUD_NVM_D_CH3_IN_MIX_1,
    AUD_NVM_D_CH3_IN_MIX_2,
    AUD_NVM_D_CH3_IN_MIX_3,
    
    AUD_NVM_D_OUT_POST_SCALE_1,
    AUD_NVM_D_OUT_POST_SCALE_2,
    
    AUD_NVM_D_CH1_BQ7_1,
    AUD_NVM_D_CH1_BQ7_2,
    AUD_NVM_D_CH1_BQ7_3,
    AUD_NVM_D_CH1_BQ7_4,
    AUD_NVM_D_CH1_BQ7_5,
    AUD_NVM_D_CH1_BQ8_1,
    AUD_NVM_D_CH1_BQ8_2,
    AUD_NVM_D_CH1_BQ8_3,
    AUD_NVM_D_CH1_BQ8_4,
    AUD_NVM_D_CH1_BQ8_5,
    AUD_NVM_D_SUB_BQ0_1,
    AUD_NVM_D_SUB_BQ0_2,
    AUD_NVM_D_SUB_BQ0_3,
    AUD_NVM_D_SUB_BQ0_4,
    AUD_NVM_D_SUB_BQ0_5,
    AUD_NVM_D_SUB_BQ1_1,
    AUD_NVM_D_SUB_BQ1_2,
    AUD_NVM_D_SUB_BQ1_3,
    AUD_NVM_D_SUB_BQ1_4,
    AUD_NVM_D_SUB_BQ1_5,
    AUD_NVM_D_CH2_BQ7_1,
    AUD_NVM_D_CH2_BQ7_2,
    AUD_NVM_D_CH2_BQ7_3,
    AUD_NVM_D_CH2_BQ7_4,
    AUD_NVM_D_CH2_BQ7_5,
    AUD_NVM_D_CH2_BQ8_1,
    AUD_NVM_D_CH2_BQ8_2,
    AUD_NVM_D_CH2_BQ8_3,
    AUD_NVM_D_CH2_BQ8_4,
    AUD_NVM_D_CH2_BQ8_5,
    
    AUD_NVM_D_PSEUDO_CH2_BQ_1,
    AUD_NVM_D_PSEUDO_CH2_BQ_2,
    AUD_NVM_D_PSEUDO_CH2_BQ_3,
    AUD_NVM_D_PSEUDO_CH2_BQ_4,
    AUD_NVM_D_PSEUDO_CH2_BQ_5,
    
    AUD_NVM_D_CH4_OUT_MIX_1,
    AUD_NVM_D_CH4_OUT_MIX_2,
    AUD_NVM_D_CH4_IN_MIX_1,
    AUD_NVM_D_CH4_IN_MIX_2,
    
    AUD_NVM_D_IDF_POST_SCALE,
    AUD_NVM_D_2ND_LEFT_CH_MIX,
    AUD_NVM_D_2ND_RIGHT_CH_MIX,

    AUD_NVM_D_MOD_LIMIT_REG,
    #endif
    //Class D (STA381BW)
    AUD_NVM_AMP_START,
    AUD_NVM_AMP_END,
    AUD_NVM_D_CONF_REG_A,
    AUD_NVM_D_CONF_REG_B,
    AUD_NVM_D_CONF_REG_C,
    AUD_NVM_D_CONF_REG_D,
    AUD_NVM_D_CONF_REG_E,
    AUD_NVM_D_CONF_REG_F,

    AUD_NVM_D_MUTE_LINE_OUT_CONF_REG,
    AUD_NVM_D_MASTER_VOL,
    AUD_NVM_D_CH1_VOL,
    AUD_NVM_D_CH2_VOL,
    AUD_NVM_D_CH3_VOL,
    AUD_NVM_D_CH1_CONF_REG,
    AUD_NVM_D_CH2_CONF_REG,
    AUD_NVM_D_CH3_CONF_REG,

    AUD_NVM_D_TONE_CONL_REG,

    AUD_NVM_D_DRC_LIMITER_1_RATE,
    AUD_NVM_D_DRC_LIMITER_1_THRES,
    AUD_NVM_D_DRC_LIMITER_2_RATE,
    AUD_NVM_D_DRC_LIMITER_2_THRES,

    AUD_NVM_D_DRC_LIMITER_1_EXT_ATTACK_THRES,
    AUD_NVM_D_DRC_LIMITER_1_EXT_RELEASE_THRES,
    AUD_NVM_D_DRC_LIMITER_2_EXT_ATTACK_THRES,
    AUD_NVM_D_DRC_LIMITER_2_EXT_RELEASE_THRES,
    
    AUD_NVM_D_EXT_CONF_REG,
    AUD_NVM_D_EXT_VOL_CONF_REG_A,
    AUD_NVM_D_EXT_VOL_CONF_REG_B,

    AUD_NVM_D_ZERO_DETECT_CONF_REG,
    AUD_NVM_D_HP_LINE_OUT_CONF_REG,

    AUD_NVM_D_EXT_COEFF_RANGE_UP_A, /*Rico 20120907*/
    AUD_NVM_D_EXT_COEFF_RANGE_UP_B,
    
    AUD_NVM_D_CH1_BQ1_H10,
    AUD_NVM_D_CH1_BQ1_H11,
    AUD_NVM_D_CH1_BQ1_H12,
    AUD_NVM_D_CH1_BQ1_H13,
    AUD_NVM_D_CH1_BQ1_H14,

    AUD_NVM_D_CH1_BQ2_H20,
    AUD_NVM_D_CH1_BQ2_H21,
    AUD_NVM_D_CH1_BQ2_H22,
    AUD_NVM_D_CH1_BQ2_H23,
    AUD_NVM_D_CH1_BQ2_H24,

    AUD_NVM_D_CH1_BQ3_H30,
    AUD_NVM_D_CH1_BQ3_H31,
    AUD_NVM_D_CH1_BQ3_H32,
    AUD_NVM_D_CH1_BQ3_H33,
    AUD_NVM_D_CH1_BQ3_H34,

    AUD_NVM_D_CH1_BQ4_H40,
    AUD_NVM_D_CH1_BQ4_H41,
    AUD_NVM_D_CH1_BQ4_H42,
    AUD_NVM_D_CH1_BQ4_H43,
    AUD_NVM_D_CH1_BQ4_H44,

    AUD_NVM_D_CH2_BQ1_H10,
    AUD_NVM_D_CH2_BQ1_H11,
    AUD_NVM_D_CH2_BQ1_H12,
    AUD_NVM_D_CH2_BQ1_H13,
    AUD_NVM_D_CH2_BQ1_H14,

    AUD_NVM_D_CH2_BQ2_H20,
    AUD_NVM_D_CH2_BQ2_H21,
    AUD_NVM_D_CH2_BQ2_H22,
    AUD_NVM_D_CH2_BQ2_H23,
    AUD_NVM_D_CH2_BQ2_H24,

    AUD_NVM_D_CH2_BQ3_H30,
    AUD_NVM_D_CH2_BQ3_H31,
    AUD_NVM_D_CH2_BQ3_H32,
    AUD_NVM_D_CH2_BQ3_H33,
    AUD_NVM_D_CH2_BQ3_H34,

    AUD_NVM_D_CH2_BQ4_H40,
    AUD_NVM_D_CH2_BQ4_H41,
    AUD_NVM_D_CH2_BQ4_H42,
    AUD_NVM_D_CH2_BQ4_H43,
    AUD_NVM_D_CH2_BQ4_H44,

    AUD_NVM_D_CH12_BQ5_H0,
    AUD_NVM_D_CH12_BQ5_H1,
    AUD_NVM_D_CH12_BQ5_H2,
    AUD_NVM_D_CH12_BQ5_H3,
    AUD_NVM_D_CH12_BQ5_H4,

    AUD_NVM_D_CH3_BQ1_H0,
    AUD_NVM_D_CH3_BQ1_H1,
    AUD_NVM_D_CH3_BQ1_H2,
    AUD_NVM_D_CH3_BQ1_H3,
    AUD_NVM_D_CH3_BQ1_H4,

    AUD_NVM_D_CH1_PRE_SCALE,
    AUD_NVM_D_CH2_PRE_SCALE,
    AUD_NVM_D_CH1_POST_SCALE,
    AUD_NVM_D_CH2_POST_SCALE,
    AUD_NVM_D_CH3_POST_SCALE,

    AUD_NVM_D_RAM_RESERVED,

    AUD_NVM_D_CH1_MIX_1,
    AUD_NVM_D_CH1_MIX_2,
    AUD_NVM_D_CH2_MIX_1,
    AUD_NVM_D_CH2_MIX_2,
    AUD_NVM_D_CH3_MIX_1,
    AUD_NVM_D_CH3_MIX_2,
    
} AUD_NVM_TYPE_T;

typedef struct
{
    UINT16              u2Offset;
    UINT8               u1Length;
    UINT32              u4DefaultValue;
    AUD_NVM_TYPE_T      eDataType;
} AUD_NVM_DATA_T;

typedef struct
{
    INT16 i2NvmInputSrcHDMI;
    INT16 i2NvmInputSrcAV;
    INT16 i2NvmInputSrcUSB;
    INT16 i2NvmInputSrcATV;
    INT16 i2NvmInputSrcDTV;
} AUD_IPT_SRC_GAIN_T;

typedef enum
{
    AUD_NVM_DK = 0,
    AUD_NVM_NON_DK,
    AUD_NVM_TOTAL
} AUD_NVM_ATV_FMT_T;

typedef enum
{
    AUD_NVM_DEFAULT_M = 0,
    AUD_NVM_DEFAULT_BG = 1,
    AUD_NVM_DEFAULT_DK = 2,
    AUD_NVM_DEFAULT_I = 3,    
    AUD_NVM_DEFAULT_TOTAL
} AUD_NVM_DEFAULT_ATV_SYS_T;

typedef enum
{
    AUD_NVM_QRY_A2_GAIN = 0,
    AUD_NVM_QRY_PAL_GAIN,
    AUD_NVM_QRY_AM_GAIN,
    AUD_NVM_QRY_NICAM_GAIN
} AUD_NVM_ATV_QRY_T;

typedef enum
{
    AUD_NVM_L_VOL_TBL = 0,
    AUD_NVM_R_VOL_TBL,
    AUD_NVM_L_VOL_TBL_SKYPE,
    AUD_NVM_R_VOL_TBL_SKYPE,
    AUD_NVM_COMMON_VOL_TBL,
    AUD_NVM_VOL_TBL_NUM
} AUD_NVM_VOL_TBL_T;

typedef struct _AUD_NVM_ATV_PREGAIN
{
    UINT32 u4A2_FineGain[AUD_NVM_TOTAL];
    UINT8  u1A2_FineGain[AUD_NVM_TOTAL];
    UINT32 u4PAL_FineGain[AUD_NVM_TOTAL];
    UINT8  u1PAL_FineGain[AUD_NVM_TOTAL];
    UINT32 u4AM_FineGain[AUD_NVM_TOTAL];
    UINT8  u1AM_FineGain[AUD_NVM_TOTAL];
    UINT32 u4NICAM_FineGain[AUD_NVM_TOTAL];
    UINT8  u1NICAM_FineGain[AUD_NVM_TOTAL];
}AUD_NVM_ATV_PREGAIN;
#endif

typedef enum
{
    AUD_DEC_OVM_OFF = 0,
    AUD_DEC_OVM_LOW,
    AUD_DEC_OVM_HIGH
} AUD_DEC_OVM_MODE_T;

typedef enum
{
    AUD_DEC_OUTPORT_MODE_UNKNOW = 0,
    AUD_DEC_OUTPORT_MODE_HEADPHONE,
    AUD_DEC_OUTPORT_MODE_LINEOUT,
    AUD_DEC_OUTPORT_MODE_SUB_WOOFER
}   AUD_DEC_OUTPORT_MODE_T;

/* new OVM mode */
#define NEW_PAL_SHIFT (0)
#define NEW_A2_SHIFT (2)
#define NEW_MTS_SHIFT (4)
#define NEW_FMFM_SHIFT (6)
#define NEW_PAL_MASK (0x3 << NEW_PAL_SHIFT)
#define NEW_A2_MASK (0x3<< NEW_A2_SHIFT)
#define NEW_MTS_MASK (0x3 << NEW_MTS_SHIFT)
#define NEW_FMFM_MASK (0x3 << NEW_FMFM_SHIFT)

//==========  Flash AQ Related Definitions ===================================
#define AUD_FLASH_AQ_SIZE (512*1024) //Total Flash AQ size on NAND Flash partition.
#define AUD_FLASH_AQ_BUF_SIZE (10*1024)   // Flash AQ buffer size for temp usage.
#define FLASH_AQ_VER_LENGTH 8
#define FLASH_AQ_VER_OFFSET 0
#define FLASH_AQ_SCENE_NUM   5
#ifdef CC_SONY_2K14_FLASH_AQ
#define FLASH_AQ_COMM_TABLE_ITEMS 5   // The number of tables in Common Sheet.(including version)
#else
#define FLASH_AQ_COMM_TABLE_ITEMS 4   // The number of tables in Common Sheet.(including version)
#endif

#if 1 //def CC_AUD_W_SUPPORT
#ifdef CC_AUD_LOUDNESS_FROM_SOURCE_SOUNDMODE
#define FLASH_AQ_ID_NUM 25            //add loudness for source & soundmode, AQ ID reduce            
#else
#define FLASH_AQ_ID_NUM 28            // The number of AQ IDs. e.x. AQ1~11 mean 11 AQ IDs.
#endif
#else
#define FLASH_AQ_ID_NUM 15            // The number of AQ IDs. e.x. AQ1~11 mean 11 AQ IDs.
#endif
#define FLASH_AQ_TYPE3_TABLE_SIZE 186 // The size in bytes of TYPE3 only tables.


#ifdef CC_SONY_2K14_FLASH_AQ
#define AQ_LEVEL_ITEMS 3
#define AQ_BASS_ITEMS 2
#define AQ_BASS_MANAGE_ITEMS 8
#define AQ_EQ_ITEMS 7
#define AQ_DRC_ITEMS 8
#define AQ_AVC_ITEMS 5
#define AQ_SPH_ITEMS 1
#define AQ_MVS_ITEMS 7
#define AQ_POSTSCL_ITEMS 2
#define AQ_PEQ_ITEMS 45
#define AQ_EXT_PEQ_ITEMS 35
#define AQ_STC_FILTER_ITEMS 20
#define AQ_EXT_PEQ_CSW_ITEMS 40
#define AQ_BACKLIGHT_ITEMS 12
#define AQ_VBASS_ITEMS 7 
#define AQ_FIR_ITEMS 256
#define AQ_LOUDNESS_ITEMS 41    ///Change
#define AQ_MTK_DRC_ITEMS 33 ///Change
#define AQ_STC381Coeff_ITEMS 75
#else
#define AQ_LEVEL_ITEMS 3
#define AQ_BASS_ITEMS 2
#define AQ_BASS_MANAGE_ITEMS 8
#define AQ_EQ_ITEMS 7
#define AQ_DRC_ITEMS 8
#define AQ_AVC_ITEMS 5
#define AQ_SPH_ITEMS 1
#define AQ_MVS_ITEMS 7
#define AQ_POSTSCL_ITEMS 2
#define AQ_PEQ_ITEMS 45
#define AQ_EXT_PEQ_ITEMS 35
#define AQ_STC_FILTER_ITEMS 20
#define AQ_EXT_PEQ_CSW_ITEMS 40
#define AQ_BACKLIGHT_ITEMS 12
#define AQ_VBASS_ITEMS 7 
#define AQ_FIR_ITEMS 256
#define AQ_LOUDNESS_ITEMS 25    ///Change
#define AQ_MTK_DRC_ITEMS 33 ///Change
#define AQ_STC381Coeff_ITEMS 75

#endif  ///CC_SONY_2K13_FLASH_AQ

#define FLASH_AQ_COMM_SRC_NUM 5
#if 1//def CC_SONY_2K14_FLASH_AQ   //add  AUX L/R
#define FLASH_AQ_COMM_CH_NUM  12
#else
#define FLASH_AQ_COMM_CH_NUM  10
#endif
#ifdef CC_SONY_2K14_FLASH_AQ
#define FLASH_AQ_SRC_NUM  3
#else
#define FLASH_AQ_SRC_NUM  4
#endif
#define FLASH_AQ_SM_NUM   4
#define AQ_LIMITER_ITEMS 7
#define AQ_BM_CUTOFF_NUM 17
#define AQ_SW_CUTOFF_NUM 7

#define AQ_MODEL_FUNC_WOOFER   (1<<0)
#define AQ_MODEL_FUNC_DVD      (1<<1)

#ifdef CC_SONY_2K14_FLASH_AQ
typedef enum
{
    AQ_TYPE_VERSION =0,
    AQ_TYPE_CH_VOLUME,
    AQ_TYPE_SRC_VOLUME,
    AQ_TYPE_VOL_TABLE,
    AQ_TYPE_LO_VOL_TABLE,
    AQ_TYPE_PEQ_TABLE,
    AQ_TYPE_PEQ_WALL,
    AQ_TYPE_LEVEL,
    AQ_TYPE_BASS_TREBLE_TABLE,
    AQ_TYPE_BASS_TREBLE_WALL,
    AQ_TYPE_BASS_TREBLE_CUTOFF,
    AQ_TYPE_BASS_MANAGEMENT_TABLE,
    AQ_TYPE_BASS_MANAGEMENT_WALL,
    AQ_TYPE_EQ,
    AQ_TYPE_DRC,
    AQ_TYPE_AVC,
    AQ_TYPE_LIMITER,
    AQ_TYPE_SPKHEIGHT_TABLE,
    AQ_TYPE_SPKHEIGHT_WALL,
    AQ_TYPE_MVS_TABLE,
    AQ_TYPE_MVS_WALL,
    AQ_TYPE_POST,
    AQ_TYPE_SP_VOL,
    AQ_TYPE_LO_VOL,
    AQ_TYPE_EXT_PEQ,
    AQ_TYPE_STC381FILTER,
    AQ_TYPE_EXT_PEQ_LSRS,
    AQ_TYPE_EXT_PEQ_CSW,
    AQ_TYPE_TYPE3_TBL,
    AQ_TYPE_BACKLIGHT,    
    AQ_TYPE_VBASS_TABLE,
    AQ_TYPE_VBASS_WALL,
    AQ_TYPE_FIR_TABLE,
    AQ_TYPE_FIR_WALL,
    AQ_TYPE_HP_VOL_TABLE,
    AQ_TYPE_LOUDNESS_TABLE,
    AQ_TYPE_LOUDNESS_WALL,
    AQ_TYPE_MTKDRC_TABLE,
    AQ_TYPE_MTKDRC_WALL,
    AQ_TYPE_STC381COEFF,
    AQ_TYPE_MIC_GAIN,
    AQ_TYPE_RESERVED, 
    AQ_TYPE_NUM
}AUD_AQ_TYPE_T;
#else
typedef enum
{
    AQ_TYPE_VERSION =0,
    AQ_TYPE_CH_VOLUME,
    AQ_TYPE_SRC_VOLUME,
    AQ_TYPE_VOL_TABLE,
    AQ_TYPE_LO_VOL_TABLE,
    AQ_TYPE_PEQ,
    AQ_TYPE_LEVEL,
    AQ_TYPE_BASS_TREBLE,
    AQ_TYPE_BASS_MANAGEMENT,
    AQ_TYPE_EQ,
    AQ_TYPE_DRC,
    AQ_TYPE_AVC,
    AQ_TYPE_LIMITER,
    AQ_TYPE_SPKHEIGHT,
    AQ_TYPE_MVS,
    AQ_TYPE_POST,
    AQ_TYPE_SP_VOL,
    AQ_TYPE_LO_VOL,
    AQ_TYPE_EXT_PEQ,
    AQ_TYPE_STC381FILTER,
    AQ_TYPE_EXT_PEQ_LSRS,
    AQ_TYPE_EXT_PEQ_CSW,
    AQ_TYPE_TYPE3_TBL,
    AQ_TYPE_BACKLIGHT,    
    AQ_TYPE_VBASS,
    AQ_TYPE_FIR,
    AQ_TYPE_HP_VOL_TABLE,
    AQ_TYPE_LOUDNESS,
    AQ_TYPE_MTKDRC,
    AQ_TYPE_STC381COEFF,
    AQ_TYPE_MIC_GAIN,
    AQ_TYPE_RESERVED, 
    AQ_TYPE_NUM
}AUD_AQ_TYPE_T;
#endif


typedef enum
{
    AUD_OPL_BBE = 0,
    AUD_OPL_VIRTUAL_BASS,
    AUD_OPL_PEQ,
    AUD_OPL_M2S,
    AUD_OPL_CLEAR_VOICE,
    AUD_OPL_SPK_HEIGHT,
    AUD_OPL_CLIPPER,
    AUD_OPL_NOISE_REDUCTION,
    AUD_OPTL_KEY_SHIFT,
    AUD_OPL_VIQ,
    AUD_OPL_LXP,
    AUD_OPL_AUDYSSEY_ADV,
    AUD_OPL_AUDYSSEY_ABX,
    AUD_OPL_AUDYSSEY_AEQ,
    AUD_OPL_CDNOTCH,
    AUD_OPL_SRS_PEQ,
    AUD_OPL_SRS_LMT,
    AUD_OPL_SRS_VIQ,
    AUD_OPL_FIR,                //CC_AUD_FIR_SUPPORT
    AUD_OPL_SRS_AEQ,//19
    AUD_OPL_SPK_HEIGHT_V2,
    AUD_OPL_END,                // the end of all elements
    AUD_OPL_NUM
} AUD_OPL_FUNC_TYPE_T;

#ifdef CC_AUD_USE_FLASH_AQ
typedef struct
{
    AUD_AQ_TYPE_T       eDataType;
    CHAR                pChar[16];
    UINT32              u4Size;
} AUD_AQ_DSCRPT_T;

#else
typedef struct
{
    AUD_AQ_TYPE_T       eDataType;
    CHAR                pChar[8];
    UINT8               u1X;
    UINT8               u1Y;
    UINT8               u1Bytes;
} AUD_AQ_DSCRPT_T;
#endif

typedef struct
{
    UINT8               u1Cutoff;
    UINT8               u1LfeGain;
    UINT8               u1AttackHi;
    UINT8               u1ReleaseHi;
    UINT8               u1RateHi;
    UINT8               u1AttackLo;
    UINT8               u1ReleaseLo;
    UINT8               u1RateLo;
} AUD_AQ_AMP_DRC_T;

typedef struct
{
    UINT32               u4DRC1FiltAlpha;
    UINT32               u4DRC1FiltOmega;
    UINT32               u4DRC1AttackRate;
    UINT32               u4DRC1ReleaseRate;
    UINT32               u4DRC1AttackTh;
    UINT32               u4DRC2FiltAlpha;
    UINT32               u4DRC2FiltOmega;
    UINT32               u4DRC2AttackRate;
    UINT32               u4DRC2ReleaseRate;
    UINT32               u4DRC2AttackTh;
} AUD_AQ_AMP_DRC2_T;     //for TAS5171

typedef struct
{
    UINT32 u4Data[130];
} AUD_AQ_EXTPEQ_DATA2_T; // for TAS5171

typedef struct
{
    UINT8               u1ReleaseRate0Hi;        //band 0, DRC0
    UINT8               u1ReleaseRate0Mid;
    UINT8               u1ReleaseRate0Lo;
    UINT8               u1AttackRate0Hi;
    UINT8               u1AttackRate0Mid;
    UINT8               u1AttackRate0Lo;
    UINT8               u1LimiterTH0Hi;
    UINT8               u1LimiterTH0Mid;
    UINT8               u1LimiterTH0Lo;
    UINT8               u1CompressorRate0Hi;
    UINT8               u1CompressorRate0Mid;
    UINT8               u1CompressorRate0Lo;
    UINT8               u1CompressorTH0Hi;
    UINT8               u1CompressorTH0Mid;
    UINT8               u1CompressorTH0Lo;

    UINT8               u1ReleaseRate1Hi;       //band 0, DRC2
    UINT8               u1ReleaseRate1Mid;
    UINT8               u1ReleaseRate1Lo;
    UINT8               u1AttackRate1Hi;
    UINT8               u1AttackRate1Mid;
    UINT8               u1AttackRate1Lo;
    UINT8               u1LimiterTH1Hi;
    UINT8               u1LimiterTH1Mid;
    UINT8               u1LimiterTH1Lo;
    UINT8               u1CompressorRate1Hi;
    UINT8               u1CompressorRate1Mid;
    UINT8               u1CompressorRate1Lo;
    UINT8               u1CompressorTH1Hi;
    UINT8               u1CompressorTH1Mid;
    UINT8               u1CompressorTH1Lo;

    UINT8               u1ReleaseRate2Hi;      //band 1, DRC1
    UINT8               u1ReleaseRate2Mid;
    UINT8               u1ReleaseRate2Lo;
    UINT8               u1AttackRate2Hi;
    UINT8               u1AttackRate2Mid;
    UINT8               u1AttackRate2Lo;
    UINT8               u1LimiterTH2Hi;
    UINT8               u1LimiterTH2Mid;
    UINT8               u1LimiterTH2Lo;
    UINT8               u1CompressorRate2Hi;
    UINT8               u1CompressorRate2Mid;
    UINT8               u1CompressorRate2Lo;
    UINT8               u1CompressorTH2Hi;
    UINT8               u1CompressorTH2Mid;
    UINT8               u1CompressorTH2Lo;

    UINT8               u1ReleaseRate3Hi;      //band 1, DRC3
    UINT8               u1ReleaseRate3Mid;
    UINT8               u1ReleaseRate3Lo;
    UINT8               u1AttackRate3Hi;
    UINT8               u1AttackRate3Mid;
    UINT8               u1AttackRate3Lo;
    UINT8               u1LimiterTH3Hi;
    UINT8               u1LimiterTH3Mid;
    UINT8               u1LimiterTH3Lo;
    UINT8               u1CompressorRate3Hi;
    UINT8               u1CompressorRate3Mid;
    UINT8               u1CompressorRate3Lo;
    UINT8               u1CompressorTH3Hi;
    UINT8               u1CompressorTH3Mid;
    UINT8               u1CompressorTH3Lo;

    UINT8               u1OffsetDRC0Hi;
    UINT8               u1OffsetDRC0Mid;
    //    UINT8               u1OffsetDRC0Lo;
    UINT8               u1OffsetDRC1Hi;
    UINT8               u1OffsetDRC1Mid;
    //    UINT8               u1OffsetDRC1Lo;
    UINT8               u1OffsetDRC2Hi;
    UINT8               u1OffsetDRC2Mid;
    //    UINT8               u1OffsetDRC2Lo;
    UINT8               u1OffsetDRC3Hi;
    UINT8               u1OffsetDRC3Mid;
    //    UINT8               u1OffsetDRC3Lo;
    UINT8               u1STCCFG0;
    UINT8               u1STCCFG1;
    UINT8               u1Reserved0;
    UINT8               u1Reserved1;

    UINT8               u1DynamicAttackRateHi;
    UINT8               u1DynamicAttackRateMid;
    UINT8               u1DynamicAttackRateLo;

} AUD_AQ_AMP_STC_COEFF_T;

typedef struct
{
    UINT8 u1Data[15];
}
AUD_AQ_AMP_RAM_DATA_T;

typedef enum
{
    AQ_OP_EXE = 0,
    AQ_OP_INIT
} AUD_AQ_OP_TYPE_T;

typedef enum
{
    AQ_EXT_PEQ_HP = 0,
    AQ_EXT_PEQ_LP,
    AQ_EXT_PEQ_BP,
    AQ_EXT_PEQ_FLAT
} AUD_AQ_EXTPEQ_TYPE_T;

typedef struct
{
    UINT32 u4Data[AQ_EXT_PEQ_ITEMS];
} AUD_AQ_EXTPEQ_DATA_T;

typedef enum
{
    AQ_AMP_FS_32K = 0,
    AQ_AMP_FS_44K,
    AQ_AMP_FS_48K,
    AQ_AMP_FS_NUM
} AUD_AQ_AMP_FS_T;

typedef struct
{
    UINT32 u4SpkCfg;
    UINT32 u4Cutoff;
    UINT8 u1Panorama;
    UINT8 u1Dimesion;
    UINT8 u1CWidth;
} AUD_AQ_TYPE3_DATA_T;

typedef enum
{
    AQ_BL_ITEM_START,
    TEMP_TH_LOW = 0,
    TEMP_TH_HIGH,
    TEMP_TH_LIMIT,
    BL_TEMP_CNTTH,
    BL_TEMP_INC,
    TEMP_BL_BOTTOM,
    OPT_BL_DEADB,
    OPT_BL_DEADB_CIN,
    BL_CRI_TEMP_L,
    BL_CRI_TEMP_H,
    TH_SHTDNLIMIT,
    AQ_BL_ITEM_END
} AUD_AQ_BL_REG;
//===================================================

#ifdef SUPPORT_FLASH_AQ
typedef struct
{
    CHAR au1AudId[4];   //00
    CHAR au1AudVersion[8]; //04
    //Input Source Volume
    INT16 aai2AudInputSrcVol[4 /*AUD_DEC_NUM*/][10 /*AUD_STREAM_FROM_NUM*/]; //04  + 8 + 2 * 4 * 10 = 92
    //Master Volume Table
    UINT32 au4VolumeTable[100 + 1]; //92 + 4 * 101 = 496
    //Channel Volume Table
    UINT32 au4ChannelVolumeTable[100 + 1]; //496 + 4 * 101 = 900
    //Limiter
    UINT32 u4AudioLimiterMode; //900 + 4 = 904
    UINT32 u4AudioLimiterThreshold; //904 + 4 = 908
    //Surround
    UINT32 u4AudioVsClarity; //908 + 4 = 912
    UINT32 u4AudioVsWidth;  //912 + 4 = 916
    UINT32 u4AudioVsLRGain; //916 + 4 = 920
    UINT32 u4AudioVsXTalk;  //920 + 4 = 924
    UINT32 u4AudioVsOutputGain; //924 + 4 = 928
    UINT32 u4AudioVsBassGain;   //928 + 4 = 932
    UINT32 u4AudioVsFO; //932 + 4 = 936

    //from the following, if you want to add UINT32 element, please make sure its start address is DWRD alignment
    //also, at least, you must make sure each UINT16 element is word alignment

    //EQ
    INT8 aai1AudChlEqCfg[AUD_EQ_NUM][CH_EQ_BAND_NO + 1]; //936 + 1 * 19 * 6 = 1050
    //AVC
    INT16 ai2AudAvcTarget[10 /*AUD_STREAM_FROM_NUM*/];     //1050 + 2 * 10 = 1070
    INT16 ai2AudAvcGainUp[10 /*AUD_STREAM_FROM_NUM*/];     //1070 + 2 * 10 = 1090
    INT16 ai2AudAvcAdjRate[10 /*AUD_STREAM_FROM_NUM*/];    //1090 + 2 * 10 = 1110
    INT16 ai2AudAvcUiAdjRate[10 /*AUD_STREAM_FROM_NUM*/];  //1110 + 2 * 10 = 1130
    //Channel Volume(Extra-Gain)
    INT8 aai1AudChVolExtraGain[4 /*AUD_DEC_NUM*/][AUD_CH_NUM];    //1130 + 1 * 4 * 15 = 1190
    //Channel Delay
    UINT8 au1AudChDelay[10 /*AUD_STREAM_FROM_NUM*/];   //1190 + 1 * 10  => 1200

    UINT8 u1AudioLRSpkTypeLarge;        //1200+1 => 1201    (AUDIO_SPK_TYPE_LARGE => eAudioSpkType)
    UINT8 u1AudioSLRSpkTypeLarge;       //1201+1 => 1202    (AUDIO_SURROUND_SPK_TYPE_LARGE => eAudioSurroundSpkType)
    UINT8 u1AudioCenterSpkTypeLarge;    //1202+1 => 1203    (AUDIO_CENTER_SPK_TYPE_LARGE => eAudioCenterSpkType)
    UINT8 u1AudioCH910SpkTypeLarge;     //1203+1 => 1204    (AUDIO_CH9_10_SPK_TYPE_LARGE => eAudioCh910SpkType)
    UINT8 u1AudioSubwooferOn;           //1204+1 => 1205    (AUDIO_SUBWOOFER_ON => eAudioSubwooferOn)
    UINT8 u1Reserved;                   //1205+1 => 1206
    UINT16 u2AudioBassMngCutoffFreq;     //1206+2 => 1208    (AUDIO_BASS_MNG_CUTOFF_FREQ => eAudioBassMngCutOffFreq)
    
    UINT32 u4PEQFlag;
    UINT32 au4PEQFrequency[9];
    UINT32 au4PEQBW[9];
    UINT32 au4PEQGain[9];

    UINT8 u1ChkSum[4];
    //UINT32 u4ChkSum; //1208+4 => 1212
} FLASH_AQ_T;

extern FLASH_AQ_T rAudFlashAQ;

#define FLASH_AQ_HDR_OFFSET 12

#define MULTIPLE_FLASH_AQ 1

#endif

//----------- End of NVM and Profile Data Types -------------------------------

typedef enum
{
    AUD_SUR_CLARITY = 0,
    AUD_SUR_WIDTH,
    AUD_SUR_LRGAIN,
    AUD_SUR_XTALK,
    AUD_SUR_OUTPUT_GAIN,
    AUD_SUR_BASS_GAIN,
    AUD_SUR_FO,
    AUD_SUR_PRAR_NUM
}   AUD_VSURR_CFG_TYPE_T;

typedef struct
{
    UINT32 u4Clarity;
    UINT32 u4Width;
    UINT32 u4LRGain;
    UINT32 u4Xtalk;
    UINT32 u4OutputGain;
    UINT32 u4BassGain;
    UINT32 u4Fo;
} AUD_VSURR_CFG_T;

typedef struct
{
    UINT16 u2Pktno;        //W_WMA_PACKET_NO
    UINT16 u2HdrSrc;       //W_WMA_HEADER_SOURCE
    UINT32 u4Pktcnt;       //D_WMA_PACKET_COUNT
    UINT32 u4Pktsz;        //D_WMA_PACKET_SIZE
    UINT16 u2Wmaver;       //W_WMA_VERSION
    UINT16 u2Numch;        //W_WMA_NUM_CH
    UINT32 u4Sampersec;    //D_WMA_SAMPLE_PER_SEC
    UINT32 u4Bpersec;      //D_WMA_BYTE_PER_SEC
    UINT32 u4Blocksz;      //D_WMA_BLOCK_SIZE
    UINT16 u2Encoderopt;   //W_WMA_ENCODER_OPTION
    UINT32 u4ChannelMask;   //W_WMA_CHANNEL_MASK
    UINT16 u2BitsPerSample;   //W_WMA_BITS_SAMPLE
    UINT16 u2AdvEncoderopt;   //W_WMA_ADV_OPTION
    UINT32 u4AdvEncoderopt2;   //W_WMA_ADV_OPTION2
    UINT32 u4VoiceEncodeOptions1;
    UINT32 u4VoiceEncodeOptions2[2];
} AUD_WMA_CFG_T;
#if 1 //CC_APE_SUPPORT
//ian APE decoder
typedef struct
{
    UINT16 u2ParseAudOnly;          //W_APE_PARSE_AUDIO_ONLY
    UINT16 u2FileVersion;              //W_APE_FILE_VESION
    UINT32 u4CompressLevel;        //D_APE_COMPRESS_LEVEL
    UINT32 u4BlockPerFrm;           //D_APE_BLOCK_PER_FRAME
    UINT32 u4FinalFrmBlock;        //D_APE_FINAL_FRAME_BLOCK
    UINT16 u2TotalFrmNumLow;    //W_APE_TOTAL_FRAME_NUM_LOW
    UINT16 u2TotalFrmNumHigh;   //W_APE_TOTAL_FRAME_NUM_HIGH
    UINT16 u2Bpersample;            //W_APE_BITS_PER_SAMPLE
    UINT16 u2CHNum;                   //W_APE_CHANNEL_NUM
    UINT32 u4SamplingRate;        //D_APE_INPUT_SAMPLING_RATE
    UINT16 u2MuteBankNum;        //W_APE_MUTE_BANK_NUMBERS
    UINT16 u2InvalidBytes;           //W_APE_INVALID_BYTES
} AUD_APE_CFG_T;
#endif
typedef struct
{
    UINT16 u2Numch;
    UINT32 u4Sampersec;
    UINT16 u2BitsPerSample;
} AUD_AMR_CFG_T;

typedef struct
{
    UINT16 u2SampPerFrame;        //W_COOK_SAMP_PER_FRAME
    UINT16 u2ChanNum;       //W_COOK_CHANNEL_NUM
    UINT16 u2SampRate;       //W_COOK_INPUT_SAMPLING_RATE
    UINT16 u2FrameSzInBytes;        //W_COOK_FRM_SZ_IN_BYTES
    UINT16 u2RegnNum;       //W_COOK_REGION_NUM
    UINT16 u2StartRegn;        //W_COOK_COUPLING_START_REGN
    UINT16 u2CplQBits;    //W_COOK_COUPLING_QBITS

} AUD_COOK_CFG_T;

typedef struct//paul_flac
{
    UINT16 u2MinBlkSz;//Minimum block size
    UINT16 u2MaxBlkSz;//Maximum block size
    UINT16 u2ChannelNum;//channel num
    UINT16 u2BPS;//bits per sample
    UINT32 u4MinFrmSz;//Minimum frame size
    UINT32 u4MaxFrmSz;//Maximum frame size
    UINT32 u4SmpRate;//Sample rate
} AUD_FLAC_INFO_T;

typedef struct
{
    UINT8   u1SmartRev[16];
} AUD_SMART_AUD_INFO_T;

typedef enum
{
    PCM_24_BITS = 0,
    PCM_IMA_ADPCM_BIG_ENDIAN = 1,
    PCM_16_BITS_BIG_ENDIAN  = 2,
    PCM_MS_ADPCM_BIG_ENDIAN  = 3,
    PCM_IMA_ADPCM_LITTLE_ENDIAN = 1 | (1 << 15),
    PCM_16_BITS_LITTLE_ENDIAN = 2 | (1 << 15),
    PCM_MS_ADPCM_LITTLE_ENDIAN = 3 | (1 << 15),
    PCM_A_LAW_LITTLE_ENDIAN = (1 << 15) | (1 << 14),
    PCM_U_LAW_LITTLE_ENDIAN = (1 << 15) | (1 << 13)
} PCM_INPUT_TYPE_T;

/// Audio PCM type
typedef struct
{
    SAMPLE_FREQ_T      eSampleFreq;  ///< PCM Sampling Rate.
    PCM_INPUT_TYPE_T   ePcmDataInfo; ///< PCM Sampling Rate.
#ifdef CC_HDMI_PCM_MULT_CHANNEL
    UINT8  ePCMChNum;   /// PCM Input Channel Number
#endif
} AUD_PCM_SETTING_T;

typedef enum
{
    DEC_LITTLE_ENDIAN = 0,
    DEC_BIG_ENDIAN
} DATA_ENDIAN_T;

/// Audio AC3 type
typedef struct
{
    DATA_ENDIAN_T      eEndian;  /// input data endian
} AUD_AC3_SETTING_T;

//Dynamic volume curve setting  //Andrew Wen 07/8/20
#define VOL_TABLE_CHK_PNT 7

/// Callbalck function if clip playback is done
typedef INT32 (*AUD_CLIP_NOTIFY)(UINT8 u1DecId, UINT32 u1Datat, UINT32 u4Datat2);

#ifdef CC_53XX_SWDMX_V2
typedef void (*AUD_WMAErr_NOTIFY)(VOID* prInst, UINT32 u4WMAErrCode);  // megaa 20090828
#else
typedef void (*AUD_WMAErr_NOTIFY)(UINT32 u4WMAErrCode);
#endif
//static AUD_WMAErr_NOTIFY _hWMAErrNotifyFunc = NULL;

//----- LPCM decoder -----
typedef enum
{
    LPCM_TYPE_NORAML = 0,
    LPCM_TYPE_MS_ADPCM = 1,
    LPCM_TYPE_IMA_ADPCM = 2,
    LPCM_TYPE_A_LAW = 3,
    LPCM_TYPE_U_LAW = 4
} LPCM_INPUT_TYPE_T;
typedef struct
{
    UINT8 isUpdate;
    VOID* pParams;
    UINT16 noParam;
    UINT8 dataOption;
    UINT8 varOption;
} LGSE_FN_PARAM_T;

typedef struct
{
    SAMPLE_FREQ_T      eSampleFreq;
    LPCM_INPUT_TYPE_T  eLPcmType;
    UINT16             ui2_num_ch;
    UINT16             ui2_block_align;
    UINT16             ui2_bits_per_sample;
    BOOL               b_big_endian;
    BOOL               b_info_not_specific; /*pcm info from dmx*/
    BOOL               b_signed_data;
} AUD_LPCM_INFO_T;

typedef struct
{
    UINT8   u1LpcmChAssignment;
    UINT8   u1LpcmFreqGrp1;
    UINT8   u1LpcmFreqGrp2;
    UINT8   u1LpcmBitShift;
    UINT8   u1LpcmDrcValue;
    UINT8   u1LpcmBitResGrp1;
    UINT8   u1LpcmBitResGrp2;
    UINT8   u1LpcmDrcFlag;
    UINT16  u2LpcmDecodingType;
    UINT16  u2ADPCMDecodingType;
    UINT16  u2ADPCMBlockAlign;
} AUD_LPCM_SETTING_T;
//----- end of LPCM -----

#ifdef CC_AUD_HP_DEPOP  ///For Remy, need remove "CC_DAC_DE_POP".
typedef enum
{
    AUD_HP_DEPOP_DC_ON = 0,
    AUD_HP_DEPOP_CASE_D,
    AUD_HP_DEPOP_CASE_E,
    AUD_HP_DEPOP_CASE_G,
    AUD_HP_DEPOP_CASE_I,
    AUD_HP_DEPOP_DC_OFF,
    AUD_HP_DEPOP_IDLE,
} AUD_HP_DEPOP_CMD_T ;
#endif

#if 1 //CC_AUD_QUERY_VDO_INFO
typedef enum
{
    AUD_SRM_VDO_480 = 0,
    AUD_SRM_VDO_576,
    AUD_SRM_VDO_720,
    AUD_SRM_VDO_1080,
    AUD_SRM_VDO_HEIGHT_NUM
} AUD_SRM_VDO_HEIGHT_T;

typedef enum
{
    AUD_SRM_VDO_P = 0,
    AUD_SRM_VDO_I,
    AUD_SRM_VDO_IP_NUM
} AUD_SRM_VDO_IP_T;

typedef enum
{
    AUD_SRM_VDO_50HZ = 0,
    AUD_SRM_VDO_60HZ,
    AUD_SRM_VDO_RATE_NUM
} AUD_SRM_VDO_RATE_T;

typedef enum
{
    AUD_SRM_VDO_NO_CUST = 0,
    AUD_SRM_VDO_MIXED_CUST,
    AUD_SRM_VDO_ALL_CUST
} AUD_SRM_VDO_DELAY_CUST_TYPE_T;

typedef struct
{
    INT16 i2Delay[AUD_SRM_VDO_HEIGHT_NUM][AUD_SRM_VDO_IP_NUM][AUD_SRM_VDO_RATE_NUM];
} AUD_SRM_VDO_DELAY_TBL;
#endif

#define VM_FILTER1_NUM   6  ///CC_VOCAL_MUTE
#define VM_FILTER2_NUM   6  ///CC_VOCAL_MUTE

//----- HDMI Pre_parser setting -----
#ifdef CC_AUD_HDMI_PARSER_2_0_SUPPORT
typedef enum
{
    AUD_HDMI_PARSER_INIT = 0,
    //AUD_HDMI_PARSER_BUFFERING, //remove buffering,it may cause some output underrun
    AUD_HDMI_PARSER_FIND_VALID_FRAME,
    AUD_HDMI_PARSER_WAIT_VALID_FRAME,
    AUD_HDMI_PARSER_TANSFER_FRAME,

}   AUD_HDMI_PARSER_STATE_T;

/*
typedef enum
{
    AUD_HDMI_NOT_FIND_PA,
    AUD_HDMI_FIND_PA,
    AUD_HDMI_FIND_PB,
}   AUD_HDMI_CHECK_SYNC_WORD_T;
*/

typedef enum
{
    AUD_RAW_AC3 = 0,    //AC3
    AUD_RAW_PAUSE,      // Pause frame
    AUD_RAW_MPEG_1_LY_1, //mpeg1 layer1
    AUD_RAW_MPEG_2_W_O,  //mpeg 1 layer 2/3,mpeg2 w/0 extension
    AUD_RAW_MPEG_2_W,    //mpeg 2 w extension
    AUD_RAW_AAC,
    AUD_RAW_MPEG_2_LY_1,  //mpeg 2 layer 1 LSF
    AUD_RAW_MPEG_2_LY_2_3, //mpeg-2 layer 2/3 LSF
    AUD_RAW_DTS_1,            //DTS type I
    AUD_RAW_DTS_2,            //DTS type II
    AUD_RAW_DTS_3,            //DTS type III
    AUD_RAW_DTS_HD,         //DTS HD
    AUD_RAW_E_AC3,          //E-AC3
    AUD_RAW_AC3_HD,         //Dolby Tru HD
    AUD_RAW_NUM,
} AUD_RAW_FMT_T;

typedef struct
{
    UINT32 u4FifoStartAddr;
    UINT32 u4FifoEndAddr;
    UINT32 u4FifoSize;
    UINT32 u4FifoPtr;
}  AUD_FIFO_INFO_T;


typedef struct
{
    UINT16 u2format;  //pc
    UINT32 u4Size;    //pd
    UINT32 u4Wp; //bit-stream start
}  AUD_HDMI_RAW_DATA_INFO_T;

typedef struct
{
    AUD_FIFO_INFO_T eHDMIBuffer;
    AUD_FIFO_INFO_T eDSPBuffer;
    HANDLE_T _hSemaHDMIParserThreadWait;
    BOOL _fgHDMIParserThreadGo;
    AUD_FMT_T e_aud_det_fmt;
    BOOL _fgFmtChg;
    AUD_HDMI_RAW_DATA_INFO_T eSpdifInfo;
    //AUD_HDMI_CHECK_SYNC_WORD_T eCheckSta;
    UINT32 u4PaPb;
    AUD_HDMI_PARSER_STATE_T eHDMIParserSta;
    UINT32 u4LastTransferPtr;  //It's used for PCM transfer
    UINT32 u4NoRawDataSize;
    UINT32 u4NonZeroDataSize; 
    UINT32 u4NoTransferSize;
    UINT32 u4TFrameCount;      //Tranfer frame count, for debug   
    BOOL _fgDTSCD;   //_fgDTSCD and _fgDTSSW is for DTS CD,DTS CD don't have papbpcpd,we should reference DTS header
    BOOL _fgDTSSW;    //find DTS Sync word
    BOOL _fgRaw;
    BOOL _fgReset;
} AUD_HDMI_PARSER_INFO_T;


#endif

typedef enum
{
    AUD_HDMI_HDCP = 0,
    AUD_HDMI_INFO_NUM,
}   AUD_HDMI_INFO_T;
//----- end of HDMI Pre_parser-----

typedef enum
{
    AUD_USER_SET_DEC_INPUT_MUTE,
    AUD_USER_SET_DEC_INPUT_VOL,
    AUD_USER_SET_DEC_OUTPUT_VOL,
    AUD_USER_SET_DEC_CHANNEL_GAIN,
    AUD_USER_SET_MIX_INPUT_MUTE, 
    AUD_USER_SET_MIX_INPUT_VOL,
    AUD_USER_SET_MIX_OUTPUT_VOL,
    AUD_USER_SET_DEC_INPUT_DELAY,
    AUD_USER_SET_DEC_OUT_CTRL,
    AUD_USER_SET_SPDIF_RAW_DEC,
    AUD_USER_SET_CMD_NUM
}AUD_USER_SET_CMD_T;


typedef enum
{
    AUD_DEC_CMD_SET_STATE,
    AUD_DEC_CMD_TERMINATE,
    AUD_DEC_CMD_MAX
} AUD_DEC_CMD;

typedef enum
{
    AUD_DEC_STATE_INIT,
    AUD_DEC_STATE_PLAY,
    AUD_DEC_STATE_STOP,
    AUD_DEC_STATE_IDLE,
    AUD_DET_STATE_MAX
} AUD_DEC_STATE;

typedef struct
{
    AUD_DEC_CMD       eCmd;
    AUD_DEC_STATE     eStateTo; // for AUD_OMX_CMD_SET_STATE
    //for the future extension
} AUD_DEC_MSG_T;

typedef enum
{
    AUD_STC_AUDIO = 0,     // for some legacy heritage, STC_A and demux STC1(0x1723C)
    AUD_STC_DMX,             // for some legacy heritage, demux_STC2(0x1728C)
    AUD_STC_DMX2            // currently cannot be used
} AUD_STC_ID; // 2_STC_Task

typedef struct      //PTSQueue_V2
{
    UINT32       u4Wp;
    UINT32       u4Pts;
    UINT32       u4Refresh;
    UINT32       u4Expired;
    UINT32       u4DmxWp;
    UINT32       u4Reserved4;
    UINT32       u4Reserved5;
    UINT32       u4Reserved6;
    UINT32       u4Reserved7;
    UINT32       u4Reserved8;
    UINT32       u4Reserved9;
    UINT32       u4Reserved10;
    UINT32       u4Reserved11;
    UINT32       u4Reserved12;
    UINT32       u4Reserved13;
    UINT32       u4Reserved14;
} AUD_PES_INFO_T;

typedef enum
{
    AUD_ENC_MODE_OFF = 0,
    AUD_ENC_MODE_INPUT_BEFORE_POST_PROCESSING,
    AUD_ENC_MODE_INPUT_AFTER_POST_PROCESSING,
    AUD_ENC_MODE_NUM
}   AUD_ENC_MODE_T;

#ifdef CC_ENABLE_AOMX
typedef struct {
    UINT8 u1DecId;
    UINT32 u4Wp;                        ///< Write pointer
    UINT64 u8Pts;                       ///< PTS
    BOOL fgEOS;                         ///< File playback: End of stream
}GST_AUDIO_PES_T;
#endif

#ifdef CC_AUD_DRC_V20
typedef enum
{
    DRC2_FLAG = 0,
    DRC2_TARGET_LEVEL,
    DRC2_ATTACK_RATE,
    DRC2_RELEASE_RATE,
    DRC2_RATIO,
    DRC2_SILENCE_LEVEL,
    DRC2_MAX_EXPAND,
    DRC2_HYSTERESIS,
    DRC2_HOLD,
    DRC2_TARGET_LEVEL_1,
    DRC2_ATTACK_RATE_1,
    DRC2_RELEASE_RATE_1,
    DRC2_RATIO_1,
    DRC2_SILENCE_LEVEL_1,
    DRC2_MAX_EXPAND_1,
    DRC2_HYSTERESIS_1,
    DRC2_HOLD_1,
    DRC2_LIMITER_POST_DRC_MODE,
    DRC2_NUM
} AUD_DRC_V20_INFO_T;
#endif

/* Prologic 2 mode */
#define PROLOGIC_EMUATION       (UINT16)0
#define VIRUTAL_MODE            (UINT16)1
#define MUSIC_MODE              (UINT16)2
#define MOVIE_MODE              (UINT16)3
#define MATRIX_MODE             (UINT16)4
#define CUSTOM_MODE             (UINT16)7

/* Prologic 2 setting */
#define PL2_ENABLE              (UINT16)1       ///< Prologic 2 enable/disable
#define PL2_MODE_AUTO           (UINT16)(1<<1)  ///< Prologic 2 mode is decided from AC3 decoder
#define PL2_PCD_AUTO            (UINT16)(1<<2)  ///< Prologic 2 panorama, center width, dimention is decided from AC3 decoder
#define PL2_AUTO_ENABLE         (UINT16)(1<<3)  ///< Prologic 2 enable/disable from AC3 decoder
#define PL2_AUTO_BALANCE        (UINT16)(1<<4)  ///< auto balance
#define PL2_RS_POLAR_INV        (UINT16)(1<<5)  ///< invert polarity of RS's output
#define PL2_PANORAMA            (UINT16)(1<<6)  ///< default OFF
#define PL2_SURR_FILTER         (UINT16)(1<<7)  ///< surround shelf filter
/* Prologic 2 center width */
#define PL2_CWIDTH(a)           (UINT16)((0x0f & (a))<<8)   ///< center width spread value 0~7 default 3
#define PL2_DIM(a)              (UINT16)((0x0f & (a))<<12)  ///< dimention control -3~3 default 0

/* Prologic 2 control routine related*/
#define PL2CTRL_SWITCH          0
#define PL2CTRL_MODE            1
#define PL2CTRL_PANORAMA        2
#define PL2CTRL_DIMENSION       3
#define PL2CTRL_C_WIDTH         4

/* Prologic 2 control related*/
#define PL2CTRL_ON              0
#define PL2CTRL_OFF             1
#define PL2CTRL_AUTO            2
/* Prologic 2 mode related*/
#define PL2CTRL_MODE_MANUAL     0
#define PL2CTRL_MODE_AUTO       1
/* Prologic 2 Panorama related*/
#define PL2CTRL_PAN_ON          0
#define PL2CTRL_PAN_OFF         1

#define AC3_KARA_DISABLE        ((UINT16)0x0001)    ///< disable karaoke mode
#define AC3_KARA_AWARE          ((UINT16)0x0002)    ///< karaoke aware mode
#define AC3_KARA_NONE           ((UINT16)0x0004)    ///< none (in capable mode)
#define AC3_KARA_V1             ((UINT16)0x0008)    ///< V1 only (in capable mode)
#define AC3_KARA_V2             ((UINT16)0x0010)    ///< V2 only (in capable mode)
#define AC3_KARA_BOTH           ((UINT16)0x0020)    ///< V1+V2   (in capable mode)
#define AC3_KARA_VOC_ASIST      ((UINT16)0x0040)    ///< vocal assist mode
#define AC3_KARA_NO_MELODY      ((UINT16)0x0080)    ///< melody off

#define AC3_DM_STEREO           0   ///< AC3 dual mono mode stereo
#define AC3_DM_L_MONO           1   ///< AC3 dual mono mode left mono
#define AC3_DM_R_MONO           2   ///< AC3 dual mono mode right mono
#define AC3_DM_M_MONO           3   ///< AC3 dual mono mode mix mono

#define AC3_COMP_CUSTOM_0       0   ///< AC3 compression mode custom 0
#define AC3_COMP_CUSTOM_1       1   ///< AC3 compression mode custom 1
#define AC3_COMP_LINE           2   ///< AC3 compression line mode P.S. default set as line mode
#define AC3_COMP_RF             3   ///< AC3 compression RF mode
#define AC3_COMP_WLINE          4   ///< AC3 compression wide range line mode

// *********************************************************************
// Constant definitions
// *********************************************************************
#define AUD_OK                  (INT32)(0)
#define AUD_FAIL                (INT32)(-1)

#define AUD_A_DSP               ((UINT8)0)
#define AUD_D_DSP               ((UINT8)1)

#define AUD_DSP0                ((UINT8)0)
#define AUD_DSP1                ((UINT8)1)

#define AUD_DEC_MAIN            ((UINT8)0)
#define AUD_DEC_AUX             ((UINT8)1)
#define AUD_DEC_THIRD           ((UINT8)2)
#define AUD_DEC_4TH             ((UINT8)3)
#define AUD_DEC_SUB             ((UINT8)4)

#define AUD_LINEIN_MAIN         ((UINT8)0)
#define AUD_LINEIN_AUX          ((UINT8)1)
#define AUD_LINEIN_THIRD        ((UINT8)2)

#define INTERNAL_DAC_ID_MAIN    ((UINT8)0)
#define INTERNAL_DAC_ID_AUX     ((UINT8)1)
#define INTERNAL_DAC_ID_THIRD   ((UINT8)2)
#define INTERNAL_DAC_ID_4TH     ((UINT8)3)


/* Move AUD_DEC_NUM to drv_cust/drvcust_if.h */

#define AUD_CMD_FLAG_PLAY       ((UINT32) (1<<(UINT32)AUD_CMD_PLAY))
#define AUD_CMD_FLAG_STOP       ((UINT32) (1<<(UINT32)AUD_CMD_STOP))
#define AUD_CMD_FLAG_RESET      ((UINT32) (1<<(UINT32)AUD_CMD_RESET))
#define AUD_CMD_FLAG_PAUSE      ((UINT32) (1<<(UINT32)AUD_CMD_PAUSE))
#define AUD_CMD_FLAG_AVSYNC     ((UINT32) (1<<(UINT32)AUD_CMD_AVSYNC))
#define AUD_CMD_FLAG_LOADCODE   ((UINT32) (1<<(UINT32)AUD_CMD_LOADCODE))
#define AUD_CMD_FLAG_RESUME     ((UINT32) (1<<(UINT32)AUD_CMD_RESUME))
#define AUD_CMD_FLAG_STOPSTREAM ((UINT32) (1<<(UINT32)AUD_CMD_STOPSTREAM))
#define AUD_CMD_FLAG_CHANGE_FORMAT     ((UINT32) (1<<(UINT32)AUD_CMD_CHANGE_FORMAT))

#define AUD_DUAL_DEC_CMD_FLAG_PLAY  ((UINT32) (1<<(UINT32)AUD_DUAL_DEC_CMD_PLAY))
#define AUD_DUAL_DEC_CMD_FLAG_STOP  ((UINT32) (1<<(UINT32)AUD_DUAL_DEC_CMD_STOP))

/* AV Sync Debug Log Switch */
#define AUD_DBG_SYNC_SEND_PTS       ((UINT16) (1<<(UINT16)0))
#define AUD_DBG_SYNC_STC_DIFF       ((UINT16) (1<<(UINT16)1))
#define AUD_DBG_SYNC_PTS_FRESH      ((UINT16) (1<<(UINT16)2))


#ifdef CC_AUD_ARM_SUPPORT  //#A0008
/* Audio to Main signals
*/
// Signal
#define APROC_A2M_SIG_REG APROC_A2MSR0
#define APROC_A2M_DATA0_REG APROC_A2MSR1
#define APROC_A2M_DATA1_REG APROC_A2MSR2
#define APROC_A2M_READ_REG APROC_A2MSR3
// Log
#define APROC_A2M_LOG_ADDR APROC_A2MSR31
#define APROC_A2M_LOG_DATA0 APROC_A2MSR30
#define APROC_A2M_LOG_DATA1 APROC_A2MSR29
#define APROC_A2M_LOG_DATA2 APROC_A2MSR28
#define APROC_A2M_LOG_DATA3 APROC_A2MSR27
// Isr Log
#define APROC_A2M_ISR_LOG_ADDR APROC_A2MSR26
#define APROC_A2M_ISR_LOG_DATA0 APROC_A2MSR25
#define APROC_A2M_ISR_LOG_DATA1 APROC_A2MSR24
#define APROC_A2M_ISR_LOG_DATA2 APROC_A2MSR23
#define APROC_A2M_ISR_LOG_DATA3 APROC_A2MSR22
/* Main to Audio signals
*/
#define APROC_M2A_SIG_REG APROC_M2ASR0
#define APROC_M2A_DATA0_REG APROC_M2ASR1
#define APROC_M2A_DATA1_REG APROC_M2ASR2

#define AUD_APROC_CHAR_NUM 2000 //2048
#define AUD_APROC_CHAR_LEN (AUD_APROC_CHAR_NUM+3+2)

/* Defines
*/
//---------------------------------------------------------------------------
// Signal define (Sync. with APROC)
//
// Signal structure:
// u4Sig, u4Data0, u4Data1
//

/* Signal = APROC_SIGNAL(Idx,Cmd) = [23,8] Idx, [7,0] Cmd
*/
#define APROC_SIG_CMD_MSK           0xFF
#define APROC_SIG_CMD(a)            ((a) & APROC_SIG_CMD_MSK)
#define APROC_SIG_IDX_MSK           0xFFFF
#define APROC_SIG_IDX_SHIFT 8
#define APROC_SIG_IDX(a)            ((a) & APROC_SIG_IDX_MSK)
#define APROC_SIGNAL(Idx,Cmd)       ((APROC_SIG_IDX(Idx) << APROC_SIG_IDX_SHIFT) | APROC_SIG_CMD(Cmd))

/* u4Sig
*/
// Cmd
#define APROC_SIG_CMD_EXIT_CODE     APROC_SIG_CMD(0x0) // kernel usage
#define APROC_SIG_CMD_READ          APROC_SIG_CMD(0x1)
#define APROC_SIG_CMD_WRITE         APROC_SIG_CMD(0x2)
#define APROC_SIG_CMD_OPEN          APROC_SIG_CMD(0x3)
#define APROC_SIG_CMD_CLOSE         APROC_SIG_CMD(0x4)
#define APROC_SIG_CMD_INIT          APROC_SIG_CMD(0x5)
#define APROC_SIG_CMD_SET           APROC_SIG_CMD(0x6)
#define APROC_SIG_CMD_GET           APROC_SIG_CMD(0x7)
#define APROC_SIG_CMD_NOTIFY        APROC_SIG_CMD(0x8)
// Idx
#define APROC_SIG_IDX_KERNEL        APROC_SIG_IDX(0x1)
#define APROC_SIG_IDX_POSTPROC      APROC_SIG_IDX(0x2)
#define APROC_SIG_IDX_ADSP          APROC_SIG_IDX(0x3)
#define APROC_SIG_IDX_IOBUF         APROC_SIG_IDX(0x4)
#define APROC_SIG_IDX_APROC         APROC_SIG_IDX(0x5)
#define APROC_SIG_IDX_DEC0          APROC_SIG_IDX(0x6)
#define APROC_SIG_IDX_DEC1          APROC_SIG_IDX(0x7)
#define APROC_SIG_IDX_DEC2          APROC_SIG_IDX(0x8)
#define APROC_SIG_IDX_DEC3          APROC_SIG_IDX(0x9)
#define APROC_SIG_IDX_MIXSND0       APROC_SIG_IDX(0xA) //parson mixsnd
#define APROC_SIG_IDX_MIXSND1       APROC_SIG_IDX(0xB)
#define APROC_SIG_IDX_MIXSND2       APROC_SIG_IDX(0xC) //parson mixsnd
#define APROC_SIG_IDX_MIXSND3       APROC_SIG_IDX(0xD)
#define APROC_SIG_IDX_UPLOAD0        APROC_SIG_IDX(0xE) //parson upload
#define APROC_SIG_IDX_SRC           APROC_SIG_IDX(0xF)
#define APROC_SIG_IDX_RISCPOST      APROC_SIG_IDX(0x10)  //parson riscpost
#define APROC_SIG_IDX_HW_MIXSND0    APROC_SIG_IDX(0x11)
#define APROC_SIG_IDX_HW_MIXSND1    APROC_SIG_IDX(0x12)
#define APROC_SIG_IDX_UPLOAD1		APROC_SIG_IDX(0x13)
#define APROC_SIG_IDX_UPLOAD2		APROC_SIG_IDX(0x14)
#define APROC_SIG_IDX_DEC2_AD       APROC_SIG_IDX(0x15)
#define APROC_SIG_IDX_MIXSND4       APROC_SIG_IDX(0x16) //parson mixsnd
#define APROC_SIG_IDX_MIXSND5       APROC_SIG_IDX(0x17)
#define APROC_SIG_IDX_MIXSND6       APROC_SIG_IDX(0x18) //parson mixsnd
#define APROC_SIG_IDX_MIXSND7       APROC_SIG_IDX(0x19)


//#define CC_AUD_APROC_HWMIX 
#endif //CC_AUD_ARM_SUPPORT  //#A0008

#if defined(CC_AUD_APOLLO_SUPPORT) || defined(CC_AUD_ARM_SUPPORT)//#A0002
/* APROC_SIG_CMD_SET u4Data0
*/
#define APROC_SIG_SET_UPDATE_RP  APROC_SIG_CMD(0x1)
#define APROC_SIG_SET_UPDATE_WP  APROC_SIG_CMD(0x2)
#define APROC_SIG_SET_RESET      APROC_SIG_CMD(0x3) //parson mixsnd
#define APROC_SIG_SET_DELAY0_ROUTINE     APROC_SIG_CMD(0x4) //parson delay
#define APROC_SIG_SET_DELAY1_ROUTINE     APROC_SIG_CMD(0x5)
#define APROC_SIG_SET_DELAY2_ROUTINE     APROC_SIG_CMD(0x6)
#define APROC_SIG_SET_SP_PATH_ROUTINE     APROC_SIG_CMD(0x7)
#define APROC_SIG_SET_HP_PATH_ROUTINE     APROC_SIG_CMD(0x8)
#define APROC_SIG_SET_DMIX0_ROUTINE       APROC_SIG_CMD(0x9)
#define APROC_SIG_SET_DMIX1_ROUTINE       APROC_SIG_CMD(0xA)
#define APROC_SIG_SET_DMIX2_ROUTINE       APROC_SIG_CMD(0xB)
#define APROC_SIG_SET_AMIXER0_ROUTINE       APROC_SIG_CMD(0xC)
#define APROC_SIG_SET_AMIXER1_ROUTINE       APROC_SIG_CMD(0xD)
#define APROC_SIG_SET_AMIXER2_ROUTINE       APROC_SIG_CMD(0xE)
#define APROC_SIG_SET_SP_LSRS_PATH_ROUTINE     APROC_SIG_CMD(0xF)
#define APROC_SIG_SET_DEV_WORKING       APROC_SIG_CMD(0x10)
#define APROC_SIG_SET_DEV_SLEEP         APROC_SIG_CMD(0x11)
#define APROC_SIG_SET_MON_PATH_ROUTINE     APROC_SIG_CMD(0x12)
#define APROC_SIG_SET_IEC_PATH_ROUTINE     APROC_SIG_CMD(0x13)
#define APROC_SIG_SET_DELAY3_ROUTINE     APROC_SIG_CMD(0x14)
#define APROC_SIG_SET_DELAY4_ROUTINE     APROC_SIG_CMD(0x15)
#define APROC_SIG_SET_DELAY5_ROUTINE     APROC_SIG_CMD(0x16)
#define APROC_SIG_SET_DELAY6_ROUTINE     APROC_SIG_CMD(0x17)    
#define APROC_SIG_SET_DELAY_INPUT0_ROUTINE    APROC_SIG_CMD(0x18)
#define APROC_SIG_SET_DELAY_INPUT1_ROUTINE    APROC_SIG_CMD(0x18)


/* APROC_SIG_CMD_OPEN u4Data0
*/ 
#define APROC_COMMAND_DONE          0x1 

/* APROC_SIG_CMD_NOTIFY u4Data0
*/ 
// Cmd
#define APROC_SIG_NOTIFY_DONE        APROC_SIG_CMD(0x1)
#define APROC_SIG_NOTIFY_WRITE      APROC_SIG_CMD(0x2)
#define APROC_SIG_NOTIFY_UPDATE_RP  APROC_SIG_CMD(0x3)
#define APROC_SIG_NOTIFY_UPDATE_WP  APROC_SIG_CMD(0x4)
#define APROC_SIG_NOTIFY_INIT_AOUT2 APROC_SIG_CMD(0x5)
#define APROC_SIG_NOTIFY_UPDATE_AOUT2_NSADR     APROC_SIG_CMD(0x6)
#define APROC_SIG_NOTIFY_APTS                   APROC_SIG_CMD(0x7)
#define APROC_SIG_NOTIFY_HOLD_COMPLETE          APROC_SIG_CMD(0x8)
#define APROC_SIG_NOTIFY_SRC                    APROC_SIG_CMD(0x9)
#define APROC_SIG_NOTIFY_MCPU_UPDATE_ROUNTIE    APROC_SIG_CMD(0xA)
#define APROC_SIG_NOTIFY_SKIP_DONE      APROC_SIG_CMD(0xB)
// Idx
#define APROC_SIG_NOTIFY_IDX_KERNEL APROC_SIG_IDX(0x1)
#define APROC_SIG_NOTIFY_IDX_DEC0  APROC_SIG_IDX(0x2)
#define APROC_SIG_NOTIFY_IDX_DEC1 APROC_SIG_IDX(0x3)
#define APROC_SIG_NOTIFY_IDX_DEC2 APROC_SIG_IDX(0x4)
#define APROC_SIG_NOTIFY_IDX_DEC3 APROC_SIG_IDX(0x5) // reserved
#define APROC_SIG_NOTIFY_IDX_ALL APROC_SIG_IDX(0x6)

//---------------------------------------------------------------------------
// Register Map (Sync. with APROC)
// 
/* _to_extend_ */
#if 0
#define APROC_DATAPORT_BASE 0x1F9000 // hard code, fix me!!!!
#define APROC_REGMAP_BASE 0x1FA000 // hard code, fix me!!!!
#endif
#if defined(CC_AUD_ARM_RENDER)
 #if defined (CC_AUD_DISABLE_2ND_DSP) && defined (CC_AUD_ENLARGE_APROC_DRAM)
#define APROC_DATAPORT_BASE 0x39D000 // hard code, refer to aarm.scat
#define APROC_REGMAP_BASE   0x39E000 // hard code, refer to aarm.scat
#define APROC_AQDATA_BASE   0x3A8000 
 #else
#define APROC_DATAPORT_BASE 0xFA000 // hard code, refer to aarm.scat
#define APROC_REGMAP_BASE 0xFB000 // hard code, refer to aarm.scat
#define APROC_AQDATA_BASE 0x104000
 #endif //CC_AUD_DISABLE_2ND_DSP && CC_AUD_ENLARGE_APROC_DRAM
#else
#define APROC_REGMAP_BASE 0xFB000
#endif // CC_AUD_ARM11_SUPPORT

#ifdef CC_AUD_SUPPORT_DBXTV
/** Total Sonics modes for DBXTV_SetMode() */
typedef enum _DBXTV_TotSonMode
{
    TOTSON_ON       = 0,
    TOTSON_OFF
} DBXTV_TotSonMode;

/** Total Volume modes for DBXTV_SetMode() */
typedef enum _DBXTV_TotVolMode
{
    TOTVOL_NORMAL   = 0,
    TOTVOL_NIGHT,
    TOTVOL_OFF
} DBXTV_TotVolMode;

/** Total Surround modes for DBXTV_SetMode() */
typedef enum _DBXTV_TotSurMode
{
    TOTSUR_ON       = 0,
    TOTSUR_OFF
} DBXTV_TotSurMode;


/** Total Sonics modes for DBXTV_SetMode() */
typedef enum _DBXTV_TotSonTestMode
{
    TOTSON_NORMAL       = 0,
    TOTSON_MOVIE,
    TOTSON_NIGHT,
    TOTSON_DRC,
    TOTSON_NUM
} DBXTV_TotSonTestMode;
/** Total Sonics preset length in DM */
#define DBXTV_Param_LENGTH 95
/** Total Sonics preset length in PM */
#define DBXTV_Coeff_LENGTH 100
/* Lengths of presets (in words) */
/* Note if the DM or PM structure is changed, these values will need to be updated */
#define DBX_Param_LENGTH_DVC (54)
#define DBX_Coeff_LENGTH_DVC (10)
#define DBX_Param_LENGTH_DPP (20)
#define DBX_Coeff_LENGTH_DPP (25)
#define DBX_Param_LENGTH_IW (3)
#define DBX_Coeff_LENGTH_IW (5)
#define DBX_Param_LENGTH_EQ (2)
#define DBX_Coeff_LENGTH_EQ (35)
#define DBX_Param_LENGTH_BE (16)
#define DBX_Coeff_LENGTH_BE (25)

/* Offsets of presets (in words) */
/* Note if the DM or PM structure is changed, these values will need to be updated */
#define DBX_Param_OFFSET_DVC (0)
#define DBX_Coeff_OFFSET_DVC (0)
#define DBX_Param_OFFSET_DPP (54)
#define DBX_Coeff_OFFSET_DPP (10)
#define DBX_Param_OFFSET_IW (74)
#define DBX_Coeff_OFFSET_IW (35)
#define DBX_Param_OFFSET_EQ (77)
#define DBX_Coeff_OFFSET_EQ (40)
#define DBX_Param_OFFSET_BE (79)
#define DBX_Coeff_OFFSET_BE (75)

/* Offsets for DVCOnOff, DPPOnOff, IWBypass, EQBypass, BEOnOff (in words) */
/* Note if the DM structure is changed, these values will need to be updated */
#define DBX_Param_OFFSET_DVCOnOff (0)
#define DBX_Param_OFFSET_DPPOnOff (54)
#define DBX_Param_OFFSET_IWBypass (74)
#define DBX_Param_OFFSET_EQBypass (77)
#define DBX_Param_OFFSET_BEOnOff (79)
#define DBX_Param_OFFSET_DVCHiLevel (2)
#define DBX_Param_OFFSET_DVCLoLevel (23)

#define Enable_DVC (1<<0)
#define Enable_DPP (1<<1)
#define Enable_IW (1<<2)
#define Enable_EQ (1<<3)
#define Enable_BE (1<<4)
#define Enable_All (Enable_DVC | Enable_DPP | Enable_IW | Enable_EQ | Enable_BE)

extern void AUD_DBXTV_Enable(BOOL fgEnable);
extern BOOL AUD_DBXTV_GetEnable(void);
extern void AUD_DBXTV_ReInit(void);
extern void AUD_DBXTV_SetMode(DBXTV_TotSonMode TotSonMode, 
    DBXTV_TotVolMode TotVolMode, DBXTV_TotSurMode TotSurMode, UINT32 Enable);
extern void AUD_DBXTV_GetMode(DBXTV_TotSonMode *pTotSonMode,
    DBXTV_TotVolMode *pTotVolMode, DBXTV_TotSurMode *pTotSurMode, UINT32 *pEnable);
extern void AUD_DBXTV_SetTestEnable(BOOL fgEnable);
extern BOOL AUD_DBXTV_GetTestEnable(void);
extern DBXTV_TotSonTestMode AUD_DBXTV_GetTestMode(void);
extern void AUD_DBXTV_SetTestMode(DBXTV_TotSonTestMode TotSonMode, UINT32 Enable);
#endif//CC_AUD_SUPPORT_DBXTV


// all resgiters in word (4 bytes)
#define APROC_REGMAP_REGSIZE 4 // bytes

#define APROC_REGMAP_IO_GRP_SIZE 32
#define APROC_REGMAP_IO_GRP_IDX_MASK (APROC_REGMAP_IO_GRP_SIZE-1)

// for Upload post usage
typedef enum
{
    /* Common, 0x0 ~ 0x100
    */
    //APROC_REG_VERSION = 0, // 0x0 ~ 0x100

    /* IO Input buffer, 0x10 ~ 0x110, 8 elements, total 32 buffers     */   
    // APROC_REG_IOBUF_IN = 0x10,     
    APROC_REG_IOBUF_IN_START = 0x10,   
    APROC_REG_IOBUF_IN_END,    
    APROC_REG_IOBUF_IN_SIZE, // in bytes    
    APROC_REG_IOBUF_IN_SI_BASE,        

    // APROC_REG_IOBUF_OUTPUT = 0x18,     
    APROC_REG_IOBUF_OUT_START = 0x18,    
    APROC_REG_IOBUF_OUT_END,    
    APROC_REG_IOBUF_OUT_SIZE, // in bytes    
    APROC_REG_IOBUF_OUT_SI_BASE,        

    /* Working register, 0x110~ 0x120    */            
    APROC_REG_POSTPROC_WORKING_VOL0 = 0x110,    
    APROC_REG_POSTPROC_WORKING_VOL1,         
    
    /* Setting register, 0x120~0x130     */    
    APROC_REG_POSTPROC_SETTING_VOL0 = 0x120,     
    APROC_REG_POSTPROC_SETTING_VOL1,     

    #ifdef CC_AUD_SUPPORT_DBXTV
    /* DBXTV Setting register, 0x130 ~ 0x1F3*/
    APROC_REG_DBX_PARAM_BASE = 0x130,
    APROC_REG_DBX_COEFF_BASE = APROC_REG_DBX_PARAM_BASE + DBXTV_Param_LENGTH,//0x18F,
    APROC_REG_DBX_END = APROC_REG_DBX_COEFF_BASE + DBXTV_Coeff_LENGTH, //0x1F3, 
    APROC_REG_DBX_ENABLE = APROC_REG_DBX_END + 1, //0x1f4
    APROC_REG_DBX_REINIT = APROC_REG_DBX_ENABLE + 1, //0x1f5
    #endif


    APROC_REGMAP_SIZE = 0x800
} _APROC_REGMAP_T;


// for Aproc Render usage
/* APROC Shared Register (ASM)
    */
typedef enum {
    APROC_ASM_ID_COMM_0 = 0x0, // APROC_ASM_COMM0_T
    APROC_ASM_ID_COMM_1, // APROC_ASM_COMM1_T
    APROC_ASM_ID_COMM_2, // APROC_ASM_COMM2_T
    APROC_ASM_ID_COMM_3, // APROC_ASM_COMM3_T    
    APROC_ASM_ID_AENV_0 = 0x4, // APROC_ASM_AENV0_T
    APROC_ASM_ID_AENV_1, // APROC_ASM_AENV1_T
    APROC_ASM_ID_AENV_2, // APROC_ASM_AENV2_T
    APROC_ASM_ID_AVSYNC_0= 0x8, // APROC_ASM_AVSYNC0_T
    APROC_ASM_ID_IOBUF_0, // APROC_ASM_IOBUF0_T
    APROC_ASM_ID_MIXSND_0, // APROC_ASM_MIXSND0_T
    APROC_ASM_ID_CPU_0,    // APROC_ASM_CPU_T
    APROC_ASM_ID_POSTPROC_0 = 0x10, // APROC_ASM_POSTPROC0_T
    APROC_ASM_ID_POSTPROC_1, // APROC_ASM_POSTPROC1_T
    APROC_ASM_ID_POSTPROC_2, // APROC_ASM_POSTPROC2_T
    APROC_ASM_ID_POSTPROC_3, // APROC_ASM_POSTPROC3_T
    APROC_ASM_ID_POSTPROC_4, // APROC_ASM_POSTPROC4_T
    APROC_ASM_ID_POSTPROC_5, // APROC_ASM_POSTPROC5_T
    APROC_ASM_ID_POSTPROC_6, // APROC_ASM_POSTPROC6_T
    APROC_ASM_ID_POSTPROC_7, // APROC_ASM_POSTPROC7_T
    APROC_ASM_ID_POSTPROC_8, // APROC_ASM_POSTPROC8_T
    APROC_ASM_ID_POSTPROC_9, // APROC_ASM_POSTPROC9_T
    APROC_ASM_ID_POSTPROC_A, // APROC_ASM_POSTPROCA_T
    APROC_ASM_ID_POSTPROC_B, // APROC_ASM_POSTPROCB_T
    APROC_ASM_ID_POSTPROC_C, // APROC_ASM_POSTPROCC_T
    APROC_ASM_ID_POSTPROC_D, // APROC_ASM_POSTPROCD_T
    APROC_ASM_ID_POSTPROC_E, // APROC_ASM_POSTPROCE_T
    APROC_ASM_ID_POSTPROC_F, // APROC_ASM_POSTPROCF_T    
    APROC_ASM_ID_POSTPROC_10, // APROC_ASM_POSTPROC10_T     
    APROC_ASM_ID_SEL_0, //APROC_ASM_ID_SEL0_T
    APROC_ASM_ID_POSTPROC_12, // APROC_ASM_POSTPROC12_T    
    APROC_ASM_ID_3RD_POST_23, 
    APROC_ASM_ID_3RD_POST_24,
    APROC_ASM_ID_3RD_POST_25,
    APROC_ASM_ID_3RD_POST_26,
    APROC_ASM_ID_3RD_POST_27,
    APROC_ASM_ID_3RD_POST_28,
    APROC_ASM_ID_3RD_POST_29,
    APROC_ASM_ID_3RD_POST_2A,
    APROC_ASM_ID_3RD_POST_2B,
    APROC_ASM_ID_3RD_POST_2C,
    APROC_ASM_ID_3RD_POST_2D,
    APROC_ASM_ID_3RD_POST_2E,
    APROC_ASM_ID_3RD_POST_2F,
    APROC_ASM_ID_POSTPROC_20 = 0x30, // APROC_ASM_POSTPROC20_T    
    APROC_ASM_ID_POSTPROC_21, // APROC_ASM_POSTPROC21_T   
    APROC_ASM_ID_HW_MIXSND_0, //APROC_ASM_HW_MIXSND0_T
    APROC_ASM_ID_3RD_POST_33, 
    APROC_ASM_ID_3RD_POST_34, 
    APROC_ASM_ID_MIXSND_1,
    APROC_ASM_ID_EQSPECTRUM, // 0x36
    APROC_ASM_ID_ENC_MP3 = 0x37, // APROC_ASM_ENC_MP3_T
    APROC_ASM_ID_POSTPROC_3B = 0x3B, // APROC_ASM_POSTPROC3B_T
    APROC_ASM_ID_POSTPROC_3C = 0x3C, // APROC_ASM_POSTPROC3B_T
    APROC_ASM_ID_MIXSND_2, // 0x3D: APROC_ASM_MIXSND0_T
    APROC_ASM_ID_MIXSND_3, // 0x3E: APROC_ASM_MIXSND0_T 

// LGSE used ASM --- start ( 0x800 ~ 0x???)
	APROC_ASM_ID_LGSE_0 = 0x40,   // APROC_ASM_LGSE_CTRL_T
	APROC_ASM_ID_LGSE_INIT = 0x41,
	APROC_ASM_ID_LGSE_VAR = 0x55,
	APROC_ASM_ID_LGSE_OUT = 0x60,

    APROC_ASM_ID_NUM = 0x80 /*_extend_to 0x40*/
} APROC_ASM_ID_T;
    
    
#define APROC_REGMAP_SIZE ((APROC_ASM_ID_NUM-1)*APROC_REGMAP_IO_GRP_SIZE)

#ifdef ARM_SUPPORT_SRS_SS
#define APROC_ASM_ID_SRS_SS3D   APROC_ASM_ID_3RD_POST_23    
#define APROC_ASM_ID_SRS_TVOL   APROC_ASM_ID_3RD_POST_24
#define APROC_ASM_ID_SRS_AEQ    APROC_ASM_ID_3RD_POST_24
#define APROC_ASM_ID_SRS_TRUD   APROC_ASM_ID_3RD_POST_24

#define APROC_ASM_ID_SRS_CC3D   APROC_ASM_ID_3RD_POST_25

#define APROC_ASM_ID_SRS_TSHD   APROC_ASM_ID_3RD_POST_26
#define APROC_ASM_ID_SRS_SPHPF  APROC_ASM_ID_3RD_POST_26

#define APROC_ASM_ID_SRS_TBHD   APROC_ASM_ID_3RD_POST_27
#define APROC_ASM_ID_SRS_SRS3D  APROC_ASM_ID_3RD_POST_27

#define APROC_ASM_ID_SRS_CSHP   APROC_ASM_ID_3RD_POST_28

#define APROC_ASM_ID_SRS_HPF    APROC_ASM_ID_3RD_POST_29
#define APROC_ASM_ID_SRS_GEQ    APROC_ASM_ID_3RD_POST_29

#define APROC_ASM_ID_SRS_BASSTREB   APROC_ASM_ID_3RD_POST_2A
#define APROC_ASM_ID_SRS_MVOL   APROC_ASM_ID_3RD_POST_2A
#define APROC_ASM_ID_SRS_HL     APROC_ASM_ID_3RD_POST_2A
#endif

#if 1//def SUPPORT_SAPOST
#define APROC_ASM_ID_SAP_MISC_23   APROC_ASM_ID_3RD_POST_23    //APROC_ASM_SPOST_MISC23_T
#endif

    
typedef enum {
    APROC_REG_VERSION = 0x0,

    APROC_REG_CPUSIDE_BASE, // APROC base address at CPU side
    APROC_REG_IOBUF_BASE,
    APROC_REG_DELAYBUF_BASE,
    APROC_REG_ADSPCMMBUF_BASE,
    APROC_REG_ADSPCMMCMPT_BASE,
    APROC_REG_DELAYBUF_SIZE,
    APROC_REG_AMX2_MODE,
    APROC_REG_POST_BYPASS_FLAG,
    APROC_REG_POST_OUTBUF_SIZE,

    APROC_REG_DSP_MVS_VERSION = 0x18,
    APROC_REG_DSP_MVB_VERSION   
} APROC_ASM_COMM0_T;

typedef enum {
    // Profiling
    APROC_REG_PROFILE_MOS_THREADTBL_BASE = 0x0,
    APROC_REG_PROFILE_MOS_THREADLIST_BASE,
    APROC_REG_PROFILE_MOS_DEVINFO_BASE,
    APROC_REG_PROFILE_IOBUF_BASE,

    // Debug
    APROC_REG_DBG_ROUTINE_BASE = 0x8,
    APROC_REG_DBG_DBGINFO_BASE,//[Debug SOP] 
    APROC_REG_DBG_MULTI_CH,
    APROC_REG_DBG_AVSYNC_INFO,

    // Assert system
    APROC_REG_ASSERTSYS_ADDR = 0x10,
    APROC_REG_ASSERTSYS_DATA0,
    APROC_REG_ASSERTSYS_DATA1,
    APROC_REG_ASSERTSYS_DATA2,

    // analysis
    APROC_REG_LEVEL_DET_FLAG = 0x18,
    APROC_REG_LEVEL_DET_POSMAX_0,
    APROC_REG_LEVEL_DET_RESULT_0,
    APROC_REG_LEVEL_DET_POSMAX_1,
    APROC_REG_LEVEL_DET_RESULT_1
} APROC_ASM_COMM1_T;

typedef enum {
    APROC_REG_BYPASS_DSP = 0x0,
    APROC_REG_AMIXER0_DOWNMIX_ENABLE, 
    APROC_REG_AMIXER2_ADMIX_FLAG, // bit[0]: AD enable flag, bit[1]: Dolby xmx enable flag
    APROC_REG_DSP_TESTTONE_FLAG,
    APROC_REG_AMIXER2_ADMIX_FADE,
    
    APROC_REG_AMIXER2_ADMIX_PAN,
    APROC_REG_AMIXER2_ADDEC_ENABLE,
    // cache meter
    APROC_REG_CACHEMETER_FLAG = 0x8,
    APROC_REG_CACHEMETER_IMISS,
    APROC_REG_CACHEMETER_IPENALTY,
    APROC_REG_CACHEMETER_DMISS,
    APROC_REG_CACHEMETER_DPENALTY,

    APROC_REG_DBG_MUTE_ALL_SWOUT,
    APROC_REG_DBG_RECOVER_A2DSIG_TH,
    APROC_REG_DBG_RECOVER_A2DSIG_COUNT

} APROC_ASM_COMM2_T;

typedef enum {
    APROC_REG_DBG_CMD = 0x0,
    APROC_REG_DBG_ADDR,
    APROC_REG_DBG_DATA,
    APROC_REG_DBG_TIMER0,
    APROC_REG_DBG_AMX0_READ_TIME,
    APROC_REG_DBG_AMX0_READ_TIME_MAX,
    APROC_REG_DBG_AMX1_READ_TIME,
    APROC_REG_DBG_AMX1_READ_TIME_MAX,
    APROC_REG_DBG_AMX2_READ_TIME,      
    APROC_REG_DBG_AMX2_READ_TIME_MAX,

    APROC_REG_DBG_RENDER_IN0 = 0x10,
    APROC_REG_DBG_RENDER_IN1,
    APROC_REG_DBG_RENDER_IN2,
    APROC_REG_DBG_RENDER_OUT,
    APROC_REG_DBG_POST_MHZ,
    APROC_REG_DBG_POST_MHZ_PERIOD,
} APROC_ASM_COMM3_T; 
    
typedef enum {
    APROC_REG_AENV_SRC0_FREQMODE = 0x0,
    APROC_REG_AENV_SRC2_FREQMODE,
    APROC_REG_AENV_SRC0_MUTECOUNT,
    APROC_REG_AENV_SRC1_MUTECOUNT,
    APROC_REG_AENV_SRC2_MUTECOUNT,
    APROC_REG_AENV_SRC0_ADJ_SWITCH,
    APROC_REG_AENV_SRC0_ADJ_OFFSET,
    APROC_REG_AENV_SRC1_ADJ_SWITCH,
    APROC_REG_AENV_SRC1_ADJ_OFFSET,
    APROC_REG_AENV_SRC2_ADJ_SWITCH,
    APROC_REG_AENV_SRC2_ADJ_OFFSET, //10
    APROC_REG_AENV_SRC1_FREQMODE,      
    APROC_REG_AENV_SRC_HALF_PLL_MODE,
    APROC_REG_AENV_SRC_RECOVER_DBG = 0x18
    
} APROC_ASM_AENV0_T;

typedef enum {
    APROC_REG_AENV_IEC_FLAG = 0x0,
    APROC_REG_AENV_IEC_CHCFG0,
    APROC_REG_AENV_IEC_CHCFG1,
    APROC_REG_AENV_IEC_CHCFG2,
    APROC_REG_AENV_IEC_CHCFG3,
    APROC_REG_AENV_IEC_CHCFG4,
    APROC_REG_AENV_IEC_CHCFG5,
    APROC_REG_AENV_IEC_CHCFG6,
    APROC_REG_AENV_IEC_CHCFG7,
    APROC_REG_AENV_IEC_BURSTINFO,
    APROC_REG_AENV_IEC_NSNUM,
    APROC_REG_AENV_IEC_RAWMUTE,
    APROC_REG_AENV_IEC_RAWDELAY,
    APROC_REG_AENV_IEC_RAWUNDERRUN,
    APROC_REG_AENV_IEC_AMX0_VALID,
    APROC_REG_AENV_IEC_AMX1_VALID,
    APROC_REG_AENV_IEC_MODE,
    APROC_REG_AENV_IEC_FRAME,
    APROC_REG_AENV_IEC_OUT,
    APROC_REG_AENV_IEC_OVF,
    APROC_REG_DBG_IEC_TIME,      
    APROC_REG_DBG_IEC_TIME_MAX
} APROC_ASM_AENV1_T;
    
typedef enum {
    APROC_REG_AENV_AOUT_APUT = 0x0,
    APROC_REG_AENV_AOUT_AOUT,
    APROC_REG_AENV_AOUT_UNDERRUN,
    APROC_REG_AENV_AOUT_AMX0_BANK,
    APROC_REG_AENV_AOUT_AMX1_BANK,
    APROC_REG_AENV_AOUT_AMX2_BANK,
    APROC_REG_DBG_AOUT_TIME,      
    APROC_REG_DBG_AOUT_TIME_MAX
} APROC_ASM_AENV2_T;

typedef enum {
    APROC_REG_AVSYNC_CMD_SRC0 = 0x0,
    APROC_REG_AVSYNC_CMD_SRC1,
    APROC_REG_AVSYNC_CMD_SRC2,
    APROC_REG_STC_SPEC_SRC0,   // 3   //STC Spec -- 0: 90K  1: 45K
    APROC_REG_STC_SPEC_SRC1,
    APROC_REG_STC_SPEC_SRC2,
    APROC_REG_APTS_SRC0,       // 6   //PTS
    APROC_REG_APTS_SRC1,
    APROC_REG_APTS_SRC2,
    APROC_REG_APTS_APUT_BANK0,  // 9   //aput_bank with the APTS from DSP
    APROC_REG_APTS_APUT_BANK1, 
    APROC_REG_APTS_APUT_BANK2,
    APROC_REG_APTS_FRM_SZ_SRC0, //12  //APTS_FRM_SIZE,  Frame Size in samples
    APROC_REG_APTS_FRM_SZ_SRC1, 
    APROC_REG_APTS_FRM_SZ_SRC2, 
    APROC_REG_SKIP_FRMNUM_SRC0, //15  //Skip Frame Number
    APROC_REG_SKIP_FRMNUM_SRC1,
    APROC_REG_SKIP_FRMNUM_SRC2,    
        //Reserved for debug
        //bit0 : 0-SRC0 is systerm master  1-audio master
        //bit1 : 0-SRC1 is systerm master  1-audio master
        //bit2 : 0-SRC2 is systerm master  1-audio master    
    APROC_REG_AVSYNC_RESERVED,      // 18, 0x12  
    APROC_REG_PTS_QRY_RESULT_ADDR,  // 19, 0x13
    APROC_REG_PTS_TABLE_ADDR,       // 20, 0x14
    APROC_REG_PTS_TABLE_SIZE,       // 21, 0x15
    APROC_REG_AFIFO0_ADDR,          // 22, 0x14
    APROC_REG_AFIFO1_ADDR,          // 23, 0x14     
    
    APROC_REG_AVSYNC_NOTIFY_APTS_SRC0 = 0x18, // 24, 0x18
    APROC_REG_AVSYNC_NOTIFY_APTS_SRC1,
    APROC_REG_AVSYNC_NOTIFY_APTS_SRC2,
    APROC_REG_AVSYNC_NOTIFY_HOLD_OK_SRC0,
    APROC_REG_AVSYNC_NOTIFY_HOLD_OK_SRC1,
    APROC_REG_AVSYNC_NOTIFY_HOLD_OK_SRC2,
    APROC_REG_AFIFO2_ADDR               // 30, 0x1E     
} APROC_ASM_AVSYNC0_T;

typedef enum {
    APROC_REG_IOBUF_DEC0_START = 0x0,
    APROC_REG_IOBUF_DEC0_END,
    APROC_REG_IOBUF_DEC0_SIZE, // bytes
    APROC_REG_IOBUF_DEC0_CH,
    APROC_REG_IOBUF_DEC0_SI_BASE,
    
    APROC_REG_IOBUF_DEC1_START = 0x8,
    APROC_REG_IOBUF_DEC1_END,
    APROC_REG_IOBUF_DEC1_SIZE, // bytes
    APROC_REG_IOBUF_DEC1_CH,
    APROC_REG_IOBUF_DEC1_SI_BASE,
    
    APROC_REG_IOBUF_DEC2_START = 0x10,
    APROC_REG_IOBUF_DEC2_END,
    APROC_REG_IOBUF_DEC2_SIZE, // bytes
    APROC_REG_IOBUF_DEC2_CH,
    APROC_REG_IOBUF_DEC2_SI_BASE,

    APROC_REG_IOBUF_SWDEC0_START,
    APROC_REG_IOBUF_SWDEC0_END,
    APROC_REG_IOBUF_SWDEC0_SIZE,
    APROC_REG_IOBUF_SWDEC0_CH,
    APROC_REG_IOBUF_SWDEC0_SI_BASE        
} APROC_ASM_IOBUF0_T;

typedef enum {
    APROC_REG_IOBUF_MIXSND_INPUT0_START = 0x0,   //parson mixsnd
    APROC_REG_IOBUF_MIXSND_INPUT0_END,
    APROC_REG_IOBUF_MIXSND_INPUT0_SIZE,            // in bytes
    APROC_REG_IOBUF_MIXSND_INPUT0_CH,
    APROC_REG_IOBUF_MIXSND_INPUT0_ENABLE,
    APROC_REG_IOBUF_MIXSND_INPUT0_STATUS,
    APROC_REG_IOBUF_MIXSND_INPUT0_IN_UNDERRUN,    

    APROC_REG_IOBUF_MIXSND_INPUT1_START = 0x8,   //parson mixsnd
    APROC_REG_IOBUF_MIXSND_INPUT1_END,
    APROC_REG_IOBUF_MIXSND_INPUT1_SIZE,            // in bytes
    APROC_REG_IOBUF_MIXSND_INPUT1_CH,
    APROC_REG_IOBUF_MIXSND_INPUT1_ENABLE,
    APROC_REG_IOBUF_MIXSND_INPUT1_STATUS,
    APROC_REG_IOBUF_MIXSND_INPUT1_IN_UNDERRUN,    

    APROC_REG_MIXSND0_IN_TRIM0 = 0x10,
    APROC_REG_MIXSND0_IN_TRIM1,
    APROC_REG_MIXSND1_IN_TRIM0,
    APROC_REG_MIXSND1_IN_TRIM1,
    APROC_REG_INPUT_MIX0_TRIM,
    APROC_REG_INPUT_MIX1_TRIM
} APROC_ASM_MIXSND0_T;

typedef enum {
    APROC_REG_IOBUF_MIXSND_INPUT2_START = 0x0,   //
    APROC_REG_IOBUF_MIXSND_INPUT2_END,
    APROC_REG_IOBUF_MIXSND_INPUT2_SIZE,            // 
    APROC_REG_IOBUF_MIXSND_INPUT2_CH,
    APROC_REG_IOBUF_MIXSND_INPUT2_ENABLE,
    APROC_REG_IOBUF_MIXSND_INPUT2_STATUS,
    APROC_REG_IOBUF_MIXSND_INPUT2_IN_UNDERRUN,    

    APROC_REG_IOBUF_MIXSND_INPUT3_START = 0x8,   //
    APROC_REG_IOBUF_MIXSND_INPUT3_END,
    APROC_REG_IOBUF_MIXSND_INPUT3_SIZE,            // 
    APROC_REG_IOBUF_MIXSND_INPUT3_CH,
    APROC_REG_IOBUF_MIXSND_INPUT3_ENABLE,
    APROC_REG_IOBUF_MIXSND_INPUT3_STATUS,
    APROC_REG_IOBUF_MIXSND_INPUT3_IN_UNDERRUN,    

    APROC_REG_MIXSND2_IN_TRIM0 = 0x10,
    APROC_REG_MIXSND2_IN_TRIM1,
    APROC_REG_MIXSND3_IN_TRIM0,
    APROC_REG_MIXSND3_IN_TRIM1,
    APROC_REG_INPUT_MIX2_TRIM,
    APROC_REG_INPUT_MIX3_TRIM
} APROC_ASM_MIXSND1_T;

typedef enum {
    APROC_REG_IOBUF_MIXSND_INPUT4_START = 0x0,   //
    APROC_REG_IOBUF_MIXSND_INPUT4_END,
    APROC_REG_IOBUF_MIXSND_INPUT4_SIZE,            // 
    APROC_REG_IOBUF_MIXSND_INPUT4_CH,
    APROC_REG_IOBUF_MIXSND_INPUT4_ENABLE,
    APROC_REG_IOBUF_MIXSND_INPUT4_STATUS,
    APROC_REG_IOBUF_MIXSND_INPUT4_IN_UNDERRUN,

    APROC_REG_IOBUF_MIXSND_INPUT5_START = 0x8,   //
    APROC_REG_IOBUF_MIXSND_INPUT5_END,
    APROC_REG_IOBUF_MIXSND_INPUT5_SIZE,            // 
    APROC_REG_IOBUF_MIXSND_INPUT5_CH,
    APROC_REG_IOBUF_MIXSND_INPUT5_ENABLE,
    APROC_REG_IOBUF_MIXSND_INPUT5_STATUS,
    APROC_REG_IOBUF_MIXSND_INPUT5_IN_UNDERRUN,    

    APROC_REG_MIXSND4_IN_TRIM0 = 0x10,
    APROC_REG_MIXSND4_IN_TRIM1,
    APROC_REG_MIXSND5_IN_TRIM0,
    APROC_REG_MIXSND5_IN_TRIM1,
    APROC_REG_INPUT_MIX4_TRIM,
    APROC_REG_INPUT_MIX5_TRIM
} APROC_ASM_MIXSND2_T;

typedef enum {
    APROC_REG_IOBUF_MIXSND_INPUT6_START = 0x0,   //
    APROC_REG_IOBUF_MIXSND_INPUT6_END,
    APROC_REG_IOBUF_MIXSND_INPUT6_SIZE,            // 
    APROC_REG_IOBUF_MIXSND_INPUT6_CH,
    APROC_REG_IOBUF_MIXSND_INPUT6_ENABLE,
    APROC_REG_IOBUF_MIXSND_INPUT6_STATUS,
    APROC_REG_IOBUF_MIXSND_INPUT6_IN_UNDERRUN,    

    APROC_REG_IOBUF_MIXSND_INPUT7_START = 0x8,   //
    APROC_REG_IOBUF_MIXSND_INPUT7_END,
    APROC_REG_IOBUF_MIXSND_INPUT7_SIZE,            // 
    APROC_REG_IOBUF_MIXSND_INPUT7_CH,
    APROC_REG_IOBUF_MIXSND_INPUT7_ENABLE,
    APROC_REG_IOBUF_MIXSND_INPUT7_STATUS,
    APROC_REG_IOBUF_MIXSND_INPUT7_IN_UNDERRUN,    

    APROC_REG_MIXSND6_IN_TRIM0 = 0x10,
    APROC_REG_MIXSND6_IN_TRIM1,
    APROC_REG_MIXSND7_IN_TRIM0,
    APROC_REG_MIXSND7_IN_TRIM1,
    APROC_REG_INPUT_MIX6_TRIM,
    APROC_REG_INPUT_MIX7_TRIM
} APROC_ASM_MIXSND3_T;

typedef enum {
    APROC_REG_IOBUF_HW_MIXSND_INPUT0_START = 0x0,   //HW mixsnd
    APROC_REG_IOBUF_HW_MIXSND_INPUT0_END,
    APROC_REG_IOBUF_HW_MIXSND_INPUT0_SIZE,            // in bytes
    APROC_REG_IOBUF_HW_MIXSND_INPUT0_CH,
    APROC_REG_IOBUF_HW_MIXSND_INPUT0_ENABLE,
    APROC_REG_IOBUF_HW_MIXSND_INPUT0_STATUS,
    APROC_REG_IOBUF_HW_MIXSND_INPUT0_IN_UNDERRUN,

    APROC_REG_IOBUF_HW_MIXSND_INPUT1_START = 0x8,   //HW mixsnd
    APROC_REG_IOBUF_HW_MIXSND_INPUT1_END,
    APROC_REG_IOBUF_HW_MIXSND_INPUT1_SIZE,            // in bytes
    APROC_REG_IOBUF_HW_MIXSND_INPUT1_CH,
    APROC_REG_IOBUF_HW_MIXSND_INPUT1_ENABLE,
    APROC_REG_IOBUF_HW_MIXSND_INPUT1_STATUS,
    APROC_REG_IOBUF_HW_MIXSND_INPUT1_IN_UNDERRUN,

    APROC_REG_HW_MIXSND0_IN_TRIM = 0x10,
    APROC_REG_HW_MIXSND1_IN_TRIM,
    APROC_REG_HW_MIXSND0_CH_CFG,
} APROC_ASM_HW_MIXSND0_T;

typedef enum {
    APROC_REG_IOBUF_UPLOAD0_START = 0x0,   //parson upload
    APROC_REG_IOBUF_UPLOAD0_END,
    APROC_REG_IOBUF_UPLOAD0_SIZE,            // in bytes
    APROC_REG_IOBUF_UPLOAD0_CH,
    APROC_REG_IOBUF_UPLOAD0_ENABLE,

    APROC_REG_IOBUF_RISCPOST_START,        //parson riscpost
    APROC_REG_IOBUF_RISCPOST_END,
    APROC_REG_IOBUF_RISCPOST_SIZE,            // in bytes
    APROC_REG_IOBUF_RISCPOST_CH,
    APROC_REG_IOBUF_RISCPOST_ENABLE,

    APROC_REG_IOBUF_UPLOAD1_START = 0x10,
    APROC_REG_IOBUF_UPLOAD1_END,
    APROC_REG_IOBUF_UPLOAD1_SIZE,            // in bytes
    APROC_REG_IOBUF_UPLOAD1_CH,
    APROC_REG_IOBUF_UPLOAD1_ENABLE,

    APROC_REG_IOBUF_UPLOAD2_START,
    APROC_REG_IOBUF_UPLOAD2_END,
    APROC_REG_IOBUF_UPLOAD2_SIZE,            // in bytes
    APROC_REG_IOBUF_UPLOAD2_CH,
    APROC_REG_IOBUF_UPLOAD2_ENABLE,

    APROC_REG_IOBUF_UPLOAD0_UPDATE_COUNT,
    APROC_REG_IOBUF_UPLOAD1_UPDATE_COUNT,
    APROC_REG_IOBUF_UPLOAD2_UPDATE_COUNT,

    APROC_REG_IOBUF_UPLOAD0_UPDATE_COUNT_CUR,
    APROC_REG_IOBUF_UPLOAD1_UPDATE_COUNT_CUR,
    APROC_REG_IOBUF_UPLOAD2_UPDATE_COUNT_CUR,
} APROC_ASM_CPU_T;

typedef enum {
    // Trims
    APROC_REG_INPUT0_TRIM = 0x0,
    APROC_REG_INPUT1_TRIM,
    APROC_REG_INPUT2_TRIM,
    APROC_REG_SP_TRIM,
    APROC_REG_SP_LSRS_TRIM,
    APROC_REG_SPSW_TRIM,
    APROC_REG_HP_TRIM,    // 6
    APROC_REG_MON_TRIM,
    APROC_REG_IEC_TRIM,
    APROC_REG_SCART_TRIM, // 9
    APROC_REG_BT_TRIM,
    APROC_REG_AMIXER0_TRIM,
    APROC_REG_AMIXER1_TRIM,
    APROC_REG_AMIXER2_TRIM,
    APROC_REG_DELAY0_TRIM,
    APROC_REG_DELAY1_TRIM,
    APROC_REG_DELAY2_TRIM, // 0x10

    // Volume
    APROC_REG_SP_VOL,
    APROC_REG_SP_LSRS_VOL,
    APROC_REG_SPSW_VOL,
    APROC_REG_HP_VOL,
    APROC_REG_MON_VOL,
    APROC_REG_IEC_VOL,
    APROC_REG_SCART_VOL,
    APROC_REG_BT_VOL,
    APROC_REG_AMIXER0_VOL, // 0x19
    APROC_REG_AMIXER1_VOL,
    APROC_REG_AMIXER2_VOL,

    APROC_REG_DSP_TRIMVOL_PERIOD,
    APROC_REG_AMIXER_TRIMVOL_PERIOD
} APROC_ASM_POSTPROC0_T;

typedef enum {
    // Balancer
    APROC_REG_SP_BALANCE_0 = 0x0,
    APROC_REG_SP_BALANCE_1,
    APROC_REG_HP_BALANCE_0,
    APROC_REG_HP_BALANCE_1,

    APROC_REG_BALANCE_PERIOD,

    // Inverter
    APROC_REG_SP_INV = 0x8,
    APROC_REG_SP_LSRS_INV,
    APROC_REG_SPSW_INV,
    // SPSW mono mix
    APROC_REG_SPSW_MONOMIX_FLAG,
    
    APROC_REG_DELAY3_TRIM = 0x10,
    APROC_REG_DELAY4_TRIM,
    APROC_REG_DELAY5_TRIM, 
    APROC_REG_DELAY6_TRIM, 
    APROC_REG_SNPT_TRIM, 
    
    APROC_REG_LSRS_BALANCE_0 = 0x16,
    APROC_REG_LSRS_BALANCE_1,
    APROC_REG_IEC_BALANCE_0,
    APROC_REG_IEC_BALANCE_1,
    APROC_REG_INPUT0_BALANCE_L,
    APROC_REG_INPUT0_BALANCE_R,
    APROC_REG_INPUT1_BALANCE_L,
    APROC_REG_INPUT1_BALANCE_R,        
    APROC_REG_INPUT2_BALANCE_L,
    APROC_REG_INPUT2_BALANCE_R 
} APROC_ASM_POSTPROC1_T;

typedef enum {
    // AVC SP band 0
    APROC_REG_AVC_SP_TARGET_0 = 0x0,
    APROC_REG_AVC_SP_ATTACK_0,
    APROC_REG_AVC_SP_RELEASE_0,
    APROC_REG_AVC_SP_RATIO_0,
    APROC_REG_AVC_SP_SILENCE_0,
    APROC_REG_AVC_SP_MAX_EXPAND_0,
    APROC_REG_AVC_SP_POST_GAIN_0,
    APROC_REG_AVC_SP_HOLD_0,

    // AVC SP band 1
    APROC_REG_AVC_SP_TARGET_1 = 0x8,
    APROC_REG_AVC_SP_ATTACK_1,
    APROC_REG_AVC_SP_RELEASE_1,
    APROC_REG_AVC_SP_RATIO_1,
    APROC_REG_AVC_SP_SILENCE_1,
    APROC_REG_AVC_SP_MAX_EXPAND_1,
    APROC_REG_AVC_SP_POST_GAIN_1,
    APROC_REG_AVC_SP_HOLD_1,

    // AVC SP band 2
    APROC_REG_AVC_SP_TARGET_2 = 0x10,
    APROC_REG_AVC_SP_ATTACK_2,
    APROC_REG_AVC_SP_RELEASE_2,
    APROC_REG_AVC_SP_RATIO_2,
    APROC_REG_AVC_SP_SILENCE_2,
    APROC_REG_AVC_SP_MAX_EXPAND_2,
    APROC_REG_AVC_SP_POST_GAIN_2,
    APROC_REG_AVC_SP_HOLD_2,

    // AVC HP band 0
    APROC_REG_AVC_HP_TARGET_0 = 0x18,
    APROC_REG_AVC_HP_ATTACK_0,
    APROC_REG_AVC_HP_RELEASE_0,
    APROC_REG_AVC_HP_RATIO_0,
    APROC_REG_AVC_HP_SILENCE_0,
    APROC_REG_AVC_HP_MAX_EXPAND_0,
    APROC_REG_AVC_HP_POST_GAIN_0,
    APROC_REG_AVC_HP_HOLD_0    
} APROC_ASM_POSTPROC2_T;

typedef enum {
    // AVC SP band 0
    APROC_REG_BIQ_SP_AVC_0_TYPE_0 = 0x0,
    APROC_REG_BIQ_SP_AVC_0_FC_0,
    APROC_REG_BIQ_SP_AVC_0_Q_0,
    APROC_REG_BIQ_SP_AVC_0_GAIN_0,
    APROC_REG_BIQ_SP_AVC_0_TYPE_1,
    APROC_REG_BIQ_SP_AVC_0_FC_1,
    APROC_REG_BIQ_SP_AVC_0_Q_1,
    APROC_REG_BIQ_SP_AVC_0_GAIN_1,

    // AVC SP band 1
    APROC_REG_BIQ_SP_AVC_1_TYPE_0 = 0x8,
    APROC_REG_BIQ_SP_AVC_1_FC_0,
    APROC_REG_BIQ_SP_AVC_1_Q_0,
    APROC_REG_BIQ_SP_AVC_1_GAIN_0,
    APROC_REG_BIQ_SP_AVC_1_TYPE_1,
    APROC_REG_BIQ_SP_AVC_1_FC_1,
    APROC_REG_BIQ_SP_AVC_1_Q_1,
    APROC_REG_BIQ_SP_AVC_1_GAIN_1,

    // AVC SP band 2
    APROC_REG_BIQ_SP_AVC_2_TYPE_0 = 0x10,
    APROC_REG_BIQ_SP_AVC_2_FC_0,
    APROC_REG_BIQ_SP_AVC_2_Q_0,
    APROC_REG_BIQ_SP_AVC_2_GAIN_0,
    APROC_REG_BIQ_SP_AVC_2_TYPE_1,
    APROC_REG_BIQ_SP_AVC_2_FC_1,
    APROC_REG_BIQ_SP_AVC_2_Q_1,
    APROC_REG_BIQ_SP_AVC_2_GAIN_1,
    
    // AVC HP band 0
    APROC_REG_BIQ_HP_AVC_0_TYPE_0 = 0x18,
    APROC_REG_BIQ_HP_AVC_0_FC_0,
    APROC_REG_BIQ_HP_AVC_0_Q_0,
    APROC_REG_BIQ_HP_AVC_0_GAIN_0,
    APROC_REG_BIQ_HP_AVC_0_TYPE_1,
    APROC_REG_BIQ_HP_AVC_0_FC_1,
    APROC_REG_BIQ_HP_AVC_0_Q_1,
    APROC_REG_BIQ_HP_AVC_0_GAIN_1,


} APROC_ASM_POSTPROC3_T;

typedef enum {
    // Delay
    APROC_REG_DELAY_SP = 0x0,
    APROC_REG_DELAY_HP,
    APROC_REG_DELAY_SPDIF_PCM,   
    APROC_REG_DELAY_SP_DBG,
    APROC_REG_DELAY_HP_DBG,
    APROC_REG_DELAY_SPDIF_PCM_DBG,
    APROC_REG_DELAY_SP_LSRS,
    APROC_REG_DELAY_AUDIO_OUT,
    APROC_REG_MDELAY_SPDIF_PCM,
    
    // Downmix
    APROC_REG_DMIX_AMIXER0_FLAG = 0xA, // 0xA
    APROC_REG_DMIX_AMIXER1_FLAG,
    APROC_REG_DMIX_AMIXER2_FLAG,
    APROC_REG_DMIX_AMIXER0_WAIT,
    APROC_REG_DMIX_AMIXER1_WAIT,
    APROC_REG_DMIX_AMIXER2_WAIT,
    APROC_REG_DMIX_AMIXER0_INFO, //0x10
    APROC_REG_DMIX_AMIXER1_INFO,
    APROC_REG_DMIX_AMIXER2_INFO
} APROC_ASM_POSTPROC4_T;


typedef enum {
    // SP Bass
    APROC_REG_BIQ_SP_BASS_TYPE = 0x0,
    APROC_REG_BIQ_SP_BASS_FC,
    APROC_REG_BIQ_SP_BASS_Q,
    APROC_REG_BIQ_SP_BASS_GAIN,

    // SP Treble
    APROC_REG_BIQ_SP_TREBLE_TYPE,
    APROC_REG_BIQ_SP_TREBLE_FC,
    APROC_REG_BIQ_SP_TREBLE_Q,
    APROC_REG_BIQ_SP_TREBLE_GAIN,

    // HP Bass
    APROC_REG_BIQ_HP_BASS_TYPE,
    APROC_REG_BIQ_HP_BASS_FC,
    APROC_REG_BIQ_HP_BASS_Q,
    APROC_REG_BIQ_HP_BASS_GAIN,

    // HP Treble
    APROC_REG_BIQ_HP_TREBLE_TYPE,
    APROC_REG_BIQ_HP_TREBLE_FC,
    APROC_REG_BIQ_HP_TREBLE_Q,
    APROC_REG_BIQ_HP_TREBLE_GAIN,

    // SP_LSRS Bass
    APROC_REG_BIQ_SP_LSRS_BASS_TYPE,
    APROC_REG_BIQ_SP_LSRS_BASS_FC,
    APROC_REG_BIQ_SP_LSRS_BASS_Q,
    APROC_REG_BIQ_SP_LSRS_BASS_GAIN,
    
    // SP_LSRS Treble
    APROC_REG_BIQ_SP_LSRS_TREBLE_TYPE,
    APROC_REG_BIQ_SP_LSRS_TREBLE_FC,
    APROC_REG_BIQ_SP_LSRS_TREBLE_Q,
    APROC_REG_BIQ_SP_LSRS_TREBLE_GAIN,
    
    APROC_REG_BIQ_SP_BASSTREBLE_FLAG = 0x18,
    APROC_REG_BIQ_HP_BASSTREBLE_FLAG,
    APROC_REG_BIQ_SP_BASSTREBLE_STEP,
    APROC_REG_BIQ_HP_BASSTREBLE_STEP,
    APROC_REG_BIQ_SP_LSRS_BASSTREBLE_FLAG,
    APROC_REG_BIQ_SP_LSRS_BASSTREBLE_STEP
} APROC_ASM_POSTPROC5_T;

typedef enum {
    // SP HPF
    APROC_REG_BIQ_SP_HPF_0_TYPE = 0x0,
    APROC_REG_BIQ_SP_HPF_0_FC,
    APROC_REG_BIQ_SP_HPF_0_Q,
    APROC_REG_BIQ_SP_HPF_0_GAIN,
    APROC_REG_BIQ_SP_HPF_1_TYPE,
    APROC_REG_BIQ_SP_HPF_1_FC,
    APROC_REG_BIQ_SP_HPF_1_Q,
    APROC_REG_BIQ_SP_HPF_1_GAIN,

    // SP LSRS HPF
    APROC_REG_BIQ_SP_LSRS_HPF_0_TYPE,
    APROC_REG_BIQ_SP_LSRS_HPF_0_FC,
    APROC_REG_BIQ_SP_LSRS_HPF_0_Q,
    APROC_REG_BIQ_SP_LSRS_HPF_0_GAIN,
    APROC_REG_BIQ_SP_LSRS_HPF_1_TYPE,
    APROC_REG_BIQ_SP_LSRS_HPF_1_FC,
    APROC_REG_BIQ_SP_LSRS_HPF_1_Q,
    APROC_REG_BIQ_SP_LSRS_HPF_1_GAIN,    

    // SPSW LPF
    APROC_REG_BIQ_SPSW_LPF_0_TYPE,
    APROC_REG_BIQ_SPSW_LPF_0_FC,
    APROC_REG_BIQ_SPSW_LPF_0_Q,
    APROC_REG_BIQ_SPSW_LPF_0_GAIN,
    APROC_REG_BIQ_SPSW_LPF_1_TYPE,
    APROC_REG_BIQ_SPSW_LPF_1_FC,
    APROC_REG_BIQ_SPSW_LPF_1_Q,
    APROC_REG_BIQ_SPSW_LPF_1_GAIN,

    APROC_REG_BIQ_SP_HPF_FLAG = 0x18,
    APROC_REG_BIQ_SP_LSRS_HPF_FLAG,
    APROC_REG_BIQ_SPSW_LPF_FLAG
} APROC_ASM_POSTPROC6_T;

typedef enum {
    // band 0
    APROC_REG_BIQ_SP_EQ_0_TYPE = 0x0,
    APROC_REG_BIQ_SP_EQ_0_FC,
    APROC_REG_BIQ_SP_EQ_0_Q,
    APROC_REG_BIQ_SP_EQ_0_GAIN,

    // band 1
    APROC_REG_BIQ_SP_EQ_1_TYPE,
    APROC_REG_BIQ_SP_EQ_1_FC,
    APROC_REG_BIQ_SP_EQ_1_Q,
    APROC_REG_BIQ_SP_EQ_1_GAIN,

    // band 2
    APROC_REG_BIQ_SP_EQ_2_TYPE,
    APROC_REG_BIQ_SP_EQ_2_FC,
    APROC_REG_BIQ_SP_EQ_2_Q,
    APROC_REG_BIQ_SP_EQ_2_GAIN,

    // band 3
    APROC_REG_BIQ_SP_EQ_3_TYPE,
    APROC_REG_BIQ_SP_EQ_3_FC,
    APROC_REG_BIQ_SP_EQ_3_Q,
    APROC_REG_BIQ_SP_EQ_3_GAIN,

    // band 4
    APROC_REG_BIQ_SP_EQ_4_TYPE,
    APROC_REG_BIQ_SP_EQ_4_FC,
    APROC_REG_BIQ_SP_EQ_4_Q,
    APROC_REG_BIQ_SP_EQ_4_GAIN,

    // band 5
    APROC_REG_BIQ_SP_EQ_5_TYPE,
    APROC_REG_BIQ_SP_EQ_5_FC,
    APROC_REG_BIQ_SP_EQ_5_Q,
    APROC_REG_BIQ_SP_EQ_5_GAIN,

    // band 6
    APROC_REG_BIQ_SP_EQ_6_TYPE,
    APROC_REG_BIQ_SP_EQ_6_FC,
    APROC_REG_BIQ_SP_EQ_6_Q,
    APROC_REG_BIQ_SP_EQ_6_GAIN,

    // band 7
    APROC_REG_BIQ_SP_EQ_7_TYPE,
    APROC_REG_BIQ_SP_EQ_7_FC,
    APROC_REG_BIQ_SP_EQ_7_Q,
    APROC_REG_BIQ_SP_EQ_7_GAIN
} APROC_ASM_POSTPROC7_T;

typedef enum {
    // DC cut SRC0
    APROC_REG_BIQ_DCCUT_SRC0_0_TYPE = 0x0,
    APROC_REG_BIQ_DCCUT_SRC0_0_FC,
    APROC_REG_BIQ_DCCUT_SRC0_0_Q,
    APROC_REG_BIQ_DCCUT_SRC0_0_GAIN,
    APROC_REG_BIQ_DCCUT_SRC0_1_TYPE,
    APROC_REG_BIQ_DCCUT_SRC0_1_FC,
    APROC_REG_BIQ_DCCUT_SRC0_1_Q,
    APROC_REG_BIQ_DCCUT_SRC0_1_GAIN,

    // DC cut SRC1
    APROC_REG_BIQ_DCCUT_SRC1_0_TYPE,
    APROC_REG_BIQ_DCCUT_SRC1_0_FC,
    APROC_REG_BIQ_DCCUT_SRC1_0_Q,
    APROC_REG_BIQ_DCCUT_SRC1_0_GAIN,
    APROC_REG_BIQ_DCCUT_SRC1_1_TYPE,
    APROC_REG_BIQ_DCCUT_SRC1_1_FC,
    APROC_REG_BIQ_DCCUT_SRC1_1_Q,
    APROC_REG_BIQ_DCCUT_SRC1_1_GAIN,

    // DC cut SRC2
    APROC_REG_BIQ_DCCUT_SRC2_0_TYPE,
    APROC_REG_BIQ_DCCUT_SRC2_0_FC,
    APROC_REG_BIQ_DCCUT_SRC2_0_Q,
    APROC_REG_BIQ_DCCUT_SRC2_0_GAIN,
    APROC_REG_BIQ_DCCUT_SRC2_1_TYPE,
    APROC_REG_BIQ_DCCUT_SRC2_1_FC,
    APROC_REG_BIQ_DCCUT_SRC2_1_Q,
    APROC_REG_BIQ_DCCUT_SRC2_1_GAIN,

    APROC_REG_BIQ_DCCUT_SRC0_FLAG = 0x18,
    APROC_REG_BIQ_DCCUT_SRC1_FLAG,
    APROC_REG_BIQ_DCCUT_SRC2_FLAG
} APROC_ASM_POSTPROC8_T;

typedef enum {
    // DC cut Mix0
    APROC_REG_BIQ_DCCUT_MIX0_0_TYPE = 0x0,
    APROC_REG_BIQ_DCCUT_MIX0_0_FC,
    APROC_REG_BIQ_DCCUT_MIX0_0_Q,
    APROC_REG_BIQ_DCCUT_MIX0_0_GAIN,
    APROC_REG_BIQ_DCCUT_MIX0_1_TYPE,
    APROC_REG_BIQ_DCCUT_MIX0_1_FC,
    APROC_REG_BIQ_DCCUT_MIX0_1_Q,
    APROC_REG_BIQ_DCCUT_MIX0_1_GAIN,
    
    // DC cut Mix1
    APROC_REG_BIQ_DCCUT_MIX1_0_TYPE,
    APROC_REG_BIQ_DCCUT_MIX1_0_FC,
    APROC_REG_BIQ_DCCUT_MIX1_0_Q,
    APROC_REG_BIQ_DCCUT_MIX1_0_GAIN,
    APROC_REG_BIQ_DCCUT_MIX1_1_TYPE,
    APROC_REG_BIQ_DCCUT_MIX1_1_FC,
    APROC_REG_BIQ_DCCUT_MIX1_1_Q,
    APROC_REG_BIQ_DCCUT_MIX1_1_GAIN,

    APROC_REG_BIQ_DCCUT_MIX0_FLAG = 0x18,
    APROC_REG_BIQ_DCCUT_MIX1_FLAG,
} APROC_ASM_POSTPROC9_T;

typedef enum {
    // MUTE
    APROC_REG_MUTE_AMIXER0_FLAG = 0x0,
    APROC_REG_MUTE_AMIXER0_PERIOD,
    APROC_REG_MUTE_AMIXER1_FLAG,
    APROC_REG_MUTE_AMIXER1_PERIOD,
    APROC_REG_MUTE_AMIXER2_FLAG,
    APROC_REG_MUTE_AMIXER2_PERIOD,

           
    // AVC
    APROC_REG_SP_AVC_FLAG,    
    APROC_REG_SP_AVC_DET_PERIOD,
    APROC_REG_SP_LSRS_AVC_FLAG,
    APROC_REG_SP_LSRS_AVC_DET_PERIOD,
    APROC_REG_HP_AVC_FLAG,  
    APROC_REG_HP_AVC_DET_PERIOD,
    APROC_REG_AVC_MODE,
    APROC_REG_AVC_SGAIN_PERIOD,    
    APROC_REG_SP_AVC_TYPE,
    APROC_REG_HP_AVC_TYPE,
    APROC_REG_SP_LSRS_AVC_TYPE,
    

} APROC_ASM_POSTPROCA_T;

typedef enum {
    // MVS
    APROC_REG_MVS_SP_CLARITY = 0x0,
    APROC_REG_MVS_SP_WIDTH,
    APROC_REG_MVS_SP_LR_GAIN,
    APROC_REG_MVS_SP_CROSSTALK,
    APROC_REG_MVS_SP_OUTPUT_GAIN,
    APROC_REG_MVS_SP_BASS_GAIN,
    APROC_REG_MVS_SP_FO,

    // VB
    APROC_REG_MVB_SPSW_OUTPUT_GAIN = 0x8,
    APROC_REG_MVB_SPSW_BASS_GAIN,
    APROC_REG_MVB_SPSW_FO,
    APROC_REG_MVB_SPSW_ATTAK,
    APROC_REG_MVB_SPSW_RELEASE,
    APROC_REG_MVB_SPSW_TH,
    APROC_REG_MVB_SPSW_DET_GAIN,

    APROC_REG_MVS_SP_FLAG = 0x18,
    APROC_REG_MVB_SPSW_FLAG,

} APROC_ASM_POSTPROCB_T;

typedef struct {
    BOOL fgMVSFlag;
    INT32 i4CrossTalkGain;
    INT32 i4ClarityGain;
    INT32 i4LRGain;
    INT32 i4WidthGain;
    INT32 i4BassGain;
    INT32 i4OutputGain;
    INT32 i4Fc;
} APROC_DSP_MVS_SET_T;

typedef struct {
    BOOL fgMVBFlag;
    UINT32 u4Fc;
    UINT32 u4ExpanderTh;
    UINT32 u4AttackStep;
    UINT32 u4ReleaseStep;
    INT32 i4BassGain;
    INT32 i4DetGain;
    INT32 i4OutputGain;
} APROC_DSP_MVB_SET_T;

typedef struct {
    UINT32 u4Fc;
    UINT32 u4Q;
    UINT32 u4Gain;
} APROC_DSP_PEQ_SET_T;

typedef struct {
    UINT32 u4Type;
    UINT32 u4Fc;
    UINT32 u4Q;
    UINT32 u4Gain;
} APROC_DSP_EQ_SET_T;

typedef struct {
    BOOL  fgLimtFlag;
    INT32 i4SetTargetLevel;
    INT32 i4SetAttackStepDB;
    INT32 i4SetReleaseStepDB;
    INT32 i4SetRatio;
    INT32 i4SetSilenceLevel;
    INT32 i4SetMaxExpandGainDB;
    INT32 i4SetPostGain;
    INT32 i4SetHoldPeriod;
    UINT32 u4SetProcMode; // DSP_DRC_SET_PROC_MODE_T
    INT32 i4SetDetPeriod; 
    INT32 i4SetSgainPeriod; 
} APROC_DSP_LIMT_SET_T;

typedef enum {
    APROC_LIMITER_SP,
    APROC_LIMITER_SP_SW,
    APROC_LIMITER_SP_LSRS,
    APROC_LIMITER_HP,
    APROC_LIMITER_IDX_NUM
} APROC_DSP_LIMT_IDX_T;

typedef enum {
    // Limiter SP
    APROC_REG_LIMITER_SP_TARGET = 0x0,
    APROC_REG_LIMITER_SP_ATTACK,
    APROC_REG_LIMITER_SP_RELEASE,
    APROC_REG_LIMITER_SP_RATIO,
    APROC_REG_LIMITER_SP_SILENCE,
    APROC_REG_LIMITER_SP_MAX_EXPAND,
    APROC_REG_LIMITER_SP_POST_GAIN,
    APROC_REG_LIMITER_SP_HOLD,

    // Limiter SPSW
    APROC_REG_LIMITER_SPSW_TARGET = 0x8,
    APROC_REG_LIMITER_SPSW_ATTACK,
    APROC_REG_LIMITER_SPSW_RELEASE,
    APROC_REG_LIMITER_SPSW_RATIO,
    APROC_REG_LIMITER_SPSW_SILENCE,
    APROC_REG_LIMITER_SPSW_MAX_EXPAND,
    APROC_REG_LIMITER_SPSW_POST_GAIN,
    APROC_REG_LIMITER_SPSW_HOLD,

    APROC_REG_LIMITER_SP_FLAG = 0x18,
    APROC_REG_LIMITER_SP_DET_PERIOD,
    APROC_REG_LIMITER_SPSW_FLAG,
    APROC_REG_LIMITER_SPSW_DET_PERIOD,
    APROC_REG_LIMITER_MODE,
    APROC_REG_LIMITER_SGAIN_PERIOD,
} APROC_ASM_POSTPROCC_T;

typedef enum {
    APROC_SPK_OUTPUT_4_0 = 0x0,
    APROC_SPK_OUTPUT_2_2,
    APROC_SPK_OUTPUT_2_1,
    APROC_SPK_OUTPUT_TEST = 0x100,
} APROC_SPK_OUTPUT_T;

typedef enum {
    APROC_HP_OUTPUT_HP = 0x0,
    APROC_HP_OUTPUT_LINE,
    APROC_HP_OUTPUT_WOOFER,
} APROC_HP_OUTPUT_T;

typedef enum {
    // TSM
    APROC_REG_TSM_0_FLAG = 0x0,
    APROC_REG_TSM_0_SPEED,
    APROC_REG_TSM_1_FLAG,
    APROC_REG_TSM_1_SPEED,
    
    // POST selection
    APROC_REG_LSRS_FROM_LR_SEL,
    APROC_REG_HP_POST_FLAG,
    APROC_REG_IEC_OUT_SEL,

    // Dual mono
    APROC_REG_DUALMONO_0_FLAG = 0x8,
    APROC_REG_DUALMONO_0_MODE,
    APROC_REG_DUALMONO_1_FLAG,
    APROC_REG_DUALMONO_1_MODE,
    APROC_REG_DUALMONO_2_FLAG,
    APROC_REG_DUALMONO_2_MODE,
    
} APROC_ASM_POSTPROCD_T;

typedef enum {
    // band 0
    APROC_REG_BIQ_SP_LSRS_EQ_0_TYPE = 0x0,
    APROC_REG_BIQ_SP_LSRS_EQ_0_FC,
    APROC_REG_BIQ_SP_LSRS_EQ_0_Q,
    APROC_REG_BIQ_SP_LSRS_EQ_0_GAIN,
    
    // band 1
    APROC_REG_BIQ_SP_LSRS_EQ_1_TYPE,
    APROC_REG_BIQ_SP_LSRS_EQ_1_FC,
    APROC_REG_BIQ_SP_LSRS_EQ_1_Q,
    APROC_REG_BIQ_SP_LSRS_EQ_1_GAIN,

    // band 2
    APROC_REG_BIQ_SP_LSRS_EQ_2_TYPE,
    APROC_REG_BIQ_SP_LSRS_EQ_2_FC,
    APROC_REG_BIQ_SP_LSRS_EQ_2_Q,
    APROC_REG_BIQ_SP_LSRS_EQ_2_GAIN,
    
    // band 3
    APROC_REG_BIQ_SP_LSRS_EQ_3_TYPE,
    APROC_REG_BIQ_SP_LSRS_EQ_3_FC,
    APROC_REG_BIQ_SP_LSRS_EQ_3_Q,
    APROC_REG_BIQ_SP_LSRS_EQ_3_GAIN,
    
    // band 4
    APROC_REG_BIQ_SP_LSRS_EQ_4_TYPE,
    APROC_REG_BIQ_SP_LSRS_EQ_4_FC,
    APROC_REG_BIQ_SP_LSRS_EQ_4_Q,
    APROC_REG_BIQ_SP_LSRS_EQ_4_GAIN,
    
    // band 5
    APROC_REG_BIQ_SP_LSRS_EQ_5_TYPE,
    APROC_REG_BIQ_SP_LSRS_EQ_5_FC,
    APROC_REG_BIQ_SP_LSRS_EQ_5_Q,
    APROC_REG_BIQ_SP_LSRS_EQ_5_GAIN,
    
    // band 6
    APROC_REG_BIQ_SP_LSRS_EQ_6_TYPE,
    APROC_REG_BIQ_SP_LSRS_EQ_6_FC,
    APROC_REG_BIQ_SP_LSRS_EQ_6_Q,
    APROC_REG_BIQ_SP_LSRS_EQ_6_GAIN,
    
    // band 7
    APROC_REG_BIQ_SP_LSRS_EQ_7_TYPE,
    APROC_REG_BIQ_SP_LSRS_EQ_7_FC,
    APROC_REG_BIQ_SP_LSRS_EQ_7_Q,
    APROC_REG_BIQ_SP_LSRS_EQ_7_GAIN
    } APROC_ASM_POSTPROCE_T;
    
    
typedef enum {
    // AVC SP LSRS band 0
    APROC_REG_AVC_SP_LSRS_TARGET_0 = 0x0,
    APROC_REG_AVC_SP_LSRS_ATTACK_0,
    APROC_REG_AVC_SP_LSRS_RELEASE_0,
    APROC_REG_AVC_SP_LSRS_RATIO_0,
    APROC_REG_AVC_SP_LSRS_SILENCE_0,
    APROC_REG_AVC_SP_LSRS_MAX_EXPAND_0,
    APROC_REG_AVC_SP_LSRS_POST_GAIN_0,
    APROC_REG_AVC_SP_LSRS_HOLD_0,
    
    // AVC SP LSRS band 0
    APROC_REG_BIQ_SP_LSRS_AVC_0_TYPE_0 = 0x8,
    APROC_REG_BIQ_SP_LSRS_AVC_0_FC_0,
    APROC_REG_BIQ_SP_LSRS_AVC_0_Q_0,
    APROC_REG_BIQ_SP_LSRS_AVC_0_GAIN_0,
    APROC_REG_BIQ_SP_LSRS_AVC_0_TYPE_1,
    APROC_REG_BIQ_SP_LSRS_AVC_0_FC_1,
    APROC_REG_BIQ_SP_LSRS_AVC_0_Q_1,
    APROC_REG_BIQ_SP_LSRS_AVC_0_GAIN_1,

    APROC_REG_DBXTV_EN, //DBX TV Enable
        
} APROC_ASM_POSTPROCF_T;

typedef enum {
    // Limiter SP LSRS
    APROC_REG_LIMITER_SP_LSRS_TARGET = 0x0,
    APROC_REG_LIMITER_SP_LSRS_ATTACK,
    APROC_REG_LIMITER_SP_LSRS_RELEASE,
    APROC_REG_LIMITER_SP_LSRS_RATIO,
    APROC_REG_LIMITER_SP_LSRS_SILENCE,
    APROC_REG_LIMITER_SP_LSRS_MAX_EXPAND,
    APROC_REG_LIMITER_SP_LSRS_POST_GAIN,
    APROC_REG_LIMITER_SP_LSRS_HOLD,

    // Limiter HP
    APROC_REG_LIMITER_HP_TARGET = 0x8,
    APROC_REG_LIMITER_HP_ATTACK,
    APROC_REG_LIMITER_HP_RELEASE,
    APROC_REG_LIMITER_HP_RATIO,
    APROC_REG_LIMITER_HP_SILENCE,
    APROC_REG_LIMITER_HP_MAX_EXPAND,
    APROC_REG_LIMITER_HP_POST_GAIN,
    APROC_REG_LIMITER_HP_HOLD,
    
    APROC_REG_LIMITER_SP_LSRS_FLAG = 0x18,   
    APROC_REG_LIMITER_SP_LSRS_DET_PERIOD,
    APROC_REG_LIMITER_HP_FLAG,
    APROC_REG_LIMITER_HP_DET_PERIOD,
} APROC_ASM_POSTPROC10_T;
typedef enum {

    // EQ
    APROC_REG_BIQ_SP_EQ_FLAG,
    APROC_REG_EQ_FIR_SP_FLAG,
    APROC_REG_BIQ_SP_EQ_STEP,
    APROC_REG_BIQ_SP_LSRS_EQ_FLAG,
    APROC_REG_EQ_FIR_SP_LSRS_FLAG,
    APROC_REG_BIQ_SP_LSRS_EQ_STEP,
    APROC_REG_BIQ_HP_EQ_FLAG,
    APROC_REG_EQ_FIR_HP_FLAG,
    APROC_REG_BIQ_HP_EQ_STEP,

} APROC_ASM_POSTPROC12_T;
typedef enum {
    //Bass Management
    APROC_REG_BMANG_CTRL_FLAG = 0x0,
    APROC_REG_BMANG_CHID_CFG,
    APROC_REG_BMANG_LEF_TRIM,
    APROC_REG_BMANG_VOLUME,
    APROC_REG_BMANG_L_SIZE,
    APROC_REG_BMANG_R_SIZE,
    APROC_REG_BMANG_C_SIZE,
    APROC_REG_BMANG_LS_SIZE,
    APROC_REG_BMANG_RS_SIZE,
    APROC_REG_BMANG_SW_SIZE,
    APROC_REG_BMANG_LFE2CH,

    // SP
    APROC_REG_BIQ_BMANG_SP_HPF_TYPE,
    APROC_REG_BIQ_BMANG_SP_HPF_FC, // cutoff FC
    APROC_REG_BIQ_BMANG_SP_HPF_Q,
    APROC_REG_BIQ_BMANG_SP_HPF_GAIN,
    //SW
    APROC_REG_BIQ_BMANG_SW_LPF_TYPE,
    APROC_REG_BIQ_BMANG_SW_LPF_FC, // cutoff FC
    APROC_REG_BIQ_BMANG_SW_LPF_Q,
    APROC_REG_BIQ_BMANG_SW_LPF_GAIN,
}APROC_ASM_POSTPROC20_T;

typedef enum {
    APROC_REG_PEQ_0_FC,
    APROC_REG_PEQ_1_FC,
    APROC_REG_PEQ_2_FC,
    APROC_REG_PEQ_3_FC,
    APROC_REG_PEQ_4_FC,
    APROC_REG_PEQ_5_FC,
    APROC_REG_PEQ_6_FC,
    APROC_REG_PEQ_7_FC,
    APROC_REG_PEQ_8_FC,

    APROC_REG_PEQ_0_Q,
    APROC_REG_PEQ_1_Q,
    APROC_REG_PEQ_2_Q,
    APROC_REG_PEQ_3_Q,
    APROC_REG_PEQ_4_Q,
    APROC_REG_PEQ_5_Q,
    APROC_REG_PEQ_6_Q,
    APROC_REG_PEQ_7_Q,
    APROC_REG_PEQ_8_Q,

    APROC_REG_PEQ_0_GAIN,
    APROC_REG_PEQ_1_GAIN,
    APROC_REG_PEQ_2_GAIN,
    APROC_REG_PEQ_3_GAIN,
    APROC_REG_PEQ_4_GAIN,
    APROC_REG_PEQ_5_GAIN,
    APROC_REG_PEQ_6_GAIN,
    APROC_REG_PEQ_7_GAIN,
    APROC_REG_PEQ_8_GAIN,

    APROC_REG_PEQ_SP_FLAG = 0x1B,
    }APROC_ASM_POSTPROC21_T;
typedef enum {

    APROC_REG_SEL_DSP_SP_IN = 0x0,
    APROC_REG_SEL_DSP_HP_IN,
    APROC_REG_SEL_DSP_SPDIF_IN,
    APROC_REG_SEL_DSP_BT_IN,
    APROC_REG_SEL_DSP_MON_IN,
    APROC_REG_SEL_SCART_IN,
    APROC_REG_SEL_MIXSND0_IN,
    APROC_REG_SEL_MIXSND1_IN,
    APROC_REG_SEL_MIXSND0_MIX_IN,
    APROC_REG_SEL_MIXSND1_MIX_IN,
    APROC_REG_SEL_UPLOAD1_IN,
    APROC_REG_SEL_ENC_IN,
    APROC_REG_SEL_UPLOAD0_IN,
    APROC_REG_SEL_UPLOAD2_IN,    
    APROC_REG_SEL_LGSE_IN, //for LGSE input
    APROC_REG_SEL_NUM
} APROC_ASM_ID_SEL0_T;

typedef enum {
    // Spost misc
    APROC_REG_SPOST_GETVAR_FLAG = 0x0,
    APROC_REG_SPOST_GETVAR_ISIZE,
    APROC_REG_SPOST_GETVAR_OSIZE,
    APROC_REG_SPOST_LOGOUT_FLAG
} APROC_ASM_SPOST_MISC23_T;
typedef enum {
    // SRS SS3D
    APROC_REG_SS3D_SPSW_ENABLE = 0x0,
    APROC_REG_SS3D_SPSW_HEADGAIN,
    APROC_REG_SS3D_SPSW_BPGAIN,
    APROC_REG_SS3D_SPSW_MAINTECH,
    APROC_REG_SS3D_SPSW_MT_NONE,
    APROC_REG_SS3D_SPSW_MT_WOWHD,
    APROC_REG_SS3D_SPSW_MT_TSHD,
    APROC_REG_SS3D_SPSW_MT_CC3D,
    APROC_REG_SS3D_SPSW_MT_CSHP,
    APROC_REG_SS3D_SPSW_MT_TBHD,
    APROC_REG_SS3D_SPSW_MV_ENABLE,
    APROC_REG_SS3D_SPSW_TBHDRefOffset,
    APROC_REG_SS3D_SPSW_HdLmtBoostOffset,    

} APROC_ASM_ID_SRS_SS3D_T;

typedef enum {
    // SRS TVOL
    APROC_REG_TVOL_SPSW_ENABLE = 0x0,
    APROC_REG_TVOL_SPSW_INPUTGAIN,
    APROC_REG_TVOL_SPSW_OUTGAIN,
    APROC_REG_TVOL_SPSW_BPGAIN,
    APROC_REG_TVOL_SPSW_LVINTER,
    APROC_REG_TVOL_SPSW_MODE,
    APROC_REG_TVOL_SPSW_NREN,
    APROC_REG_TVOL_SPSW_NRTH,
    APROC_REG_TVOL_SPSW_LKFS,
    APROC_REG_TVOL_SPSW_SMOOTHEN,

    // SRS AEQ
    APROC_REG_LAEQ_SPSW_ENABLE = 0xA,
    APROC_REG_LAEQ_SPSW_INPUTGAIN,
    APROC_REG_LAEQ_SPSW_OUTGAIN,
    APROC_REG_LAEQ_SPSW_BPGAIN,
    APROC_REG_RAEQ_SPSW_ENABLE,
    APROC_REG_RAEQ_SPSW_INPUTGAIN,
    APROC_REG_RAEQ_SPSW_OUTGAIN,
    APROC_REG_RAEQ_SPSW_BPGAIN,

    
    // SRS TruDialog
    APROC_REG_TRUD_SPSW_ENABLE = 0x12,
    APROC_REG_TRUD_SPSW_INPUTGAIN,
    APROC_REG_TRUD_SPSW_OUTGAIN,
    APROC_REG_TRUD_SPSW_BPGAIN,
    APROC_REG_TRUD_SPSW_PRGAIN,
    APROC_REG_TRUD_SPSW_CLAGAIN,

} APROC_ASM_ID_SRS_TVOL_AEQ_TRUD_T;

typedef enum {
    // SRS CC3D
    APROC_REG_CC3D_SPSW_ENABLE = 0x0,
    APROC_REG_CC3D_SPSW_INPUTGAIN,
    APROC_REG_CC3D_SPSW_OUTGAIN,
    APROC_REG_CC3D_SPSW_BPGAIN,
    APROC_REG_CC3D_SPSW_DPEN,
    APROC_REG_CC3D_SPSW_SUREN,
    APROC_REG_CC3D_SPSW_MODE,
    APROC_REG_CC3D_SPSW_DEPMODE,
    APROC_REG_CC3D_SPSW_DEPFADE_ENABLE,
    APROC_REG_CC3D_SPSW_LR_ENABLE,
    APROC_REG_CC3D_SPSW_LR_GAIN,
    APROC_REG_CC3D_SPSW_MIXFADE_CTRL,

} APROC_ASM_ID_SRS_CC3D_T;

typedef enum {
    // SRS STTSHD
    APROC_REG_TSHD_SPSW_ENABLE = 0x0,
    APROC_REG_TSHD_SPSW_INPUTGAIN,
    APROC_REG_TSHD_SPSW_OUTGAIN,
    APROC_REG_TSHD_SPSW_BPGAIN,
    APROC_REG_TSHD_SPSW_SURENABLE,
    APROC_REG_TSHD_SPSW_INMODE,
    APROC_REG_TSHD_SPSW_FS_CTRL,
    APROC_REG_TSHD_SPSW_FC_CTRL,
    APROC_REG_TSHD_SPSW_RS_CTRL,
    APROC_REG_TSHD_SPSW_RC_CTRL,
    APROC_REG_TSHD_SPSW_FR_GAIN1,
    APROC_REG_TSHD_SPSW_FR_GAIN2,
    APROC_REG_TSHD_SPSW_TS_LEV,
    APROC_REG_TSHD_SPSW_SPTUNE,
    APROC_REG_TSHD_SPSW_MF_CTRL,
    APROC_REG_TSHD_SPSW_MR_CTRLEXT,
    APROC_REG_TSHD_SPSW_MIX_EN,//0X10
    APROC_REG_TSHD_SPSW_LDEF_EN,
    APROC_REG_TSHD_SPSW_LDEF_Lev,
    APROC_REG_TSHD_SPSW_RDEF_EN,
    APROC_REG_TSHD_SPSW_RDEF_Lev,//0X14
    APROC_REG_TSHD_SPSW_LFOCUS_EN,
    APROC_REG_TSHD_SPSW_LFOCUS_Lev,
    APROC_REG_TSHD_SPSW_RFOCUS_EN,
    APROC_REG_TSHD_SPSW_RFOCUS_Lev,//0X18
    APROC_REG_TSHD_SPSW_CFOCUS_EN,
    APROC_REG_TSHD_SPSW_CFOCUS_Lev,//0X1A

    // SRS SPHPF
    APROC_REG_SPHPF_SPSW_ENABLE = 0x1B,
    APROC_REG_SPHPF_SPSW_INPUTGAIN,
    APROC_REG_SPHPF_SPSW_OUTGAIN,
    APROC_REG_SPHPF_SPSW_BPGAIN,
    APROC_REG_SPHPF_SPSW_CUTOFF_FRQ,

} APROC_ASM_ID_SRS_TSHD_SPHPF_T;

typedef enum {
    // SRS TBHD
    APROC_REG_TBHD_SPSW_ENABLE = 0x0,
    APROC_REG_TBHD_SPSW_INDEP,
    APROC_REG_TBHD_SPSW_ANA_EN,
    APROC_REG_TBHD_SPSW_INPUTGAIN,
    APROC_REG_TBHD_SPSW_OUTGAIN,
    APROC_REG_TBHD_SPSW_BPGAIN,
    APROC_REG_TBHD_SPSW_TB_CTRL,
    APROC_REG_TBHD_SPSW_CMP_CTRL,
    APROC_REG_TBHD_SPSW_RF_LEV,
    APROC_REG_TBHD_SPSW_INTGRA_LIMIT,
    APROC_REG_TBHD_SPSW_SPK_SIZE,
    APROC_REG_TBHD_SPSW_ANA_SIZE,
    APROC_REG_TBHD_SPSW_MODE,
    APROC_REG_TBHD_SPSW_OUT_OPT,
    APROC_REG_TBHD_SPSW_TBQ,
    APROC_REG_TBHD_SPSW_DCU_LEV,//0xF
    
    // SRS SRS3D
    APROC_REG_SRS3D_SPSW_ENABLE = 0x10,
    APROC_REG_SRS3D_SPSW_INPUTGAIN,
    APROC_REG_SRS3D_SPSW_OUTGAIN,
    APROC_REG_SRS3D_SPSW_BPGAIN,
    APROC_REG_SRS3D_SPSW_HBITRATE,
    APROC_REG_SRS3D_SPSW_SPACE_CTRL,
    APROC_REG_SRS3D_SPSW_CEN_CTRL,
    APROC_REG_SRS3D_SPSW_HP,
    APROC_REG_SRS3D_SPSW_MODE,
    APROC_REG_SRS3D_SPSW_NORM_GAIN,

} APROC_ASM_ID_SRS_TBHD_SRS3D_T;

typedef enum {
    // SRS CSHP
    APROC_REG_CSHP_SPSW_ENABLE = 0x0,
    APROC_REG_CSHP_SPSW_INPUTGAIN,
    APROC_REG_CSHP_SPSW_OUTGAIN,
    APROC_REG_CSHP_SPSW_BPGAIN,
    APROC_REG_CSHP_SPSW_RENDMODE,

} APROC_ASM_ID_SRS_CSHP_T;

typedef enum {
    // SRS GEQ5B
    APROC_REG_LGEQ5B_SPSW_ENABLE = 0x0,
    APROC_REG_LGEQ5B_SPSW_INTER_BNAD,
    APROC_REG_LGEQ5B_SPSW_EXBAND_BEH,
    APROC_REG_LGEQ5B_SPSW_BGAIN0,
    APROC_REG_LGEQ5B_SPSW_BGAIN1,
    APROC_REG_LGEQ5B_SPSW_BGAIN2,
    APROC_REG_LGEQ5B_SPSW_BGAIN3,
    APROC_REG_LGEQ5B_SPSW_BGAIN4,
    APROC_REG_LGEQ5B_SPSW_GAIN_UPDATE,
    APROC_REG_LGEQ5B_SPSW_INPUTGAIN,
    APROC_REG_RGEQ5B_SPSW_ENABLE,
    APROC_REG_RGEQ5B_SPSW_INTER_BNAD,
    APROC_REG_RGEQ5B_SPSW_EXBAND_BEH,
    APROC_REG_RGEQ5B_SPSW_BGAIN0,
    APROC_REG_RGEQ5B_SPSW_BGAIN1,
    APROC_REG_RGEQ5B_SPSW_BGAIN2,
    APROC_REG_RGEQ5B_SPSW_BGAIN3,
    APROC_REG_RGEQ5B_SPSW_BGAIN4,
    APROC_REG_RGEQ5B_SPSW_GAIN_UPDATE,
    APROC_REG_RGEQ5B_SPSW_INPUTGAIN,//0x13

     // SRS HPF
    APROC_REG_LHPF_SPSW_ENABLE = 0x14,
    APROC_REG_LHPF_SPSW_SECTION,
    APROC_REG_RHPF_SPSW_ENABLE,
    APROC_REG_RHPF_SPSW_SECTION,

} APROC_ASM_ID_SRS_GEQ_HPF_T;

typedef enum {
    // SRS BASSTREBLE
    APROC_REG_BASTRB_SPSW_EANBLE = 0x0,
    APROC_REG_BASTRB_SPSW_INPUTGAIN,
    APROC_REG_BASTRB_SPSW_OUTGAIN,
    APROC_REG_BASTRB_SPSW_BPGAIN,
    APROC_REG_BASTRB_SPSW_BASSGAIN,
    APROC_REG_BASTRB_SPSW_TRBGAIN,

    // SRS Master Volume
    APROC_REG_MVOL_SPSW_ENABLE = 0x8,
    APROC_REG_MVOL_SPSW_VOLSTEP,
    APROC_REG_MVOL_SPSW_PAN,
    APROC_REG_MVOL_SPSW_MUTE,
    APROC_REG_MVOL_SPSW_BPGAIN,
    APROC_REG_MVOL_SPSW_PROCMODE,
    APROC_REG_MVOL_SPSW_HL_BOOST,

     // SRS HardLimiter
    APROC_REG_HL_SPSW_ENABLE = 0x10,
    APROC_REG_HL_SPSW_INPUTGAIN,
    APROC_REG_HL_SPSW_OUTGAIN,
    APROC_REG_HL_SPSW_BPGAIN,
    APROC_REG_HL_SPSW_DEL_LEN,
    APROC_REG_HL_SPSW_BOOST,
    APROC_REG_HL_SPSW_AC_ENA,
    APROC_REG_HL_SPSW_AC_LEV,   

} APROC_ASM_ID_SRS_BT_MV_HL_T;

typedef enum {
    // EqSpectrum
    APROC_REG_SPECTRUM_ENABLE = 0x0,
    APROC_REG_SPECTRUM_0,
    APROC_REG_SPECTRUM_1,
    APROC_REG_SPECTRUM_2,
    APROC_REG_SPECTRUM_3,
    APROC_REG_SPECTRUM_4,
    APROC_REG_SPECTRUM_5,
    APROC_REG_SPECTRUM_6,
    APROC_REG_SPECTRUM_7,
    APROC_REG_SPECTRUM_8,
    APROC_REG_SPECTRUM_9,
    APROC_REG_SPECTRUM_10,
    APROC_REG_SPECTRUM_11,
    APROC_REG_SPECTRUM_12,
    APROC_REG_SPECTRUM_13,
    APROC_REG_SPECTRUM_14,
    //APROC_REG_SPECTRUM_15, 

} APROC_ASM_ID_EQSPECTRUM_T;

typedef enum {
    // MP3 encoder
    APROC_REG_IOBUF_INTBUF4_ENA = 0x0,
    APROC_REG_IOBUF_INTBUF4_START, 
    APROC_REG_IOBUF_INTBUF4_END,
    APROC_REG_IOBUF_INTBUF4_CH,
    APROC_REG_IOBUF_INTBUF4_SIZE,
    
}APROC_ASM_ENC_MP3_T;


typedef enum {
    // band 0
    APROC_REG_BIQ_HP_EQ_0_TYPE = 0x0,
    APROC_REG_BIQ_HP_EQ_0_FC,
    APROC_REG_BIQ_HP_EQ_0_Q,
    APROC_REG_BIQ_HP_EQ_0_GAIN,

    // band 1
    APROC_REG_BIQ_HP_EQ_1_TYPE,
    APROC_REG_BIQ_HP_EQ_1_FC,
    APROC_REG_BIQ_HP_EQ_1_Q,
    APROC_REG_BIQ_HP_EQ_1_GAIN,

    // band 2
    APROC_REG_BIQ_HP_EQ_2_TYPE,
    APROC_REG_BIQ_HP_EQ_2_FC,
    APROC_REG_BIQ_HP_EQ_2_Q,
    APROC_REG_BIQ_HP_EQ_2_GAIN,

    // band 3
    APROC_REG_BIQ_HP_EQ_3_TYPE,
    APROC_REG_BIQ_HP_EQ_3_FC,
    APROC_REG_BIQ_HP_EQ_3_Q,
    APROC_REG_BIQ_HP_EQ_3_GAIN,

    // band 4
    APROC_REG_BIQ_HP_EQ_4_TYPE,
    APROC_REG_BIQ_HP_EQ_4_FC,
    APROC_REG_BIQ_HP_EQ_4_Q,
    APROC_REG_BIQ_HP_EQ_4_GAIN,

    // band 5
    APROC_REG_BIQ_HP_EQ_5_TYPE,
    APROC_REG_BIQ_HP_EQ_5_FC,
    APROC_REG_BIQ_HP_EQ_5_Q,
    APROC_REG_BIQ_HP_EQ_5_GAIN,

    // band 6
    APROC_REG_BIQ_HP_EQ_6_TYPE,
    APROC_REG_BIQ_HP_EQ_6_FC,
    APROC_REG_BIQ_HP_EQ_6_Q,
    APROC_REG_BIQ_HP_EQ_6_GAIN,

    // band 7
    APROC_REG_BIQ_HP_EQ_7_TYPE,
    APROC_REG_BIQ_HP_EQ_7_FC,
    APROC_REG_BIQ_HP_EQ_7_Q,
    APROC_REG_BIQ_HP_EQ_7_GAIN
}APROC_ASM_POSTPROC3B_T;

typedef enum {
    // Trims
    APROC_REG_SP_CH7CH8_TRIM = 0x0,
    APROC_REG_DELAY7_TRIM,
    APROC_REG_DELAY8_TRIM,
    APROC_REG_DELAY_INPUT0_TRIM,
    APROC_REG_DELAY_INPUT1_TRIM,
    
    // Inverter
    APROC_REG_SP_CH7CH8_INV = 0x8,
    
    // Volume
    APROC_REG_SP_CH7CH8_VOL = 0xc,

    // Delay
    APROC_REG_DELAY_SP_CH7CH8 = 0x10,
    APROC_REG_DELAY_BT,
    APROC_REG_DELAY_INPUT0,
    APROC_REG_DELAY_INPUT1,
    
} APROC_ASM_POSTPROC3C_T;

typedef enum {
	APROC_REG_LGSE_ENABLE = 0x0,
	APROC_REG_LGSE_FUNC_ENABLE,
	APROC_REG_LGSE_MODIFIED_INIT,
	APROC_REG_LGSE_MODIFIED_VAR,
	APROC_REG_LGSE_MODIFIED_MODE,
	APROC_REG_LGSE_INIT_NO,
	APROC_REG_LGSE_VAR_NO,
	APROC_REG_LGSE_GET_FN010_OUT, // 0x7
	APROC_REG_LGSE_VOL_DEBUG1,
	APROC_REG_LGSE_VOL_DEBUG2
} APROC_ASM_LGSE_CTRL_T;

typedef enum {
	APROC_REG_LGSE_FN010_LGSE00663_0 = 0x0,
	APROC_REG_LGSE_FN010_LGSE00663_1	
} APROC_ASM_LGSE_OUT_T;

/* APROC Data Port (ADP)
    */
#define APROC_DATAPORT_SIZE 1024
    
#define APROC_DATAPORT_DOWNMIX_TBL_BASE 0x0
#define APROC_DATAPORT_DOWNMIX_TBL_SIZE 10
#define APROC_DATAPORT_DOWNMIX_TBL_1_BASE (APROC_DATAPORT_DOWNMIX_TBL_BASE + APROC_DATAPORT_DOWNMIX_TBL_SIZE)
    
#define APROC_DATAPORT_FIR_SCALE_SIZE 1
    
#define APROC_DATAPORT_SP_LSRS_FIR_TAP 128
#define APROC_DATAPORT_SP_LSRS_128FIR_BASE (APROC_DATAPORT_DOWNMIX_TBL_1_BASE + APROC_DATAPORT_DOWNMIX_TBL_SIZE)
#define APROC_DATAPORT_SP_LSRS_128FIR_SCALE_BASE (APROC_DATAPORT_SP_LSRS_128FIR_BASE+APROC_DATAPORT_SP_LSRS_FIR_TAP)
#define APROC_DATAPORT_SP_LSRS_128FIR_SIZE (APROC_DATAPORT_SP_LSRS_FIR_TAP+APROC_DATAPORT_FIR_SCALE_SIZE)
    
#define APROC_DATAPORT_SP_FIR_TAP 128
#define APROC_DATAPORT_SP_FIR_BASE (APROC_DATAPORT_SP_LSRS_128FIR_SCALE_BASE + APROC_DATAPORT_FIR_SCALE_SIZE)
#define APROC_DATAPORT_SP_FIR_SCALE_BASE (APROC_DATAPORT_SP_FIR_BASE+APROC_DATAPORT_SP_LSRS_FIR_TAP)
#define APROC_DATAPORT_SP_FIR_SIZE (APROC_DATAPORT_SP_FIR_TAP+1)

#define APROC_DATAPORT_SAPOST_DEBUG_BASE (APROC_DATAPORT_SP_FIR_SCALE_BASE+APROC_DATAPORT_FIR_SCALE_SIZE)
#define APROC_DATAPORT_SAPOST_DEBUG_SIZE (64)
#define SAPOST_DEBUG_SZ (APROC_DATAPORT_SAPOST_DEBUG_SIZE*4)

#define APROC_DATAPORT_DBXTV_PARAM_BASE (APROC_DATAPORT_SAPOST_DEBUG_BASE+SAPOST_DEBUG_SZ)
#define APROC_DATAPORT_DBXTV_PARAM_SIZE (95)
#define APROC_DATAPORT_DBXTV_COEFF_BASE (APROC_DATAPORT_DBXTV_PARAM_BASE+APROC_DATAPORT_DBXTV_PARAM_SIZE)
#define APROC_DATAPORT_DBXTV_COEFF_SIZE (100)
//#define APROC_DATAPORT_DBXTV_ENABLE_BASE (APROC_DATAPORT_DBXTV_COEFF_BASE+APROC_DATAPORT_DBXTV_COEFF_SIZE)
//#define APROC_DATAPORT_DBXTV_ENABLE_SIZE (1)


#define APROC_ASM_ADDR(grp,idx) ((grp) * APROC_REGMAP_IO_GRP_SIZE + (idx))


typedef enum {
    APROC_ROUTINE_ID_ER_SP_PATH = 0x0,
    APROC_ROUTINE_ID_ER_SP_LSRS_PATH,       
    APROC_ROUTINE_ID_ER_HP_PATH,
    APROC_ROUTINE_ID_DR_SP_PATH,
    APROC_ROUTINE_ID_DR_HP_PATH,
    APROC_ROUTINE_ID_DR_SPDIF_PCM_PATH,
    APROC_ROUTINE_ID_DR_SPDIF_SOUNDBAR_PATH,
    APROC_ROUTINE_ID_DR_INPUT0_PATH,
    APROC_ROUTINE_ID_DR_INPUT1_PATH, 
    APROC_ROUTINE_ID_CR_DMIX0_PATH,
    APROC_ROUTINE_ID_CR_DMIX1_PATH,
    APROC_ROUTINE_ID_CR_DMIX2_PATH,
    APROC_ROUTINE_ID_AER_AMIXER0_PATH,
    APROC_ROUTINE_ID_AER_AMIXER1_PATH,
    APROC_ROUTINE_ID_AER_AMIXER2_PATH,
    APROC_ROUTINE_ID_NUM
} APROC_ROUTINE_ID_T;


#define Aproc_COMM_SIG_INT           0x1
#define Aproc_Comm_Siganl() (vAprocBIM_write(APROC_M2AINT, Aproc_COMM_SIG_INT))
#define u4Aproc_Comm_Signal_Is_Buzy() (u4AprocBIM_read(APROC_M2AINT) & Aproc_COMM_SIG_INT)

typedef void (*AUD_APROC_FUNC_T)(void);

typedef struct {
    UINT32 u4Idx;
    UINT32 u4Cmd;    
    UINT32 u4Data0;
    UINT32 u4Data1;
    AUD_APROC_FUNC_T u4Func;
} _AUD_APROC_COMM_SIG_T;

#define Aproc_Sig_A2M_Is_Hit(a,b) (((a) && (a == b)) || (a == 0)) 
#define u4AudAprocSig_GetCMD(a) ((a) & APROC_SIG_CMD_MSK)
#define u4AudAprocSig_GetIdx(a) (((a) >> APROC_SIG_IDX_SHIFT) & APROC_SIG_IDX_MSK)

typedef enum {
    APROC_CONTROL_TYPE_SGAIN = 0,
    APROC_CONTROL_TYPE_VOL,
    APROC_CONTROL_TYPE_MUTE,
    APROC_CONTROL_TYPE_TRIM,
    APROC_CONTROL_TYPE_BALANCE,
    APROC_CONTROL_TYPE_BASSTREBLE,
    APROC_CONTROL_TYPE_MVS,
    APROC_CONTROL_TYPE_MVB,
    APROC_CONTROL_TYPE_DELAY,
    APROC_CONTROL_TYPE_DOWNMIX,
    APROC_CONTROL_TYPE_IEC,
    APROC_CONTROL_TYPE_DUALMONO,
    APROC_CONTROL_TYPE_VS,
    APROC_CONTROL_TYPE_AD,
    APROC_CONTROL_TYPE_MONOMIX,
    APROC_CONTROL_TYPE_FIR,
    APROC_CONTROL_TYPE_SEL,
    APROC_CONTROL_TYPE_CHSEL,
    APROC_CONTROL_TYPE_DRC,    
    APROC_CONTROL_TYPE_EQ,     
    APROC_CONTROL_TYPE_LIMITER,        
    APROC_CONTROL_TYPE_BMANG, /*Aproc Bmang*/
    APROC_CONTROL_TYPE_PEQ, /*Aproc PEQ*/
    APROC_CONTROL_TYPE_MIXSND,
    APROC_CONTROL_TYPE_DBG,
    APROC_CONTROL_TYPE_NUM
} APROC_CONTROL_TYPE_T;

typedef enum {
    APROC_CONTROL_CH_L = 0,
    APROC_CONTROL_CH_R,
    APROC_CONTROL_CH_NUM
} APROC_CONTROL_CH_T;

typedef enum {
    APROC_DSP_DRCV20_ID_AVC0 = 0,
    APROC_DSP_DRCV20_ID_AVC1,
    APROC_DSP_DRCV20_ID_NUM
} APROC_DSP_DRCV20_ID_T;


typedef enum {
    APROC_DSP_BIQ_ID_BASS_SP = 0,
    APROC_DSP_BIQ_ID_BASS_SP_LSRS,
    APROC_DSP_BIQ_ID_BASS_HP,
    APROC_DSP_BIQ_ID_TREBLE_SP,
    APROC_DSP_BIQ_ID_TREBLE_SP_LSRS,
    APROC_DSP_BIQ_ID_TREBLE_HP,
    APROC_DSP_BIQ_ID_BASS_TREBLE_FLAG_SP,
    APROC_DSP_BIQ_ID_BASS_TREBLE_FLAG_SP_LSRS,
    APROC_DSP_BIQ_ID_BASS_TREBLE_FLAG_HP,        
    APROC_DSP_BIQ_ID_BASSTREBLE_NUM
} APROC_DSP_BIQ_ID_BASSTREBLE_T;

typedef enum {
       APROC_DSP_BIQ_ID_HPF_SP = 0,
       APROC_DSP_BIQ_ID_LPF_SP_LSRS,
       APROC_DSP_BIQ_ID_CHSEL_MODE,
       APROC_DSP_BIQ_ID_CHSEL_NUM
} APROC_DSP_BIQ_ID_CHSEL_T;

typedef enum {
       APROC_DSP_BIQ_ID_3DRC_0 = 0,
       APROC_DSP_BIQ_ID_3DRC_1,
       APROC_DSP_BIQ_ID_3DRC_2_1,
       APROC_DSP_BIQ_ID_3DRC_2_2,
       APROC_DSP_BIQ_ID_3DRC_TARGET0,
       APROC_DSP_BIQ_ID_3DRC_TARGET1,
       APROC_DSP_BIQ_ID_3DRC_TARGET2,
       APROC_DSP_BIQ_ID_3DRC_FLAG,
       APROC_DSP_BIQ_ID_1DRC_0,
       APROC_DSP_BIQ_ID_1DRC_TARGET0,
       APROC_DSP_BIQ_ID_1DRC_FLAG,
       APROC_DSP_BIQ_ID_DRC_NUM
} APROC_DSP_BIQ_ID_DRC_T;

/* IO control types
*/

#define APROC_IOCTRL_RET_OK 0
#define APROC_IOCTRL_RET_FAIL 1


#define APROC_IOCTRL_NO_ROUTINE 0xFFFFFFFF

// Smooth gain
typedef enum {
    APROC_IOCTRL_SGAIN_DSP_PERIOD = 0,
    APROC_IOCTRL_SGAIN_AMIXER_PERIOD,
    APROC_IOCTRL_SGAIN_MUTE_AMIXER0_PERIOD,
    APROC_IOCTRL_SGAIN_MUTE_AMIXER1_PERIOD,
    APROC_IOCTRL_SGAIN_MUTE_AMIXER2_PERIOD,
    APROC_IOCTRL_SGAIN_NUM
} APROC_IOCTRL_SGAIN_T;

// Volume
typedef enum {
    APROC_IOCTRL_VOL_SP = 0,
    APROC_IOCTRL_VOL_SP_LSRS,       
    APROC_IOCTRL_VOL_SPSW,
    APROC_IOCTRL_VOL_HP,
    APROC_IOCTRL_VOL_MON,
    APROC_IOCTRL_VOL_IEC,
    APROC_IOCTRL_VOL_SCART,
    APROC_IOCTRL_VOL_BT,
    APROC_IOCTRL_VOL_AMIXER0,
    APROC_IOCTRL_VOL_AMIXER1,
    APROC_IOCTRL_VOL_AMIXER2, 
    APROC_IOCTRL_VOL_NUM
} APROC_IOCTRL_VOL_T;

// Trim
typedef enum {
    APROC_IOCTR_TRIM_INPUT0 = 0,
    APROC_IOCTR_TRIM_INPUT1,
    APROC_IOCTR_TRIM_INPUT2,
    APROC_IOCTR_TRIM_SP,
    APROC_IOCTR_TRIM_SPLSRS,
    APROC_IOCTR_TRIM_SPSW,
    APROC_IOCTR_TRIM_HP,
    APROC_IOCTR_TRIM_MON,
    APROC_IOCTR_TRIM_IEC,
    APROC_IOCTR_TRIM_SCART,
    APROC_IOCTR_TRIM_BT,
    APROC_IOCTR_TRIM_AMIXER0,
    APROC_IOCTR_TRIM_AMIXER1,
    APROC_IOCTR_TRIM_AMIXER2,    
    APROC_IOCTR_TRIM_DELAY0,
    APROC_IOCTR_TRIM_DELAY1,
    APROC_IOCTR_TRIM_NUM
} APROC_IOCTR_TRIM_T;

// Mute
typedef enum {
    APROC_IOCTR_MUTE_AMIXER0 = 0,
    APROC_IOCTR_MUTE_AMIXER1,
    APROC_IOCTR_MUTE_AMIXER2,
    APROC_IOCTR_MUTE_NUM
} APROC_IOCTR_MUTE_T;

// Mute
typedef enum {
    APROC_IOCTR_BALANCE_SP = 0,
    APROC_IOCTR_BALANCE_HP,
    APROC_IOCTR_BALANCE_IEC,
    APROC_IOCTR_BALANCE_INPUT0,
    APROC_IOCTR_BALANCE_INPUT1,
    APROC_IOCTR_BALANCE_INPUT2,
    APROC_IOCTR_BALANCE_NUM
} APROC_IOCTR_BALANCE_T;


// Dual mono
typedef enum {
    APROC_IOCTRL_DUALMONO_AMXIER0_FLAG = 0,
    APROC_IOCTRL_DUALMONO_AMXIER1_FLAG,
    APROC_IOCTRL_DUALMONO_AMXIER2_FLAG,
    APROC_IOCTRL_DUALMONO_AMXIER0_MODE,
    APROC_IOCTRL_DUALMONO_AMXIER1_MODE,
    APROC_IOCTRL_DUALMONO_AMXIER2_MODE,
    APROC_IOCTRL_DUALMONO_NUM
} APROC_IOCTRL_DUALMONO_T;

// TSM
typedef enum {
    APROC_IOCTRL_TSM_AMIXER0_FLAG = 0,
    APROC_IOCTRL_TSM_AMIXER1_FLAG,
    APROC_IOCTRL_TSM_AMIXER0_SPEED,
    APROC_IOCTRL_TSM_AMIXER1_SPEED,
    APROC_IOCTRL_TSM_NUM
} APROC_IOCTRL_TSM_T;

// IEC
typedef enum {
    APROC_IOCTRL_IEC_FLAG = 0,
    APROC_IOCTRL_IEC_MODE,
    APROC_IOCTRL_IEC_COPYPROTECT,
    APROC_IOCTRL_IEC_MUTE,
    APROC_IOCTRL_IEC_CHANNELSTATUS,
    APROC_IOCTRL_IEC_RAWINFO,
    APROC_IOCTRL_IEC_RAWDELAY,
    APROC_IOCTRL_IEC_PCMCHSEL,
    APROC_IOCTRL_IEC_PCMBITS,
    APROC_IOCTRL_IEC_OUTPUT,
    APROC_IOCTRL_IEC_RAWAUDFMT,
    APROC_IOCTRL_IEC_NUM
} APROC_IOCTRL_IEC_T;

// Mixsound
typedef enum
{
    AUD_MIXSND_0 = 0,
    AUD_MIXSND_1,
    AUD_MIXSND_2,
    AUD_MIXSND_3,
    AUD_MIXSND_4,
    AUD_MIXSND_5,
    AUD_MIXSND_6,   
    AUD_MIXSND_7,
#ifdef CC_AUD_APROC_HWMIX 
    AUD_HW_MIXSND_0,
    AUD_HW_MIXSND_1,
#endif   
    AUD_MIXSND_NUM
}   AUD_MIXSND_ID_T;

typedef enum {
    APROC_IOCTRL_MIXSND_ENABLE = 0,
    APROC_IOCTRL_MIXSND_TRIM,
    APROC_IOCTRL_MIXSND_IN_TRIM, 
    APROC_IOCTRL_MIXSND_NUM
} APROC_IOCTRL_MIXSND_T;

typedef enum {
    APROC_IOCTRL_SP = 0,
    APROC_IOCTRL_LSRS,
    APROC_IOCTRL_SW,
    APROC_IOCTRL_HP,
    APROC_IOCTRL_IEC,
    APROC_IOCTRL_SCART,
} APROC_IOCTRL_T;

typedef enum {
    APROC_MEMMAP_WORKBUF = 0,
    APROC_MEMMAP_WORKBUF_PHY,
    APROC_MEMMAP_REGMAP,
    APROC_MEMMAP_DATAPORT,
    APROC_MEMMAP_ADSP_DRAM,
    APROC_MEMMAP_ADSP_CMPT,    
    APROC_MEMMAP_AQDATA,    
    APROC_MEMMAP_SIZE
} _APROC_MEMMAP_T;


typedef enum {
    AVSYNC_CMD_NON = 0,
    AVSYNC_CMD_WAIT_ON,
    AVSYNC_CMD_WAIT_OFF,
    AVSYNC_CMD_WAIT_OFFON,
    AVSYNC_CMD_SKIP
} AVSYNC_CMD_T;


typedef enum
{
    APROC_AVSYNC_CTRL_WORK,
    APROC_AVSYNC_CTRL_HOLD,
    APROC_AVSYNC_CTRL_HOLD_COMPLETE,
}   APROC_AVSYNC_CTRL_MODE_T;

typedef struct {
    UINT32 u4Type;
    UINT32 u4Fc;
    UINT32 u4Q;
    INT32 i4Gain;
} APROC_DSP_BIQ_INFO_T;

typedef enum {
    APROC_BIQUAD_TYPE_LPF = 0,
    APROC_BIQUAD_TYPE_HPF,    
    APROC_BIQUAD_TYPE_PEAKINGEQ,
    APROC_BIQUAD_TYPE_LOWSHELF,
    APROC_BIQUAD_TYPE_HIGHSHELF,
    APROC_BIQUAD_TYPE_BPF,
    APROC_BIQUAD_TYPE_APF,
    APROC_BIQUAD_TYPE_LPF_HI = 0x100,
    APROC_BIQUAD_TYPE_HPF_HI,
    APROC_BIQUAD_TYPE_PEAKINGEQ_HI,
    APROC_BIQUAD_TYPE_LOWSHELF_HI,
    APROC_BIQUAD_TYPE_HIGHSHELF_HI,
    APROC_BIQUAD_TYPE_BPF_HI,
    APROC_BIQUAD_TYPE_APF_HI,
    APROC_BIQUAD_TYPE_NUM
} APROC_BIQUAD_TYPE_T;

#define ADSP_BIQ_DB(dB) ((18 + (dB)) * 20)
#define APROC_BIQUAD_ONE 0x3fffff
#define APROC_BIQUAD_FS 48000

#define APROC_BIQUAD_BIT_SHIFT 7
#define APROC_BIQUAD_BIT_SHIFT1 7

#define ADSP_BIQ_DIV(a,b) ((INT32) ((INT64)((INT64) (a) << 23) / (b)))


/* ADSP Decoder IO buffer define
*/
//#if 0
#if !defined(CC_AUD_ARM_RENDER)
#define APROC_ADSP_IOBUF_DEC0_START (4*0x64000) // bytes offset
#define APROC_ADSP_IOBUF_DEC0_END (8)
#define APROC_ADSP_IOBUF_DEC0_SIZE (4*1024) // bytes
#define APROC_ADSP_IOBUF_DEC0_CH (8) 
#define APROC_ADSP_IOBUF_DEC0_SI_BASE (4*0x2000) // bytes offset

#define APROC_ADSP_IOBUF_DEC1_START (4*0x70000) // bytes offset
#define APROC_ADSP_IOBUF_DEC1_END (8)
#define APROC_ADSP_IOBUF_DEC1_SIZE (4*1024) // bytes
#define APROC_ADSP_IOBUF_DEC1_CH (8) 
#define APROC_ADSP_IOBUF_DEC1_SI_BASE (4*0x2400) // bytes offset

#define APROC_ADSP_IOBUF_DEC2_START (4*0x7C000) // bytes offset
#define APROC_ADSP_IOBUF_DEC2_END (8)
#define APROC_ADSP_IOBUF_DEC2_SIZE (4*1024) // bytes
#define APROC_ADSP_IOBUF_DEC2_CH (8) 
#define APROC_ADSP_IOBUF_DEC2_SI_BASE (4*0x2800) // bytes offset
#else
#define APROC_ADSP_IOBUF_DEC0_START (4*0x80C00) // bytes offset
#define APROC_ADSP_IOBUF_DEC0_END (8)
#define APROC_ADSP_IOBUF_DEC0_SIZE (4*1024) // bytes
#define APROC_ADSP_IOBUF_DEC0_CH (8) 
#define APROC_ADSP_IOBUF_DEC0_SI_BASE (4*0x3000) // bytes offset

#define APROC_ADSP_IOBUF_DEC1_START (4*0x8CC00) // bytes offset
#define APROC_ADSP_IOBUF_DEC1_END (8)
#define APROC_ADSP_IOBUF_DEC1_SIZE (4*1024) // bytes
#define APROC_ADSP_IOBUF_DEC1_CH (8) 
#define APROC_ADSP_IOBUF_DEC1_SI_BASE (4*0x3400) // bytes offset

#define APROC_ADSP_IOBUF_DEC2_START (4*0x9EC00) // bytes offset
#define APROC_ADSP_IOBUF_DEC2_END (8)
#define APROC_ADSP_IOBUF_DEC2_SIZE (4*1024) // bytes
#define APROC_ADSP_IOBUF_DEC2_CH (8) 
#define APROC_ADSP_IOBUF_DEC2_SI_BASE (4*0x3800) // bytes offset

#define APROC_ADSP_IOBUF_INTBUF4_START (4*0x5e520) // bytes offset, paul record
#define APROC_ADSP_IOBUF_INTBUF4_END (36) // 0x6d0/0x30 = 0x24
#define APROC_ADSP_IOBUF_INTBUF4_CH (2)
#define APROC_ADSP_IOBUF_INTBUF4_SIZE (4 * 64 * 3 / 4) // bytes

#endif

/* IEC
*/
typedef struct {
    UINT32 u4CategoryCode; // category code
    UINT32 u4PcmRawInfo; // PCM/Raw Info
    UINT32 u4WordLength; // word length
    UINT32 u4SampleRate; // sample rate
    UINT32 u4AdditionalInfo; // additional information
    UINT32 u4CopyRight; // copyright
    UINT32 u4DigitalInfo; // digital
    UINT32 u4SourceNum; // source number
    UINT32 u4ChannelNum0; // channel number 0
    UINT32 u4ChannelNum1; // channel number 1
    UINT32 u4ClockAccuracy; // clock accuracy
} APROC_IEC_CHANNELSTATUS_T;

#ifdef CC_AUD_ARM_RENDER
typedef struct {
    AUD_DEC_STREAM_FROM_T _aeInSrc[AUD_DEC_NUM];
    AUD_DEC_CMD_T eCmd;
} APROC_MW_CTRL_T;    
#endif

#define AUD_DBG_APROC_PTS           ((UINT16) (1<<(UINT16)0))
#define AUD_DBG_APROC_UPDATE_STC    ((UINT16) (1<<(UINT16)1))

#endif //defined(CC_AUD_APOLLO_SUPPORT) || defined(CC_AUD_ARM_SUPPORT) //#A0002
/*Bass Management*/
    typedef enum{
        BMANG_SPKCFG_DISABLE=0,
        BMANG_SPKCFG_ENABLE=1,
    }BMANG_SPKCFG_T;

    typedef enum{
        BMANG_CHCFG_DISABLE=0,
        BMANG_CHCFG_ENABLE=1,
    }BMANG_CHCFG_T;
    
    typedef enum{
        BMANG_MODE_DISABLE=0,
        BMANG_MODE_ENABLE=1,
        BMANG_MODE_BYPASS_IIR=2,
    }BMANG_MODE_T;
    
    typedef enum{
        BMANG_SPKSZ_SMALL=0,
        BMANG_SPKSZ_LARGE=1,
        BMANG_SPKSZ_UNKNOWN,
    }BMANG_SPKSZ_T;

    typedef enum{
        BMANG_CHID_DUALMONO,
        BMANG_CHID_100,
        BMANG_CHID_200,
        BMANG_CHID_201,
        BMANG_CHID_210,
        BMANG_CHID_220,
        BMANG_CHID_221,
        BMANG_CHID_300,
        BMANG_CHID_301,
        BMANG_CHID_310,
        BMANG_CHID_320,
        BMANG_CHID_321,
        BMANG_CHID_UNKNOWN,
    }BMANG_CHID_T;
    
    typedef enum {
        BMANG_L_SPK = 0x0,    
        BMANG_R_SPK,
        BMANG_C_SPK,
        BMANG_LS_SPK,
        BMANG_RS_SPK,
        BMANG_LFE_SPK,
        BMANG_SPK_NUM
    } APROC_BMANG_SPK_T;
    
    typedef struct {
        BMANG_MODE_T CtrlFlag;
        BMANG_CHID_T ChCfg;
        UINT32 u4LFETrim;
        UINT32 u4Volume;
        BMANG_SPKSZ_T sSpkSize[BMANG_SPK_NUM];
        UINT32 u4CutFc;
        UINT32 u4LFE2CH;
    } APROC_DSP_BMANG_SET_T;
/*Bass Management End*/


typedef enum
{
    ADSP_BIN_MAIN,
    ADSP_BIN_DYNLD_AAC,        
    ADSP_BIN_DYNLD_DD,
    ADSP_BIN_DYNLD_DDC,
    ADSP_BIN_DYNLD_DDT,
    ADSP_BIN_DYNLD_DCV,
    ADSP_BIN_DYNLD_DDCO,
    ADSP2_BIN_MAIN,
    ADSP_BIN_AARM,
    ADSP_BIN_NUM
}   ADSP_BIN_T;

#ifdef ADSP_BIN_SUPPORT
typedef struct
{
    UINT32 u4BinAddr;//Virtual address of dsp binary without header
    UINT32 u4BinSize;
    CHAR    acName[32];
} ADSP_BIN_CFG_T;
#endif

typedef enum{
    E_CUST_AUD_NO_EVEMT=0,
    E_CUST_AUD_SOURCE_CHANGE,            //bPath
    E_CUST_AUD_FORMAT_CHANGE,            //bPath
    E_CUST_AUD_DELAY_CHANGE,
    E_CUST_AUD_VOLUME_CHANGE,            //It must be issued once the UI value is ready after the boot up.
    E_CUST_AUD_OUT_FORMAT_PRE_CHANGE,
    E_CUST_AUD_OUT_FORMAT_CHANGE, 
    E_CUST_AUD_SOUND_SETTING_CHANGE,     //It must be issued once the UI value is ready after the boot up.
    E_CUST_AUD_CH_MUTE_STATE_CHANGE,     //bPath 
    E_CUST_AUD_PLAY_STATE_CHANGE,        //bPath
    E_CUST_AUD_MIX_STATE_CHANGE,
    E_CUST_AUD_SERVICE_STATE_CHANGE,
    E_CUST_AUD_SUBWOOFER_STATE_CHANGE,
    E_CUST_AUD_MCK_STATE_CHANGE,
    //Request event
    E_CUST_AUD_SPEAKER_MUTE_REQUEST,
    E_CUST_AUD_SPEAKER_UNMUTE_REQUEST,
}E_CUST_AUD_EVENT;
extern void DRVCUST_SendAudEvent(E_CUST_AUD_EVENT eCustEvent, UINT8 u1Option);

#ifdef CC_MAPLE_CUST_DRV     
extern CUST_AUD_SOUND_SETTING_T gSoundSetting;
#endif

typedef enum
{
    AUD_DrvCust_AQ_Set_Init = 0,       
    AUD_DrvCust_AQ_Get_VolTal = 50,            
    AUD_DrvCust_AQ_Get_List,    
    AUD_DrvCust_AQ_Gen_Key,    
    AUD_DrvCust_AQ_Get_SVT_SP,
    AUD_DrvCust_AQ_Get_SVT_SPW,
    AUD_DrvCust_AQ_Get_SVT_HP,
    AUD_DrvCust_AQ_Get_SVT_LO,    
    AUD_DrvCust_AQ_Get_SVT_LOH,
    AUD_DrvCust_AQ_Get_SVT_ASW,    
    AUD_DrvCust_AQ_Get_SVT_DSW,
    AUD_DrvCust_AQ_Get_SVT_BT,
    AUD_DrvCust_AQ_Get_DRC,
    AUD_DrvCust_AQ_Get_LIMT,
    AUD_DrvCust_AQ_Get_SMT,    
    AUD_DrvCust_AQ_Get_SPEQ,
    AUD_DrvCust_AQ_Get_SLT,
    AUD_DrvCust_AQ_Get_SWS,
    AUD_DrvCust_AQ_Get_LOUDT,
    AUD_DrvCust_AQ_Get_FIR,
    AUD_DrvCust_AQ_Get_ADCT,
    AUD_DrvCust_AQ_Get_ACT,
    AUD_DrvCust_AQ_Get_ADT,
    AUD_DrvCust_AQ_Get_RSV,
    AUD_DrvCust_AQ_Get_DA_GEN_CFG,
    AUD_DrvCust_AQ_Get_DA_5760_CFG1,
    AUD_DrvCust_AQ_Get_DA_5760_CFG2,    
    AUD_DrvCust_AQ_Get_DA_5538_CFG,
    AUD_DrvCust_AQ_Get_DA_5538_BQ,
    AUD_DrvCust_AQ_Get_DA_5538_BT,
    AUD_DrvCust_AQ_Get_DA_5538_LOUD,
    AUD_DrvCust_AQ_Get_DA_5538_DRC,

    // Driver use
    AUD_DrvCust_AQ_Get_DA_5760_SET,
    
    AUD_DrvCust_AQ_OP_Num
} AUD_DrvCust_AQ_OP_T;  //CC_AUD_AQ_IN_DRVCUST

typedef struct
{
    UINT8 u1Fs;  
    UINT8 u1MmMode;
    UINT8 u1TvPos;
    UINT8 u1Input;    
    UINT8 u1SoundMode;        
    UINT8 u1SoundBooster;
    UINT8 u1WirelessWoofer;
    UINT8 u1SurroundOnOff;    
    UINT8 u1AutoOnOff;
} AUD_DrvCust_AQ_Key_T;


#define AQ_PARAM_ANY 0xF
typedef enum {
    AQ_FS_DONT=0,
    AQ_FS_48KHZ=1,
    AQ_FS_96KHZ=2,
} AQ_FS;

typedef enum {
    AQ_MM_MEASURE_MODE_DONT=0,
    AQ_MM_MEASURE_MODE_0=1,
    AQ_MM_MEASURE_MODE_1=2,
} AQ_MM_MEASURE_MODE;

typedef enum {
    AQ_TV_POSITION_DONT=0,
    AQ_TV_POSITION_TABLE_STAND=1,
    AQ_TV_POSITION_WALL_MOUNT=2,
} AQ_TV_POSITION;

typedef enum {
    AQ_INPUT_DONT=0,
    AQ_INPUT_ANALOG_RF=1,
    AQ_INPUT_DIGITAL_RF=2,
    AQ_INPUT_HDMI=3,
    AQ_INPUT_COMPOSITE=4,
    AQ_INPUT_COMPONENT=5,
    AQ_INPUT_PC=6,
    AQ_INPUT_VIDEO_APP=7,
    AQ_INPUT_PHOTO=8,
    AQ_INPUT_JELLYFISH=9,
    AQ_INPUT_SKYPE=10,
    AQ_INPUT_MIRACAST=11,
    AQ_INPUT_GENERAL_APP=12,
    AQ_INPUT_RESERVED1=13,
    AQ_INPUT_RESERVED2=14,
    AQ_INPUT_RESERVED3=15,
} AQ_INPUT;

typedef enum {
    AQ_SOUND_MODE_DONT=0,
    AQ_SOUND_MODE_STANDARD=1,
    AQ_SOUND_MODE_CINEMA=2,
    AQ_SOUND_MODE_MUSIC=3,
    AQ_SOUND_MODE_DEMO=4,
    AQ_SOUND_MODE_FOOTBALL=5,
    AQ_SOUND_MODE_STANDARD_S0A0,
    AQ_SOUND_MODE_STANDARD_S0A1,
    AQ_SOUND_MODE_STANDARD_S1A0,
    AQ_SOUND_MODE_STANDARD_S1A1,
    AQ_SOUND_MODE_DEMO_S0A0, //10
    AQ_SOUND_MODE_DEMO_S0A1,
    AQ_SOUND_MODE_DEMO_S1A0,
    AQ_SOUND_MODE_DEMO_S1A1, //13
} AQ_SOUND_MODE;

typedef enum {
    AQ_WIRELESS_SUBWOOFER_MODE_DONT=0,
    AQ_WIRELESS_SUBWOOFER_MODE_OFF=1,//Headphone/AudioOut=(Headphone OR Line-out OR (Subwoofer AND WirelessSW dongle not connected))
    AQ_WIRELESS_SUBWOOFER_MODE_ON=2,//Headphone/AudioOut=Subwoofer AND WirelessSW dongle connected
} AQ_WIRELESS_SUBWOOFER_MODE;

typedef enum {
    AQ_SURROUND_DONT=0,
    AQ_SURROUND_OFF=1,
    AQ_SURROUND_ON=2,
    AQ_SURROUND_AUTO=3,
} AQ_SURROUND;

typedef enum {
    AQ_CHANNEL_CONF_MONO=0,
    AQ_CHANNEL_CONF_DUAL_MONO=1,
    AQ_CHANNEL_CONF_STEREO=2,
    AQ_CHANNEL_CONF_2_0_1=3,
    AQ_CHANNEL_CONF_2_1_1=4,
    AQ_CHANNEL_CONF_2_1_0=5,
    AQ_CHANNEL_CONF_2_2_0=6,
    AQ_CHANNEL_CONF_2_2_1=7,
    AQ_CHANNEL_CONF_3_0_0=8,
    AQ_CHANNEL_CONF_3_0_1=9,
    AQ_CHANNEL_CONF_3_1_0=10,
    AQ_CHANNEL_CONF_3_1_1=11,
    AQ_CHANNEL_CONF_3_2_0=12,
    AQ_CHANNEL_CONF_3_2_1=13,
} AQ_CHANNEL_CONF;

typedef enum {
    AQ_BT_AUDIO_DONT=0,
    AQ_BT_AUDIO_NOT_CONNECTED=1,
    AQ_BT_AUDIO_CONNECTED=2,
    AQ_BT_TYPE_SPEAKER,
    AQ_BT_TYPE_HEADPHONE,
} AQ_BT_AUDIO;

typedef enum {
    AQ_PICTURE_MODE_DONT=0,
    AQ_PICTURE_MODE_GAME_GRAPHICS=1,
    AQ_PICTURE_MODE_OTHER=2,
} AQ_PICTURE_MODE;

typedef enum {
    AQ_FOOTBALL_NARRATION_CANCEL_DONT=0,
    AQ_FOOTBALL_NARRATION_CANCEL_OFF=1,
    AQ_FOOTBALL_NARRATION_CANCEL_ON=2,
} AQ_FOOTBALL_NARRATION_CANCEL;

typedef enum {
    AQ_BOOSTER_ON =0,
    AQ_BOOSTER_OFF=1,
} AQ_BOOSTER_SET;

typedef enum {
    AQ_CLEAR_AUDIO_PLUS_DONT=0,
    AQ_CLEAR_AUDIO_PLUS_OFF=1,//ClearAudio+ Off
    AQ_CLEAR_AUDIO_PLUS_ON=2,//normal mode
} AQ_CLEAR_AUDIO_PLUS;

extern INT32 DrvCust_AQ_SetGet(AUD_DrvCust_AQ_OP_T eType, void* arg1, void* arg2, UINT32 arg3, UINT32 arg4);

//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------

#define DEC_NUM_VALIDATE(NUM)   \
    if (NUM > AUD_DEC_NUM)      \
    {                           \
        ASSERT(0);              \
    }                           \
     
//---------------------------------------------------------------------------
// Inter-file functions
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Public functions
//---------------------------------------------------------------------------
extern const CHAR* AUD_EnumToName(const AUD_ENUM_TO_NAME_T *eTable, UINT32 u4Size, UINT32 u4Value); 

//-----------------------------------------------------------------------------
// AUD_IsDecoderPlay
//
/** Brief of AUD_IsDecoderPlay.
 *  Details of AUD_IsDecoderPlay (optional).
 *
 *  @param      u1DecId          Decoder ID (AUD_DEC_MAIN or AUD_DEC_AUX)
 *
 *  @retval     TRUE
 *  @retval     FALSE
 */
//-----------------------------------------------------------------------------
extern BOOL AUD_IsDecoderPlay(UINT8 u1DspId, UINT8 u1DecId);

//-----------------------------------------------------------------------------
// AUD_IsDecoderPlay
//
/** Brief of AUD_IsDecoderRealPlay.
 *  Details of AUD_IsDecoderRealPlay (optional).
 *
 *  @param      u1DecId          Decoder ID (AUD_DEC_MAIN or AUD_DEC_AUX)
 *
 *  @retval     TRUE
 *  @retval     FALSE
 */
//-----------------------------------------------------------------------------
BOOL AUD_IsDecoderRealPlay(UINT8 u1DspId, UINT8 u1DecId);


//-----------------------------------------------------------------------------
// AUD_IsDSPRealPlay
//
/** Brief of AUD_IsDSPRealPlay.
 *  Details of AUD_IsDSPRealPlay (optional).
 *
 *  @param      u1DecId          Decoder ID (AUD_DEC_MAIN or AUD_DEC_AUX)
 *
 *  @retval     TRUE
 *  @retval     FALSE
 */
//-----------------------------------------------------------------------------
BOOL AUD_IsDSPRealPlay(UINT8 u1DspId, UINT8 u1DecId);


//-----------------------------------------------------------------------------
// AUD_IsDecoderStop
//
/** Brief of AUD_IsDecoderStop.
 *  Details of AUD_IsDecoderStop (optional).
 *
 *  @param      u1DecId          Decoder ID (AUD_DEC_MAIN or AUD_DEC_AUX)
 *
 *  @retval     TRUE
 *  @retval     FALSE
*/
//-----------------------------------------------------------------------------
BOOL AUD_IsDecoderStop(UINT8 u1DspId, UINT8 u1DecId);

//-----------------------------------------------------------------------------
// AUD_CommandDone
//
/** Audio notify command check.
 *
 *  @param  ucDecId         Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
 *  @param  eAudDecCmd      audio command.
 *  @param  u4Result        audio status result.
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void AUD_CommandDone(UINT8 u1DspId, UINT8 u1DecId,  AUD_DEC_CMD_T eAudDecCmd, UINT32 u4Result);

//-----------------------------------------------------------------------------
// AUD_WaitCommandDone
//
/** Audio wait notify command check.
 *
 *  @param  ucDecId         Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
 *  @param  eAudDecCmd      audio command.
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void AUD_WaitCommandDone(UINT8 u1DspId, UINT8 u1DecId, AUD_DEC_CMD_T eAudDecCmd);

extern void AUD_DualDecWaitCommandDone(AUD_DUAL_DEC_CMD_T eDualDecCmd, BOOL fgTimeOut);
extern void AUD_DualDecCommandDone(AUD_DUAL_DEC_CMD_T eAudDecCmd, UINT32 u4Result);

//-----------------------------------------------------------------------------
// AUD_Init
//
/** Aud driver initialization. This function will create driver thread and let
 *  DSP go on.
 *
 *  @retval AUD_OK    Success
 *  @retval AUD_FAIL  Fail
 */
//-----------------------------------------------------------------------------
extern INT32 AUD_Init(void);

//-----------------------------------------------------------------------------
// AUD_IsAudInit
//
/** Query audio initialization status.
 *
 *  @retval TRUE    Audio is initialized
 *  @retval FALSE   Audio is not initialized
 */
//-----------------------------------------------------------------------------
extern BOOL AUD_IsAudInit(void);
extern BOOL AUD_IsAudInitOK(void);

#if 0   // Unused
//-----------------------------------------------------------------------------
// AUD_GetDecType
//
/** Get audio decoder type.
 *
 *  @param      u1DecId        Decoder ID (AUD_DEC_MAIN or AUD_DEC_AUX)
 *  @param      peStreamFrom   Stream source
 *  @param      prDecType      Stream format
 *
 *  @retval AUD_OK    Success
 *  @retval AUD_FAIL  Fail
 */
//-----------------------------------------------------------------------------
extern INT32 AUD_GetDecType(UINT8 u1DecId, AUD_DEC_STREAM_FROM_T* peStreamFrom, AUD_FMT_T* prDecType);
#endif

//-----------------------------------------------------------------------------
// AUD_GetHDMIDecodeType
//
/** Brief of AUD_GetHDMIDecodeType.
 *  Details of AUD_GetHDMIDecodeType (optional).
 *
 *  @param      u1DecId          Decoder ID (AUD_DEC_MAIN or AUD_DEC_AUX)
 *  @param      peStreamFrom     Stream source
 *  @param      peDecType        Stream format
 */
//-----------------------------------------------------------------------------
extern void AUD_GetHDMIDecodeType(UINT8 u1DecId, AUD_DEC_STREAM_FROM_T* peStreamFrom, AUD_FMT_T* peDecType);


//-----------------------------------------------------------------------------
// AUD_SetDecMemBuffer
//
/** Brief of AUD_SetDecMemBuffer.
 *  Details of AUD_SetDecMemBuffer (optional).
 *
 *  @param      u1DecId           Decoder ID (AUD_DEC_MAIN or AUD_DEC_AUX)
 *  @param      prDecMemBuffer    Memory buffer
 *
 *  @retval AUD_OK    Success
 *  @retval AUD_FAIL  Fail
 */
//-----------------------------------------------------------------------------
extern INT32 AUD_SetDecMemBuffer(UINT8 u1DspId, UINT8 u1DecId, const MEM_BUFFER_INFO_T * prDecMemBuffer);


//-----------------------------------------------------------------------------
// AUD_SetDecType
//
/** Set audio decoder type.
 *
 *  @param      u1DecId        Decoder ID (AUD_DEC_MAIN or AUD_DEC_AUX)
 *  @param      eStreamFrom    Stream source
 *  @param      eDecFmt        Stream format
 *
 *  @retval AUD_OK    Success
 *  @retval AUD_FAIL  Fail
 */
//-----------------------------------------------------------------------------
extern INT32 AUD_SetDecType(UINT8 u1DspId, UINT8 u1DecId,  AUD_DEC_STREAM_FROM_T eStreamFrom, AUD_FMT_T eDecFmt);

extern INT32 AUD_SetSourceFrom(UINT8 u1DecId,  AUD_DEC_STREAM_FROM_T eStreamFrom);

#ifdef CC_AUD_DDI
extern UINT8 AUD_GetResource(AUD_DEC_STREAM_FROM_T eStreamFrom, BOOL fgIsAD, BOOL fgForRec);
extern void AUD_SetGstId(UINT8 u1DecId);
extern UINT8 AUD_GetGstId(void);
extern void AUD_SetGstEncFlag(BOOL u1EncFlag);
extern UINT8 AUD_GetGstEncFlag(void);
#endif
extern INT32 _AudDumpAudioData (const INT8 *fileName, const INT8 *buf, size_t count);
//-----------------------------------------------------------------------------
// AUD_DSPCmdPlay
//
/** Brief of AUD_DSPCmdPlay.
 *  Issue play command to audio decoder, note that this is a synchronous play
 *  function
 *
 *  @param      u1DecId        Decoder ID (AUD_DEC_MAIN or AUD_DEC_AUX)
 *
 *  @retval AUD_OK    Success
 *  @retval AUD_FAIL  Fail
 */
//-----------------------------------------------------------------------------
extern INT32 AUD_DSPCmdPlay(UINT8 u1DspId, UINT8 u1DecId);

//-----------------------------------------------------------------------------
// AUD_DSPCmdPlayAsyn
//
/** Brief of AUD_DSPCmdPlayAsyn.
 *  Issue play command to audio decoder, note that this is a asynchronous play
 *  function. There will be a audio decoder notification when the command was done
 *
 *  @param      u1DecId        Decoder ID (AUD_DEC_MAIN or AUD_DEC_AUX)
 *
 *  @retval AUD_OK    Success
 *  @retval AUD_FAIL  Fail
 */
//-----------------------------------------------------------------------------
extern INT32 AUD_DSPCmdPlayAsyn(UINT8 u1DspId, UINT8 u1DecId);


//-----------------------------------------------------------------------------
// AUD_DSPCmdResume
//
/** Brief of AUD_DSPCmdResume.
 *  Details of AUD_DSPCmdResume (optional).
 *
 *  @param        u1DecId          Decoder ID (AUD_DEC_MAIN or AUD_DEC_AUX)
 *
 *  @retval AUD_OK    Success
 *  @retval AUD_FAIL  Fail
 */
//-----------------------------------------------------------------------------
extern INT32 AUD_DSPCmdResume(UINT8 u1DspId, UINT8 u1DecId);



//-----------------------------------------------------------------------------
// AUD_DSPCmdPause
//
/** Brief of AUD_DSPCmdPause.
 *  Details of AUD_DSPCmdPause (optional).
 *
 *  @param        u1DecId          Decoder ID (AUD_DEC_MAIN or AUD_DEC_AUX)
 *
 *  @retval AUD_OK    Success
 *  @retval AUD_FAIL  Fail
 */
//-----------------------------------------------------------------------------
extern INT32 AUD_DSPCmdPause(UINT8 u1DspId, UINT8 u1DecId);

#if 0   // Unused
//-----------------------------------------------------------------------------
// AUD_DSPCmdPauseAsyn
//
/** Brief of AUD_DSPCmdPauseAsyn.
 *  Details of AUD_DSPCmdPauseAsyn (optional).
 *
 *  @param[in]        u1DecId          Decoder ID (AUD_DEC_MAIN or AUD_DEC_AUX)
 *
 *  @retval AUD_OK    Success
 *  @retval AUD_FAIL  Fail
 */
//-----------------------------------------------------------------------------
extern INT32 AUD_DSPCmdPauseAsyn(UINT8 u1DspId, UINT8 u1DecId);
#endif

//-----------------------------------------------------------------------------
// AUD_DSPCmdStop
//
/** Brief of AUD_DSPCmdStop.
 *  Details of AUD_DSPCmdStop (optional).
 *
 *  @param        u1DecId          Decoder ID (AUD_DEC_MAIN or AUD_DEC_AUX)
 *
 *  @retval AUD_OK    Success
 *  @retval AUD_FAIL  Fail
 */
//-----------------------------------------------------------------------------
extern INT32 AUD_DSPCmdStop(UINT8 u1DspId, UINT8 u1DecId);


//-----------------------------------------------------------------------------
// AUD_DSPCmdStopAsyn
//
/** Brief of AUD_DSPCmdStopAsyn.
 *  Details of AUD_DSPCmdStopAsyn (optional).
 *
 *  @param        u1DecId          Decoder ID (AUD_DEC_MAIN or AUD_DEC_AUX)
 *
 *  @retval AUD_OK    Success
 *  @retval AUD_FAIL  Fail
 */
//-----------------------------------------------------------------------------
extern INT32 AUD_DSPCmdStopAsyn(UINT8 u1DspId, UINT8 u1DecId);

#if 0   // Unused
//-----------------------------------------------------------------------------
// AUD_DSPCmdReset
//
/** Brief of AUD_DSPCmdReset.
 *  Details of AUD_DSPCmdReset (optional).
 *
 *  @param        u1DecId          Decoder ID (AUD_DEC_MAIN or AUD_DEC_AUX)
 *
 *  @retval AUD_OK    Success
 *  @retval AUD_FAIL  Fail
 */
//-----------------------------------------------------------------------------
extern INT32 AUD_DSPCmdReset(UINT8 u1DspId, UINT8 u1DecId);
#endif

//-----------------------------------------------------------------------------
// AUD_DSPCmdResetAsyn
//
/** Brief of AUD_DSPCmdResetAsyn.
 *  Details of AUD_DSPCmdResetAsyn (optional).
 *
 *  @param        u1DecId          Decoder ID (AUD_DEC_MAIN or AUD_DEC_AUX)
 *
 *  @retval AUD_OK    Success
 *  @retval AUD_FAIL  Fail
 */
//-----------------------------------------------------------------------------
extern INT32 AUD_DSPCmdResetAsyn(UINT8 u1DspId, UINT8 u1DecId);


//-----------------------------------------------------------------------------
// AUD_RegNotifyFunc
//
/** Register audio notify function.
 *
 *  @param     pu4Handler   Notify function handler
 *  @param     pfNfyFunc    Notify function pointer
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
extern BOOL AUD_RegNotifyFunc(UINT32* pu4Handler, AudDecNfyFct pfNfyFunc);



//-----------------------------------------------------------------------------
// AUD_DeRegNotifyFunc
//
/** Deregister audio notify function.
 *
 *  @param  u4Handler   Handler
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
extern BOOL AUD_DeRegNotifyFunc(UINT32 u4Handler);

//-----------------------------------------------------------------------------
// AUD_GetDecodeType
//
/** Brief of AUD_GetDecodeType.
 *  Details of AUD_GetDecodeType (optional).
 *
 *  @param      u1DecId          Decoder ID (AUD_DEC_MAIN or AUD_DEC_AUX)
 *  @param      peStreamFrom     Stream source
 *  @param      peDecType        Stream format
 */
//-----------------------------------------------------------------------------
extern void AUD_GetDecodeType(UINT8 u1DspId, UINT8 u1DecId, AUD_DEC_STREAM_FROM_T* peStreamFrom, AUD_FMT_T* peDecType);

//-----------------------------------------------------------------------------
// AUD_GetStreamFrom
//
/** Brief of AUD_GetStreamFrom.
 *  Details of AUD_GetStreamFrom (optional).
 *
 *  @param      u1DecId          Decoder ID (AUD_DEC_MAIN or AUD_DEC_AUX)
 *  @param      peStreamFrom     Stream source
 */
//-----------------------------------------------------------------------------
extern void AUD_GetStreamFrom(UINT8 u1DspId, UINT8 u1DecId, AUD_DEC_STREAM_FROM_T* peStreamFrom);

#ifdef CC_AUD_DDI
extern void AudLGSEFN000(UINT8 fNo, UINT32 pParamsPtr, UINT16 noParam, UINT8 dataOption, UINT8 varOption);
extern void AudGetLGSEFN010Out(UINT32* pParam);
#endif

extern void AUD_SetUserCommmand(UINT32 u4CmdType, UINT32 u4Index, UINT32 u4Arg1, UINT32 u4Arg2, UINT32 u4Arg3, UINT32 u4Arg4);
//-----------------------------------------------------------------------------
// AUD_SetAudClip
//
/** Brief of AUD_SetAudClip.
 *  Details of AUD_SetAudClip (optional).
 *
 *  @param      u1DecId          Decoder ID (AUD_DEC_MAIN or AUD_DEC_AUX)
 *  @param      peAudClip
 */
//-----------------------------------------------------------------------------
extern void AUD_SetAudClip(UINT8 u1DspId, UINT8 u1DecId, AUD_CLIP_TYPE_T * peAudClip);

//-----------------------------------------------------------------------------
// AUD_GetAudioPTS
//
/** Brief of AUD_GetAudioPTS.
 *  Details of AUD_GetAudioPTS (optional).
 *
 *  @param      u1DecId          Decoder ID (AUD_DEC_MAIN or AUD_DEC_AUX)
 *  @param      prAudioPts       AudioPts
 */
//-----------------------------------------------------------------------------
extern void AUD_GetAudioPTS(UINT8 u1DecId, UINT32* rAudioPts);


//-----------------------------------------------------------------------------
// AUD_PlayAudClip
//
/** Brief of AUD_PlayAudClip.
 *  Details of AUD_PlayAudClip (optional).
 *
 *  @param      u1DecId          Decoder ID (AUD_DEC_MAIN or AUD_DEC_AUX)
 *  @param      u4Channel
 *  @param      fgPlayTestTone
 */
//-----------------------------------------------------------------------------
extern void AUD_PlayAudClip(UINT8 u1DspId, UINT8 u1DecId, UINT32 u4Channel, BOOL fgPlayTestTone);

//-----------------------------------------------------------------------------
// AUD_PlayAudClipWithRPN
//
/** Brief of AUD_PlayAudClipWithRPN.
 *  Details of AUD_PlayAudClipWithRPN (optional).
 *
 *  @param      u1DecId          Decoder ID (AUD_DEC_MAIN or AUD_DEC_AUX)
 *  @param      u4Channel
 *  @param      ReaptNumber
 *  @param      fgPlayTestTone
 */
extern void AUD_PlayAudClipWithRPN(UINT8 u1DspId, UINT8 u1DecId, UINT32 u4Channel, UINT32 ReaptNumber, BOOL fgPlayTestTone);



//-----------------------------------------------------------------------------
// AUD_StopAudClip
//
/** Brief of AUD_StopAudClip.
 *  Details of AUD_StopAudClip (optional).
 *
 *  @param      u1DecId        Decoder ID (AUD_DEC_MAIN or AUD_DEC_AUX)
 */
//-----------------------------------------------------------------------------
extern void AUD_StopAudClip(UINT8 u1DspId, UINT8 u1DecId);
extern void Aud_SetAudClipVolume(UINT8 u1DspId, UINT8 u1DecId, UINT32 u4Channel, UINT8 u1Volume);
extern void AUD_DeleteAudClip(void);

//-----------------------------------------------------------------------------
// AUD_AvInputMuxSel
//
/** Brief of AUD_AvInputMuxSel.
 *  Details of AUD_AvInputMuxSel (optional).
 *
 *  @param  eInputId   AV component id
 *
 */
//-----------------------------------------------------------------------------
extern void AUD_AvInputMuxSel(AUD_INPUT_ID_T eInputId);

//-----------------------------------------------------------------------------
// AUD_AvInputMuxSelByAD
//
/** Brief of AUD_AvInputMuxSelByAD.
 *  Details of AUD_AvInputMuxSelByAD(optional).
 *
 *  @param  UINT8 eADNum
 *
 */
//-----------------------------------------------------------------------------
extern void AUD_AvInputMuxSelByAD(UINT8 eADNum);


//-----------------------------------------------------------------------------
// AUD_SetCtrlType
//
/** Brief of AUD_SetCtrlType.
 *  Details of AUD_SetCtrlType (optional).
 *
 *  @param  u4MWSetCtrlType
 *
 */
//-----------------------------------------------------------------------------
extern void AUD_SetCtrlType(UINT8 u1DspId,  UINT8 u1DecId, UINT32 u4MWSetCtrlType);

//-----------------------------------------------------------------------------
// AUD_SetCtrlCmd
//
/** Brief of AUD_SetCtrlCmd.
 *  Details of AUD_SetCtrlCmd (optional).
 *
 *  @param  eMWSetCtrlCmd
 *
 */
//-----------------------------------------------------------------------------
extern void AUD_SetCtrlCmd(UINT8 u1DspId,  UINT8 u1DecId, AUD_DEC_CMD_T eMWSetCtrlCmd);

//-----------------------------------------------------------------------------
// AUD_Mute
//
/** Brief of AUD_Mute.
 *  Details of AUD_Mute (optional).
 *
 *  @param      u1DecId          Decoder ID (AUD_DEC_MAIN or AUD_DEC_AUX)
 *
 *  @retval AUD_OK    Success
 *  @retval AUD_FAIL  Fail
 */
//-----------------------------------------------------------------------------
extern INT32 AUD_Mute(UINT8 u1DecId);

#if 0   // Unused
//-----------------------------------------------------------------------------
// AUD_DtvLock
//
/** DTV audio lock check
 *
 *  @param      u1DecId          Decoder ID (AUD_DEC_MAIN or AUD_DEC_AUX)
 */
//-----------------------------------------------------------------------------
extern BOOL AUD_DtvLock(UINT8 u1DecId);
#endif

//-----------------------------------------------------------------------------
// AUD_SetAvSynMode
//
/** DTV audio sync check
 *
 *  @param      u1DecId          Decoder ID (AUD_DEC_MAIN or AUD_DEC_AUX)
 *  @param      eSynMode       Sync mode, 0:Freerun 1: Slave, 2:Master
 */
//-----------------------------------------------------------------------------
extern void AUD_SetAvSynMode(UINT8 u1DecId, AV_SYNC_MODE_T eSynMode);

extern AUD_MM_MODE_T AUD_GetMultimediaMode(UINT8 u1DecId);

//-----------------------------------------------------------------------------
// AUD_SetMultimediaMode
//
/** Set Audio Multimedia Mode
 *
 *  @param      u1DecId          Decoder ID (AUD_DEC_MAIN)
 *  @param      eMultimediaMode  Multimedia mode, 0: Normal 1: Karaoke
 *                                                2: Karaoke mic pass
 */
//-----------------------------------------------------------------------------
extern void AUD_SetMultimediaMode(UINT8 u1DecId, AUD_MM_MODE_T eMultimediaMode);

//-----------------------------------------------------------------------------
// AUD_SetAdMode
//
/** Set AD Mode
 *
 *  @param      fgAdMode  0: AD will not be turned on in this connection, 1: AD will be turned on
 */
//-----------------------------------------------------------------------------
extern void AUD_SetAdMode(BOOL fgAdMode);

//-----------------------------------------------------------------------------
// AUD_UnMute
//
/** Brief of AUD_UnMute.
 *  Details of AUD_UnMute (optional).
 *
 *  @param      u1DecId          Decoder ID (AUD_DEC_MAIN or AUD_DEC_AUX)
 *
 *  @retval AUD_OK    Success
 *  @retval AUD_FAIL  Fail
 */
//-----------------------------------------------------------------------------
extern INT32 AUD_UnMute(UINT8 u1DecId);
extern void AUD_DrvSetMMAudioOnly(UINT8 u1DecId, BOOL fgMMAudioOnly);
#ifdef CC_ENABLE_AOMX
extern BOOL AUD_DrvGetMMAudioOnly(UINT8 u1DecId);
#endif
#ifdef CC_AUD_DDI
extern BOOL AUD_GetMMAudioOnly(UINT8 u1DecId);
#endif

extern void AUD_SetAdFadeEnable(BOOL bEnable);
extern void AUD_SetAdPanEnable(BOOL bEnable);
extern void AUD_PanFadePesUpdateEn(BOOL bEnable);

//-----------------------------------------------------------------------------
// AUD_SendAudioPes
//
/** Brief of AUD_SendAudioPes.
 *  This is the callback function for dmx inform audio driver when retrieving
 *  audio PES
 *
 *  @param      prPes          PES information
 *
 *  @retval AUD_OK    Success
 *  @retval AUD_FAIL  Fail
 */
//-----------------------------------------------------------------------------
extern BOOL AUD_SendAudioPes(const DMX_AUDIO_PES_T* prPes);
#ifdef CC_ENABLE_AOMX
extern BOOL GST_SendAudioPes(const GST_AUDIO_PES_T rPes);     //koro gst
extern void GST_StartSTC(UINT8 stcId);
extern void GST_StopSTC(UINT8 stcId);
extern void GST_SetSTCVal(UINT8 stcId, UINT64 u8Stc);
extern UINT64 GST_GetSTCVal(UINT8 stcId);
extern void GST_AOMX_DumpAudioData(UINT8 u1DecId, const char *addr, UINT32 size, BOOL fgnew);
#endif

//-----------------------------------------------------------------------------
// AUD_PcmSetting
//
/** PCM format setting
 *
 *  @param      u1DecId          Decoder ID (AUD_DEC_MAIN or AUD_DEC_AUX)
 *  @param      prPcmSetting
 */
//-----------------------------------------------------------------------------
extern void AUD_PcmSetting(UINT8 u1DspId, UINT8 u1DecId, const AUD_PCM_SETTING_T* prPcmSetting);

#ifdef CC_HDMI_PCM_MULT_CHANNEL
//-----------------------------------------------------------------------------
// AUD_LineInChNumSetting
//
/** Line-In Channel Number Setting
 *
 *  @param      u1LineInChNum    Line-In Channel Number Setting
 *  @param      *u4Format        Default Line Module Configure
 */
//-----------------------------------------------------------------------------
extern void AUD_LineInChNumSetting(UINT8 u1DecId, UINT8 u1LineInChNum);
//-----------------------------------------------------------------------------
// AUD_LineInChNumFormat
//
/** Line-In Channel Number Format
 *
 *  @param      u1LineInChNum    Line-In Channel Number Setting
 *  @param      pu4Format        
 */
//-----------------------------------------------------------------------------
extern void AUD_LineInChNumFormat(UINT8 u1LineInChNum, UINT32* pu4Format);
///Drv_hdmi.h
extern UINT8 API_HDMI_Get_Aud_ChannelNum(void);
#endif

//-----------------------------------------------------------------------------
// AUD_FlacSetting
//
/** FLAC format setting
 *
 *  @param      u1DecId          Decoder ID (AUD_DEC_MAIN)
 *  @param      prFlacSetting
 *  paul_flac
 */
//-----------------------------------------------------------------------------
extern void AUD_FlacSetting(UINT8 u1DecID, AUD_FLAC_INFO_T* prFlacSetting);

//-----------------------------------------------------------------------------
/** AUD_DspFineTuneVolume
 *  Fine tune master volume/trim, and set it to DSP
 *  @param  u1DecId          Audio decoder id (0: first decoder, 1: second decoder).
 *  @param  eChannel  : 0: front left channel.
 *                               1: front right channel.
 *                               2: rear left channel.
 *                               3: rear right channel.
 *                               4: center channel.
 *                               5: sub woofer channel.
 *                               6: bypass left channel.
 *                               7: bypass right channel.
 *                               8: downmix left channel.
 *                               9: downmix right channel.
 *                             10: aux front left channel.
 *                             11: aux front right channel.
 *                             12: input source gain.
 *                             13: all channels    ( For master volume control ).
 *  @param  u4ShmValue       Volume (0x20000 is equal to 0 dBFs, max value: 0x7fffff)
 *
 *  @return void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspFineTuneVolume(UINT8 u1DecId, AUD_CH_T eChannel, UINT32 u4ShmValue);

//-----------------------------------------------------------------------------
/** AUD_DspGetFineTuneVolume
 *  Fine tune master volume/trim, and set it to DSP
 *  @param  u1DecId          Audio decoder id (0: first decoder, 1: second decoder).
 *  @param  eChannel  : 0: front left channel.
 *                               1: front right channel.
 *                               2: rear left channel.
 *                               3: rear right channel.
 *                               4: center channel.
 *                               5: sub woofer channel.
 *                               6: bypass left channel.
 *                               7: bypass right channel.
 *                               8: downmix left channel.
 *                               9: downmix right channel.
 *                             10: aux front left channel.
 *                             11: aux front right channel.
 *                             12: input source gain.
 *                             13: all channels    ( For master volume control ).
 *  @param  u4ShmValue       Volume (0x20000 is equal to 0 dBFs, max value: 0x7fffff)
 *
 *  @return void
 */
//-----------------------------------------------------------------------------
extern UINT32 AUD_DspGetFineTuneVolume(UINT8 u1DecId, AUD_CH_T eChannel);

//-----------------------------------------------------------------------------
/** AUD_DspVolumeInit
 *  Initialize all variables for volume control
 *
 *  @return void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspVolumeInit(void);

#ifdef CC_AUD_ARM_RENDER//CC_AUD_APROC_POST 
extern void AUD_AprocBMangInit(void);
#endif
//-----------------------------------------------------------------------------
/** AUD_DspEqCfgInit
 *  Initialize variables for equalizer preset configuration
 *
 *  @return void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspEqCfgInit(void);

#ifdef CC_AUD_SOUND_MODE
extern void AUD_DspPSMCfgInit(void);
extern void AUD_DspPostSoundModeChange(UINT8 u1DecId, UINT8 u1ModeIdx);
#endif
#ifdef CC_AUD_NEW_POST_MULTI_VS_SUPPORT
extern void AUD_DspVSMCfgInit(void);
#endif
#ifdef CC_AUD_VOLUME_OFFSET
extern void AUD_DspVolumeOffset(UINT8 u1DecId, UINT8 u1Index);
#endif
extern void AUD_DspClearSoundEnable(UINT8 u1DecId, BOOL fgEnable);
#ifdef CC_AUD_DSP_SUPPORT_CDNOTCH
extern void AUD_DspClearSoundCfg(UINT8 u1UserQIdx, UINT16 u2UserFc);
#endif
#if (defined(CC_SONY_2K14_FLASH_AQ) || defined(CC_MAPLE_CUST_DRV))
extern void AUD_DspSetTvPosition(UINT8 u1DecId,UINT8 u1Position);
#endif
extern void AUD_DspPostSoundMode(UINT8 u1DecId, UINT8 u1ModeIdx);
extern void AUD_DspSetPEQ(UINT8 u1DecId,UINT8 u1ModeIdx);
#ifdef CC_AUD_PEQ_BASS_TREBLE
extern void AUD_DspPEQBassTreble(UINT8 u1DecId,BOOL isTreble,UINT8 u1Level);
#endif

#ifdef CC_AUD_SUPPORT_AMP_INIT_DETECT_FUNCTION
extern void AUD_DspSetRegDetection(BOOL fgRegDetection);
extern BOOL AUD_DspGetRegDetection(void);
#endif

//-----------------------------------------------------------------------------
/** AUD_DspPostSoundModeStart
 *  Indicate sound mode start/end
 *
 *  @param  u1DecId          Audio decoder id (0: first decoder, 1: second decoder).
 *  @param  fgStart          start(TRUE), end(FALSE).
 *
 *  @return void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspPostSoundModeStart(UINT8 u1DecId, BOOL fgStart);

//-----------------------------------------------------------------------------
/** AUD_DspChDelayInit
 *  Initialize table for channel delay configuration
 *
 *  @return void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspChDelayInit(void);

#ifdef CC_AUD_SPIDF_LVL
//-----------------------------------------------------------------------------
/** AUD_DspSpdifLevel
 *  Change spdif level
 *
 *  @param u1Level
 *
 *  @return void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspSpdifLevel(UINT8 u1Level);
#endif

//-----------------------------------------------------------------------------
/** AUD_DspSpdifInit
 *  Initialize SPDIF configuration
 *
 *  @return void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspSpdifInit(void);

//-----------------------------------------------------------------------------
/** AUD_DspSpdifInit
 *  Initialize SPDIF-in configuration
 *
 *  @return void
 */
//-----------------------------------------------------------------------------
extern void AUD_SpdifinInit(void);

//-----------------------------------------------------------------------------
/** AUD_DspAsrcInit
 *  Initialize ASRC configuration
 *
 *  @return void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspAsrcInit(void);


//-----------------------------------------------------------------------------
// AUD_DspReverbEnable
//
/** turn on / off reverb control.
 *
 *  @param  u1DecId          Audio decoder id (0: first decoder, 1: second decoder).
 *  @param  fgEnable          "TRUE" for enable.
 *
 *  @return void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspReverbEnable(UINT8 u1DecId, BOOL fgEnable);


//-----------------------------------------------------------------------------
/** AUD_DspReverbMode
 *  load the predefined reverberation parameters sets.
 *
 *  @param  u1DecId Audio decoder id (0: first decoder, 1: second decoder).
 *  @param  bMode From mode 0 to mode 7. Mode 0 will turn the reverberation effect off.
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspReverbMode(UINT8 u1DecId, UINT8 u1Mode);


//-----------------------------------------------------------------------------
// AUD_DspChannelEqEnable
//
/** turn on /off the bass boost control
 *
 *  @param  u1DecId         Audio decoder id (0: first decoder, 1: second decoder).
 *  @param  fgEnable          "TRUE" for enable.
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspChannelEqEnable(UINT8 u1DecId, BOOL fgEnable);
extern BOOL AUD_DspGetChannelEqEnable(UINT8 u1DecId);

//-----------------------------------------------------------------------------
// AUD_DspSpkSizeCfg
//
/** Set the bass management cut-off frequency
 *
 *  @param  u1DecId         Audio decoder id (0: first decoder, 1: second decoder).
 *  @param  u4SpkSize         u2SpkSize in xx Hz.
 *  @retval void
 */
extern void AUD_DspSpkSizeCfg(UINT8 u1DecId, UINT32 u4SpkSize);

#ifdef CC_S_SUBWOFFER_SUPPORT  ///CC_BASS_MANGER_INDEP_LPF
//-----------------------------------------------------------------------------
// AUD_DspSpkSizeLpfCfg
//
/** Set the bass management cut-off frequency
 *
 *  @param  u1DecId         Audio decoder id (0: first decoder, 1: second decoder).
 *  @param  u4SpkSize         u2SpkSize in xx Hz.
 *  @retval void
 */
extern void AUD_DspSpkSizeLpfCfg(UINT8 u1DecId, UINT32 u4SpkSize);
#endif

//-----------------------------------------------------------------------------
/** AUD_DspBassBoostEnable
 *  turn on /off the bass boost control.
 *
 *  @param  u1DecId Audio decoder id (0: first decoder, 1: second decoder).
 *  @param  fgEnable ADSP_ON / ADSP_OFF.
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspBassBoostEnable(UINT8 u1DecId, BOOL fgEnable) ;
extern BOOL AUD_DspGetBassBoostEnable(UINT8 u1DecId);

//-----------------------------------------------------------------------------
/** AUD_DspBassBoostGain
 *   set the bass boost gain value.
 *
 *  @param  u1DecId Audio decoder id (0: first decoder, 1: second decoder).
 *  @param  u1Level the setting value is from 0 to 28, level 14 means 0db. A large
 *                         number will boost the bass and a smaller number will attenuate the bass.
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspBassBoostGain(UINT8 u1DecId, UINT8 u1Level);


//-----------------------------------------------------------------------------
/** AUD_DspGetBassBoostGain
 *   get the bass boost gain value.
 *
 *  @param  u1DecId Audio decoder id (0: first decoder, 1: second decoder).
 *
 *  @retval bass gain value
 */
//-----------------------------------------------------------------------------
extern UINT32 AUD_DspGetBassBoostGain(UINT8 u1DecId);


//-----------------------------------------------------------------------------
/** AUD_DspTrebleBoostCtrl
 *   turn on / off treble boost control.
 *
 *  @param  u1DecId Audio decoder id (0: first decoder, 1: second decoder).
 *  @param  fgEnable  ADSP_ON / ADSP_OFF.
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspSuperBassEnable(UINT8 u1DecId, BOOL fgEnable);


//-----------------------------------------------------------------------------
// AUD_DspClearBoostEnable
//
/** turn on / off treble boost control
 *
 *  @param  u1DecId Audio decoder id (0: first decoder, 1: second decoder).
 *  @param  fgEnable  ADSP_ON / ADSP_OFF.
 *  @retval void
 */
extern void AUD_DspClearBoostEnable(UINT8 u1DecId, BOOL fgEnable);
extern BOOL AUD_DspGetClearBoostEnable(UINT8 u1DecId);

//-----------------------------------------------------------------------------
/** AUD_DspClearBoostGain
 *   set the clear boost gain value.
 *
 *  @param  u1DecId Audio decoder id (0: first decoder, 1: second decoder).
 *  @param  u1Level  range from 0 to 28, level 14 means 0db.
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspClearBoostGain(UINT8 u1DecId, UINT8 u1Level);


//-----------------------------------------------------------------------------
/** AUD_DspGetClearBoostGain
 *   get the treble boost gain value.
 *
 *  @param  u1DecId Audio decoder id (0: first decoder, 1: second decoder).
 *
 *  @retval treble gain value
 */
//-----------------------------------------------------------------------------
extern UINT32 AUD_DspGetClearBoostGain(UINT8 u1DecId);


#ifdef CC_AUD_FY12_LOUDNESS

//-----------------------------------------------------------------------------
/** AUD_DspBassBoostEnable
 *  turn on /off the bass boost control.
 *
 *  @param  u1DecId Audio decoder id (0: first decoder, 1: second decoder).
 *  @param  fgEnable ADSP_ON / ADSP_OFF.
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspBassBoostEnable2(UINT8 u1DecId, BOOL fgEnable) ;
extern BOOL AUD_DspGetBassBoostEnable2(UINT8 u1DecId);

//-----------------------------------------------------------------------------
/** AUD_DspBassBoostGain
 *   set the bass boost gain value.
 *
 *  @param  u1DecId Audio decoder id (0: first decoder, 1: second decoder).
 *  @param  u1Level the setting value is from 0 to 28, level 14 means 0db. A large
 *                         number will boost the bass and a smaller number will attenuate the bass.
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspBassBoostGain2(UINT8 u1DecId, UINT8 u1Level);


//-----------------------------------------------------------------------------
/** AUD_DspGetBassBoostGain
 *   get the bass boost gain value.
 *
 *  @param  u1DecId Audio decoder id (0: first decoder, 1: second decoder).
 *
 *  @retval bass gain value
 */
//-----------------------------------------------------------------------------
extern UINT32 AUD_DspGetBassBoostGain2(UINT8 u1DecId);


//-----------------------------------------------------------------------------
/** AUD_DspTrebleBoostCtrl
 *   turn on / off treble boost control.
 *
 *  @param  u1DecId Audio decoder id (0: first decoder, 1: second decoder).
 *  @param  fgEnable  ADSP_ON / ADSP_OFF.
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspSuperBassEnable2(UINT8 u1DecId, BOOL fgEnable);


//-----------------------------------------------------------------------------
// AUD_DspClearBoostEnable
//
/** turn on / off treble boost control
 *
 *  @param  u1DecId Audio decoder id (0: first decoder, 1: second decoder).
 *  @param  fgEnable  ADSP_ON / ADSP_OFF.
 *  @retval void
 */
extern void AUD_DspClearBoostEnable2(UINT8 u1DecId, BOOL fgEnable);
extern BOOL AUD_DspGetClearBoostEnable2(UINT8 u1DecId);

//-----------------------------------------------------------------------------
/** AUD_DspClearBoostGain
 *   set the clear boost gain value.
 *
 *  @param  u1DecId Audio decoder id (0: first decoder, 1: second decoder).
 *  @param  u1Level  range from 0 to 28, level 14 means 0db.
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspClearBoostGain2(UINT8 u1DecId, UINT8 u1Level);


//-----------------------------------------------------------------------------
/** AUD_DspGetClearBoostGain
 *   get the treble boost gain value.
 *
 *  @param  u1DecId Audio decoder id (0: first decoder, 1: second decoder).
 *
 *  @retval treble gain value
 */
//-----------------------------------------------------------------------------
extern UINT32 AUD_DspGetClearBoostGain2(UINT8 u1DecId);

#endif

//-----------------------------------------------------------------------------
/** AUD_DspEqEnable
 *  turn on / off equalizer control.
 *
 *  @param  u1DecId Audio decoder id (0: first decoder, 1: second decoder).
 *  @param  fgEnable  TRUE / FALSE.
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspEqEnable(UINT8 u1DecId, BOOL fgEnable);


//-----------------------------------------------------------------------------
// AUD_DspChEqBand
//
/** used to adjusting the equalizer's parameter for each channel
 *
 *  @param  u1DecId Audio decoder id (0: first decoder, 1: second decoder).
 *  @param  u1BandIdx  EQ band index
 *  @param  u1Level range is from 0 to 28
 *  @param  fgNotify  to indicate changing the parameter immediately or not
 *
 *  @retval void
 */
extern void AUD_DspChEqBand(UINT8 u1DecId, UINT8 u1BandIdx, UINT8 u1Level, BOOL fgNotify);



//-----------------------------------------------------------------------------
/** AUD_DspChEqLoadPreset
 *   load the predefined channel equalizer setting.
 *
 *  @param  u1DecId Audio decoder id (0: first decoder, 1: second decoder).
 *  @param  eEqType  the valid type number is from 0 to type 7.
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspChEqLoadPreset(UINT8 u1DecId, AUD_EQ_TYPE_T eEqType) ;


//-----------------------------------------------------------------------------
// AUD_DspChEqBandMode
//
/** used to adjusting the equalizer's parameter for each channel
 *
 *  @param  u1DecId Audio decoder id (0: first decoder, 1: second decoder).
 *  @param  u1BandIdx  EQ band index
 *  @param  u1Level range is from 0 to 28
 *  @param  fgNotify  to indicate changing the parameter immediately or not
 *
 *  @retval void
 */
 
#ifdef CC_AUD_ARM_RENDER
extern void AUD_DspChEqBandMode(UINT8 u1DecId, UINT8 u1BandIdx, INT8 i1Level);
#else
extern void AUD_DspChEqBandMode(UINT8 u1DecId, UINT8 u1BandIdx, UINT8 u1Level, BOOL fgNotify);
#endif


//-----------------------------------------------------------------------------
/** AUD_DspChEqLoadPresetChgMode
 *   load the predefined channel equalizer setting for mode changing.
 *
 *  @param  u1DecId Audio decoder id (0: first decoder, 1: second decoder).
 *  @param  eEqType  the valid type number is from 0 to type 7.
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspChEqLoadPresetChgMode(UINT8 u1DecId, AUD_EQ_TYPE_T eEqType) ;


//-----------------------------------------------------------------------------
/** AUD_DspChSetEqTable
 *   set the band gain of a specific equalizer.
 *
 *  @param  u1DecId  Audio decoder id (0: first decoder, 1: second decoder).
 *  @param  eEqType  the valid type number is from 0 to type 7.
 *  @param  eqValue  band gain, valid value from -20 ~ 20
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspChSetEqTable(UINT8 u1DecId, AUD_EQ_TYPE_T eEqType, INT8 eqValue[]);


//-----------------------------------------------------------------------------
/** AUD_DspChQryEqTable
 *   query the band gain of a specific equalizer.
 *
 *  @param  u1DecId  Audio decoder id (0: first decoder, 1: second decoder).
 *  @param  eEqType  the valid type number is from 0 to type 7.
 *  @param  eqValue  band gain, valid value from -20 ~ 20
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspChQryEqTable(UINT8 u1DecId, AUD_EQ_TYPE_T eEqType, INT8 eqValue[]);

//-----------------------------------------------------------------------------
/** AUD_DspGetEqType
 *   get the type of current equalizer.
 *
 *  @param  u1DecId  Audio decoder id (0: first decoder, 1: second decoder).
 *  @param  eEqType
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspGetEqType(UINT8 u1DecId, AUD_EQ_TYPE_T* eEqType);

//-----------------------------------------------------------------------------
/** AUD_DspSetLimiterMode
 *   set limiter mode
 *
 *  @param  u1Mode  limiter mode (0: off, 1: adaptive mode, 2: fixed mode).
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspSetLimiterMode(UINT8 u1Mode);

//-----------------------------------------------------------------------------
/** AUD_DspSetLimiterFlag
 *   set limiter flag
 *
 *  @param  u1Flag  limiter flag (0: off, 1: on)
 *                         bit 0: vol
 *                         bit 1: vsurr
 *                         bit 2: sbass
 *                         bit 3: EQ
 *                         bit 4: reverb
 *                         bit 5: speaker
 *                         bit 6: voice
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspSetLimiterFlag(UINT8 u1Flag);

//-----------------------------------------------------------------------------
/** AUD_DspSetLimiterThreshold
 *   set limiter threshold
 *
 *  @param  u4Thre  limiter threshold (0x0 ~ 0x7fffff)
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspSetLimiterThreshold(UINT32 u4Thre);

#ifdef CC_AUD_DRC_V20
//-----------------------------------------------------------------------------
/** AUD_DspSetLimiterAttackRate
 *   set limiter attack rate
 *
 *  @param  u4AttackRate  limiter attack rate (0x0 ~ 0x7fffff)
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspSetLimiterAttackRate(UINT32 u4AttackRate);

//-----------------------------------------------------------------------------
/** AUD_DspSetLimiterRleaseRate
 *   set limiter attack rate
 *
 *  @param  u4ReleaseRate  limiter release rate (0x0 ~ 0x7fffff)
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspSetLimiterRelaseRate(UINT32 u4ReleaseRate);

//-----------------------------------------------------------------------------
/** AUD_DspSetLimiterGainRatio
 *   set limiter gain ratio
 *
 *  @param  u4Ratio  limiter gain ratio (0x0 ~ 0x7fffff)
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspSetLimiterGainRatio(UINT32 u4Ratio);
#else

#ifdef CC_RATIO_LIMITER
extern void AUD_DspSetLimiterGainRatio(UINT32 u4Ratio);
#endif

#endif //CC_AUD_DRC_V20
//-----------------------------------------------------------------------------
/** AUD_DspGetLimiterConfig
 *   get limiter configuration
 *
 *  @param  *pu1Mode  mode
 *  @param  *pu2Flag  flag
 *  @param  *pu4Thre  threshold
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspGetLimiterConfig(UINT8* pu1Mode, UINT16* pu2Flag, UINT32* pu4Thre);

#ifdef CC_AUD_DRC_V20
//-----------------------------------------------------------------------------
/** AUD_DspGetLimiterConfig1
 *   get limiter configuration1
 *
 *  @param
 *  @param
 *  @param
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspGetLimiterConfig1(UINT32* pu4Attack, UINT32* pu4Release, UINT32* pu4Ratio);
#endif

//-----------------------------------------------------------------------------
/** AUD_DspPL2Config
 *  main processing routine for Prologic 2.
 *
 *  @param  uCtrl control
                         PL2CTRL_SWITCH          0.
                         PL2CTRL_MODE            1.
                         PL2CTRL_PANORAMA        2.
                         PL2CTRL_DIMENSION       3.
                         PL2CTRL_C_WIDTH         4.
 *  @param  uMode corresponding parameters for PL2 control (uCtrl).
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspPL2Config(UINT8 uCtrl, UINT8 uMode);


//-----------------------------------------------------------------------------
/** AUD_DspPL2ConfigInit
 *  initialization for PL2 configuration.
 *
 *  @param  u2PL2cfg W_PLIICONFIG
 *  @param  uMode W_PLIIMODE.
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspPL2ConfigInit(UINT16 u2PL2cfg, UINT16 u2PL2Mode) ;


//-----------------------------------------------------------------------------
/** AUD_DspAC3KaraMode
 *  setup for AC3 karaoke mode.
 *
 *  @param  u2KaraMode Karaoke mode
 *  @param  uDecIndx 0: first decoder 1: seconder decoder.
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspAC3KaraMode(UINT16 u2KaraMode, UINT8 uDecIndx) ;

//-----------------------------------------------------------------------------
/** AUD_DspGetAC3KaraMode
 *  setup for AC3 karaoke mode.
 *
 *  @param  uDecIndx 0: first decoder 1: seconder decoder.
 *  @retval u2KaraMode
 */
//-----------------------------------------------------------------------------
extern UINT16 AUD_DspGetAC3KaraMode(UINT8 uDecIndx) ;

//-----------------------------------------------------------------------------
/** AUD_DspAC3DualMono
 *  setup for AC3 dual mono mode.
 *
 *  @param  u2DMMode dual mono mode
 *  @param  uDecIndx 0: first decoder 1: seconder decoder.
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspAC3DualMono(UINT16 u2DMMode, UINT8 uDecIndx) ;

//-----------------------------------------------------------------------------
/** AUD_DspGetAC3DualMono
 *  setup for AC3 dual mono mode.
 *
 *  @param  u2DMMode dual mono mode
 *  @param  uDecIndx 0: first decoder 1: seconder decoder.
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern UINT16 AUD_DspGetAC3DualMono(UINT8 uDecIndx) ;

//-----------------------------------------------------------------------------
/** AUD_DspAC3CompMode
 *  setup for compression mode.
 *
 *  @param  u2CompMode compression mode.
 *  @param  uDecIndx 0: first decoder 1: seconder decoder.
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspAC3CompMode(UINT16 u2CompMode, UINT8 uDecIndx) ;


//-----------------------------------------------------------------------------
/** AUD_DspGetAC3CompMode
 *  setup for compression mode.
 *
 *  @param  uDecIndx 0: first decoder 1: seconder decoder.
 *  @retval u2CompMode compression mode.
 */
//-----------------------------------------------------------------------------
extern UINT16 AUD_DspGetAC3CompMode(UINT8 uDecIndx) ;


//-----------------------------------------------------------------------------
/** AUD_DspAC3DRCRange
 *  setup range for dynamix range compression.
 *
 *  @param  uDRCLevel  DRC range.
 *                               0x00000000-->0.0
 *                               0x007FFFFF-->1.0
 *                               0.0 ~ 1.0 step 0.125
 *  @param  uDecIndx 0: first decoder 1: seconder decoder.
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspAC3DRCRange(UINT8 uDRCLevel, UINT8 u1DecId);

#if 0   // Unused
//-----------------------------------------------------------------------------
/** AUD_DspGetAC3DRCRange
 *  setup range for dynamix range compression.
 *
 *  @param  uDecIndx 0: first decoder 1: seconder decoder.
 *  @retval uDRCLevel  DRC range.
 *                               0x00000000-->0.0
 *                               0x007FFFFF-->1.0
 *                               0.0 ~ 1.0 step 0.125
 */
//-----------------------------------------------------------------------------
extern UINT8 AUD_DspGetAC3DRCRange(UINT8 u1DecId);
#endif

//-----------------------------------------------------------------------------
/** AUD_DspGetAC3DRCRange100
 *  setup range for dynamix range compression.
 *
 *  @param  uDecIndx 0: first decoder 1: seconder decoder.
 *  @retval UINT32  uDRCLevel  DRC range.
 *                               0x00000000-->0.0
 *                               0x007FFFFF-->1.0
 */
//-----------------------------------------------------------------------------
extern UINT8 AUD_DspGetAC3DRCRange100(UINT8 u1DecId);

//-----------------------------------------------------------------------------
/** AUD_DspAC3DRCRange100
 *  setup range for dynamix range compression.
 *
 *  @param  uDRCLevel  DRC range 0~100.
 *  @param  uDecIndx 0: first decoder 1: seconder decoder.
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspAC3DRCRange100(UINT8 uDRCLevel, UINT8 u1DecId);

extern void Aud_G726DecCfg(UINT8 u1DecId, UINT8 u1rate);

#ifdef CC_AUD_SUPPORT_MS10
//-----------------------------------------------------------------------------
/** AUD_DspSetDDCStrmId
 *  setup range for associated stream id.
 *
 *  @param  uDecIndx 0: first decoder 1: seconder decoder.
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspSetDDCControl (UINT16 u2Control, UINT8 u1DecId);

//-----------------------------------------------------------------------------
/** AUD_DspSetDDCStrmId
 *  setup range for associated stream id.
 *
 *  @param  uDecIndx 0: first decoder 1: seconder decoder.
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspSetDDCStrmId (UINT8 u1StrmId, UINT8 u1DecId);

//-----------------------------------------------------------------------------
/** AUD_DspSetDDCAdEnable
 *  setup for DDC AD.
 *
 *  @param  uDecIndx 0: first decoder 1: seconder decoder.
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspSetDDCAdEnable (UINT8 u1DecId, BOOL fgEnable);

//-----------------------------------------------------------------------------
/** AUD_DspSetDDCMixer
 *  setup for DDC mixer.
 *
 *  @param  uDecIndx 0: first decoder 1: seconder decoder.
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspSetDDCMixer (UINT8 u1Control, UINT8 u1DecId);

/** AUD_DspDdtDmxMode
 *  setup for DDT dmx mode.
 *
 *  @param  u1Mode: dmxmode/RF mode, u1DecId 0: first decoder 1: seconder decoder. 2: third decoder, bSetDmx 0: set dmx, 1: set RF
 *  @retval void
 *  u1Mode:
 *  00b: Line, Lt/Rt
 *  01b: Line, Lo/Ro
 *  10b: RF, Lt/Rt
 *  11b: RF, Lo/Ro
 */
//-----------------------------------------------------------------------------
extern void AUD_DspDdtDmxRfMode (UINT8 u1Mode, UINT8 u1DecId, BOOL bSetDmx);

//-----------------------------------------------------------------------------
/** AUD_DspDdtDualMode
 *  setup for DDT dual mode.
 *
 *  @param  u1Mode: dual mode, u1DecId 0: first decoder 1: seconder decoder. 2: third decoder
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspDdtDualMode (UINT8 u1Mode, UINT8 u1DecId);

//-----------------------------------------------------------------------------
/** AUD_DspDdtCompVal
 *  setup for DDT DRC cut/boost factor.
 *
 *  @param  u1Mode: cut/boost factor 0~100, u1DecId 0: first decoder 1: seconder decoder. 2: third decoder
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspDdtCompVal (UINT8 u1Mode, UINT8 u1DecId);

//-----------------------------------------------------------------------------
/** AUD_DspGetDdtDmxRfMode
 *  setup for DDT dmx mode.
 *
 *  @param  u1DecId 0: first decoder 1: seconder decoder. 2: third decoder, bGetDmx 0: get dmx, 1: get RF
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern UINT8 AUD_DspGetDdtDmxRfMode (UINT8 u1DecId, BOOL bGetDmx);

//-----------------------------------------------------------------------------
/** AUD_DspDdtDualMode
 *  setup for DDT dual mode.
 *
 *  @param  u1Mode: dual mode, u1DecId 0: first decoder 1: seconder decoder. 2: third decoder
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern UINT8 AUD_DspGetDdtDualMode (UINT8 u1DecId);

//-----------------------------------------------------------------------------
/** AUD_DspDdtCompVal
 *  setup for DDT DRC cut/boost factor.
 *
 *  @param  u1Mode: cut/boost factor 0~100, u1DecId 0: first decoder 1: seconder decoder. 2: third decoder
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern UINT8 AUD_DspGetDdtDrcRange (UINT8 u1DecId);

//-----------------------------------------------------------------------------

#endif  // CC_AUD_SUPPORT_MS10

/** AUD_DspSpeakerConfig
 *  speaker configuration setup.
 *
 *  @param  u4SpkCfg  Speaker config
 *  @param  uDecIndx 0: first decoder 1: seconder decoder.
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspSpeakerConfig(UINT32 u4SpkCfg, UINT8 uDecIndex);
extern UINT32 AUD_DspGetSpeakerConfig(UINT8 uDecIndex);

#if 0   // Unused
//-----------------------------------------------------------------------------
/** AUD_DspSpeakerSizeConfig
 *  speaker configuration setup.
 *
 *  @param  uDecIndex  Speaker config
 *  @param  uDecIndx 0: first decoder 1: seconder decoder.
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspSpeakerSizeConfig(UINT8 uDecIndex, BOOL fgFrontLarge, BOOL fgSurroundLarge, BOOL fgCenterLarge);
#endif

//-----------------------------------------------------------------------------
/** AUD_DspSpeakerLargeSmallConfig
 *  Speaker size configuration of each channel pair.
 *  @param  u1DecIndex          Audio decoder id (0: first decoder, 1: second decoder).
 *  @param  u1FrontPair          Speaker size configuration for front left and right channels. (1: large, 0: small)
 *  @param  u2Center             Speaker size configuration for center channel. (1: large, 0: small)
 *  @param  u1Surround          Speaker size configuration for left and right surround channels. (1: large, 0: small)
 *  @param  u1CH910              Speaker size configuration for CH9/10 channels. (1: large, 0: small)
 *
 *  @return void
 */
//-----------------------------------------------------------------------------
// large(1)/small(0)
extern void AUD_DspSpeakerLargeSmallConfig(UINT8 u1DecIndex, UINT8 u1FrontPair, UINT8 u2Center, UINT8 u1Surround, UINT8 u1CH910);
extern void AUD_DspGetSpeakerLargeSmallConfig(UINT8 u1DecIndex, UINT8* u1FrontPair, UINT8* u1Center, UINT8* u1Surround, UINT8* u1CH910);

//-----------------------------------------------------------------------------
/** AUD_DspSpeakerLsConfig
 *  setup speaker size of each channel.
 *
 *  @param  u1DecIndex 0: first decoder 1: seconder decoder.
 *  @param  u1L  large(1)/small(0).
 *  @param  u1R  large(1)/small(0).
 *  @param  u1Ls   large(1)/small(0).
 *  @param  u1Rs   large(1)/small(0).
 *  @param  u1C  large(1)/small(0).
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspSpeakerLsConfig(UINT8 u1DecIndex, UINT8 u1L, UINT8 u1R, UINT8 u1Ls, UINT8 u1Rs, UINT8 u1C);


//-----------------------------------------------------------------------------
/** AUD_DspSpeakerOutputConfig
 *  set speaker output config.
 *
 *  @param  u1DecIndex 0: first decoder 1: seconder decoder.
 *  @param  u1SpkCfg.
 *                              bit 0 ~ 2:.
 *                              b000: 2/0 (LT/RT downmix: prologic compatible).
 *                              b001: 1/0.
 *                              b010: 2/0 (LO/RO).
 *                              b011: 3/0.
 *                              b100: 2/1.
 *                              b101: 3/1.
 *                              b110: 2/2.
 *                              b111: 3/2.
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspSpeakerOutputConfig(UINT8 u1DecIndex, UINT8 u1SpkCfg);
extern UINT8 AUD_DspGetSpeakerOutputConfig(UINT8 u1DecIndex);

//-----------------------------------------------------------------------------
/** AUD_DspSpeakerSubwooferEnable
 *  Turn On/Off subwoofer
 *  @param  u1DecIndex          Audio decoder id (0: first decoder, 1: second decoder).
 *  @param  fgEnable              Subwoofer channel is turned on or off. (1: on, 0: off)
 *
 *  @return void
 */
//-----------------------------------------------------------------------------
// bit 5: Subwoofer Channel present(1)/absent(0)
extern void AUD_DspSpeakerSubwooferEnable(UINT8 u1DecIndex, BOOL fgEnable);
extern BOOL AUD_DspGetSpeakerSubwooferEnable(UINT8 u1DecIndex);

//-----------------------------------------------------------------------------
/** AUD_DspChannelDelay
 *  setup channel delay for individual channel.
 *
 *  @param  u2Delay 0.05 m/unit.
*  @param  eChIndex 0(L), 1(R), 2(LS), 3(RS), 4(C), 5(SUB), 6(Bypass L), 7(Bypass R)
 *                   8(Downmix L), 9(Downmix R), 10(AUD L), 11(AUD R), 13(All), 14(LFE)
 *  @param  u1DecIndex 0: first decoder 1: seconder decoder.
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspChannelDelayValue(INT16 u2Delay, AUD_CH_T eChIndex, UINT8 uDecIndex);
extern void AUD_DspChannelDelayAP(AUD_CH_T eChIndex, UINT8 uDecIndex);
extern void AUD_DspChannelDelay(UINT8 u1DspId, UINT16 u2Delay, AUD_CH_T eChIndex, UINT8 uDecIndex);
extern void AUD_DspChannelDelay_initial(UINT16 u2Delay, AUD_CH_T eChIndex, UINT8 uDecIndex);
#ifdef CC_AUD_DDI
extern BOOL AUD_PlayMuteSetChannelDelayReady(UINT16 u2ChannId);
#endif
#ifdef CC_ENABLE_AOMX
extern BOOL AUD_PlayMuteSetAoutEnableReady(UINT16 u2Timeout);
#endif
//-----------------------------------------------------------------------------
/** AUD_DspGetChannelDelay
 *  Get channel delay for individual channel.
 *
 *  @param  eChIndex 0(L), 1(R), 2(LS), 3(RS), 4(C), 5(SUB), 6(Bypass L), 7(Bypass R)
 *                   8(Downmix L), 9(Downmix R), 10(AUD L), 11(AUD R), 13(All), 14(LFE)
 *  @retval channel delay
 */
//-----------------------------------------------------------------------------
extern UINT16 AUD_DspGetChannelDelay(UINT8 u1DspId, AUD_CH_T eChIndex);

#ifndef CC_AUD_ENABLE_PLAY_MUTE_CONTROL
#ifndef CC_ENABLE_AV_SYNC
//-----------------------------------------------------------------------------
/** AUD_DspInputChannelDelay
 *  setup channel delay by input source.
 *
 *  @param  eStreamFrom
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspInputChannelDelay(AUD_DEC_STREAM_FROM_T eStreamFrom);
#endif
#endif

//-----------------------------------------------------------------------------
/** AUD_DspChannelVolume
 *  Get channel volume of individual channel.
 *
 *  @param  u1DecId 0: first decoder 1: seconder decoder.
 *  @param  eChannel  : 0: front left channel.
 *                               1: front right channel.
 *                               2: rear left channel.
 *                               3: rear right channel.
 *                               4: center channel.
 *                               5: sub woofer channel.
 *                               6: bypass left channel.
 *                               7: bypass right channel.
 *                               8: downmix left channel.
 *                               9: downmix right channel.
 *                             10: aux front left channel.
 *                             11: aux front right channel.
 *                             12: input source gain.
 *                             13: all channels    ( For master volume control ).
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern UINT8 AUD_DspGetChannelVolume(UINT8 u1DecId, AUD_CH_T eChannel);

//-----------------------------------------------------------------------------
/** AUD_DspPanFadePesUpdateEn
 *  setup ad fade pan values update by pes.
 *
 *  @param
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspPanFadePesUpdateEn(BOOL bEnable);

//-----------------------------------------------------------------------------
/** AUD_DspAdFadeVolumeEnable
 *  setup ad fade volume for individual channel.
 *
 *  @param
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspAdPanFadeEnable(BOOL bEnable);
extern UINT8 AUD_DspGetAdPanFadeSetting(void);

#if 1
//-----------------------------------------------------------------------------
// AudLoadAudioClip
//
/** Brief of AudLoadAudioClip.
 *  Details of AudLoadAudioClip (optional).
 *
 *  @param      u1DecId                   Decoder ID (AUD_DEC_MAIN or AUD_DEC_AUX)
 *  @param      u1BufferSize             ClipBufferSize
 *  @param      u1ClipBufferPointer   u1ClipBufferPointer
 *  @param      u1DecFmt                 DecFmt
 *  @param      u1ReaptNumber        ReaptNumber
 */
//-----------------------------------------------------------------------------

void AudLoadAudioClip(UINT8 u1DspId, UINT8 u1DecId, UINT32 u1BufferSize, UINT32 u1ClipBufferPointer, AUD_FMT_T u1DecFmt, UINT32 u1ReaptNumber );
#else
//-----------------------------------------------------------------------------
// AudLoadAudioClip
//
/** Brief of AudLoadAudioClip.
 *  Details of AudLoadAudioClip (optional).
 *
 *  @param      u1DecId                   Decoder ID (AUD_DEC_MAIN or AUD_DEC_AUX)
 *  @param      u1BufferSize             ClipBufferSize
 *  @param      u1ClipBufferPointer   u1ClipBufferPointer
 */
//-----------------------------------------------------------------------------

extern void AudLoadAudioClip(UINT8 u1DecId, UINT32 u1BufferSize, UINT32 u1ClipBufferPointer);
#endif



//-----------------------------------------------------------------------------
/** AUD_DspAdFadeVolumeSet
 *  setup ad fade volume for individual channel.
 *
 *  @param
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspAdFadeVolumeSet(UINT8 u1Value);

//-----------------------------------------------------------------------------
/** AUD_DspAdFadeVolumeUpdate
 *  setup ad fade volume for individual channel.
 *
 *  @param
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspAdFadeVolumeUpdate(UINT8 u1DecId, AUD_CH_T eChannel);
extern void AUD_GetEncData(UINT32 pDest,UINT32 pBufAddr,UINT32 datasize,UINT32 pRStart,UINT32 pREnd);
extern void AUD_GetBluetoothDate(UINT32 pDest,UINT32 pBufAddr,UINT32 datasize);

//-----------------------------------------------------------------------------
/** AUD_DspAdPanVolumeSet
 *  setup ad pan volume for individual channel.
 *
 *  @param  u1DecId 0: first decoder 1: seconder decoder.
 *  @param  eChannel  : 0: front left channel.
 *                               1: front right channel.
 *                               2: rear left channel.
 *                               3: rear right channel.
 *                               4: center channel.
 *                               5: sub woofer channel.
 *                               6: bypass left channel.
 *                               7: bypass right channel.
 *                               8: downmix left channel.
 *                               9: downmix right channel.
 *                             10: aux front left channel.
 *                             11: aux front right channel.
 *                             12: input source gain.
 *                             13: all channels    ( For master volume control ).
 *  @param  u1Value 0~100.
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspAdPanVolumeSet(UINT8 u1DecId, AUD_CH_T eChannel, UINT8 u1Value);

//-----------------------------------------------------------------------------
/** AUD_DspAdPanVolume
 *  setup ad pan volume for individual channel.
 *
 *  @param  u1DecId 0: first decoder 1: seconder decoder.
 *  @param  eChannel  : 0: front left channel.
 *                               1: front right channel.
 *                               2: rear left channel.
 *                               3: rear right channel.
 *                               4: center channel.
 *                               5: sub woofer channel.
 *                               6: bypass left channel.
 *                               7: bypass right channel.
 *                               8: downmix left channel.
 *                               9: downmix right channel.
 *                             10: aux front left channel.
 *                             11: aux front right channel.
 *                             12: input source gain.
 *                             13: all channels    ( For master volume control ).
 *  @param  u1Value 0~100.
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspAdPanVolumeUpdate(UINT8 u1DecId, AUD_CH_T eChannel);

//-----------------------------------------------------------------------------
/** _AUD_DspChannelVolShmRawValue
 *  Set master volume use share memory raw data .
 *
 *  Note that AUD_DspChannelVolume & _AUD_DspChannelVolShmRawValue can only select
 *  one to use
 *
 *  @param  u1DecId 0: first decoder 1: seconder decoder.
 *  @param  u4VolShm : 0 ~ 0x20000
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspChannelVolShmRawValue(UINT8 u1DecId, AUD_CH_T eChannel, UINT32 u4VolShm);

//-----------------------------------------------------------------------------
/** AUD_DspChannelVolume
 *  setup channel volume for individual channel.
 *
 *  @param  u1DecId 0: first decoder 1: seconder decoder.
 *  @param  eChannel  : 0: front left channel.
 *                               1: front right channel.
 *                               2: rear left channel.
 *                               3: rear right channel.
 *                               4: center channel.
 *                               5: sub woofer channel.
 *                               6: bypass left channel.
 *                               7: bypass right channel.
 *                               8: downmix left channel.
 *                               9: downmix right channel.
 *                             10: aux front left channel.
 *                             11: aux front right channel.
 *                             12: input source gain.
 *                             13: all channels    ( For master volume control ).
 *  @param  u1Value 0~100.
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspChannelVolume(UINT8 u1DspId, UINT8 u1DecId, AUD_CH_T eChannel, UINT8 u1Value);
extern void AUD_DspChannelVolumeNew(UINT8 u1DecId, AUD_CH_T eChannel, UINT8 u1Value, UINT8 u1Value1);
#ifdef CC_AUD_DDI
extern void AUD_DspBluetoothVolume(UINT8 u1DecId, UINT8 u1Value, UINT8 u1Value1);
#endif
#ifdef CC_AUD_LINEOUT_VOL_OFFSET
//-----------------------------------------------------------------------------
/** AUD_DspLineOutVolOffset
 *  setup bypass channel volume offset
 *
 *  @param  u1DecId 0: first decoder 1: seconder decoder.
 *  @param  u1Value 0~100.
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspLineOutVolOffset(UINT8 u1DecId, UINT8 u1Value);
#endif
#ifdef CC_AUD_VOLUME_OFFSET
//-----------------------------------------------------------------------------
/** AUD_DspVolumeOffset
 *  set volume offset to master volume.
 *
 *  @param  u1DecId 0: first decoder 1: seconder decoder.
 *  @param  u1Index  : offset map index.
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspVolumeOffset(UINT8 u1DecId, UINT8 u1Index);

//-----------------------------------------------------------------------------
/** AUD_DspHpVolumeOffset
 *  set HP volume offset.
 *
 *  @param  u1DecId 0: first decoder 1: seconder decoder.
 *  @param  u1Index  : offset map index.
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspHpVolumeOffset(UINT8 u1DecId, UINT8 u1Index);

//-----------------------------------------------------------------------------
/** AUD_DspLineOutVolOffset
 *  setup bypass channel volume offset
 *
 *  @param  u1DecId 0: first decoder 1: seconder decoder.
 *  @param  u1Index  : offset map index.
 *  @retval void
 */
//-----------------------------------------------------------------------------
//extern void AUD_DspLineOutVolOffset(UINT8 u1DecId,UINT8 u1Index);

#ifdef CC_S_SUBWOFFER_SUPPORT
//-----------------------------------------------------------------------------
/** AUD_DspSWVolumeOffset
 *  set HP volume offset.
 *
 *  @param  u1DecId 0: first decoder 1: seconder decoder.
 *  @param  u1Index  : offset map index. 
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspSWVolumeOffset(UINT8 u1DecId,UINT8 u1Index);
#endif

#endif

#ifdef CC_AUD_SX1_FEATURE
//-----------------------------------------------------------------------------
/** AUD_DspAdVolOffset
 *  setup audio description volume offset
 *
 *  @param  u1DecId 0: first decoder 1: seconder decoder.
 *  @param  u1Index  : offset map index.
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspAdVolOffset(UINT8 u1Value);
#endif

//-----------------------------------------------------------------------------
/** AUD_DspWMAHeader
 *  setup WMA information
 *
 *  @param  u1DecId 0: first decoder 1: seconder decoder.
 *  @param  prWmaInfo  : pointer of WMA header information
 *  @retval void
 */
//-----------------------------------------------------------------------------
#ifdef DSP_WMA_MWIF
extern void AUD_DspWMAHeader(UINT8 u1DecId, AUD_WMA_CFG_T* prWmaInfo);
#endif
#if 1 //CC_APE_SUPPORT
// APE decoder
//-----------------------------------------------------------------------------
/** AUD_DspAPEHeader
 *  setup APE information
 *
 *  @param  u1DecId 0: first decoder 1: seconder decoder.
 *  @param  prApeInfo  : pointer of APE header information
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspAPEHeader(UINT8 u1DecId, AUD_APE_CFG_T* prApeInfo);
extern void AUD_DspAPEMuteBankNum(UINT8 u1DecId, UINT16 u2MuteBankNum);
#endif
extern void AUD_DspWMAProHeader(UINT8 u1DecId, AUD_WMA_CFG_T* prWmaInfo);
extern void AUD_DspFLACHeader(UINT8 u1DecId, AUD_FLAC_INFO_T* prFlacInfo);

//-----------------------------------------------------------------------------
/** AUD_DspChannelMute
 *  setup mute for individual channel.
 *
 *  @param  u1DecId 0: first decoder 1: seconder decoder.
 *  @param  eChannel  : 0: front left channel.
 *                               1: front right channel.
 *                               2: rear left channel.
 *                               3: rear right channel.
 *                               4: center channel.
 *                               5: sub woofer channel.
 *                               6: bypass left channel.
 *                               7: bypass right channel.
 *                               8: downmix left channel.
 *                               9: downmix right channel.
 *                             10: aux front left channel.
 *                             11: aux front right channel.
 *                             12: input source gain.
 *                             13: all channels    ( For master volume control ).
 *  @param  fgMute TRUE, FALSE
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspChannelMute(UINT8 u1DspId, UINT8 u1DecId, AUD_CH_T eChannel, BOOL fgMute);

//-----------------------------------------------------------------------------
/** AUD_DspChannelVolGain
 *  setup channel volume extra gain for individual channel.
 *
 *  @param  u1DecId 0: first decoder 1: seconder decoder.
 *  @param  eChannel  : 0: front left channel.
 *                               1: front right channel.
 *                               2: rear left channel.
 *                               3: rear right channel.
 *                               4: center channel.
 *                               5: sub woofer channel.
 *                               6: bypass left channel.
 *                               7: bypass right channel.
 *                               8: downmix left channel.
 *                               9: downmix right channel.
 *                             10: aux front left channel.
 *                             11: aux front right channel.
 *                             12: input source gain.
 *                             13: all channels    ( For master volume control ).
 *  @param  i4Value     -256~96.
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspChannelVolGain(UINT8 u1DecId, AUD_CH_T eChannel, INT32 i4Value);

//-----------------------------------------------------------------------------
/** AUD_DspChannelVolDolbyGainTB11
 *  setup channel volume Dolby gain TB11 for individual channel.
 *
 *  @param  u1DecId 0: first decoder 1: seconder decoder.
 *  @param  eChannel  : 0: front left channel.
 *                               1: front right channel.
 *                               2: rear left channel.
 *                               3: rear right channel.
 *                               4: center channel.
 *                               5: sub woofer channel.
 *                               6: bypass left channel.
 *                               7: bypass right channel.
 *                               8: downmix left channel.
 *                               9: downmix right channel.
 *                             10: aux front left channel.
 *                             11: aux front right channel.
 *                             12: input source gain.
 *                             13: all channels    ( For master volume control ).
 *  @param  i4Value     -256~96.
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspChannelVolDolbyGainTB11(UINT8 u1DecId, AUD_CH_T eChannel, INT32 i4Value);

/***************************************************************************
     Function : AUD_DspDolbyGainTB11
     Description: Dolby Technical Bulletin 11.5 (addtional 3dB attenuation for EU)
     @param u1DecId
                AUD_DEC_MAIN: 1st dec
                AUD_DEC_AUX: 2nd dec
     @retval    void
***************************************************************************/
extern void AUD_DspDolbyGainTB11(UINT8 u1DecId);

//-----------------------------------------------------------------------------
/** AUD_DspGetChannelVolGain
 *  Get channel volume extra gain for individual channel.
 *
 *  @param  u1DecId 0: first decoder 1: seconder decoder.
 *  @param  eChannel  : 0: front left channel.
 *                               1: front right channel.
 *                               2: rear left channel.
 *                               3: rear right channel.
 *                               4: center channel.
 *                               5: sub woofer channel.
 *                               6: bypass left channel.
 *                               7: bypass right channel.
 *                               8: downmix left channel.
 *                               9: downmix right channel.
 *                             10: aux front left channel.
 *                             11: aux front right channel.
 *                             12: input source gain.
 *                             13: all channels    ( For master volume control ).
 *
 *  @retval extra gain (-256~96)
 */
//-----------------------------------------------------------------------------
extern INT32 AUD_DspGetChannelVolGain(UINT8 u1DecId, AUD_CH_T eChannel);

//-----------------------------------------------------------------------------
/** AUD_DspGetChannelVolGainNonLineIn
 *  Get non line in channel volume extra gain or individual channel.
 *
 *  @param  u1DecId 0: first decoder 1: seconder decoder.
 *  @param  eChannel  : 0: front left channel.
 *                               1: front right channel.
 *                               2: rear left channel.
 *                               3: rear right channel.
 *                               4: center channel.
 *                               5: sub woofer channel.
 *                               6: bypass left channel.
 *                               7: bypass right channel.
 *                               8: downmix left channel.
 *                               9: downmix right channel.
 *                             10: aux front left channel.
 *                             11: aux front right channel.
 *                             12: input source gain.
 *                             13: all channels    ( For master volume control ).
 *
 *  @retval extra gain (-256~96)
 */
//-----------------------------------------------------------------------------
extern INT32 AUD_DspGetChannelVolGainNonLineIn(UINT8 u1DecId, AUD_CH_T eChannel);

//-----------------------------------------------------------------------------
/** AUD_DspChannelOutPort
 *  setup output port for individual channel.
 *
 *  @param  u1DecId 0: first decoder 1: seconder decoder.
 *  @param  eChannel  : 0: front left channel.
 *                               1: front right channel.
 *                               2: rear left channel.
 *                               3: rear right channel.
 *                               4: center channel.
 *                               5: sub woofer channel.
 *                               6: bypass left channel.
 *                               7: bypass right channel.
 *                               8: downmix left channel.
 *                               9: downmix right channel.
 *                             10: aux front left channel.
 *                             11: aux front right channel.
 *                             12: input source gain.
 *                             13: all channels    ( For master volume control ).
 *  @param  fgEnable TRUE, FALSE
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspChannelOutPort(UINT8 u1DecId, AUD_CH_T eChannel, BOOL fgEnable);

//-----------------------------------------------------------------------------
/** AUD_DspBalance
 *  setup balance.
 *
 *  @param  u1DecId 0: first decoder 1: seconder decoder.
 *  @param  u1Balance  : balance value (0~100).
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspBalance(UINT8 u1DecId, UINT8 u1Balance);

//-----------------------------------------------------------------------------
/** AUD_DspMasterVolShmRawValue
 *  Set master volume use share memory raw data .
 *
 *  Note that AUD_DspMasterVolume & _AUD_DspMasterVolShmRawValue can only select
 *  one to use
 *
 *  @param  u1DecId 0: first decoder 1: seconder decoder.
 *  @param  u4VolShm : 0 ~ 0x20000
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspMasterVolShmRawValue(UINT8 u1DecId, UINT32 u4VolShm);

//-----------------------------------------------------------------------------
/** AUD_DspMasterVolume
 *  set master volume.
 *
 *  @param  u1DecId 0: first decoder 1: seconder decoder.
 *  @param  u1Volume  : volume value (0~100).
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspMasterVolume(UINT8 u1DecId, UINT8 u1Volume);
extern UINT8 AUD_DspGetMasterVolume(UINT8 u1DecId);
extern void AUD_DspMixSndVolume(UINT8 u1DecId, UINT8 u1Volume);
//-----------------------------------------------------------------------------
/** AUD_DspAVCEnable
 *  enable AVC.
 *
 *  @param  u1DecId 0: first decoder 1: seconder decoder.
 *  @param  fgEnable  : on/off.
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspAVCEnable(UINT8 u1DecId, BOOL fgEnable);

#ifdef CC_AUD_DRC_V20
//-----------------------------------------------------------------------------
/** AUD_DspSetDrc
 *
 *
 *  @param  u2Type
 *  @param  u2Value
 *  @param  u1DecId    0: first decoder 1: seconder decoder.
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspSetDrc(AUD_DRC_V20_INFO_T eType, UINT32 u4Value, UINT8 u1DecId);

//-----------------------------------------------------------------------------
/** AUD_DspGetDrc
 *
 *
 *  @param  u2Type
 *  @param  u2Value
 *  @param  u1DecId    0: first decoder 1: seconder decoder.
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspGetDrc(AUD_DRC_V20_INFO_T eType, UINT32* u4Value, UINT8 u1DecId);
#endif

#ifdef CC_AUD_AVC_V20
//-----------------------------------------------------------------------------
/** AUD_DspChangeAVCPara
 *  setup automatic volume control parameters.
 *
 *  @param  u2Type     parameter selection
 *                             0: AVC on/off (u2Value = 0: AVC off, 1: AVC on)
 *                             1: AVC target level (dB) (u2Value = -6 ~ -24)
 *                             3: Max gain up (dB) (u2Value = 0 ~ 8)
 *                             5: Adjust rate (dB/128ms)
 *                             6: Adjust rate when AVC is turned on in UI (dB/128ms)
 *  @param  u2Value    parameter value
 *  @param  u1DecId    0: first decoder 1: seconder decoder.
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspChangeAVCPara (AUD_DRC_V20_INFO_T eType, UINT32 u4Value, UINT8 u1DecId);
extern void AUD_DspGetAVCPara(AUD_DRC_V20_INFO_T eType, UINT32* u4Value, UINT8 u1DecId);
extern void AUD_DspChange2BandAVCPara (AUD_DRC_V20_INFO_T eType, UINT32 u4Value, UINT8 u1DecId);

#else

//-----------------------------------------------------------------------------
/** AUD_DspChangeAVCPara
 *  setup automatic volume control parameters.
 *
 *  @param  u2Type     parameter selection
 *                             0: AVC on/off (u2Value = 0: AVC off, 1: AVC on)
 *                             1: AVC target level (dB) (u2Value = -6 ~ -24)
 *                             3: Max gain up (dB) (u2Value = 0 ~ 8)
 *                             5: Adjust rate (dB/128ms)
 *                             6: Adjust rate when AVC is turned on in UI (dB/128ms)
 *  @param  u2Value    parameter value
 *  @param  u1DecId    0: first decoder 1: seconder decoder.
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspChangeAVCPara(UINT16 u2Type, INT16 u2Value, UINT8 u1DecId);
extern void AUD_DspGetAVCPara(UINT16 u2Type, UINT16* u2Value, UINT8 u1DecId);

//-----------------------------------------------------------------------------
#ifdef CC_AUD_LOUDNESS_FROM_SOURCE_SOUNDMODE
extern void AUD_DspInputChangeLoudnessPara(AUD_DEC_STREAM_FROM_T eStreamFrom);
#endif

//-----------------------------------------------------------------------------
/** AUD_DspChange2BandAVCPara
 *  setup automatic volume control parameters.
 *
 *  @param  u2Type     parameter selection
 *                             0: AVC on/off (u2Value = 0: AVC off, 1: AVC on)
 *                             1: AVC target level (dB) (u2Value = -6 ~ -24)
 *                             3: Max gain up (dB) (u2Value = 0 ~ 8)
 *                             5: Adjust rate (dB/128ms)
 *                             6: Adjust rate when AVC is turned on in UI (dB/128ms)
 *  @param  u2Value    parameter value
 *  @param  u1DecId    0: first decoder 1: seconder decoder.
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspChange2BandAVCPara(UINT8 u1Band, UINT16 u2Type,
                                      UINT16 u2Value, UINT8 u1DecId);

#endif

//-----------------------------------------------------------------------------
/** AUD_DspInputChangeAVCPara
 *  setup automatic volume control parameters by input source.
 *
 *  @param  eStreamFrom
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspInputChangeAVCPara(AUD_DEC_STREAM_FROM_T eStreamFrom);

//-----------------------------------------------------------------------------
/** AUD_DspMuteEnable
 *  audio mute control.
 *
 *  @param  u1DecId    0: first decoder 1: seconder decoder.
 *  @param  fgEnable   Mute/unmute. (1: mute audio, 0: unmute audio)
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspMuteEnable(UINT8 u1DecId, BOOL fgEnable);

//-----------------------------------------------------------------------------
/** AUD_DspMuteAllEnable
 *  audio mute control.
 *
 *  @param  u1DecId    0: first decoder 1: seconder decoder.
 *  @param  fgEnable   Mute/unmute. (1: mute audio, 0: unmute audio)
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void AUD_DspMuteAllEnable(UINT8 u1DecId, BOOL fgEnable);

//-----------------------------------------------------------------------------
/** AUD_DspHdmiModeMuteEnable
 *  audio mute control in HDMI mode.
 *
 *  @param  u1DecId    0: first decoder 1: seconder decoder.
 *  @param  fgEnable   Mute/unmute. (1: mute audio, 0: unmute audio)
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspHdmiModeMuteEnable(UINT8 u1DecId, BOOL fgEnable);

//-----------------------------------------------------------------------------
/** AUD_DspBassMngEnalbe
 *  Bass management control.
 *
 *  @param  u1DecId    0: first decoder 1: seconder decoder.
 *  @param  fgEnable   Bass management enable. (1: enable, 0: disable)
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspBassMngEnalbe(UINT8 u1DspId, UINT8 u1DecId, BOOL fgEnable);
extern BOOL AUD_DspGetBassMngEnable(UINT8 u1DspId, UINT8 u1DecId);
extern BOOL AUD_DspGetChannelVolumeEnable(UINT8 u1DspId, UINT8 u1DecId);
extern BOOL AUD_DspGetDelayEnable(UINT8 u1DspId, UINT8 u1DecId);
#ifdef CC_AUD_DDI
extern void AUD_DspSetDelayEnable(UINT8 u1DspId, UINT8 u1DecId, BOOL fgEnable);
#endif
extern BOOL AUD_DspGetPostProcessingEnable(UINT8 u1DspId, UINT8 u1DecId);

//-----------------------------------------------------------------------------
/** AUD_DspProcMode
 *  setup for compression mode.
 *
 *  @param  u2CompMode compression mode.
 *  @param  uDecIndx 0: first decoder 1: seconder decoder.
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspProcMode(UINT8 u1DspId, UINT8 u1DecId, UINT16 u2Mode);


//-----------------------------------------------------------------------------
/** AUD_DspVirtualSurroundEnalbe
 *  enable virtual surround.
 *
 *  @param  u1DecId 0: first decoder 1: seconder decoder.
 *  @param  fgEnable  : on/off.
 *  @retval void
 */
//-----------------------------------------------------------------------------
#ifndef CC_AUD_NEW_POST_MULTI_VS_SUPPORT
extern void AUD_DspVirtualSurroundEnalbe(UINT8 u1DecId, BOOL fgEnable);
#else
extern void AUD_DspVirtualSurroundEnalbe(UINT8 u1DecId, UINT8 u1ModeIdx);
extern void AUD_DspVSModeChange(UINT8 u1DecId, UINT8 u1ModeIdx);
#endif
extern BOOL AUD_DspGetVirtualSurroundEnalbe(UINT8 u1DecId);

/** AUD_DspBbeMode
 *  Set BBE mode.
 *
 *  @param  u1DecId 0: first decoder 1: seconder decoder.
 *  @param  u1BbeMode  : 0: OFF, 1: BBE, 2: BBE ViVA4.
 *  @retval void
 */
#ifdef CC_AUD_BBE_SUPPORT
extern void AUD_DspBbeMode(UINT8 u1DecId, UINT8 u1BbeMode);
#endif
#ifdef CC_SET_VOLUME
extern void AUD_DspSetVolTable(UINT8 u1Idx, UINT32  u4Volume);
#endif
#ifdef CC_AUD_NEW_CV_TABLE
#ifndef CC_AUD_NVM_LR_USE_DIFFER_VOL_TBL
extern void AUD_DspSetCVTable(UINT8 u1Idx, UINT32  u4Volume);
extern UINT32 AUD_DspGetCVTable(UINT8 u1VolTabIdx);
extern void AUD_DspSetStepCVTable(UINT32* u4VolTabChkPnt);
#else
extern void AUD_DspSetCVTable(AUD_NVM_VOL_TBL_T rVolTbl, UINT8 u1Idx, UINT32  u4Volume);
extern UINT32 AUD_DspGetCVTable(AUD_NVM_VOL_TBL_T rVolTbl, UINT8 u1VolTabIdx);
extern void AUD_DspSetStepCVTable(AUD_NVM_VOL_TBL_T rVolTbl, UINT32* u4VolTabChkPnt);
#endif
#endif

//-----------------------------------------------------------------------------
/** AUD_DspMonoDownMix
 *  enable mono downmix.
 *
 *  @param  u1DecId 0: first decoder 1: seconder decoder.
 *  @param  fgEnable  : on/off.
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspMonoDownMix(UINT8 u1DecId, BOOL fgEnable);

//-----------------------------------------------------------------------------
/** AUD_DspSetKaraokeMixRatio
 *  Set karaoke mix ratio
 *
 *  @param  u1DecId 0: first decoder 1: seconder decoder.
 *  @param  u4MixRatio  : 0x0 ~ 0x800000 (for R channel)
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspSetKaraokeMixRatio(UINT8 u1DecId, UINT32 u4MixRatio);


//-----------------------------------------------------------------------------
/** AUD_DspSetMatrixOutMode
 *  setup for matrix output mode.
 *
 *  @param  u1DecId 0: first decoder 1: seconder decoder.
 *  @param  eMode 0: stereo (disable), 1: L mono, 2: R mono, 3: mix mono
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspSetMatrixOutMode(UINT8 u1DecId, AUD_MATRIX_OUT_T eMode) ;
extern UINT8 AUD_DspGetMatrixOutMode(UINT8 u1DecId);

//-----------------------------------------------------------------------------
/** AUD_DspAutoDmx
 *  Setup auto downmix mode for AC3
 *
 *  @param  u1DecId 0: first decoder 1: seconder decoder.
 *  @param  fgEnable  : 0: disable AC3 auto downmix, 1: enable AC3 auto downmix
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspAutoDmx(UINT8 u1DecId, BOOL fgEnable);

//-----------------------------------------------------------------------------
/** AUD_GetDspAutoDmx
 *  Get auto downmix mode for AC3
 *
 *  @param  u1DecId 0: first decoder 1: seconder decoder.
 *  @param  fgEnable  : 0: disable AC3 auto downmix, 1: enable AC3 auto downmix
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern BOOL AUD_GetDspAutoDmx(UINT8 u1DecId);

//-----------------------------------------------------------------------------
/** AUD_DspBypassPostProcess
 *  Bypass audio post-processing.
 *
 *  @param  u1DecId 0: first decoder 1: seconder decoder.
 *  @param  u2BypassMode  : 1: bypass post-processing.
 *                          0x8000: bypass delay
 *                          0x4000: bypass trim
 *                          0x2000: bypass post-processing
 *                          0x1000: bypass bass management
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspBypassPostProcess(UINT8 u1DspId, UINT8 u1DecId, UINT16 u2BypassMode);

//-----------------------------------------------------------------------------
/** AUD_DspSoundEffectFlag
 *  Enable all audio sound effect. (EQ, Bass/Treble, Reverb, Surround, AVC)
 *
 *  @param  u1DecId  0: first decoder 1: seconder decoder.
 *  @param  fgFlag     0: disable, 1: enable
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspSoundEffectFlag(UINT8 u1DecId, BOOL fgFlag);



//-----------------------------------------------------------------------------
/** AUD_DspIECConfig
 *  Routine handling IEC Configuration.
 *
 *  @param  eIecCfg  0: PCM; 1:RAW.
 *  @param  fgEnable  on/off.
 *  @retval  TRUE
 */
//-----------------------------------------------------------------------------
extern BOOL AUD_DspIECConfig(AUD_IEC_T eIecCfg, BOOL fgEnable);

extern BOOL AUD_GetDspIECConfig(void);


//-----------------------------------------------------------------------------
/** AUD_DspSetIecRawFlag
 *  Enable/disable sync IEC RAW delay to channel delay automatically.
 *
 *  @param  u1Flag
 *  @retval  N/A
 */
//-----------------------------------------------------------------------------
extern BOOL AUD_DspSetIecRawFlag(UINT8 u1Flag);

//-----------------------------------------------------------------------------
/** AUD_DspSetIecRawDelay
 *  Set IEC RAW delay.
 *
 *  @param  i2Delay (ms)
 *  @retval  N/A
 */
//-----------------------------------------------------------------------------
extern BOOL AUD_DspSetIecRawDelay(INT16 i2Delay);

//-----------------------------------------------------------------------------
/** AUD_DspIecChannel
 *  Routine handling IEC Configuration.
 *
 *  @param  eIecChannel  select audio channel,ex L/R.
 *  @retval  TRUE
 */
//-----------------------------------------------------------------------------
extern BOOL AUD_DspIecChannel(AUD_IEC_CH_T eIecChannel);

//-----------------------------------------------------------------------------
/** AUD_DspIecCopyright
 *  Routine handling IEC Configuration.
 *
 *  @param  u1Enable 0: disable 1: enable
 *  @param  u1CategoryCode
 *  @retval  TRUE
 */
//-----------------------------------------------------------------------------
BOOL AUD_DspIecCopyright(UINT8 u1Enable, UINT8 u1CategoryCode);

//-----------------------------------------------------------------------------
/** AUD_DspGetIecCopyright
 *  Routine handling IEC Configuration.
 *
 *  @param  eStreamFrom     input source
 *
 *  @retval  copyright value
 */
//-----------------------------------------------------------------------------
extern UINT8 AUD_DspGetIecCopyright(AUD_DEC_STREAM_FROM_T eStreamFrom);

//-----------------------------------------------------------------------------
/** AUD_DspGetIecCategoryCode
 *  Routine handling IEC Configuration.
 *
 *  @param  eStreamFrom     input source
 *
 *  @retval  category code value
 */
//-----------------------------------------------------------------------------
extern UINT8 AUD_DspGetIecCategoryCode(AUD_DEC_STREAM_FROM_T eStreamFrom);

//-----------------------------------------------------------------------------
/** AUD_DspResetSpdifReg
 *   reset SPDIF register.
 *
 *  @param  void
 *
 *  @retval  void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspResetSpdifReg (AUD_DEC_STREAM_FROM_T eStreamFrom);

#ifndef CC_AUD_SUPPORT_SPDIF_V20
//-----------------------------------------------------------------------------
/** AUD_DspSetSpdifRegType
 *   get SPDIF register type.
 *
 *  @param  SPIDF_REG_TYPE_T
 *
 *  @retval  void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspSetSpdifRegType (SPDIF_REG_TYPE_T type, AUD_DEC_STREAM_FROM_T eStreamFrom);
#endif

//-----------------------------------------------------------------------------
/** AUD_DspSetSpdifCopyright
 *  Routine handling SPDIF Configuration.
 *
 *  @param  eStreamFrom     input source
 *
 *  @retval  copyright value
 */
//-----------------------------------------------------------------------------
#ifdef CC_AUD_SUPPORT_SPDIF_V20
extern void AUD_DspSetSpdifCopyright(AUD_DEC_STREAM_FROM_T eStreamFrom, UINT8 copyright);
#else
extern void AUD_DspSetSpdifCopyright(SPDIF_REG_TYPE_T type, AUD_DEC_STREAM_FROM_T eStreamFrom, UINT8 copyright);
#endif

//-----------------------------------------------------------------------------
/** AUD_DspSetSpdifCategoryCode
 *  Routine handling SPDIF Configuration.
 *
 *  @param  eStreamFrom     input source
 *
 *  @retval  copyright value
 */
//-----------------------------------------------------------------------------
extern void AUD_DspSetSpdifCategoryCode(AUD_DEC_STREAM_FROM_T eStreamFrom, UINT8 categoryCode);

//-----------------------------------------------------------------------------
/** AUD_DspSetSpdifWordLength
 *  Routine handling SPDIF Configuration.
 *
 *  @param  eStreamFrom     input source
 *
 *  @retval  copyright value
 */
//-----------------------------------------------------------------------------
extern void AUD_DspSetSpdifWordLength(AUD_DEC_STREAM_FROM_T eStreamFrom, UINT8 wordLength);

//-----------------------------------------------------------------------------
/** AUD_DspSpdifSetInfo
 *  Routine handling IEC Configuration.
 *
 *  @param  u1DecId 0: first decoder 1: seconder decoder.
 *  @param  u1Enable 0: disable 1: enable
 *  @retval  TRUE
 */
//-----------------------------------------------------------------------------
extern BOOL AUD_DspSpdifSetInfo(AUD_DEC_STREAM_FROM_T eStreamFrom, UINT8 u1CopyRight, UINT8 u1CategoryCode);

//-----------------------------------------------------------------------------
/** AUD_DspSpdifSetCopyProtect
 *  Routine handling IEC Configuration.
 *
 *  @param  u1DecId 0: first decoder 1: seconder decoder.
 *  @param  u1Enable 0: disable 1: enable
 *  @retval  TRUE
 */
//-----------------------------------------------------------------------------
BOOL AUD_DspSpdifSetCopyProtect(AUD_DEC_STREAM_FROM_T eStreamFrom, BOOL bCpBit, BOOL bLBit);

#ifdef CC_AUD_DDI
void AUD_DspSetSoundBarOnOff(BOOL fgOnOff);

void AUD_DspSetSoundBarIDData(UINT32 Id, UINT8 data, UINT8 volumeInfo);

void AUD_DspGetSoundBarStatus(UINT8 *Id, UINT8 *data);
#endif

//-----------------------------------------------------------------------------
/** AUD_DspSetSpdifUpdateMode
 *  Routine handling SPDIF Configuration.
 *
 *  @param  eStreamFrom     input source
 *
 *  @retval  copyright value
 */
//-----------------------------------------------------------------------------
extern void AUD_DspSetSpdifUpdateMode(AUD_DEC_STREAM_FROM_T eStreamFrom, UINT8 eUpdate);

//-----------------------------------------------------------------------------
/** AUD_DspGetSpdifCopyright
 *  Routine handling IEC Configuration.
 *
 *  @param  eStreamFrom     input source
 *
 *  @retval  copyright value
 */
//-----------------------------------------------------------------------------
extern UINT8 AUD_DspGetSpdifCopyright(SPDIF_REG_TYPE_T type, AUD_DEC_STREAM_FROM_T eStreamFrom);

//-----------------------------------------------------------------------------
/** AUD_DspGetSpdifCategoryCode
 *  Routine handling IEC Configuration.
 *
 *  @param  eStreamFrom     input source
 *
 *  @retval  copyright value
 */
//-----------------------------------------------------------------------------
extern UINT8 AUD_DspGetSpdifCategoryCode(SPDIF_REG_TYPE_T type, AUD_DEC_STREAM_FROM_T eStreamFrom);

//-----------------------------------------------------------------------------
/** AUD_DspGetSpdifWordLength
 *  Routine handling IEC Configuration.
 *
 *  @param  eStreamFrom     input source
 *
 *  @retval  word length value
 */
//-----------------------------------------------------------------------------
extern UINT8 AUD_DspGetSpdifWordLength(SPDIF_REG_TYPE_T type, AUD_DEC_STREAM_FROM_T eStreamFrom);

//-----------------------------------------------------------------------------
/** AUD_DspGetSpdifUpdateMode
 *  Routine handling IEC Configuration.
 *
 *  @param  eStreamFrom     input source
 *
 *  @retval  by source? value
 */
//-----------------------------------------------------------------------------
extern UINT8 AUD_DspGetSpdifUpdateMode(SPDIF_REG_TYPE_T type, AUD_DEC_STREAM_FROM_T eStreamFrom);

//-----------------------------------------------------------------------------
/** AUD_DspGetSpdifUpdateMode
 *  Routine handling IEC Configuration.
 *
 *  @param
 *
 *  @retval
 */
//-----------------------------------------------------------------------------
extern UINT8 AUD_DspGetSpdifChannelStatus(SPDIF_CHANNEL_STATUS_T* rInfo);


//-----------------------------------------------------------------------------
/** AUD_DspGetSpdifOutputMode
 *  Routine handling IEC Configuration.
 *
 *  @param  u1DecId     decoder ID
 *
 *  @retval  value
 */
//-----------------------------------------------------------------------------
extern UINT8 AUD_DspGetSpdifOutputMode(UINT8 u1DecId);

#ifdef CC_AUD_MIXSOUND_SUPPORT
//-----------------------------------------------------------------------------
/** AUD_DspMixSndEnable
 *  Mix sound enable.
 *
 *  @param  fgEnable TRUE(enable) FALSE(disable).
 *  @retval  void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspMixSndEnable(BOOL fgEnable);

//-----------------------------------------------------------------------------
/** AUD_DspMixSndMute
 *  Mix sound mute.
 *
 *  @param  fgMutre TRUE(mute) FALSE(un-mute).
 *  @retval  void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspMixSndMute(BOOL fgMute);

//-----------------------------------------------------------------------------
/** AUD_DspMixSndControl
 *  Mix sound control.
 *
 *  @param  u1Mode play/stop.
 *  @retval  void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspMixSndControl(UINT16 u2Mode);
extern void AUD_DspMixSndControl2(UINT16 u2Mode); //NEW_MIXSOUND

extern void AUD_DspMixSndDec3(BOOL fgEnable);
//-----------------------------------------------------------------------------
/** AUD_DspMixSndData
 *  Select mix sound data source.
 *
 *  @param  u1DataIdx 0~3
 *  @retval  void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspMixSndClip(UINT8 u1ClipIdx);

//-----------------------------------------------------------------------------
/** AUD_DspGetMixSndEnable
 *  Get mix sound enable.
 *
 *  @param  void.
 *  @retval  TRUE(enable) FALSE(disable).
 */
//-----------------------------------------------------------------------------
extern BOOL AUD_DspGetMixSndEnable(void);

//-----------------------------------------------------------------------------
/** AUD_DspGetMixSndClipIdx
 *  Get mix sound clip index.
 *
 *  @param  void.
 *  @retval  TRUE(enable) FALSE(disable).
 */
//-----------------------------------------------------------------------------
extern BOOL AUD_DspGetMixSndClipIdx(void);

extern void AUD_DspSetMixSndLoop(UINT32 u4Loop);
extern void AUD_DspSetMixSndVol(UINT32 u4Vol);
extern UINT32 AUD_DspGetMixSndLoop(void);
extern UINT32 AUD_DspGetMixSndVol(void);
#endif

#if 0   // Unused
//-----------------------------------------------------------------------------
/** AUD_DspSetDemodOutputMode
 *  Set MTS output mode.
 *
 *  @param  u1Output    0: Output (Mono,Mono)
 *                               1: If stereo exists, Output (L,R) otherwise (Mono,Mono)
 *                               2: If SAP exists, Output (S,S) otherwise (L,R) otherwise (Mono,Mono)
 *
 *  @retval  void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspSetDemodOutputMode(UINT8 u1Output);
#endif

//-----------------------------------------------------------------------------
/** AUD_DspSetDetectDefaultMode
 *   call this function before issue detection command.
 *
 *  @param  u1Mode 0: PAL_DK, 4: SECAM-L.
 *  @retval  void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspSetDetectDefaultMode(UINT8 u1Mode);


//-----------------------------------------------------------------------------
/** AUD_GetVideoScrambleStatus
 *  Get VideoScrambleStatus
 *
 *  @param  void.
 *  @retval  TRUE(enable) FALSE(disable).
 */
//-----------------------------------------------------------------------------
extern BOOL AUD_GetVideoScrambleStatus(void);


//-----------------------------------------------------------------------------
/** AUD_VideoScrambled
 *   Set Video Scramble Status
 *
 *  @param  fgEnable     0: Normal Video, 1: Video scrambled .
 *
 *  @retval  void
 */
//-----------------------------------------------------------------------------
extern void AUD_VideoScrambled(BOOL fgEnable);


//-----------------------------------------------------------------------------
/** AUD_DspSetUserDefinedEqCfg
 *   setup user difined EQ config.
 *
 *  @param  u1DecId  0:first decoder 1: seconder decoder.
 *  @param  eEqType  0: off   others: 1~12.
 *  @param  u1BandIdx  0~4.
 *  @param  i1Value -100 ~ +100
 *  @retval  void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspSetUserDefinedEqCfg(UINT8 u1DecId, AUD_EQ_TYPE_T eEqType, UINT8 u1BandIdx, INT8 i1Value);


//-----------------------------------------------------------------------------
/** AUD_DspSetUserDefinedEqCfg
 *   Get user difined EQ config.
 *
 *  @param  eEqType  0: off   others: 1~12.
 *  @param  u1BandIdx  0~4.
 *  @param  i1Value (-20 ~ +20)
 *
 *  @retval  TRUE: success
 *              FALSE: failed
 */
//-----------------------------------------------------------------------------
extern BOOL AUD_DspGetUserDefineEqCfg(AUD_EQ_TYPE_T eEqType, UINT8 u1BandIdx, INT8* pi1Value);


//-----------------------------------------------------------------------------
/** AUD_DspGetUserEqBandNum
 *   Get user defined band number.
 *
 *  @retval  CH_EQ_BAND_NO = 5.
 */
//-----------------------------------------------------------------------------
extern UINT8 AUD_DspGetUserEqBandNum(void);


//-----------------------------------------------------------------------------
/** AUD_DspGetEqBandInfo
 *   Get user defined band info.
 *
 *  @param  prEqBandInfo  structure of eq band info (AUD_EQ_BAND_INFO_T).
 *  @retval  void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspGetEqBandInfo(AUD_EQ_BAND_INFO_T* prEqBandInfo);


//-----------------------------------------------------------------------------
/** AUD_DspGetSrcVol
 *   get source volume.
 *
 *  @param  u1DecId  0:first decoder 1: seconder decoder.
 *  @param  eStrmSrc      AUD_STREAM_FROM_OTHERS = 0,         // ex. Pink Noise Generator.
                                   AUD_STREAM_FROM_DIGITAL_TUNER,      // Digital tuner.
                                   AUD_STREAM_FROM_ANALOG_TUNER,       // ex. Analog tuner.
                                   AUD_STREAM_FROM_SPDIF,              // SPDIF-in.
                                   AUD_STREAM_FROM_LINE_IN,            // Line in.
                                   AUD_STREAM_FROM_HDMI,               // HDMI in.
                                   AUD_STREAM_FROM_MEMORY,             // Memory.
                                   AUD_STREAM_FROM_BUF_AGT,            //Buffer Agent.
                                   AUD_STREAM_FROM_MULTI_MEDIA,             // Multi-media

 *  @retval  input source volume
 */
//-----------------------------------------------------------------------------
extern INT16 AUD_DspGetSrcVol(UINT8 u1DecId, AUD_DEC_STREAM_FROM_T eStrmSrc);


//-----------------------------------------------------------------------------
/** AUD_DspSetSrcVol
 *   set source volume.
 *
 *  @param  u1DecId  0:first decoder 1: seconder decoder.
 *  @param  eStrmSrc      AUD_STREAM_FROM_OTHERS = 0,         // ex. Pink Noise Generator.
                                   AUD_STREAM_FROM_DIGITAL_TUNER,      // Digital tuner.
                                   AUD_STREAM_FROM_ANALOG_TUNER,       // ex. Analog tuner.
                                   AUD_STREAM_FROM_SPDIF,              // SPDIF-in.
                                   AUD_STREAM_FROM_LINE_IN,            // Line in.
                                   AUD_STREAM_FROM_HDMI,               // HDMI in.
                                   AUD_STREAM_FROM_MEMORY,             // Memory.
                                   AUD_STREAM_FROM_BUF_AGT,            //Buffer Agent.
                                   AUD_STREAM_FROM_MULTI_MEDIA,             // Multi-media

 *  @param  i2Vol  -128 ~ 96
 *  @retval  void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspSetSrcVol(UINT8 u1DecId, AUD_DEC_STREAM_FROM_T eStrmSrc, INT16 i2Vol);

#ifdef CC_AUD_INPUT_SRC_VOL_OFFSET
//-----------------------------------------------------------------------------
/** AUD_DspGetSrcVolOffset
 *   get source volume offset.
 *
 *  @param  u1DecId  0:first decoder 1: seconder decoder.
 *  @param  eStrmSrc      AUD_STREAM_FROM_OTHERS = 0,         // ex. Pink Noise Generator.
                                   AUD_STREAM_FROM_DIGITAL_TUNER,      // Digital tuner.
                                   AUD_STREAM_FROM_ANALOG_TUNER,       // ex. Analog tuner.
                                   AUD_STREAM_FROM_SPDIF,              // SPDIF-in.
                                   AUD_STREAM_FROM_LINE_IN,            // Line in.
                                   AUD_STREAM_FROM_HDMI,               // HDMI in.
                                   AUD_STREAM_FROM_MEMORY,             // Memory.
                                   AUD_STREAM_FROM_BUF_AGT,            //Buffer Agent.
                                   AUD_STREAM_FROM_MULTI_MEDIA,             // Multi-media

 *  @retval  input source volume offset
 */
//-----------------------------------------------------------------------------
extern INT16 AUD_DspGetSrcVolOffset(UINT8 u1DecId, AUD_DEC_STREAM_FROM_T eStrmSrc);

//-----------------------------------------------------------------------------
/** AUD_DspSetSrcVolOffset
 *   set source volume offset.
 *
 *  @param  u1DecId  0:first decoder 1: seconder decoder.
 *  @param  eStrmSrc      AUD_STREAM_FROM_OTHERS = 0,         // ex. Pink Noise Generator.
                                   AUD_STREAM_FROM_DIGITAL_TUNER,      // Digital tuner.
                                   AUD_STREAM_FROM_ANALOG_TUNER,       // ex. Analog tuner.
                                   AUD_STREAM_FROM_SPDIF,              // SPDIF-in.
                                   AUD_STREAM_FROM_LINE_IN,            // Line in.
                                   AUD_STREAM_FROM_HDMI,               // HDMI in.
                                   AUD_STREAM_FROM_MEMORY,             // Memory.
                                   AUD_STREAM_FROM_BUF_AGT,            //Buffer Agent.
                                   AUD_STREAM_FROM_MULTI_MEDIA,             // Multi-media

 *  @param  i2Vol  -128 ~ 96
 *  @retval  void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspSetSrcVolOffset(UINT8 u1DecId, AUD_DEC_STREAM_FROM_T eStrmSrc, INT16 i2Vol);
#endif

//-----------------------------------------------------------------------------
/** AUD_DspVolumeChange
 *   Setup master volume, or input source gain, or channel trim to DSP.
 *
 *  @param  u1DecId    0: first decoder 1: seconder decoder.
 *  @param  eAudChl  : 0: front left channel.
 *                              1: front right channel.
 *                              2: rear left channel.
 *                              3: rear right channel.
 *                              4: center channel.
 *                              5: sub woofer channel.
 *                              6: bypass left channel.
 *                              7: bypass right channel.
 *                              8: downmix left channel.
 *                              9: downmix right channel.
 *                            10: aux front left channel.
 *                            11: aux front right channel.
 *                            12: input source gain.
 *                            13: all channels    ( For master volume control ).
 *
 *  @retval  void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspVolumeChange(UINT8 u1DspId, UINT8 u1DecId, AUD_CH_T eAudChl);

#ifdef CC_AUD_CHL_VOL_MODE
//-----------------------------------------------------------------------------
/** AUD_DspLRChannelVolMode
 *   Set LR channel trim mode to DSP.
 *
 *  @param  u1DecId    0: first decoder 1: seconder decoder.
 *  @param  u1Mode  : 0: Normal mode.
 *                              1: Skeype mode.
 *
 *  @retval  void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspLRChannelVolMode(UINT8 u1DecId, UINT8 u1Mode);
#endif
//-----------------------------------------------------------------------------
/** AUD_DspSetAvSrcVol
 *   set AV source volume.
 *
 *  @param  eInputSrcId         AUD_INPUT_ID_COMP_VID_0 = 0,
                                          AUD_INPUT_ID_COMP_VID_1,
                                          AUD_INPUT_ID_COMP_VID_2,
                                          AUD_INPUT_ID_COMP_VID_3,
                                          AUD_INPUT_ID_S_VID_0,
                                          AUD_INPUT_ID_S_VID_1,
                                          AUD_INPUT_ID_S_VID_2,
                                          AUD_INPUT_ID_YPBPR_0,
                                          AUD_INPUT_ID_YPBPR_1,
                                          AUD_INPUT_ID_YPBPR_2,
                                          AUD_INPUT_ID_VGA_0,
                                          AUD_INPUT_ID_VGA_1,
                                          AUD_INPUT_ID_HDMI_0,
                                          AUD_INPUT_ID_HDMI_1,
                                          AUD_INPUT_ID_HDMI_2,
                                          AUD_INPUT_ID_HDMI_3,
                                          AUD_INPUT_ID_DVI_0,
                                          AUD_INPUT_ID_DVI_1,
                                          AUD_INPUT_ID_DVI_2,
                                          AUD_INPUT_ID_DVI_3,
                                          AUD_INPUT_ID_SCART_0,
                                          AUD_INPUT_ID_SCART_1,
                                          AUD_INPUT_ID_SCART_2,
                                          AUD_INPUT_ID_SCART_3,
                                          AUD_INPUT_ID_AUXIN_0,
                                          AUD_INPUT_ID_AUXIN_1,
 *  @param  i2Vol  -256 ~ 256.
 *  @retval  void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspSetAvSrcVol(AUD_INPUT_ID_T eInputSrcId , INT16 i2Vol);

extern AUD_INPUT_ID_T AUD_DspGetAvInputSrcId(void);

//-----------------------------------------------------------------------------
/** AUD_DspGetAvInputSrcDvdId
 *
 *
 *  @retval
 */
//-----------------------------------------------------------------------------
extern AUD_INPUT_ID_T AUD_DspGetAvInputSrcDvdId(void);
#ifdef CC_AUD_DDI
extern UINT8 AUD_GetAudDtvId(void);
extern void AUD_SetDmxId(UINT8 u1DecId, UINT8 u1StcId);
#endif

//-----------------------------------------------------------------------------
/** AUD_DspGetAtvTvSys
 *
 *  @param  prTvAudSys  tv audio system structure.
 *  @retval  void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspGetAtvTvSys(MW_TV_AUD_SYS_T* prTvAudSys);

//-----------------------------------------------------------------------------
/** AUD_DspGetAtvTvSystem
 *
 *  @param void
 *  @retval  TV_AUD_SYS_T
 */
//-----------------------------------------------------------------------------
extern TV_AUD_SYS_T AUD_DspGetAtvTvSystem(void);

/** AUD_DspGetAtvTvSystemStrength
 *
 *  @param TV_AUD_SYS_T
 *  @retval  UINT32
 */
//-----------------------------------------------------------------------------
extern UINT32 AUD_DspGetAtvTvSystemStrength(TV_AUD_SYS_T);


//-----------------------------------------------------------------------------
/** AUD_DspSetTvSrcMode
 *   set tv source mode.
 *
 *  @param  eAudSys      SV_NONE_DETECTED = 0x0,
                                   SV_MTS           = 0x1,
                                   SV_FM_FM         = 0x2,
                                   SV_NTSC_M        = 0x3,
                                   SV_A2_BG         = 0x4,
                                   SV_A2_DK1        = 0x5,
                                   SV_A2_DK2        = 0x6,
                                   SV_A2_DK3        = 0x7,
                                   SV_PAL_I         = 0x8,
                                   SV_PAL_BG        = 0x9,
                                   SV_PAL_DK        = 0xa,
                                   SV_SECAM_L       = 0xb,
                                   SV_SECAM_L_PLUM  = 0xc,
                                   SV_SECAM_BG      = 0xd,
                                   SV_SECAM_DK      = 0xe
 *  @retval  void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspSetTvSrcMode(TV_AUD_SYS_T eAudSys);
extern TV_AUD_SYS_T AUD_DspGetTvSrcMode(void);
#ifdef CC_AUD_TV_SYS_FINE_TUNE_VOL
extern  void AUD_SetTVSysFineTuneVol(TV_AUD_SYS_T eAudSys);
#endif
//-----------------------------------------------------------------------------
/** AUD_DspSetAtvOutputMode
 *   set tv source mode.
 *
 *  @param  u1DecId 0:first decoder 1: seconder decoder.
 *  @param  eSoundMode         UNKNOWN = 0,
                                           MONO,
                                           STEREO,
                                           SUB_LANG,
                                           DUAL1,
                                           DUAL2,
                                           NICAM_MONO,
                                           NICAM_STEREO,
                                           NICAM_DUAL1,
                                           NICAM_DUAL2,
                                           A2_DUAL1,
                                           A2_DUAL2,
 *  @retval  void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspSetAtvOutputMode(UINT8 u1DecId, AUD_SOUND_MODE_T eSoundMode);

#if 0   // Unused
//-----------------------------------------------------------------------------
/** AUD_DspHdmiSpdifOutEnable
 *   SPDIF output control in HDMI mode.
 *
 *  @param  fgEnable    0: disable SPDIF output, 1: enable SPDIF output
 *
 *  @retval  void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspHdmiSpdifOutEnable(BOOL fgEnable);
#endif

//-----------------------------------------------------------------------------
/** AUD_DspGetSoundMode
 *   Get ATV sound mode
 *
 *  @param  u1DecId 0:first decoder 1: seconder decoder.
 *
 *  @retval AUD_SOUND_MODE_T
 */
//-----------------------------------------------------------------------------
extern AUD_SOUND_MODE_T AUD_DspGetSoundMode(UINT8 u1DecId);

//-----------------------------------------------------------------------------
/** AUD_DspGetSpectrumInfo
 *   Get audio spectrum information.
 *
 *  @param  u1DecId 0:first decoder 1: seconder decoder.
 *  @param  u1BandNum      1~15 .
 *  @retval  u4Spectrum
 */
//-----------------------------------------------------------------------------
extern UINT32 AUD_DspGetSpectrumInfo(UINT8 u1DecId, UINT8 u1BandNum);


//-----------------------------------------------------------------------------
/** AUD_DspSpectrumEnable
 *   Enable audio spectrum calculation.
 *
 *  @param  u1DecId    0:first decoder 1: seconder decoder.
 *  @param  fgEnable    0: disable calculation, 1: enable calculation.
 *
 *  @retval  void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspSpectrumEnable(UINT8 u1DecId, BOOL fgEnable);


//-----------------------------------------------------------------------------
/** AUD_AudioSetStcDiffBound
 *   Set STC PTS difference bound. Bound would be (x MS * (1+uStcDiff)).
 *
 *  @param  u1DecId      0:first decoder 1: seconder decoder.
 *  @param  uStcDiffLo    for low boundary of stc diff (0~255) .
 *  @param  uStcDiffHi    for high boundary of stc diff (0~255) .
 *  @param  uStcDiffWs   for worst boundary of stc diff (0~255) .
 *
 *  @retval  void
 */
//-----------------------------------------------------------------------------
extern void AUD_AudioSetStcDiffBound(UINT8 u1DecId, UINT8 uStcDiffLo, UINT8 uStcDiffHi, UINT8 uStcDiffWs);


//-----------------------------------------------------------------------------
/** AUD_DDBannerEnable
 *   Turn on this funtion to close some effect and post processing,  for DD test only.
 *
 *  @param  u1DecId      0:first decoder 1: seconder decoder.
 *  @param  fgEnable     0: disable, 1: enable .
 *
 *  @retval  void
 */
//-----------------------------------------------------------------------------
extern void AUD_DDBannerEnable(UINT8 u1DecId, BOOL fgEnable);

//-----------------------------------------------------------------------------
/** AUD_GetDDBannerEnable
 *   Get Dolby banner information.
 *
 *  @param  u1DecId      0:first decoder 1: seconder decoder.
 *  @param  fgEnable     0: disable, 1: enable .
 *
 *  @retval  void
 */
//-----------------------------------------------------------------------------
extern BOOL AUD_GetDDBannerEnable (void);

//-----------------------------------------------------------------------------
/** AUD_DspSetPalDetection
 *   demodulation related configuration.
 *
 *  @param  eChangedItem
 *  @param  u2Value
 *
 *  @retval  void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspSetPalDetection ( AUD_FACTORY_PAL_DETECTION_ITEM_T eChangedItem, UINT16 u2Value);



//-----------------------------------------------------------------------------
/** AUD_DspSetPalDetection
 *   demodulation related configuration.
 *
 *  @param  eItem
 *
 *  @retval
 */
//-----------------------------------------------------------------------------
extern UINT16 AUD_DspGetPalDetection ( AUD_FACTORY_PAL_DETECTION_ITEM_T eItem);


//-----------------------------------------------------------------------------
/** AUD_DspSetA2Detection
 *   demodulation related configuration.
 *
 *  @param  eItem
 *
 *  @retval
 */
//-----------------------------------------------------------------------------
extern void AUD_DspSetA2Detection ( AUD_FACTORY_A2_DETECTION_ITEM_T eItem, UINT16 u2Value);



//-----------------------------------------------------------------------------
/** AUD_DspGetA2Detection
 *   demodulation related configuration.
 *
 *  @param  eItem
 *
 *  @retval
 */
//-----------------------------------------------------------------------------
extern UINT16 AUD_DspGetA2Detection ( AUD_FACTORY_A2_DETECTION_ITEM_T eItem);



//-----------------------------------------------------------------------------
/** AUD_DspSetA2Threshold
 *   demodulation related configuration.
 *
 *  @param  eItem
 *
 *  @retval
 */
//-----------------------------------------------------------------------------
extern void AUD_DspSetA2Threshold ( AUD_FACTORY_A2_THRESHOLD_ITEM_T eItem, UINT16 u2Value);



//-----------------------------------------------------------------------------
/** AUD_DspGetA2Threshold
 *   demodulation related configuration.
 *
 *  @param  eItem
 *
 *  @retval
 */
//-----------------------------------------------------------------------------
extern UINT16 AUD_DspGetA2Threshold ( AUD_FACTORY_A2_THRESHOLD_ITEM_T  eItem);



//-----------------------------------------------------------------------------
/** AUD_DspSetMtsDetection
 *   Set NPTV MTS Detection Configurations.
 *
 *  @param  eChangedItem
 *  @param  u2Value
 *
 *  @retval
 */
//-----------------------------------------------------------------------------
extern void AUD_DspSetMtsDetection ( AUD_FACTORY_MTS_DETECTION_ITEM_T eChangedItem, UINT16 u2Value)  ;



//-----------------------------------------------------------------------------
/** AUD_DspGetMtsDetection
 *   Set NPTV MTS Detection Configurations.
 *
 *  @param  eItem
 *
 *  @retval
 */
//-----------------------------------------------------------------------------
extern UINT16 AUD_DspGetMtsDetection ( AUD_FACTORY_MTS_DETECTION_ITEM_T eItem);

#if 0   // Unused
//-----------------------------------------------------------------------------
/** AUD_DspSetFmfmDetection
 *   demodulation related configuration.
 *
 *  @param  eChangedItem
 *  @param  u2Value
 *
 *  @retval
 */
//-----------------------------------------------------------------------------
void AUD_DspSetFmfmDetection ( AUD_FACTORY_FMFM_DETECTION_ITEM_T eChangedItem, UINT16 u2Value);


//-----------------------------------------------------------------------------
/** AUD_DspGetFmfmDetection
 *   demodulation related configuration.
 *
 *  @param  eItem
 *
 *  @retval
 */
//-----------------------------------------------------------------------------
extern UINT16 AUD_DspGetFmfmDetection ( AUD_FACTORY_FMFM_DETECTION_ITEM_T eItem) ;
#endif

#ifndef CC_AUD_WAFFLE_OVM_SUPPORT
//-----------------------------------------------------------------------------
/** AUD_DspSetHdevMode
 *   set high deveation mode on/off.
 *
 *  @param  eDecType
 *          bit0 : PAL
 *                  bit1 : A2
 *                  bit2 : MTS
 *                  bit3 : FMFM
 *  @param  fgEnable
 *
 *  @retval  void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspSetHdevMode ( AUD_ATV_DECODER_TYPE_T eDecType, UINT16 u2Mode);

//-----------------------------------------------------------------------------
/** AUD_DspGetHdevMode
 *   set high deveation mode on/off.
 *
 *  @param  eDecType
 *          bit0 : PAL
 *                  bit1 : A2
 *                  bit2 : MTS
 *                  bit3 : FMFM
 *  @param  fgEnable
 *
 *  @retval  void
 */
//-----------------------------------------------------------------------------
extern UINT8 AUD_DspGetHdevMode ( AUD_ATV_DECODER_TYPE_T eDecType);
#else
//-----------------------------------------------------------------------------
/** AUD_DspSetHdevMode
 *   set high deveation mode on/off or off/low/high.
 *
 *  @param  eDecType
 *          bit0 : PAL
 *                  bit1 : A2
 *                  bit2 : MTS
 *                  bit3 : FMFM
 *  @param  u1Mode
 *
 *  @retval  void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspSetHdevMode ( AUD_ATV_DECODER_TYPE_T eDecType, UINT8 u1Mode);

//-----------------------------------------------------------------------------
/** AUD_DspGetHdevMode
 *   set high deveation mode on/off.
 *
 *  @param  eDecType
 *          bit0 : PAL
 *                  bit1 : A2
 *                  bit2 : MTS
 *                  bit3 : FMFM
 *  @param  fgEnable
 *
 *  @retval  void
 */
//-----------------------------------------------------------------------------
extern UINT8 AUD_DspGetHdevMode ( AUD_ATV_DECODER_TYPE_T eDecType);
#endif

//-----------------------------------------------------------------------------
/** AUD_DspSetAmMute
 *   set AM mute mode on/off and thresholds.
 *
 *  @param  eChangedItem
 *  @param  u1Value
 *
 *  @retval  void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspSetAmMute( AUD_FACTORY_PAL_AM_MUTE_ITEM_T eChangedItem, UINT8 u1Value);


//-----------------------------------------------------------------------------
/** AUD_DspGetAmMute
 *   set AM mute mode on/off and thresholds.
 *
 *  @param  eItem
 *
 *  @retval  void
 */
//-----------------------------------------------------------------------------
extern UINT8 AUD_DspGetAmMute( AUD_FACTORY_PAL_AM_MUTE_ITEM_T eItem);

//-----------------------------------------------------------------------------
/** AUD_DspSetFmMute
 *   set FM mute mode on/off and thresholds.
 *
 *  @param  eDecType
 *          bit0 : PAL
 *                  bit1 : A2
 *                  bit2 : MTS
 *                  bit3 : FMFM
 *  @param  eChangedItem
 *  @param  u1Value
 *
 *  @retval  void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspSetFmMute ( AUD_ATV_DECODER_TYPE_T eDecType,
                               AUD_FACTORY_FM_MUTE_ITEM_T eChangedItem,
                               UINT8 u1Value) ;

//-----------------------------------------------------------------------------
/** AUD_DspGetFmMute
 *   set FM mute mode on/off and thresholds.
 *
 *  @param  eDecType
 *          bit0 : PAL
 *                  bit1 : A2
 *                  bit2 : MTS
 *                  bit3 : FMFM
 *  @param  eChangedItem
 *  @param  u1Value
 *
 *  @retval  void
 */
//-----------------------------------------------------------------------------
extern BOOL AUD_DspGetFmMute ( AUD_ATV_DECODER_TYPE_T eDecType,
                               AUD_FACTORY_FM_MUTE_ITEM_T eChangedItem) ;

//-----------------------------------------------------------------------------
/** AUD_DspSetCarrierShiftMode
 *   set carrier shift mode on/off .
 *
 *  @param  eDecType
 *          bit0 : PAL
 *                  bit1 : A2
 *                  bit2 : MTS
 *                  bit3 : FMFM
 *  @param  fgEnable
 *
 *  @retval  void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspSetCarrierShiftMode ( AUD_ATV_DECODER_TYPE_T eDecType, BOOL fgEnable);


//-----------------------------------------------------------------------------
/** AUD_DspGetCarrierShiftMode
 *   set carrier shift mode on/off .
 *
 *  @param  eDecType
 *          bit0 : PAL
 *                  bit1 : A2
 *                  bit2 : MTS
 *                  bit3 : FMFM
 *
 *  @retval  TRUE/FAUSE
 */
//-----------------------------------------------------------------------------
extern BOOL AUD_DspGetCarrierShiftMode (AUD_ATV_DECODER_TYPE_T eDecType);

//-----------------------------------------------------------------------------
/** AUD_DspSetNonEUMode
 *   set Non-EU mode on/off.
 *
 *  @param  fgEnable    TRUE/FALSE
 *
 *  @retval  void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspSetNonEUMode (BOOL fgEnable) ;


//-----------------------------------------------------------------------------
/** AUD_DspGetNonEUMode
 *   Get Non-EU mode on/off.
 *
 *  @retval  void
 */
//-----------------------------------------------------------------------------
extern BOOL AUD_DspGetNonEUMode (void);


//-----------------------------------------------------------------------------
/** AUD_DspSetFMSatuMuteMode
 *   demodulation related configuration.
 *  @param  fgEnable    TRUE/FALSE
 *  @param  eDecType
 *          bit0 : PAL
 *                  bit1 : A2
 *                  bit2 : MTS
 *                  bit3 : FMFM
 *
 *  @retval  void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspSetFMSatuMuteMode (AUD_ATV_DECODER_TYPE_T eDecType, BOOL fgEnable) ;



//-----------------------------------------------------------------------------
/** AUD_DspGetFMSatuMuteMode
 *   demodulation related configuration.
 *  @param  eDecType
 *          bit0 : PAL
 *                  bit1 : A2
 *                  bit2 : MTS
 *                  bit3 : FMFM
 *
 *  @retval  void
 */
//-----------------------------------------------------------------------------
extern BOOL AUD_DspGetFMSatuMuteMode (AUD_ATV_DECODER_TYPE_T eDecType) ;

//-----------------------------------------------------------------------------
/** AUD_DspSetPALFineVolume
 *   Set fine tune volume for PAL.
 *
 *  @param  u1Index      0~40 .
 *
 *  @retval  void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspSetPALFineVolume (UINT8 u1Index) ;


//-----------------------------------------------------------------------------
/** AUD_DspGetPALFineVolume
 *   Get fine tune volume for PAL.
 *
 *  @retval  u1Index (0~40)
 */
//-----------------------------------------------------------------------------
extern UINT8 AUD_DspGetPALFineVolume (void) ;


//-----------------------------------------------------------------------------
/** AUD_DspSetNicamFineVolume
 *   Set fine tune volume for NICAM.
 *
 *  @param  u1Index      0~40 .
 *
 *  @retval  void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspSetNicamFineVolume (UINT8 u1Index) ;


//-----------------------------------------------------------------------------
/** AUD_DspGetNicamFineVolume
 *   Get fine tune volume for NICAM.
 *
 *  @retval  u1Index (0~40)
 */
//-----------------------------------------------------------------------------
extern UINT8 AUD_DspGetNicamFineVolume (void) ;


//-----------------------------------------------------------------------------
/** AUD_DspSetAMFineVolume
 *   Set fine tune volume for AM.
 *
 *  @param  u1Index      0~40 .
 *
 *  @retval  void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspSetAMFineVolume (UINT8 u1Index);

//-----------------------------------------------------------------------------
/** AUD_DspGetAMFineVolume
 *   Get fine tune volume for AM.
 *
 *  @retval  u1Index (0~40)
 */
//-----------------------------------------------------------------------------
extern UINT8 AUD_DspGetAMFineVolume (void) ;


//-----------------------------------------------------------------------------
/** AUD_DspSetA2FineVolume
 *   Set fine tune volume for A2.
 *
 *  @param  u1Index      0~40 .
 *
 *  @retval  void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspSetA2FineVolume (UINT8 u1Index) ;


//-----------------------------------------------------------------------------
/** AUD_DspGetA2FineVolume
 *   Get fine tune volume for A2.
 *
 *  @retval  u1Index (0~40)
 */
//-----------------------------------------------------------------------------
extern UINT8 AUD_DspGetA2FineVolume (void) ;

//-----------------------------------------------------------------------------
/** AUD_DspSetMtsMonoFineVolume
 *   Set fine tune volume for MTS mono.
 *
 *  @param  u1Index      0~40 .
 *
 *  @retval  void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspSetMtsMonoFineVolume (UINT8 u1Index) ;

//-----------------------------------------------------------------------------
/** AUD_DspGetMtsMonoFineVolume
 *   Get fine tune volume for MTS mono.
 *
 *  @retval  u1Index (0~40)
 */
//-----------------------------------------------------------------------------
extern UINT8 AUD_DspGetMtsMonoFineVolume (void) ;


//-----------------------------------------------------------------------------
/** AUD_DspSetSAPFineVolume
 *   Set fine tune volume for SAP.
 *
 *  @param  u1Index      0~40 .
 *
 *  @retval  void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspSetSAPFineVolume(UINT8 u1Index);


//-----------------------------------------------------------------------------
/** AUD_DspGetSapFineVolume
 *   Get fine tune volume for SAP.
 *
 *  @retval  u1Index (0~40)
 */
//-----------------------------------------------------------------------------
extern UINT8 AUD_DspGetSapFineVolume (void) ;

#if 0   // Unused
//-----------------------------------------------------------------------------
/** AUD_DspSetFmfmMonoFineVolume
 *   Set fine tune volume for FMFM mono.
 *
 *  @param  u1Index      0~40 .
 *
 *  @retval  void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspSetFmfmMonoFineVolume (UINT8 u1Index) ;

//-----------------------------------------------------------------------------
/** AUD_DspGetFmfmMonoFineVolume
 *   Get fine tune volume for FMFM mono.
 *
 *  @retval  u1Index (0~40)
 */
//-----------------------------------------------------------------------------
extern UINT8 AUD_DspGetFmfmMonoFineVolume (void) ;

//-----------------------------------------------------------------------------
/** AUD_DspSetFmfmDualFineVolume
 *   Set fine tune volume for FMFM dual.
 *
 *  @param  u1Index      0~40 .
 *
 *  @retval  void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspSetFmfmDualFineVolume(UINT8 u1Index);

//-----------------------------------------------------------------------------
/** AUD_DspGetFmfmDualFineVolume
 *   Get fine tune volume for FMFM dual.
 *
 *  @retval  u1Index (0~40)
 */
//-----------------------------------------------------------------------------
extern UINT8 AUD_DspGetFmfmDualFineVolume (void) ;
#endif

//-----------------------------------------------------------------------------
/** AUD_DspSetFmRadioFineVolume
 *   Set fine tune volume for FM radio.
 *
 *  @param  u1Index      0~40 .
 *
 *  @retval  void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspSetFmRadioFineVolume (UINT8 u1Index) ;

//-----------------------------------------------------------------------------
/** AUD_DspSetMtsPilotDetection
 *   set MTS pilot offset detection on/off.
 *  @param  fgEnable    TRUE/FALSE
 *  @retval  void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspSetMtsPilotDetection (BOOL fgEnable) ;

//-----------------------------------------------------------------------------
/** AUD_DspGetMtsPilotDetection
 *   get MTS pilot offset detection on/off.
 *  @param  void
 *  @retval  BOOL
 */
//-----------------------------------------------------------------------------
extern BOOL AUD_DspGetMtsPilotDetection (void) ;

//-----------------------------------------------------------------------------
/** AUD_DspSetSapMute
 *   set SAP noise mute thresholds
 *  @param  eChangedItem    lower threshold or higher threshold
 *  @param  u1Value
 *  @retval  void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspSetSapMute( AUD_FACTORY_SAP_MUTE_ITEM_T eChangedItem, UINT8 u1Value);

//-----------------------------------------------------------------------------
/** AUD_DspGetSapMute
 *   get SAP noise mute thresholds
 *  @param  eItem lower threshold or higher threshold
 *  @retval  UINT8
 */
//-----------------------------------------------------------------------------
extern UINT8 AUD_DspGetSapMute( AUD_FACTORY_SAP_MUTE_ITEM_T eItem);

//-----------------------------------------------------------------------------
/** AUD_DspSetFMSatuMuteMode
 *   Set saturation mute thresholds
 *  @param  eItem lower threshold or higher threshold
 *  @retval  UINT8
 */
//-----------------------------------------------------------------------------
extern void AUD_DspSetFMSatuMuteTh ( AUD_FACTORY_SATU_MUTE_ITEM_T eChangedItem,
                                     UINT8 u1Value) ;

//-----------------------------------------------------------------------------
/** AUD_DspGetFMSatuMuteTh
 *   get saturation mute thresholds
 *  @param  eItem lower threshold or higher threshold
 *  @retval  UINT8
 */
//-----------------------------------------------------------------------------
extern UINT8 AUD_DspGetFMSatuMuteTh( AUD_FACTORY_SATU_MUTE_ITEM_T eItem);

//-----------------------------------------------------------------------------
// ADAC_Init
//
/** Initial audio codec and digital amp.
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void ADAC_Init(void);

#ifdef DIGITAL_I2S_AMP_REINIT
//-----------------------------------------------------------------------------
// I2S_AMP_Init
//
/** Initial Audio I2S digital amplifier.
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void I2S_AMP_Init(void);
#endif

//-----------------------------------------------------------------------------
// ADAC_Enable
//
/** Audio codec and amp function table init.
 *
 *  @param[in]     fgEnable   TRUE for initial.
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void ADAC_Enable(BOOL fgEnable);

//-----------------------------------------------------------------------------
// ADAC_Mute
//
/** Audio codec and speaker mute.
 *
 *  @param[in]     fgEnable   "TRUE" for mute,"FALSE" for unmute.
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void ADAC_Mute(BOOL fgEnable);

//-----------------------------------------------------------------------------
// ADAC_MuteCLK
//
/** Audio codec Mute PWM CLK - PWM Fade-in / Fade-out
 *
 *  @param[in]     fgEnable   "TRUE" for mute,"FALSE" for unmute.
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void ADAC_MuteCLK(BOOL fgEnable);

//-----------------------------------------------------------------------------
// ADAC_DacFormatCfg
//
/** Setting DAC Data Format.
 *
 *  @param[in]     u1DecId   Decoder ID.
 *  @param[in]     eFormat   Audio data format,ex:LJ,RJ,I2S.
 *  @param[in]     eMclk   MCLK frequency.
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void ADAC_DacFormatCfg(UINT8 u1DecId, DATA_FORMAT_T eFormat, MCLK_FREQUENCY_T eMclk)  ;


//-----------------------------------------------------------------------------
// ADAC_AdcFormatCfg
//
/** Setting Codec Adc Data Format.
 *
 *  @param[in]     u1DecId   Decoder ID.
 *  @param[in]     eFormat   Audio data format,ex:LJ,RJ,I2S.
 *  @param[in]     eMclk   MCLK frequency.
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void ADAC_AdcFormatCfg(UINT8 u1DecId, DATA_FORMAT_T eFormat, MCLK_FREQUENCY_T eMclk)  ;

//-----------------------------------------------------------------------------
// ADAC_HeadphoneVolCtl
//
/** Setting Codec Adc Data Format.
 *
 *  @param[in]     i2VolInHalfDb   volume value.
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void ADAC_HeadphoneVolCtl(INT16 i2VolInHalfDb);

//-----------------------------------------------------------------------------
// ADAC_GetHeadphoneVolMaxMin
/**  Setting the max and min value of headphone volume.
*
*  @param[in]     pi2Max   volume max value.
*  @param[in]     pi2Min   volume min value.
*
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void ADAC_GetHeadphoneVolMaxMin(INT16* pi2Max, INT16* pi2Min);

//-----------------------------------------------------------------------------
// ADAC_DacChlSel
//
/** Configure DAC output channel
 *
 *  @param[in]     u1DacId: Dac ID, eChl: output channel
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void ADAC_DacSetChl(UINT8 u1DacId, AUD_CHL_DEF_T eChl);

//-----------------------------------------------------------------------------
// ADAC_DacChlSel
//
/** Get Dac output channel  config
 *
 *  @param[in]     void
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void ADAC_DacGetChl(void);

//-----------------------------------------------------------------------------
// ADAC_ADCDown
//
/** Power on/down ADC
 *
 *  @param[in]     fgEnable: True: power on, FALSE: power off
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void ADAC_ADCDown(BOOL fgEnable);

//-----------------------------------------------------------------------------
// ADAC_VolumeMuteSpeakerEnable
//
/** Mute speaker when volume value equal to zero.
 *
 *  @param[in]     fgFlag   "TRUE" for enable this function.
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void ADAC_VolumeMuteSpeakerEnable(BOOL fgFlag);

//-----------------------------------------------------------------------------
// ADAC_DcDetectSpkEnable
//
/** Detect the DC and mute speaker.
 *
 *  @param[in]     fgFlag   "TRUE" for enable this function.
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void ADAC_DcDetectSpkEnable(BOOL fgFlag);

#if defined(CC_AUD_SX1_FEATURE) || defined (CC_MAPLE_CUST_DRV)
//-----------------------------------------------------------------------------
// ADAC_SpkHpLinkEnable
//
/** Speaker & headphone link enable.
 *
 *  @param[in]     fgFlag   "TRUE" for enable this function.
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void ADAC_SpkHpLinkEnable(BOOL fgFlag);
#endif

//-----------------------------------------------------------------------------
// ADAC_HpDetectSpkEnable
//
/** Detect the headphone plug in and mute speaker.
 *
 *  @param[in]     fgFlag   "TRUE" for enable this function.
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void ADAC_HpDetectSpkEnable(BOOL fgFlag);

//-----------------------------------------------------------------------------
// ADAC_HpDetectCodecMute
//
/** Detect the headphone plug in and mute codec.
 *
 *  @param[in]     fgMute   "TRUE" for enable this function.
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void ADAC_HpDetectCodecMute(BOOL fgMute);


//-----------------------------------------------------------------------------
// ADAC_HpDetectDacMute
//
/** Detect the headphone plug in and mute internaldac.
 *
 *  @param[in]     fgMute   "TRUE" for enable this function.
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void ADAC_HpDetectDacMute(BOOL fgMute);

//-----------------------------------------------------------------------------
// ADAC_ClkChangeCodecMute
//
/** Some codec have to mute before clock source change
 *
 *  @param[in]     fgMute   "TRUE" for enable this function.
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void ADAC_ClkChangeCodecMute(BOOL fgMute);

//-----------------------------------------------------------------------------
// ADAC_SpeakerEnable
//
/** Speaker enable routine.
 *
 *  @param[in]     fgEnable   "TRUE" for enable, "FALSE" for mute speaker.
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void ADAC_SpeakerEnable(BOOL fgEnable);

//-----------------------------------------------------------------------------
// ADAC_SpeakerState_Reinit
//
/** Speaker State Reinit
 *
 *  Call at _SetOutPort for pwmdac de-pop noise flowchart which make
 *  - System boot & headphone plug-in, speaker un-mute (I2S non-mute)
 *  - UI Audio Speaker:Off & system re-boot speaker un-mute (I2S non-mute)
 *
 */
//-----------------------------------------------------------------------------
extern void ADAC_SpeakerState_Reinit(void);

//-----------------------------------------------------------------------------
// ADAC_SpeakerStateQuery
//
/** Speaker enable query function.
 *
 *  @param[in]     void.
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void ADAC_SpeakerStateQuery(void);

//-----------------------------------------------------------------------------
// ADAC_CodecMute
//
/** Codec enable routine.
 *
 *  @param[in]     fgMute   "TRUE" for enable, "FALSE" for mute codec.
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void ADAC_CodecMute(BOOL fgMute);


//-----------------------------------------------------------------------------
// ADAC_GpioCodecMute
//
/** Codec GPIO control for mute/unmute.
 *
 *  @param[in]     fgEnable   "TRUE" for enable, "FALSE" for mute codec from GPIO.
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void ADAC_GpioCodecMute(BOOL fgEnable);


//-----------------------------------------------------------------------------
// ADAC_GpioAmpEnable
//
/** Speaker GPIO control for mute/unmute.
 *
 *  @param[in]     fgEnable   "TRUE" for enable, "FALSE" for mute speaker from GPIO.
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void ADAC_GpioAmpEnable(BOOL fgEnable);
extern void ADAC_GpioAmpEnable1(BOOL fgEnable);

//-----------------------------------------------------------------------------
// ADAC_CodecAdChlSel
//
/** Codec ad channel select.
 *
 *  @param[in]     u1Chl   Input data channel.
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void ADAC_CodecAdChlSel(UINT8 u1Chl);

//-----------------------------------------------------------------------------
// ADAC_CodecRegRead
//
/** Read register of external codec.
 *
 *  @param[in]     u1Offset   Address.
 *
 *  @retval     UINT8   Data.
 */
//-----------------------------------------------------------------------------
extern UINT8 ADAC_CodecRegRead(UINT8 u1Offset);

//-----------------------------------------------------------------------------
// ADAC_CodecRegWrite
//
/** Write register of external codec.
 *
 *  @param[in]     u1Offset     Address.
 *  @param[in]     u1Data       Data.
 *
 *  @retval     void.
 */
//-----------------------------------------------------------------------------
extern void ADAC_CodecRegWrite(UINT8 u1Offset, UINT8 u1Data);


//-----------------------------------------------------------------------------
// ADAC_AmpRegRead
//
/** Read register of external digital amp.
 *
 *  @param[in]     u1Offset   Address.
 *
 *  @retval     UINT8   Data.
 */
//-----------------------------------------------------------------------------
extern UINT8 ADAC_AmpRegRead(UINT8 u1Offset);

//-----------------------------------------------------------------------------
// ADAC_AmpRegWrite
//
/** Write register of external digital amplify.
 *
 *  @param[in]     u1Offset     Address.
 *  @param[in]     u1Data       Data.
 *
 *  @retval     void.
 */
//-----------------------------------------------------------------------------
extern void ADAC_AmpRegWrite(UINT8 u1Offset, UINT8 u1Data);

extern void ADAC_AmpInit(void);
extern void ADAC_AmpMute(UINT8 u1Data);

#ifdef CC_DSP_SUPPORT_ARC
extern void ADAC_ArcEnable(BOOL fgEnable);
#endif
extern BOOL AUD_SetSPDIFEnable(BOOL fgEnable, BOOL fgLight);

#ifdef CC_FAST_INIT
//-----------------------------------------------------------------------------
/** AUD_pm_resume
 *  Resume function.
 *
 *  @param
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void AUD_pm_resume(void);

//-----------------------------------------------------------------------------
/** AUD_pm_suspend
 *  suspend function.
 *
 *  @param
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void AUD_pm_suspend(void);
#endif

#ifdef CC_AUD_BBE_SUPPORT
extern UINT8 AUD_DspGetBbeMode(void);
extern void AUD_DspSetBbeLevel (INT8 i1Level);
extern void AUD_DspSetBbeProcess (UINT8 u1BoostDb);
extern void AUD_DspSetBbeLoContour(UINT8 u1BoostDb);
extern void AUD_DspSetBbe3dGain(UINT8 u13dGain);
extern INT8 AUD_DspGetBbeLevel(void);
extern UINT8 AUD_DspGetBbeProcess(void);
extern UINT8 AUD_DspGetBbeLoContour(void);
extern UINT8 AUD_DspGetBbe3dGain(void);
#endif

//-----------------------------------------------------------------------------
// AUD_FeederSetRequestFunc
//
/** For feeder to set request function.
 *
 *  @param     prTransmit   Notify function handler
 *
 *  @retval    TRUE   ok
 *  @retval    FALSE  fail
 */
//-----------------------------------------------------------------------------
//extern BOOL AUD_FeederSetRequestInfo(FEEDER_TRANSMIT *prTransmit);

extern void AUD_GetStreamInfo(UINT8 u1DspId, UINT8 u1DecId, UINT8 *pu1Acmode, UINT8 *pu1SmpRate, UINT32 * pu4DataRate);
extern void AUD_GetStreamInfo_AC3(UINT8 u1DecId, UINT8* pu1Acmode, UINT8* pu1Ac3DecType, UINT8* pu1SmpRate, UINT32* pu4DataRate);
extern void AUD_StrSelect(UINT8 u1DspId, UINT8 u1DecId, UINT8 u1Str, MEM_BUFFER_INFO_T *prMemBuf);
extern void AUD_ToneGen(UINT8 u1DspId, UINT8 u1DecId, INT32 i4Fc[2], MEM_BUFFER_INFO_T *prMemBuf);
extern void AUD_UniversalTonePlay(INT32 i4LeftFreq, INT32 i4RightFreq);
extern void AUD_DspCfgSetDecType(UINT8 u1DecId, AUD_DEC_STREAM_FROM_T eStreamFrom, AUD_FMT_T eDecFmt);
extern void _AUD_DspDolbyGainTB11(UINT8 u1DecId);

extern void _AUD_OutPadEnable(UINT8 u1DecId, BOOL fgEnable);

extern BOOL AUD_SetStartPts(UINT8 u1DecId, UINT32 u4Pts);
extern void AUD_DrvClkCfgDualDecMode(BOOL fgEnable);

extern void AudShowDspStatus(UINT8 u1DspId);
extern void AudShowDolbyInfo(void);
extern void AudShowMtsStatus(UINT8 u1DecId);
#if 1///def CC_VORBIS_SUPPORT    ///CC_VORBIS_SUPPORT_Q
extern void AudShowVorbisStatus(UINT8 u1DecId);
#endif
extern void AudShowStatus(UINT8 u1DspId, UINT8 u1DecId);
extern void AUD_ChlOutDump(UINT8 u1DspId, UINT8 u1Chl);
extern UINT32 AUD_GetUnderFlowCnt(UINT8 u1DecId);
#ifdef CC_AUD_DSP_SUPPORT_AUDYSSEY
extern void AudShowADVStatus(void);
#endif

//-----------------------------------------------------------------------------
/** AUD_DspSetVolumeTable
 *   Set volume table.
 *
 *  @param  u4VolTabChkPnt[]
 *
 *  @retval  void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspSetVolumeTable(UINT32* u4VolTabChkPnt);
extern void AUD_SetOutPortMode(UINT8 u1DecId, UINT8 fgMode);
#ifdef CC_AUD_LINEOUT_VOL_MODE
extern void AUD_DspSetLineOutVolumeMode(UINT8 u1DecId, UINT8 fgMode);
extern UINT8 AUD_DspGetLineOutFixVol(void);
extern BOOL AUD_GetLineOutVolumeMode(UINT8 u1DecId);
#endif
#ifdef CC_AUD_INPUT_SRC_LINEOUT_VOL
extern void AUD_DspSrcLinoutVol(UINT8 u1DecId, AUD_DEC_STREAM_FROM_T eStrmSrc, INT16 i2Vol);
#endif

//-----------------------------------------------------------------------------
/** AUD_DspGetVolumeTable
 *   Query volume table.
 *
 *  @param  u1VolTabIdx
 *
 *  @retval  volume gain
 */
//-----------------------------------------------------------------------------
#ifdef CC_AUD_NEW_HP_VOL_TABLE
extern void AUD_DspSetHpVolumeTable(UINT8 u1Idx, UINT32 u4Vol);
extern UINT32 AUD_DspGetHpVolumeTable(UINT8 u1VolTabIdx);
#endif
extern UINT32 AUD_DspGetVolumeTable(UINT8 u1VolTabIdx);
extern UINT32 AUD_DspSpkSizeQry(UINT8 u1DecId);
#ifdef CC_S_SUBWOFFER_SUPPORT  ///CC_BASS_MANGER_INDEP_LPF
extern UINT32 AUD_DspSpkSizeLpfQry(UINT8 u1DecId);
#endif

//-----------------------------------------------------------------------------
// AUD_DrvSetDDBanner
//
/** Turn on DDBanner.
 *
 *  @param     fgEnable     TRUE/FALSE
 *
 *  @retval    void
 */
//-----------------------------------------------------------------------------
extern void AUD_DrvSetDDBanner(BOOL fgEnable);

#if 0   // Unused
//-----------------------------------------------------------------------------
/** AUD_DspGetVolumeBaseLevel
 *   Get volume base level (0dB level).
 *
 *  @retval  void
 */
//-----------------------------------------------------------------------------
extern UINT8 AUD_DspGetVolumeBaseLevel(void);
#endif

extern void Aud_SetTvSystemMask(UINT8 u1DecId, UINT32 u4Mask);
extern UINT32 Aud_GetTvSystemMask(UINT8 u1DecId);
extern void AUD_DspADModeSet(UINT8 u1DecId, UINT16 u2Value);
extern void AUD_DspADEnable(UINT8 u1DecId, BOOL fgEnable);
#ifdef CC_AUD_SKYPE_SUPPORT
extern void AUD_DspSkypeRingMix(BOOL fgEnable);
extern void AUD_DspAuxMixToMain(BOOL fgEnable);
#endif

#ifdef CC_AUD_PCM_LINE_IN_LPF
extern void AUD_DSPSetPCMLineInLPF(BOOL fgEnable);
#endif

extern void AUD_DspSetPcmPreBuffer (UINT8 u1DspId, UINT8 u1DecId, UINT16 u2Length);
extern UINT16 AUD_DspGetPcmPreBuffer(UINT8 u1DspId, UINT8 u1DecId);
#ifdef CC_SUPPORT_AUTO_DETECT_AUD_SYSTEM
extern void Aud_SetTvAudSystemMask(UINT8 u1DecId, UINT32 u4TvMask, UINT32 u4TvAudMask, ISO_3166_COUNT_T country_code);
#ifdef CC_AUD_DDI
extern AUD_FMT_T AudAtvFmtDetection(UINT8 u1DecId);
#else
extern AUD_FMT_T AudAtvFmtDetection(void);
#endif
#endif
extern void AUD_PCM_Detect_Raw_Mute(BOOL fgFlag);
extern BOOL AUD_Is_PCMDetectRaw(void);

//-----------------------------------------------------------------------------
/** AUD_Surround_Get_Config
 *   Set new mtk vsurr config
 *  @retval  void
 */
//-----------------------------------------------------------------------------
extern void AUD_Surround_Get_Config(AUD_VSURR_CFG_T* prVsurrCfg);

//-----------------------------------------------------------------------------
/** AUD_Surround_Set_Config
 *   Set new mtk vsurr config
 *  @retval  void
 */
//-----------------------------------------------------------------------------
extern void AUD_Surround_Set_Config(AUD_VSURR_CFG_TYPE_T eVSCfgType, UINT32 u4VSParameter);

//audio depths links with video depths, by gallen/sammy, 20110526
//-----------------------------------------------------------------------------
/** AUD_3D_AVLINK_Config
 *   Set new mtk vsurr width config according to 3D video depth
 *  @retval  void
 */
//-----------------------------------------------------------------------------
#ifdef CC_3DTV_AUDIO_LINK_SUPPORT
extern void AUD_3D_AVLINK_Config(UINT32 u4DepthField);
#endif

//-----------------------------------------------------------------------------
/** AUD_SendApllAdjustCmd
 *  Send APLL MODIN adjust command
 *
 *  @param  u4Level    video calculated level (0 ~ 255. 128 as center)
 *                     bit 8: 0 (slower), 1 (faster)
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void AUD_SendApllAdjustCmd(UINT32 u4Level);

//-----------------------------------------------------------------------------
/** AUD_GetNicamStatus
 *  Get Nicam exists or not
 *
 *  @param  void
 *
 *  @retval TRUE/FALSE
 */
//-----------------------------------------------------------------------------
extern BOOL AUD_GetNicamStatus(void);

//-----------------------------------------------------------------------------
/** AUD_GetDtvAudInfo
 *  Get DTV info
 *
 *  @param  u1DecId
 *  @param  prAudInfo
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void AUD_GetDtvAudInfo(UINT8 u1DecId, AUD_DEC_INFO_T* prAudInfo);


//-----------------------------------------------------------------------------
/** AUD_DspSpeed
 *  Set dsp speed (non-fractional)
 *
 *  @param  u1DecId 0: first decoder 1: seconder decoder.
 *  @param  u1Speed  : 0:normal play
 *                            : 2:FF 2-times
 *                            : 4:FF 4-times
 *                            : 8:FF 8-times
 *                            : 16:FF 16-times
 *                            : 32:FF 32-times
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspSpeed(UINT8 u1DecId, UINT16 u2Speed);

//-----------------------------------------------------------------------------
/** AUD_DspDownmixPosition
 *  Set Downmix Channel (CH9/10) Position
 *
 *  @param  u1DMPosition   : 0: do ALL post processing
 *                         : 1: do downmix after input source gain and before AVC
 *                         : 2: Skip post-proc & do manual output matrix
 *                         : 3: Skip post-proc & skip manual output matrix
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspDownmixPosition(UINT8 u1DspId, UINT8 u1DMPosition);
extern UINT8 AUD_DspGetDownmixPosition(UINT8 u1DspId);

//-----------------------------------------------------------------------------
/** AUD_DspMultiPairOutput
 *  Copy CH9/10 to other channel(LS/RS/C/SUB)
 *
 *  @param  u1DecId 0       : first decoder 1: seconder decoder.
 *  @param  u1MultiPair   : 0: OFF
 *                                   : 1: ON

 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspMultiPairOutput(UINT8 u1DspId, UINT8 u1DecId, UINT16 u1MultiPair);

//-----------------------------------------------------------------------------
/** _AUD_DspLRDownmix
 *  Downmix to L/R channel after Reverbation and before BassManagement
 *
 *  @param  u1DecId 0       : first decoder 1: seconder decoder.
 *  @param  u1MultiPair   : 0: OFF
 *                                   : 1: ON

 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspLRDownmix(UINT8 u1DspId, UINT8 u1DecId, BOOL fgEnable);
extern UINT16 AUD_DspGetSystemSetup(UINT8 u1DspId, UINT8 u1DecId);

//-----------------------------------------------------------------------------
/** AUD_SetLpcmTable
 *  Set Lpcm decoder parameters
 *
 *  @param  u1DecId 0: first decoder 1: seconder decoder.
 *  @param  AUD_LPCM_SETTING_T
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void AUD_SetLpcmTable(UINT8 u1DecId, AUD_LPCM_SETTING_T rLpcmSetting);
extern UINT32 AUD_GetLPCMSampleNum(UINT8 u1DecId, UINT32 u4Length);
extern UINT32 AUD_GetLPCMType(void);

//-----------------------------------------------------------------------------
// AUD_DrvAc3Setting
//
/** AC3 format setting
 *
 *  @param      u1DecId          Decoder ID (AUD_DEC_MAIN or AUD_DEC_AUX)
 *  @param      prPcmSetting
 */
//-----------------------------------------------------------------------------
extern void AUD_DrvAc3Setting(UINT8 u1DecId, const AUD_AC3_SETTING_T* prAc3Setting);

//-----------------------------------------------------------------------------
// AUD_DrvFlacSetting
//
/** FLAC format setting
 *
 *  @param      u1DecId          Decoder ID (AUD_DEC_MAIN)
 *  @param      prPcmSetting
 *  paul_flac
 */
//-----------------------------------------------------------------------------
extern void AUD_DrvFlacSetting(UINT8 u1DecID, AUD_FLAC_INFO_T* prFlacInfo);

#if 0   // Unused
//-----------------------------------------------------------------------------
/** AUD_DspSetDualDecMode
 *
 *  @param  enable/disable
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspSetDualDecMode(BOOL fgEnable);
#endif

#ifdef CC_AUD_LINEIN_USE_DEC3
extern void AUD_DspSetTriOnlyDecMode(BOOL fgEnable);
#endif
extern BOOL AUD_DspIsTriOnlyDecMode(void);

#ifdef CC_AUD_HPF_SUPPORT
//-----------------------------------------------------------------------------
/** AUD_DspSetHPFEnable
 *
 *  @param  enable/disable
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspSetHPFEnable(BOOL fgEnable);

//-----------------------------------------------------------------------------
/** _AUD_DspSetHPFFc
 *
 *  @param  enable/disable
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspSetHPFFc(UINT16 u2Fc);
extern void AUD_DspSetHPF2Fc(UINT16 u2Fc);

#endif

#ifdef DSP_SUPPORT_TCL_VBASS

extern void AUD_DspSetTVBEnable(BOOL fgEnable);

#endif


#ifdef CC_AUD_CLIPPER_SUPPORT
//-----------------------------------------------------------------------------
/** _AUD_DspSetClipperEnable
 *
 *  @param  enable/disable
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspSetClipperEnable(BOOL fgEnable);

//-----------------------------------------------------------------------------
/** AUD_DspSetClipperValue
 *
 *  @param  vaeChannel
 *  @param  u4Value
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspSetClipperValue(AUD_CH_T eChannel, UINT32 u4Value);
#endif

//-----------------------------------------------------------------------------
/** _AUD_DspSetPEQEnable
 *
 *  @param  enable/disable
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern BOOL AUD_DspGetPEQEnable(void);
extern void AUD_DspSetPEQEnable(BOOL fgEnable);

#ifdef CC_AUD_SPEAKER_HEIGHT
#ifndef CC_AUD_SPEAKER_HEIGHT_MODE
//-----------------------------------------------------------------------------
/** AUD_DspSetSPHEnable
 *
 *  @param  enable/disable
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspSetSPHEnable(BOOL fgEnable);
#else
//-----------------------------------------------------------------------------
/** AUD_DspSetSPHMode
 *
 *  @param  mode
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspSetSPHMode(UINT8 u1Mode);
#endif
#endif

//-----------------------------------------------------------------------------
/** AUD_DspSetPEQCfg2
 *
 *  @param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
 *  @param u1Set                      PEQ set (0 ~ 7)
 *  @param u4Frequency                Frequency (20 ~ 20K)
 *  @param eQValue                    Q
 *  @param i2Gain                     Gain (+-12dB) with 0.5dB/step
 *  @retval void
 */
//-----------------------------------------------------------------------------
#if 0 //sharp Q setting
extern void AUD_DspSetPEQCfg2(UINT8 u1DecId, UINT8 u1PEQSet, UINT32 u4Frequency,
                              AUD_PEQ_Q_TYPE_T eQValue, INT16 i2Gain);
#else
extern void AUD_DspSetPEQCfg2(UINT8 u1DecId, UINT8 u1PEQSet, UINT32 u4Frequency,
                              UINT32 eQValue, INT16 i2Gain);
extern void AUD_DspGetPEQCfg2(UINT8 u1DecId, UINT8 u1PEQSet, UINT32* u4Frequency,
                              UINT32* eQValue, UINT32* u4Gain);
#ifdef CC_AUD_PEQ_LR
extern void AUD_DspSetPEQCfg2_R(UINT8 u1DecId, UINT8 u1PEQSet, UINT32 u4Frequency,
                            UINT32 eQValue, INT16 i2Gain);
extern void AUD_DspGetPEQCfg2_R(UINT8 u1DecId, UINT8 u1PEQSet, UINT32 *u4Frequency,
                            UINT32 *eQValue, UINT32 *u4Gain);    
#endif
#endif

//-----------------------------------------------------------------------------
/** AUD_DspSetPEQCfg
 *
 *  @param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
 *  @param u1Set                      PEQ set (0 ~ 7)
 *  @param u4Frequency                Frequency (20 ~ 20K)
 *  @param u2BW                       Bandwidth (xx Hz)
 *  @param i2Gain                     Gain (+-12dB) with 0.5dB/step
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspSetPEQCfg(UINT8 u1DecId, UINT8 u1PEQSet, UINT32 u4Frequency,
                             UINT16 u2BW, INT16 i2Gain);
#ifdef CC_AUD_PEQ_LR
extern void AUD_DspSetPEQCfg_R(UINT8 u1DecId, UINT8 u1PEQSet, UINT32 u4Frequency,
                            UINT16 u2BW, INT16 i2Gain);
#endif

#ifdef CC_AUD_VBASS_SUPPORT
//-----------------------------------------------------------------------------
/** AUD_DspSetVirBassEnable
 *
 *  @param  enable/disable
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspSetVirBassEnable(BOOL fgEnable);

//-----------------------------------------------------------------------------
/** AUD_DspSetPEQCfg
 *
 *  @param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
 *  @param u4Frequency                Frequency (20 ~ xx)
 *  @param i2Gain                     Gain (+-12dB) with 0.5dB/step
 *  @param i2Gain2                    Gain2 (+-12dB) with 0.5dB/step
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspSetVirBassCfg(UINT8 u1DecId, UINT32 u4Frequency, UINT32 u4Gain,
                                 UINT32 u4Gain2);
extern VOID AUD_SetVBassCutOff(UINT8 u1Idx);
extern UINT8 AUD_GetVBassCutOff(void);
extern void AUD_DspGetVirBassCfg(UINT8 u1DecId, BOOL* fgEnable, UINT32* u4Gain, UINT32* u4Gain2);
#endif

#ifdef CC_AUD_FIR_SUPPORT
extern void AUD_DspSetFIREnable(UINT8 u1DecId, UINT8 u1ConeqMode);
#endif

extern void AUD_SetHeadphoneCond(UINT8 u1DecId, UINT8 u1HPCond);

extern UINT8 AUD_GetHeadphoneCond(UINT8 u1DecId);

#ifdef CC_AUD_USE_NVM
extern UINT32 AUD_NVM_Get_Profile(SMART_AUD_GROUP_INFO_T* prSmartAudioInfo);
extern UINT8 AUD_NVM_Get_Profile_Id(void);
extern void AUD_NVM_Get_FlashAQ_List(UINT8 u1Profile, UINT8 *pu1Data);
extern UINT32 AUD_NVM_Read(void);
#endif

#ifdef CC_AUD_DSP_SUPPORT_AUDYSSEY
extern void AUD_DspADVEnable(UINT8 fgEnable);
extern void AUD_DspADVStatus(void);
extern void AUD_DspADVSetChGain(UINT8 ch, UINT32 value);
extern void _AudADVQuery(void);
extern void _AUD_DspSetADVEnable(UINT8 fgEnable);
extern void AUD_DspSetABXEnable(UINT8 fgEnable);
extern void AUD_DspABXQuery(void);
extern void AUD_DspABXConfig(UINT8 item, UINT8 val);
extern void AUD_DspSetAEQEnable(BOOL fgEnable);
extern void AUD_DspAEQEnable(UINT8 u1DecId, BOOL fgEnable);
extern void AUD_DspAEQQuery(void);
#endif

extern void AUD_DspDDCOEnable(BOOL fgEnable);

extern void AUD_DspDDCOAgcEnable(BOOL fgEnable);

extern void AUD_DspDDCOLFEEnable(BOOL fgEnable);

//#ifdef ENABLE_AUDIO_RECORD_SUPPORT
extern void AUD_DspUploadDataEnable(void);
extern void AUD_DspSetUploadMode(UINT8 u1Mode);
extern void AUD_DataUpload_Init (void);
extern void AUD_DataUpload_start(void);
extern void AUD_GetUploadInfo(UINT32* u4Pts, UINT32* u4Addr, UINT32* u4Size);
extern void AUD_GetYWUploadInfo(UINT32* u4base, UINT32* u4Size);

extern void AUD_GetUploadPCMInfo(UINT32* uPCMBits, UINT32* uPCMChannels, UINT32* uPCMSampleRate);

extern void AUD_DspSetEncMode(UINT8 u1DecId, AUD_ENC_MODE_T eEncMode);
extern void AUD_DspSetEncInfo(UINT8 u1DecId, UINT32 u4Bitrates);
extern void AUD_DspBluetoothDataEnable(void);
extern void AUD_DspSetBluetoothCnt(UINT8 cnt);
extern void AUD_DspSetBluetoothMode(UINT8 u1Mode);
extern void AUD_DataBluetooth_Init (void);
extern void AUD_DataBluetooth_start(void);
extern void AUD_GetBluetoothInfo(UINT32* u4Pts,UINT32* u4Addr, UINT32* u4Size);
extern void AUD_GetYWBluetoothInfo(UINT32* u4base, UINT32* u4Size);

extern void AUD_GetBluetoothPCMInfo(UINT32* uPCMBits, UINT32* uPCMChannels, UINT32* uPCMSampleRate);

#if 0   // Unused
//-----------------------------------------------------------------------------
// AUD_ReadDspPROM
//
/** Read PROM 16-bits instruction.
 *  NOTICE: Do not use this function with Rom booting!
 *
 *  @param[in]        u4Addr           read PROM address
 *
 *  @retval PROM value
 */
//-----------------------------------------------------------------------------
extern UINT16 AUD_ReadPROM (UINT8 u1DspId, UINT32 u4Addr);
#endif

//-----------------------------------------------------------------------------
// AUD_ReadDspDROM
//
/** Read DROM 24-bits data.
 *
 *  @param[in]        u4Addr           read DROM offset
 *
 *  @retval DROM value
 */
//-----------------------------------------------------------------------------
UINT16 AUD_ReadDROM (UINT8 u1DspId, UINT32 u4Addr);

//-----------------------------------------------------------------------------
// AUD_Read32ByteID
//
/** Read ROM 32 bytes ID.
 *
 *  @param[in]        u1ID_p           ROM ID pointer
 *                    u4Num            ROM ID byte number
 *
 *  @retval AUD_OK    Success
 *  @retval AUD_FAIL  Fail
 */
//-----------------------------------------------------------------------------
extern INT32 AUD_Read32ByteID (UINT8 u1DspId, UINT8 *u1ID_p, UINT32 u4Num);
extern void AUD_MWCtrl_Set(UINT8 u1DecId, BOOL fgPlay);
extern BOOL AUD_MWCtrl_Get(UINT8 u1DecId);


extern void AUD_GetDspPtsInfo (UINT8 u1DecId, UINT32* pu4DspPts, UINT32* pu4PtsPrsPnt);
extern void AUD_GetAudioTrickPts(UINT8 u1DecId, UINT32* prAudioPts, BOOL fgOldPts);
#ifdef CC_AUD_HDMI_SPDIF_CFG    ///[Joel] 20101011
extern void AUD_HdmiCfgAnalogOutput(AUD_OUTPUT_STATE u1OutputState);
extern void AUD_CfgSpdifByHDMI(AUD_HDMI_CHL_STATUS* uChlStatus);
extern void AUD_SpdifByHDMINtfy(void);
#endif

///For MTA mt_aud_mw.c use.
extern void AUD_SPDIF_Output_HDMI_Rx(BOOL fgEnable);

#ifdef CC_AUD_SETUP_MELODY
extern void Aud_PlaySetupMelody(UINT8 u1DecId, UINT32 u4Channel, UINT32 u4RepeatCount);
void Aud_GetSetupMelodyInfo(AUD_CLIP_TYPE_T* prAudClip);
#endif

extern void AUD_DspSetLogoMelody(UINT8 u1VolLevel, UINT8 u1HpVolLevel, BOOL fgEnable);

//-----------------------------------------------------------------------------
// AUD_DrvDmxIntThreshold
//
/** Set DMX interrupt threshold.
 *
 *  @param[in]          u1DecId         Decoder ID
 *                      u1Factor        threshold = 188 * u1Factor
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void AUD_DrvDmxIntThreshold(UINT8 u1DecId, UINT8 u1Factor);

//-----------------------------------------------------------------------------
// AUD_SetLPCMFmt
//
/** Set LPCM parameter
 *
 *  @param[in]          u4PriData         private data about lpcm format in VOB
 *
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void AUD_SetLPCMFmt(UINT8 u1DecId, UINT32 u4PriData);
#if 1   // Unused
extern void AUD_SetLPCMFmtPS(UINT8 u1DecId, UINT32 u4PriData);
extern void AUD_SetLPCMFmtTS(UINT8 u1DecId, UINT32 u4PriData);
extern void AUD_SetLPCMFmtWIFI(UINT8 u1DecId, UINT32 u4PriData);
#endif
//-----------------------------------------------------------------------------
// AUD_IsLpcmFmtPS
//
/** Query LPCM from AVI or Program Stream.
 *
 *  @param[in]          void
 *
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern BOOL AUD_IsLpcmFmtPS(void);
//-----------------------------------------------------------------------------
// AUD_LPcmSetting
//
/** Set LPCM parameter
 *
 *  @param[in]          u1DecId
 *                      prLPcmInfo
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void AUD_LPcmSetting(UINT8 u1DecId, AUD_LPCM_INFO_T* prLPcmInfo);

//-----------------------------------------------------------------------------
// AUD_Ac3Setting
//
/** AC3 format setting
 *
 *  @param      u1DecId          Decoder ID (AUD_DEC_MAIN or AUD_DEC_AUX)
 *  @param      prPcmSetting
 */
//-----------------------------------------------------------------------------
extern void AUD_Ac3Setting(UINT8 u1DecId, const AUD_AC3_SETTING_T* prAC3Setting);


//-----------------------------------------------------------------------------
// AUD_MM_Set_Dec_Fmt_Conflict
//
/** For SWDMX callback AUD_DRV to notify that audio fomat is conflict.
 *
 *  @param[in]        u1DecId          Decoder ID
 *                    fgFlag           Enable Flag
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void AUD_MM_Set_Dec_Fmt_Conflict(UINT8 u1DecId, BOOL fgFlag);
extern BOOL AUD_Is_MMDecFmtConflict(UINT8 u1DecId);
extern void AUD_MM_Set_Dec_Fmt_Scramble(UINT8 u1DecId, BOOL fgFlag);
extern void AUD_MM_Set_Avsync_event(UINT8 u1DecId);

extern BOOL AUD_Is_MMDecFmtScramble(UINT8 u1DecId);
extern INT32 AUD_MTS_CFO_IDX(void);
extern INT32 AudCmdAtvHdevDetQuery(void);

//-----------------------------------------------------------------------------
// AUD_MMClearEOSFlag
//
/** Clear Audio Multimedia EOS Flag
 *
 *  @param      u1DecId          Decoder ID (AUD_DEC_MAIN)
 */
//-----------------------------------------------------------------------------
extern void AUD_MMClearEOSFlag(UINT8 u1DecId);

//-----------------------------------------------------------------------------
// AUD_MMClearPesCnt
//
/** Clear Audio Multimedia Pes Count
 *
 *  @param      u1DecId          Decoder ID (AUD_DEC_MAIN)
 */
//-----------------------------------------------------------------------------
extern void AUD_MMClearPesCnt(UINT8 u1DecId);

//-----------------------------------------------------------------------------
/** AUD_DspGetMpegAudInfo
 *
 *  @param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
 *  @param *prAudInfo                audio info structure pointer
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspGetMpegAudInfo(UINT8 u1DecId, AUD_DEC_INFO_T* prAudInfo);

#if 0   // Unused
//-----------------------------------------------------------------------------
/** AUD_DspSetMp3PreparsingMode
 *
 *  @param  enable/disable
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspSetMp3PreparsingMode(BOOL fgEnable);

//-----------------------------------------------------------------------------
/** AUD_DspIsMp3Vbr
 *
 *  @param  void
 *  @retval true (VBR)  false(CBR)
 */
//-----------------------------------------------------------------------------
extern BOOL AUD_DspIsMp3Vbr(void);
#endif

#ifdef CC_AUD_DSP_SUPPORT_MIXSOUND_MEMPLAY
//-----------------------------------------------------------------------------
/** AUD_StopMixSndClip
 *
 *  @param u1StreamID           MixSound Stream ID (0~MAX_AUD_MIXSOUND_STREAM_NUM)
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void AUD_StopMixSndClip(UINT8 u1StreamID);

//-----------------------------------------------------------------------------
/** AUD_PauseMixSndClip
 *
 *  @param u1StreamID           MixSound Stream ID (0~MAX_AUD_MIXSOUND_STREAM_NUM)
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void AUD_PauseMixSndClip(UINT8 u1StreamID);

//-----------------------------------------------------------------------------
/** AUD_ResumeMixSndClip
 *
 *  @param u1StreamID           MixSound Stream ID (0~MAX_AUD_MIXSOUND_STREAM_NUM)
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void AUD_ResumeMixSndClip(UINT8 u1StreamID);

//-----------------------------------------------------------------------------
/** AUD_FlushMixSndClip
 *
 *  @param u1StreamID           MixSound Stream ID (0~MAX_AUD_MIXSOUND_STREAM_NUM)
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void AUD_FlushMixSndClip(UINT8 u1StreamID);


//-----------------------------------------------------------------------------
/** AUD_RepeatMixSndClip
 *
 *  @param u1StreamID           MixSound Stream ID (0~MAX_AUD_MIXSOUND_STREAM_NUM)
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void AUD_RepeatMixSndClip(UINT8 u1StreamID, UINT32 u1RepeatNum);

//-----------------------------------------------------------------------------
/** AUD_SetMixSndClip
 *
 *  @param u1StreamID           MixSound Stream ID (0~MAX_AUD_MIXSOUND_STREAM_NUM)
 *  @param u4Addr               MixSound Clip Memory Physical Address
 *  @param u4Size               MixSound Clip Size (in byte)
 *  @param u4Gain               MixSound Clip Gain (0db => 0x20000)
 *  @param u1StereoOnOff        MixSound Clip Mono or Stereo (FALSE: mono, TRUE: stereo)
 *  @param u4SampleRate         MixSound Clip Upsample Ratio
 *  @param u1BitDepth           MixSound Clip Bit Depth (8/16)
 *  @retval BOOL                TRUE - set succ, FALSE - set fail
 */
//-----------------------------------------------------------------------------
extern BOOL AUD_SetMixSndClip(UINT8 u1StreamID, UINT32 u4Addr, UINT32 u4Size, UINT32 u4Gain, UINT8 u1StereoOnOff, UINT32 u4SampleRate, UINT8 u1BitDepth, UINT8 u1Endian);

//-----------------------------------------------------------------------------
/** AUD_PlayMixSndClip
 *
 *  @param u1StreamID           MixSound Stream ID (0~MAX_AUD_MIXSOUND_STREAM_NUM)
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void AUD_PlayMixSndClip(UINT8 u1StreamID);
extern BOOL AUD_IsMixSndClipPlay(UINT8 u1StreamID);

extern void AUD_EnableMixSndClip(void);
extern void AUD_DisableMixSndClip(void);
extern void AUD_ToneGen_MemAlloc(void);
#ifdef CC_SUPPORT_STR
extern void AUD_MixsndResume(void);
#endif
extern void AUD_ClearMixSndClip(void);
//extern void AUD_EnableAlsaMixSnd(void);

//-----------------------------------------------------------------------------
/** AUD_QueryMixSndClip
 *
 *  @param void
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void AUD_QueryMixSndClip(void);

extern void AUD_PlayMixSndRingFifo(UINT8 u1StreamId, UINT32 u4SampleRate, UINT8 u1StereoOnOff, UINT8 u1BitDepth, UINT32 u4BufferSize); //ALSA_MIXSND_PATH
extern void AUD_StopMixSndRingFifo(UINT8 u1StreamId); //ALSA_MIXSND_PATH

extern void AUD_DspSetMixSndLength(UINT32 u4Len);
#ifndef NEW_MIXSOUND
extern void AUD_DspSetMixSndGain(UINT32 u4Gain);
extern void AUD_DspSetMixSndStereo(UINT8 bOnOff);
extern void AUD_DspSetMixSndUpSampleRatio(UINT8 bRatio);
#endif
extern UINT32 AUD_DspGetMixSndGain(void);
extern BOOL AUD_DspGetMixSndStereo(void);
extern void AUD_DspSetMixSndMemPlay(UINT8 bOnOff); //!NEW_MIXSOUND
extern BOOL AUD_DspIsMixSndPlay(void);
extern void AUD_DspSetMixSndChannel(UINT16 wCh);
extern UINT32 AUD_DspGetMixSndReadPtr(UINT8 u1StreamID);
extern void AUD_DspSetMixSndWritePtr(UINT8 u1StreamID, UINT32 u4WritePtr);
extern void AUD_DspShowMixSndStatus(UINT8 u1StreamID);
#ifdef CC_ENABLE_AOMX
extern void AUD_DspSetGstGain(UINT32 u4Gain);
extern void AUD_DspPcmVolume(UINT8 u1Value, UINT8 u1Value1);
#endif
#endif

#ifdef CC_VOCAL_MUTE
extern void AUD_VocalMuteOnOff(UINT8 bMode,UINT8 fgOnOff);  ///
extern void AUD_SetVoiceMuteFilter1(UINT8 u1Idx);
extern void AUD_SetVoiceMuteFilter2(UINT8 u1Idx);
extern UINT8 AUD_GetVocaleMuteFilter1(void);
extern UINT8 AUD_GetVocaleMuteFilter2(void);
#endif

extern void AUD_DspSetWithVideo(UINT8 u1DecId, BOOL fgIsWithVideo);
extern BOOL AUD_DspIsWithVideo(UINT8 u1DecId);

//-----------------------------------------------------------------------------
/** AUD_DspSetCapability
 *   Set Capability
 *
 *  @param      void
 *  @retval     void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspSetCapability(void);

//-----------------------------------------------------------------------------
/** AUD_DspGetCapability
 *   Get Capability
 *
 *  @param      void
 *  @retval     format capability
 */
//-----------------------------------------------------------------------------
extern UINT32 AUD_DspGetCapability(UINT8 u1Idx);

//-----------------------------------------------------------------------------
/** AUD_DspIsFormatValid
 *  Is format valid
 *
 *  @param      u4FormatInfo: format index
 *  @retval     TRUE(valid). FALSE(invalid)
 */
//-----------------------------------------------------------------------------
extern BOOL AUD_DspIsFormatValid(UINT32 u4FormatInfo);

extern BOOL AUD_DspIsSupportAC3(void);

extern BOOL AUD_DspIsSupportEAC3(void);

#if 0   // Unused
extern BOOL AUD_DspIsSupportDDCO(void);
#endif

//-----------------------------------------------------------------------------
/** AUD_DspCOOKParameter
 *  setup WMA information
 *
 *  @param  u1DecId 0: first decoder 1: seconder decoder.
 *  @param  prCookInfo  : pointer of COOK header information
 *  @retval void
 */
//-----------------------------------------------------------------------------
extern void AUD_DspCOOKParameter(UINT8 u1DecId, AUD_COOK_CFG_T* prCookInfo);

//sunman_aout
extern UINT32 AudGetAoutStatus(void);
extern void AudResetAoutStatus(void);

//sunman cfo
extern INT32 AUD_MTS_CFO_IDX(void);

extern UINT32 AUD_GetTargetCountry(void);

#ifdef CC_AUD_SETUP_MELODY
extern void AUD_PlaySetupMelody(UINT8 u1DecId, UINT32 u4Channel, UINT32 u4RepeatCount);
extern void AUD_SetSetupPlayStatue(BOOL fgIsFinish);
extern BOOL AUD_IsSetupMelodyFinish(void);
extern void AUD_StopSetupMelody(UINT8 u1DecId);
extern void AUD_DspSetMelodyVol(UINT8 u1Vol);
extern UINT8 AUD_GetSetupMelodyVol(void);
#endif

extern INT32 AUD_FlashAQReadEx(UINT8 u1AQPartition, UINT32 u4Offset, UINT8 *u1Data, UINT32 u4Length);
#ifdef SUPPORT_FLASH_AQ
extern void AUD_FlashAQInitEx(void);
extern void AUD_GetFlashAQVer(CHAR* str);
extern void AUD_FlashAQInitEx_LZMA(void);
#else
extern void AUD_GetFlashAQVer(CHAR* str);
#endif

#ifdef CC_AUD_USE_FLASH_AQ
extern INT32 Aud_FlashAQRead(UINT32 u4Offset, UINT8* u1Data, UINT32 u4Length);
extern void Aud_FlashAQWrite(UINT32 u4Offset, UINT8* u1Data, UINT32 u4size);
extern BOOL Aud_FlashAQCompVersion(CHAR* szCurVer, CHAR* szUpgVer);
extern void Aud_FlashAQ_SetDefault(void);
extern void Aud_FlashAQ_Fill_Data(BOOL fgDelayMode);
extern BOOL Aud_FlashAQ_GetDscrpt(AUD_AQ_DSCRPT_T** pAudFlashAQDscrpt);
extern BOOL Aud_FlashAQ_GetDscrpt1(AUD_AQ_DSCRPT_T** pAudFlashAQDscrpt);
extern void AUD_FlashAQ_Init(void);
extern BOOL Aud_FlashAQQuery(AUD_AQ_TYPE_T eAQDataType, VOID* *pData);
extern void _AUD_FlashAQOperation(UINT8 u1DecId, AUD_AQ_TYPE_T eAQ_Type, AUD_AQ_OP_TYPE_T eAQ_OPType, UINT16 u2Data);
extern UINT8 AUD_FlashAQ_GetTableNum(void);
extern void AUD_DspSetBassTrebleOffset(UINT8 u1BassOffset, UINT8 u1TrebleOffset);
extern void AUD_DspGetBassTrebleOffset(UINT8* u1BassOffset, UINT8* u1TrebleOffset);
#ifdef CC_SONY_2K14_FLASH_AQ
extern void _AUD_Downmix_Usage(BOOL fgUsage);
#endif
extern UINT8 AUD_FlashAQ_ModelQueryFunc(void);
extern void Aud_DspSetIndiaSpeakerGain(AUD_DEC_OVM_MODE_T eMode);
#endif /* CC_AUD_USE_FLASH_AQ */

extern BOOL ADAC_DigiAmp_ReadRam(AUD_AMP_ID_T u1AmpID, UINT8 u1Bank, UINT8 u1Addr, UINT8* u1Data, BOOL fgAllSet);
extern BOOL ADAC_DigiAmp_WriteRam(AUD_AMP_ID_T u1AmpID, UINT8 u1Bank, UINT8 u1Addr, AUD_AQ_AMP_RAM_DATA_T rAmpRamData, BOOL fgAllSet);
#ifdef CC_AUD_USE_FLASH_AQ
extern void ADAC_DigiAmp_SetChannelVol(AUD_AMP_ID_T u1AmpID, UINT8 u1ChId, UINT8 u1Value);
extern void ADAC_DigiAmp_SetDualDRC(AUD_AMP_ID_T u1AmpID, AUD_AQ_AMP_DRC_T eDrcData);
extern void ADAC_DigiAmp_SetPostScale(AUD_AMP_ID_T u1AmpID, UINT32 u4PostSclCh1, UINT32 u4PostSclCh2);
extern void ADAC_DigiAmp_SetPEQ(AUD_AMP_ID_T u1AmpID, AUD_AQ_EXTPEQ_DATA_T* rAQExtPeqData);
#endif

extern BOOL ADAC_STA381_ReadRam(AUD_AMP_ID_T u1AmpID, UINT8 u1Bank, UINT8 u1Addr, UINT8* u1Data, BOOL fgAllSet);
extern BOOL ADAC_STA381_WriteRam(AUD_AMP_ID_T u1AmpID, UINT8 u1Bank, UINT8 u1Addr, AUD_AQ_AMP_RAM_DATA_T rAmpRamData, BOOL fgAllSet);
extern void ADAC_STA381_SetChannelVol(AUD_AMP_ID_T u1AmpID, UINT8 u1ChId, UINT8 u1Value);
extern void ADAC_STA381_SetDualDRC(AUD_AMP_ID_T u1AmpID, AUD_AQ_AMP_DRC_T eDrcData);
extern void ADAC_STA381_SetPostScale(AUD_AMP_ID_T u1AmpID, UINT32 u4PostSclCh1, UINT32 u4PostSclCh2);
extern void ADAC_STA381_SetPEQ(AUD_AMP_ID_T u1AmpID, AUD_AQ_EXTPEQ_DATA_T* rAQExtPeqData);
extern void ADAC_STA381_SetSTCCoeff(AUD_AMP_ID_T u1AmpID, AUD_AQ_AMP_STC_COEFF_T* rAQAmpStcCoeff);
extern void ADAC_STA381_SetSTCFilter(AUD_AMP_ID_T u1AmpID, AUD_AQ_EXTPEQ_DATA_T* rAQExtPeqData);

extern void _TAS5717_SetPostScale(AUD_AMP_ID_T u1AmpID, UINT32 u4Value);
extern void _TAS5717_SetPreScale(AUD_AMP_ID_T u1AmpID, UINT32 u4Value);
extern void _TAS5717_SetChannelVol(AUD_AMP_ID_T u1AmpID, UINT8 u1ChId, UINT16 u2Value);

extern UINT8 AUD_GetPIPVideoPath(void);
extern UINT32 AUD_DspQueryChannelDelay(UINT8 u1ChIdx); //QUERY_DELAY
extern void AUD_SetSyncDbgLvl(UINT16 u2Flag);
extern UINT16 AUD_GetSyncDbgLvl(void);

extern DATA_ENDIAN_T AUD_GetDTSInfo (void);
extern void AUD_SetDTSInfo (DATA_ENDIAN_T endian);
extern void AUD_SetDTSFrameSize(UINT32 u4Size);
extern UINT32 AUD_GetDTSFrameSize (void);

#ifdef  CC_AUD_HDMI_PARSER_2_0_SUPPORT
extern void AUD_DrvGetParserDetect(UINT8 u1DecId, BOOL* pfgDeteced);
extern void AUD_DrvGetParserFormat(UINT8 u1DecId, AUD_FMT_T* peDecFmt);
extern AUD_FMT_T AUD_DrvPcToFormat(UINT8 u1DecId, UINT16 u2Pc);
#endif
#ifdef CC_MPEG_LEVEL_ADJ
extern void AUD_MpegLevelAdjust (UINT8 u1Level);
extern void AUD_DrvMpegLevelAdjust (UINT8 u1Level);
#endif

extern UINT32 AUD_GetSilence(UINT8 u1DecId);
extern void AUD_SetSilence(UINT8 u1DecId, UINT32 u4Threshold);

extern void AUD_SetVdoPictureOffStatus(BOOL fgEnable);
extern BOOL AUD_GetVdoPictureOffStatus(void);


extern BOOL Aud_IsHeadphoneExist(void);

extern void AUD_DspSetStcId(UINT8 u1DecId, UINT16 u2StcId); // 2_STC_Task
extern UINT8 AUD_DspQryStcID(UINT8 u1DecId);
extern void AUD_DspGetStcId(UINT8 u1DecId, UINT16* u2StcId);
#ifdef KARAOKE_MIC_LINE_IN2
extern void AUD_SetAout2MixAout1(UINT32 u4ChSetting);
#endif

#ifdef CC_ENABLE_AV_SYNC
extern void Aud_AddLogDump(CHAR acMsg[], INT32 i4Value); //CC_AUD_DUMP_LOG
extern void Aud_ListLogDump(void);
#endif
extern void AUD_OpenGamePcm(UINT8 u1DecId, AUD_FMT_T u1DecFmt, SAMPLE_FREQ_T SampleRate);

extern void AUD_CloseGamePcm(UINT8 u1DecId);
extern void AUD_GetPcmDelay(UINT32 u1DecId, UINT32* pdelay2);
extern UINT32 AUD_FillPcmBufer(UINT8 u1DecId, UINT32* lpBuffer2, UINT32 size2);

#if defined(CC_AUD_ARM_SUPPORT)
extern void vAprocReg_Write (UINT32 u4Offset, UINT32 u4Val);


extern UINT32 u4AprocCommSignal_send (UINT32 u4Idx, UINT32 u4Cmd, UINT32 u4Data0, UINT32 u4Data1);
extern UINT32 u4ReadProc(UINT32 u4Addr);
extern void vWriteProc(UINT32 u4Addr, UINT32 u4Data);
extern void vAprocReg_Write (UINT32 u4Offset, UINT32 u4Val);
extern void vAprocReg_Write_Multi (UINT32 u4Offset, UINT32* u4Val, UINT32 u4Count);
extern UINT32 u4AprocReg_Read (UINT32 u4Offset);
extern void vAproc_NotifyAdsp(UINT32 u4Idx, UINT32 u4Cmd, UINT32 u4Data);

extern void vAprocDec_Open (UINT32 u4MixerId);
extern void vAprocDec_Close (UINT32 u4MixerId);
extern void vAprocPostProc_Open (void);;
extern void vAprocPostProc_Close (void);;
#ifdef CC_AUD_ARM_RENDER
extern void vAprocDataPort_Write (UINT32 u4Offset, UINT32 *pu4Data, UINT32 u4Num);
extern void vAprocDataPort_Read (UINT32 u4Offset, UINT32 *pu4Data, UINT32 u4Num);
extern void _vAUD_Aproc_Set (UINT32 u4Type, UINT32 u4Item, UINT32 *pu4Addr, UINT32 u4Num);
extern void _vAUD_Aproc_Get (UINT32 u4Type, UINT32 u4Item, UINT32 *pu4Addr, UINT32 u4Num);
extern BOOL _AUD_DspGetAprocEqEnable(APROC_IOCTRL_T ePort, BOOL isFir);
extern void _AUD_DspSetAprocEqEnable(APROC_IOCTRL_T ePort, BOOL isFir, BOOL fgEnable);
extern BOOL bIsSupportAproc (void);
extern void vAproc_Open (void);
extern void vAprocMain_Open(void);
extern void vAprocMain_Close (void);
extern void AUD_AprocEnableMixSnd(UINT8 u1MixID); //parson mixsnd
extern void AUD_AprocDisableMixSnd(UINT8 u1MixID);
extern void AUD_AprocEnableUpload(void);
extern void AUD_AprocDisableUpload(void);
extern void AUD_AprocEnableRiscPost(void); //parson riscpost
extern void AUD_AprocDisableRiscPost(void);
extern void vAprocAqData_Write (UINT32 u4Offset, UINT8 *pu1Data, UINT32 u4Num);
extern void vAprocAqData_Read (UINT32 u4Offset, UINT8 *pu1Data, UINT32 u4Num);
extern void _vAUD_Aproc_SetMp3Enc(BOOL fgEnable);
#ifdef ARM_SUPPORT_SRS_SS
extern void _vAproc_PostProcSRSAEQ_Set(UINT32 u4type,UINT32 u4v0);
extern void _vAproc_PostProcSRSAEQ_Get(void);
extern void _vAproc_PostProcSRSBT_Set(UINT32 u4type,UINT32 u4v0);
extern void _vAproc_PostProcSRSBT_Get(void);
extern void _vAproc_PostProcSRSCC3D_Set(UINT32 u4type,UINT32 u4v0);
extern void _vAproc_PostProcSRSCC3D_Get(void);
extern void _vAproc_PostProcSRSCSHP_Set(UINT32 u4type,UINT32 u4v0);
extern void _vAproc_PostProcSRSCSHP_Get(void);
extern void _vAproc_PostProcSRSGEQ_Set(UINT32 u4type,UINT32 u4v0);
extern void _vAproc_PostProcSRSGEQ_Get(void);
extern void _vAproc_PostProcSRSHL_Set(UINT32 u4type,UINT32 u4v0);
extern void _vAproc_PostProcSRSHL_Get(void);
extern void _vAproc_PostProcSRSHPF_Set(UINT32 u4type,UINT32 u4v0);
extern void _vAproc_PostProcSRSHPF_Get(void);
extern void _vAproc_PostProcSRSMVOL_Set(UINT32 u4type,UINT32 u4v0);
extern void _vAproc_PostProcSRSMVOL_Get(void);
extern void _vAproc_PostProcSRSSPHPF_Set(UINT32 u4type,UINT32 u4v0);
extern void _vAproc_PostProcSRSSPHPF_Get(void);
extern void _vAproc_PostProcSRSSRS3D_Set(UINT32 u4type,UINT32 u4v0);
extern void _vAproc_PostProcSRSSRS3D_Get(void);
extern void _vAproc_PostProcSRSSS3D_Set(UINT32 u4type,UINT32 u4v0);
extern void _vAproc_PostProcSRSSS3D_Get(void);
extern void _vAproc_PostProcSRSTSHD_Set(UINT32 u4type,UINT32 u4v0);
extern void _vAproc_PostProcSRSTSHD_Get(void);
extern void _vAproc_PostProcSRSTBHD_Set(UINT32 u4type,UINT32 u4v0);
extern void _vAproc_PostProcSRSTBHD_Get(void);
extern void _vAproc_PostProcSRSTRUD_Set(UINT32 u4type,UINT32 u4v0);
extern void _vAproc_PostProcSRSTRUD_Get(void);
extern void _vAproc_PostProcSRSTVOL_Set(UINT32 u4type,UINT32 u4v0);
extern void _vAproc_PostProcSRSTVOL_Get(void);
#endif

extern void _vAproc_PostProcEqSpec_Get(void);
extern void _vAproc_PostProcEqSpec_Set(UINT32 u4v0);

#endif //CC_AUD_ARM_RENDER

#endif //CC_AUD_ARM_SUPPORT

void AUD_MMSetSwdmxSrcId(UINT8 u1DecId, UINT8 u1SwdmxSrcId);

#ifdef CC_AUD_HP_DEPOP  ///For Remy, need remove "CC_DAC_DE_POP".
extern void _AudHpDePopTaskStatusChange(AUD_HP_DEPOP_CMD_T u4Message);
#endif

#ifdef KARAOKE_MIC_LINE_IN2
extern void AUD_MicInput_Enable(BOOL fgEnable);
#endif
extern void ADAC_SIFSrcSel(UINT32 isUseSIF);
#ifdef CC_AUD_DDI
extern void vSoundBarUpdate(void);
extern void AUD_CheckPvrPause(INT32 i4Speed);
extern void AUD_SetDelayMode (BOOL fgEnable);
#endif
extern void AUD_PlayMuteEnable(BOOL fgEnable);
extern UINT32 vGetFBMCMMSize(void);
extern void AUD_SetDelayMode (BOOL fgEnable); 
#ifdef CC_AUD_DDI
extern void AUD_LineInHWCtrlEnable(UINT8 u1DecId, BOOL fgEnable);
#endif
#ifdef CC_SUPPORT_STR
extern void AUD_PostProc_Resume(void);
extern void AUD_PostProc_Suspend(void);
extern void AUD_SetUiSpkOnOff(BOOL fgEnable);
#endif
extern void AUD_QueryPacketCount(UINT8 u1DecId,  UINT32 * pPacketCount);
extern void AUD_QueryFrameCount(UINT8 u1DecId,  UINT32 * pFrameCount);
extern void AUD_QueryDecoderStatus(UINT8 u1DecId,  AUD_DEC_STATE * pDecoderStatus);
extern void AUD_QueryStreamType(UINT8 u1DecId,  AUD_FMT_T * pStreamType);
extern void AUD_QuerySampleRate(UINT8 u1DecId,  UINT32 * pSampleRate);
extern void AUD_QueryStc(UINT8 u1DecId,  UINT32 * pStc);
extern void AUD_QueryPts(UINT8 u1DecId,  UINT32 * pPts);
extern void AUD_QueryFirstPts(UINT8 u1DecId,  UINT32 * pPts);

#if defined(CC_AUD_ARM_SUPPORT) && defined(CC_AUD_ARM_RENDER)
extern BOOL AUD_Aproc_Chk_Notify(UINT8 *u1DecId, UINT32 u4NfyTag, UINT32 eNfyCond);
extern BOOL  AUD_Aproc_Chk_Cmd(UINT8 u1DecId);
extern void AUD_Aproc_Chk_DecId(UINT8* u1DecId);
extern void AUD_Aproc_OutCtrl(UINT8 u1DecId, APROC_MW_CTRL_T *arMwCtrl);
extern void AUD_Aproc_ChkTVDecInfoChgNfy(UINT8 u1DspId, UINT8 u1DecId);
#ifdef CC_DUAL_AUD_DEC_SUPPORT
extern void AUD_Aproc_SetFocus(UINT8 u1DecId, UINT8 u1Focus);
extern void AUD_Aproc_ChgFocus(UINT8 u1DecId, UINT8 u1Focus);
extern void AUD_Aproc_Show_SkipList(void);
extern UINT8 AUD_Aproc_ChkAVPVR(UINT8 u1DecId);
#endif  //CC_DUAL_AUD_DEC_SUPPORT
#if defined(CC_AUD_APROC_KEYSOUND)
extern void AUD_AprocEnableKeySnd(void);
extern void AUD_AprocDisableKeySnd(void);
#endif
#endif //CC_AUD_ARM_RENDER


extern UINT32 AUD_GetAoutBandNum(UINT8 u1DecId);
extern void AUD_FlushAudioFifo(UINT8 u1DecId);


#if defined(CC_AUD_ARM_SUPPORT) && defined(CC_AUD_ARM_RENDER)
extern void AUD_Aproc_SetMp3Enc(BOOL fgEnable);
extern BOOL AUD_Aproc_GetMp3Enc(void);
extern UINT32 AudDecId2MixerId(UINT8 u1DecId);
#endif
extern UINT8 AUD_Check_Dual_DecId(UINT8 u1DecId);

#endif /* _AUD_IF_H_ */
