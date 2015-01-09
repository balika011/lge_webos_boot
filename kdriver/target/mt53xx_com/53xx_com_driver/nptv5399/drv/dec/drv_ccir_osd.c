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

/**
 * @file DVI/HDMI/CCIR decoder driver
 *
 * @author yj_chiang@mtk.com.tw
 * @entry points
 * @note init -> bApiVideoMainSubSrc -> _bDviExtSrc -> vDviConnect
 *	 -> vDviChkModeChange -> vDviInitial -> vDviModeDetect
 * @note vDviModeDetect -> DVI_SEARCH_STATE -> bDviStdTimingSearch
 *	 !MODE_NOSIGNAL -> fgIsVgaTiming -> DVI_WAIT_AUTO_STATE (auto position) -> DVI_MODE_CHG_DONE_STATE
 *	 MODE_NOSIGNAL -> DVI_MODE_CHG_DONE_STATE
 * @note HLEN, VLEN -> HCLK, VCLK -> timing table
 */
#include "general.h"
	 
#include "x_printf.h"
	 // Kernel
#include "util.h"
	 // vdo kernel
#include "hw_vdoin.h"
#include "hw_dvi.h"
#include "hw_sys.h"
#include "hw_ckgen.h"
#include "drv_vdoclk.h"
	 // sync
#include "drv_async.h"
#include "drv_dsync.h"
	 // vga
#include "drv_vga.h"
#include "drv_auto.h"
#include "vga_auto.h"
#include "vga_table.h"
	 // source
#include "source_select.h"
	 // autocolor
#include "drv_autocolor.h"
	 // hdtv
#include "drv_hdtv.h"
	 // dvi
#include "drv_dvi.h"
	 // scpos
#include "drv_scaler.h"
	 // ycproc
#include "drv_ycproc.h"
	 // display
#include "drv_display.h"
	 // video API
#include "video_def.h"
	 // hdmi
#include "drv_hdmi.h"
#include "hw_hdmi.h"
	 
#include "nptv_debug.h"
#include <x_lint.h>
#include "drvcust_if.h"
	 
#include "hw_dsync.h"
	 //#include "drv_scpos.h"
#include "drv_video.h"
#include "api_notify.h"
#include "vdo_misc.h"
#include "srm_drvif.h"
#include "drv_di.h"
#include "mute_if.h"
#include "drv_tdtv.h"
#include "drv_scaler_psc.h"	
#include "panel.h"


#define CCIR_OSD_ON_OFF Fld(1,22,AC_MSKB2)//[22:22]

static UINT32 width=0;
static UINT32 height=0;
extern UINT8 _bCcir2status;
//EXTERN INT32 OSD_SC_GetScalerInfo(UINT32 u4Scaler, UINT32* pu4Enable,
//                                  UINT32* pu4SrcW, UINT32* pu4SrcH,
//                                  UINT32* pu4DstW, UINT32* pu4DstH,
//                                  UINT32* pu4Is16Bpp);

void vCCIROSDSetAvWidth(UINT16 wWidth)
{
	vIO32WriteFldAlign(CCIR_03_NEW,wWidth,CCH_AV_WIDTH_PIP);
}

void vCCIROSDSetVsWidth(UINT16 wStart)
{
    vIO32WriteFldAlign(CCIR_03_NEW,wStart,CCH_VDE_WIDTH_PIP);
}

static UINT16 vs_start_tmp;
static UINT16 vs_start;
static UINT16 v_total;

void vCCIROSDSETVSTART(void)
{		
	//UINT32 tmp = 0;

	if((width == 0)&&(height == 0))
	{
		//OSD_SC_GetScalerInfo(1,&tmp,&tmp,&tmp,&width,&height,&tmp);
		if (fgIsMJCToOSTG())
		{
			width =  u4PscGetPanelWidth();
			height = u4PscGetPanelHeight();
		}
		else
		{
			width = u4ScpipGetPanelWidth();
			height = u4ScpipGetPanelHeight();
		}

		vIO32Write4B(CCIR_01_NEW,0xb000e000);
	}
	
	if(vDrvGetLCDFreq() == 50)
	{
		v_total = PANEL_GetVTotal50Hz();
		vs_start = v_total - height - 3;		
	}
	else
	{
		v_total = PANEL_GetVTotal60Hz();
		vs_start = v_total - height - 3;
	}

	if(vs_start > (vs_start_tmp+5) || vs_start < (vs_start_tmp-5) )
	{
		//LOG(1, "vs_start %d \n", vs_start);
		//LOG(1, "vs_start_tmp %d \n", vs_start_tmp);
		vs_start_tmp = vs_start;		
		vIO32WriteFldAlign(CCIR_01_NEW,vs_start,CCH_VS_START_PIP);
	}
	
	
}

//1:turn on OSD   0:turn off OSD
void vCCIROSDOnOff(UINT8 fgIsOn)
{
	if(fgIsOn == SV_ON)
	{
		//turn on OSD
		vIO32WriteFldAlign(0xf0027800,0,CCIR_OSD_ON_OFF);
	}
	else
	{
		//turn off OSD
		vIO32WriteFldAlign(0xf0027800,1,CCIR_OSD_ON_OFF);
	}
}

void vCCIROSDInitial(void)
{

}

/**
 * @brief 	OSD Call-back function for source select connect or disconnect.
 * @param	bchannel - SV_VP_MAIN / SV_VP_PIP
 * @param	fgIsOn - SV_ON/SV_OFF
 * @retval	None
 */
void vCCIROSDConnect(UINT8 bchannel, UINT8 fgIsOn)
{
#if defined(CC_MT5880)

	if(fgIsOn == SV_ON)
	{
		_bCcir2status = 1;

		vCCIROSDSETVSTART();
		printf("width=%ld\n",width);
		printf("hight=%ld\n",height);
		
		vIO32Write4B(CCIR_00_NEW,0xf2866380);

		vCCIROSDSetAvWidth(width);
		vCCIROSDSetVsWidth(height);

		//pip dta from OSTG
		vIO32WriteFldAlign(HDMI_PIP_SEL,0,REG_VIDEO_OUT_PIP_DATA_SEL);
		//pip clk from OSTG
		vIO32WriteFldAlign(HDMI_PIP_SEL,0,REG_VIDEO_OUT_PIP_MAVIS_SEL2);
		vIO32WriteFldAlign(HDMI_PIP_SEL,0,REG_VIDEO_OUT_PIP_MAVIS_SEL1);
		vIO32WriteFldAlign(HDMI_PIP_SEL,0,REG_VIDEO_OUT_PIP_MAVIS_SEL);
		
		//pip from CCIR1
		vIO32WriteFldAlign(OMUX_00, 0x5, OMUX_PIP_SEL);
		
        vSetPipFlg(PIP_FLG_MODE_CHG);		
        vSetPipFlg(PIP_FLG_MODE_DET_DONE);
		
		//_bHDMIColorSpace = 1; //RGB
	}
	else
	{
		_bCcir2status= 0;
		width = 0;
		height =0;
		vs_start = 0;
		v_total  = 0;
		vs_start_tmp = 0;
		//pip dta from HDMI
		vIO32WriteFldAlign(HDMI_PIP_SEL,1,REG_VIDEO_OUT_PIP_DATA_SEL);
		//pip clk from HDMI
		vIO32WriteFldAlign(HDMI_PIP_SEL,1,REG_VIDEO_OUT_PIP_MAVIS_SEL2);
	}
#elif defined(CC_MT5882)

	if(fgIsOn == SV_ON)
	{
		_bCcir2status = 1;

		vCCIROSDSETVSTART();
		printf("width=%ld\n",width);
		printf("hight=%ld\n",height);
		
		vIO32Write4B(CCIR_00_NEW,0xf2866380);

		vCCIROSDSetAvWidth(width/2);
		vCCIROSDSetVsWidth(height);

		vIO32WriteFldAlign(CCIR_04_NEW,1,CCH_VIDEO_SEL_PIP);
		
		//pip from CCIR1
		vIO32WriteFldAlign(OMUX_00, 0x5, OMUX_PIP_SEL);
		
        vSetPipFlg(PIP_FLG_MODE_CHG);		
        vSetPipFlg(PIP_FLG_MODE_DET_DONE);
		
		//_bHDMIColorSpace = 1; //RGB
	}
	else
	{
		_bCcir2status= 0;
		vIO32WriteFldAlign(CCIR_04_NEW,0,CCH_VIDEO_SEL_PIP);
	}

#endif	
}
/**
 * @brief 	DVI call-back function for getting DVI input signal HTotal.
 * @param	None
 * @retval	HTotal of input signal.
 */
UINT16 wCCIROSDInputHTotal(void)
{
    UINT16 wHTotal = PANEL_GetHTotal60Hz();

    return wHTotal;
}

/**
 * @brief 	DVI call-back function for getting DVI input signal width.
 * @param	None
 * @retval	Height of input signal.
 */
UINT16 wCCIROSDInputVTotal(void)
{
    UINT16 wVTotal = PANEL_GetVTotal60Hz();


    return wVTotal;
}

/**
 * @brief 	DVI call-back function for getting DVI input signal width.
 * @param	None
 * @retval	Hactive of input signal.
 */
UINT16 wCCIROSDInputWidth(void)
{
	return width;

}

/**
 * @brief 	DVI call-back function for getting DVI input signal height.
 * @param	None
 * @retval	Vactive of input signal.
 */
UINT16 wCCIROSDInputHeight(void)
{
    return height;
}

/**
 * @brief Exported to display driver, video mainloop, video "ISR", middleware interface.
 * Call this function when signal is stable. DVI driver use another function to get refresh rate.
 * @param None
 * @return Refresh rate of input signal.
 */
UINT8 bCCIROSDRefreshRate(void)
{
	UINT8 freq;

	freq = vDrvGetLCDFreq();
	return freq;
}

//	0 - progressive, 1 - interlace
UINT8 bCCIROSDInterlace(void)
{
	return 0;
}

/**
 * @brief 	DVI call-back function for vVdoMainState, vVdoPipState polling update video status to UI.
 * @param	None
 * @retval	SV_VDO_UNKNOWN / SV_VDO_NOSIGNAL / SV_VDO_NOSUPPORT / SV_VDO_STABLE
 */
UINT8 bCCIROSDSigStatus(void)
{
	return (UINT8)(SV_VDO_STABLE);
}  

#if 0
void vCCIROSDDebugMsg(void)
{
        printf("sw: DVI width: %d\n", wDviInputWidth());
        printf("sw: DVI height: %d\n", wDviInputHeight());
        printf("sw: DVI refresh rate: %d\n", bDviRefreshRate());
        printf("sw: DVI interlace/progressive: %d\n", bDviInterlace());
        printf("sw: DVI signal status: %x\n", bDviSigStatus());
        // timing index
        printf("sw: _bDviTiming: %d\n", _bDviTiming);
        printf("sw: pixel clock: %d\n", dwDviPIXClock());

}
#endif


