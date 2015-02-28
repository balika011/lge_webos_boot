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

#include "x_assert.h"

#include "vdo_rm.h"
#include "x_os.h"
#include "drv_predown.h"
#include "drv_scaler.h"
#include "drv_scaler_psc.h"
#include "drv_scaler_gfx.h"
#include "drv_mon_out.h"
#include "vdp_venc_ctrl.h"


#include "drv_tdtv.h"
#include "drv_tdtv_drvif.h"
#include "vdo_misc.h"
#include "drv_regen.h"
#include "vdp_display.h"
#include "hw_sw.h"
#include "b2r_if.h"
#include "scpos_debug.h"
#include "drv_di.h"
#include "drv_display.h"
#ifdef CC_MT5399
#include "drv_mjc.h"
#endif
#include "tve_if.h"

#define VRM_MIXER_MAX_FR 120

EXTERN BOOL _fgAutoSearch;
#ifdef REGEN_BOTH_IN_PREREGEN_AND_FSCREGEN
BOOL _fgOvsn1PixelInPreregen = SV_FALSE;
BOOL _fgOvsn2PixelInPreregen = SV_FALSE;
#endif
#define PDS_WIDTH_FIX_UNKNOWN 0xFFFF

static VRM_INFO_T _vrm_info[VDP_MAX_NS][VRM_MODULE_MAX];
static VRM_INFO_T _vrm_info_pre[VDP_MAX_NS][VRM_MODULE_MAX];

static UINT32 _u4VRMEventFlag[VDP_MAX_NS]= {0,0};
static UINT32 _u4VRMAppFlag= 0;
static UINT32 _u4DIMaxOutWidth[VDP_MAX_NS] = {1920,1920};
static APP_RES_T _rVencRes;
static UINT32 _u4PDSWidthFix[VDP_MAX_NS] = {PDS_WIDTH_FIX_UNKNOWN,PDS_WIDTH_FIX_UNKNOWN};
static FORCED_OVSN_MODULE_E _forcedOvsnModule[VDP_MAX_NS] = {FORCED_OVSN_MODULE_AUTO, FORCED_OVSN_MODULE_AUTO};
static OVSN_MODULE_E _ovsnModule[VDP_MAX_NS] = {OVSN_MODULE_REGEN, OVSN_MODULE_REGEN};

typedef enum
{
    B2R_TRIG_START,            
    B2R_TRIG_WAITING,
    B2R_TRIG_OFF,
}B2R_TRIGGER_STATE_E;
static B2R_TRIGGER_STATE_E _eVRMWaitForB2RTrigger[VDP_MAX_NS] = {B2R_TRIG_OFF, B2R_TRIG_OFF};
UINT8 bVRMUpdateSrcReg[VDP_MAX_NS] = {FALSE, FALSE};

UCHAR* VRM_MODULE_NAMES[VRM_MODULE_MAX] = 
{
    DEF_TO_STR(_OMUX),
    DEF_TO_STR(_CB),
    DEF_TO_STR(_DQ),
    DEF_TO_STR(_TTT),
    DEF_TO_STR(_PDS),
    DEF_TO_STR(_REGEN),
    DEF_TO_STR(_MIB),
    DEF_TO_STR(_FREGEN),
    DEF_TO_STR(_FSC),
    DEF_TO_STR(_MIXER),
    DEF_TO_STR(_MJC),
    DEF_TO_STR(_PSC),
    DEF_TO_STR(_PMIXER),
    DEF_TO_STR(_OSTG),    
};

/***************************/
/** For Set resolutin ******/
/***************************/
static IMPORT_INFO_T _importInfo[VDP_MAX_NS];
static void vVRMFireFSCResolutionChange(UINT8 bPath);
static void vVRMFireMIBResolutionChange(UINT8 bPath);

///////////////////////////////////////////////////
void vVRMSwInit(void)
{
    UINT8 i;
    UINT8 bPath;

    for(bPath = SV_VP_MAIN;bPath <= SV_VP_PIP; bPath++)
    {
        for(i=0;i<VRM_MODULE_MAX;i++)
        {
            _vrm_info[bPath][i].u2VRMID = i;
        }
        vDrvSetPDSTriggerFlg(bPath, FALSE);
        _importInfo[bPath].forcedImportModule = IMPORT_MODULE_UNKNOWN;
    }
    _u4VRMAppFlag = 0;
}

VRM_INFO_T* VRMGetVRMInfo(UINT8 bPath, VRM_MODULE_E moduleID)
{
    VRM_INFO_T* ret = NULL;

    //error handle
    if(bPath > SV_VP_PIP)
    {
        return ret;
    }
    
    if(moduleID < VRM_MODULE_MAX)
    {
        ret = &(_vrm_info[bPath][moduleID]);
    }
    return ret;
}

void VRMGetModuleVRMInfo(UINT8 bPath, VRM_MODULE_E moduleID, VRM_INFO_T* in, VRM_INFO_T* out)
{
    //error handle
    if(bPath > SV_VP_PIP)
    {
        return;
    }

    if(moduleID < VRM_MODULE_MAX)
    {
        x_memcpy((void*)in, (void*)(&_vrm_info[bPath][moduleID]), sizeof(VRM_INFO_T));
    }     

    if(((VRM_MODULE_E)((UINT16)moduleID + 1)) < VRM_MODULE_MAX)
    {
        x_memcpy((void*)out, (void*)(&_vrm_info[bPath][moduleID+1]), sizeof(VRM_INFO_T));
    }
   
    return;
}

void vVRMDumpVRMInfo(UINT8 bPath, VRM_MODULE_E moduleID)
{
    VRM_INFO_T* vrmInfo;    
    vrmInfo = VRMGetVRMInfo(bPath, moduleID);
    
    if(vrmInfo == NULL)
    {
        LOG(2, "Illegal module ID(%d)\n",moduleID);
        return;
    }
    
    LOG(2, "[%2d%8s  [%1d] X=%4d Y=%4d HA=%4d VA=%4d HT=%4d VT=%4d FR=%3d %c]\n",
            vrmInfo->u2VRMID,
            VRM_MODULE_NAMES[moduleID],
            bPath,
            vrmInfo->u2X,
            vrmInfo->u2Y,
            vrmInfo->u2Width,
            vrmInfo->u2Height,
            vrmInfo->u2HTotal,
            vrmInfo->u2VTotal,
            vrmInfo->u2FrameRate,
            vrmInfo->u2ScanMode?'I':'P'
            );
    return;    
}

void vVRMDumpAllVRMInfo(void)
{
    UINT8 bPath= SV_VP_MAIN;
    UINT8 i;
    UINT32 mode;
    VDP_SEAMLESS_INFO_T b2rVrmInfo;

    mode = u4DrvVrmGetAppFlag();
    LOG(0, "Application mode %d\n", mode);
    if (mode & VRM_APP_GRAPHIC_DUMP)
    {
        LOG(0, "GFX:   %d [%4dx%4d]\n", u1Scpip_GFX_GetSource(), u4Scpip_GFX_GetWidth(), u4Scpip_GFX_GetHeight());
    }
    #ifdef CC_SUPPORT_VENC
    if (mode & VRM_APP_VIDEO_ENC)
    {
        LOG(0, "VENC:  %d [%4dx%4d]%c\n", u1DrvScpipH264GetSource(), u4DrvScpipH264GetWidth(), u4DrvScpipH264GetHeight(), u4DrvScpipH264IsInterlace()?'I':'P');
    }
    #endif
    if (mode & VRM_APP_SCART_OUT)
    {
        LOG(0, "SCART: %d\n", SCPIP_READ_FLD(SCPIP_SCSYS_14, SCSYS_14_SC_TVE_SRC_SEL)); // FIXME
    }
    LOG(2,"Seamless change %d\n",VDP_GetSeamlessInfo(bPath, &b2rVrmInfo)!=VDP_SET_ERROR);
    LOG(2,"Seamless change with %d height %d\n",b2rVrmInfo.u4SrcWidth,b2rVrmInfo.u4SrcHeight);
    LOG(0, "========================================================================\n");
    LOG(0, "FW STA DUMP\n");        
    LOG(0, "========================================================================\n");
    for(bPath = SV_VP_MAIN;bPath <= SV_VP_PIP; bPath++)
    {
        for(i=0;i<VRM_MODULE_MAX;i++)
        {
            vVRMDumpVRMInfo(bPath, (VRM_MODULE_E)i);
        }

        LOG(2, "\tForce Import Module 0x%X\n",_importInfo[bPath].forcedImportModule);
        LOG(2, "\tImport Module       0x%X\n",_importInfo[bPath].importModule);
        LOG(2, "\tImport Flag         0x%X\n",_importInfo[bPath].importWaitFlag); 
        LOG(2, "\tVRMRdyForB2R          %d\n",bVRMReadyForB2R(bPath));
        LOG(2, "\tvVRMB2RTrigge         %d\n",_eVRMWaitForB2RTrigger[bPath]);
        LOG(2, "========================================================================\n");
    }

    //Video Module Status Dump
    
    LOG(0, "========================================================================\n");
    LOG(0, "HW STA DUMP\n");        
    LOG(0, "========================================================================\n");
    for(bPath = SV_VP_MAIN;bPath <= SV_VP_PIP; bPath++)
    {   
        vDrvPDSInOutStatusDump(bPath);
        vDrvRegenInOutStatusDump(bPath,REGEN_MODULE_PRE_REGEN);        
        vDrvDIInOutStatusDump(bPath);
        vDrvRegenInOutStatusDump(bPath,REGEN_MODULE_FSC_REGEN);        
        vScpipInOutStatusDump(bPath);
        
        LOG(0, "========================================================================\n");
    }    
}

BOOL fgVRMIsEventFlgSet(UINT8 bPath, UINT32 u4arg)
{
    return ((_u4VRMEventFlag[bPath] & u4arg) > 0) ? TRUE : FALSE;
}

UINT32 vVRMGetEventFlg(UINT8 bPath)
{
    if(bPath > SV_VP_PIP)
    {
        return 0;
    }

    return _u4VRMEventFlag[bPath];
}

VOID vVRMSetEventFlg(UINT8 bPath, UINT32 u4arg)
{
    CRIT_STATE_T csState;
    csState = x_crit_start();
    _u4VRMEventFlag[bPath] |= (u4arg);

    if(u4DrvTDTVIsDualFPI()&&(SV_VP_MAIN==bPath))
    {
        _u4VRMEventFlag[SV_VP_PIP] |= (u4arg);
    }
    
    x_crit_end(csState);
    return;
}

VOID vVRMClrEventFlg(UINT8 bPath)
{
    CRIT_STATE_T csState;
    csState = x_crit_start();
    _u4VRMEventFlag[bPath] = 0;
    x_crit_end(csState);
    return;
}

BOOL fgVRMIsEqual(VRM_INFO_T* vrmInfo1, VRM_INFO_T* vrmInfo2)
{
    BOOL ret = SV_TRUE;
    if(vrmInfo1->u2X         != vrmInfo2->u2X ||
       vrmInfo1->u2Y         != vrmInfo2->u2Y ||
       vrmInfo1->u2Width     != vrmInfo2->u2Width ||
       vrmInfo1->u2Height    != vrmInfo2->u2Height ||
       vrmInfo1->u2HTotal    != vrmInfo2->u2HTotal ||
       vrmInfo1->u2VTotal    != vrmInfo2->u2VTotal ||
       vrmInfo1->u2FrameRate != vrmInfo2->u2FrameRate ||
       vrmInfo1->u2ScanMode  != vrmInfo2->u2ScanMode
      )
    {
        ret = SV_FALSE;
    }

    return ret;
}

//Update Resolution of OMUX input
void vVRMUpdateOMUXResolution(UINT8 bPath)
{
    RPicInfo* picInfo;
    VRM_INFO_T* preVrmInfo;
    VRM_INFO_T curVrmInfo;

    if(bPath > SV_VP_PIP)
    {
        return;
    }
    
    preVrmInfo = VRMGetVRMInfo(bPath, VRM_MODULE_OMUX);
    picInfo = getPicInfo(bPath);

    curVrmInfo.u2VRMID = VRM_MODULE_OMUX;
    curVrmInfo.u2X = 0;
    curVrmInfo.u2Y = 0;

    curVrmInfo.u2Width = picInfo->wSrcW;
    curVrmInfo.u2Height = picInfo->wSrcH;    
    
    curVrmInfo.u2HTotal = picInfo->wHTotal;
    curVrmInfo.u2VTotal = picInfo->wVTotal;
    curVrmInfo.u2FrameRate = bDrvVideoGetRefreshRate(bPath);
    curVrmInfo.u2ScanMode  = bDrvVideoIsSrcInterlace(bPath); // 1: interlaced, 0: progressive

    if(fgVRMIsEqual(&curVrmInfo, preVrmInfo) == SV_FALSE)
    {
        x_memcpy((void*)preVrmInfo, (void*)(&curVrmInfo), sizeof(VRM_INFO_T));
    }
}

/** 
 *  Update Resolution of CB input.
 *  CB input comes from OMUX output.
 *  Currently OMUX will not change resolution.
 */ 
void vVRMUpdateCBResolution(UINT8 bPath)
{
    VRM_INFO_T* preVrmInfo;
    VRM_INFO_T curVrmInfo;
	E_TD_IN eDQCIn= TD_DQC_IN(u4DrvTDTVDQCModeQuery());

    if(bPath > SV_VP_PIP)
    {
        return;
    }
    
    //1. get previous CB info
    preVrmInfo = VRMGetVRMInfo(bPath, VRM_MODULE_CB);
    //2. calculate new CB resolution
    x_memcpy((void*)&curVrmInfo, (void*)VRMGetVRMInfo(bPath, VRM_MODULE_OMUX), sizeof(VRM_INFO_T));
    curVrmInfo.u2VRMID = VRM_MODULE_CB;
	if((E_TD_IN_CB_I == eDQCIn) || (E_TD_IN_CB_P == eDQCIn) || (E_TD_IN_SS_I == eDQCIn) || (E_TD_IN_SS_P == eDQCIn))
	{
        curVrmInfo.u2Width = (curVrmInfo.u2Width>>2)<<2;
	}
    //3. if different from previous one,update
    if(fgVRMIsEqual(preVrmInfo, &curVrmInfo) == SV_FALSE)
    {
        x_memcpy((void*)preVrmInfo, (void*)(&curVrmInfo), sizeof(VRM_INFO_T));
    }
}

/**
 *  Update Resolution of DQ input.
 *  DQ input comes from CB output.
 *  Currently, CB will only turn CB to SBS.
 *  No resolution change happened.
 */
void vVRMUpdateDQResolution(UINT8 bPath)
{
    VRM_INFO_T* preVrmInfo;
    VRM_INFO_T curVrmInfo;

    if(bPath > SV_VP_PIP)
    {
        return;
    }
    
    //1. get previous CB info
    preVrmInfo = VRMGetVRMInfo(bPath, VRM_MODULE_DQ);
    //2. calculate new CB resolution
    x_memcpy((void*)&curVrmInfo, (void*)VRMGetVRMInfo(bPath, VRM_MODULE_CB), sizeof(VRM_INFO_T));
    curVrmInfo.u2VRMID = VRM_MODULE_DQ;
    //3. if different from previous one,update
    if(fgVRMIsEqual(preVrmInfo, &curVrmInfo) == SV_FALSE)
    {
        x_memcpy((void*)preVrmInfo, (void*)(&curVrmInfo), sizeof(VRM_INFO_T));
    }
}

/**
 *  Update TTT input Resolution
 *  TTT input comes from DQ output.
 *  Currently, DQ will not change output resolution.
 */ 
void vVRMUpdateTTTResoluton(UINT8 bPath)
{
    VRM_INFO_T* preVrmInfo;
    VRM_INFO_T curVrmInfo;

    if(bPath > SV_VP_PIP)
    {
        return;
    }
    
    //1. get previous CB info
    preVrmInfo = VRMGetVRMInfo(bPath, VRM_MODULE_TTT);
    //2. calculate new CB resolution.(currently just have a copy from DQ).
    x_memcpy((void*)&curVrmInfo, (void*)VRMGetVRMInfo(bPath, VRM_MODULE_DQ), sizeof(VRM_INFO_T));
    curVrmInfo.u2VRMID = VRM_MODULE_TTT;
    //3. if different from previous one,update
    if(fgVRMIsEqual(preVrmInfo, &curVrmInfo) == SV_FALSE)
    {
        x_memcpy((void*)preVrmInfo, (void*)(&curVrmInfo), sizeof(VRM_INFO_T));
    }
}

/**
 *  Update PREDOWN input Resolution
 *  PREDOWN input comes from TTT output.
 *  When FS -> 2D => Frame rate become 1/2
 *  When SBS -> 2D => Width become 1/2
 *  When TAB -> 2D => Height become 1/2
 */
void vVRMUpdatePredownResolution(UINT8 bPath)
{
    VRM_INFO_T* tttIn;
    VRM_INFO_T* preVrmInfo;
    VRM_INFO_T curVrmInfo;
    UINT32 u4TTTMode;
    E_TD_IN eTTDIn;
    E_TD_OUT eTTDOut;

    if(bPath > SV_VP_PIP)
    {
        return;
    }
    
    tttIn = VRMGetVRMInfo(bPath, VRM_MODULE_TTT);
    preVrmInfo = VRMGetVRMInfo(bPath, VRM_MODULE_PDS);
    x_memcpy((void*)&curVrmInfo, (void*)tttIn, sizeof(VRM_INFO_T));
    curVrmInfo.u2VRMID = VRM_MODULE_PDS;
    
    u4TTTMode = u4DrvTDTVTTTModeQuery(bPath);
    eTTDIn=TD_TTT_IN(u4TTTMode);
    eTTDOut=TD_TTT_OUT(u4TTTMode);
    eTTDIn = (E_TD_IN)((UINT32)eTTDIn & ~0x1);  //make I/P to same value

    if(eTTDOut==E_TD_OUT_3D_2_2D)
    {    
        switch(eTTDIn)
        {        
            //FS in, 2D out, FR/2
            case E_TD_IN_FS_I:
                curVrmInfo.u2FrameRate = curVrmInfo.u2FrameRate / 2;
                curVrmInfo.u2VTotal    = curVrmInfo.u2VTotal    * 2;
                break;
            //SBS in, 2D out, width/2
            case E_TD_IN_SBS_I:
            case E_TD_IN_SS_I:
            case E_TD_IN_RD_I:
                curVrmInfo.u2Width = curVrmInfo.u2Width / 2;
                break;            
            //TAB or LI in, 2D out, height/2
            case E_TD_IN_TB_I:
            case E_TD_IN_LI_I:
                curVrmInfo.u2Height = curVrmInfo.u2Height / 2;
                break;
            default:
                break;
        }		
    }
   
    if(fgVRMIsEqual(preVrmInfo, &curVrmInfo) == SV_FALSE)
    {
        x_memcpy((void*)preVrmInfo, (void*)(&curVrmInfo), sizeof(VRM_INFO_T));
    }
}

/**
 *  Update REGEN input Resolution
 *  REGEN input comes from PREDOWN output.
 */
void vVRMUpdateRegenResolution(UINT8 bPath)
{
    VRM_INFO_T* predownIn;
    VRM_INFO_T* preVrmInfo;
    VRM_INFO_T curVrmInfo;
#if !defined(CC_MT5399)
    TDTV_3D_CROP_INTO_T ePhotoCrop;
#endif
    static E_TD_IN ePreMIBIn = E_TD_IN_END;
    
    UINT32 u4MIB     = u4DrvTDTVDIModeQuery();
    E_TD_IN eMIBIn   = TD_MIB_IN(u4MIB);
    E_TD_OUT eMIBOUT = TD_MIB_OUT(u4MIB);

    UINT32 u4Scl     = u4DrvTDTVScalerModeQuery();
    E_TD_IN eSclIn   = TD_SCL_IN(u4Scl);
    E_TD_OUT eSclOut = TD_SCL_OUT(u4Scl);
    
    UINT32 pdsWidth;
    UINT32 pdsHeight;

    if(bPath > SV_VP_PIP)
    {
        return;
    }

    if(PDS_WIDTH_FIX_UNKNOWN!=_u4PDSWidthFix[bPath])
    {
        preVrmInfo = VRMGetVRMInfo(bPath, VRM_MODULE_REGEN);
        x_memcpy((void*)&curVrmInfo, (void*)preVrmInfo, sizeof(VRM_INFO_T));    
        curVrmInfo.u2Width = _u4PDSWidthFix[bPath];
    }   
    else if(u4DrvTDTVIsDualFPI()&&(SV_VP_PIP==bPath))
    {
        preVrmInfo = VRMGetVRMInfo(SV_VP_MAIN, VRM_MODULE_REGEN);
        x_memcpy((void*)&curVrmInfo, (void*)preVrmInfo, sizeof(VRM_INFO_T));
        preVrmInfo = VRMGetVRMInfo(SV_VP_PIP , VRM_MODULE_REGEN);            
    }
    else
    {
  
        predownIn = VRMGetVRMInfo(bPath, VRM_MODULE_PDS);
        preVrmInfo = VRMGetVRMInfo(bPath, VRM_MODULE_REGEN);
        x_memcpy((void*)&curVrmInfo, (void*)predownIn, sizeof(VRM_INFO_T));
        curVrmInfo.u2VRMID = VRM_MODULE_REGEN;
    
        if(curVrmInfo.u2Width == 0 || curVrmInfo.u2Height == 0)
        {
            return;
        }

        //1. get H predown width
        vPDSUpdateMddiFifo(bPath);
        pdsWidth = u4PDSUpdatePDSWidth(bPath); 
        pdsHeight = u4PDSUpdatePDSHeight(bPath);

        //Dual FPI Mode can NOT use horizontal down scale, it will break the pixel order
        if(!u4DrvTDTVIsDualFPI())
        {
            //h-blanking will not do predown
            curVrmInfo.u2HTotal = (curVrmInfo.u2HTotal - curVrmInfo.u2Width) + pdsWidth;
            curVrmInfo.u2Width = pdsWidth;
        }
        //v-blanking will not do predown
        curVrmInfo.u2VTotal = (curVrmInfo.u2VTotal - curVrmInfo.u2Height) + pdsHeight;
        curVrmInfo.u2Height = pdsHeight;

          if(SV_VP_MAIN==bPath)
        {
            #ifdef SCALER_WA2_PR_DOWN            
            //Scaler Can not support PR down, downscaling should be performed at predown
            if((E_TD_OUT_3D_LI==eMIBOUT)
                &&((E_TD_IN_TB_P!=eMIBIn)&&(E_TD_IN_TB_I!=eMIBIn) && (E_TD_IN_LI_P!=eMIBIn) && (E_TD_IN_LI_I!=eMIBIn)))
            {
                 curVrmInfo.u2Height = MIN(curVrmInfo.u2Height ,getDispInfo(bPath)->wHeight /2);
            }
               //when input is LA 3D format, and the V lenght > display height, then  need to pre_down.
            if((E_TD_IN_LI_P == eMIBIn )&& (E_TD_OUT_3D_LI==eMIBOUT))
            {
                //PR V must be down to even line,and display height should be even.
                curVrmInfo.u2Height = MIN(curVrmInfo.u2Height  ,getDispInfo(bPath)->wHeight);         
            }
  
            if(((E_TD_OUT_NATIVE==eMIBOUT)&&(E_TD_IN_LI_P==eMIBIn))
                ||((E_TD_OUT_3D_LI==eMIBOUT)&&((E_TD_IN_TB_P==eMIBIn)||(E_TD_IN_TB_I==eMIBIn))))
            {
                curVrmInfo.u2Height = MIN(curVrmInfo.u2Height , getDispInfo(bPath)->wHeight);
            }

            if( (eSclIn == E_TD_IN_SBS_I || eSclIn == E_TD_IN_SBS_P ||
                     eSclIn == E_TD_IN_FS_I || eSclIn == E_TD_IN_FS_P) 
                 &&(eSclOut == E_TD_OUT_3D_LI))
            {
                curVrmInfo.u2Height = MIN(curVrmInfo.u2Height , getDispInfo(bPath)->wHeight /2);
            }
            #endif
            
            //for scaler SBS to 3D, width have to be 4 align
            if((eSclIn == E_TD_IN_SBS_P || eSclIn == E_TD_IN_SBS_I) 
                &&(eSclOut != E_TD_OUT_NATIVE))
            {
                curVrmInfo.u2Width -= curVrmInfo.u2Width%4;
            }
            
            //for scaler TAB to PR, height have to be even
            if((eSclIn == E_TD_IN_TB_P || eSclIn == E_TD_IN_TB_I) &&
                (eSclOut == E_TD_OUT_3D_LI))
            {
                //make height to be even
                curVrmInfo.u2Height -= curVrmInfo.u2Height&0x1;
            }
            
#if !defined(CC_MT5399)
            if(u4DrvTDTVIs3DPhoto(&ePhotoCrop)&&ePhotoCrop.fgIsFullResolution)
            {
                if((E_TD_IN_TB_P==eMIBIn)&&(E_TD_OUT_3D_LI==eMIBOUT))
                {
                    curVrmInfo.u2Height /=2;
                }           
            }
#endif

            //MIB 420
            if (bGetVideoDecType(bPath) == SV_VD_MPEGSD || bGetVideoDecType(bPath) == SV_VD_MPEGHD)
            {
                if (bDrvVideoIsSrcInterlace(bPath) == SV_TRUE)
                {
                    if((eMIBIn == E_TD_IN_TB_I) && (E_TD_OUT_NATIVE != eMIBOUT))
                    {
                        curVrmInfo.u2Height = curVrmInfo.u2Height& ~0x7; // Make it algin to 8
                    }
                    else
                    {
                        curVrmInfo.u2Height = curVrmInfo.u2Height&~0x3; // Make it algin to 4
                    }
                }
                else
                {                 
                    if((eMIBIn == E_TD_IN_TB_P) && (E_TD_OUT_NATIVE != eMIBOUT))
                    {
                        curVrmInfo.u2Height = curVrmInfo.u2Height&~0x3; // Make it algin to 4
                    }
                    else
                    {
                        curVrmInfo.u2Height = curVrmInfo.u2Height & ~0x1; // Make it algin to 2
                    }
                }
            }
			//TTD v pre-down 
			if(!bDrvVideoIsSrcInterlace(bPath)
                &&!u4DrvTDTVIs3DPhoto(NULL))
			{
				if((E_TD_IN_2D_P == TD_TTD_IN(u4DrvTDTVTTDModeQuery()))
					&& (E_TD_OUT_3D_LI == TD_TTD_OUT(u4DrvTDTVTTDModeQuery())))
				{
					curVrmInfo.u2Height= MIN(curVrmInfo.u2Height , getDispInfo(bPath)->wHeight /2);
				}
			}
        }
        else    //for sub
        {
            if (bGetVideoDecType(bPath) == SV_VD_MPEGSD || bGetVideoDecType(bPath) == SV_VD_MPEGHD)
            {
                if (bDrvVideoIsSrcInterlace(bPath) == SV_TRUE)
                {
                    curVrmInfo.u2Height = curVrmInfo.u2Height&~0x3; // Make it algin to 4
                }
                else
                {                 
                    curVrmInfo.u2Height = curVrmInfo.u2Height & ~0x1; // Make it algin to 2
                }
            }        
        }
    }

    if(eVrmGetOvsnModule(bPath) == OVSN_MODULE_FSC_REGEN)
    {
        if((!bIsScalerInput444(bPath))  )  //422 case
        {
            curVrmInfo.u2Width &= ~0x1;  //make it even
        }

        if(((E_TD_IN_SBS_I==eMIBIn)||(E_TD_IN_SBS_P==eMIBIn))
            &&(E_TD_OUT_NATIVE!=eMIBOUT))
        {
            curVrmInfo.u2Width &= ~0x3; // Make it algin to 4
        }
      
        if((bDrvVideoIsSrcInterlace(bPath) == SV_TRUE) 
            ||(eMIBIn == E_TD_IN_TB_P)
            ||(eMIBIn == E_TD_IN_LI_P)
            )
        {
            curVrmInfo.u2Height &= ~0x1;  //make it even
        }

        if(eMIBIn == E_TD_IN_TB_I)
        {
            curVrmInfo.u2Height &= ~0x3; // Make it algin to 4
        }
        if((bDrvVideoIsSrcInterlace(bPath) == SV_TRUE) 
            && (eMIBIn == E_TD_IN_LI_I) )
        {
            curVrmInfo.u2Height &= ~0x3;  //make it algin to 4
        }
    }

    if((fgVRMIsEqual(preVrmInfo, &curVrmInfo) == SV_FALSE)
        || (ePreMIBIn != eMIBIn))
    {
        ePreMIBIn = eMIBIn;
        x_memcpy((void*)preVrmInfo, (void*)(&curVrmInfo), sizeof(VRM_INFO_T));
    }
}

void vVRMUpdateMIBResolution(UINT8 bPath)
{
    if(eVrmGetOvsnModule(bPath) == OVSN_MODULE_REGEN)
    {
        vVRMUpdateMIBResolutionWithOvsn(bPath);    
    }
    else
    {
        vVRMUpdateMIBResolutionWithoutOvsn(bPath);
    }
    
}

void vVRMUpdateMIBResolutionWithoutOvsn(UINT8 bPath)
{
    VRM_INFO_T* mibIn;
    VRM_INFO_T* preVrmInfo;
    VRM_INFO_T curVrmInfo;
    #ifdef REGEN_BOTH_IN_PREREGEN_AND_FSCREGEN
    UINT32 u4HPDSFactor;
    #endif
    mibIn = VRMGetVRMInfo(bPath, VRM_MODULE_REGEN);
    preVrmInfo = VRMGetVRMInfo(bPath, VRM_MODULE_MIB);
    x_memcpy((void*)&curVrmInfo, (void*)mibIn, sizeof(VRM_INFO_T));
    curVrmInfo.u2VRMID = VRM_MODULE_MIB;

    if(u4DrvTDTVIsDualFPI())
    {
        if(SV_VP_MAIN==bPath)
        {
            curVrmInfo.u2Width=curVrmInfo.u2Width/4*2; //make it even
            curVrmInfo.u2X    =((mibIn->u2Width/4*2)-curVrmInfo.u2Width)*2;
        }
        else
        {

            preVrmInfo = VRMGetVRMInfo(SV_VP_MAIN, VRM_MODULE_MIB);
            x_memcpy((void*)&curVrmInfo, (void*)preVrmInfo, sizeof(VRM_INFO_T));
            curVrmInfo.u2X=0;
            preVrmInfo = VRMGetVRMInfo(SV_VP_PIP , VRM_MODULE_MIB);     
        }
    }
    #ifdef REGEN_BOTH_IN_PREREGEN_AND_FSCREGEN
	  if(bPath ==  SV_VP_MAIN &&
	  	(TD_TTT_IN(u4DrvTDTVTTTModeQuery(bPath)) == E_TD_IN_2D_I || 
		  TD_TTT_IN(u4DrvTDTVTTTModeQuery(bPath)) == E_TD_IN_2D_P ||
		  TD_TTT_OUT(u4DrvTDTVTTTModeQuery(bPath)) == E_TD_OUT_3D_2_2D)
		  )//for mheg5 test
	  {
		  _vPDSUpdateDispPrm(bPath);	
		  u4HPDSFactor = u4PDSGetHPDSFactor(bPath);
		  if((getMixedHPorch(bPath) >0) && (SV_FALSE==u4DrvTDTVIs3DPhoto(NULL)) &&(curVrmInfo.u2Width > 0))
		  {
			  if(DIVIDE_WITH_ROUND((getMixedHPorch(bPath) * u4HPDSFactor) , DFT_HPD_FACTOR) %2)//when x is odd, overscan 1 piexel in pre-regen,otherwise,overscan 2 pixel in pre-regen
			  {
				  curVrmInfo.u2X +=1;
				  _fgOvsn1PixelInPreregen = SV_TRUE;
			  }
			  else
			  {
				  curVrmInfo.u2X +=2;
				  _fgOvsn2PixelInPreregen= SV_TRUE; 
			  }
			  curVrmInfo.u2Width-=2;
		  }
	  }
      #endif	
    if(fgVRMIsEqual(preVrmInfo, &curVrmInfo) == SV_FALSE)
    {
        x_memcpy((void*)preVrmInfo, (void*)(&curVrmInfo), sizeof(VRM_INFO_T));
        vVRMFireMIBResolutionChange(bPath);        
    }    
}

/**
 *  Update MIB input Resolution
 *  MIB input comes from REGEN output.
 */
void vVRMUpdateMIBResolutionWithOvsn(UINT8 bPath)
{
    VRM_INFO_T* pdsIn;
    VRM_INFO_T* mibIn;
    VRM_INFO_T* preVrmInfo;
    VRM_INFO_T curVrmInfo;
    UINT32 u4HPDSFactor;
    UINT32 u4VPDSFactor;
    RPicInfo* picInfo;
    TDTV_3D_CROP_INTO_T ePhotoCrop;
    UINT32 u4MIB     = u4DrvTDTVDIModeQuery();
    E_TD_IN eMIBIn   = TD_MIB_IN(u4MIB);
    E_TD_OUT eMIBOUT = TD_MIB_OUT(u4MIB);
    UINT32 u4TTTMode;
    E_TD_IN eTTTIn;
    E_TD_OUT eTTTOut;

    u4TTTMode = u4DrvTDTVTTTModeQuery(bPath);
    eTTTIn = TD_TTT_IN(u4TTTMode);
    eTTTOut = TD_TTT_OUT(u4TTTMode);

    if(bPath > SV_VP_PIP)
    {
        return;
    }

    /* Since predown in/out is determined and 
     * "vVRMUpdateMIBResolution" need predown factor for calculation,
     * call predown calculation here first.
     */        

    picInfo = getPicInfo(bPath);
    pdsIn = VRMGetVRMInfo(bPath, VRM_MODULE_PDS);
    mibIn = VRMGetVRMInfo(bPath, VRM_MODULE_REGEN);

    u4HPDSFactor = (mibIn->u2Width<pdsIn->u2Width) ?  mibIn->u2Width *DFT_PDS_FACTOR / pdsIn->u2Width:
        DFT_PDS_FACTOR;
    u4VPDSFactor =(mibIn->u2Height<pdsIn->u2Height) ? mibIn->u2Height *DFT_PDS_FACTOR / pdsIn->u2Height:
        DFT_PDS_FACTOR;
    
    preVrmInfo = VRMGetVRMInfo(bPath, VRM_MODULE_MIB);
    x_memcpy((void*)&curVrmInfo, (void*)mibIn, sizeof(VRM_INFO_T));
    curVrmInfo.u2VRMID = VRM_MODULE_MIB;

    if((SV_VP_MAIN==bPath)&&u4DrvTDTVIs3DPhoto(&ePhotoCrop))
    {
        if(eTTTOut == E_TD_OUT_3D_2_2D)
        {
        	if(eTTTIn == E_TD_IN_SBS_I || eTTTIn == E_TD_IN_SBS_P)
        	{
        	    if(E_TDTV_UI_3D_2_2D_L==eDrvTDTVGet3D22D(SV_VP_MAIN))
                {   
            		ePhotoCrop.u4CropRightOffset = 0;
                }
                else
                {
            		ePhotoCrop.u4CropLeftOffset = 0;            
                }
        	}

            if(eTTTIn == E_TD_IN_TB_I || eTTTIn == E_TD_IN_TB_P)
        	{
        	    if(E_TDTV_UI_3D_2_2D_L==eDrvTDTVGet3D22D(SV_VP_MAIN))
                {
            		ePhotoCrop.u4CropBottomOffset = 0;           
                }
                else
                {
            		ePhotoCrop.u4CropTopOffset = 0;            
                }              
        	}
        }
        
	    curVrmInfo.u2X = DIVIDE_WITH_ROUND((ePhotoCrop.u4CropLeftOffset * u4HPDSFactor) , DFT_HPD_FACTOR);
	    curVrmInfo.u2Y = DIVIDE_WITH_ROUND((ePhotoCrop.u4CropTopOffset  * u4VPDSFactor) , DFT_HPD_FACTOR);

	    curVrmInfo.u2Width = curVrmInfo.u2Width -
	        DIVIDE_WITH_ROUND(((ePhotoCrop.u4CropLeftOffset + ePhotoCrop.u4CropRightOffset)* u4HPDSFactor) , DFT_HPD_FACTOR);    

	    curVrmInfo.u2Height = curVrmInfo.u2Height -
	        DIVIDE_WITH_ROUND(((ePhotoCrop.u4CropTopOffset + ePhotoCrop.u4CropBottomOffset)* u4VPDSFactor) , DFT_HPD_FACTOR);
    }	
	else if(eTTTOut == E_TD_OUT_3D_2_2D)
	{	   
        switch(eTTTIn)
        {        
            //SBS 
            case E_TD_IN_SBS_I:
			case E_TD_IN_SBS_P:
			case E_TD_IN_RD_I:
			case E_TD_IN_RD_P:
				curVrmInfo.u2X = DIVIDE_WITH_ROUND((getMixedHPorch(bPath) * u4HPDSFactor) , DFT_HPD_FACTOR)/2;
				curVrmInfo.u2Y = DIVIDE_WITH_ROUND((getMixedVPorch(bPath) * u4VPDSFactor) , DFT_HPD_FACTOR);
				
				curVrmInfo.u2Width = curVrmInfo.u2Width - curVrmInfo.u2X -
					DIVIDE_WITH_ROUND(((picInfo->wSrcW - picInfo->wTargetW	- getMixedHPorch(bPath)) * u4HPDSFactor)/2 , DFT_HPD_FACTOR);    
				
				curVrmInfo.u2Height = curVrmInfo.u2Height - curVrmInfo.u2Y - 
					DIVIDE_WITH_ROUND(((picInfo->wSrcH	- picInfo->wTargetH  - getMixedVPorch(bPath))* u4VPDSFactor) , DFT_HPD_FACTOR); 	   
                break;            
            //TAB,LI
            case E_TD_IN_TB_I:
            case E_TD_IN_TB_P:
            case E_TD_IN_LI_I:
            case E_TD_IN_LI_P:
                
				curVrmInfo.u2X = DIVIDE_WITH_ROUND((getMixedHPorch(bPath) * u4HPDSFactor) , DFT_HPD_FACTOR);
				curVrmInfo.u2Y = DIVIDE_WITH_ROUND((getMixedVPorch(bPath) * u4VPDSFactor) , DFT_HPD_FACTOR)/2;
				
				curVrmInfo.u2Width = curVrmInfo.u2Width - curVrmInfo.u2X -
					DIVIDE_WITH_ROUND(((picInfo->wSrcW - picInfo->wTargetW	- getMixedHPorch(bPath)) * u4HPDSFactor) , DFT_HPD_FACTOR);    
				
				curVrmInfo.u2Height = curVrmInfo.u2Height - curVrmInfo.u2Y - 
					DIVIDE_WITH_ROUND(((picInfo->wSrcH	- picInfo->wTargetH  - getMixedVPorch(bPath))* u4VPDSFactor)/2 , DFT_HPD_FACTOR); 	   
                break;
            //FS
            default:
                curVrmInfo.u2X = DIVIDE_WITH_ROUND((getMixedHPorch(bPath) * u4HPDSFactor) , DFT_HPD_FACTOR);
                curVrmInfo.u2Y = DIVIDE_WITH_ROUND((getMixedVPorch(bPath) * u4VPDSFactor) , DFT_HPD_FACTOR);

                curVrmInfo.u2Width = curVrmInfo.u2Width - curVrmInfo.u2X -
                    DIVIDE_WITH_ROUND(((picInfo->wSrcW - picInfo->wTargetW  - getMixedHPorch(bPath)) * u4HPDSFactor) , DFT_HPD_FACTOR);   
                   
                curVrmInfo.u2Height = curVrmInfo.u2Height - curVrmInfo.u2Y - 
                    DIVIDE_WITH_ROUND(((picInfo->wSrcH  - picInfo->wTargetH  - getMixedVPorch(bPath))* u4VPDSFactor) , DFT_HPD_FACTOR);  
                break;      	
       }
	}
    else
    {
        curVrmInfo.u2X = DIVIDE_WITH_ROUND((getMixedHPorch(bPath) * u4HPDSFactor) , DFT_HPD_FACTOR);
        curVrmInfo.u2Y = DIVIDE_WITH_ROUND((getMixedVPorch(bPath) * u4VPDSFactor) , DFT_HPD_FACTOR);

        curVrmInfo.u2Width = curVrmInfo.u2Width - curVrmInfo.u2X -
            DIVIDE_WITH_ROUND(((picInfo->wSrcW - picInfo->wTargetW  - getMixedHPorch(bPath)) * u4HPDSFactor) , DFT_HPD_FACTOR);    

        curVrmInfo.u2Height = curVrmInfo.u2Height - curVrmInfo.u2Y - 
            DIVIDE_WITH_ROUND(((picInfo->wSrcH  - picInfo->wTargetH  - getMixedVPorch(bPath))* u4VPDSFactor) , DFT_HPD_FACTOR);        
    }
    
    if((!bIsScalerInput444(bPath))  )  //422 case
    {
        curVrmInfo.u2Width &= ~0x1;  //make it even
    }

    if(((E_TD_IN_SBS_I==eMIBIn)||(E_TD_IN_SBS_P==eMIBIn))
        &&(E_TD_OUT_NATIVE!=eMIBOUT))
    {
        curVrmInfo.u2Width &= ~0x3; // Make it algin to 4
    }
  
    if((bDrvVideoIsSrcInterlace(bPath) == SV_TRUE) 
        ||(eMIBIn == E_TD_IN_TB_P)
        ||(eMIBIn == E_TD_IN_LI_P)
        )
    {
        curVrmInfo.u2Height &= ~0x1;  //make it even
    }

    if(eMIBIn == E_TD_IN_TB_I)
    {
        curVrmInfo.u2Height &= ~0x3; // Make it algin to 4
    }
    if((bDrvVideoIsSrcInterlace(bPath) == SV_TRUE) 
        && (eMIBIn == E_TD_IN_LI_I) )
    {
        curVrmInfo.u2Height &= ~0x3;  //make it algin to 4
    }
    //MIB 420
    if (bGetVideoDecType(bPath) == SV_VD_MPEGSD || bGetVideoDecType(bPath) == SV_VD_MPEGHD ||IS_FORCE420MODE(bPath))
    {
        if (bDrvVideoIsSrcInterlace(bPath) == SV_TRUE)
        {
            if((eMIBIn == E_TD_IN_TB_I) && (E_TD_OUT_NATIVE != eMIBOUT)) //make it  align to 8
            {
                if((mibIn->u2Height <= ALIGN_8_CEIL(curVrmInfo.u2Height))
                    || ((curVrmInfo.u2Height & 0x7) < 4)) //down
                {
                    curVrmInfo.u2Y += (curVrmInfo.u2Height & 0x7)/2;
                    curVrmInfo.u2Height &= ~0x7; 
                }
                else //up
                {
                    if(curVrmInfo.u2Y < (ALIGN_8_CEIL(curVrmInfo.u2Height)-curVrmInfo.u2Height)/2)
                    {
                        curVrmInfo.u2Y = 0;
                    }
                    else
                    {
                        curVrmInfo.u2Y -= (ALIGN_8_CEIL(curVrmInfo.u2Height)-curVrmInfo.u2Height)/2;
                    }
                    curVrmInfo.u2Height = ALIGN_8_CEIL(curVrmInfo.u2Height);
                }
            }
		    else // make it align to 4
            {
                if((mibIn->u2Height <= ALIGN_4_CEIL(curVrmInfo.u2Height))
                    ||((curVrmInfo.u2Height & 0x3) < 2)) //down
                {
                    curVrmInfo.u2Y += (curVrmInfo.u2Height & 0x3)/2;
                    curVrmInfo.u2Height &= ~0x3; 
                }
                else //up
                {
                    if(curVrmInfo.u2Y < (ALIGN_4_CEIL(curVrmInfo.u2Height)-curVrmInfo.u2Height)/2)
                    {
                        curVrmInfo.u2Y = 0;
                    }
                    else
                    {
                        curVrmInfo.u2Y -= (ALIGN_4_CEIL(curVrmInfo.u2Height)-curVrmInfo.u2Height)/2;
                    }
                    curVrmInfo.u2Height = ALIGN_4_CEIL(curVrmInfo.u2Height);
                }
            }
        }
        else
        {
            if((eMIBIn == E_TD_IN_TB_P) && (E_TD_OUT_NATIVE != eMIBOUT)) // make it align to 4
            {
                if((mibIn->u2Height <= ALIGN_4_CEIL(curVrmInfo.u2Height))
                    ||((curVrmInfo.u2Height & 0x3) < 2)) //down
                {
                    curVrmInfo.u2Y += (curVrmInfo.u2Height & 0x3)/2;
                    curVrmInfo.u2Height &= ~0x3; 
                }
                else //up
                {
                    if(curVrmInfo.u2Y < (ALIGN_4_CEIL(curVrmInfo.u2Height)-curVrmInfo.u2Height)/2)
                    {
                        curVrmInfo.u2Y = 0;
                    }
                    else
                    {
                        curVrmInfo.u2Y -= (ALIGN_4_CEIL(curVrmInfo.u2Height)-curVrmInfo.u2Height)/2;
                    }
                    curVrmInfo.u2Height = ALIGN_4_CEIL(curVrmInfo.u2Height);
                }
            }
		    else // make it align to 2
            {
                if(mibIn->u2Height <= ALIGN_2_CEIL(curVrmInfo.u2Height)) //down
                {
                    curVrmInfo.u2Height &= ~0x1; 
                }
                else //up
                {
                    curVrmInfo.u2Height = ALIGN_2_CEIL(curVrmInfo.u2Height);
                }
            }
        }
    }

    if((eMIBIn == E_TD_IN_TB_I)||(eMIBIn == E_TD_IN_TB_P))
    {
        curVrmInfo.u2Y = curVrmInfo.u2Y + (curVrmInfo.u2Y & 0x1);         //make it even
    }

    if(eMIBIn == E_TD_IN_LI_P)
    {
        curVrmInfo.u2Y = curVrmInfo.u2Y + (curVrmInfo.u2Y & 0x1);         //make it even
    }
    
    if (bDrvVideoIsSrcInterlace(bPath) == SV_TRUE)
    {
        curVrmInfo.u2Y = curVrmInfo.u2Y + (curVrmInfo.u2Y & 0x1); //make it even,Rounding 
    }
    
    if(u4DrvTDTVIsDualFPI())
    {
        if(SV_VP_MAIN==bPath)
        {
            curVrmInfo.u2Width=curVrmInfo.u2Width/4*2; //make it even
            curVrmInfo.u2X    =((mibIn->u2Width/4*2)-curVrmInfo.u2Width)*2;
        }
        else
        {
            preVrmInfo = VRMGetVRMInfo(SV_VP_MAIN, VRM_MODULE_MIB);
            x_memcpy((void*)&curVrmInfo, (void*)preVrmInfo, sizeof(VRM_INFO_T));
            curVrmInfo.u2X=0;
            preVrmInfo = VRMGetVRMInfo(SV_VP_PIP , VRM_MODULE_MIB);            
        }
    }


    
    //for mib input sbs, output 3d, mib input width need to 32 align
    if((E_TD_IN_SBS_P==eMIBIn)
            &&(E_TD_OUT_NATIVE!=eMIBOUT)
            &&(SV_VP_MAIN == bPath))
    {
        if(PDS_WIDTH_FIX_UNKNOWN==_u4PDSWidthFix[bPath])
        {
            UINT32 u4Factor;
            UINT32 u4Remain;
            
            u4Factor = ((curVrmInfo.u2Width&(~0x1F)) * 0x8000 + curVrmInfo.u2Width -1)
                / curVrmInfo.u2Width;
            
            u4Remain = mibIn->u2Width - curVrmInfo.u2X- curVrmInfo.u2Width;
            
            _u4PDSWidthFix[bPath] = (curVrmInfo.u2Width & ~(0x1F))
                + (curVrmInfo.u2X * u4Factor + 0x7FFF) / 0x8000
                + (u4Remain * u4Factor + 0x7FFF) / 0x8000;
                
            vVRMSetEventFlg(bPath,VRM_EVENT_BY_RECAL);            
        }

        curVrmInfo.u2X += (curVrmInfo.u2Width & 0x1F)/2;
        curVrmInfo.u2Width &= ~(0x1F);
    }

    //Final protection
    if((curVrmInfo.u2X > (mibIn->u2Width - curVrmInfo.u2Width))&&(mibIn->u2Width > curVrmInfo.u2Width))
    {
        curVrmInfo.u2X = mibIn->u2Width - curVrmInfo.u2Width;
    }
    if((curVrmInfo.u2Y > (mibIn->u2Height - curVrmInfo.u2Height))&&(mibIn->u2Height > curVrmInfo.u2Height))
    {
        curVrmInfo.u2Y = mibIn->u2Height - curVrmInfo.u2Height;
    }

    #ifdef CC_FLIP_MIRROR_SUPPORT 
    if(u4GetFlipMirrorModule(bPath)==FLIP_BY_B2R)
    {
        if ((u1GetFlipMirrorConfig() & SYS_MIRROR_CONFIG_ON ) &&(!((u4DrvTDTVIs3DPhoto(&ePhotoCrop)) && (SV_VP_MAIN==bPath) && (eTTTOut == E_TD_OUT_3D_2_2D))))
        {
            if(mibIn->u2Width < curVrmInfo.u2Width + curVrmInfo.u2X)
            {
                curVrmInfo.u2X = 0;
            }
            else
            {
                 curVrmInfo.u2X = mibIn->u2Width - curVrmInfo.u2Width - curVrmInfo.u2X;
            }
        }    
        if ((u1GetFlipMirrorConfig() & SYS_FLIP_CONFIG_ON)&&(!((u4DrvTDTVIs3DPhoto(&ePhotoCrop)) && (SV_VP_MAIN==bPath) && (eTTTOut == E_TD_OUT_3D_2_2D))))
        {
            if(mibIn->u2Height < curVrmInfo.u2Height + curVrmInfo.u2Y)
            {
                curVrmInfo.u2Y = 0;
            }
            else
            {
                curVrmInfo.u2Y = mibIn->u2Height - curVrmInfo.u2Height - curVrmInfo.u2Y; 
            }
        }  
    }  
    #endif
  
    if(fgVRMIsEqual(preVrmInfo, &curVrmInfo) == SV_FALSE)
    {
        x_memcpy((void*)preVrmInfo, (void*)(&curVrmInfo), sizeof(VRM_INFO_T));
    }
}

void vVRMUpdateFSCRegenResolution(UINT8 bPath)
{
    VRM_INFO_T* fscRegenIn;
    VRM_INFO_T* preVrmInfo;
    VRM_INFO_T curVrmInfo;

    UINT32 u4MIB     = u4DrvTDTVDIModeQuery();
    E_TD_IN eMIBIn   = TD_MIB_IN(u4MIB);
    E_TD_OUT eMIBOUT = TD_MIB_OUT(u4MIB);

    if(bPath > SV_VP_PIP)
    {
        return;
    }

    fscRegenIn = VRMGetVRMInfo(bPath, VRM_MODULE_MIB);    
    preVrmInfo = VRMGetVRMInfo(bPath, VRM_MODULE_FSC_REGEN);
    x_memcpy((void*)&curVrmInfo, (void*)fscRegenIn, sizeof(VRM_INFO_T));
    curVrmInfo.u2VRMID = VRM_MODULE_FSC_REGEN;
    curVrmInfo.u2X = 0;
    curVrmInfo.u2Y = 0;

    if((SV_VP_MAIN==bPath)&&(E_TD_OUT_NATIVE!=eMIBOUT))
    {
        if((E_TD_IN_TB_I==eMIBIn)||(E_TD_IN_TB_P==eMIBIn)
            ||(E_TD_IN_LI_I==eMIBIn)||(E_TD_IN_LI_P==eMIBIn))
        {
            curVrmInfo.u2Height/=2;
            curVrmInfo.u2VTotal/=2;            
        }
        else if((E_TD_IN_SBS_I==eMIBIn)||(E_TD_IN_SBS_P==eMIBIn))
        {
            curVrmInfo.u2Width/=2;
            curVrmInfo.u2HTotal/=2;
        }
        else if((E_TD_IN_FS_I==eMIBIn)||(E_TD_IN_FS_P==eMIBIn))
        {
            curVrmInfo.u2FrameRate/=2;
        }

        if((E_TD_OUT_3D_LI==eMIBOUT)||(E_TD_OUT_3D_TB==eMIBOUT))
        {
            curVrmInfo.u2Height*=2;
            curVrmInfo.u2VTotal*=2;                        
        }        
        else if(E_TD_OUT_3D_FS==eMIBOUT)
        {
            curVrmInfo.u2FrameRate*=2;
        }        
    }
    
    if(u4DrvTDTVIsDualFPI())
    {
        curVrmInfo.u2Width*=2;
        curVrmInfo.u2HTotal*=2;        
    }

    if(curVrmInfo.u2ScanMode) // interlaced input, de-interlaced at MIB
    {     
        if (bDrvDIIsScalerBob(bPath) || u1DrvDIIsForceBypass(bPath))
        {
            curVrmInfo.u2Height /= 2;
        }
        else
        {
            curVrmInfo.u2ScanMode = 0;
        }
    }
    
    if(fgVRMIsEqual(preVrmInfo, &curVrmInfo) == SV_FALSE)
    {
        x_memcpy((void*)preVrmInfo, (void*)(&curVrmInfo), sizeof(VRM_INFO_T));
    }
}
void vVRMUpdateFSCResolutionWithOvsn(UINT8 bPath)
{
    VRM_INFO_T* fscIn;
    VRM_INFO_T* preVrmInfo;
    VRM_INFO_T curVrmInfo;
    UINT32 u4HPDSFactor;
    UINT32 u4VPDSFactor;
    RPicInfo* picInfo;
    TDTV_3D_CROP_INTO_T ePhotoCrop;
    //UINT32 u4RegenFixWidth;
    //UINT32 u4MIB     = u4DrvTDTVDIModeQuery();
    //E_TD_IN eMIBIn   = TD_MIB_IN(u4MIB);
    //E_TD_OUT eMIBOUT = TD_MIB_OUT(u4MIB);
	UINT32 u4TTTMode;
    E_TD_IN eTTTIn;
    E_TD_OUT eTTTOut;

	#ifdef REGEN_BOTH_IN_PREREGEN_AND_FSCREGEN
	BOOL fgOvsn1PixelInFSCregen = SV_FALSE;
	#endif
    
	u4TTTMode = u4DrvTDTVTTTModeQuery(bPath);
    eTTTIn = TD_TTT_IN(u4TTTMode);
    eTTTOut = TD_TTT_OUT(u4TTTMode);
    //u4RegenFixWidth = 0;

    if(bPath > SV_VP_PIP)
    {
        return;
    }

    /* Since predown in/out is determined and 
     * "vVRMUpdateMIBResolution" need predown factor for calculation,
     * call predown calculation here first.
     */        
    _vPDSUpdateDispPrm(bPath);    
    u4HPDSFactor = u4PDSGetHPDSFactor(bPath);
    u4VPDSFactor = u4PDSGetVPDSFactor(bPath);
    picInfo = getPicInfo(bPath);
    fscIn = VRMGetVRMInfo(bPath, VRM_MODULE_FSC_REGEN);
    preVrmInfo = VRMGetVRMInfo(bPath, VRM_MODULE_FSC);
    x_memcpy((void*)&curVrmInfo, (void*)fscIn, sizeof(VRM_INFO_T));
    curVrmInfo.u2VRMID = VRM_MODULE_FSC;

    if((SV_VP_MAIN==bPath)&&u4DrvTDTVIs3DPhoto(&ePhotoCrop))
    {
        if(eTTTOut == E_TD_OUT_3D_2_2D)
        {
        	if(eTTTIn == E_TD_IN_SBS_I || eTTTIn == E_TD_IN_SBS_P)
        	{
        	    if(E_TDTV_UI_3D_2_2D_L==eDrvTDTVGet3D22D(SV_VP_MAIN))
                {   
            		ePhotoCrop.u4CropRightOffset = 0;
                }
                else
                {
            		ePhotoCrop.u4CropLeftOffset  = 0;                
                }
        	}

            if(eTTTIn == E_TD_IN_TB_I || eTTTIn == E_TD_IN_TB_P)
        	{
        	    if(E_TDTV_UI_3D_2_2D_L==eDrvTDTVGet3D22D(SV_VP_MAIN))
                {
            		ePhotoCrop.u4CropBottomOffset = 0;                
                }
                else
                {
            		ePhotoCrop.u4CropTopOffset = 0;                
                }           
        	}
        }
        
	    curVrmInfo.u2X = DIVIDE_WITH_ROUND((ePhotoCrop.u4CropLeftOffset * u4HPDSFactor) , DFT_HPD_FACTOR);
	    curVrmInfo.u2Y = DIVIDE_WITH_ROUND((ePhotoCrop.u4CropTopOffset  * u4VPDSFactor) , DFT_HPD_FACTOR);

	    curVrmInfo.u2Width = curVrmInfo.u2Width -
	        DIVIDE_WITH_ROUND(((ePhotoCrop.u4CropLeftOffset + ePhotoCrop.u4CropRightOffset)* u4HPDSFactor) , DFT_HPD_FACTOR);    

	    curVrmInfo.u2Height = curVrmInfo.u2Height -
	        DIVIDE_WITH_ROUND(((ePhotoCrop.u4CropTopOffset + ePhotoCrop.u4CropBottomOffset)* u4VPDSFactor) , DFT_HPD_FACTOR);
    }	
	else if(eTTTOut == E_TD_OUT_3D_2_2D)
	{	   
        switch(eTTTIn)
        {        
            //SBS 
            case E_TD_IN_SBS_I:
			case E_TD_IN_SBS_P:
			case E_TD_IN_RD_I:
			case E_TD_IN_RD_P:
				curVrmInfo.u2X = DIVIDE_WITH_ROUND((getMixedHPorch(bPath) * u4HPDSFactor) , DFT_HPD_FACTOR)/2;
				curVrmInfo.u2Y = DIVIDE_WITH_ROUND((getMixedVPorch(bPath) * u4VPDSFactor) , DFT_HPD_FACTOR);
				
				curVrmInfo.u2Width = curVrmInfo.u2Width - curVrmInfo.u2X -
					DIVIDE_WITH_ROUND(((picInfo->wSrcW - picInfo->wTargetW	- getMixedHPorch(bPath)) * u4HPDSFactor)/2 , DFT_HPD_FACTOR);    
				
				curVrmInfo.u2Height = curVrmInfo.u2Height - curVrmInfo.u2Y - 
					DIVIDE_WITH_ROUND(((picInfo->wSrcH	- picInfo->wTargetH  - getMixedVPorch(bPath))* u4VPDSFactor) , DFT_HPD_FACTOR); 	   
                break;            
            //TAB,LI
            case E_TD_IN_TB_I:
            case E_TD_IN_TB_P:
            case E_TD_IN_LI_I:
            case E_TD_IN_LI_P:
                
				curVrmInfo.u2X = DIVIDE_WITH_ROUND((getMixedHPorch(bPath) * u4HPDSFactor) , DFT_HPD_FACTOR);
				curVrmInfo.u2Y = DIVIDE_WITH_ROUND((getMixedVPorch(bPath) * u4VPDSFactor) , DFT_HPD_FACTOR)/2;
				
				curVrmInfo.u2Width = curVrmInfo.u2Width - curVrmInfo.u2X -
					DIVIDE_WITH_ROUND(((picInfo->wSrcW - picInfo->wTargetW	- getMixedHPorch(bPath)) * u4HPDSFactor) , DFT_HPD_FACTOR);    
				
				curVrmInfo.u2Height = curVrmInfo.u2Height - curVrmInfo.u2Y - 
					DIVIDE_WITH_ROUND(((picInfo->wSrcH	- picInfo->wTargetH  - getMixedVPorch(bPath))* u4VPDSFactor)/2 , DFT_HPD_FACTOR); 	   
                break;
            //FS
            default:
                curVrmInfo.u2X = DIVIDE_WITH_ROUND((getMixedHPorch(bPath) * u4HPDSFactor) , DFT_HPD_FACTOR);
                curVrmInfo.u2Y = DIVIDE_WITH_ROUND((getMixedVPorch(bPath) * u4VPDSFactor) , DFT_HPD_FACTOR);

                curVrmInfo.u2Width = curVrmInfo.u2Width - curVrmInfo.u2X -
                    DIVIDE_WITH_ROUND(((picInfo->wSrcW - picInfo->wTargetW  - getMixedHPorch(bPath)) * u4HPDSFactor) , DFT_HPD_FACTOR);   
                   
                curVrmInfo.u2Height = curVrmInfo.u2Height - curVrmInfo.u2Y - 
                    DIVIDE_WITH_ROUND(((picInfo->wSrcH  - picInfo->wTargetH  - getMixedVPorch(bPath))* u4VPDSFactor) , DFT_HPD_FACTOR);  
                break;      	
       }

		if((picInfo->wSrcW - picInfo->wTargetW) < getMixedHPorch(bPath))
		{
			curVrmInfo.u2Width = curVrmInfo.u2Width - curVrmInfo.u2X;
		}

		if((picInfo->wSrcH  - picInfo->wTargetH) < getMixedVPorch(bPath))
		{
			curVrmInfo.u2Height = curVrmInfo.u2Height - curVrmInfo.u2Y;
		}
	}
    else
    {
        curVrmInfo.u2X = DIVIDE_WITH_ROUND((getMixedHPorch(bPath) * u4HPDSFactor) , DFT_HPD_FACTOR);
        curVrmInfo.u2Y = DIVIDE_WITH_ROUND((getMixedVPorch(bPath) * u4VPDSFactor) , DFT_HPD_FACTOR);

		#ifdef REGEN_BOTH_IN_PREREGEN_AND_FSCREGEN
	    if(_fgOvsn1PixelInPreregen)
	    { 
	    	_fgOvsn1PixelInPreregen = SV_FALSE;
			fgOvsn1PixelInFSCregen  = SV_TRUE;
			
			if(curVrmInfo.u2X > 0)
			{
			    curVrmInfo.u2X -= 1;
			}
			
	    }
	    else if(_fgOvsn2PixelInPreregen)
	    {
	        _fgOvsn2PixelInPreregen = SV_FALSE;
			if(curVrmInfo.u2X > 1)
			{
			    curVrmInfo.u2X -= 2;
			}
	    }
		#endif

		#ifdef REGEN_BOTH_IN_PREREGEN_AND_FSCREGEN
	    if(fgOvsn1PixelInFSCregen && (DIVIDE_WITH_ROUND(((picInfo->wSrcW - picInfo->wTargetW  - getMixedHPorch(bPath)) * u4HPDSFactor) , DFT_HPD_FACTOR) > 0))
	    { 
	    	fgOvsn1PixelInFSCregen = SV_FALSE;
        	curVrmInfo.u2Width = curVrmInfo.u2Width - curVrmInfo.u2X -
            	(DIVIDE_WITH_ROUND(((picInfo->wSrcW - picInfo->wTargetW  - getMixedHPorch(bPath)) * u4HPDSFactor) , DFT_HPD_FACTOR) - 1); //left have been - 1 in preregen
	    }
		else
		#endif
		{	
			curVrmInfo.u2Width = curVrmInfo.u2Width - curVrmInfo.u2X -
            	DIVIDE_WITH_ROUND(((picInfo->wSrcW - picInfo->wTargetW  - getMixedHPorch(bPath)) * u4HPDSFactor) , DFT_HPD_FACTOR); 
		}
		
        curVrmInfo.u2Height = curVrmInfo.u2Height - curVrmInfo.u2Y - 
            DIVIDE_WITH_ROUND(((picInfo->wSrcH  - picInfo->wTargetH  - getMixedVPorch(bPath))* u4VPDSFactor) , DFT_HPD_FACTOR);  

		if((picInfo->wSrcW - picInfo->wTargetW) < getMixedHPorch(bPath))
		{
			curVrmInfo.u2Width = curVrmInfo.u2Width - curVrmInfo.u2X;
		}

		if((picInfo->wSrcH  - picInfo->wTargetH) < getMixedVPorch(bPath))
		{
			curVrmInfo.u2Height = curVrmInfo.u2Height - curVrmInfo.u2Y;
		}   
    }
	
    if(!bIsScalerInput444(bPath)) //422 case
    {
        curVrmInfo.u2Width &= ~0x1;  //make it even
        curVrmInfo.u2X &= ~0x1;  //make it even
    }


#ifdef CC_FLIP_MIRROR_SUPPORT 
    if(u4GetFlipMirrorModule(bPath)==FLIP_BY_B2R)
    {
        if ((u1GetFlipMirrorConfig() & SYS_MIRROR_CONFIG_ON ) &&(!((u4DrvTDTVIs3DPhoto(&ePhotoCrop)) && (SV_VP_MAIN==bPath) && (eTTTOut == E_TD_OUT_3D_2_2D))))
        {
            if(fscIn->u2Width < curVrmInfo.u2Width + curVrmInfo.u2X)
            {
                curVrmInfo.u2X = 0;
            }
            else
            {
                 curVrmInfo.u2X = fscIn->u2Width - curVrmInfo.u2Width - curVrmInfo.u2X;
            }
        }    
        if ((u1GetFlipMirrorConfig() & SYS_FLIP_CONFIG_ON)&&(!((u4DrvTDTVIs3DPhoto(&ePhotoCrop)) && (SV_VP_MAIN==bPath) && (eTTTOut == E_TD_OUT_3D_2_2D))))
        {
            if(fscIn->u2Height < curVrmInfo.u2Height + curVrmInfo.u2Y)
            {
                curVrmInfo.u2Y = 0;
            }
            else
            {
                curVrmInfo.u2Y = fscIn->u2Height - curVrmInfo.u2Height - curVrmInfo.u2Y; 
            }
        }  
    }  
#endif
    
    if(fgVRMIsEqual(preVrmInfo, &curVrmInfo) == SV_FALSE)
    {
        x_memcpy((void*)preVrmInfo, (void*)(&curVrmInfo), sizeof(VRM_INFO_T));
        vVRMFireFSCResolutionChange(bPath);        
    }
}

void vVRMUpdateFSCResolutionWithoutOvsn(UINT8 bPath)
{
    VRM_INFO_T* fscIn;
    VRM_INFO_T* preVrmInfo;
    VRM_INFO_T curVrmInfo;

    if(bPath > SV_VP_PIP)
    {
        return;
    }

    fscIn = VRMGetVRMInfo(bPath, VRM_MODULE_FSC_REGEN);    
    preVrmInfo = VRMGetVRMInfo(bPath, VRM_MODULE_FSC);
    x_memcpy((void*)&curVrmInfo, (void*)fscIn, sizeof(VRM_INFO_T));
    curVrmInfo.u2VRMID = VRM_MODULE_FSC;
    
    if(fgVRMIsEqual(preVrmInfo, &curVrmInfo) == SV_FALSE)
    {
        x_memcpy((void*)preVrmInfo, (void*)(&curVrmInfo), sizeof(VRM_INFO_T));
        vVRMFireFSCResolutionChange(bPath);        
    }
}

void vVRMUpdateFSCResolution(UINT8 bPath)
{
    if(eVrmGetOvsnModule(bPath) == OVSN_MODULE_FSC_REGEN)
    {
        vVRMUpdateFSCResolutionWithOvsn(bPath);
    }
    else
    {
        vVRMUpdateFSCResolutionWithoutOvsn(bPath);
    }
}

inline void vVRMUpdateMixerResolutionWithMJC(UINT8 bPath)
{
#if defined(CC_MT5399)
    RDispInfo* dispInfo;
    VRM_INFO_T* mixerIn;
    VRM_INFO_T* preVrmInfo;
    VRM_INFO_T curVrmInfo;
    TDTV_3D_CROP_INTO_T ePhotoCrop;
    #ifdef CC_SUPPORT_VENC
    #ifdef VENC_SUB_SCALER_SCALING
    APP_RES_T rVencRes;
    #endif
    #endif

    E_TD_IN eSclIn = TD_SCL_IN(u4DrvTDTVScalerModeQuery());
    E_TD_OUT eSclOut = TD_SCL_OUT(u4DrvTDTVScalerModeQuery());

    E_TD_IN  eMJCIn  = TD_MJC_IN(u4DrvTDTVMJCModeQuery());
    E_TD_OUT eMJCOUT = TD_MJC_OUT(u4DrvTDTVMJCModeQuery());

    UINT16 u2MainMixerFR;

    if(bPath > SV_VP_PIP)
    {
        return;
    }
    
    if((bPath == SV_VP_PIP) && bDrvVrmIsAppFlagSet(VRM_APP_SCART_OUT))
    {
        dispInfo = getTveDispInfo();
    }
    else
    {
        dispInfo = getDispInfo(bPath);
    }
    mixerIn = VRMGetVRMInfo(bPath, VRM_MODULE_FSC);
    preVrmInfo = VRMGetVRMInfo(bPath, VRM_MODULE_MIXER);
    x_memcpy((void*)&curVrmInfo, (void*)mixerIn, sizeof(VRM_INFO_T));

    curVrmInfo.u2VRMID = VRM_MODULE_MIXER;

    //Mixer HTotal    
    if (u4DrvTDTVIsDualFPI())
    {
        curVrmInfo.u2HTotal = 2200;
    }
    else
    {
        curVrmInfo.u2HTotal = wDrvGetOutputHTotal();
    }

    if(eMJCOUT != E_TD_OUT_NATIVE)
    {
        curVrmInfo.u2X = 0;
        curVrmInfo.u2Y = 0;
    }
    else
    { 
        curVrmInfo.u2X = dispInfo->wXOffset;
        curVrmInfo.u2Y = dispInfo->wYOffset;
    }

    if((SV_VP_MAIN==bPath)&&u4DrvTDTVIs3DPhoto(&ePhotoCrop))
    {
       //Bypass 
    }
    else
    {
        curVrmInfo.u2Width = dispInfo->wWidth;
        curVrmInfo.u2Height = dispInfo->wHeight;  
    }
    
    //Mixer VTotal
    if(!(eMJCIn == E_TD_IN_2D_P && eMJCOUT == E_TD_OUT_NATIVE))       
    {
        //3D case, just use main outr as time gen timing.
        curVrmInfo.u2VTotal = curVrmInfo.u2Height;   // workaround for mjc output v_active <  post scaler input v_active;and need max dly 14 ,so need mjc input blank line add 14(45+14) to protect mjc init under   
    }
    else
    {
        //2D case, Need to be panel timing    
        if(IS_PANEL_L12R12)
        {
        	curVrmInfo.u2VTotal = wDrvGetOutputVActive()/2;
        }
		else
		{
        	curVrmInfo.u2VTotal = wDrvGetOutputVActive();
		}   
    }

    if(SV_VP_MAIN==bPath)
    {
        if(!ePhotoCrop.fgIsFullResolution)
        {
            if((E_TD_IN_TB_P==eMJCIn) || (E_TD_IN_LI_P==eMJCIn))
            {
                curVrmInfo.u2Height *=2;
                curVrmInfo.u2Y *= 2; 
                curVrmInfo.u2VTotal *=2;
            }
            
            //MJC SBS 3840 Mode
            if(((E_TD_IN_SBS_P==eMJCIn) && (E_TD_OUT_3D_LI==eMJCOUT))) // FPR MJC NOT SUPPORT SBS FULL
            {        
                curVrmInfo.u2Width *= 2;
                curVrmInfo.u2X *= 2;
                curVrmInfo.u2HTotal *= 2;
            }

            if((E_TD_IN_LI_P==eMJCIn) && (E_TD_OUT_3D_FPR==eMJCOUT)) // FPR MJC NOT SUPPORT LI ,BUT CAN SUPPORT HALF WIDTH LI
            {
                curVrmInfo.u2Width /= 2;
                curVrmInfo.u2X /= 2;
                curVrmInfo.u2HTotal /= 2;
            }
        }
        
        if(E_TD_OUT_3D_LI==eMJCOUT)    // mjc do 1920*1080 + 1920*1080 = 1920 * 2160 in all output format when panel is L12R12 PDP 
        {        
            curVrmInfo.u2Height /=2;
            curVrmInfo.u2Y /= 2;
            curVrmInfo.u2VTotal /=2;
        }
        
        // FOR MJC DRAM ALLIGN SETTING
        if(E_TD_IN_TB_P==eMJCIn)
        {
            curVrmInfo.u2Height = ALIGN_8_CEIL(curVrmInfo.u2Height); //mjc input height  align 8 when mjc input is  tab format  
        }
        
        //FOR FPR MODE MJC HEIGHT = SCALER OUTPUT HEIGHT /2 => SCALER OUTPUT HEIGHT NEED EVEN LINES
        if(E_TD_OUT_3D_FPR==eMJCOUT) 
        {
            curVrmInfo.u2Height = ALIGN_2_CEIL(curVrmInfo.u2Height);
        }
        
        if(E_TD_OUT_NATIVE!=eMJCOUT)
        {
            //mjc input width  align 8 when mjc input is tb format
            //mjc input widht  align 32 due to ttd need to be aligned 32
            curVrmInfo.u2Width = ALIGN_32_CEIL(curVrmInfo.u2Width - 0x1F); //mjc input height  align 32 when mjc input is  sbs format  and  up to panel size in post scaler         
        }
        
        if(E_TD_IN_SBS_P==eMJCIn)
        {
            //mjc input width  align 32 when mjc input is sbs format
            //mjc input widht  align 64 due to ttd need to be aligned 32
            curVrmInfo.u2Width = ALIGN_64_CEIL(curVrmInfo.u2Width - 0x3F);
        }

        //Before ECO, Scaler have sram efficiency problem
        if((BSP_GetIcVersion()==IC_VER_5398_AA)
            &&(curVrmInfo.u2FrameRate > 30)
            &&(
                //TB output datarate will over HW SPEC        
                ((eSclIn == E_TD_IN_TB_P) && (eSclOut == E_TD_OUT_NATIVE)&&(curVrmInfo.u2Height>1080) && (SV_VP_MAIN==bPath))
                //FS FHD 120 with upscaling
                //||(u4DrvTDTVIsDualFPI() && (curVrmInfo.u2Height>=1080))
                )
            )
        {
                curVrmInfo.u2FrameRate/=2;
        }
    }

    //sub's output frame rate is the same with main
    #ifdef SUPPORT_CUSTOMER_FRAME_RATE_SPEC
	if(curVrmInfo.u2FrameRate <= 30)
	{
	    curVrmInfo.u2FrameRate <<= 1;
	}
	#endif
    u2MainMixerFR = VRMGetVRMInfo(SV_VP_MAIN, VRM_MODULE_MIXER)->u2FrameRate;
    if(bPath == SV_VP_PIP)
    {
        curVrmInfo.u2FrameRate = u2MainMixerFR;
    }

    //Overwrite PIP X Offset & Width when TVE is enabled.
    if(_u4ForceDispOff[bPath])
    {
        curVrmInfo.u2X = 0xFFF;
        #if 0
        if (bDrvVrmIsAppFlagSet(VRM_APP_GRAPHIC_DUMP))
        {
            curVrmInfo.u2Width = 0;
            curVrmInfo.u2Height= 0;
        }
        #endif
    }

    //make height to be even when output Line Interleave
    if(((eSclIn == E_TD_IN_LI_P)||(eSclOut == E_TD_OUT_3D_LI))&& (SV_VP_MAIN==bPath))
    {
        curVrmInfo.u2Y      -= curVrmInfo.u2Y&0x1;   
        curVrmInfo.u2Height -= curVrmInfo.u2Height&0x1;
    }

    //after 3D operation, vtotal = original vtotal + 45;
    curVrmInfo.u2VTotal += 45;
    curVrmInfo.u2VTotal = MAX( curVrmInfo.u2VTotal , 120);

    //bound mixer frame rate
    curVrmInfo.u2FrameRate = MIN(curVrmInfo.u2FrameRate, VRM_MIXER_MAX_FR);

    if(curVrmInfo.u2ScanMode && bDrvDIIsScalerBob(bPath)) // interlaced input, scaler bob mode
    {
        curVrmInfo.u2ScanMode = 0;
    }

    #ifdef CC_SUPPORT_VENC
    #ifdef VENC_SUB_SCALER_SCALING
    vDrvVrmGetAppRes(VRM_APP_VIDEO_ENC, &rVencRes);
    if(bPath == VDP_2 && bDrvVrmIsAppFlagSet(VRM_APP_VIDEO_ENC) && u1DrvScpipGetVencModule() == E_VENC_MODULE_SUB_SCALER)
    {
        curVrmInfo.u2Width = rVencRes.u4Width;
        curVrmInfo.u2Height = rVencRes.u4Height; 
    }
    #endif
    #endif
            
    if(fgVRMIsEqual(&curVrmInfo, preVrmInfo) == SV_FALSE)
    {
        x_memcpy((void*)preVrmInfo, (void*)(&curVrmInfo), sizeof(VRM_INFO_T));
    }
#endif    
}

inline void vVRMUpdateMixerResolutionWithoutMJC(UINT8 bPath)
{
    RDispInfo* dispInfo;
    VRM_INFO_T* mixerIn;
    VRM_INFO_T* preVrmInfo;
    VRM_INFO_T curVrmInfo;
    TDTV_3D_CROP_INTO_T ePhotoCrop;

    E_TD_IN eSclIn = TD_SCL_IN(u4DrvTDTVScalerModeQuery());
    E_TD_OUT eSclOut = TD_SCL_OUT(u4DrvTDTVScalerModeQuery());

    UINT32 u4MIB     = u4DrvTDTVDIModeQuery();
    E_TD_IN eMIBIn   = TD_MIB_IN(u4MIB);
  //  E_TD_OUT eMIBOUT = TD_MIB_OUT(u4MIB);
    UINT32 u4TTTMode = u4DrvTDTVTTTModeQuery(bPath);
    E_TD_IN eTTTIn = TD_TTT_IN(u4TTTMode);
    E_TD_OUT eTTTOut = TD_TTT_OUT(u4TTTMode);
    if(bPath > SV_VP_PIP)
    {
        return;
    }
    
    if((bPath == SV_VP_PIP) && bDrvVrmIsAppFlagSet(VRM_APP_SCART_OUT))
    {
        dispInfo = getTveDispInfo();
    }
    else
    {
        dispInfo = getDispInfo(bPath);
    }
    mixerIn = VRMGetVRMInfo(bPath, VRM_MODULE_FSC);
    preVrmInfo = VRMGetVRMInfo(bPath, VRM_MODULE_MIXER);
    x_memcpy((void*)&curVrmInfo, (void*)mixerIn, sizeof(VRM_INFO_T));

    curVrmInfo.u2VRMID = VRM_MODULE_MIXER;
   
    if((SV_VP_MAIN==bPath)&&u4DrvTDTVIs3DPhoto(&ePhotoCrop))
    {        
        curVrmInfo.u2Width = wDrvVideoInputWidth(SV_VP_MAIN) - ePhotoCrop.u4CropLeftOffset - ePhotoCrop.u4CropRightOffset;
        curVrmInfo.u2Height = wDrvVideoInputHeight(SV_VP_MAIN) - ePhotoCrop.u4CropTopOffset - ePhotoCrop.u4CropBottomOffset;
        if(ePhotoCrop.fgIsFullResolution)
        {
            if((E_TD_IN_SBS_P==eMIBIn) || (E_TD_IN_SBS_I==eMIBIn)
                || ((eTTTOut == E_TD_OUT_3D_2_2D) && (eTTTIn == E_TD_IN_SBS_I || eTTTIn == E_TD_IN_SBS_P)))
            {
                curVrmInfo.u2Width = curVrmInfo.u2Width >> 1;
            }
            else if((E_TD_IN_TB_P==eMIBIn) || (E_TD_IN_TB_I==eMIBIn) || (E_TD_IN_LI_P==eMIBIn) || (E_TD_IN_LI_I==eMIBIn)
                || ((eTTTOut == E_TD_OUT_3D_2_2D) && ((eTTTIn == E_TD_IN_TB_P) || (eTTTIn == E_TD_IN_TB_I) || (eTTTIn == E_TD_IN_LI_P) || (eTTTIn == E_TD_IN_LI_I))))
            {
                curVrmInfo.u2Height = curVrmInfo.u2Height >> 1;
            }
            
        }
        curVrmInfo.u2X = (wDrvGetOutputHActive() - curVrmInfo.u2Width)/2;
        curVrmInfo.u2Y = (wDrvGetOutputVActive() - curVrmInfo.u2Height)/2;        
    }
    else
    {
        curVrmInfo.u2Width = dispInfo->wWidth;
        curVrmInfo.u2Height = dispInfo->wHeight;

        curVrmInfo.u2X = dispInfo->wXOffset;
        curVrmInfo.u2Y = dispInfo->wYOffset;        
    }

    //Overwrite PIP X Offset & Width when TVE is enabled.
    //if(_u4ForceDispOff[bPath])
    if(bPath == SV_VP_PIP)
    {
        curVrmInfo.u2X = 0xFFF;
    }
      
    //make height to be even when output Line Interleave
    if(((eSclIn == E_TD_IN_LI_P)||(eSclOut == E_TD_OUT_3D_LI))&& (SV_VP_MAIN==bPath))
    {
        curVrmInfo.u2Y      -= curVrmInfo.u2Y&0x1;   
        curVrmInfo.u2Height -= curVrmInfo.u2Height&0x1;
    }
	
	#ifdef CC_FLIP_MIRROR_SUPPORT 
        if (u1GetFlipMirrorConfig() & SYS_MIRROR_CONFIG_ON)
        {
            curVrmInfo.u2X = wDISPLAY_WIDTH - curVrmInfo.u2X - curVrmInfo.u2Width;
        }    

        if (u1GetFlipMirrorConfig() & SYS_FLIP_CONFIG_ON)
        {
            curVrmInfo.u2Y = wDISPLAY_HEIGHT - curVrmInfo.u2Y - curVrmInfo.u2Height;        
        }
	#endif		

    //Mixer VTotal
#ifdef SUPPORT_CUSTOMER_FRAME_RATE_SPEC
	if(curVrmInfo.u2FrameRate <= 30)
	{
	    curVrmInfo.u2FrameRate <<= 1;
	}
#endif
    curVrmInfo.u2FrameRate =  vDrvGetLCDFreq();
    curVrmInfo.u2VTotal = wDrvGetOutputVTotal();    
    curVrmInfo.u2HTotal = wDrvGetOutputHTotal();

    if(curVrmInfo.u2ScanMode && bDrvDIIsScalerBob(bPath)) // interlaced input, scaler bob mode
    {
        curVrmInfo.u2ScanMode = 0;
    }
        
    if(fgVRMIsEqual(&curVrmInfo, preVrmInfo) == SV_FALSE)
    {
        x_memcpy((void*)preVrmInfo, (void*)(&curVrmInfo), sizeof(VRM_INFO_T));
    }
}

//VRM module "Mixer" let FSC know the size to do scaling
void vVRMUpdateMixerResolution(UINT8 bPath)
{
    if (fgIsMJCToOSTG())
    {
        vVRMUpdateMixerResolutionWithMJC(bPath);
    }
    else
    {
        vVRMUpdateMixerResolutionWithoutMJC(bPath);
    }
}

 void vVRMUpdateMJCResolution(UINT8 bPath)
{
    VRM_INFO_T* mjcIn;
    VRM_INFO_T* preVrmInfo;
    VRM_INFO_T curVrmInfo;

    E_TD_IN eMJCIn = TD_MJC_IN(u4DrvTDTVMJCModeQuery());
    E_TD_OUT eMJCOUT = TD_MJC_OUT(u4DrvTDTVMJCModeQuery());
	
    TDTV_3D_CROP_INTO_T ePhotoCrop;
	UINT32 u4TTTMode;
    E_TD_OUT eTTDOut;
	u4TTTMode = u4DrvTDTVTTTModeQuery(bPath);
    eTTDOut = TD_TTT_OUT(u4TTTMode);

    if(bPath > SV_VP_PIP)
    {
        return;
    }
    
           
    mjcIn = VRMGetVRMInfo(bPath, VRM_MODULE_MIXER);     //get vrm from previous module
    preVrmInfo = VRMGetVRMInfo(bPath, VRM_MODULE_MJC);  //get old vrm info of current module
    x_memcpy((void*)&curVrmInfo, (void*)mjcIn, sizeof(VRM_INFO_T)); //copy vrm of previous module to current copy

    curVrmInfo.u2VRMID = VRM_MODULE_MJC;

    if(u4DrvTDTVModelWithoutMJC())
    {
        //to move output window on front scaler or post scaler will depends on diff. situation
        curVrmInfo.u2X = 0;
        curVrmInfo.u2Y = 0;
        curVrmInfo.u2Width  = wDrvGetOutputHActive();
        curVrmInfo.u2Height = wDrvGetOutputVActive();
        curVrmInfo.u2HTotal = wDrvGetOutputHTotal();
        curVrmInfo.u2VTotal = wDrvGetOutputVTotal();
    }
    else
    {        
	if(eMJCIn == E_TD_IN_2D_P && eMJCOUT == E_TD_OUT_NATIVE 
		&& !(u4DrvTDTVIs3DPhoto(&ePhotoCrop) && eTTDOut==E_TD_OUT_3D_2_2D))    
    {
        curVrmInfo.u2X = 0;
        curVrmInfo.u2Y = 0;
        curVrmInfo.u2Width  = wDrvGetOutputHActive();
        curVrmInfo.u2Height = wDrvGetOutputVActive();
        //curVrmInfo.u2HTotal = wDrvGetOutputHTotal();        
        if(IS_PANEL_L12R12)
        {
            curVrmInfo.u2Height >>= 1;
		}		
    }
    }

    //curVrmInfo.u2VTotal = curVrmInfo.u2Height + 45;   // workaround for mjc output v_active <  post scaler input v_active;and need max dly 14 ,so need mjc input blank line add 14(45+14) to protect mjc init under
    /* HTotal/VTotal cames from Mixer */
    
    if(fgVRMIsEqual(&curVrmInfo, preVrmInfo) == SV_FALSE)
    {
        x_memcpy((void*)preVrmInfo, (void*)(&curVrmInfo), sizeof(VRM_INFO_T));
        LOG(3, "vVRMUpdateMJCResolution: (%d, %d, %d).\n",
            preVrmInfo->u2Width, preVrmInfo->u2Height, preVrmInfo->u2FrameRate);
    }    
}

void vVRMUpdatePSCResolution(UINT8 bPath)
{
    VRM_INFO_T* pscIn;
    VRM_INFO_T* preVrmInfo;
    VRM_INFO_T curVrmInfo;
    
    E_TD_IN eMJCIn = TD_MJC_IN(u4DrvTDTVMJCModeQuery());
    E_TD_OUT eMJCOUT = TD_MJC_OUT(u4DrvTDTVMJCModeQuery());
    
    if(bPath > SV_VP_PIP)
    {
        return;
    }

    pscIn = VRMGetVRMInfo(bPath, VRM_MODULE_MJC);

    preVrmInfo = VRMGetVRMInfo(bPath, VRM_MODULE_PSC);
    x_memcpy((void*)&curVrmInfo, (void*)pscIn, sizeof(VRM_INFO_T));
    curVrmInfo.u2VRMID = VRM_MODULE_PSC;
    
    if(!u4DrvTDTVModelWithoutMJC())
    {
    curVrmInfo.u2FrameRate =  vDrvGetLCDFreq();
        
    if(E_TD_IN_SBS_P==eMJCIn)
    {
        curVrmInfo.u2Width /=2;
		curVrmInfo.u2HTotal /=2;
    }

    if((E_TD_IN_TB_P==eMJCIn) || (E_TD_IN_LI_P==eMJCIn)) 
    {
        curVrmInfo.u2Height /=2;
		curVrmInfo.u2VTotal /=2;
    }

    if((E_TD_OUT_3D_LI==eMJCOUT) || (IS_PANEL_L12R12))
    {        
        curVrmInfo.u2Height *=2;
		curVrmInfo.u2VTotal *=2;
    }

	curVrmInfo.u2HTotal = wDrvGetOutputHTotal();
    curVrmInfo.u2VTotal = wDrvGetOutputVTotal();    
    } 
	
    if(fgVRMIsEqual(preVrmInfo, &curVrmInfo) == SV_FALSE)
    {
        x_memcpy((void*)preVrmInfo, (void*)(&curVrmInfo), sizeof(VRM_INFO_T));
    }
}

void vVRMUpdatePSCMixerResolution(UINT8 bPath)
{
    RDispInfo* dispInfo;    
    VRM_INFO_T* pscMixerIn;
    VRM_INFO_T* preVrmInfo;
    VRM_INFO_T curVrmInfo;    
    E_TD_OUT eMJCOUT;    
    TDTV_3D_CROP_INTO_T ePhotoCrop;
    
    if(bPath > SV_VP_PIP)
    {
        return;
    }
    
    dispInfo = getDispInfo(bPath);
    eMJCOUT = TD_MJC_OUT(u4DrvTDTVMJCModeQuery());
    
    pscMixerIn = VRMGetVRMInfo(bPath, VRM_MODULE_PSC);
    preVrmInfo = VRMGetVRMInfo(bPath, VRM_MODULE_PSC_MIXER);
    x_memcpy((void*)&curVrmInfo, (void*)pscMixerIn, sizeof(VRM_INFO_T));
    
    curVrmInfo.u2VRMID = VRM_MODULE_PSC_MIXER;

    if(!u4DrvTDTVModelWithoutMJC())
    {
    if(eMJCOUT != E_TD_OUT_NATIVE)
    {
        if((SV_VP_MAIN==bPath)&&u4DrvTDTVIs3DPhoto(&ePhotoCrop))
        {    
            curVrmInfo.u2X = (wDrvGetOutputHActive() - curVrmInfo.u2Width)/2;
            curVrmInfo.u2Y = (wDrvGetOutputVActive() - curVrmInfo.u2Height)/2;
        }
        else
        {
            curVrmInfo.u2X = dispInfo->wXOffset;
            curVrmInfo.u2Y = dispInfo->wYOffset;
            curVrmInfo.u2Width = dispInfo->wWidth;
        }
    }
	else if(u4DrvTDTVIs3DPhoto(&ePhotoCrop))
	{	 
		curVrmInfo.u2X = (wDrvGetOutputHActive() - curVrmInfo.u2Width)/2;
		curVrmInfo.u2Y = (wDrvGetOutputVActive() - curVrmInfo.u2Height)/2;
	}	
    else 
    {
        curVrmInfo.u2X = 0;
        curVrmInfo.u2Y = 0;
        curVrmInfo.u2Width  = wDrvGetOutputHActive();
        curVrmInfo.u2Height = wDrvGetOutputVActive();
    }

#ifdef CC_MT5399
        if (u4DrvTDTV3DModeQuery() != E_TDTV_UI_3D_MODE_OFF)
    {
        if (fgDrvMJCGetFlipMirror() & 0x1)
        {
            curVrmInfo.u2X = wDISPLAY_WIDTH - curVrmInfo.u2X - curVrmInfo.u2Width;
        }    

        if (fgDrvMJCGetFlipMirror() & 0x2)
        {
            curVrmInfo.u2Y = wDISPLAY_HEIGHT - curVrmInfo.u2Y - curVrmInfo.u2Height;        
        }
    }
#endif
    }

    if(fgVRMIsEqual(preVrmInfo, &curVrmInfo) == SV_FALSE)
    {
        x_memcpy((void*)preVrmInfo, (void*)(&curVrmInfo), sizeof(VRM_INFO_T));
    }    
}

void vVRMUpdateOSTGResolution(UINT8 bPath)
{
    VRM_INFO_T* ostgIn;
    VRM_INFO_T* preVrmInfo;
    VRM_INFO_T curVrmInfo;
    //RDispInfo* dispInfo;

    if(bPath != SV_VP_MAIN)
    {
        return;
    }
    
    //dispInfo = getDispInfo(bPath);
    ostgIn = VRMGetVRMInfo(bPath, VRM_MODULE_PSC_MIXER);
    preVrmInfo = VRMGetVRMInfo(bPath, VRM_MODULE_OSTG);
    x_memcpy((void*)&curVrmInfo, (void*)ostgIn, sizeof(VRM_INFO_T));
    curVrmInfo.u2VRMID = VRM_MODULE_OSTG;

    //to move output window on front scaler or post scaler will depends on diff. situation
    curVrmInfo.u2X = 0;
    curVrmInfo.u2Y = 0;
    curVrmInfo.u2Width  = wDrvGetOutputHActive();
    curVrmInfo.u2Height = wDrvGetOutputVActive();
    curVrmInfo.u2HTotal = wDrvGetOutputHTotal();
    curVrmInfo.u2VTotal = wDrvGetOutputVTotal();
    
    if(fgVRMIsEqual(preVrmInfo, &curVrmInfo) == SV_FALSE)
    {
        x_memcpy((void*)preVrmInfo, (void*)(&curVrmInfo), sizeof(VRM_INFO_T));
    }
}

UINT8 fgVRMUpdateDIMaxOutputWidth(UINT8 bPath)
{
    UINT8 ret = SV_TRUE;
    VRM_INFO_T in;
    VRM_INFO_T out;
    UINT16 u2InVActive;
    UINT16 u2OutVActive;
    UINT16 u2OutVTotal;
    UINT16 u2OutFr;
    UINT32 u4DIMaxOutWidth;
    UINT32 u4MIB     = u4DrvTDTVDIModeQuery();
    E_TD_IN eMIBIn   = TD_MIB_IN(u4MIB);
    E_TD_OUT eMIBOUT = TD_MIB_OUT(u4MIB);

    //get MIB in/out
    VRMGetModuleVRMInfo(bPath,VRM_MODULE_MIB,&in,&out);
    u2InVActive = in.u2Height;
    VRMGetModuleVRMInfo(bPath,VRM_MODULE_MIXER,&in,&out);
    u2OutVActive = in.u2Height;
    u2OutVTotal = in.u2VTotal;
    u2OutFr = in.u2FrameRate;

    if((SV_VP_MAIN==bPath)&&(E_TD_OUT_3D_FS==eMIBOUT))
    {
        if((E_TD_IN_TB_I==eMIBIn)||(E_TD_IN_TB_P==eMIBIn)
            ||(E_TD_IN_LI_I==eMIBIn)||(E_TD_IN_LI_P==eMIBIn))
        {
            u2InVActive/=2;
        }
    }
    
    u4DIMaxOutWidth = u4DrvDIMaxOutWidth(getScalerVDP(bPath),u2InVActive,u2OutVActive,u2OutVTotal,u2OutFr);
    LOG(1, "u4DIMaxOutWidth %d[%d] = %d %d %d %d\n", u4DIMaxOutWidth,bPath,u2InVActive,u2OutVActive,u2OutVTotal,u2OutFr);

    if((SV_VP_MAIN==bPath)
        &&((E_TD_IN_SBS_I==eMIBIn)||(E_TD_IN_SBS_P==eMIBIn))
        && (E_TD_OUT_NATIVE!=eMIBOUT))
    {
        u4DIMaxOutWidth = MIN(1920,u4DIMaxOutWidth*2);
    }

    if(u4DIMaxOutWidth == _u4DIMaxOutWidth[bPath])
    {
        //means there's no update
        ret = SV_FALSE;
    }
    else
    {
        _u4DIMaxOutWidth[bPath] = u4DIMaxOutWidth;    
    }
            
    LOG(1,"path(%d) _u4DIMaxOutWidth(%d) MDDIMaxFifoWidth=(%d)\n",bPath, _u4DIMaxOutWidth[bPath], getPDSInfo(bPath)->u4MddiFifoMax);

    return ret;
}

UINT32 u4VrmGetDIMaxOutWidth(UINT8 bPath)
{
    return _u4DIMaxOutWidth[bPath];
}

static void vVRMFireOMUXResolutionChange(UINT8 bPath)
{ 
    LOG(3, "vVRMFireOMUXResolutionChange\n");
}

static void vVRMFireCBResolutionChange(UINT8 bPath)
{
    //Trigger TDTV mode change when CB resolution change.
    vDrvTDTVTriggerCfgCheck();
    
    LOG(3, "vVRMFireCBResolutionChange\n");
}

static void vVRMFireDQResolutionChange(UINT8 bPath)
{
    LOG(3, "vVRMFireDQResolutionChange\n");
}

static void vVRMFireTTTResolutionChange(UINT8 bPath)
{
    LOG(3, "vVRMFireOMUXResolutionChange\n");
}

static void vVRMFirePREDOWNResolutionChange(UINT8 bPath)
{
    //Set predown mode chg. Because predown input changed.
    vPDSSetModeChanged(bPath);

    vDrvSetImportWaitFlag(bPath, VRM_IMPORT_WAIT_FLAG_PDS);
    LOG(3, "vVRMFirePredownResolutionChange\n");
}

static void vVRMFireREGENResolutionChange(UINT8 bPath)
{
    //Set predown mode chg. Because predown output changed.
    vPDSSetModeChanged(bPath);
    //Set regen mode chg. Because regen input changed.    
    vRegenSetModeChanged(bPath,REGEN_MODULE_PRE_REGEN);

    vDrvSetImportWaitFlag(bPath, VRM_IMPORT_WAIT_FLAG_PDS);
    vDrvSetImportWaitFlag(bPath, VRM_IMPORT_WAIT_FLAG_REGEN);    
    LOG(3, "vVRMFireREGENResolutionChange\n");
}

static void vVRMFireMIBResolutionChange(UINT8 bPath)
{
    //Set regen mode chg. Because regen output changed.    
    vRegenSetModeChanged(bPath,REGEN_MODULE_PRE_REGEN);
    
    vDrvSetImportWaitFlag(bPath, VRM_IMPORT_WAIT_FLAG_REGEN);    
    vDrvSetImportWaitFlag(bPath, VRM_IMPORT_WAIT_FLAG_MIB);
    
    LOG(3, "vVRMFireMIBResolutionChange\n");
}

void vVRMFireFSCRegenResolutionChange(UINT8 bPath)
{
    vRegenSetModeChanged(bPath,REGEN_MODULE_FSC_REGEN);
    vDrvSetImportWaitFlag(bPath, VRM_IMPORT_WAIT_FLAG_MIB);
    vDrvSetImportWaitFlag(bPath, VRM_IMPORT_WAIT_FLAG_FSC_REGEN);
    LOG(3, "vVRMFireFSCRegenResolutionChange\n");
}

static void vVRMFireFSCResolutionChange(UINT8 bPath)
{
    //Set FSC mode chg. Because FSC input changed.
    vRegenSetModeChanged(bPath,REGEN_MODULE_FSC_REGEN);
    vScpipReconfigScl(getScalerVDP(bPath));
    vDrvSetImportWaitFlag(bPath, VRM_IMPORT_WAIT_FLAG_FSC_REGEN);
    vDrvSetImportWaitFlag(bPath, VRM_IMPORT_WAIT_FLAG_FSC);
    vDrvTDTVTriggerCfgCheck();    
    
    if(bPath == SV_VP_MAIN)
    {
        vApiRegisterVideoEvent(PE_EVENT_SCALER_PE, SV_VP_MAIN, SV_ON);
    }
    LOG(3, "vVRMFireFSCResolutionChange\n");
}

static void vVRMFireMixerResolutionChange(UINT8 bPath)
{
    //Set FSC mode chg. Because FSC output changed.
    vScpipReconfigScl(getScalerVDP(bPath));
    
    vDrvSetImportWaitFlag(bPath, VRM_IMPORT_WAIT_FLAG_FSC);
    vDrvSetPEUIResetFlg(bPath, SV_ON);        
    vDrvTDTVTriggerCfgCheck();
    
    if(bPath == SV_VP_MAIN)
    {
        vApiRegisterVideoEvent(PE_EVENT_SCALER_PE, SV_VP_MAIN, SV_ON);
    }
    LOG(3, "vVRMFireMixerResolutionChange\n");
}

static void vVRMFireMJCResolutionChange(UINT8 bPath)
{
    #ifdef CC_MT5399
    if (bPath == SV_VP_MAIN)
    {
        vDrvMJCSetModeChangeTrig();
    }
    LOG(3, "vVRMFireMJCResolutionChange\n");
    #endif
}

static void vVRMFirePSCResolutionChange(UINT8 bPath)
{
    //Set PSC mode chg. Because PSC input changed.
    vScpipReconfigScl(getScalerVDP(bPath));
    LOG(3, "vVRMFirePSCResolutionChange\n");
}

static void vVRMFirePSCMixerResolutionChange(UINT8 bPath)
{
    //Set PSC mode chg. Because PSC input changed.
    vScpipReconfigScl(getScalerVDP(bPath));
    LOG(3, "vVRMFirePSCMixerResolutionChange\n");
}

static void vVRMFireOSTGResolutionChange(UINT8 bPath)
{
    //Set PSC mode chg. Because PSC output changed.
    vScpipReconfigScl(getScalerVDP(bPath));
    LOG(3, "vVRMFireOSTGResolutionChange\n");
}

void vVRMFireResolutionChange(UINT8 bPath)
{    
    if(x_memcmp((const void *)&_vrm_info_pre[bPath][VRM_MODULE_OMUX]
        ,(const void *)&_vrm_info[bPath][VRM_MODULE_OMUX],sizeof(VRM_INFO_T)))
    {
        vVRMFireOMUXResolutionChange(bPath);        
    }
    if(x_memcmp((const void *)&_vrm_info_pre[bPath][VRM_MODULE_CB]
        ,(const void *)&_vrm_info[bPath][VRM_MODULE_CB],sizeof(VRM_INFO_T)))
    {
        vVRMFireCBResolutionChange(bPath);        
    }
    if(x_memcmp((const void *)&_vrm_info_pre[bPath][VRM_MODULE_DQ]
        ,(const void *)&_vrm_info[bPath][VRM_MODULE_DQ],sizeof(VRM_INFO_T)))
    {
        vVRMFireDQResolutionChange(bPath);            
    }
    if(x_memcmp((const void *)&_vrm_info_pre[bPath][VRM_MODULE_TTT]
        ,(const void *)&_vrm_info[bPath][VRM_MODULE_TTT],sizeof(VRM_INFO_T)))
    {
        vVRMFireTTTResolutionChange(bPath);        
    }
    if(x_memcmp((const void *)&_vrm_info_pre[bPath][VRM_MODULE_PDS]
        ,(const void *)&_vrm_info[bPath][VRM_MODULE_PDS],sizeof(VRM_INFO_T)))
    {
        vVRMFirePREDOWNResolutionChange(bPath);        
    }
    if(x_memcmp((const void *)&_vrm_info_pre[bPath][VRM_MODULE_REGEN]
        ,(const void *)&_vrm_info[bPath][VRM_MODULE_REGEN],sizeof(VRM_INFO_T)))
    {
        vVRMFireREGENResolutionChange(bPath);        
    }
    if(x_memcmp((const void *)&_vrm_info_pre[bPath][VRM_MODULE_MIB]
        ,(const void *)&_vrm_info[bPath][VRM_MODULE_MIB],sizeof(VRM_INFO_T)))
    {
        vVRMFireMIBResolutionChange(bPath);        
    }
    if(x_memcmp((const void *)&_vrm_info_pre[bPath][VRM_MODULE_FSC]
        ,(const void *)&_vrm_info[bPath][VRM_MODULE_FSC],sizeof(VRM_INFO_T)))
    {
        vVRMFireFSCResolutionChange(bPath);        
    }        
    if(x_memcmp((const void *)&_vrm_info_pre[bPath][VRM_MODULE_MIXER]
        ,(const void *)&_vrm_info[bPath][VRM_MODULE_MIXER],sizeof(VRM_INFO_T)))
    {
        vVRMFireMixerResolutionChange(bPath);        
    }           
    if(x_memcmp((const void *)&_vrm_info_pre[bPath][VRM_MODULE_MJC]
        ,(const void *)&_vrm_info[bPath][VRM_MODULE_MJC],sizeof(VRM_INFO_T)))
    {
        vVRMFireMJCResolutionChange(bPath);        
    }        
    if(x_memcmp((const void *)&_vrm_info_pre[bPath][VRM_MODULE_PSC]
        ,(const void *)&_vrm_info[bPath][VRM_MODULE_PSC],sizeof(VRM_INFO_T)))
    {
        vVRMFirePSCResolutionChange(bPath);
        vVRMFireMJCResolutionChange(bPath);
    }        
    if(x_memcmp((const void *)&_vrm_info_pre[bPath][VRM_MODULE_PSC_MIXER]
        ,(const void *)&_vrm_info[bPath][VRM_MODULE_PSC_MIXER],sizeof(VRM_INFO_T)))
    {
        vVRMFirePSCMixerResolutionChange(bPath);        
    }        
    if(x_memcmp((const void *)&_vrm_info_pre[bPath][VRM_MODULE_OSTG]
        ,(const void *)&_vrm_info[bPath][VRM_MODULE_OSTG],sizeof(VRM_INFO_T)))
    {
        vVRMFireOSTGResolutionChange(bPath);
    }   

    LOG(3, "vVRMFireResolutionChange\n");
}


void vVRMUpdate121Resolution(UINT8 bPath)
{
    RDispInfo* dispInfo;

    dispInfo = getDispInfo(bPath);
    vScpipUpdate121MapEn(getScalerVDP(bPath));                                //decide is 121 and 121-able
    bApiVideoSetDispRegion(bPath, dispInfo->rOutRegion.u4X,        //update dispInfo pixel-based (x, y, width, height)
                                  dispInfo->rOutRegion.u4Y, 
                                  dispInfo->rOutRegion.u4Width, 
                                  dispInfo->rOutRegion.u4Height);    
}

void vVRMUpdateResolution(UINT8 bPath)
{
    RPicInfo* picInfo;
    VDP_SEAMLESS_INFO_T tb2rVrmInfo;
    static VDP_SEAMLESS_INFO_T tPreb2rVrmInfo;

    if(VDP_SET_ERROR != VDP_GetSeamlessInfo(bPath, &tb2rVrmInfo))
    {
        if (tPreb2rVrmInfo.u4SrcWidth  != tb2rVrmInfo.u4SrcWidth ||
            tPreb2rVrmInfo.u4SrcHeight  != tb2rVrmInfo.u4SrcHeight)
        {
            bVRMUpdateSrcReg[bPath] = TRUE; 	
            bVideoUpdateSrcRegion(bPath);
            tPreb2rVrmInfo = tb2rVrmInfo;
        }   
    }

    if(u4DrvTDTVIsDualFPI()&&(SV_VP_PIP==bPath))
    {
        picInfo = getPicInfo(SV_VP_MAIN);
    }
    else
    {
        picInfo = getPicInfo(bPath);
    }
        
    LOG(2, "VRM(%d) Update resolution start.....\n",bPath);
    
    vVrmUpdateOvsnModule(bPath);
    vVRMUpdate121Resolution(bPath);
    if(picInfo->wSrcH != 0 && picInfo->wSrcW != 0 && picInfo->wHTotal != 0 && picInfo->wVTotal != 0)
    {
        vVRMUpdateOMUXResolution(bPath);
        vVRMUpdateCBResolution(bPath);
        vVRMUpdateDQResolution(bPath);
        vVRMUpdateTTTResoluton(bPath);
        vVRMUpdatePredownResolution(bPath);
        vVRMUpdateRegenResolution(bPath);
        vVRMUpdateMIBResolution(bPath);
        vVRMUpdateFSCRegenResolution(bPath);
        vVRMUpdateFSCResolution(bPath);
    }
    //Need to update following resolution even there's no input.
    vVRMUpdateMixerResolution(bPath);
    vVRMUpdateMJCResolution(bPath);
    vVRMUpdatePSCResolution(bPath);
    vVRMUpdatePSCMixerResolution(bPath);
    vVRMUpdateOSTGResolution(bPath);
    LOG(2, "VRM(%d) Update resolution done......\n",bPath);
    return;
}

void vVRMTriggerUpdateByCli(UINT8 bPath)
{
    vVRMSetEventFlg(bPath, VRM_EVENT_BY_CLI);
}

#ifdef VRM_CHK_SET_RES_STATUS
void vVRMChkSetResStatus(UINT8 bPath)
{
    if (bDrvPDSIsSetResDone(bPath, SET_RES_INPUT))
    {
        LOG(0,"Path(%d) Predown Gets In Set Resolution Signal\n", bPath);
        vDrvPDSClrSetRes(bPath, SET_RES_INPUT);  
    }
    if (bDrvPDSIsSetResDone(bPath, SET_RES_OUTPUT))
    {
        LOG(0,"Path(%d) Predown Outputs Set Resolution Signal\n", bPath);
        vDrvPDSClrSetRes(bPath, SET_RES_OUTPUT);
    }

    if (bDrvRegenIsSetResDone(bPath, SET_RES_INPUT))
    {
        LOG(0,"Path(%d) Regen Gets In Set Resolution Signal\n", bPath);
        vDrvRegenClrSetRes(bPath, SET_RES_INPUT);
    }
    if (bDrvRegenIsSetResDone(bPath, SET_RES_OUTPUT))
    {
        LOG(0,"Path(%d) Regen Outputs Set Resolution Signal\n", bPath);
        vDrvRegenClrSetRes(bPath, SET_RES_OUTPUT);
    }

    if (bDrvDIIsSetResDone(bPath, SET_RES_INPUT))
    {
        LOG(0,"Path(%d) DI Gets In Set Resolution Signal\n", bPath);
        vDrvDIClrSetRes(bPath, SET_RES_INPUT);
    }
    if (bDrvDIIsSetResDone(bPath, SET_RES_OUTPUT))
    {
        LOG(0,"Path(%d) DI Outputs Set Resolution Signal\n", bPath);
        vDrvDIClrSetRes(bPath, SET_RES_OUTPUT);
    }

    if (bDrvScpipIsSetResDone(bPath, SET_RES_INPUT))
    {
        LOG(0,"Path(%d) Scaler Gets In Set Resolution Signal\n", bPath);
        vDrvScpipClrSetRes(bPath, SET_RES_INPUT);
    }
    if (bDrvScpipIsSetResDone(bPath, SET_RES_OUTPUT))
    {
        LOG(0,"Path(%d) Scaler Outputs Set Resolution Signal\n", bPath); 
        vDrvScpipClrSetRes(bPath, SET_RES_OUTPUT);
    }

}
#endif

UINT32 u4VRMSetTriggerMode(UINT8 bPath)
{   
    UINT32 u4NotPending = SV_TRUE;
    
    if(bPath > VDP_2)
    {
        return 0;
    }

    if(VRM_EVENT_BY_RECAL!=vVRMGetEventFlg(bPath))
    {
        _u4PDSWidthFix[bPath]=PDS_WIDTH_FIX_UNKNOWN;
    }

    if(eDrvGetForcedImportTriggerModule(bPath) != IMPORT_MODULE_UNKNOWN)
    {
        LOG(1,"path(%d) u4VRMSetTriggerMode:: force import module to %d\n",
            bPath, eDrvGetForcedImportTriggerModule(bPath));
        vDrvSetImportTriggerModule(bPath, eDrvGetForcedImportTriggerModule(bPath));
        goto END_OF_SET_TRIGGER_MODLE;
    }
    
    if(bDrvVideoSignalStatus(bPath)!=(UINT8)SV_VDO_STABLE)
    {
        LOG(1, "path(%d) Signal un-stable, set import module to NONE\n",bPath);
        vDrvSetImportTriggerModule(bPath, IMPORT_MODULE_NONE);
    }
    else if (fgVRMIsEventFlgSet(bPath, VRM_EVENT_BY_VDO_MODCHG))
    {
        LOG(1, "path(%d) VRM_EVENT_BY_VDO_MODCHG is 1, set import module to NONE\n",bPath);
        vDrvSetImportTriggerModule(bPath, IMPORT_MODULE_NONE);
    }
    else if (fgVRMIsEventFlgSet(bPath, VRM_EVENT_BY_MIB))
    {
        LOG(1, "path(%d) VRM_EVENT_BY_MIB is 1, set import module to NONE\n",bPath);
        vDrvSetImportTriggerModule(bPath, IMPORT_MODULE_NONE);
    }
    else if(_fgAutoSearch)
    {
        LOG(1, "path(%d) Auto Search mode,  set import module to NONE\n",bPath);
        vDrvSetImportTriggerModule(bPath, IMPORT_MODULE_NONE);
    }    
#if defined(VRM_TURNOFF_IMPORT_AT_3D)  
    else if(bPath == VDP_1 &&
            TD_TTD_IN(u4DrvTDTVTTDModeQuery())  != E_TD_IN_2D_P 
           )
    {
        LOG(1, "path(%d) Under 3D, set import module to NONE\n",bPath);
        vDrvSetImportTriggerModule(bPath, IMPORT_MODULE_NONE);
    }
#endif         
    else if (VRM_IMPORT_WAIT_NONE == vDrvGetImportWaitFlag(bPath))
    {
        if (fgVRMIsEventFlgSet(bPath, VRM_EVENT_BY_B2R) || VDP_SeamlessSeqChanging(bPath))
        {
            LOG(1, "path(%d) set import module to B2R\n",bPath);
            vDrvSetImportTriggerModule(bPath, IMPORT_MODULE_B2R);
            vDrvSetImportWaitFlag(bPath, VRM_IMPORT_WAIT_FLAG_TRIG_START);
        }
        else
        {
            LOG(1, "path(%d) set import module to PDS\n",bPath);
            vDrvSetImportTriggerModule(bPath, IMPORT_MODULE_PDS);
            vDrvSetImportWaitFlag(bPath, VRM_IMPORT_WAIT_FLAG_TRIG_START);
        }    
    }    
    else if(fgVRMIsEventFlgSet(bPath, VRM_EVENT_BY_RECAL))
    {
        // Do Nothing
    }
    else
    {
        //Set Resolution not ready yet, can not handle next resolution event
        LOG(4, "path(%d) set import module no change, import flag(0x%X)\n",
            bPath,vDrvGetImportWaitFlag(bPath));
        u4NotPending = SV_FALSE;
    }

END_OF_SET_TRIGGER_MODLE:

    if(fgVRMIsEventFlgSet(bPath, VRM_EVENT_BY_TDTV)&&u4NotPending)
    {   //if 3D Fmt changed , regen need to be re-calculate
        vRegenSetModeChanged(bPath, REGEN_MODULE_PRE_REGEN);
    }
    
    return u4NotPending;
}

UINT8 _VRMImport_Delay[2] = {0, 0};
void vVRMSetImportDelay(UINT8 bPath, UINT8 u1Delay)
{
    LOG(0, "Delay[%d] = %d\n", bPath, u1Delay);
    _VRMImport_Delay[bPath] = u1Delay;
}

UINT8 vVRMGetImportDelay(UINT8 bPath)
{
    UINT8 u1Delay = _VRMImport_Delay[bPath];
    if(u1Delay == 0)
    {
        UINT8 u1Ratio = 1;
        UINT8 u1InputFrameRate = bDrvVideoGetRefreshRate(bPath);
        UINT8 u1OutputFrameRate = vDrvGetLCDFreq();
        LOG(1,"===u1InputFrameRate %d, u1OutputFrameRate %d,",u1InputFrameRate,u1OutputFrameRate);
        if(u1InputFrameRate != 0)
        {
            if(u1OutputFrameRate > 77)
            {
                u1OutputFrameRate = u1OutputFrameRate/2;
            }
            u1Ratio = (u1OutputFrameRate + u1InputFrameRate - 1)/u1InputFrameRate;
        }
        LOG(1," u1Ratio %d,",u1Ratio);
        if(bDrvVideoIsSrcInterlace(bPath))
        {
            u1Delay = 4 * u1Ratio;
        }
        else
        {
            u1Delay = 2 * u1Ratio;
        }
        LOG(1," u1Delay %d.\n",u1Delay);
    }
    
    return u1Delay;
}

void vVRMLoop(UINT8 bPath)
{
    #define SET_RES_TRIGGER_DELAY 7

    CRIT_STATE_T csState;
    UINT32 u4VRMPassCnt = 0;    
    static UINT8 u4TriggerWaitCnt[2] = {0,0};

    if(VRM_IMPORT_WAIT_FLAG_TRIG_START == vDrvGetImportWaitFlag(bPath) &&
       B2R_TRIG_OFF == _eVRMWaitForB2RTrigger[bPath])
    {
        if (TRUE == fgVRMIsEventFlgSet(bPath, VRM_EVENT_BY_B2R))
        {//B2R resolution changes before triggering import
         //Force to calculate new resolution to reduce worst case time.
            vDrvClrImportWaitFlag(bPath, VRM_IMPORT_WAIT_FLAG_TRIG_START); 
        }
        else
        {
            vDrvFireImportTrigger(bPath);
            vDrvSetImportWaitFlag(bPath, VRM_IMPORT_WAIT_FLAG_TRIG_END);        
            vDrvClrImportWaitFlag(bPath, VRM_IMPORT_WAIT_FLAG_TRIG_START);

            u4TriggerWaitCnt[bPath] =(IMPORT_MODULE_NONE == vDrvGetImportTriggerModule(bPath)) ? 
                0 :vVRMGetImportDelay(bPath);
        }
    }

    if(0==u4TriggerWaitCnt[bPath])
    {
        if(VRM_IMPORT_WAIT_FLAG_TRIG_END & vDrvGetImportWaitFlag(bPath))
        {
            LOG(3, "vVRMLoop -END Event %X\n", vVRMGetEventFlg(bPath));
            vDrvClrImportWaitFlag(bPath, VRM_IMPORT_WAIT_FLAG_TRIG_END);
        }
    }
    else
    {
        u4TriggerWaitCnt[bPath]--;
    }

    if(VRM_EVENT_NONE !=vVRMGetEventFlg(bPath))
    {
        x_memcpy((void *)_vrm_info_pre,(const void *)_vrm_info,sizeof(_vrm_info));
    
        do
        {
            csState = x_crit_start();
        
            if(SV_FAIL == u4VRMSetTriggerMode(bPath))
            {
                //if still waiting video engine processing , don't update new event        
                x_crit_end(csState);
                break;
            }

            LOG(1, "========VRM(%d) Loop Event flag (0x%X) at %d-th pass========\n", 
                bPath,vVRMGetEventFlg(bPath),u4VRMPassCnt++);

            vVRMClrEventFlg(bPath); 
            x_crit_end(csState);

            vVRMUpdateResolution(bPath);

            //Check if Resolution converged
            if(SV_TRUE==fgVRMUpdateDIMaxOutputWidth(bPath))
            {        
                vVRMSetEventFlg(bPath,VRM_EVENT_BY_RECAL);                
            }

            if(u4VRMPassCnt > 0xFF)
            {
                ASSERT(0);                                    
            }

			#if defined(CC_MT5399)
			vVRMSetMonOutEventFlg(VRM_MON_OUT_IN_SEL_CHG);
			#endif

        }while(VRM_EVENT_NONE !=vVRMGetEventFlg(bPath));

        vVRMFireResolutionChange(bPath);

        if (B2R_TRIG_START == _eVRMWaitForB2RTrigger[bPath])
        {
            _eVRMWaitForB2RTrigger[bPath] = B2R_TRIG_WAITING;
            LOG(1, "VRM(%d) VRMWaitForB2RTrigger B2R_TRIG_WAITING\n", bPath);   
        }
    }
              
#ifdef VRM_CHK_SET_RES_STATUS
    vVRMChkSetResStatus(bPath);
#endif
}

void vVrmAllImportEn(UINT8 bPath, UINT8 bOnOff)
{
    vPDSImportEn(bPath, bOnOff);
    vRegenSetResolutionBypass(bPath,REGEN_MODULE_PRE_REGEN, !bOnOff);
    vDrvDISetResMode(bPath, bOnOff);
    vRegenSetResolutionBypass(bPath,REGEN_MODULE_FSC_REGEN, !bOnOff);
    vScpipSetImportEnable(bPath, bOnOff);
}


void vVrmOnVdoModeChange(UINT8 bPath)
{
    vVRMSetEventFlg(bPath, VRM_EVENT_BY_VDO_MODCHG);
}

void vVrmOnVdoModeChangeDone(UINT8 bPath)
{
    vVRMSetEventFlg(bPath, VRM_EVENT_BY_VDO_MODCHG);
}

/***************************/
/** For Set resolutin ******/
/***************************/

//1. Function to set import trigger module
void vDrvSetImportTriggerModule(UINT8 bPath, IMPORT_MODULE_E module)
{
    _importInfo[bPath].importModule = module;

    vDrvPDSClrSetRes(bPath, SET_RES_BOTH);
    vDrvRegenClrSetRes(bPath,REGEN_MODULE_PRE_REGEN, SET_RES_BOTH);
    vDrvDIClrSetRes(bPath, SET_RES_BOTH);
    vDrvRegenClrSetRes(bPath,REGEN_MODULE_FSC_REGEN, SET_RES_BOTH);
    vDrvScpipClrSetRes(bPath, SET_RES_BOTH);
    
    LOG(3, "VRM(%d) Set Resolution Module is Set To (%d)\n", bPath, module);
    if (_importInfo[bPath].importModule == IMPORT_MODULE_NONE)
    {
        vVrmAllImportEn(bPath, SV_OFF);
        //vScpipSetTgImportOnOff(SV_OFF);
        _eVRMWaitForB2RTrigger[bPath] = B2R_TRIG_OFF;
        LOG(1, "VRM(%d) VRMWaitForB2RTrigger B2R_TRIG_OFF\n", bPath);
    }
    else
    {
        if (_importInfo[bPath].importModule == IMPORT_MODULE_B2R)
        {
            _eVRMWaitForB2RTrigger[bPath] = B2R_TRIG_START;
            LOG(1, "VRM(%d) VRMWaitForB2RTrigger B2R_TRIG_START\n", bPath);
        }    
        vVrmAllImportEn(bPath, SV_ON);
        vScpipSetTgImportOnOff(SV_ON);
    }
}
   
//2. Function to get import trigger module
IMPORT_MODULE_E vDrvGetImportTriggerModule(UINT8 bPath)
{
    return _importInfo[bPath].importModule;
}

void vDrvSetForcedImportTriggerModule(UINT8 bPath, IMPORT_MODULE_E module)
{
    _importInfo[bPath].forcedImportModule = module;
}

IMPORT_MODULE_E eDrvGetForcedImportTriggerModule(UINT8 bPath)
{
    return _importInfo[bPath].forcedImportModule;
}
     
//3. Function to trigger import
void vDrvFireB2RImportTrigger(UINT8 bPath)
{

}

void vDrvFirePDSImportTrigger(UINT8 bPath)
{
    vPDSImportTrigger(bPath);
}

void vDrvFireRegenImportTrigger(UINT8 bPath)
{
    vRegenSetImport(bPath,REGEN_MODULE_PRE_REGEN);
}

void vDrvFireMIBImportTrigger(UINT8 bPath)
{

}

void vDrvFireFSCImportTrigger(UINT8 bPath)
{
    vScpipSetImportOnOff(bPath, SV_OFF);
    vScpipSetImportOnOff(bPath, SV_ON);
}

/// workaround for predown worng set resolution signal
static UINT8 u1PDTTrigger[VDP_NS]; 
void vDrvSetPDSTriggerFlg(UINT8 u1Path, UINT8 fgOnOff)
{
    CRIT_STATE_T csState;
    csState = x_crit_start();
    u1PDTTrigger[u1Path] = fgOnOff;
    x_crit_end(csState);
}
UINT8 vDrvQueryPDSTriggerFlg(UINT8 u1Path)
{
    return u1PDTTrigger[u1Path];
}
  
void vDrvFireImportTrigger(UINT8 bPath)
{
    IMPORT_MODULE_E module;

    module = vDrvGetImportTriggerModule(bPath);
    LOG(1, "vDrvFireImportTrigger %d ==> %d\n", bPath, module);
    
    switch(module)
    {
        case IMPORT_MODULE_NONE:
            break;
        case IMPORT_MODULE_B2R:
            //vDrvFireB2RImportTrigger(bPath);
            //VDP_NfySetResolutionReady(bPath, TRUE);
            break;
        case IMPORT_MODULE_PDS:
            vDrvFirePDSImportTrigger(bPath);                 //mustang fix this hw bug
            //vDrvSetPDSTriggerFlg(bPath, TRUE);
            //when b2r repeats field, set resolution signal cannot pass MIB.
            // but this may also cause transition noise.
            /*if (bDrvDIRepeatFldFound(bPath))
            {
                vDrvFireFSCImportTrigger(bPath);
            }*/
            break;
        case IMPORT_MODULE_REGEN:
            vDrvFireRegenImportTrigger(bPath);
            break;
        case IMPORT_MODULE_MIB:
            vDrvFireMIBImportTrigger(bPath);
            break;
        case IMPORT_MODULE_FSC:
            vDrvFireFSCImportTrigger(bPath);
            break;
        default:
            break;
    };
}
    
//4. Function to set import flag
void vDrvSetImportWaitFlag(UINT8 bPath, UINT32 u4Flag)
{
    CRIT_STATE_T csState;
    csState = x_crit_start();
    _importInfo[bPath].importWaitFlag |= (u4Flag);
    _importInfo[bPath].importWaitFlag |= VRM_IMPORT_WAIT_FLAG_TRIG_START;
    x_crit_end(csState);
    return;
}
   
//5. Function to clear import flag (clear when module finish reg setting)
void vDrvClrImportWaitFlag(UINT8 bPath, UINT32 u4Flag)
{
    CRIT_STATE_T csState;
    csState = x_crit_start();
    _importInfo[bPath].importWaitFlag &= (~(u4Flag));
    x_crit_end(csState);
    return;
}
   
//6. Function to get import flag
UINT32 vDrvGetImportWaitFlag(UINT8 bPath)
{
    return _importInfo[bPath].importWaitFlag;
}

//1. Function to set application data path stage
void vDrvVrmSetAppStage(void)
{
    UINT8 u1Sel;
    UINT16 u2Width, u2Height, u2Scan;
    VRM_INFO_T* vrmInfo;
    MonOutPicInfo* pMonOutInfo;

    if (u4DrvVrmGetAppFlag() & VRM_APP_GRAPHIC_DUMP)
    {
        u2Width = u2Height = u2Scan = 0;
        
        u1Sel = u1Scpip_GFX_GetSource();
        LOG(5, "VRM_APP_GRAPHIC_DUMP %d\n", u1Sel);
        switch (u1Sel)
        {
        case GFX_MAIN_SCL:
            vrmInfo = VRMGetVRMInfo(SV_VP_MAIN, VRM_MODULE_FSC);
            u2Width  = vrmInfo->u2Width;
            u2Height = vrmInfo->u2Height;
            u2Scan   = vrmInfo->u2ScanMode;
            break;
        case GFX_SUB_SCL:
            vrmInfo = VRMGetVRMInfo(SV_VP_PIP,  VRM_MODULE_FSC);
            u2Width  = vrmInfo->u2Width;
            u2Height = vrmInfo->u2Height;
            u2Scan   = vrmInfo->u2ScanMode;
            break;
        case GFX_MAIN_DI:
            vrmInfo = VRMGetVRMInfo(SV_VP_MAIN, VRM_MODULE_MIB);
            u2Width  = vrmInfo->u2Width;
            u2Height = vrmInfo->u2Height;
            u2Scan   = vrmInfo->u2ScanMode;
            break;
        case GFX_SUB_DI:
            vrmInfo = VRMGetVRMInfo(SV_VP_PIP,  VRM_MODULE_MIB);
            u2Width  = vrmInfo->u2Width;
            u2Height = vrmInfo->u2Height;
            u2Scan   = vrmInfo->u2ScanMode;
            break;
        case GFX_MAIN_PDS:
            vrmInfo = VRMGetVRMInfo(SV_VP_MAIN, VRM_MODULE_TTT);
            u2Width  = vrmInfo->u2Width;
            u2Height = vrmInfo->u2Height;
            u2Scan   = vrmInfo->u2ScanMode;
            break;
        case GFX_SUB_PDS:
            vrmInfo = VRMGetVRMInfo(SV_VP_PIP,  VRM_MODULE_TTT);
            u2Width  = vrmInfo->u2Width;
            u2Height = vrmInfo->u2Height;
            u2Scan   = vrmInfo->u2ScanMode;
            break;
        case GFX_MON_OUT:
            pMonOutInfo = getMonOutInfo();
            u2Width  = pMonOutInfo->wPdsOutWidth;
            u2Height = pMonOutInfo->wPdsOutHeight;
            u2Scan   = pMonOutInfo->u1Interlace;
            break;
        default:
            break;
        }

        if (u2Scan == 1) // interlace
        {
            u2Height /= 2;
        }

        if (u2Width != u4Scpip_GFX_GetWidth() || u2Height != u4Scpip_GFX_GetHeight())
        {
            u1Scpip_GFX_Update_Resolution(u2Width, u2Height);
            LOG(1, "GFX update resolution %d x %d\n", u2Width, u2Height);
        }
    }    

    #ifdef CC_SUPPORT_VENC
    if(u4DrvVrmGetAppFlag() & VRM_APP_VIDEO_ENC)
    {
        u2Width = u2Height = u2Scan = 0;
        
        u1Sel = u1DrvScpipH264GetSource();
        LOG(5, "VRM_APP_VIDEO_ENC u1Sel: %d\n", u1Sel);

        switch (u1Sel)
        {
        case VRM_PATH_MAIN:
            vrmInfo = VRMGetVRMInfo(SV_VP_MAIN, VRM_MODULE_FSC);
            u2Width  = vrmInfo->u2Width;
            u2Height = vrmInfo->u2Height;
            u2Scan   = vrmInfo->u2ScanMode;
            break;
        case VRM_PATH_SUB:
            #ifdef VENC_SUB_SCALER_SCALING
            vrmInfo = VRMGetVRMInfo(SV_VP_PIP,  VRM_MODULE_MIXER);
            u2Width  = vrmInfo->u2Width;
            u2Height = vrmInfo->u2Height;
            u2Scan   = vrmInfo->u2ScanMode;
            #else
            vrmInfo = VRMGetVRMInfo(SV_VP_PIP,  VRM_MODULE_FSC);
            u2Width  = vrmInfo->u2Width;
            u2Height = vrmInfo->u2Height;
            u2Scan   = vrmInfo->u2ScanMode;
            #endif
            break;
        case VRM_PATH_MON:
            pMonOutInfo = getMonOutInfo();
            u2Width  = pMonOutInfo->wPdsOutWidth;
            u2Height = pMonOutInfo->wPdsOutHeight;
            u2Scan   = pMonOutInfo->u1Interlace;
            break;
        default:
            break;
        }

        if(u2Scan == 1)
        {
            if(u1DrvScpipGetVencModule()== E_VENC_MODULE_SUB_SCALER && u1DrvScpipGetVencRecordMode() == E_SCALER_VENC_BOTH_FLD)
            {
               u2Height *= 2;    //because di bypass,so vrm get height is half
            }
            else if(u1DrvScpipGetVencModule()== E_VENC_MODULE_MON_SCALER && (u1DrvScpipGetVencRecordMode() == E_SCALER_VENC_TOP_FLD || 
                     u1DrvScpipGetVencRecordMode() == E_SCALER_VENC_BOT_FLD))
            {
               u2Height /= 2;   //because set target height * 2
            }
        }

        if (u2Width != u4DrvScpipH264GetWidth() || u2Height != u4DrvScpipH264GetHeight() || u2Scan != u4DrvScpipH264IsInterlace())
        {
            u1DrvScpipH264UpdateResolution(u2Width, u2Height, u2Scan);
            LOG(0, "VENC update resolution %d x %d %c\n", u2Width, u2Height, u2Scan?'I':'P');
        }
    }
    #endif
    return;
}

//2. Function to set application output window
void vDrvVrmSetAppWindow(UINT8 bVRMPath, UINT32 u4Flag)
{
    return;
}

//1. Function to set import flag
void vDrvVrmSetAppFlag(UINT32 u4Flag)
{
    CRIT_STATE_T csState;
    if (!bDrvVrmIsAppFlagSet(u4Flag))
    {
        vVRMSetEventFlg(VDP_2, VRM_EVENT_BY_APP);
    }
    csState = x_crit_start();
    _u4VRMAppFlag |= u4Flag;
    x_crit_end(csState);
    return;
}

//2. Function to clear App flag (clear when the app is disabled)
void vDrvVrmClrAppFlag(UINT32 u4Flag)
{
    CRIT_STATE_T csState;
    if (bDrvVrmIsAppFlagSet(u4Flag))
    {
        vVRMSetEventFlg(VDP_2, VRM_EVENT_BY_APP);
    }
    csState = x_crit_start();
    _u4VRMAppFlag &= (~(u4Flag));  
    x_crit_end(csState);
    return;
}

//3. Function to query App flag 
BOOL bDrvVrmIsAppFlagSet(UINT32 u4Flag)
{
    return ((_u4VRMAppFlag & u4Flag) > 0) ? TRUE : FALSE;
}
//4. Function to query App flag 
UINT32 u4DrvVrmGetAppFlag(void)
{
    return _u4VRMAppFlag;
}
//5. Check Is there FiFo/Dram limit for App
BOOL bDrvVrmIsAppFifoDramLimit(UINT8 bPath)
{
	if (bPath == VDP_2)
	{
		if(u1DrvDIIsBypass(VDP_2))
		{
			return FALSE;
		}
		else if (bDrvVrmIsAppFlagSet(VRM_APP_POP_PIP) ||
				 bDrvVrmIsAppFlagSet(VRM_APP_SCART_OUT)||
				 (bDrvVrmIsAppFlagSet(VRM_APP_VIDEO_ENC) && (u1DrvScpipGetVencModule() == E_VENC_MODULE_SUB_SCALER))
				 )
		{
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}
	else
	{
		return TRUE;
	}
}

void vVrmSetForcedOvsnModule(UINT8 bPath, FORCED_OVSN_MODULE_E ovsnModule)
{
    if(bPath > SV_VP_PIP)
    {
        return;
    }
    
    _forcedOvsnModule[bPath] = ovsnModule;
    fireVRMModeChange(bPath, VRM_EVENT_BY_CLI);
}

OVSN_MODULE_E eVrmGetOvsnModule(UINT8 bPath)
{
    return _ovsnModule[bPath];
}

void vVrmUpdateOvsnModule(UINT8 bPath)
{
    OVSN_MODULE_E ovsnModule = OVSN_MODULE_REGEN;
    
    if(bPath <= SV_VP_PIP)
    {
        if(_forcedOvsnModule[bPath] == FORCED_OVSN_MODULE_AUTO)
        {
            if(getScalerMode(bPath) == VDP_SCPOS_DISPMODE_OFF)
            {
               ovsnModule = OVSN_MODULE_FSC_REGEN;
            }
            else
            {
                ovsnModule = OVSN_MODULE_REGEN;
            }
            
        }
        else if(_forcedOvsnModule[bPath] == FORCED_OVSN_MODULE_REGEN)
        {
            ovsnModule = OVSN_MODULE_REGEN;
        }
        else
        {
            ovsnModule = OVSN_MODULE_FSC_REGEN;
        }
        
        _ovsnModule[bPath] = ovsnModule;
    }    
}

//6. Venc Resolution
void vDrvVrmSetAppRes(UINT32 u4AppType, APP_RES_T rVencRes)
{
    UINT8 u1Sel = u1DrvScpipGetVencModule();
  
  if(u4AppType == VRM_APP_VIDEO_ENC && 
      x_memcmp((const void *)&_rVencRes,(const void *)&rVencRes,sizeof(APP_RES_T))
     )
    {
        _rVencRes = rVencRes;
        if(u1Sel == E_VENC_MODULE_MAIN_SCALER)
        {           
            vVRMSetEventFlg(VDP_1, VRM_EVENT_BY_APP);
        }
        else if(u1Sel == E_VENC_MODULE_SUB_SCALER || u1Sel == E_VENC_MODULE_SUB_DI)
        {
            vVRMSetEventFlg(VDP_2, VRM_EVENT_BY_APP);
        }
        else if(u1Sel == E_VENC_MODULE_MON_SCALER)
        {
            vMonOutSetOutputRes(_rVencRes.u4Width,_rVencRes.u4Height);
        }
    }
}

 
//7. Get Venc Resolution
void vDrvVrmGetAppRes(UINT32 u4AppType, APP_RES_T *rVencRes)
{
    if (u4AppType == VRM_APP_VIDEO_ENC)
    {
        *rVencRes = _rVencRes;
    }
}

/******B2R interface**************/
/***It's for B2R to query whether VRM is ready.**/
UINT8 bVRMReadyForB2R(UINT8 bPath)
{
    if (IMPORT_MODULE_NONE    == _importInfo[bPath].importModule &&
    	 FALSE == fgVRMIsEventFlgSet(bPath, VRM_EVENT_BY_B2R))
    {
        return TRUE;
    }
    else if(B2R_TRIG_WAITING                == _eVRMWaitForB2RTrigger[bPath] && 
               VRM_IMPORT_WAIT_FLAG_TRIG_START == vDrvGetImportWaitFlag(bPath))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}
/***B2R tells VRM it already triggered.**********/
void vVRMB2RTrigger(UINT8 bPath)
{
    _eVRMWaitForB2RTrigger[bPath] = B2R_TRIG_OFF;
    LOG(1, "VRM(%d) VRMWaitForB2RTrigger B2R_TRIG_OFF\n", bPath);
}

UINT8 bVRMUpdatingSrcRegDone(UINT8 bPath)
{
       if (bVRMUpdateSrcReg[bPath])
       {
       	bVRMUpdateSrcReg[bPath] = FALSE;
       	return TRUE;
       }
       else
       {
       	return FALSE;
       }
}
void vDrvVRMResume(void)
{
    memset(&_vrm_info,0,sizeof(_vrm_info));
}


