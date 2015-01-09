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
 */
 /*------------------------------------------------------------------------------------------------ 
 * Owner: menghu.lin
 * 
 * Description: 
 * This file contains Luma/Chroma processing functions. 
 *-----------------------------------------------------------------------------------------------
 */ 

// === INCLUDE =============================================================================
#include "x_os.h"	
#include "x_rand.h"
#include "sv_const.h"
#include "hw_vdoin.h"
#include "vdo_misc.h"
#include "nptv_debug.h"
#include "video_def.h"
#include "drv_scaler.h"
#if defined (CC_MT5399) || defined (CC_MT5882)
#include "drv_lcdim_v2.h"
#else
#include "drv_lcdim.h"
#endif
#include "drv_od.h"
#include "drv_meter.h"
#include "drv_pqstress.h"
#include "x_assert.h"
#include "b2r_drvif.h"
#include "drv_upscaler.h"
#include "drv_predown.h"
#include "drv_scaler_drvif.h"
#include "drv_di.h"
#include "drv_contrast.h"

// === HW INCLUDE =============================================================================
#include "hw_ycproc.h"
#include "hw_sw.h"
#include "hw_ospe.h"
#include "hw_scpos.h"
#include "drv_tdtv_drvif.h"
#include "hw_od.h"
#include "drv_scaler_gfx.h"
// === DEFINE =============================================================================
#define  vDrvPoclkSetClock(x)      (vIO32WriteFldAlign(OSTG_00, x, R_POST_POCLK_EN))
#define  vDrvOclkSetClock(x)      (vIO32WriteFldAlign(OSTG_00, x, R_POST_OCLK_EN))

#define REG_BUFFER_NUM      0x100
// === FUNCTION PROTOTYPE =================================================================

// === EXTERN FUNCTION ====================================================================
extern void vDrvSharpProc(void);
extern void vDrvADLProc(void);
extern void vDrvADLProcMainLoop(void);
extern void vDrvGammaProc(void);
extern void vDrvDIForce3DModeWithDelay(void);
extern void vDrvNRSet2DNRDemo(UINT8 bOnOff, UINT8 bRegion, UINT16 wXCenter);
extern void vDrvNRSet3DNRDemo(UINT8 bOnOff, UINT8 bRegion, UINT16 wXCenter);
// === GLOBAL VARIABLE ====================================================================
MMAppInfo sMMAppInfo; 
UINT8 u1SupportHdmiYCbCr444 = SV_FALSE;
UINT8 u1SupportVgaYCbCr444 = SV_FALSE;
#ifdef CC_SUPPORT_PREPROC_TIMING
typedef struct
{
    UINT8 bTiming;
    UINT32 u4HTotal;
   	UINT32 u4VTotal;
   	UINT32 u4HActive;
   	UINT32 u4VActive;
	UINT8 u1FieldEn;
	UINT8 bRefreshRate;
	BOOL IsInterlace;
} PRE_PROC_GEN_T;

PRE_PROC_GEN_T pre_proc_gen_tim_tbl[]=
{
   {MODE_480P, 858, 525, 720, 480, 0, 60, 0}, //T_480P,
   {MODE_525I, 858, 262, 720, 240, 1, 60, 1}, //T_525I,
   {MODE_576P, 858, 625, 720, 576, 0, 50, 0}, //T_576P,
   {MODE_625I, 858, 312, 720, 288, 1, 50, 1}, //T_625I,
   {MODE_720p_60, 1650, 750, 1280, 720, 0, 60, 0}, //T_720P_60,
   {MODE_1080p_60, 2200, 1125, 1920, 1080, 0, 60, 0}, //T_1080P_60,
   {MODE_1080i, 2200, 563, 1920, 540, 1, 60, 1}, //T_1080I,
   {MODE_DE_MODE, 1184, 790, 1024, 768, 0, 60, 0}, //1024*768,
   {MODE_4096_2160P_24, 4400, 2250, 4096, 2160, 0, 24, 0}, //4096*2160,
};

UINT8 _bPreProcTiming;
UINT8 _bPreProcRefreshRate;
BOOL _fgPreProcInterlace;
#endif
// === EXTERN VARIABLE ====================================================================
extern UINT16 u2PQItemMatrix[SV_VP_NA][PQ_MAX];
extern UINT16 u2ExtAdjMatrix[SV_VP_NA][PQ_MAX];
extern UINT16 u2Ext2AdjMatrix[SV_VP_NA][PQ_MAX];  //--For fade in/fade out function
extern UINT8 bSceGain[3];
extern UINT8 bSkinTable[3];
extern UINT8 bSce3dGain[2];
extern UINT8 _bMAIN_COLOR_SYS;
extern UINT8 _bPIP_COLOR_SYS;
extern UINT8 DrvODOvfUdfFlag;
// === STATIC VARIABLE ====================================================================
static Region DemoRec; //, b4ScalerDemoRec;
static INT8 bHMove=1, bVMove=1;
static REGTBL_T aRegisterBuffer[REG_BUFFER_NUM];
static UINT32 u4RegBufIndex = 0;

// === BODY ===============================================================================
UINT8 bDrvCheckPqReg(UINT32* pu4Table, UINT32 u4Size)
{
    UINT32 i, u4RegCheck = 0, u4MskCheck = 0;
    UINT32 uCnt = u4Size/12-1;

    for (i=0; i<uCnt; i++)
    {
        u4RegCheck += (pu4Table[i*3]>>(i&0xF));
        u4MskCheck += (pu4Table[i*3+2]>>(i&0xF));   
    }

    if ((u4RegCheck == pu4Table[i*3]) && (u4MskCheck == pu4Table[i*3+2]))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

void vDrvSetPqReg(UINT32* pu4Table, UINT32 u4Size)
{
    UINT32 i;
    UINT32 uCnt = u4Size/12-1;

    for (i=0; i<uCnt; i++)
    {
        if (pu4Table[i*3] < VDOIN_BASE)
        {
            vIO32Write4BMsk(u4SWReg(pu4Table[i*3]), pu4Table[i*3+1], pu4Table[i*3+2]);
        }
        else
        {
        	vIO32Write4BMsk(pu4Table[i*3], pu4Table[i*3+1], pu4Table[i*3+2]);
    	}
    }
    
    return;
}

void vDrvGetPqReg(UINT32* pu4Table, UINT32 u4Size)
{
    UINT32 i;
    UINT32 uCnt = u4Size/12-1;

    for (i=0; i<uCnt; i++)
    {
        if (pu4Table[i*3] < VDOIN_BASE)
        {
            pu4Table[i*3+1] = u4IO32Read4B(u4SWReg(pu4Table[i*3]))&pu4Table[i*3+2];
        }
        else
        {
        	pu4Table[i*3+1] = u4IO32Read4B(pu4Table[i*3])&pu4Table[i*3+2];
    	}
    }
    
    return;
}

void vDrvWritePostRegBuffer(UINT32 u4Addr, UINT32 u4Value, UINT32 u4Mask)
{    
    static UINT32 u4OverCnt = 0;

    CRIT_STATE_T csState;
    
    csState = x_crit_start();    

    if (u4RegBufIndex < REG_BUFFER_NUM)
    {
        aRegisterBuffer[u4RegBufIndex].u4Reg = u4Addr;
        aRegisterBuffer[u4RegBufIndex].u4Value = u4Value;
        aRegisterBuffer[u4RegBufIndex].u4Mask = u4Mask;
        u4RegBufIndex++;
        u4OverCnt = 0;
    }    
    else
    {
        u4OverCnt++;
        LOG(0, "Register buffer overflow %d \n", u4OverCnt);
    }
    
    vApiRegisterVideoEvent(PE_EVENT_FLUSH_POST_REG, SV_VP_MAIN, SV_ON);
    
    x_crit_end(csState);
}

void vDrvFlushPostRegBuffer(void)
{    
    UINT32 i;

    for (i=0; i < u4RegBufIndex; i++)
    {
        vIO32WriteFldAlign(aRegisterBuffer[i].u4Reg, aRegisterBuffer[i].u4Value, aRegisterBuffer[i].u4Mask);        
        LOG(2, "Flush Register      0x%08x  0x%08x  0x%08x \n", 
            aRegisterBuffer[i].u4Reg, aRegisterBuffer[i].u4Value, aRegisterBuffer[i].u4Mask);
    } 

    u4RegBufIndex = 0;
}

void vDrvPreprocHWInit(void)
{
    vIO32WriteFldAlign(PAT_PIP_00,0,C_MAIN_SELFGEN_EN);
    vIO32WriteFldAlign(TRANS_PIP_00,0x600, IN_OFFSET_1P);
    vIO32WriteFldAlign(TRANS_MAIN_07, 0, CONTROL_SEL);
    vIO32WriteFldAlign(TRANS_MAIN_07, 6, INPUT_SELP);
    vIO32WriteFldAlign(TRANS_PIP_07, 0, CONTROL_SEL);
    vIO32WriteFldAlign(TRANS_PIP_07, 6, INPUT_SELP);
    vIO32WriteFldAlign(YCBCR2YC_MAIN_00, 0x0, C_MAIN_HSYNC_BLACK);
    vIO32WriteFldAlign(YCBCR2YC_PIP_00, 0x0, C_MAIN_HSYNC_BLACK);
    
#if defined (CC_MT5399) || defined (CC_MT5882)
    vDrvPreSharpInit();
    vDrvCTIFInit();
#endif
}

void vDrvPreprocSWInit(void)
{
    u1SupportHdmiYCbCr444 = DRVCUST_OptGet(eSupportHdmiYCbCr444);
    u1SupportVgaYCbCr444 = DRVCUST_OptGet(eSupportVgaYCbCr444);
    vIO32WriteFldAlign(MATRIX_00, 0x0, IN_CSC_BLACK_LVL_M);
    vIO32WriteFldAlign(MATRIX_00, 0x0, IN_CSC_BLACK_LVL_P);
    vIO32WriteFldAlign(MATRIX_00, DRVCUST_OptGet(eMatrixHDYCbCr709), MATRIX_HD_709);
    vIO32WriteFldAlign(MATRIX_01, 0x200, IN_Y_GAIN);
    vIO32WriteFldAlign(MATRIX_01, 0x200, IN_Y_OFST);
    vIO32WriteFldAlign(MATRIX_01, 0x200, IN_C_GAIN);
    vIO32WriteFldAlign(MATRIX_03, u1SupportHdmiYCbCr444, YCBCR444_HDMI);
    vIO32WriteFldAlign(MATRIX_03, u1SupportVgaYCbCr444, YCBCR444_VGA);
}

void vDrvPreprocInit(void)
{
    vDrvPreprocHWInit();
    vDrvPreprocSWInit();
}

void vDrvOstgHWInit(void)
{
    vDrvxvYCCSetClip(DRVCUST_OptGet(eFlagxvYCCClip));

    // 1. loader logo not exist, option sel will be set during driver init
    // 2. fast boot condition
    if((DRVCUST_InitGet(eLoaderLogoNumber)==0) || (DRVCUST_OptGet(eOSOptionPos) != IO32ReadFldAlign(OSTG_01, REG_OP_SEL)))
    {
        vDrvSetOstgOptPos(DRVCUST_OptGet(eOSOptionPos));
    }
    
    vIO32WriteFldAlign(YCBCR2RGB_00, 0x0, YUV2RGB_BYPASS_MAIN);
    vIO32WriteFldAlign(YCBCR2RGB_00, 0x0, YUV2RGB_BYPASS_PIP);
    vIO32WriteFldAlign(GAINOSET_00, 0x0, RGB_GAIN_SEL);      // Select RGB gain range 0~2
    vIO32WriteFldAlign(GAMMA_00, 0, C_GM_SWAP_POST);         // default use pre-gamma (before local dimming)
    vIO32WriteFldAlign(OSTG_00, 1, C_DBUF_POCLK_ON);         // 5398 enable OSTG double buffer
    vIO32WriteFldAlign(OSTG_00, 1, C_DBUF_OCLK_ON);         // 5398 enable OSTG double buffer    
//    vDrvGammaOnOff(SV_ON);    // gamma ON step need to be done after SRAM initialized
    vDrvDBLHWInit();
}

void vDrvOstgSWInit(void)
{
    vDrvDBLSWInit();

    // Initial color filter value
    vIO32WriteFldAlign(MATRIX_02, 0x200, COLOR_FILTER_R_GAIN);
    vIO32WriteFldAlign(MATRIX_02, 0x200, COLOR_FILTER_G_GAIN);
    vIO32WriteFldAlign(MATRIX_02, 0x200, COLOR_FILTER_B_GAIN);
    
    vIO32WriteFldMulti(GLOBAL_ADJ_04, P_Fld(0x80, EXT_ADJ_CONT) | P_Fld(0x80, EXT_ADJ_BRI) | 
                        P_Fld(0x80, EXT_ADJ_SAT) | P_Fld(0x80, EXT_ADJ_HUE));        
    vIO32WriteFldMulti(GLOBAL_ADJ_05, P_Fld(0x80, EXT_ADJ_CONT) | P_Fld(0x80, EXT_ADJ_BRI) | 
                        P_Fld(0x80, EXT_ADJ_SAT) | P_Fld(0x80, EXT_ADJ_HUE));

    //--For fade in/fade out function
    vIO32WriteFldMulti(GLOBAL_ADJ_06, P_Fld(0x0, EXT2_ADJ_CONT) 
                                | P_Fld(0x200, EXT2_ADJ_BRI));
    vIO32WriteFldMulti(GLOBAL_ADJ_07, P_Fld(0x0, EXT2_ADJ_CONT) 
                                | P_Fld(0x200, EXT2_ADJ_BRI));        

    // Initial OS 3x3 matrix
    vDrvCalVideoColorMatrix(SV_VP_MAIN, 0x80, 0x80, 0x80, 0x80);
    vDrvCalVideoColorMatrix(SV_VP_PIP, 0x80, 0x80, 0x80, 0x80);
}

void vDrvGlobalPQSWInit(void)
{
    // initial PQ setting
    vDrvSetBrightness(SV_VP_MAIN, 0x80);
    vDrvSetContrast(SV_VP_MAIN, 0x80);
    vDrvSetSaturation(SV_VP_MAIN, 0x80);
    vDrvSetHue(SV_VP_MAIN, 0x80);
    vDrvSetBrightness(SV_VP_PIP, 0x80);
    vDrvSetContrast(SV_VP_PIP, 0x80);
    vDrvSetSaturation(SV_VP_PIP, 0x80);
    vDrvSetHue(SV_VP_PIP, 0x80);
}

void vDrvOstgInit(void)
{
    vDrvOstgHWInit();
    vDrvOstgSWInit();
    vDrvGlobalPQSWInit();
}

void vDrvPreprocResume(void)
{
    vDrvPreprocHWInit();
}

void vDrvOSTGReset(UINT8 bOnOff)
{
    vIO32WriteFldAlign(OSTG_00, bOnOff, CLK2_RST_CTRL);
    vIO32WriteFldAlign(OSTG_00, bOnOff, CLK_RST_CTRL);
    vIO32WriteFldAlign(OSTG_00, bOnOff, REG_RST_CTRL);
}

void vDrvPostprocSuspend(void)
{
    vDrvPoclkSetClock(SV_OFF);
    vDrvOclkSetClock(SV_OFF);
}

void vDrvPostprocResume(void)
{
    vDrvPoclkSetClock(SV_ON);
    vDrvOclkSetClock(SV_ON);
    //fix PP resume OSD offset wrong menghu 0626
//    vDrvOSTGReset(SV_ON);
//    vDrvOSTGReset(SV_OFF);

    vDrvPostSharpInit();
    vDrvCTIRInit();   
    vDrvSCEHWInit();
    vDrvADLInit();
    vDrvOstgHWInit();
    vDrvGammaInitSRAM();   
    vDrvMeterInit();
#if defined (CC_MT5399)
    vDrvCBEInit();
#endif

}

void vDrvYcprocInit(void)
{
    //Pre-Proc initialize
    vDrvPreprocInit();

    //Post-Proc initialize
    vDrvPostSharpInit();
    vDrvCTIFInit();
    vDrvCTIRInit();    
    vDrvSCEInit();
    vDrvADLInit();

    // Output-Stage initialize
    vDrvOstgInit();
    vDrvMeterInit();
#if defined (CC_MT5399)

	vDrvCBEInit();
#endif

}

// ============================================================================
/**
 *  @brief Set filed signal resync with vsync when input is interlace.
 *  @param bPath SV_VP_MAIN/SV_VP_PIP
 */
void vDrvPreprocSetFieldResync(UINT8 bPath)
{
	UINT8 bFieldResync;

	if ((bGetVideoDecType(bPath) == SV_VD_MPEGHD) ||
	        (bDrvVideoIsSrcInterlace(bPath) == SV_FALSE)) // DTV or progressive timing, disable field prediction
	{
		bFieldResync = SV_FALSE;
	}
	else
	{
		bFieldResync = SV_TRUE;
	}

	LOG(3,"Set Field Resync to (%d)\n",bFieldResync);

	if (bPath == SV_VP_MAIN)
	{
        // TODO
		//vIO32WriteFldAlign(PAT_MAIN_00, bFieldResync, C_MAIN_F_V_SHK);    //Field resync by Vsync when vsize is reduced.
	}
	else
	{
        // TODO
        //vIO32WriteFldAlign(PAT_PIP_00, bFieldResync, C_PIP_F_V_SHK);    //Field resync by Vsync when vsize is reduced.
	}
}
#ifdef CC_SUPPORT_PREPROC_TIMING

void vDrvSetPreProcTiming(UINT8 bPath,PRE_PROC_TIMING mTiming)
{
    UINT32 u4Ration_N = 1;
	UINT32 u4Ration_M = 25;
	UINT32 u4HTotal,u4VTotal,u4HActive,u4VActive;
	UINT8 u1FieldEn = 0;
	UINT32 u4Clock;
	
	_bPreProcTiming = pre_proc_gen_tim_tbl[mTiming].bTiming;
	u4HTotal = pre_proc_gen_tim_tbl[mTiming].u4HTotal;
	u4VTotal = pre_proc_gen_tim_tbl[mTiming].u4VTotal;
	u4HActive = pre_proc_gen_tim_tbl[mTiming].u4HActive;
	u4VActive = pre_proc_gen_tim_tbl[mTiming].u4VActive;
	u1FieldEn = pre_proc_gen_tim_tbl[mTiming].u1FieldEn;
	_bPreProcRefreshRate = pre_proc_gen_tim_tbl[mTiming].bRefreshRate;
	_fgPreProcInterlace =pre_proc_gen_tim_tbl[mTiming].IsInterlace;
	

    u4Clock = BSP_GetClock(SRC_VDOIN_MPCLK, NULL, NULL, NULL, NULL);
	u4Ration_N = DIVIDE_WITH_ROUND((u4HTotal*u4VTotal*_bPreProcRefreshRate/1000)*u4Ration_M ,u4Clock/1000);
	if (bPath == SV_VP_MAIN)
	{
	    vIO32WriteFldAlign(PAT_MAIN_00,u1FieldEn,C_MAIN_SELF_FLD_EN);
		vIO32WriteFldAlign(PAT_MAIN_00,u4VTotal,C_MAIN_SELF_VT);
		vIO32WriteFldAlign(PAT_MAIN_00,u4HTotal,C_MAIN_SELF_HT);
		vIO32WriteFldAlign(PAT_MAIN_01,u4VActive,C_MAIN_V_ACT);
		vIO32WriteFldAlign(PAT_MAIN_01,u4HActive,C_MAIN_H_ACT);
		vIO32WriteFldAlign(PAT_MAIN_02,u4Ration_N,C_MAIN_SELF_DE_N);
	}
	else
	{
	    vIO32WriteFldAlign(PAT_PIP_00,u1FieldEn,C_MAIN_SELF_FLD_EN);
		vIO32WriteFldAlign(PAT_PIP_00,u4VTotal,C_MAIN_SELF_VT);
		vIO32WriteFldAlign(PAT_PIP_00,u4HTotal,C_MAIN_SELF_HT);
		vIO32WriteFldAlign(PAT_PIP_01,u4VActive,C_MAIN_V_ACT);
		vIO32WriteFldAlign(PAT_PIP_01,u4HActive,C_MAIN_H_ACT);
		vIO32WriteFldAlign(PAT_PIP_02,u4Ration_N,C_MAIN_SELF_DE_N);
	}

	if (bPath == SV_VP_MAIN)
	{
		vSetMainFlg(MAIN_FLG_MODE_CHG);
		vSetMainFlg(MAIN_FLG_MODE_DET_DONE);
	}
	else
	{
		vSetPipFlg(PIP_FLG_MODE_CHG);
		vSetPipFlg(PIP_FLG_MODE_DET_DONE);
	}
}

UINT8 bPreProcRefreshRate(void)
{
    return _bPreProcRefreshRate;
}



UINT16 u2PreProcHTotal(UINT8 bPath)
{
    UINT16 u2Htotal;
    if (bPath == SV_VP_MAIN)
	{
	
	    u2Htotal = IO32ReadFldAlign(PAT_MAIN_00, C_MAIN_SELF_HT);
	}
	else
	{
	    u2Htotal = IO32ReadFldAlign(PAT_PIP_00, C_MAIN_SELF_HT);
	}
	return u2Htotal;
}
UINT16 u2PreProcVTotal(UINT8 bPath)
{
    UINT16 u2Vtotal;
    if (bPath == SV_VP_MAIN)
	{
	    u2Vtotal = IO32ReadFldAlign(PAT_MAIN_00, C_MAIN_SELF_VT);
		
	}
	else
	{
	    u2Vtotal = IO32ReadFldAlign(PAT_PIP_00, C_MAIN_SELF_VT);
	}
	if(bDrvVideoIsSrcInterlace(bPath) == SV_TRUE)
	{
	    u2Vtotal = u2Vtotal *2 +1;
	}
	return u2Vtotal;
}
UINT16 u2PreProcInputWidth(UINT8 bPath)
{
    UINT16 u2Width;
    if (bPath == SV_VP_MAIN)
	{
	    u2Width = IO32ReadFldAlign(PAT_MAIN_01, C_MAIN_H_ACT);
	}
	else
	{
	    u2Width = IO32ReadFldAlign(PAT_PIP_01, C_MAIN_H_ACT);
	}
	return u2Width;
}

UINT16 u2PreProcInputHeight(UINT8 bPath)
{
    UINT16 u2Height;
    if (bPath == SV_VP_MAIN)
	{
	    u2Height = IO32ReadFldAlign(PAT_MAIN_01, C_MAIN_V_ACT);
	}
	else
	{
	    u2Height = IO32ReadFldAlign(PAT_PIP_01, C_MAIN_V_ACT);
	}
	if(bDrvVideoIsSrcInterlace(bPath) == SV_TRUE)
	{
	    u2Height = u2Height *2 ;
	}
	return u2Height;
}


void vDrvEnablePreprocSelfGen(UINT8 bPath,UINT8 bOnOff)
{
    if (bPath == SV_VP_MAIN)
    {
        if(bOnOff)
    	{
    	    
		    vIO32WriteFldAlign(PAT_MAIN_00,1,C_MAIN_SELFGEN_EN);
		 }
		else
		{
		    vIO32WriteFldAlign(PAT_MAIN_00,0,C_MAIN_SELFGEN_EN);
		 }
    }
	else
	{
	    if(bOnOff)
    	{
		    vIO32WriteFldAlign(PAT_PIP_00,1,C_MAIN_SELFGEN_EN);
	    }
		else
		{
		    vIO32WriteFldAlign(PAT_PIP_00,0,C_MAIN_SELFGEN_EN);
	    }
	}
}
void vDrvSetOmuxTimToPreProc(UINT8 bPath,UINT8 bOnOff)
{
    UINT32 u4Ration_N = 1;
	UINT32 u4Ration_M = 25;
	UINT32 u4HTotal,u4VTotal,u4HActive,u4VActive;
	UINT8 u1FieldEn = 0;
	UINT32 u4Clock;

	_bPreProcTiming = bDrvVideoGetTiming(bPath);
	u4HTotal = wDrvVideoGetHTotal(bPath);
	u4VTotal = wDrvVideoGetVTotal(bPath);
	u4HActive = wDrvVideoInputWidth(bPath);
	u4VActive = wDrvVideoInputHeight(bPath);
	_fgPreProcInterlace = bDrvVideoIsSrcInterlace(bPath)?1 : 0;
	u1FieldEn = _fgPreProcInterlace;
	_bPreProcRefreshRate = bDrvVideoGetRefreshRate(bPath);

    u4Clock = BSP_GetClock(SRC_VDOIN_MPCLK, NULL, NULL, NULL, NULL);
	u4Ration_N = DIVIDE_WITH_ROUND((u4HTotal*u4VTotal*_bPreProcRefreshRate/1000)*u4Ration_M ,u4Clock/1000);
	if(bOnOff && (bDrvVideoSignalStatus(bPath) == (UINT8)SV_VDO_STABLE))
	{
	    vDrvEnablePreprocSelfGen(bPath,SV_ON);
	    
	    if (bPath == SV_VP_MAIN)
		{
		    vIO32WriteFldAlign(PAT_MAIN_00,u1FieldEn,C_MAIN_SELF_FLD_EN);
			vIO32WriteFldAlign(PAT_MAIN_00,u4VTotal,C_MAIN_SELF_VT);
			vIO32WriteFldAlign(PAT_MAIN_00,u4HTotal,C_MAIN_SELF_HT);
			vIO32WriteFldAlign(PAT_MAIN_01,u4VActive,C_MAIN_V_ACT);
			vIO32WriteFldAlign(PAT_MAIN_01,u4HActive,C_MAIN_H_ACT);
			vIO32WriteFldAlign(PAT_MAIN_02,u4Ration_N,C_MAIN_SELF_DE_N);
		}
		else
		{
		    vIO32WriteFldAlign(PAT_PIP_00,u1FieldEn,C_MAIN_SELF_FLD_EN);
			vIO32WriteFldAlign(PAT_PIP_00,u4VTotal,C_MAIN_SELF_VT);
			vIO32WriteFldAlign(PAT_PIP_00,u4HTotal,C_MAIN_SELF_HT);
			vIO32WriteFldAlign(PAT_PIP_01,u4VActive,C_MAIN_V_ACT);
			vIO32WriteFldAlign(PAT_PIP_01,u4HActive,C_MAIN_H_ACT);
			vIO32WriteFldAlign(PAT_PIP_02,u4Ration_N,C_MAIN_SELF_DE_N);
		}
	}
	else
	{
	    vDrvEnablePreprocSelfGen(bPath,SV_OFF);
	}
	
}


#endif

/*****************************************************************************************/
/**********************************   DEMO MODE   ****************************************/
/*****************************************************************************************/
void vDrvDemoSetRegion(Region rDemoRegion)
{
    if((rDemoRegion.wHEnd<=rDemoRegion.wHStart)||(rDemoRegion.wVEnd<=rDemoRegion.wVStart))
    {
        printf("Invalid Demo Width or Demo Height Set !!\n");
    }
    vIO32WriteFldAlign(DEMO_00, rDemoRegion.wHStart, DEMOMODE_HSTART);
    vIO32WriteFldAlign(DEMO_00, rDemoRegion.wVStart, DEMOMODE_VSTART);
    vIO32WriteFldAlign(DEMO_01, (rDemoRegion.wHEnd-rDemoRegion.wHStart), DEMOMODE_WIDTH);
    vIO32WriteFldAlign(DEMO_01, (rDemoRegion.wVEnd-rDemoRegion.wVStart), DEMOMODE_HEIGHT); 
    vIO32WriteFldAlign(DEMO_00, rDemoRegion.u1OnOff, DEMOMODE_ENABLE);
}

void vDrvGetb4ScalerRec(void)
{
    #if 0  
    RDispPrm* dispPrm;    
    UINT16 wHPorch, wVPorch;
    UINT32 u4PreDownScale = u4PDSGetHPSFactorReg(SV_VP_MAIN);
    dispPrm = getDispPrm(SV_VP_MAIN); 
    
    if((dispPrm==NULL) || 
       (DemoRec.wHEnd<DemoRec.wHStart) || 
       (DemoRec.wVEnd<DemoRec.wVStart))
    {
        printf("Error parameters of demo region ! \n");
	}

    ASSERT(dispPrm->u4DSclH && dispPrm->u4DSclV);

    //In display mode, overscan is clipped by Regen, others by Scaler.
    wHPorch = (u1ScpipGetDispMode(SV_VP_MAIN)) ? 0 : getMixedHPorch(SV_VP_MAIN);
    wVPorch = (u1ScpipGetDispMode(SV_VP_MAIN)) ? 0 : getMixedVPorch(SV_VP_MAIN);
     
    //Transform demo region in panel size base to demo region in input image size base.

    b4ScalerDemoRec.wHStart = (UINT32)DemoRec.wHStart*dispPrm->u4USclH/0x8000*
                               0x8000/dispPrm->u4DSclH + wHPorch*u4PreDownScale/0x8000;
    b4ScalerDemoRec.wVStart = (UINT32)DemoRec.wVStart*dispPrm->u4USclV/0x8000*
                               0x8000/dispPrm->u4DSclV + wVPorch;
    b4ScalerDemoRec.wHEnd = b4ScalerDemoRec.wHStart + (DemoRec.wHEnd-DemoRec.wHStart)*
                               dispPrm->u4USclH/0x8000*0x8000/dispPrm->u4DSclH;
    b4ScalerDemoRec.wVEnd = b4ScalerDemoRec.wVStart + (DemoRec.wVEnd-DemoRec.wVStart)*
                               dispPrm->u4USclV/0x8000*0x8000/dispPrm->u4DSclV;
    #endif
}

void vDrvDemoRecValidation(void)
{
    UINT16 wWidth = u2ScpipGetDispWidth(SV_VP_MAIN);
    UINT16 wHeight = u2ScpipGetDispHeight(SV_VP_MAIN);

    // region Validation.
    if(IO32ReadFldAlign(DEMO_01, DEMOMODE_WIDTH)>wWidth)
    {
        vIO32WriteFldAlign(DEMO_01, wWidth, DEMOMODE_WIDTH);
    }
    if(IO32ReadFldAlign(DEMO_01, DEMOMODE_HEIGHT)>wHeight)
    {
        vIO32WriteFldAlign(DEMO_01, wHeight, DEMOMODE_HEIGHT);
    }
    if(DemoRec.wHEnd>=wWidth)
    {
        DemoRec.wHStart = wWidth-IO32ReadFldAlign(DEMO_01, DEMOMODE_WIDTH);
        DemoRec.wHEnd = wWidth;
    }
    if(DemoRec.wVEnd>=wHeight)
    {
        DemoRec.wVStart = wHeight-IO32ReadFldAlign(DEMO_01, DEMOMODE_HEIGHT);
        DemoRec.wVEnd = wHeight;
    }    
}

void vDrvDemoRecMoving(void)
{
    UINT16 wWidth = u2ScpipGetDispWidth(SV_VP_MAIN);
    UINT16 wHeight = u2ScpipGetDispHeight(SV_VP_MAIN);
    
    bHMove = (DemoRec.wHStart==0) ? 1 : (DemoRec.wHEnd==wWidth) ? -1 : bHMove;
    bVMove = (DemoRec.wVStart==0) ? 1 : (DemoRec.wVEnd==wHeight) ? -1 : bVMove;
    vIO32WriteFldAlign(DEMO_00, DemoRec.wHStart+bHMove, DEMOMODE_HSTART);
    vIO32WriteFldAlign(DEMO_00, DemoRec.wVStart+bVMove, DEMOMODE_VSTART);
}

void vDrvDemoProc(void)
{
    UINT8 bOnOff = IO32ReadFldAlign(DEMO_00, DEMOMODE_ENABLE);
    UINT8 bSplitOnOff = IO32ReadFldAlign(DEMO_00, SPLIT_ONOFF);    
    UINT8 bRegion = IO32ReadFldAlign(DEMO_00, REGION_LEFT_RIGHT);
    UINT8 bMovingOnOff = IO32ReadFldAlign(DEMO_00, MOVINGEMO_ENABLE);
    static UINT8 bDEMOModules = 0;
    static UINT8 btiming = 0xFF;
    static UINT8 u1FlipModule = 0xFF;
    static UINT16 u2DIWidth = 0xFFFF;
	static UINT16 u2SCWidth = 0xFFFF;
    static UINT8 bpreOnOff = SV_OFF;
    UINT8 bDemoToggle = (bOnOff != bpreOnOff) ? SV_ON : SV_OFF;

    bpreOnOff = bOnOff;

    if((bOnOff == SV_OFF)&&(bDemoToggle == SV_OFF))
        return;
    
    if((u2DIWidth !=u2DrvDIGetWidth())||(u1FlipModule != (UINT8)u4QueryFlipModule(SV_VP_MAIN))||
        (btiming != bDrvVideoGetSourceTypeTiming(VDP_1))||(u2SCWidth !=vScpipGetInputWidth(SV_VP_MAIN))||
        (DemoRec.u1OnOff!=bOnOff)|| (bDEMOModules != IO32ReadFldAlign(DEMO_02, DEMO_MODULES))||
        (DemoRec.wHStart!=IO32ReadFldAlign(DEMO_00, DEMOMODE_HSTART))||
        (DemoRec.wVStart!=IO32ReadFldAlign(DEMO_00, DEMOMODE_VSTART))||
        (DemoRec.wHEnd!=(IO32ReadFldAlign(DEMO_00, DEMOMODE_HSTART)+IO32ReadFldAlign(DEMO_01, DEMOMODE_WIDTH)))||
        (DemoRec.wVEnd!=(IO32ReadFldAlign(DEMO_00, DEMOMODE_VSTART)+IO32ReadFldAlign(DEMO_01, DEMOMODE_HEIGHT))))
    {
		u2DIWidth = u2DrvDIGetWidth(); 
		u2SCWidth = vScpipGetInputWidth(SV_VP_MAIN); 
        u1FlipModule = (UINT8)u4QueryFlipModule(SV_VP_MAIN);
        btiming = bDrvVideoGetSourceTypeTiming(VDP_1);
        bDEMOModules = IO32ReadFldAlign(DEMO_02, DEMO_MODULES);
        DemoRec.u1OnOff = bOnOff;
        DemoRec.wHStart = IO32ReadFldAlign(DEMO_00, DEMOMODE_HSTART);
        DemoRec.wVStart = IO32ReadFldAlign(DEMO_00, DEMOMODE_VSTART);
        DemoRec.wHEnd = IO32ReadFldAlign(DEMO_00, DEMOMODE_HSTART)+IO32ReadFldAlign(DEMO_01, DEMOMODE_WIDTH);
        DemoRec.wVEnd = IO32ReadFldAlign(DEMO_00, DEMOMODE_VSTART)+IO32ReadFldAlign(DEMO_01, DEMOMODE_HEIGHT);
        vDrvDemoRecValidation();
        vDrvGetb4ScalerRec();

        //Split demo
        //vDrvDIDemo(bSplitOnOff&&(IO32ReadFldAlign(DEMO_02, DEMO_MDDI_EN)), bRegion,(u2DIWidth/2)+0x44);   // 0x44 is for MEMA Demo position alignment. why 0x44? just test the position, and got this difference value.
        vDrvNRSet2DNRDemo(bSplitOnOff&&(IO32ReadFldAlign(DEMO_02, DEMO_2DNR_EN)), bRegion, ((u2DIWidth+1)/2));
        vDrvNRSet3DNRDemo(bSplitOnOff&&(IO32ReadFldAlign(DEMO_02, DEMO_3DNR_EN)), bRegion, ((u2DIWidth+1)/2));
		vDrvHSharpDemo(bSplitOnOff&&(IO32ReadFldAlign(DEMO_02, DEMO_HSHARP_EN)), bRegion, ((u2SCWidth+1)/2));

        //Rectangular demo
        //vDrv3DNRDemo(bOnOff, b4ScalerDemoRec);
        vDrvSetPostSharpDemoRange(DemoRec);    
        vDrv2DSharpDemo(bOnOff&&(IO32ReadFldAlign(DEMO_02, DEMO_TDSHARP_EN)), SV_ON);
        vDrvECTIDemo(bOnOff&&(IO32ReadFldAlign(DEMO_02, DEMO_ECTI_EN)), SV_ON);          

		vDrvSetColorDemoRange(DemoRec);
        vDrvColorDemo(bOnOff&&(IO32ReadFldAlign(DEMO_02, DEMO_COLOR_EN)), SV_ON);
    }
    
    if(bMovingOnOff==SV_ON)
    {    
        vDrvDemoRecMoving();
    }    
}

/*****************************************************************************************/
/*************************************** General  *****************************************/
/*****************************************************************************************/
UINT8 bDrvGetPeUiDebugMode(void)
{
    return IO32ReadFldAlign(PEUI_00, PEUI_DEBUG_EN);
}

void vDrvSetPeUiDebugMode(UINT8 bOnOff)
{
    vIO32WriteFldAlign(PEUI_00, bOnOff, PEUI_DEBUG_EN);
}

void vDrvPreprocEnablePattern(UINT8 bPath,UINT8 bPattern)
{
    vIO32WriteFldAlign(PAT_MAIN_04,   0x3   , C_MAIN_HV_MODE);
    vIO32WriteFldAlign(PAT_MAIN_04, 0x300   , C_MAIN_FC_Y);
    vIO32WriteFldAlign(PAT_MAIN_04, 0x300   , C_MAIN_FC_CB);
    vIO32WriteFldAlign(PAT_MAIN_04, 0x300   , C_MAIN_FC_CR);
    vIO32WriteFldAlign(PAT_MAIN_05,   0x0   , C_MAIN_BC_Y);
    vIO32WriteFldAlign(PAT_MAIN_05, 0x100   , C_MAIN_BC_CB);
    vIO32WriteFldAlign(PAT_MAIN_05, 0x100   , C_MAIN_BC_CR);


    vIO32WriteFldAlign(PAT_PIP_04,   0x3    , C_MAIN_HV_MODE);
    vIO32WriteFldAlign(PAT_PIP_04, 0x300    , C_MAIN_FC_Y);
    vIO32WriteFldAlign(PAT_PIP_04, 0x300    , C_MAIN_FC_CB);
    vIO32WriteFldAlign(PAT_PIP_04, 0x300    , C_MAIN_FC_CR);
    vIO32WriteFldAlign(PAT_PIP_05,   0x0    , C_MAIN_BC_Y);
    vIO32WriteFldAlign(PAT_PIP_05, 0x100    , C_MAIN_BC_CB);
    vIO32WriteFldAlign(PAT_PIP_05, 0x100    , C_MAIN_BC_CR);


    if (bPath == SV_VP_MAIN)
    {
        // TODO
        vIO32WriteFldAlign(PAT_MAIN_02, bPattern, C_MAIN_PAT_TYPE);   
        vIO32WriteFldAlign(PAT_MAIN_02, bPattern, C_MAIN_NOISE_EN);
        vIO32WriteFldAlign(PAT_MAIN_02, 0x8     , C_MAIN_NOISE_LEVEL);
    }
    else
    {
        // TODO
        vIO32WriteFldAlign(PAT_PIP_02,  bPattern, C_MAIN_PAT_TYPE);
        vIO32WriteFldAlign(PAT_PIP_02,  bPattern, C_MAIN_NOISE_EN);
        vIO32WriteFldAlign(PAT_PIP_02,   0x8    , C_MAIN_NOISE_LEVEL);
    }
}

/*****************************************************************************************/
/******************************   YCPROC MAIN LOOP   ***************************************/
/*****************************************************************************************/

static void _vDrvSCEParialLoadProc(void)
{
    UINT32 u4Temp = u4IO32Read4B(SCE_SW_LOAD);
    
    if (u4Temp&Fld2Msk32(SCE_SW_LOAD_TRIG))
    {
        vDrvSCELoadIdx(
            (u4Temp&Fld2Msk32(SCE_SW_LOAD_FUNC_IDX))>>Fld_shft(SCE_SW_LOAD_FUNC_IDX),
            (u4Temp&Fld2Msk32(SCE_SW_LOAD_HUE_IDX))>>Fld_shft(SCE_SW_LOAD_HUE_IDX),
            (u4Temp&Fld2Msk32(SCE_SW_LOAD_VALUE))>>Fld_shft(SCE_SW_LOAD_VALUE));
        vIO32WriteFldAlign(SCE_SW_LOAD, 0, SCE_SW_LOAD_TRIG);
    }
}

void vDrvBitPlusProc(void)
{
    UINT8 u1Index = 0;
    UINT8 u1NoiseIdx = 3;
    UINT8 u1NoiselevelTh[4] = {0, 8, 16, 24};
    UINT8 u1BitPlusYRange[4] = {12, 8, 4, 0};
    UINT8 u1BitPlusSRange[4] = {16, 12, 8, 0};
    UINT8 u1Noiselevel = (UINT8)IO32ReadFldAlign(NR_NMSTA_0E, NR_NM_STA_NL);

    for(u1Index = 0; u1Index < 4; u1Index ++)
    {
        if(u1Noiselevel >= u1NoiselevelTh[u1Index])
        {
            u1NoiseIdx = u1Index;
        }
    }

    vDrvBitPlusSetYRange(u1BitPlusYRange[u1NoiseIdx]);
    vDrvBitPlusSetSRange(u1BitPlusSRange[u1NoiseIdx]);    
    
}

void vDrvGlobalPQAdj(void)
{
    // Global PQ adjustment
    UINT8 bPath, bOffset, bDemoModeOnOff = SV_OFF, bDemoToggle = SV_OFF;
    static UINT8 bpreDemoModeOnOff = 0xFF;
    UINT8 u1MatrixPQAdj = IO32ReadFldAlign(MATRIX_00, MATRIX_PQ_ADJ);

    for (bPath = SV_VP_MAIN; bPath < SV_VP_NA; bPath++)
    {
        bOffset = (bPath ==SV_VP_MAIN) ? 0 : 0x4;
        if (bPath == SV_VP_MAIN)
        {
            bDemoModeOnOff = (IO32ReadFldAlign(DEMO_00, DEMOMODE_ENABLE)&&IO32ReadFldAlign(DEMO_02, DEMO_COLOR_EN)) ? SV_ON : SV_OFF;
            bDemoToggle = (bDemoModeOnOff != bpreDemoModeOnOff) ? SV_ON : SV_OFF;
            bpreDemoModeOnOff = bDemoModeOnOff ;
        }
        
		if ((bPath == SV_VP_MAIN) && IO32ReadFldAlign(PEUI_00, PEUI_OS_CSC_MODE_CHANGE_M)) 
		{
            vDrvCalVideoColorMatrix(SV_VP_MAIN, 0x80, 0x80, 0x80, 0x80);
			vIO32WriteFldAlign(PEUI_00, 0, PEUI_OS_CSC_MODE_CHANGE_M);
		}

		if ((bPath == SV_VP_PIP) && IO32ReadFldAlign(PEUI_00, PEUI_OS_CSC_MODE_CHANGE_P)) 
		{
            vDrvCalVideoColorMatrix(SV_VP_PIP, 0x80, 0x80, 0x80, 0x80);
			vIO32WriteFldAlign(PEUI_00, 0, PEUI_OS_CSC_MODE_CHANGE_P);
		}			
		
        if ((u1MatrixPQAdj & !bDemoModeOnOff) ||
            (bPath == SV_VP_PIP) || (ISSCALERRGB(bPath)))
        {			
            if ((IO32ReadFldAlign(GLOBAL_ADJ_00 + bOffset, BRIGHT_DUMMY) != u2PQItemMatrix[bPath][PQ_BRIGHTNESS]) ||
                (IO32ReadFldAlign(GLOBAL_ADJ_02 + bOffset, CONT_DUMMY) != u2PQItemMatrix[bPath][PQ_CONTRAST]) ||
                (IO32ReadFldAlign(GLOBAL_ADJ_02 + bOffset, SAT_DUMMY) != u2PQItemMatrix[bPath][PQ_SATURATION]) ||
                (IO32ReadFldAlign(GLOBAL_ADJ_00 + bOffset, HUE_DUMMY) != u2PQItemMatrix[bPath][PQ_HUE]) ||
                (IO32ReadFldAlign(GLOBAL_ADJ_04 + bOffset, EXT_ADJ_BRI) != u2ExtAdjMatrix[bPath][PQ_BRIGHTNESS]) ||
                (IO32ReadFldAlign(GLOBAL_ADJ_04 + bOffset, EXT_ADJ_CONT) != u2ExtAdjMatrix[bPath][PQ_CONTRAST]) ||
                (IO32ReadFldAlign(GLOBAL_ADJ_04 + bOffset, EXT_ADJ_SAT) != u2ExtAdjMatrix[bPath][PQ_SATURATION]) ||
                (IO32ReadFldAlign(GLOBAL_ADJ_04 + bOffset, EXT_ADJ_HUE) != u2ExtAdjMatrix[bPath][PQ_HUE]) ||
                (IO32ReadFldAlign(GLOBAL_ADJ_06 + bOffset, EXT2_ADJ_BRI) != u2Ext2AdjMatrix[bPath][PQ_BRIGHTNESS]) ||
                (IO32ReadFldAlign(GLOBAL_ADJ_06 + bOffset, EXT2_ADJ_CONT) != u2Ext2AdjMatrix[bPath][PQ_CONTRAST])  ||
                (IO32ReadFldAlign(GLOBAL_ADJ_08 + bOffset, EXT2_ADJ_SAT) != u2Ext2AdjMatrix[bPath][PQ_SATURATION]) ||
                bDemoToggle)
            {
                vDrvVideoColorMatrix(bPath);

                // Reset Bri/Con/Hue/Sat;  Set Blue stretch
                if (bPath == SV_VP_MAIN)
                {
                    vRegWriteFldAlign(G_PIC_ADJ_MAIN_1, 0x400, BRIGHTNESS);
                    vRegWriteFldAlign(G_PIC_ADJ_MAIN_2, 0x200, HUE);
                    vRegWriteFldAlign(G_PIC_ADJ_MAIN_2, 0x80, SAT);
                    vRegWriteFldAlign(G_PIC_ADJ_MAIN_1, 0x80, CONTRAST);
                    vDrvSetBlueStretchBase();
                }
            }
        }
        else
        {
            if ((IO32ReadFldAlign(GLOBAL_ADJ_00 + bOffset, BRIGHT_DUMMY) != u2PQItemMatrix[bPath][PQ_BRIGHTNESS]) ||
                (IO32ReadFldAlign(GLOBAL_ADJ_02 + bOffset, CONT_DUMMY) != u2PQItemMatrix[bPath][PQ_CONTRAST]) ||
                (IO32ReadFldAlign(GLOBAL_ADJ_02 + bOffset, SAT_DUMMY) != u2PQItemMatrix[bPath][PQ_SATURATION]) ||
                (IO32ReadFldAlign(GLOBAL_ADJ_00 + bOffset, HUE_DUMMY) != u2PQItemMatrix[bPath][PQ_HUE]) ||
                (IO32ReadFldAlign(GLOBAL_ADJ_04 + bOffset, EXT_ADJ_BRI) != u2ExtAdjMatrix[bPath][PQ_BRIGHTNESS]) ||
                (IO32ReadFldAlign(GLOBAL_ADJ_04 + bOffset, EXT_ADJ_CONT) != u2ExtAdjMatrix[bPath][PQ_CONTRAST]) ||
                (IO32ReadFldAlign(GLOBAL_ADJ_04 + bOffset, EXT_ADJ_SAT) != u2ExtAdjMatrix[bPath][PQ_SATURATION]) ||
                (IO32ReadFldAlign(GLOBAL_ADJ_04 + bOffset, EXT_ADJ_HUE) != u2ExtAdjMatrix[bPath][PQ_HUE]) ||
                (IO32ReadFldAlign(GLOBAL_ADJ_06 + bOffset, EXT2_ADJ_BRI) != u2Ext2AdjMatrix[bPath][PQ_BRIGHTNESS]) ||
                (IO32ReadFldAlign(GLOBAL_ADJ_06 + bOffset, EXT2_ADJ_CONT) != u2Ext2AdjMatrix[bPath][PQ_CONTRAST])  ||
                (IO32ReadFldAlign(GLOBAL_ADJ_08 + bOffset, EXT2_ADJ_SAT) != u2Ext2AdjMatrix[bPath][PQ_SATURATION]) ||
                bDemoToggle)
            {                
                vIO32WriteFldAlign(G_PIC_ADJ_MAIN_1, 
                    0x400 + ((IO32ReadFldAlign(GLOBAL_ADJ_00, BRIGHT_DUMMY) - 0x80) << IO32ReadFldAlign(SCE_REG_00, SCE_BRI_INTERVAL)), 
                    BRIGHTNESS);
                vIO32WriteFldAlign(G_PIC_ADJ_MAIN_2, 
                    0x200 + ((IO32ReadFldAlign(GLOBAL_ADJ_00, HUE_DUMMY) - 0x80) << IO32ReadFldAlign(SCE_REG_00, SCE_HUE_INTERVAL)), 
                    HUE);
                
                vIO32WriteFldAlign(G_PIC_ADJ_MAIN_2, IO32ReadFldAlign(GLOBAL_ADJ_02, SAT_DUMMY), SAT);
                vIO32WriteFldAlign(G_PIC_ADJ_MAIN_1, IO32ReadFldAlign(GLOBAL_ADJ_02, CONT_DUMMY), CONTRAST);

                vDrvCalVideoColorMatrix(bPath, 0x80, 0x80, 0x80, 0x80);
                // Blue stretch
                if (bPath == SV_VP_MAIN)
                {
                    vDrvSetBlueStretchBase();
                }
            }
        }
    }

	if (IO32ReadFldAlign(PEUI_00, PEUI_IN_CSC_MODE_CHANGE_M))
	{
	    vDrvDIForce3DModeWithDelay();   // Avoid 3x3 matrix change then DI flicker       
		vVdoSetColorTransform(SV_VP_MAIN);
		vIO32WriteFldAlign(PEUI_00, SV_OFF, PEUI_IN_CSC_MODE_CHANGE_M);	
	}

	if (IO32ReadFldAlign(PEUI_00, PEUI_IN_CSC_MODE_CHANGE_P))
	{
		vVdoSetColorTransform(SV_VP_PIP);
		vIO32WriteFldAlign(PEUI_00, SV_OFF, PEUI_IN_CSC_MODE_CHANGE_P);	
	}
}

void vDrvSCEProc(void)
{
    static UINT8 bPreSceGain[3] = {0x80, 0x80, 0x80};
    static UINT8 bPreSkinTable[3] = {0x80, 0x80, 0x80};
    static UINT8 bPreSce3dGain[2] = {0x80, 0x80};

    vDrvGlobalPQAdj();

    //SCE cannot be enabled under VGA signal
    if (ISSCALERRGB(SV_VP_MAIN))
    {
    	return;
    }

    bSceGain[0] = IO32ReadFldAlign(ADAPTIVE_SCE_REG, ADAPTIVE_SCE_L);
    bSceGain[1] = IO32ReadFldAlign(ADAPTIVE_SCE_REG, ADAPTIVE_SCE_S);
    bSceGain[2] = IO32ReadFldAlign(ADAPTIVE_SCE_REG, ADAPTIVE_SCE_H);
    bSkinTable[0] = IO32ReadFldAlign(ADAPTIVE_SKIN_REG, ADAPTIVE_SKIN_L);
    bSkinTable[1] = IO32ReadFldAlign(ADAPTIVE_SKIN_REG, ADAPTIVE_SKIN_S);
    bSkinTable[2] = IO32ReadFldAlign(ADAPTIVE_SKIN_REG, ADAPTIVE_SKIN_H);
    bSce3dGain[0] = IO32ReadFldAlign(TDTV_FW_00, SCE_3D_GAIN_00);
    bSce3dGain[1] = IO32ReadFldAlign(TDTV_FW_00, SCE_3D_GAIN_01);
        
    if ((IO32ReadFldAlign(ADAPTIVE_SCE_REG, ADAPTIVE_SCE_FORCE_LOAD) == SV_ON) 
        || (bPreSceGain[0] != bSceGain[0]) || (bPreSceGain[1] != bSceGain[1]) || (bPreSceGain[2] != bSceGain[2])
        || (bPreSkinTable[0] != bSkinTable[0]) || (bPreSkinTable[1] != bSkinTable[1]) || (bPreSkinTable[2] != bSkinTable[2])
        || (((bPreSce3dGain[0] != bSce3dGain[0]) || (bPreSce3dGain[1] != bSce3dGain[1])) && (u4DrvTDTV3DModeQuery() != E_TDTV_UI_3D_MODE_OFF)))
    {
        bPreSceGain[0] = bSceGain[0]; 
        bPreSceGain[1] = bSceGain[1]; 
        bPreSceGain[2] = bSceGain[2]; 
        bPreSkinTable[0] = bSkinTable[0]; 
        bPreSkinTable[1] = bSkinTable[1]; 
        bPreSkinTable[2] = bSkinTable[2]; 
        bPreSce3dGain[0] = bSce3dGain[0];
        bPreSce3dGain[1] = bSce3dGain[1];

        if (IO32ReadFldAlign(SCE_REG_00, SCE_CALC_ONOFF))
        {
            vDrvSCECalculate();
            vIO32WriteFldAlign(ADAPTIVE_SCE_REG,SV_OFF, ADAPTIVE_SCE_FORCE_LOAD);
            vApiRegisterVideoEvent(PE_EVENT_SCE, SV_VP_MAIN, SV_ON);
        }
    }

    _vDrvSCEParialLoadProc();

    if(IO32ReadFldAlign(ADAP_LUMA2_00, ADAP_BITPLUS_EN) == SV_ON)
    {
        vDrvBitPlusProc();
    }
}

void vDrvMMAPPorc(void)
{
    if (IO32ReadFldAlign(MMAPP_04, APP_CHANGE_NOTIFY) == SV_ON)
    {
        vIO32WriteFldAlign(MMAPP_04, SV_OFF, APP_CHANGE_NOTIFY);

        sMMAppInfo.u1AppType = VDP_GetPlayMode(SV_VP_MAIN);
        //sMMAppInfo.u1AppType = IO32ReadFldAlign(MMAPP_00, APP_TYPE);
        
        if (sMMAppInfo.u1AppType == 0xFF) /*Pretend for Netflix App */
        {
            sMMAppInfo.u2SrcWidth = IO32ReadFldAlign(MMAPP_01, APP_SRC_WIDTH);
            sMMAppInfo.u2SrcHeight = IO32ReadFldAlign(MMAPP_01, APP_SRC_HEIGHT);
            sMMAppInfo.u2OutWidth = IO32ReadFldAlign(MMAPP_02, APP_OUT_WIDTH);
            sMMAppInfo.u2OutHeight = IO32ReadFldAlign(MMAPP_02, APP_OUT_HEIGHT);
            sMMAppInfo.u4BitRate = IO32ReadFldAlign(MMAPP_03, APP_BITRATE);
            DRVCUST_MMAPProc();
        }
    }
}


#define OD_STATE_CNT 2
#define DI_STATE_CNT 3
#define GFX_STATE_CNT 3
EXTERN void HalFlushInvalidateDCache(void);

void vDrvDumpShrink(void)
{   
    UINT32 i, j;
    UINT32 pos_x, pos_y;
    UINT32 ratio_x, ratio_y;
    UINT32* pBuf = (UINT32 *)VIRTUAL(u4IO32Read4B(SWRW_01));
    UINT32 u4SrcWidth = IO32ReadFldAlign(SWRW_03, DUMP_CTRL_WIDTH);
    UINT32 u4SrcHeight = IO32ReadFldAlign(SWRW_03, DUMP_CTRL_HEIGHT);
    UINT32 u4DesWidth = IO32ReadFldAlign(SWRW_05, DUMP_CTRL_SHRINK_W);
    UINT32 u4DesHeight = IO32ReadFldAlign(SWRW_05, DUMP_CTRL_SHRINK_H);
    
    LOG(4, "OD Dump shrink W %d  H %d", u4DesWidth, u4DesHeight);

    
    if (pBuf == NULL)
    {        
        LOG(0, "Dump Shrink error");        
        return;
    }

    if ((u4DesWidth == 0) || (u4DesHeight == 0))
    {        
        LOG(4, "No Shrink", u4DesWidth, u4DesHeight);        
        return;
    }

    if ((u4DesWidth >= u4SrcWidth) || (u4DesHeight >= u4SrcHeight))
    {
        LOG(4, "Not support enlarge function");
        return;
    }

    ratio_x = (u4SrcWidth<<8)/u4DesWidth;
    ratio_y = (u4SrcHeight<<8)/u4DesHeight;
    
    for (j=0; j<u4DesHeight; j++)
    {
        pos_y = (j*ratio_y)>>8;
        
        for (i=0; i<u4DesWidth; i++)
        {            
            pos_x = (i*ratio_x)>>8;
            
            pBuf[j*u4DesWidth+i] = pBuf[pos_y*u4SrcWidth+pos_x];
        }
    }           
    
    vIO32WriteFldAlign(SWRW_03, u4DesWidth, DUMP_CTRL_WIDTH);
    vIO32WriteFldAlign(SWRW_03, u4DesHeight, DUMP_CTRL_HEIGHT);
    vIO32WriteFldAlign(SWRW_04, u4DesWidth*u4DesHeight*4, DUMP_CTRL_SIZE); 
}

void vDrvDumpPostProc(void)
{     
    HalFlushInvalidateDCache();  
    vDrvDumpShrink();      
    HalFlushInvalidateDCache();

    // finish dump
    vIO32WriteFldAlign(SWRW_04, 0, DUMP_CTRL_STATE);
    vIO32WriteFldAlign(SWRW_04, DUMP_ENG_NONE, DUMP_CTRL_ENG_SEL);  
}

void vDrvDumpSetSize(UINT16 u2DumpWidth, UINT16 u2DumpHeight)
{
    vIO32WriteFldAlign(SWRW_03, u2DumpWidth, DUMP_CTRL_WIDTH);
    vIO32WriteFldAlign(SWRW_03, u2DumpHeight, DUMP_CTRL_HEIGHT);

    vIO32WriteFldAlign(SWRW_07, u2DumpWidth, DUMP_REQ_WIDTH);
    vIO32WriteFldAlign(SWRW_07, u2DumpHeight, DUMP_REQ_HEIGHT);    
}

void vDrvDumpSetShrinkSize(UINT16 u2ShrinkWidth, UINT16 u2ShrinkHeight)
{
    vIO32WriteFldAlign(SWRW_05, u2ShrinkWidth, DUMP_CTRL_SHRINK_W);
    vIO32WriteFldAlign(SWRW_05, u2ShrinkHeight, DUMP_CTRL_SHRINK_H);    
}

void vDrvDumpSetContinuousMode(UINT8 u1OnOff)
{
    vIO32WriteFldAlign(SWRW_09, u1OnOff, CONTINUOUS_DUMP_EN);    
}

void vDrvDumpStart(UINT8 u1Module)
{
    vIO32WriteFldAlign(SWRW_04, u1Module, DUMP_CTRL_ENG_SEL);    
    vIO32WriteFldAlign(SWRW_04, DUMP_STATE_INIT, DUMP_CTRL_STATE);
}

void vDrvDumpForceDRAMAddress(UINT32 u4DumpDRAMAddr)
{
    vIO32Write4B(SWRW_08, u4DumpDRAMAddr);
}

void vDrvGetDumpInfo(void)
{
    UINT32 u4DRAMPoolSize = u4IO32Read4B(SWRW_02);     
    UINT32 u4DRAMAddress = u4IO32Read4B(SWRW_01);
    UINT32 u4DumpImageSize = IO32ReadFldAlign(SWRW_04, DUMP_CTRL_SIZE);
    UINT16 u2ImgSrcWidth = IO32ReadFldAlign(SWRW_07, DUMP_REQ_WIDTH);
    UINT16 u2ImgSrcHeight = IO32ReadFldAlign(SWRW_07, DUMP_REQ_HEIGHT);        
    UINT16 u2ImgShrinkWidth = IO32ReadFldAlign(SWRW_05, DUMP_CTRL_SHRINK_W);
    UINT16 u2ImgShrinkHeight = IO32ReadFldAlign(SWRW_05, DUMP_CTRL_SHRINK_H);        

    Printf("\n[dump] dram pool size = 0x%x\n", u4DRAMPoolSize);        
    Printf("[dump] image dram address = 0x%x\n", u4DRAMAddress);
    Printf("[dump] image dram size = 0x%x\n", u4DumpImageSize);    
    Printf("[dump] image source (width x height) = (%d x %d)\n", u2ImgSrcWidth, u2ImgSrcHeight);    
    Printf("[dump] image shrinked (width x height) = (%d x %d) = 0x%x\n", u2ImgShrinkWidth, u2ImgShrinkHeight, u2ImgShrinkWidth*u2ImgShrinkHeight*4);        
}

void vDrvDumpProc(void)
{
    UINT32 u4State = IO32ReadFldAlign(SWRW_04, DUMP_CTRL_STATE);
    UINT32 u4EngSel = IO32ReadFldAlign(SWRW_04, DUMP_CTRL_ENG_SEL);
    UINT32 u4ODReady = IO32ReadFldAlign(OD_STA00, OD_RDY);
    UINT8 u1ContinuousDumpEn = IO32ReadFldAlign(SWRW_09, CONTINUOUS_DUMP_EN);
    UINT8 u1ContinuousDumpPause = IO32ReadFldAlign(SWRW_09, CONTINUOUS_DUMP_PAUSE);    

    static UINT32 u4DumpWaitCnt = 0;
    static UINT8 u1ODWaitCnt = 0;
    static UINT8 u1ODErrorCnt = 0;
    
    if (u4State == DUMP_STATE_INIT)
    {

        if((u1ContinuousDumpEn == SV_TRUE) && (u1ContinuousDumpPause == SV_TRUE))
        {
            return;
        }
        
        if (u4EngSel == DUMP_ENG_OD)
        {            
            vDrvOdDumpInit();
            u4DumpWaitCnt = 0;  //OD_STATE_CNT;            
        }
        else if (u4EngSel == DUMP_ENG_PSCAN)        
        {            
            vDrvDIDumpInit(VDP_1);
            u4DumpWaitCnt = DI_STATE_CNT;            
        }
#ifdef CC_CDC_SUPPORT
		else if(u4EngSel == DUMP_ENG_GRAPHIC)
		{
			vScpipDumpInit();
            u4DumpWaitCnt = GFX_STATE_CNT;            
		}
#endif
        else
        {            
            u4DumpWaitCnt = 0;
            vIO32WriteFldAlign(SWRW_04, DUMP_STATE_INIT, DUMP_CTRL_STATE);
            return;
        }       
        
        vIO32WriteFldAlign(SWRW_04, DUMP_STATE_START, DUMP_CTRL_STATE);
    }   
    else if (u4State == DUMP_STATE_START)
    {        
        if (u4DumpWaitCnt == 0)
        {
            if (u4EngSel == DUMP_ENG_OD)
            {            
                if (u4ODReady == 1)
                {
                    //Printf("OD  ready \n");
                    vDrvOdDumpStart();
                    u4DumpWaitCnt = OD_STATE_CNT;
                    u1ODWaitCnt = 0;
                    vIO32WriteFldAlign(SWRW_04, DUMP_STATE_END, DUMP_CTRL_STATE);
                }
                else if (u1ODWaitCnt < 10)
                {
                    u1ODWaitCnt++;
                    //Printf("OD not ready:u1ODWaitCnt =%d \n",u1ODWaitCnt);
                }
                else
                {
                    //Printf("OD not ready:return \n");
                    u4DumpWaitCnt = 0;
                    u1ODWaitCnt = 0;
                    vIO32WriteFldAlign(SWRW_04, DUMP_ENG_NONE, DUMP_CTRL_ENG_SEL);
                    vIO32WriteFldAlign(SWRW_04, DUMP_STATE_INIT, DUMP_CTRL_STATE);
                    return;
                }
            }        
            else if (u4EngSel == DUMP_ENG_PSCAN)        
            {            
                vDrvDIDumpStart(VDP_1);
                u4DumpWaitCnt = DI_STATE_CNT;
                vIO32WriteFldAlign(SWRW_04, DUMP_STATE_END, DUMP_CTRL_STATE);
            }        
#ifdef CC_CDC_SUPPORT
			else if (u4EngSel == DUMP_ENG_GRAPHIC)        
            {            
                vScposGfxDumpStart(SV_VP_MAIN);
                u4DumpWaitCnt = GFX_STATE_CNT;
                vIO32WriteFldAlign(SWRW_04, DUMP_STATE_END, DUMP_CTRL_STATE);
            }
#endif
            else
            {                           
                u4DumpWaitCnt = 0;                
                vIO32WriteFldAlign(SWRW_04, DUMP_ENG_NONE, DUMP_CTRL_ENG_SEL);
                vIO32WriteFldAlign(SWRW_04, DUMP_STATE_INIT, DUMP_CTRL_STATE);
                return;
            }
        
            //vIO32WriteFldAlign(SWRW_04, DUMP_STATE_END, DUMP_CTRL_STATE);
        }
        else
        {
            //Printf("Start wait count %d \n", u4DumpWaitCnt);
            u4DumpWaitCnt = (u4DumpWaitCnt > 0) ? (u4DumpWaitCnt-1) : 0;
        }
    }
    else if (u4State == DUMP_STATE_END)
    {           
        if (u4DumpWaitCnt == 0)
        {
            if (u4EngSel == DUMP_ENG_OD)
            {
                vDrvOdDumpEnd();
                vDrvDumpPostProc();
            }
            else if (u4EngSel == DUMP_ENG_PSCAN)
            {
                vDrvDIDumpEnd(VDP_1);
                vDrvDumpPostProc();
            }

#ifdef CC_CDC_SUPPORT
            else if (u4EngSel == DUMP_ENG_GRAPHIC)
            {
                vScposGfxDumpEnd(VDP_1);
                //vScposGfxDumpPostProc();
            }
#endif

            if(u1ContinuousDumpEn == SV_TRUE)
            {
                vIO32WriteFldAlign(SWRW_04, DUMP_STATE_INIT, DUMP_CTRL_STATE);
            }
            else
            {
                if(DrvODOvfUdfFlag == SV_FALSE)
                {
                    vIO32WriteFldAlign(SWRW_04, DUMP_ENG_NONE, DUMP_CTRL_ENG_SEL);
                    vIO32WriteFldAlign(SWRW_04, DUMP_STATE_IDLE, DUMP_CTRL_STATE);
                    vDrvGetDumpInfo();
                }
                else
                {
                    u1ODErrorCnt ++;
                    if(u1ODErrorCnt < 5)
                    {
                        vIO32WriteFldAlign(SWRW_04, DUMP_STATE_INIT, DUMP_CTRL_STATE);
                    }
                    else
                    {
                        vIO32WriteFldAlign(SWRW_04, DUMP_ENG_NONE, DUMP_CTRL_ENG_SEL);
                        vIO32WriteFldAlign(SWRW_04, DUMP_STATE_IDLE, DUMP_CTRL_STATE);
                        u1ODErrorCnt = 0;
                        LOG(4, "OD Dump Fail\n");
                    }
                }
            }
        }
        else
        {
            //Printf("End wait count %d \n", u4DumpWaitCnt);
            u4DumpWaitCnt = (u4DumpWaitCnt > 0) ? (u4DumpWaitCnt-1) : 0;
        }            
    }
}

void vDrv3DPQProc(void)
{
    /*Reset PQ items when : 
        * a. Mode change done
        * b. 3D format change
        * c. Scaler resolution change
        */
    if(fgGetPEUIResetFlg(SV_VP_MAIN))
    {
    	#if SUPPORT_MIB_3D_CONVERT_444_TO_422
		vDrvSet422(SV_VP_MAIN, !(bIsScalerInput444(SV_VP_MAIN)));
		#endif
        vApiApplyPQSetting(SV_VP_MAIN);
        vDrvSetPEUIResetFlg(SV_VP_MAIN, SV_OFF);
    }
    if(fgGetPEUIResetFlg(SV_VP_PIP))
    {
        vApiApplyPQSetting(SV_VP_PIP);
        vDrvSetPEUIResetFlg(SV_VP_PIP, SV_OFF);
    }
}
void vDrvYcprocProc(void)
{
    // PQ SRAM Stress Test Procedure
#ifndef NDEBUG
    vDrvPQStressProc();
#endif // NDEBUG
    
    vDrvMeterProc();
    
    vDrvSCEProc();

    vDrvSharpProc();

    //vDrvOPCProc();	

    vDrvDemoProc();

    vDrvMMAPPorc();

    //vDrvADLProc();    
    vDrvADLProcMainLoop();

    //vDrvLcdimProc();
    vDrvLcdimProcMainLoop();

    //vDrvDumpProc();

    vDrv3DPQProc(); 
}

