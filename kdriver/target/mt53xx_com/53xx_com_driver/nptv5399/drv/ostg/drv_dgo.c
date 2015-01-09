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
 * $Author: 
 * $Date: 
 * $RCSfile: 
 * $Revision: 
 *
 *---------------------------------------------------------------------------*/
#ifdef CC_UP8032_ATV
#include "general_mt82.h"
#endif
#ifdef CC_COPLAT_MT82
#include "util.h"
#endif
#include "general.h"
#include "x_timer.h"
#include "x_ckgen.h"
#include "x_gpio.h"
#include "x_pinmux.h"
#include "nptv_debug.h"
#include "hw_dgo.h"
#include "hw_ospe.h"
#include "hw_ckgen.h"
#include "hw_pdwnc.h"
#include "drv_display.h"
#include "drv_scaler.h"
#include "drv_scaler_psc.h"
#ifndef CC_COPLAT_MT82
#include "drvcust_if.h"
#include "pmx_drvif.h"
#include "osd_drvif.h"
#include "vdp_if.h"
#endif


//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------
void vDrvDGOClkSwitch(UINT8 u1OnOff)
{
	vIO32WriteFldAlign(CKGEN_TCLK_TEST, u1OnOff, FLD_OCLK_INV);  // oclk invert
	//vIO32WriteFldAlign(CKGEN_TCLK_TEST, 0x1, FLD_OCLK_PD);   // power down oclk
	//vIO32WriteFldAlign(CKGEN_TCLK_TEST, 0x0, FLD_OCLK_TSTSEL);  // oclk test clk select
	vIO32WriteFldAlign(CKGEN_TCLK_TEST, 0x1, FLD_OCLK_SEL);  // oclk source select: lvds_dpix_ck
	vIO32WriteFldAlign(DGO_ANAIF_CTRL0, u1OnOff, DGO_CLK_EN); 
	vIO32Write4B(MIX_CTRL3,0x20160000);  // [29] h_max_2fs; [20]?; [18]?;[17]?
	vIO32WriteFldAlign(MULTI_CTRL, u1OnOff, DGO_CLK_ENABLE);
}

void vDrvDGOInit(UINT8 u1DispMode)
{
	UINT16 u2Htotal, u2Vtotal, u2HBgn, u2HEnd, u2VBgn, u2VEnd;
	BOOL fgOstg422=FALSE;
	BOOL fgDgo444=FALSE;
	BOOL fgDgoHDOn=FALSE;
	UINT8 u1DgoYuvSel = 1;
	// pinmux setting
#if defined(CC_MT5399)
   BSP_PinSet(PIN_TCON0, PINMUX_FUNCTION6);  // dgo_de, pmux1[31:29](0xf000d604)
   BSP_PinSet(PIN_TCON1, PINMUX_FUNCTION6);  // dgo_hsync, pmux2[2:0](0xf000d608)
   BSP_PinSet(PIN_TCON2, PINMUX_FUNCTION6);  // dgo_vsync, pmux2[5:3]
   BSP_PinSet(PIN_TCON3, PINMUX_FUNCTION6);  // dgo_clk, pmux2[8:6]
   BSP_PinSet(PIN_TCON4, PINMUX_FUNCTION6);  // dgo_y7, pmux2[11:9]
   BSP_PinSet(PIN_TCON5, PINMUX_FUNCTION6);  // dgo_y6, pmux2[14:12]
   BSP_PinSet(PIN_TCON6, PINMUX_FUNCTION6);  // dgo_y5, pmux2[17:15]
   BSP_PinSet(PIN_TCON7, PINMUX_FUNCTION6);  // dgo_y4, pmux2[20:18]
   BSP_PinSet(PIN_TCON8, PINMUX_FUNCTION6);  // dgo_y3, pmux7[10:8](0xf000d628)
   BSP_PinSet(PIN_TCON9, PINMUX_FUNCTION6);  // dgo_y2, pmux7[13:11]
   BSP_PinSet(PIN_TCON10, PINMUX_FUNCTION6);  // dgo_y1, pmux7[16:14]
   BSP_PinSet(PIN_TCON11, PINMUX_FUNCTION6);  // dgo_y0, pmux7[19:17]
   BSP_PinSet(PIN_TCON12, PINMUX_FUNCTION6);  // dgo_c7, pmux7[22:20]
   BSP_PinSet(PIN_GPIO49, PINMUX_FUNCTION6);  // dgo_c6, pmux3[26:24](0xf000d60c)
#if 0  // need to review later
   BSP_PinSet(PIN_GPIO50, PINMUX_FUNCTION6);  // dgo_c5, pmux3[30:28]
   BSP_PinSet(PIN_GPIO51, PINMUX_FUNCTION6);  // dgo_c4, pmux4[2:0](0xf000d610)
   BSP_PinSet(PIN_GPIO52, PINMUX_FUNCTION6);  // dgo_c3, pmux4[6:4]
   BSP_PinSet(PIN_GPIO53, PINMUX_FUNCTION6);  // dgo_c2, pmux4[10:8]
   BSP_PinSet(PIN_GPIO54, PINMUX_FUNCTION6);  // dgo_c1,  pmux5[25:23](0xf000d614)
   BSP_PinSet(PIN_GPIO55, PINMUX_FUNCTION6);  // dgo_c0,  pmux5[28:26]
#else
	vIO32WriteFldAlign(PDWNC_PINMUX2,0x0,FLD_PAD_GPIO50_PDMUX);
	vIO32WriteFldAlign(PDWNC_PINMUX2,0x0,FLD_PAD_GPIO51_PDMUX);
	vIO32WriteFldAlign(PDWNC_PINMUX2,0x0,FLD_PAD_GPIO52_PDMUX);
	vIO32WriteFldAlign(PDWNC_PINMUX2,0x0,FLD_PAD_GPIO53_PDMUX);
	vIO32WriteFldAlign(PDWNC_PINMUX2,0x0,FLD_PAD_GPIO54_PDMUX);
	vIO32WriteFldAlign(PDWNC_PINMUX2,0x0,FLD_PAD_GPIO55_PDMUX);
	vIO32WriteFldAlign(CKGEN_PMUX3,0x6,FLD_PAD_GPIO50);  // PAD_GPIO50-->Function6
	vIO32WriteFldAlign(CKGEN_PMUX4,0x6,FLD_PAD_GPIO51);  // PAD_GPIO51-->Function6
	vIO32WriteFldAlign(CKGEN_PMUX4,0x6,FLD_PAD_GPIO52);  // PAD_GPIO52-->Function6
	vIO32WriteFldAlign(CKGEN_PMUX4,0x6,FLD_PAD_GPIO53);  // PAD_GPIO53-->Function6
	vIO32WriteFldAlign(CKGEN_PMUX3,0x6,FLD_PAD_GPIO54);  // PAD_GPIO54-->Function6
	vIO32WriteFldAlign(CKGEN_PMUX4,0x6,FLD_PAD_GPIO55);  // PAD_GPIO55-->Function6
#endif
#endif

	Printf("[DGO]Init: %d (0:480p, 1:576p, 2:720p, 3:1080p)\n", u1DispMode);
	
#ifdef SUPPORT_DGO
	SetDGOTiming(u1DispMode);
#endif
	
	// DGO clk switch
	vDrvDGOClkSwitch(SV_ON);
	if ((u1DispMode==E_DGO_480P) || (u1DispMode==E_DGO_576P))
	{
		vIO32WriteFldAlign(DGO_ANAIF_CTRL0, 0x0, DGO_CLK_INV_EN);
	}
	else
	{
		vIO32WriteFldAlign(DGO_ANAIF_CTRL0, 0x1, DGO_CLK_INV_EN);
	}

	vIO32WriteFldAlign(OSTG_01,0,REG_RGB2YUV_BYPASS);  // OSTG YCBCR output
	vIO32WriteFldAlign(OSTG_01,fgOstg422,REG_YUV2YC_EN);  // OSTG 422 output
	
	switch(u1DispMode)
	{
		case E_DGO_576P:
			u2Htotal = 864;
			u2Vtotal = 625;
			u2HBgn = 132;  // 0x84
			u2HEnd = 851;  // 0x353
			u2VBgn = 7;   // 0x7
			u2VEnd = 582;  // 0x246
			u1DgoYuvSel = 0;
			break;
		case E_DGO_720P:
			u2Htotal = 1650;
			u2Vtotal = 750;
			u2HBgn = 128;  // 0x80
			u2HEnd = 1407;  // 0x57f
			u2VBgn = 24;   // 0x18
			u2VEnd = 743;  // 0x2e7
			fgDgoHDOn = TRUE;
			break;
		case E_DGO_1080P:
			u2Htotal = 2200;
			u2Vtotal = 1125;
			u2HBgn = 128;  // 0x80
			u2HEnd = 2047;  // 0x7ff
			u2VBgn = 21;   // 0x15
			u2VEnd = 1100;  // 0x44c
			//fgOstg422 = TRUE;
			//fgDgo444 = TRUE;
			u1DgoYuvSel = 0;
			fgDgoHDOn = TRUE;
			break;
		case E_DGO_480P:
		default:
			u2Htotal = 858;
			u2Vtotal = 525;
			u2HBgn = 138;  // 0x8a
			u2HEnd = 857;  // 0x359
			u2VBgn = 16;   // 0x10
			u2VEnd = 495;  // 0x1ef
			u1DgoYuvSel = 0;
			break;
	}
	
	vIO32WriteFldMulti(FMT_HV_TOTAL, P_Fld(0x1, ADJ_T) |P_Fld(u2Htotal, H_TOTAL) |P_Fld(u2Vtotal,V_TOTAL));
    vIO32WriteFldMulti(DGO_DE_H, P_Fld(u2HBgn, CCIR_HBGN) |P_Fld(u2HEnd,CCIR_HEND));  
    vIO32WriteFldMulti(DGO_DE_VO, P_Fld(u2VBgn, CCIR_VOBGN) |P_Fld(u2VEnd,CCIR_VOEND)); 
    vIO32WriteFldMulti(BG_COLOR, P_Fld(0x80, BGCR)|P_Fld(0x80, BGCB)|P_Fld(0xff,BGY));
    vIO32WriteFldMulti(DGO_SYNC_DELAY, P_Fld(0x3, VSYN_DELAY) |P_Fld(0x5,HSYN_DELAY));
    vIO32WriteFldAlign(DGO_MODE, 0x0, DGO_656_EN); 
    vIO32WriteFldAlign(DGO_MODE, 0x1, ADJ_SYN_EN);
    vIO32WriteFldAlign(DGO_CTRL, u1DgoYuvSel, DGO_YUV_SEL_ADJ);
    vIO32WriteFldAlign(DGO_CTRL, fgDgo444, DGO_444);
    vIO32WriteFldAlign(FMT_MODE, fgDgoHDOn, HD_ON);
	
    vIO32WriteFldAlign(FMT_CTRL, 0x1, HPOR);  // sync polarity
    vIO32WriteFldAlign(FMT_CTRL, 0x1, VPOR);
	vIO32WriteFldAlign(FMT_CTRL, 0x1, FMT_ON);
	vIO32Write4B(DGO_TST_PAT_CTRL,0x0);
	vIO32Write4B(SMPTE,0x0);
	vIO32WriteFldAlign(SMPTE,0x1,SMPTE_TOTAL_USE_REG);
	vIO32WriteFldAlign(DGO_CTRL2, 0x2, DGO_C2_CHANNEL_SEL);
	vIO32WriteFldAlign(DGO_CTRL2, 0x1, DGO_C_CHANNEL_SEL);
	vIO32WriteFldAlign(DGO_CTRL2, 0x0, DGO_Y_CHANNEL_SEL);
	vIO32Write4B(MIX_CTRL3,0x20160000);  // [29] h_max_2fs; [20]?; [18]?;[17]?
	//vIO32WriteFldAlign(OSTG_01, 0x1, R_BYPASS_OD);  // bypass OD, SCPOS->TCON(LVDS)
	Printf("[DGO][FSC]output timing: Htotal(%d), Vtotal(%d), Width(%d), Height(%d)\n", u4PscGetPanelHTotal(), u4PscGetPanelVTotal(), u4PscGetPanelWidth(), u4PscGetPanelHeight());
	Printf("[DGO][PSC]output timing: Htotal(%d), Vtotal(%d), Width(%d), Height(%d)\n", u4ScpipGetPanelHtotal(), u4ScpipGetPanelVtotal(), u4ScpipGetPanelWidth(), u4ScpipGetPanelHeight());
	Printf("[DGO]Init Done!!\n", u1DispMode);
}

void vDrvDGO3DSetting(UINT8 u1DispMode)
{
	UINT16 u2Htotal, u2Vtotal, u2VBgn, u2VEnd;

	u2Htotal = 0;
	u2Vtotal = 0;
	u2VBgn = 0;
	u2VEnd = 0;

	if ((u1DispMode<E_DGO_3D_1080P_24_FP)||(u1DispMode>E_DGO_3D_1080P_30_FP))
		u1DispMode = E_DGO_3D_1080P_24_FP;

	Printf("[DGO][FSC]output timing: Htotal(%d), Vtotal(%d), Width(%d), Height(%d)\n", u4PscGetPanelHTotal(), u4PscGetPanelVTotal(), u4PscGetPanelWidth(), u4PscGetPanelHeight());
	Printf("[DGO][PSC]output timing: Htotal(%d), Vtotal(%d), Width(%d), Height(%d)\n", u4ScpipGetPanelHtotal(), u4ScpipGetPanelVtotal(), u4ScpipGetPanelWidth(), u4ScpipGetPanelHeight());

	switch(u1DispMode)
	{
		case E_DGO_3D_1080P_24_FP:
			u2Htotal = 2750;
			u2Vtotal = 2250;
			u2VBgn = 21;   // 0x15
			u2VEnd = 2225;  // 0x8B1
			break;
		case E_DGO_3D_1080P_25_FP:
			u2Htotal = 2640;
			u2Vtotal = 2250;
			u2VBgn = 21;   // 0x15
			u2VEnd = 2270;  // 0x8DE
			break;
		case E_DGO_3D_1080P_30_FP:
			u2Htotal = 2200;
			u2Vtotal = 2250;
			u2VBgn = 21;   // 0x15
			u2VEnd = 2270;  // 0x8DE
			break;
		default:
			break;
	}

	vIO32WriteFldMulti(FMT_HV_TOTAL, P_Fld(0x1, ADJ_T) |P_Fld(u2Htotal, H_TOTAL) |P_Fld(u2Vtotal,V_TOTAL));
    //vIO32WriteFldMulti(DGO_DE_H, P_Fld(u2HBgn, CCIR_HBGN) |P_Fld(u2HEnd,CCIR_HEND));  
    vIO32WriteFldMulti(DGO_DE_VO, P_Fld(u2VBgn, CCIR_VOBGN) |P_Fld(u2VEnd,CCIR_VOEND)); 
	vIO32WriteFldAlign(SMPTE, 0x1, SMPTE_TOTAL_USE_REG);
	vIO32WriteFldAlign(DGO_CTRL2, 0x1, DGO_3D_EN);
}

void vDrvDGOPatternGen(UINT8 u1OnOff, UINT8 u1DispMode)
{
	UINT16 u2Htotal, u2Vtotal;

	if (!u1OnOff)
	{
		vIO32WriteFldAlign(FMT_CTRL, 0x1, HPOR);  // sync polarity
	    vIO32WriteFldAlign(FMT_CTRL, 0x1, VPOR);
    }
    else
    {
		vIO32WriteFldAlign(FMT_CTRL, 0x0, HPOR);  // sync polarity
	    vIO32WriteFldAlign(FMT_CTRL, 0x0, VPOR);
    }

	switch(u1DispMode)
	{
		case E_DGO_576P:
			u2Htotal = 864;
			u2Vtotal = 625;
			break;
		case E_DGO_720P:
			u2Htotal = 1650;
			u2Vtotal = 750;
			break;
		case E_DGO_1080P:
			u2Htotal = 2200;
			u2Vtotal = 1125;
			break;
		case E_DGO_480P:
		default:
			u2Htotal = 858;
			u2Vtotal = 525;
			break;
	}
	//vIO32WriteFldAlign(DGO_MODE,0x1,ADJ_SYN_EN);
	//vIO32WriteFldAlign(FMT_CTRL,0x1,FMT_ON);
	vIO32WriteFldMulti(FMT_HV_TOTAL, P_Fld(0x1, ADJ_T) |P_Fld(u2Htotal, H_TOTAL) |P_Fld(u2Vtotal,V_TOTAL));  // total
	vIO32WriteFldAlign(SMPTE,0x1,SMPTE_TOTAL_USE_REG);
	vIO32WriteFldAlign(DGO_TST_PAT_CTRL,u1OnOff,RG_TST_PAT_CLK_EN);
	vIO32WriteFldAlign(DGO_TST_PAT_CTRL,u1OnOff,RG_TST_PAT_EN);
	vIO32WriteFldAlign(DGO_TST_PAT_CTRL,0x1,RG_PTGEN_TYPE);  // pure color
	vIO32WriteFldAlign(BG_COLOR,0x80,BGCR);  // white
	vIO32WriteFldAlign(BG_COLOR,0x80,BGCB);
	vIO32WriteFldAlign(BG_COLOR,0xff,BGY);
	
	vIO32WriteFldAlign(MULTI_CTRL,u1OnOff,RG_TST_DATA_PAT_EN);
}

void vDrvDGOOff(void)
{
	vDrvDGOClkSwitch(SV_OFF);
	
	vIO32WriteFldAlign(OSTG_01,1,REG_RGB2YUV_BYPASS);  // OSTG RGB output
	vIO32WriteFldAlign(OSTG_01,0,REG_YUV2YC_EN);  // OSTG 422 output

#ifdef SUPPORT_DGO
	SetDGOTiming(E_DGO_DEFAULT);
#endif
}

LINT_RESTORE
