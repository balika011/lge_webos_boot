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
 *-----------------------------------------------------------------------------
 * $Author: dtvbm11 $
 * $Date: 2015/01/09 $
 * $RCSfile: vdec_esm.c,v $
 * $Revision: #1 $
 *---------------------------------------------------------------------------*/

/** @file vdec_esm.c
 *  This file contains ES manager of MPV. 
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#include "vdec_drvif.h"
#include "mpv_drvif.h"
#include "vdec_debug.h"
#include "drvcust_if.h"


#ifdef CC_VDEC_RM_SUPPORT


#ifdef VDEC_RM_FLOW_TRACE
#define LOG_TRACE   LOG 
#else
#define LOG_TRACE(smt...)  ((VOID)0)
#endif
//-----------------------------------------------------------------------------
// Define functions
//-----------------------------------------------------------------------------

#define IS_VLD_ES_CONNECT(ucVldId, ucEsId) \
        ((prVldInfo[ucVldId].u4ConnEsIdMask & (0x01<<(ucEsId))) && \
         (prVldInfo[ucVldId].au1EsLoading[ucEsId]))
        
#define VLD_ES_CONNECT(ucVldId, ucEsId, ucPercent) \
        {\
            prVldInfo[ucVldId].u4ConnEsIdMask |= (0x01<<(ucEsId)); \
            prVldInfo[ucVldId].u1TotalLoading += (ucPercent);\
            prVldInfo[ucVldId].au1EsLoading[ucEsId] = (ucPercent);\
        }
        
#define VLD_ES_DISCONNECT(ucVldId, ucEsId) \
        {\
            prVldInfo[ucVldId].u4ConnEsIdMask &= ~(0x01<<(ucEsId)); \
            prVldInfo[ucVldId].u1TotalLoading -= prVldInfo[ucVldId].au1EsLoading[(ucEsId)];\
            prVldInfo[ucVldId].au1EsLoading[ucEsId] = 0;\
        }
            
//Is VLD still free for extra percent loading
#define IS_VLD_FREE_FOR(ucVldId, ucPercent) \
        (prVldInfo[ucVldId].u1TotalLoading + (ucPercent) <= 100)

//Make/unMake VLD Mask by VLD id. 
// if core0 only, vldMask = 0x01; 
// if core1 only, vldMask = 0x02; 
// if dual core, vld Mask = 0x03;
#define VLD_MAKE_VLD_MASK(ucVldId, ucVld2Id) \
        ((((ucVldId != VDEC_MAX_VLD) ? 1 : 0) << ucVldId) & \
        (((ucVld2Id != VDEC_MAX_VLD) ? 1 : 0) << ucVld2Id))


#define VLD_UNMAKE_VLD_MASK(ucVldMsk, ucVldId, ucVld2Id) \
        {\
            ucVldId = ((ucVldMsk&0x01) ? VLD0 : ((ucVldMsk&0x02) ? VLD1 : VDEC_MAX_VLD));\
            ucVld2Id = (((ucVldMsk&0x03) == 0x03) ? VLD1 : VDEC_MAX_VLD);\
        }


#ifndef NDEBUG  //Use timeout instead while DEBUG version.

#define VDEC_RES_MNGR_LOCK(prVdecInfo)  \
    if (x_sema_lock_timeout(prVdecInfo->hVldInfoSemaphore, 20000) == OSR_TIMEOUT)\
    {\
        LOG(0, "[%s]lock_timeout(hVldInfoSemaphore).\n", __func__);\
    }

#else

#define VDEC_RES_MNGR_LOCK(prVdecInfo)  \
    (VERIFY(x_sema_lock(prVdecInfo->hVldInfoSemaphore, X_SEMA_OPTION_WAIT) == OSR_OK))

#endif

#define VDEC_RES_MNGR_UNLOCK(prVdecInfo)  \
    (VERIFY(x_sema_unlock(prVdecInfo->hVldInfoSemaphore) == OSR_OK))


#define VDEC_RES_P4_EQ(ptP4Info1, ptP4Info2)    \
    ((ptP4Info1->u4Core0Fps == ptP4Info2->u4Core0Fps) && \
     (ptP4Info1->u4Core1Fps == ptP4Info2->u4Core1Fps) && \
     (ptP4Info1->u4DCoreFps == ptP4Info2->u4DCoreFps) && \
     (ptP4Info1->uc1stChoice == ptP4Info2->uc1stChoice))

#define VA_MSK_END  ((int)0xED)

//-----------------------------------------------------------------------------
// Static Value
//-----------------------------------------------------------------------------

#ifdef CC_MAPLE_CUST_DRV
// Timing performance table
static VDEC_RMNGR_TIM_T arVdecP4Info[]={
    //Name                      Fmt  Timing Core0_T  Core1_T DCore_T  1st_Choice
    {"H264/H265/VP9 4K2K",    0,  0,      0,    0,       60,     2},
    {"H264/H265/VP9 4K1K",    0,  0,      0,    0,       60,     2},
    {"H264/H265/VP9 FHD",     0,  0,    120,  120,      240,     2},
    {"H264/H265/VP9 720",     0,  0,    120,  120,      240,     1},
    {"H264/H265/VP9 <720",    0,  0,    120,  120,      240,     1},
    {"MPEG2 FHD",             0,  0,     60,   60,        0,     1},
    {"MPEG2 720",             0,  0,     60,   60,        0,     1},
    {"MPEG2 <720",            0,  0,     60,   60,        0,     1},
};

// Default Timing performance table for not define case above
static VDEC_RMNGR_TIM_T arVdecP4DefInfo[]={
    //Name                      Fmt  Timing Core0_T  Core1_T DCore_T  1st_Choice
    {"Default 4K2K",      0,  0,      0,    0,        0,     1},
    {"Default 4K1K",      0,  0,      0,    0,        0,     1},
    {"Default FHD",       0,  0,     60,    0,       60,     2},
    {"Default 720",       0,  0,     60,    0,       60,     2},
    {"Default <720",      0,  0,     60,    0,       60,     2},
};
#else //For turnkey

// Timing performance table
static VDEC_RMNGR_TIM_T arVdecP4Info[]={
    //Name                      Fmt  Timing Core0_T  Core1_T DCore_T  1st_Choice
    {"H264/H265/VP9 4K2K",    0,  0,      0,    0,       60,     2},
    {"H264/H265/VP9 4K1K",    0,  0,      0,    0,       60,     2},
    {"H264/H265/VP9 FHD",     0,  0,    120,  120,      240,     2},
    {"H264/H265/VP9 720",     0,  0,    120,  120,      240,     1},
    {"H264/H265/VP9 <720",    0,  0,    120,  120,      240,     1},
    {"MPEG2 FHD",             0,  0,     80,   80,        0,     1},
    {"MPEG2 720",             0,  0,    100,  100,        0,     1},
    {"MPEG2 <720",            0,  0,    100,  100,        0,     1},
};

// Default Timing performance table for not define case above
static VDEC_RMNGR_TIM_T arVdecP4DefInfo[]={
    //Name                      Fmt  Timing Core0_T  Core1_T DCore_T  1st_Choice
    {"Default 4K2K",      0,  0,      0,    0,        0,     1},
    {"Default 4K1K",      0,  0,      0,    0,        0,     1},
    {"Default FHD",       0,  0,     80,    0,       80,     2},
    {"Default 720",       0,  0,    100,    0,      100,     2},
    {"Default <720",      0,  0,    100,    0,      100,     2},
};

#endif
//-----------------------------------------------------------------------------
// Golbal value
//-----------------------------------------------------------------------------
EXTERN VDEC_INFO_T _rVdecInfo;
static VDEC_INFO_T *prVdecInfo = &_rVdecInfo;
static VDEC_VLD_INFO_T *prVldInfo = &(_rVdecInfo.arVdecVldInfo[0]);


//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------

//Must end with VA_MSK_END!
static UINT32 u4VAMaskFct(int u1Value, ...)
{
    int u1Type = u1Value;
    VA_LIST t_arg;
    UINT32 u4Mask = 0x0;

    VA_START(t_arg, u1Value);
    while(u1Type != VA_MSK_END)
    {
        u4Mask |= (1<<u1Type);
        u1Type = VA_ARG(t_arg, int);
    }
    VA_END(t_arg);

    return u4Mask;
}

#if (VDEC_MAX_VLD > 2)

static VOID _VDEC_VLDMngrInitP4Table(VOID)
{
    UINT8 u1Idx = 0;
    
    //{"H264/H265 4K2K",
    arVdecP4Info[u1Idx].u4FmtMsk = u4VAMaskFct(VDEC_FMT_H264, VDEC_FMT_H265, VDEC_FMT_VP9, VA_MSK_END);
    arVdecP4Info[u1Idx].u4TimMsk = u4VAMaskFct(FBM_FBG_TYPE_4K2K, VA_MSK_END);
    u1Idx++;
    
    //{"H264/H265 4K1K"
    arVdecP4Info[u1Idx].u4FmtMsk = u4VAMaskFct(VDEC_FMT_H264, VDEC_FMT_H265, VDEC_FMT_VP9, VA_MSK_END);
    arVdecP4Info[u1Idx].u4TimMsk = u4VAMaskFct(FBM_FBG_TYPE_SBSHD, FBM_FBG_TYPE_TNBHD, FBM_FBG_TYPE_4K1K_VIDEO, VA_MSK_END);
    u1Idx++;

    //{"H264/H265 FHD", 
    arVdecP4Info[u1Idx].u4FmtMsk = u4VAMaskFct(VDEC_FMT_H264, VDEC_FMT_H265, VDEC_FMT_VP9, VA_MSK_END);
    arVdecP4Info[u1Idx].u4TimMsk = u4VAMaskFct(FBM_FBG_TYPE_1080HD, FBM_FBG_TYPE_1080HD_RR, VA_MSK_END);
    u1Idx++;
    
    //{"H264/H265 720", 
    arVdecP4Info[u1Idx].u4FmtMsk = u4VAMaskFct(VDEC_FMT_H264, VDEC_FMT_H265, VDEC_FMT_VP9, VA_MSK_END);
    arVdecP4Info[u1Idx].u4TimMsk = u4VAMaskFct(FBM_FBG_TYPE_720HD, FBM_FBG_TYPE_720HD_RR, VA_MSK_END);
    u1Idx++;
    
    //{"H264/H265 <720",
    arVdecP4Info[u1Idx].u4FmtMsk = u4VAMaskFct(VDEC_FMT_H264, VDEC_FMT_H265, VDEC_FMT_VP9, VA_MSK_END);
    arVdecP4Info[u1Idx].u4TimMsk = u4VAMaskFct(FBM_FBG_TYPE_PAL, FBM_FBG_TYPE_NTSC, VA_MSK_END);
    u1Idx++;
    
    //{"MPEG2 FHD",     
    arVdecP4Info[u1Idx].u4FmtMsk = u4VAMaskFct(VDEC_FMT_MPV, VA_MSK_END);
    arVdecP4Info[u1Idx].u4TimMsk = u4VAMaskFct(FBM_FBG_TYPE_1080HD, FBM_FBG_TYPE_1080HD_RR, VA_MSK_END);
    u1Idx++;
    
    //{"MPEG2 720",     
    arVdecP4Info[u1Idx].u4FmtMsk = u4VAMaskFct(VDEC_FMT_MPV, VA_MSK_END);
    arVdecP4Info[u1Idx].u4TimMsk = u4VAMaskFct(FBM_FBG_TYPE_720HD, FBM_FBG_TYPE_720HD_RR, VA_MSK_END);
    u1Idx++;
    
    //{"MPEG2 <720"
    arVdecP4Info[u1Idx].u4FmtMsk = u4VAMaskFct(VDEC_FMT_MPV, VA_MSK_END);
    arVdecP4Info[u1Idx].u4TimMsk = u4VAMaskFct(FBM_FBG_TYPE_PAL, FBM_FBG_TYPE_NTSC, VA_MSK_END);
    u1Idx++;
    
    ASSERT(u1Idx == (sizeof(arVdecP4Info)/sizeof(VDEC_RMNGR_TIM_T)));

    //1 //Default setting below
    
    u1Idx = 0;
    //{"Default 4K2K"
    arVdecP4DefInfo[u1Idx].u4FmtMsk = u4VAMaskFct(VA_MSK_END);
    arVdecP4DefInfo[u1Idx].u4TimMsk = u4VAMaskFct(FBM_FBG_TYPE_4K2K, VA_MSK_END);
    u1Idx++;
    
    //{"Default 4K1K"
    arVdecP4DefInfo[u1Idx].u4FmtMsk = u4VAMaskFct(VA_MSK_END);
    arVdecP4DefInfo[u1Idx].u4TimMsk = u4VAMaskFct(FBM_FBG_TYPE_SBSHD, FBM_FBG_TYPE_TNBHD, FBM_FBG_TYPE_4K1K_VIDEO, VA_MSK_END);
    u1Idx++;
    
    //{"Default FHD",    
    arVdecP4DefInfo[u1Idx].u4FmtMsk = u4VAMaskFct(VA_MSK_END);
    arVdecP4DefInfo[u1Idx].u4TimMsk = u4VAMaskFct(FBM_FBG_TYPE_1080HD, FBM_FBG_TYPE_1080HD_RR, VA_MSK_END);
    u1Idx++;
    
    //{"Default 720",    
    arVdecP4DefInfo[u1Idx].u4FmtMsk = u4VAMaskFct(VA_MSK_END);
    arVdecP4DefInfo[u1Idx].u4TimMsk = u4VAMaskFct(FBM_FBG_TYPE_720HD, FBM_FBG_TYPE_720HD_RR, VA_MSK_END);
    u1Idx++;
    
    //{"Default <720",    
    arVdecP4DefInfo[u1Idx].u4FmtMsk = u4VAMaskFct(VA_MSK_END);
    arVdecP4DefInfo[u1Idx].u4TimMsk = u4VAMaskFct(FBM_FBG_TYPE_PAL, FBM_FBG_TYPE_NTSC, VA_MSK_END);
    u1Idx++;
    
    ASSERT(u1Idx == (sizeof(arVdecP4DefInfo)/sizeof(VDEC_RMNGR_TIM_T)));

}

static VDEC_RMNGR_TIM_T* _VDEC_VLDMngrP4Get(ENUM_VDEC_FMT_T eCurFMT, UINT32 u4Width, UINT32 u4Height)
{
    UCHAR ucTimeType = 0;
    UINT8 u1Idx = 0;
    UINT8 u1InfoIdx = (UINT8)(-1);
    BOOL  fgFind = FALSE;
    BOOL  fgDefFind = FALSE;
    static BOOL fgTableInit = FALSE;

    VDEC_RES_MNGR_LOCK(prVdecInfo);
    if (!fgTableInit)
    {
        _VDEC_VLDMngrInitP4Table();
        fgTableInit = TRUE;
    }

    ucTimeType = FBM_SelectGroupType(u4Width, u4Height);

    if (ucTimeType == FBM_FBG_TYPE_UNKNOWN)
    {
        VDEC_RES_MNGR_UNLOCK(prVdecInfo);
        return NULL;
    }

    for (u1Idx=0; u1Idx < (sizeof(arVdecP4Info)/sizeof(VDEC_RMNGR_TIM_T)); u1Idx++)
    {
        if ((arVdecP4Info[u1Idx].u4FmtMsk & (1<<eCurFMT)) && 
            (arVdecP4Info[u1Idx].u4TimMsk & (1<<ucTimeType)))
        {
            fgFind = TRUE;
            u1InfoIdx = u1Idx;
            break;
        }
    }

    //Default setting
    if (!fgFind)
    {
        for (u1Idx=0; u1Idx < (sizeof(arVdecP4DefInfo)/sizeof(VDEC_RMNGR_TIM_T)); u1Idx++)
        {
            if (arVdecP4DefInfo[u1Idx].u4TimMsk & (1<<ucTimeType))
            {
                u1InfoIdx = u1Idx;
                fgDefFind = TRUE;
                break;
            }
        }
    }

    ASSERT(fgFind || fgDefFind);

    VDEC_RES_MNGR_UNLOCK(prVdecInfo);
    return fgFind ? &arVdecP4Info[u1InfoIdx] : &arVdecP4DefInfo[u1InfoIdx];
}


/**
 * Alloc Vld by precent request
 * 
 * @Input: 
 * @ucEsId: the Es Id.
 * @pau1CorePercent: precent requested by each core.
 * @paucReservedEsId: If those Es released, can vld alloced for the new one?
 * @ucRsrdNum: The reserved Es Id numbers.
 * @return:
 * @TRUE/FALSE for OK and NG.
 */

static BOOL _VDEC_VLDAllocByPer(UCHAR ucEsId, VDEC_VLDM_OUTPUT_T *ptPctInfo,
                                        UCHAR* paucReservedEsId, UCHAR ucRsrdNum)
{    
    BOOL fgSuccess = TRUE;
    UINT8 au1CorePercent[VDEC_MAX_VLD];
    UINT32 u4VldId, u4Vld2Id;
    VDEC_ES_INFO_T *prVdecEsInfo = NULL;
    
    //Tmp struct to save the vld info.
    VDEC_VLD_INFO_T arVdecVldInfo[VDEC_MAX_VLD];
    VDEC_VLD_INFO_T *prVldInfo = &arVdecVldInfo[0];

    u4VldId = u4Vld2Id = VDEC_MAX_VLD;
    x_memset(au1CorePercent, 0, sizeof(au1CorePercent));

    if ((ucEsId >= VDEC_MAX_ES) || !ptPctInfo)
    {
        LOG(0, "ucEsId %d >= VDEC_MAX_ES or ptPctInfo == NULL.\n", ucEsId);
        ASSERT(ucEsId < VDEC_MAX_ES);
        return FALSE;
    }

    prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);
    if (!prVdecEsInfo)
    {
        LOG(0, "ucEsId fail\n");
        return FALSE;
    }

    VDEC_RES_MNGR_LOCK(prVdecInfo);
    au1CorePercent[VLD0] = ptPctInfo->ucVLD0Percent;
    au1CorePercent[VLD1] = ptPctInfo->ucVLD1Percent;

    do {
        //Copy the Vld Info first.
        x_memcpy(arVdecVldInfo, prVdecInfo->arVdecVldInfo, sizeof(VDEC_VLD_INFO_T)*VDEC_MAX_VLD);

        // Simulate disconnect the reserved Es first.
        if ((ucRsrdNum > 0) && paucReservedEsId)
        {
            INT8 i1Idx = 0;
            for (i1Idx = 0; i1Idx < ucRsrdNum; i1Idx++)
            {
                VDEC_ES_INFO_T *prTmpVdecEsInfo = NULL;
                UCHAR ucEsId = paucReservedEsId[i1Idx];
                
                prTmpVdecEsInfo = _VDEC_GetEsInfo(ucEsId);
                
                if (prTmpVdecEsInfo->u4VldId != VDEC_MAX_VLD)
                {
                    if (!IS_VLD_ES_CONNECT(prTmpVdecEsInfo->u4VldId, ucEsId))
                    {
                        LOG_TRACE(0, "Free Fail: VldId(%d) EsId(%d) not connect.\n", prTmpVdecEsInfo->u4VldId, ucEsId);
                        ASSERT(IS_VLD_ES_CONNECT(prTmpVdecEsInfo->u4VldId, ucEsId));
                    }
                    LOG_TRACE(0, "Tmp Disconnect: VldId(%d) EsId(%d).\n", prTmpVdecEsInfo->u4VldId, ucEsId);
                    VLD_ES_DISCONNECT(prTmpVdecEsInfo->u4VldId, ucEsId);
                }
                
                if (prTmpVdecEsInfo->u4Vld2Id != VDEC_MAX_VLD)
                {
                    if (!IS_VLD_ES_CONNECT(prTmpVdecEsInfo->u4Vld2Id, ucEsId))
                    {
                        LOG_TRACE(0, "Free Fail: VldId(%d) EsId(%d) not connect.\n", prTmpVdecEsInfo->u4Vld2Id, ucEsId);
                        ASSERT(IS_VLD_ES_CONNECT(prTmpVdecEsInfo->u4Vld2Id, ucEsId));
                    }
                    LOG_TRACE(0, "Tmp Disconnect: VldId(%d) EsId(%d).\n", prTmpVdecEsInfo->u4VldId, ucEsId);
                    VLD_ES_DISCONNECT(prTmpVdecEsInfo->u4Vld2Id, ucEsId);
                }
            }
            
        }
        
        //Alloc
        if (ptPctInfo->ucVLDComType == VLDM_COMB_NONE)
        {
            fgSuccess = FALSE;
            if (IS_VLD_FREE_FOR(VLD0, ptPctInfo->ucVLD0Percent) && 
                IS_VLD_FREE_FOR(VLD1, ptPctInfo->ucVLD1Percent))
            {
                if (ptPctInfo->ucVLD0Percent)
                {
                    VLD_ES_CONNECT(VLD0, ucEsId, ptPctInfo->ucVLD0Percent);
                    u4VldId = VLD0;
                    fgSuccess = TRUE;
                }
                if (ptPctInfo->ucVLD1Percent)
                {
                    VLD_ES_CONNECT(VLD1, ucEsId, ptPctInfo->ucVLD1Percent);
                    (u4VldId == VDEC_MAX_VLD) ? (u4VldId = VLD1) : (u4Vld2Id = VLD1);
                    fgSuccess = TRUE;
                }
            }
        }
        else if (ptPctInfo->ucVLDComType & VLDM_COMB_USED_FIRST)
        {
            if (ptPctInfo->ucVLD0Percent &&
                (prVldInfo[VLD0].u1TotalLoading >= prVldInfo[VLD1].u1TotalLoading) &&
                IS_VLD_FREE_FOR(VLD0, ptPctInfo->ucVLD0Percent))
            {
                VLD_ES_CONNECT(VLD0, ucEsId, ptPctInfo->ucVLD0Percent);
                u4VldId = VLD0;
            }
            else if (ptPctInfo->ucVLD1Percent &&
                (prVldInfo[VLD0].u1TotalLoading <= prVldInfo[VLD1].u1TotalLoading) &&
                IS_VLD_FREE_FOR(VLD1, ptPctInfo->ucVLD1Percent))
            {
                VLD_ES_CONNECT(VLD1, ucEsId, ptPctInfo->ucVLD1Percent);
                u4VldId = VLD1;
            }
            else
            {
                UCHAR ucVld1st= (ptPctInfo->ucVLDComType & VLDM_COMB_CORE0_FIRST) ? VLD0 : VLD1;
                UCHAR ucVld2nd = (ucVld1st == VLD1) ? VLD0 : VLD1;
                
                if (au1CorePercent[ucVld1st] && IS_VLD_FREE_FOR(ucVld1st, au1CorePercent[ucVld1st]))
                {
                    VLD_ES_CONNECT(ucVld1st, ucEsId, au1CorePercent[ucVld1st]);
                    u4VldId = ucVld1st;
                }
                else if (au1CorePercent[ucVld2nd] && IS_VLD_FREE_FOR(ucVld2nd, au1CorePercent[ucVld2nd]))
                {
                    VLD_ES_CONNECT(ucVld2nd, ucEsId, au1CorePercent[ucVld2nd]);
                    u4VldId = ucVld2nd;
                }
                else
                {
                    fgSuccess =FALSE;
                }
            }
        }
        else
        {
            UCHAR ucVld1st= (ptPctInfo->ucVLDComType & VLDM_COMB_CORE0_FIRST) ? VLD0 : VLD1;
            UCHAR ucVld2nd = (ucVld1st == VLD1) ? VLD0 : VLD1;
            
            if (au1CorePercent[ucVld1st] && IS_VLD_FREE_FOR(ucVld1st, au1CorePercent[ucVld1st]))
            {
                VLD_ES_CONNECT(ucVld1st, ucEsId, au1CorePercent[ucVld1st]);
                u4VldId = ucVld1st;
            }
            else if (au1CorePercent[ucVld2nd] && IS_VLD_FREE_FOR(ucVld2nd, au1CorePercent[ucVld2nd]))
            {
                VLD_ES_CONNECT(ucVld2nd, ucEsId, au1CorePercent[ucVld2nd]);
                u4VldId = ucVld2nd;
            }
            else
            {
                fgSuccess =FALSE;
            }
        }
    }while(0);
    
    if ((ucRsrdNum <= 0) || !paucReservedEsId)
    {
        //Save the mapping table .
        prVdecEsInfo->u4VldId = u4VldId;
        prVdecEsInfo->u4Vld2Id = u4Vld2Id;
        x_memcpy(prVdecInfo->arVdecVldInfo, &arVdecVldInfo, sizeof(VDEC_VLD_INFO_T)*VDEC_MAX_VLD);
    }
    /*LOG(3, "Alloced(%d) EsId(%d) VldId(%d, %d) ConEs(%x, %x).\n", 
        fgSuccess, ucEsId, 
        prVdecEsInfo->u4VldId, 
        prVdecEsInfo->u4Vld2Id,
        prVdecInfo->arVdecVldInfo[VLD0].u4ConnEsIdMask,
        prVdecInfo->arVdecVldInfo[VLD1].u4ConnEsIdMask);*/

    VDEC_RES_MNGR_UNLOCK(prVdecInfo);
    return fgSuccess;
}
/**
 * Judge which VLD to use
 * 
 * @Input:
 * @ptInCond:  "Codec, Resolution(W/H), FrameRate, Priority"
 * @ptOutParam: 
 * @ucVLD0/1Percent: need m% percent capacity of Core0/1 to decode.
 * @ucVLDComType: not use now.
 * @aucConflictEsId: The Es which priority lower than current ucEsId, 
 * @      but occupied the vld that current ucEsId request.
 * @return TRUE/FALSE
 */


static BOOL _VDEC_VLDAllocByCond(UCHAR ucEsId, VDEC_VLDM_INPUT_T *ptInCond, VDEC_VLDM_OUTPUT_T *ptOutParam)
{
    BOOL fgAlloc = FALSE;
    BOOL fgNotSupport = FALSE;
    VDEC_VLDM_OUTPUT_T tPctInfo;
    VDEC_RMNGR_TIM_T* ptVldP4Info = NULL;
    UINT32 uc1stChoice = 0;
    
    VDEC_ES_INFO_T *prVdecEsInfo = NULL;

    prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);

    if (!ptInCond)
    {
        return FALSE;
    }

    VDEC_RES_MNGR_LOCK(prVdecInfo);
    do{
        //Clear First.
        fgAlloc = FALSE;
        tPctInfo.ucVLDComType = VLDM_COMB_NONE;
        
        x_memset(&tPctInfo, 0, sizeof(tPctInfo));

        ptVldP4Info = _VDEC_VLDMngrP4Get(ptInCond->eFmt, ptInCond->u4Width, ptInCond->u4Height);

        if (ptVldP4Info)
        {
            LOG(3, "RM Timing: %s.\n", ptVldP4Info->acName);
            
            if (!(ptVldP4Info->u4Core0Fps || ptVldP4Info->u4Core1Fps || ptVldP4Info->u4DCoreFps))
            {
                fgNotSupport = TRUE;
                LOG(0, "RM Timing: %s not support.\n", ptVldP4Info->acName);
                break; 
            }
            uc1stChoice = ptVldP4Info->uc1stChoice;
            #ifdef ENABLE_MULTIMEDIA
            if (((prVdecEsInfo->e3DType == VDEC_3D_MVC) || (SWDMX_SRC_TYPE_NETWORK_NETFLIX_3D == prVdecEsInfo->eMMSrcType)) 
                && ((uc1stChoice == 2)&& ptVldP4Info->u4DCoreFps))
            {
                uc1stChoice = 1;
            }
            #endif
            //Percent = frame rate * 100 /(frame per second of HW abililty)
            if ((uc1stChoice == 2)&& ptVldP4Info->u4DCoreFps)
            {
                tPctInfo.ucVLD0Percent = tPctInfo.ucVLD1Percent = ptInCond->u4FrmRatCod/ptVldP4Info->u4DCoreFps;
                tPctInfo.ucVLDComType = VLDM_COMB_NONE;
            }
            else if ((uc1stChoice == 0)&& ptVldP4Info->u4Core0Fps)
            {
                tPctInfo.ucVLD0Percent = ptInCond->u4FrmRatCod/ptVldP4Info->u4Core0Fps;
                if (ptVldP4Info->u4Core1Fps)
                {
                    tPctInfo.ucVLD1Percent = ptInCond->u4FrmRatCod/ptVldP4Info->u4Core1Fps;
                    tPctInfo.ucVLDComType = VLDM_COMB_USED_FIRST_CORE0_THEN;  //TBD
                }
                else
                {
                    tPctInfo.ucVLD1Percent = 0;
                    tPctInfo.ucVLDComType = VLDM_COMB_NONE;
                }
            }
            else if ((uc1stChoice == 1)&& ptVldP4Info->u4Core1Fps)
            {
                tPctInfo.ucVLD1Percent = ptInCond->u4FrmRatCod/ptVldP4Info->u4Core1Fps;
                if (ptVldP4Info->u4Core0Fps)
                {
                    tPctInfo.ucVLD0Percent = ptInCond->u4FrmRatCod/ptVldP4Info->u4Core0Fps;
                    tPctInfo.ucVLDComType = VLDM_COMB_USED_FIRST_CORE1_THEN;  //TBD
                }
                else
                {
                    tPctInfo.ucVLD0Percent = 0;
                    tPctInfo.ucVLDComType = VLDM_COMB_NONE;
                }
            }
            
            fgAlloc = _VDEC_VLDAllocByPer(ucEsId, &tPctInfo, NULL, 0);
        }
    }while(0);

    if (!fgAlloc && ptOutParam && ptVldP4Info && !fgNotSupport)
    {
        //Try: Min0 -> Min1 -> Min0&Min1 -> Min2->Min0&Min2 ->Min1&Min2 ->Min0&Min1&Min2
        #if (VDEC_MAX_ES==4)
        static UCHAR aucMinIdx[7][VDEC_MAX_ES-1] =
            {{0,VDEC_MAX_ES,VDEC_MAX_ES}, {1,VDEC_MAX_ES,VDEC_MAX_ES}, {0,1,VDEC_MAX_ES},
             {2,VDEC_MAX_ES,VDEC_MAX_ES}, {0,2,VDEC_MAX_ES},{1,2,VDEC_MAX_ES},{0,1,2}};
        #elif (VDEC_MAX_ES==2)
        static UCHAR aucMinIdx[1][VDEC_MAX_ES-1] =
            {{0}};
        #else 
        static UCHAR aucMinIdx[1][1] =
            {{0}};
        #endif
        
        UCHAR ucIdx = 0, ucIdxY = 0;
        UCHAR aucMinPrId[VDEC_MAX_ES] = {VDEC_MAX_ES};  // Es Id  after priority sort: Min --> Max
        UINT32 au4MinPrty[VDEC_MAX_ES] = {100};          // Priority after sort: Min --> Max
        UCHAR  ucMinPrIdTmp;
        UINT32 u4MinPrtyTmp;
        UCHAR aucReservedEsId[VDEC_MAX_ES-1] = {VDEC_MAX_ES};

        //Sort the Priority
        for (ucIdx = 0; ucIdx < VDEC_MAX_ES; ucIdx++)
        {
            VDEC_ES_INFO_KEEP_T *parVdecEsInfoKeepTmp = NULL;

            parVdecEsInfoKeepTmp = _VDEC_GetEsInfoKeep(ucIdx);
            //aucMinPrty[ucIdx] = parVdecEsInfoTmp ? parVdecEsInfoTmp->ucPriority : 100;
            au4MinPrty[ucIdx] = parVdecEsInfoKeepTmp ? parVdecEsInfoKeepTmp->u4Priority : 100;
            aucMinPrId[ucIdx] = ucIdx;
        }
        LOG_TRACE(3, "All Priority: %d, %d.\n", au4MinPrty[0], au4MinPrty[1]);
        for (ucIdx = 0; ucIdx < VDEC_MAX_ES; ucIdx++)
        {
            for (ucIdxY = VDEC_MAX_ES - 1; ucIdxY > ucIdx; ucIdxY--)
            {
                if (au4MinPrty[ucIdxY] < au4MinPrty[ucIdx])
                {
                    u4MinPrtyTmp = au4MinPrty[ucIdx];
                    au4MinPrty[ucIdx] = au4MinPrty[ucIdxY];
                    au4MinPrty[ucIdxY] = u4MinPrtyTmp;

                    ucMinPrIdTmp = aucMinPrId[ucIdx];
                    aucMinPrId[ucIdx] = aucMinPrId[ucIdxY];
                    aucMinPrId[ucIdxY] = ucMinPrIdTmp;
                }
            }
        }
        LOG_TRACE(3, "All Priority After Sort: Es(%d,%d) =(%d,%d) Ns(%d).\n", 
            aucMinPrId[0], aucMinPrId[1], au4MinPrty[0], au4MinPrty[1],
            sizeof(aucMinIdx)/(sizeof(UCHAR)*(VDEC_MAX_ES-1)));

        for (ucIdx = 0; ucIdx < sizeof(aucMinIdx)/(sizeof(UCHAR)*(VDEC_MAX_ES-1)); ucIdx++) //loop aucMinIdx
        {
            UINT8 u1TmpNs = 0;
            for (u1TmpNs = 0; u1TmpNs < VDEC_MAX_ES-1; u1TmpNs++)
            {
                LOG_TRACE(3, "Priority Check Tmp(%d) Idx(%d) Prty(%d).\n", u1TmpNs, aucMinIdx[ucIdx][u1TmpNs], au4MinPrty[u1TmpNs]);
                if ((aucMinIdx[ucIdx][u1TmpNs] >= VDEC_MAX_ES) || 
                    (au4MinPrty[u1TmpNs] >= ptInCond->u4Priority))
                {
                    break;
                }
                aucReservedEsId[u1TmpNs] = aucMinPrId[aucMinIdx[ucIdx][u1TmpNs]];
            }
            if (!u1TmpNs)
            {
                LOG_TRACE(3, "No Es Priority low then current(%d).\n", ptInCond->u4Priority);
                break;
            }
            LOG_TRACE(3, "Reserve alloc(%d): Id(%d, %d).\n", u1TmpNs, aucReservedEsId[0], aucReservedEsId[1]);
            if (_VDEC_VLDAllocByPer(ucEsId, &tPctInfo, aucReservedEsId, u1TmpNs))
            {
                x_memcpy(ptOutParam->aucConflictEsId, aucReservedEsId, u1TmpNs);
                break;
            }
        }
    }
    
    if (ptOutParam && ptVldP4Info && !fgNotSupport)
    {
        ptOutParam->ucVLD0Percent = tPctInfo.ucVLD0Percent;
        ptOutParam->ucVLD1Percent = tPctInfo.ucVLD1Percent;
        ptOutParam->ucVLDComType  = tPctInfo.ucVLDComType;
        ptOutParam->ptVldP4Info   = ptVldP4Info;
    }

    VDEC_RES_MNGR_UNLOCK(prVdecInfo);
    return fgAlloc;
}


/**
 * Convert cond from EsInfo to InCond Struct
 * 
 * @ptInCond:  "Codec, Resolution(W/H), FrameRate" or EsInfo if ptInCond==NULL
 * @return TRUE/FALSE
 */

static BOOL _VDEC_VLDMngrCondCvt(UCHAR ucEsId, VDEC_VLDM_INPUT_T *ptInCond)
{
    VDEC_ES_INFO_T *prVdecEsInfo = NULL;
    VDEC_ES_INFO_KEEP_T *prVdecEsInfoKeep = NULL;

    ASSERT(ucEsId < VDEC_MAX_ES);
    if(ucEsId >= VDEC_MAX_ES)
    {
        LOG(0, "ucEsId %d >= VDEC_MAX_ES\n", ucEsId);
        return FALSE;
    }

    prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);
    prVdecEsInfoKeep = _VDEC_GetEsInfoKeep(ucEsId);

    if (!prVdecEsInfo || !prVdecEsInfoKeep)
    {
        LOG(0, "prVdecEsInfo or prVdecEsInfoKeep is NULL!!\n");
        return FALSE;
    }
    
    VDEC_RES_MNGR_LOCK(prVdecInfo);
    ptInCond->eFmt        = prVdecEsInfoKeep->eCurFMT;
    ptInCond->u4Priority  = prVdecEsInfoKeep->u4Priority;
    
    if (prVdecEsInfo->u4FrameRate == 0)
    {
        prVdecEsInfo->u4FrameRate = 3000;  //TBD
    }
    #ifdef ENABLE_MULTIMEDIA
    if (VDEC_IS_SEAMLESS(prVdecEsInfo->eMMSrcType))
    {
        ptInCond->u4Width     = prVdecEsInfo->u4SeamlessWidth ? prVdecEsInfo->u4SeamlessWidth : 1920;
        ptInCond->u4Height    = prVdecEsInfo->u4SeamlessHeight ? prVdecEsInfo->u4SeamlessHeight : 1080;
        //4K2K seamless default set to 30HZ, FHD seamless set to 60fps.
        ptInCond->u4FrmRatCod = prVdecEsInfo->fgProgressive ? prVdecEsInfo->u4FrameRate : (prVdecEsInfo->u4FrameRate>>1);
        ptInCond->u4FrmRatCod = ((ptInCond->u4Width > 1920) && (ptInCond->u4FrmRatCod <= 3000)) ? 3000 : 
            ((ptInCond->u4FrmRatCod <= 6000) ? 6000 : ptInCond->u4FrmRatCod); 
        ptInCond->fgProgressive = prVdecEsInfo->fgProgressive;
    }
    else
    #endif
    {
        ptInCond->u4Width     = prVdecEsInfo->u2OrgHSize;
        ptInCond->u4Height    = prVdecEsInfo->u2OrgVSize;
        // Decode performance: 60I == 30P
        ptInCond->u4FrmRatCod = prVdecEsInfo->fgProgressive ? prVdecEsInfo->u4FrameRate : (prVdecEsInfo->u4FrameRate>>1);
        ptInCond->fgProgressive = prVdecEsInfo->fgProgressive;
    }
    
    VDEC_RES_MNGR_UNLOCK(prVdecInfo);
    return TRUE;
}


/****************************************
 * Check UFO enable or not
 * 
 * @input ptInCond, or use EsInfo if ptInCond==NULL
 * @return TRUE/FALSE
 *****************************************/
BOOL _VDEC_VLDMngrChkUFO(UCHAR ucEsId, VDEC_VLDM_INPUT_T *ptInCond)
{
    UCHAR ucTimeType = 0;
    BOOL fgUFOMode = FALSE;
    VDEC_VLDM_INPUT_T tCond = {0};
    
    VDEC_ES_INFO_T *prVdecEsInfo = NULL;
    VDEC_ES_INFO_KEEP_T *prVdecEsInfoKeep = NULL;

    ASSERT(ucEsId < VDEC_MAX_ES);
    if(ucEsId >= VDEC_MAX_ES)
    {
        LOG(0, "ucEsId %d >= VDEC_MAX_ES\n", ucEsId);
        return FALSE;
    }

    prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);
    prVdecEsInfoKeep = _VDEC_GetEsInfoKeep(ucEsId);

    if (!prVdecEsInfo || !prVdecEsInfoKeep)
    {
        LOG(0, "prVdecEsInfo is NULL!!\n");
        return FALSE;
    }
    
    VDEC_RES_MNGR_LOCK(prVdecInfo);
    if (ptInCond == NULL)
    {
        x_memset(&tCond, 0, sizeof(tCond));
        _VDEC_VLDMngrCondCvt(ucEsId, &tCond);
    }
    else
    {
        x_memcpy(&tCond, ptInCond, sizeof(VDEC_VLDM_INPUT_T));
    }
    
    ucTimeType = FBM_SelectGroupType(ptInCond->u4Width, ptInCond->u4Height);

    if (ucTimeType == FBM_FBG_TYPE_UNKNOWN)
    {
        VDEC_RES_MNGR_UNLOCK(prVdecInfo);
        return FALSE;
    }
    
    if ((tCond.eFmt == VDEC_FMT_H264) ||
        (tCond.eFmt == VDEC_FMT_H265) ||
        (tCond.eFmt == VDEC_FMT_VP9))
    {
        // >720 enable UFO
        if ((ucTimeType == FBM_FBG_TYPE_1080HD) ||
            (ucTimeType == FBM_FBG_TYPE_4K2K))
        {
            fgUFOMode = TRUE;
        }
        #ifdef CC_COMBINE_4K_2K
        {
        UINT32 u4ModelType = 0;
        if (DRVCUST_InitQuery(eGet2KModelSupport, &u4ModelType) == 0)
        {
            if ((u4ModelType&0x01) == 0)
            {
                //2K Model, enable FHD UFO .
            }
            else if ((ucTimeType == FBM_FBG_TYPE_1080HD) && (tCond.u4FrmRatCod <= 6000))
            {
                //4K Model, enable FHD @120 and 4K UFO .
                LOG(4, "4K model: FHD <=60fps disable UFO.");
                fgUFOMode = FALSE;
            }
        }
        }
        #endif
        if (tCond.fg10Bit) // < 720 &10bit, none UFO 
        {
            //fgUFOMode = TRUE;
        }
        // 120 fps enable UFO
        if (tCond.u4FrmRatCod > 6000) // < 720 & 120, none UFO
        {
            //fgUFOMode = TRUE;
        }
        
        // Interlace not support UFO
        if (!tCond.fgProgressive)
        {
            //Not disable UFO here for I / MABFF, disable while decoder seq_chg.  
            //fgUFOMode = FALSE;
        }
        #ifdef ENABLE_MULTIMEDIA
        if (VDEC_IS_SEAMLESS(prVdecEsInfo->eMMSrcType))
        {
            fgUFOMode = FALSE;
        }
        #endif
        if (prVdecEsInfoKeep->fgVPush && (prVdecEsInfo->u4RenderVDPId == 0xFF)) // GFX Path can't support UFO.
        {
            fgUFOMode = FALSE;
        }
    }

    if (fgUFOMode)
    {
        LOG(3, "Vdec_RM: Enable UFO.\n");
    }
    VDEC_RES_MNGR_UNLOCK(prVdecInfo);
    return fgUFOMode;
}

#endif

/**
 * Free VLD 
 * 
 * @Input:  ucEsId
 * @return VOID
 */

VOID VDEC_VLDMngrFreeVld(UCHAR ucEsId)
{
        
    VDEC_ES_INFO_T *prVdecEsInfo = NULL;

    ASSERT(ucEsId < VDEC_MAX_ES);
    if(ucEsId >= VDEC_MAX_ES)
    {
        LOG(0, "ucEsId %d >= VDEC_MAX_ES\n", ucEsId);
        return;
    }

    prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);

    if (!prVdecEsInfo)
    {
        LOG(0, "ucEsId fail\n");
        return;
    }
    VDEC_RES_MNGR_LOCK(prVdecInfo);

#if (VDEC_MAX_VLD > 2)
    if (prVdecEsInfo->u4VldId != VDEC_MAX_VLD)
    {
        if (!IS_VLD_ES_CONNECT(prVdecEsInfo->u4VldId, ucEsId))
        {
            LOG(0, "Free Fail: VldId(%d) EsId(%d) not connect.\n", prVdecEsInfo->u4VldId, ucEsId);
            ASSERT(IS_VLD_ES_CONNECT(prVdecEsInfo->u4VldId, ucEsId));
        }
        VLD_ES_DISCONNECT(prVdecEsInfo->u4VldId, ucEsId);
        prVdecEsInfo->u4VldId = VDEC_MAX_VLD;
    }
    
    if (prVdecEsInfo->u4Vld2Id != VDEC_MAX_VLD)
    {
        if (!IS_VLD_ES_CONNECT(prVdecEsInfo->u4Vld2Id, ucEsId))
        {
            LOG(0, "Free Fail: VldId(%d) EsId(%d) not connect.\n", prVdecEsInfo->u4Vld2Id, ucEsId);
            ASSERT(IS_VLD_ES_CONNECT(prVdecEsInfo->u4Vld2Id, ucEsId));
        }
        VLD_ES_DISCONNECT(prVdecEsInfo->u4Vld2Id, ucEsId);
        prVdecEsInfo->u4Vld2Id = VDEC_MAX_VLD;
    }
#else
    prVdecEsInfo->u4VldId = VDEC_MAX_VLD;
#endif
    prVdecEsInfo->ptVldP4Info = NULL;

    VDEC_RES_MNGR_UNLOCK(prVdecInfo);
    return ;
}


/**
 * Alloc VLD_IMG 
 * 
 * @Input: 
 * @ucEsId: the Es Id.
 * @return:
 * @ucVldId: fail return VDEC_MAX_VLD.
 */

UCHAR VDEC_VLDAllocVldImg(UCHAR ucEsId)
{
    UCHAR ucVldId = VLD_IMG;
    VDEC_ES_INFO_T *prVdecEsInfo = NULL;

    if (ucEsId >= VDEC_MAX_ES)
    {
        LOG(0, "ucEsId %d >= VDEC_MAX_ES.\n", ucEsId);
        return VDEC_MAX_VLD;
    }

    prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);
    if (!prVdecEsInfo)
    {
        LOG(0, "prVdecEsInfo is NULL!!\n");
        return VDEC_MAX_VLD;
    }

    VDEC_RES_MNGR_LOCK(prVdecInfo);

    do{
        if (IS_VLD_FREE_FOR(ucVldId, 1)) //TBD 1
        {
            VLD_ES_CONNECT(ucVldId, ucEsId, 1);  //TBD 1
            prVdecEsInfo->u4VldId = ucVldId;
        }
    }while(0);
    
    VDEC_RES_MNGR_UNLOCK(prVdecInfo);
    LOG(1,"Allocate vld Img(%d).\n",prVdecEsInfo->u4VldId);
    
    return (UCHAR)(prVdecEsInfo->u4VldId);
}

/**
 * Alloc LAE 
 * 
 * @Input: 
 * @ucEsId: the Es Id.
 * @return:
 * @ucVldId: fail return VDEC_MAX_VLD.
 */

UCHAR VDEC_VLDAllocLAE(UCHAR ucEsId)
{
    UCHAR ucVldId = VDEC_MAX_VLD;
    VDEC_ES_INFO_T *prVdecEsInfo = NULL;

    if (ucEsId >= VDEC_MAX_ES)
    {
        LOG(0, "ucEsId %d >= VDEC_MAX_ES.\n", ucEsId);
        return VDEC_MAX_VLD;
    }

    prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);
    if (!prVdecEsInfo)
    {
        LOG(0, "prVdecEsInfo is NULL!!\n");
        return VDEC_MAX_VLD;
    }

    VDEC_RES_MNGR_LOCK(prVdecInfo);

    do{
        for (ucVldId = VLD_LAE0; ucVldId <= VLD_LAE1; ucVldId++)
        {
            if (IS_VLD_FREE_FOR(ucVldId, 100)) //TBD 30
            {
                VLD_ES_CONNECT(ucVldId, ucEsId, 100);  //TBD 30
                prVdecEsInfo->u4VldId = ucVldId;
                if((ucVldId == VLD_LAE0) || (ucVldId == VLD_LAE1))
                {
                    break;
                }
            }
        }
    }while(0);
    VDEC_RES_MNGR_UNLOCK(prVdecInfo);
    LOG(1,"Allocate vld %d\n",prVdecEsInfo->u4VldId);
    return (UCHAR)(prVdecEsInfo->u4VldId);
}

/**
 * Alloc Vld For short time use, such as pre-parser.
 * 
 * @Input: 
 * @ucEsId: the Es Id.
 * @eFmt:    alloc vld for which codec.
 * @i4TimeOut: not use yet.
 * @return:
 * @ucVldId: fail return VDEC_MAX_VLD.
 */

UCHAR VDEC_VLDAllocShortUse(UCHAR ucEsId, ENUM_VDEC_FMT_T eFmt, INT32 i4TimeOut)
{
    UCHAR ucVldId = VDEC_MAX_VLD;
    VDEC_ES_INFO_T *prVdecEsInfo = NULL;

    if (ucEsId >= VDEC_MAX_ES)
    {
        LOG(0, "ucEsId %d >= VDEC_MAX_ES.\n", ucEsId);
        return VDEC_MAX_VLD;
    }

    prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);
    if (!prVdecEsInfo)
    {
        LOG(0, "prVdecEsInfo is NULL!!\n");
        return VDEC_MAX_VLD;
    }
    VDEC_RES_MNGR_LOCK(prVdecInfo);

    do{
        if (eFmt == VDEC_FMT_MJPEG)
        {
            ucVldId = VLD_IMG;
            //TBD
        }
#ifdef CC_MT5890
        else if ((eFmt == VDEC_FMT_H264) || (eFmt == VDEC_FMT_H264VER))
        {
            //TBD
            ucVldId = VLD_LAE1;
        }
        else if (eFmt == VDEC_FMT_MPV)
        {
            //TBD
            ucVldId = VLD_Core1;
        }
#endif       
        else //Others
        {
            //TBD
            ucVldId = VLD_Core0;
        }
    }while(0);
    
    if (ucVldId >= VDEC_MAX_VLD)
    {
        LOG(0, "ucVldId %d >= VDEC_MAX_VLD(%d).\n", ucVldId, VDEC_MAX_VLD);
        VDEC_RES_MNGR_UNLOCK(prVdecInfo);
        return VDEC_MAX_VLD;
    }

    ASSERT(prVdecInfo->arVdecVldInfo[ucVldId].ucConnTmpEsId == VDEC_MAX_ES);
    prVdecInfo->arVdecVldInfo[ucVldId].ucConnTmpEsId = ucEsId;

    ASSERT(prVdecEsInfo->u4VldId == VDEC_MAX_VLD);
    prVdecEsInfo->u4VldId = ucVldId;
    
    VDEC_RES_MNGR_UNLOCK(prVdecInfo);
    return ucVldId;
}

/**
 * Free Vld For short time use, such as pre-parser.
 * 
 * @Input: 
 * @ucEsId: the Es Id.
 * @ucVldId: free vld.
 * @return:
 * @TRUE/FALSE for OK and NG.
 */

BOOL VDEC_VLDFreeShortUse(UCHAR ucEsId, UCHAR ucVldId)
{
    VDEC_ES_INFO_T *prVdecEsInfo = NULL;

    if ((ucEsId >= VDEC_MAX_ES) || (ucVldId >= VDEC_MAX_VLD))
    {
        LOG(0, "ucEsId %d >= VDEC_MAX_ES or ucVldId %d >= VDEC_MAX_VLD\n", ucEsId, ucVldId);
        return FALSE;
    }
    
    prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);
    if (!prVdecEsInfo)
    {
        LOG(0, "prVdecEsInfo is NULL!!\n");
        return FALSE;
    }
    VDEC_RES_MNGR_LOCK(prVdecInfo);

    if (prVdecInfo->arVdecVldInfo[ucVldId].ucConnTmpEsId != VDEC_MAX_ES)
    {
        ASSERT(prVdecInfo->arVdecVldInfo[ucVldId].ucConnTmpEsId == ucEsId);
        prVdecInfo->arVdecVldInfo[ucVldId].ucConnTmpEsId = VDEC_MAX_ES;
        
        ASSERT(prVdecEsInfo->u4VldId == ucVldId);
        prVdecEsInfo->u4VldId = VDEC_MAX_VLD;
    }
    
    VDEC_RES_MNGR_UNLOCK(prVdecInfo);
    return TRUE;
}


/**
 * Judge which VLD to use
 * 
 * @Input:
 * @ucEsId:  vdec es id.
 * @output: 
 * @paucConflictEsId:  Conflict Es Id if priority check.
 * @return TRUE/FALSE
 */
#ifdef USB_CLI_FORCE_SETTING_SUPPORT

VOID VDEC_VLDMngrMapping(UCHAR ucEsId, UCHAR ucVldUsage, UCHAR ucB2rUsage)
{
    VDEC_ES_INFO_T *prVdecEsInfo = NULL;
    
    prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);
    
    if (!prVdecEsInfo)
    {
        LOG(0, "prVdecEsInfo is NULL!!\n");
        return ;
    }
    if ((ucEsId >= VDEC_MAX_ES) || (ucVldUsage >= (1<<VDEC_MAX_VLD)))
    {
        LOG(1, "Setting error: ucEsId(%d) VldIdUsage(%d) B2rIdUsage(%d).\n", ucEsId, ucVldUsage, ucB2rUsage - 1);
        return;
    }
    prVdecEsInfo->u4VldIdUsage = ucVldUsage;
    prVdecEsInfo->u4B2rIdUsage = ucB2rUsage;
    LOG(1, "Connect EsId(%d) VldIdUsage(%d) B2rIdUsage(%d).\n", ucEsId, prVdecEsInfo->u4VldIdUsage, prVdecEsInfo->u4B2rIdUsage - 1);
    
}

#endif


BOOL VDEC_VLDMngrAllocVld(UCHAR ucEsId, UCHAR *paucConflictEsId)
{
#if (VDEC_MAX_VLD > 2)
    VDEC_VLDM_INPUT_T tInCond;
    VDEC_VLDM_OUTPUT_T tOutParam;
#endif
    VDEC_ES_INFO_T *prVdecEsInfo = NULL;
    VDEC_ES_INFO_KEEP_T *prVdecEsInfoKeep = NULL;
    BOOL fgRet = FALSE;

    ASSERT(ucEsId < VDEC_MAX_ES);
    if(ucEsId >= VDEC_MAX_ES)
    {
        LOG(0, "ucEsId %d >= VDEC_MAX_ES\n", ucEsId);
        return FALSE;
    }

    prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);
    prVdecEsInfoKeep = _VDEC_GetEsInfoKeep(ucEsId);

    if (!prVdecEsInfo || !prVdecEsInfoKeep)
    {
        LOG(0, "prVdecEsInfo or prVdecEsInfoKeep is NULL!!\n");
        return FALSE;
    }
    
    VDEC_RES_MNGR_LOCK(prVdecInfo);
    
#if (VDEC_MAX_VLD <= 2)
    prVdecEsInfo->u4VldId = (prVdecEsInfoKeep->eCurFMT == VDEC_FMT_MJPEG) ? VLD_IMG : VLD0;
    fgRet = TRUE;
    UNUSED(paucConflictEsId);
#else

    x_memset(&tInCond, 0, sizeof(tInCond));
    x_memset(&tOutParam, 0, sizeof(tOutParam));
    x_memset(tOutParam.aucConflictEsId, VDEC_MAX_ES, sizeof(tOutParam.aucConflictEsId));

    _VDEC_VLDMngrCondCvt(ucEsId, &tInCond);

    if (prVdecEsInfo->ptVldP4Info)
    {
        VDEC_RMNGR_TIM_T* ptVldP4Info = NULL;
        ptVldP4Info = _VDEC_VLDMngrP4Get(tInCond.eFmt, tInCond.u4Width, tInCond.u4Height);
        if (!VDEC_RES_P4_EQ(prVdecEsInfo->ptVldP4Info, ptVldP4Info))
        {
            VDEC_VLDMngrFreeVld(ucEsId);  //Free first for Re-alloc
        }
        else
        {
            prVdecEsInfo->ptVldP4Info = ptVldP4Info;
            LOG(3, "Re-alloc not need, as VLD require not changed.!!\n");
            VDEC_RES_MNGR_UNLOCK(prVdecInfo);
            return FALSE;
        }
    }
    
#ifdef USB_CLI_FORCE_SETTING_SUPPORT
    if (prVdecEsInfo->u4VldIdUsage)
    {
        VLD_UNMAKE_VLD_MASK(prVdecEsInfo->u4VldIdUsage, prVdecEsInfo->u4VldId, prVdecEsInfo->u4Vld2Id);
        if (prVdecEsInfo->u4VldId != VDEC_MAX_VLD)
        {
            VLD_ES_CONNECT(prVdecEsInfo->u4VldId, ucEsId, 1);
        }
        if (prVdecEsInfo->u4Vld2Id != VDEC_MAX_VLD)
        {
            VLD_ES_CONNECT(prVdecEsInfo->u4Vld2Id, ucEsId, 1);
        }

        fgRet = TRUE;
        tOutParam.ptVldP4Info = _VDEC_VLDMngrP4Get(tInCond.eFmt, tInCond.u4Width, tInCond.u4Height);
    }
    else
#endif
    fgRet = _VDEC_VLDAllocByCond(ucEsId, &tInCond, &tOutParam);

    if (fgRet == TRUE)  //UFO Not ready yet
    {
        prVdecEsInfo->fgEnableUFO = _VDEC_VLDMngrChkUFO(ucEsId, &tInCond);
    }
    prVdecEsInfo->ptVldP4Info = tOutParam.ptVldP4Info;
    
    LOG(1, "Connect EsId(%d) with VLD0(%d) VLD1(%d) Ret(%d).\n", ucEsId, prVdecEsInfo->u4VldId, prVdecEsInfo->u4Vld2Id, fgRet);

    if (paucConflictEsId)
    {
        x_memcpy(paucConflictEsId, tOutParam.aucConflictEsId, sizeof(tOutParam.aucConflictEsId));
    }
#endif
    VDEC_RES_MNGR_UNLOCK(prVdecInfo);

    return fgRet;
}

/**
 * Query the left VLD's Capability 
 * 
 * @ptInCond:  "Codec, Resolution(W/H), FrameRate"
 * @return TRUE/FALSE
 */

BOOL VDEC_VLDMngrQueryCap(ENUM_VDEC_FMT_T eCurFMT, UINT32 u4Width, UINT32 u4Height, UINT32 u4FrmRatCod)
{
#if (VDEC_MAX_VLD > 2)
    VDEC_RMNGR_TIM_T* ptVldP4Info = NULL;
    UINT32 uc1stChoice = 0;
#endif
    BOOL fgRet = FALSE;

#if (VDEC_MAX_VLD <= 2)
    fgRet = TRUE;
#else
    VDEC_RES_MNGR_LOCK(prVdecInfo);

    //fgRet =  _VDEC_VLDAllocByCond(VDEC_MAX_ES - 1, &tInCond, NULL); //TBD:  VDEC_MAX_ES - 1 
    ptVldP4Info = _VDEC_VLDMngrP4Get(eCurFMT, u4Width, u4Height);
    
    while (ptVldP4Info)
    {
        LOG(3, "RM Timing: %s.\n", ptVldP4Info->acName);
        
        if (eCurFMT == VDEC_FMT_MJPEG)
        {
            fgRet = TRUE;
            break; 
        }
        
        if (!(ptVldP4Info->u4Core0Fps || ptVldP4Info->u4Core1Fps || ptVldP4Info->u4DCoreFps))
        {
            fgRet = FALSE;
            LOG(0, "RM Timing: %s not support.\n", ptVldP4Info->acName);
            break; 
        }
        uc1stChoice = ptVldP4Info->uc1stChoice;
    
        //Percent = frame rate * 100 /(frame per second of HW abililty)
        if ((uc1stChoice == 2)&& ptVldP4Info->u4DCoreFps)
        {
            UCHAR ucVldPercent = u4FrmRatCod/ptVldP4Info->u4DCoreFps;
            
            if (IS_VLD_FREE_FOR(VLD0, ucVldPercent) && 
                IS_VLD_FREE_FOR(VLD1, ucVldPercent))
            {
                fgRet = TRUE;
            }
        }
        else if (ptVldP4Info->u4Core1Fps || ptVldP4Info->u4Core0Fps)
        {
            UCHAR ucVld0Percent = ptVldP4Info->u4Core0Fps ? u4FrmRatCod/ptVldP4Info->u4Core0Fps : 0;
            UCHAR ucVld1Percent = ptVldP4Info->u4Core1Fps ? u4FrmRatCod/ptVldP4Info->u4Core1Fps : 0;
            
            if ((ptVldP4Info->u4Core0Fps && IS_VLD_FREE_FOR(VLD0, ucVld0Percent)) || 
                (ptVldP4Info->u4Core1Fps && IS_VLD_FREE_FOR(VLD1, ucVld1Percent)))
            {
                fgRet = TRUE;
            }
        }
        else
        {
            LOG(0, "RM Query Fail, Current Loading(%d, %d).\n", prVldInfo[VLD0].u1TotalLoading, prVldInfo[VLD1].u1TotalLoading);
        }
    
        break;
    }
    VDEC_RES_MNGR_UNLOCK(prVdecInfo);

#endif
    LOG(1, "[VDEC_RM]Query(%d): Codec(%d) %dx%d @%dfps.\n", fgRet, eCurFMT, u4Width, u4Height, u4FrmRatCod);

    return fgRet;
}
#endif

