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
 * $RCSfile: drv_scaler_3d.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file drv_scaler.c
 *  Brief of file drv_scaler.c.
 *  Details of file drv_scaler.c (optional).
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "panel.h"
#include "drv_scaler.h"
#include "drv_scaler_drvif.h"
#include "vdo_misc.h"
#include "drv_tdtv_drvif.h"
#include "scpos_debug.h"
#include "hw_di.h"
#include "mute_if.h"
#ifdef CC_MT5399
#include "drv_mjc.h"
#endif

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
typedef enum
{
    E_HW_SC_MODE_2D     =0x0,
    E_HW_SC_MODE_FS     =0x2,
    E_HW_SC_MODE_TB     =0x4,
    E_HW_SC_MODE_SBS    =0x8,
    E_HW_SC_MODE_LI     =0xC
}E_HW_SC_3D_MODE;

//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------
#ifdef CC_ENBALE_SCPIP_3D
static void _vScpipUpdate3DPROutPrm(UINT32 u4VdpId, SCALER_TDTV_IN_TYPE in, RPicInfo* picInfo, RDispInfo* dispInfo, RDispPrm* dispPrm);
static void _vScpipUpdate3DFPROutPrm(UINT32 u4VdpId, SCALER_TDTV_IN_TYPE in, RPicInfo* picInfo, RDispInfo* dispInfo, RDispPrm* dispPrm);
static void _vScpipUpdate3DSBSOutPrm(UINT32 u4VdpId, SCALER_TDTV_IN_TYPE in, RPicInfo* picInfo, RDispInfo* dispInfo, RDispPrm* dispPrm);
static void _vScpipUpdate3DSGOutPrm(UINT32 u4VdpId, SCALER_TDTV_IN_TYPE in, RPicInfo* picInfo, RDispInfo* dispInfo, RDispPrm* dispPrm);
static void _vScpipUpdate3DTo2DOutPrm(UINT32 u4VdpId, SCALER_TDTV_IN_TYPE in, RPicInfo* picInfo, RDispInfo* dispInfo, RDispPrm* dispPrm);
#endif
//for set 3D-related register
static void _vScpipSet3DIOConvertReg(UINT32 u4VdpId);

//-----------------------------------------------------------------------------
// Global functions
//-----------------------------------------------------------------------------
void vScpipSetTdtvIO(UINT32 u4VdpId, SCALER_TDTV_IN_TYPE in, SCALER_TDTV_OUT_TYPE out)
{
    if (u4VdpId >= VDP_NS)
    {
        return;
    }
    
    vScpipSetTdtvInType(u4VdpId, in);
    vScpipSetTdtvOutType(u4VdpId, out);
    vScpipReconfig(u4VdpId);
}

void vScpipSetTdtvInType(UINT32 u4VdpId, SCALER_TDTV_IN_TYPE in)
{
    SCALER_TDTV_PRM_T* tdtvInfo;

    tdtvInfo = getTdtvInfo(u4VdpId);
    tdtvInfo->eTdtvInType = in;
}

void vScpipSetTdtvOutType(UINT32 u4VdpId, SCALER_TDTV_OUT_TYPE out)
{
    SCALER_TDTV_PRM_T* tdtvInfo;

    tdtvInfo = getTdtvInfo(u4VdpId);    
    tdtvInfo->eTdtvOutType = out;
}

SCALER_TDTV_IN_TYPE eScpipGetTdtvInType(UINT32 u4VdpId)
{
    SCALER_TDTV_PRM_T* tdtvInfo;
    
    tdtvInfo = getTdtvInfo(u4VdpId);
    return tdtvInfo->eTdtvInType;
}

SCALER_TDTV_OUT_TYPE eScpipGetTdtvOutType(UINT32 u4VdpId)
{
    SCALER_TDTV_PRM_T* tdtvInfo;
    
    tdtvInfo = getTdtvInfo(u4VdpId);
    return tdtvInfo->eTdtvOutType;
}

void vScpipSetTdtvResolutionType(UINT32 u4VdpId, SCALER_TDTV_RES_POSITION pos, E_TDTV_RES_OUTPUT type)
{
    SCALER_TDTV_PRM_T* tdtvInfo;
    E_TDTV_RES_OUTPUT preResType;

    if (u4VdpId >= VDP_NS)
    {
        return;
    }

    tdtvInfo = getTdtvInfo(u4VdpId);
    preResType = eScpipGetTdtvResolutionType(u4VdpId, pos);    
        
    LOG(1, "Set scaler resolution prm for 3D VDP(%d) position(%d) type(%d) previous type(%d)\n", u4VdpId,pos,type,preResType);
    
    if(preResType == type)
    {
        return;
    }
    
    switch(pos)
    {
        case SCALER_TDTV_RES_POSITION_PRE_H:
                tdtvInfo->ePreH = type;
            break;
        case SCALER_TDTV_RES_POSITION_PRE_V:
                tdtvInfo->ePreV = type;
            break;
        case SCALER_TDTV_RES_POSITION_PST_H:
                tdtvInfo->ePstH = type;
            break;
        case SCALER_TDTV_RES_POSITION_PST_V: 
                tdtvInfo->ePstV = type;
            break;            
        default:
            break;
    };

    vScpipFire3DResChange(u4VdpId);
}

E_TDTV_RES_OUTPUT eScpipGetTdtvResolutionType(UINT32 u4VdpId, SCALER_TDTV_RES_POSITION pos)
{
    E_TDTV_RES_OUTPUT ret = E_TD_RES_NORMAL;
    SCALER_TDTV_PRM_T* tdtvInfo;
    
    tdtvInfo = getTdtvInfo(u4VdpId);

    switch(pos)
    {
        case SCALER_TDTV_RES_POSITION_PRE_H:
            ret = tdtvInfo->ePreH;
            break;
        case SCALER_TDTV_RES_POSITION_PRE_V:
            ret = tdtvInfo->ePreV;
            break;
        case SCALER_TDTV_RES_POSITION_PST_H:
            ret = tdtvInfo->ePstH;
            break;
        case SCALER_TDTV_RES_POSITION_PST_V: 
            ret = tdtvInfo->ePstV;
            break;            
        default:
            break;
    };  

    return ret;
}

void vScpipUpdateTdtvInfo(UINT32 u4VdpId, UINT32 u4TdtvPrm)
{
    
    E_TD_IN tdtvIn;
    E_TD_OUT tdtvOut;
    SCALER_TDTV_IN_TYPE sclIn;
    SCALER_TDTV_OUT_TYPE sclOut;
    UINT8 bUpdate = SV_FALSE;

    if (u4VdpId >= VDP_NS)
    {
        return;
    }
    
    //PRE_H
    if(eScpipGetTdtvResolutionType(u4VdpId, SCALER_TDTV_RES_POSITION_PRE_H) != TD_SCL_PRE_H(u4TdtvPrm))
    {
        vScpipSetTdtvResolutionType(u4VdpId, SCALER_TDTV_RES_POSITION_PRE_H, TD_SCL_PRE_H(u4TdtvPrm));
        bUpdate = SV_TRUE;
    }
    //PRE_V    
    if(eScpipGetTdtvResolutionType(u4VdpId, SCALER_TDTV_RES_POSITION_PRE_V) != TD_SCL_PRE_V(u4TdtvPrm))
    {
        vScpipSetTdtvResolutionType(u4VdpId, SCALER_TDTV_RES_POSITION_PRE_V, TD_SCL_PRE_V(u4TdtvPrm));
        bUpdate = SV_TRUE;
    }
    //PST_H
    if(eScpipGetTdtvResolutionType(u4VdpId, SCALER_TDTV_RES_POSITION_PST_H) != TD_SCL_PST_H(u4TdtvPrm))
    {
        vScpipSetTdtvResolutionType(u4VdpId, SCALER_TDTV_RES_POSITION_PST_H, TD_SCL_PST_H(u4TdtvPrm));
        bUpdate = SV_TRUE;        
    }
    //PST_V  
    if(eScpipGetTdtvResolutionType(u4VdpId, SCALER_TDTV_RES_POSITION_PST_V) != TD_SCL_PST_V(u4TdtvPrm))
    {
        vScpipSetTdtvResolutionType(u4VdpId, SCALER_TDTV_RES_POSITION_PST_V, TD_SCL_PST_V(u4TdtvPrm));
        bUpdate = SV_TRUE;
    }
    
    tdtvIn = TD_SCL_IN(u4TdtvPrm);
    tdtvOut = TD_SCL_OUT(u4TdtvPrm);    

    switch(tdtvIn)
    {
        case E_TD_IN_2D_I:   
        case E_TD_IN_2D_P:
            sclIn = SCALER_TDTV_IN_TYPE_2D;
            break;
        case E_TD_IN_FS_I:   
        case E_TD_IN_FS_P:   
        case E_TD_IN_FP_I:   
        case E_TD_IN_FP_P:
            sclIn = SCALER_TDTV_IN_TYPE_FS;
            break;
        case E_TD_IN_TB_I:
        case E_TD_IN_TB_P: 
            sclIn = SCALER_TDTV_IN_TYPE_TAB;
            break;
        case E_TD_IN_SBS_I:  
        case E_TD_IN_SBS_P:
            sclIn = SCALER_TDTV_IN_TYPE_SBS;
            break;
        case E_TD_IN_LI_I:   
        case E_TD_IN_LI_P:
            sclIn = SCALER_TDTV_IN_TYPE_LI;
            break;
        default:
            sclIn = SCALER_TDTV_IN_TYPE_2D;
            break;
    };

    if(sclIn != eScpipGetTdtvInType(u4VdpId))
    {
        vScpipSetTdtvInType(u4VdpId, sclIn);
        bUpdate = SV_TRUE;        
    }

    switch(tdtvOut)
    {
        case E_TD_OUT_NATIVE:
            sclOut = SCALER_TDTV_OUT_TYPE_2D;
            break;
        case E_TD_OUT_3D_FS:
            sclOut = SCALER_TDTV_OUT_TYPE_SG;
            break;
        case E_TD_OUT_3D_LI:
            sclOut = SCALER_TDTV_OUT_TYPE_PR;
            break;            
        case E_TD_OUT_3D_SBS:
            sclOut = SCALER_TDTV_OUT_TYPE_SBS;
            break;
        case E_TD_OUT_3D_FPR:   
            sclOut = SCALER_TDTV_OUT_TYPE_FPR;
            break;
        default:
            sclOut = SCALER_TDTV_OUT_TYPE_2D;
            break;
    };

    if(sclOut != eScpipGetTdtvOutType(u4VdpId))
    {
        vScpipSetTdtvOutType(u4VdpId,sclOut);
        bUpdate = SV_TRUE;        
    }

    if(bUpdate == SV_TRUE)
    {
        _vDrvVideoSetMute(MUTE_MODULE_SCPOS_TV3D, u4VdpId, VDP_SCPOS_TV3D_MUTE_DELAY, TRUE);
    }
    
    vScpipFire3DResChange(u4VdpId);    
}

void vScpipFire3DResChange(UINT32 u4VdpId)
{
    if (u4VdpId == VDP_1)
    {
        vSetScposFlg(MAIN_3D_RES_CHG);
    }
    else if (u4VdpId == VDP_2)
    {
        vSetScposFlg(PIP_3D_RES_CHG);
    }    
}
#ifdef CC_ENBALE_SCPIP_3D

void vScpipUpdateSrm3DResolution(UINT32 u4VdpId)
{
    UINT8 bPath;
    RPicInfo* picInfo;
    RDispInfo* dispInfo;
    E_TDTV_RES_OUTPUT preHType;
    E_TDTV_RES_OUTPUT preVType;
    E_TDTV_RES_OUTPUT pstVType;

    if (u4VdpId >= VDP_NS)
    {
        return;
    }

    bPath = getScalerPath(u4VdpId);
    picInfo = getPicInfo(bPath);
    dispInfo = getDispInfo(bPath);
    
    //PRE H
    preHType = eScpipGetTdtvResolutionType(u4VdpId, SCALER_TDTV_RES_POSITION_PRE_H);
    if(preHType == E_TD_RES_HALF)
    {
        picInfo->wHTotal = picInfo->wHTotal>>1;
        picInfo->wSrcW = picInfo->wSrcW>>1;
        picInfo->wTargetW = picInfo->wTargetW>>1;
        picInfo->wHPorch = picInfo->wHPorch>>1;
        picInfo->wXOffset= picInfo->wXOffset>>1;
    }
    else if(preHType == E_TD_RES_DOUBLE)
    {
        picInfo->wHTotal = picInfo->wHTotal<<1;
        picInfo->wSrcW = picInfo->wSrcW<<1;
        picInfo->wTargetW = picInfo->wTargetW<<1;
        picInfo->wHPorch = picInfo->wHPorch<<1;    
        picInfo->wXOffset= picInfo->wXOffset<<1;
    }

    //PRE V
    preVType = eScpipGetTdtvResolutionType(u4VdpId, SCALER_TDTV_RES_POSITION_PRE_V);
    if(preVType == E_TD_RES_HALF)
    {
        picInfo->wVTotal = picInfo->wVTotal>>1;
        picInfo->wSrcH = picInfo->wSrcH>>1;
        picInfo->wTargetH = picInfo->wTargetH>>1;
        picInfo->wVPorch = picInfo->wVPorch>>1;
        picInfo->wYOffset= picInfo->wYOffset>>1;        
    }
    else if(preVType == E_TD_RES_DOUBLE)
    {
        picInfo->wVTotal = picInfo->wVTotal<<1;
        picInfo->wSrcH = picInfo->wSrcH<<1;
        picInfo->wTargetH = picInfo->wTargetH<<1;
        picInfo->wVPorch = picInfo->wVPorch<<1;    
        picInfo->wYOffset = picInfo->wYOffset<<1;    
    }

    //PST V
    pstVType = eScpipGetTdtvResolutionType(u4VdpId, SCALER_TDTV_RES_POSITION_PST_V);
    if(pstVType == E_TD_RES_HALF)    //means MJC will make vertical to half, so scaler have to up scale to double height of panel.
    {
        dispInfo->wYOffset= dispInfo->wYOffset<<1;
        dispInfo->wHeight = dispInfo->wHeight<<1;
    }
    else if(pstVType == E_TD_RES_DOUBLE) //means MJC will make vertical double, so scaler have to down to half of panel size
    {
        dispInfo->wYOffset= dispInfo->wYOffset>>1;    
        dispInfo->wHeight = dispInfo->wHeight>>1;
    }

    vScpipReconfig(u4VdpId);
}

#endif
UINT32 u4ScpipGetTDTVPRUpFlag(UINT32 u4VdpId)
{
    SCALER_TDTV_PRM_T* tdtvInfo;
    
    tdtvInfo = getTdtvInfo(u4VdpId);
    return tdtvInfo->u4IsPRUp;
}

UINT8 vScpipIs3DDoneByScaler(UINT32 u4VdpId)
{
    UINT8 ret = SV_FALSE;    
    UINT8 bPath;
    
    bPath = getScalerPath(u4VdpId);

    if (u4VdpId >= VDP_NS)
    {
        return ret;
    }
    
    if(eScpipGetTdtvInType(u4VdpId) != SCALER_TDTV_IN_TYPE_2D && 
        eScpipGetTdtvOutType(u4VdpId) != SCALER_TDTV_OUT_TYPE_2D &&
        getScalerMode(bPath) == VDP_SCPOS_DISPMODE_OFF) //dram mode
    {
        ret = SV_TRUE;
    }

    return ret;
}

UINT8 vScpipIs3DDoneByMJC(void)
{
    UINT8 ret = SV_FALSE;    
    SCALER_TDTV_IN_TYPE in;
    SCALER_TDTV_OUT_TYPE out;
#if defined(CC_MT5399)
    //determine if current mode is TTD mode.
	UINT32 u4TTD;
    E_TD_IN eTTDIn;
    E_TD_OUT eTTDOut;
#endif    

    in = eScpipGetTdtvInType(SV_VP_MAIN);
    out = eScpipGetTdtvOutType(SV_VP_MAIN);
    
    if((in != SCALER_TDTV_IN_TYPE_2D) &&
        (out == SCALER_TDTV_OUT_TYPE_2D))
    {
        ret = SV_TRUE;
    }

    #if defined(CC_MT5399)
    //determine if current mode is TTD mode.
    u4TTD = u4DrvTDTVTTDModeQuery();
    eTTDIn  = TD_TTD_IN(u4TTD);
    eTTDOut = TD_TTD_OUT(u4TTD);    
    if ( ((eTTDIn == E_TD_IN_2D_P) && (eTTDOut == E_TD_OUT_3D_FS)) ||
         ((eTTDIn == E_TD_IN_FS_P) && ((eTTDOut == E_TD_OUT_3D_FS) || (eTTDOut == E_TD_OUT_NATIVE))))
    {
        ret = SV_TRUE;
    }
    #endif
        
    return ret;    
}

void vScpipSetDispROutInv(UINT32 bOnOff)
{
    _arScalePrm[SV_VP_MAIN].u4IsLRDataSwap = bOnOff;
    SCPIP_WRITE_FLD(SCPIP_DRAM_M_12, _arScalePrm[SV_VP_MAIN].u4FlipEnable^_arScalePrm[SV_VP_MAIN].u4IsLRDataSwap, DRAM_M_12_DA_3D_OUTPUT_LR_SWAP);
}
#ifdef CC_ENBALE_SCPIP_3D

void vScpipUpdate3DPrm(UINT32 u4VdpId)
{    
    SCALER_TDTV_IN_TYPE in;
    SCALER_TDTV_OUT_TYPE out;
    UINT8 bPath;
    RPicInfo* picInfo;
    RDispInfo* dispInfo;
    RDispPrm* dispPrm;

    if (u4VdpId >= VDP_NS)
    {
        return;
    }

    in = eScpipGetTdtvInType(u4VdpId);
    out = eScpipGetTdtvOutType(u4VdpId);
    bPath = getScalerPath(u4VdpId);
    picInfo = getPicInfo(bPath);
    dispInfo = getDispInfo(bPath);
    dispPrm = getDispPrm(bPath);

    if(picInfo->wHTotal == 0 || picInfo->wVTotal == 0 ||
       picInfo->wSrcW == 0 || picInfo->wSrcH == 0 || 
       picInfo->wTargetW == 0 || picInfo->wTargetH == 0)
    {
        return;
    }

    if(dispInfo->wWidth == 0 || dispInfo->wHeight == 0)
    {
        return;
    }
    
    switch(out)
    {
        case SCALER_TDTV_OUT_TYPE_PR:
            _vScpipUpdate3DPROutPrm(u4VdpId,in, picInfo, dispInfo, dispPrm);
            break;
        case SCALER_TDTV_OUT_TYPE_FPR:
            _vScpipUpdate3DFPROutPrm(u4VdpId,in, picInfo, dispInfo, dispPrm);            
            break;
        case SCALER_TDTV_OUT_TYPE_SBS:
            _vScpipUpdate3DSBSOutPrm(u4VdpId,in, picInfo, dispInfo, dispPrm);
            break;
        case SCALER_TDTV_OUT_TYPE_SG:
            _vScpipUpdate3DSGOutPrm(u4VdpId,in, picInfo, dispInfo, dispPrm);
            break;
        case SCALER_TDTV_OUT_TYPE_3D_TO_2D:
            _vScpipUpdate3DTo2DOutPrm(u4VdpId,in, picInfo, dispInfo, dispPrm);
            break;
        default:
            //do nothing
            break;
    }
}
#endif
void vScpipSet3DModeReg(UINT32 u4VdpId, SCALER_TDTV_IN_TYPE inType)
{
    UINT8 value;

    if(u4VdpId != VDP_1)
    {
        return;
    }

    switch(inType)
    {
        case SCALER_TDTV_IN_TYPE_2D:
            value = E_HW_SC_MODE_2D;
        break;
        case SCALER_TDTV_IN_TYPE_FS:
            value = E_HW_SC_MODE_FS;
        break;
        case SCALER_TDTV_IN_TYPE_SBS:
            value = E_HW_SC_MODE_2D;
        break;
        case SCALER_TDTV_IN_TYPE_TAB:
            //even MP will not fix TAB
            value = E_HW_SC_MODE_2D;
        break;            
        case SCALER_TDTV_IN_TYPE_LI:
            value = E_HW_SC_MODE_LI;
        break;            
        default:
            value = E_HW_SC_MODE_2D;
        break;            
    };

    if(vScpipIs3DDoneByScaler(u4VdpId) == SV_TRUE)
    {
        value = E_HW_SC_MODE_2D;
    }

    vScpipSetSC3DMode(value);
    vScpipSetDispFifoWaterLevel(u4VdpId,((value == E_HW_SC_MODE_LI) ? 0x2bc : 0x4b0));
}

void vScpipSet3DLRFreerunReg(UINT32 u4VdpId, SCALER_TDTV_IN_TYPE inType)
{
    UINT8 bPath;
    bPath = getScalerPath(u4VdpId);

    if(u4VdpId != VDP_1)
    {
        return;
    }
    
    if(inType == SCALER_TDTV_IN_TYPE_FS &&
       getScalerMode(bPath) == VDP_SCPOS_PSCAN_DISPMODE)
    {
        vScpipSetDispRFreerun(SV_ON);
        vScpipSetMibDispRMode(0);
    }
    else if(getTdtvInfo(u4VdpId)->u4IsDisprFreerun == SV_TRUE)
    {
        vScpipSetDispRFreerun(SV_ON);
        vScpipSetMibDispRMode(1);
    }
    else
    {
        vScpipSetDispRFreerun(SV_OFF);
        vScpipSetMibDispRMode(1);
    }
}

void vScpipSet3DReg(UINT32 u4VdpId)
{
    _vScpipSet3DIOConvertReg(u4VdpId);
}

void vScpipUpdateDisprFreerunPrm(UINT32 u4VdpId)
{
    SCALER_TDTV_IN_TYPE in;
    SCALER_TDTV_OUT_TYPE out;
    SCALER_TDTV_PRM_T* tdtvInfo;
    #if defined(CC_MT5880)
    UINT32 u4TTD = u4DrvTDTVTTDModeQuery();
    E_TD_IN eTTDIn  = TD_TTD_IN(u4TTD);
    E_TD_OUT eTTDOut = TD_TTD_OUT(u4TTD);
    #endif
    if (u4VdpId >= VDP_NS)
    {
        return;
    }

    in = eScpipGetTdtvInType(u4VdpId);
    out = eScpipGetTdtvOutType(u4VdpId);
    tdtvInfo = getTdtvInfo(u4VdpId);    

    if(in != SCALER_TDTV_IN_TYPE_FS && out == SCALER_TDTV_OUT_TYPE_SG)
    {
        tdtvInfo->u4IsDisprFreerun = SV_TRUE;
    }
#if defined(CC_MT5880)
	else if(IS_LVDS_DISP_3D_SHUTTER && ((eTTDIn==E_TD_IN_2D_P)&&(eTTDOut==E_TD_OUT_3D_FS)))
	{
		tdtvInfo->u4IsDisprFreerun = SV_TRUE;
	}
#endif
    else
    {
        tdtvInfo->u4IsDisprFreerun = SV_FALSE;
    }
}

static void _vScpipSet3DIOConvertReg(UINT32 u4VdpId)
{    
    UINT8 bPath; 
    UINT16 dramOffset;
    SCALER_TDTV_IN_TYPE in;
    SCALER_TDTV_OUT_TYPE out;
    
    if (u4VdpId >= VDP_NS)
    {
        return;
    }

    bPath  = getScalerPath(u4VdpId);
    dramOffset = getDramOffset(bPath);    
    in = eScpipGetTdtvInType(u4VdpId);
    out = eScpipGetTdtvOutType(u4VdpId);

    LOG(1,"Scaler 3D (in,out)=(%d,%d), Is3DDoneByScaler(%d), Is3DDoneByMJC(%d)\n", in, out, vScpipIs3DDoneByScaler(u4VdpId), vScpipIs3DDoneByMJC());
    
    if(getScalerMode(bPath) != VDP_SCPOS_DISPMODE_OFF)  //not in dram mode
    {
        vRegWriteFldAlign(SCPIP_DRAM_M_12+dramOffset, 0, DRAM_M_12_DA_3D_INPUT_EN);
    }
    else if(in == SCALER_TDTV_IN_TYPE_2D)               //input is 2D
    {
        vRegWriteFldAlign(SCPIP_DRAM_M_12+dramOffset, 0, DRAM_M_12_DA_3D_INPUT_EN);
    }
    else                                                //input is 3D & scaler is in dram mode
    {
        #if defined(SUPPORT_3D_OVSN_BY_SCL)
        if(in == SCALER_TDTV_IN_TYPE_FS && out == SCALER_TDTV_OUT_TYPE_SG)  //FS in FS out, don't have to do 3D convert.
        {
            vRegWriteFldAlign(SCPIP_DRAM_M_12+dramOffset, 0, DRAM_M_12_DA_3D_INPUT_EN);    
            vRegWriteFldAlign(SCPIP_DRAM_M_12+dramOffset, 0, DRAM_M_12_DA_3D_INPUT_MODE);            
        }
        else
        #endif
        if(vScpipIs3DDoneByScaler(u4VdpId) == SV_TRUE)
        {
            vRegWriteFldAlign(SCPIP_DRAM_M_12+dramOffset, 1, DRAM_M_12_DA_3D_INPUT_EN);    
            vRegWriteFldAlign(SCPIP_DRAM_M_12+dramOffset, ((UINT8)in)-1, DRAM_M_12_DA_3D_INPUT_MODE);            
        }
        else
        {
            vRegWriteFldAlign(SCPIP_DRAM_M_12+dramOffset, 0, DRAM_M_12_DA_3D_INPUT_EN);    
            vRegWriteFldAlign(SCPIP_DRAM_M_12+dramOffset, 0, DRAM_M_12_DA_3D_INPUT_MODE);
        }
    }

    if((u4VdpId == VDP_2)&& (u1DrvScpipGetVencModule() == E_VENC_MODULE_SUB_SCALER) 
         #ifdef CC_SUPPORT_VENC
          && (u1DrvScpipGetVencRecordMode() != E_SCALER_VENC_PROGRESSIVE) 
         #endif
         )
    {
        vRegWriteFldAlign(SCPIP_DRAM_M_12+dramOffset, 0, DRAM_M_12_DA_3D_INPUT_EN);
    }

    if(getScalerMode(bPath) != VDP_SCPOS_DISPMODE_OFF)  //not in dram mode
    {
        vRegWriteFldAlign(SCPIP_DRAM_M_12+dramOffset, 0, DRAM_M_12_DA_3D_OUTPUT_EN);        
        vRegWriteFldAlign(SCPIP_DRAM_M_12+dramOffset, 0, DRAM_M_12_DA_WRITE_3D_TO_2D_EN);
    }    
    else if(out == SCALER_TDTV_OUT_TYPE_2D)             //output is 2D
    {
        vRegWriteFldAlign(SCPIP_DRAM_M_12+dramOffset, 0, DRAM_M_12_DA_3D_OUTPUT_EN);        
        vRegWriteFldAlign(SCPIP_DRAM_M_12+dramOffset, 0, DRAM_M_12_DA_WRITE_3D_TO_2D_EN);            
    }
    else                                                //output is 3D
    {
        if(vScpipIs3DDoneByScaler(u4VdpId) == SV_TRUE)
        {
            #if defined(SUPPORT_3D_OVSN_BY_SCL)
            if(in == SCALER_TDTV_IN_TYPE_FS && out == SCALER_TDTV_OUT_TYPE_SG)
            {
                vRegWriteFldAlign(SCPIP_DRAM_M_12+dramOffset, 0, DRAM_M_12_DA_3D_OUTPUT_EN);
                vRegWriteFldAlign(SCPIP_DRAM_M_12+dramOffset, 0, DRAM_M_12_DA_WRITE_3D_TO_2D_EN);
                
            }
            else
            #endif
            if(out == SCALER_TDTV_OUT_TYPE_3D_TO_2D)    //3D to 2D
            {
                vRegWriteFldAlign(SCPIP_DRAM_M_12+dramOffset, 0, DRAM_M_12_DA_3D_OUTPUT_EN);
                vRegWriteFldAlign(SCPIP_DRAM_M_12+dramOffset, 1, DRAM_M_12_DA_WRITE_3D_TO_2D_EN);            
            }
            else    //3D convert
            {
                vRegWriteFldAlign(SCPIP_DRAM_M_12+dramOffset, 1, DRAM_M_12_DA_3D_OUTPUT_EN);    
                vRegWriteFldAlign(SCPIP_DRAM_M_12+dramOffset, 0, DRAM_M_12_DA_WRITE_3D_TO_2D_EN);
                vRegWriteFldAlign(SCPIP_DRAM_M_12+dramOffset, ((UINT8)out)-1, DRAM_M_12_DA_3D_OUTPUT_MODE);            
            }
        }
    }
}
#ifdef CC_ENBALE_SCPIP_3D

static void _vScpipUpdate3DPROutPrm(UINT32 u4VdpId, SCALER_TDTV_IN_TYPE in, RPicInfo* picInfo, RDispInfo* dispInfo, RDispPrm* dispPrm)
{

}

static void _vScpipUpdate3DFPROutPrm(UINT32 u4VdpId, SCALER_TDTV_IN_TYPE in, RPicInfo* picInfo, RDispInfo* dispInfo, RDispPrm* dispPrm)
{    
}

static void _vScpipUpdate3DSBSOutPrm(UINT32 u4VdpId, SCALER_TDTV_IN_TYPE in, RPicInfo* picInfo, RDispInfo* dispInfo, RDispPrm* dispPrm)
{
}

static void _vScpipUpdate3DSGOutPrm(UINT32 u4VdpId, SCALER_TDTV_IN_TYPE in, RPicInfo* picInfo, RDispInfo* dispInfo, RDispPrm* dispPrm)
{
}

static void _vScpipUpdate3DTo2DOutPrm(UINT32 u4VdpId, SCALER_TDTV_IN_TYPE in, RPicInfo* picInfo, RDispInfo* dispInfo, RDispPrm* dispPrm)
{
}
#endif
void vScpipUpdateScalerBobModeFlag(UINT32 u4VdpId)
{
    VDP_SCALER_PRM_T* scalerInfo;

    scalerInfo = getScalerInfo(getScalerPath(u4VdpId));

    //only main support scaler bob mode.
    if(u4VdpId != VDP_1)
    {
        scalerInfo->u4IsScalerBobMode = SV_FALSE;
        return;
    }
    
    if(bDrvDIIsScalerBob(u4VdpId) == SV_TRUE)
    {
        scalerInfo->u4IsScalerBobMode = SV_TRUE;
    }
    else
    {
        scalerInfo->u4IsScalerBobMode = SV_FALSE;
    }
}

