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
 * $RCSfile: b2r_drvif.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file vdp_if.h
 *  Video plane interface, including init, enable,
 *  mode selection and region desciption.
 */

#ifndef B2R_DRVIF_H
#define B2R_DRVIF_H

#include "x_typedef.h"
#include "stc_drvif.h"
#include "fbm_drvif.h"
#include "b2r_if.h"
#include "vdp_if.h"
#include "mute_if.h"
#include "../b2r8099/b2r_hal.h"
//#include "swdmx_drvif.h"

#define _B2R_SetDPEarlyDisp(o,value)      \
    do \
    { \
        UINT32 i; \
        VDP_CFG_T *ptDP; \
        for (i = 0; i < o->u4VideoPanelNs; i++) \
        { \
            ptDP = (VDP_CFG_T *)o->avVideoPanel[i]; \
            if (ptDP) ptDP->ucEarlyDisp = value; \
        } \
    }while (0); 

#define LOG_REL(FLAG, REL_LVL, DBG_LVL, MSG...)     \
    if (FLAG) { LOG(REL_LVL, MSG); } else { LOG(DBG_LVL, MSG); }

#define _B2R_GetDP(o)                   ((VDP_CFG_T *)(o)->avVideoPanel[0])

#define VDP2B2RID(ucVdpId)              (_prVdpCfg[ucVdpId]->ucB2rId) 

#define B2R_MUTEX_LOCK(id)              _B2R_VsyncMutexLock(id)
#define B2R_MUTEX_UNLOCK(id)            _B2R_VsyncMutexUnlock(id)

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------
#define PD_32_FLAG_SEQ                  (4)
// after field trail (ch48_f49_0530_signal84_V_Shake)
#define PD_32_FLAG_LOOP_SIZE            (12)                        // 0.5 second
#define PD_32_FLAG_LEAVE_LOOP_SIZE      (6)                         // 0.25 second
#define VDP_STC_CLOCK                   (90000)                     // 90 KHz
// 0~3 for MPEG, 4 for Video Input, 5 for DV
#define MAX_ES_NS                    6

#define B2R_MAX_INPORT_NS           1    ///< Maximum number of input port for each video plane.

// Trick Play at 20081001
#define VDP_PLAY_NORMAL 0 ///< Normal Play
#define VDP_PLAY_TRICK 1 ///< Trick Play
#define VDP_PLAY_STEP_FORWARD 2 ///< Step Forward
#define VDP_PLAY_SLOW_FORWARD 3 ///< Slow Forward

// B2R Video plane mode.
#define VDP_MODE_NORMAL             0    ///< normal mode, video out
#define VDP_MODE_BG                 1    ///< background mode, mute
#define VDP_MODE_QV                 2    ///< obsolete
#define VDP_MODE_DEINT              3    ///< obsolete
#define VDP_MODE_UNKNOWN            4

// Maximum number of B2R module
#define B2R_NS                      4// 2
#define B2R_1                       0
#define B2R_2                       1
#define B2R_3                       2//// New B2R HD
#define B2R_4                       3//// New B2R HD

#define B2R_INVALID                 0xFF

// Video plane status
#define VDP_STATUS_NOSIGNAL         0
#define VDP_STATUS_NOSUPPORT        1
#define VDP_STATUS_UNKNOWN          2
#define VDP_STATUS_STABLE           3

// Aspect Ratio
#define VDP_UNIT_ASPECT_RATIO       0x10000
#define VDP_4_3_ASPECT_RATIO        ((B2R_UNIT_ASPECT_RATIO * 4) / 3)
#define VDP_16_9_ASPECT_RATIO       ((B2R_UNIT_ASPECT_RATIO * 16) / 9)
#define VDP_221_1_ASPECT_RATIO      ((B2R_UNIT_ASPECT_RATIO * 221) / 100)

// B2R Mode
#define VDP_B2R_MODE_UNKNOWN        0
//i
#define VDP_B2R_MODE_480I_60        1
#define VDP_B2R_MODE_480I_120       2

#define VDP_B2R_MODE_576I_50        3
#define VDP_B2R_MODE_576I_100       4

#define VDP_B2R_MODE_1080I_48		5
#define VDP_B2R_MODE_1080I_50       6
#define VDP_B2R_MODE_1080I_60       7
#define VDP_B2R_MODE_1080I_100      8
#define VDP_B2R_MODE_1080I_120      9


//p
#define VDP_B2R_MODE_480P_24        10
#define VDP_B2R_MODE_480P_30        11
#define VDP_B2R_MODE_480P_48        12
#define VDP_B2R_MODE_480P_60        13
#define VDP_B2R_MODE_480P_120       14

#define VDP_B2R_MODE_576P_25        15
#define VDP_B2R_MODE_576P_50        16
#define VDP_B2R_MODE_576P_100       17

#define VDP_B2R_MODE_720P_24        18
#define VDP_B2R_MODE_720P_25        19
#define VDP_B2R_MODE_720P_30        20
#define VDP_B2R_MODE_720P_48        21
#define VDP_B2R_MODE_720P_50        22
#define VDP_B2R_MODE_720P_60        23
#define VDP_B2R_MODE_720P_100       24
#define VDP_B2R_MODE_720P_120       25

#define VDP_B2R_MODE_1080P_24       26
#define VDP_B2R_MODE_1080P_25       27
#define VDP_B2R_MODE_1080P_30       28
#define VDP_B2R_MODE_1080P_48       29
#define VDP_B2R_MODE_1080P_50       30
#define VDP_B2R_MODE_1080P_60       31
#define VDP_B2R_MODE_1080P_120      32


//non standard
#define VDP_B2R_MODE_480P_1440W_24       33
#define VDP_B2R_MODE_480P_1440W_60       34

#define VDP_B2R_MODE_1440P_1280W_24       35
#define VDP_B2R_MODE_1440P_1280W_25       36
#define VDP_B2R_MODE_1440P_1280W_30       37
#define VDP_B2R_MODE_1440P_1280W_50       38
#define VDP_B2R_MODE_1440P_1280W_60       39

#define VDP_B2R_MODE_720P_1080W_24		40  //33~37 add for CR DTV00351047 when mm video is 1024*768(tab)and 1080*540(sbs) => v_total too large to do MJC 3D in mt5396
#define VDP_B2R_MODE_720P_1080W_25		41
#define VDP_B2R_MODE_720P_1080W_30		42
#define VDP_B2R_MODE_720P_1080W_50		43
#define VDP_B2R_MODE_720P_1080W_60		44

#define VDP_B2R_MODE_1080P_ULTRAW_24       45
#define VDP_B2R_MODE_1080P_ULTRAW_25       46
#define VDP_B2R_MODE_1080P_ULTRAW_30       47
#define VDP_B2R_MODE_1080P_ULTRAW_50       48

#define VDP_B2R_MODE_480P_12		49
#define VDP_B2R_MODE_480P_15		50
#define VDP_B2R_MODE_480P_20		51

#define VDP_B2R_MODE_576P_12		52
#define VDP_B2R_MODE_576P_15		53
#define VDP_B2R_MODE_576P_20		54
#define VDP_B2R_MODE_576P_60		55

#define VDP_B2R_MODE_720P_12		56
#define VDP_B2R_MODE_720P_15		57
#define VDP_B2R_MODE_720P_20		58

#define VDP_B2R_MODE_1080P_12		59
#define VDP_B2R_MODE_1080P_15		60
#define VDP_B2R_MODE_1080P_20		61


#ifdef CC_MT5399
#define VDP_B2R_MODE_4K2K_15		62
#define VDP_B2R_MODE_4K1K_30       63
#define VDP_B2R_MODE_4K2K_30       64
#define VDP_B2R_MODE_4K2K_24       65
#define VDP_B2R_MODE_4K2K_25       66

#endif
#define VDP_B2R_MODE_MAX            70

// Video Call Back Message Type
#define VDP_MSG_NOTIFY              0
#define VDP_MSG_CC_CB               1
#define VDP_MSG_AFD_CB              2
#define VDP_MSG_UNMUTE_CB           3
#define VDP_MSG_MM_CB               4
#define VDP_MSG_LIPSYNC_CB          5
#define VDP_MSG_MM_STEP_FIN_CB      6
#define VDP_MSG_MM_SEEK_FIN_CB      7
#define VDP_MSG_REPEAT_DROP_CB      8
#define VDP_MSG_LIPSYNC_OK_TO_NG_CB 9
#define VDP_MSG_LIPSYNC_NG_TO_OK_CB 10
#define VDP_MSG_GET_PIC_TYPE_CB     11
#define VDP_MSG_PLAY_DONE_CB		12
#define VDP_MSG_RES_CHG_CB			13
#define VDP_MSG_TRIG_AUD_TO_PLAY    14
#define VDP_MSG_MM_COMM_CB          15


//B2R clock mode
#define VDP_B2R_CLK_UNKNOWN         0
#define VDP_B2R_CLK_27_000          1   // 27.000 MHz: for 23.97Hz, 29.97Hz, 59.94Hz, 25Hz, 50Hz
#define VDP_B2R_CLK_27_027          2   // 27.027 MHz: for 24Hz, 30Hz, 60Hz
#define VDP_B2R_CLK_74_176          3   // 75.176 MHz: for 23.97Hz, 29.97Hz, 59.94Hz
#define VDP_B2R_CLK_74_250          4   // 75.250 MHz: for 24Hz, 25Hz, 30Hz, 50Hz, 60Hz
#define VDP_B2R_CLK_148_352         5   // 148.352 MHz: for 23.97Hz, 29.97Hz, 59.94Hz
#define VDP_B2R_CLK_148_500         6   // 148.500 MHz: for 24Hz, 25Hz, 30Hz, 50Hz, 60Hz
#define VDP_B2R_CLK_54_000          7
#define VDP_B2R_CLK_54_054          8
#define VDP_B2R_CLK_10_368			9
#define VDP_B2R_CLK_21_996			10
#define VDP_B2R_CLK_37_125			11
#define VDP_B2R_CLK_08_100			12
#define VDP_B2R_CLK_10_800          13
#define VDP_B2R_CLK_32_400          14
#define VDP_B2R_CLK_297_000         15   // 297000 MHZ for 4K2K 30FPS
#define VDP_B2R_CLK_064_800         16




// Video plane status
#define VDP_B2R_SOURCE_CHG          0
#define VDP_B2R_OUTPUT_CHG          1
#define VDP_B2R_START_PLAY          2
#define VDP_B2R_RESOLUTION_NFY      3
#define VDP_B2R_NO_SIGNAL           4
 

// Video Plane COMMAND (Max 256)
#define VDP_CMD_RST                 0
#define VDP_CMD_SET_ENABLE          1
#define VDP_CMD_SET_MODE            2
#define VDP_CMD_SET_INPUT           3
#define VDP_CMD_SET_OUTPUT          4
#define VDP_CMD_SET_SRCR            5
#define VDP_CMD_SET_OUTR            6
#define VDP_CMD_SET_BG              7
#define VDP_CMD_SET_SRCSIZE         8
#define VDP_CMD_MAX                 9

//B2R Enable Status
#define B2R_ENABLE                  1
#define B2R_DISABLE                 0

#undef  _INFO_HEADER
#undef  _API_HEADER
#undef  _ERROR_HEADER
#define _INFO_HEADER        "{B2R}Info:"
#define _API_HEADER         "{B2R}API: "
#define _ERROR_HEADER       "{B2R}Err: "

/*  B2R Notify Conditions for MM*/
typedef enum
{
    VDP_COND_ERROR = -1,
    VDP_COND_EOS,
    VDP_COND_SYNC_POINT,
    VDP_COND_RANGE_AB_DONE,
    VDP_COND_CHAP_DONE = 5
}  VDP_COND_T;

typedef enum
{
	ADDR_MODE_64_32,
	ADDR_MODE_16_32,
	ADDR_MODE_RASTER
}ADDR_MODE_T;

// Video Call Back Message Q
typedef struct
{
    UINT32 u4Type;
    UINT32 u4Arg1;
    UINT32 u4Arg2;
    UINT32 u4Arg3;
} VDP_CB_MSG_T;

/**
 *  PTS Reach Notify Function Type
 */
typedef void (*PTS_CB_FUNC)(UINT32 u4VdpId, UINT32 u4Pts, UINT32 u4Arg);

/**
 *  AFD/Aspect Ratio Change Notify Function Type
 */
typedef void (*AFD_CFG_CB_FUNC)(UINT32 u4VdpId, UINT32 u4Afd, FBM_ASPECT_RATIO_T* rAspectRatio);

/**
 *  Video Play Done Notify Function Type
 */
typedef void (*PLAYDONE_CFG_CB_FUNC)(UINT32 u4VdpId, UINT32 u4Arg1, UINT32 u4Arg2);

/**
 *  UnMute Status Change Notify Function Type
 */
typedef void (*UNMUTE_CFG_CB_FUNC)(UINT32 u4VdpId, BOOL fgUnMute);


/**
 *  Video Plane Notify Function Type for MM
 */
typedef void (*VDP_MM_CB_FUNC)(UINT32 u4VdpId, VDP_COND_T eCond, BOOL fgFastForward, UINT32 u4AttachedSrcId);


/**
 *  Video Plane Lip Sync Notify
 */
typedef void (*VDP_LIPSYNC_CB_FUNC)(void);

typedef void (*B2R_CB_COMM_FUNC)(UINT32 u4VdpId, UINT32 u4Data, UINT32 u4FuncPtr, UINT32 u4Tag);

#ifdef CC_B2R_RM_SUPPORT

typedef enum
{
    B2R_RM_ALLOC_OK,     ///< B2R Decode RM alloc HW OK
    B2R_RM_ALLOC_NG,     ///< B2R Decode RM alloc HW Fail
    B2R_RM_ALLOC_CFT,    ///< B2R Decode RM alloc HW conflict
}B2R_RM_COND_T;

typedef VOID (*PFN_B2R_RM_CB)
(
     UCHAR ucVdpId, B2R_RM_COND_T eCond,  UCHAR ucCurrentVdpId
);
EXTERN VOID B2R_RegRmCb(PFN_B2R_RM_CB pfnRmCb);
EXTERN VOID B2R_RmSetKickDone(UCHAR ucEsId, BOOL fgSuccess);

#endif
#define VDP_SEEK_STEP_OK         0
#define VDP_SEEK_STEP_NO_DATA    1
/**
 *  Video Plane Step Forward Finish Notify
 */
typedef void (*VDP_STEP_FIN_CB_FUNC)(UINT32 u4VdpId, UINT32 u4Pts, UINT32 u4Ret, UINT32 u4AttachedSrcId);


/**
 *  Video Plane Seek Finish Notify
 */
typedef void (*VDP_SEEK_FIN_CB_FUNC)(UINT32 u4AttachedSrcId, UINT32 u4VdpId, UINT32 u4Pts, UINT32 u4Ret);


/**
 *  Video Plane Repeat/Drop Notify
 */
typedef void (*VDP_REPEAT_DROP_CB_FUNC)(UINT32 u4VdpId, UINT32 u4RepeatDrop, UCHAR ucEsId);

typedef void (*VDP_RES_CHG_CB_FUNC)(UINT32 u4VdpId);



//from vdp_hal.c
typedef struct
{
    UCHAR ucTrickPlay; /* Trick Play at 20081001 */
    UCHAR ucStepForward; /* Step Forward at 20081030 */
    UCHAR ucSlowForward; /* Slow Forward at 20090506 */
} B2R_INFO_T;

/** Brief of VDP_STC_T
 */
typedef struct
{
    BOOL fgEnable;
    BOOL fgDiv10;
    UINT32 u4Speed;
    UINT32 u4VSyncPeriod;
    UINT32 u4B2rStcVal;
} B2R_STC_T;

enum
{
	MIRROR_ON,
	MIRROR_OFF,
	INVALID_MIRROR_STATUS
};

typedef struct _VDP_FREEZE_PRM_T
{
    UINT32 u4OnOff;
    UINT32 u4Recovery;
    UINT32 u4UnFreeze;
    UINT32 u4CleanDispQ;
} VDP_FREEZE_PRM_T;

//integrate local variable 
typedef struct 
{
    B2R_INFO_T rB2rData;
    B2R_STC_T rB2rStc;
    UCHAR ucMirrorStatus;
    UINT32 u4RepeatCntForPushTrickMode;
    UINT32 u4ElapseCntForPushTrickMode;
    UCHAR ucRepeatFbForPushTrickMode;
    BOOL fgModeChangeSent;
    UINT32 u4LastWidth;
    UINT32 u4LastHeight;
    UINT32 u4OriSrcWidth;
    UINT32 u4OriSrcHeight;
    BOOL fgRepeatFrame;
    BOOL fgDropFrame;
    //UINT32 u4PreviousResizeWidth;
    //UINT32 u4PreviousResizeHeight;
    //UINT32 u4PreviousResizeWidth2;
    //UINT32 u4PreviousResizeHeight2;
    UINT32 u4DriftCount;
    UINT32 u4MaxDriftCount;
    UINT32 u4ValidCount;
    BOOL fgB2RProfile;
    UCHAR ucRemainTimeToSendSeekDone;
    BOOL fgTimeShfitThumbStartPlay;
    UINT32 u4TimeShfitChangeFrmCnt;
    BOOL fgNPTVStable;
    UINT32 u4WaitNPTVStableCount;
    UINT8 u1PlayMode;
    UINT16 u2VsyncNotStableCount;
    UCHAR ucFrameRateUnkownCount;
    BOOL fgInvalidFrmRate;
    BOOL fgHandleSeqChg;
    BOOL fgEnableWaitSTC;
    FBM_ASPECT_RATIO_T rFbmAspectRatio;
    UINT32        u4ReturnValue;

    BOOL  fgDropFb;
    BOOL  fgGstPlayBack;
    BOOL  fgPauseMM ;
    BOOL  fgTimeShift;
    BOOL  fgFirstFrame;
    BOOL  fgPendingForSTC;
    BOOL  fgPendingChgFrmMsg;
    VDP_B2R_CHG_FRAME_MSG_T rPendingChgFrmMsg;
    VDP_FREEZE_PRM_T  rVdpDtvFreeze;
    VDP_TRIG_AUD_PRM_T rVdpTrigAud;
    STC_SPEED_TYPE_T eSpeed;
    VDP_SEEK_PRM_T rVdpSeek;
    INT32 i4TimeShiftSpeed;
    

    // LipSync Call Back
    UINT32 u4CbSyncEn;
    UINT32 u4CbSyncThrsd;
    UINT32 u4CbSyncChkFrmCnt;
    UINT32 u4CbSyncFrmCnt;
    UINT32 u4CbSyncDone;

    // PTS Call Back
    UINT32 u4CbPtsEn;
    UINT32 u4CbPts;
    UINT32 u4CbArg;
    UINT32 u4CbCntDown;
    UINT32 u4CbLastPts;

    //--for AB repeat
    VDP_AB_INTO_T rAB;

    // for sequence change speedup
    BOOL fgSeqChgSpeedup;

    // for double clock
    BOOL fgDoubleClock;

    VDP_LOCK_PIC_INFO_T arLockBuf[VDP_MAX_LOCK_BUF_NS]; 
    BOOL fgEnableVideoLog;
#ifdef CC_B2R_ENABLE_CHG_FRAME_MSG
    BOOL fgEnableGstLog;
#endif
} B2R_VAR_T;

typedef struct
{
    //hal part, must be in the first position
    VOID* pvData;
    VOID  (*IsrCb)(VOID *, VOID *);
    VOID* hB2r;

    UCHAR ucB2rId;

    //todo
    UINT32 u4State; //b2r thread State
    UINT32 u4HWSt;  //b2r thread State
    HANDLE_T hMsgQ;
    HANDLE_T hNotifyMsgQ;


    //output panel
    VOID* avVideoPanel[4];
    UINT32 u4VideoPanelNs;

    //sub structure
    B2R_PRM_T *ptB2rPrm;
    B2R_VAR_T *ptB2rVar;
} B2R_OBJECT_T;

typedef struct _B2R_4K60FRC4K30_T
{
    UINT32 u4SeqWidth;
    UINT32 u4SeqHeight;
    UINT32 u4Rate;
    UINT32 u4Scale;
} B2R_4K60FRC4K30_T;

///-----------------------------------------------------------------------------
/// Public functions
///-----------------------------------------------------------------------------
B2R_OBJECT_T * _B2R_GetObj(UCHAR ucB2rId);
VOID _B2R_ObjectInit(VOID);

void _VDP_StatusNotify(UCHAR ucVdpId, UINT32 u4Status);

void _VDP_EventNofify(UCHAR ucVdpId, UINT32 u4Arg1, UINT32 u4Arg2, UINT32 u4Arg3);

VOID _B2R_SetTimeShiftMode(UCHAR ucB2rId,BOOL fgEnable);

VOID _B2R_SetPauseMM(UCHAR ucB2rId, BOOL fgPauseMM);

STC_SPEED_TYPE_T _B2R_GetFrcStcSpeed(UCHAR ucB2rId);

UCHAR _B2R_GetWaitSTC(UCHAR ucB2rId);

VOID _B2R_SetABRepeat(UCHAR ucB2rId, UCHAR ucSet);

BOOL _B2R_TriggerAudReceiveEx(UCHAR ucB2rId, BOOL fgEnable, UINT32 u4Pts);

VOID _B2R_SetEnableLog(UCHAR ucB2rId,BOOL fgEnableGstLog, BOOL fgEnableVideoLog);

BOOL _B2R_GetEnableGstLog(UCHAR ucB2rId);
//from vdp_hal.h
void _B2R_SetTrickMode(UCHAR ucB2rId, UCHAR ucTrick);
UCHAR _B2R_GetTrickMode(UCHAR ucB2rId);
void _B2R_SetStepForward(UCHAR ucB2rId, UCHAR ucSet);
UCHAR _B2R_GetStepForward(UCHAR ucB2rId);
void _B2R_SetSlowForward(UCHAR ucB2rId, UCHAR ucSet);
UCHAR _B2R_GetSlowForward(UCHAR ucB2rId);
void _B2R_UpdateStc(B2R_OBJECT_T *this);
void _B2R_StcSpeed(UCHAR ucB2rId,UINT32 u4Speed, BOOL fgDiv10);
void _B2R_StcPeriod(UCHAR ucB2rId,UINT32 u4VSyncPeriod);
void _B2R_EnB2rUpdateStc(UCHAR ucB2rId,BOOL fgEnable);
BOOL _B2R_GetUpdateStc(UCHAR ucB2rId);
void _B2R_SetB2rStc(UCHAR ucB2rId,UINT32 u4B2rStc);
UINT32 _B2R_GetB2rStc(UCHAR ucB2rId);
void _B2R_SetMirrorStatus(UCHAR ucB2rId, BOOL fgOn);
UINT8 _B2R_HalGetUnderFlowFlag(UCHAR ucB2rId);
VOID _B2R_HalSetUnderFlow(UCHAR ucB2rId, UINT32 u4Val);
VOID VDP_HalIncreaseUnderFlow(UCHAR ucB2rId, UINT32 u4Step);
VOID _B2R_HalSetStartLineCnt(UCHAR ucB2rId, UINT32 u4Value);
VOID VDP_HalSetStopLineCnt(UCHAR ucB2rId, UINT32 u4Value);
UINT32 VDP_HalGetB2RFifoStatus(UCHAR ucB2rId);
void _B2R_HalResetB2RFifoStatus(UCHAR ucB2rId);
VOID _B2R_HalIncreaseTearing(UCHAR ucB2rId, UINT32 u4Step);
/* whether Update STC when VDP start output or not */
BOOL _B2R_SetPendingForStc(UCHAR ucB2rId, BOOL fgPending);
/* VDP Seek done notification */
VOID _B2R_SetSeek(UCHAR ucB2rId, UCHAR ucSet, UINT32 u4Pts);
VOID _B2R_SetSeekMode(UCHAR ucB2rId, UCHAR ucSet, UINT32 u4Pts);
/* VDP Set Wait STC while starts */
VOID _B2R_SetWaitSTC(UCHAR ucVdpId, UCHAR ucSet);
VOID _B2R_SetTimeShiftSpeed(UCHAR ucB2rId,UINT32 u4SpeedValue);
BOOL _B2R_GetTimeShfitStartPlay(UCHAR ucB2rId);
INT32 _B2R_GetPtsStcDelta(UCHAR ucB2rId);

void _B2R_StatusTest (UCHAR ucB2rId);
void _B2R_DisStatusTest (UCHAR ucB2rId);
void _B2R_GetYCStart(UCHAR ucB2rId,UINT32 *pu4YStart, UINT32 *pu4CStart, UINT32 *pu4LineSize, UINT32 *pu4VActive);
void _B2R_SetBlack(UCHAR ucB2rId, BOOL fgBlack);
void _B2R_ForceBg(UCHAR ucB2rId, UCHAR ucForce);
EXTERN void B2R_SetB2rUpdateStc(UINT8 ucVdpId,BOOL fgEnable);
EXTERN BOOL B2R_GetB2rUpdateStc(UINT8 ucVdpId);

void _B2R_FrcSetFreeze(UCHAR ucB2rId, UINT32 u4OnOff);
VOID _B2R_SetDropFrame(UCHAR ucB2rId);
VOID _B2R_SetRepeatFrame(UCHAR ucB2rId);
UINT32 _B2R_SetABInfo(UCHAR ucB2rId, VDP_AB_INTO_T *prAB);
VOID _B2R_FrcSetLipSyncCb(UCHAR ucVdpId, UCHAR ucPort, UINT32 u4Thrsd, UINT32 u4FrmCnt);
UINT32 _B2R_CalculateVsyncNs(B2R_OBJECT_T* this);
UINT32 _B2R_PtsCbProc(B2R_OBJECT_T * this, UINT32 u4Status);
void _B2R_PostNotify(B2R_OBJECT_T *this, UINT32 u4Type, UINT32 u4Arg2, UINT32 u4Arg3);
UINT32 _B2R_GetContainerType(B2R_OBJECT_T *this);
VOID _B2R_PreparePendingForNextVsync(B2R_OBJECT_T* this);
UINT32 _B2R_SetPathMute(UCHAR ucB2rId, MUTE_MODULE_ID eModule, UINT32 u4UnMuteDelay, BOOL bReplaceByLongerDelay);
void _B2R_GetSeek(B2R_OBJECT_T* this, UCHAR* pucSeekMode, UINT32* pu4Pts, UCHAR* pucNotifyTrigger);
UINT32 _B2R_GetDispingPts(B2R_OBJECT_T* this);
UINT32 _B2R_GetEsmQCnt(void);
void B2R_EnalbleSmartPcr(UCHAR ucB2rId, BOOL fgEn);
BOOL _VDP_RTNeedSuperGameMode(UCHAR ucVdpId);
UINT32 _B2R_GetContentSourceType(B2R_OBJECT_T *this);

#ifdef CC_SUPPORT_NPTV_SEAMLESS 
EXTERN VOID _B2R_NPTVSeamlessStatus(B2R_OBJECT_T* this, UCHAR ucMode);
#endif

UINT8 _B2R_GetVdpIds(UINT8 u1B2rId, UINT8* pu1VdpIds);

UINT8 _B2R_GetVdpId(UINT8 u1VdpId);

void VDP_SetRenderRegion(UINT8 ucVdpId, VDP_REGION_T *prRendRegion);
/**
 *  Vsync callback
 */
typedef VOID (*VDP_VSYNC_CB_FUNC)(UCHAR ucVdpId, VOID* pv_tag);
BOOL VDP_VsyncCBRegister(UCHAR ucVdpId, VDP_VSYNC_CB_FUNC VDP_VsyncCB, VOID* pv_tag);

BOOL B2R_OneFbChgTrigger(UCHAR ucVdpId);

void _B2R_EventNofify(UCHAR ucVdpId, UINT32 u4Arg1, UINT32 u4Arg2, UINT32 u4Arg3);

#endif    // B2R_DRVIF_H

