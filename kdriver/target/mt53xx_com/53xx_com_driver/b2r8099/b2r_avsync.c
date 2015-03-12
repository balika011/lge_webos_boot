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
 * $RCSfile: b2r_avsync.c,v $
 * $Revision: #9 $
 *
 *---------------------------------------------------------------------------*/

/** @file b2r_avsync.c
 *  Brief of file b2r_avsync.c.
 *  Details of file b2r_avsync.c (optional).
 */


//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#include "b2r_debug.h"
#include "b2r_drvif.h"
#include "b2r_if.h"
#include "fbm_drvif.h"
#include "stc_drvif.h"
#include "mute_if.h"
#include "x_debug.h"
#include "x_syslog.h"
#include "sv_const.h"
#include "vdo_rm.h"

#define VDP_HANDLE_32FLAG
#define VDP_EARLY_DISPLAY
//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
#define VDP_WAIT_1ST_PTS_NS             (12)
#define VDP_WAIT_NONZERO_PTS_NS         (5)

#define VDP_MAX_PTS_FILTER_LEN          (4)                         // 4 tap, Enhance for SEOUL_CH11_1_RF14
#define VDP_MAX_PTS_DRIFT               (900)                       // 10 ms, Enhance for SEOUL_CH11_1_RF14
#ifdef MHEG5_IC_SUPPORT_CERT
/* Modify for passing DTG MHEG5 ICS Test Suite  */
#define VDP_INVALID_DRIFT		        (VDP_STC_CLOCK * 1 / 10) // 100ms
#else
#define VDP_INVALID_DRIFT		        (VDP_STC_CLOCK * 7)		 // to fixe Korea goverment office internal broadcasting
#endif
#ifndef CC_B2R_DISABLE_ONE_SECOND_FRC
#define VDP_MAX_ASYNC_REPEAT_NS         (15)
#else
#define VDP_MAX_ASYNC_REPEAT_NS         (150)                        // should not more than 125
#endif
#ifdef MHEG5_IC_SUPPORT_CERT
/* Modify for passing DTG MHEG5 ICS Test Suite  */
#define VDP_MM_PEND_MAX                 (VDP_STC_CLOCK * 1 / 10) // 100ms
#else
#define VDP_MM_PEND_MAX                 (VDP_STC_CLOCK * 20)     //20 Seconds
#endif
#define VDP_EARLY_DISP_DRIFT		    (VDP_STC_CLOCK * 10)		 // 1 Seconds
#define VDP_WAIT_UNMUTE_VSYNC           (255)
#define VDP_VIDEO_DELAY_VSYNC           (4)
#define VDP_KEEP_DROP_B_NS              (10)                        // Enhance for SEOUL_CH11_1_RF14

//WFD related parameter
#define VDP_STATISTICS_PER_SECOND            (5)
#define VDP_MICROSECOND_PER_SECOND           (1000000)
#define VDP_RT_LATENCY_AJD_CNT               (8)
#define VDP_RT_LATENCY_TOLERATE              (2)
#define VDP_RT_LATENCY_LIMIT                 (25)
#define VDP_RT_DIV_TWO                       (2)
#define VDP_RT_INITIAL_CNT                   (1)//time = VDP_INITIAL_CNT * u4OutStcPeriod
#define VDP_RT_STATICE_CNT                   (300)//monitor packet input time interval
#define VDP_RT_LATENCY_TIME                  (40) //wfd latency time limit is 25ms ,so 1000ms / 25ms = 40
#define VDP_RT_STC_ADJ_CNT                   (5)
#define VDP_RT_MULTIPLY_THREE                (3)
#define VDP_RT_STC_ADJ                       (100)

EXTERN BOOL fgB2REarlyDisp;
//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------

typedef struct _B2R_WFD_PTS_INFO_T
{
    UINT32  u4_vid_pts;
    UINT32  u4_aud_pts;
    INT32   i4_vid_diff;
    INT32   i4_aud_diff;
    UINT32  u4CurStc;
    HAL_TIME_T  rDeltaTime;
} B2R_WFD_PTS_INFO_T;



//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------
#define GUESS_32_STEP(tff, rff)         (((1 - tff) << 1) + rff)


//-----------------------------------------------------------------------------
// Imported variables
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Imported functions
//-----------------------------------------------------------------------------
extern BOOL STC_ChkStcStatusDrift(UINT8 u1StcId);
extern BOOL STC_ChkStcStatusValid(UINT8 u1StcId);

//-----------------------------------------------------------------------------
// Static function forward declarations
//-----------------------------------------------------------------------------
#ifdef ENABLE_MULTIMEDIA
static INT32 _B2R_RTAVsyncProc(B2R_OBJECT_T* this);
#endif
//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------
#ifdef VDP_HANDLE_32FLAG
static UINT32 _au4Pd32FlagCadence[2][PD_32_FLAG_SEQ] =
{
    {1, 0, 1, 0},
    {1, 0, 0, 1}
};
#endif


//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/** Brief of _VdpChk32Flag.
 * reference 8105 vsv_sync.c (vVsvChk32Flag)
 */
//-----------------------------------------------------------------------------
static void _B2R_Chk32Flag(B2R_OBJECT_T* this)
{
#ifdef VDP_HANDLE_32FLAG
    UCHAR ucLast32Seq;
    B2R_PRM_T* prFrcPrm = NULL;
    if (!this)
    {
        return;
    }
    prFrcPrm = this->ptB2rPrm;
    if(!prFrcPrm)
    {
        return;
    }
    
    ucLast32Seq = prFrcPrm->ucValid32Seq;

    // check the following 32 flag sequence
    // TFF Sequence    : 1 0 0 1 1 0 0 1 ...
    // RFF Sequence    : 1 0 1 0 1 0 1 0 ...

    if (prFrcPrm->ucValid32Seq == 0)
    {
        UINT32 u4Step;

        u4Step = prFrcPrm->ucPd32SeqStep & (PD_32_FLAG_SEQ - 1);

        if ((prFrcPrm->ucRff == _au4Pd32FlagCadence[0][u4Step]) &&
                (prFrcPrm->ucTff == _au4Pd32FlagCadence[1][u4Step]))
        {
            // valid cadence

            if (++(prFrcPrm->ucPd32SeqStep) >= PD_32_FLAG_LOOP_SIZE)
            {
                prFrcPrm->ucValid32Seq = 1;
                prFrcPrm->ucRelease32Cnt = 0;
            }
        }
        else
        {
            prFrcPrm->ucPd32SeqStep = GUESS_32_STEP(prFrcPrm->ucTff, prFrcPrm->ucRff);
        }
    }
    else
    {
        UINT32 u4Step;

        u4Step = (prFrcPrm->ucPd32SeqStep++) & (PD_32_FLAG_SEQ - 1);

        if ((prFrcPrm->ucRff == _au4Pd32FlagCadence[0][u4Step]) &&
                (prFrcPrm->ucTff == _au4Pd32FlagCadence[1][u4Step]))
        {
            prFrcPrm->ucRelease32Cnt = 0;
        }
        else
        {
            // invalid cadence

//            LOG(1, "Invalid 32 (%d %d %d) (%d)\n",
//                prFrcPrm->ucPd32SeqStep,
//                prFrcPrm->ucTff,
//                prFrcPrm->ucRff,
//                prFrcPrm->ucRelease32Cnt);

            if (++(prFrcPrm->ucRelease32Cnt) >= PD_32_FLAG_LEAVE_LOOP_SIZE)
            {
                prFrcPrm->ucValid32Seq = 0;
                prFrcPrm->ucPd32SeqStep = 0;
            }
            else
            {
                // guess next cadence
                prFrcPrm->ucPd32SeqStep = GUESS_32_STEP(prFrcPrm->ucTff, prFrcPrm->ucRff);
            }
        }
    }

    if (ucLast32Seq != prFrcPrm->ucValid32Seq)
    {
        LOG(3, "32 Flag %d >> %d\n", ucLast32Seq, prFrcPrm->ucValid32Seq);
    }
#else

    // ignore 32 flag in MT5371 project

#ifdef VDP_TEST_32FLAG
    do
    {
        static UINT32 u4Test32Flag = 0;

        if (++u4Test32Flag >= 64)
        {
            u4Test32Flag = 0;

            if (prFrcPrm->ucValid32Seq == 0)
            {
                prFrcPrm->ucValid32Seq = 1;
            }
            else
            {
                prFrcPrm->ucValid32Seq = 0;
            }

            LOG(3, "32 Flag >> %d\n", prFrcPrm->ucValid32Seq);
        }
    }
    while (0);
#else
    UNUSED(this);
#endif

#endif
}
/** Brief of _VdpBaseVSyncNs.
 * reference 8105 vsv_sync.c (fgVsvBaseVSyncNs)
 * Description : Get Base VSync Ns according to RFF, TFF, and PScan state
 *        (1) VSyncNs: duration to display the frame
 *        (2) CCNs:0 Field numbers for Close Caption
 *        (3) CombineNs: duration to combine with previous frame when in PScan output
 */
//-----------------------------------------------------------------------------
static void _B2R_BaseVSyncNs(B2R_OBJECT_T* this)
{
#ifdef VDP_MONITOR_TFF
    static UCHAR ucLastTff = 1;
#endif
    B2R_PRM_T* prFrcPrm = NULL;

    do
    {
        if (!this)
        {
            break;
        }
        prFrcPrm = this->ptB2rPrm;
        if(!prFrcPrm)
        {
            break;
        }
        
        _B2R_Chk32Flag(this);

#ifdef VDP_MONITOR_TFF
        if ((prFrcPrm->ucValid32Seq == 0) &&
                (ucLastTff != prFrcPrm->ucTff))
        {
            LOG(1, "TFF %d >> %d (RFF %d %d)\n"
                , ucLastTff, prFrcPrm->ucTff
                , prFrcPrm->ucLastRff, prFrcPrm->ucRff);
        }
        ucLastTff = prFrcPrm->ucTff;
#endif

        prFrcPrm->u4InStcPeriod = prFrcPrm->u4InStcPeriodBase;

        prFrcPrm->ucMultipleField = 0;

        // 20080209 William: SGP, StickyVideo2.trp
        // Handle RFF, we should not consider frame rate
        if (prFrcPrm->ucProgressiveSeqFlag)        // Progressive Sequence
        {
            if (prFrcPrm->ucProgressiveFrame == 0)  // Progressive Frame
            {
                // Progressive Sequence with Interlace Frame ?! (Bad Edit?)
                if (prFrcPrm->ucRff)
                {
                    // According to 13818-2, If Progressive Frame = 0, RFF shall be 0.
                    LOG(3, "Warning! Progressive Frame = 0 and RFF = 1\n");
                }
            }

            // Acoording to 13818-2, top_field_first section

            if ((prFrcPrm->ucRff) && (prFrcPrm->ucTff))
            {
                // 3 Frame for PRGS_SEQ + REPEAT_1ST_Fld + TOP_FLS_FIRST
                prFrcPrm->u4InStcPeriod *= 3;

                if (!prFrcPrm->ucProgressiveOut)
                {
                    // although according to spec, it should play 6 vsync
                    // but we only encounter streams that will error if we play 6 vsync, but will correct if we play 3 vsync,
                    // this is the stream error, but we modify our code to cover the stream error

                    prFrcPrm->u4InStcPeriod /= 2;
                }

                prFrcPrm->ucMultipleField = 1;
            }
            else if (prFrcPrm->ucRff)
            {
                // 2 Frame for PRGS_SEQ + REPEAT_1ST_Fld
                prFrcPrm->u4InStcPeriod *= 2;

                if (!prFrcPrm->ucProgressiveOut)
                {
                    // although according to spec, it should play 4 vsync
                    // but we only encounter streams that will error if we play 4 vsync, but will correct if we play 2 vsync,
                    // this is the stream error, but we modify our code to cover the stream error

                    prFrcPrm->u4InStcPeriod /= 2;
                }

                prFrcPrm->ucMultipleField = 1;
            }
            else
            {
                // 1 Frame for PRGS_SEQ
            }
        }
        else                                // Interlace Sequence
        {
            // According to 13818-2, repeat_first_field section
            if (prFrcPrm->ucRff)
            {
                if (prFrcPrm->ucProgressiveFrame == 0)  // Progressive Frame
                {
                    // According to 13818-2, If Progressive Frame = 0, RFF shall be 0.
                    LOG(3, "Warning! Progressive Frame = 0 and RFF = 1\n");
                }

                prFrcPrm->u4InStcPeriod *= 3;
                prFrcPrm->u4InStcPeriod /= 2;

                prFrcPrm->ucMultipleField = 1;
            }
        }
    }while(0);
}

static BOOL _B2R_CheckPtsOk(B2R_OBJECT_T *this, UCHAR ucAvSyncMode)
{
    B2R_PRM_T *prFrcPrm = NULL;

    if (!this)
    {
        return FALSE;
    }
    
    prFrcPrm = this->ptB2rPrm;
    if(!prFrcPrm)
    {
        return FALSE;
    }

    if(!prFrcPrm->prFbCounter)
    {
        return FALSE;
    }
    
    if (prFrcPrm->u4Pts == 0)
    {
        LOG(9, "Zero PTS (%d) Delta(%d)\n", prFrcPrm->u4ZeroPtsNs, prFrcPrm->u4PtsDelta);
    
        prFrcPrm->u4ZeroPtsNs++;
    
        if (prFrcPrm->ucReady == 0)
        {
            if ((ucAvSyncMode == VID_SYNC_MODE_SLAVE_MM)
                    || (ucAvSyncMode == VID_SYNC_MODE_MASTER) /* 2x Trick Play */)
            {
                // MM playback receive 1st PTS as 0, Start to Play
                LOG(3, "MM playback receive 1st PTS as 0, Start to Play\n");
                prFrcPrm->u4PtsAsyncNs = 0;
            }
            else
            {
                if (prFrcPrm->u4PtsAsyncNs < VDP_WAIT_NONZERO_PTS_NS)
                {
                    // pending for the 1st PTS
                    prFrcPrm->u2TargetNs = 0;
                    prFrcPrm->u2ChangeFieldNs = 0;
    
                    prFrcPrm->u4InFrameStc = 0;
                    prFrcPrm->u4OutFrameStc = 0;
    
                    // drop this frame, wait for another frame with PTS != 0
                    /*
                                                prFrcPrm->ucPendingFbId = prFrcPrm->ucFbId;
                                                prFrcPrm->ucLastFbId = FBM_FB_ID_UNKNOWN;
                                                prFrcPrm->ucFbId = FBM_FB_ID_UNKNOWN;
                                        */
                    prFrcPrm->u4PtsAsyncNs++;
                }
                else
                {
                    // STC still invalid after 12 vsync, forget it, go display!
                    LOG(3, "Force Video Start 1\n");
                    prFrcPrm->u4PtsAsyncNs = 0;
                }
            }
    
        }
        else
        {
            // Handle Keep Drop B
            if (prFrcPrm->u4KeepDropB != 0)
            {
                LOG(3, "AVS-D VLD Zero PTS MPV Q(%d)\n", _B2R_GetEsmQCnt());
    
                FBM_SetFrameBufferFlag(prFrcPrm->ucFbgId, FBM_FLAG_SKIP_PIC_NO_PTS);
            }
        }
    
        prFrcPrm->prFbCounter->u4SyncMode = FBM_FBG_AVSYNC_NO_PTS;
        FBM_SetAVSyncStatus(prFrcPrm->ucFbgId, prFrcPrm->prFbCounter->u4SyncMode);
        return FALSE;
    
    }
    else // if current pts !=0 , calculate difference between current frame and last frame
    {
        if (prFrcPrm->u4ZeroPtsNs != 0)
        {
            prFrcPrm->u4KeepDropB = 0;
        }

        prFrcPrm->u4ZeroPtsNs = 0;

    
        if (prFrcPrm->u4LastPts != 0)
        {
            UINT32 u4PtsDelta;
            // Calculate PTS Delta
            if (prFrcPrm->u4Pts > prFrcPrm->u4LastPts)
            {
                u4PtsDelta = (prFrcPrm->u4Pts - prFrcPrm->u4LastPts);
            }
            else
            {
                // handle wrap around
                u4PtsDelta = (0xFFFFFFFF - prFrcPrm->u4LastPts);
                u4PtsDelta += prFrcPrm->u4Pts;
            }
    
            if (prFrcPrm->u4PtsDelta != 0)
            {
                prFrcPrm->u4PtsDelta -= (prFrcPrm->u4PtsDelta / VDP_MAX_PTS_FILTER_LEN);
                prFrcPrm->u4PtsDelta += (u4PtsDelta / VDP_MAX_PTS_FILTER_LEN);
            }
            else
            {
                // Filter initial
                prFrcPrm->u4PtsDelta = u4PtsDelta;
            }
        }
    
        // Backup last PTS
        prFrcPrm->u4LastPts = prFrcPrm->u4Pts;
    }


    return TRUE;
    
}
//-----------------------------------------------------------------------------
/** Brief of _B2R_FrameRateProc.
 * reference 8105 vsv_sync.c (fgVsvFrameRateProc)
 * Frame Rate Convert Procedure
 * PS_FILM_XX, first number: target frame_no, 2nd number: combine frame_no
 */
//-----------------------------------------------------------------------------
static void _B2R_FrameRateProc(B2R_OBJECT_T* this)
{
    UINT32 u4InStcPeriod;
    UINT32 u4OutStcPeriod;
    UINT32 u4OutVSyncNs;
    UINT32 u4InFrameStc;
    UINT32 u4OutFrameStc;
    B2R_VAR_T* prB2rVar = NULL;
    UINT32 u4StcDiffVSyncNs;
    B2R_PRM_T* prFrcPrm = NULL;

    if (!this)
    {
        return;
    }

    prB2rVar = this->ptB2rVar;
    prFrcPrm = this->ptB2rPrm;
    if(!prB2rVar || !prFrcPrm)
    {
        return;
    }
    
    u4InStcPeriod = prFrcPrm->u4InStcPeriod;
    u4OutStcPeriod = prFrcPrm->u4OutStcPeriod;

    if (u4OutStcPeriod == 0)
    {
        return;
    }

    u4OutVSyncNs = u4InStcPeriod / u4OutStcPeriod;

    u4InFrameStc = prFrcPrm->u4InFrameStc + u4InStcPeriod;
    u4OutFrameStc = prFrcPrm->u4OutFrameStc + (u4OutStcPeriod * u4OutVSyncNs);

    if (u4InFrameStc >= u4OutFrameStc)    // Check to see if we need to repeat?
    {
        u4StcDiffVSyncNs = (u4InFrameStc - u4OutFrameStc) / u4OutStcPeriod;

        if (u4StcDiffVSyncNs > 0)
        {
            if ((prFrcPrm->ucInFrameRate == FBM_FRAME_RATE_30) ||
                (prFrcPrm->ucInFrameRate == FBM_FRAME_RATE_25))        // Interlace Source
            {
                if (prFrcPrm->ucValid32Seq)    // 32 flag sequence valid, repeat at NOT RFF
                {
                    if (!prFrcPrm->ucRff)
                    {
                        // @쪺k
                        u4OutVSyncNs += u4StcDiffVSyncNs;
                    }
                }
                else    // Progressive/Interlace Output
                {
                    // @쪺k
                    u4OutVSyncNs += u4StcDiffVSyncNs;
                }
            }
            else                                                    // Progressive Source
            {
                // @쪺k
                u4OutVSyncNs += u4StcDiffVSyncNs;
            }
        }
    }
    else                                // Check to see if we need to skip?
    {
        // It is possible
        // At 30 >> 25 FRC case, we will output 2 2 1 2 2 1 ...
        // When this case integrate will deinterlace, we add a constraint "Don't skip 1st B's field (IBBPBB...)"
        // When we change back to 30, we will have (u4InFrameStc < u4OutFrameStc)

        LOG(3, "FRC OutFrameStc(%d) > InFrameStc(%d) \n", prFrcPrm->u4OutFrameStc, prFrcPrm->u4InFrameStc);

        while ((u4OutVSyncNs) && (u4OutFrameStc > u4InFrameStc))
        {
            u4OutFrameStc -= u4OutStcPeriod;
            u4OutVSyncNs--;
        }
    }

    //LOG(3,"u4OutVSyncNs = %d u4InStcPeriod = %d u4OutStcPeriod = %d u4InFrameStc = %d u4OutFrameStc = %d\n", u4OutVSyncNs,u4InStcPeriod,u4OutStcPeriod,u4InFrameStc,u4OutFrameStc);

    if (prB2rVar->fgTimeShift)
    {
        if ((prB2rVar->i4TimeShiftSpeed == 200) && (prB2rVar->fgDoubleClock == TRUE))
        {
            prFrcPrm->u2TargetNs = u4OutVSyncNs / 2;
        }
        else if (prB2rVar->i4TimeShiftSpeed == 50)
        {
            prFrcPrm->u2TargetNs = u4OutVSyncNs * 2;
        }
        else
        {
            prFrcPrm->u2TargetNs = u4OutVSyncNs;
        }
    }
    else
    {
        prFrcPrm->u2TargetNs = u4OutVSyncNs;
    }


    prFrcPrm->u2ChangeFieldNs = prFrcPrm->u2TargetNs;

    // Field Procedure
    // MPEG1 (Progressive Sequence = 1) ȮɺO Progressive Source
    prFrcPrm->ucInterlace2Interlace = 0;
    if (prFrcPrm->ucProgressiveOut)        // Progressive output
    {
        // Different with MT5351
        // I2P or P2P
        prFrcPrm->u2ChangeFieldNs = 0;
    }
    else                                // interlace output
    {
        // Different with MT5351
        // P2I or I2I

        prFrcPrm->ucWrongField = 0;

        if ((prFrcPrm->ucProgressiveSeqFlag) ||
            (prFrcPrm->ucValid32Seq))
        {
            // P2I, Top field first is don't care
        }
        else
        {
            if ((prFrcPrm->ucBottom == prFrcPrm->ucTff) &&
                (prFrcPrm->ucStopAuto == 0) &&
                ((prFrcPrm->ucLastRff == 0) && (prFrcPrm->ucRff == 0)))
            {

                if ((((prFrcPrm->ucInFrameRate ==FBM_FRAME_RATE_30) ||
                        (prFrcPrm->ucInFrameRate ==FBM_FRAME_RATE_60)) &&
                        (prFrcPrm->ucOutFrameRate ==FBM_FRAME_RATE_60)) ||
                        (((prFrcPrm->ucInFrameRate ==FBM_FRAME_RATE_25) ||
                          (prFrcPrm->ucInFrameRate ==FBM_FRAME_RATE_50)) &&
                         (prFrcPrm->ucOutFrameRate ==FBM_FRAME_RATE_50)))
                {
                    // switch to correct field, please consider fast change channel


                    if ((prFrcPrm->u2TargetNs & 1) != 1)
                    {
                        prFrcPrm->u2TargetNs++;
                        prFrcPrm->u2ChangeFieldNs = 0;
                        prFrcPrm->ucWrongField = 1;
                    }

                    LOG(3, "Wrong Field, Odd(%d) Tff(%d)\n", prFrcPrm->ucBottom, prFrcPrm->ucTff);

                }
                else
                {
                    LOG(7, "No TargetNs++\n");
                }
            }
        }
        prFrcPrm->u2ChangeFieldNs /= 2;
    }

    // local STC Accumulate
    prFrcPrm->u4InFrameStc += u4InStcPeriod;
    prFrcPrm->u4OutFrameStc += (u4OutStcPeriod * u4OutVSyncNs);

    if(prFrcPrm->t_wfd_prm.fgDblClk)
    {
        prFrcPrm->u2TargetNs *= 2;
    }
    
    // prevent overflow
    if ((prFrcPrm->u4InFrameStc > 0x80000000) &&
        (prFrcPrm->u4OutFrameStc > 0x80000000))
    {
        prFrcPrm->u4InFrameStc -= 0x80000000;
        prFrcPrm->u4OutFrameStc -= 0x80000000;
    }
}

static BOOL _B2R_IsSkipAVSync(B2R_OBJECT_T *this)
{
    BOOL fgRet = TRUE;
    B2R_PRM_T *prFrcPrm = NULL;
    B2R_VAR_T *prB2rVar = NULL;
    UCHAR ucAvSyncMode;
    UCHAR ucStcSrc;

    do
    {
        if (!this)
        {
            break;
        }
        
        prFrcPrm = this->ptB2rPrm;
        prB2rVar = this->ptB2rVar;
        if(!prFrcPrm || !prB2rVar)
        {
            break;
        }
        
        if (prFrcPrm->u2TargetNs == 0)
        {
            break;
        }
    
        //we don't perform av sync in (timeshift && speed != 1x)
        if (prB2rVar->fgTimeShift && (prB2rVar->i4TimeShiftSpeed != 100))
        {
            break;
        }
   
        FBM_GetSyncStc(prFrcPrm->ucFbgId, &ucAvSyncMode, &ucStcSrc);
    
        // skip AV Sync for Late Notify
        if (prFrcPrm->ucNewFbArrive != 0 && !prB2rVar->fgTimeShift)
        {
            BOOL fgSeamless = FALSE;
            FBM_PIC_HDR_T* prPicHdr = NULL;
            if (prFrcPrm->ucFbgId != FBM_FBG_ID_UNKNOWN && prFrcPrm->ucFbId != FBM_FB_ID_UNKNOWN)
            {
                prPicHdr = FBM_GetFrameBufferPicHdr(prFrcPrm->ucFbgId, prFrcPrm->ucFbId);
                if (prPicHdr != NULL && prPicHdr->fgSeamlessDisp)
                {
                    fgSeamless = TRUE;
                }
            }
#ifdef CC_3D_MM_DS_SUPPORT
            if (prFrcPrm->ucSubFbgId != FBM_FBG_ID_UNKNOWN && prFrcPrm->ucSubFbId != FBM_FB_ID_UNKNOWN)
            {
                if (FBM_IsSeamlessFB(prFrcPrm->ucSubFbgId, prFrcPrm->ucSubFbId))
                {
                    fgSeamless = TRUE;
                }
            }
#endif
    
            if (fgSeamless)
            {
                // Because there are only 2 resize buffer in new seamless mode, it always trigger late notify...
                // We must have avsync in this case
            }
            else
            {
                break;
            }
        }
    
        if (prFrcPrm->ucProgressiveSeqFlag == 0)
        {
            // if drift > 3 secs then free run
            if ((prB2rVar->u4MaxDriftCount) >= 100)
            {
                // count valid
                if (STC_ChkStcStatusValid(ucStcSrc))
                {
                    ++(prB2rVar->u4ValidCount);
                }
                else
                {
                    (prB2rVar->u4ValidCount) = 0;
                }
    
                // if valid > 5 secs then restore the av sync
                if (prB2rVar->u4ValidCount >= 155)
                {
                    (prB2rVar->u4MaxDriftCount) = 0;
                }
    
                break;
            }
    
            // count drift
            if (STC_ChkStcStatusDrift(ucStcSrc))
            {
                ++(prB2rVar->u4DriftCount);
            }
            else
            {
                (prB2rVar->u4DriftCount) = 0;
            }
    
            (prB2rVar->u4MaxDriftCount) = MAX((prB2rVar->u4MaxDriftCount), (prB2rVar->u4DriftCount));
        }


        fgRet = FALSE;
    
    }while (0);

    return fgRet;
}

static BOOL _B2R_StartToPlay(B2R_OBJECT_T* this, UINT32 u4Delta, UINT32 u4Stc)
{
    UINT32 u4Step;
    UCHAR ucB2rId, ucVdpId;
    UINT32 u4OutFrameStc;
    B2R_VAR_T* prB2rVar = NULL; 
    B2R_PRM_T* prFrcPrm = NULL;

    if(!this)
    {
        return FALSE;
    }
    
    ucB2rId = this->ucB2rId;
    ucVdpId = _B2R_GetVdpId(ucB2rId);
    if (ucB2rId >= B2R_NS || ucVdpId >= VDP_NS)
    {
        LOG(0, "ucB2rId = %d, ucVdpId = %d [%s-%d]\n", ucB2rId, ucVdpId, __FUNCTION__, __LINE__);
        return FALSE;
    }
    prB2rVar = this->ptB2rVar;
    prFrcPrm = this->ptB2rPrm;
    if(!prB2rVar || !prFrcPrm)
    {
        return FALSE;
    }

    u4OutFrameStc = (prFrcPrm->u2TargetNs * prFrcPrm->u4OutStcPeriod);

#ifdef VDP_EARLY_DISPLAY
    if (u4Delta > VDP_EARLY_DISP_DRIFT && u4Delta < VDP_INVALID_DRIFT)
    {
        // pending for the 1st PTS
        _B2R_PreparePendingForNextVsync(this);
        LOG(3, "----Pending for the 1st PTS\n");
        return FALSE;
    }
    else if (u4Delta > VDP_INVALID_DRIFT)
    {
        // Bad Edit Program
        LOG(3, "Force Video Start 999 for bad edit bitstream\n");
        prFrcPrm->u4PtsAsyncNs = 0;
        return FALSE;
    }
    else if (prB2rVar->u2VsyncNotStableCount > 0)
    {
        if (prB2rVar->u2VsyncNotStableCount == 0xFF)
        {
            prB2rVar->u2VsyncNotStableCount = prFrcPrm->u2TargetNs;
        }
        // pending for the 1st PTS
        _B2R_PreparePendingForNextVsync(this);
        (prB2rVar->u2VsyncNotStableCount)--;
        LOG(9, "VSYNC not stable (%d)!\n", prB2rVar->u2VsyncNotStableCount);
        return FALSE;
    }

    _B2R_SetDPEarlyDisp(this, 1);
    LOG(3, "ucEarlyDisp VDP(%d)\n", ucVdpId);
#ifdef TIME_MEASUREMENT
   fgB2REarlyDisp =TRUE;
#endif
    //if (!prB2rVar->fgSeqChgSpeedup)
    {
#ifdef VDP_MUTE_EARLY_DISPLAY
        _B2R_SetPathMute(ucB2rId, MUTE_MODULE_DTV, FOREVER_MUTE, FALSE);
        LOG(3, "DTV Mute Early Display -1 VDP(%d)\n", ucVdpId);
#else // VDP_MUTE_EARLY_DISPLAY
        LOG(0, "WARNING: DTV mute early display is disabled \n");
#endif
    }
    if(u4OutFrameStc)
    {
        u4Step = 1 + (u4Delta / u4OutFrameStc);
    }

    prFrcPrm->i4PtsDrift += (INT32)(u4OutFrameStc * (u4Step - 1));

    if (u4Step > 1)
    {
        prFrcPrm->u2RestoreNs = prFrcPrm->u2TargetNs;
    }

    prFrcPrm->u2TargetNs *= u4Step;

    if (prFrcPrm->u2TargetNs != 2)
    {
        prFrcPrm->u2ChangeFieldNs = 0;
    }

    if (prFrcPrm->ucProgressiveOut)
    {
        prFrcPrm->u4WaitUnmute = (prFrcPrm->u2TargetNs + VDP_VIDEO_DELAY_VSYNC) - 2;
    }
    else
    {
        prFrcPrm->u4WaitUnmute = prFrcPrm->u2TargetNs + VDP_VIDEO_DELAY_VSYNC;
    }

    if (prFrcPrm->u4WaitUnmute >= VDP_WAIT_UNMUTE_VSYNC)
    {
        prFrcPrm->u4WaitUnmute = VDP_WAIT_UNMUTE_VSYNC;
    }

    LOG(5, "-----Wait Unmute VSync(%d)  PTS(0x%x) \n", prFrcPrm->u4WaitUnmute, prFrcPrm->u4Pts);

    LOG(3, "AVS-R B2R(%d) Fbg(%d) %d PTS(0x%x) STC(0x%x)\n", prFrcPrm->ucB2rId, prFrcPrm->ucFbgId, prFrcPrm->u2TargetNs, prFrcPrm->u4Pts, u4Stc);
#ifdef TIME_MEASUREMENT
    if(prFrcPrm->u4Pts > u4Stc)
    {
        LOG(0,"B2R Wait Early display PTS(0x%x),STC(0x%x),mute Time(%d)ms!\n",
            prFrcPrm->u4Pts, u4Stc, (prFrcPrm->u4WaitUnmute * prFrcPrm->u4OutStcPeriod) / 90 );
    }
#endif
    return FALSE;

#else
#ifdef CC_FAST_CHANGE_CHANNEL
    UINT32 u4Ahead;
    u4Ahead = STC_GetFastChangeChangeAhead(ucStcSrc);

    if (u4Ahead > u4Delta)
    {
        if (prFrcPrm->ucInterlace2Interlace)
        {
            if (prFrcPrm->ucWrongField == 0)
            {
                // Playback single field only for FRC
                prFrcPrm->u2TargetNs++;
                prFrcPrm->u2ChangeFieldNs = 0;
            }
        }
        else
        {
            prFrcPrm->u2TargetNs++;
        }

        prFrcPrm->ucFastChangeEn = 1;

        LOG(3, "Enable Fast Start PTS %x, STC %x, Delta (%d) Ahead(%d)\n", prFrcPrm->u4Pts, rStcClk.u4Base, i4Delta, u4Ahead);

#ifdef CC_FAST_CHANGE_CHANNEL_LOG
        HAL_GetTime(&rNvgtVideoStartTime);
        u4NvgtTimeAhead = u4Ahead;
#endif
    }
    else
    {
#endif

        if (FBM_ChkFrameBufferFlag(prFrcPrm->ucFbgId, FBM_FLAG_ES_FIFO_FULL))
        {
            // ES FIFO full, can not wait anymore
            LOG(3, "Force Video Start 2\n");
            prFrcPrm->u4PtsAsyncNs = 0;
        }
        else if (u4Delta > VDP_INVALID_DRIFT)
        {
            // Bad Edit Program
            LOG(3, "Force Video Start 4\n");
            prFrcPrm->u4PtsAsyncNs = 0;
        }
        else
        {
            // pending for the 1st PTS
            _B2R_PreparePendingForNextVsync(this);
        }

        return FALSE;

#ifdef CC_FAST_CHANGE_CHANNEL
    }
#endif
#endif
    return TRUE;
}
/*
static BOOL _B2R_IsPathMute(B2R_OBJECT_T *this)
{
    BOOL fgRet = FALSE;
    UCHAR ucVdpId;

    do 
    {
        if (!this)
        {
            break;
        }

        ucVdpId = _B2R_GetVdpId(this->ucB2rId);
        
        if (ucVdpId >= VDP_NS)
        {
            break;
        }

        fgRet = (_u4DrvVideoGetMute(ucVdpId) == SV_ON) ? TRUE : FALSE;
    
    }while (0);

    return fgRet;
}
*/
#ifdef CC_SUPPORT_NPTV_SEAMLESS 

VOID _B2R_NPTVSeamlessStatus(B2R_OBJECT_T* this, UCHAR ucMode)
{
    B2R_PRM_T* prFrcPrm = NULL;
    UCHAR ucCurrnetMode = 0;
    
    if (!this)
    {
        return;
    }

    prFrcPrm = this->ptB2rPrm;
    if ((!prFrcPrm) || (!FBM_ChkSeamlessMode(prFrcPrm->ucFbgId, SEAMLESS_BY_NPTV)))
    {
        return;
    }
    
    if (ucMode == VDP_SMLS_MODE_NONE)
    {
        LOG(3, "VDP Seamless Mode Clear: %d -> %d.\n", prFrcPrm->ucSeamlessVDPMode, 0);
        prFrcPrm->ucSeamlessVDPMode = 0;
        return;
    }
    else
    {
        ucCurrnetMode = prFrcPrm->ucSeamlessVDPMode;
    }

    switch (ucCurrnetMode)
    {
    case VDP_SMLS_MODE_NONE:
        ASSERT((ucMode == VDP_SMLS_PREPARE) || (ucMode == VDP_SMLS_PREPARE_WHILE_START_PLAY));
        vVRMSetEventFlg(SV_VP_MAIN, VRM_EVENT_BY_B2R);//lijia debug NPTV
        ucCurrnetMode = ucMode;
        break;
    case VDP_SMLS_PREPARE:
        ASSERT(ucMode == VDP_SMLS_READY);
        ucCurrnetMode = VDP_SMLS_READY;
        break;
    case VDP_SMLS_READY:
        ASSERT(ucMode == VDP_SMLS_FRAME_COMING);
        ucCurrnetMode = VDP_SMLS_FRAME_COMING;
        break;
    case VDP_SMLS_FRAME_COMING:
        break;
    case VDP_SMLS_PREPARE_WHILE_START_PLAY:
        if (ucMode == VDP_SMLS_PREPARE_WHILE_START_PLAY)
        {
            break;
        }
        ASSERT(ucMode == VDP_SMLS_READY_WHILE_START_PLAY);
        ucCurrnetMode = VDP_SMLS_READY_WHILE_START_PLAY;
        break;
    case VDP_SMLS_READY_WHILE_START_PLAY:
        ASSERT(ucMode == VDP_SMLS_FRAME_COMING);
        ucCurrnetMode = VDP_SMLS_FRAME_COMING;
        break;
    } 

    LOG(3, "VDP Seamless Mode Chg: %d -> %d.\n", prFrcPrm->ucSeamlessVDPMode, ucCurrnetMode);
    prFrcPrm->ucSeamlessVDPMode = ucCurrnetMode;
}

#endif

static BOOL _B2R_IsNptvStable(B2R_OBJECT_T *this, UCHAR ucStcSrc, STC_CLOCK* prStcClk)
{
	B2R_VAR_T* prB2rVar = NULL;
	B2R_PRM_T* prFrcPrm = NULL;
	UCHAR ucB2rId;
	BOOL fgRet = FALSE;
	
	do 
	{
		if (!this || !prStcClk)
		{
			break;
		}

		ucB2rId  = this->ucB2rId;
		if(ucB2rId >= B2R_NS)
		{
			break;
		}
		
		prB2rVar = this->ptB2rVar;
		prFrcPrm = this->ptB2rPrm;
		if(!prB2rVar || !prFrcPrm)
		{
			break;
		}
		
		FBM_GetPlayMode(prFrcPrm->ucFbgId,&(prB2rVar->u1PlayMode));
		if ( (FBM_FBG_MM_MODE == prB2rVar->u1PlayMode) &&
#ifdef ENABLE_MULTIMEDIA
			  (_B2R_GetContainerType(this) != SWDMX_FMT_MPEG2_TS_TIME_SHIFT) &&
#endif
			  (prFrcPrm->ucReady ==1) && !(prB2rVar->fgNPTVStable) && !(prFrcPrm->fgB2R3DEnable))
		{
		
			BOOL fgSeamlessReady = FALSE;
        #ifndef CC_SUPPORT_NPTV_SEAMLESS 
			if (FBM_ChkSeamlessMode(prFrcPrm->ucFbgId, SEAMLESS_BY_NPTV))
			{
				fgSeamlessReady = FALSE;
				if ((prFrcPrm->ucSeamlessVDPMode == VDP_SMLS_PREPARE_WHILE_START_PLAY) && bVRMReadyForB2R(_B2R_GetVdpId(this->ucB2rId)))
				{
					_B2R_NPTVSeamlessStatus(this, VDP_SMLS_READY_WHILE_START_PLAY);
					fgSeamlessReady = TRUE;
				}
				else if (prFrcPrm->ucSeamlessVDPMode == VDP_SMLS_READY_WHILE_START_PLAY)
				{
					fgSeamlessReady = TRUE;
				}
			}
			else
        #endif                
			{
				fgSeamlessReady = TRUE;
			}
			
			if ((!fgSeamlessReady) && (prFrcPrm->ucFbId !=FBM_FB_ID_UNKNOWN))
			{
				// LOG(3,"wait %d vsyncs for NPTV statlbe\n",_au4WaitNPTVStableCount[prFrcPrm->ucVdpId]);
				if (prB2rVar->fgPendingForSTC)
				{
					if (prStcClk->u4Base + prFrcPrm->u4OutStcPeriod < prFrcPrm->u4Pts)
					{
						// pending for the 1st PTS
						_B2R_PreparePendingForNextVsync(this);

						LOG(2, "%d)----Waiting for STC:x%X Frm:0x%X Prd:0x%X\n",
							ucB2rId, prStcClk->u4Base,
							prFrcPrm->u4Pts, prFrcPrm->u4OutStcPeriod);

						break;
					}

				//	_B2R_SetBlack(ucB2rId, FALSE);
					prB2rVar->fgPendingForSTC = FALSE;
				}
				else
				{
					STC_StopStc(ucStcSrc);
					STC_SetStcValue(ucStcSrc,(UINT32)(prFrcPrm->u4Pts));
					prStcClk->u4Base = (UINT32)(prFrcPrm->u4Pts);
					STC_StartStc(ucStcSrc);
					prStcClk->u4Base = prFrcPrm->u4Pts;
				}

				prB2rVar->u4WaitNPTVStableCount = 0;
				prB2rVar->fgNPTVStable = TRUE;

				//LOG(3,"wait %d vsyncs for NPTV statlbe\n",_au4WaitNPTVStableCount[prFrcPrm->ucVdpId]);
            #ifndef CC_SUPPORT_NPTV_SEAMLESS  
				if (prFrcPrm->ucSeamlessVDPMode)
				{
					_B2R_NPTVSeamlessStatus(this, VDP_SMLS_FRAME_COMING);
				}
            #endif

				FBM_ClrFrameBufferGlobalFlag(prFrcPrm->ucFbgId, FBM_FLAG_FB_NO_TIMEOUT);
				FBM_SetFrameBufferFlag(prFrcPrm->ucFbgId, FBM_FLAG_DISPLAYING);
				LOG(3, "%d)----ready for waiting NPTV stable\n", ucB2rId);
			}
		}
		else if ( prFrcPrm->fgB2R3DEnable 
#ifdef ENABLE_MULTIMEDIA
			|| (_B2R_GetContainerType(this) == SWDMX_FMT_MPEG2_TS_TIME_SHIFT)
#endif
				 )
		{
			prB2rVar->fgNPTVStable = TRUE;
			FBM_SetFrameBufferFlag(prFrcPrm->ucFbgId, FBM_FLAG_DISPLAYING);
		}
		else if(0 == prFrcPrm->ucReady &&
			FBM_FBG_MM_MODE == prB2rVar->u1PlayMode)
		{
			break;
		}

		fgRet = TRUE;
		
	}while (0);

	return fgRet;
}


static void _B2R_CalPtsStcDiff(UINT32 u4Pts, UINT32 u4Stc, UINT32 *pu4Diff, INT32 *pi4Diff)
{
    UINT32 u4Delta;
    INT32 i4Delta;

    if (u4Pts > u4Stc)
    {
        u4Delta = (u4Pts - u4Stc);
    
        if (u4Delta < 0x80000000)
        {
            i4Delta = - (INT32) u4Delta;
        }
        else
        {
            // handle wrap around
            u4Delta = (0xFFFFFFFF - u4Delta);
            i4Delta = (INT32) u4Delta;
    
            // [LOG] AVSYNC, V-PTS, Wrap around
        }
    }
    else
    {
        u4Delta = (u4Stc - u4Pts);
    
        if (u4Delta < 0x80000000)
        {
            i4Delta = (INT32) u4Delta;
        }
        else
        {
            // handle wrap around
            u4Delta = (0xFFFFFFFF - u4Delta);
            i4Delta = - (INT32) u4Delta;
    
            // [LOG] AVSYNC, V-PTS, Wrap around
        }
    }

    if (pu4Diff) *pu4Diff = u4Delta;
    if (pi4Diff) *pi4Diff = i4Delta;
    
}

static BOOL _B2R_IsPtsDrift(B2R_OBJECT_T *this, UINT32 u4Delta, INT32 i4Delta, 
    UINT32 u4ZeroPtsNs, UCHAR ucAvSyncMode)
{
    BOOL fgBypass = FALSE;
    B2R_PRM_T *prFrcPrm = NULL;
    B2R_VAR_T *prB2rVar = NULL;

    if (!this)
    {
        return FALSE;
    }
    
    prFrcPrm = this->ptB2rPrm;
    prB2rVar = this->ptB2rVar;
    if(!prFrcPrm || !prB2rVar)
    {
        return FALSE;
    }

    if(!prFrcPrm->prFbCounter)
    {
        return FALSE;
    }
    
    // handle PTS drift
#ifdef CC_VDO_PTS_FILTER

#ifdef CC_FAST_CHANGE_CHANNEL
    if (prFrcPrm->ucFastChangeEn != 0)
    {
        fgBypass = TRUE;    
    }
#endif

    // do not handle PTS drift while Fast Channel Changing

    if (!fgBypass)
    {
        UINT32 u4OutFrameStc;

        if(!prFrcPrm->t_wfd_prm.fgDblClk)
        {
            u4OutFrameStc = (prFrcPrm->u2TargetNs * prFrcPrm->u4OutStcPeriod);
        }
        else
        {
            u4OutFrameStc = prFrcPrm->u4OutStcPeriod;
        }
    
        // Handle Keep Drop B
        if (prFrcPrm->u4KeepDropB != 0)
        {
            if ((i4Delta > 0) && (u4Delta > u4OutFrameStc))
            {
                FBM_SetFrameBufferFlag(prFrcPrm->ucFbgId, FBM_FLAG_SKIP_PIC);
#if (defined(CC_3D_MM_DS_SUPPORT) && defined(CC_B2R_ENABLE_R_VIEW_SKIP_FRAME))
                if (prFrcPrm->ucSubFbgId != FBM_FBG_ID_UNKNOWN && FBM_GetFbgAppMode(prFrcPrm->ucSubFbgId) == FBM_FBG_APP_Frame_Pack_3D)
                {
                    FBM_SetFrameBufferFlag(prFrcPrm->ucSubFbgId, FBM_FLAG_SKIP_PIC);
                }
#endif
            }
            else
            {
                prFrcPrm->u4KeepDropB = 0;
            }
        }

#ifndef CC_B2R_DISABLE_PTS_IIR
        if ((prFrcPrm->ucReady) &&
                (prB2rVar->rVdpDtvFreeze.u4UnFreeze == 0) &&
                (u4ZeroPtsNs == 0) && (ucAvSyncMode == VID_SYNC_MODE_SLAVE))
        {
            if (!(
#ifdef ENABLE_MULTIMEDIA
                ((_B2R_GetContainerType(this) == (UINT32)SWDMX_FMT_MPEG2_TS_TIME_SHIFT) ||
                    (_B2R_GetContentSourceType(this) == (UINT32)SWDMX_SRC_TYPE_WIFI_DISPLAY))  &&
#endif
                   (prFrcPrm->fgReleaseDispQ)))
            {
                INT32 i4PtsDrift;
                i4PtsDrift = i4Delta - prFrcPrm->i4PtsDrift;
                prFrcPrm->i4PtsDrift += (i4PtsDrift / VDP_MAX_PTS_FILTER_LEN);
            
                if ((i4PtsDrift > VDP_MAX_PTS_DRIFT) ||
                        (i4PtsDrift < -VDP_MAX_PTS_DRIFT))
                {
                    LOG(7, "PTS Jitter! (%d, %d, %d)\n", i4PtsDrift, i4Delta, prFrcPrm->i4PtsDrift);
            
                    prFrcPrm->prFbCounter->u4SyncMode = FBM_FBG_AVSYNC_PTS_JITTER;
                    FBM_SetAVSyncStatus(prFrcPrm->ucFbgId, prFrcPrm->prFbCounter->u4SyncMode);
                    if (prFrcPrm->ucWrongField && (prFrcPrm->u2TargetNs&1))
                    {
                        prFrcPrm->i4PtsDrift += prFrcPrm->u4OutStcPeriod;
                    }
            
                    return TRUE;
                }
                else
                {
                    prFrcPrm->fgReleaseDispQ = FALSE;
                }
            }

        }
        else
#endif
        {
            if (prB2rVar->rVdpDtvFreeze.u4UnFreeze != 0)
            {
                prFrcPrm->u4PtsAvSyncNotAllow = 0;
                prB2rVar->rVdpDtvFreeze.u4UnFreeze = 0;
            }

            prFrcPrm->i4PtsDrift = i4Delta;
        }

    }
#endif

    if (ucAvSyncMode == VID_SYNC_MODE_SLAVE)
    {
        if ((prFrcPrm->ucReady != 0) && (u4Delta > VDP_INVALID_DRIFT))
        {
            // Bad Edit Program
            prFrcPrm->u4KeepDropB = 0;
            if(prFrcPrm->prFbCounter)
            {
                prFrcPrm->prFbCounter->u4SyncMode = FBM_FBG_AVSYNC_PTS_JITTER;
                FBM_SetAVSyncStatus(prFrcPrm->ucFbgId, prFrcPrm->prFbCounter->u4SyncMode);
            }
            return TRUE;
        }
    }

    FBM_SetPtsSync(prFrcPrm->ucFbgId,TRUE);
    return FALSE;
}

static BOOL _B2R_ProcFrameRepeat(B2R_OBJECT_T *this, UINT32 u4Delta, UINT32 u4Stc)//LIJIA
{
    UCHAR ucAvSyncMode;
    UCHAR ucStcSrc;
    UINT32 u4OutFrameStc;
    B2R_PRM_T *prFrcPrm = NULL;
    UCHAR ucB2rId;

    if(!this)
    {
        return FALSE;
    }

    ucB2rId = this->ucB2rId;
    if (ucB2rId >= B2R_NS)
    {
        LOG(0, "ucB2rId = %d,[%s-%d]\n", ucB2rId, __FUNCTION__, __LINE__);
        return FALSE;
    }
    prFrcPrm = this->ptB2rPrm;
    if(!prFrcPrm)
    {
        return FALSE;
    }

    if(!prFrcPrm->prFbCounter)
    {
        return FALSE;
    }
    
    prFrcPrm->prFbCounter->u4SyncMode = FBM_FBG_AVSYNC_NORMAL;
    FBM_SetAVSyncStatus(prFrcPrm->ucFbgId, prFrcPrm->prFbCounter->u4SyncMode);
    FBM_GetSyncStc(prFrcPrm->ucFbgId, &ucAvSyncMode, &ucStcSrc);
    if(!prFrcPrm->t_wfd_prm.fgDblClk)
    {
        u4OutFrameStc = (prFrcPrm->u2TargetNs * prFrcPrm->u4OutStcPeriod);
    }
    else
    {
        u4OutFrameStc = prFrcPrm->u4OutStcPeriod;
    }
    if (u4Delta > u4OutFrameStc)
    {
        UINT32 u4Step;
        UINT32 u4TargetNs;

        if (u4OutFrameStc != 0)
        {
            prFrcPrm->u4B2rSkipFrameNo = u4Delta / u4OutFrameStc;
        }
        else
        {
            prFrcPrm->u4B2rSkipFrameNo = 0;
        }
        prFrcPrm->prFbCounter->u4SyncMode = FBM_FBG_AVSYNC_REPEAT;
        FBM_SetAVSyncStatus(prFrcPrm->ucFbgId, prFrcPrm->prFbCounter->u4SyncMode);

#ifndef CC_B2R_DISABLE_ONE_SECOND_FRC
        // protection
        if ((ucAvSyncMode == VID_SYNC_MODE_SLAVE) && (prFrcPrm->u4PtsAvSyncNotAllow))
        {
            return FALSE;
        }
#endif

        if (prFrcPrm->ucReady == 0)
        {
            if (!_B2R_StartToPlay(this, u4Delta, u4Stc))
            {
                return FALSE;
            }
        }

        // ES FIFO FULL, don't repeat anymore
        if (FBM_ChkFrameBufferFlag(prFrcPrm->ucFbgId, FBM_FLAG_ES_FIFO_FULL))
        {
            return FALSE;
        }

        if (prFrcPrm->ucReady == 1)
        {
#ifdef CC_FAST_CHANGE_CHANNEL
            if (prFrcPrm->ucFastChangeEn == 1)
            {
                if (prFrcPrm->ucInterlace2Interlace)
                {
                    if (prFrcPrm->ucWrongField == 0)
                    {
                        // Playback single field only for FRC
                        prFrcPrm->u2TargetNs++;
                        prFrcPrm->u2ChangeFieldNs = 0;
                    }
                }
                else
                {
                    prFrcPrm->u2TargetNs++;
                }

                LOG(3, "Fast Start PTS %x, STC %x, Delta (%d)\n", prFrcPrm->u4Pts, rStcClk.u4Base, i4Delta);
            }
            else
            {
#endif
                if ((ucAvSyncMode == VID_SYNC_MODE_SLAVE_MM)
                    || (ucAvSyncMode == VID_SYNC_MODE_MASTER) /* Trick Play */
#ifdef ENABLE_MULTIMEDIA
                    || (_B2R_GetContainerType(this) == SWDMX_FMT_MPEG2_TS_TIME_SHIFT)
#endif
                   )

                {
                    if (u4Delta > VDP_MM_PEND_MAX)
                    {
                        if (ucAvSyncMode == VID_SYNC_MODE_MASTER)
                        {
                            STC_SetStcValue(ucStcSrc, _B2R_GetDispingPts(this));
                            LOG(3, "MM ----Force Set Stc for bad edit bitstream\n");
                        }
                        else
                        {
                            LOG(3, "MM ----Force Video Start for bad edit bitstream\n");
                            return FALSE;
                        }
                    }
                    else //if (u4Delta > VDP_EARLY_DISP_DRIFT)
                    {
                        UCHAR ucSeekMode;
                        UINT32 u4SeekPts;
                        UCHAR ucSeekNotifyTrigger;

                        _B2R_GetSeek(this, &ucSeekMode, &u4SeekPts, &ucSeekNotifyTrigger);

                        if ((ucSeekMode != 0) && (ucSeekNotifyTrigger != 0) /*&&
                           (u4SeekPts <= prFrcPrm->u4Pts)*/)
                        {
                            LOG(3, "+++++MM ----Frame Repeat in Seek. force play \n");
                            return FALSE;
                        }
                        
                        // pending for the current PTS
                        _B2R_PreparePendingForNextVsync(this);
                        prFrcPrm->ucMMWaitSTC = 1;

                        LOG(3, " MM ----Pending for current PTS\n");
                        return FALSE;
                    }
                    //LOG(3, "MM -- Repeat Last Frame\n\r");
                }
                else if (ucAvSyncMode == VID_SYNC_MODE_SLAVE)
                {
                    UINT32 u4MaxStep = VDP_MAX_ASYNC_REPEAT_NS;

                    if(u4OutFrameStc)
                    {
                        u4Step = 1 + (u4Delta / u4OutFrameStc);
                    }
                    
                    u4Step = (u4Step < u4MaxStep) ? u4Step : u4MaxStep;

                    prFrcPrm->prFbCounter->u4DispRepeat += (prFrcPrm->u2TargetNs * (u4Step - 1));

                    prFrcPrm->i4PtsDrift += (INT32)(u4OutFrameStc * (u4Step - 1));

                    if (prFrcPrm->ucWrongField && (prFrcPrm->u2TargetNs&1))
                    {
                        prFrcPrm->i4PtsDrift += prFrcPrm->u4OutStcPeriod;
                    }

                    u4TargetNs = (prFrcPrm->u2TargetNs*u4Step);
                    if ( u4TargetNs >= 0xFF )
                    {
                        prFrcPrm->u2TargetNs = 250;
                        prFrcPrm->u2ChangeFieldNs =(prFrcPrm->u2TargetNs/2);
                    }
                    else
                    {
                        prFrcPrm->u2TargetNs = (UCHAR)u4TargetNs;
                        prFrcPrm->u2ChangeFieldNs *= u4Step;
                    }

                    // protection
#ifndef CC_DCR_TEST
                    prFrcPrm->u4PtsAvSyncNotAllow = prFrcPrm->ucInFrameRate;
#endif
                    // [LOG] AVSYNC, V-PTS Repeat
                    SYSLOG(SYNC_PREFIX + 20 + prFrcPrm->ucFbgId, prFrcPrm->prFbCounter->u4DispOk, prFrcPrm->prFbCounter->u4DispFail, prFrcPrm->u2TargetNs);

                    /* Video Frame repeat notification */
                    //_VDP_VsyncNotify(, ucVdpId,
                    //                 prFrcPrm->prFbCounter->u4SyncMode, 0);
                    _B2R_PostNotify(this, VDP_MSG_REPEAT_DROP_CB, prFrcPrm->prFbCounter->u4SyncMode, 0);

                    LOG(3, "AVS-R B2R(%d) Fbg(%d) %d PTS(0x%x) STC(0x%x)\n", prFrcPrm->ucB2rId, prFrcPrm->ucFbgId, prFrcPrm->u2TargetNs, prFrcPrm->u4Pts, u4Stc);
                    FBM_SetPtsSync(prFrcPrm->ucFbgId,FALSE);
                }

#ifdef CC_FAST_CHANGE_CHANNEL
            }
#endif
        }
    }
    else
    {
#ifdef CC_FAST_CHANGE_CHANNEL
        if (prFrcPrm->ucFastChangeEn)
        {
            LOG(3, "Fast Stop 1\n");
            prFrcPrm->ucFastChangeEn = 0;
#ifdef CC_FAST_CHANGE_CHANNEL_LOG
            HAL_GetTime(&rNvgtAvSyncTime);
#endif
        }
#endif
    }
    return TRUE;
}

static BOOL _B2R_ProcFrameDrop(B2R_OBJECT_T* this, UINT32 u4Delta, INT32 i4Delta, UINT32 u4ZeroPtsNs, UINT32 u4Stc)
{
    UCHAR ucAvSyncMode;
    UCHAR ucStcSrc;
    UINT32 u4OutFrameStc;
    UCHAR ucB2rId;
    B2R_PRM_T* prFrcPrm = NULL;
    UCHAR ucRet = B2R_OK;
    
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
            LOG(0, "ucB2rId = %d [%s-%d]\n", ucB2rId, __FUNCTION__, __LINE__);
            ucRet = B2R_INV_ARG;
            break;
        }
        prFrcPrm = this->ptB2rPrm;
        if(!prFrcPrm)
        {
            ucRet = B2R_INV_ARG;
            break;
        }

        if(!prFrcPrm->prFbCounter)
        {
            ucRet = B2R_INV_ARG;
            break;
        }
#ifdef CC_FAST_CHANGE_CHANNEL
        if (prFrcPrm->ucFastChangeEn)
        {
            LOG(3, "Fast Stop 2\n");
            prFrcPrm->ucFastChangeEn = 0;
#ifdef CC_FAST_CHANGE_CHANNEL_LOG
            HAL_GetTime(&rNvgtAvSyncTime);
#endif
        }
#endif

        prFrcPrm->prFbCounter->u4SyncMode = FBM_FBG_AVSYNC_NORMAL;
        FBM_SetAVSyncStatus(prFrcPrm->ucFbgId, prFrcPrm->prFbCounter->u4SyncMode);
        FBM_GetSyncStc(prFrcPrm->ucFbgId, &ucAvSyncMode, &ucStcSrc);
        if(!prFrcPrm->t_wfd_prm.fgDblClk)
        {
            u4OutFrameStc = (prFrcPrm->u2TargetNs * prFrcPrm->u4OutStcPeriod);
        }
        else
        {
            u4OutFrameStc = prFrcPrm->u4OutStcPeriod;
        }
        if (u4Delta > u4OutFrameStc)
        {
            if(prFrcPrm->t_wfd_prm.fgDblClk &&
                prFrcPrm->u2TargetNs > 1)
            {
                prFrcPrm->u2TargetNs = 1;
            }
            
            if (u4OutFrameStc != 0)
            {
                prFrcPrm->u4B2rSkipFrameNo = u4Delta / u4OutFrameStc;
            }
            else
            {
                prFrcPrm->u4B2rSkipFrameNo = 0;
            }
            prFrcPrm->prFbCounter->u4SyncMode = FBM_FBG_AVSYNC_DROP;
            FBM_SetAVSyncStatus(prFrcPrm->ucFbgId, prFrcPrm->prFbCounter->u4SyncMode);

#ifndef CC_B2R_DISABLE_ONE_SECOND_FRC
            // protection
            if ((ucAvSyncMode == VID_SYNC_MODE_SLAVE) && (prFrcPrm->u4PtsAvSyncNotAllow))
            {
                ucRet = B2R_FAIL;
            }
#endif
             FBM_SetPtsSync(prFrcPrm->ucFbgId,FALSE);
            if ((u4ZeroPtsNs != 0) && (u4Delta > prFrcPrm->u4PtsDelta) &&
                !prFrcPrm->t_wfd_prm.fgDblClk)
            {
                // ask MPV to keep drop B if NOT Enough
                FBM_SetFrameBufferFlag(prFrcPrm->ucFbgId, FBM_FLAG_SKIP_PIC_NO_PTS);
                LOG(3, "AVS-D VLD Keep Drop B (Zero PTS) B2R(%d)\n", prFrcPrm->ucB2rId);
                prFrcPrm->u4KeepDropB = 1;
            }
            else if (u4Delta > (u4OutFrameStc * VDP_KEEP_DROP_B_NS) &&
                !prFrcPrm->t_wfd_prm.fgDblClk)
            {
                // ask MPV to keep drop B if NOT Enough
                FBM_SetFrameBufferFlag(prFrcPrm->ucFbgId, FBM_FLAG_SKIP_PIC);
#if (defined(CC_3D_MM_DS_SUPPORT) && defined(CC_B2R_ENABLE_R_VIEW_SKIP_FRAME))
                if (prFrcPrm->ucSubFbgId != FBM_FBG_ID_UNKNOWN && FBM_GetFbgAppMode(prFrcPrm->ucSubFbgId) == FBM_FBG_APP_Frame_Pack_3D)
                {
                    FBM_SetFrameBufferFlag(prFrcPrm->ucSubFbgId, FBM_FLAG_SKIP_PIC);
                }
#endif
                LOG(3, "AVS-D VLD Keep Drop B B2R(%d)\n", prFrcPrm->ucB2rId);
                prFrcPrm->u4KeepDropB = 1;
            }

            // protection
#ifndef CC_DCR_TEST
            prFrcPrm->u4PtsAvSyncNotAllow = prFrcPrm->ucInFrameRate;
#endif
            if (FBM_ChkFrameBufferFlag(prFrcPrm->ucFbgId, FBM_FLAG_SKIP_PIC))
            {
                if (!(prFrcPrm->fgB2R3DEnable && prFrcPrm->u1B2R3DType == B2R_3D_FRAME_SEQUENTIAL))
                {
                    // ask MPV to drop if Display Queue is Empty
                    if (FBM_CheckFrameBufferDispQ(prFrcPrm->ucFbgId) != 0)
                    {
                        prFrcPrm->i4PtsDrift -= (INT32)u4OutFrameStc;

                        prFrcPrm->u2ChangeFieldNs = 0;
                        prFrcPrm->u2TargetNs = 0;

                        // [LOG] AVSYNC, V-PTS Drop
                        SYSLOG(SYNC_PREFIX + 30 + prFrcPrm->ucFbgId, prFrcPrm->prFbCounter->u4DispOk, prFrcPrm->prFbCounter->u4DispFail, prFrcPrm->u2TargetNs);
                        LOG(3, "AVS-D B2R(%d) Fbg(%d) PTS(0x%x) STC(0x%x)\n", prFrcPrm->ucB2rId, prFrcPrm->ucFbgId, prFrcPrm->u4Pts, u4Stc);
                    }
                    else
                    {
                        LOG(3, "[Warning] Can not drop at both VDP & MPV\n");
                    }
                }
            }
            else if(!prFrcPrm->t_wfd_prm.fgDblClk)
            {
                UINT32 u4EsmQCnt;

                u4EsmQCnt = _B2R_GetEsmQCnt();

                if (u4EsmQCnt > 1)
                {
                    // ask MPV to drop
                    FBM_SetFrameBufferFlag(prFrcPrm->ucFbgId, FBM_FLAG_SKIP_PIC);
#if (defined(CC_3D_MM_DS_SUPPORT) && defined(CC_B2R_ENABLE_R_VIEW_SKIP_FRAME))
                    if (prFrcPrm->ucSubFbgId != FBM_FBG_ID_UNKNOWN && FBM_GetFbgAppMode(prFrcPrm->ucSubFbgId) == FBM_FBG_APP_Frame_Pack_3D)
                    {
                        FBM_SetFrameBufferFlag(prFrcPrm->ucSubFbgId, FBM_FLAG_SKIP_PIC);
                    }
#endif
                    LOG(3, "AVS-D VLD B2R(%d) Fbg(%d) PTS(0x%x) STC(0x%x) Q(%d)\n", prFrcPrm->ucB2rId, prFrcPrm->ucFbgId, prFrcPrm->u4Pts, u4Stc, u4EsmQCnt);
                }
                else
                {
                    if (FBM_CheckFrameBufferDispQ(prFrcPrm->ucFbgId) != 0)
                    {
                        prFrcPrm->i4PtsDrift -= (INT32)u4OutFrameStc;

                        prFrcPrm->u2ChangeFieldNs = 0;
                        prFrcPrm->u2TargetNs = 0;

                        // [LOG] AVSYNC, V-PTS Drop
                        SYSLOG(SYNC_PREFIX + 30 + prFrcPrm->ucFbgId, prFrcPrm->prFbCounter->u4DispOk, prFrcPrm->prFbCounter->u4DispFail, prFrcPrm->u2TargetNs);
                        LOG(3, "AVS-D B2R(%d) Fbg(%d) PTS(0x%x) STC(0x%x)\n", prFrcPrm->ucB2rId, prFrcPrm->ucFbgId, prFrcPrm->u4Pts, u4Stc);
                    }
                    else
                    {
                        LOG(3, "[Warning] Can not drop at VDP & MPV\n");
                    }
                }
            }

            /* Video Frame drop notification */
            /*_VDP_VsyncNotify(VDP_MSG_REPEAT_DROP_CB, prFrcPrm->ucVdpId,
                             prFrcPrm->prFbCounter->u4SyncMode, 0);*/

            prFrcPrm->prFbCounter->u4DispDrop++;

        }
        else /*u4Delta < u4OutFrameStc*/
        {
            //only consider wrongfield case
            if (prFrcPrm->ucWrongField && (prFrcPrm->u2TargetNs&1))
            {
                prFrcPrm->i4PtsDrift = i4Delta + prFrcPrm->u4OutStcPeriod;
            }
        }
    }while(FALSE);
    
    return ucRet;
}

static void _B2R_AVSyncProc(B2R_OBJECT_T* this)
{
    UCHAR ucAvSyncMode;
    UCHAR ucStcSrc;
    UCHAR ucB2rId;
    B2R_VAR_T* prB2rVar = NULL;
    B2R_PRM_T* prFrcPrm = NULL;
	
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
    prB2rVar = this->ptB2rVar;
    prFrcPrm = this->ptB2rPrm;
    if(!prB2rVar || !prFrcPrm)
    {
        return;
    }

    if(!prFrcPrm->prFbCounter)
    {
        return;
    }
    
    _B2R_PtsCbProc(this, 0);

    // We can do only 1 AVS-R per second
    if (prFrcPrm->u4PtsAvSyncNotAllow > 0)
    {
        prFrcPrm->u4PtsAvSyncNotAllow--;
    }

    if (_B2R_IsSkipAVSync(this))
    {
        FBM_SetPtsSync(prFrcPrm->ucFbgId,FALSE);
        return;
    }

    FBM_GetSyncStc(prFrcPrm->ucFbgId, &ucAvSyncMode, &ucStcSrc);
    if (ucAvSyncMode != VID_SYNC_MODE_NONE)
    {
        STC_CLOCK rStcClk;
        if (STC_GetSrc(ucStcSrc, &rStcClk) == STC_VALID)
        {
            UINT32 u4Delta;
            INT32 i4Delta;
            UINT32 u4OutFrameStc;
            UINT32 u4ZeroPtsNs;

            if(!prFrcPrm->t_wfd_prm.fgDblClk)
            {
                u4OutFrameStc = (prFrcPrm->u2TargetNs * prFrcPrm->u4OutStcPeriod);
            }
            else
            {
                u4OutFrameStc = prFrcPrm->u4OutStcPeriod;
            }

            // The actual display time is 1 Vsync later
            rStcClk.u4Base += prFrcPrm->u4OutStcPeriod;

            // Zero PTS
            u4ZeroPtsNs = prFrcPrm->u4ZeroPtsNs;
            if (
                #ifdef CC_B2R_WAIT_NPTV_STABLE
                //wait for NPTV statble
                _B2R_IsNptvStable(this, ucStcSrc, &rStcClk) && 
                #endif
                _B2R_CheckPtsOk(this, ucAvSyncMode)) 
            {
                //calcus difference between current pts and STC
                _B2R_CalPtsStcDiff(prFrcPrm->u4Pts, rStcClk.u4Base, &u4Delta, &i4Delta);

                LOG_REL(prB2rVar->fgEnableVideoLog, 1, 7, "B2R(%d) PTS %x, STC(%d) %x, Delta (%d, %d), %d\n",
                        prFrcPrm->ucB2rId,
                        prFrcPrm->u4Pts, ucStcSrc, rStcClk.u4Base, i4Delta, prFrcPrm->i4PtsDrift,
                        u4OutFrameStc);
                
#ifdef ENABLE_MULTIMEDIA
                _B2R_RTAVsyncProc(this);
#endif
					
					

					
                if (!_B2R_IsPtsDrift(this, u4Delta, i4Delta, u4ZeroPtsNs, ucAvSyncMode))
                {
                    HAL_GetTime(&(prFrcPrm->rLastAvSyncTime));
                
                    if (i4Delta < 0)
                    {
                        // PTS slower than STC, repeat
                        // [LOG] AVSYNC, V-PTS slower than STC
                        _B2R_ProcFrameRepeat(this, u4Delta, rStcClk.u4Base);
                    }
                    else
                    {
                        // PTS faster than STC, drop
                        // [LOG] AVSYNC, V-PTS faster than STC
                        _B2R_ProcFrameDrop(this, u4Delta, i4Delta, u4ZeroPtsNs, rStcClk.u4Base);
                    }

                    // DTV picture information callback when frame start to display.
                   
                    /*-------------------------------------*/
                    //callback 
                    /*---------------------------------------*/
                }
            }
        }
        else if (prFrcPrm->ucReady == 0)
        {
            if (prFrcPrm->u4PtsAsyncNs < VDP_WAIT_1ST_PTS_NS)
            {
                // pending for the 1st PTS
                _B2R_PreparePendingForNextVsync(this);

                prFrcPrm->u4PtsAsyncNs++;
            }
            else
            {
                // STC still invalid after 12 vsync, forget it, go display!
                LOG(3, "Force Video Start 3\n");
                prFrcPrm->u4PtsAsyncNs = 0;
            }

            prFrcPrm->prFbCounter->u4SyncMode = FBM_FBG_AVSYNC_WAIT_1ST;
            FBM_SetAVSyncStatus(prFrcPrm->ucFbgId, FBM_FBG_AVSYNC_WAIT_1ST);
        }
        else
        {
            prFrcPrm->prFbCounter->u4SyncMode = FBM_FBG_AVSYNC_STC_INVALID;
            FBM_SetAVSyncStatus(prFrcPrm->ucFbgId, FBM_FBG_AVSYNC_STC_INVALID);
        }
    }
    else
    {
        prFrcPrm->prFbCounter->u4SyncMode = FBM_FBG_AVSYNC_DISABLE;
        FBM_SetAVSyncStatus(prFrcPrm->ucFbgId, FBM_FBG_AVSYNC_DISABLE);
    }
}
//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------
UINT32 _B2R_CalculateVsyncNs(B2R_OBJECT_T* this)
{
    if (!this)
    {
        return B2R_INV_ARG;
    }

    _B2R_BaseVSyncNs(this);

    _B2R_FrameRateProc(this);
#ifdef CC_B2R_SUPPORT_GAME_MODE
    if(this->ptB2rPrm &&
        !this->ptB2rPrm->fgGameModeEnable)
#endif
    {
      _B2R_AVSyncProc(this);
    }
    return B2R_OK;
}

#ifdef ENABLE_MULTIMEDIA
/*-----------------------------------------------------------------------------
* Name: _B2R_RTPtsStcDiff
*
* Description: This API Get Audio/Video PTS info from SWDMX.
*              
* Inputs:  prFrcPrm    Specifies the struct of the B2R PRM. Different B2R PRM 
*                       refer to different B2R Id.
*          
*             pt_pts_info  Specifies the PTS info
*
* Outputs: pt_pts_info  Specifies the PTS info
*
* Function: Get SWDMX current AUDIO/VIDEO PTS and Last PTS interval time
----------------------------------------------------------------------------*/
static INT32 _B2R_RTPtsStcDiff(B2R_PRM_T*             prFrcPrm, 
                                    B2R_WFD_PTS_INFO_T*    pt_pts_info)
{
    UINT32  u4SwdmxId  = 0;
    UCHAR   ucAvSyncMode;
    UCHAR   ucStcSrc;
    INT32   i4Ret   = B2R_OK;
    SWDMX_PTS_INFO_T    t_vid_info;
    SWDMX_PTS_INFO_T    t_aud_info;

    if(!prFrcPrm
        || !pt_pts_info)
    {
        return B2R_INV_ARG;
    }

    do
    {
        u4SwdmxId = prFrcPrm->u4AttachedSrcId;
        FBM_GetSyncStc(prFrcPrm->ucFbgId, &ucAvSyncMode, &ucStcSrc);
        if(ucStcSrc >= STC_SRC_NS
            || u4SwdmxId >= SWDMX_SOURCE_MAX)
        {
            i4Ret = B2R_FAIL;
            break;
        }
        
        if(!SWDMX_GetInfo(u4SwdmxId, eSWDMX_GET_VIDEO_PTS_INFO, (UINT32*)&t_vid_info, NULL, NULL))
        {
            i4Ret = B2R_FAIL;
            break;
        }

        if(!SWDMX_GetInfo(u4SwdmxId, eSWDMX_GET_AUDIO_PTS_INFO, (UINT32*)&t_aud_info, NULL, NULL))
        {
            i4Ret = B2R_FAIL;
            break;
        }

        pt_pts_info->u4CurStc        = STC_GetStcValue(ucStcSrc);
        pt_pts_info->u4_vid_pts      = t_vid_info.u4Pts;
        pt_pts_info->i4_vid_diff = pt_pts_info->u4_vid_pts - pt_pts_info->u4CurStc;
        pt_pts_info->u4_aud_pts      = t_aud_info.u4Pts;
        pt_pts_info->i4_aud_diff     = pt_pts_info->u4_aud_pts - pt_pts_info->u4CurStc;
        
        if(t_vid_info.rDeltaTime.u4Micros > t_aud_info.rDeltaTime.u4Micros)
        {
            pt_pts_info->rDeltaTime = t_vid_info.rDeltaTime;
        }
        else
        {
            pt_pts_info->rDeltaTime = t_aud_info.rDeltaTime;
        }
        
    }while(0);

    if(B2R_OK != i4Ret)
    {
        pt_pts_info->u4CurStc        = 0;
        pt_pts_info->u4_vid_pts      = 0;
        pt_pts_info->i4_vid_diff     = 0;
        pt_pts_info->u4_aud_pts      = 0;
        pt_pts_info->i4_aud_diff     = 0;
        x_memset(&pt_pts_info->rDeltaTime,0x0,sizeof(HAL_TIME_T));
    }

    return i4Ret;
}

/*-----------------------------------------------------------------------------
* Name: _B2R_RTVarInitial
*
* Description: This API Initial Wifi Display Related parameter.
*              
* Inputs:  prFrcPrm    Specifies the struct of the B2R PRM. Different B2R PRM 
*                       refer to different B2R Id.
*          
* Outputs: 
*
* Function: Initial Wifi Display Related parameter
----------------------------------------------------------------------------*/
static inline INT32 _B2R_RTVarInitial (B2R_PRM_T*   prFrcPrm)
{
    if(!prFrcPrm)
    {
        return B2R_INV_ARG;
    }

    prFrcPrm->t_wfd_prm.u4_pkt_cnt   = 0;

    if(prFrcPrm->ptRtCli &&
        prFrcPrm->ptRtCli->u4_latency_ns)
    {
        prFrcPrm->t_wfd_prm.fgDnmcltncy  = FALSE;
        prFrcPrm->t_wfd_prm.u4_ltncy_ns  = prFrcPrm->ptRtCli->u4_latency_ns;
        LOG(1,"%s , Disable Dynamic Latency , u4_ltncy_ns : %d!\n",__FUNCTION__,
            prFrcPrm->ptRtCli->u4_latency_ns);
    }
    else
    {
        prFrcPrm->t_wfd_prm.fgDnmcltncy  = TRUE;
        prFrcPrm->t_wfd_prm.u4_ltncy_ns  = VDP_RT_INITIAL_CNT;
        LOG(1,"%s , Enable Dynamic Latency!\n",__FUNCTION__);
    }
    prFrcPrm->t_wfd_prm.u4_ltncy_max = 0;
    prFrcPrm->t_wfd_prm.fgInitalized = TRUE;
    HAL_GetTime(&(prFrcPrm->t_wfd_prm.t_start_time));

    FBM_SetWfdFrameBufferFlag(prFrcPrm->ucFbgId,FBM_WFD_FRAME_MONITOR);
    FBM_ClrWfdFrameBufferFlag(prFrcPrm->ucFbgId,FBM_WFD_FRAME_COUNTING);

    return B2R_OK;
}

/*-----------------------------------------------------------------------------
* Name: _B2R_RTAVsyncInitializing
*
* Description: This API Initial Wifi Display Playback status.
*              
* Inputs:  prFrcPrm    Specifies the struct of the B2R PRM. Different B2R PRM 
*                       refer to different B2R Id.
*
*             pt_pts_info Specifies the PTS info
*
* Outputs: 
*
* Function: Use PTS info ,Wait Video PTS and STC status stable , then Initial WFD paramerer for start to playback
----------------------------------------------------------------------------*/
static INT32 _B2R_RTAVsyncInitializing (B2R_PRM_T*            prFrcPrm,
                                             B2R_WFD_PTS_INFO_T*   pt_pts_info)
{
    UCHAR   ucAvSyncMode;
    UCHAR   ucStcSrc;
    INT32   i4Ret   = B2R_OK;

    if(!prFrcPrm || !pt_pts_info)
    {
        return B2R_INV_ARG;
    }

    do
    {
        FBM_GetSyncStc(prFrcPrm->ucFbgId, &ucAvSyncMode, &ucStcSrc);
        if(ucStcSrc >= STC_SRC_NS)
        {
            i4Ret = B2R_FAIL;
            break;
        }
        
        if(pt_pts_info->i4_vid_diff >= 0
            && pt_pts_info->i4_vid_diff > prFrcPrm->u4OutStcPeriod * VDP_RT_INITIAL_CNT)
        {
            LOG(1,"%s , u4_vid_pts : 0x%x!\n",__FUNCTION__,pt_pts_info->u4_vid_pts);
            STC_SetStcValue(ucStcSrc, pt_pts_info->u4_vid_pts);
        }
        else
        {
            _B2R_RTVarInitial(prFrcPrm);
            LOG(3,"B2R RT Initialize End ......\n");
        }
    }while(0);

    return i4Ret;
}

/*-----------------------------------------------------------------------------
* Name: _VDP_RTStcCorrection
*
* Description: This API Fine tuning STC.
*              
* Inputs:  prFrcPrm    Specifies the struct of the B2R PRM. Different B2R PRM 
*                       refer to different B2R Id.
*
*             pt_pts_info Specifies the PTS info
*
* Outputs: 
*
* Function: Use PTS info ,Fine tuning STC . Keep STC and SWDMX Latest input PTS Diff in range [Latency Time/2 , Latency Time]
----------------------------------------------------------------------------*/
static INT32 _B2R_RTStcCorrection(B2R_PRM_T*            prFrcPrm,
                                          B2R_WFD_PTS_INFO_T*   pt_pts_info)
{
    UINT32  u4Delta = 0;
    UINT32  u4StcAdj;
    INT32   i4Ret = B2R_OK;
    UCHAR   ucAvSyncMode;
    UCHAR   ucStcSrc;
    
    if(!prFrcPrm || !pt_pts_info)
    {
        return B2R_INV_ARG;
    }

    do
    {
        FBM_GetSyncStc(prFrcPrm->ucFbgId, &ucAvSyncMode, &ucStcSrc);
        if(ucStcSrc >= STC_SRC_NS)
        {
            i4Ret = B2R_FAIL;
            break;
        }

        u4Delta = pt_pts_info->i4_vid_diff >= 0?
            pt_pts_info->i4_vid_diff : -(pt_pts_info->i4_vid_diff);

        if(prFrcPrm->t_wfd_prm.t_stc_adj.u4_adj_cnt != 0)
        {
            LOG(1,"%s,stc adj : %d , u4_adj_cnt : %d!\n",__FUNCTION__,
                prFrcPrm->t_wfd_prm.t_stc_adj.i4_ave_diff,prFrcPrm->t_wfd_prm.t_stc_adj.u4_adj_cnt);
            STC_SetStcValue(ucStcSrc,pt_pts_info->u4CurStc + prFrcPrm->t_wfd_prm.t_stc_adj.i4_ave_diff);
            prFrcPrm->t_wfd_prm.t_stc_adj.u4_adj_cnt --;
        }

        if(prFrcPrm->t_wfd_prm.fgInitalized)
        {
            if(!prFrcPrm->t_wfd_prm.fgDnmcltncy)
            {
                prFrcPrm->t_wfd_prm.u4_pkt_cnt ++;
            }
            
            if(prFrcPrm->t_wfd_prm.u4_pkt_cnt % (VDP_RT_STC_ADJ_CNT * VDP_RT_MULTIPLY_THREE) != 0)
            {
                prFrcPrm->t_wfd_prm.t_stc_adj.i4_vid_diff +=
                    pt_pts_info->i4_vid_diff;
                prFrcPrm->t_wfd_prm.t_stc_adj.i4_aud_diff +=
                    pt_pts_info->i4_aud_diff;
                break;
            }
            else
            {

               prFrcPrm->t_wfd_prm.t_stc_adj.i4_ave_diff =
                    ((prFrcPrm->t_wfd_prm.t_stc_adj.i4_vid_diff
                    + prFrcPrm->t_wfd_prm.t_stc_adj.i4_aud_diff)/VDP_RT_DIV_TWO)/(VDP_RT_STC_ADJ_CNT * VDP_RT_MULTIPLY_THREE);

                u4Delta = prFrcPrm->t_wfd_prm.t_stc_adj.i4_ave_diff >= 0 ?
                    prFrcPrm->t_wfd_prm.t_stc_adj.i4_ave_diff : -(prFrcPrm->t_wfd_prm.t_stc_adj.i4_ave_diff);

                u4StcAdj = (prFrcPrm->u4OutStcPeriod - VDP_RT_STC_ADJ)/VDP_RT_DIV_TWO;
                LOG(3,"%s,i4_ave_diff : %d , u4StcAdj : %d!\n",__FUNCTION__,prFrcPrm->t_wfd_prm.t_stc_adj.i4_ave_diff,u4StcAdj);
                if(prFrcPrm->t_wfd_prm.t_stc_adj.i4_ave_diff >= 0)
                {
                    if(u4Delta > prFrcPrm->u4OutStcPeriod * prFrcPrm->t_wfd_prm.u4_ltncy_ns)
                    {
                        u4Delta =
                            u4Delta - prFrcPrm->u4OutStcPeriod * prFrcPrm->t_wfd_prm.u4_ltncy_ns;
                        if(u4Delta > prFrcPrm->u4OutStcPeriod)
                        {
                            prFrcPrm->t_wfd_prm.t_stc_adj.i4_ave_diff = u4Delta/VDP_RT_STC_ADJ_CNT;
                            prFrcPrm->t_wfd_prm.t_stc_adj.u4_adj_cnt = VDP_RT_STC_ADJ_CNT;
                        }
                        else
                        {
                            if(u4Delta != 0)
                            {
                                prFrcPrm->t_wfd_prm.t_stc_adj.i4_ave_diff = u4Delta;
                                prFrcPrm->t_wfd_prm.t_stc_adj.u4_adj_cnt = 1;
                            }
                            else
                            {
                                prFrcPrm->t_wfd_prm.t_stc_adj.u4_adj_cnt = 0;
                            }
                        }
                    }
                    else if(prFrcPrm->t_wfd_prm.u4_ltncy_ns > VDP_RT_LATENCY_AJD_CNT
                        && u4Delta < prFrcPrm->u4OutStcPeriod * prFrcPrm->t_wfd_prm.u4_ltncy_ns/VDP_RT_DIV_TWO)
                    {
                        u4Delta =
                            prFrcPrm->u4OutStcPeriod * prFrcPrm->t_wfd_prm.u4_ltncy_ns/VDP_RT_DIV_TWO - u4Delta;
                        if(u4Delta > prFrcPrm->u4OutStcPeriod)
                        {
                            UINT32 u4_ave_delta;

                            u4_ave_delta = u4Delta/(VDP_RT_STC_ADJ_CNT * VDP_RT_MULTIPLY_THREE);
                            if(u4_ave_delta <= u4StcAdj)
                            {
                                prFrcPrm->t_wfd_prm.t_stc_adj.i4_ave_diff = (-1) * (INT32)u4StcAdj;
                                if(u4StcAdj)
                                {
                                    prFrcPrm->t_wfd_prm.t_stc_adj.u4_adj_cnt = u4Delta / u4StcAdj;
                                }
                            }
                            else
                            {
                                prFrcPrm->t_wfd_prm.t_stc_adj.i4_ave_diff = (-1) * (INT32)u4_ave_delta;
                                prFrcPrm->t_wfd_prm.t_stc_adj.u4_adj_cnt = (VDP_RT_STC_ADJ_CNT * VDP_RT_MULTIPLY_THREE);
                            }
                        }
                        else
                        {
                            if(u4Delta != 0)
                            {
                                prFrcPrm->t_wfd_prm.t_stc_adj.i4_ave_diff = (-1) * (INT32)u4Delta;
                                prFrcPrm->t_wfd_prm.t_stc_adj.u4_adj_cnt = 1;
                            }
                            else
                            {
                                prFrcPrm->t_wfd_prm.t_stc_adj.u4_adj_cnt = 0;
                            }
                        }
                    }
                }
                else
                {      
                    if(u4Delta > prFrcPrm->u4OutStcPeriod)
                    {
                        UINT32 u4_ave_delta;

                        u4_ave_delta = u4Delta/(VDP_RT_STC_ADJ_CNT * VDP_RT_MULTIPLY_THREE);
                        if(u4_ave_delta <= u4StcAdj)
                        {
                            prFrcPrm->t_wfd_prm.t_stc_adj.i4_ave_diff = (-1) * (INT32)u4StcAdj;
                            if(u4StcAdj)
                            {
                                prFrcPrm->t_wfd_prm.t_stc_adj.u4_adj_cnt = u4Delta / u4StcAdj;
                            }
                        }
                        else
                        {
                            prFrcPrm->t_wfd_prm.t_stc_adj.i4_ave_diff = (-1) * (INT32)u4_ave_delta;
                            prFrcPrm->t_wfd_prm.t_stc_adj.u4_adj_cnt = (VDP_RT_STC_ADJ_CNT * VDP_RT_MULTIPLY_THREE);
                        }
                    }
                    else
                    {
                        prFrcPrm->t_wfd_prm.t_stc_adj.u4_adj_cnt = 1;
                    }
                }
                prFrcPrm->t_wfd_prm.t_stc_adj.i4_vid_diff = 0;
                prFrcPrm->t_wfd_prm.t_stc_adj.i4_aud_diff = 0;
            }
        }
        
    }while(0);
    
    return i4Ret;
}

/*-----------------------------------------------------------------------------
* Name: _B2R_MM1Calculation
*
* Description: This API Adjust STC using MM1 Algorithm.
*              
* Inputs:  prFrcPrm    Specifies the struct of the B2R PRM. Different B2R PRM 
*                       refer to different B2R Id.
*
*             u4_pic_cnt  Picture Count in 200 ms
*
*             pt_pts_info  Specifies the PTS info
*
* Outputs: 
*
* Function: Use MM1 algorithm to calculation average Latency From Vdec to B2R
*               If Average Latency larger Than defined Latency , We should Adjust STC.
----------------------------------------------------------------------------*/
static INT32 _B2R_MM1Calculation(B2R_PRM_T*           prFrcPrm, 
                                       UINT32               u4_pic_cnt,
                                       B2R_WFD_PTS_INFO_T*  pt_pts_info)
{
    UINT32 u4_arrival_speed = 0;
    UINT32 u4_serve_speed = 0;
    UINT32 u4_ave_wait_time = VDP_RT_LATENCY_TIME;
    UINT32 u4CurStc     = 0;
    UINT32 u4_framerate = 0;
    UINT32 u4_frame_count = 0;
    UINT32 u4_stc_adj     = 0;
    UCHAR  ucStcSrc     = 0;
    UCHAR  ucAvSyncMode = 0;
    INT32  i4Ret   = B2R_OK;
    
    if(!prFrcPrm || !pt_pts_info)
    {
        LOG(1, "[%s-%d]return %d.\n", __func__, __LINE__);
        return B2R_INV_ARG;
    }

    do
    {
        FBM_GetSyncStc(prFrcPrm->ucFbgId, &ucAvSyncMode, &ucStcSrc);
        if(ucStcSrc >= STC_SRC_NS)
        {
            i4Ret = B2R_FAIL;
            break;
        }

        u4_framerate = prFrcPrm->t_wfd_prm.fgDblClk ?
                        prFrcPrm->ucInFrameRate/VDP_RT_DIV_TWO : prFrcPrm->ucInFrameRate;
        u4_frame_count = u4_framerate/VDP_STATISTICS_PER_SECOND;
        u4_arrival_speed = u4_pic_cnt * VDP_STATISTICS_PER_SECOND;
        u4_serve_speed = prFrcPrm->ucOutFrameRate;

        if(prFrcPrm->t_wfd_prm.fgDblClk
            && 0 != u4_arrival_speed)
        {
            if(u4_serve_speed >= u4_arrival_speed)
            {
                if(u4_arrival_speed)
                {
                    u4_ave_wait_time = u4_serve_speed * (u4_serve_speed - u4_arrival_speed) / u4_arrival_speed;
                }
            }
            else
            {
                u4_ave_wait_time = VDP_RT_LATENCY_TIME - 1;
            }
        }

        //LOG(2,"u4_ave_wait_time : %d!\n",u4_ave_wait_time);
        if(((prFrcPrm->t_wfd_prm.fgDblClk
            && u4_ave_wait_time < VDP_RT_LATENCY_TIME)
            ||(!prFrcPrm->t_wfd_prm.fgDblClk
            && u4_pic_cnt > u4_frame_count))
            && FBM_CheckFrameBufferDispQ(prFrcPrm->ucFbgId))
        {
            u4CurStc = STC_GetStcValue(ucStcSrc);
            if(u4_pic_cnt > u4_frame_count)
            {
                u4_stc_adj = (u4_pic_cnt - u4_frame_count) * prFrcPrm->u4OutStcPeriod;
                if(u4CurStc + u4_stc_adj > pt_pts_info->u4_vid_pts - prFrcPrm->u4OutStcPeriod * prFrcPrm->t_wfd_prm.u4_ltncy_ns)
                {
                    break;
                }
                LOG(1,"%s adj STC : %d, u4_pic_cnt : %d!\n",__FUNCTION__,u4_stc_adj,u4_pic_cnt);
                STC_SetStcValue(ucStcSrc, u4CurStc + u4_stc_adj);
            }
            else
            {
                u4_stc_adj = prFrcPrm->u4OutStcPeriod;
                if(u4CurStc + u4_stc_adj > pt_pts_info->u4_vid_pts - prFrcPrm->u4OutStcPeriod * prFrcPrm->t_wfd_prm.u4_ltncy_ns)
                {
                    break;
                }
                LOG(1,"%s adj STC : %d, u4_pic_cnt : %d!\n",__FUNCTION__,u4_stc_adj,u4_pic_cnt);
                STC_SetStcValue(ucStcSrc, u4CurStc + u4_stc_adj);
            }
        }
    }while(0);
    
    return i4Ret;
}

/*-----------------------------------------------------------------------------
* Name: _B2R_RTStcAdjust
*
* Description: This API use for Adjust STC.
*              
* Inputs:  prFrcPrm    Specifies the struct of the B2R PRM. Different B2R PRM 
*                       refer to different B2R Id.
*
*             pt_pts_info  Specifies the PTS info
*
* Outputs: 
*
* Function: Frame Count Statistics 
----------------------------------------------------------------------------*/
static INT32 _B2R_RTStcAdjust(B2R_PRM_T*             prFrcPrm,
                                     B2R_WFD_PTS_INFO_T*    pt_pts_info)
{
    INT32   i4Ret   = B2R_OK;

    do
    {
        HAL_TIME_T rTime;
        HAL_TIME_T rDeltaTime;
        UINT32     u4_pic_cnt;
        UINT32     u4_time_interval = 0;

        if(!prFrcPrm || !pt_pts_info)
        {
            i4Ret = B2R_INV_ARG;
            break;
        }
        
        if(!FBM_ChkWfdFrameBufferFlag(prFrcPrm->ucFbgId,FBM_WFD_FRAME_MONITOR))
        {
            i4Ret = B2R_INTERNAL_ERR;
            break;
        }

        HAL_GetTime(&rTime);
        HAL_GetDeltaTime(&rDeltaTime, &(prFrcPrm->t_wfd_prm.t_start_time), &rTime);
        if(rDeltaTime.u4Seconds != 0)
        {
            HAL_GetTime(&(prFrcPrm->t_wfd_prm.t_start_time));
            if(FBM_ChkWfdFrameBufferFlag(prFrcPrm->ucFbgId,FBM_WFD_FRAME_COUNTING))
            {
                FBM_ClrWfdFrameBufferFlag(prFrcPrm->ucFbgId,FBM_WFD_FRAME_COUNTING);
            }
            i4Ret = B2R_INTERNAL_ERR;
            break;
        }
        else                       
        {
            u4_time_interval = (VDP_MICROSECOND_PER_SECOND/VDP_STATISTICS_PER_SECOND);
            if(rDeltaTime.u4Micros >= u4_time_interval)
            {
                if(FBM_ChkWfdFrameBufferFlag(prFrcPrm->ucFbgId,FBM_WFD_FRAME_COUNTING))
                {
                    UINT32 u4_count = 0;

                    if(u4_time_interval)
                    {
                        u4_count = rDeltaTime.u4Micros / u4_time_interval;
                    }
                    u4_pic_cnt = FBM_WfdFrameStatistics(prFrcPrm->ucFbgId);
                    FBM_ClrWfdFrameBufferFlag(prFrcPrm->ucFbgId,FBM_WFD_FRAME_COUNTING);
                    HAL_GetTime(&(prFrcPrm->t_wfd_prm.t_start_time));
                    if(u4_count != 1)
                    {
                        break;
                    }
                    _B2R_MM1Calculation(prFrcPrm,u4_pic_cnt,pt_pts_info);
                }
            }
        }
                
    }while(0);

    return i4Ret;
}

/*-----------------------------------------------------------------------------
* Name: _B2R_RTLatencyAdjust
*
* Description: This API Dynamic Adjust Latency.
*              
* Inputs:  prFrcPrm    Specifies the struct of the B2R PRM. Different B2R PRM 
*                       refer to different B2R Id.
*
*             pt_pts_info  Specifies the PTS info
*
* Outputs: 
*
* Function: IF Double Clock Enable , we use Adjacent Packet Time Interval to adjust Latency 
----------------------------------------------------------------------------*/
static INT32 _B2R_RTLatencyAdjust(B2R_PRM_T*             prFrcPrm,
                                          B2R_WFD_PTS_INFO_T*    pt_pts_info)
{
    UINT32  u4_vsync_ns;
    INT32   i4Ret   = B2R_OK;
    
    if(!prFrcPrm
        || !pt_pts_info)
    {
        return B2R_INV_ARG;
    }

    do
    {
        if(prFrcPrm->u4OutStcPeriod)
        {
            u4_vsync_ns = 
                pt_pts_info->rDeltaTime.u4Micros / (prFrcPrm->u4OutStcPeriod * 10);
        }
        
        if(prFrcPrm->t_wfd_prm.u4_pkt_cnt < VDP_RT_STATICE_CNT)
        {
            if(prFrcPrm->t_wfd_prm.u4_ltncy_max < u4_vsync_ns)
            {
                prFrcPrm->t_wfd_prm.u4_ltncy_max = u4_vsync_ns;
            }
            prFrcPrm->t_wfd_prm.u4_pkt_cnt ++;
            break;
        }

        if(prFrcPrm->t_wfd_prm.u4_ltncy_ns <= prFrcPrm->t_wfd_prm.u4_ltncy_max)
        {
            prFrcPrm->t_wfd_prm.u4_ltncy_max -- ;
            prFrcPrm->t_wfd_prm.u4_ltncy_ns = prFrcPrm->t_wfd_prm.u4_ltncy_max;
        }
        else
        {
            UINT32 u4_ltncy_adj = 0;
            u4_ltncy_adj =
                (prFrcPrm->t_wfd_prm.u4_ltncy_ns - prFrcPrm->t_wfd_prm.u4_ltncy_max) / VDP_RT_DIV_TWO;
            prFrcPrm->t_wfd_prm.u4_ltncy_ns -= u4_ltncy_adj;
        }
        prFrcPrm->t_wfd_prm.u4_pkt_cnt = 0;
        prFrcPrm->t_wfd_prm.u4_ltncy_max = 0;
        if(prFrcPrm->t_wfd_prm.u4_ltncy_ns > VDP_RT_LATENCY_TOLERATE)
        {
            prFrcPrm->t_wfd_prm.u4_ltncy_ns -= VDP_RT_LATENCY_TOLERATE;
        }
        if(prFrcPrm->t_wfd_prm.u4_ltncy_ns > VDP_RT_LATENCY_LIMIT)
        {
            prFrcPrm->t_wfd_prm.u4_ltncy_ns = VDP_RT_LATENCY_LIMIT;
        }
        LOG(3,"%s,B2r latency adj : %d!\n",__FUNCTION__,prFrcPrm->t_wfd_prm.u4_ltncy_ns);
    }while(0);
    return i4Ret;
}

/*-----------------------------------------------------------------------------
* Name: _B2R_RTAVsyncProc
*
* Description: This API Is Main Loop Of Wifi Display A/V Sync Procedure.
*              
* Inputs:  prFrcPrm    Specifies the struct of the B2R PRM. Different B2R PRM 
*                       refer to different B2R Id.
*
* Outputs: 
*
* Function: Main Loop Of WFD A/V Sync , Adjust STC to Keep Latency.
----------------------------------------------------------------------------*/
static INT32 _B2R_RTAVsyncProc(B2R_OBJECT_T* this)
{
    INT32       i4Ret    = B2R_OK;
    B2R_PRM_T*  prFrcPrm = NULL;
    B2R_WFD_PTS_INFO_T  t_pts_info;

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
        
        if(_B2R_GetContentSourceType(this) != (UINT32)SWDMX_SRC_TYPE_WIFI_DISPLAY)
        {
            break;
        }
        
        i4Ret = _B2R_RTPtsStcDiff(prFrcPrm,&t_pts_info);
        if(B2R_OK != i4Ret)
        {
            break;
        }
        
        if(!prFrcPrm->t_wfd_prm.fgInitalized)
        {
            i4Ret = _B2R_RTAVsyncInitializing(prFrcPrm,&t_pts_info);
            break;
        }

        if(prFrcPrm->t_wfd_prm.fgDnmcltncy)
        {
            i4Ret = _B2R_RTLatencyAdjust(prFrcPrm,&t_pts_info);
            if(B2R_OK != i4Ret)
            {
                break;
            }
        }
        
        i4Ret = _B2R_RTStcAdjust(prFrcPrm,&t_pts_info);
        if(B2R_OK != i4Ret)
        {
            break;
        }
        
        i4Ret = _B2R_RTStcCorrection(prFrcPrm,&t_pts_info);
        if(B2R_OK != i4Ret)
        {
            break;
        }

    }while(0);

    return i4Ret;
}
#endif

