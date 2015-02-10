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
 * $Author: p4admin $
 * $Date: 2015/02/10 $
 * $RCSfile: setpll.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

#include "x_typedef.h"
#include "x_ckgen.h"
#include "x_timer.h"
#include "sv_const.h"

#include "hw_vdoin.h"
#include "hw_ckgen.h"
#include "hw_ddds.h"
#include "hw_tg.h"
#ifdef CC_MT5882
#include "hw_hpcpll.h"
#include "hw_ifpga.h"
#include "hw_ifpll.h"
#endif

#ifndef CC_MTK_LOADER
#include "x_util.h"
#include "feature.h"
#include "drv_display.h"
#else /* CC_MTK_LOADER */
#define u8Div6432(a, b, c)  ((a) / (b))
#endif /* CC_MTK_LOADER */

#include "x_bim.h"
#include "hw_lvds.h"
#include "x_pdwnc.h"
#include "drv_vdoclk.h"
#include "drv_tcon.h"
#include "drv_lvds.h"
#include "drv_vbyone.h"
#include "ostg_if.h"
#include "panel.h"

//=====================================================================
// Configuration

#define VPLL_SPREAD_CW              (0x1fffffff)
#define VPLL_SPREAD_INT_SHIFT       (24)
#ifndef NDEBUG
#define VOPLL_Debug
#define VOPLL_Para_Chk 0
#endif
#define VOPLL_STRESS_TEST 0

#define ENABLE_DDDS2_FEEDBACK 1
#define SUPPORT_EPI


#define syspll_d2_ck 0x2
#define AD_DDDS1_VSP 0x14    
#define AD_DDDS2_VSP 0x15    
#define AD_VPLL_TCON_CK 0x16    
#define AD_LVDS_DPIX_CK 0x17    
#define AD_PPLL_DPIX_CK 0x18    
#define AD_CLKDIG_CTS_D11 0x19 
#define DBGCKSEL_tclk 0x1a 
#ifdef CC_MT5882
#define DBGCKSEL_tclk_div 0x55
#else
#define DBGCKSEL_tclk_div 0x5C
#endif
#define DBGCKSEL_oclk 0x1b 
#ifdef CC_MT5882
#define DBGCKSEL_oclk_div2 0x56
#define DBGCKSEL_oclk_div 0x57
#else
#define DBGCKSEL_oclk_div2 0x5d
#define DBGCKSEL_oclk_div 0x5e
#endif
#define DBGCKSEL_lvds_clk_cts 0x1c 
#ifdef CC_MT5882
#define DBGCKSEL_mon_clk75m_ck 0x58 
#else
#define DBGCKSEL_mon_clk75m_ck 0x5f 
#endif

#define VOPLLSetRecoverCnt 12

enum 
{
  LVDS_MODE,
  MLVDS_MODE,
  VB1_MODE,
  EPI_MODE,
  iDP_MODE
};

enum 
{
  VCO32PHASE_594M,
  VCO32PHASE_297M,
  VCO32PHASE_148P5M,
  VCO32PHASE_222P8M,
  VCO32PHASE_445P5M  
};


#ifdef CONFIG_OPM
BOOL fgVOPLLInit = FALSE;
#else
static BOOL fgVOPLLInit = FALSE;
#endif
BOOL  _fgVOPLLPowerOn = FALSE;
static BOOL _fgVopllUseDDDS = TRUE;
UINT8 _Lvds7To4FifoReset = 0;
UINT8 _u1TconMode = 0;
static UINT8 _u1VCO32PHASE_FREQ = 0;
static UINT32 _u4SetVopllFreq = 148500000;
static UINT8 u1VOPLLType = LVDS_MODE;
static UINT8 _u1DddsCWMul = 1;

static UINT8 MINI_LVDS_OUTPUT_PORT;
static UINT8 TCLK_PCLK_RATION;
static UINT8 TCLK_PCLK_RATION_NUM;
static UINT8 TCLK_PCLK_RATION_DEN;
static UINT8 TCON_TCLK_RATION;

#ifdef SUPPORT_CW_CHANGE_BY_STEP 
extern UINT32 _u4CurrentCW;
extern UINT32 _u4TargetCW;
#endif

/**
 * @brief set VOPLL clock
 *
 * Set video output PLL frequency
 * @param bCLK is the target VOPLL clock rate (MHz)
 * @retval void
 */
static void vDrvPIXPLLPathEnable(UINT8 u1Enable)
{

  if ((_u1TconMode!=LVDS_MODE) && (u1Enable))
  {
	  if(_u1TconMode==iDP_MODE)
	  {
		  vIO32WriteFldAlign(REG_VOPLL_CFG4, PPLL_REFSEL_IDP, RG_PPLL_REFSEL);
		  vIO32WriteFldMulti(REG_VOPLL_CFG2, P_Fld(0, RG_VPLL_FMEN)|P_Fld(0, RG_VPLL_FPEN));
	  }
	  else
	  {
		  vIO32WriteFldAlign(REG_VOPLL_CFG4, PPLL_REFSEL_VPLL, RG_PPLL_REFSEL);
		  vIO32WriteFldMulti(REG_VOPLL_CFG2, P_Fld(1, RG_VPLL_FMEN)|P_Fld(0, RG_VPLL_FPEN));
	  }
  } 
  else
  {
	  vIO32WriteFldAlign(REG_VOPLL_CFG4, PPLL_REFSEL_VPLL, RG_PPLL_REFSEL);
	  vIO32WriteFldMulti(REG_VOPLL_CFG2, P_Fld(0, RG_VPLL_FMEN)|P_Fld(1, RG_VPLL_FPEN));
  }
      
}


static void vDrvPIXPLLInit_iDP(void)
{
    vIO32WriteFldMulti(REG_VOPLL_CFG4, P_Fld(1, RG_PPLL_VODEN)|P_Fld(0, RG_PPLL_LF)|P_Fld(0, RG_PPLL_BP)|
                                       P_Fld(0, RG_PPLL_BR)|P_Fld(0, RG_PPLL_DIVEN));

    vIO32WriteFldMulti(REG_VOPLL_CFG3, P_Fld(0, RG_PPLL_PREDIV)|P_Fld(0, RG_PPLL_POSDIV)|P_Fld(0, RG_PPLL_FBKSEL)|
                                       P_Fld(7, RG_PPLL_FBKDIV)|P_Fld(1, RG_PPLL_POSDIV2));
}

static void vDrvPIXPLLInit_VbyOne(void)
{
    vIO32WriteFldMulti(REG_VOPLL_CFG4, P_Fld(2, RG_PPLL_REFSEL)|P_Fld(0, RG_PPLL_DIVEN)|P_Fld(0, RG_PPLL_BP)
                                      |P_Fld(0, RG_PPLL_BR)|P_Fld(0, RG_PPLL_LF)|P_Fld(1, RG_PPLL_VODEN));

    vIO32WriteFldMulti(REG_VOPLL_CFG3, P_Fld(0x0F, RG_PPLL_FBKDIV)|P_Fld(0, RG_PPLL_PREDIV)|P_Fld(1, RG_PPLL_FBKSEL)
		                              |P_Fld(1, RG_PPLL_POSDIV)|P_Fld(0, RG_PPLL_POSDIV2));
}

#ifdef SUPPORT_EPI
static void vDrvPIXPLLInit_EPI(void)
{
    vIO32WriteFldMulti(REG_VOPLL_CFG4, P_Fld(2, RG_PPLL_REFSEL)|P_Fld(0, RG_PPLL_DIVEN)|P_Fld(0, RG_PPLL_BP)
		                              |P_Fld(0, RG_PPLL_BR)|P_Fld(0, RG_PPLL_LF)|P_Fld(1, RG_PPLL_VODEN));
	
	if (IS_EPI_4LANE)
	{
	vIO32WriteFldMulti(REG_VOPLL_CFG3, P_Fld(0x0F, RG_PPLL_FBKDIV)|P_Fld(1, RG_PPLL_PREDIV)|P_Fld(1, RG_PPLL_FBKSEL)
									  |P_Fld(1, RG_PPLL_POSDIV)|P_Fld(0, RG_PPLL_POSDIV2));	
	}
	else if (IS_EPI_8LANE)
	{
	vIO32WriteFldMulti(REG_VOPLL_CFG3, P_Fld(0x0F, RG_PPLL_FBKDIV)|P_Fld(1, RG_PPLL_PREDIV)|P_Fld(1, RG_PPLL_FBKSEL)
									  |P_Fld(1, RG_PPLL_POSDIV)|P_Fld(0, RG_PPLL_POSDIV2));	
	}	
	else
	{
    vIO32WriteFldMulti(REG_VOPLL_CFG3, P_Fld(0x0B, RG_PPLL_FBKDIV)|P_Fld(1, RG_PPLL_PREDIV)|P_Fld(1, RG_PPLL_FBKSEL)
		                              |P_Fld(1, RG_PPLL_POSDIV)|P_Fld(0, RG_PPLL_POSDIV2));
	}
}
#endif


static void vDrvVOPLLInit_iDP(void)
{

}

#ifdef SUPPORT_EPI
static void vDrvVOPLLInit_EPI(void)
{

	vIO32WriteFldAlign(REG_VOPLL_CFG2, 1, RG_VPLL_MONEN);
	vIO32WriteFldAlign(REG_VOPLL_CFG2, 3, RG_VPLL_BIR);
	vIO32WriteFldAlign(REG_VOPLL_CFG1, 1, RG_VPLL_AUTOK_EN);
	vIO32WriteFldMulti(REG_VOPLL_CFG0, P_Fld(0, RG_VPLL_BG_VB1_TX_PWD)|P_Fld(0, RG_VPLL_BG_VB1_PRE_PWD)|P_Fld(0, RG_VPLL_BG_VB1_COMP_PWD));

	vIO32WriteFldMulti(REG_VOPLL_CFG2, P_Fld(2, RG_VPLL_DIVEN)|P_Fld(2, RG_VPLL_BC)|P_Fld(0x0B, RG_VPLL_BP));
	
	vIO32WriteFldAlign(REG_VOPLL_CFG4, 0, RG_PPLL_BP);
	vIO32WriteFldAlign(REG_VOPLL_CFG4, 1, RG_PPLL_MONEN);

	vIO32WriteFldAlign(REG_VOPLL_CFG4, 0, RG_PPLL_TSTEN);
	vIO32WriteFldAlign(REG_VOPLL_CFG4, 0, RG_PPLL_RESERVE);
	
}
#endif

static void vDrvVOPLLInit_VbyOne(void)
{
	//set d1a4 and d1b4
	UINT32 u4DispBit;
	u4DispBit = DISP_BIT;

	vIO32WriteFldAlign(REG_VOPLL_CFG2, 1, RG_VPLL_MONEN);
	vIO32WriteFldMulti(REG_VOPLL_CFG2, P_Fld(1, RG_VPLL_BIC)|P_Fld(3, RG_VPLL_BC)|P_Fld(0, RG_VPLL_FPEN)
		                              |P_Fld(1, RG_VPLL_FMEN));

	vIO32WriteFldAlign(REG_VOPLL_CFG1, 1, RG_VPLL_AUTOK_EN);	
	vIO32WriteFldMulti(REG_VOPLL_CFG0, P_Fld(0, RG_VPLL_BG_VB1_TX_PWD)|P_Fld(0, RG_VPLL_BG_VB1_PRE_PWD)|P_Fld(0, RG_VPLL_BG_VB1_COMP_PWD));
	vIO32WriteFldAlign(REG_VOPLL_CFG1, 0, RG_VPLL_LVDSCHL_EN);	

	vIO32WriteFldMulti(REG_VOPLL_CFG0, P_Fld(3, RG_VPLL_PREDIV)|P_Fld(0, RG_VPLL_POSDIV)|P_Fld(0, RG_VPLL_REFSEL));

	vIO32WriteFldMulti(REG_VOPLL_CFG1, P_Fld(0, RG_VPLL_LVDS_DPIX_DIV2)|P_Fld(0, RG_VPLL_TXCLK_DIV)|P_Fld(0, RG_VPLL_TXCLK_DIV2)
		                              |P_Fld(0, RG_VPLL_AUTOK_CTRL)|P_Fld(1, RG_VPLL_BAND)|P_Fld(1, RG_VPLL_AUTOK_LOAD)
		                              |P_Fld(1, RG_VPLL_VODEN));

	switch (u4DispBit)
	{
	    case DISP_36BIT:
			vIO32WriteFldMulti(REG_VOPLL_CFG2, P_Fld(0, RG_VPLL_DIVEN)|P_Fld(0x0E, RG_VPLL_BIR)
				                              |P_Fld(0x0C, RG_VPLL_BP)|P_Fld(4, RG_VPLL_BR)); 
		break;
		
		    case DISP_30BIT:
			vIO32WriteFldMulti(REG_VOPLL_CFG2, P_Fld(2, RG_VPLL_DIVEN)|P_Fld(2, RG_VPLL_BIR)|
			                                   P_Fld(0x0B, RG_VPLL_BP)|P_Fld(2, RG_VPLL_BR)); 
		break;
		
		case DISP_24BIT:
		case DISP_18BIT:
			vIO32WriteFldMulti(REG_VOPLL_CFG2, P_Fld(2, RG_VPLL_DIVEN)|P_Fld(2, RG_VPLL_BIR)|
			                                   P_Fld(0x09, RG_VPLL_BP)|P_Fld(2, RG_VPLL_BR)); 
		break;
		default:
		break;
	}
	vIO32WriteFldAlign(REG_VOPLL_CFG4, 0, RG_PPLL_TSTEN);
	vIO32WriteFldAlign(REG_VOPLL_CFG4, 0, RG_PPLL_RESERVE);
	
}

static void vDrvVOPLLInit_LVDS(void)
{
  //  PFD/CHP/LPF/BAND setting
	vIO32WriteFldMulti(REG_VOPLL_CFG0, P_Fld(0, RG_VPLL_BG_VB1_TX_PWD)|P_Fld(0, RG_VPLL_BG_VB1_PRE_PWD)|P_Fld(0, RG_VPLL_BG_VB1_COMP_PWD)
	                                	|P_Fld(0x0D, RG_VPLL_FBKDIV)|P_Fld(0, RG_VPLL_FBKSEL)|P_Fld(0, RG_VPLL_POSDIV)
	                                	|P_Fld(0, RG_VPLL_REFSEL));

	vIO32WriteFldMulti(REG_VOPLL_CFG1, P_Fld(1, RG_VPLL_LVDSCHL_EN)|P_Fld(3, RG_VPLL_AUTOK_CTRL)|P_Fld(1, RG_VPLL_BAND)
		                                |P_Fld(0, RG_VPLL_AUTOK_EN)|P_Fld(0, RG_VPLL_AUTOK_LOAD)|P_Fld(1, RG_VPLL_VODEN));		

	vIO32WriteFldMulti(REG_VOPLL_CFG2, P_Fld(1, RG_VPLL_BIC)|P_Fld(3, RG_VPLL_BC)|P_Fld(2, RG_VPLL_DIVEN)
	                                	|P_Fld(1, RG_VPLL_BIR)|P_Fld(9, RG_VPLL_BP)|P_Fld(4, RG_VPLL_BR)
	                                	|P_Fld(1, RG_VPLL_FPEN)|P_Fld(0, RG_VPLL_FMEN));

	vIO32WriteFldMulti(REG_VOPLL_CFG3, P_Fld(0, RG_VPLL_RESERVE)|P_Fld(0x0F, RG_PPLL_FBKDIV)|P_Fld(0, RG_PPLL_PREDIV)
		                                |P_Fld(0, RG_PPLL_FBKSEL)|P_Fld(1, RG_PPLL_POSDIV)|P_Fld(0, RG_PPLL_POSDIV2));
	                                    
	vIO32WriteFldMulti(REG_VOPLL_CFG4, P_Fld(2, RG_PPLL_REFSEL)|P_Fld(0, RG_PPLL_DIVEN)|P_Fld(0, RG_PPLL_BP)
		                                |P_Fld(0, RG_PPLL_BR)|P_Fld(0, RG_PPLL_LF)|P_Fld(1, RG_PPLL_VODEN)); 
}

static void vDrvVOPLLInit_MINI(void)
{
  if (_u1VCO32PHASE_FREQ == VCO32PHASE_594M)
  {

   vIO32WriteFldMulti(REG_VOPLL_CFG2, P_Fld(1, RG_VPLL_BIC)|P_Fld(3, RG_VPLL_BC)
   	                                 |P_Fld(2, RG_VPLL_DIVEN)|P_Fld(2, RG_VPLL_BIR)
                                     |P_Fld(0xB, RG_VPLL_BP)|P_Fld(4, RG_VPLL_BR));
                                        
   vIO32WriteFldMulti(REG_VOPLL_CFG4, P_Fld(0, RG_PPLL_BP)|P_Fld(0, RG_PPLL_BR)); 

  }
  else if (_u1VCO32PHASE_FREQ == VCO32PHASE_445P5M)
  {
   vIO32WriteFldMulti(REG_VOPLL_CFG2, P_Fld(1, RG_VPLL_BIC)|P_Fld(3, RG_VPLL_BC)
   	                                 |P_Fld(2, RG_VPLL_DIVEN)|P_Fld(2, RG_VPLL_BIR)
                                     |P_Fld(0x9, RG_VPLL_BP)|P_Fld(4, RG_VPLL_BR));
      
   vIO32WriteFldMulti(REG_VOPLL_CFG4, P_Fld(0, RG_PPLL_BP)|P_Fld(0, RG_PPLL_BR)); 
   
  }    
  else if (_u1VCO32PHASE_FREQ == VCO32PHASE_297M)
  {
   vIO32WriteFldMulti(REG_VOPLL_CFG2, P_Fld(1, RG_VPLL_BIC)|P_Fld(2, RG_VPLL_BC)
   	                                 |P_Fld(2, RG_VPLL_DIVEN)|P_Fld(2, RG_VPLL_BIR)
                                     |P_Fld(0x6, RG_VPLL_BP)|P_Fld(4, RG_VPLL_BR));
                                                
   vIO32WriteFldMulti(REG_VOPLL_CFG4, P_Fld(0, RG_PPLL_BP)|P_Fld(0, RG_PPLL_BR)); 

  }  
  else if (_u1VCO32PHASE_FREQ == VCO32PHASE_222P8M)
  {
   vIO32WriteFldMulti(REG_VOPLL_CFG2, P_Fld(1, RG_VPLL_BIC)|P_Fld(3, RG_VPLL_BC)
   	                                 |P_Fld(3, RG_VPLL_DIVEN)|P_Fld(1, RG_VPLL_BIR)
                                     |P_Fld(0x5, RG_VPLL_BP)|P_Fld(4, RG_VPLL_BR));
      
   vIO32WriteFldMulti(REG_VOPLL_CFG4, P_Fld(0, RG_PPLL_BP)|P_Fld(0, RG_PPLL_BR)); 

  }  
  else if (_u1VCO32PHASE_FREQ == VCO32PHASE_148P5M)
  {
   vIO32WriteFldMulti(REG_VOPLL_CFG2, P_Fld(1, RG_VPLL_BIC)|P_Fld(2, RG_VPLL_BC)
   	                                 |P_Fld(4, RG_VPLL_DIVEN)|P_Fld(1, RG_VPLL_BIR)
                                     |P_Fld(0x6, RG_VPLL_BP)|P_Fld(4, RG_VPLL_BR));
      
   vIO32WriteFldMulti(REG_VOPLL_CFG4, P_Fld(0, RG_PPLL_BP)|P_Fld(0, RG_PPLL_BR)); 

  }  

  vIO32WriteFldMulti(REG_VOPLL_CFG1, P_Fld(0x00,RG_VPLL_LVDSEN)|P_Fld(0x00,RG_VPLL_VB1EN)
                                    |P_Fld(0x01,RG_VPLL_MLVDSEN)|P_Fld(0x00,RG_VPLL_EPIEN)
			                        |P_Fld(0x01,RG_VPLL_IDPEN)|P_Fld(0x00,RG_VPLL_LVDSCHL_EN)	                        
			                        |P_Fld(0x00,RG_VPLL_LVDS_DPIX_DIV2)|P_Fld(0x00,RG_VPLL_TXCLK_DIV)
			                        |P_Fld(0x00,RG_VPLL_TXCLK_DIV2)|P_Fld(0x03,RG_VPLL_AUTOK_CTRL)
		                            |P_Fld(0x01,RG_VPLL_BAND)|P_Fld(0x00,RG_VPLL_AUTOK_EN) 
		                            |P_Fld(0x00,RG_VPLL_AUTOK_LOAD)|P_Fld(0x01,RG_VPLL_VODEN)); 

}

static void vDrvVOPLLInit(void)
{

    if(u1VOPLLType != _u1TconMode)
    {
        // re-init vopll if tcon mode changed.
        fgVOPLLInit = 0;
    }

    if ((IO32ReadFldAlign(DDDS_08, FLAG_VOPLL_INITIAL) == 1)&& (!(IS_PANEL_L12R12)))
    {
        fgVOPLLInit = TRUE;
    }
	
	#if VOPLL_STRESS_TEST
		fgVOPLLInit = FALSE;
	#endif
	
    if(!fgVOPLLInit)
    {       
       vIO32WriteFldAlign(CKGEN_VPCLK_CFG, 0, FLD_OCLK_SEL);
       vIO32WriteFldAlign(CKGEN_VPCLK_CFG, 0, FLD_TCLK_SEL);

       // VOPLL Power Control
       vIO32WriteFldAlign(REG_VOPLL_CFG0, 1, RG_VPLL_BG_LVDS_PWD);       
       vIO32WriteFldAlign(REG_VOPLL_CFG0, 1, RG_VPLL_PWD);      
       vIO32WriteFldAlign(REG_VOPLL_CFG0, 1, RG_VPLL_BG_PWD);       
       HAL_Delay_us(100);
       
       vIO32WriteFldAlign(REG_VOPLL_CFG0, 0, RG_VPLL_BG_PWD);
       HAL_Delay_us(1000);       


	   vIO32WriteFldMulti(REG_VOPLL_CFG0, P_Fld(0, RG_VPLL_BG_CKEN)|P_Fld(0, RG_VPLL_XTAL_DIV)|P_Fld(8, RG_VPLL_BG_TRIM)|P_Fld(2, RG_VPLL_BG_RASEL)|P_Fld(2, RG_VPLL_BG_RBSEL)
	   	|P_Fld(0, RG_VPLL_BG_PWD)|P_Fld(0, RG_VPLL_BG_VB1_COMP_PWD)|P_Fld(0, RG_VPLL_BG_VB1_PRE_PWD)|P_Fld(0, RG_VPLL_BG_VB1_TX_PWD)|P_Fld(0, RG_VPLL_BG_LVDS_PWD)|P_Fld(1, RG_VPLL_BIAS_BYP));


   	   if(_u1TconMode == VB1_MODE)
       {
   		 vDrvVOPLLInit_VbyOne();
   		 vDrvPIXPLLInit_VbyOne();
   		 printf("###VbyOne VOPLLInit  finsih###");
	   }
       else if(_u1TconMode == iDP_MODE)
       {
     	 vDrvVOPLLInit_iDP();
     	 vDrvPIXPLLInit_iDP();     	 
     	 printf("###iDP VOPLLInit  finsih###");
       }	
#ifdef SUPPORT_EPI
       else if(_u1TconMode == EPI_MODE)
       {
	     vDrvVOPLLInit_EPI();
    	 vDrvPIXPLLInit_EPI();
   	   }
#endif
	   else
	   {
        vDrvVOPLLInit_LVDS();
	   }
    
	   vDrvPIXPLLPathEnable(1);

       // Input clcok type : single-end & VCO Band seclect
       vIO32WriteFldAlign(REG_VOPLL_CFG0, 0, RG_VPLL_REFSEL);
       vIO32WriteFldAlign(REG_VOPLL_CFG1, 1, RG_VPLL_BAND);
    

#ifndef CC_UBOOT
       vDrvSetOCLKClockSchemaInit();
#endif
             
       vIO32WriteFldAlign(REG_PLL_GROUP_CFG8, 7, FLD_RG_DDDS1_PI_C);
       vIO32WriteFldAlign(REG_PLL_GROUP_CFG8, 7, FLD_RG_DDDS2_PI_C);

       fgVOPLLInit = TRUE;       
       u1VOPLLType = _u1TconMode;       

       vIO32WriteFldAlign(DDDS_08, 1, FLAG_VOPLL_INITIAL);
       
    }
	else
	{
#ifndef CC_UBOOT
      vDrvSetOCLKClockSchemaInit();	
#endif
	}   
}

void vDrvDPIX_CLK_SEL(void)
{
    if (_u1TconMode==MLVDS_MODE)
    {     
     vIO32WriteFldAlign(REG_VOPLL_CFG1, VPLL_POST_MINIEN, VPLL_POST_SEL);
    }
    else if (_u1TconMode==LVDS_MODE)
    {
     vIO32WriteFldAlign(REG_VOPLL_CFG1, VPLL_POST_LVDSEN, VPLL_POST_SEL);
    }
    else if (_u1TconMode==VB1_MODE)
    {
     vIO32WriteFldAlign(REG_VOPLL_CFG1, VPLL_POST_VB1EN, VPLL_POST_SEL);
    }
    #ifdef SUPPORT_EPI
    else if (_u1TconMode==EPI_MODE)
    {
//     vIO32WriteFldAlign(REG_VOPLL_CFG1, VPLL_POST_EPIEN, VPLL_POST_SEL);
    }    
    #endif
    else if (_u1TconMode==iDP_MODE)
    {
//     vIO32WriteFldAlign(REG_VOPLL_CFG1, VPLL_POST_IDPEN, VPLL_POST_SEL);     
    }       
}


#define DEFAULT_VCO_PHASE 8
static void vDrvVCOPhase_Init(void)
{
    vIO32WriteFldMulti(REG_VOPLL_CFG2,P_Fld(0x08,RG_VPLL_DLY_CLKA)|P_Fld(0x08,RG_VPLL_DLY_CLKB));
    vIO32WriteFldAlign(REG_VOPLL_CFG1, 0x09, RG_VPLL_DLY_DATA);
    
//    vIO32WriteFldMulti(REG_LVDS_TX_CFG11, P_Fld(1,RG_LVDSA_CLK_IQ_SEL_E3)|P_Fld(1,RG_LVDSA_CLK_IQ_SEL_O3));
//    vIO32WriteFldMulti(REG_LVDS_TX_CFG2, P_Fld(1,RG_LVDSB_CLK_IQ_SEL_E3)|P_Fld(1,RG_LVDSB_CLK_IQ_SEL_O3));
}

void vDrvVCOMode_SEL(void)
{
    if (_u1TconMode==MLVDS_MODE) // 32-Phase VCO
    {     
       vDrvVCOPhase_Init();
    }
    else // 4-Phase VCO
    {
    }
}

void vDrvVOPLLSet(void)
{
    UINT8 u1PreDiv=0,u1FBKDIV=0;	
    UINT8 u1PostDiv=0;
    UINT8 fgLVDSClkDiv2=0;
    UINT8 fgPixelClkDiv2=0;
    UINT8 u1FBDiv=0x07;
    UINT8 u1PPLLPOSDiv2=0x0;
    UINT8 u1SsOclkDiv = 0;
    UINT32 u4DispBit;
    UINT32 u4VB1Lane;	
    UINT8 u1TCONDivSel=0; //=MLDVS_output_port * tclk_pclk_ratio
	UINT32 u4VCOFreq=0;

    u4DispBit = DISP_BIT;
    u4VB1Lane = LVDS_OUTPUT_PORT;
    

    if (_u1TconMode == MLVDS_MODE)
    {      
      // To OSD and LVDSTx setting
      vIO32WriteFldAlign(CKGEN_OCLK_TEST, 0, FLD_OCLK_DIV_SEL);
      
        if (TCLK_PCLK_RATION == TCLK_PCLK_RATION_1)
        {          
             if (MINI_LVDS_OUTPUT_PORT == SINGLE_PORT)
             {
                _u1DddsCWMul =1;
                u1FBDiv=0x07;
                u1PPLLPOSDiv2=0x2;
				u1TCONDivSel=0;
                _u1VCO32PHASE_FREQ = VCO32PHASE_148P5M;
             }
             else  if (MINI_LVDS_OUTPUT_PORT == DUAL_PORT) 
             {
                _u1DddsCWMul =2;
                u1FBDiv=0x0F;                
                u1PPLLPOSDiv2=0x1;
				u1TCONDivSel=1;
                _u1VCO32PHASE_FREQ = VCO32PHASE_297M;
             }
             else if (LVDS_OUTPUT_PORT == FOUR_PORT) 
             {
                _u1DddsCWMul =4;
                u1FBDiv=0x1F;     
                u1PPLLPOSDiv2=0x0;
				u1TCONDivSel=1;
                _u1VCO32PHASE_FREQ = VCO32PHASE_594M;
             }      
             TCLK_PCLK_RATION_NUM = 1;
             TCLK_PCLK_RATION_DEN = 1;      
        }
        else if  (TCLK_PCLK_RATION == TCLK_PCLK_RATION_1P5)
        {
             if (MINI_LVDS_OUTPUT_PORT == SINGLE_PORT)
             {
                _u1DddsCWMul =1;
                u1FBDiv=0x0B;             
                u1PPLLPOSDiv2=0x2;
				u1TCONDivSel=2; 
                _u1VCO32PHASE_FREQ = VCO32PHASE_222P8M;
             }
			 else if (MINI_LVDS_OUTPUT_PORT == DUAL_PORT)
			 {
				 _u1DddsCWMul =2;
				 u1FBDiv=0x17;			   
				 u1PPLLPOSDiv2=0x1;
				 u1TCONDivSel=2;
				 _u1VCO32PHASE_FREQ = VCO32PHASE_445P5M;
        	 }
             TCLK_PCLK_RATION_NUM = 3;
             TCLK_PCLK_RATION_DEN = 2;
        }
        else if  (TCLK_PCLK_RATION == TCLK_PCLK_RATION_2)
        {
             if (MINI_LVDS_OUTPUT_PORT == SINGLE_PORT)
             {
                _u1DddsCWMul =1;
                u1FBDiv=0x0F;                  
                u1PPLLPOSDiv2=0x2;
				u1TCONDivSel=1;
                _u1VCO32PHASE_FREQ = VCO32PHASE_297M;           
             }
             else  if (MINI_LVDS_OUTPUT_PORT == DUAL_PORT) 
             {
                _u1DddsCWMul =2;
                u1FBDiv=0x1F;                      
                u1PPLLPOSDiv2=0x1;
				u1TCONDivSel=3;
                _u1VCO32PHASE_FREQ = VCO32PHASE_594M;
             }        
             TCLK_PCLK_RATION_NUM = 2;
             TCLK_PCLK_RATION_DEN = 1;        
        }
        else if  (TCLK_PCLK_RATION == TCLK_PCLK_RATION_3)
        {
			 if (MINI_LVDS_OUTPUT_PORT == SINGLE_PORT)
			 {
			    _u1DddsCWMul =1;
			    u1FBDiv=0x17;      
                u1PPLLPOSDiv2=0x2;
				u1TCONDivSel=2;
                _u1VCO32PHASE_FREQ = VCO32PHASE_445P5M;
			 }
			 TCLK_PCLK_RATION_NUM = 3;
             TCLK_PCLK_RATION_DEN = 1;        
        }
        else if  (TCLK_PCLK_RATION == TCLK_PCLK_RATION_4)
        {
        	 if (MINI_LVDS_OUTPUT_PORT == SINGLE_PORT)
			 {
			    _u1DddsCWMul =1;
                u1FBDiv=0x1F;      
                u1PPLLPOSDiv2=0x2;
				u1TCONDivSel=3;
                _u1VCO32PHASE_FREQ = VCO32PHASE_594M;           
        	 }
			 TCLK_PCLK_RATION_NUM = 4;
             TCLK_PCLK_RATION_DEN = 1;        
        } 
		else if  (TCLK_PCLK_RATION == TCLK_PCLK_RATION_0P75)
		{
		     if (MINI_LVDS_OUTPUT_PORT == DUAL_PORT) 
		     {
			    _u1DddsCWMul =2;
                u1FBDiv=0x0B;
                u1PPLLPOSDiv2=0x1;
				u1TCONDivSel=0;  //have no 0.75 selection?
				_u1VCO32PHASE_FREQ = VCO32PHASE_222P8M;
		     }	  
		     else if (MINI_LVDS_OUTPUT_PORT == FOUR_PORT) 
		     {
			    _u1DddsCWMul =4;
                u1FBDiv=0x17;
                u1PPLLPOSDiv2=0x0;
				u1TCONDivSel=2;
                _u1VCO32PHASE_FREQ = VCO32PHASE_445P5M;           
		     }	  
		     TCLK_PCLK_RATION_NUM = 3;
		     TCLK_PCLK_RATION_DEN = 4;
		}		
        else
        {
             TCLK_PCLK_RATION_NUM = 1;
             TCLK_PCLK_RATION_DEN = 1;
        }        

        // For Panasonic 120Hz panel
        if(TCON_TCLK_RATION == TCON_TCLK_RATION_1)
        {
            vIO32WriteFldAlign(REG_VOPLL_CFG3, 0x0,RG_VPLL_RESERVE_bit7);
        }
        else
        {
            vIO32WriteFldAlign(REG_VOPLL_CFG3, 0x1,RG_VPLL_RESERVE_bit7);
        }

			vIO32WriteFldAlign(TMGR2, u1TCONDivSel, TMGR2_TCLK_PLLSEL);

		//Set VPLL	
        vIO32WriteFldMulti(REG_VOPLL_CFG0, P_Fld(u1FBDiv, RG_VPLL_FBKDIV)|P_Fld(0x3, RG_VPLL_PREDIV)
		 	                              |P_Fld(0, RG_VPLL_FBKSEL)|P_Fld(0,RG_VPLL_POSDIV)
		 	                              |P_Fld(0,RG_VPLL_REFSEL));
                     
        // 2.B Set PPLL
        vIO32WriteFldMulti(REG_VOPLL_CFG3, P_Fld(0,RG_PPLL_PREDIV)|P_Fld(1,RG_PPLL_FBKSEL)
                                          |P_Fld(1,RG_PPLL_POSDIV)|P_Fld(u1PPLLPOSDiv2,RG_PPLL_POSDIV2)
                                          |P_Fld(0x0F,RG_PPLL_FBKDIV));                                   
                
		vDrvVOPLLInit_MINI();

    }
    else if (_u1TconMode == LVDS_MODE)
    {
	    if (LVDS_OUTPUT_PORT == SINGLE_PORT)
	    {
	       if(IS_LVDS_HIGHSPEED)
	       { 
		   	_u1DddsCWMul =2;
			u1PostDiv=0;
	       }
		   else
		   {
	        _u1DddsCWMul =1;
	        u1PostDiv=1; //total VOPLL = VCO / 2 
	   	   }	    
	        
	        fgLVDSClkDiv2=0;
	        fgPixelClkDiv2=1;
	        vIO32WriteFldAlign(CKGEN_OCLK_TEST, 0, FLD_OCLK_DIV_SEL);        
	    }
	    else  if (LVDS_OUTPUT_PORT == DUAL_PORT) 
	    {
	        if(IS_LVDS_HIGHSPEED)
	        {    
	         _u1DddsCWMul =4;
	         u1PostDiv=0; 		 
	        }
			else
			{
			 _u1DddsCWMul =2;
	         u1PostDiv=1;  //total VOPLL = VCO / 2 	 
			}
			fgLVDSClkDiv2=0;   
			fgPixelClkDiv2=0;
	        u1SsOclkDiv = 1; //default lvds clock for dual-port

	        #ifdef DRV_SUPPORT_EXTMJC
	        if (IS_COMPANION_CHIP_ON())
	        {
	            if ((vDrvGetCurLvdsMode() ==  eLvdsVidYuv422OnePort ) || (vDrvGetCurLvdsMode() ==  eLvdsVidRgb444OnePort )
	                || (vDrvGetCurLvdsMode() ==  eLvdsVidYuv422OnePort8Bit ) || (vDrvGetCurLvdsMode() ==  eLvdsVidRgb444OnePort8Bit ))
	            {
	                u1PostDiv=1;  
	                fgLVDSClkDiv2=0;
	                fgPixelClkDiv2=0;
	                u1SsOclkDiv = 0; //lvds clock for one-port video data
	            }
	        }   
	        #endif
	        
			vIO32WriteFldAlign(CKGEN_OCLK_TEST, u1SsOclkDiv, FLD_OCLK_DIV_SEL);        
	    }
	    else //if (LVDS_OUTPUT_PORT == FOUR_PORT) 
	    {
	        if(IS_LVDS_HIGHSPEED)
	        {
	         _u1DddsCWMul =8;
	        u1SsOclkDiv = 1;
			fgLVDSClkDiv2=0;
	        }
	        else
	        {
	         _u1DddsCWMul =4;
	        u1SsOclkDiv = 2;        
			fgLVDSClkDiv2=1;
	        }
			
	        u1PostDiv=0;   //total VOPLL = VCO / 1 
	        fgPixelClkDiv2=0;

	        vIO32WriteFldAlign(CKGEN_OCLK_TEST, u1SsOclkDiv, FLD_OCLK_DIV_SEL);
	    }
	    
	    // 2. setup vpll start -----------------------------------------------------

	    // 2.A FB divider setting + Protect Mode (patch for LVDS VCO double freq. if VCO is lower than 360MHz)

	    // VCO = 74.25  / VPLL_PREDIV * (VPLL_FBDIVBYPEN & VPLL_FBDIV3P5EN) * VPLL01_VPLL_FBDIV
		u4VCOFreq = (_u4SetVopllFreq / _u1DddsCWMul)* 7;

	    if(_fgVopllUseDDDS)
	    {
	    	u1PreDiv = 1;  
			u1FBKDIV = 0x0D;
			if(u4VCOFreq < 360000000)
			{		
				if((u1PostDiv < 2)
					&&(u1PreDiv > 0));
				{
					u1PostDiv = u1PostDiv + 1;
					u1PreDiv = u1PreDiv - 1;
				}
				#ifdef VOPLL_Debug
				Printf("For LVDS VCO double freq. if VCO is lower than 360MHz u4VCOFreq = %d\n", u4VCOFreq);		
				#endif
			}
	    }
	    else
	    {
	    	u1PreDiv = 0;
	        if(u4VCOFreq < 360000000)
	        {
	           if(u1PostDiv < 2);
	           {
	              u1PostDiv = u1PostDiv + 1;
	              u4VCOFreq = u4VCOFreq << 1;
	           }
	        }
	        if(u4VCOFreq < 360000000)
	        {
	            if(u1PostDiv < 2);
	            {
	              u1PostDiv = u1PostDiv + 1;
	              u4VCOFreq = u4VCOFreq << 1;				
	            }
	        }		
	        if(IS_XTAL_24MHZ())
	    		u1FBKDIV =	(u4VCOFreq / 24000000) - 1;
	        else
	    		u1FBKDIV =	(u4VCOFreq / 27000000) - 1;		
	    }

		// [DDDS] 519.75 = 74.25 / 2 * 1 *14
		vIO32WriteFldMulti(REG_VOPLL_CFG0, P_Fld(u1PreDiv, RG_VPLL_PREDIV)|P_Fld(0x00, RG_VPLL_FBKSEL)
											 |P_Fld(u1FBKDIV, RG_VPLL_FBKDIV));

	    // 2.B Set post divider
	    vIO32WriteFldAlign(REG_VOPLL_CFG1, fgLVDSClkDiv2<<1, RG_VPLL_TXCLK_DIV2);     
	    vIO32WriteFldMulti(REG_VOPLL_CFG1, P_Fld(u1PostDiv,RG_VPLL_TXCLK_DIV)
			                              |P_Fld(fgPixelClkDiv2,RG_VPLL_LVDS_DPIX_DIV2));                                                                     
	    vIO32WriteFldMulti(REG_VOPLL_CFG3, P_Fld(0, RG_VPLL_RESERVE)|P_Fld(0x0F, RG_PPLL_FBKDIV)|P_Fld(0, RG_PPLL_PREDIV)
	  	                                  |P_Fld(0, RG_PPLL_FBKSEL)|P_Fld(1, RG_PPLL_POSDIV)|P_Fld(0, RG_PPLL_POSDIV2));
	    vIO32WriteFldAlign(REG_VOPLL_CFG0, fgLVDSClkDiv2, RG_VPLL_POSDIV);     

	}           
#ifdef SUPPORT_EPI
    else if (_u1TconMode == EPI_MODE)
    {
		if(LVDS_OUTPUT_PORT == FOUR_PORT)
		{	
			if ((IS_EPI_8LANE) || (IS_EPI_4LANE))			
			{
				_u1DddsCWMul = 8;
			}
			else
			{
				_u1DddsCWMul = 6;
			}

			switch (u4DispBit)
			{
				case DISP_30BIT:
					vIO32WriteFldMulti(REG_VOPLL_CFG0, P_Fld(0x10, RG_VPLL_FBKDIV)|P_Fld(0, RG_VPLL_PREDIV)
						                              |P_Fld(0, RG_VPLL_FBKSEL));

					vIO32WriteFldMulti(REG_VOPLL_CFG2, P_Fld(3, RG_VPLL_BC)|P_Fld(2, RG_VPLL_DIVEN)
						                              |P_Fld(2, RG_VPLL_BIR)|P_Fld(9, RG_VPLL_BP));
			
				break;
				case DISP_24BIT:
					vIO32WriteFldMulti(REG_VOPLL_CFG0, P_Fld(0x0D, RG_VPLL_FBKDIV)|P_Fld(0, RG_VPLL_PREDIV)
						                              |P_Fld(0, RG_VPLL_FBKSEL));

					if (IS_EPI_8LANE)
					{
					vIO32WriteFldMulti(REG_VOPLL_CFG2, P_Fld(3, RG_VPLL_BC)|P_Fld(3, RG_VPLL_DIVEN)
													  |P_Fld(2, RG_VPLL_BIR)|P_Fld(0xC, RG_VPLL_BP));

					}
					else
					{
                    vIO32WriteFldMulti(REG_VOPLL_CFG2, P_Fld(3, RG_VPLL_BC)|P_Fld(2, RG_VPLL_DIVEN)
						                              |P_Fld(2, RG_VPLL_BIR)|P_Fld(8, RG_VPLL_BP));
					}
				break;
				case DISP_18BIT:
					vIO32WriteFldMulti(REG_VOPLL_CFG0, P_Fld(0x0A, RG_VPLL_FBKDIV)|P_Fld(0, RG_VPLL_PREDIV)
						                              |P_Fld(0, RG_VPLL_FBKSEL));

					vIO32WriteFldMulti(REG_VOPLL_CFG2, P_Fld(2, RG_VPLL_BC)|P_Fld(4, RG_VPLL_DIVEN)
						                              |P_Fld(1, RG_VPLL_BIR)|P_Fld(0X0D, RG_VPLL_BP));
				break;
				default:
				break;
			}

			vIO32WriteFldMulti(REG_VOPLL_CFG0, P_Fld(0, RG_VPLL_POSDIV)|P_Fld(0, RG_VPLL_REFSEL));
			
            vIO32WriteFldAlign(REG_VOPLL_CFG1, VPLL_POST_VB1EN|VPLL_POST_EPIEN, VPLL_POST_SEL);
			vIO32WriteFldMulti(REG_VOPLL_CFG1, P_Fld(1, RG_VPLL_LVDSCHL_EN)|P_Fld(0, RG_VPLL_LVDS_DPIX_DIV2)|P_Fld(0, RG_VPLL_TXCLK_DIV)
				                              |P_Fld(0, RG_VPLL_TXCLK_DIV2)|P_Fld(3, RG_VPLL_AUTOK_CTRL)|P_Fld(3, RG_VPLL_BAND)
				                              |P_Fld(1, RG_VPLL_AUTOK_EN)|P_Fld(1, RG_VPLL_AUTOK_LOAD)|P_Fld(1, RG_VPLL_VODEN));		  

			vIO32WriteFldMulti(REG_VOPLL_CFG2, P_Fld(1, RG_VPLL_BIC)|P_Fld(4, RG_VPLL_BR)|P_Fld(0, RG_VPLL_FPEN)
				                              |P_Fld(1, RG_VPLL_FMEN));
			
			vIO32WriteFldMulti(REG_VOPLL_CFG3, P_Fld(1, RG_PPLL_PREDIV)|P_Fld(0, RG_PPLL_POSDIV2));

			vIO32WriteFldAlign(REG_LVDS_TX_CFG19, 0x0, RG_LVDSA_RESYNC_LVDS_SEL_D11);
            vIO32WriteFldAlign(REG_LVDS_TX_CFG14, 0, RG_LVDSA_LVDS_SEL);
		
		}
		else if (LVDS_OUTPUT_PORT == DUAL_PORT)
		{

			if ((IS_EPI_8LANE) || (IS_EPI_4LANE))
			{
				_u1DddsCWMul = 4;
			}
			else
			{
				_u1DddsCWMul = 3;
			}
			
			switch (u4DispBit)
			{
				case DISP_30BIT:
					vIO32WriteFldMulti(REG_VOPLL_CFG0, P_Fld(0x10, RG_VPLL_FBKDIV)|P_Fld(1, RG_VPLL_PREDIV)
													  |P_Fld(1, RG_VPLL_FBKSEL));
			
					vIO32WriteFldMulti(REG_VOPLL_CFG2, P_Fld(3, RG_VPLL_BC)|P_Fld(2, RG_VPLL_DIVEN)
													  |P_Fld(3, RG_VPLL_BIR)|P_Fld(0x0E, RG_VPLL_BP));
				break;
				case DISP_24BIT:
					if (IS_EPI_4LANE)
					{
					vIO32WriteFldMulti(REG_VOPLL_CFG0, P_Fld(0x0D, RG_VPLL_FBKDIV)|P_Fld(0, RG_VPLL_PREDIV)
													  |P_Fld(0, RG_VPLL_FBKSEL));
					vIO32WriteFldMulti(REG_VOPLL_CFG2, P_Fld(3, RG_VPLL_BC)|P_Fld(3, RG_VPLL_DIVEN)
													  |P_Fld(1, RG_VPLL_BIR)|P_Fld(0x0C, RG_VPLL_BP));					}
					else
					{
					vIO32WriteFldMulti(REG_VOPLL_CFG0, P_Fld(0x0D, RG_VPLL_FBKDIV)|P_Fld(1, RG_VPLL_PREDIV)
													  |P_Fld(1, RG_VPLL_FBKSEL));
					if (IS_EPI_8LANE)
					{
					vIO32WriteFldMulti(REG_VOPLL_CFG2, P_Fld(3, RG_VPLL_BC)|P_Fld(2, RG_VPLL_DIVEN)
													  |P_Fld(2, RG_VPLL_BIR)|P_Fld(0x0C, RG_VPLL_BP));					

					}
					else
					{
					vIO32WriteFldMulti(REG_VOPLL_CFG2, P_Fld(2, RG_VPLL_BC)|P_Fld(2, RG_VPLL_DIVEN)
													  |P_Fld(3, RG_VPLL_BIR)|P_Fld(0x0B, RG_VPLL_BP));					
					}
					}
					

				break;
				case DISP_18BIT:
					vIO32WriteFldMulti(REG_VOPLL_CFG0, P_Fld(0x0A, RG_VPLL_FBKDIV)|P_Fld(1, RG_VPLL_PREDIV)
													  |P_Fld(1, RG_VPLL_FBKSEL));
					
					vIO32WriteFldMulti(REG_VOPLL_CFG2, P_Fld(2, RG_VPLL_BC)|P_Fld(3, RG_VPLL_DIVEN)
													  |P_Fld(2, RG_VPLL_BIR)|P_Fld(0x0C, RG_VPLL_BP));
				break;
				default:
				break;
			}		
				

		    vIO32WriteFldMulti(REG_VOPLL_CFG0, P_Fld(0, RG_VPLL_POSDIV)|P_Fld(0, RG_VPLL_REFSEL));
			
            vIO32WriteFldAlign(REG_VOPLL_CFG1, VPLL_POST_EPIEN, VPLL_POST_SEL);
			vIO32WriteFldMulti(REG_VOPLL_CFG1, P_Fld(1, RG_VPLL_LVDSCHL_EN)|P_Fld(0, RG_VPLL_LVDS_DPIX_DIV2)
				                              |P_Fld(0, RG_VPLL_TXCLK_DIV2)|P_Fld(3, RG_VPLL_AUTOK_CTRL)|P_Fld(3, RG_VPLL_BAND)
				                              |P_Fld(1, RG_VPLL_AUTOK_EN)|P_Fld(1, RG_VPLL_AUTOK_LOAD)|P_Fld(1, RG_VPLL_VODEN));	
			if (IS_EPI_4LANE)
			vIO32WriteFldAlign(REG_VOPLL_CFG1, 0, RG_VPLL_TXCLK_DIV);
			else
			vIO32WriteFldAlign(REG_VOPLL_CFG1, 1, RG_VPLL_TXCLK_DIV);				

			vIO32WriteFldMulti(REG_VOPLL_CFG2, P_Fld(1, RG_VPLL_BIC)|P_Fld(4, RG_VPLL_BR)|P_Fld(0, RG_VPLL_FPEN)
				                              |P_Fld(1, RG_VPLL_FMEN));

			if (IS_EPI_4LANE)
			{
			vIO32WriteFldMulti(REG_VOPLL_CFG3, P_Fld(1, RG_PPLL_PREDIV)|P_Fld(1, RG_PPLL_POSDIV2));
			}
			else
			{
			vIO32WriteFldMulti(REG_VOPLL_CFG3, P_Fld(0, RG_PPLL_PREDIV)|P_Fld(1, RG_PPLL_POSDIV2));
			}
			
            vIO32WriteFldAlign(REG_LVDS_TX_CFG19, 0x1, RG_LVDSA_RESYNC_LVDS_SEL_D11);
			#ifdef CC_MT5399
            	vIO32WriteFldAlign(REG_LVDS_TX_CFG14, 0xFFF, RG_LVDSA_LVDS_SEL);
			#else
				vIO32WriteFldAlign(REG_LVDS_TX_CFG14, 0x3FFF, RG_LVDSA_LVDS_SEL);
			#endif
		}
		else // if (LVDS_OUTPUT_PORT == SINGLE_PORT)
		{

		}
    
    }
#endif
    else if (_u1TconMode == VB1_MODE)    
    {
                             
		if(u4VB1Lane == FOUR_PORT)
		{	
			if(IS_LVDS_HIGHSPEED)
				_u1DddsCWMul =8;
			else
				_u1DddsCWMul =4;
			vIO32WriteFldAlign(REG_VOPLL_CFG3, 0, RG_PPLL_POSDIV2);
		
		}
		else if (u4VB1Lane == DUAL_PORT)
		{
			_u1DddsCWMul =2;				   
			vIO32WriteFldAlign(REG_VOPLL_CFG3, 1, RG_PPLL_POSDIV2);
				   
		}
		else if (u4VB1Lane == SINGLE_PORT)
		{
			_u1DddsCWMul =1;			   
			vIO32WriteFldAlign(REG_VOPLL_CFG3, 2, RG_PPLL_POSDIV2);
		}
		
		switch (u4DispBit)
		{
			case DISP_36BIT:
				vIO32WriteFldMulti(REG_VOPLL_CFG0, P_Fld(0x18, RG_VPLL_FBKDIV)|P_Fld(3, RG_VPLL_FBKSEL));		
			break;
			
			case DISP_30BIT:
				vIO32WriteFldMulti(REG_VOPLL_CFG0, P_Fld(0x27, RG_VPLL_FBKDIV)|P_Fld(1, RG_VPLL_FBKSEL));
			break;
			
	 		case DISP_18BIT:
	 		case DISP_24BIT:
				vIO32WriteFldMulti(REG_VOPLL_CFG0, P_Fld(0x1D, RG_VPLL_FBKDIV)|P_Fld(1, RG_VPLL_FBKSEL));
			break;
			
			default:
			break;
    	}
    	#if (!(VOPLL_STRESS_TEST))
    	  printf("###VbyOne VOPLLSet  finsih###");
    	#endif
    }    
    else if (_u1TconMode == iDP_MODE)
    {

    }

    // 3. set AD_DPIX_CLK output
    vDrvDPIX_CLK_SEL();

    // 4. select VCO mode
    vDrvVCOMode_SEL();    

}

void vDrvVOPLLVCOKBand(void)
{
    if (_u1TconMode==VB1_MODE)
    {
    
    }
}


UINT32 GetPLLCounter(UINT8 u1case)
{
    vIO32WriteFldAlign(CKGEN_PLLCALIB, 0, FLD_CAL_SEL);        
    vIO32WriteFldAlign(CKGEN_PLLCALIB, 0, FLD_CAL_MODE); // 11 bit                   
    
    vIO32WriteFldAlign(CKGEN_PLLCALIB,u1case,FLD_DBGCKSEL);
    vIO32WriteFldAlign(CKGEN_PLLCALIB,0x01,FLD_SOFT_RST_CAL);
    HAL_Delay_us(20);        
    vIO32WriteFldAlign(CKGEN_PLLCALIB,0x01,FLD_CAL_TRI);         
    // Wait calibration finishing.
    while (IO32ReadFldAlign(CKGEN_PLLCALIB, FLD_CAL_TRI)) { }        
    return(IO32ReadFldAlign(CKGEN_PLLCALIBCNT,FLD_CALI_CNT));

}

void u1VOPLLSource(UINT8 u1arg1)
{
    if (u1arg1<2)
    {   
    _fgVopllUseDDDS = !(u1arg1);
    }
}

#define MUSTANG_SYSPLL_CLK 648000000
#define SYSPLLCLK_FREQ MUSTANG_SYSPLL_CLK

void vSetVOPLLFreq(UINT32 u4Freq)
{

    UINT32 u4Syspll = 0;
    UINT64 u8Cw;
    UINT8 u1ResetCnt=0,u1Mode=0;    
    UINT16 u2Oclk=0, u2Oclk2=0;   
    UINT32 u2Oclk_target=0, u2Oclk2_target=0;
    UINT32 u2Oclk_targetH=0, u2Oclk_targetL=0,u2Oclk2_targetH=0,u2Oclk2_targetL=0;    
    UINT32 u4RefCount=0;
    #ifdef VOPLL_Debug  
      UINT32 CW_Read,CWS_Read;
    #endif
    #if VOPLL_STRESS_TEST
      UINT32 u8TestCnt=1000000;
    #endif

    if (u4Freq == 0)
    {
        return;
    }

	_u4SetVopllFreq = u4Freq;

#ifdef VOPLL_Debug 
 #if (!(VOPLL_STRESS_TEST))
           Printf("[LVDS] u4Freq=%d \n",u4Freq);     
 #endif
#endif

    if ((IO32ReadFldAlign(DDDS_08, FLAG_VOPLL_POWERON) == 1)&& (!(IS_PANEL_L12R12)))
    {
        _fgVOPLLPowerOn = TRUE;
    }
      
    #if VOPLL_STRESS_TEST
    do
    {
    #endif    

     if( !_fgVOPLLPowerOn)
     {   
      u1ResetCnt = VOPLLSetRecoverCnt;
     }
     else
     {
      u1ResetCnt = 0;
     }

     #ifdef VOPLL_Debug 
      #if (!(VOPLL_STRESS_TEST))
      Printf("[LVDS] Do Loop=%d \n",u1ResetCnt);     
      #endif
     #endif

     vDrvVOPLLInit();
      
     do
     {
      #ifdef VOPLL_Debug     
      if (u1ResetCnt!=VOPLLSetRecoverCnt)
      {
       Printf("[LVDS] Do Loop=%d \n",u1ResetCnt);     
      } 
      #endif       

      vDrvVOPLLSet();
      
    if(_fgVopllUseDDDS)// use ddds
    {           		
		Printf("[SA7] _fgVopllUseDDDS = True\n");
	        
        // B1. setup ddds start -------------------------------------------------
        vIO32WriteFldAlign(DDDS_03, 1,DDDS_03_SPREAD_INIT);
   
        // B2. calculate control word
        u4Syspll = SYSPLLCLK_FREQ; // _CalGetPLLCounter(syspll_d2_ck)*2000; //SYSPLL = 459MHz in MT5395 //CNTseng check

        u8Cw = u8Div6432(((UINT64)u4Syspll << VPLL_SPREAD_INT_SHIFT), u4Freq, NULL);
		#if DDDS_OUT_DIV2
        u8Cw = ((((UINT32)u8Cw*_u1DddsCWMul) >> 1) & VPLL_SPREAD_CW);
		#else
        u8Cw = (((UINT32)u8Cw*_u1DddsCWMul) & VPLL_SPREAD_CW);
		#endif

		#if defined(SUPPORT_CW_CHANGE_BY_STEP) && !defined(CC_MTK_LOADER) 
		if(((PANEL_GetDisplayType()== PANEL_DISPLAY_TYPE_VB1) && (IS_STEP_CLOCK_CHG))||(PANEL_GetDisplayType()== PANEL_DISPLAY_TYPE_EPI))
		{
        	if(!bIsPanelOn())
        	{
            	vIO32WriteFldAlign(DDDS_00, u8Cw,DDDS_00_DDDS_FREQ_CW);
        	}
        	_u4CurrentCW = IO32ReadFldAlign(DDDS_00, DDDS_00_DDDS_FREQ_CW);
        	_u4TargetCW = (UINT32)u8Cw;
        }
        else
        {
			vIO32WriteFldAlign(DDDS_00, u8Cw,DDDS_00_DDDS_FREQ_CW);
        }
		#else
        vIO32WriteFldAlign(DDDS_00, u8Cw,DDDS_00_DDDS_FREQ_CW);
		#endif

        // B3. enable init
        vIO32WriteFldAlign(DDDS_03, 0,DDDS_03_SPREAD_INIT);        
        // setup ddds done -----------------------------------------------------
        #ifdef SUPPORT_DDDS_STEP_TRACKING
        vDrvUpdateDisplayCenterCW((UINT32)u8Cw);
        #endif

		vIO32WriteFldAlign(DDDS_03, 1, DDDS_03_DATA_SYNC_AUTO); 	

		#if DDDS_OUT_DIV2
        vIO32WriteFldAlign(REG_PLL_GROUP_CFG8, 0, FLD_RG_DDDS1_CKSEL); //VPLL from DDDS1
    	#if ENABLE_DDDS2_FEEDBACK
        vIO32WriteFldAlign(REG_PLL_GROUP_CFG8, 0, FLD_RG_DDDS2_CKSEL); //PIXPLL from DDDS2
    	#endif
		#else
        vIO32WriteFldAlign(REG_PLL_GROUP_CFG8, 1, FLD_RG_DDDS1_CKSEL); //VPLL from DDDS1
    	#if ENABLE_DDDS2_FEEDBACK
        vIO32WriteFldAlign(REG_PLL_GROUP_CFG8, 1, FLD_RG_DDDS2_CKSEL); //PIXPLL from DDDS2
    	#endif
		#endif
    }
    else // use xtal
    {
		
        vIO32WriteFldAlign(REG_PLL_GROUP_CFG8, 2, FLD_RG_DDDS1_CKSEL); //VPLL from DDDS1
        #if ENABLE_DDDS2_FEEDBACK        
        vIO32WriteFldAlign(REG_PLL_GROUP_CFG8, 2, FLD_RG_DDDS2_CKSEL); //PIXPLL from DDDS2
        #endif
    }    

    #ifndef CC_FPGA
    vDrvSetVOPLLClockSchema(VOPLL_CLK_SRC_DDDS);
    #endif 

    // 3. Power on VOPLL and check clock
    if(_fgVOPLLPowerOn)
    {
        u1ResetCnt = 0;
    }
    else
    {
		 vIO32WriteFldAlign(REG_VOPLL_CFG0, 1, RG_VPLL_BG_LVDS_PWD);
		 vIO32WriteFldAlign(REG_VOPLL_CFG0, 1, RG_VPLL_BG_VB1_TX_PWD);
		 vIO32WriteFldAlign(REG_VOPLL_CFG0, 1, RG_VPLL_PWD);
		 vIO32WriteFldAlign(REG_VOPLL_CFG0, 1, RG_VPLL_BIAS_BYP);
		 vIO32WriteFldAlign(REG_VOPLL_CFG3, 1, RG_PPLL_PWD);	   
		 HAL_Delay_us(10);
		 vIO32WriteFldAlign(REG_VOPLL_CFG0, 0, RG_VPLL_PWD); 

		 if (_u1TconMode != LVDS_MODE) //use PPLL
		 {
		 	if (_u1TconMode==VB1_MODE)
		    HAL_Delay_us(200);
			else
		    HAL_Delay_us(100);
		 	vIO32WriteFldAlign(REG_VOPLL_CFG3, 0, RG_PPLL_PWD);
		 }
		 vIO32WriteFldAlign(REG_VOPLL_CFG0, 0, RG_VPLL_BG_LVDS_PWD);
		 vIO32WriteFldAlign(REG_VOPLL_CFG0, 0, RG_VPLL_BG_VB1_TX_PWD);
		 vIO32WriteFldAlign(REG_VOPLL_CFG0, 0, RG_VPLL_BIAS_BYP);
		 HAL_Delay_us(20);
     

		if((u1ResetCnt%4) == 1)
		{      
			#ifdef VOPLL_Debug
			 Printf(" [LVDS] Toggle DDDS u1ResetCnt=%d  \n",u1ResetCnt);
			#endif
			vIO32WriteFldAlign(DDDS_03,1,DDDS_03_SPREAD_INIT);
			HAL_Delay_us(20);
			vIO32WriteFldAlign(DDDS_03,0,DDDS_03_SPREAD_INIT);            
			if (_u1TconMode!=LVDS_MODE) //use PPLL
			{
				vIO32WriteFldAlign(REG_VOPLL_CFG3, 1, RG_PPLL_PWD);       
				if (_u1TconMode==VB1_MODE)
				HAL_Delay_us(400);
				else
				HAL_Delay_us(200);
				vIO32WriteFldAlign(REG_VOPLL_CFG3, 0, RG_PPLL_PWD);
			}
		}

		vDrvVOPLLVCOKBand();

		if(_u1TconMode==LVDS_MODE)//switch DPIX clock
		{
		u2Oclk =  GetPLLCounter(AD_LVDS_DPIX_CK);      
		u2Oclk2 = GetPLLCounter(AD_VPLL_TCON_CK);
		}
		else
		{
		u2Oclk =  GetPLLCounter(AD_PPLL_DPIX_CK);	  
		u2Oclk2 = GetPLLCounter(AD_VPLL_TCON_CK);
		}

		#ifdef VOPLL_Debug
		#if (!(VOPLL_STRESS_TEST))
		    Printf("[LVDS] u2Oclk2=0x%x  \n",u2Oclk2);
		    Printf("[LVDS] u2Oclk=0x%x  \n",u2Oclk);            
		#endif
		#endif

		#ifdef VOPLL_Debug     
		CW_Read=IO32ReadFldAlign(DDDS_00,DDDS_00_DDDS_FREQ_CW);
		CWS_Read=IO32ReadFldAlign(STA_DDDS_00,STA_DDDS_00_DDDS_FREQ_CW);

		#if (!(VOPLL_STRESS_TEST))       
		Printf("[LVDS]CW_Read=0x%x \n",CW_Read);
		Printf("[LVDS]CWS_Read=0x%x \n",CWS_Read);  
		#endif 
		#endif

		u1Mode = IO32ReadFldAlign(CKGEN_PLLCALIB, FLD_CAL_MODE);

		switch(u1Mode) {
		case 0: u4RefCount = 11; break;
		case 1: u4RefCount = 16; break;
		case 2: u4RefCount = 20; break;
		case 3: u4RefCount = 24; break;
		default: u4RefCount = 11; break;
		}

		if (((LVDS_OUTPUT_PORT == FOUR_PORT)&&(IS_LVDS_HIGHSPEED)&&(_u1TconMode==LVDS_MODE))
			  || ((_u1TconMode==VB1_MODE) && (IS_LVDS_HIGHSPEED)))
		{
			u2Oclk_target= ((u4Freq/2/1000000)<<u4RefCount)/(GET_XTAL_CLK()/1000000);
		}
		else
		{
			u2Oclk_target= ((u4Freq/1000000)<<u4RefCount)/(GET_XTAL_CLK()/1000000);
		}
		u2Oclk_targetH= u2Oclk_target + (u2Oclk_target*5/100);
		u2Oclk_targetL= u2Oclk_target - (u2Oclk_target*5/100);

		if(_u1TconMode==MLVDS_MODE)
		{     
		u2Oclk2_target= (u2Oclk_target*TCLK_PCLK_RATION_NUM)/TCLK_PCLK_RATION_DEN;	
		//if(TCON_TCLK_RATION == TCON_TCLK_RATION_2)
		//u2Oclk2_target= u2Oclk2_target << 1;			
		u2Oclk2_targetH= u2Oclk2_target + (u2Oclk2_target*5/100);
		u2Oclk2_targetL= u2Oclk2_target - (u2Oclk2_target*5/100);
		}
		else if (_u1TconMode==LVDS_MODE)
		{
		if ((LVDS_OUTPUT_PORT == FOUR_PORT)&&(IS_LVDS_HIGHSPEED))
		{
		u2Oclk2_target= (u2Oclk_target*7)/(_u1DddsCWMul/2)/5;
		} 
		else if ((LVDS_OUTPUT_PORT == DUAL_PORT)&&(IS_LVDS_HIGHSPEED))
		{
		u2Oclk2_target= (u2Oclk_target*7)/(_u1DddsCWMul/2)/2/5;
		} 
		else if ((LVDS_OUTPUT_PORT == SINGLE_PORT)&&(IS_LVDS_HIGHSPEED))
		{
		u2Oclk2_target= (u2Oclk_target*7)/(_u1DddsCWMul/2)/2/5;
		} 	   
		else
		{
		u2Oclk2_target= (u2Oclk_target*7)/_u1DddsCWMul/2/5;
		} 
		u2Oclk2_targetH= u2Oclk2_target + (u2Oclk2_target*5/100);
		u2Oclk2_targetL= u2Oclk2_target - (u2Oclk2_target*5/100);
		}    
		else if (_u1TconMode==VB1_MODE)
		{
		if(DISP_BIT == DISP_36BIT)
			u2Oclk2_target= (u2Oclk_target*5)/_u1DddsCWMul;
		else if(DISP_BIT == DISP_30BIT)
			u2Oclk2_target= (u2Oclk_target*4)/_u1DddsCWMul;
		else
			u2Oclk2_target= (u2Oclk_target*3)/_u1DddsCWMul;
		u2Oclk2_targetH= u2Oclk2_target + (u2Oclk2_target*5/100);
		u2Oclk2_targetL= u2Oclk2_target - (u2Oclk2_target*5/100);
		}    
		else if (_u1TconMode==EPI_MODE)
		{
		  if(DISP_BIT == DISP_30BIT)
			  u2Oclk2_target= (u2Oclk_target*17)/3/10;
		  else if(DISP_BIT == DISP_24BIT)
			  u2Oclk2_target= (u2Oclk_target*14)/3/10;
		  else
			  u2Oclk2_target= (u2Oclk_target*11)/3/10;
		u2Oclk2_targetH= u2Oclk2_target + (u2Oclk2_target*5/100);
		u2Oclk2_targetL= u2Oclk2_target - (u2Oclk2_target*5/100);
		}      
		else if (_u1TconMode==iDP_MODE)
		{

		}      

		#if (!(VOPLL_STRESS_TEST))  
		if (_u1TconMode==MLVDS_MODE)
		{
		Printf("[MLVDS]_u1TconMode=0x%x \n",_u1TconMode);
		Printf("[MLVDS]TCLK_PCLK_RATION_NUM=0x%x \n",TCLK_PCLK_RATION_NUM);
		Printf("[MLVDS]TCLK_PCLK_RATION_DEN=0x%x \n",TCLK_PCLK_RATION_DEN);  
		Printf("[MLVDS]_u1VCO32PHASE_FREQ=0x%x \n",_u1VCO32PHASE_FREQ);
		Printf("[MLVDS]u2Oclk_target=0x%x \n",u2Oclk_target);  
		Printf("[MLVDS]u2Oclk2_target=0x%x \n",u2Oclk2_target);            
		}
		#endif


		if((((((u2Oclk2>=u2Oclk2_targetL)&&(u2Oclk2<=u2Oclk2_targetH))&&((u2Oclk>=u2Oclk_targetL)&&(u2Oclk<=u2Oclk_targetH)))
		 && ((_u1TconMode == MLVDS_MODE)||(_u1TconMode == LVDS_MODE)))
		 || (((u2Oclk>=u2Oclk_targetL)&&(u2Oclk<=u2Oclk_targetH))&& ((_u1TconMode == VB1_MODE)||(_u1TconMode == EPI_MODE))))
		 || (!_fgVopllUseDDDS ))
		 
		{
		u1ResetCnt=0;
		if(_u1TconMode != VB1_MODE)
		{	   
			_Lvds7To4FifoReset = 2;
		}

		#if (!(VOPLL_STRESS_TEST))
			Printf("[LVDS] VOPLL Initialize successful !\n");
		#endif
		if(_u1TconMode==LVDS_MODE)//switch DPIX clock
		{
		  vIO32WriteFldAlign(CKGEN_TCLK_TEST, FLD_OCLK_SEL_LVDS, FLD_OCLK_SEL); 	
		  vIO32WriteFldAlign(CKGEN_TCLK_TEST, 1, FLD_TCLK_SEL);	// vpll_tcon_ck	
		}
		else if (_u1TconMode==VB1_MODE)
		{
		  vIO32WriteFldAlign(CKGEN_TCLK_TEST, FLD_OCLK_SEL_PPLL, FLD_OCLK_SEL);   
		  vIO32WriteFldAlign(CKGEN_TCLK_TEST, 0, FLD_TCLK_SEL);	// xtal to tclk for tcon isr
		}			
		else
		{
		  vIO32WriteFldAlign(CKGEN_TCLK_TEST, FLD_OCLK_SEL_PPLL, FLD_OCLK_SEL);   
		  vIO32WriteFldAlign(CKGEN_TCLK_TEST, 1, FLD_TCLK_SEL);	// vpll_tcon_ck	
		}

		 vIO32WriteFldAlign(CKGEN_MBIST_CK_CTRL, 0, FLD_BIST_VPCLK_SEL);
		 
		 vIO32WriteFldAlign(REG_LVDS_TX_CFG11, 0,RG_LVDSA_EN);	 
		 HAL_Delay_us(LVDSA_EN_REST_DELAYTIME);
		 vIO32WriteFldAlign(REG_LVDS_TX_CFG11, 1,RG_LVDSA_EN);	
		 
		_fgVOPLLPowerOn = TRUE;
		vIO32WriteFldAlign(DDDS_08, 1, FLAG_VOPLL_POWERON);

		}        
		else
		{
		u1ResetCnt--;      
		#ifdef VOPLL_Debug 
		Printf("[LVDS] u2Oclk2=0x%x \n",u2Oclk2);
		Printf("[LVDS] u2Oclk=0x%x \n",u2Oclk);
		if(u2Oclk2 == u2Oclk)
		{
		Printf("[LVDS] VOPLL Reset Fail- OCLK no clock\n");          
		}
		else
		{
		Printf("[LVDS] VOPLL Reset Fail- OCLK wrong clock \n");          
		}
		Printf("[LVDS] CW_Read=0x%x \n",CW_Read);
		Printf("[LVDS] CWS_Read=0x%x \n",CWS_Read);       
		if (u1ResetCnt == 0)
		{
		Printf("[LVDS] VOPLL Rrecover Fail \n");          
		}
		#endif
		if (u1ResetCnt == 0)
		{       
		Printf("[LVDS] u2Oclk2=0x%x \n",u2Oclk2);
		Printf("[LVDS] u2Oclk=0x%x \n",u2Oclk);       
		Printf("[MLVDS]u2Oclk_target=0x%x \n",u2Oclk_target);  
		Printf("[MLVDS]u2Oclk2_target=0x%x \n",u2Oclk2_target);          
		Printf("[LVDS] VOPLL Initialize Fail !\n");
		} 
		#if VOPLL_STRESS_TEST
		while(1);
		#endif    
		}
	}      
	}while(u1ResetCnt>0);

	#if VOPLL_STRESS_TEST
	u8TestCnt--;
	Printf("%d \n",u8TestCnt);	   
	}while(u8TestCnt>0);
	#endif    
 
}

void vPanelPowerSaving(void)
{

	if(PANEL_GetDisplayType()== PANEL_DISPLAY_TYPE_LVDS)
	{
	 vIO32WriteFldAlign(TMGR0, 1, CKEN_CFG_TCON);
	 vIO32WriteFldAlign(TMGR0, 1, CKEN_CFG_MLVDS);
	 vIO32WriteFldAlign(TMGR0, 0, CKEN_CFG_VB1);
	 vIO32WriteFldAlign(TMGR0, 0, CKEN_CFG_EPI);
	}
	else if(PANEL_GetDisplayType()== PANEL_DISPLAY_TYPE_MLVDS)
	{
	 vIO32WriteFldAlign(TMGR0, 1, CKEN_CFG_TCON);
	 vIO32WriteFldAlign(TMGR0, 1, CKEN_CFG_MLVDS);
	 vIO32WriteFldAlign(TMGR0, 0, CKEN_CFG_VB1);
	 vIO32WriteFldAlign(TMGR0, 0, CKEN_CFG_EPI);	
	}
	else if(PANEL_GetDisplayType()== PANEL_DISPLAY_TYPE_VB1)
	{
	 vIO32WriteFldAlign(TMGR0, 0, CKEN_CFG_TCON);
	 vIO32WriteFldAlign(TMGR0, 1, CKEN_CFG_MLVDS);
	 vIO32WriteFldAlign(TMGR0, 1, CKEN_CFG_VB1);
	 vIO32WriteFldAlign(TMGR0, 0, CKEN_CFG_EPI);	
	}	 
	else if(PANEL_GetDisplayType()== PANEL_DISPLAY_TYPE_EPI)
	{
	 vIO32WriteFldAlign(TMGR0, 1, CKEN_CFG_TCON);
	 vIO32WriteFldAlign(TMGR0, 1, CKEN_CFG_MLVDS);
	 vIO32WriteFldAlign(TMGR0, 0, CKEN_CFG_VB1);
	 vIO32WriteFldAlign(TMGR0, 1, CKEN_CFG_EPI);
	}
	else if(PANEL_GetDisplayType()== PANEL_DISPLAY_TYPE_iDP)
	{
	}  

}

void vDrvVOPLLFreqSet(UINT32 u4Freq)
{
	PANEL_MINILVDS_ATTRIBUTE_T *pParam;
	if(PANEL_GetDisplayType()== PANEL_DISPLAY_TYPE_LVDS)
	{
		_u1TconMode = LVDS_MODE;
	}
	else if(PANEL_GetDisplayType()== PANEL_DISPLAY_TYPE_MLVDS)
	{

		_u1TconMode = MLVDS_MODE;
		if (PANEL_GetMlvdsParam(&pParam) == SV_TRUE)
		{
			if(pParam != NULL)
			{   
	 			MINI_LVDS_OUTPUT_PORT = pParam->u1Port;
	 			TCLK_PCLK_RATION = pParam->u1Ratio; 
	 			TCON_TCLK_RATION = pParam->u1TconRatio; 
			}
		}
	}
	else if(PANEL_GetDisplayType()== PANEL_DISPLAY_TYPE_VB1)
	{
		_u1TconMode = VB1_MODE;
	}    
	else if(PANEL_GetDisplayType()== PANEL_DISPLAY_TYPE_EPI)
	{
		_u1TconMode = EPI_MODE;
	}
	else if(PANEL_GetDisplayType()== PANEL_DISPLAY_TYPE_iDP)
	{
		_u1TconMode = iDP_MODE;
	}  

	if (IO32ReadFldAlign(DDDS_08, FLAG_PANEL_TYPE) != _u1TconMode)
	{
	    vIO32WriteFldAlign(DDDS_08, 0, FLAG_VOPLL_INITIAL);
	    fgVOPLLInit = FALSE;       
	    vIO32WriteFldAlign(DDDS_08, 0, FLAG_VOPLL_POWERON);
		_fgVOPLLPowerOn = FALSE;
		vIO32WriteFldAlign(DDDS_08, _u1TconMode, FLAG_PANEL_TYPE);
	}

	vSetVOPLLFreq(u4Freq);
	vPanelPowerSaving();  
}

void vDrvMLDSVOPLLFreqSet(UINT32 u4Freq, UINT8 MLVDS_PORT,UINT8 MLVDS_RATION)
{
	_u1TconMode = MLVDS_MODE;
	MINI_LVDS_OUTPUT_PORT=MLVDS_PORT;
	TCLK_PCLK_RATION=MLVDS_RATION;
	#ifdef VOPLL_Debug 
	Printf("[LVDS] MINI_LVDS_OUTPUT_PORT=%d \n",MINI_LVDS_OUTPUT_PORT);     
	Printf("[LVDS] TCLK_PCLK_RATION=%d \n",TCLK_PCLK_RATION);     
	#endif  
	vSetVOPLLFreq(u4Freq);
	}

void vDrvVB1VOPLLFreqSet(UINT32 u4Freq)
{
	_u1TconMode = VB1_MODE;
	vSetVOPLLFreq(u4Freq);
}

void vDrvEPIVOPLLFreqSet(UINT32 u4Freq)
{
	_u1TconMode = EPI_MODE;
	vSetVOPLLFreq(u4Freq);
}

void vDrviDPVOPLLFreqSet(UINT32 u4Freq)
{
	_u1TconMode = iDP_MODE;
	vSetVOPLLFreq(u4Freq);
}

void vDrvVOPLLStatus(void)
{
    UINT32 u4Syspll, u4cw, u4dclock, VCO_clk, PPLL_VCO_clk, DPIX_clk, CTS_clk,u4HFreq_ext, u4ErrorLimit_Range;
    UINT8 u1MutFactor1,u1MutFactor2, u1DivFactor1, u1DivFactor2, u1DivFactor4, u1DivFactor5;
    UINT16 u2DivFactor3;    

    Printf("==[TconMode]==\n");
	Printf("_u1TconMode = %d ,",_u1TconMode); 
	if (_u1TconMode == LVDS_MODE)
    Printf("[0]LVDS \n"); 	
	else if (_u1TconMode == MLVDS_MODE)
    Printf("[1]MLVDS \n"); 		
	else if (_u1TconMode == VB1_MODE)
    Printf("[2]VB1 \n"); 		
	else if (_u1TconMode == EPI_MODE)
    Printf("[3]EPI \n"); 		
	else if (_u1TconMode == iDP_MODE)
    Printf("[4]iDP \n"); 		

    Printf("==[Freq Setting]==\n");  
    Printf("u4Freq=%d \n",_u4SetVopllFreq);
	
	if(_fgVopllUseDDDS)
	{	
    Printf("==[DVOPLL]==\n");  
    u4cw = IO32ReadFldAlign(DDDS_00,DDDS_00_DDDS_FREQ_CW);
    Printf("Close loop=%d\n",IO32ReadFldAlign(DDDS_00,DDDS_00_DISP_EN));
    Printf("CW=0x%7x, CWS=0x%7x, Error Limit=0x%3x\n",u4cw,IO32ReadFldAlign(STA_DDDS_00,STA_DDDS_00_DDDS_FREQ_CW),IO32ReadFldAlign(DDDS_03,DDDS_03_DDDS_ERR_LIM));
    Printf("Lock Status=%d, Overflow Status=%d\n",IO32ReadFldAlign(STA_DDDS_00,STA_DDDS_00_DDDS_LOCK)
                                                 ,IO32ReadFldAlign(STA_DDDS_00,STA_DDDS_00_FIFO_OFLOW));        
    u4Syspll = _CalGetPLLCounter(syspll_d2_ck)*2000; //SYSPLL = 459MHz in MT5395 //CNTseng check
    Printf("Syspll=%d \n",u4Syspll);         
    u4dclock = u8Div6432(((UINT64)u4Syspll << VPLL_SPREAD_INT_SHIFT), u4cw, NULL);
    Printf("DVOPLL Clock=%d Hz \n",u4dclock);     		
    u4HFreq_ext = ((IO32ReadFldAlign(DDDS_02,DDDS_02_HLEN_DEN) * IO32ReadFldAlign(DDDS_01,DDDS_01_HLEN_INT)) + IO32ReadFldAlign(DDDS_01,DDDS_01_HLEN_NUM)) * 10 / IO32ReadFldAlign(DDDS_02,DDDS_02_HLEN_DEN);
	#if DDDS_OUT_DIV2
	u4dclock = u4dclock >> 1;
	#endif
    Printf("DVOPLL Clock_FB_DIV=%d Hz \n", u4dclock); 	
	u4HFreq_ext = u4dclock / u4HFreq_ext * 10;
    Printf("HFreq_ext_input=%d Hz \n",u4HFreq_ext);
	if (IO32ReadFldAlign(DDDS_00,DDDS_00_DISP_EN))
	{
	  u4cw = IO32ReadFldAlign(STA_DDDS_00,STA_DDDS_00_DDDS_FREQ_CW);
	  u4dclock = u8Div6432(((UINT64)u4Syspll << VPLL_SPREAD_INT_SHIFT), u4cw, NULL);
	  u4HFreq_ext = ((IO32ReadFldAlign(DDDS_02,DDDS_02_HLEN_DEN) * IO32ReadFldAlign(DDDS_01,DDDS_01_HLEN_INT)) + IO32ReadFldAlign(DDDS_01,DDDS_01_HLEN_NUM)) * 10 / IO32ReadFldAlign(DDDS_02,DDDS_02_HLEN_DEN);
	  #if DDDS_OUT_DIV2
	  u4dclock = u4dclock >> 1;
	  #endif	  
	  u4HFreq_ext = u4dclock / u4HFreq_ext * 10;
      Printf("HFreq_int_result=%d Hz \n",u4HFreq_ext);	  
	}
	  u4ErrorLimit_Range = (IO32ReadFldAlign(DDDS_03,DDDS_03_DDDS_ERR_LIM) << 16) * 1000 / IO32ReadFldAlign(STA_DDDS_00,STA_DDDS_00_DDDS_FREQ_CW);
      Printf("ErrorLimit_Range (Permillage)= %d (0/00) \n",u4ErrorLimit_Range);	
	}
	else
	{
     u4dclock = 27000;
	 Printf("PLL Source : Xtal \n");     
	}

    // 519.75 = 74.25 / 2 * 1 *14
    u1DivFactor1 = IO32ReadFldAlign(REG_VOPLL_CFG0,RG_VPLL_PREDIV);    
    if (u1DivFactor1==0)
    u1DivFactor1 = 1;
    else if (u1DivFactor1==1)
    u1DivFactor1 = 2;    
    else if (u1DivFactor1==3)
    u1DivFactor1 = 4;    

    u1MutFactor1 = IO32ReadFldAlign(REG_VOPLL_CFG0, RG_VPLL_FBKSEL);

    if (u1MutFactor1 == 0)
    u1MutFactor1 = 10;  // 1.0 * 10
    else if (u1MutFactor1==1)
	u1MutFactor1 = 20;	// 2.0 * 10
    else
    u1MutFactor1 = 40;  // 4.0 * 10

    u1MutFactor2 = IO32ReadFldAlign(REG_VOPLL_CFG0, RG_VPLL_FBKDIV)+1;

    VCO_clk = u4dclock/u1DivFactor1*u1MutFactor1/1000*u1MutFactor2/10;

    Printf("==[AVOPLL]==\n"); 
    Printf("u1DivFactor1=%d, u1MutFactor1=%d,u1MutFactor2=%d \n",u1DivFactor1,u1MutFactor1,u1MutFactor2);         
    Printf("VCO Clock=%d KHz\n",VCO_clk);

    u4dclock = u4dclock/u1DivFactor1; //VPLL_PREDIV
    u1DivFactor1 = IO32ReadFldAlign(REG_VOPLL_CFG3,RG_PPLL_PREDIV);    
    if (u1DivFactor1==0)
    u1DivFactor1 = 1;
    else if (u1DivFactor1==1)
    u1DivFactor1 = 2;    
    else if (u1DivFactor1==3)
    u1DivFactor1 = 4;    

    u1MutFactor1 = IO32ReadFldAlign(REG_VOPLL_CFG3, RG_PPLL_FBKSEL);

    if (u1MutFactor1 == 0)
    u1MutFactor1 = 10;  // 1.0 * 10
    else if (u1MutFactor1==1)
	u1MutFactor1 = 20;	// 2.0 * 10
    else
    u1MutFactor1 = 40;  // 4.0 * 10

    u1MutFactor2 = IO32ReadFldAlign(REG_VOPLL_CFG3, RG_PPLL_FBKDIV)+1;

    PPLL_VCO_clk = u4dclock/u1DivFactor1*u1MutFactor1/1000*u1MutFactor2/10;

    Printf("==[AVOPLL]==\n"); 
    Printf("u1DivFactor1=%d, u1MutFactor1=%d,u1MutFactor2=%d \n",u1DivFactor1,u1MutFactor1,u1MutFactor2);         
    Printf("PPLL VCO Clock=%d KHz\n",PPLL_VCO_clk);
	#ifndef CONFIG_TV_DRV_VFY
    Printf("[VOPLLdbg]_u1TconMode = %d, panelID = %d, VPLL = %d, PPLL = %d\n",_u1TconMode,GetCurrentPanelIndex(),VCO_clk,PPLL_VCO_clk); 	
	#endif
    if (_u1TconMode == LVDS_MODE)
    {
     u1DivFactor2 = IO32ReadFldAlign(REG_VOPLL_CFG1, RG_VPLL_TXCLK_DIV);
     u2DivFactor3 = IO32ReadFldAlign(REG_VOPLL_CFG1, RG_VPLL_LVDS_DPIX_DIV2); 
     u1DivFactor4 = 1;
     u1DivFactor5 = IO32ReadFldAlign(REG_VOPLL_CFG1, RG_VPLL_TXCLK_DIV2);

     if (u2DivFactor3 == 0)
     u2DivFactor3 = 175; // 1.75*100
     else
     u2DivFactor3 = 350; // 3.5*100     

     if (u1DivFactor5 == 1)
     u1DivFactor5 = 2;
     else
     u1DivFactor5 = 1;
     
    }
    else
    {
      u1DivFactor2 = IO32ReadFldAlign(REG_VOPLL_CFG3, RG_PPLL_POSDIV);    
      u2DivFactor3 = IO32ReadFldAlign(REG_VOPLL_CFG3, RG_PPLL_POSDIV2);      
	  if (u2DivFactor3==0)
		  u2DivFactor3 = 1;
	  else if (u2DivFactor3==1)
		  u2DivFactor3 = 2;
	  else if (u2DivFactor3==2)
		  u2DivFactor3 = 4;
	  else
		  u2DivFactor3 = 8;
      u1DivFactor4 = 1;
      u1DivFactor5 = 1;
    }

    if (u1DivFactor2==0)
    u1DivFactor2 = 1;
    else if (u1DivFactor2==1)
    u1DivFactor2 = 2;    
    else
    u1DivFactor2 = 4;  
   
    if (_u1TconMode == LVDS_MODE)
	    DPIX_clk = VCO_clk / u1DivFactor2 / u2DivFactor3 /u1DivFactor4 * 100;
	else
	    DPIX_clk = PPLL_VCO_clk / u1DivFactor2 / u2DivFactor3 /u1DivFactor4 * 100;
    Printf("u1DivFactor2=%d, u2DivFactor3=%d, u1DivFactor4=%d\n",u1DivFactor2,u2DivFactor3,u1DivFactor4);    
    Printf("DPIX_clk=%d KHz\n",DPIX_clk);

     CTS_clk = VCO_clk / 2;
     Printf("CTS_clk=%d KHz\n",CTS_clk);

    Printf("==[Clock Measure]==\n"); 

    if (_u1TconMode==MLVDS_MODE)
    {
     Printf("[MLVDS]_u1TconMode=0x%x \n",_u1TconMode);
     Printf("[LVDS] TCLK_PCLK_RATION=%d \n",TCLK_PCLK_RATION);      
     Printf("[MLVDS]TCLK_PCLK_RATION_NUM=0x%x \n",TCLK_PCLK_RATION_NUM);
     Printf("[MLVDS]TCLK_PCLK_RATION_DEN=0x%x \n",TCLK_PCLK_RATION_DEN);     
     Printf("[MLVDS]_u1VCO32PHASE_FREQ=0x%x \n",_u1VCO32PHASE_FREQ);
    } 
 
    Printf("[MLVDS]AD_DDDS1_VSP=%d \n",_CalGetPLLCounter(AD_DDDS1_VSP));
    Printf("[MLVDS]AD_DDDS2_VSP=%d \n",_CalGetPLLCounter(AD_DDDS2_VSP));     

    Printf("[LVDS]AD_VPLL_TCON_CK=%d \n",_CalGetPLLCounter(AD_VPLL_TCON_CK));
    Printf("[LVDS]AD_LVDS_DPIX_CK=%d \n",_CalGetPLLCounter(AD_LVDS_DPIX_CK));
    Printf("[LVDS]AD_PPLL_DPIX_CK=%d \n",_CalGetPLLCounter(AD_PPLL_DPIX_CK));

    Printf("[LVDS]AD_CLKDIG_CTS_D11=%d \n",_CalGetPLLCounter(AD_CLKDIG_CTS_D11));
    Printf("[LVDS]tclk=%d \n",_CalGetPLLCounter(DBGCKSEL_tclk));
    Printf("[LVDS]tclk_div=%d \n",_CalGetPLLCounter(DBGCKSEL_tclk_div));
    Printf("[LVDS]oclk=%d \n",_CalGetPLLCounter(DBGCKSEL_oclk));
    Printf("[LVDS]oclk_div=%d \n",_CalGetPLLCounter(DBGCKSEL_oclk_div));
    Printf("[LVDS]oclk_div2=%d \n",_CalGetPLLCounter(DBGCKSEL_oclk_div2));
    Printf("[LVDS]lvds_clk_cts=%d \n",_CalGetPLLCounter(DBGCKSEL_lvds_clk_cts));
    Printf("[LVDS]mon_clk75m_ck=%d \n",_CalGetPLLCounter(DBGCKSEL_mon_clk75m_ck));

	Printf("\n");	 	
	Printf("[VOPLL]TCLK_PCLK_RATION =%d \n",TCLK_PCLK_RATION);
    Printf("[VOPLL]MINI_LVDS_OUTPUT_PORT =%d \n",MINI_LVDS_OUTPUT_PORT);

	Printf("[VOPLL]LVDS_OUTPUT_PORT =%d \n",LVDS_OUTPUT_PORT);
	Printf("[VOPLL]_u1DddsCWMul =%d \n",_u1DddsCWMul);
	if(IS_LVDS_HIGHSPEED)
	Printf("[VOPLL]IS_LVDS_HIGHSPEED==1\n");
	else
	Printf("[VOPLL]IS_LVDS_HIGHSPEED==0\n");

	#ifdef VOPLL_Debug
    #if VOPLL_Para_Chk
	  Printf("%d \n",IO32ReadFldAlign(REG_VOPLL_CFG0,RG_VPLL_FBKDIV));				
	  Printf("%d \n",IO32ReadFldAlign(REG_VOPLL_CFG0,RG_VPLL_PREDIV));				
	  Printf("%d \n",IO32ReadFldAlign(REG_VOPLL_CFG0,RG_VPLL_FBKSEL));				
	  Printf("%d \n",IO32ReadFldAlign(REG_VOPLL_CFG0,RG_VPLL_POSDIV));				
	  Printf("%d \n",IO32ReadFldAlign(REG_VOPLL_CFG0,RG_VPLL_REFSEL));				
																											   
	  Printf("%d \n",IO32ReadFldAlign(REG_VOPLL_CFG1,RG_VPLL_LVDSEN));				  
	  Printf("%d \n",IO32ReadFldAlign(REG_VOPLL_CFG1,RG_VPLL_VB1EN));				   
	  Printf("%d \n",IO32ReadFldAlign(REG_VOPLL_CFG1,RG_VPLL_MLVDSEN)); 			 
	  Printf("%d \n",IO32ReadFldAlign(REG_VOPLL_CFG1,RG_VPLL_EPIEN));				   
	  Printf("%d \n",IO32ReadFldAlign(REG_VOPLL_CFG1,RG_VPLL_IDPEN));				   
	  Printf("%d \n",IO32ReadFldAlign(REG_VOPLL_CFG1,RG_VPLL_LVDSCHL_EN));		  
	  Printf("%d \n",IO32ReadFldAlign(REG_VOPLL_CFG1,RG_VPLL_LVDS_DPIX_DIV2));
	  Printf("%d \n",IO32ReadFldAlign(REG_VOPLL_CFG1,RG_VPLL_TXCLK_DIV));		 
	  Printf("%d \n",IO32ReadFldAlign(REG_VOPLL_CFG1,RG_VPLL_TXCLK_DIV2));		
	  Printf("%d \n",IO32ReadFldAlign(REG_VOPLL_CFG1,RG_VPLL_AUTOK_CTRL));		
	  Printf("%d \n",IO32ReadFldAlign(REG_VOPLL_CFG1,RG_VPLL_BAND));				  
	  Printf("%d \n",IO32ReadFldAlign(REG_VOPLL_CFG1,RG_VPLL_AUTOK_EN));			
	  Printf("%d \n",IO32ReadFldAlign(REG_VOPLL_CFG1,RG_VPLL_AUTOK_LOAD));		  
	  Printf("%d \n",IO32ReadFldAlign(REG_VOPLL_CFG1,RG_VPLL_VODEN));				   
	  Printf("%d \n",IO32ReadFldAlign(REG_VOPLL_CFG1,RG_VPLL_DLY_DATA));		  
																											   
	  Printf("%d \n",IO32ReadFldAlign(REG_VOPLL_CFG2,RG_VPLL_DLY_CLKA));		  
	  Printf("%d \n",IO32ReadFldAlign(REG_VOPLL_CFG2,RG_VPLL_DLY_CLKB));		  
	  Printf("%d \n",IO32ReadFldAlign(REG_VOPLL_CFG2,RG_VPLL_BIC));				   
	  Printf("%d \n",IO32ReadFldAlign(REG_VOPLL_CFG2,RG_VPLL_BC));						
	  Printf("%d \n",IO32ReadFldAlign(REG_VOPLL_CFG2,RG_VPLL_DIVEN));				 
	  Printf("%d \n",IO32ReadFldAlign(REG_VOPLL_CFG2,RG_VPLL_BIR)); 				   
	  Printf("%d \n",IO32ReadFldAlign(REG_VOPLL_CFG2,RG_VPLL_BP));						
	  Printf("%d \n",IO32ReadFldAlign(REG_VOPLL_CFG2,RG_VPLL_BR));						
	  Printf("%d \n",IO32ReadFldAlign(REG_VOPLL_CFG2,RG_VPLL_FPEN));					
	  Printf("%d \n",IO32ReadFldAlign(REG_VOPLL_CFG2,RG_VPLL_FMEN));					
																											   
	  Printf("%d \n",IO32ReadFldAlign(REG_VOPLL_CFG3,RG_VPLL_RESERVE)); 		   
	  Printf("%d \n",IO32ReadFldAlign(REG_VOPLL_CFG3,RG_PPLL_FBKDIV));				
	  Printf("%d \n",IO32ReadFldAlign(REG_VOPLL_CFG3,RG_PPLL_PREDIV));				
	  Printf("%d \n",IO32ReadFldAlign(REG_VOPLL_CFG3,RG_PPLL_FBKSEL));				
	  Printf("%d \n",IO32ReadFldAlign(REG_VOPLL_CFG3,RG_PPLL_POSDIV));				
	  Printf("%d \n",IO32ReadFldAlign(REG_VOPLL_CFG3,RG_PPLL_POSDIV2)); 		   
																											   
	  Printf("%d \n",IO32ReadFldAlign(REG_VOPLL_CFG4,RG_PPLL_REFSEL));				
	  Printf("%d \n",IO32ReadFldAlign(REG_VOPLL_CFG4,RG_PPLL_DIVEN));				 
	  Printf("%d \n",IO32ReadFldAlign(REG_VOPLL_CFG4,RG_PPLL_BP));						  
	  Printf("%d \n",IO32ReadFldAlign(REG_VOPLL_CFG4,RG_PPLL_BR));						  
	  Printf("%d \n",IO32ReadFldAlign(REG_VOPLL_CFG4,RG_PPLL_LF));						  
	  Printf("%d \n",IO32ReadFldAlign(REG_VOPLL_CFG4,RG_PPLL_VODEN));				   

	#else
	
	Printf("[VOPLL]RG_VPLL_FBKDIV[6:0]=%d \n",IO32ReadFldAlign(REG_VOPLL_CFG0,RG_VPLL_FBKDIV));
	Printf("[VOPLL]RG_VPLL_PREDIV[1:0]=%d \n",IO32ReadFldAlign(REG_VOPLL_CFG0,RG_VPLL_PREDIV));
	Printf("[VOPLL]RG_VPLL_FBKSEL[1:0]=%d \n",IO32ReadFldAlign(REG_VOPLL_CFG0,RG_VPLL_FBKSEL));
	Printf("[VOPLL]RG_VPLL_POSDIV[1:0]=%d \n",IO32ReadFldAlign(REG_VOPLL_CFG0,RG_VPLL_POSDIV));
	Printf("[VOPLL]RG_VPLL_REFSEL[1:0]=%d \n",IO32ReadFldAlign(REG_VOPLL_CFG0,RG_VPLL_REFSEL));
	
	Printf("[VOPLL]RG_VPLL_LVDSEN[0]=%d \n",IO32ReadFldAlign(REG_VOPLL_CFG1,RG_VPLL_LVDSEN));
	Printf("[VOPLL]RG_VPLL_VB1EN[0]=%d \n",IO32ReadFldAlign(REG_VOPLL_CFG1,RG_VPLL_VB1EN));
	Printf("[VOPLL]RG_VPLL_MLVDSEN[0]=%d \n",IO32ReadFldAlign(REG_VOPLL_CFG1,RG_VPLL_MLVDSEN));
	Printf("[VOPLL]RG_VPLL_EPIEN[0]=%d \n",IO32ReadFldAlign(REG_VOPLL_CFG1,RG_VPLL_EPIEN));
	Printf("[VOPLL]RG_VPLL_IDPEN[0]=%d \n",IO32ReadFldAlign(REG_VOPLL_CFG1,RG_VPLL_IDPEN));
	Printf("[VOPLL]RG_VPLL_LVDSCHL_EN[0]=%d \n",IO32ReadFldAlign(REG_VOPLL_CFG1,RG_VPLL_LVDSCHL_EN));
	Printf("[VOPLL]RG_VPLL_LVDS_DPIX_DIV2[0]=%d \n",IO32ReadFldAlign(REG_VOPLL_CFG1,RG_VPLL_LVDS_DPIX_DIV2));
	Printf("[VOPLL]RG_VPLL_TXCLK_DIV[1:0]=%d \n",IO32ReadFldAlign(REG_VOPLL_CFG1,RG_VPLL_TXCLK_DIV));
	Printf("[VOPLL]RG_VPLL_TXCLK_DIV2[1:0]=%d \n",IO32ReadFldAlign(REG_VOPLL_CFG1,RG_VPLL_TXCLK_DIV2));
	Printf("[VOPLL]RG_VPLL_AUTOK_CTRL[1:0]=%d \n",IO32ReadFldAlign(REG_VOPLL_CFG1,RG_VPLL_AUTOK_CTRL));
	Printf("[VOPLL]RG_VPLL_BAND[5:0]=%d \n",IO32ReadFldAlign(REG_VOPLL_CFG1,RG_VPLL_BAND));
	Printf("[VOPLL]RG_VPLL_AUTOK_EN[0]=%d \n",IO32ReadFldAlign(REG_VOPLL_CFG1,RG_VPLL_AUTOK_EN));
	Printf("[VOPLL]RG_VPLL_AUTOK_LOAD[0]=%d \n",IO32ReadFldAlign(REG_VOPLL_CFG1,RG_VPLL_AUTOK_LOAD));
	Printf("[VOPLL]RG_VPLL_VODEN[0]=%d \n",IO32ReadFldAlign(REG_VOPLL_CFG1,RG_VPLL_VODEN));
	Printf("[VOPLL]RG_VPLL_DLY_DATA[3:0]=%d \n",IO32ReadFldAlign(REG_VOPLL_CFG1,RG_VPLL_DLY_DATA));
	
	Printf("[VOPLL]RG_VPLL_DLY_CLKA[3:0]=%d \n",IO32ReadFldAlign(REG_VOPLL_CFG2,RG_VPLL_DLY_CLKA));
	Printf("[VOPLL]RG_VPLL_DLY_CLKB[3:0]=%d \n",IO32ReadFldAlign(REG_VOPLL_CFG2,RG_VPLL_DLY_CLKB));
    Printf("[VOPLL]RG_VPLL_BIC[2:0]=%d \n",IO32ReadFldAlign(REG_VOPLL_CFG2,RG_VPLL_BIC));
	Printf("[VOPLL]RG_VPLL_BC[1:0]=%d \n",IO32ReadFldAlign(REG_VOPLL_CFG2,RG_VPLL_BC));
	Printf("[VOPLL]RG_VPLL_DIVEN[2:0]=%d \n",IO32ReadFldAlign(REG_VOPLL_CFG2,RG_VPLL_DIVEN));
	Printf("[VOPLL]RG_VPLL_BIR[3:0]=%d \n",IO32ReadFldAlign(REG_VOPLL_CFG2,RG_VPLL_BIR));
	Printf("[VOPLL]RG_VPLL_BP[3:0]=%d \n",IO32ReadFldAlign(REG_VOPLL_CFG2,RG_VPLL_BP));
	Printf("[VOPLL]RG_VPLL_BR[3:0]=%d \n",IO32ReadFldAlign(REG_VOPLL_CFG2,RG_VPLL_BR));
	Printf("[VOPLL]RG_VPLL_FPEN[0]=%d \n",IO32ReadFldAlign(REG_VOPLL_CFG2,RG_VPLL_FPEN));
	Printf("[VOPLL]RG_VPLL_FMEN[0]=%d \n",IO32ReadFldAlign(REG_VOPLL_CFG2,RG_VPLL_FMEN));
	
	Printf("[VOPLL]RG_VPLL_RESERVE[7:0]=%d \n",IO32ReadFldAlign(REG_VOPLL_CFG3,RG_VPLL_RESERVE));
	Printf("[VOPLL]RG_PPLL_FBKDIV[6:0]=%d \n",IO32ReadFldAlign(REG_VOPLL_CFG3,RG_PPLL_FBKDIV));
	Printf("[VOPLL]RG_PPLL_PREDIV[1:0]=%d \n",IO32ReadFldAlign(REG_VOPLL_CFG3,RG_PPLL_PREDIV));
	Printf("[VOPLL]RG_PPLL_FBKSEL[1:0]=%d \n",IO32ReadFldAlign(REG_VOPLL_CFG3,RG_PPLL_FBKSEL));
	Printf("[VOPLL]RG_PPLL_POSDIV[1:0]=%d \n",IO32ReadFldAlign(REG_VOPLL_CFG3,RG_PPLL_POSDIV));
	Printf("[VOPLL]RG_PPLL_POSDIV2[1:0]=%d \n",IO32ReadFldAlign(REG_VOPLL_CFG3,RG_PPLL_POSDIV2));
	
	Printf("[VOPLL]RG_PPLL_REFSEL[1:0]=%d \n",IO32ReadFldAlign(REG_VOPLL_CFG4,RG_PPLL_REFSEL));
	Printf("[VOPLL]RG_PPLL_DIVEN[2:0]=%d \n",IO32ReadFldAlign(REG_VOPLL_CFG4,RG_PPLL_DIVEN));
	Printf("[VOPLL]RG_PPLL_BP[0]=%d \n",IO32ReadFldAlign(REG_VOPLL_CFG4,RG_PPLL_BP));
	Printf("[VOPLL]RG_PPLL_BR[0]=%d \n",IO32ReadFldAlign(REG_VOPLL_CFG4,RG_PPLL_BR));
	Printf("[VOPLL]RG_PPLL_LF[0]=%d \n",IO32ReadFldAlign(REG_VOPLL_CFG4,RG_PPLL_LF));
	Printf("[VOPLL]RG_PPLL_VODEN[0]=%d \n",IO32ReadFldAlign(REG_VOPLL_CFG4,RG_PPLL_VODEN));
	#endif
	#endif	
}

void vDDDSInit_uboot(void)
{
#ifdef CC_UBOOT

    vIO32Write4B(0xF000E004, 0xB0010898);
    vIO32Write4B(0xF000E008, 0x00010000);
    vIO32Write4B(0xF000E014, 0x00000000);
    vIO32Write4B(0xF000E00C, 0x80000000);
    vIO32Write4B(0xF000E014, 0x40000000);

	if(PANEL_GetDisplayType()== PANEL_DISPLAY_TYPE_EPI)
	{
	    vIO32Write4B(0xF000E000, 0x67AE147);
	}
	else
	{
        vIO32Write4B(0xF000E000, 0x45E353F);
	}

    vIO32Write4B(0xF000E008, 0x20010711);

    vIO32Write4B(0xF000E010, 0x00000000);

    vIO32WriteFldAlign(0xF000E000,1,Fld(1, 30, AC_MSKB3));
	
    vIO32WriteFldAlign(0xF000E000, 1, Fld(1, 31, AC_MSKB3));	
	HAL_Delay_us(30);
	vIO32WriteFldAlign(0xF000E000, 1, Fld(1, 29, AC_MSKB3));	

	HAL_Delay_us(30);
    vIO32WriteFldAlign(0xF000E000, 0, Fld(1, 31, AC_MSKB3));	
	vIO32WriteFldAlign(0xF000E000, 0, Fld(1, 29, AC_MSKB3));	

    vIO32Write4B(0xF000E00C, 0x80000103);

    vIO32WriteFldAlign(0xF000D118, 0, Fld(2,0,AC_MSKB0));      

    vIO32Write4B(0xF000D3C0, 0x00000001);

    vIO32WriteFldAlign(DDDS_08, 1, FLAG_DDDS_INIT);
#endif	
}

void vDrvVOPLLDDDS_SS_TestMode(UINT8 u1Mode)
{
	#ifndef NDEBUG
	UINT8 u1BIC = 0, u1BIR = 0;
	
	switch(u1Mode)
	{
		case 1:
		u1BIC = 1;
		u1BIR = 2;
		break;

		case 2:
		u1BIC = 3;
		u1BIR = 3;
		break;

		case 3:
		u1BIC = 5;
		u1BIR = 5;
		break;

		case 4:
		u1BIC = 7;
		u1BIR = 7;
		break;

		default:
		u1BIC = 1;
		u1BIR = 1;
		break;			


	}

	vIO32WriteFldMulti(REG_VOPLL_CFG2, P_Fld(u1BIC, RG_VPLL_BIC)|P_Fld(u1BIR, RG_VPLL_BIR));
	#endif
}

#ifdef CC_MT5882

#ifndef NDEBUG
#define PLL_FREQ_Debug
#endif

#ifdef PLL_FREQ_Debug
#define CAL_SRC_APLL2 0x5
#define AD_ARMPLL_CK1500M 0x5A
//#define SRC_DMUXPLL_MONCLK 0x56
#define SRC_G3DPLL_MONCLK  0x09
#define SRC_HPCPLL_MONCLK 0x01
#define SRC_SYSPLL_CLK 0x02
#define SRC_VDECPLL_MONCLK 0x58
#define SRC_SAWLESSPLL_CLK 0x0
#define SRC_TAPLL_CLK 0x64
#define AD_DMUXPLL_MONCLK 0x49
#define AD_DMUXPLL_CKOUT 0x4C
#endif

void vDrvChkPLLFreq(void)
{	
	#ifdef PLL_FREQ_Debug
	vIO32WriteFldAlign(REG_ARMPLL_CFG0, 0x1, FLD_RG_ARMPLL_MONCKEN);
	vIO32WriteFldAlign(REG_ARMPLL_CFG1, 0x0, FLD_RG_ARMPLL_TST_PD);
	// arm from xtal
	// vIO32WriteFldAlign(REG_ARMPLL_CFG0, 0x1, FLD_RG_ARMPLL_REFCK_MON_EN);
	// vIO32WriteFldAlign(REG_ARMPLL_CFG1, 0x4, FLD_RG_ARMPLL_TSTCK_MONSEL);
	Printf("[PLL]AD_ARMPLL_CK1500M=%d \n",_CalGetPLLCounter(AD_ARMPLL_CK1500M));	
	Printf("[PLL]CAL_SRC_G3DPLL_MONCLK=%d \n",_CalGetPLLCounter(SRC_G3DPLL_MONCLK));
	Printf("[PLL]CAL_SRC_HPCPLL_MONCLK=%d \n",_CalGetPLLCounter(SRC_HPCPLL_MONCLK));
	Printf("[PLL]SRC_SYSPLL_CLK=%d \n",_CalGetPLLCounter(SRC_SYSPLL_CLK));	
	Printf("[PLL]SRC_TAPLL_CLK=%d \n",_CalGetPLLCounter(SRC_TAPLL_CLK));		
	Printf("[PLL]SRC_SAWLESSPLL_CLK=%d \n",_CalGetPLLCounter(SRC_SAWLESSPLL_CLK));
	vIO32WriteFldAlign(REG_DMUXPLL_CFG0, 0x0, FLD_RG_DMUXPLL_PWD);
	vIO32WriteFldAlign(REG_DMUXPLL_CFG0, 0x0, FLD_RG_DMUXPLL_BIAS_PWD);	
	vIO32WriteFldAlign(REG_DMUXPLL_CFG1, 0x1, FLD_RG_DMUXPLL_MONCKEN);
	Printf("[PLL]AD_DMUXPLL_MONCLK=%d \n",_CalGetPLLCounter(AD_DMUXPLL_MONCLK));
	Printf("[PLL]AD_DMUXPLL_CKOUT=%d \n",_CalGetPLLCounter(AD_DMUXPLL_CKOUT));
	#endif
}


void vPLLVCOMonOff(void)
{
	#ifdef PLL_FREQ_Debug
	vIO32WriteFldAlign(REG_PLL_GROUP_CFG5, 0, FLD_RG_G3DPLL_MONEN);	
	vIO32WriteFldAlign(REG_PLL_GROUP_CFG2, 0, FLD_RG_SYSPLL_MONEN);			
	vIO32WriteFldAlign(REG_ARMPLL_CFG0, 0, FLD_RG_ARMPLL_MONVCEN);
	vIO32WriteFldAlign(REG_HPCPLL_CFG0, 0x0, FLD_RG_HPCPLL_MONEN);
	vIO32WriteFldAlign(REG_IFPLL_CFG1, 0, FLD_RG_IFPLL_MONVC_EN);
	vIO32WriteFldAlign(REG_PLL_GROUP_CFG0, 0x0, FLD_RG_TAPLL_MONEN);
	#endif
}

#define RG_PDREV_0 Fld(8,0,AC_FULLB0)//[7:0]

void vDrvPLLVCO_TestMode(UINT8 u1Mode)
{
	#ifdef PLL_FREQ_Debug
	vPLLVCOMonOff();
	vIO32WriteFldAlign(REG_PLL_GROUP_CFG10, 0, FLD_RG_PLLGP_TSTEN);
	HAL_Delay_us(400);
	switch(u1Mode)
	{
		case 1:

		break;

		case 2:

		break;

		case 3:
			vIO32WriteFldAlign(0xf0028900, 0x21, RG_PDREV_0);				
			vIO32WriteFldAlign(REG_PLL_GROUP_CFG5, 1, FLD_RG_G3DPLL_MONEN);
			vIO32WriteFldAlign(REG_PLL_GROUP_CFG10, 1, FLD_RG_PLLGP_TSTEN);
			vIO32WriteFldAlign(REG_PLL_GROUP_CFG10, 1, FLD_RG_PLLGP_TSTCKENB);
			vIO32WriteFldAlign(REG_PLL_GROUP_CFG10, 2, FLD_RG_PLLGP_TSTSEL);
			Printf("G3DPLL VCO Control voltage\n");
		break;

		case 4:
			vIO32WriteFldAlign(0xf0028900, 0x21, RG_PDREV_0);			
			vIO32WriteFldAlign(REG_PLL_GROUP_CFG2, 1, FLD_RG_SYSPLL_MONEN);
			vIO32WriteFldAlign(REG_PLL_GROUP_CFG10, 1, FLD_RG_PLLGP_TSTEN);
			vIO32WriteFldAlign(REG_PLL_GROUP_CFG10, 1, FLD_RG_PLLGP_TSTCKENB);
			vIO32WriteFldAlign(REG_PLL_GROUP_CFG10, 3, FLD_RG_PLLGP_TSTSEL);
			Printf("syspll VCO Control voltage\n");
		break;

		case 5:

		break;

		case 6:
			vIO32WriteFldAlign(REG_ARMPLL_CFG0, 0x1, FLD_RG_ARMPLL_MONVCEN);
			vIO32WriteFldAlign(REG_ARMPLL_CFG1, 0x1, FLD_RG_MPX_SEL);			
			vIO32WriteFldAlign(REG_ARMPLL_CFG1, 0x1, FLD_RG_MPX_EN);								
			vIO32WriteFldAlign(REG_ARMPLL_CFG0, 0x0, FLD_RG_ARMPLL_TSTCK_EN);
			vIO32WriteFldAlign(REG_ARMPLL_CFG1, 0x0, FLD_RG_ARMPLL_TST_PD);			
			Printf("ARMCPLL VCO Control voltage\n");
		break;

		case 7:
			vIO32WriteFldAlign(REG_HPCPLL_CFG0, 0x1, FLD_RG_HPCPLL_MONEN);			
			vIO32WriteFldAlign(REG_HPCPLL_CFG0, 0x0, FLD_RG_HPCPLL_SEL_CK);
			vIO32WriteFldAlign(REG_HPCPLL_CFG0, 0x0, FLD_RG_HPCPLL_BRCKMON_PD);
			Printf("HPCPLL VCO Control voltage\n");
		break;

		case 8:
			vIO32WriteFldAlign(REG_IFPGA_CFG0, 0x1, FLD_RG_IFPGA_TESTE);
			vIO32WriteFldAlign(REG_IFPGA_CFG1, 0x4, FLD_RG_IFTQI_MON);
			vIO32WriteFldAlign(0xf0028900, 0x25, RG_PDREV_0);
			vIO32WriteFldAlign(REG_IFPLL_CFG1, 0x1, FLD_RG_IFPLL_MONVC_EN);
			Printf("IFPLL(SAWLESSPLL) VCO Control voltage\n");
		break;

		case 9:
			vIO32WriteFldAlign(0xf0028900, 0x21, RG_PDREV_0);			
			vIO32WriteFldAlign(REG_PLL_GROUP_CFG0, 1, FLD_RG_TAPLL_MONEN);
			vIO32WriteFldAlign(REG_PLL_GROUP_CFG10, 1, FLD_RG_PLLGP_TSTEN);
			vIO32WriteFldAlign(REG_PLL_GROUP_CFG10, 1, FLD_RG_PLLGP_TSTCKENB);
			vIO32WriteFldAlign(REG_PLL_GROUP_CFG10, 4, FLD_RG_PLLGP_TSTSEL);		
			Printf("TAPLL VCO Control voltage\n");
		break;		
		default:

		break;			


	}	
	#endif
}
#endif

#if defined(CC_FAST_INIT) || defined(CC_SUPPORT_STR)
void VOPLL_pm_resume(void)    
{
    fgVOPLLInit = FALSE;
	_fgVOPLLPowerOn = FALSE;
}
#endif

#if defined(CONFIG_OPM) || defined(CC_SUPPORT_STR)
extern UINT8 fgDDDSInit;
extern UINT8 fgLVDSonoff;
extern BOOL fgLVDSInit;

void vDrvVOPLLSuspend(void)
{
    //clear power on/init flag
    vIO32WriteFldAlign(DDDS_08, 0, FLAG_VOPLL_POWERON);	
    vIO32WriteFldAlign(DDDS_08, 0, FLAG_VOPLL_INITIAL);	
    vIO32WriteFldAlign(DDDS_08, 0, FLAG_LVDS_INITIAL);	
    vIO32WriteFldAlign(DDDS_08, 0, FLAG_LVDS_ONOFF);	
    vIO32WriteFldAlign(DDDS_08, 0, FLAG_DDDS_INIT);	
    _fgVOPLLPowerOn=FALSE;
    fgVOPLLInit = FALSE;
#ifndef CC_MTK_LOADER
    fgLVDSInit=FALSE;
    fgLVDSonoff=FALSE;
#endif
    fgDDDSInit=FALSE;

    //VOPLL power down
    vIO32WriteFldAlign(REG_VOPLL_CFG0, 1, RG_VPLL_PWD);
    
}
void vDrvVOPLLResume(void)
{
    //VOPLL power on
    vIO32WriteFldAlign(REG_VOPLL_CFG0, 0, RG_VPLL_PWD);

}
#endif
