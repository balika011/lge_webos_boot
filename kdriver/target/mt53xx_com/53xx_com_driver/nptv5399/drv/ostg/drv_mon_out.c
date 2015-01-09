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
 *   $Workfile: drv_monitorout.c $
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
#include "hw_mon_out.h"
#include "drv_mon_out.h"
#include "drv_tve.h"
#include "drv_display.h"
#include "drv_tdtv_drvif.h"
#include "scpos_debug.h"
#include "vdo_misc.h"
#include "vdo_rm.h"
#include "video_def.h"
#include "drv_scaler.h"
#include "drv_regen.h"
#include "drv_scaler_gfx.h"

void vMonOutSetDramHVPitch(UINT32 u4DramWidth, UINT32 u4DramHeight);
void vMonOutSetDramWidthHeight(UINT32 u4DramWidth, UINT32 u4DramHeight);

EXTERN void vScpipOstgMonSrcSel(UINT8 u1Sel);
EXTERN void vVRMSetMonOutEventFlg( UINT32 u4MonOutFlag );

extern void TVE_ScalerSetSyncTime1(void);
extern void TVE_ScalerSetDacOnOff(UCHAR ucOn);

extern void TVE_InterlaceInput(UINT8 u8OnOFF);


#if defined(CC_MT5399)
UINT8 uMonOutEnable = SV_TRUE;
#else
UINT8 uMonOutEnable = SV_FALSE;
#endif

UINT8 uDraminit = SV_FALSE;

static BOOL fgMonDotByDotOutput = SV_FALSE;


//----------------------------------------------------------------------------------
//
//
//----------------------------------------------------------------------------------
void vMonOutSetMonCenSel (UINT8 u1Mon_cen_sel)
{
    MONOUT_WRITE_FLD(SCPIP_MON_CTRL_03, u1Mon_cen_sel, SCPIP_MON_OUT_MON_CEN_SEL);
}
//----------------------------------------------------------------------------------
//
//
//----------------------------------------------------------------------------------
void vMonOutSetHscalerbypass(UINT8 u1bypass)
{
    MONOUT_WRITE_FLD(SCPIP_MON_PRE_DS_05, u1bypass, SCPIP_MON_OUT_PRE_DSCALE_H_BYPASS_1);
}
//----------------------------------------------------------------------------------
//
//
//----------------------------------------------------------------------------------
void vMonOutSetVscalerbypass(UINT8 u1bypass)
{
    MONOUT_WRITE_FLD(SCPIP_MON_PRE_DS_05, u1bypass, SCPIP_MON_OUT_PRE_DSCALE_V_BYPASS_1);
}
//----------------------------------------------------------------------------------
//
//
//----------------------------------------------------------------------------------
void vMonOutBypassRGB2YUV (UINT8 Rgb2Yuv)
{
	MONOUT_WRITE_FLD(SCPIP_MON_CTRL_03, Rgb2Yuv, SCPIP_MON_OUT_MON_RGB2YUV_BYPASS);
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void vMonOutYc2yuvBeforeMonRegen(UINT8 Yc2Yuv)
{
    MONOUT_WRITE_FLD(SCPIP_MON_CTRL_04, Yc2Yuv, SCPIP_MON_OUT_MON_YC2YUV_EN);
}
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void vMonOutScDramWr444_1(UINT8 Yc2Yuv)
{
    MONOUT_WRITE_FLD(SCPIP_MON_CTRL_02, Yc2Yuv, SCPIP_MON_OUT_SC_DRAM_W_444_1);
}
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void vMonOutTVETG422to444_1(UINT8 Yc2Yuv)
{
	MONOUT_WRITE_FLD(SCPIP_MON_CTRL_05, Yc2Yuv, SCPIP_MON_OUT_TVE_YC2YUV_EN);
}
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void vMonOutBeforeDram444to422_1(UINT8 Yc2Yuv)
{
	MONOUT_WRITE_FLD(SCPIP_MON_CTRL_02, Yc2Yuv, SCPIP_MON_OUT_MON_DRAM_YUV2YC_EN);
}
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void vMonOutRegenActiveInv(UINT8 regen_active_inv)
{
    //direct 0xf0025ea8[8:8]       1     //set regen_active_inv =1
    MONOUT_WRITE_FLD(SCPIP_MON_CTRL_02, regen_active_inv, SCPIP_MON_OUT_REGEN_V_ACT_INV);
}
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void vMonOutSetDramLinePitch( UINT8 u1_da_dram_line_pitch )
{
    MONOUT_WRITE_FLD(SCPIP_DRAM_MON_00, u1_da_dram_line_pitch, SCPIP_MON_OUT_DA_DRAM_LINE_PITCH);
}
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void vMonOutSetDramFrameNum( UINT8 u1FrameNum )
{
    MONOUT_WRITE_FLD(SCPIP_DRAM_MON_00, u1FrameNum, SCPIP_MON_OUT_DA_FRAME_NUM);  
}
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
void vMonOutSetDramReadEnable(UINT8 fgEnable)
{
    MONOUT_WRITE_FLD(SCPIP_DRAM_MON_00, fgEnable, SCPIP_MON_OUT_DA_READ_ENABLE);  
}

UINT8 vMonOutGetDramReadEnable(void)
{
	return MONOUT_READ_FLD(SCPIP_DRAM_MON_00, SCPIP_MON_OUT_DA_READ_ENABLE);  
}

//----------------------------------------------------------------------------------
void vMonOutSetDramWriteEnable(UINT8 fgEnable)
{
    MONOUT_WRITE_FLD(SCPIP_DRAM_MON_00, fgEnable, SCPIP_MON_OUT_DA_WRITE_ENABLE);  
}

UINT8 vMonOutGetDramWriteEnable(void)
{
	return MONOUT_READ_FLD(SCPIP_DRAM_MON_00, SCPIP_MON_OUT_DA_WRITE_ENABLE);  
}

UINT8 u1MonOutGetFldInfo(void)
{
	return (UINT8)MONOUT_READ_FLD(SCPIP_DRAM_MON_02, SCPIP_MON_OUT_STA_FLD_INFO);  
}

#ifdef CC_FLIP_MIRROR_SUPPORT
void _vMonOutSetMirrorFlipReg(void)
{
    MonOutPicInfo* pMonOutInfo = getMonOutInfo();
    
    UINT8 bMirrorFlipMode = SV_OFF;
    
    if (u1GetFlipMirrorConfig() & SYS_FLIP_CONFIG_ON)
    {
        if(pMonOutInfo->u1MonOutInSel == E_MON_OUT_PANEL)
        {
            bMirrorFlipMode = SV_ON ;
            bMirrorFlipMode = bMirrorFlipMode << 1;
        }

    }    
    if (u1GetFlipMirrorConfig() & SYS_MIRROR_CONFIG_ON)
    {
        if(pMonOutInfo->u1MonOutInSel == E_MON_OUT_PANEL)
        {
            bMirrorFlipMode = bMirrorFlipMode | SV_ON;
        }  
    }
    MONOUT_WRITE_FLD(SCPIP_DRAM_MON_00, bMirrorFlipMode, SCPIP_MON_OUT_DA_DISPLAY_MODE);
}
#endif

static void _vMonOutSetReadPosGen(UINT32 ucRpt)
{
	MONOUT_WRITE_FLD(SCPIP_MON_CTRL_03, ucRpt, SCPIP_MON_OUT_SC_RPT_GEN_VCNT_1);	
}

void vMonOutModuleCfg(void)
{
    MonOutPicInfo* pMonOutInfo;
    pMonOutInfo = getMonOutInfo();

    LOG(0, "Mon Out Config\n");
    LOG(0, "==============================================================================\n");
    LOG(0, "MonOut is %s\n",uMonOutEnable? "Enable":"Disable");	
    LOG(0, "MonOut MuxInSel=%d\n",pMonOutInfo->u1MonOutInSel);
	LOG(0, "MonOut Interlace=%d\n",pMonOutInfo->u1Interlace);
    LOG(0, "MonOut Tve Fmt is %s\n",(pMonOutInfo->u1TimeGenSel ? "PAL":"NTSC") );
	LOG(0, "MonOut input width %d height %d\n",pMonOutInfo->wSrcW,pMonOutInfo->wSrcH);	
	LOG(0, "MonOut input 3D fmt is %d (0:2d 1:PR 2:SG 3:SBS 4:TB)\n",pMonOutInfo->wInput3DFmt);	
	LOG(0, "MonOut Regen x %d y %d width %d height %d\n",pMonOutInfo->wXOffset,pMonOutInfo->wYOffset,pMonOutInfo->wTargetW,pMonOutInfo->wTargetH);	
	LOG(0, "MonOut TTT input width %d height %d\n",pMonOutInfo->w3Dto2DInWidth,pMonOutInfo->w3Dto2DInHeight);	
	LOG(0, "MonOut TTT output width %d height %d\n",pMonOutInfo->w3Dto2DOutWidth,pMonOutInfo->w3Dto2DOutHeight);	
	LOG(0, "MonOut scaler input width %d height %d\n",pMonOutInfo->wPdsInWidth,pMonOutInfo->wPdsInHeight);	
	LOG(0, "MonOut scaler output width %d height %d\n",pMonOutInfo->wPdsOutWidth,pMonOutInfo->wPdsOutHeight);	
	LOG(0, "MonOut Output is %d(0:tve  1:venc 2:GFX)\n",pMonOutInfo->u1Output);	
	LOG(0, "MonOut Output width %d height %d\n",pMonOutInfo->wOutputWidth,pMonOutInfo->wOutputHeight);
	LOG(0, "MonOut InFR %d OutFR %d\n",pMonOutInfo->u2InFramerate,pMonOutInfo->u2OutFramerate);
	LOG(0, "MonOut dram read %d dram write %d\n",vMonOutGetDramReadEnable(),vMonOutGetDramWriteEnable());
    LOG(0, "==============================================================================\n"); 
}

//---------------------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------------------
void vMonOutDramWr3DInputEnable(UINT8 fgEnable3DInput )
{  
	MONOUT_WRITE_FLD(SCPIP_DRAM_MON_0C, fgEnable3DInput, SCPIP_MON_OUT_DA_3D_INPUT_EN);
}

void vMonOutDramWr3DInputFpLrSwap(UINT8 fg3DLrSwap )
{  
	MONOUT_WRITE_FLD(SCPIP_DRAM_MON_0C, fg3DLrSwap, SCPIP_MON_OUT_DA_3D_INPUT_LR_SWAP);
}
/*
u13DMode=
 0: frame packing
 1:side-by-side
 2: top -bottom
 3: line interlave
*/
void vMonOutDram3DInputMode(UINT8 u13DMode )
{  
	MONOUT_WRITE_FLD(SCPIP_DRAM_MON_0C, u13DMode, SCPIP_MON_OUT_DA_3D_INPUT_MODE);
}


void vMonOutDramWr3DOutputEnable(UINT8 fgEnable3Doutput )
{  
	MONOUT_WRITE_FLD(SCPIP_DRAM_MON_0C, fgEnable3Doutput, SCPIP_MON_OUT_DA_3D_OUTPUT_EN);
}

/*
u13DMode=
 0: PR
 1:FULL PR
 2: side-by-side
 3: shutter
*/
void vMonOutDram3DOutputMode(UINT8 u13DoutMode )
{  
	MONOUT_WRITE_FLD(SCPIP_DRAM_MON_0C, u13DoutMode, SCPIP_MON_OUT_DA_3D_OUTPUT_MODE);
}


void vMonOutPip2TveInterlaced(UINT8 u1interlace )
{  
	MONOUT_WRITE_FLD(SCPIP_MON_TVE_14, u1interlace, SCPIP_MON_OUT_SC_PIP2TVE_INTERLACED);
}


void vMonOutPip2TveFldInv(UINT8 u1fldinv )
{  
	MONOUT_WRITE_FLD(SCPIP_MON_TVE_14, u1fldinv, SCPIP_MON_OUT_SC_PIP2TVE_FLD_INV);
}


//mon_tve_
void vMonOutTveMonDram3D(UINT8 mon_dram_3d_enable )        //mon_dram_3d_enable=1 
{
    MONOUT_WRITE_FLD(SCPIP_MON_TVE_10, mon_dram_3d_enable, SCPIP_MON_OUT_MON_DRAM_3D_ENABLE);
}
void vMonOutTveMonDramfrmDly(UINT8 mon_dram_frm_dly )    //mon_dram_frm_dly=1
{
    MONOUT_WRITE_FLD(SCPIP_MON_TVE_10, mon_dram_frm_dly, SCPIP_MON_OUT_MON_DRAM_FRM_DLY);
}
void vMonOutTveRptMode(UINT8 tve_rpt_mode )          //tve_rpt_mode=1:free run,
{
    MONOUT_WRITE_FLD(SCPIP_MON_TVE_10, tve_rpt_mode, SCPIP_MON_OUT_MON_TVE_RPT_MODE);
}


//---------------------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------------------
void vMonOutSetDramAccesYbuffer( UINT32 u4MonDramYMsb, UINT32 u4MonDramYLsb )
{
	MONOUT_WRITE_FLD(SCPIP_DRAM_MON_04, u4MonDramYMsb, SCPIP_MON_OUT_DA_ADDR_BASE_MSB_Y);
	MONOUT_WRITE_FLD(SCPIP_DRAM_MON_05, u4MonDramYLsb, SCPIP_MON_OUT_DA_ADDR_BASE_LSB_Y);
}

void vMonOutSetDramAccesUbuffer( UINT32 u4MonDramUMsb, UINT32 u4MonDramULsb )
{
	MONOUT_WRITE_FLD(SCPIP_DRAM_MON_06, u4MonDramUMsb, SCPIP_MON_OUT_DA_ADDR_BASE_MSB_U);
	MONOUT_WRITE_FLD(SCPIP_DRAM_MON_07, u4MonDramULsb, SCPIP_MON_OUT_DA_ADDR_BASE_LSB_U);
}

void vMonOutSetDramAccesVbuffer( UINT32 u4MonDramVMsb, UINT32 u4MonDramVLsb )
{
	MONOUT_WRITE_FLD(SCPIP_DRAM_MON_08, u4MonDramVMsb, SCPIP_MON_OUT_DA_ADDR_BASE_MSB_V);
	MONOUT_WRITE_FLD(SCPIP_DRAM_MON_09, u4MonDramVLsb, SCPIP_MON_OUT_DA_ADDR_BASE_LSB_V);
}

void vMonOutSetDramAccesLimit( UINT32 u4MonDramHighLimit, UINT32 u4MonDramLowLimit )
{
	MONOUT_WRITE_FLD(SCPIP_DRAM_MON_0A, u4MonDramHighLimit, SCPIP_MON_OUT_DA_WADDR_HI_LIMIT);
	MONOUT_WRITE_FLD(SCPIP_DRAM_MON_0B, u4MonDramLowLimit, SCPIP_MON_OUT_DA_WADDR_LO_LIMIT);
}
//---------------------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------------------
void vFrameStartAtVsync(UINT8 fgFlag)  //0 : falling , 1:rising
{
    MONOUT_WRITE_FLD(SCPIP_MON_CTRL_04, fgFlag, SCPIP_MON_OUT_YC2YUV_FRM_STR_FALL);
}
//---------------------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------------------
void vMonOutCheckInterlaceInput( UINT8 u1MonOutInterlace )
{
    #ifdef CC_SUPPORT_VENC
    MonOutPicInfo* pMonOutInfo;
	pMonOutInfo = getMonOutInfo();
    #endif
    
	if ( u1MonOutInterlace )    //only pre_down main or pre_down sub may have interlace signal for MON_Out
	{
        #ifdef CC_SUPPORT_VENC
            if(pMonOutInfo->u1Output == E_MON_OUT_VENC)
            {
                vMonOutDramWr3DInputEnable(0);
            }
            else
            {
               vMonOutDramWr3DInputEnable(1);  
            }
        #else
		    vMonOutDramWr3DInputEnable(1);
        #endif

		vMonOutDramWr3DInputFpLrSwap(1);
		vMonOutDram3DInputMode(0);   //3d input to frame packing mode
		vMonOutDramWr3DOutputEnable(1);
		vMonOutDram3DOutputMode(3);

		vMonOutPip2TveInterlaced(1);    //progressive: 0 / interlace =1
		vMonOutPip2TveFldInv(1);        //sc_pip2tve_fld_inv 

		//MON_OUT _TVE
		vMonOutTveMonDram3D(1);        //mon_dram_3d_enable=1 
		vMonOutTveMonDramfrmDly(1);    //mon_dram_frm_dly=1
		vMonOutTveRptMode(1);          //tve_rpt_mode=1:free run,

        TVE_InterlaceInput(1);
	}
	else
	{
		// if  not 3d input 
		vMonOutDramWr3DInputEnable(0);
		vMonOutDramWr3DInputFpLrSwap(0);

		vMonOutDram3DInputMode(0);   //todo: check if 3d input, set the input 3d mode
		vMonOutDramWr3DOutputEnable(0);
		vMonOutDram3DOutputMode(0);

		vMonOutPip2TveInterlaced(0);     //progressive: 0 / interlace =1
		vMonOutPip2TveFldInv(0);        //sc_pip2tve_fld_inv 

		//MON_OUT_TVE
		vMonOutTveMonDram3D(0);        //mon_dram_3d_enable=1 
		vMonOutTveMonDramfrmDly(0);    //mon_dram_frm_dly=1
		vMonOutTveRptMode(0);          //tve_rpt_mode=1:free run,
        
        TVE_InterlaceInput(0);
	}
}

UINT8 vGetMonOutInputIsRGB(void)
{
	UINT8 bIsRGB;
	UINT8 bPath;
	MonOutPicInfo* pMonOutInfo;
	pMonOutInfo = getMonOutInfo();
	if((pMonOutInfo->u1MonOutInSel == E_MON_OUT_SUB_FSC_OUT) ||
		(pMonOutInfo->u1MonOutInSel == E_MON_OUT_SUB_MIB_OUT)||
		(pMonOutInfo->u1MonOutInSel == E_MON_OUT_SUB_PREDOWN))
	{		
		bPath = VDP_2;
	}
	else
	{
		bPath = VDP_1;
	}

	bIsRGB = bIsScalerInputRGB(bPath);
	if(pMonOutInfo->u1MonOutInSel == E_MON_OUT_PANEL)
	{
		bIsRGB = SV_TRUE;
	}	
	LOG(1,"Monitor out input is %s\n",bIsRGB? "RGB":"YUV");
	return bIsRGB;
}


/*
0x0d240[2:0]
0: oclk
1: oclk/2
2: poclk_dpll_ck
3: mib_oclk
*/
void vMonOutUpdateMuxSelIn( void)
{
	MonOutPicInfo* pMonOutInfo;
	pMonOutInfo = getMonOutInfo();
	LOG(1, "update mux select in =%d\n", pMonOutInfo->u1MonOutInSel );
    if ( pMonOutInfo->u1MonOutInSel <= E_FSC_SUB_TO_TVE )   //mon_out to TVE
    {  
		//mon_out mux switch -- start...  
		MONOUT_WRITE_FLD(SCPIP_MON_CTRL_02,  pMonOutInfo->u1MonOutInSel , SCPIP_MON_OUT_MON_IN_SEL);   //mon_out input mux
		//check  the interlace if input select to MIB_main or MIB_SUB
		vMonOutCheckInterlaceInput( pMonOutInfo->u1Interlace );


		// 1. MON_OUT_CLK select
		// 2. RGB2YUV bypass
		// 3. mon_cen_sel   , de condition
		// 4. yc2yuv beofre mon_regen
		if(vGetMonOutInputIsRGB())
		{
			vMonOutBypassRGB2YUV(0);
		}
		else
		{
			vMonOutBypassRGB2YUV(1);
		}
		vMonOutBeforeDram444to422_1(1);
		vMonOutScDramWr444_1(0);
		vMonOutTVETG422to444_1(1);
		switch( pMonOutInfo->u1MonOutInSel )
		{
			case E_MON_OUT_MAIN_MIB_OUT:
			case  E_MON_OUT_SUB_MIB_OUT:
				vScpipSetMonOutCLKSelReg(FLD_POCLK_SEL_MIB_OCLK);   //set src clk
				vMonOutSetMonCenSel(0);                             //mon_cen_sel  , the de 
				vMonOutRegenActiveInv(0); 
				vMonOutYc2yuvBeforeMonRegen(1);                     //YC2YUV  be fore MON_OUT REGEN 
				vFrameStartAtVsync(FRAME_START_AT_RISING);
				break;  
			case  E_MON_OUT_MAIN_PREDOWN:
				vScpipSetMonOutCLKSelReg(FLD_POCLK_SEL_MIB_OCLK);   //set src clk
				vMonOutSetMonCenSel(0);                            //mon_cen_sel  , the de 
				vMonOutRegenActiveInv(0); 
				vMonOutYc2yuvBeforeMonRegen(0);                     //YC2YUV  be fore MON_OUT REGEN 
                vFrameStartAtVsync(FRAME_START_AT_RISING);
				break;  
			case  E_MON_OUT_SUB_PREDOWN:
				vScpipSetMonOutCLKSelReg(FLD_POCLK_SEL_MIB_OCLK);   //set src clk
				vMonOutSetMonCenSel(0);                            //mon_cen_sel  , the de 
				vMonOutRegenActiveInv(0); 
				vMonOutYc2yuvBeforeMonRegen(0);                     //YC2YUV  be fore MON_OUT REGEN 
                vFrameStartAtVsync(FRAME_START_AT_RISING);
				break;  
			case  E_MON_OUT_MAIN_FSC_OUT:
				if (fgIsMJCToOSTG())
				{
					vScpipSetMonOutCLKSelReg(FLD_POCLK_SEL_DPLL);   //set src pclk
				}
				else
			    {
			    	vScpipSetMonOutCLKSelReg(FLD_POCLK_SEL_OCLK);   //set src oclk
			    } 

				vMonOutSetMonCenSel(1);                         //mon_cen_sel  , the de tie to 1
				vMonOutRegenActiveInv(1); 
				vMonOutYc2yuvBeforeMonRegen(0);                 //YC2YUV  be fore MON_OUT REGEN 
                vFrameStartAtVsync(FRAME_START_AT_FALLING);
				break;  
			case  E_MON_OUT_SUB_FSC_OUT:
				if (fgIsMJCToOSTG())
				{
					vScpipSetMonOutCLKSelReg(FLD_POCLK_SEL_DPLL);   //set src pclk
				}
				else
			    {
			    	vScpipSetMonOutCLKSelReg(FLD_POCLK_SEL_OCLK);   //set src oclk
			    } 
				vMonOutSetMonCenSel(1);                         //mon_cen_sel  , the de tie to 1
				vMonOutRegenActiveInv(1); 
				vMonOutYc2yuvBeforeMonRegen(0);                 //YC2YUV  be fore MON_OUT REGEN 
                vFrameStartAtVsync(FRAME_START_AT_FALLING);
              
				break;  
			case E_MON_OUT_MIXER:
				if (fgIsMJCToOSTG())
				{
					vScpipSetMonOutCLKSelReg(FLD_POCLK_SEL_DPLL);   //set src pclk
				}
				else
			    {
			    	vScpipSetMonOutCLKSelReg(FLD_POCLK_SEL_OCLK);   //set src oclk
			    } 

				vMonOutSetMonCenSel(1);                         //mon_cen_sel  , the de tie to 1            
				vMonOutRegenActiveInv(1); 
				vMonOutYc2yuvBeforeMonRegen(0);                 //YC2YUV  be fore MON_OUT REGEN 
                vFrameStartAtVsync(FRAME_START_AT_FALLING);
              
				break;
			case E_MON_OUT_PANEL:   
				vScpipSetMonOutCLKSelReg(FLD_POCLK_SEL_OCLK);  //set src clk            
				vMonOutSetMonCenSel(1);                        //mon_cen_sel  , the de tie to 1
				vMonOutRegenActiveInv(1); 
				vMonOutYc2yuvBeforeMonRegen(0);                 //YC2YUV  be fore MON_OUT REGEN             
                vFrameStartAtVsync(FRAME_START_AT_FALLING);            
				if ( pMonOutInfo->u1MonOutInSel_2 <= E_MON_OUT_WITH_NO_OSD )
				{
					vScpipOstgMonSrcSel( pMonOutInfo->u1MonOutInSel_2 );
				}
				else
				{
					LOG(1,"Error in OSTG mux in select.\n");
				}
				break;
		}

        //mon_out mux switch --- end...


		if((pMonOutInfo->u1Output == E_MON_OUT_TVE))   //fsc_sub to TVE
		{      
		    if ( pMonOutInfo->u1MonOutInSel == E_FSC_SUB_TO_TVE )
            {      
			   vIO32WriteFldAlign(SCPIP_SCSYS_14, 1,  SCSYS_14_SC_TVE_SRC_SEL);   //mux_13 =1 , FSC_SUB to TVE
			   vScpipSetOclkTveSelReg(FLD_OCLK_TVE_SEL_TVE27_CK);  //OCLK_TVE_SEL  TVE_27M
    		}
    		else
    		{			
    			vIO32WriteFldAlign(SCPIP_SCSYS_14, 0,  SCSYS_14_SC_TVE_SRC_SEL);   //mux_13 =0	 , MON_OUT TO TVE
    			vScpipSetOclkTveSelReg(FLD_OCLK_TVE_SEL_DPLL);	 //OCLK_TVE_SEL =  POCLK_DPLL		
    		}
       }
    }
}


//todo: memory location in DRAM,
UINT8 vMonOutUpdateDramLocate( void )
{
    UINT8  bIs10Bit=0;
    UINT16 wDramWidth,wDramHeight;
    UINT32 u4MemLengthMsb,u4MemLegthLsb; 
    UINT8  MonOutMuxSelIn;
    
    UINT32 u4MonOutMsbY,u4MonOutLsbY; 
    UINT32 u4MonOutMsbU,u4MonOutLsbU; 
    UINT32 u4MonOutMsbV,u4MonOutLsbV; 

    FBM_POOL_T* prFbmPool = NULL;
    
    MonOutPicInfo* pMonOutInfo;
    pMonOutInfo = getMonOutInfo();

    //check mon_out input select first,
    MonOutMuxSelIn=pMonOutInfo->u1MonOutInSel;
    if ( MonOutMuxSelIn > E_FSC_SUB_TO_TVE )
    {  //fsc_sub to TVE , not mon_out to TVE case!!
		return SV_FALSE;
    }
    //check 8/10 bit in VDP
    if ( ( MonOutMuxSelIn == E_MON_OUT_SUB_FSC_OUT) || 
		(MonOutMuxSelIn == E_MON_OUT_SUB_MIB_OUT) || 
		(MonOutMuxSelIn==E_MON_OUT_SUB_PREDOWN ) )
    {  //sub path
		bIs10Bit= u4ScpipGet10BitMode(getScalerVDP(SV_VP_PIP));
    }
    else
    {  //main video path
		bIs10Bit=u4ScpipGet10BitMode(getScalerVDP(SV_VP_MAIN));
    }

    //check memory length:
    wDramWidth=pMonOutInfo->wPdsOutWidth;
    wDramHeight=pMonOutInfo->wPdsOutHeight;
    
    LOG(1," wDramWidth = %d , wDramHeight=%d \n",wDramWidth,wDramHeight );
    u4MemLengthMsb= (((wDramWidth + 255 )/256)* ((( wDramHeight * 2 ) +7 )/ 8 )  );

    //if 10bits
    if(bIs10Bit == SV_TRUE)
	{
		u4MemLegthLsb= (((wDramWidth + 255 )/256)* ((( wDramHeight * 2 ) +  255 )/ 256 ) ) * 8;
	}
    else
	{
		u4MemLegthLsb=0;
	}

	//get dram locate from FBM ?
	if(pMonOutInfo->u1Output == E_MON_OUT_VENC)
	{
    	prFbmPool = FBM_GetPoolInfo(FBM_POOL_TYPE_VENC);   //take the VENC buffer
	}
	else if(pMonOutInfo->u1Output == E_MON_OUT_TVE)
	{
		prFbmPool = FBM_GetPoolInfo(FBM_POOL_TYPE_TVE);
	}
	else
	{
		return SV_FALSE;
	}
	
    if((prFbmPool == NULL) ||
		(prFbmPool->u4Size == 0))		
    {
    	uDraminit = SV_FALSE;
    	LOG(1,"Can't get fbm buffer\n");
		return SV_FALSE;
    }
	else
	{
		uDraminit = SV_TRUE;
	}
	LOG(1," prFbmPool->u4Addr = %x , u4Size=%x \n",prFbmPool->u4Addr,prFbmPool->u4Size );
	LOG(1," u4MemLengthMsb = %x , u4MemLegthLsb=%x \n",u4MemLengthMsb,u4MemLegthLsb );

	u4MonOutMsbY = prFbmPool->u4Addr / 0x800;
	u4MonOutLsbY = (u4MonOutMsbY + u4MemLengthMsb );

	u4MonOutMsbU = ( u4MonOutLsbY + u4MemLegthLsb );
	u4MonOutLsbU = ( u4MonOutMsbU + u4MemLengthMsb );

	u4MonOutMsbV = ( u4MonOutLsbU + u4MemLegthLsb );
	u4MonOutLsbV = ( u4MonOutMsbV + u4MemLengthMsb );

	vMonOutSetDramAccesYbuffer(u4MonOutMsbY,u4MonOutLsbY);
	vMonOutSetDramAccesUbuffer(u4MonOutMsbU,u4MonOutLsbU);
	vMonOutSetDramAccesVbuffer(u4MonOutMsbV,u4MonOutLsbV);
	vMonOutSetDramAccesLimit( (u4MonOutLsbV + u4MemLegthLsb), u4MonOutMsbY );
	return SV_TRUE;
}
//----------------------------------------------------
//
//----------------------------------------------------

void vMonOutUpdateDramParameter( void )
{
    UINT8  u1_da_dram_line_pitch;
    MonOutPicInfo* pMonOutInfo;
    UINT32 u4DramWidth,u4DramHeight;
	UINT32 u4RptVcnt;

	vMonOutSetDramReadEnable(SV_FALSE);
	vMonOutSetDramWriteEnable(SV_FALSE);

    //get the DRAM locate for monout
    if(vMonOutUpdateDramLocate() == SV_FALSE)
	{
		LOG(3,"Dram locate not success\n");
		return;
	}
	
    pMonOutInfo = getMonOutInfo();

     //set th eline pich anf width/height
    u4DramWidth=pMonOutInfo->wPdsOutWidth;
    u4DramHeight=pMonOutInfo->wPdsOutHeight;
	if((pMonOutInfo->u1Output == E_MON_OUT_VENC) && (pMonOutInfo->u1Interlace))
	{
    	vMonOutSetDramWidthHeight( u4DramWidth,u4DramHeight/2 );		
		vMonOutSetDramHVPitch(u4DramWidth,u4DramHeight/2);
	}
	else
	{
    	vMonOutSetDramWidthHeight( u4DramWidth,u4DramHeight );		
		vMonOutSetDramHVPitch(u4DramWidth,u4DramHeight);
	}
	if(pMonOutInfo->u1Interlace)
	{
		u4RptVcnt = u4DramHeight/2 - 1;
	}
	else
	{
		u4RptVcnt = u4DramHeight - 1;
	}	
	_vMonOutSetReadPosGen(u4RptVcnt);
    u1_da_dram_line_pitch = ( u4DramWidth + 255)/256 -1;
    vMonOutSetDramLinePitch( u1_da_dram_line_pitch );

    //set DRAM frame number ..
    if(pMonOutInfo->u1Output == E_MON_OUT_TVE)
	{
		vMonOutSetDramFrameNum( MON_OUT_DRAM_FRAME_NUM - 1 );   //frame_num=2 , reg setting 2-1=1
	}
	else
	{
		
		vMonOutSetDramFrameNum( MON_OUT_DRAM_FRAME_NUM + 1 -1 );   //venc frame_num=3 , reg setting 3-1=1
	}
}
//=============================================================
//
//=============================================================
void vMonOutTveMaskColor( UINT8 fgEnableMaskColor, UINT8 MaskColorY, UINT8 MaskColorU, UINT8 MaskColorV )
{
    MONOUT_WRITE_FLD(SCPIP_MON_TVE_00,  fgEnableMaskColor , SCPIP_MON_OUT_SC_TVE_MASK_EN);   //mon_out input mux
    MONOUT_WRITE_FLD(SCPIP_MON_TVE_00,  MaskColorY , SCPIP_MON_OUT_SC_TVE_MASK_Y);   //mon_out input mux
    MONOUT_WRITE_FLD(SCPIP_MON_TVE_00,  MaskColorU , SCPIP_MON_OUT_SC_TVE_MASK_U);   //mon_out input mux
    MONOUT_WRITE_FLD(SCPIP_MON_TVE_00,  MaskColorV , SCPIP_MON_OUT_SC_TVE_MASK_V);   //mon_out input mux

}
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
void vMonOutUpdateTveTg( void )
{
    MonOutPicInfo* pMonOutInfo;
    pMonOutInfo = getMonOutInfo();

    if( pMonOutInfo->u1TimeGenSel == TVE_FMT_576I)
    {
        MONOUT_WRITE_FLD(SCPIP_MON_TVE_0B, 864  , SCPIP_MON_OUT_SC_TVE_HTOTAL);   //tve h_total
        MONOUT_WRITE_FLD(SCPIP_MON_TVE_0B, 625  , SCPIP_MON_OUT_SC_TVE_VTOTAL);   //tve v_total
    }
    else   //TVE_FMT_480I
    {
        MONOUT_WRITE_FLD(SCPIP_MON_TVE_0B, 858  , SCPIP_MON_OUT_SC_TVE_HTOTAL);  //tve h_total
        MONOUT_WRITE_FLD(SCPIP_MON_TVE_0B, 525  , SCPIP_MON_OUT_SC_TVE_VTOTAL);  //tve v_total
    }
        
    MONOUT_WRITE_FLD(SCPIP_MON_TVE_0C, pMonOutInfo->wPdsOutWidth , SCPIP_MON_OUT_SC_TVE_WIDTH);   // Tve hsize
    MONOUT_WRITE_FLD(SCPIP_MON_TVE_0C, pMonOutInfo->wPdsOutHeight, SCPIP_MON_OUT_SC_TVE_HEIGHT);   //Tve Vsize

    MONOUT_WRITE_FLD(SCPIP_MON_TVE_0D, 0x0 , SCPIP_MON_OUT_SC_TVE_FRNT_HPORCH);    //sc_TVE_H back porch
    MONOUT_WRITE_FLD(SCPIP_MON_TVE_0D, 0x0 , SCPIP_MON_OUT_SC_TVE_BACK_HPORCH);   //sc_TVE_H back porch
    MONOUT_WRITE_FLD(SCPIP_MON_TVE_15, 0x0 , SCPIP_MON_OUT_SC_TVE_FRNT_VPORCH);    //sc_TVE_V back porch  
    MONOUT_WRITE_FLD(SCPIP_MON_TVE_15, 0x0 , SCPIP_MON_OUT_SC_TVE_BACK_VPORCH);   // sc_TVE_V froent porch

    MONOUT_WRITE_FLD(SCPIP_MON_TVE_14, 1 , SCPIP_MON_OUT_SC_PIP2TVE_ENABLE);       //  sc_TVE sc_pip2tve_enable 

    vMonOutTveMaskColor( ON , 0, 0x80,0x80 );  //TVE_mask to black color

}
//=============================================================
//
//
//=============================================================
void vRegenSetResAtive( void )  //SCPIP_MON_OUT_RG_M_SET_RES_ACTIVE
{
    MONOUT_WRITE_FLD(SCPIP_MON_REGEN_00, 1 , SCPIP_MON_OUT_RG_M_SET_RES_ACTIVE);       //  sc_TVE sc_pip2tve_enable 
}


void vMonOutUpdateRegen( void ) 
{
    MonOutPicInfo* pMonOutInfo;
    pMonOutInfo = getMonOutInfo();

    //update input resolution
    vRegenSetInputResolutionReg( 0 ,REGEN_MODULE_MONOUT_REGEN ,pMonOutInfo->wSrcW, pMonOutInfo->wSrcH );
    //update output resolution
    vRegenSetOutputResolutionReg( 0 ,REGEN_MODULE_MONOUT_REGEN , pMonOutInfo->wTargetW , pMonOutInfo->wTargetH );
    //regen x/y offset or 3D L,
    vRegenSetLStartReg(0, REGEN_MODULE_MONOUT_REGEN, pMonOutInfo->wXOffset,pMonOutInfo->wYOffset );
    //regen x/y offset 3D R
    vRegenSetRStartReg( 0, REGEN_MODULE_MONOUT_REGEN, pMonOutInfo->wXOffset,pMonOutInfo->wYOffset );    

    vRegenSetResAtive();  //SCPIP_MON_OUT_RG_M_SET_RES_ACTIVE
}
//=============================================================
//  Monitor Out 3D to 2D converter
// 
//=============================================================

void vMonOutTTTSetInputResolution(UINT16 inW, UINT16 inH)
{
    MONOUT_WRITE_FLD(SCPIP_MON_3D22D_00, inW, SCPIP_MON_OUT_R_UIIMGWIDTH_M);
    MONOUT_WRITE_FLD(SCPIP_MON_3D22D_00, inH, SCPIP_MON_OUT_R_UIIMGHEIGHT_M);
}



void vMonOutTTTConfig(void)
{

    MonOutPicInfo* pMonOutInfo;
    pMonOutInfo = getMonOutInfo();

	if(pMonOutInfo->u1Interlace)
	{
		vMonOutTTTSetInputResolution(pMonOutInfo->w3Dto2DInWidth, pMonOutInfo->w3Dto2DInHeight/2);
	}
	else
	{
		vMonOutTTTSetInputResolution(pMonOutInfo->w3Dto2DInWidth, pMonOutInfo->w3Dto2DInHeight);
	}
    switch(pMonOutInfo->wInput3DFmt)
    {
		case E_TTT_2D_IN:	
			MONOUT_WRITE_FLD(SCPIP_MON_3D22D_00, 0, SCPIP_MON_OUT_R_B3D22D_EN_M);
			break;
		case E_TTT_PR_IN:	
			MONOUT_WRITE_FLD(SCPIP_MON_3D22D_00, 1, SCPIP_MON_OUT_R_B3D22D_EN_M);
			MONOUT_WRITE_FLD(SCPIP_MON_3D22D_00, 3, SCPIP_MON_OUT_R_UI3DMODESEL_M);
			break;
		case E_TTT_SG_IN:
			MONOUT_WRITE_FLD(SCPIP_MON_3D22D_00, 1, SCPIP_MON_OUT_R_B3D22D_EN_M);
			MONOUT_WRITE_FLD(SCPIP_MON_3D22D_00, 0, SCPIP_MON_OUT_R_UI3DMODESEL_M);
			break;
		case E_TTT_SBS_IN:
			MONOUT_WRITE_FLD(SCPIP_MON_3D22D_00, 1, SCPIP_MON_OUT_R_B3D22D_EN_M);
			MONOUT_WRITE_FLD(SCPIP_MON_3D22D_00, 1, SCPIP_MON_OUT_R_UI3DMODESEL_M);
			break;
		case E_TTT_TB_IN:	
			MONOUT_WRITE_FLD(SCPIP_MON_3D22D_00, 1, SCPIP_MON_OUT_R_B3D22D_EN_M);
			MONOUT_WRITE_FLD(SCPIP_MON_3D22D_00, 2, SCPIP_MON_OUT_R_UI3DMODESEL_M);
 			break;
    }

	if(pMonOutInfo->wInput3DFmt != E_TTT_2D_IN)
	{
		switch(pMonOutInfo->u1MonOutInSel)
		{
			case E_MON_OUT_MIXER:
			case E_MON_OUT_MAIN_FSC_OUT:
			case E_MON_OUT_MAIN_PREDOWN:
			case E_MON_OUT_PANEL:
				MONOUT_WRITE_FLD(SCPIP_MON_3D22D_00, 1, SCPIP_MON_OUT_R_BMASKSEL_M);	
				break;
			case E_MON_OUT_MAIN_MIB_OUT:
				MONOUT_WRITE_FLD(SCPIP_MON_3D22D_00, 0, SCPIP_MON_OUT_R_BMASKSEL_M);
				break;
			default:
				MONOUT_WRITE_FLD(SCPIP_MON_3D22D_00, 0, SCPIP_MON_OUT_R_BMASKSEL_M);			
				break;
		}
	}
}


//=============================================================
//  
// 
//=============================================================

void vMonOutUpdatePreDownParameter(void)
{

	UINT16 u2pre_dscale_h_1,u2pre_dscale_h_init_1;
	UINT16 u2pre_dscale_v_1,u2pre_dscale_v_init_1;

	MonOutPicInfo* pMonOutInfo;
	pMonOutInfo = getMonOutInfo();

	LOG(1,"  wPdsOutWidth= %d , wPdsInWidth=%d  \n", pMonOutInfo->wPdsOutWidth, pMonOutInfo->wPdsInWidth);	
	LOG(1,"  wPdsOutHeight=%d , wPdsInHeight=%d \n", pMonOutInfo->wPdsOutHeight,pMonOutInfo->wPdsInHeight);
        
    
	if((pMonOutInfo->wPdsInWidth != 0) && (pMonOutInfo->wPdsInWidth >= pMonOutInfo->wPdsOutWidth))
	{
		u2pre_dscale_h_1 = pMonOutInfo->wPdsOutWidth * DFT_DS_FACTOR / pMonOutInfo->wPdsInWidth;
	}
	else
	{
		u2pre_dscale_h_1 = DFT_DS_FACTOR;
	}
	if((pMonOutInfo->wPdsInHeight != 0) && (pMonOutInfo->wPdsInHeight >= pMonOutInfo->wPdsOutHeight))
	{
		u2pre_dscale_v_1 = pMonOutInfo->wPdsOutHeight * DFT_DS_FACTOR / pMonOutInfo->wPdsInHeight;
	}
	else
	{
		u2pre_dscale_v_1 = DFT_DS_FACTOR;
	}
	u2pre_dscale_h_init_1 = DFT_DS_FACTOR - u2pre_dscale_h_1;
	u2pre_dscale_v_init_1 = DFT_DS_FACTOR - u2pre_dscale_v_1;


	LOG(1,"u2pre_dscale_h_1=%x ,  u2pre_dscale_v_1=%x\n", u2pre_dscale_h_1,u2pre_dscale_v_1 );
	if(pMonOutInfo->wPdsOutWidth > 1280)
	{
		vMonOutSetHscalerbypass(1);
		vMonOutSetVscalerbypass(1);
	}
	else
	{		
		vMonOutSetHscalerbypass(0);
		vMonOutSetVscalerbypass(0);
	}
	MONOUT_WRITE_FLD(SCPIP_MON_PRE_DS_04,pMonOutInfo->wPdsInWidth,SCPIP_MON_OUT_PREDS_MAIN_IN_H );
	MONOUT_WRITE_FLD(SCPIP_MON_PRE_DS_04,pMonOutInfo->wPdsInHeight,SCPIP_MON_OUT_PREDS_MAIN_IN_V );

	MONOUT_WRITE_FLD(SCPIP_MON_PRE_DS_03,pMonOutInfo->wPdsOutWidth,SCPIP_MON_OUT_PREDS_MAIN_OUT_H );
	MONOUT_WRITE_FLD(SCPIP_MON_PRE_DS_03,pMonOutInfo->wPdsOutHeight,SCPIP_MON_OUT_PREDS_MAIN_OUT_V );


	MONOUT_WRITE_FLD(SCPIP_MON_PRE_DS_00,u2pre_dscale_h_1,SCPIP_MON_OUT_PRE_DSCALE_H_1 );
	MONOUT_WRITE_FLD(SCPIP_MON_PRE_DS_00,u2pre_dscale_v_1,SCPIP_MON_OUT_PRE_DSCALER_V_1 );

	MONOUT_WRITE_FLD(SCPIP_MON_PRE_DS_01,u2pre_dscale_h_init_1,SCPIP_MON_OUT_PRE_DSCALE_H_INIT_1 );
	MONOUT_WRITE_FLD(SCPIP_MON_PRE_DS_01,u2pre_dscale_v_init_1,SCPIP_MON_OUT_PRE_DSCALE_V_INIT_1 );
}

//=============================================================
//
//
//=============================================================
void vMonOutSetDramWidthHeight(UINT32 u4DramWidth, UINT32 u4DramHeight)
{
    if(u4DramWidth != 0)
    {
	    MONOUT_WRITE_FLD(SCPIP_MON_CTRL_00, u4DramWidth, SCPIP_MON_OUT_SC_DRAM_W_WIDTH_1);
    }
    
    if(u4DramHeight != 0)
    {
	    MONOUT_WRITE_FLD(SCPIP_MON_CTRL_00, u4DramHeight, SCPIP_MON_OUT_SC_DRAM_W_HEIGHT_1);
    }
}
void vMonOutSetDramHVPitch(UINT32 u4DramWidth, UINT32 u4DramHeight)
{
    if(u4DramWidth != 0)
    {
	    MONOUT_WRITE_FLD(SCPIP_MON_CTRL_01, u4DramWidth, SCPIP_MON_OUT_SC_WR_DRAM_H_PITCH_1);
    }
    
    if(u4DramHeight != 0)
    {
	    MONOUT_WRITE_FLD(SCPIP_MON_CTRL_01, u4DramHeight, SCPIP_MON_OUT_SC_WR_DRAM_V_PITCH_1);
    }
}
//=============================================================
//
//
//=============================================================

void vMonOutSetMuxSelIn(E_MON_OUT_IN_SELECT MonOutMuxIn)
{
	MonOutPicInfo* pMonOutInfo;
	pMonOutInfo = getMonOutInfo();
	
	pMonOutInfo->u1MonOutInSel = MonOutMuxIn;

	LOG(1,"Mon out input change to %d \n",MonOutMuxIn);
	LOG(1,"0: Mixer, 1:FSC_main,2: FSC_SUB,3:Mib_main 4:Mib_sub,5:pds_main,6:pds_sub,7:Panel_out,8: FSC_sub to TVE\n");
	vVRMSetMonOutEventFlg(VRM_MON_OUT_IN_SEL_CHG);
}

UINT8 vMonOutGetMuxSelIn(void)
{
	MonOutPicInfo* pMonOutInfo;
	pMonOutInfo = getMonOutInfo();
	return 	pMonOutInfo->u1MonOutInSel;
}

void vMonOutSetOSDSelIn(E_MON_OUT_IN_SELECT_2 MonOutOSDIn)
{
	MonOutPicInfo* pMonOutInfo;
	pMonOutInfo = getMonOutInfo();
	
	pMonOutInfo->u1MonOutInSel_2 = MonOutOSDIn;

	LOG(1,"Mon out input OSD is %d \n",MonOutOSDIn);
	LOG(1,"0: All osd, 1:One osd,2: No osd\n");
	vVRMSetMonOutEventFlg(VRM_MON_OUT_IN_SEL_CHG);
}

void vMonOutSetOutput(E_MON_OUT_OUTPUT_SELECT MonOutOutput)
{
	
	MonOutPicInfo* pMonOutInfo;
	pMonOutInfo = getMonOutInfo();
	
	pMonOutInfo->u1Output = MonOutOutput;
	
	LOG(1,"Mon out output change to %d (0:tve  1:venc 2:GFX)\n",MonOutOutput);	
	vVRMSetMonOutEventFlg(VRM_MON_OUT_OUT_SEL_CHG);
}

void vMonOutSetOutputRes(UINT16 width ,UINT16 height)
{
	
	MonOutPicInfo* pMonOutInfo;
	pMonOutInfo = getMonOutInfo();

	pMonOutInfo->wOutputWidth  = width;
	pMonOutInfo->wOutputHeight = height;
	LOG(1,"Mon out resolution change to width %d height %d\n",width,height);
	vVRMSetMonOutEventFlg(VRM_MON_OUT_OUTPUT_RES_CHG);
}

void vMonOutSetDotByDotOutput(BOOL bflag)
{
    if(fgMonDotByDotOutput != bflag)
    {
      fgMonDotByDotOutput = bflag;
      vVRMSetMonOutEventFlg(VRM_MON_OUT_OUTPUT_RES_CHG);
    }
}

BOOL vMonOutGetDotByDotOutput(void)
{
   return fgMonDotByDotOutput; 
}

void vMonOutGetOutputRes(UINT16* width ,UINT16* height)
{
	
	MonOutPicInfo* pMonOutInfo;
	pMonOutInfo = getMonOutInfo();

	*width = pMonOutInfo->wOutputWidth;
	*height = pMonOutInfo->wOutputHeight;
}


void vMonOutSetRegen(VDP_DISP_REGION_T rRegion)
{
	
	MonOutPicInfo* pMonOutInfo;
	pMonOutInfo = getMonOutInfo();
	
	pMonOutInfo->rRegenScan = rRegion;
	vVRMSetMonOutEventFlg(VRM_MON_OUT_REGEN_CHG);
}

void vMonOutSetTveFormat(UINT8 TimeGenSel)
{
	
	MonOutPicInfo* pMonOutInfo;
	pMonOutInfo = getMonOutInfo();
	
	pMonOutInfo->u1TimeGenSel = TimeGenSel;
	if(pMonOutInfo->u1Output == E_MON_OUT_TVE)
	{		
		LOG(1,"set tve to %s \n", pMonOutInfo->u1TimeGenSel ? "PAL":"NTSC");
		vVRMSetMonOutEventFlg(VRM_MON_OUT_TIME_GEN_CHG);
	}
}

void vMonOutSetEnable(UINT8 Enable)
{
	if(Enable)
	{
	#ifdef CC_SUPPORT_VENC
		if((u4DrvVrmGetAppFlag() & VRM_APP_VIDEO_ENC) && (u1DrvScpipH264GetSource() == VRM_PATH_MON))
		{
			vMonOutSetOutput(E_MON_OUT_VENC);
		}
	#endif	
		if((u4DrvVrmGetAppFlag() & VRM_APP_GRAPHIC_DUMP) && (u1Scpip_GFX_GetSource() == VRM_PATH_MON))
		{
			vMonOutSetOutput(E_MON_OUT_GFX);
		}

		if((u4DrvVrmGetAppFlag() & VRM_APP_SCART_OUT) )//&& (u1DrvScpipH264GetSource() == VRM_PATH_MON))
		{
			vMonOutSetOutput(E_MON_OUT_TVE);
		}
		uMonOutEnable = SV_TRUE;
	}
	else //defalut
	{
		vMonOutSetOutput(E_MON_OUT_GFX);
		uMonOutEnable = SV_FALSE;
	}
}

UINT8 vMonOutGetEnable(void)
{
	if((vMonOutGetDramReadEnable() == SV_TRUE) &&
		(vMonOutGetDramWriteEnable() == SV_TRUE))
	{
		return SV_TRUE;
	}

	return SV_FALSE;
}

void vMonOutGetVideoBlackBar(UINT32 *top,UINT32 *bottom,UINT32 *left,UINT32 *right)
{
	UINT32 u4SRCWidth;
	UINT32 u4SRCHeight;
	MonOutPicInfo* pMonOutInfo;
	pMonOutInfo = getMonOutInfo();
	*top = 0;
	*bottom = 0;
	*left = 0;
	*right = 0;
	if(pMonOutInfo->u1Output != E_MON_OUT_TVE)
	{
		return;
	}
	u4SRCWidth = (UINT32)MONOUT_READ_FLD(SCPIP_MON_TVE_0C, SCPIP_MON_OUT_SC_TVE_WIDTH);
	u4SRCHeight = (UINT32)MONOUT_READ_FLD(SCPIP_MON_TVE_0C, SCPIP_MON_OUT_SC_TVE_HEIGHT);
	if(u4SRCWidth < 720)
	{
		*left = (720 -u4SRCWidth)/2;
		*right = *left;
	}
	if(pMonOutInfo->u1TimeGenSel == TVE_FMT_576I)
	{
		if(u4SRCHeight < 576)
		{
			*top = (576 -u4SRCHeight)/2;
			*bottom = *top;
		}
	}
	else
	{
		if(u4SRCHeight < 480)
		{
			*top = (480 -u4SRCHeight)/2;
			*bottom = *top;
		}
	}
}
UINT8 u1IsMonOutVenc(void)
{
#ifdef CC_SUPPORT_VENC
	if((u4DrvVrmGetAppFlag() & VRM_APP_VIDEO_ENC) && (u1DrvScpipH264GetSource() == VRM_PATH_MON))
	{
		return TRUE;
	}
#endif
	return FALSE;
}

void vMonOutDramReadWriteCtrl(void)
{	
	UINT8 bPath;
	UINT8 bOnOff = SV_ON;
	MonOutPicInfo* pMonOutInfo;
	pMonOutInfo = getMonOutInfo();

	if((pMonOutInfo->u1MonOutInSel == E_MON_OUT_SUB_FSC_OUT) ||
		(pMonOutInfo->u1MonOutInSel == E_MON_OUT_SUB_MIB_OUT)||
		(pMonOutInfo->u1MonOutInSel == E_MON_OUT_SUB_PREDOWN))
	{		
		bPath = VDP_2;
	}
	else
	{
		bPath = VDP_1;
	}
	
	if((bDrvVideoSignalStatus(bPath)!=(UINT8)SV_VDO_STABLE) &&
		(pMonOutInfo->u1MonOutInSel != E_MON_OUT_PANEL))
	{
		bOnOff = SV_OFF;
	}
	
	if(pMonOutInfo->u1Output == E_MON_OUT_GFX)
	{
		bOnOff = SV_OFF;
	}
	
	if(uMonOutEnable != SV_TRUE)
	{
		bOnOff = SV_OFF;
	}

	if(uDraminit != SV_TRUE)
	{
		bOnOff = SV_OFF;
	}
	vMonOutSetDramReadEnable(bOnOff);
	vMonOutSetDramWriteEnable(bOnOff);
}

/**
 * @brief monitor out normal procedure
 */
void vMonOutNormProc(void) 
{
	MonOutPicInfo* pMonOutInfo;
	pMonOutInfo = getMonOutInfo();
   
	if ( pMonOutInfo->u4MonOutModeChg  )
	{
		vMonOutUpdateMuxSelIn(); 
		vMonOutUpdateRegen(); 
		vMonOutTTTConfig();  // 3D to 2D
		vMonOutUpdatePreDownParameter(); 
		if(pMonOutInfo->u1Output == E_MON_OUT_TVE)
		{
			vMonOutUpdateTveTg();
			TVE_ScalerSetSyncTime1();
		}
		vMonOutUpdateDramParameter();

#ifdef CC_FLIP_MIRROR_SUPPORT 
		_vMonOutSetMirrorFlipReg();
#endif   
		pMonOutInfo->u4MonOutModeChg =0;
	}
	vMonOutDramReadWriteCtrl();
}


//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void vDrvMonOutInit(void)
{
	vRegenSetRegenEnableReg(0,REGEN_MODULE_MONOUT_REGEN,SV_ON);  // ??
	vRegenSetClipEnableReg(0,REGEN_MODULE_MONOUT_REGEN,SV_ON);   // ??

	vRegenSetResolutionMode( 0, REGEN_MODULE_MONOUT_REGEN,SV_BOTH ); //mt5399 , regen can be trigger by previous module and regen module
	vRegenSetResolutionInTriSel( 0 , REGEN_MODULE_MONOUT_REGEN, SV_ON );
	vRegenSetResolutionBypass(0,REGEN_MODULE_MONOUT_REGEN, SV_OFF);  //bypass off
    vScpipTveSrcSel( 1 );  //default set sub_FSC to TVE
}




