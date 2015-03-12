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
 * $RCSfile: drv_nr_int.c,v $
 * $Revision: #3 $
 *
 *---------------------------------------------------------------------------*/

////////////////////////////////////////////////////////////////////////////////
// Inclusions
////////////////////////////////////////////////////////////////////////////////
#define _DRV_NR_INT_C_

#include "video_def.h"
#include "nr_debug.h"
#include "drv_nr.h"
#include "drv_di_int.h"
#include "drv_bnr_int.h"
#include "drv_nr_int.h"

#include "hw_vdoin.h"
#include "hw_nr.h"
#include "hw_di.h"
#include "hw_di_int.h"
//#include "hw_sw.h"
#include "vdo_misc.h"

#include "pe_if.h"

#include "x_assert.h"
#include "x_os.h"
#include "x_bim.h"
#include "hw_mjcfd.h"
#include "source_select.h"

EXTERN void vDrvDISetInkColor(UINT32 u4Idx, UINT32 u4Color);
EXTERN void vDrvNRSemaUnlock(void);

void _vDRVR2CIntervalSet(void);

////////////////////////////////////////////////////////////////////////////////
// Variables for driver
////////////////////////////////////////////////////////////////////////////////

NR_NM_STA_T _rNmSta;

static UINT32 _u4EnMsk = 0;
static UINT8 _u1Init = 0;
static volatile UINT8 _u1NrProcFlg = 0;
UINT8 _u1Is_TVDNM_Stable =0;
UINT8 _u1TVDStable=0;

// NR MCNR_R2C_interval
static UINT8 bR2CInterval = 5;
static UINT8 bR2RSTFlag = SV_FALSE;

// variables for BNR SRAM R/W
static UINT8 _u1LRFreeRunCnt = 0;
static UINT16 _u2VBlkPxCnt   = 0;
static UINT16 _u2VBlkPxCntR  = 0;
static UINT16 _u2HBlkPxCnt   = 0;
static UINT16 _u2HBlkPxCntR  = 0;
static UINT32* _u4VBlkLineSum  = 0;
static UINT32* _u4VBlkLineSumR = 0;
static UINT32 _u4BLKPos[NX_BLKPOS_SIZE] = {0};
static UINT32* _u4BLKPosH = ((UINT32*)_u4BLKPos + 120);


#define _SFTX(x) (((x)&0xF)*2)
#define _SFTY(y) _SFTX(y)
#define RD_VBLK(x) ((_u4BLKPos[((x)>>4)] >> _SFTX(x)) & 0x3)
#define RD_HBLK(y) ((_u4BLKPosH[((y)>>4)] >> _SFTY(y)) & 0x3)
#define WR_VBLK(x, v) _u4BLKPos[(x)>>4] = (_u4BLKPos[(x)>>4] & (~(0x3 << _SFTX(x)))) | ((v) << _SFTX(x))
#define WR_HBLK(y, v) _u4BLKPosH[(y)>>4] = (_u4BLKPosH[(y)>>4] & (~(0x3 << _SFTY(y)))) | ((v) << _SFTY(y))

#define V_LS(x) (((*((UINT32*)_u4VBlkLineSum+((x)>>1))) >> (((x) & 0x1) ? 24 : 16)) & 0xFF)
#define H_LS(y) (((*((UINT32*)_u4VBlkLineSum+((y)>>1))) >> (((y) & 0x1) ? 8 : 0)) & 0xFF)
#define V_LS_R(x) (((*((UINT32*)_u4VBlkLineSumR+((x)>>1))) >> (((x) & 0x1) ? 24 : 16)) & 0xFF)
#define H_LS_R(y) (((*((UINT32*)_u4VBlkLineSumR+((y)>>1))) >> (((y) & 0x1) ? 8 : 0)) & 0xFF)

////////////////////////////////////////////////////////////////////////////////
// NR utilities
////////////////////////////////////////////////////////////////////////////////
UINT32 u4DrvNRUtilIIR(NR_IIR_T* rIIR, UINT8 u1Res, UINT32 u4Val)
{
#if 1
    u4Val <<= u1Res;
    rIIR->u4Sta = LERP(rIIR->u4Sta, u4Val, rIIR->u1Wei, u1Res);
    return (rIIR->u4Sta + W_ROUND(u1Res)) >> u1Res;
#else
    UINT32 u4StaPre = rIIR->u4Sta;
    UINT32 u4StaNew;
    u4StaNew = LERP(u4StaPre, u4Val, rIIR->u1Wei, 8);
    if ((u4StaNew == u4StaPre) && (u4StaPre != u4Val))
    {
        rIIR->u1Err = MIN((UINT32)255, (UINT32)rIIR->u1Err + 256 - rIIR->u1Wei);
    }
    else
    {
        rIIR->u1Err = 0;
    }
    
    if (rIIR->u1Err >= 128)
    {
        rIIR->u1Err = 0;
        u4StaNew = (u4Val > u4StaNew) ? (u4StaNew+1) :
            ((u4Val < u4StaNew) ? (MAX(u4StaNew,1)-1) : u4StaNew);
    }

    rIIR->u4Sta = u4StaNew;
    return u4StaNew;
#endif
}

UINT32 u4DrvNRUtilIIR_1(NR_IIR_T* rIIR, UINT8 u1Res, UINT32 u4Val)
{
    UINT32 u4Wei;
#if 1
    u4Val <<= u1Res;
    u4Wei =  (u4Val > rIIR->u4Sta) ? rIIR->u1Wei : rIIR->u1Wei_dec;
    
    rIIR->u4Sta = LERP(rIIR->u4Sta, u4Val, u4Wei, u1Res);
    return (rIIR->u4Sta + W_ROUND(u1Res)) >> u1Res;
#else
    UINT32 u4StaPre = rIIR->u4Sta;
    UINT32 u4StaNew;
    u4StaNew = LERP(u4StaPre, u4Val, rIIR->u1Wei, 8);
    if ((u4StaNew == u4StaPre) && (u4StaPre != u4Val))
    {
        rIIR->u1Err = MIN((UINT32)255, (UINT32)rIIR->u1Err + 256 - rIIR->u1Wei);
    }
    else
    {
        rIIR->u1Err = 0;
    }
    
    if (rIIR->u1Err >= 128)
    {
        rIIR->u1Err = 0;
        u4StaNew = (u4Val > u4StaNew) ? (u4StaNew+1) :
            ((u4Val < u4StaNew) ? (MAX(u4StaNew,1)-1) : u4StaNew);
    }

    rIIR->u4Sta = u4StaNew;
    return u4StaNew;
#endif
}


////////////////////////////////////////////////////////////////////////////////
// NR drivers
////////////////////////////////////////////////////////////////////////////////
static inline UINT32 _u4GetCurrLineCnt(void)
{
    return NR_R(MCVP_KC_2D, OUT_VDE_CNT);
}

UINT32 u4GetEnMsk(void)
{
    return _u4EnMsk;
}

static UINT32 *_pu4VBlkLSPtr(void)
{
    return ((_u1LRFreeRunCnt) ? _u4VBlkLineSumR : _u4VBlkLineSum);
}

UINT16 u2DrvNXNRGetVBlkPxCnt(BOOL fgR)
{
    return fgR ? _u2VBlkPxCntR : _u2VBlkPxCnt;
}

UINT16 u2DrvNXNRGetHBlkPxCnt(BOOL fgR)
{
    return fgR ? _u2HBlkPxCntR : _u2HBlkPxCnt;
}

UINT16 u2DrvNXNRGetVBlkLineSum(BOOL fgR, UINT32 u4Pos)
{
    u4Pos = MIN(u4Pos, 1920);
    return fgR ? V_LS_R(u4Pos) : V_LS(u4Pos);
}

UINT16 u2DrvNXNRGetHBlkLineSum(BOOL fgR, UINT32 u4Pos)
{
    u4Pos = MIN(u4Pos, 1080);
    return fgR ? H_LS_R(u4Pos) : H_LS(u4Pos);
}

void vDrvNXNRResetPos(void)
{
    x_memset(_u4BLKPos, 0, sizeof(UINT32)*NX_BLKPOS_SIZE);
}

void vDrvNXNRResetPosH(void)
{
    x_memset(_u4BLKPosH, 0, sizeof(UINT32)*NX_BLKPOS_SIZE_H);
}

void vDrvNXNRResetPosV(void)
{
    x_memset(_u4BLKPos, 0, sizeof(UINT32)*NX_BLKPOS_SIZE_V);
}

void vDrvNXNRWritePos(BOOL u1IsH, UINT32 u4Pos, UINT8 u1OnOff)
{
    if (u1IsH == 0)
    {
        WR_VBLK(MIN(u4Pos, 1919), (u1OnOff?3:0));
    }
    else
    {
        WR_HBLK(MIN(u4Pos, 1079), (u1OnOff?3:0));
    }
}

void vDrvNXNRWritePosH(UINT32 u4Pos, UINT8 u1OnOff)
{
    u1OnOff = (u1OnOff?3:0);
    WR_HBLK(u4Pos, u1OnOff);
}

void vDrvNXNRWritePosV(UINT32 u4Pos, UINT8 u1OnOff)
{
    u1OnOff = (u1OnOff?3:0);
    WR_VBLK(u4Pos, u1OnOff);
}

void vDrvNXNRWritePosLR(BOOL u1IsR, UINT32 u4Pos, UINT8 u1OnOff)
{
    UINT32 u4OnOff;
    UINT32 u4Msk;

    u4Pos = MIN(u4Pos, 1919);
    u4OnOff = RD_VBLK(u4Pos);
    u4Msk = u1IsR ? 2 : 1;
    u4OnOff = (u4OnOff & (~u4Msk)) | (u1OnOff?u4Msk:0);

    WR_VBLK(u4Pos, u4OnOff);
}

UINT8 u1DrvNXNRReadPos(BOOL u1IsH, UINT32 u4Pos)
{
    if (u1IsH == 0)
    {
        return RD_VBLK(MIN(u4Pos, 1919));
    }
    else
    {
        return RD_HBLK(MIN(u4Pos, 1079));
    }
}

void vDrvNXNRCopyLineSumTool(void)
{
    static UINT8 _u1Cnt = 0;

    _u1Cnt = _SWR(NR_CTRL_00, EN_TRIG_COPY_LS) && !_SWR(NR_BLK_STA_03, NR_BLK_READ_FAIL) ? _u1Cnt + 1 : 0;

    if (_u1Cnt >= 6)
    {
        x_memcpy((UINT32*)NR_VBLK_START, _SWR(NR_FWBNR_00, NR_FWBNR_READ_RIGHT) ? _u4VBlkLineSumR : _u4VBlkLineSum, sizeof(UINT32)*NX_WIDTH/2);
        _SWW(NR_CTRL_00, 0, EN_TRIG_COPY_LS);
    }
}

void vDrvMCNRSetDftDcnrAlpha(void)
{
    NR_W(MCNR_01, wReadQualityTable(QUALITY_3DNR_TNR_DCNR_ALPHA), MCNR_DCNR_ALPHA);
}

void vDrvMCNRSetWeiTbl(E_3DNR_IIR_TBL eIIRTbl, const UINT8 arTbl[8])
{
    switch (eIIRTbl)
    {
    case MCNR_IIR_TBL_MA:
        NR_WM(MCNR_0A, 
            P_Fld(arTbl[0], MCNR_TBL_MA_0)|P_Fld(arTbl[1], MCNR_TBL_MA_1)|P_Fld(arTbl[2], MCNR_TBL_MA_2)|
            P_Fld(arTbl[3], MCNR_TBL_MA_3)|P_Fld(arTbl[4], MCNR_TBL_MA_4));
        NR_WM(MCNR_0B,
            P_Fld(arTbl[5], MCNR_TBL_MA_5)|P_Fld(arTbl[6], MCNR_TBL_MA_6)|P_Fld(arTbl[7], MCNR_TBL_MA_7));
        break;
    case MCNR_IIR_TBL_MC:     
        NR_WM(MCNR_0B, P_Fld(arTbl[0], MCNR_TBL_MC_0)|P_Fld(arTbl[1], MCNR_TBL_MC_1));
        NR_WM(MCNR_0C,
            P_Fld(arTbl[2], MCNR_TBL_MC_2)|P_Fld(arTbl[3], MCNR_TBL_MC_3)|P_Fld(arTbl[4], MCNR_TBL_MC_4)|P_Fld(arTbl[5], MCNR_TBL_MC_5)|
            P_Fld(arTbl[6], MCNR_TBL_MC_6));
        NR_WM(MCNR_0D, P_Fld(arTbl[7], MCNR_TBL_MC_7));
        break;
    case MCNR_IIR_TBL_C:
        NR_WM(MCNR_0D,
            P_Fld(arTbl[0], MCNR_TBL_C_0)|P_Fld(arTbl[1], MCNR_TBL_C_1)|P_Fld(arTbl[2], MCNR_TBL_C_2)|P_Fld(arTbl[3], MCNR_TBL_C_3));
        NR_WM(MCNR_0E, 
            P_Fld(arTbl[4], MCNR_TBL_C_4)|P_Fld(arTbl[5], MCNR_TBL_C_5)|P_Fld(arTbl[6], MCNR_TBL_C_6)|P_Fld(arTbl[7], MCNR_TBL_C_7));
        break;
    }
}

void vDrvMCNRSetTblThd(const UINT8 arTbl[8])
{
    NR_WM(MCNR_2E,
        P_Fld(arTbl[0], MCNR_TBL_THD1)|P_Fld(arTbl[1], MCNR_TBL_THD2)|
        P_Fld(arTbl[2], MCNR_TBL_THD3)|P_Fld(arTbl[3], MCNR_TBL_THD4));
    NR_WM(MCNR_2F,
        P_Fld(arTbl[4], MCNR_TBL_THD5)|P_Fld(arTbl[5], MCNR_TBL_THD6)|
        P_Fld(arTbl[6], MCNR_TBL_THD7)|P_Fld(arTbl[7], MCNR_TBL_THD8));
}

void vDrvMCNRSetTblThdFixed(UINT8 u1EndPtVal)
{ 
    NR_WM(MCNR_2E,
        P_Fld(((u1EndPtVal+4)>>3), MCNR_TBL_THD1)|P_Fld(((u1EndPtVal*2+4)>>3), MCNR_TBL_THD2)|
        P_Fld(((u1EndPtVal*3+4)>>3), MCNR_TBL_THD3)|P_Fld(((u1EndPtVal*4+4)>>3), MCNR_TBL_THD4));
    NR_WM(MCNR_2F,
        P_Fld(((u1EndPtVal*5+4)>>3), MCNR_TBL_THD5)|P_Fld(((u1EndPtVal*6+4)>>3), MCNR_TBL_THD6)|
        P_Fld(((u1EndPtVal*7+4)>>3), MCNR_TBL_THD7)|P_Fld(u1EndPtVal, MCNR_TBL_THD8));
}

void vDrvNRSetNMDebugInk(UINT32 u4Enable)
{
    vDrvDISetDebugOSDModule(u4Enable ? E_OSD_DBG_NR : E_OSD_DBG_NONE);
    vDrvDISetInkColor(0, 0x89A95310);
    vDrvDISetInkColor(1, 0xDCBB8546);
    vDrvDISetInkColor(2, 0x89AADCC0);
    vDrvDISetInkColor(3, 0x11235566);
    vDrvDISetInkColor(4, 0x23134570);
    vDrvDISetInkColor(5, 0x98744455);
    NR_WM(MCVP_KC_21, P_Fld((u4Enable ? 4 : 0)|((_rNrPrm.u2Width >= 960)<<3), OSD_MODE)|P_Fld(5, OSD_Y_POS)|P_Fld(5, OSD_X_POS));
}

UINT8 u1DrvNXNRReadBlkLineSum(void)
{
    UINT32 u4Ret = 2;
    UINT32 u4VBlkDataSize = MIN((_rNrPrm.u2Width>>1), (NX_WIDTH>>1));
    UINT32 u4HBlkDataSize = MIN((_rNrPrm.u2Height>>1), (NX_HEIGHT>>1));
    UINT32 u4PacketSize   = MIN(NX_PACK_SIZE, u4VBlkDataSize);
    UINT16* pu2VBlkPxCnt = ((_u1LRFreeRunCnt) ? &_u2VBlkPxCntR : &_u2VBlkPxCnt);
    UINT16* pu2HBlkPxCnt = ((_u1LRFreeRunCnt) ? &_u2HBlkPxCntR : &_u2HBlkPxCnt);
    UINT32* pu4VBlkAddr = (UINT32*) _pu4VBlkLSPtr();

    if (pu4VBlkAddr == NULL || !u1DrvNRGetBlockMeterSupport() || u4VBlkDataSize == 0)
        return 0;
    
    _SWW(NR_BLK_STA_02, u4VBlkDataSize, NR_BLK_STA_VBLK_DATA_SIZE);
    _SWW(NR_BLK_STA_02, u4HBlkDataSize, NR_BLK_STA_HBLK_DATA_SIZE);

    if (_rNrPrm.u1Ver)
    {
        UINT32 u4Tmp = NR_R4B(NXNR_13);
        *pu2VBlkPxCnt = R_Fld(u4Tmp, NX_STA_VBLK_PIX_CNT);
        *pu2HBlkPxCnt = R_Fld(u4Tmp, NX_STA_HBLK_PIX_CNT);
        _SWW_4B(NR_BLK_STA_11, u4Tmp);
    }

    NR_W(NXNR_14, 1, NX_CPU_SEL_LINE_SUM_SRAM);
    //NR_W(NXNR_14, 0, NX_SRAM_AUTO_INC_ADDR);
    //NR_W(NXNR_15, 0, NX_INIT_SRAM_ADDR);
    //NR_R4B(NXNR_24);
    NR_W(NXNR_15, 0, NX_INIT_SRAM_ADDR);
    NR_W(NXNR_14, 1, NX_SRAM_AUTO_INC_ADDR);

    while (u4PacketSize--)
    {
        *pu4VBlkAddr++ = NR_R4B(NXNR_24);        
    }

    NR_W(NXNR_14, 0, NX_CPU_SEL_LINE_SUM_SRAM);

    return u4Ret;
}

UINT8 u1DrvNXNRWriteBlkPos(void)
{
    UINT32 i = 0;

    if (_u4BLKPos == NULL)
        return 0;
    
    NR_W(NXNR_14, 1, NX_CPU_SEL_LOC_SRAM);
    //NR_W(NXNR_14, 0, NX_SRAM_AUTO_INC_ADDR);
    //NR_W(NXNR_15, 0, NX_INIT_SRAM_ADDR);
    //NR_W4B(NXNR_27, 0);
    NR_W(NXNR_15, 0, NX_INIT_SRAM_ADDR);
    NR_W(NXNR_14, 1, NX_SRAM_AUTO_INC_ADDR);

    for (i = 0; i < NX_BLKPOS_SIZE; i++)
    {
        NR_W4B(NXNR_27, _u4BLKPos[i]);
    }

    NR_W(NXNR_14, 0, NX_CPU_SEL_LOC_SRAM);
    
    return 1;
}

static void _vDrvNXNRDeterminePos(void)
{
    UINT32 i = 0;
    UINT32 u4Width = _rNrPrm.u2Width;
    UINT32 u4Thr = _SWR(NR_BK_METER_02, NR_BK_THR);

    for (i = 1; i < u4Width-1; i++)
    {
        if ((V_LS(i) > u4Thr) && (V_LS(i) >= V_LS(i-1)) && (V_LS(i) >= V_LS(i+1)))
        {
            WR_VBLK(i, 3);
        }
        else
        {
            WR_VBLK(i, 0);
        }
    }
}

static void _vDrvMCNRReadSADHist(void)
{
#ifdef _WA_FHD_P_SADHIST_
    static UINT8 _u1Tag = 0;
#endif
    UINT8 i = 0;
    UINT32 u4Addr;
    UINT32 u4TmpVal;

    // FHD SAD Hist WA
#ifdef _WA_FHD_P_SADHIST_
    u4TmpVal = NR_R(MCNR_18, SAD_HIST_CNT_STAT);
	if (_rNrPrm.u1IsFhdP)
	{
		if (u4TmpVal == 0)
		{
			if (NR_R(MCNR_28, MCNR_ME_FHD_P))
			{
				NR_W(MCNR_28, 0, MCNR_ME_FHD_P);
				_u1Tag = 1;
			}			
		}
		else 
		{		
			if (NR_R(MCNR_28, MCNR_ME_FHD_P) == 0 && _u1Tag)
			{
				NR_W(MCNR_28, 1, MCNR_ME_FHD_P);
				_u1Tag = 0;
			}
		}
	}
#endif

    for (u4Addr = MCNR_19; u4Addr < MCNR_25; u4Addr += 4)
    {
        u4TmpVal = NR_R4B(u4Addr);
        _rNmSta.arSADHistCurr[i]   = R_Fld(u4TmpVal, SAD_HIST00_STAT);
        _rNmSta.arSADHistCurr[i+1] = R_Fld(u4TmpVal, SAD_HIST01_STAT);
        i += 2;        
    }

    _rNmSta.arSADHistCurr[SAD_HIST_LAST] = NR_R(MCNR_25, SAD_HIST24_STAT);
    _rNmSta.u4SADHistCntCurr = NR_R(MCNR_18, SAD_HIST_CNT_STAT);
}

static UINT8 _u1DrvNRStatusSel(UINT8 u1Sel)
{
    UINT8 u1Ret = 0;
    switch (u1Sel)
    {
    case 0:
        u1Ret = _SWR(NR_NMSTA_0E, NR_NM_STA_NL);
        break;
    case 1:
        u1Ret = _SWR(NR_NMSTA_0E, NR_NM_STA_CNTD);
        break;
    case 2:
        u1Ret = _SWR(NR_NMSTA_0E, NR_NM_STA_PEAK);
        break;
    case 3:
        u1Ret = _SWR(NR_NMSTA_0E, NR_NM_STA_FIRPEAK);
        break;
    case 4:
        u1Ret = _SWR(NR_NMSTA_0E, NR_NM_STA_PEAKMO);
        break;
    case 5:
        u1Ret = _SWR(NR_NMSTA_17, NR_NM_STA_FATTAIL_RTO);
        break;
    case 6:
        u1Ret = _SWR(NR_NMSTA_18, NR_NM_STA_MOFBK_RTO);
        break;
    case 7:
        u1Ret = _SWR(NR_MO_STA_01, STA_MO_IDX);
        break;
#ifndef BNR_NEW_FW
    case 9:
        u1Ret = NR_R(NXNR_0D, NX_MANUAL_VBLK_CONFIDENCE);
        break;
    case 10:
        u1Ret = _SWR(NR_BLK_STA_0F, NR_BLK_FOUND_SP);
        break;
    case 11:
        u1Ret = _SWR(NR_BLK_STA_0F, NR_BLK_EST_SIZE);
        break;
    case 12:
        u1Ret = _SWR(NR_BLK_STA_0F, NR_BLK_EST_PERD);
        break;
#else
    case 8:
        u1Ret = _SWR(NR_STA_BNR_02, NR_STA_BNR_HBLK_CONF);
        break;
    case 9:
        u1Ret = _SWR(NR_STA_BNR_02, NR_STA_BNR_VBLK_CONF);
        break;
    case 10:
        u1Ret = _SWR(NR_STA_BNR_03, NR_STA_BNR_VBLK_LVL_LO);
        break;
    case 11:
        u1Ret = _SWR(NR_STA_BNR_03, NR_STA_BNR_VBLK_LVL_HI);
        break;
    case 12:
        u1Ret = _SWR(NR_STA_BNR_03, NR_STA_BNR_HBLK_LVL_LO);
        break;
    case 13:
        u1Ret = _SWR(NR_STA_BNR_03, NR_STA_BNR_HBLK_LVL_HI);
        break;
    case 14:
        u1Ret = _SWR(NR_STA_BNR_02, NR_STA_BNR_GRID_DET_FLG);
        break;
#endif
    case 15:
        u1Ret = _SWR(NR_BLK_STA_13, NR_STA_BFW_CNT);
        break;
    case 16:
        u1Ret = _SWR(NR_MO_STA_00, STA_TNR_STR);
        break;
    case 17:
        u1Ret = _SWR(NR_MO_STA_00, STA_SNR_STR);
        break;
    case 18:
        u1Ret = _SWR(NR_MO_STA_00, STA_NL_IDX);
        break;
    }

    return u1Ret;
}

static void _vDrvMCNRUpdateOSDInk(void)
{
    UINT8 u1DbgSel0 = _SWR(NR_NM_DBG_00, NM_DBG_SEL_0);
    UINT8 u1DbgSel1 = _SWR(NR_NM_DBG_00, NM_DBG_SEL_1);
    UINT8 u1DbgSel2 = _SWR(NR_NM_DBG_00, NM_DBG_SEL_2);
    UINT8 u1DbgSel3 = _SWR(NR_NM_DBG_00, NM_DBG_SEL_3);
    
    UINT32 u4RegVal = _u1DrvNRStatusSel(u1DbgSel0)|((UINT32)_u1DrvNRStatusSel(u1DbgSel1)<<8)|
        ((UINT32)_u1DrvNRStatusSel(u1DbgSel2)<<16)|((UINT32)_u1DrvNRStatusSel(u1DbgSel3)<<24);

    _SWW_4B(NR_NMSTA_0D, u4RegVal);
    vDrvDISetDebugOSD(E_OSD_DBG_NR, u4RegVal);

    if (_rNmSta.u1ScnChg && _EN(EN_LOG_SCNCHG))
        LOG(3, "Scene Change: Peak = %d, HOMO = %d, TH = %d\n",
        _SWR(NR_NMSTA_0E, NR_NM_STA_PEAK), _rNmSta.u4PreHomoSum, _rNmSta.u4HomoSumDiffTh);
}

void vDrvNRPrintNMStatus(void)
{
//    LOG(3, "Peak Bin = %d\n", _rNmSta.u1PeakBin);
//    LOG(3, "Peak Bin MO = %d\n", _rNmSta.u1PeakBinMo);
//    LOG(3, "Centroid = %d\n", _rNmSta.u1Centroid);
    LOG(3, "----------\n");
//    LOG(3, "(MvApprove) = (%d)\n", _rNmSta.u1MvApprove);
    LOG(3, "----------\n");        
//    LOG(3, "NoiseLevel, Valid, FBCnt = %d, %d, %d\n", _rNmSta.u1NoiseLevel, _rNmSta.u1ValidNoiseLevel, _rNmSta.u1FbCnt);
//    LOG(3, (_rNmSta.u1FbkSta) ? "FbkSta = E_NR_FB_MCNR\n" : "FbkSta = E_NR_FB_MANR\n");
//    LOG(3, "FbkStep = %d\n", _rNmSta.u1FbkStep);
    LOG(3, "----------\n");
//    LOG(3, "GMV (Valid, X, Y) = (%d, %d, %d)\n", _rNmSta.u1GmvValid, _rNmSta.u1GmvX, _rNmSta.u1GmvY);
//    LOG(3, "SNR THM = %d\n", _rSnrTh.u2SnrThm);
}

static void _vScnChgForceMA(void)
{
    static UINT8 _u1PrevOnOff = 0xFF;
    UINT8 u1OnOff = _SWR(NR_CTRL_01, EN_FORCE_MA_SC);

    if (u1OnOff)
    {
        if (_rNmSta.u1ScnChgCnt == _SWR(NR_NM_07, NM_SC_HIST_SKIP))
        {    	
            NR_WM(MCNR_11, P_Fld(1, MCNR_FORCE_ALPHA_MC_EN)|P_Fld(0, MCNR_FORCE_ALPHA_MC));
        }
        else if (_rNmSta.u1ScnChgCnt == _SWR(NR_NM_07, NM_SC_HIST_SKIP) - 1)
        {		
            NR_WM(MCNR_11, P_Fld(0, MCNR_FORCE_ALPHA_MC_EN)|P_Fld(0, MCNR_FORCE_ALPHA_MC));
        }
    }
    else if (u1OnOff != _u1PrevOnOff)
    {
        NR_WM(MCNR_11, P_Fld(0, MCNR_FORCE_ALPHA_MC_EN)|P_Fld(0, MCNR_FORCE_ALPHA_MC));
    }

    _u1PrevOnOff = u1OnOff;
}

static void _vPatch8Bit(void)
{
    static UINT8 _u1PrevOnOff = 0xFF;
    UINT8 u1OnOff = _SWR(NR_CTRL_01, EN_PATCH_8BIT);
    
    // patch for 8bit
    if (u1OnOff || _u1PrevOnOff)
    {
        UINT32 u4Pause = u1OnOff ? (_rNmSta.u1Pause?1:0) : 0;
        NR_WM(MCNR_11,
            P_Fld(u4Pause, MCNR_FORCE_MA_WEI_Y_EN)|
            P_Fld(u4Pause, MCNR_FORCE_MC_WEI_Y_EN)|
            P_Fld(u4Pause, MCNR_FORCE_C_WEI_EN));
    }

    _u1PrevOnOff = u1OnOff;
}

static BOOL _fgIsFrequentlyScnChg(BOOL fgScnChg)
{
    static UINT8 _u1State = 0;
    static UINT8 _u1AccCnt = 0;
    static UINT8 _u1FrmCnt = 0;
    BOOL fgRet = 0;

    switch (_u1State)
    {
    case 0:
        fgRet = 0;
        if (fgScnChg)
        {
            _u1AccCnt = 1;
            _u1FrmCnt = 0;
            _u1State = 1;
        }
        break;
    case 1:
        fgRet = 0;
        if (fgScnChg)
        {
            _u1AccCnt ++;
        }
        _u1FrmCnt ++;

        if (_u1FrmCnt >= 20)
        {
            if (_u1AccCnt >= 6)
            {
                _u1AccCnt = 0;
                _u1FrmCnt = 0;
                _u1State = 2;
                fgRet = 1;
            }
            else
            {
                _u1State = 0;
            }
        }
        break;
    case 2:
        fgRet = 1;
        _u1FrmCnt = fgScnChg ? 0 : _u1FrmCnt+1;
        if (_u1FrmCnt >= (16 + _SWR(NR_NM_07, NM_SC_HIST_SKIP)))
        {
            _u1State = 0;
            fgRet = 0;
        }
        break;
    }

    //LOG(5, "_u1State = %d, Freq ScnChg = %d, _u1FrmCnt = %d, _u1AccCnt = %d\n", _u1State, fgRet, _u1FrmCnt, _u1AccCnt);
    
    return fgRet;
}

static void _vScnChgCnt(void)
{
    UINT8 u1ScSkip = 0;
    static UINT8 _u1ScnChgMotion =0xFF;
    static UINT8 _u1ScnChg =0;

    if (_rNmSta.u1ScnChg)
    {
        u1ScSkip = _SWR(NR_NM_07, NM_SC_HIST_SKIP);
        _u1ScnChg =1;
        _rNmSta.u1NLScnChgCnt = 0;
        LOG(1,"===============> NR Scene Change  \n");
    }
    else
    {
        u1ScSkip = (_rNmSta.u1ScnChgCnt > 1) ? (_rNmSta.u1ScnChgCnt - 1) : _rNmSta.u1ScnChgCnt;
        
        //check if still at scene change update time
        //if still , need  to update NL second time when motion
        if ((u1ScSkip ==1) && (_u1ScnChg ==1) && _SWR(NR_NM_07, NM_SC_MO_EN))
        {
            _u1ScnChgMotion = (_rNmSta.u4HomoSum > 10) ? 1 : 0;
            _u1ScnChg =0;
            LOG(1,"===============>Method1: NR Motion= %d \n", _u1ScnChgMotion);
        }

        //check if NL updated at scene change is smaller than MOTNR_MIN_NL_TH
        //if NL smaller than MOTNR_MIN_NL_TH, need to update NL second time when motion
        if ((_u1ScnChg ==1) && (_rNmSta.u1NLScnChgCnt==0) &&(_rNmSta.u1ScnChgCnt==0)&& _SWR(NR_NM_07, NM_SC_MO_MTHD2_EN))
        {
            //if (_rNmAna.u1TargetNL < _SWR(NR_AUTO_02, MOTNR_MIN_NL_TH))
            if (_rNmSta.u4HomoSum < 10)
            {
                if (_SWR(NR_NM_07, NR_NM_MTHD1))
                {
                    _u1ScnChgMotion = (_rNmAna.u1TargetNL < _SWR(NR_AUTO_02, MOTNR_MIN_NL_TH)) ? 0 : 1;
                }
                else
            {
                _u1ScnChgMotion = 0;
            }
            }
            else
            {
                _u1ScnChgMotion = 1;
            }
            
                _u1ScnChg =0;
            
            LOG(1,"===============>Method2: NR Motion= %d ; u4HomoSum =%d ; target NL =%d ; current NL = %d \n", _u1ScnChgMotion ,_rNmSta.u4HomoSum  ,_rNmAna.u1TargetNL, _SWR(NR_NMSTA_0E, NR_NM_STA_NL)); 
        }
        
    }

    //update NL second time when motion
    if ((_u1ScnChgMotion !=1) && (_rNmSta.u4HomoSum > _SWR(NR_NM_08, NM_SC_MO_TH)))
    {
        if (_SWR(NR_NM_07, NR_NM_SC_MO_CONTI_EN))
        {
            if (_u1ScnChgMotion ==0)
            {
                u1ScSkip = _SWR(NR_NM_07, NM_SC_HIST_SKIP_MO);
                _u1ScnChgMotion =2;
            }
                
            //if still happens during skip_mo period, reset skip_mo again => need to wait skip_mo frames
            if ((_rNmSta.u4HomoSum <= _SWR(NR_NM_08, NM_SC_MO_TH))&& (u1ScSkip >0))
            {
                u1ScSkip = _SWR(NR_NM_07, NM_SC_HIST_SKIP_MO);
                LOG(1,"===============> NR Scene Change   and   Motion  : Reset Skip_Mo \n");
            }   
            else
            {
                _u1ScnChgMotion =1;
                _rNmSta.u1NLScnChgCnt = 0;
                LOG(1,"===============> NR Scene Change   and   Motion : update NL \n");
            }
        }
        else
        {
        u1ScSkip = _SWR(NR_NM_07, NM_SC_HIST_SKIP_MO);
        _u1ScnChgMotion =1;
        _rNmSta.u1NLScnChgCnt = 0;
        LOG(1,"===============> NR Scene Change   and   Motion \n");
        }

    } 
    _rNmSta.u1ScnChgCnt = u1ScSkip;
}

static void _vNLScnChgCnt(void)
{
    UINT8 _u1ScSkip = 0;

    if (_rNmSta.u1ScnChgCnt == 1)
    {
        _u1ScSkip = 16;
        _rNmSta.u1ScnChgCnt = 0;
    }
    else
    {
        _u1ScSkip = _rNmSta.u1NLScnChgCnt ? _rNmSta.u1NLScnChgCnt - 1 : 0;
    }

    _rNmSta.u1NLScnChgCnt = _u1ScSkip;
}

static UINT8 _u1DrvNRPerfectStill(void)
{
    static UINT8 _u1Idx = 0;
    static UINT8 _u1Cnt = 0;
    static UINT32 _u4PrevCrc[2] = {0};
    
    UINT32 u4Crc = NR_R(MCVP_KC_28, OUT_CRC_OUT_V);

    if (u4Crc == _u4PrevCrc[_u1Idx])
    {
        if (_u1Cnt < 20)
            _u1Cnt ++;
    }
    else
    {
        _u1Cnt = 0;
    }

    _u4PrevCrc[_u1Idx] = u4Crc;

    _u1Idx = (_u1Idx + 1) & 0x1;

    return _u1Cnt;
}

static void _vDrvNRCalcScnChgHomoSum(void)
{
    static UINT8 _u1PauseState = 0;
    UINT32 u4Gain = _SWR(NR_NM_0E, NM_SCNCHG_DET_TH_GAIN);
    UINT32 u4Max = _SWR(NR_NM_0E, NM_SCNCHG_DET_TH_MAX);
    UINT32 u4Min = _SWR(NR_NM_0C, NM_SCNCHG_DET_TH_MIN);

    UINT32 u4Diff = AbsDiff(_rNmSta.u4PreHomoSum, _rNmSta.u4HomoSum);

    UINT32 u4Th = (_rNmSta.u4PreHomoSum * u4Gain) >> 2;

    UINT8 u1PerfectStillCnt = _u1DrvNRPerfectStill();
    
    u4Th = MAX(MIN(u4Th, u4Max), u4Min);

    // scene change detection
    _SWW(NR_NMSTA_0F, ((u4Diff > u4Th) ? 1 : 0), NR_NM_HOMO_SCNCHG);
    _SWW(NR_NMSTA_10, u4Th, NR_NM_HOMO_SCNHG_TH);

    // patch for 8bit remain
    if (10 == NR_R(MCNR_01, MCNR_RND_PRECISION))
    {
        switch (_u1PauseState)
        {
        case 0:
            if ((_rNmSta.u4MoPixCnt == 0 && _rNmSta.u4PreMoPixCnt != 0 &&
                _rNmSta.u4HomoSum == 0 && _rNmSta.u4PreHomoSum != 0) ||
                u1PerfectStillCnt == 10)
            {
                _u1PauseState = 1;
            }
            break;
        case 1:
            if (_rNmSta.u4MoPixCnt == 0 && _rNmSta.u4PreMoPixCnt == 0 &&
                _rNmSta.u4HomoSum == 0 && _rNmSta.u4PreHomoSum == 0)
            {
                _rNmSta.u1Pause ++;
                if (_rNmSta.u1Pause >= 4)
                    _u1PauseState = 2;
            }
            else
            {
                _rNmSta.u1Pause = 0;
                _u1PauseState = 0;
            }
            break;
        case 2:
            if (_rNmSta.u1Pause)
            {
                _rNmSta.u1Pause --;
            }
            if (_rNmSta.u1Pause == 0 || _rNmSta.u4MoPixCnt || _rNmSta.u4HomoSum)
            {
                _rNmSta.u1Pause = 0;
                _u1PauseState = 0;
            }
            break;
        }        
    }
    else
    {
        _rNmSta.u1Pause = 0;
        _u1PauseState = 0;
    }

    _rNmSta.u4PreMoPixCnt = _rNmSta.u4MoPixCnt;
    _rNmSta.u4PreHomoSum = _rNmSta.u4HomoSum;
    _rNmSta.u4HomoSumDiffTh = u4Th;
}

#if 0
static UINT8 _u1DrvNRCalcForceScnChg(void)
{
    static UINT8 _u1PrevNL = 0xFF;
    static UINT8 _u1Cnt = 0;

    UINT8 u1CurrNL = _rNmSta.u1NoiseLevel; // _u1DrvNRGetNLForQ();
    UINT8 u1Ret = 0;

    if ((u1CurrNL == _u1PrevNL) /*&& (u1CurrNL != _rNmSta.u1NoiseLevel)*/)
    {
        if (_u1Cnt < 0xFF)
            _u1Cnt ++;
    }
    else
    {
        _u1Cnt = 0;
    }

    if (_u1Cnt > 240)
    {
        _u1Cnt = 0;
        //u1Ret = 1;
    }

    _u1PrevNL = (UINT8) u1CurrNL;

    return u1Ret;
}
#else

#define FRAME_NUM 16
static UINT8 _u1DrvNRCalcForceScnChg(void)
{
    UINT8 u1Ret = 0;
    UINT8 u1IsMotion=0;
    UINT8 i;
    //static UINT8 u1FrameSeq[FRAME_NUM]=0;
    static UINT16 u2FrameSeq =0;
    static UINT8 u1FrmCnt =0;
    static UINT8 u1FwdRewCnt =0;
    static UINT8 u1ScnChgCnt =0;
    static UINT8 u1ScnChgCnt1 =0;
    static UINT8 u1FwdRewStart =0;
    static UINT8 u1FwdRewStart1 =0;
    static UINT8 u1FrameStillCnt =0;
    static UINT8 u1TVDStable =0;
    
    UINT8 u1FwdRewTH = IO32ReadFldAlign(NR_NM_10, NM_FWDREW_CNT_TH);
    UINT8 u1WaitFrmCnt = IO32ReadFldAlign(NR_NM_10, NM_FWDREW_WAIT_CNT);
    //UINT16 u2FilmSeq = IO32ReadFldAlign(MJCFD_SEQ, MJCFD_FD_DET_SEQ_L);
    //UINT32 u4DIFilmSeq = IO32ReadFldAlign(MCVP_STATUS_09, FRAME_ST_SEQ_STATUS);

    UINT8 u1FrmStillCnt_MIN = IO32ReadFldAlign(NR_NM_15, NM_FRAME_STILL_CNT_MIN);
    UINT8 u1FrmStillCnt_MAX = IO32ReadFldAlign(NR_NM_15, NM_FRAME_STILL_CNT_MAX);

    if (IO32ReadFldAlign(NR_NM_14, NM_NL_FWRW_UPDATE)==0)
    {
        return 0;
    }
    //calculate the motion sequence
    u1IsMotion = (_rNmSta.u4HomoSum <50) ? 0 :1;
    u2FrameSeq = (u2FrameSeq & (~(1<<u1FrmCnt))) | 
                        ((u1IsMotion<<u1FrmCnt) & (1<<u1FrmCnt));
    vIO32WriteFldAlign(NR_NMSTA_19, u2FrameSeq, STA_NM_FRAME_SEQ);
    u1FrmCnt = (u1FrmCnt< (FRAME_NUM-1)) ? (u1FrmCnt+1) : 0;

    //LOG(3, "u2FrameSeq =%x\n",u2FrameSeq );
    if (IO32ReadFldAlign(NR_NM_14, NM_NL_FWRW_LOG))
    {
        LOG(1, "u2FrameSeq =%x\n",u2FrameSeq );
    }

    // mothod 2 : check still pattern 
    if ((u1FwdRewStart1==0) && (IO32ReadFldAlign(NR_NM_14, NM_FWDREW_DET_2)))
    {
        if (u2FrameSeq ==0)
        {
            u1FrameStillCnt++;
        }
        else
        {
            if ((u1FrameStillCnt > u1FrmStillCnt_MIN) && (u1FrameStillCnt < u1FrmStillCnt_MAX))
            {
                u1FwdRewStart1 =1;
                LOG(1, "NewFWBW stop :u1FrameStillCnt=%d \n" ,u1FrameStillCnt );
            }
            u1FrameStillCnt=0;
        }
    }
    else if (u1FwdRewStart1==1)
    {
        u1ScnChgCnt1++;
        if (u1ScnChgCnt1 > IO32ReadFldAlign(NR_NM_14, NM_FWDREW_STOP_DLY))
        {
            u1Ret =1;
            u1FwdRewStart1 =0;
            u1ScnChgCnt1 =0;
            LOG(1, "New NL update : FWBW\n" );
        }
    }
    
    //method 1: check seq : 100000001000000100000 or 110000000011000000110000
    u1FwdRewCnt =0;
    if (IS_INTERLACE(VDP_1))
    {
        for (i=0 ; i<15 ; i++)
        {
            if (u2FrameSeq == (3<<i))
            {
                u1FwdRewCnt++;
            }
        }

        //special case
        if (u2FrameSeq == 0x8001)
        {
            u1FwdRewCnt++;
        }
    }
    else
    {
        for (i=0 ; i<16 ; i++)
        {
            if (u2FrameSeq == (1<<i))
            {
                u1FwdRewCnt++;
            }
        }
    }

    if (u1FwdRewStart <2)
    {
        if (u1FwdRewCnt)
        {
            u1ScnChgCnt =(u1ScnChgCnt<u1FwdRewTH) ?  u1ScnChgCnt+1 : u1ScnChgCnt;
        }
        else if (u2FrameSeq !=0)
        {
            u1ScnChgCnt = (u1ScnChgCnt > 0) ? (u1ScnChgCnt-1) : 0;
        }
    }
    vIO32WriteFldAlign(NR_NMSTA_18, u1ScnChgCnt, STA_NM_SCNCHG_CNT);
    
    //FWBW method 1 : default OFF
    if ((u1FwdRewStart ==0) &&(IO32ReadFldAlign(NR_NM_14, NM_FWDREW_DET_1)))
    { 
        if (u1ScnChgCnt == u1FwdRewTH)
        {
            u1FwdRewStart =1;
        }
    }
    else if (u1FwdRewStart ==1) 
    {
        if (u1ScnChgCnt <=MAX(0, u1FwdRewTH-u1WaitFrmCnt))
        {
            //u1Ret =1;
            LOG(1, "FWBW stop : u2FrameSeq =%x\n",u2FrameSeq );
            u1FwdRewStart =2;
            u1ScnChgCnt =0;
        }
    }
    else if (u1FwdRewStart ==2)
    {
        u1ScnChgCnt++;
        if (u1ScnChgCnt > IO32ReadFldAlign(NR_NM_14, NM_FWDREW_STOP_DLY))
        {
            u1Ret =1;
            u1FwdRewStart =0;
            u1ScnChgCnt =0;
            LOG(1, "NL update : u2FrameSeq =%x\n",u2FrameSeq );
        }
    }

     if (bGetVideoDecTypeNew(SV_VP_MAIN) == SV_VD_TVD3D)
    {
        if ((_u1Is_TVDNM_Stable ==1) && (u1TVDStable==0))
        {
            u1Ret =1;
            u1TVDStable =1;
            LOG(1, " New NL update : TVD stable \n" );
        }

        //TVD is unstable or other input, clear the flag
        if (_u1Is_TVDNM_Stable ==0)
        { 
            u1TVDStable =0;
        }
    }
    vIO32WriteFldAlign(NR_NMSTA_18, u1FwdRewStart, STA_NM_FWDREW_START);
    vIO32WriteFldAlign(NR_NMSTA_18, u1Ret, STA_NM_FORCE_SCN);

    return u1Ret;
}


#endif

static void _vDrvMCNRReadHWStatus(void)
{
    UINT8 u1AbsX, u1AbsY, u1ForceChg;

    if (!_u1Init)
        return;

    _rNmSta.u1FlfbStatus = NR_R(MCVP_CS_29, B_FLFB_STATUS);
    _rNmSta.u1CompositeStatus = NR_R(MCVP_CS_29, COMPOSITE_STATUS);
    _rNmSta.u2Nzc = NR_R(MCVP_CS_29, CS_NON_ZERO_CNT);
    _rNmSta.u1GmvValid = NR_R(MCVP_CS_28, GMV_VALID);
    _rNmSta.u1GmvRatio = NR_R(MCNR_18, MCNR_GMV_RATIO);
    _rNmSta.u1FavorCSState = _SWR(PSCAN_FWCS_02, FAVOR_CS_STATE);
    _rNmSta.u4HomoSum = NR_R(MCVP_FUSION_13, IF_PD_SUM_HOMO_UP) + NR_R(MCVP_FUSION_15, IF_PD_SUM_HOMO_DN);
    _rNmSta.u4StaDetail = NR_R(MCVP_CS_1C, STA_DETAIL);
    _rNmSta.u4EdgeCnt = NR_R(MCVP_CS_18, STA_EDGE_CNT);
    _rNmSta.u2HistMaxCnt = NR_R(MCVP_CS_27, STA_MAX_CNT1);
    _rNmSta.u4TmprDiffSum = NR_R(MCNR_2A, SUM_TMPRL_DIFF_STAT);
    _rNmSta.u4TmprDiffCnt = NR_R(MCNR_2B, TMPRL_DIFF_CNT_STAT);
    _rNmSta.u4MoPixCnt = NR_R(MCVP_STATUS_0F, OLD_FRAME_MO_Q_STATUS);
    _rNmSta.u4HistDiffAcc = NR_R(MCVP_CS_2A, HIST_DIFF_ACC);

    _vDrvNRCalcScnChgHomoSum();
    u1ForceChg = _u1DrvNRCalcForceScnChg();
    
    if (_SWR(NR_NM_10, NM_SCNCHG_DET_FORCE_OFF))
    {
        _rNmSta.u1ScnChg = 0;
    }
    else
    {
        BOOL fgScnChg = (_SWR(NR_NM_0E, NM_SCNCHG_DET_EN) ? 
            _SWR(NR_NMSTA_0F, NR_NM_HOMO_SCNCHG) : NR_R(MCVP_CS_29, SCENE_CHANGE_STATUS));
        BOOL fgIsFreqScnChg = _fgIsFrequentlyScnChg(fgScnChg);
        fgScnChg = fgScnChg && !fgIsFreqScnChg;
        _rNmSta.u1ScnChg = fgScnChg || u1ForceChg;
        if (_rNmSta.u1ScnChg)
        {
            LOG(1,"=====>fgScnChg = %d , u1ForceChg = %d \n ",fgScnChg,u1ForceChg );
        }    
    }

    u1AbsX = NR_R(MCVP_CS_28, GMV_MVX);
    u1AbsY = NR_R(MCVP_CS_28, GMV_MVY);

    u1AbsX = (u1AbsX & 0x10) ? (((~u1AbsX)&0x1F)+1) : u1AbsX;
    u1AbsY = (u1AbsY & 0x8) ? (((~u1AbsY)&0xF)+1) : u1AbsY;

    _rNmSta.u1GmvX = u1AbsX;
    _rNmSta.u1GmvY = u1AbsY;

    _vScnChgCnt();

    _SWW(NR_NMSTA_0F, _rNmSta.u1FavorCSState, NR_NM_STA_DI_FAVOR_CS_LVL);

    _vDrvMCNRReadSADHist();
}

void vDrvNRPQInit(void)
{
    x_memset(&_rNmSta, 0, sizeof(NR_NM_STA_T));

    _u4VBlkLineSum  = (UINT32*) x_mem_alloc(sizeof(UINT32)*NX_WIDTH/2);
    _u4VBlkLineSumR = (UINT32*) x_mem_alloc(sizeof(UINT32)*NX_WIDTH/2);

    if (_u4VBlkLineSum == NULL || _u4VBlkLineSumR == NULL)
    {
        LOG(0, "Memory allocation for block meter failed.\n");
    }
    else
    {
        x_memset(_u4VBlkLineSum,  0, sizeof(UINT32)*NX_WIDTH/2);
        x_memset(_u4VBlkLineSumR, 0, sizeof(UINT32)*NX_WIDTH/2);
    }
    

    NR_WM(MCNR_00, P_Fld(1, MCNR_FORCE_SCENE_CHANGE_EN)|P_Fld(0, MCNR_FORCE_SCENE_CHANGE));
    NR_W(MCNR_00, 2, MCNR_ACDC_MODE);
    NR_W(MCNR_01, 0, MCNR_DCNR_ALPHA);
    NR_W(MCNR_3A, 4, DNR0_MV0_CONF_TH); 
    //R2C
    NR_W(MCNR_00, 5, MCNR_R2C_INTERVAL);    

    //DNR
    NR_W(MCNR_3B, 0, DNR0_MA_ADJUST_SEL);    
    NR_W(MCNR_3E, 0, MCNR_DITHER_PHASE_R);        
    NR_W(MCNR_3C, 0, MCNR_ROUNDING_EN);     
    NR_W(MCNR_3E, 0, MCNR_DITHER_FPHASE_EN);
    
    NR_W(NXNR_2B, 0x0C, NX_VBLK_BASE_FILTER_STR);
    NR_W(NXNR_2C, 0x80, NX_VBLK_TXTR_BASE_STR_SLOPE);
    NR_W(NXNR_2C, 0x10, NX_VBLK_TXTR_BASE_STR_TH);

    //snr
    NR_W(NXNR_04, 4, NX_SNR_GAIN_Y);
	NR_W(NXNR_04, 0x10, NX_BNR_GAIN);
	
    _SWW_4B(NR_CTRL_00, 0x80011434);
    
    _SWW_4B(NR_NM_00, 0x00AC8120);
    _SWW_4B(NR_NM_03, 0x0149A810);  // clip noise level at 8 (0x20)
    _SWW_4B(NR_NM_04, 0x000026FF);

    _SWW_4B(NR_PQ_07, 0xA28A2847); //0x318C6047

    // MA Table
    _SWW(NR_PQ_00, 32, NR_TBL_MA_0);
    _SWW(NR_PQ_00, 40, NR_TBL_MA_1);
    _SWW(NR_PQ_00, 40, NR_TBL_MA_2);
    _SWW(NR_PQ_00, 32, NR_TBL_MA_3);
    _SWW(NR_PQ_00, 20, NR_TBL_MA_4);
    _SWW(NR_PQ_01, 15, NR_TBL_MA_5);
    _SWW(NR_PQ_01, 10, NR_TBL_MA_6);
    _SWW(NR_PQ_01,  5, NR_TBL_MA_7);
    // MA Table End
    _SWW(NR_PQ_03, 36, NR_TBL_MA_E_0);
    _SWW(NR_PQ_03, 48, NR_TBL_MA_E_1);
    _SWW(NR_PQ_03, 44, NR_TBL_MA_E_2);
    _SWW(NR_PQ_03, 30, NR_TBL_MA_E_3);
    _SWW(NR_PQ_04, 22, NR_TBL_MA_E_4);
    _SWW(NR_PQ_04, 16, NR_TBL_MA_E_5);
    _SWW(NR_PQ_04, 12, NR_TBL_MA_E_6);
    _SWW(NR_PQ_04,  5, NR_TBL_MA_E_7);
    // MA C Table
    _SWW(NR_PQ_0C, 32, NR_TBL_C_0);
    _SWW(NR_PQ_0C, 40, NR_TBL_C_1);
    _SWW(NR_PQ_0C, 40, NR_TBL_C_2);
    _SWW(NR_PQ_0C, 32, NR_TBL_C_3);
    _SWW(NR_PQ_0C, 20, NR_TBL_C_4);
    _SWW(NR_PQ_0D, 15, NR_TBL_C_5);
    _SWW(NR_PQ_0D, 10, NR_TBL_C_6);
    _SWW(NR_PQ_0D,  5, NR_TBL_C_7);
    // MAX Wei
    _SWW(NR_PQ_05, 23, NR_IIR_MAX_WEI);
    _SWW(NR_PQ_05,  0, NR_IIR_MAX_WEI_E);
    // MA Thd
    _SWW(NR_PQ_0E, 0x6,  NR_TBL_THD_1);
    _SWW(NR_PQ_0E, 0xC,  NR_TBL_THD_2);
    _SWW(NR_PQ_0E, 0x12, NR_TBL_THD_3);
    _SWW(NR_PQ_0E, 0x18, NR_TBL_THD_4);
    _SWW(NR_PQ_0F, 0x1E, NR_TBL_THD_5);
    _SWW(NR_PQ_0F, 0x24, NR_TBL_THD_6);
    _SWW(NR_PQ_0F, 0x2A, NR_TBL_THD_7);
    _SWW(NR_PQ_0F, 0x30, NR_TBL_THD_8);
    // MA Thd End
    _SWW(NR_PQ_10,  8, NR_TBL_THD_E_1);
    _SWW(NR_PQ_10, 16, NR_TBL_THD_E_2);
    _SWW(NR_PQ_10, 28, NR_TBL_THD_E_3);
    _SWW(NR_PQ_10, 40, NR_TBL_THD_E_4);
    _SWW(NR_PQ_11, 50, NR_TBL_THD_E_5);
    _SWW(NR_PQ_11, 60, NR_TBL_THD_E_6);
    _SWW(NR_PQ_11, 80, NR_TBL_THD_E_7);
    _SWW(NR_PQ_11,100, NR_TBL_THD_E_8);
	//rnr
    _SWW(NR_PQ_17,0x200, RNR_AGG_THM_OFST);
    // SNR Gain
    #if 0
    _SWW(NR_PQ_06,  0, SNR_MAX_RTO_LVL_0);
    _SWW(NR_PQ_06,  2, SNR_MAX_RTO_LVL_1);
    _SWW(NR_PQ_06,  4, SNR_MAX_RTO_LVL_2);
    _SWW(NR_PQ_06,  8, SNR_MAX_RTO_LVL_3);
    _SWW(NR_PQ_06, 12, SNR_MAX_RTO_LVL_4);
    _SWW(NR_PQ_0B, 16, SNR_MAX_RTO_LVL_5);
    _SWW(NR_PQ_0B, 20, SNR_MAX_RTO_LVL_6);
    _SWW(NR_PQ_0B, 24, SNR_MAX_RTO_LVL_7);
    _SWW(NR_PQ_0B, 28, SNR_MAX_RTO_LVL_8);
    _SWW(NR_PQ_0B, 32, SNR_MAX_RTO_LVL_9);
	#endif 
    // Remapping
    _SWW(NR_PQ_12,  8, NR_NL_THD_MIN);
    _SWW(NR_PQ_12, 32, NR_NL_THD_MAX);
    _SWW(NR_PQ_12,  8, SNR_NL_THD_MIN);
    _SWW(NR_PQ_12, 16, SNR_NL_THD_MAX);

    _SWW(NR_NM_05, 2300, SNR_THM_GAIN_A);
    _SWW(NR_NM_05, 75743, SNR_THM_GAIN_B);
    _SWW(NR_NM_06, (-908984), SNR_THM_OFST_C);
    _SWW(NR_NM_06, 1, NM_SC_NL_MODE);
        
    _SWW(NR_NM_07, 1, SNR_THM_SEL);
    _SWW(NR_NM_07, 5, NM_SC_HIST_SKIP);
    _SWW(NR_NM_07, 0x3, NM_SC_HIST_SKIP_MO);
    _SWW(NR_NM_07, 0x0, NM_SC_MO_EN);
    _SWW(NR_NM_07, 0x1, NM_SC_MO_MTHD2_EN);
    _SWW(NR_NM_08, 0xA, NM_SC_MO_TH);
    _SWW(NR_NM_07, 0, NR_NM_LPF_TVD);

    //_SWW(NR_NM_0C, 0x3E00, CLEAN_DF_LVL_PDSUM_TH);

    _SWW(NR_NM_0E, 1200000, NM_SCNCHG_DET_TH_MAX);
    _SWW(NR_NM_0E, 10, NM_SCNCHG_DET_TH_GAIN);
    _SWW(NR_NM_0C, 10000, NM_SCNCHG_DET_TH_MIN);

    _SWW(NR_NM_0F, 0x10, NM_ANTI_NL_GAIN_THD_LO);
    _SWW(NR_NM_0F, 0x32, NM_ANTI_NL_GAIN_THD_HI);
    _SWW(NR_NM_0F, 0x3F, NM_ANTI_NL_GAIN_MAX);
    _SWW(NR_NM_0F, 0x0, NM_ANTI_NL_GAIN_EN);
#if 0
    _SWW(NR_BK_METER_00, 7, NR_BK_GOOD_THD);
    _SWW(NR_BK_METER_00, 8, NR_BK_LOCAL_MIN_RTO);
    _SWW(NR_BK_METER_00, 5, NR_BK_LOCAL_MIN_THD);
    _SWW(NR_BK_METER_00, 20, NR_BK_GOOD_AVG_MIN_DEN);
    _SWW(NR_BK_METER_00, 235, NR_BK_FNL_IIR);
#endif
    _SWW(NR_NM_03,   0, UPDATE_NL_SC);
    _SWW(NR_NM_0F,   0, NM_ADJ_BY_FATTAIL);
    _SWW(NR_NM_10,   1, NM_ADJ_BY_MOFBK);
    _SWW(NR_NM_10,  16, NM_ADJ_BY_MOFBK_OFST);
    _SWW(NR_NM_10,  0x20, NM_FWDREW_CNT_TH);
    _SWW(NR_NM_10,  0x10, NM_FWDREW_WAIT_CNT);
    _SWW(NR_NM_14,  0x10, NM_FWDREW_STOP_DLY);
    _SWW(NR_NM_14,  0x1, NM_NL_FWRW_UPDATE);
    _SWW(NR_NM_14,  0x1, NM_FWDREW_DET_2);
    _SWW(NR_NM_15,  0x10, NM_FRAME_STILL_CNT_MIN);
    _SWW(NR_NM_15,  0x80, NM_FRAME_STILL_CNT_MAX);
    _SWW(NR_NM_15,  0x40, NM_TVD_STABLE_CNT_TH);
    _SWW(NR_MO_02,  0x00, MONR_MOTION_FROM_MV);
    _SWW(NR_MO_03,  0x02, MONR_MOTION_MV_WEI);

    //MONR APL
    _SWW(NR_MO_03,  0x10, MONR_APL_TH1);
    _SWW(NR_MO_03,  0x40, MONR_APL_TH2);
    _SWW(NR_MO_03,  0x02, MONR_APL_GAIN);
    //MONR High freq
    _SWW(NR_MO_05,  0x00, MONR_HIGH_FREQ_EN);
    _SWW(NR_MO_04, 0x04, MONR_HIGH_FREQ_GAIN);
    _SWW(NR_MO_05,  0x9, MONR_HIGH_FREQ_TH1);
    _SWW(NR_MO_05,  0x14, MONR_HIGH_FREQ_TH2);
    
    _SWW(NR_MO_05,  1, MONR_SUB_WEI_EN);
    
    _SWW(NR_NM_0F,   1, NM_STABLIZER_IIR_EN);
    _SWW(NR_NM_10, 0xA0, NM_STABLIZER_IIR);
    _SWW(NR_CTRL_01, 1, EN_FORCE_MA_SC);
        
    _SWW(NR_CTRL_00, 1, EN_SLOW_ERFB);
    _SWW(NR_NM_0D, 4, NM_ERFB_INC);
    _SWW(NR_NM_0D, -4, NM_ERFB_DEC);
    _SWW(NR_NM_0E, 2, NM_ERFB_INC_OFST);
    _SWW(NR_NM_03, 1, ERFB_MODE);

    _SWW(NR_NM_0F, 8, NM_TDF_STA_IIR);
    
    _SWW(NR_NM_11, 1, NM_ADJ_MO_GAIN_EN);
    _SWW(NR_NM_11, 64, NM_ADJ_MO_GAIN_APL);
    _SWW(NR_NM_11, 8, NM_ADJ_MO_GAIN);
    _SWW(NR_NM_11, 6, NM_FM_MO_SHIFT1);
    _SWW(NR_NM_11, 2, NM_FM_MO_SHIFT2);
	_SWW(NR_NM_11, 6, NM_HOMO_MO_SHIFT);
	_SWW(NR_NM_12, 0x01, NM_HOMO_MO_TH1);
	_SWW(NR_NM_12, 0x08, NM_HOMO_MO_TH2);
	_SWW(NR_NM_12, 0x23, NM_HOMO_MO_TH3);
	_SWW(NR_NM_12, 0x08, NM_HOMO_MO_POS);
    
/*
    _SWW(NR_NM_03, 0, UPDATE_NL_SC);
    _SWW(NR_CTRL_00, 0, EN_SLOW_ERFB);
    _SWW(NR_NM_0D, 8, NM_ERFB_INC);
    _SWW(NR_NM_0D, -8, NM_ERFB_DEC);
    _SWW(NR_NM_0E, 1, NM_ERFB_INC_OFST);
*/
    //_SWW(NR_PQ_13, 1, RNR_TH_ADAP);
    //_SWW(NR_PQ_13, 1, BNR_RNR_ADAP);
    _SWW(NR_PQ_13, 0x28/*0x3E*/, BNR_RNR_TH);
    _SWW(NR_PQ_13, 0x3F/*28*/, RNR_MAX_TH);

    _SWW(NR_PQ_14, 20, SNR_NL_GAIN_MIN);
    _SWW(NR_PQ_14, 32, SNR_NL_GAIN_MAX);

    _SWW(NR_FWBNR_00, 1, NR_FWBNR_LOC_WRITE);
    _SWW(NR_FWBNR_00, 1, NR_FWBNR_LSUM_READ);
    _SWW(NR_FWBNR_00, 1, NR_FWBNR_VDET_EN);
    _SWW(NR_FWBNR_00, 1, NR_FWBNR_HDET_EN);

    _SWW(NR_MNR_00, 0x7D, NR_MNR_LO_TH);
    _SWW(NR_MNR_00, 0xC8, NR_MNR_HI_TH);
    _SWW(NR_MNR_00, 0x04, NR_MNR_LO_GAIN);    
    _SWW(NR_MNR_00, 0x08, NR_MNR_HI_GAIN);
    _SWW(NR_MNR_00, 0x02, NR_MNR_UPDATE_SPEED);
    //_SWW(NR_MNR_00, 0x01, NR_MNR_AUTO_EN);        //disable auto MNR

#ifndef BNR_NEW_FW
    _SWW(NR_FWBNR_05, 2, NR_FWBNR_INIT_RTO_NUM);
    _SWW(NR_FWBNR_05, 1, NR_FWBNR_INIT_RTO_DEN);
#else
    _SWW(NR_FWBNR_01,  8, NR_FWBNR_MIN_H_BK_SIZE);
    _SWW(NR_FWBNR_01,  8, NR_FWBNR_MIN_V_BK_SIZE);
    _SWW(NR_FWBNR_01, 24, NR_FWBNR_MAX_H_BK_SIZE);
    _SWW(NR_FWBNR_01, 24, NR_FWBNR_MAX_V_BK_SIZE);
    _SWW(NR_FWBNR_02, 12, NR_FWBNR_HBLK_PS0);
    _SWW(NR_FWBNR_02, 12, NR_FWBNR_VBLK_PS0);
    _SWW(NR_FWBNR_02,  4, NR_FWBNR_HBLK_PS1);
    _SWW(NR_FWBNR_02,  4, NR_FWBNR_VBLK_PS1);
    _SWW(NR_FWBNR_02, 12, NR_FWBNR_HBLK_IS0);
    _SWW(NR_FWBNR_02, 12, NR_FWBNR_VBLK_IS0);
    _SWW(NR_FWBNR_02,  4, NR_FWBNR_HBLK_IS1);
    _SWW(NR_FWBNR_02,  4, NR_FWBNR_VBLK_IS1);
    _SWW(NR_FWBNR_03, 12, NR_FWBNR_HBLK_AS0);
    _SWW(NR_FWBNR_03, 12, NR_FWBNR_VBLK_AS0);
    _SWW(NR_FWBNR_03,  4, NR_FWBNR_HBLK_AS1);
    _SWW(NR_FWBNR_03,  4, NR_FWBNR_VBLK_AS1);
    _SWW(NR_FWBNR_03,  8, NR_FWBNR_HBLK_PMATCH_TH);
    _SWW(NR_FWBNR_03,  8, NR_FWBNR_VBLK_PMATCH_TH);
    _SWW(NR_FWBNR_03,  4, NR_FWBNR_HBLK_PDCT_PCD_TH);
    _SWW(NR_FWBNR_03,  4, NR_FWBNR_VBLK_PDCT_PCD_TH);
    _SWW(NR_FWBNR_04,  8, NR_FWBNR_HBLK_PDCT_VLD_TH);
    _SWW(NR_FWBNR_04,  8, NR_FWBNR_VBLK_PDCT_VLD_TH);
    
    _SWW(NR_FWBNR_04, 12, NR_FWBNR_HBLK_LVL_ACT_TH); 
    _SWW(NR_FWBNR_04,  4, NR_FWBNR_HBLK_LVL_INC); 
    _SWW(NR_FWBNR_04,  2, NR_FWBNR_HBLK_LVL_DEC); 
    _SWW(NR_FWBNR_01,  8, NR_FWBNR_HBLK_IO_RTO); 
    _SWW(NR_FWBNR_05, 10, NR_FWBNR_HBLK_ACT_TRIAL_TH);

    _SWW(NR_FWBNR_04, 12, NR_FWBNR_VBLK_LVL_ACT_TH); 
    _SWW(NR_FWBNR_04,  4, NR_FWBNR_VBLK_LVL_INC); 
    _SWW(NR_FWBNR_04,  2, NR_FWBNR_VBLK_LVL_DEC); 
    _SWW(NR_FWBNR_01,  8, NR_FWBNR_VBLK_IO_RTO); 
    _SWW(NR_FWBNR_05, 10, NR_FWBNR_VBLK_ACT_TRIAL_TH);

    _SWW(NR_FWBNR_06,  9, NR_FWBNR_TXPXL_CNT_TH_SZ_0);
    _SWW(NR_FWBNR_06, 11, NR_FWBNR_TXPXL_CNT_TH_SZ_1);
    _SWW(NR_FWBNR_06, 13, NR_FWBNR_TXPXL_CNT_TH_SZ_2);
    _SWW(NR_FWBNR_06, 15, NR_FWBNR_TXPXL_CNT_TH_SZ_3);

    _SWW(NR_FWBNR_07,  64, NR_FWBNR_TXPXL_CNT_TH_VAL_0);
    _SWW(NR_FWBNR_07, 128, NR_FWBNR_TXPXL_CNT_TH_VAL_1);
    _SWW(NR_FWBNR_07, 128, NR_FWBNR_TXPXL_CNT_TH_VAL_2);
    _SWW(NR_FWBNR_07, 192, NR_FWBNR_TXPXL_CNT_TH_VAL_3);
#endif   

    _SWW(NR_PQ_19, 0x29, RNR_NL_THD_MAX);
    _SWW(NR_PQ_19, 0x11, RNR_NL_THD_MIN);
    _SWW(NR_PQ_19, 0x38, RNR_NL_AGG_THD_MAX);
    _SWW(NR_PQ_19, 0x18, RNR_NL_AGG_THD_MIN);
    
    _SWW(NR_MO_00,    0x02, MONR_MOTION_LEVEL_TH1);
    _SWW(NR_MO_00,    0x20, MONR_MOTION_LEVEL_TH2);
    _SWW(NR_MO_01,    0xD8, MONR_IIR_FACTOR);
    _SWW(NR_MO_02,    0xFB, MONR_IIR_FACTOR_DEC);
    
    _SWW(NR_AUTO_02, 0x14, MOTNR_MAX_NL_TH);
    _SWW(NR_AUTO_02, 0x08, MOTNR_MIN_NL_TH);
    _SWW(NR_AUTO_00, 0x10, TNR_THR_GAIN_ST);
    _SWW(NR_AUTO_00, 0x8, TNR_THR_GAIN_MO);
    _SWW(NR_MO_04, 0x14, MOSNR_MAX_NL_TH);
    _SWW(NR_MO_04, 0x08, MOSNR_MIN_NL_TH);

    _SWW(NR_AUTO_00, 0x10, SNR_ADP_MAX);
    _SWW(NR_AUTO_00, 0x04, SNR_ADP_MIN);
    _SWW(NR_AUTO_04, 0x20, BNR_MAX_GAIN);
    _SWW(NR_AUTO_04, 0x10, BNR_STILL_GAIN);
    _SWW(NR_MO_00,    0x04, MOMNR_EDGE_TH_MO);
    _SWW(NR_MO_00,    0x0F, MOMNR_EDGE_TH_ST);
    _SWW(NR_MO_01,    0x20, MONR_CORING_MO);
    _SWW(NR_MO_01,    0x02, MONR_CORING_ST);

    _SWW(NR_AUTO_02,1, SNR_GAIN_PATCH_EN);
    //_SWW(NR_AUTO_02,1, BNR_AGAINST_PATCH_EN);
#ifndef BNR_NEW_FW
    _SWW(NR_MO_04, 0xF,MOBNR_VAGAINST_ST);
    _SWW(NR_MO_04, 0x70,MOBNR_VLEAKAGE_ST);
    _SWW(NR_MO_05, 0x20,MOBNR_VLINESUMTH_ST);
#endif    
    vDrvNRSet3DNRAutoStrength(0);
    NR_FW_NM_Init();
    NR_FW_ANR_Init();
    BNR_FW_Initialize(1920, 1080);
        
    _u1Init = 1;
}

void NR_FW_SetState(E_NR_FW_STATE eState)
{
    _rNmSta.eNrFwState = eState;
}

void vDrvNRLRSwap(void)
{
    if (_SWR(NR_FWBNR_00, NR_FWBNR_LR_FREERUN) == 0)
    {
        _u1LRFreeRunCnt = _SWR(NR_FWBNR_00, NR_FWBNR_READ_RIGHT);
    }
    else
    {
        _u1LRFreeRunCnt = !_u1LRFreeRunCnt;   
    }
}

void vDrvNRISR(void)
{
    UINT32 u4Start;
    UINT32 u4End;
    UINT32 u4SignalState;
    static UINT8 _u1TVDCnt =0;

    if (!_u1Init)
        return;

    u4Start = _u4GetCurrLineCnt();
    _u1NrProcFlg = 0;

    _vDrvMCNRCleanDiffLvl();
    if (bGetVideoDecType(SV_VP_MAIN) == SV_VD_TVD3D)
    {
        if (_rTvd3dStatus.bSigStatus ==SV_VDO_STABLE)
        {
            _u1TVDCnt = (_u1TVDCnt ==255) ? 255 : (_u1TVDCnt+1);
        }
        else
        {
            _u1TVDCnt =0;
            //_u1Is_TVDNM_Stable =0;
        }

        if ((_u1TVDCnt > _SWR(NR_NM_15, NM_TVD_STABLE_CNT_TH)) /*|| (_u1TVDCnt ==0)*/)
        {
            u4SignalState =  _rTvd3dStatus.bSigStatus;
            _u1Is_TVDNM_Stable =1;
        } 
        else
        {
            u4SignalState =0;
            _u1Is_TVDNM_Stable =0;
        }
        
    }
    else
    {
        u4SignalState = bDrvVideoSignalStatus(SV_VP_MAIN);
        _u1TVDCnt =0;
        _u1Is_TVDNM_Stable=0;
    }
    //if ((_rNmSta.eNrFwState == NR_FW_VSYNC) &&(bDrvVideoSignalStatus(SV_VP_MAIN) == (UINT8)SV_VDO_STABLE))
    if ((_rNmSta.eNrFwState == NR_FW_VSYNC) &&(u4SignalState == (UINT8)SV_VDO_STABLE))
    {
        NR_FW_ANR_WriteReg();
        _vDrvMCNRUpdateOSDInk();

        _vDrvMCNRReadHWStatus();
        _vScnChgForceMA();
        _vPatch8Bit();        
        _vDRVR2CIntervalSet();

        if (_rNmSta.eNrFwState != NR_FW_STOP)
            _rNmSta.eNrFwState = NR_FW_ACTIVE;
    }
	
    u4End = _u4GetCurrLineCnt();
    _SWWM(NR_NMSTA_19, P_Fld(_rTvd3dStatus.bSigStatus, STA_NM_FW_STATE)|P_Fld(_u1Is_TVDNM_Stable, STA_NM_TVD_STATE));
    _SWWM(NR_NMSTA_12, P_Fld(u4Start, NR_VSYNC_PROC_START)|P_Fld(u4End, NR_VSYNC_PROC_END));
}

void vDrvNRVBlankProc(void)
{
    UINT32 u4StartLine = 0;
    UINT32 u4CurrLine = 0;
    UINT32 u4VBlkOK = 1;
    UINT32 u4WriteOK = 0;
    UINT32 u4ReadFail = 0;
    UINT32 u4VSkipLine = 8*NR_R(NXNR_00, NX_DEBLOCK_V_MASK);

    if (!_u1Init)
        return;

    u4StartLine = _u4GetCurrLineCnt();

    _u1LRFreeRunCnt = 0; //!_u1LRFreeRunCnt;
    
#if 0
    if( (u4StartLine < NR_R(MCVP_KC_0B, OUT_ADJ_INT_LINE)) && (u4StartLine > u4VSkipLine) ||
        u4StartLine+5 > 1125 )
    {	   
        _SWWM(NR_BLK_STA_03,
            P_Fld(1, NR_BLK_READ_FAIL)| P_Fld(1, NR_BLK_READ_VHOLD)|
            P_Fld(u4StartLine, NR_BLK_READ_END)| P_Fld(u4StartLine, NR_BLK_READ_START));

        return;
    }
#endif
    //if (!_EN(EN_READ_NXNR_SRAM))
    {
        BNR_FW_VSyncProcess();

        #if 0
        u4VBlkOK = u1DrvNXNRReadBlkLineSum();

        if (_EN(EN_FW_BLK_POS))
        {
            u4WriteOK = u1DrvNXNRWriteBlkPos();
        }
        #endif
    }

    u4CurrLine = _u4GetCurrLineCnt();

    //u4ReadFail = (u4CurrLine < u4StartLine) || (u4VBlkOK == 0) || (!u4HBlkOK) ||
    //    (MIN(u4StartLine, u4CurrLine) < NR_R(MCVP_KC_0B, OUT_ADJ_INT_LINE));	
    u4ReadFail = !((u4CurrLine >= u4StartLine) || (u4CurrLine <= u4VSkipLine))
        || (u4VBlkOK == 0);
	
    _SWWM(NR_BLK_STA_03,
        P_Fld(u4ReadFail, NR_BLK_READ_FAIL)|P_Fld((u4VBlkOK == 2), NR_BLK_READ_VHOLD)|P_Fld(u4WriteOK, NR_BLK_POS_WRITE_OK)|
        P_Fld(u4CurrLine, NR_BLK_READ_END)|P_Fld(u4StartLine, NR_BLK_READ_START));

    vDrvNRSemaUnlock();
}

void vDrvNRNoiseLevelProc(void)
{
    UINT32 u4Start;
    UINT32 u4End;
    UINT32 u4Dirty;

    NR_NM_ANA_T rNmAna;

    if (!_u1Init)
        return;

    _u1NrProcFlg = 1;
    u4Start = _u4GetCurrLineCnt();

    //_u1LRFreeRunCnt = !_u1LRFreeRunCnt;

    if (_rNmSta.eNrFwState == NR_FW_ACTIVE /* && _u1LRFreeRunCnt*/)
    {
        _vNLScnChgCnt();

        //if (_rNmSta.u1ScnChg)
        //    BNR_FW_SetWindowMode(4);
        _u4EnMsk = _SWR_4B(NR_CTRL_00);
        
        NR_FW_NM_Process(&_rNmSta, &rNmAna);
        NR_FW_ANR_Process(&_rNmSta, &rNmAna);

        //_vDrvNXNRCalcBlkHist();
        //_vDrvNXNRCalcFinalStrength();
        if (_EN(EN_COPY_LINE_SUM) || _EN(EN_READ_NXNR_SRAM))
        {
            BNR_FW_Stop();
            if (_EN(EN_COPY_LINE_SUM))
                BNR_FW_Dbg();
        }
        else
        {
            BNR_FW_Resume();
        }

        if (_EN(EN_TEST_AUTO_BLK))
            _vDrvNXNRDeterminePos();

        if (_rNmSta.eNrFwState != NR_FW_STOP)
            _rNmSta.eNrFwState = NR_FW_VSYNC;
    }

    u4End = _u4GetCurrLineCnt();

    u4Dirty = (u4Start <= _SWR(NR_NMSTA_12, NR_VSYNC_PROC_END) || _u1NrProcFlg == 0);
    _u1NrProcFlg = 0;

    _SWWM(NR_NMSTA_11, P_Fld(u4Dirty, NR_PROC_DIRTY)|P_Fld(u4Start, NR_PROC_START)|P_Fld(u4End, NR_PROC_END));
}
//patch for NR R2C interval algorithm, it will form diagonal line under raster pattern.
void vDrvR2CIntervalPatch(UINT16 u2MIBWidth, UINT16 u2MIBHight)
{
    UINT8 i, RIV, Remainder;
    UINT32 u4MIBTotalPixel;    
    
    u4MIBTotalPixel = u2MIBWidth * u2MIBHight;
    for(RIV = 5; RIV <= 15; RIV++)		   //  search range from 5 to 15.
    {
    	if(u2MIBWidth%RIV == 0)
    		continue;

		Remainder = u4MIBTotalPixel%RIV;
    	for(i=1; i<RIV; i++)
    	{
    		if((Remainder*i)%RIV == i)
    			break;
    	}

    	if(i==RIV)
    	{
    		bR2CInterval = RIV;
    		bR2RSTFlag = SV_TRUE;
    		break; 
    	}
       
    } 
    if(RIV > 15)
    {
    	bR2CInterval = 7;
    	bR2RSTFlag = SV_TRUE;
    }
}
void _vDRVR2CIntervalSet(void)
{	

   if (!_u1Init)
        return;
	
	if(bR2RSTFlag == SV_TRUE)
	{		
		NR_W(MCNR_00, bR2CInterval, MCNR_R2C_INTERVAL);
		bR2RSTFlag = SV_FALSE;
	}
}
// --- end patch


