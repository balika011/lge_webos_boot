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
 /*************************************************************************
 *
 * Filename:
 * ---------
 *   $Workfile: drv_regen.c $
 *
 * Project:
 * --------
 *
 *
 * Description:
 * ------------
 *
 *
 * Author:
 * -------
 *
 *
 * Last changed:
 * -------------
 * $Author: dtvbm11 $
 *
 * $Date: 2015/01/09 $
 *
 * $Revision: #1 $
*************************************************************************/
#include "hw_regen.h"
#include "hw_mon_out.h"
#include "nptv_debug.h"
#include "drv_regen.h"
#include "drv_scaler_drvif.h"
#include "drv_tdtv_drvif.h"
#include "drv_tdtv.h"
#include "mute_if.h"


//for preprocess regen
REGEN_INFO_T _regenInfo[VDP_MAX_NS];
//for fsc regen
REGEN_INFO_T _fscRegenInfo[VDP_MAX_NS];

static void _vRegenUpdateInOutPrm(UINT8 bPath,REGEN_MODULE_E mod);
static void _vRegenUpdateInputResolution(UINT8 bPath,REGEN_MODULE_E mod,UINT16 inW, UINT16 inH);
static void _vRegenUpdateOutputResolution(UINT8 bPath,REGEN_MODULE_E mod, UINT16 outX, UINT16 outY, UINT16 outW, UINT16 outH);
static void _vRegenUpdateDispPrm(UINT8 bPath,REGEN_MODULE_E mod);
static void _vRegenUpdateReg(UINT8 bPath,REGEN_MODULE_E mod);

void vRegenSwInit(UINT8 bPath,REGEN_MODULE_E mod)
{

}

void vRegenHwInit(UINT8 bPath,REGEN_MODULE_E mod)
{
    //disable regen
    vRegenSetRegenEnableReg(bPath,mod,SV_OFF);
    vRegenSetClipEnableReg(bPath,mod,SV_OFF);

    //disable set resolution 
    vRegenSetResolutionMode(bPath,mod, SV_BOTH);   //mt5399 , regen can be trigger by previous module and regen module
    vRegenSetResolutionInTriSel(bPath,mod, SV_ON);
}

/**
  * @brief Set Regen input resolution
  * @param u4VdpId VDP_1/VDP_2
  */
void vRegenSetInputResolutionReg(UINT32 u4VdpId, REGEN_MODULE_E mod,UINT32 u4InW, UINT32 u4InH)
{        
    switch ( mod )
    {
        case  REGEN_MODULE_PRE_REGEN:
          REGEN_WRITE_FLD(PRE_RE_GEN_MAIN_01 + getRegenOffset(getScalerPath(u4VdpId)), (UINT16)u4InW, RG_M_IN_ACTIVE_WIDTH);
          REGEN_WRITE_FLD(PRE_RE_GEN_MAIN_01 + getRegenOffset(getScalerPath(u4VdpId)), (UINT16)u4InH, RG_M_IN_ACTIVE_HEIGHT);
        break;  
       case REGEN_MODULE_FSC_REGEN:
          REGEN_WRITE_FLD(SCPIP_SCSYS_53 + getFSCRegenOffset(getScalerPath(u4VdpId)), (UINT16)u4InW, SCSYS_53_SC_M_REGEN_IN_ACTIVE_WIDTH);
          REGEN_WRITE_FLD(SCPIP_SCSYS_53 + getFSCRegenOffset(getScalerPath(u4VdpId)), (UINT16)u4InH, SCSYS_53_SC_M_REGEN_IN_ACTIVE_HEIGHT);
       break;
       case REGEN_MODULE_MONOUT_REGEN:
          REGEN_WRITE_FLD(SCPIP_MON_REGEN_01 ,  (UINT16)u4InW, SCPIP_MON_OUT_RG_M_IN_ACTIVE_WIDTH);
          REGEN_WRITE_FLD(SCPIP_MON_REGEN_01 ,  (UINT16)u4InH, SCPIP_MON_OUT_RG_M_IN_ACTIVE_HEIGHT);
       break;
    }
}

/**
  * @brief Set Regen output resolution
  * @param u4VdpId VDP_1/VDP_2
  */
void vRegenSetOutputResolutionReg(UINT32 u4VdpId, REGEN_MODULE_E mod,UINT32 u4InW, UINT32 u4InH)
{        
    switch (mod)
    { 
       case REGEN_MODULE_PRE_REGEN:
          REGEN_WRITE_FLD(PRE_RE_GEN_MAIN_02 + getRegenOffset(getScalerPath(u4VdpId)), (UINT16)u4InW, RG_M_OUT_ACTIVE_WIDTH);
          REGEN_WRITE_FLD(PRE_RE_GEN_MAIN_02 + getRegenOffset(getScalerPath(u4VdpId)), (UINT16)u4InH, RG_M_OUT_ACTIVE_HEIGHT);
       break;
       case REGEN_MODULE_FSC_REGEN:
          REGEN_WRITE_FLD(SCPIP_SCSYS_54 + getFSCRegenOffset(getScalerPath(u4VdpId)), (UINT16)u4InW, SCSYS_54_SC_M_REGEN_OUT_ACTIVE_WIDTH);
          REGEN_WRITE_FLD(SCPIP_SCSYS_54 + getFSCRegenOffset(getScalerPath(u4VdpId)), (UINT16)u4InH, SCSYS_54_SC_M_REGEN_OUT_ACTIVE_HEIGHT);
       break;
       case  REGEN_MODULE_MONOUT_REGEN:        
          REGEN_WRITE_FLD(SCPIP_MON_REGEN_02 , (UINT16)u4InW, SCPIP_MON_OUT_RG_M_OUT_ACTIVE_WIDTH);
          REGEN_WRITE_FLD(SCPIP_MON_REGEN_02 , (UINT16)u4InH, SCPIP_MON_OUT_RG_M_OUT_ACTIVE_HEIGHT);
       break;
    }
}

UINT32 u4RegenGetInHeight(UINT8 bPath,REGEN_MODULE_E mod)
{
    return ((mod == REGEN_MODULE_PRE_REGEN)?
            REGEN_READ_FLD(PRE_RE_GEN_MAIN_05 + getRegenOffset(bPath), RG_M_IN_HEIGHT_MEASURE):
            REGEN_READ_FLD(SCPIP_SCSYS_57 + getFSCRegenOffset(bPath), SCSYS_57_SC_M_REGEN_IN_HEIGHT_MEASURE));
}

UINT32 u4RegenGetInWidth(UINT8 bPath,REGEN_MODULE_E mod)
{
    return ((mod == REGEN_MODULE_PRE_REGEN)?
            REGEN_READ_FLD(PRE_RE_GEN_MAIN_05 + getRegenOffset(bPath), RG_M_IN_WIDTH_MEASURE):
            REGEN_READ_FLD(SCPIP_SCSYS_57 + getFSCRegenOffset(bPath), SCSYS_57_SC_M_REGEN_IN_WIDTH_MEASURE));
}

UINT32 u4RegenGetOutHeight(UINT8 bPath,REGEN_MODULE_E mod)
{
    return ((mod == REGEN_MODULE_PRE_REGEN)?
            REGEN_READ_FLD(PRE_RE_GEN_MAIN_06 + getRegenOffset(bPath), RG_M_OUT_HEIGHT_MEASURE):
            REGEN_READ_FLD(SCPIP_SCSYS_58 + getFSCRegenOffset(bPath), SCSYS_58_SC_M_REGEN_OUT_HEIGHT_MEASURE));
}

UINT32 u4RegenGetOutWidth(UINT8 bPath,REGEN_MODULE_E mod)
{
    return ((mod == REGEN_MODULE_PRE_REGEN)?
            REGEN_READ_FLD(PRE_RE_GEN_MAIN_06 + getRegenOffset(bPath), RG_M_OUT_WIDTH_MEASURE):
            REGEN_READ_FLD(SCPIP_SCSYS_58 + getFSCRegenOffset(bPath), SCSYS_58_SC_M_REGEN_OUT_WIDTH_MEASURE));
}

/**
  * @brief Set Regen L Start
  * @param u4VdpId VDP_1/VDP_2
  */
void vRegenSetLStartReg(UINT32 u4VdpId,REGEN_MODULE_E mod,UINT32 u4StaH, UINT32 u4StaV)
{ 
    switch (mod )
   {
        case REGEN_MODULE_PRE_REGEN:
          REGEN_WRITE_FLD(PRE_RE_GEN_MAIN_03 + getRegenOffset(getScalerPath(u4VdpId)), (UINT16)u4StaH, RG_M_H_START_L);
          REGEN_WRITE_FLD(PRE_RE_GEN_MAIN_03 + getRegenOffset(getScalerPath(u4VdpId)), (UINT16)u4StaV, RG_M_V_START_L);
        break;
        case REGEN_MODULE_FSC_REGEN:
           REGEN_WRITE_FLD(SCPIP_SCSYS_55 + getFSCRegenOffset(getScalerPath(u4VdpId)), (UINT16)u4StaH, RG_M_H_START_L);
           REGEN_WRITE_FLD(SCPIP_SCSYS_55 + getFSCRegenOffset(getScalerPath(u4VdpId)), (UINT16)u4StaV, RG_M_V_START_L);        
        break;
        case REGEN_MODULE_MONOUT_REGEN:
            REGEN_WRITE_FLD(SCPIP_MON_REGEN_03 ,  (UINT16)u4StaH , SCPIP_MON_OUT_RG_M_H_START_L);
            REGEN_WRITE_FLD(SCPIP_MON_REGEN_03 ,  (UINT16)u4StaV , SCPIP_MON_OUT_RG_M_V_START_L);
        break;     
   } 
}
/**
  * @brief Set Regen R Start
  * @param u4VdpId VDP_1/VDP_2
  */
void vRegenSetRStartReg(UINT32 u4VdpId,REGEN_MODULE_E mod, UINT32 u4StaH, UINT32 u4StaV)
{        
    switch(mod)
    { 
        case REGEN_MODULE_PRE_REGEN:
           REGEN_WRITE_FLD(PRE_RE_GEN_MAIN_04 + getRegenOffset(getScalerPath(u4VdpId)), (UINT16)u4StaH, SCSYS_55_SC_M_REGEN_H_START_L);
           REGEN_WRITE_FLD(PRE_RE_GEN_MAIN_04 + getRegenOffset(getScalerPath(u4VdpId)), (UINT16)u4StaV, SCSYS_55_SC_M_REGEN_V_START_L);
        break;
        case REGEN_MODULE_FSC_REGEN:
           REGEN_WRITE_FLD(SCPIP_SCSYS_56 + getFSCRegenOffset(getScalerPath(u4VdpId)), (UINT16)u4StaH, SCSYS_56_SC_M_REGEN_H_START_R);
           REGEN_WRITE_FLD(SCPIP_SCSYS_56 + getFSCRegenOffset(getScalerPath(u4VdpId)), (UINT16)u4StaV, SCSYS_56_SC_M_REGEN_V_START_R);            
        break;
        case REGEN_MODULE_MONOUT_REGEN:            
            REGEN_WRITE_FLD(SCPIP_MON_REGEN_04 ,  (UINT16)u4StaH , SCPIP_MON_OUT_RG_M_H_START_R);
            REGEN_WRITE_FLD(SCPIP_MON_REGEN_04 ,  (UINT16)u4StaV  , SCPIP_MON_OUT_RG_M_V_START_R);
        break;
    }
}
 
void vRegenSetModeChanged(UINT8 bPath, REGEN_MODULE_E mod)
{
    REGEN_INFO_T* regenInfo;
    regenInfo = getRegenInfo(bPath,mod);

    regenInfo->bREGENModeChanged = SV_TRUE;
}

void vRegenSetRegenEnableReg(UINT8 bPath,REGEN_MODULE_E mod, UINT8 bOnOff)
{
    switch(mod)
    {
    case  REGEN_MODULE_PRE_REGEN:
        REGEN_WRITE_FLD(PRE_RE_GEN_MAIN_00+getRegenOffset(bPath), (bOnOff==SV_ON)?1:0 , RG_M_REGEN_ENABLE);
    break;
    case REGEN_MODULE_FSC_REGEN:
        REGEN_WRITE_FLD(SCPIP_SCSYS_52+getFSCRegenOffset(bPath), (bOnOff==SV_ON)?1:0 , SCSYS_52_SC_M_REGEN_ENABLE);
    break;
    case REGEN_MODULE_MONOUT_REGEN:
        REGEN_WRITE_FLD(SCPIP_MON_REGEN_00, (bOnOff==SV_ON)?1:0 , SCPIP_MON_OUT_RG_M_REGEN_ENABLE);         
    break;
    default:
        Printf("unknow regen module set regen enable.\n");        
    break;    
    }
}

void vRegenSetClipEnableReg(UINT8 bPath, REGEN_MODULE_E mod,UINT8 bOnOff)
{
    switch (mod )
    {
       case  REGEN_MODULE_PRE_REGEN:
           REGEN_WRITE_FLD(PRE_RE_GEN_MAIN_00+getRegenOffset(bPath), (bOnOff==SV_ON)?1:0 , RG_M_CLIP_EN);
       break;
       case REGEN_MODULE_FSC_REGEN:
           REGEN_WRITE_FLD(SCPIP_SCSYS_52+getFSCRegenOffset(bPath), (bOnOff==SV_ON)?1:0 , SCSYS_52_SC_M_REGEN_CLIP_EN);
       break;
       case REGEN_MODULE_MONOUT_REGEN:
           REGEN_WRITE_FLD(SCPIP_MON_REGEN_00, (bOnOff==SV_ON)?1:0 , SCPIP_MON_OUT_RG_M_CLIP_EN);         
       break;
       default:
           Printf("unknow regen module set clip.\n");        
       break;
    }
}

void vRegenSetRegenInFormat(UINT8 bPath, REGEN_MODULE_E mod,REGEN_IN_FMT eInFmt)
{
    (mod == REGEN_MODULE_PRE_REGEN)?
    REGEN_WRITE_FLD(PRE_RE_GEN_MAIN_00+getRegenOffset(bPath), eInFmt, RG_M_IN_FORMAT):        
    REGEN_WRITE_FLD(SCPIP_SCSYS_52+getRegenOffset(bPath), eInFmt, RG_M_IN_FORMAT);
}

void vRegenClearSetResOutput(UINT8 bPath, REGEN_MODULE_E mod)
{
    (mod == REGEN_MODULE_PRE_REGEN)?
    REGEN_WRITE_FLD(PRE_RE_GEN_MAIN_00+getRegenOffset(bPath), 1, RG_M_SET_RES_CLEAR_O):
    REGEN_WRITE_FLD(SCPIP_SCSYS_52+getFSCRegenOffset(bPath), 1, SCSYS_52_SC_M_REGEN_SET_RES_CLEAR_O);
}

void vRegenClearSetResInput(UINT8 bPath, REGEN_MODULE_E mod)
{
    (mod == REGEN_MODULE_PRE_REGEN)?
    REGEN_WRITE_FLD(PRE_RE_GEN_MAIN_00+getRegenOffset(bPath), 1, RG_M_SET_RES_CLEAR_I):
    REGEN_WRITE_FLD(SCPIP_SCSYS_52+getFSCRegenOffset(bPath), 1, SCSYS_52_SC_M_REGEN_SET_RES_CLEAR_I);
}

UINT8 bRegenGetSetResOutputStatus(UINT8 bPath, REGEN_MODULE_E mod)
{
    UINT8 ret;

    ret = (mod == REGEN_MODULE_PRE_REGEN)?
          REGEN_READ_FLD(PRE_RE_GEN_MAIN_00+getRegenOffset(bPath), RG_M_SET_RES_FINISHED_O):
          REGEN_READ_FLD(SCPIP_SCSYS_52+getFSCRegenOffset(bPath), SCSYS_52_SC_M_REGEN_SET_RES_FINISHED_O);

    return ret;
}

UINT8 bRegenGetSetResInputStatus(UINT8 bPath, REGEN_MODULE_E mod)
{
    UINT8 ret;

    ret = (mod == REGEN_MODULE_PRE_REGEN)?
          REGEN_READ_FLD(PRE_RE_GEN_MAIN_00+getRegenOffset(bPath), RG_M_SET_RES_FINISHED_I):
          REGEN_READ_FLD(SCPIP_SCSYS_52+getFSCRegenOffset(bPath), SCSYS_52_SC_M_REGEN_SET_RES_FINISHED_I);

    return ret;
}

void vRegenSetImport(UINT8 bPath,REGEN_MODULE_E mod)
{
    (mod == REGEN_MODULE_PRE_REGEN)?
    REGEN_WRITE_FLD(PRE_RE_GEN_MAIN_00+getRegenOffset(bPath), 1, RG_M_SET_RES_ACTIVE):
    REGEN_WRITE_FLD(SCPIP_SCSYS_52+getFSCRegenOffset(bPath), 1, SCSYS_52_SC_M_REGEN_SET_RES_ACTIVE);
}

void vRegenSetResolutionMode(UINT8 bPath, REGEN_MODULE_E mod,UINT8 bOnOff)
{
    switch ( mod )
    {
    case REGEN_MODULE_PRE_REGEN:
        REGEN_WRITE_FLD(PRE_RE_GEN_MAIN_00+getRegenOffset(bPath), bOnOff , RG_M_SET_RES_MODE);
    break;
    case REGEN_MODULE_FSC_REGEN:
        REGEN_WRITE_FLD(SCPIP_SCSYS_52+getFSCRegenOffset(bPath), bOnOff, SCSYS_52_SC_M_REGEN_SET_RES_MODE);
    break;
    case REGEN_MODULE_MONOUT_REGEN:
        REGEN_WRITE_FLD(SCPIP_MON_REGEN_00, bOnOff, SCPIP_MON_OUT_RG_M_SET_RES_MODE);
        break;
    default:
        Printf("unknow regen module set resolution mode setting\n");
        break;
    }
}

void vRegenSetResolutionInTriSel(UINT8 bPath, REGEN_MODULE_E mod,UINT8 bOnOff)
{
   // (mod == REGEN_MODULE_PRE_REGEN)?
   switch ( mod )
   {
    case REGEN_MODULE_PRE_REGEN:
        REGEN_WRITE_FLD(PRE_RE_GEN_MAIN_00+getRegenOffset(bPath), (bOnOff==SV_ON)?1:0, RG_M_SET_RES_IN_TRIG_SEL);
    break;
    case REGEN_MODULE_FSC_REGEN:
        REGEN_WRITE_FLD(SCPIP_SCSYS_52+getFSCRegenOffset(bPath), (bOnOff==SV_ON)?1:0, SCSYS_52_SC_M_REGEN_SET_RES_IN_TRIG_SEL);
    break;
    case REGEN_MODULE_MONOUT_REGEN:
        REGEN_WRITE_FLD(SCPIP_MON_REGEN_00, (bOnOff==SV_ON)?1:0, SCPIP_MON_OUT_RG_M_SET_RES_IN_TRIG_SEL);
    break;    
    default:
        Printf("unknow regen module trigger select setting\n");
    break;
   }
}

void vRegenSetResolutionBypass(UINT8 bPath,REGEN_MODULE_E mod, UINT8 bBypass)
{
    switch ( mod )
    {
      case  REGEN_MODULE_PRE_REGEN:
          REGEN_WRITE_FLD(PRE_RE_GEN_MAIN_00+getRegenOffset(bPath), (bBypass==SV_ON)?1:0, RG_M_BYPASS_SET_RES_DB);
      break;
      case  REGEN_MODULE_FSC_REGEN:
          REGEN_WRITE_FLD(SCPIP_SCSYS_52+getFSCRegenOffset(bPath), (bBypass==SV_ON)?1:0, SCSYS_52_SC_M_REGEN_BYPASS_SET_RES_DB);
      break;
      case REGEN_MODULE_MONOUT_REGEN:
          REGEN_WRITE_FLD(SCPIP_MON_REGEN_00 , (bBypass==SV_ON)?1:0, SCPIP_MON_OUT_RG_M_BYPASS_SET_RES_DB);
      break;
      default:
        Printf("unknow regen module bypass setting\n");
      break;
   }
}

UINT8 fgRegenIsModeChanged(UINT8 bPath,REGEN_MODULE_E mod)
{
    REGEN_INFO_T* regenInfo= getRegenInfo(bPath,mod);
    UINT8 ret= SV_FALSE;

    if(regenInfo->bREGENModeChanged == SV_TRUE)
    {
         ret = SV_TRUE;
    }
    return ret;
}

void vRegenClearModeChanged(UINT8 bPath,REGEN_MODULE_E mod)
{
    REGEN_INFO_T* regenInfo;
    regenInfo = getRegenInfo(bPath,mod);

    regenInfo->bREGENModeChanged = SV_FALSE;
}


static void _vRegenUpdateInOutPrm(UINT8 bPath,REGEN_MODULE_E mod)
{
    REGEN_INFO_T* regenInfo;
    regenInfo = getRegenInfo(bPath,mod);

    VRMGetModuleVRMInfo(bPath, 
                        (mod==REGEN_MODULE_PRE_REGEN)?VRM_MODULE_REGEN:VRM_MODULE_FSC_REGEN, 
                        &(regenInfo->regenIn), 
                        &(regenInfo->regenOut));    

//    if((bDrvVideoIsSrcInterlace(bPath) == SV_TRUE)
//        ||u4DrvTDTVIsDualFPI())
    if((regenInfo->regenIn.u2ScanMode == 1) && ( mod != REGEN_MODULE_FSC_REGEN ) )
    {
        regenInfo->regenIn.u2Y = regenInfo->regenIn.u2Y>>1;
   		regenInfo->regenIn.u2Height = regenInfo->regenIn.u2Height>>1;
   		regenInfo->regenOut.u2Y = regenInfo->regenOut.u2Y>>1;
   		regenInfo->regenOut.u2Height = regenInfo->regenOut.u2Height>>1;
    }
}

static void _vRegenUpdateInputResolution(UINT8 bPath,REGEN_MODULE_E mod,UINT16 inW, UINT16 inH)
{
    REGEN_INFO_T* regenInfo;    
    regenInfo = getRegenInfo(bPath,mod);

    regenInfo->u4InputWidth = inW;
    regenInfo->u4InputHeight = inH;

    if(SV_OFF==u4DrvTDTVIs3DPhoto(NULL)&&(bPath == VDP_1))
    {
        switch(TD_MIB_IN(u4DrvTDTVDIModeQuery()))
        {          
            case E_TD_IN_TB_I:
            case E_TD_IN_TB_P:
                regenInfo->u4InputHeight/=2;
                break;  
            case E_TD_IN_SBS_I:
            case E_TD_IN_SBS_P:
			case E_TD_IN_RD_I:
			case E_TD_IN_RD_P:	
                regenInfo->u4InputWidth/=2;
                break;
            default:
                break;
        }    
    }
}

static void _vRegenUpdateOutputResolution(UINT8 bPath, REGEN_MODULE_E mod,UINT16 outX, UINT16 outY, UINT16 outW, UINT16 outH)
{
    REGEN_INFO_T* regenInfo;    
    regenInfo = getRegenInfo(bPath,mod);

    regenInfo->u4OutputX = outX;
    regenInfo->u4OutputY = outY;
    regenInfo->u4OutputWidth = outW;
    regenInfo->u4OutputHeight = outH;

    if(SV_OFF==u4DrvTDTVIs3DPhoto(NULL)&&(bPath == VDP_1))
    {
        switch(TD_MIB_IN(u4DrvTDTVDIModeQuery()))
        {          
            case E_TD_IN_TB_I:
            case E_TD_IN_TB_P:
                regenInfo->u4OutputHeight/=2;
                regenInfo->u4OutputY/=2;
                break;  
            case E_TD_IN_SBS_I:
            case E_TD_IN_SBS_P:
			case E_TD_IN_RD_I:
			case E_TD_IN_RD_P:
                regenInfo->u4OutputWidth/=2;
                regenInfo->u4OutputX/=2;
                break;
            default:
                break;
        }
    }
}

static void _vRegenUpdateDispPrm(UINT8 bPath,REGEN_MODULE_E mod)
{
    REGEN_INFO_T* regenInfo;
    REGEN_INFO_T regenPreInfo;
    regenInfo = getRegenInfo(bPath,mod);

    x_memcpy((void *)&regenPreInfo,(const void *)regenInfo,sizeof(REGEN_INFO_T));

    //1. update in/out resolution from VRM
    _vRegenUpdateInOutPrm(bPath,mod);
    //2. update input resolution to global variable
    _vRegenUpdateInputResolution(bPath,mod,regenInfo->regenIn.u2Width, regenInfo->regenIn.u2Height);
    //3. update output resolution to global variable
    _vRegenUpdateOutputResolution(bPath,mod, regenInfo->regenOut.u2X, regenInfo->regenOut.u2Y, 
                                         regenInfo->regenOut.u2Width, regenInfo->regenOut.u2Height);     

    //4. set mute when import is off.
    if(x_memcmp((const void *)&regenPreInfo,(const void *)regenInfo,sizeof(REGEN_INFO_T)))
    {
        if(vDrvGetImportTriggerModule(bPath) == IMPORT_MODULE_NONE)
        {
            #define REGEN_MUTE_PERIOD 5
            _vDrvVideoSetMute(MUTE_MODULE_REGEN, bPath, REGEN_MUTE_PERIOD, TRUE);
            LOG(1,"REGEN change mute when no import.\n");
        }
    }
}
 
static void _vRegenUpdateReg(UINT8 bPath,REGEN_MODULE_E mod)
{
    REGEN_INFO_T* regenInfo;
    REGEN_IN_FMT eInFmt=REGEN_IN_FMT_2D;

    regenInfo = getRegenInfo(bPath,mod);

    if(regenInfo->u4InputWidth == 0 || regenInfo->u4InputHeight == 0)
    {
        goto REGEN_UPDATE_FINISH;
    }    

    //0. Set 3D format
    if(SV_OFF==u4DrvTDTVIs3DPhoto(NULL)&&(bPath == VDP_1))
    {    
        switch(TD_MIB_IN(u4DrvTDTVDIModeQuery()))
        {
            case E_TD_IN_FS_I:
            case E_TD_IN_FS_P:
                eInFmt =REGEN_IN_FMT_FS;
                break;            
            case E_TD_IN_TB_I:
            case E_TD_IN_TB_P:
                eInFmt =REGEN_IN_FMT_TB;
                break;  
            case E_TD_IN_SBS_I:
            case E_TD_IN_SBS_P:
			case E_TD_IN_RD_I:
			case E_TD_IN_RD_P:
                eInFmt =REGEN_IN_FMT_SS;
                break;
            default:
                eInFmt =REGEN_IN_FMT_2D;                
                break;
        }
    }

    //0. update input format
    vRegenSetRegenInFormat(bPath,mod, eInFmt);
    
    //1. update input resolution
    vRegenSetInputResolutionReg(bPath,mod,regenInfo->u4InputWidth,regenInfo->u4InputHeight);
    //2. update output resolution
    #if defined(REGEN_WA1_FOR_MIB_HEIGHT)
    if(TD_TTT_IN(u4DrvTDTVTTTModeQuery(bPath)) == E_TD_IN_2D_I || 
        TD_TTT_IN(u4DrvTDTVTTTModeQuery(bPath)) == E_TD_IN_2D_P ||
        TD_TTT_OUT(u4DrvTDTVTTTModeQuery(bPath)) == E_TD_OUT_3D_2_2D
        )
    {
        vRegenSetOutputResolutionReg(bPath,mod,regenInfo->u4OutputWidth,regenInfo->u4InputHeight - regenInfo->u4OutputY);    
    }
    else
    #endif
    {
        vRegenSetOutputResolutionReg(bPath,mod,regenInfo->u4OutputWidth,regenInfo->u4OutputHeight);
    }
    //3. update L/R overscan
    vRegenSetLStartReg(bPath,mod,regenInfo->u4OutputX,regenInfo->u4OutputY);
    vRegenSetRStartReg(bPath,mod,regenInfo->u4OutputX,regenInfo->u4OutputY);    
    //4. set regen enable
    vRegenSetRegenEnableReg(bPath,mod,SV_ON);
    vRegenSetClipEnableReg(bPath,mod,SV_ON);
    //5. import 
    if(vDrvGetImportTriggerModule(bPath) == IMPORT_MODULE_NONE)
    {
        vRegenSetImport(bPath,mod); // trigger
    }
    
REGEN_UPDATE_FINISH:    
    //6. Resolution Reg update done
    (mod == REGEN_MODULE_PRE_REGEN)?
    vDrvClrImportWaitFlag(bPath, VRM_IMPORT_WAIT_FLAG_REGEN):
    vDrvClrImportWaitFlag(bPath, VRM_IMPORT_WAIT_FLAG_FSC_REGEN);        
}

void vRegenStaMon(UINT8 bPath)
{         
    if(SV_VP_MAIN==bPath)
    {   
        MON_STA(REGEN_M,iHA,IO32ReadFldAlign(PRE_RE_GEN_MAIN_01, RG_M_IN_ACTIVE_WIDTH));
        MON_STA(REGEN_M,iVA,IO32ReadFldAlign(PRE_RE_GEN_MAIN_01, RG_M_IN_ACTIVE_HEIGHT));
        MON_STA(REGEN_M,oHA,IO32ReadFldAlign(PRE_RE_GEN_MAIN_02, RG_M_OUT_ACTIVE_WIDTH));
        MON_STA(REGEN_M,oVA,IO32ReadFldAlign(PRE_RE_GEN_MAIN_02, RG_M_OUT_ACTIVE_HEIGHT));                                
    }
    else
    {
        MON_STA(REGEN_P,iHA,IO32ReadFldAlign(PRE_RE_GEN_MAIN_01, RG_P_IN_ACTIVE_WIDTH));
        MON_STA(REGEN_P,iVA,IO32ReadFldAlign(PRE_RE_GEN_MAIN_01, RG_P_IN_ACTIVE_HEIGHT));
        MON_STA(REGEN_P,oHA,IO32ReadFldAlign(PRE_RE_GEN_MAIN_02, RG_P_OUT_ACTIVE_WIDTH));
        MON_STA(REGEN_P,oVA,IO32ReadFldAlign(PRE_RE_GEN_MAIN_02, RG_P_OUT_ACTIVE_HEIGHT));                                
    }
}
    
/**
  * @regen main loop
  * @param u4VdpId VDP_1/VDP_2
  */
void vRegenLoop(UINT8 bPath,REGEN_MODULE_E mod)
{
    if(fgRegenIsModeChanged(bPath,mod)== SV_TRUE)
    {
        _vRegenUpdateDispPrm(bPath,mod);
        _vRegenUpdateReg(bPath,mod);
        vRegenClearModeChanged(bPath,mod);      
    }
}


/**
 * @brief Clear Regen set resolution status
 * @param u1VdpId VDP_1/VDP_2
 */
void vDrvRegenClrSetRes(UINT8 u1VdpId, REGEN_MODULE_E mod,SET_RES_STATUS_TYPE_E eStatusType)
{
    if (eStatusType == SET_RES_OUTPUT || eStatusType == SET_RES_BOTH)
    {
        vRegenClearSetResOutput(u1VdpId,mod);
    }
    if (eStatusType == SET_RES_INPUT || eStatusType == SET_RES_BOTH)
    {
        vRegenClearSetResInput(u1VdpId,mod);
    }
}

/**
 * @brief Get Regen set resolution status
 * @param u1VdpId VDP_1/VDP_2
 * @param eStatusType     SET_RES_INPUT, SET_RES_OUTPUT, SET_RES_BOTH,
 */
BOOL bDrvRegenIsSetResDone(UINT8 u1VdpId, REGEN_MODULE_E mod,SET_RES_STATUS_TYPE_E eStatusType)
{
    UINT8 bInStatus, bOutStatus;
    bInStatus  = bRegenGetSetResInputStatus(u1VdpId,mod);
    bOutStatus = bRegenGetSetResOutputStatus(u1VdpId,mod);
    
    if (eStatusType == SET_RES_INPUT)
    {
        return bInStatus;
    }
    else if (eStatusType == SET_RES_OUTPUT)
    {
        return bOutStatus;
    }
    else
    {
        return (bInStatus & bOutStatus);
    }
}


void vDrvRegenInOutStatusDump(UINT8 bPath,REGEN_MODULE_E mod)
{
    LOG(0,"\tRGN  IN HT: N/A VT: N/A HA:%4d VA:%4d\n",           
        u4RegenGetInWidth(bPath,mod),                
        u4RegenGetInHeight(bPath,mod)
        );
    LOG(0,"\tRGN OUT HT: N/A VT: N/A HA:%4d VA:%4d\n",           
        u4RegenGetOutWidth(bPath,mod),                
        u4RegenGetOutHeight(bPath,mod)
        );        
}
