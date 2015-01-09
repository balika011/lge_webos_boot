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
 * $RCSfile: drv_scaler_ptgen.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file drv_scaler.h
 *  Brief of file drv_scaler_ptgen.c.
 *  Details of file drv_scaler_ptgen.c (optional).
 */

#include "hw_tdc.h"
#include "hw_ycproc.h"

#include "drv_video.h"
#include "drv_scaler.h"
#if defined(CC_MT5399)
#include "drv_scaler_psc.h"
#endif

#include "drv_scaler_drvif.h"
#include "frametrack_drvif.h"
#include "drv_scpos.h"

#include "vdo_misc.h"
#include "general.h"
#include "video_def.h"
#include "mute_if.h"
#include "scpos_debug.h"
#include "vdo_rm.h"

#include "x_assert.h"

#if SUPPORT_DOT_TO_DOT_PATTERN
UINT32 bOri10BitMode; 
UINT32 bOriUsH; 
UINT32 bOriUsV; 
UINT32 bCount=0;
UINT16 bOriDataLen;
UINT16  bOriHL;
UINT16  bOriVL;
UINT16 bOriRHP;
UINT16 bOriRHL;
UINT16 bOriRVP;
UINT16 bOriRVL;
UINT8 bOriEN3D; 
UINT8 bOriDSIs444Mode; 
UINT8 bOriVUSisBypass; 
UINT8 bOriFSEL_1;
UINT8 bOriVOFST_BYPASS;
UINT8 bOriALLBP;
UINT8 bOriYUV2RGB;
UINT16 wOriHBoundary;
UINT32 dwPreBaseAddress; 
UINT8 bOriDramBurstMode;

extern void vWriteDramB(UINT32 u4Addr, UINT8 bData);

UINT8 G0PixelValue;   //the pixel0 R G B
UINT8 B0PixelValue;
UINT8 R0PixelValue;

UINT8 G1PixelValue;   //the pixel1 R G B
UINT8 B1PixelValue;
UINT8 R1PixelValue;

UINT8 BeginRevLine;//begin reverse the Revline's head of a line's pixel. 
UINT8 RevLine = 0;    
UINT8 SpecialRevLine = 0;
#endif

/**
 * @8098, scaler self gen pattern, main only
 */
void vScpipSelfInputPatternOnOff(UINT32 u4OnOff)
{
	
	if (u4OnOff == SV_ON)
	{
		UINT32 u4InputW, u4InputH, u4Htotal, u4Vtotal;
		VDP_SCALER_PRM_T* scalerInfo;
		scalerInfo = getScalerInfo(SV_VP_MAIN);

		if(scalerInfo == NULL)
		{
			return;
		}
		else
		{
			u4InputW = scalerInfo->fscIn.u2Width;
			u4InputH = scalerInfo->fscIn.u2Height;
			u4Htotal = scalerInfo->fscIn.u2HTotal;
			u4Vtotal = scalerInfo->fscIn.u2VTotal;
		}

		vScpipSetInPtgBndSize(VDP_1, u4InputW, u4InputH);
		vScpipSetSelfPtgRes(u4InputW, u4InputH, u4Htotal, u4Vtotal);
	}
	
	vScpipInPtgBndEnable(VDP_1, u4OnOff);
	vScpipInPtgEnable(VDP_1, u4OnOff);
}


/**
 * @8098, using pattern replace content
 */
void vScpipSetInputPattern(UINT32 u4VdpId, UINT32 u4OnOff, UINT32 u4Width, UINT32 u4Height)
{
    UINT8 bPath;
    VDP_SCALER_PRM_T* scalerInfo;

	bPath = getScalerPath(u4VdpId);
	scalerInfo = getScalerInfo(bPath);
	
    if (u4VdpId >= VDP_NS || scalerInfo == NULL) 
    { 
        return;
    }

    bPath = getScalerPath(u4VdpId);
    
    if (u4OnOff == SCPIP_PTGEN_AUTO) // auto mode
    {
        UINT32 u4InputW, u4InputH;
        UINT32 u4YUV;
        //get width, height
		u4InputW = scalerInfo->fscIn.u2Width;
		u4InputH = scalerInfo->fscIn.u2Height;
		
		u4YUV = (bIsScalerInput444(bPath)? 0: 1);
		
        LOG(3, "VDP(%d), SC input size(%d, %d) YUV(%d)\n", u4VdpId, u4InputW, u4InputH, u4YUV);
		vScpipSetInPtgStyle(u4VdpId);
		vScpipSetInPtgBndSize(u4VdpId, u4InputW, u4InputH);

        // disable frame track, is it needed in 5398? just replace active using pattern
        //u4ForceFrameTrackCtrl |= SCPIP_FRAME_TRACK_OFF_DS_PATTERN;
        // turn on pattern
		vScpipInPtgBndEnable(u4VdpId, SV_ON);
		vScpipInPtgEnable(u4VdpId, SV_ON); 
    }
    else if (u4OnOff == SCPIP_PTGEN_MANUAL)  //manual mode
    {	
    	vScpipSetInPtgStyle(u4VdpId);
    	vScpipSetInPtgBndSize(u4VdpId, u4Width, u4Height);
        // disable frame track
        //u4ForceFrameTrackCtrl |= SCPIP_FRAME_TRACK_OFF_DS_PATTERN;
        // turn on pattern
        
		vScpipInPtgBndEnable(u4VdpId, SV_ON);
		vScpipInPtgEnable(u4VdpId, SV_ON);
    }
    else // off
    {
		vScpipInPtgBndEnable(u4VdpId, SV_OFF);
		vScpipInPtgEnable(u4VdpId, SV_OFF);  
		
        //u4ForceFrameTrackCtrl &= (~SCPIP_FRAME_TRACK_OFF_DS_PATTERN);    
    }
}

/**
 * @8096
 */
void vScpipSetOutputPattern(UINT32 u4VdpId, UINT32 u4OnOff, UINT32 u4Width, UINT32 u4Height)
{
    UINT8 bPath;    
    VDP_SCALER_PRM_T* scalerInfo;

	bPath = getScalerPath(u4VdpId);	
	scalerInfo = getScalerInfo(bPath);
   
    if (u4VdpId >= VDP_NS || scalerInfo == NULL) 
    { 
        return;
    }

     
    if ((u4OnOff == SCPIP_PTGEN_AUTO) || (u4OnOff == SCPIP_PTGEN_MANUAL))
    {
        if (u4OnOff == SCPIP_PTGEN_AUTO)        
        {
			u4Width = scalerInfo->fscOut.u2Width;
			u4Height = scalerInfo->fscOut.u2Height;
            LOG(3, "VDP(%d), SC output size (%d, %d)\n", u4VdpId, u4Width, u4Height);
        }
		vScpipSetOutPtgStyle(u4VdpId);
		vScpipSetOutPtgBndSize(u4VdpId, u4Width, u4Height);
		vScpipOutPtgBndEnable(u4VdpId, SV_ON);                      
		vScpipOutPtgEnable(u4VdpId, SV_ON);
    }
    else // off
    {
		vScpipOutPtgBndEnable(u4VdpId, SV_OFF);
		vScpipOutPtgEnable(u4VdpId, SV_OFF);
    }
}

#ifdef CC_MT5399
void vScpipSetPostScalerPattern(UINT32 u4InOut, UINT32 u4OnOff, UINT32 u4Width, UINT32 u4Height)
{
	if (u4InOut == 0) // post scaler in
	{
        if (u4OnOff == SCPIP_PTGEN_OFF)
        {
        	vPscSetInputPatternGenBoundaryEnable(SV_OFF);
        	vPscSetInputPatternGenEnable(SV_OFF);
            return;
        }

        if (u4OnOff == SCPIP_PTGEN_AUTO)
        {
            u4Width  = wDISPLAY_WIDTH;
            u4Height = wDISPLAY_HEIGHT;			
        }
		vPscSetInPtgStyle();
		vPscSetInputPatternGenBoundary(u4Width, u4Height);
		vPscSetInputPatternGenBoundaryEnable(SV_ON);
		vPscSetInputPatternGenEnable(SV_ON);
	}
	else // post scaler out
	{
        if (u4OnOff == SCPIP_PTGEN_OFF)
        {
        	vPscSetInputPatternGenBoundaryEnable(SV_OFF);
            vPscSetOutputPatternGenEnable(SV_OFF);
            return;
        }

        if (u4OnOff == SCPIP_PTGEN_AUTO)
        {
            u4Width  = wDISPLAY_WIDTH;
            u4Height = wDISPLAY_HEIGHT;
        }
		vPscSetOutPtgStyle();
		vPscSetOutputPatternGenBoundary(u4Width, u4Height);
		vPscSetOutputPatternGenBoundaryEnable(SV_ON);
		vPscSetOutputPatternGenEnable(SV_ON);
	}
}
#endif

#if SUPPORT_DOT_TO_DOT_PATTERN
/*
 *  @review this
 */
static void VDP_GetPattern(UINT8 bIndex)
{
	    switch(bIndex)
        {
            case 0: //black-white dot
                G0PixelValue = 0x00;
                B0PixelValue = 0x00;
                R0PixelValue = 0x00;
                G1PixelValue = 0xff;
                B1PixelValue = 0xff;
                R1PixelValue = 0xff;
                BeginRevLine = 1;
                break;
            case 1: //black-green dot
                G0PixelValue = 0x00;
                B0PixelValue = 0x00;
                R0PixelValue = 0x00;
                G1PixelValue = 0xff;
                B1PixelValue = 0x00;
                R1PixelValue = 0x00;
                BeginRevLine = 1;
                break;

            case 2: //black-blue dot
                G0PixelValue = 0x00;
                B0PixelValue = 0x00;
                R0PixelValue = 0x00;
                G1PixelValue = 0x00;
                B1PixelValue = 0xff;
                R1PixelValue = 0x00;
                BeginRevLine = 1;
                break;
            case 3: //black-red dot
                G0PixelValue = 0x00;
                B0PixelValue = 0x00;
                R0PixelValue = 0x00;
                G1PixelValue = 0x00;
                B1PixelValue = 0x00;
                R1PixelValue = 0xff;
                BeginRevLine = 1;
                break;
            case 4: //black-green dot
                G0PixelValue = 0x00;
                B0PixelValue = 0x00;
                R0PixelValue = 0x00;
                G1PixelValue = 0x7f;
                B1PixelValue = 0x00;
                R1PixelValue = 0x00;
                BeginRevLine = 2;
                break;
            case 5: //black-bule dot
                G0PixelValue = 0x00;
                B0PixelValue = 0x00;
                R0PixelValue = 0x00;
                G1PixelValue = 0x00;
                B1PixelValue = 0x7f;
                R1PixelValue = 0x00;
                BeginRevLine = 2;
                break;
            case 6: //black-red dot
                G0PixelValue = 0x00;
                B0PixelValue = 0x00;
                R0PixelValue = 0x00;
                G1PixelValue = 0x00;
                B1PixelValue = 0x00;
                R1PixelValue = 0x7f;
                BeginRevLine = 2;
                break;
            case 7: //special black-green dot
                G0PixelValue = 0x00;
                B0PixelValue = 0x00;
                R0PixelValue = 0x00;
                G1PixelValue = 0x7f;
                B1PixelValue = 0x00;
                R1PixelValue = 0x00;
                BeginRevLine = 2;
                SpecialRevLine = 1;
                break;
            case 8: //special black-blue dot
                G0PixelValue = 0x00;
                B0PixelValue = 0x00;
                R0PixelValue = 0x00;
                G1PixelValue = 0x00;
                B1PixelValue = 0x7f;
                R1PixelValue = 0x00;
                BeginRevLine = 2;
                SpecialRevLine = 1;
                break;
            case 9: //special black-red dot
                G0PixelValue = 0x00;
                B0PixelValue = 0x00;
                R0PixelValue = 0x00;
                G1PixelValue = 0x00;
                B1PixelValue = 0x00;
                R1PixelValue = 0x7f;
                BeginRevLine = 2;
                SpecialRevLine = 1;
                break;
            default:
                G0PixelValue = 0x00;
                B0PixelValue = 0x00;
                R0PixelValue = 0x00;
                G1PixelValue = 0x7f;
                B1PixelValue = 0x00;
                R1PixelValue = 0x00;
                BeginRevLine = 2;
                SpecialRevLine = 1;
                break;
        }
}

/**
 *  @review this
 */
void vScpipDotToDotPattern(UINT8 ucOnOff,UINT32 bIndex)
{	     
}
#endif

