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

#ifndef BAGT_MWIF2

#ifndef DRV_FEEDER_IF_H
#define DRV_FEEDER_IF_H

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#if !defined(CC_UBOOT) && defined(__KERNEL__)
#include <linux/mt53xx_cb.h>
#endif
#ifdef TIME_MEASUREMENT
#include "x_time_msrt.h"
#include "u_time_msrt_name.h"
#endif

//#include "x_mm_common.h"
//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------
//#define CC_FLASH_TEST

//#ifdef FEEDER_HEADER_CACHE
#define FEEDER_CACHED_MAX_READ_SIZE (256 * 1024)
#define FEEDER_END_OF_FILE 0xffffffff
//#endif

//#define CC_53XX_SWDMX_V2   // joshua 20110401 tmp solution

#ifdef CC_53XX_SWDMX_V2
#define FEEDER_MULTI_INSTANCE
#ifdef CC_SKYPE_FINE_INSTANCE
#define FEEDER_FIVE_INSTANCE
#endif
#endif

#define MEDIA_SEEK_BGN             ((UINT8) 1)
#define MEDIA_SEEK_CUR             ((UINT8) 2)
#define MEDIA_SEEK_END             ((UINT8) 3)

#ifdef TIME_MEASUREMENT
typedef enum
{
	FEEDER_PRINT_TMS_UNKNOWN = 0,
	FEEDER_PRINT_TMS_PRINT,
	FEEDER_PRINT_TMS_NOT_PRINT
}FeederPrintTMS;
#endif

/// Feeder Request data type,For dlna time-based seek
#define DLNA_TIME_SEEK_INVALID_TIME 0xFFFFFFFFFFFFFFFF
typedef enum
{
	FEEDER_OFFSET_SEEK = 0,
	FEEDER_TIME_SEEK
}FeederRequestType;
/// Data query mode
typedef enum
{
    FEEDER_AUDIO_SOURCE = 0,    ///< Audio data
    FEEDER_VIDEO_SOURCE,        ///< Video data
    FEEDER_PROGRAM_SOURCE,      ///< Mpeg data
    FEEDER_PARSING_SOURCE,      ///< Hdr data
    FEEDER_PARSING_AUDIO,
 #ifdef FEEDER_FIVE_INSTANCE
    FEEDER_PROGRAM_SOURCE_2,      // 2nd video instance
    FEEDER_PARSING_SOURCE_2,    
    FEEDER_AUDIO_SOURCE_2,    // 2nd audio instance
    FEEDER_PARSING_AUDIO_2,    
    FEEDER_AUDIO_SOURCE_3,    // 3nd audio instance
    FEEDER_PARSING_AUDIO_3,
 #endif    
#ifdef CC_FBM_FOUR_INST_SUPPORT
    FEEDER_PARSING_SOURCE_3,
    FEEDER_SOURCE_3,
    FEEDER_PARSING_SOURCE_4,
    FEEDER_SOURCE_4,
#endif
    FEEDER_SOURCE_TOTAL,         ///< Total support data type
    FEEDER_SOURCE_INVALID
} FeederSourceType;

/// Data query mode
typedef enum
{
    FEEDER_PUSH_MODE = 0,       ///< Feeder push data to Application/Receiver
    FEEDER_PULL_MODE            ///< Application/Receiver send the data request
} FeederQueryMode;

typedef enum
{
    FEEDER_TRANSMIT_OK = 0,         ///< Data transmit success
    FEEDER_TRANSMIT_FAIL,           ///< Data transmit fail
    FEEDER_TRANSMIT_BUFFER_FULL,    ///< Data transmit success & buffer full
    FEEDER_TRANSMIT_BUFFER_EMPTY    ///< Data transmit success & buffer empty
} FeederTransmitCondition;

typedef enum
{    
    FEEDER_NORMAL_MM = 0,           
    FEEDER_VIDEO_CHUNK,    
    FEEDER_AUDIO_CHUNK
} FeederMMRequestMode;

typedef enum
{
    FEEDER_SWDMX_FILESIZE,
    FEEDER_SWDMX_UNDERFLOW_CB,
    FEEDER_SWDMX_READY_CB,
    FEEDER_SWDMX_SRC_TYPE,
    FEEDER_SWDMX_THRESHOLD,
    FEEDER_SWDMX_INSTANCE,
    FEEDER_SWDMX_EOS,
    FEEDER_SWDMX_STATUS_CB,
    FEEDER_MM_USE,
    FEEDER_SWDMX_CACHESIZE    
} FeederSetInfoType;

typedef enum
{
    FEEDER_SRC_TYPE_UNKNOWN = 0,
    FEEDER_SRC_TYPE_HIGH_SPEED_STORAGE,
    FEEDER_SRC_TYPE_NETWORK_DLNA,
    FEEDER_SRC_TYPE_NETWORK_NETFLIX,
    FEEDER_SRC_TYPE_MAX
}FeederSourceType_T;

/// Feeder data transmit condition
typedef struct _FEEDER_TRANSMIT_COND_T
{
    INT32  i4TransmitResult;    ///< Transmit result
    UINT32 u4TransmitSize;      ///< Size of transmitted data 
    UINT32 u4BufAddr;           ///< The start address of the transmitted data
#ifdef CC_FLASH_TEST
    FeederMMRequestMode eMode;
    BOOL fgFirst;
#endif
} FEEDER_TRANSMIT_COND;

/// Feeder buffer information
typedef struct _FEEDER_BUF_INFO_T
{
    UINT32  u4StartAddr;
    UINT32  u4EndAddr;
    UINT32  u4ReadAddr;
    UINT32  u4WriteAddr;
    UINT32  u4LastReadAddr;
    UINT32  u4LastReadSize;
    UINT32  u4FreeSize;
    UINT32  u4BytesInBuf;
    UINT32  u4Size;
	UINT32  u4UnAckNum;
	UINT32  FeederDeviceId;
	UINT32  u4PhysicalAddr;		
	UINT32  u4PhysicalAddrOffset;	
    BOOL    fgRingBuf;
} FEEDER_BUF_INFO_T;

typedef struct
{
  UINT32 u4Counter;
  UINT32 u4FeederVirAddr;
  UINT32 u4FeederBufSz;
  UINT32 u4FeederMMpVirAddr;
  UINT32 u4FeederMMpBufSz;
#if defined (FEEDER_MULTI_INSTANCE) || defined(CC_FBM_TWO_FBP) || defined(CC_FBM_FOUR_INST_SUPPORT)
  UINT32 u4Feeder2VirAddr;
  UINT32 u4Feeder2BufSz;
#endif
#ifdef CC_FBM_FOUR_INST_SUPPORT
  UINT32 u4Feeder3VirAddr;
  UINT32 u4Feeder3BufSz;
  UINT32 u4Feeder4VirAddr;
  UINT32 u4Feeder4BufSz;
#endif

}FeederMemInfo;


typedef enum
{
  FEEDER_FBM_FEEDER = 0,
  FEEDER_FBM_MMP,
  FEEDER_FBM_FEEDER2,
  FEEDER_FBM_FEEDER3,
  FEEDER_FBM_FEEDER4,
  FEEDER_FBM_MAX
}FeederFbmType;


/// Feeder file information
typedef struct _FEEDER_CACHE_INFO_T
{
    UINT32  i8CurrentPhyAddr;
    UINT32  i8CachedStartPhyAddr;
    UINT32  i8CachedEndPhyAddr;
} FEEDER_CACHE_INFO_T;


// IBC information
typedef enum
{
    FEEDER_IBC_NONE = 0,
    FEEDER_IBC_DMX_VUDU_KEY,
    FEEDER_IBC_CMPB_MTK,
//#ifndef _X_BUF_AGT_H_
    FEEDER_IBC_SEQHEADER,
    FEEDER_IBC_PDCF,
    FEEDER_IBC_CENC,
    FEEDER_IBC_PARAM_STRM_INFO,
//#endif
    FEEDER_IBC_DASH_SET_INFO,
    FEEDER_IBC_HDD_CONTENT_KEY,
    FEEDER_IBC_DTCP_IP_KEY,
    FEEDER_IBC_PUSH_CHANGE_AUDIO,
    FEEDER_IBC_MAX
} FeederIBCType;

#define FEEDER_CTRL_MTK_ENCRYPT_SEGMENT_MAX_NUM   (64)
typedef struct
{
    UINT32      u4Size;     ///< Segment Size
    BOOL        fgEnc;      ///< Segment encrypted or not
} FEEDER_CTRL_MTK_ENCRYPT_SEGMENT_T;

typedef struct 
{
    UINT8                                   u1KeyIdx;   ///< MTK key slot index
    UINT16                                  u2SegNum;   ///< How many segments of atSegInfo
    FEEDER_CTRL_MTK_ENCRYPT_SEGMENT_T        atSegInfo[FEEDER_CTRL_MTK_ENCRYPT_SEGMENT_MAX_NUM]; ///< array of segments
 	UINT8                                    au1IV[16];
}FEEDER_CTRL_MTK_ENCRYPT_INFO_T;

//#ifndef _X_BUF_AGT_H_
typedef enum
{
    FEEDER_CTRL_STREAM_TYPE_AUDIO,
    FEEDER_CTRL_STREAM_TYPE_VIDEO
} FEEDER_CTRL_STREAM_TYPE_T;

typedef enum
{
    FEEDER_CTRL_VSH_SRC_FRAME_RATE_UNKNOWN = 0,
    FEEDER_CTRL_VSH_SRC_FRAME_RATE_23_976, /* 24000/1001 (23.976...) */
    FEEDER_CTRL_VSH_SRC_FRAME_RATE_24,
    FEEDER_CTRL_VSH_SRC_FRAME_RATE_25,
    FEEDER_CTRL_VSH_SRC_FRAME_RATE_29_97, /* 30000/1001 (29.97...) */
    FEEDER_CTRL_VSH_SRC_FRAME_RATE_30,
    FEEDER_CTRL_VSH_SRC_FRAME_RATE_50,
    FEEDER_CTRL_VSH_SRC_FRAME_RATE_59_94, /* 60000/1001 (59.94...) */
    FEEDER_CTRL_VSH_SRC_FRAME_RATE_60,
    FEEDER_CTRL_VSH_SRC_FRAME_RATE_120,
    FEEDER_CTRL_VSH_SRC_FRAME_RATE_1,
    FEEDER_CTRL_VSH_SRC_FRAME_RATE_5,
    FEEDER_CTRL_VSH_SRC_FRAME_RATE_8,
    FEEDER_CTRL_VSH_SRC_FRAME_RATE_10,
    FEEDER_CTRL_VSH_SRC_FRAME_RATE_12,
    FEEDER_CTRL_VSH_SRC_FRAME_RATE_15,
    FEEDER_CTRL_VSH_SRC_FRAME_RATE_2,
    FEEDER_CTRL_VSH_SRC_FRAME_RATE_6,
    FEEDER_CTRL_VSH_SRC_FRAME_RATE_48,
    FEEDER_CTRL_VSH_SRC_FRAME_RATE_70,
    FEEDER_CTRL_VSH_SRC_FRAME_RATE_20,
    FEEDER_CTRL_VSH_SRC_FRAME_RATE_VARIABLE
} FEEDER_CTRL_VSH_SRC_FRAME_RATE_T;

typedef enum
{    
    FEEDER_CTRL_VSH_ASP_USR_TYPE_SRC = 1,   
    FEEDER_CTRL_VSH_ASP_USR_TYPE_DISP    
} FEEDER_CTRL_VSH_ASP_USR_TYPE_T;

typedef enum 
{
    FEEDER_CTRL_VSH_SRC_ASPECT_RATIO_UNKNOWN = 0,
    FEEDER_CTRL_VSH_SRC_ASPECT_RATIO_4_3,         /* DAR 4:3 */
    FEEDER_CTRL_VSH_SRC_ASPECT_RATIO_16_9,        /* DAR 16:9 */
    FEEDER_CTRL_VSH_SRC_ASPECT_RATIO_2_21_1,      /* DAR 2.21:1 */
    FEEDER_CTRL_VSH_SRC_ASPECT_RATIO_PXL_10_11,   /* PAR 10:11 */
    FEEDER_CTRL_VSH_SRC_ASPECT_RATIO_PXL_40_33,   /* PAR 40:33 */
    FEEDER_CTRL_VSH_SRC_ASPECT_RATIO_PXL_16_11,   /* PAR 16:11 */
    FEEDER_CTRL_VSH_SRC_ASPECT_RATIO_PXL_12_11,   /* PAR 12:11 */
    FEEDER_CTRL_VSH_SRC_ASPECT_RATIO_PXL_3_2,     /* PAR 3:2 */
    FEEDER_CTRL_VSH_SRC_ASPECT_RATIO_PXL_1_1,     /* PAR 1:1 */
    FEEDER_CTRL_VSH_SRC_ASPECT_RATIO_USR          /* user */
} FEEDER_CTRL_VSH_SRC_ASPECT_RATIO_T;

typedef struct 
{
    UINT16                     ui2_width;      /* horizontal_size_value */
    UINT16                     ui2_height;     /* vertical_size_value */
    FEEDER_CTRL_VSH_SRC_FRAME_RATE_T       e_frame_rate;   /* Frame Rate */
    FEEDER_CTRL_VSH_SRC_ASPECT_RATIO_T     e_src_asp;      /* Display Aspect Ratio (DAR) */
    FEEDER_CTRL_VSH_ASP_USR_TYPE_T         e_asp_usr_type; /* source or display asp */
    UINT16                     ui2_asp_usr_h;
    UINT16                     ui2_asp_usr_w;      
    UINT32                     ui4_seq_hdr;
    UINT16                     ui2_seq_hdr_sz;        
    UINT8                      ui1_sample_id;
    BOOL                       b_chk_framerate;
}   FEEDER_CTRL_VSH_SEQUENCE_DATA_T;


typedef struct 
{
    UINT8                      *pc_buff;
    SIZE_T                     z_buff_size;
    FEEDER_CTRL_VSH_SRC_FRAME_RATE_T       e_frame_rate;  
    UINT8                      ui1_sample_id;
}   FEEDER_CTRL_VSH_SEQUENCE_RAW_T;


typedef struct _FEEDER_CTRL_IBC_VIDEO_SEQHEADER_INFO_T
{
    BOOL                       b_is_raw;
    BOOL                       b_is_second_raw;

    union
    {
        FEEDER_CTRL_VSH_SEQUENCE_RAW_T         t_seq_raw;
        FEEDER_CTRL_VSH_SEQUENCE_DATA_T        t_seq_data;
    } u;
#if SUPPORT_MULTI_INFO
    struct _FEEDER_CTRL_IBC_VIDEO_SEQHEADER_INFO_T*  pt_next_scale;
#endif
}  FEEDER_CTRL_IBC_VIDEO_SEQHEADER_INFO;


/*
typedef struct
{
    UINT32    ui4_width;
    UINT32    ui4_height;
    UINT32    ui4_frame_rate;
    UINT8     pui1_seq_buf[100];
    UINT32    ui4_buffer_size;       
    UINT32    ui4_time_scale;
}PLAYBACK_CTRL_IBC_VIDEO_SEQHEADER_INFO;
*/

typedef struct
{
    INT16     i2_channle;
    UINT32    ui4_sample_rate;
    UINT32    ui4_profile;
    BOOL      b_frame_length;
    BOOL      b_is_audio_change;
}FEEDER_CTRL_IBC_AUDIO_SEQHEADER_INFO;
//#endif
/*DASH  ENCRYPT_INFO*/
typedef struct _FEEDER_SUB_SAMPLE_INFO_T
{
    UINT16   ui2_size_clear_data;
    UINT32   ui4_size_encrypted_data;
}FEEDER_SUB_SAMPLE_INFO_T;

typedef struct _FEEDER_SENC_T
{
    UINT32     ui4_sub_sample_cnt;
    FEEDER_SUB_SAMPLE_INFO_T   a_sub_sample[32];
}FEEDER_SENC_T;

typedef struct _FEEDER_CTR_ENCRYPT
{  
    UINT8      ui1_iv[16];
    UINT32     ui4_ivlength;
    FEEDER_SENC_T   t_dash_senc;
}FEEDER_AES_CTR_ENCRYPT;

#define MAX_MARLIN_TRACK_NUM        17 ///<  marlin max number of track : 1(video) + 2(audio) + 4(subtitle)

typedef enum
{
    FEEDER_CRYPT_MODE_INVALID = 0,
    FEEDER_CRYPT_MODE_DES_EBC,
    FEEDER_CRYPT_MODE_DES_CBC,
    FEEDER_CRYPT_MODE_3_DES_EBC,
    FEEDER_CRYPT_MODE_3_DES_CBC,
    FEEDER_CRYPT_MODE_DVB,
    FEEDER_CRYPT_MODE_DVB_CONF,
    FEEDER_CRYPT_MODE_MULTI2_BIG,
    FEEDER_CRYPT_MODE_MULTI2_LITTLE,
    FEEDER_CRYPT_MODE_AES_ECB,
    FEEDER_CRYPT_MODE_AES_CBC,
    FEEDER_CRYPT_MODE_AES_CTR
}   FEEDER_CRYPT_MODE_T;

typedef enum
{
    FEEDER_ST_UNKNOWN = -1,  /* Must be set to '-1' else I loose an entry in the bit mask. */
    FEEDER_ST_AUDIO,
    FEEDER_ST_VIDEO,
    FEEDER_ST_CLOSED_CAPTION,
    FEEDER_ST_ISDB_CAPTION,
    FEEDER_ST_ISDB_TEXT,
    FEEDER_ST_TELETEXT,
    FEEDER_ST_SUBTITLE,
    FEEDER_ST_DATA,
    FEEDER_ST_BYPASS,
    FEEDER_ST_MM_SUBTITLE,
    FEEDER_ST_RCRD_STRM,
    FEEDER_ST_GINGA_STOP_AUDIO,
    FEEDER_ST_GINGA_STOP_VIDEO
}   FEEDER_STREAM_TYPE_T;


typedef struct 
{
    FEEDER_CRYPT_MODE_T     e_mode;
    FEEDER_STREAM_TYPE_T       e_strm_type;
    UINT32              ui4_content_id;
    UINT16              ui2_key_idx;
    UINT8               aui1_key[16];
}FEEDER_TRACK_MARLIN_CRYPT_INFO_T;

typedef struct 
{
    UINT8                           ui1_valid_cnt;
    FEEDER_TRACK_MARLIN_CRYPT_INFO_T    t_track_drm[MAX_MARLIN_TRACK_NUM];
}FEEDER_PDCF_CRYPT_INFO_T;

typedef struct
{
    UINT64    ui8_bandwidth;
    BOOL      b_bandwidth_chg;
}FEEDER_CTRL_IBC_DASH_SET_INFO_T ;

typedef struct
{
    BOOL    b_is_encrypt;
    UINT16  ui2_key_len;
    UINT8   aui1_key_info[256];
    UINT16  ui2_ext_key_len;
    UINT8   aui1_ext_key_info[256];
}FEEDER_CTRL_HDD_DTCP_IP_INFO_T;

typedef struct
{    
    FeederSourceType eDataType;
    UINT32 u4Id;
    UINT32 u4ReadSize;
    UINT32 u4WriteAddr;
    UINT32 u4AlignOffset;
    UINT64 u8FilePos;
    BOOL   fgPartial;
#ifdef CC_FLASH_TEST
    FeederMMRequestMode eMode;    
    UINT64 u8Pts;
    BOOL fgFirst;
#endif     
    BOOL fgEof;    
    FeederIBCType eFeederIBC;
    FEEDER_CTRL_MTK_ENCRYPT_INFO_T *prMtkEncryptInfo;
	FEEDER_CTRL_STREAM_TYPE_T eFeederStrmType;
    
    union
    {
        FEEDER_CTRL_IBC_VIDEO_SEQHEADER_INFO t_vid_seqheader_info;
        FEEDER_CTRL_IBC_AUDIO_SEQHEADER_INFO t_aud_seqheader_info;
    }u;

    FEEDER_PDCF_CRYPT_INFO_T *pt_pdcf_crypt;
    FEEDER_AES_CTR_ENCRYPT *pt_aes_crt_crypt;
    FEEDER_CTRL_IBC_DASH_SET_INFO_T *pt_dash_set_info;
    FEEDER_CTRL_HDD_DTCP_IP_INFO_T  *pt_hdd_dtcp_ip_info;
#ifdef TIME_MEASUREMENT
    FeederPrintTMS eFeederPrintTMS;
#endif
    FeederRequestType eFeederReqType;
    UINT64         ui8_vid_pts;//for wfd debug
    UINT64         ui8_push_time;//ms,for wfd debug
}FEEDER_REQ_DATA_T;

/// Callbalck function if Feeder retrieves the data
#ifdef FEEDER_MULTI_INSTANCE
typedef UINT32 (*FEEDER_QUERY_CALLBACK)(UINT8 u1AttachedSrcId,
                                        FeederSourceType eSrcType,
                                        FEEDER_BUF_INFO_T *pQryBufInfo,
                                        FEEDER_REQ_DATA_T *pReqData);
#else
typedef UINT32 (*FEEDER_QUERY_CALLBACK)(FeederSourceType eSrcType,
                                             FEEDER_BUF_INFO_T *pQryBufInfo,
                                             FEEDER_REQ_DATA_T *pReqData);
#endif

/// Callbalck function if Feeder consumes the data
typedef VOID (*FEEDER_CONSUME_CALLBACK)(FeederSourceType eSrcType, 
                                             FEEDER_BUF_INFO_T *pQryBufInfo,
                                             UINT32 u4NewRPtr);

typedef UINT32 (*FEEDER_FLUSH_CALLBACK)(FeederSourceType eSrcType);
/// Callbalck function if Feeder retrieves the data
typedef INT32 (*FEEDER_TRANMIT_HANDLER)(FeederSourceType eSrcType,
                                        UINT32 u4QrySize,
                                         UINT64 u8Offset,
                                        FEEDER_TRANSMIT_COND *pResult,  UINT32 u4ReqId);

/// Callbalck function if Feeder buffer is overflow
typedef VOID (*FEEDER_OVERFLOW_CALLBACK)(FeederSourceType eSrcType);

/// Callbalck function if Feeder buffer is underflow
typedef VOID (*FEEDER_UNDERFLOW_CALLBACK)(FeederSourceType eSrcType);


typedef enum
{
    FEEDER_NOTIFY_NEEDDATA,   // notification of copy buffer
    FEEDER_NOTIFY_ERROR,      // notification of play error
    FEEDER_NOTIFY_FILEEND,     // notification of file end
    FEEDER_NOTIFY_CAPTION,    // notification of caption
    FEEDER_NOTIFY_FILEBEGIN,                ///< notification of fileplay begin
    FEEDER_NOTIFY_DATA_REQ_DONE,            ///for async reading
    FEEDER_NOTIFY_BUFFULL,
    FEEDER_NOTIFY_ELAPSEDTIME, // notification of timer
    FEEDER_NOTIFY_TOTALTIME,   // notification of updated total time
    FEEDER_NOTIFY_PROFILE_CHANGE,
    FEEDER_NOTIFY_SEEK_END,     // notification of seek end
    FEEDER_NOTIFY_ABORT_DONE,     // notification of abort end
    FEEDER_NOTIFY_TIME_OUT,     // notification of time out
    FEEDER_NOTIFY_ELAPSEDBYTE,     // notification of comsumed byte
    FEEDER_NOTIFY_DRAWSUBTITLE,
    FEEDER_NOTIFY_DRAW_EXT_SUBTITLE,
    FEEDER_NOTIFY_PACKET_SIZE,    //notification of netflix packet size
    FEEDER_NOTIFY_DATA_EXHAUSTED, //notification fo netflix data exhausted info
    FEEDER_NOTIFY_PVR_RANGE_NOTIFY,  // only correspond to user space
    FEEDER_NOTIFY_ACCESS_DIVX_DRM,
    FEEDER_NOTIFY_MAX
} FEEDER_NOTIFY_COND;

/// Set the callback function of the Feeder module
typedef BOOL (*FEEDER_NOTIFY)(FeederSourceType eSrcType,
                              FEEDER_NOTIFY_COND eCondition,
                              UINT32 u4Param);

/// Feeder data transmit condition
typedef struct _FEEDER_STATUS_T
{
    UINT32 u4TimePosition ;                 ///< Current time position
	UINT32 u4speed;                         ///< Play speed
	BOOL   bReversePlay;                    ///< Play direction
} FEEDER_DECODE_STATUS;

/// Set the callback function of the Feeder module
typedef INT32 (*FEEDER_GET_STATUS)(FEEDER_DECODE_STATUS *prStatus);

/// Feeder data qeury setting
typedef struct _FEEDER_TRANSMIT_T
{
    FeederQueryMode         eTargetMode;    ///< Tranmit mode of the receiver
    UINT32                  u4BufferSize;   ///< Feeder buffer size
    UINT32                  u4Tag;          ///< Query data type
    FEEDER_TRANMIT_HANDLER  pfnTransHandle; ///< Callback to transmit data
    FEEDER_NOTIFY           pfnNotify;      ///< Notify function pointer
    FEEDER_GET_STATUS       pfnGetStatus;   ///< Get the status of decode target
} FEEDER_TRANSMIT;

/// Set the data transmit parameters of the receiver
typedef BOOL (*FEEDER_SET_TRANSMIT)(FEEDER_TRANSMIT *prTransmit);

#if !defined(__KERNEL__) || defined(CC_UBOOT)
/// Information of a callback function
typedef struct
{
    UINT8 u1SwdmxId;
    UINT8 u1PlaymgrId;
    UINT32 u4SrcType;                       ///< Callback source
    UINT32 u4Condition;                     ///< Callback condition
    UINT32 u4Param;                         ///< Callback paramter
} MM_NOTIFY_INFO_T;

typedef struct _MM_FILE_OPS_T
{
    UINT64  u8Offset;
    UINT32  u4QueryID;
    UINT8   u1Whence;
} MM_FILE_OPS_T;

typedef struct
{
    MM_NOTIFY_INFO_T rMMNotify;
    MM_FILE_OPS_T rFileInfo;
    UINT32 u4QuerySize; 
    UINT32 u4WritePtr;
} MM_CALLBACK_INFO_T;

#endif

typedef void (*FEEDER_CALBACK)(MM_NOTIFY_INFO_T *prNfyInfo, UINT32 u4Param);
#ifdef FEEDER_MULTI_INSTANCE
typedef INT64 (*FEEDER_SEEKFILEPOS_HANDLER)(UINT8 u1AttachedSrcId, FeederSourceType eSrcType, MM_FILE_OPS_T *prFileInfo);
#else
typedef INT64 (*FEEDER_SEEKFILEPOS_HANDLER)(FeederSourceType eSrcType, MM_FILE_OPS_T *prFileInfo);
#endif

/// Application register its data transmit callback to the feeder
typedef struct _FEEDER_QUERY_DATA_T
{
    UINT8 u1PlaymgrAttachedId;
    UINT8 u1SwdmxAttachedId;
    UINT32                  u4QuerySize;    ///< Query data size
    FeederSourceType        eDataType;      ///< Query data type
    FEEDER_QUERY_CALLBACK   pfnSource;      ///< Callback function
    FEEDER_SEEKFILEPOS_HANDLER pfnSeekPos;    ///< Callback function
    FEEDER_CONSUME_CALLBACK pfnConsume;     ///< Callback function
    FEEDER_OVERFLOW_CALLBACK pfnOverflow;   ///< Callback function, for URI mode
    FEEDER_UNDERFLOW_CALLBACK pfnUnderflow; ///< Callback function, for URI mode
    FEEDER_FLUSH_CALLBACK pfFlush;
} FEEDER_QUERY_DATA;




//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------

#define FEEDER_E_OK             (INT32)(0)          ///< Success
#define FEEDER_E_FAIL           (INT32)(-1)         ///< IO error
#define FEEDER_E_INVALID_PARAM  (INT32)(-2)         ///< Get the invalid param
#define FEEDER_E_MEMORY_ALLOC   (INT32)(-3)         ///< Memory allocation fail
#define FEEDER_E_BUFFER_FULL    (INT32)(-4)         ///< Memory buffer is full
#define FEEDER_E_STOP           (INT32)(-5)         ///< Feeder stops

#define FEEDER_NFY_STATUS_READY                 0x00000000
#define FEEDER_NFY_STATUS_IGNORE_READY   0x00000001

#ifdef CC_ONLY_FOR_SH
#define FEEDER_BUF_SIZE_EX  2
#else
#define FEEDER_BUF_SIZE_EX  1
#endif

#define FEEDER_MIN_BUF_SIZE             (500*1024)

#define FEEDER_RESERVED_SIZE            (64 * 1024)

#define FEEDER_PROGRAM_BUF_START        FEEDER_RESERVED_SIZE

#if defined(FEEDER_FIVE_INSTANCE)
#if (defined(CC_SUPPORT_4K2K)|| defined(CC_FBM_SUPPORT_AMAZON_APP))  
#define FEEDER_MAX_DEFAULT_BUF_SIZE  	(12288*1024*FEEDER_BUF_SIZE_EX) 
#else
#define FEEDER_MAX_DEFAULT_BUF_SIZE  	(2256*1024*FEEDER_BUF_SIZE_EX) 
#endif
#define FEEDER_DEFAULT_BUF_SIZE         (1128*1024) // 192, 188 alignment
#define FEEDER_PARSING_BUF_START        (FEEDER_PROGRAM_BUF_START + FEEDER_DEFAULT_BUF_SIZE)
#define FEEDER_PARSING_BUF_SIZE         (344 * 1024)

#define FEEDER_PROGRAM_SOURCE_2_BUF_START   (FEEDER_PARSING_BUF_START+FEEDER_PARSING_BUF_SIZE+FEEDER_RESERVED_SIZE)
#define FEEDER_PROGRAM_SOURCE_2_BUF_SIZE    (1128*1024)
#define FEEDER_PARSING_SOURCE_2_START      (FEEDER_PROGRAM_SOURCE_2_BUF_START+FEEDER_PROGRAM_SOURCE_2_BUF_SIZE)
#define FEEDER_PARSING_SOURCE_2_SIZE       (344 * 1024)

#define FEEDER_AUDIO_BUF_START          (FEEDER_PARSING_SOURCE_2_START+FEEDER_PARSING_SOURCE_2_SIZE+FEEDER_RESERVED_SIZE)
#define FEEDER_AUDIO_BUF_SIZE           (512*1024)
#define FEEDER_PARSING_AUD_BUF_START    (FEEDER_AUDIO_BUF_START + FEEDER_AUDIO_BUF_SIZE)
#define FEEDER_PARSING_AUD_BUF_SIZE     (344 * 1024)

/*
#define FEEDER_AUDIO_BUF_2_START          (FEEDER_PARSING_AUD_BUF_START+FEEDER_PARSING_AUD_BUF_SIZE+FEEDER_RESERVED_SIZE)
#define FEEDER_AUDIO_BUF_2_SIZE           (512*1024)
#define FEEDER_PARSING_AUD_BUF_2_START    (FEEDER_AUDIO_BUF_2_START + FEEDER_AUDIO_BUF_2_SIZE)
#define FEEDER_PARSING_AUD_BUF_2_SIZE     (344 * 1024)

#define FEEDER_AUDIO_BUF_3_START          (FEEDER_PARSING_AUD_BUF_2_START+FEEDER_PARSING_AUD_BUF_2_SIZE+FEEDER_RESERVED_SIZE)
#define FEEDER_AUDIO_BUF_3_SIZE           (512*1024)
#define FEEDER_PARSING_AUD_BUF_3_START    (FEEDER_AUDIO_BUF_3_START + FEEDER_AUDIO_BUF_3_SIZE)
#define FEEDER_PARSING_AUD_BUF_3_SIZE     (344 * 1024)
*/

#define FEEDER_PROGRAM_BUF_START_SKYPE	FEEDER_RESERVED_SIZE
#define FEEDER_DEFAULT_BUF_SIZE_SKYPE         (1128*1024) // 192, 188 alignment
#define FEEDER_PROGRAM_SOURCE_2_BUF_START_SKYPE   (FEEDER_PROGRAM_BUF_START_SKYPE+FEEDER_DEFAULT_BUF_SIZE_SKYPE+FEEDER_RESERVED_SIZE)
#define FEEDER_PROGRAM_SOURCE_2_BUF_SIZE_SKYPE    (1128*1024)
#define FEEDER_AUDIO_BUF_START_SKYPE          (FEEDER_PROGRAM_SOURCE_2_BUF_START_SKYPE+FEEDER_PROGRAM_SOURCE_2_BUF_SIZE_SKYPE+FEEDER_RESERVED_SIZE)
#define FEEDER_AUDIO_BUF_SIZE_SKYPE           (512*1024)
#define FEEDER_AUDIO_BUF_2_START_SKYPE          (FEEDER_AUDIO_BUF_START_SKYPE+FEEDER_AUDIO_BUF_SIZE_SKYPE+FEEDER_RESERVED_SIZE)
#define FEEDER_AUDIO_BUF_2_SIZE_SKYPE           (512*1024)
#define FEEDER_AUDIO_BUF_3_START_SKYPE          (FEEDER_AUDIO_BUF_2_START_SKYPE+FEEDER_AUDIO_BUF_2_SIZE_SKYPE+FEEDER_RESERVED_SIZE)
#define FEEDER_AUDIO_BUF_3_SIZE_SKYPE           (512*1024)

#elif defined(FEEDER_THREE_INSTANCE)
#if ((defined(CC_SUPPORT_4K2K)) || defined(CC_FBM_SUPPORT_AMAZON_APP))
#define FEEDER_MAX_DEFAULT_BUF_SIZE  	(12288*1024*FEEDER_BUF_SIZE_EX) 
#else
#define FEEDER_MAX_DEFAULT_BUF_SIZE  	(2256*1024*FEEDER_BUF_SIZE_EX) 
#endif
#define FEEDER_DEFAULT_BUF_SIZE         (1128*1024) // 192, 188 alignment
#define FEEDER_PARSING_BUF_START        (FEEDER_PROGRAM_BUF_START + FEEDER_DEFAULT_BUF_SIZE)
#define FEEDER_PARSING_BUF_SIZE         (0 * 1024)
#define FEEDER_PARSING_AUD_BUF_START    (FEEDER_PARSING_BUF_START + FEEDER_PARSING_BUF_SIZE)
#define FEEDER_PARSING_AUD_BUF_SIZE     (0 * 1024)
#define FEEDER_THIRD_STREAM_BUF_START   (FEEDER_PARSING_AUD_BUF_START+FEEDER_PARSING_AUD_BUF_SIZE+FEEDER_RESERVED_SIZE)
#define FEEDER_THIRD_STREAM_BUF_SIZE    (1128*1024)
#define FEEDER_THIRD_PARSING_START      (FEEDER_THIRD_STREAM_BUF_START+FEEDER_THIRD_STREAM_BUF_SIZE)
#define FEEDER_THIRD_PARSING_SIZE       (0 * 1024)
#define FEEDER_AUDIO_BUF_START          (FEEDER_THIRD_PARSING_START+FEEDER_THIRD_PARSING_SIZE+FEEDER_RESERVED_SIZE)
#define FEEDER_AUDIO_BUF_SIZE           (512*1024)
#elif defined(CC_FBM_TWO_FBP)
#if ((defined(CC_SUPPORT_4K2K)) || defined(CC_FBM_SUPPORT_AMAZON_APP))
#define FEEDER_MAX_DEFAULT_BUF_SIZE  	(12288*1024*FEEDER_BUF_SIZE_EX) 
#else
#define FEEDER_MAX_DEFAULT_BUF_SIZE  	(2256*1024*FEEDER_BUF_SIZE_EX) 
#endif
#define FEEDER_DEFAULT_BUF_SIZE         (1128*1024) // 192, 188 alignment
#define FEEDER_PARSING_BUF_START        (FEEDER_PROGRAM_BUF_START + FEEDER_DEFAULT_BUF_SIZE)
#define FEEDER_PARSING_BUF_SIZE         (344 * 1024)
#define FEEDER_PARSING_AUD_BUF_START    (FEEDER_PARSING_BUF_START + FEEDER_PARSING_BUF_SIZE)
#define FEEDER_PARSING_AUD_BUF_SIZE     (344*1024)
#define FEEDER_AUDIO_BUF_START          (FEEDER_PARSING_AUD_BUF_START+FEEDER_PARSING_AUD_BUF_SIZE+FEEDER_RESERVED_SIZE)
#define FEEDER_AUDIO_BUF_SIZE           (1128*1024)
#else
#if ((defined(CC_SUPPORT_4K2K)) || defined(CC_FBM_SUPPORT_AMAZON_APP))
#define FEEDER_MAX_DEFAULT_BUF_SIZE  	(12288*1024*FEEDER_BUF_SIZE_EX) 
#else
#define FEEDER_MAX_DEFAULT_BUF_SIZE  	(2256*1024*FEEDER_BUF_SIZE_EX) 
#endif
#define FEEDER_DEFAULT_BUF_SIZE         (2256*1024)
#define FEEDER_PARSING_BUF_START        (FEEDER_PROGRAM_BUF_START + FEEDER_DEFAULT_BUF_SIZE)
#define FEEDER_PARSING_BUF_SIZE         (512 * 1024)
#define FEEDER_PARSING_AUD_BUF_START    (FEEDER_PARSING_BUF_START + FEEDER_PARSING_BUF_SIZE)
#define FEEDER_PARSING_AUD_BUF_SIZE     (0)
#define FEEDER_AUDIO_BUF_START          (FEEDER_PROGRAM_BUF_START)
#define FEEDER_AUDIO_BUF_SIZE           (FEEDER_DEFAULT_BUF_SIZE)
#endif

#if 0
#ifdef CC_FBM_TWO_FBP
#define FEEDER_DEFAULT_BUF_SIZE         (1128*1024) // 192, 188 alignment
#else
#define FEEDER_DEFAULT_BUF_SIZE         (2256*1024)
#endif
#define FEEDER_PARSING_BUF_START        (FEEDER_PROGRAM_BUF_START + FEEDER_DEFAULT_BUF_SIZE)
#ifdef CC_FBM_TWO_FBP
#define FEEDER_PARSING_BUF_SIZE         ( 344 * 1024)
#else
#define FEEDER_PARSING_BUF_SIZE         ( 512 * 1024)
#endif
#define FEEDER_PARSING_AUD_BUF_START    (FEEDER_PARSING_BUF_START + FEEDER_PARSING_BUF_SIZE)
#ifdef CC_FBM_TWO_FBP
#define FEEDER_PARSING_AUD_BUF_SIZE     (344*1024)
#else
#define FEEDER_PARSING_AUD_BUF_SIZE     (0)
#endif

#ifdef CC_FBM_TWO_FBP
#define FEEDER_AUDIO_BUF_START          (FEEDER_PARSING_AUD_BUF_START+FEEDER_PARSING_AUD_BUF_SIZE+FEEDER_RESERVED_SIZE)
#define FEEDER_AUDIO_BUF_SIZE           (1128*1024)
#else
#define FEEDER_AUDIO_BUF_START          (FEEDER_PROGRAM_BUF_START)
#define FEEDER_AUDIO_BUF_SIZE           (FEEDER_DEFAULT_BUF_SIZE)
#endif
#endif

#define FEEDER_DATA_Q_SIZE 128    // Feeder message Q size (maximum number of requests to hold)


//-----------------------------------------------------------------------------
// Prototype of inter-file functions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Prototype of public functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/** Init feeder param when system boot up.
 *
 *  @retval FEEDER_E_OK     Succeed
 *  @retval FEEDER_E_FAIL   Fail
 */
//-----------------------------------------------------------------------------
//extern INT32 FeederInit(void);

//-----------------------------------------------------------------------------
/** Allot the receiver-related hardware resources
 *
 *  @param  eDataType       Data type (audio, video..)
 *
 *  @retval FEEDER_E_OK     Succeed
 *  @retval FEEDER_E_FAIL   Fail
 */
//-----------------------------------------------------------------------------
extern INT32 FeederOpen(FeederSourceType eDataType);

//-----------------------------------------------------------------------------
/** Release the allotted resources
 *
 *  @param  eDataType       Data type (audio, video..)
 *
 *  @retval FEEDER_E_OK     Succeed
 *  @retval FEEDER_E_FAIL   Fail
 */
//-----------------------------------------------------------------------------
extern INT32 FeederClose(FeederSourceType eDataType);

//-----------------------------------------------------------------------------
/** Initialize the variables which are required to transmit the data to the
 *  local buffer of the receiver
 *
 *  @param  eDataType       Tramsmit data type (audio, video or program)
 *
 *  @retval FEEDER_E_OK     Succeed
 *  @retval FEEDER_E_FAIL   Fail
 */
//-----------------------------------------------------------------------------
extern INT32 FeederStart(FeederSourceType eDataType);

//-----------------------------------------------------------------------------
/** Initialize the local buffer by NULL
 *
 *  @param  eDataType       Tramsmit data type (audio, video or program)
 *
 *  @retval FEEDER_E_OK     Succeed
 *  @retval FEEDER_E_FAIL   Fail
 */
//-----------------------------------------------------------------------------
extern INT32 FeederStop(FeederSourceType eDataType);

//-----------------------------------------------------------------------------
/** Pause the data transmit to the audio decoder
 *
 *  @param  eDataType       Tramsmit data type (audio, video or program)
 *
 *  @retval FEEDER_E_OK     Succeed
 *  @retval FEEDER_E_FAIL   Fail
 */
//-----------------------------------------------------------------------------
extern INT32 FeederPause(FeederSourceType eDataType);

//-----------------------------------------------------------------------------
/** Resume the data transmit to the audio decoder
 *
 *  @param  eDataType       Tramsmit data type (audio, video or program)
 *
 *  @retval FEEDER_E_OK     Succeed
 *  @retval FEEDER_E_FAIL   Fail
 */
//-----------------------------------------------------------------------------
extern INT32 FeederResume(FeederSourceType eDataType);

//-----------------------------------------------------------------------------
/** Get the free size of feeder local buffer
 *
 *  @param  eDataType       Tramsmit data type (audio, video or program)
 *
 *  @retval the size of the feeder buffer
 */
//-----------------------------------------------------------------------------
//extern INT32 FeederGetBufferSize(FeederSourceType eDataType);

//-----------------------------------------------------------------------------
/** Get the information of feeder local buffer
 *
 *  @param  eDataType       Tramsmit data type (audio, video or program)
 *  @param  prBuffInfo      Pointers of the buffer
 *
 *  @retval FEEDER_E_OK     Succeed
 *  @retval FEEDER_E_FAIL   Fail
 */
//-----------------------------------------------------------------------------
extern INT32 FeederGetBufferInfo(FeederSourceType eDataType, 
                                 FEEDER_BUF_INFO_T *prBuffInfo);

extern INT32 FeederGetMemoryInfo(FeederMemInfo * pMemInfo);

//-----------------------------------------------------------------------------
/** Update the write pointer of the feeder local buffer
 *
 *  @param  eDataType       Tramsmit data type (audio, video or program)
 *  @param  u4WriteSize     Write data size
 *
 *  @retval FEEDER_E_OK     Succeed
 *  @retval FEEDER_E_FAIL   Fail
 */
//-----------------------------------------------------------------------------
extern INT32 FeederUpdateWritePointer(FeederSourceType eDataType,
                                      UINT32 u4WriteSize);
//-----------------------------------------------------------------------------
/** Reset the write pointer of the feeder local buffer
 *
 *  @param  eDataType       Tramsmit data type (audio, video or program)
 *  @param  u4WritePointer     WritePointer
 *
 *  @retval FEEDER_E_OK     Succeed
 *  @retval FEEDER_E_FAIL   Fail
 */
//-----------------------------------------------------------------------------
extern INT32 FeederSetWritePointer(FeederSourceType eDataType,UINT32 u4WritePointer);

//-----------------------------------------------------------------------------
/** Reallocate the feeder local buffer with user-specific size
 *
 *  @param  eDataType       Tramsmit data type (audio, video or program)
 *  @param  u4Size          Memory buffer size
 *
 *  @retval FEEDER_E_OK     Succeed
 *  @retval FEEDER_E_FAIL   Fail
 */
//-----------------------------------------------------------------------------
//extern INT32 FeederSetBufferSize(FeederSourceType eDataType, UINT32 u4Size);

//-----------------------------------------------------------------------------
/** Transmit the data inside buffer as getting the data of Application 
 *  through the pBuffer
 *
 *  @param  eDataType       Data type (audio, video..)
 *  @param  puiBuffer       Data buffer trasmitted by application
 *  @param  u4Size          Data size
 *
 *  @retval FEEDER_E_OK     Succeed
 *  @retval FEEDER_E_FAIL   Fail
 */
//-----------------------------------------------------------------------------
//extern INT32 FeederSubmitData(FeederSourceType eDataType, 
//                              const UINT8 *puiBuffer, UINT32 u4Size);

//-----------------------------------------------------------------------------
/** Application set its data query function for the feeder
 *
 *  @param  eDataType       Tramsmit data type (audio, video or program)
 *  @param  prSetQuery      Query setting
 *
 *  @retval FEEDER_E_OK     Succeed
 *  @retval FEEDER_E_FAIL   Fail
 */
//-----------------------------------------------------------------------------
extern INT32 FeederRegisterPullDataFunc(FeederSourceType eDataType, 
                                        const FEEDER_QUERY_DATA *prSetQuery);

//-----------------------------------------------------------------------------
/** Set the transmit parameters to the receiver
 *
 *  @param  eDataType       Tramsmit data type (audio, video or program)
 *  @param  fEnable         Enable / Disable data transmit to the receiver
 *
 *  @retval FEEDER_E_OK     Succeed
 *  @retval FEEDER_E_FAIL   Fail
 */
//-----------------------------------------------------------------------------
//extern INT32 FeederSetTransmit(FeederSourceType eDataType, BOOL fEnable);

//-----------------------------------------------------------------------------
/** Wait the event if there is more buffer space free
 *
 *  @param  eDataType       Data type (audio, video..)
 *
 *  @retval FEEDER_E_OK     Succeed
 *  @retval FEEDER_E_FAIL   Fail
 *  @retval FEEDER_E_STOP   Feeder stops
 */
//-----------------------------------------------------------------------------
//extern INT32 FeederWaitFreeBufferSpace(FeederSourceType eDataType);

//-----------------------------------------------------------------------------
/** Wait the event if the buffer is empty
 *
 *  @param  eDataType       Data type (audio, video..)
 *
 *  @retval FEEDER_E_OK     Succeed
 *  @retval FEEDER_E_FAIL   Fail
 *  @retval FEEDER_E_STOP   Feeder stops
 */
//-----------------------------------------------------------------------------
//extern INT32 FeederWaitBufferEmpty(FeederSourceType eDataType);

//-----------------------------------------------------------------------------
/** Set the transmit decode target
 *
 *  @param  u4VideoFmt      video decode format
 *  @param  u4AudioFmt      audio decode format
 *
 *  @retval FEEDER_E_OK     Succeed
 *  @retval FEEDER_E_FAIL   Fail
 */
//-----------------------------------------------------------------------------
//extern INT32 FeederSetDecodeFmt(UINT32 u4VideoFmt, UINT32 u4AudioFmt);

//-----------------------------------------------------------------------------
/** Get the transmit decode target format
 *
 *  @param  pu4VideoFmt     video decode format
 *  @param  pu4AudioFmt     audio decode format
 *
 *  @retval FEEDER_E_OK     Succeed
 *  @retval FEEDER_E_FAIL   Fail
 */
//-----------------------------------------------------------------------------
//extern INT32 FeederGetDecodeFmt(UINT32 *pu4VideoFmt, UINT32 *pu4AudioFmt);

//-----------------------------------------------------------------------------
/** Get the state of the target decoder
 *
 *  @param  eDataType       Tramsmit data type (audio, video or program)
 *  @param  prStatus        Decoder status
 *
 *  @retval FEEDER_E_OK     Succeed
 *  @retval FEEDER_E_FAIL   Fail
 */
//-----------------------------------------------------------------------------
//extern INT32 FeederGetDecodeStatus(FeederSourceType eDataType, 
//                                   FEEDER_DECODE_STATUS *prStatus);

//#ifndef __KERNEL__
//-----------------------------------------------------------------------------
/** Register the callback function of Feeder module
 *
 *  @param  eDataType       Tramsmit data type (audio, video or program)
 *  @param  eCallbackType   Context parameter
 *  @param  pfnCallback     Callback function pointer
 *  @param  u4Param         
 *
 *  @retval FEEDER_E_OK     Succeed
 *  @retval FEEDER_E_FAIL   Fail
 */
//-----------------------------------------------------------------------------
extern INT32 FeederInstallCallback(FeederSourceType eDataType,
                                   FEEDER_NOTIFY_COND eCallbackType,
                                   FEEDER_CALBACK pfnCallback, UINT32 u4Param);

//-----------------------------------------------------------------------------
/** Release the callback function of Feeder module
 *
 *  @param  eDataType       Tramsmit data type (audio, video or program)
 *  @param  eCallbackType   Context parameter
 *  @param  pfnCallback     Callback function pointer
 *
 *  @retval FEEDER_E_OK     Succeed
 *  @retval FEEDER_E_FAIL   Fail
 */
//-----------------------------------------------------------------------------
//extern INT32 FeederUnInstallCallback(FeederSourceType eDataType,
//                                     FEEDER_NOTIFY_COND eCallbackType,
//                                     FEEDER_CALBACK pfnCallback);
//#endif

//-----------------------------------------------------------------------------
/** Set the request parameters to the receiver
 *
 *  @param  eDataType       Data type (audio, video..)
 *  @param  u4QrySize       Query data size
 *  @param  u8Position        Query offset from file head
 *  @param  pResult           Feeder data transmit condition
 *  @param  u4ReqID         Reqest ID
 *
 *  @retval FEEDER_E_OK     Succeed
 *  @retval FEEDER_E_FAIL   Fail
 *  @retval FEEDER_E_STOP   Feeder stops
 */
//-----------------------------------------------------------------------------
extern INT32 FeederSetRequest(FeederSourceType eDataType, UINT32 u4QrySize, UINT64 u8Position,
                             FEEDER_TRANSMIT_COND *pResult, UINT32 u4ReqID);

//-----------------------------------------------------------------------------
/** notify handle of the driver
 *
 *  @param  eSrcType       Data type (audio, video..)
 *  @param  eCondition      Notify condition
 *  @param  u4Param
 *
 *  @retval FEEDER_E_OK     Succeed
 *  @retval FEEDER_E_FAIL   Fail
 *  @retval FEEDER_E_STOP   Feeder stops
 */
//-----------------------------------------------------------------------------
extern BOOL FeederNotifyHandle(FeederSourceType eSrcType, 
                         FEEDER_NOTIFY_COND eCondition, UINT32 u4Param);

#if 0
//-----------------------------------------------------------------------------
/** Set current application to pull mode
 *
 *  @param  eDataType       Tramsmit data type (audio, video or program)
 *  @param  eQueryMode    App query mode
 *
 *  @retval 
 */
//-----------------------------------------------------------------------------
#endif

//#ifdef CC_IC_VERIFY
extern VOID FeederSetOffset(FeederSourceType eDataType, UINT64 u8Offset, UINT8 u1Whence, UINT32 u4ReqID);
//#endif
extern VOID FeederSetRing(FeederSourceType eDataType, BOOL fgRing);
extern VOID FeederSetAppQueryMode(FeederSourceType eDataType, FeederQueryMode eQueryMode);
extern INT32 FeederUpdateReadPointer(FeederSourceType eDataType, UINT32 u4ReadAddr);
//#ifdef CC_IC_VERIFY
extern UINT32 FeederReadSomeBytes(FeederSourceType eDataType, UINT32 u4Size, UINT32 *pu4ReadSize);
extern INT32 FeederCopyBytesLE(VOID* pvFeederObj, VOID* pv_buf, SIZE_T z_buf_leng, SIZE_T z_copy_size, UINT32* pui4_size);
extern INT32 FeederInput4BytesLE(VOID* pvFeederObj, UINT32* pui4_data);
extern INT32 FeederInput3BytesLE(VOID* pvFeederObj, UINT32* pui4_data);
extern INT32 FeederInput2BytesLE(VOID* pvFeederObj, UINT16* pui2_data);
extern INT32 FeederInput1Bytes(VOID* pvFeederObj, UINT8*  pui1_data);
extern INT32 FeederSetPos(VOID* pvFeederObj, INT64 i8Offset, UINT8 u1Whence);
extern INT32 FeederGetPos(VOID* pvFeederObj, UINT64* pui8_cur_pos);
extern INT32 FeederCopyBytesBE(VOID* pvFeederObj, VOID* pv_buf, 
                                        SIZE_T z_buf_leng, SIZE_T z_copy_size, UINT32* pui4_size);
extern INT32 FeederInput4BytesBE(VOID* pvFeederObj, UINT32* pui4_data);
extern INT32 FeederInput3BytesBE(VOID* pvFeederObj, UINT32* pui4_data);
extern INT32 FeederInput2BytesBE(VOID* pvFeederObj, UINT16* pui2_data);
//#endif

#ifdef FEEDER_MULTI_INSTANCE
extern VOID FeederSetBagtInfo(FeederSourceType eDataType, UINT32 u4Align, UINT32 u4BlockSize);
extern VOID FeederSetUnderFlowThreshold(FeederSourceType eDataType, UINT32 u4Threshold);
extern VOID FeederSetReadyThreshold(FeederSourceType eDataType, UINT32 u4Threshold);
extern VOID FeederSetFlush(FeederSourceType eDataType);
//extern VOID FeederResetFlushId(FeederSourceType eDataType);
extern VOID FeederSetInfo(FeederSourceType eDataType, FeederSetInfoType eType,UINT32 u4Para);
extern VOID FeederIgnoreReady(FeederSourceType eDataType);
extern UINT32 FeederGetAlignSize(FeederSourceType eDataType);
#else
extern VOID FeederSetBagtInfo(UINT32 u4Align, UINT32 u4BlockSize);
extern VOID FeederSetUnderFlowThreshold(UINT32 u4Threshold);
extern VOID FeederSetReadyThreshold(UINT32 u4Threshold);
extern VOID FeederSetFlush(VOID);
extern VOID FeederSetInfo(FeederSetInfoType eType,UINT32 u4Para);
extern VOID FeederIgnoreReady(VOID);
extern UINT32 FeederGetAlignSize(void);
#endif
#ifdef CC_DLNA_SUPPORT
extern BOOL FeederIsReady(FeederSourceType eDataType);
#endif
//extern BOOL FeederIsUnderFlow(FeederSourceType eDataType);
extern INT32 FeederSetMMP(FeederSourceType eDataType);
extern INT32 FeederSetDataDelay(FeederSourceType eDataType,UINT32 u4DelayTime);
extern INT32 FeederSetUriMode(FeederSourceType eType, UINT32 u4BufSize, UINT32 u4KeepBufThres, UINT32 u4RebufThres);
extern INT32 FeederGetBufferFullness(FeederSourceType eType, UINT32 *pu4Len, UINT32 *pu4Percent);
extern INT32 FeederInitPreLoad(FeederSourceType eType, UINT32 *pu4Addr, UINT32 *pu4Size);
extern INT32 FeederSetPreLoadDone(FeederSourceType eType, UINT32 u4WriteSize);
extern INT32 FeederSetCacheSize(FeederSourceType eDataType, UINT32 u4Size);
extern UINT32 FeederCLIGetCacheSize(FeederSourceType eDataType);

extern VOID FeederSetSkypeSourceFlag(BOOL isSkype);
extern BOOL FeederGetSkypeSourceFlag(void);
//extern VOID FeederResetBuffer(FeederSourceType eDataType);
extern VOID FeederFlushBuffer(FeederSourceType eDataType);
extern VOID FeederSetRequestType(FeederSourceType eDataType, FeederRequestType eRequestType);
#endif  // DRV_FEEDER_IF_H

#endif  // BAGT_MWIF2

