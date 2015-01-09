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
 * $RCSfile: drv_lvds.c,v $
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
#include "hw_lvds.h"
#include "hw_mlvds.h"
#include "hw_epi.h"
#include "hw_ospe.h"
#include "drv_lvds.h"
#include "drv_display.h"
#include "drv_vdoclk.h"
#include "x_timer.h"
#include "nptv_debug.h"
#ifndef CC_COPLAT_MT82
#include "drvcust_if.h"
#include "pmx_drvif.h"
#include "osd_drvif.h"
#include "vdp_if.h"
#endif
#include "x_ckgen.h"
#include "x_gpio.h"


// switch the source of both group b and group c from internal group b
//#define CC_LVDS_WA1


//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------
#ifdef DRV_SUPPORT_EXTMJC
static UINT32 _u4CurrentLvdsMode = (UINT32)eLvdsVidNormal;
static UINT32 _u4LvdsColorDomain = eColorDomainRGB;
#endif
static UINT32 _u4APadPDValue=0;
#ifdef CC_MT5399
UINT32  LVDSTVO=0;
#endif

//automation
enum
{
  LVDS_PWON,
  LVDS_PWDOWN
};

UINT32 u2LVDSFifoSkwPre = 0;

//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------

static void _vDrvLVDSSetPortMap(void)
{
    // Lvds control : 
    UINT32 u4LvdsCtrlAEven = 0;
    UINT32 u4LvdsCtrlAOdd = 1;
    #ifndef CC_LVDS_WA1
    UINT32 u4LvdsCtrlBEven = 2;
    #endif
    UINT32 u4LvdsCtrlBOdd = 3;
    UINT32 au4PortRotte[4] = {0, 1, 2, 3};

    // panel specific port rotate
    if (LVDS_USE_INDEPENDENT_SETTING)
    {
        au4PortRotte[0] = LVDS_A_INDEPENDENT_SETTING;
        au4PortRotte[1] = LVDS_B_INDEPENDENT_SETTING;
        au4PortRotte[2] = LVDS_C_INDEPENDENT_SETTING;
        au4PortRotte[3] = LVDS_D_INDEPENDENT_SETTING;
    }

    // PCB layout rotate
    u4LvdsCtrlAEven = DRVCUST_PanelGet(eLvdsACtrlEven);
    u4LvdsCtrlAOdd = DRVCUST_PanelGet(eLvdsACtrlOdd);
    u4LvdsCtrlAEven = (u4LvdsCtrlAEven < 4)? au4PortRotte[u4LvdsCtrlAEven] : 0;
    u4LvdsCtrlAOdd = (u4LvdsCtrlAOdd < 4)? au4PortRotte[u4LvdsCtrlAOdd] : 0;   

    u4LvdsCtrlBEven = DRVCUST_PanelGet(eLvdsBCtrlEven);
    u4LvdsCtrlBOdd = DRVCUST_PanelGet(eLvdsBCtrlOdd);
    u4LvdsCtrlBEven = (u4LvdsCtrlBEven < 4)? au4PortRotte[u4LvdsCtrlBEven] : 0;
    u4LvdsCtrlBOdd = (u4LvdsCtrlBOdd < 4)? au4PortRotte[u4LvdsCtrlBOdd] : 0;   

    // A/B channel for PORTA, C/D channel for PORTB    
    vIO32WriteFldAlign(LVDSB_REG08, u4LvdsCtrlAEven, RG_A_SW);
    vIO32WriteFldAlign(LVDSB_REG08, u4LvdsCtrlAOdd,  RG_B_SW);
    vIO32WriteFldAlign(LVDSB_REG08, u4LvdsCtrlBEven, RG_C_SW);
    vIO32WriteFldAlign(LVDSB_REG08, u4LvdsCtrlBOdd,  RG_D_SW);
	
    if ((IS_PANEL_L12R12)&&(LVDS_OUTPUT_PORT == DUAL_PORT))
    {
        vIO32WriteFldAlign(LVDSB_REG08, 0, RG_A_SW);
        vIO32WriteFldAlign(LVDSB_REG08, 2, RG_B_SW);
        vIO32WriteFldAlign(LVDSB_REG06, 0, RG_ODD_SW); 
    }

}

void vDrvLVDSIFReorder(UINT8 val)
{
  if (val<=3)
  {

  }
}

static void _vDrvLVDSPanelInterfaceConfig(void)
{

    if ((IS_SEPARATE_OSD_AND_DATA)
        #if defined(CC_SUPPORT_4K2K)||defined(CC_SUPPORT_HDMI_4K2K30)
        ||(IS_DISABLE_LVDS_LINEBUFFER)
        #endif
        )
    {
    	vIO32WriteFldAlign(VINTF_B0, 1,  VINTF_LINE_BUFFER_BYPASS); 
    	vIO32WriteFldAlign(VINTF_P19, 3,  VINTF_DISP_R_ALLIGN_TYPE); 		
    }	
	else
	{
    	vIO32WriteFldAlign(VINTF_B0, 0,  VINTF_LINE_BUFFER_BYPASS); 
    	vIO32WriteFldAlign(VINTF_P19, 0,  VINTF_DISP_R_ALLIGN_TYPE); 				
	}
	
    vIO32WriteFldAlign(VINTF_P4, 1,  VINTF_REORDER_BYPASS); 	
    vIO32WriteFldAlign(VINTF_P4, wDISPLAY_WIDTH/2 ,  VINTF_REORDER_MIDDLE_PT);

    vIO32WriteFldAlign(VINTF_B0, 0 ,  VINTF_HV_BYPASS);
    vIO32WriteFldAlign(VINTF_B0, 1 ,  VINTF_HV_HOLD);     
	
    if(IS_PANEL_L12R12)
    {
     vIO32WriteFldAlign(VINTF_B0, 1,  VINTF_PDP_EN);
     vIO32WriteFldAlign(PIXEL_REORDER_07, 0, RG_PIXEL_REORDER_SHARE_SRAM); // Line buffer for L2R2/L1R1 case	 
    }
    else
    {
     vIO32WriteFldAlign(VINTF_B0, 0,  VINTF_PDP_EN);    
     vIO32WriteFldAlign(PIXEL_REORDER_07, 1, RG_PIXEL_REORDER_SHARE_SRAM);		 
    }

    // reorder setting
	#ifdef CC_SCPOS_3DTV_SUPPORT 
	if ((LVDS_DISP_3D == LVDS_DISP_3D_SHUTTER) && (LVDS_DISP_SPLIT == LVDS_DISP_SPLIT_ON))
	{
	 vIO32WriteFldAlign(VINTF_P4, 0,  VINTF_REORDER_BYPASS);    
 	 vIO32WriteFldAlign(VINTF_P4, REORDER_FORMAIN_OUT_2CH_HALF,	VINTF_REORDER_FORMAIN_OUT); 	
	}	
    else
	#endif
    {
     vIO32WriteFldAlign(VINTF_P4, REORDER_FORMAIN_OUT_SEQ,  VINTF_REORDER_FORMAIN_OUT);     
    }


    if(IS_LBRB_3D_ON)
    {
     vIO32WriteFldAlign(VINTF_B7, 1,  VINTF_LBRB_EN); 
    }
    else
    {
     vIO32WriteFldAlign(VINTF_B7, 0,  VINTF_LBRB_EN); 
    }

	// For 3D_Glasses Control - software work around
    #if (!defined(CC_SUPPORT_4K2K))&&(!defined(CC_SUPPORT_HDMI_4K2K30))
	if(!IS_DISPR_INVERSE)
	{
    vIO32WriteFldAlign(VINTF_P11, 3,  VINTF_LEYE_V_DLY); 		
    vIO32WriteFldAlign(VINTF_P11, 1,  VINTF_LEYE_POL); 		
	}
    #endif
}

void vTXCRCStatus(void)
{
    UINT8 u1group,Group_Cnt;
    
    vIO32WriteFldAlign(LVDSB_REG10, 60,  RG_CRC_VCNT);
	if((LVDS_OUTPUT_PORT == SINGLE_PORT))
	{
     Group_Cnt = 1;
	}
	else if((LVDS_OUTPUT_PORT == DUAL_PORT))
	{
	 Group_Cnt = 2;
	}
	else // FOUR_PORT
	{
	 Group_Cnt = 4;
	}
	
    for (u1group = 0; u1group < Group_Cnt; u1group++)
    {
     vIO32WriteFldAlign(LVDSB_REG10, u1group,  RG_CRC_SEL);     
     vIO32WriteFldAlign(LVDSB_REG10, 1,  RG_CRC_CLR); 
     x_thread_delay(10);//10mS    
     vIO32WriteFldAlign(LVDSB_REG10, 0,  RG_CRC_CLR);     
     vIO32WriteFldAlign(LVDSB_REG10, 0,  RG_CRC_START);              
     x_thread_delay(10);//10mS         
     vIO32WriteFldAlign(LVDSB_REG10, 1,  RG_CRC_START);         
     while (!(IO32ReadFldAlign(LVDSB_REG12, ST_LVDS_CRC_RDY))) { }    
     if(IO32ReadFldAlign(LVDSB_REG12, ST_LVDS_CRC_ERR))
     {
      Printf("Fail = [LVDS_%d] = %x%x\n",u1group,IO32ReadFldAlign(LVDSB_REG13, ST_LVDS_CRC_OUT_41_24),IO32ReadFldAlign(LVDSB_REG12, ST_LVDS_CRC_OUT_23_0));
	  Printf("Fail = [VGA_%d] = %x\n",u1group,IO32ReadFldAlign(LVDSB_REG11, ST_VGA_CRC_OUT));
     }
     else
     {
      Printf("OK = [LVDS_%d] = %x%x\n",u1group,IO32ReadFldAlign(LVDSB_REG13, ST_LVDS_CRC_OUT_41_24),IO32ReadFldAlign(LVDSB_REG12, ST_LVDS_CRC_OUT_23_0));
	  Printf("OK = [VGA_%d] = %x\n",u1group,IO32ReadFldAlign(LVDSB_REG11, ST_VGA_CRC_OUT));
     } 
    }

    vIO32WriteFldAlign(LVDSB_REG20, 1,  RG_LVDSRX_FIFO_EN);

	if(wDrvGetOutputVTotal()>=1)
	{
	 if(IS_PANEL_L12R12)
	 vIO32WriteFldAlign(LVDSB_REG21, (PANEL_GetPanelHeight()/2)-1,  RG_LVDSRX_CRC_H_END);
	 else
	 vIO32WriteFldAlign(LVDSB_REG21, PANEL_GetPanelHeight()-1,  RG_LVDSRX_CRC_H_END);
	} 
	vIO32WriteFldAlign(LVDSB_REG21, 1,  RG_LVDSRX_CRC_H_START);	

	vIO32WriteFldAlign(LVDSB_REG20, 1,  RG_LVDSRX_CRC_CLR);	
	x_thread_delay(10);//10mS  
	vIO32WriteFldAlign(LVDSB_REG20, 0,  RG_LVDSRX_CRC_CLR);	
	vIO32WriteFldAlign(LVDSB_REG20, 0,  RG_LVDSRX_CRC_START);
	x_thread_delay(10);//10mS  
	vIO32WriteFldAlign(LVDSB_REG20, 1,  RG_LVDSRX_CRC_START);
	
	while (!(IO32ReadFldAlign(LVDSB_REG22,  LVDSRX_CRC_DONE))) { }    
	if(IO32ReadFldAlign(LVDSB_REG22, LVDSRX_CRC_FAIL))
	{
	 Printf("Fail = [FIFO LVDS] = %x\n",IO32ReadFldAlign(LVDSB_REG22, LVDSRX_CRC_VALUE));
	}
	else
	{
	 Printf("OK = [FIFO LVDS] = %x\n",IO32ReadFldAlign(LVDSB_REG22, LVDSRX_CRC_VALUE));
	} 

	vDrvTCONRXCRC(0);

	
}

static void _vDrvLVDSConfigFormat(void)
{
    UINT32 u4MSBSW;
    
    u4MSBSW = LVDS_DISP_FORMAT;
    #ifdef DRV_SUPPORT_EXTMJC
    if (IS_COMPANION_CHIP_ON())
    {
        if (vDrvGetCurLvdsMode() !=  eLvdsVidNormal)
        {
            u4MSBSW = LVDS_MSB_SW_OFF;
        }
    }
    #endif

    
    // set lvds format
    if (u4MSBSW == LVDS_SPECIAL_NS) // special NS
    {
        vIO32WriteFldAlign(LVDSB_REG06, 0, RG_NS_VESA_EN);        
        vIO32WriteFldAlign(LVDSB_REG06, 1, RG_SPECIAL_NS);        
    }
    else if (u4MSBSW == LVDS_MSB_SW_ON) // NS
    {
		if (DISP_BIT == DISP_18BIT)
		{
			vIO32WriteFldAlign(LVDSB_REG06, 0, RG_NS_VESA_EN);
		}
		else
		{
        	vIO32WriteFldAlign(LVDSB_REG06, 1, RG_NS_VESA_EN);  
        }
        vIO32WriteFldAlign(LVDSB_REG06, 0, RG_SPECIAL_NS);        
    }
    else // JEIDA
    {
        vIO32WriteFldAlign(LVDSB_REG06, 0, RG_NS_VESA_EN);        
        vIO32WriteFldAlign(LVDSB_REG06, 0, RG_SPECIAL_NS);        
    }

	if (IS_SEPARATE_OSD_AND_DATA)
	{
        vIO32WriteFldAlign(LVDSB_REG26, 1, RG_OSD_SWITCH_CH);  
        vIO32WriteFldAlign(LVDSB_REG26, 1, RG_OSD_ENCODE_EN);  		
	}
	else
	{
        vIO32WriteFldAlign(LVDSB_REG26, 0, RG_OSD_SWITCH_CH);  
        vIO32WriteFldAlign(LVDSB_REG26, 0, RG_OSD_ENCODE_EN);  		
	}

    _vDrvLVDSPanelInterfaceConfig();
}

//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

#ifdef CONFIG_OPM
    BOOL fgLVDSInit = FALSE;
#else
    static BOOL fgLVDSInit = FALSE;
#endif
void vDrvLVDSInit(void)
{
    UINT32 u4Value;
    UINT32 *pu4PinMap,*pu4PNSwap;
	
	if (IS_PANEL_L12R12)
    {    
     vIO32WriteFldAlign(LVDSB_REG00, 1, RG_LVDSTX_2CH_ARCH);     
    }
    else
    {
     vIO32WriteFldAlign(LVDSB_REG00, 0, RG_LVDSTX_2CH_ARCH); 	    
    }
	
    if (IO32ReadFldAlign(DDDS_08, FLAG_LVDS_INITIAL) == 1)
    {
        fgLVDSInit = TRUE;
    }

	if (!fgLVDSInit)
	{
    // set TVO(driving current) if necessary
    u4Value = PANEL_GetDrivingCurrent();
    if (u4Value <= 15)
    {
        vDrvLVDSSetDriving(u4Value);
    }
    else
    {
		#ifdef CC_MT5399
      	vIO32WriteFldMulti(REG_LVDS_TX_CFG6, P_Fld(0x3fff,RG_LVDSB_R_TERM_EVEN_D11)| P_Fld(0x3fff,RG_LVDSB_R_TERM_ODD_D11));   
   		#endif
		
        vDrvLVDSSetDriving(5); // MT5396 5: 1mA+4mA =5mA
    }

    // switch to LVDS TX mode
    vIO32WriteFldAlign(MLVDS_CTRL_09, 0, TCLK_FIFO_ACTIVE_PORT);                

    // Disable 2CH architecture in MT5368/96
	//    vIO32WriteFldAlign(VINTF_B0, 0, VINTF_2CH_EN);  // 98/80 remove

    //bypass LVDS input buffer since the clock source is the same
	// it can avoid vsync reset makes data improper
    vIO32WriteFldAlign(LVDSB_REG01, 1, RG_IBUF_BYPASS);
	

    // ==Pair Swap ==
    if(NULL == (pu4PinMap = (UINT32*)DRVCUST_PanelGet(ePanelLvds10bitPinMap)))
    {
        // Port B Setting
        vIO32WriteFldMulti(LVDSB_REG03, P_Fld(7, RG_RLV7_SEL)|P_Fld(6, RG_RLV6_SEL)|P_Fld(5, RG_RLV5_SEL)|P_Fld(4, RG_RLV4_SEL)|P_Fld(3, RG_RLV3_SEL)|P_Fld(2, RG_RLV2_SEL)|P_Fld(1, RG_RLV1_SEL)|P_Fld(0, RG_RLV0_SEL));
        vIO32WriteFldMulti(LVDSB_REG04, P_Fld(11, RG_RLV_CK1_SEL)|P_Fld(10, RG_RLV_CK0_SEL)|P_Fld(9, RG_RLV9_SEL)|P_Fld(8, RG_RLV8_SEL));		

	    // Port A Setting	
        vIO32WriteFldMulti(LVDSB_REG02, P_Fld(7, RG_LLV7_SEL)|P_Fld(6, RG_LLV6_SEL)|P_Fld(5, RG_LLV5_SEL)|P_Fld(4, RG_LLV4_SEL)|P_Fld(3, RG_LLV3_SEL)|P_Fld(2, RG_LLV2_SEL)|P_Fld(1, RG_LLV1_SEL)|P_Fld(0, RG_LLV0_SEL));
        vIO32WriteFldMulti(LVDSB_REG04, P_Fld(11, RG_LLV_CK1_SEL)|P_Fld(10, RG_LLV_CK0_SEL)|P_Fld(9, RG_LLV9_SEL)|P_Fld(8, RG_LLV8_SEL));
    }
    else
    {
        // remap data pairs
        vIO32WriteFldMulti(LVDSB_REG02, P_Fld(pu4PinMap[7], RG_LLV7_SEL)|P_Fld(pu4PinMap[6], RG_LLV6_SEL)|P_Fld(pu4PinMap[5], RG_LLV5_SEL)|P_Fld(pu4PinMap[4], RG_LLV4_SEL)|P_Fld(pu4PinMap[3], RG_LLV3_SEL)|P_Fld(pu4PinMap[2], RG_LLV2_SEL)|P_Fld(pu4PinMap[1], RG_LLV1_SEL)|P_Fld(pu4PinMap[0], RG_LLV0_SEL));
        vIO32WriteFldMulti(LVDSB_REG04, P_Fld(pu4PinMap[11], RG_LLV_CK1_SEL)|P_Fld(pu4PinMap[10], RG_LLV_CK0_SEL)|P_Fld(pu4PinMap[9], RG_LLV9_SEL)|P_Fld(pu4PinMap[8], RG_LLV8_SEL));
        vIO32WriteFldMulti(LVDSB_REG03, P_Fld(pu4PinMap[19], RG_RLV7_SEL)|P_Fld(pu4PinMap[18], RG_RLV6_SEL)|P_Fld(pu4PinMap[17], RG_RLV5_SEL)|P_Fld(pu4PinMap[16], RG_RLV4_SEL)|P_Fld(pu4PinMap[15], RG_RLV3_SEL)|P_Fld(pu4PinMap[14], RG_RLV2_SEL)|P_Fld(pu4PinMap[13], RG_RLV1_SEL)|P_Fld(pu4PinMap[12], RG_RLV0_SEL));
        vIO32WriteFldMulti(LVDSB_REG04, P_Fld(pu4PinMap[23], RG_RLV_CK1_SEL)|P_Fld(pu4PinMap[22], RG_RLV_CK0_SEL)|P_Fld(pu4PinMap[21], RG_RLV9_SEL)|P_Fld(pu4PinMap[20], RG_RLV8_SEL));
    }

	// == PN Swap ==
	// pn swap BE0(RLV5)/ BO2(RLV2)/ BO0(RLV0)
	if(NULL == (pu4PNSwap = (UINT32*)DRVCUST_PanelGet(ePanelLvds10bitPNSwap)))
	{
	 vIO32WriteFldMulti(LVDSB_REG05, 
	 P_Fld(0, RG_RLVCK1_PN_SWAP)|P_Fld(0, RG_RLVCK0_PN_SWAP)|P_Fld(0, RG_RLV9_PN_SWAP)|P_Fld(0, RG_RLV8_PN_SWAP)|P_Fld(0, RG_RLV7_PN_SWAP)|P_Fld(0, RG_RLV6_PN_SWAP)|
	 P_Fld(0, RG_RLV5_PN_SWAP)|P_Fld(0, RG_RLV4_PN_SWAP)|P_Fld(0, RG_RLV3_PN_SWAP)|P_Fld(0, RG_RLV2_PN_SWAP)|P_Fld(0, RG_RLV1_PN_SWAP)|P_Fld(0, RG_RLV0_PN_SWAP)|	
	 P_Fld(0, RG_LLVCK1_PN_SWAP)|P_Fld(0, RG_LLVCK0_PN_SWAP)|P_Fld(0, RG_LLV9_PN_SWAP)|P_Fld(0, RG_LLV8_PN_SWAP)|P_Fld(0, RG_LLV7_PN_SWAP)|P_Fld(0, RG_LLV6_PN_SWAP)|
	 P_Fld(0, RG_LLV5_PN_SWAP)|P_Fld(0, RG_LLV4_PN_SWAP)|P_Fld(0, RG_LLV3_PN_SWAP)|P_Fld(0, RG_LLV2_PN_SWAP)|P_Fld(0, RG_LLV1_PN_SWAP)|P_Fld(0, RG_LLV0_PN_SWAP)
	 );
	}
	else
	{
	 vIO32WriteFldMulti(LVDSB_REG05, 
	 P_Fld(pu4PNSwap[11], RG_RLVCK1_PN_SWAP)|P_Fld(pu4PNSwap[10], RG_RLVCK0_PN_SWAP)|P_Fld(pu4PNSwap[9], RG_RLV9_PN_SWAP)|P_Fld(pu4PNSwap[8], RG_RLV8_PN_SWAP)|P_Fld(pu4PNSwap[7], RG_RLV7_PN_SWAP)|P_Fld(pu4PNSwap[6], RG_RLV6_PN_SWAP)|
	 P_Fld(pu4PNSwap[5], RG_RLV5_PN_SWAP)|P_Fld(pu4PNSwap[4], RG_RLV4_PN_SWAP)|P_Fld(pu4PNSwap[3], RG_RLV3_PN_SWAP)|P_Fld(pu4PNSwap[2], RG_RLV2_PN_SWAP)|P_Fld(pu4PNSwap[1], RG_RLV1_PN_SWAP)|P_Fld(pu4PNSwap[0], RG_RLV0_PN_SWAP)|   
	 P_Fld(pu4PNSwap[23], RG_LLVCK1_PN_SWAP)|P_Fld(pu4PNSwap[22], RG_LLVCK0_PN_SWAP)|P_Fld(pu4PNSwap[21], RG_LLV9_PN_SWAP)|P_Fld(pu4PNSwap[20], RG_LLV8_PN_SWAP)|P_Fld(pu4PNSwap[19], RG_LLV7_PN_SWAP)|P_Fld(pu4PNSwap[18], RG_LLV6_PN_SWAP)|
	 P_Fld(pu4PNSwap[17], RG_LLV5_PN_SWAP)|P_Fld(pu4PNSwap[16], RG_LLV4_PN_SWAP)|P_Fld(pu4PNSwap[15], RG_LLV3_PN_SWAP)|P_Fld(pu4PNSwap[14], RG_LLV2_PN_SWAP)|P_Fld(pu4PNSwap[13], RG_LLV1_PN_SWAP)|P_Fld(pu4PNSwap[12], RG_LLV0_PN_SWAP)
	 );
	}
	
	//A Port
	vIO32WriteFldMulti(REG_LVDS_TX_CFG19, P_Fld(1, RG_LVDSA_LDO_BIAS_EVEN_PWD_D11)|P_Fld(1, RG_LVDSA_LDO_EVEN_PWD_D11)|P_Fld(1, RG_LVDSA_LDO_BIAS_ODD_PWD_D11)|P_Fld(1, RG_LVDSA_LDO_ODD_PWD_D11));
    vIO32WriteFldMulti(REG_LVDS_TX_CFG11, P_Fld(0x0B, RG_LVDSA_TVCM)|P_Fld(1, RG_LVDSA_NSRC)|P_Fld(1, RG_LVDSA_PSRC)
					|P_Fld(1, RG_LVDSA_EN)|P_Fld(1, RG_LVDSA_BIAS_ODD_PWD)|P_Fld(1, RG_LVDSA_BIAS_EVEN_PWD));
	#ifdef CC_MT5399
	vIO32WriteFldAlign(REG_LVDS_TX_CFG11, 0xFFF, RG_LVDSA_EXT_EN_D11);
	#else
	vIO32WriteFldAlign(REG_LVDS_TX_CFG11, 0x3FFF, RG_LVDSA_EXT_EN_D11);
	#endif
	#ifdef CC_MT5399
	//B Port
	vIO32WriteFldMulti(REG_LVDS_TX_CFG0, P_Fld(0x0B, RG_LVDSB_TVCM_D11 )|P_Fld(1, RG_LVDSB_EN_D11 )|P_Fld(1, RG_LVDSB_BIAS_EN_D11));
	#endif	
	
	
	//A Port 
    vIO32WriteFldMulti(REG_LVDS_TX_CFG14, P_Fld(0, RG_LVDSA_HSN_MODE_EVEN_EN_D11)|P_Fld(0, RG_LVDSA_HSP_MODE_EVEN_EN_D11)|P_Fld(0, RG_LVDSA_HSN_MODE_ODD_EN_D11)|P_Fld(0, RG_LVDSA_HSP_MODE_ODD_EN_D11));
    vIO32WriteFldMulti(REG_LVDS_TX_CFG12, P_Fld(4, RG_LVDSA_BIAS_SEL)|P_Fld(1, RG_LVDSA_NSRC_PRE)|P_Fld(1, RG_LVDSA_PSRC_PRE));
	#ifdef CC_MT5399
	vIO32WriteFldAlign(REG_LVDS_TX_CFG14, 0xFFF, RG_LVDSA_LVDS_SEL);
	#else
	vIO32WriteFldAlign(REG_LVDS_TX_CFG14, 0x3FFF, RG_LVDSA_LVDS_SEL);
	#endif
    vIO32WriteFldAlign(REG_LVDS_TX_CFG13, 0x0, RG_LVDSA_CLK_IQ_SEL);
	#ifdef CC_MT5399
	//B Port
	vIO32WriteFldMulti(REG_LVDS_TX_CFG2, P_Fld(0x0, RG_LVDSB_CLK_IQ_SEL_EVEN_D11)|P_Fld(0,RG_LVDSB_CLK_IQ_SEL_ODD_D11 ));
	#endif
	
	//A Port
    vIO32WriteFldAlign(REG_LVDS_TX_CFG19, 0x1, RG_LVDSA_RESYNC_LVDS_SEL_D11);
    vIO32WriteFldAlign(REG_LVDS_TX_CFG19, 0x0, RG_LVDSA_RESYNC_CLK_IQ_SEL_D11);

	if (IS_DISABLE_PANEL_HVSYNC)
	{
	// disable vsync output for dual LVDS channels
	vIO32WriteFldAlign(LVDSB_REG06, 1, RG_VS_SEL);				  
	// disable hsync output for dual LVDS channels
	vIO32WriteFldAlign(LVDSB_REG06, 1, RG_HS_SEL);				  
	}
	else
	{
    // enable vsync output for dual LVDS channels
    vIO32WriteFldAlign(LVDSB_REG06, 0, RG_VS_SEL);                
    // enable hsync output for dual LVDS channels
    vIO32WriteFldAlign(LVDSB_REG06, 0, RG_HS_SEL);                
	}
    // enable data_en output for dual LVDS channels
    vIO32WriteFldAlign(LVDSB_REG06, 0, RG_DE_SEL);                
    
        
    //LVDS 7->4 FIFO enable 
    #ifdef CC_MTK_LOADER
    vDrvLVDS7To4FifoEnable();
    #else
    vIO32WriteFldAlign(LVDSB_REG08, 1, RG_LVDS_7TO10FIFO_EN);                
    #endif

    
    vIO32WriteFldAlign(DDDS_08, 1, FLAG_LVDS_INITIAL);
	} 
}

#define LVDSDrivingIndex 8
PRIVATE UINT8 code bLVDSDrivingCurrent[LVDSDrivingIndex+1] =
{
	0x00, 0x00,0x00,0x00,0x02,0x04,0x06,0x08,0x0A,
//  3mA,  3mA,  3mA,, 3mA,,   4mA,   5mA,   6mA,   7mA,    8mA,//Remy,99 will be add 4 

};


void vDrvLVDSSetDriving(UINT8 bDriving)
{    
    //mt5363/87 bit [0] 1 mA bit [1] 1 mA bit [2] 1 mA bit [3] 2 mA
    //mt5365/95 bit [0] 1 mA bit [1] 2 mA bit [2] 4 mA bit [3] 4 mA
    //mt5396      bit [0] 05mA bit [1] 1 mA bit [2] 2 mA bit [3] 4 mA
    //mt5398      bit [0] 05mA bit [1] 1 mA bit [2] 2 mA bit [3] 4 mA    
    bDriving &= 0xf;  //0~15
    if (bDriving > LVDSDrivingIndex) bDriving = LVDSDrivingIndex;
    bDriving = bLVDSDrivingCurrent[bDriving];  
    vIO32WriteFldAlign(REG_LVDS_TX_CFG11, (bDriving+4), RG_LVDSA_TVO);
	#ifdef CC_MT5399
   	LVDSTVO=vDrvLVDSsettingDrivingCurrent(bDriving);
    vIO32WriteFldAlign(REG_LVDS_TX_CFG3,  LVDSTVO, RG_LVDSB_TVO_EVEN_D11);
    vIO32WriteFldAlign(REG_LVDS_TX_CFG4, LVDSTVO, RG_LVDSB_TVO_ODD_D11);
	#endif
}

#ifdef CC_MT5399
UINT32 vDrvLVDSsettingDrivingCurrent(UINT8 bCurrent)
{	
	UINT32 TVOValue;
	TVOValue=bCurrent|bCurrent<<4|bCurrent<<8|bCurrent<<12|bCurrent<<16|bCurrent<<20|bCurrent<<24;
	return (TVOValue);
}
#endif

UINT8 vDrvLVDSGetDriving(void)
{
    UINT8 bDriving;
    bDriving = IO32ReadFldAlign(REG_LVDS_TX_CFG11,  RG_LVDSA_TVO) /2+1;
	bDriving &= 0xf;  //0~15
    return (bDriving);
}

void vDrvLVDSSetVCM(UINT8 bVCM)
{    
    bVCM &= 0xf;  //0~15
    vIO32WriteFldAlign(REG_LVDS_TX_CFG11, bVCM, RG_LVDSA_TVCM);
	#ifdef CC_MT5399
	vIO32WriteFldAlign(REG_LVDS_TX_CFG0, bVCM, RG_LVDSB_TVCM_D11 );
	#endif
}

UINT8 vDrvLVDSGetVCM(void)
{
    UINT8 bVCM;
    bVCM = IO32ReadFldAlign(REG_LVDS_TX_CFG11,RG_LVDSA_TVCM);
	bVCM &= 0xf;  //0~15
    return (bVCM);
}

void LVDS_SwitchDataEnable(BOOL fEnable)
{
    if (fEnable)
    {
       //OUTSTG_DEN_EN==0 : data enable
        vIO32WriteFldAlign(OSTG_PTGEN_00, 0, FLD_DEN_EN);
    }
    else
    {
    	//OUTSTG_DEN_EN==1 : data disable
        vIO32WriteFldAlign(OSTG_PTGEN_00, 1, FLD_DEN_EN);
    }
}

#ifdef DRV_SUPPORT_EXTMJC
void vDrvSetLvdsColorDomain(UINT32 u4ColorDomain)
{
#ifndef CC_MTK_LOADER
    	UINT32 u4BgColor;
#endif

	_u4LvdsColorDomain =u4ColorDomain;
#ifndef CC_MTK_LOADER
	// set mute color
	VDP_GetBg(SV_VP_MAIN, &u4BgColor);
	VDP_SetBg(SV_VP_MAIN, u4BgColor);
	VDP_GetBg(SV_VP_PIP, &u4BgColor);
	VDP_SetBg(SV_VP_PIP, u4BgColor);
#endif
    	if(vDrvGetLvdsColorDomain() == eColorDomainYUV)
    	{
		//PMX_SetYuvMode(TRUE);
		//OSD_BASE_SetYuvOutput(TRUE);
		PMX_SetYuvMode(FALSE); // RGB to YUV do by LVDS TX Transform
		OSD_BASE_SetYuvOutput(FALSE); // RGB to YUV do by LVDS TX Transform
    	}
    	else
    	{
    		PMX_SetYuvMode(FALSE);
    	       OSD_BASE_SetYuvOutput(FALSE);
    	}

}

UINT32 vDrvGetLvdsColorDomain(void)
{
	return _u4LvdsColorDomain;
}

void vDrvSetCurLvdsMode(UINT32 u4LvdsMode)
{
    if (_u4CurrentLvdsMode == u4LvdsMode)
    {
        return;
    }
    
    _u4CurrentLvdsMode = u4LvdsMode;
    LOG(1, "vDrvSetCurLvdsMode, _u4CurrentLvdsMode=%d\n", _u4CurrentLvdsMode);

    vDrvLVDSChgMod(_u4CurrentLvdsMode);
}

UINT32 vDrvGetCurLvdsMode(void)
{
    return _u4CurrentLvdsMode;
}

#endif

/**
 * @brief vDrvLVDSChgMod
 * Change mode for 53xx + 828x
 *
 * @param  u4Mode
 * @retval void
 */
void vDrvLVDSChgMod(UINT32 u4Mode)
{
    UINT32 u48BitMode;


    if (((LVDS_MODE_T)u4Mode == eLvdsVidYuv422TwoPort8Bit)
        || ((LVDS_MODE_T)u4Mode == eLvdsVidYuv422OnePort8Bit)
        || ((LVDS_MODE_T)u4Mode == eLvdsVidRgb444OnePort8Bit))
    {
        u48BitMode = 1;
    }
    else
    {
        u48BitMode = 0;
    }

    #ifdef CC_SCPOS_3DTV_SUPPORT
    if ((LVDS_MODE_T)u4Mode != eLvdsVidNormal) //  MLVDS_SO0_RG_RES_FLD msut be 1
    {
		UINT32 u4EnPair;
		if (DRVCUST_PanelQuery(eDISP_RLVDSEncodeLink, &u4EnPair) == 0)
		{
			u1SetDISP_REncodeEn(((UINT8)u4EnPair)|0x01); //  MLVDS_SO0_RG_RES_FLD msut be 1
		} 
		else
		{
			vIO32WriteFldAlign(LVDSB_REG08, 1, RG_OSD_3D_IN_HS);
			vIO32WriteFldAlign(LVDSB_REG08, 1, RG_OSD_3D_IN_VS);	 
		}
    }
	else
	{
	 vIO32WriteFldAlign(LVDSB_REG08, 0, RG_OSD_3D_IN_HS);
	 vIO32WriteFldAlign(LVDSB_REG08, 0, RG_OSD_3D_IN_VS);	
	}
	#endif
	
    // select one mode
    switch ((LVDS_MODE_T)u4Mode)
    {
    case eLvdsVidNormal:
        // Back to the initial setting and return
        vIO32WriteFldAlign(OSTG_01, 0, REG_YUV2YC_EN);
        vIO32WriteFldAlign(OSTG_01, 0, REG_LPF_EN);
        vIO32WriteFldAlign(OSTG_01, 0, REG_C_LINE_EXT);
        vIO32WriteFldMulti(LVDSB_REG06, P_Fld(0, RG_MERGE_OSD)|P_Fld(0, RG_2CH_MERGE)
                                            |P_Fld(0, RG_RGB_444_MERGE)|P_Fld(u48BitMode, RG_8BIT_DUAL));
        vDrvVOPLLSet();
        break;


    case eLvdsVidYuv422TwoPort:
    case eLvdsVidYuv422TwoPort8Bit:
        // enable Tx mode 3
        vIO32WriteFldAlign(OSTG_01, 1, REG_YUV2YC_EN);
        vIO32WriteFldAlign(OSTG_01, 1, REG_LPF_EN);
        vIO32WriteFldAlign(OSTG_01, 1, REG_C_LINE_EXT);
        vIO32WriteFldMulti(LVDSB_REG06, P_Fld(1, RG_MERGE_OSD)|P_Fld(1, RG_2CH_MERGE)
                                            |P_Fld(0, RG_RGB_444_MERGE)|P_Fld(u48BitMode, RG_8BIT_DUAL));
        #if 0
        #ifdef DRV_SUPPORT_EXTMJC_MODE7
        if (DRVCUST_InitGet(eFlagDDRQfp)) //mt5387 condition
        {
            vIO32WriteFldAlign(OUTSTG_OS_19, 1, OS_19_REG_8BIT_DUAL);
        }
        #endif
        #endif
        vDrvVOPLLSet();
        break;

    case eLvdsVidRgb444TwoPort:  
        // enable Tx mode 4
        vIO32WriteFldAlign(OSTG_01, 0, REG_YUV2YC_EN);
        vIO32WriteFldAlign(OSTG_01, 0, REG_LPF_EN);
        vIO32WriteFldAlign(OSTG_01, 0, REG_C_LINE_EXT);
        vIO32WriteFldMulti(LVDSB_REG06, P_Fld(1, RG_MERGE_OSD)|P_Fld(1, RG_2CH_MERGE)
                                            |P_Fld(1, RG_RGB_444_MERGE)|P_Fld(u48BitMode, RG_8BIT_DUAL));
        vDrvVOPLLSet();
        break;

    case eLvdsVidYuv422OnePort:
    case eLvdsVidYuv422OnePort8Bit:
        vIO32WriteFldAlign(OSTG_01, 1, REG_YUV2YC_EN);
        vIO32WriteFldAlign(OSTG_01, 1, REG_LPF_EN);
        vIO32WriteFldAlign(OSTG_01, 1, REG_C_LINE_EXT);
        vIO32WriteFldMulti(LVDSB_REG06, P_Fld(1, RG_MERGE_OSD)|P_Fld(0, RG_2CH_MERGE)
                                            |P_Fld(0, RG_RGB_444_MERGE)|P_Fld(u48BitMode, RG_8BIT_DUAL));
        vDrvVOPLLSet();
        break;

    case eLvdsVidRgb444OnePort:
    case eLvdsVidRgb444OnePort8Bit:
        vIO32WriteFldAlign(OSTG_01, 0, REG_YUV2YC_EN);
        vIO32WriteFldAlign(OSTG_01, 0, REG_LPF_EN);
        vIO32WriteFldAlign(OSTG_01, 0, REG_C_LINE_EXT);
        vIO32WriteFldMulti(LVDSB_REG06, P_Fld(1, RG_MERGE_OSD)|P_Fld(0, RG_2CH_MERGE)
                                            |P_Fld(1, RG_RGB_444_MERGE)|P_Fld(u48BitMode, RG_8BIT_DUAL));
        vDrvVOPLLSet();
        break;
    default:
        LOG(0, "Not Support LVDS Mode (%d)\n", u4Mode);
        break;
    }

    // the clock changed, reset 74Fifo
    vDrvLVDS7To4FifoEnable();

    // set lvds format
    _vDrvLVDSConfigFormat();

    // config LVDS PD, dual port, and 10bit control bits
    vDrvLVDSOn();
    
}

void vSetLVDSPadPD(UINT32 u4PDVal)
{
#ifdef CC_MT5399
    UINT8 EVENPD;
    UINT8 ODDPD;
    UINT32 u4PDVal_tmp;

    // Lvds control : 
    UINT32 u4LvdsCtrlAEven = 0;
    UINT32 u4LvdsCtrlAOdd = 1;
    UINT32 u4LvdsCtrlBEven = 2;
    UINT32 u4LvdsCtrlBOdd = 3;
    UINT32 au4PortRotte[4] = {0, 1, 2, 3};
    UINT32 au4PDCh[4];
    UINT32 u4TmpCnt;
    UINT32 *pu4PinMap;
    UINT32 u4TmpPDCh;
    UINT32 u4TmpCnt2;
    UINT8 u1PinCh;
    const UINT32 au4PadRemapTbl[12]={0,1,2,3,4,0,1,2,3,4,5/*EvenCLK*/,5/*OddCLK*/};

    // panel specific port rotate
    if (LVDS_USE_INDEPENDENT_SETTING)
    {
        au4PortRotte[0] = LVDS_A_INDEPENDENT_SETTING;
        au4PortRotte[1] = LVDS_B_INDEPENDENT_SETTING;
        au4PortRotte[2] = LVDS_C_INDEPENDENT_SETTING;
        au4PortRotte[3] = LVDS_D_INDEPENDENT_SETTING;
    }

    // PCB layout rotate
    u4LvdsCtrlAEven = DRVCUST_PanelGet(eLvdsACtrlEven);
    u4LvdsCtrlAOdd = DRVCUST_PanelGet(eLvdsACtrlOdd);
    u4LvdsCtrlAEven = (u4LvdsCtrlAEven < 4)? au4PortRotte[u4LvdsCtrlAEven] : 0;
    u4LvdsCtrlAOdd = (u4LvdsCtrlAOdd < 4)? au4PortRotte[u4LvdsCtrlAOdd] : 0;   

    u4LvdsCtrlBEven = DRVCUST_PanelGet(eLvdsBCtrlEven);
    u4LvdsCtrlBOdd = DRVCUST_PanelGet(eLvdsBCtrlOdd);
    u4LvdsCtrlBEven = (u4LvdsCtrlBEven < 4)? au4PortRotte[u4LvdsCtrlBEven] : 0;
    u4LvdsCtrlBOdd = (u4LvdsCtrlBOdd < 4)? au4PortRotte[u4LvdsCtrlBOdd] : 0;   

    pu4PinMap = (UINT32*)DRVCUST_PanelGet(ePanelLvds10bitPinMap);

    // u4PDVal: BO/BE/AO/AE (3/2/1/0)
    for(u4TmpCnt=0;u4TmpCnt<4;u4TmpCnt++)
    {
        au4PDCh[u4TmpCnt] = ((u4PDVal >> (6*u4TmpCnt)) & 0x3f);
        if(NULL != pu4PinMap)
        {
            u4TmpPDCh = 0; //au4PDCh[u4TmpCnt]; 
            // remap the analog pad channel
            for(u4TmpCnt2=0;u4TmpCnt2<6;u4TmpCnt2++)
            {
                if(u4TmpCnt2 < 5)
                {   //data channel
                    u1PinCh = u4TmpCnt2+5*(u4TmpCnt%2);
                }
                else
                {   //clk channel
                    u1PinCh = 10+(u4TmpCnt%2);
                }
                if(0 != (au4PDCh[u4TmpCnt] & (1<< au4PadRemapTbl[u1PinCh])))
                {
                    //power down this channel
                    u4TmpPDCh |= (1 << au4PadRemapTbl[pu4PinMap[u1PinCh]]);
                }  
            }
            au4PDCh[u4TmpCnt] = u4TmpPDCh;
        }
    }
    u4PDVal = (au4PDCh[0] << (6*u4LvdsCtrlAEven)) |
              (au4PDCh[1] << (6*u4LvdsCtrlAOdd)) |
              (au4PDCh[2] << (6*u4LvdsCtrlBEven)) |
              (au4PDCh[3] << (6*u4LvdsCtrlBOdd));
   //A Port
   vIO32WriteFldAlign(REG_LVDS_TX_CFG11, (0xFFF-u4PDVal), RG_LVDSA_EXT_EN_D11);
   //vIO32WriteFldAlign(REG_LVDS_TX_CFG0, (0xFFF-(u4PDVal>>12)), RG_LVDSB_EXT_EN_D11);
   vIO32WriteFldAlign(REG_LVDS_TX_CFG11, 0x1, RG_LVDSA_EXT_EN_D11_E0);
   vIO32WriteFldAlign(REG_LVDS_TX_CFG12, u4PDVal, RG_LVDSA_DRV_PWD);
   //B Port
   u4PDVal_tmp= (u4PDVal>>12&0xFFF);
   EVENPD=(( u4PDVal_tmp>>6)|(((u4PDVal_tmp>>6)&& 0x20)<<7))&0x5F;
   ODDPD=(( u4PDVal_tmp&0x3F)|(((u4PDVal_tmp&0x3F)&& 0x20)<<7))&0x5F;
   vIO32WriteFldMulti(REG_LVDS_TX_CFG1, P_Fld((0x5F-EVENPD), RG_LVDSB_DRV_EN_EVEN_D11)|P_Fld((0x5F-ODDPD),RG_LVDSB_DRV_EN_ODD_D11));
#else
   vIO32WriteFldAlign(REG_LVDS_TX_CFG11, (0x3FFF-u4PDVal), RG_LVDSA_EXT_EN_D11);
   vIO32WriteFldAlign(REG_LVDS_TX_CFG11, 0x1, RG_LVDSA_EXT_EN_D11_E0);
   vIO32WriteFldAlign(REG_LVDS_TX_CFG12, u4PDVal, RG_LVDSA_DRV_PWD);
#endif   

}

#define DEBUG_MSG_TX 0
static void vSetLVDSPadSourcePD(UINT32 u4DispBit, UINT32 u4LVDSPort)
{
	UINT8  u1EnableDataBit,u1Link;	
	UINT32 u4ClockPD, u4LinkPD, u4EnablePair, u4FinaleEnablePair;		
    UINT32 u4PadPDTbl[4]={0,1,2,3}; // {PD_A,PD_B,PD_C,PD_D}
    
	#ifndef NDEBUG
    #if DEBUG_MSG_TX
    Printf("[SA7]u4DispBit = 0x%8x, u4LVDSPort = 0x%8x, \n",u4DispBit,u4LVDSPort);
    #else
    Printf("[SA7]u4DispBit = 0x%8x, u4LVDSPort = 0x%8x",u4DispBit,u4LVDSPort);    
    #endif
    #endif

    u4LinkPD = 0x00; 
    u4EnablePair = 0;
    u4LinkPD = 0; 
    u4ClockPD = 0;  
	
	// Enable bit Control
	if (u4DispBit == DISP_18BIT) u1EnableDataBit = 0x18;
	else if (u4DispBit == DISP_24BIT) u1EnableDataBit = 0x10;	
   	else if (u4DispBit == DISP_30BIT) u1EnableDataBit = 0x00;		
   	else u1EnableDataBit = 0x00;
   
   #if DEBUG_MSG_TX
   Printf("u1EnableDataBit = 0x%8x\n",u1EnableDataBit);  
   #endif
                 
    for (u1Link=0; u1Link<4; u1Link++)
    {
     u4EnablePair = u4EnablePair + (u1EnableDataBit << (u1Link*5));        
    }             
    #if DEBUG_MSG_TX                   
    Printf("u4EnablePair = 0x%8x\n",u4EnablePair);
    #endif
    
   	// Power Down Link Control   	
	if (u4LVDSPort == SINGLE_PORT) 
    {
       for (u1Link=0; u1Link<4; u1Link++)
       {
         if(u4PadPDTbl[u1Link]!=0)
         {
            u4LinkPD = u4LinkPD +   (0x1F<<(u1Link*5));          
         }                             
       }                
    }
	else if (u4LVDSPort == DUAL_PORT)
	
	{
       for (u1Link=0; u1Link<4; u1Link++)
       {
         if ((u4PadPDTbl[u1Link]==2) || (u4PadPDTbl[u1Link]==3))
         {
            u4LinkPD = u4LinkPD +   (0x1F<<(u1Link*5));
         }                             
       }                  
    }             
   	else if (u4LVDSPort == FOUR_PORT) u4LinkPD = 0x00;		
   	else u4LinkPD = 0x00;   	   	

    #if DEBUG_MSG_TX
    Printf("u4EnablePair = 0x%8x\n",u4EnablePair);
    Printf("u4LinkPD = 0x%8x\n",u4LinkPD);    
    #endif
    
    // Power Down clock  path
	if (u4LVDSPort == SINGLE_PORT)
	{
       for (u1Link=0; u1Link<4; u1Link++)
       {
         if(u4PadPDTbl[u1Link]!=0)
         {
            u4ClockPD = u4ClockPD +   (0x1<<u1Link); 
         }                             
       }              
    }               
	else if (u4LVDSPort == DUAL_PORT)
	{
       for (u1Link=0; u1Link<4; u1Link++)
       {
         if ((u4PadPDTbl[u1Link]==2) || (u4PadPDTbl[u1Link]==3))
         {
            u4ClockPD = u4ClockPD +   (0x1<<u1Link);  
         }                             
       }             
    }     
   	else if (u4LVDSPort == FOUR_PORT) u4ClockPD = 0x00;		
   	else u4ClockPD = 0x00;   
    
    #if DEBUG_MSG_TX
    Printf("u1ClockPD = 0x%8x\n",u4ClockPD);
    #endif

    if(((IS_PANEL_L12R12) && (u4LVDSPort == DUAL_PORT))||(IS_FORCE_LVDS_DIG_4CH_ON))
    {
      u4LinkPD = 0x00; 
      u4ClockPD = 0x00;
    }	

#ifdef CC_MT5881
    u4ClockPD = 0x00;  // Remy just has one clk pair, if need to swap 0DD/EVEN link, should enable linkA and linkB's clk.
#endif

	// Combin
    u4FinaleEnablePair = (u4LinkPD | u4EnablePair | (u4ClockPD << 20)) & 0xFFFFFF; 
	#ifndef NDEBUG
    Printf("u4FinaleEnablePair = 0x%8x\n",u4FinaleEnablePair); 
	#endif
	vIO32WriteFldAlign(LVDSB_REG07, u4FinaleEnablePair, RG_PD);
}     

// Power on all analog and digital PAD control
void vSetLVDSDAPadPDForceOn(void)
{
	vSetLVDSPadPD(0);
	vSetLVDSPadSourcePD(DISP_30BIT,FOUR_PORT);	
}

void u4ReadAPad(void)
{
#ifdef CC_MT5399
	printf("			   ECK E4 E3 E2 | E1 E0 OCK O4 | O3 O2 O1 O0\n");
	printf("LVDSA_EXT_EN:  <0x%3x>\n", IO32ReadFldAlign(REG_LVDS_TX_CFG11,RG_LVDSA_EXT_EN_D11));
	printf("LVDSA_DRV_PWD: <0x%3x>\n", IO32ReadFldAlign(REG_LVDS_TX_CFG12,RG_LVDSA_DRV_PWD));
#else
	printf("			   E5 O5 | ECK E4 E3 E2 | E1 E0 OCK O4 | O3 O2 O1 O0\n"); // Only for Capri
	printf("LVDSA_EXT_EN:  <0x%4x>\n", IO32ReadFldAlign(REG_LVDS_TX_CFG11,RG_LVDSA_EXT_EN_D11));
	printf("LVDSA_DRV_PWD: <0x%4x>\n", IO32ReadFldAlign(REG_LVDS_TX_CFG12,RG_LVDSA_DRV_PWD));
#endif
}
void vSetLVDSAPad(UINT8 u1OnOff, UINT16 u4BExtEn, UINT16 u4BDrvPwd,UINT16 u4AExtEn, UINT16 u4ADrvPwd)
{
	switch (u1OnOff)
	{
		case 0:  // power on
			vSetLVDSPadPD(_u4APadPDValue);
			break;
		case 1:  // power down
		#ifdef CC_MT5399
			vSetLVDSPadPD(0xFFF);
		#else
			vSetLVDSPadPD(0x3FFF);
		#endif
			break;
		case 2: // user-defined
			vIO32WriteFldAlign(REG_LVDS_TX_CFG11, u4AExtEn, RG_LVDSA_EXT_EN_D11);
   			vIO32WriteFldAlign(REG_LVDS_TX_CFG11, 0x1, RG_LVDSA_EXT_EN_D11_E0);
   			vIO32WriteFldAlign(REG_LVDS_TX_CFG12, u4ADrvPwd, RG_LVDSA_DRV_PWD);
			break;
		default:
			break;
	}
	u4ReadAPad();
}

static UINT32 u4DrvLVDSSetDispBit(void)
{
	UINT32 u4DispBit;

	u4DispBit = DISP_BIT;

#ifdef DRV_SUPPORT_EXTMJC
    if (IS_COMPANION_CHIP_ON())
    {
        if (vDrvGetCurLvdsMode() !=  eLvdsVidNormal)        
        {
            u4DispBit = DISP_30BIT;
        }
    }
#endif

	return (u4DispBit);
}

static UINT32 u4DrvLVDSSetDispPort(void)
{
	UINT32 u4LVDSPort;

	// for force power on lvds analog pad.
	if(IS_FORCE_LVDS_ANA_4CH_ON)
	{
		u4LVDSPort = FOUR_PORT; 
	}
	else if(IS_FORCE_LVDS_ANA_2CH_ON)
	{
    	u4LVDSPort = DUAL_PORT;	
	}
	else
	{
    	u4LVDSPort = LVDS_OUTPUT_PORT;
	}

#ifdef DRV_SUPPORT_EXTMJC
    if (IS_COMPANION_CHIP_ON())
    {
        if (vDrvGetCurLvdsMode() !=  eLvdsVidNormal)        
        {
            u4LVDSPort = DUAL_PORT;
        }
    }
#endif

	return (u4LVDSPort);
}

void u4ReadDPadPD(void)
{
	Printf("       CD CC CB CA | D4 D3 D2 D1 | D0 C4 C3 C2 | C1 C0 B4 B3 | B2 B1 B0 A4 | A3 A2 A1 A0\n");
	Printf("RG_PD: <0x%6x>\n", (IO32ReadFldAlign(LVDSB_REG07,RG_PD)));
}

void vSetLVDSDPadPD(UINT8 u1OnOff, UINT32 u4Value)
{
	switch (u1OnOff)
	{
		case 0:  // power on
			vSetLVDSPadSourcePD(u4DrvLVDSSetDispBit(),u4DrvLVDSSetDispPort());
			break;
		case 1:  // power down
			vIO32WriteFldAlign(LVDSB_REG07, 0xFFFFFF, RG_PD);
			break;
		case 2: // user-defined
			vIO32WriteFldAlign(LVDSB_REG07, (u4Value & 0xFFFFFF), RG_PD);
			break;
		default:
			break;
	}
	u4ReadDPadPD();
}

/**
 * @brief vSetPairSeqInverse

 * Control LVDS analog pad sequence for power on

 * @param  1: Inverse 0: Normal
 * @retval void
 */

UINT8 u1PairSeqInverse = 0;
void vSetPairSeqInverse(UINT8 u1Type)
{
// u1PairSeqInverse = 0;
// [MT5398] 		Analog related setting - 0: O0 ; 1: O1 ; 2: O2 ; 3: O3 ; 4: O4 ;		    5: OCK ; 6: E0 ; 7: E1 ; 8: E2 ;  9: E3 ;	10: E4 ;             11: ECK
// [MT5880/5860] Analog related setting - 0: O0 ; 1: O1 ; 2: O2 ; 3: O3 ; 4: O4 ; 5: O5 ; 6: OCK ; 7: E0 ; 8: E1 ; 9: E2 ; 10: E3 ; 11: E4 ; 12: E5 ; 13: ECK
// u1PairSeqInverse = 1;
// [MT5398]		 Analog related setting - 0: OCK; 1: O4 ; 2: O3 ; 3: O2 ; 4: O1 ;	     5: O0 ; 6: ECK ; 7: E4 ; 8: E3 ;  9: E3 ;	 10: E1 ;	           11: E0
// [MT5880/5860] Analog related setting - 0: OCK ; 1: O5 ; 2: O4 ; 3: O3 ; 4: O2 ; 5: O1 ; 6: O1 ; 7: ECK ; 8: E5 ; 9: E4 ; 10: E3 ; 11: E2 ; 12: E1 ; 13: E0

 u1PairSeqInverse = u1Type;
}

extern UINT8 _Lvds7To4FifoReset;
extern BOOL fgIsVsyncIsrStart;
#ifdef CONFIG_OPM
    UINT8 fgLVDSonoff = 0;
#else
    static UINT8 fgLVDSonoff = 0;
#endif
/**
 * @brief vDrvLVDSOn

 * Power On LVDS

 * @param  void
 * @retval void
 */
void vDrvLVDSOn(void)
{
    UINT32 u4DispBit;
    UINT32 u4LVDSPort;
    UINT32 u4PDValue = 0;
    UINT32 u4TwoChMerge;
	UINT32 u4LvdsCtrlAEven;
	UINT32 *pu4PinMap;
	  
    if ((IO32ReadFldAlign(DDDS_08, FLAG_LVDS_ONOFF) == 1)&& (!(IS_PANEL_L12R12)))
    {
        fgLVDSonoff = TRUE;
    }


	if (!fgLVDSonoff)
	{
	#ifdef CC_MT5399
    vIO32WriteFldMulti(REG_LVDS_TX_CFG6, P_Fld(0x3fff,RG_LVDSB_R_TERM_EVEN_D11)| P_Fld(0x3fff,RG_LVDSB_R_TERM_ODD_D11));   
    #endif
  	
    
	//A group power on sequence
    vIO32WriteFldMulti(REG_LVDS_TX_CFG11,P_Fld( 0, RG_LVDSA_BIAS_ODD_PWD)|P_Fld( 0, RG_LVDSA_BIAS_EVEN_PWD));
    vIO32WriteFldMulti(REG_LVDS_TX_CFG19,P_Fld( 0, RG_LVDSA_LDO_BIAS_ODD_PWD_D11)|P_Fld( 0, RG_LVDSA_LDO_BIAS_EVEN_PWD_D11));
    vIO32WriteFldMulti(REG_LVDS_TX_CFG19, P_Fld( 0, RG_LVDSA_LDO_ODD_PWD_D11)|P_Fld( 0, RG_LVDSA_LDO_EVEN_PWD_D11));
	vIO32WriteFldAlign(REG_LVDS_TX_CFG11, 1, RG_LVDSA_EN); 
	#ifdef CC_MT5399
    //B group power on sequence
    vIO32WriteFldAlign(REG_LVDS_TX_CFG0, 1,RG_LVDSB_BIAS_EN_D11 );
	vIO32WriteFldAlign(REG_LVDS_TX_CFG0, 1, RG_LVDSB_EN_D11 );;  
	#endif

	u4DispBit = DISP_BIT;

	// for force power on lvds analog pad.
	if(IS_FORCE_LVDS_ANA_4CH_ON)
	{
		u4LVDSPort = FOUR_PORT; 
	}
	else if(IS_FORCE_LVDS_ANA_2CH_ON)
	{
    	u4LVDSPort = DUAL_PORT;	
	}
	else
	{
    	u4LVDSPort = LVDS_OUTPUT_PORT;
	}	
	
    u4TwoChMerge = (LVDS_OUTPUT_PORT == DUAL_PORT)? 1: 0;
    #ifdef DRV_SUPPORT_EXTMJC
    if (IS_COMPANION_CHIP_ON())
    {
        if (vDrvGetCurLvdsMode() !=  eLvdsVidNormal)        
        {
            u4DispBit = DISP_30BIT;
            u4LVDSPort = DUAL_PORT;

            if ((vDrvGetCurLvdsMode() == eLvdsVidYuv422TwoPort8Bit) || (vDrvGetCurLvdsMode() == eLvdsVidYuv422TwoPort) || (vDrvGetCurLvdsMode() == eLvdsVidRgb444TwoPort))
            {
                u4TwoChMerge = 1;
            }
            else if ((vDrvGetCurLvdsMode() == eLvdsVidYuv422OnePort) || (vDrvGetCurLvdsMode() == eLvdsVidRgb444OnePort) || (vDrvGetCurLvdsMode() == eLvdsVidYuv422OnePort8Bit) || (vDrvGetCurLvdsMode() == eLvdsVidRgb444OnePort8Bit))
            {
                u4TwoChMerge = 0;
            }
        }
    }
    #endif

	if(LVDS_DISP_ODD_SW)
	{
		u4LvdsCtrlAEven = 1;
	}
	else if (LVDS_USE_INDEPENDENT_SETTING)
	{
	    if ((LVDS_A_INDEPENDENT_SETTING ==1) || (LVDS_A_INDEPENDENT_SETTING ==0))
	    {
			u4LvdsCtrlAEven = LVDS_A_INDEPENDENT_SETTING;
	    }	
		else
		{
			u4LvdsCtrlAEven = 0;
		}
	}	
	else
	{
		u4LvdsCtrlAEven = DRVCUST_PanelGet(eLvdsACtrlEven);
	}	
            
    switch (u4DispBit)
    {
        case DISP_30BIT:
            // enable LVDS 10-bit
            vIO32WriteFldAlign(LVDSB_REG06, 1, RG_10B_EN);                
			#ifdef CC_MT5399
            if(u4LVDSPort == FOUR_PORT)
            {
                vIO32WriteFldAlign(LVDSB_REG06, 1, RG_4CH);                
                vIO32WriteFldAlign(LVDSB_REG06, 0, RG_DUAL);
				u4PDValue = 0;
            }
            else       
			#endif
            if (u4LVDSPort == DUAL_PORT)
            {
				if (IS_PANEL_L12R12)
				{            
                 vIO32WriteFldAlign(LVDSB_REG06, 1, RG_4CH);                
                 vIO32WriteFldAlign(LVDSB_REG06, 0, RG_DUAL);                
				}
				else
				{            
                 vIO32WriteFldAlign(LVDSB_REG06, 0, RG_4CH);                
                 vIO32WriteFldAlign(LVDSB_REG06, u4TwoChMerge, RG_DUAL);                
				}					
                
                #ifdef CC_MT5399
                u4PDValue = 0x00fff000; // PD BO/BE
                #else
				if (u1PairSeqInverse)
                u4PDValue = 0x0003;				
				else					
                u4PDValue = 0x3000;   // PD O5, E5
                #endif
            }
            else  // single port
            {
                vIO32WriteFldAlign(LVDSB_REG06, 0, RG_4CH);                
                vIO32WriteFldAlign(LVDSB_REG06, 0, RG_DUAL);                
                if (u4LvdsCtrlAEven == 1)  // power down Port A-Odd
                {
                	#ifdef CC_MT5399
					if (u1PairSeqInverse)
					u4PDValue = 0x00ffffC0;
					else
                    u4PDValue = 0x00fff03f; // PD BO/BE/AO
                    #else
					if (u1PairSeqInverse)
                    u4PDValue = 0x3F03;
                    else
                    u4PDValue = 0x303F;     // PD O0~O5, OCK, E5
					#endif
                }
                else if (u4LvdsCtrlAEven == 0) // power down Port A-Even
                {
                	#ifdef CC_MT5399
					if (u1PairSeqInverse)
                    u4PDValue = 0x00ffff3f;
					else	
                    u4PDValue = 0x00ffffc0; // PD BO/BE/AE
                    #else
					if (u1PairSeqInverse)
                    u4PDValue = 0x00FF;
                    else					
					u4PDValue = 0x3FC0;     // PD E0~E5, ECK, O5
					#endif
                }
            }
            break;

        case DISP_24BIT:
            // disable LVDS 10-bit
            vIO32WriteFldAlign(LVDSB_REG06, 0, RG_10B_EN);
			#ifdef CC_MT5399
            if(u4LVDSPort == FOUR_PORT)
            {
                vIO32WriteFldAlign(LVDSB_REG06, 1, RG_4CH);                
                vIO32WriteFldAlign(LVDSB_REG06, 0, RG_DUAL);
				if (u1PairSeqInverse)
				u4PDValue = 0x00082082;
				else					
				u4PDValue = 0x00410410;  // PD O4, E4 
            }
            else
			#endif
            if (u4LVDSPort == DUAL_PORT)
            {
				if (IS_PANEL_L12R12)
				{            
                 vIO32WriteFldAlign(LVDSB_REG06, 1, RG_4CH);                
                 vIO32WriteFldAlign(LVDSB_REG06, 0, RG_DUAL);
				}
				else
				{            
                 vIO32WriteFldAlign(LVDSB_REG06, 0, RG_4CH);                
                 vIO32WriteFldAlign(LVDSB_REG06, 1, RG_DUAL);
				}
				#ifdef CC_MT5399
				if (u1PairSeqInverse)
				u4PDValue = 0x00fff082;
				else
                u4PDValue = 0x00fff410;   // PD BO/BE/AE4/AO4
                #else
				if (u1PairSeqInverse)
				u4PDValue = 0x020B;
				else
                u4PDValue = 0x3410;   // PD O5, O4, E5, E4
				#endif
            }
            else  // single port
            {
                vIO32WriteFldAlign(LVDSB_REG06, 0, RG_4CH);                
                vIO32WriteFldAlign(LVDSB_REG06, 0, RG_DUAL);
                if (u4LvdsCtrlAEven == 1)  // power down Port A-Odd
                {
                	#ifdef CC_MT5399
					if (u1PairSeqInverse)
                    u4PDValue = 0x00ffffC2;
                    else
                    u4PDValue = 0x00fff43f;   // PD BO/BE/AO/AE4
                    #else
					if (u1PairSeqInverse)
                    u4PDValue = 0x3F0B;
                    else
                    u4PDValue = 0x343F;     // PD O0~O5, OCK, E5, E4
					#endif
                }
                else if (u4LvdsCtrlAEven == 0) // power down Port A-Even
                {
                	#ifdef CC_MT5399
					if (u1PairSeqInverse)
                    u4PDValue = 0x00fff0bf;
                    else					
                    u4PDValue = 0x00ffffd0;   // PD BO/BE/AE/AO4
                    #else
					if (u1PairSeqInverse)
                    u4PDValue = 0x02FF;
                    else					
                    u4PDValue = 0x3FD0;     // PD E0~E5, ECK, O5, O4
					#endif
                }
            }
            break;

        case DISP_18BIT:
            // disable LVDS 10-bit
            vIO32WriteFldAlign(LVDSB_REG06, 0, RG_10B_EN);                
            if (u4LVDSPort == DUAL_PORT)
            {
                vIO32WriteFldAlign(LVDSB_REG06, 0, RG_4CH);                
                vIO32WriteFldAlign(LVDSB_REG06, 1, RG_DUAL);
				#ifdef CC_MT5399
				if (u1PairSeqInverse)
				u4PDValue = 0x00fff186;
				else
				u4PDValue = 0x00fff618;
				#else
				if (u1PairSeqInverse)
				u4PDValue = 0x061B;					
				else
				u4PDValue = 0x3618; // PD 05, 04 , 03, E5, E4, E3
				#endif
           }
            else  // single port
            {
                vIO32WriteFldAlign(LVDSB_REG06, 0, RG_4CH);                
                vIO32WriteFldAlign(LVDSB_REG06, 0, RG_DUAL);
                if (u4LvdsCtrlAEven == 1)  // power down Port A-Odd
                {
                	#ifdef CC_MT5399
					if (u1PairSeqInverse)
					u4PDValue = 0x00ffffc6;						
					else	
					u4PDValue = 0x00fff63f;
					#else
					if (u1PairSeqInverse)
					u4PDValue = 0x3F1B;
					else
					u4PDValue = 0x363F;  // PD O0~O5, OCK, E5, E4, E3
					#endif
                }
                else if (u4LvdsCtrlAEven == 0) // power down Port A-Even
                {
                	#ifdef CC_MT5399
					if (u1PairSeqInverse)
					u4PDValue = 0x00fff1bf;						
					else
					u4PDValue = 0x00ffffd8;
					#else
					if (u1PairSeqInverse)
					u4PDValue = 0x06FF;
					else					
					u4PDValue = 0x3FD8;  // PD E0~E5, ECK, O5 ,O4 ,O3
					#endif
                }
            }
            break;

        default:
            break;
    }

	if(!(NULL == (pu4PinMap = (UINT32 *)DRVCUST_PanelGet(ePanelLvds10bitPinMap))))
	{
		u4PDValue = 0; // force powen on all pair
	}

    // Analog Pair Power down control
    _u4APadPDValue = u4PDValue;
	vSetLVDSPadPD(u4PDValue);
	// Some case the digital pad control is different with analog pad control by LVDS Port Contrl (ex: OSD and Data seperated)
	u4LVDSPort = LVDS_OUTPUT_PORT;
    // Digital Pair Power down control
	vSetLVDSPadSourcePD(u4DispBit,u4LVDSPort);

	#ifdef LVDS_ESD_PROTECT
	#ifdef CC_MT5399
	u1CheckLVDSClockSkew_Enable(1);
	#endif
	#endif
	
    #ifdef CC_COPLAT_MT82
    vUtDelay2us(5);
    #else
    HAL_Delay_us(10);
    #endif
		
	vIO32WriteFldAlign(REG_LVDS_TX_CFG11, 0,RG_LVDSA_EN);	
	HAL_Delay_us(LVDSA_EN_REST_DELAYTIME);
	vIO32WriteFldAlign(REG_LVDS_TX_CFG11, 1,RG_LVDSA_EN);
	#ifdef CC_MT5399
	vIO32WriteFldAlign(REG_LVDS_TX_CFG0, 0,RG_LVDSB_EN_D11 );	
	HAL_Delay_us(LVDSA_EN_REST_DELAYTIME);
	vIO32WriteFldAlign(REG_LVDS_TX_CFG0, 1,RG_LVDSB_EN_D11 );
	#endif
	
    #ifdef CC_MTK_LOADER    
    vDrvLVDS7To4FifoEnable();   
	#else
	if (fgIsVsyncIsrStart)
	{
		_Lvds7To4FifoReset = 2;
	}
	else
	{
		LOG(0, "[LVDS]NO output Vsync! toggle FIFO immediately!\n");
		vDrvLVDS7To4FifoEnable();
	}	
	#endif
	#ifndef NDEBUG
    Printf(" ======= [SA7] vDrvLVDSOn toggele RG_LVDSA_EN ===========\n");
	#endif
	
	#ifdef LVDS_ESD_PROTECT
	#ifdef CC_MT5882
		u2LVDSFifoSkwPre = u1ChkLVDSFIFOSkewStatus();
	#endif
	#endif

    #ifdef CC_EXTERNAL_LVDS_CHIP 
    IMpac3P_SetLvds(1);
    #endif
	#ifdef CC_EXTMJC_WA	
	UINT8 u1MsgErr;
	drv_extmjc_reset_rx_tx(100,&u1MsgErr);
	#endif	
    fgLVDSonoff = TRUE;
    vIO32WriteFldAlign(DDDS_08, 1, FLAG_LVDS_ONOFF);
	}
}

/**
 * @brief vDrvLVDSOff

 * Power Down LVDS

 * @param  void
 * @retval void
 */
void vDrvLVDSOff(void)
{
    // power down all digital channel
    vIO32WriteFldAlign(LVDSB_REG07, 0xffffff, RG_PD);                                           
 
	//A group power off sequence
    vIO32WriteFldMulti(REG_LVDS_TX_CFG11, P_Fld(0, RG_LVDSA_EXT_EN_D11)|P_Fld(0,RG_LVDSA_EN));
	#ifdef CC_MT5399
    vIO32WriteFldAlign(REG_LVDS_TX_CFG12, 0xFFF, RG_LVDSA_DRV_PWD);
	#else
	vIO32WriteFldAlign(REG_LVDS_TX_CFG12, 0x3FFF, RG_LVDSA_DRV_PWD);
	#endif
    vIO32WriteFldMulti(REG_LVDS_TX_CFG19, P_Fld( 1, RG_LVDSA_LDO_ODD_PWD_D11)|P_Fld( 1, RG_LVDSA_LDO_EVEN_PWD_D11));
    vIO32WriteFldMulti(REG_LVDS_TX_CFG19,P_Fld( 1, RG_LVDSA_LDO_BIAS_ODD_PWD_D11)|P_Fld( 1, RG_LVDSA_LDO_BIAS_EVEN_PWD_D11));
    vIO32WriteFldMulti(REG_LVDS_TX_CFG11,P_Fld( 1, RG_LVDSA_BIAS_ODD_PWD)|P_Fld( 1, RG_LVDSA_BIAS_EVEN_PWD));
	#ifdef CC_MT5399
    //B group power off sequence
    vIO32WriteFldAlign(REG_LVDS_TX_CFG0, 0,RG_LVDSB_EN_D11 );    
    vIO32WriteFldMulti(REG_LVDS_TX_CFG1, P_Fld(0, RG_LVDSB_DRV_EN_EVEN_D11)|P_Fld(0,RG_LVDSB_DRV_EN_ODD_D11));  
    vIO32WriteFldAlign(REG_LVDS_TX_CFG0, 0, RG_LVDSB_BIAS_EN_D11 );
	#endif
	
	#ifdef CC_EXTERNAL_LVDS_CHIP 
    IMpac3P_SetLvds(0);
	#endif

	 fgLVDSonoff = FALSE;
	 vIO32WriteFldAlign(DDDS_08, 0, FLAG_LVDS_ONOFF);
}

/**
 * @brief vDrvLVDSConfig

 * Power Down LVDS

 * @param  void
 * @retval void
 */
void vDrvLVDSConfig(void)
{   
    // set lvds format
    _vDrvLVDSConfigFormat();

    if (LVDS_DISP_ODD_SW == LVDS_ODD_SW_ON)
    {
        if (LVDS_OUTPUT_PORT == SINGLE_PORT)
        {
         vIO32WriteFldAlign(LVDSB_REG06, 0, RG_ODD_SW); 
         vIO32WriteFldAlign(LVDSB_REG06, 1, RG_CH_SW);        
        }
        else
        {
         vIO32WriteFldAlign(LVDSB_REG06, 1, RG_ODD_SW); 
		 vIO32WriteFldAlign(LVDSB_REG06, 0, RG_CH_SW); 
        } 
    }
    else
    {
        if (LVDS_OUTPUT_PORT == SINGLE_PORT)
        {
         vIO32WriteFldAlign(LVDSB_REG06, 0, RG_CH_SW);        
        }
        else
        {    
        vIO32WriteFldAlign(LVDSB_REG06, 0, RG_ODD_SW);                
        }
    }

    if (HSYNC_POLARITY == HSYNC_LOW)
    {  
        vIO32WriteFldAlign(LVDSB_REG00, 1, RG_HSYNC_P);                
    }
    else
    {        
        vIO32WriteFldAlign(LVDSB_REG00, 0, RG_HSYNC_P);                
    }

    if (VSYNC_POLARITY == VSYNC_LOW)
    {       
        vIO32WriteFldAlign(LVDSB_REG00, 1, RG_VSYNC_P);                
        vIO32WriteFldAlign(LVDSB_REG01, 1, RG_IBUF_RST_SEL);                
    }
    else
    {
        vIO32WriteFldAlign(LVDSB_REG00, 0, RG_VSYNC_P);                
        vIO32WriteFldAlign(LVDSB_REG01, 0, RG_IBUF_RST_SEL);                
    }


    // Lvds port swap control
    _vDrvLVDSSetPortMap();
   
}

void u1TCONIFSourceSet(UINT8 val)
{
  if (val<=2)
  {
   vIO32WriteFldAlign(VINTF_P0, val, VINTF_PANEL_IN_SEL);   
  } 
}

UINT8 u1TCONIFSourceGet(void)
{
  return (IO32ReadFldAlign(VINTF_P0, VINTF_PANEL_IN_SEL));
}

void u1TCONIFLineBufferSet(UINT8 val)
{
   if (val<=1)
   {
    vIO32WriteFldAlign(VINTF_B0, val, VINTF_LINE_BUFFER_BYPASS);   
   }	
}

UINT8 u1TCONIFLineBufferGet(void)
{
  return (IO32ReadFldAlign(VINTF_B0, VINTF_LINE_BUFFER_BYPASS));
}


void vDrvLVDS7To4FifoEnable(void)
{
    UINT8 u1Unmute;
	
	#ifndef NDEBUG
	Printf("[LVDS] vDrvLVDS7To4FifoEnable !\n");
	#endif

    if(IO32ReadFldAlign(MUTE_00, R_MUTE_POST_EN) ==1 )   
    {
	  u1Unmute = 0;
    }
	else
	{
      vIO32WriteFldAlign(MUTE_00, 1,  R_MUTE_POST_EN);
	  u1Unmute = 1;
	}  
    vIO32WriteFldAlign(LVDSB_REG08, 0, RG_LVDS_7TO10FIFO_EN);                
    vIO32WriteFldAlign(LVDSB_REG08, 1, RG_LVDS_7TO10FIFO_EN);                
    
    vIO32WriteFldAlign(MLVDS_CTRL_09, 1, ASYNC_FIFO_RST);   
    vIO32WriteFldAlign(MLVDS_CTRL_09, 0, ASYNC_FIFO_RST);   

	if(PANEL_GetDisplayType()== PANEL_DISPLAY_TYPE_EPI)
    {
	//EPI FIFO Reset
	HAL_Delay_us(LVDSA_EN_REST_DELAYTIME);
    vIO32WriteFldAlign(EPI_T5, 0, RG_FIFO_EN);
	HAL_Delay_us(LVDSA_EN_REST_DELAYTIME);
    vIO32WriteFldAlign(EPI_T5, 1, RG_FIFO_EN);
	}
	
    if(u1Unmute)
    vIO32WriteFldAlign(MUTE_00, 0, R_MUTE_POST_EN);
}

//automation cli 
void LVDS_ATERM_ATVO_Setting(UINT8 mode)
{
   if(mode==1)
   {
	vIO32WriteFldAlign(REG_LVDS_TX_CFG11,8,RG_LVDSA_TVO);
	#ifdef CC_MT5399
    LVDSTVO=vDrvLVDSsettingDrivingCurrent(4);
    vIO32WriteFldAlign(REG_LVDS_TX_CFG3,  LVDSTVO, RG_LVDSB_TVO_EVEN_D11);
    vIO32WriteFldAlign(REG_LVDS_TX_CFG4, LVDSTVO, RG_LVDSB_TVO_ODD_D11);
    vIO32WriteFldMulti(REG_LVDS_TX_CFG6, P_Fld(0x3fff,RG_LVDSB_R_TERM_EVEN_D11)| P_Fld(0x3fff,RG_LVDSB_R_TERM_ODD_D11)); 
	#endif
   }
   else if(mode==2)
   {
	
	vIO32WriteFldAlign(REG_LVDS_TX_CFG11,4,RG_LVDSA_TVO);
	#ifdef CC_MT5399
    LVDSTVO=vDrvLVDSsettingDrivingCurrent(4);
    vIO32WriteFldAlign(REG_LVDS_TX_CFG3,  LVDSTVO, RG_LVDSB_TVO_EVEN_D11);
    vIO32WriteFldAlign(REG_LVDS_TX_CFG4, LVDSTVO, RG_LVDSB_TVO_ODD_D11);
    vIO32WriteFldMulti(REG_LVDS_TX_CFG6, P_Fld(0x1555,RG_LVDSB_R_TERM_EVEN_D11)| P_Fld(0x1555,RG_LVDSB_R_TERM_ODD_D11));
	#endif
   }	
   else if(mode==3)
   {
	vIO32WriteFldAlign(REG_LVDS_TX_CFG11,8,RG_LVDSA_TVO);
	#ifdef CC_MT5399
    LVDSTVO=vDrvLVDSsettingDrivingCurrent(4);
    vIO32WriteFldAlign(REG_LVDS_TX_CFG3,  LVDSTVO, RG_LVDSB_TVO_EVEN_D11);
    vIO32WriteFldAlign(REG_LVDS_TX_CFG4, LVDSTVO, RG_LVDSB_TVO_ODD_D11);
	#endif
   }			   
}

void LVDS_ATERM_ATVO_Restore(UINT32 ATERN, UINT8 ATVO)
{
	vIO32WriteFldAlign(REG_LVDS_TX_CFG11,(ATVO+4),RG_LVDSA_TVO);
	#ifdef CC_MT5399
    LVDSTVO=vDrvLVDSsettingDrivingCurrent(ATVO);
    vIO32WriteFldAlign(REG_LVDS_TX_CFG3,  LVDSTVO, RG_LVDSB_TVO_EVEN_D11);
    vIO32WriteFldAlign(REG_LVDS_TX_CFG4, LVDSTVO, RG_LVDSB_TVO_ODD_D11);
    vIO32WriteFldAlign(REG_LVDS_TX_CFG6, ATERN, RG_LVDSB_R_TERM_EVEN_D11);
    vIO32WriteFldAlign(REG_LVDS_TX_CFG6, ATERN, RG_LVDSB_R_TERM_ODD_D11);
	#endif
}

void LVDS_ATVO_Set(UINT8 mode)
{
	vIO32WriteFldAlign(REG_LVDS_TX_CFG11,(mode+4),RG_LVDSA_TVO);
	#ifdef CC_MT5399
    LVDSTVO=vDrvLVDSsettingDrivingCurrent(mode);
    vIO32WriteFldAlign(REG_LVDS_TX_CFG3,  LVDSTVO, RG_LVDSB_TVO_EVEN_D11);
    vIO32WriteFldAlign(REG_LVDS_TX_CFG4, LVDSTVO, RG_LVDSB_TVO_ODD_D11);
	#endif
}

void LVDS_APSRC_Set(UINT8 mode)
{
	vIO32WriteFldAlign(REG_LVDS_TX_CFG11,mode,RG_LVDSA_PSRC);
}

void LVDS_ANSRC_Set(UINT8 mode)
{
	vIO32WriteFldAlign(REG_LVDS_TX_CFG11,mode,RG_LVDSA_NSRC);
}

void LVDS_PADPD_Set(UINT8 mode)
{
    if(mode == LVDS_PWDOWN)
    {
    vIO32WriteFldAlign(REG_LVDS_TX_CFG11, 0x0, RG_LVDSA_EXT_EN_D11);
	#ifdef CC_MT5399
	vIO32WriteFldMulti(REG_LVDS_TX_CFG1, P_Fld(0, RG_LVDSB_DRV_EN_EVEN_D11)|P_Fld(0,RG_LVDSB_DRV_EN_ODD_D11));
    #endif
    }
    else
   	{
    #ifdef CC_MT5399
    vIO32WriteFldAlign(REG_LVDS_TX_CFG11, 0xFFF, RG_LVDSA_EXT_EN_D11);
	vIO32WriteFldMulti(REG_LVDS_TX_CFG1, P_Fld(0xFFF, RG_LVDSB_DRV_EN_EVEN_D11)|P_Fld(0xFFF,RG_LVDSB_DRV_EN_ODD_D11));
    #else
 	vIO32WriteFldAlign(REG_LVDS_TX_CFG11, 0x3FFF, RG_LVDSA_EXT_EN_D11);
    #endif
    }
}

void LVDS_RESET_Set(UINT8 mode)
{
}

void MLVDS_LS_Set(UINT8 mode)
{
   if (mode==0)
   {
        vIO32WriteFldMulti(REG_LVDS_TX_CFG11, P_Fld(1, RG_LVDSA_NSRC)|P_Fld(1, RG_LVDSA_PSRC));
   } 
   else
   {
        vIO32WriteFldMulti(REG_LVDS_TX_CFG11, P_Fld(0, RG_LVDSA_NSRC)|P_Fld(7, RG_LVDSA_PSRC));
   }
}

void MLVDS_TestMODE_Set(UINT8 mode)
{
  vIO32WriteFldAlign(MLVDS_CTRL_09,mode,RG_MLVDSM_ANA_TEST);
  vIO32WriteFldAlign(MLVDS_CTRL_09,0x155,RG_MLVDSM_ANA_FORCE);  
}

void vDrvVCOPhase_SEL(UINT8 u1Phase)
{
	vIO32WriteFldAlign(REG_VOPLL_CFG1, u1Phase, RG_VPLL_DLY_DATA);
}

void SET_DDDSFreeRun(void)
{
  vIO32WriteFldAlign(DDDS_00,0,DDDS_00_DISP_EN);
  vIO32WriteFldAlign(DDDS_00,0,DDDS_00_FIX_FS_DDDS_SEL);			
  vIO32WriteFldAlign(DDDS_03,0,DDDS_03_SPREAD_INIT);
  vIO32WriteFldAlign(DDDS_03,1,DDDS_03_SPREAD_INIT);
  vIO32WriteFldAlign(DDDS_03,0,DDDS_03_SPREAD_INIT);						
}

void vSetPI_GROUP(UINT8 u1AEven, UINT8 u1AODD, UINT8 u1BEven, UINT8 u1BOdd)
{
 vIO32WriteFldAlign(LVDSB_REG08, u1AEven, RG_A_SW);
 vIO32WriteFldAlign(LVDSB_REG08, u1AODD,  RG_B_SW);
 vIO32WriteFldAlign(LVDSB_REG08, u1BEven, RG_C_SW);
 vIO32WriteFldAlign(LVDSB_REG08, u1BOdd,  RG_D_SW);
}

UINT8 vGetPI_GROUP(UINT8 u1Link)
{
 if(u1Link == 0)
 return (IO32ReadFldAlign(LVDSB_REG08, RG_A_SW));
 else if(u1Link == 1)
 return (IO32ReadFldAlign(LVDSB_REG08, RG_B_SW));
 else if(u1Link == 2) 
 return (IO32ReadFldAlign(LVDSB_REG08, RG_C_SW));
 else if(u1Link == 3)  
 return (IO32ReadFldAlign(LVDSB_REG08, RG_D_SW));
 else
 return 0;
}

void vSet_DataPair(UINT8 u1Port,UINT8 u1Pair, UINT8 u1Value)
{
    if(u1Port)  // Port B
    {
		if(u1Pair == 0)
		 vIO32WriteFldAlign(LVDSB_REG03, u1Value, RG_RLV0_SEL);
		else if(u1Pair == 1)
		 vIO32WriteFldAlign(LVDSB_REG03, u1Value, RG_RLV1_SEL);
		else if(u1Pair == 2)
		 vIO32WriteFldAlign(LVDSB_REG03, u1Value, RG_RLV2_SEL);
		else if(u1Pair == 3)
		 vIO32WriteFldAlign(LVDSB_REG03, u1Value, RG_RLV3_SEL);
		else if(u1Pair == 4)
		 vIO32WriteFldAlign(LVDSB_REG03, u1Value, RG_RLV4_SEL);
		else if(u1Pair == 5)
		 vIO32WriteFldAlign(LVDSB_REG03, u1Value, RG_RLV5_SEL);	
		else if(u1Pair == 6)
		 vIO32WriteFldAlign(LVDSB_REG03, u1Value, RG_RLV6_SEL);		
		else if(u1Pair == 7)
		 vIO32WriteFldAlign(LVDSB_REG03, u1Value, RG_RLV7_SEL);		
		else if(u1Pair == 8)
		 vIO32WriteFldAlign(LVDSB_REG04, u1Value, RG_RLV8_SEL);			
		else if(u1Pair == 9)
		 vIO32WriteFldAlign(LVDSB_REG04, u1Value, RG_RLV9_SEL);			
		else if(u1Pair == 0x0A)
		 vIO32WriteFldAlign(LVDSB_REG04, u1Value, RG_RLV_CK0_SEL);			
		else if(u1Pair == 0x0B)
		 vIO32WriteFldAlign(LVDSB_REG04, u1Value, RG_RLV_CK1_SEL);		
    }
	else  // Port A
	{
		if(u1Pair == 0)
		 vIO32WriteFldAlign(LVDSB_REG02, u1Value, RG_LLV0_SEL);
		else if(u1Pair == 1)
		 vIO32WriteFldAlign(LVDSB_REG02, u1Value, RG_LLV1_SEL);
		else if(u1Pair == 2)
		 vIO32WriteFldAlign(LVDSB_REG02, u1Value, RG_LLV2_SEL);
		else if(u1Pair == 3)
		 vIO32WriteFldAlign(LVDSB_REG02, u1Value, RG_LLV3_SEL);
		else if(u1Pair == 4)
		 vIO32WriteFldAlign(LVDSB_REG02, u1Value, RG_LLV4_SEL);
		else if(u1Pair == 5)
		 vIO32WriteFldAlign(LVDSB_REG02, u1Value, RG_LLV5_SEL);	
		else if(u1Pair == 6)
		 vIO32WriteFldAlign(LVDSB_REG02, u1Value, RG_LLV6_SEL);		
		else if(u1Pair == 7)
		 vIO32WriteFldAlign(LVDSB_REG02, u1Value, RG_LLV7_SEL);		
		else if(u1Pair == 8)
		 vIO32WriteFldAlign(LVDSB_REG04, u1Value, RG_LLV8_SEL);			
		else if(u1Pair == 9)
		 vIO32WriteFldAlign(LVDSB_REG04, u1Value, RG_LLV9_SEL);			
		else if(u1Pair == 0x0A)
		 vIO32WriteFldAlign(LVDSB_REG04, u1Value, RG_LLV_CK0_SEL);			
		else if(u1Pair == 0x0B)
		 vIO32WriteFldAlign(LVDSB_REG04, u1Value, RG_LLV_CK1_SEL);		
	}
}

UINT8 u1Get_DataPair(UINT8 u1Port,UINT8 u1Pair)
{
    if(u1Port)  // Port B
    {
		if(u1Pair == 0x00)
		return (IO32ReadFldAlign(LVDSB_REG03, RG_RLV0_SEL));
		else if(u1Pair == 0x01)
		return (IO32ReadFldAlign(LVDSB_REG03, RG_RLV1_SEL));
		else if(u1Pair == 0x02)
		return (IO32ReadFldAlign(LVDSB_REG03, RG_RLV2_SEL));
		else if(u1Pair == 0x03)
		return (IO32ReadFldAlign(LVDSB_REG03, RG_RLV3_SEL));
		else if(u1Pair == 0x04)
		return (IO32ReadFldAlign(LVDSB_REG03, RG_RLV4_SEL));
		else if(u1Pair == 0x05)
		return (IO32ReadFldAlign(LVDSB_REG03, RG_RLV5_SEL));	
		else if(u1Pair == 0x06)
		return (IO32ReadFldAlign(LVDSB_REG03, RG_RLV6_SEL));		
		else if(u1Pair == 0x07)
		return (IO32ReadFldAlign(LVDSB_REG03, RG_RLV7_SEL));		
		else if(u1Pair == 0x08)
		return (IO32ReadFldAlign(LVDSB_REG04, RG_RLV8_SEL));			
		else if(u1Pair == 0x09)
		return (IO32ReadFldAlign(LVDSB_REG04, RG_RLV9_SEL));			
		else if(u1Pair == 0x0A)
		return (IO32ReadFldAlign(LVDSB_REG04, RG_RLV_CK0_SEL));			
		else if(u1Pair == 0x0B)
		return (IO32ReadFldAlign(LVDSB_REG04, RG_RLV_CK1_SEL));			
		else
		return 0;		
    }
	else  // Port A
	{
		if(u1Pair == 0x00)
		return (IO32ReadFldAlign(LVDSB_REG02, RG_LLV0_SEL));
		else if(u1Pair == 0x01)
		return (IO32ReadFldAlign(LVDSB_REG02, RG_LLV1_SEL));
		else if(u1Pair == 0x02)
		return (IO32ReadFldAlign(LVDSB_REG02, RG_LLV2_SEL));
		else if(u1Pair == 0x03)
		return (IO32ReadFldAlign(LVDSB_REG02, RG_LLV3_SEL));
		else if(u1Pair == 0x04)
		return (IO32ReadFldAlign(LVDSB_REG02, RG_LLV4_SEL));
		else if(u1Pair == 0x05)
		return (IO32ReadFldAlign(LVDSB_REG02, RG_LLV5_SEL));	
		else if(u1Pair == 0x06)
		return (IO32ReadFldAlign(LVDSB_REG02, RG_LLV6_SEL));		
		else if(u1Pair == 0x07)
		return (IO32ReadFldAlign(LVDSB_REG02, RG_LLV7_SEL));		
		else if(u1Pair == 0x08)
		return (IO32ReadFldAlign(LVDSB_REG04, RG_LLV8_SEL));			
		else if(u1Pair == 0x09)
		return (IO32ReadFldAlign(LVDSB_REG04, RG_LLV9_SEL));	
		else if(u1Pair == 0x0A)
		return (IO32ReadFldAlign(LVDSB_REG04, RG_LLV_CK0_SEL));			
		else if(u1Pair == 0x0B)
		return (IO32ReadFldAlign(LVDSB_REG04, RG_LLV_CK1_SEL));			
		else
		return 0;
	}
}	

 /**

 * @brief vDrvSetVOPLLClockSchema
 *
 * Config VOPLL clock schema
 *
 * @param  u1ClockSrc: vopll clock source 
 * @retval void 
 */
void vDrvSetVOPLLClockSchema(UINT8 u1ClockSrc)
{
    LOG(3,"vDrvSetVOPLLClockSchema, u1ClockSrc=%d.\n", u1ClockSrc);

    vIO32WriteFldAlign(CKGEN_REG_XDDS_CKCFG, u1ClockSrc, FLD_XDDS_SEL);
	switch (u1ClockSrc)
	{
		case VOPLL_CLK_SRC_DDDS:
			
#if defined (CC_MT5399) || defined(CC_MT5880) || defined (CC_MT5882)
			if (IS_PANEL_L12R12)
			vIO32WriteFldAlign(CKGEN_OCLK_TEST, 1, FLD_OCLK_DIV2_SEL);//for SS-XGA 240P and SP-PDP panel, set /2
            else
			vIO32WriteFldAlign(CKGEN_OCLK_TEST, 0, FLD_OCLK_DIV2_SEL);
#endif

            if (LVDS_OUTPUT_PORT == SINGLE_PORT)
            {
#if defined (CC_MT5399) || defined(CC_MT5880) || defined (CC_MT5882)
			  vIO32WriteFldAlign(CKGEN_OCLK_TEST, 0, FLD_OCLK_DIV_SEL);
#else
//              vIO32WriteFldAlign(CKGEN_VPCLK_CFG, 0, FLD_OCLK_DIV_SEL);
#endif
            }
            else if (LVDS_OUTPUT_PORT == DUAL_PORT)
            {
#if defined (CC_MT5399) || defined(CC_MT5880) || defined (CC_MT5882)
              vIO32WriteFldAlign(CKGEN_OCLK_TEST, 1, FLD_OCLK_DIV_SEL);
#else
//              vIO32WriteFldAlign(CKGEN_VPCLK_CFG, 1, FLD_OCLK_DIV_SEL);
#endif
            }
            else // FOUR_PORT 
            {
//              if(IS_LVDS_HIGHSPEED)
//              {
//			   vIO32WriteFldAlign(CKGEN_OCLK_TEST, 2, FLD_OCLK_DIV_SEL);
//              }
//			  else
			  {
#if defined (CC_MT5399)
              vIO32WriteFldAlign(CKGEN_OCLK_TEST, 2, FLD_OCLK_DIV_SEL);
#endif
			  }
            }
			break;
		case VOPLL_CLK_SRC_XTAL:

			break;
		default:	
			break;
	}

}
 
#ifdef LVDS_ESD_PROTECT
#ifdef CC_MT5399
void u1LVDSClockSkew_Reset(void)
{
	vIO32WriteFldAlign(REG_LVDS_TX_CFG2, 0, RG_LVDSB_ENABIST_E_ODD_D11);  // 0000000
	vIO32WriteFldAlign(REG_LVDS_TX_CFG2, 0, RG_LVDSB_ENABIST_E_EVEN_D11);  // 0000000
	
	vIO32WriteFldAlign(REG_LVDS_TX_CFG2, 0x7E, RG_LVDSB_ENABIST_E_ODD_D11);  // 1111110
	vIO32WriteFldAlign(REG_LVDS_TX_CFG2, 0x7F, RG_LVDSB_ENABIST_E_EVEN_D11);  // 1111111
}

void u1LVDSClockSkew_Recover(void)
{
	vIO32WriteFldAlign(REG_LVDS_TX_CFG0, 0, RG_LVDSB_EN_D11);	
	HAL_Delay_us(LVDSA_EN_REST_DELAYTIME);
	vIO32WriteFldAlign(REG_LVDS_TX_CFG0, 1, RG_LVDSB_EN_D11);	
	
	#ifdef CC_MTK_LOADER	
    vDrvLVDS7To4FifoEnable();   
	#else
	_Lvds7To4FifoReset = 2;	
	#endif
	
	#ifndef NDEBUG
    Printf(" ======= [ESD] vDrvLVDSOn toggele RG_LVDSB_EN_D11 ===========\n");
	#endif

	u1LVDSClockSkew_Reset();
}

void u1CheckLVDSClockSkew_Enable(UINT8 u1OnOff)
{
	if (u1OnOff)
	{
	vIO32WriteFldAlign(REG_LVDS_TX_CFG0, 0, RG_LVDSB_ABIST_ISO_D11);
	vIO32WriteFldAlign(REG_LVDS_TX_CFG10, 1, RG_DIV_EN);
	vIO32WriteFldAlign(RGS_LVDS_TX_CFG0, 1, RG_LVDSB_EN_D11);
	vIO32WriteFldAlign(REG_LVDS_TX_CFG2, 0x7E, RG_LVDSB_ENABIST_E_ODD_D11);  // 1111110
	vIO32WriteFldAlign(REG_LVDS_TX_CFG2, 0x7F, RG_LVDSB_ENABIST_E_EVEN_D11);  // 1111111

	u1LVDSClockSkew_Reset();
	}
	else
	{

	}
}

UINT8 u1ChkLVDSClockSkewStatus(void)
{
	UINT8 u1ChA = 0,u1ChB = 0,u1ChABCD= 0;
	u1ChA = IO32ReadFldAlign(RGS_LVDS_TX_CFG0, RGS_LVDSB_ABIST_ODD_51EDG_D11);
	u1ChB = IO32ReadFldAlign(RGS_LVDS_TX_CFG0, RGS_LVDSB_ABIST_EVEN_51EDG_D11);
	
	u1ChABCD = u1ChA | u1ChB ;
	return (u1ChABCD);
}
#endif

#ifdef CC_MT5882
void vLVDSFIFOSkew_Recover(void)
{
	if(!IS_VBYONE)
	{
		#ifdef CC_MTK_LOADER	
	    vDrvLVDS7To4FifoEnable();   
		#else
		_Lvds7To4FifoReset = 2;	
		#endif
		
		#ifndef NDEBUG
	    Printf(" ======= [ESD] vDrvLVDSOn toggele RG_LVDSB_EN_D11 ===========\n");
		#endif
	}
}

UINT32 u1ChkLVDSFIFOSkewStatus(void)
{
	UINT16 u1WrPtr = 0;
	UINT16 u1RdPtr = 0;
	UINT32 u2FIFOSkw=0;
	u1WrPtr = IO32ReadFldAlign(LVDSB_REG22, LVDSTX_FIFO_WRPTR_ESD);
	u1RdPtr = IO32ReadFldAlign(LVDSB_REG22, LVDSTX_FIFO_RDPTR_ESD);
	
	u2FIFOSkw = (u1WrPtr<<8) | u1RdPtr ;
	return (u2FIFOSkw);
}

BOOL fgIsLVDSFIFOSkew(void)
{
	static UINT8 u1FifoSkewCnt = 0;
	UINT32 u2Skew = 0;
	BOOL fgFifoSkwRecover = FALSE;

	u2Skew = u1ChkLVDSFIFOSkewStatus();
	LOG(6, "[LVDS] FIFO skew = 0x%x\n",u2Skew);
	if(u2LVDSFifoSkwPre != u2Skew)
	{
		LOG(6, "[LVDS] FIFO pre skew: 0x%x\n", u2LVDSFifoSkwPre);
		u1FifoSkewCnt ++;
		LOG(6, "[LVDS] FIFO CNT: %d\n", u1FifoSkewCnt);
		if (u1FifoSkewCnt > 2)
		{
			u1FifoSkewCnt = 0;
			fgFifoSkwRecover = TRUE;
		}
	}
	else
	{
		u1FifoSkewCnt = 0;
		fgFifoSkwRecover = FALSE;
	}
	u2LVDSFifoSkwPre = u2Skew;
	LOG(6, "[LVDS] FIFO Recover Flag: %d\n", fgFifoSkwRecover);
	
	return (fgFifoSkwRecover);	
}
#endif
#endif
