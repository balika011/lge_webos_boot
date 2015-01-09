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
 * $RCSfile: drv_nr_int.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/
#ifndef _DRV_NR_INT_H_
#define _DRV_NR_INT_H_

//#define DRV_SUPPORT_CUST_ANR

#define NX_WIDTH 1920
#define NX_HEIGHT 1080
#define NX_PACK_SIZE (1920>>1)
#define NX_BLKPOS_SIZE 240
#define NX_BLKPOS_SIZE_H 120
#define NX_BLKPOS_SIZE_V 120

#ifdef V_Fld
#undef V_Fld
#endif
#define V_Fld(u4Val, fld) ((u4Val)<<Fld_shft(fld))

// read field
#ifdef R_Fld
#undef R_Fld
#endif
#define R_Fld(u4Val, fld) (((u4Val)&Fld2Msk32(fld))>>Fld_shft(fld))

#ifdef _EN
#undef _EN
#endif
#define _EN(fld) R_Fld(u4GetEnMsk(), fld)

////////////////////////////////////////////////////////////////////////////////
// Utils
////////////////////////////////////////////////////////////////////////////////
#ifdef AbsDiff
#undef AbsDiff
#endif
#define AbsDiff(a, b)  (((a)>(b)) ? ((a)-(b)) : ((b)-(a)))

#define W_MAX(res) (1<<(res))
#define W_ROUND(res) (W_MAX(res)>>1)
#define LERP(a, b, wei, res) ((((UINT32)(a))*(wei) + ((UINT32)(b))*(W_MAX(res)-(wei)) + (W_MAX(res)>>1)) >> (res))
#define LERP_TGT(a, b, wei, res) ((((UINT32)(a))*(wei) + ((UINT32)(b))*(W_MAX(res)-(wei)) + (((b)>(a))?(W_MAX(res)-1):0)) >> (res))
//#define LERP_TGT(a, b, wei, res) ((((a) == LERP(a, b, wei, res)) && (a)!=(b)) ? (((a)>(b))?((a)-1):((a)+1)) : LERP(a, b, wei, res))

// start point (sx, sy), end point (ex, ey), x belongs to [sx, ex], find interpolated value y
//#define LERP_F(sx, sy, ex, ey, x) ((((x)-(sx))*(ey) + ((ex)-(x))*(sy)) / ((ex)-(sx)))
#define LERP_F(sx, sy, ex, ey, x) (((((x)-(sx))*((ey)-(sy))) / MAX(1,((ex)-(sx)))) + (sy))
#define LERP_F_C(sx, sy, ex, ey, x) LERP_F(sx, sy, ex, ey, (MIN(MAX((INT32)x,(INT32)sx),(INT32)ex)))


#define _vInsertSort(T, va, n, cmp)\
{\
    UINT32 i, j;\
    T temp;\
    for (i = 1; i < n; i++)\
    {\
        temp = va[i];\
        for(j = i ; j > 0 && cmp(&temp, &va[j-1]) < 0; j--)\
        {\
            va[j] = va[j-1];\
        }\
        va[j] = temp;\
    }\
}

// sort for getting first k elements
#define _vSelectSortK(T, va, n, cmp, k)\
{\
    UINT32 i, j, m;\
    T temp;\
    for (j = 0; j < MIN((k),(n)-1); j++)\
    {\
        m = j;\
        for ( i = j+1; i < (n); i++ )\
        {\
            if (cmp(&va[i], &va[m]) < 0)\
            {\
                m = i;\
            }\
        }\
        if ( m != j )\
        {\
            temp = va[j];\
            va[j] = va[m];\
            va[m] = temp;\
        }\
    }\
}

#define Q_TYPE(T, Sz, TyName)\
typedef struct \
{\
    T      rQueue[Sz];\
    UINT32 u4Idx;\
    UINT32 u4Sum;\
    UINT32 u4Len;\
} TyName;

#define Q_INIT(q,len)\
{\
    UINT32 __i;\
    for (__i = 0; __i < len; __i++)\
        q.rQueue[__i] = 0;\
    q.u4Idx = 0;\
    q.u4Sum = 0;\
    q.u4Len = len;\
}

#define ADD_TO_Q(q,val)\
{\
    q.u4Sum -= MIN(q.u4Sum, q.rQueue[q.u4Idx]);\
    q.rQueue[q.u4Idx] = val;\
    q.u4Sum += val;\
    q.u4Idx ++;\
    q.u4Idx = (q.u4Idx == q.u4Len) ? 0 : q.u4Idx;\
}

#define Q_AVG(q,base) (q.u4Sum + ((1<<(base))/2)) >> (base)

////////////////////////////////////////////////////////////////////////////////
// Defs & Variabls
////////////////////////////////////////////////////////////////////////////////
typedef enum
{
    NR_FW_STOP   = 0,
    NR_FW_VSYNC  = 1,
    NR_FW_ACTIVE = 2
} E_NR_FW_STATE;


#define NR_MOIDX_MAX        (16)
#define NR_NLIDX_MAX        (32)

#define SAD_HIST_ENTRY_NUM  (25)
#define SAD_HIST_LAST       (SAD_HIST_ENTRY_NUM - 1)

#define NUM_PARAM_GROUP		(10)

#define NR_FILT_FIFO_WD         4
#define NR_FILT_FIFO_NUM        (1<<NR_FILT_FIFO_WD)
#define NR_FILT_FIFO_MOD_MSK    (NR_FILT_FIFO_NUM-1)
#define NR_FILT_NUM_BIN         16

#define GET_Q_AVG(q) Q_AVG(q, NR_FILT_FIFO_WD)

Q_TYPE(UINT8, NR_FILT_FIFO_NUM, NR_Q1B_T);
Q_TYPE(UINT16, NR_FILT_FIFO_NUM, NR_Q2B_T);
Q_TYPE(UINT32, NR_FILT_FIFO_NUM, NR_Q4B_T);


/*
typedef struct
{
    UINT32 u4BlkIdxSum;
    UINT8 u1Phase;
} NR_BLK_PHASE_SUM_T;
*/

typedef struct
{
    UINT8  u1Wei;
    UINT8  u1Err;
    UINT32 u4Sta;
    UINT8 u1Wei_dec;
} NR_IIR_T;

typedef struct
{
    E_NR_FW_STATE eNrFwState;

    UINT8 u1GmvX;
    UINT8 u1GmvY;
    UINT8 u1GmvValid;
    UINT8 u1GmvRatio;
    UINT8 u1FlfbStatus;
    UINT8 u1CompositeStatus;
    UINT8 u1ScnChg;
    UINT8 u1ScnChgCnt;
    UINT8 u1NLScnChgCnt;
    UINT8 u1FavorCSState;
    UINT8 u1Pause;
    
    UINT16 u2Nzc;
    UINT16 u2HistMaxCnt; 
    
    UINT32 u4HistDiffAcc;
    UINT32 u4HomoSum;
    UINT32 u4PreHomoSum;
    UINT32 u4HomoSumDiffTh;
    UINT32 u4StaDetail;
    UINT32 u4EdgeCnt;
    UINT32 u4TmprDiffSum;
    UINT32 u4TmprDiffCnt;
    UINT32 u4MoPixCnt;
    UINT32 u4PreMoPixCnt;
    UINT32 u4SADHistCntCurr;
    UINT16 arSADHistCurr[SAD_HIST_ENTRY_NUM];
} NR_NM_STA_T;

typedef struct
{
    UINT8 u1NoiseLevel;

    UINT8 u1Centroid;
    UINT8 u1TdfNL;
    UINT8 u1MoIdx;

    UINT8 u1PeakBin;
    UINT8 u1PeakBinMo;
    UINT8 u1FIRPeakBin;
    UINT8 u1FatTailRto;
    UINT8 u1MoFbckBlend;
    UINT8 u1HistInvalidRto;
    
    UINT8 u1RTrough;
    UINT8 u1Terminate;
    UINT8 u1ZoomTex;
    UINT8 u1MvApprove;
    UINT8 u1FbkSta;
    UINT8 u1FbkStep;    
    
    UINT8 u1ValidNoiseLevel;
    UINT8 u1AntiNLGain;
    UINT8 u1FbCnt;
    UINT8 u1CleanDiffLvl;
    
    UINT16 u2FIRPeakBinVal;
    UINT32 u4TailSum; 
    UINT32 u4HistMoment;
    UINT32 u4TotalHistCnt;
    UINT8 u1TVD_NL;
    UINT8 u1TargetNL;
} NR_NM_ANA_T;


EXTERN NR_PARAM_T _rNrPrm;
EXTERN NR_NM_STA_T _rNmSta;
EXTERN NR_NM_ANA_T _rNmAna;
EXTERN UINT8 _u1Is_TVDNM_Stable;
EXTERN UINT8 _u1TVDStable;



EXTERN void NR_FW_NM_Init(void);
EXTERN void NR_FW_NM_Process(const NR_NM_STA_T* prNmSta, NR_NM_ANA_T* prNmAna);
EXTERN void NR_FW_ANR_Init(void);
EXTERN void NR_FW_ANR_Process(const NR_NM_STA_T* prNmSta, const NR_NM_ANA_T* prNmAna);
EXTERN void NR_FW_ANR_WriteReg(void);
EXTERN void NR_FW_SetState(E_NR_FW_STATE eState);

EXTERN UINT32 u4DrvNRUtilIIR(NR_IIR_T* rIIR, UINT8 u1Res, UINT32 u4Val);
EXTERN UINT32 u4DrvNRUtilIIR_1(NR_IIR_T* rIIR, UINT8 u1Res, UINT32 u4Val);

UINT32 u4GetEnMsk(void);

// === For ANR ====================================================================
#define NR_WR_ST_NULL    0
#define NR_WR_ST_IDLE    1
#define NR_WR_ST_AUTO    2
#define NR_WR_ST_FROM_SW 3
#define NR_WR_ST_RESET   4

typedef void (*ANR_FUNC_MODULE_PROC)(const NR_NM_STA_T* prNmSta, const NR_NM_ANA_T* prNmAna);

typedef struct _NR_ADAP_WREG_BASE_T
{
    BOOL (*fgAutoOn)(void);
    BOOL (*fgFromSW)(void);
    void (*vSet)(void);
    void (*vReset)(void);
    UINT8 u1State;
} NR_ADAP_WREG_BASE_T;

typedef struct _NR_ADAP_MODEUL_T
{
    //void (*vModuleProc)(const NR_NM_STA_T* prNmSta, const NR_NM_ANA_T* prNmAna);
    ANR_FUNC_MODULE_PROC vModuleProc;
    NR_ADAP_WREG_BASE_T* parSubFunc;
    char* sDesc;
} NR_ADAP_MODULE_T;

EXTERN BOOL fgDrvAnrFromSWOff(void);

// Noise Level
EXTERN BOOL fgDrvAnrAutoOnNL(void);
EXTERN void vDrvAnrSetNL(void);
EXTERN void vDrvAnrResetNL(void);
EXTERN void vDrvAnrNLProc(UINT8 u1StrLvl);
EXTERN void _vDrvMCNRCleanDiffLvl(void);

// DC alpha
EXTERN BOOL fgDrvAnrAutoOnXNR(void);
EXTERN void vDrvAnrSetXNR(void);
EXTERN void vDrvAnrResetXNR(void);
EXTERN void vDrvAnrDCAlphaProc(UINT8 u1StrLvl);


// SNR
EXTERN BOOL fgDrvAnrAutoOnSNR(void);
EXTERN BOOL fgDrvAnrFromSWSNR(void);
EXTERN void vDrvAnrSetSNR(void);
EXTERN void vDrvAnrResetSNR(void);
EXTERN void vDrvAnrSnrProc(UINT8 u1StrLvl);


// TNR
EXTERN BOOL fgDrvAnrAutoOnTNR(void);
EXTERN BOOL fgDrvAnrFromSWTNR(void);
EXTERN void vDrvAnrSetTNR(void);
EXTERN void vDrvAnrResetTNR(void);
EXTERN void vDrvAnrTnrProc(UINT8 u1StrLvl);


// RNR
EXTERN BOOL fgDrvAnrAutoOnRNR(void);
EXTERN BOOL fgDrvAnrFromSWRNR(void);
EXTERN void vDrvAnrSetRNR(void);
EXTERN void vDrvAnrResetRNR(void);
EXTERN void vDrvAnrRnrProc(UINT8 u1StrLvl);


// MONR
EXTERN BOOL fgDrvAnrAutoOnMOANR(void);
EXTERN BOOL fgDrvAnrFromSWMOANR(void);
EXTERN void vDrvAnrSetMOANR(void);
EXTERN void vDrvAnrResetMOANR(void);
EXTERN void vDrvAnrMoAnrProc(UINT32 u4NoiseLevel, UINT32 u4MoIdx);


#ifdef DRV_SUPPORT_CUST_ANR
EXTERN void NR_FW_ANR_RegisterCustFunc(ANR_FUNC_MODULE_PROC vCustModuleFunc, NR_ADAP_WREG_BASE_T* prCustSubFunc);
#endif

#endif //_DRV_NR_INT_H_

