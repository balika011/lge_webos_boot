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

// === INCLUDE =============================================================================
#include "x_os.h"	
#include "sv_const.h"
#include "hw_vdoin.h"
#include "nptv_debug.h"
#include "drv_pq_cust.h"
#include "drv_ycproc.h"
#include "drv_tdtv_drvif.h"
#include "fbm_drvif.h"
#include "drv_display.h"

// === HW INCLUDE =============================================================================
#include "hw_ycproc.h"
#include "hw_sw.h"
// === DEFINE =============================================================================
#define SCE_PHASE 28

#define SUPPORT_NEW_C_BOOST     SV_TRUE

#define RANGE_3D_GAIN     128

#define SCE_LUMA_NUM        16
#define SCE_SAT_NUM         28
#define SCE_HUE_NUM         28

#define SKIN_START_28       10  // SKIN_START_28 phase can't be 0. Start phase must be larger than end phase
#define SKIN_END_28         12  // SKIN_END_28 phase can't be 27 

#define SKIN_START_16       6   // SKIN_START_28 phase can't be 0. Start phase must be larger than end phase
#define SKIN_END_16         6   // SKIN_END_16 phase can't be 15

#define UINT8_CLIP(val, min, max) ((UINT8)((val>=max) ? max : ((val<=min) ? min : val)))  


// === FUNCTION PROTOTYPE =================================================================

// === EXTERN FUNCTION ====================================================================

// === GLOBAL VARIABLE ====================================================================
UINT8 bSceGain[3]  = {0x80, 0x80, 0x80};
UINT8 bSkinTable[3] = {0x80, 0x80, 0x80};
UINT8 bSce3dGain[2] = {0x80, 0x80};

// === EXTERN VARIABLE ====================================================================
EXTERN UINT8 bForceAllPQOff;
EXTERN UINT8 gSCE_3DMapping;


// === STATIC VARIABLE ====================================================================
static UINT16 wAngularHue[BASE_COLOR_MAX]; // hue : 128 as 0
static UINT16 wAngularSat[BASE_COLOR_MAX]; // sat : 128 as 1.0
static UINT16 wAngularBri[BASE_COLOR_MAX]; // bri : 128 as 0
// === BODY ===============================================================================


/*****************************************************************************************/
/**************************************  INITIALIZE  *************************************/
/*****************************************************************************************/
void vDrvSCEHWInit(void)
{

    // Initialize
    vDrvSCEAllOnOff(SV_ON);
	vDrv3DCMSInit();
    vDrvSCELoadTable();
    
    vIO32WriteFldMulti(G_PIC_ADJ_MAIN_1, P_Fld(0x400, BRIGHTNESS) | P_Fld(0x80, CONTRAST));
    vIO32WriteFldMulti(G_PIC_ADJ_MAIN_2, P_Fld(0x200, HUE) | P_Fld(0x80, SAT));
    vIO32WriteFldAlign(G_PIC_ADJ_MAIN_3, 0x200, POST_HUE);
    vIO32WriteFldAlign(LCOLORNOISE_1, 0x1, LCN_SAT_SRC);
    vIO32WriteFldAlign(LUMA_ADJ, 0x40, Y_SLOPE_LMT);
    vIO32WriteFldAlign(LUMA_ADJ, 0x40, Y_LEV_ADJ);
    vIO32WriteFldAlign(C_BOOST_MAIN, 0x2, CBOOST_YOFFSET_SEL);    
    vIO32WriteFldMulti(C_BOOST_MAIN_2, P_Fld(0x40, CBOOST_YOFFSET) | 
                                 P_Fld(0x10, HUE_HIST_STH));

	
#ifdef CC_MT5880
    if(IS_IC_5880_ES())
    {
        vIO32WriteFldAlign(CH_DLY_MAIN, 2, Y_DEL);        
    }
    else if(IS_IC_5880_ES2())
    {
        vIO32WriteFldAlign(CH_DLY_MAIN, 2, Y_DEL);
        vIO32WriteFldAlign(CH_DLY_MAIN, 2, CB_DEL);
        vIO32WriteFldAlign(CH_DLY_MAIN, 2, CR_DEL);
    }
    else
    {
        vIO32WriteFldAlign(CH_DLY_MAIN, 0, Y_DEL);
        vIO32WriteFldAlign(CH_DLY_MAIN, 0, CB_DEL);
        vIO32WriteFldAlign(CH_DLY_MAIN, 0, CR_DEL);
    }
#endif
 
}

void vDrvSCESWInit(void)
{
    // Customerization Setting
    vIO32WriteFldAlign(MATRIX_00, DRVCUST_OptGet(eFlagUseMatrixPQAdj), MATRIX_PQ_ADJ);
    vIO32WriteFldAlign(SCE_REG_00, DRVCUST_OptGet(eBrightInterval), SCE_BRI_INTERVAL);
    vIO32WriteFldAlign(SCE_REG_00, DRVCUST_OptGet(eHueInterval), SCE_HUE_INTERVAL);    
    vIO32WriteFldAlign(SCE_REG_00, 1, SCE_CALC_ONOFF);
    vIO32WriteFldAlign(TDTV_FW_00, 0x80, SCE_3D_GAIN_00);
    vIO32WriteFldAlign(TDTV_FW_00, 0x80, SCE_3D_GAIN_01);
    vDrvSCEAngularReset();
}

void vDrvSCEInit(void)
{
    vDrvSCESWInit();
    vDrvSCEHWInit();
}

/*****************************************************************************************/
/***********************************  SYSTEM & DEBUG  ************************************/
/*****************************************************************************************/
void vDrvSCEAllOnOff(UINT8 bOnOff)
{
    UINT8 bModuleOnOff = bOnOff && (!bForceAllPQOff) && wReadQualityTable(QUALITY_MODULE_ONOFF_SCE);   
	vIO32WriteFldAlign(SCE_CFG_MAIN, ~bModuleOnOff, ALLBP);
}

void vDrvSCEParam(void)
{
    // LCN
    vIO32WriteFldMulti(LCOLORNOISE_1,
              P_Fld(!wReadQualityTable(QUALITY_LCN_ONOFF), LCN_BYPASS) |
              P_Fld(wReadQualityTable(QUALITY_LCN_LIRE_YTH), LIRE_YTH) |
              P_Fld(wReadQualityTable(QUALITY_LCN_LIRE_YSLOPE), LIRE_YSLOPE) |
              P_Fld(wReadQualityTable(QUALITY_LCN_LIRE_STH), LIRE_STH) |
              P_Fld(wReadQualityTable(QUALITY_LCN_LIRE_SSLOPE), LIRE_SSLOPE));
    vIO32WriteFldMulti(LCOLORNOISE_2,
              P_Fld(wReadQualityTable(QUALITY_LCN_LIRE_GAIN), LIRE_GAIN) |
              P_Fld(wReadQualityTable(QUALITY_LCN_LSAT_GAIN), LSAT_GAIN) |
              P_Fld(wReadQualityTable(QUALITY_LCN_LSAT_STH), LSAT_STH) |
              P_Fld(wReadQualityTable(QUALITY_LCN_LSAT_SSLOPE), LSAT_SSLOPE));

    // AL_SKIN
    vIO32WriteFldMulti(Y_FTN2_SKIN_RANGE_1,
              P_Fld(!wReadQualityTable(QUALITY_ALCOM_SKIN_LUMA_EN), Y_FTN2_BYPASS) |
              P_Fld(wReadQualityTable(QUALITY_SKIN_YMIN) << 2, Y_FTN2_YMIN) |
              P_Fld(wReadQualityTable(QUALITY_SKIN_YMAX) << 2, Y_FTN2_YMAX) |
              P_Fld(wReadQualityTable(QUALITY_SKIN_YSLOPE), Y_FTN2_YSLOPE));
    vIO32WriteFldMulti(Y_FTN2_SKIN_RANGE_2,
              P_Fld(wReadQualityTable(QUALITY_SKIN_SMIN) << 2, Y_FTN2_SMIN) |
              P_Fld(wReadQualityTable(QUALITY_SKIN_SMAX) << 2, Y_FTN2_SMAX) |
              P_Fld(wReadQualityTable(QUALITY_SKIN_SSLOPE), Y_FTN2_SSLOPE));
    vIO32WriteFldMulti(Y_FTN2_SKIN_RANGE_3,
              P_Fld(wReadQualityTable(QUALITY_SKIN_HMIN) << 2, Y_FTN2_HMIN) |
              P_Fld(wReadQualityTable(QUALITY_SKIN_HMAX) << 2, Y_FTN2_HMAX) |
              P_Fld(wReadQualityTable(QUALITY_SKIN_HSLOPE), Y_FTN2_HSLOPE));

    /* Mustang Modified : Chroma Boost */
    vIO32WriteFldMulti(C_BOOST_MAIN,
              P_Fld(wReadQualityTable(QUALITY_CBOOST_ONOFF), NEW_CBOOST_EN) |
              P_Fld(wReadQualityTable(QUALITY_CBOOST_GAIN), CBOOST_GAIN) |
              P_Fld(wReadQualityTable(QUALITY_CBOOST_LMT_U), NEW_CBOOST_LMT_U) |
              P_Fld(wReadQualityTable(QUALITY_CBOOST_LMT_L), NEW_CBOOST_LMT_L));

    // Y BitPlus
    vIO32WriteFldMulti(BITPLUS_1,
              P_Fld(wReadQualityTable(QUALITY_Y_BITPLUS_Q4), Y_BITPLUS_Q4) |
              P_Fld(wReadQualityTable(QUALITY_Y_BITPLUS_Q3), Y_BITPLUS_Q3) |
              P_Fld(wReadQualityTable(QUALITY_Y_BITPLUS_Q2), Y_BITPLUS_Q2) |
              P_Fld(wReadQualityTable(QUALITY_Y_BITPLUS_Q1), Y_BITPLUS_Q1) |
              P_Fld(wReadQualityTable(QUALITY_Y_BITPLUS_YRANGE), Y_BITPLUS_YRANGE));
    vIO32WriteFldMulti(BITPLUS_2,
              P_Fld(wReadQualityTable(QUALITY_Y_BITPLUS_EN), Y_BITPLUS_EN) |
              P_Fld(wReadQualityTable(QUALITY_Y_BITPLUS_P3), Y_BITPLUS_P3) |
              P_Fld(wReadQualityTable(QUALITY_Y_BITPLUS_P2), Y_BITPLUS_P2) |
              P_Fld(wReadQualityTable(QUALITY_Y_BITPLUS_P1), Y_BITPLUS_P1));
    vIO32WriteFldMulti(BITPLUS_3,
              //P_Fld(!wReadQualityTable(QUALITY_Y_BITPLUS_EN), Y_BITPLUS_BYPASS) |
              P_Fld(wReadQualityTable(QUALITY_Y_BITPLUS_RANDOM_EN), Y_BITPLUS_RANDOM_EN) |
              P_Fld(wReadQualityTable(QUALITY_Y_BITPLUS_SHIFT), Y_BITPLUS_SHIFT) |
              P_Fld(wReadQualityTable(QUALITY_Y_BITPLUS_G34), Y_BITPLUS_G34) |
              P_Fld(wReadQualityTable(QUALITY_Y_BITPLUS_G23), Y_BITPLUS_G23) |              
              P_Fld(wReadQualityTable(QUALITY_Y_BITPLUS_G12), Y_BITPLUS_G12) |
              P_Fld(wReadQualityTable(QUALITY_Y_BITPLUS_P4), Y_BITPLUS_P4));
    // S BitPlus
#if defined (CC_MT5399)
    vIO32WriteFldMulti(BITPLUS_4,
              P_Fld(wReadQualityTable(QUALITY_S_BITPLUS_Q4), S_BITPLUS_Q4) |
              P_Fld(wReadQualityTable(QUALITY_S_BITPLUS_Q3), S_BITPLUS_Q3) |
              P_Fld(wReadQualityTable(QUALITY_S_BITPLUS_Q2), S_BITPLUS_Q2) |
              P_Fld(wReadQualityTable(QUALITY_S_BITPLUS_Q1), S_BITPLUS_Q1) |
              P_Fld(wReadQualityTable(QUALITY_S_BITPLUS_YRANGE), S_BITPLUS_SRANGE));
    vIO32WriteFldMulti(BITPLUS_5,
              P_Fld(wReadQualityTable(QUALITY_S_BITPLUS_EN), S_BITPLUS_EN) |
              P_Fld(wReadQualityTable(QUALITY_S_BITPLUS_P3), S_BITPLUS_P3) |
              P_Fld(wReadQualityTable(QUALITY_S_BITPLUS_P2), S_BITPLUS_P2) |
              P_Fld(wReadQualityTable(QUALITY_S_BITPLUS_P1), S_BITPLUS_P1));
    vIO32WriteFldMulti(BITPLUS_6,
              //P_Fld(!wReadQualityTable(QUALITY_S_BITPLUS_EN), S_BITPLUS_BYPASS) |
              P_Fld(wReadQualityTable(QUALITY_S_BITPLUS_RANDOM_EN), S_BITPLUS_RANDOM_EN) |
              P_Fld(wReadQualityTable(QUALITY_S_BITPLUS_SHIFT), S_BITPLUS_SHIFT) |
              P_Fld(wReadQualityTable(QUALITY_S_BITPLUS_G34), S_BITPLUS_G34) |
              P_Fld(wReadQualityTable(QUALITY_S_BITPLUS_G23), S_BITPLUS_G23) |              
              P_Fld(wReadQualityTable(QUALITY_S_BITPLUS_G12), S_BITPLUS_G12) |
              P_Fld(wReadQualityTable(QUALITY_S_BITPLUS_P4), S_BITPLUS_P4));  
#endif

    vIO32WriteFldAlign(ADAP_LUMA2_00, SV_OFF, ADAP_BITPLUS_EN);

    
} 

void vDrvSCELoadTable(void)
{
    UINT32 i, j, u4Temp, u4Addr;
	CMSCtrlPt SCECMSCtrlPt;
	CMSGain   SCECMSGain;
	UINT32 u4wgtL, u4wgtU, u4Win_En;

    // Partial Y Function
    for (i = 0; i < 8; i++)
    {
        u4Addr = Y_SLOPE_1_0_MAIN + 4 * i;
        u4Temp = (READ_SCE_TBL(ENUM_Y_SLOPE, 2 * i)) | 
                 (READ_SCE_TBL(ENUM_Y_SLOPE, 2 * i + 1) << 16);
    	vIO32Write4B(u4Addr, u4Temp);
    }

    // CCMP
    u4Temp = (READ_SCE_TBL(ENUM_Y_SLOPE, 16)) | 
             (READ_SCE_TBL(ENUM_Y_SLOPE, 17) << 8) | 
             (READ_SCE_TBL(ENUM_Y_SLOPE, 18) << 16) |
             (READ_SCE_TBL(ENUM_Y_SLOPE, 19) << 24);
    vIO32Write4B(CCMP_GROUP_1, u4Temp);

    u4Temp = (READ_SCE_TBL(ENUM_Y_SLOPE, 20)) | 
             (READ_SCE_TBL(ENUM_Y_SLOPE, 21) << 8) | 
             (READ_SCE_TBL(ENUM_Y_SLOPE, 22) << 16) |
             (READ_SCE_TBL(ENUM_Y_SLOPE, 23) << 24);
    vIO32Write4B(CCMP_GROUP_2, u4Temp);

    u4Temp = READ_SCE_TBL(ENUM_Y_SLOPE, 24);
    vIO32Write1B(CCMP_GROUP_3, u4Temp);

    // Partial Saturation Function
    vIO32WriteFldAlign(LOCAL_SAT_1, 1, R_SAT_CPURW_ACTIVE);             // enable cpu RW
    vIO32WriteFldAlign(LOCAL_SAT_1, 1, R_SAT_WR_AUTO_INC_ADDR);         // set write auto-incremental
    vIO32WriteFldAlign(LOCAL_SAT_2, 0, SAT_CPU_SRAM_ADDR);              // set initial address to 0
    for (i = 0; i < SCE_PHASE; i++)
    {
        u4Temp = (READ_SCE_TBL(ENUM_S_GAIN3, i)) |
		         ((READ_SCE_TBL(ENUM_S_Y000, i)) << 8) |
		         ((READ_SCE_TBL(ENUM_S_Y064, i)) << 16) |
		         ((READ_SCE_TBL(ENUM_S_Y128, i)) << 24);
        vIO32Write4B(LOCAL_SAT_4, u4Temp);

        u4Temp = (READ_SCE_TBL(ENUM_S_Y192, i)) |
		         ((READ_SCE_TBL(ENUM_S_Y256, i)) << 8) |
		         ((READ_SCE_TBL(ENUM_LCMP_M11L, i)) << 16) |
		         ((READ_SCE_TBL(ENUM_LCMP_M11M, i)) << 24);
        vIO32Write4B(LOCAL_SAT_5, u4Temp);
        
        u4Temp = (READ_SCE_TBL(ENUM_LCMP_M12L, i)) |
		         ((READ_SCE_TBL(ENUM_LCMP_M12M, i)) << 8) |
		         ((READ_SCE_TBL(ENUM_LCMP_M13L, i)) << 10) |
		         ((READ_SCE_TBL(ENUM_LCMP_M13M, i)) << 18) |
                 ((READ_SCE_TBL(ENUM_LCMP_M21L, i)) << 20) |
                 ((READ_SCE_TBL(ENUM_LCMP_M21M, i)) << 28);
        vIO32Write4B(LOCAL_SAT_6, u4Temp);

        u4Temp = (READ_SCE_TBL(ENUM_LCMP_M22L, i)) |
		         ((READ_SCE_TBL(ENUM_LCMP_M22M, i)) << 8) |
		         ((READ_SCE_TBL(ENUM_LCMP_M23L, i)) << 10) |
		         ((READ_SCE_TBL(ENUM_LCMP_M23M, i)) << 18);
        vIO32Write4B(LOCAL_SAT_7, u4Temp);

        u4Temp = (READ_SCE_TBL(ENUM_S_P1, i)) |
		         ((READ_SCE_TBL(ENUM_S_P2, i)) << 8) |
		         ((READ_SCE_TBL(ENUM_S_GAIN1, i)) << 16) |
		         ((READ_SCE_TBL(ENUM_S_GAIN2, i)) << 24);
        vIO32Write4B(LOCAL_SAT_3, u4Temp);   // LOCAL_SAT_3: SRAM addr increment trigger
    }                                           
    vIO32WriteFldAlign(LOCAL_SAT_1, 0, R_SAT_CPURW_ACTIVE);             // enable cpu RW
    vIO32WriteFldAlign(LOCAL_SAT_1, 0, R_SAT_WR_AUTO_INC_ADDR);         // set write auto-incremental
    vIO32WriteFldAlign(LOCAL_SAT_2, 0, SAT_CPU_SRAM_ADDR);              // set initial address to 0

	// Partial Hue Function
    vIO32WriteFldAlign(LOCAL_HUE_1, 1, R_HUE_CPURW_ACTIVE);             // enable cpu RW
    vIO32WriteFldAlign(LOCAL_HUE_1, 1, R_HUE_WR_AUTO_INC_ADDR);         // set write auto-incremental
    vIO32WriteFldAlign(LOCAL_HUE_2, 0, HUE_CPU_SRAM_ADDR);              // set initial address to 0
    for (i = 0; i < SCE_PHASE; i++)
    {
        u4Temp = ((UINT32)READ_SCE_TBL(ENUM_H_Y192, i)) +
		         (((UINT32)READ_SCE_TBL(ENUM_H_Y256, i)) << 8) +
		         (((UINT32)READ_SCE_TBL(ENUM_H_S000, i)) << 16) +
		         (((UINT32)READ_SCE_TBL(ENUM_H_S032, i)) << 24);
        vIO32Write4B(LOCAL_HUE_4, u4Temp);
        
        u4Temp = ((UINT32)READ_SCE_TBL(ENUM_H_S064, i)) +
		         (((UINT32)READ_SCE_TBL(ENUM_H_S096, i)) << 8) +
		         (((UINT32)READ_SCE_TBL(ENUM_H_S128, i)) << 16);
        vIO32Write4B(LOCAL_HUE_5, u4Temp);

        u4Temp = ((UINT32)READ_SCE_TBL(ENUM_H_FTN, i)) +
		         (((UINT32)READ_SCE_TBL(ENUM_H_Y000, i)) << 8) +
		         (((UINT32)READ_SCE_TBL(ENUM_H_Y064, i)) << 16) +
		         (((UINT32)READ_SCE_TBL(ENUM_H_Y128, i)) << 24);
        vIO32Write4B(LOCAL_HUE_3, u4Temp);   // LOCAL_HUE_3: SRAM addr increment trigger
    }                                           
    vIO32WriteFldAlign(LOCAL_HUE_1, 0, R_HUE_CPURW_ACTIVE);             // enable cpu RW    
    vIO32WriteFldAlign(LOCAL_HUE_1, 0, R_HUE_WR_AUTO_INC_ADDR);         // set write auto-incremental
    vIO32WriteFldAlign(LOCAL_HUE_2, 0, HUE_CPU_SRAM_ADDR);              // set initial address to 0

	//3D color mapping
	if (gSCE_3DMapping == TRUE)
	{
		for (i = 0; i < 3; i++)
		{
			for (j = 0; j < 3; j++)
			{
				// set curve for luma
			    SCECMSCtrlPt.u1CMSBoundL = (READ_SCE_TBL(ENUM_3DMAP_W1H+i*3+j, 1) + (READ_SCE_TBL(ENUM_3DMAP_W1H+i*3+j, 0) <<8) );
			    SCECMSCtrlPt.u1CMSP0     = (READ_SCE_TBL(ENUM_3DMAP_W1H+i*3+j, 3) + (READ_SCE_TBL(ENUM_3DMAP_W1H+i*3+j, 2) <<8) );
			    SCECMSCtrlPt.u1CMSP1     = (READ_SCE_TBL(ENUM_3DMAP_W1H+i*3+j, 5) + (READ_SCE_TBL(ENUM_3DMAP_W1H+i*3+j, 4) <<8) );
			    SCECMSCtrlPt.u1CMSP2     = (READ_SCE_TBL(ENUM_3DMAP_W1H+i*3+j, 7) + (READ_SCE_TBL(ENUM_3DMAP_W1H+i*3+j, 6) <<8) );
			    SCECMSCtrlPt.u1CMSP3     = (READ_SCE_TBL(ENUM_3DMAP_W1H+i*3+j, 9) + (READ_SCE_TBL(ENUM_3DMAP_W1H+i*3+j, 8) <<8) );
			    SCECMSCtrlPt.u1CMSP4     = (READ_SCE_TBL(ENUM_3DMAP_W1H+i*3+j, 11) + (READ_SCE_TBL(ENUM_3DMAP_W1H+i*3+j, 10) <<8) );   
			    SCECMSCtrlPt.u1CMSBoundU = (READ_SCE_TBL(ENUM_3DMAP_W1H+i*3+j, 13) + (READ_SCE_TBL(ENUM_3DMAP_W1H+i*3+j, 12) <<8) );       

			    SCECMSGain.u1CMSG0 = READ_SCE_TBL(ENUM_3DMAP_W1H+i*3+j, 14);
			    SCECMSGain.u1CMSG1 = READ_SCE_TBL(ENUM_3DMAP_W1H+i*3+j, 15);
			    SCECMSGain.u1CMSG2 = READ_SCE_TBL(ENUM_3DMAP_W1H+i*3+j, 16);
			    SCECMSGain.u1CMSG3 = READ_SCE_TBL(ENUM_3DMAP_W1H+i*3+j, 17);
			    SCECMSGain.u1CMSG4 = READ_SCE_TBL(ENUM_3DMAP_W1H+i*3+j, 18);
			    SCECMSGain.u1CMSG5 = READ_SCE_TBL(ENUM_3DMAP_W1H+i*3+j, 19);

				vDrv3DCMSSetCurve(i, j, SCECMSCtrlPt, SCECMSGain);

				u4wgtL = (READ_SCE_TBL(ENUM_3DMAP_W1H+i*3+j, 21) + (READ_SCE_TBL(ENUM_3DMAP_W1H+i*3+j, 20) <<8) );
				u4wgtU = (READ_SCE_TBL(ENUM_3DMAP_W1H+i*3+j, 23) + (READ_SCE_TBL(ENUM_3DMAP_W1H+i*3+j, 22) <<8) );

				vDrv3DCMSSetWeight(i, j, u4wgtL, u4wgtU);
				
			}
			u4Win_En = READ_SCE_TBL(ENUM_3DMAP_W1H+i*3+2, 24);
			vDrv3DCMSWindowOnOff(u4Win_En, i);
		}
	}
}

UINT32 vDrvSCELoadIdx(UINT32 u4FunIdx, UINT32 u4Phase, UINT32 u4Value)
{
    UINT32 u4Temp;
    
    if ((u4FunIdx >= ENUM_END) || (u4Phase >= SCE_PHASE))
    {
        LOG(0, "SCE index out of range: function:%d, hue:%d\n", u4FunIdx, u4Phase);
        return SV_FAIL;
    }

    WRITE_SCE_TBL(u4FunIdx, u4Phase, (UINT8)(u4Value & 0xFF));

    if (u4FunIdx == ENUM_Y_SLOPE)
    {
        if (u4Phase <= 15)
        {
            u4Temp = (u4Phase >> 1);
            vIO32Write4B(Y_SLOPE_1_0_MAIN + 4 * u4Temp,
                        ((UINT32)READ_SCE_TBL(ENUM_Y_SLOPE, 2 * u4Temp)) |
                        ((UINT32)READ_SCE_TBL(ENUM_Y_SLOPE, 2 * u4Temp + 1) << 16));
        }
        else if ((u4Phase >= 16) && (u4Phase <= 24))
        {
            UINT32 u4Temp = 4 * ((u4Phase-16) >> 2);
            vIO32Write4B(CCMP_GROUP_1 + u4Temp,
                        (READ_SCE_TBL(ENUM_Y_SLOPE, 16 + u4Temp)) | 
                        (READ_SCE_TBL(ENUM_Y_SLOPE, 17 + u4Temp) << 8) | 
                        (READ_SCE_TBL(ENUM_Y_SLOPE, 18 + u4Temp) << 16) |
                        (READ_SCE_TBL(ENUM_Y_SLOPE, 19 + u4Temp) << 24));
        }
        else
        {
            LOG(0, "SCE index out of range: function:%d, hue:%d\n", u4FunIdx, u4Phase);
            return SV_FAIL;
        }
    }
    else if ((u4FunIdx >= ENUM_S_GAIN1) && (u4FunIdx <= ENUM_S_Y256))
    {
		// saturation related function
        vIO32WriteFldAlign(LOCAL_SAT_1, 1, R_SAT_CPURW_ACTIVE);             // enable cpu RW
        vIO32WriteFldAlign(LOCAL_SAT_1, 0, R_SAT_WR_AUTO_INC_ADDR);         // set write auto-incremental
        vIO32WriteFldAlign(LOCAL_SAT_2, u4Phase, R_SAT_ADDR);       // set initial address

        u4Temp = (READ_SCE_TBL(ENUM_S_GAIN3, u4Phase)) |
                 ((READ_SCE_TBL(ENUM_S_Y000, u4Phase)) << 8) |
                 ((READ_SCE_TBL(ENUM_S_Y064, u4Phase)) << 16) |
                 ((READ_SCE_TBL(ENUM_S_Y128, u4Phase)) << 24);
        vIO32Write4B(LOCAL_SAT_4, u4Temp);
    
        u4Temp = (READ_SCE_TBL(ENUM_S_Y192, u4Phase)) |
                 ((READ_SCE_TBL(ENUM_S_Y256, u4Phase)) << 8) |
                 ((READ_SCE_TBL(ENUM_LCMP_M11L, u4Phase)) << 16) |
                 ((READ_SCE_TBL(ENUM_LCMP_M11M, u4Phase)) << 24);
        vIO32Write4B(LOCAL_SAT_5, u4Temp);
        
        u4Temp = (READ_SCE_TBL(ENUM_LCMP_M12L, u4Phase)) |
                 ((READ_SCE_TBL(ENUM_LCMP_M12M, u4Phase)) << 8) |
                 ((READ_SCE_TBL(ENUM_LCMP_M13L, u4Phase)) << 10) |
                 ((READ_SCE_TBL(ENUM_LCMP_M13M, u4Phase)) << 18) |
                 ((READ_SCE_TBL(ENUM_LCMP_M21L, u4Phase)) << 20) |
                 ((READ_SCE_TBL(ENUM_LCMP_M21M, u4Phase)) << 28);
        vIO32Write4B(LOCAL_SAT_6, u4Temp);
    
        u4Temp = (READ_SCE_TBL(ENUM_LCMP_M22L, u4Phase)) |
                 ((READ_SCE_TBL(ENUM_LCMP_M22M, u4Phase)) << 8) |
                 ((READ_SCE_TBL(ENUM_LCMP_M23L, u4Phase)) << 10) |
                 ((READ_SCE_TBL(ENUM_LCMP_M23M, u4Phase)) << 18);
        vIO32Write4B(LOCAL_SAT_7, u4Temp);
    
        u4Temp = (READ_SCE_TBL(ENUM_S_P1, u4Phase)) |
                 ((READ_SCE_TBL(ENUM_S_P2, u4Phase)) << 8) |
                 ((READ_SCE_TBL(ENUM_S_GAIN1, u4Phase)) << 16) |
                 ((READ_SCE_TBL(ENUM_S_GAIN2, u4Phase)) << 24);
        vIO32Write4B(LOCAL_SAT_3, u4Temp);   // LOCAL_SAT_3: SRAM addr increment trigger

        vIO32WriteFldAlign(LOCAL_SAT_1, 0, R_SAT_CPURW_ACTIVE);             // enable cpu RW
        vIO32WriteFldAlign(LOCAL_SAT_2, 0, SAT_CPU_SRAM_ADDR);              // set initial address to 0
    }
    else
    {
		// hue related function
        vIO32WriteFldAlign(LOCAL_HUE_1, 1, R_HUE_CPURW_ACTIVE);             // enable cpu RW
        vIO32WriteFldAlign(LOCAL_HUE_1, 0, R_HUE_WR_AUTO_INC_ADDR);         // set write auto-incremental
        vIO32WriteFldAlign(LOCAL_HUE_2, u4Phase, R_HUE_ADDR);       // set initial address

        u4Temp = ((UINT32)READ_SCE_TBL(ENUM_H_Y192, u4Phase)) +
                 (((UINT32)READ_SCE_TBL(ENUM_H_Y256, u4Phase)) << 8) +
                 (((UINT32)READ_SCE_TBL(ENUM_H_S000, u4Phase)) << 16) +
                 (((UINT32)READ_SCE_TBL(ENUM_H_S032, u4Phase)) << 24);
        vIO32Write4B(LOCAL_HUE_4, u4Temp);
        
        u4Temp = ((UINT32)READ_SCE_TBL(ENUM_H_S064, u4Phase)) +
                 (((UINT32)READ_SCE_TBL(ENUM_H_S096, u4Phase)) << 8) +
                 (((UINT32)READ_SCE_TBL(ENUM_H_S128, u4Phase)) << 16);
        vIO32Write4B(LOCAL_HUE_5, u4Temp);
    
        u4Temp = ((UINT32)READ_SCE_TBL(ENUM_H_FTN, u4Phase)) +
                 (((UINT32)READ_SCE_TBL(ENUM_H_Y000, u4Phase)) << 8) +
                 (((UINT32)READ_SCE_TBL(ENUM_H_Y064, u4Phase)) << 16) +
                 (((UINT32)READ_SCE_TBL(ENUM_H_Y128, u4Phase)) << 24);
        vIO32Write4B(LOCAL_HUE_3, u4Temp);   // LOCAL_HUE_3: SRAM addr increment trigger

        vIO32WriteFldAlign(LOCAL_HUE_1, 0, R_HUE_CPURW_ACTIVE);             // enable cpu RW    
        vIO32WriteFldAlign(LOCAL_HUE_2, 0, HUE_CPU_SRAM_ADDR);              // set initial address to 0
    }
    return SV_TRUE;
}

static void _vDrvSCEReadTable(void)
{
	UINT32 u4Temp, u4Phase;

	// Partial Y Function
	for (u4Phase = 0; u4Phase < 8; u4Phase++)
	{
		WRITE_SCE_TBL(ENUM_Y_SLOPE, 2*u4Phase, IO32ReadFldAlign(Y_SLOPE_1_0_MAIN + 4 * u4Phase, Y_SLOPE_0));
		WRITE_SCE_TBL(ENUM_Y_SLOPE, 2*u4Phase+1, IO32ReadFldAlign(Y_SLOPE_1_0_MAIN + 4 * u4Phase, Y_SLOPE_1));
	}

    // CCMP
	u4Temp = u4IO32Read4B(CCMP_GROUP_1);
    WRITE_SCE_TBL(ENUM_Y_SLOPE, 16, u4Temp & 0xFF);
    WRITE_SCE_TBL(ENUM_Y_SLOPE, 17, (u4Temp & 0xFF00)>>8);
    WRITE_SCE_TBL(ENUM_Y_SLOPE, 18, (u4Temp & 0xFF0000)>>16);
    WRITE_SCE_TBL(ENUM_Y_SLOPE, 19, (u4Temp & 0xFF000000)>>24);
    u4Temp = u4IO32Read4B(CCMP_GROUP_2);
    WRITE_SCE_TBL(ENUM_Y_SLOPE, 20, u4Temp & 0xFF);
    WRITE_SCE_TBL(ENUM_Y_SLOPE, 21, (u4Temp & 0xFF00)>>8);
    WRITE_SCE_TBL(ENUM_Y_SLOPE, 22, (u4Temp & 0xFF0000)>>16);
	WRITE_SCE_TBL(ENUM_Y_SLOPE, 23, (u4Temp & 0xFF000000)>>24);
	WRITE_SCE_TBL(ENUM_Y_SLOPE, 24, u1IO32Read1B(CCMP_GROUP_3));

	// Partial Saturation Function
    vIO32WriteFldAlign(LOCAL_SAT_1, 1, R_SAT_CPURW_ACTIVE);           // enable cpu RW
    vIO32WriteFldAlign(LOCAL_SAT_1, 1, R_SAT_RD_AUTO_INC_ADDR);       // set read auto-incremental
    vIO32WriteFldAlign(LOCAL_SAT_2, 0, R_SAT_ADDR);                   // set initial address to 0

	for (u4Phase = 0; u4Phase < SCE_PHASE; u4Phase++)
	{
		// sFtn
        u4Temp = u4IO32Read4B(LOCAL_SAT_8);
        WRITE_SCE_TBL(ENUM_S_P1, u4Phase, u4Temp & 0xFF);
        WRITE_SCE_TBL(ENUM_S_P2, u4Phase, (u4Temp & 0xFF00) >> 8);
        WRITE_SCE_TBL(ENUM_S_GAIN1, u4Phase, (u4Temp & 0xFF0000) >> 16);
        WRITE_SCE_TBL(ENUM_S_GAIN2, u4Phase, (u4Temp & 0xFF000000) >> 24);
    	u4Temp = u4IO32Read4B(LOCAL_SAT_9);
		WRITE_SCE_TBL(ENUM_S_GAIN3, u4Phase, u4Temp & 0xFF);

		// sGain
		WRITE_SCE_TBL(ENUM_S_Y000, u4Phase, (u4Temp & 0xFF00) >> 8);
		WRITE_SCE_TBL(ENUM_S_Y064, u4Phase, (u4Temp & 0xFF0000) >> 16);
		WRITE_SCE_TBL(ENUM_S_Y128, u4Phase, (u4Temp & 0xFF000000) >> 24);
    	u4Temp = u4IO32Read4B(LOCAL_SAT_10);
		WRITE_SCE_TBL(ENUM_S_Y192, u4Phase, u4Temp & 0xFF);
		WRITE_SCE_TBL(ENUM_S_Y256, u4Phase, (u4Temp & 0xFF00) >> 8);

        // LCMP
        WRITE_SCE_TBL(ENUM_LCMP_M11L, u4Phase, (u4Temp & 0xFF0000) >> 16);
        WRITE_SCE_TBL(ENUM_LCMP_M11M, u4Phase, (u4Temp & 0x3000000) >> 24);
    	u4Temp = u4IO32Read4B(LOCAL_SAT_11);
        WRITE_SCE_TBL(ENUM_LCMP_M12L, u4Phase, u4Temp & 0xFF);
        WRITE_SCE_TBL(ENUM_LCMP_M12M, u4Phase, (u4Temp & 0x300) >> 8);
        WRITE_SCE_TBL(ENUM_LCMP_M13L, u4Phase, (u4Temp & 0x3FC00) >> 10);
        WRITE_SCE_TBL(ENUM_LCMP_M13M, u4Phase, (u4Temp & 0xC0000) >> 18);
        WRITE_SCE_TBL(ENUM_LCMP_M21L, u4Phase, (u4Temp & 0xFF00000) >> 20);
        WRITE_SCE_TBL(ENUM_LCMP_M21M, u4Phase, (u4Temp & 0x30000000) >> 28);
    	u4Temp = u4IO32Read4B(LOCAL_SAT_12);
        WRITE_SCE_TBL(ENUM_LCMP_M22L, u4Phase, u4Temp & 0xFF);
        WRITE_SCE_TBL(ENUM_LCMP_M22M, u4Phase, (u4Temp & 0x300) >> 8);
        WRITE_SCE_TBL(ENUM_LCMP_M23L, u4Phase, (u4Temp & 0x3FC00) >> 10);
        WRITE_SCE_TBL(ENUM_LCMP_M23M, u4Phase, (u4Temp & 0xC0000) >> 18);
	}
    vIO32WriteFldAlign(LOCAL_SAT_1, 0, R_SAT_CPURW_ACTIVE);           // enable cpu RW
    vIO32WriteFldAlign(LOCAL_SAT_1, 0, R_SAT_RD_AUTO_INC_ADDR);       // set read auto-incremental
    vIO32WriteFldAlign(LOCAL_SAT_2, 0, R_SAT_ADDR);                   // set initial address to 0

	// Partial Hue Function
    vIO32WriteFldAlign(LOCAL_HUE_1, 1, R_HUE_CPURW_ACTIVE);           // enable cpu RW
    vIO32WriteFldAlign(LOCAL_HUE_1, 1, R_HUE_RD_AUTO_INC_ADDR);       // set read auto-incremental
    vIO32WriteFldAlign(LOCAL_HUE_2, 0, R_HUE_ADDR);                   // set initial address to 0

	for (u4Phase = 0; u4Phase < SCE_PHASE; u4Phase++)
	{
		// hFtn
    	u4Temp = u4IO32Read4B(LOCAL_HUE_6);
		WRITE_SCE_TBL(ENUM_H_FTN, u4Phase, u4Temp & 0xFF);

		// h at y
		WRITE_SCE_TBL(ENUM_H_Y000, u4Phase, (u4Temp & 0xFF00)>>8);
		WRITE_SCE_TBL(ENUM_H_Y064, u4Phase, (u4Temp & 0xFF0000)>>16);
		WRITE_SCE_TBL(ENUM_H_Y128, u4Phase, (u4Temp & 0xFF000000)>>24);
    	u4Temp = u4IO32Read4B(LOCAL_HUE_7);
		WRITE_SCE_TBL(ENUM_H_Y192, u4Phase, u4Temp & 0xFF);
		WRITE_SCE_TBL(ENUM_H_Y256, u4Phase, (u4Temp & 0xFF00)>>8);

		// h at s
		WRITE_SCE_TBL(ENUM_H_S000, u4Phase, (u4Temp & 0xFF0000)>>16);
		WRITE_SCE_TBL(ENUM_H_S032, u4Phase, (u4Temp & 0xFF000000)>>24);
    	u4Temp = u4IO32Read4B(LOCAL_HUE_8);
		WRITE_SCE_TBL(ENUM_H_S064, u4Phase, u4Temp & 0xFF);
		WRITE_SCE_TBL(ENUM_H_S096, u4Phase, (u4Temp & 0xFF00)>>8) ;
		WRITE_SCE_TBL(ENUM_H_S128, u4Phase, (u4Temp & 0xFF0000)>>16);
	}
    vIO32WriteFldAlign(LOCAL_HUE_1, 0, R_HUE_CPURW_ACTIVE);           // enable cpu RW
    vIO32WriteFldAlign(LOCAL_HUE_1, 0, R_HUE_RD_AUTO_INC_ADDR);       // set read auto-incremental
    vIO32WriteFldAlign(LOCAL_HUE_2, 0, R_HUE_ADDR);                   // set initial address to 0
}

static void _vDrvSetFindColor(UINT32 u4FunIdx, UINT32 u4Phase, UINT32 u4Value)
{
    if (u4FunIdx <= ENUM_S_Y256)
    {
        UINT32 u4Temp;

        vIO32WriteFldAlign(LOCAL_SAT_1, 1, R_SAT_CPURW_ACTIVE);
        vIO32WriteFldAlign(LOCAL_SAT_1, 0, R_SAT_WR_AUTO_INC_ADDR);
        vIO32WriteFldAlign(LOCAL_SAT_2, u4Phase, R_SAT_ADDR);
         
        u4Temp = (READ_SCE_TBL(ENUM_S_GAIN3, u4Phase)) |
                 ((READ_SCE_TBL(ENUM_S_Y000, u4Phase)) << 8) |
                 ((READ_SCE_TBL(ENUM_S_Y064, u4Phase)) << 16) |
                 ((READ_SCE_TBL(ENUM_S_Y128, u4Phase)) << 24);
        vIO32Write4B(LOCAL_SAT_4, u4Temp);
         
        u4Temp = (READ_SCE_TBL(ENUM_S_Y192, u4Phase)) |
                 ((READ_SCE_TBL(ENUM_S_Y256, u4Phase)) << 8) |
                 ((READ_SCE_TBL(ENUM_LCMP_M11L, u4Phase)) << 16) |
                 ((READ_SCE_TBL(ENUM_LCMP_M11M, u4Phase)) << 24);
        vIO32Write4B(LOCAL_SAT_5, u4Temp);
         
        u4Temp = (READ_SCE_TBL(ENUM_LCMP_M12L, u4Phase)) |
                 ((READ_SCE_TBL(ENUM_LCMP_M12M, u4Phase)) << 8) |
                 ((READ_SCE_TBL(ENUM_LCMP_M13L, u4Phase)) << 10) |
                 ((READ_SCE_TBL(ENUM_LCMP_M13M, u4Phase)) << 18) |
                 ((READ_SCE_TBL(ENUM_LCMP_M21L, u4Phase)) << 20) |
                 ((READ_SCE_TBL(ENUM_LCMP_M21M, u4Phase)) << 28);
        vIO32Write4B(LOCAL_SAT_6, u4Temp);
        
        u4Temp = (READ_SCE_TBL(ENUM_LCMP_M22L, u4Phase)) |
                 ((READ_SCE_TBL(ENUM_LCMP_M22M, u4Phase)) << 8) |
                 ((READ_SCE_TBL(ENUM_LCMP_M23L, u4Phase)) << 10) |
                 ((READ_SCE_TBL(ENUM_LCMP_M23M, u4Phase)) << 18);
        vIO32Write4B(LOCAL_SAT_7, u4Temp);
                 
        u4Temp = (READ_SCE_TBL(ENUM_S_P1, u4Phase)) |
                 ((READ_SCE_TBL(ENUM_S_P2, u4Phase)) << 8) |
                 (u4Value << 16) |
                 (u4Value << 24);
        vIO32Write4B(LOCAL_SAT_3, u4Temp);   // LOCAL_SAT_3: SRAM addr increment trigger
         
        vIO32WriteFldAlign(LOCAL_SAT_1, 0, R_SAT_CPURW_ACTIVE);
        vIO32WriteFldAlign(LOCAL_SAT_2, 0, R_SAT_ADDR);
    }
    else if (u4FunIdx <= ENUM_H_S128)
    {
        UINT32 u4Temp3, u4Temp4, u4Temp5, u4Sft;

        u4Temp3 = ((UINT32)READ_SCE_TBL(ENUM_H_FTN, u4Phase)) +
                  (((UINT32)READ_SCE_TBL(ENUM_H_Y000, u4Phase)) << 8) +
                  (((UINT32)READ_SCE_TBL(ENUM_H_Y064, u4Phase)) << 16) +
                  (((UINT32)READ_SCE_TBL(ENUM_H_Y128, u4Phase)) << 24);
        u4Temp4 = ((UINT32)READ_SCE_TBL(ENUM_H_Y192, u4Phase)) +
                  (((UINT32)READ_SCE_TBL(ENUM_H_Y256, u4Phase)) << 8) +
                  (((UINT32)READ_SCE_TBL(ENUM_H_S000, u4Phase)) << 16) +
                  (((UINT32)READ_SCE_TBL(ENUM_H_S032, u4Phase)) << 24);
        u4Temp5 = ((UINT32)READ_SCE_TBL(ENUM_H_S064, u4Phase)) +
                  (((UINT32)READ_SCE_TBL(ENUM_H_S096, u4Phase)) << 8) +
                  (((UINT32)READ_SCE_TBL(ENUM_H_S128, u4Phase)) << 16);

        if (u4FunIdx <= ENUM_H_Y128)
        {
            u4Sft = (u4FunIdx - ENUM_H_FTN) << 3;
            u4Temp3 = (u4Temp3 & (~(0xFF<<u4Sft))) | ((u4Value)<<u4Sft);
        }
        else if (u4FunIdx <= ENUM_H_S032)
        {
            u4Sft = (u4FunIdx - ENUM_H_Y192) << 3;
            u4Temp4 = (u4Temp4 & (~(0xFF<<u4Sft))) | ((u4Value)<<u4Sft);
        }
        else
        {
            u4Sft = (u4FunIdx - ENUM_H_S064) << 3;
            u4Temp5 = (u4Temp5 & (~(0xFF<<u4Sft))) | ((u4Value)<<u4Sft);
        }

        vIO32WriteFldAlign(LOCAL_HUE_1, 1, R_HUE_CPURW_ACTIVE);
        vIO32WriteFldAlign(LOCAL_HUE_1, 0, R_HUE_WR_AUTO_INC_ADDR);
        vIO32WriteFldAlign(LOCAL_HUE_2, u4Phase, R_HUE_ADDR);

        vIO32Write4B(LOCAL_HUE_4, u4Temp4);
        vIO32Write4B(LOCAL_HUE_5, u4Temp5);
        vIO32Write4B(LOCAL_HUE_3, u4Temp3);   // LOCAL_HUE_3: SRAM addr increment trigger

        vIO32WriteFldAlign(LOCAL_HUE_1, 0, R_HUE_CPURW_ACTIVE);
        vIO32WriteFldAlign(LOCAL_HUE_2, 0, R_HUE_ADDR);
    }
    else
    {
        LOG(1, "SCE Find Color u4Phase Error!");
    }
}

void vDrvSCEFindColor(void)
{
    static UINT32 u4PrePhase = 0;
    static UINT32 u4PreFunIdx = 0;
    static UINT32 u4SCECounter = 0;
    static UINT32 u4FindColorOnOff = SV_OFF;
    UINT32 u4Phase, u4FunIdx;

    if ((u4FindColorOnOff == SV_OFF) && (IO32ReadFldAlign(SCE_REG_00, SCE_FINDCOLOR_ONOFF) == SV_ON))
	{
        // First time enable find color state
        _vDrvSCEReadTable();
	}
    else if ((u4FindColorOnOff == SV_ON) && (IO32ReadFldAlign(SCE_REG_00, SCE_FINDCOLOR_ONOFF) == SV_OFF))
	{
        // Disable find color state
        vDrvSCELoadTable();
	}

	u4FindColorOnOff = IO32ReadFldAlign(SCE_REG_00, SCE_FINDCOLOR_ONOFF);

	if (u4FindColorOnOff == SV_ON)
	{
		u4Phase = IO32ReadFldAlign(SCE_REG_00, SCE_FINDCOLOR_PHASE);
        u4FunIdx = IO32ReadFldAlign(SCE_REG_00, SCE_FINDCOLOR_FUN_IDX);

		if ((u4Phase < SCE_PHASE) && (u4PrePhase < SCE_PHASE))
		{
			if ((u4Phase != u4PrePhase) || (u4FunIdx != u4PreFunIdx))
			{
                vDrvSCELoadTable();
			}

			if (u4SCECounter % 150 == 60)
			{
                _vDrvSetFindColor(u4FunIdx, u4Phase, 0xFF);
			}

			if (u4SCECounter % 150 == 0)
			{
                _vDrvSetFindColor(u4FunIdx, u4Phase, 0x0);
			}
		}
		u4PrePhase = u4Phase;
        u4PreFunIdx = u4FunIdx;
		u4SCECounter++;
	}
}

void vDrvSCEPatGen(UINT8 bMode, UINT16 Ydata)
{
	if (bMode == SV_OFF)
	{
		vIO32WriteFldAlign(DBG_CFG_MAIN, SV_OFF, INK_EN);
	}
	else
	{
        vIO32WriteFldMulti(DBG_CFG_MAIN,
                  P_Fld(SV_ON, INK_EN) |
                  P_Fld(1, Y_REPLACE) |
                  P_Fld(1, CB_REPLACE) |
                  P_Fld(1, CR_REPLACE) |
                  P_Fld(bMode, Y_INK_MODE) |
                  P_Fld(bMode, CB_INK_MODE) |
                  P_Fld(bMode, CR_INK_MODE));
        vIO32WriteFldMulti(INK_DATA_MAIN,
                  P_Fld(Ydata, INK_DATA_Y) |
                  P_Fld(0x200, INK_DATA_CB));
		vIO32WriteFldAlign(INK_DATA_MAIN_CR, 0x200, INK_DATA_CR);
	}
}

#ifndef NDEBUG    // PQ SRAM Stress Test Only Take Effect in Debug Build
UINT8 bDrvSCEVerifyTable(void)
{
    UINT32 i;
	UINT32 u4Read, u4Write;
    
	// Partial Saturation Function
    vIO32WriteFldAlign(LOCAL_SAT_1, 1, R_SAT_CPURW_ACTIVE);           // enable cpu RW
    vIO32WriteFldAlign(LOCAL_SAT_1, 1, R_SAT_RD_AUTO_INC_ADDR);       // set read auto-incremental
    vIO32WriteFldAlign(LOCAL_SAT_2, 0, R_SAT_ADDR);                   // set initial address to 0
	for (i = 0; i < SCE_PHASE; i++)
	{
        u4Read = u4IO32Read4B(LOCAL_SAT_8);		
        u4Write = (READ_SCE_TBL(ENUM_S_P1, i)) |
                  ((READ_SCE_TBL(ENUM_S_P2, i)) << 8) |
                  ((READ_SCE_TBL(ENUM_S_GAIN1, i)) << 16) |
                  ((READ_SCE_TBL(ENUM_S_GAIN2, i)) << 24);
        if(u4Read != u4Write)
        {
            return SV_FALSE;
        }
        
		u4Read = u4IO32Read4B(LOCAL_SAT_9);
        u4Write = (READ_SCE_TBL(ENUM_S_GAIN3, i)) |
                  ((READ_SCE_TBL(ENUM_S_Y000, i)) << 8) |
                  ((READ_SCE_TBL(ENUM_S_Y064, i)) << 16) |
                  ((READ_SCE_TBL(ENUM_S_Y128, i)) << 24);
        if(u4Read != u4Write)
        {
            return SV_FALSE;
        }
        
        u4Read = u4IO32Read4B(LOCAL_SAT_10);
        u4Write = (READ_SCE_TBL(ENUM_S_Y192, i)) |
		         ((READ_SCE_TBL(ENUM_S_Y256, i)) << 8) |
		         ((READ_SCE_TBL(ENUM_LCMP_M11L, i)) << 16) |
		         ((READ_SCE_TBL(ENUM_LCMP_M11M, i)) << 24);
        u4Read &= 0x3FFFFFF;
        u4Write &= 0x3FFFFFF;
        if(u4Read != u4Write)
        {
            return SV_FALSE;
        }

        u4Read = u4IO32Read4B(LOCAL_SAT_11);
        u4Write = (READ_SCE_TBL(ENUM_LCMP_M12L, i)) |
		         ((READ_SCE_TBL(ENUM_LCMP_M12M, i)) << 8) |
		         ((READ_SCE_TBL(ENUM_LCMP_M13L, i)) << 10) |
		         ((READ_SCE_TBL(ENUM_LCMP_M13M, i)) << 18) |
                 ((READ_SCE_TBL(ENUM_LCMP_M21L, i)) << 20) |
                 ((READ_SCE_TBL(ENUM_LCMP_M21M, i)) << 28);
        u4Read &= 0x3FFFFFFF;
        u4Write &= 0x3FFFFFFF;
        if(u4Read != u4Write)
        {
            return SV_FALSE;
        }

        u4Read = u4IO32Read4B(LOCAL_SAT_12);
        u4Write = (READ_SCE_TBL(ENUM_LCMP_M22L, i)) |
                  ((READ_SCE_TBL(ENUM_LCMP_M22M, i)) << 8) |
                  ((READ_SCE_TBL(ENUM_LCMP_M23L, i)) << 10) |
                  ((READ_SCE_TBL(ENUM_LCMP_M23M, i)) << 18);
        u4Read &= 0xFFFFF;
        u4Write &= 0xFFFFF;
        if(u4Read != u4Write)
        {
            return SV_FALSE;
        }
	}
    vIO32WriteFldAlign(LOCAL_SAT_1, 0, R_SAT_CPURW_ACTIVE);           // enable cpu RW
    vIO32WriteFldAlign(LOCAL_SAT_1, 0, R_SAT_RD_AUTO_INC_ADDR);       // set read auto-incremental
    vIO32WriteFldAlign(LOCAL_SAT_2, 0, R_SAT_ADDR);                   // set initial address to 0

	// Partial Hue Function
    vIO32WriteFldAlign(LOCAL_HUE_1, 1, R_HUE_CPURW_ACTIVE);           // enable cpu RW
    vIO32WriteFldAlign(LOCAL_HUE_1, 1, R_HUE_RD_AUTO_INC_ADDR);       // set read auto-incremental
    vIO32WriteFldAlign(LOCAL_HUE_2, 0, R_HUE_ADDR);                   // set initial address to 0
	for (i = 0; i < SCE_PHASE; i++)
	{
        u4Read = u4IO32Read4B(LOCAL_HUE_6);
        u4Write = ((UINT32)READ_SCE_TBL(ENUM_H_FTN, i)) +
                  (((UINT32)READ_SCE_TBL(ENUM_H_Y000, i)) << 8) +
                  (((UINT32)READ_SCE_TBL(ENUM_H_Y064, i)) << 16) +
                  (((UINT32)READ_SCE_TBL(ENUM_H_Y128, i)) << 24);
        if(u4Read != u4Write)
        {
            return SV_FALSE;
        }

        u4Read = u4IO32Read4B(LOCAL_HUE_7);
        u4Write = ((UINT32)READ_SCE_TBL(ENUM_H_Y192, i)) +
                  (((UINT32)READ_SCE_TBL(ENUM_H_Y256, i)) << 8) +
                  (((UINT32)READ_SCE_TBL(ENUM_H_S000, i)) << 16) +
                  (((UINT32)READ_SCE_TBL(ENUM_H_S032, i)) << 24);
        if(u4Read != u4Write)
        {
            return SV_FALSE;
        }

        u4Read = u4IO32Read4B(LOCAL_HUE_8);
        u4Write = ((UINT32)READ_SCE_TBL(ENUM_H_S064, i)) +
                  (((UINT32)READ_SCE_TBL(ENUM_H_S096, i)) << 8) +
                  (((UINT32)READ_SCE_TBL(ENUM_H_S128, i)) << 16);
        u4Read &= 0xFFFFFF;
        u4Write &= 0xFFFFFF;
        if(u4Read != u4Write)
        {
            return SV_FALSE;
        }
	}
    vIO32WriteFldAlign(LOCAL_HUE_1, 0, R_HUE_CPURW_ACTIVE);           // enable cpu RW
    vIO32WriteFldAlign(LOCAL_HUE_1, 0, R_HUE_RD_AUTO_INC_ADDR);       // set read auto-incremental
    vIO32WriteFldAlign(LOCAL_HUE_2, 0, R_HUE_ADDR);                   // set initial address to 0

	return SV_TRUE;
}
#endif

/*****************************************************************************************/
/***************************   SCE TABLE CALCULATION  ************************************/
/*****************************************************************************************/
void vDrvSCEInterpolation(UINT16* inval, UINT8* inpos, UINT8 insize, UINT16* outval, UINT8 outsize)
{
    UINT32 i, j;
    INT32 diff;

    if ((inval == NULL) || (inpos == NULL) || (outval == NULL))
    {
        return;
    }
    
    for (i=0; i<insize; i++)
    {
        if (i == (insize-1))
        {
            diff = ((INT32)inval[0] - inval[i])/((inpos[0] + outsize - inpos[i]));
            
            for (j=inpos[i]; j<(inpos[0]+outsize); j++)
            {   
                if (j < outsize)
                {
                    outval[j] = inval[i]+diff*(j-inpos[i]);
                }
                else
                {
                    outval[j-outsize] = inval[i]+diff*(j-inpos[i]);
                }
            }
        }
        else
        {
            diff = ((INT32)inval[i+1] - inval[i])/((inpos[i+1] - inpos[i]));
            for (j=inpos[i]; j<inpos[i+1]; j++)
            {   
                outval[j] = inval[i]+diff*(j-inpos[i]);
            }
        }        
    }

    LOG(6, "input size %d \n", insize);
    for (i=0; i<insize; i++)
    {
        LOG(6, "[%d] = %d \n", inpos[i], inval[i]);
    }
    
    LOG(6, "output size %d \n", outsize);
    for (i=0; i<outsize; i++)
    {
        LOG(6, "[%d] = %d \n", i, outval[i]);
    }
}

void vDrvSetSixColorReg(void)
{
    vIO32Write4B(SIXCOLOR_REG_00, wAngularBri[0]|(wAngularBri[1]<<8)|
                                (wAngularBri[2]<<16)|(wAngularBri[3]<<24));
    vIO32Write4B(SIXCOLOR_REG_01, wAngularBri[4]|(wAngularBri[5]<<8)|
                                (wAngularSat[0]<<16)|(wAngularSat[1]<<24));
    vIO32Write4B(SIXCOLOR_REG_02, wAngularSat[2]|(wAngularSat[3]<<8)|
                                (wAngularSat[4]<<16)|(wAngularSat[5]<<24));
    vIO32Write4B(SIXCOLOR_REG_03, wAngularHue[0]|(wAngularHue[1]<<8)|
                                (wAngularHue[2]<<16)|(wAngularHue[3]<<24));
    vIO32Write4B(SIXCOLOR_REG_04, wAngularHue[4]|(wAngularHue[5]<<8));
    vIO32WriteFldAlign(ADAPTIVE_SCE_REG, SV_ON, ADAPTIVE_SCE_FORCE_LOAD);

    LOG(5, "\n=== Six Color ====\n", wAngularBri[0], wAngularBri[1], 
        wAngularBri[2], wAngularBri[3], wAngularBri[4], wAngularBri[5]);
    LOG(5, "Six Color Y %d %d %d %d %d %d\n", wAngularBri[0], wAngularBri[1], 
        wAngularBri[2], wAngularBri[3], wAngularBri[4], wAngularBri[5]);
    LOG(5, "Six Color S %d %d %d %d %d %d\n", wAngularSat[0], wAngularSat[1], 
        wAngularSat[2], wAngularSat[3], wAngularSat[4], wAngularSat[5]);
    LOG(5, "Six Color H %d %d %d %d %d %d\n", wAngularHue[0], wAngularHue[1], 
        wAngularHue[2], wAngularHue[3], wAngularHue[4], wAngularHue[5]);    
}

void vDrvGetSixColorReg(void)
{
    wAngularBri[0] = IO32ReadFldAlign(SIXCOLOR_REG_00, SIXCOLOR_Y_M);
    wAngularBri[1] = IO32ReadFldAlign(SIXCOLOR_REG_00, SIXCOLOR_Y_R);
    wAngularBri[2] = IO32ReadFldAlign(SIXCOLOR_REG_00, SIXCOLOR_Y_Y);
    wAngularBri[3] = IO32ReadFldAlign(SIXCOLOR_REG_00, SIXCOLOR_Y_G);
    wAngularBri[4] = IO32ReadFldAlign(SIXCOLOR_REG_01, SIXCOLOR_Y_C);
    wAngularBri[5] = IO32ReadFldAlign(SIXCOLOR_REG_01, SIXCOLOR_Y_B);
    wAngularSat[0] = IO32ReadFldAlign(SIXCOLOR_REG_01, SIXCOLOR_S_M);
    wAngularSat[1] = IO32ReadFldAlign(SIXCOLOR_REG_01, SIXCOLOR_S_R);
    wAngularSat[2] = IO32ReadFldAlign(SIXCOLOR_REG_02, SIXCOLOR_S_Y);
    wAngularSat[3] = IO32ReadFldAlign(SIXCOLOR_REG_02, SIXCOLOR_S_G);
    wAngularSat[4] = IO32ReadFldAlign(SIXCOLOR_REG_02, SIXCOLOR_S_C);
    wAngularSat[5] = IO32ReadFldAlign(SIXCOLOR_REG_02, SIXCOLOR_S_B);
    wAngularHue[0] = IO32ReadFldAlign(SIXCOLOR_REG_03, SIXCOLOR_H_M);
    wAngularHue[1] = IO32ReadFldAlign(SIXCOLOR_REG_03, SIXCOLOR_H_R);
    wAngularHue[2] = IO32ReadFldAlign(SIXCOLOR_REG_03, SIXCOLOR_H_Y);
    wAngularHue[3] = IO32ReadFldAlign(SIXCOLOR_REG_03, SIXCOLOR_H_G);
    wAngularHue[4] = IO32ReadFldAlign(SIXCOLOR_REG_04, SIXCOLOR_H_C);
    wAngularHue[5] = IO32ReadFldAlign(SIXCOLOR_REG_04, SIXCOLOR_H_B);
}

void vDrvSCEAngularReset(void)
{
	UINT16 i;

	for (i = 0; i < BASE_COLOR_MAX; i++)
	{
		wAngularHue[i] = 0x80;
		wAngularSat[i] = 0x80;
		wAngularBri[i] = 0x80;
	}

	vDrvSetSixColorReg();
}

void vDrvSCEAngularSetVal(UINT8 bmodule, UINT8 bColor, UINT8 bValue)
{
    if (bColor >= BASE_COLOR_MAX)
    {
        return;
    }

    switch (bmodule)
    {
        case SCE_BRI:
            wAngularBri[bColor] = bValue;
            break;
        case SCE_HUE:
            wAngularHue[bColor] = bValue;
            break;
        case SCE_SAT:
            wAngularSat[bColor] = bValue;
            break;
        default:
            break;
    }
    
	vDrvSetSixColorReg();
}

void vDrvSetSCELevel(UINT8 bLumaGain, UINT8 bSatGain, UINT8 bHueGain)
{
    vIO32WriteFldAlign(ADAPTIVE_SCE_REG, bLumaGain, ADAPTIVE_SCE_L);
    vIO32WriteFldAlign(ADAPTIVE_SCE_REG, bSatGain, ADAPTIVE_SCE_S);
    vIO32WriteFldAlign(ADAPTIVE_SCE_REG, bHueGain, ADAPTIVE_SCE_H);
    vIO32WriteFldAlign(ADAPTIVE_SCE_REG, SV_ON, ADAPTIVE_SCE_FORCE_LOAD);
}

void vDrvSetSkinLevel(UINT8 bLumaGain, UINT8 bSatGain, UINT8 bHueGain)
{
	vIO32WriteFldAlign(ADAPTIVE_SKIN_REG, bLumaGain, ADAPTIVE_SKIN_L);
	vIO32WriteFldAlign(ADAPTIVE_SKIN_REG, bSatGain, ADAPTIVE_SKIN_S);
	vIO32WriteFldAlign(ADAPTIVE_SKIN_REG, bHueGain, ADAPTIVE_SKIN_H);
	vIO32WriteFldAlign(ADAPTIVE_SCE_REG, SV_ON, ADAPTIVE_SCE_FORCE_LOAD);
}

void vDrvSCE3dGain(void)
{
    INT32 i4Tmp, i4Gaindiff;
    UINT32 i, j, u4Idx;
    INT32 d1, d2, d3;

    LOG(5, "\n3D SCE gain :  %d   %d\n", bSce3dGain[0], bSce3dGain[1]);  

    for (j=ENUM_S_GAIN1; j<=ENUM_S_GAIN3; j++)
    {
        // no effect if second point > RANGE_3D_GAIN
    	for (i = 0; i < SCE_SAT_NUM; i++)
        {
            u4Idx = j*SCE_SAT_NUM+i;
            i4Tmp = DEFAULT_SCE_TBL[u4Idx]; 
            
            if (j == ENUM_S_GAIN1)
            {                        
                i4Tmp = i4Tmp*bSce3dGain[0]/128;
            }
                    
            if (j == ENUM_S_GAIN2)
                    {
                i4Tmp = i4Tmp*bSce3dGain[1]/128;
                    }
            
            if (j == ENUM_S_GAIN3)
            {
                d1 = BASE_SCE_TBL[ENUM_S_P1*SCE_SAT_NUM+i]; 
                d2 = BASE_SCE_TBL[ENUM_S_P2*SCE_SAT_NUM+i] - BASE_SCE_TBL[ENUM_S_P1*SCE_SAT_NUM+i];
                d3 = RANGE_3D_GAIN - BASE_SCE_TBL[ENUM_S_P2*SCE_SAT_NUM+i];

                i4Gaindiff = (d1*(0x80-bSce3dGain[0])+d2*(0x80-bSce3dGain[1]))/d3;
                i4Tmp = i4Tmp*(0x80+i4Gaindiff)/128;
            }            
            
            DEFAULT_SCE_TBL[u4Idx] = UINT8_CLIP(i4Tmp, 0, 255);
            LOG(6, "[%d]  %d  => %d \n", u4Idx, DEFAULT_SCE_TBL[u4Idx], i4Tmp);
        }
    }              
}

void vDrvSCECalculateOnOff(UINT8 bOnOff)
{    
    vIO32WriteFldAlign(SCE_REG_00, bOnOff?1:0, SCE_CALC_ONOFF);
}

void vDrvSCECalculate(void)
{
    static UINT8 bSCE_Y_Pos[BASE_COLOR_MAX] = {2, 5, 8, 10, 13, 15};
    static UINT8 bSCE_S_Pos[BASE_COLOR_MAX] = {2, 8, 16, 22, 24, 27};
    static UINT8 bSCE_H_Pos[BASE_COLOR_MAX] = {2, 8, 16, 22, 24, 27};

	UINT32 i, j, u4Idx;
	INT32 i4Tmp;
    UINT16 wSceYTable[SCE_LUMA_NUM];
    UINT16 wSceSTable[SCE_SAT_NUM];
    UINT16 wSceHTable[SCE_HUE_NUM];
    
    vDrvGetSixColorReg();
    
    vDrvSCEInterpolation(wAngularBri, bSCE_Y_Pos, BASE_COLOR_MAX, wSceYTable, SCE_LUMA_NUM);
    vDrvSCEInterpolation(wAngularSat, bSCE_S_Pos, BASE_COLOR_MAX, wSceSTable, SCE_SAT_NUM);
    vDrvSCEInterpolation(wAngularHue, bSCE_H_Pos, BASE_COLOR_MAX, wSceHTable, SCE_HUE_NUM);

    LOG(5, "\n bSceGain[0] = %d  bSkinTable[0] %d\n", bSceGain[0], bSkinTable[0]);
	for (i = 0; i < SCE_LUMA_NUM; i++)
	{   	    
        i4Tmp = ((((INT32)BASE_SCE_TBL[i]-MIN_SCE_TBL[i])*bSceGain[0]+64)>>7)+MIN_SCE_TBL[i]; 	

        // Six color 
        i4Tmp = wSceYTable[i]+i4Tmp-0x80;

        // Skin 
        if ((i >= SKIN_START_16) && (i <= SKIN_END_16))
        {
            i4Tmp += ((INT32)bSkinTable[0]-0x80);
        }    
        else if ((i == (SKIN_START_16-1)) || (i == (SKIN_END_16+1)))
        {
            i4Tmp += ((INT32)bSkinTable[0]-0x80+1)>>1;
        }
        
        LOG(6, "[%d]  %d   %d  => %d \n", i, BASE_SCE_TBL[i], MIN_SCE_TBL[i], i4Tmp);
        
	    DEFAULT_SCE_TBL[i] = UINT8_CLIP(i4Tmp, 1, 255);
	}

	LOG(5, "\n bSceGain[1] = %d  bSkinTable[1] = %d \n", bSceGain[1], bSkinTable[1]);  
    for (j=ENUM_S_GAIN1; j<ENUM_S_GAIN3+1; j++)
    {
    	for (i = 0; i < SCE_SAT_NUM; i++)
        {
            u4Idx = j*SCE_SAT_NUM+i;
            i4Tmp = ((((INT32)BASE_SCE_TBL[u4Idx]-MIN_SCE_TBL[u4Idx])*bSceGain[1]+64)>>7)+MIN_SCE_TBL[u4Idx]; 
            
            // Six color 
            i4Tmp = (i4Tmp*wSceSTable[i]+64)>>7;

            // Skin 
            if ((i >= SKIN_START_28) && (i <= SKIN_END_28))
            {
                i4Tmp = (i4Tmp*bSkinTable[1]+64)>>7;
            }
            else if ((i == (SKIN_START_28-1)) ||  (i == (SKIN_END_28+1)))
            {
                i4Tmp = (i4Tmp*((((INT32)bSkinTable[1]-0x80+1)>>1)+0x80))>>7;
            }
            
            LOG(6, "[%d]  %d   %d  => %d \n", i, BASE_SCE_TBL[u4Idx], MIN_SCE_TBL[u4Idx], i4Tmp);
            
            DEFAULT_SCE_TBL[u4Idx] = UINT8_CLIP(i4Tmp, 0, 255);
        }
    }

    LOG(5, "\n bSceGain[2] = %d  bSkinTable[2] = %d\n", bSceGain[2], bSkinTable[2]);  
    for (j=ENUM_H_FTN; j<ENUM_H_S128+1; j++)
    {        
    	for (i = 0; i < SCE_HUE_NUM; i++)
        {
            u4Idx = j*SCE_HUE_NUM+i;
            i4Tmp = ((((INT32)BASE_SCE_TBL[u4Idx]-MIN_SCE_TBL[u4Idx])*bSceGain[2]+64)>>7)+MIN_SCE_TBL[u4Idx]; 

            // Six color and Skin only works on Hue primary effect
            if (j == ENUM_H_FTN) 
            {                
                // Six color 
                i4Tmp = wSceHTable[i]+i4Tmp-0x80;

                // Skin
                if ((i >= SKIN_START_28) && (i <= SKIN_END_28))
                {
                    i4Tmp += ((INT32)bSkinTable[2]-0x80);
                }
                else if ((i == (SKIN_START_28-1)) ||  (i == (SKIN_END_28+1)))
                {
                    i4Tmp += ((INT32)bSkinTable[2]-0x80+1)>>1;
                }
            }

            LOG(6, "[%d]  %d   %d  => %d \n", i, BASE_SCE_TBL[u4Idx], MIN_SCE_TBL[u4Idx], i4Tmp);
            DEFAULT_SCE_TBL[u4Idx] = UINT8_CLIP(i4Tmp, 1, 255);
        }
    }

    if (u4DrvTDTV3DModeQuery() != E_TDTV_UI_3D_MODE_OFF)
    {
        vDrvSCE3dGain();
    }    
}

void vDrvSetSCEContrast(UINT16 u2Value)
{
    vRegWriteFldAlign(G_PIC_ADJ_MAIN_1, u2Value, CONTRAST);
}

void vDrvSetSCEBrightness(UINT16 u2Value)
{
    vRegWriteFldAlign(G_PIC_ADJ_MAIN_1, u2Value, BRIGHTNESS);
}

void vDrvSetSCESaturation(UINT16 u2Value)
{
    vRegWriteFldAlign(G_PIC_ADJ_MAIN_2, u2Value, SAT);
}

void vDrvSetSCEHue(UINT16 u2Value)
{
    vRegWriteFldAlign(G_PIC_ADJ_MAIN_2, u2Value, HUE);
}

/*****************************************************************************************/
/*************************   GLOBAL PQ ADJUSTMENT   **************************************/
/*****************************************************************************************/
void vDrvSetContrast(UINT8 bPath, UINT16 u2Value)
{
    UINT32 u4Addr = GLOBAL_ADJ_02 + ((bPath == SV_VP_MAIN) ? 0 : 0x4);
    vIO32WriteFldAlign(u4Addr, u2Value, CONT_DUMMY);
}

void vDrvSetBrightness(UINT8 bPath, UINT16 wValue)
{
    UINT32 u4Addr = GLOBAL_ADJ_00 + ((bPath == SV_VP_MAIN) ? 0 : 0x4);
    vIO32WriteFldAlign(u4Addr, wValue, BRIGHT_DUMMY);
}

void vDrvSetSaturation(UINT8 bPath, UINT16 u2Value)
{
    UINT32 u4Addr = GLOBAL_ADJ_02 + ((bPath == SV_VP_MAIN) ? 0 : 0x4);
    vIO32WriteFldAlign(u4Addr, u2Value, SAT_DUMMY);
}

void vDrvSetHue(UINT8 bPath, UINT16 wValue)
{
    UINT32 u4Addr = GLOBAL_ADJ_00 + ((bPath == SV_VP_MAIN) ? 0 : 0x4);
    vIO32WriteFldAlign(u4Addr, wValue, HUE_DUMMY);
}

/*****************************************************************************************/
/*************************   External PQ ADJUSTMENT   **************************************/
/*****************************************************************************************/
void vDrvSetExtContrast(UINT8 bPath, UINT16 wValue)
{
    UINT32 u4Addr = GLOBAL_ADJ_04 + ((bPath == SV_VP_MAIN) ? 0 : 0x4);
    vIO32WriteFldAlign(u4Addr, wValue, EXT_ADJ_CONT);
}

void vDrvSetExtBrightness(UINT8 bPath, UINT16 wValue)
{
    UINT32 u4Addr = GLOBAL_ADJ_04 + ((bPath == SV_VP_MAIN) ? 0 : 0x4);
    vIO32WriteFldAlign(u4Addr, wValue, EXT_ADJ_BRI);
}

void vDrvSetExtSaturation(UINT8 bPath, UINT16 wValue)
{
    UINT32 u4Addr = GLOBAL_ADJ_04 + ((bPath == SV_VP_MAIN) ? 0 : 0x4);
    vIO32WriteFldAlign(u4Addr, wValue, EXT_ADJ_SAT);
}

void vDrvSetExtHue(UINT8 bPath, UINT16 wValue)
{
    UINT32 u4Addr = GLOBAL_ADJ_04 + ((bPath == SV_VP_MAIN) ? 0 : 0x4);
    vIO32WriteFldAlign(u4Addr, wValue, EXT_ADJ_HUE);
}

/*****************************************************************************************/
/**********************************   BIT PLUS   *****************************************/
/*****************************************************************************************/
/*
    Set Luma Range: 0x00 ~ 0x3F, default 0x08
*/
void vDrvBitPlusSetYRange(UINT8 u1Value)
{
    vIO32WriteFldAlign(BITPLUS_1, u1Value, Y_BITPLUS_YRANGE);
}
/*
    Set Sat Range: 0x00 ~ 0x3F, default 0x10
*/



void vDrvBitPlusSetSRange(UINT8 u1Value)
{
	#if defined(CC_MT5399) //mt5882 not support s bitplus
    vIO32WriteFldAlign(BITPLUS_4, u1Value, S_BITPLUS_SRANGE);
	
	#endif
}


/*
    Set Luma Strength: 0x00 ~ 0x07, 0x00: weak, 0x01: medium, 0x02: strong
*/
void vDrvBitPlusSetYShift(UINT8 u1Value)
{
    vIO32WriteFldAlign(BITPLUS_2, u1Value, Y_BITPLUS_SDIFF_SHIFT);
}
/*
    Set Sat Strength: 0x00 ~ 0x07, 0x00: weak, 0x01: medium, 0x02: strong
*/



void vDrvBitPlusSetSShift(UINT8 u1Value)
{
	#if defined(CC_MT5399) //mt5882 not support s bitplus
    vIO32WriteFldAlign(BITPLUS_5, u1Value, S_BITPLUS_SDIFF_SHIFT);
	
	#endif
}




UINT8 u1DrvBitPlusYIsBypass(void)
{
    return (IO32ReadFldAlign(BITPLUS_3, Y_BITPLUS_BYPASS));   
}



UINT8 u1DrvBitPlusSIsBypass(void)
{
	#if defined(CC_MT5399) //mt5882 not support s bitplus
    	return (IO32ReadFldAlign(BITPLUS_6, S_BITPLUS_BYPASS));
	#else
		return SV_TRUE;
	#endif
}



/*****************************************************************************************/
/**********************************   DEMO MODE   ****************************************/
/*****************************************************************************************/
#if SUPPORT_SEAMLESS_MODE
void vDrvColorDemo(UINT8 bOnOff, UINT8 bInside)
{
    #ifdef CC_FLIP_MIRROR_SUPPORT
	if((u4QueryFlipModule(SV_VP_MAIN) & (FLIP_BY_SCALER | FLIP_BY_PSCAN | FLIP_BY_B2R) )&&(u1GetFlipMirrorConfig()& SYS_MIRROR_CONFIG_ON))
	{bInside = !bInside;}
    #endif
    vIO32WriteFldAlign(DBG_CFG_MAIN, !bInside, SPLIT_SWAP);
    vIO32WriteFldAlign(DBG_CFG_MAIN, bOnOff, SPLIT_EN);
}
void vDrvSetColorDemoRange(Region rRegion)
{

        //Define demo region
        vIO32WriteFldMulti(WIN_X_MAIN, P_Fld(rRegion.wHStart, WIN_X_START) | P_Fld(rRegion.wHEnd, WIN_X_END));
        vIO32WriteFldMulti(WIN_Y_MAIN, P_Fld(rRegion.wVStart, WIN_Y_START) | P_Fld(rRegion.wVEnd, WIN_Y_END));
}

#endif //SUPPORT_SEAMLESS_MODE

/*****************************************************************************************/
/************************************   3D CMS   *****************************************/
/*****************************************************************************************/




void vDrv3DCMSInit(void)
{
	#if defined(CC_MT5399) //mt5882 not support 3D color mapping
	vDrv3DCMSModuleBypass(SV_OFF);
	vIO32WriteFldMulti(COLOR_MAPPING_CONTROL, P_Fld(SV_TRUE, CM_W1_WGT_EN) | P_Fld(SV_TRUE, CM_W2_WGT_EN) | P_Fld(SV_TRUE, CM_W3_WGT_EN));  
	#else
	printf("not suport 3D Color Mapping!\n");
	#endif
}

void vDrv3DCMSModuleBypass(UINT8 u1Bypass)
{
	#if defined(CC_MT5399) //mt5882 not support 3D color mapping
    vIO32WriteFldAlign(COLOR_MAPPING_CONTROL, u1Bypass, CM_BYPASS);  
	#else
		printf("not suport 3D Color Mapping!\n");
	#endif
}

UINT8 u1Drv3DCMSIsBypass(void)
{
	#if defined(CC_MT5399) //mt5882 not support 3D color mapping
    	return (IO32ReadFldAlign(COLOR_MAPPING_CONTROL, CM_BYPASS));
    #else
    	return SV_TRUE;    
	#endif
}

void vDrv3DCMSWindowOnOff(UINT8 u1OnOff, UINT8 u1WindowID)
{
 	#if defined(CC_MT5399) //mt5882 not support 3D color mapping
    switch (u1WindowID)
    {
        case 0:
            vIO32WriteFldAlign(COLOR_MAPPING_CONTROL, u1OnOff, CM_W1_EN);  
            break;
        case 1:
            vIO32WriteFldAlign(COLOR_MAPPING_CONTROL, u1OnOff, CM_W2_EN);  
            break;
        case 2:
            vIO32WriteFldAlign(COLOR_MAPPING_CONTROL, u1OnOff, CM_W3_EN);  
            break;
        default:
            break;

    }
	#else
		printf("not suport 3D Color Mapping!\n");
	#endif
}

void vDrv3DCMSSetInk(UINT8 u1OnOff, UINT8 u1InkMode)
{
	#if defined(CC_MT5399) //mt5882 not support 3D color mapping
    vIO32WriteFldAlign(DBG_CFG_MAIN, u1OnOff, INK_EN);    

    /*
    YSH pattern gen (after c2p, will overide ink_en pattern)
        0: disable (default)
        1: Hue (horizontal) and Luma (vertical) pattern, Saturation fix to 512
        2: Hue (horizontal) and Saturation (vertical) pattern, Luma fix to 512
    */    
    vIO32WriteFldAlign(DBG_CFG_MAIN, u1InkMode, YSH_PTG_MODE);
	#else
	printf("not suport 3D Color Mapping!\n");
	#endif
}

void vDrv3DCMSSetWeight(UINT8 u1WindowID, UINT8 u1ColorAxis, UINT16 u2WeightLSlope, UINT16 u2WeightUSlope)
{
	#if defined(CC_MT5399) //mt5882 not support 3D color mapping
    /////////////////////////////////////////////////////////////////////    
    /* Axis : (0) Hue (1) Luma (2) Saturation */
    //  weight (Y) = 8192 / (P2 - BOUND_L)
    //  weight (S) = 8192 / ((P2 - BOUND_L) * 4)
    //  weight (H) = 8192 / ((P2 - BOUND_L) * 8)   
    /////////////////////////////////////////////////////////////////////    
    vIO32WriteFldMulti(COLOR_MAPPING_W1_HUE_4 + (u1WindowID * 60) + (u1ColorAxis * 20), 
        P_Fld(u2WeightUSlope, CM_H_W1_WGT_USLOPE) | P_Fld(u2WeightLSlope, CM_H_W1_WGT_LSLOPE)); 
	#else
	printf("not suport 3D Color Mapping!\n");
	#endif
}

void vDrv3DCMSSetCurve(UINT8 u1WindowID, UINT8 u1ColorAxis, CMSCtrlPt rCMSCtrlPt, CMSGain rCMSGain)
{
	#if defined(CC_MT5399) //mt5882 not support 3D color mapping
    /////////////////////////////////////////////////////////////////////    
    // control point : (1) bound_l, bound_u (2) pt_0 ~ pt_4
    // 3D color mapping processing resolution was aligned with SCE processing resolution
    //  Hue : 13 bits 		(range : 0 ~ 8191)
    //  Saturation : 12 bits 	(range : 0 ~ 4095)
    //  Luma : 10 bits 		(range : 0 ~ 1023)
    // Processing window hardware register resolution is 10 bit for H/S/Y, which means when changing 1 code in register, it changes
    //  8 code / 8192 for Hue
    //  4 code / 4096 for Saturation
    //  1 code / 1024 for Luma
    /////////////////////////////////////////////////////////////////////
    vIO32WriteFldMulti(COLOR_MAPPING_W1_HUE_0 + (u1WindowID * 60) + (u1ColorAxis * 20), 
        P_Fld(rCMSCtrlPt.u1CMSBoundL, CM_H_W1_L) | 
        P_Fld(rCMSCtrlPt.u1CMSBoundU, CM_H_W1_U) | 
        P_Fld(rCMSCtrlPt.u1CMSP0, CM_H_W1_POINT0)); 
    
    vIO32WriteFldMulti(COLOR_MAPPING_W1_HUE_1 + (u1WindowID * 60) + (u1ColorAxis * 20), 
        P_Fld(rCMSCtrlPt.u1CMSP1, CM_H_W1_POINT1) | 
        P_Fld(rCMSCtrlPt.u1CMSP2, CM_H_W1_POINT2) | 
        P_Fld(rCMSCtrlPt.u1CMSP3, CM_H_W1_POINT3)); 

    vIO32WriteFldAlign(COLOR_MAPPING_W1_HUE_2 + (u1WindowID * 60) + (u1ColorAxis * 20), 
        rCMSCtrlPt.u1CMSP4, CM_H_W1_POINT4);

    /////////////////////////////////////////////////////////////////////
    // gain control : gain_0 ~ gain_5
    //                0x80 means 1.0, 0xFF means 1.992
    /////////////////////////////////////////////////////////////////////
    vIO32WriteFldMulti(COLOR_MAPPING_W1_HUE_2 + (u1WindowID * 60) + (u1ColorAxis * 20), 
        P_Fld(rCMSGain.u1CMSG0, CM_H_W1_SLOPE0) | 
        P_Fld(rCMSGain.u1CMSG1, CM_H_W1_SLOPE1));     

    vIO32WriteFldMulti(COLOR_MAPPING_W1_HUE_3 + (u1WindowID * 60) + (u1ColorAxis * 20), 
        P_Fld(rCMSGain.u1CMSG2, CM_H_W1_SLOPE2) | 
        P_Fld(rCMSGain.u1CMSG3, CM_H_W1_SLOPE3) | 
        P_Fld(rCMSGain.u1CMSG4, CM_H_W1_SLOPE4) | 
        P_Fld(rCMSGain.u1CMSG5, CM_H_W1_SLOPE5)); 
	#else
	printf("not suport 3D Color Mapping!\n");
	#endif
}

void vDrv3DCMSGetBound(UINT8 u1WindowID, UINT8 u1ColorAxis, UINT8 *u1CMSBoundL, UINT8 *u1CMSBoundU)
{
	#if defined(CC_MT5399) //mt5882 not support 3D color mapping
    *u1CMSBoundL = IO32ReadFldAlign(COLOR_MAPPING_W1_HUE_0 + (u1WindowID * 60) + (u1ColorAxis * 20), CM_H_W1_L);
    *u1CMSBoundU = IO32ReadFldAlign(COLOR_MAPPING_W1_HUE_0 + (u1WindowID * 60) + (u1ColorAxis * 20), CM_H_W1_U); 
	#else
	printf("not suport 3D Color Mapping!\n");
	#endif
}


