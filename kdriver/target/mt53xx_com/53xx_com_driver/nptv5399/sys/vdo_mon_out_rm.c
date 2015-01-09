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
#include "drv_tdtv.h"
#include "drv_tdtv_drvif.h"
#include "drv_display.h"
#include "vdo_misc.h"
#include "drv_regen.h"
#include "vdp_display.h"
#include "hw_sw.h"
#include "b2r_if.h"
#include "scpos_debug.h"
#include "drv_di.h"
#ifdef CC_MT5398
#include "drv_mjc.h"
#endif
#include "tve_if.h"

#include "drv_mon_out.h"
#include "vdp_venc_ctrl.h"



//MT5399 MON_OUT porting
//---start-----
// monitor out  Info
MonOutPicInfo   _rMonitorOutInfo;
MonOutPicInfo   _vrm_mon_out_info;

static UINT32     _u4VRMMonOutEventFlag = 0;

EXTERN UCHAR* VRM_MODULE_NAMES[VRM_MODULE_MAX];
//---end-----


//============================================================
//MT5399 MON_OUT porting
//
//============================================================
//---start----
UINT32 vVRMGetMonOutEventFlg( void )
{
    return _u4VRMMonOutEventFlag;
}

void vVRMSetMonOutEventFlg( UINT32 u4MonOutFlag )
{
    _u4VRMMonOutEventFlag |= u4MonOutFlag;
}



VOID vVRMClrMonOutEventFlg( void )
{
    CRIT_STATE_T csState;
    csState = x_crit_start();
    _u4VRMMonOutEventFlag = 0;
    x_crit_end(csState);
    return;
}
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
void vVrmMonOutSwInit( void )
{
   MonOutPicInfo* pMonOutInfo;
   pMonOutInfo = getMonOutInfo();    

   pMonOutInfo->u1MonOutInSel =  E_MON_OUT_PANEL;
	
   pMonOutInfo->wSrcW = 1920;
   pMonOutInfo->wSrcH = 1080;
   pMonOutInfo->u1Output = E_MON_OUT_GFX;

   pMonOutInfo->rRegenScan.u4X=0;
   pMonOutInfo->rRegenScan.u4Y=0;
   pMonOutInfo->rRegenScan.u4Width = VDP_MAX_REGION_WIDTH;
   pMonOutInfo->rRegenScan.u4Height = VDP_MAX_REGION_HEIGHT;


}

void vVRMUpdateVdpInforForMonOut( UINT8 u1MonOutMuxIn )
{
    UINT8   bPath=SV_VP_MAIN;
    VRM_MODULE_E moduleID=VRM_MODULE_OSTG;
    MonOutPicInfo* pMonOutInfo;
    VRM_INFO_T* vrmInfo;       
    pMonOutInfo = getMonOutInfo();  	
	pMonOutInfo->u1MonOutInSel = u1MonOutMuxIn;
	
    switch (u1MonOutMuxIn )
    {
        case E_MON_OUT_MIXER:
            bPath=SV_VP_MAIN;
            moduleID=VRM_MODULE_MJC;
            break;
        case E_MON_OUT_MAIN_FSC_OUT:
            bPath=SV_VP_MAIN;
            moduleID=VRM_MODULE_MIXER;
            break;
        case E_MON_OUT_SUB_FSC_OUT:
            bPath=SV_VP_PIP;
            moduleID=VRM_MODULE_MIXER;
           break;
        case E_MON_OUT_MAIN_MIB_OUT:
           moduleID=VRM_MODULE_FSC;
           bPath=SV_VP_MAIN;
           break;
        case E_MON_OUT_SUB_MIB_OUT:
           moduleID=VRM_MODULE_FSC;
           bPath=SV_VP_PIP;
           break;
        case E_MON_OUT_MAIN_PREDOWN:
           moduleID=VRM_MODULE_PDS;
            bPath=SV_VP_MAIN;
           break;
        case E_MON_OUT_SUB_PREDOWN:
           moduleID=VRM_MODULE_PDS;
           bPath=SV_VP_PIP;
           break;
        case E_MON_OUT_PANEL:
           bPath=SV_VP_MAIN;
           moduleID=VRM_MODULE_OSTG;
           break;
    }
    
    vrmInfo = VRMGetVRMInfo(bPath , moduleID);
    LOG(1,"MoniOut Input Module [%2d%8s  [%1d] X=%4d Y=%4d Width=%4d Height=%4d HT=%4d VT=%4d FR=%3d %c]\n",
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

	pMonOutInfo->wSrcW = vrmInfo->u2Width;

    if ( vrmInfo->u2ScanMode )
   	   pMonOutInfo->wSrcH =  vrmInfo->u2Height/2;   //if interlace input signal
    else
        pMonOutInfo->wSrcH =  vrmInfo->u2Height;
    
	pMonOutInfo->wHTotal = vrmInfo->u2HTotal;
	pMonOutInfo->wVTotal =  vrmInfo->u2VTotal;
	pMonOutInfo->u2InFramerate = vrmInfo->u2FrameRate;
	pMonOutInfo->u2OutFramerate = vrmInfo->u2FrameRate;
	pMonOutInfo->u1Interlace = vrmInfo->u2ScanMode;
	if( pMonOutInfo->u1MonOutInSel == E_MON_OUT_PANEL )
	{
		pMonOutInfo->wSrcW = vrmInfo->u2HTotal;
		pMonOutInfo->wSrcH = vrmInfo->u2VTotal;
	}
}



//----------------------------------------------------------------------------
void vVRMUpdateMonOutMUXin(void)
{
    MonOutPicInfo* pMonOutInfo;
    pMonOutInfo = getMonOutInfo();    
    LOG(1,"vVRMUpdateMonOutMUXin\n");   

    vVRMUpdateVdpInforForMonOut( pMonOutInfo->u1MonOutInSel );
}


/**
 *  Update REGEN input Resolution
 *  REGEN input comes from PREDOWN output.
 */
void vVRMUpdateMonOutRegenResolution( void )
{
	MonOutPicInfo* pMonOutInfo;
	pMonOutInfo = getMonOutInfo();

	LOG(1,"vVRMUpdateMonOutRegenResolution\n");
	//to do: 3d process ???  
	// pMonOutInfo->rRegenScan = _vrm_mon_out_info.rRegenScan;


	// Calculate Source Region
	pMonOutInfo->wXOffset = DIVIDE_WITH_ROUND(pMonOutInfo->wSrcW * pMonOutInfo->rRegenScan.u4X,VDP_MAX_REGION_WIDTH);
	pMonOutInfo->wYOffset = DIVIDE_WITH_ROUND(pMonOutInfo->wSrcH * pMonOutInfo->rRegenScan.u4Y,VDP_MAX_REGION_HEIGHT);
	pMonOutInfo->wTargetW = DIVIDE_WITH_ROUND(pMonOutInfo->wSrcW * pMonOutInfo->rRegenScan.u4Width,VDP_MAX_REGION_WIDTH);
	pMonOutInfo->wTargetH = DIVIDE_WITH_ROUND(pMonOutInfo->wSrcH * pMonOutInfo->rRegenScan.u4Height,VDP_MAX_REGION_HEIGHT);  
	if ( pMonOutInfo->u1MonOutInSel == E_MON_OUT_PANEL )
	{
		if (fgIsMJCToOSTG())
		{
			pMonOutInfo->wXOffset = u4PscGetPanelHSyncBp();
			pMonOutInfo->wYOffset = u4PscGetPanelVSyncBp();
			pMonOutInfo->wTargetW = u4PscGetPanelWidth();
			pMonOutInfo->wTargetH = u4PscGetPanelHeight();
		}
		else
	    {
	    	pMonOutInfo->wXOffset = u4ScpipGetHsyncBp();
	    	pMonOutInfo->wYOffset = u4ScpipGetVsyncBp();
	    	pMonOutInfo->wTargetW = u4ScpipGetPanelWidth();
	    	pMonOutInfo->wTargetH = u4ScpipGetPanelHeight();
	    }
	}

}

/**
 *  Update PREDOWN input Resolution
 *  PREDOWN input comes from TTT output.
 *  When FS -> 2D => Frame rate become 1/2
 *  When SBS -> 2D => Width become 1/2
 *  When TAB -> 2D => Height become 1/2
 */
void vVRMMonOutUpdatePredownResolution(void)
{
    MonOutPicInfo* pMonOutInfo;
    APP_RES_T pVencres;
	pMonOutInfo = getMonOutInfo();

	//to do: get 3d to 2d output resolution:
	pMonOutInfo->wPdsInWidth = pMonOutInfo->w3Dto2DOutWidth;
	pMonOutInfo->wPdsInHeight = pMonOutInfo->w3Dto2DOutHeight;

    if(vMonOutGetDotByDotOutput() == SV_TRUE)
    {
        pMonOutInfo->wPdsOutWidth = pMonOutInfo->wPdsInWidth;
	    pMonOutInfo->wPdsOutHeight = pMonOutInfo->wPdsInHeight;
  
        pMonOutInfo->wOutputWidth  = pMonOutInfo->wPdsOutWidth;
	    pMonOutInfo->wOutputHeight = pMonOutInfo->wPdsOutHeight;

        LOG(1, "Monitor out dot by dot output (%d X %d)\n",pMonOutInfo->wOutputWidth,pMonOutInfo->wOutputHeight);
        return;
    }
    
    if(pMonOutInfo->u1Output == E_MON_OUT_TVE)
    {
		if( pMonOutInfo->u1TimeGenSel == TVE_FMT_576I)
		{
			pMonOutInfo->wOutputWidth  =  720;
			pMonOutInfo->wOutputHeight = 576;
			LOG(1,"predn to PAL 720*576\n");
		}
		else   //720x480i
		{
			pMonOutInfo->wOutputWidth  = 720;
			pMonOutInfo->wOutputHeight = 480;
			LOG(1,"predn to NTSC 720*480\n");
		}
    }

    if(pMonOutInfo->u1Output == E_MON_OUT_VENC)
    {
        vDrvVrmGetAppRes(VRM_APP_VIDEO_ENC,&pVencres);	
        pMonOutInfo->wOutputHeight = pVencres.u4Height;
        pMonOutInfo->wOutputWidth = pVencres.u4Width;

        if(pMonOutInfo->u1Interlace 
            #ifdef CC_SUPPORT_VENC
            && bDrvVencVDPIsOneFldMode()
            #endif
            )
        {
            pMonOutInfo->wOutputHeight <<= 1;
        }
        
    }
    
	if((pMonOutInfo->wPdsInWidth != 0) &&
		(pMonOutInfo->wPdsInHeight != 0))
	{
        if ( pMonOutInfo->u1Interlace &&    \
             ( (pMonOutInfo ->u1MonOutInSel == E_MON_OUT_MAIN_PREDOWN ) || (pMonOutInfo ->u1MonOutInSel == E_MON_OUT_SUB_PREDOWN ))  )  
        {
           //if mon_out input mux select to pre_dn_main or pre_dn_sub && INPUT SIGNAL IS INTERLACE!!!
            pMonOutInfo->wPdsInHeight = pMonOutInfo->wPdsInHeight * 2;
        }
                
		if(pMonOutInfo->wPdsInWidth > pMonOutInfo->wOutputWidth)
		{
			pMonOutInfo->wPdsOutWidth = pMonOutInfo->wOutputWidth;
		}
		else
		{
            pMonOutInfo->wPdsOutWidth = pMonOutInfo->wPdsInWidth;
		}

		if ( pMonOutInfo->wPdsInHeight > pMonOutInfo->wOutputHeight)
		{
			pMonOutInfo->wPdsOutHeight	= pMonOutInfo->wOutputHeight;
		}
		else
		{
			pMonOutInfo->wPdsOutHeight = pMonOutInfo->wPdsInHeight;
		}

        
		if(pMonOutInfo->u1Output == E_MON_OUT_TVE)
		{
			if(pMonOutInfo->wPdsInWidth < 720)
			{
				pMonOutInfo->wPdsOutWidth = pMonOutInfo->wPdsInWidth;
			}
                       
            if(( pMonOutInfo->u1TimeGenSel == TVE_FMT_576I) &&
                (pMonOutInfo->wPdsInHeight < 576))
            {
                 pMonOutInfo->wPdsOutHeight = pMonOutInfo->wPdsInHeight;
            }
            if((pMonOutInfo->u1TimeGenSel == TVE_FMT_480I) &&
                (pMonOutInfo->wPdsInHeight < 480))
            {               
                 pMonOutInfo->wPdsOutHeight = pMonOutInfo->wPdsInHeight;
            }

		}
	}
	else
	{
		pMonOutInfo->wPdsOutWidth  = pMonOutInfo->wPdsInWidth;
		pMonOutInfo->wPdsOutHeight = pMonOutInfo->wPdsInHeight;
	}
	//1280 fifo limit
    if((pMonOutInfo->wPdsOutWidth > 1280 && (pMonOutInfo->wPdsInWidth > pMonOutInfo->wPdsOutWidth)) ||
        (pMonOutInfo->wPdsOutWidth > 1280 && (pMonOutInfo->wPdsInHeight > pMonOutInfo->wPdsOutHeight))
        )
	{
		LOG(1, "Monitor out pre-down have 1280 line buffer limitation\n");
		pMonOutInfo->wPdsOutWidth = 1280;
	}
	pMonOutInfo->wOutputWidth  = pMonOutInfo->wPdsOutWidth;
	pMonOutInfo->wOutputHeight = pMonOutInfo->wPdsOutHeight;

}

UINT32 u4DrvMonOutTTTModeQuery(void)
{

    UINT32 u4MIB     = u4DrvTDTVDIModeQuery();
    UINT32 u4MJC     = u4DrvTDTVMJCModeQuery();
    UINT32 u4TTD     = u4DrvTDTVTTDModeQuery();

	E_TD_IN  eMIBIn	 = TD_MIB_IN(u4MIB);
	E_TD_OUT eMJCOUT = TD_MJC_OUT(u4MJC);
	E_TD_OUT  eTTDOUT = TD_TTD_OUT(u4TTD);
	
	MonOutPicInfo* pMonOutInfo;	
	pMonOutInfo = getMonOutInfo();


	if((pMonOutInfo->u1MonOutInSel == E_MON_OUT_SUB_PREDOWN)
		||(pMonOutInfo->u1MonOutInSel == E_MON_OUT_SUB_FSC_OUT) 
		|| (pMonOutInfo->u1MonOutInSel == E_MON_OUT_SUB_MIB_OUT)) //sub 
	{
		LOG(1,"sub path will do 3D to 2D if it is needed\n");
		return E_TTT_2D_IN;
	}
	else  //main
	{
		if((E_TD_OUT)u4DrvTDTVTTTModeQuery(VDP_1)!= E_TD_OUT_3D_2_2D)
		{
			if(eMJCOUT != E_TD_OUT_NATIVE)	//3D
			{
				if((eTTDOUT == E_TD_OUT_3D_LI)
					||(eTTDOUT == E_TD_OUT_3D_FS))//ttd
				{
					switch (pMonOutInfo->u1MonOutInSel)
					{
						case E_MON_OUT_MIXER:
						case E_MON_OUT_MAIN_FSC_OUT:
						case E_MON_OUT_MAIN_MIB_OUT:
						case E_MON_OUT_MAIN_PREDOWN:
							return E_TTT_2D_IN;
						case E_MON_OUT_PANEL:
							return ((eTTDOUT == E_TD_OUT_3D_LI)? E_TTT_PR_IN : E_TTT_SG_IN);
					}
				}
				else //other 3d 
				{
				
					switch (pMonOutInfo->u1MonOutInSel)
					{
						case E_MON_OUT_MAIN_PREDOWN:
							switch(eMIBIn)
							{
								case E_TD_IN_FS_I:
								case E_TD_IN_FS_P:
									return E_TTT_SG_IN;
								case E_TD_IN_SBS_I:
								case E_TD_IN_SBS_P:
								case E_TD_IN_RD_I:
								case E_TD_IN_RD_P:	
									return E_TTT_SBS_IN;
								case E_TD_IN_TB_I:
								case E_TD_IN_TB_P:
									return E_TTT_TB_IN;
								case E_TD_IN_LI_P:
									return E_TTT_PR_IN;
								default:
									return E_TTT_2D_IN;
							}
						case E_MON_OUT_MIXER:
						case E_MON_OUT_MAIN_FSC_OUT:
						case E_MON_OUT_MAIN_MIB_OUT:
							return E_TTT_SG_IN;
						case E_MON_OUT_PANEL:
							return ((eMJCOUT == E_TD_OUT_3D_LI)? E_TTT_PR_IN : E_TTT_SG_IN);
					}
				}
			}
			else    //2D
			{
				return E_TTT_2D_IN;
			}
		}
		else // 3D to 2D
		{
		
			return E_TTT_2D_IN;  //main TTT already do 3D to 2D if it is 3d to 2d now
		}
	}
	
	return E_TTT_2D_IN;
}


/**
 *  Update TTT input Resolution
 *  TTT input comes from DQ output.
 *  Currently, DQ will not change output resolution.
 */ 
void vVRMMonOutUpdateTTTResoluton( void )
{
	MonOutPicInfo* pMonOutInfo;
	pMonOutInfo = getMonOutInfo();
	LOG(1,"vVRMMonOutUpdateTTTResoluton \n");

	pMonOutInfo->w3Dto2DInWidth  = pMonOutInfo->wTargetW;   //3d to 2d_input = regen out
	pMonOutInfo->w3Dto2DInHeight = pMonOutInfo->wTargetH;   //3d to 2d input = regen output

	pMonOutInfo->wInput3DFmt = u4DrvMonOutTTTModeQuery(); 
	LOG(1,"Get monitor out 3D input is %d\n",pMonOutInfo->wInput3DFmt);

	pMonOutInfo->w3Dto2DOutWidth  = pMonOutInfo->w3Dto2DInWidth;
	pMonOutInfo->w3Dto2DOutHeight = pMonOutInfo->w3Dto2DInHeight;

	switch(pMonOutInfo->wInput3DFmt)
	{        
		//FS in, 2D out, FR/2
		case E_TTT_SG_IN:
			pMonOutInfo->u2OutFramerate= pMonOutInfo->u2InFramerate / 2;
			break;
		//SBS in, 2D out, width/2
		case E_TTT_SBS_IN:
			pMonOutInfo->w3Dto2DOutWidth = pMonOutInfo->w3Dto2DInWidth / 2;
			break;            
		//TAB or LI in, 2D out, height/2
		case E_TTT_TB_IN:
		case E_TTT_PR_IN:
			pMonOutInfo->w3Dto2DOutHeight = pMonOutInfo->w3Dto2DInHeight /2;
			break;
		default:
			break;
	}       

}

void vVRMUpdateMonOutOutputChange(void)
{
    MonOutPicInfo* pMonOutInfo;
//	APP_RES_T pVencres;
    pMonOutInfo = getMonOutInfo();
	if(pMonOutInfo->u1Output == E_MON_OUT_VENC)
	{
//		vDrvVrmGetAppRes(VRM_APP_VIDEO_ENC,&pVencres);	
//		pMonOutInfo->wVencHeight = pVencres.u4Height;
//		pMonOutInfo->wOutputWidth = pVencres.u4Width;
		LOG(1,"Use monitor out for Venc\n");
	}
	else
	{
		LOG(1,"Use monitor out for TVE,TimeGen select =%d\n",pMonOutInfo->u1TimeGenSel);   
	}
}

void vVRMFireMonOutModeChange(UINT32 u4FgMonOut)
{
    MonOutPicInfo* pMonOutInfo;
    pMonOutInfo = getMonOutInfo();

    pMonOutInfo->u4MonOutModeChg |= u4FgMonOut ;
}

void vVRMMonOutFireResolutionChange( void )
{    
    if( _vrm_mon_out_info.u1MonOutInSel != _rMonitorOutInfo.u1MonOutInSel ||
        _vrm_mon_out_info.u1MonOutInSel_2 != _rMonitorOutInfo.u1MonOutInSel_2)
    {
        vVRMFireMonOutModeChange( VRM_MON_OUT_IN_SEL_CHG );
    }

    if(x_memcmp((const void *)&_vrm_mon_out_info.rRegenScan
        ,(const void *)&_rMonitorOutInfo.rRegenScan,sizeof(VDP_PIC_OVERSCAN_REGION_T)))
    {
        vVRMFireMonOutModeChange( VRM_MON_OUT_REGEN_CHG );  
    }
	
    if( _vrm_mon_out_info.u2InFramerate != _rMonitorOutInfo.u2InFramerate )
    {
        vVRMFireMonOutModeChange( VRM_MON_OUT_INPUT_RES_CHG );
    }
	
    if( (_vrm_mon_out_info.wPdsInWidth  != _rMonitorOutInfo.wPdsInWidth ) || 
        (_vrm_mon_out_info.wPdsInHeight != _rMonitorOutInfo.wPdsInHeight) ||
        (_vrm_mon_out_info.wPdsOutWidth != _rMonitorOutInfo.wPdsOutWidth ) ||
        (_vrm_mon_out_info.wPdsOutHeight!= _rMonitorOutInfo.wPdsOutHeight) ||
        (_vrm_mon_out_info.u1Interlace	!= _rMonitorOutInfo.u1Interlace))
        
    {
        vVRMFireMonOutModeChange( VRM_MON_OUT_DOWN_SC_CHG );
    }
    if( _vrm_mon_out_info.u1Output != _rMonitorOutInfo.u1Output )
    {
        vVRMFireMonOutModeChange( VRM_MON_OUT_OUT_SEL_CHG );
    }

    if( _vrm_mon_out_info.u1TimeGenSel!= _rMonitorOutInfo.u1TimeGenSel)
    {
        vVRMFireMonOutModeChange( VRM_MON_OUT_TIME_GEN_CHG );
    }

	if( _vrm_mon_out_info.u1Output!= _rMonitorOutInfo.u1Output)
    {
        vVRMFireMonOutModeChange( VRM_MON_OUT_OUT_SEL_CHG );
    }
	
	if(( _vrm_mon_out_info.wOutputWidth	!= _rMonitorOutInfo.wOutputWidth) ||
	   ( _vrm_mon_out_info.wOutputHeight!= _rMonitorOutInfo.wOutputHeight))
    {
        vVRMFireMonOutModeChange( VRM_MON_OUT_OUTPUT_RES_CHG );
    }	
	
	if( _vrm_mon_out_info.wInput3DFmt != _rMonitorOutInfo.wInput3DFmt)
    {
        vVRMFireMonOutModeChange( VRM_MON_OUT_TTT_FMT_CHG );
    }
	
    LOG(1, "vVRMMonOutFireResolutionChange \n");
}



void vVRMMonOutUpdateResolution( void )
{
        
    LOG(2, "VRM monitor out Update resolution start.....\n");

    vVRMUpdateMonOutMUXin();
    vVRMUpdateMonOutOutputChange();	 //done.
    vVRMUpdateMonOutRegenResolution();   //done..
    vVRMMonOutUpdateTTTResoluton();
    vVRMMonOutUpdatePredownResolution();
    //Need to update following resolution even there's no input.
    LOG(1, "VRM Update monitor out resolution done......\n");
    return;
}



void vVRM_MonOutLoop( void )
{
    CRIT_STATE_T csState;

     if(VRM_MON_OUT_EVENT_NONE !=vVRMGetMonOutEventFlg())
      {
          LOG(1,"vVRM_MonOutLoop\n");      
          do
          {
              csState = x_crit_start();
          
              LOG(3, "========VRM_mon_out Loop Event flag (0x%X) ========\n",vVRMGetMonOutEventFlg());
      
              vVRMClrMonOutEventFlg(); 
              x_crit_end(csState);
              
              vVRMMonOutUpdateResolution();
      
          }while(VRM_MON_OUT_EVENT_NONE !=vVRMGetMonOutEventFlg());
      
          vVRMMonOutFireResolutionChange();		  
          x_memcpy((void*)&_vrm_mon_out_info,(void*)&_rMonitorOutInfo,sizeof(_rMonitorOutInfo));
      }
}
