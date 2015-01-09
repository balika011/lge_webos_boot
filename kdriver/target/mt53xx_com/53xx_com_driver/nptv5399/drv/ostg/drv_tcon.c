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
 * $RCSfile: drv_tcon.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/
#ifdef CC_UP8032_ATV
#include "general_mt82.h"
#endif

#ifdef CC_COPLAT_MT82
#include "util.h"
#endif

#include "general.h"

#include "x_pinmux.h"
#include "x_bim.h"
#include "x_assert.h"

#include "hw_ckgen.h"
#include "hw_tcon.h"
#include "hw_lvds.h"
#include "hw_mlvds.h"
#include "hw_tg.h"
#include "hw_epi.h"

#include "drv_tcon.h"
#include "drv_vdoclk.h"
#include "drv_lvds.h"
#include "drv_pwm.h"


#include "sif_if.h"
#include "panel.h"
#include "drv_display.h"

#include "nptv_debug.h"
#include "pe_if.h"

#ifdef CC_SUPPORT_FLASH_TCON
#include "nor_if.h"
#ifdef CC_NAND_ENABLE
#include "nand_if.h"
#endif
#endif

#include "drvcust_if.h"

#ifdef SUPPORT_LOCK_FROM_SRVAD
#include "x_pdwnc.h"
#define SERVOADC_TCON_LOCK 3
#endif

//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------
#define TCON_ADDR_BASE 0xf0000000

static UINT8 _u1GammaReady=0;
static BOOL _fgTconCustSel=FALSE;
static VIDEO_TCON_TABLE_T _u1TconCustSel = EPIV13FHD60_32;
//static BOOL _fgTconCuston=FALSE;
UINT8 bbypass_isr=0;
//UINT8 init_VCC18=0;
extern UINT8 _Lvds7To4FifoReset;

//-----------------------------------------------------------------------------
// Global variables
//-----------------------------------------------------------------------------

extern PANEL_FLASH_DATA_ATTRIBUTE_T rFlashPanelTable;

//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------
// DPM power control
static void vTconDPMCtrl(UINT8 u1Val)
{
  u1TconCustDPMCtrlEN1(u1Val);
  u1TconCustDPMCtrlEN2(u1Val);  	
}

//TCON Register Write
void vTconRegWrite(UINT32 u4addr, UINT32 val)
{
    #ifndef CC_MTK_LOADER
    UINT32 u4tmp;
    #endif

	if ((u4addr == 0xffffffff) || ((u4addr&TCON_ADDR_BASE) != TCON_ADDR_BASE))
	{
		LOG(1,"Write addr Err!!! w 0x%x 0x%x\n", u4addr, val);
	}
    else
    {
	    vIO32Write4B(u4addr, val);
        #ifndef CC_MTK_LOADER
   	    u4tmp = u4IO32Read4B(u4addr);
        LOG(1,"0x%8x, 0x%8x, 0xffffffff,\n", u4addr, u4tmp);
        #endif
    }
}

//TCON Register Read
UINT32 u4TconRegRead(UINT32 u4addr)
{
    UINT32 u4tmp;
	
	if ((u4addr == 0xffffffff) || ((u4addr&TCON_ADDR_BASE) != TCON_ADDR_BASE))
	{
		LOG(1,"Read addr Err!!! w 0x%x\n", u4addr);
		return (0);
	}
    else
    {
   	    u4tmp = u4IO32Read4B(u4addr);
        LOG(1,"0x%8x, 0x%8x, 0xffffffff,\n", u4addr, u4tmp);		
		return (u4tmp);
    }
}

// Panel  Interface Pattern
void vDrvMLVDSPatternGen(UINT8 u1OnOff)
{

	 vIO32WriteFldAlign(VINTF_B6, 0, VINTF_PTGEN_MIRROR);
	 vIO32WriteFldAlign(VINTF_B6, 0, VINTF_PTGEN_TYPE_BD_DIS);	 
	 
	 vIO32WriteFldAlign(VINTF_B12, 0, VINTF_PTGEN_V_RUN);
	 vIO32WriteFldAlign(VINTF_B12, 0, VINTF_PTGEN_H_RUN);			
			
    if (IS_PANEL_L12R12)
    {
     vIO32WriteFldAlign(VINTF_B6, 3, VINTF_PTGEN_TYPE);    
     vIO32WriteFldAlign(VINTF_B12, 3, VINTF_PTGEN_OSD_TYPE); 	 	 
     vIO32WriteFldAlign(VINTF_B6, 0, VINTF_PTGEN_SEQ);       
    }
	else
	{
     vIO32WriteFldAlign(VINTF_B6, 2, VINTF_PTGEN_TYPE);    	
     vIO32WriteFldAlign(VINTF_B12, 2, VINTF_PTGEN_OSD_TYPE); 	 
	 vIO32WriteFldAlign(VINTF_B6, 1, VINTF_PTGEN_SEQ);	
	}
    
    vIO32WriteFldAlign(VINTF_B1, PANEL_GetPanelWidth(), VINTF_PTGEN_H_ACTIVE);
    vIO32WriteFldAlign(VINTF_B1, wDrvGetOutputHTotal(), VINTF_PTGEN_H_TOTAL);    
    vIO32WriteFldAlign(VINTF_B2, PANEL_GetPanelHeight(), VINTF_PTGEN_V_ACTIVE);
    vIO32WriteFldAlign(VINTF_B11, PANEL_GetPanelHeight(), VINTF_PTGEN_V_OSD_ACTIVE);	
    vIO32WriteFldAlign(VINTF_B2, wDrvGetOutputVTotal(), VINTF_PTGEN_V_TOTAL);

    vIO32WriteFldAlign(VINTF_B6, (PANEL_GetPanelHeight()/4), VINTF_PTGEN_COLOR_BAR_TH);

    vIO32WriteFldAlign(VINTF_B4, IO32ReadFldAlign(EH_0C, EH_DE_HS_WIDTH), VINTF_PTGEN_H_WIDTH);
	
    if((PANEL_GetDisplayType()== PANEL_DISPLAY_TYPE_EPI)||(PANEL_GetDisplayType()== PANEL_DISPLAY_TYPE_MLVDS))
    {
     vIO32WriteFldAlign(VINTF_B4, 1, VINTF_PTGEN_V_WIDTH);

	 vIO32WriteFldAlign(PIXEL_REORDER_01, 0, RG_HS_OUT_POL);
	 
	 vIO32WriteFldAlign(PIXEL_REORDER_01, 1, RG_VS_OUT_POL);
	 vIO32WriteFldAlign(PIXEL_REORDER_15, 1, RG_VWIDTH_R);
	 vIO32WriteFldAlign(PIXEL_REORDER_16, 8, RG_VDE_VSTART_R);
	 vIO32WriteFldAlign(MUTE_PATTERN_05, 8, MUTE_V_START);
	 vIO32WriteFldAlign(VINTF_B3, 8, VINTF_PTGEN_V_START);
	 vIO32WriteFldAlign(EH_13,8,EH_PTGEN_V_START);

	if(PANEL_GetDisplayType()== PANEL_DISPLAY_TYPE_EPI) 
		{
		  vIO32WriteFldAlign(TMGR3, 4, TMGR3_RG_TCON_OUT_SEL);
		}
	if(PANEL_GetDisplayType()== PANEL_DISPLAY_TYPE_MLVDS)
		{
		
		vIO32WriteFldAlign(TMGR3, 1, TMGR3_RG_TCON_OUT_SEL);
		}
    }
	else
	{
	 if (IS_SEPARATE_OSD_AND_DATA)
	 {
	 	vIO32WriteFldAlign(VINTF_B11, 6, VINTF_PTGEN_V_OSD_START);
	 }	
	 	vIO32WriteFldAlign(VINTF_B4, IO32ReadFldAlign(EH_0D, EH_DE_VS_WIDTH)+1, VINTF_PTGEN_V_WIDTH);  
	}
    if (u1OnOff)
    {
        vIO32WriteFldAlign(VINTF_B6, 1, VINTF_PTGEN_EN);
        vIO32WriteFldAlign(EH_01, 0, EH_USE_SC_TIMING_EN);		
    }
    else
    {
        vIO32WriteFldAlign(VINTF_B6, 0, VINTF_PTGEN_EN);
        vIO32WriteFldAlign(EH_01, 1, EH_USE_SC_TIMING_EN);				
    }
}


/**
 * @brief u1DrvTconTestPattern

 * TCON test pattern

 * @param  void
 * @retval 0: fail, 1: success
 */

// Panel Interface Pattern
UINT8 u1DrvTconTP(UINT8 val)
{
    UINT8 u1Ret = 0;

	vDrvMLVDSPatternGen(1);

	switch(val)
	{
	    case TCON_TP_OFF:
			vDrvMLVDSPatternGen(0);
			break;
			
		case TCON_TP_R:
			vIO32WriteFldAlign(VINTF_B6, 1, VINTF_PTGEN_TYPE_BD_DIS);	 
			vIO32WriteFldAlign(VINTF_B6, 1, VINTF_PTGEN_TYPE);	 
			vIO32WriteFldAlign(VINTF_B5, 0x3FF, VINTF_PTGEN_R);	 
			vIO32WriteFldAlign(VINTF_B5, 0, VINTF_PTGEN_G);	 
			vIO32WriteFldAlign(VINTF_B5, 0, VINTF_PTGEN_B);	 			
			break;
			
		case TCON_TP_G:
			vIO32WriteFldAlign(VINTF_B6, 1, VINTF_PTGEN_TYPE_BD_DIS);	 			
			vIO32WriteFldAlign(VINTF_B6, 1, VINTF_PTGEN_TYPE);	 
			vIO32WriteFldAlign(VINTF_B5, 0, VINTF_PTGEN_R);	 
			vIO32WriteFldAlign(VINTF_B5, 0x3FF, VINTF_PTGEN_G);	 
			vIO32WriteFldAlign(VINTF_B5, 0, VINTF_PTGEN_B);	 			
			break;
			
		case TCON_TP_B:
			vIO32WriteFldAlign(VINTF_B6, 1, VINTF_PTGEN_TYPE_BD_DIS);	 			
			vIO32WriteFldAlign(VINTF_B6, 1, VINTF_PTGEN_TYPE);	 
			vIO32WriteFldAlign(VINTF_B5, 0, VINTF_PTGEN_R);	 
			vIO32WriteFldAlign(VINTF_B5, 0, VINTF_PTGEN_G);	 
			vIO32WriteFldAlign(VINTF_B5, 0x3FF, VINTF_PTGEN_B);	 			
			break;
			
		case TCON_TP_BLACK:
			vIO32WriteFldAlign(VINTF_B6, 1, VINTF_PTGEN_TYPE_BD_DIS);	 
			vIO32WriteFldAlign(VINTF_B6, 1, VINTF_PTGEN_TYPE);	 
			vIO32WriteFldAlign(VINTF_B5, 0, VINTF_PTGEN_R);	 
			vIO32WriteFldAlign(VINTF_B5, 0, VINTF_PTGEN_G);	 
			vIO32WriteFldAlign(VINTF_B5, 0, VINTF_PTGEN_B);			
			break;
			
		case TCON_TP_WHITE:
			vIO32WriteFldAlign(VINTF_B6, 1, VINTF_PTGEN_TYPE_BD_DIS);	 			
			vIO32WriteFldAlign(VINTF_B6, 1, VINTF_PTGEN_TYPE);	 
			vIO32WriteFldAlign(VINTF_B5, 0x3FF, VINTF_PTGEN_R);	 
			vIO32WriteFldAlign(VINTF_B5, 0x3FF, VINTF_PTGEN_G);	 
			vIO32WriteFldAlign(VINTF_B5, 0x3FF, VINTF_PTGEN_B);
			break;
			
		case TCON_TP_COLORRAMP:
			vIO32WriteFldAlign(VINTF_B6, 2, VINTF_PTGEN_TYPE);	 
			vIO32WriteFldAlign(VINTF_B5, 0, VINTF_PTGEN_R);	 
			vIO32WriteFldAlign(VINTF_B5, 0, VINTF_PTGEN_G);	 
			vIO32WriteFldAlign(VINTF_B5, 0, VINTF_PTGEN_B);
			break;
			
		case TCON_TP_MOVING:
			vIO32WriteFldAlign(VINTF_B6, 1, VINTF_PTGEN_TYPE_BD_DIS);	 			
			vIO32WriteFldAlign(VINTF_B12, 1, VINTF_PTGEN_V_RUN);
			vIO32WriteFldAlign(VINTF_B12, 1, VINTF_PTGEN_H_RUN);			
			vIO32WriteFldAlign(VINTF_B6, 2, VINTF_PTGEN_TYPE);	 
			vIO32WriteFldAlign(VINTF_B5, 0, VINTF_PTGEN_R);	 
			vIO32WriteFldAlign(VINTF_B5, 0, VINTF_PTGEN_G);	 
			vIO32WriteFldAlign(VINTF_B5, 0, VINTF_PTGEN_B);
			break;
			
		case TCON_TP_AUTO:

			break;
			
		default:
			break;

	}

	return(u1Ret);	
}

// Pixel Reorder Pattern
void u1DrvPixelReorderTP(UINT8 val)
{

	vIO32WriteFldAlign(MUTE_PATTERN_01, val, MANUAL_PATTERN_ENABLE);
	vIO32WriteFldAlign(MUTE_PATTERN_07, 0xC, RG_BIST0);
	vIO32WriteFldAlign(MUTE_PATTERN_11, 0x86, RG_H_COLOR_BAR_TH);

}

//Error Handling
#define EH_PANEL_TABLE
#ifdef EH_PANEL_TABLE
#define EH_PANEL_FORMULATE 0
#endif
void vErrorHandleSetByPanel(void)
{
#ifdef EH_PANEL_TABLE 
	  	UINT32 H_LOW_TH,H_HIGH_TH;
	  	UINT32 V_LOW_TH,V_HIGH_TH; 
	  	UINT32 DE_LOW_TH,DE_HIGH_TH; 
	  	UINT32 VDE_LOW_TH,VDE_HIGH_TH;	   
	  	UINT32 EH_CLK_HIGH_TH=0, EH_CLK_LOW_TH=0;
      #if EH_PANEL_FORMULATE  
	  	  UINT32 EH_CLK_TOTAL_RATION,EH_TOTAL; 	    
      #endif
	
      #ifndef NDEBUG
		   Printf("[NPTV][EH] Error handling init \n");
		   Printf("[NPTV][EH] PANEL_GetPanelWidth=0x%x, PANEL_GetPanelHeight=0x%x , wDrvGetOutputHTotal=0x%x , wDrvGetOutputVTotal=0x%x\n",
				  PANEL_GetPanelWidth(),PANEL_GetPanelHeight(),wDrvGetOutputHTotal(),wDrvGetOutputVTotal());		
      #endif     
	
	
      #if EH_PANEL_FORMULATE             
		   H_LOW_TH = PANEL_GetPanelWidth() + 100;
		   H_HIGH_TH = wDrvGetOutputHTotal() * 2;
	
		   if (PANEL_GetPanelHeight()>=1060)
		   {
		   V_LOW_TH = PANEL_GetPanelHeight() + 30;
		   V_HIGH_TH = wDrvGetOutputVTotal() * 2;
		   }
		   else
		   {
		   V_LOW_TH = PANEL_GetPanelHeight() + 20;
		   V_HIGH_TH = wDrvGetOutputVTotal() * 2;
		   }
	
		   DE_HIGH_TH = PANEL_GetPanelWidth()+7;
		   DE_LOW_TH = PANEL_GetPanelWidth()-8;
	
		   VDE_HIGH_TH = PANEL_GetPanelHeight();
		   VDE_LOW_TH = PANEL_GetPanelHeight();
		   
		   if (IS_XTAL_24MHZ())
		   {
				EH_TOTAL = 1024*24000 / (PANEL_GetPixelClk60Hz()/1000);
		   }
		   else if (IS_XTAL_27MHZ())
		   {
				EH_TOTAL = 1024*27000 / (PANEL_GetPixelClk60Hz()/1000);
		   }
		   EH_CLK_TOTAL_RATION = EH_TOTAL / 4; 
	
		   EH_CLK_HIGH_TH = EH_TOTAL + EH_CLK_TOTAL_RATION;
		   EH_CLK_LOW_TH = EH_TOTAL - EH_CLK_TOTAL_RATION;
      #else      
      #ifndef NDEBUG
		   Printf("[NPTV][EH] PANEL_GetHTotalMax=0x%x, PANEL_GetHTotalMin=0x%x, PANEL_GetVTotalMax=0x%x, PANEL_GetVTotalMin=0x%x \n",
				  PANEL_GetHTotalMax(),PANEL_GetHTotalMin(),PANEL_GetVTotalMax(),PANEL_GetVTotalMin()); 	   
		   Printf("[NPTV][EH] PANEL_GetPixelClkMax=%d, PANEL_GetPixelClkMin=%d \n",
				  PANEL_GetPixelClkMax(),PANEL_GetPixelClkMin());					   
      #endif        
		   H_HIGH_TH = PANEL_GetHTotalMax();
		   H_LOW_TH = PANEL_GetHTotalMin();
	
		   V_HIGH_TH = PANEL_GetVTotalMax();
		   V_LOW_TH = PANEL_GetVTotalMin();
	
		   DE_HIGH_TH = PANEL_GetPanelWidth()+7;
		   DE_LOW_TH = PANEL_GetPanelWidth()-8;
	
		   VDE_HIGH_TH = PANEL_GetPanelHeight();
		   VDE_LOW_TH = PANEL_GetPanelHeight(); 	  
	
		   // fix error for mt8283
		   if (IS_XTAL_24MHZ())
		   	{
				EH_CLK_HIGH_TH = (1024*24000/ (PANEL_GetPixelClkMin()/ 1000));		 
		   		EH_CLK_LOW_TH = (1024*24000/ (PANEL_GetPixelClkMax()/ 1000));
		   }
		   else if (IS_XTAL_27MHZ())
		   	{
				EH_CLK_HIGH_TH = (1024*27000/ (PANEL_GetPixelClkMin()/ 1000));		 
		   		EH_CLK_LOW_TH = (1024*27000/ (PANEL_GetPixelClkMax()/ 1000));
		   }
      #endif
		 
       #ifndef NDEBUG
			Printf("[NPTV][EH] PANEL_GetPixelClk60Hz = 0x%x \n",PANEL_GetPixelClk60Hz());
       #endif
				 
		   vIO32WriteFldAlign(EH_02,H_HIGH_TH,EH_FAST_MUTE_H_TOTAL_HIGH_TH);
		   vIO32WriteFldAlign(EH_02,H_LOW_TH,EH_FAST_MUTE_H_TOTAL_LOW_TH);		  
		 
		   vIO32WriteFldAlign(EH_03,V_HIGH_TH,EH_FAST_MUTE_V_TOTAL_HIGH_TH);
		   vIO32WriteFldAlign(EH_03,V_LOW_TH,EH_FAST_MUTE_V_TOTAL_LOW_TH);				  
				 
		   vIO32WriteFldAlign(EH_04,DE_HIGH_TH,EH_FAST_MUTE_DE_TOTAL_HIGH_TH);
		   vIO32WriteFldAlign(EH_04,DE_LOW_TH,EH_FAST_MUTE_DE_TOTAL_LOW_TH);	   
		   
		   vIO32WriteFldAlign(EH_05,VDE_HIGH_TH,EH_FAST_MUTE_VDE_TOTAL_HIGH_TH);		
		   vIO32WriteFldAlign(EH_05,VDE_LOW_TH,EH_FAST_MUTE_VDE_TOTAL_LOW_TH);					   
	
		   vIO32WriteFldAlign(EH_06,EH_CLK_HIGH_TH,EH_FAST_MUTE_CLK_TOTAL_HIGH_TH);
		   vIO32WriteFldAlign(EH_06,EH_CLK_LOW_TH,EH_FAST_MUTE_CLK_TOTAL_LOW_TH);			
	
		   vIO32WriteFldAlign(EH_0B,(PANEL_GetPanelHeight()-1),EH_CALI_RANGE_H);						
				 
       #if EH_PANEL_FORMULATE         
		   //vIO32WriteFldAlign(EH_0C,PANEL_GetPanelWidth()+60,EH_DE_HS_START); 	  
		   //vIO32WriteFldAlign(EH_0D,PANEL_GetPanelHeight()+18,EH_DE_VS_START);		  
		   //vIO32WriteFldAlign(EH_0C,PANEL_GetPanelWidth()+60,EH_DE_HS_START); 	  
		   //vIO32WriteFldAlign(EH_0D,(V_LOW_TH-IO32ReadFldAlign(EH_0D,EH_DE_VS_WIDTH)),EH_DE_VS_START);						
		   vIO32WriteFldAlign(EH_0C, PANEL_GetHSyncWidth(),DE_HS_WIDTH);				
		   // [Note] minus 4 --> Dealy 4T in DE fix mode .
		   if (PANEL_GetHPosition60Hz()>=0x800)
		   {
		   vIO32WriteFldAlign(EH_0C,(PANEL_GetPanelWidth() -4 +(PANEL_GetHPosition60Hz() - 0x800)),EH_DE_HS_START); 	  
		   }
		   else
		   {
		   vIO32WriteFldAlign(EH_0C,(PANEL_GetPanelWidth() -4 -(0x800 - PANEL_GetHPosition60Hz())),EH_DE_HS_START); 	  
		   }
		   
		   vIO32WriteFldAlign(EH_0D, PANEL_GetVSyncWidth()-1,EH_DE_VS_WIDTH); 
		   // [Note] minus OS_75_VSYNC_DELSEL
		   // [Note] minus 2 --> Dealy 1 Line in DE fix mode and 1 Line in HSYNC & VSYNC align.
		   if (PANEL_GetVPosition()>=0x400)
		   {
		   vIO32WriteFldAlign(EH_0D,(PANEL_GetPanelHeight() + (wDrvGetOutputVTotal() - PANEL_GetPanelHeight() - 6) - 2 + (PANEL_GetVPosition() - 0x400)),EH_DE_VS_START);
		   }
		   else
		   {
		   vIO32WriteFldAlign(EH_0D,(PANEL_GetPanelHeight() + (wDrvGetOutputVTotal() - PANEL_GetPanelHeight() - 6) - 2 - (0x400 -PANEL_GetVPosition())),EH_DE_VS_START);
		   }
       #else
		   vErrorHandleSetByTiming();  
       #endif
		  
		   vIO32WriteFldAlign(EH_10,PANEL_GetPanelWidth(),EH_PTGEN_H_ACTIVE);			
		   vIO32WriteFldAlign(EH_10,wDrvGetOutputHTotal(),EH_PTGEN_H_TOTAL);				   
		 
		   vIO32WriteFldAlign(EH_12,PANEL_GetPanelHeight(),EH_PTGEN_V_ACTIVE);			 
		   vIO32WriteFldAlign(EH_12,wDrvGetOutputVTotal(),EH_PTGEN_V_TOTAL);	
		 
		   vIO32WriteFldAlign(EH_18,wDrvGetOutputVTotal(),EH_DE_VS_TOTAL_SW);			
		   vIO32WriteFldAlign(EH_18,wDrvGetOutputHTotal(),EH_DE_HS_TOTAL_SW); 
		 
		   // For Set Data Function
		   // vIO32WriteFldAlign(EH_19,PANEL_GetPanelWidth()+80,EH_FIX_HTOTAL_VALUE);			 
		   // vIO32WriteFldAlign(EH_19,PANEL_GetPanelWidth()+80,EH_FIX_HTOTAL_TH); 		
		   
			vIO32WriteFldAlign(EH_1A,PANEL_GetPanelWidth()-20,EH_FIX_VDE_TH);			
/*
		//for middle line of IPS2010 Panel	 
	#if HISENSE_IPS_2010_PANEL
			vIO32WriteFldAlign(EH_1A,PANEL_GetPanelWidth() + 2,EH_FIXED_DE_TOATL); 
	 #else
			vIO32WriteFldAlign(EH_1A,PANEL_GetPanelWidth(),EH_FIXED_DE_TOATL); 
	 #endif
*/
	 
	      //WXGA Panel , Error Handling FIXED_DE_TOATL+2=1368
		  if((PANEL_GetDisplayType()== PANEL_DISPLAY_TYPE_MLVDS) && (PANEL_GetPanelWidth() == 1366))
		  {
		   vIO32WriteFldAlign(EH_01, 0, EH_USE_SC_TIMING_EN);
		   vIO32WriteFldAlign(EH_0E, 1, EH_FIXED_DE_EN);		   
		   vIO32WriteFldAlign(EH_1A,PANEL_GetPanelWidth() + 2,EH_FIXED_DE_TOATL); 
		  }
		  else
		  {		  
		   vIO32WriteFldAlign(EH_1A,PANEL_GetPanelWidth(),EH_FIXED_DE_TOATL); 
		  } 
#endif

#ifndef NDEBUG	
Printf("---------- [NPTV][EH] vErrorHandleSetByPanel end----------\n"); 
#endif

}


void vErrorHandleSetByTiming(void)
{
    UINT16 u2HSyncWidth,u2VSyncWidth,u2HSyncStart,u2VSyncStart;
#ifndef NDEBUG	
    Printf("---------- [NPTV][EH] vErrorHandleSetByTiming ----------\n"); 
#endif
    u2HSyncWidth = wDrvGetOutputHTotal() - wDrvGetOutputHActive() - wDrvGetHsyncBp() - wDrvGetHsyncFp();
	u2HSyncStart = wDrvGetOutputHActive() + wDrvGetHsyncFp();

	u2VSyncWidth = wDrvGetOutputVTotal() - wDrvGetOutputVActive() - wDrvGetVsyncBp() - wDrvGetVsyncFp();
	u2VSyncStart = wDrvGetOutputVActive() + wDrvGetVsyncFp();

	
#ifndef NDEBUG
    Printf("[NPTV][EH] u2HSyncWidth=%d, u2HSyncStart=%d, u2VSyncWidth=%d, u2VSyncStart=%d\n", u2HSyncWidth, u2HSyncStart,u2VSyncWidth,u2VSyncStart);
#endif

    vIO32WriteFldAlign(EH_0C, u2HSyncWidth ,EH_DE_HS_WIDTH);
    // [Note] minus 4 --> Dealy 4T in DE fix mode .
    // 2 ch minus 8, 1 ch minus 4
    vIO32WriteFldAlign(EH_0C, u2HSyncStart - 4 ,EH_DE_HS_START);
	if (u2VSyncWidth >= 2)
	{
     vIO32WriteFldAlign(EH_0D, u2VSyncWidth - 1 ,EH_DE_VS_WIDTH); 
	} 
	else
	{
 	 vIO32WriteFldAlign(EH_0D, 1 ,EH_DE_VS_WIDTH); 
	}
    // [Note] minus OS_75_VSYNC_DELSEL 
    // [Note] minus 2 --> Dealy 1 Line in DE fix mode and 1 Line in HSYNC & VSYNC align.
    // [Note] 6line=1, HV_BYPASS=0, HV_hold=1, VBack porch delay 1 line
    // Need jimmy check
    if((IO32ReadFldAlign(VINTF_B0,VINTF_HV_HOLD) == 1)
       &&(IO32ReadFldAlign(VINTF_B0, VINTF_HV_BYPASS) == 0)
       && (IO32ReadFldAlign(VINTF_B0, VINTF_LINE_BUFFER_BYPASS) == 0))
    {
     vIO32WriteFldAlign(EH_0D, u2VSyncStart - 2 ,EH_DE_VS_START);
    }
    else
    {
     vIO32WriteFldAlign(EH_0D, u2VSyncStart - 1 ,EH_DE_VS_START);
    }

     // For Set Data Function
	 vIO32WriteFldAlign(EH_19,wDrvGetOutputHActive()+80,EH_FIX_HTOTAL_VALUE);			 
	 vIO32WriteFldAlign(EH_19,wDrvGetOutputHActive()+80,EH_FIX_HTOTAL_TH); 	
#ifndef NDEBUG		 
	 Printf("---------- [NPTV][EH] vErrorHandleSetByTiming end----------\n"); 
#endif
}
#define ErrorHandling_DEMODE 0
void vErrorHandleInit(void)
{
#ifndef NDEBUG	
    Printf("---------- [NPTV][EH] vErrorHandleInit ----------\n"); 
#endif
	if(PANEL_GetDisplayType()== PANEL_DISPLAY_TYPE_EPI)
	{
        vIO32WriteFldMulti(EH_00, P_Fld(1, EH_DISPR_DETECT_EN)|P_Fld(0, EH_INPUT_SEL)|P_Fld(1, EH_FRAME_METER_EN)|P_Fld(1, EH_INV_OUT_VS_EN)|P_Fld(1, EH_INV_OUT_HS_EN)|P_Fld(0, EH_DUAL_CHAN_PROCESS)|
                                  P_Fld(1, EH_H_CALI_WO_FILTER)|P_Fld(1, EH_REAL_TIME_CALI_EN)|P_Fld(0, EH_MUTE_MLVDS_EN)|P_Fld(0, EH_SET_DATA_TIME_SEL)|
                                  P_Fld(0, EH_XTAL_CLK_RST)|P_Fld(0, EH_OCLK_RST)|P_Fld(0, EH_REG_RST));
    }
    else
    {
        vIO32WriteFldMulti(EH_00, P_Fld(1, EH_DISPR_DETECT_EN)|P_Fld(0, EH_INPUT_SEL)|P_Fld(1, EH_FRAME_METER_EN)|P_Fld(1, EH_INV_OUT_VS_EN)|P_Fld(1, EH_INV_OUT_HS_EN)|P_Fld(0, EH_DUAL_CHAN_PROCESS)|
                                  P_Fld(1, EH_H_CALI_WO_FILTER)|P_Fld(1, EH_REAL_TIME_CALI_EN)|P_Fld(1, EH_MUTE_MLVDS_EN)|P_Fld(0, EH_SET_DATA_TIME_SEL)|
                                  P_Fld(0, EH_XTAL_CLK_RST)|P_Fld(0, EH_OCLK_RST)|P_Fld(0, EH_REG_RST));
    }
    vIO32WriteFldMulti(EH_01, P_Fld(1, EH_CLK_MISS_CHECK_EN)|P_Fld(0, EH_VDE_GEN_SEL)|P_Fld(0, EH_MUTE_DDDS_EN)|P_Fld(1, EH_DE_MISS_MUTE_EN)|
                              P_Fld(1, EH_USE_SC_TIMING_EN)|P_Fld(1, EH_INV_VS_EN)|P_Fld(1, IEH_NV_HS_EN)|P_Fld(1, EH_CLK_STABLE_CHECK_EN)|
                              P_Fld(1, EH_VDE_STABLE_CHECK_EN)|P_Fld(1, EH_DE_STABLE_CHECK_EN)|P_Fld(1, EH_V_STABLE_CHECK_EN)|P_Fld(1, EH_H_STABLE_CHECK_EN)|
                              P_Fld(1, EH_STABLE_CHECK_EN)|P_Fld(1, EH_CONFIG_RDY_WO_FM)|P_Fld(1, EH_CONFIG_RDY_WO_STABLE)|P_Fld(0, EH_DISP_MLVDS_WO_DDDS_LOCK)|
                              P_Fld(0, EH_DISP_MLVDS_WO_OD_RDY)|P_Fld(0, EH_SW_MUTE)|P_Fld(0, EH_SW_MUTE_EN)|P_Fld(0, EH_SCALER_MUTE_LVDS_RX_EN)|
                              P_Fld(1, EH_H_MISS_MUTE_EN)|P_Fld(1, EH_CLK_MUTE_EN)|P_Fld(0, EH_VDE_MUTE_EN)|P_Fld(0, EH_DE_MUTE_EN)|
                              P_Fld(1, EH_V_MUTE_EN)|P_Fld(1, EH_H_MUTE_EN)|P_Fld(1, EH_FAST_MUTE_EN));

    vIO32WriteFldMulti(EH_07, P_Fld(0, EH_CLK_MUTE_TEST_PERIOD)|P_Fld(2, EH_CLK_MISS_PERIOD_SEL)|P_Fld(5, EH_FAST_MUTE_CLK_CNT_TH)|P_Fld(0, EH_FAST_MUTE_V_CNT_TH)|
                              P_Fld(2, EH_FAST_MUTE_H_CNT_TH));                
    vIO32WriteFldMulti(EH_08, P_Fld(2, EH_V_STB_CNT_TH)|P_Fld(2, EH_V_STB_DIFF_TH)|P_Fld(5, EH_H_STB_CNT_TH)|P_Fld(0x0A, EH_H_STB_DIFF_TH));
    vIO32WriteFldMulti(EH_09, P_Fld(2, EH_VDE_STB_CNT_TH)|P_Fld(2, EH_VDE_STB_DIFF_TH)|P_Fld(5, EH_DE_STB_CNT_TH)|P_Fld(0x0A, EH_DE_STB_DIFF_TH));
    vIO32WriteFldMulti(EH_0A, P_Fld(0x80, EH_CLK_STB_CNT_TH)|P_Fld(2, EH_CLK_STB_DIFF_TH));
    // MT8283 Fail --> EH_FIXED_VDE_EN
    #if ErrorHandling_DEMODE
    vIO32WriteFldMulti(EH_0E, P_Fld(1, EH_FIXED_DE_EN)|P_Fld(0, EH_FIXED_VDE_EN)|P_Fld(1, EH_SET_LOW_HTOTAL_EN)|P_Fld(0, EH_WXGA_DATA_DELAY_SEL)|
                              P_Fld(0, EH_CALI_MONI_SEL)|P_Fld(0, EH_DE_COUNTER_RESET_SEL)|P_Fld(0, EH_DE_DEGLITCH_EN)|P_Fld(0, EH_HTOTAL_CALI_MEDAIN_EN)|
                              P_Fld(0, EH_HTOTAL_CALI_LOW_PASS_EN)|P_Fld(1, EH_MUTE_MLVDS_OFF_WHEN_PTGEN)|P_Fld(0, EH_MUTE_MLVDS_PERIOD)|P_Fld(0, EH_EH_MONI_SEL));        
	#else
    vIO32WriteFldMulti(EH_0E, P_Fld(0, EH_FIXED_DE_EN)|P_Fld(0, EH_FIXED_VDE_EN)|P_Fld(1, EH_SET_LOW_HTOTAL_EN)|P_Fld(0, EH_WXGA_DATA_DELAY_SEL)|
                              P_Fld(0, EH_CALI_MONI_SEL)|P_Fld(0, EH_DE_COUNTER_RESET_SEL)|P_Fld(0, EH_DE_DEGLITCH_EN)|P_Fld(0, EH_HTOTAL_CALI_MEDAIN_EN)|
                              P_Fld(0, EH_HTOTAL_CALI_LOW_PASS_EN)|P_Fld(1, EH_MUTE_MLVDS_OFF_WHEN_PTGEN)|P_Fld(0, EH_MUTE_MLVDS_PERIOD)|P_Fld(0, EH_EH_MONI_SEL));        
	#endif
    vIO32WriteFldMulti(EH_11, P_Fld(0x18, EH_PTGEN_H_WIDTH)|P_Fld(0x32, EH_PTGEN_H_START));
    vIO32WriteFldMulti(EH_13, P_Fld(0x10, EH_PTGEN_V_WIDTH)|P_Fld(0x13, EH_PTGEN_V_START));
    vIO32WriteFldMulti(EH_14, P_Fld(0, EH_PTGEN_TYPE_EN)|P_Fld(0, EH_PTGEN_B)|P_Fld(0, EH_PTGEN_G)|P_Fld(0, EH_PTGEN_R));
    vIO32WriteFldMulti(EH_15, P_Fld(0xFF, EH_AUTO_PTGEN_TIME_TH)|P_Fld(0, EH_PTGEN_H_RUN)|P_Fld(0, EH_PTGEN_V_RUN)|P_Fld(0, EH_AUTO_CHG_PTGEN_EN)|
                              P_Fld(2, EH_PTGEN_TYPE));
    vIO32WriteFldMulti(EH_16, P_Fld(0, EH_INTR_CLR)|P_Fld(0xFF, EH_INTR_MASK));
    vIO32WriteFldAlign(EH_17, 0x70, EH_FAST_MUTE_DE_MISS_TH);
    vIO32WriteFldAlign(EH_STATUS_05,0,EH_LVDS_RX_INTR);

    if (LVDS_OUTPUT_PORT == SINGLE_PORT) // WXGA/60
    {
      vIO32WriteFldMulti(EH_02, P_Fld(0xAAC, EH_FAST_MUTE_H_TOTAL_HIGH_TH)|P_Fld(0x578, EH_FAST_MUTE_H_TOTAL_LOW_TH));
      vIO32WriteFldMulti(EH_03, P_Fld(0x600, EH_FAST_MUTE_V_TOTAL_HIGH_TH)|P_Fld(0x310, EH_FAST_MUTE_V_TOTAL_LOW_TH));
      vIO32WriteFldMulti(EH_04, P_Fld(0x560, EH_FAST_MUTE_DE_TOTAL_HIGH_TH)|P_Fld(0x551, EH_FAST_MUTE_DE_TOTAL_HIGH_TH));
      vIO32WriteFldMulti(EH_05, P_Fld(0x310, EH_FAST_MUTE_VDE_TOTAL_HIGH_TH)|P_Fld(0x2F0, EH_FAST_MUTE_VDE_TOTAL_LOW_TH));
      vIO32WriteFldMulti(EH_06, P_Fld(0x228, EH_FAST_MUTE_CLK_TOTAL_HIGH_TH)|P_Fld(0x113, EH_FAST_MUTE_CLK_TOTAL_HIGH_TH));            
      vIO32WriteFldMulti(EH_0B, P_Fld(1, EH_CALI_RANGE_SEL)|P_Fld(0, EH_CALI_WO_STABLE)|P_Fld(0x2, EH_CALI_RANGE_L)|P_Fld(0x2FF, EH_CALI_RANGE_H));
      #if ErrorHandling_DEMODE	  
      vIO32WriteFldMulti(EH_0C, P_Fld(1, EH_DE_MODE_EN)|P_Fld(0x10, EH_DE_HS_WIDTH)|P_Fld(0x568, EH_DE_HS_START));
	  #else
	  vIO32WriteFldMulti(EH_0C, P_Fld(0, EH_DE_MODE_EN)|P_Fld(0x10, EH_DE_HS_WIDTH)|P_Fld(0x568, EH_DE_HS_START));
	  #endif
      vIO32WriteFldMulti(EH_0D, P_Fld(1, EH_DE_VS_WIDTH)|P_Fld(0x310, EH_DE_VS_START));
      vIO32WriteFldMulti(EH_10, P_Fld(0x558, EH_PTGEN_H_ACTIVE)|P_Fld(0x647, EH_PTGEN_H_TOTAL));
      vIO32WriteFldMulti(EH_12, P_Fld(0x300,EH_PTGEN_V_ACTIVE)|P_Fld(0x329, EH_PTGEN_V_TOTAL));  
      vIO32WriteFldMulti(EH_18, P_Fld(0, EH_CALI_HS_USE_SW_EN)|P_Fld(0, EH_CALI_VS_USE_SW_EN)|P_Fld(0, EH_CALI_DE_USE_SW_EN)|P_Fld(0, EH_CALI_VDE_USE_SW_EN)|
                                P_Fld(0x329, EH_DE_VS_TOTAL_SW)|P_Fld(0x647, EH_DE_HS_TOTAL_SW));     
      vIO32WriteFldMulti(EH_19, P_Fld(0x5A6, EH_FIX_HTOTAL_VALUE)|P_Fld(0x5A7, EH_FIX_HTOTAL_TH));
      vIO32WriteFldMulti(EH_1A, P_Fld(0x540, EH_FIX_VDE_TH)|P_Fld(0x558, EH_FIXED_DE_TOATL));    
    }
    else  if ((LVDS_OUTPUT_PORT == DUAL_PORT)||(LVDS_OUTPUT_PORT == FOUR_PORT)) // 1080P/60
    {
      vIO32WriteFldMulti(EH_02, P_Fld(0xFFF, EH_FAST_MUTE_H_TOTAL_HIGH_TH)|P_Fld(0x7D0, EH_FAST_MUTE_H_TOTAL_LOW_TH));
      vIO32WriteFldMulti(EH_03, P_Fld(0xFFF, EH_FAST_MUTE_V_TOTAL_HIGH_TH)|P_Fld(0x456, EH_FAST_MUTE_V_TOTAL_LOW_TH));
      vIO32WriteFldMulti(EH_04, P_Fld(0x788, EH_FAST_MUTE_DE_TOTAL_HIGH_TH)|P_Fld(0x779, EH_FAST_MUTE_DE_TOTAL_HIGH_TH));
      vIO32WriteFldMulti(EH_05, P_Fld(0x442, EH_FAST_MUTE_VDE_TOTAL_HIGH_TH)|P_Fld(0x42E, EH_FAST_MUTE_VDE_TOTAL_LOW_TH));
      vIO32WriteFldMulti(EH_06, P_Fld(0x114, EH_FAST_MUTE_CLK_TOTAL_HIGH_TH)|P_Fld(0x99, EH_FAST_MUTE_CLK_TOTAL_HIGH_TH));        
      vIO32WriteFldMulti(EH_0B, P_Fld(1, EH_CALI_RANGE_SEL)|P_Fld(0, EH_CALI_WO_STABLE)|P_Fld(0x2, EH_CALI_RANGE_L)|P_Fld(0x438, EH_CALI_RANGE_H));
	  #if ErrorHandling_DEMODE
      vIO32WriteFldMulti(EH_0C, P_Fld(1, EH_DE_MODE_EN)|P_Fld(0x10, EH_DE_HS_WIDTH)|P_Fld(0x7C4, EH_DE_HS_START));
	  #else
      vIO32WriteFldMulti(EH_0C, P_Fld(0, EH_DE_MODE_EN)|P_Fld(0x10, EH_DE_HS_WIDTH)|P_Fld(0x7C4, EH_DE_HS_START));
	  #endif
      vIO32WriteFldMulti(EH_0D, P_Fld(1, EH_DE_VS_WIDTH)|P_Fld(0x44A, EH_DE_VS_START));
      vIO32WriteFldMulti(EH_10, P_Fld(0x780, EH_PTGEN_H_ACTIVE)|P_Fld(0x898, EH_PTGEN_H_TOTAL));
      vIO32WriteFldMulti(EH_12, P_Fld(0x348,EH_PTGEN_V_ACTIVE)|P_Fld(0x464, EH_PTGEN_V_TOTAL));
      vIO32WriteFldMulti(EH_18, P_Fld(0, EH_CALI_HS_USE_SW_EN)|P_Fld(0, EH_CALI_VS_USE_SW_EN)|P_Fld(0, EH_CALI_DE_USE_SW_EN)|P_Fld(0, EH_CALI_VDE_USE_SW_EN)|
                                P_Fld(0x465, EH_DE_VS_TOTAL_SW)|P_Fld(0x898, EH_DE_HS_TOTAL_SW));     
      vIO32WriteFldMulti(EH_19, P_Fld(0x7D0, EH_FIX_HTOTAL_VALUE)|P_Fld(0x7D0, EH_FIX_HTOTAL_TH));
      vIO32WriteFldMulti(EH_1A, P_Fld(0x76C, EH_FIX_VDE_TH)|P_Fld(0x780, EH_FIXED_DE_TOATL));
    }

    if (LVDS_OUTPUT_PORT == FOUR_PORT)
    {
     vIO32WriteFldMulti(EH_06, P_Fld(0x8A, EH_FAST_MUTE_CLK_TOTAL_HIGH_TH)|P_Fld(0x4C, EH_FAST_MUTE_CLK_TOTAL_HIGH_TH));        
    }

    if(IS_LBRB_3D_ON) // sync mode , disp_r no regen on Error Handling
    {
     vIO32WriteFldAlign(EH_0C, 0 ,EH_DE_MODE_EN);     
    }
  vErrorHandleSetByPanel();
}

void vErrorHandleStatus(void)
{
	UINT32 u2MUTE_STATUS; 
	UINT32 u2Temp;
	UINT32 u2Temp_num,u2Temp_point;
	
	u2MUTE_STATUS = IO32ReadFldAlign(EH_STATUS_02,EH_FLD_ALL);
	
	Printf("---------- Error Handling Status ----------\n"); 
	
	Printf("[Panel Interface Information]\n");
	if (IO32ReadFldAlign(VINTF_P0,VINTF_PANEL_IN_SEL)==0)
	Printf("(0)From TCON-Error handling\n");
	else if (IO32ReadFldAlign(VINTF_P0,VINTF_PANEL_IN_SEL)==1)
	Printf("(1)From OD\n");
	else if (IO32ReadFldAlign(VINTF_P0,VINTF_PANEL_IN_SEL)==2)
	Printf("(2)From From SCPOS\n");

	Printf("[Error handling input]\n");    
	if (IO32ReadFldAlign(EH_00, EH_INPUT_SEL) == 1)
	Printf("[0] From OD (Test)\n");
	else if (IO32ReadFldAlign(EH_00, EH_INPUT_SEL) == 0)
	Printf("[1] From SCPOS (default) \n");
	
	Printf("[Clock Information]\n");
	if (IS_XTAL_24MHZ())
	{
		Printf("(Input)Clock=%d MHz\n",(((1024*24)/(IO32ReadFldAlign(EH_STATUS_04,EH_CLK_TOTAL_MON)))));
	}
	else if (IS_XTAL_27MHZ())
	{
		Printf("(Input)Clock=%d MHz\n",(((1024*27)/(IO32ReadFldAlign(EH_STATUS_04,EH_CLK_TOTAL_MON)))));
	}

	Printf("[Frame Rate]\n"); 
	u2Temp = IO32ReadFldAlign(EH_STATUS_0D,EH_XCLK_IN_FRAME);	
	if (IS_XTAL_24MHZ())
	{
		u2Temp_num = 24000000/u2Temp;
	  	u2Temp_point = (24000000 - u2Temp_num * u2Temp)*100/u2Temp;
		Printf("(Input) frame rate = %d.%2d Hz\n", u2Temp_num, u2Temp_point);
	}
	else
	{
		u2Temp_num = 27000000/u2Temp;
	  	u2Temp_point = (27000000 - u2Temp_num * u2Temp)*100/u2Temp;	
		Printf("(Input)frame rate = %d.%2d Hz\n", u2Temp_num, u2Temp_point);

	}
	
	Printf("[DISP_R status]\n"); 
	if(IO32ReadFldAlign(EH_STATUS_0D,EH_DISP_R_STATUS)==0)	
	{
		Printf("LRLR Mode\n");
	}
	else if(IO32ReadFldAlign(EH_STATUS_0D,EH_DISP_R_STATUS)==1)	
	{
		Printf("LLRR Mode\n");
	}
	else if(IO32ReadFldAlign(EH_STATUS_0D,EH_DISP_R_STATUS)==2)	
	{
		Printf("MUTE Mode\n");
	}
	else if(IO32ReadFldAlign(EH_STATUS_0D,EH_DISP_R_STATUS)==3)	
	{
		Printf("IDLE Mode\n");
	}
		
	Printf("[V,F Information]\n");
	Printf("(Input)DE_TOTAL_MON=%d, H_TOTAL_MON=%d, VDE_TOTAL_MON=%d, V_TOTAL_MON=%d\n",
			(IO32ReadFldAlign(EH_STATUS_00,EH_DE_TOTAL_MON)),((IO32ReadFldAlign(EH_STATUS_00,EH_H_TOTAL_MON)) + 1),
			IO32ReadFldAlign(EH_STATUS_01,EH_VDE_TOTAL_MON), (IO32ReadFldAlign(EH_STATUS_01,EH_V_TOTAL_MON)+1));		  
    if(IO32ReadFldAlign(EH_01,EH_USE_SC_TIMING_EN)==1)	
    {
		Printf("Ouput Status from Scaler \n");	
    }
	else
	{
		Printf("Ouput Status from Error Handling \n");	
	}
	u2Temp = IO32ReadFldAlign(EH_01,EH_USE_SC_TIMING_EN);
	vIO32WriteFldAlign(EH_01,1,EH_USE_SC_TIMING_EN);
	Printf("(Ouput#1 from scaler )DE_TOTAL_MON=%d, H_TOTAL_MON=%d, VDE_TOTAL_MON=%d, V_TOTAL_MON=%d\n",
			IO32ReadFldAlign(EH_STATUS_08,EH_DE_TOTAL_CALI),IO32ReadFldAlign(EH_STATUS_08,EH_H_TOTAL_CALI),
			IO32ReadFldAlign(EH_STATUS_09,EH_VDE_TOTAL_CALI), IO32ReadFldAlign(EH_STATUS_09,EH_V_TOTAL_CALI));
	vIO32WriteFldAlign(EH_01,0,EH_USE_SC_TIMING_EN);
	Printf("(Ouput#2 EH calculate)DE_TOTAL_MON=%d, H_TOTAL_MON=%d, VDE_TOTAL_MON=%d, V_TOTAL_MON=%d\n",
			IO32ReadFldAlign(EH_STATUS_08,EH_DE_TOTAL_CALI),IO32ReadFldAlign(EH_STATUS_08,EH_H_TOTAL_CALI),
			IO32ReadFldAlign(EH_STATUS_09,EH_VDE_TOTAL_CALI), IO32ReadFldAlign(EH_STATUS_09,EH_V_TOTAL_CALI));
	vIO32WriteFldAlign(EH_01,u2Temp,EH_USE_SC_TIMING_EN);
	Printf("(Ouput)H_WIDTH=%d, H_START=%d, V_WIDTH=%d, V_START=%d\n",
			IO32ReadFldAlign(EH_STATUS_0A,EH_H_WIDTH_CALI),IO32ReadFldAlign(EH_STATUS_0B,EH_H_START_CALI),
			IO32ReadFldAlign(EH_STATUS_0A,EH_V_WIDTH_CALI), IO32ReadFldAlign(EH_STATUS_0B,EH_V_START_CALI));	
	
	Printf("[UNREADY Status]\n"); 
	// if (((u2MUTE_STATUS >> EHS_OD_READY)& 0x01) == 0)
	// Printf("| OD_UNREADY");
	if (((u2MUTE_STATUS >> EHS_OD_OK)& 0x01) == 0)
	Printf("| OD_UNOK");
	if (((u2MUTE_STATUS >> EHS_DISP_OK)& 0x01) == 0)
	Printf("| DISP_UNOK");
	if (((u2MUTE_STATUS >> EHS_FAST_MUTE_READY)& 0x01) == 0)
	Printf("FAST_MUTE_UNREADY");
	Printf("\n");
	
	Printf("[MUTE Condition]\n");
	Printf("HOTAL[High = %4d,Low = %4d]\n",
			IO32ReadFldAlign(EH_02,EH_FAST_MUTE_H_TOTAL_HIGH_TH),
			IO32ReadFldAlign(EH_02,EH_FAST_MUTE_H_TOTAL_LOW_TH));
	Printf("VOTAL[High = %4d,Low = %4d]\n",
			IO32ReadFldAlign(EH_03,EH_FAST_MUTE_V_TOTAL_HIGH_TH),
			IO32ReadFldAlign(EH_03,EH_FAST_MUTE_V_TOTAL_LOW_TH));
	Printf("DE   [High = %4d,Low = %4d]\n",
			IO32ReadFldAlign(EH_04,EH_FAST_MUTE_DE_TOTAL_HIGH_TH),
			IO32ReadFldAlign(EH_04,EH_FAST_MUTE_DE_TOTAL_LOW_TH)); 		 
	Printf("VDE  [High = %4d,Low = %4d]\n",
			IO32ReadFldAlign(EH_05,EH_FAST_MUTE_VDE_TOTAL_HIGH_TH),
			IO32ReadFldAlign(EH_05,EH_FAST_MUTE_VDE_TOTAL_LOW_TH)); 		 
	if (IS_XTAL_24MHZ())
	{
		Printf("Clock[High = %d MHz,Low = %d MHz]\n",
			(((1024*24)/(IO32ReadFldAlign(EH_06,EH_FAST_MUTE_CLK_TOTAL_LOW_TH)))),
			(((1024*24)/(IO32ReadFldAlign(EH_06,EH_FAST_MUTE_CLK_TOTAL_HIGH_TH)))));
		}
	else if (IS_XTAL_27MHZ())
	{
		Printf("Clock[High = %d MHz,Low = %d MHz]\n",
			(((1024*27)/(IO32ReadFldAlign(EH_06,EH_FAST_MUTE_CLK_TOTAL_LOW_TH)))),
			(((1024*27)/(IO32ReadFldAlign(EH_06,EH_FAST_MUTE_CLK_TOTAL_HIGH_TH)))));
	}
	
	Printf("[Mute Status]\n");	  
	if (((u2MUTE_STATUS >> EHS_OD_INTF_ERROR)& 0x01) == 1)
	Printf("OD_INTF_ERROR");
	if (((u2MUTE_STATUS >> EHS_MUTE_LVDS)& 0x01) == 1)
	Printf("| MUTE_MLVDS");
	if (((u2MUTE_STATUS >> EHS_DE_MISS_MUTE)& 0x01) == 1)
	Printf("| DE_MISS_MUTE");
	if (((u2MUTE_STATUS >> EHS_INPUT_OK)& 0x01) == 1)
	Printf("| INPUT_OK");
	// if (((u2MUTE_STATUS >> EHS_OD_BYPASS)& 0x01) == 1)
	// Printf("| OD_BYPASS");
	if (((u2MUTE_STATUS >> EHS_OD_MUTE_DDDS)& 0x01) == 1)
	Printf("| OD_MUTE_DDDS");
	Printf("\n");	 
	
	if (((u2MUTE_STATUS >> EHS_H_MISS_MUTE)& 0x01) == 1)
	Printf("| H_MISS_MUTE");
	if (((u2MUTE_STATUS >> EHS_CLOCK_MUTE)& 0x01) == 1)
	Printf("| CLOCK MUTE");
	if (((u2MUTE_STATUS >> EHS_VDE_MUTE)& 0x01) == 1)
	Printf("| VDE_MUTE");
	if (((u2MUTE_STATUS >> EHS_V_MUTE)& 0x01) == 1)
	Printf("| V_MUTE");
	if (((u2MUTE_STATUS >> EHS_DE_MUTE)& 0x01) == 1)
	Printf("| DE_MUTE");
	if (((u2MUTE_STATUS >> EHS_H_MUTE)& 0x01) == 1)
	Printf("| H_MUTE");
	if (((u2MUTE_STATUS >> EHS_MUTE)& 0x01) == 1)
	Printf("| MUTE");  
	Printf("\n");	 
	
	Printf("[UNSTABLE Status]\n");	  
	
	if (((u2MUTE_STATUS >> EHS_CONFIG_READY)& 0x01) == 0)
	Printf("Configuration_UNREADY");
	//if (((u2MUTE_STATUS >> EHS_DDDS_LOCK)& 0x01) == 0)
	//Printf("| DDDS_UNLOCK");
	if (((u2MUTE_STATUS >> EHS_CLOCK_STABLE)& 0x01) == 0)
	Printf("| CLOCK_UNSTABLE");
	if (((u2MUTE_STATUS >> EHS_VDE_STABLE)& 0x01) == 0)
	Printf("| VDE_UNSTABLE");
	if (((u2MUTE_STATUS >> EHS_V_STABLE)& 0x01) == 0)
	Printf("| V_UNSTABLE");
	if (((u2MUTE_STATUS >> EHS_DE_STABLE)& 0x01) == 0)
	Printf("| DE_UNSTABLE");
	if (((u2MUTE_STATUS >> EHS_H_STABLE)& 0x01) == 0)
	Printf("| H_UNSTABLE");
	if (((u2MUTE_STATUS >> EHS_STABLE)& 0x01) == 0)
	Printf("| UNSTABLE");  
	Printf("\n");	 
			

}

void vScEhSta(void)
{
	UINT32 u2Temp;
	u2Temp = IO32ReadFldAlign(EH_01,EH_USE_SC_TIMING_EN);
	vIO32WriteFldAlign(EH_01,1,EH_USE_SC_TIMING_EN);
	Printf("(Ouput#1 from scaler )DE_TOTAL_MON=%d, H_TOTAL_MON=%d, VDE_TOTAL_MON=%d, V_TOTAL_MON=%d\n",
			IO32ReadFldAlign(EH_STATUS_08,EH_DE_TOTAL_CALI),IO32ReadFldAlign(EH_STATUS_08,EH_H_TOTAL_CALI),
			IO32ReadFldAlign(EH_STATUS_09,EH_VDE_TOTAL_CALI), IO32ReadFldAlign(EH_STATUS_09,EH_V_TOTAL_CALI));
	vIO32WriteFldAlign(EH_01,0,EH_USE_SC_TIMING_EN);
	Printf("(Ouput#2 EH calculate)DE_TOTAL_MON=%d, H_TOTAL_MON=%d, VDE_TOTAL_MON=%d, V_TOTAL_MON=%d\n",
			IO32ReadFldAlign(EH_STATUS_08,EH_DE_TOTAL_CALI),IO32ReadFldAlign(EH_STATUS_08,EH_H_TOTAL_CALI),
			IO32ReadFldAlign(EH_STATUS_09,EH_VDE_TOTAL_CALI), IO32ReadFldAlign(EH_STATUS_09,EH_V_TOTAL_CALI));
	vIO32WriteFldAlign(EH_01,u2Temp,EH_USE_SC_TIMING_EN);
}

void u1ErrorHandleFixEnable(UINT8 val)
{
  vIO32WriteFldMulti(EH_0E, P_Fld(val, EH_FIXED_DE_EN)
                                       |P_Fld(val, EH_FIXED_VDE_EN)|P_Fld(val, EH_DE_DEGLITCH_EN));

}

void u1ErrorHandleMuteEnable(UINT8 val)
{
   vIO32WriteFldAlign(EH_00, val, EH_MUTE_MLVDS_EN);   
}


void u1ErrorHandlingSourceSet(UINT8 val)
{
  if (val<=1)
  {
   vIO32WriteFldAlign(EH_00, val, EH_INPUT_SEL);   
  } 
}

UINT8 u1ErrorHandlingSourceGet(void)
{
  return (IO32ReadFldAlign(EH_00, EH_INPUT_SEL));
}

/**
 * @brief vDrvErrorHandleEN

 * Error handle enable control

 * @param  bEn
 * @retval void
 */
/*
static void vDrvErrorHandleEN(UINT8 bEn) 
{
    vIO32WriteFldAlign(EH_00,bEn,EH_MUTE_MLVDS_EN);
    vIO32WriteFldMulti(EH_01,P_Fld(bEn,EH_FAST_MUTE_EN)|P_Fld(bEn,EH_DE_MISS_MUTE_EN));

}
*/

//Gamma Initialize	
static void vDrvTconGammaInit(void)
{	
	#if  HISENSE_IPS_2010_PANEL 
		   x_thread_delay(15);
	#endif
	
		if (u1TconCustGammaInit() == 1)
		{
			_u1GammaReady = 1;
		}
		else
		{
			_u1GammaReady = 0;
		}
		
		LOG(1,"Tcon Gamma init\n");
}

//EPI Initialize
UINT8 u1DrvEPIPreInit(void)
{
	Printf("[EPI] vDrvEPI_PreInit \n");
	// TCON_TOP
	vIO32WriteFldMulti(TMGR2, P_Fld(0, TMGR2_TCLK_PLLRST)|P_Fld(0, TMGR2_TCLK_PLLSEL)|P_Fld(1, TMGR2_TCLK_PLLBP)
							  |P_Fld(4, TMGR2_RG_FIFO_CK_SEL)|P_Fld(0xFFFF, TMGR2_CKEN_CFG1));
	vIO32WriteFldMulti(TMGR0, P_Fld(1, CKEN_CFG_EH)|P_Fld(1, CKEN_CFG_TCON)|P_Fld(1, CKEN_CFG_PWM_SCAN)
							  |P_Fld(1, CKEN_CFG_MLVDS)|P_Fld(0, CKEN_CFG_VB1)|P_Fld(0, CKEN_CFG_iDP)
							  |P_Fld(1, CKEN_CFG_EPI)|P_Fld(0, CKEN_CFG_LVDS_ANA)|P_Fld(0, CKEN_CFG_PPML)
							  |P_Fld(0, CKEN_CFG_LVDS_TX)|P_Fld(1, CKEN_CFG_Pixel_Reorder)|P_Fld(1, CKEN_CFG_2Port_FIFO));
		 
	vIO32WriteFldMulti(TMGR3, P_Fld(4, TMGR3_RG_TCON_OUT_SEL));
	vIO32WriteFldMulti(TMGR5, P_Fld(2, TMGR5_RG_FIFO_CTRL));

	//EPI MUTE PATTERN
	vIO32WriteFldMulti(MUTE_PATTERN_02, P_Fld(wDrvGetOutputHTotal(), MUTE_H_TOTAL)|P_Fld(0x1C, MUTE_H_WIDTH));
	vIO32WriteFldMulti(MUTE_PATTERN_03, P_Fld(0x20, MUTE_H_START)|P_Fld(wDrvGetOutputHActive(), MUTE_H_ACTIVE));
	vIO32WriteFldMulti(MUTE_PATTERN_04, P_Fld(wDrvGetOutputVTotal(), MUTE_V_TOTAL)|P_Fld(1, MUTE_V_WIDTH));
	vIO32WriteFldMulti(MUTE_PATTERN_05, P_Fld(8, MUTE_V_START)|P_Fld(wDrvGetOutputVActive(), MUTE_V_ACTIVE));
	   
	return 1;
}

UINT8 u1DrvEPIPostInit(void)
{
    Printf("[EPI] vDrvEPI_PostInit \n");
	//EPI FIFO Reset
    vIO32WriteFldMulti(MLVDS_CTRL_09, P_Fld(1, ASYNC_FIFO_RST)|P_Fld(4, RG_FIFO_SEL));
	vIO32WriteFldMulti(MLVDS_CTRL_09, P_Fld(0, ASYNC_FIFO_RST)|P_Fld(4, RG_FIFO_SEL));

	//EPI TCLK Start
	vIO32WriteFldMulti(FIFO_CTRL_01, P_Fld(0, TCLK_FIFO_START));
	
	if (IS_EPI_8LANE)
	{
		vIO32WriteFldAlign(FIFO_CTRL_01, 7, TCLK_FIFO_ACTIVE_PORT);  // 8 lane
	}
	else if(IS_EPI_4LANE)
	{
		vIO32WriteFldAlign(FIFO_CTRL_01, 3, TCLK_FIFO_ACTIVE_PORT);  // 4 lane
	}
	else
	{
		vIO32WriteFldAlign(FIFO_CTRL_01, 5, TCLK_FIFO_ACTIVE_PORT);  // 6 lane
	}
	
	vIO32WriteFldMulti(FIFO_CTRL_01, P_Fld(1, TCLK_FIFO_START));

	//Enable D/A Interface FIFO
	vIO32WriteFldMulti(TMGR5, P_Fld(1, TMGR5_RG_FIFO_EN));
	
	return 1;
}

void vDrvEPI_AnalogInit(void)
{
	Printf("[EPI] vDrvEPI_AnalogInit \n");

#ifdef CC_MT5399
		//power down B group
		vIO32WriteFldAlign(REG_LVDS_TX_CFG0, 0, RG_LVDSB_EN_D11 );
		//B group main driver power down
		//vIO32WriteFldAlign(REG_LVDS_TX_CFG1, 0xfff, RG_LVDSB_DRV_PWD);
		vIO32WriteFldMulti(REG_LVDS_TX_CFG1, P_Fld(0, RG_LVDSB_DRV_EN_EVEN_D11)|P_Fld(0,RG_LVDSB_DRV_EN_ODD_D11));	
		//B group pre driver power down
		//vIO32WriteFldAlign(REG_LVDS_TX_CFG2, 0xfff, RG_LVDSB_PRE_PWD);
		vIO32WriteFldMulti(REG_LVDS_TX_CFG1, P_Fld(0, RG_LVDSB_PREEM_EN_EVEN_D11)|P_Fld(0,RG_LVDSB_PREEM_EN_ODD_D11));	
		//vIO32WriteFldAlign(REG_LVDS_TX_CFG0, 0, RG_LVDSB_EXT_EN_D11);   //need to check
		//B group bias current power down
		vIO32WriteFldAlign(REG_LVDS_TX_CFG0, 0, RG_LVDSB_BIAS_EN_D11 );
#endif	
		//A group ch1~8 AE3 AE2 AE0 AO4 AO3 AOCK AE1 AECK	
		if(fgIsSupport120HzPanel())
		{
			vIO32WriteFldMulti(REG_LVDS_TX_CFG11, P_Fld(0xA, RG_LVDSA_TVO)|P_Fld(4, RG_LVDSA_TVCM)|P_Fld(4, RG_LVDSA_NSRC)|
												 P_Fld(4, RG_LVDSA_PSRC));
			vIO32WriteFldMulti(REG_LVDS_TX_CFG12, P_Fld(1, RG_LVDSA_TVO_PRE)|P_Fld(0xC, RG_LVDSA_BIAS_SEL)|P_Fld(4, RG_LVDSA_NSRC_PRE)|
												 P_Fld(4, RG_LVDSA_PSRC_PRE));
			#ifdef CC_MT5399
			vIO32WriteFldAlign(REG_LVDS_TX_CFG13, 0xFFF, RG_LVDSA_PRE_PWD);
			#else
			vIO32WriteFldAlign(REG_LVDS_TX_CFG13, 0x3FFF, RG_LVDSA_PRE_PWD);
			#endif
		}
		else
		{
			vIO32WriteFldMulti(REG_LVDS_TX_CFG11, P_Fld(0x8, RG_LVDSA_TVO)|P_Fld(0x0B, RG_LVDSA_TVCM)|P_Fld(4, RG_LVDSA_NSRC)|
											 P_Fld(4, RG_LVDSA_PSRC));
			vIO32WriteFldMulti(REG_LVDS_TX_CFG12, P_Fld(0, RG_LVDSA_TVO_PRE)|P_Fld(0xC, RG_LVDSA_BIAS_SEL)|P_Fld(4, RG_LVDSA_NSRC_PRE)|
												 P_Fld(4, RG_LVDSA_PSRC_PRE));
			#ifdef CC_MT5399
			vIO32WriteFldAlign(REG_LVDS_TX_CFG13, 0xFFF, RG_LVDSA_PRE_PWD);
			#else
			vIO32WriteFldAlign(REG_LVDS_TX_CFG13, 0x3FFF, RG_LVDSA_PRE_PWD);
			#endif
			
		}	
}

void vDrvEPI_AnalogOn(void)
{
	Printf("[LVDS] vDrvEPI_AnalogOn \n");

    //A group ch1~8 AE3 AE2 AE0 AO4 AO3 AOCK AE1 AECK
    //A group bias current power on
	vIO32WriteFldMulti(REG_LVDS_TX_CFG11, P_Fld(0, RG_LVDSA_BIAS_ODD_PWD)|P_Fld(0, RG_LVDSA_BIAS_EVEN_PWD));
	//A group clk regulator power on
    vIO32WriteFldMulti(REG_LVDS_TX_CFG19, P_Fld(0, RG_LVDSA_LDO_ODD_PWD_D11)|P_Fld(0, RG_LVDSA_LDO_BIAS_ODD_PWD_D11)|
                                          P_Fld(0, RG_LVDSA_LDO_EVEN_PWD_D11)|P_Fld(0, RG_LVDSA_LDO_BIAS_EVEN_PWD_D11));
#ifdef CC_MT5399
	if (IS_EPI_8LANE)
	{
		vIO32WriteFldAlign(REG_LVDS_TX_CFG11, 0xBF8, RG_LVDSA_EXT_EN_D11); //Select EPI 8 LANE Output
		vIO32WriteFldAlign(REG_LVDS_TX_CFG12, 0x407, RG_LVDSA_DRV_PWD);
	}
	else if(IS_EPI_4LANE)
	{
		vIO32WriteFldAlign(REG_LVDS_TX_CFG11, 0x350, RG_LVDSA_EXT_EN_D11); //Selest EPI 4 LANE Output
		vIO32WriteFldAlign(REG_LVDS_TX_CFG12, 0xCAF, RG_LVDSA_DRV_PWD);
	}
	else
	{
		vIO32WriteFldAlign(REG_LVDS_TX_CFG11, 0x378, RG_LVDSA_EXT_EN_D11); //Selest EPI 6 LANE Output
		vIO32WriteFldAlign(REG_LVDS_TX_CFG12, 0xC87, RG_LVDSA_DRV_PWD);
	}
#else
	if (IS_EPI_8LANE)
	{
		vIO32WriteFldAlign(REG_LVDS_TX_CFG11, 0x0BF8, RG_LVDSA_EXT_EN_D11); //Selest EPI 8 LANE Output
		vIO32WriteFldAlign(REG_LVDS_TX_CFG12, 0x3407, RG_LVDSA_DRV_PWD);
	}
	else if(IS_EPI_4LANE)
	{
		vIO32WriteFldAlign(REG_LVDS_TX_CFG11, 0x0350, RG_LVDSA_EXT_EN_D11); //Selest EPI 4 LANE Output
		vIO32WriteFldAlign(REG_LVDS_TX_CFG12, 0x3CAF, RG_LVDSA_DRV_PWD);
	}
	else 
	{
		vIO32WriteFldAlign(REG_LVDS_TX_CFG11, 0x0378, RG_LVDSA_EXT_EN_D11); //Selest EPI 6 LANE Output
		vIO32WriteFldAlign(REG_LVDS_TX_CFG12, 0x3C87, RG_LVDSA_DRV_PWD);
	}
#endif

	//power on A group
	vIO32WriteFldAlign(REG_LVDS_TX_CFG11, 0x0, RG_LVDSA_EN);
	HAL_Delay_us(LVDSA_EN_REST_DELAYTIME);
    vIO32WriteFldAlign(REG_LVDS_TX_CFG11, 0x1, RG_LVDSA_EN);

#ifdef CC_MTK_LOADER
   vDrvLVDS7To4FifoEnable();
#else
   _Lvds7To4FifoReset = 2;
#endif

}

void vDrvEPI_AnalogOff(void)
{
	Printf("[EPI] vDrvEPI_AnalogOff \n");

#ifdef CC_MT5399
		//power down B group
		vIO32WriteFldMulti(REG_LVDS_TX_CFG0, P_Fld(0, RG_LVDSB_EN_D11 )|P_Fld(0, RG_LVDSB_BIAS_EN_D11 ));
		//B group main driver power down
		vIO32WriteFldMulti(REG_LVDS_TX_CFG1, P_Fld(0, RG_LVDSB_DRV_EN_EVEN_D11)|P_Fld(0,RG_LVDSB_DRV_EN_ODD_D11));	
		//B group pre driver power down
		vIO32WriteFldMulti(REG_LVDS_TX_CFG1, P_Fld(0, RG_LVDSB_PREEM_EN_EVEN_D11)|P_Fld(0,RG_LVDSB_PREEM_EN_ODD_D11));
		//B group clk regulator power down
		//vIO32WriteFldAlign(REG_LVDS_TX_CFG8, 1, RG_LVDSB_LDO_PWD_D11);  //need to check
#endif
		//A group ch1~8 AE3 AE2 AE0 AO4 AO3 AOCK AE1 AECK
		//power down A group
		vIO32WriteFldAlign(REG_LVDS_TX_CFG11, 0, RG_LVDSA_EN);
		
		#ifdef CC_MT5399
		//A group main driver power down
		vIO32WriteFldAlign(REG_LVDS_TX_CFG12, 0xfff, RG_LVDSA_DRV_PWD);
		//A group pre driver power down
		vIO32WriteFldAlign(REG_LVDS_TX_CFG13, 0xfff, RG_LVDSA_PRE_PWD);
		#else
		//A group main driver power down
		vIO32WriteFldAlign(REG_LVDS_TX_CFG12, 0x3fff, RG_LVDSA_DRV_PWD);
		//A group pre driver power down
		vIO32WriteFldAlign(REG_LVDS_TX_CFG13, 0x3fff, RG_LVDSA_PRE_PWD);
		#endif
		vIO32WriteFldAlign(REG_LVDS_TX_CFG11, 0, RG_LVDSA_EXT_EN_D11);
	
		//A group clk regulator power off
		vIO32WriteFldMulti(REG_LVDS_TX_CFG19, P_Fld(1, RG_LVDSA_LDO_ODD_PWD_D11)|P_Fld(1, RG_LVDSA_LDO_BIAS_ODD_PWD_D11)|
											  P_Fld(1, RG_LVDSA_LDO_EVEN_PWD_D11)|P_Fld(1, RG_LVDSA_LDO_BIAS_EVEN_PWD_D11));
		//A group bias current power down
		vIO32WriteFldMulti(REG_LVDS_TX_CFG11, P_Fld(1, RG_LVDSA_BIAS_ODD_PWD)|P_Fld(1, RG_LVDSA_BIAS_EVEN_PWD));	

}

/**
 * @brief u1DrvEPITgOnOff

 * TCON power control

 * @param  void
 * @retval 0: fail, 1: success
 */
UINT8 u1DrvEPITgOnOff(VIDEO_TCON_POWER_SEQ_T eTconPowSeq, BOOL val)
{
    UINT8 u1Ret = 0;

	switch (eTconPowSeq)
	{
        case VIDEO_TCON_GST:
            vIO32WriteFldAlign(GR0, val, GR0_TCTG_OEN_GST);
//            vIO32WriteFldAlign(GR0, val, GR0_TCTG_OEN_GST_END);
            break;
        case VIDEO_TCON_MCLK:
            vIO32WriteFldAlign(GR0, val, GR0_TCTG_OEN_MCLK);
            break;
        case VIDEO_TCON_GCLK:
            vIO32WriteFldAlign(GR0, val, GR0_TCTG_OEN_GCLK);
            vIO32WriteFldAlign(GR0, val, GR0_TCTG_OEN_GCLK_END);
            vIO32WriteFldAlign(GR0, val, GR0_TCTG_OEN_GST_END);
            break;
        case VIDEO_TCON_EO:
            vIO32WriteFldAlign(GR0, val, GR0_TCTG_OEN_EO);
            vIO32WriteFldAlign(GR0, val, GR0_TCTG_OEN_EO2);
            break;
        case VIDEO_TCON_VCOM_DYN:
            vIO32WriteFldAlign(GR0, val, GR0_TCTG_OEN_VCOM_DYN);
            break;
        case VIDEO_TCON_EPISIGNAL:
            if (val)
            {
                vDrvEPI_AnalogOn();
            }
            else
                vDrvEPI_AnalogOff();
            break;
        case VIDEO_TCON_ALL:
            if (val)
                vIO32WriteFldAlign(GR0, 0xffff, GR0_TCTG_OEN);
            else
                vIO32WriteFldAlign(GR0, 0, GR0_TCTG_OEN);
            break;

        default:
            bbypass_isr = val;
            break;
    }

    if(IO32ReadFldAlign(GR0, GR0_TC_EN))
        u1Ret = 1;
    else
        u1Ret = 0;
	return u1Ret;	
}

#ifdef CONFIG_OPM
    BOOL _fgEPIInit = FALSE;
#else
    static BOOL _fgEPIInit = FALSE;
#endif
void vDrvEPIInit(void)
{
    if (IO32ReadFldAlign(DDDS_08, FLAG_EPI_INITIAL) == 1)
    {
        _fgEPIInit = TRUE;
    }
	
	if (!_fgEPIInit)
	{
		Printf("[Mini-LVDS] vDrvMiniLVDS_Init(START) \n");
		//EPI Digital initial
		u1DrvEPIPreInit();
		u1TconCustEPIInit();
		u1DrvEPIPostInit();
		//EPI Analog initial
		vDrvEPI_AnalogInit();

		//LOCKN manual control
		#ifdef SUPPORT_LOCK_FROM_SRVAD
	   		vIO32WriteFldAlign(TMGR18, 1, TMGR18_RG_LOCK_CTRL);
	   		vIO32WriteFldAlign(TMGR18, 0, TMGR18_RG_LOCK_VALUE);
		#endif
		
		_fgEPIInit = TRUE;
		vIO32WriteFldAlign(DDDS_08, 1, FLAG_EPI_INITIAL);
	}
}

void vDrvEPISLT(void)
{
#ifdef CC_MT5399
vIO32Write4B(0xf0060038, 0x00000000);
vIO32Write4B(0xf0060034, 0x0FFF0000);
vIO32Write4B(0xf0060030, 0x0C480000);
vIO32Write4B(0xf006004C, 0x00000000);
//vIO32Write4B(0xf0060000, 0x000EC000);  need to check
//vIO32Write4B(0xf0060004, 0x01000FFF);  need to check
//vIO32Write4B(0xf0060008, 0x0FFF0000);  need to check
//vIO32Write4B(0xf0060020, 0x0001C000);  need to check
vIO32Write4B(0xf006002C, 0xA4490FFF);
vIO32Write4B(0xf0034014, 0x00710000);
vIO32Write4B(0xf003402C, 0x2010012C);
vIO32Write4B(0xf0034030, 0x06008010);
vIO32Write4B(0xf0034034, 0x00000003);
vIO32Write4B(0xf0034038, 0x00210000);

#endif
}

//Mini-LVDS Initialize
UINT8 u1DrvMlvdsPreInit(void)
{
	Printf("[Mini-LVDS] vDrvMiniLVDS_PreInit \n");
   // TCON_TOP
   vIO32WriteFldMulti(TMGR2, P_Fld(0, TMGR2_TCLK_PLLRST)|P_Fld(0, TMGR2_TCLK_PLLBP)
							|P_Fld(1, TMGR2_RG_FIFO_CK_SEL)|P_Fld(0xFFFF, TMGR2_CKEN_CFG1));
   vIO32WriteFldMulti(TMGR0, P_Fld(1, CKEN_CFG_EH)|P_Fld(1, CKEN_CFG_TCON)|P_Fld(1, CKEN_CFG_PWM_SCAN)
     						|P_Fld(1, CKEN_CFG_MLVDS)|P_Fld(0, CKEN_CFG_VB1)|P_Fld(0, CKEN_CFG_iDP)
							|P_Fld(0, CKEN_CFG_EPI)|P_Fld(0, CKEN_CFG_LVDS_ANA)|P_Fld(0, CKEN_CFG_PPML)
							|P_Fld(0, CKEN_CFG_LVDS_TX)|P_Fld(1, CKEN_CFG_Pixel_Reorder)|P_Fld(1, CKEN_CFG_2Port_FIFO));
	 
   vIO32WriteFldMulti(TMGR3, P_Fld(1, TMGR3_RG_TCON_OUT_SEL));
   vIO32WriteFldMulti(TMGR5, P_Fld(4, TMGR5_RG_FIFO_CTRL));

   // WXGA mini-LVDS Mute Pattern
   if(PANEL_GetPanelWidth() == 1366)
   {
	   vIO32WriteFldMulti(MUTE_PATTERN_02, P_Fld(wDrvGetOutputHTotal(), MUTE_H_TOTAL)|P_Fld(8, MUTE_H_WIDTH));
	   vIO32WriteFldMulti(MUTE_PATTERN_03, P_Fld(0x20, MUTE_H_START)|P_Fld(0x556, MUTE_H_ACTIVE));
	   vIO32WriteFldMulti(MUTE_PATTERN_04, P_Fld(wDrvGetOutputVTotal(), MUTE_V_TOTAL)|P_Fld(1, MUTE_V_WIDTH));
	   vIO32WriteFldMulti(MUTE_PATTERN_05, P_Fld(6, MUTE_V_START)|P_Fld(0x300, MUTE_V_ACTIVE));
   }
   else
   {
		vIO32WriteFldMulti(MUTE_PATTERN_02, P_Fld(wDrvGetOutputHTotal(), MUTE_H_TOTAL)|P_Fld(0x1C, MUTE_H_WIDTH));
		vIO32WriteFldMulti(MUTE_PATTERN_03, P_Fld(0x20, MUTE_H_START)|P_Fld(wDrvGetOutputHActive(), MUTE_H_ACTIVE));
		vIO32WriteFldMulti(MUTE_PATTERN_04, P_Fld(wDrvGetOutputVTotal(), MUTE_V_TOTAL)|P_Fld(1, MUTE_V_WIDTH));
		vIO32WriteFldMulti(MUTE_PATTERN_05, P_Fld(8, MUTE_V_START)|P_Fld(wDrvGetOutputVActive(), MUTE_V_ACTIVE));
   }
   return 1;
}

void vDrvMiniLVDS_AnalogInit(void)
{
    UINT32 u4Value;

	Printf("[Mini-LVDS] vDrvMiniLVDS_AnalogInit \n");	
	
    #ifdef CC_MTK_LOADER
	vDrvLVDS7To4FifoEnable();
    #endif
        
    // Set Driving Current (TVO)
    u4Value = PANEL_GetDrivingCurrent();
    if (u4Value <= 15)
    {
      vDrvLVDSSetDriving(u4Value);
    }
    else
    {
      vDrvLVDSSetDriving(4); //4mA	
    }

	 #ifdef CC_MT5399
	 //Set RG_LVDSA/B_EN to Keep Low
	 vIO32WriteFldAlign(REG_LVDS_TX_CFG0, 0,RG_LVDSB_EN_D11 );
	 #endif
	 vIO32WriteFldAlign(REG_LVDS_TX_CFG11, 0,RG_LVDSA_EN); 
	 
	 // Set VCM (TVCM)
	 vIO32WriteFldMulti(REG_LVDS_TX_CFG11, P_Fld(0x0A, RG_LVDSA_TVCM)|P_Fld(1, RG_LVDSA_NSRC)|P_Fld(1, RG_LVDSA_PSRC));
	 #ifdef CC_MT5399	 
	 vIO32WriteFldAlign(REG_LVDS_TX_CFG0, 0x0A, RG_LVDSB_TVCM_D11 );
	 #endif

	 #ifdef CC_MT5399
	 //Set PREEM_EN
	 vIO32WriteFldAlign(REG_LVDS_TX_CFG1, 0, RG_LVDSB_PREEM_EN_EVEN_D11 );
	 vIO32WriteFldAlign(REG_LVDS_TX_CFG1, 0, RG_LVDSB_PREEM_EN_ODD_D11 );
	 #endif
	 
	 //Set Bias Voltage (BIAS_SEL)
	 vIO32WriteFldAlign(REG_LVDS_TX_CFG12, 4, RG_LVDSA_BIAS_SEL); // for mini LVDS BIAS
	 //vIO32WriteFldAlign(REG_LVDS_TX_CFG10, 0x0C, RG_LVDSA_BIAS_SEL); // for EPI BIAS
	 #ifdef CC_MT5399
	 vIO32WriteFldAlign(REG_LVDS_TX_CFG2, 0, RG_LVDSB_ENABIST_E_ODD_D11); // for mini LVDS BIAS 
   	 vIO32WriteFldAlign(REG_LVDS_TX_CFG2, 0, RG_LVDSB_ENABIST_E_EVEN_D11); // for mini LVDS BIAS
	 //vIO32WriteFldAlign(REG_LVDS_TX_CFG1, 1, RG_LVDSB_BIAS_SEL);
	 #endif

	 #ifdef CC_MT5399
	 //Set CLK_IQ SEL 
	 vIO32WriteFldAlign(REG_LVDS_TX_CFG13, 0x008, RG_LVDSA_CLK_IQ_SEL);
	 //vIO32WriteFldAlign(REG_LVDS_TX_CFG11, 0, RG_LVDSA_CLK_IQ_SEL);  // for EPI
	 vIO32WriteFldAlign(REG_LVDS_TX_CFG14, 0xFFF, RG_LVDSA_LVDS_SEL);
	 #else
	 //Set CLK_IQ SEL 
	 vIO32WriteFldAlign(REG_LVDS_TX_CFG13, 0x0820, RG_LVDSA_CLK_IQ_SEL);
	 //vIO32WriteFldAlign(REG_LVDS_TX_CFG11, 0, RG_LVDSA_CLK_IQ_SEL);  // for EPI
	 vIO32WriteFldAlign(REG_LVDS_TX_CFG14, 0x3FFF, RG_LVDSA_LVDS_SEL);
	 #endif
	 
	 #ifdef CC_MT5399
	 vIO32WriteFldAlign(REG_LVDS_TX_CFG2, 0x00, RG_LVDSB_CLK_IQ_SEL_EVEN_D11); //set for output CLK
	 vIO32WriteFldAlign(REG_LVDS_TX_CFG2, 0x08, RG_LVDSB_CLK_IQ_SEL_ODD_D11);
	 #endif
	 
} 

UINT8 u1DrvMlvdsPostInit(void)
{
    Printf("[Mini-LVDS] vDrvMiniLVDS_PostInit \n");
	//mini LVDS FIFO Reset
    vIO32WriteFldMulti(MLVDS_CTRL_09, P_Fld(1, ASYNC_FIFO_RST)|P_Fld(1, RG_FIFO_SEL));
	vIO32WriteFldMulti(MLVDS_CTRL_09, P_Fld(0, ASYNC_FIFO_RST)|P_Fld(1, RG_FIFO_SEL));

	//mini LVDS TCLK Start
	vIO32WriteFldMulti(FIFO_CTRL_01, P_Fld(1, TCLK_FIFO_START));
	
	//Enable D/A Interface FIFO
	vIO32WriteFldMulti(TMGR5, P_Fld(1, TMGR5_RG_FIFO_EN));
	
	return 1;
}

void vDrvMiniLvdsOn(void)
{

	Printf("[Mini-LVDS] vDrvMiniLvdsOn \n");	
  
    //A Group Power on Sequence
	vIO32WriteFldMulti(REG_LVDS_TX_CFG11,P_Fld( 0, RG_LVDSA_BIAS_ODD_PWD)|P_Fld( 0, RG_LVDSA_BIAS_EVEN_PWD));
	vIO32WriteFldMulti(REG_LVDS_TX_CFG19,P_Fld( 0, RG_LVDSA_LDO_BIAS_ODD_PWD_D11)|P_Fld( 0, RG_LVDSA_LDO_BIAS_EVEN_PWD_D11));
	vIO32WriteFldMulti(REG_LVDS_TX_CFG19, P_Fld( 0, RG_LVDSA_LDO_ODD_PWD_D11)|P_Fld( 0, RG_LVDSA_LDO_EVEN_PWD_D11));
	vIO32WriteFldAlign(REG_LVDS_TX_CFG12, 0, RG_LVDSA_DRV_PWD);
	#ifdef CC_MT5399
	vIO32WriteFldAlign(REG_LVDS_TX_CFG11, 0xFFF, RG_LVDSA_EXT_EN_D11);
	#else
	vIO32WriteFldAlign(REG_LVDS_TX_CFG11, 0x3FFF, RG_LVDSA_EXT_EN_D11);
	#endif
	
    #ifdef CC_MT5399
    //B Group Power on Sequence
	vIO32WriteFldAlign(REG_LVDS_TX_CFG0, 1, RG_LVDSB_BIAS_EN_D11 );
    vIO32WriteFldMulti(REG_LVDS_TX_CFG1, P_Fld(0x7F, RG_LVDSB_DRV_EN_EVEN_D11)|P_Fld(0x7F,RG_LVDSB_DRV_EN_ODD_D11));
	#endif 
	
	#ifdef CC_MT5399
	//Enable B group
	vIO32WriteFldAlign(REG_LVDS_TX_CFG0, 0,RG_LVDSB_EN_D11);	
	HAL_Delay_us(LVDSA_EN_REST_DELAYTIME);
	vIO32WriteFldAlign(REG_LVDS_TX_CFG0, 1,RG_LVDSB_EN_D11);
	#endif
	//Enable A group
	vIO32WriteFldAlign(REG_LVDS_TX_CFG11, 0,RG_LVDSA_EN);	
	HAL_Delay_us(LVDSA_EN_REST_DELAYTIME);
	vIO32WriteFldAlign(REG_LVDS_TX_CFG11, 1,RG_LVDSA_EN);

	#ifdef CC_MTK_LOADER
       vDrvLVDS7To4FifoEnable();
	#else
       _Lvds7To4FifoReset = 2;
	#endif

	#ifdef CC_COPLAT_MT82
	vUtDelay2us(5);
	#else
	HAL_Delay_us(10);
#endif
}

void vDrvMiniLVDSOff(void)
{
	Printf("[Mini-LVDS] vDrvMiniLvdsOff \n");	
		
#ifdef CC_MT5399
	//Disable B group
	vIO32WriteFldAlign(REG_LVDS_TX_CFG0, 0,RG_LVDSB_EN_D11);
	//B Group Power Off Sequence
	vIO32WriteFldMulti(REG_LVDS_TX_CFG1, P_Fld(0, RG_LVDSB_DRV_EN_EVEN_D11)|P_Fld(0,RG_LVDSB_DRV_EN_ODD_D11));
    vIO32WriteFldAlign(REG_LVDS_TX_CFG0, 0, RG_LVDSB_BIAS_EN_D11);
#endif
	//Disable A group
	vIO32WriteFldAlign(REG_LVDS_TX_CFG11, 0,RG_LVDSA_EN);
	//A Group Power Off Sequence
	vIO32WriteFldAlign(REG_LVDS_TX_CFG11, 0, RG_LVDSA_EXT_EN_D11); //12bit & 14bit
#ifdef CC_MT5399
    vIO32WriteFldAlign(REG_LVDS_TX_CFG12, 0xFFF, RG_LVDSA_DRV_PWD); //12bit
#else
	vIO32WriteFldAlign(REG_LVDS_TX_CFG12, 0x3FFF, RG_LVDSA_DRV_PWD); //14bit
#endif
    vIO32WriteFldMulti(REG_LVDS_TX_CFG19, P_Fld( 1, RG_LVDSA_LDO_ODD_PWD_D11)|P_Fld( 1, RG_LVDSA_LDO_EVEN_PWD_D11));
    vIO32WriteFldMulti(REG_LVDS_TX_CFG19,P_Fld( 1, RG_LVDSA_LDO_BIAS_ODD_PWD_D11)|P_Fld( 1, RG_LVDSA_LDO_BIAS_EVEN_PWD_D11));
    vIO32WriteFldMulti(REG_LVDS_TX_CFG11,P_Fld( 1, RG_LVDSA_BIAS_ODD_PWD)|P_Fld( 1, RG_LVDSA_BIAS_EVEN_PWD));	
}

UINT8 vDrvMiniTgOnOff(VIDEO_TCON_MINI_POWER_SEQ_T MiniTgOn, UINT8 val)
{
	UINT8 u1Ret = 0;

	LOG(1,"%d On !!!\n", MiniTgOn);

	switch (MiniTgOn)
		{
			case VIDEO_TCON_MINI_0: vIO32WriteFldAlign(GR0, val, GR0_TCTG_OEN_TCON0);
				break;
			case VIDEO_TCON_MINI_1: vIO32WriteFldAlign(GR0, val, GR0_TCTG_OEN_TCON1);
				break;
			case VIDEO_TCON_MINI_2: vIO32WriteFldAlign(GR0, val, GR0_TCTG_OEN_TCON2);
				break;
			case VIDEO_TCON_MINI_3: vIO32WriteFldAlign(GR0, val, GR0_TCTG_OEN_TCON3);
				break;
			case VIDEO_TCON_MINI_4: vIO32WriteFldAlign(GR0, val, GR0_TCTG_OEN_TCON4);
				break;
			case VIDEO_TCON_MINI_5: vIO32WriteFldAlign(GR0, val, GR0_TCTG_OEN_TCON5);
				break;
			case VIDEO_TCON_MINI_6: vIO32WriteFldAlign(GR0, val, GR0_TCTG_OEN_TCON6);
				break;
            case VIDEO_TCON_MINI_7: vIO32WriteFldAlign(GR0, val, GR0_TCTG_OEN_TCON7);
				break;
			case VIDEO_TCON_MINI_8: vIO32WriteFldAlign(GR0, val, GR0_TCTG_OEN_TCON8);
				break;
			case VIDEO_TCON_MINI_9: vIO32WriteFldAlign(GR0, val, GR0_TCTG_OEN_TCON9);
				break;
			case VIDEO_TCON_MINI_A: vIO32WriteFldAlign(GR0, val, GR0_TCTG_OEN_TCONA);
				break;
			case VIDEO_TCON_MINI_B: vIO32WriteFldAlign(GR0, val, GR0_TCTG_OEN_TCONB);
				break;
			case VIDEO_TCON_MINI_C: vIO32WriteFldAlign(GR0, val, GR0_TCTG_OEN_TCONC);
				break;
			case VIDEO_TCON_MINI_D: vIO32WriteFldAlign(GR0, val, GR0_TCTG_OEN_TCOND);
				break;
		    case VIDEO_TCON_MINI_E: vIO32WriteFldAlign(GR0, val, GR0_TCTG_OEN_TCONE);
				break;
			case VIDEO_TCON_MINI_F: vIO32WriteFldAlign(GR0, val, GR0_TCTG_OEN_TCONF);
				break;
			case VIDEO_TCON_MINISIGNAL:
            	if (val)
                	vDrvMiniLvdsOn();
            	else
                	vDrvMiniLVDSOff();
            	break;
			case VIDEO_TCON_MINI_ALL:
				if (val)
					vIO32WriteFldAlign(GR0, 0xFFFF, GR0_TCTG_OEN);
				else
					vIO32WriteFldAlign(GR0, 0, GR0_TCTG_OEN);
				break;
			default:
				break;
		}
	
		if(IO32ReadFldAlign(GR0, GR0_TC_EN))
			u1Ret = 1;
		else
			u1Ret = 0;
		
		return u1Ret;	
}

#ifdef CONFIG_OPM
    BOOL _fgMLVDSInit = FALSE;
#else
    static BOOL _fgMLVDSInit = FALSE;
#endif
void vDrvMiniLVDSInit(void)
{
	if (IO32ReadFldAlign(DDDS_08, FLAG_MLVDS_INITIAL) == 1)
    {
        _fgMLVDSInit = TRUE;
    }

	if(!_fgMLVDSInit)
	{
		Printf("[Mini-LVDS] vDrvMiniLVDS_Init(START) \n");
		//MLVDS Digital initial
		u1DrvMlvdsPreInit(); 
		u1TconCustMlvdsInit();
		u1DrvMlvdsPostInit();
		//MLVDS Analog initial
    	vDrvMiniLVDS_AnalogInit();

		_fgMLVDSInit = TRUE;
		vIO32WriteFldAlign(DDDS_08, 1, FLAG_MLVDS_INITIAL);
	}
}

/**
 * @brief u1DrvTconMLDSVOPLLFreqSet

 * Set vopll for MiniLVDS

 * @param  UINT32 u4Freq, (Hz)
 * @param  MLVDS_PORT: Single(0), Dual(4), Four(8)
 * @param  MLVDS_RATION: 1(0), 1P5(1), 2(2), 3(3), 4(4)
 * @retval void
 */
void u1DrvTconMLDSVOPLLFreqSet(UINT32 u4Freq, UINT8 MLVDS_PORT,UINT8 MLVDS_RATION)
{
 vDrvMLDSVOPLLFreqSet(u4Freq,MLVDS_PORT,MLVDS_RATION);
}

//Tg Initialize
static void vDrvTgPreInit(void)
{
	Printf("[TCON] vDrvTgPreInit \n");
	
    if(PANEL_GetDisplayType()== PANEL_DISPLAY_TYPE_EPI)
    {
        vIO32WriteFldMulti(GR0, P_Fld(1, GR0_TC_EN)|P_Fld(0x100, GR0_TCTG_FCR)|P_Fld(0, GR0_TCTG_OEN));
    }
    else
    {
		vIO32WriteFldMulti(GR0, P_Fld(1, GR0_TC_EN)|P_Fld(0x002, GR0_TCTG_FCR)|P_Fld(0, GR0_TCTG_OEN));
    }
	vIO32WriteFldMulti(GR1, P_Fld(1, GR1_TCTG_HSINV)|P_Fld(1, GR1_TCTG_VSINV));
	vIO32WriteFldMulti(GR7, P_Fld(0, GR7_TCO_MUTE)|P_Fld(0, GR7_TCO_DEFAULT));
	vIO32WriteFldMulti(GR8, P_Fld(0, GR8_TCGPO)|P_Fld(0, GR8_TCGPOEN));
}


#ifdef CONFIG_OPM
    BOOL _fgTCONInit = FALSE;
#else
    static BOOL _fgTCONInit = FALSE;
#endif
void vDrvTgInit(void)
{
    if (IO32ReadFldAlign(DDDS_08, FLAG_TCON_INITIAL) == 1)
    {
        _fgTCONInit = TRUE;
    }

	if (!_fgTCONInit)
	{
		Printf("[TCON] vDrvTgInit(START) \n");
		vDrvTgPreInit();

		if(_fgTconCustSel == FALSE)
    	{
    		u1TconCustTgInit();
    	}
    	else
    	{
        	vDrvTCONCustSel(_u1TconCustSel);
    	}

		_fgTCONInit = TRUE;
		vIO32WriteFldAlign(DDDS_08, 1, FLAG_TCON_INITIAL);
	}
}

UINT16 wApiFlashPqGetPanelId(void)
{
    UINT16 wFashPanelid=PANEL_TOTAL_NUMBER;

	switch (GetCurrentPanelIndex())
    {                 
    	#ifdef PANEL_MLVDS_LGDV4FHD60
        case PANEL_MLVDS_LGDV4FHD60: 
            wFashPanelid = 0;
        break;
		#endif

		#ifdef PANEL_MLVDS_LGDV4FHD120
		case PANEL_MLVDS_LGDV4FHD120:             
            wFashPanelid = 1;
        break;
		#endif

		#ifdef PANEL_MLVDS_PANASONIC32WXGA1P6P
		case PANEL_MLVDS_PANASONIC32WXGA1P6P:             
            wFashPanelid = 2;
        break;
		#endif

		#ifdef PANEL_MLVDS_PANASONIC32WXGA2P3P
		case PANEL_MLVDS_PANASONIC32WXGA2P3P:             
            wFashPanelid = 3;
        break;
		#endif

		#ifdef PANEL_MLVDS_PANASONIC42FHD120
        case PANEL_MLVDS_PANASONIC42FHD120:             
            wFashPanelid = 4;
        break;
		#endif

		#ifdef PANEL_MLVDS_CMO2011FHD60
        case PANEL_MLVDS_CMO2011FHD60:             
            wFashPanelid = 5;
        break;
		#endif
		
		#ifdef PANEL_MLVDS_CMO2011FHD120
		case PANEL_MLVDS_CMO2011FHD120:             
            wFashPanelid = 6;
        break;
		#endif

		#ifdef PANEL_MLVDS_SS_MB7
        case PANEL_MLVDS_SS_MB7:             
            wFashPanelid = 7;
        break;
		#endif

		#ifdef PANEL_EPI_LGDV12GIPFHD60
        case PANEL_EPI_LGDV12GIPFHD60:             
            wFashPanelid = 8;
        break;
		#endif

		#ifdef PANEL_EPI_LGDV12GIPFHD120
        case PANEL_EPI_LGDV12GIPFHD120:             
            wFashPanelid = 9;
        break;
		#endif

		#ifdef PANEL_MLVDS_LGD32WXGAV9
		case PANEL_MLVDS_LGD32WXGAV9:
            wFashPanelid = 10;
		break;	
		#endif

        default:
          break;
    }    
	return wFashPanelid;
}

#ifndef CC_UBOOT
static void vFlashPanelTconTim(PANEL_FLASH_TIM_ATTRIBUTE_T Timx, UINT8 TimNum)
{


    vIO32WriteFldMulti(TIM0R0+(TimNum*0x20), P_Fld(Timx.u2FPTimr0He, TIM0R0_HE)|
        P_Fld(Timx.u2FPTimr0Hs, TIM0R0_HS));

    vIO32WriteFldMulti(TIM0R1+(TimNum*0x20), P_Fld(Timx.u1FPTimr1Hmrsel, TIM0R1_HMRSEL)|
        P_Fld(Timx.u1FPTimr1Hminv, TIM0R1_HMINV)|P_Fld(Timx.u1FPTimr1Hmnotgl, TIM0R1_HMNOTGL)|
        P_Fld(Timx.u1FPTimr1Hmlcnt, TIM0R1_HMLCNT)|P_Fld(Timx.u1FPTimr1Hmhcnt, TIM0R1_HMHCNT));

    vIO32WriteFldMulti(TIM0R2+(TimNum*0x20), P_Fld(Timx.u1FPTimr2Htcsel, TIM0R2_HTCSEL)|
        P_Fld(Timx.u2FPTimr2Htf, TIM0R2_HTF)|P_Fld(Timx.u2FPTimr2Htr, TIM0R2_HTR));

    vIO32WriteFldMulti(TIM0R3+(TimNum*0x20), P_Fld(Timx.u2FPTimr3Ve, TIM0R3_VE)|
        P_Fld(Timx.u2FPTimr3Vs, TIM0R3_VS));

    vIO32WriteFldMulti(TIM0R4+(TimNum*0x20), P_Fld(Timx.u1FPTimr4Vmrsel, TIM0R4_VMRSEL)|
        P_Fld(Timx.u1FPTimr4Vminv, TIM0R4_VMINV)|P_Fld(Timx.u1FPTimr4Vmnotgl, TIM0R4_VMNOTGL)|
        P_Fld(Timx.u1FPTimr4Vmlcnt, TIM0R4_VMLCNT)|P_Fld(Timx.u1FPTimr4Vmhcnt, TIM0R4_VMHCNT));

    vIO32WriteFldMulti(TIM0R5+(TimNum*0x20), P_Fld(Timx.u1FPTimr5Vtcsel, TIM0R5_VTCSEL)|
        P_Fld(Timx.u2FPTimr5Vtf, TIM0R5_VTF)|P_Fld(Timx.u2FPTimr5Vtr, TIM0R5_VTR));

    vIO32WriteFldMulti(TIM0R6+(TimNum*0x20), P_Fld(Timx.u1FPTimr6Tcinv, TIM0R6_TCINV)|
        P_Fld(Timx.u1FPTimr6LosEn, TIM0R6_LOS_EN)|P_Fld(Timx.u1FPTimr6PolEn, TIM0R6_POL_EN)|
        P_Fld(Timx.u1FPTimr6HContEn, TIM0R6_H_CONT_EN)|P_Fld(Timx.u1FPTimr6Tcopr, TIM0R6_TCOPR));

    vIO32WriteFldMulti(TIM0R7+(TimNum*0x20), P_Fld(Timx.u1FPTimr7Ften, TIM0R7_FTEN)|
        P_Fld(Timx.u1FPTimr7Fposttglen, TIM0R7_FPOSTTGLEN)|P_Fld(Timx.u1FPTimr7Fpretglen, TIM0R7_FPRETGLEN)|
        P_Fld(Timx.u2FPTimr7Ftf, TIM0R7_FTF)|P_Fld(Timx.u2FPTimr7Ftr, TIM0R7_FTR));

}

void vDrvFlashPanelUpdateParam(void)
{
UINT32 u4Data;

    //TCON
    //GR0
   vIO32WriteFldAlign(GR0, rFlashPanelTable.u2FPGr0TctgFcr, GR0_TCTG_FCR);

    //GR4
    vIO32WriteFldMulti(GR4, P_Fld(rFlashPanelTable.u1FPGr4TgMuxselB15, GR4_TG_MUXSEL_B15)|
        P_Fld(rFlashPanelTable.u1FPGr4TgMuxselB14, GR4_TG_MUXSEL_B14)|P_Fld(rFlashPanelTable.u1FPGr4TgMuxselB13, GR4_TG_MUXSEL_B13)|
        P_Fld(rFlashPanelTable.u1FPGr4TgMuxselB12, GR4_TG_MUXSEL_B12)|P_Fld(rFlashPanelTable.u1FPGr4TgMuxselB11, GR4_TG_MUXSEL_B11)|
        P_Fld(rFlashPanelTable.u1FPGr4TgMuxselB10, GR4_TG_MUXSEL_B10)|P_Fld(rFlashPanelTable.u1FPGr4TgMuxselB9, GR4_TG_MUXSEL_B9)|
        P_Fld(rFlashPanelTable.u1FPGr4TgMuxselB8, GR4_TG_MUXSEL_B8));

    //GR5
    vIO32WriteFldMulti(GR5, P_Fld(rFlashPanelTable.u1FPGr5TgMuxselB7, GR5_TG_MUXSEL_B7)|
        P_Fld(rFlashPanelTable.u1FPGr5TgMuxselB6, GR5_TG_MUXSEL_B6)|P_Fld(rFlashPanelTable.u1FPGr5TgMuxselB5, GR5_TG_MUXSEL_B5)|
        P_Fld(rFlashPanelTable.u1FPGr5TgMuxselB4, GR5_TG_MUXSEL_B4)|P_Fld(rFlashPanelTable.u1FPGr5TgMuxselB3, GR5_TG_MUXSEL_B3)|
        P_Fld(rFlashPanelTable.u1FPGr5TgMuxselB2, GR5_TG_MUXSEL_B2)|P_Fld(rFlashPanelTable.u1FPGr5TgMuxselB1, GR5_TG_MUXSEL_B1)|
        P_Fld(rFlashPanelTable.u1FPGr5TgMuxselB0, GR5_TG_MUXSEL_B0));

    //GR6
    vIO32WriteFldMulti(GR6, P_Fld(rFlashPanelTable.u1FPGr6TgMode15, GR6_TG_MODE15)|
        P_Fld(rFlashPanelTable.u1FPGr6TgMode14, GR6_TG_MODE14)|P_Fld(rFlashPanelTable.u1FPGr6TgMode13, GR6_TG_MODE13)|
        P_Fld(rFlashPanelTable.u1FPGr6TgMode12, GR6_TG_MODE12)|P_Fld(rFlashPanelTable.u1FPGr6TgMode11, GR6_TG_MODE11)|
        P_Fld(rFlashPanelTable.u1FPGr6TgMode10, GR6_TG_MODE10)|P_Fld(rFlashPanelTable.u1FPGr6TgMode9, GR6_TG_MODE9)|
        P_Fld(rFlashPanelTable.u1FPGr6TgMode8, GR6_TG_MODE8)|P_Fld(rFlashPanelTable.u1FPGr6TgMode7, GR6_TG_MODE7)|
        P_Fld(rFlashPanelTable.u1FPGr6TgMode6, GR6_TG_MODE6)|P_Fld(rFlashPanelTable.u1FPGr6TgMode5, GR6_TG_MODE5)|
        P_Fld(rFlashPanelTable.u1FPGr6TgMode4, GR6_TG_MODE4)|P_Fld(rFlashPanelTable.u1FPGr6TgMode3, GR6_TG_MODE3)|
        P_Fld(rFlashPanelTable.u1FPGr6TgMode2, GR6_TG_MODE2)|P_Fld(rFlashPanelTable.u1FPGr6TgMode1, GR6_TG_MODE1)|
        P_Fld(rFlashPanelTable.u1FPGr6TgMode0, GR6_TG_MODE0));

    //TCSEL0
    vIO32WriteFldMulti(TCSEL0, P_Fld(rFlashPanelTable.u1FPTcsel0TgOutsel15, TCSEL0_TG_OUTSEL15)|
        P_Fld(rFlashPanelTable.u1FPTcsel0TgOutsel14, TCSEL0_TG_OUTSEL14)|P_Fld(rFlashPanelTable.u1FPTcsel0TgOutsel13, TCSEL0_TG_OUTSEL13)|
        P_Fld(rFlashPanelTable.u1FPTcsel0TgOutsel12, TCSEL0_TG_OUTSEL12)|P_Fld(rFlashPanelTable.u1FPTcsel0TgOutsel11, TCSEL0_TG_OUTSEL11)|
        P_Fld(rFlashPanelTable.u1FPTcsel0TgOutsel10, TCSEL0_TG_OUTSEL10)|P_Fld(rFlashPanelTable.u1FPTcsel0TgOutsel9, TCSEL0_TG_OUTSEL9)|
        P_Fld(rFlashPanelTable.u1FPTcsel0TgOutsel8, TCSEL0_TG_OUTSEL8));

    //TCSEL1
    vIO32WriteFldMulti(TCSEL1, P_Fld(rFlashPanelTable.u1FPTcsel1TgOutsel7, TCSEL1_TG_OUTSEL7)|
        P_Fld(rFlashPanelTable.u1FPTcsel1TgOutsel6, TCSEL1_TG_OUTSEL6)|P_Fld(rFlashPanelTable.u1FPTcsel1TgOutsel5, TCSEL1_TG_OUTSEL5)|
        P_Fld(rFlashPanelTable.u1FPTcsel1TgOutsel4, TCSEL1_TG_OUTSEL4)|P_Fld(rFlashPanelTable.u1FPTcsel1TgOutsel3, TCSEL1_TG_OUTSEL3)|
        P_Fld(rFlashPanelTable.u1FPTcsel1TgOutsel2, TCSEL1_TG_OUTSEL2)|P_Fld(rFlashPanelTable.u1FPTcsel1TgOutsel1, TCSEL1_TG_OUTSEL1)|
        P_Fld(rFlashPanelTable.u1FPTcsel1TgOutsel0, TCSEL1_TG_OUTSEL0));

    vFlashPanelTconTim(rFlashPanelTable.rFPTim0, 0);
    vFlashPanelTconTim(rFlashPanelTable.rFPTim1, 1);
    vFlashPanelTconTim(rFlashPanelTable.rFPTim2, 2);
    vFlashPanelTconTim(rFlashPanelTable.rFPTim3, 3);
    vFlashPanelTconTim(rFlashPanelTable.rFPTim4, 4);
    vFlashPanelTconTim(rFlashPanelTable.rFPTim5, 5);
    vFlashPanelTconTim(rFlashPanelTable.rFPTim6, 6);
    vFlashPanelTconTim(rFlashPanelTable.rFPTim7, 7);
    vFlashPanelTconTim(rFlashPanelTable.rFPTim8, 8);
    vFlashPanelTconTim(rFlashPanelTable.rFPTim9, 9);
    vFlashPanelTconTim(rFlashPanelTable.rFPTimA, 10);
    vFlashPanelTconTim(rFlashPanelTable.rFPTimB, 11);

    //MLVDS
    
    //MLVDS_CTRL_01
    vIO32WriteFldMulti(MLVDS_CTRL_01, P_Fld(rFlashPanelTable.u1FPRgMlvdsRst0, RG_MLVDS_RST0)|
        P_Fld(rFlashPanelTable.u1FPRgMlvdsRst1, RG_MLVDS_RST1)|P_Fld(rFlashPanelTable.u1FPRgMlvdsRst2, RG_MLVDS_RST2)|
        P_Fld(rFlashPanelTable.u1FPRgMlvdsRst3, RG_MLVDS_RST3));

    //MLVDS_T7
    vIO32WriteFldMulti(MLVDS_CTRL_02, P_Fld(rFlashPanelTable.u1FPRgMlvdsRst4, RG_MLVDS_RST4)|
        P_Fld(rFlashPanelTable.u1FPRgMlvdsRst5, RG_MLVDS_RST5)|P_Fld(rFlashPanelTable.u1FPRgMlvdsRst6, RG_MLVDS_RST6)|
        P_Fld(rFlashPanelTable.u1FPRgMlvdsRst7, RG_MLVDS_RST7));

    //MLVDS_T8
    vIO32WriteFldMulti(MLVDS_CTRL_03, P_Fld(rFlashPanelTable.u1FPRgMlvdsRst8, RG_MLVDS_RST8)|
        P_Fld(rFlashPanelTable.u1FPRgMlvdsRst9, RG_MLVDS_RST9));

    //EPI
    //EPI_B0
    vIO32WriteFldAlign(EPI_B0, rFlashPanelTable.u1FPRgPnSel, RG_PN_SEL);

    //EPI_B1
    vIO32WriteFldMulti(EPI_B1, P_Fld(rFlashPanelTable.u1FPRgSwapOut5Sel, RG_SWAP_OUT5_SEL)|
        P_Fld(rFlashPanelTable.u1FPRgSwapOut4Sel, RG_SWAP_OUT4_SEL)|P_Fld(rFlashPanelTable.u1FPRgSwapOut3Sel, RG_SWAP_OUT3_SEL)|
        P_Fld(rFlashPanelTable.u1FPRgSwapOut2Sel, RG_SWAP_OUT2_SEL)|P_Fld(rFlashPanelTable.u1FPRgSwapOut1Sel, RG_SWAP_OUT1_SEL)|
        P_Fld(rFlashPanelTable.u1FPRgSwapOut0Sel, RG_SWAP_OUT0_SEL));

    //EPI_B7
    vIO32WriteFldMulti(EPI_B7, P_Fld(rFlashPanelTable.u1FPRgCtrl2Self5, RG_CTRL2_SELF_5)|
        P_Fld(rFlashPanelTable.u1FPRgCtrl2Self4, RG_CTRL2_SELF_4)|P_Fld(rFlashPanelTable.u1FPRgCtrl2Self3, RG_CTRL2_SELF_3)|
        P_Fld(rFlashPanelTable.u1FPRgCtrl2Self2, RG_CTRL2_SELF_2)|P_Fld(rFlashPanelTable.u1FPRgCtrl2Self1, RG_CTRL2_SELF_1)|
        P_Fld(rFlashPanelTable.u1FPRgCtrl2Self0, RG_CTRL2_SELF_0)|P_Fld(rFlashPanelTable.u1FPRgCtrl1Self5, RG_CTRL1_SELF_5)|
        P_Fld(rFlashPanelTable.u1FPRgCtrl1Self4, RG_CTRL1_SELF_4)|P_Fld(rFlashPanelTable.u1FPRgCtrl1Self3, RG_CTRL1_SELF_3)|
        P_Fld(rFlashPanelTable.u1FPRgCtrl1Self2, RG_CTRL1_SELF_2)|P_Fld(rFlashPanelTable.u1FPRgCtrl1Self1, RG_CTRL1_SELF_1)|
        P_Fld(rFlashPanelTable.u1FPRgCtrl1Self0, RG_CTRL1_SELF_0));

    //EPI_T0
    vIO32WriteFldAlign(EPI_T0, rFlashPanelTable.u2FPRgCtr2RgcEn, RG_CTR_2_RGC_EN);
    vIO32WriteFldAlign(EPI_T14, rFlashPanelTable.u2FPRgCtr2RgcEn, RG_CTR_2_RGC_EN_0);
    vIO32WriteFldAlign(EPI_T15, rFlashPanelTable.u2FPRgCtr2RgcEn, RG_CTR_2_RGC_EN_1);
    vIO32WriteFldAlign(EPI_T16, rFlashPanelTable.u2FPRgCtr2RgcEn, RG_CTR_2_RGC_EN_2);
    vIO32WriteFldAlign(EPI_T17, rFlashPanelTable.u2FPRgCtr2RgcEn, RG_CTR_2_RGC_EN_3);
    vIO32WriteFldAlign(EPI_T18, rFlashPanelTable.u2FPRgCtr2RgcEn, RG_CTR_2_RGC_EN_4);
    vIO32WriteFldAlign(EPI_T19, rFlashPanelTable.u2FPRgCtr2RgcEn, RG_CTR_2_RGC_EN_0);

    //EPI_T1
    vIO32WriteFldMulti(EPI_T1, P_Fld(rFlashPanelTable.u4FPRgCtrStartDummy, RG_CTR_START_DUMMY)|
        P_Fld(rFlashPanelTable.u1FPRgCtrStartInd, RG_CTR_START_IND));

    //EPI_T2
    vIO32WriteFldMulti(EPI_T2, P_Fld(rFlashPanelTable.u2FPRgCtr1Dummy, RG_CTR_1_DUMMY)|
        P_Fld(rFlashPanelTable.u2FPRgCtr1SoeWidth, RG_CTR_1_SOE_WIDTH)|P_Fld(rFlashPanelTable.u1FPRgCtr1SoeStart, RG_CTR_1_SOE_START));

    //EPI_T3
    vIO32WriteFldMulti(EPI_T3, P_Fld(rFlashPanelTable.u2FPRgCtr2Rev2, RG_CTR_2_REV2)|
        P_Fld(rFlashPanelTable.u1FPRgCtr2Polc, RG_CTR_2_POLC)|P_Fld(rFlashPanelTable.u1FPRgCtr2GmaEn2, RG_CTR_2_GMAEN2)|
        P_Fld(rFlashPanelTable.u1FPRgCtr2GmaEn1, RG_CTR_2_GMAEN1)|P_Fld(rFlashPanelTable.u1FPRgCtr2Csc, RG_CTR_2_CSC)|
        P_Fld(rFlashPanelTable.u1FPRgCtr2Gsp, RG_CTR_2_GSP)|P_Fld(rFlashPanelTable.u1FPRgCtr2Rev1, RG_CTR_2_REV1)|
        P_Fld(rFlashPanelTable.u1FPRgCtr2Pwrc2, RG_CTR_2_PWRC2)|P_Fld(rFlashPanelTable.u1FPRgCtr2Pwrc1, RG_CTR_2_PWRC1)|
        P_Fld(rFlashPanelTable.u1FPRgCtr2Ltd2, RG_CTR_2_LTD2)|P_Fld(rFlashPanelTable.u1FPRgCtr2Ltd1, RG_CTR_2_LTD1)|
        P_Fld(rFlashPanelTable.u1FPRgCtr2H2dot, RG_CTR_2_H2DOT)|P_Fld(rFlashPanelTable.u1FPRgCtr2Mode, RG_CTR_2_MODE)|
        P_Fld(rFlashPanelTable.u1FPRgCtr2Pol, RG_CTR_2_POL));

    //EPI_T4
    vIO32WriteFldMulti(EPI_T4, P_Fld(rFlashPanelTable.u4FPRgDataStartDummy, RG_DATA_START_DUMMY)|
        P_Fld(rFlashPanelTable.u1FPRgDataStartInd, RG_DATA_START_IND));

    //EPI_T12
    vIO32WriteFldMulti(EPI_T12, P_Fld(rFlashPanelTable.u1FPRgTco7Sel, RG_TCO_7_SEL)|
        P_Fld(rFlashPanelTable.u1FPRgTco6Sel, RG_TCO_6_SEL)|P_Fld(rFlashPanelTable.u1FPRgTco5Sel, RG_TCO_5_SEL)|
        P_Fld(rFlashPanelTable.u1FPRgTco4Sel, RG_TCO_4_SEL)|P_Fld(rFlashPanelTable.u1FPRgTco3Sel, RG_TCO_3_SEL)|
        P_Fld(rFlashPanelTable.u1FPRgTco2Sel, RG_TCO_2_SEL)|P_Fld(rFlashPanelTable.u1FPRgTco1Sel, RG_TCO_1_SEL)|
        P_Fld(rFlashPanelTable.u1FPRgTco0Sel, RG_TCO_0_SEL));
    u4Data = u4IO32Read4B(EPI_T12);
    vIO32Write4B(EPI_T32,u4Data);
    vIO32Write4B(EPI_T34,u4Data);
    vIO32Write4B(EPI_T36,u4Data);
    vIO32Write4B(EPI_T38,u4Data);
    vIO32Write4B(EPI_T40,u4Data);
    vIO32Write4B(EPI_T42,u4Data);

    //EPI_T13
    vIO32WriteFldMulti(EPI_T13, P_Fld(rFlashPanelTable.u1FPRgTco15Sel, RG_TCO_15_SEL)|
        P_Fld(rFlashPanelTable.u1FPRgTco14Sel, RG_TCO_14_SEL)|P_Fld(rFlashPanelTable.u1FPRgTco13Sel, RG_TCO_13_SEL)|
        P_Fld(rFlashPanelTable.u1FPRgTco12Sel, RG_TCO_12_SEL)|P_Fld(rFlashPanelTable.u1FPRgTco11Sel, RG_TCO_11_SEL)|
        P_Fld(rFlashPanelTable.u1FPRgTco10Sel, RG_TCO_10_SEL)|P_Fld(rFlashPanelTable.u1FPRgTco9Sel, RG_TCO_9_SEL)|
        P_Fld(rFlashPanelTable.u1FPRgTco8Sel, RG_TCO_8_SEL));
    u4Data = u4IO32Read4B(EPI_T13);
    vIO32Write4B(EPI_T33,u4Data);
    vIO32Write4B(EPI_T35,u4Data);
    vIO32Write4B(EPI_T37,u4Data);
    vIO32Write4B(EPI_T39,u4Data);
    vIO32Write4B(EPI_T41,u4Data);
    vIO32Write4B(EPI_T43,u4Data);

    //EPI_T20
    vIO32WriteFldAlign(EPI_T20, rFlashPanelTable.u4FPRgCtrl10, RG_CTRL1_0);

    //EPI_T21
    vIO32WriteFldAlign(EPI_T21, rFlashPanelTable.u4FPRgCtrl11, RG_CTRL1_1);

    //EPI_T22
    vIO32WriteFldAlign(EPI_T22, rFlashPanelTable.u4FPRgCtrl12, RG_CTRL1_2);

    //EPI_T23
    vIO32WriteFldAlign(EPI_T23, rFlashPanelTable.u4FPRgCtrl13, RG_CTRL1_3);

    //EPI_T24
    vIO32WriteFldAlign(EPI_T24, rFlashPanelTable.u4FPRgCtrl14, RG_CTRL1_4);

    //EPI_T25
    vIO32WriteFldAlign(EPI_T25, rFlashPanelTable.u4FPRgCtrl15, RG_CTRL1_5);

    //EPI_T26
    vIO32WriteFldAlign(EPI_T26, rFlashPanelTable.u4FPRgCtrl20, RG_CTRL2_0);

    //EPI_T27
    vIO32WriteFldAlign(EPI_T27, rFlashPanelTable.u4FPRgCtrl21, RG_CTRL2_1);

    //EPI_T28
    vIO32WriteFldAlign(EPI_T28, rFlashPanelTable.u4FPRgCtrl22, RG_CTRL2_2);

    //EPI_T29
    vIO32WriteFldAlign(EPI_T29, rFlashPanelTable.u4FPRgCtrl23, RG_CTRL2_3);

    //EPI_T30
    vIO32WriteFldAlign(EPI_T30, rFlashPanelTable.u4FPRgCtrl24, RG_CTRL2_4);

    //EPI_T31
    vIO32WriteFldAlign(EPI_T31, rFlashPanelTable.u4FPRgCtrl25, RG_CTRL2_5);

    //Swing and Skew
    #ifdef CC_MT5399
    //LVDS
    vIO32WriteFldMulti(REG_LVDS_TX_CFG0, P_Fld(rFlashPanelTable.u1FPRgLVDSBTvo, RG_LVDSB_TVO_EVEN_D11)|
        P_Fld(rFlashPanelTable.u1FPRgLVDSBTvo, RG_LVDSB_TVO_EVEN_D11)|
        P_Fld(rFlashPanelTable.u1FPRgLVDSBTvcm, RG_LVDSB_TVCM_D11));
	#endif
    vIO32WriteFldMulti(REG_LVDS_TX_CFG11, P_Fld(rFlashPanelTable.u1FPRgLVDSATvo, RG_LVDSA_TVO)|
        P_Fld(rFlashPanelTable.u1FPRgLVDSATvcm, RG_LVDSA_TVCM));

    vIO32WriteFldAlign(REG_VOPLL_CFG1, rFlashPanelTable.u1FPRgVpllDlyData, RG_VPLL_DLY_DATA);
}
#endif

/**
 * @brief u1DrvTconInit

 * Init TCON

 * @param  void
 * @retval 0: fail, 1: success
 */

UINT8 u1DrvTconInit(void)
{
    UINT8 u1Ret = 0;

	_u1GammaReady = 0;

	// Error Handling
    vErrorHandleInit();        

    if(PANEL_GetDisplayType()== PANEL_DISPLAY_TYPE_EPI)
    {
        // EPI Init
		vDrvEPIInit();
    }
    else
    {
        // miniLVDS Init
        vDrvMiniLVDSInit();
    }

	// Tg Init
	vDrvTgInit();

    // Set TCON Pinmux and EPI LOCKN    
    _vDrvTconSetPinMux();

    // Flash TCON Init
    #ifdef CC_SUPPORT_FLASH_TCON
        vFlashTconInit();
    #endif

	#ifndef CC_UBOOT
    if (bApiFlashPqUpdatePanelTable() == SV_FALSE)
    {
        printf("Restore FlashPanel fail\n");
    }
    else	
    {
        printf("Restore FlashPanel success");
        vDrvFlashPanelUpdateParam();
    }         
	#endif
	
	// Enable TCON PMIC
    if(PANEL_GetDisplayType()!= PANEL_DISPLAY_TYPE_EPI)
    {
    	#ifndef __MLVDS_slt__
	   	 	vTconDPMCtrl(1);
    	#else
        	vTconDPMCtrl(0); //disable TCON power for SLT
    	#endif
    }

	// Set Gamma Voltage
    if (_u1GammaReady == 0)
        vDrvTconGammaInit();

    #if !defined(CC_FPGA) && !defined(CC_UBOOT)
        // tcon on
        vDrvTconOn();
    #endif

	u1Ret=1;	
	return(u1Ret);
}

/**
 * @brief u1DrvTcon_Enable

 * Enable TCON

 * @param  void
 * @retval 0: fail, 1: success
 */
void u1DrvTcon_Enable(BOOL fgOnOff)
{
	if (fgOnOff)
		vIO32WriteFldAlign(GR0, 0xFFFF, GR0_TCTG_OEN);
	else
		vIO32WriteFldAlign(GR0, 0, GR0_TCTG_OEN);

	if(PANEL_GetDisplayType()== PANEL_DISPLAY_TYPE_EPI)
	{
		if(fgOnOff)
		{
			vDrvEPI_AnalogOn();
		}
		else
		{
			vDrvEPI_AnalogOff();
		}
	}
	else
	{
		if(fgOnOff)
		{
			vDrvMiniLvdsOn();
		}
		else
		{
			vDrvMiniLVDSOff();
		}
	}
}

/**
 * @brief u1DrvTconIsReady

 * TCON Ready or not

 * @param  void
 * @retval 0: not ready, 1: ready
 */
UINT8 u1DrvTconIsReady(void)
{
	UINT8 u1Ret = 0;
	u1Ret = IO32ReadFldAlign(DDDS_08, FLAG_TCON_INITIAL) && (IO32ReadFldAlign(DDDS_08, FLAG_MLVDS_INITIAL)||IO32ReadFldAlign(DDDS_08, FLAG_EPI_INITIAL));
    return (u1Ret);
}

/**
 * @brief vDrvTconDump

 * TCON Dump

 * @param  void
 * @retval 0: fail, 1: success
 */
/*
UINT8 u1DrvTconRegDump(void)
{
    UINT8 u1Ret = 1;
	UINT8 i = 0;
	UINT8 u1count;
	UINT32 u4tmp;
	  
    // CKGEN

	// error handling

	// post

	// minilvds
  	LOG(1,"minilvds reg dump\n");
	u1count = (sizeof(sRegMini) / sizeof(struct sTCONREGTABLE));
    for ( i = 0 ; i < u1count ; i++)
    {
        u4tmp = sRegMini[i].u4Addr;
		u4TconRegRead(u4tmp);
    }
    LOG(1,"Tcon reg dump %d counts, %d / %d\n", i, sizeof(sRegMini),sizeof(struct sTCONREGTABLE));
	


	// tg
  	LOG(1,"tg reg dump\n");
	u1count = (sizeof(sRegTg) / sizeof(struct sTCONREGTABLE));
    for ( i = 0 ; i < u1count ; i++)
    {
        u4tmp = sRegTg[i].u4Addr;
		u4TconRegRead(u4tmp);
    }
    LOG(1,"Tcon reg dump %d counts, %d / %d\n", i, sizeof(sRegTg),sizeof(struct sTCONREGTABLE));

	return(u1Ret);
}
*/
/**
 * @brief u1DrvTconRegDiff

 * TCON register diff

 * @param  void
 * @retval 0: fail, 1: success
 */
/*
UINT8 u1DrvTconRegDiff(void)
{
    UINT8 u1Ret = 1;
	UINT8 i = 0;
	UINT8 u1count;
	UINT32 u4tmp, u4tmp1;
	
	// minilvds
  	LOG(1,"minilvds reg diff\n");
	u1count = (sizeof(sRegMini) / sizeof(struct sTCONREGTABLE));
    for ( i = 0 ; i < u1count ; i++)
    {
        u4tmp = sRegMini[i].u4Addr;
		u4tmp1 = u4TconRegRead(u4tmp);
		if (u4tmp1 != sRegMini[i].u4Val)
		{
            LOG(1,"!!! diff addr 0x%8x, 0x%8x --> 0x%8x\n", u4tmp, u4tmp1, sRegMini[i].u4Val);			
		}
    }
    LOG(1,"Tcon reg diff %d counts, %d / %d\n", i, sizeof(sRegMini),sizeof(struct sTCONREGTABLE));

	// tg
  	LOG(1,"tg reg diff\n");
	u1count = (sizeof(sRegTg) / sizeof(struct sTCONREGTABLE));
    for ( i = 0 ; i < u1count ; i++)
    {
        u4tmp = sRegTg[i].u4Addr;
		u4tmp1 = u4TconRegRead(u4tmp);
		if (u4tmp1 != sRegTg[i].u4Val)
		{
            LOG(1,"!!! diff addr 0x%8x, 0x%8x --> 0x%8x\n", u4tmp, u4tmp1, sRegTg[i].u4Val);			
		}
    }
    LOG(1,"Tcon reg diff %d counts, %d / %d\n", i, sizeof(sRegTg),sizeof(struct sTCONREGTABLE));

    return (u1Ret);
}
*/

#ifdef CC_SUPPORT_FLASH_TCON
INT32 i4FlashTconRead(UINT16 u2Offset, UINT8 *u1Data, UINT32 u4Length)
{
    UINT8 u1AQPartition;
    UINT32 u2AQSize;    
    INT32 i4Ret = 0;
    UINT64 u8Offset;
    
    UNUSED(u2AQSize);
    u1Data[0] = '\0';        
    u1AQPartition = (UINT8)DRVCUST_InitGet(eTconFlashTconPartition);
    if (u1AQPartition == 0xFF)
    {
        LOG(3,"AudioFlashAQPartition is not defined !!\n");
        return -1;
    }

#if 0    
    // check boundary 
    //u2AQSize = (UINT32)DRVCUST_OptGet(eAudioFlashAQSize);
    if (u2Offset > u2AQSize)
    {
        LOG(0,"Fail! AQ test read length larger than 0x%x\n", u2AQSize);
        return;
    }
#endif

#ifndef CC_NOR_DISABLE
     //   return ((INT32)NORSTG_Read(DRVCUST_OptGet(eTconFlashTconPartition), u2Offset, u1Data, u4Length));
            u8Offset = 0;
	     i4Ret = (INT32)DRVCUST_InitGet(eTconFlashTconPartition);
	    if (i4Ret > 1)
	    {
	        Printf("eTconFlashTconPartition %d", i4Ret);
	        u8Offset |= (((UINT64)(UINT32)i4Ret) << 32);	
	        i4Ret = NORPART_Read(u8Offset, (UINT32)u1Data, u4Length);
	        return i4Ret;
	    }   
#endif  //CC_NOR_DISABLE

    return (i4Ret);

}


void vFlashTconWrite(UINT16 u2Offset, UINT8 *u1Data, UINT32 u4size)
{
    UINT8 u1AQPartition;
    UINT32 u2AQSize;
    INT32 i4Ret;
    UINT64 u8Offset;
//    UINT32 u4Offset;
    
    UNUSED(u2AQSize);
    u1AQPartition = (UINT8)DRVCUST_InitGet(eTconFlashTconPartition);
    if (u1AQPartition == 0xFF)
    {
        LOG(3,"eTconFlashTconPartition is not defined !!\n");
        return;
    }    

#if 1
    // check boundary.
    //u2AQSize = (UINT32)DRVCUST_OptGet(eAudioFlashAQSize)
    if ((u2Offset + u4size) > FLASH_TCON_SIZE)
    {
        LOG(0,"TCON test write length larger than 0x%x\n", FLASH_TCON_SIZE);
        return;
    }
#endif    
//    u4Offset = ((UINT64)u1AQPartition << 32) | ((UINT64)u2Offset);
//    NANDPART_Write(u8Offset, (UINT32)u1Data, u4size);

#ifndef CC_NOR_DISABLE
      //  NORSTG_Write(DRVCUST_OptGet(eTconFlashTconPartition), u2Offset, u1Data, u4size);
            u8Offset = 0;
	    i4Ret = (INT32)DRVCUST_InitGet(eTconFlashTconPartition);
	    if (i4Ret > 1)
	    {
	        u8Offset |= (((UINT64)(UINT32)i4Ret) << 32);
			
		 i4Ret = NORPART_Erase(u8Offset, 1);
 	        i4Ret |= NORPART_Write(u8Offset, (UINT32)u1Data, u4size);
	    }      
#endif  //CC_NOR_DISABLE

}

void vFlashTconInit(void)
{
    UINT8 pau1Buf[FLASH_TCON_BUF_SIZE];
    UINT32 i;
    struct sTCONREGTABLE *pArray;    

    // check for header
    pau1Buf[0] = '\0';
    
    i4FlashTconRead(0, (UINT8*)&pau1Buf[0], FLASH_TCON_BUF_SIZE);
    
    if (x_strncmp((CHAR*)pau1Buf, "TCON", 4) !=0)
    {
        Printf("Flash TCON header is not correct, using default value\n");
        return;
    }

  
    Printf("\nFlash Tcon init reading...\n");
        
    for (i=0; i<FLASH_TCON_BUF_SIZE; i++)
    {
        if (i % 12 == 0) 
        {
        //    Printf("\n");
        }
        //Printf("0x%x ", pau1Buf[i]);
    }

   // Printf("\n");

    pArray = (struct sTCONREGTABLE *) &pau1Buf[0];

    if (pArray[0].u4Msk != GetCurrentPanelIndex())
    {
        Printf("Flash TCON panel ID is not matching, using default value\n");
        return;
    }

    for (i=0 ; i < 255 && i < pArray[0].u4Val ; i++)
    {
    //    Printf("tcon reg write %x %x %x \n", pArray[i].u4Addr, pArray[i].u4Val, pArray[i].u4Msk);
        vTconRegWrite(pArray[i].u4Addr,pArray[i].u4Val);
    }

     Printf("Flash Tcon init done...PID: %d, %d cmds\n", pArray[0].u4Msk, pArray[0].u4Val);
    
}

#define FLASH_TCON_HEADER_SZIE
void vFlashTconSave(void)
{
    UINT8 pau1Data[FLASH_TCON_BUF_SIZE] = {0};
    UINT32 i;
    struct sTCONREGTABLE *pArray;
    struct sTCONREGTABLE *pBufArray = NULL;
    UINT32 u4tmp = 0;
    UINT32 u4Length = 0;
    UINT32 u4TotalBytes = 0;    
    UINT32 u4CmdCnt = 0;
    UINT32 u4TotalCmdCnt = 0;    
    
    u4TotalBytes = 0;
    u4TotalCmdCnt = 0;
    // check for header
    pau1Data[0] = '\0';
    
    i4FlashTconRead(0, (UINT8*) &pau1Data[0], 12);
    
    if (x_strncmp((CHAR*)pau1Data, "TCON", 4) !=0)
    {
        LOG(0, "Flash TCON header is not correct, using default value\n");
        return;
    }

    // tg setting
    pArray = pTconCustTgGet(&u4CmdCnt);
    u4Length = sizeof(struct sTCONREGTABLE ) * u4CmdCnt;
    
    if (pArray == NULL)
    {
        LOG(0, "Flash TCON array not existed\n");
        return;
    }    
    
    if (u4CmdCnt ==0)
    {
        LOG(0, "Flash TCON data length is zero\n");
        return;
    }
   
    if (u4TotalCmdCnt + u4Length > FLASH_TCON_BUF_SIZE)
    {
        LOG(0, "Flash TCON data length over FLASH_TCON_BUF_SIZE %d\n", FLASH_TCON_BUF_SIZE);
        return;
    }
    
    pBufArray = (struct sTCONREGTABLE *) &pau1Data[0];
    
    for ( i = 0 ; i < u4CmdCnt ; i++)
    {
        u4tmp = pArray[i].u4Addr;
        pBufArray[i+1].u4Addr = u4tmp;
        pBufArray[i+1].u4Val = u4TconRegRead(u4tmp);
        pBufArray[i+1].u4Msk = pArray[i].u4Msk;
        Printf("Get reg %x %8x %x \n", pBufArray[i+1].u4Addr, pBufArray[i+1].u4Val, pBufArray[i+1].u4Msk);
    }
    
    u4TotalBytes += u4Length;
    u4TotalCmdCnt += u4CmdCnt;

    pBufArray[0].u4Val = 0x78796784;
    pBufArray[0].u4Val = u4TotalCmdCnt;
    pBufArray[0].u4Msk = GetCurrentPanelIndex();
    Printf("Set buf %x %8x %x \n", pBufArray[0].u4Addr, pBufArray[0].u4Val, pBufArray[0].u4Msk);

    Printf("save to flash\n");

    vFlashTconWrite(0, &pau1Data[0], u4TotalBytes);
    
    LOG(0,"Tcon reg save %d bytes\n", u4TotalBytes);
    
}

static UINT8 tcondata[12] = {0};
void vFlashTconWriteDefault(void)
{
    vFlashTconWrite(0, tcondata, sizeof(tcondata));
}
#endif  /*#ifdef CC_SUPPORT_FLASH_TCON*/

#define TCON_LOCK_HI_TH 100 //1.28V
#define TCON_LOCK_LO_TH 50 //0.64V

void vDrvTconOutVsyncISR(void)
{
#ifdef SUPPORT_LOCK_FROM_SRVAD
UINT32 u4Lock;
    
if((bbypass_isr == 0)&&(PANEL_GetDisplayType()== PANEL_DISPLAY_TYPE_EPI))
{
    u4Lock = PDWNC_ReadServoADCChannelValue(SERVOADC_TCON_LOCK);
    if(u4Lock > TCON_LOCK_HI_TH) //LOCK is High
    {
        vIO32WriteFldAlign(TMGR18, 1, TMGR18_RG_LOCK_VALUE);
    }
    else if(u4Lock < TCON_LOCK_LO_TH) //Lock is Low
    {
        vIO32WriteFldAlign(TMGR18, 0, TMGR18_RG_LOCK_VALUE);
    }
}
#endif
}

/**
 * @brief vDrvTCONSel

 * TCON power control

 * @param  void
 * @retval 0: fail, 1: success
 */
void vDrvTCONSel(VIDEO_TCON_TABLE_T eTconTableSel)
{
    _fgTconCustSel = TRUE;
    _u1TconCustSel = eTconTableSel;
    vDrvTCONCustSel(eTconTableSel);
}

UINT8 vDrvTCONRXCRC(UINT8 Mode)
{
	#if defined(CC_MT5399) || defined(CC_MT5882)
    UINT8 u1CRC_Done=0;
    UINT32 u4counter=0;

	vIO32WriteFldAlign(MLVDS_CTRL_14, 1, RG_TOP_CRC_NEW);
	vIO32WriteFldAlign(MLVDS_CTRL_09, Mode, RG_FIFO_SEL); // 0: LVDS 1: mini-lvds 4:EPI
	
	//CRC Check Frame Range
	vIO32WriteFldAlign(MLVDS_CTRL_14, 0xA, RG_TOP_CRC_V_CNT);
	
	//CRC Check Lines Range
	vIO32WriteFldAlign(MLVDS_CTRL_15, 1, RG_TOP_CRC_H_START); 

  	vIO32WriteFldAlign(MLVDS_CTRL_15, PANEL_GetPanelHeight(), RG_TOP_CRC_H_END);

	vIO32WriteFldAlign(MLVDS_CTRL_16, 0x1FFF, RG_CRC_HLEN_NEW); 


	vIO32WriteFldAlign(MLVDS_CTRL_16, 0, RG_TOP_CRC_START);

	//Select L/R Channel to do CRC Check
	vIO32WriteFldAlign(MLVDS_CTRL_16, 1, RG_TOP_CRC_SEL_L); //Select L Channel

	//Clear CRC Check Result
	vIO32WriteFldAlign(MLVDS_CTRL_16, 1, RG_TOP_CRC_CLR); 
	vIO32WriteFldAlign(MLVDS_CTRL_16, 0, RG_TOP_CRC_CLR); 		
		
	//Start to do CRC Check
	vIO32WriteFldAlign(MLVDS_CTRL_16, 1, RG_TOP_CRC_START); 

	do
	{
		u4counter++;
		u1CRC_Done = IO32ReadFldAlign(MLVDS_CTRL_17,ST_TOP_CRC1_DONE)
				  && IO32ReadFldAlign(MLVDS_CTRL_18,ST_TOP_CRC2_DONE)
				  && IO32ReadFldAlign(MLVDS_CTRL_19,ST_TOP_CRC3_DONE)
				  && IO32ReadFldAlign(MLVDS_CTRL_20,ST_TOP_CRC4_DONE);
		
		if((IO32ReadFldAlign(MLVDS_CTRL_17, ST_TOP_CRC1_FAIL) == 1)
		 ||(IO32ReadFldAlign(MLVDS_CTRL_18, ST_TOP_CRC2_FAIL) == 1)
		 ||(IO32ReadFldAlign(MLVDS_CTRL_19, ST_TOP_CRC3_FAIL) == 1)
		 ||(IO32ReadFldAlign(MLVDS_CTRL_20, ST_TOP_CRC4_FAIL) == 1))
		{
			   Printf("L Channel RX_CRC Check Fail\n");
			return FALSE;
		}
		
	}while(u1CRC_Done == 0);
	
		  Printf("L Channel RX_CRC_Done = %d\n",u1CRC_Done);
		  Printf("L Channel RX_CRC OK, Loop Counter = %d\n",u4counter);
		  Printf("L Channel RX_CRC1 Value = %d\n",IO32ReadFldAlign(MLVDS_CTRL_17, ST_TOP_CRC1_VALUE));
		  Printf("L Channel RX_CRC2 Value = %d\n",IO32ReadFldAlign(MLVDS_CTRL_18, ST_TOP_CRC2_VALUE));
		  Printf("L Channel RX_CRC3 Value = %d\n",IO32ReadFldAlign(MLVDS_CTRL_19, ST_TOP_CRC3_VALUE));
		  Printf("L Channel RX_CRC4 Value = %d\n",IO32ReadFldAlign(MLVDS_CTRL_20, ST_TOP_CRC4_VALUE));
		  Printf("\n");

	#endif
	return TRUE;
}



UINT8 vDrvMLVDSCRC(void)
{
    UINT8 u1CRC_Done=0;
    UINT32 u4counter=0;
	UINT32 j,u4loopcounter=1;
	
	vIO32WriteFldAlign(MLVDS_CTRL_14, 0, RG_TOP_CRC_ALG_SEL);
	vIO32WriteFldAlign(MLVDS_CTRL_14, 1, RG_TOP_CRC_NEW);

	//CRC Check Frame Range
	vIO32WriteFldAlign(MLVDS_CTRL_14, 0xC8, RG_TOP_CRC_V_CNT);
	
	//CRC Check Lines Range
	vIO32WriteFldAlign(MLVDS_CTRL_15, 1, RG_TOP_CRC_H_START);
    vIO32WriteFldAlign(MLVDS_CTRL_15, PANEL_GetPanelHeight(), RG_TOP_CRC_H_END); 

    //Setting H_Active Length 
    vIO32WriteFldAlign(MLVDS_CTRL_16, 0xFFF, RG_CRC_HLEN_NEW);  

////////////////////////////L Channel CRC Check////////////////////////////////////////////
	for (j = 0 ; j < u4loopcounter ; j++)
	{
       u1CRC_Done=0;
       u4counter=0;

	vIO32WriteFldAlign(MLVDS_CTRL_16, 0, RG_TOP_CRC_START);

	//Select L/R Channel to do CRC Check
	vIO32WriteFldAlign(MLVDS_CTRL_16, 1, RG_TOP_CRC_SEL_L); //Select L Channel
		
	//Clear CRC Check Result
	vIO32WriteFldAlign(MLVDS_CTRL_16, 1, RG_TOP_CRC_CLR); 
	vIO32WriteFldAlign(MLVDS_CTRL_16, 0, RG_TOP_CRC_CLR); 		
		
	//Start to do CRC Check
	vIO32WriteFldAlign(MLVDS_CTRL_16, 1, RG_TOP_CRC_START); 

do
{
	u4counter++;
	u1CRC_Done = IO32ReadFldAlign(MLVDS_CTRL_17,ST_TOP_CRC1_DONE)
	          && IO32ReadFldAlign(MLVDS_CTRL_18,ST_TOP_CRC2_DONE)
	          && IO32ReadFldAlign(MLVDS_CTRL_19,ST_TOP_CRC3_DONE)
	          && IO32ReadFldAlign(MLVDS_CTRL_20,ST_TOP_CRC4_DONE);
	
	if((IO32ReadFldAlign(MLVDS_CTRL_17, ST_TOP_CRC1_FAIL) == 1)
	 ||(IO32ReadFldAlign(MLVDS_CTRL_18, ST_TOP_CRC2_FAIL) == 1)
	 ||(IO32ReadFldAlign(MLVDS_CTRL_19, ST_TOP_CRC3_FAIL) == 1)
	 ||(IO32ReadFldAlign(MLVDS_CTRL_20, ST_TOP_CRC4_FAIL) == 1))
	{
		     Printf("L Channel %d CRC Check Fail\n",j+1);
		return FALSE;
	}
	
	}while(u1CRC_Done == 0);

       Printf("L Channel %d CRC_Done = %d\n",j+1,u1CRC_Done);
       Printf("L Channel %d CRC OK, Loop Counter = %d\n",j+1,u4counter);
	   Printf("L Channel %d CRC1 Value = %d\n",j+1,IO32ReadFldAlign(MLVDS_CTRL_17, ST_TOP_CRC1_VALUE));
	   Printf("L Channel %d CRC2 Value = %d\n",j+1,IO32ReadFldAlign(MLVDS_CTRL_18, ST_TOP_CRC2_VALUE));
	   Printf("L Channel %d CRC3 Value = %d\n",j+1,IO32ReadFldAlign(MLVDS_CTRL_19, ST_TOP_CRC3_VALUE));
	   Printf("L Channel %d CRC4 Value = %d\n",j+1,IO32ReadFldAlign(MLVDS_CTRL_20, ST_TOP_CRC4_VALUE));
	   Printf("\n");
	}
////////////////////////////R Channel CRC Check////////////////////////////////////////////
#if defined(CC_MT5399) || defined(CC_MT5882)
	for (j = 0 ; j < u4loopcounter ; j++)
	{
    u1CRC_Done=0;
    u4counter=0;
	
	vIO32WriteFldAlign(MLVDS_CTRL_16, 0, RG_TOP_CRC_START); 

	//Select L/R Channel to do CRC Check
	vIO32WriteFldAlign(MLVDS_CTRL_16, 0, RG_TOP_CRC_SEL_L); //Select R Channel
		
	//Clear CRC Check Result
	vIO32WriteFldAlign(MLVDS_CTRL_16, 1, RG_TOP_CRC_CLR); 
	vIO32WriteFldAlign(MLVDS_CTRL_16, 0, RG_TOP_CRC_CLR); 		
		
	//Start to do CRC Check
	vIO32WriteFldAlign(MLVDS_CTRL_16, 1, RG_TOP_CRC_START); 

do
{
	u4counter++;
	u1CRC_Done = IO32ReadFldAlign(MLVDS_CTRL_17,ST_TOP_CRC1_DONE)
	          && IO32ReadFldAlign(MLVDS_CTRL_18,ST_TOP_CRC2_DONE)
	          && IO32ReadFldAlign(MLVDS_CTRL_19,ST_TOP_CRC3_DONE)
	          && IO32ReadFldAlign(MLVDS_CTRL_20,ST_TOP_CRC4_DONE);
	
	if((IO32ReadFldAlign(MLVDS_CTRL_17, ST_TOP_CRC1_FAIL) == 1)
	 ||(IO32ReadFldAlign(MLVDS_CTRL_18, ST_TOP_CRC2_FAIL) == 1)
	 ||(IO32ReadFldAlign(MLVDS_CTRL_19, ST_TOP_CRC3_FAIL) == 1)
	 ||(IO32ReadFldAlign(MLVDS_CTRL_20, ST_TOP_CRC4_FAIL) == 1))
	{
		   Printf("R Channel %d CRC Check Fail\n",j+1);
		return FALSE;
	}
	
	}while(u1CRC_Done == 0);

      Printf("R Channel %d CRC_Done = %d\n",j+1,u1CRC_Done);
      Printf("R Channel %d CRC OK, Loop Counter = %d\n",j+1,u4counter);
	  Printf("R Channel %d CRC1 Value = %d\n",j+1,IO32ReadFldAlign(MLVDS_CTRL_17, ST_TOP_CRC1_VALUE));
	  Printf("R Channel %d CRC2 Value = %d\n",j+1,IO32ReadFldAlign(MLVDS_CTRL_18, ST_TOP_CRC2_VALUE));
	  Printf("R Channel %d CRC3 Value = %d\n",j+1,IO32ReadFldAlign(MLVDS_CTRL_19, ST_TOP_CRC3_VALUE));
	  Printf("R Channel %d CRC4 Value = %d\n",j+1,IO32ReadFldAlign(MLVDS_CTRL_20, ST_TOP_CRC4_VALUE));
	  Printf("\n");
	}
#endif
	
return TRUE;
}

//TCON ISR 
void vDrvTCONTimingGenEn(UINT8 u1OnOff)
{
    if (!u1OnOff)
    {
	    vIO32WriteFldAlign(GR0, 0x0, GR0_TC_EN);
    }
    else
    {
	    vIO32WriteFldAlign(GR0, 0x1, GR0_TC_EN);
    }
}

void vDrvTCONTimingGenFrameCounter(UINT16 u2Frame)
{
	vIO32WriteFldAlign(GR0, u2Frame, GR0_TCTG_FCR);
}

void vDrvTCONTimingGenHsyncInvert(UINT8 u1OnOff)
{
	vIO32WriteFldAlign(GR1, u1OnOff, GR1_TCTG_HSINV);
}

void vDrvTCONTimingGenVsyncInvert(UINT8 u1OnOff)
{
	vIO32WriteFldAlign(GR1, u1OnOff, GR1_TCTG_VSINV);
}


void vDrvTCONISRSetLineCounter(UINT32 u4VFp, UINT32 u4VActive, UINT32 u4VBp)
{
	
    if (IS_PANEL_L12R12)
	{
	    vIO32WriteFldAlign(GR12, (((u4VActive/2)+u4VBp)+1), GR12_TCTG_VCNT_INT);
	}
	else
	{
		vIO32WriteFldAlign(GR12, (u4VBp +2), GR12_TCTG_VCNT_INT);
	}

}
void vDrvTCONISRSetFrameCounter(UINT16 u2INTRFrame)
{
    vIO32WriteFldAlign(GR12, u2INTRFrame, GR12_TCTG_FCNT_INT); 
}

void vDrvTCONIntrEn(void)
{
    vIO32WriteFldAlign(GR1, 0x30, GR1_TCON_INT_EN);  // [5:4]=2'b11
}

void vDrvTCONIntrDisable(void)
{
    vIO32WriteFldAlign(GR1, 0x0, GR1_TCON_INT_EN);  // [5:4]=2'b11
}

void vDrvTCONIntrClr(void)
{
    vIO32WriteFldAlign(GR1, 0x30, GR1_TCON_INT_CLR);  // [5:4]=2'b11
    vIO32WriteFldAlign(GR1, 0x00, GR1_TCON_INT_CLR);  // [5:4]=2'b00    
}

extern UINT8 fgApplyScanPWMSetData;
extern UINT8 u1ScanPwmCrossFlg;
extern UINT8 fgApplyPWMModeChange;
extern UINT8 fgScanPWMSetpControl;
extern void vResetPWMFreq(void);
//extern UINT32 u4Support_ScanPWM;
extern UINT16 u2PscGetPanelVCNTStatus(void);

void vDrvTCONISR(UINT16 u2Vector)
{	
	static UINT16 u2Cnt = 0;
	static UINT8 u1PrevFcnt = 4;
	UINT8 u1ScanPWMApply = 2, u1CurrentFcnt = 0;
		
    if(u2Vector == VECTOR_TCON_TCH)  
    {
    	u1CurrentFcnt = u1DrvGetVsyncISRFCNTStatus();
	    vDrvTCONIntrClr();
		if(u1CurrentFcnt != u1PrevFcnt)
		{
			u1ScanPWMApply = 0;
		}
		else
		{
			if ((fgApplyScanPWMSetData) && (u1ScanPwmCrossFlg == 0))
			{
				u2Cnt = u2PscGetPanelVCNTStatus();
				if (u2Cnt >= u2DrvGetCrozzUpLimit())
				{
					u1ScanPWMApply = 0;			
				}
				else
				{
					u1ScanPWMApply = 1;			
					vDrvSetScanPWMSW_SETDATA();
				    vDrvSetScanPWMLatchMode(SCAN_PWM_LATCH_MODE_SETDATA_OFF,SCAN_PWM_LATCH_MODE_OFF,SCAN_PWM_ALIGN_MODE_VSYNC);  // Aligned Vsync
        			vDrvScanPWMDataFire();
				}
			}
		}

		if (u1ScanPWMApply == 1)
		{
			if (fgScanPWMSetpControl)
			{		
				if(u1DrvGetScanPWMStruct_Ready())
				{
			  		fgApplyScanPWMSetData = 0;								 //clear scanpwm setting flag
				}
				else
				{
			  	vDrvSetScanPWMSW_StepControl();		
				} 
			}
			else
			{
					fgApplyScanPWMSetData = 0;								   //clear scanpwm setting flag
			}
		}
		u1PrevFcnt = u1CurrentFcnt;
    }
    VERIFY(BIM_ClearIrq(VECTOR_TCON_TCH));
}

/** Brief
 * TCON ISR initial
 * @return void
 */
void vDrvTCONInitISR(void)
{
#ifndef CC_MTK_LOADER 
	static BOOL u1TCONISRInited = SV_FALSE;
	
	x_os_isr_fct pfnOldIsr=NULL;
	if(PANEL_GetDisplayType()!= PANEL_DISPLAY_TYPE_EPI)
	{		
		if(PANEL_GetDisplayType()== PANEL_DISPLAY_TYPE_VB1)
		{
	   	vIO32WriteFldAlign(CKGEN_TCLK_TEST, 0, FLD_TCLK_SEL);	// xtal to tclk for tco isr.
		}
		if (!u1TCONISRInited)
		{
			if (x_reg_isr(VECTOR_TCON_TCH, vDrvTCONISR, &pfnOldIsr) != OSR_OK)
			{
				LOG(0, "[TCON] Error: Fail to register TCON ISR!\n");
			}
			else
			{
				LOG(0, "[TCON] Success to register TCON ISR!\n");
			}
			if(!IO32ReadFldAlign(TMGR0,CKEN_CFG_TCON))
			{
				vIO32WriteFldAlign(TMGR0, 1, CKEN_CFG_TCON);
			}
	        vDrvTCONTimingGenEn(SV_ON);
	        vDrvTCONTimingGenFrameCounter(0x02);
	        vDrvTCONISRSetFrameCounter(0x01);
	#ifdef SUPPORT_PANEL_CUSTOMER_SPEC
		if (HSYNC_POLARITY == HSYNC_HIGH)
			vDrvTCONTimingGenHsyncInvert(SV_OFF);
		else
	        vDrvTCONTimingGenHsyncInvert(SV_ON);
		
		if (VSYNC_POLARITY == VSYNC_HIGH)
			vDrvTCONTimingGenVsyncInvert(SV_OFF);
		else
	        vDrvTCONTimingGenVsyncInvert(SV_ON);
	#else
		vDrvTCONTimingGenHsyncInvert(SV_ON);
		vDrvTCONTimingGenVsyncInvert(SV_ON);
	#endif
			vDrvTCONIntrEn();
		
			u1TCONISRInited = SV_TRUE;
		}
	}
#endif	
}
