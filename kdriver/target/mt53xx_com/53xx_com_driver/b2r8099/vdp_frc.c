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
 * $Date: 2015/03/05 $
 * $RCSfile: vdp_frc.c,v $
 * $Revision: #16 $
 *
 *---------------------------------------------------------------------------*/

/*****************************************************************************
*  Notes: (MT5351)
*  (1) Deint initialize, 20050613
*      Z(657a800) Y(657a800) X(657a800) W(657a800), Normal Mode (ucDeintEn = 1, ucDeintNextEn = 1)
*      Z(657a800) Y(657a800) X(657a800) W(65e6800), Normal Mode (ucDeintEn = 1, ucDeintNextEn = 1)
*  ->  Z(657a800) Y(657a800) X(65e6800) W(65e6800), Deintelace Mode (ucDeintEn = 1, ucDeintNextEn = 0)
*  ->  Z(657a800) Y(65e6800) X(65e6800) W(64a2800), Deinterlace Mode (ucDeintEn = 1, ucDeintNextEn = 0)
*      ucDeintNextEn
*  (2) 32 pull down bad edit detection, 20050626
*      Z Y X W   (Frame n - 1) (Check comb register)
*        Z Y X W (Frame n)
*      At frame n, if merge ZY, check YX comb, if merge YX, check XW comb
*      At frame n, if merge ZY, YX < XW comb, if merge YX, XW < YX comb
*      At frame n, if merge YX, XW comb < (XW + threshold) comb of frame n - 3
*  (3) display fail (no frame buffer) at deinterlace mode, 20050621
*      Z(65e6800) Y(65e6800) X(6f6b800) W(6f6b800), XZ(53) WY(0) YX(0) WX(0) Reg(2014)
*      Z(65e6800) Y(6f6b800) X(6f6b800) W(650e800), XZ(0) WY(57) YX(0) WX(1340) Reg(2016)
*      Z(6f6b800) Y(6f6b800) X(650e800) W(650e800), XZ(57) WY(54) YX(1218) WX(0) Reg(2014)
*      Z(6f6b800) Y(650e800) X(650e800) W(6f6b800), XZ(54) WY(68) YX(0) WX(1488) Reg(2016)
*  ->  Z(6f6b800) Y(650e800) X(650e800) W(6f6b800), XZ(68) WY(54) YX(103) WX(3815) Reg(2016)
*  ->  Z(6f6b800) Y(650e800) X(650e800) W(6f6b800), XZ(54) WY(68) YX(1488) WX(0) Reg(2016)
*  ->  Z(650e800) Y(650e800) X(64a2800) W(6f6b800), XZ(68) WY(54) YX(103) WX(3815) Reg(2014)
*      a) we should skip bad edit detection, twice
*      b) we should update both X and W together when new frame rx
*      b) we should leave 32/22 pd mode
*  (4) VDP_HalSetDeintX, 20050621
*      Z = Y, Y = X, W = X = New
*      Save WXYZ at FRC, easy for chroma ZX flip
*      Update Address after function _VdpDeintHandleCounter, which will design still field direction (YZ or YX)
*      If merge YZ, flip chroma address of Z & X
*  (5) Access Frame, 20050621
*      ucProgressiveSeqFlag, ucValid32Seq, or NOT prFrcPrm->ucForceInterlaceSrc
*  (6) Access Top, 20050621
*      According to MPEG2 top field first
*  (7) 32 Pull Down Sequence
*      1  2->2  2
*         2<-2->2  3           '<-' or '->'? '->' is better, looking forward
*            2<-2  3  3
*               2  3->3  4
*                  3<-3  4  4
*      Example:
*      XZ(44) WY(39) YX(626) WX(0) Reg(2014)
*      XZ(39) WY(28) YX(0) WX(553) Reg(2014)
*      (0) PD Mode 1 >> 0
*      Bad-Edit(2) (27, 1c, 0, 229)
*  (8) Increase 32 motion threshold from 8 to 64 for Ô¤h & TS pattern, 20050628
*  (9) 32 flag detection, 20050628
*      8 >> 24, make it more difficult to enter 32 flag
*      enter 32 pull down mode in the future
*  (10) Error handle at deinterlace mode, 20050628
*      When we detect discontiuous termporal reference, we will pause
*      deinterlace for 2 field, which will reduce sawtooth when error happen
*  (11) 32 pull down bad edit detection, 20050630
*      Z Y X W   (Frame n - 1) (Check comb register)
*        Z Y X W (Frame n)
*      At frame n, if merge ZY, check YX comb, if merge YX, check XW comb (Skip)
*      At frame n, if merge ZY, YX < XW comb, if merge YX, XW < YX comb (Skip)
*      Check motion counter
*  (12) Refine AV Sync timing, STC' = STC + 1 Vsync Time, display 1 Vsync earlier
*  (13) Scene Change, 20050630
*      If Current YW motion > (Previous YW motion + Threshold)
*      Based on MT8205 motion register
*  (14) I to I without deinterlace, 20050707
*      50i to 60i or 60i to 50i, playback Top only (DC)
*  (15) Bug Fixed of (10)
*      I we pause deinterlace, a flicker will happen while changing mode
*      worst than sawtooth!
*****************************************************************************/

/*****************************************************************************
*  Notes: (MT5371)
*  (1) If multiple field change due to TFF/RFF (_VdpBaseVSyncNs), skip FRC process
*****************************************************************************/


/** @file vdp_frc.c
 *  Brief of file vdp_frc.c.
 *  Details of file vdp_frc.c (optional).
 *  Porting From DTV_X_IDTV0801 1.8.2.88
 */


//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "vdp_drvif.h"
#include "vdp_if.h"  // Trick Play at 20081001
#include "b2r_if.h"
#include "fbm_drvif.h"
#include "b2r_drvif.h"
#include "vdp_frc.h"
#include "vdp_vsync.h"
#include "vdp_hal.h"
#include "vdp_debug.h"
#include "drv_di.h"
#include "aud_drvif.h"
#include "imgrz_if.h"
#include "imgrz_drvif.h"
#include "vdec_drvif.h"
#include "mute_if.h"
#include "tve_if.h"
#include "x_bim.h"
#include "b2r_hal.h"
#include "vdo_rm.h"

#include "x_lint.h"
//LINT_EXT_HEADER_BEGIN
#include "gfx_if.h"
#include "gfx_drvif.h"
#include "drv_common.h"
#include "drv_dbase.h"
#ifdef CC_SCPOS_EN
#include "drv_scpos.h"
#include "drv_video.h"
#endif
#include "x_hal_5381.h"
#include "x_os.h"
#include "x_assert.h"
#include "x_syslog.h"
#include "x_timer.h"
#ifdef TIME_MEASUREMENT
#include "x_time_msrt.h"
#include "u_time_msrt_name.h"
BOOL fgB2REarlyDisp = FALSE;
#endif
#ifdef CC_FLIP_MIRROR_SUPPORT
#include "drv_display.h"
#endif
#ifdef CC_MAPLE_CUST_DRV
#include "pe_if.h"
#endif

#include "b2r_timing.h"
#include "drv_display.h"
#include "vdo_misc.h"

#ifdef VDP_B2R_PSCAN_CRC_FLOW
static UINT32 _u4SetPts = 0xFFFFFFFF;
static BOOL _fgTriggerEnc = FALSE;
#endif
static UCHAR uVsyncNum = 2;

#ifdef CC_B2R_SUPPORT_GAME_MODE
#define VDP_MAX_RELEASE_Q 10

typedef struct _B2R_ReleaseQ
{
   INT32   i4FbReadIdx;
   INT32   i4FbWriteIdx;
   INT32   i4FbReleaseQ[VDP_MAX_RELEASE_Q];
   INT32   i4FbCount;
} B2R_ReleaseQ;

static B2R_GAME_MODE_TYPE_T eGameMode[B2R_NS] = {B2R_GAME_MODE_OFF,B2R_GAME_MODE_OFF};

B2R_ReleaseQ  _rB2rRleaseFb;
static void VdpInitreleaseDiaplayQ(void);
static void vdpSetFrameToRelease( UCHAR ucFbId);
static void vdpReadyReleaseFB(B2R_OBJECT_T * this);
#endif

//LINT_EXT_HEADER_END

//LINT_SUPPRESS_BRACE(717)        // Info 717: do ... while(0);

//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------

//#define CC_NETFLIX_SUPPORT

/// Handle 32 Flag
//CLI test for Nordig HDTV overscan and frame cropping
//#define CC_CLI_TEST

// Monitor TFF
//#define VDP_MONITOR_TFF

// Frc Test
//#define CC_FRC_TEST

// Force 32 flag error
//#define CC_FORCE_32_FLAG_ERR

// 32 flag enable/disable test
//#define VDP_TEST_32FLAG

// Work around, Interlace FRC Fail, Between B2R/SCPOS
//#define CC_VDP_NO_INTERLACE_FRC

//#define CC_VDP_FPGA

//#define CC_VDP_FORCE_FIELD_ENCODE

//#define CC_VDP_FRC_DEBUG

//#define WA_1080P_SUB_NO_FRC // 1080p at sub, no 24/30 to 60 conversion

#define CC_B2R_ENABLE_3D_LATE_NOTIFY
#define CC_B2R_ENABLE_R_VIEW_SKIP_FRAME     // Let 3D R view also drop frame (like L view case).

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------

#define VDP_DEBUG_BUF_SIZE              (16)

#define VDP_DISPLAY_FAIL_FREEZE         (255)

#define VDP_TFF_STABLE_COUNT            (12)


#define DURATION_TO_SEND_SEEK_DONE 5

#define STC_PTS_DIFF    (7500)

#define VDP_LIP_SYNC_OK 0
#define VDP_LIP_SYNC_NG 1

#define BURST_READ_THRESHOLD 128

#define KEEP_FB_NUM    3

static UCHAR _ucFrcMuteInitiated = 0;

//#ifdef CC_B2R_RM_SUPPORT

static __TIMING_TBL_T VB1TimingTab[] =
{
    {"1920x2160/30P",   2200, 2250, 120,   1,    297000000}
   
};
BOOL fgVB1Byass=FALSE;
//static UINT32 u4FrameCount;
//#endif
//extern BOOL fgPhotoRasterModeIn4k;

#define B2R_SEEK_TIMEOUT_CNT 15
//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------
/** Brief of B2R_PRM_T
 */

/** Brief of VDP_GFX_COPY_FIELD_PRM_T
 */
typedef struct _VDP_GFX_COPY_FIELD_PRM_T
{
    UCHAR ucFbgId;
    UCHAR ucFbId;
    UINT32 u4Init;
    HANDLE_T hSemaphore;
    HANDLE_T hMutex;
} VDP_GFX_COPY_FIELD_PRM_T;

typedef struct _VDP_FRC_COPY_PRM_T
{
    B2R_PRM_T rPortPrm;
    UINT32 u4TriggerCopy;
} VDP_FRC_COPY_PRM_T;

typedef struct _VDP_LIPSYNC_OK_NG_PRM_T
{
    UCHAR ucLipSyncState; // OK or NG
    UCHAR ucNg2OkCbEn;
    UCHAR ucNg2OkCbDone;
    UINT32 u4NumFrmsOK; //Number of frames after Lip-sync is OK
    UINT32 u4OKThrd;
    UINT32 u4OKFrmCnt; // Number of Frames OK accumulated
    UCHAR ucOk2NgCbEn;
    UCHAR ucOk2NgCbDone;
    UINT32 u4NumFrmsNG; //Number of frames after Lip-sync is NG
    UINT32 u4NGThrd;
    UINT32 u4NGFrmCnt; // Number of Frames NG accumulated
} VDP_LIPSYNC_OK_NG_PRM_T;


//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------

#define FRC_INC_VSYNC_NS(pFrcPrm)                                        \
          do {                                                            \
            if (pFrcPrm->u2CurrentVsyncNs < 0xFF)                        \
            { pFrcPrm->u2CurrentVsyncNs++; }                            \
          } while (0)

#define FRC_SET_VSYNC_NS(pFrcPrm, u2Ns, u2ChgNs)                        \
          do {                                                            \
            pFrcPrm->u2CurrentVsyncNs = 0;                                \
            pFrcPrm->u2ChangeFieldVsyncNs = u2ChgNs;                    \
            pFrcPrm->u2TargerVsyncNs = u2Ns;                            \
          } while (0)

#define FRC_DISP_NS(pFrcPrm)                                            \
    ((pFrcPrm->u2CurrentVsyncNs >= pFrcPrm->u2TargerVsyncNs) ? 0 : (pFrcPrm->u2TargerVsyncNs - pFrcPrm->u2CurrentVsyncNs))

#define FRC_CHANGE_FIELD_NS(pFrcPrm)                                    \
    ((pFrcPrm->u2CurrentVsyncNs >= pFrcPrm->u2ChangeFieldVsyncNs) ? 0 : (pFrcPrm->u2ChangeFieldVsyncNs - pFrcPrm->u2CurrentVsyncNs))

#define FRC_DISP_FINISH(pFrcPrm)                                        \
    (pFrcPrm->u2CurrentVsyncNs >= pFrcPrm->u2TargerVsyncNs)

#define FRC_CHANGE_FIELD(pFrcPrm)                                        \
    (pFrcPrm->u2CurrentVsyncNs == pFrcPrm->u2ChangeFieldVsyncNs)

#define FRC_MEMSET(addr, value, len)    (VERIFY(x_memset((addr), value, len) == (addr)))

#define PENDING_IS_NEEDED   (prFrcPrm->ucPendingFbId != FBM_FB_ID_UNKNOWN)
#define REFETCH_IS_NEEDED   (_arB2rVar[prFrcPrm->ucB2rId].fgDropFb && !prFrcPrm->fgDummyFrame && !_arB2rVar[prFrcPrm->ucB2rId].fgGstPlayBack)

#define FHD_HEIGHT         (1088)
#define FHD_ALIGN_VALUE    (8)
#define HEIGHT_MUST_ALIGN(u4_height) ((u4_height) == FHD_HEIGHT)

//-----------------------------------------------------------------------------
// Imported variables
//-----------------------------------------------------------------------------

#ifdef CC_FAST_CHANGE_CHANNEL_LOG
EXTERN HAL_TIME_T rNvgtStartAvTime;
EXTERN HAL_TIME_T rNvgtVideoStartTime;
EXTERN HAL_TIME_T rNvgtAvSyncTime;
EXTERN UINT32 u4NvgtTimeAhead;
#endif



//-----------------------------------------------------------------------------
// Imported functions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Static function forward declarations
//-----------------------------------------------------------------------------

//extern BOOL fgApiVideoStable(UINT8 u1VideoFrame); // 0: main frame, 1: sub frame for PIP/POP

static VOID _B2R_SeamlessJob(B2R_OBJECT_T* this);
static UINT32 _B2R_FrmInit(UCHAR ucB2rId, UCHAR ucPort, BOOL fgCreateThread);
static VOID _B2R_ChangeFrameBuffer(B2R_OBJECT_T* this);
static BOOL _B2R_GetSequenceInfo(B2R_OBJECT_T *this,BOOL fgPreChk);
static VOID _B2R_GetNextDisp(B2R_OBJECT_T* this);
static UCHAR _B2R_PreLookNextDisp(B2R_PRM_T* prFrcPrm);
 //static VOID _B2R_SetOutInfoByPmx(UCHAR ucPmxMode,VDP_OUT_INFO_T *prOutInfo);
//VOID _B2R_CheckOutputMode(B2R_OBJECT_T *this);
static VOID _B2R_FrcFrameBufferReady(FB_CHG_INFO_T* pt_fb);
#ifndef CC_SUPPORT_NPTV_SEAMLESS 
static BOOL _B2R_ChkSequenceInfo(UCHAR ucFbgId);
#endif
static UINT32 _B2R_TriggerAudOn(B2R_OBJECT_T *this, UINT32 u4CurPts);
static UINT32 _B2R_FrcGetStc(UCHAR ucFbgId);
static BOOL _B2R_HandleEos(B2R_OBJECT_T *this);
static UCHAR _B2R_FreeFrameBeforeEosHandler(B2R_OBJECT_T* this, BOOL fgMain, BOOL fgOMX, BOOL* fgEos);
static BOOL _B2R_ChangeFrameDirectForOMX(B2R_OBJECT_T *this, VDP_B2R_CHG_FRAME_MSG_T* prMsg);
#ifdef CC_3D_MM_DS_SUPPORT
static VOID _B2R_GetFrameBufferForMVC3D(B2R_OBJECT_T* this);
#endif
static VOID _B2R_GetFrameBufferForOMX(B2R_OBJECT_T* this, BOOL* pfgGstPlayBack);
static VOID _B2R_GetFrameBufferForSkype(B2R_OBJECT_T* this);
static VOID _B2R_GetFrameBufferForGeneral(B2R_OBJECT_T* this);
static VOID _B2R_SearchFrameInDisplayQ(B2R_OBJECT_T *this);
static BOOL _B2R_IsFastForward(B2R_OBJECT_T* this);
static VOID _B2R_SetFrameBufferStatus(B2R_OBJECT_T* this, UCHAR ucFbgId, UCHAR ucFbId, UCHAR ucFbStatus);
static VOID _B2R_HandleResolutionChange(B2R_OBJECT_T *this);
static VOID _B2R_RelResource(UCHAR ucB2rId);
static VOID _B2R_GetFrameBufferForBivl3D(B2R_OBJECT_T* this);
static UCHAR _B2R_ConvertFrameRateInx2FrameRate(UCHAR ucIndex);
static BOOL _B2R_IsBivl3D(B2R_OBJECT_T* this);
static UINT32 _B2R_DetermineRepeatForPushTrickMode(B2R_OBJECT_T* this, BOOL fgRepeatFbForNoFb);
static VOID _B2R_CalcRepeatCntForPushTrickMode(B2R_PRM_T* prFrcPrm, BOOL fgRepeatFbForNoFb);
static VOID _B2R_GetFrameBufferFromPending(B2R_OBJECT_T* this);
static VOID _B2R_CpyPicHdr2B2r(B2R_PRM_T* prFrcPrm, FBM_PIC_HDR_T* prPicHdr);
static VOID _B2R_SetB2RMirrorFlip(B2R_OBJECT_T* this);
static VOID _VDP_SetXYOffset(UCHAR ucB2rId);
#ifdef ENABLE_MULTIMEDIA
static UCHAR _B2R_RTFrameRateConvert(B2R_PRM_T* prFrcPrm, UCHAR ucIndex);
#endif
static INT32 _B2R_OneFbChgHandle(B2R_PRM_T* prFrcPrm);

#ifdef DIVX_PLUS_CER
#ifdef   CC_B2R_CHAPTER_CHG
static VDP_CHAPTER_T* _B2R_FrcGetChapterObj(VOID);
static VDP_CHAPTER_T  _arVdpChapter;
static BOOL fgFirstSetChapter = FALSE;
#endif
#endif

#ifdef CC_B2R_ENABLE_CHG_FRAME_MSG
static INT32 _B2R_ReceiveChgFrameMsg(UCHAR ucB2rId,
                                     VOID            *pv_msg,
                                     MSGQ_OPTION_T   e_option);
#endif

#ifdef CC_MAPLE_CUST_DRV
INT32 _B2R_Cust_info_cfg(VDP_CFG_T*    prVdpConf,
                         B2R_PRM_T*    ptVdpPrm);
#endif
static B2R_PACK_MODE_T _B2R_ConBlkMd(FBM_PACK_MODE_T ePackCod);
static B2R_SWAP_MODE_T _B2R_ConSwapMd(FBM_SWAP_MODE_T eSwapCod);
static BOOL _B2R_IsTimingChg(B2R_OBJECT_T *this,
                             B2R_HAL_TIMING_T* ptOrgTiming,
                             B2R_HAL_TIMING_T* ptTiming);
#ifdef ENABLE_MULTIMEDIA
static INT32 _B2R_SeekErrHandle(B2R_OBJECT_T *this);
static INT32 _B2R_RT_TMS_Statistics(B2R_OBJECT_T* this);
#endif
static inline BOOL _B2R_IN_Enable_Status (UCHAR    ucVdpId,
                                                UINT32   u4Enable);
INT32 _B2R_4K60TimingChg(B2R_OBJECT_T*      this, 
                         B2R_4K60FRC4K30_T* pr_force,
                         UCHAR              ucFrmRate);
INT32 _B2R_4K60FbHdr(B2R_OBJECT_T* this);
INT32 _B2R_SpecialTimingHdr(BOOL  fg3D,
                                     B2R_HAL_TIMING_T* ptTiming,
                                     B2R_OUT_INFO_T*   ptOutInfo);
static INT32 _B2R_ColorFmtMonitor(B2R_OBJECT_T* this);
#ifdef CC_B2R_RES_SUPPORT
INT32 _B2R_HwResChgNotify(UCHAR ucFbgId,
                                  B2R_CHG_INFO_T*  pt_change);
#endif

//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------
#ifdef CC_B2R_ENABLE_CHG_FRAME_MSG
static HANDLE_T _ahChgFrameQueue[B2R_NS] = {NULL_HANDLE, NULL_HANDLE};
static HANDLE_T _ahChgFrameMutex[B2R_NS] = {NULL_HANDLE, NULL_HANDLE};
#endif
static HANDLE_T _hDispBufLockMutex[B2R_NS] = {NULL_HANDLE, NULL_HANDLE};    // Mutex to protect multi-thread access buf lock array

#if defined(CC_NETFLIX_SUPPORT) || defined(CC_RESIZE_SMALL_IMAGE)
#define IMG_RESIZE_ALIGN_SIZE 2047
#define IMG_RESIZE_ALIGN(X)  (((X) + IMG_RESIZE_ALIGN_SIZE) & (~IMG_RESIZE_ALIGN_SIZE))
#endif

static B2R_PRM_T    _arVdpPrm[B2R_NS];
static VDP_CFG_T    _arVdpCfg[VDP_NS];

static DRV_DBASE_T* _prDbase = NULL;

/* VDP trigger mode + audio master to do trigger mode */
static VDP_TRIG_AUD_PRM_T _arVdpTrigAud[VDP_NS] = {{0}};

static VDP_LIPSYNC_OK_NG_PRM_T _rVdpLipSync =
{
    VDP_LIP_SYNC_NG,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0
};

#ifdef __MODEL_slt__
UINT32 b2rState = 0xff;
#endif

static B2R_VAR_T _arB2rVar[B2R_NS];

//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------
static BOOL _B2R_Is4To1024XSpeed(UINT32 u4Speed)
{

    if ( (u4Speed >= B2R_SPEED_TYPE_FORWARD_4X )&&
            (u4Speed <= B2R_SPEED_TYPE_FORWARD_1024X ) )
    {
        return TRUE;
    }

    return FALSE;
}

VOID B2R_SetVsyncNum(UCHAR uVNum)
{
   uVsyncNum=uVNum;
   LOG(0,"B2R_SetVsyncNum=%d\n",uVsyncNum);
}
static BOOL _B2R_IsEnableDP(B2R_OBJECT_T *this)
{
    UINT32 i,j;
    VDP_CFG_T *ptDP;

    if (!this)
    {
        return FALSE;
    }

    for (i = 0, j = 0; i < this->u4VideoPanelNs; i++) 
    { 
        ptDP = (VDP_CFG_T *)this->avVideoPanel[i]; 
        if (ptDP)
        {
            j++;
        }
        
        if (ptDP && !ptDP->ucEnable) 
        {
            LOG(3,"ptDP->ucEnable=%d,i=%d,j=%d\n",ptDP->ucEnable,i,j);
            return FALSE;
        }
    } 

    return (j > 0) ? TRUE: FALSE;
}

void _B2R_IsrFin(B2R_OBJECT_T* this)
{
    UCHAR ucB2rId;
    B2R_HAL_STATUS_T tStatus;
    
    if (!this)
    {
        return;
    }
    ucB2rId = this->ucB2rId;
    B2R_HAL_Get(this->hB2r, B2R_HAL_STATUS, &tStatus);
    
    // check tearing
    VDP_HalSetStopLineCnt(ucB2rId, tStatus.u4LineCnt);
    #if 0 //todo
    if (tStatus->u4LineCnt > _u4B2rActiveHeight[ucB2rId])
    {
        VDP_HalIncreaseTearing(ucB2rId, 1);
    }
    #endif
    
}


static void _B2R_SetFrc(B2R_OBJECT_T* this, UINT16 u2TargetNs, UINT16 u2ChangeFieldNs, UCHAR ucFrcMode)
{
    B2R_HAL_FRC_ARG_T tFrcArg;

    if (!this)
    {
        return;
    }
    tFrcArg.u2TargetNs = u2TargetNs;
    tFrcArg.u2ChangeFieldNs = u2ChangeFieldNs;
    tFrcArg.ucMode = ucFrcMode;
    B2R_HAL_Set(this->hB2r, B2R_HAL_FRC, &tFrcArg);
    //_VDP_HalSetFrc(this->ucB2rId, u2TargetNs, u2ChangeFieldNs, ucFrcMode);
}


#ifdef CC_3D_MM_DS_SUPPORT
static UINT8 _B2R_GetAdditionalFbgId(B2R_OBJECT_T* this)
{
    UCHAR ucRet = FBM_FBG_ID_UNKNOWN;
    B2R_PRM_T* prFrcPrm;
    UINT32 u4idx =0;
    UCHAR  ucFbgId;

    do
    {
        if (!this)
        {
            break;
        }
        if (this->ucB2rId >= B2R_NS)
        {
            break;
        }
        prFrcPrm = this->ptB2rPrm;
        prFrcPrm->ucSubFbgId = FBM_FBG_ID_UNKNOWN;
        for (u4idx=0 ; u4idx < MAX_ES_NS ; u4idx++)
        {
            ucFbgId = FBM_GetFbgByEs(u4idx);
            if (ucFbgId != FBM_FBG_ID_UNKNOWN && ucFbgId != prFrcPrm->ucFbgId)
            {
                prFrcPrm->ucSubFbgId = ucFbgId;
            }
        }
        if (_B2R_IsBivl3D(this))
        {
            prFrcPrm->ucSubFbgId = prFrcPrm->ucFbgId;
        }

        if (prFrcPrm->ucSubFbgId == FBM_FBG_ID_UNKNOWN)
        {
            if (prFrcPrm->ucSubFbgIdRecorded!= FBM_FBG_ID_UNKNOWN && prFrcPrm->ucSubFbId != FBM_FB_ID_UNKNOWN)
            {
                _B2R_SetFrameBufferStatus(this, prFrcPrm->ucSubFbgIdRecorded, prFrcPrm->ucSubFbId, FBM_FB_STATUS_EMPTY);
            }
            if (prFrcPrm->ucSubFbgIdRecorded != FBM_FBG_ID_UNKNOWN && prFrcPrm->ucSubReleaseFbId != FBM_FB_ID_UNKNOWN)
            {
                _B2R_SetFrameBufferStatus(this, prFrcPrm->ucSubFbgIdRecorded, prFrcPrm->ucSubReleaseFbId, FBM_FB_STATUS_EMPTY);
            }
            if (prFrcPrm->ucSubFbgIdRecorded != FBM_FBG_ID_UNKNOWN && prFrcPrm->ucSubPendingFbId != FBM_FB_ID_UNKNOWN)
            {
                _B2R_SetFrameBufferStatus(this, prFrcPrm->ucSubFbgIdRecorded, prFrcPrm->ucSubPendingFbId, FBM_FB_STATUS_EMPTY);
            }
            prFrcPrm->ucSubReleaseFbId = FBM_FB_ID_UNKNOWN;
            prFrcPrm->ucSubPendingFbId = FBM_FB_ID_UNKNOWN;
            prFrcPrm->ucSubFbId = FBM_FB_ID_UNKNOWN;
        }
        ucRet = prFrcPrm->ucSubFbgId;
    }while(FALSE);
    
    return ucRet;
}

#endif

static VOID _B2R_SeamlessJob(B2R_OBJECT_T* this)
{
    UINT32 u4AddrY      =   0x0;
    UINT32 u4AddrC      =   0x0;
    UINT32 u4AddrY_len  =   0x0;
    UINT32 u4AddrC_len  =   0x0;
#ifdef CC_3D_MM_DS_SUPPORT
    UINT32 u4AddrYSub   =   0x0;
    UINT32 u4AddrCSub   =   0x0;
    UINT32 u4AddrYSub_len = 0x0;
    UINT32 u4AddrCSub_len = 0x0;
#endif
    UINT32 u4YSize      =   0x0;
    B2R_VAR_T* prB2rVar =NULL;
    UINT8 u1DecoderSrcId = 0;
    UCHAR ucB2rId       =  0;
    UCHAR ucVdpId       =  0;
    FBM_PIC_HDR_T* prFbmPicHdr = NULL;
    FBM_SEQ_HDR_T* prFbmSeqHdr = NULL;
    B2R_PRM_T* prFrcPrm;

    do
     {

        if (!this)
        {
            break;
        }
        
        ucB2rId = this->ucB2rId;
        ucVdpId = _B2R_GetVdpId(ucB2rId);
        prB2rVar = this->ptB2rVar;
        prFrcPrm = this->ptB2rPrm;
        u1DecoderSrcId = FBM_GetDecoderSrcId(prFrcPrm->ucFbgId);
        prFbmSeqHdr = FBM_GetFrameBufferSeqHdr(prFrcPrm->ucFbgId);
        prFbmPicHdr = FBM_GetFrameBufferPicHdr(prFrcPrm->ucFbgId, prFrcPrm->ucFbId);
        //LOG(1,"Pts(%x) FbId(%d)\n",prFbmPicHdr->u4PTS,prFrcPrm->ucFbId);

        if ((prFbmPicHdr != NULL) && (prFbmSeqHdr != NULL))
        {
            UINT8 u1PicType = VDEC_PIC_TYPE_I;
            HAL_TIME_T rEndTime;
            HAL_TIME_T rDeltaTime;
            HAL_TIME_T rDeltaTimeDecoder;

            HAL_GetTime(&rEndTime);

            HAL_GetDeltaTime(&rDeltaTime, &prFbmPicHdr->rTimeHwS, &rEndTime);
            HAL_GetDeltaTime(&rDeltaTimeDecoder, &prFbmPicHdr->rTimeFromDecoder, &rEndTime);

            if (prFbmPicHdr->fgSeqHeader)
            {
                u1PicType = VDEC_PIC_TYPE_SEQ;
                _B2R_PostNotify(this, VDP_MSG_GET_PIC_TYPE_CB,u1DecoderSrcId,u1PicType);
            }

            if (prFbmPicHdr->ucPicCdTp ==MPEG_PIC_TYPE_I)
            {
                u1PicType = VDEC_PIC_TYPE_I;
            }
            else if (prFbmPicHdr->ucPicCdTp ==MPEG_PIC_TYPE_P)
            {
                u1PicType = VDEC_PIC_TYPE_P;
            }
            else if (prFbmPicHdr->ucPicCdTp ==MPEG_PIC_TYPE_B)
            {
                u1PicType = VDEC_PIC_TYPE_B;
            }
            else
            {
                u1PicType = VDEC_PIC_TYPE_I;
            }
            _B2R_PostNotify(this, VDP_MSG_GET_PIC_TYPE_CB,u1DecoderSrcId,u1PicType);

            if (prB2rVar->u4TimeShfitChangeFrmCnt >= 5)
            {
                prB2rVar->fgTimeShfitThumbStartPlay = TRUE;
            }
            prB2rVar->u4TimeShfitChangeFrmCnt ++;

           
            if (prFbmSeqHdr->fgResizeSmallPic)
            {
                //only 2d Mode
                UINT32 u4YResizeAddr = 0;
                UINT32 u4CResizeAddr = 0;
                //UINT32 *pu4PrevResizeWidth = NULL;
                //UINT32 *pu4PrevResizeHeight = NULL;
                UINT32 u4PicX, u4PicY, u4PicWidth, u4PicHeight;
                UINT32 u4HeightAlign = (((prFbmPicHdr->u4PicHeight) + 31) & (~31));

                RZ_VDO_SCL_PARAM_SET_T rScaleParamV;
                x_memset(&rScaleParamV,0,sizeof(RZ_VDO_SCL_PARAM_SET_T));


                // Use rest memory of original frame buffer
                FBM_GetFrameBufferAddr(prFrcPrm->ucFbgId, prFrcPrm->ucFbId, &u4AddrY, &u4AddrC);
                u4YSize = prFbmPicHdr->u4PicWidthPitch * u4HeightAlign;

                if ((u4AddrY > 0) && (u4AddrC > 0))
                {
                    u4YResizeAddr = (u4AddrY + (UINT32)u4YSize);
                    u4YResizeAddr = IMG_RESIZE_ALIGN(u4YResizeAddr);
                    u4CResizeAddr = (u4AddrC + (UINT32)(u4YSize/2));
                    u4CResizeAddr = IMG_RESIZE_ALIGN(u4CResizeAddr);
                }

                //pu4PrevResizeWidth = &(prB2rVar->u4PreviousResizeWidth);
                //pu4PrevResizeHeight = &(prB2rVar->u4PreviousResizeHeight);

                if (prFbmPicHdr->fgCropping)
                {
                    u4PicX = prFbmPicHdr->u4CropX;
                    u4PicY = prFbmPicHdr->u4CropY;
                    u4PicWidth = prFbmPicHdr->u4CropWidth;
                    u4PicHeight = prFbmPicHdr->u4CropHeight;
                }
                else
                {
                    u4PicX = 0;
                    u4PicY = 0;
                    u4PicWidth = prFbmPicHdr->u4PicWidth;
                    u4PicHeight = prFbmPicHdr->u4PicHeight;
                }

                if ((u4PicWidth > 0) && (u4PicHeight > 0) && (u4YResizeAddr) && (u4CResizeAddr))
                {
                    if (prFbmSeqHdr->fgRasterOrder)
                    {
                        rScaleParamV.u4IsRsIn = 1;
                    }
                    else
                    {
                        rScaleParamV.u4IsRsIn = 0;
                    }
                    rScaleParamV.u4InMode = E_RZ_VDO_OUTMD_420;
                    rScaleParamV.u4YSrcBase = u4AddrY;
                    rScaleParamV.u4YSrcBufLen = prFbmPicHdr->u4PicWidthPitch;
                    rScaleParamV.u4YSrcHOffset = u4PicX;
                    rScaleParamV.u4YSrcVOffset = u4PicY;
                    rScaleParamV.u4YSrcW = u4PicWidth;
                    rScaleParamV.u4YSrcH = u4PicHeight;
                    rScaleParamV.u4CSrcBase = u4AddrC;
                    rScaleParamV.u4CSrcHOffset = u4PicX >> 1;
                    rScaleParamV.u4CSrcVOffset = u4PicY >> 1;
                    rScaleParamV.u4CSrcW = u4PicWidth >> 1;
                    rScaleParamV.u4CSrcH = u4PicHeight >> 1;

                    if (prFbmSeqHdr->fgRasterOrder)
                    {
                        rScaleParamV.u4IsRsOut = 1;
                    }
                    else
                    {
                        rScaleParamV.u4IsRsOut = 0;
                    }
                    rScaleParamV.u4OutMode = E_RZ_VDO_OUTMD_420;
                    rScaleParamV.u4IsVdo2Osd = 0;
                    rScaleParamV.u4YTgBase = u4YResizeAddr;
                    rScaleParamV.u4YTgCM = E_RZ_VDO_OUTMD_420;
                    rScaleParamV.u4YTgHOffset = 0;
                    rScaleParamV.u4YTgVOffset = 0;

                    if (prFbmPicHdr->u4PicWidth < FBM_IMG_RESIZE_LIMITATION)
                    {
                        rScaleParamV.u4YTgW = FBM_IMG_RESIZE_LIMITATION;
                    }
                    else if (prFbmPicHdr->u4PicWidth > FBM_IMG_RESIZE_LIMITATION_MAX)
                    {
                        rScaleParamV.u4YTgW = FBM_IMG_RESIZE_LIMITATION_MAX;
                    }
                    else
                    {
                        rScaleParamV.u4YTgW = prFbmSeqHdr->u2HSize&(0xfff0);
                    }

                    if (prFbmPicHdr->u4PicHeight < FBM_IMG_RESIZE_LIMITATION)
                    {
                        rScaleParamV.u4YTgH = FBM_IMG_RESIZE_LIMITATION;
                    }
                    else if (prFbmPicHdr->u4PicHeight > FBM_IMG_RESIZE_LIMITATION_MAX)
                    {
                        rScaleParamV.u4YTgH = FBM_IMG_RESIZE_LIMITATION_MAX;
                    }
                    else
                    {
                        rScaleParamV.u4YTgH = prFbmSeqHdr->u2VSize;
                    }
                    rScaleParamV.u4YTgBufLen= prFbmSeqHdr->u2LineSize;

                    rScaleParamV.u4CTgBase = (UINT32)u4CResizeAddr;
                    if (rScaleParamV.u4YTgW&0x1)
                    {
                        rScaleParamV.u4YTgW--;
                    }

                    if (rScaleParamV.u4YTgH&0x1)
                    {
                        rScaleParamV.u4YTgH--;
                    }

                    rScaleParamV.u4CTgW=rScaleParamV.u4YTgW>>1;
                    rScaleParamV.u4CTgH=rScaleParamV.u4YTgH>>1;

                    if (prFbmSeqHdr->fgRasterOrder)
                    {
                        rScaleParamV.u4OutSwap = 0;
                        rScaleParamV.u4SrcSwap = 0;
                    }
                    else
                    {
                        rScaleParamV.u4OutSwap = 6;
                        rScaleParamV.u4SrcSwap = 6;
                    }
                    
                    IMGRZ_Lock_Ex(E_FIRST_RESIZER);
                    IMGRZ_ReInit_Ex(E_FIRST_RESIZER);
                    IMGRZ_SetScaleOpt_Ex(E_FIRST_RESIZER,E_RZ_INOUT_TYPE_VDOMD);
                    IMGRZ_Scale_Ex(E_FIRST_RESIZER,(void *)(&rScaleParamV));
                    IMGRZ_Flush_Ex(E_FIRST_RESIZER);
                    IMGRZ_Wait_Ex(E_FIRST_RESIZER);
                    IMGRZ_Unlock_Ex(E_FIRST_RESIZER);
                    
                    u4AddrY = u4YResizeAddr;
                    u4AddrC = u4CResizeAddr;
                }

            }
            else if (prFbmPicHdr->fgSeamlessDisp && prFbmPicHdr->ucSeamlessResizeFbid != FBM_FB_ID_UNKNOWN)
            {
                //support 3d Mode
                #ifdef CC_3D_MM_DS_SUPPORT
                FBM_PIC_HDR_T* prRightPicHdr = FBM_GetFrameBufferPicHdr(prFrcPrm->ucSubFbgId, prFrcPrm->ucSubFbId);
                if (prRightPicHdr != NULL && (prRightPicHdr->ucSeamlessResizeFbid != FBM_FB_ID_UNKNOWN))
                {
                    FBM_GetResizeFrameBufferAddr(prFrcPrm->ucSubFbgId, prRightPicHdr->ucSeamlessResizeFbid, &u4AddrYSub, &u4AddrCSub);
                }
                #endif
                
                FBM_GetResizeFrameBufferAddr(prFrcPrm->ucFbgId, prFbmPicHdr->ucSeamlessResizeFbid, &u4AddrY, &u4AddrC);
                
            }
            else if (prFbmPicHdr->fgSeamlessDisp && prFbmPicHdr->ucSeamlessResize2Fbid != FBM_FB_ID_UNKNOWN)
            {
                FBM_GetResize2FrameBufferAddr(prFrcPrm->ucFbgId, prFbmPicHdr->ucSeamlessResize2Fbid, &u4AddrY, &u4AddrC);
            }
            else //default
            {
                //get default Y & C addr
                FBM_GetFrameBufferAddr(prFrcPrm->ucFbgId,prFrcPrm->ucFbId, &u4AddrY, &u4AddrC);
                if(prFbmPicHdr->fgUfoEn)
                {
                    FBM_GetFrameBufferAddrExt(prFrcPrm->ucFbgId,prFrcPrm->ucFbId, &u4AddrY_len, &u4AddrC_len);
                }
                #ifdef CC_3D_MM_DS_SUPPORT
                if (prFrcPrm->ucSubFbgId != FBM_FBG_ID_UNKNOWN && prFrcPrm->ucSubFbId != FBM_FB_ID_UNKNOWN)
                {
                    FBM_GetFrameBufferAddr(prFrcPrm->ucSubFbgId, prFrcPrm->ucSubFbId, &u4AddrYSub, &u4AddrCSub);
                    if(prFbmPicHdr->fgUfoEn)
                    {
                        FBM_GetFrameBufferAddrExt(prFrcPrm->ucSubFbgId, prFrcPrm->ucSubFbId, &u4AddrYSub_len, &u4AddrCSub_len);
                    }
                }
                #endif
            }

            #ifdef CC_3D_MM_DS_SUPPORT
            if (!(((prB2rVar->eSpeed == STC_SPEED_TYPE_FORWARD_1X) || (prB2rVar->eSpeed != STC_SPEED_TYPE_FORWARD_1X && prFrcPrm->u1B2R3DTrickModeType == B2R_3D_TRICK_MODE_3D_VIEW))
                    && (prFrcPrm->fgB2R3DEnable)
                    && (prFrcPrm->u1B2R3DType == B2R_3D_FRAME_SEQUENTIAL)))
            {
                u4AddrYSub = u4AddrY;
                u4AddrCSub = u4AddrC;
                u4AddrYSub_len = u4AddrY_len;
                u4AddrCSub_len = u4AddrC_len;
            }
            #endif
            {
                B2R_HAL_ADDR_T tFrmAddr;
                  
                tFrmAddr.u4Y0 = u4AddrY;
                tFrmAddr.u4C0 = u4AddrC;
                #ifdef CC_3D_MM_DS_SUPPORT
                tFrmAddr.u4Y1 = u4AddrYSub;
                tFrmAddr.u4C1 = u4AddrCSub;
                #endif
                B2R_HAL_Set(this->hB2r, B2R_HAL_ADDR, &tFrmAddr);
                if(prFbmPicHdr->fgUfoEn)
                {
                    B2R_HAL_ADDR_LEN_T tFrmAddr_len;
                    
                    tFrmAddr_len.u4YLen0 = u4AddrY_len;
                    tFrmAddr_len.u4CLen0 = u4AddrC_len;
                    #ifdef CC_3D_MM_DS_SUPPORT
                    tFrmAddr_len.u4YLen1 = u4AddrYSub_len;
                    tFrmAddr_len.u4CLen1 = u4AddrCSub_len;
                    #endif
                    B2R_HAL_Set(this->hB2r, B2R_HAL_ADDR_LEN, &tFrmAddr_len);
                }
            }
        }
    }while (FALSE);

}

static UINT32 _B2R_HandleStepForward(B2R_OBJECT_T* this)
{
    B2R_PRM_T* prFrcPrm;
    UCHAR ucB2rId;
    

    if (!this)
    {
        return B2R_INV_ARG;
    }

    ucB2rId = this->ucB2rId;
    prFrcPrm = this->ptB2rPrm;

    /* Step Forward at 20081030 */
    if ((_B2R_GetStepForward(ucB2rId) == VDP_PLAY_STEP_FORWARD))
    {
        if (prFrcPrm->ucRealChangeFb == 0)
        {
            LOG(3, "STEP MM wait STC Queue 1 Frame\n");
        }
        else
        {
            LOG(3, "STEP Real change FB PAUSE\n");
            /* Play One Frame and then Pause */
            _B2R_SetPauseMM(ucB2rId, TRUE);

            if (FBM_ChkFrameBufferPicFlag(prFrcPrm->ucFbgId, prFrcPrm->ucFbId, FBM_MM_PSEUDO_EOS_FLAG))
            {
                _B2R_PostNotify(this, VDP_MSG_MM_STEP_FIN_CB, 0, VDP_SEEK_STEP_NO_DATA);
            }
            else
            {
                _B2R_PostNotify(this, VDP_MSG_MM_STEP_FIN_CB, 0, VDP_SEEK_STEP_OK);
            }
        }
    }

    return B2R_OK;
}

//-----------------------------------------------------------------------------
/** Brief of _B2R_FrmInit.
 */
//-----------------------------------------------------------------------------
static UINT32 _B2R_FrmInit(UCHAR ucB2rId, UCHAR ucPort, BOOL fgCreateThread)
{
    B2R_PRM_T rVdpPrm;
    UCHAR  ucVdpId;
    B2R_PRM_T *  ptVdpPrm;
    B2R_VAR_T*  prB2rVar;
    VDP_CFG_T* prVdpConf;
    B2R_OBJECT_T *this;
    B2R_HANDLE_T hB2r;
    B2R_HAL_TIMING_T tTiming;
    
    UNUSED(ucPort);

    if (ucB2rId >= B2R_NS)
    {
        LOG(1, "[%s-%d]Input ucB2rId(%d) error!.\n", __func__, __LINE__, ucB2rId);
        ASSERT(ucB2rId < B2R_NS);
        return B2R_INV_ARG;
    }
    ucVdpId  = _B2R_GetVdpId(ucB2rId);
    this     = _B2R_GetObj(ucB2rId);
    if(!this)
    {
        return B2R_INV_ARG;
    }
    
    ptVdpPrm = this->ptB2rPrm;
    prB2rVar = this->ptB2rVar;

    prVdpConf = _B2R_GetDP(this);

    // backup those we don't want to reset
    //rVdpPrm = _arVdpPrm[ucB2rId];//TBD
    x_memcpy(&rVdpPrm, ptVdpPrm, sizeof(B2R_PRM_T));
    prB2rVar->u4MaxDriftCount = 0;
    prB2rVar->u4DriftCount = 0;
    prB2rVar->u4ValidCount = 0;
    prB2rVar->ucFrameRateUnkownCount = 0x0;
    prB2rVar->ucMirrorStatus = INVALID_MIRROR_STATUS;
    //prB2rVar->u4PreviousResizeHeight = 0;
    //prB2rVar->u4PreviousResizeHeight2 = 0;
    //prB2rVar->u4PreviousResizeWidth = 0;
    //prB2rVar->u4PreviousResizeWidth2 = 0;
    prB2rVar->fgRepeatFrame = FALSE;
    prB2rVar->fgDropFrame = FALSE;
    prB2rVar->fgModeChangeSent = FALSE;
    prB2rVar->u4LastHeight = 0;
    prB2rVar->u4LastWidth = 0;
    prB2rVar->u4RepeatCntForPushTrickMode = 0;
    prB2rVar->u4ElapseCntForPushTrickMode = 0;
    prB2rVar->ucRepeatFbForPushTrickMode = FBM_FB_ID_UNKNOWN;
    prB2rVar->fgSeqChgSpeedup = FALSE;
    prB2rVar->fgEnableVideoLog = FALSE;
    prB2rVar->rB2rStc.u4Speed = B2R_SPEED_TYPE_FORWARD_1X;
    x_memset(&(prB2rVar->rAB), 0, sizeof(VDP_AB_INTO_T));
    x_memset(&(prB2rVar->arLockBuf), 0, sizeof(VDP_LOCK_PIC_INFO_T)*VDP_MAX_LOCK_BUF_NS);
    prB2rVar->fgDoubleClock = 0;
    FBM_RegCbFunc(FBM_CB_FUNC_FB_READY_IND, (UINT32)_B2R_FrcFrameBufferReady);
    
#ifndef CC_SUPPORT_NPTV_SEAMLESS 
    FBM_RegCbFunc(FBM_CB_FUNC_SEQ_CHG_PRE_CB, (UINT32)_B2R_ChkSequenceInfo);
#endif

#ifdef CC_B2R_RES_SUPPORT
    FBM_RegCbFunc(FBM_CB_FUNC_B2R_HW_CHG_CB, (UINT32)_B2R_HwResChgNotify);
#endif

#ifdef CC_3D_MM_DS_SUPPORT
    if (ptVdpPrm->ucSubFbgId != FBM_FBG_ID_UNKNOWN && ptVdpPrm->ucSubFbId != FBM_FB_ID_UNKNOWN)
    {
        _B2R_SetFrameBufferStatus(this, ptVdpPrm->ucSubFbgId, ptVdpPrm->ucSubFbId, FBM_FB_STATUS_EMPTY);
    }
    if (ptVdpPrm->ucSubFbgId != FBM_FBG_ID_UNKNOWN && ptVdpPrm->ucSubReleaseFbId != FBM_FB_ID_UNKNOWN)
    {
        _B2R_SetFrameBufferStatus(this, ptVdpPrm->ucSubFbgId, ptVdpPrm->ucSubReleaseFbId, FBM_FB_STATUS_EMPTY);
    }
    if (ptVdpPrm->ucSubFbgId != FBM_FBG_ID_UNKNOWN && ptVdpPrm->ucSubPendingFbId != FBM_FB_ID_UNKNOWN)
    {
        _B2R_SetFrameBufferStatus(this,ptVdpPrm->ucSubFbgId, ptVdpPrm->ucSubPendingFbId, FBM_FB_STATUS_EMPTY);
    }
#endif

    FRC_MEMSET((void *)(ptVdpPrm), 0, sizeof(B2R_PRM_T));

    ptVdpPrm->ucB2rId = ucB2rId;

    // avoid the first start TV
    if(_ucFrcMuteInitiated)
    {
        _vDrvVideoSetMute(MUTE_MODULE_DTV, ucVdpId, FOREVER_MUTE, FALSE); 
    }
    
#if 0//def CC_SUPPORT_TVE
    if (fgCreateThread)
    {
        _vDrvVideoSetMute(MUTE_MODULE_DTV, ucB2rId, FOREVER_MUTE, FALSE);  
    }
    else
    {
        _vDrvVideoSetMute(MUTE_MODULE_DTV, ucVdpId, FOREVER_MUTE, FALSE);  
    }
#else
//    _VDP_HalForceBg(ucB2rId, 1);
#endif
#ifdef CC_B2R_SUPPORT_GAME_MODE
    VdpInitreleaseDiaplayQ();
#endif
    if (prVdpConf)
    {
        // Output Info, can not be init again
        ptVdpPrm->ucOutFrameRate = prVdpConf->rOutInfo.ucFrameRate;
        ptVdpPrm->ucProgressiveOut = prVdpConf->rOutInfo.ucPrg;
    }
    if (ptVdpPrm->ucOutFrameRate)
    {
        ptVdpPrm->u4OutStcPeriod =
            (UINT32)(VDP_STC_CLOCK / ptVdpPrm->ucOutFrameRate);
    }
    else
    {
        ptVdpPrm->u4OutStcPeriod = (VDP_STC_CLOCK / 60);
    }

#ifdef CC_B2R_WAIT_NPTV_STABLE
    prB2rVar->u4WaitNPTVStableCount = 0;
    prB2rVar->fgNPTVStable = FALSE;
#endif

    // default value
    prB2rVar->eSpeed = STC_SPEED_TYPE_FORWARD_1X;

#ifdef CC_3D_MM_DS_SUPPORT
    ptVdpPrm->ucSubFbgId = FBM_FBG_ID_UNKNOWN;
    ptVdpPrm->ucSubFbId = FBM_FB_ID_UNKNOWN;
    ptVdpPrm->ucSubReleaseFbId = FBM_FB_ID_UNKNOWN;
    ptVdpPrm->ucSubPendingFbId = FBM_FB_ID_UNKNOWN;
    ptVdpPrm->ucSubFbgIdRecorded = FBM_FBG_ID_UNKNOWN;
#endif

    ptVdpPrm->ucFbgId = FBM_FBG_ID_UNKNOWN;
    ptVdpPrm->ucFbId = FBM_FB_ID_UNKNOWN;
    ptVdpPrm->ucReleaseFbId = FBM_FB_ID_UNKNOWN;
    ptVdpPrm->ucPendingFbId = FBM_FB_ID_UNKNOWN;
    ptVdpPrm->ucLastFbId = FBM_FB_ID_UNKNOWN;
    ptVdpPrm->fgReleaseDispQ= FALSE;
    ptVdpPrm->fgSeqChg= FALSE;
    ptVdpPrm->ucLastAcsFrame = ~0;
    ptVdpPrm->ucLastAcsAuto = ~0;

    //ptVdpPrm->u4CbSyncEn = rVdpPrm.u4CbSyncEn;
    //ptVdpPrm->u4CbSyncThrsd= rVdpPrm.u4CbSyncThrsd;
    //ptVdpPrm->u4CbSyncChkFrmCnt= rVdpPrm.u4CbSyncChkFrmCnt;
    ptVdpPrm->ptRtCli = rVdpPrm.ptRtCli;
    ptVdpPrm->prFbCounter = &(_prDbase->rVdp.arFbCounter[ucB2rId][0]);
    ptVdpPrm->u4B2rSkipFrameNo = 0;
    //ptVdpPrm->fgSeqChg =  FALSE;
    prB2rVar->rFbmAspectRatio.u1AspectRatioType = MPEG_ASPECT_RATIO_4_3;
    prB2rVar->rFbmAspectRatio.u2AspectHorizSize = 0;
    prB2rVar->rFbmAspectRatio.u2AspectVertSize =0;
    
    FRC_MEMSET((void *)ptVdpPrm->prFbCounter, 0, sizeof(PARAM_VDP_FB_COUNTER_T));

#ifdef CC_MAPLE_CUST_DRV
    ptVdpPrm->pt_cust_info = rVdpPrm.pt_cust_info;
    if(!ptVdpPrm->pt_cust_info)
    {
        ptVdpPrm->pt_cust_info =
            (B2R_CUST_INFO_T*)x_mem_alloc(sizeof(B2R_CUST_INFO_T));
    }
#endif

    if(ptVdpPrm->ptRtCli)
    {
        x_memset(ptVdpPrm->ptRtCli, 0x0, sizeof(B2R_RT_CLI_T));
    }
    
#ifdef CC_B2R_ENABLE_CHG_FRAME_MSG
    //_B2R_FlushB2RChgFrameMsg(ucB2rId);
#endif

    // for _VdpCheckOutputMode, if ucPmxMode != VDP_B2R_MODE_UNKNOWN, no output change
    //[DTV00129735] DTV SCART out consideration, not to reset PMX mode
    if (prVdpConf)
    { 
        prVdpConf->rOutInfo.ucPmxMode = VDP_B2R_MODE_UNKNOWN;
    }

#ifdef DIVX_PLUS_CER
#ifdef   CC_B2R_CHAPTER_CHG
    if (!fgFirstSetChapter)
    {
        x_memset(&_arVdpChapter,0,sizeof(VDP_CHAPTER_T));
    }
    else
    {
        fgFirstSetChapter = FALSE;
    }
#endif
#endif

    hB2r = this->hB2r;
    x_memset(&tTiming, 0x0, sizeof(B2R_HAL_TIMING_T));
    B2R_HAL_Set(hB2r, B2R_HAL_TIMING, &tTiming);
    
    _B2R_PostNotify(this, VDP_MSG_UNMUTE_CB, 0, 0);

    return B2R_OK;

}

//-----------------------------------------------------------------------------
/** Brief of _VdpChangeFrameBuffer.
 */
//-----------------------------------------------------------------------------
static void _B2R_ChangeFrameBuffer(B2R_OBJECT_T* this)
{
    UCHAR ucAvSyncMode;
    UCHAR ucStcSrc;
    UCHAR ucOriginalFlow = 1;
    UCHAR ucB2rId;
    UCHAR ucPlayMode;
    B2R_VAR_T* prB2rVar;
    VDP_CFG_T* prVdpConf;
    B2R_PRM_T* prFrcPrm;
    
    do 
    {
        if (!this)
        {
            break;
        }
        ucB2rId = this->ucB2rId;
        if (ucB2rId >= B2R_NS)
        {
            LOG(0, "ucB2rId >= B2R_NS , oops \n");
            break;
        }
        prVdpConf = _B2R_GetDP(this);
        if (!prVdpConf)
        {
            LOG(0, "[%s-%d]VdpId error!.\n", __func__, __LINE__);
            break;
        }
        prFrcPrm = this->ptB2rPrm;
        if (FBM_CheckFbg(prFrcPrm->ucFbgId))
        {
            break;
        }
        prB2rVar = this->ptB2rVar;
        if (prFrcPrm->ucFbgId != FBM_FBG_ID_UNKNOWN)
        {
            UCHAR ucFrcMode;
            UINT16 u2OldTargetNs;
            UCHAR u2OldChangeFieldNs;

            // release last frame buffer (next ISR)
            // if we release it here, tearing will happen since this frame buffer is still displaying
            if (FBM_GetFrameBufferNs(prFrcPrm->ucFbgId) > 1)
            {
#ifdef CC_B2R_SUPPORT_GAME_MODE

             if(eGameMode[ucB2rId]==B2R_GAME_MODE_LOW_DELAY)
             {
                 vdpSetFrameToRelease(prFrcPrm->ucFbId);
             }
             else
             {
                prFrcPrm->ucReleaseFbId = prFrcPrm->ucFbId;
             }
#else
                prFrcPrm->ucReleaseFbId = prFrcPrm->ucFbId;
#endif
#ifdef CC_3D_MM_DS_SUPPORT
                prFrcPrm->ucSubReleaseFbId = prFrcPrm->ucSubFbId;
#endif
            }

            // backup for fail
            u2OldTargetNs = prFrcPrm->u2TargetNs;
            u2OldChangeFieldNs = prFrcPrm->u2ChangeFieldNs;

            if (prB2rVar->fgEnableWaitSTC)
            {
#ifdef ENABLE_MULTIMEDIA
                UCHAR ucEsId;
                VDEC_ES_INFO_T *prVdecEsInfo = NULL;
                ucEsId = prVdpConf->ucInputPort[0];
                prVdecEsInfo = (ucEsId < VDEC_MAX_ES)? _VDEC_GetEsInfo(ucEsId) : NULL;
#endif
                /* 20081113 MM start, STC will be updated by AUDIO but not yet updated */
                FBM_GetSyncStc(prFrcPrm->ucFbgId, &ucAvSyncMode, &ucStcSrc);
                if ((prFrcPrm->ucReady == 0) &&
                        ((ucAvSyncMode == VID_SYNC_MODE_SLAVE_MM)
#ifdef ENABLE_MULTIMEDIA
                         || (prVdecEsInfo && prVdecEsInfo->eContainerType == SWDMX_FMT_MPEG2_TS_TIME_SHIFT)
#endif
                        ))
                {
                    UCHAR ucPreLookFbId;
                    FBM_PIC_HDR_T* prPreLookNextPicHdr;
                    STC_CLOCK rStcClk;

                    ucPreLookFbId = _B2R_PreLookNextDisp(prFrcPrm);
                    if (ucPreLookFbId != FBM_FB_ID_UNKNOWN)
                    {
                        if (STC_GetSrc(ucStcSrc, &rStcClk) == STC_VALID)
                        {
                            UINT32 u4Delta;

                            prPreLookNextPicHdr = FBM_GetFrameBufferPicHdr(prFrcPrm->ucFbgId, ucPreLookFbId);
                            if (prPreLookNextPicHdr != NULL)
                            {
                                if ((prPreLookNextPicHdr->u4PTS) > (rStcClk.u4Base))
                                {
                                    u4Delta = ((prPreLookNextPicHdr->u4PTS) - (rStcClk.u4Base));

                                    if (u4Delta >= ((UINT32)0x80000000))
                                    {
                                        // handle wrap around
                                        u4Delta = (0xFFFFFFFF - u4Delta);
                                    }
                                }
                                else
                                {
                                    u4Delta = ((rStcClk.u4Base) - (prPreLookNextPicHdr->u4PTS));

                                    if (u4Delta >= ((UINT32)0x80000000))
                                    {
                                        // handle wrap around
                                        u4Delta = (0xFFFFFFFF - u4Delta);
                                    }
                                }
                                LOG(3, "--- PTS(%X), STC(%X), Diff(%d), OutStcPeriod(%d)\n",
                                    (prPreLookNextPicHdr->u4PTS), rStcClk.u4Base,
                                    u4Delta, prFrcPrm->u4OutStcPeriod);

                                if (u4Delta > STC_PTS_DIFF)
                                {
                                    ucOriginalFlow = 0;
                                }

                            }
                        }
                    }
                }
                /* 20081113 MM start, STC will be updated by AUDIO but not yet updated */
            }

            _B2R_SearchFrameInDisplayQ(this);//for seek 
            LOG(5,"_B2R_ChangeFrameBuffer: ucOriginalFlow=%d,u4OnOff =%d,ucReady=%d,u4Recovery=%d,fgPauseMM=%d\n", \
                ucOriginalFlow, prB2rVar->rVdpDtvFreeze.u4OnOff,prFrcPrm->ucReady,prB2rVar->rVdpDtvFreeze.u4Recovery,\
                prB2rVar->fgPauseMM);

            if (ucOriginalFlow == 1)
            {
                if (((prB2rVar->rVdpDtvFreeze.u4OnOff == 0) ||
                        (prFrcPrm->ucReady == 0)) &&
                        (prB2rVar->rVdpDtvFreeze.u4Recovery == 0) &&
                        (prB2rVar->fgPauseMM == 0))
                {
                    _B2R_GetNextDisp(this);
                }
                else
                {
                    prFrcPrm->u2TargetNs = 0;
                }
                
            }
            else
            {
                prFrcPrm->u2TargetNs = 0;
            }
            

            LOG(5,"_B2R_ChangeFrameBuffer: Fbg=%d, Fb=%d,u2TargetNs=%d\n", prFrcPrm->ucFbgId,prFrcPrm->ucFbId,prFrcPrm->u2TargetNs);
             prFrcPrm->u4FrameCount++;
			if(prFrcPrm->u4FrameCount>=prFrcPrm->ucInFrameRate)
			{
				 prFrcPrm->u4FrameCount=0;
				 FBM_FrameDisplayStart(prFrcPrm->ucFbgId, prFrcPrm->ucFbId);
			}
            if ((prFrcPrm->u2TargetNs != 0) &&
                    (prFrcPrm->ucFbId != FBM_FB_ID_UNKNOWN))
            {
                BOOL fgFastForward;
                UINT32 u4AddrY;
                UINT32 u4AddrC;

                
                // update VSYNC Ns
                FRC_SET_VSYNC_NS(prFrcPrm, prFrcPrm->u2TargetNs, prFrcPrm->u2ChangeFieldNs);

                if ((prFrcPrm->ucWrongField) || (prFrcPrm->ucMultipleField))
                {
                    ucFrcMode = VDP_FRC_NONE;
                }
                else
                {
                    ucFrcMode = VDP_FRC_NORMAL;
                }

                prFrcPrm->ucStopAuto = 0;

                if (prFrcPrm->ucNewFbArrive != 0)
                {
                    prFrcPrm->ucNewFbArrive = 0;

                    // Display Fail Count -1
                    if (prFrcPrm->prFbCounter)
                    {
                        if (prFrcPrm->prFbCounter->u4DispFail > 0)
                        {
                            prFrcPrm->prFbCounter->u4DispFail--;
                            //LOG(3, "DispFail--\n");
                        }
                        ucFrcMode = VDP_FRC_CANCEL;
                    }
                }

#ifdef CC_VDP_NO_INTERLACE_FRC
                if ((getScalerMode(prVdpConf->ucVdpId) != 0) ||
                    (prVdpConf->ucBobMode != 0))
                {
                    ucFrcMode = VDP_FRC_NONE;
                }
#endif

                /* Step Forward at 20081030 */
                _B2R_HandleStepForward(this);

                if (ucB2rId < B2R_NS)
                {
                    if (prB2rVar->eSpeed <= STC_SPEED_TYPE_FORWARD_1_DOT_5X)
                    {
                        fgFastForward = TRUE;
                    }
                    else
                    {
                        fgFastForward = FALSE;
                    }
                }
                else
                {
                    fgFastForward = TRUE;
                }
                UNUSED(fgFastForward);

                if (!prB2rVar->rVdpSeek.fgABReach)
                {
                    if (FBM_ChkFrameBufferPicFlag(prFrcPrm->ucFbgId, prFrcPrm->ucFbId, FBM_MM_PSEUDO_EOS_FLAG))
                    {
                        FBM_ClrFrameBufferPicFlag(prFrcPrm->ucFbgId, prFrcPrm->ucFbId, FBM_MM_PSEUDO_EOS_FLAG);

                        if (!prB2rVar->rAB.fgValid)  // if in AB repeat mode, no need to notify EOS
                        {
                            _B2R_PostNotify(this, VDP_MSG_MM_CB, VDP_COND_EOS, FBM_GetDecoderSrcId(prFrcPrm->ucFbgId));
                            LOG(3, "FBM_MM_PSEUDO_EOS_FLAG Notify\n");

                        }
                        else
                        {
                            _B2R_PostNotify(this, VDP_MSG_MM_CB, VDP_COND_RANGE_AB_DONE, fgFastForward);
                            LOG(3, "VDP_COND_RANGE_AB_DONE Notify\n");
                        }
                    }
                }

                /* Trick Play at 20081001 */
                FBM_GetSyncStc(prFrcPrm->ucFbgId, &ucAvSyncMode, &ucStcSrc);

     // Display Success should not set Frame Repeat in Trick mode
                // for CR DTV00144143
                //UCHAR ucPlayMode;
                FBM_GetPlayMode(prFrcPrm->ucFbgId,&ucPlayMode);
                if ( FBM_FBG_MM_MODE == ucPlayMode )
                {
                    static UINT32 u4PrePTS = 0xFFFFFFFF;
                    if (prFrcPrm->u4Pts == u4PrePTS)
                    {
                        ucFrcMode = VDP_FRC_TRICK_PLAY;
                    }
                    u4PrePTS = prFrcPrm->u4Pts;
                    LOG(10,"display success : Pts(%x),Fbid(%d)\n",prFrcPrm->u4Pts,prFrcPrm->ucFbId);
                }

                #if 1 //move from _vdp_HalSetFrc
                //if (_B2R_GetTrickMode(ucB2rId)==VDP_PLAY_TRICK)
                if(_B2R_GetFrcStcSpeed(ucB2rId) != STC_SPEED_TYPE_FORWARD_2X
                        && _B2R_GetTrickMode(ucB2rId)==VDP_PLAY_TRICK
                        && prFrcPrm->fgB2R3DEnable)
                {
                    ucFrcMode = VDP_FRC_TRICK_PLAY;
                }
                #endif

                
                _B2R_SetFrc(this, prFrcPrm->u2TargetNs, prFrcPrm->u2ChangeFieldNs, ucFrcMode);

                // update frame buffer addr
                FBM_GetFrameBufferAddr(prFrcPrm->ucFbgId,prFrcPrm->ucFbId, &u4AddrY, &u4AddrC);
                
                _B2R_SeamlessJob(this);

                _B2R_ColorFmtMonitor(this);
                //prFrcPrm->u4FrameRepeated = 0;
                //prFrcPrm->u4FrameSuccess++;

#ifdef CC_VDP_NO_INTERLACE_FRC
                prFrcPrm->u2DisplayFailNs = 0;
#endif
            }
            else
            {
                LOG(10,"display fail : Pts(%x),Fbid(%d)\n",prFrcPrm->u4Pts,prFrcPrm->ucLastFbId);
                // Pending New FB (MPEG) at get display Q fail, except
                // 1) not ready
                // 2) freeze
                // 3) Fail Count < 2 (ST, u4FrameRepeated @W[, Ý­n Pending New FB)
                // 4) Success >= 4 (sí©›í³°T~Ò° Pending, OI, PÉ¥]tF Fail Count < 2) (prFrcPrm->u4FrameSuccess >= 4)
                // 5) ...
                if ((prFrcPrm->ucReady != 0) &&
                        (prB2rVar->rVdpDtvFreeze.u4OnOff == 0) &&
                        (prFrcPrm->ucMMWaitSTC != 1))
                {
                    if ((FBM_ChkFrameBufferFlag(prFrcPrm->ucFbgId, FBM_FLAG_DEBLOCKING)) &&
                            (FBM_GetFrameBufferNs(prFrcPrm->ucFbgId) <= 6))
                    {
                        // 5FB Deblocking
                        prFrcPrm->ucPendingNewFb = 1;
                    }
                    else if (FBM_ChkFrameBufferFlag(prFrcPrm->ucFbgId, FBM_FLAG_THREE_B))
                    {
                        prFrcPrm->ucPendingNewFb = 1;
                    }

                    prFrcPrm->ucPendingNewFb = 1;
                }

#ifdef CC_B2R_WAIT_NPTV_STABLE
                if (prB2rVar->u4WaitNPTVStableCount > 0)
                {
                    prFrcPrm->ucPendingNewFb = 0;
                }
#endif

                if (prFrcPrm->ucNewFbArrive != 0)
                {
                    prFrcPrm->ucNewFbArrive = 0;

                    // possible
                    // SS: againg test with power on/off.
    //                ASSERT(0);  // possible?
                }
                if (prB2rVar->rVdpDtvFreeze.u4OnOff != 0)
                {
                    if (prB2rVar->rVdpDtvFreeze.u4CleanDispQ == 0)
                    {
                        prB2rVar->rVdpDtvFreeze.u4CleanDispQ = 1;
                    }
                }

                // Visual Quality Tuning
                // Handle display next fail
                // T B T B B B T B (Old)
                //         ^^^ Fail
                // T B T B T B T B (New)
                //         ^^^ Fail

                // restore
                prFrcPrm->ucReleaseFbId = FBM_FB_ID_UNKNOWN;
                prFrcPrm->u2TargetNs = u2OldTargetNs;
#ifdef CC_3D_MM_DS_SUPPORT
                prFrcPrm->ucSubReleaseFbId = FBM_FB_ID_UNKNOWN;
#endif

                //// TODO, system crash

                //prFrcPrm->u4FrameSuccess = 0;
                /*if (((prFrcPrm->u4FrameRepeated < 2) &&
                        (_fgPauseMM[ucB2rId] == 0) &&
                        (prFrcPrm->ucMMWaitSTC != 1)) || fgMainMuteOn())*/

                
                
                /* MM Pause or MM pending for AVS-R also STOP AUTO */
                prFrcPrm->u2ChangeFieldNs = u2OldTargetNs;

                /* 2-3 pulldown sequence. Repeat on 3-field display frame
                   To avoid B2R repeat count as odd number. */
                if ((prFrcPrm->ucWrongField) || (prFrcPrm->ucMultipleField))
                {
                    if ((prFrcPrm->u2TargetNs == 3) && (prFrcPrm->ucProgressiveOut == 0))
                    {
                        prFrcPrm->u2TargetNs = 2;
                        prFrcPrm->u2ChangeFieldNs = 2;
                    }
                }

                prFrcPrm->ucMultipleField = 0;

                ucFrcMode = VDP_FRC_STOP_AUTO;

                prFrcPrm->ucStopAuto = 1;
                

                // Recovery from mpeg freeze to normal playback
                if (prB2rVar->rVdpDtvFreeze.u4Recovery != 0)
                {
                    UINT32 u4NextPts;

                    prB2rVar->rVdpDtvFreeze.u4Recovery = 0;

                    u4NextPts = FBM_GetNextDispQPts(prFrcPrm->ucFbgId);

                    if (u4NextPts != 0)
                    {
                        FBM_GetSyncStc(prFrcPrm->ucFbgId, &ucAvSyncMode, &ucStcSrc);

#ifndef CC_VDP_FPGA
                        if (ucAvSyncMode != VID_SYNC_MODE_NONE)
                        {
                            STC_CLOCK rStcClk;

                            if (STC_GetSrc(ucStcSrc, &rStcClk) == STC_VALID)
                            {
                                INT32 i4Delta;
                                UINT32 u4Delta;

                                if (u4NextPts > rStcClk.u4Base)
                                {
                                    u4Delta = (u4NextPts - rStcClk.u4Base);

                                    if (u4Delta < ((UINT32)0x80000000))
                                    {
                                        i4Delta = - (INT32) u4Delta;
                                    }
                                    else
                                    {
                                        // handle wrap around
                                        u4Delta = (0xFFFFFFFF - u4Delta);
                                        i4Delta = (INT32) u4Delta;
                                    }
                                }
                                else
                                {
                                    u4Delta = (rStcClk.u4Base - u4NextPts);

                                    if (u4Delta < ((UINT32)0x80000000))
                                    {
                                        i4Delta = (INT32) u4Delta;
                                    }
                                    else
                                    {
                                        // handle wrap around
                                        u4Delta = (0xFFFFFFFF - u4Delta);
                                        i4Delta = - (INT32) u4Delta;
                                    }
                                }

                                if (i4Delta < 0)
                                {
                                    // PTS slower than STC, repeat

                                    UINT32 u4OutFrameStc;
                                    UINT32 u4Step;

                                    u4OutFrameStc = (prFrcPrm->u2TargetNs * prFrcPrm->u4OutStcPeriod);

                                    if (u4OutFrameStc != 0)
                                    {
                                        u4Step = (u4Delta / u4OutFrameStc);

                                        if (u4Step != 0)
                                        {
                                            prFrcPrm->u2ChangeFieldNs *= u4Step;
                                            prFrcPrm->u2TargetNs *= u4Step;

                                            LOG(3, "Freeze-R %d PTS(0x%x) STC(0x%x)\n", prFrcPrm->u2TargetNs, u4NextPts, rStcClk.u4Base);
                                        }
                                    }
                                }
                                else
                                {
                                    // imposible !
                                }
                            }
                        }
#endif
                    }
                }

#ifdef CC_VDP_NO_INTERLACE_FRC

                if (getScalerMode(prVdpConf->ucVdpId) != 0)
                {
                    ucFrcMode = VDP_FRC_NONE;
                }

                // Display Mode ON & Interlace Output mode
                if ((prB2rVar->rVdpDtvFreeze.u4OnOff == 0) &&(getScalerMode(prVdpConf->ucVdpId) != 0) &&(prFrcPrm->ucProgressiveOut == 0))
                {
                    if (prFrcPrm->u2DisplayFailNs != VDP_DISPLAY_FAIL_FREEZE)
                    {
                        prFrcPrm->u2DisplayFailNs += prFrcPrm->u2TargetNs;

                        // Fail > 1/4 Second
                        if (prFrcPrm->u2DisplayFailNs >= (prFrcPrm->ucOutFrameRate >> 2))
                        {
                            LOG(1,"Display Fail Start Gfx Copy Field\n");

                            prFrcPrm->u2DisplayFailNs = VDP_DISPLAY_FAIL_FREEZE;
                        }
                    }
                }
#endif

                // update VSYNC Ns
                FRC_SET_VSYNC_NS(prFrcPrm, prFrcPrm->u2TargetNs, prFrcPrm->u2ChangeFieldNs);



                /* Trick Play at 20081001 */
                FBM_GetSyncStc(prFrcPrm->ucFbgId, &ucAvSyncMode, &ucStcSrc);
                if ((_B2R_GetTrickMode(ucB2rId)==VDP_PLAY_TRICK) && (ucAvSyncMode==VID_SYNC_MODE_NONE))
                {
                    //ucFrcMode = VDP_FRC_TRICK_PLAY;
                }


                #if 1 //move from _vdp_HalSetFrc
                //if (_B2R_GetTrickMode(ucB2rId)==VDP_PLAY_TRICK)
                if(_B2R_GetFrcStcSpeed(ucB2rId) != STC_SPEED_TYPE_FORWARD_2X
                        && _B2R_GetTrickMode(ucB2rId)==VDP_PLAY_TRICK
                        && prFrcPrm->fgB2R3DEnable)
                {
                    ucFrcMode = VDP_FRC_TRICK_PLAY;
                }
                #endif

                _B2R_SetFrc(this, prFrcPrm->u2TargetNs, prFrcPrm->u2ChangeFieldNs, ucFrcMode);

                // release current frame buffer until new buffer is ready
                prFrcPrm->ucReleaseFbId = FBM_FB_ID_UNKNOWN;
#ifdef CC_3D_MM_DS_SUPPORT
                prFrcPrm->ucSubReleaseFbId = FBM_FB_ID_UNKNOWN;
#endif
            }

            if ((prFrcPrm->ucFbId != FBM_FB_ID_UNKNOWN) && (prFrcPrm->u2TargetNs != 0))
            {
                LOG(11, "%d %d %d Ns(%d) (%d, %d) (%d %d %d)\n",
                    ucB2rId, prFrcPrm->ucFbgId, prFrcPrm->ucFbId,
                    prFrcPrm->u2TargetNs, prFrcPrm->u4InFrameStc, prFrcPrm->u4OutFrameStc,
                    prFrcPrm->ucRff, prFrcPrm->ucTff, prFrcPrm->i4TemporalRef);

                LOG_REL(prB2rVar->fgEnableVideoLog, 2, 9, "B2R(%d) NF (%d %d) Ns(%d, %d) B(%d) TFF(%d) FRC(%d) R(%d)\n",
                        ucB2rId,
                        prFrcPrm->ucFbId, prFrcPrm->i4TemporalRef,
                        prFrcPrm->u2TargetNs, prFrcPrm->u2ChangeFieldNs,
                        prFrcPrm->ucBottom, prFrcPrm->ucTff,
                        ucFrcMode, prFrcPrm->ucReleaseFbId);

#ifdef CC_VDP_FRC_DEBUG
                LOG(4, "NS(%d)TR(%d)FR(%d)T/RFF(%d,%d)PTS(0x%06x)DP(%d)STC(0x%06x)D(%d)\n",
                    prFrcPrm->u2TargetNs,
                    prFrcPrm->i4TemporalRef,
                    prFrcPrm->ucInFrameRate,
                    prFrcPrm->ucTff,
                    prFrcPrm->ucRff,
                    prFrcPrm->u4Pts,
                    (prFrcPrm->u4Pts - prFrcPrm->u4PtsBackup),
                    prFrcPrm->u4Stc,
                    prFrcPrm->i4PtsStcDelta);

                prFrcPrm->u4PtsBackup = prFrcPrm->u4Pts;
#endif
#ifdef ENABLE_MULTIMEDIA
                _B2R_RT_TMS_Statistics(this);
#endif
            }
        }
    }while (FALSE);
}

#ifdef DIVX_PLUS_CER
#ifdef   CC_B2R_CHAPTER_CHG
VOID _B2R_FrcSetChapterEnable(VDP_CHAPTER_T*    prChapter)
{
    x_memcpy(&_arVdpChapter, prChapter, sizeof(VDP_CHAPTER_T) );
    if (_arVdpChapter.fgFirstSetChapter)
    {
        fgFirstSetChapter = TRUE;
    }
    _arVdpChapter.fgIsSetEndPts = TRUE;
    _arVdpChapter.fgPending     = FALSE;
    LOG(3, "ChapterEnable: PTS[%lld, %lld] IsFirst(%d).\n", 
        _arVdpChapter.u8StartPts, _arVdpChapter.u8EndPts, _arVdpChapter.fgFirstSetChapter);
    return;
}

static VDP_CHAPTER_T* _B2R_FrcGetChapterObj(VOID)
{
    return &_arVdpChapter;
}
#endif
#endif

#ifdef CC_MAPLE_CUST_DRV
INT32 _B2R_Get_Cust_info(UCHAR              ucB2rId,
                              B2R_CUST_INFO_T*    pt_cust_info)
{
    INT32           i4Ret    = B2R_OK;
    B2R_PRM_T*      ptVdpPrm = NULL;
    B2R_OBJECT_T*   this     = NULL;
    
    do
    {
        if(ucB2rId >= B2R_NS || 
            !pt_cust_info)
        {
            i4Ret = B2R_INV_ARG;
            break;
        }

        this = _B2R_GetObj(ucB2rId);

        if(!this)
        {
            i4Ret = B2R_INTERNAL_ERR;
            break;
        }
        
        ptVdpPrm = this->ptB2rPrm;
        if(!ptVdpPrm ||
            !ptVdpPrm->pt_cust_info)
        {
            i4Ret = B2R_INTERNAL_ERR;
            break;
        }

        x_memcpy(pt_cust_info,ptVdpPrm->pt_cust_info,sizeof(B2R_CUST_INFO_T));
    }while(0);

    return i4Ret;
}

INT32 _B2R_GetAspInfo(UCHAR     ucB2rId,
                           UINT8*    pu1_asp_info)
{
    INT32           i4Ret     = B2R_OK;
    UINT8           u1AspInfo = 0;
    UINT32          u4DAR     = 0;
    VDP_CFG_T*     prVdpConf = NULL;
    B2R_OBJECT_T*   this      = NULL;
    

    do
    {
        if(ucB2rId >= B2R_NS ||
            !pu1_asp_info)
        {
            i4Ret = B2R_INV_ARG;
            break;
        }

        this = _B2R_GetObj(ucB2rId);

        if(!this)
        {
            i4Ret = B2R_INTERNAL_ERR;
            break;
        }

        prVdpConf = _B2R_GetDP(this);
        if(!prVdpConf)
        {
            i4Ret = B2R_INTERNAL_ERR;
            break;
        }

        u1AspInfo = (UINT8)prVdpConf->u4AspectRatio;
        if ((u1AspInfo != MPEG_ASPECT_RATIO_4_3) &&
         (u1AspInfo != MPEG_ASPECT_RATIO_16_9))
        {
            if (prVdpConf->u4AspectRatioHeight)
            {
                u4DAR = 100 * prVdpConf->u4AspectRatioWidth/prVdpConf->u4AspectRatioHeight;
            }
            else
            {
                u4DAR = 100*16/9;
            }
            if (((u4DAR *3) >> 2) < 110)
            {
                u1AspInfo = MPEG_ASPECT_RATIO_4_3;
            }
            else if (((u4DAR *9) >> 4) < 110)
            {
                u1AspInfo = MPEG_ASPECT_RATIO_16_9;
            }
            else
            {
                u1AspInfo = MPEG_ASPECT_RATIO_16_9;
            }
        }

        *pu1_asp_info = u1AspInfo;
    
    }while(0);

    return i4Ret;
}

INT32 _B2R_Cust_info_cfg(VDP_CFG_T*   prVdpConf,
                              B2R_PRM_T*    ptVdpPrm)
{
    INT32   i4Ret    = B2R_OK;

    do
    {
        if(!prVdpConf || !ptVdpPrm)
        {
            i4Ret = B2R_INV_ARG;
            break;
        }
        
        if(ptVdpPrm->pt_cust_info)
        { 
            UCHAR                   ucEsId;
            FBM_PIC_HDR_T*          prHdr        = NULL;
            VDEC_ES_INFO_T*         prVdecEsInfo = NULL;
            FBM_CODEC_RESOLUTION_T  t_res;
            

            ucEsId = prVdpConf->ucInputPort[0];
            prVdecEsInfo = (ucEsId < VDEC_MAX_ES)? _VDEC_GetEsInfo(ucEsId) : NULL;
            if(prVdecEsInfo)
            {
                ptVdpPrm->pt_cust_info->eMMSrcType = prVdecEsInfo->eMMSrcType;
            }
            else if(FBM_GetFbgAppMode(ptVdpPrm->ucFbgId) == FBM_FBG_APP_MTIMAGE)
            {
                ptVdpPrm->pt_cust_info->eMMSrcType = SWDMX_SRC_TYPE_HIGH_SPEED_STORAGE;
            }

            prHdr = FBM_GetFrameBufferPicHdr(ptVdpPrm->ucFbgId, ptVdpPrm->ucFbId);
            if(prHdr)
            {
                 ptVdpPrm->pt_cust_info->eColorID = prHdr->eColorSpace;
            }

            ptVdpPrm->pt_cust_info->eCodecInfo = 
                FBM_GetCodecInfo(ptVdpPrm->ucFbgId);

            if(FBM_GetCodecResolution(ptVdpPrm->ucFbgId,&t_res))
            {
                x_memcpy(&(ptVdpPrm->pt_cust_info->rResolution),
                    &t_res,sizeof(FBM_CODEC_RESOLUTION_T));
            }
        }
    }while(0);

    return i4Ret;
}
#endif

static BOOL _B2R_IsProg(B2R_OBJECT_T *this, FBM_SEQ_HDR_T* prSeqHdr)
{
    UCHAR ucVal, ucPlayMode;
    B2R_PRM_T *  prFrcPrm;
    
    if (!prSeqHdr || !this)
    {
        return TRUE;
    }

    ucVal    = prSeqHdr->fgProgressiveSeq;
    prFrcPrm = this->ptB2rPrm;

    FBM_GetPlayMode(prFrcPrm->ucFbgId, &ucPlayMode);
    if ((ucVal == 0) &&
            ((prSeqHdr->ucFrmRatCod == MPEG_FRAME_RATE_24_) ||
             (prSeqHdr->ucFrmRatCod == MPEG_FRAME_RATE_24) ||
             (prSeqHdr->ucFrmRatCod == MPEG_FRAME_RATE_50) ||
             (prSeqHdr->ucFrmRatCod == MPEG_FRAME_RATE_60_) ||
             (prSeqHdr->ucFrmRatCod == MPEG_FRAME_RATE_60)) &&
             (prFrcPrm->ucProgressiveFrame!= 0) &&
             (ucPlayMode == FBM_FBG_MM_MODE))
    {
        LOG(0, "new check Interlace Seq but Progressive Pic\n");
        ucVal = 1;
    }

    return ucVal ? TRUE : FALSE;

}

static VOID _B2R_ConvFps(UCHAR ucRatCod, UINT32 *pu4Rate, UINT32 *u4Rate)
{
    if (!pu4Rate || !u4Rate)
    {
        return;
    }
    
    switch (ucRatCod)
    {
    case MPEG_FRAME_RATE_12:
        *pu4Rate = 12;
        *u4Rate  = 1;        
        break;

    case MPEG_FRAME_RATE_15:
        *pu4Rate = 15;
        *u4Rate  = 1;        
        break;
        
#ifdef SUPPORT_DISP_WEB_VIDEO_SMOOTHER
    case MPEG_FRAME_RATE_20:
        *pu4Rate = 20;
        *u4Rate  = 1;        
        break;
#endif
    case MPEG_FRAME_RATE_24_:
#ifdef SUPPORT_DISP_FILM22_48TO120
        *pu4Rate = 48000;
#else
        *pu4Rate = 24000;
#endif
        *u4Rate  = 1001;        
        break;
    case MPEG_FRAME_RATE_24:
#ifdef SUPPORT_DISP_FILM22_48TO120
        *pu4Rate = 48;//change from 24 to 48
#else
        *pu4Rate = 24;
#endif
        *u4Rate  = 1;        
        break;
    case MPEG_FRAME_RATE_25:
        *pu4Rate = 25;
        *u4Rate  = 1;        
        break;
    case MPEG_FRAME_RATE_30_:
        *pu4Rate = 30000;
        *u4Rate  = 1001;        
        break;
    case MPEG_FRAME_RATE_30:
        *pu4Rate = 30;
        *u4Rate  = 1;        
        break;
    case MPEG_FRAME_RATE_50:
        *pu4Rate = 50;
        *u4Rate  = 1;        
        break;
    case MPEG_FRAME_RATE_60_:
        *pu4Rate = 60000;
        *u4Rate  = 1001;        
        break;
    case MPEG_FRAME_RATE_60:
        *pu4Rate = 60;
        *u4Rate  = 1;        
        break;
    case MPEG_FRAME_RATE_UNKNOWN:
    default:
        *pu4Rate = 30;
        *u4Rate  = 1;        
        break;
    }

}

static INT32 _B2R_ColorFmtGen(B2R_OBJECT_T *this,
                              FBM_SEQ_HDR_T* ptSeqHdr,
                              B2R_HAL_COLOR_FMT_T* ptColorFmt)
{
    INT32 i4Ret = B2R_OK;
    B2R_HAL_COLOR_FMT_T tColorFmt;
    
    do
    {
        if(!ptSeqHdr || !ptColorFmt || !this)
        {
            i4Ret = B2R_INV_ARG;
            break;
        }

        B2R_HAL_Get(this->hB2r, B2R_HAL_GET_COLOR_FMT, &tColorFmt);
        LOG(1,"org depth : %d!\n", tColorFmt.u1Depth);
        ptColorFmt->eMode = ptSeqHdr->fg422Mode ? B2R_CM_422 : B2R_CM_420;
        
        if(ptSeqHdr->fgSeamlessPlay &&
            !(ptSeqHdr->eSeamlessMode & SEAMLESS_BY_NPTV) &&
            tColorFmt.u1Depth != 10)
        {
            ptColorFmt->u1Depth = 8;
            ptColorFmt->fgUfoEn = FALSE;
        }
        else
        {
            ptColorFmt->u1Depth = ptSeqHdr->u1Depth ? ptSeqHdr->u1Depth : 8;
            ptColorFmt->fgUfoEn = ptSeqHdr->fgUfoEn;
        }
    }while(0);

    return i4Ret;
}

static VOID _B2R_SeqSetting(B2R_OBJECT_T *this, FBM_SEQ_HDR_T* prSeqHdr, UINT32 u4Width, UINT32 u4Height)
{
    B2R_HANDLE_T hB2r;
    B2R_HAL_3D_T t3D;
    B2R_HAL_TIMING_T tTiming, tOrgTiming;
    B2R_VAR_T* prB2rVar;
    VDP_CFG_T* prVdpConf;
    B2R_PRM_T *prFrcPrm=NULL;
    B2R_OUT_INFO_T * prOutInfo ;
    B2R_4K60FRC4K30_T t_force;
    UCHAR ucB2rId;
    UCHAR ucVdpId;
    UINT32 i;
    BOOL  fgTimingChg = TRUE;

    if (!this || !prSeqHdr)
    {
        return;
    }

    hB2r = this->hB2r;
    prB2rVar  = this->ptB2rVar;
    if(!prB2rVar)
    {
        return;
    }
    
    ucB2rId = this->ucB2rId;
    ucVdpId = _B2R_GetVdpId(ucB2rId);
    if (ucVdpId >= VDP_NS)
    {
        return;
    }
    //new b2r settings begin
    //3D type
    t3D.fg3D     = (prSeqHdr->fgB2R3DEnable && !prSeqHdr->fgB2RForce2D) ? TRUE : FALSE;
    t3D.u4Layout =  prSeqHdr->u1B2R3DType;
    B2R_HAL_Set(hB2r, B2R_HAL_3DTYPE, &t3D);

    //Timing 
    {
    __TIMING_TBL_T *ptTbl;
    BOOL fgProg,fgBrustRead;
    UINT32 u4Rate, u4Scale;

    fgProg  = _B2R_IsProg(this, prSeqHdr);
    _B2R_ConvFps(prSeqHdr->ucFrmRatCod, &u4Rate, &u4Scale);

    prVdpConf = _B2R_GetDP(this);
    if(!prVdpConf)
    {
        return;
    }
    
//#ifdef CC_B2R_RM_SUPPORT
  #if defined(CC_MT5890)
    if(fgIs4K2KInBypassPath())
	{
	    fgVB1Byass =TRUE;
	}
	else
	{
	    fgVB1Byass =FALSE;
	}
    LOG(1, "Video bypass enable or disable :u4Width %d, u4Height: %d,fgVB1Byass=%d\n", u4Width,u4Height,fgVB1Byass);
	fgPhotoRasterModeIn4k=prSeqHdr->fgRasterOrder;
    if((u4Width>1920||(u4Height>1088))&&fgVB1Byass&&!(prSeqHdr->fgRasterOrder))
    {
        BOOL fgByass=TRUE;
        B2R_HAL_Set(hB2r,B2R_HAL_VB1,&fgByass);
        LOG(1,"Omux  Set Bypass enable : %d!\n",fgByass);
        prVdpConf->fgBypssEnabe=TRUE;
    }
    else
    {
        BOOL fgByass=FALSE;
        B2R_HAL_Set(hB2r,B2R_HAL_VB1,&fgByass);
        LOG(1,"Omux  Set Bypass Disable : %d!\n",fgByass);
        prVdpConf->fgBypssEnabe=FALSE;
    }
    #endif
//#endif

    if(!prVdpConf->fgBypssEnabe) //This Process Will Remove After Oryx E3, Capri and Gazelle do not use
    {
        t_force.u4SeqWidth = u4Width;
        t_force.u4SeqHeight = u4Height;
        t_force.u4Rate = u4Rate;
        t_force.u4Scale = u4Scale;
        _B2R_4K60TimingChg(this, &t_force, prSeqHdr->ucFrmRatCod);
        u4Rate = t_force.u4Rate;
        u4Scale = t_force.u4Scale;
    }

    if (fgProg)
    {
        ptTbl = _b2r_query_P_timing(u4Width, u4Height, u4Rate, u4Scale);
    }
    else
    {
        ptTbl = _b2r_query_I_timing(u4Width, u4Height, u4Rate, u4Scale);
    }

    if(!ptTbl)
    {
        prB2rVar->u4ReturnValue &= ~VDP_EVENT_SEQ_CHG;
        LOG(0,"u4Width:0x%x, u4Height:0x%x, fgProgressive:%d!\n",u4Width, u4Height, prSeqHdr->fgProgressiveSeq);
        LOG(0,"ucFrmRatCod : %d!\n",prSeqHdr->ucFrmRatCod);
        return;
    }
    
    LOG(1, "Timing name: %s, 3D: %s\n", ptTbl->acName, t3D.fg3D ? "On":"Off");
 
    tTiming.fgProg   = fgProg ? 1 : 0;
    tTiming.u4HTotal = ptTbl->u2HTotal;
    tTiming.u4VTotal = ptTbl->u2VTotal;
    tTiming.u4Rate   = u4Rate;
    tTiming.u4Scale  = u4Scale;
    tTiming.u4SrcW   = u4Width;
    tTiming.u4SrcH   = u4Height;    
    tTiming.u4Clock  = t3D.fg3D ? (ptTbl->u4Clock << 1) : ptTbl->u4Clock;
    B2R_HAL_Get(hB2r, B2R_HAL_GET_TIMING, &tOrgTiming);

    fgTimingChg = _B2R_IsTimingChg(this, &tOrgTiming, &tTiming);
    
    if(fgTimingChg)
    {
        B2R_HAL_Disable(hB2r);
    }
    
    B2R_HAL_Set(hB2r, B2R_HAL_TIMING, &tTiming);
    if(u4Width <= BURST_READ_THRESHOLD)
    {
        fgBrustRead = FALSE;
        B2R_HAL_Set(hB2r,B2R_HAL_BURST_READ,&fgBrustRead);
    }
    }

    //dram layout
    {
    B2R_HAL_DRAM_LAYOUT_T tDramLayout;
    tDramLayout.eAddrMode = prSeqHdr->fgRasterOrder ? ADDR_MODE_RASTER : (prSeqHdr->ucBlockType ? B2R_AM_BLOCK1 : B2R_AM_BLOCK4);;
    tDramLayout.eSwapMode = _B2R_ConSwapMd(prSeqHdr->eSwapMode);
    tDramLayout.ePackMode = _B2R_ConBlkMd(prSeqHdr->ePackMode);
    tDramLayout.u4Pitch   = prSeqHdr->u2LineSize;
    B2R_HAL_Set(hB2r, B2R_HAL_DRAM_LAYOUT, &tDramLayout);
    LOG(1,"eAddrMode : %d , eSwapMode : %d , ePackMode : %d!\n",
        tDramLayout.eAddrMode,tDramLayout.eSwapMode,tDramLayout.ePackMode);
    }

    //color format
    {
    B2R_HAL_COLOR_FMT_T tColorFmt;
    _B2R_ColorFmtGen(this, prSeqHdr, &tColorFmt);
    B2R_HAL_Set(hB2r, B2R_HAL_COLOR_FMT, &tColorFmt);
    LOG(1,"u1Depth : %d , UFO Mode : %d!\n",tColorFmt.u1Depth, tColorFmt.fgUfoEn);
    }

   #if 0
    {
        B2R_HAL_OMUX_T tOmux = {0};
        
        tOmux.ucPath = ucVdpId;
        tOmux.fgScartOut = prVdpConf->fgScartOut;
        B2R_HAL_Set(hB2r,B2R_HAL_OMUX,&tOmux);
        LOG(1,"Omux B2rId : %d , VdpId : %d , ucPath : %d , fgScartOut : %d!\n",
                ucB2rId, ucVdpId, tOmux.ucPath, tOmux.fgScartOut);
    }
    #endif
    // B2R Output Mode Change
 
    prOutInfo = &(prVdpConf->rOutInfo);
    prFrcPrm  = this->ptB2rPrm;

    UNUSED(prOutInfo);
    UNUSED(prVdpConf); 
    UNUSED(prFrcPrm);

    prFrcPrm->ucNotSupport = 0;
    prB2rVar->u2VsyncNotStableCount = 0xFF;

    prOutInfo->ucPmxMode     = VDP_B2R_MODE_480I_60;

    prOutInfo->ucPrg         =  tTiming.fgProg;
    prOutInfo->ucFrameRate   = ((tTiming.u4Rate * 10)/tTiming.u4Scale + 5)/10;
    prOutInfo->ucFrameRate   = tTiming.fgProg ? prOutInfo->ucFrameRate : (prOutInfo->ucFrameRate << 1);
    prOutInfo->ucFrameRate   = t3D.fg3D? (prOutInfo->ucFrameRate<<1) : prOutInfo->ucFrameRate;
    prOutInfo->u4Width       = tTiming.u4SrcW;
    prOutInfo->u4Height      = tTiming.u4SrcH;
    prOutInfo->u4TotalWidth  = tTiming.u4HTotal;
    prOutInfo->u4TotalHeight = tTiming.u4VTotal;
    //prOutInfo->ucDoubleClk = 1;

    _B2R_SpecialTimingHdr(t3D.fg3D, &tTiming, prOutInfo);
    // Update vdp_frc parameter
    prFrcPrm->ucOutFrameRate   = prOutInfo->ucFrameRate;
    prFrcPrm->ucProgressiveOut = prOutInfo->ucPrg;

    if (prFrcPrm->ucOutFrameRate)
    {
        prFrcPrm->u4OutStcPeriod = VDP_STC_CLOCK / prFrcPrm->ucOutFrameRate;
    }
    else
    {
        prFrcPrm->u4OutStcPeriod = (VDP_STC_CLOCK / 60);
    }

    _B2R_StcPeriod(ucB2rId,prFrcPrm->u4OutStcPeriod);


    //update DPs parameters 
    for (i = 1; i < this->u4VideoPanelNs; i++)
    {
       if (this->avVideoPanel[i]!=NULL)
        {
            VDP_CFG_T * ptDP = (VDP_CFG_T *)this->avVideoPanel[i];
       
            ptDP->u4MpegHeight    = prVdpConf->u4MpegHeight;
            ptDP->u4MpegWidth     = prVdpConf->u4MpegWidth;
            ptDP->u4SrcLineSize   = prVdpConf->u4SrcLineSize;
            ptDP->u4SrcWidth      = prVdpConf->u4SrcWidth;
            ptDP->u4SrcHeight     = prVdpConf->u4SrcHeight;
            ptDP->u4FrameRate     = prVdpConf->u4FrameRate;
            ptDP->ucColourPrimary = prVdpConf->ucColourPrimary;
            ptDP->ucTransCharacter= prVdpConf->ucTransCharacter;
            ptDP->ucMatrixCoeff   = prVdpConf->ucMatrixCoeff;
            ptDP->ucColorDescrip  = prVdpConf->ucColorDescrip;
            ptDP->ucNoOverScan    = prVdpConf->ucNoOverScan;
            ptDP->u4FrmCropTop    = prVdpConf->u4FrmCropTop;
            ptDP->u4FrmCropBtm    = prVdpConf->u4FrmCropBtm;
            ptDP->fgCropping      = prVdpConf->fgCropping;
            ptDP->u4CropX         = prVdpConf->u4CropX;
            ptDP->u4CropY         = prVdpConf->u4CropY;
            ptDP->u4CropWidth     = prVdpConf->u4CropWidth;
            ptDP->u4CropHeight    = prVdpConf->u4CropHeight;
            ptDP->u4Afd           = prVdpConf->u4Afd;
            ptDP->u4AspectRatio   = prVdpConf->u4AspectRatio;
            ptDP->u4AspectRatioWidth  = prVdpConf->u4AspectRatioWidth;
            ptDP->u4AspectRatioHeight = prVdpConf->u4AspectRatioHeight;
            ptDP->u4AspectRatioIdc    = prVdpConf->u4AspectRatioIdc;

            x_memcpy((VOID *)&(ptDP->rOutInfo), (VOID *)&(prVdpConf->rOutInfo), sizeof(B2R_OUT_INFO_T));
        }
    }
    
    // notify SCPOS
  

    _B2R_PostNotify(this, VDP_MSG_NOTIFY, VDP_B2R_OUTPUT_CHG, 0);
    prB2rVar->u4ReturnValue |= VDP_EVENT_SEQ_CHG;
        
    LOG(0, "%s \n", __FUNCTION__);

 }

UINT16 VDP_Get_DAR_SAR_Type(VDP_CFG_T* prVdpConf)
{
    UINT16 u2AspecRatio;
    if((prVdpConf->u4AspectRatio==MPEG_ASPECT_RATIO_4_3)||(prVdpConf->u4AspectRatio==MPEG_ASPECT_RATIO_16_9)||(prVdpConf->u4AspectRatio==MPEG_ASPECT_RATIO_221_1))
    {
        u2AspecRatio = 1;//DAR type
    }
    else
    {
        u2AspecRatio = 0;//SAR
    }
    return u2AspecRatio;
      
}

#if defined(CC_MT5890) && defined(CC_SUPPORT_4KBE)
UINT32 VDP_SetVB1ControlBitTest(B2R_OBJECT_T *this,UINT32 u4SrcWidth,UINT32 u4SrcHeight) ///ONLY FOR TEST 
{
    CONTROL_BIT_VB1_T  VB1ControlBit;
 
    VB1ControlBit.u13DStructure=B2R_3D_NONE;
    VB1ControlBit.u1AspectRatio=16;
    VB1ControlBit.u1AspectType=1;//VDP_Get_DAR_SAR_Type(prVdpConf);

    VB1ControlBit.u1CodeType=4;//this->ptB2rPrm->pt_cust_info->eCodecInfo;
    VB1ControlBit.u1ColorFormat=0;//this->ptB2rPrm->pt_cust_info->eColorID;

    VB1ControlBit.u1CtrlBitValid=0;
    VB1ControlBit.u1DoubleRF=0;//prB2rVar->fgDoubleClock;
    VB1ControlBit.u1Range=0;
    VB1ControlBit.u1VideoPath=1;
    VB1ControlBit.u4CropHPosition = 0;//prVdpConf->u4CropX;
    VB1ControlBit.u4CropHSize=0;//prVdpConf->u4CropWidth;
    VB1ControlBit.u4CropVPosition= 0;//prVdpConf->u4CropY;
    VB1ControlBit.u4CropVSize=0;//prVdpConf->u4CropHeight;
    VB1ControlBit.u4PicHSize=u4SrcWidth;//prVdpConf->u4SrcWidth;
    VB1ControlBit.u4PicVSize=u4SrcHeight;//prVdpConf->u4SrcHeight;
    VB1ControlBit.u4SarHSize= 0;//prB2rVar->rFbmAspectRatio.u2AspectHorizSize;
    VB1ControlBit.u4SarVSize= 0;//prB2rVar->rFbmAspectRatio.u2AspectVertSize;
    vDrvUpdateVB1ControlBit(&VB1ControlBit);

    return TRUE;
}

UINT32 VDP_SetVB1ControlBit(B2R_OBJECT_T *this)
{
    CONTROL_BIT_VB1_T  VB1ControlBit;
    VDP_CFG_T* prVdpConf;
   // UCHAR ucEsId;
  //  VDEC_ES_INFO_T* prVdecEsInfo = NULL;
    B2R_VAR_T* prB2rVar;
  //  B2R_PRM_T *  prFrcPrm;
    
    if(!this)
    {
       return FALSE;
    }
    
    
    prVdpConf = _B2R_GetDP(this);
	
	if(!prVdpConf)
    {
        return FALSE;
    }
    prB2rVar = this->ptB2rVar;
	
	if(!prB2rVar)
    {
        return FALSE;
    }

    VB1ControlBit.u13DStructure=this->ptB2rPrm->u1B2R3DType;
    VB1ControlBit.u1AspectRatio=prVdpConf->u4AspectRatio;
    VB1ControlBit.u1AspectType=VDP_Get_DAR_SAR_Type(prVdpConf);
#ifdef CC_MAPLE_CUST_DRV
    VB1ControlBit.u1CodeType=this->ptB2rPrm->pt_cust_info->eCodecInfo;
    VB1ControlBit.u1ColorFormat=this->ptB2rPrm->pt_cust_info->eColorID;
#endif
    VB1ControlBit.u1CtrlBitValid=0;
    VB1ControlBit.u1DoubleRF=prB2rVar->fgDoubleClock;
    VB1ControlBit.u1Range=0;
    VB1ControlBit.u1VideoPath=1;
    VB1ControlBit.u4CropHPosition = prVdpConf->u4CropX;
    VB1ControlBit.u4CropHSize=prVdpConf->u4CropWidth;
    VB1ControlBit.u4CropVPosition= prVdpConf->u4CropY;
    VB1ControlBit.u4CropVSize=prVdpConf->u4CropHeight;
    VB1ControlBit.u4PicHSize=prVdpConf->u4SrcWidth;
    VB1ControlBit.u4PicVSize=prVdpConf->u4SrcHeight;
    VB1ControlBit.u4SarHSize= prB2rVar->rFbmAspectRatio.u2AspectHorizSize;
    VB1ControlBit.u4SarVSize= prB2rVar->rFbmAspectRatio.u2AspectVertSize;
    vDrvUpdateVB1ControlBit(&VB1ControlBit);

    return TRUE;
}
#endif

#ifndef CC_SUPPORT_NPTV_SEAMLESS 
static BOOL _B2R_ChkSequenceInfo(UCHAR ucFbgId)
{
    UCHAR ucEsId;
    UCHAR ucB2rId;
    UCHAR ucVdpIdx;
    VDP_CFG_T* prVdpConf;
    B2R_OBJECT_T* this;
    
    ucEsId = FBM_GetDecoderSrcId(ucFbgId);
    if(ucEsId >= MAX_ES_NS)
    {
        LOG(1, "[%s-%d]Fail\n", __func__, __LINE__);
        return FALSE;
    }
    
    for (ucVdpIdx = 0; ucVdpIdx < VDP_NS; ucVdpIdx++)
    {
        prVdpConf = _B2R_GetVdpConf(ucVdpIdx);
        if (prVdpConf &&
            prVdpConf->ucInputPort[0] == ucEsId)
        {
            break;
        }
    }

    if(!prVdpConf)
    {
        return FALSE;
    }

    ucB2rId = prVdpConf->ucB2rId;
    this = _B2R_GetObj(ucB2rId);
    if(!this)
    {
        return FALSE;
    }
    
    return _B2R_GetSequenceInfo(this, TRUE);  
}
#endif

static BOOL _B2R_GetSequenceInfo(B2R_OBJECT_T *this,BOOL fgPreChk)
{
    FBM_SEQ_HDR_T* prSeqHdr;
    BOOL fgFrmRateChg = FALSE;
    BOOL fg3DChg = FALSE;
    BOOL fgSeqChg = FALSE;
    BOOL fgResChg = FALSE;
    //UINT32 u4AddrMode;
    UINT32 u4SeqWidth;
    UINT32 u4SeqHeight;
    UINT32 u4SrcLineSize = 0;
    BOOL fgAfdChg = FALSE;
    B2R_VAR_T* prB2rVar;
    B2R_PRM_T *  prFrcPrm;
    VDP_CFG_T* prVdpConf;
    UCHAR ucB2rId;
    UCHAR ucVdpId;
	UCHAR ucEsId;
    VDEC_ES_INFO_T* prVdecEsInfo = NULL;
    
    if(!this)
    {
        return FALSE;
    }

    ucB2rId = this->ucB2rId;
    prB2rVar = this->ptB2rVar;
    prFrcPrm = this->ptB2rPrm;

    prFrcPrm->ucSeqInfoValid = 1;
    prVdpConf = _B2R_GetDP(this);

    if(!prVdpConf)
    {
        return FALSE;
    }
    ucVdpId = _B2R_GetVdpId(ucB2rId);
    // Prepare Input Frame Rate
    prSeqHdr = FBM_GetFrameBufferSeqHdr(prFrcPrm->ucFbgId);
    if (prSeqHdr == NULL)
    {
        return FALSE;
    }

	 if(prFrcPrm->fgSeqChg)
    {
        prVdpConf->u4SrcLineSize = prSeqHdr->u2LineSize;
        FBM_ClrFrameBufferFlag(prFrcPrm->ucFbgId, FBM_FLAG_SEQ_CHG);
        FBM_ClrFrameBufferFlag(prFrcPrm->ucFbgId, FBM_FLAG_3D_SEQ_CHG);
        FBM_ClrFrameBufferFlag(prFrcPrm->ucFbgId, FBM_FLAG_SEQ_CHG_SPEEDUP);
	
        LOG(2,"_B2R_GetSequenceInfo prSeqHdr->u2LineSize=%d\n",prSeqHdr->u2LineSize);
         return TRUE;
    }
	 
    if (prSeqHdr->ucFrmRatCod == MPEG_FRAME_RATE_UNKNOWN)
    {
        prSeqHdr->ucFrmRatCod = FBM_CalFrmRate(prFrcPrm->ucFbgId);
    }
    if ((prSeqHdr->ucFrmRatCod != MPEG_FRAME_RATE_UNKNOWN) && 
        (prVdpConf->rOutInfo.ucInputFrameRate != prSeqHdr->ucFrmRatCod))
    {
        fgFrmRateChg = TRUE;
    }

    u4SrcLineSize = prSeqHdr->u2LineSize;

#ifdef ENABLE_MULTIMEDIA
    LOG(1,"SeamlessPlay(%d),Mode(0x%x)!\n",prSeqHdr->fgSeamlessPlay, prSeqHdr->eSeamlessMode);
    if (prSeqHdr->fgSeamlessPlay) // seamless play
    {
        // ** For seamless mode, cropping is handled by seamless job, so don't care here
        u4SeqWidth = (UINT32) prSeqHdr->u4ResizeWidth;
        u4SeqHeight = (UINT32) prSeqHdr->u4ResizeHeight;
    }
    else 
#endif
    {
        // If cropping info is provided, the correct video size is identified by cropping size
        u4SeqWidth = (prSeqHdr->fgCropping  ? prSeqHdr->u4CropWidth : (UINT32)prSeqHdr->u2HSize);
        u4SeqHeight = (prSeqHdr->fgCropping ? prSeqHdr->u4CropHeight : (UINT32)prSeqHdr->u2VSize);
    }

	ucEsId = prVdpConf->ucInputPort[0];
    prVdecEsInfo = (ucEsId < VDEC_MAX_ES)? _VDEC_GetEsInfo(ucEsId) : NULL;

	if (prVdecEsInfo && (prVdecEsInfo->eSeamlessMode & SEAMLESS_BY_NPTV))
	  {
		  u4SeqWidth = (UINT32) 1920;//prSeqHdr->u4ResizeWidth;
		  u4SeqHeight = (UINT32)1080;// prSeqHdr->u4ResizeHeight;
	  }


    // because of the 420 sample method
    // progressive : the height should be a multiple of 2
    // interlace : the height should be a multiple of 4
    u4SeqHeight = (prSeqHdr->fgProgressiveSeq ? (u4SeqHeight&0xFFFFFFFE) : (u4SeqHeight&0xFFFFFFFC));

#ifdef CC_B2R_3D_SUPPROT
    if (!prSeqHdr->fgB2R3DEnable)
#endif
    {
		#ifdef CC_SUPPORT_PIPELINE
    	u4SeqWidth = ((u4SeqWidth > 1920) ? 1920 : u4SeqWidth);
		if((u4SeqWidth == 1920) &&(u4SeqHeight > 1080) )
		{
        	u4SeqHeight = 1080;
		}
		#else
		u4SeqWidth = ((u4SeqWidth > 3840) ? 3840 : u4SeqWidth);
		u4SeqHeight = ((u4SeqHeight > 2160) ? 2160 : u4SeqHeight);
		#endif
    }
    
#ifdef CC_B2R_3D_SUPPROT
    if (prSeqHdr->fgB2R3DEnable)
    {
        if (prSeqHdr->u1B2R3DType == B2R_3D_SIDE_BY_SIDE_REAL_D || prSeqHdr->u1B2R3DType == B2R_3D_SIDE_BY_SIDE)
        {
            u4SeqWidth/=2;
        }
        else if (prSeqHdr->u1B2R3DType == B2R_3D_TOP_N_BOTTOM)
        {
            u4SeqHeight/=2;
        }
    }
#endif

    if ((prVdpConf->u4SrcLineSize != (UINT32) u4SrcLineSize) ||
        (prB2rVar->u4OriSrcWidth != u4SeqWidth) ||
        (prB2rVar->u4OriSrcHeight != u4SeqHeight))
    {
        fgSeqChg = TRUE;
    }
    
    if ((prB2rVar->u4LastWidth != prSeqHdr->u2OrgWidth) ||
        (prB2rVar->u4LastHeight != prSeqHdr->u2OrgHeight))
    {
        fgResChg = TRUE;
        prB2rVar->u4LastWidth = prSeqHdr->u2OrgWidth;
        prB2rVar->u4LastHeight = prSeqHdr->u2OrgHeight;
    }

#ifdef CC_B2R_3D_SUPPROT
    if (prFrcPrm->fgB2R3DEnable != prSeqHdr->fgB2R3DEnable)   // Check 3D enable/disable changed
    {
        fg3DChg = TRUE;
    }
    else if (prFrcPrm->fgB2R3DEnable)                 // If 3D is enabled, check 3D configuration changed
    {
        if ((prFrcPrm->u1B2R3DType != prSeqHdr->u1B2R3DType) ||
            (prFrcPrm->fgB2RForce2D != prSeqHdr->fgB2RForce2D) ||
            (prFrcPrm->u1B2RForce2DType != prSeqHdr->u1B2RForce2DType) ||
            (prFrcPrm->u4B2R3DWidthOffset != prSeqHdr->u4B2R3DWidthOffset) ||
            (prFrcPrm->u4B2R3DHeightOffset != prSeqHdr->u4B2R3DHeightOffset))
        {
            fg3DChg = TRUE;
        }
    }
#endif // CC_B2R_3D_SUPPROT

    if ((prVdpConf->u4Afd != (UINT32) prSeqHdr->ucActFmt) ||
        (prVdpConf->u4AspectRatio != (UINT32) prSeqHdr->ucAspRatInf) ||
        ((prVdpConf->u4AspectRatio == MPEG_ASPECT_RATIO_TRANSMIT) &&
           ((prVdpConf->u4AspectRatioWidth != prSeqHdr->u4AspectRatioWidth) ||
           (prVdpConf->u4AspectRatioHeight != prSeqHdr->u4AspectRatioHeight))) ||
        (prVdpConf->u4AspectRatioIdc != prSeqHdr->u4AspectRatioIdc))
    {
        fgAfdChg = TRUE;
    }

    if (fgPreChk)
    {
        //Only pre-check the seq info chg or not. .
        //For VDP seamless pre check only the w,h change or not.
        if (fgResChg && !fg3DChg && !fgFrmRateChg)
        {
            #ifndef CC_SUPPORT_NPTV_SEAMLESS 
            _B2R_NPTVSeamlessStatus(this, VDP_SMLS_PREPARE);
            #endif
            LOG(1, "[%s-%d]H,V(%d-%d).Line(%d->%d) Org(%d-%d->%d-%d).\n", __func__, __LINE__, 
                prSeqHdr->u2HSize, prSeqHdr->u2VSize,
                prVdpConf->u4SrcLineSize, (UINT32) prSeqHdr->u2LineSize,
                prB2rVar->u4LastWidth, prB2rVar->u4LastHeight, prSeqHdr->u2OrgHSize, prSeqHdr->u2OrgVSize);
            return TRUE;
        }
        return FALSE;
    }
    
    _B2R_HandleResolutionChange(this);
    
#ifndef CC_SUPPORT_NPTV_SEAMLESS 
    if (prFrcPrm->ucSeamlessVDPMode)
    {
        
        prVdpConf->u4SrcLineSize = prSeqHdr->u2LineSize;
        LOG(1, "SeqChg VDP_Sml: P(%d) H,V(%d %d) Fr(%d) Asp(%d %d %d) 3D(%d).\n",
            prSeqHdr->u2LineSize, u4SeqWidth, u4SeqHeight, prFrcPrm->ucInFrameRate,
            prSeqHdr->ucAspRatInf, prSeqHdr->u4AspectRatioWidth, prSeqHdr->u4AspectRatioHeight, prSeqHdr->u1B2R3DType);
        
        _B2R_NPTVSeamlessStatus(this, VDP_SMLS_FRAME_COMING);
        FBM_ClrFrameBufferFlag(prFrcPrm->ucFbgId, FBM_FLAG_SEQ_CHG);
        FBM_ClrFrameBufferFlag(prFrcPrm->ucFbgId, FBM_FLAG_3D_SEQ_CHG);
        FBM_ClrFrameBufferFlag(prFrcPrm->ucFbgId, FBM_FLAG_SEQ_CHG_SPEEDUP);
        return TRUE;
    }
#endif

    if (fgSeqChg || fg3DChg || fgFrmRateChg)
    {
        // Update Sequence Parameter to hardware
        // do not update everything, since display mode will flash
        prB2rVar->u4ReturnValue |= VDP_EVENT_SEQ_CHG;
    }
    else // Optional Sequence Change
    {
        prB2rVar->u4ReturnValue |= VDP_EVENT_OPT_SEQ_CHG;
    }

    if (fgAfdChg && (prFrcPrm->ucReady ==1))
    {
        LOG(1, "Aspect Chg %d \n",prVdpConf->u4AspectRatio);
        _B2R_PostNotify(this, VDP_MSG_AFD_CB, (UINT32) prSeqHdr->ucActFmt,(UINT32) (& prB2rVar->rFbmAspectRatio));
    }
    
    // fps    : STC Period
    // 24    : 3750   // 25    : 3600  // 30    : 3000   // 50    : 1800   // 60    : 1500
#ifdef ENABLE_MULTIMEDIA
    if(_B2R_GetContentSourceType(this) == (UINT32)SWDMX_SRC_TYPE_WIFI_DISPLAY)
    {
        if(!prFrcPrm->t_wfd_prm.fgInitalized)
        {
            prFrcPrm->ucInFrameRate = _B2R_RTFrameRateConvert(prFrcPrm,prSeqHdr->ucFrmRatCod);
            if (prSeqHdr->ucFrmRatCod == MPEG_FRAME_RATE_UNKNOWN)
            {
                prSeqHdr->ucFrmRatCod = MPEG_FRAME_RATE_60;
            }
        }
    }
    else
#endif
    {
        prFrcPrm->ucInFrameRate = _B2R_ConvertFrameRateInx2FrameRate(prSeqHdr->ucFrmRatCod);
    }

    prFrcPrm->u4InStcPeriodBase    = VDP_STC_CLOCK / prFrcPrm->ucInFrameRate;
    prFrcPrm->ucProgressiveSeqFlag = prSeqHdr->fgProgressiveSeq;
#ifdef CC_B2R_3D_SUPPROT
    prFrcPrm->fgB2RForce2D = prSeqHdr->fgB2RForce2D;
    prFrcPrm->u1B2RForce2DType = prSeqHdr->u1B2RForce2DType;
    prFrcPrm->fgB2R3DEnable = prSeqHdr->fgB2R3DEnable;
    prFrcPrm->u1B2R3DType = prSeqHdr->u1B2R3DType;
    prFrcPrm->u4B2R3DWidthOffset = prSeqHdr->u4B2R3DWidthOffset;
    prFrcPrm->u4B2R3DHeightOffset = prSeqHdr->u4B2R3DHeightOffset;
    prFrcPrm->u1B2R3DTrickModeType = prSeqHdr->u1B2R3DTrickModeType;
#endif
    prB2rVar->rFbmAspectRatio.u1AspectRatioType = prSeqHdr->ucAspRatInf;
    prB2rVar->rFbmAspectRatio.u2AspectHorizSize = prSeqHdr->u4AspectRatioWidth;
    prB2rVar->rFbmAspectRatio.u2AspectVertSize = prSeqHdr->u4AspectRatioHeight;
    prB2rVar->u4OriSrcWidth = prVdpConf->u4SrcWidth;
    prB2rVar->u4OriSrcHeight = prVdpConf->u4SrcHeight;
    prB2rVar->fgInvalidFrmRate = (prSeqHdr->ucFrmRatCod == MPEG_FRAME_RATE_UNKNOWN) ? TRUE : FALSE;
    prB2rVar->fgSeqChgSpeedup = (BOOL)(FBM_ChkFrameBufferFlag(prFrcPrm->ucFbgId, FBM_FLAG_SEQ_CHG_SPEEDUP));

    // Update Sequence Parameter to Vdp_If
    // Store MPEG Size
    prVdpConf->u4MpegHeight = prSeqHdr->u2OrgVSize;
    prVdpConf->u4MpegWidth  = prSeqHdr->u2OrgHSize;
    prVdpConf->u4SrcLineSize = prSeqHdr->u2LineSize;
    prVdpConf->u4SrcWidth = u4SeqWidth;
    prVdpConf->u4SrcHeight = u4SeqHeight;
    prVdpConf->u4FrameRate = prFrcPrm->ucInFrameRate;
    prVdpConf->ucColourPrimary = prSeqHdr->ucColourPrimaries;
    prVdpConf->ucTransCharacter = prSeqHdr->ucTransCharacter;
    prVdpConf->ucMatrixCoeff = prSeqHdr->ucMatrixCoeff;
    prVdpConf->ucColorDescrip = prSeqHdr->fgColorDescrip;
    prVdpConf->ucNoOverScan = prSeqHdr->fgNoOverScan;
    prVdpConf->u4FrmCropTop = prSeqHdr->u4FrmCropTop;
    prVdpConf->u4FrmCropBtm = (u4SeqHeight - prSeqHdr->u4FrmCropBtm);
    prVdpConf->fgCropping = prSeqHdr->fgCropping;
    prVdpConf->u4CropX = prSeqHdr->u4CropX;
    prVdpConf->u4CropY = prSeqHdr->u4CropY;
    prVdpConf->u4CropWidth = prSeqHdr->u4CropWidth;
    prVdpConf->u4CropHeight = prSeqHdr->u4CropHeight;
    prVdpConf->u4Afd = (UINT32) prSeqHdr->ucActFmt;
    prVdpConf->u4AspectRatio = (UINT32) prSeqHdr->ucAspRatInf;
    prVdpConf->u4AspectRatioWidth = prSeqHdr->u4AspectRatioWidth;
    prVdpConf->u4AspectRatioHeight = prSeqHdr->u4AspectRatioHeight;
    prVdpConf->u4AspectRatioIdc = prSeqHdr->u4AspectRatioIdc;
    prVdpConf->rOutInfo.ucInputFrameRate = prSeqHdr->ucFrmRatCod;

    if ((prVdpConf->u4SrcHeight == 1088) &&
            ((prVdpConf->u4FrmCropBtm - prVdpConf->u4FrmCropTop) == 1080))
    {
        vDrvDISetDtvClip(ucVdpId, prVdpConf->u4FrmCropTop);
    }
    
    FBM_ClrFrameBufferFlag(prFrcPrm->ucFbgId, FBM_FLAG_SEQ_CHG);
    FBM_ClrFrameBufferFlag(prFrcPrm->ucFbgId, FBM_FLAG_3D_SEQ_CHG);
    FBM_ClrFrameBufferFlag(prFrcPrm->ucFbgId, FBM_FLAG_SEQ_CHG_SPEEDUP);

    //_B2R_CheckOutputMode(this);//LIJIA
    //new hal interface
    _B2R_SeqSetting(this, prSeqHdr, u4SeqWidth, u4SeqHeight);

    LOG(1, "SeqChg %d H,V(%d %d) Fr(%d) Asp(%d %d %d) 3D(%d).\n",
        prSeqHdr->u2LineSize, u4SeqWidth, u4SeqHeight, prFrcPrm->ucInFrameRate,
        prSeqHdr->ucAspRatInf, prSeqHdr->u4AspectRatioWidth, prSeqHdr->u4AspectRatioHeight, prSeqHdr->u1B2R3DType);

    LOG(1, "prog flag %d %d \n", prFrcPrm->ucProgressiveSeqFlag, prFrcPrm->ucProgressiveFrame );

    if (prB2rVar->fgSeqChgSpeedup)
    {
#ifdef TIME_MEASUREMENT
        TMS_DIFF_EX(TMS_FLAG_CHG_CHL, TMS_CHL_CHE_TIME_DRV, "B2R Set Start Play(S)");
#endif
        _B2R_PostNotify(this, VDP_MSG_NOTIFY, VDP_B2R_START_PLAY,0);
        prB2rVar->fgModeChangeSent = TRUE;
    }

    return TRUE;
}

#ifdef CC_3D_MM_DS_SUPPORT
static VOID _B2R_GetFrameBufferForMVC3D(B2R_OBJECT_T* this)
{
    B2R_PRM_T* prFrcPrm;
    B2R_VAR_T* prB2rVar;
    FBM_PIC_HDR_T* prLeftPicHdr = NULL;
    FBM_PIC_HDR_T* prRightPicHdr = NULL;
    BOOL fgEos = FALSE;

    if (!this)
    {
        return;
    }

    prFrcPrm = this->ptB2rPrm;
    prB2rVar = this->ptB2rVar;
    // pick L/R frame and check L/R sync behavior
    if (FBM_CheckFrameBufferDispQ(prFrcPrm->ucFbgId) && FBM_CheckFrameBufferDispQ(prFrcPrm->ucSubFbgId))
    {
        UINT8 u1LeftFbId = FBM_PreLookFrameBufferFromDispQ(prFrcPrm->ucFbgId);
        UINT8 u1RightFbId = FBM_PreLookFrameBufferFromDispQ(prFrcPrm->ucSubFbgId);

        prRightPicHdr = FBM_GetFrameBufferPicHdr(prFrcPrm->ucSubFbgId, u1RightFbId);
        prLeftPicHdr = FBM_GetFrameBufferPicHdr(prFrcPrm->ucFbgId, u1LeftFbId);

        if (prRightPicHdr != NULL && prLeftPicHdr != NULL)
        {
            if ((prRightPicHdr->u4PTS > prLeftPicHdr->u4PTS))
            {
                while ((prRightPicHdr->u4PTS > prLeftPicHdr->u4PTS))
                {
                    if (((prRightPicHdr->u4PTS - prLeftPicHdr->u4PTS) > 500) && ((prRightPicHdr->u4PTS - prLeftPicHdr->u4PTS) < 180000))
                    {
                        LOG_REL(prB2rVar->fgEnableVideoLog, 1, 3, "LRSync prelook: Drop L frame pts(%d) TempRef(%d) \n",prLeftPicHdr->u4PTS,prLeftPicHdr->i4TemporalRef);
                        _B2R_FreeFrameBeforeEosHandler(this, TRUE, FALSE, &fgEos);
                        if (fgEos)
                        {
                            break;
                        }

                        if (FBM_CheckFrameBufferDispQ(prFrcPrm->ucFbgId))
                        {
                            u1LeftFbId = FBM_PreLookFrameBufferFromDispQ(prFrcPrm->ucFbgId);
                            prLeftPicHdr = FBM_GetFrameBufferPicHdr(prFrcPrm->ucFbgId, u1LeftFbId);
                            if (prLeftPicHdr == NULL)
                            {
                                break;
                            }
                        }
                        else
                        {
                            break;
                        }
                    }
                    else if ((prRightPicHdr->u4PTS - prLeftPicHdr->u4PTS) >= 180000)
                    {
                        if (FBM_CheckFrameBufferDispQ(prFrcPrm->ucFbgId) > 1)
                        {
                            LOG_REL(prB2rVar->fgEnableVideoLog, 1, 3, "LRSync prelook: Drop L frame pts(%d) TempRef(%d) \n",prLeftPicHdr->u4PTS,prLeftPicHdr->i4TemporalRef);
                            _B2R_FreeFrameBeforeEosHandler(this, TRUE, FALSE, &fgEos);
                        }
                        break;
                    }
                    else
                    {
                        break;
                    }
                }
            }
            else
            {
                while ((prLeftPicHdr->u4PTS > prRightPicHdr->u4PTS))
                {
                    if (((prLeftPicHdr->u4PTS - prRightPicHdr->u4PTS) > 500) && ((prLeftPicHdr->u4PTS - prRightPicHdr->u4PTS) < 180000))
                    {
                        LOG_REL(prB2rVar->fgEnableVideoLog, 1, 3, "LRSync prelook: Drop R frame pts(%d) TempRef(%d) \n",prRightPicHdr->u4PTS,prRightPicHdr->i4TemporalRef);
                        _B2R_FreeFrameBeforeEosHandler(this, FALSE, FALSE, &fgEos);
                        if (fgEos)
                        {
                            break;
                        }

                        if (FBM_CheckFrameBufferDispQ(prFrcPrm->ucSubFbgId))
                        {
                            u1RightFbId = FBM_PreLookFrameBufferFromDispQ(prFrcPrm->ucSubFbgId);
                            prRightPicHdr = FBM_GetFrameBufferPicHdr(prFrcPrm->ucSubFbgId, u1RightFbId);
                            if (prRightPicHdr == NULL)
                            {
                                break;
                            }
                        }
                        else
                        {
                            break;
                        }
                    }
                    else if ((prLeftPicHdr->u4PTS - prRightPicHdr->u4PTS) >= 180000)
                    {
                        if (FBM_CheckFrameBufferDispQ(prFrcPrm->ucSubFbgId) > 1)
                        {
                            LOG_REL(prB2rVar->fgEnableVideoLog, 1, 3, "LRSync prelook: Drop R frame pts(%d) TempRef(%d) \n",prRightPicHdr->u4PTS,prRightPicHdr->i4TemporalRef);
                            _B2R_FreeFrameBeforeEosHandler(this, FALSE, FALSE, &fgEos);
                        }
                        break;
                    }
                    else
                    {
                        break;
                    }
                }
            }
        }
    }

    if (!fgEos)
    {
        if (FBM_CheckFrameBufferDispQ(prFrcPrm->ucFbgId) && FBM_CheckFrameBufferDispQ(prFrcPrm->ucSubFbgId))
        {
            // L/R view are both exist => check their PTS
            UCHAR ucLookFbIdL = FBM_FB_ID_UNKNOWN;
            UCHAR ucLookFbIdR = FBM_FB_ID_UNKNOWN;
            FBM_PIC_HDR_T* prLookPicHdrL = NULL;
            FBM_PIC_HDR_T* prLookPicHdrR = NULL;

            ucLookFbIdL = FBM_PreLookFrameBufferFromDispQ(prFrcPrm->ucFbgId);
            prLookPicHdrL = (ucLookFbIdL == FBM_FB_ID_UNKNOWN)? NULL : FBM_GetFrameBufferPicHdr(prFrcPrm->ucFbgId, ucLookFbIdL);

            ucLookFbIdR = FBM_PreLookFrameBufferFromDispQ(prFrcPrm->ucSubFbgId);
            prLookPicHdrR = (ucLookFbIdR == FBM_FB_ID_UNKNOWN)? NULL : FBM_GetFrameBufferPicHdr(prFrcPrm->ucSubFbgId, ucLookFbIdR);

            if (prLookPicHdrL != NULL &&  prLookPicHdrR != NULL)
            {
                UINT32 u4LRDelta = (prLookPicHdrL->u4PTS > prLookPicHdrR->u4PTS)?
                                   (prLookPicHdrL->u4PTS - prLookPicHdrR->u4PTS) : (prLookPicHdrR->u4PTS - prLookPicHdrL->u4PTS);
                if (u4LRDelta < 500 || u4LRDelta > 180000)
                {
                    // LR is sync, or free run => display it
                    if (!FBM_IsSeamlessFB(prFrcPrm->ucSubFbgId, ucLookFbIdR) || 
                        (prLookPicHdrR->ucSeamlessResizeFbid != FBM_FB_ID_UNKNOWN) || 
                        (prLookPicHdrR->ucSeamlessResize2Fbid != FBM_FB_ID_UNKNOWN))
                    {
                        prFrcPrm->ucFbId = FBM_GetFrameBufferFromDispQ(prFrcPrm->ucFbgId);
                        prFrcPrm->ucSubFbId = FBM_GetFrameBufferFromDispQ(prFrcPrm->ucSubFbgId);
                    }
                    else
                    {
                        prFrcPrm->ucFbId = FBM_FB_ID_UNKNOWN;
                        LOG_REL(prB2rVar->fgEnableVideoLog, 1, 3, "LRSync L/R presync but rz not started\n");
                    }
                }
                else
                {
                    // LR is not sync => ignore this time
                    LOG_REL(prB2rVar->fgEnableVideoLog, 1, 3, "LRSync Ignore display because LR PTS not sync: L(%d), R(%d), Delta(%d)\n",
                            prLookPicHdrL->u4PTS, prLookPicHdrR->u4PTS, u4LRDelta);
                    prFrcPrm->ucFbId = FBM_FB_ID_UNKNOWN;
                }
            }
            else
            {
                // should not be here...
                prFrcPrm->ucFbId = FBM_FB_ID_UNKNOWN;
            }
        }
        else
        {
            // one view is not exist => ignore this time
            LOG_REL(prB2rVar->fgEnableVideoLog, 1, 3, "LRSync Ignore display because LR one view not exist: L(%d), R(%d)\n",
                    FBM_CheckFrameBufferDispQ(prFrcPrm->ucFbgId), FBM_CheckFrameBufferDispQ(prFrcPrm->ucSubFbgId));
            prFrcPrm->ucFbId = FBM_FB_ID_UNKNOWN;
        }
    }
}
#endif

void _B2R_GetFrameBufferForOMX(B2R_OBJECT_T* this, BOOL* pfgGstPlayBack)
{
    //UINT16 u2MsgQIdx;
    SIZE_T zMsgSize;
    VDP_B2R_CHG_FRAME_MSG_T rMsg;
    B2R_VAR_T* prB2rVar; 
    BOOL fgEos = FALSE;
    B2R_PRM_T* prFrcPrm;
    UCHAR ucB2rId;
	
    FBM_PIC_HDR_T* prNextFbmPicHdr=NULL;
	FBM_PIC_HDR_T* prCurFbmPicHdr =NULL;
	VDEC_ES_INFO_T* prVdecEsInfo = NULL;
	UCHAR ucEsId;
	VDP_CFG_T* prVdpConf;
	UCHAR ucPreLookFbId;
	FBM_SEQ_HDR_T* prSeqHdr;
	UINT32 u4PitchWidth;
	UINT16 u2MsgQNum;
    *pfgGstPlayBack = TRUE;
    
    do
    {
        if (!this)
        {
            break;
        }
        ucB2rId = this->ucB2rId;
        if(ucB2rId >= B2R_NS)
        {
            LOG(0, "ucB2rId = %d [%s-%d]\n", ucB2rId, __FUNCTION__, __LINE__);;
            break;
        }

		prVdpConf = _B2R_GetDP(this);
		ucEsId = prVdpConf->ucInputPort[0];
		prVdecEsInfo = (ucEsId < VDEC_MAX_ES)? _VDEC_GetEsInfo(ucEsId) : NULL;
		 if(!prVdecEsInfo)
		 {
		     break;
		 }
		
        prFrcPrm = this->ptB2rPrm;
        prB2rVar = this->ptB2rVar;
        zMsgSize = sizeof(VDP_B2R_CHG_FRAME_MSG_T);
        x_memset(&rMsg, 0, zMsgSize);

        if(prB2rVar->fgPendingChgFrmMsg)
        {
            LOG(2,"_B2R_GetFrameBufferForOMX: Pending \n");
        }
        
        if (!prB2rVar->fgPendingChgFrmMsg &&
                //x_msg_q_receive(&u2MsgQIdx, &rMsg, &zMsgSize,&(_ahChgFrameQueue[prFrcPrm->ucVdpId]), 1, X_MSGQ_OPTION_NOWAIT) != OSR_OK
                (_B2R_ReceiveChgFrameMsg(ucB2rId, &rMsg, X_MSGQ_OPTION_NOWAIT) != OSR_OK))
        {
            prFrcPrm->ucFbId = 0xFF;
        }
        else
        {
            if (prB2rVar->fgPendingChgFrmMsg)
            {
                x_memcpy(&rMsg, &(prB2rVar->rPendingChgFrmMsg), zMsgSize);
                prB2rVar->fgPendingChgFrmMsg = FALSE;
            }

		   if((prVdecEsInfo->eSeamlessMode & SEAMLESS_BY_NPTV))
	       {
	           ucPreLookFbId = FBM_PreLookBFromDispQ(prFrcPrm->ucFbgId,uVsyncNum);
	     	   prCurFbmPicHdr= FBM_GetFrameBufferPicHdr(prFrcPrm->ucFbgId, prFrcPrm->ucFbId );
	           prNextFbmPicHdr= FBM_GetFrameBufferPicHdr(prFrcPrm->ucFbgId, ucPreLookFbId);
			   x_msg_q_num_msgs(_ahChgFrameQueue[ucB2rId],&u2MsgQNum);
			  
			   if((prNextFbmPicHdr!=NULL)&&(prCurFbmPicHdr!=NULL))
			   {
				 if((prNextFbmPicHdr->u4PicWidth!=prCurFbmPicHdr->u4PicWidth)||(prNextFbmPicHdr->u4PicHeight!=prCurFbmPicHdr->u4PicHeight))
				 {
				     if(prFrcPrm->fgSeqChg == FALSE)
                     {
                         LOG(1,"PreCheck:WH(%d,%d,Fbid=%d),Next WH(%d,%d,ucPreLookFbId=%d),ucFbgId=%d,TagNs=%d,rMsg->ucFbId=%d\n",prCurFbmPicHdr->u4PicWidth,prCurFbmPicHdr->u4PicHeight,prFrcPrm->ucFbId,prNextFbmPicHdr->u4PicWidth,prNextFbmPicHdr->u4PicHeight,ucPreLookFbId,prFrcPrm->ucFbgId,(prFrcPrm->u4InStcPeriod /prFrcPrm->u4OutStcPeriod ),rMsg.ucFbId);
                         LOG(1,"org(W=%d,H=%d), Current(W=%d,H=%d),u2MsgQNum=%d\n",prCurFbmPicHdr->u4PicWidth,prNextFbmPicHdr->u4PicHeight,prNextFbmPicHdr->u4PicWidth,prNextFbmPicHdr->u4PicHeight,u2MsgQNum);
                         vVRMSetEventFlg(SV_VP_MAIN, VRM_EVENT_BY_B2R);
                         prFrcPrm->fgSeqChg=TRUE;
                     }
				}
				else
				{
					 prFrcPrm->fgSeqChg=FALSE;
				}
		      }

				 ucPreLookFbId = FBM_PreLookBFromDispQ(prFrcPrm->ucFbgId,0);
			     prNextFbmPicHdr= FBM_GetFrameBufferPicHdr(prFrcPrm->ucFbgId, ucPreLookFbId);
			     prCurFbmPicHdr= FBM_GetFrameBufferPicHdr(prFrcPrm->ucFbgId, prFrcPrm->ucFbId);
				 
				 if((prNextFbmPicHdr!=NULL)&&(prCurFbmPicHdr!=NULL))
				 {
					 if((prNextFbmPicHdr->u4PicWidth!=prCurFbmPicHdr->u4PicWidth)||(prNextFbmPicHdr->u4PicHeight!=prCurFbmPicHdr->u4PicHeight))
					 {
					      BOOL fgSet = TRUE;
						  prSeqHdr = FBM_GetFrameBufferSeqHdr(prFrcPrm->ucFbgId);
                          LOG(1,"CurrentCheck: WH(%d,%d,Fbid=%d),Next WH(%d,%d,ucPreLookFbId=%d),ucFbgId=%d\n",prCurFbmPicHdr->u4PicWidth,prCurFbmPicHdr->u4PicHeight,prFrcPrm->ucFbId,prNextFbmPicHdr->u4PicWidth,prNextFbmPicHdr->u4PicHeight,ucPreLookFbId,prFrcPrm->ucFbgId);
                          u4PitchWidth=prSeqHdr->u2LineSize;
						  LOG(1,"Next pitch=%d,current Pitch=%d,FB=%d,u2LineSize=%d\n",prNextFbmPicHdr->u4PicWidth,prCurFbmPicHdr->u4PicWidth,ucPreLookFbId,prSeqHdr->u2LineSize);
						  B2R_HAL_Set(this->hB2r, B2R_HAL_PITCH, &u4PitchWidth);
                          B2R_HAL_Set(this->hB2r, B2R_HAL_SET_RESOLUTION,&fgSet);
                          FBM_DoSeqChanging(prFrcPrm->ucFbgId,FALSE, FALSE);
                          vVRMB2RTrigger( _B2R_GetVdpId(ucB2rId));
					  }
				 }
	     	}
           
            if ( (ucB2rId < B2R_NS) && (prB2rVar->eSpeed != STC_SPEED_TYPE_FORWARD_1X))
            {
                static UINT32 _u4Counter[B2R_NS] = {0, 0};
                if (prB2rVar->eSpeed == STC_SPEED_TYPE_FORWARD_2X)
                {
                    /*if((FBM_CheckFrameBufferDispQ(prFrcPrm->ucFbgId) >= 2))
                                         {
                                             prFrcPrm->ucFbId = FBM_GetFrameBufferFromDispQ(prFrcPrm->ucFbgId);
                                             FBM_SetFrameBufferStatus(prFrcPrm->ucFbgId, prFrcPrm->ucFbId, FBM_FB_STATUS_LOCK);
                                             FBM_SetFrameBufferStatus(prFrcPrm->ucFbgId, prFrcPrm->ucFbId, FBM_FB_STATUS_EMPTY);
                                             prFrcPrm->ucFbId = FBM_GetFrameBufferFromDispQ(prFrcPrm->ucFbgId);
                                         }
                                         else
                                         {
                                             prFrcPrm->ucFbId = 0xFF;
                                         }*/
                    if (_u4Counter[ucB2rId] % 2 ==0)
                    {
                        UCHAR ucFbId = _B2R_FreeFrameBeforeEosHandler(this, TRUE, _B2R_ChangeFrameDirectForOMX(this, &rMsg), &fgEos);
                        if (FBM_FB_ID_UNKNOWN != ucFbId)
                        {
                            _u4Counter[ucB2rId]++;
                        }
                    }
                    else
                    {
                        if (!_B2R_ChangeFrameDirectForOMX(this, &rMsg))
                        {
                            prFrcPrm->ucFbId = FBM_GetFrameBufferFromDispQ(prFrcPrm->ucFbgId);
                        }
                        if (prFrcPrm->ucFbId != 0xff)
                        {
                            _u4Counter[ucB2rId]++;
                        }
                    }
                }
                else //1.5x case
                {
                    if (_u4Counter[ucB2rId] % 3 ==0)
                    {
                        UCHAR ucFbId = _B2R_FreeFrameBeforeEosHandler(this, TRUE, _B2R_ChangeFrameDirectForOMX(this, &rMsg), &fgEos);
                        if (FBM_FB_ID_UNKNOWN != ucFbId)
                        {
                            _u4Counter[ucB2rId]++;
                        }
                    }
                    else
                    {
                        if (!_B2R_ChangeFrameDirectForOMX(this, &rMsg))
                        {
                            prFrcPrm->ucFbId = FBM_GetFrameBufferFromDispQ(prFrcPrm->ucFbgId);
                        }
                        if (prFrcPrm->ucFbId != 0xff)
                        {
                            _u4Counter[ucB2rId]++;
                        }
                    }
                }
            }
            else
            {
                BOOL fgFound = TRUE;
                FBM_PIC_HDR_T* prPicHdr = NULL;

                if (VDP_B2R_MULTI_VIEW & rMsg.u4Flag)
                {
                    UINT8 ucLId;
#ifdef CC_3D_MM_DS_SUPPORT
                    UINT8 ucRId;
#endif

                    ucLId = FBM_PreLookFrameBufferFromDispQ(prFrcPrm->ucFbgId);

#ifdef CC_3D_MM_DS_SUPPORT
                    ucRId = FBM_PreLookFrameBufferFromDispQ(prFrcPrm->ucSubFbgId);
#endif

                    prFrcPrm->ucFbId = 0xFF;

                    //check if it need resize
                    if (ucLId != FBM_FB_ID_UNKNOWN)
                    {
                        prPicHdr = FBM_GetFrameBufferPicHdr(prFrcPrm->ucFbgId, ucLId);
                        if (!prPicHdr || (prPicHdr->fgSeamlessDisp && 
                            (prPicHdr->ucSeamlessResizeFbid == FBM_FB_ID_UNKNOWN)&&
                            (prPicHdr->ucSeamlessResize2Fbid == FBM_FB_ID_UNKNOWN)))
                        {
                            ucLId = FBM_FB_ID_UNKNOWN;
                        }
                    }

#ifdef CC_3D_MM_DS_SUPPORT

                    if (ucRId != FBM_FB_ID_UNKNOWN)
                    {
                        prPicHdr = FBM_GetFrameBufferPicHdr(prFrcPrm->ucSubFbgId, ucRId);
                        if (!prPicHdr || (prPicHdr->fgSeamlessDisp && 
                            (prPicHdr->ucSeamlessResizeFbid == FBM_FB_ID_UNKNOWN)&&
                            (prPicHdr->ucSeamlessResize2Fbid == FBM_FB_ID_UNKNOWN)))
                        {
                            ucRId = FBM_FB_ID_UNKNOWN;
                        }
                    }


                    if (ucLId != FBM_FB_ID_UNKNOWN &&
                            ucRId != FBM_FB_ID_UNKNOWN)
                    {
                        FBM_PIC_HDR_T* prLeftPicHdr = NULL;
                        FBM_PIC_HDR_T* prRightPicHdr = NULL;
                        prFrcPrm->ucFbId = FBM_GetFrameBufferFromDispQ(prFrcPrm->ucFbgId);
                        prFrcPrm->ucSubFbId = FBM_GetFrameBufferFromDispQ(prFrcPrm->ucSubFbgId);
                        prRightPicHdr = FBM_GetFrameBufferPicHdr(prFrcPrm->ucSubFbgId, prFrcPrm->ucSubFbId);
                        prLeftPicHdr  = FBM_GetFrameBufferPicHdr(prFrcPrm->ucFbgId, prFrcPrm->ucFbId);
                        LOG(2, "[OMX] 3D fbid (%d, %d)(%d %d)\n", prFrcPrm->ucFbId, prFrcPrm->ucSubFbId, (prLeftPicHdr?prLeftPicHdr->i4TemporalRef:0xFFFFFFFF), (prRightPicHdr?prRightPicHdr->i4TemporalRef:0xFFFFFFFF));
                    }
                    else
                    {
                        fgFound = FALSE;
                        LOG(2, "[OMX] 3D fbid (%d, %d)(%d %d)\n", ucLId, ucRId);
                    }

#endif

                }
                else
                {
                    if (!_B2R_ChangeFrameDirectForOMX(this, &rMsg))
                    {
                        prFrcPrm->ucFbId = FBM_PreLookFrameBufferFromDispQ(prFrcPrm->ucFbgId);
                        if (prFrcPrm->ucFbId != FBM_FB_ID_UNKNOWN)
                        {
                            prPicHdr = FBM_GetFrameBufferPicHdr(prFrcPrm->ucFbgId, prFrcPrm->ucFbId);
                            if(!prPicHdr || (prPicHdr->fgSeamlessDisp && 
                                (prPicHdr->ucSeamlessResizeFbid == FBM_FB_ID_UNKNOWN)&&
                                (prPicHdr->ucSeamlessResize2Fbid == FBM_FB_ID_UNKNOWN)))
                            {
                                prFrcPrm->ucFbId = FBM_FB_ID_UNKNOWN;
                                fgFound = FALSE;
                            }
                        }

                        if (prFrcPrm->ucFbId != 0xFF)
                        {
                            FBM_PIC_HDR_T* prPicHdr = NULL;
                            prFrcPrm->ucFbId = FBM_GetFrameBufferFromDispQ(prFrcPrm->ucFbgId);
                            prPicHdr = FBM_GetFrameBufferPicHdr(prFrcPrm->ucFbgId, prFrcPrm->ucFbId);
                            LOG(5, "_B2R_GetFrameBufferForOMX fbid (%d,%d, %d)\n", prFrcPrm->ucFbgId, prFrcPrm->ucFbId,(prPicHdr?prPicHdr->i4TemporalRef:0));
                        }
                    }
                    else
                    {
                        if (prFrcPrm->ucFbId != FBM_FB_ID_UNKNOWN)
                        {
                            prPicHdr = FBM_GetFrameBufferPicHdr(prFrcPrm->ucFbgId, prFrcPrm->ucFbId);
                            if (!prPicHdr || (prPicHdr->fgSeamlessDisp && 
                                (prPicHdr->ucSeamlessResizeFbid == FBM_FB_ID_UNKNOWN)&&
                                (prPicHdr->ucSeamlessResize2Fbid == FBM_FB_ID_UNKNOWN)))
                            {
                                prFrcPrm->ucFbId = FBM_FB_ID_UNKNOWN;
                                fgFound = FALSE;
                                LOG(2, "_B2R_GetFrameBufferForOMX: FB(%d) Resize not ready.\n", prFrcPrm->ucFbId);
                            }
                        }
                    }
                }

                if (!fgFound)
                {
                    prFrcPrm->ucFbId = FBM_FB_ID_UNKNOWN;
                    LOG(2,"_B2R_GetFrameBufferForOMX: not Found\n");
                    x_memcpy(&(prB2rVar->rPendingChgFrmMsg), &rMsg, zMsgSize);
                    prB2rVar->fgPendingChgFrmMsg = TRUE;
                }

            }
        }
    }while(FALSE);
}

void _B2R_GetFrameBufferForSkype(B2R_OBJECT_T* this)
{
    BOOL fgEos = FALSE;
    UCHAR ucDropFbId = FBM_FB_ID_UNKNOWN;
    B2R_PRM_T* prFrcPrm =NULL;
    prFrcPrm = this->ptB2rPrm;
    // For skype case, always display latest frame (drop old frames)
    
    while (FBM_CheckFrameBufferDispQ(prFrcPrm->ucFbgId) > 1)
    {
        ucDropFbId = _B2R_FreeFrameBeforeEosHandler(this, TRUE, FALSE, &fgEos);
        LOG(6, "[SKYPE] DispQ(%d), drop one fb = %d\n", FBM_CheckFrameBufferDispQ(prFrcPrm->ucFbgId), ucDropFbId);
        if (ucDropFbId == FBM_FB_ID_UNKNOWN)
        {
            break;
        }
    }
    if (!fgEos)
    {
        prFrcPrm->ucFbId = FBM_GetFrameBufferFromDispQ(prFrcPrm->ucFbgId);
    }
}
#ifdef CC_B2R_SUPPORT_GAME_MODE
void _B2R_GameModeGetFrameBuffer(B2R_OBJECT_T* this,UCHAR ucCount)
{
    BOOL fgEos = FALSE;
    B2R_PRM_T* prFrcPrm =NULL;
    UCHAR ucDropFbId;
    prFrcPrm = this->ptB2rPrm;
    ucDropFbId = FBM_FB_ID_UNKNOWN;
    while (FBM_CheckFrameBufferDispQ(prFrcPrm->ucFbgId) > ucCount)
    {
        ucDropFbId = _B2R_FreeFrameBeforeEosHandler(this, TRUE, FALSE, &fgEos);
        LOG(6, "[Game Mode] DispQ(%d), drop one fb = %d\n", FBM_CheckFrameBufferDispQ(prFrcPrm->ucFbgId), ucDropFbId);
        if (ucDropFbId == FBM_FB_ID_UNKNOWN)
        {
            break;
        }
    }
    if (!fgEos)
    {
        prFrcPrm->ucFbId = FBM_GetFrameBufferFromDispQ(prFrcPrm->ucFbgId);
    }
}
#endif
static VOID _B2R_GetFrameBufferForGeneral(B2R_OBJECT_T* this)
{
    B2R_VAR_T* prB2rVar;
    B2R_PRM_T* prFrcPrm;
        
    if (!this)
    {
        return;
    }
    
    prB2rVar = this->ptB2rVar;
    prFrcPrm = this->ptB2rPrm;
    
    if (prB2rVar->fgDropFrame && (FBM_CheckFrameBufferDispQ(prFrcPrm->ucFbgId) >= 2))
    {
        BOOL fgEos = FALSE;
        _B2R_FreeFrameBeforeEosHandler(this, TRUE, FALSE, &fgEos);
        if (!fgEos)
        {
            prFrcPrm->ucFbId = FBM_GetFrameBufferFromDispQ(prFrcPrm->ucFbgId);
            prB2rVar->fgDropFrame = FALSE;
            LOG(1,"audio driver notfiy vdp to drop one frame for av sync\n");
        }
    }
    else if (prB2rVar->fgRepeatFrame)
    {
        UINT32 u4OutFrameStc;
        UNUSED(u4OutFrameStc);
        prFrcPrm->ucFbId = 0xFF;
        prB2rVar->fgRepeatFrame = FALSE;
        prB2rVar->fgGstPlayBack = TRUE;

        if (prFrcPrm->ucProgressiveSeqFlag)
        {
            prFrcPrm->i4PtsDrift += prFrcPrm->u4OutStcPeriod;
        }
        else
        {
            prFrcPrm->i4PtsDrift += 2*prFrcPrm->u4OutStcPeriod;
        }
        LOG(1,"audio driver notfiy vdp to repeat one frame for av sync\n");
    }
    else if (prB2rVar->fgTimeShift && !prB2rVar->fgDoubleClock && (prB2rVar->i4TimeShiftSpeed == 200))
    {
        BOOL fgEos = FALSE;
        // When timeshift + FF 2x, and double clock is not enabled, drop half of frames.
        if (FBM_CheckFrameBufferDispQ(prFrcPrm->ucFbgId) >= 2)
        {
            _B2R_FreeFrameBeforeEosHandler(this, TRUE, FALSE, &fgEos);
        }
        if (!fgEos)
        {
            prFrcPrm->ucFbId = FBM_GetFrameBufferFromDispQ(prFrcPrm->ucFbgId);
        }
    }
    else
    {
        UCHAR ucLookFbId = FBM_FB_ID_UNKNOWN;
        FBM_PIC_HDR_T* prLookPicHdr = NULL;
        ucLookFbId = FBM_PreLookFrameBufferFromDispQ(prFrcPrm->ucFbgId);
        prLookPicHdr = (ucLookFbId == FBM_FB_ID_UNKNOWN)? NULL : FBM_GetFrameBufferPicHdr(prFrcPrm->ucFbgId, ucLookFbId);
        if (prLookPicHdr)
        {
            BOOL fgReady;
            fgReady = (prLookPicHdr->fgSeamlessDisp) ? 
                ((prLookPicHdr->ucSeamlessResizeFbid != FBM_FB_ID_UNKNOWN) || (prLookPicHdr->ucSeamlessResize2Fbid != FBM_FB_ID_UNKNOWN)) : TRUE;

            if(prLookPicHdr->fgSeamlessDisp &&
                !FBM_CheckResizeFrameBufferStatus(prFrcPrm->ucFbgId,
                prLookPicHdr->ucSeamlessResizeFbid, FBM_FB_STATUS_RESIZE_DONE))
            {
                fgReady = FALSE;
            }
            
            if (fgReady)
            {
                prFrcPrm->ucFbId = FBM_GetFrameBufferFromDispQ(prFrcPrm->ucFbgId);
                _B2R_4K60FbHdr(this);
            }
            else
            {
                prFrcPrm->ucFbId = FBM_FB_ID_UNKNOWN;
            }
        }
        else
        {
            prFrcPrm->ucFbId = FBM_FB_ID_UNKNOWN;
        }
#ifdef CC_3D_MM_DS_SUPPORT
        prFrcPrm->ucSubFbId = FBM_FB_ID_UNKNOWN;
#endif
    }
}

static VOID _B2R_SearchFrameInDisplayQ(B2R_OBJECT_T *this)
{
    UCHAR ucSeekMode = 0;
    UINT32 u4SeekPts = 0;
    UCHAR ucSeekNotifyTrigger = 0;
    B2R_VAR_T* prB2rVar;
    B2R_PRM_T* prFrcPrm;
    FBM_PIC_HDR_T* prFbmPicHdr=NULL;
    UCHAR ucB2rId;
    UCHAR ucDisQ=0;
    UCHAR ucFbId=0xFF;
    UCHAR ucFbIdTmp=0xFF;
    UCHAR i=0;
    if (!this)
    {
        return ;
    }

    ucB2rId = this->ucB2rId;
    prB2rVar = this->ptB2rVar;
    prFrcPrm = this->ptB2rPrm;
    ucDisQ=FBM_CheckFrameBufferDispQ(prFrcPrm->ucFbgId);
    _B2R_GetSeek(this, &ucSeekMode, &u4SeekPts, &ucSeekNotifyTrigger);
    if(ucSeekMode!=2)
    {
       return;
    }
     this->ptB2rVar->rVdpSeek.ucSeekMode=0;
    if(ucDisQ>0)
    {
         
        for(i=0;i<=ucDisQ;i++)
        {
             ucFbId=FBM_PreLookNthFBFromDispQ(prFrcPrm->ucFbgId, i);
             prFbmPicHdr= FBM_GetFrameBufferPicHdr(prFrcPrm->ucFbgId, ucFbId);
             if(prFbmPicHdr!=0)
             {
                  LOG(1,"VDP_SetSeekQstatus ucFbId=%d,u8PTS=%x,Seek PTS=%x\n",ucFbId,prFbmPicHdr->u4PTS,u4SeekPts);
                  if(u4SeekPts>=prFbmPicHdr->u4PTS)
                  {
                        break;
                  }
             }
        }

        if(ucFbId!=0xFF)
        {
            for(i=0;i<=ucDisQ;i++)
            {
                ucFbIdTmp=FBM_GetFrameBufferFromDispQ(prFrcPrm->ucFbgId);
                LOG(0,"_B2R_SearchFrameInDisplayQ ucFbIdTmp=%d,ucFbId=%d\n",ucFbIdTmp,ucFbId);
                if(ucFbIdTmp!=ucFbId)
                {
                     _B2R_SetFrameBufferStatus(this, prFrcPrm->ucFbgId,ucFbIdTmp, FBM_FB_STATUS_LOCK);
                     _B2R_SetFrameBufferStatus(this, prFrcPrm->ucFbgId,ucFbIdTmp, FBM_FB_STATUS_EMPTY);
                }
                else if(ucFbIdTmp==ucFbId)
                {
                    if (FBM_ChkFrameBufferPicFlag(prFrcPrm->ucFbgId, ucFbId, FBM_MM_PSEUDO_EOS_FLAG))
                    {
                        _B2R_PostNotify(this, VDP_MSG_MM_SEEK_FIN_CB,u4SeekPts,VDP_SEEK_STEP_NO_DATA);
                    }
                    else
                    {
                        _B2R_PostNotify(this, VDP_MSG_MM_SEEK_FIN_CB,u4SeekPts,VDP_SEEK_STEP_OK);
                    }
                    break;
                }
            }
            
        }
    }
    
  
}
/* Seek Finish Notify at 20081212 */
static UINT32 _B2R_SeekFinishNtfy(B2R_OBJECT_T *this, BOOL fgDash)
{
    UCHAR ucSeekMode = 0;
    UINT32 u4SeekPts = 0;
    UCHAR ucSeekNotifyTrigger = 0;
    B2R_VAR_T* prB2rVar;
    B2R_PRM_T* prFrcPrm;
    UCHAR ucB2rId;

    if (!this)
    {
        return B2R_INV_ARG;
    }

    ucB2rId = this->ucB2rId;
    prB2rVar = this->ptB2rVar;
    prFrcPrm = this->ptB2rPrm;

    _B2R_GetSeek(this, &ucSeekMode, &u4SeekPts, &ucSeekNotifyTrigger);
    if ((prB2rVar->ucRemainTimeToSendSeekDone) == 0 && (ucSeekMode != 0) && (ucSeekNotifyTrigger != 0) &&
      ((u4SeekPts <= prFrcPrm->u4Pts &&prB2rVar->eSpeed >= STC_SPEED_TYPE_FORWARD_1X && prB2rVar->eSpeed <= STC_SPEED_TYPE_FORWARD_1_DOT_5X) ||
      (u4SeekPts >= prFrcPrm->u4Pts && prB2rVar->eSpeed >= STC_SPEED_TYPE_REWIND_1X && prB2rVar->eSpeed < STC_SPEED_TYPE_NS)))
    {
        if (fgDash)
        {
            prB2rVar->ucRemainTimeToSendSeekDone = DURATION_TO_SEND_SEEK_DONE;
        }
        else 
        {
            prB2rVar->ucRemainTimeToSendSeekDone = 1;
        }
    }
    
    if (prB2rVar->ucRemainTimeToSendSeekDone == 1)
    {
        LOG(3, "+++++ Seek trigger notify\n");
        /* Seek Finish notify trigger status clear */
        prB2rVar->rVdpSeek.ucNotifyTrigger = 0;
        /* Seek Finish Notification */
        if (FBM_ChkFrameBufferPicFlag(prFrcPrm->ucFbgId, prFrcPrm->ucFbId, FBM_MM_PSEUDO_EOS_FLAG))
        {
            _B2R_PostNotify(this, VDP_MSG_MM_SEEK_FIN_CB,u4SeekPts,VDP_SEEK_STEP_NO_DATA);
        }
        else
        {
            _B2R_PostNotify(this, VDP_MSG_MM_SEEK_FIN_CB,u4SeekPts,VDP_SEEK_STEP_OK);
        }
    }
    
    if (prB2rVar->ucRemainTimeToSendSeekDone > 0)
    {
        (prB2rVar->ucRemainTimeToSendSeekDone)--;
    }

    return B2R_OK;
    
}

static void _B2R_HandleABRepeat(B2R_OBJECT_T* this, FBM_PIC_HDR_T *prPic, BOOL fgFastForward)
{
    BOOL fgCheckPts = TRUE;
    BOOL fgTempref = TRUE;
    BOOL fgABReach = FALSE;
    UINT32 ui4_stc = 0x0;
    UCHAR ucAvSyncMode;
    UCHAR ucStcSrc;
    B2R_VAR_T* prB2rVar;
    UCHAR ucB2rId;
    B2R_PRM_T* prFrcPrm;
    UCHAR ucVdpId;

    do
    {
        if (!this)
        {
            break;
        }
        ucB2rId = this->ucB2rId;
        ucVdpId = _B2R_GetVdpId(ucB2rId);
        prFrcPrm = this->ptB2rPrm;
        if (ucB2rId >= B2R_NS)
        {
            LOG(0, "prFrcPrm->ucB2rId = %d [%s-%d]\n", ucB2rId, __FUNCTION__, __LINE__);
            break;
        }
        prB2rVar = this->ptB2rVar;
        if ((prB2rVar->eSpeed >= STC_SPEED_TYPE_FORWARD_1_DIV_2X) &&
                (prB2rVar->eSpeed <= STC_SPEED_TYPE_FORWARD_1_DOT_5X))
        {
            fgTempref = FALSE;
        }
#ifdef ENABLE_MULTIMEDIA
        {
        ENUM_SWDMX_FMT_T eContainerType;
        eContainerType = (ENUM_SWDMX_FMT_T)_B2R_GetContainerType(this);
        if ((eContainerType == SWDMX_FMT_MPEG1_DAT) ||
            (eContainerType == SWDMX_FMT_MPEG2_PS) ||
            (eContainerType == SWDMX_FMT_MPEG2_TS) ||
            (eContainerType == SWDMX_FMT_MPEG2_TS_192) ||
            (eContainerType == SWDMX_FMT_MPEG2_TS_ZERO_192) ||
            (eContainerType == SWDMX_FMT_MPEG2_TS_ENCRYPT_192) ||
            (eContainerType == SWDMX_FMT_VC1_ES) ||
            (eContainerType == SWDMX_FMT_MPEG2_VIDEO_ES) ||
            (eContainerType == SWDMX_FMT_MPEG4_VIDEO_ES) ||
            (eContainerType == SWDMX_FMT_H264_VIDEO_ES) ||
            /*for avs video es, pts and stc value may be error, use file position*/
            (eContainerType == SWDMX_FMT_AVS_VIDEO_ES)
           )
        {
            fgCheckPts = FALSE;
        }
        }
#endif
        ui4_stc = _B2R_FrcGetStc(prFrcPrm->ucFbgId);

        if (prB2rVar->rAB.fgPosition)
        {
            if (fgFastForward)
            {
                if ( ((fgCheckPts) && (prPic->u4PTS >= prB2rVar->rAB.rB.u8Pts) &&
                        (ui4_stc >= prB2rVar->rAB.rB.u8Stc)) ||
                        ((!fgCheckPts) &&((prPic->u8OffsetDisp >= prB2rVar->rAB.rB.u8OffsetDisp) &&
                                          ((fgTempref) ||
                                           (prPic->i4TemporalRef == prB2rVar->rAB.rB.i4TemporalRef)))))
                {
                    fgABReach = TRUE;
                }
            }
            else
            {
                if (((fgCheckPts) && (prPic->u4PTS <= prB2rVar->rAB.rA.u8Pts)) ||
                        ((!fgCheckPts) &&
                         (prPic->u8OffsetDisp <= prB2rVar->rAB.rA.u8OffsetDisp)))
                {
                    fgABReach = TRUE;
                }
            }
        }
        else
        {
            if (fgFastForward)
            {
                if ( ( (UINT64)prPic->u4PTS >= prB2rVar->rAB.rB.u8Pts ) &&
                        ((  ui4_stc >= prB2rVar->rAB.rB.u8Stc) ||
                         ((_B2R_GetStepForward(ucB2rId) == VDP_PLAY_STEP_FORWARD)&&
                          (prB2rVar->rAB.rA.u8Pts != prB2rVar->rAB.rB.u8Pts))))
                {
                    fgABReach = TRUE;
                }
            }
            else
            {
                if ((UINT64)prPic->u4PTS <= prB2rVar->rAB.rA.u8Pts)
                {
                    fgABReach = TRUE;
                }
            }
        }
        if ((fgABReach) && (prB2rVar->rVdpSeek.ucABNotifyTrigger != 0))
        {

            // if AB reach, driver should send SEEK DONE / STEP DONE notify mw before AB DONE notify ,
            // It's mw flow request
            _B2R_HandleStepForward(this);

            /* Seek Finish Notify at 20081212 */
            _B2R_SeekFinishNtfy(this, FALSE);  // LEO_B2R_REFACTORY

            prB2rVar->rVdpSeek.ucABNotifyTrigger = 0;
            prB2rVar->rVdpSeek.fgABReach = TRUE;
            Printf("***VDP_COND_RANGE_AB_DONE Notify****");
            FBM_GetSyncStc(prFrcPrm->ucFbgId, &ucAvSyncMode, &ucStcSrc);
            STC_StopStc(ucStcSrc);
            _B2R_PostNotify(this, VDP_MSG_MM_CB, VDP_COND_RANGE_AB_DONE, fgFastForward);
            LOG(3, "VDP_COND_RANGE_AB_DONE Notify\n");
        }
    }while(FALSE);
}


//LEO_B2R_REFACTORY ,LEO,20120124
#if (defined( DIVX_PLUS_CER) && defined(CC_B2R_CHAPTER_CHG))
static void _B2R_HandleChapter(B2R_OBJECT_T *this)
{
    B2R_PRM_T *prFrcPrm=NULL;
    UCHAR ucB2rId   =0;
    UCHAR ucVdpId   = 0;
    VDP_CHAPTER_T* prChapter =NULL;
    B2R_VAR_T* prB2rVar  =NULL;
    prFrcPrm= this->ptB2rPrm;
    if (this->ucB2rId >= B2R_NS)
    {
        LOG(0, "ucB2rId = %d [%s-%d]\n", this->ucB2rId, __FUNCTION__, __LINE__);
    }
    ucB2rId=this->ucB2rId;
    ucVdpId = _B2R_GetVdpId(prFrcPrm->ucB2rId);
	prChapter = _B2R_FrcGetChapterObj();
    prB2rVar = this->ptB2rVar;
    LOG(10, "Set(%d) Pts[0x%8x] @[0x%llx, 0x%llx].\n", prChapter->fgIsSetEndPts, prFrcPrm->u4Pts, prChapter->u8StartPts, prChapter->u8EndPts);
    if ( prChapter->fgIsSetEndPts )
    {
        prChapter->fgPending     = FALSE;

        if (ucB2rId < B2R_NS)
        {
            LOG(10, "Line(%d): Pts[0x%8x] @[0x%llx, 0x%llx].\n", __LINE__, prFrcPrm->u4Pts, prChapter->u8StartPts, prChapter->u8EndPts);
            if ( ( prB2rVar->eSpeed <= STC_SPEED_TYPE_REWIND_1024X ) &&
                    (prB2rVar->eSpeed >= STC_SPEED_TYPE_REWIND_1X ) )
            {
                if ( prFrcPrm->u4Pts < prChapter->u8StartPts * 90 || prFrcPrm->u4Pts == 0)
                {
                    _B2R_PostNotify(this, VDP_MSG_MM_CB,VDP_COND_CHAP_DONE,0);
                    LOG(3, "VDP_COND_CHAP_DONE Notify (BACKWARD)\n");
                    prChapter->fgIsSetEndPts = FALSE;
                    prChapter->fgPending     = TRUE;
                }
                else if ( prChapter->u8StartPts == 0 && prChapter->u8EndPts == (UINT64)(-1))
                {
                    LOG(10, "Line(%d): u8StartPts == 0 && u8EndPts = -1.\n", __LINE__);
                    prChapter->fgIsSetEndPts = FALSE;
                    prChapter->fgPending     = TRUE;
                }
            }
            else
            {
                if ( prFrcPrm->u4Pts >= prChapter->u8EndPts * 90 )
                {

                    //CR[DTV00348906], seek the charpter using the number key when pause status.
                    //Don't send the step done(Seek Done in MW) because of this charpter done to MW.
                    //Add the step done after charpter done when this case.
                    _B2R_HandleStepForward(this);
                    _B2R_PostNotify(this, VDP_MSG_MM_CB,VDP_COND_CHAP_DONE,0);
                    LOG(3, "VDP_COND_CHAP_DONE Notify (FORWARD)\n");

                    prChapter->fgIsSetEndPts = FALSE;
                    prChapter->fgPending     = TRUE;
                }
                else if ( prChapter->u8StartPts == 0 && prChapter->u8EndPts == (UINT64)(-1))
                {
                    LOG(10, "Line(%d): u8StartPts == 0 && u8EndPts = -1.\n", __LINE__);
                    prChapter->fgIsSetEndPts = FALSE;
                    prChapter->fgPending     = TRUE;
                }
            }
        }
    }


    if (prChapter->fgPending)
    {
        _B2R_PreparePendingForNextVsync(this);
    }
}
#endif

static void _B2R_CpyPicHdr2B2r(B2R_PRM_T * prFrcPrm, FBM_PIC_HDR_T * prNextPicHdr)
{
    if (prFrcPrm && prNextPicHdr)
    {
        prFrcPrm->fgProgress2Interlace = prNextPicHdr->fgProgressive2Interlace;
        prFrcPrm->u4Pts = prNextPicHdr->u4PTS;
        prFrcPrm->i4TemporalRef = prNextPicHdr->i4TemporalRef;
        prFrcPrm->ucPicCdTp = prNextPicHdr->ucPicCdTp;
        prFrcPrm->ucLastProgressiveFrame = prFrcPrm->ucProgressiveFrame;
        prFrcPrm->ucProgressiveFrame = prNextPicHdr->fgProgressiveFrm;
        prFrcPrm->ucRff = prNextPicHdr->fgRepFirstFld;
        prFrcPrm->ucTff = prNextPicHdr->fgTopFldFirst;
        prFrcPrm->u4TotlaTimeOffset = prNextPicHdr->u4TotlaTimeOffset;
        prFrcPrm->u8FilePosition = prNextPicHdr->u8Offset;
        prFrcPrm->u8FilePositionI = prNextPicHdr->u8OffsetI;
        prFrcPrm->u4TickNum = prNextPicHdr->u4TickNum;
        prFrcPrm->u2DecodingOrder = prNextPicHdr->u2DecodingOrder;
        prFrcPrm->fgDummyFrame = prNextPicHdr->fgDummyFrame;
        x_memcpy(&prFrcPrm->rExtra, &prNextPicHdr->rExtra, sizeof(VDP_PIC_EXTRA_INTO_T));
    }
}

static UINT32 _B2R_GetFrameBuffer(B2R_OBJECT_T* this)
{
    B2R_VAR_T* prB2rVar;
    B2R_PRM_T* prFrcPrm;
    UINT8 u1AppMode =0; 
#ifdef CC_3D_MM_DS_SUPPORT
    UINT8 u1AppModeSub =0;
#endif
    UCHAR ucRet = B2R_OK;
    UCHAR ucB2rId;

    do
    {
        if (!this)
        {
            ucRet = B2R_INV_ARG;
            break;
        }
        ucB2rId = this->ucB2rId;
        if (ucB2rId >= B2R_NS)
        {
            ucRet = B2R_INV_ARG;
            break;
        }

        prB2rVar = this->ptB2rVar;
        prFrcPrm = this->ptB2rPrm;
        
        prB2rVar->fgGstPlayBack = FALSE;
      
#ifdef CC_HWCOMP_UT
       u1AppMode = FBM_FBG_APP_OMX_DISP;
#else
       u1AppMode = FBM_GetFbgAppMode(prFrcPrm->ucFbgId);
#endif

#ifdef CC_3D_MM_DS_SUPPORT
        _B2R_GetAdditionalFbgId(this);
        if (prFrcPrm->ucSubFbgId != FBM_FBG_ID_UNKNOWN)
        {
            u1AppModeSub = FBM_GetFbgAppMode(prFrcPrm->ucSubFbgId);
        }
#endif
         
#ifdef CC_3D_MM_DS_SUPPORT
        {
            FBM_SEQ_HDR_T* prSeqHdr = FBM_GetFrameBufferSeqHdr(prFrcPrm->ucFbgId);
            if (prSeqHdr)
            {
                prFrcPrm->u1B2R3DTrickModeType = prSeqHdr->u1B2R3DTrickModeType;
            }
        }
        if (((prB2rVar->eSpeed == STC_SPEED_TYPE_FORWARD_1X) || (prB2rVar->eSpeed != STC_SPEED_TYPE_FORWARD_1X && prFrcPrm->u1B2R3DTrickModeType == B2R_3D_TRICK_MODE_3D_VIEW))
            && ((u1AppMode == FBM_FBG_APP_Frame_Pack_3D) || (u1AppModeSub == FBM_FBG_APP_Frame_Pack_3D)))
        {
            _B2R_GetFrameBufferForMVC3D(this);
        }
        else
#endif
#ifdef CC_B2R_ENABLE_CHG_FRAME_MSG
        if (u1AppMode == FBM_FBG_APP_OMX_DISP)
        {
            _B2R_GetFrameBufferForOMX(this, &(prB2rVar->fgGstPlayBack));
        }
        else
#endif // CC_B2R_ENABLE_CHG_FRAME_MSG
        if (u1AppMode == FBM_FBG_APP_SKYPE)
        {
            _B2R_GetFrameBufferForSkype(this);
        }
        else if (_B2R_IsBivl3D(this))
        {
            _B2R_GetFrameBufferForBivl3D(this);
        }
        else
        {
            _B2R_GetFrameBufferForGeneral(this);
        }


        if (prFrcPrm->ucFbId != 0xFF)
        {
            prFrcPrm->ucRealChangeFb = 1;
        }
        else
        {
            prB2rVar->u4TimeShfitChangeFrmCnt = 0;
            prB2rVar->fgTimeShfitThumbStartPlay = FALSE;
        }
    }while(FALSE);
    
    return ucRet;
}
static void _B2R_SetNewFrameBufferStatus(B2R_OBJECT_T* this)
{
    do
    {
        B2R_PRM_T* prFrcPrm;
        if (!this)
        {
            break;
        }
        if (this->ucB2rId >= B2R_NS)
        {
            break;
        }
        prFrcPrm = this->ptB2rPrm;
       
        if (prFrcPrm->ucFbId != FBM_FB_ID_UNKNOWN)
        {       
        
#ifdef CC_MAPLE_CUST_DRV
            FBM_SEQ_HDR_T *prFbmSeqHdr = FBM_GetFrameBufferSeqHdr(prFrcPrm->ucFbgId);
            if(prFbmSeqHdr)
            {
                if(prFbmSeqHdr->u4resolutionChg != prFbmSeqHdr->u4ChgPreStatus)
                {
                    DRVCUST_SendEvent(E_CUST_SIGNAL_FMT_CHANGE,SV_VP_MAIN);
                    DRVCUST_SendEvent(E_CUST_SIGNAL_FMT_CHANGE,SV_VP_PIP);
                }

                if(prFrcPrm->pt_cust_info)
                {
                    prFrcPrm->pt_cust_info->u4resolutionChg =
                        prFbmSeqHdr->u4resolutionChg;
                }
                prFbmSeqHdr->u4ChgPreStatus = prFbmSeqHdr->u4resolutionChg;//record current status
            }      
#endif
            _B2R_SetFrameBufferStatus(this, prFrcPrm->ucFbgId, prFrcPrm->ucFbId, FBM_FB_STATUS_LOCK);            
            if (FBM_ChkFbgCreateFromInst(prFrcPrm->ucFbgId) == FALSE)
             {
                if (FBM_CheckFrameBufferStatus(prFrcPrm->ucFbgId, prFrcPrm->ucFbId, FBM_FB_STATUS_LOCK) == 0)
                {
                 prFrcPrm->ucFbId = FBM_FB_ID_UNKNOWN;
                }
            }
         }
#ifdef CC_3D_MM_DS_SUPPORT
        if (prFrcPrm->ucSubFbId != FBM_FB_ID_UNKNOWN)
        {
            _B2R_SetFrameBufferStatus(this, prFrcPrm->ucSubFbgId, prFrcPrm->ucSubFbId, FBM_FB_STATUS_LOCK);
            if (FBM_ChkFbgCreateFromInst(prFrcPrm->ucSubFbgId) == FALSE)
             {
                if (FBM_CheckFrameBufferStatus(prFrcPrm->ucSubFbgId, prFrcPrm->ucSubFbId, FBM_FB_STATUS_LOCK) == 0)
                {
                  prFrcPrm->ucSubFbId= FBM_FB_ID_UNKNOWN;
                }
             }
        }
#endif
    }while(FALSE);
}
#ifdef CC_B2R_SUPPORT_GAME_MODE
 void VdpSetGameModeOnOff(UCHAR ucVdpId,B2R_GAME_MODE_TYPE_T eModeType)
{
    if(ucVdpId<VDP_NS)
    {
        UCHAR ucB2rId = VDP_GetB2rId(ucVdpId);
        eGameMode[ucB2rId] = eModeType;
       LOG(0,"VdpSetGameMode   ucB2rId=%d,eModeType=%d\n",ucB2rId,eModeType);
    }
    else
    {
       LOG(0,"ucVdpId->NULL\n");
    }
}

static void VdpInitreleaseDiaplayQ(void)
{
    INT32 i;
    _rB2rRleaseFb.i4FbCount=0;
    _rB2rRleaseFb.i4FbWriteIdx=0;
    _rB2rRleaseFb.i4FbReadIdx=0;
    for(i=0;i<VDP_MAX_RELEASE_Q;i++)
      {
         _rB2rRleaseFb.i4FbReleaseQ[i]=FBM_FB_ID_UNKNOWN;
      }
}
static void vdpSetFrameToRelease( UCHAR ucFbId)
{
   if((ucFbId!=FBM_FB_ID_UNKNOWN))
    {
        LOG(4,"Put to Release=%d,i4FbWriteIdx=%d,i4FbCount=%d\n",ucFbId,_rB2rRleaseFb.i4FbWriteIdx,_rB2rRleaseFb.i4FbCount);
       _rB2rRleaseFb.i4FbReleaseQ[_rB2rRleaseFb.i4FbWriteIdx]=ucFbId;
       _rB2rRleaseFb.i4FbCount++;
       _rB2rRleaseFb.i4FbWriteIdx=(_rB2rRleaseFb.i4FbWriteIdx+1)%VDP_MAX_RELEASE_Q;
    }
   else
    {
         LOG(1,"ucFbId is UNknown\n");
    }
}

static void vdpReadyReleaseFB( B2R_OBJECT_T * this)
{

    UCHAR ucFbId;
    B2R_PRM_T* prFrcPrm;
    prFrcPrm = &_arVdpPrm[this->ucB2rId];
    if(_rB2rRleaseFb.i4FbCount>2)
    {
     while(_rB2rRleaseFb.i4FbCount>1)
       {
           ucFbId=_rB2rRleaseFb.i4FbReleaseQ[_rB2rRleaseFb.i4FbReadIdx];
           LOG(4,"Put to Empty ucFbId=%d,i4FbReadIdx=%d,i4FbCount=%d\n",ucFbId,_rB2rRleaseFb.i4FbReadIdx,_rB2rRleaseFb.i4FbCount);
          _rB2rRleaseFb.i4FbCount--;
          _rB2rRleaseFb.i4FbReadIdx=(_rB2rRleaseFb.i4FbReadIdx+1)%VDP_MAX_RELEASE_Q;
           _B2R_SetFrameBufferStatus(this, prFrcPrm->ucFbgId, ucFbId, FBM_FB_STATUS_EMPTY);
       }
    }
}
#endif
static UINT32 _B2R_CalculateTFF(B2R_OBJECT_T * this)
{
#ifdef CC_FORCE_32_FLAG_ERR
    B2R_PRM_T* prFrcPrm;
    if (!this)
    {
        return B2R_INV_ARG;
    }
    if (!this || this->ucB2rId >= B2R_NS)
    {
        return B2R_INV_ARG;
    }
    prFrcPrm = this->ptB2rPrm;
    
    if (prFrcPrm->ucValid32Seq)
    {
        static UINT32 u4Force32Idx = 0;

        if (++u4Force32Idx >= 16)
        {
            if (prFrcPrm->ucRff)
            {
                if (prFrcPrm->ucTff)
                {
                    prFrcPrm->ucTff = 0;
                }
                else
                {
                    prFrcPrm->ucTff = 1;
                }
            }
        }

        if (u4Force32Idx >= (16 + (PD_32_FLAG_LEAVE_LOOP_SIZE / 2)))
        {
            u4Force32Idx = 0;
        }
    }
#endif
    return B2R_OK;
}

static BOOL _B2R_PreProessNewFrameBuffer(B2R_OBJECT_T* this)
{   
    BOOL fgRepeatFbForAvsync = FALSE;
    UCHAR ucB2rId=0;
    UCHAR ucFrameEncode=0;
    FBM_PIC_HDR_T* prNextPicHdr=NULL;
    B2R_VAR_T* prB2rVar=NULL;
    UINT8 u1DecoderSrcId=0;
    FBM_SEQ_HDR_T* prSeqHdr = NULL;
    VDP_CFG_T* prVdpConf;
    B2R_PRM_T* prFrcPrm;
    
#ifdef ENABLE_MULTIMEDIA
    VDEC_ES_INFO_T* prVdecEsInfo = NULL;
            
#endif
    if(!this)
    {
        LOG(0,"prVdecEsInfo == NULL, oops\n");
        return FALSE;
    }
    ucB2rId=this->ucB2rId;
    if (ucB2rId >= B2R_NS)
    {
        LOG(0, "ucB2rId >= B2R_NS , oops \n");
        return FALSE;
    }

    prFrcPrm = this->ptB2rPrm;
    prB2rVar = this->ptB2rVar;
    prVdpConf = _B2R_GetDP(this);
    if (!prVdpConf)
    {
        return FALSE;
    }

    //for DTV00320479
    prFrcPrm->fgFastForward = !(FBM_ChkFrameBufferPicFlag(prFrcPrm->ucFbgId, prFrcPrm->ucFbId, FBM_MM_BACKWARD_FLAG));

    // backup picture header info
    prNextPicHdr = FBM_GetFrameBufferPicHdr(prFrcPrm->ucFbgId, prFrcPrm->ucFbId);

    if (prNextPicHdr != NULL)
    {
        FBM_PIC_HDR_T *prPic;
        //         BOOL fgABReach      = FALSE;  // move to _VdpHandleABRepeat
        BOOL fgFastForward;

        if (prB2rVar->eSpeed <= STC_SPEED_TYPE_FORWARD_1_DOT_5X)
        {
            fgFastForward = TRUE;
        }
        else
        {
            fgFastForward = FALSE;
        }


        if (FBM_ChkFrameBufferPicFlag(prFrcPrm->ucFbgId, prFrcPrm->ucFbId, FBM_MM_SYNC_POINT_FLAG))
        {
            FBM_ClrFrameBufferPicFlag(prFrcPrm->ucFbgId, prFrcPrm->ucFbId, FBM_MM_SYNC_POINT_FLAG);
            //_VDP_VsyncNotify(, (UINT32)ucVdpId, VDP_COND_SYNC_POINT, 0);
            _B2R_PostNotify(this, VDP_MSG_MM_CB, VDP_COND_SYNC_POINT, 0);
        }

        if (FBM_ChkFrameBufferPicFlag(prFrcPrm->ucFbgId, prFrcPrm->ucFbId, FBM_MM_3D_CHG_FLAG))
        {
            FBM_ClrFrameBufferPicFlag(prFrcPrm->ucFbgId, prFrcPrm->ucFbId, FBM_MM_3D_CHG_FLAG);
            FBM_SetFrameBufferFlag(prFrcPrm->ucFbgId, FBM_FLAG_3D_SEQ_CHG);
        }

        if (!prB2rVar->rVdpSeek.fgABReach)
        {


            if (_B2R_HandleEos(this))
            {
                 return FALSE;
            }

        }

        prPic = FBM_GetFrameBufferPicHdr(prFrcPrm->ucFbgId, prFrcPrm->ucFbId);
        if ((prPic) && (prB2rVar->rAB.fgValid))
        {
            _B2R_HandleABRepeat(this, prPic, fgFastForward);
        }

        // Check Temporal reference
        if (prFrcPrm->ucLastFbId != FBM_FB_ID_UNKNOWN)
        {
            if ((prNextPicHdr->i4TemporalRef!= (1 + prFrcPrm->i4TemporalRef)) &&
                    (prNextPicHdr->i4TemporalRef != 0))
            {
                // [LOG] Discontinuous temporal reference
                SYSLOG(VDP_PREFIX + 97, prFrcPrm->ucFbgId, prFrcPrm->i4TemporalRef, prNextPicHdr->i4TemporalRef );

                LOG(10, "Fbg(%d) Temporal Ref %d >> %d\n"
                    , prFrcPrm->ucFbgId
                    , prFrcPrm->i4TemporalRef
                    , prNextPicHdr->i4TemporalRef);

                if (prFrcPrm->prFbCounter)
                {
                    prFrcPrm->prFbCounter->u4DispSkip++;
                }
            }
        }

        _B2R_CpyPicHdr2B2r(prFrcPrm, prNextPicHdr);
        prFrcPrm->ucLastRff = prFrcPrm->ucRff;
        prNextPicHdr->fgDummyFrame = 0;
        
        prVdpConf->u4QpAvg = prNextPicHdr->u4QpAvg;

        /* VDP trigger mode + audio master to do trigger mode */
        /* For Trick to Normal or Audio language change */
#ifndef CC_B2R_WAIT_NPTV_STABLE
        if (!(_B2R_GetStepForward(ucB2rId) == VDP_PLAY_STEP_FORWARD) && prFrcPrm->ucReady)
        {
            /* Trigger Audio while not step forward state */
            _B2R_TriggerAudOn(this, prFrcPrm->u4Pts);
        }
#endif
        _B2R_CalculateTFF(this);
    }
    else
    {
        return FALSE;
    }


    // Sequence Header Change, update size, frame rate and aspect ratio
    if ((FBM_ChkFrameBufferFlag(prFrcPrm->ucFbgId, FBM_FLAG_SEQ_CHG) ||
            FBM_ChkFrameBufferFlag(prFrcPrm->ucFbgId, FBM_FLAG_3D_SEQ_CHG) ||
            FBM_ChkFrameBufferFlag(prFrcPrm->ucFbgId, FBM_FLAG_SEQ_CHG_SPEEDUP) ||
            (prB2rVar->fgInvalidFrmRate) ||
            (!prFrcPrm->ucSeqInfoValid)) && (prFrcPrm->ucNewFbArrive == 0))
    {
        if (_B2R_GetSequenceInfo(this,FALSE) == FALSE)
        {
            LOG(1, "Get sequence info failed (with fb)\n");
             return FALSE;
        }

        prB2rVar->fgHandleSeqChg = FALSE;
    }
    else if ((FBM_ChkFrameBufferFlag(prFrcPrm->ucFbgId, FBM_FLAG_SEQ_CHG) ||
              FBM_ChkFrameBufferFlag(prFrcPrm->ucFbgId, FBM_FLAG_3D_SEQ_CHG) ||
              FBM_ChkFrameBufferFlag(prFrcPrm->ucFbgId, FBM_FLAG_SEQ_CHG_SPEEDUP) ||
              (prB2rVar->fgInvalidFrmRate) ||
              (!prFrcPrm->ucSeqInfoValid)) && (prFrcPrm->ucNewFbArrive == 1))
    {
        prB2rVar->fgHandleSeqChg = TRUE;
    }

    //calculate repeat count for push trick mode before drop and repeat
    if (!(prB2rVar->fgDropFb || fgRepeatFbForAvsync))
    {
        _B2R_CalcRepeatCntForPushTrickMode(prFrcPrm, FALSE);
    }
    // Backup Last Frame Buffer ID
    prFrcPrm->ucLastFbId = prFrcPrm->ucFbId;
    prB2rVar->fgDropFb = FALSE;

    // Frame Rate Conversion
#ifdef CC_B2R_SUPPORT_GAME_MODE
    if(eGameMode[this->ucB2rId]==B2R_GAME_MODE_OFF)
    {
        this->ptB2rPrm->fgGameModeEnable =FALSE;
    }
    else
    {
        this->ptB2rPrm->fgGameModeEnable =TRUE;
    }
#endif
     
    _B2R_CalculateVsyncNs(this);
     

    prSeqHdr = FBM_GetFrameBufferSeqHdr(prFrcPrm->ucFbgId);
    if ((prSeqHdr)&&(prB2rVar->fgInvalidFrmRate)&&(prB2rVar->ucFrameRateUnkownCount <= 0x5))
    {
        if ( ( FBM_FB_ID_UNKNOWN != prFrcPrm->ucFbId) &&
                ( MPEG_FRAME_RATE_UNKNOWN == prSeqHdr->ucFrmRatCod ) )
        {
            // pending for unkown frame rate
            _B2R_PreparePendingForNextVsync(this);

            (prB2rVar->ucFrameRateUnkownCount)++;
            LOG(3, "pending for unkown frame rate(%d) \n", prB2rVar->ucFrameRateUnkownCount);
        }
    }

    // Dummy frame contain frame infomation but not to display
    if (prFrcPrm->fgDummyFrame)
    {
        // not display this frame
        prFrcPrm->u2TargetNs = 0;
        prFrcPrm->u2ChangeFieldNs = 0;
    }

    if (prB2rVar->rAB.fgValid)
    {
        if (prB2rVar->rVdpSeek.fgABReach)
        {
            // not display this frame
            prFrcPrm->u2TargetNs = 0;
            prFrcPrm->u2ChangeFieldNs = 0;
        }
    }

    if (prFrcPrm->ucFbId != FBM_FB_ID_UNKNOWN)
    {
        /* prFrcPrm->u2TargetNs should be 0 if  prFrcPrm->ucFbId = FBM_FB_ID_UNKNOWN
           for AVS-R case */
        if (FBM_ChkFrameBufferPicFlag(prFrcPrm->ucFbgId, prFrcPrm->ucFbId, FBM_MM_NOT_DISPLAY_FLAG))
        {
            FBM_ClrFrameBufferPicFlag(prFrcPrm->ucFbgId, prFrcPrm->ucFbId, FBM_MM_NOT_DISPLAY_FLAG);

            prFrcPrm->u2TargetNs = 0;
            LOG(7, "No display (%d %d %d)\n", ucB2rId, prFrcPrm->ucFbgId, prFrcPrm->ucFbId);
        }
#if (defined( DIVX_PLUS_CER) && defined(CC_B2R_CHAPTER_CHG))
        _B2R_HandleChapter(this);  // LEO_B2R_REFACTORY
#endif


#ifdef CC_B2R_3D_ERROR_SUPPORT
        if (!FBM_ChkFrameBufferFlag(prFrcPrm->ucFbgId,FBM_FLAG_3D_ERROR_HANDLE_DISABLE))
        {
            if (prFrcPrm->fgB2R3DError && prFrcPrm->fgB2R3DEnable && !prFrcPrm->fgB2RForce2D)
            {
                prFrcPrm->u4B2R3DSuccessCount++;
                if (prFrcPrm->u4B2R3DSuccessCount > B2R_3D_ERROR_RECOVER_COUNT)
                {
                    FBM_SEQ_HDR_T* prSeqHdr;
                    prSeqHdr = FBM_GetFrameBufferSeqHdr(prFrcPrm->ucFbgId);
                    prSeqHdr->fgB2RForce2D = FALSE;
                    prSeqHdr->u1B2RForce2DType = B2R_FORCE_2D_L_VIEW;
                    FBM_SetFrameBufferFlag(prFrcPrm->ucFbgId, FBM_FLAG_3D_SEQ_CHG);
                    prFrcPrm->fgB2R3DError = FALSE;
                }
            }
            prFrcPrm->u4B2R3DErrorCount =0;
        }
#endif
        prFrcPrm->fgDispFail = FALSE;
    }
    else
    {
#ifdef CC_B2R_3D_ERROR_SUPPORT
        if (!prFrcPrm->fgB2R3DError && prFrcPrm->fgB2R3DEnable && !prFrcPrm->fgB2RForce2D)
        {
            prFrcPrm->u4B2R3DErrorCount++;
            if (prFrcPrm->u4B2R3DErrorCount > B2R_3D_ERROR_COUNT)
            {
                FBM_SEQ_HDR_T* prSeqHdr;
                prSeqHdr = FBM_GetFrameBufferSeqHdr(prFrcPrm->ucFbgId);
                prSeqHdr->fgB2RForce2D = TRUE;
                prSeqHdr->u1B2RForce2DType = B2R_FORCE_2D_L_VIEW;
                FBM_SetFrameBufferFlag(prFrcPrm->ucFbgId, FBM_FLAG_3D_SEQ_CHG);
                prFrcPrm->fgB2R3DError = TRUE;
            }
        }
        prFrcPrm->u4B2R3DSuccessCount = 0;
#endif
        prFrcPrm->fgDispFail = TRUE;
        prB2rVar->u4TimeShfitChangeFrmCnt = 0;
        prB2rVar->fgTimeShfitThumbStartPlay = FALSE;
    }

    /* Seek Finish Notify at 20081212 */

    u1DecoderSrcId = FBM_GetDecoderSrcId(prFrcPrm->ucFbgId);
#ifdef ENABLE_MULTIMEDIA 
    if (u1DecoderSrcId < VDEC_MAX_ES)
    {
        prVdecEsInfo = _VDEC_GetEsInfo(u1DecoderSrcId);
    }
    else if(FBM_GetFbgAppMode(prFrcPrm->ucFbgId) != FBM_FBG_APP_MTIMAGE)
    {
        LOG(0,"u1DecoderSrcId > VDEC_MAX_ES , oops \n");
        prVdecEsInfo = _VDEC_GetEsInfo(ES0);
    }
    
    if(prVdecEsInfo)
    {
        _B2R_SeekFinishNtfy(this, (prVdecEsInfo->eMMSrcType == SWDMX_SRC_TYPE_NETWORK_DASH)); 
    }
#else
    _B2R_SeekFinishNtfy(this, FALSE); 
#endif

#ifdef CC_B2R_WAIT_NPTV_STABLE
    //UCHAR ucPlayMode;
    //FBM_GetPlayMode(prFrcPrm->ucFbgId,&ucPlayMode);
    //if( FBM_FBG_MM_MODE == ucPlayMode )
    {
        /* VDP trigger mode + audio master to do trigger mode */
        /* For Trick to Normal or Audio language change */
        //if(!((VDP_HalGetStepForward(ucB2rId) == VDP_PLAY_STEP_FORWARD)) && prFrcPrm->ucFbId != FBM_FB_ID_UNKNOWN)
        if (!((_B2R_GetStepForward(ucB2rId) == VDP_PLAY_STEP_FORWARD))
                && prFrcPrm->ucFbId != FBM_FB_ID_UNKNOWN &&
                prFrcPrm->ucReady && prB2rVar->fgNPTVStable)

        {
            /* Trigger Audio while not step forward state */
            _B2R_TriggerAudOn(this, prFrcPrm->u4Pts);
        }
    }
#endif
    // if no field change required, u2ChangeFieldNs = 0
    if (prFrcPrm->u2TargetNs < prFrcPrm->u2ChangeFieldNs)
    {
        LOG(5, "u2TargetNs > u2ChangeFieldNs\n");
    }

    // frame/field encode
    // According to suggestion of DVD Benchmark
    if (prFrcPrm->ucProgressiveFrame)
    {
        ucFrameEncode = 1;
    }
    else if (prFrcPrm->ucLastProgressiveFrame && prFrcPrm->ucLastRff)
    {
        ucFrameEncode = 1;
    }
    else
    {
        ucFrameEncode = 0;
    }

    // frame/field encode type change
    if (prFrcPrm->u2TargetNs != 0)
    {
#ifdef CC_VDP_FORCE_FIELD_ENCODE
        if (ucFrameEncode != prFrcPrm->ucHwFrameEncode)
        {
            prFrcPrm->ucHwFrameEncode = ucFrameEncode;
            _VDP_HalSetChromaMode(ucB2rId, 0, 0);
        }
#else
        // 5381
        // 1) frame encode & TFF stable >> frame encode
        // 2) else field encode

        if (ucFrameEncode != 0)
        {
            if (prFrcPrm->ucLastTff == prFrcPrm->ucTff)
            {
                if (prFrcPrm->ucTffStableCounter < VDP_TFF_STABLE_COUNT)
                {
                    prFrcPrm->ucTffStableCounter++;
                }
            }
            else
            {
                prFrcPrm->ucTffStableCounter = 0;
            }

            if (prFrcPrm->ucTffStableCounter < VDP_TFF_STABLE_COUNT)
            {
                ucFrameEncode = 0;
            }
        }

        prFrcPrm->ucLastTff = prFrcPrm->ucTff;

        if (ucFrameEncode != prFrcPrm->ucHwFrameEncode)
        {
            prFrcPrm->ucHwFrameEncode = ucFrameEncode;
            //_VDP_HalSetChromaMode(ucB2rId, prFrcPrm->ucHwFrameEncode, prFrcPrm->ucTff);
        }
#endif
    }

    // clear log
    if (prFrcPrm->prFbCounter)
    {
        if ((prFrcPrm->ucReady == 0) &&
                (prFrcPrm->u2TargetNs != 0))
        {
            prFrcPrm->prFbCounter->u4DispOk = 0;
            prFrcPrm->prFbCounter->u4DispFail = 0;
            prFrcPrm->prFbCounter->u4DispSkip = 0;
            prFrcPrm->prFbCounter->u4DispRepeat = 0;
            prFrcPrm->prFbCounter->u4DispDrop = 0;

#ifdef CHANNEL_CHANGE_LOG
            STC_CLOCK rStcClk;
            UCHAR ucAvSyncMode;
            UCHAR ucStcSrc;
            HAL_TIME_T dt;

            FBM_GetSyncStc(prFrcPrm->ucFbgId, &ucAvSyncMode, &ucStcSrc);
            STC_GetSrc(ucStcSrc, &rStcClk);
            HAL_GetTime(&dt);
            LOG(0, " %u.%06u s [AV SYNC] 8 VDP 1st Video (PTS,STC)=(0x%x,0x%x)\n", dt.u4Seconds, dt.u4Micros, prFrcPrm->u4Pts, rStcClk.u4Base);
#endif
#ifdef TIME_MEASUREMENT
            TMS_DIFF_EX(TMS_FLAG_CHG_CHL, TMS_CHL_CHE_TIME_DRV, "REC_1st_VID, B2R");
#endif
            /* boot up measurement in release version, record once.*/
            if (!(prB2rVar->fgB2RProfile))
            {
                x_os_drv_set_timestamp("REC_1st_VID, B2R");
                prB2rVar->fgB2RProfile = TRUE;
            }
        }

        prFrcPrm->prFbCounter->u4DispOk++;
    }
    else
    {
        ASSERT(0);
    }
    
    if (prFrcPrm->u2TargetNs == 0)
    {
        if (prFrcPrm->ucFbId != FBM_FB_ID_UNKNOWN)
        {
            LOG(7, "D (%d %d %d)\n", ucB2rId, prFrcPrm->ucFbgId, prFrcPrm->ucFbId);
            _B2R_SetFrameBufferStatus(this, prFrcPrm->ucFbgId, prFrcPrm->ucFbId, FBM_FB_STATUS_EMPTY);
#ifdef CC_3D_MM_DS_SUPPORT
            if (prFrcPrm->ucSubFbId != FBM_FB_ID_UNKNOWN)
            {
                LOG(7, "D (%d %d %d)\n", ucB2rId, prFrcPrm->ucSubFbgId, prFrcPrm->ucSubFbId);
                _B2R_SetFrameBufferStatus(this, prFrcPrm->ucSubFbgId, prFrcPrm->ucSubFbId, FBM_FB_STATUS_EMPTY);
            }
#endif
            prB2rVar->fgDropFb = TRUE;
        }
        else
        {
            ASSERT(prFrcPrm->ucPendingFbId != FBM_FB_ID_UNKNOWN);
            prFrcPrm->ucLastFbId = FBM_FB_ID_UNKNOWN;
            fgRepeatFbForAvsync = TRUE;
        }
    }

    // [LOG] VDP get display FB success
    SYSLOG(VDP_PREFIX + prFrcPrm->ucFbgId, prFrcPrm->ucFbId, prFrcPrm->prFbCounter->u4DispOk, prFrcPrm->prFbCounter->u4DispFail);
    return TRUE;

}


static void _B2R_GetNextDisp(B2R_OBJECT_T* this)
{
    B2R_PRM_T* prFrcPrm;
    BOOL fgRepeatFbForNoFb = FALSE;
    UCHAR ucBackupFbId;
#ifdef CC_3D_MM_DS_SUPPORT
    UCHAR ucBackupSubFbId;
#endif
    UCHAR ucB2rId;
    UINT8 u1AppMode =0;
    B2R_VAR_T* prB2rVar;
    BOOL fgRet = TRUE;
#ifdef CC_3D_MM_DS_SUPPORT
    UINT8 u1AppModeSub =0;
#endif
    

    if (!this)
    {
        return;
    }
    ucB2rId = this->ucB2rId;
    if (ucB2rId >= B2R_NS)
    {
        LOG(0, "ucB2rId >= B2R_NS , oops \n");
        return;
    }

    prFrcPrm = this->ptB2rPrm;
    prB2rVar = this->ptB2rVar;
#ifdef CC_HWCOMP_UT
    u1AppMode = FBM_FBG_APP_OMX_DISP;
#else
    u1AppMode = FBM_GetFbgAppMode(prFrcPrm->ucFbgId);
#endif

    ucBackupFbId = prFrcPrm->ucFbId;
#ifdef CC_3D_MM_DS_SUPPORT
    ucBackupSubFbId = prFrcPrm->ucSubFbId;
#endif
    prFrcPrm->u2TargetNs = 0;
    prFrcPrm->ucMMWaitSTC = 0;
    prFrcPrm->ucRealChangeFb = 0;
    prB2rVar->fgGstPlayBack = FALSE;

#if defined(CC_DTV_SUPPORT_LG)
    if (prB2rVar->fgTimeShift)
    {
        prB2rVar->fgGstPlayBack = TRUE;
    }
#endif
    
    do
    {
#ifdef CC_3D_MM_DS_SUPPORT
        _B2R_GetAdditionalFbgId(this);
        if (prFrcPrm->ucSubFbgId != FBM_FBG_ID_UNKNOWN)
        {
            u1AppModeSub = FBM_GetFbgAppMode(prFrcPrm->ucSubFbgId);
        }
#endif
#ifdef CC_B2R_SUPPORT_GAME_MODE
        if(eGameMode[ucB2rId]==B2R_GAME_MODE_OFF)
#endif
        {
            if (PENDING_IS_NEEDED)
            {  
                _B2R_GetFrameBufferFromPending(this);
            }
            else
            {
                _B2R_GetFrameBuffer(this);
            }
        }
#ifdef CC_B2R_SUPPORT_GAME_MODE
        else if((eGameMode[ucB2rId]==B2R_GAME_MODE_LOW_DELAY)||(eGameMode[ucB2rId]==B2R_GAME_MODE_HIGHT_QUALITY))
        {
             LOG(3,"prFrcPrm->ucReady=%d,Display Q=%d\n",prFrcPrm->ucReady,FBM_CheckFrameBufferDispQ(prFrcPrm->ucFbgId));
             if(FBM_CheckFrameBufferDispQ(prFrcPrm->ucFbgId) > 2)
             {
               FBM_PIC_HDR_T* prPicHdr = NULL;
               _B2R_GameModeGetFrameBuffer(this,2);//drop frame if display Q >2 frame
               prPicHdr = FBM_GetFrameBufferPicHdr(prFrcPrm->ucFbgId, prFrcPrm->ucFbId);
               LOG(2, "[OMX Game mode H Q>2] fbid (%d, %d)\n", prFrcPrm->ucFbId, (prPicHdr?prPicHdr->i4TemporalRef:0));
             }
             else
             {
                FBM_PIC_HDR_T* prPicHdr = NULL;
                prFrcPrm->ucFbId = FBM_GetFrameBufferFromDispQ(prFrcPrm->ucFbgId);
                prPicHdr = FBM_GetFrameBufferPicHdr(prFrcPrm->ucFbgId, prFrcPrm->ucFbId);
                LOG(2, "[OMX Game mode H] fbid (%d, %d)\n", prFrcPrm->ucFbId, (prPicHdr?prPicHdr->i4TemporalRef:0));
             }
        }
#endif
        if (FBM_GetFrameBufferNs(prFrcPrm->ucFbgId) == 1)
        {
            _B2R_OneFbChgHandle(prFrcPrm);
        }
    
        _B2R_SetNewFrameBufferStatus(this);

        if (prFrcPrm->ucFbId != FBM_FB_ID_UNKNOWN)
        {
            fgRet = _B2R_PreProessNewFrameBuffer(this);
            if(!fgRet)
            {
                 LOG(0,"_vdpPreProess Return Null\n");
                 return;
            }
        }
        else
        {
            UCHAR ucAvSyncMode;
            UCHAR ucStcSrc;
            fgRepeatFbForNoFb = TRUE;
            prB2rVar->fgDropFb = FALSE;
            prFrcPrm->fgDispFail = TRUE;
            _B2R_CalcRepeatCntForPushTrickMode(prFrcPrm, TRUE);
            if ((prB2rVar->fgHandleSeqChg) || FBM_ChkFrameBufferFlag(prFrcPrm->ucFbgId, FBM_FLAG_SEQ_CHG_SPEEDUP))
            {
                if (_B2R_GetSequenceInfo(this,FALSE) == FALSE)
                {
                    LOG(1, "Get sequence info failed (without fb)\n");
                    return;
                }

                prB2rVar->fgHandleSeqChg = FALSE;

#ifdef CC_B2R_WAIT_NPTV_STABLE
                prB2rVar->u4WaitNPTVStableCount = 0;
#endif
            }
#ifndef VDP_B2R_PSCAN_CRC_FLOW

            // ask mpeg decode to skip one picture due to repeat, if AV Sync NOT enable
            FBM_GetSyncStc(prFrcPrm->ucFbgId, &ucAvSyncMode, &ucStcSrc);
            if (ucAvSyncMode == VID_SYNC_MODE_NONE )
            {
//#ifdef ENABLE_MULTIMEDIA
//                if(prVdecEsInfo->eMMSrcType != SWDMX_SRC_TYPE_NETWORK_SKYPE)
//#endif
                if ((u1AppMode != FBM_FBG_APP_OMX_DISP) && (u1AppMode != FBM_FBG_APP_SKYPE))
                {
                    FBM_SetFrameBufferFlag(prFrcPrm->ucFbgId, FBM_FLAG_SKIP_PIC);
#if (defined(CC_3D_MM_DS_SUPPORT) && defined(CC_B2R_ENABLE_R_VIEW_SKIP_FRAME))
                    if (prFrcPrm->ucSubFbgId != FBM_FBG_ID_UNKNOWN && u1AppModeSub == FBM_FBG_APP_Frame_Pack_3D)
                    {
                        FBM_SetFrameBufferFlag(prFrcPrm->ucSubFbgId, FBM_FLAG_SKIP_PIC);
                    }
#endif
                }
            }
#endif

            if (prFrcPrm->prFbCounter)
            {
                prFrcPrm->prFbCounter->u4DispFail++;
                //LOG(3, "DispFail++\n");
            }
            else
            {
                ASSERT(0);
            }

#ifdef ENABLE_MULTIMEDIA
            _B2R_SeekErrHandle(this);
#endif
        }
    }
#ifdef CC_B2R_SUPPORT_GAME_MODE
    while (REFETCH_IS_NEEDED&&(eGameMode[prFrcPrm->ucB2rId]==B2R_GAME_MODE_OFF));
#else
    while (REFETCH_IS_NEEDED);
#endif
    if (prFrcPrm->u2TargetNs == 0)
    {
        prFrcPrm->ucFbId = ucBackupFbId;
#ifdef CC_3D_MM_DS_SUPPORT
        prFrcPrm->ucSubFbId = ucBackupSubFbId;
#endif
    }
    _B2R_DetermineRepeatForPushTrickMode(this, fgRepeatFbForNoFb);
}
//-----------------------------------------------------------------------------
/** Brief of _B2R_FrcFrameBufferReady.
 */
//-----------------------------------------------------------------------------

UCHAR _B2R_Fbg2B2r(UCHAR ucFbgId)
{
    UCHAR ucIdx;
    UCHAR ucB2rId;
    ucB2rId = B2R_NS;
#ifdef CC_B2R_SUPPORT_GAME_MODE
for (ucIdx = 0; ucIdx < B2R_NS; ucIdx++)
    {
        if ((_arVdpPrm[ucIdx].ucFbgId == ucFbgId) &&
                ((_arVdpPrm[ucIdx].ucPendingNewFb != 0)||(eGameMode[ucIdx]==B2R_GAME_MODE_LOW_DELAY)))
        {
            ucB2rId = ucIdx;
            ucIdx = B2R_NS;
        }
    }

#else
    for (ucIdx = 0; ucIdx < B2R_NS; ucIdx++)
    {
        if ((_arVdpPrm[ucIdx].ucFbgId == ucFbgId) &&
                (_arVdpPrm[ucIdx].ucPendingNewFb != 0))
        {
            ucB2rId = ucIdx;
            ucIdx = B2R_NS;
        }
    }
#endif
#if defined(CC_B2R_ENABLE_3D_LATE_NOTIFY) && defined(CC_3D_MM_DS_SUPPORT)
    if (ucB2rId == B2R_NS)
    {
        for (ucIdx = 0; ucIdx < B2R_NS; ucIdx++)
        {
            if ((_arVdpPrm[ucIdx].ucSubFbgId != FBM_FBG_ID_UNKNOWN) && (_arVdpPrm[ucIdx].ucSubFbgId == ucFbgId) &&
                    (_arVdpPrm[ucIdx].ucPendingNewFb != 0))
            {
                ucB2rId = ucIdx;
                ucIdx = B2R_NS;
            }
        }
    }
#endif

    return ucB2rId;

}

static void _B2R_FrcFrameBufferReady(FB_CHG_INFO_T* pt_fb)
{
    UCHAR ucB2rId;
    UCHAR ucFbgId;
    UCHAR ucFbId;
    UINT32 u4CurrentLine;
    UCHAR ucCntL=0, ucCntR=0;
    B2R_PRM_T *  ptVdpPrm;
    B2R_OBJECT_T* this;
    FBM_PIC_HDR_T* prFbmPicHdr = NULL;
    // get video plane id
    
    if(!pt_fb)
    {
        return;
    }

    ucFbId  = pt_fb->ucFbid;
    ucFbgId = pt_fb->ucFbgId;
    if(ucFbgId == FBM_FBG_ID_UNKNOWN ||
        ucFbId == FBM_FB_ID_UNKNOWN)
    {
        return;
    }
    
    ucB2rId = _B2R_Fbg2B2r(ucFbgId);

    if (ucB2rId >= B2R_NS)
    {
        return;
    }
    this = _B2R_GetObj(ucB2rId);
    if(!this)
    {
        return;
    }
    
    ptVdpPrm = this->ptB2rPrm;
    if(!ptVdpPrm)
    {
        return;
    }
    
    if (FBM_ChkFrameBufferFlag(ucFbgId, FBM_FLAG_RELEASE_FBG))
    {
        return;
    }
 #ifdef CC_B2R_SUPPORT_GAME_MODE
    if(eGameMode[ucB2rId]!=B2R_GAME_MODE_LOW_DELAY)
 #endif
    {
    // After Semaphore Wait, ucPendingNewFb can be reset by B2R VSYNC
    ucCntL = FBM_CheckFrameBufferDispQ(ptVdpPrm->ucFbgId);
#if defined(CC_3D_MM_DS_SUPPORT)
    ucCntR = FBM_CheckFrameBufferDispQ(ptVdpPrm->ucSubFbgId);
#endif

    prFbmPicHdr = FBM_GetFrameBufferPicHdr(ptVdpPrm->ucFbgId, ucCntL);

    if (ptVdpPrm->fgB2RNoLock && prFbmPicHdr && !prFbmPicHdr->fgSeamlessDisp)
    {
        //PS: This pic hdr is not the next display FB while stage fright.
        //Just get the seamless info here.
        //B2R_MUTEX_UNLOCK(ucB2rId);
        return;
    }

    if(ptVdpPrm->t_wfd_prm.fgDnmcltncy)
    {
        return;
    }
    
    if(ptVdpPrm->t_b2r_vdp_uhd.fgEnable)
    {
        return;
    }
    
#if defined(CC_B2R_ENABLE_3D_LATE_NOTIFY) && defined(CC_3D_MM_DS_SUPPORT)
    if (ptVdpPrm->ucPendingNewFb == 0
            || (ptVdpPrm->fgB2R3DEnable && (!((ucCntL > 0) && (ucCntR > 0)))))
#else
    if (ptVdpPrm->ucPendingNewFb == 0 || ptVdpPrm->fgB2R3DEnable)
#endif
    {
        if (ptVdpPrm->ucPendingNewFb > 0)
        {
            LOG(1, "Pending dispQ %d/%d not ready\n", ucCntL, ucCntR);
        }
       // B2R_MUTEX_UNLOCK(ucB2rId);
        return;
    }
        }
    // reset pending (do it once)
    B2R_MUTEX_LOCK(ucB2rId);
    ptVdpPrm->ucPendingNewFb = 0;

    if(ucFbId == ptVdpPrm->ucFbId &&
        ucFbgId == ptVdpPrm->ucFbgId)
    {
        B2R_MUTEX_UNLOCK(ucB2rId);
        return;
    }
    
    // check active region (can we stilll update frame buffer now? or It is too late to update)
    //u4CurrentLine = _VDP_HalInsideBlankRegion(ucB2rId);
 
    if (B2R_HAL_Is(this->hB2r, B2R_HAL_IN_BLANK_REGION, &u4CurrentLine))
    {
        // frame rate config core
       LOG(5,"Check u4CurrentLine=%d\n",u4CurrentLine);
        switch (ptVdpPrm->ucMode)
        {
        case VDP_MODE_DEINT:
        case VDP_MODE_NORMAL:
        case VDP_MODE_BG:            // Background Mode, do nothing

#ifdef CC_B2R_ENABLE_3D_LATE_NOTIFY
            if (ptVdpPrm->fgB2R3DEnable)
            {
                LOG(2,"late notify frame in 3D case\n");
            }
#endif

            ptVdpPrm->ucNewFbArrive = 1;
#ifdef CC_B2R_SUPPORT_GAME_MODE
            ptVdpPrm->ucGameModeFrameChange =1;
#endif
            _B2R_ChangeFrameBuffer(this);

          //  ASSERT(ptVdpPrm->ucFbId != FBM_FB_ID_UNKNOWN);
            LOG(2,"ptVdpPrm->ucFbId=%d\n",ptVdpPrm->ucFbId);
        //     LOG(2, "FBR (%d) (%d)\n", _arVdpPrm[ucVdpId].ucFbId, _arVdpPrm[ucVdpId].prFbCounter->u4DispFail);
            break;

        default:
            break;
        }
    }
    else
    {
        LOG(2, "FB late \n");
    }

    B2R_MUTEX_UNLOCK(ucB2rId);
}

//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

static UINT32 _B2R_FrcGetStc(UCHAR ucFbgId)
{
    UCHAR ucAvSyncMode  = 0x0;
    UCHAR ucStcSrc      = 0x0;

    FBM_GetSyncStc(ucFbgId, &ucAvSyncMode, &ucStcSrc);

    return STC_GetStcValue(ucStcSrc);
}

static BOOL _B2R_IsFastForward(B2R_OBJECT_T* this)
{
    BOOL fgFastForward = TRUE;
    
    if (this)
    {
        fgFastForward = (this->ptB2rVar->eSpeed <= STC_SPEED_TYPE_FORWARD_1_DOT_5X) ? TRUE : FALSE;
    }

    return fgFastForward;
}

static BOOL _B2R_ChangeFrameDirectForOMX(B2R_OBJECT_T *this, VDP_B2R_CHG_FRAME_MSG_T* prMsg)
{
    B2R_PRM_T *prFrcPrm = NULL;
    B2R_VAR_T* prB2rVar = NULL;
    prFrcPrm = this->ptB2rPrm;
    prB2rVar = this->ptB2rVar;
    if (prMsg->u4DispMode == VDP_B2R_CHG_FRAME_DIRECT)
    {
        ASSERT(prMsg->ucFbgId == prFrcPrm->ucFbgId);
        prFrcPrm->ucFbId = prMsg->ucFbId;
        LOG_REL(prB2rVar->fgEnableVideoLog, 2, 7, "OMX DIRECT: FbgId(%d-%d)\n", prMsg->ucFbgId, prMsg->ucFbId);
        return TRUE;
    }
    
    return FALSE;
}

static UCHAR _B2R_FreeFrameBeforeEosHandler(B2R_OBJECT_T* this, BOOL fgMain, BOOL fgOMX, BOOL* fgEos)
{
    UCHAR ucFbId = FBM_FB_ID_UNKNOWN;
    B2R_PRM_T* prFrcPrm;
    if (!this || this->ucB2rId >= B2R_NS)
    {
        return ucFbId;
    }
    prFrcPrm = this->ptB2rPrm;
    if (fgMain)
    {
        if (fgOMX)
        {
            ucFbId = prFrcPrm->ucFbId;
        }
        else
        {
            ucFbId = FBM_GetFrameBufferFromDispQ(prFrcPrm->ucFbgId);
        }
        *fgEos = FALSE;
        if (ucFbId == FBM_FB_ID_UNKNOWN)
        {
            prFrcPrm->ucFbId = FBM_FB_ID_UNKNOWN;
        }
        else if (FBM_ChkFrameBufferPicFlag(prFrcPrm->ucFbgId, ucFbId, FBM_MM_EOS_FLAG))//eos frame, empty it in later eos handler
        {
            prFrcPrm->ucFbId = ucFbId;
            *fgEos = TRUE;
            ucFbId = FBM_FB_ID_UNKNOWN;
        }
        else
        {
            _B2R_SetFrameBufferStatus(this, prFrcPrm->ucFbgId, ucFbId, FBM_FB_STATUS_LOCK);
            _B2R_SetFrameBufferStatus(this, prFrcPrm->ucFbgId, ucFbId, FBM_FB_STATUS_EMPTY);
            prFrcPrm->ucFbId = FBM_FB_ID_UNKNOWN;
        }
    }
#ifdef CC_3D_MM_DS_SUPPORT
    else
    {
        ucFbId = FBM_GetFrameBufferFromDispQ(prFrcPrm->ucSubFbgId);
        *fgEos = FALSE;
        if (ucFbId == FBM_FB_ID_UNKNOWN)
        {
            prFrcPrm->ucSubFbId = FBM_FB_ID_UNKNOWN;
        }
        if (FBM_ChkFrameBufferPicFlag(prFrcPrm->ucSubFbgId, ucFbId, FBM_MM_EOS_FLAG))//eos frame, empty it in later eos handler
        {
            prFrcPrm->ucSubFbId = ucFbId;
            *fgEos = TRUE;
            ucFbId = FBM_FB_ID_UNKNOWN;
        }
        else
        {
            _B2R_SetFrameBufferStatus(this, prFrcPrm->ucSubFbgId, ucFbId, FBM_FB_STATUS_LOCK);
            _B2R_SetFrameBufferStatus(this, prFrcPrm->ucSubFbgId, ucFbId, FBM_FB_STATUS_EMPTY);
            prFrcPrm->ucSubFbId = FBM_FB_ID_UNKNOWN;
        }
    }
#endif
    return ucFbId;
}

static BOOL _B2R_HandleEos(B2R_OBJECT_T *this)
{
    BOOL fgEos = FALSE;
    UCHAR ucFbgId = FBM_FB_ID_UNKNOWN;
    B2R_VAR_T* prB2rVar;
    B2R_PRM_T* prFrcPrm;
    UCHAR ucB2rId;

    
    if (!this)
    {
        return FALSE;
    }

    ucB2rId  = this->ucB2rId;
    prB2rVar = this->ptB2rVar;
    prFrcPrm = this->ptB2rPrm;

    if (FBM_ChkFrameBufferPicFlag(prFrcPrm->ucFbgId, prFrcPrm->ucFbId, FBM_MM_EOS_FLAG))
    {
        fgEos = TRUE;
        ucFbgId = prFrcPrm->ucFbgId;
        FBM_ClrFrameBufferPicFlag(prFrcPrm->ucFbgId, prFrcPrm->ucFbId, FBM_MM_EOS_FLAG);
        _B2R_SetFrameBufferStatus(this, prFrcPrm->ucFbgId, prFrcPrm->ucFbId, FBM_FB_STATUS_EMPTY);
        prFrcPrm->ucFbId = prFrcPrm->ucReleaseFbId;
        prFrcPrm->ucReleaseFbId = FBM_FB_ID_UNKNOWN;
    }
#ifdef CC_3D_MM_DS_SUPPORT
    if (FBM_ChkFrameBufferPicFlag(prFrcPrm->ucSubFbgId, prFrcPrm->ucSubFbId, FBM_MM_EOS_FLAG))
    {
        fgEos = TRUE;
        ucFbgId = prFrcPrm->ucSubFbgId;
        FBM_ClrFrameBufferPicFlag(prFrcPrm->ucSubFbgId, prFrcPrm->ucSubFbId, FBM_MM_EOS_FLAG);
        _B2R_SetFrameBufferStatus(this, prFrcPrm->ucSubFbgId, prFrcPrm->ucSubFbId, FBM_FB_STATUS_EMPTY);
        prFrcPrm->ucSubFbId= prFrcPrm->ucSubReleaseFbId;
        prFrcPrm->ucSubReleaseFbId = FBM_FB_ID_UNKNOWN;
    }
#endif

    if (fgEos)
    {
        UCHAR ucSeekMode, ucSeekNotifyTrigger;
        UINT32 u4SeekPts;
        if ((_B2R_GetStepForward(ucB2rId) == VDP_PLAY_STEP_FORWARD))
        {
            /* Play One Frame and then Pause */
            _B2R_SetPauseMM(ucB2rId, TRUE);
        }

        _B2R_GetSeek(this, &ucSeekMode, &u4SeekPts, &ucSeekNotifyTrigger);
        if ((ucSeekMode != 0 && ucSeekNotifyTrigger != 0) || prB2rVar->ucRemainTimeToSendSeekDone)
        {
            /* Seek Finish notify trigger status clear */
            prB2rVar->rVdpSeek.ucNotifyTrigger = 0;
            prB2rVar->ucRemainTimeToSendSeekDone = 0;
            /* Seek Finish Notification */
            _B2R_PostNotify(this, VDP_MSG_MM_SEEK_FIN_CB, u4SeekPts, VDP_SEEK_STEP_OK);
        }

        if (!prB2rVar->rAB.fgValid)	 // if in AB repeat mode, no need to notify EOS
        {
            _B2R_PostNotify(this, VDP_MSG_MM_CB, VDP_COND_EOS, FBM_GetDecoderSrcId(ucFbgId));
            LOG(3, "FBM_MM_EOS_FLAG fbg(%d) ES(%d) Notify\n", ucFbgId, FBM_GetDecoderSrcId(ucFbgId));
        }
        else
        {
            _B2R_PostNotify(this, VDP_MSG_MM_CB, VDP_COND_RANGE_AB_DONE, _B2R_IsFastForward(this));
            LOG(3, "VDP_COND_RANGE_AB_DONE Notify\n");
        }
    }
    return fgEos;
}

/* VDP trigger mode + audio master to do trigger mode */
VOID _B2R_TriggerAudReceive(UCHAR ucVdpId, UINT32 u4Pts)
{
    LOG(3,"ucVdpId:%d, u4Pts:0x%x!\n",ucVdpId, u4Pts);
    if(ucVdpId >= VDP_NS)
    {
        return;
    }

    _arVdpTrigAud[ucVdpId].u4TriggerPts = u4Pts;
	_arVdpTrigAud[ucVdpId].ucTriggerReceived = 1;
}

static UINT32 _B2R_TriggerAudOn(B2R_OBJECT_T *this, UINT32 u4CurPts)
{
    UCHAR ucB2rId;
    UCHAR ucVdpId;
    B2R_PRM_T *ptVdpPrm;
    VDP_TRIG_AUD_PRM_T* pt_aud_trigger;

    if (!this)
    {
        return B2R_INV_ARG;
    }

    ucB2rId  = this->ucB2rId;
    ptVdpPrm = this->ptB2rPrm;

    ucVdpId = _B2R_GetVdpId(ucB2rId);
    if(ucVdpId >= VDP_NS)
    {
        return B2R_INV_ARG;
    }

    pt_aud_trigger = &_arVdpTrigAud[ucVdpId];
    if(!pt_aud_trigger)
    {
        return B2R_INV_ARG;
    }
    
    if (pt_aud_trigger->ucTriggerReceived == 1)
    {
        if (u4CurPts >= pt_aud_trigger->u4TriggerPts)
        {
            LOG(1, "VDP Trigger Aud: CurPTS(%X), SetPTS(%X)\n", u4CurPts,pt_aud_trigger->u4TriggerPts);

            /* AUD trigger function */
            if (pt_aud_trigger->rTrgAudCbInfo.pfnTrigAud)
            {
                ((PFN_TRIG_AUD_ARRIVE_CB)pt_aud_trigger->rTrgAudCbInfo.pfnTrigAud)(pt_aud_trigger->rTrgAudCbInfo.pvTag);
            }
            else
            {
                _B2R_PostNotify(this, VDP_MSG_TRIG_AUD_TO_PLAY, u4CurPts, 0);
#ifdef CC_USE_DDI
            AUD_MMAoutEnable(AUD_DEC_AUX, TRUE);
#endif

#ifdef ENABLE_MULTIMEDIA
            UNUSED(SWDMX_SetInfo(ptVdpPrm->u4AttachedSrcId, eSWDMX_SET_AOUT_ENABLE, 0, 0, 0));
#else
            // actually this is no meaning.
            AUD_MMAoutEnable(0, TRUE);
#endif
            }
            
            x_memset(&pt_aud_trigger->rTrgAudCbInfo, 0, sizeof(VDP_TRIG_AUD_ARRIVE_CB_T));
            pt_aud_trigger->ucTriggerReceived = 0;
        }
    }

    return B2R_OK;
}

static void _B2R_SetFrameBufferStatus(B2R_OBJECT_T* this, UCHAR ucFbgId, UCHAR ucFbId, UCHAR ucFbStatus)
{
    B2R_PRM_T* prFrcPrm;
    if (!this || this->ucB2rId >= B2R_NS)
    {
        return;
    }
    prFrcPrm = this->ptB2rPrm;
    
    if (prFrcPrm->fgB2RNoLock &&
        ((ucFbStatus == FBM_FB_STATUS_LOCK) || 
        (ucFbStatus == FBM_FB_STATUS_EMPTY)))
    {
        //Not do lock
        if(ucFbStatus == FBM_FB_STATUS_LOCK)
        {
            return;
        }
        else if(ucFbStatus == FBM_FB_STATUS_EMPTY)
        {
            if(FBM_CheckFrameBufferStatus(ucFbgId, ucFbId, FBM_FB_STATUS_LOCK))
            {
                FBM_SetFrameBufferStatus(ucFbgId, ucFbId, ucFbStatus);
            }
            else
            {
                LOG(0,"[B2R WARN]FbgId(%d),FbId(%d) not locked!\n",ucFbgId, ucFbId);
            }
        }
        else
        {
            LOG(0,"[B2R WARN]Status Wrong!\n");
        }
    }
    else
    {
        FBM_SetFrameBufferStatus(ucFbgId, ucFbId, ucFbStatus);
    }
}

static void _B2R_HandleResolutionChange(B2R_OBJECT_T *this)//LIJIA
{
    FBM_SEQ_HDR_T* prSeqHdr;
    B2R_VAR_T* prB2rVar;
    B2R_PRM_T *prFrcPrm=NULL;
  
    if (this->ucB2rId >= B2R_NS)
    {
        LOG(0, "prFrcPrm->ucB2rId = %d [%s-%d]\n", prFrcPrm->ucB2rId, __FUNCTION__, __LINE__);
        return;
    }
    prFrcPrm = this->ptB2rPrm;
    if(!prFrcPrm)
    {
        return;
    }
    
    prSeqHdr = FBM_GetFrameBufferSeqHdr(prFrcPrm->ucFbgId);
    prB2rVar = this->ptB2rVar;
    if (prSeqHdr && (prB2rVar->u4LastWidth != prSeqHdr->u2OrgWidth || prB2rVar->u4LastHeight != prSeqHdr->u2OrgHeight))
    {
        prB2rVar->u4LastWidth = prSeqHdr->u2OrgWidth;
        prB2rVar->u4LastHeight = prSeqHdr->u2OrgHeight;
        if (prB2rVar->fgModeChangeSent)
        {
            //_VDP_VsyncNotify(, ucVdpId, 0, 0);
            _B2R_PostNotify(this, VDP_MSG_RES_CHG_CB, 0, 0);
        }
    }
}

//release framebuffer which is locked by B2R
static void _B2R_RelResource(UCHAR ucB2rId)
{
    B2R_PRM_T * prFrcPrm = NULL;
    B2R_OBJECT_T* this;
    if (ucB2rId >= B2R_NS)
    {
        LOG(3,"ucB2rId >= B2R_NS oops\n");
        return ;
    }

    this = _B2R_GetObj(ucB2rId);
    if(!this)
    {
        return;
    }
    
    prFrcPrm = this->ptB2rPrm;
    if (prFrcPrm && prFrcPrm->ucFbgId != FBM_FBG_ID_UNKNOWN)
    {
        if (prFrcPrm->ucReleaseFbId !=FBM_FBG_ID_UNKNOWN)
        {
            if (FBM_ChkFbgCreateFromInst(prFrcPrm->ucFbgId) == FALSE)
            {
                if (FBM_CheckFrameBufferStatus(prFrcPrm->ucFbgId, prFrcPrm->ucReleaseFbId, FBM_FB_STATUS_LOCK))
                {
                    _B2R_SetFrameBufferStatus(this, prFrcPrm->ucFbgId, prFrcPrm->ucReleaseFbId, FBM_FB_STATUS_EMPTY);
                }
            }
            else
            {
                _B2R_SetFrameBufferStatus(this, prFrcPrm->ucFbgId, prFrcPrm->ucReleaseFbId, FBM_FB_STATUS_EMPTY);
            }
            prFrcPrm->ucReleaseFbId = FBM_FB_ID_UNKNOWN;
        }

        if (prFrcPrm->ucFbId !=FBM_FBG_ID_UNKNOWN)
        {
            if (FBM_ChkFbgCreateFromInst(prFrcPrm->ucFbgId) == FALSE)
            {
                if (FBM_CheckFrameBufferStatus(prFrcPrm->ucFbgId, prFrcPrm->ucFbId, FBM_FB_STATUS_LOCK))
                {
                    _B2R_SetFrameBufferStatus(this, prFrcPrm->ucFbgId, prFrcPrm->ucFbId, FBM_FB_STATUS_EMPTY);
                }
            }
            else
            {
                _B2R_SetFrameBufferStatus(this, prFrcPrm->ucFbgId, prFrcPrm->ucFbId, FBM_FB_STATUS_EMPTY);
            }
            prFrcPrm->ucFbId = FBM_FB_ID_UNKNOWN;
        }
    }
}

static void _B2R_GetFrameBufferForBivl3D(B2R_OBJECT_T* this)
{
    B2R_PRM_T* prFrcPrm;
    FBM_SEQ_HDR_T* prSeqHdr;
    do
    {
        if (!this || this->ucB2rId >= B2R_NS)
        {
            break;
        }
        prFrcPrm = this->ptB2rPrm;
        prSeqHdr = FBM_GetFrameBufferSeqHdr(prFrcPrm->ucFbgId);
        if (prSeqHdr)
        {
            UCHAR ucLFbId = FBM_FB_ID_UNKNOWN;
            UCHAR ucRFbId = FBM_FB_ID_UNKNOWN;
            FBM_PIC_HDR_T* prLPicHdr = NULL;
            FBM_PIC_HDR_T* prRPicHdr = NULL;
            INT64 i8LPts, i8RPts;
            BOOL fgLRealSide;
            BOOL fgRRealSide;
            UCHAR ucIndex = prSeqHdr->ucFrmRatCod;
            UCHAR ucFrameRate = _B2R_ConvertFrameRateInx2FrameRate(ucIndex);
            INT32 i4Interval = VDP_STC_CLOCK / ucFrameRate / 2;
            while (TRUE)
            {
                ucLFbId = FBM_PreLookFrameBufferFromDispQ(prFrcPrm->ucFbgId);
                if (ucLFbId == FBM_FB_ID_UNKNOWN)//no frame available, go...
                {
                    prFrcPrm->ucFbId = FBM_FB_ID_UNKNOWN;
                        LOG(3, "BIVL, no frame available, go...\n");
                    break;
                }
                else 
                {
                    ucRFbId = FBM_PreLook2ndFrameBufferFromDispQ(prFrcPrm->ucFbgId);
                    if (ucRFbId == FBM_FB_ID_UNKNOWN)
                    {
                        prFrcPrm->ucFbId = FBM_FB_ID_UNKNOWN;
                            LOG(3, "BIVL, no 2 frames available, go...\n ");
                        break;
                    }
                }

                prLPicHdr = FBM_GetFrameBufferPicHdr(prFrcPrm->ucFbgId, ucLFbId);
                prRPicHdr = FBM_GetFrameBufferPicHdr(prFrcPrm->ucFbgId, ucRFbId);
                
                if (prLPicHdr && prRPicHdr)
                {
                    fgLRealSide = prLPicHdr->fgBivl3DLR;
                    fgRRealSide = prRPicHdr->fgBivl3DLR;
                    if (fgLRealSide && !fgRRealSide)//L&R
                    {
                        i8LPts = prLPicHdr->u4PTS;
                        i8RPts = prRPicHdr->u4PTS;
                        if (i8RPts - i8LPts - i4Interval <= 500 && i8RPts - i8LPts - i4Interval >= -500)//right case, and go...
                        {
                            LOG(3, "BIVL, right case, L = %d, R = %d, go...\n", ucLFbId, ucRFbId);
                            prFrcPrm->ucFbId = FBM_GetFrameBufferFromDispQ(prFrcPrm->ucFbgId);
                            FBM_SetFrameBufferStatus(prFrcPrm->ucFbgId, prFrcPrm->ucFbId, FBM_FB_STATUS_LOCK);
#ifdef CC_3D_MM_DS_SUPPORT
                            prFrcPrm->ucSubFbId = FBM_GetFrameBufferFromDispQ(prFrcPrm->ucFbgId);
                            FBM_SetFrameBufferStatus(prFrcPrm->ucFbgId, prFrcPrm->ucSubFbId, FBM_FB_STATUS_LOCK);
#endif
                            break;
                        }
                        else 
                        {
                            LOG(3, "BIVL, the difference between L and R's PTS is not legal, and go on to fetch L and R\n");
                            ucLFbId = FBM_GetFrameBufferFromDispQ(prFrcPrm->ucFbgId);
                            FBM_SetFrameBufferStatus(prFrcPrm->ucFbgId, ucLFbId, FBM_FB_STATUS_LOCK);
                            FBM_SetFrameBufferStatus(prFrcPrm->ucFbgId, ucLFbId, FBM_FB_STATUS_EMPTY);
                            ucRFbId = FBM_GetFrameBufferFromDispQ(prFrcPrm->ucFbgId);
                            FBM_SetFrameBufferStatus(prFrcPrm->ucFbgId, ucRFbId, FBM_FB_STATUS_LOCK);
                            FBM_SetFrameBufferStatus(prFrcPrm->ucFbgId, ucRFbId, FBM_FB_STATUS_EMPTY);
                            ucLFbId = ucRFbId = FBM_FB_ID_UNKNOWN;
                        }
                    }
                    else if (!fgLRealSide && !fgRRealSide)//R&R
                    {
                        LOG(3, "BIVL, wrong case, drop both frames, and go on to fetch L and R\n");
                        ucLFbId = FBM_GetFrameBufferFromDispQ(prFrcPrm->ucFbgId);
                        FBM_SetFrameBufferStatus(prFrcPrm->ucFbgId, ucLFbId, FBM_FB_STATUS_LOCK);
                        FBM_SetFrameBufferStatus(prFrcPrm->ucFbgId, ucLFbId, FBM_FB_STATUS_EMPTY);
                        ucRFbId = FBM_GetFrameBufferFromDispQ(prFrcPrm->ucFbgId);
                        FBM_SetFrameBufferStatus(prFrcPrm->ucFbgId, ucRFbId, FBM_FB_STATUS_LOCK);
                        FBM_SetFrameBufferStatus(prFrcPrm->ucFbgId, ucRFbId, FBM_FB_STATUS_EMPTY);
                        ucLFbId = ucRFbId = FBM_FB_ID_UNKNOWN;
                    }
                    else//R&L, L&L
                    {
                        LOG(3, "BIVL, wrong case, drop left frame, and go on to fetch L and R\n");
                        ucLFbId = FBM_GetFrameBufferFromDispQ(prFrcPrm->ucFbgId);
                        FBM_SetFrameBufferStatus(prFrcPrm->ucFbgId, ucLFbId, FBM_FB_STATUS_LOCK);
                        FBM_SetFrameBufferStatus(prFrcPrm->ucFbgId, ucLFbId, FBM_FB_STATUS_EMPTY);
                        ucLFbId = FBM_FB_ID_UNKNOWN;
                    }
                }
                else
                {
                    ASSERT(prLPicHdr && prRPicHdr);
                }
            }
        }
        else
        {
            prFrcPrm->ucFbId = FBM_FB_ID_UNKNOWN;
        }
    }while(FALSE);
}

static UCHAR _B2R_ConvertFrameRateInx2FrameRate(UCHAR ucIndex)
{
    UCHAR ucRet;
    
    switch (ucIndex)
    {
    case MPEG_FRAME_RATE_12:
        ucRet = FBM_FRAME_RATE_12;
        break;

    case MPEG_FRAME_RATE_15:
        ucRet = FBM_FRAME_RATE_15;
        break;
        
#ifdef SUPPORT_DISP_WEB_VIDEO_SMOOTHER
    case MPEG_FRAME_RATE_20:
        ucRet = FBM_FRAME_RATE_20;
        break;
#endif
    case MPEG_FRAME_RATE_24_:
    case MPEG_FRAME_RATE_24:
        ucRet = FBM_FRAME_RATE_24;
        break;

    case MPEG_FRAME_RATE_25:
        ucRet = FBM_FRAME_RATE_25;
        break;

    case MPEG_FRAME_RATE_30_:
    case MPEG_FRAME_RATE_30:
        ucRet = FBM_FRAME_RATE_30;
        break;

    case MPEG_FRAME_RATE_50:
        ucRet = FBM_FRAME_RATE_50;
        break;

    case MPEG_FRAME_RATE_60_:
    case MPEG_FRAME_RATE_60:
        ucRet = FBM_FRAME_RATE_60;
        break;

    case MPEG_FRAME_RATE_UNKNOWN:
    default:
        LOG(1, "Unknown Frame Rate %d\n", ucIndex);
        ucRet = FBM_FRAME_RATE_30;
        break;
    }
    return ucRet;
}

#ifdef ENABLE_MULTIMEDIA
/*-----------------------------------------------------------------------------
* Name: _VDP_RTFrameRateConvert
*
* Description: This API Determine Enable/Disable Double Clock from Frame Rate.
*              
* Inputs:  prFrcPrm    Specifies the struct of the B2R PRM. Different B2R PRM 
*                       refer to different B2R Id.
*          
*             ucIndex  Specifies the Frame rate code
*
* Outputs: 
*
* Function: For WiFi Display , if input frame rate less or equal than 30fps , Enable Double Clock, Reduce Target NS for A/V sync
*              Otherwise Disable Double Clock and Keep Drop Frame When A/V sync
----------------------------------------------------------------------------*/
static UCHAR _B2R_RTFrameRateConvert(B2R_PRM_T* prFrcPrm, UCHAR ucIndex)
{
	switch (ucIndex)
    {
    case MPEG_FRAME_RATE_12:
        prFrcPrm->ucInFrameRate = FBM_FRAME_RATE_24;
        prFrcPrm->t_wfd_prm.fgDblClk = TRUE;
        break;

    case MPEG_FRAME_RATE_15:
        prFrcPrm->ucInFrameRate = FBM_FRAME_RATE_30;
        prFrcPrm->t_wfd_prm.fgDblClk = TRUE;
        break;

    case MPEG_FRAME_RATE_20:
        prFrcPrm->ucInFrameRate = FBM_FRAME_RATE_20;
        break;
        
    case MPEG_FRAME_RATE_24_:
    case MPEG_FRAME_RATE_24:
        prFrcPrm->ucInFrameRate = FBM_FRAME_RATE_24;
        break;

    case MPEG_FRAME_RATE_25:
        prFrcPrm->ucInFrameRate = FBM_FRAME_RATE_50;
        prFrcPrm->t_wfd_prm.fgDblClk = TRUE;
        break;

    case MPEG_FRAME_RATE_30_:
    case MPEG_FRAME_RATE_30:
        prFrcPrm->ucInFrameRate = FBM_FRAME_RATE_30;
        //prFrcPrm->t_wfd_prm.fgDblClk = TRUE;
        break;
        
    case MPEG_FRAME_RATE_50:
        prFrcPrm->ucInFrameRate = FBM_FRAME_RATE_50;
        break;

    case MPEG_FRAME_RATE_60_:
    case MPEG_FRAME_RATE_60:
        prFrcPrm->ucInFrameRate = FBM_FRAME_RATE_60;
        break;

    case MPEG_FRAME_RATE_UNKNOWN:
    default:
        LOG(1, "wfd not support Frame Rate %d , fgDblClk : %d\n", 
            ucIndex,prFrcPrm->t_wfd_prm.fgDblClk);
        prFrcPrm->ucInFrameRate = FBM_FRAME_RATE_60;
        break;
    }
	return prFrcPrm->ucInFrameRate;
}
#endif

static BOOL _B2R_IsBivl3D(B2R_OBJECT_T* this)
{
    BOOL fgRet = FALSE;
    B2R_PRM_T* prFrcPrm;
    FBM_SEQ_HDR_T* prSeqHdr;
    do
    {
        if (!this || this->ucB2rId >= B2R_NS)
        {
            break;
        }
        prFrcPrm = this->ptB2rPrm;
#ifdef CC_3D_MM_DS_SUPPORT
        prSeqHdr = FBM_GetFrameBufferSeqHdr(prFrcPrm->ucFbgId);
        if (prSeqHdr && prSeqHdr->u1B2R3DType == B2R_3D_FRAME_SEQUENTIAL && 
            (prFrcPrm->ucSubFbgId == FBM_FBG_ID_UNKNOWN || prFrcPrm->ucFbgId == prFrcPrm->ucSubFbgId))
        {
            fgRet = TRUE;
        }
#endif
    }while(FALSE);
    
    return fgRet;
}

static void _B2R_CalcRepeatCntForPushTrickMode(B2R_PRM_T* prFrcPrm, BOOL fgRepeatFbForNofb)
{
    UINT32 u4FirstPts;
    UINT32 u4LastPts;
    UINT32 u4Speed;
    FBM_PIC_HDR_T* prFirstHdr = NULL;
    FBM_PIC_HDR_T* prLastHdr = NULL;
    B2R_VAR_T* prB2rVar;
    if (prFrcPrm->ucB2rId >= B2R_NS)
    {
        LOG(0, "prFrcPrm->ucB2rId = %d [%s-%d]\n", prFrcPrm->ucB2rId, __FUNCTION__, __LINE__);
        return;
    }
    prB2rVar = &_arB2rVar[prFrcPrm->ucB2rId];
    prFirstHdr = FBM_GetFrameBufferPicHdr(prFrcPrm->ucFbgId, prFrcPrm->ucLastFbId);
    prLastHdr = FBM_GetFrameBufferPicHdr(prFrcPrm->ucFbgId, prFrcPrm->ucFbId);
    if (prFirstHdr && prFirstHdr->fgPushModeTrick && prLastHdr &&
        ((!(prB2rVar->u4RepeatCntForPushTrickMode) && !(prB2rVar->u4ElapseCntForPushTrickMode)) || 
        ((prB2rVar->u4RepeatCntForPushTrickMode) && prB2rVar->u4ElapseCntForPushTrickMode >= prB2rVar->u4RepeatCntForPushTrickMode)))
    {
        prB2rVar->ucRepeatFbForPushTrickMode = prFrcPrm->ucLastFbId;
        u4FirstPts = prFirstHdr->u4PTS;
        u4LastPts = prLastHdr->u4PTS;
        u4Speed = prB2rVar->eSpeed;
        if (u4LastPts > u4FirstPts && u4Speed >= STC_SPEED_TYPE_FORWARD_4X && u4Speed <= STC_SPEED_TYPE_FORWARD_1024X && prFrcPrm->ucInFrameRate != FBM_FRAME_RATE_UNKNOWN)
        {
            prB2rVar->u4RepeatCntForPushTrickMode = (u4LastPts - u4FirstPts) / (2 << (u4Speed - STC_SPEED_TYPE_FORWARD_4X + 2)) / (VDP_STC_CLOCK / prFrcPrm->ucInFrameRate)  + 1;
            prB2rVar->u4ElapseCntForPushTrickMode = 0;
        }
        else 
        {
            prB2rVar->u4RepeatCntForPushTrickMode = 1;
            prB2rVar->u4ElapseCntForPushTrickMode = 0;
        }
    }
    if (prFirstHdr && prFirstHdr->fgPushModeTrick)
    {
        if (fgRepeatFbForNofb)
        {
            prB2rVar->ucRepeatFbForPushTrickMode = prFrcPrm->ucFbId;
        }
    }
}

void _B2R_GetFrameBufferFromPending(B2R_OBJECT_T* this)
{
    B2R_PRM_T* prFrcPrm;
    do
    {
        if (!this)
        {
            break;
        }
        if (this->ucB2rId >= B2R_NS)
        {
            break;
        }
        prFrcPrm = this->ptB2rPrm;
        prFrcPrm->ucFbId = prFrcPrm->ucPendingFbId;
        prFrcPrm->ucPendingFbId = FBM_FB_ID_UNKNOWN;

#ifdef CC_3D_MM_DS_SUPPORT
        if (prFrcPrm->ucSubPendingFbId != FBM_FB_ID_UNKNOWN)
        {
            prFrcPrm->ucSubFbId = prFrcPrm->ucSubPendingFbId;
            prFrcPrm->ucSubPendingFbId = FBM_FB_ID_UNKNOWN;
        }
#endif
    }while(FALSE);
}

static UINT32 _B2R_DetermineRepeatForPushTrickMode(B2R_OBJECT_T* this, BOOL fgRepeatFbForNoFb)
{
    B2R_VAR_T* prB2rVar;
    B2R_PRM_T * prFrcPrm;
    UCHAR ucB2rId;
    
    if (!this)
    {
        return B2R_INV_ARG;
    }

    ucB2rId  = this->ucB2rId;
    prB2rVar = this->ptB2rVar;
    prFrcPrm = this->ptB2rPrm;

    if (prB2rVar->u4RepeatCntForPushTrickMode > 0)
    {
        (prB2rVar->u4ElapseCntForPushTrickMode)++;
        LOG(3, "go on to repeat, %d(%d, %d)\n", prB2rVar->ucRepeatFbForPushTrickMode, prB2rVar->u4ElapseCntForPushTrickMode, prB2rVar->u4RepeatCntForPushTrickMode);
        if (prB2rVar->u4ElapseCntForPushTrickMode < prB2rVar->u4RepeatCntForPushTrickMode)
        {
            _B2R_PreparePendingForNextVsync(this);
        }
        else 
        {
            prB2rVar->u4ElapseCntForPushTrickMode = prB2rVar->u4RepeatCntForPushTrickMode = 0;
        }
    }
    else 
    {
        FBM_PIC_HDR_T* prPicHdr = FBM_GetFrameBufferPicHdr(prFrcPrm->ucFbgId, prB2rVar->ucRepeatFbForPushTrickMode);
        if (prPicHdr && prPicHdr->fgPushModeTrick && fgRepeatFbForNoFb)
        {
            (prB2rVar->u4ElapseCntForPushTrickMode)++;
            LOG(3, "the second frame is unavailable, and go on to repeat, %d(%d, ?)\n", prB2rVar->ucRepeatFbForPushTrickMode, prB2rVar->u4ElapseCntForPushTrickMode);
        }
        else
        {
            //normal case
        }
    }

    return B2R_OK;
    
}
#ifdef VDP_B2R_PSCAN_CRC_FLOW
/* B2R CRC check on PTS value */
void _VDPSetChkPTS(UINT32 u4Pts)
{
    _u4SetPts = u4Pts;
    LOG(1, "Set PTS (%d)\n", _u4SetPts);
}

void _VDP_SetEncTrigger(BOOL fgEnable)
{
    _fgTriggerEnc = fgEnable;
    UNUSED(_fgTriggerEnc);
}
#endif

//-----------------------------------------------------------------------------
/** Brief of _B2R_PreLookNextDisp.
 */
//-----------------------------------------------------------------------------
static UCHAR _B2R_PreLookNextDisp(B2R_PRM_T* prFrcPrm)
{
    return FBM_PreLookFrameBufferFromDispQ(prFrcPrm->ucFbgId);
}

//-----------------------------------------------------------------------------
/** Brief of _VDP_SetXYOffset.
 */
//-----------------------------------------------------------------------------
void _VDP_SetXYOffset(UCHAR ucB2rId)
{
    B2R_OBJECT_T *this = NULL;
    B2R_PRM_T *prFrcPrm = NULL;
    B2R_HAL_DISP_INFO_T tDispInfo;

    this = _B2R_GetObj(ucB2rId);
    if(!this)
    {
        return;
    }
    
    prFrcPrm = &_arVdpPrm[ucB2rId];
    ASSERT(prFrcPrm->rRenderRegion.u4X == ((prFrcPrm->rRenderRegion.u4X + 0xF) & ~0xF)); // H_Offset must be 16-pixer unit
    
    B2R_HAL_Get(this->hB2r, B2R_HAL_DISP_INFO, &tDispInfo);
    
    if (tDispInfo.eAddrMode == B2R_AM_RASTER)
    {
        UINT32 u4AddrOffset = 0;
        UINT32 u4FBAddrY = 0, u4FBAddrC = 0;
        B2R_HAL_ADDR_T tFrmAddr = {0};
        
        u4AddrOffset = prFrcPrm->rRenderRegion.u4Y * tDispInfo.u4Pitch +
                       prFrcPrm->rRenderRegion.u4X;
        
        FBM_GetFrameBufferAddr(prFrcPrm->ucFbgId, prFrcPrm->ucFbId, &u4FBAddrY, &u4FBAddrC);
        if (u4FBAddrY == 0 || u4FBAddrC == 0)
        {
            LOG(0, "FBM_GetFrameBufferAddr fail, ucFbgId %d, ucFbId %d\n", prFrcPrm->ucFbgId, prFrcPrm->ucFbId);
            return;
        }
        
        tFrmAddr.u4Y0 = u4FBAddrY + u4AddrOffset;
        tFrmAddr.u4C0 = u4FBAddrC + u4AddrOffset;
        
        B2R_HAL_Set(this->hB2r, B2R_HAL_ADDR, &tFrmAddr);
        
        LOG(3, "Raster Set XY shift XY(%d, %d), Orig Addr(%x, %x), New Addr(%x, %x)\n",
            prFrcPrm->rRenderRegion.u4X, prFrcPrm->rRenderRegion.u4Y,
            u4FBAddrY, u4FBAddrC, tFrmAddr.u4Y0, tFrmAddr.u4C0);
    }
    else
    {
        LOG(0, "Non-Raster(%d) do not support XY shift Mode!\n", tDispInfo.eAddrMode);
    }
}

//-----------------------------------------------------------------------------
/** Brief of _VDP_HalSetRenderRegion
 ** For JPEG raster mode to set the render region.
 ** Such JPEG decode a picture with 9160 * 9160, but only show (3000, 3000, 1920, 1080).
 ** Only change X,Y can be change the Y/C start Addr. 
 ** Change W/H will mode change with black, It is not implement yet.
 */
//-----------------------------------------------------------------------------
UINT32 _VDP_HalSetRenderRegion(UCHAR ucB2rId, VDP_REGION_T* prRenderRegion)
{
    //static VDP_REGION_T rRenderRegion = {0, 0, 0, 0};
    //static BOOL fgRenderRegionChg = FALSE;
    B2R_PRM_T* prFrcPrm;

    if ((ucB2rId >= B2R_NS) || !prRenderRegion)
    {
        return 0;
    }
    prFrcPrm = &_arVdpPrm[ucB2rId];

    if ((prFrcPrm->rRenderRegion.u4Width && (prRenderRegion->u4Width != prFrcPrm->rRenderRegion.u4Width)) ||
            (prFrcPrm->rRenderRegion.u4Height && (prRenderRegion->u4Height != prFrcPrm->rRenderRegion.u4Height)))
    {
        LOG(0, "Render W/H not support changed. W/H(%d, %d)->(%d, %d)!\n ", 
                prFrcPrm->rRenderRegion.u4Width, prFrcPrm->rRenderRegion.u4Height, 
                prRenderRegion->u4Width, prRenderRegion->u4Height);
        ASSERT(0);
        return 0;
    }

    if (prFrcPrm->fgRenderRegionChg)
    {
        LOG(1, "Change too fast: the last X,Y change not process yet!\n ");
        return 0;
    }

    if ((prRenderRegion->u4X == prFrcPrm->rRenderRegion.u4X) &&
            (prRenderRegion->u4Y == prFrcPrm->rRenderRegion.u4Y))
    {
        LOG(1, "RenderXY not change: X,Y(%d, %d) -> (%d, %d)!\n ", 
                prFrcPrm->rRenderRegion.u4X, prFrcPrm->rRenderRegion.u4Y, 
                prRenderRegion->u4X, prRenderRegion->u4Y);
        return 0;
    }

    LOG(3, "RenderXY changed: X,Y(%d, %d) -> (%d, %d)!\n ", 
            prFrcPrm->rRenderRegion.u4X, prFrcPrm->rRenderRegion.u4Y, 
            prRenderRegion->u4X, prRenderRegion->u4Y);

    prFrcPrm->fgRenderRegionChg = TRUE;
    prFrcPrm->rRenderRegion.u4X = ((UINT32)prRenderRegion->u4X + 0xF) & (~0xF);
    prFrcPrm->rRenderRegion.u4Y = prRenderRegion->u4Y;

    return 1;
}

#ifdef CC_B2R_ENABLE_CHG_FRAME_MSG
EXTERN VOID i4VDOOmxRenderFrame(UINT8 ucType);

static BOOL _NptvSeamlessCheckChgFrameMsg(UCHAR ucB2rId,B2R_OBJECT_T* this)
{
   
    UINT16 u2MsgQNum;
	VDEC_ES_INFO_T* prVdecEsInfo = NULL;
	UCHAR ucEsId;
	VDP_CFG_T* prVdpConf;

     x_msg_q_num_msgs(_ahChgFrameQueue[ucB2rId],&u2MsgQNum);

     prVdpConf = _B2R_GetDP(this);
     
     ucEsId = prVdpConf->ucInputPort[0];
     if(FBM_GetFbgAppMode(this->ptB2rPrm->ucFbgId) == FBM_FBG_APP_MTIMAGE)
     {
        return TRUE;
     }
     
     prVdecEsInfo = (ucEsId < VDEC_MAX_ES) ? _VDEC_GetEsInfo(ucEsId) : NULL;
	 if(!prVdecEsInfo)
	 {
	     return FALSE;
	 }
	 if(prVdecEsInfo->eSeamlessMode & SEAMLESS_BY_NPTV)
	 {
	     if(u2MsgQNum>=KEEP_FB_NUM)
	     {
	         return TRUE;
	     }
		 else
		 {
		     return FALSE;
		 }
	 }


    return TRUE;
}


static INT32 _B2R_ReceiveChgFrameMsg(UCHAR ucB2rId,
                                     VOID            *pv_msg,
                                     MSGQ_OPTION_T   e_option)
{
    INT32 iRet;
    UINT16 u2MsgQIdx;
    SIZE_T zMsgSize;

    if ((ucB2rId >= B2R_NS) || !pv_msg)
    {
        return OSR_INV_ARG;
    }

    zMsgSize = sizeof(VDP_B2R_CHG_FRAME_MSG_T);

    VERIFY(x_sema_lock(_ahChgFrameMutex[ucB2rId], X_SEMA_OPTION_WAIT) == OSR_OK);
    iRet = x_msg_q_receive(&u2MsgQIdx, pv_msg, &zMsgSize, &(_ahChgFrameQueue[ucB2rId]), 1, e_option);
    if(iRet == OSR_OK)
    {
        i4VDOOmxRenderFrame(2);
    }
    VERIFY(x_sema_unlock(_ahChgFrameMutex[ucB2rId]) == OSR_OK);
    return iRet;
}


BOOL _B2R_SendB2RAysncRenderFrameMsg(VDP_B2R_CHG_FRAME_MSG_T* prMsg)
{
    VDP_B2R_CHG_FRAME_MSG_T rMsg;
    B2R_OBJECT_T* this;
    UCHAR ucB2rId;
    UINT16 u2MsgQNum;

    x_memset(&rMsg,0,sizeof(VDP_B2R_CHG_FRAME_MSG_T));
    rMsg.u4DispMode = VDP_B2R_CHG_FRAME_DISPQ;
    rMsg.ucFbgId= prMsg->ucFbgId;
    rMsg.ucFbId = prMsg->ucFbId;
    ucB2rId=_FBM_Fbg2B2r(prMsg->ucFbgId);
    if(ucB2rId >= B2R_NS)
    {
        LOG(0,"_B2R_SendB2RRenderFrameMsg fbg %d not match a b2r id\n",prMsg->ucFbgId);
        return FALSE;
    }

    this = _B2R_GetObj(ucB2rId);
    if(!this)
    {
        LOG(0,"_B2R_SendB2RRenderFrameMsg fbg %d , b2r %d no this \n",prMsg->ucFbgId,ucB2rId);
        return FALSE;
    }
        
    x_msg_q_num_msgs(_ahChgFrameQueue[ucB2rId],&u2MsgQNum);
    LOG(5, "B2R(%d) rMsg.u4DispMode(%d) rMsg.ucFbgId =%d,rMsg.ucFbId=%d,flag=%d,u2MsgQNum=%d\n",
            ucB2rId, rMsg.u4DispMode, rMsg.ucFbgId, rMsg.ucFbId,(rMsg.u4Flag & VDP_B2R_CHG_FRAME_MSG_SYNC),u2MsgQNum);    
    
    if(FBM_CheckFrameBufferStatus(rMsg.ucFbgId, rMsg.ucFbId, FBM_FB_STATUS_EMPTY))
    {   
        LOG(1,"_B2R_SendB2RRenderFrameMsg, FbgId=%d,FbId=%d is empty \n",rMsg.ucFbgId,rMsg.ucFbId);
        return FALSE;
    }
    
    if (x_msg_q_send(_ahChgFrameQueue[ucB2rId], (void *)(&rMsg), sizeof(VDP_B2R_CHG_FRAME_MSG_T), 0) != OSR_OK)
    {
        LOG(1,"Msg Queue full for change frame buffer Msg queue\n");
        return FALSE;
    }
    
    return TRUE;
}

BOOL _B2R_SendB2RChgFrameMsg(VDP_B2R_CHG_FRAME_MSG_T* prMsg)
{
    VDP_B2R_CHG_FRAME_MSG_T rMsg;
    B2R_PRM_T* prVdpPrm = NULL;
    B2R_VAR_T* prB2rVar = NULL;
    VDP_CFG_T* prVdpConf = NULL;
    B2R_OBJECT_T* this;
    UCHAR ucB2rId;
    UINT16 u2MsgQNum;
    x_memset(&rMsg,0,sizeof(VDP_B2R_CHG_FRAME_MSG_T));

    rMsg.u4VdpId = prMsg->u4VdpId;
    rMsg.u4Flag = prMsg->u4Flag;

    rMsg.u4DispMode = prMsg->u4DispMode;
    rMsg.ucFbgId= prMsg->ucFbgId;
    rMsg.ucFbId = prMsg->ucFbId;

    if (rMsg.u4VdpId >= VDP_NS)
    {
        LOG(1,"u4VdpId >= VDP_NS oops\n");
        return FALSE;
    }

    prVdpConf = _B2R_GetVdpConf(rMsg.u4VdpId);
    if(!prVdpConf ||
        prVdpConf->ucB2rId >= B2R_NS)
    {
        return FALSE;
    }
    
    ucB2rId = prVdpConf->ucB2rId;
    this = _B2R_GetObj(ucB2rId);
    if(!this)
    {
        return FALSE;
    }
    
    prVdpPrm = this->ptB2rPrm;
    prB2rVar = this->ptB2rVar;
    
    if (rMsg.u4DispMode == VDP_B2R_CHG_FRAME_DIRECT && (prVdpPrm->ucFbgId != rMsg.ucFbgId))
    {
        LOG(1, "B2R(%d) render FB(%d-%d) mismatch B2R_Fbg(%d).\n",
            ucB2rId, rMsg.ucFbgId, rMsg.ucFbId, prVdpPrm->ucFbgId);
        return FALSE;
    }
    x_msg_q_num_msgs(_ahChgFrameQueue[ucB2rId],&u2MsgQNum);
    LOG(5, "B2R(%d) rMsg.u4DispMode(%d) rMsg.ucFbgId =%d,rMsg.ucFbId=%d,flag=%d,u2MsgQNum=%d\n",
            ucB2rId, rMsg.u4DispMode, rMsg.ucFbgId, rMsg.ucFbId,(rMsg.u4Flag & VDP_B2R_CHG_FRAME_MSG_SYNC),u2MsgQNum);

    if (rMsg.u4Flag & VDP_B2R_CHG_FRAME_MSG_SYNC)
    {
        // Pop out all display message
        VDP_B2R_CHG_FRAME_MSG_T rPopOutMsg;
        UCHAR ucFbId = FBM_FB_ID_UNKNOWN;

        if (prVdpPrm && (prVdpPrm->ucFbgId != FBM_FBG_ID_UNKNOWN))
        {
            while (_B2R_ReceiveChgFrameMsg(ucB2rId, &rPopOutMsg, X_MSGQ_OPTION_NOWAIT) == OSR_OK)
            {

#ifndef CC_DTV_SUPPORT_LG
                //UINT16 u2MsgQNum;
                
                if(x_msg_q_num_msgs(_ahChgFrameQueue[ucB2rId],&u2MsgQNum))
                {
                    if(u2MsgQNum <= KEEP_FB_NUM)
                    {
                        break;
                    }
                }
#endif
                
                if (rPopOutMsg.u4DispMode == VDP_B2R_CHG_FRAME_DIRECT)
                {
                    if (rPopOutMsg.ucFbId != rMsg.ucFbId)
                    {
                        LOG_REL(prB2rVar->fgEnableGstLog, 1, 3, "sync disp: overwrite display frame : FB(%d-%d).\n", 
                                rPopOutMsg.ucFbgId, rPopOutMsg.ucFbId);
                        _B2R_SetFrameBufferStatus(this, rPopOutMsg.ucFbgId, rPopOutMsg.ucFbId, FBM_FB_STATUS_EMPTY);
                    }
                    continue;
                }

                ucFbId = FBM_GetFrameBufferFromDispQ(prVdpPrm->ucFbgId);
                
                LOG_REL(prB2rVar->fgEnableGstLog, 1, 3, "sync disp: drop old frames Fb(%d-%d)\n", prVdpPrm->ucFbgId, ucFbId);

                if (ucFbId != FBM_FB_ID_UNKNOWN)
                {
                    _B2R_SetFrameBufferStatus(this, prVdpPrm->ucFbgId, ucFbId, FBM_FB_STATUS_LOCK);
                    _B2R_SetFrameBufferStatus(this, prVdpPrm->ucFbgId, ucFbId, FBM_FB_STATUS_EMPTY);
                }
            }
        }
    }

#ifndef CC_DTV_SUPPORT_LG
    if(FBM_CheckFrameBufferStatus(rMsg.ucFbgId, rMsg.ucFbId, FBM_FB_STATUS_EMPTY))
    {
        return FALSE;
    }
#endif

    if (x_msg_q_send(_ahChgFrameQueue[ucB2rId], (void *)(&rMsg), sizeof(VDP_B2R_CHG_FRAME_MSG_T), 0) != OSR_OK)
    {
        LOG(1,"Msg Queue full for change frame buffer Msg queue\n");
        return FALSE;
    }
    
    #ifndef CC_DTV_SUPPORT_LG
    if (rMsg.u4DispMode == VDP_B2R_CHG_FRAME_DIRECT)
    {
        //FBM_TriggerSmlsByResizer(rMsg.ucFbgId, rMsg.ucFbId);
    }
    #endif

    //_VdpFrcFrameBufferReady((_arVdpPrm[0]).ucFbgId);

    return TRUE;
}

BOOL _B2R_FlushB2RChgFrameMsg(UCHAR ucB2rId)
{
    //UINT16 u2MsgQIdx;
    //SIZE_T zMsgSize;
    UINT16 u2Cnt;
    VDP_B2R_CHG_FRAME_MSG_T rMsg;
    B2R_VAR_T* prB2rVar; 
    B2R_OBJECT_T* this;
    //B2R_PRM_T* prVdpPrm = NULL;
    
    if (ucB2rId >= B2R_NS)
    {
        LOG(0,"_B2R_FlushB2RChgFrameMsg ucB2rId %d >= B2R_NS oops\n", ucB2rId);
        return FALSE;
    }
    this = _B2R_GetObj(ucB2rId);
    if(!this)
    {    
        LOG(0,"_B2R_FlushB2RChgFrameMsg ucB2rId %d no this\n", ucB2rId);
        return FALSE;
    }
    
    //prVdpPrm = this->ptB2rPrm;
     
    prB2rVar = &_arB2rVar[ucB2rId];
    if (_ahChgFrameQueue[ucB2rId] != (HANDLE_T)(NULL) /*&& prVdpPrm*/)
    {
        VERIFY(x_msg_q_num_msgs(_ahChgFrameQueue[ucB2rId], &u2Cnt) == OSR_OK);
        LOG(1,"_B2R_FlushB2RChgFrameMsg (%d) cnt=%d\n",ucB2rId,u2Cnt);
        while (_B2R_ReceiveChgFrameMsg(ucB2rId, &rMsg, X_MSGQ_OPTION_NOWAIT) == OSR_OK)
        {
            if (rMsg.u4DispMode == VDP_B2R_CHG_FRAME_DIRECT)
            {
                _B2R_SetFrameBufferStatus(this, rMsg.ucFbgId, rMsg.ucFbId, FBM_FB_STATUS_EMPTY);
            }
        }
        VERIFY(x_msg_q_num_msgs(_ahChgFrameQueue[ucB2rId], &u2Cnt) == OSR_OK);
        ASSERT(u2Cnt== 0);

        prB2rVar->fgPendingChgFrmMsg = FALSE;
        x_memset(&( prB2rVar->rPendingChgFrmMsg), 0, sizeof(VDP_B2R_CHG_FRAME_MSG_T));
    }
    return TRUE;
}

UINT32 _B2rUnRenderFrameCnt(UCHAR ucVdpId)
{
    VDP_CFG_T* prVdpConf = NULL;
    UINT16 u2MsgQNum = 0;
    prVdpConf = _B2R_GetVdpConf(ucVdpId);
    if(!prVdpConf || prVdpConf->ucB2rId >= B2R_NS)
    {
       LOG(0,"_B2rWaitRenderFrameCnt error\n");
       return 0;
    }
    
    x_msg_q_num_msgs(_ahChgFrameQueue[prVdpConf->ucB2rId],&u2MsgQNum);
    return u2MsgQNum;
}

BOOL  VDP_SeamlessSeqChanging(UCHAR ucVdpId)
{
    VDP_CFG_T* prVdpConf = NULL;
    UCHAR ucFbgId,ucPlayMde=0;
    prVdpConf = _B2R_GetVdpConf(ucVdpId);
    if(!prVdpConf || prVdpConf->ucB2rId >= B2R_NS)
    {
       LOG(3,"_NptvSeamlessDuraingSeqChange return FALSE 1\n");
       return FALSE;
    }
    
    ucFbgId = _B2R_GetFbg(prVdpConf->ucB2rId);
    
    FBM_GetPlayMode(ucFbgId,&ucPlayMde);
    if(ucFbgId != FBM_FBG_ID_UNKNOWN &&  ucPlayMde == FBM_FBG_MM_MODE)
    {
        return FBM_DoSeqChanging(ucFbgId,FALSE, TRUE);
    }
    
    return FALSE;
}

#endif



static UINT32 _B2R_GetInput(UCHAR ucB2rId, UCHAR ucPort, UCHAR* pucFbgId)
{
    UCHAR ucIdx;
    UCHAR ucCnt = 0;
    UCHAR aucVdpIds[VDP_NS];
    UCHAR ucVdpId;
    
    if (ucPort >= VDP_MAX_INPORT_NS || !pucFbgId ||
        ucB2rId >= B2R_NS)
    {
        return VDP_SET_ERROR;
    }
    
    ucCnt = _B2R_GetVdpIds(ucB2rId, aucVdpIds);
    for (ucIdx = 0; ucIdx < ucCnt; ucIdx++)
    {
        VDP_CFG_T* prVdpConf;
        ucVdpId = aucVdpIds[ucIdx];
        prVdpConf = _B2R_GetVdpConf(ucVdpId);
        if(!prVdpConf)
        {
            continue;
        }
        
        if (prVdpConf->ucInputPort[ucPort] < MAX_ES_NS)
        {
            *pucFbgId = FBM_GetFbgByEs(prVdpConf->ucInputPort[ucPort]);
            return VDP_SET_OK;
        }
        else
        {
            *pucFbgId = FBM_FBG_ID_UNKNOWN;
        }
    }
    return VDP_SET_OK;
}

#ifdef CC_B2R_RES_SUPPORT

void _B2R_HandleScartOut(UCHAR ucVdpId,
                                UCHAR ucEsId, 
                                UCHAR ucPort)
{

    VDP_CFG_T *ptVdpCfg     = NULL;
    VDP_CFG_T *ptOpVdpCfg   = NULL;

    ptVdpCfg    = _B2R_GetVdpConf(ucVdpId);
    ptOpVdpCfg  = _B2R_GetVdpConf(VDP_NS -(1 + ucVdpId));

    if(!ptVdpCfg || !ptOpVdpCfg)
    {
        LOG(0,"UcVDPid is invalid(%d)\n",ucVdpId);
        return;
    }
    
    // if esid is the same scart case: handle first switch then set input case
    LOG(1,"Set input:vdpid(%d) esid = %d and opesid = %d\n",ucVdpId,ucEsId,ptOpVdpCfg->ucInputPort[ucPort]);
    if(ucEsId == ptOpVdpCfg->ucInputPort[ucPort]
       && ucEsId != 0XFF)
    {
        LOG(1,"Set input:vdpid(%d) b2rid = %d and opb2rid = %d\n",ucVdpId,ptVdpCfg->ucB2rId,ptOpVdpCfg->ucB2rId);
        if(ptVdpCfg->ucB2rId != ptOpVdpCfg->ucB2rId)
        {
            LOG(1,"Set input:switch new b2rid:%d\n",ptOpVdpCfg->ucB2rId);
            VDP_B2rSwitch(ucVdpId,B2R_NS);
            VDP_B2rSwitch(ucVdpId,ptOpVdpCfg->ucB2rId);
        }
    }

    return;

}

UCHAR _B2R_ChkHwConflict(UCHAR ucVdpId,
                               UCHAR ucB2rId)
{

    UCHAR  ucFbgId          = FBM_FBG_ID_UNKNOWN;
    UCHAR  ucOpFbgId        = FBM_FBG_ID_UNKNOWN;
    UCHAR  ucRetId          = B2R_NS;
    VDP_CFG_T *ptVdpCfg     = NULL;
    VDP_CFG_T *ptOpVdpCfg   = NULL;
    VDP_CFG_T *ptMainVdpCfg = NULL;
    B2R_OBJECT_T  *this     = NULL;
    B2R_HAL_OMUX_T tOmux    = {0};

    ptVdpCfg    = _B2R_GetVdpConf(ucVdpId);
    ptOpVdpCfg  = _B2R_GetVdpConf(VDP_NS -(1 + ucVdpId));

    if(!ptVdpCfg || !ptOpVdpCfg)
    {
        LOG(0,"UcVDPid is invalid(%d)\n",ucVdpId);
        return ucRetId;
    }

    /*use the same b2r_id*/
    if(ptOpVdpCfg->ucB2rId == ucB2rId)
    {
        /*if get fbgid, it means other vdp is played and already use the b2rid,if not handle may become scart-out case*/
        ucOpFbgId = FBM_GetFbgByEs(ptOpVdpCfg->ucInputPort[0]);
        ucFbgId = FBM_GetFbgByEs(ptVdpCfg->ucInputPort[0]);

        LOG(1,"The same case:ucOpFbgId (%d) and ucFbgId (%d)\n",ucOpFbgId,ucFbgId);
        if(ucOpFbgId != FBM_FBG_ID_UNKNOWN)
        {
            LOG(1,"OpESid (%d) and ESid (%d)\n",ptOpVdpCfg->ucInputPort[0],ptVdpCfg->ucInputPort[0]);
            
            /*real scart out*/
            if(ptOpVdpCfg->ucInputPort[0] == ptVdpCfg->ucInputPort[0])
            {
                LOG(1,"Scart out case!!!\n");
                
                this =_B2R_GetObj(ucB2rId);
                
                ptMainVdpCfg = _B2R_GetVdpConf(VDP_1);

                ptMainVdpCfg->fgScartOut = TRUE;
                
                LOG(1,"Set main fgScartOut = TRUE!!!\n");
                
                tOmux.ucPath = VDP_1;
                if (ptVdpCfg)
                {
                    tOmux.fgScartOut = ptVdpCfg->fgScartOut;
                    LOG(1,"tOmux.fgScartOut = %d!!!\n",tOmux.fgScartOut);
                }
                
                if(this)
                {
                    B2R_HAL_Set(this->hB2r, B2R_HAL_OMUX, &tOmux);
                    LOG(1,"Set hardware Scart out flag!!!\n");
                }
                
            }
            else
            {
                LOG(0,"Non-scart out case!!!\n");
                /*update the b2rid*/
                if (ucB2rId % 2 == 1)
                {
                    ucRetId = ucB2rId - 1;
                }
                else
                {
                    ucRetId = ucB2rId + 1;
                }
            }
        }
        
    }
    else
    {
        LOG(1,"The different case!!!\n");
        ucRetId = ucB2rId;
    }

    return ucRetId;
}

BOOL _B2R_ChkHwSwitch(UCHAR ucVdpId,
                               UCHAR ucB2rId,
                               UCHAR* pucRetId)
{
    UCHAR ucFbgId        = FBG_MAX_NS;
    UCHAR ucChgB2rId     = B2R_NS;
    BOOL  b_HwIdChg      = FALSE;
    VDP_CFG_T* ptVdpConf = NULL;

    do
    {
        if(ucB2rId >= B2R_NS)
        {
            break;
        }
        
        ptVdpConf = _B2R_GetVdpConf(ucVdpId);
        if(!ptVdpConf)
        {
            LOG(0,"Break with ptVdpConf = false!!!\n");
            break;
        }

        if(!ptVdpConf->t_HwChg.b_HwChg)
        {
            LOG(10,"Break with ptVdpConf->t_HwChg.b_HwChg = false!!!\n");
            break;
        }

        LOG(1,"[CHK]VdpId(%d) B2rId(%d), b_HwChg(%d)!\n", ucVdpId, ucB2rId, ptVdpConf->t_HwChg.b_HwChg);
        ucFbgId = FBM_GetFbgByEs(ptVdpConf->ucInputPort[0]);
        ucChgB2rId = FBM_B2rResIdAccess(ucFbgId, RES_R, NULL);
        ptVdpConf->t_HwChg.b_HwChg = FALSE;

        LOG(1,"VdpId(%d) changeB2rId(%d)!\n", ucVdpId,  ucChgB2rId);
        if(ucChgB2rId >= B2R_NS ||
            ucChgB2rId == ucB2rId)
        {
            break;
        }
        
        *pucRetId = ucChgB2rId;
        b_HwIdChg = TRUE;
        
        LOG(1,"Here return OK with b2rid (%d)\n",ucChgB2rId);
        
    }while(0);

    return b_HwIdChg;
}

BOOL _B2R_IsHwResChg(B2R_OBJECT_T* this)
{
    BOOL  b_HwChg = FALSE;
    UCHAR ucB2rId;
    UCHAR ucChgId;
    VDP_CFG_T* ptVdpConf = NULL;
    
    do
    {
        if(!this)
        {
            LOG(0,"this == null\n");
            break;
        }

        ucB2rId = this->ucB2rId;
        if(ucB2rId >= B2R_NS)
        {
            LOG(0,"ucB2rId(%d) > B2R_NS\n",ucB2rId);
            break;
        }

        if(_B2R_ChkHwSwitch(VDP_1, ucB2rId, &ucChgId))
        {
            LOG(1,"VDP_1 with ucB2rId(%d) and chagid = %d\n",ucB2rId,ucChgId);
            ptVdpConf = _B2R_GetVdpConf(VDP_1);
            ptVdpConf->ucEnable = 0;

            B2R_MUTEX_UNLOCK(ucB2rId);
            
            _B2R_VsyncSendCmd(ucB2rId, VDP_CMD_SET_ENABLE);
            
            VDP_B2rSwitch(VDP_1, B2R_NS);

            VDP_B2rSwitch(VDP_1, ucChgId);

            B2R_MUTEX_LOCK(ucB2rId);
            
            b_HwChg = TRUE;
        }

        if(_B2R_ChkHwSwitch(VDP_2, ucB2rId, &ucChgId))
        {
            LOG(1,"VDP_2 with ucB2rId(%d) and chagid = %d\n",ucB2rId,ucChgId);
            ptVdpConf = _B2R_GetVdpConf(VDP_2);
            ptVdpConf->ucEnable = 0;
            
            B2R_MUTEX_UNLOCK(ucB2rId);

            _B2R_VsyncSendCmd(ucB2rId, VDP_CMD_SET_ENABLE);
            
            VDP_B2rSwitch(VDP_2, B2R_NS);

            VDP_B2rSwitch(VDP_2, ucChgId);
            
            B2R_MUTEX_LOCK(ucB2rId);

            b_HwChg = TRUE;
        }
    }while(0);
    
    return b_HwChg;
}

INT32 _B2R_HwResChgNotify(UCHAR ucFbgId,
                                  B2R_CHG_INFO_T*  pt_change)
{
    UCHAR ucEsId;
    INT32 i4Ret = B2R_OK;
    VDP_CFG_T* ptVdpConf = NULL;
    
    do
    {
        ucEsId = FBM_GetDecoderSrcId(ucFbgId);
        if(ucEsId >= MAX_ES_NS || pt_change == NULL)
        {
            LOG(1, "[%s-%d]Fail\n", __func__, __LINE__);
            i4Ret = B2R_INV_ARG;
            break;
        }
        
        LOG(1,"ucFbgId(%d),t_change.u1VdpId(%d),t_change.b_change(%d),esid(%d)\n",
            ucFbgId,pt_change->u1VdpId,pt_change->b_change,ucEsId);

        ptVdpConf = _B2R_GetVdpConf(pt_change->u1VdpId);
            if (ptVdpConf &&
                ptVdpConf->ucInputPort[0] == ucEsId)
            {
            ptVdpConf->t_HwChg.b_HwChg = pt_change->b_change;

            LOG(1,"Vdp(%d) HwRes Chg To(%d)!\n", pt_change->u1VdpId, pt_change->b_change);
                break;
            }
        
    }while(0);

    return i4Ret;
}
#endif

#if defined(CC_FLIP_MIRROR_SUPPORT)
static void _B2R_DoMirrorFlip(B2R_OBJECT_T* this, BOOL fgDo)
{
    B2R_HAL_MF_T tMF;
    UINT8 u1FlipMirrorEnable;

    if (!this)
    {
        return;
    }

    u1FlipMirrorEnable = u1GetFlipMirrorConfig();

    if (fgDo != 0)
    {
        tMF.fgFlip   = (u1FlipMirrorEnable & SYS_FLIP_CONFIG_ON)   ? TRUE : FALSE;
        tMF.fgMirror = (u1FlipMirrorEnable & SYS_MIRROR_CONFIG_ON) ? TRUE : FALSE;
    }
    else
    {
        tMF.fgFlip   = FALSE;
        tMF.fgMirror = FALSE;
    }

    B2R_HAL_Set(this->hB2r, B2R_HAL_MIRROR_FLIP, &tMF);
}

#endif
static VOID _B2R_SetB2RMirrorFlip(B2R_OBJECT_T* this)
{
#if defined(CC_FLIP_MIRROR_SUPPORT)
    UCHAR ucVdpId;
    UINT32 u4MainFlipModule = u4GetFlipMirrorModule(VDP_1);
    UINT32 u4SubFlipModule = u4GetFlipMirrorModule(VDP_2);
    UINT8 ucTVEEnable;  // If TVEEnable then it is SCART out DTV

    if (!this)
    {
        return;
    }
    ucVdpId = _B2R_GetVdpId(this->ucB2rId);
    TVE_GetEnable(TVE_1, &ucTVEEnable);

    // if M/F by MJC => B2R(X), others(X)
    // else if not M/F by MJC then follow the table
    // (X) means don't do Mirror/Flip
    // (O) means do Mirror/Flip
    // M means Main path
    // S means Sub path
    // Main    Sub     SCART       B2R   Others   Output
    // -------------------------------------------------
    // DTV     X       DTV      M:  O      X        O
    //                          S:  O      O        X
    // HDMI    X       DTV      M:  X      O        O
    //                          S:  O      O        X
    // DTV     HDMI    X        M:  O      X        O
    //                          S:  X      O        O
    // HDMI    DTV     X        M:  X      O        O
    //                          S:  X      O        O
    LOG(10, "u4MainFlipModule = %d, u4SubFlipModule = %d\n", u4MainFlipModule, u4SubFlipModule);

    switch (ucVdpId)
    {
    case VDP_1:
        if ((u4MainFlipModule == FLIP_BY_MJC)
#if defined(CC_MT5882)
            ||(u4MainFlipModule == FLIP_NONE)
#endif
#ifdef DRV_USE_EXTERNAL_3D_FRC // MT8283_FLIP_MIRROR
            ||(u4MainFlipModule == FLIP_BY_EXT3DFRC)
#endif
                )
        {
            _B2R_DoMirrorFlip(this, FALSE);
        }
        else
        {
            if (u4MainFlipModule == FLIP_BY_B2R)
            {
                _B2R_DoMirrorFlip(this, TRUE);
            }
        }
        break;
    case VDP_2:
        if ((u4SubFlipModule == FLIP_BY_MJC)
#if defined(CC_MT5882)
            ||(u4SubFlipModule == FLIP_NONE)
#endif
#ifdef DRV_USE_EXTERNAL_3D_FRC // MT8283_FLIP_MIRROR
            ||(u4SubFlipModule == FLIP_BY_EXT3DFRC)
#endif
            )
        {
            _B2R_DoMirrorFlip(this, FALSE);
        }
        else
        {
            if (ucTVEEnable != 0)
            {
                _B2R_DoMirrorFlip(this, TRUE);
            }
            else
            {
                if (u4SubFlipModule == FLIP_BY_B2R)
                {
                    _B2R_DoMirrorFlip(this, TRUE);
                }
                else
                {
                    _B2R_DoMirrorFlip(this, FALSE);
                }
            }
        }
        break;
    default:
        LOG(0, "VDP ID wrong, Id = %d\n", ucVdpId);
        ASSERT(0);
        break;
    }
#ifdef CC_SECOND_B2R_SUPPORT
    if (this->ucB2rId < B2R_NS)
    {
        B2R_VAR_T* prB2rVar = this->ptB2rVar;
        if (prB2rVar->ucMirrorStatus == MIRROR_ON || prB2rVar->ucMirrorStatus == MIRROR_OFF)
        {
            B2R_HAL_MF_T tMF;
            tMF.fgMirror = (prB2rVar->ucMirrorStatus == MIRROR_ON ? TRUE : FALSE);
            tMF.fgFlip   = FALSE;
            B2R_HAL_Set(this->hB2r, B2R_HAL_MIRROR_FLIP, &tMF);
        }
    }
#endif
#endif

    UNUSED(this);
}

//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/** Brief of _VDP_FrcInit.
 */
//-----------------------------------------------------------------------------
void _B2R_FrcInit(void)
{
    UCHAR ucB2rId;
    CRIT_STATE_T _rCritState;

    _prDbase = DBS_Lock(&_rCritState);
    ASSERT(_prDbase);
    VERIFY(DBS_Unlock(_prDbase, _rCritState));

    _ucFrcMuteInitiated = 0;

    x_memset(&_arB2rVar[0], 0, sizeof(_arB2rVar));    

    for (ucB2rId = 0; ucB2rId < B2R_NS; ucB2rId++)
    {
        B2R_VAR_T* prB2rVar = &_arB2rVar[ucB2rId];
        
        _arVdpPrm[ucB2rId].ucMode = VDP_MODE_BG;

#ifdef CC_B2R_ENABLE_CHG_FRAME_MSG
        if ((_ahChgFrameQueue[ucB2rId]) == (HANDLE_T)(NULL))
        {
            CHAR szBuf[16];
            x_snprintf(szBuf, sizeof(szBuf), "ChgFrmQ%d", ucB2rId);

            VERIFY(x_msg_q_create(&_ahChgFrameQueue[ucB2rId], szBuf, sizeof(VDP_B2R_CHG_FRAME_MSG_T), 64) == OSR_OK);
            VERIFY(x_sema_create(&_ahChgFrameMutex[ucB2rId], X_SEMA_TYPE_BINARY, X_SEMA_STATE_UNLOCK) == OSR_OK);
        }
#endif
        prB2rVar->fgPendingChgFrmMsg = FALSE;
        x_memset(&(prB2rVar->rPendingChgFrmMsg), 0, sizeof(VDP_B2R_CHG_FRAME_MSG_T));
        VERIFY(x_sema_create(&(_hDispBufLockMutex[ucB2rId]), X_SEMA_TYPE_MUTEX, X_SEMA_STATE_UNLOCK) == OSR_OK);
        _B2R_FrmInit(ucB2rId, 0, TRUE);

        prB2rVar->ucMirrorStatus = INVALID_MIRROR_STATUS;
        prB2rVar->ucRepeatFbForPushTrickMode = FBM_FB_ID_UNKNOWN;
        prB2rVar->u1PlayMode = FBM_FBG_MODE_NS;
        prB2rVar->i4TimeShiftSpeed = 100;
        prB2rVar->fgFirstFrame = TRUE;
    }

    _ucFrcMuteInitiated = 1;

}

//-----------------------------------------------------------------------------
/** Brief of _VDP_FrcProc.
 */
//-----------------------------------------------------------------------------
UINT32 _B2R_FrcProc(B2R_OBJECT_T * this,  UCHAR ucBottom, UCHAR ucRightView)
{
    B2R_PRM_T* prFrcPrm;
    VDP_CFG_T* prVdpConf;
    UCHAR ucVdpId;
    B2R_VAR_T* prB2rVar;
    B2R_PRM_T *  ptVdpPrm;
    UCHAR ucB2rId;
    UINT32 u4Ret = 0;
//	UCHAR i;
    #ifdef CC_B2R_SUPPORT_GAME_MODE
    BOOL fgGameModeStart =FALSE;
    #endif
    do 
    {
        if (!this)
        {
            break;
        }
        ucB2rId = this->ucB2rId;
        
        if (ucB2rId >= B2R_NS)
        {
            break;
        }
        ucVdpId = _B2R_GetVdpId(ucB2rId);
        if (ucVdpId >= VDP_NS)
        {
            break;
        }
        prB2rVar = this->ptB2rVar;
        ptVdpPrm = this->ptB2rPrm;
        
        _B2R_SetB2RMirrorFlip(this);


        if ((ptVdpPrm->fgB2R3DEnable) && (!ptVdpPrm->fgB2RForce2D)
                /*&& (_arVdpPrm[ucVdpId].u1B2R3DType == B2R_3D_FRAME_SEQUENTIAL)*/
                && ptVdpPrm->ucProgressiveOut ==1)
        {
            BOOL fgLeft;
            if (ucRightView ==1)
            {
                LOG(10,"FrcProc R ");
            }
            else
            {
                LOG(10,"FrcProc L ");
            }

            LOG(10,"%d\n",ptVdpPrm->u2CurrentVsyncNs);

            if ((ptVdpPrm->u2CurrentVsyncNs & 0x1) == 1)
            {
                fgLeft = FALSE;
            }
            else
            {
                fgLeft = TRUE;
            }
            B2R_HAL_Set(this->hB2r, B2R_HAL_LR_STATE, &fgLeft);
        }

#if 1//def CC_B2R_3D_MVC_INTERLACE_SUPPORT
        else if (ptVdpPrm->fgB2R3DEnable && !ptVdpPrm->fgB2RForce2D
                 && ptVdpPrm->u1B2R3DType == B2R_3D_FRAME_SEQUENTIAL
                 && ptVdpPrm->ucProgressiveOut ==0)
        {

            //if(_arVdpPrm[ucVdpId].u2CurrentVsyncNs == 1 || _arVdpPrm[ucVdpId].u2CurrentVsyncNs==3 || _arVdpPrm[ucVdpId].u2CurrentVsyncNs==5)
            BOOL fgLeft;

            if ((ptVdpPrm->u2CurrentVsyncNs & 0x1) == 1)
            {
                fgLeft = FALSE;
            }
            else
            {
                fgLeft = TRUE;
            }
            B2R_HAL_Set(this->hB2r, B2R_HAL_LR_STATE, &fgLeft);

            if (ptVdpPrm->u2CurrentVsyncNs ==0)
            {
                fgLeft = TRUE;
                B2R_HAL_Set(this->hB2r, B2R_HAL_TB_STATE, &fgLeft);
            }
            /*if(_arVdpPrm[ucVdpId].u2CurrentVsyncNs ==0 && _arVdpPrm[ucVdpId].ucReady == 1)
            {
                if(!((ucRightView==1) && (ucBottom==1)))
                {
                    return 0;
                }
            }*/

        }
#endif
        
        // Frame Rate Control Process is NOT required if NOT enable
        if (!_B2R_IsEnableDP(this))
        {
            break;
        }
        
        // backup the current field information
        ptVdpPrm->ucBottom = ucBottom;
        // reset pending
        ptVdpPrm->ucPendingNewFb = 0;
        prFrcPrm  = ptVdpPrm;
        prVdpConf = _B2R_GetDP(this);

        
        if (prVdpConf->ucEarlyDisp == 1)
        {
            if (prFrcPrm->u4WaitUnmute != 0)
            {
                prFrcPrm->u4WaitUnmute--;
#ifdef TIME_MEASUREMENT
                if(fgB2REarlyDisp)
                {
                    TMS_DIFF_EX(TMS_FLAG_CHG_CHL, TMS_CHL_CHE_TIME, "B2R Wait Early display");
                    fgB2REarlyDisp=FALSE;
                }
#endif        
            }
            else
            {
#ifdef TIME_MEASUREMENT
                TMS_DIFF_EX(TMS_FLAG_CHG_CHL, TMS_CHL_CHE_TIME, "B2R Early display Unmute");
#endif        
                _B2R_SetPathMute(ucB2rId, MUTE_MODULE_DTV, 0, FALSE);
                _B2R_SetDPEarlyDisp(this, 0);
                LOG(3, "DTV Unmute Early Display -1 B2R(%d)\n", ucB2rId);
            }
        }
        
        // FBG pending release
        if (FBM_ChkFrameBufferFlag(prFrcPrm->ucFbgId, FBM_FLAG_RELEASE_FBG))
        {
            FBM_ReleaseDispQ(prFrcPrm->ucFbgId);

            if (prFrcPrm->ucReady != 0)
            {
                LOG(1, "FBM_FLAG_RELEASE_FBG\n");

                if (prFrcPrm->ucReleaseFbId != FBM_FB_ID_UNKNOWN)
                {
                    _B2R_SetFrameBufferStatus(this, prFrcPrm->ucFbgId, prFrcPrm->ucReleaseFbId, FBM_FB_STATUS_EMPTY);
                    prFrcPrm->ucReleaseFbId = FBM_FB_ID_UNKNOWN;
                }

                if (prFrcPrm->ucFbId != FBM_FB_ID_UNKNOWN)
                {
                    _B2R_SetFrameBufferStatus(this, prFrcPrm->ucFbgId, prFrcPrm->ucFbId, FBM_FB_STATUS_EMPTY);
                    prFrcPrm->ucFbId = FBM_FB_ID_UNKNOWN;
                }

                prFrcPrm->ucReleased = 1;
                prFrcPrm->ucReady = 0;

                //_VDP_HalForceBg(ucB2rId, 1);
                _B2R_ForceBg(ucB2rId, 1);

                //_VDP_VsyncNotify(VDP_MSG_NOTIFY, (UINT32)(VDP_GetVdpId(ucB2rId)), VDP_B2R_NO_SIGNAL, 0);
                _B2R_PostNotify(this, VDP_MSG_NOTIFY, VDP_B2R_NO_SIGNAL, 0);
            }

            break;//return prB2rVar->u4ReturnValue;
        }
        // frame rate config core
        switch (ptVdpPrm->ucMode)
        {
        default:
            break;
        case VDP_MODE_DEINT:        // Deinterlace Mode, Temp
        case VDP_MODE_NORMAL:        // Normal Mode
        case VDP_MODE_BG:            // Background Mode, do nothing
            // reference 8105 main_vdo.c (vMVdoDispProc)
            // single input port at normal mode, master port only

            // clear return value
            prB2rVar->u4ReturnValue = 0;

            // call back protection
            _B2R_PtsCbProc(this, 1);

            // check fbm flag
            if (FBM_ChkFrameBufferFlag(prFrcPrm->ucFbgId, FBM_FLAG_CLEAR_FRC_PRM))
            {
                FBM_ClrFrameBufferFlag(prFrcPrm->ucFbgId, FBM_FLAG_CLEAR_FRC_PRM);
                _B2R_FrcResetPort(ucB2rId, 0);
            }

            // Increase Current Vsync Ns here or at ISR ?
            FRC_INC_VSYNC_NS(prFrcPrm);
#ifdef CC_B2R_SUPPORT_GAME_MODE
          if(eGameMode[ucB2rId]==B2R_GAME_MODE_LOW_DELAY)
          {
              vdpReadyReleaseFB(this);
          }
          else
#endif
           {
                if (prFrcPrm->ucReleaseFbId != FBM_FB_ID_UNKNOWN)
                {
                    if (prFrcPrm->ucReleased == 0)
                    {
                        _B2R_SetFrameBufferStatus(this, prFrcPrm->ucFbgId, prFrcPrm->ucReleaseFbId, FBM_FB_STATUS_EMPTY);
                    }
                    prFrcPrm->ucReleased = 0;

                    prFrcPrm->ucReleaseFbId = FBM_FB_ID_UNKNOWN;
                }
            }
#ifdef CC_3D_MM_DS_SUPPORT
            if (prFrcPrm->ucSubFbgId != FBM_FBG_ID_UNKNOWN && prFrcPrm->ucSubReleaseFbId != FBM_FB_ID_UNKNOWN)
            {
                if (prFrcPrm->ucSubReleased == 0)
                {
                    _B2R_SetFrameBufferStatus(this, prFrcPrm->ucSubFbgId, prFrcPrm->ucSubReleaseFbId, FBM_FB_STATUS_EMPTY);
                }
                prFrcPrm->ucSubReleased = 0;

                prFrcPrm->ucSubReleaseFbId = FBM_FB_ID_UNKNOWN;
            }
#endif
#ifndef CC_SUPPORT_NPTV_SEAMLESS 
            if (prFrcPrm->ucSeamlessVDPMode == VDP_SMLS_FRAME_COMING)
            {
                _B2R_NPTVSeamlessStatus(this, VDP_SMLS_MODE_NONE);
            }
#endif

            if (prFrcPrm->fgRenderRegionChg)
            {
                _VDP_SetXYOffset(ucB2rId);
                prFrcPrm->fgRenderRegionChg = FALSE;
            }

#ifdef VDP_B2R_PSCAN_CRC_FLOW
            if ((prFrcPrm->u4Pts == _u4SetPts) &&
                    (((prFrcPrm->ucProgressiveOut == 0) && (FRC_CHANGE_FIELD(prFrcPrm))) ||
                     ((prFrcPrm->ucProgressiveOut != 0) && (prFrcPrm->u2CurrentVsyncNs == 1)))
               )
            {
                B2R_HAL_STATUS_T  rHalStatus;
                static UINT32 u4ReadCnt = 0;
                x_memset(&rHalStatus,0,sizeof(B2R_HAL_STATUS_T));
                B2R_HAL_Get(this->hB2r, B2R_HAL_STATUS, &rHalStatus);
                LOG(0, "(PTS, Bd, Bp, PcSrc, Pc, MJC in, MJC out) = (%08X, %08X, %08X, %08X, %08X, %08X, %08X)\n", prFrcPrm->u4Pts,
                        rHalStatus.u4DramChkSum, rHalStatus.u4CrC, rHalStatus.u4PSCANSrcCrc, rHalStatus.u4PSCANOutCrc,
                        rHalStatus.u4MJCSrcCrc,rHalStatus.u4MJCOutCrc);
                if(prB2rVar &&
                        u4ReadCnt < 10)
                {
                    prB2rVar->fgPauseMM = 1;
                    u4ReadCnt ++;
                }
                else
                {
                    u4ReadCnt = 0;
                    prB2rVar->fgPauseMM = 0;
                }
            }
#endif

            if (FRC_DISP_FINISH(prFrcPrm)&&_NptvSeamlessCheckChgFrameMsg(ucB2rId,this))
            {
                if (prFrcPrm->u2RestoreNs != 0)
                {
                    // Restore target NS after repeat done. It is prevent unexpected repeat forever.
                    prFrcPrm->u2TargetNs = prFrcPrm->u2RestoreNs;
                    prFrcPrm->u2RestoreNs = 0;
                }
#ifdef CC_B2R_SUPPORT_GAME_MODE
                if(_arVdpPrm[ucB2rId].ucGameModeFrameChange!=0)
                {
                    _arVdpPrm[ucB2rId].ucGameModeFrameChange=0;
                }
#endif
                #ifndef CC_SUPPORT_NPTV_SEAMLESS 
                if ((prFrcPrm->ucSeamlessVDPMode == VDP_SMLS_PREPARE) && bVRMReadyForB2R(ucVdpId))
                {
                    _B2R_NPTVSeamlessStatus(this, VDP_SMLS_READY);
                }
                else if ((prFrcPrm->ucSeamlessVDPMode == VDP_SMLS_PREPARE_WHILE_START_PLAY) && bVRMReadyForB2R(ucVdpId))
                {
                    _B2R_NPTVSeamlessStatus(this, VDP_SMLS_READY_WHILE_START_PLAY);
                }
                #endif

#ifdef CC_B2R_RES_SUPPORT

   #ifdef CC_SUPPORT_PIPELINE
          if(!fgLGPipLine)
   #endif
                if(_B2R_IsHwResChg(this))
                {
                    //break;
                }
#endif
				


                _B2R_ChangeFrameBuffer(this);
                #ifndef CC_SUPPORT_NPTV_SEAMLESS //Lijia NPTV Seamless debug
                if (prFrcPrm->ucSeamlessVDPMode == VDP_SMLS_FRAME_COMING)
                {
                   sdf
                    BOOL fgSet = TRUE;
                    B2R_HAL_Set(this->hB2r, B2R_HAL_PITCH, &(prVdpConf->u4SrcLineSize));
                    B2R_HAL_Set(this->hB2r, B2R_HAL_SET_RESOLUTION, &fgSet);
                    vVRMB2RTrigger(ucVdpId);
                }
                #endif

#ifdef VDP_B2R_PSCAN_CRC_FLOW
                if (prFrcPrm->u4Pts == _u4SetPts)
                {
                    /* Calculate PSCAN CRC at specified frame */
#if !(defined(CC_MT5396) || defined(CC_MT5368) || defined(CC_MT5389) || defined(CC_MT5398) || defined(CC_MT5880) || defined(CC_MT5881) || defined(CC_MT5399)|| defined(CC_MT5890)|| defined(CC_MT5882))
                    // Clear PSCAN CRC
                    //IO_WRITE32(PSCAN_BASE, 0x05C, (IO_READ32(PSCAN_BASE, 0x05C)&0xDFFFFFFFF)|0x20000000);
                    vDrvDISetCRCClr(1);
                    // Trigger PSCAN CRC
                    //IO_WRITE32(PSCAN_BASE, 0x05C, (IO_READ32(PSCAN_BASE, 0x05C)&0xEFFFFFFFF)|0x10000000);
                    vDrvDISetCRCTrigger(1);
                    LOG(2, "Trigger PSCAN CRC\n");
#endif
    
                }
#endif


                if (prFrcPrm->ucReady == 0)
                {
                    UCHAR ucAvSyncMode;
                    UCHAR ucStcSrc;
                #ifdef CC_B2R_SUPPORT_GAME_MODE
                    if((eGameMode[ucB2rId]==B2R_GAME_MODE_HIGHT_QUALITY)&&(FBM_CheckFrameBufferDispQ(prFrcPrm->ucFbgId)>=2))
                    {
                        fgGameModeStart=TRUE;
                    }
                   else
                    {
                        fgGameModeStart=FALSE;
                    }
                #endif
#ifdef CC_B2R_SUPPORT_GAME_MODE
                if ((prFrcPrm->ucFbId != FBM_FB_ID_UNKNOWN) &&
                        (prFrcPrm->ucNotSupport == 0)&&((eGameMode[ucB2rId]!=B2R_GAME_MODE_HIGHT_QUALITY)||fgGameModeStart))
#else
                   if ((prFrcPrm->ucFbId != FBM_FB_ID_UNKNOWN) &&
                            (prFrcPrm->ucNotSupport == 0))
#endif
                    {

					   LOG(2,"bVRMReadyForB2R(%d),Display Q=%d\n",bVRMReadyForB2R(ucVdpId),FBM_CheckFrameBufferDispQ(prFrcPrm->ucFbgId));
                       prFrcPrm->u4FrameCount=0;
#ifdef CC_B2R_SUPPORT_GAME_MODE

                       LOG(3,"========B2R ready to play 1st Frame===eGameMode=%d,DisplayQ Num=%d\n",eGameMode[ucB2rId],FBM_CheckFrameBufferDispQ(prFrcPrm->ucFbgId));
#endif                     
#ifdef CC_SUPPORT_PIPELINE
/*
                         if(bApiQuearyScartOutStatus())
                         {
                               VDP_PipeConnectFromVdp(VDP_2,prVdpConf->ucInputPort[0]);
                              //LG_pipLineScartConnect(VDP_2,prVdpConf->ucInputPort[0]);
                         }

						   for(i=0;i<VDP_NS;i++)
						   {
						   	  if(bApiQuearyVSCConnectStatus(i)==SV_VD_MPEGHD)
							   {
							       VDP_PipeConnectFromVdp(i, prVdpConf->ucInputPort[0]);
								   break;
							   }
						   }
*/
#endif
#ifdef TIME_MEASUREMENT
                        TMS_DIFF_EX(TMS_FLAG_CHG_CHL, TMS_CHL_CHE_TIME_DRV, "B2R ready to play 1st Frame");
#endif
                        //_VDP_HalForceBg(ucB2rId, 0);
                        _B2R_ForceBg(ucB2rId, 0);
                        prFrcPrm->ucReady = 1;
                        
#ifdef CC_MAPLE_CUST_DRV
                        _B2R_Cust_info_cfg(prVdpConf,prFrcPrm);
#endif

                        FBM_GetSyncStc(prFrcPrm->ucFbgId, &ucAvSyncMode, &ucStcSrc);

#ifndef CC_B2R_WAIT_NPTV_STABLE
                        FBM_SetFrameBufferFlag(prFrcPrm->ucFbgId, FBM_FLAG_DISPLAYING);
#else
                        FBM_GetPlayMode(prFrcPrm->ucFbgId, &(prB2rVar->u1PlayMode));
                        if (( FBM_FBG_DTV_MODE == prB2rVar->u1PlayMode) || 
                            (( FBM_FBG_MM_MODE == prB2rVar->u1PlayMode) && (ucAvSyncMode == VID_SYNC_MODE_NONE)))
                        {
                            FBM_SetFrameBufferFlag(prFrcPrm->ucFbgId, FBM_FLAG_DISPLAYING);
                        }
                        if (( FBM_FBG_MM_MODE == prB2rVar->u1PlayMode) && (ucAvSyncMode == VID_SYNC_MODE_NONE))
                        {
                            FBM_ClrFrameBufferGlobalFlag(prFrcPrm->ucFbgId, FBM_FLAG_FB_NO_TIMEOUT);
                        }
#endif
                        
                        //_VDP_VsyncNotify(VDP_MSG_UNMUTE_CB, (UINT32)ucVdpId, 1, 0);
                        //_VDP_VsyncNotify(VDP_MSG_PLAY_DONE_CB, (UINT32)ucVdpId, 0, 0);
                        _B2R_PostNotify(this, VDP_MSG_UNMUTE_CB, 1, 0);
                        _B2R_PostNotify(this, VDP_MSG_PLAY_DONE_CB, 0, 0);

                        if (prB2rVar->rVdpDtvFreeze.u4OnOff != 0)
                        {
                            FBM_SetFrameBufferFlag(prFrcPrm->ucFbgId, FBM_FLAG_FREEZE);
                            FBM_ReleaseDispQ(prFrcPrm->ucFbgId);
                        }

#ifdef TIME_MEASURE
                        if (prFrcPrm->prFbCounter)
                        {
                            HAL_TIME_T rResult;

                            HAL_GetDeltaTime(&rResult, &(_prDbase->rDmx.rEnableTime), &(prFrcPrm->prFbCounter->rStartTime));

                            LOG(1, "Video Time Measure (%d.%06d)\n", rResult.u4Seconds, rResult.u4Micros);
                        }
#endif


                        // notify SCPOS
                        if (!(prB2rVar->fgSeqChgSpeedup))
                        {
                            //_VDP_VsyncNotify(VDP_MSG_NOTIFY, (UINT32)ucVdpId, VDP_B2R_START_PLAY, 0);
                            _B2R_PostNotify(this, VDP_MSG_NOTIFY, VDP_B2R_START_PLAY, 0);
                            prB2rVar->fgModeChangeSent = TRUE;
                        }
#ifdef VDP_MUTE_EARLY_DISPLAY
                        if ((prVdpConf->ucEarlyDisp == 0) && (prB2rVar->fgSeqChgSpeedup))
#endif
                        {
#ifdef TIME_MEASUREMENT
                            TMS_DIFF_EX(TMS_FLAG_CHG_CHL, TMS_CHL_CHE_TIME_DRV, "B2R UnMute +Speedup");
#endif
                            _B2R_SetPathMute(ucB2rId, MUTE_MODULE_DTV, 0, FALSE);
                            LOG(3, "DTV Unmute Early Display + Seq Chg Speedup B2R(%d)\n", ucB2rId);
                       }
                       if(prFrcPrm->t_b2r_vdp_uhd.fgEnable)
                       {
                           prFrcPrm->t_b2r_vdp_uhd.u4StartPTS = prFrcPrm->u4Pts;
                       }
                    }
                    else
                    {
                        //_VDP_HalForceBg(ucB2rId, 1);
                        _B2R_ForceBg(ucB2rId, 1);
                        FBM_ClrFrameBufferFlag(prFrcPrm->ucFbgId, FBM_FLAG_DISPLAYING);
                    }
                }
            }

            u4Ret = prB2rVar->u4ReturnValue;
            break;
        }

    }while (0);
    
    return u4Ret;//prB2rVar->u4ReturnValue;
}

//-----------------------------------------------------------------------------
/** Brief of _B2R_FrcProcEarly.
 */
//-----------------------------------------------------------------------------
void _B2R_FrcProcEarly(B2R_OBJECT_T * this, UCHAR ucBottom)
{
    B2R_PRM_T* prFrcPrm;
    VDP_CFG_T* prVdpConf;
    UCHAR ucB2rId;     
    UNUSED(ucBottom);

    if (!this)
    {
        return;
    }

    ucB2rId = this->ucB2rId;
    prVdpConf = _B2R_GetDP(this);
    // Frame Rate Control Process is NOT required if NOT enable
    if (!prVdpConf || !(prVdpConf->ucEnable))
    {
        return;
    }
    
    prFrcPrm = this->ptB2rPrm;
     
    // FBG pending release
    if (FBM_ChkFrameBufferFlag(prFrcPrm->ucFbgId, FBM_FLAG_RELEASE_FBG))
    {
        if (prFrcPrm->ucReady != 0)
        {
            LOG(1, "FBM_FLAG_RELEASE_FBG\n");

            if (prFrcPrm->ucReleaseFbId != FBM_FB_ID_UNKNOWN)
            {
                _B2R_SetFrameBufferStatus(this, prFrcPrm->ucFbgId, prFrcPrm->ucReleaseFbId, FBM_FB_STATUS_EMPTY);
                prFrcPrm->ucReleaseFbId = FBM_FB_ID_UNKNOWN;
            }

            if (prFrcPrm->ucFbId != FBM_FB_ID_UNKNOWN)
            {
                _B2R_SetFrameBufferStatus(this, prFrcPrm->ucFbgId, prFrcPrm->ucFbId, FBM_FB_STATUS_EMPTY);
                prFrcPrm->ucFbId = FBM_FB_ID_UNKNOWN;
            }

            prFrcPrm->ucReleased = 1;
            prFrcPrm->ucReady = 0;

            //_VDP_HalForceBg(ucB2rId, 1);
            _B2R_ForceBg(ucB2rId, 1);
            _B2R_PostNotify(this, VDP_MSG_NOTIFY, VDP_B2R_NO_SIGNAL,0);
        }

        return;
    }

    // frame rate config core
    switch (prFrcPrm->ucMode)
    {
    case VDP_MODE_DEINT:        // Deinterlace Mode, Temp
    case VDP_MODE_NORMAL:        // Normal Mode
    case VDP_MODE_BG:            // Background Mode, do nothing
        if (FBM_ChkFrameBufferFlag(prFrcPrm->ucFbgId, FBM_FLAG_DEBLOCKING))
        {
            if (prFrcPrm->ucReleaseFbId != FBM_FB_ID_UNKNOWN)
            {
                if (prFrcPrm->ucReleased == 0)
                {
                    _B2R_SetFrameBufferStatus(this, prFrcPrm->ucFbgId, prFrcPrm->ucReleaseFbId, FBM_FB_STATUS_EMPTY);
                }
                prFrcPrm->ucReleased = 0;

                prFrcPrm->ucReleaseFbId = FBM_FB_ID_UNKNOWN;
            }
        }
        break;

    default:
        break;
    }
}



//-----------------------------------------------------------------------------
/** Brief of _B2R_FrcHandlerEvent.
 */
//-----------------------------------------------------------------------------
UINT32 _B2R_FrcHandlerEvent(UCHAR ucB2rId, UINT32 u4Event)
{
    B2R_PRM_T* _pFrcPrm;
    UCHAR ucFbgId = 0xFF;
    UCHAR ucOldFbgId;
    BOOL fgDisableB2r = TRUE;
    B2R_PRM_T *  ptVdpPrm;
    VDP_CFG_T* prVdpConf;
    UCHAR ucVdpId = _B2R_GetVdpId(ucB2rId);

    if (ucVdpId >= VDP_NS || ucB2rId >= B2R_NS)
    {
        LOG(0, "ucB2rId = %d, ucVdpId = %d [%s-%d]\n", ucB2rId, ucVdpId, __FUNCTION__, __LINE__);
        return 0;
    }
    ptVdpPrm= &_arVdpPrm[ucB2rId];
    prVdpConf = _B2R_GetVdpConf(ucVdpId);
    // Handle configuration(input/output) changes
    u4Event &= (VDP_EVENT_MASTER_CHG |
                VDP_EVENT_INPUT_CHG |
                VDP_EVENT_OUTPUT_CHG |
                VDP_EVENT_MODE_CHG |
                VDP_EVENT_ENABLE_CHG);

#ifdef CC_SUPPORT_TVE
    // Handle  Main change from DTV to other source when TV Scart out.
    if ((_arVdpCfg[VDP_1].ucB2rId == B2R_1) &&
            (_arVdpCfg[VDP_2].ucB2rId == B2R_1)&&
            (_B2R_IN_Enable_Status(VDP_1,B2R_DISABLE)) &&
            (_B2R_IN_Enable_Status(VDP_2,B2R_ENABLE)))
    {
        fgDisableB2r = FALSE;
    }
#endif

    while (u4Event)
    {
        if (u4Event & VDP_EVENT_MASTER_CHG)
        {
            u4Event &= ~VDP_EVENT_MASTER_CHG;
        }

        if (u4Event & VDP_EVENT_ENABLE_CHG)
        {
            u4Event &= ~VDP_EVENT_ENABLE_CHG;
            if (!(prVdpConf->ucEnable))
            {
                // clean port
                _pFrcPrm = &(_arVdpPrm[ucB2rId]);

                // clean Fbg & VdpPrm
                if (_pFrcPrm->ucFbgId != FBM_FBG_ID_UNKNOWN)
                {

                    if ((_B2R_IN_Enable_Status(VDP_1,B2R_DISABLE)) &&
                            (_B2R_IN_Enable_Status(VDP_2,B2R_DISABLE)))
                    {
                        FBM_FlushLockFrameBuffer(_pFrcPrm->ucFbgId);
                        FBM_ClrFrameBufferFlag(_pFrcPrm->ucFbgId, FBM_FLAG_DISPLAYING);
                    }

                    //disable vpd before stop vdec  and need to release resource which is locked by B2R
                    //ignore triggerCopy == 1
                    if (fgDisableB2r)
                    {
                        _B2R_RelResource(ucB2rId);//LIJIA
                    }

                    _B2R_FrmInit(ucB2rId, 0, FALSE);
                }

                //_VDP_HalForceBg(ucB2rId, 1);
                _B2R_ForceBg(ucB2rId, 1);
            }

            // update input information
            u4Event |= VDP_EVENT_INPUT_CHG;

            // update output information
            u4Event |= VDP_EVENT_OUTPUT_CHG;

            // update mode information
            u4Event |= VDP_EVENT_MODE_CHG;

            LOG(1, "Enable change\n");
        }

        if (u4Event & VDP_EVENT_INPUT_CHG)
        {
            u4Event &= ~VDP_EVENT_INPUT_CHG;

            VERIFY(_B2R_GetInput(ucB2rId, 0, &ucFbgId) == VDP_SET_OK);
            _pFrcPrm = &(_arVdpPrm[ucB2rId]);

            if (ucFbgId != _pFrcPrm->ucFbgId)
            {
                ucOldFbgId = _pFrcPrm->ucFbgId;

                LOG(1,"OldFbgId(%d) change to FbgId(%d)!\n",
                    ucOldFbgId, ucFbgId);

                // clean old fbg
                if (ucOldFbgId != FBM_FBG_ID_UNKNOWN)
                {

                    if ((_B2R_IN_Enable_Status(VDP_1,B2R_DISABLE)) &&
                            (_B2R_IN_Enable_Status(VDP_2,B2R_DISABLE)))
                    {
                        FBM_FlushLockFrameBuffer(ucFbgId);
                    }
                }

                // clean new fbg
                if (ucFbgId != FBM_FBG_ID_UNKNOWN)
                {

                    if ((_B2R_IN_Enable_Status(VDP_1,B2R_DISABLE)) &&
                            (_B2R_IN_Enable_Status(VDP_2,B2R_DISABLE)))
                    {
                        FBM_FlushLockFrameBuffer(ucFbgId);
                    }
                }

                _B2R_FrmInit(ucB2rId, 0, FALSE);
                _pFrcPrm->ucFbgId = ucFbgId;

                // update output information
                u4Event |= VDP_EVENT_OUTPUT_CHG;

                // update mode information
                u4Event |= VDP_EVENT_MODE_CHG;

                LOG(1, "Input change\n");
            }
        }

        if (u4Event & VDP_EVENT_MODE_CHG)
        {
            u4Event &= ~VDP_EVENT_MODE_CHG;

            LOG(1,"B2rId (%d) copy VdpId (%d) cfg mode , ucMode Change to %d!\n",
                ucVdpId,ucB2rId,prVdpConf->ucMode);
            ptVdpPrm->ucMode = prVdpConf->ucMode;


        }

        if (u4Event & VDP_EVENT_OUTPUT_CHG)
        {
#ifdef CC_SCPOS_EN
            u4Event &= ~VDP_EVENT_OUTPUT_CHG;
#else

            u4Event &= ~VDP_EVENT_OUTPUT_CHG;

            ptVdpPrm->ucOutFrameRate = prVdpConf->rOutInfo.ucFrameRate;
            ptVdpPrm->ucProgressiveOut = prVdpConf->rOutInfo.ucPrg;

            if (prVdpConf->rOutInfo.ucFrameRate)
            {
                ptVdpPrm->u4OutStcPeriod = VDP_STC_CLOCK / prVdpConf->rOutInfo.ucFrameRate;
            }
            else
            {
               ptVdpPrm->u4OutStcPeriod = (VDP_STC_CLOCK / 60);
            }

            LOG(1, "Output change (%d, %d, %d)\n", prVdpConf->rOutInfo.ucPmxId, prVdpConf->rOutInfo.ucFrameRate, prVdpConf->rOutInfo.ucPrg);
#endif
        }
    }

    return 0;
}

//-----------------------------------------------------------------------------
/** Brief of _VDP_HalGetB2rStc
 */
//-----------------------------------------------------------------------------

void _B2R_UpdateStc(B2R_OBJECT_T *this)
{
    B2R_STC_T *ptStc;
	B2R_VAR_T* prB2rVar;
    UCHAR ucB2rId;

#ifdef ENABLE_MULTIMEDIA
    UINT32 u4Type;
#endif

    if(!this)
    {
        LOG(0, "ucB2rId = %d [%s-%d]\n", this->ucB2rId, __FUNCTION__, __LINE__);
        return;
    }

    ucB2rId  = this->ucB2rId;
    prB2rVar = this->ptB2rVar;
    ptStc    = &prB2rVar->rB2rStc;

#ifdef ENABLE_MULTIMEDIA
    u4Type = _B2R_GetContainerType(this);
    if(!prB2rVar->rVdpSeek.fgABReach && ((u4Type != SWDMX_SRC_TYPE_NETWORK_SKYPE) || (u4Type != SWDMX_SRC_TYPE_HW_DIRECT_PLAY)))
#else
	if(!prB2rVar->rVdpSeek.fgABReach)
#endif
    {

        /* Check whether b2r update stc or not */

        UCHAR ucAvSyncMode = 0xff;
        UCHAR ucStcSrc;
        UCHAR ucFbg;

        ucFbg = _B2R_GetFbg(ucB2rId);

        FBM_GetSyncStc(ucFbg, &ucAvSyncMode, &ucStcSrc);
        if (STC_GetSyncMode(ucStcSrc) == AV_SYNC_MODE_VIDEO_MASTER && STC_IsStartedStc(ucStcSrc))
        {
            ptStc->fgEnable = TRUE;  // Video update STC if video master and stc started
        }
        else
        {
            ptStc->fgEnable = FALSE;
        }

        if ( (VID_SYNC_MODE_NONE == ucAvSyncMode) && _B2R_Is4To1024XSpeed(ptStc->u4Speed))
        {
            VDP_SetB2rStc(ucB2rId, _B2R_GetDispingPts(this));
            STC_SetStcValue(ucStcSrc, VDP_GetB2rStc(ucB2rId));

            LOG(5," Pts update STC(0x%x)\n", VDP_GetB2rStc(ucB2rId));
        }
        else if ((ptStc->fgEnable) && (ucStcSrc < STC_SRC_V2))
        {
            UINT32 u4Increment;
            UINT32 u4Diff;

            ptStc->u4B2rStcVal = STC_GetStcValue(ucStcSrc);

            if (ptStc->fgDiv10)
            {
                u4Increment = ((ptStc->u4VSyncPeriod * ptStc->u4Speed) >> 10) / 10;
            }
            else
            {
                u4Increment = (ptStc->u4VSyncPeriod * ptStc->u4Speed) >> 10;
            }

            u4Diff = (0xffffffff - ptStc->u4B2rStcVal);

            if (u4Diff > u4Increment)
            {
                ptStc->u4B2rStcVal += u4Increment;
            }
            else
            {
                ptStc->u4B2rStcVal = (u4Increment - u4Diff);
            }
            LOG(5, "Speed update STC (0x%x)\n", ptStc->u4B2rStcVal);
            STC_SetStcValue(ucStcSrc, ptStc->u4B2rStcVal);
        }
        else
        {
            ptStc->u4B2rStcVal = 0;
        }
    }
}



//-----------------------------------------------------------------------------
/** Brief of _B2R_FrcResetPort.
 */
//-----------------------------------------------------------------------------
void _B2R_FrcResetPort(UCHAR ucB2rId, UCHAR ucPort)
{
    B2R_PRM_T rVdpPrm;
    B2R_PRM_T *  ptVdpPrm;
    UNUSED(ucPort);

    if (ucB2rId >= B2R_NS)
    {
        LOG(0, "ucB2rId = %d [%s-%d]\n", ucB2rId, __FUNCTION__, __LINE__);
        return;
    }
     ptVdpPrm =&(_arVdpPrm[ucB2rId]);
    // backup those we don't want to reset
    // rVdpPrm = _arVdpPrm[ucB2rId];  //TBD
    x_memcpy(&rVdpPrm, ptVdpPrm, sizeof(B2R_PRM_T));

    _B2R_FrmInit(ucB2rId, 0, FALSE);

    // restore
    ptVdpPrm->ucFbgId = rVdpPrm.ucFbgId;
    ptVdpPrm->ucOutFrameRate = rVdpPrm.ucOutFrameRate;
    ptVdpPrm->u4OutStcPeriod = rVdpPrm.u4OutStcPeriod;
    ptVdpPrm->ucProgressiveOut = rVdpPrm.ucProgressiveOut;

    ptVdpPrm->prFbCounter = rVdpPrm.prFbCounter;

    //ptVdpPrm->u4CbPtsEn = rVdpPrm.u4CbPtsEn;
    //ptVdpPrm->u4CbPts = rVdpPrm.u4CbPts;
    //ptVdpPrm->u4CbArg = rVdpPrm.u4CbArg;

    //ptVdpPrm->u4CbSyncEn = rVdpPrm.u4CbSyncEn;
    //ptVdpPrm->u4CbSyncThrsd= rVdpPrm.u4CbSyncThrsd;
    //ptVdpPrm->u4CbSyncFrmCnt= rVdpPrm.u4CbSyncFrmCnt;


    // clean FBM_FLAG_DISPLAYING at next VSYNC
    FBM_ClrFrameBufferFlag(rVdpPrm.ucFbgId, FBM_FLAG_DISPLAYING);
}

void _B2R_FrcSetPtsCb(UINT32 u4EsId, UINT32 u4CbPts)
{
    UINT32 u4VdpId;
    UCHAR  ucB2rId = B2R_NS;
    UINT32 u4PortIdx;
    VDP_CFG_T* prVdpConf = NULL;

    for(u4VdpId = 0; u4VdpId < VDP_NS; u4VdpId++)
    {
        for(u4PortIdx = 0; u4PortIdx < VDP_MAX_INPORT_NS; u4PortIdx++)
{
            prVdpConf = _B2R_GetVdpConf(u4VdpId);
            if(prVdpConf &&
                prVdpConf->ucInputPort[u4PortIdx] == u4EsId)
            {
                ucB2rId = prVdpConf->ucB2rId;
                break;
            }
        }
    }
    
    if (ucB2rId < B2R_NS)
    {
        B2R_VAR_T *prB2rVar;
        B2R_PRM_T *ptVdpPrm;
        prB2rVar = &_arB2rVar[ucB2rId];
        ptVdpPrm = &_arVdpPrm[ucB2rId];
        prB2rVar->u4CbPtsEn = 1;
        prB2rVar->u4CbPts = u4CbPts;
        prB2rVar->u4CbArg = u4EsId;
        prB2rVar->u4CbCntDown = 0;
        prB2rVar->u4CbLastPts = ptVdpPrm->u4Pts;
    }
    else
    {
        LOG(0, "ucB2rId = %d [%s-%d]\n", ucB2rId, __FUNCTION__, __LINE__);
    }
}


void _B2R_ResetPts(UCHAR ucB2rId)
{
    B2R_PRM_T *  ptVdpPrm= &_arVdpPrm[ucB2rId];

    if (ucB2rId >= B2R_NS)
    {
        LOG(0, "ucB2rId = %d [%s-%d]\n", ucB2rId, __FUNCTION__, __LINE__);
        return;
    }
    ptVdpPrm->u4LastPts = 0;
}

//-----------------------------------------------------------------------------
/** Brief of _B2R_FrcSetLipSyncNGOKCb.
 */
//-----------------------------------------------------------------------------
void _B2R_FrcSetLipSyncNGOKCb(UCHAR ucNgOk, UCHAR ucRemove, UINT32 u4Thrsd, UINT32 u4FrmCnt)
{
    if (ucRemove == 0)
    {
        if (ucNgOk == VDP_LIPSYNC_REG_NG_TO_OK)
        {
            _rVdpLipSync.ucNg2OkCbEn = 1;
            _rVdpLipSync.u4OKThrd = (u4Thrsd*90);
            _rVdpLipSync.u4NumFrmsOK = u4FrmCnt;
            _rVdpLipSync.u4OKFrmCnt = 0;
            _rVdpLipSync.ucLipSyncState = VDP_LIP_SYNC_NG;

            LOG(9, "Lip Sync NG to OK Thrd(%d), FrmCnt(%d)\n", u4Thrsd, u4FrmCnt);
        }
        else if (ucNgOk == VDP_LIPSYNC_REG_OK_TO_NG)
        {
            _rVdpLipSync.ucOk2NgCbEn = 1;
            _rVdpLipSync.u4NGThrd = (u4Thrsd*90);
            _rVdpLipSync.u4NumFrmsNG = u4FrmCnt;
            _rVdpLipSync.u4NGFrmCnt = 0;
            _rVdpLipSync.ucLipSyncState = VDP_LIP_SYNC_OK;

            LOG(9, "Lip Sync OK to NG Thrd(%d), FrmCnt(%d)\n", u4Thrsd, u4FrmCnt);
        }
        else
        {
            ASSERT(0);
        }
    }
    else
    {
        if (ucNgOk == VDP_LIPSYNC_REG_NG_TO_OK)
        {
            _rVdpLipSync.ucNg2OkCbEn = 0;
            LOG(9, "Remove Lip Sync NG to OK callback\n", u4Thrsd, u4FrmCnt);
        }
        else if (ucNgOk == VDP_LIPSYNC_REG_OK_TO_NG)
        {
            _rVdpLipSync.ucOk2NgCbEn = 0;
            LOG(9, "Remove Lip Sync OK to NG callback\n", u4Thrsd, u4FrmCnt);
        }
        else
        {
            ASSERT(0);
        }
        UNUSED(_rVdpLipSync);
    }
}


BOOL _B2R_FrcGetUnMuteStatus(UCHAR ucB2rId)
{
    if (ucB2rId < B2R_NS)
    {
        if (_arVdpPrm[ucB2rId].ucReady != 0)
        {
            return TRUE;
        }
    }

    return FALSE;
}

UCHAR _B2R_FrcGetLockFrameBuffer(UCHAR ucB2rId)
{
    UCHAR ucLockFbId;

    if (ucB2rId < B2R_NS)
    {
        B2R_MUTEX_LOCK(ucB2rId);
        ucLockFbId = _arVdpPrm[ucB2rId].ucFbId;
        B2R_MUTEX_UNLOCK(ucB2rId);
    }
    else
    {
        ucLockFbId = FBM_FB_ID_UNKNOWN;
    }
    return ucLockFbId;
}


VOID _B2R_SetCurrentTick(UCHAR ucB2rId, UINT32 u4Tick)
{
    FBM_PIC_HDR_T* prLockPicHdr;
    UCHAR ucFbgId;
    UCHAR ucLockFbId;
    if (ucB2rId >= B2R_NS)
    {
        LOG(0, "ucB2rId = %d [%s-%d]\n", ucB2rId, __FUNCTION__, __LINE__);
        return;
    }

    ucFbgId = _arVdpPrm[ucB2rId].ucFbgId;
    ucLockFbId = _arVdpPrm[ucB2rId].ucFbId;

    if ((ucLockFbId != FBM_FB_ID_UNKNOWN) && (ucFbgId  != FBM_FBG_ID_UNKNOWN))
    {
        prLockPicHdr = FBM_GetFrameBufferPicHdr(ucFbgId, ucLockFbId);

        if (prLockPicHdr != NULL)
        {
            prLockPicHdr->u4TickNum = u4Tick;
        }
    }
}

UINT32 _B2R_GetCurrentPts(UCHAR ucB2rId)
{
    FBM_PIC_HDR_T* prLockPicHdr;
    UCHAR ucFbgId;
    UCHAR ucLockFbId;

    if (ucB2rId < B2R_NS)
    {
        B2R_MUTEX_LOCK(ucB2rId);
        
        ucFbgId = _arVdpPrm[ucB2rId].ucFbgId;
        ucLockFbId = _arVdpPrm[ucB2rId].ucFbId;

        B2R_MUTEX_UNLOCK(ucB2rId);
        
        if ((ucLockFbId != FBM_FB_ID_UNKNOWN) && (ucFbgId  != FBM_FBG_ID_UNKNOWN))
        {
            prLockPicHdr = FBM_GetFrameBufferPicHdr(ucFbgId, ucLockFbId);

            if (prLockPicHdr != NULL)
            {
                return prLockPicHdr->u4PTS;
            }
        }
    }
    return 0;
}

UINT32 _B2R_GetLastPts(UCHAR ucB2rId)
{
    return (ucB2rId < B2R_NS )? _arVdpPrm[ucB2rId].u4LastPts : 0;
}

UCHAR _B2R_GetOutFrameRate(UCHAR ucB2rId)
{
    return (ucB2rId < B2R_NS )? _arVdpPrm[ucB2rId].ucOutFrameRate : 0;
}

UCHAR _B2R_GetFbg(UCHAR ucB2rId)
{
    return (ucB2rId < B2R_NS )? _arVdpPrm[ucB2rId].ucFbgId : FBM_FBG_ID_UNKNOWN;
}

UINT32 _B2R_GetFilePosition(UCHAR ucB2rId,
                            INT32 *pi4TempRef, UINT16 *pu2DecodingOrder,
                            UINT64 *pu8Offset, UINT64 *pu8OffsetI)
{
    FBM_PIC_HDR_T* prLockPicHdr;
    UCHAR ucFbgId;
    UCHAR ucLockFbId;

    B2R_MUTEX_LOCK(ucB2rId);
    
    ucFbgId = _arVdpPrm[ucB2rId].ucFbgId;
    ucLockFbId = _arVdpPrm[ucB2rId].ucFbId;

    B2R_MUTEX_UNLOCK(ucB2rId);
    
    if ((ucLockFbId != FBM_FB_ID_UNKNOWN) && (ucFbgId  != FBM_FBG_ID_UNKNOWN))
    {
        prLockPicHdr = FBM_GetFrameBufferPicHdr(ucFbgId, ucLockFbId);
        if (prLockPicHdr != NULL)
        {
            *pi4TempRef = prLockPicHdr->i4TemporalRef;
            *pu2DecodingOrder = prLockPicHdr->u2DecodingOrder;
            *pu8Offset = prLockPicHdr->u8Offset;
            *pu8OffsetI = prLockPicHdr->u8OffsetI;
            return VDP_SET_OK;
        }
        else
        {
            *pi4TempRef = 0;
            *pu2DecodingOrder = 0;
            *pu8Offset = 0;
            *pu8OffsetI = 0;
        }
    }
    else
    {
        *pi4TempRef = 0;
        *pu2DecodingOrder = 0;
        *pu8Offset = 0;
        *pu8OffsetI = 0;
    }
    return VDP_SET_ERROR;
}


UINT32 _B2R_GetPosInfo(UCHAR ucB2rId, VDP_POS_INTO_T *prPosInfo)
{
    FBM_PIC_HDR_T* prLockPicHdr;
    UCHAR ucFbgId;
    UCHAR ucLockFbId;
    B2R_VAR_T* prB2rVar ;

    if (!prPosInfo)
    {
        return VDP_SET_ERROR;
    }
    if (ucB2rId >= B2R_NS)
    {
        return VDP_SET_ERROR;
    }

    x_memset(prPosInfo, 0, sizeof(VDP_POS_INTO_T));

    B2R_MUTEX_LOCK(ucB2rId);
    
    prB2rVar = &_arB2rVar[ucB2rId];
    ucFbgId = _arVdpPrm[ucB2rId].ucFbgId;
    ucLockFbId = _arVdpPrm[ucB2rId].ucFbId;

    B2R_MUTEX_UNLOCK(ucB2rId);
    
    if ((ucLockFbId != FBM_FB_ID_UNKNOWN) && (ucFbgId  != FBM_FBG_ID_UNKNOWN))
    {
        prLockPicHdr = FBM_GetFrameBufferPicHdr(ucFbgId, ucLockFbId);
        if (prLockPicHdr != NULL)
        {
            prPosInfo->i4TemporalRef = prLockPicHdr->i4TemporalRef;
            prPosInfo->u2DecodingOrder = prLockPicHdr->u2DecodingOrder;
            prPosInfo->u8Stc    = (UINT64)_B2R_FrcGetStc(ucFbgId);
            prPosInfo->u8Offset = prLockPicHdr->u8Offset;
            prPosInfo->u8OffsetI = prLockPicHdr->u8OffsetI;
            prPosInfo->u8OffsetDisp = prLockPicHdr->u8OffsetDisp;
            prPosInfo->u8Pts = (UINT64)prLockPicHdr->u4PTS;
            prPosInfo->u8PtsI = prLockPicHdr->rExtra.u8PTSI;
            prPosInfo->u8AudPts = prLockPicHdr->rExtra.u8AudPTS;
            prPosInfo->u8AudOffset = prLockPicHdr->rExtra.u8AudOffset;
            prPosInfo->u4Timestap = prLockPicHdr->u4Timestap;

            //DTV00316878 for avi file same PTS value when AB repeat in trick mode
            if (((prB2rVar->eSpeed >= STC_SPEED_TYPE_FORWARD_4X) &&
                    (prB2rVar->eSpeed <= STC_SPEED_TYPE_FORWARD_1024X))||
                    ((prB2rVar->eSpeed >= STC_SPEED_TYPE_REWIND_1X) &&
                     (prB2rVar->eSpeed <= STC_SPEED_TYPE_REWIND_1024X)))
            {
                prPosInfo->u8Stc  = prPosInfo->u8Pts;
            }
            LOG(0,"prPosInfo->u8Stc = %llu\n", prPosInfo->u8Stc);
            return VDP_SET_OK;
        }
    }
    return VDP_SET_ERROR;
}


UCHAR _B2R_GetPendingFB(UCHAR ucB2rId, BOOL fgSubFbg)
{
    if (ucB2rId < B2R_NS)
    {
        if (!fgSubFbg)
        {
            return _arVdpPrm[ucB2rId].ucPendingFbId;
        }
        else
        {
#ifdef CC_3D_MM_DS_SUPPORT
            return _arVdpPrm[ucB2rId].ucSubPendingFbId;
#else
            return FBM_FB_ID_UNKNOWN;
#endif
        }
    }
    else
    {
        return FBM_FB_ID_UNKNOWN;
    }
}

void _B2R_CleanPendingFB(UCHAR ucB2rId, BOOL fgSubFbg)
{
    if (ucB2rId < B2R_NS)
    {
        _arVdpPrm[ucB2rId].fgReleaseDispQ = TRUE;
        if (!fgSubFbg)
        {
            _arVdpPrm[ucB2rId].ucPendingFbId = FBM_FB_ID_UNKNOWN;
        }
#ifdef CC_3D_MM_DS_SUPPORT
        else
        {
           _arVdpPrm[ucB2rId].ucSubPendingFbId = FBM_FB_ID_UNKNOWN;
        }
#endif
    }
}

void _B2R_SetReleaseDispQ(UCHAR ucB2rId, BOOL fgSubFbg)
{
    UNUSED(fgSubFbg);
    if (ucB2rId < B2R_NS)
    {
      _arVdpPrm[ucB2rId].fgReleaseDispQ = TRUE;
    }
}

UINT32 _B2R_GetDispBufLocked(UCHAR ucB2rId, UINT32 u4BufAddr, BOOL *pfgLocked)
{
    return 1;
}



VOID _B2R_FrcStcSpeed(UCHAR ucB2rId,STC_SPEED_TYPE_T eSpeed)
{
	
   B2R_VAR_T* prB2rVar = &_arB2rVar[ucB2rId];
    _arVdpPrm[ucB2rId].u4B2rSkipFrameNo = 0;
    if (ucB2rId < B2R_NS)
    {
        prB2rVar->eSpeed = eSpeed;
    }
    else
    {
        LOG(0, "u1B2rId = %d [%s-%d]\n", ucB2rId, __FUNCTION__, __LINE__);
    }
}
VOID _B2R_SetB2rNoLock(UCHAR ucB2rId, BOOL fgNoLock)
{
    if (ucB2rId < B2R_NS)
    {
        _arVdpPrm[ucB2rId].fgB2RNoLock = fgNoLock;
    }
}

//#ifdef CC_B2R_RM_SUPPORT
VOID B2R_VB1_ENABLE_CLI( BOOL fgEnable)
{
    fgVB1Byass=fgEnable;
    LOG(1,"B2R_VB1_ENABLE_CLI=%d\n",fgVB1Byass);
}

VOID B2R_VB1_Setting(void)
{
    B2R_OBJECT_T *this;
    B2R_HAL_TIMING_T tTiming;
    __TIMING_TBL_T *ptTbl;
    FBM_SEQ_HDR_T* prSeqHdr;
    B2R_PRM_T *  prFrcPrm;
    BOOL fgProg;
    B2R_HANDLE_T hB2r;
    B2R_HAL_DRAM_LAYOUT_T tDramLayout;
    ptTbl=VB1TimingTab;
    this= _B2R_GetObj(B2R_3);
    if(!this)
    {
        return;
    }
    
    prFrcPrm  = this->ptB2rPrm;
    prSeqHdr = FBM_GetFrameBufferSeqHdr(prFrcPrm->ucFbgId);
    if(!prSeqHdr)
    {
        return;
    }
    
    fgProg=_B2R_IsProg(this, prSeqHdr);
    hB2r=this->hB2r;
    B2R_HAL_Reset(hB2r);
    tTiming.fgProg   = fgProg ? 1: 0;
    tTiming.u4HTotal = ptTbl->u2HTotal;
    tTiming.u4VTotal = ptTbl->u2VTotal;
    tTiming.u4Rate   = 60;
    tTiming.u4Scale  = 1;
    tTiming.u4SrcW   = prSeqHdr->u2HSize;
    tTiming.u4SrcH   = tTiming.fgProg? prSeqHdr->u2VSize : (prSeqHdr->u2VSize << 1);    
    tTiming.u4Clock  = ptTbl->u4Clock; 
    B2R_HAL_Set(hB2r, B2R_HAL_TIMING, &tTiming);
   
    {
   
    tDramLayout.eAddrMode = prSeqHdr->fgRasterOrder ? ADDR_MODE_RASTER : (prSeqHdr->ucBlockType ? B2R_AM_BLOCK1 : B2R_AM_BLOCK4);;
    tDramLayout.eSwapMode = _B2R_ConSwapMd(prSeqHdr->eSwapMode);
    tDramLayout.ePackMode = _B2R_ConBlkMd(prSeqHdr->ePackMode);
    tDramLayout.u4Pitch   = prSeqHdr->u2LineSize;
    B2R_HAL_Set(hB2r, B2R_HAL_DRAM_LAYOUT, &tDramLayout);
    LOG(1,"eAddrMode : %d , eSwapMode : %d , ePackMode : %d!\n",
       tDramLayout.eAddrMode,tDramLayout.eSwapMode,tDramLayout.ePackMode);
    }

    //color format
    {
    B2R_HAL_COLOR_FMT_T tColorFmt;
    tColorFmt.eMode   = prSeqHdr->fg422Mode ? B2R_CM_422 : B2R_CM_420;
    tColorFmt.u1Depth = prSeqHdr->u1Depth ? prSeqHdr->u1Depth : 8;
    tColorFmt.fgUfoEn = prSeqHdr->fgUfoEn;
    B2R_HAL_Set(hB2r, B2R_HAL_COLOR_FMT, &tColorFmt);
    LOG(1,"u1Depth : %d!\n",tColorFmt.u1Depth);
    }

    {
    
    BOOL fgByass=TRUE;
    B2R_HAL_Set(hB2r,B2R_HAL_VB1,&fgByass);
    LOG(1,"Omux  Set Bypass enable : %d!\n",fgByass);
    
    }
     B2R_HAL_Config(hB2r);
  //  VDP_SetBypassHFph(2035);//0x7f3
  //  VDP_SetBypassHBph(2079);//0x81fss
  //  VDP_SetBypassVFph(2176);//0x880
   // VDP_SetBypassVBph(2186);//0x88A
   #if defined(CC_MT5890)
    VDP_SetBypassHVInvert(1,1);
    VDP_SetBypassHVInvert(0,1);
    #endif
  //  _B2R_HW_Select(2);//New B2R HD 
//    B2R_HAL_Enable(hB2r);
   Printf("tTiming.u4SrcW=%d,tTiming.u4SrcH =%d,fgProg=%d,prSeqHdr->ucBlockType=%d\n",tTiming.u4SrcW,tTiming.u4SrcH,fgProg,prSeqHdr->ucBlockType);
}
//#endif

VOID _B2R_Set3DInfo(UCHAR ucB2rId,UINT8 u13DType,BOOL fg3DEnable)
{
   _arVdpPrm[ucB2rId].fgB2R3DEnable = fg3DEnable;
   _arVdpPrm[ucB2rId].u1B2R3DType = u13DType;
}

BOOL _B2R_IsStartToPlay(UCHAR ucB2rId)
{
    B2R_VAR_T* prB2rVar = &_arB2rVar[ucB2rId];
    if (ucB2rId >= B2R_NS)
    {
        return TRUE;
    }
#ifdef CC_B2R_WAIT_NPTV_STABLE
    if (_arVdpPrm[ucB2rId].ucFbgId != 0xFF)
    {
        B2R_PRM_T* prFrcPrm = &(_arVdpPrm[ucB2rId]);//TBD
        FBM_GetPlayMode(_arVdpPrm[ucB2rId].ucFbgId,&(prB2rVar->u1PlayMode));

        if (prFrcPrm != NULL)
        {
            UCHAR ucVdpId = _B2R_GetVdpId(ucB2rId);
            if ((prB2rVar->u1PlayMode) == FBM_FBG_MM_MODE && !(prB2rVar->fgNPTVStable) && !prFrcPrm->fgB2R3DEnable && (prFrcPrm->ucReady ==1) && _u4DrvVideoGetMute(ucVdpId))
            {
                return FALSE;
            }
            else
            {
                return TRUE;
            }
        }
        else
        {
            return TRUE;
        }
    }
    else
    {
        return TRUE;
    }
#else
    return TRUE;
#endif
}


UINT32 _B2R_TrickModeSkipFrameNo(UCHAR ucB2rId)
{
    return _arVdpPrm[ucB2rId].u4B2rSkipFrameNo;
}

INT32 _B2R_GetPtsStcDelta(UCHAR ucB2rId)
{
    return _arVdpPrm[ucB2rId].i4PtsDrift;
}


UINT8 _B2R_HalGetUnderFlowFlag(UCHAR ucB2rId)
{
    if(_prDbase)
    {
        if(_prDbase->rVdp.au4UnderFlow[ucB2rId] ==0)
        {
            return 0;
        }
        else
        {
            return 1;
        }
    }
    else
    {
        return 0;
    }
}

VOID _B2R_HalSetUnderFlow(UCHAR ucB2rId, UINT32 u4Val)
{
    if(_prDbase)
    {
        _prDbase->rVdp.au4UnderFlow[ucB2rId] = u4Val;
    }
}

VOID VDP_HalIncreaseUnderFlow(UCHAR ucB2rId, UINT32 u4Step)
{
    if(_prDbase)
    {
        _prDbase->rVdp.au4UnderFlow[ucB2rId] += u4Step;
    }
}

VOID _B2R_HalSetStartLineCnt(UCHAR ucB2rId, UINT32 u4Value)
{
    if(_prDbase)
    {
        _prDbase->rVdp.au4StartLineCnt[ucB2rId] = u4Value;
    }
}

VOID VDP_HalSetStopLineCnt(UCHAR ucB2rId, UINT32 u4Value)
{
    if(_prDbase)
    {
        _prDbase->rVdp.au4StopLineCnt[ucB2rId] = u4Value;
    }
}

VOID _B2R_HalIncreaseTearing(UCHAR ucB2rId, UINT32 u4Step)
{
    if(_prDbase)
    {
        _prDbase->rVdp.au4Tearing[ucB2rId] += u4Step;
    }
}



//-----------------------------------------------------------------------------
/** Brief of VDP_HalGetB2RFifoStatus
 */
//-----------------------------------------------------------------------------
UINT32 VDP_HalGetB2RFifoStatus(UCHAR ucB2rId)
{
    UINT32 u4Idx = 0;
    if (ucB2rId >= B2R_NS)
    {
        ASSERT(ucB2rId < B2R_NS);
        return 0;
    }

    if (_prDbase)
    {
        u4Idx = sizeof(_prDbase->rVdp.au4UnderFlow)/
            sizeof(_prDbase->rVdp.au4UnderFlow[0]);
        if(ucB2rId < u4Idx)
        {
            return _prDbase->rVdp.au4UnderFlow[ucB2rId];
        }
    }
    return 0;
}

//-----------------------------------------------------------------------------
/** Brief of VDP_HalGetB2RFifoStatus
 */
//-----------------------------------------------------------------------------
void _B2R_HalResetB2RFifoStatus(UCHAR ucB2rId)
{
    UINT32 u4Idx = 0;
    if (ucB2rId >= B2R_NS)
    {
        ASSERT(ucB2rId < B2R_NS);
        return;
    }

    if (_prDbase)
    {
        u4Idx = sizeof(_prDbase->rVdp.au4UnderFlow)/
            sizeof(_prDbase->rVdp.au4UnderFlow[0]);
        if(ucB2rId < u4Idx)
        {
            _prDbase->rVdp.au4UnderFlow[ucB2rId] = 0;
        }
    }
    return;
}

B2R_VAR_T* _B2R_GetB2rVar(UCHAR ucB2rId)
{
    return (ucB2rId >= B2R_NS)? NULL : &_arB2rVar[ucB2rId];
}

B2R_PRM_T* _B2R_GetB2rPrm(UCHAR ucB2rId)
{
    return (ucB2rId >= B2R_NS)? NULL : &_arVdpPrm[ucB2rId];
}

UINT32 _B2R_GetFrcEsmQCnt()
{
    return _prDbase->rMpv[0].u4EsmQCnt;
}


BOOL _VDP_RTNeedSuperGameMode(UCHAR ucVdpId)
{
    #ifdef ENABLE_MULTIMEDIA
    UCHAR ucEsId;
    VDEC_ES_INFO_T *prVdecEsInfo = NULL;
    VDP_CFG_T* prVdpConf;
    
    if(ucVdpId >= VDP_NS)
    {
        return FALSE;
    }

    prVdpConf = _B2R_GetVdpConf(ucVdpId);
    if(!prVdpConf)
    {
        return FALSE;
    }

    ucEsId = prVdpConf->ucInputPort[0];
    prVdecEsInfo = (ucEsId < VDEC_MAX_ES)? _VDEC_GetEsInfo(ucEsId) : NULL;
    if (prVdecEsInfo && 
        (prVdecEsInfo->eMMSrcType == SWDMX_SRC_TYPE_WIFI_DISPLAY))
    {
        return TRUE;
    }
    #else
    UNUSED(ucVdpId);
    #endif
    return FALSE;
}

#ifdef ENABLE_MULTIMEDIA
/*-----------------------------------------------------------------------------
* Name: _B2R_RTCliActive
*
* Description: This API For Disable Dynamic Adjust Latency.
*              
* Inputs:  ucB2rId    Specifies the B2R Id of the B2R PRM. Different B2R PRM 
*                       refer to different B2R Id.
*
*             ucEnable   Specifies Enable/Disable Dynamic CMD
*
* Outputs: 
*
* Function: CLI for Disable Dynamic Latency.
----------------------------------------------------------------------------*/
VOID _B2R_RTCliActive(UCHAR ucB2rId,  UCHAR ucEnable)
{
    B2R_PRM_T*     prFrcPrm = NULL;
    
    do
    {
        if(ucB2rId >= B2R_NS)
        {
            LOG(0,"%s,Invalid B2rId!\n",__FUNCTION__);
            break;
        }

        prFrcPrm = &_arVdpPrm[ucB2rId];
        if(!prFrcPrm)
        {
            LOG(0,"%s,Invalid VdpPrm!\n",__FUNCTION__);
            break;
        }
        
        if(ucEnable)
        {
            if(prFrcPrm->ptRtCli)
            {
                LOG(0,"%s,RT CLI Already Active!\n",__FUNCTION__);
                break;
            }

            prFrcPrm->ptRtCli =
                (VOID*)x_mem_alloc(sizeof(B2R_RT_CLI_T));
            if(!prFrcPrm->ptRtCli)
            {
                LOG(0,"%s,RT CLI memmory alloc Failed!\n",__FUNCTION__);
                break;
            }
            prFrcPrm->ptRtCli->u4_latency_ns = 1;
            LOG(0,"B2R (%d), RT CLI Active Success!\n",ucB2rId);
        }
        else
        {
            if(!prFrcPrm->ptRtCli)
            {
                LOG(0,"B2R (%d), RT CLI Already Deactive!\n",ucB2rId);
                break;
            }

            x_mem_free(prFrcPrm->ptRtCli);
            LOG(0,"B2R (%d), RT CLI Deactive Success!\n",ucB2rId);
        }
    }while(0);
}

/*-----------------------------------------------------------------------------
* Name: _B2R_RTCliActive
*
* Description: This API Set Static Latency to WiFi Display Test.
*              
* Inputs:  ucB2rId    Specifies the B2R Id of the B2R PRM. Different B2R PRM 
*                       refer to different B2R Id.
*
*             u4Latency  Latency Number , 1 Latency Number = 16 ms
*
* Outputs: 
*
* Function: CLI for Set Static Latency , Should Disable Dynamic First.
----------------------------------------------------------------------------*/
VOID _B2R_SetLatency(UCHAR ucB2rId,  UINT32 u4Latency)
{
    B2R_PRM_T*     prFrcPrm = NULL;
    
    if(ucB2rId >= B2R_NS)
    {
        LOG(0,"%s,Invalid B2rId!\n",__FUNCTION__);
        return;
    }

    prFrcPrm = &_arVdpPrm[ucB2rId];
    if(!prFrcPrm)
    {
        LOG(0,"%s,Invalid VdpPrm!\n",__FUNCTION__);
        return;
    }
    
    if(prFrcPrm->ptRtCli)
    {
        prFrcPrm->ptRtCli->u4_latency_ns = u4Latency;
        LOG(0,"Set Static Latency ns : %d!\n",u4Latency);
    }
    else
    {
        LOG(0,"Please active wfd CLI and disable dynamic latency!\n");
    }
}

VOID _B2R_QueryLatency(UCHAR ucB2rId)
{
    B2R_PRM_T*    prFrcPrm = NULL;
    
    if(ucB2rId >= B2R_NS)
    {
        LOG(0,"%s,Invalid B2rId!\n",__FUNCTION__);
        return;
    }

    prFrcPrm = &_arVdpPrm[ucB2rId];
    if(!prFrcPrm ||
        !prFrcPrm->ptRtCli)
    {
        LOG(0,"%s,Invalid VdpPrm!\n",__FUNCTION__);
        return;
    }

    LOG(0,"\nTimeCur(%dms),Aver(%dms),TotalTime(%lldms),FrameCnt(%d)\n",
        prFrcPrm->ptRtCli->u4TimeCur, prFrcPrm->ptRtCli->u4TimeAve,
        prFrcPrm->ptRtCli->u8TimeTotal, prFrcPrm->ptRtCli->u4FrameCnt);
    
}
#endif

static B2R_PACK_MODE_T _B2R_ConBlkMd(FBM_PACK_MODE_T ePackCod)
{
    B2R_PACK_MODE_T ePackMode;
    switch (ePackCod)
    {
        case FBM_PM_NORMAL:
            ePackMode = B2R_PM_NORMAL;
            break;

        case FBM_PM_COMPACT:
            ePackMode = B2R_PM_COMPACT;
            break;
        
        default:
            ePackMode = B2R_PM_NORMAL;
            break;
    }

    return ePackMode;
}

INT32 _B2R_Height_Align(UINT32*  pu4_height)
{
    INT32   i4Ret = B2R_OK;

    do
    {
        if(!pu4_height)
        {
            i4Ret = B2R_INV_ARG;
            break;
        }

        if(!HEIGHT_MUST_ALIGN(*pu4_height))
        {
            i4Ret = B2R_FAIL;
            break;
        }

        switch(*pu4_height)
        {
            case FHD_HEIGHT:
                *pu4_height = FHD_HEIGHT - FHD_ALIGN_VALUE;
                break;

            default :
                i4Ret = B2R_NOT_IMPL;
                break;
        }
    }while(0);

    return i4Ret;
}

static B2R_SWAP_MODE_T _B2R_ConSwapMd(FBM_SWAP_MODE_T eSwapCod)
{
    B2R_SWAP_MODE_T eSwapMode;
    switch (eSwapCod)
    {
        case FBM_SM_NO_SWAP:
            eSwapMode = B2R_SM_NO_SWAP;
            break;
            
        case FBM_SM_SWAP0:
            eSwapMode = B2R_SM_SWAP0;
            break;

        default:
            eSwapMode = B2R_SM_NO_SWAP;
            break;
    }

    return eSwapMode;
}

static BOOL _B2R_IsTimingChg(B2R_OBJECT_T *this, 
                             B2R_HAL_TIMING_T* ptOrgTiming,
                             B2R_HAL_TIMING_T* ptTiming)
{
    BOOL fgDiff = TRUE;
    B2R_HAL_DISP_INFO_T tDispInfo = {0};
    B2R_PRM_T* prFrcPrm = NULL;
    FBM_SEQ_HDR_T* prSeqHdr = NULL;

    do
    {
        if(!this || !ptOrgTiming || !ptTiming)
        {
            break;
        }

        if(ptOrgTiming->fgProg    != ptTiming->fgProg ||
            ptOrgTiming->u4SrcW   != ptTiming->u4SrcW ||
            ptOrgTiming->u4SrcH   != ptTiming->u4SrcH ||
            ptOrgTiming->u4HTotal != ptTiming->u4HTotal ||
            ptOrgTiming->u4VTotal != ptTiming->u4VTotal ||
            ptOrgTiming->u4Rate   != ptTiming->u4Rate ||
            ptOrgTiming->u4Scale  != ptTiming->u4Scale)
        {
            LOG(1, "older info: fgprog(%d),u4SrcW(%d),u4SrcH(%d),u4HTotal(%d),u4VTotal(%d),u4Rate(%d),u4Scale(%d)!\n",
                ptOrgTiming->fgProg , ptOrgTiming->u4SrcW,ptOrgTiming->u4SrcH,ptOrgTiming->u4HTotal,
                ptOrgTiming->u4VTotal,ptOrgTiming->u4Rate,ptOrgTiming->u4Scale);

            LOG(1, "new info: fgprog(%d),u4SrcW(%d),u4SrcH(%d),u4HTotal(%d),u4VTotal(%d),u4Rate(%d),u4Scale(%d)!\n",
                ptTiming->fgProg , ptTiming->u4SrcW,ptTiming->u4SrcH,ptTiming->u4HTotal,
                ptTiming->u4VTotal,ptTiming->u4Rate,ptTiming->u4Scale);
            
            break;
        }

        // check pitch change
        prFrcPrm = this->ptB2rPrm;
        if(!prFrcPrm)
        {
            LOG(0,"prFrcPrm == NULL\n");
            break;
        }
        
        prSeqHdr = FBM_GetFrameBufferSeqHdr(prFrcPrm->ucFbgId);
        if (!prSeqHdr)
        {
            LOG(0,"prSeqHdr == NULL with fbgid(%d)\n",prFrcPrm->ucFbgId);
            break;
        }
        
        B2R_HAL_Get(this->hB2r, B2R_HAL_DISP_INFO, &tDispInfo);
        if (prSeqHdr->u2LineSize != (UINT16)tDispInfo.u4Pitch)
        {
            LOG(1, "Cur pitch : %d, New pitch : %d!\n",
                tDispInfo.u4Pitch, prSeqHdr->u2LineSize);
            break;
        }

        fgDiff = FALSE;
    }while(0);

    return fgDiff;
}

#ifdef ENABLE_MULTIMEDIA
static INT32 _B2R_SeekErrHandle(B2R_OBJECT_T *this)
{
    UCHAR ucSeekMode = 0;
    UCHAR ucSeekNotifyTrigger = 0;
    UINT8 u1DecoderSrcId = 0;
    INT32 i4Ret = B2R_OK;
    UINT32 u4SeekPts = 0;
    B2R_VAR_T* prB2rVar = NULL;
    B2R_PRM_T* prFrcPrm = NULL;
    VDEC_ES_INFO_T* prVdecEsInfo = NULL;
       
    do
    {
        if(!this)
        {
            i4Ret = B2R_INV_ARG;
            break;
        }
        
        prB2rVar = this->ptB2rVar;
        prFrcPrm = this->ptB2rPrm;
        if(!prB2rVar || !prFrcPrm)
        {
            i4Ret = B2R_INV_ARG;
            break;
        }
        
        _B2R_GetSeek(this, &ucSeekMode, &u4SeekPts, &ucSeekNotifyTrigger);
        if(!ucSeekMode && !ucSeekNotifyTrigger &&
            !prB2rVar->ucRemainTimeToSendSeekDone)
        {
            prB2rVar->rVdpSeek.u1TimeoutCnt = 0;
            break;
        }

        u1DecoderSrcId = FBM_GetDecoderSrcId(prFrcPrm->ucFbgId);
        prVdecEsInfo = u1DecoderSrcId < VDEC_MAX_ES ? _VDEC_GetEsInfo(u1DecoderSrcId) : NULL;
        if(!prVdecEsInfo ||
            (prVdecEsInfo && prVdecEsInfo->eMMSrcType != SWDMX_SRC_TYPE_HIGH_SPEED_STORAGE))
        {
            i4Ret = B2R_NOT_IMPL;
            break;
        }
        
        prB2rVar->rVdpSeek.u1TimeoutCnt ++;

        if(prB2rVar->rVdpSeek.u1TimeoutCnt > B2R_SEEK_TIMEOUT_CNT)
        { 
            _B2R_SeekFinishNtfy(this, FALSE);
            prB2rVar->rVdpSeek.u1TimeoutCnt = 0;
        }
    }while(0);

    return i4Ret;
}

static INT32 _B2R_RT_TMS_Statistics(B2R_OBJECT_T* this)
{
    INT32  i4Ret = B2R_OK;
    UINT64 u8Remainder = 0;
    B2R_PRM_T*     prFrcPrm = NULL;
    FBM_PIC_HDR_T* prPicHdr = NULL;

    do
    {
        if(!this)
        {
            i4Ret = B2R_INV_ARG;
            break;
        }

        prFrcPrm = this->ptB2rPrm;
        if(!prFrcPrm)
        {
            i4Ret = B2R_INV_ARG;
            break;
        }
        
        if(_B2R_GetContentSourceType(this) != (UINT32)SWDMX_SRC_TYPE_WIFI_DISPLAY ||
            !prFrcPrm->ptRtCli)
        {
            break;
        }
        
        prPicHdr = FBM_GetFrameBufferPicHdr(prFrcPrm->ucFbgId, prFrcPrm->ucFbId);
        if(prPicHdr && prPicHdr->fgWFDTMSLogEnable)
        {
            HAL_TIME_T  rTimeCur, rTimeDelta;
            
            HAL_GetTime(&rTimeCur);
            HAL_GetDeltaTime(&rTimeDelta, &prPicHdr->rTimePutDispQ, &rTimeCur);

            prFrcPrm->ptRtCli->u4TimeCur = rTimeDelta.u4Seconds*1000 + rTimeDelta.u4Micros/1000;
            prFrcPrm->ptRtCli->u8TimeTotal += prFrcPrm->ptRtCli->u4TimeCur;
            prFrcPrm->ptRtCli->u4TimeAve =
                u8Div6432(prFrcPrm->ptRtCli->u8TimeTotal, (prFrcPrm->ptRtCli->u4FrameCnt + 1), &u8Remainder);
            prFrcPrm->ptRtCli->u4FrameCnt ++;
            
            if(prFrcPrm->ptRtCli->u4FrameCnt > 300)
            {
                prFrcPrm->ptRtCli->u4TimeAve   = 0;
                prFrcPrm->ptRtCli->u4FrameCnt  = 0;
                prFrcPrm->ptRtCli->u8TimeTotal = 0;  
            }

#ifdef TIME_MEASUREMENT
            TMS_DIFF_EX(TMS_FLAG_WFD_LATENCY, "WFD_DATA:", "B2R Chg Frm");
            TMS_DIFF_EX(TMS_FLAG_WFD_LATENCY, "WFD_DATA", "B2R Chg Frm");
            LOG(4, "WFD_B2R_TMS: FbId(%d) PTS(0x%08x).\n", prFrcPrm->ucFbId, prFrcPrm->u4Pts);
#endif
            prPicHdr->fgWFDTMSLogEnable = FALSE;
        } 

#ifdef __KERNEL__
        if(prPicHdr && prPicHdr->u8_vid_pts)
        {                 
            UINT64 u8CurTime = 0;
            struct timeval tv = {0};
            
            do_gettimeofday(&tv);
            
            u8CurTime = (UINT64)tv.tv_sec * 1000000;                
            u8CurTime = u8CurTime + (UINT64)tv.tv_usec;
            u8CurTime = u8Div6432(u8CurTime, 1000, &u8Remainder);
            LOG(2, "[Tao][B2R]PTS(0x%llx),Time[%llu],CurTime[%llu],CVPTS[0x%x]\n",
            prPicHdr->u8_vid_pts,
            prPicHdr->u8_push_time,
            u8CurTime,
            prFrcPrm->u4Pts);
            
            prPicHdr->u8_vid_pts = 0;
            prPicHdr->u8_push_time = 0;
        }
#endif
    }
    while(0);

    return i4Ret;
}
#endif

VDP_CFG_T* _B2R_GetVdpConf(UCHAR ucVdpId)
{
    return (ucVdpId >= VDP_NS)? NULL : &_arVdpCfg[ucVdpId];
}

void _B2R_VdpCfgReset(UCHAR ucVdpId)
{
    UCHAR  ucOrgVdpId;
    UINT32 u4_idx;
    //UINT32 u4_LogFilter;

    ucOrgVdpId = _arVdpCfg[ucVdpId].ucVdpId;
    //u4_LogFilter = _arVdpCfg[ucVdpId].u4LogFilter;
    x_memset(&_arVdpCfg[ucVdpId],0x0,sizeof(VDP_CFG_T));
    _arVdpCfg[ucVdpId].ucVdpId  = ucOrgVdpId;
    _arVdpCfg[ucVdpId].ucB2rId  = B2R_NS;
    _arVdpCfg[ucVdpId].ucEnable = B2R_DISABLE;
    _arVdpCfg[ucVdpId].ucStatus = VDP_STATUS_NOSIGNAL;
    _arVdpCfg[ucVdpId].ucMode = VDP_MODE_UNKNOWN;
    for(u4_idx = 0; u4_idx < VDP_MAX_INPORT_NS; u4_idx++)
    {
        _arVdpCfg[ucVdpId].ucInputPort[u4_idx] = MAX_ES_NS;
    }
    _arVdpCfg[ucVdpId].u4SrcLineSize = 768;
    _arVdpCfg[ucVdpId].u4SrcWidth = 720;
    _arVdpCfg[ucVdpId].u4SrcHeight = 480;
    _arVdpCfg[ucVdpId].u4AspectRatio = VDP_UNIT_ASPECT_RATIO;
    _arVdpCfg[ucVdpId].u4FrameRate = 60;
    _arVdpCfg[ucVdpId].u4Afd = ACT_FMT_DEFAULT;
    //_arVdpCfg[ucVdpId].u4LogFilter = u4_LogFilter;
}

static inline BOOL _B2R_IN_Enable_Status (UCHAR    ucVdpId,
                                                UINT32   u4Enable)
{
    VDP_CFG_T* prVdpConf;
    
    if(ucVdpId >= VDP_NS)
    {
        LOG(1,"ucVdpId invalid!\n");
        return FALSE;
    }

    prVdpConf = _B2R_GetVdpConf(ucVdpId);
    if(!prVdpConf)
    {
        return FALSE;
    }
    
    return (prVdpConf->ucEnable == u4Enable);
}

static INT32 _B2R_OneFbChgHandle(B2R_PRM_T *prFrcPrm)
{
    UCHAR ucVdpId = 0;
    B2R_OBJECT_T *this = NULL;

    if (prFrcPrm == NULL)
    {
        LOG(0, "(%s) prFrcPrm is NULL!\n", __FUNCTION__);
        return -1;
    }

    this = _B2R_GetObj(prFrcPrm->ucB2rId);
    if (!this)
    {
        LOG(0, "(%s) this is NULL, ucB2rId %d!\n", __FUNCTION__, prFrcPrm->ucB2rId);
        return -1;
    }

    if (FBM_GetFrameBufferStatus(prFrcPrm->ucFbgId, 0) != FBM_FB_STATUS_LOCK)
    {
        prFrcPrm->rOneFbChg.u4Count = 0;
        return -2;
    }

    prFrcPrm->rOneFbChg.u4Count++;
    ucVdpId = _B2R_GetVdpId(prFrcPrm->ucB2rId);

    if (prFrcPrm->rOneFbChg.fgTrigger && (prFrcPrm->rOneFbChg.u4Count >= 3))
    {
        VDP_SetFreezeEx(ucVdpId, 1);
        LOG(0, "(%s) Set VDP freeze, @count(%u)!\n", __FUNCTION__, prFrcPrm->rOneFbChg.u4Count);
        prFrcPrm->rOneFbChg.fgReusing = 1; 
        _B2R_SetFrameBufferStatus(this, prFrcPrm->ucFbgId, 0, FBM_FB_STATUS_EMPTY);
        prFrcPrm->rOneFbChg.fgTrigger = 0;

        return 0;
    }
    else if (prFrcPrm->rOneFbChg.fgReusing && (prFrcPrm->rOneFbChg.u4Count >= 1))
    {
        VDP_SetFreezeEx(ucVdpId, 0);
        LOG(0, "(%s) Set VDP unfreeze, @count(%u)!\n", __FUNCTION__, prFrcPrm->rOneFbChg.u4Count);
        prFrcPrm->rOneFbChg.fgReusing = 0;

        return 1;
    }

    return -3;
}

BOOL B2R_OneFbChgTrigger(UCHAR ucVdpId)
{
    UCHAR ucB2rId = 0;
    B2R_PRM_T *prFrcPrm = NULL;
    VDP_CFG_T *prVdpCfg = NULL;

    if(ucVdpId >= VDP_NS)
    {
        LOG(0, "(%s) ucVdpId >= VDP_NS oops!\n", __FUNCTION__);
        return FALSE;
    }

    prVdpCfg = _B2R_GetVdpConf(ucVdpId);
    if (!prVdpCfg)
    {
        LOG(0, "(%s) _B2R_GetVdpConf(%d) oops!\n", __FUNCTION__, ucVdpId);
        return FALSE;
    }

    ucB2rId = prVdpCfg->ucB2rId;
    if (ucB2rId >= B2R_NS)
    {
        LOG(0, "(%s) ucB2RId >= B2R_NS oops!\n", __FUNCTION__);
        return FALSE;
    }

    prFrcPrm = &_arVdpPrm[ucB2rId];
    if (FBM_GetFrameBufferNs(prFrcPrm->ucFbgId) != 1)
    {
        LOG(0, "(%s) There is has %u FB now!\n", __FUNCTION__,
                FBM_GetFrameBufferNs(prFrcPrm->ucFbgId));
        return TRUE;
    } 

    if (prFrcPrm->rOneFbChg.fgTrigger || prFrcPrm->rOneFbChg.fgReusing)
    {
        LOG(0, "(%s) Working(%u/%u), FbNR(%u)!\n", __FUNCTION__,
                prFrcPrm->rOneFbChg.fgTrigger,
                prFrcPrm->rOneFbChg.fgReusing,
                FBM_GetFrameBufferNs(prFrcPrm->ucFbgId));
        return FALSE;
    }

    if (FBM_GetFrameBufferStatus(prFrcPrm->ucFbgId, 0) == FBM_FB_STATUS_EMPTY)
    {
        LOG(0, "(%s) FB has be empty now!\n", __FUNCTION__);
        return TRUE;
    }

    prFrcPrm->rOneFbChg.fgTrigger = TRUE;
    LOG(0, "(%s) Trigger on\n", __FUNCTION__);

    return TRUE;
}

INT32 _B2R_4K60TimingChg(B2R_OBJECT_T*      this, 
                         B2R_4K60FRC4K30_T* pr_force,
                         UCHAR              ucFrmRate)
{
    INT32 i4Ret = B2R_OK;
    B2R_PRM_T* prFrcPrm = NULL;
    VDP_CFG_T* prVdpConf = NULL;
    
    do
    {
        if(!IS_IC_5890_ES1())
        {
            i4Ret = B2R_NOT_IMPL;
            break;
        }
        
        if(!this || !pr_force)
        {
            i4Ret = B2R_INV_ARG;
            break;
        }

        prFrcPrm = this->ptB2rPrm;
        prVdpConf = _B2R_GetDP(this);
        if(!prFrcPrm || !prVdpConf)
        {
            i4Ret = B2R_INV_ARG;
            break;
        }

        if(prVdpConf->fgBypssEnabe)
        {
            break;
        }

        if(!(pr_force->u4SeqWidth > 1920 || pr_force->u4SeqHeight > 1080))
        {
            break;
        }
        
        switch (ucFrmRate)
        {
            case MPEG_FRAME_RATE_60_:
                pr_force->u4Rate  = 30000;
                pr_force->u4Scale = 1001;
                prFrcPrm->t_b2r_vdp_uhd.fgEnable = TRUE;
                prVdpConf->u4FrameRate = FBM_FRAME_RATE_30;
                prFrcPrm->ucInFrameRate = FBM_FRAME_RATE_30;
                prFrcPrm->u4InStcPeriodBase = VDP_STC_CLOCK / prFrcPrm->ucInFrameRate;
                LOG(0,"force 4K60 to 4K30!\n");
                break;
                
            case MPEG_FRAME_RATE_60:
                pr_force->u4Rate  = 30;
                pr_force->u4Scale = 1;
                prFrcPrm->t_b2r_vdp_uhd.fgEnable = TRUE;
                prVdpConf->u4FrameRate = FBM_FRAME_RATE_30;
                prFrcPrm->ucInFrameRate = FBM_FRAME_RATE_30;
                prFrcPrm->u4InStcPeriodBase = VDP_STC_CLOCK / prFrcPrm->ucInFrameRate;
                LOG(0,"force 4K60 to 4K30!\n");
                break;

            default:
                break;
        }
        
    }while(0);

    return i4Ret;
}

INT32 _B2R_4K60FbHdr(B2R_OBJECT_T* this)
{
    BOOL  b_FstDrop = FALSE;
    BOOL  b_SndDrop = FALSE;
    INT32 i4Ret = B2R_OK;
    UCHAR ucSndFbId = FBM_FB_ID_UNKNOWN;
    UINT32 u4Offset;
    UINT64 u8Remider;
    UINT64 u8Threshold;
    B2R_PRM_T* prFrcPrm = NULL;
    B2R_VAR_T* prB2rVar = NULL;
    FBM_PIC_HDR_T* prFstPicHdr = NULL;
    FBM_PIC_HDR_T* prSndPicHdr = NULL;

    do
    {
        if(!IS_IC_5890_ES1())
        {
            i4Ret = B2R_NOT_IMPL;
            break;
        }

        if(!this)
        {
            i4Ret = B2R_INV_ARG;
            break;
        }

        prFrcPrm = this->ptB2rPrm;
        prB2rVar = this->ptB2rVar;
        if(!prFrcPrm || !prB2rVar)
        {
            i4Ret = B2R_INV_ARG;
            break;
        }
        
        if(!prFrcPrm->t_b2r_vdp_uhd.fgEnable ||
            !prB2rVar->fgNPTVStable)
        {
            break;
        }

        if(FBM_FB_ID_UNKNOWN == prFrcPrm->ucFbId)
        {
            break;
        }
        
        u8Threshold = (UINT64)prFrcPrm->u4OutStcPeriod/2;
        
        prFstPicHdr = FBM_GetFrameBufferPicHdr(prFrcPrm->ucFbgId, prFrcPrm->ucFbId);
        if(prFstPicHdr)
        {
            u4Offset = prFstPicHdr->u4PTS - prFrcPrm->t_b2r_vdp_uhd.u4StartPTS;
            u8Div6432((UINT64)u4Offset, (UINT64)prFrcPrm->u4OutStcPeriod, &u8Remider);
            if(u8Remider < u8Threshold)
            {
                if(FBM_ChkFrameBufferPicFlag(prFrcPrm->ucFbgId, prFrcPrm->ucFbId, FBM_MM_EOS_FLAG) ||
                    (FBM_ChkFrameBufferPicFlag(prFrcPrm->ucFbgId, prFrcPrm->ucFbId, FBM_MM_PSEUDO_EOS_FLAG)))
                {
                    break;
                }
                _B2R_SetFrameBufferStatus(this, prFrcPrm->ucFbgId, prFrcPrm->ucFbId, FBM_FB_STATUS_LOCK);
                _B2R_SetFrameBufferStatus(this, prFrcPrm->ucFbgId, prFrcPrm->ucFbId, FBM_FB_STATUS_EMPTY);
                b_FstDrop = TRUE;
                LOG(8,"First Drop FbId:%d!\n",prFrcPrm->ucFbId);
            }
        }

        if(prB2rVar->fgDropFb)
        {
            break;
        }
        
        ucSndFbId = FBM_PreLookFrameBufferFromDispQ(prFrcPrm->ucFbgId);
        prSndPicHdr = (ucSndFbId == FBM_FB_ID_UNKNOWN)? NULL : FBM_GetFrameBufferPicHdr(prFrcPrm->ucFbgId, ucSndFbId);
        if(prSndPicHdr)
        {
            u4Offset = prSndPicHdr->u4PTS - prFrcPrm->t_b2r_vdp_uhd.u4StartPTS;
            u8Div6432((UINT64)u4Offset, (UINT64)prFrcPrm->u4OutStcPeriod, &u8Remider);
            ucSndFbId = FBM_GetFrameBufferFromDispQ(prFrcPrm->ucFbgId);
            if(u8Remider < u8Threshold)
            {
                if(FBM_ChkFrameBufferPicFlag(prFrcPrm->ucFbgId, ucSndFbId, FBM_MM_EOS_FLAG) ||
                    (FBM_ChkFrameBufferPicFlag(prFrcPrm->ucFbgId, ucSndFbId, FBM_MM_PSEUDO_EOS_FLAG)))
                {
                    break;
                }
                _B2R_SetFrameBufferStatus(this, prFrcPrm->ucFbgId, ucSndFbId, FBM_FB_STATUS_LOCK);
                _B2R_SetFrameBufferStatus(this, prFrcPrm->ucFbgId, ucSndFbId, FBM_FB_STATUS_EMPTY);
                b_SndDrop = TRUE;
                LOG(8,"Second Drop FbId:%d!\n",prFrcPrm->ucFbId);
            }
        }
    }while(0);

        if(b_FstDrop || b_SndDrop)
        {
            if(b_FstDrop && b_SndDrop)
            {
                prFrcPrm->ucFbId = FBM_FB_ID_UNKNOWN;
            }
            else if(b_FstDrop)
            {
                prFrcPrm->ucFbId = ucSndFbId;
            }
        }
        else
        {
            if(ucSndFbId != FBM_FB_ID_UNKNOWN)
            {
                if(!(FBM_ChkFrameBufferPicFlag(prFrcPrm->ucFbgId, ucSndFbId, FBM_MM_EOS_FLAG) ||
                    (FBM_ChkFrameBufferPicFlag(prFrcPrm->ucFbgId, ucSndFbId, FBM_MM_PSEUDO_EOS_FLAG))))
                {
                    _B2R_SetFrameBufferStatus(this, prFrcPrm->ucFbgId, ucSndFbId, FBM_FB_STATUS_LOCK);
                    _B2R_SetFrameBufferStatus(this, prFrcPrm->ucFbgId, ucSndFbId, FBM_FB_STATUS_EMPTY);
                }
            }
        }

    return i4Ret;
}

INT32 _B2R_AudTrigger(UINT8 u1VdpId, VDP_TRIG_AUD_CFG_T* pt_trig_cfg)
{
    INT32   i4Ret = B2R_OK;
    VDP_TRIG_AUD_ARRIVE_CB_T*   prTrgAudCbInfo = NULL;
    do
    {
        if(u1VdpId >= VDP_NS ||
            !pt_trig_cfg)
        {
            i4Ret = B2R_INV_ARG;
            break;
        }

        if(pt_trig_cfg->fgTrigEnable)
        {
            _B2R_TriggerAudReceive(u1VdpId,pt_trig_cfg->u4Pts);
        }
        else
        {
            _arVdpTrigAud[u1VdpId].ucTriggerReceived = 0;
        }

        x_memset((void *)&_arVdpTrigAud[u1VdpId].rTrgAudCbInfo, 0, sizeof(VDP_TRIG_AUD_ARRIVE_CB_T));
        if (pt_trig_cfg->prTrgAudCbInfo && 
            pt_trig_cfg->fgTrigEnable)
        {
            prTrgAudCbInfo = (VDP_TRIG_AUD_ARRIVE_CB_T*)pt_trig_cfg->prTrgAudCbInfo;
            _arVdpTrigAud[u1VdpId].rTrgAudCbInfo.pfnTrigAud = prTrgAudCbInfo->pfnTrigAud;
            _arVdpTrigAud[u1VdpId].rTrgAudCbInfo.pvTag = prTrgAudCbInfo->pvTag;
            LOG(3,"%s Set Callback!\n",__FUNCTION__);
        }

        LOG(3,"%s End ,B2R(%d) Trigger PTS : 0x%x!\n",__FUNCTION__,u1VdpId,pt_trig_cfg->u4Pts);
    }while(0);

    return i4Ret;
}

INT32 _B2R_SpecialTimingHdr(BOOL  fg3D,
                                     B2R_HAL_TIMING_T* ptTiming,
                                     B2R_OUT_INFO_T*   ptOutInfo)
{
    INT32 i4Ret = B2R_OK;

    do
    {
        if(!ptTiming || !ptOutInfo)
        {
            i4Ret = B2R_INV_ARG;
            break;
        }

        if(!ptTiming->fgProg &&
            ptTiming->u4HTotal == 2640 &&
            ptTiming->u4VTotal == 1125)
        {
            ptOutInfo->ucFrameRate = 50;
            ptOutInfo->ucFrameRate = 
                fg3D? (ptOutInfo->ucFrameRate<<1) : ptOutInfo->ucFrameRate;
            break;
        }
    }while(0);

    return i4Ret;
}

static INT32 _B2R_ColorFmtMonitor(B2R_OBJECT_T* this)
{
    INT32 i4Ret = B2R_OK;
    BOOL bColorFmtChg = FALSE;
    B2R_PRM_T* prFrcPrm = NULL;
    FBM_PIC_HDR_T* ptPicHdr = NULL;
    FBM_SEQ_HDR_T* ptSeqHdr = NULL;
    B2R_HAL_COLOR_FMT_T tColorFmt;
    
    do
    {
        if(!this)
        {
            i4Ret = B2R_INV_ARG;
            break;
        }

        prFrcPrm = this->ptB2rPrm;
        if(!prFrcPrm)
        {
            i4Ret = B2R_INV_ARG;
            break;
        }

        ptPicHdr = FBM_GetFrameBufferPicHdr(prFrcPrm->ucFbgId, prFrcPrm->ucFbId);
        if(!ptPicHdr)
        {
            i4Ret = B2R_FAIL;
            break;
        }

        ptSeqHdr = FBM_GetFrameBufferSeqHdr(prFrcPrm->ucFbgId);
        if(!ptSeqHdr)
        {
            i4Ret = B2R_FAIL;
            break;
        }

        B2R_HAL_Get(this->hB2r, B2R_HAL_GET_COLOR_FMT, &tColorFmt);

        if(tColorFmt.fgUfoEn != ptPicHdr->fgUfoEn)
        {
            tColorFmt.fgUfoEn = ptPicHdr->fgUfoEn;
            B2R_HAL_Set(this->hB2r, B2R_HAL_COLOR_FMT, &tColorFmt);
            bColorFmtChg = TRUE;
            LOG(0,"Dectec UFO CHG(%d)!\n",tColorFmt.fgUfoEn);
        }

        LOG(8,"SeamPlay(%d), Mode(%d),Depth(%d)!\n",
            ptPicHdr->fgSeamlessDisp, ptSeqHdr->eSeamlessMode, ptPicHdr->ucSrcDepth);
            
        if((ptSeqHdr->eSeamlessMode & SEAMLESS_TWO_RESIZER) == SEAMLESS_TWO_RESIZER)                    
        {
            if(ptPicHdr->fgSeamlessDisp &&
                8 != tColorFmt.u1Depth)
            {
                LOG(3,"Depth Chg (%d)-->(8)!\n", tColorFmt.u1Depth, ptPicHdr->ucSrcDepth);
                tColorFmt.u1Depth = 8;
                B2R_HAL_Set(this->hB2r, B2R_HAL_COLOR_FMT, &tColorFmt);
                bColorFmtChg = TRUE;
            }
            else if(!ptPicHdr->fgSeamlessDisp &&
                tColorFmt.u1Depth != ptPicHdr->ucSrcDepth)
            {
                LOG(3,"Depth Chg (%d)-->(%d)!\n", tColorFmt.u1Depth, ptPicHdr->ucSrcDepth);
                tColorFmt.u1Depth = ptPicHdr->ucSrcDepth;
                B2R_HAL_Set(this->hB2r, B2R_HAL_COLOR_FMT, &tColorFmt);
                bColorFmtChg = TRUE;
            }
        }

        if(bColorFmtChg)
        {
            LOG(2,"HW chg!\n");
            B2R_HAL_Set(this->hB2r, B2R_HAL_COLOR_FMT_CHG, &(tColorFmt.fgUfoEn));
        }
        LOG(8,"Pic Ufo(%d)!\n",ptPicHdr->fgUfoEn);
    }while(0);

    return i4Ret;
}


