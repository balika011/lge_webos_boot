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
 * $Date: 2015/04/01 $
 * $RCSfile: vdp_if.h,v $
 * $Revision: #15 $
 *
 *---------------------------------------------------------------------------*/

/** @file vdp_if.h
 *  Video plane interface, including init, enable,
 *  mode selection and region desciption.
 */

#ifndef B2R_IF_H
#define B2R_IF_H

#include "x_typedef.h"
#include "stc_drvif.h"
#include "x_timer.h"
#include "drv_common.h"
#include "drv_dbase.h"
#ifdef ENABLE_MULTIMEDIA
#include "swdmx_drvif.h"
#endif

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------

// Trick Play at 20081001
#define VDP_PLAY_NORMAL 0 ///< Normal Play
#define VDP_PLAY_TRICK 1 ///< Trick Play
#define VDP_PLAY_STEP_FORWARD 2 ///< Step Forward

#define VDP_MAX_LOCK_BUF_NS         5


// B2R change frame message flags
#define VDP_B2R_CHG_FRAME_MSG_SYNC  (1 << 0)    // Synchronous display
#define VDP_B2R_MULTI_VIEW          (1 << 1)    // MVC or two frame buffer mode 

#define B2R_CHECK_CB_FUNC_VERIFY(func, crc)     ((UINT32)func == ~((UINT32)crc))
#ifdef CC_SUPPORT_PIPELINE

#define B2R_NS                      4
EXTERN BOOL fgLGPipLine;
EXTERN BOOL fgVdpModeChg[B2R_NS];

/*
EXTERN UCHAR  LG_PipLineConnect(UCHAR ucVdpId, UCHAR ucB2rId);
EXTERN void  LG_PipLineDisconnect(UCHAR ucVdpId);
EXTERN void  LG_PipLineTest(UCHAR ucVdpId,UCHAR ucEsId);
EXTERN void  LG_PipLineVdpConnect(UCHAR ucVdpId,UCHAR ucEsId);
EXTERN void LG_pipLineScartConnect(UCHAR ucVdpId,UCHAR ucEsId);
*/

EXTERN BOOL VDP_PipeModeChangeing(UCHAR ucVdpId,UCHAR ucB2rId);
EXTERN VOID VDP_PipeModeChangeDone(UCHAR ucVdpId,UCHAR ucB2rId);
EXTERN BOOL VDP_PipeIsConnected(UCHAR ucVdpId,UCHAR ucEsId);
EXTERN VOID VDP_PipeConnectFromVdp(UCHAR ucVdpId,UCHAR ucEsId);
EXTERN VOID VDP_PipeConnectFromVdec(UCHAR ucEsId,UCHAR ucFbgId);
EXTERN VOID VDP_PipeGetInifor(VOID);
EXTERN VOID VDP_PipeStartSeqChange(UCHAR ucFbgId);
EXTERN BOOL VDP_PipeIsInputReady(UCHAR ucVdpId);
EXTERN BOOL VDP_PipeIsOutputReady(UCHAR ucEsId);

#endif

EXTERN VOID B2R_SetVsyncNum(UCHAR uVNum);

typedef enum
{
    VDP_B2R_CHG_FRAME_DISPQ = 0,        // Get display frame address from FBG display Q 
    VDP_B2R_CHG_FRAME_DIRECT,           // The display frame address is given directly (give only 1 address, the YC data are continuous)
    VDP_B2R_CHG_FRAME_DIRECTYC          // The display frame address is given directly (give both Y and C addressare)
} VDP_B2R_CHG_FRAME_DISPMODE;

typedef struct
{
    UINT32 u4VdpId;
    UINT32 u4Flag;
    UINT32 u4DispMode;
    UCHAR  ucFbgId;
    UCHAR  ucFbId;
	UINT8  u4FrameNum;
} VDP_B2R_CHG_FRAME_MSG_T;


// B2R Video Image Module Color Mode.
#define VDP_IMAGE_420_MODE          (1 << 0)    ///< 420 mode
#define VDP_IMAGE_422_MODE          (1 << 1)    ///< 422 mode
#define VDP_IMAGE_444_MODE          (1 << 2)    ///< 424 mode

/**
 *  B2R Video plane image module, Capability infomation
 */
typedef struct
{
    UINT32 u4Width;             ///< width of maximum supported image width
    UINT32 u4Height;            ///< height of maximum supported image height
    UINT32 u4ColorMode;         ///< supported color mode
    UINT32 u4FrameBufferNs;     ///< supported number for Frame Buffer
    UINT32 u4PitchAlignement;   ///< supported alignement step for Pitch
} VDP_IMAGE_INFO_T;

/**
 *  Video plane image module, Image Argument
 */
typedef struct
{
    UINT32 u4FrameBufferId;     ///< frame buffer id of image
    UINT32 u4Width;             ///< width of imge
    UINT32 u4Height;            ///< height of image
    UINT32 u4ColorMode;         ///< color mode of image
    UINT32 u4Pitch;             ///< Pitch of image
} VDP_IMAGE_ARG_T;

/*  B2R picture information for MM*/
typedef struct
{
    UCHAR ucHours;
    UCHAR ucMinutes;
    UCHAR ucSeconds;
    UCHAR ucPictures;
    UINT32 u4Pts;
    UINT32 u4TotlaTimeOffset;
    UINT32 u4TickNum;
    UINT64 u8Offset;     // file offset
    UINT64 u8OffsetDisp; // file offset for display

} VDP_PIC_INFO_T;

/*  B2R picture information for MM*/
typedef struct
{
    UINT32 u4TickNum;

} VDP_SET_PIC_INFO_T;


/**
 *  VDP capture picture information
 */
typedef struct
{
    BOOL    fgUFOOn;
    UINT32 u4UFOType;
    UINT32 u4AddrY;
    UINT32 u4AddrC;
    UINT32 u4AddrYLen;
    UINT32 u4AddrCLen;
    UINT32 u4DstY;
    UINT32 u4DstC;
    UINT32 u4MemHSize;
    UINT32 u4MemVSize;
    UINT32 u4HSize;
    UINT32 u4VSize;
    UINT32 u4RRMode;
    UINT32 u4IsRsIn;
    UINT32 u4SrcSwap;
} VDP_CAPTURE_INTO_T;

/**
 *  VDP position information
 */
typedef struct
{
    UINT64 u8PtsI;
    UINT64 u8Pts;

    UINT64 u8Stc;

    UINT64 u8AudPts;
    UINT64 u8AudOffset;

    UINT64 u8OffsetI;
    UINT64 u8Offset;
    UINT64 u8OffsetDisp;
    INT32  i4TemporalRef;
    UINT32 u4Timestap;
    UINT16 u2DecodingOrder;
} VDP_POS_INTO_T;

/**
 *  VDP AB position information
 */
typedef struct
{
    BOOL fgValid;
    BOOL fgPosition; // use position or pts.
    VDP_POS_INTO_T rA;
    VDP_POS_INTO_T rB;
} VDP_AB_INTO_T;

// This structure must be same with FBM_PIC_EXTRA_INFO_T
/**
 *  VDP PIC EXTRA INFO
 */
typedef struct
{
    BOOL fgValid;

    UINT64 u8PTSI;                                  //  I frame pts
    UINT64 u8AudPTS;
    UINT64 u8AudOffset;
} VDP_PIC_EXTRA_INTO_T;

typedef struct
{
    BOOL fgValid;
    UINT32 u4BufAddr;
} VDP_LOCK_PIC_INFO_T;

/**
 *  Seamless display info
 */
 typedef struct
{
    UINT32 u4SrcWidth;
    UINT32 u4SrcHeight;
} VDP_SEAMLESS_INFO_T;
typedef VOID (* PFN_TRIG_AUD_ARRIVE_CB)(VOID *pvTag);

typedef struct _VDP_TRIG_AUD_ARRIVE_CB_T
{
    PFN_TRIG_AUD_ARRIVE_CB  pfnTrigAud;
    VOID                    *pvTag;
} VDP_TRIG_AUD_ARRIVE_CB_T;

/**
 *  VDP Seek mode structure 
 */
typedef struct _VDP_SEEK_PRM_T
{
    UCHAR ucSeekMode;
    UINT32 u4SeekPts;
    UCHAR ucNotifyTrigger;
    UCHAR ucABNotifyTrigger;
    UINT8 u1TimeoutCnt;
    BOOL fgABReach;
} VDP_SEEK_PRM_T;

typedef struct _VDP_RT_STC_ADJ_T
{
    INT32       i4_vid_diff;
    INT32       i4_aud_diff;
    INT32       i4_ave_diff;
    UINT32      u4_adj_cnt;
} VDP_RT_STC_ADJ_T;

typedef struct _VDP_WFD_PRM_T
{
    BOOL        fgInitalized;
    BOOL        fgDblClk;
    BOOL        fgDnmcltncy;
    HAL_TIME_T  t_start_time;
    UINT32      u4_pkt_cnt;
    UINT32      u4_ltncy_ns;
    UINT32      u4_ltncy_max;
    VDP_RT_STC_ADJ_T    t_stc_adj;
} VDP_WFD_PRM_T;

typedef struct _B2R_VDP_4K60_T
{
    BOOL   fgEnable;
    UINT32 u4StartPTS;
    UINT32 u4Rate;
    UINT32 u4Scale;
} B2R_VDP_4K60_T;

typedef struct _B2R_RT_CLI_T
{
    BOOL   b_active;
    BOOL   b_dynamic_latency;
    UINT32 u4_latency_ns;
    UINT32 u4TimeCur;
    UINT32 u4TimeAve;
    UINT32 u4FrameCnt;
    UINT64 u8TimeTotal;
} B2R_RT_CLI_T;

/**
 *  VDP trigger mode + audio master to do trigger mode 
 */
typedef struct _VDP_TRIG_AUD_PRM_T
{
    UCHAR ucTriggerReceived;
    UINT32 u4TriggerPts;
    VDP_TRIG_AUD_ARRIVE_CB_T rTrgAudCbInfo;
} VDP_TRIG_AUD_PRM_T;

typedef struct _VDP_TRIG_AUD_INFO_T
{
    BOOL    b_aud_auto_en;
    UINT32  u4AdecId;
} VDP_TRIG_AUD_INFO_T;

typedef struct _VDP_TRIG_AUD_CFG_T
{
    BOOL    fgTrigEnable;
    UINT32  u4Pts;
    VDP_TRIG_AUD_INFO_T    rAudInfo;
    VOID*                  prTrgAudCbInfo;
} VDP_TRIG_AUD_CFG_T;

typedef struct _B2R_CB_COMM_DATA_T
{
    VOID*   pv_tag;
} B2R_CB_COMM_DATA_T;

typedef struct _B2R_CB_INFO_T
{
union
    {
        B2R_CB_COMM_DATA_T t_comm;
    }u;
} B2R_CB_INFO_T;

typedef struct _B2R_REG_CB_OBJ_T
{
    VOID*          pfCb;
    B2R_CB_INFO_T  rCbInfo;
} B2R_REG_CB_OBJ_T;

#ifdef CC_MAPLE_CUST_DRV
typedef struct _B2R_CUST_INFO_T
{
    UINT32                  u4resolutionChg;
    ENUM_CODEC_INFO_T       eCodecInfo;
    FBM_COLOR_SPACE_ID      eColorID;
    SWDMX_SOURCE_TYPE_T     eMMSrcType;
    FBM_CODEC_RESOLUTION_T  rResolution;
} B2R_CUST_INFO_T;
#endif

/**
 *  B2R using only one frame to change picture
 */
typedef struct _B2R_ONE_FB_CHANGE_T
{
    BOOL fgTrigger;
    BOOL fgReusing;
    UINT32 u4Count;
} B2R_ONE_FB_CHANGE_T;

/** Brief of B2R_PRM_T
 */
typedef struct
{
    UCHAR ucMode;

    // Last Field
    UCHAR ucBottom;
    UCHAR ucRightView;
    // Common
    //UCHAR ucVdpId;
    UCHAR ucB2rId;
    UCHAR ucFbgId;
    UCHAR ucReady;              // if Ready, forceBg = 0;
    UCHAR ucNotSupport;
    UCHAR ucFastChangeEn;

    // Frame Buffer
    UCHAR ucFbId;
    UCHAR ucLastFbId;           // check temporal reference
    UCHAR ucReleaseFbId;
    UCHAR ucPendingFbId;        // Store FB which is pending for display at start
    UCHAR ucReleased;

    UCHAR ucPendingNewFb;       // Pending for New Frame Buffer, FBM (MPEG) can put new frame buffer to VDP, instread of polling by VDP thread
    UCHAR ucNewFbArrive;        // New Frame Buffer Arrive
    UCHAR ucMMWaitSTC;          // MM mode, wait STC while PTS is larger than STC
    UCHAR ucRealChangeFb;       // MM mode, Read change FB, not change to pending FB
#ifdef CC_B2R_SUPPORT_GAME_MODE
    BOOL fgGameModeEnable;

    UCHAR ucGameModeFrameChange;  
#endif
#ifdef CC_3D_MM_DS_SUPPORT
    UCHAR ucSubFbgId;
    UCHAR ucSubFbId;
    UCHAR ucSubReleaseFbId;
    UCHAR ucSubPendingFbId;
    UCHAR ucSubFbgIdRecorded;
    UCHAR ucSubReleased;
#endif
    BOOL fgSeqChg;
    // Output
    UCHAR ucOutputMode;
    UCHAR ucOutFrameRate;
    UCHAR ucProgressiveOut;

    // I 2 I
    UCHAR ucInterlace2Interlace;
    UCHAR ucWrongField;

    // for 420 chroma mode
    UCHAR ucTff;
    UCHAR ucRff;
    UCHAR ucLastTff;
    UCHAR ucLastRff;
    UCHAR ucProgressiveFrame;
    UCHAR ucLastProgressiveFrame;
    UCHAR ucPicCdTp;
    UCHAR ucHwFrameEncode;
    UCHAR ucTffStableCounter;
    
    // Frc
    UCHAR ucMultipleField;      // multiple field display due to RFF/TFF
    UCHAR ucStopAuto;

    // Display Ns
    UINT16 u2TargetNs;
    UCHAR u2ChangeFieldNs;
    UCHAR u2TargerVsyncNs;
    UCHAR u2ChangeFieldVsyncNs;
    UCHAR u2CurrentVsyncNs;
    UCHAR u2DisplayFailNs;
    UINT16 u4FrameCount;// Total display Num
    // 32 flag source
    UCHAR ucValid32Seq;
    UCHAR ucPd32SeqStep;
    UCHAR ucRelease32Cnt;

    // Mpeg2 Sequence/Picture Header
    UCHAR ucEarlyNotify;
    UCHAR ucSeqInfoValid;
    UCHAR ucProgressiveSeqFlag;
    UCHAR ucLastAcsFrame;
    UCHAR ucLastAcsAuto;
    UCHAR ucInFrameRate;
    INT32 i4TemporalRef;
    UINT16 u2DecodingOrder;

    // Frame Rate Conversion
    UINT32 u4InStcPeriodBase;
    UINT32 u4InStcPeriod;
    UINT32 u4OutStcPeriod;
    UINT32 u4OutFrameStc;
    UINT32 u4InFrameStc;

    // AV Sync
    UINT32 u4Pts;
    UINT32 u4TotlaTimeOffset;
    UINT32 u4PtsAsyncNs;
    UINT32 u4PtsAvSyncNotAllow;
    INT32 i4PtsDrift;
    UINT32 u4KeepDropB;         // Too much slower than Headend
    UINT32 u4WaitUnmute;
    UINT32 u4B2rSkipFrameNo;

    // Partial PES Handle
    UINT32 u4LastPts;
    UINT32 u4PtsDelta;       // PTS delta (IIR) between two non-zero PTS
    UINT32 u4ZeroPtsNs;	     // handle Zero PTS & PTS Jitter Filter

    // Frame Success Counter
    //UINT32 u4FrameSuccess;

    // Frame Repeated Counter
    //UINT32 u4FrameRepeated;

    //File position
    UINT32 u4TickNum;
    UINT64 u8FilePosition;
    UINT64 u8FilePositionI; // closed I frame pos

    //for AB repeat
    VDP_PIC_EXTRA_INTO_T rExtra;

    BOOL fgDummyFrame;
    BOOL fgProgress2Interlace;

    // Last Sync Time
    HAL_TIME_T rLastAvSyncTime;

    PARAM_VDP_FB_COUNTER_T* prFbCounter;


    //--todo refine it later
    //other info
    BOOL fgReleaseDispQ;

    //for fastforward info
    BOOL fgFastForward;

    //b2r 3d info
    BOOL fgB2R3DError;
    UINT32 u4B2R3DSuccessCount;
    UINT32 u4B2R3DErrorCount;
    BOOL fgB2R3DEnable;
    BOOL fgB2RForce2D;
    BOOL fgB2RNoLock;
    UINT8 u1B2R3DType;
    UINT8 u1B2RForce2DType;
    UINT32 u4B2R3DWidthOffset;
    UINT32 u4B2R3DHeightOffset;
    BOOL fgDispFail;
    UINT8 u1B2R3DTrickModeType;
    // for multiple instance
    UINT32 u4AttachedSrcId;
 #ifdef CC_MAPLE_CUST_DRV
    B2R_CUST_INFO_T*    pt_cust_info;
 #endif
 
    UCHAR ucSeamlessVDPMode; 

    // Need restore NS after frame repeat
    UCHAR u2RestoreNs;
    VDP_WFD_PRM_T   t_wfd_prm;
    B2R_RT_CLI_T*   ptRtCli;

    B2R_ONE_FB_CHANGE_T rOneFbChg;
    BOOL fgRenderRegionChg;
    VDP_REGION_T rRenderRegion;
    B2R_VDP_4K60_T  t_b2r_vdp_uhd;
} B2R_PRM_T;

/**
 *  B2R call back function type id.
 */
typedef enum
{
	B2R_CB_FUNC_TRIG_AUD_TO_PLAY,
    B2R_CB_FUNC_EOS,
    B2R_CB_FUNC_NS
} B2R_CB_FUNC_ENUM_T;

/**
 * B2R call back function pool
 */
typedef struct _B2R_CB_FUNC_T
{
    UINT32 au4CbFunc[B2R_CB_FUNC_NS];
    UINT32 au4CbFuncCRC[B2R_CB_FUNC_NS];
    B2R_REG_CB_OBJ_T  pu4CbObj[B2R_CB_FUNC_NS];
} B2R_CB_FUNC_T;

typedef enum
{
    VDP_SMLS_MODE_NONE,   
    VDP_SMLS_PREPARE,     // Set vVRMSetEventFlg(path, VRM_EVENT_BY_B2R) for NPTV to prepare new w/h cfg.
    VDP_SMLS_READY,       // Check NPTV ready or not by bVRMReadyForB2R(ucVdpId).
    VDP_SMLS_FRAME_COMING,  // Resolution change frames coming, current must be ready status.
    VDP_SMLS_PREPARE_WHILE_START_PLAY,  //Start play with NPTV seamless, prepare
    VDP_SMLS_READY_WHILE_START_PLAY,    //Start play with NPTV seamless, ready
}SEAMLESS_MODE_E;


typedef enum _B2R_RT_TYPE_T
{
    B2R_RT_TYPE_CLI_ACTIVE,
    B2R_RT_TYPE_LARENCY_NS,
    B2R_RT_TYPE_LARENCY_QY
}B2R_RT_TYPE_T;

typedef struct _B2R_CFG_SRC_REG_T
{
    UINT32  u4SrcLineSize;
    UINT32  u4SrcWidth;
    UINT32  u4SrcHeight;
}B2R_CFG_SRC_REG_T;

typedef struct _B2R_OUT_INFO_T
{
    UCHAR ucPmxId;
    UCHAR ucPmxMode;
    UCHAR ucPrg;
    UCHAR ucFrameRate;
    UCHAR ucDoubleClk;
    UCHAR ucInputFrameRate;     // For Clock Configuration
    UINT32 u4Width;
    UINT32 u4Height;
    UINT32 u4TotalWidth;
    UINT32 u4TotalHeight;
    UINT32 u4ClkMode;
} B2R_OUT_INFO_T;

typedef struct _B2R_HW_CHG_T
{
    BOOL b_HwChg;
    UCHAR ucChgB2rId;
} B2R_HW_CHG_T;

typedef struct _VDP_CFG_T
{
    UCHAR ucVdpEnable;
    UCHAR ucB2rId;
    UCHAR ucEnable;
    UCHAR ucStatus;
    UCHAR ucVdpId;
    UCHAR ucMode;
    
    //   UCHAR ucDispMode;
    UCHAR ucBobMode;
    // Input port (frame buffer group ID)
    UCHAR ucInputPort[VDP_MAX_INPORT_NS];
	BOOL   fgLGPipLine;
    //UCHAR ucPauseMm;
    UCHAR ucNoOverScan;
    UCHAR ucEarlyDisp;
    BOOL fgMMPlayback;//DTV or mmp
    UCHAR ucColorDescrip;
    UCHAR ucColourPrimary;
    UCHAR ucTransCharacter;
    UCHAR ucMatrixCoeff;

    // source information
    UINT32 u4SrcLineSize;
    UINT32 u4SrcWidth;
    UINT32 u4SrcHeight;
    UINT32 u4AspectRatio;
    UINT32 u4FrameRate;
    UINT32 u4QpAvg;
    UINT32 u4Afd;

    B2R_OUT_INFO_T rOutInfo;
    
    UINT32 u4BgColor; // YCbCr
    UINT32 u4MpegWidth;
    UINT32 u4MpegHeight;

    UINT32 u4FrmCropTop;
    UINT32 u4FrmCropBtm;

    HANDLE_T hMutex;
    BOOL   fgBypssEnabe;
    BOOL   fgCropping;
    UINT32 u4CropX;
    UINT32 u4CropY;
    UINT32 u4CropWidth;
    UINT32 u4CropHeight;

    UINT32 u4AspectRatioWidth;
    UINT32 u4AspectRatioHeight;
    UINT32 u4AspectRatioIdc;

    BOOL fgScartOut; // Scart out enable
    UINT32 u4LogFilter;
    B2R_HW_CHG_T t_HwChg;
}VDP_CFG_T;

typedef struct _B2R_STATUS_T
{
    UCHAR ucB2rId;
    UCHAR ucEnable;
    UCHAR ucStatus;
     BOOL fgMMPlayback;//DTV or mmp
} B2R_STATUS_T;

typedef struct _B2R_ASP_INFO_T
{
    UINT32 u4Afd;
    UINT32 u4AspectRatio;
    UINT32 u4AspectRatioIdc;
    UINT32 u4AspectRatioWidth;
    UINT32 u4AspectRatioHeight;
} B2R_ASP_INFO_T;

#define B2R_SPEED_TYPE_FORWARD_1X            (1 << 10)
#define B2R_SPEED_TYPE_FORWARD_2X            (2 << 10)
#define B2R_SPEED_TYPE_FORWARD_3X            (3 << 10) // FixMe
#define B2R_SPEED_TYPE_FORWARD_4X            (4 << 10)
#define B2R_SPEED_TYPE_FORWARD_8X            (8 << 10)
#define B2R_SPEED_TYPE_FORWARD_16X           (16 << 10)
#define B2R_SPEED_TYPE_FORWARD_32X           (32 << 10)
#define B2R_SPEED_TYPE_FORWARD_64X           (64 << 10)
#define B2R_SPEED_TYPE_FORWARD_128X          (128 << 10)
#define B2R_SPEED_TYPE_FORWARD_256X          (256 << 10)
#define B2R_SPEED_TYPE_FORWARD_512X          (512 << 10)
#define B2R_SPEED_TYPE_FORWARD_1024X         (1024 << 10)
#define B2R_SPEED_TYPE_FORWARD_1_DIV_2X      (1 << 9)
#define B2R_SPEED_TYPE_FORWARD_1_DIV_3X      (1 << 9) // FixMe
#define B2R_SPEED_TYPE_FORWARD_1_DIV_4X      (1 << 8)
#define B2R_SPEED_TYPE_FORWARD_1_DIV_8X      (1 << 7)
#define B2R_SPEED_TYPE_FORWARD_1_DIV_16X     (1 << 6)
#define B2R_SPEED_TYPE_FORWARD_1_DIV_32X     (1 << 5)
#define B2R_SPEED_TYPE_FORWARD_0_DOT_6X      (6 << 10)   /* Slow Forward 0.6X */
#define B2R_SPEED_TYPE_FORWARD_0_DOT_7X      (7<< 10)    /* Slow Forward 0.7X */
#define B2R_SPEED_TYPE_FORWARD_0_DOT_8X      (8 << 10)   /* Slow Forward 0.8X */
#define B2R_SPEED_TYPE_FORWARD_0_DOT_9X      (9 << 10)   /* Slow Forward 0.9X */
#define B2R_SPEED_TYPE_FORWARD_1_DOT_1X      (11 << 10)  /* Fast Forward 1.1X */
#define B2R_SPEED_TYPE_FORWARD_1_DOT_2X      (12 << 10)  /* Fast Forward 1.2X */
#define B2R_SPEED_TYPE_FORWARD_1_DOT_3X      (13 << 10)  /* Fast Forward 1.3X */
#define B2R_SPEED_TYPE_FORWARD_1_DOT_4X      (14 << 10)  /* Fast Forward 1.4X */
#define B2R_SPEED_TYPE_FORWARD_1_DOT_5X      (15 << 10)  /* Fast Forward 1.5X */
#define B2R_SPEED_TYPE_REWIND_1X
#define B2R_SPEED_TYPE_REWIND_2X
#define B2R_SPEED_TYPE_REWIND_3X
#define B2R_SPEED_TYPE_REWIND_4X
#define B2R_SPEED_TYPE_REWIND_8X
#define B2R_SPEED_TYPE_REWIND_16X
#define B2R_SPEED_TYPE_REWIND_32X
#define B2R_SPEED_TYPE_REWIND_64X
#define B2R_SPEED_TYPE_REWIND_128X
#define B2R_SPEED_TYPE_REWIND_256X
#define B2R_SPEED_TYPE_REWIND_512X
#define B2R_SPEED_TYPE_REWIND_1024X


//return value
#define B2R_OK                     (0)
#define B2R_FAIL                   (1)
#define B2R_INV_ARG                (2)
#define B2R_INTERNAL_ERR           (3)
#define B2R_NOT_IMPL               (4)

typedef enum _B2R_SET_TYPE_CFG_T
{
    B2R_CFG_MODE,
    B2R_CFG_VDP_ENABLE
}B2R_SET_TYPE_CFG_T;

typedef enum _B2R_GET_TYPE_T
{
    B2R_GET_TYPE_B2R_STATUS,
    B2R_GET_TYPE_ASP_INFO
}B2R_GET_TYPE_T;

typedef enum _B2R_GET_CUST_TYPE_T
{
    B2R_GET_TYPE_CUST_INFO,
    B2R_GET_TYPE_ASPINFO
}B2R_GET_CUST_TYPE_T;
#ifdef CC_B2R_SUPPORT_GAME_MODE
typedef enum
{
 B2R_GAME_MODE_OFF,
 B2R_GAME_MODE_LOW_DELAY,
 B2R_GAME_MODE_HIGHT_QUALITY
}B2R_GAME_MODE_TYPE_T;
EXTERN void VdpSetGameModeOnOff(UCHAR ucVdpId,B2R_GAME_MODE_TYPE_T eModeType);

#endif
///-----------------------------------------------------------------------------
/// Public functions
///-----------------------------------------------------------------------------

/**
 * VDP connect to video decoder.
 *
 * @param ucVdpId specify the video plane id.
 * @param ucEsId specify video decoder id.
 * @param ucPort always 0.
 * @return VDP_SET_ERROR or VDP_SET_OK
 */
 #ifdef CC_B2R_RM_SUPPORT
EXTERN void b2r_resource_release(UCHAR ucB2rId);
EXTERN UCHAR B2R_Source_Connect(B2R_RM_T input_Src);
EXTERN void VDP_SetVideSrc(UCHAR ucSrc, UCHAR ucVdpId);
EXTERN void VDP_SetB2RId( UCHAR ucB2RId,UCHAR ucVdpId, UCHAR ucVdecEsId);
#endif

EXTERN VOID B2R_VB1_Setting(VOID);
EXTERN VOID B2R_VB1_ENABLE_CLI( BOOL fgEnable);

EXTERN UINT32 VDP_Vdp2B2rId(UCHAR ucVdpId);
EXTERN UINT32 VDP_SetInput(UCHAR ucVdpId, UCHAR ucEsId, UCHAR ucPort);

/**
 * VDP query video plane input connection.
 *
 * @param ucVdpId specify the video plane id.
 * @param ucPort always 0.
 * @param pucFbgId video decoder id.
 * @return VDP_SET_ERROR or VDP_SET_OK
 */
EXTERN UINT32 VDP_GetLockFrameBufferAddr(UCHAR ucVdpId, VDP_CAPTURE_INTO_T* prCapInfo);

EXTERN UINT32 VDP_GetDisplayFrameBufferAddr(UCHAR ucVdpId, VDP_CAPTURE_INTO_T* prCapInfo);

EXTERN UINT32 VDP_SetSpeed(UCHAR ucVdpId, STC_SPEED_TYPE_T eSpeed);

EXTERN UINT32 VDP_GetPts(UCHAR ucVdpId);

EXTERN UINT32 VDP_SetStepForward(UINT8 ucVdpId, UCHAR ucSet);

EXTERN UCHAR VDP_GetStepForward(UINT8 ucVdpId);

EXTERN UINT32 VDP_SetSlowForward(UINT8 ucVdpId, UCHAR ucSet);

EXTERN UCHAR VDP_GetSlowForward(UINT8 ucVdpId);

EXTERN UINT32 VDP_SetFBNoTimeout(UINT8 ucVdpId, UCHAR ucSet);

EXTERN UINT32 VDP_GetFrameRate(UCHAR ucVdpId);

EXTERN UINT32 VDP_TrickModeSkipFrameNo(UCHAR ucVdpId);

EXTERN UINT32 VDP_SetTrickMode(UINT8 ucVdpId,UCHAR ucTrick);

EXTERN VOID VDP_SetSeek(UCHAR ucVdpId, UCHAR ucSet, UINT32 u4Pts);
EXTERN void VDP_SetSeekMode(UCHAR ucVdpId, UCHAR ucSet, UINT32 u4Pts);

EXTERN VOID VDP_SetABRepeat(UCHAR ucVdpId, UCHAR ucSet);

EXTERN UCHAR VDP_SetThumbnailMode(UCHAR ucVdpId, UCHAR ucSet);

EXTERN VOID VDP_SetCbAttachedSrcId(UCHAR ucVdpId, UINT32 u4AttachId);

EXTERN BOOL B2R_CkgenOnOff(BOOL fgEnable);

EXTERN BOOL VDP_IsStartToPlay(UCHAR ucVdpId);

EXTERN BOOL VDP_GetB2R2DSupport(UINT32 u4Width,UINT32 u4Height);

EXTERN UCHAR VDP_GetSeamlessInfo(UCHAR ucVdpId, VDP_SEAMLESS_INFO_T* prSeamlessInfo);

EXTERN VOID VDP_NfySetResolutionReady(UCHAR ucVdpId, BOOL fgReady);

EXTERN UCHAR VDP_FrcGetLockFrameBuffer(UCHAR ucVdpId);

EXTERN VOID VDP_SetTimeShiftSpeed(UINT8 u1B2rId,UINT32 u4SpeedValue);

EXTERN VOID VDP_SetTimeShiftMode(UINT8 u1B2rId,BOOL fgEnable);

EXTERN UINT32 VDP_SetB2rNoLock(UINT8 ucVdpId, BOOL fgNoLock);

EXTERN UCHAR VDP_GetFbg(UCHAR ucB2rId);

EXTERN BOOL VDP_GstEnablGstLog(UCHAR ucVdpId);

EXTERN VOID VDP_SetEnableLog(UCHAR ucVdpId, UINT32 u4Arg1, UINT32 u4Arg2);

EXTERN UINT32 VDP_SetPauseMm(UCHAR ucVdpId, UCHAR ucPause);

EXTERN VOID VDP_SetDropFrame(UCHAR ucVdpId);

EXTERN VOID VDP_SetRepeatFrame(UCHAR ucVdpId);

EXTERN VOID VDP_FrcSetFreeze(UCHAR ucVdpId, UINT32 u4OnOff);

EXTERN VOID VDP_Pattern(UCHAR ucVdpId, UINT32 u4Arg1, UINT32 u4Arg2, UINT32 u4Arg3);

EXTERN UCHAR VDP_GetOutFrameRate(UCHAR ucVdpId);

EXTERN UINT32 VDP_Es2Vdp(UCHAR ucEsId);

EXTERN VOID VDP_B2rSwitch(UCHAR ucVdpId, UCHAR ucB2rId);


/* VDP trigger mode + audio master to do trigger mode */
EXTERN VOID VDP_TriggerAudReceive(UINT8 ucVdpId, UINT32 u4Pts);

/** Disable VDP Trigger action */
EXTERN BOOL VDP_TriggerAudReceiveEx(UINT8 ucVdpId, BOOL fgEnable, UINT32 u4Pts);

EXTERN UCHAR VDP_GetPlayMode(UCHAR ucVdpId);

EXTERN VOID VDP_SetB2RMirror(UCHAR ucVdpId, BOOL fgOn);

EXTERN VOID VDP_FrcResetPort(UCHAR ucVdpId, UCHAR ucPort);

/**
 * Get mute status of DTV playback
 *
 * @param ucVdpId specify the video plane id.
 * @return TRUE for mute, FALSE for not mute
 */
EXTERN BOOL VDP_GetUnMuteStatus(UCHAR ucVdpId);
//EXTERN BOOL VDP_Is4To1024XSpeed(UINT32 u4Speed);
EXTERN VOID VDP_SetB2rStc(UINT8 ucVdpId,UINT32 u4B2rStc);
EXTERN UINT32 VDP_GetB2rStc(UINT8 ucVdpId);

EXTERN UINT32 VDP_SetFrmCrop(UCHAR ucVdpId, UINT32 u4FrmCropTop, UINT32 u4FrmCropBtm);


EXTERN UCHAR VDP_GetPendingFB(UCHAR ucVdpId, UCHAR ucFbgId);
EXTERN VOID VDP_CleanPendingFB(UCHAR ucVdpId, UCHAR ucFbgId);
EXTERN VOID VDP_SetReleaseDispQ(UCHAR ucVdpId, UCHAR ucFbgId);

EXTERN VOID VDP_GetYCStart(UCHAR ucVdpId,UINT32 *pu4YStart, UINT32 *pu4CStart, UINT32 *pu4LineSize, UINT32 *pu4VActive);
EXTERN VOID VDP_HalGetB2rDispInfo(UCHAR ucVdpId,B2R_DISP_INFO_T* ptB2rInfo);

EXTERN VOID VDP_VsyncMutexLock(UCHAR ucVdpId);

EXTERN VOID VDP_VsyncMutexUnlock(UCHAR ucVdpId);

EXTERN VOID VDP_SetChkPTS(UINT32 u4Pts);
EXTERN VOID VDP_SetEncTrigger(BOOL fgEnable);

EXTERN BOOL VDP_GetFilePosition(UCHAR ucVdpId,
                                INT32 *pi4TempRef, UINT16 *pu2DecodingOrder,
                                UINT64 *pu8Offset, UINT64 *pu8OffsetI);

#define B2R_GETPOSINFO_AVAILABLE // for different version code complier OK.
EXTERN BOOL VDP_GetPosInfo(UCHAR ucVdpId, VDP_POS_INTO_T *prPosInfo);

EXTERN BOOL VDP_SetABInfo(UCHAR ucVdpId, VDP_AB_INTO_T *prAB);

/**
 * VDP query function, show DTV status in CLI with debug level 1.
 *
 * @param VOID
 * @return VOID
 */
EXTERN VOID VDP_PrintDtvInfo(VOID);

EXTERN INT32 VDP_GetPtsStcDelta(UCHAR ucB2rId);
EXTERN BOOL VDP_GetTimeShfitStartPlay(UCHAR ucB2rId);

EXTERN UINT32 VDP_GetCurrentPts(UCHAR ucB2rId);

EXTERN UCHAR VDP_B2r2Vdp(UCHAR ucB2rId);

EXTERN UINT32 VDP_Vdp2Es(UCHAR ucVdpId);

EXTERN UCHAR VDP_Stc2B2r(UCHAR ucStcId);

EXTERN UCHAR B2R_GetEs2Fbg(UCHAR ucEsId);

EXTERN VOID VDP_ResetPts(UCHAR ucVdpId);

EXTERN VOID VDP_FrcSetPtsCb(UINT32 u4CbArg, UINT32 u4CbPts);

EXTERN VOID VDP_FrcSetLipSyncCb(UCHAR ucVdpId, UCHAR ucPort, UINT32 u4Thrsd, UINT32 u4FrmCnt);

EXTERN VOID VDP_FrcSetLipSyncNGOKCb(UCHAR ucNgOk, UCHAR ucRemove, UINT32 u4Thrsd, UINT32 u4FrmCnt);

EXTERN VOID B2R_SetImgConnect(UCHAR ucVdpId, UCHAR ucConnect);

EXTERN UCHAR B2R_GetImgConnect(UCHAR ucVdpId);
EXTERN void B2R_HalEnableSmartPcr(UCHAR ucB2rId, BOOL fgEn);


/**
 * Video Image Module : Get information
 *
 * @param VOID
 * @return pointer of Video Image Module Capability Infomation
 */
EXTERN VDP_IMAGE_INFO_T* VDP_Image_GetInfo(VOID);

/**
 * Video Image Module : Connect
 *
 * @param u4VdpId: Video plane connected to
 * @return VDP_SET_OK or VDP_SET_ERROR
 */
EXTERN UINT32 VDP_Image_Connect(UINT32 u4VdpId);

/**
 * Video Image Module : Disconnect
 *
 * @param VOID
 * @return VDP_SET_OK or VDP_SET_ERROR
 */
EXTERN UINT32 VDP_Image_Disconnect(VOID);

/**
 * Video Image Module : Mute on/off
 *
 * @param u4OnOff: Mute on/off
 * @return VDP_SET_OK or VDP_SET_ERROR
 */
EXTERN UINT32 VDP_Image_Mute(UINT32 u4OnOff);

/**
 * Video Image Module : Show Frame Buffer
 *
 * @param prInfo: Frame Buffer Info
 * @return VDP_SET_OK or VDP_SET_ERROR
 */
EXTERN UINT32 VDP_Image_Show(VDP_IMAGE_ARG_T* prInfo);

/**
 * Video Image Module : Get Frame Buffer Address
 *
 * @param u4FrameBufferId: Frame Buffer id
 * @param u4YAddr: pointer to store Y address
 * @param u4CAddr: pointer to store C address
 * @return VDP_SET_OK or VDP_SET_ERROR
 */
EXTERN UINT32 VDP_Image_GetFrameBuffer(UINT32 u4FrameBufferId, UINT32* u4YAddr, UINT32* u4CAddr);


EXTERN UINT32 VDP_GetPicInfo(UCHAR ucVdpId, VDP_PIC_INFO_T* prPicInfo);

EXTERN UINT32 VDP_SetPicInfo(UCHAR ucVdpId, VDP_SET_PIC_INFO_T rPicInfo);

EXTERN VOID VDP_SetB2rEnable(UCHAR ucVdpId);

EXTERN VOID _VdpCheckFbgReady(UCHAR ucFbgId, UCHAR ucEsId);


EXTERN VOID VDP_CheckFbgReady(VOID);

EXTERN void VDP_InputChange(UCHAR ucB2rId);
/* whether Update STC when VDP start output or not */
EXTERN BOOL VDP_SetPendingForStc(UINT8 ucVdpId, BOOL fgPending);

EXTERN VOID VDP_VsyncSendCmd(UCHAR ucVdpId, UCHAR ucCmdId);

EXTERN VOID VDP_VsyncReset(UCHAR ucVdpId);

EXTERN UINT32 VDP_Set3DInfo(UCHAR ucVdpId,UINT8 u13DType,BOOL fg3DEnable);

EXTERN BOOL VDP_GetSeamlessResizeSize(UCHAR ucVdpId, UINT32 *pu4ResizeWidth, UINT32 *pu4ResizeHeight);

EXTERN UINT32 VDP_GetDispBufLocked(UCHAR ucVdpId, UINT32 u4BufAddr, BOOL *pfgLocked);
#ifdef DIVX_PLUS_CER
#ifdef   CC_B2R_CHAPTER_CHG
EXTERN VOID B2R_SetChapterEnable(VDP_CHAPTER_T * prChapter);
#endif
#endif
EXTERN B2R_PRM_T* VDP_GetVdpPrm(UINT8 u1B2rId);
EXTERN BOOL VDP_SendB2RAysncRenderFrameMsg(VDP_B2R_CHG_FRAME_MSG_T* prMsg);
EXTERN BOOL VDP_FlushB2RAysyncRenderFrameMsg(UCHAR ucFbgId);
EXTERN VOID VDP_DropB2RAysyncRenderFrame(UCHAR ucFbgId);

EXTERN BOOL VDP_SendB2RChgFrameMsg(VDP_B2R_CHG_FRAME_MSG_T *prMsg);

EXTERN BOOL VDP_FlushB2RChgFrameMsg(UCHAR ucVdpId);

EXTERN VOID B2R_Init(VOID);

EXTERN BOOL  VDP_SeamlessSeqChanging(UCHAR ucVdpId);
EXTERN BOOL VDP_SecureVideoPlaying(UCHAR ucVdpId);
#if 1// for 4K2K    vb1
EXTERN UINT16 VDP_GetBypassHTotal(VOID);

EXTERN UINT16 VDP_GetBypassVTotal(VOID);

EXTERN VOID VDP_SetBypassHactive(UINT16 u2HActive);
EXTERN VOID VDP_SetBypassVactive(UINT16 u2VActive);
EXTERN VOID VDP_SetBypassHTotal(UINT16 u2HTotal);
EXTERN VOID VDP_SetBypassVTotal(UINT16 u2VTotal);
EXTERN VOID VDP_SetBypassHFph(UINT16 porch);
EXTERN VOID VDP_SetBypassHBph(UINT16 porch);
EXTERN VOID VDP_SetBypassVFph(UINT16 porch);
EXTERN VOID VDP_SetBypassVBph(UINT16 porch);
EXTERN VOID VDP_SetBypassHVInvert(BOOL HVInvert,BOOL enable);
EXTERN VOID VDP_SetBypassUFOModeEnable(BOOL UFOEnable,BOOL Bittype);
EXTERN VOID VDP_SetBypassHwEnable(BOOL fgEnable);

#endif

#ifdef CC_MAPLE_CUST_DRV
EXTERN INT32 B2R_Get_Cust_info(UCHAR                 ucB2rId,
                               B2R_GET_CUST_TYPE_T   e_set_type,
                               VOID*                 pv_get_info,
                               SIZE_T                z_set_info_size);
#endif
#if defined(CC_MT5890)
UINT16 VDP_GetNB2R_Htotal(VOID);
UINT16 VDP_GetNB2R_Vtotal(VOID);
UINT16 VDP_GetNB2R_HActive(VOID);
UINT16 VDP_GetNB2R_VActive(VOID);

void VDP_SetNB2R_Bypass_H_ACTIVE(UINT16 u2HActive);
void VDP_SetNB2R_Bypass_H_TOTAL(UINT16 u2HTotal);
void VDP_SetNB2R_Bypass_V_ACTIVE(UINT16 u2VActive);
void VDP_SetNB2R_Bypass_V_TOTAL(UINT16 u2VTotal);
void VDP_SetNB2R_Bypass_H_FrontPorch(UINT16 u2Fp);
void VDP_SetNB2R_Bypass_H_BackPorch(UINT16 u2Fp);
void VDP_SetNB2R_Bypass_V_FrontPorch(UINT16 u2Fp);
void VDP_SetNB2R_Bypass_V_BackPorch(UINT16 u2Fp);
void VDP_SetNB2R_Bypass_conf(BOOL HV_SYNC,BOOL enable);
void VDP_SetNB2R_Bypass_enable(BOOL enable,BOOL bittype);

#endif

EXTERN INT32 B2R_RTSetInfo (UCHAR               ucB2rId,
                            B2R_RT_TYPE_T       e_set_type,
                            VOID*               pv_set_info,
                            SIZE_T              z_set_info_size);
EXTERN VDP_CFG_T* B2R_GetVdpConf(UCHAR ucVdpId);

EXTERN INT32 VDP_B2R_GetInfo (UCHAR            ucVdpId,
                            B2R_GET_TYPE_T   e_get_type,
                            VOID*            pv_get_info,
                            SIZE_T           z_get_info_size);

EXTERN INT32 VDP_B2R_SetCfgParam (UCHAR               ucVdpId,
                                         B2R_SET_TYPE_CFG_T   e_set_type,
                                         VOID*                pv_set_info,
                                         SIZE_T               z_set_info_size);

EXTERN INT32 B2R_RegCbFunc(B2R_CB_FUNC_ENUM_T eFuncType, UINT32 u4FuncPtr, UINT32 u4Arg1);

EXTERN INT32 B2R_AudTrigger(UINT8 ucVdpId, VDP_TRIG_AUD_CFG_T* pr_trig_cfg);

EXTERN INT32 B2R_FilterLog(UCHAR ucVdpId, UCHAR ucEnable, UINT32 u4Mask);

EXTERN UCHAR Get_VDP_2_B2R_ID(UINT8 ucVdpId);

#ifdef CC_B2R_RES_SUPPORT
EXTERN INT32 B2R_MainSubStatus(UCHAR* pucMVdpId,
                                       UCHAR* pucSVdpId);
EXTERN BOOL B2R_ChkHwSwitch(UCHAR ucVdpId,
                                     UCHAR ucB2rId,
                                     UCHAR* pucRetId);
#endif
#endif    // B2R_IF_H

