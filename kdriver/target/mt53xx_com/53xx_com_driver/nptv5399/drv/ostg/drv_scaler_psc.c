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
#include "drv_scaler_drvif.h"
#include "vdp_display.h"
#include "scpos_debug.h"
#include "drv_scaler_psc.h"
#if defined(CC_MT5399)
#include "drv_css.h"
#endif

#if defined(SCPIP_SUPPORT_POST_SCALER)


#define PSC_FRAME_TRACK_TARGET  60
#define PSC_DEFAULT_WTLV 1000

typedef struct
{
    UINT8 bPscModeChanged;
    VRM_INFO_T pscIn;
    VRM_INFO_T pscOut;
	UINT32 u4HDeltaPhaseIntg;	            //H Scaling factor (integer part)
	UINT32 u4HDeltaPhaseFrac;	            //H Scaling factor (fraction part)
    UINT32 u4HInitPhaseSign;                //H Scaling phase  (sign)
    UINT32 u4HInitPhaseIntg;                //H Scaling phase  (integer)
    UINT32 u4HInitPhaseFrac;                //H Scaling phase  (fraction)	
	UINT32 u4InputWidth;			        //input width(pixel based)
	UINT32 u4InputHeight;                   //input height(pixel based)
	UINT32 u4OutputX;			            //output X(pixel based)
	UINT32 u4OutputY;                       //output Y(pixel based)
	UINT32 u4OutputWidth;                   //output Width(pixel based)
	UINT32 u4OutputHeight;	                //output Height(pixel based)    
	BOOL bNeedSetData;                    //update XY later (Before Set data trigger)
}PSC_INFO_T;

PSC_INFO_T pscInfo;

UINT8 pscInitFlag = SV_FALSE;

extern UINT32 _u4VSyncCountSD;  //for set data use

#ifndef CC_MTK_LOADER
//static functions
static void _vPscUpdateDispPrm(void);
static void _vPscUpdateInOutRegion(void);
//static void _vPscUpdateScaliingFactorPrm(UINT32 inW, UINT32 outW) ;
static void _vPscUpdateOutputResolution(UINT32 outX, UINT32 outY, UINT32 outW, UINT32 outH);
static void _vPscUpdateDispmodePrm(void);
static void _vPscUpdateReg(void);
static void _vPscSetImportOnOff(UINT8 bOnOff);                
static void _vPscModeSwitch(void);
#endif
static void _vPscInitMjcDispmode(void);
static void _vPscInitSyncAdj(void);
EXTERN BOOL fgFrameTrackIsFlgSet(UINT32 u4Flag);
    
#ifndef CC_MTK_LOADER

void vPscFireModeChanged(void)
{
    pscInfo.bPscModeChanged = SV_TRUE;
}

void vPscClearModeChanged(void)
{
    pscInfo.bPscModeChanged = SV_FALSE;
}

UINT8 fgPscIsModeChanged(void)
{
    RVChannel* mainChannel;
    RVChannel* subChannel;
    UINT8 ret = SV_FALSE;

    mainChannel = getChannel(SV_VP_MAIN);
    subChannel = getChannel(SV_VP_PIP);
    
    if ( (mainChannel->bIsChannelOn ==SV_TRUE && mainChannel->bIsFreeze == SV_FALSE) || 
         (subChannel->bIsChannelOn ==SV_TRUE && subChannel->bIsFreeze == SV_FALSE) ||
         (pscInitFlag == SV_TRUE))   //channel is on & not freezed
    {
        if(pscInfo.bPscModeChanged == SV_TRUE)
        {
            ret = SV_TRUE;
        }
    }
    return ret;
}
/******************************************
*  @Cobra porting done
******************************************/
void vPscSetImportOff(UINT8 bOnOff)
{
    if(bOnOff == SV_ON)
    {
		vPscSetResToggle(SV_ON);
    }
    else
    {	
		vPscSetResToggle(SV_OFF);
    }
}

extern UINT32 GCD(UINT32 a, UINT32 b);

/******************************************
*  @ cobra need review
******************************************/
void vPscConfigFrameTrack(UINT32 u4Step)
{
    UINT8 u1InFR;
    UINT8 u1OutFR;
    UINT8 u1TrackVNum;
    //UINT32 u4PreVofst;
    //UINT32 u4PanelVTotal;
    //UINT32 u4UpperBound;
    //UINT32 u4LowerBound;
    //UINT32 u4LockRange;

    u1InFR = bDrvVideoGetRefreshRate(SV_VP_MAIN);
    u1OutFR = vDrvGetLCDFreq();
    
    if ((bDrvVideoSignalStatus(SV_VP_MAIN) != SV_VDO_STABLE) || (u1InFR==0) || (u1OutFR==0))
    	return;
        
    u1TrackVNum = u1InFR / GCD(u1InFR, u1OutFR);
    
    //u4PanelVTotal = u4PscGetPanelVTotal();
    
    //1. set frame track interval
    vPscMaskInputFrame(u1TrackVNum);  

    //2. set frame track target. current's target is PRE_VOFST_BYPASS
    //using 96 default value?? need to review!!
	vPscSetFrameTrackTargetDistance(PSC_FRAME_TRACK_TARGET);
	
    //3. reference front scaler output since Post scaler is in MJC dispmode.
    //cobra post scaler no need to select which input vsync to track.
    
    //4. set to mask mode
    //PSC_WRITE_FLD(PSPIP_OA_0D, 1, PSPIP_OA_0D_FRAME_TRACK_VSYNC_DIST_MODE);  
    
    //6. set track range, need to do
    
    vPscSetFrameTrackLockWin(5);
    vPscSetFrameTrackLockTolerateLineNum(5);
    //vPscSetFrameTrackTurboRegion(UINT32 u4Region)
	//vPscSetFrameTrackStep(UINT32 u4Step)

	//7. other setting
	//vPscSetFrameTrackOnceOnly(UINT8 bOnOff)
}

/******************************************
*  @ Cobra porting done
******************************************/
void vPscSetFrameTrackOnOff(UINT32 u4OnOff)
{
    #if defined(__MODEL_slt__) || defined(DRV_SUPPORT_EXTMJC) // Frame tracking always off for SLT and external MJC
    vPscSetFrameTrackStep(0);
    vPscSetDddsFrameTrackEnable(SV_OFF);
    #else
    if (u4OnOff)  // notice the order
    {  
    	 if(fgFrameTrackIsFlgSet(FRAME_TRACK_MODE_DDDS_BY_PSTSCALER)) // DDDS frame track mode
        {
            //set to DDDS Adjust mode        
            vPscSetDddsFrameTrackEnable(SV_ON);
            vPscSetFrameTrackStep(0);
        }
        else 
        {
            //set to V Total Adjust mode         
            vPscSetDddsFrameTrackEnable(SV_OFF);
            vPscSetFrameTrackStep(1);
        }    
    }
    else
    {
         vPscSetFrameTrackStep(0);
         vPscSetDddsFrameTrackEnable(SV_OFF);
    }
    #endif    
}

void vPscSwInit(void)
{    
    pscInfo.bPscModeChanged = SV_FALSE;
    pscInfo.bNeedSetData = SV_FALSE;
	pscInfo.u4OutputX = 0XFFFF;
	pscInfo.u4OutputY = 0XFFFF;
}

static void _vPscUpdateInOutRegion(void)
{
    VRMGetModuleVRMInfo(SV_VP_MAIN, VRM_MODULE_PSC, &(pscInfo.pscIn), &(pscInfo.pscOut));
}

#if 0
static void _vPscUpdateScaliingFactorPrm(UINT32 inW, UINT32 outW) 
{
	UINT32 u4HPhaseInc;   
	
    //for H up case
    if ( inW == 0 || outW == 0 || outW < inW)
    {    
		return;
    }
    else  
    {    
        //scaling delta phase
		u4HPhaseInc = (inW * DFT_UNIT_PHASE(SV_VP_MAIN)) / outW;
		pscInfo.u4HDeltaPhaseIntg = u4HPhaseInc >> DFT_FRAC_PHASE_BITS(SV_VP_MAIN);	
		pscInfo.u4HDeltaPhaseFrac = u4HPhaseInc & DFT_FRAC_PHASE_MASK(SV_VP_MAIN);

        //scaling init phase
		pscInfo.u4HInitPhaseIntg = 0;
	    if(pscInfo.u4HDeltaPhaseFrac == 0)
    	{
			pscInfo.u4HInitPhaseFrac = 0;
			pscInfo.u4HInitPhaseSign = 0;    	
    	}
	    else
    	{
			pscInfo.u4HInitPhaseFrac = DFT_UNIT_PHASE(SV_VP_MAIN) + 
				( ((inW-1)*DFT_UNIT_PHASE(SV_VP_MAIN)) - ((outW-1)*pscInfo.u4HDeltaPhaseFrac))/2;
			pscInfo.u4HInitPhaseSign = 1;            
    	}
    }
}
#endif 


static void _vPscUpdateInputResolution(UINT32 inW,UINT32 inH)
{
    pscInfo.u4InputWidth = inW;
    pscInfo.u4InputHeight = inH;
}

static void _vPscUpdateOutputResolution(UINT32 outX, UINT32 outY, UINT32 outW, UINT32 outH)
{
    CRIT_STATE_T csState;    

    if(pscInfo.u4OutputX != outX || pscInfo.u4OutputY != outY)
    {
        vPscSetNeedSetDataFlag(SV_TRUE);
        LOG(3,"PSC SET DATA FLG = SV_TRUE, _u4VSyncCountSD=%d \n",_u4VSyncCountSD);            
    }

    csState = x_crit_start();
    pscInfo.u4OutputX = outX;
    pscInfo.u4OutputY = outY;
    pscInfo.u4OutputWidth = outW;
    pscInfo.u4OutputHeight = outH;
    x_crit_end(csState);
}

static void _vPscUpdateDispmodePrm(void)
{
}

static void _vPscUpdateDispPrm()
{    
    //1. update input & output size
    _vPscUpdateInOutRegion();
    //2. update H up scaling factor
    //_vPscUpdateScaliingFactorPrm(pscInfo.pscIn.u2Width, pscInfo.pscOut.u2Width);
    //3. update input resolution
    _vPscUpdateInputResolution(pscInfo.pscIn.u2Width,pscInfo.pscIn.u2Height);
    //4. update output resolution
    _vPscUpdateOutputResolution(pscInfo.pscOut.u2X,pscInfo.pscOut.u2Y,
                                        pscInfo.pscOut.u2Width,pscInfo.pscOut.u2Height);
    //5. update display mode prm
    _vPscUpdateDispmodePrm();
}
/******************************************
*  @Cobra porting done
******************************************/
static void _vPscSetImportOnOff(UINT8 bOnOff)
{
    LOG(3,"Set PSC import(%d)\n",bOnOff);
	vPscSetResImportEnable(bOnOff);
}

static void _vPscUpdateReg(void)
{
    /**** start import protect ****/
    _vPscSetImportOnOff(SV_OFF);                
    //1. set input resolution
    #if defined(CC_MT5399)
    if (fgCSSOnOff)
    {
        vPscSetInputWidthHeight(pscInfo.u4InputWidth - CSS_MAX_COMPENSATION * 2,pscInfo.u4InputHeight- CSS_MAX_COMPENSATION*2);
    }
    else
    #endif
    {
        vPscSetInputWidthHeight(pscInfo.u4InputWidth,pscInfo.u4InputHeight);
    }
    //2. set output resolution
    if(bPscIsNeedSetData()== SV_TRUE)
    {
        #if defined(CC_MT5399)
        if (fgCSSOnOff)
        {
            vPscSetHmskStartOfst(CSS_MAX_COMPENSATION);
            vPscSetVmskStartOfst(CSS_MAX_COMPENSATION);    
        }
        else
        #endif
        {
            vPscSetHmskStartOfst(pscInfo.u4OutputX);
            vPscSetVmskStartOfst(pscInfo.u4OutputY);    
        }
    }

    #if defined(CC_MT5399)
    if (fgCSSOnOff)
    {
    	vPscSetOutputWidthHeight(pscInfo.u4OutputWidth-CSS_MAX_COMPENSATION*2, pscInfo.u4OutputHeight-CSS_MAX_COMPENSATION*2);    	
    }
    else
    #endif
    {
    	vPscSetOutputWidthHeight(pscInfo.u4OutputWidth, pscInfo.u4OutputHeight);    	
    }
    //3. set H Delta phase
    //vPscSetHDeltaPhase(pscInfo.u4HDeltaPhaseIntg, pscInfo.u4HDeltaPhaseFrac);
    //4. set H Initial phase
    //vPscSetHInitPhase(pscInfo.u4HInitPhaseSign, pscInfo.u4HInitPhaseIntg, pscInfo.u4HInitPhaseFrac);
    //5. set mjc dispmode register (need review)
    _vPscModeSwitch();
    
    _vPscSetImportOnOff(SV_ON);
    /**** end of import protect ****/

    if(pscInitFlag == SV_TRUE)
    {
        //PSC_WRITE_FLD(PSSYSTEM_10, 0, PSSYSTEM_10_IMPORT_OFF_1);
        pscInitFlag = SV_FALSE;
    }	
}

void vPscUpdateReg_SETDATA(void)
{
	LOG(3,"vPscUpdateReg_SETDATA _u4VSyncCountSD(%d)\n",_u4VSyncCountSD);
    //2. set output resolution
    vPscSetHmskStartOfst(pscInfo.u4OutputX);
	vPscSetVmskStartOfst(pscInfo.u4OutputY);
}

static void _vPscModeSwitch(void)
{
	vPscSetMjcDispInterfaceHtotal(wDrvGetOutputHTotal());
}

BOOL bPscIsNeedSetData(void)
{
    return pscInfo.bNeedSetData;
}

void vPscSetNeedSetDataFlag(UINT8 bFlag)
{
    pscInfo.bNeedSetData = bFlag;
}

void vPSCDumpResInfo(void)
{
}

void vPscPrintCurrentScale(void)
{
    Printf("PSC H(0x%x-0x%x)\n",pscInfo.u4HDeltaPhaseIntg, pscInfo.u4HDeltaPhaseFrac);
}

void vPscNormProc(void)
{
    if(fgPscIsModeChanged()==SV_TRUE)
    {    
        //1. update display parameter
        _vPscUpdateDispPrm();
        //2. update related register
        _vPscUpdateReg();
        //3. clear mode change
        vPscClearModeChanged();
    }
}
#endif

void vPscHwInit(void)
{
    //1. init MJC dispmode
    _vPscInitMjcDispmode();
    //2. init SyncAdj/Active Adj/Read Delay
    _vPscInitSyncAdj();
    //3. set initial watel level
    vPscSetDispFifoWaterLevel(PSC_DEFAULT_WTLV);
    //4. set psc h init phase
    //vPscSetHInitPhase(0, 0, 0);
    //5. select OSD v signal to VDE
    vPscSelectOsdVsync(1);
    //inverse osd h/v sync 
    vPscSetInvOsdHVSync(SV_TRUE, SV_TRUE);
    pscInitFlag = SV_TRUE;
}

static void _vPscInitMjcDispmode(void)
{
	UINT32 u4HTotal;
	u4HTotal = wDrvGetOutputHTotal();	
	vPscSetMjcDispInterfaceHtotal(u4HTotal);	
}
static void _vPscInitSyncAdj(void)
{
  	//not use
}


void vPscPreInit(void)
{
    static UINT32 _pscPreset = 0;

    if (_pscPreset == 0)
    {
        _pscPreset = 1;

        //todo: do pre init here
        vPscHwInit();
    }   
}

#endif
