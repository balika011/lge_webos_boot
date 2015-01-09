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
 * $RCSfile: x_buf_agt.h,v $
 * $Revision: #1 $
 * $Date: 2015/01/09 $
 * $Author: dtvbm11 $
 *
 * Description:
 *         This header file contains buffer agent driver specific
 *         definitions, which are exported.
 *---------------------------------------------------------------------------*/

#ifndef _X_BUF_AGT_H_
#define _X_BUF_AGT_H_


/*-----------------------------------------------------------------------------
                    include files
-----------------------------------------------------------------------------*/

#include "x_common.h"
#include "x_rm.h"


/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/

//#define BAGT_URI_MODE_1  // for new consume mechanism
#define BAGT_URI_MODE_2    // for flow control and other stuff

/* Get operations */
#define BUF_AGT_GET_TYPE_SRC_ALIGN_INFO    (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 0))
#define BUF_AGT_GET_TYPE_ACK_FCT_TAB       (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 1))
#define BUF_AGT_GET_TYPE_POSITION          (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 2))

#define BUF_AGT_GET_TYPE_BUFFER_FULLNESS   (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 3))
#define BUF_AGT_GET_TYPE_SHARED_MEMORY     (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 4))
#define BUF_AGT_GET_TYPE_TIME_TO_SIZE      (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 5))

/* Set operations */
#define BUF_AGT_SET_TYPE_NFY_FCT           (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 0))
#define BUF_AGT_SET_TYPE_SRC_ALIGN_INFO    (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 1))
#define BUF_AGT_SET_TYPE_FLUSH_DONE        ((RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 2)) | RM_SET_TYPE_ARG_NO_REF)
#define BUF_AGT_SET_TYPE_ACK               (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 3))

#define BUF_AGT_SET_OPERATION_MODE         (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 4))
#define BUF_AGT_SET_TYPE_PRELOAD           (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 5))
#define BUF_AGT_SET_TYPE_SHARED_MEMORY     ((RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 6)) | RM_SET_TYPE_ARG_NO_REF)
#define BUF_AGT_SET_TYPE_SOURCE_TYPE       ((RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 7)) | RM_SET_TYPE_ARG_NO_REF)




/* Registration Flags */
#define BUF_AGT_REG_FLAG_NONE              ((UINT32) 0)

/* flags in BUF_AGT_DATA_BLK_T */
#define BUF_AGT_SET_DATA_OFFSET            ((UINT32) 1)
#define BUF_AGT_GET_DATA_EOF               ((UINT32) 2)
#define BUF_AGT_SHARED_MEMORY              ((UINT32) 3)

#define BUF_AGT_DLNA_TIME_SEEK_INVALID_TIME 0xFFFFFFFFFFFFFFFF
typedef enum
{
	BUF_AGT_OFFSET_SEEK = 0,
	BUF_AGT_TIME_SEEK
}BUF_AGT_REQUEST_TYPE_T;
typedef enum
{
    BUF_AGT_SOURCE_TYPE_DEFAULT = 0,
    BUF_AGT_SOURCE_TYPE_SKYPE
} BUF_AGT_SOURCE_TYPE_T;

typedef enum
{
    BUF_AGT_OPERATION_MODE_PULL = 0,
    BUF_AGT_OPERATION_MODE_CIRCULAR,
    BUF_AGT_OPERATION_MODE_PUSH_SHARE_MEM,
} BUF_AGT_OPERATION_MODE_T;

typedef enum
{
    BUF_AGT_FEEDER_BUF_MODE_TIME = 0,
    BUF_AGT_FEEDER_BUF_MODE_BYTE,
    BUF_AGT_FEEDER_BUF_MODE_PIDX,
} BUF_AGT_FEEDER_BUF_MODE_T;


typedef struct _BUF_AGT_FEEDER_BUF_INFO_T
{
  BUF_AGT_FEEDER_BUF_MODE_T e_mode;
  union
  {
      UINT32 ui4_time_ms;    
  }  u;
} BUF_AGT_FEEDER_BUF_INFO_T;


typedef struct _BUF_AGT_FLOW_CTRL_T
{
    UINT32  ui4_buffer_len;             /*Buffer size in Bytes. */
    UINT32  ui4_keep_buffer_threshold;  /*Keep threshold in Bytes, at least keep this size of data which can used by seek backward. */
    UINT32  ui4_re_buferf_threshold;    /*Re-Buffer threshold in Bytes, at least this size of space before pulling data from Application */
} BUF_AGT_FLOW_CTRL_T;

typedef struct _BUF_MODE_CTRL_T
{
    BUF_AGT_OPERATION_MODE_T    e_op_mode; /*buffer agent operation mode.*/
    BUF_AGT_FEEDER_BUF_INFO_T   t_feeder_buf_info;     
    BOOL                        b_quick_consume; /*if true, buffer agent consume data once receive ack data*/
    union 
    {
        BUF_AGT_FLOW_CTRL_T     t_flow_ctrl;
    } u;
    UINT32                      ui4_pid;
} BUF_MODE_CTRL_T;

typedef struct _BUF_AGT_BUFFER_FULLNESS_T
{
    UINT32     ui4_length;
    UINT32     ui4_percentage;
} BUF_AGT_BUFFER_FULLNESS_T;

typedef struct _BUF_AGT_BUFFER_SHARED_MEMORY_T
{
    VOID*   pv_buffer;
    VOID*   pv_private;
    SIZE_T  z_buffer_size;
}   BUF_AGT_BUFFER_SHARED_MEMORY_T;

typedef struct _BUF_AGT_BUFFER_TIME_TO_SIZE_T
{
    UINT32  ui4_ms; /*IN: time in miliseconds*/
    UINT32  ui4_size; /*OUT: size in bytes*/
}   BUF_AGT_BUFFER_TIME_TO_SIZE_T;

/* BUF_AGT_GET_TYPE_SRC_ALIGN_INFO/BUF_AGT_SET_TYPE_SRC_ALIGN_INFO */
typedef struct _BUF_AGT_SRC_ALIGN_INFO_T
{
    UINT32     ui4_align;
    UINT32     ui4_min_blk_size;
}   BUF_AGT_SRC_ALIGN_INFO_T;


/* BUF_AGT_GET_TYPE_ACK_FCT_TAB */
typedef enum
{
    BUF_AGT_DATA_REQ_ACK_COND_UNKNOWN = 0,
    BUF_AGT_DATA_REQ_ACK_COND_DONE,
    BUF_AGT_DATA_REQ_ACK_COND_ERROR,
    BUF_AGT_DATA_REQ_ACK_COND_CANCELED
} BUF_AGT_DATA_REQ_ACK_COND_T;


typedef struct
{
    PTS_T  t_pts[10];                     //record all pts values in this requrst;
    UINT32 ui4_pts_size[10];              //range of each pts value (in bytes); 
    UINT16 ui2_pts_cnt;                   //the number of pts in this request
    BOOL b_eof;                           //drivers need this info when they handle the last frame
}   BUF_AGT_DATA_BLK_ES_INFO_T;


typedef enum
{
    BUF_AGT_CTRL_STREAM_TYPE_AUDIO,
    BUF_AGT_CTRL_STREAM_TYPE_VIDEO
} BUF_AGT_CTRL_STREAM_TYPE_T;

typedef struct
{
    BUF_AGT_CTRL_STREAM_TYPE_T      e_stream_type;      /* the stream type whose packet info is to be modified */
    UINT32                          ui4_stream_id;      /* the stream ID used by the ASF demuxer, associated with this stream type */
    UINT64                          ui8_preroll;        /* preroll */
    UINT32                          ui4_packet_size;    /* size of the packet associated with this stream type */
} BUF_AGT_CTRL_IBC_PARAM_SET_ASF_PACKET_INFO_T;

typedef enum
{
    BUF_AGT_CTRL_IBC_TYPE_SET_ASF_PACKET_INFO = 1,
    BUF_AGT_CTRL_IBC_TYPE_EOS,
    BUF_AGT_CTRL_IBC_TYPE_ENCRYPT,
    BUF_AGT_CTRL_IBC_TYPE_SEQHEADER,
    BUF_AGT_CTRL_IBC_TYPE_STRM_INFO,
    BUF_AGT_CTRL_IBC_TYPE_DASH_SET_INFO,
    BUF_AGT_CTRL_IBC_TYPE_HDD_CONTENT_KEY,
    BUF_AGT_CTRL_IBC_TYPE_DTCP_IP_KEY,
    BUF_AGT_CTRL_IBC_TYPE_PUSH_CHANGE_AUDIO
} BUF_AGT_CTRL_IBC_TYPE_T;

typedef enum
{
    BUF_AGT_CTRL_IBC_ENCRYPT_MODE_NONE = 0,             
    BUF_AGT_CTRL_IBC_ENCRYPT_MODE_DES_ECB,              
    BUF_AGT_CTRL_IBC_ENCRYPT_MODE_DES_CBC,              
    BUF_AGT_CTRL_IBC_ENCRYPT_MODE_3DES_ECB,             
    BUF_AGT_CTRL_IBC_ENCRYPT_MODE_3DES_CBC,             
    BUF_AGT_CTRL_IBC_ENCRYPT_MODE_DVB,                  
    BUF_AGT_CTRL_IBC_ENCRYPT_MODE_DVB_CONF,             
    BUF_AGT_CTRL_IBC_ENCRYPT_MODE_MULTI2_BIG,           
    BUF_AGT_CTRL_IBC_ENCRYPT_MODE_MULTI2_LITTLE,        
    BUF_AGT_CTRL_IBC_ENCRYPT_MODE_AES_ECB,              
    BUF_AGT_CTRL_IBC_ENCRYPT_MODE_AES_CBC,
    BUF_AGT_CTRL_IBC_ENCRYPT_MODE_MTK
} BUF_AGT_CTRL_IBC_ENCRYPT_MODE_T;

/* Descrambling residual termination block(RTB)*/
typedef enum
{
    BUF_AGT_CTRL_IBC_RTB_MODE_CLEAR = 0,
    BUF_AGT_CTRL_IBC_RTB_MODE_CTS,
    BUF_AGT_CTRL_IBC_RTB_MODE_SCTE52,
    BUF_AGT_CTRL_IBC_RTB_MODE_MAX,
} BUF_AGT_CTRL_IBC_RTB_MODE_T;

#define BUF_AGT_AES_ENCRYPT_IV        1
#define BUF_AGT_AES_ENCRYPT_UNIQUE    2
#define BUF_AGT_AES_ENCRYPT_EVEN      4
#define BUF_AGT_AES_ENCRYPT_ODD       8
typedef struct
{
    UINT8       ui1_mask;
    UINT8       ui1_key_len; /*specify key length of aui1_even_key and aui1_odd_key*/
    BOOL        b_warp_key;
    BOOL        b_warp_IV;
    UINT8       aui1_IV[16];
    UINT8       aui1_uniqueKey[16];
    UINT8       aui1_even_key[32];
    UINT8       aui1_odd_key[32];
    BUF_AGT_CTRL_IBC_RTB_MODE_T     e_rtb_mode;
    BUF_AGT_CTRL_IBC_ENCRYPT_MODE_T e_aes_mode;
} BUF_AGT_CTRL_IBC_AES_ENCRYPT_INFO_T;


/* Encryption/Descryption key index */
typedef enum
{
    BUF_AGT_MM_CRYPT_KEY_ODD = 0,
    BUF_AGT_MM_CRYPT_KEY_EVEN,
    BUF_AGT_MM_CRYPT_KEY_ALTERNATE
}   BUF_AGT_MM_CRYPT_KEY_INDEX_T;

/* Encryption/Descryption info */
typedef struct 
{
    BUF_AGT_CTRL_IBC_ENCRYPT_MODE_T       e_mode;
    UINT32                   aui4_odd_key_1[2];
    UINT32                   aui4_odd_key_2[2];
    UINT32                   aui4_even_key_1[2];
    UINT32                   aui4_even_key_2[2];
    BUF_AGT_MM_CRYPT_KEY_INDEX_T    e_key_idx;    
}BUF_AGT_PVR_CRYPT_INFO_T;


typedef struct 
{
    BUF_AGT_CTRL_IBC_ENCRYPT_MODE_T     e_mode;
    STREAM_TYPE_T       e_strm_type;
    UINT32              ui4_content_id;
    UINT16              ui2_key_idx;
    UINT8               aui1_key[16];
}BUF_AGT_MM_TRACK_MARLIN_CRYPT_INFO_T;

#define BUF_AGT_MAX_MARLIN_TRACK_NUM        17/* marlin max number of track : 1(video) + 2(audio) + 4(subtitle) */
#define BUF_AGT_RESEVED_KEY_LENGTH_MAX      576
typedef struct 
{
    UINT8                   ui1_valid_cnt;
    BUF_AGT_MM_TRACK_MARLIN_CRYPT_INFO_T    t_track_drm[BUF_AGT_MAX_MARLIN_TRACK_NUM];
}BUF_AGT_MM_MLN_CRYPT_INFO_T;

typedef struct 
{
    UINT8                           ui1_valid_cnt;
    BUF_AGT_MM_TRACK_MARLIN_CRYPT_INFO_T    t_track_drm[BUF_AGT_MAX_MARLIN_TRACK_NUM];
}BUF_AGT_MM_PDCF_CRYPT_INFO_T;

/*DASH  ENCRYPT_INFO*/
typedef struct _BUF_AGT_SUB_SAMPLE_INFO_T
{
    UINT16   ui2_size_clear_data;
    UINT32   ui4_size_encrypted_data;
}BUF_AGT_SUB_SAMPLE_INFO_T;

typedef struct _BUF_AGT_SENC_T
{
    UINT32     ui4_sub_sample_cnt;
    BUF_AGT_SUB_SAMPLE_INFO_T   a_sub_sample[32];
}BUF_AGT_SENC_T;

typedef struct _BUF_AGT_CTR_ENCRYPT
{  
    UINT8      ui1_iv[16];
    UINT32     ui4_ivlength;
    BUF_AGT_SENC_T   t_dash_senc;
}BUF_AGT_AES_CTR_ENCRYPT;

typedef enum
{
    BUF_AGT_MM_PRDY_ENCRPT_KEY_UNKNOWN = 0,
    BUF_AGT_MM_PRDY_ENCRPT_KEY_RC4,
    BUF_AGT_MM_PRDY_ENCRPT_KEY_AES
} BUF_AGT_MM_PRDY_KEY_TYPE;

typedef struct
{
    BUF_AGT_MM_PRDY_KEY_TYPE    e_key_type;
    UINT8               aui1_key_info[128];
    UINT32              ui4_key_size;
}BUF_AGT_MM_PRDY_ENCRPT_KEY_INFO_T;

typedef struct
{
    UINT32             ui4_aud_strm_id;       ///< PID
    UINT32             ui4_vid_strm_id;       ///< PID    
    UINT8          e_vid_enc;
    UINT8          e_aud_enc;
} BUF_AGT_CTRL_IBC_PARAM_STRM_INFO_T;

#define BUF_AGT_CTRL_MTK_ENCRYPT_SEGMENT_MAX_NUM   (64)
typedef struct
{
    UINT32      u4Size;     ///< Segment Size
    BOOL        fgEnc;      ///< Segment encrypted or not
} BUF_AGT_CTRL_MTK_ENCRYPT_SEGMENT_T;

/*! @struct IMTK_PB_CTRL_MTK_ENCRPT_KEY_INFO_T
 *  @brief  MTK Encrypted segment key info
 */
typedef struct 
{
    UINT8                                   u1KeyIdx;   ///< MTK key slot index
    UINT16                                  u2SegNum;   ///< How many segments of atSegInfo
    BUF_AGT_CTRL_MTK_ENCRYPT_SEGMENT_T      atSegInfo[BUF_AGT_CTRL_MTK_ENCRYPT_SEGMENT_MAX_NUM]; ///< array of segments
    UINT8                                   au1IV[16];
}BUF_AGT_CTRL_MTK_ENCRYPT_INFO_T;

typedef enum
{
    BUF_AGT_MM_CRYPT_TYPE_UNKNOWN = 0,
    BUF_AGT_MM_CRYPT_TYPE_PVR,
    BUF_AGT_MM_CRYPT_TYPE_MLN,
    BUF_AGT_MM_CRYPT_TYPE_AES,
    BUF_AGT_MM_CRYPT_TYPE_PDCF,
    BUF_AGT_MM_CRYPT_TYPE_PRDY,
    BUF_AGT_MM_CRYPT_TYPE_CENC,
    BUF_AGT_MM_CRYPT_TYPE_MTK,
    //MM_CRYPT_TYPE_MS3,
    BUF_AGT_MM_CRYPT_TYPE_RSV
} BUF_AGT_MM_CRYPT_TYPE_T;

typedef struct
{
    BUF_AGT_MM_CRYPT_TYPE_T         e_mm_crypt_type;
    union
    {
        BUF_AGT_PVR_CRYPT_INFO_T                t_pvr_crypt;
        BUF_AGT_MM_MLN_CRYPT_INFO_T             t_ml_crypt;
        BUF_AGT_CTRL_IBC_AES_ENCRYPT_INFO_T     t_aes;
        BUF_AGT_MM_PDCF_CRYPT_INFO_T            t_pdcf_crypt;
        BUF_AGT_MM_PRDY_ENCRPT_KEY_INFO_T       t_prdy_crypt;
        BUF_AGT_CTRL_MTK_ENCRYPT_INFO_T         t_mtk_key;
        BUF_AGT_AES_CTR_ENCRYPT                 t_aes_ctr_crypt;
        UINT8                                   aui1Reserved[BUF_AGT_RESEVED_KEY_LENGTH_MAX];
    }u_encrypt_info;
} BUF_AGT_CTRL_IBC_ENCRYPT_INFO_T;

typedef enum
{
    BUF_AGT_CTRL_VSH_SRC_FRAME_RATE_UNKNOWN = 0,
    BUF_AGT_CTRL_VSH_SRC_FRAME_RATE_23_976, /* 24000/1001 (23.976...) */
    BUF_AGT_CTRL_VSH_SRC_FRAME_RATE_24,
    BUF_AGT_CTRL_VSH_SRC_FRAME_RATE_25,
    BUF_AGT_CTRL_VSH_SRC_FRAME_RATE_29_97, /* 30000/1001 (29.97...) */
    BUF_AGT_CTRL_VSH_SRC_FRAME_RATE_30,
    BUF_AGT_CTRL_VSH_SRC_FRAME_RATE_50,
    BUF_AGT_CTRL_VSH_SRC_FRAME_RATE_59_94, /* 60000/1001 (59.94...) */
    BUF_AGT_CTRL_VSH_SRC_FRAME_RATE_60,
    BUF_AGT_CTRL_VSH_SRC_FRAME_RATE_120,
    BUF_AGT_CTRL_VSH_SRC_FRAME_RATE_1,
    BUF_AGT_CTRL_VSH_SRC_FRAME_RATE_5,
    BUF_AGT_CTRL_VSH_SRC_FRAME_RATE_8,
    BUF_AGT_CTRL_VSH_SRC_FRAME_RATE_10,
    BUF_AGT_CTRL_VSH_SRC_FRAME_RATE_12,
    BUF_AGT_CTRL_VSH_SRC_FRAME_RATE_15,
    BUF_AGT_CTRL_VSH_SRC_FRAME_RATE_2,
    BUF_AGT_CTRL_VSH_SRC_FRAME_RATE_6,
    BUF_AGT_CTRL_VSH_SRC_FRAME_RATE_48,
    BUF_AGT_CTRL_VSH_SRC_FRAME_RATE_70,
    BUF_AGT_CTRL_VSH_SRC_FRAME_RATE_20,
    BUF_AGT_CTRL_VSH_SRC_FRAME_RATE_VARIABLE
} BUF_AGT_CTRL_VSH_SRC_FRAME_RATE_T;

typedef enum
{    
    BUF_AGT_CTRL_VSH_ASP_USR_TYPE_SRC = 1,   
    BUF_AGT_CTRL_VSH_ASP_USR_TYPE_DISP    
} BUF_AGT_CTRL_VSH_ASP_USR_TYPE_T;

typedef enum 
{
    BUF_AGT_CTRL_VSH_SRC_ASPECT_RATIO_UNKNOWN = 0,
    BUF_AGT_CTRL_VSH_SRC_ASPECT_RATIO_4_3,         /* DAR 4:3 */
    BUF_AGT_CTRL_VSH_SRC_ASPECT_RATIO_16_9,        /* DAR 16:9 */
    BUF_AGT_CTRL_VSH_SRC_ASPECT_RATIO_2_21_1,      /* DAR 2.21:1 */
    BUF_AGT_CTRL_VSH_SRC_ASPECT_RATIO_PXL_10_11,   /* PAR 10:11 */
    BUF_AGT_CTRL_VSH_SRC_ASPECT_RATIO_PXL_40_33,   /* PAR 40:33 */
    BUF_AGT_CTRL_VSH_SRC_ASPECT_RATIO_PXL_16_11,   /* PAR 16:11 */
    BUF_AGT_CTRL_VSH_SRC_ASPECT_RATIO_PXL_12_11,   /* PAR 12:11 */
    BUF_AGT_CTRL_VSH_SRC_ASPECT_RATIO_PXL_3_2,     /* PAR 3:2 */
    BUF_AGT_CTRL_VSH_SRC_ASPECT_RATIO_PXL_1_1,     /* PAR 1:1 */
    BUF_AGT_CTRL_VSH_SRC_ASPECT_RATIO_USR          /* user */
} BUF_AGT_CTRL_VSH_SRC_ASPECT_RATIO_T;


typedef enum
{
	BUF_AGT_PRINT_TMS_UNKNOWN = 0,
	BUF_AGT_PRINT_TMS_PRINT,
	BUF_AGT_PRINT_TMS_NOT_PRINT
}BUF_AGT_PRINT_TMS_T;

typedef struct 
{
    UINT16                     ui2_width;      /* horizontal_size_value */
    UINT16                     ui2_height;     /* vertical_size_value */
    BUF_AGT_CTRL_VSH_SRC_FRAME_RATE_T       e_frame_rate;   /* Frame Rate */
    BUF_AGT_CTRL_VSH_SRC_ASPECT_RATIO_T     e_src_asp;      /* Display Aspect Ratio (DAR) */
    BUF_AGT_CTRL_VSH_ASP_USR_TYPE_T         e_asp_usr_type; /* source or display asp */
    UINT16                     ui2_asp_usr_h;
    UINT16                     ui2_asp_usr_w;      
    UINT32                     ui4_seq_hdr;
    UINT16                     ui2_seq_hdr_sz;        
    UINT8                      ui1_sample_id;
    BOOL                       b_chk_framerate;
}   BUF_AGT_CTRL_VSH_SEQUENCE_DATA_T;


typedef struct 
{
    UINT8*                     pc_buff;
    SIZE_T                     z_buff_size;
    BUF_AGT_CTRL_VSH_SRC_FRAME_RATE_T       e_frame_rate;  
    UINT8                      ui1_sample_id;
}   BUF_AGT_CTRL_VSH_SEQUENCE_RAW_T;


typedef struct _BUF_AGT_CTRL_IBC_VIDEO_SEQHEADER_INFO_T
{
    BOOL                       b_is_raw;
    BOOL                       b_is_second_raw;

    union
    {
        BUF_AGT_CTRL_VSH_SEQUENCE_RAW_T         t_seq_raw;
        BUF_AGT_CTRL_VSH_SEQUENCE_DATA_T        t_seq_data;
    } u;
#if SUPPORT_MULTI_INFO
    struct _BUF_AGT_CTRL_IBC_VIDEO_SEQHEADER_INFO_T*  pt_next_scale;
#endif
}  BUF_AGT_CTRL_IBC_VIDEO_SEQHEADER_INFO;


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
}BUF_AGT_CTRL_IBC_AUDIO_SEQHEADER_INFO;

typedef struct
{
    BUF_AGT_CTRL_STREAM_TYPE_T    e_strm_type;
    union
    {
        BUF_AGT_CTRL_IBC_VIDEO_SEQHEADER_INFO   t_vid_seqheader_info;
        BUF_AGT_CTRL_IBC_AUDIO_SEQHEADER_INFO   t_aud_seqheader_info;
    }u;
}BUF_AGT_CTRL_IBC_PARAM_SET_SEQHEADER_INFO;

typedef struct
{
    UINT64    ui8_bandwidth;
    BOOL      b_bandwidth_chg;
}BUF_AGT_CTRL_IBC_DASH_SET_INFO_T;

typedef struct
{
    BOOL    b_is_encrypt;
    UINT16  ui2_key_len;
    UINT8   aui1_key_info[256];
    UINT16  ui2_ext_key_len;
    UINT8   aui1_ext_key_info[256];
}BUF_AGT_CTRL_HDD_DTCP_IP_INFO_T;
typedef union
{
    BUF_AGT_CTRL_IBC_PARAM_SET_ASF_PACKET_INFO_T    t_set_asf_packet_info_param;
    BUF_AGT_CTRL_IBC_ENCRYPT_INFO_T                 t_set_encrypt_info;
    BUF_AGT_CTRL_IBC_PARAM_SET_SEQHEADER_INFO       t_set_seqheader_info_param;
    BUF_AGT_CTRL_IBC_PARAM_STRM_INFO_T              t_set_strm_info_param;
    BUF_AGT_CTRL_IBC_DASH_SET_INFO_T                t_set_dash_info;
    BUF_AGT_CTRL_HDD_DTCP_IP_INFO_T                 t_set_hdd_dtcp_ip_info;
} BUF_AGT_CTRL_IBC_PARAM_T;

typedef struct _BUF_AGT_CTRL_IBC_T
{
    BUF_AGT_CTRL_IBC_TYPE_T     e_cmd_type;
    UINT32                      ui4_id;           /*< reserved for future use*/
    BUF_AGT_CTRL_IBC_PARAM_T    t_param;
} BUF_AGT_CTRL_IBC_T;

typedef union
{
    BUF_AGT_DATA_BLK_ES_INFO_T  t_es_info;
    BUF_AGT_CTRL_IBC_T          t_ctrl_ibc;
}   BUF_AGT_DATA_BLK_EXTRA_T;

typedef struct _BUF_AGT_DATA_BLK_T
{
    UINT8*              pui1_buff;
    SIZE_T              z_size;
    UINT32              ui4_req_id;
    UINT32              ui4_flags;
    UINT64              ui8_offset;
    DATA_FMT_T          e_data_fmt;
    BUF_AGT_DATA_BLK_EXTRA_T  t_extra;
    BUF_AGT_PRINT_TMS_T e_print_tms;  
    BUF_AGT_REQUEST_TYPE_T t_req_type;
    UINT64              ui8_vid_pts;//for wfd debug
    UINT64              ui8_push_time;//ms,for wfd debug
} BUF_AGT_DATA_BLK_T;

typedef struct
{
    BUF_AGT_DATA_REQ_ACK_COND_T  e_cond;
    BUF_AGT_DATA_BLK_T           t_block;
}   BUF_AGT_ACK_NFY_T;

typedef BOOL (*x_buf_agt_data_req_ack_fct) (
    UINT16                        ui2_buf_agt_id,
    BUF_AGT_DATA_REQ_ACK_COND_T   e_ack_cond,
    BUF_AGT_DATA_BLK_T*           pt_data_blk );

typedef struct _BUF_AGT_FCT_TAB_T
{
    x_buf_agt_data_req_ack_fct   pf_data_req_ack;
} BUF_AGT_FCT_TAB_T;

/* BUF_AGT_GET_TYPE_POSITION */
typedef struct _BUF_AGT_POSITION_T
{
    UINT32     ui4_current_req_id;
    UINT32     ui4_current_byte_offset;
} BUF_AGT_POSITION_T;

/* BUF_AGT_SET_TYPE_NFY_FCT */
typedef enum
{
    BUF_AGT_NFY_COND_UNKNOWN = 0,
    BUF_AGT_NFY_COND_DATA_REQ,
    BUF_AGT_NFY_COND_FLUSH_REQ,
    BUF_AGT_NFY_COND_DATA_CONSUMED,
    BUF_AGT_NFY_COND_DATA_OVERFLOW,
    BUF_AGT_NFY_COND_DATA_UNDERFLOW
} BUF_AGT_NFY_COND_T;

typedef BOOL (*x_buf_agt_nfy_fct) (
    VOID*                  pv_nfy_tag,
    BUF_AGT_NFY_COND_T     e_nfy_cond,
    UINT32                 ui4_data_1,
    UINT32                 ui4_data_2 );

typedef struct _BUF_AGT_NFY_INFO_T
{
    VOID*                   pv_tag;
    x_buf_agt_nfy_fct       pf_nfy;
    VOID*                   pv_previous_tag;  /* OUTPUT */
    x_buf_agt_nfy_fct       pf_previous_nfy;  /* OUTPUT */
}   BUF_AGT_NFY_INFO_T;


#endif /* _X_BUF_AGT_H_ */
