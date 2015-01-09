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
 * $RCSfile: vdp_venc_ctrl.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

#ifdef CC_SUPPORT_VENC

#include "srm_drvif.h"
#include "drv_di.h"
#include "vdp_venc_ctrl.h"
#include "drv_di.h"
#include "drv_scaler.h"
#include "vdo_rm.h"
#include "vdp_if.h"
#include "drv_mon_out.h"
#include "venc_if.h"
#include "drv_video.h"
#include "vdo_misc.h"
#include "nptv_debug.h"
#include "x_assert.h"


#define _VENC_MAX_WIDTH  (1920)
#define _VENC_MAX_HEIGHT (1080)

extern void vDrvScpipSupportH264Init(void);
extern void vDrvScpipSetVEncHandle(void *handle, void *ptFrmBuffMgr, void *ptFrmBuffMgrResize, void *ptFilterPts);
extern void vDrvScpipStopSendBufToVENC(BOOL bFlag);
extern void vVdoSendEvevntToSRM(UINT8 bPath);
extern void vVRMUpdateVdpInforForMonOut( UINT8 u1MonOutMuxIn );


VDP_VENC_INFO_t           tVencInfo;
VDP_VENC_INPUT_RES_t      tVencRes;

static UINT8          _u1WaitForVRM       = FALSE;
static UINT8          _u1WaitForSubSig    = FALSE;
static UINT8          u1VRMStableCounter  = 0;



static HANDLE_T       _hVencVDPCtrl;
static HANDLE_T       _hVencVDPWaitForVRM;
static HANDLE_T       _hVencVDPWaitForSubSig;
///// vdp related static function /////////


extern  BOOL bEncInputDbg ;
extern  BOOL bEncFldModeDbg ;
extern  UINT8 eEncInputDbg ;
extern  UINT8 eEncFldTypeDbg;
extern  UINT8 _u1tvmode_venc;



static void vDrvVencVDPEnableWaitForVRM(UINT8 fgEnable)
{
    CRIT_STATE_T csState;
    csState = x_crit_start();
    _u1WaitForVRM = fgEnable;
	u1VRMStableCounter = 0;
    x_crit_end(csState);
}

static UINT8 u1DrvVencVDPIsWaitingForVRM(void)
{
    return _u1WaitForVRM;
}

static void vDrvVencVDPEnableWaitForSubSig(UINT8 fgEnable)
{
    CRIT_STATE_T csState;
    csState = x_crit_start();
    _u1WaitForSubSig = fgEnable;
    x_crit_end(csState);
}

static UINT8 u1DrvVencVDPIsWaitingForSubSig(void)
{
    return _u1WaitForSubSig;
}

static void vDrvVencCtrlConfig(VENC_PROFILE_T eProfileDef)
{
    // do configuration here, it's better to do it by APP
    if (VENC_PROFILE_PVR  == eProfileDef)
    {
        tVencInfo.eEncModule    = E_VDP_VENC_SUB_SCALER;
        tVencInfo.eEncFldType   = E_VDP_VENC_PROGRESSIVE;

        // special case, atvpvr wants to record the input resolution  need AP to modify it.
        tVencInfo.u4TrgtWidth = _VENC_MAX_WIDTH;
        tVencInfo.u4TrgtHeight= _VENC_MAX_HEIGHT;
    }
    else if (VENC_PROFILE_CUST == eProfileDef)
    {
        tVencInfo.eEncModule   = E_VDP_VENC_SUB_SCALER;
        
        // special case, atvpvr wants to record the input resolution  need AP to modify it.
        tVencInfo.u4TrgtWidth  = (tVencInfo.u4TrgtWidth  == 0) ? _VENC_MAX_WIDTH  : tVencInfo.u4TrgtWidth;
        tVencInfo.u4TrgtHeight = (tVencInfo.u4TrgtHeight == 0) ? _VENC_MAX_HEIGHT : tVencInfo.u4TrgtHeight;
    }
    else // current msi
    {
    	#if defined(CC_MT5882)
			tVencInfo.eEncModule   = E_VDP_VENC_SUB_SCALER;
			tVencInfo.eEncFldType  = E_VDP_VENC_PROGRESSIVE;
		#else
			tVencInfo.eEncModule   = E_VDP_VENC_MONITOR_OUT;
		#endif   
    }

    if(bEncFldModeDbg == TRUE)
    {
        tVencInfo.eEncFldType = (E_VDP_VENC_FIELD_MODE)eEncFldTypeDbg;
    }

    if(bEncInputDbg == TRUE)
    {
        tVencInfo.eEncInput  = (E_VDP_VENC_INPUT)eEncInputDbg;
    }
        
    u1DrvScpipSetVencModule((UINT8)tVencInfo.eEncModule);   
       
    LOG(1 ,"[VDP_VENC]- eEncInput:%d  eEncModule:%d eEncFldType %d\n", tVencInfo.eEncInput, tVencInfo.eEncModule, tVencInfo.eEncFldType);
    
}

static void inline vDrvVencSetFldMode(void)
{
    if (tVencInfo.eEncModule == E_VDP_VENC_SUB_DI)
    {
        if (!tVencRes.fgInterlace)
        {
            vDrvDIVencFldRecordMode(E_DI_VENC_NORMAL);
        }
        else
        {
            switch (tVencInfo.eEncFldType)
            {
                case E_VDP_VENC_TOP_FLD: 
                    vDrvDIVencFldRecordMode(E_DI_VENC_TOP_FLD);
                    break;
                case E_VDP_VENC_BOTTOM_FLD:
                    vDrvDIVencFldRecordMode(E_DI_VENC_BOT_FLD);
                    break;
                default: //E_VDP_VENC_ENC_BOTH_FLD
                    vDrvDIVencFldRecordMode(E_DI_VENC_NORMAL);
                    break;
            }
        }           
    }

    else if(tVencInfo.eEncModule == E_VDP_VENC_SUB_SCALER)
    {
        if (!tVencRes.fgInterlace)
        {
            vDrvScpipVencFldRecordMode(E_SCALER_VENC_PROGRESSIVE);
        }
        else
        {
            switch (tVencInfo.eEncFldType)
            {
                case E_VDP_VENC_TOP_FLD: 
                    vDrvScpipVencFldRecordMode(E_SCALER_VENC_TOP_FLD);
                    break;
                case E_VDP_VENC_BOTTOM_FLD:
                    vDrvScpipVencFldRecordMode(E_SCALER_VENC_BOT_FLD);
                    break;
                default: //E_VDP_VENC_ENC_BOTH_FLD
                    vDrvScpipVencFldRecordMode(E_SCALER_VENC_BOTH_FLD);
                    break;
            }
        }
    }
    else if(tVencInfo.eEncModule == E_VDP_VENC_MONITOR_OUT)
    {
        if (!tVencRes.fgInterlace)
        {
            vDrvScpipVencFldRecordMode(E_SCALER_VENC_PROGRESSIVE);
        }
        else
        {
            switch (tVencInfo.eEncFldType)
            {
                case E_VDP_VENC_TOP_FLD: 
                    vDrvScpipVencFldRecordMode(E_SCALER_VENC_TOP_FLD);
                    break;
                case E_VDP_VENC_BOTTOM_FLD:
                    vDrvScpipVencFldRecordMode(E_SCALER_VENC_BOT_FLD);
                    break;
                default: //E_VDP_VENC_ENC_BOTH_FLD
                    vDrvScpipVencFldRecordMode(E_SCALER_VENC_BOTH_FLD);
                    break;
            }  
        }    
    }

}


static void vDrvVencCalVencInRes(void)
{
    LOG(1,"[VDP_VENC]- Start to wait for VRM\n");
    VERIFY (x_sema_lock(_hVencVDPWaitForVRM, X_SEMA_OPTION_WAIT) == OSR_OK);
    vDrvVencVDPEnableWaitForVRM(TRUE);    
    //wait for vrm ready
    VERIFY (x_sema_lock(_hVencVDPWaitForVRM, X_SEMA_OPTION_WAIT) == OSR_OK);
    x_sema_unlock(_hVencVDPWaitForVRM);
    LOG(1,"[VDP_VENC]- Finish waiting \n");   
    
    if (tVencInfo.eEncModule == E_VDP_VENC_MONITOR_OUT)
    {
        MonOutPicInfo* pMonOutInfo;
	    pMonOutInfo = getMonOutInfo();

	    tVencRes.u4EncInWidth   = (UINT32)pMonOutInfo->wPdsOutWidth;
        tVencRes.u4EncInHeight  = (UINT32)pMonOutInfo->wPdsOutHeight; 
        tVencRes.u4Fps          = (UINT32)pMonOutInfo->u2OutFramerate;
        tVencRes.fgInterlace    = pMonOutInfo->u1Interlace;  
    
    }
    else if(tVencInfo.eEncModule == E_VDP_VENC_SUB_SCALER)
    {
        VRM_INFO_T*  vrmInfo; 
        VRM_MODULE_E eVrmModule;
        SV_VP_T      eVDP;
    
        switch (tVencInfo.eSubEncInput)
        {          
            case E_SUB_VENC_MAIN_BEFORE_PRED:
                eVrmModule = VRM_MODULE_PDS;
                eVDP = SV_VP_MAIN;
                break; 
            case E_SUB_VENC_SUB_BEFORE_PRED:
                eVrmModule = VRM_MODULE_PDS;
                eVDP = SV_VP_PIP;
                break;    
            case E_SUB_VENC_OSTG:
            case E_SUB_VENC_OSTG_OSD:
                eVrmModule = VRM_MODULE_OSTG;
                eVDP = SV_VP_MAIN;
                break;
            case E_SUB_VENC_SUB_SCALER_IN:
            default:
                #ifdef VENC_SUB_SCALER_SCALING
                eVrmModule = VRM_MODULE_MIXER;
                eVDP = SV_VP_PIP;
                #else
                eVrmModule = VRM_MODULE_FSC;
                eVDP = SV_VP_PIP;
                #endif
                break;
        }

        vrmInfo = VRMGetVRMInfo(eVDP, eVrmModule);

        LOG(1 ,"[VDP_VENC] eVDP %d eVrmModule %d\n",eVDP, eVrmModule);

        tVencRes.u4EncInWidth     = (UINT32)vrmInfo->u2Width;
        tVencRes.u4EncInHeight    = (UINT32)vrmInfo->u2Height; 
        tVencRes.u4Fps            = (UINT32)vrmInfo->u2FrameRate;
        tVencRes.fgInterlace      = (UINT8) vrmInfo->u2ScanMode;  
    }

    if (tVencRes.fgInterlace && bDrvVencVDPIsOneFldMode() && tVencInfo.eEncModule != E_VDP_VENC_SUB_SCALER)
    {
        tVencRes.u4EncInHeight >>= 1;
    }

    LOG(1 ,"[VDP_VENC]- eEncInput: %d EncInHeight: %d u4Fps:%d\n", tVencRes.u4EncInWidth, tVencRes.u4EncInHeight, tVencRes.u4Fps);
    LOG(1 ,"[VDP_VENC]- TgtWidth:  %d TgtHeight:   %d fgInterlace:%d\n", tVencInfo.u4TrgtWidth, tVencInfo.u4TrgtHeight, tVencRes.fgInterlace);

}


static void inline vDrvVencSetModuleInput(void)
{
    if (tVencInfo.eEncModule == E_VDP_VENC_SUB_SCALER)
    {
        u1DrvScpipH264SourceSel(tVencInfo.eEncModule); 
    }
    else if (tVencInfo.eEncModule == E_VDP_VENC_MONITOR_OUT)
    {
        switch (tVencInfo.eEncInput)
        {
           case E_VDP_VENC_MIXER:
                vMonOutSetOSDSelIn(E_MON_OUT_WITH_NO_OSD); 
                u1DrvScpipH264Input(MON_SIGNAL_FROM_MIXER);  
                vMonOutSetMuxSelIn(E_MON_OUT_MIXER);       // Outstage
                vVRMUpdateVdpInforForMonOut(E_MON_OUT_MIXER);
                break;
            case E_VDP_VENC_MAIN_FSC_OUT:                 // MAIN Scaler out
                u1DrvScpipH264Input(MON_SIGNAL_FROM_MAIN);
                vMonOutSetMuxSelIn(E_MON_OUT_MAIN_FSC_OUT); 
                vVRMUpdateVdpInforForMonOut(E_MON_OUT_MAIN_FSC_OUT);
                break;
            case E_VDP_VENC_SUB_FSC_OUT:                   // SUB Scaler out
                u1DrvScpipH264Input(MON_SIGNAL_FROM_SUB);
                vMonOutSetMuxSelIn(E_MON_OUT_SUB_FSC_OUT); 
                vVRMUpdateVdpInforForMonOut(E_MON_OUT_SUB_FSC_OUT);
                break;
            case E_VDP_VENC_MAIN_MIB_OUT:                 // main DI out
                u1DrvScpipH264Input(MON_SIGNAL_FROM_MAIN);
                vMonOutSetMuxSelIn(E_MON_OUT_MAIN_MIB_OUT); 
                vVRMUpdateVdpInforForMonOut(E_MON_OUT_MAIN_MIB_OUT);
                break;
            case E_VDP_VENC_SUB_MIB_OUT:                   // sub DI out
                u1DrvScpipH264Input(MON_SIGNAL_FROM_SUB);
                vMonOutSetMuxSelIn(E_MON_OUT_SUB_MIB_OUT); 
                vVRMUpdateVdpInforForMonOut(E_MON_OUT_SUB_MIB_OUT);
                break;
            case E_VDP_VENC_MAIN_BEFORE_PRED:              // MAIN before predown
                u1DrvScpipH264Input(MON_SIGNAL_FROM_MAIN); 
                vMonOutSetMuxSelIn(E_MON_OUT_MAIN_PREDOWN);                   
                vVRMUpdateVdpInforForMonOut(E_MON_OUT_MAIN_PREDOWN);
                break;
            case E_VDP_VENC_SUB_BEFORE_PRED:                // sub before predown
                u1DrvScpipH264Input(MON_SIGNAL_FROM_SUB);
                vMonOutSetMuxSelIn(E_MON_OUT_SUB_PREDOWN); 
                vVRMUpdateVdpInforForMonOut(E_MON_OUT_SUB_PREDOWN);
                break;
            case E_VDP_VENC_OSTG_OSD:
                vMonOutSetOSDSelIn(E_MON_OUT_WITH_ALL_OSD); 
                u1DrvScpipH264Input(MON_SIGNAL_FROM_OSTG_OSD);  
                vMonOutSetMuxSelIn(E_MON_OUT_PANEL);       // Outstage
                vVRMUpdateVdpInforForMonOut(E_MON_OUT_PANEL);
                break;
            case E_VDP_VENC_OSTG:
                vMonOutSetOSDSelIn(E_MON_OUT_WITH_NO_OSD);  
                u1DrvScpipH264Input(MON_SIGNAL_FROM_OSTG);
                vMonOutSetMuxSelIn(E_MON_OUT_PANEL);       // Outstage
                vVRMUpdateVdpInforForMonOut(E_MON_OUT_PANEL);
                break;
            default:    
                vMonOutSetOSDSelIn(E_MON_OUT_WITH_ALL_OSD);  
                u1DrvScpipH264Input(MON_SIGNAL_FROM_OSTG);
                vMonOutSetMuxSelIn(E_MON_OUT_PANEL);       // Outstage
                vVRMUpdateVdpInforForMonOut(E_MON_OUT_PANEL);
                break;
        }  
        
        u1DrvScpipH264SourceSel(tVencInfo.eEncModule);
    }
         
}

void vDrvVencCtrlStopModule(void)
{
    if (tVencInfo.eEncModule == E_VDP_VENC_SUB_DI)
    {
        vDrvDIStopSendBufToVENC(TRUE);
    }
    else
    {
        LOG(1 ,"[VDP_VENC]- VENC Disable scaler Config !\n");
        u1DrvScpipH264ReadyMode(0); //ready mode
        u1DrvScpipH264Enable(0); //enable        
        vDrvScpipStopSendBufToVENC(TRUE);
        u1DrvScpipSetVencModule(E_VENC_MODULE_MAX);
        vDrvVencVDPSetEncInput(E_VDP_VENC_NOT_SELECTED);
        if(tVencInfo.eEncModule == E_VDP_VENC_MONITOR_OUT)
        {
            vMonOutSetOutput(E_MON_OUT_GFX);
        }
    }
}

static inline void vDrvVencVDPSubSetting(void)
{
    if (tVencInfo.eEncModule == E_VDP_VENC_SUB_SCALER  && 
        tVencInfo.eSubEncInput  == E_SUB_VENC_SUB_SCALER_IN)
    {
        if (SV_VDO_STABLE != bDrvVideoSignalStatus(VDP_2)) //signal not ready, need to wait for it.
        {
            LOG(1,"[VDP_VENC]- Start to wait for signal stable \n");
            VERIFY (x_sema_lock(_hVencVDPWaitForSubSig, X_SEMA_OPTION_WAIT) == OSR_OK);
            vDrvVencVDPEnableWaitForSubSig(TRUE);    
            //wait for vrm ready
            VERIFY (x_sema_lock(_hVencVDPWaitForSubSig, X_SEMA_OPTION_WAIT) == OSR_OK);
            x_sema_unlock(_hVencVDPWaitForSubSig);
            LOG(1,"[VDP_VENC]- Finish waiting for Signal Stable \n");   
        }
        LOG(1,"[VDP_VENC]- Sub Path Signal Stable \n");   

        if (TRUE == bDrvVideoIsSrcInterlace(VDP_2))
        {
            if (E_VDP_VENC_PROGRESSIVE == tVencInfo.eEncFldType)
            {
            #if defined(CC_MT5882)  //Oryx Gazelle sub DI fifo 1920
                if (MIN((UINT32)wDrvVideoInputWidth(VDP_2), tVencInfo.u4TrgtWidth) > 1280) //Capri sub DI fifo 1280
                {
                    tVencInfo.eEncFldType = E_VDP_VENC_TOP_FLD;
                }
                else 
            #else
		    #if defined(CC_MT5399)  //mustang sub DI fifo pip=720
                if (MIN((UINT32)wDrvVideoInputWidth(VDP_2), tVencInfo.u4TrgtWidth) > 720) 
                {
                    tVencInfo.eEncFldType = E_VDP_VENC_TOP_FLD;
                }
                else 
            #endif
			#endif
                if (SRM_TV_MODE_TYPE_PIP != SRM_GetTvMode())
                {
                    _u1tvmode_venc = 1;
                    vVdoSendEvevntToSRM(SV_VP_PIP) ;
                }
            }

            if (E_VDP_VENC_PROGRESSIVE != tVencInfo.eEncFldType)
            {
                u1DrvDIForceBypass(VDP_2, TRUE);
            }
    
        }

    }
}

UINT8 bDrvVencVDPIsOneFldMode(void)
{
    if (tVencInfo.eEncFldType == E_VDP_VENC_TOP_FLD||
        tVencInfo.eEncFldType == E_VDP_VENC_BOTTOM_FLD)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

UINT8 u1DrvVencVDPSignalReady(void)
{
    if (E_VDP_VENC_MAIN_BEFORE_PRED == tVencInfo.eEncInput ||
        E_VDP_VENC_MAIN_MIB_OUT     == tVencInfo.eEncInput ||
        E_VDP_VENC_MAIN_FSC_OUT     == tVencInfo.eEncInput ||
        E_VDP_VENC_MIXER            == tVencInfo.eEncInput 
        )
    {
        if (bDrvVideoSignalStatus(VDP_1) != SV_VDO_STABLE)
        {
            LOG(1, "[VDP_VENC]- Main Path Signal not stable \n");
            return FALSE;
        }
        // Question may also need to check may's color space ???
        return TRUE;
    }
    else if (E_VDP_VENC_SUB_FSC_OUT     == tVencInfo.eEncInput    ||
             E_VDP_VENC_SUB_MIB_OUT     == tVencInfo.eEncInput    ||
             E_VDP_VENC_SUB_BEFORE_PRED == tVencInfo.eEncInput    ||
             E_SUB_VENC_SUB_SCALER_IN   == tVencInfo.eSubEncInput ||
             E_SUB_VENC_SUB_BEFORE_PRED == tVencInfo.eSubEncInput 
             )
    {
        if (bDrvVideoSignalStatus(VDP_2) != SV_VDO_STABLE)
        {
            LOG(1, "[VDP_VENC]- Sub Path Signal not stable \n");
            return FALSE;
        }
        // force modechange to make it always become 422
        if (bIsScalerInputRGB(VDP_2))
        {    
            vSetPipFlg(PIP_FLG_MODE_CHG);
            vSetPipFlg(PIP_FLG_MODE_DET_DONE);
            LOG(1, "[VDP_VENC]- 3x3 Matrix is not ready \n");
            return FALSE;
        }
        return TRUE;
    }
    else
    {
        return TRUE;
    }
}

//not sure about this part
UINT8 u1DrvVencVDPVRMReady(void)
{		   
	if (E_VDP_VENC_MONITOR_OUT == tVencInfo.eEncModule)
	{
		if (VRM_MON_OUT_EVENT_NONE == vVRMGetMonOutEventFlg())
		{
			u1VRMStableCounter ++;
			LOG(1 ,"[VDP_VENC]- Monitor is not ready Counter = %d\n", u1VRMStableCounter);
			if(u1VRMStableCounter >= 5)
			{
				u1VRMStableCounter = 0;
				return TRUE;
			}
			else
			{
				return FALSE;				
			}
		}
		else
		{
			LOG(1 ,"[VDP_VENC]- Monitor out VRM is not ready \n");
			u1VRMStableCounter = 0;
			return FALSE;
		}
	}
	else
	{
		RPicInfo* picInfo;
		UINT8 u1Path;
		
		switch (tVencInfo.eSubEncInput)
		{
			case E_SUB_VENC_MAIN_BEFORE_PRED:
			case E_SUB_VENC_OSTG:
			case E_SUB_VENC_OSTG_OSD:
				u1Path = VDP_1;
			case E_SUB_VENC_SUB_SCALER_IN:
			case E_SUB_VENC_SUB_BEFORE_PRED:
			default:
				u1Path = VDP_2;
		}
		
		picInfo = getPicInfo(u1Path);

		if (VRM_EVENT_NONE == vVRMGetEventFlg(u1Path))
		{
			u1VRMStableCounter ++;
			LOG(1 ,"[VDP_VENC]- Path %d VRM is not ready Counter = %d\n",u1Path, u1VRMStableCounter);
			if(u1VRMStableCounter >= 5)
			{
				u1VRMStableCounter = 0;
				return TRUE;
			}
			else
			{
				return FALSE;				
			}
		}
		else if(picInfo->wSrcW ==0 || picInfo->wTargetW == 0)
		{
			LOG(1 ,"[VDP_VENC]- Path %d VRM is not ready wSrcW == 0\n", u1Path);
			u1VRMStableCounter = 0;
			return FALSE;	
		}
		else
		{
			LOG(1 ,"[VDP_VENC]- Path %d VRM is not ready \n", u1Path);
			u1VRMStableCounter = 0;
			return FALSE;
		}
		
	}
}

void vDrvVencVDPSetScalerBuf(UINT32 u4BaseAdd, UINT32 u4YBufSize)
{    
    u1DrvScpipH264SetDramAddress(u4BaseAdd, u4YBufSize); 
    u1DrvScpipH264ReadyMode(1); //ready mode
    u1DrvScpipH264Enable(1); //enable
}


void vDrvVencVDPDropFrame(UINT8 u1OnOff, UINT8 u1M, UINT8 u1N)
{
    if (tVencInfo.eEncModule == E_VDP_VENC_SUB_DI)
    {
        vDrvDIVencDropFrame(u1OnOff);
        LOG(1 ,"[VDP_VENC]- DI Frame Drop %d!\n",u1OnOff);
    }
    else
    {
        vDrvScpipVencDropFrame(tVencInfo.eEncModule, u1OnOff, u1M, u1N); 
        LOG(1,"[VDP_VENC]- Scaler Frame Drop Enable:%d M:%d N:%d\n",u1OnOff, u1M, u1N);
    }
}

void vDrvVencVDPStart(void *handle, void *ptFrmBuffMgr, void *ptResizeFrmBuffMgr, void *ptFilterPts)
{
    VERIFY (x_sema_lock(_hVencVDPCtrl, X_SEMA_OPTION_WAIT) == OSR_OK);

    LOG(1,"[VDP_VENC]- vDrvVencVDPStart \n");
    switch (tVencInfo.eEncModule)
    {
        case E_VDP_VENC_SUB_DI:
            vDrvDISupportH264Init();
            vDrvDISetVEncHandle(handle, ptFrmBuffMgr, ptResizeFrmBuffMgr, ptFilterPts);
            break;
        case E_VDP_VENC_MONITOR_OUT:
        case E_VDP_VENC_SUB_SCALER:
        case E_VDP_VENC_MAIN_SCALER:
            vDrvScpipSupportH264Init();
            vDrvScpipSetVEncHandle(handle, ptFrmBuffMgr, ptResizeFrmBuffMgr, ptFilterPts);
            break;
    }

#ifdef CC_ATV_PVR_SUPPORT
    if (VENC_PROFILE_PVR == tVencInfo.eProfileDef)
    {
        // trigger VBI
        VBI_PVRStartNotifyCCOnOff(SV_VP_MAIN, TRUE);
    }
#endif
    // complete the whole start procedure unlock the semaphore
    x_sema_unlock(_hVencVDPCtrl);
}

void vDrvVencVDPStop()
{
    VERIFY (x_sema_lock(_hVencVDPCtrl, X_SEMA_OPTION_WAIT) == OSR_OK);

    LOG(1,"[VDP_VENC]- vDrvVencVDPStop \n");
#ifdef CC_ATV_PVR_SUPPORT
    if (VENC_PROFILE_PVR == tVencInfo.eProfileDef)
    {
        // stop VBI
        VBI_PVRStartNotifyCCOnOff(SV_VP_MAIN, FALSE);
    }
#endif

    vDrvVrmClrAppFlag(VRM_APP_VIDEO_ENC);
    vDrvVencCtrlStopModule();

    if (tVencInfo.eEncModule == E_VDP_VENC_SUB_SCALER  && 
        tVencInfo.eSubEncInput  == E_SUB_VENC_SUB_SCALER_IN)
    {
       _u1tvmode_venc = 0;
       vVdoSendEvevntToSRM(SV_VP_PIP) ;
       u1DrvDIForceBypass(VDP_2, FALSE);
    }
        
    x_sema_unlock(_hVencVDPCtrl);
}

void vDrvVencVDPSetParam(VENC_PROFILE_T eProfileDef, UINT32 u4RecWidth, UINT32 u4RecHeight)
{
    APP_RES_T rVencRes;

    LOG(1,"[VDP_VENC]- vDrvVencVDPSetParam \n");
    VERIFY (x_sema_lock(_hVencVDPCtrl, X_SEMA_OPTION_WAIT) == OSR_OK);

    tVencInfo.eProfileDef  = eProfileDef;
    tVencInfo.u4TrgtWidth  = u4RecWidth;
    tVencInfo.u4TrgtHeight = u4RecHeight;
    
    vDrvVencCtrlConfig(tVencInfo.eProfileDef);
    vDrvVrmSetAppFlag(VRM_APP_VIDEO_ENC);
    vDrvVencSetModuleInput();

    // w/h = 0  is a special case. it means it wants to record the input size
    rVencRes.u4Width  = tVencInfo.u4TrgtWidth;
    rVencRes.u4Height = tVencInfo.u4TrgtHeight;
    vDrvVrmSetAppRes(VRM_APP_VIDEO_ENC, rVencRes);

    vDrvVencVDPSubSetting(); // wait for sub signal ready and apply sub setting

    vDrvVencCalVencInRes(); // wait for vrm ready and get venc input resolution
    
    vDrvVencSetFldMode();

    x_sema_unlock(_hVencVDPCtrl);
}

/**
 * @brief  Check whether the frame rate is feasible for the write engine
 * @param  u4APFR  the frame rate AP sets
 * @param  u4InFR  video input frame/field rate
 * @return feasible frame rate for AP
 */
UINT32 u4DrvVencVDPCtrlCheckFrameRate(UINT32 u4APFR, UINT32 u4InFR)
{
    if (u4APFR > u4InFR || u4APFR == 0)
    {//AP frame rate is larger than input frame => not feasible, use input frame rate
        LOG(0,"[VDP_VENC]- AP FR %d / Input FR %d, AP frame rate is not feasible, use input frame rate instead\n", u4APFR, u4InFR);
        return u4InFR;
    }
    else if (E_VDP_VENC_SUB_DI == tVencInfo.eEncModule &&
             u4APFR != u4InFR && u4APFR != u4InFR/2)
    {
        LOG(1,"[VDP_VENC]- AP FR %d/Input FR %d, AP frame rate is not feasible (DI write) \n", u4APFR, u4InFR);
        return u4InFR;
    }
    else if (bDrvVencVDPIsOneFldMode() && tVencRes.fgInterlace)
    {
        LOG(1,"[VDP_VENC]- Interlace One Fld mode, doesn't support AP frame rate, use input frame rate \n");
        return u4InFR;
    }
    else 
    {
        LOG(1,"[VDP_VENC]- AP FR %d/Input FR %d, AP frame rate is feasible \n", u4APFR, u4InFR);
        return u4APFR;
    }
}

void vDrvVencVDPSetEncFldType(E_VDP_VENC_FIELD_MODE eEncFldType)
{
    tVencInfo.eEncFldType = eEncFldType;
}

void vDrvVencVDPSetEncModule(E_VDP_VENC_MODULE eEncModule)
{
    tVencInfo.eEncModule = eEncModule;
}

void vDrvVencVDPSetEncInput(E_VDP_VENC_INPUT eEncInput)
{
    tVencInfo.eEncInput = eEncInput;
}

void vDrvVencVDPGetEncInRes(VDP_VENC_INPUT_RES_t* tRes)
{
    *tRes = tVencRes;
}

E_VDP_VENC_MODULE eDrvVencVDPGetEncModule(void)
{
    return tVencInfo.eEncModule;
}
E_VDP_VENC_INPUT eDrvVencVDPGetEncInput(void)
{
    return tVencInfo.eEncInput;
}

void vDrvVencVDPInit(void)
{
    static UINT8 fgIsInit = FALSE;

    if (!fgIsInit)
    {
        VERIFY(x_sema_create(&_hVencVDPCtrl         , X_SEMA_TYPE_BINARY, X_SEMA_STATE_UNLOCK) == OSR_OK);
        VERIFY(x_sema_create(&_hVencVDPWaitForVRM   , X_SEMA_TYPE_BINARY, X_SEMA_STATE_UNLOCK) == OSR_OK);
        VERIFY(x_sema_create(&_hVencVDPWaitForSubSig, X_SEMA_TYPE_BINARY, X_SEMA_STATE_UNLOCK) == OSR_OK);
        tVencInfo.eProfileDef = VENC_PROFILE_MSI_OSTG;
        tVencInfo.eEncFldType = E_VDP_VENC_TOP_FLD;
        tVencInfo.eEncModule  = E_VDP_VENC_MONITOR_OUT;
        tVencInfo.eEncInput   = E_VDP_VENC_NOT_SELECTED;
        tVencInfo.eSubEncInput = E_SUB_VENC_SUB_SCALER_IN;
		u1VRMStableCounter = 0;
        fgIsInit = TRUE;
    }    
}

void vDrvVencVDPCtrlMLoop(void)
{
    if (u1DrvVencVDPIsWaitingForSubSig())
    {
        LOG(2,"[VDP_VENC]- Wait For Sub Signal Stable %d  \n", u1DrvVencVDPSignalReady());
        if (u1DrvVencVDPSignalReady())
        {
            // unlock semaphore
            vDrvVencVDPEnableWaitForSubSig(FALSE);
            x_sema_unlock(_hVencVDPWaitForSubSig);
        }   
    }
    else if (u1DrvVencVDPIsWaitingForVRM())
    {
        LOG(2,"[VDP_VENC]- Wait For VRM\n");
        if (u1DrvVencVDPVRMReady())
        {
            // unlock semaphore
            vDrvVencVDPEnableWaitForVRM(FALSE);
            x_sema_unlock(_hVencVDPWaitForVRM);
        }   
    }
    
}

#endif  //CC_SUPPORT_VENC

