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
 * $RCSfile: x_mux.h,v $
 * $Revision: #1 $
 * $Date: 2015/01/09 $
 * $Author: dtvbm11 $
 *
 * Description:
 *         This header file contains Personal Video Recorder driver exported definitions.
 *---------------------------------------------------------------------------*/

#ifndef _X_MUX_H_
#define _X_MUX_H_

/*-----------------------------------------------------------------------------
                    include files
-----------------------------------------------------------------------------*/

#include "x_common.h"
#include "x_rm.h"
#include "x_mm_common.h"
#include "x_aud_enc.h"
#include "x_vid_enc.h"


/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 -----------------------------------------------------------------------------*/

/* Get operations */
#define MUX_GET_TYPE_CTRL              ( RM_GET_TYPE_LAST_ENTRY + ( ( DRV_GET_TYPE_T)  1 ) )


/* Set operations */
#define MUX_SET_TYPE_NFY_FCT           ( RM_SET_TYPE_LAST_ENTRY + ( ( DRV_SET_TYPE_T ) 0 ) ) /*set notify info to muxer or muxer ctrl*/
#define MUX_SET_TYPE_CTRL              ( ( RM_SET_TYPE_LAST_ENTRY + ( ( DRV_SET_TYPE_T ) 1 ) ) | RM_SET_TYPE_ARG_NO_REF )/*enable muxer or muxer ctrl*/
#define MUX_SET_TYPE_BUFFER_SETTING    ( RM_SET_TYPE_LAST_ENTRY + ( ( DRV_SET_TYPE_T ) 2 ) ) /*set ring buffer address and length*/
#define MUX_SET_TYPE_UNLOCK_BUFFER     ( RM_SET_TYPE_LAST_ENTRY + ( ( DRV_SET_TYPE_T ) 3 ) ) /*release notified data buffer*/ 
#define MUX_SET_TYPE_FLUSH_BUFFER      ( RM_SET_TYPE_LAST_ENTRY + ( ( DRV_SET_TYPE_T ) 4 ) ) /*flush ring buffer*/
#define MUX_SET_TYPE_ENCRYPT_KEY       ( RM_SET_TYPE_LAST_ENTRY + ( ( DRV_SET_TYPE_T ) 5 ) ) /* encrypt key*/
#define MUX_SET_TYPE_ENCRYPT_CTRL      ( ( RM_SET_TYPE_LAST_ENTRY + ( ( DRV_SET_TYPE_T ) 6 ) ) | RM_SET_TYPE_ARG_NO_REF )/*enable muxer or muxer ctrl*/
#define MUX_SET_TYPE_CODEC_INFO         ( RM_SET_TYPE_LAST_ENTRY + ( ( DRV_SET_TYPE_T ) 7 ) ) /* set codec*/

/* notification conditions */
typedef enum _MUX_NFY_COND_T
{
    MUX_COND_ERROR    = -2, /* Error */
    MUX_COND_OVERFLOW = -1, /* Buffer Overflow */
    MUX_COND_DATA     =  0  /* Buffer Data Available */
} MUX_NFY_COND_T;


/* Overflow reasons ((*x_mux_nfy_fct)(e_nfy_cond == MUX_COND_OVERFLOW,  ui4_data => overflow reason) ) */
#define MUX_OVERFLOW_REASON_RING_BUF_FULL         ( ( UINT32 ) 1 )
#define MUX_OVERFLOW_REASON_METADATA_BUF_FULL     ( ( UINT32 ) 2 )


/* Error reasons ((*x_mux_nfy_fct)(e_nfy_cond == MUX_COND_OVERFLOW,  ui4_data => overflow reason) ) */
#define MUX_ERROR_REASON_INTERNAL         ( ( UINT32 ) 1 )


//typedef UINT64  PTS_T;    

/* for MUX_SET_TYPE_NFY_FCT */
typedef BOOL (*x_mux_nfy_fct)( VOID*           pv_tag, 
                                  MUX_NFY_COND_T  e_nfy_cond,
                                  UINT32          ui4_data );
/*mux format*/
typedef UINT8                          MUX_BUF_TYPE_T;
#define MUX_DATA_TYPE_MTK              ( ( MUX_BUF_TYPE_T ) 0 ) /* set to muxer for only one stream  or to  muxer ctrl for multiple streams*/
#define MUX_DATA_TYPE_TS               ( ( MUX_BUF_TYPE_T ) 1 ) /* set to muxer for only one stream  or  to muxer ctrl for multiple streams*/
#define MUX_DATA_TYPE_TS_192           ( ( MUX_BUF_TYPE_T ) 4 ) /* set to muxer for only one stream  or  to muxer ctrl for multiple streams*/
#define MUX_DATA_TYPE_VIDEO_ES         ( ( MUX_BUF_TYPE_T ) 2 ) /*ony mux video : set to muxer */
#define MUX_DATA_TYPE_AUDIO_ES         ( ( MUX_BUF_TYPE_T ) 3 ) /*only mux audio: set to muxer */


#define MUX_TS_NETWORK_PID  (0x001F & 0x1FFF)
#define MUX_TS_PMT_PID      (0x0100 & 0x1FFF)
#define MUX_TS_PCR_PID      (0x1001 & 0x1FFF)
#define MUX_TS_V_PID        (0x3FF & 0x1FFF)
#define MUX_TS_A_PID        (0x1100 & 0x1FFF)
#define MUX_TS_TT_PID       (0x446 & 0x1FFF)


/*-----------------------------------------------------------------------------
                    structures
-----------------------------------------------------------------------------*/

/* for MUX_COND_DATA condition: notify data to mw */
typedef struct _MUX_BUF_DATA_T
{
    UINT8*          pui1_buf_phy;   /* kernel space address */ 
    SIZE_T          t_size;
    UINT64          t_pts;
    UINT32          ui4_flag;
} MUX_BUF_DATA_T;

/* for MUX_SET_TYPE_UNLOCK_BUFFER,  */
typedef struct _MUX_UNLOCK_BUFFER_INFO_T
{
    UINT8*      pui1_data_addr;    /* kernel space address */ 
    SIZE_T      z_data_len;
} MUX_UNLOCK_BUFFER_INFO_T;

typedef struct _MUX_BUF_MTK_T
{
    UINT32              ui4_tick_period; /*if 0 can notify anytime or notify by timer*/
    SIZE_T              z_buf_size;
    VOID*               pv_buf_phy;     /* Kernel Space Address */
} MUX_BUF_MTK_T;

typedef struct _MUX_BUF_TS_T
{
    UINT32              ui4_tick_period;
    
    SIZE_T              z_buf_size;
    VOID*               pv_buf_phy;     /* Kernel Space Address */
} MUX_BUF_TS_T;

typedef struct _MUX_BUF_VID_ES_T
{
    UINT32              ui4_tick_period; /*if 0 can notify anytime or notify by timer*/
    SIZE_T              z_buf_size;
    VOID*               pv_buf_phy;     /* Kernel Space Address */
} MUX_BUF_VID_ES_T;

typedef struct _MUX_BUF_AUD_ES_T
{
    UINT32              ui4_tick_period; /*if 0 can notify anytime or notify by timer*/
    SIZE_T              z_buf_size;
    VOID*               pv_buf_phy;     /* Kernel Space Address */
} MUX_BUF_AUD_ES_T;


/* for MUX_SET_TYPE_BUFFER_SETTING operation */
typedef struct _MUX_BUF_SETTING_T
{
    MUX_BUF_TYPE_T          t_type;
    union
    {
        MUX_BUF_MTK_T       t_buf_mtk;
        MUX_BUF_TS_T        t_buf_ts;
        MUX_BUF_VID_ES_T    t_buf_vid;
        MUX_BUF_AUD_ES_T    t_buf_aud;
    }u;
} MUX_BUF_SETTING_T;

typedef struct _MUX_NFY_INFO_T
{
    VOID*               pv_tag;
    x_mux_nfy_fct       pf_nfy;
    
} MUX_NFY_INFO_T;

typedef enum _MUX_SET_CODEC_TYPE_T
{
    MUX_SET_CODEC_TYPE_VENC = 0,                      ///< source type video encoder
    MUX_SET_CODEC_TYPE_AENC,                          ///< source type audio encoder
    MUX_SET_CODEC_TYPE_AV,                          ///< source type audio encoder
    MUX_SET_CODEC_TYPE_VBI,                           ///< source type VBI
    MUX_SET_CODEC_TYPE_MAX                            ///< source type unknown
} MUX_SET_CODEC_TYPE_T;


/* for set codec to muxer */
typedef struct MUX_SET_CODEC_INFO_T
{
    MUX_SET_CODEC_TYPE_T    eCodecType;
    VID_ENC_FMT_TYPE_T      eVenc;
    AUD_ENC_FMT_TYPE_T      eAenc;
} MUX_SET_CODEC_INFO_T;

#endif /* _X_MUX_H_ */



