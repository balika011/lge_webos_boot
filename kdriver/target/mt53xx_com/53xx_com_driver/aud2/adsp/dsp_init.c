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
 * $Date: 2015/03/12 $
 * $RCSfile: dsp_init.c,v $
 * $Revision: #4 $
 *
 *---------------------------------------------------------------------------*/

/** @file dsp_init.c
 *  Brief of file dsp_init.c.
 *  Details of file dsp_init.c (optional).
 */


//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#define _DSP_INIT_C

#include "util.h"
#include "x_os.h"
//#include "x_chip_id.h"

#include "dsp_common.h"
#include "dsp_intf.h"
#include "dsp_shm.h"
#include "dsp_rg_ctl.h"
#include "dsp_func.h"
#include "d2rc_shm.h"
#include "dsp_table.h"
#include "dsp_data.h"

#include "aud_if.h"
#include "aud_drv.h"
#include "drvcust_if.h"
#include "aud_debug.h"

// Temporaray used -> should be removed
#include "dsp_reg.h"

#include "x_assert.h"
//#include "x_mid.h"

//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------

#define USE_16BIT_ALIGNMENT
//#define ATV_HDEV_AUTO_SWITCH



//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------

// Please refer to drv_default.h or customer's configuration files in \drv_cust\config\XXX.h
#define CUTOFF_FREQ             GetAudioBassMngCutOffFreq()
#ifdef CC_S_SUBWOFFER_SUPPORT  ///CC_BASS_MANGER_INDEP_LPF
///#define CUTOFF_FREQ_LPF             GetAudioBassMngCutOffFreqLpf()   ,[Joel] To Do
#endif
#define SOFT_MUTE_ORDER         DRVCUST_OptGet(eAudioSoftMuteOrder)
#define IS_BYPASS_PROC()        DRVCUST_OptGet(eAudioBypassPostProc)
//#define AVC_TARGET_LEV          DRVCUST_OptGet(eAudioAvcTarget)
//#define AVC_GAIN_UP             DRVCUST_OptGet(eAudioAvcGainUp)
#define AVC_ATTACK_THRES        DRVCUST_OptGet(eAudioAvcAttackThres)
//#define AVC_ADJUST_RATE         DRVCUST_OptGet(eAudioAvcAdjustRate)
#define IS_SPKCFG_SSLN()        (((u4ReadDspShmDWRD(AUD_DSP0, D_SPKCFG) & 0x1f020) == 0x18000)? 1:0 ) //andrew 07/1/8
//-----------------------------------------------------------------------------
// Imported variables
//-----------------------------------------------------------------------------
extern UINT16 _u2AenvSemaCtrl[AUD_DSP_NUM];
//-----------------------------------------------------------------------------
// Imported functions
//-----------------------------------------------------------------------------
extern UINT32 dwGetPEQFsIdx(UINT8 u1Set);
extern UINT32 dwGetPEQCosIdx(UINT8 u1Set);
#ifdef CC_AUD_PEQ_LR
extern UINT32 dwGetPEQFsIdx_R(UINT8 u1Set);
extern UINT32 dwGetPEQCosIdx_R(UINT8 u1Set);
#endif
extern UINT32 GetAudioBassMngCutOffFreq(void);
extern UINT32 GetAudioLRSpkTypeLarge(void);
extern UINT32 GetAudioSLRSpkTypeLarge(void);
extern UINT32 GetAudioCenterSpkTypeLarge(void);
extern UINT32 GetAudioCH910SpkTypeLarge(void);
extern UINT32 GetAudioSubwooferOn(void);
extern UINT32 GetAudioVsClarity(void);
extern UINT32 GetAudioVsWidth(void);
extern UINT32 GetAudioVsLRGain(void);
extern UINT32 GetAudioVsXTalk(void);
extern UINT32 GetAudioVsOutputGain(void);
extern UINT32 GetAudioVsBassGain(void);
extern UINT32 GetAudioVsFO(void);
extern UINT32 GetAudioLimiterMode(void);
extern UINT32 GetAudioLimiterThreshold(void);
#ifdef CC_AUD_FY12_LOUDNESS
extern void vEQ2Sbass2BandGainFlush (UINT8 u1DecId);
#endif

#ifdef CC_AUD_USE_NVM
extern UINT32 AUD_NVM_Init_From_EEPROM(void);
extern UINT32 AUD_NVM_QRY_ATV_GAIN(AUD_NVM_ATV_QRY_T eAudNvmQryT, AUD_NVM_ATV_FMT_T eAudQryFmt);
#endif

UINT32 _u4pbdchecksum;
UINT32 _u4_pbD_DDCchecksum;
UINT32 _u4_pbD_DCVchecksum;
UINT32 _u4_pbD_DDchecksum;
UINT32 _u4_pbD_DDTchecksum;
UINT32 _u4_pbD_AACchecksum;
UINT32 _u4_pbD_DDCOchecksum;


//UINT32 _u4adspRegion;

extern UINT8 _u1MtsFineVol;
extern UINT8 _u1SapFineVol;

//-----------------------------------------------------------------------------
// Static function forward declarations
//-----------------------------------------------------------------------------

static void vCustomOptInit (void);
static UINT8 vFineTuneVolTblIdxChk(UINT8 u1VolTabIdx);

//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------

void vDspReset(UINT8 u1DspId, UINT8 u1DecId);
void vDecCommonInit (UINT8 u1DspId);
void vDecCommonInitDec2 (UINT8 u1DspId);
#ifdef CC_MT5391_AUD_3_DECODER
void vDecCommonInitDec3 (UINT8 u1DspId);
#endif
#ifdef CC_AUD_4_DECODER_SUPPORT
void vDecCommonInitDec4 (UINT8 u1DspId);
#endif

#ifdef CC_AUD_USE_FLASH_AQ
extern void vDspFlashAQOperation(AUD_AQ_TYPE_T eAQType);
#endif

#ifndef CC_AUD_HDMI_PARSER_2_0_SUPPORT
extern UINT32 _AudGetHDMICase (void);
#endif

/***************************************************************************
     Function : _u4DspGetSpkConfig
  Description : Load speaker size configuration from drv_cust
    Parameter : None
    Return    : None
***************************************************************************/
static UINT32 _u4DspGetSpkConfig(void)
{
    UINT32 u4SpkCfg = 0x17;  // Defualt set as full channel

    if (GetAudioLRSpkTypeLarge())
    {
        u4SpkCfg |= ((UINT32)0x3 << 13);
    }
    if (GetAudioSLRSpkTypeLarge())
    {
        u4SpkCfg |= ((UINT32)0x3 << 15);
    }
    if (GetAudioCenterSpkTypeLarge())
    {
        u4SpkCfg |= ((UINT32)0x1 << 12);
    }
    if (GetAudioCH910SpkTypeLarge())
    {
        u4SpkCfg |= ((UINT32)0x3 << 19);
    }
    if (GetAudioSubwooferOn())
    {
        u4SpkCfg |= ((UINT32)0x1 << 5);
    }

    return u4SpkCfg;
}

/***************************************************************************
     Function : vDecCommonInit
  Description : This function is used to init the decoder EACH time when it
                is played. In other words, it is to set the properties which
                is different song by song.
    Parameter : None
    Return    : None
***************************************************************************/
void vDecCommonInit (UINT8 u1DspId)
{
    UINT16 u2DspData = 0;
    UINT32 dDspData;
    TV_AUD_SYS_T u1ChannelSrc;
    UINT16 u2ChannelSrcSet;
    UINT8 u1Temp1, u1Temp2;
    UINT32 u4Temp3;    
    UINT16 u2BypassMode = 0;

    UNUSED(u1Temp1);
    UNUSED(u1Temp2);

    //CTRL3 NUM_1
    // ... should be in switch below
    u2BypassMode = uReadDspShmBYTE (u1DspId, B_DELAY_MODE_BYPASS);
	  u2BypassMode = (u2BypassMode & uReadDspShmBYTE (u1DspId, B_DELAY_MODE));
    vWriteDspWORD (u1DspId, ADDR_RC2D_DELAY_MODE, u2BypassMode);
    vWriteDspWORD (u1DspId, ADDR_RC2D_CTRL_3_NUM_1, (UINT16)CTRL_3_NUM_1);
    // Bonding options
    vWriteDspWORD (u1DspId, ADDR_RC2D_BONDING_OPTIONS, u2ReadDspShmWORD (u1DspId, W_BONDING_OPTIONS));
    //Pink Noise Setting
    vWriteDspWORD (u1DspId, ADDR_RC2D_CHANNEL_USE, u2ReadDspShmWORD (u1DspId, W_PINKNOISE));
    //Soft Mute (Max. 7)
    vWriteDspWORD (u1DspId, ADDR_RC2D_VOLUME_SOFT_MUTE_ORDER,
        uReadDspShmBYTE (u1DspId, B_SOFTMUTEORDER));
    //Reset DMX rpnt
    WriteDspCommDram(u1DspId, ADDR_D2RC_RISC_INFO_REG_BUF_PNT , _u4AFIFO[u1DspId][AUD_DEC_MAIN][0]);   //(AUD_READ32(REG_DMX_STR) << 2)

    //PTS Table Shift Amount
    u4Temp3 = dReadDspCommDram(u1DspId, ADDR_RC2D_PTS_STC_CTRL_FLAG);
    u4Temp3 &= 0xFFBF00; //Clear bit 6 (align @ bit8)
    WriteDspCommDram (u1DspId, ADDR_RC2D_PTS_STC_CTRL_FLAG, u4Temp3);  


    DSP_FlushInvalidateDCache(u1DspId, CACHE_FLUSH_TYPE_DATA, TYPE_COMM_DRAM_IDX);

    switch (_rDspVars[u1DspId][AUD_DEC_MAIN].bDspStrTyp)
    {
    case PCM_STREAM:
        dDspData = wReadDspWORD (u1DspId, ADDR_D2RC_INPUT_CH_CFG);
        dDspData = u2DspData & ~0xf;
        dDspData = u2DspData | 0x2;  // Hard set cdda channel config
        vWriteDspWORD (u1DspId, ADDR_D2RC_INPUT_CH_CFG, dDspData);
        vWriteDspWORD (u1DspId, ADDR_RC2D_PCM_INPUT_CHANNEL_NUM,
                       u2ReadDspShmWORD (u1DspId, W_PCM_INPUT_CHANNEL_NUM));
        vWriteDspWORD (u1DspId, ADDR_RC2D_PCM_INPUT_CH_ASSIGNMENT,
                       u2ReadDspShmWORD (u1DspId, W_PCM_INPUT_CH_ASSIGNMENT));
        vWriteDspWORD (u1DspId, ADDR_RC2D_PCM_INPUT_TYPE,
                       u2ReadDspShmWORD (u1DspId, W_PCM_INPUT_TYPE));  //0:pcm24 1:IMA 3:MS 2:pcm16 //msb=1: little endian
        vWriteDspWORD (u1DspId, ADDR_RC2D_PCM_INPUT_SAMPLING_RATE,
                       u2ReadDspShmWORD (u1DspId, W_PCM_INPUT_SAMPLING_RATE));
        vWriteDspWORD (u1DspId, ADDR_RC2D_PCM_PREBUF_DELAY_BANK,
                       u2ReadDspShmWORD (u1DspId, W_PCM_PREBUF_DELAY_BANK));
#if 0
        vWriteDspWORD (ADDR_RC2D_ADPCM_BLOCK_ALIGN,
                       u2ReadDspShmWORD (W_ADPCM_BLOCK_ALIGN));
        vWriteDspWORD (ADDR_RC2D_ADPCM_CHANNEL_NUM,
                       u2ReadDspShmWORD (B_ADPCM_CHANNEL_NUM));
#endif
        break;
    case AC3_STREAM:
        //AC3 Karaoke Mode
        vWriteDspWORD (u1DspId, ADDR_RC2D_AC3_KARAOKE_MODE,
                       u2ReadDspShmWORD (u1DspId, W_AC3KARAMOD));
        //AC3 Dual Mono Mode
        vWriteDspWORD (u1DspId, ADDR_RC2D_AC3_DUAL_MODE,
                       u2ReadDspShmWORD (u1DspId, W_AC3DUALMODE));
        //AC3 Compression Mode
        vWriteDspWORD (u1DspId, ADDR_RC2D_AC3_COMPRESS_MODE,
                       u2ReadDspShmWORD (u1DspId, W_AC3COMPMOD));
        //AC3 Low Boost
        WriteDspCommDram (u1DspId, ADDR_RC2D_AC3_DYNAMIC_LOW,
                          u4ReadDspShmDWRD (u1DspId, D_AC3DYN_LOW));
        //AC3 High Cut
        WriteDspCommDram (u1DspId, ADDR_RC2D_AC3_DYNAMIC_HIGH,
                          u4ReadDspShmDWRD (u1DspId, D_AC3DYN_HIGH));
        // AC3 Auto downmix
        vWriteDspWORD (u1DspId, ADDR_RC2D_AC3_DOWNMIX,
                          uReadDspShmBYTE (u1DspId, B_AC3AUTODNMIX));

#ifdef CC_AUD_SUPPORT_MS10
	    // DDC control
        vWriteDspWORD (u1DspId, ADDR_RC2D_AC3_CONTROL, u2ReadDspShmWORD(u1DspId, W_DDC_CONTROL));
        // DDC associated stream id
        vWriteDspWORD (u1DspId, ADDR_RC2D_DDC_STRMID,  uReadDspShmBYTE (u1DspId, B_DDC_STRMID));
        // DDC mixer
        vWriteDspWORD (u1DspId, ADDR_RC2D_DDC_ASSOC_MIX, uReadDspShmBYTE (u1DspId, B_DDC_ASSOC_MIX));
#endif


#ifdef CC_AUD_HDMI_PARSER_2_0_SUPPORT
        vWriteDspWORD (u1DspId, ADDR_RC2D_AC3_CONTROL, u2ReadDspShmWORD(u1DspId, W_DDC_CONTROL) | (uReadDspShmBYTE(u1DspId, B_DEC_ENDIAN) ? 0x8 : 0x0));
#else
        /* Patch for wrong HDMI content from player
                Do not decode DDP and skip CRC error mute  if HDMI source to avoid this kind of HDMI content.
             */
        if (_AudGetHDMICase())
        {
            // force to DD and skip CRC error mute
            vWriteDspWORD(u1DspId, ADDR_RC2D_AC3_CONTROL, 0x4 | (uReadDspShmBYTE(u1DspId, B_DEC_ENDIAN) ? 0x8 : 0x0));
            LOG (3, "====> HDMI : DD, no CRC error mute\n");
        }
        else
        {
            vWriteDspWORD(u1DspId, ADDR_RC2D_AC3_CONTROL, u2ReadDspShmWORD(u1DspId, W_DDC_CONTROL) | (uReadDspShmBYTE(u1DspId, B_DEC_ENDIAN) ? 0x8 : 0x0));
            LOG (3, "====> HDMI : DDP, with CRC error mute\n");
        }
#endif
        break;

      case COOK_STREAM:
        vWriteDspWORD (u1DspId, ADDR_RC2D_COOK_SAMP_PER_FRAME,
            u2ReadDspShmWORD(u1DspId, W_COOK_SAMP_PER_FRAME));
        vWriteDspWORD (u1DspId, ADDR_RC2D_COOK_CHANNEL_NUM,
            u2ReadDspShmWORD(u1DspId, W_COOK_CHANNEL_NUM));
        vWriteDspWORD (u1DspId, ADDR_RC2D_COOK_INPUT_SAMPLING_RATE,
            u2ReadDspShmWORD(u1DspId, W_COOK_INPUT_SAMPLING_RATE));
        vWriteDspWORD (u1DspId, ADDR_RC2D_COOK_FRM_SZ_IN_BYTES,
            u2ReadDspShmWORD(u1DspId, W_COOK_FRM_SZ_IN_BYTES));
        vWriteDspWORD (u1DspId, ADDR_RC2D_COOK_REGION_NUM,
            u2ReadDspShmWORD(u1DspId, W_COOK_REGION_NUM));
        vWriteDspWORD (u1DspId, ADDR_RC2D_COOK_COUPLING_START_REGN,
            u2ReadDspShmWORD(u1DspId, W_COOK_COUPLING_START_REGN));
        vWriteDspWORD (u1DspId, ADDR_RC2D_COOK_COUPLING_QBITS,
            u2ReadDspShmWORD(u1DspId, W_COOK_COUPLING_QBITS));
        break;

    case MPEG12_STREAM:
        // MPEG sync mode
        vWriteDspWORD (u1DspId, ADDR_RC2D_MPEG_SYNC_MODE, 0x01);
        // MPEG deemphasis mode
        vWriteDspWORD (u1DspId, ADDR_RC2D_MPEG_DEEMPHASIS_MODE, 0x00);
         // MPEG CRC mode
        vWriteDspWORD (u1DspId, ADDR_RC2D_MPEG_CRC_MODE,
                       u2ReadDspShmWORD (u1DspId, W_MPEGERRDET));
        // MPEG DRC flag
        vWriteDspWORD (u1DspId, ADDR_RC2D_MPEG_DRC_FLAG, 0x00);
        break;
    case MPEG3_STREAM:
        // Normal play
        vWriteDspWORD (u1DspId, ADDR_RC2D_MP3_NORMAL_PLAY, 0x00);
        // MP3 sync mode
        vWriteDspWORD (u1DspId, ADDR_RC2D_MP3_SYNC_MODE, 0x01);
        // MP3 deemphasis mode
        vWriteDspWORD (u1DspId, ADDR_RC2D_MP3_DEEMPHASIS_MODE, 0x00);
        // MP3 CRC mode
        vWriteDspWORD (u1DspId, ADDR_RC2D_MP3_CRC_MODE,
                       u2ReadDspShmWORD (u1DspId, W_MPEGERRDET));
        // MP3 preparsing mode
        vWriteDspWORD (u1DspId, ADDR_RC2D_MP3_PREPARSING_MODE, 0x00);
        // MP3 smooth mode
        vWriteDspWORD (u1DspId, ADDR_RC2D_MP3_SMOOTH_FLAG, 0x00);
        // MP3 quality flag
        vWriteDspWORD (u1DspId, ADDR_RC2D_MP3_QUALITY_FLAG, 0x200);
        break;
#ifdef DATA_DISC_WMA_SUPPORT
    case WMA_STREAM:
        //WNA Packet No
        vWriteDspWORD (u1DspId, ADDR_RC2D_WMA_PACKET_NO,
                       u2ReadDspShmWORD (u1DspId, W_WMA_PACKET_NO));
        vWriteDspWORD (u1DspId, ADDR_RC2D_WMA_HEADER_SOURCE,
                       u2ReadDspShmWORD (u1DspId, W_WMA_HEADER_SOURCE));
        WriteDspCommDram (u1DspId, ADDR_RC2D_WMA_PACKET_COUNT,
                       ((u4ReadDspShmDWRD (u1DspId, D_WMA_PACKET_COUNT))<<8));
        WriteDspCommDram (u1DspId, ADDR_RC2D_WMA_PACKET_SIZE,
                       ((u4ReadDspShmDWRD (u1DspId, D_WMA_PACKET_SIZE))<<8));
        vWriteDspWORD (u1DspId, ADDR_RC2D_WMA_VERSION,
                       u2ReadDspShmWORD (u1DspId, W_WMA_VERSION));
        vWriteDspWORD (u1DspId, ADDR_RC2D_WMA_NUM_CHANNELS,
                       u2ReadDspShmWORD (u1DspId, W_WMA_NUM_CH));
        WriteDspCommDram (u1DspId, ADDR_RC2D_WMA_SAMPLE_PER_SECOND,
                       ((u4ReadDspShmDWRD (u1DspId, D_WMA_SAMPLE_PER_SEC))<<8));
        WriteDspCommDram (u1DspId, ADDR_RC2D_WMA_BYTES_PER_SECOND,
                       ((u4ReadDspShmDWRD (u1DspId, D_WMA_BYTE_PER_SEC))<<8));
        WriteDspCommDram (u1DspId, ADDR_RC2D_WMA_BLOCK_SIZE,
                       ((u4ReadDspShmDWRD (u1DspId, D_WMA_BLOCK_SIZE))<<8));
        vWriteDspWORD (u1DspId, ADDR_RC2D_WMA_ENCODER_OPTIONS,
                       u2ReadDspShmWORD (u1DspId, W_WMA_ENCODER_OPTION));
        //koro : if issue play without setting audio format (seek mode), wma need initial table
        vLoadDspDRAMTable (u1DspId, AUD_WMA_DEC1);
        vWriteDspWORD (u1DspId, ADDR_RC2D_DOWNLOAD_TABLE_FLAG, 0x1);

        break;
#endif
#if 1 //CC_APE_SUPPORT
    case APE_STREAM:


        vWriteDspWORD (u1DspId, ADDR_RC2D_CTRL_3_NUM_1, (UINT16)CTRL_3_NUM_1);
        // 1
        vWriteDspWORD (u1DspId, ADDR_RC2D_APE_PARSE_AUDIO_ONLY,
                       u2ReadDspShmWORD (u1DspId, W_APE_PARSE_AUDIO_ONLY));
        // 2
        vWriteDspWORD (u1DspId, ADDR_RC2D_APE_FILE_VESION,
                       u2ReadDspShmWORD (u1DspId, W_APE_FILE_VESION));
        // 3
        WriteDspCommDram (u1DspId, ADDR_RC2D_APE_COMPRESS_LEVEL,
                       ((u4ReadDspShmDWRD (u1DspId, D_APE_COMPRESS_LEVEL))<<8));
        // 4
        WriteDspCommDram32bit(u1DspId, ADDR_RC2D_APE_BLOCK_PER_FRAME,
                       (u4ReadDspShmDWRD (u1DspId, D_APE_BLOCK_PER_FRAME)));
        // 5
        WriteDspCommDram (u1DspId, ADDR_RC2D_APE_FINAL_FRAME_BLOCK,
                       (u4ReadDspShmDWRD (u1DspId, D_APE_FINAL_FRAME_BLOCK)));
        // 6
        //notes: it should set (((high << 16)+low) & 0xFFFF) to ADDR_RC2D_APE_TOTAL_FRAME_NUM_LOW
        vWriteDspWORD (u1DspId, ADDR_RC2D_APE_TOTAL_FRAME_NUM_LOW,
                       u2ReadDspShmWORD (u1DspId, W_APE_TOTAL_FRAME_NUM_LOW));
        // 7
        vWriteDspWORD (u1DspId, ADDR_RC2D_APE_TOTAL_FRAME_NUM_HIGH,
                       u2ReadDspShmWORD (u1DspId, W_APE_TOTAL_FRAME_NUM_HIGH));
        // 8
        vWriteDspWORD (u1DspId, ADDR_RC2D_APE_BITS_PER_SAMPLE,
                       u2ReadDspShmWORD (u1DspId, W_APE_BITS_PER_SAMPLE));
        // 9
        vWriteDspWORD (u1DspId, ADDR_RC2D_APE_CHANNEL_NUM,
                       (u2ReadDspShmWORD (u1DspId, W_APE_CHANNEL_NUM) -1));
        // A
        WriteDspCommDram (u1DspId, ADDR_RC2D_APE_INPUT_SAMPLING_RATE,
                       u4ReadDspShmDWRD (u1DspId, D_APE_INPUT_SAMPLING_RATE));
        // B
        vWriteDspWORD (u1DspId, ADDR_RC2D_APE_MUTE_BANK_NUMBERS,
                       u2ReadDspShmWORD (u1DspId, W_APE_MUTE_BANK_NUMBERS));
        // C
        vWriteDspWORD (u1DspId, ADDR_RC2D_APE_INVALID_BYTES,
                       u2ReadDspShmWORD (u1DspId, W_APE_INVALID_BYTES));

        break;
#endif
    case WMAPRO_STREAM:

        vWriteDspWORD (u1DspId, ADDR_RC2D_CTRL_3_NUM_1, (UINT16)0xC);
        //WNA Packet No
        vWriteDspWORD (u1DspId, ADDR_RC2D_WMAPRO_PACKET_NO,
                       u2ReadDspShmWORD (u1DspId, W_WMAPRO_PACKET_NO));
        vWriteDspWORD (u1DspId, ADDR_RC2D_WMAPRO_HEADER_SOURCE,
                       u2ReadDspShmWORD (u1DspId, W_WMAPRO_HEADER_SOURCE));
        vWriteDspWORD (u1DspId, ADDR_RC2D_WMAPRO_NUM_CHANNELS,
                       u2ReadDspShmWORD (u1DspId, W_WMAPRO_NUM_CH));
        WriteDspCommDram (u1DspId, ADDR_RC2D_WMAPRO_SAMPLE_PER_SECOND,
                       ((u4ReadDspShmDWRD (u1DspId, D_WMAPRO_SAMPLE_PER_SEC))<<8));
        WriteDspCommDram (u1DspId, ADDR_RC2D_WMAPRO_BYTES_PER_SECOND,
                       ((u4ReadDspShmDWRD (u1DspId, D_WMAPRO_BYTE_PER_SEC))<<8));
        WriteDspCommDram (u1DspId, ADDR_RC2D_WMAPRO_BLOCK_SIZE,
                       ((u4ReadDspShmDWRD (u1DspId, D_WMAPRO_BLOCK_SIZE))<<8));
        vWriteDspWORD (u1DspId, ADDR_RC2D_WMAPRO_ENCODER_OPTIONS,
                       u2ReadDspShmWORD (u1DspId, W_WMAPRO_ENCODER_OPTION));
        WriteDspCommDram (u1DspId, ADDR_RC2D_WMAPRO_CH_MASK,
                       ((u4ReadDspShmDWRD (u1DspId, D_WMAPRO_CH_MASK))<<8));
        vWriteDspWORD (u1DspId, ADDR_RC2D_WMAPRO_VALID_BITS,
                       u2ReadDspShmWORD (u1DspId, W_WMAPRO_VALID_BITS));
        vWriteDspWORD (u1DspId, ADDR_RC2D_WMAPRO_ADV_OPTION,
                       u2ReadDspShmWORD (u1DspId, W_WMAPRO_ADV_OPTION));
        WriteDspCommDram (u1DspId, ADDR_RC2D_WMAPRO_ADV_OPTION2,
                       ((u4ReadDspShmDWRD (u1DspId, D_WMAPRO_ADV_OPTION2))<<8));

        break;

    case PINK_NOISE_STREAM:
        //Pink Noise Setting
        /* 20041220: For testing, hard coded here, remove in the Future */
        //vWriteDspWORD (ADDR_RC2D_CHANNEL_USE, 0x06ff);
        //vWriteDspWORD (ADDR_RC2D_CHANNEL_USE, 0x4002);
        vWriteDspWORD (u1DspId, ADDR_RC2D_CHANNEL_USE, u2ReadDspShmWORD (u1DspId, W_PINKNOISE));
        break;
#ifdef DSP_SUPPORT_NPTV
    case NTSC_STREAM:
        vWriteDspWORD (u1DspId, ADDR_DEM_ADC_GAIN,u2ReadDspShmWORD (u1DspId, W_ADC_GAIN));
        vWriteDspWORD (u1DspId, ADDR_DEM_USER_MODE,u2ReadDspShmWORD (u1DspId, W_USER_MODE));
        vWriteDspWORD (u1DspId, ADDR_DEM_SOURCE_MODE,u2ReadDspShmWORD (u1DspId, W_SOURCE_MODE));
        vWriteDspWORD (u1DspId, ADDR_MTS_NUM_CHECK, u2ReadDspShmWORD (u1DspId, W_MTS_NUM_CHECK));
        vWriteDspWORD (u1DspId, ADDR_MTS_STEREO_MID,u2ReadDspShmWORD (u1DspId, W_MTS_STEREO_MID));
        vWriteDspWORD (u1DspId, ADDR_MTS_STEREO_CON_MID, u2ReadDspShmWORD (u1DspId, W_MTS_STEREO_CON_MID));
        vWriteDspWORD (u1DspId, ADDR_MTS_NUM_PILOT, u2ReadDspShmWORD (u1DspId, W_MTS_NUM_PILOT));
        vWriteDspWORD (u1DspId, ADDR_MTS_NUM_SAP, u2ReadDspShmWORD (u1DspId, W_MTS_NUM_SAP));
        vWriteDspWORD (u1DspId, ADDR_MTS_SAP_MID, u2ReadDspShmWORD (u1DspId, W_MTS_SAP_MID));
        vWriteDspWORD (u1DspId, ADDR_MTS_SAP_CON_MID,u2ReadDspShmWORD (u1DspId, W_MTS_SAP_CON_MID));
        vWriteDspWORD (u1DspId, ADDR_RC2D_MTS_LR_AMP_M, 0x14b);
        vWriteDspWORD (u1DspId, ADDR_RC2D_MTS_LR_AMP_M_576K, 0x124);
        vWriteDspWORD (u1DspId, ADDR_RC2D_MTS_LR_AMP_N, 0x140);
        vWriteDspWORD (u1DspId, ADDR_RC2D_MTS_LR_AMP_N_576K, 0x15a);
        if ((uReadDspShmBYTE (u1DspId, B_HDEV_MODE) & 0x10) == 0x10)
        {
            vWriteDspWORD (u1DspId, ADDR_HDEV_MODE, 0x2);  // set bit 1 for 576k mode
        }
	 else
	 {
        vWriteDspWORD (u1DspId, ADDR_HDEV_MODE, (uReadDspShmBYTE (u1DspId, B_HDEV_MODE) & 0x04)>>2);
	 }
        vWriteDspWORD (u1DspId, ADDR_CARRIER_SHIFT_MODE, (uReadDspShmBYTE (u1DspId, B_CARRIER_SHIFT_MODE) & 0x04)>>2);
        vWriteDspWORD (u1DspId, ADDR_FM_SATU_MUTE_MODE,  (uReadDspShmBYTE (u1DspId, B_FM_SATU_MUTE_MODE)& 0x04)>>2);
        vWriteDspWORD (u1DspId, ADDR_FM_CARRIER_MUTE_MODE, (uReadDspShmBYTE (u1DspId, B_FM_MUTE_MODE) & 0x04)>>2);
        vWriteDspWORD (u1DspId, ADDR_FM_CARRIER_MUTE_THRESHOLD_HIGHT, uReadDspShmBYTE (u1DspId, B_FM_MUTE_THRESHOLD_HIGHT_MTS));
        vWriteDspWORD (u1DspId, ADDR_FM_CARRIER_MUTE_THRESHOLD_LOW, uReadDspShmBYTE (u1DspId, B_FM_MUTE_THRESHOLD_LOW_MTS));
        vWriteDspWORD (u1DspId, ADDR_DEM_MTS_FINE_TUNE_VOLUME,u4ReadDspShmDWRD  (u1DspId, D_MTS_FINE_TUNE_VOLUME)>>8);
        vWriteDspWORD (u1DspId, ADDR_DEM_SAP_FINE_TUNE_VOLUME,u4ReadDspShmDWRD (u1DspId, D_SAP_FINE_TUNE_VOLUME)>>8);
        vWriteDspWORD (u1DspId, ADDR_MTS_PILOT_OFFSET_DETECTION,uReadDspShmBYTE (u1DspId, B_MTS_PILOT_OFFSET_DETECTION));
        vWriteDspWORD (u1DspId, ADDR_MTS_MUTE_SAP_LOW,uReadDspShmBYTE (u1DspId, B_MTS_MUTE_SAP_LOW));
        vWriteDspWORD (u1DspId, ADDR_MTS_MUTE_SAP_HIGH,uReadDspShmBYTE (u1DspId, B_MTS_MUTE_SAP_HIGH));
        vWriteDspWORD (u1DspId, ADDR_MTS_SATU_MUTE_HIGH,uReadDspShmBYTE (u1DspId, B_SATU_MUTE_THRESHOLD_HIGH));
        vWriteDspWORD (u1DspId, ADDR_MTS_SATU_MUTE_LOW,uReadDspShmBYTE (u1DspId, B_SATU_MUTE_THRESHOLD_LOW));
#ifndef CC_AUD_SX1_FEATURE
        vWriteDspWORD (u1DspId, ADDR_DEFAULT_MTS_MODE, 0);  // no default mode
#else
        vWriteDspWORD (u1DspId, ADDR_DEFAULT_MTS_MODE, 1);  // default mode: Stereo
#endif
        vWriteDspWORD (u1DspId, ADDR_MTS_SAP_HP_NOISE_LOW,uReadDspShmBYTE (u1DspId, B_SAP_HP_MUTE_LOW));
        vWriteDspWORD (u1DspId, ADDR_MTS_SAP_HP_NOISE_HIGH,uReadDspShmBYTE (u1DspId, B_SAP_HP_MUTE_HIGH));
        vWriteDspWORD (u1DspId, ADDR_MTS_SAP_FILTER_SEL,uReadDspShmBYTE (u1DspId, B_SAP_FILTER_SEL));
        vWriteDspWORD (u1DspId, ADDR_MTS_SAP_RATIO,uReadDspShmBYTE (u1DspId, B_SAP_RATIO));
        vWriteDspWORD (u1DspId, ADDR_MTS_SAP_MUTE_CTRL,uReadDspShmBYTE (u1DspId, B_MTS_SAP_MUTE_CTRL));
        vWriteDspWORD (u1DspId, ADDR_MTS_HDEV_LVL_REDUCE,uReadDspShmBYTE (u1DspId, B_MTS_HDEV_LVL_REDUCE));        
        break;
    case FMFM_STREAM:
        vWriteDspWORD (u1DspId, ADDR_DEM_ADC_GAIN,u2ReadDspShmWORD (u1DspId, W_ADC_GAIN));
        vWriteDspWORD (u1DspId, ADDR_DEM_USER_MODE,u2ReadDspShmWORD (u1DspId, W_USER_MODE));
        vWriteDspWORD (u1DspId, ADDR_DEM_SOURCE_MODE,u2ReadDspShmWORD (u1DspId, W_SOURCE_MODE));
        vWriteDspWORD (u1DspId, ADDR_FMFM_NUM_CHECK,u2ReadDspShmWORD (u1DspId, W_FMFM_NUM_CHECK));
        vWriteDspWORD (u1DspId, ADDR_FMFM_NUM_DOUBLE_CHECK,u2ReadDspShmWORD (u1DspId, W_FMFM_NUM_DOUBLE_CHECK));
        vWriteDspWORD (u1DspId, ADDR_FMFM_MONO_WEIGHT,u2ReadDspShmWORD (u1DspId, W_FMFM_MONO_WEIGHT));
        vWriteDspWORD (u1DspId, ADDR_FMFM_STEREO_WEIGHT,u2ReadDspShmWORD (u1DspId, W_FMFM_STEREO_WEIGHT));
        vWriteDspWORD (u1DspId, ADDR_FMFM_DUAL_WEIGHT,u2ReadDspShmWORD (u1DspId, W_FMFM_DUAL_WEIGHT));
        vWriteDspWORD (u1DspId, ADDR_FMFM_DETECTION_CONFID, u2ReadDspShmWORD (u1DspId, W_FMFM_DETECT_CONFID));
#ifndef CC_AUD_WAFFLE_OVM_SUPPORT
        vWriteDspWORD (u1DspId, ADDR_HDEV_MODE,  (uReadDspShmBYTE (u1DspId, B_HDEV_MODE) & 0x08)>>3);
#else
        vWriteDspWORD (u1DspId, ADDR_HDEV_MODE, (uReadDspShmBYTE (u1DspId, B_HDEV_MODE) & NEW_FMFM_MASK)>>NEW_FMFM_SHIFT);
#endif
        vWriteDspWORD (u1DspId, ADDR_CARRIER_SHIFT_MODE, (uReadDspShmBYTE (u1DspId, B_CARRIER_SHIFT_MODE) & 0x08)>>3);
        vWriteDspWORD (u1DspId, ADDR_FM_SATU_MUTE_MODE,  (uReadDspShmBYTE (u1DspId, B_FM_SATU_MUTE_MODE)& 0x08)>>3);
        vWriteDspWORD (u1DspId, ADDR_FM_CARRIER_MUTE_MODE,  (uReadDspShmBYTE (u1DspId, B_FM_MUTE_MODE) & 0x08)>>3);
        vWriteDspWORD (u1DspId, ADDR_FM_CARRIER_MUTE_THRESHOLD_HIGHT,  uReadDspShmBYTE (u1DspId, B_FM_MUTE_THRESHOLD_HIGHT_EIAJ));
        vWriteDspWORD (u1DspId, ADDR_FM_CARRIER_MUTE_THRESHOLD_LOW,  uReadDspShmBYTE (u1DspId, B_FM_MUTE_THRESHOLD_LOW_EIAJ));
       //vWriteDspWORD (ADDR_DEM_FMFM_MONO_FINE_TUNE_VOLUME,u4ReadDspShmDWRD  (u1DspId, D_FMFM_MONO_FINE_TUNE_VOLUME)>>8);
       // vWriteDspWORD (ADDR_DEM_FMFM_DUAL_FINE_TUNE_VOLUME,u4ReadDspShmDWRD (u1DspId, D_FMFM_DUAL_FINE_TUNE_VOLUME)>>8);
        break;
    case A2_STREAM:
        vWriteDspWORD (u1DspId, ADDR_DEM_ADC_GAIN,u2ReadDspShmWORD (u1DspId, W_ADC_GAIN));
        vWriteDspWORD (u1DspId, ADDR_DEM_USER_MODE, u2ReadDspShmWORD (u1DspId, W_USER_MODE));
        u1ChannelSrc = DSP_ReturnChannelSource ();
        u2ChannelSrcSet = SOURCE_A2_BG;
        if (u1ChannelSrc == SV_A2_BG)
        {
            u2ChannelSrcSet = SOURCE_A2_BG;
        }
        else if (u1ChannelSrc == SV_A2_DK1)
        {
            u2ChannelSrcSet = SOURCE_A2_DK1;
        }
        else if (u1ChannelSrc == SV_A2_DK2)
        {
            u2ChannelSrcSet = SOURCE_A2_DK2;
        }
        else if (u1ChannelSrc == SV_A2_DK3)
        {
            u2ChannelSrcSet = SOURCE_A2_DK3;
        }
#ifdef CC_AUD_DDI
        else if (u1ChannelSrc == SV_PAL_DK)
        {
            u2ChannelSrcSet = SOURCE_A2_DK3;
        }
#endif
        else if (u1ChannelSrc == SV_NTSC_M)
        {
            u2ChannelSrcSet = SOURCE_NTSC_M;
        }
        vWriteDspWORD (u1DspId, ADDR_DEM_SOURCE_MODE, u2ChannelSrcSet);
        vWriteDspWORD (u1DspId, ADDR_A2_NUM_CHECK, u2ReadDspShmWORD (u1DspId, W_A2_NUM_CHECK));
        vWriteDspWORD (u1DspId, ADDR_A2_NUM_DOUBLE_CHECK,u2ReadDspShmWORD (u1DspId, W_A2_NUM_DOUBLE_CHECK));
        vWriteDspWORD (u1DspId, ADDR_A2_MONO_WEIGHT,u2ReadDspShmWORD (u1DspId, W_A2_MONO_WEIGHT));
        vWriteDspWORD (u1DspId, ADDR_A2_STEREO_WEIGHT,u2ReadDspShmWORD (u1DspId, W_A2_STEREO_WEIGHT));
        vWriteDspWORD (u1DspId, ADDR_A2_DUAL_WEIGHT,u2ReadDspShmWORD (u1DspId, W_A2_DUAL_WEIGHT));
#ifndef CC_AUD_WAFFLE_OVM_SUPPORT
        vWriteDspWORD (u1DspId, ADDR_HDEV_MODE, (uReadDspShmBYTE (u1DspId, B_HDEV_MODE) & 0x02)>>1);
#else
        vWriteDspWORD (u1DspId, ADDR_HDEV_MODE, 0);  //A2 HDEV_MODE protection
#endif
        vWriteDspWORD (u1DspId, ADDR_CARRIER_SHIFT_MODE, (uReadDspShmBYTE (u1DspId, B_CARRIER_SHIFT_MODE) & 0x02)>>1);
        vWriteDspWORD (u1DspId, ADDR_FM_SATU_MUTE_MODE,  (uReadDspShmBYTE (u1DspId, B_FM_SATU_MUTE_MODE)& 0x02)>>1);
        vWriteDspWORD (u1DspId, ADDR_FM_CARRIER_MUTE_MODE,  (uReadDspShmBYTE (u1DspId, B_FM_MUTE_MODE) & 0x02)>>1);
        vWriteDspWORD (u1DspId, ADDR_FM_CARRIER_MUTE_THRESHOLD_HIGHT,  uReadDspShmBYTE (u1DspId, B_FM_MUTE_THRESHOLD_HIGHT_A2));
        vWriteDspWORD (u1DspId, ADDR_FM_CARRIER_MUTE_THRESHOLD_LOW,  uReadDspShmBYTE (u1DspId, B_FM_MUTE_THRESHOLD_LOW_A2));
        vWriteDspWORD (u1DspId, ADDR_DEM_PAL_FINE_TUNE_VOLUME,u4ReadDspShmDWRD  (u1DspId, D_A2_FINE_TUNE_VOLUME)>>8);
        vWriteDspWORD (u1DspId, ADDR_MANUAL_MTS_SW, uReadDspShmBYTE (u1DspId, B_MANUAL_MTS_SW));
        //vWriteDspWORD (u1DspId, ADDR_MANUAL_OUTPUT_MATRIX, uReadDspShmBYTE (u1DspId, B_MANUAL_OUTPUT_MATRIX));
        vWriteDspWORD (u1DspId, ADDR_FM_NON_EU_MODE,uReadDspShmBYTE  (u1DspId, B_NON_EU_FM_MODE));
        if (u2ChannelSrcSet == SOURCE_NTSC_M)
        {
            vWriteDspWORD (u1DspId, ADDR_A2_STEREO_MONO_THRES, u2ReadDspShmWORD(u1DspId, W_A2_STEREO_MONO_THRES_M));
            vWriteDspWORD (u1DspId, ADDR_A2_MONO_STEREO_THRES, u2ReadDspShmWORD(u1DspId, W_A2_MONO_STEREO_THRES_M));
        }
        else
        {
            vWriteDspWORD (u1DspId, ADDR_A2_STEREO_MONO_THRES, u2ReadDspShmWORD(u1DspId, W_A2_STEREO_MONO_THRES_EU));
            vWriteDspWORD (u1DspId, ADDR_A2_MONO_STEREO_THRES, u2ReadDspShmWORD(u1DspId, W_A2_MONO_STEREO_THRES_EU));
        }
	 vWriteDspWORD (u1DspId, ADDR_A2_WEAK_RF_LVL_S2M_THRES, u2ReadDspShmWORD(u1DspId, W_A2_WEAK_RF_LVL_S2M_THRES_EU));
        vWriteDspWORD (u1DspId, ADDR_A2_WEAK_RF_LVL_M2S_THRES, u2ReadDspShmWORD(u1DspId, W_A2_WEAK_RF_LVL_M2S_THRES_EU));
        //for dynamic detection control
        vWriteDspWORD(u1DspId, ADDR_DYNAMIC_DETECTIO_SWITCH,uReadDspShmBYTE(u1DspId, B_DYNAMIC_DETECTIO_SWITCH));
        break;
    case PAL_STREAM:
        vWriteDspWORD (u1DspId, ADDR_DEM_ADC_GAIN,u2ReadDspShmWORD (u1DspId, W_ADC_GAIN));
        vWriteDspWORD (u1DspId, ADDR_DEM_USER_MODE,u2ReadDspShmWORD (u1DspId, W_USER_MODE));
        vWriteDspWORD (u1DspId, ADDR_DEM_ENFORCED_MODE,u2ReadDspShmWORD (u1DspId, W_ENFORCED_MODE));
        u1ChannelSrc = DSP_ReturnChannelSource ();
#ifdef CC_AUD_DDI
		switch (u1ChannelSrc)
		  {
		  case SV_PAL_BG:
		  case SV_A2_BG :
			  u2ChannelSrcSet = SOURCE_PAL_BG;
			  break;
		  case SV_A2_DK1 :
		  case SV_A2_DK2 :
		  case SV_A2_DK3 :
		  case SV_PAL_DK :
			  u2ChannelSrcSet = SOURCE_PAL_DK;
			  break;
		  case SV_PAL_I :
			  u2ChannelSrcSet = SOURCE_PAL_I; 
			  break;
		  case SV_SECAM_L :
			  u2ChannelSrcSet = SOURCE_SECAM_L;
			  break;
		  case SV_SECAM_L_PLUM:
			  u2ChannelSrcSet = SOURCE_SECAM_L;
			  break;
		  case SV_SECAM_BG:
			  u2ChannelSrcSet = SOURCE_SECAM_BG;
			  break;
		  case SV_SECAM_DK:
			  u2ChannelSrcSet = SOURCE_SECAM_DK;
			  break;
		  default :
			  u2ChannelSrcSet = SOURCE_PAL_I;
		  }
#else
        u2ChannelSrcSet = SOURCE_PAL_I; // default value
        if ((u1ChannelSrc == SV_PAL_BG) || (u1ChannelSrc == SV_A2_BG))
        {
            u2ChannelSrcSet = SOURCE_PAL_BG;
        }
        else if ((u1ChannelSrc == SV_PAL_DK) ||
                (u1ChannelSrc == SV_A2_DK1) ||
                (u1ChannelSrc == SV_A2_DK2) ||
                (u1ChannelSrc == SV_A2_DK3))
        {
            u2ChannelSrcSet = SOURCE_PAL_DK;
        }
        else if (u1ChannelSrc == SV_SECAM_L)
        {
            u2ChannelSrcSet = SOURCE_SECAM_L;
        }
        else if (u1ChannelSrc == SV_SECAM_BG)
        {
            u2ChannelSrcSet = SOURCE_SECAM_BG;
        }
        else if (u1ChannelSrc == SV_SECAM_DK)
        {
            u2ChannelSrcSet = SOURCE_SECAM_DK;
        }
        else if (u1ChannelSrc == SV_SECAM_L_PLUM)
        {
            u2ChannelSrcSet = SOURCE_SECAM_L;
        }
#endif
        vWriteDspWORD (u1DspId, ADDR_DEM_SOURCE_MODE,u2ChannelSrcSet);
        vWriteDspWORD (u1DspId, ADDR_PAL_CORRECT_THRESHOLD,u2ReadDspShmWORD (u1DspId, W_PAL_CORRECT_THRESHOLD));
        vWriteDspWORD (u1DspId, ADDR_PAL_TOTAL_SYNC1_LOOP,u2ReadDspShmWORD (u1DspId, W_PAL_TOTAL_SYNC1_LOOP));
        vWriteDspWORD (u1DspId, ADDR_PAL_ERR_THRESHOLD,u2ReadDspShmWORD (u1DspId, W_PAL_ERR_THRESHOLD));
        vWriteDspWORD (u1DspId, ADDR_PAL_PARITY_ERR_THRESHOLD,u2ReadDspShmWORD (u1DspId, W_PAL_PARITY_ERR_THRESHOLD));
        vWriteDspWORD (u1DspId, ADDR_PAL_EVERY_NUM_FRAMES,u2ReadDspShmWORD (u1DspId, W_PAL_EVERY_NUM_FRAMES));
#ifdef ATV_HDEV_AUTO_SWITCH
        vWriteDspWORD (u1DspId, ADDR_HDEV_MODE, ((uReadDspShmBYTE (u1DspId, B_HDEV_MODE) & 0x01)| 0x03));
#else
#ifndef CC_AUD_WAFFLE_OVM_SUPPORT
#ifdef CC_AUD_PAL_NEW_HDEV_SUPPORT
        vWriteDspWORD (u1DspId, ADDR_HDEV_MODE, ((uReadDspShmBYTE (u1DspId, B_HDEV_MODE) & 0x01) << 2) | 0x1);  // set bit 0 and 2
#else
#ifdef CC_AUD_DDI
		vWriteDspWORD (u1DspId, ADDR_HDEV_MODE, ((uReadDspShmBYTE (u1DspId, B_HDEV_MODE) & 0x01) << 2) );  // set bit 2
#else
        vWriteDspWORD (u1DspId, ADDR_HDEV_MODE, (uReadDspShmBYTE (u1DspId, B_HDEV_MODE) & 0x01));
#endif
#endif
#else
        switch ((uReadDspShmBYTE (u1DspId, B_HDEV_MODE) & NEW_PAL_MASK)>>NEW_PAL_SHIFT)
        {
        case 0:
            vWriteDspWORD (u1DspId, ADDR_HDEV_MODE, 0x0);
            break;
        case 1:
            vWriteDspWORD (u1DspId, ADDR_HDEV_MODE, 0x1);
            break;
        case 2:
            vWriteDspWORD (u1DspId, ADDR_HDEV_MODE, 0x4);
            break;
        default:
            break;
        }
#endif
#endif
        vWriteDspWORD (u1DspId, ADDR_CARRIER_SHIFT_MODE, (uReadDspShmBYTE (u1DspId, B_CARRIER_SHIFT_MODE) & 0x01));
        vWriteDspWORD (u1DspId, ADDR_FM_SATU_MUTE_MODE,  (uReadDspShmBYTE (u1DspId, B_FM_SATU_MUTE_MODE)& 0x01));
        //for AM carrier sense mute
        vWriteDspWORD (u1DspId, ADDR_SECAM_L_MUTE_MODE,  uReadDspShmBYTE (u1DspId, B_AM_MUTE_MODE));
        vWriteDspWORD (u1DspId, ADDR_SECAM_L_MUTE_THRESHOLD_HIGHT, uReadDspShmBYTE (u1DspId, B_AM_MUTE_THRESHOLD_HIGHT));
        vWriteDspWORD (u1DspId, ADDR_SECAM_L_MUTE_THRESHOLD_LOW,uReadDspShmBYTE (u1DspId, B_AM_MUTE_THRESHOLD_LOW));
        //for FM carrier sense mute
        vWriteDspWORD (u1DspId, ADDR_FM_CARRIER_MUTE_MODE,  (uReadDspShmBYTE (u1DspId, B_FM_MUTE_MODE) & 0x01));
#ifndef CC_AUD_WAFFLE_OVM_SUPPORT
        vWriteDspWORD (u1DspId, ADDR_FM_CARRIER_MUTE_THRESHOLD_HIGHT, uReadDspShmBYTE (u1DspId, B_FM_MUTE_THRESHOLD_HIGHT_PAL));
        vWriteDspWORD (u1DspId, ADDR_FM_CARRIER_MUTE_THRESHOLD_LOW,uReadDspShmBYTE (u1DspId, B_FM_MUTE_THRESHOLD_LOW_PAL));
#else
        if (((uReadDspShmBYTE (u1DspId, B_HDEV_MODE) & NEW_PAL_MASK)>>NEW_PAL_SHIFT)==2)
        {
            // for OVM=HIGH threshold
            vWriteDspWORD(u1DspId, ADDR_FM_CARRIER_MUTE_THRESHOLD_HIGHT, uReadDspShmBYTE(u1DspId, B_FM_MUTE_THRESHOLD_HIGHT));
            vWriteDspWORD(u1DspId, ADDR_FM_CARRIER_MUTE_THRESHOLD_LOW,uReadDspShmBYTE(u1DspId, B_FM_MUTE_THRESHOLD_LOW));
        }
        else
        {
            vWriteDspWORD (u1DspId, ADDR_FM_CARRIER_MUTE_THRESHOLD_HIGHT, uReadDspShmBYTE (u1DspId, B_FM_MUTE_THRESHOLD_HIGHT_PAL));
            vWriteDspWORD (u1DspId, ADDR_FM_CARRIER_MUTE_THRESHOLD_LOW,uReadDspShmBYTE (u1DspId, B_FM_MUTE_THRESHOLD_LOW_PAL));
        }
#endif
        //for EU/NON-EU FM
        vWriteDspWORD (u1DspId, ADDR_FM_NON_EU_MODE,uReadDspShmBYTE  (u1DspId, B_NON_EU_FM_MODE));
        //for NICAM I boost or not
        vWriteDspWORD(u1DspId, ADDR_NICAM_I_BOOST_FLAG,uReadDspShmBYTE(u1DspId, B_NICAM_I_BOOST_FLAG));
         //for PAL FM BAD SIGNAL CHECK
	 //vWriteDspWORD (ADDR_PAL_HDEV_BAD_SIGNAL_CHECK,uReadDspShmBYTE (B_FM_BAD_SIGNAL));
        //for dynamic detection control
        vWriteDspWORD(u1DspId, ADDR_DYNAMIC_DETECTIO_SWITCH,uReadDspShmBYTE(u1DspId, B_DYNAMIC_DETECTIO_SWITCH));
        vWriteDspWORD (u1DspId, ADDR_DEM_PAL_FINE_TUNE_VOLUME,u4ReadDspShmDWRD  (u1DspId, D_PAL_FINE_TUNE_VOLUME)>>8);
        vWriteDspWORD (u1DspId, ADDR_DEM_NICAM_FINE_TUNE_VOLUME,u4ReadDspShmDWRD (u1DspId, D_NICAM_FINE_TUNE_VOLUME)>>8);
        vWriteDspWORD (u1DspId, ADDR_DEM_AM_FINE_TUNE_VOLUME,u4ReadDspShmDWRD  (u1DspId, D_AM_FINE_TUNE_VOLUME)>>8);
        break;
    case DETECTOR_STREAM:
        vWriteDspWORD (u1DspId, ADDR_DETECTOR_DEFAULT_MODE, uReadDspShmBYTE(u1DspId, B_DETECT_DEFAULT));
        vWriteDspWORD (u1DspId, ADDR_DETECTOR_START_FLAG, 1); // default turn on detection
        WriteDspCommDram (u1DspId, ADDR_DETECTOR_FM_NOISE_TH, u4ReadDspShmDWRD (u1DspId, D_DETECTOR_FM_NOISE_TH));
        vWriteDspWORD (u1DspId, ADDR_DETECTOR_A2_PILOT_CHK, uReadDspShmBYTE(u1DspId, B_DETECTOR_A2_PILOT_CHK));
        break;
    case FMRDODET_STREAM:
        vWriteDspWORD (u1DspId, ADDR_DETECTOR_DEFAULT_MODE, uReadDspShmBYTE(u1DspId, B_DETECT_DEFAULT));
        vWriteDspWORD (u1DspId, ADDR_DETECTOR_START_FLAG, 1); // default turn on detection
        WriteDspCommDram (u1DspId, ADDR_FM_RDO_DET_INBAND_TH, u4ReadDspShmDWRD (u1DspId, D_FM_RDO_INBAND_TH));
        WriteDspCommDram (u1DspId, ADDR_FM_RDO_DET_NOISE_TH, u4ReadDspShmDWRD (u1DspId, D_FM_RDO_NOISE_TH));
        break;
    case FMRDO_STREAM:
        vWriteDspWORD (u1DspId, ADDR_MTS_NUM_CHECK, 0);   //from shawn
        vWriteDspWORD (u1DspId, ADDR_DEM_ENFORCED_MODE, 0x1); // enable enforced
        vWriteDspWORD (u1DspId, ADDR_HDEV_MODE, (uReadDspShmBYTE (u1DspId, B_HDEV_MODE) & 0x00));
        vWriteDspWORD (u1DspId, ADDR_CARRIER_SHIFT_MODE, 0x0); // disable CFO
        vWriteDspWORD (u1DspId, ADDR_DEM_FMRADIO_FINE_TUNE_VOLUME,u4ReadDspShmDWRD  (u1DspId, D_FMRADIO_FINE_TUNE_VOLUME)>>8);
        vWriteDspWORD (u1DspId, ADDR_DEM_USER_MODE,u2ReadDspShmWORD (u1DspId, W_USER_MODE));
        break;
#endif
    case G711DEC_STREAM:
        vWriteDspWORD (u1DspId, ADDR_RC2D_PCM_INPUT_TYPE,
                       u2ReadDspShmWORD (u1DspId, W_PCM_INPUT_TYPE));  //0:pcm24 1:IMA 3:MS 2:pcm16 //msb=1: little endian
        vWriteDspWORD (u1DspId, ADDR_RC2D_PCM_INPUT_SAMPLING_RATE,
                       u2ReadDspShmWORD (u1DspId, W_PCM_INPUT_SAMPLING_RATE));

        break;
    case G711ENC_STREAM:
        vWriteDspWORD (u1DspId, ADDR_RC2D_PCM_INPUT_TYPE,
                       u2ReadDspShmWORD (u1DspId, W_PCM_INPUT_TYPE));  //0:pcm24 1:IMA 3:MS 2:pcm16 //msb=1: little endian

        break;
    case LPCM_STREAM:
        #if 0
        // This is OK for lpcm 16 bit (from VOB file)
        WriteDspCommDram(ADDR_RC2D_LPCM_CHANNEL_ASSIGNMENT, 0x100);
        WriteDspCommDram(ADDR_RC2D_LPCM_FREQUENCY, 0x000000);
        WriteDspCommDram(ADDR_RC2D_LPCM_BIT_SHIFT, 0);
        WriteDspCommDram(ADDR_RC2D_LPCM_DRC_VALUE, 0x08000);
        WriteDspCommDram(ADDR_RC2D_LPCM_Q, 0x3000);
        WriteDspCommDram(ADDR_RC2D_LPCM_DRC_FLAG, 0x00);
        WriteDspCommDram(ADDR_RC2D_LPCM_DECODING_TYPE, 0xd00);
        /*
        WriteDspCommDram(ADDR_RC2D_LPCM_ADPCM_DEC_TYPE, 0x1100);
        WriteDspCommDram(ADDR_RC2D_LPCM_ADPCM_BITS_PER_SAMPLE, 0x400);
        WriteDspCommDram(ADDR_RC2D_ADPCM_BLOCK_ALIGN, 0x080000);
        WriteDspCommDram(ADDR_RC2D_ADPCM_CHANNEL_NUM, 0x200);
        */
        #else
        u1Temp1 = (u2ReadDspShmWORD(u1DspId, W_LPCM_FREQUENCY)>>8) & 0xFF;
        u1Temp2 = u2ReadDspShmWORD(u1DspId, W_LPCM_FREQUENCY) & 0xFF;

        vWriteDspWORD(u1DspId, ADDR_RC2D_LPCM_CHANNEL_ASSIGNMENT, uReadDspShmBYTE(u1DspId, B_LPCM_CH_ASSIGNMENT));
        //WriteDspCommDram(ADDR_RC2D_LPCM_FREQUENCY,  u2ReadDspShmWORD(W_LPCM_FREQUENCY));
        WriteDspCommDram(u1DspId, ADDR_RC2D_LPCM_FREQUENCY,  (u1Temp1<<12)+u1Temp2);
        vWriteDspWORD(u1DspId, ADDR_RC2D_LPCM_BIT_SHIFT,  uReadDspShmBYTE(u1DspId, B_LPCM_BIT_SHIFT));
        vWriteDspWORD(u1DspId, ADDR_RC2D_LPCM_DRC_VALUE,  uReadDspShmBYTE(u1DspId, B_LPCM_DRC_VALUE));
        WriteDspCommDram(u1DspId, ADDR_RC2D_LPCM_Q,  u2ReadDspShmWORD(u1DspId, W_LPCM_Q_VALUE));
        vWriteDspWORD(u1DspId, ADDR_RC2D_LPCM_DRC_FLAG,  uReadDspShmBYTE(u1DspId, B_LPCM_DRC_FLAG));
        vWriteDspWORD(u1DspId, ADDR_RC2D_LPCM_DECODING_TYPE,  u2ReadDspShmWORD(u1DspId, W_LPCM_DECODING_TYPE));

        if (uReadDspShmBYTE(u1DspId, W_ADPCM_DECODING_TYPE)!=0) //ADPCM
        {
            vWriteDspWORD(u1DspId, ADDR_RC2D_LPCM_ADPCM_DEC_TYPE, u2ReadDspShmWORD(u1DspId, W_ADPCM_DECODING_TYPE));
            vWriteDspWORD(u1DspId, ADDR_RC2D_LPCM_ADPCM_BITS_PER_SAMPLE, uReadDspShmBYTE(u1DspId, B_ADPCM_BIT_PERSAMPLE));
            vWriteDspWORD(u1DspId, ADDR_RC2D_ADPCM_BLOCK_ALIGN, u2ReadDspShmWORD(u1DspId, W_ADPCM_BLOCK_ALIGN));
            vWriteDspWORD(u1DspId, ADDR_RC2D_ADPCM_CHANNEL_NUM, uReadDspShmBYTE(u1DspId, B_ADPCM_CHANNEL_NUM));
        }
        #endif
        break;
    case DTSDEC_STREAM:
        if (AUD_GetDTSInfo())
        {
            //Big Endian
            vWriteDspWORD (u1DspId, ADDR_RC2D_DTS_DRC_FLAG, 0xffd9);
        }
        else
        {
            //Little Endian
            vWriteDspWORD (u1DspId, ADDR_RC2D_DTS_DRC_FLAG, 0xfffb);
        }
        vWriteDspWORD (u1DspId, ADDR_RC2D_DTS_LFE_mixto_FRONT, uReadDspShmBYTE(u1DspId, B_DTS_BC_LFE_MIXTO_FRONT));
        vWriteDspWORD (u1DspId, ADDR_RC2D_DTS_DRC_ONOFF, uReadDspShmBYTE(u1DspId, B_DTS_BC_DRC));
        vWriteDspWORD (u1DspId, ADDR_RC2D_DTS_DIALNORM_ONOFF, uReadDspShmBYTE(u1DspId, B_DTS_BC_Dialnrom));
        WriteDspCommDram (u1DspId, ADDR_RC2D_DTS_DRC_LOW, u4ReadDspShmDWRD (u1DspId, D_DTS_BC_DRC_LOW));
        WriteDspCommDram (u1DspId, ADDR_RC2D_DTS_DRC_MEDIUM, u4ReadDspShmDWRD (u1DspId, D_DTS_BC_DRC_MEDIUM));
        WriteDspCommDram (u1DspId, ADDR_RC2D_DTS_DRC_HIGH, u4ReadDspShmDWRD (u1DspId, D_DTS_BC_DRC_HIGH));
        WriteDspCommDram (u1DspId, ADDR_RC2D_DTS_DIALNORM_LOW, u4ReadDspShmDWRD (u1DspId, D_DTS_BC_Dialnorm_LOW));
        WriteDspCommDram (u1DspId, ADDR_RC2D_DTS_DIALNORM_MEDIUM, u4ReadDspShmDWRD (u1DspId, D_DTS_BC_Dialnorm_MEDIUM));
        WriteDspCommDram (u1DspId, ADDR_RC2D_DTS_DIALNORM_HIGH, u4ReadDspShmDWRD (u1DspId, D_DTS_BC_Dialnorm_HIGH));
        break;

    case AAC_STREAM:
//#ifndef CC_AUD_SUPPORT_MS10
#ifndef CC_AUD_DOLBY_SUPPORT_DDT
        vWriteDspWORD (u1DspId, ADDR_RC2D_AAC_DOWNMIX_LEVEL,
                       AAC_DEFAULT_DOWNMIX_LEVEL);
        vWriteDspWORD (u1DspId, ADDR_RC2D_AAC_TARGET_LEVEL,
                       AAC_TARGET_LEVEL);
#else
        vWriteDspWORD (u1DspId, ADDR_RC2D_DDT_DUAL_MODE,
                       uReadDspShmBYTE (u1DspId, B_DDT_DUALMODE));
        vWriteDspWORD (u1DspId, ADDR_RC2D_DDT_DRC,
                       uReadDspShmBYTE (u1DspId, B_DDT_COMPVAL));
        vWriteDspWORD (u1DspId, ADDR_RC2D_DDT_DMX_MODE,
                       uReadDspShmBYTE (u1DspId, B_DDT_COMPMODE));
#endif
        vWriteDspWORD (u1DspId, ADDR_RC2D_DDT_CONTROL,
                       uReadDspShmBYTE(u1DspId, B_DDT_CONTROL));

        //PTS Table Shift Amount
#if defined(CC_AUD_ARM_SUPPORT) && defined(CC_AUD_ARM_RENDER)
        if (uReadDspShmBYTE(u1DspId, B_DDT_CONTROL)==0) //EU Mode
        {
            u4Temp3 = dReadDspCommDram(u1DspId, ADDR_RC2D_PTS_STC_CTRL_FLAG);
            u4Temp3 |= 0x4000; //Set bit 6 (align @ bit8)
            WriteDspCommDram (u1DspId, ADDR_RC2D_PTS_STC_CTRL_FLAG, u4Temp3);  
        }
#endif  
        break;

    #if 1///def CC_VORBIS_SUPPORT
    case VORBIS_STREAM:
        vWriteDspWORD(u1DspId, ADDR_RC2D_VORBIS_PACKET_NO, u2ReadDspShmWORD(u1DspId, W_VORBISPACKET));
        vWriteDspWORD(u1DspId, ADDR_D2RC_VORBIS_TABLE_OK, 0);
        vWriteDspWORD(u1DspId, ADDR_D2RC_VORBIS_ABS_GRANULE_POS_LL16, 0);
        vWriteDspWORD(u1DspId, ADDR_D2RC_VORBIS_ABS_GRANULE_POS_LH16, 0);
        vWriteDspWORD(u1DspId, ADDR_D2RC_VORBIS_ABS_GRANULE_POS_OK, 0);
        /// Decode Dram is big enough
        /// DSP_DAT_DEC1_SZ (0x12A00*4+0x18000*4+0x76000) = 0x48200
        /// Vorbis
        /// OUTPUT_BUF_ADR (016B00h-08000h)/100h = 0xEB00
        /// Aout Buffer = 0x48200 - 0xEB00 = 0x39700
        /// Dec1 Each Banck Compact Size = 256*3/4*10 = 0x780
        /// Aout Maximum Buffer Bank Size = 0x39700/0x780 = 0x7A(122) > (8+30) and (18+30+48)
        /// it is big enough, no need to limit it when play vorbis.
        /// Dec1: 8+30
        ///vWriteDspWORD (ADDR_RC2D_AOUT_BANK_256_NUM, (8+30));   ///[Joel] Vorbis decoder dram only support 8+30 Bank
        break;
    #endif
     case FLAC_STREAM://paul_flac
		WriteDspCommDram(u1DspId, ADDR_RC2D_FLAC_MIN_BLK_SZ, u2ReadDspShmWORD(u1DspId, W_FLAC_MIN_BLK_SZ));
		WriteDspCommDram(u1DspId, ADDR_RC2D_FLAC_MAX_BLK_SZ, u2ReadDspShmWORD(u1DspId, W_FLAC_MAX_BLK_SZ));
		WriteDspCommDram(u1DspId, ADDR_RC2D_FLAC_CH_NUM_1, u2ReadDspShmWORD(u1DspId, W_FLAC_CHN_NUM));
		WriteDspCommDram(u1DspId, ADDR_RC2D_FLAC_BPS_1, u2ReadDspShmWORD(u1DspId, W_FLAC_BPS));
		WriteDspCommDram(u1DspId, ADDR_RC2D_FLAC_MIN_FRM_SZ, u4ReadDspShmDWRD(u1DspId, D_FLAC_MIN_FRM_SZ));
		WriteDspCommDram(u1DspId, ADDR_RC2D_FLAC_MAX_FRM_SZ, u4ReadDspShmDWRD(u1DspId, D_FLAC_MAX_FRM_SZ));
		WriteDspCommDram(u1DspId, ADDR_RC2D_FLAC_SAMP_RATE, u4ReadDspShmDWRD(u1DspId, D_FLAC_SMP_RATE));
        break;
     case G726_STREAM:
		WriteDspCommDram(u1DspId, ADDR_RC2D_G726_FRAME_SZ, u2ReadDspShmWORD(u1DspId, W_G726_FRAME)<<8);
		WriteDspCommDram(u1DspId, ADDR_RC2D_G726_CH_NUM, uReadDspShmBYTE(u1DspId, B_G726_CH)<<8);
		WriteDspCommDram(u1DspId, ADDR_RC2D_G726_RATE, uReadDspShmBYTE(u1DspId, B_G726_RATE)<<8);
		WriteDspCommDram(u1DspId, ADDR_RC2D_G726_LAW, uReadDspShmBYTE(u1DspId, B_G726_LAW)<<8);
    WriteDspCommDram(u1DspId, ADDR_RC2D_G726_SRATE, uReadDspShmBYTE(u1DspId, B_G726_SRATE)<<8);
        break;
    default:
        break;
    }
}

/***************************************************************************
     Function : vDecCommonInitDec2 (for second decoder)
  Description : This function is used to init the decoder EACH time when it
                is played. In other words, it is to set the properties which
                is different song by song.
    Parameter : None
    Return    : None
***************************************************************************/
void vDecCommonInitDec2 (UINT8 u1DspId)
{
    UINT32 u4DspData;
    TV_AUD_SYS_T u1ChannelSrc;
    UINT16 u2ChannelSrcSet;
    UINT8 u1Temp1, u1Temp2;
    UINT32 u4Temp3;
    #ifdef CC_AUD_USE_NVM
    AUD_NVM_ATV_FMT_T eAudNvmAtvFmt = AUD_NVM_NON_DK;
    #endif
    
    UNUSED(u1Temp1);
    UNUSED(u1Temp2);

    //CTRL5 NUM_1
    // ... should be in switch below
    vWriteDspWORD (u1DspId, ADDR_RC2D_CTRL_5_NUM_1, (UINT16)CTRL_5_NUM_1);
    // Bonding options
    vWriteDspWORD (u1DspId, ADDR_RC2D_BONDING_OPTIONS, u2ReadDspShmWORD (u1DspId, W_BONDING_OPTIONS));
    //Pink Noise Setting
    vWriteDspWORD (u1DspId, ADDR_RC2D_CHANNEL_USE_DEC2, u2ReadDspShmWORD (u1DspId, W_PINKNOISE));

    //Soft Mute (Max. 7)
    vWriteDspWORD (u1DspId, ADDR_RC2D_VOLUME_SOFT_MUTE_ORDER_DEC2,
        u2ReadDspShmWORD (u1DspId, B_SOFTMUTEORDER_DEC2));

    //PTS Table Shift Amount
    u4Temp3 = dReadDspCommDram(u1DspId, ADDR_RC2D_PTS_STC_CTRL_FLAG);
    u4Temp3 &= 0xFF7F00; //Clear bit 7 (align @ bit8)
    WriteDspCommDram (u1DspId, ADDR_RC2D_PTS_STC_CTRL_FLAG, u4Temp3);      
    

    DSP_FlushInvalidateDCache(u1DspId, CACHE_FLUSH_TYPE_DATA, TYPE_COMM_DRAM_IDX);

    switch (_rDspVars[u1DspId][AUD_DEC_AUX].bDspStrTyp)
    {
    case PCM_STREAM:
        u4DspData = wReadDspWORD (u1DspId, ADDR_D2RC_INPUT_CH_CFG_DEC2);
        u4DspData = u4DspData & ~0xf;
        u4DspData = u4DspData | 0x2;  // Hard set cdda channel config
        vWriteDspWORD (u1DspId, ADDR_D2RC_INPUT_CH_CFG_DEC2, u4DspData);
        vWriteDspWORD (u1DspId, ADDR_RC2D_PCM_INPUT_CHANNEL_NUM_DEC2,
                       u2ReadDspShmWORD (u1DspId, W_PCM_INPUT_CHANNEL_NUM_DEC2));
        vWriteDspWORD (u1DspId, ADDR_RC2D_PCM_INPUT_CH_ASSIGNMENT_DEC2,
                       u2ReadDspShmWORD (u1DspId, W_PCM_INPUT_CH_ASSIGNMENT_DEC2));
        vWriteDspWORD (u1DspId, ADDR_RC2D_PCM_INPUT_TYPE_DEC2,
                       u2ReadDspShmWORD (u1DspId, W_PCM_INPUT_TYPE_DEC2));  //0:pcm24 1:IMA 3:MS 2:pcm16 //msb=1: little endian
        vWriteDspWORD (u1DspId, ADDR_RC2D_PCM_INPUT_SAMPLING_RATE_DEC2,
                       u2ReadDspShmWORD (u1DspId, W_PCM_INPUT_SAMPLING_RATE_DEC2));
#if 0
        vWriteDspWORD (ADDR_RC2D_ADPCM_BLOCK_ALIGN_DEC2,
                       u2ReadDspShmWORD (W_ADPCM_BLOCK_ALIGN));
        vWriteDspWORD (ADDR_RC2D_ADPCM_CHANNEL_NUM_DEC2,
                       u2ReadDspShmWORD (B_ADPCM_CHANNEL_NUM));
#endif
        break;
    case AC3_STREAM:
        //AC3 Karaoke Mode
        vWriteDspWORD (u1DspId, ADDR_RC2D_AC3_KARAOKE_MODE_DEC2,
                       u2ReadDspShmWORD (u1DspId, W_AC3KARAMOD_DEC2));
        //AC3 Dual Mono Mode
        vWriteDspWORD (u1DspId, ADDR_RC2D_AC3_DUAL_MODE_DEC2,
                       u2ReadDspShmWORD (u1DspId, W_AC3DUALMODE_DEC2));
        //AC3 Compression Mode
        vWriteDspWORD (u1DspId, ADDR_RC2D_AC3_COMPRESS_MODE_DEC2,
                       u2ReadDspShmWORD (u1DspId, W_AC3COMPMOD_DEC2));
        //AC3 Low Boost
        WriteDspCommDram (u1DspId, ADDR_RC2D_AC3_DYNAMIC_LOW_DEC2,
                          u4ReadDspShmDWRD (u1DspId, D_AC3DYN_LOW_DEC2));
        //AC3 High Cut
        WriteDspCommDram (u1DspId, ADDR_RC2D_AC3_DYNAMIC_HIGH_DEC2,
                          u4ReadDspShmDWRD (u1DspId, D_AC3DYN_HIGH_DEC2));
        // AC3 Auto downmix
        WriteDspCommDram (u1DspId, ADDR_RC2D_AC3_DOWNMIX_DEC2,
                          uReadDspShmBYTE (u1DspId, B_AC3AUTODNMIX_DEC2));
        // DDC control
        vWriteDspWORD (u1DspId, ADDR_RC2D_AC3_CONTROL_DEC2, u2ReadDspShmWORD(u1DspId, W_DDC_CONTROL_DEC2));
#ifdef CC_AUD_SUPPORT_MS10
        // DDC associated stream id
        vWriteDspWORD (u1DspId, ADDR_RC2D_DDC_STRMID_DEC2,  uReadDspShmBYTE (u1DspId, B_DDC_STRMID_DEC2));
        // DDC mixer
        vWriteDspWORD (u1DspId, ADDR_RC2D_DDC_ASSOC_MIX_DEC2, uReadDspShmBYTE (u1DspId, B_DDC_ASSOC_MIX_DEC2));
#endif //CC_AUD_SUPPORT_MS10
#ifdef CC_AUD_SUPPORT_MS10
	// XMX mixer in PCM decoder
        vWriteDspWORD (u1DspId, ADDR_RC2D_XMX_FLAG, 0x1);
#endif
        break;
    case MPEG12_STREAM:
        /* 20041220: For testing, hard coded here, remove in the Future */
        // MPEG sync no error checking
        vWriteDspWORD (u1DspId, ADDR_RC2D_MPEG_SYNC_MODE_DEC2, 0x1);
        //MPEG Error Detection
        vWriteDspWORD (u1DspId, ADDR_RC2D_CRC_FLAG,
                       u2ReadDspShmWORD (u1DspId, W_MPEGERRDET));
        break;
    case MPEG3_STREAM:
        // Normal play
        vWriteDspWORD (u1DspId, ADDR_RC2D_MP3_NORMAL_PLAY_DEC2, 0x00);
        // MP3 sync mode
        vWriteDspWORD (u1DspId, ADDR_RC2D_MP3_SYNC_MODE_DEC2, 0x01);
        // MP3 deemphasis mode
        vWriteDspWORD (u1DspId, ADDR_RC2D_MP3_DEEMPHASIS_MODE_DEC2, 0x00);
        // MP3 preparsing mode
        vWriteDspWORD (u1DspId, ADDR_RC2D_MP3_PREPARSING_MODE_DEC2, 0x00);
        break;
#ifdef DSP_SUPPORT_NPTV
    case NTSC_STREAM:
        vWriteDspWORD (u1DspId, ADDR_DEM_ADC_GAIN,u2ReadDspShmWORD (u1DspId, W_ADC_GAIN));
        vWriteDspWORD (u1DspId, ADDR_DEM_USER_MODE,u2ReadDspShmWORD (u1DspId, W_USER_MODE));
        vWriteDspWORD (u1DspId, ADDR_DEM_SOURCE_MODE,u2ReadDspShmWORD (u1DspId, W_SOURCE_MODE));
        vWriteDspWORD (u1DspId, ADDR_MTS_NUM_CHECK, u2ReadDspShmWORD (u1DspId, W_MTS_NUM_CHECK));
        vWriteDspWORD (u1DspId, ADDR_MTS_STEREO_MID,u2ReadDspShmWORD (u1DspId, W_MTS_STEREO_MID));
        vWriteDspWORD (u1DspId, ADDR_MTS_STEREO_CON_MID, u2ReadDspShmWORD (u1DspId, W_MTS_STEREO_CON_MID));
        vWriteDspWORD (u1DspId, ADDR_MTS_NUM_PILOT, u2ReadDspShmWORD (u1DspId, W_MTS_NUM_PILOT));
        vWriteDspWORD (u1DspId, ADDR_MTS_NUM_SAP, u2ReadDspShmWORD (u1DspId, W_MTS_NUM_SAP));
        vWriteDspWORD (u1DspId, ADDR_MTS_SAP_MID, u2ReadDspShmWORD (u1DspId, W_MTS_SAP_MID));
        vWriteDspWORD (u1DspId, ADDR_MTS_SAP_CON_MID,u2ReadDspShmWORD (u1DspId, W_MTS_SAP_CON_MID));
        vWriteDspWORD (u1DspId, ADDR_RC2D_MTS_LR_AMP_M, 0x14b);
        vWriteDspWORD (u1DspId, ADDR_RC2D_MTS_LR_AMP_M_576K, 0x124);
        vWriteDspWORD (u1DspId, ADDR_RC2D_MTS_LR_AMP_N, 0x140);
        vWriteDspWORD (u1DspId, ADDR_RC2D_MTS_LR_AMP_N_576K, 0x15a);
        if ((uReadDspShmBYTE (u1DspId, B_HDEV_MODE) & 0x10) == 0x10)
        {
            vWriteDspWORD (u1DspId, ADDR_HDEV_MODE, 0x2);  // set bit 1 for 576k mode
        }
        else
        {
        vWriteDspWORD (u1DspId, ADDR_HDEV_MODE, (uReadDspShmBYTE (u1DspId, B_HDEV_MODE) & 0x04)>>2);
        }
        vWriteDspWORD (u1DspId, ADDR_CARRIER_SHIFT_MODE, (uReadDspShmBYTE (u1DspId, B_CARRIER_SHIFT_MODE) & 0x04)>>2);
        vWriteDspWORD (u1DspId, ADDR_FM_SATU_MUTE_MODE,  (uReadDspShmBYTE (u1DspId, B_FM_SATU_MUTE_MODE)& 0x04)>>2);
        vWriteDspWORD (u1DspId, ADDR_FM_CARRIER_MUTE_MODE, (uReadDspShmBYTE (u1DspId, B_FM_MUTE_MODE) & 0x04)>>2);
        vWriteDspWORD (u1DspId, ADDR_FM_CARRIER_MUTE_THRESHOLD_HIGHT, uReadDspShmBYTE (u1DspId, B_FM_MUTE_THRESHOLD_HIGHT_MTS));
        vWriteDspWORD (u1DspId, ADDR_FM_CARRIER_MUTE_THRESHOLD_LOW, uReadDspShmBYTE (u1DspId, B_FM_MUTE_THRESHOLD_LOW_MTS));
        vWriteDspWORD (u1DspId, ADDR_DEM_MTS_FINE_TUNE_VOLUME,u4ReadDspShmDWRD  (u1DspId, D_MTS_FINE_TUNE_VOLUME)>>8);
        vWriteDspWORD (u1DspId, ADDR_DEM_SAP_FINE_TUNE_VOLUME,u4ReadDspShmDWRD (u1DspId, D_SAP_FINE_TUNE_VOLUME)>>8);
        vWriteDspWORD (u1DspId, ADDR_MTS_PILOT_OFFSET_DETECTION,uReadDspShmBYTE (u1DspId, B_MTS_PILOT_OFFSET_DETECTION));
        vWriteDspWORD (u1DspId, ADDR_MTS_MUTE_SAP_LOW,uReadDspShmBYTE (u1DspId, B_MTS_MUTE_SAP_LOW));
        vWriteDspWORD (u1DspId, ADDR_MTS_MUTE_SAP_HIGH,uReadDspShmBYTE (u1DspId, B_MTS_MUTE_SAP_HIGH));
        vWriteDspWORD (u1DspId, ADDR_MTS_SATU_MUTE_HIGH,uReadDspShmBYTE (u1DspId, B_SATU_MUTE_THRESHOLD_HIGH));
        vWriteDspWORD (u1DspId, ADDR_MTS_SATU_MUTE_LOW,uReadDspShmBYTE (u1DspId, B_SATU_MUTE_THRESHOLD_LOW));
#ifndef CC_AUD_SX1_FEATURE
        vWriteDspWORD (u1DspId, ADDR_DEFAULT_MTS_MODE, 0);  // no default mode
#else
        vWriteDspWORD (u1DspId, ADDR_DEFAULT_MTS_MODE, 1);  // default mode: Stereo
#endif
        vWriteDspWORD (u1DspId, ADDR_MTS_SAP_HP_NOISE_LOW,uReadDspShmBYTE (u1DspId, B_SAP_HP_MUTE_LOW));
        vWriteDspWORD (u1DspId, ADDR_MTS_SAP_HP_NOISE_HIGH,uReadDspShmBYTE (u1DspId, B_SAP_HP_MUTE_HIGH));
        vWriteDspWORD (u1DspId, ADDR_MTS_SAP_FILTER_SEL,uReadDspShmBYTE (u1DspId, B_SAP_FILTER_SEL));
        vWriteDspWORD (u1DspId, ADDR_MTS_SAP_RATIO,uReadDspShmBYTE (u1DspId, B_SAP_RATIO));
        vWriteDspWORD (u1DspId, ADDR_MTS_SAP_MUTE_CTRL,uReadDspShmBYTE (u1DspId, B_MTS_SAP_MUTE_CTRL));
        vWriteDspWORD (u1DspId, ADDR_MTS_HDEV_LVL_REDUCE,uReadDspShmBYTE (u1DspId, B_MTS_HDEV_LVL_REDUCE));
#ifdef CC_AUD_MTS_OUTPUT_LEVEL_GAIN_SUPPORT
        vWriteDspWORD (u1DspId, ADDR_MTS_OUTPUT_LEVEL_GAIN, u4ReadDspShmDWRD(u1DspId, D_MTS_OUTPUT_LEVEL_GAIN));
#endif

        // Decoder 2
        vWriteDspWORD (u1DspId, ADDR_DEM_SOURCE_MODE_DEC2,u2ReadDspShmWORD (u1DspId, W_SOURCE_MODE));
        vWriteDspWORD (u1DspId, ADDR_DEM_USER_MODE_DEC2,u2ReadDspShmWORD (u1DspId, W_USER_MODE));
        vWriteDspWORD (u1DspId, ADDR_DEM_ADC_GAIN_DEC2,u2ReadDspShmWORD (u1DspId, W_ADC_GAIN));
        vWriteDspWORD (u1DspId, ADDR_DEM_SAP_FINE_TUNE_VOLUME_DEC2,u4ReadDspShmDWRD (u1DspId, D_SAP_FINE_TUNE_VOLUME)>>8);
        vWriteDspWORD (u1DspId, ADDR_DEM_MTS_FINE_TUNE_VOLUME_DEC2,u4ReadDspShmDWRD  (u1DspId, D_MTS_FINE_TUNE_VOLUME)>>8);
        break;
    case FMFM_STREAM:
        vWriteDspWORD (u1DspId, ADDR_DEM_ADC_GAIN,u2ReadDspShmWORD (u1DspId, W_ADC_GAIN));
        vWriteDspWORD (u1DspId, ADDR_DEM_USER_MODE,u2ReadDspShmWORD (u1DspId, W_USER_MODE));
        vWriteDspWORD (u1DspId, ADDR_DEM_SOURCE_MODE,u2ReadDspShmWORD (u1DspId, W_SOURCE_MODE));
        vWriteDspWORD (u1DspId, ADDR_FMFM_NUM_CHECK,u2ReadDspShmWORD (u1DspId, W_FMFM_NUM_CHECK));
        vWriteDspWORD (u1DspId, ADDR_FMFM_NUM_DOUBLE_CHECK,u2ReadDspShmWORD (u1DspId, W_FMFM_NUM_DOUBLE_CHECK));
        vWriteDspWORD (u1DspId, ADDR_FMFM_MONO_WEIGHT,u2ReadDspShmWORD (u1DspId, W_FMFM_MONO_WEIGHT));
        vWriteDspWORD (u1DspId, ADDR_FMFM_STEREO_WEIGHT,u2ReadDspShmWORD (u1DspId, W_FMFM_STEREO_WEIGHT));
        vWriteDspWORD (u1DspId, ADDR_FMFM_DUAL_WEIGHT,u2ReadDspShmWORD (u1DspId, W_FMFM_DUAL_WEIGHT));
        vWriteDspWORD (u1DspId, ADDR_FMFM_DETECTION_CONFID, u2ReadDspShmWORD (u1DspId, W_FMFM_DETECT_CONFID));
#ifndef CC_AUD_WAFFLE_OVM_SUPPORT
        vWriteDspWORD (u1DspId, ADDR_HDEV_MODE,  (uReadDspShmBYTE (u1DspId, B_HDEV_MODE) & 0x08)>>3);
#else
        vWriteDspWORD (u1DspId, ADDR_HDEV_MODE, (uReadDspShmBYTE (u1DspId, B_HDEV_MODE) & NEW_FMFM_MASK)>>NEW_FMFM_SHIFT);
#endif
        vWriteDspWORD (u1DspId, ADDR_CARRIER_SHIFT_MODE, (uReadDspShmBYTE (u1DspId, B_CARRIER_SHIFT_MODE) & 0x08)>>3);
        vWriteDspWORD (u1DspId, ADDR_FM_SATU_MUTE_MODE,  (uReadDspShmBYTE (u1DspId, B_FM_SATU_MUTE_MODE)& 0x08)>>3);
        vWriteDspWORD (u1DspId, ADDR_FM_CARRIER_MUTE_MODE,  (uReadDspShmBYTE (u1DspId, B_FM_MUTE_MODE) & 0x08)>>3);
        vWriteDspWORD (u1DspId, ADDR_FM_CARRIER_MUTE_THRESHOLD_HIGHT,  uReadDspShmBYTE (u1DspId, B_FM_MUTE_THRESHOLD_HIGHT_EIAJ));
        vWriteDspWORD (u1DspId, ADDR_FM_CARRIER_MUTE_THRESHOLD_LOW,  uReadDspShmBYTE (u1DspId, B_FM_MUTE_THRESHOLD_LOW_EIAJ));
        //vWriteDspWORD (ADDR_DEM_FMFM_MONO_FINE_TUNE_VOLUME,u4ReadDspShmDWRD  (D_FMFM_MONO_FINE_TUNE_VOLUME)>>8);
        //vWriteDspWORD (ADDR_DEM_FMFM_DUAL_FINE_TUNE_VOLUME,u4ReadDspShmDWRD (D_FMFM_DUAL_FINE_TUNE_VOLUME)>>8);

        // Decoder 2
        vWriteDspWORD (u1DspId, ADDR_DEM_SOURCE_MODE_DEC2,u2ReadDspShmWORD (u1DspId, W_SOURCE_MODE));
        vWriteDspWORD (u1DspId, ADDR_DEM_USER_MODE_DEC2,u2ReadDspShmWORD (u1DspId, W_USER_MODE));
        vWriteDspWORD (u1DspId, ADDR_DEM_ADC_GAIN_DEC2,u2ReadDspShmWORD (u1DspId, W_ADC_GAIN));
        vWriteDspWORD (u1DspId, ADDR_FMFM_NUM_CHECK_DEC2,u2ReadDspShmWORD (u1DspId, W_FMFM_NUM_CHECK));
        vWriteDspWORD (u1DspId, ADDR_FMFM_NUM_DOUBLE_CHECK_DEC2,u2ReadDspShmWORD (u1DspId, W_FMFM_NUM_DOUBLE_CHECK));
        vWriteDspWORD (u1DspId, ADDR_FMFM_MONO_WEIGHT_DEC2,u2ReadDspShmWORD (u1DspId, W_FMFM_MONO_WEIGHT));
        vWriteDspWORD (u1DspId, ADDR_FMFM_STEREO_WEIGHT_DEC2,u2ReadDspShmWORD (u1DspId, W_FMFM_STEREO_WEIGHT));
        vWriteDspWORD (u1DspId, ADDR_FMFM_DUAL_WEIGHT_DEC2,u2ReadDspShmWORD (u1DspId, W_FMFM_DUAL_WEIGHT));
        vWriteDspWORD (u1DspId, ADDR_FMFM_DETECTION_CONFID_DEC2, u2ReadDspShmWORD (u1DspId, W_FMFM_DETECT_CONFID));
        break;
    case A2_STREAM:
        vWriteDspWORD (u1DspId, ADDR_DEM_ADC_GAIN,u2ReadDspShmWORD (u1DspId, W_ADC_GAIN));
        vWriteDspWORD (u1DspId, ADDR_DEM_USER_MODE, u2ReadDspShmWORD (u1DspId, W_USER_MODE));
        u1ChannelSrc = DSP_ReturnChannelSource ();
        u2ChannelSrcSet = SOURCE_A2_BG;
        if (u1ChannelSrc == SV_A2_BG)
        {
            u2ChannelSrcSet = SOURCE_A2_BG;
            #ifdef CC_AUD_USE_NVM
            eAudNvmAtvFmt = AUD_NVM_NON_DK;
            #endif
        }
        else if (u1ChannelSrc == SV_A2_DK1)
        {
            u2ChannelSrcSet = SOURCE_A2_DK1;
            #ifdef CC_AUD_USE_NVM
            eAudNvmAtvFmt = AUD_NVM_DK;
            #endif
        }
        else if (u1ChannelSrc == SV_A2_DK2)
        {
            u2ChannelSrcSet = SOURCE_A2_DK2;
            #ifdef CC_AUD_USE_NVM
            eAudNvmAtvFmt = AUD_NVM_DK;
            #endif
        }
        else if (u1ChannelSrc == SV_A2_DK3)
        {
            u2ChannelSrcSet = SOURCE_A2_DK3;
            #ifdef CC_AUD_USE_NVM
            eAudNvmAtvFmt = AUD_NVM_DK;
            #endif
        }
#ifdef CC_AUD_DDI
        else if (u1ChannelSrc == SV_PAL_DK)
        {
            u2ChannelSrcSet = SOURCE_A2_DK3;
            #ifdef CC_AUD_USE_NVM
            eAudNvmAtvFmt = AUD_NVM_DK;
            #endif
        }
#endif
        else if (u1ChannelSrc == SV_NTSC_M)
        {
            u2ChannelSrcSet = SOURCE_NTSC_M;
            #ifdef CC_AUD_USE_NVM
            eAudNvmAtvFmt = AUD_NVM_NON_DK;
            #endif
        }
        vWriteDspWORD (u1DspId, ADDR_DEM_SOURCE_MODE, u2ChannelSrcSet);
        vWriteDspWORD (u1DspId, ADDR_A2_NUM_CHECK, u2ReadDspShmWORD (u1DspId, W_A2_NUM_CHECK));
        vWriteDspWORD (u1DspId, ADDR_A2_NUM_DOUBLE_CHECK,u2ReadDspShmWORD (u1DspId, W_A2_NUM_DOUBLE_CHECK));
        vWriteDspWORD (u1DspId, ADDR_A2_MONO_WEIGHT,u2ReadDspShmWORD (u1DspId, W_A2_MONO_WEIGHT));
        vWriteDspWORD (u1DspId, ADDR_A2_STEREO_WEIGHT,u2ReadDspShmWORD (u1DspId, W_A2_STEREO_WEIGHT));
        vWriteDspWORD (u1DspId, ADDR_A2_DUAL_WEIGHT,u2ReadDspShmWORD (u1DspId, W_A2_DUAL_WEIGHT));
#ifndef CC_AUD_WAFFLE_OVM_SUPPORT
        vWriteDspWORD (u1DspId, ADDR_HDEV_MODE, (uReadDspShmBYTE (u1DspId, B_HDEV_MODE) & 0x02)>>1);
#else
        //vWriteDspWORD (ADDR_HDEV_MODE, (uReadDspShmBYTE (B_HDEV_MODE) & NEW_A2_MASK)>>NEW_A2_SHIFT);
        vWriteDspWORD (u1DspId, ADDR_HDEV_MODE, 0);  //A2 HDEV_MODE protection
#endif
        vWriteDspWORD (u1DspId, ADDR_CARRIER_SHIFT_MODE, (uReadDspShmBYTE (u1DspId, B_CARRIER_SHIFT_MODE) & 0x02)>>1);
        vWriteDspWORD (u1DspId, ADDR_FM_SATU_MUTE_MODE,  (uReadDspShmBYTE (u1DspId, B_FM_SATU_MUTE_MODE)& 0x02)>>1);
        vWriteDspWORD (u1DspId, ADDR_FM_CARRIER_MUTE_MODE,  (uReadDspShmBYTE (u1DspId, B_FM_MUTE_MODE) & 0x02)>>1);
        vWriteDspWORD (u1DspId, ADDR_FM_CARRIER_MUTE_THRESHOLD_HIGHT,  uReadDspShmBYTE (u1DspId, B_FM_MUTE_THRESHOLD_HIGHT_A2));
        vWriteDspWORD (u1DspId, ADDR_FM_CARRIER_MUTE_THRESHOLD_LOW,  uReadDspShmBYTE (u1DspId, B_FM_MUTE_THRESHOLD_LOW_A2));
#ifndef CC_AUD_USE_NVM
        vWriteDspWORD (u1DspId, ADDR_DEM_PAL_FINE_TUNE_VOLUME,u4ReadDspShmDWRD  (u1DspId, D_A2_FINE_TUNE_VOLUME)>>8);
#else
        vWriteDspWORD (u1DspId, ADDR_DEM_PAL_FINE_TUNE_VOLUME,AUD_NVM_QRY_ATV_GAIN(AUD_NVM_QRY_A2_GAIN, eAudNvmAtvFmt)>>8);
#endif
        vWriteDspWORD (u1DspId, ADDR_MANUAL_MTS_SW, uReadDspShmBYTE (u1DspId, B_MANUAL_MTS_SW));
        //vWriteDspWORD (ADDR_MANUAL_OUTPUT_MATRIX, uReadDspShmBYTE (u1DspId, B_MANUAL_OUTPUT_MATRIX));
        vWriteDspWORD (u1DspId, ADDR_FM_NON_EU_MODE,uReadDspShmBYTE  (u1DspId, B_NON_EU_FM_MODE));
        // Decoder 2
        vWriteDspWORD (u1DspId, ADDR_DEM_SOURCE_MODE_DEC2, u2ChannelSrcSet);
        vWriteDspWORD (u1DspId, ADDR_DEM_USER_MODE_DEC2, u2ReadDspShmWORD (u1DspId, W_USER_MODE));
        vWriteDspWORD (u1DspId, ADDR_DEM_ADC_GAIN_DEC2,u2ReadDspShmWORD (u1DspId, W_ADC_GAIN));
#ifndef CC_AUD_USE_NVM
        vWriteDspWORD (u1DspId, ADDR_DEM_PAL_FINE_TUNE_VOLUME_DEC2,u4ReadDspShmDWRD  (u1DspId, D_A2_FINE_TUNE_VOLUME)>>8);
#else
        vWriteDspWORD (u1DspId, ADDR_DEM_PAL_FINE_TUNE_VOLUME_DEC2,AUD_NVM_QRY_ATV_GAIN(AUD_NVM_QRY_A2_GAIN, eAudNvmAtvFmt)>>8);
#endif
        if (u2ChannelSrcSet == SOURCE_NTSC_M)
        {
            vWriteDspWORD (u1DspId, ADDR_A2_STEREO_MONO_THRES, u2ReadDspShmWORD(u1DspId, W_A2_STEREO_MONO_THRES_M));
            vWriteDspWORD (u1DspId, ADDR_A2_MONO_STEREO_THRES, u2ReadDspShmWORD(u1DspId, W_A2_MONO_STEREO_THRES_M));
        }
        else
        {
            vWriteDspWORD (u1DspId, ADDR_A2_STEREO_MONO_THRES, u2ReadDspShmWORD(u1DspId, W_A2_STEREO_MONO_THRES_EU));
            vWriteDspWORD (u1DspId, ADDR_A2_MONO_STEREO_THRES, u2ReadDspShmWORD(u1DspId, W_A2_MONO_STEREO_THRES_EU));
        }
	      vWriteDspWORD (u1DspId, ADDR_A2_WEAK_RF_LVL_S2M_THRES, u2ReadDspShmWORD(u1DspId, W_A2_WEAK_RF_LVL_S2M_THRES_EU));
        vWriteDspWORD (u1DspId, ADDR_A2_WEAK_RF_LVL_M2S_THRES, u2ReadDspShmWORD(u1DspId, W_A2_WEAK_RF_LVL_M2S_THRES_EU));
        //for dynamic detection control
        vWriteDspWORD(u1DspId, ADDR_DYNAMIC_DETECTIO_SWITCH,uReadDspShmBYTE(u1DspId, B_DYNAMIC_DETECTIO_SWITCH));
        break;
    case PAL_STREAM:
        vWriteDspWORD (u1DspId, ADDR_DEM_ADC_GAIN,u2ReadDspShmWORD (u1DspId, W_ADC_GAIN));
        vWriteDspWORD (u1DspId, ADDR_DEM_USER_MODE,u2ReadDspShmWORD (u1DspId, W_USER_MODE));
        vWriteDspWORD (u1DspId, ADDR_DEM_ENFORCED_MODE,u2ReadDspShmWORD (u1DspId, W_ENFORCED_MODE));
        u1ChannelSrc = DSP_ReturnChannelSource ();

#ifdef CC_AUD_DDI
				switch (u1ChannelSrc)
				  {
				  case SV_PAL_BG:
				  case SV_A2_BG :
					  u2ChannelSrcSet = SOURCE_PAL_BG;
					  break;
				  case SV_A2_DK1 :
				  case SV_A2_DK2 :
				  case SV_A2_DK3 :
				  case SV_PAL_DK :
					  u2ChannelSrcSet = SOURCE_PAL_DK;
					  break;
				  case SV_PAL_I :
					  u2ChannelSrcSet = SOURCE_PAL_I; 
					  break;
				  case SV_SECAM_L :
					  u2ChannelSrcSet = SOURCE_SECAM_L;
					  break;
				  case SV_SECAM_L_PLUM:
					  u2ChannelSrcSet = SOURCE_SECAM_L;
					  break;
				  case SV_SECAM_BG:
					  u2ChannelSrcSet = SOURCE_SECAM_BG;
					  break;
				  case SV_SECAM_DK:
					  u2ChannelSrcSet = SOURCE_SECAM_DK;
					  break;
				  default :
					  u2ChannelSrcSet = SOURCE_PAL_I;
				  }
#else
				u2ChannelSrcSet = SOURCE_PAL_I; // default value
				if ((u1ChannelSrc == SV_PAL_BG) || (u1ChannelSrc == SV_A2_BG))
				{
					u2ChannelSrcSet = SOURCE_PAL_BG;
                    #ifdef CC_AUD_USE_NVM
                    eAudNvmAtvFmt = AUD_NVM_NON_DK;
                    #endif
				}
				else if ((u1ChannelSrc == SV_PAL_DK) ||
						(u1ChannelSrc == SV_A2_DK1) ||
						(u1ChannelSrc == SV_A2_DK2) ||
						(u1ChannelSrc == SV_A2_DK3))
				{
					u2ChannelSrcSet = SOURCE_PAL_DK;
                    #ifdef CC_AUD_USE_NVM
                    eAudNvmAtvFmt = AUD_NVM_DK;
                    #endif
				}
				else if (u1ChannelSrc == SV_SECAM_L)
				{
					u2ChannelSrcSet = SOURCE_SECAM_L;
                    #ifdef CC_AUD_USE_NVM
                    eAudNvmAtvFmt = AUD_NVM_NON_DK;
                    #endif
				}
				else if (u1ChannelSrc == SV_SECAM_BG)
				{
					u2ChannelSrcSet = SOURCE_SECAM_BG;
                    #ifdef CC_AUD_USE_NVM
                    eAudNvmAtvFmt = AUD_NVM_NON_DK;
                    #endif
				}
				else if (u1ChannelSrc == SV_SECAM_DK)
				{
					u2ChannelSrcSet = SOURCE_SECAM_DK;
                    #ifdef CC_AUD_USE_NVM
                    eAudNvmAtvFmt = AUD_NVM_DK;
                    #endif
				}
				else if (u1ChannelSrc == SV_SECAM_L_PLUM)
				{
					u2ChannelSrcSet = SOURCE_SECAM_L;
                    #ifdef CC_AUD_USE_NVM
                    eAudNvmAtvFmt = AUD_NVM_NON_DK;
                    #endif
				}
#endif

        vWriteDspWORD (u1DspId, ADDR_DEM_SOURCE_MODE,u2ChannelSrcSet);
        vWriteDspWORD (u1DspId, ADDR_PAL_CORRECT_THRESHOLD,u2ReadDspShmWORD (u1DspId, W_PAL_CORRECT_THRESHOLD));
        vWriteDspWORD (u1DspId, ADDR_PAL_TOTAL_SYNC1_LOOP,u2ReadDspShmWORD (u1DspId, W_PAL_TOTAL_SYNC1_LOOP));
        vWriteDspWORD (u1DspId, ADDR_PAL_ERR_THRESHOLD,u2ReadDspShmWORD (u1DspId, W_PAL_ERR_THRESHOLD));
        vWriteDspWORD (u1DspId, ADDR_PAL_PARITY_ERR_THRESHOLD,u2ReadDspShmWORD (u1DspId, W_PAL_PARITY_ERR_THRESHOLD));
        vWriteDspWORD (u1DspId, ADDR_PAL_EVERY_NUM_FRAMES,u2ReadDspShmWORD (u1DspId, W_PAL_EVERY_NUM_FRAMES));
#ifdef ATV_HDEV_AUTO_SWITCH
        vWriteDspWORD (u1DspId, ADDR_HDEV_MODE, ((uReadDspShmBYTE (u1DspId, B_HDEV_MODE) & 0x01)| 0x03));
#else
#ifndef CC_AUD_WAFFLE_OVM_SUPPORT
#ifdef CC_AUD_PAL_NEW_HDEV_SUPPORT
        vWriteDspWORD (u1DspId, ADDR_HDEV_MODE, ((uReadDspShmBYTE (u1DspId, B_HDEV_MODE) & 0x01) << 2) | 0x1);  // set bit 0 and 2
#else
#ifdef CC_AUD_DDI
		vWriteDspWORD (u1DspId, ADDR_HDEV_MODE, ((uReadDspShmBYTE (u1DspId, B_HDEV_MODE) & 0x01) << 2) );  // set bit 2
#else
        vWriteDspWORD (u1DspId, ADDR_HDEV_MODE, (uReadDspShmBYTE (u1DspId, B_HDEV_MODE) & 0x01));
#endif
#endif
#else
        switch ((uReadDspShmBYTE (u1DspId, B_HDEV_MODE) & NEW_PAL_MASK)>>NEW_PAL_SHIFT)
        {
        case 0:
            vWriteDspWORD (u1DspId, ADDR_HDEV_MODE, 0x0);
            break;
        case 1:
            vWriteDspWORD (u1DspId, ADDR_HDEV_MODE, 0x1);
            break;
        case 2:
            vWriteDspWORD (u1DspId, ADDR_HDEV_MODE, 0x4);
            break;
        default:
            break;
        }
#endif
#endif
        vWriteDspWORD (u1DspId, ADDR_CARRIER_SHIFT_MODE, (uReadDspShmBYTE (u1DspId, B_CARRIER_SHIFT_MODE) & 0x01));
        vWriteDspWORD (u1DspId, ADDR_FM_SATU_MUTE_MODE,  (uReadDspShmBYTE (u1DspId, B_FM_SATU_MUTE_MODE) & 0x01));
        //for AM carrier sense mute
        vWriteDspWORD (u1DspId, ADDR_SECAM_L_MUTE_MODE,  uReadDspShmBYTE (u1DspId, B_AM_MUTE_MODE));
        vWriteDspWORD (u1DspId, ADDR_SECAM_L_MUTE_THRESHOLD_HIGHT, uReadDspShmBYTE (u1DspId, B_AM_MUTE_THRESHOLD_HIGHT));
        vWriteDspWORD (u1DspId, ADDR_SECAM_L_MUTE_THRESHOLD_LOW,uReadDspShmBYTE (u1DspId, B_AM_MUTE_THRESHOLD_LOW));
        //for FM carrier sense mute
        vWriteDspWORD (u1DspId, ADDR_FM_CARRIER_MUTE_MODE,  (uReadDspShmBYTE (u1DspId, B_FM_MUTE_MODE) & 0x01));
#ifndef CC_AUD_WAFFLE_OVM_SUPPORT
        vWriteDspWORD (u1DspId, ADDR_FM_CARRIER_MUTE_THRESHOLD_HIGHT, uReadDspShmBYTE (u1DspId, B_FM_MUTE_THRESHOLD_HIGHT_PAL));
        vWriteDspWORD (u1DspId, ADDR_FM_CARRIER_MUTE_THRESHOLD_LOW,uReadDspShmBYTE (u1DspId, B_FM_MUTE_THRESHOLD_LOW_PAL));
#else
        if (((uReadDspShmBYTE (u1DspId, B_HDEV_MODE) & NEW_PAL_MASK)>>NEW_PAL_SHIFT)==2)
        {
            // for OVM=HIGH threshold
            vWriteDspWORD(u1DspId, ADDR_FM_CARRIER_MUTE_THRESHOLD_HIGHT, uReadDspShmBYTE(u1DspId, B_FM_MUTE_THRESHOLD_HIGHT));
            vWriteDspWORD(u1DspId, ADDR_FM_CARRIER_MUTE_THRESHOLD_LOW,   uReadDspShmBYTE(u1DspId, B_FM_MUTE_THRESHOLD_LOW));
        }
        else
        {
        vWriteDspWORD (u1DspId, ADDR_FM_CARRIER_MUTE_THRESHOLD_HIGHT, uReadDspShmBYTE (u1DspId, B_FM_MUTE_THRESHOLD_HIGHT_PAL));
        vWriteDspWORD (u1DspId, ADDR_FM_CARRIER_MUTE_THRESHOLD_LOW,uReadDspShmBYTE (u1DspId, B_FM_MUTE_THRESHOLD_LOW_PAL));
        }
#endif
        //for EU/NON-EU FM
        vWriteDspWORD (u1DspId, ADDR_FM_NON_EU_MODE,uReadDspShmBYTE  (u1DspId, B_NON_EU_FM_MODE));
        //for NICAM I boost or not
        vWriteDspWORD(u1DspId, ADDR_NICAM_I_BOOST_FLAG,uReadDspShmBYTE(u1DspId, B_NICAM_I_BOOST_FLAG));
         //for PAL FM BAD SIGNAL CHECK
        //vWriteDspWORD (ADDR_PAL_HDEV_BAD_SIGNAL_CHECK,uReadDspShmBYTE (B_FM_BAD_SIGNAL));
        //for dynamic detection control
        vWriteDspWORD(u1DspId, ADDR_DYNAMIC_DETECTIO_SWITCH,uReadDspShmBYTE(u1DspId, B_DYNAMIC_DETECTIO_SWITCH));
#ifndef CC_AUD_USE_NVM
        vWriteDspWORD (u1DspId, ADDR_DEM_PAL_FINE_TUNE_VOLUME,u4ReadDspShmDWRD  (u1DspId, D_PAL_FINE_TUNE_VOLUME)>>8);
        vWriteDspWORD (u1DspId, ADDR_DEM_NICAM_FINE_TUNE_VOLUME,u4ReadDspShmDWRD (u1DspId, D_NICAM_FINE_TUNE_VOLUME)>>8);
        vWriteDspWORD (u1DspId, ADDR_DEM_AM_FINE_TUNE_VOLUME,u4ReadDspShmDWRD  (u1DspId, D_AM_FINE_TUNE_VOLUME)>>8);
#else
         vWriteDspWORD (u1DspId, ADDR_DEM_PAL_FINE_TUNE_VOLUME, AUD_NVM_QRY_ATV_GAIN(AUD_NVM_QRY_PAL_GAIN, eAudNvmAtvFmt)>>8);
         vWriteDspWORD (u1DspId, ADDR_DEM_NICAM_FINE_TUNE_VOLUME, AUD_NVM_QRY_ATV_GAIN(AUD_NVM_QRY_NICAM_GAIN, eAudNvmAtvFmt)>>8);       
         vWriteDspWORD (u1DspId, ADDR_DEM_AM_FINE_TUNE_VOLUME, AUD_NVM_QRY_ATV_GAIN(AUD_NVM_QRY_AM_GAIN, eAudNvmAtvFmt)>>8);        
#endif

        // Decoder 2
        vWriteDspWORD (u1DspId, ADDR_DEM_SOURCE_MODE_DEC2,u2ChannelSrcSet);
        vWriteDspWORD (u1DspId, ADDR_DEM_USER_MODE_DEC2,u2ReadDspShmWORD (u1DspId, W_USER_MODE));
        vWriteDspWORD (u1DspId, ADDR_DEM_ADC_GAIN_DEC2,u2ReadDspShmWORD (u1DspId, W_ADC_GAIN));
        vWriteDspWORD (u1DspId, ADDR_DEM_ENFORCED_MODE_DEC2,u2ReadDspShmWORD (u1DspId, W_ENFORCED_MODE));

#ifndef CC_AUD_USE_NVM
        vWriteDspWORD (u1DspId, ADDR_DEM_PAL_FINE_TUNE_VOLUME_DEC2,u4ReadDspShmDWRD  (u1DspId, D_PAL_FINE_TUNE_VOLUME)>>8);
        vWriteDspWORD (u1DspId, ADDR_DEM_NICAM_FINE_TUNE_VOLUME_DEC2,u4ReadDspShmDWRD (u1DspId, D_NICAM_FINE_TUNE_VOLUME)>>8);
        vWriteDspWORD (u1DspId, ADDR_DEM_AM_FINE_TUNE_VOLUME_DEC2,u4ReadDspShmDWRD  (u1DspId, D_AM_FINE_TUNE_VOLUME)>>8);
#else
        vWriteDspWORD (u1DspId, ADDR_DEM_PAL_FINE_TUNE_VOLUME_DEC2, AUD_NVM_QRY_ATV_GAIN(AUD_NVM_QRY_PAL_GAIN, eAudNvmAtvFmt)>>8);
        vWriteDspWORD (u1DspId, ADDR_DEM_NICAM_FINE_TUNE_VOLUME_DEC2, AUD_NVM_QRY_ATV_GAIN(AUD_NVM_QRY_NICAM_GAIN, eAudNvmAtvFmt)>>8);       
        vWriteDspWORD (u1DspId, ADDR_DEM_AM_FINE_TUNE_VOLUME_DEC2, AUD_NVM_QRY_ATV_GAIN(AUD_NVM_QRY_AM_GAIN, eAudNvmAtvFmt)>>8);        
#endif

        vWriteDspWORD (u1DspId, ADDR_SECAM_L_MUTE_MODE_DEC2,  uReadDspShmBYTE (u1DspId, B_AM_MUTE_MODE));
        vWriteDspWORD (u1DspId, ADDR_SECAM_L_MUTE_THRESHOLD_HIGHT_DEC2, uReadDspShmBYTE (u1DspId, B_AM_MUTE_THRESHOLD_HIGHT));
        vWriteDspWORD (u1DspId, ADDR_SECAM_L_MUTE_THRESHOLD_LOW_DEC2,uReadDspShmBYTE (u1DspId, B_AM_MUTE_THRESHOLD_LOW));
        break;

    case DETECTOR_STREAM:
        vWriteDspWORD (u1DspId, ADDR_DETECTOR_DEFAULT_MODE_DEC2, uReadDspShmBYTE(u1DspId, B_DETECT_DEFAULT));
        vWriteDspWORD (u1DspId, ADDR_DETECTOR_START_FLAG_DEC2, 1); // default turn on detection
        WriteDspCommDram (u1DspId, ADDR_DETECTOR_FM_NOISE_TH_DEC2, u4ReadDspShmDWRD (u1DspId, D_DETECTOR_FM_NOISE_TH));
        vWriteDspWORD (u1DspId, ADDR_DETECTOR_A2_PILOT_CHK_DEC2, uReadDspShmBYTE(u1DspId, B_DETECTOR_A2_PILOT_CHK));
        break;
#endif

    case LPCM_STREAM:
        u1Temp1 = (u2ReadDspShmWORD(u1DspId, W_LPCM_FREQUENCY_DEC2)>>8) & 0xFF;
        u1Temp2 = u2ReadDspShmWORD(u1DspId, W_LPCM_FREQUENCY_DEC2) & 0xFF;

        vWriteDspWORD(u1DspId, ADDR_RC2D_LPCM_CHANNEL_ASSIGNMENT_DEC2, uReadDspShmBYTE(u1DspId, B_LPCM_CH_ASSIGNMENT_DEC2));
        //WriteDspCommDram(ADDR_RC2D_LPCM_FREQUENCY,  u2ReadDspShmWORD(W_LPCM_FREQUENCY));
        WriteDspCommDram(u1DspId, ADDR_RC2D_LPCM_FREQUENCY_DEC2,  (u1Temp1<<12)+u1Temp2);
        vWriteDspWORD(u1DspId, ADDR_RC2D_LPCM_BIT_SHIFT_DEC2,  uReadDspShmBYTE(u1DspId, B_LPCM_BIT_SHIFT_DEC2));
        vWriteDspWORD(u1DspId, ADDR_RC2D_LPCM_DRC_VALUE_DEC2,  uReadDspShmBYTE(u1DspId, B_LPCM_DRC_VALUE_DEC2));
        WriteDspCommDram(u1DspId, ADDR_RC2D_LPCM_Q_DEC2,  u2ReadDspShmWORD(u1DspId, W_LPCM_Q_VALUE_DEC2));
        vWriteDspWORD(u1DspId, ADDR_RC2D_LPCM_DRC_FLAG_DEC2,  uReadDspShmBYTE(u1DspId, B_LPCM_DRC_FLAG_DEC2));
        vWriteDspWORD(u1DspId, ADDR_RC2D_LPCM_DECODING_TYPE_DEC2,  uReadDspShmBYTE(u1DspId, W_LPCM_DECODING_TYPE_DEC2));

        if (uReadDspShmBYTE(u1DspId, W_ADPCM_DECODING_TYPE_DEC2)!=0) //ADPCM
        {
            vWriteDspWORD(u1DspId, ADDR_RC2D_LPCM_ADPCM_DEC_TYPE_DEC2, uReadDspShmBYTE(u1DspId, W_ADPCM_DECODING_TYPE_DEC2));
            vWriteDspWORD(u1DspId, ADDR_RC2D_LPCM_ADPCM_BITS_PER_SAMPLE_DEC2, uReadDspShmBYTE(u1DspId, B_ADPCM_BIT_PERSAMPLE_DEC2));
            vWriteDspWORD(u1DspId, ADDR_RC2D_ADPCM_BLOCK_ALIGN_DEC2, u2ReadDspShmWORD(u1DspId, W_ADPCM_BLOCK_ALIGN_DEC2));
            vWriteDspWORD(u1DspId, ADDR_RC2D_ADPCM_CHANNEL_NUM_DEC2, uReadDspShmBYTE(u1DspId, B_ADPCM_CHANNEL_NUM_DEC2));
        }
        break;
    case DTSDEC_STREAM:
        if (AUD_GetDTSInfo())
        {
            //Big Endian
            vWriteDspWORD (u1DspId, ADDR_RC2D_DTS_DRC_FLAG_DEC2, 0xffd9);
        }
        else
        {
            //Little Endian
            vWriteDspWORD (u1DspId, ADDR_RC2D_DTS_DRC_FLAG_DEC2, 0xfffb);
        }
	    vWriteDspWORD (u1DspId, ADDR_RC2D_DTS_LFE_mixto_FRONT_DEC2, uReadDspShmBYTE(u1DspId, B_DTS_BC_LFE_MIXTO_FRONT_DEC2));
	    vWriteDspWORD (u1DspId, ADDR_RC2D_DTS_DRC_ONOFF_DEC2, uReadDspShmBYTE(u1DspId, B_DTS_BC_DRC_DEC2));
	    vWriteDspWORD (u1DspId, ADDR_RC2D_DTS_DIALNORM_ONOFF_DEC2, uReadDspShmBYTE(u1DspId, B_DTS_BC_Dialnrom_DEC2));
	    WriteDspCommDram (u1DspId, ADDR_RC2D_DTS_DRC_LOW_DEC2, u4ReadDspShmDWRD (u1DspId, D_DTS_BC_DRC_LOW));
        WriteDspCommDram (u1DspId, ADDR_RC2D_DTS_DRC_MEDIUM_DEC2, u4ReadDspShmDWRD (u1DspId, D_DTS_BC_DRC_MEDIUM));
        WriteDspCommDram (u1DspId, ADDR_RC2D_DTS_DRC_HIGH_DEC2, u4ReadDspShmDWRD (u1DspId, D_DTS_BC_DRC_HIGH));
        WriteDspCommDram (u1DspId, ADDR_RC2D_DTS_DIALNORM_LOW_DEC2, u4ReadDspShmDWRD (u1DspId, D_DTS_BC_Dialnorm_LOW));
        WriteDspCommDram (u1DspId, ADDR_RC2D_DTS_DIALNORM_MEDIUM_DEC2, u4ReadDspShmDWRD (u1DspId, D_DTS_BC_Dialnorm_MEDIUM));
        WriteDspCommDram (u1DspId, ADDR_RC2D_DTS_DIALNORM_HIGH_DEC2, u4ReadDspShmDWRD (u1DspId, D_DTS_BC_Dialnorm_HIGH));

        break;

    case AAC_STREAM:
//#ifndef CC_AUD_SUPPORT_MS10
#ifndef CC_AUD_DOLBY_SUPPORT_DDT

        vWriteDspWORD (u1DspId, ADDR_RC2D_AAC_DOWNMIX_LEVEL_DEC2,
                       AAC_DEFAULT_DOWNMIX_LEVEL);
        vWriteDspWORD (u1DspId, ADDR_RC2D_AAC_TARGET_LEVEL_DEC2,
                       AAC_TARGET_LEVEL);
#else
        vWriteDspWORD (u1DspId, ADDR_RC2D_DDT_DUAL_MODE_DEC2,
                       uReadDspShmBYTE (u1DspId, B_DDT_DUALMODE_DEC2));
        vWriteDspWORD (u1DspId, ADDR_RC2D_DDT_DRC_DEC2,
                       uReadDspShmBYTE (u1DspId, B_DDT_COMPVAL_DEC2));
        vWriteDspWORD (u1DspId, ADDR_RC2D_DDT_DMX_MODE_DEC2,
                       uReadDspShmBYTE (u1DspId, B_DDT_COMPMODE_DEC2));

#endif
        vWriteDspWORD (u1DspId, ADDR_RC2D_DDT_CONTROL_DEC2,
                       uReadDspShmBYTE(u1DspId, B_DDT_CONTROL));

        //PTS Table Shift Amount
#if defined(CC_AUD_ARM_SUPPORT) && defined(CC_AUD_ARM_RENDER)
        if (uReadDspShmBYTE(u1DspId, B_DDT_CONTROL)==0) //EU Mode
        {
            u4Temp3 = dReadDspCommDram(u1DspId, ADDR_RC2D_PTS_STC_CTRL_FLAG);
            u4Temp3 |= 0x8000; //Set bit 7 (align @ bit8)
            WriteDspCommDram (u1DspId, ADDR_RC2D_PTS_STC_CTRL_FLAG, u4Temp3);  
        }

        break;
#endif
	case COOK_STREAM:
#if 0
		vWriteDspWORD (ADDR_RC2D_COOK_SAMP_PER_FRAME_DEC2,0x400);
		vWriteDspWORD (ADDR_RC2D_COOK_CHANNEL_NUM_DEC2,0x2);
		vWriteDspWORD (ADDR_RC2D_COOK_INPUT_SAMPLING_RATE_DEC2,0xac44);
		vWriteDspWORD (ADDR_RC2D_COOK_FRM_SZ_IN_BYTES_DEC2,0x80);
		vWriteDspWORD (ADDR_RC2D_COOK_REGION_NUM_DEC2,0x25);
		vWriteDspWORD (ADDR_RC2D_COOK_COUPLING_START_REGN_DEC2,0x2);
		vWriteDspWORD (ADDR_RC2D_COOK_COUPLING_QBITS_DEC2,0x4);
		LOG(0," ===========>COOK DEC2 Initial \n");

#else
	  	vWriteDspWORD (u1DspId, ADDR_RC2D_COOK_SAMP_PER_FRAME_DEC2,
		  	u2ReadDspShmWORD(u1DspId, W_COOK_SAMP_PER_FRAME));
	  	vWriteDspWORD (u1DspId, ADDR_RC2D_COOK_CHANNEL_NUM_DEC2,
		  	u2ReadDspShmWORD(u1DspId, W_COOK_CHANNEL_NUM));
	  	vWriteDspWORD (u1DspId, ADDR_RC2D_COOK_INPUT_SAMPLING_RATE_DEC2,
		  	u2ReadDspShmWORD(u1DspId, W_COOK_INPUT_SAMPLING_RATE));
	  	vWriteDspWORD (u1DspId, ADDR_RC2D_COOK_FRM_SZ_IN_BYTES_DEC2,
		  	u2ReadDspShmWORD(u1DspId, W_COOK_FRM_SZ_IN_BYTES));
	  	vWriteDspWORD (u1DspId, ADDR_RC2D_COOK_REGION_NUM_DEC2,
		  	u2ReadDspShmWORD(u1DspId, W_COOK_REGION_NUM));
	  	vWriteDspWORD (u1DspId, ADDR_RC2D_COOK_COUPLING_START_REGN_DEC2,
		  	u2ReadDspShmWORD(u1DspId, W_COOK_COUPLING_START_REGN));
	  	vWriteDspWORD (u1DspId, ADDR_RC2D_COOK_COUPLING_QBITS_DEC2,
		  	u2ReadDspShmWORD(u1DspId, W_COOK_COUPLING_QBITS));
#endif
	  break;

    #if 1///def CC_VORBIS_SUPPORT
    case VORBIS_STREAM:
        vWriteDspWORD(u1DspId, ADDR_RC2D_VORBIS_PACKET_NO, u2ReadDspShmWORD(u1DspId, W_VORBISPACKET));
        vWriteDspWORD(u1DspId, ADDR_D2RC_VORBIS_TABLE_OK_DEC2, 0);
        vWriteDspWORD(u1DspId, ADDR_D2RC_VORBIS_ABS_GRANULE_POS_LL16_DEC2, 0);
        vWriteDspWORD(u1DspId, ADDR_D2RC_VORBIS_ABS_GRANULE_POS_LH16_DEC2, 0);
        vWriteDspWORD(u1DspId, ADDR_D2RC_VORBIS_ABS_GRANULE_POS_OK_DEC2, 0);
        /// Decode Dram is big enough
        /// DSP_DAT_DEC2_SZ (0x12A00*4) = 0x12A00
        /// Vorbis
        /// OUTPUT_BUF_ADR (016B00h-08000h)/100h = 0xEB00
        /// Aout Buffer = 0x12A00 - 0xEB00 = 0x3F00
        /// Dec2 Each Banck Compact Size = 256*3/4*2 = 0x180
        /// Aout Maximum Buffer Bank Size = 0x3F00/0x180 = 0x2A(42) > (8) and (18)
        /// it is big enough, no need to limit it when play vorbis.
        /// Dec2: 8.
        ///vWriteDspWORD (ADDR_RC2D_AOUT_BANK_256_NUM_DEC2, 8);   ///[Joel] Vorbis decoder dram only support 8+30 Bank
        break;
    #endif


#ifdef DATA_DISC_WMA_SUPPORT
        case WMA_STREAM:

            vWriteDspWORD (u1DspId, ADDR_RC2D_CTRL_5_NUM_1, (UINT16)0xB);
            //WNA Packet No
            vWriteDspWORD (u1DspId, ADDR_RC2D_WMA_PACKET_NO_DEC2,
                           u2ReadDspShmWORD (u1DspId, W_WMA_PACKET_NO_DEC2));
            vWriteDspWORD (u1DspId, ADDR_RC2D_WMA_HEADER_SOURCE_DEC2,
                           u2ReadDspShmWORD (u1DspId, W_WMA_HEADER_SOURCE_DEC2));
            WriteDspCommDram (u1DspId, ADDR_RC2D_WMA_PACKET_COUNT_DEC2,
                           ((u4ReadDspShmDWRD (u1DspId, D_WMA_PACKET_COUNT_DEC2))<<8));
            WriteDspCommDram (u1DspId, ADDR_RC2D_WMA_PACKET_SIZE_DEC2,
                           ((u4ReadDspShmDWRD (u1DspId, D_WMA_PACKET_SIZE_DEC2))<<8));
            vWriteDspWORD (u1DspId, ADDR_RC2D_WMA_VERSION_DEC2,
                           u2ReadDspShmWORD (u1DspId, W_WMA_VERSION_DEC2));
            vWriteDspWORD (u1DspId, ADDR_RC2D_WMA_NUM_CHANNELS_DEC2,
                           u2ReadDspShmWORD (u1DspId, W_WMA_NUM_CH_DEC2));
            WriteDspCommDram (u1DspId, ADDR_RC2D_WMA_SAMPLE_PER_SECOND_DEC2,
                           ((u4ReadDspShmDWRD (u1DspId, D_WMA_SAMPLE_PER_SEC_DEC2))<<8));
            WriteDspCommDram (u1DspId, ADDR_RC2D_WMA_BYTES_PER_SECOND_DEC2,
                           ((u4ReadDspShmDWRD (u1DspId, D_WMA_BYTE_PER_SEC_DEC2))<<8));
            WriteDspCommDram (u1DspId, ADDR_RC2D_WMA_BLOCK_SIZE_DEC2,
                           ((u4ReadDspShmDWRD (u1DspId, D_WMA_BLOCK_SIZE_DEC2))<<8));
            vWriteDspWORD (u1DspId, ADDR_RC2D_WMA_ENCODER_OPTIONS_DEC2,
                           u2ReadDspShmWORD (u1DspId, W_WMA_ENCODER_OPTION_DEC2));
            //koro : if issue play without setting audio format (seek mode), wma need initial table
            vLoadDspDRAMTable (u1DspId, AUD_WMA_DEC2);
            vWriteDspWORD (u1DspId, ADDR_RC2D_DOWNLOAD_TABLE_FLAG_DEC2, 0x1);

            break;
#endif

    case WMAPRO_STREAM:

        vWriteDspWORD (u1DspId, ADDR_RC2D_CTRL_5_NUM_1, (UINT16)0xC);
        //WNA Packet No
        vWriteDspWORD (u1DspId, ADDR_RC2D_WMAPRO_PACKET_NO_DEC2,
                       u2ReadDspShmWORD (u1DspId, W_WMAPRO_PACKET_NO_DEC2));
        vWriteDspWORD (u1DspId, ADDR_RC2D_WMAPRO_HEADER_SOURCE_DEC2,
                       u2ReadDspShmWORD (u1DspId, W_WMAPRO_HEADER_SOURCE_DEC2));
        vWriteDspWORD (u1DspId, ADDR_RC2D_WMAPRO_NUM_CHANNELS_DEC2,
                       u2ReadDspShmWORD (u1DspId, W_WMAPRO_NUM_CH_DEC2));
        WriteDspCommDram (u1DspId, ADDR_RC2D_WMAPRO_SAMPLE_PER_SECOND_DEC2,
                       ((u4ReadDspShmDWRD (u1DspId, D_WMAPRO_SAMPLE_PER_SEC_DEC2))<<8));
        WriteDspCommDram (u1DspId, ADDR_RC2D_WMAPRO_BYTES_PER_SECOND_DEC2,
                       ((u4ReadDspShmDWRD (u1DspId, D_WMAPRO_BYTE_PER_SEC_DEC2))<<8));
        WriteDspCommDram (u1DspId, ADDR_RC2D_WMAPRO_BLOCK_SIZE_DEC2,
                       ((u4ReadDspShmDWRD (u1DspId, D_WMAPRO_BLOCK_SIZE_DEC2))<<8));
        vWriteDspWORD (u1DspId, ADDR_RC2D_WMAPRO_ENCODER_OPTIONS_DEC2,
                       u2ReadDspShmWORD (u1DspId, W_WMAPRO_ENCODER_OPTION_DEC2));
        WriteDspCommDram (u1DspId, ADDR_RC2D_WMAPRO_CH_MASK_DEC2,
                       ((u4ReadDspShmDWRD (u1DspId, D_WMAPRO_CH_MASK_DEC2))<<8));
        vWriteDspWORD (u1DspId, ADDR_RC2D_WMAPRO_VALID_BITS_DEC2,
                       u2ReadDspShmWORD (u1DspId, W_WMAPRO_VALID_BITS_DEC2));
        vWriteDspWORD (u1DspId, ADDR_RC2D_WMAPRO_ADV_OPTION_DEC2,
                       u2ReadDspShmWORD (u1DspId, W_WMAPRO_ADV_OPTION_DEC2));
        WriteDspCommDram (u1DspId, ADDR_RC2D_WMAPRO_ADV_OPTION2_DEC2,
                       ((u4ReadDspShmDWRD (u1DspId, D_WMAPRO_ADV_OPTION2_DEC2))<<8));

        break;

    case FLAC_STREAM://paul_flac_2
    #if 0
        WriteDspCommDram(ADDR_RC2D_FLAC_MIN_BLK_SZ_DEC2, 0x1000);
		WriteDspCommDram(ADDR_RC2D_FLAC_MAX_BLK_SZ_DEC2, 0x1000);
		WriteDspCommDram(ADDR_RC2D_FLAC_CH_NUM_1_DEC2, 1);
		WriteDspCommDram(ADDR_RC2D_FLAC_BPS_1_DEC2, 15);
		WriteDspCommDram(ADDR_RC2D_FLAC_MIN_FRM_SZ_DEC2, 0x398);
		WriteDspCommDram(ADDR_RC2D_FLAC_MAX_FRM_SZ_DEC2, 0x12a0);
		WriteDspCommDram(ADDR_RC2D_FLAC_SAMP_RATE_DEC2, 0xBB80);
    #else
             WriteDspCommDram(u1DspId, ADDR_RC2D_FLAC_MIN_BLK_SZ_DEC2, u2ReadDspShmWORD(u1DspId, W_FLAC_MIN_BLK_SZ));
		WriteDspCommDram(u1DspId, ADDR_RC2D_FLAC_MAX_BLK_SZ_DEC2, u2ReadDspShmWORD(u1DspId, W_FLAC_MAX_BLK_SZ));
		WriteDspCommDram(u1DspId, ADDR_RC2D_FLAC_CH_NUM_1_DEC2, u2ReadDspShmWORD(u1DspId, W_FLAC_CHN_NUM));
		WriteDspCommDram(u1DspId, ADDR_RC2D_FLAC_BPS_1_DEC2, u2ReadDspShmWORD(u1DspId, W_FLAC_BPS));
		WriteDspCommDram(u1DspId, ADDR_RC2D_FLAC_MIN_FRM_SZ_DEC2, u4ReadDspShmDWRD(u1DspId, D_FLAC_MIN_FRM_SZ));
		WriteDspCommDram(u1DspId, ADDR_RC2D_FLAC_MAX_FRM_SZ_DEC2, u4ReadDspShmDWRD(u1DspId, D_FLAC_MAX_FRM_SZ));
		WriteDspCommDram(u1DspId, ADDR_RC2D_FLAC_SAMP_RATE_DEC2, u4ReadDspShmDWRD(u1DspId, D_FLAC_SMP_RATE));
    #endif
        break;
    case G726_STREAM:
		WriteDspCommDram(u1DspId, ADDR_RC2D_G726_FRAME_SZ_DEC2, u2ReadDspShmWORD(u1DspId, W_G726_FRAME_DEC2)<<8);
		WriteDspCommDram(u1DspId, ADDR_RC2D_G726_CH_NUM_DEC2, uReadDspShmBYTE(u1DspId, B_G726_CH_DEC2)<<8);
		WriteDspCommDram(u1DspId, ADDR_RC2D_G726_RATE_DEC2, uReadDspShmBYTE(u1DspId, B_G726_RATE_DEC2)<<8);
		WriteDspCommDram(u1DspId, ADDR_RC2D_G726_LAW_DEC2, uReadDspShmBYTE(u1DspId, B_G726_LAW_DEC2)<<8);
        WriteDspCommDram(u1DspId, ADDR_RC2D_G726_SRATE_DEC2, uReadDspShmBYTE(u1DspId, B_G726_SRATE_DEC2)<<8);
        break;
    default:
        break;
    }
}

#ifdef CC_MT5391_AUD_3_DECODER
/***************************************************************************
     Function : vDecCommonInitDec3 (for third decoder)
  Description : This function is used to init the decoder EACH time when it
                is played. In other words, it is to set the properties which
                is different song by song. Note currently we only allow PCM and MP2 played on dec3
    Parameter : None
    Return    : None
***************************************************************************/
void vDecCommonInitDec3 (UINT8 u1DspId)
{
    TV_AUD_SYS_T u1ChannelSrc;
    UINT16 u2ChannelSrcSet;
    UINT8 u1Temp1, u1Temp2;  //parson MM3
    UINT32 u4Temp3;    

    // Bonding options
    vWriteDspWORD (u1DspId, ADDR_RC2D_BONDING_OPTIONS, u2ReadDspShmWORD (u1DspId, W_BONDING_OPTIONS));

    //CTRL6 NUM_1
    vWriteDspWORD (u1DspId, ADDR_RC2D_CTRL_6_NUM_1, (UINT16)CTRL_6_NUM_1);

    //Soft Mute (Max. 7)
    vWriteDspWORD (u1DspId, ADDR_RC2D_VOLUME_SOFT_MUTE_ORDER_DEC3,
        u2ReadDspShmWORD (u1DspId, B_SOFTMUTEORDER_DEC3));

    //PTS Table Shift Amount
    u4Temp3 = dReadDspCommDram(u1DspId, ADDR_RC2D_PTS_STC_CTRL_FLAG);
    u4Temp3 &= 0xFEFF00; //Clear bit 8 (align @ bit8)
    WriteDspCommDram (u1DspId, ADDR_RC2D_PTS_STC_CTRL_FLAG, u4Temp3);        
    

    DSP_FlushInvalidateDCache(u1DspId, CACHE_FLUSH_TYPE_DATA, TYPE_COMM_DRAM_IDX);

    switch (_rDspVars[u1DspId][AUD_DEC_THIRD].bDspStrTyp)
    {
    case PCM_STREAM:
        //u4DspData = wReadDspWORD (ADDR_D2RC_INPUT_CH_CFG_DEC3);
        //u4DspData = u4DspData & ~0xf;
        //u4DspData = u4DspData | 0x2;  // Hard set cdda channel config
        //vWriteDspWORD (ADDR_D2RC_INPUT_CH_CFG_DEC3, u4DspData);
        vWriteDspWORD (u1DspId, ADDR_RC2D_PCM_INPUT_CHANNEL_NUM_DEC3,
                       u2ReadDspShmWORD (u1DspId, W_PCM_INPUT_CHANNEL_NUM_DEC3));
        vWriteDspWORD (u1DspId, ADDR_RC2D_PCM_INPUT_CH_ASSIGNMENT_DEC3,
                       u2ReadDspShmWORD (u1DspId, W_PCM_INPUT_CH_ASSIGNMENT_DEC3));
        vWriteDspWORD (u1DspId, ADDR_RC2D_PCM_INPUT_TYPE_DEC3,
                       u2ReadDspShmWORD (u1DspId, W_PCM_INPUT_TYPE_DEC3));  //0:pcm24 1:IMA 3:MS 2:pcm16 //msb=1: little endian
        vWriteDspWORD (u1DspId, ADDR_RC2D_PCM_INPUT_SAMPLING_RATE_DEC3,
                       u2ReadDspShmWORD (u1DspId, W_PCM_INPUT_SAMPLING_RATE_DEC3));
#if 0
        vWriteDspWORD (u1DspId, ADDR_RC2D_ADPCM_BLOCK_ALIGN_DEC2,
                       u2ReadDspShmWORD (u1DspId, W_ADPCM_BLOCK_ALIGN));
        vWriteDspWORD (u1DspId, ADDR_RC2D_ADPCM_CHANNEL_NUM_DEC2,
                       u2ReadDspShmWORD (u1DspId, B_ADPCM_CHANNEL_NUM));
#endif
        break;

case AC3_STREAM:
	 // Use the same settings as decoder 2, fix me!!!!
        //AC3 Karaoke Mode
        vWriteDspWORD (u1DspId, ADDR_RC2D_AC3_KARAOKE_MODE_DEC3,
                       u2ReadDspShmWORD (u1DspId, W_AC3KARAMOD_DEC3));
        //AC3 Dual Mono Mode
        vWriteDspWORD (u1DspId, ADDR_RC2D_AC3_DUAL_MODE_DEC3,
                       u2ReadDspShmWORD (u1DspId, W_AC3DUALMODE_DEC3));
        //AC3 Compression Mode
        vWriteDspWORD (u1DspId, ADDR_RC2D_AC3_COMPRESS_MODE_DEC3,
                       u2ReadDspShmWORD (u1DspId, W_AC3COMPMOD_DEC3));
        //AC3 Low Boost
        WriteDspCommDram (u1DspId, ADDR_RC2D_AC3_DYNAMIC_LOW_DEC3,
                          u4ReadDspShmDWRD (u1DspId, D_AC3DYN_LOW_DEC3));
        //AC3 High Cut
        WriteDspCommDram (u1DspId, ADDR_RC2D_AC3_DYNAMIC_HIGH_DEC3,
                          u4ReadDspShmDWRD (u1DspId, D_AC3DYN_HIGH_DEC3));
        // AC3 Auto downmix
        WriteDspCommDram (u1DspId, ADDR_RC2D_AC3_DOWNMIX_DEC3,
                          uReadDspShmBYTE (u1DspId, B_AC3AUTODNMIX_DEC3));
        // DDC control
        vWriteDspWORD (u1DspId, ADDR_RC2D_AC3_CONTROL_DEC3, u2ReadDspShmWORD(u1DspId, W_DDC_CONTROL_DEC2));
        break;

    case MPEG12_STREAM:
        /* 20041220: For testing, hard coded here, remove in the Future */
        // MPEG sync no error checking
//      vWriteDspWORD (ADDR_RC2D_MPEG_SYNC_MODE_DEC2, 0x1);
        //MPEG Error Detection
        vWriteDspWORD (u1DspId, ADDR_RC2D_MPEG_CRC_FLAG_DEC3,
        u2ReadDspShmWORD (u1DspId, W_MPEGERRDET_DEC3));
        break;
#ifdef DSP_SUPPORT_NPTV
    case NTSC_STREAM:
        vWriteDspWORD (u1DspId, ADDR_DEM_ADC_GAIN,u2ReadDspShmWORD (u1DspId, W_ADC_GAIN));
        vWriteDspWORD (u1DspId, ADDR_DEM_USER_MODE,u2ReadDspShmWORD (u1DspId, W_USER_MODE));
        vWriteDspWORD (u1DspId, ADDR_DEM_SOURCE_MODE,u2ReadDspShmWORD (u1DspId, W_SOURCE_MODE));
        vWriteDspWORD (u1DspId, ADDR_MTS_NUM_CHECK, u2ReadDspShmWORD (u1DspId, W_MTS_NUM_CHECK));
        vWriteDspWORD (u1DspId, ADDR_MTS_STEREO_MID,u2ReadDspShmWORD (u1DspId, W_MTS_STEREO_MID));
        vWriteDspWORD (u1DspId, ADDR_MTS_STEREO_CON_MID, u2ReadDspShmWORD (u1DspId, W_MTS_STEREO_CON_MID));
        vWriteDspWORD (u1DspId, ADDR_MTS_NUM_PILOT, u2ReadDspShmWORD (u1DspId, W_MTS_NUM_PILOT));
        vWriteDspWORD (u1DspId, ADDR_MTS_NUM_SAP, u2ReadDspShmWORD (u1DspId, W_MTS_NUM_SAP));
        vWriteDspWORD (u1DspId, ADDR_MTS_SAP_MID, u2ReadDspShmWORD (u1DspId, W_MTS_SAP_MID));
        vWriteDspWORD (u1DspId, ADDR_MTS_SAP_CON_MID,u2ReadDspShmWORD (u1DspId, W_MTS_SAP_CON_MID));
        vWriteDspWORD (u1DspId, ADDR_RC2D_MTS_LR_AMP_M, 0x14b);
        vWriteDspWORD (u1DspId, ADDR_RC2D_MTS_LR_AMP_M_576K, 0x124);
        vWriteDspWORD (u1DspId, ADDR_RC2D_MTS_LR_AMP_N, 0x140);
        vWriteDspWORD (u1DspId, ADDR_RC2D_MTS_LR_AMP_N_576K, 0x15a);
#ifdef CC_AUD_MTS_OUTPUT_HDEV_GAIN_SUPPORT
        vWriteDspWORD (u1DspId, ADDR_HDEV_MODE, (uReadDspShmBYTE (u1DspId, B_HDEV_MODE) & 0x10)>>3);  // set bit 1
#else
        vWriteDspWORD (u1DspId, ADDR_HDEV_MODE, (uReadDspShmBYTE (u1DspId, B_HDEV_MODE) & 0x04)>>2);
#endif
        vWriteDspWORD (u1DspId, ADDR_CARRIER_SHIFT_MODE, (uReadDspShmBYTE (u1DspId, B_CARRIER_SHIFT_MODE) & 0x04)>>2);
        vWriteDspWORD (u1DspId, ADDR_FM_SATU_MUTE_MODE,  (uReadDspShmBYTE (u1DspId, B_FM_SATU_MUTE_MODE)& 0x04)>>2);
        vWriteDspWORD (u1DspId, ADDR_FM_CARRIER_MUTE_MODE, (uReadDspShmBYTE (u1DspId, B_FM_MUTE_MODE) & 0x04)>>2);
        vWriteDspWORD (u1DspId, ADDR_FM_CARRIER_MUTE_THRESHOLD_HIGHT, uReadDspShmBYTE (u1DspId, B_FM_MUTE_THRESHOLD_HIGHT_MTS));
        vWriteDspWORD (u1DspId, ADDR_FM_CARRIER_MUTE_THRESHOLD_LOW, uReadDspShmBYTE (u1DspId, B_FM_MUTE_THRESHOLD_LOW_MTS));
        vWriteDspWORD (u1DspId, ADDR_DEM_MTS_FINE_TUNE_VOLUME,u4ReadDspShmDWRD  (u1DspId, D_MTS_FINE_TUNE_VOLUME)>>8);
        vWriteDspWORD (u1DspId, ADDR_DEM_SAP_FINE_TUNE_VOLUME,u4ReadDspShmDWRD (u1DspId, D_SAP_FINE_TUNE_VOLUME)>>8);
        vWriteDspWORD (u1DspId, ADDR_MTS_PILOT_OFFSET_DETECTION,uReadDspShmBYTE (u1DspId, B_MTS_PILOT_OFFSET_DETECTION));
        vWriteDspWORD (u1DspId, ADDR_MTS_MUTE_SAP_LOW,uReadDspShmBYTE (u1DspId, B_MTS_MUTE_SAP_LOW));
        vWriteDspWORD (u1DspId, ADDR_MTS_MUTE_SAP_HIGH,uReadDspShmBYTE (u1DspId, B_MTS_MUTE_SAP_HIGH));
        vWriteDspWORD (u1DspId, ADDR_MTS_SATU_MUTE_HIGH,uReadDspShmBYTE (u1DspId, B_SATU_MUTE_THRESHOLD_HIGH));
        vWriteDspWORD (u1DspId, ADDR_MTS_SATU_MUTE_LOW,uReadDspShmBYTE (u1DspId, B_SATU_MUTE_THRESHOLD_LOW));
        vWriteDspWORD (u1DspId, ADDR_DEFAULT_MTS_MODE, 0);  // no default mode
        vWriteDspWORD (u1DspId, ADDR_MTS_SAP_HP_NOISE_LOW,uReadDspShmBYTE (u1DspId, B_SAP_HP_MUTE_LOW));
        vWriteDspWORD (u1DspId, ADDR_MTS_SAP_HP_NOISE_HIGH,uReadDspShmBYTE (u1DspId, B_SAP_HP_MUTE_HIGH));
        vWriteDspWORD (u1DspId, ADDR_MTS_SAP_FILTER_SEL,uReadDspShmBYTE (u1DspId, B_SAP_FILTER_SEL));
        vWriteDspWORD (u1DspId, ADDR_MTS_SAP_RATIO,uReadDspShmBYTE (u1DspId, B_SAP_RATIO));
        vWriteDspWORD (u1DspId, ADDR_MTS_SAP_MUTE_CTRL,uReadDspShmBYTE (u1DspId, B_MTS_SAP_MUTE_CTRL));
        vWriteDspWORD (u1DspId, ADDR_MTS_HDEV_LVL_REDUCE,uReadDspShmBYTE (u1DspId, B_MTS_HDEV_LVL_REDUCE));
#ifdef CC_AUD_MTS_OUTPUT_LEVEL_GAIN_SUPPORT
        vWriteDspWORD (u1DspId, ADDR_MTS_OUTPUT_LEVEL_GAIN, u4ReadDspShmDWRD(u1DspId, D_MTS_OUTPUT_LEVEL_GAIN));
#endif

        // Decoder 3
        vWriteDspWORD (u1DspId, ADDR_DEM_SOURCE_MODE_DEC3,u2ReadDspShmWORD (u1DspId, W_SOURCE_MODE));
        vWriteDspWORD (u1DspId, ADDR_DEM_USER_MODE_DEC3,u2ReadDspShmWORD (u1DspId, W_USER_MODE));
        vWriteDspWORD (u1DspId, ADDR_DEM_ADC_GAIN_DEC3,u2ReadDspShmWORD (u1DspId, W_ADC_GAIN));
        vWriteDspWORD (u1DspId, ADDR_DEM_SAP_FINE_TUNE_VOLUME_DEC3,u4ReadDspShmDWRD (u1DspId, D_SAP_FINE_TUNE_VOLUME)>>8);
        vWriteDspWORD (u1DspId, ADDR_DEM_MTS_FINE_TUNE_VOLUME_DEC3,u4ReadDspShmDWRD  (u1DspId, D_MTS_FINE_TUNE_VOLUME)>>8);
        break;
    case A2_STREAM:
        vWriteDspWORD (u1DspId, ADDR_DEM_ADC_GAIN,u2ReadDspShmWORD (u1DspId, W_ADC_GAIN));
        vWriteDspWORD (u1DspId, ADDR_DEM_USER_MODE, u2ReadDspShmWORD (u1DspId, W_USER_MODE));
        u1ChannelSrc = DSP_ReturnChannelSource ();
        u2ChannelSrcSet = SOURCE_A2_BG;
        if (u1ChannelSrc == SV_A2_BG)
        {
            u2ChannelSrcSet = SOURCE_A2_BG;
        }
        else if (u1ChannelSrc == SV_A2_DK1)
        {
            u2ChannelSrcSet = SOURCE_A2_DK1;
        }
        else if (u1ChannelSrc == SV_A2_DK2)
        {
            u2ChannelSrcSet = SOURCE_A2_DK2;
        }
        else if (u1ChannelSrc == SV_A2_DK3)
        {
            u2ChannelSrcSet = SOURCE_A2_DK3;
        }
        else if (u1ChannelSrc == SV_NTSC_M)
        {
            u2ChannelSrcSet = SOURCE_NTSC_M;
        }
        vWriteDspWORD (u1DspId, ADDR_DEM_SOURCE_MODE, u2ChannelSrcSet);
        vWriteDspWORD (u1DspId, ADDR_A2_NUM_CHECK, u2ReadDspShmWORD (u1DspId, W_A2_NUM_CHECK));
        vWriteDspWORD (u1DspId, ADDR_A2_NUM_DOUBLE_CHECK,u2ReadDspShmWORD (u1DspId, W_A2_NUM_DOUBLE_CHECK));
        vWriteDspWORD (u1DspId, ADDR_A2_MONO_WEIGHT,u2ReadDspShmWORD (u1DspId, W_A2_MONO_WEIGHT));
        vWriteDspWORD (u1DspId, ADDR_A2_STEREO_WEIGHT,u2ReadDspShmWORD (u1DspId, W_A2_STEREO_WEIGHT));
        vWriteDspWORD (u1DspId, ADDR_A2_DUAL_WEIGHT,u2ReadDspShmWORD (u1DspId, W_A2_DUAL_WEIGHT));
        vWriteDspWORD (u1DspId, ADDR_HDEV_MODE, (uReadDspShmBYTE (u1DspId, B_HDEV_MODE) & 0x02)>>1);
        vWriteDspWORD (u1DspId, ADDR_CARRIER_SHIFT_MODE, (uReadDspShmBYTE (u1DspId, B_CARRIER_SHIFT_MODE) & 0x02)>>1);
        vWriteDspWORD (u1DspId, ADDR_FM_SATU_MUTE_MODE,  (uReadDspShmBYTE (u1DspId, B_FM_SATU_MUTE_MODE)& 0x02)>>1);
        vWriteDspWORD (u1DspId, ADDR_FM_CARRIER_MUTE_MODE,  (uReadDspShmBYTE (u1DspId, B_FM_MUTE_MODE) & 0x02)>>1);
        vWriteDspWORD (u1DspId, ADDR_FM_CARRIER_MUTE_THRESHOLD_HIGHT,  uReadDspShmBYTE (u1DspId, B_FM_MUTE_THRESHOLD_HIGHT_A2));
        vWriteDspWORD (u1DspId, ADDR_FM_CARRIER_MUTE_THRESHOLD_LOW,  uReadDspShmBYTE (u1DspId, B_FM_MUTE_THRESHOLD_LOW_A2));
        vWriteDspWORD (u1DspId, ADDR_DEM_PAL_FINE_TUNE_VOLUME,u4ReadDspShmDWRD  (u1DspId, D_A2_FINE_TUNE_VOLUME)>>8);
        vWriteDspWORD (u1DspId, ADDR_MANUAL_MTS_SW, uReadDspShmBYTE (u1DspId, B_MANUAL_MTS_SW));
        //vWriteDspWORD (ADDR_MANUAL_OUTPUT_MATRIX, uReadDspShmBYTE (B_MANUAL_OUTPUT_MATRIX));
        vWriteDspWORD (u1DspId, ADDR_FM_NON_EU_MODE,uReadDspShmBYTE  (u1DspId, B_NON_EU_FM_MODE));
        // Decoder 3
        vWriteDspWORD (u1DspId, ADDR_DEM_SOURCE_MODE_DEC3, u2ChannelSrcSet);
        vWriteDspWORD (u1DspId, ADDR_DEM_USER_MODE_DEC3, u2ReadDspShmWORD (u1DspId, W_USER_MODE));
        vWriteDspWORD (u1DspId, ADDR_DEM_ADC_GAIN_DEC3,u2ReadDspShmWORD (u1DspId, W_ADC_GAIN));
        vWriteDspWORD (u1DspId, ADDR_DEM_PAL_FINE_TUNE_VOLUME_DEC3,u4ReadDspShmDWRD  (u1DspId, D_A2_FINE_TUNE_VOLUME)>>8);
        if (u2ChannelSrcSet == SOURCE_NTSC_M)
        {
            vWriteDspWORD (u1DspId, ADDR_A2_STEREO_MONO_THRES, u2ReadDspShmWORD(u1DspId, W_A2_STEREO_MONO_THRES_M));
            vWriteDspWORD (u1DspId, ADDR_A2_MONO_STEREO_THRES, u2ReadDspShmWORD(u1DspId, W_A2_MONO_STEREO_THRES_M));
        }
        else
        {
            vWriteDspWORD (u1DspId, ADDR_A2_STEREO_MONO_THRES, u2ReadDspShmWORD(u1DspId, W_A2_STEREO_MONO_THRES_EU));
            vWriteDspWORD (u1DspId, ADDR_A2_MONO_STEREO_THRES, u2ReadDspShmWORD(u1DspId, W_A2_MONO_STEREO_THRES_EU));
        }
	      vWriteDspWORD (u1DspId, ADDR_A2_WEAK_RF_LVL_S2M_THRES, u2ReadDspShmWORD(u1DspId, W_A2_WEAK_RF_LVL_S2M_THRES_EU));
        vWriteDspWORD (u1DspId, ADDR_A2_WEAK_RF_LVL_M2S_THRES, u2ReadDspShmWORD(u1DspId, W_A2_WEAK_RF_LVL_M2S_THRES_EU));
        break;
    case PAL_STREAM:
        vWriteDspWORD (u1DspId, ADDR_DEM_ADC_GAIN,u2ReadDspShmWORD (u1DspId, W_ADC_GAIN));
        vWriteDspWORD (u1DspId, ADDR_DEM_USER_MODE,u2ReadDspShmWORD (u1DspId, W_USER_MODE));
        vWriteDspWORD (u1DspId, ADDR_DEM_ENFORCED_MODE,u2ReadDspShmWORD (u1DspId, W_ENFORCED_MODE));
        u1ChannelSrc = DSP_ReturnChannelSource ();
        u2ChannelSrcSet = SOURCE_PAL_I; // default value
        if ((u1ChannelSrc == SV_PAL_BG) || (u1ChannelSrc == SV_A2_BG))
        {
            u2ChannelSrcSet = SOURCE_PAL_BG;
        }
        else if ((u1ChannelSrc == SV_PAL_DK) ||
                (u1ChannelSrc == SV_A2_DK1) ||
                (u1ChannelSrc == SV_A2_DK2) ||
                (u1ChannelSrc == SV_A2_DK3))
        {
            u2ChannelSrcSet = SOURCE_PAL_DK;
        }
        else if (u1ChannelSrc == SV_SECAM_L)
        {
            u2ChannelSrcSet = SOURCE_SECAM_L;
        }
        else if (u1ChannelSrc == SV_SECAM_BG)
        {
            u2ChannelSrcSet = SOURCE_SECAM_BG;
        }
        else if (u1ChannelSrc == SV_SECAM_DK)
        {
            u2ChannelSrcSet = SOURCE_SECAM_DK;
        }
        else if (u1ChannelSrc == SV_SECAM_L_PLUM)
        {
            u2ChannelSrcSet = SOURCE_SECAM_L;
        }
        vWriteDspWORD (u1DspId, ADDR_DEM_SOURCE_MODE,u2ChannelSrcSet);
        vWriteDspWORD (u1DspId, ADDR_PAL_CORRECT_THRESHOLD,u2ReadDspShmWORD (u1DspId, W_PAL_CORRECT_THRESHOLD));
        vWriteDspWORD (u1DspId, ADDR_PAL_TOTAL_SYNC1_LOOP,u2ReadDspShmWORD (u1DspId, W_PAL_TOTAL_SYNC1_LOOP));
        vWriteDspWORD (u1DspId, ADDR_PAL_ERR_THRESHOLD,u2ReadDspShmWORD (u1DspId, W_PAL_ERR_THRESHOLD));
        vWriteDspWORD (u1DspId, ADDR_PAL_PARITY_ERR_THRESHOLD,u2ReadDspShmWORD (u1DspId, W_PAL_PARITY_ERR_THRESHOLD));
        vWriteDspWORD (u1DspId, ADDR_PAL_EVERY_NUM_FRAMES,u2ReadDspShmWORD (u1DspId, W_PAL_EVERY_NUM_FRAMES));
#ifdef ATV_HDEV_AUTO_SWITCH
        vWriteDspWORD (u1DspId, ADDR_HDEV_MODE, ((uReadDspShmBYTE (u1DspId, B_HDEV_MODE) & 0x01)| 0x03));
#else
#ifdef CC_AUD_PAL_NEW_HDEV_SUPPORT
        vWriteDspWORD (u1DspId, ADDR_HDEV_MODE, ((uReadDspShmBYTE (u1DspId, B_HDEV_MODE) & 0x01) << 2) | 0x1);  // set bit 0 and 2
#else
#ifdef CC_AUD_DDI
		    vWriteDspWORD (u1DspId, ADDR_HDEV_MODE, ((uReadDspShmBYTE (u1DspId, B_HDEV_MODE) & 0x01) << 2) );  // set bit 2
#else
        vWriteDspWORD (u1DspId, ADDR_HDEV_MODE, (uReadDspShmBYTE (u1DspId, B_HDEV_MODE) & 0x01));
#endif
#endif
#endif
        vWriteDspWORD (u1DspId, ADDR_CARRIER_SHIFT_MODE, (uReadDspShmBYTE (u1DspId, B_CARRIER_SHIFT_MODE) & 0x01));
        vWriteDspWORD (u1DspId, ADDR_FM_SATU_MUTE_MODE,  (uReadDspShmBYTE (u1DspId, B_FM_SATU_MUTE_MODE) & 0x01));
        //for AM carrier sense mute
        vWriteDspWORD (u1DspId, ADDR_SECAM_L_MUTE_MODE,  uReadDspShmBYTE (u1DspId, B_AM_MUTE_MODE));
        vWriteDspWORD (u1DspId, ADDR_SECAM_L_MUTE_THRESHOLD_HIGHT, uReadDspShmBYTE (u1DspId, B_AM_MUTE_THRESHOLD_HIGHT));
        vWriteDspWORD (u1DspId, ADDR_SECAM_L_MUTE_THRESHOLD_LOW,uReadDspShmBYTE (u1DspId, B_AM_MUTE_THRESHOLD_LOW));
        //for FM carrier sense mute
        vWriteDspWORD (u1DspId, ADDR_FM_CARRIER_MUTE_MODE,  (uReadDspShmBYTE (u1DspId, B_FM_MUTE_MODE) & 0x01));
        vWriteDspWORD (u1DspId, ADDR_FM_CARRIER_MUTE_THRESHOLD_HIGHT, uReadDspShmBYTE (u1DspId, B_FM_MUTE_THRESHOLD_HIGHT_PAL));
        vWriteDspWORD (u1DspId, ADDR_FM_CARRIER_MUTE_THRESHOLD_LOW,uReadDspShmBYTE (u1DspId, B_FM_MUTE_THRESHOLD_LOW_PAL));
        //for EU/NON-EU FM
        vWriteDspWORD (u1DspId, ADDR_FM_NON_EU_MODE,uReadDspShmBYTE  (u1DspId, B_NON_EU_FM_MODE));
        //for NICAM I boost or not
        vWriteDspWORD(u1DspId, ADDR_NICAM_I_BOOST_FLAG,uReadDspShmBYTE(u1DspId, B_NICAM_I_BOOST_FLAG));
         //for PAL FM BAD SIGNAL CHECK
        //vWriteDspWORD (ADDR_PAL_HDEV_BAD_SIGNAL_CHECK,uReadDspShmBYTE (B_FM_BAD_SIGNAL));
        vWriteDspWORD (u1DspId, ADDR_DEM_PAL_FINE_TUNE_VOLUME,u4ReadDspShmDWRD  (u1DspId, D_PAL_FINE_TUNE_VOLUME)>>8);
        vWriteDspWORD (u1DspId, ADDR_DEM_NICAM_FINE_TUNE_VOLUME,u4ReadDspShmDWRD (u1DspId, D_NICAM_FINE_TUNE_VOLUME)>>8);
        vWriteDspWORD (u1DspId, ADDR_DEM_AM_FINE_TUNE_VOLUME,u4ReadDspShmDWRD  (u1DspId, D_AM_FINE_TUNE_VOLUME)>>8);

        // Decoder 3
        vWriteDspWORD (u1DspId, ADDR_DEM_SOURCE_MODE_DEC3,u2ChannelSrcSet);
        vWriteDspWORD (u1DspId, ADDR_DEM_USER_MODE_DEC3,u2ReadDspShmWORD (u1DspId, W_USER_MODE));
        vWriteDspWORD (u1DspId, ADDR_DEM_ADC_GAIN_DEC3,u2ReadDspShmWORD (u1DspId, W_ADC_GAIN));
        vWriteDspWORD (u1DspId, ADDR_DEM_ENFORCED_MODE_DEC3,u2ReadDspShmWORD (u1DspId, W_ENFORCED_MODE));

        vWriteDspWORD (u1DspId, ADDR_DEM_PAL_FINE_TUNE_VOLUME_DEC3,u4ReadDspShmDWRD  (u1DspId, D_PAL_FINE_TUNE_VOLUME)>>8);
        vWriteDspWORD (u1DspId, ADDR_DEM_NICAM_FINE_TUNE_VOLUME_DEC3,u4ReadDspShmDWRD (u1DspId, D_NICAM_FINE_TUNE_VOLUME)>>8);
        vWriteDspWORD (u1DspId, ADDR_DEM_AM_FINE_TUNE_VOLUME_DEC3,u4ReadDspShmDWRD  (u1DspId, D_AM_FINE_TUNE_VOLUME)>>8);

        vWriteDspWORD (u1DspId, ADDR_SECAM_L_MUTE_MODE_DEC3,  uReadDspShmBYTE (u1DspId, B_AM_MUTE_MODE));
        vWriteDspWORD (u1DspId, ADDR_SECAM_L_MUTE_THRESHOLD_HIGHT_DEC3, uReadDspShmBYTE (u1DspId, B_AM_MUTE_THRESHOLD_HIGHT));
        vWriteDspWORD (u1DspId, ADDR_SECAM_L_MUTE_THRESHOLD_LOW_DEC3,uReadDspShmBYTE (u1DspId, B_AM_MUTE_THRESHOLD_LOW));
        break;

    case DETECTOR_STREAM:
        vWriteDspWORD (u1DspId, ADDR_DETECTOR_DEFAULT_MODE_DEC3, uReadDspShmBYTE(u1DspId, B_DETECT_DEFAULT));
        vWriteDspWORD (u1DspId, ADDR_DETECTOR_START_FLAG_DEC3, 1); // default turn on detection
        WriteDspCommDram (u1DspId, ADDR_DETECTOR_FM_NOISE_TH_DEC3, u4ReadDspShmDWRD (u1DspId, D_DETECTOR_FM_NOISE_TH));
        vWriteDspWORD (u1DspId, ADDR_DETECTOR_A2_PILOT_CHK_DEC3, uReadDspShmBYTE(u1DspId, B_DETECTOR_A2_PILOT_CHK));
        break;
#endif

    case AAC_STREAM:
//#ifndef CC_AUD_SUPPORT_MS10
#ifndef CC_AUD_DOLBY_SUPPORT_DDT

        vWriteDspWORD (u1DspId, ADDR_RC2D_AAC_DOWNMIX_LEVEL_DEC3,
                       AAC_DEFAULT_DOWNMIX_LEVEL);
        vWriteDspWORD (u1DspId, ADDR_RC2D_AAC_TARGET_LEVEL_DEC3,
                       AAC_TARGET_LEVEL);
#else
        vWriteDspWORD (u1DspId, ADDR_RC2D_DDT_DUAL_MODE_DEC3,
                       uReadDspShmBYTE (u1DspId, B_DDT_DUALMODE_DEC3));
        vWriteDspWORD (u1DspId, ADDR_RC2D_DDT_DRC_DEC3,
                       uReadDspShmBYTE (u1DspId, B_DDT_COMPVAL_DEC3));
        vWriteDspWORD (u1DspId, ADDR_RC2D_DDT_DMX_MODE_DEC3,
                       uReadDspShmBYTE (u1DspId, B_DDT_COMPMODE_DEC3));
#endif
        vWriteDspWORD (u1DspId, ADDR_RC2D_DDT_CONTROL_DEC3,
                       uReadDspShmBYTE(u1DspId, B_DDT_CONTROL));

        //PTS Table Shift Amount
#if defined(CC_AUD_ARM_SUPPORT) && defined(CC_AUD_ARM_RENDER)
        if (uReadDspShmBYTE(u1DspId, B_DDT_CONTROL)==0) //EU Mode
        {
            u4Temp3 = dReadDspCommDram(u1DspId, ADDR_RC2D_PTS_STC_CTRL_FLAG);
            u4Temp3 |= 0x10000; //Set bit 8 (align @ bit8)
            WriteDspCommDram (u1DspId, ADDR_RC2D_PTS_STC_CTRL_FLAG, u4Temp3);  
        }
#endif
        break;
    case DTSDEC_STREAM:
        if (AUD_GetDTSInfo())
        {
        	//Big Endian
            vWriteDspWORD (u1DspId, ADDR_RC2D_DTS_DRC_FLAG_DEC3, 0xffd9);
        }
        else
        {
        	//Little Endian
        	vWriteDspWORD (u1DspId, ADDR_RC2D_DTS_DRC_FLAG_DEC3, 0xfffb);
        }
        AUD_SetDTSInfo(DEC_BIG_ENDIAN); //reset to Big Endian
        vWriteDspWORD (u1DspId, ADDR_RC2D_DTS_LFE_mixto_FRONT_DEC3, 0);
        vWriteDspWORD (u1DspId, ADDR_RC2D_DTS_DRC_ONOFF_DEC3,       9);
        vWriteDspWORD (u1DspId, ADDR_RC2D_DTS_DIALNORM_ONOFF_DEC3,  9);
        WriteDspCommDram (u1DspId, ADDR_RC2D_DTS_DRC_LOW_DEC3,      u4ReadDspShmDWRD (u1DspId, D_DTS_BC_DRC_LOW));
        WriteDspCommDram (u1DspId, ADDR_RC2D_DTS_DRC_MEDIUM_DEC3,   u4ReadDspShmDWRD (u1DspId, D_DTS_BC_DRC_MEDIUM));
        WriteDspCommDram (u1DspId, ADDR_RC2D_DTS_DRC_HIGH_DEC3,     u4ReadDspShmDWRD (u1DspId, D_DTS_BC_DRC_HIGH));
        WriteDspCommDram (u1DspId, ADDR_RC2D_DTS_DIALNORM_LOW_DEC3, u4ReadDspShmDWRD (u1DspId, D_DTS_BC_Dialnorm_LOW));
        WriteDspCommDram (u1DspId, ADDR_RC2D_DTS_DIALNORM_MEDIUM_DEC3, u4ReadDspShmDWRD (u1DspId, D_DTS_BC_Dialnorm_MEDIUM));
        WriteDspCommDram (u1DspId, ADDR_RC2D_DTS_DIALNORM_HIGH_DEC3, u4ReadDspShmDWRD (u1DspId, D_DTS_BC_Dialnorm_HIGH));
        break;
    case MPEG3_STREAM:
        // Normal play
        vWriteDspWORD (u1DspId, ADDR_RC2D_MP3_NORMAL_PLAY_DEC3, 0x00);
        // MP3 sync mode
        vWriteDspWORD (u1DspId, ADDR_RC2D_MP3_SYNC_MODE_DEC3, 0x01);
        // MP3 deemphasis mode
        vWriteDspWORD (u1DspId, ADDR_RC2D_MP3_DEEMPHASIS_MODE_DEC3, 0x00);
        // MP3 preparsing mode
        vWriteDspWORD (u1DspId, ADDR_RC2D_MP3_PREPARSING_MODE_DEC3, 0x00);
        break;
#if 1///def CC_VORBIS_SUPPORT
    case VORBIS_STREAM:
        vWriteDspWORD(u1DspId, ADDR_RC2D_VORBIS_PACKET_NO, u2ReadDspShmWORD(u1DspId, W_VORBISPACKET));
        vWriteDspWORD(u1DspId, ADDR_D2RC_VORBIS_TABLE_OK_DEC3, 0);
        vWriteDspWORD(u1DspId, ADDR_D2RC_VORBIS_ABS_GRANULE_POS_LL16_DEC3, 0);
        vWriteDspWORD(u1DspId, ADDR_D2RC_VORBIS_ABS_GRANULE_POS_LH16_DEC3, 0);
        vWriteDspWORD(u1DspId, ADDR_D2RC_VORBIS_ABS_GRANULE_POS_OK_DEC3, 0);
        /// Decode Dram is big enough
        /// DSP_DAT_DEC3_SZ (0x12A00*4) = 0x12A00
        /// Vorbis
        /// OUTPUT_BUF_ADR (016B00h-08000h)/100h = 0xEB00
        /// Aout Buffer = 0x12A00 - 0xEB00 = 0x3F00
        /// Dec3 Each Banck Compact Size = 256*3/4*2 = 0x180
        /// Aout Maximum Buffer Bank Size = 0x3F00/0x180 = 0x2A(42) > (8) and (18)
        /// it is big enough, no need to limit it when play vorbis.
        /// Dec3: 8.
        ///vWriteDspWORD (ADDR_RC2D_AOUT_BANK_256_NUM_DEC3, 8);   ///[Joel] Vorbis decoder dram only support 8+30 Bank
        break;
#endif
	case COOK_STREAM:
#if 0
		vWriteDspWORD (ADDR_RC2D_COOK_SAMP_PER_FRAME_DEC3,0x400);
		vWriteDspWORD (ADDR_RC2D_COOK_CHANNEL_NUM_DEC3,0x2);
		vWriteDspWORD (ADDR_RC2D_COOK_INPUT_SAMPLING_RATE_DEC3,0xac44);
		vWriteDspWORD (ADDR_RC2D_COOK_FRM_SZ_IN_BYTES_DEC3,0x80);
		vWriteDspWORD (ADDR_RC2D_COOK_REGION_NUM_DEC3,0x25);
		vWriteDspWORD (ADDR_RC2D_COOK_COUPLING_START_REGN_DEC3,0x2);
		vWriteDspWORD (ADDR_RC2D_COOK_COUPLING_QBITS_DEC3,0x4);
		LOG(0," ===========>COOK DEC3 Initial \n");
#else
  		vWriteDspWORD (u1DspId, ADDR_RC2D_COOK_SAMP_PER_FRAME_DEC3,
	  		u2ReadDspShmWORD(u1DspId, W_COOK_SAMP_PER_FRAME));
  		vWriteDspWORD (u1DspId, ADDR_RC2D_COOK_CHANNEL_NUM_DEC3,
	  		u2ReadDspShmWORD(u1DspId, W_COOK_CHANNEL_NUM));
  		vWriteDspWORD (u1DspId, ADDR_RC2D_COOK_INPUT_SAMPLING_RATE_DEC3,
	  		u2ReadDspShmWORD(u1DspId, W_COOK_INPUT_SAMPLING_RATE));
  		vWriteDspWORD (u1DspId, ADDR_RC2D_COOK_FRM_SZ_IN_BYTES_DEC3,
	  		u2ReadDspShmWORD(u1DspId, W_COOK_FRM_SZ_IN_BYTES));
  		vWriteDspWORD (u1DspId, ADDR_RC2D_COOK_REGION_NUM_DEC3,
	  		u2ReadDspShmWORD(u1DspId, W_COOK_REGION_NUM));
  		vWriteDspWORD (u1DspId, ADDR_RC2D_COOK_COUPLING_START_REGN_DEC3,
	  		u2ReadDspShmWORD(u1DspId, W_COOK_COUPLING_START_REGN));
  		vWriteDspWORD (u1DspId, ADDR_RC2D_COOK_COUPLING_QBITS_DEC3,
	  		u2ReadDspShmWORD(u1DspId, W_COOK_COUPLING_QBITS));
#endif
  		break;
    case LPCM_STREAM:
        u1Temp1 = (u2ReadDspShmWORD(u1DspId, W_LPCM_FREQUENCY_DEC3)>>8) & 0xFF;
        u1Temp2 = u2ReadDspShmWORD(u1DspId, W_LPCM_FREQUENCY_DEC3) & 0xFF;

        vWriteDspWORD(u1DspId, ADDR_RC2D_LPCM_CHANNEL_ASSIGNMENT_DEC3, uReadDspShmBYTE(u1DspId, B_LPCM_CH_ASSIGNMENT_DEC3));
        WriteDspCommDram(u1DspId, ADDR_RC2D_LPCM_FREQUENCY_DEC3,  (u1Temp1<<12)+u1Temp2);
        vWriteDspWORD(u1DspId, ADDR_RC2D_LPCM_BIT_SHIFT_DEC3,  uReadDspShmBYTE(u1DspId, B_LPCM_BIT_SHIFT_DEC3));
        vWriteDspWORD(u1DspId, ADDR_RC2D_LPCM_DRC_VALUE_DEC3,  uReadDspShmBYTE(u1DspId, B_LPCM_DRC_VALUE_DEC3));
        WriteDspCommDram(u1DspId, ADDR_RC2D_LPCM_Q_DEC3,  u2ReadDspShmWORD(u1DspId, W_LPCM_Q_VALUE_DEC3));
        vWriteDspWORD(u1DspId, ADDR_RC2D_LPCM_DRC_FLAG_DEC3,  uReadDspShmBYTE(u1DspId, B_LPCM_DRC_FLAG_DEC3));
        vWriteDspWORD(u1DspId, ADDR_RC2D_LPCM_DECODING_TYPE_DEC3,  uReadDspShmBYTE(u1DspId, W_LPCM_DECODING_TYPE_DEC3));

        if (uReadDspShmBYTE(u1DspId, W_ADPCM_DECODING_TYPE_DEC3)!=0) //ADPCM
        {
            vWriteDspWORD(u1DspId, ADDR_RC2D_LPCM_ADPCM_DEC_TYPE_DEC3, uReadDspShmBYTE(u1DspId, W_ADPCM_DECODING_TYPE_DEC3));
            vWriteDspWORD(u1DspId, ADDR_RC2D_LPCM_ADPCM_BITS_PER_SAMPLE_DEC3, uReadDspShmBYTE(u1DspId, B_ADPCM_BIT_PERSAMPLE_DEC3));
            vWriteDspWORD(u1DspId, ADDR_RC2D_ADPCM_BLOCK_ALIGN_DEC3, u2ReadDspShmWORD(u1DspId, W_ADPCM_BLOCK_ALIGN_DEC3));
            vWriteDspWORD(u1DspId, ADDR_RC2D_ADPCM_CHANNEL_NUM_DEC3, uReadDspShmBYTE(u1DspId, B_ADPCM_CHANNEL_NUM_DEC3));
        }
        break;

#ifdef DATA_DISC_WMA_SUPPORT
    case WMA_STREAM:
        //WNA Packet No
        vWriteDspWORD (u1DspId, ADDR_RC2D_WMA_PACKET_NO_DEC3,
                       u2ReadDspShmWORD (u1DspId, W_WMA_PACKET_NO_DEC3));
        vWriteDspWORD (u1DspId, ADDR_RC2D_WMA_HEADER_SOURCE_DEC3,
                       u2ReadDspShmWORD (u1DspId, W_WMA_HEADER_SOURCE_DEC3));
        WriteDspCommDram (u1DspId, ADDR_RC2D_WMA_PACKET_COUNT_DEC3,
                       ((u4ReadDspShmDWRD (u1DspId, D_WMA_PACKET_COUNT_DEC3))<<8));
        WriteDspCommDram (u1DspId, ADDR_RC2D_WMA_PACKET_SIZE_DEC3,
                       ((u4ReadDspShmDWRD (u1DspId, D_WMA_PACKET_SIZE_DEC3))<<8));
        vWriteDspWORD (u1DspId, ADDR_RC2D_WMA_VERSION_DEC3,
                       u2ReadDspShmWORD (u1DspId, W_WMA_VERSION_DEC3));
        vWriteDspWORD (u1DspId, ADDR_RC2D_WMA_NUM_CHANNELS_DEC3,
                       u2ReadDspShmWORD (u1DspId, W_WMA_NUM_CH_DEC3));
        WriteDspCommDram (u1DspId, ADDR_RC2D_WMA_SAMPLE_PER_SECOND_DEC3,
                       ((u4ReadDspShmDWRD (u1DspId, D_WMA_SAMPLE_PER_SEC_DEC3))<<8));
        WriteDspCommDram (u1DspId, ADDR_RC2D_WMA_BYTES_PER_SECOND_DEC3,
                       ((u4ReadDspShmDWRD (u1DspId, D_WMA_BYTE_PER_SEC_DEC3))<<8));
        WriteDspCommDram (u1DspId, ADDR_RC2D_WMA_BLOCK_SIZE_DEC3,
                       ((u4ReadDspShmDWRD (u1DspId, D_WMA_BLOCK_SIZE_DEC3))<<8));
        vWriteDspWORD (u1DspId, ADDR_RC2D_WMA_ENCODER_OPTIONS_DEC3,
                       u2ReadDspShmWORD (u1DspId, W_WMA_ENCODER_OPTION_DEC3));
        //koro : if issue play without setting audio format (seek mode), wma need initial table
        vLoadDspDRAMTable (u1DspId, AUD_WMA_DEC3);
        vWriteDspWORD (u1DspId, ADDR_RC2D_DOWNLOAD_TABLE_FLAG_DEC3, 0x1);

        break;
#endif

    case WMAPRO_STREAM:

        vWriteDspWORD (u1DspId, ADDR_RC2D_CTRL_6_NUM_1, (UINT16)0xC);
        //WNA Packet No
        vWriteDspWORD (u1DspId, ADDR_RC2D_WMAPRO_PACKET_NO_DEC3,
                       u2ReadDspShmWORD (u1DspId, W_WMAPRO_PACKET_NO_DEC3));
        vWriteDspWORD (u1DspId, ADDR_RC2D_WMAPRO_HEADER_SOURCE_DEC3,
                       u2ReadDspShmWORD (u1DspId, W_WMAPRO_HEADER_SOURCE_DEC3));
        vWriteDspWORD (u1DspId, ADDR_RC2D_WMAPRO_NUM_CHANNELS_DEC3,
                       u2ReadDspShmWORD (u1DspId, W_WMAPRO_NUM_CH_DEC3));
        WriteDspCommDram (u1DspId, ADDR_RC2D_WMAPRO_SAMPLE_PER_SECOND_DEC3,
                       ((u4ReadDspShmDWRD (u1DspId, D_WMAPRO_SAMPLE_PER_SEC_DEC3))<<8));
        WriteDspCommDram (u1DspId, ADDR_RC2D_WMAPRO_BYTES_PER_SECOND_DEC3,
                       ((u4ReadDspShmDWRD (u1DspId, D_WMAPRO_BYTE_PER_SEC_DEC3))<<8));
        WriteDspCommDram (u1DspId, ADDR_RC2D_WMAPRO_BLOCK_SIZE_DEC3,
                       ((u4ReadDspShmDWRD (u1DspId, D_WMAPRO_BLOCK_SIZE_DEC3))<<8));
        vWriteDspWORD (u1DspId, ADDR_RC2D_WMAPRO_ENCODER_OPTIONS_DEC3,
                       u2ReadDspShmWORD (u1DspId, W_WMAPRO_ENCODER_OPTION_DEC3));
        WriteDspCommDram (u1DspId, ADDR_RC2D_WMAPRO_CH_MASK_DEC3,
                       ((u4ReadDspShmDWRD (u1DspId, D_WMAPRO_CH_MASK_DEC3))<<8));
        vWriteDspWORD (u1DspId, ADDR_RC2D_WMAPRO_VALID_BITS_DEC3,
                       u2ReadDspShmWORD (u1DspId, W_WMAPRO_VALID_BITS_DEC3));
        vWriteDspWORD (u1DspId, ADDR_RC2D_WMAPRO_ADV_OPTION_DEC3,
                       u2ReadDspShmWORD (u1DspId, W_WMAPRO_ADV_OPTION_DEC3));
        WriteDspCommDram (u1DspId, ADDR_RC2D_WMAPRO_ADV_OPTION2_DEC3,
                       ((u4ReadDspShmDWRD (u1DspId, D_WMAPRO_ADV_OPTION2_DEC3))<<8));

        break;

    case FLAC_STREAM://paul_flac_3
    #if 0
        WriteDspCommDram(ADDR_RC2D_FLAC_MIN_BLK_SZ_DEC3, 0x1000);
		WriteDspCommDram(ADDR_RC2D_FLAC_MAX_BLK_SZ_DEC3, 0x1000);
		WriteDspCommDram(ADDR_RC2D_FLAC_CH_NUM_1_DEC3, 1);
		WriteDspCommDram(ADDR_RC2D_FLAC_BPS_1_DEC3, 15);
		WriteDspCommDram(ADDR_RC2D_FLAC_MIN_FRM_SZ_DEC3, 0x398);
		WriteDspCommDram(ADDR_RC2D_FLAC_MAX_FRM_SZ_DEC3, 0x12a0);
		WriteDspCommDram(ADDR_RC2D_FLAC_SAMP_RATE_DEC3, 0xBB80);
    #else
        WriteDspCommDram(u1DspId, ADDR_RC2D_FLAC_MIN_BLK_SZ_DEC3, u2ReadDspShmWORD(u1DspId, W_FLAC_MIN_BLK_SZ));
		WriteDspCommDram(u1DspId, ADDR_RC2D_FLAC_MAX_BLK_SZ_DEC3, u2ReadDspShmWORD(u1DspId, W_FLAC_MAX_BLK_SZ));
		WriteDspCommDram(u1DspId, ADDR_RC2D_FLAC_CH_NUM_1_DEC3, u2ReadDspShmWORD(u1DspId, W_FLAC_CHN_NUM));
		WriteDspCommDram(u1DspId, ADDR_RC2D_FLAC_BPS_1_DEC3, u2ReadDspShmWORD(u1DspId, W_FLAC_BPS));
		WriteDspCommDram(u1DspId, ADDR_RC2D_FLAC_MIN_FRM_SZ_DEC3, u4ReadDspShmDWRD(u1DspId, D_FLAC_MIN_FRM_SZ));
		WriteDspCommDram(u1DspId, ADDR_RC2D_FLAC_MAX_FRM_SZ_DEC3, u4ReadDspShmDWRD(u1DspId, D_FLAC_MAX_FRM_SZ));
		WriteDspCommDram(u1DspId, ADDR_RC2D_FLAC_SAMP_RATE_DEC3, u4ReadDspShmDWRD(u1DspId, D_FLAC_SMP_RATE));
    #endif
        break;
    case G726_STREAM:
		WriteDspCommDram(u1DspId, ADDR_RC2D_G726_FRAME_SZ_DEC3, u2ReadDspShmWORD(u1DspId, W_G726_FRAME_DEC3)<<8);
		WriteDspCommDram(u1DspId, ADDR_RC2D_G726_CH_NUM_DEC3, uReadDspShmBYTE(u1DspId, B_G726_CH_DEC3)<<8);
		WriteDspCommDram(u1DspId, ADDR_RC2D_G726_RATE_DEC3, uReadDspShmBYTE(u1DspId, B_G726_RATE_DEC3)<<8);
		WriteDspCommDram(u1DspId, ADDR_RC2D_G726_LAW_DEC3, uReadDspShmBYTE(u1DspId, B_G726_LAW_DEC3)<<8);
        WriteDspCommDram(u1DspId, ADDR_RC2D_G726_SRATE_DEC3, uReadDspShmBYTE(u1DspId, B_G726_SRATE_DEC3)<<8);
        break;
    default:
        break;
    }
}
#endif

#ifdef CC_AUD_4_DECODER_SUPPORT
/***************************************************************************
     Function : vDecCommonInitDec4 (for 4th decoder)
  Description : This function is used to init the decoder EACH time when it
                is played. In other words, it is to set the properties which
                is different song by song. Note currently we only allow PCM and MP2 played on dec3
    Parameter : None
    Return    : None
***************************************************************************/
void vDecCommonInitDec4 (UINT8 u1DspId)
{
    // Bonding options
    vWriteDspWORD (u1DspId, ADDR_RC2D_BONDING_OPTIONS, u2ReadDspShmWORD (u1DspId, W_BONDING_OPTIONS));

    switch (_rDspVars[u1DspId][AUD_DEC_4TH].bDspStrTyp)
    {
    case SBCE_STREAM:
        //Printf("vDecCommonInitDec4 => SBC_STREAM\n"); //SBCENC_TEST
        vWriteDspWORD (u1DspId, ADDR_RC2D_SBCE_SAMPLE_FREQ_DEC4, uReadDspShmBYTE(u1DspId, B_SBCENC_SAMPLE_FREQ));
        vWriteDspWORD (u1DspId, ADDR_RC2D_SBCE_BLK_NUM_1_DEC4, uReadDspShmBYTE(u1DspId, B_SBCENC_BLK_NUM_1));
        vWriteDspWORD (u1DspId, ADDR_RC2D_SBCE_CH_NUM_1_DEC4, uReadDspShmBYTE(u1DspId, B_SBCENC_CH_NUM_1));
        vWriteDspWORD (u1DspId, ADDR_RC2D_SBCE_BITPOOL_DEC4, uReadDspShmBYTE(u1DspId, B_SBCENC_BITPOOL));
        vWriteDspWORD (u1DspId, ADDR_RC2D_SBCE_SAMPLE_NUM_DEC4, ((UINT32)uReadDspShmBYTE(u1DspId, B_SBCENC_BLK_NUM_1)+1)*8);
        WriteDspCommDram (u1DspId, ADDR_RC2D_SBCE_FRAME_SIZE_DEC4, ((UINT32)uReadDspShmBYTE(u1DspId, B_SBCENC_BLK_NUM_1)+1)*8*3);
        vWriteDspWORD (u1DspId, ADDR_RC2D_SBCE_HDR_0_DEC4,
                        ((uReadDspShmBYTE(u1DspId, B_SBCENC_SAMPLE_FREQ)<<6) |
                        ((uReadDspShmBYTE(u1DspId, B_SBCENC_BLK_NUM_1)>>2)<<4) |
                        (uReadDspShmBYTE(u1DspId, B_SBCENC_CH_NUM_1)<<3) | 0x01));
        {
            //SBCENC_TEST
            //Borrow the internal_buf4 and upload buffer
            UINT32 u4StartBlock,u4EndBlock;
            u4StartBlock = ((_u4SBCEncBuf[0]>>8)&0xffff);
            u4EndBlock = ((_u4SBCEncBuf[1]>>8)&0xffff);
            AUD_WRITE32(REG_PUTBS_BLK0_RISC,(u4StartBlock<<16) | u4EndBlock);
            AUD_WRITE32(REG_PUTBS_BLK1_RISC,(u4StartBlock<<16) | u4EndBlock);
            vWriteDspWORD(u1DspId, ADDR_RC2D_SBCE_WRITE_PTR_DEC4,u4StartBlock);
        }
        Printf("!!!!!!!!!!!!!!! SBCE_STREAM !!!!!!!!!!!!!\n");
        Printf("  ADDR_RC2D_SBCE_SAMPLE_FREQ_DEC4: 0x%08x\n", dReadDspCommDram(u1DspId, ADDR_RC2D_SBCE_SAMPLE_FREQ_DEC4));
        Printf("  ADDR_RC2D_SBCE_BLK_NUM_1_DEC4: 0x%08x\n", dReadDspCommDram(u1DspId, ADDR_RC2D_SBCE_BLK_NUM_1_DEC4));
        Printf("  ADDR_RC2D_SBCE_CH_NUM_1_DEC4: 0x%08x\n", dReadDspCommDram(u1DspId, ADDR_RC2D_SBCE_CH_NUM_1_DEC4));
        Printf("  ADDR_RC2D_SBCE_BITPOOL_DEC4: 0x%08x\n", dReadDspCommDram(u1DspId, ADDR_RC2D_SBCE_BITPOOL_DEC4));
        Printf("  ADDR_RC2D_SBCE_SAMPLE_NUM_DEC4: 0x%08x\n", dReadDspCommDram(u1DspId, ADDR_RC2D_SBCE_SAMPLE_NUM_DEC4));
        Printf("  ADDR_RC2D_SBCE_FRAME_SIZE_DEC4: 0x%08x\n", dReadDspCommDram(u1DspId, ADDR_RC2D_SBCE_FRAME_SIZE_DEC4));
        break;
    case PCM_STREAM:
        vWriteDspWORD (u1DspId, ADDR_RC2D_PCM_INPUT_CHANNEL_NUM_DEC4,
                       u2ReadDspShmWORD (u1DspId, W_PCM_INPUT_CHANNEL_NUM_DEC4));
        vWriteDspWORD (u1DspId, ADDR_RC2D_PCM_INPUT_CH_ASSIGNMENT_DEC4,
                       u2ReadDspShmWORD (u1DspId, W_PCM_INPUT_CH_ASSIGNMENT_DEC4));
        vWriteDspWORD (u1DspId, ADDR_RC2D_PCM_INPUT_TYPE_DEC4,
                       u2ReadDspShmWORD (u1DspId, W_PCM_INPUT_TYPE_DEC4));  //0:pcm24 1:IMA 3:MS 2:pcm16 //msb=1: little endian
        vWriteDspWORD (u1DspId, ADDR_RC2D_PCM_INPUT_SAMPLING_RATE_DEC4,
                       u2ReadDspShmWORD (u1DspId, W_PCM_INPUT_SAMPLING_RATE_DEC4));
        break;
#ifdef MP3ENC_SUPPORT
	case MP3ENC_STREAM:		///[Joel]20090327
		//Need set by MW
	    ///Printf("Mp3 Encoder Share Info Initialize \n");
		WriteDspCommDram(u1DspId, ADDR_RC2D_ENCODER_OUTBUF_READ_PNT_DEC4, _tPbsBlk[0].wSblk << 8);
		WriteDspCommDram(u1DspId, ADDR_RC2D_ENCODER_OUTBUF_WRITE_PNT_DEC4, _tPbsBlk[0].wSblk << 8);
		WriteDspCommDram(u1DspId, ADDR_RC2D_ENCODER_OUTBUF_SIZE_DEC4, (_tPbsBlk[0].wEblk - _tPbsBlk[0].wSblk) << 8);

		WriteDspCommDram(u1DspId, ADDR_RC2D_ENCODER_SFREQ_DEC4, 0x0100);			///48KHz
		WriteDspCommDram(u1DspId, ADDR_RC2D_ENCODER_BITRATE_DEC4, (UINT16)MP3ENC_BR<<8);		///0x0E00, 320kbps / 0x0900, 128Kbps / 0x0B00, 192Jbps
		WriteDspCommDram(u1DspId, ADDR_RC2D_ENCODER_CHANNEL_MODE_DEC4, 0x0000);	///Stereo
		WriteDspCommDram(u1DspId, ADDR_RC2D_ENCODER_MPEG_FLAG_DEC4, 0x0000); /// Set padding bit to never for fix frame size.

        //For upload info
        vWriteDspWORD (u1DspId, ADDR_RC2D_UPLOAD_DATA_FLAG, u2ReadDspShmWORD(u1DspId, W_UPLOAD_DATA_FLAG));
        vWriteDspWORD (u1DspId, ADDR_RC2D_UPLOAD_BLOCK_NUM, uReadDspShmBYTE(u1DspId, B_UPLOAD_BLOCK_NUM));
        vWriteDspWORD (u1DspId, ADDR_RC2D_UPLOAD_BLOCK_SIZE, u2ReadDspShmWORD(u1DspId, W_UPLOAD_BLOCK_SIZE));
        vWriteDspWORD (u1DspId, ADDR_RC2D_UPLOAD_BLK_INT_CNT, uReadDspShmBYTE(u1DspId, B_UPLOAD_BLK_INT_CNT));

		///Set RP,WP pointer address
		_tPbsBlk[0].dwRdPtrAdr = ADDR_RC2D_ENCODER_OUTBUF_READ_PNT_DEC4;
		_tPbsBlk[0].dwWrPtrAdr = ADDR_RC2D_ENCODER_OUTBUF_WRITE_PNT_DEC4;

		///Set Mp3 encoder putbit start & end address
        AUD_WRITE32(REG_PUTBS_BLK0_RISC,
        	(_tPbsBlk[0].wSblk << 16) | (_tPbsBlk[0].wEblk));
		AUD_WRITE32(REG_PUTBS_BLK1_RISC,
        	(_tPbsBlk[1].wSblk << 16) | (_tPbsBlk[1].wEblk));
		break;
#endif
    default:
        break;
    }
}
#endif

/***************************************************************************
     Function : vDspStateInit
  Description :Reset _rDspStatus variable and command queue
               Make sure dsp is in init wait play loop
               if not wake up -> Change to RC_INIT state and issue wakeup cmd
    Parameter :
    Return    :
***************************************************************************/
void vDspStateInit (UINT8 u1DspId)
{
    UINT8 u1DecId;
	
    for (u1DecId=0; u1DecId < AUD_DEC_NUM; u1DecId++)
    {
        if (_rDspStatus[u1DspId][u1DecId].fgDspPlay)
        {
            vDspReset(u1DspId, u1DecId);
        }
    }

    _rDspStatus[u1DspId][AUD_DEC_MAIN].fgDspStop = TRUE;          // stop ok
    _rDspStatus[u1DspId][AUD_DEC_MAIN].fgDspStopIssue = TRUE;
    _rDspStatus[u1DspId][AUD_DEC_MAIN].fgDspPlay = FALSE;
    _rDspStatus[u1DspId][AUD_DEC_MAIN].fgDspRealPlay = FALSE;
    _rDspStatus[u1DspId][AUD_DEC_MAIN].fgDspFlush = FALSE;
    _rDspStatus[u1DspId][AUD_DEC_MAIN].fgSendSample = FALSE;
    _rDspStatus[u1DspId][AUD_DEC_MAIN].fgDspAoutMuted = FALSE;
    _rDspStatus[u1DspId][AUD_DEC_MAIN].fgDspReParsing = FALSE;
    _rDspStatus[u1DspId][AUD_DEC_MAIN].fgDspGetMpgTyp = FALSE;
    _rDspStatus[u1DspId][AUD_DEC_MAIN].fgDACOn = TRUE;
    _rDspVars[u1DspId][AUD_DEC_MAIN].dwDspReInitPts = 0;
    _rDspVars[u1DspId][AUD_DEC_MAIN].bDspStrTyp = (UINT8)0xFF;  //Set default to 0xff, since type 0 is AC3.
    _rDspVars[u1DspId][AUD_DEC_MAIN].dwDspForceResetCnt = 0;
    _rDspVars[u1DspId][AUD_DEC_MAIN].dwStcDiff = 0;

    /* secondary decoder */
    _rDspStatus[u1DspId][AUD_DEC_AUX].fgDspStop = TRUE;      // stop ok
    _rDspStatus[u1DspId][AUD_DEC_AUX].fgDspStopIssue = TRUE;
    _rDspStatus[u1DspId][AUD_DEC_AUX].fgDspPlay = FALSE;
    _rDspStatus[u1DspId][AUD_DEC_AUX].fgDspRealPlay = FALSE;
    _rDspStatus[u1DspId][AUD_DEC_AUX].fgDspFlush = FALSE;
    _rDspStatus[u1DspId][AUD_DEC_AUX].fgDspAoutMuted = FALSE;
    _rDspStatus[u1DspId][AUD_DEC_AUX].fgSendSample = FALSE;
    _rDspVars[u1DspId][AUD_DEC_AUX].dwDspForceResetCnt = 0;
    _rDspVars[u1DspId][AUD_DEC_AUX].bDspStrTyp = (UINT8)0xFF;  //Set default to 0xff, since type 0 is AC3.

#ifdef CC_MT5391_AUD_3_DECODER
     /* third decoder */
    _rDspStatus[u1DspId][AUD_DEC_THIRD].fgDspStop = TRUE;      // stop ok
    _rDspStatus[u1DspId][AUD_DEC_THIRD].fgDspStopIssue = TRUE;
    _rDspStatus[u1DspId][AUD_DEC_THIRD].fgDspPlay = FALSE;
    _rDspStatus[u1DspId][AUD_DEC_THIRD].fgDspRealPlay = FALSE;
    _rDspStatus[u1DspId][AUD_DEC_THIRD].fgDspFlush = FALSE;
    _rDspStatus[u1DspId][AUD_DEC_THIRD].fgDspAoutMuted = FALSE;
    _rDspVars[u1DspId][AUD_DEC_THIRD].dwDspForceResetCnt = 0;
    _rDspVars[u1DspId][AUD_DEC_THIRD].bDspStrTyp = (UINT8)0xFF;  //Set default to 0xff, since type 0 is AC3.
#endif

#ifdef CC_AUD_4_DECODER_SUPPORT
     /* fourth decoder */
    _rDspStatus[u1DspId][AUD_DEC_4TH].fgDspStop = TRUE;      // stop ok
    _rDspStatus[u1DspId][AUD_DEC_4TH].fgDspStopIssue = TRUE;
    _rDspStatus[u1DspId][AUD_DEC_4TH].fgDspPlay = FALSE;
    _rDspStatus[u1DspId][AUD_DEC_4TH].fgDspRealPlay = FALSE;
    _rDspStatus[u1DspId][AUD_DEC_4TH].fgDspFlush = FALSE;
    _rDspStatus[u1DspId][AUD_DEC_4TH].fgDspAoutMuted = FALSE;
    _rDspVars[u1DspId][AUD_DEC_4TH].dwDspForceResetCnt = 0;
    _rDspVars[u1DspId][AUD_DEC_4TH].bDspStrTyp = (UINT8)0xFF;  //Set default to 0xff, since type 0 is AC3.
#endif

    // DSP interrupt flag initial
    _rDspStatus[u1DspId][AUD_DEC_MAIN].fgDspRInt = FALSE;
    _rDspStatus[u1DspId][AUD_DEC_MAIN].fgDspSInt = FALSE;
    _rDspStatus[u1DspId][AUD_DEC_MAIN].fgDspForceReset = FALSE;
    _rDspVars[u1DspId][AUD_DEC_MAIN].dwDspSIntFail = 0;
    _rDspVars[u1DspId][AUD_DEC_MAIN].dwDspFreq = 0xffff;
    _rDspVars[u1DspId][AUD_DEC_MAIN].dwDspRamCodeType = (UINT8)-1;

    // DSP user operation initial
    _rDspStatus[u1DspId][AUD_DEC_MAIN].fgDspUop = FALSE;
    _rDspVars[u1DspId][AUD_DEC_MAIN].dwDspUop = 0;

    WriteDspCommDram (u1DspId, ADDR_RC2D_DOWNLOAD_TABLE_FLAG, 0);
    _rDspStatus[u1DspId][AUD_DEC_MAIN].fgDspPtsSet = FALSE;

    if (!_rDspStatus[u1DspId][AUD_DEC_MAIN].fgDspWakeUp)
    {
        // We can only load code when not wakeup
        // Because at this time dsp is not in cache
        //lint -e{506}
        //lint -e{774}
        if (_fgCommRAMFromFlash)
        {
            vDspLoadRamCode (u1DspId, AUD_COMMRAM);
        }

        /*lint -e{506}*/
        /*lint -e{774}*/
        if (_fgPostRamFromFlash)
        {
            vDspLoadRamCode (u1DspId, AUD_POSTRAM);
        }

        _uDspState[u1DspId] = (UINT8)DSP_RC_INIT;
    }

#if 1///def CC_VORBIS_SUPPORT ///def DSP_VORBIS_SUPPORT
    _uVorbisMkTbl[AUD_DEC_MAIN] = FALSE;
    _uVorbisTblDone[AUD_DEC_MAIN] = FALSE;
    _uVorbisMkTbl[AUD_DEC_AUX] = FALSE;
    _uVorbisTblDone[AUD_DEC_AUX] = FALSE;
    _uVorbisMkTbl[AUD_DEC_THIRD] = FALSE;
    _uVorbisTblDone[AUD_DEC_THIRD] = FALSE;
    _uVorbisIntDec = 0;
#endif
}

/***************************************************************************
     Function : vDspRest
  Description : Reset DSP ,affect DSP status
    Parameter :
    Return    :
****************************************************************************/
void vDspReset(UINT8 u1DspId, UINT8 u1DecId)
{
    UINT32 dwTryCnt;
    UINT32 u4IntData = (UINT32)JOB_RESET_DECODER;

    AUD_DSP_ID_VALIDATE(u1DspId);
    AUD_DEC_ID_VALIDATE(u1DecId);

    _rDspVars[u1DspId][u1DecId].dwDspForceResetCnt++;

    if (!_rDspStatus[u1DspId][AUD_DEC_MAIN].fgDspWakeUp)
    {
        return;
    }

    if (u1DecId == AUD_DEC_AUX)
    {
        u4IntData = JOB_RESET_DECODER2;
    }
#ifdef CC_MT5391_AUD_3_DECODER
    else if (u1DecId == AUD_DEC_THIRD)
    {
        u4IntData = JOB_RESET_JOB3;
    }
#endif
#ifdef CC_AUD_4_DECODER_SUPPORT
    else if (u1DecId == AUD_DEC_4TH)
    {
        u4IntData = JOB_RESET_JOB4;
    }
#endif

    if (!_rDspStatus[u1DspId][u1DecId].fgDspForceReset)
    {
        dwTryCnt = 0;
        _rDspStatus[u1DspId][u1DecId].fgDspStop = TRUE;
        _rDspStatus[u1DspId][u1DecId].fgDspStopIssue = TRUE;
        _rDspStatus[u1DspId][u1DecId].fgDspPlay = FALSE;
        _rDspStatus[u1DspId][u1DecId].fgDspRealPlay = FALSE;
        _rDspStatus[u1DspId][u1DecId].fgDspFlush = FALSE;
        _rDspStatus[u1DspId][u1DecId].fgDspForceResetDec = TRUE;
        _rDspStatus[u1DspId][u1DecId].fgSendSample = FALSE;
        _rDspStatus[u1DspId][u1DecId].fgDspReParsing = FALSE;
        _rDspStatus[u1DspId][u1DecId].fgDspPtsSet = FALSE;
        _rDspVars[u1DspId][u1DecId].dwDspReInitPts = 0;
        _rDspVars[u1DspId][u1DecId].dwDspRamCodeType = (UINT32)-1;

        dwTryCnt = 0;

        while (TRUE)
        {
            CRIT_STATE_T rCritState;

            dwTryCnt++;

            rCritState = x_crit_start();
            if (!fgDspBusy (u1DspId))
            {
                SendDSPInt (u1DspId, INT_RC2D_DSP_JOB_RESET, u4IntData);  // short data & interrupt
                vDspBlockPtrReset (u1DspId, TRUE);
                x_crit_end(rCritState);
                break;
            }
            else
            {
                x_crit_end(rCritState);
                if (dwTryCnt == DSP_TIMEOUT)
                {
                    break;
                }
            }
        }

        vDspDecDes(u1DspId, u1DecId);
    }
}

#ifdef CC_AUD_MIXSOUND_SUPPORT
extern UINT32 _u4MixsoundLoop;
#endif

/***************************************************************************
     Function : void vDspShareInfoInit(void)
  Description : DSP share info initialization(used when no flash download available)
    Parameter : None
    Return    : None
***************************************************************************/
void vDspShareInfoInit (UINT8 u1DspId)
{
    int i;
    UINT32 u4Temp;
#ifdef CC_AUD_USE_FLASH_AQ
    AUD_AQ_TYPE3_DATA_T rAQTypeData;
#endif
#ifdef DSP_SUPPORT_NPTV
    AUD_MTS_FAC_T*     prMtsFactory;
    AUD_A2_FAC_T*     prA2Factory;
    AUD_PAL_FAC_T*     prPalFactory;
    UNUSED(DRVCUST_OptQuery(eAudioMtsFactory, (UINT32 *)(void *) &prMtsFactory));
    UNUSED(DRVCUST_OptQuery(eAudioA2Factory, (UINT32 *)(void *) &prA2Factory));
    UNUSED(DRVCUST_OptQuery(eAudioPalFactory, (UINT32 *)(void *) &prPalFactory));
#endif //DSP_SUPPORT_NPTV

    UNUSED(i);
    UNUSED(u4Temp);
#ifdef CC_AUD_USE_FLASH_AQ
    UNUSED(rAQTypeData);
#endif
    // for debug use
    vWriteDspShmDWRD (u1DspId, D_SPKCFG, _u4DspGetSpkConfig() | 0x40); // full channel, set auto downmix by default (bit6)

    vWriteDspShmDWRD (u1DspId, D_SPKCFG_DEC2,     0x40); // LtRt, set auto downmix by default (bit6)
    vWriteDspShmDWRD (u1DspId, D_SPKCFG_DEC3,     0x40); // LtRt, set auto downmix by default (bit6)

    // Bonding options check  
    vWriteDspShmWORD(u1DspId, W_BONDING_OPTIONS,
#if (!defined(CC_MT5890)&&!defined(CC_MT5882))     
        ((IS_SupportDOLBY()<<0) |(IS_SupportH264()<<4) | (IS_SupportDTS()<<5) | (IS_SupportDRA()<<6)));
#else
        ((IS_SupportDOLBY()<<0) |(IS_SupportH264()<<4) | (IS_SupportDTS()<<5) | (IS_SupportAVS()<<6)));
#endif

    // Reset channel delay
    vWriteDspShmWORD (u1DspId, W_CHDELAY_C,          0x0); //0x0
    vWriteDspShmWORD (u1DspId, W_CHDELAY_L,          0x0);
    vWriteDspShmWORD (u1DspId, W_CHDELAY_R,          0x0);
    vWriteDspShmWORD (u1DspId, W_CHDELAY_LS,         0x0);
    vWriteDspShmWORD (u1DspId, W_CHDELAY_RS,         0x0);
    vWriteDspShmWORD (u1DspId, W_CHDELAY_CH7,        0x0);
    vWriteDspShmWORD (u1DspId, W_CHDELAY_CH8,        0x0);
    vWriteDspShmWORD (u1DspId, W_CHDELAY_SUB,        0x0);
    vWriteDspShmWORD (u1DspId, W_CHDELAY_CH9,        0x0);
    vWriteDspShmWORD (u1DspId, W_CHDELAY_CH10,       0x0);

    vWriteDspShmWORD (u1DspId, W_CHDELAY_C_DEC2,     0);
    vWriteDspShmWORD (u1DspId, W_CHDELAY_L_DEC2,     0);
    vWriteDspShmWORD (u1DspId, W_CHDELAY_R_DEC2,     0);

    vWriteDspShmWORD (u1DspId, W_CHDELAY_DEC3,     0);

    // Input Src Gain
    vWriteDspShmDWRD (u1DspId, D_INSRC_GAIN,         0x20000);   // 0x20000 is 0dB
    vWriteDspShmDWRD (u1DspId, D_INSRC_GAIN_DEC2,    0x20000);   // 0x20000 is 0dB
#ifdef CC_MT5391_AUD_3_DECODER
    vWriteDspShmDWRD (u1DspId, D_INSRC_GAIN_DEC3,    0x20000);   // 0x20000 is 0dB
#endif
#ifdef CC_AUD_4_DECODER_SUPPORT
    vWriteDspShmDWRD (u1DspId, D_INSRC_GAIN_DEC4,    0x20000);   // 0x20000 is 0dB
#endif

    // Set IEC output channel selection to SPDIF
    // For temporary use, need to move to driver custermization
#ifdef CC_AUD_DSP_SUPPORT_AUDYSSEY_TEST_MODE
    vWriteDspShmBYTE (u1DspId, B_IEC_PCMCH,          0x0);//L,R
#else
    vWriteDspShmBYTE (u1DspId, B_IEC_PCMCH,          0x3);//7,8
#endif
#ifdef CC_AUD_ARM_SUPPORT
    vWriteDspShmWORD (u1DspId, W_PROCMOD,    (IS_BYPASS_PROC() ? 0x2c00 : (0xc00 | (IS_SPKCFG_SSLN()<<7))) ); // ch9/10 downmix 0x2c00 yi.xiao for A2
#else
    vWriteDspShmWORD (u1DspId, W_PROCMOD,    (IS_BYPASS_PROC() ? 0xfc00 : (0xc00 | (IS_SPKCFG_SSLN()<<7))) ); // ch9/10 downmix
#endif
    vWriteDspShmBYTE (u1DspId, B_KARAFLAG,          0x0);
    vWriteDspShmDWRD (u1DspId, D_LRMIXRATIO,        0x400000);  // 0.5
    vWriteDspShmBYTE (u1DspId, B_KARAFLAG_DEC2,     0x0);
    vWriteDspShmDWRD (u1DspId, D_LRMIXRATIO_DEC2,   0x400000);  // 0.5

    vWriteDspShmWORD (u1DspId, W_PROCMOD_DEC2, 0x9400);		//downmix to LR
#ifdef CC_MT5391_AUD_3_DECODER
    vWriteDspShmDWRD (u1DspId, W_PROCMOD_DEC3,    0x9000);
#endif
#ifdef CC_AUD_4_DECODER_SUPPORT
    vWriteDspShmDWRD (u1DspId, W_PROCMOD_DEC4,    0x9000);
#endif

    vWriteDspShmDWRD(u1DspId, D_SBASSBOOSTGAIN,  (BASS_TREBLE_GAIN_MAX-1)/2);
    vWriteDspShmDWRD(u1DspId, D_SBASSCLEARGAIN,  (BASS_TREBLE_GAIN_MAX-1)/2);
#ifdef CC_AUD_FY12_LOUDNESS
    vWriteDspShmDWRD(u1DspId, D_SBASSBOOSTGAIN2,  (BASS_TREBLE_ATTN_GAIN_MAX-1)/2); //0dB
    vWriteDspShmDWRD(u1DspId, D_SBASSCLEARGAIN2,  (BASS_TREBLE_ATTN_GAIN_MAX-1)/2);	//0dB
#endif

    vWriteDspShmBYTE (u1DspId, B_IECFLAG,         0);
    //vWriteDspShmBYTE (B_IECFLAG_DEC2,    0);
    vWriteDspShmBYTE (u1DspId, B_IEC_MAX_FREQ, SV_48K);
    vWriteDspShmBYTE (u1DspId, B_IEC_COPYRIGHT, IEC_COPY_FREELY);
    if (AUD_GetTargetCountry() == COUNTRY_EU)
    {
        vWriteDspShmBYTE (u1DspId, B_IEC_CATEGORY_CODE, IEC_ORIGINAL_BROAD_CAST_EU);
    }
    else
    {
        vWriteDspShmBYTE (u1DspId, B_IEC_CATEGORY_CODE, IEC_ORIGINAL_BROAD_CAST);
    }
    vWriteDspShmBYTE (u1DspId, B_IEC_WORD_LENGTH, IEC_WORD_LENGTH_16BIT);
    vWriteDspShmBYTE (u1DspId, B_IECRAW_DELAY_FLAG, (UINT8)DRVCUST_OptGet(eAudioIecRawDelayFlag));
    vWriteDspShmWORD(u1DspId, W_IEC_RAW_OFFSET, (INT16)DRVCUST_OptGet(eAudioIecRawDelay));


    // DSP Group 0 Initial
    vWriteDspShmWORD (u1DspId, W_INTCTRL, 0);
    if (u1DspId==AUD_DSP0) {vWriteDspShmWORD(u1DspId, W_SYSTEM_CTRL,  0x3);} // bit0: enable internal buffer, bit1: internal data is from dec1(0) or dec2(1)
    else                               {vWriteDspShmWORD(u1DspId, W_SYSTEM_CTRL,  0x2);}

    //DEC3_USE_FIFO3
    vWriteDspShmWORD (u1DspId, W_SYSTEM_CTRL2, 2); //bit1 is dec3 source sel.

    vWriteDspShmWORD (u1DspId, W_INTERNAL_BUF_IN_FLAG, 0x2);  // bit 1: fill internal buffer after volume_dec2
#ifdef CC_MT5391_AUD_3_DECODER
#ifndef CC_AUD_SUPPORT_MS10
    vWriteDspShmWORD (u1DspId, W_INTERNAL_BUF2_IN_FLAG, 0x4);  // bit 1: after volume
#else
    vWriteDspShmWORD (u1DspId, W_INTERNAL_BUF2_IN_FLAG, 0x2);  // bit 1: after volume
#endif
#endif
#ifdef CC_AUD_4_DECODER_SUPPORT
    vWriteDspShmWORD (u1DspId, W_INTERNAL_BUF3_IN_FLAG, 0x1);
#endif
    vWriteDspShmWORD (u1DspId, W_AENV_SEMA_CTRL, _u2AenvSemaCtrl[u1DspId]);

/*
#ifdef MP3ENC_SUPPORT
    // Internal Buffer4 from dec2 for ATV audio record.
    // Enable Internal Buffer4 for mp3enc
    vWriteDspShmWORD (u1DspId, W_SYSTEM_CTRL,(u2ReadDspShmWORD (W_SYSTEM_CTRL)|SHM_INBUF4_SRC_SEL));
    vWriteDspShmWORD (u1DspId, W_INTERNAL_BUF4_IN_FLAG, 0x02);   /// 1:Before postprocessor, 2:After postprocessor.
#endif
*/
    vWriteDspShmWORD (u1DspId, W_SYSTEM_SETUP,
        (SYSTEM_MASTER | USE_STC_A1 | AIN_BUF_RD_NOT_HOLD_WR | COPY_CH78_TO_LS_RS |
        (((UINT16)DRVCUST_OptGet(eAudioPanFadeControl))<<9) |
        (((UINT8)DRVCUST_OptGet(eAudioDownmixLR))<<6) |
        (((UINT8)DRVCUST_OptGet(eAudioDownmixPosition))<<4) |
        (((UINT8)DRVCUST_OptGet(eAudioMultiPairOutput))<<3) |
        (IS_SPKCFG_SSLN()<<2))); //FIXME
    //Audio Description
    vWriteDspShmWORD (u1DspId, W_SYSTEM_SETUP_DEC2,
        (SYSTEM_MASTER | USE_STC_A1 | AIN_BUF_RD_NOT_HOLD_WR)); //FIXME
#ifdef CC_MT5391_AUD_3_DECODER
    vWriteDspShmWORD (u1DspId, W_SYSTEM_SETUP_DEC3,
        (SYSTEM_MASTER | USE_STC_A1 | AIN_BUF_RD_NOT_HOLD_WR)); //FIXME
#endif
#ifdef CC_AUD_4_DECODER_SUPPORT
    vWriteDspShmWORD (u1DspId, W_SYSTEM_SETUP_DEC4,
        (SYSTEM_MASTER | USE_STC_A1 | AIN_BUF_RD_NOT_HOLD_WR));
#endif

    vWriteDspShmWORD (u1DspId, W_SPEED,              0);
    vWriteDspShmBYTE (u1DspId, B_BIT,               24); // Bit Resolution
    if (u1DspId==AUD_DSP0)
    {
	vWriteDspShmBYTE (u1DspId, B_BANK576NUM, 	  9+13+22); // for mp3 modification
	vWriteDspShmBYTE (u1DspId, B_BANK384NUM,		  12+20+32);
	vWriteDspShmBYTE (u1DspId, B_BANK320NUM,		  14+24+38);// 320*B_BANK320NUM_DECx must be multiples of 256
	vWriteDspShmBYTE (u1DspId, B_BANK256NUM,		  18+30+48);//sunman for 500ms (48kHz) buffer
    }
    else
    {
	vWriteDspShmBYTE (u1DspId, B_BANK576NUM, 	  9); // for mp3 modification
	vWriteDspShmBYTE (u1DspId, B_BANK384NUM,		  12);
	vWriteDspShmBYTE (u1DspId, B_BANK320NUM,		  14);// 320*B_BANK320NUM_DECx must be multiples of 256
	vWriteDspShmBYTE (u1DspId, B_BANK256NUM,		  18);
    }
    vWriteDspShmWORD (u1DspId, W_SPEED_DEC2,         0);

    vWriteDspShmBYTE (u1DspId, B_BIT_DEC2,          24); // Bit Resolution
    vWriteDspShmBYTE (u1DspId, B_BANK576NUM_DEC2,    9); // for mp3 modification
    vWriteDspShmBYTE (u1DspId, B_BANK384NUM_DEC2,    12);
    vWriteDspShmBYTE (u1DspId, B_BANK320NUM_DEC2,    12);// 320*B_BANK320NUM_DECx must be multiples of 256
    vWriteDspShmBYTE (u1DspId, B_BANK256NUM_DEC2,    18);
#ifdef CC_MT5391_AUD_3_DECODER
    vWriteDspShmBYTE (u1DspId, B_BIT_DEC3,          24); // Bit Resolution
    vWriteDspShmBYTE (u1DspId, B_BANK576NUM_DEC3,    9); // for mp3 modification
    vWriteDspShmBYTE (u1DspId, B_BANK384NUM_DEC3,    12);
    vWriteDspShmBYTE (u1DspId, B_BANK320NUM_DEC3,    12);// 320*B_BANK320NUM_DECx must be multiples of 256
    vWriteDspShmBYTE (u1DspId, B_BANK256NUM_DEC3,    18);
    vWriteDspShmWORD (u1DspId, W_SPEED_DEC3,         0);
#endif

    // Init volume related
    vWriteDspShmBYTE (u1DspId, B_VOLUPORDER,         5); /* volume display up order */
    vWriteDspShmBYTE (u1DspId, B_VOLDOWNORDER,       6); /* volume display down order */
#ifdef __MODEL_slt__
    vWriteDspShmBYTE (u1DspId, B_SOFTMUTEORDER,      0);
#else
    vWriteDspShmBYTE (u1DspId, B_SOFTMUTEORDER,      SOFT_MUTE_ORDER); /* pause key fade in/out */
                                                /* set to 3, fade-in/out time = 56/37 ms @ 32k/48k Hz */
                                                /*--------------------------*/
                                                // Light add comment,
                                                // softmute_num = 128 * 2^(softmute_order)
                                                // Set softmute_order = 0 won't do softmute!
                                                /*--------------------------*/
#endif //__MODEL_slt__

    vWriteDspShmWORD (u1DspId, W_ERRORMUTEBANK,      0);
    vWriteDspShmBYTE (u1DspId, B_IEC_DELAY,         0);
    vWriteDspShmBYTE (u1DspId, B_VOLUPORDER_DEC2,    3); /* volume display up order */
    vWriteDspShmBYTE (u1DspId, B_VOLDOWNORDER_DEC2,  6); /* volume display down order */
#ifdef __MODEL_slt__
    vWriteDspShmBYTE (u1DspId, B_SOFTMUTEORDER_DEC2, 0);
#else
    vWriteDspShmBYTE (u1DspId, B_SOFTMUTEORDER_DEC2, 4); /* pause key fade in/out */
#endif //__MODEL_slt__

#ifdef CC_MT5391_AUD_3_DECODER
#ifdef __MODEL_slt__
    vWriteDspShmBYTE (u1DspId, B_SOFTMUTEORDER_DEC3, 0);
#else
    vWriteDspShmBYTE (u1DspId, B_SOFTMUTEORDER_DEC3, 4); /* pause key fade in/out */
#endif
#endif //CC_MT5391_AUD_3_DECODER
     vWriteDspShmWORD (u1DspId, W_ERRORMUTEBANK_DEC2, 0);

    vWriteDspShmDWRD (u1DspId, D_VOL,            MASTER_VOL_SHM_DEFAULT);
    vWriteDspShmDWRD (u1DspId, D_TRIM_C,         CH_VOL_SHM_DEFAULT);
    vWriteDspShmDWRD (u1DspId, D_TRIM_L,         CH_VOL_SHM_DEFAULT);
    vWriteDspShmDWRD (u1DspId, D_TRIM_R,         CH_VOL_SHM_DEFAULT);
    vWriteDspShmDWRD (u1DspId, D_TRIM_LS,        CH_VOL_SHM_DEFAULT);
    vWriteDspShmDWRD (u1DspId, D_TRIM_RS,        CH_VOL_SHM_DEFAULT);
    vWriteDspShmDWRD (u1DspId, D_TRIM_CH7,       CH_VOL_SHM_DEFAULT);
    vWriteDspShmDWRD (u1DspId, D_TRIM_CH8,       CH_VOL_SHM_DEFAULT);
    vWriteDspShmDWRD (u1DspId, D_TRIM_SUB,       CH_VOL_SHM_DEFAULT);
    vWriteDspShmDWRD (u1DspId, D_TRIM_LFE,       CH_VOL_SHM_DEFAULT);
    vWriteDspShmDWRD (u1DspId, D_DIALOGUE,       CH_VOL_SHM_DEFAULT);
    vWriteDspShmDWRD (u1DspId, D_TRIM_CH9,       CH_VOL_SHM_DEFAULT);
    vWriteDspShmDWRD (u1DspId, D_TRIM_CH10,      CH_VOL_SHM_DEFAULT);
	vWriteDspShmDWRD (u1DspId, D_UPLOAD_VOL,     CH_VOL_SHM_DEFAULT);

    vWriteDspShmDWRD (u1DspId, D_VOL_DEC2,       MASTER_VOL_SHM_DEFAULT);
    vWriteDspShmDWRD (u1DspId, D_TRIM_C_DEC2,    CH_VOL_SHM_DEFAULT);
    vWriteDspShmDWRD (u1DspId, D_TRIM_L_DEC2,    CH_VOL_SHM_DEFAULT);
    vWriteDspShmDWRD (u1DspId, D_TRIM_R_DEC2,    CH_VOL_SHM_DEFAULT);
    vWriteDspShmDWRD (u1DspId, D_TRIM_LFE_DEC2,  CH_VOL_SHM_DEFAULT);
    vWriteDspShmDWRD (u1DspId, D_DIALOGUE_DEC2,  CH_VOL_SHM_DEFAULT);

#ifdef CC_MT5391_AUD_3_DECODER
    vWriteDspShmDWRD (u1DspId, D_VOL_DEC3,       MASTER_VOL_SHM_DEFAULT);
    vWriteDspShmDWRD (u1DspId, D_TRIM_L_DEC3,  CH_VOL_SHM_DEFAULT);
    vWriteDspShmDWRD (u1DspId, D_TRIM_R_DEC3,  CH_VOL_SHM_DEFAULT);
    vWriteDspShmDWRD (u1DspId, D_DIALOGUE_DEC3,  CH_VOL_SHM_DEFAULT);
#endif
#ifdef CC_AUD_4_DECODER_SUPPORT
    vWriteDspShmDWRD (u1DspId, D_VOL_DEC4,       MASTER_VOL_SHM_DEFAULT);
    vWriteDspShmDWRD (u1DspId, D_TRIM_L_DEC4,    CH_VOL_SHM_DEFAULT);
    vWriteDspShmDWRD (u1DspId, D_TRIM_R_DEC4,    CH_VOL_SHM_DEFAULT);
#endif

    vWriteDspShmDWRD (u1DspId, D_VOL_L,          CH_VOL_SHM_DEFAULT);
    vWriteDspShmDWRD (u1DspId, D_VOL_R,          CH_VOL_SHM_DEFAULT);
    
    //FOR DSP PAN FUNCTION INITIAL VALUES, CC_AUD_PANFADE_SUPPORT
    vWriteDspShmDWRD (u1DspId, D_FADEVALUE,      CH_VOL_SHM_DEFAULT);

    // Operation mode
    vWriteDspShmDWRD (u1DspId, W_AC3KARAMOD, 1); // 1: disable
    vWriteDspShmDWRD (u1DspId, W_AC3KARAMOD_DEC2, 1);
    vWriteDspShmDWRD (u1DspId, W_AC3KARAMOD_DEC3, 1);
    if (AUD_GetTargetCountry() == COUNTRY_EU)
    {
#ifdef CC_AUD_SX1_FEATURE
        vWriteDspShmDWRD (u1DspId, W_AC3COMPMOD, 3); // 0:Custome 0, 1: custom 1, 2:Line, 3:RF
        vWriteDspShmDWRD (u1DspId, W_AC3COMPMOD_DEC2, 3);
        vWriteDspShmDWRD (u1DspId, W_AC3COMPMOD_DEC3, 3);
#else
        vWriteDspShmDWRD (u1DspId, W_AC3COMPMOD, 2); // 0:Custome 0, 1: custom 1, 2:Line, 3:RF
        vWriteDspShmDWRD (u1DspId, W_AC3COMPMOD_DEC2, 2);
        vWriteDspShmDWRD (u1DspId, W_AC3COMPMOD_DEC3, 2);
#endif
    }
    else
    {
        vWriteDspShmDWRD (u1DspId, W_AC3COMPMOD, 2); // 0:Custome 0, 1: custom 1, 2:Line, 3:RF
        vWriteDspShmDWRD (u1DspId, W_AC3COMPMOD_DEC2, 2);
        vWriteDspShmDWRD (u1DspId, W_AC3COMPMOD_DEC3, 2);
    }
    vWriteDspShmDWRD (u1DspId, D_AC3DYN_LOW, 0x007FFFFF); // 1dB
    vWriteDspShmDWRD (u1DspId, D_AC3DYN_HIGH, 0x007FFFFF); // 1dB
    vWriteDspShmDWRD (u1DspId, D_AC3DYN_LOW_DEC2, 0x007FFFFF); // 1dB
    vWriteDspShmDWRD (u1DspId, D_AC3DYN_HIGH_DEC2, 0x007FFFFF); // 1dB
    vWriteDspShmDWRD (u1DspId, D_AC3DYN_LOW_DEC3, 0x007FFFFF); // 1dB
    vWriteDspShmDWRD (u1DspId, D_AC3DYN_HIGH_DEC3, 0x007FFFFF); // 1dB

    vWriteDspShmBYTE (u1DspId, B_DDT_DUALMODE, 0); // stereo
    vWriteDspShmBYTE (u1DspId, B_DDT_COMPVAL, 100);   //100 percentage
    vWriteDspShmBYTE (u1DspId, B_DDT_COMPMODE, DMX_MS10_RF);   // RF mode
    vWriteDspShmBYTE (u1DspId, B_DDT_DUALMODE_DEC2, 0); // stereo
    vWriteDspShmBYTE (u1DspId, B_DDT_COMPVAL_DEC2,100);   //100 percentage
    vWriteDspShmBYTE (u1DspId, B_DDT_COMPMODE_DEC2, DMX_MS10_RF);   // RF mode
    vWriteDspShmBYTE (u1DspId, B_DDT_DUALMODE_DEC3, 0); // stereo
    vWriteDspShmBYTE (u1DspId, B_DDT_COMPVAL_DEC3,100);   //100 percentage
    vWriteDspShmBYTE (u1DspId, B_DDT_COMPMODE_DEC3, DMX_MS10_RF);   // RF mode
    //B_DDT_CONTROL=0: normal mode (for all models except JP)
    //B_DDT_CONTROL=2: JP model (ddco should use dmx.pra, no CC_AUD_DOLBY_SUPPORT_DDCO, no CC_AUD_DOLBY_SUPPORT_DDT)
    //B_DDT_CONTROL=3: special mode (for JP model but share code with ex. EU model, output AAC RAW and bypass MDC/DDCO
#ifndef CC_AUD_IECRAW_AAC
    vWriteDspShmBYTE (u1DspId, B_DDT_CONTROL, 0); //bit0: bypass MDC/DDCO, bit1: output AAC RAW
#else
    #if defined(CC_AUD_DOLBY_SUPPORT_DDCO) || defined(CC_AUD_DOLBY_SUPPORT_DDT)
    //should not happen, set to 3
    vWriteDspShmBYTE (u1DspId, B_DDT_CONTROL, 3); //bit0: bypass MDC/DDCO, bit1: output AAC RAW
    #else
    vWriteDspShmBYTE (u1DspId, B_DDT_CONTROL, 2); //bit0: bypass MDC/DDCO, bit1: output AAC RAW
    #endif
#endif

#ifdef FRAC_SPEED_SUPPORT
    vWriteDspShmDWRD(u1DspId, D_PLAY_SPEED, 0x00000000); //sunman for play speed : initial set not on
#endif

#ifdef CC_AUD_AVC_V20
    // New AVC
    vWriteDspShmWORD (u1DspId, W_AVC_FLAG, 0x3); // default is enable
    vWriteDspShmWORD (u1DspId, W_AVC_FC, 26); // 26 = 270 Hz
    vWriteDspShmDWRD (u1DspId, D_AVC_TARGET_LEVEL, 0x10000); // -24dB
    vWriteDspShmDWRD (u1DspId, D_AVC_ATTACK_RATE, 0x200);
    vWriteDspShmDWRD (u1DspId, D_AVC_RELEASE_RATE, 0x28);
    vWriteDspShmDWRD (u1DspId, D_AVC_RATIO, 0x0);
    vWriteDspShmDWRD (u1DspId, D_AVC_SILENCE_LEVEL, 0x10); // -96dB 
    vWriteDspShmDWRD (u1DspId, D_AVC_MAX_EXPAND, 0x300); // 3dB 
    vWriteDspShmDWRD (u1DspId, D_AVC_POST_GAIN, 0x0); // off 
    vWriteDspShmDWRD (u1DspId, D_AVC_HOLD, 0x0); // 0ms     
    vWriteDspShmDWRD (u1DspId, D_AVC_TARGET_LEVEL_1, 0x10000); // -24dB
    vWriteDspShmDWRD (u1DspId, D_AVC_ATTACK_RATE_1, 0x200);
    vWriteDspShmDWRD (u1DspId, D_AVC_RELEASE_RATE_1, 0x28);
    vWriteDspShmDWRD (u1DspId, D_AVC_RATIO_1, 0x0);   
    vWriteDspShmDWRD (u1DspId, D_AVC_SILENCE_LEVEL_1, 0x10); // -96dB 
    vWriteDspShmDWRD (u1DspId, D_AVC_MAX_EXPAND_1, 0x300); // 3dB   
    vWriteDspShmDWRD (u1DspId, D_AVC_POST_GAIN_1, 0x0); // off 
    vWriteDspShmDWRD (u1DspId, D_AVC_HOLD_1, 0x0); // 0ms    
#else
    // Light added for AVC
    vWriteDspShmWORD (u1DspId, W_AVC_TARGET_LEV,         (-20));
    vWriteDspShmWORD (u1DspId, W_AVC_SILENCE_LEV,        (-98));
    vWriteDspShmWORD (u1DspId, W_AVC_MAX_GAIN_UP,        6);
    vWriteDspShmWORD (u1DspId, W_AVC_MAX_GAIN_DOWN,      40);
    vWriteDspShmWORD (u1DspId, W_AVC_FLAG,               3);     // 3: On, 0: off
    vWriteDspShmWORD (u1DspId, W_AVC_ATTACK_THRES,       AVC_ATTACK_THRES);
    vWriteDspShmWORD (u1DspId, W_AVC_ADJUST_RATE,        0x80);  // (8.8) format, 0x80 = 0.5 dB
    vWriteDspShmWORD (u1DspId, W_DSP_AVC_UI_ADJUST_RATE, 0x80);  // (16.8) format, 4dB / sec @ 32k Hz
#endif

    // Mixsound
    vWriteDspShmWORD (u1DspId, W_MIXSOUND_FLAG,         0);
    vWriteDspShmWORD (u1DspId, W_MIXSOUND_CFG,     0x1f00); // LR/C/LsRs mix, Mono, LPCM, 1X
    vWriteDspShmDWRD (u1DspId, D_MIXSOUND_PADDR,   u4GetMixSoundStartByteAddr());
    vWriteDspShmDWRD (u1DspId, D_MIXSOUND_GAIN, DRVCUST_OptGet(eAudioMixsoundGain)); // 0x20000 --> 0dB
    vWriteDspShmDWRD (u1DspId, D_MIXSOUND_PLEN, 0);
    _u4MixsoundLoop = DRVCUST_OptGet(eAudioMixsoundLength);

    // System master
    vWriteDspShmDWRD (u1DspId, D_STC_DIFF_LO,       (STC_DIFF_BOUND_032MS * 1 *(1+u4GetStcDiffFactor(AUD_STC_LO))) );      // 32ms (1 frame of AC-3 @ 48kHz)
    vWriteDspShmDWRD (u1DspId, D_STC_DIFF_HI,       (STC_DIFF_BOUND_064MS * 2 * (1+u4GetStcDiffFactor(AUD_STC_HI))) );  // 128ms (4 frame of AC-3 @ 48kHz)
    vWriteDspShmDWRD (u1DspId, D_STC_DIFF_WS,       (STC_DIFF_BOUND_500MS * (1+u4GetStcDiffFactor(AUD_STC_WS))) );      // 0.5 second
    vWriteDspShmDWRD (u1DspId, D_STC_THRESHOLD,      STC_THR_42DB);                // -42dB
    vWriteDspShmDWRD (u1DspId, D_STC_DIFF_LO_DEC2,   STC_DIFF_BOUND_032MS * 1);        // 32ms (1 frame of AC-3 @ 48kHz)
    vWriteDspShmDWRD (u1DspId, D_STC_DIFF_HI_DEC2,  (STC_DIFF_BOUND_064MS * 2));   // 128ms (4 frame of AC-3 @ 48kHz)
    vWriteDspShmDWRD (u1DspId, D_STC_DIFF_WS_DEC2,   STC_DIFF_BOUND_500MS);        // 0.5 second

    vWriteDspShmDWRD (u1DspId, D_STC_THRESHOLD_DEC2, STC_THR_42DB);                // -36dB
    vWriteDspShmDWRD (u1DspId, D_STC_DIFF_LO_DEC3,   STC_DIFF_BOUND_032MS * 1);        // 32ms (1 frame of AC-3 @ 48kHz)
    vWriteDspShmDWRD (u1DspId, D_STC_DIFF_HI_DEC3,  (STC_DIFF_BOUND_064MS * 2));   // 128ms (4 frame of AC-3 @ 48kHz)
    vWriteDspShmDWRD (u1DspId, D_STC_DIFF_WS_DEC3,   STC_DIFF_BOUND_500MS);        // 0.5 second
    vWriteDspShmDWRD (u1DspId, D_STC_THRESHOLD_DEC3, STC_THR_36DB);                // -36dB

    vWriteDspShmDWRD (u1DspId, D_1ST_PTS_PRS_PNT,        0);
    vWriteDspShmWORD (u1DspId, W_1ST_PTS_STCH,           0);
    vWriteDspShmWORD (u1DspId, W_1ST_PTS_STCL,           0);
    vWriteDspShmDWRD (u1DspId, D_1ST_PTS_PRS_PNT_DEC2,   0);
    vWriteDspShmWORD (u1DspId, W_1ST_PTS_STCH_DEC2,      0);
    vWriteDspShmWORD (u1DspId, W_1ST_PTS_STCL_DEC2,      0);
    vWriteDspShmDWRD (u1DspId, D_1ST_PTS_PRS_PNT_DEC3,   0);
    vWriteDspShmWORD (u1DspId, W_1ST_PTS_STCH_DEC3,      0);
    vWriteDspShmWORD (u1DspId, W_1ST_PTS_STCL_DEC3,      0);

    vWriteDspShmDWRD (u1DspId, D_STC_THRESHOLD_DEC2, STC_THR_36DB); // -36dB

    // Decoding start point
    vWriteDspShmDWRD (u1DspId, D_DECODING_STR_PNT,       _u4AFIFO[u1DspId][AUD_DEC_MAIN][0]);
    vWriteDspShmDWRD (u1DspId, D_DECODING_STR_PNT_DEC2,  _u4AFIFO[u1DspId][AUD_DEC_AUX][0]);
    vWriteDspShmDWRD (u1DspId, D_DECODING_STR_PNT_DEC3,  _u4AFIFO[u1DspId][AUD_DEC_THIRD][0]);

    // DSP Group6 Initial
    //Individual Speaker Config Setting
    vWriteDspShmWORD (u1DspId, W_AC3_SPKCFG,         0xFFFF);
    vWriteDspShmWORD (u1DspId, W_MP2_SPKCFG,         0xFFFF);

    //Address Setting
    vWriteDspShmDWRD (u1DspId, D_MEMBACKUPADDR,      CMPT_POST_TEMP_BUFFER);
    vWriteDspShmDWRD (u1DspId, D_3D_BUF_ADDR,        CMPT_UPMIX_BUFFER); // 0x0d000
    vWriteDspShmDWRD (u1DspId, D_DNMX_BUF_ADR,       CMPT_DONWMIX_BUFFER);

    // DSP Group7 Initial
    vWriteDspShmBYTE (u1DspId, B_SPECTRUMUP,         SPECTRUM_HIGH_ORDER); //Spectrum Order
    vWriteDspShmBYTE (u1DspId, B_SPECTRUMDOWN,       SPECTRUM_LOW_ORDER);

    vWriteDspShmWORD (u1DspId, W_WMAPACKET,           0);
    vWriteDspShmWORD (u1DspId, W_VORBISPACKET,        0);    ///CC_VORBIS_SUPPORT
    // Bass filter cutoff freq
    vWriteDspShmDWRD (u1DspId, D_CUTOFF_FREQ,            CUTOFF_FREQ);
    vWriteDspShmDWRD (u1DspId, D_CUTOFF_FREQ_DEC2,  CUTOFF_FREQ);
#ifdef CC_S_SUBWOFFER_SUPPORT  ///CC_BASS_MANGER_INDEP_LPF
    vWriteDspShmDWRD (u1DspId, D_CUTOFF_FREQ_LPF, 300);
    vWriteDspShmDWRD (u1DspId, D_CUTOFF_FREQ_LPF_DEC2, 300);
#endif

    //vWriteDspShmBYTE (B_AC3AUTODNMIX,        1); // default is off: [0: no Right surround for LoRo]
    //vWriteDspShmBYTE (B_AC3AUTODNMIX_DEC2,        1); // default is off: [0: no Right surround for LoRo]

    // DDC control
#ifdef CC_AUD_DVBT_AC3_BYPASS
    vWriteDspShmWORD(u1DspId, W_DDC_CONTROL,        0x1);
    vWriteDspShmWORD(u1DspId, W_DDC_CONTROL_DEC2,        0x1);
#else
    vWriteDspShmWORD(u1DspId, W_DDC_CONTROL,        0x0);
    vWriteDspShmWORD(u1DspId, W_DDC_CONTROL_DEC2,        0x0);
#endif
#ifdef CC_AUD_SUPPORT_MS10
    // DDC associated strem id
    vWriteDspShmBYTE (u1DspId, B_DDC_STRMID,        0);
    vWriteDspShmBYTE (u1DspId, B_DDC_STRMID_DEC2,        0);
    // DDC mixer
    vWriteDspShmBYTE (u1DspId, B_DDC_ASSOC_MIX,        0);
    vWriteDspShmBYTE (u1DspId, B_DDC_ASSOC_MIX_DEC2,        0);
#endif

    // initialization of B_EQBANDNUM or B_EQBANDNUM_DEC2
    vWriteDspShmBYTE (u1DspId, B_EQBANDNUM,           4);
    vWriteDspShmBYTE (u1DspId, B_EQBANDNUM_DEC2,      4);

    //Virtual Surround
#ifndef CC_AUD_NEW_POST_MULTI_VS_SUPPORT
    vWriteDspShmBYTE (u1DspId, B_VSURRFLAG,    VSS_NONE);
#else
    vWriteDspShmWORD (u1DspId, W_VSURRFLAG,    VSS_NONE);
#endif
    vWriteDspShmBYTE (u1DspId, B_VSURRDELAYNUM,       5);
    vWriteDspShmDWRD (u1DspId, D_VSURRGAIN,     0x20000);
    vWriteDspShmDWRD (u1DspId, D_VSURRWIDEGAIN, 0x20000);
    #if 1
    vWriteDspShmDWRD (u1DspId, D_VSURR_CLARITY,GetAudioVsClarity());   //center gain,
    vWriteDspShmDWRD (u1DspId, D_VSURR_WIDTH,GetAudioVsWidth());   //surround gain,
    vWriteDspShmDWRD (u1DspId, D_VSURR_LR_GAIN,GetAudioVsLRGain()); //L/R gain,
    vWriteDspShmDWRD (u1DspId, D_VSURR_CROS_TALK,GetAudioVsXTalk()); //crosstalk,
    vWriteDspShmDWRD (u1DspId, D_VSURR_OUTPUT_GAIN,GetAudioVsOutputGain()); //output gain,
    vWriteDspShmDWRD (u1DspId, D_VSURR_BASS_GAIN,GetAudioVsBassGain()); //bass enhancement,
    vWriteDspShmDWRD (u1DspId, D_VSURR_FO,GetAudioVsFO()); //FO,
    #else
    vWriteDspShmDWRD (u1DspId, D_VSURR_CLARITY,   0xa0000);   //center gain,
    vWriteDspShmDWRD (u1DspId, D_VSURR_WIDTH,   0xa0000);   //surround gain,
    vWriteDspShmDWRD (u1DspId, D_VSURR_LR_GAIN,   0x200000); //L/R gain,
    vWriteDspShmDWRD (u1DspId, D_VSURR_CROS_TALK,   0xa0000); //crosstalk,
    vWriteDspShmDWRD (u1DspId, D_VSURR_OUTPUT_GAIN,   0xc0000); //output gain,
    vWriteDspShmDWRD (u1DspId, D_VSURR_BASS_GAIN,   0x150000); //bass enhancement,
    vWriteDspShmDWRD (u1DspId, D_VSURR_FO,   0x07f4300); //FO,
    #endif

    _rDspVars[u1DspId][AUD_DEC_MAIN].b3DSurrMode = SV_OFF;

#ifdef DSP_SUPPORT_SRSTSXT
	/*
		Rice: Define all flags by nibble for easy maintaining.

		bit  0~ 3:	Bypass (0: No, 1: Yes)
		bit  4~ 7:	TS Disable (0: No, 1: Yes)
		bit  8~11:	TS Headphone Mode (0: No, 1: Yes)
		bit 12~15:	Input Mode	(0~12)
		bit 16~19:	FOCUS Enable (0: No, 1: Yes)
		bit 20~23:	TruBass Enable (0: No, 1: Yes)
		bit 24~27:	TruBass Speaker Size (0~7)
		bit 28~31:	reserved
	*/
	vWriteDspShmDWRD(u1DspId, D_SRS_TSXT_FLAGS,		   DRVCUST_OptGet(eAudioTSXTFlag)); //default speaker size 2
	vWriteDspShmDWRD(u1DspId, D_SRS_TSXT_FC_Elevation, DRVCUST_OptGet(eAudioTSXTElev));
	vWriteDspShmDWRD(u1DspId, D_SRS_TSXT_TB_TBCtrl,	   DRVCUST_OptGet(eAudioTSXTTBCtrl));
	vWriteDspShmDWRD(u1DspId, D_SRS_TSXT_Input_gain,    DRVCUST_OptGet(eAudioTSXTInputGain));
#endif //DSP_SUPPORT_SRSTSXT

    /* SRS TSHD2CH */
#if (defined(DSP_SUPPORT_SRSTSHD) || defined(DSP_SUPPORT_SRSWOWHD)) //sunman_tshd

    vWriteDspShmDWRD(u1DspId, D_SRS_TSHD_INPUT_GAIN, DRVCUST_OptGet(eAudioTSHDInputGain)); //-8dB
    vWriteDspShmDWRD(u1DspId, D_SRS_TSHD_OUTPUT_GAIN, DRVCUST_OptGet(eAudioTSHDOutputGain)); //0dB
    vWriteDspShmDWRD(u1DspId, D_SRS_TSHD_Surr_Level, DRVCUST_OptGet(eAudioTSHDSurrLvl) ); //60%
    vWriteDspShmBYTE(u1DspId, B_SRS_TSHD_SPKSIZE, (UINT8)DRVCUST_OptGet(eAudioTSHDSpkSize)); //150Hz
    vWriteDspShmDWRD(u1DspId, D_SRS_TSHD_TB_TBCtrl, DRVCUST_OptGet(eAudioTSHDTbCtrl));      //50%
    vWriteDspShmDWRD(u1DspId, D_SRS_TSHD_TB_COMPRCtrl, DRVCUST_OptGet(eAudioTSHDComprCtrl));      //25%
    vWriteDspShmDWRD(u1DspId, D_SRS_TSHD_DefCtrl, DRVCUST_OptGet(eAudioTSHDDefCtrl));          //30%
    vWriteDspShmDWRD(u1DspId, D_SRS_TSHD_FC_Elevation,  DRVCUST_OptGet(eAudioTSHDElevCtrl)); //50%
    vWriteDspShmBYTE(u1DspId, B_SRS_TSHD_ANASIZE, (UINT8)DRVCUST_OptGet(eAudioTSHDAnaSize)); //150Hz
    vWriteDspShmBYTE(u1DspId, B_SRS_TSHD_TBFLAG, 0x0f);  //EANBLE
    vWriteDspShmBYTE(u1DspId, B_SRS_TSHD_HPFSIZE, 0); //80Hz
    vWriteDspShmBYTE(u1DspId, B_SRS_TSHD_SUBSPKSIZE, 0);
    vWriteDspShmBYTE(u1DspId, B_SRS_TSHD_COFREQ, 0);
    vWriteDspShmBYTE(u1DspId, B_SRS_TSHD_INP_TYPE, 1 );
    vWriteDspShmBYTE(u1DspId, B_SRS_TSHD_CH_DEST, 7);
    vWriteDspShmDWRD(u1DspId, D_SRS_TSHD_LimiterCtrl, 0x00600000);
#ifdef DSP_SUPPORT_SRSTSHD
    /*
        ;ts_mode_flags
        ;bit-0  enable Surround/WOW3D block
        ;bit-1  enable TruBass        block
        ;bit-2  enable Definition     block
        ;bit-3  enable Focus          block
        ;bit-4  enable WOW Limiter    block
        ;bit-5  enable C output
        ;bit-6  enable LFE output
        ;bit-7  enable Headphone mode
        ;bit-8  enable SubTruBass
        ;bit-9  enable TrueSurround  Mode ts_trusurround
        ;bit-10 enable PassiveMatrix Mode ts_passive
        ;bit-11 set space and center
        ;bit-12 LtRt
        ;bit-13 PL2_music
        ;bit-15 move lfe to left
    */
     vWriteDspShmDWRD(u1DspId, D_SRS_TSHD_FLAGS, 0x0000151f);
     vWriteDspShmDWRD(u1DspId, D_SRS_TSHD_Space_Ctrl, 0x0050a3d7);
     vWriteDspShmDWRD(u1DspId, D_SRS_TSHD_Center_Ctrl, 0x002d70a4);
#else
     vWriteDspShmDWRD(u1DspId, D_SRS_TSHD_FLAGS, 0x0000141f);
     vWriteDspShmDWRD(u1DspId, D_SRS_TSHD_Space_Ctrl, 0x00666666);
     vWriteDspShmDWRD(u1DspId, D_SRS_TSHD_Center_Ctrl, 0x00400000);
#endif
#endif


#if (defined(DSP_SUPPORT_SRSCC3D))

    vWriteDspShmWORD(u1DspId, W_SRS_CC3D_FLAG, (UINT16)DRVCUST_OptGet(eAudioCC3Dflag)); //0x00007bfb
    //vWriteDspShmBYTE(B_SRS_CC3D_MODE, (UINT8)DRVCUST_OptGet(eAudioCC3Dmode)); // 1
    vWriteDspShmBYTE(u1DspId, B_SRS_CC3D_MODE, 0); // 0
    vWriteDspShmWORD(u1DspId, W_SRS_CC3D_INPUT_GAIN, 0x4000); //-8dB
    vWriteDspShmDWRD(u1DspId, D_SRS_CC3D_OUTPUT_GAIN, DRVCUST_OptGet(eAudioCC3DOutputGain)); //0dB

    //vWriteDspShmBYTE(B_SRS_CC3D_INMODE, 0x1); //SRS_IO_2_0_0
    vWriteDspShmBYTE(u1DspId, B_SRS_CC3D_INMODE, 17); //SRS_IO_LtRt
    vWriteDspShmBYTE(u1DspId, B_SRS_CC3D_3DMODE, 0x2); //SRS_SRS3D_STEREO
   
    vWriteDspShmBYTE(u1DspId, B_SRS_CC3D_SPKSIZE, (UINT8)DRVCUST_OptGet(eAudioCC3DSpkSize)); //150Hz
    //vWriteDspShmDWRD(D_SRS_CC3D_TB_TBCtrl, DRVCUST_OptGet(eAudioCC3DTbCtrl));      //50%
    vWriteDspShmDWRD(u1DspId, D_SRS_CC3D_TB_TBCtrl, 0x33333330);      //0.4
    vWriteDspShmDWRD(u1DspId, D_SRS_CC3D_TB_COMPRCtrl, DRVCUST_OptGet(eAudioCC3DComprCtrl));      //25%

    vWriteDspShmBYTE(u1DspId, B_SRS_CC3D_ANASIZE, (UINT8)DRVCUST_OptGet(eAudioCC3DAnaSize)); //150Hz
    vWriteDspShmBYTE(u1DspId, B_SRS_CC3D_TBFLAG, 0x0f);  //

    vWriteDspShmBYTE(u1DspId, B_SRS_CC3D_HPFSIZE, (UINT8)DRVCUST_OptGet(eAudioCC3DHpfSize)); //80HZ
    vWriteDspShmBYTE(u1DspId, B_SRS_CC3D_INISIZE, (UINT8)DRVCUST_OptGet(eAudioCC3DIniSize)); //InitNHFilter
    vWriteDspShmBYTE(u1DspId, B_SRS_CC3D_INIDEPTH, 0x20); //DelayLineInitialDepth

    vWriteDspShmWORD(u1DspId, W_SRS_SPACE_CTRLTUNE, 0x2000); //
    vWriteDspShmWORD(u1DspId, W_SRS_MIX_FADECTRL, 0x2000); //
    //vWriteDspShmWORD(W_SRS_MIX_FADECTRL, 0x7fff); //
    //vWriteDspShmWORD(W_SRS_FRONT_CENTERCTRL, 0x2d71); //
    vWriteDspShmWORD(u1DspId, W_SRS_FRONT_CENTERCTRL, 0x4000); //

    vWriteDspShmWORD(u1DspId, W_SRS_FRONT_SPACECTRL, 0x50a4); //
    vWriteDspShmWORD(u1DspId, W_SRS_REAR_SPACECTRL, 0x2d71); //
    vWriteDspShmWORD(u1DspId, W_SRS_REAR_CENTERCTRL, 0x2d71); //Not Used
    vWriteDspShmWORD(u1DspId, W_SRS_TRUSUR_LEVEL, 0x4ccd); //

    //vWriteDspShmDWRD(D_SRS_MIX_FADECTRLEXT, 0x10002000); //
    vWriteDspShmDWRD(u1DspId, D_SRS_MIX_FADECTRLEXT, 0x40000000); //


    vWriteDspShmWORD(u1DspId, W_SRS_SRS3D_CENTERCTRL, 0x4000); //  SRS_FXP32(0.5, 1)
    vWriteDspShmWORD(u1DspId, W_SRS_SRS3D_SPACECTRL, 0x6666); //  SRS_FXP32(0.8f, 1)

    vWriteDspShmWORD(u1DspId, W_SRS_FOCUS_LEVEL, 0x3333); //
    vWriteDspShmWORD(u1DspId, W_SRS_LFOCUS_LEVEL, 0x3333); //
    vWriteDspShmWORD(u1DspId, W_SRS_RFOCUS_LEVEL, 0x3333); //
    vWriteDspShmWORD(u1DspId, W_SRS_LDEF_LEVEL, 0x0ccd); //
    vWriteDspShmWORD(u1DspId, W_SRS_RDEF_LEVEL, 0x0ccd); //

    vWriteDspShmWORD(u1DspId, W_SRS_CC3D_REF_LEV, 0x1400); //REF LEVEL
    vWriteDspShmWORD(u1DspId, W_SRS_CC3D_TBQ, 0x4000); //TBQ
    vWriteDspShmWORD(u1DspId, W_SRS_CC3D_DUCLEV, 0x1999); //DcuLevel

#endif


#ifdef DSP_SUPPORT_SRSVIQ
    //sunman_viq
#if 0
    vWriteDspShmDWRD (u1DspId, D_VIQ_REF_LVL, 0xFFA00000);
    vWriteDspShmDWRD (u1DspId, D_VIQ_MODE, 0x0100); //default light mode
    vWriteDspShmDWRD (u1DspId, D_VIQ_MAX_GAIN, 0x00078000); //default 15
    vWriteDspShmDWRD (u1DspId, D_VIQ_INPUT_GAIN, 0x00200000); //default 1 (0.25) scale 2
    vWriteDspShmDWRD (u1DspId, D_VIQ_OUTPUT_GAIN, 0x00133333); //default (0x15)
#else
    vWriteDspShmDWRD (u1DspId, D_VIQ_REF_LVL, DRVCUST_OptGet(eAudioVIQRefLvl));//default minus for master vol
    vWriteDspShmDWRD (u1DspId, D_VIQ_MODE, DRVCUST_OptGet(eAudioVIQMode)); //default light mode
    vWriteDspShmDWRD (u1DspId, D_VIQ_MAX_GAIN, DRVCUST_OptGet(eAudioVIQMaxGain)); //default 15
    vWriteDspShmDWRD (u1DspId, D_VIQ_INPUT_GAIN, DRVCUST_OptGet(eAudioVIQInputGain)); //default 1 (0.25) scale 2
    vWriteDspShmDWRD (u1DspId, D_VIQ_OUTPUT_GAIN, DRVCUST_OptGet(eAudioVIQOutputGain)); //default (0x15)
    vWriteDspShmDWRD (u1DspId, D_VIQ_NOISE_TH, DRVCUST_OptGet(eAudioVIQNoiseTh)); //sunman_viq2
#endif
#endif
       //;bit 0 SRS dialog, now no use
       //;bit 1 SRS peq
       //;bit 2 SRS HPF
       //;bit 3 SRS hard limiter
       //;bit 4 puresound trubass
       //;bit 5 SRS aeq
#ifdef DSP_SUPPORT_SRS_SSHD
       vWriteDspShmWORD (u1DspId, W_SRS_FLAG,0xA00);                                //ON/OFF
       //SRS HeadRoom
       vWriteDspShmDWRD(u1DspId, D_SRS_HEADROOM, 3); //0: 0db,1: 6db,2: 12db,3: 18db,4: 24db,5: 30db
       vWriteDspShmDWRD(u1DspId, D_SRS_COMPENSATE, 3); //0: 0db,1: 6db,2: 12db,3: 18db,4: 24db,5: 30db
#endif
#ifdef DSP_SUPPORT_SRS_PURESOUND
       vWriteDspShmWORD (u1DspId, W_SRS_FLAG,0x3800);                            //ON/OFF
       //SRS HeadRoom
       vWriteDspShmDWRD(u1DspId, D_SRS_HEADROOM, 3); //0: 0db,1: 6db,2: 12db,3: 18db,4: 24db,5: 30db
       vWriteDspShmDWRD(u1DspId, D_SRS_COMPENSATE, 3); //0: 0db,1: 6db,2: 12db,3: 18db,4: 24db,5: 30db
#endif
#ifdef DSP_SUPPORT_SRS_SS3D
       vWriteDspShmWORD (u1DspId, W_SRS_FLAG,0x2E00);                                //ON/OFF
       //SRS HeadRoom
       vWriteDspShmDWRD(u1DspId, D_SRS_HEADROOM, 3); //0: 0db,1: 6db,2: 12db,3: 18db,4: 24db,5: 30db
       vWriteDspShmDWRD(u1DspId, D_SRS_COMPENSATE, 3); //0: 0db,1: 6db,2: 12db,3: 18db,4: 24db,5: 30db
       Printf("fubofeng shm \n");
#endif

#if defined(DSP_SUPPORT_SRS_PEQ)
    vWriteDspShmDWRD (u1DspId, D_SRS_PEQ_INPUT_GAIN, DRVCUST_OptGet(eAudioSRSPEQInputGain));
    vWriteDspShmDWRD (u1DspId, D_SRS_PEQ_OUTPUT_GAIN, DRVCUST_OptGet(eAudioSRSPEQOutputGain));
    vWriteDspShmWORD (u1DspId, W_SRS_PEQ_BANDS_1, (DRVCUST_OptGet(eAudioSRSPEQBands)<<8));
#endif

#ifdef DSP_SUPPORT_SRS_AEQ
       //bofeng_aeq
	   vWriteDspShmDWRD (u1DspId, D_SRS_AEQ_INPUT_GAIN, DRVCUST_OptGet(eAudioSRSAeqInputGain));
	   vWriteDspShmDWRD (u1DspId, D_SRS_AEQ_OUTPUT_GAIN, DRVCUST_OptGet(eAudioSRSAeqOutputGain));
	   vWriteDspShmDWRD (u1DspId, D_SRS_AEQ_FILTER_SIZE, DRVCUST_OptGet(eAudioSRSAeqFilterSize));
#endif

#ifdef DSP_SUPPORT_SRS_LMT
       //bofeng_lmt
	   //vWriteDspShmDWRD (D_SRS_LMT_INPUT_GAIN, DRVCUST_OptGet(eAudioSRSLmtInputGain));
	   //vWriteDspShmDWRD (D_SRS_LMT_OUTPUT_GAIN, DRVCUST_OptGet(eAudioSRSLmtOutputGain));
	   //vWriteDspShmDWRD (D_SRS_LMT_BOOST, DRVCUST_OptGet(eAudioSRSLmtRefBoost));
	   //vWriteDspShmDWRD (D_SRS_LMT_LEVEL, DRVCUST_OptGet(eAudioSRSLmtRefLevel));
	   //vWriteDspShmDWRD (D_SRS_LMT_DELAY, (DRVCUST_OptGet(eAudioSRSLmtRefDelay) << 8));
	   //vWriteDspShmDWRD (D_SRS_LMT_COEFF, DRVCUST_OptGet(eAudioSRSLmtRefCoeff));
	   //vWriteDspShmDWRD (D_SRS_LMT_THRESH, DRVCUST_OptGet(eAudioSRSLmtRefThresh));

     vWriteDspShmDWRD (u1DspId, D_SRS_LMT_INPUT_GAIN, 0x7fffffff);
	   vWriteDspShmDWRD (u1DspId, D_SRS_LMT_OUTPUT_GAIN, 0x7fffffff);
	   vWriteDspShmDWRD (u1DspId, D_SRS_LMT_BOOST, 0x0a000a00);//
	   vWriteDspShmDWRD (u1DspId, D_SRS_LMT_LEVEL, 0x7fffffff);
	   vWriteDspShmDWRD (u1DspId, D_SRS_LMT_DELAY, 14 << 16);
	   vWriteDspShmDWRD (u1DspId, D_SRS_LMT_COEFF, 0x0010624e);
	   vWriteDspShmDWRD (u1DspId, D_SRS_LMT_THRESH, 0);
#endif

#ifdef DSP_SUPPORT_TCL_VBASS

   vWriteDspShmBYTE(u1DspId, B_TCLBASS_ENABLE     ,  1);
   vWriteDspShmDWRD(u1DspId, D_TCL_AO_PEAK        ,  5);  
   vWriteDspShmDWRD(u1DspId, D_TCL_A1_PEAK        ,  143558);
   vWriteDspShmDWRD(u1DspId, D_TCL_A2_PEAK        ,  4601674);
   vWriteDspShmDWRD(u1DspId, D_TCL_BO_PEAK        ,  24);
   vWriteDspShmDWRD(u1DspId, D_TCL_B1_PEAK        ,  315945);
   vWriteDspShmDWRD(u1DspId, D_TCL_B2_PEAK        ,  4429268);
   vWriteDspShmDWRD(u1DspId, D_TCL_AT             ,  3112);
   vWriteDspShmDWRD(u1DspId, D_TCL_RT             ,  3);
   vWriteDspShmDWRD(u1DspId, D_TCL_A_PEAK         ,  3277);
   vWriteDspShmDWRD(u1DspId, D_TCL_GAIN_HAMONICGEN,  256); 
   vWriteDspShmDWRD(u1DspId, D_TCL_G_OUT          ,  2);
   vWriteDspShmDWRD(u1DspId, D_TCL_GAIN_BACK      ,  1);
   vWriteDspShmDWRD(u1DspId, D_TCL_GAIN_OUT       ,  1);
   vWriteDspShmDWRD(u1DspId, D_TCL_GAINHAMONICOUT ,  1);
   vWriteDspShmDWRD(u1DspId, D_TCL_COEFF_HAM      ,  0);
   vWriteDspShmDWRD(u1DspId, D_TCL_X1PEAK         ,  0);
   vWriteDspShmDWRD(u1DspId, D_TCL_X2PEAK         ,  0);
   vWriteDspShmDWRD(u1DspId, D_TCL_C1PEAK         ,  0);
   vWriteDspShmDWRD(u1DspId, D_TCL_C2PEAK         ,  0);
   vWriteDspShmDWRD(u1DspId, D_TCL_COUT_PEAK      ,  0);
   vWriteDspShmDWRD(u1DspId, D_TCL_Y_OUT          ,  0);

#endif


   // FIXME for testing
   vWriteDspShmWORD (u1DspId, W_PINKNOISE,       0x06ff);
   // Initialize for PCM dec/dec2
   vWriteDspShmWORD (u1DspId, W_PCM_INPUT_CHANNEL_NUM,           0);  //0 for 2-ch, 1 for 4-ch, 2 for 6-ch, 3 for 8-ch
   vWriteDspShmWORD (u1DspId, W_PCM_INPUT_CH_ASSIGNMENT,         0);
   vWriteDspShmWORD (u1DspId, W_PCM_INPUT_CHANNEL_NUM_DEC2,      0);
   vWriteDspShmWORD (u1DspId, W_PCM_INPUT_CH_ASSIGNMENT_DEC2,    0);

#ifdef USE_16BIT_ALIGNMENT
   vWriteDspShmWORD (u1DspId, W_PCM_INPUT_TYPE,2);
#else
   vWriteDspShmWORD (u1DspId, W_PCM_INPUT_TYPE,0);
#endif

   vWriteDspShmWORD (u1DspId, W_PCM_INPUT_TYPE_DEC2,0);
   vWriteDspShmWORD (u1DspId, W_PCM_INPUT_SAMPLING_RATE,8);
   vWriteDspShmWORD (u1DspId, W_PCM_INPUT_SAMPLING_RATE_DEC2,8);

#ifdef CC_MT5391_AUD_3_DECODER
   vWriteDspShmWORD (u1DspId, W_PCM_INPUT_CHANNEL_NUM_DEC3,      0);
   vWriteDspShmWORD (u1DspId, W_PCM_INPUT_CH_ASSIGNMENT_DEC3,    0);
   vWriteDspShmWORD (u1DspId, W_PCM_INPUT_TYPE_DEC3,0);
   vWriteDspShmWORD (u1DspId, W_PCM_INPUT_SAMPLING_RATE_DEC3,8);
#endif //CC_MT5391_AUD_3_DECODER

#ifdef CC_AUD_4_DECODER_SUPPORT
    vWriteDspShmWORD (u1DspId, W_PCM_INPUT_CHANNEL_NUM_DEC4,      0);
    vWriteDspShmWORD (u1DspId, W_PCM_INPUT_CH_ASSIGNMENT_DEC4,    0);
    vWriteDspShmWORD (u1DspId, W_PCM_INPUT_TYPE_DEC4,0);
    vWriteDspShmWORD (u1DspId, W_PCM_INPUT_SAMPLING_RATE_DEC4,8);
#endif //CC_AUD_4_DECODER_SUPPORT

   vWriteDspShmWORD (u1DspId, W_PCM_PREBUF_DELAY_BANK, 0); // 256 sample/unit,
                                                  //ie. 5.33 ms/uinit with 48 KHz
#ifdef DSP_SUPPORT_NPTV
   //all ATV audio shared shm
    vWriteDspShmWORD (u1DspId, W_USER_MODE,      0);
#ifdef __MODEL_slt__
    vWriteDspShmWORD (u1DspId, W_SOURCE_MODE,    0);
#else
    vWriteDspShmWORD (u1DspId, W_SOURCE_MODE,    1);
#endif //__MODEL_slt__
    vWriteDspShmWORD (u1DspId, W_ADC_GAIN,     0x8018);
    vWriteDspShmBYTE (u1DspId, B_HDEV_MODE,
	 (((UINT8)((prMtsFactory->u1HdevMode)&&2) << 3) |
        ((UINT8)((prMtsFactory->u1HdevMode)&&1) << 2) |
        ((UINT8)prA2Factory->fgHdevMode << 1) |
        ((UINT8)prPalFactory->fgHdevMode << 0)));

#ifdef __MODEL_slt__
    vWriteDspShmBYTE (u1DspId, B_CARRIER_SHIFT_MODE,   0);
#else
    vWriteDspShmBYTE (u1DspId, B_CARRIER_SHIFT_MODE,
        (((UINT8)prMtsFactory->fgCarrierShift << 2) |
        ((UINT8)prA2Factory->fgCarrierShift << 1)|
        ((UINT8)prPalFactory->fgCarrierShift << 0)));
#endif //__MODEL_slt__

    vWriteDspShmBYTE (u1DspId, B_FM_SATU_MUTE_MODE,
        (((UINT8)prMtsFactory->fgSatuMuteMode<< 2) |
        ((UINT8)prA2Factory->fgSatuMuteMode << 1) |
        ((UINT8)prPalFactory->fgSatuMuteMode << 0)));
    vWriteDspShmBYTE (u1DspId, B_FM_MUTE_MODE,
        (((UINT8)prMtsFactory->fgCarrierMuteMode<< 2) |
        ((UINT8)prA2Factory->fgCarrierMuteMode << 1) |
        ((UINT8)prPalFactory->fgCarrierMuteMode << 0)));
    vWriteDspShmBYTE (u1DspId, B_NON_EU_FM_MODE,
        (((UINT8)prA2Factory->fgNonEuMode) |
        ((UINT8)prPalFactory->fgNonEuMode)));
    vWriteDspShmBYTE(u1DspId, B_NICAM_I_BOOST_FLAG,
        ((UINT8)prPalFactory->fgNicamIBoost));

    // MTS Parameters
    vWriteDspShmWORD(u1DspId, W_MTS_NUM_CHECK, prMtsFactory->u2NumCheck);
    vWriteDspShmWORD(u1DspId, W_MTS_STEREO_MID, prMtsFactory->u2PilotLowerThd);
    vWriteDspShmWORD(u1DspId, W_MTS_STEREO_CON_MID, prMtsFactory->u2PilotHigherThd);
    vWriteDspShmWORD(u1DspId, W_MTS_NUM_PILOT, prMtsFactory->u2NumPilot);
    vWriteDspShmWORD(u1DspId, W_MTS_NUM_SAP, prMtsFactory->u2NumSap);
    vWriteDspShmWORD(u1DspId, W_MTS_SAP_MID, prMtsFactory->u2SapLowerThd);
    vWriteDspShmWORD(u1DspId, W_MTS_SAP_CON_MID, prMtsFactory->u2SapHigherThd);
    vWriteDspShmDWRD(u1DspId, D_SAP_FINE_TUNE_VOLUME,
        (VOLUME_FINE_TUNE[vFineTuneVolTblIdxChk(prMtsFactory->u1SapPrescale)]) << 8);
    vWriteDspShmDWRD(u1DspId, D_MTS_FINE_TUNE_VOLUME,
        (VOLUME_FINE_TUNE[vFineTuneVolTblIdxChk(prMtsFactory->u1MonoStereoPrescale)]) << 8);
    _u1MtsFineVol = prMtsFactory->u1MonoStereoPrescale;
    _u1SapFineVol = prMtsFactory->u1SapPrescale;
    vWriteDspShmBYTE (u1DspId, B_FM_MUTE_THRESHOLD_HIGHT_MTS,prMtsFactory->u1CarrierMuteHigherThd);
    vWriteDspShmBYTE (u1DspId, B_FM_MUTE_THRESHOLD_LOW_MTS, prMtsFactory->u1CarrierMuteLowerThd);
    vWriteDspShmBYTE (u1DspId, B_MTS_PILOT_OFFSET_DETECTION, (UINT8)prMtsFactory->fgPilotOffsetDetect);
    vWriteDspShmBYTE (u1DspId, B_MTS_MUTE_SAP_LOW,   prMtsFactory->u1SapNoiseMuteLow);
    vWriteDspShmBYTE (u1DspId, B_MTS_MUTE_SAP_HIGH,  prMtsFactory->u1SapNoiseMuteHigh);
    vWriteDspShmBYTE (u1DspId, B_SATU_MUTE_THRESHOLD_HIGH,  prMtsFactory->u1SatuMuteHigherThd );
    vWriteDspShmBYTE (u1DspId, B_SATU_MUTE_THRESHOLD_LOW,  prMtsFactory->u1SatuMuteLowerThd );
#ifdef CC_AUD_DDI
    vWriteDspShmBYTE (u1DspId, B_SAP_HP_MUTE_HIGH,   90);
    vWriteDspShmBYTE (u1DspId, B_SAP_HP_MUTE_LOW,  176);
#else
    vWriteDspShmBYTE (u1DspId, B_SAP_HP_MUTE_HIGH,   35);
    vWriteDspShmBYTE (u1DspId, B_SAP_HP_MUTE_LOW,  10);
#endif
#ifndef CC_AUD_SX1_FEATURE
    vWriteDspShmBYTE (u1DspId, B_SAP_FILTER_SEL,   (((prMtsFactory->u1SAPFilterType)<<1)|(prMtsFactory->u1SAPFilterMode)));
#else
    vWriteDspShmBYTE (u1DspId, B_SAP_FILTER_SEL,   3); //0->3 for COL FT IDW_2000
#endif
    vWriteDspShmBYTE (u1DspId, B_SAP_RATIO,   0x16); //Original :0x1C  //US FT issue ... SAP sensitivity
    vWriteDspShmBYTE (u1DspId, B_MTS_SAP_MUTE_CTRL,   1); // 1 to enable "real sap noise mute function"
    vWriteDspShmBYTE (u1DspId, B_MTS_HDEV_LVL_REDUCE, 1); // 1 to reduce mono output level by 6dB under 576k/768k mode
#ifdef CC_AUD_MTS_OUTPUT_LEVEL_GAIN_SUPPORT
    vWriteDspShmDWRD(u1DspId, D_MTS_OUTPUT_LEVEL_GAIN, prMtsFactory->u2OutputLevelGain);
#endif

    // FMFM Parameters
    vWriteDspShmWORD(u1DspId, W_FMFM_NUM_CHECK, 4);
    vWriteDspShmWORD(u1DspId, W_FMFM_NUM_DOUBLE_CHECK, 9);
    vWriteDspShmWORD(u1DspId, W_FMFM_MONO_WEIGHT, 1);
    vWriteDspShmWORD(u1DspId, W_FMFM_STEREO_WEIGHT, 1);
    vWriteDspShmWORD(u1DspId, W_FMFM_DUAL_WEIGHT, 1);
    vWriteDspShmDWRD(u1DspId, D_FMFM_MONO_FINE_TUNE_VOLUME, (0x2000) << 8);
    vWriteDspShmDWRD(u1DspId, D_FMFM_DUAL_FINE_TUNE_VOLUME, (0x2000) << 8);
    vWriteDspShmWORD(u1DspId, W_FMFM_DETECT_CONFID, 7);
    vWriteDspShmBYTE (u1DspId, B_FM_MUTE_THRESHOLD_HIGHT_EIAJ,   0x22);
    vWriteDspShmBYTE (u1DspId, B_FM_MUTE_THRESHOLD_LOW_EIAJ,   0x20);

    // A2 Parameters
    vWriteDspShmWORD(u1DspId, W_A2_NUM_CHECK, prA2Factory->u2NumCheck);
    vWriteDspShmWORD(u1DspId, W_A2_NUM_DOUBLE_CHECK,  prA2Factory->u2NumDoubleCheck);
    vWriteDspShmWORD(u1DspId, W_A2_MONO_WEIGHT, prA2Factory->u2MonoWeight);
    vWriteDspShmWORD(u1DspId, W_A2_STEREO_WEIGHT,prA2Factory->u2StereoWeight);
    vWriteDspShmWORD(u1DspId, W_A2_DUAL_WEIGHT, prA2Factory->u2DualWeight);
    vWriteDspShmDWRD(u1DspId, D_A2_FINE_TUNE_VOLUME,
        (VOLUME_FINE_TUNE[vFineTuneVolTblIdxChk(prA2Factory->u1A2Prescale)]) << 8);
    vWriteDspShmBYTE (u1DspId, B_FM_MUTE_THRESHOLD_HIGHT_A2, prA2Factory->u1CarrierMuteHigherThd);
    vWriteDspShmBYTE (u1DspId, B_FM_MUTE_THRESHOLD_LOW_A2,  prA2Factory->u1CarrierMuteLowerThd);
    vWriteDspShmBYTE (u1DspId, B_MANUAL_MTS_SW,0);
    vWriteDspShmBYTE (u1DspId, B_MANUAL_OUTPUT_MATRIX,0);
    vWriteDspShmBYTE (u1DspId, B_MANUAL_OUTPUT_MATRIX_DEC2,0);
    vWriteDspShmWORD (u1DspId, W_A2_MONO_STEREO_THRES_EU,   prA2Factory->u2Mono2StereoEU);
    vWriteDspShmWORD (u1DspId, W_A2_STEREO_MONO_THRES_EU, prA2Factory->u2Stereo2MonoEU);
    vWriteDspShmWORD (u1DspId, W_A2_MONO_STEREO_THRES_M,   prA2Factory->u2Mono2StereoM);
    vWriteDspShmWORD (u1DspId, W_A2_STEREO_MONO_THRES_M, prA2Factory->u2Stereo2MonoM);
    vWriteDspShmWORD (u1DspId, W_A2_WEAK_RF_LVL_M2S_THRES_EU, prA2Factory->u2WeakRfLvlM2SEU);
    vWriteDspShmWORD (u1DspId, W_A2_WEAK_RF_LVL_S2M_THRES_EU, prA2Factory->u2WeakRfLvlS2MEU);


    // PAL Parameters
    vWriteDspShmWORD(u1DspId, W_PAL_CORRECT_THRESHOLD, prPalFactory->u2CorrectThd);
    vWriteDspShmWORD(u1DspId, W_PAL_TOTAL_SYNC1_LOOP, prPalFactory->u2TotalSyncLoop);
    vWriteDspShmWORD(u1DspId, W_PAL_ERR_THRESHOLD, prPalFactory->u2ErrorThd);
    vWriteDspShmWORD(u1DspId, W_PAL_PARITY_ERR_THRESHOLD, prPalFactory->u2ParityErrorThd);
    vWriteDspShmWORD(u1DspId, W_PAL_EVERY_NUM_FRAMES, prPalFactory->u2EveryNumFrames);
    vWriteDspShmBYTE (u1DspId, B_AM_MUTE_MODE,   prPalFactory->fgAmMuteMode);
    vWriteDspShmBYTE (u1DspId, B_AM_MUTE_THRESHOLD_LOW,   prPalFactory->u1AmMuteLowerThd);
    vWriteDspShmBYTE (u1DspId, B_AM_MUTE_THRESHOLD_HIGHT,   prPalFactory->u1AmMuteHigherThd);
    vWriteDspShmBYTE (u1DspId, B_FM_MUTE_THRESHOLD_HIGHT_PAL,   prPalFactory->u1CarrierMuteHigherThd);
    vWriteDspShmBYTE (u1DspId, B_FM_MUTE_THRESHOLD_LOW_PAL, prPalFactory->u1CarrierMuteLowerThd);
#ifdef CC_AUD_SX1_FEATURE
    vWriteDspShmBYTE(u1DspId, B_FM_MUTE_THRESHOLD_HIGHT,   prPalFactory->u1CarrierMuteHdevHigherThd);
    vWriteDspShmBYTE(u1DspId, B_FM_MUTE_THRESHOLD_LOW, prPalFactory->u1CarrierMuteHdevLowerThd);
#endif
    vWriteDspShmDWRD(u1DspId, D_PAL_FINE_TUNE_VOLUME,
        (VOLUME_FINE_TUNE[vFineTuneVolTblIdxChk(prPalFactory->u1PalPrescale)]) << 8);
    vWriteDspShmDWRD(u1DspId, D_AM_FINE_TUNE_VOLUME,
        (VOLUME_FINE_TUNE[vFineTuneVolTblIdxChk(prPalFactory->u1AmPrescale)]) << 8);
    vWriteDspShmDWRD(u1DspId, D_NICAM_FINE_TUNE_VOLUME,
        (VOLUME_FINE_TUNE[vFineTuneVolTblIdxChk(prPalFactory->u1NicamPrescale)]) << 8);

    //for PAL/A2, Dynamic detection control
    vWriteDspShmBYTE (u1DspId, B_DYNAMIC_DETECTIO_SWITCH, 0x00);   // bit 0 : 1 -> disable PAL -> A2 dynamic detection for PAL decoder
    //vWriteDspShmBYTE (u1DspId, B_DYNAMIC_DETECTIO_SWITCH, 0x11); // bit 4 : 1 -> disable A2 -> PAL dynamic detection for A2 decoder

    //for PAL, check FM BAD SIGNAL
    vWriteDspShmBYTE (u1DspId, B_FM_BAD_SIGNAL, 0x0);
    vWriteDspShmBYTE (u1DspId, B_CANON_PLUS,   0);

    // Detection Parameters
    vWriteDspShmBYTE(u1DspId, B_DETECT_DEFAULT, 0);  // 0: PAL_DK, 1: SECAM-L
#ifdef CC_AUD_PAA_SUPPORT
    vWriteDspShmDWRD(u1DspId, D_DETECTOR_FM_NOISE_TH  , 0x46000);//Sony FT detect BG as M because noise level is about 0x40000
#else
    vWriteDspShmDWRD(u1DspId, D_DETECTOR_FM_NOISE_TH  , 0x16000);
#endif
    vWriteDspShmBYTE(u1DspId, B_DETECTOR_A2_PILOT_CHK, 0x00);

    //FM RADIO Parameters
    vWriteDspShmDWRD(u1DspId, D_FM_RDO_INBAND_TH  , 0x3000);
    vWriteDspShmDWRD(u1DspId, D_FM_RDO_NOISE_TH  , 0x15000);
    vWriteDspShmDWRD(u1DspId, D_FMRADIO_FINE_TUNE_VOLUME  , 0xcbd00);

#ifdef CC_AUD_BBE_SUPPORT
    // BBE default settings
    vWriteDspShmBYTE (u1DspId, B_BBE_FLAG, 0x0);
    vWriteDspShmBYTE (u1DspId, B_BBE_LEVEL, (UINT8)DRVCUST_OptGet(eAudioBbeLevel));
    vWriteDspShmBYTE (u1DspId, B_BBE_PROCESS, (UINT8)DRVCUST_OptGet(eAudioBbeProcess));
    vWriteDspShmBYTE (u1DspId, B_BBE_LO_CONTOUR, (UINT8)DRVCUST_OptGet(eAudioBbeLoContour));
    vWriteDspShmBYTE (u1DspId, B_BBE_3D_GAIN, (UINT8)DRVCUST_OptGet(eAudioBbe3dGain));
#endif

    //Audio Description setting
    vWriteDspShmBYTE (u1DspId, B_ADFLAG, (UINT8)DRVCUST_OptGet(eAudioADFlag));
    #ifdef CC_AUD_AD_MIX_BYPASS
    vWriteDspShmBYTE (u1DspId, B_ADFLAG , (uReadDspShmBYTE(u1DspId, B_ADFLAG)) | 0x10);
    #endif


    // Limiter mode
    vWriteDspShmWORD (u1DspId, W_LIMITER_MODE, GetAudioLimiterMode());
#if defined(CC_AUD_DSP_SUPPORT_AUDYSSEY_TEST_MODE)
    vWriteDspShmWORD (u1DspId, W_LIMITER_FLAG, 0x0);
#elif defined(DSP_SUPPORT_SRS_SSHD) || defined(DSP_SUPPORT_SRS_SS3D)
    vWriteDspShmWORD (u1DspId, W_LIMITER_FLAG, 0x0); //limiter[5] before volume compensate
#elif defined(DSP_SUPPORT_SRS_PURESOUND)
    vWriteDspShmWORD (u1DspId, W_LIMITER_FLAG, 0x0); //
#else
    vWriteDspShmWORD (u1DspId, W_LIMITER_FLAG, 0xffff);
#endif

    vWriteDspShmDWRD (u1DspId, D_LIMITER_FIX_THRESHOLD, GetAudioLimiterThreshold()); // default is 0x100000
#ifdef CC_RATIO_LIMITER
    vWriteDspShmDWRD (u1DspId, D_LIMITER_GAIN_RATIO, 0x07fffff); // default is 0x7fffff
#endif
#ifdef CC_AUD_DRC_V20
    vWriteDspShmDWRD (u1DspId, D_LIMITER_ATTACK_RATE, 0x200);  // 2 dB/ms, AQ?
    vWriteDspShmDWRD (u1DspId, D_LIMITER_RELEASE_RATE, 0x14);  // 0.08 dB/ms, AQ?
    vWriteDspShmDWRD (u1DspId, D_LIMITER_GAIN_RATIO, 0x0); // default is 0x0

    // DRC
    vWriteDspShmWORD (u1DspId, W_DRC_FLAG, 0x0); // default is disabled
    vWriteDspShmWORD (u1DspId, W_DRC_FC, 30); // 30 = 390 Hz
    vWriteDspShmDWRD (u1DspId, D_DRC_TARGET_LEVEL, 0x20000); // -12dB
    vWriteDspShmDWRD (u1DspId, D_DRC_ATTACK_RATE, 0x200);
    vWriteDspShmDWRD (u1DspId, D_DRC_RELEASE_RATE, 0x14);
    vWriteDspShmDWRD (u1DspId, D_DRC_RATIO, 0x0);
    vWriteDspShmDWRD (u1DspId, D_DRC_SILENCE_LEVEL, 0x10); // -96dB
    vWriteDspShmDWRD (u1DspId, D_DRC_MAX_EXPAND, 0x300); // 3dB
    vWriteDspShmDWRD (u1DspId, D_DRC_HYSTERESIS, 0x0); // 0dB
    vWriteDspShmDWRD (u1DspId, D_DRC_HOLD, 0x0); // 0ms
    vWriteDspShmDWRD (u1DspId, D_DRC_TARGET_LEVEL_1, 0x20000); // -12dB
    vWriteDspShmDWRD (u1DspId, D_DRC_ATTACK_RATE_1, 0x200);
    vWriteDspShmDWRD (u1DspId, D_DRC_RELEASE_RATE_1, 0x14);
    vWriteDspShmDWRD (u1DspId, D_DRC_RATIO_1, 0x0);
    vWriteDspShmDWRD (u1DspId, D_DRC_SILENCE_LEVEL_1, 0x10); // -96dB
    vWriteDspShmDWRD (u1DspId, D_DRC_MAX_EXPAND_1, 0x300); // 3dB
    vWriteDspShmDWRD (u1DspId, D_DRC_HYSTERESIS_1, 0x0); // 0dB
    vWriteDspShmDWRD (u1DspId, D_DRC_HOLD_1, 0x0); // 0ms

    // Limiter post drc mode
    vWriteDspShmDWRD (u1DspId, W_DRC_LIMITER_POST_DRC_MODE, 0x1);//default is ON
#endif
#ifdef CC_AUD_HPF_SUPPORT
#if defined(DSP_SUPPORT_SRS_SSHD) || defined(DSP_SUPPORT_SRS_SS3D)
    // Audio post-processing initialization
    vWriteDspShmBYTE(u1DspId, B_HPF_ENABLE, 1);
    vWriteDspShmWORD(u1DspId, W_HPF_FC, DRVCUST_OptGet(eAudioMtkHpfFc));  // Fc = 100Hz
    vWriteDspShmWORD(u1DspId, W_HPF2_FC, DRVCUST_OptGet(eAudioMtkHpf2Fc));  // Fc = 120Hz
#else
    // Audio post-processing initialization
    vWriteDspShmBYTE(u1DspId, B_HPF_ENABLE, 1);
    vWriteDspShmWORD(u1DspId, W_HPF_FC, DRVCUST_OptGet(eAudioMtkHpfFc));  // Fc = 100Hz
    vWriteDspShmWORD(u1DspId, W_HPF2_FC, DRVCUST_OptGet(eAudioMtkHpf2Fc));  // Fc = 120Hz
#endif
#endif

    // PEQ relative setting
#ifdef SUPPORT_FLASH_AQ
	vWriteDspShmBYTE(u1DspId, B_PEQ_ENABLE, rAudFlashAQ.u4PEQFlag);
#else
#ifdef CC_AUD_PEQ_INIT_ON
    vWriteDspShmBYTE(u1DspId, B_PEQ_ENABLE, 1);
#else
    if ((UINT8)DRVCUST_OptGet(eAudioPeqEnable) != 0)
    {
        vWriteDspShmBYTE(u1DspId, B_PEQ_ENABLE, 1);
    }
	else
	{
        vWriteDspShmBYTE(u1DspId, B_PEQ_ENABLE, 0);
	}

#endif
#endif


#ifdef CC_AUD_SPEAKER_HEIGHT
#ifndef CC_AUD_SPEAKER_HEIGHT_MODE
    vWriteDspShmBYTE(u1DspId, B_SPH_ENABLE, 0);
#else
    vWriteDspShmBYTE(u1DspId, B_SPH_MODE, 0);
#endif
#endif

#ifdef SUPPORT_FLASH_AQ
    vWriteDspShmWORD(u1DspId, W_PEQ_SET0_FC  , rAudFlashAQ.au4PEQFrequency[0]);
    vWriteDspShmWORD(u1DspId, W_PEQ_SET1_FC  , rAudFlashAQ.au4PEQFrequency[1]);
    vWriteDspShmWORD(u1DspId, W_PEQ_SET2_FC  , rAudFlashAQ.au4PEQFrequency[2]);
    vWriteDspShmWORD(u1DspId, W_PEQ_SET3_FC  , rAudFlashAQ.au4PEQFrequency[3]);
    vWriteDspShmWORD(u1DspId, W_PEQ_SET4_FC  , rAudFlashAQ.au4PEQFrequency[4]);
    vWriteDspShmWORD(u1DspId, W_PEQ_SET5_FC  , rAudFlashAQ.au4PEQFrequency[5]);
    vWriteDspShmWORD(u1DspId, W_PEQ_SET6_FC  , rAudFlashAQ.au4PEQFrequency[6]);
    vWriteDspShmWORD(u1DspId, W_PEQ_SET7_FC  , rAudFlashAQ.au4PEQFrequency[7]);
    vWriteDspShmWORD(u1DspId, W_PEQ_SET8_FC  , rAudFlashAQ.au4PEQFrequency[8]);

    for (i=0; i<8; i++)
    {
        vWriteDspShmDWRD(u1DspId, D_PEQ_SET0_GAIN + i*8, rAudFlashAQ.au4PEQGain[i]);
        vWriteDspShmWORD(u1DspId, W_PEQ_SET0_BW + i*8, rAudFlashAQ.au4PEQBW[i]);
    }
    vWriteDspShmDWRD(u1DspId, D_PEQ_SET8_GAIN, rAudFlashAQ.au4PEQGain[8]);
    vWriteDspShmWORD(u1DspId, W_PEQ_SET8_BW, rAudFlashAQ.au4PEQBW[8]);
#else
    // FIXME
    vWriteDspShmWORD(u1DspId, W_PEQ_SET0_FC  , 50);
    vWriteDspShmWORD(u1DspId, W_PEQ_SET1_FC  , 100);
    vWriteDspShmWORD(u1DspId, W_PEQ_SET2_FC  , 200);
    vWriteDspShmWORD(u1DspId, W_PEQ_SET3_FC  , 300);
    vWriteDspShmWORD(u1DspId, W_PEQ_SET4_FC  , 1000);
    vWriteDspShmWORD(u1DspId, W_PEQ_SET5_FC  , 3000);
    vWriteDspShmWORD(u1DspId, W_PEQ_SET6_FC  , 5000);
    vWriteDspShmWORD(u1DspId, W_PEQ_SET7_FC  , 15000);
    vWriteDspShmWORD(u1DspId, W_PEQ_SET8_FC  , 18000);

    for (i=0; i<8; i++)
    {
        vWriteDspShmDWRD(u1DspId, D_PEQ_SET0_GAIN + i*8, 0x200000);
   	    vWriteDspShmWORD(u1DspId, W_PEQ_SET0_BW + i*8, 500);
    }
    vWriteDspShmDWRD(u1DspId, D_PEQ_SET8_GAIN, 0x200000);
    vWriteDspShmWORD(u1DspId, W_PEQ_SET8_BW, 500);
#ifdef CC_AUD_PEQ_LR
    vWriteDspShmWORD(u1DspId, W_PEQ_SET0_FC_R	, 50);
    vWriteDspShmWORD(u1DspId, W_PEQ_SET1_FC_R	, 100);
    vWriteDspShmWORD(u1DspId, W_PEQ_SET2_FC_R	, 200);
    vWriteDspShmWORD(u1DspId, W_PEQ_SET3_FC_R	, 300);
    vWriteDspShmWORD(u1DspId, W_PEQ_SET4_FC_R	, 1000);
    vWriteDspShmWORD(u1DspId, W_PEQ_SET5_FC_R	, 3000);
    vWriteDspShmWORD(u1DspId, W_PEQ_SET6_FC_R	, 5000);
    vWriteDspShmWORD(u1DspId, W_PEQ_SET7_FC_R	, 15000);
    vWriteDspShmWORD(u1DspId, W_PEQ_SET8_FC_R	, 18000);
    
    for (i=0; i<8; i++)
    {
    	vWriteDspShmDWRD(u1DspId, D_PEQ_SET0_GAIN_R + i*8, 0x200000);
    	vWriteDspShmWORD(u1DspId, W_PEQ_SET0_BW_R + i*8, 500);
    }
    vWriteDspShmDWRD(u1DspId, D_PEQ_SET8_GAIN_R, 0x200000);
    vWriteDspShmWORD(u1DspId, W_PEQ_SET8_BW_R, 500);
#endif
#endif

#ifdef CC_AUD_VBASS_SUPPORT
    // Virtual Bass
    vWriteDspShmBYTE(u1DspId, B_VIR_BASS_ENABLE, 0);
    vWriteDspShmDWRD(u1DspId, D_VIR_BASS_GAIN, 0x1000);
    vWriteDspShmDWRD(u1DspId, D_VIR_BASS_GAIN_2, 0x800);
#endif

#ifdef CC_AUD_DSP_SUPPORT_CDNOTCH
    vWriteDspShmBYTE(u1DspId, B_CDNOTCH_FLAG, 0);
    vWriteDspShmBYTE(u1DspId, B_CDNOTCH_USER_Q, 0);
    vWriteDspShmWORD(u1DspId, W_CDNOTCH_USER_FC, 0);
#endif

#ifdef  KARAOKE_SUPPORT
    vWriteDspShmBYTE(u1DspId, B_KEYSHIFT_FLAG, 0);
    vWriteDspShmBYTE(u1DspId, B_KEYSHIFT_KEY, 0);
#endif

#ifdef CC_AUD_CLIPPER_SUPPORT
    // Clipper enable
    vWriteDspShmBYTE(u1DspId, B_CLIPPER_ENABLE, 0);

    vWriteDspShmDWRD(u1DspId, D_CLIPPER_C_VALUE,   CLIPPER_DEFAULT_VALUE);
    vWriteDspShmDWRD(u1DspId, D_CLIPPER_L_VALUE,   CLIPPER_DEFAULT_VALUE);
    vWriteDspShmDWRD(u1DspId, D_CLIPPER_R_VALUE,   CLIPPER_DEFAULT_VALUE);
    vWriteDspShmDWRD(u1DspId, D_CLIPPER_LS_VALUE,  CLIPPER_DEFAULT_VALUE);
    vWriteDspShmDWRD(u1DspId, D_CLIPPER_RS_VALUE,  CLIPPER_DEFAULT_VALUE);
    vWriteDspShmDWRD(u1DspId, D_CLIPPER_SW_VALUE,  CLIPPER_DEFAULT_VALUE);
    vWriteDspShmDWRD(u1DspId, D_CLIPPER_CH7_VALUE, CLIPPER_DEFAULT_VALUE);
    vWriteDspShmDWRD(u1DspId, D_CLIPPER_CH8_VALUE, CLIPPER_DEFAULT_VALUE);
    vWriteDspShmDWRD(u1DspId, D_CLIPPER_CH9_VALUE, CLIPPER_DEFAULT_VALUE);
    vWriteDspShmDWRD(u1DspId, D_CLIPPER_CH10_VALUE,CLIPPER_DEFAULT_VALUE);
#endif

#endif //DSP_SUPPORT_NPTV

#ifdef CC_AUD_USE_NVM
    AUD_NVM_Init_From_EEPROM();
#endif  //CC_AUD_USE_NVM

    //DDCO flag
    vWriteDspShmBYTE (u1DspId, B_DDCO_FLAG,0x0);
    vWriteDspShmBYTE (u1DspId, B_DDCO_AGC_FLAG,0x0);
    vWriteDspShmBYTE (u1DspId, B_DDCO_LFE_LPF_FLAG,0x0);

    // DTS flag
    vWriteDspShmBYTE (u1DspId, B_DTS_ENCODER_FLAG, 0x0);
    vWriteDspShmBYTE (u1DspId, B_DTS_ENCODER_AMODE, 0x0);

//#ifdef ENABLE_AUDIO_RECORD_SUPPORT
    // upload data
    vWriteDspShmWORD (u1DspId, W_UPLOAD_DATA_FLAG, 0x0);
    // Block Number     PCM=128,     MP3=128
    vWriteDspShmBYTE (u1DspId, B_UPLOAD_BLOCK_NUM, 128);
    // Block Size,      PCM=0x400,  MP3=0x3C0(320kbps)/0x240(192kbps)/0x180(128kbps)
    vWriteDspShmWORD (u1DspId, W_UPLOAD_BLOCK_SIZE, 0x400)
    vWriteDspShmBYTE (u1DspId, B_UPLOAD_BLK_INT_CNT, 16);
	vWriteDspShmWORD (u1DspId, W_BLUETOOTH_DATA_FLAG, 0x0);
	// Block Number 	PCM=128,	 MP3=128
	vWriteDspShmBYTE (u1DspId, B_BLUETOOTH_BLOCK_NUM, 128);
	// Block Size,		PCM=0x400,	MP3=0x3C0(320kbps)/0x240(192kbps)/0x180(128kbps)
	vWriteDspShmWORD (u1DspId, W_BLUETOOTH_BLOCK_SIZE, 0x400)
	vWriteDspShmBYTE (u1DspId, B_BLUETOOTH_BLK_INT_CNT, 16);
//#endif

#ifdef CC_AUD_DSP_SUPPORT_AUDYSSEY
    //ADV flag
    vWriteDspShmBYTE (u1DspId, B_ADV_FLAG,0x0); //ON/OFF
    vWriteDspShmDWRD (u1DspId, D_ADV_chCalbGain_L,DRVCUST_OptGet(eAudioAdvChCalbGain_L));//0x8fb: dB,(10.14), FOR 2.1V=0dB
    vWriteDspShmDWRD (u1DspId, D_ADV_chCalbGain_R,DRVCUST_OptGet(eAudioAdvChCalbGain_R));//0x44f: dB,(10.14)

    vWriteDspShmBYTE (u1DspId, B_ABX_ENABLE, 0);      //Default On for testing
    vWriteDspShmBYTE (u1DspId, B_ABX_FILTER_SET, DRVCUST_OptGet(eAudioAbxFilterSet)); //0: default use FILTER Set 1
    vWriteDspShmBYTE (u1DspId, B_ABX_DRY_GAIN_INDX, DRVCUST_OptGet(eAudioAbxDryGainIndx)); //0: default 0dB
    vWriteDspShmBYTE (u1DspId, B_ABX_WET_GAIN_INDX, DRVCUST_OptGet(eAudioAbxWetGainIndx)); //6: default 3dB

    vWriteDspShmBYTE (u1DspId, B_AEQ_FLAG,0);
#endif

    vWriteDspShmBYTE (u1DspId, B_SBCENC_SAMPLE_FREQ,3);
    vWriteDspShmBYTE (u1DspId, B_SBCENC_BLK_NUM_1,(16-1));
    #if 1
    //for 48K/Stereo, frame_len = 12 + 2 * bitpool = 128 bytes (per 128 samples/2 channel)
    vWriteDspShmBYTE (u1DspId, B_SBCENC_CH_NUM_1,(2-1));
    vWriteDspShmBYTE (u1DspId, B_SBCENC_BITPOOL,58); //53
    #else
    //for 48K/Mono, frame_len = 8 + 2 * bitpool = 64 bytes (per 128 samples/1 channel)
    vWriteDspShmBYTE (u1DspId, B_SBCENC_CH_NUM_1,(1-1));
    vWriteDspShmBYTE (u1DspId, B_SBCENC_BITPOOL,28);
    #endif

#ifdef CC_VOCAL_MUTE
    /// Vocal Cancel
    vWriteDspShmBYTE(u1DspId, B_VC_DIFF_PRE,     0x00);
#endif

#if 1//added by ling for DTS
    vWriteDspShmBYTE (u1DspId, B_DTS_BC_LFE_MIXTO_FRONT,0x0);
    vWriteDspShmBYTE (u1DspId, B_DTS_BC_DRC,0x9);
    vWriteDspShmBYTE (u1DspId, B_DTS_BC_Dialnrom,0x9);

    vWriteDspShmBYTE (u1DspId, B_DTS_BC_LFE_MIXTO_FRONT_DEC2,0x0);
    vWriteDspShmBYTE (u1DspId, B_DTS_BC_DRC_DEC2,0x9);
    vWriteDspShmBYTE (u1DspId, B_DTS_BC_Dialnrom_DEC2,0x9);

    vWriteDspShmDWRD(u1DspId, D_DTS_BC_DRC_LOW,   0x2AAAA7);
    vWriteDspShmDWRD(u1DspId, D_DTS_BC_DRC_MEDIUM,   0x55554e);
    vWriteDspShmDWRD(u1DspId, D_DTS_BC_DRC_HIGH,   0x7fffff);
    vWriteDspShmDWRD(u1DspId, D_DTS_BC_Dialnorm_LOW,   0x44444);
    vWriteDspShmDWRD(u1DspId, D_DTS_BC_Dialnorm_MEDIUM,   0x88888);
    vWriteDspShmDWRD(u1DspId, D_DTS_BC_Dialnorm_HIGH,   0x7fffff);
#endif

    // g726dec
    vWriteDspShmWORD (u1DspId, W_G726_FRAME,256);
    vWriteDspShmBYTE (u1DspId, B_G726_CH,1);
    vWriteDspShmBYTE (u1DspId, B_G726_RATE,32);          // rate: 40, 32, 24, 16, default:32
    vWriteDspShmBYTE (u1DspId, B_G726_LAW,0);            // 0:u-law, 1:a-law, 2: linear, unused for PCM in/out
    vWriteDspShmBYTE (u1DspId, B_G726_SRATE, 0x4);       // only support 8kHz, 4:SFREQ_8K
    vWriteDspShmWORD (u1DspId, W_G726_FRAME_DEC2,256);
    vWriteDspShmBYTE (u1DspId, B_G726_CH_DEC2,1);
    vWriteDspShmBYTE (u1DspId, B_G726_RATE_DEC2,32);
    vWriteDspShmBYTE (u1DspId, B_G726_LAW_DEC2,0);       // 0:u-law, 1:a-law, 2: linear, unused for PCM in/out
    vWriteDspShmBYTE (u1DspId, B_G726_SRATE_DEC2, 0x4);  // only support 8kHz, 4:SFREQ_8K
    vWriteDspShmWORD (u1DspId, W_G726_FRAME_DEC3,256);
    vWriteDspShmBYTE (u1DspId, B_G726_CH_DEC3,1);
    vWriteDspShmBYTE (u1DspId, B_G726_RATE_DEC3,32);
    vWriteDspShmBYTE (u1DspId, B_G726_LAW_DEC3,0);       // 0:u-law, 1:a-law, 2: linear, unused for PCM in/out
    vWriteDspShmBYTE (u1DspId, B_G726_SRATE_DEC3, 0x4);  // only support 8kHz, 4:SFREQ_8K


#ifndef CC_AUD_FAST_STOP_BY_HW_GAIN
    vWriteDspShmWORD (u1DspId, W_SYSTEM_CTRL2, u2ReadDspShmWORD(u1DspId, W_SYSTEM_CTRL2) | 0x0);      //bit2 : 1 for half APLL(147M) ASRC tracking mode 
#else
    vWriteDspShmWORD (u1DspId, W_SYSTEM_CTRL2, u2ReadDspShmWORD(u1DspId, W_SYSTEM_CTRL2) | 0x200);    //bit9 enable FAST_STOP_BY_HW_GAIN
#endif
#ifdef CC_AUD_FIR_SUPPORT
    vWriteDspShmBYTE(u1DspId, B_FIR_ENABLE, 1);
#endif

}

/***************************************************************************
     Function : vDSPInit
  Description : RISC Initialize DSP DRAM Value when Powen On
    Parameter : None
    Return    : None
***************************************************************************/
void vDspInit (UINT8 u1DspId)
{
    UINT16 u2DspData;
    UINT8  iLoopVar;

    //CTRL1 NUM_1
    vWriteDspWORD (u1DspId, ADDR_RC2D_CTRL_1_NUM_1, CTRL_1_NUM_1);

    //Interrupt Control
    u2DspData = u2ReadDspShmWORD (u1DspId, W_INTCTRL);
    u2DspData = u2DspData | RAM_EXT_RISC_INT | TRAP_PRESET_ON ;
    vWriteDspWORD (u1DspId, ADDR_RC2D_INTERRUPT_CTRL, u2DspData);
    // Light add for internal buffer control
    vWriteDspWORD (u1DspId, ADDR_RC2D_SYSTEM_CTRL,    u2ReadDspShmWORD (u1DspId, W_SYSTEM_CTRL));
    vWriteDspWORD (u1DspId, ADDR_RC2D_SYSTEM_CTRL2,    u2ReadDspShmWORD (u1DspId, W_SYSTEM_CTRL2));
    vWriteDspWORD (u1DspId, ADDR_RD2D_INTERNAL_BUF_IN_FLAG, u2ReadDspShmWORD (u1DspId, W_INTERNAL_BUF_IN_FLAG));
    vWriteDspWORD (u1DspId, ADDR_RC2D_SYSTEM_SETUP,   u2ReadDspShmWORD (u1DspId, W_SYSTEM_SETUP));
    vWriteDspWORD (u1DspId, ADDR_RC2D_SYSTEM_SETUP_DEC2, u2ReadDspShmWORD (u1DspId, W_SYSTEM_SETUP_DEC2));
#ifdef CC_MT5391_AUD_3_DECODER
    vWriteDspWORD (u1DspId, ADDR_RC2D_SYSTEM_SETUP_DEC3,
        u2ReadDspShmWORD (u1DspId, W_SYSTEM_SETUP_DEC3));
    vWriteDspWORD (u1DspId, ADDR_RD2D_INTERNAL_BUF2_IN_FLAG, u2ReadDspShmWORD (u1DspId, W_INTERNAL_BUF2_IN_FLAG));
#endif

#ifdef CC_AUD_4_DECODER_SUPPORT
    vWriteDspWORD (u1DspId, ADDR_RC2D_SYSTEM_SETUP_DEC4,
        u2ReadDspShmWORD (u1DspId, W_SYSTEM_SETUP_DEC4));
    vWriteDspWORD (u1DspId, ADDR_RD2D_INTERNAL_BUF3_IN_FLAG, u2ReadDspShmWORD (u1DspId, W_INTERNAL_BUF3_IN_FLAG));
#endif

    //semaphore control
    vWriteDspWORD (u1DspId, ADDR_RC2D_AENV_SEMA_CTRL, u2ReadDspShmWORD (u1DspId, W_AENV_SEMA_CTRL));

    //Volume
    WriteDspCommDram (u1DspId, ADDR_RC2D_MASTER_VOLUME,u4ReadDspShmDWRD (u1DspId, D_VOL));
#ifndef CC_AUD_DDI
    WriteDspCommDram (u1DspId, ADDR_RC2D_MASTER_VOLUME_BACKUP,u4ReadDspShmDWRD (u1DspId, D_VOL));
#endif
    WriteDspCommDram (u1DspId, ADDR_RC2D_MASTER_VOLUME_DEC2,u4ReadDspShmDWRD (u1DspId, D_VOL_DEC2));
    WriteDspCommDram (u1DspId, ADDR_RC2D_MASTER_VOLUME_BACKUP_DEC2,u4ReadDspShmDWRD (u1DspId, D_VOL_DEC2));
#ifdef CC_MT5391_AUD_3_DECODER
    WriteDspCommDram (u1DspId, ADDR_RC2D_MASTER_VOLUME_DEC3,u4ReadDspShmDWRD (u1DspId, D_VOL_DEC3));
    WriteDspCommDram (u1DspId, ADDR_RC2D_MASTER_VOLUME_BACKUP_DEC3,u4ReadDspShmDWRD (u1DspId, D_VOL_DEC3));
#endif
#ifdef CC_AUD_4_DECODER_SUPPORT
    WriteDspCommDram (u1DspId, ADDR_RC2D_MASTER_VOLUME_DEC4,u4ReadDspShmDWRD (u1DspId, D_VOL_DEC4));
    WriteDspCommDram (u1DspId, ADDR_RC2D_MASTER_VOLUME_BACKUP_DEC4,u4ReadDspShmDWRD (u1DspId, D_VOL_DEC4));
#endif

    //Input Src Gain
    WriteDspCommDram (u1DspId, ADDR_RC2D_INPUT_SRC_GAIN,u4ReadDspShmDWRD (u1DspId, D_INSRC_GAIN));
    WriteDspCommDram (u1DspId, ADDR_RC2D_INPUT_SRC_GAIN_DEC2,u4ReadDspShmDWRD (u1DspId, D_INSRC_GAIN_DEC2));
#ifdef CC_MT5391_AUD_3_DECODER
    WriteDspCommDram (u1DspId, ADDR_RC2D_INPUT_SRC_GAIN_DEC3,u4ReadDspShmDWRD (u1DspId, D_INSRC_GAIN_DEC3));
#endif
#ifdef CC_AUD_4_DECODER_SUPPORT
    WriteDspCommDram (u1DspId, ADDR_RC2D_INPUT_SRC_GAIN_DEC4,u4ReadDspShmDWRD (u1DspId, D_INSRC_GAIN_DEC4));
#endif

    // Speed
    WriteDspCommDram (u1DspId, ADDR_RC2D_SPEED, 0);
    WriteDspCommDram (u1DspId, ADDR_RC2D_SPEED_DEC2, 0);
#ifdef DSP_SUPPORT_DUAL_DECODE
    WriteDspCommDram (u1DspId, ADDR_RC2D_SPEED_DEC3, 0);
#endif
    //Channel Trim and Dialogue Gain
    for (iLoopVar = 0; iLoopVar < 8; iLoopVar++)
    {
        WriteDspCommDram (u1DspId, (ADDR_RC2D_CH_TRIM + iLoopVar),
            u4ReadDspShmDWRD (u1DspId, (D_TRIM_C + (iLoopVar * 4))));
    }
	  WriteDspCommDram (u1DspId, ADDR_RC2D_UPLOAD_GAIN_MPY, u4ReadDspShmDWRD (u1DspId, D_UPLOAD_VOL));

    if ((UINT8)DRVCUST_OptGet(eAudioDownmixPosition) == DOWNMIX_CH_BEFORE_AVC)
    {
        UINT32 u4TrimValue;

        u4TrimValue = u4ReadDspShmDWRD (u1DspId, D_TRIM_CH9) << 3;
        if (u4TrimValue > 0x07fffff)
        {
            u4TrimValue = 0x07fffff;
        }

        WriteDspCommDram (u1DspId, ADDR_RC2D_CH_TRIM_CH9, u4TrimValue);

        u4TrimValue = u4ReadDspShmDWRD (u1DspId, D_TRIM_CH10) << 3;
        if (u4TrimValue > 0x07fffff)
        {
            u4TrimValue = 0x07fffff;
        }

        WriteDspCommDram (u1DspId, ADDR_RC2D_CH_TRIM_CH10, u4TrimValue);
    }
    else
    {
        WriteDspCommDram (u1DspId, ADDR_RC2D_CH_TRIM_CH9,   u4ReadDspShmDWRD (u1DspId, D_TRIM_CH9));
        WriteDspCommDram (u1DspId, ADDR_RC2D_CH_TRIM_CH10,  u4ReadDspShmDWRD (u1DspId, D_TRIM_CH10));
    }

    WriteDspCommDram (u1DspId, ADDR_RC2D_CH_TRIM_LFE,   u4ReadDspShmDWRD (u1DspId, D_TRIM_LFE));
    WriteDspCommDram (u1DspId, ADDR_RC2D_DIALOGUE_GAIN, u4ReadDspShmDWRD (u1DspId, D_DIALOGUE));
    WriteDspCommDram (u1DspId, ADDR_RC2D_DIALOGUE_GAIN_DEC2, u4ReadDspShmDWRD (u1DspId, D_DIALOGUE_DEC2));

    WriteDspCommDram (u1DspId, ADDR_RC2D_CH_TRIM_DEC2_L,u4ReadDspShmDWRD (u1DspId, D_TRIM_L_DEC2));
    WriteDspCommDram (u1DspId, ADDR_RC2D_CH_TRIM_DEC2_R,u4ReadDspShmDWRD (u1DspId, D_TRIM_R_DEC2));
    WriteDspCommDram (u1DspId, ADDR_RC2D_CH_TRIM_DEC2_LFE,u4ReadDspShmDWRD (u1DspId, D_TRIM_LFE_DEC2));

    //FOR DSP PAN FUNCTION INITIAL VALUES, CC_AUD_PANFADE_SUPPORT
    WriteDspCommDram (u1DspId, ADDR_RC2D_CH_TRIM_DEC3_L, u4ReadDspShmDWRD (u1DspId, D_TRIM_L_DEC3));
    WriteDspCommDram (u1DspId, ADDR_RC2D_CH_TRIM_DEC3_R, u4ReadDspShmDWRD (u1DspId, D_TRIM_R_DEC3));
    WriteDspCommDram (u1DspId, ADDR_RC2D_DIALOGUE_GAIN_DEC3, u4ReadDspShmDWRD (u1DspId, D_DIALOGUE_DEC3));
    WriteDspCommDram (u1DspId, ADDR_RC2D_AD_FADE_VALUE, u4ReadDspShmDWRD (u1DspId, D_FADEVALUE));

    //WriteDspCommDram (ADDR_RC2D_CH_TRIM_DEC4_L, u4ReadDspShmDWRD (D_TRIM_L_DEC4));
    //WriteDspCommDram (ADDR_RC2D_CH_TRIM_DEC4_R, u4ReadDspShmDWRD (D_TRIM_R_DEC4));

    WriteDspCommDram (u1DspId, ADDR_RC2D_L_CH_VOL, u4ReadDspShmDWRD(u1DspId, D_VOL_L));
    WriteDspCommDram (u1DspId, ADDR_RC2D_R_CH_VOL, u4ReadDspShmDWRD(u1DspId, D_VOL_R));

    WriteDspCommDram (u1DspId, ADDR_RC2D_L_CH_LAST_VOL, u4ReadDspShmDWRD(u1DspId, D_VOL_L));
    WriteDspCommDram (u1DspId, ADDR_RC2D_R_CH_LAST_VOL, u4ReadDspShmDWRD(u1DspId, D_VOL_R));
    
    //Other Information
    /* ------- Setup MixSound Coefficient ------- */

    for (iLoopVar = 0; iLoopVar < (3*5); iLoopVar++)
    {
        WriteDspCommDram (u1DspId, ADDR_RC2D_DRAM_MIXSOUND_UPX8_COEFF + iLoopVar,
            MIXSOUND_COEFF[iLoopVar]);
    }

    // system master information
    WriteDspCommDram (u1DspId, ADDR_RC2D_STC_DIFF_LOBOUND,
        u4ReadDspShmDWRD (u1DspId, D_STC_DIFF_LO));
    WriteDspCommDram (u1DspId, ADDR_RC2D_STC_DIFF_HIBOUND,
        u4ReadDspShmDWRD (u1DspId, D_STC_DIFF_HI));
    WriteDspCommDram (u1DspId, ADDR_RC2D_STC_DIFF_WSBOUND,
        u4ReadDspShmDWRD (u1DspId, D_STC_DIFF_WS));
    WriteDspCommDram (u1DspId, ADDR_RC2D_STC_LEVEL_THRESHOLD,
        u4ReadDspShmDWRD (u1DspId, D_STC_THRESHOLD));
    WriteDspCommDram (u1DspId, ADDR_RC2D_DECODING_STR_PNT,
        u4ReadDspShmDWRD (u1DspId, D_DECODING_STR_PNT));
    WriteDspCommDram (u1DspId, ADDR_RC2D_1ST_PTS_PRS_PNT,
        u4ReadDspShmDWRD (u1DspId, D_1ST_PTS_PRS_PNT));
    vWriteDspWORD (u1DspId, ADDR_RC2D_1ST_PTS_STCH,
        u2ReadDspShmWORD (u1DspId, W_1ST_PTS_STCH));
    vWriteDspWORD (u1DspId, ADDR_RC2D_1ST_PTS_STCL,
        u2ReadDspShmWORD (u1DspId, W_1ST_PTS_STCL));

    WriteDspCommDram (u1DspId, ADDR_RC2D_STC_DIFF_LOBOUND_DEC2,
        u4ReadDspShmDWRD (u1DspId, D_STC_DIFF_LO_DEC2));
    WriteDspCommDram (u1DspId, ADDR_RC2D_STC_DIFF_HIBOUND_DEC2,
        u4ReadDspShmDWRD (u1DspId, D_STC_DIFF_HI_DEC2));
    WriteDspCommDram (u1DspId, ADDR_RC2D_STC_DIFF_WSBOUND_DEC2,
        u4ReadDspShmDWRD (u1DspId, D_STC_DIFF_WS_DEC2));
    WriteDspCommDram (u1DspId, ADDR_RC2D_STC_LEVEL_THRESHOLD_DEC2,
        u4ReadDspShmDWRD (u1DspId, D_STC_THRESHOLD_DEC2));
    WriteDspCommDram (u1DspId, ADDR_RC2D_DECODING_STR_PNT_DEC2,
        u4ReadDspShmDWRD (u1DspId, D_DECODING_STR_PNT_DEC2));
    WriteDspCommDram (u1DspId, ADDR_RC2D_1ST_PTS_PRS_PNT_DEC2,
        u4ReadDspShmDWRD (u1DspId, D_1ST_PTS_PRS_PNT_DEC2));
    vWriteDspWORD (u1DspId, ADDR_RC2D_1ST_PTS_STCH_DEC2,
        u2ReadDspShmWORD (u1DspId, W_1ST_PTS_STCH_DEC2));
    vWriteDspWORD (u1DspId, ADDR_RC2D_1ST_PTS_STCL_DEC2,
        u2ReadDspShmWORD (u1DspId, W_1ST_PTS_STCL_DEC2));

    WriteDspCommDram (u1DspId, ADDR_RC2D_STC_DIFF_LOBOUND_DEC3,
        u4ReadDspShmDWRD (u1DspId, D_STC_DIFF_LO_DEC3));
    WriteDspCommDram (u1DspId, ADDR_RC2D_STC_DIFF_HIBOUND_DEC3,
        u4ReadDspShmDWRD (u1DspId, D_STC_DIFF_HI_DEC3));
    WriteDspCommDram (u1DspId, ADDR_RC2D_STC_DIFF_WSBOUND_DEC3,
        u4ReadDspShmDWRD (u1DspId, D_STC_DIFF_WS_DEC3));
    WriteDspCommDram (u1DspId, ADDR_RC2D_STC_LEVEL_THRESHOLD_DEC3,
        u4ReadDspShmDWRD (u1DspId, D_STC_THRESHOLD_DEC3));
    WriteDspCommDram (u1DspId, ADDR_RC2D_DECODING_STR_PNT_DEC3,
        u4ReadDspShmDWRD (u1DspId, D_DECODING_STR_PNT_DEC3));
    WriteDspCommDram (u1DspId, ADDR_RC2D_1ST_PTS_PRS_PNT_DEC3,
        u4ReadDspShmDWRD (u1DspId, D_1ST_PTS_PRS_PNT_DEC3));
    vWriteDspWORD (u1DspId, ADDR_RC2D_1ST_PTS_STCH_DEC3,
        u2ReadDspShmWORD (u1DspId, W_1ST_PTS_STCH_DEC3));
    vWriteDspWORD (u1DspId, ADDR_RC2D_1ST_PTS_STCL_DEC3,
        u2ReadDspShmWORD (u1DspId, W_1ST_PTS_STCL_DEC3));

    /* ------- Setup IEC ------- */
    IecInfoInit ();

    vWriteDspWORD (u1DspId, ADDR_RC2D_IEC_CHANNEL_CFG0, 0);
    vWriteDspWORD (u1DspId, ADDR_RC2D_IEC_CHANNEL_CFG1, 0);
    vWriteDspWORD (u1DspId, ADDR_RC2D_IEC_BURST_INFO, 0);

    /* ------- Setup Customer Option ------ */
    vCustomOptInit ();

    // Memory backup address
    WriteDspCommDram (u1DspId, ADDR_RC2D_MEMORY_BUFFER_ADDRESS,
        u4ReadDspShmDWRD (u1DspId, D_MEMBACKUPADDR));

#ifdef CC_AUD_AVC_V20
    // New AVC
    vWriteDspWORD (u1DspId, DRAM_DRC2_AVC_FLAG, u2ReadDspShmWORD(u1DspId, W_AVC_FLAG));
    WriteDspCommDram (u1DspId, (DRAM_DRC2_SET_AVC_BAND0_BASE+DRC2_SET_TARGET_LEVEL),
        u4ReadDspShmDWRD (u1DspId, D_AVC_TARGET_LEVEL));   
    WriteDspCommDram (u1DspId, (DRAM_DRC2_SET_AVC_BAND0_BASE+DRC2_SET_ATTACK_RATE),
        u4ReadDspShmDWRD (u1DspId, D_AVC_ATTACK_RATE));     
    WriteDspCommDram (u1DspId, (DRAM_DRC2_SET_AVC_BAND0_BASE+DRC2_SET_RELEASE_RATE),
        u4ReadDspShmDWRD (u1DspId, D_AVC_RELEASE_RATE)); 
    WriteDspCommDram (u1DspId, (DRAM_DRC2_SET_AVC_BAND0_BASE+DRC2_SET_RATIO),
        u4ReadDspShmDWRD (u1DspId, D_AVC_RATIO)); 
    WriteDspCommDram (u1DspId, (DRAM_DRC2_SET_AVC_BAND0_BASE+DRC2_SET_SILENCE_LEVEL),
        u4ReadDspShmDWRD (u1DspId, D_AVC_SILENCE_LEVEL));     
    WriteDspCommDram (u1DspId, (DRAM_DRC2_SET_AVC_BAND0_BASE+DRC2_SET_MAX_EXPAND),
        u4ReadDspShmDWRD (u1DspId, D_AVC_MAX_EXPAND));   
    WriteDspCommDram (u1DspId, (DRAM_DRC2_SET_AVC_BAND0_BASE+DRC2_SET_HYSTERESIS),
        u4ReadDspShmDWRD (u1DspId, D_AVC_POST_GAIN)); 
    vWriteDspWORD (u1DspId, (DRAM_DRC2_SET_AVC_BAND0_BASE+DRC2_SET_HOLD),
        u4ReadDspShmDWRD (u1DspId, D_AVC_HOLD));     
    WriteDspCommDram (u1DspId, (DRAM_DRC2_SET_AVC_BAND1_BASE+DRC2_SET_TARGET_LEVEL),
        u4ReadDspShmDWRD (u1DspId, D_AVC_TARGET_LEVEL_1)); 
    WriteDspCommDram (u1DspId, (DRAM_DRC2_SET_AVC_BAND1_BASE+DRC2_SET_ATTACK_RATE),
        u4ReadDspShmDWRD (u1DspId, D_AVC_ATTACK_RATE_1)); 
    WriteDspCommDram (u1DspId, (DRAM_DRC2_SET_AVC_BAND1_BASE+DRC2_SET_RELEASE_RATE),
        u4ReadDspShmDWRD (u1DspId, D_AVC_RELEASE_RATE_1)); 
    WriteDspCommDram (u1DspId, (DRAM_DRC2_SET_AVC_BAND1_BASE+DRC2_SET_RATIO),
        u4ReadDspShmDWRD (u1DspId, D_AVC_RATIO_1));     
    WriteDspCommDram (u1DspId, (DRAM_DRC2_SET_AVC_BAND1_BASE+DRC2_SET_SILENCE_LEVEL),
        u4ReadDspShmDWRD (u1DspId, D_AVC_SILENCE_LEVEL_1));     
    WriteDspCommDram (u1DspId, (DRAM_DRC2_SET_AVC_BAND1_BASE+DRC2_SET_MAX_EXPAND),
        u4ReadDspShmDWRD (u1DspId, D_AVC_MAX_EXPAND_1)); 
    WriteDspCommDram (u1DspId, (DRAM_DRC2_SET_AVC_BAND1_BASE+DRC2_SET_HYSTERESIS),
        u4ReadDspShmDWRD (u1DspId, D_AVC_POST_GAIN_1));    
    vWriteDspWORD (u1DspId, (DRAM_DRC2_SET_AVC_BAND1_BASE+DRC2_SET_HOLD),
        u4ReadDspShmDWRD (u1DspId, D_AVC_HOLD_1));        
#else
    // Light added for AVC
    WriteDspCommDram (u1DspId, ADDR_RC2D_AVC_TARGET_LEVEL,
        (UINT16)(u2ReadDspShmWORD (u1DspId, W_AVC_TARGET_LEV))<<8);
    WriteDspCommDram (u1DspId, ADDR_RC2D_AVC_SILENCE_LEVEL,
        u2ReadDspShmWORD (u1DspId, W_AVC_SILENCE_LEV)<<8);
    WriteDspCommDram (u1DspId, ADDR_RC2D_AVC_MAX_GAIN_UP,
        u2ReadDspShmWORD (u1DspId, W_AVC_MAX_GAIN_UP)<<8);
    WriteDspCommDram (u1DspId, ADDR_RC2D_AVC_MAX_GAIN_DOWN,
        u2ReadDspShmWORD (u1DspId, W_AVC_MAX_GAIN_DOWN)<<8);
    WriteDspCommDram (u1DspId, ADDR_RC2D_AVC_FLAG,
        u2ReadDspShmWORD (u1DspId, W_AVC_FLAG)<<8);
    WriteDspCommDram (u1DspId, ADDR_RC2D_AVC_ATTACK_THRES,
        u2ReadDspShmWORD (u1DspId, W_AVC_ATTACK_THRES)<<8);
    WriteDspCommDram (u1DspId, ADDR_RC2D_AVC_ADJUST_RATE,
        u2ReadDspShmWORD (u1DspId, W_AVC_ADJUST_RATE));
    WriteDspCommDram (u1DspId, ADDR_RC2D_AVC_UI_ADJUST_RATE,
        u2ReadDspShmWORD (u1DspId, W_DSP_AVC_UI_ADJUST_RATE));
#endif

    // Audio Output Bank Number
    vWriteDspWORD (u1DspId, ADDR_RC2D_AOUT_BANK_576_NUM,
        uReadDspShmBYTE (u1DspId, B_BANK576NUM));
    vWriteDspWORD (u1DspId, ADDR_RC2D_AOUT_BANK_384_NUM,
        uReadDspShmBYTE (u1DspId, B_BANK384NUM));
    vWriteDspWORD (u1DspId, ADDR_RC2D_AOUT_BANK_320_NUM,
        uReadDspShmBYTE (u1DspId, B_BANK320NUM));
    vWriteDspWORD (u1DspId, ADDR_RC2D_AOUT_BANK_256_NUM,
        uReadDspShmBYTE (u1DspId, B_BANK256NUM));

    // Audio Output Bank Number for second decoder
    vWriteDspWORD (u1DspId, ADDR_RC2D_AOUT_BANK_576_NUM_DEC2,
        uReadDspShmBYTE (u1DspId, B_BANK576NUM_DEC2));
    vWriteDspWORD (u1DspId, ADDR_RC2D_AOUT_BANK_384_NUM_DEC2,
        uReadDspShmBYTE (u1DspId, B_BANK384NUM_DEC2));
    vWriteDspWORD (u1DspId, ADDR_RC2D_AOUT_BANK_320_NUM_DEC2,
        uReadDspShmBYTE (u1DspId, B_BANK320NUM_DEC2));
    vWriteDspWORD (u1DspId, ADDR_RC2D_AOUT_BANK_256_NUM_DEC2,
        uReadDspShmBYTE (u1DspId, B_BANK256NUM_DEC2));

#ifdef CC_MT5391_AUD_3_DECODER
    //Audio Output Bank Number for 3rd decoder
    vWriteDspWORD (u1DspId, ADDR_RC2D_AOUT_BANK_576_NUM_DEC3,
        uReadDspShmBYTE (u1DspId, B_BANK576NUM_DEC3));
    vWriteDspWORD (u1DspId, ADDR_RC2D_AOUT_BANK_384_NUM_DEC3,
        uReadDspShmBYTE (u1DspId, B_BANK384NUM_DEC3));
    vWriteDspWORD (u1DspId, ADDR_RC2D_AOUT_BANK_320_NUM_DEC3,
        uReadDspShmBYTE (u1DspId, B_BANK320NUM_DEC3));
    vWriteDspWORD (u1DspId, ADDR_RC2D_AOUT_BANK_256_NUM_DEC3,
        uReadDspShmBYTE (u1DspId, B_BANK256NUM_DEC3));
#endif

    //Audio Output Error Mute Bank Number
#ifdef DSP_ERROR_MUTE_ON
    // DSP will beep when under flow
    vWriteDspWORD (u1DspId, ADDR_RC2D_AOUT_ERROR_MUTE_BANK_NUM, 0x3ff);
    vWriteDspWORD (u1DspId, ADDR_RC2D_AOUT_ERROR_MUTE_BANK_NUM_DEC2, 0x3ff);
#else
    vWriteDspWORD (u1DspId, ADDR_RC2D_AOUT_ERROR_MUTE_BANK_NUM,
        u2ReadDspShmWORD (u1DspId, W_ERRORMUTEBANK));
    vWriteDspWORD (u1DspId, ADDR_RC2D_AOUT_ERROR_MUTE_BANK_NUM_DEC2,
        u2ReadDspShmWORD (u1DspId, W_ERRORMUTEBANK_DEC2));
#endif // DSP_ERROR_MUTE_ON

    //Volume LEVEL UP/DOWN ORDER
    vWriteDspWORD (u1DspId, ADDR_RC2D_VOLUME_LEVEL_UP_ORDER,
        uReadDspShmBYTE (u1DspId, B_VOLUPORDER));
    vWriteDspWORD (u1DspId, ADDR_RC2D_VOLUME_LEVEL_DOWN_ORDER,
        uReadDspShmBYTE (u1DspId, B_VOLDOWNORDER));
    //Soft Mute (Max. 7)
    vWriteDspWORD (u1DspId, ADDR_RC2D_VOLUME_SOFT_MUTE_ORDER,
        uReadDspShmBYTE (u1DspId, B_SOFTMUTEORDER));

    //Volume LEVEL UP/DOWN ORDER for second decoder
    vWriteDspWORD (u1DspId, ADDR_RC2D_VOLUME_LEVEL_UP_ORDER_DEC2,
        uReadDspShmBYTE (u1DspId, B_VOLUPORDER_DEC2));
    vWriteDspWORD (u1DspId, ADDR_RC2D_VOLUME_LEVEL_DOWN_ORDER_DEC2,
        uReadDspShmBYTE (u1DspId, B_VOLDOWNORDER_DEC2));
    //Soft Mute (Max. 7)
    vWriteDspWORD (u1DspId, ADDR_RC2D_VOLUME_SOFT_MUTE_ORDER_DEC2,
        uReadDspShmBYTE (u1DspId, B_SOFTMUTEORDER_DEC2));

    //Volume Smooth (Max. 15)
    vWriteDspWORD (u1DspId, ADDR_RC2D_VOLUME_SMOOTH_ORDER, (UINT16) 8); // Light modified for AVC, order should be <= 8
    //Volume Smooth (Max. 15)
    vWriteDspWORD (u1DspId, ADDR_RC2D_VOLUME_SMOOTH_ORDER_DEC2, (UINT16) 8);
 #ifdef CC_MT5391_AUD_3_DECODER
    vWriteDspWORD (u1DspId, ADDR_RC2D_VOLUME_SMOOTH_ORDER_DEC3, (UINT16) 8);
#endif
#ifdef CC_AUD_4_DECODER_SUPPORT
    vWriteDspWORD (u1DspId, ADDR_RC2D_VOLUME_SMOOTH_ORDER_DEC4, (UINT16) 8);
#endif
    vWriteDspWORD (u1DspId, ADDR_RC2D_EFFVOL_SMOOTH_ORDER_DEC3, (UINT16) 10);

    vWriteDspWORD (u1DspId, ADDR_RC2D_CH_DELAY_DEC3, 0);

    //DAC Bit Resolution
    vWriteDspWORD (u1DspId, ADDR_RC2D_DAC_BIT_RESOLUTION,
        (UINT16) (uReadDspShmBYTE (u1DspId, B_DACBIT) & 0x1f));
    vWriteDspWORD (u1DspId, ADDR_RC2D_DAC_BIT_RESOLUTION_DEC2,
        (UINT16) (uReadDspShmBYTE (u1DspId, B_DACBIT_DEC2) & 0x1f));
    //Audio Mute Threshold
    WriteDspCommDram (u1DspId, ADDR_RC2D_AUDIO_MUTE_THRESHOLD, 0x0);
    WriteDspCommDram (u1DspId, ADDR_RC2D_AUDIO_MUTE_THRESHOLD_DEC2, 0x0);

    //Set Mute Mode (Not Mute)

    //Prologic II
    vWriteDspWORD (u1DspId, ADDR_RC2D_PRO_LOGIC_II_CONFIG,
        u2ReadDspShmWORD (u1DspId, W_PLIICONFIG));
    vWriteDspWORD (u1DspId, ADDR_RC2D_PRO_LOGIC_II_MODE, u2ReadDspShmWORD (u1DspId, W_PLIIMODE));
    //Spectrum Speed
    vWriteDspWORD (u1DspId, ADDR_RC2D_SPECTRUM_HIGH_ORDER,
        uReadDspShmBYTE (u1DspId, B_SPECTRUMUP));
    vWriteDspWORD (u1DspId, ADDR_RC2D_SPECTRUM_LOW_ORDER,
        uReadDspShmBYTE (u1DspId, B_SPECTRUMDOWN));
    //Trap Setting
    vWriteDspWORD (u1DspId, ADDR_RC2D_TRAP_BPEN, (UINT16) TRAP_BPEN);
    vWriteDspWORD (u1DspId, ADDR_RC2D_TRAP_BP0, (UINT16) TRAP_BP0);
    vWriteDspWORD (u1DspId, ADDR_RC2D_TRAP_BPT0, (UINT16) TRAP_BPT0);
    vWriteDspWORD (u1DspId, ADDR_RC2D_TRAP_BP1, (UINT16) TRAP_BP1);
    vWriteDspWORD (u1DspId, ADDR_RC2D_TRAP_BPT1, (UINT16) TRAP_BPT1);
    vWriteDspWORD (u1DspId, ADDR_RC2D_TRAP_BP2, (UINT16) TRAP_BP2);
    vWriteDspWORD (u1DspId, ADDR_RC2D_TRAP_BPT2, (UINT16) TRAP_BPT2);
    vWriteDspWORD (u1DspId, ADDR_RC2D_TRAP_BP3, (UINT16) TRAP_BP3);
    vWriteDspWORD (u1DspId, ADDR_RC2D_TRAP_BPT3, (UINT16) TRAP_BPT3);
    vWriteDspWORD (u1DspId, ADDR_RC2D_TRAP_BP4, (UINT16) TRAP_BP4);
    vWriteDspWORD (u1DspId, ADDR_RC2D_TRAP_BPT4, (UINT16) TRAP_BPT4);
    vWriteDspWORD (u1DspId, ADDR_RC2D_TRAP_BP5, (UINT16) TRAP_BP5);
    vWriteDspWORD (u1DspId, ADDR_RC2D_TRAP_BPT5, (UINT16) TRAP_BPT5);
    vWriteDspWORD (u1DspId, ADDR_RC2D_TRAP_BP6, (UINT16) TRAP_BP6);
    vWriteDspWORD (u1DspId, ADDR_RC2D_TRAP_BPT6, (UINT16) TRAP_BPT6);
    vWriteDspWORD (u1DspId, ADDR_RC2D_TRAP_BP7, (UINT16) TRAP_BP7);
    vWriteDspWORD (u1DspId, ADDR_RC2D_TRAP_BPT7, (UINT16) TRAP_BPT7);

    _rDspVars[u1DspId][AUD_DEC_MAIN].tSrcSpkCfg.wDspSpkCfg = 0xFFFF;
    _rDspVars[u1DspId][AUD_DEC_AUX].tSrcSpkCfg.wDspSpkCfg = 0xFFFF;

    // set stream status initial value
    // dsp polling for MPEG4
    vWriteDspWORD (u1DspId, ADDR_RC2D_POLL_DRAM_TIMER_CNT, 0);
    vWriteDspWORD (u1DspId, ADDR_RC2D_POLL_DRAM_READ_UINT32_CNT_1, 0);
    vWriteDspWORD (u1DspId, ADDR_RC2D_PTS_STC_CTRL_FLAG, 0);

#ifdef CC_VOCAL_MUTE
    /// Voice Cancel
    vWriteDspWORD (u1DspId, ADDR_RC2D_POWER_DIFF_PRE, uReadDspShmBYTE(u1DspId, B_VC_DIFF_PRE));
    for(iLoopVar=0;iLoopVar<10;iLoopVar++)
    {
        WriteDspCommDram (u1DspId, (ADDR_RC2D_KARA_VOICE_MUTE_FILTER_COEFF+iLoopVar), VC_BSF_BPF_COEF[iLoopVar]);
    }
    WriteDspCommDram (u1DspId, ADDR_RC2D_KARA_VOICE_MUTE_BSF_GAIN, 0x20000); /// 0x20000 is 0dB
    WriteDspCommDram (u1DspId, ADDR_RC2D_KARA_VOICE_MUTE_BPF_GAIN, 0x20000); /// 0x20000 is 0dB
#endif
}

/***************************************************************************
     Function : vDspPlayCommonInit
  Description : RISC Initialize DSP DRAM Value when Decorder On (Play)
    Parameter : None
    Return    : None
***************************************************************************/
void vDspPlayCommonInit (UINT8 u1DspId)
{
    UINT16 u2DspData;
    INT32  i;
    UINT32 u4DramBase;
#if 1//def CC_AUD_PEQ_SUPPORT
    UINT32 temp;
    INT16 j;
#endif
#ifdef CC_AUD_SILENCE_SUPPORT
    AUD_DEC_STREAM_FROM_T eAudDecStreamFrom;
#endif

    UNUSED(i);
    UNUSED(u4DramBase);

    // Light add for internal buffer control
    vWriteDspWORD (u1DspId, ADDR_RC2D_SYSTEM_CTRL,    u2ReadDspShmWORD (u1DspId, W_SYSTEM_CTRL));
    vWriteDspWORD (u1DspId, ADDR_RC2D_SYSTEM_CTRL2,    u2ReadDspShmWORD (u1DspId, W_SYSTEM_CTRL2));
    vWriteDspWORD (u1DspId, ADDR_RD2D_INTERNAL_BUF_IN_FLAG, u2ReadDspShmWORD (u1DspId, W_INTERNAL_BUF_IN_FLAG));
    vWriteDspWORD (u1DspId, ADDR_MANUAL_OUTPUT_MATRIX, uReadDspShmBYTE (u1DspId, B_MANUAL_OUTPUT_MATRIX));
#ifdef CC_MT5391_AUD_3_DECODER
    vWriteDspWORD (u1DspId, ADDR_RD2D_INTERNAL_BUF2_IN_FLAG, u2ReadDspShmWORD (u1DspId, W_INTERNAL_BUF2_IN_FLAG));
#endif
#ifdef CC_AUD_4_DECODER_SUPPORT
    vWriteDspWORD (u1DspId, ADDR_RD2D_INTERNAL_BUF3_IN_FLAG, u2ReadDspShmWORD (u1DspId, W_INTERNAL_BUF3_IN_FLAG));
#endif

#ifdef CC_AUD_USE_FLASH_AQ
    vDspFlashAQOperation(AQ_TYPE_LIMITER);
#ifdef CC_SONY_2K14_FLASH_AQ
	if(_au1MWSetPositionIdx==1)
	{
	vDspFlashAQOperation(AQ_TYPE_SPKHEIGHT_TABLE);	
	}
	else
	{
	vDspFlashAQOperation(AQ_TYPE_SPKHEIGHT_WALL);	
	}
#else
	vDspFlashAQOperation(AQ_TYPE_SPKHEIGHT);
#endif
#endif //CC_AUD_USE_FLASH_AQ

#ifdef CC_AUD_SUPPORT_MS10
    vWriteDspWORD (u1DspId, ADDR_RC2D_AD_FLAG, uReadDspShmBYTE (u1DspId, B_ADFLAG));
#endif
#ifdef CC_AUD_VBASS_SUPPORT
    // New virtual bass
    u4DramBase = dReadDspCommDram(u1DspId, ADDR_RC2D_VBASS_BASE);
    vWriteDspWORD (u1DspId, (u4DramBase+ADDR_VBASS_CROS_PARA4), u4ReadDspShmDWRD(u1DspId, D_VIR_BASS_GAIN));
    vWriteDspWORD (u1DspId, (u4DramBase+ADDR_VBASS_CROS_PARA7), u4ReadDspShmDWRD(u1DspId, D_VIR_BASS_GAIN_2));
    vWriteDspWORD (u1DspId, (u4DramBase+ADDR_VBASS_CROS_PARA5), 0x3);
    WriteDspCommDram (u1DspId, (u4DramBase+ADDR_VBASS_CROS_PARA6), 0x1000);//0x1000,0db
    vWriteDspWORD (u1DspId, (u4DramBase+ADDR_EXPEND_GAIN), 0x3);
    vWriteDspWORD (u1DspId, (u4DramBase+ADDR_dram_attck_count), 0x1000);
    WriteDspCommDram (u1DspId, (u4DramBase+ADDR_VBASS_CROS_PARA8), 0x1000);
    WriteDspCommDram (u1DspId, (u4DramBase+ADDR_VBASS_CROS_PARA9), 0x1000);
#endif

#ifdef CC_AUD_DSP_SUPPORT_CDNOTCH
    u4DramBase = dReadDspCommDram(u1DspId, ADDR_RC2D_CDNOTCH_BASE);
    WriteDspCommDram (u1DspId, (u4DramBase+ADDR_CDNOTCH_USER_FC), u2ReadDspShmWORD(u1DspId, W_CDNOTCH_USER_FC));
    WriteDspCommDram (u1DspId, (u4DramBase+ADDR_CDNOTCH_USER_Q), uReadDspShmBYTE(u1DspId, B_CDNOTCH_USER_Q));
#endif

#ifndef DSP_SUPPORT_SRSWOW
#ifndef DSP_SUPPORT_SRSTSXT
#ifndef DSP_SUPPORT_SRSTSHD //sunman_tshd
#ifndef DSP_SUPPORT_SRSWOWHD
#ifndef DSP_SUPPORT_SRSCC3D

    // New MTK Vsurr
    WriteDspCommDram (u1DspId, ADDR_VSURR_CROS_PARA1, u4ReadDspShmDWRD(u1DspId, D_VSURR_CROS_TALK));//crosstalk,0x8400
    WriteDspCommDram (u1DspId, ADDR_VSURR_CROS_PARA2, u4ReadDspShmDWRD(u1DspId, D_VSURR_WIDTH)); //surround gain,0x8401
    WriteDspCommDram (u1DspId, ADDR_VSURR_CROS_PARA3, u4ReadDspShmDWRD(u1DspId, D_VSURR_LR_GAIN) );//L/R gain,0x8402
    WriteDspCommDram (u1DspId, ADDR_VSURR_LIMITER_ON, 0x000);
    WriteDspCommDram (u1DspId, ADDR_VSURR_LIMITER_THRESHOLD, 0x1BA000);
    WriteDspCommDram (u1DspId, ADDR_VSURR_LIMITER_RSTEP, 0x2000);
    WriteDspCommDram (u1DspId, ADDR_VSURR_GAIN_MPY, u4ReadDspShmDWRD(u1DspId, D_VSURR_CLARITY));//center gain,0x8406
    WriteDspCommDram (u1DspId, ADDR_VSURR_GAIN_SL, 0x300);
    WriteDspCommDram (u1DspId, ADDR_VSURR_OUT_OF_PHASE, 0x0);
    WriteDspCommDram (u1DspId, ADDR_VSURR_DELAY_ON, 0x0);//off by xiaoyi20110525,actually is bypass in DSP
    WriteDspCommDram (u1DspId, ADDR_VSURR_DELAY_SAMPLE_NUM_L, 0x30000);
    WriteDspCommDram (u1DspId, ADDR_VSURR_DELAY_SAMPLE_NUM_R, 0x30000);
	  WriteDspCommDram (u1DspId, (ADDR_VSURR_BASS_COEF + 3),u4ReadDspShmDWRD(u1DspId, D_VSURR_FO)); //0x843c
    WriteDspCommDram (u1DspId, ADDR_VSURR_CROS_PARA4, u4ReadDspShmDWRD(u1DspId, D_VSURR_BASS_GAIN));//bass enhancement,0x843e
    WriteDspCommDram (u1DspId, ADDR_VSURR_CROS_PARA5, u4ReadDspShmDWRD(u1DspId, D_VSURR_OUTPUT_GAIN));//output gain,0x843f

    // WriteDspCommDram (ADDR_VSURR_CROS_PARA6, 0x60000);
#endif
#endif  //DSP_SUPPORT_SRSWOWHD
#endif  //DSP_SUPPORT_SRSTSHD
#endif  // DSP_SUPPORT_SRSTSXT
#endif  // DSP_SUPPORT_SRSWOW

#ifdef CC_AUD_SILENCE_SUPPORT
    AUD_DRVGetStreamFrom(u1DspId, AUD_DEC_MAIN, &eAudDecStreamFrom);
    if  (eAudDecStreamFrom == AUD_STREAM_FROM_LINE_IN)
    {
        WriteDspCommDram (u1DspId, ADDR_RC2D_SILENCE_GAIN_MODE, (UINT32)DRVCUST_OptGet(eAudioSilenceMode));
    }
	else
	{
        WriteDspCommDram (u1DspId, ADDR_RC2D_SILENCE_GAIN_MODE, 0x0);
    }
	WriteDspCommDram (u1DspId, ADDR_RC2D_SILENCE_FIX_THRESHOLD, (UINT32)DRVCUST_OptGet(eAudioSilenceFixThreshold));
	WriteDspCommDram (u1DspId, ADDR_RC2D_SILENCE_WAIT_NUM, (((UINT32)DRVCUST_OptGet(eAudioSilenceAttackWait))*48/256) << 8);
	WriteDspCommDram (u1DspId, ADDR_RC2D_SILENCE_RELEASE_STEP,(0x7fffff/(((UINT32)DRVCUST_OptGet(eAudioSilenceReleaseStep))*48/256)));
	WriteDspCommDram (u1DspId, ADDR_RC2D_SILENCE_ATTACK_STEP,(0x7fffff/(((UINT32)DRVCUST_OptGet(eAudioSilenceAttackStep))*48/256)));
	WriteDspCommDram (u1DspId, ADDR_RC2D_SILENCE_ATTACK_GAIN,(UINT32)DRVCUST_OptGet(eAudioSilenceAttackFixGain));
#else
    WriteDspCommDram (u1DspId, ADDR_RC2D_SILENCE_GAIN_MODE, 0x0);
#endif

#ifdef CC_AUD_BBE_SUPPORT
    // BBE VIVA 4
    u4DramBase = dReadDspCommDram(u1DspId, ADDR_RC2D_BBE_BASE);
    WriteDspCommDram (u1DspId, (u4DramBase+ADDR_BBEV4_LEVEL_COEF), BBEV4_LEVEL_DB[uReadDspShmBYTE (B_BBE_LEVEL)]);
    WriteDspCommDram (u1DspId, (u4DramBase+ADDR_BBEV4_3D_GAIN), BBEV4_3D_GAIN[uReadDspShmBYTE (B_BBE_3D_GAIN)]);
    WriteDspCommDram (u1DspId, (u4DramBase+ADDR_BBEV4_LOW_COEF), BBEV4_DB_8[uReadDspShmBYTE (B_BBE_LO_CONTOUR)]); // 7dB
    WriteDspCommDram (u1DspId, (u4DramBase+ADDR_BBEV4_HIGH_COEF), BBEV4_DB_8[uReadDspShmBYTE (B_BBE_PROCESS)]);  // 7dB

    vWriteDspWORD (u1DspId, (u4DramBase+ADDR_BBEV4_BBE_ON), 0x1);
    vWriteDspWORD (u1DspId, (u4DramBase+ADDR_BBEV4_LOW_ON), 0x1);
    vWriteDspWORD (u1DspId, (u4DramBase+ADDR_BBEV4_MID_ON), 0x1);
    vWriteDspWORD (u1DspId, (u4DramBase+ADDR_BBEV4_HIGH_ON), 0x1);
#endif // CC_AUD_BBE_SUPPORT

    // Light: AVC need different initial mode for channel changing and UI Off/On setting (aout reinitial).
    WriteDspCommDram (u1DspId, ADDR_RC2D_AVC_FLAG, u2ReadDspShmWORD (u1DspId, W_AVC_FLAG)<<8);
    vWriteDspWORD(u1DspId, ADDR_RC2D_AVC_DSP_FLAG, 0x0);

    //System Setup
    vWriteDspWORD (u1DspId, ADDR_RC2D_SYSTEM_SETUP, u2ReadDspShmWORD (u1DspId, W_SYSTEM_SETUP));

    vWriteDspWORD (u1DspId, ADDR_RC2D_CTRL_2_NUM_1, (UINT16) CTRL_2_NUM_1);

    //Input Src Gain
    WriteDspCommDram (u1DspId, ADDR_RC2D_INPUT_SRC_GAIN, u4ReadDspShmDWRD (u1DspId, D_INSRC_GAIN));

    WriteDspCommDram (u1DspId, ADDR_RC2D_STC_DIFF, 0x0);

    // system master information
    WriteDspCommDram (u1DspId, ADDR_RC2D_STC_DIFF_LOBOUND,
        u4ReadDspShmDWRD (u1DspId, D_STC_DIFF_LO));
    WriteDspCommDram (u1DspId, ADDR_RC2D_STC_DIFF_HIBOUND,
        u4ReadDspShmDWRD (u1DspId, D_STC_DIFF_HI));
    WriteDspCommDram (u1DspId, ADDR_RC2D_STC_DIFF_WSBOUND,
        u4ReadDspShmDWRD (u1DspId, D_STC_DIFF_WS));
    WriteDspCommDram (u1DspId, ADDR_RC2D_STC_LEVEL_THRESHOLD,
        u4ReadDspShmDWRD (u1DspId, D_STC_THRESHOLD));
    WriteDspCommDram (u1DspId, ADDR_RC2D_DECODING_STR_PNT,
        u4ReadDspShmDWRD (u1DspId, D_DECODING_STR_PNT));
    WriteDspCommDram (u1DspId, ADDR_RC2D_1ST_PTS_PRS_PNT,
        u4ReadDspShmDWRD (u1DspId, D_1ST_PTS_PRS_PNT));
    vWriteDspWORD (u1DspId, ADDR_RC2D_1ST_PTS_STCH,
        u2ReadDspShmWORD (u1DspId, W_1ST_PTS_STCH));
    vWriteDspWORD (u1DspId, ADDR_RC2D_1ST_PTS_STCL,
        u2ReadDspShmWORD (u1DspId, W_1ST_PTS_STCL));

    //not reset to 0 by driver, LGE Gstreamer will set to normal play mode.
    vWriteDspWORD (u1DspId, ADDR_RC2D_SPEED, u2ReadDspShmWORD (u1DspId, W_SPEED));

    vWriteDspWORD (u1DspId, ADDR_RC2D_KARAOKE_FLAG, uReadDspShmBYTE (u1DspId, B_KARAFLAG));

    WriteDspCommDram (u1DspId, ADDR_RC2D_LRCH_MIX_RATIO,
        u4ReadDspShmDWRD (u1DspId, D_LRMIXRATIO));

    // Reset aout bank number to default value for some decoding will change it
    vWriteDspWORD (u1DspId, ADDR_RC2D_AOUT_BANK_576_NUM,
        uReadDspShmBYTE (u1DspId, B_BANK576NUM));
    vWriteDspWORD (u1DspId, ADDR_RC2D_AOUT_BANK_384_NUM,
        uReadDspShmBYTE (u1DspId, B_BANK384NUM));
    vWriteDspWORD (u1DspId, ADDR_RC2D_AOUT_BANK_320_NUM,
        uReadDspShmBYTE (u1DspId, B_BANK320NUM));
    vWriteDspWORD (u1DspId, ADDR_RC2D_AOUT_BANK_256_NUM,
        uReadDspShmBYTE (u1DspId, B_BANK256NUM));

    //Processing Mode
    u2DspData = u2ReadDspShmWORD (u1DspId, W_PROCMOD);
    if (_rDspVars[u1DspId][AUD_DEC_MAIN].bDspStrTyp != AC3_STREAM)
    {
        u2DspData &= 0xffdf;
    }

    vWriteDspWORD (u1DspId, ADDR_RC2D_PROCESSING_MODE, u2DspData);

    // Strange <- sammy
    //Bit Resolution (24 bit)
    vWriteDspWORD (u1DspId, ADDR_RC2D_BIT_RESOLUTION, uReadDspShmBYTE (u1DspId, B_BIT));

    //Decoding Relation Information
    vDecCommonInit (u1DspId);

    //IEC delay
    vWriteDspWORD (u1DspId, ADDR_RC2D_IEC_DELAY, uReadDspShmBYTE (u1DspId, B_IEC_DELAY));

    //Equalizer Switch
    vWriteDspWORD (u1DspId, ADDR_RC2D_EQUALIZER_FLAG, uReadDspShmBYTE (u1DspId, B_EQFLAG));

    vEQ2SbassBandGainFlush(AUD_DEC_MAIN);

#ifdef CC_AUD_FY12_LOUDNESS
    vWriteDspWORD (u1DspId, ADDR_RC2D_S1_LOUDNESS_FLAG, uReadDspShmBYTE (u1DspId, B_EQFLAG2));
    vEQ2Sbass2BandGainFlush(AUD_DEC_MAIN);
#endif

    //Speaker Config
    vSetSpeakerConfig(u1DspId, AUD_DEC_MAIN);

    //Prologic II
    vWriteDspWORD (u1DspId, ADDR_RC2D_PRO_LOGIC_II_MODE, u2ReadDspShmWORD (u1DspId, W_PLIIMODE));
    vWriteDspWORD (u1DspId, ADDR_RC2D_PRO_LOGIC_II_CONFIG,
        u2ReadDspShmWORD (u1DspId, W_PLIICONFIG));

    // Virtual surround flag
#ifndef CC_AUD_NEW_POST_MULTI_VS_SUPPORT
    vWriteDspWORD (u1DspId, ADDR_RC2D_VIRTUAL_SURROUND, uReadDspShmBYTE (u1DspId, B_VSURRFLAG));
#else
    vWriteDspWORD (u1DspId, ADDR_RC2D_VIRTUAL_SURROUND, u2ReadDspShmWORD(u1DspId, W_VSURRFLAG));
#endif

#ifdef CC_AUD_SPEAKER_HEIGHT
	u4DramBase = dReadDspCommDram(u1DspId, ADDR_RC2D_SPKHEIGHT_BASE);
    WriteDspCommDram (u1DspId, (u4DramBase + ADDR_SPKHIGH_PARA4), 0x100000);
    WriteDspCommDram (u1DspId, (u4DramBase + ADDR_SPKHIGH_PARA5), 0x300);
#endif

    //Reverb Flag
    vWriteDspWORD (u1DspId, ADDR_RC2D_REVERB_FLAG, uReadDspShmBYTE (u1DspId, B_REVERBFLAG));

    // SPDIF Line/in
    vWriteDspWORD(u1DspId, ADDR_RC2D_SPDIF_FLAG, AUD_IsSpdifInSrc(AUD_DEC_MAIN));

	// DDCO flag
    // vWriteDspWORD (ADDR_RC2D_DDCO_FLAG, 0x0);	//only initial at power on
    vWriteDspWORD (u1DspId, ADDR_RC2D_DDCO_AGC_FLAG,  uReadDspShmBYTE (u1DspId, B_DDCO_AGC_FLAG));
    vWriteDspWORD (u1DspId, ADDR_RC2D_DDCO_LFE_LPF_FLAG,  uReadDspShmBYTE (u1DspId, B_DDCO_LFE_LPF_FLAG));

    // Audio mute threshold
    WriteDspCommDram (u1DspId, ADDR_RC2D_AUDIO_MUTE_THRESHOLD, 0x0);

    // Limiter mode
    vWriteDspWORD (u1DspId, ADDR_RC2D_LIMITER_GAIN_MODE, u2ReadDspShmWORD (u1DspId, W_LIMITER_MODE));
    vWriteDspWORD (u1DspId, ADDR_RC2D_LIMITER_FLAG, u2ReadDspShmWORD (u1DspId, W_LIMITER_FLAG));
    WriteDspCommDram (u1DspId, ADDR_RC2D_LIMITER_FIX_THRESHOLD, u4ReadDspShmDWRD (u1DspId, D_LIMITER_FIX_THRESHOLD));
#ifdef CC_RATIO_LIMITER
    WriteDspCommDram (u1DspId, ADDR_RC2D_LIMITER_GAIN_RATIO, u4ReadDspShmDWRD (D_LIMITER_GAIN_RATIO));
#endif

#ifdef CC_AUD_DRC_V20
    vWriteDspWORD (u1DspId, DRAM_DRC2_LIMITER_MODE,  u2ReadDspShmWORD (u1DspId, W_LIMITER_MODE));
    vWriteDspWORD (u1DspId, DRAM_DRC2_LIMITER_FLAG, u2ReadDspShmWORD (u1DspId, W_LIMITER_FLAG));
    WriteDspCommDram (u1DspId, (DRAM_DRC2_SET_LIMITER_BASE+DRC2_SET_TARGET_LEVEL),
    	u4ReadDspShmDWRD (u1DspId, D_LIMITER_FIX_THRESHOLD));
    WriteDspCommDram (u1DspId, (DRAM_DRC2_SET_LIMITER_BASE+DRC2_SET_ATTACK_RATE),
    	u4ReadDspShmDWRD (u1DspId, D_LIMITER_ATTACK_RATE));
    WriteDspCommDram (u1DspId, (DRAM_DRC2_SET_LIMITER_BASE+DRC2_SET_RELEASE_RATE),
    	u4ReadDspShmDWRD (u1DspId, D_LIMITER_RELEASE_RATE));
    WriteDspCommDram (u1DspId, (DRAM_DRC2_SET_LIMITER_BASE+DRC2_SET_RATIO),
    u4ReadDspShmDWRD (u1DspId, D_LIMITER_GAIN_RATIO));

    // DRC
    vWriteDspWORD (u1DspId, DRAM_DRC2_DRC_FLAG, u2ReadDspShmWORD(u1DspId, W_DRC_FLAG));
    WriteDspCommDram (u1DspId, (DRAM_DRC2_SET_DRC_BAND0_BASE+DRC2_SET_TARGET_LEVEL),
        u4ReadDspShmDWRD (u1DspId, D_DRC_TARGET_LEVEL));
    WriteDspCommDram (u1DspId, (DRAM_DRC2_SET_DRC_BAND0_BASE+DRC2_SET_ATTACK_RATE),
        u4ReadDspShmDWRD (u1DspId, D_DRC_ATTACK_RATE));
    WriteDspCommDram (u1DspId, (DRAM_DRC2_SET_DRC_BAND0_BASE+DRC2_SET_RELEASE_RATE),
        u4ReadDspShmDWRD (u1DspId, D_DRC_RELEASE_RATE));
    WriteDspCommDram (u1DspId, (DRAM_DRC2_SET_DRC_BAND0_BASE+DRC2_SET_RATIO),
        u4ReadDspShmDWRD (u1DspId, D_DRC_RATIO));
    WriteDspCommDram (u1DspId, (DRAM_DRC2_SET_DRC_BAND0_BASE+DRC2_SET_SILENCE_LEVEL),
        u4ReadDspShmDWRD (u1DspId, D_DRC_SILENCE_LEVEL));
    WriteDspCommDram (u1DspId, (DRAM_DRC2_SET_DRC_BAND0_BASE+DRC2_SET_MAX_EXPAND),
        u4ReadDspShmDWRD (u1DspId, D_DRC_MAX_EXPAND));
    WriteDspCommDram (u1DspId, (DRAM_DRC2_SET_DRC_BAND0_BASE+DRC2_SET_HYSTERESIS),
        u4ReadDspShmDWRD (u1DspId, D_DRC_HYSTERESIS));
    vWriteDspWORD (u1DspId, (DRAM_DRC2_SET_DRC_BAND0_BASE+DRC2_SET_HOLD),
        u4ReadDspShmDWRD (u1DspId, D_DRC_HOLD));
    WriteDspCommDram (u1DspId, (DRAM_DRC2_SET_DRC_BAND1_BASE+DRC2_SET_TARGET_LEVEL),
        u4ReadDspShmDWRD (u1DspId, D_DRC_TARGET_LEVEL_1));
    WriteDspCommDram (u1DspId, (DRAM_DRC2_SET_DRC_BAND1_BASE+DRC2_SET_ATTACK_RATE),
        u4ReadDspShmDWRD (u1DspId, D_DRC_ATTACK_RATE_1));
    WriteDspCommDram (u1DspId, (DRAM_DRC2_SET_DRC_BAND1_BASE+DRC2_SET_RELEASE_RATE),
        u4ReadDspShmDWRD (u1DspId, D_DRC_RELEASE_RATE_1));
    WriteDspCommDram (u1DspId, (DRAM_DRC2_SET_DRC_BAND1_BASE+DRC2_SET_RATIO),
        u4ReadDspShmDWRD (u1DspId, D_DRC_RATIO_1));
    WriteDspCommDram (u1DspId, (DRAM_DRC2_SET_DRC_BAND1_BASE+DRC2_SET_SILENCE_LEVEL),
        u4ReadDspShmDWRD (u1DspId, D_DRC_SILENCE_LEVEL_1));
    WriteDspCommDram (u1DspId, (DRAM_DRC2_SET_DRC_BAND1_BASE+DRC2_SET_MAX_EXPAND),
        u4ReadDspShmDWRD (u1DspId, D_DRC_MAX_EXPAND_1));
    WriteDspCommDram (u1DspId, (DRAM_DRC2_SET_DRC_BAND1_BASE+DRC2_SET_HYSTERESIS),
        u4ReadDspShmDWRD (u1DspId, D_DRC_HYSTERESIS_1));
    vWriteDspWORD (u1DspId, (DRAM_DRC2_SET_DRC_BAND1_BASE+DRC2_SET_HOLD),
        u4ReadDspShmDWRD (u1DspId, D_DRC_HOLD_1));

    // Limiter post drc mode
    WriteDspCommDram (u1DspId, DRAM_DRC2_LIMITER_POST_DRC_MODE, u2ReadDspShmWORD(u1DspId, W_DRC_LIMITER_POST_DRC_MODE));

    // post DRC limiter
    {
        UINT32 u4Target[2], u4Attack[2], u4Release[2];
        UINT32 u4Value;

        u4Target [0]= u4ReadDspShmDWRD (u1DspId, D_DRC_TARGET_LEVEL);
        u4Target [1]= u4ReadDspShmDWRD (u1DspId, D_DRC_TARGET_LEVEL_1);
        if (u4Target [0] < u4Target [1])
        {
            u4Value = u4Target [1];
        }
        else
        {
            u4Value = u4Target [0];
        }
        //u4Value = u4Value - ((u4Value * 15)/ 100);   //0.85
        u4Value = u4Value - ((u4Value * 17)/ 100); //0.83
        if (u4Value < 0x1000) // fix me !!!!
        {
            u4Value = 0x1000;
        }

        WriteDspCommDram (u1DspId, (DRAM_DRC2_SET_LIMITER_POST_DRC_BASE+DRC2_SET_TARGET_LEVEL), u4Value);

        u4Attack[0] = u4ReadDspShmDWRD (u1DspId, D_DRC_ATTACK_RATE) / 2;
        u4Attack[1] = u4ReadDspShmDWRD (u1DspId, D_DRC_ATTACK_RATE_1) / 2;
        u4Value = (u4Attack[0] >= u4Attack[1]) ? u4Attack[0] : u4Attack[1];

        if (u4Value == 0)
        {
            u4Value = 1;
        }
        WriteDspCommDram (u1DspId, (DRAM_DRC2_SET_LIMITER_POST_DRC_BASE+DRC2_SET_ATTACK_RATE), u4Value);

        u4Release[0] = u4ReadDspShmDWRD (u1DspId, D_DRC_RELEASE_RATE) / 2;
        u4Release[1] = u4ReadDspShmDWRD (u1DspId, D_DRC_RELEASE_RATE_1) / 2;
        u4Value = (u4Release[0] >= u4Release[1]) ? u4Release[0] : u4Release[1];

        if (u4Value == 0)
        {
            u4Value = 1;
        }
        WriteDspCommDram (u1DspId, (DRAM_DRC2_SET_LIMITER_POST_DRC_BASE+DRC2_SET_RELEASE_RATE), u4Value);

        WriteDspCommDram (u1DspId, (DRAM_DRC2_SET_LIMITER_POST_DRC_BASE+DRC2_SET_RATIO), u4ReadDspShmDWRD (u1DspId, D_DRC_RATIO));
        WriteDspCommDram (u1DspId, (DRAM_DRC2_SET_LIMITER_POST_DRC_BASE+DRC2_SET_SILENCE_LEVEL), u4ReadDspShmDWRD (u1DspId, D_DRC_SILENCE_LEVEL));
        WriteDspCommDram (u1DspId, (DRAM_DRC2_SET_LIMITER_POST_DRC_BASE+DRC2_SET_MAX_EXPAND), 0);
        WriteDspCommDram (u1DspId, (DRAM_DRC2_SET_LIMITER_POST_DRC_BASE+D_DRC_HYSTERESIS_1), 0);

        u4Value = u4ReadDspShmDWRD (D_DRC_HOLD) * 2;
        vWriteDspWORD (u1DspId, (DRAM_DRC2_SET_LIMITER_POST_DRC_BASE+DRC2_SET_HOLD), u4Value);

        // open drc_limiter for P2-2H & P2-LH       //#ifdef FIGARO_P2L_PROJECT
        //WriteDspCommDram (DRAM_DRC2_LIMITER_POST_DRC_MODE, 0x1); // default is ON, mask by paul
        //#else
        //WriteDspCommDram (DRAM_DRC2_LIMITER_POST_DRC_MODE, 0x0); // default is OFF
        //#endif
    }
#endif

    //sunman for play speed
#ifdef FRAC_SPEED_SUPPORT
    WriteDspCommDram(u1DspId, ADDR_RC2D_PLAY_SPEED, 	u4ReadDspShmDWRD(u1DspId, D_PLAY_SPEED));
#endif

#ifdef CC_AUD_NEW_POST_MULTI_VS_SUPPORT
	vWriteDspWORD (u1DspId, ADDR_RC2D_NEWPOST_FLAG,
        ((wReadDspWORD(u1DspId, ADDR_RC2D_NEWPOST_FLAG)) | NEWPOST_FLAG_M2S_BIT));
#endif

#ifdef CC_AUD_HPF_SUPPORT
    // Setup HPF enable bit
    vWriteDspWORD (u1DspId, ADDR_RC2D_NEWPOST_FLAG,
        ((wReadDspWORD(u1DspId, ADDR_RC2D_NEWPOST_FLAG)) & ~NEWPOST_FLAG_HPF_BIT) |
        ((uReadDspShmBYTE(u1DspId, B_HPF_ENABLE) > 0) ? NEWPOST_FLAG_HPF_BIT : 0));
#endif

#ifdef DSP_SUPPORT_TCL_VBASS
    // Setup TCL Vbass enable bit
    vWriteDspWORD (u1DspId, ADDR_RC2D_NEWPOST_FLAG,
        ((wReadDspWORD(u1DspId, ADDR_RC2D_NEWPOST_FLAG)) & ~NEWPOST_FLAG_TCLVBASS_BIT) |
        ((uReadDspShmBYTE(u1DspId, B_TCLBASS_ENABLE) > 0) ? NEWPOST_FLAG_TCLVBASS_BIT : 0));
#endif


#ifdef CC_AUD_CLIPPER_SUPPORT
    // Setup clipper enable bit
    vWriteDspWORD (u1DspId, ADDR_RC2D_NEWPOST_FLAG,
        ((wReadDspWORD(u1DspId, ADDR_RC2D_NEWPOST_FLAG)) & ~NEWPOST_FLAG_CLIPPER_BIT) |
        ((uReadDspShmBYTE(u1DspId, B_CLIPPER_ENABLE) > 0) ? NEWPOST_FLAG_CLIPPER_BIT : 0));
#endif

#ifdef CC_AUD_CLIPPER_SUPPORT
    // Setup clipper value
    WriteDspCommDram(u1DspId, ADDR_RC2D_CLIPPER_C_VAL, u4ReadDspShmDWRD(D_CLIPPER_C_VALUE));
    WriteDspCommDram(u1DspId, ADDR_RC2D_CLIPPER_L_VAL, u4ReadDspShmDWRD(D_CLIPPER_L_VALUE));
    WriteDspCommDram(u1DspId, ADDR_RC2D_CLIPPER_R_VAL, u4ReadDspShmDWRD(D_CLIPPER_R_VALUE));
    WriteDspCommDram(u1DspId, ADDR_RC2D_CLIPPER_LS_VAL, u4ReadDspShmDWRD(D_CLIPPER_LS_VALUE));
    WriteDspCommDram(u1DspId, ADDR_RC2D_CLIPPER_RS_VAL, u4ReadDspShmDWRD(D_CLIPPER_RS_VALUE));
    WriteDspCommDram(u1DspId, ADDR_RC2D_CLIPPER_SW_VAL, u4ReadDspShmDWRD(D_CLIPPER_SW_VALUE));
    WriteDspCommDram(u1DspId, ADDR_RC2D_CLIPPER_CH7_VAL, u4ReadDspShmDWRD(D_CLIPPER_CH7_VALUE));
    WriteDspCommDram(u1DspId, ADDR_RC2D_CLIPPER_CH8_VAL, u4ReadDspShmDWRD(D_CLIPPER_CH8_VALUE));
    WriteDspCommDram(u1DspId, ADDR_RC2D_CLIPPER_CH9_VAL, u4ReadDspShmDWRD(D_CLIPPER_CH9_VALUE));
    WriteDspCommDram(u1DspId, ADDR_RC2D_CLIPPER_CH10_VAL, u4ReadDspShmDWRD(D_CLIPPER_CH10_VALUE));
#endif

#ifdef CC_AUD_SPEAKER_HEIGHT
    vWriteDspWORD (u1DspId, ADDR_RC2D_NEWPOST_FLAG,
        ((wReadDspWORD(u1DspId, ADDR_RC2D_NEWPOST_FLAG)) & ~NEWPOST_FLAG_SPH_BIT) |
    #ifndef	CC_AUD_SPEAKER_HEIGHT_MODE
        ((uReadDspShmBYTE(u1DspId, B_SPH_ENABLE) > 0) ? NEWPOST_FLAG_SPH_BIT : 0));
    #else
        ((uReadDspShmBYTE(u1DspId, B_SPH_MODE) > 0) ? NEWPOST_FLAG_SPH_BIT : 0));
    #endif
#endif

#if 1//def CC_AUD_PEQ_SUPPORT
    // Setup PEQ relative
    u4DramBase = dReadDspCommDram(u1DspId, ADDR_RC2D_PEQ_BASE);
    vWriteDspWORD (u1DspId, ADDR_RC2D_NEWPOST_FLAG,
        ((wReadDspWORD(u1DspId, ADDR_RC2D_NEWPOST_FLAG)) & ~NEWPOST_FLAG_PEQ_BIT) |
        ((uReadDspShmBYTE(u1DspId, B_PEQ_ENABLE) > 0) ? NEWPOST_FLAG_PEQ_BIT : 0));


    for (i=0; i<9; i++)
    {
        WriteDspCommDram (u1DspId, (u4DramBase + ADDR_PEQ1_FC + i*3), u2ReadDspShmWORD(u1DspId, W_PEQ_SET0_FC + i*8));
        WriteDspCommDram (u1DspId, (u4DramBase + ADDR_PEQ1_GAIN + i*3), u4ReadDspShmDWRD(u1DspId, D_PEQ_SET0_GAIN + i*8));
        j = dwGetPEQFsIdx(i) ;
        VERIFY(j < sizeof(MTK_TAN_TABLE)/4);
        temp = MTK_TAN_TABLE[j];

        if (j > sizeof(MTK_TAN_TABLE)/4)
        {
            j = (sizeof(MTK_TAN_TABLE)/2) - j;
            temp=-(MTK_TAN_TABLE[j]);
        }

        WriteDspCommDram (u1DspId, (u4DramBase + ADDR_PEQ1_BW + i*3), temp);
		j = dwGetPEQCosIdx(i) ;
		temp = COS_TABLE[j];
		WriteDspCommDram (u1DspId, (u4DramBase + ADDR_PEQ1_FC + i*3), temp);
    }
#ifdef CC_AUD_PEQ_LR
    for (i=0; i<9; i++)
    {
        WriteDspCommDram (u1DspId, (u4DramBase + ADDR_PEQ1_FC_R + i*3), u2ReadDspShmWORD(u1DspId, W_PEQ_SET0_FC_R + i*8));
        WriteDspCommDram (u1DspId, (u4DramBase + ADDR_PEQ1_GAIN_R + i*3), u4ReadDspShmDWRD(u1DspId, D_PEQ_SET0_GAIN_R + i*8));
        j = dwGetPEQFsIdx_R(i) ;
        VERIFY(j < sizeof(MTK_TAN_TABLE)/4);
        temp = MTK_TAN_TABLE[j];

        if (j > sizeof(MTK_TAN_TABLE)/4)
        {
            j = (sizeof(MTK_TAN_TABLE)/2) - j;
            temp=-(MTK_TAN_TABLE[j]);
        }

        WriteDspCommDram (u1DspId, (u4DramBase + ADDR_PEQ1_BW_R + i*3), temp);
		j = dwGetPEQCosIdx_R(i) ;
            temp = COS_TABLE[j];
		WriteDspCommDram (u1DspId, (u4DramBase + ADDR_PEQ1_FC_R + i*3), temp);
    }
#endif
#endif

#ifdef CC_AUD_VBASS_SUPPORT
    vWriteDspWORD (u1DspId, ADDR_RC2D_NEWPOST_FLAG,
        ((wReadDspWORD(u1DspId, ADDR_RC2D_NEWPOST_FLAG)) & ~NEWPOST_FLAG_VIR_BASS_BIT) |
        ((uReadDspShmBYTE(u1DspId, B_VIR_BASS_ENABLE) > 0) ? NEWPOST_FLAG_VIR_BASS_BIT : 0));
#endif

#if 0 //Daniel, 20100202, Move to set sampling rate interrupt handling
#ifdef CC_AUD_DSP_SUPPORT_AUDYSSEY
{
    UINT8 u1BankIdx;
    UINT32 u4DestAddr;
    UINT32 iLoopVar;
    UINT32 *pu4Table;

    pu4Table = (UINT32 *)&AEQ_5BAND_TABLE[0][0];//[u4FreqIdx][0];

    u4DestAddr = ADDR_RC2D_AEQ_TABLE_NEW;

    for (u1BankIdx=0; u1BankIdx<AUD_AEQ_IIR_BAND_NUM; u1BankIdx++)//band nuber
    {
        // copy filter coefficients (5) to DSP common DRAM
        for (iLoopVar = 0; iLoopVar < 5; iLoopVar++)
        {
	        WriteDspCommDram ((u4DestAddr+iLoopVar), pu4Table[iLoopVar]);
        }

        // move destination address to next band
        u4DestAddr += 5;
        // move table pointer to next band
        pu4Table += 5;//DSP_AEQ_BAND_TABLE_SIZE;
    }
}
#endif
#endif

//#ifdef ENABLE_AUDIO_RECORD_SUPPORT
    //For upload info
    vWriteDspWORD (u1DspId, ADDR_RC2D_UPLOAD_DATA_FLAG, u2ReadDspShmWORD(u1DspId, W_UPLOAD_DATA_FLAG));
    vWriteDspWORD (u1DspId, ADDR_RC2D_UPLOAD_BLOCK_NUM, uReadDspShmBYTE(u1DspId, B_UPLOAD_BLOCK_NUM));
    vWriteDspWORD (u1DspId, ADDR_RC2D_UPLOAD_BLOCK_SIZE, u2ReadDspShmWORD(u1DspId, W_UPLOAD_BLOCK_SIZE));
    vWriteDspWORD (u1DspId, ADDR_RC2D_UPLOAD_BLK_INT_CNT, uReadDspShmBYTE(u1DspId, B_UPLOAD_BLK_INT_CNT));
    WriteDspCommDram (u1DspId, ADDR_RC2D_UPLOAD_WRITE_PTR, ((_u4DspCmptBuf[u1DspId][TYPE_COMM_DRAM_IDX] + AUD_UPLOAD_BUFFER_BASE)&0xffffff));
    vWriteDspWORD (u1DspId, ADDR_RC2D_BLUETOOTH_DATA_FLAG, u2ReadDspShmWORD(u1DspId, W_BLUETOOTH_DATA_FLAG));
    vWriteDspWORD (u1DspId, ADDR_RC2D_BLUETOOTH_BLOCK_NUM, uReadDspShmBYTE(u1DspId, B_BLUETOOTH_BLOCK_NUM));
    vWriteDspWORD (u1DspId, ADDR_RC2D_BLUETOOTH_BLOCK_SIZE, u2ReadDspShmWORD(u1DspId, W_BLUETOOTH_BLOCK_SIZE));
    vWriteDspWORD (u1DspId, ADDR_RC2D_BLUETOOTH_BLK_INT_CNT, uReadDspShmBYTE(u1DspId, B_BLUETOOTH_BLK_INT_CNT));
    WriteDspCommDram (u1DspId, ADDR_RC2D_BLUETOOTH_WRITE_PTR, ((_u4DspCmptBuf[u1DspId][TYPE_COMM_DRAM_IDX] + AUD_BLUETOOTH_BUFFER_BASE)&0xffffff));
//#endif
#ifdef CC_AUD_FIR_SUPPORT
    vWriteDspWORD (u1DspId, ADDR_RC2D_NEWPOST_FLAG,
                ((wReadDspWORD(u1DspId, ADDR_RC2D_NEWPOST_FLAG)) & ~NEWPOST_FLAG_FIR_BIT) |
                ((uReadDspShmBYTE(u1DspId, B_FIR_ENABLE) > 0) ? NEWPOST_FLAG_FIR_BIT : 0));
#endif
#ifdef CC_MT5881
    WriteDspCommDram32bit (ADDR_RC2D_DECODER_PATCH_MODE, 
                           dReadDspCommDram32(ADDR_RC2D_DECODER_PATCH_MODE) | PARSER_DMA_WAIT_BIT);
#endif
}



/***************************************************************************
     Function : vDspPlayCommonInitDec2
  Description : RISC Initialize DSP DRAM Value when Second Decorder On (Play)
                Use first information temporarily
    Parameter : None
    Return    : None
***************************************************************************/
void vDspPlayCommonInitDec2 (UINT8 u1DspId)
{
    UINT16 u2DspData;

    // Light add for internal buffer control
    vWriteDspWORD (u1DspId, ADDR_RC2D_SYSTEM_CTRL,    u2ReadDspShmWORD (u1DspId, W_SYSTEM_CTRL));
    vWriteDspWORD (u1DspId, ADDR_RC2D_SYSTEM_CTRL2,    u2ReadDspShmWORD (u1DspId, W_SYSTEM_CTRL2));
    vWriteDspWORD (u1DspId, ADDR_RD2D_INTERNAL_BUF_IN_FLAG, u2ReadDspShmWORD (u1DspId, W_INTERNAL_BUF_IN_FLAG));
    vWriteDspWORD (u1DspId, ADDR_MANUAL_OUTPUT_MATRIX_DEC2, uReadDspShmBYTE (u1DspId, B_MANUAL_OUTPUT_MATRIX_DEC2));

    //System Setup
    vWriteDspWORD (u1DspId, ADDR_RC2D_SYSTEM_SETUP_DEC2, u2ReadDspShmWORD (u1DspId, W_SYSTEM_SETUP_DEC2));

    //Input Src Gain
    WriteDspCommDram (u1DspId, ADDR_RC2D_INPUT_SRC_GAIN_DEC2, u4ReadDspShmDWRD (u1DspId, D_INSRC_GAIN_DEC2));

    /* Initial ctrl_4 */
    vWriteDspWORD (u1DspId, ADDR_RC2D_CTRL_4_NUM_1, (UINT16) CTRL_4_NUM_1);
    // WriteDspCommDram(ADDR_RC2D_STC_DIFF_DEC2, _dwStcDiffDec2);
    WriteDspCommDram (u1DspId, ADDR_RC2D_STC_DIFF_DEC2, 0x0);
    // system master information
    WriteDspCommDram (u1DspId, ADDR_RC2D_STC_DIFF_LOBOUND_DEC2,
        u4ReadDspShmDWRD (u1DspId, D_STC_DIFF_LO_DEC2));
    WriteDspCommDram (u1DspId, ADDR_RC2D_STC_DIFF_HIBOUND_DEC2,
        u4ReadDspShmDWRD (u1DspId, D_STC_DIFF_HI_DEC2));
    WriteDspCommDram (u1DspId, ADDR_RC2D_STC_DIFF_WSBOUND_DEC2,
        u4ReadDspShmDWRD (u1DspId, D_STC_DIFF_WS_DEC2));
    WriteDspCommDram (u1DspId, ADDR_RC2D_STC_LEVEL_THRESHOLD_DEC2,
        u4ReadDspShmDWRD (u1DspId, D_STC_THRESHOLD_DEC2));
    WriteDspCommDram (u1DspId, ADDR_RC2D_DECODING_STR_PNT_DEC2,
        u4ReadDspShmDWRD (u1DspId, D_DECODING_STR_PNT_DEC2));
    WriteDspCommDram (u1DspId, ADDR_RC2D_1ST_PTS_PRS_PNT_DEC2,
        u4ReadDspShmDWRD (u1DspId, D_1ST_PTS_PRS_PNT_DEC2));
    vWriteDspWORD (u1DspId, ADDR_RC2D_1ST_PTS_STCH_DEC2,
        u2ReadDspShmWORD (u1DspId, W_1ST_PTS_STCH_DEC2));
    vWriteDspWORD (u1DspId, ADDR_RC2D_1ST_PTS_STCL_DEC2,
        u2ReadDspShmWORD (u1DspId, W_1ST_PTS_STCL_DEC2));

    //not reset to 0 by driver, LGE Gstreamer will set to normal play mode.
    vWriteDspWORD (u1DspId, ADDR_RC2D_SPEED_DEC2, u2ReadDspShmWORD (u1DspId, W_SPEED_DEC2));
    vWriteDspWORD (u1DspId, ADDR_RC2D_KARAOKE_FLAG_DEC2, uReadDspShmBYTE (u1DspId, B_KARAFLAG));
    WriteDspCommDram (u1DspId, ADDR_RC2D_LRCH_MIX_RATIO_DEC2,
        u4ReadDspShmDWRD (u1DspId, D_LRMIXRATIO));

    // Reset aout bank number to default value for some decoding will change it
    vWriteDspWORD (u1DspId, ADDR_RC2D_AOUT_BANK_576_NUM_DEC2,
        uReadDspShmBYTE (u1DspId, B_BANK576NUM_DEC2));
    vWriteDspWORD (u1DspId, ADDR_RC2D_AOUT_BANK_384_NUM_DEC2,
        uReadDspShmBYTE (u1DspId, B_BANK384NUM_DEC2));
    vWriteDspWORD (u1DspId, ADDR_RC2D_AOUT_BANK_320_NUM_DEC2,
        uReadDspShmBYTE (u1DspId, B_BANK320NUM_DEC2));
    vWriteDspWORD (u1DspId, ADDR_RC2D_AOUT_BANK_256_NUM_DEC2,
        uReadDspShmBYTE (u1DspId, B_BANK256NUM_DEC2));

    //Processing Mode
    u2DspData = u2ReadDspShmWORD (u1DspId, W_PROCMOD_DEC2);
    if (_rDspVars[u1DspId][AUD_DEC_AUX].bDspStrTyp != AC3_STREAM)
    {
        u2DspData &= 0xffdf;
    }

    vWriteDspWORD (u1DspId, ADDR_RC2D_PROCESSING_MODE_DEC2, u2DspData);
    //Bit Resolution (24 bit)
    vWriteDspWORD (u1DspId, ADDR_RC2D_BIT_RESOLUTION_DEC2,
        uReadDspShmBYTE (u1DspId, B_BIT_DEC2));

#ifdef CC_AUD_SUPPORT_MS10
	// XMX mixer in PCM decoder
	// reset it
	vWriteDspWORD (u1DspId, ADDR_RC2D_XMX_FLAG, 0x0);
#endif

    //Decoding Relation Information
    vDecCommonInitDec2 (u1DspId);

    //IEC Latency
    vWriteDspWORD (u1DspId, ADDR_RC2D_IEC_DELAY_DEC2, uReadDspShmBYTE (u1DspId, B_IEC_DELAY));

    //Speaker Config
    vSetSpeakerConfig(u1DspId, AUD_DEC_AUX);

    // SPDIF Line/in
    vWriteDspWORD(u1DspId, ADDR_RC2D_SPDIF_FLAG_DEC2, AUD_IsSpdifInSrc(AUD_DEC_AUX));

    // Audio mute threshold
    WriteDspCommDram (u1DspId, ADDR_RC2D_AUDIO_MUTE_THRESHOLD_DEC2, 0x0);
#ifdef CC_MT5881
    WriteDspCommDram32bit (ADDR_RC2D_DECODER_PATCH_MODE, 
                           dReadDspCommDram32(ADDR_RC2D_DECODER_PATCH_MODE) | PARSER_DMA_WAIT_BIT);
#endif

}

#ifdef CC_MT5391_AUD_3_DECODER
/***************************************************************************
     Function : vDspPlayCommonInitDec3
  Description : RISC Initialize DSP DRAM Value when 3rd Decorder On (Play)
                Use first information temporarily
    Parameter : None
    Return    : None
***************************************************************************/
void vDspPlayCommonInitDec3 (UINT8 u1DspId)
{
    UINT16 u2DspData;

    // Light add for internal buffer control
    vWriteDspWORD (u1DspId, ADDR_RC2D_SYSTEM_CTRL,    u2ReadDspShmWORD (u1DspId, W_SYSTEM_CTRL));
    vWriteDspWORD (u1DspId, ADDR_RC2D_SYSTEM_CTRL2,    u2ReadDspShmWORD (u1DspId, W_SYSTEM_CTRL2));
    vWriteDspWORD (u1DspId, ADDR_RD2D_INTERNAL_BUF_IN_FLAG, u2ReadDspShmWORD (u1DspId, W_INTERNAL_BUF_IN_FLAG));
    vWriteDspWORD (u1DspId, ADDR_RD2D_INTERNAL_BUF2_IN_FLAG, u2ReadDspShmWORD (u1DspId, W_INTERNAL_BUF2_IN_FLAG));

    //System Setup
    vWriteDspWORD (u1DspId, ADDR_RC2D_SYSTEM_SETUP_DEC3, u2ReadDspShmWORD (u1DspId, W_SYSTEM_SETUP_DEC3));

    //Input Src Gain
    WriteDspCommDram (u1DspId, ADDR_RC2D_INPUT_SRC_GAIN_DEC3, u4ReadDspShmDWRD (u1DspId, D_INSRC_GAIN_DEC3));

    /* Initial ctrl_4 */ //gallen, needs to modify accroding to new MT539x DSP
    vWriteDspWORD (u1DspId, ADDR_RC2D_CTRL_6_NUM_1, (UINT16) CTRL_6_NUM_1);

    WriteDspCommDram (u1DspId, ADDR_RC2D_STC_DIFF_DEC3, 0x0);
    // system master information
    WriteDspCommDram (u1DspId, ADDR_RC2D_STC_DIFF_LOBOUND_DEC3,
                      u4ReadDspShmDWRD (u1DspId, D_STC_DIFF_LO_DEC3));
    WriteDspCommDram (u1DspId, ADDR_RC2D_STC_DIFF_HIBOUND_DEC3,
                      u4ReadDspShmDWRD (u1DspId, D_STC_DIFF_HI_DEC3));
    WriteDspCommDram (u1DspId, ADDR_RC2D_STC_DIFF_WSBOUND_DEC3,
                      u4ReadDspShmDWRD (u1DspId, D_STC_DIFF_WS_DEC3));
    WriteDspCommDram (u1DspId, ADDR_RC2D_STC_LEVEL_THRESHOLD_DEC3,
                      u4ReadDspShmDWRD (u1DspId, D_STC_THRESHOLD_DEC3));
    WriteDspCommDram (u1DspId, ADDR_RC2D_1ST_PTS_PRS_PNT_DEC3,
                      u4ReadDspShmDWRD (u1DspId, D_1ST_PTS_PRS_PNT_DEC3));
    vWriteDspWORD (u1DspId, ADDR_RC2D_1ST_PTS_STCH_DEC3,
                      u2ReadDspShmWORD (u1DspId, W_1ST_PTS_STCH_DEC3));
    vWriteDspWORD (u1DspId, ADDR_RC2D_1ST_PTS_STCL_DEC3,
                      u2ReadDspShmWORD (u1DspId, W_1ST_PTS_STCL_DEC3));
#ifdef DSP_SUPPORT_DUAL_DECODE
    //Reset Speed to 0
    vWriteDspWORD (u1DspId, ADDR_RC2D_SPEED_DEC3, u2ReadDspShmWORD (u1DspId, W_SPEED_DEC3));
#endif
    // Reset aout bank number to default value for some decoding will change it
    vWriteDspWORD (u1DspId, ADDR_RC2D_AOUT_BANK_576_NUM_DEC3,
        uReadDspShmBYTE (u1DspId, B_BANK576NUM_DEC3));
    vWriteDspWORD (u1DspId, ADDR_RC2D_AOUT_BANK_384_NUM_DEC3,
        uReadDspShmBYTE (u1DspId, B_BANK384NUM_DEC3));
    vWriteDspWORD (u1DspId, ADDR_RC2D_AOUT_BANK_320_NUM_DEC3,
        uReadDspShmBYTE (u1DspId, B_BANK320NUM_DEC3));
    vWriteDspWORD (u1DspId, ADDR_RC2D_AOUT_BANK_256_NUM_DEC3,
        uReadDspShmBYTE (u1DspId, B_BANK256NUM_DEC3));

    // Dec3 uses D_DECODING_STR_PNT for AD function
    WriteDspCommDram (u1DspId, ADDR_RC2D_DECODING_STR_PNT_DEC3,
                      u4ReadDspShmDWRD (u1DspId, D_DECODING_STR_PNT_DEC3));

    //Processing Mode
    u2DspData = u2ReadDspShmWORD (u1DspId, W_PROCMOD_DEC3);
    vWriteDspWORD (u1DspId, ADDR_RC2D_PROCESSING_MODE_DEC3, u2DspData);

    //Decoding Relation Information
    vDecCommonInitDec3 (u1DspId);

    //Speaker Config
    vSetSpeakerConfig(u1DspId, AUD_DEC_THIRD);

#ifdef CC_MT5881
    WriteDspCommDram32bit (ADDR_RC2D_DECODER_PATCH_MODE, 
                           dReadDspCommDram32(ADDR_RC2D_DECODER_PATCH_MODE) | PARSER_DMA_WAIT_BIT);
#endif
}
#endif

#ifdef CC_AUD_4_DECODER_SUPPORT
/***************************************************************************
     Function : vDspPlayCommonInitDec4
  Description : RISC Initialize DSP DRAM Value when 4th Decorder On (Play)
                Use first information temporarily
    Parameter : None
    Return    : None
***************************************************************************/
void vDspPlayCommonInitDec4 (UINT8 u1DspId)
{
    UINT16 u2DspData;

    //Printf("vDspPlayCommonInitDec4\n"); //SBCENC_TEST
    // Light add for internal buffer control
    vWriteDspWORD (u1DspId, ADDR_RC2D_SYSTEM_CTRL,    u2ReadDspShmWORD (u1DspId, W_SYSTEM_CTRL));
    vWriteDspWORD (u1DspId, ADDR_RC2D_SYSTEM_CTRL2,    u2ReadDspShmWORD (u1DspId, W_SYSTEM_CTRL2));
    vWriteDspWORD (u1DspId, ADDR_RD2D_INTERNAL_BUF3_IN_FLAG, u2ReadDspShmWORD (u1DspId, W_INTERNAL_BUF3_IN_FLAG));

    // System Setup
    vWriteDspWORD (u1DspId, ADDR_RC2D_SYSTEM_SETUP_DEC4, u2ReadDspShmWORD (u1DspId, W_SYSTEM_SETUP_DEC4));

    //Input Src Gain
    WriteDspCommDram (u1DspId, ADDR_RC2D_INPUT_SRC_GAIN_DEC4, u4ReadDspShmDWRD (u1DspId, D_INSRC_GAIN_DEC4));

    // Initial ctrl_7
    vWriteDspWORD (u1DspId, ADDR_RC2D_CTRL_7_NUM_1, (UINT16)CTRL_7_NUM_1);

    //Processing Mode
    u2DspData = u2ReadDspShmWORD (u1DspId, W_PROCMOD_DEC4);
    vWriteDspWORD (u1DspId, ADDR_RC2D_PROCESSING_MODE_DEC4, u2DspData);

    // Decoding Relation Information
    vDecCommonInitDec4 (u1DspId);
}
#endif

/***************************************************************************
     Function : vCustomOptInit
  Description : Customer different setting option
    Parameter : None
    Return    : None
***************************************************************************/
void vCustomOptInit (void)
{
    UINT16 u2DspData;

    // Function oriented options
    u2DspData = 0;
    vWriteDspWORD (AUD_DSP0, ADDR_RC2D_CUSTOMER_OPTION0,      (u2DspData|AUDIO_UPDATE_EFFECT_ONOFF));
    vWriteDspWORD (AUD_DSP0, ADDR_RC2D_CUSTOMER_OPTION0_DEC2, u2DspData);

}
//-----------------------------------------------------------------------------
/** _AUD_DspGetVolumeTable
 *   Query volume table.
 *
 *  @param  u1VolTabIdx
 *
 *  @retval  void
 */
//-----------------------------------------------------------------------------
static UINT8 vFineTuneVolTblIdxChk(UINT8 u1VolTabIdx)
{
    UINT8 tbl_idx;
    tbl_idx = u1VolTabIdx;
    if (tbl_idx > 40) tbl_idx = 40;
    return tbl_idx;
}

#ifdef CC_AUD_USE_FLASH_AQ
void vDspFlashAQOperation(AUD_AQ_TYPE_T eAQType)
{
    UINT32 u4Value;
    AUD_AQ_TYPE3_DATA_T rAQTypeData;
    UINT8 u1Data[7];

    UNUSED(rAQTypeData);
    UNUSED(u1Data);
    switch (eAQType)
    {
    case AQ_TYPE_LIMITER:
        if (Aud_FlashAQQuery(AQ_TYPE_LIMITER, (void*)u1Data))
        {
            u4Value = (u1Data[0]<<16) | (u1Data[1]<<8) | (u1Data[2]);
            if ((u4Value) < 0x10000)
                u4Value = 0x10000;  //If data is error.... 0x20000 is already 42 dB headroom.
            vWriteDspShmDWRD (D_LIMITER_FIX_THRESHOLD, u4Value); // default is 0x100000

            // bit 0: vol,    bit 1: vsurr,  bit 2: sbass, bit 3: EQ,
            // bit 4: reverb, bit 5: speaker,bit 6: voice
            u4Value = u2ReadDspShmWORD(W_LIMITER_FLAG) & 0xF0;
            u4Value |= ((u1Data[6]&0xF)<<3) | ((u1Data[5]&0xF)<<2)| ((u1Data[4]&0xF)<<1)| (u1Data[3]&0xF);
            vWriteDspShmWORD (W_LIMITER_FLAG, (UINT16)u4Value);

            vWriteDspShmWORD (W_LIMITER_MODE, 2); //Fixed mode.
        }
        break;

#ifdef CC_SONY_2K14_FLASH_AQ
			case AQ_TYPE_SPKHEIGHT_TABLE:
				if (Aud_FlashAQQuery(AQ_TYPE_SPKHEIGHT_TABLE, (void*)&u4Value))
				{
        #if (defined(CC_AUD_SPEAKER_HEIGHT) || defined(CC_AUD_SPEAKER_HEIGHT_V2))
            #ifndef CC_AUD_SPEAKER_HEIGHT_MODE
					vWriteDspShmBYTE(B_SPH_ENABLE, (UINT8)u4Value);
            #else
					vWriteDspShmBYTE(B_SPH_MODE, (UINT8)u4Value);
            #endif
        #endif
				}
				break;
				
				case AQ_TYPE_SPKHEIGHT_WALL:
					if (Aud_FlashAQQuery(AQ_TYPE_SPKHEIGHT_WALL, (void*)&u4Value))
					{
	#if (defined(CC_AUD_SPEAKER_HEIGHT) || defined(CC_AUD_SPEAKER_HEIGHT_V2))
		#ifndef CC_AUD_SPEAKER_HEIGHT_MODE
						vWriteDspShmBYTE(B_SPH_ENABLE, (UINT8)u4Value);
		#else
						vWriteDspShmBYTE(B_SPH_MODE, (UINT8)u4Value);
		#endif
	#endif
					}
					break;
		
#else
    case AQ_TYPE_SPKHEIGHT:
        if (Aud_FlashAQQuery(AQ_TYPE_SPKHEIGHT, (void*)&u4Value))
        {
        #if (defined(CC_AUD_SPEAKER_HEIGHT) || defined(CC_AUD_SPEAKER_HEIGHT_V2))
            #ifndef CC_AUD_SPEAKER_HEIGHT_MODE
            vWriteDspShmBYTE(B_SPH_ENABLE, (UINT8)u4Value);
            #else
            vWriteDspShmBYTE(B_SPH_MODE, (UINT8)u4Value);
            #endif
        #endif
        }
        break;
#endif

    default:
        break;
    };
}
#endif // CC_AUD_USE_FLASH_AQ

static const UINT32 _arMtkOplFuncTab[AUD_OPL_NUM] =
{
    #ifdef CC_AUD_BBE_SUPPORT
    AUD_OPL_BBE,
    #endif
    #ifdef CC_AUD_VBASS_SUPPORT
    AUD_OPL_VIRTUAL_BASS,
    #endif
    #if 1//def CC_AUD_PEQ_SUPPORT
    AUD_OPL_PEQ,
    #endif
	#ifdef CC_AUD_NEW_POST_MULTI_VS_SUPPORT
    AUD_OPL_M2S,
    #endif
    //AUD_OPL_CLEAR_VOICE,
    #ifdef CC_AUD_SPEAKER_HEIGHT
    AUD_OPL_SPK_HEIGHT,
    #endif
    #ifdef CC_AUD_CLIPPER_SUPPORT
    AUD_OPL_CLIPPER,
    #endif
    //AUD_OPL_NOISE_REDUCTION,
    #ifdef KARAOKE_SUPPORT
    AUD_OPTL_KEY_SHIFT,
    #endif
    //AUD_OPL_VIQ,
    //AUD_OPL_LXP,
    #ifdef CC_AUD_DSP_SUPPORT_AUDYSSEY
    AUD_OPL_AUDYSSEY_ADV,
    AUD_OPL_AUDYSSEY_ABX,
    AUD_OPL_AUDYSSEY_AEQ,
    #endif
    #ifdef CC_AUD_DSP_SUPPORT_CDNOTCH
    AUD_OPL_CDNOTCH,
    #endif
#if defined(DSP_SUPPORT_SRS_PEQ)
    AUD_OPL_SRS_PEQ,
    #endif
    #ifdef DSP_SUPPORT_SRSVIQ
    AUD_OPL_SRS_VIQ, //sunman_viq
    #endif
#ifdef CC_AUD_FIR_SUPPORT
	AUD_OPL_FIR,
#endif
    #ifdef DSP_SUPPORT_SRS_AEQ
    AUD_OPL_SRS_AEQ,
    #endif
    #ifdef DSP_SUPPORT_SRS_LMT
    AUD_OPL_SRS_LMT,
    #endif

    AUD_OPL_NUM
};

void vOplFuncInit(void)
{
    UINT32 *u4Ptr;
    UINT32 u4NormalDramAddr=ADDR_RC2D_OPL_FUNC_NORMAL_BASE;
    UINT32 u4CmptDramAddr=DRAM_OPL_FUNC_CMPT_BASE;
    UINT32 u4VctOffset, u4NormalSize, u4CmptSize;
    UINT8 u1Idx=0;

    u4Ptr = (UINT32 *)_arMtkOplFuncTab;
    // skip the last element: AUD_OPL_END
    // skip the last element: AUD_OPL_END
    //while (*(u4Ptr+u1Idx) != AUD_OPL_NUM && u1Idx < AUD_OPL_NUM)
    while ((u1Idx < AUD_OPL_NUM) && (*(u4Ptr+u1Idx) != AUD_OPL_NUM)) //Klocwork require, check boundary first, then acces array.
    {
        u4VctOffset = ADDR_RC2D_OPL_FUNC_VCT_BASE +
            _au4OptlFuncTableSize[*(u4Ptr+u1Idx)][0];
        u4NormalSize = _au4OptlFuncTableSize[*(u4Ptr+u1Idx)][1];
        u4CmptSize = _au4OptlFuncTableSize[*(u4Ptr+u1Idx)][2];
        if (u4NormalSize > 0)
        {
            WriteDspCommDram(AUD_DSP0, u4VctOffset, u4NormalDramAddr);
            u4VctOffset++;
            u4NormalDramAddr += u4NormalSize;
        }

        if (u4CmptSize > 0)
        {
            WriteDspCommDram(AUD_DSP0, u4VctOffset, u4CmptDramAddr);
            u4CmptDramAddr += u4CmptSize;
        }

        u1Idx++;

    }

}


#define ROM_ADDR      0x0      // ROM
#define CRI_ADDR      0x2800   // Critical
#define COMMA_ADDR    0x1000   // CommonA
#define COMMB_ADDR    0x4000   // CommonB  0x14000
#define COMMAEXT_ADDR    0x3000   // CommonAEXT
#define DEC0_ADDR     0x8000   // Job0
#define DEC1_ADDR     0xC000   // Job1
#define DEC2_ADDR     0x4000   // Job2
#define DEC3_ADDR     0xA000   // Job3  0x1A000
#define ENC2_ADDR     0xD000   // ENCODER2  0x1D000

#define POST_VCT_ADDR 0x1000   // Post Vect  0x11000
//#if defined(DSP_SUPPORT_SRSVIQ) || defined(DSP_SUPPORT_SRSTSHD) || defined(DSP_SUPPORT_SRSCC3D)
//#define POST1_ADDR                  0x1010   // Post 1
//#define POST2_ADDR                0x1510   // Post 2
//#define POST3_ADDR             0x2b7e   // Post 3
//#define POST4_ADDR               0x2b80   // Post 4
//#else
#define POST1_ADDR                  0x1010   // Post 1
#define POST2_ADDR                0x1510   // Post 2
#define POST3_ADDR             0x2b10   // Post 3
#define POST4_ADDR               0x2e80   // Post 4
//#endif


UINT32 _u4DspPraStart[DSPF_SRS_CC3D_SA/2+1]=
{

// Define Item Index
//MT5365                                0,1
//#define DSPF_IMAGE_SA                   0
//#define DSPF_IMAGE_SZ                   1
  0xff,
//Comm\CommRamCode.pra                  2,3
//#define DSPF_COMM_CODE_SA               2
//#define DSPF_COMM_CODE_SZ               3
  COMMA_ADDR,
//Comm\ROMCode.pra                      4,5
//#define DSPF_ROM_CODE_SA                4
//#define DSPF_ROM_CODE_SZ                5
  ROM_ADDR,
//ROM\DROM_V700000.data                 6,7
//#define DSPF_DROM_SA                    6
//#define DSPF_DROM_SZ                    7
  0xff,
//ROM\PROM_V700000.data                 8,9
//#define DSPF_PROM_SA                    8
//#define DSPF_PROM_SZ                    9
  0xff,
//Ain\AinIntRam.pra                     10,11
//#define DSPF_AIN_INTRAM_SA              10
//#define DSPF_AIN_INTRAM_SZ              11
  0xff,
//Ain\AinIntRam_Miner.pra               12,13
//#define DSPF_AIN_INTRAM_MINER_SA        12
//#define DSPF_AIN_INTRAM_MINER_SZ        13
  0xff,
//Critical\critical.pra    		        14, 15
//#define DSPF_CRITICAL_CODE_SA           14
//#define DSPF_CRITICAL_CODE_SZ           15
  CRI_ADDR,
//Dec1\NTSCDec1.pra                     16,17
//#define DSPF_NTSC_DEC1_SA               16
//#define DSPF_NTSC_DEC1_SZ               17
  DEC0_ADDR,
//Dec2\NTSCDec2.pra                     18,19
//#define DSPF_NTSC_DEC2_SA               18
//#define DSPF_NTSC_DEC2_SZ               19
  DEC1_ADDR,
//Tbl\ntscdec.bin                       20,21
//#define DSPF_NTSCTBL_SA                 20
//#define DSPF_NTSCTBL_SZ                 21
  0xff,
//Dec1\DetectorDec1.pra                 22,23
//#define DSPF_DETECTOR_DEC1_SA           22
//#define DSPF_DETECTOR_DEC1_SZ           23
  DEC0_ADDR,
//Dec2\DetectorDec2.pra                 24,25
//#define DSPF_DETECTOR_DEC2_SA           24
//#define DSPF_DETECTOR_DEC2_SZ           25
  DEC1_ADDR,
//Tbl\Detector.data                     26,27
//#define DSPF_DETECTORTBL_SA             26
//#define DSPF_DETECTORTBL_SZ             27
  0xff,
//Dec1\A2Dec1.pra                       28,29
//#define DSPF_A2_DEC1_SA                 28
//#define DSPF_A2_DEC1_SZ                 29
  DEC0_ADDR,
//Dec2\A2Dec2.pra                       30,31
//#define DSPF_A2_DEC2_SA                 30
//#define DSPF_A2_DEC2_SZ                 31
  DEC1_ADDR,
//Tbl\A2.data                           32,33
//#define DSPF_A2TBL_SA                   32
//#define DSPF_A2TBL_SZ                   33
  0xff,
//Dec1\PALDec1.pra                      34,35
//#define DSPF_PAL_DEC1_SA                34
//#define DSPF_PAL_DEC1_SZ                35
  DEC0_ADDR,
//Dec2\PALDec2.pra                      36,37
//#define DSPF_PAL_DEC2_SA                36
//#define DSPF_PAL_DEC2_SZ                37
  DEC1_ADDR,
//Tbl\PAL.data                          38,39
//#define DSPF_PALTBL_SA                  38
//#define DSPF_PALTBL_SZ                  39
  0xff,
//Dec1\fmfmdec1.pra                     40,41
//#define DSPF_FMFM_DEC1_SA               40
//#define DSPF_FMFM_DEC1_SZ               41
  DEC0_ADDR,
//Dec2\fmfmdec2.pra                     42,43
//#define DSPF_FMFM_DEC2_SA               42
//#define DSPF_FMFM_DEC2_SZ               43
  DEC1_ADDR,
//Tbl\fmfmdec.data                      44,45
//#define DSPF_FMFMTBL_SA                 44
//#define DSPF_FMFMTBL_SZ                 45
  0xff,
//Dec1\MinerDec1.pra                    46,47
//#define DSPF_MINER_DEC1_SA              46
//#define DSPF_MINER_DEC1_SZ              47
  DEC0_ADDR,
//Dec2\MinerDec2.pra                    48,49
//#define DSPF_MINER_DEC2_SA              48
//#define DSPF_MINER_DEC2_SZ              49
  DEC1_ADDR,
//Tbl\MinerTbl.data                     50,51
//#define DSPF_MINERTBL_SA                50
//#define DSPF_MINERTBL_SZ                51
  0xff,
//Dec1\fmrdodec1.pra                    52,53
//#define DSPF_FMRDODEC1_SA               52
//#define DSPF_FMRDODEC1_SZ               53
  DEC0_ADDR,
//Dec1\fmrdodetdec1.pra                 54,55
//#define DSPF_FMRDODETDEC1_SA            54
//#define DSPF_FMRDODETDEC1_SZ            55
  DEC0_ADDR,
//Tbl\FMRadio.data                      56,57
//#define DSPF_FMRDOTBL_SA                56
//#define DSPF_FMRDOTBL_SZ                57
  0xff,
//Tbl\atv_cos_tab.data                  58,59
//#define DSPF_ATV_COSTBL_SA              58
//#define DSPF_ATV_COSTBL_SZ              59
  0xff,

//Dec1\pcmdec1.pra                      60,61
//#define DSPF_PCM_DEC1_SA                60
//#define DSPF_PCM_DEC1_SZ                61
  DEC0_ADDR,
//Dec2\pcmdec2.pra                      62,63
//#define DSPF_PCM_DEC2_SA                62
//#define DSPF_PCM_DEC2_SZ                63
  DEC1_ADDR,
//Dec3\pcmdec3.pra                      64,65
//#define DSPF_PCM_DEC3_SA                64
//#define DSPF_PCM_DEC3_SZ                65
  DEC2_ADDR,
//Dec1\mp2dec1.pra                      66,67
//#define DSPF_MP2_DEC1_SA                66
//#define DSPF_MP2_DEC1_SZ                67
  DEC0_ADDR,
//Dec2\mp2dec2.pra                      68,69
//#define DSPF_MP2_DEC2_SA                68
//#define DSPF_MP2_DEC2_SZ                69
  DEC1_ADDR,
//Dec3\mp2dec3.pra                      70,71
//#define DSPF_MP2_DEC3_SA                70
//#define DSPF_MP2_DEC3_SZ                71
  DEC2_ADDR,
//Tbl\mp2tbl.bin                        72,73
//#define DSPF_MP2_TABLE_SA               72
//#define DSPF_MP2_TABLE_SZ               73
  0xff,
//Dec1\mp3dec1.pra                      74,75
//#define DSPF_MP3_DEC1_SA                74
//#define DSPF_MP3_DEC1_SZ                75
  DEC0_ADDR,
//Tbl\mp3tbl_5371.bin                   76,77
//#define DSPF_MP3_TABLE_SA               76
//#define DSPF_MP3_TABLE_SZ               77
  0xff,
//Dec1\ac3dec1.pra                      78,79
//#define DSPF_AC3_DEC1_SA                78
//#define DSPF_AC3_DEC1_SZ                79
  DEC0_ADDR,
//Dec2\ac3dec2.pra                      80,81
//#define DSPF_AC3_DEC2_SA                80
//#define DSPF_AC3_DEC2_SZ                81
  DEC1_ADDR,
//Dec3\ac3dec3.pra                      82,83
//#define DSPF_AC3_DEC3_SA                82
//#define DSPF_AC3_DEC3_SZ                83
  DEC2_ADDR,
//Tbl\ac3_tbl.bin                       84,85
//#define DSPF_AC3_TABLE_SA               84
//#define DSPF_AC3_TABLE_SZ               85
  0xff,
//Dec1\aacdec1.pra                      86,87
//#define DSPF_AAC_DEC1_SA                86
//#define DSPF_AAC_DEC1_SZ                87
  DEC0_ADDR,
//Dec2\aacdec2.pra                      88,89
//#define DSPF_AAC_DEC2_SA                88
//#define DSPF_AAC_DEC2_SZ                89
  DEC1_ADDR,
//Dec3\aacdec3.pra                      90,91
//#define DSPF_AAC_DEC3_SA                90
//#define DSPF_AAC_DEC3_SZ                91
  DEC2_ADDR,
//Tbl\aac_dram.bin    		            92,93
//#define DSPF_AAC_CMPT_TABLE_SA          92
//#define DSPF_AAC_CMPT_TABLE_SZ          93
  0xff,
//Tbl\aac_dram.data                     94,95
//#define DSPF_AAC_TABLE_SA               94
//#define DSPF_AAC_TABLE_SZ               95
  0xff,
//Dec1\ddco.pra                         96,97
//#define DSPF_DDCO_SA                    96
//#define DSPF_DDCO_SZ                    97
  ENC2_ADDR,
//Tbl\ddco_tbl.bin                      98,99
//#define DSPF_DDCO_TBL_SA                98
//#define DSPF_DDCO_TBL_SZ                99
  0xff,
//Dec1\wmadec1.pra                      100,101
//#define DSPF_WMA_DEC1_SA                100
//#define DSPF_WMA_DEC1_SZ                101
  DEC0_ADDR,
//Tbl\wma_cmpt.bin                      102,103
//#define DSPF_WMA_C_TABLE_SA             102
//#define DSPF_WMA_C_TABLE_SZ             103
  0xff,
//Tbl\wma_norm.bin                      104,105
//#define DSPF_WMA_N_TABLE_SA             104
//#define DSPF_WMA_N_TABLE_SZ             105
  0xff,
//Dec1\lpcmdec1.pra                     106,107
//#define DSPF_LPCM_DEC1_SA               106
//#define DSPF_LPCM_DEC1_SZ               107
  DEC0_ADDR,
//Tbl\lpcm_tbl.bin                      108.109
//#define DSPF_LPCM_TBL_SA                108
//#define DSPF_LPCM_TBL_SZ                109
  0xff,
//Dec1\pink_noise.pra                   110,111
//#define DSPF_PNOISE_SA                  110
//#define DSPF_PNOISE_SZ                  111
  DEC0_ADDR,
//Dec1\sbcdec1.data                     112,113
//#define DSPF_SBCDEC1_SA                 112
//#define DSPF_SBCDEC1_SZ                 113
  0xff,
//Tbl\sbctbl.bin                        114,115
//#define DSPF_SBC_TABLE_SA               114
//#define DSPF_SBC_TABLE_SZ               115
  0xff,
//Dec2\mp3enc_dec2.pra                  116,117
//#define DSPF_MP3ENC_DEC2_SA	            116		///[Joel]20090311
//#define DSPF_MP3ENC_DEC2_SZ	            117
  DEC1_ADDR,
//Tbl\mp3enc_tbl.bin                    118,119
//#define DSPF_MP3ENC_TABLE_SA	        118		///[Joel]20090311
//#define DSPF_MP3ENC_TABLE_SZ	        119
  0xff,
//Dec1\cookdec1.pra                     120,121
//#define DSPF_COOK_DEC1_SA               120
//#define DSPF_COOK_DEC1_SZ               121
  DEC0_ADDR,
//Tbl\cooktbl.bin                       122,123
//#define DSPF_COOKTBL_SA                 122
//#define DSPF_COOKTBL_SZ                 123
  0xff,

//Post\postvect.pra                     124,125
//#define DSPF_POST_VECT_SA               124
//#define DSPF_POST_VECT_SZ               125
  POST_VCT_ADDR,
//Post\pl2.pra                          126,127
//#define DSPF_PL2_POST_SA                126
//#define DSPF_PL2_POST_SZ                127
  POST1_ADDR,
//Tbl\pl2_tbl.bin                       128,129
//#define DSPF_PL2_TABLE_SA               128
//#define DSPF_PL2_TABLE_SZ               129
  0xff,
//Post\vsurr.pra                        130,131
//#define DSPF_VSURR_POST_SA              130
//#define DSPF_VSURR_POST_SZ              131
  POST2_ADDR,
//Post\bbe_viv4.pra                     132,133
//#define DSPF_BBE_VIVA4_SA               132
//#define DSPF_BBE_VIVA4_SZ               133
  POST3_ADDR,
//Post\SRS\srs_viq.pra                          134,135
//#define DSPF_VIQ_SA                     134
//#define DSPF_VIQ_SZ                     135
  POST4_ADDR,
//Tbl\srs_viq_tbl.bin                       136,137
//#define DSPF_VIQ_TBL_SA                 136
//#define DSPF_VIQ_TBL_SZ                 137
  0xff,
//Dec4\sbcenc4.pra                      138,139
//#define DSPF_SBCENC4_SA                 138
//#define DSPF_SBCENC4_SZ                 139
  DEC3_ADDR,
//tbl\sbcenc_tbl.bin                    140,141
//#define DSPF_SBCENC_TABLE_SA	        140
//#define DSPF_SBCENC_TABLE_SZ	        141
  0xff,
//Post\adv.pra                          142,143
//#define DSPF_ADV_SA                     142
//#define DSPF_ADV_SZ                     143
  0xff,
//Tbl\adv_tbl.bin                       144,145
//#define DSPF_ADV_TBL_SA                 144
//#define DSPF_ADV_TBL_SZ                 145
  0xff,
//#endif
//Dec1\G711dec1.pra                     146,147
//#define DSPF_G711DEC1_SA                146
//#define DSPF_G711DEC1_SZ                147
  DEC0_ADDR,
//Dec1\G711enc1.pra                     148,149
//#define DSPF_G711ENC1_SA                148
//#define DSPF_G711ENC1_SZ                149
  DEC0_ADDR,
//ROM\DROM_V65_comm.data                150, 151
//#define DSPF_DROM_COMM_SA               150
//#define DSPF_DROM_COMM_SZ               151
  0xff,
//ROM\DROM_V65_mts.data                 152, 153
//#define DSPF_DROM_MTS_SA                152
//#define DSPF_DROM_MTS_SZ                153
  0xff,
//ROM\DROM_V65_ac3.data                 154, 155
//#define DSPF_DROM_AC3_SA                154
//#define DSPF_DROM_AC3_SZ                155
  0xff,
//ROM\DROM_V65_mpeg.data                156, 157
//#define DSPF_DROM_MPEG_SA               156
//#define DSPF_DROM_MPEG_SZ               157
  0xff,
//ROM\DROM_V65_aac.data                 158, 159
//#define DSPF_DROM_AAC_SA                158
//#define DSPF_DROM_AAC_SZ                159
  0xff,
//Dec1\dtsdec1.pra                      160, 161
//#define DSPF_DTS_DEC1_SA                160
//#define DSPF_DTS_DEC1_SZ                161
  DEC0_ADDR,
//Dec2\dtsdec2.pra                      162, 163
//#define DSPF_DTS_DEC2_SA                162
//#define DSPF_DTS_DEC2_SZ                163
  DEC1_ADDR,
//+tbl\dts_huff_tbl.bin                 164, 165
//#define DSPF_DTS_HUFF_TABLE_SA          164
//#define DSPF_DTS_HUFF_TABLE_SZ          165
  0xff,
//+tbl\dts_adpcm_vq_tbl.bin              166, 167
//#define DSPF_DTS_ADPCM_VQ_TABLE_SA      166
//#define DSPF_DTS_ADPCM_VQ_TABLE_SZ      167
  0xff,
//+tbl\dts_rom_tbl.bin                  168, 169
//#define DSPF_DTS_ROM_TABLE_SA           168
//#define DSPF_DTS_ROM_TABLE_SZ           169
  0xff,
//Tbl\dts_BC_tbl.bin      170, 171
//#define DSPF_DTS_BC_SA                     170
//#define DSPF_DTS_BC_SZ                     171
  0xff,


//Dec2\mp3dec2.pra                     172, 173
//#define DSPF_MP3_DEC2_SA                172
//#define DSPF_MP3_DEC2_SZ                173
  DEC1_ADDR,
//Dec2\lpcmdec2.pra                      174, 175
//#define DSPF_LPCM_DEC2_SA               174
//#define DSPF_LPCM_DEC2_SZ               175
  DEC1_ADDR,
//Dec1\dra_dec1.pra                     176, 177
//#define DSPF_DRA_DEC1_SA                176
//#define DSPF_DRA_DEC1_SZ                177
  DEC0_ADDR,
//Tbl\dra_tbl.bin                        178, 179
//#define DSPF_DRA_TABLE_SA               178
//#define DSPF_DRA_TABLE_SZ               179
  0xff,
//Dec2\dra_dec2.pra                      180, 181
//#define DSPF_DRA_DEC2_SA                180
//#define DSPF_DRA_DEC2_SZ                181
  DEC1_ADDR,
//Dec3\mp2dec3vct.pra                  182, 183
//#define DSPF_VCT_MP2DEC3_SA             182
//#define DSPF_VCT_MP2DEC3_SZ             183
  DEC2_ADDR,
//Dec3\aacdec3vct.pra                   184, 185
//#define DSPF_VCT_AACDEC3_SA             184
//#define DSPF_VCT_AACDEC3_SZ             185
  DEC2_ADDR,
//Dec3\ac3dec3vct.pra                    186,187
//#define DSPF_VCT_AC3DEC3_SA             186
//#define DSPF_VCT_AC3DEC3_SZ             187
  DEC2_ADDR,
//Dec3\pcmdec3vct.pra                   188,189
//#define DSPF_VCT_PCMDEC3_SA             188
//#define DSPF_VCT_PCMDEC3_SZ             189
  DEC2_ADDR,

//Dec3\mp3encdec3.pra                    190,191
//#define DSPF_MP3ENC_DEC3_SA             190
//#define DSPF_MP3ENC_DEC3_SZ             191
  DEC2_ADDR,
//Dec3\mp3encdec3vct.pra         192,193
//#define DSPF_VCT_MP3ENC_DEC3_SA         192
//#define DSPF_VCT_MP3ENC_DEC3_SZ         193         ///MP3ENC_SUPPORT
  DEC2_ADDR,
//Dec2\g729dec2.pra                    194,195
//#define DSPF_G729DEC2_SA                194
//#define DSPF_G729DEC2_SZ                195
  DEC1_ADDR,
//Tbl\g729dec_dram.data                        196,197
//#define DSPF_G729DEC_TABLE_SA               196
//#define DSPF_G729DEC_TABLE_SZ               197
  0xff,
//Dec1\g729dec1.pra                     198,199
//#define DSPF_G729DEC1_SA                198
//#define DSPF_G729DEC1_SZ                199
  DEC0_ADDR,
//Post\cdnotch.pra                      200,201
//#define DSPF_CDNOTCH_SA					200
//#define DSPF_CDNOTCH_SZ					201
  POST4_ADDR,
//Post\vbass.pra                        202,203
//#define DSPF_VBASS_SA					202
//#define DSPF_VBASS_SZ					203
  POST4_ADDR,
//Post\SRS\SRS_TSHDM.pra                204,205
//#define DSPF_SRS_TSHD_SA                204
//#define DSPF_SRS_TSHD_SZ                205
  POST2_ADDR,
//Dec3\dtsenc.pra
//#define DSPF_DTSENC_SA					206
//#define DSPF_DTSENC_SZ					207
  ENC2_ADDR,
//Tbl\ddco_tb3.bin                      208,209
//#define DSPF_DDCO_TB3_SA                208
//#define DSPF_DDCO_TB3_SZ                209
  0xff,
//Post\spkheight.pra                    210,211
//#define DSPF_SPKHEIGHT_SA               210
//#define DSPF_SPKHEIGHT_SZ               211
  POST3_ADDR,
//Tbl\ddco_tb4.bin                      212,213
//#define DSPF_DDCO_TB4_SA                212
//#define DSPF_DDCO_TB4_SZ                213
  0xff,
//Dec3\ddpconv.pra                      214,215
//#define DSPF_DDPCONV_SA                 214
//#define DSPF_DDPCONV_SZ                 215
  DEC2_ADDR,
//Dec4\pcmdec4.pra                      216,217
//#define DSPF_PCM_DEC4_SA                216
//#define DSPF_PCM_DEC4_SZ                217
  DEC3_ADDR,
//Dec1\vorbisdec.pra
//#define DSPF_VORBIS_DEC1_SA             218 ///CC_VORBIS_SUPPORT
//#define DSPF_VORBIS_DEC1_SZ             219
  DEC0_ADDR,
//Tbl\vorbis_cmpt.bin
//#define DSPF_VORBIS_CTBL_SA             220 ///CC_VORBIS_SUPPORT
//#define DSPF_VORBIS_CTBL_SZ             221
  0xff,
//Tbl\vorbis_norm.bin
//#define DSPF_VORBIS_NTBL_SA             222 ///CC_VORBIS_SUPPORT
//#define DSPF_VORBIS_NTBL_SZ             223
  0xff,
//Dec2\vorbisdec2.pra
//#define DSPF_VORBIS_DEC2_SA             224 ///CC_VORBIS_SUPPORT
//#define DSPF_VORBIS_DEC2_SZ             225
  DEC1_ADDR,
//Tbl\pts_info.data
//#define DSPF_PTSINFO_SA             226
//#define DSPF_PTSINFO_SZ             227
  0xff,
#ifndef DSP_SUPPORT_DUAL_DECODE
//Dec3\aacdec2ext.pra
//#define DSPF_AAC_DEC2EXT_SA             228
//#define DSPF_AAC_DEC2EXT_SZ             229
  DEC2_ADDR,
#else
//Enc2\aacdecext.pra
//#define DSPF_AAC_DECEXT_SA             228
//#define DSPF_AAC_DECEXT_SZ             229
  ENC2_ADDR,
#endif
//Dec1\wmaprodec1.pra
//#define DSPF_WMAPRO_DEC1_SA                230
//#define DSPF_WMAPRO_DEC1_SZ                231
  DEC0_ADDR,
//Tbl\wmapro_cmpt.bin
//#define DSPF_WMAPRO_C_TABLE_SA             232
//#define DSPF_WMAPRO_C_TABLE_SZ             233
  0xff,
//Tbl\wmapro_norm.bin
//#define DSPF_WMAPRO_N_TABLE_SA             234
//#define DSPF_WMAPRO_N_TABLE_SZ             235
  0xff,
//Tbl\ddco_crctab.bin, for test mode only
//#define DSPF_DDCO_CRC_TABLE_SA             236
//#define DSPF_DDCO_CRC_TABLE_SZ             237
  0xff,
//Post\SRS\srs_wowhd.pra
//#define DSPF_SRS_WOWHD_SA                238
//#define DSPF_SRS_WOWHD_SZ                239
  POST2_ADDR,
//Post\keyshift.pra
//#define DSPF_KEYSHIFT_SA                240
//#define DSPF_KEYSHIFT_SZ                241
  POST4_ADDR,
//Dec1\amrdec1.pra
//#define DSPF_AMRDEC1_SA                 242
//#define DSPF_AMRDEC1_SZ                 243
  DEC0_ADDR,
//Tbl\amr_tbl.bin
//#define DSPF_AMR_TBL_SA                 244
//#define DSPF_AMR_TBL_SZ                 245
  0xff,
//Tbl\amr_mode_tbl.bin
//#define DSPF_AMR_MODE_TBL_SA            246
//#define DSPF_AMR_MODE_TBL_SZ            247
  0xff,
//Dec1\awbdec1.pra
//#define DSPF_AWBDEC1_SA                 248
//#define DSPF_AWBDEC1_SZ                 249
  DEC0_ADDR,
//Tbl\awb_tbl.bin
//#define DSPF_AWB_TBL_SA                 250
//#define DSPF_AWB_TBL_SZ                 251
  0xff,
//Tbl\awb_mode_tbl.bin
//#define DSPF_AWB_MODE_TBL_SA            252
//#define DSPF_AWB_MODE_TBL_SZ            253
  0xff,
//Dec1\flacdec1.pra, paul_flac
//#define DSPF_FLAC_DEC1_SA                254
//#define DSPF_FLAC_DEC1_SZ                255
  DEC0_ADDR,
//Tbl\flac_tbl.bin
//#define DSPF_FLAC_TABLE_SA               256
//#define DSPF_FLAC_TABLE_SZ               257
  0xff,
// APE decoder
//Dec1\apedec1.pra
//#define DSPF_APE_DEC1_SA                258
//#define DSPF_APE_DEC1_SZ                259
  DEC0_ADDR,
//Comm\CommRamCodeAExt.pra
//#define DSPF_COMM_EXT_CODE_SA           260
//#define DSPF_COMM_EXT_CODE_SZ           261
  COMMAEXT_ADDR,
//Comm\CommRamCodeB.pra
//#define DSPF_COMMB_CODE_SA              262
//#define DSPF_COMMB_CODE_SZ              263
  COMMB_ADDR,
//Dec3\lpcmdec3.pra
//#define DSPF_LPCM_DEC3_SA               264    //lpcm_dec3
//#define DSPF_LPCM_DEC3_SZ               265
  DEC2_ADDR,
//Dec2\apedec2.pra
//#define DSPF_APE_DEC2_SA                266
//#define DSPF_APE_DEC2_SZ                267
  DEC1_ADDR,
//Dec3\apede3.pra
//#define DSPF_APE_DEC3_SA                268    //lpcm_dec3
//#define DSPF_APE_DEC3_SZ                269
  DEC2_ADDR,
//mp3 encdec4
//#define DSPF_MP3ENC_DEC4_SA             270
//#define DSPF_MP3ENC_DEC4_SZ             271
  DEC3_ADDR,
//Dec3\dtsdec3.pra
//#define DSPF_DTS_DEC3_SA                272
//#define DSPF_DTS_DEC3_SZ                273
  DEC2_ADDR,
//tbl\dts_enc_tbl.pra
//#define DSPF_DTS_ENC_TBL_SA             274
//#define DSPF_DTS_ENC_TBL_SZ             275
  0xff,
//tbl\dts_enc_rom_tbl.pra
//#define DSPF_DTS_ENC_ROM_TBL_SA         276
//#define DSPF_DTS_ENC_ROM_TBL_SZ         277
  0xff,
//Dec2\amrdec2.pra
//#define DSPF_AMRDEC2_SA                 278
//#define DSPF_AMRDEC2_SZ                 279
  DEC1_ADDR,
//Dec3\amrdec3.pra
//#define DSPF_AMRDEC3_SA                 280
//#define DSPF_AMRDEC3_SZ                 281
  DEC2_ADDR,
//Dec2\awbdec2.pra
//#define DSPF_AWBDEC2_SA                 282
//#define DSPF_AWBDEC2_SZ                 283
  DEC1_ADDR,
//Dec3\awbdec3.pra
//#define DSPF_AWBDEC3_SA                 284
//#define DSPF_AWBDEC3_SZ                 285
  DEC2_ADDR,

//Dec3\mp3dec3.pra
//#define DSPF_MP3_DEC3_SA                 286
//#define DSPF_MP3_DEC3_SZ                 287
  DEC2_ADDR,

//Dec3\NTSCDec3.pra                     288,289
//#define DSPF_NTSC_DEC3_SA               288
//#define DSPF_NTSC_DEC3_SZ               289
  DEC2_ADDR,

//Dec3\DetectorDec3.pra                 290,291
//#define DSPF_DETECTOR_DEC3_SA           290
//#define DSPF_DETECTOR_DEC3_SZ           291
  DEC2_ADDR,

//Dec3\A2Dec3.pra                       292,293
//#define DSPF_A2_DEC3_SA                 292
//#define DSPF_A2_DEC3_SZ                 293
  DEC2_ADDR,

//Dec3\PALDec3.pra                      294,295
//#define DSPF_PAL_DEC3_SA                294
//#define DSPF_PAL_DEC3_SZ                295
  DEC2_ADDR,

//Dec3\vorbisdec3.pra                   296,297
//#define DSPF_VORBIS_DEC3_SA             296     ///CC_VORBIS_SUPPORT
//#define DSPF_VORBIS_DEC3_SZ             297
  DEC2_ADDR,

//#define DSPF_WMA_DEC2_SA                298
//#define DSPF_WMA_DEC2_SZ                299
  DEC1_ADDR,

//#define DSPF_WMA_DEC3_SA                300
//#define DSPF_WMA_DEC3_SZ                301
  DEC2_ADDR,

//#define DSPF_COOK_DEC2_SA               302
//#define DSPF_COOK_DEC2_SZ               303    //cook_dec2
  DEC1_ADDR,

//#define DSPF_COOK_DEC3_SA               304
//#define DSPF_COOK_DEC3_SZ               305    //cook_dec3
  DEC2_ADDR,

//#define DSPF_WMAPRO_DEC2_SA               306
//#define DSPF_WMAPRO_DEC2_SZ               307
  DEC1_ADDR,

//#define DSPF_WMAPRO_DEC3_SA               308
//#define DSPF_WMAPRO_DEC3_SZ               309
  DEC2_ADDR,

//Dec2\flacdec2.pra paul_flac_2
//#define DSPF_FLAC_DEC2_SA               310
//#define DSPF_FLAC_DEC2_SZ               311
  DEC1_ADDR,
//Dec3\flacdec3.pra paul_flac_3
//#define DSPF_FLAC_DEC3_SA               312
//#define DSPF_FLAC_DEC3_SZ               313
  DEC2_ADDR,

//#define DSPF_DRA_DEC3_SA               314
//#define DSPF_DRA_DEC3_SZ               315
  DEC2_ADDR,

//#define DSPF_G726_DEC1_SA               316
//#define DSPF_G726_DEC1_SZ               317
  DEC0_ADDR,

//#define DSPF_G726_DEC2_SA               318
//#define DSPF_G726_DEC2_SZ               319
  DEC1_ADDR,

//#define DSPF_G726_DEC3_SA               320
//#define DSPF_G726_DEC3_SZ               321
  DEC2_ADDR,

//#define DSPF_G726_TBL_SA               322
//#define DSPF_G726_TBL_SZ               323
  0xff,

//Post\SRS\SRS_CC3D.pra
//#define DSPF_SRS_CC3D_SA               324
//#define DSPF_SRS_CC3D_SZ               325
  POST2_ADDR,

};


extern void CheckSumIt(UINT16 u2StartPc, UINT32 u4CodeSA, UINT32 u4CodeSZ);
extern void CheckSumItDynam(UINT16 u2StartPc, UINT32 u4CodeSA, UINT32 u4CodeSZ, UINT32 u4ImgAddr,UINT32 u4ImageSz);

#ifdef DSP_NEW_SCRAMBLE
void CheckSumAll(void)
{
    UINT32 u2Idx;
    UINT16 u2Size;
    UINT32 u4ImgAddr, u4ImageSz;
    UINT32 u4sizepbd;
    UINT32 *ptr;
    UINT32 i;

    //MID_DisableRegionProtect(_u4adspRegion);

    u2Size = sizeof(_u4DspPraStart)/4;

    for (u2Idx=0; u2Idx<u2Size; u2Idx++)
    {
        // !!!!!!!!! Try this for use API in pre-built binary !!!!!!
        CheckSumIt(_u4DspPraStart[u2Idx], (u2Idx*2), (u2Idx*2+1));
    }
    u4sizepbd = u4GetDspBinarySize(ADSP_BIN_MAIN)/4;
    ptr = (UINT32 *)u4GetDspBinaryAddr(ADSP_BIN_MAIN);
    _u4pbdchecksum = 0;
    for(i= 0; i < u4sizepbd; i++)
    {
	    _u4pbdchecksum ^= ptr[i];
	}
    Printf("_u4pbdchecksum is : 0x%x\n", _u4pbdchecksum);
#if defined(CC_AUD_DOLBY_SUPPORT_DDC)
    u4ImgAddr = (UINT32)u4GetDspBinaryAddr(ADSP_BIN_DYNLD_DDC);
    u4ImageSz = u4GetDspBinarySize(ADSP_BIN_DYNLD_DDC);
    for (u2Idx=0; u2Idx<u2Size; u2Idx++)
    {
        // !!!!!!!!! Try this for use API in pre-built binary !!!!!!
        CheckSumItDynam(_u4DspPraStart[u2Idx], (u2Idx*2), (u2Idx*2+1), u4ImgAddr, u4ImageSz);
    }
    u4sizepbd = u4GetDspBinarySize(ADSP_BIN_DYNLD_DDC)/4;
    ptr = (UINT32 *)u4GetDspBinaryAddr(ADSP_BIN_DYNLD_DDC);
    _u4_pbD_DDCchecksum = 0;
    for(i= 0; i < u4sizepbd; i++)
    {
	    _u4_pbD_DDCchecksum ^= ptr[i];
	}
    Printf("_u4_pbD_DDCchecksum is : 0x%x\n", _u4_pbD_DDCchecksum);

#elif defined(CC_AUD_DOLBY_SUPPORT_DDP)
    u4ImgAddr = (UINT32)u4GetDspBinaryAddr(ADSP_BIN_DYNLD_DCV);
    u4ImageSz = u4GetDspBinarySize(ADSP_BIN_DYNLD_DCV);
    for (u2Idx=0; u2Idx<u2Size; u2Idx++)
    {
        // !!!!!!!!! Try this for use API in pre-built binary !!!!!!
        CheckSumItDynam(_u4DspPraStart[u2Idx], (u2Idx*2), (u2Idx*2+1), u4ImgAddr, u4ImageSz);
    }
    u4sizepbd = u4GetDspBinarySize(ADSP_BIN_DYNLD_DCV)/4;
    ptr = (UINT32 *)u4GetDspBinaryAddr(ADSP_BIN_DYNLD_DCV);
    _u4_pbD_DCVchecksum = 0;
    for(i= 0; i < u4sizepbd; i++)
    {
	    _u4_pbD_DCVchecksum ^= ptr[i];
	}
    Printf("_u4_pbD_DCVchecksum is : 0x%x\n", _u4_pbD_DCVchecksum);

#elif defined(CC_AUD_DOLBY_SUPPORT_DD)
    u4ImgAddr = (UINT32)u4GetDspBinaryAddr(ADSP_BIN_DYNLD_DD);
    u4ImageSz = u4GetDspBinarySize(ADSP_BIN_DYNLD_DD);
    for (u2Idx=0; u2Idx<u2Size; u2Idx++)
    {
        // !!!!!!!!! Try this for use API in pre-built binary !!!!!!
        CheckSumItDynam(_u4DspPraStart[u2Idx], (u2Idx*2), (u2Idx*2+1), u4ImgAddr, u4ImageSz);
    }
    u4sizepbd = u4GetDspBinarySize(ADSP_BIN_DYNLD_DD)/4;
    ptr = (UINT32 *)u4GetDspBinaryAddr(ADSP_BIN_DYNLD_DD);
    _u4_pbD_DDchecksum = 0;
    for(i= 0; i < u4sizepbd; i++)
    {
	    _u4_pbD_DDchecksum ^= ptr[i];
	}
    Printf("_u4_pbD_DDchecksum is : 0x%x\n", _u4_pbD_DDchecksum);

#endif
// AAC
#if defined(CC_AUD_DOLBY_SUPPORT_DDT)
    u4ImgAddr = (UINT32)u4GetDspBinaryAddr(ADSP_BIN_DYNLD_DDT);
    u4ImageSz = u4GetDspBinarySize(ADSP_BIN_DYNLD_DDT);
    for (u2Idx=0; u2Idx<u2Size; u2Idx++)
    {
        // !!!!!!!!! Try this for use API in pre-built binary !!!!!!
        CheckSumItDynam(_u4DspPraStart[u2Idx], (u2Idx*2), (u2Idx*2+1), u4ImgAddr, u4ImageSz);
    }
    u4sizepbd = u4GetDspBinarySize(ADSP_BIN_DYNLD_DDT)/4;
    ptr = (UINT32 *)u4GetDspBinaryAddr(ADSP_BIN_DYNLD_DDT);
    _u4_pbD_DDTchecksum = 0;
    for(i= 0; i < u4sizepbd; i++)
    {
	    _u4_pbD_DDTchecksum ^= ptr[i];
	}
    Printf("_u4_pbD_DDTchecksum is : 0x%x\n", _u4_pbD_DDTchecksum);

#elif defined(CC_AUD_DOLBY_SUPPORT_AAC)
    u4ImgAddr = (UINT32)u4GetDspBinaryAddr(ADSP_BIN_DYNLD_AAC);
    u4ImageSz = u4GetDspBinarySize(ADSP_BIN_DYNLD_AAC);
    for (u2Idx=0; u2Idx<u2Size; u2Idx++)
    {
        // !!!!!!!!! Try this for use API in pre-built binary !!!!!!
        CheckSumItDynam(_u4DspPraStart[u2Idx], (u2Idx*2), (u2Idx*2+1), u4ImgAddr, u4ImageSz);
    }
    u4sizepbd = u4GetDspBinarySize(ADSP_BIN_DYNLD_AAC)/4;
    ptr = (UINT32 *)u4GetDspBinaryAddr(ADSP_BIN_DYNLD_AAC);
    _u4_pbD_AACchecksum = 0;
    for(i= 0; i < u4sizepbd; i++)
    {
	    _u4_pbD_AACchecksum ^= ptr[i];
	}
    Printf("_u4_pbD_AACchecksum is : 0x%x\n", _u4_pbD_AACchecksum);

#endif
// DDCO
#if defined(CC_AUD_DOLBY_SUPPORT_DDCO)
    u4ImgAddr = (UINT32)u4GetDspBinaryAddr(ADSP_BIN_DYNLD_DDCO);
    u4ImageSz = u4GetDspBinarySize(ADSP_BIN_DYNLD_DDCO);
    for (u2Idx=0; u2Idx<u2Size; u2Idx++)
    {
        // !!!!!!!!! Try this for use API in pre-built binary !!!!!!
        CheckSumItDynam(_u4DspPraStart[u2Idx], (u2Idx*2), (u2Idx*2+1), u4ImgAddr, u4ImageSz);
    }
    u4sizepbd = u4GetDspBinarySize(ADSP_BIN_DYNLD_DDCO)/4;
    ptr = (UINT32 *)u4GetDspBinaryAddr(ADSP_BIN_DYNLD_DDCO);
    _u4_pbD_DDCOchecksum = 0;
    for(i= 0; i < u4sizepbd; i++)
    {
	    _u4_pbD_DDCOchecksum ^= ptr[i];
	}
    Printf("_u4_pbD_DDCOchecksum is : 0x%x\n", _u4_pbD_DDCOchecksum);

#endif


  //MID_EnableRegionProtect(_u4adspRegion);

}
#endif

