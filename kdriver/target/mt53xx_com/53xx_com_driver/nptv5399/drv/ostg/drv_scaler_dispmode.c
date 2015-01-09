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
 * $RCSfile: drv_scaler.c,v $
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
#include "x_lint.h"
LINT_EXT_HEADER_BEGIN
// Scaler related header files
#include "hw_ckgen.h"
#include "hw_scpos.h"
#include "drv_scpos.h"
#include "drv_scaler.h"
#include "drv_upscaler.h"
#include "drv_scaler_drvif.h"
#include "drv_predown.h"
#include "frametrack_drvif.h"
#include "scpos_debug.h"

// nptv header files
#include "general.h"       
#include "sv_const.h"       
#include "video_def.h"      
#include "vdo_misc.h"       
#include "vdp_display.h"
#include "drv_video.h"   
#include "source_select.h"
#include "drv_display.h"
#include "panel.h"
#include "mute_if.h"
#ifdef CC_MT5399
#include "drv_mjc.h"
#endif

#include "drv_tdtv_drvif.h"

// drvif header files
#include "fbm_drvif.h"
#include "srm_drvif.h"
#include "vdp_drvif.h"
#include "nptv_drvif.h"

// Other header files
#include "c_model.h"
#include "x_bim.h"
#include "x_mid.h"
#include "x_assert.h"
#include "x_util.h"

LINT_EXT_HEADER_END

#define CC_SCPOS_FREERUN_MAX        150
UINT32 _u4DispModeFreeRunCounter =0;

EXTERN VDP_SCALER_PRM_T _arScalePrm[VDP_NS];

void vScpipSetDispModeVar(UINT8 bPath, UINT8 u1Dispmode);
void vScpipSetLineSyncDispmode(UINT8 bPath,UINT8 bMode);

/*********************************************************
 ****************** ONLY FOR MT5398 **********************
 *********************************************************/
#if defined(CC_MT5399) || defined (CC_MT5882)
#define POCLK_SOURCE_CLK 324000000
#define POCLK_MINIMUM_PSCAN_DISPMODE_444 74000000
#define POCLK_MINIMUM_PSCAN_DISPMODE_422 68000000
EXTERN SCALER_OUTTIMING_PRM_T _arOutTimingPrm;

void vScpipConfigPOCLK(UINT8 mode);
void vScpipConfigPOCLKSel(void);
static void _vScpipUpdateInternalFrameRate(UINT8 mode);
static void _vScpipUpdatePoclkDivider(UINT8 mode);
static void _vScpipUpdateFSCTimeGenPrm(UINT8 mode);
#endif

/**
 * @brief Switch to dispmode
 * @param u4OnOff On/Off
 * @6896 wait review
 */
void vSwitchDispMode(UINT32 u4OnOff) 
{
    //UINT8 bScalerMode;
    //bScalerMode = u1ScpipGetDispMode(SV_VP_MAIN);

    if (u4OnOff == SV_ON)
    {
        /* Enable Display Mode */
    }
    else    //turn off
    {
        if (_arScalePrm[VDP_1].u4Enable)
        {
            CRIT_STATE_T csState = x_crit_start(); 
            // avoid other thread turn on write control before all related registers are set.          
            _arScalePrm[VDP_1].u4CntToRstDramPrm = (_arScalePrm[VDP_1].u4CntToRstDramPrm < 2)? 2 : _arScalePrm[VDP_1].u4CntToRstDramPrm;
			#ifdef CC_SUPPORT_PREPROC_TIMING
			if(bGetVideoDecType(SV_VP_MAIN) == SV_VD_PREPROC)
			{
			    _arScalePrm[VDP_1].u4CntToRstDramPrm = 0;
			}
			#endif
            x_crit_end(csState);
        }
    }
}


/**
 * @brief line sync mode on off
 * @param u4OnOff On/Off
 * @8096 review done
 */
void vScpipSetLineSyncDispmode(UINT8 bPath,UINT8 bMode)
{
    //only main support line sync dispmode
    if(bPath != SV_VP_MAIN)
    {
        return;
    }
    
    if (bMode == SV_ON)
    {
    	vScpipLinesyncModeOnOff(SV_ON); 
    }
    else    //turn off
    {
    	vScpipLinesyncModeOnOff(SV_OFF);
    }        
}

/**
 * @brief set pscan display mode on/off
 * @param bPath VDP_1/VDP_2
 * @param bMode what mode to set
 * @8096 need review
 */
void vScpipSetPSCANDispmode(UINT8 bPath,UINT8 bOnOff)
{
	if(bPath >= SV_VP_NA)
	{
		return;
	}

	if(bOnOff == SV_ON)
	{
        //disable import
        vScpipSetImportEnable(SV_VP_MAIN, SV_OFF);
        vScpipSetTgImportOnOff(SV_OFF);

        vScpipSetFrontMode(bPath, SV_OFF);
        vScpipSetBackMode(bPath, SV_OFF);

        //enable import
        vScpipSetImportEnable(SV_VP_MAIN, SV_ON);
        if(vDrvGetImportTriggerModule(bPath) != IMPORT_MODULE_NONE)
        {
            vScpipSetTgImportOnOff(SV_ON);		
        }
	}
	else
	{
		//need set frn or bck mode ? 

	}    
}

/**
 * @brief set dram mode
 * @param bPath VDP_1/VDP_2
 * @param bMode what mode to set
 * @8096 need review
 */
void vScpipSetDramMode(UINT8 bPath,UINT8 bOnOff)
{
    if(bOnOff == SV_OFF)
    {
        //R/W enable will turned on in scaler loop, so we don't have to do this here.
        //turn off R/W enable.
        vScpipWriteCtrlOnOff(bPath, SV_OFF);
    }
}

/**
 * @brief Set scaler mode (DRAM/scaler dispmode/PSCAN dispmode)
 * @param bPath VDP_1/VDP_2
 */
void vScpipModeSwitch(UINT8 bPath)
{
    UINT8 mode;
    mode = getScalerMode(bPath);
    //PIP support dram & pscan dispmode.
    if (bPath == SV_VP_PIP && 
		(mode == VDP_SCPOS_MJC_DISPMODE || mode == VDP_SCPOS_LINESYNC_DISPMODE ||
		 mode == VDP_SCPOS_DISPMODE_AUTO || mode == VDP_SCPOS_DISPMODE_MANUAL))
    {
        return;
    }

    if (mode == VDP_SCPOS_PSCAN_DISPMODE)      //pscan dispmode
    {
        vScpipSetDramMode(bPath,SV_OFF);
        vScpipSetLineSyncDispmode(bPath,SV_OFF);
        vScpipSetPSCANDispmode(bPath,SV_ON);
    }
    else if (mode == VDP_SCPOS_DISPMODE_OFF)        //dram mode
    {    
        vScpipSetPSCANDispmode(bPath,SV_OFF);
        vScpipSetLineSyncDispmode(bPath,SV_OFF);
        vScpipSetDramMode(bPath,SV_ON);
    }
    else if(mode== VDP_SCPOS_LINESYNC_DISPMODE)     //line sync dispmode
    {
        vScpipSetDramMode(bPath,SV_OFF);
        vScpipSetPSCANDispmode(bPath,SV_OFF);
        vScpipSetLineSyncDispmode(bPath,SV_ON);
    }

	#if SUPPORT_HDTV_HARDWARE_MUTE
    vScpipConfigHardwareMute();
	#endif
	
    return;
}

#if defined(CC_MT5399) || defined (CC_MT5882)
void vScpipConfigPOCLK(UINT8 mode)
{
    _vScpipUpdateInternalFrameRate(mode);
    //update FSC time gen parameter. If changed, will be set in ISR.
    _vScpipUpdateFSCTimeGenPrm(mode);

    if(_arOutTimingPrm.u2PanelVTotal <= _arOutTimingPrm.u2PanelVActive)
    {
        _arOutTimingPrm.u2PanelVTotal = _arOutTimingPrm.u2PanelVActive + 20;
    }    
    _vScpipUpdatePoclkDivider(mode);
  
    //update poclk divider
    vScpipSetPOCLKDividerReg(_arOutTimingPrm.u4DPllDividend,_arOutTimingPrm.u4DPLLDivisor);
    
    //update internal frame rate between Front scaler and MJC
    vDrvSetInternalFrameRate(_arOutTimingPrm.u2FrameRate);
    
    //set poclk to dpll
    vScpipConfigPOCLKSel();
}

static void _vScpipUpdateInternalFrameRate(UINT8 mode)
{
    UINT16 u2FrameRate;
    VDP_SCALER_PRM_T* scalerInfo;

    scalerInfo = getScalerInfo(SV_VP_MAIN);    
    u2FrameRate = scalerInfo->fscTimeGen.u2FrameRate;
    
    LOG(3,"Set Internal Frame Rate (%d)\n", u2FrameRate);
    _arOutTimingPrm.u2FrameRate = u2FrameRate;
}

static void _vScpipUpdateFSCTimeGenPrm(UINT8 mode)
{
    UINT16 u2PanelHTotal;
    UINT16 u2PanelHActive;
    UINT16 u2PanelVTotal;
    UINT16 u2PanelVActive;
    UINT16 u2PanelHFPorch;
    UINT16 u2PanelHBPorch;
    UINT16 u2PanelVFPorch;
    UINT16 u2PanelVBPorch;
    //UINT32 u4SclIn = u4DrvTDTVDIModeQuery();
    //E_TD_IN eSclIn = TD_SCL_IN(u4SclIn);
    VDP_SCALER_PRM_T* scalerInfo;
    CRIT_STATE_T csState;    

    scalerInfo = getScalerInfo(SV_VP_MAIN);
    u2PanelHTotal  = ((scalerInfo->fscTimeGen.u2HTotal == 0)? (wDrvGetOutputHTotal()) : (scalerInfo->fscTimeGen.u2HTotal));
    u2PanelHActive = ((scalerInfo->fscTimeGen.u2Width == 0) ? (wDrvGetOutputHActive()): (scalerInfo->fscTimeGen.u2Width));
    u2PanelVTotal  = ((scalerInfo->fscTimeGen.u2VTotal == 0)? (wDrvGetOutputVTotal()) : (scalerInfo->fscTimeGen.u2VTotal));
    u2PanelVActive = ((scalerInfo->fscTimeGen.u2Height == 0)? (wDrvGetOutputVActive()): (scalerInfo->fscTimeGen.u2Height));

    //determine FSC porch
    u2PanelHFPorch = 0;
    u2PanelHBPorch = 0;
    u2PanelVBPorch = 0;

    
    u2PanelVFPorch =  (u2PanelVTotal - u2PanelVActive) / 2;
    

    csState = x_crit_start();
    if((_arOutTimingPrm.u2PanelHTotal != u2PanelHTotal) || (_arOutTimingPrm.u2PanelHActive != u2PanelHActive) ||
       (_arOutTimingPrm.u2PanelVTotal != u2PanelVTotal) || (_arOutTimingPrm.u2PanelVActive != u2PanelVActive) ||
       (_arOutTimingPrm.u2PanelHFPorch != u2PanelHFPorch) || (_arOutTimingPrm.u2PanelHBPorch != u2PanelHBPorch) ||
       (_arOutTimingPrm.u2PanelVFPorch != u2PanelVFPorch) || (_arOutTimingPrm.u2PanelVBPorch != u2PanelVBPorch) )
    {
        _arOutTimingPrm.bUpdateTGLater = SV_TRUE;    
        _arOutTimingPrm.u2PanelHTotal = u2PanelHTotal;
        _arOutTimingPrm.u2PanelHActive = u2PanelHActive;
        _arOutTimingPrm.u2PanelVTotal = u2PanelVTotal;
        _arOutTimingPrm.u2PanelVActive = u2PanelVActive;
        _arOutTimingPrm.u2PanelHFPorch = u2PanelHFPorch;
        _arOutTimingPrm.u2PanelHBPorch = u2PanelHBPorch;
        _arOutTimingPrm.u2PanelVFPorch = u2PanelVFPorch;
        _arOutTimingPrm.u2PanelVBPorch = u2PanelVBPorch;
        LOG(1,"_vScpipUpdateFSCTimeGenPrm(HT,HA,VT,VA)=(%d,%d,%d,%d)\n",u2PanelHTotal,u2PanelHActive,u2PanelVTotal,u2PanelVActive);
        LOG(1,"_vScpipUpdateFSCTimeGenPrm(HFP,HBP,VFP,VBP)=(%d,%d,%d,%d)\n",u2PanelHFPorch,u2PanelHBPorch,u2PanelVFPorch,u2PanelVBPorch); 
    }    
    x_crit_end(csState);    
}

static void _vScpipUpdatePoclkDivider(UINT8 mode)
{
    UINT64 dividend;
    UINT64 divisor;
    UINT64 u4HTotal;
    UINT64 u4VTotal;
    UINT64 u4FrameRate;
    
    divisor = 0x8000;
    u4HTotal = _arOutTimingPrm.u2PanelHTotal;
    u4VTotal = _arOutTimingPrm.u2PanelVTotal;
    u4FrameRate = _arOutTimingPrm.u2FrameRate;
    
    if(mode == VDP_SCPOS_LINESYNC_DISPMODE)
    {
        if(u4FrameRate == 0)
        {
            dividend = 0x4000;
        }
        else
        {
            dividend = 0x8000;
            if(u4FrameRate < 50)
            {
                dividend = u8Div6464(dividend * u4FrameRate, 50, NULL);
            }
        }
    }
    else if(mode == VDP_SCPOS_PSCAN_DISPMODE || mode == VDP_SCPOS_DISPMODE_OFF)
    {               
        if(u4HTotal == 0 || u4VTotal ==0 || u4FrameRate ==0)
        {
            dividend = 0x4000;
        }
        else
        {	        
            dividend = u8Div6464((u4HTotal * u4VTotal * u4FrameRate * 0x8000), POCLK_SOURCE_CLK, NULL);
        }        
    }
    else // other dispmode...
    {
        return; // FIXME
    }
    
    LOG(3,"Set POCLK dividend (0x%x)\n", dividend);
    LOG(3,"Set POCLK divisor (0x%x)\n", divisor);
    _arOutTimingPrm.u4DPllDividend = dividend;
    _arOutTimingPrm.u4DPLLDivisor = divisor;
}

void vScpipSetSCTimeGen(UINT16 u2HTotal,UINT16 u2HActive, UINT16 u2VTotal, UINT16 u2VActive)
{    
    //update FSC output time gen.
    if(u4ScpipGetPanelHtotal() != u2HTotal)
    {
    	LOG(3,"Update SC PANEL HTOTAL from (%d) to (%d)\n", u4ScpipGetPanelHtotal(), u2HTotal);     
		vScpipSetPanelHtotal(u2HTotal);
    }

    if(u4ScpipGetPanelWidth() != u2HActive)
    {
    	LOG(3,"Update SC PANEL HACTIVE from (%d) to (%d)\n", u4ScpipGetPanelWidth(), u2HActive);
        vScpipSetPanelWidth(u2HActive);            
    }

    if(u4ScpipGetPanelVtotal() != u2VTotal)
    {
    	LOG(3,"Update SC PANEL VTOTAL from (%d) to (%d)\n", u4ScpipGetPanelVtotal(), u2VTotal);
        vScpipSetPanelVtotal(u2VTotal);           
    }

#ifndef __MODEL_slt__
    if (BSP_GetIcVersion() == IC_VER_5399_AA)
    {
        if((u4ScpipGetPanelHeight()-4) != u2VActive)
        {
            LOG(0,"Update SC PANEL VACTIVE from (%d) to (%d)\n", u4ScpipGetPanelHeight(), u2VActive);
            vScpipSetPanelHeight(u2VActive+4);           
        }
    }
    else
#endif
    {   
        
        if(u4ScpipGetPanelHeight() != u2VActive)
        {
            LOG(3,"Update SC PANEL VACTIVE from (%d) to (%d)\n", u4ScpipGetPanelHeight(), u2VActive);
            vScpipSetPanelHeight(u2VActive);           
        }
    }
}

void vScpipSetSCTimeGenPorch(UINT16 u2HFPorch,UINT16 u2HBPorch, UINT16 u2VFPorch, UINT16 u2VBPorch)
{
    //update FSC output time gen.
    if(u4ScpipGetHsyncFp() != u2HFPorch)
    {
    	LOG(3,"Update SC PANEL HF Porch from (%d) to (%d)\n", u4ScpipGetHsyncFp(), u2HFPorch);     
		vScpipSetHsyncFp(u2HFPorch);
    }

    if(u4ScpipGetHsyncBp() != u2HBPorch)
    {
    	LOG(3,"Update SC PANEL HB Porch from (%d) to (%d)\n", u4ScpipGetHsyncBp(), u2HBPorch);     
		vScpipSetHsyncBp(u2HBPorch);
    }    

    if(u4ScpipGetVsyncFp() != u2VFPorch)
    {
    	LOG(3,"Update SC PANEL VF Porch from (%d) to (%d)\n", u4ScpipGetVsyncFp(), u2VFPorch);     
		vScpipSetVsyncFp(u2VFPorch);
    } 

    if(u4ScpipGetVsyncBp() != u2VBPorch)
    {
    	LOG(3,"Update SC PANEL VB Porch from (%d) to (%d)\n", u4ScpipGetVsyncBp(), u2VBPorch);     
		vScpipSetVsyncBp(u2VBPorch);
    }     
}

void vScpipConfigPOCLKSel(void)
{
    //set dpll source from system pll /2 (324MHz)
    vScpipSetPOCLKDPllSelReg(FLD_POCLK_DPLL_SEL_SYSPLL_DIV2);
    
    #if defined(CC_SUPPORT_4K2K)||defined(CC_SUPPORT_HDMI_4K2K30)
    if(getScalerInfo(SV_VP_MAIN)->u4Bypass== SV_TRUE)
    {
        vScpipSetPOCLKSelReg(FLD_POCLK_SEL_MIB_OCLK);
    }
    else
    #endif
    {
        vScpipSetPOCLKSelReg(FLD_POCLK_SEL_DPLL);
    }
}

// When MJC on/off update PSC Global to  FSC Global 
EXTERN LCDTIMING_INFO_T lcdtimingInfo;

void vDrvUpdateFSCTimeGenPrm(void)
{       
    _arOutTimingPrm.u2PanelHTotal  = lcdtimingInfo.u4HTotal;
    _arOutTimingPrm.u2PanelHActive = lcdtimingInfo.u4HActive;
    _arOutTimingPrm.u2PanelVTotal  = lcdtimingInfo.u4VTotal;
    _arOutTimingPrm.u2PanelVActive = lcdtimingInfo.u4VActive;
    _arOutTimingPrm.u2PanelHFPorch = lcdtimingInfo.u4HFP;
    _arOutTimingPrm.u2PanelHBPorch = lcdtimingInfo.u4HBP;
    _arOutTimingPrm.u2PanelVFPorch = lcdtimingInfo.u4VFP;
    _arOutTimingPrm.u2PanelVBPorch = lcdtimingInfo.u4VBP;
}

#endif

 
/**
 * @brief Enable/Disable scaler dispmode
 * @param ucVdpId VDP_1/VDP_2
 * @param ucOnOff On/Off
 * @8096 review done. change name??
 */
void _VDP_DispModeEnable(UCHAR ucVdpId, UCHAR ucMode)
{
    UINT8 bPath;

    if (ucVdpId >= VDP_NS)
    {
        return;
    }

    bPath = getScalerPath(ucVdpId);

    #if !defined(SCPIP_SUPPORT_POST_SCALER)
    if(ucMode == VDP_SCPOS_LINESYNC_DISPMODE)
    {
        LOG(0,"Line sync dispmode is not supported\n");
        ASSERT(0);
    }
    #endif

    if (u1ScpipGetDispMode(bPath) != ucMode) 
    {
        LOG(4,"Scaler mode changed. VDP(%d) mode(%d->%d)\n",ucVdpId,u1ScpipGetDispMode(bPath),ucMode);
        vScpipSetDispModeVar(bPath, (UINT8)ucMode);
        //Scaler disp mode changed, turn off import protection.

        if (ucVdpId == VDP_1)
        {
            vSetScposFlg(MAIN_DISP_PRM_CHG);
            vSetScposFlg(SCALER_MAIN_MODE_CHG);
        }
        else
        {
            if(ucMode != VDP_SCPOS_DISPMODE_OFF && ucMode != VDP_SCPOS_PSCAN_DISPMODE)
            {
                //Hardware Limitation, sub path can't get into scaler dispmode
                ASSERT(0);
            }
            vSetScposFlg(PIP_DISP_PRM_CHG);
            vSetScposFlg(SCALER_PIP_MODE_CHG);
        }
    }
}

/**
 * @brief Set dispmode status
 * @param bPath VDP_1/VDP_2
 * @param bPath VDP_1/VDP_2
 */
void vScpipSetDispModeVar(UINT8 bPath, UINT8 u1Dispmode)
{
    #if 0
    if(bPath >=VDP_2 && u1Dispmode != VDP_SCPOS_DISPMODE_OFF)
    {
        Printf("Set VDP(%d) to Non-dram mode(%d)\n",getScalerVDP(bPath),u1Dispmode);
        ASSERT(0);
    }
    #endif
    
    if (bPath >= VDP_NS)
    {
        return;
    }
    _arScalePrm[bPath].u4DispMode = (UINT32) u1Dispmode;
}

/**
 * @brief Get dispmode status
 * @param bPath VDP_1/VDP_2
 * @return Dispmode type
 * @6896 porting done
 */
UINT8 u1ScpipGetDispMode(UINT8 bPath)
{
    ASSERT(bPath <= SV_VP_PIP);
    return _arScalePrm[bPath].u4DispMode;
}

