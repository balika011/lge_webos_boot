/*----------------------------------------------------------------------------*
 * No Warranty                                                                *
 * Except as may be otherwise agreed to in writing, no warranties of any      *
 * kind, whether express or implied, are given by MTK with respect to any MTK *
 * Deliverables or any use thereof, and MTK Deliverables are provided on an   *
 * "AS IS" basis.  MTK hereby expressly disclaims all such warranties,        *
 * including any implied warranties of merchantability, non-infringement and  *
 * fitness for a particular purpose and any warranties arising out of course  *
 * of performance, course of dealing or usage of trade.  Parties further      *
 * acknowledge that Company may, either presently and/or in the future,       *
 * instruct MTK to assist it in the development and the implementation, in    *
 * accordance with Company's designs, of certain softwares relating to        *
 * Company's product(s) (the "Services").  Except as may be otherwise agreed  *
 * to in writing, no warranties of any kind, whether express or implied, are  *
 * given by MTK with respect to the Services provided, and the Services are   *
 * provided on an "AS IS" basis.  Company further acknowledges that the       *
 * Services may contain errors, that testing is important and Company is      *
 * solely responsible for fully testing the Services and/or derivatives       *
 * thereof before they are used, sublicensed or distributed.  Should there be *
 * any third party action brought against MTK, arising out of or relating to  *
 * the Services, Company agree to fully indemnify and hold MTK harmless.      *
 * If the parties mutually agree to enter into or continue a business         *
 * relationship or other arrangement, the terms and conditions set forth      *
 * hereunder shall remain effective and, unless explicitly stated otherwise,  *
 * shall prevail in the event of a conflict in the terms in any agreements    *
 * entered into between the parties.                                          *
 *---------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * Copyright (c) 2004, MediaTek Inc.
 * All rights reserved.
 * 
 * Unauthorized use, practice, perform, copy, distribution, reproduction,
 * or disclosure of this information in whole or in part is prohibited.  
 *-----------------------------------------------------------------------------
 * $RCSfile: x_aud_enc.h,v $
 * $Revision: #1 $
 * $Date: 2015/01/09 $
 * $Author: dtvbm11 $
 *
 * Description:
 *         This header file contains Personal Video Recorder driver exported definitions.
 *---------------------------------------------------------------------------*/

#ifndef _X_AUD_ENC_H_
#define _X_AUD_ENC_H_

/*-----------------------------------------------------------------------------
                    include files
-----------------------------------------------------------------------------*/

#include "x_common.h"
#include "x_rm.h"
#include "x_mm_common.h"
#include "x_aud_dec.h"

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 -----------------------------------------------------------------------------*/

#define AUD_ENC_COMP_ID                    ( 3 )

/* Get operations */
#define AUD_ENC_GET_TYPE_CTRL              ( AUD_ENC_GET_TYPE_START + ( ( DRV_GET_TYPE_T )  0 ) )
#define AUD_ENC_GET_TYPE_SETTINGS          ( AUD_ENC_GET_TYPE_START + ( ( DRV_GET_TYPE_T )  1 ) )


/* Set operations */
#define AUD_ENC_SET_TYPE_CTRL              ( ( AUD_ENC_SET_TYPE_START + ( ( DRV_SET_TYPE_T) 0 ) ) | RM_SET_TYPE_ARG_NO_REF )
#define AUD_ENC_SET_TYPE_SETTINGS          ( AUD_ENC_SET_TYPE_START + ( ( DRV_SET_TYPE_T) 1 ) )
#define AUD_ENC_SET_TYPE_CHANNEL_NUMBER    ( ( AUD_ENC_SET_TYPE_START + ( ( DRV_SET_TYPE_T) 2 ) ) | RM_SET_TYPE_ARG_NO_REF )
#define AUD_ENC_SET_TYPE_SAMPLE_RATE       ( ( AUD_ENC_SET_TYPE_START + ( ( DRV_SET_TYPE_T) 3 ) ) | RM_SET_TYPE_ARG_NO_REF )
#define AUD_ENC_SET_TYPE_BITRATE           ( ( AUD_ENC_SET_TYPE_START + ( ( DRV_SET_TYPE_T) 4 ) ) | RM_SET_TYPE_ARG_NO_REF )


typedef enum
{
    AUD_ENC_SRC_TYPE_MAIN = 0,
    AUD_ENC_SRC_TYPE_AUX,
    AUD_ENC_SRC_TYPE_THIRD
} AUD_SRC_TYPE_T;

typedef enum
{
    AUD_ENC_POS_MODE_OFF = 0,
    AUD_ENC_POS_MODE_INPUT_BEFORE_POST_PROCESSING,
    AUD_ENC_POS_MODE_INPUT_AFTER_POST_PROCESSING,
    AUD_ENC_POS_MODE_NUM
}   AUD_ENC_POS_MODE_T;

typedef enum
{
    AUD_ENC_CH_MONO        =   1,
    AUD_ENC_CH_STEREO,
    AUD_ENC_CH_SURROUND_2CH,
    AUD_ENC_CH_SURROUND,
    AUD_ENC_CH_3_0,
    AUD_ENC_CH_4_0,
    AUD_ENC_CH_5_0,
    AUD_ENC_CH_5_1,
    AUD_ENC_CH_7_1
} AUD_ENC_CH_NUM_T;

/* Sample frequency. */
typedef enum
{
    AUD_ENC_SAMPLE_RATE_8K   =   1,
    AUD_ENC_SAMPLE_RATE_11K,
    AUD_ENC_SAMPLE_RATE_11_025K,
    AUD_ENC_SAMPLE_RATE_12K,
    AUD_ENC_SAMPLE_RATE_16K,
    AUD_ENC_SAMPLE_RATE_22K,
    AUD_ENC_SAMPLE_RATE_22_050K,
    AUD_ENC_SAMPLE_RATE_24K,
    
    AUD_ENC_SAMPLE_RATE_32K,
    AUD_ENC_SAMPLE_RATE_44K,
    AUD_ENC_SAMPLE_RATE_44_100K,
    AUD_ENC_SAMPLE_RATE_48K,
    AUD_ENC_SAMPLE_RATE_96K,
    AUD_ENC_SAMPLE_RATE_192K
} AUD_ENC_SAMPLE_RATE_T;

/* Sample bits. */
typedef enum
{
    AUD_ENC_PCM_BIT_DEPTH_8   =   1,
    AUD_ENC_PCM_BIT_DEPTH_16,
    AUD_ENC_PCM_BIT_DEPTH_18,
    AUD_ENC_PCM_BIT_DEPTH_20,
    AUD_ENC_PCM_BIT_DEPTH_24
} AUD_ENC_PCM_BIT_DEPTH_T;


typedef enum
{
    AUD_ENC_PCM_TYPE_NORMAL        =   1,
    AUD_ENC_PCM_TYPE_MS_ADPCM,
    AUD_ENC_PCM_TYPE_IMA_ADPCM
} AUD_ENC_PCM_TYPE_T;


typedef enum
{
    AUD_ENC_FMT_TYPE_UNKNOWN = 0,       /* Unknown Audio Codec Type*/
    AUD_ENC_FMT_TYPE_MPEG,              /* MPEG1 or MPEG2 audio, layer I or layer II*/
    AUD_ENC_FMT_TYPE_MP3,               /* MPEG1 or MPEG2 audio, layer III*/
    AUD_ENC_FMT_TYPE_AAC,               /* AAC, or HE-AAC*/
    AUD_ENC_FMT_TYPE_DD,                /* Dolby Digital, or Dolby Digital Plus*/
    AUD_ENC_FMT_TYPE_TRUEHD,            /* Dolby TrueHD*/
    AUD_ENC_FMT_TYPE_PCM,               /* PCM*/
    AUD_ENC_FMT_TYPE_DTS,               /* DTS*/
    AUD_ENC_FMT_TYPE_DTS_HD_HR,         /* DTS-HD High Resolution*/
    AUD_ENC_FMT_TYPE_DTS_HD_MA,         /* DTS-HD Master Audio*/
    AUD_ENC_FMT_TYPE_WMA,               /* WMA standard; not WMA Pro nor WMA Lossless*/
    AUD_ENC_FMT_TYPE_COOK,              /* Cook*/
    AUD_ENC_FMT_TYPE_VORBIS,            /* Vorbis*/
    AUD_ENC_FMT_TYPE_FLAC,              /* Free Lossless Audio Codec*/
    AUD_ENC_FMT_TYPE_MONKEY             /* Monkey's Audio*/
} AUD_ENC_FMT_TYPE_T;

/*-----------------------------------------------------------------------------
                    structures
-----------------------------------------------------------------------------*/
typedef struct
{
    AUD_ENC_PCM_TYPE_T       e_pcm_type;           /* PCM type*/
    AUD_ENC_CH_NUM_T         e_num_nh;             /* Channels*/
    AUD_ENC_SAMPLE_RATE_T    e_sample_rate;        /* Sampling Frequence*/
    UINT16                   u2_block_align;       /* BLock Alignment*/
    AUD_ENC_PCM_BIT_DEPTH_T  e_bits_per_sample;    /* Bits per Sample*/
    BOOL                     b_big_endian;         /* Big Endian PCM*/
} AUD_ENC_PCM_INFO_T;

typedef struct
{
    /*TBD*/
    AUD_ENC_CH_NUM_T           e_num_nh;             /* Channels*/
    AUD_ENC_PCM_BIT_DEPTH_T    e_sample_bits;
    AUD_ENC_SAMPLE_RATE_T      e_samle_freq;
} AUD_ENC_MP3_INFO_T;


/* for AUD_ENC_SET_TYPE_SETTINGS */
typedef struct _AUD_ENC_SETTINGS_T
{
    AUD_SRC_TYPE_T              e_src_type; /* Source Type */
    AUD_ENC_POS_MODE_T          e_enc_pos; /* Encode position */
    AUD_ENC_FMT_TYPE_T          e_enc_fmt;  /* Encode Format Type */
    /* Encoding Setting according to e_enc_fmt */
    union
    {
        AUD_ENC_PCM_INFO_T  t_pcm_info;         /* PCM codec info*/
        AUD_ENC_MP3_INFO_T  t_mp3_info;         /* Mp3 codec info*/
    }u_fmt;
} AUD_ENC_SETTINGS_T;

#endif /* _X_AUD_ENC_H_ */


