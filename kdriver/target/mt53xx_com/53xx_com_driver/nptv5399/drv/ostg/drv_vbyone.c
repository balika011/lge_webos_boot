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
 * $RCSfile: drv_vbyone.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

#include "general.h"
#include "hw_vbyone.h"
#include "x_typedef.h"
#include "drv_lvds.h"
#include "hw_mlvds.h"
#include "hw_lvds.h"
#include "hw_tg.h"
#include "drv_vbyone.h"
#include "drv_vdoclk.h"
#include "x_pinmux.h"

#if !defined(CC_MT5399) && !defined(CC_MT5882)
  #define CC_VB1_DISABLE
#endif

typedef struct RVB1AnaRegTbl
{
	UINT32 wReg;
	UINT32 wValue;
} RVB1AnaRegTbl;

RVB1AnaRegTbl const VB1_ana_default[] = // Analog Setting A Group default
{
{REG_LVDS_TX_CFG11, 0x0448c000},
{REG_LVDS_TX_CFG12, 0x0c480fff},
{REG_LVDS_TX_CFG13, 0x0fff0000},
{REG_LVDS_TX_CFG14, 0x00000000},
{REG_LVDS_TX_CFG15, 0x00000000},
{REG_LVDS_TX_CFG16, 0x00000000},
{REG_LVDS_TX_CFG17, 0x00000000},
{REG_LVDS_TX_CFG18, 0x00000000},
{REG_LVDS_TX_CFG19, 0x000f0000},
{REG_LVDS_TX_CFG20, 0x00000000},
{0xFFFFFFFF, 0x00000000}
};

void _svDrvVB1LoadRegTbl(RVB1AnaRegTbl const *pRegTbl)
{
	if(pRegTbl == NULL)
	{
		return;
	}

	while(pRegTbl->wReg != 0xFFFFFFFF)
	{
		vIO32Write4B(pRegTbl->wReg, pRegTbl->wValue);
		pRegTbl++;
	}
}

/**
 * @brief vVByOneAnalogSetting
 * VB1 analog setting  
 * @param  void
 * @retval void
 */
void vVByOneAnalogSetting(void)
{
#ifndef CC_VB1_DISABLE
    UINT32 u4Value;

	//** Analog Setting A Group**
	//load default setting
	_svDrvVB1LoadRegTbl(VB1_ana_default);

	vIO32WriteFldAlign(REG_LVDS_TX_CFG19, 0x0, RG_LVDSA_VSW_CALIB_SEL_D11); 
	vIO32WriteFldAlign(REG_LVDS_TX_CFG17, 0x1c, RG_LVDSA_ABIST_VL_ODD_SEL_D11); 
	
	u4Value = PANEL_GetDrivingCurrent();
    if (u4Value <= 15)
    {
      vDrvLVDSSetDriving(u4Value);
    }
    else
    {
      //vDrvLVDSSetDriving(6);
	  vIO32WriteFldAlign(REG_LVDS_TX_CFG11, 0xd, RG_LVDSA_TVO); 
    }
	
	vIO32WriteFldAlign(REG_LVDS_TX_CFG11, 0x4, RG_LVDSA_TVCM);
	vIO32WriteFldAlign(REG_LVDS_TX_CFG12, 0x4, RG_LVDSA_BIAS_SEL); 
	vIO32WriteFldAlign(REG_LVDS_TX_CFG11, 0x6, RG_LVDSA_NSRC);
	vIO32WriteFldAlign(REG_LVDS_TX_CFG11, 0x6, RG_LVDSA_PSRC);
	vIO32WriteFldAlign(REG_LVDS_TX_CFG11, 0x170, RG_LVDSA_EXT_EN_D11);
	vIO32WriteFldAlign(REG_LVDS_TX_CFG14, 0x0, RG_LVDSA_LVDS_SEL);
	vIO32WriteFldAlign(REG_LVDS_TX_CFG13, 0x0, RG_LVDSA_CLK_IQ_SEL);
	vIO32WriteFldAlign(REG_LVDS_TX_CFG19, 0x0, RG_LVDSA_ENEDGE_SEL_D11);
	vIO32WriteFldAlign(REG_LVDS_TX_CFG19, 0x0, RG_LVDSA_RESYNC_CLK_IQ_SEL_D11);
	vIO32WriteFldAlign(REG_LVDS_TX_CFG19, 0x0, RG_LVDSA_RESYNC_LVDS_SEL_D11);
	vIO32WriteFldAlign(REG_LVDS_TX_CFG11, 0x0, RG_LVDSA_BIAS_ODD_PWD);
	vIO32WriteFldAlign(REG_LVDS_TX_CFG11, 0x0, RG_LVDSA_BIAS_EVEN_PWD);
	vIO32WriteFldAlign(REG_LVDS_TX_CFG19, 0x0, RG_LVDSA_LDO_BIAS_ODD_PWD_D11);
	vIO32WriteFldAlign(REG_LVDS_TX_CFG19, 0x0, RG_LVDSA_LDO_BIAS_EVEN_PWD_D11);
	vIO32WriteFldAlign(REG_LVDS_TX_CFG19, 0x0, RG_LVDSA_LDO_ODD_PWD_D11);
	vIO32WriteFldAlign(REG_LVDS_TX_CFG19, 0x0, RG_LVDSA_LDO_EVEN_PWD_D11);
	#ifdef CC_MT5882
	vIO32WriteFldAlign(REG_LVDS_TX_CFG12,0x3E8F,RG_LVDSA_DRV_PWD);
	vIO32WriteFldAlign(REG_LVDS_TX_CFG13,0x3FFF,RG_LVDSA_PRE_PWD);
	#else
	vIO32WriteFldAlign(REG_LVDS_TX_CFG12, 0xE8F, RG_LVDSA_DRV_PWD);
	vIO32WriteFldAlign(REG_LVDS_TX_CFG13, 0xe8f, RG_LVDSA_PRE_PWD);	
	#endif
	vIO32WriteFldAlign(REG_LVDS_TX_CFG12, 0x1, RG_LVDSA_TVO_PRE);
	vIO32WriteFldAlign(REG_LVDS_TX_CFG12, 0x4, RG_LVDSA_NSRC_PRE);
	vIO32WriteFldAlign(REG_LVDS_TX_CFG12, 0x4, RG_LVDSA_PSRC_PRE);
    vIO32WriteFldMulti(REG_LVDS_TX_CFG14, P_Fld(1, RG_LVDSA_HSN_MODE_EVEN_EN_D11)|P_Fld(1, RG_LVDSA_HSP_MODE_EVEN_EN_D11)
                                         |P_Fld(1, RG_LVDSA_HSN_MODE_ODD_EN_D11)|P_Fld(1, RG_LVDSA_HSP_MODE_ODD_EN_D11));
	vIO32WriteFldAlign(REG_LVDS_TX_CFG11, 0x0, RG_LVDSA_EN);
	HAL_Delay_us(LVDSA_EN_REST_DELAYTIME);
	vIO32WriteFldAlign(REG_LVDS_TX_CFG11, 0x1, RG_LVDSA_EN);	
#endif
}

/**
 * @brief vVByOneAnalogOffSetting
 * VB1 analog setting  
 * @param  void
 * @retval void
 */
void vVByOneAnalogOffSetting(void)
{
#ifndef CC_VB1_DISABLE
	vIO32WriteFldAlign(REG_LVDS_TX_CFG11, 0, RG_LVDSA_EN);
	vIO32WriteFldAlign(REG_LVDS_TX_CFG11, 0, RG_LVDSA_EXT_EN_D11);
    vIO32WriteFldMulti(REG_LVDS_TX_CFG14, P_Fld(0, RG_LVDSA_HSN_MODE_EVEN_EN_D11)|P_Fld(0, RG_LVDSA_HSP_MODE_EVEN_EN_D11)
                                         |P_Fld(0, RG_LVDSA_HSN_MODE_ODD_EN_D11)|P_Fld(0, RG_LVDSA_HSP_MODE_ODD_EN_D11));
	#ifdef CC_MT5882	
	vIO32WriteFldAlign(REG_LVDS_TX_CFG13, 0x3FFF, RG_LVDSA_PRE_PWD);
	vIO32WriteFldAlign(REG_LVDS_TX_CFG12, 0x3FFF, RG_LVDSA_DRV_PWD);
	#else
	vIO32WriteFldAlign(REG_LVDS_TX_CFG13, 0xFFF, RG_LVDSA_PRE_PWD);
	vIO32WriteFldAlign(REG_LVDS_TX_CFG12, 0xFFF, RG_LVDSA_DRV_PWD);	
	#endif
	vIO32WriteFldAlign(REG_LVDS_TX_CFG19, 1, RG_LVDSA_LDO_EVEN_PWD_D11);
	vIO32WriteFldAlign(REG_LVDS_TX_CFG19, 1, RG_LVDSA_LDO_ODD_PWD_D11);
	vIO32WriteFldAlign(REG_LVDS_TX_CFG19, 1, RG_LVDSA_LDO_BIAS_EVEN_PWD_D11);
	vIO32WriteFldAlign(REG_LVDS_TX_CFG19, 1, RG_LVDSA_LDO_BIAS_ODD_PWD_D11);
	vIO32WriteFldAlign(REG_LVDS_TX_CFG11, 1, RG_LVDSA_BIAS_EVEN_PWD);
	vIO32WriteFldAlign(REG_LVDS_TX_CFG11, 1, RG_LVDSA_BIAS_ODD_PWD);	
#endif
}

void vVByOne_Impedance_Cal(void)
{
#ifndef CC_VB1_DISABLE
	UINT8 i = 0;
	UINT32 u4tmp = 0, u4TVO_Backup = 0;

	u4TVO_Backup = IO32ReadFldAlign(REG_LVDS_TX_CFG11, RG_LVDSA_TVO);

	vIO32WriteFldAlign(VB1SYS6, 0, VB1SYS6_START);
	vIO32WriteFldAlign(TMGR16,0xfffff,TMGR16_RG_TMDS_CTRL_REG_EN);
	vIO32WriteFldAlign(TMGR17,0x3ff,TMGR17_RG_TMDS_REG_VALUE);
	vIO32WriteFldAlign(TMGR5,0x7,TMGR5_RG_FIFO_CTRL);
	vIO32WriteFldAlign(REG_LVDS_TX_CFG19,0,RG_LVDSA_VSW_CALIB_SEL_D11);
	vIO32WriteFldAlign(REG_LVDS_TX_CFG17,0x1c,RG_LVDSA_ABIST_VL_ODD_SEL_D11);
	vDrvLVDSSetDriving(6); // default 6mA 	vIO32WriteFldAlign(REG_LVDS_TX_CFG11, 0xd,RG_LVDSA_TVO);
	vIO32WriteFldAlign(REG_LVDS_TX_CFG11,0x4,RG_LVDSA_TVCM);
	vIO32WriteFldAlign(REG_LVDS_TX_CFG12,0x4,RG_LVDSA_BIAS_SEL);
	vIO32WriteFldAlign(REG_LVDS_TX_CFG11,0x6,RG_LVDSA_NSRC);
	vIO32WriteFldAlign(REG_LVDS_TX_CFG11,0x6,RG_LVDSA_PSRC);
	vIO32WriteFldAlign(REG_LVDS_TX_CFG11,0x170,RG_LVDSA_EXT_EN_D11);
	vIO32WriteFldAlign(REG_LVDS_TX_CFG14,0,RG_LVDSA_LVDS_SEL);
	vIO32WriteFldAlign(REG_LVDS_TX_CFG13,0,RG_LVDSA_CLK_IQ_SEL);
	vIO32WriteFldAlign(REG_LVDS_TX_CFG19,0,RG_LVDSA_ENEDGE_SEL_D11);
	vIO32WriteFldAlign(REG_LVDS_TX_CFG19,0,RG_LVDSA_RESYNC_CLK_IQ_SEL_D11);
	vIO32WriteFldAlign(REG_LVDS_TX_CFG19,0,RG_LVDSA_RESYNC_LVDS_SEL_D11);
	vIO32WriteFldAlign(REG_LVDS_TX_CFG11,0,RG_LVDSA_BIAS_ODD_PWD);
	vIO32WriteFldAlign(REG_LVDS_TX_CFG11,0,RG_LVDSA_BIAS_EVEN_PWD);
	vIO32WriteFldAlign(REG_LVDS_TX_CFG19,0,RG_LVDSA_LDO_BIAS_ODD_PWD_D11);
	vIO32WriteFldAlign(REG_LVDS_TX_CFG19,0,RG_LVDSA_LDO_BIAS_EVEN_PWD_D11);
	vIO32WriteFldAlign(REG_LVDS_TX_CFG19,0,RG_LVDSA_LDO_ODD_PWD_D11);
	vIO32WriteFldAlign(REG_LVDS_TX_CFG19,0,RG_LVDSA_LDO_EVEN_PWD_D11);
	#ifdef CC_MT5882
	vIO32WriteFldAlign(REG_LVDS_TX_CFG12,0x3E8F,RG_LVDSA_DRV_PWD);
	vIO32WriteFldAlign(REG_LVDS_TX_CFG13,0x3FFF,RG_LVDSA_PRE_PWD);
	#else
	vIO32WriteFldAlign(REG_LVDS_TX_CFG12,0xE8F,RG_LVDSA_DRV_PWD);
	vIO32WriteFldAlign(REG_LVDS_TX_CFG13,0xfff,RG_LVDSA_PRE_PWD);
	#endif
	vIO32WriteFldAlign(REG_LVDS_TX_CFG12,0x1,RG_LVDSA_TVO_PRE);
	vIO32WriteFldAlign(REG_LVDS_TX_CFG12,0x4,RG_LVDSA_NSRC_PRE);
	vIO32WriteFldAlign(REG_LVDS_TX_CFG12,0x4,RG_LVDSA_PSRC_PRE);
	vIO32WriteFldMulti(REG_LVDS_TX_CFG14, P_Fld(1, RG_LVDSA_HSN_MODE_EVEN_EN_D11)|P_Fld(1, RG_LVDSA_HSP_MODE_EVEN_EN_D11)
	                                 |P_Fld(1, RG_LVDSA_HSN_MODE_ODD_EN_D11)|P_Fld(1, RG_LVDSA_HSP_MODE_ODD_EN_D11));
	vIO32WriteFldAlign(REG_LVDS_TX_CFG11,0x1,RG_LVDSA_EN);
	vIO32WriteFldAlign(REG_LVDS_TX_CFG19,0x1,RG_LVDSA_IMP_COMP_EN_D11);

	vUtDelay2us(100);
	vIO32WriteFldMulti(TMGR7, P_Fld(0, TMGR7_RG_LOOPPAT_2TO1)|P_Fld(0, TMGR7_RG_LOOPPAT_EN)|P_Fld(1, TMGR7_IMP_CAL_SET)
							|P_Fld(0, TMGR7_RG_RXIMP)|P_Fld(0, TMGR7_RG_AUTOK_RX_IMP)|P_Fld(0, TMGR7_RG_TXIMP)
                            |P_Fld(0, TMGR7_REG_IMP_DP_DIS)|P_Fld(0, TMGR7_RG_IMPFREQSEL)|P_Fld(1, TMGR7_RG_AUTOK_TX_IMP)
                            |P_Fld(1, TMGR7_REG_SEL_1_ALL)|P_Fld(0, TMGR7_SAPIS_MODE_SYNC)|P_Fld(0, TMGR7_CTL_EN_CALIBRATION));
	
	vUtDelay2us(100);
	vIO32WriteFldMulti(TMGR7, P_Fld(0, TMGR7_RG_LOOPPAT_2TO1)|P_Fld(0, TMGR7_RG_LOOPPAT_EN)|P_Fld(1, TMGR7_IMP_CAL_SET)
							|P_Fld(0, TMGR7_RG_RXIMP)|P_Fld(0, TMGR7_RG_AUTOK_RX_IMP)|P_Fld(0, TMGR7_RG_TXIMP)
                            |P_Fld(0, TMGR7_REG_IMP_DP_DIS)|P_Fld(0, TMGR7_RG_IMPFREQSEL)|P_Fld(1, TMGR7_RG_AUTOK_TX_IMP)
                            |P_Fld(1, TMGR7_REG_SEL_1_ALL)|P_Fld(1, TMGR7_SAPIS_MODE_SYNC)|P_Fld(1, TMGR7_CTL_EN_CALIBRATION));

	do
	{
		u4tmp = IO32ReadFldAlign(TMGR19,TMGR19_HOLD);
		vUtDelay2us(10);
		i++;
		if(i>200)
		{
				printf("VB1 impedance cal error then break......\n ");
				break;
		}
		
	}while(!u4tmp);
	
	u4tmp = IO32ReadFldAlign(TMGR19,TMGR19_DA_IMPCAL_TX_LAT);
	if(u4tmp>0 && u4tmp<0xf)
	{
			printf("VB1 VB1 impedance cal  pass and value =%x .................\n",u4tmp);
	}
	else
	{
			printf("VB1 VB1 impedance cal  failed .........................\n");
	}
	
	vIO32WriteFldAlign(REG_LVDS_TX_CFG19, 0x0, RG_LVDSA_IMP_COMP_EN_D11);

	vIO32WriteFldAlign(REG_LVDS_TX_CFG11, u4TVO_Backup, RG_LVDSA_TVO);


#endif
}

#ifdef CONFIG_OPM
    BOOL fgVB1Init = FALSE;
#else
    static BOOL fgVB1Init = FALSE;
#endif

void vVByOneInit(void)
{
#ifndef CC_VB1_DISABLE
	UINT8 i = 0;
	UINT32 u4tmp = 0;	
    UINT32 *pu4PinMap,*pu4PNSwap;

    if (IO32ReadFldAlign(DDDS_08, FLAG_VB1_INITIAL) == 1)
    {
        fgVB1Init = TRUE;
    }

	if (!fgVB1Init)
	{
	
	do
	{
		u4tmp = u4IO32Read4B(RGS_VOPLL_CFG0);
		vUtDelay2us(10);
		i++;
		if(i>200)
		{
			printf("VB1 Vpll clock check error......\n ");
			break;
		}
		
	}while((u4tmp &0xC0000000)!=0x80000000); //bit 31 30


    // switch to LVDS TX mode               
	vIO32WriteFldAlign(FIFO_CTRL_01, 3, TCLK_FIFO_ACTIVE_PORT); 
	vIO32WriteFldAlign(TMGR2, 2, TMGR2_RG_FIFO_CK_SEL); 
	vIO32WriteFldAlign(FIFO_CTRL_01, 1, TCLK_FIFO_START); 


	vIO32WriteFldMulti(VB1SYS0, P_Fld(0, VB1SYS0_BIT_INV)|
								P_Fld(0, VB1SYS0_VSYN_INV)|
								P_Fld(0, VB1SYS0_HSYN_INV)|
								P_Fld(1, VB1SYS0_LANE_3_EN)|
								P_Fld(1, VB1SYS0_LANE_2_EN)|
								P_Fld(1, VB1SYS0_LANE_1_EN)|
								P_Fld(1, VB1SYS0_LANE_0_EN)|
								P_Fld(0, VB1SYS0_FIELDBET)|
								P_Fld(0, VB1SYS0_PATGEN_START)|
								P_Fld(1, VB1SYS0_DE_RST_CTRL)|
								P_Fld(VB1SYS0_LANE_NUM_4, VB1SYS0_LANE_NUM)|
								P_Fld(VB1SYS0_COL_4BYTES, VB1SYS0_COL));

	#ifdef CC_SCPOS_3DTV_SUPPORT
	vIO32WriteFldAlign(VB1SYS0, 1, VB1SYS0_3DLR_EN); 
	#endif


	vIO32WriteFldMulti(VB1SYS3, P_Fld(0, VB1SYS3_CRC_EN)|
								P_Fld(0, VB1SYS3_LANE_PRBS_EN)|
								P_Fld(1, VB1SYS3_LANE3_ANA)|
								P_Fld(1, VB1SYS3_LANE2_ANA)|
								P_Fld(1, VB1SYS3_LANE1_ANA)|
								P_Fld(1, VB1SYS3_LANE0_ANA));
	if(NULL == (pu4PNSwap = (UINT32*)DRVCUST_PanelGet(ePanelVB1LanePNSwap)))
	{
		vIO32WriteFldMulti(VB1SYS3, 
								P_Fld(0, VB1SYS3_LANE3_INV)|
								P_Fld(0, VB1SYS3_LANE2_INV)|
								P_Fld(0, VB1SYS3_LANE1_INV)|
								P_Fld(0, VB1SYS3_LANE0_INV));
	}
	else
	{
		vIO32WriteFldMulti(VB1SYS3, 
									P_Fld(pu4PNSwap[3], VB1SYS3_LANE3_INV)|
									P_Fld(pu4PNSwap[2], VB1SYS3_LANE2_INV)|
									P_Fld(pu4PNSwap[1], VB1SYS3_LANE1_INV)|
									P_Fld(pu4PNSwap[0], VB1SYS3_LANE0_INV));																	

	}
	// ==Lane Swap ==
	if(NULL == (pu4PinMap = (UINT32*)DRVCUST_PanelGet(ePanelVB1LaneSwap)))
	{
	vIO32WriteFldMulti(VB1SYS1, 
								P_Fld(3, VB1SYS1_LANE3_MUX)|
								P_Fld(2, VB1SYS1_LANE2_MUX)|
								P_Fld(1, VB1SYS1_LANE1_MUX)|
								P_Fld(0, VB1SYS1_LANE0_MUX));	
	}
	else
	{
	vIO32WriteFldMulti(VB1SYS1, 
								P_Fld(pu4PinMap[3], VB1SYS1_LANE3_MUX)|
								P_Fld(pu4PinMap[2], VB1SYS1_LANE2_MUX)|
								P_Fld(pu4PinMap[1], VB1SYS1_LANE1_MUX)|
								P_Fld(pu4PinMap[0], VB1SYS1_LANE0_MUX));	
	}

	vIO32WriteFldAlign(VB1SYS1, 0, VB1SYS1_MON_SEL);	  	
	vIO32WriteFldMulti(VB1SYS3, P_Fld(0, VB1SYS3_ANA_RX_EN)|
								P_Fld(0x3FF, VB1SYS3_LANE3_ANA_VALUE));         

    vIO32WriteFldAlign(TMGR5, 0, TMGR5_RG_FIFO_CTRL);
    vIO32WriteFldAlign(TMGR5, 1, TMGR5_RG_FIFO_EN);

	vVByOne_Impedance_Cal();

	vIO32WriteFldMulti(VB1SYS3, P_Fld(0, VB1SYS3_LANE3_ANA)|
								P_Fld(0, VB1SYS3_LANE2_ANA)|
								P_Fld(0, VB1SYS3_LANE1_ANA)|
								P_Fld(0, VB1SYS3_LANE0_ANA));

	vIO32WriteFldAlign(REG_LVDS_TX_CFG19, 0x1, RG_LVDSA_LDO_ODD_PWD_D11);
	vIO32WriteFldAlign(REG_LVDS_TX_CFG19, 0x1, RG_LVDSA_LDO_EVEN_PWD_D11);
	vIO32WriteFldAlign(REG_LVDS_TX_CFG19, 0x1, RG_LVDSA_LDO_BIAS_ODD_PWD_D11);
	vIO32WriteFldAlign(REG_LVDS_TX_CFG19, 0x1, RG_LVDSA_LDO_BIAS_EVEN_PWD_D11);
	vIO32WriteFldAlign(REG_LVDS_TX_CFG11, 0x1, RG_LVDSA_BIAS_ODD_PWD);
	vIO32WriteFldAlign(REG_LVDS_TX_CFG11, 0x1, RG_LVDSA_BIAS_EVEN_PWD);

	vIO32WriteFldAlign(DDDS_08, 1, FLAG_VB1_INITIAL);
	}
#endif
}

void vVByOneFormatConfig(void)
{
#ifndef CC_VB1_DISABLE
    UINT32 u4MSBSW;
    
    u4MSBSW = LVDS_DISP_FORMAT;

	if (u4MSBSW == LVDS_MSB_SW_ON) // NS, VESA
	{
		vIO32WriteFldAlign(VB1SYS1, 1, VB1SYS1_TTL_REORDER);	  
	}
	else if (u4MSBSW == LVDS_MSB_SW_OFF) // JEIDA
	{
		vIO32WriteFldAlign(VB1SYS1, 0, VB1SYS1_TTL_REORDER);	  		
	}
#endif
}

void vDrvVB14K2KPanelInterfaceConfig(void)
{
    if (IS_DISABLE_LVDS_LINEBUFFER)
    {
    	vIO32WriteFldAlign(VINTF_B0, 1,  VINTF_LINE_BUFFER_BYPASS); 
    }	
	else
	{
    	vIO32WriteFldAlign(VINTF_B0, 0,  VINTF_LINE_BUFFER_BYPASS); 
	}
}


void vDrvVB1Config(void)
{   
#ifndef CC_VB1_DISABLE
    // set VB1 format
    vVByOneFormatConfig();

	// line buffer bypass
	vDrvVB14K2KPanelInterfaceConfig();

    if (HSYNC_POLARITY == HSYNC_LOW)
    {  
        vIO32WriteFldAlign(VB1SYS0, 0, VB1SYS0_HSYN_INV);                
    }
    else
    {        
        vIO32WriteFldAlign(VB1SYS0, 1, VB1SYS0_HSYN_INV);                
    }

    if (VSYNC_POLARITY == VSYNC_LOW)
    {       
        vIO32WriteFldAlign(VB1SYS0, 0, VB1SYS0_VSYN_INV);                
    }
    else
    {
        vIO32WriteFldAlign(VB1SYS0, 1, VB1SYS0_VSYN_INV);                     
    }

#endif   
}

/**
 * @brief vVByOneDigitalReset
 * disable VB1 digital module
 * @param  void
 * @retval void
 */
void vVByOneDigitalReset(void)
{
#ifndef CC_VB1_DISABLE
    vIO32WriteFldAlign(TMGR5, 0, TMGR5_RG_FIFO_CTRL);
    vIO32WriteFldAlign(TMGR5, 0, TMGR5_RG_FIFO_EN);
    vIO32WriteFldAlign(VB1SYS6, 0, VB1SYS6_START);    
#endif
}
/**
 * @brief vDrvVB1On
 * Power On VBYONE
 * @param  void
 * @retval void
 */
void vDrvVByOneOn(void)
{
#ifndef CC_VB1_DISABLE
    UINT32 u4DispBit;
    UINT32 u4VB1Lane;
    
    u4DispBit = DISP_BIT;
    u4VB1Lane = LVDS_OUTPUT_PORT;

#if defined(CC_MT5399)
    BSP_PinSet( PIN_GPIO42, PINMUX_FUNCTION2);  // LOCKN
    BSP_PinSet( PIN_TCON11, PINMUX_FUNCTION3);  // HDPN
#elif defined(CC_MT5882)
	BSP_PinSet( PIN_TCON12, PINMUX_FUNCTION2);  // LOCKN
    BSP_PinSet( PIN_TCON11, PINMUX_FUNCTION2);  // HDPN
#endif
    
	switch (u4DispBit)
	{
	case DISP_36BIT:	   
		vIO32WriteFldAlign(VB1SYS0,VB1SYS0_COL_5BYTES,VB1SYS0_COL);
	   break;
   	 case DISP_30BIT:   	 	
		vIO32WriteFldAlign(VB1SYS0,VB1SYS0_COL_4BYTES,VB1SYS0_COL);
    	break;		
	 case DISP_24BIT:
     case DISP_18BIT:
   	 	vIO32WriteFldAlign(VB1SYS0,VB1SYS0_COL_3BYTES,VB1SYS0_COL);
    	break;
	default:
    	break;
	}

	vDrvVB1Config();

	if(u4VB1Lane == FOUR_PORT)
	{
	   vIO32WriteFldAlign(CKGEN_OCLK_TEST, 2,FLD_VB1_CK_SEL);
	   vVByOneAnalogSetting();
	   vVByOneDigitalReset();			   
	   vIO32WriteFldAlign(VB1SYS0,VB1SYS0_LANE_NUM_4,VB1SYS0_LANE_NUM);	   
	   if(IS_LVDS_HIGHSPEED)//240Hz
	   {
		   vIO32WriteFldAlign(VB1SYS0,0xff,VB1SYS0_LANE_EN_MUX);
	   }
	   else
	   {
		   vIO32WriteFldAlign(VB1SYS0,0xf,VB1SYS0_LANE_EN_MUX);
	   }
	}
	else if (u4VB1Lane == DUAL_PORT)
	{
	   vIO32WriteFldAlign(CKGEN_OCLK_TEST, 1,FLD_VB1_CK_SEL);
	   vVByOneAnalogSetting();
	   vVByOneDigitalReset();
	   vIO32WriteFldAlign(VB1SYS0,VB1SYS0_LANE_NUM_2,VB1SYS0_LANE_NUM);
	   #ifdef VB1_ENABLE_4LANE_CLK_TRAINING
	   vIO32WriteFldAlign(VB1SYS0,0xF,VB1SYS0_LANE_EN_MUX);
	   #else
	   vIO32WriteFldAlign(VB1SYS0,0x3,VB1SYS0_LANE_EN_MUX);
	   #endif
			   
	}
	else if (u4VB1Lane == SINGLE_PORT)
	{
	   vIO32WriteFldAlign(CKGEN_OCLK_TEST, 0,FLD_VB1_CK_SEL);
	   vVByOneAnalogSetting();
	   vVByOneDigitalReset();
	   vIO32WriteFldAlign(VB1SYS0,VB1SYS0_LANE_NUM_1,VB1SYS0_LANE_NUM);
	   #ifdef VB1_EN_4LANE_CLK_TRAINING
	   vIO32WriteFldAlign(VB1SYS0,0xF,VB1SYS0_LANE_EN_MUX);
	   #else
	   vIO32WriteFldAlign(VB1SYS0,0x3,VB1SYS0_LANE_EN_MUX);
	   #endif
	}
		   
	vIO32WriteFldAlign(VB1SYS0,0x1,VB1SYS0_DE_RST_CTRL);
	vIO32WriteFldAlign(VB1SYS1,0x0,VB1SYS0_DE_RST_CTRL);
	
    vIO32WriteFldAlign(TMGR5, 0, TMGR5_RG_FIFO_CTRL);
    vIO32WriteFldAlign(TMGR5, 1, TMGR5_RG_FIFO_EN);
    vIO32WriteFldAlign(VB1SYS6, 1, VB1SYS6_START);	
#endif
}

/**
 * @brief vDrvVB1Off
 * Power off VBYONE digital
 * @param  void
 * @retval void
 */
void vDrvVByOneOff(void)
{
#ifndef CC_VB1_DISABLE
     vIO32WriteFldAlign(TMGR5, 0, TMGR5_RG_FIFO_CTRL);
     vIO32WriteFldAlign(TMGR5, 0, TMGR5_RG_FIFO_EN);
     vIO32WriteFldAlign(VB1SYS6, 0, VB1SYS6_START);
#endif
}

/**
 * @brief vDrvVByOneCRCCheck
 * crc check
 * @param  void
 * @retval void
 */
void vDrvVByOneCRCCheck(void)
{
#ifndef CC_VB1_DISABLE
	UINT32 u4VB1Lane;

	u4VB1Lane = LVDS_OUTPUT_PORT;

	vIO32WriteFldAlign(VB1RES,0x1,VB1RES_REG_CONTRL);
	vIO32WriteFldAlign(VB1SYS3,0x0,VB1SYS3_CRC_EN);
	vIO32WriteFldAlign(VB1SYS3,0x1,VB1SYS3_CRC_EN);
	if(u4VB1Lane == FOUR_PORT)
	{
		if(IO32ReadFldAlign(VB1SYS8, VB1SYS8_CRC1_FAIL) ||IO32ReadFldAlign(VB1SYS8, VB1SYS8_CRC2_FAIL) ||IO32ReadFldAlign(VB1SYS8, VB1SYS8_CRC3_FAIL)||IO32ReadFldAlign(VB1SYS8, VB1SYS8_CRC4_FAIL))
		{
			printf("4 lane CRC fail and Fali value =%x\n",IO32ReadFldAlign(VB1SYS8, VB1SYS8_CRC1_FAIL) |IO32ReadFldAlign(VB1SYS8, VB1SYS8_CRC2_FAIL) |IO32ReadFldAlign(VB1SYS8, VB1SYS8_CRC3_FAIL)|IO32ReadFldAlign(VB1SYS8, VB1SYS8_CRC4_FAIL));
		}
		else
		{
			printf("4 lane CRC pass\n");
		}
	}
	else if (u4VB1Lane == DUAL_PORT)
	{   
		if(IO32ReadFldAlign(VB1SYS8, VB1SYS8_CRC1_FAIL) ||IO32ReadFldAlign(VB1SYS8, VB1SYS8_CRC2_FAIL) )
		{
			printf("2 lane CRC fail and Fali value =%x\n",IO32ReadFldAlign(VB1SYS8, VB1SYS8_CRC1_FAIL) |IO32ReadFldAlign(VB1SYS8, VB1SYS8_CRC2_FAIL) );
		}	
		else
		{
			printf("2 lane CRC pass\n");
		}
	}
	else if (u4VB1Lane == SINGLE_PORT)
	{
		if(IO32ReadFldAlign(VB1SYS8, VB1SYS8_CRC1_FAIL))
		{
	    	printf("1 lane CRC fail and Fali value =%x\n",IO32ReadFldAlign(VB1SYS8, VB1SYS8_CRC1_FAIL));
		}	
			else
		{
	    	printf("1 lane CRC pass\n");
		}
	}
#endif
}

/**
 * @brief vGetVByOneStatus
 * Get VBYONE status
 * @param  void
 * @retval void
 */
void vGetVByOneStatus(void)
{
#ifndef CC_VB1_DISABLE	
	printf("[VB1] LOCKN = %d \n", IO32ReadFldAlign(VB1SYS8,VB1SYS8_LOCKN));
	printf("[VB1] HTDPN = %d \n", IO32ReadFldAlign(VB1SYS8,VB1SYS8_HTDPN));	
#endif	
}

#ifndef CC_VB1_DISABLE
UINT16 u2UnlockCnt = 0;
UINT16 u2UnHTDP = 0;
UINT8 u1VB1Chk = 0;
#endif	

void vSetVByOneUnLockChk(UINT8 u1En)
{
#ifndef CC_VB1_DISABLE	
	u1VB1Chk = u1En;
#endif
}

void vGetVByOneUnLock(void)
{
#ifndef CC_VB1_DISABLE	
	if (IO32ReadFldAlign(VB1SYS8,VB1SYS8_LOCKN)==1)
	{
	u2UnlockCnt++;
	printf("[VB1] UnLOCKN Count=%d \n", u2UnlockCnt);
	}
	if (IO32ReadFldAlign(VB1SYS8,VB1SYS8_HTDPN)==1)	
	{
	u2UnHTDP++;
	printf("[VB1] HTDPN_FAIL Count=%d \n", u2UnHTDP);	
	}
#endif	
}

void vGetVByOneUnLockCnt(void)
{
#ifndef CC_VB1_DISABLE	
	printf("[VB1] u2UnlockCnt=%d \n", u2UnlockCnt);
	printf("[VB1] u2UnHTDP=%d \n", u2UnHTDP);	
#endif	

}

void vDrvVB1LockCtrl(UINT8 bOnOff, UINT8 bValue)
{
#ifndef CC_VB1_DISABLE	

	vIO32WriteFldAlign(TMGR18,bOnOff,TMGR18_RG_LOCK_CTRL);
	vIO32WriteFldAlign(TMGR18,bValue,TMGR18_RG_LOCK_VALUE);
#endif	
}

static void VByOneToggleVOPLL(void)
{
#ifndef CC_VB1_DISABLE	
	// toggle VOPLL Power
	vIO32WriteFldAlign(REG_VOPLL_CFG0, 1, RG_VPLL_BG_LVDS_PWD);
	vIO32WriteFldAlign(REG_VOPLL_CFG0, 1, RG_VPLL_BG_VB1_TX_PWD);
	vIO32WriteFldAlign(REG_VOPLL_CFG0, 1, RG_VPLL_PWD);
	vIO32WriteFldAlign(REG_VOPLL_CFG3, 1, RG_PPLL_PWD); 	  
	HAL_Delay_us(400);
	vIO32WriteFldAlign(REG_VOPLL_CFG0, 0, RG_VPLL_PWD); 
	vIO32WriteFldAlign(REG_VOPLL_CFG3, 0, RG_PPLL_PWD);
	vIO32WriteFldAlign(REG_VOPLL_CFG0, 0, RG_VPLL_BG_LVDS_PWD);
	vIO32WriteFldAlign(REG_VOPLL_CFG0, 0, RG_VPLL_BG_VB1_TX_PWD);
	HAL_Delay_us(20);
#endif	
}

void vVByOneLane4_8bit_script(void)
{
#ifndef CC_VB1_DISABLE							   
	vIO32WriteFldMulti(REG_VOPLL_CFG0, P_Fld(0x1D, RG_VPLL_FBKDIV)|P_Fld(1, RG_VPLL_FBKSEL));
	VByOneToggleVOPLL();
	vIO32WriteFldAlign(VB1SYS0,VB1SYS0_COL_3BYTES,VB1SYS0_COL);
	vIO32WriteFldAlign(VB1SYS1,0,VB1SYS1_TTL_REORDER);
	vIO32WriteFldAlign(CKGEN_OCLK_TEST, 2,FLD_VB1_CK_SEL);
	vIO32WriteFldAlign(VB1SYS0,VB1SYS0_LANE_NUM_4,VB1SYS0_LANE_NUM);
	vIO32WriteFldAlign(VB1SYS0,0xf,VB1SYS0_LANE_EN_MUX);
#endif
}

void vVByOneLane4_10bit_script(void)
{
#ifndef CC_VB1_DISABLE
	vIO32WriteFldMulti(REG_VOPLL_CFG0, P_Fld(0x27, RG_VPLL_FBKDIV)|P_Fld(1, RG_VPLL_FBKSEL));
	VByOneToggleVOPLL();
	vIO32WriteFldAlign(VB1SYS0,VB1SYS0_COL_4BYTES,VB1SYS0_COL);
	vIO32WriteFldAlign(CKGEN_OCLK_TEST, 2,FLD_VB1_CK_SEL);
	vIO32WriteFldAlign(VB1SYS0,VB1SYS0_LANE_NUM_4,VB1SYS0_LANE_NUM);
	vIO32WriteFldAlign(VB1SYS0,0xf,VB1SYS0_LANE_EN_MUX);
#endif  
}

void vVByOneLane4_12bit_script(void)
{
#ifndef CC_VB1_DISABLE
	vIO32WriteFldMulti(REG_VOPLL_CFG0, P_Fld(0x18, RG_VPLL_FBKDIV)|P_Fld(3, RG_VPLL_FBKSEL));       
	VByOneToggleVOPLL();
	vIO32WriteFldAlign(VB1SYS0,VB1SYS0_COL_5BYTES,VB1SYS0_COL);
	vIO32WriteFldAlign(VB1SYS1,0,VB1SYS1_TTL_REORDER);
	vIO32WriteFldAlign(CKGEN_OCLK_TEST, 2,FLD_VB1_CK_SEL);
	vIO32WriteFldAlign(VB1SYS0,VB1SYS0_LANE_NUM_4,VB1SYS0_LANE_NUM);
	vIO32WriteFldAlign(VB1SYS0,0xf,VB1SYS0_LANE_EN_MUX);
#endif  
}

void vVByOneTestMode(void)
{
#ifndef CC_VB1_DISABLE
	vIO32WriteFldAlign(REG_LVDS_TX_CFG13,0xfff,RG_LVDSA_PRE_PWD);
	vIO32WriteFldAlign(VB1SYS3,0x1,VB1SYS3_LANE3_ANA);
	vIO32WriteFldAlign(VB1SYS3,0x1,VB1SYS3_LANE2_ANA);
	vIO32WriteFldAlign(VB1SYS3,0x1,VB1SYS3_LANE1_ANA);
	vIO32WriteFldAlign(VB1SYS3,0x1,VB1SYS3_LANE0_ANA);	
	vIO32WriteFldAlign(VB1SYS3,0x3FF,VB1SYS3_LANE3_ANA_VALUE);		
#endif  
}

 
