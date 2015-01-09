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

// === INCLUDE =============================================================================
#include "x_assert.h"
#include "sv_const.h"
#include "nptv_debug.h"
#include "pe_if.h"
#include "pe_table.h"
#include "drvcust_if.h"
#include "drv_nr.h"
#include "vdp_drvif.h"
#include "vdo_misc.h"
#include "mute_if.h"
#include "drv_contrast.h"
#if defined(CC_MT5399) || defined(CC_MT5882)
#include "drv_lcdim_v2.h"
#else
#include "drv_lcdim.h"
#endif
#include "drv_scpos.h"
#include "drv_video.h"
#include "drv_upscaler.h"
#include "video_def.h"
#include "drv_upscaler.h"
#include "drv_od.h"
#include "drv_di.h"
#include "drv_display.h"
#include "drv_scaler.h"
#if defined(CC_MT5399)
#include "drv_mjc.h"
#endif
#ifdef DRV_SUPPORT_MT8283
#include "extmjc_if.h"
#endif
#ifdef CUSTOM_VIDEO_DRV_INCLUDE_FILE
	#include CUSTOM_VIDEO_DRV_INCLUDE_FILE
#endif // #ifndef CUSTOM_VIDEO_DRV_INCLUDE_FILE

// === DEFINE =============================================================================
#define GAMMA_INDEX 257

#ifndef SUPPORT_FLASH_PQ_GAMMA_INIT_DRAM
    #define SUPPORT_FLASH_PQ_GAMMA_INIT_DRAM    (0)
#endif

#ifndef FLASH_PQ_GAMMA_NUM
    #define FLASH_PQ_GAMMA_NUM                  (3)
#endif

#ifndef SUPPORT_EXTMJC_SEND_GAMMA_TABLE
    #define SUPPORT_EXTMJC_SEND_GAMMA_TABLE     (1)
#endif

#define SET_FLASH_PQ_READY(fgReady) vIO32WriteFldAlign(FLASH_PQ_00, ((fgReady)?1:0), FLASH_PQ_READY)

// === FUNCTION PROTOTYPE =================================================================
static void vHalVideoGammaTblInit(void);

// === EXTERN FUNCTION ====================================================================
extern UINT16 u2ScpipGetDispWidth(UINT8 bPath);
extern UINT16 u2ScpipGetDispHeight(UINT8 bPath); 
extern void vDrvDemoSetRegion(Region rDemoRegion);
extern UINT8 bxvYCCOnOff;
extern UINT8 bxvYCCGammaOnOff;
extern UINT8 bxvYCCAntiGammaOnOff;
extern UINT16 u2xvYCCMatrix[9];
// === GLOBAL VARIABLE ====================================================================
UINT8 QUALITY_TBL[QUALITY_MAX];
UINT8 _bSuperResolutionStats;
UINT8 bForceAllPQOff = SV_FALSE;
BOOL u1LEDMotionFlag = 0;

// === EXTERN VARIABLE ====================================================================
extern UINT8 _bDviTiming;
extern const tagDFT_QTY  CODE aDefaultQtyTbl[QUALITY_MAX];
extern UINT8 GAMMA_256[3][GAMMA_INDEX];
extern INT32 const YCBCR2RGBOS[3][15];
extern INT32 COLOR_TRANSFORM_ADJ[15];

extern const FLASH_PQ_SW_INFO aFlashPqSWInfo[FLASHPQ_NUM];
extern const FLASH_PQ_FLASH_INFO aFlashPqFlashInfo[FLASHPQ_NUM];
extern UINT16 pPedestalMatrixISR[15];
extern UINT8 bfgIsGammaDecoding;
extern UINT8 bIsGammaSRAMInitialized;

// === STATIC VARIABLE ====================================================================
static UINT8 bRegisterList[PE_EVENT_NS][REGISTER_DIM];
static UINT8 u1AdaptiveBackLightPWM[ADP_BACKLIGHT_END_IDX];

#if SUPPORT_FLASH_PQ_GAMMA_INIT_DRAM 
static UINT8 _rGammaTbl[FLASH_PQ_GAMMA_NUM][3*GAMMA_INDEX];
#endif

// === BODY ===============================================================================

UINT8 vDrvVerfPETable(void)
{
	UINT16 i;
	UINT8 RetVal = SV_TRUE;
		
	for(i=0; i < QUALITY_MAX; i++)
	{
		if(aDefaultQtyTbl[i].wQtyItem != i)
		{
			RetVal = SV_FALSE;
			LOG(0, "aDefaultQtyTbl[%d].wQtyItem = %d was wrong\n", 
				i, aDefaultQtyTbl[i].wQtyItem);
		}
	}	

	if(RetVal)
	{
		LOG(0, "Verf PE Table Passed\n");
	}

	return RetVal;
}

void vApiGammaInit(void)
{
    vHalVideoGammaTblInit();
    vHalVideoGammaTblUpdate(0);
    vHalVideoGammaLoadTable();

#ifdef DRV_SUPPORT_EXTMJC
    if (IS_COMPANION_CHIP_ON())
    {
        vHalVideoExternalGammaInit();
    }
#endif
}

void vApiVideoPeInit(void)
{
    UINT8 u1FlashPQReady = SV_FALSE;
    UINT8  bIndex = 0, index_count = FLASHPQ_NUM;
    
    vDrvVerfPETable();

    // Restore quality table from flash if support FlashPQ function.
    for(bIndex=0; bIndex<index_count; bIndex++)           //------------------------
    {
        if (bApiFlashPqVerifyHeader(bIndex) == SV_TRUE)
        {
            // If not support SmartPic, update QtyTable at boot-up. Otherwise, it will be done at each mode change done.
    		//Prevent 1st time boot up mode change done before flashPQ valued loaded.	

	        if (DRVCUST_OptGet((QUERY_TYPE_T)aFlashPqSWInfo[bIndex].dwSwQtySmartPicEnable) == SV_FALSE) 
	        {
	            if (bApiFlashPqUpdateQtyDft(bIndex) == SV_FALSE)
	            {
	                LOG(0, "Restore FlashPQ Qty fail\n");
	            }
                else
                {
                    u1FlashPQReady = SV_TRUE; //any one flashPQ is true, set  u1FlashPQReady = true
                }
	        }
            else //if smart Picture, don't check bApiFlashPqUpdateQtyDft, set the u1FlashPQReady to True directly
            {
                u1FlashPQReady = SV_TRUE;
            }

            
    	}
        else
        {
            //u1FlashPQReady = SV_FALSE;
            LOG(0, "FlashPQ<%d> verify Header fail\n", bIndex);
        }
    }

    if (u1FlashPQReady == SV_TRUE) 
    {
        u1FlashPQReady = bApiFlashPqRawDataInit();
    }
    
    SET_FLASH_PQ_READY(u1FlashPQReady);
    bApiParseCustomQtyItem();
	
#ifdef SCE_BY_SOURCE
	if(bApiFlashPqIsSupportSCEbySource(0))
	{
		DRVCUST_vMappingSCETiming();
	}
#endif
    bApiSetPWMDft(); // Set the default value for PWM value.

    bApiVideoProc(SV_VP_MAIN, PE_ARG_MODULE_ONOFF);
    
    vDrvSCELoadTable();

    // Init Gamma
	wWriteQualityTable(QUALITY_MODULE_ONOFF_GAMMA,1);
    vApiGammaInit();
    
    // Init OD
    //vDrvODInit(); move to videohwinit

    // Initial PQ setting
    vDrvSetBrightness(SV_VP_MAIN, 0x80);
    vDrvSetContrast(SV_VP_MAIN, 0x80);
    vDrvSetSaturation(SV_VP_MAIN, 0x80);
    vDrvSetHue(SV_VP_MAIN, 0x80);
    vDrvSetBrightness(SV_VP_PIP, 0x80);
    vDrvSetContrast(SV_VP_PIP, 0x80);
    vDrvSetSaturation(SV_VP_PIP, 0x80);
    vDrvSetHue(SV_VP_PIP, 0x80);
	
#ifdef __MODEL_slt__
    vApiVideoAllPQItemOnOff(SV_OFF);
#endif
}

void vApiRegisterVideoEvent(UINT8 bVdpArg, UINT8 bPath, UINT8 bOnOff)
{
#if defined(CC_MT5399)    
    if (bVdpArg < PE_EVENT_BEFORE_SCALER)
    {
        vApiRegisterVideoEventByApplyTime(bVdpArg, bPath, bOnOff, PEQTY_APPLY_TIME_INPUT_VSYNC_ISR);
    }
    else if ((bVdpArg < PE_EVENT_BEFORE_MJC) && (u1DrvMJCIsOnOff() == SV_ON))
    {
        vApiRegisterVideoEventByApplyTime(bVdpArg, bPath, bOnOff, PEQTY_APPLY_TIME_MJC_INPUT_VSYNC_ISR);
    }
    else if (bVdpArg == PE_EVENT_OD)
    {
        vApiRegisterVideoEventByApplyTime(bVdpArg, bPath, bOnOff, PEQTY_APPLY_TIME_OD_VSYNC_ISR);
    }
    else if (bVdpArg == PE_EVENT_DITHER)
    {
        vApiRegisterVideoEventByApplyTime(bVdpArg, bPath, bOnOff, PEQTY_APPLY_TIME_OUTPUT_VSYNC_ISR);
    }
    else
    {        
        vApiRegisterVideoEventByApplyTime(bVdpArg, bPath, bOnOff, PEQTY_APPLY_TIME_OUTPUT_VSYNC_ISR);
    }
#else
    if (bVdpArg < PE_EVENT_BEFORE_SCALER)
    {
        vApiRegisterVideoEventByApplyTime(bVdpArg, bPath, bOnOff, PEQTY_APPLY_TIME_INPUT_VSYNC_ISR);
    }
    else if (bVdpArg == PE_EVENT_OD)
    {
        vApiRegisterVideoEventByApplyTime(bVdpArg, bPath, bOnOff, PEQTY_APPLY_TIME_OD_VSYNC_ISR);
    }
    else if (bVdpArg == PE_EVENT_DITHER)
    {
        vApiRegisterVideoEventByApplyTime(bVdpArg, bPath, bOnOff, PEQTY_APPLY_TIME_OUTPUT_VSYNC_ISR);
    }    
    else
    {
        vApiRegisterVideoEventByApplyTime(bVdpArg, bPath, bOnOff, PEQTY_APPLY_TIME_OUTPUT_VSYNC_ISR);
    }
#endif
}

void vApiRegisterVideoEventByApplyTime(UINT8 bVdpArg, UINT8 bPath, UINT8 bOnOff, UINT8 bApplyTime)
{
    if(bVdpArg >= PE_EVENT_NS)
    {
        return;
    }
    
    bRegisterList[bVdpArg][REGISTER_CALLBACK] = SV_ON;
    bRegisterList[bVdpArg][REGISTER_PATH] = bPath;
    bRegisterList[bVdpArg][REGISTER_ON_OFF] = bOnOff;
    bRegisterList[bVdpArg][REGISTER_APPLY_TIME] = bApplyTime;
}

void vApiApplyVideoEvent(UINT8 bApplyTime)
{
    UINT8 bVdpArg;

    for (bVdpArg = 0; bVdpArg < PE_EVENT_NS; bVdpArg++)
    {
        if (bRegisterList[bVdpArg][REGISTER_CALLBACK] == SV_ON 
            && bRegisterList[bVdpArg][REGISTER_APPLY_TIME] == bApplyTime)
        {
            switch (bVdpArg)
            {
                case PE_EVENT_LUMA_CURVE:
                    vDrvADLUpdateLumaCurve();
                    break;     
                case PE_EVENT_LUMA_SEC_CURVE:
                    vDrvADLUpdateSecondLumaCurve();
                    break;                         
                case PE_EVENT_SCE:
                    vDrvSCELoadTable();
                    break;    				
                case PE_EVENT_SCE_PAT:
                    vHalSetSCEPat(bRegisterList[bVdpArg][REGISTER_PATH], bRegisterList[bVdpArg][REGISTER_ON_OFF] << 2);
                    break;                  
                case PE_EVENT_SHARPNESS:
    		        vDrvLoadYLevTable();
					vDrvLoadTDSCurve();
                    break;
				case PE_EVENT_SCALER_H:
					vDrvSCPQSetHCoeff(SV_VP_MAIN);			
					break;					
				case PE_EVENT_SCALER_V:
					SYS_MEASURE_TIME_START(SV_SCALER_TIMER);
					vDrvSCPQSetVCoeff(SV_VP_MAIN);
					SYS_MEASURE_TIME_END(SV_SCALER_TIMER);				
					SYS_STORE_TIME_INFO_TO_REG(SV_SCALER_TIMER); 					
					break;
                case PE_EVENT_SCALER_PE:
                    #if defined(CC_MT5399)|| defined (CC_MT5882)
                    vDrvUsPeiOnOff(SV_VP_MAIN, _bSuperResolutionStats);
                    #endif
                    break;   
                case PE_EVENT_OD:
                     //vDrvODLoadODTable();
                    vDrvODSetTable();
                    break; 
				case PE_EVENT_XVYCC:
                    vIO32WriteFldAlign(XVYCC_00, !bxvYCCOnOff, REG_BYPASS_XVYCC_MAIN);
					break;					                      
				case PE_EVENT_XVYCC_GAMMA:
                    vIO32WriteFldAlign(XVYCC_00, !bxvYCCGammaOnOff, REG_BYPASS_G);
					break;				 
				case PE_EVENT_XVYCC_ANTIGAMMA:
                    vIO32WriteFldAlign(XVYCC_00, !bxvYCCAntiGammaOnOff, REG_BYPASS_AG);
					break;				                    
				case PE_EVENT_XVYCC_MATRIX:                  
                	vIO32WriteFldAlign(XVYCC_05, u2xvYCCMatrix[0], REG_XV_MATRIX00);
                	vIO32WriteFldAlign(XVYCC_06, u2xvYCCMatrix[1], REG_XV_MATRIX01);
                	vIO32WriteFldAlign(XVYCC_06, u2xvYCCMatrix[2], REG_XV_MATRIX02);
                	vIO32WriteFldAlign(XVYCC_07, u2xvYCCMatrix[3], REG_XV_MATRIX10);
                	vIO32WriteFldAlign(XVYCC_07, u2xvYCCMatrix[4], REG_XV_MATRIX11);
                	vIO32WriteFldAlign(XVYCC_08, u2xvYCCMatrix[5], REG_XV_MATRIX12);
                	vIO32WriteFldAlign(XVYCC_08, u2xvYCCMatrix[6], REG_XV_MATRIX20);
                	vIO32WriteFldAlign(XVYCC_09, u2xvYCCMatrix[7], REG_XV_MATRIX21);
                	vIO32WriteFldAlign(XVYCC_09, u2xvYCCMatrix[8], REG_XV_MATRIX22);
					break;
                case PE_EVENT_COLOR_TRANSFORM:
                    vLoadColorMatrix(SV_VP_MAIN, pPedestalMatrixISR);
                    break;    
                case PE_EVENT_FLUSH_POST_REG:
                    vDrvFlushPostRegBuffer();    
                    break; 
                default:
                    break;
            }

            // After call video api, turn-off callback flag.
            bRegisterList[bVdpArg][REGISTER_CALLBACK] = SV_OFF;
        }
    }
}

/*****************************************************************************************/
/**********************************   DEMO MODE   ****************************************/
/*****************************************************************************************/
void vApiVideoDemoMode(UINT8 bmode, Region DemoRec, UINT8 bDEMOModules)
{
    Region DemoRegion;
    UINT8 bSplitOnOff, bMovingOnOff;
    UINT8 bRegion=0;
    UINT16 wDispCenter;
    //UINT16 wInputWidth = u2DrvDIGetWidth();
    UINT16 wDispWidth = u2ScpipGetDispWidth(SV_VP_MAIN);
    wDispCenter = (UINT16)u8Div6432(wDispWidth, 2, NULL);
    DemoRegion.wVStart = 0x0;
    DemoRegion.wVEnd = u2ScpipGetDispHeight(SV_VP_MAIN);
    //Fix Klocwork
    DemoRegion.wHStart = 0x0;
    DemoRegion.wHEnd = wDispCenter;
    DemoRegion.u1OnOff = SV_ON;
    
    switch (bmode)
    {
        case VDP_PQ_DEMO_ON_LEFT://Split demo
            bSplitOnOff = SV_ON;        
            bRegion = SV_LEFT;
            DemoRegion.u1OnOff = SV_ON;        
            DemoRegion.wHStart = 0x0;
            DemoRegion.wHEnd = wDispCenter;
            bMovingOnOff = SV_OFF;
            break;
        case VDP_PQ_DEMO_ON_RIGHT://Split demo
            bSplitOnOff = SV_ON;    
            bRegion = SV_RIGHT;
            DemoRegion.u1OnOff = SV_ON;        
            DemoRegion.wHStart = wDispCenter;
            DemoRegion.wHEnd = wDispWidth;
            bMovingOnOff = SV_OFF;
            break;
        case VDP_PQ_DEMO_STILL_REC://Rectangular demo
            bSplitOnOff = SV_OFF;
            DemoRegion.u1OnOff = SV_ON;
            DemoRegion.wHStart = DemoRec.wHStart;
            DemoRegion.wHEnd = DemoRec.wHEnd;
            DemoRegion.wVStart = DemoRec.wVStart;
            DemoRegion.wVEnd = DemoRec.wVEnd;
            bMovingOnOff = SV_OFF;
            break;
        case VDP_PQ_DEMO_MOVING_REC:
            bSplitOnOff = SV_OFF;
            DemoRegion.u1OnOff = SV_ON;
            DemoRegion.wHStart = DemoRec.wHStart;
            DemoRegion.wHEnd = DemoRec.wHEnd;
            DemoRegion.wVStart = DemoRec.wVStart;
            DemoRegion.wVEnd = DemoRec.wVEnd;
            bMovingOnOff = SV_ON;
        	 break;
        case VDP_PQ_DEMO_OFF:
        default:
             bSplitOnOff = SV_OFF;
             DemoRegion.u1OnOff = SV_OFF;
             bMovingOnOff = SV_OFF;
            break;
    }

    //Save Demo setting
    vIO32WriteFldAlign(DEMO_00, bSplitOnOff, SPLIT_ONOFF);
    vIO32WriteFldAlign(DEMO_00, bRegion, REGION_LEFT_RIGHT);
    vIO32WriteFldAlign(DEMO_02, bDEMOModules, DEMO_MODULES);
    vIO32WriteFldAlign(DEMO_00, bMovingOnOff, MOVINGEMO_ENABLE);
    vDrvDemoSetRegion(DemoRegion);

    //Split demo   ---move to  vDrvDemoProc after drv_ycproc.c---
    //vDrvDIDemo(bSplitOnOff&&(IO32ReadFldAlign(DEMO_02, DEMO_MDDI_EN)), bRegion,(wInputWidth/2)+0x44);   // 0x44 is for MEMA Demo position alignment. why 0x44? just test the position, and got this difference value.
    //vDrvNRSet2DNRDemo(bSplitOnOff&&(IO32ReadFldAlign(DEMO_02, DEMO_2DNR_EN)), bRegion, (wInputWidth/2));
    //vDrvNRSet3DNRDemo(bSplitOnOff&&(IO32ReadFldAlign(DEMO_02, DEMO_3DNR_EN)), bRegion, (wInputWidth/2));

    //Rectangular demo  ---move to  vDrvDemoProc after drv_ycproc.c---
    //vDrvSetPostSharpDemoRange(DemoRegion);    
    //vIO32WriteFldAlign(DEMO_00, bMovingOnOff, MOVINGEMO_ENABLE);
}

/*****************************************************************************************/
/**********************************  Sharpness  ******************************************/
/*****************************************************************************************/
void vHalVideoSharpnessOnOff(UINT8 bPath, UINT8 bOnOff)
{
    UINT8 bModuleOnOff = bOnOff && (!bForceAllPQOff) && wReadQualityTable(QUALITY_MODULE_ONOFF_SHARPNESS);     
    
	if(bPath==SV_VP_MAIN)
	{
		if(bIsScalerInputRGB(bPath))
		{
			bModuleOnOff = SV_OFF;
		}	
	       vDrvPostSharpOnOff(bModuleOnOff);
	}
	else
	{
		return;
	}	
}

void vHalVideoSharpnessHorizontalOnOff(UINT8 bPath, UINT8 bOnOff)
{
	if(bPath==SV_VP_MAIN)
	{
		if(bIsScalerInputRGB(bPath))
		{
			bOnOff = SV_OFF;
		}	
	       vDrvPostSharpHorizontalOnOff(bOnOff);
	}
	else
	{
		return;
	}	
}

void vHalVideoSharpnessVerticalOnOff(UINT8 bPath, UINT8 bOnOff)
{
	if(bPath==SV_VP_MAIN)
	{
		if(bIsScalerInputRGB(bPath))
		{
			bOnOff = SV_OFF;
		}	
	       vDrvPostSharpVerticalOnOff(bOnOff);
	}
	else
	{
		return;
	}	
}

void vHalVideoSharpnessDiagonalOnOff(UINT8 bPath, UINT8 bOnOff)
{
	if(bPath==SV_VP_MAIN)
	{
		if(bIsScalerInputRGB(bPath))
		{
			bOnOff = SV_OFF;
		}	
       	vDrvPostSharpDiagonalOnOff(bOnOff);
	}
	else
	{
		return;
	}	
}

void vHalVideoSharpnessParam(UINT8 bPath)
{
    if (bIsScalerInputRGB(bPath))
    {
		vDrvPreSharpOnOff(bPath, SV_OFF);
		if (bPath == SV_VP_MAIN)
		{
			vDrvPostSharpOnOff(SV_OFF);
		}
        return;
    }

    vDrvPreSharpParam(bPath);
	if (bPath == SV_VP_MAIN)
	{
	    vDrvPostSharpParam();
	}
}

void vHalVideoSharpnessParamHorizontal(UINT8 bPath)
{
    if (bIsScalerInputRGB(bPath))
    {
		if (bPath == SV_VP_MAIN)
		{
			vDrvPostSharpOnOff(SV_OFF);
		}
        return;
    }

	if (bPath == SV_VP_MAIN)
	{
	    vDrvPostSharpParamHorizontal();
	}
}

void vHalVideoSharpnessParamVertical(UINT8 bPath)
{
    if (bIsScalerInputRGB(bPath))
    {
		if (bPath == SV_VP_MAIN)
		{
			vDrvPostSharpOnOff(SV_OFF);
		}
        return;
    }

	if (bPath == SV_VP_MAIN)
	{
	    vDrvPostSharpParamVertical();
	}
}

void vHalVideoSharpnessParamDiagonal(UINT8 bPath)
{
    if (bIsScalerInputRGB(bPath))
    {
		if (bPath == SV_VP_MAIN)
		{
			vDrvPostSharpOnOff(SV_OFF);
		}
        return;
    }

	if (bPath == SV_VP_MAIN)
	{
	    vDrvPostSharpParamDiagonal();
	}
}

/*****************************************************************************************/
/********************************  Super Resolution   ****************************************/
/*****************************************************************************************/
void vHalVideoSuperResolutionOnOff(UINT8 bPath, UINT8 bOnOff)
{
#if defined(CC_MT5399)|| defined (CC_MT5882)
    UINT8 bModuleOnOff = bOnOff && (!bForceAllPQOff) && wReadQualityTable(QUALITY_MODULE_ONOFF_SUPER_RES);
	if(bPath==SV_VP_MAIN)
	{
		if(bIsScalerInputRGB(bPath))
		{
			bModuleOnOff = SV_OFF;
		}	
    #if defined(CC_MT5399)
		vDrvCBELoadParam();	
		vDrvCBEOnOff(bModuleOnOff);
	#endif
        //to set PE on/off at MJC isr for MJC model and output isr for non-MJC model
        vApiRegisterVideoEvent(PE_EVENT_SCALER_PE, SV_VP_MAIN, SV_ON);
		

        _bSuperResolutionStats = bModuleOnOff;
	}
	else
	{
		return;
	}
#endif
}



/*****************************************************************************************/
/**********************************  ADAPTIVE LUMA  ***************************************/
/*****************************************************************************************/
void vHalVideoAdaptiveLumaOnOff(UINT8 bPath, UINT8 bOnOff)
{
    if(bIsScalerInputRGB(bPath))
    {
    	bOnOff = SV_OFF;
    }
	if(bPath==SV_VP_MAIN)
	{
		vDrvADLOnOff(bOnOff);
	}
}

void vHalVideoAdaptiveLumaParam(void)
{
    vDrvADLUpdateParam();
}

/*****************************************************************************************/
/****************************   ADAPTIVE BACKLIGHT    ************************************/
/*****************************************************************************************/
void vHalSetAdpBacklight(UINT8 u1Level, UINT8 u1Value)
{
    u1AdaptiveBackLightPWM[u1Level] = u1Value;
}

UINT8 bHalGetAdpBacklight(UINT8 u1Level)
{
    return u1AdaptiveBackLightPWM[u1Level];
}

/*****************************************************************************************/
/********************************  LOCAL DIMMING    **************************************/
/*****************************************************************************************/
void vHalVideoLcDimOnOff(UINT8 bPath, UINT8 bOnOff)
{
	if(bPath==SV_VP_MAIN)
	{
		vDrvSetLcDimOnOff(bOnOff);
	}
}

/*****************************************************************************************/
/***********************************   CONTRAST   ****************************************/
/*****************************************************************************************/
void vHalVideoContrast(UINT8 bPath, UINT16 u2Value)
{
    vDrvSetContrast(bPath, u2Value);
}

/*****************************************************************************************/
/**********************************   BRIGHTNESS   ***************************************/
/*****************************************************************************************/
void vHalVideoBrightness(UINT8 bPath, UINT8 bValue)
{
    vDrvSetBrightness(bPath, bValue);
}

/*****************************************************************************************/
/**********************************   SATURATION   ***************************************/
/*****************************************************************************************/
void vHalVideoSaturation(UINT8 bPath, UINT16 u2Value)
{
    vDrvSetSaturation(bPath, u2Value);
}

/*****************************************************************************************/
/*************************************   HUE   *******************************************/
/*****************************************************************************************/
void vHalVideoHue(UINT8 bPath, UINT8 bValue)
{
    vDrvSetHue(bPath, bValue);
}

/*****************************************************************************************/
/*************************************   SCE   *******************************************/
/*****************************************************************************************/
void vHalVideoSCEOnOff(UINT8 bPath)
{
    if (bPath == SV_VP_MAIN)
    {
        vDrvSCEAllOnOff(!bIsScalerInputRGB(SV_VP_MAIN));
    }
}

void vHalSetSCEAngular(UINT8 bmodule, UINT8 bColor, UINT8 bValue)
{
    if (bmodule < SCE_MAX)
    {
        vDrvSCEAngularSetVal(bmodule, bColor, bValue);
    }
    else
    {
        vDrvSCEAngularReset();
    }
}

/**
 * @brief Set SCE pattern
 * @param bOnOff SV_ON/SV_OFF
 * @param wYdata SCE pattern Y level
 * @return NONE
 * @retval void
 */
void vHalSetSCEPat(UINT8 bOnOff, UINT16 wYdata)
{
    static UINT8 bBSOnOff = SV_OFF;
    static UINT8 bxvOnOff = SV_OFF;
    INT32 COLOR_TRANSFORM_Y2R[15];

    vDrvSCEPatGen(bOnOff, wYdata);
    vDrvSCEAllOnOff(SV_ON);
    if (bOnOff) 
    { 
       #if defined(CC_MT5399)
	    if (IO32ReadFldAlign(MJC_SYS_MUTE_0, MJC_MUTE_1TYPE))
        {
            vIO32WriteFldAlign(MJC_SYS_MUTE_0, 0, MJC_MUTE_1TYPE);
        }
	   #endif
        bBSOnOff = bDrvGetBlueStretchOnOff(); /* store latest setting of blue stretch*/
        if (bBSOnOff)
        {
            vDrvSetBlueStretchOnOff(SV_VP_MAIN, SV_OFF); /* bypass blue stretch */
        }

        bxvOnOff = bDrvGetxvYCCOnOff(); /* store latest setting of xvYCC*/
        if (bxvOnOff)
        {
            vDrvxvYCCOnOff(SV_OFF); /* bypass xvYCC */
        }
        
        x_memcpy(COLOR_TRANSFORM_Y2R, YCBCR2RGBOS[2], sizeof(COLOR_TRANSFORM_Y2R));
        vLoadOSColorMatrix(SV_VP_MAIN, COLOR_TRANSFORM_Y2R);
    }
    else
    { 
        if (bBSOnOff != bDrvGetBlueStretchOnOff())
        {
            vDrvSetBlueStretchOnOff(SV_VP_MAIN, bBSOnOff); /* restore user blue stretch setting */
        }
        
        if (bxvOnOff != bDrvGetxvYCCOnOff())
        {
            vDrvxvYCCOnOff(bBSOnOff); /* restore user blue stretch setting */
        }

        vLoadOSColorMatrix(SV_VP_MAIN, COLOR_TRANSFORM_ADJ);
    }	
}

/*****************************************************************************************/
/********************************** BLUE STRETCH  ****************************************/
/*****************************************************************************************/
void vHalVideoBlueStretchOnOff(UINT8 bPath, UINT8 bOnOff)
{
    UINT8 bModuleOnOff = bOnOff && (!bForceAllPQOff) && wReadQualityTable(QUALITY_MODULE_ONOFF_BLUE_STRETCH);      

    if (bIsScalerInputRGB(bPath))
    {
        bModuleOnOff = SV_OFF;
    }
    
    vDrvSetBlueStretchOnOff(bPath, bModuleOnOff);
}

/*****************************************************************************************/
/************************************   CTI   *******************************************/
/*****************************************************************************************/
void vHalVideoCTIOnOff(UINT8 bPath, UINT8 bOnOff)
{
    if (bIsScalerInputRGB(bPath))
    {
        bOnOff = SV_OFF;
    }

    if (bPath == SV_VP_MAIN)
    {
        vDrvCTIFOnOff(bOnOff);
        vDrvCTIROnOff(bOnOff);
    }
}

void vHalVideoCTIParam(UINT8 bPath)
{
    if (bIsScalerInputRGB(bPath))
    {
        vDrvCTIFOnOff(SV_OFF);
        vDrvCTIROnOff(SV_OFF);
        vDrvSCPQVCTIOnOff(SV_OFF);
        return;
    }

    if (bPath == SV_VP_MAIN)
    {
        vDrvCTIFParam();
        vDrvCTIRParam();
        vDrvSCPQVCTIOnOff(SV_ON);
    }
}

/*****************************************************************************************/
/************************************   LTI   *******************************************/
/*****************************************************************************************/
void vHalVideoLTIOnOff(UINT8 u1VidPath, UINT8 bOnOff)
{
    if (bIsScalerInputRGB(SV_VP_MAIN))
    {
        bOnOff = SV_OFF;
    }

    if (u1VidPath == SV_VP_MAIN)
    {
	    vDrvLTIOnOff(bOnOff);
    }
}

void vHalVideoLTIParam(UINT8 u1VidPath)
{
    if (bIsScalerInputRGB(SV_VP_MAIN))
    {
        vDrvLTIOnOff(SV_OFF);
        return;
    }

    if (u1VidPath == SV_VP_MAIN)
    {
	    vDrvLTIParam();
    }
}

/*****************************************************************************************/
/***********************************   RGB GAIN OFFSET ***********************************/
/*****************************************************************************************/
void vHalVideoRGBGain(UINT8 bPath, UINT8 bRGB, UINT16 wValue)
{    
    UINT8 bModuleOnOff = (!bForceAllPQOff) && wReadQualityTable(QUALITY_MODULE_ONOFF_RGB_GAIN);      
    
    if ((bPath != SV_VP_MAIN) && (bPath != SV_VP_PIP))
    {
        LOG(0, "vHalVideoRGBGain Path Error %d !", bPath);
        ASSERT(0);
    }
    
    wValue = ((bModuleOnOff == SV_ON)?(wValue):(0x200));
    
    switch (bRGB)
    {
        case SV_RGB_R:
            vDrvSetRGain1(bPath, wValue);
            break;
        case SV_RGB_G:
            vDrvSetGGain1(bPath, wValue);
            break;
        case SV_RGB_B:
            vDrvSetBGain1(bPath, wValue);
            break;
        default:
            ASSERT(0);
            break;
    }
}

void vHalVideoRGBOffset(UINT8 bPath, UINT8 bRGB, UINT16 wValue)
{
    UINT8 bModuleOnOff = (!bForceAllPQOff) && wReadQualityTable(QUALITY_MODULE_ONOFF_RGB_OFFSET);   
    
    wValue = ((bModuleOnOff == SV_ON)?(DRVCUST_RGBOfstRemap(wValue)):(0x0));

    if ((bPath != SV_VP_MAIN) && (bPath != SV_VP_PIP))
    {
        LOG(0, "vHalVideoRGBOffset Path Error %d !", bPath);
        ASSERT(0);
    }
    
    switch (bRGB)
    {
        // 10 bit RGB offset
        case SV_RGB_R:
            vDrvSetROffset11(bPath, (wValue));
            break;
        case SV_RGB_G:
            vDrvSetGOffset11(bPath, (wValue));
            break;
        case SV_RGB_B:
            vDrvSetBOffset11(bPath, (wValue));
            break;
        default:
            ASSERT(0);
            break;
    }
}

void vHalVideoRGBOffset2(UINT8 bPath, UINT8 bRGB, UINT16 wValue)
{
    wValue = DRVCUST_RGBOfstRemap(wValue);
    
    if ((bPath != SV_VP_MAIN) && (bPath != SV_VP_PIP))
    {
        LOG(0, "vHalVideoRGBOffset2 Path Error %d !", bPath);
        ASSERT(0);
    }
    
    switch (bRGB)
    {
        // 10 bit RGB offset
        case SV_RGB_R:
            vDrvSetROffset12(bPath, (wValue));
            break;
        case SV_RGB_G:
            vDrvSetGOffset12(bPath, (wValue));
            break;
        case SV_RGB_B:
            vDrvSetBOffset12(bPath, (wValue));
            break;
        default:
            ASSERT(0);
            break;
    }
}

void vHalOutRGBColorOnOff(UINT8 bOnOff)
{
    vScpipOutRGBColorOnOff(bOnOff);
}

void vHalOutRGBColor(UINT8 RValue, UINT8 GValue, UINT8 BValue)
{
    vScpipOutRGBColor(RValue, GValue, BValue);
}

/*****************************************************************************************/
/************************************  Gamma  ********************************************/
/*****************************************************************************************/
void vHalVideoGammaLoadTable(void)
{
    vDrvGammaDecodeTable();

    // remapp for customer specification. [optional]
    DRVCUST_SET_GAMMA();

    vDrvSetGammaWritePingPongFlag(SV_TRUE);       

    // only toggle gamma hardware on/off when SRAM has data ( initiated )
    if(bIsGammaSRAMInitialized == SV_TRUE)
    {
        vDrvGammaOnOff(SV_ON);
    }
}

void vHalVideoGammaLoadTableEX(void)
{
    bfgIsGammaDecoding = SV_FALSE;	
    vDrvSetGammaWritePingPongFlag(SV_TRUE);
}

void vHalVideoGammaTblInit(void)
{
    #if SUPPORT_FLASH_PQ_GAMMA_INIT_DRAM    
    UINT32 i;    
    
    for (i = 0; i < FLASH_PQ_GAMMA_NUM; i++)
    {
        vApiFlashPqSetGammaId(i+1);

        if (bApiFlashPqUpdateGamma())
        {
            x_memcpy(&_rGammaTbl[i][0], &GAMMA_256[0][0], 3*GAMMA_INDEX);
            
            #if (defined(DRV_SUPPORT_MT8283) && SUPPORT_EXTMJC_SEND_GAMMA_TABLE)
                drv_extmjc_gamma_write(&GAMMA_256[0][0], 3*GAMMA_INDEX, i);
            #endif
        }
    }
    #endif
    
    // if SUPPORT_FLASH_PQ_GAMMA_INIT_DRAM == 1
    // then it will only read Gamma from Flash once
    // otherwise, every time when we load Gamma, it will read Flash
    SET_GAMMA_IN_DRAM(SUPPORT_FLASH_PQ_GAMMA_INIT_DRAM);
}

void vHalVideoGammaTblUpdate(UINT8 u1Mode)
{         
#if SUPPORT_FLASH_PQ_GAMMA_INIT_DRAM
    if ((GAMMA_IN_DRAM) && (u1Mode >= 1) && (u1Mode <= FLASH_PQ_GAMMA_NUM))
    {
        x_memcpy(&GAMMA_256[0][0], &_rGammaTbl[u1Mode-1][0], 3*GAMMA_INDEX);
    }
    else
#endif        
    {
        vApiFlashPqSetGammaId(u1Mode);    
        bApiFlashPqUpdateGamma();
    }
}


#ifdef DRV_SUPPORT_EXTMJC
void vHalVideoExternalGammaInit(void)
{
//	//UINT8 bData, bChannel;
//    UINT32 ui4_start_time;
//    UINT8 *pDark[3],*pMiddle[3],*pBright[3];
//
//    ui4_start_time = x_os_get_sys_tick();
//	//CRIT_STATE_T csState;
//    pDark[0]=GAMMA_1024_DARK[SV_RGB_R];
//    pDark[1]=GAMMA_1024_DARK[SV_RGB_G];
//    pDark[2]=GAMMA_1024_DARK[SV_RGB_B];
//    pMiddle[0]=GAMMA_1024_MIDDLE[SV_RGB_R];
//    pMiddle[1]=GAMMA_1024_MIDDLE[SV_RGB_G];
//    pMiddle[2]=GAMMA_1024_MIDDLE[SV_RGB_B];
//    pBright[0]=GAMMA_1024_BRIGHT[SV_RGB_R];
//    pBright[1]=GAMMA_1024_BRIGHT[SV_RGB_G];
//    pBright[2]=GAMMA_1024_BRIGHT[SV_RGB_B];
//    drv_extmjc_gamma_write(pDark,pMiddle,pBright,1024);
//    drv_extmjc_set_gamma(SV_V_GAMMA_DARK, NULL);

//    Printf( "\r\n vHalVideoExternalGammaInit() cost %u ms\r\n\r\n", (x_os_get_sys_tick()-ui4_start_time)*5 );
}
#endif


/*****************************************************************************************/
/**************************************  White Peak  *************************************/
/*****************************************************************************************/
void vHalVideoWhitePeakLimit(UINT8 bOnOff)
{
    vDrvWhitePeakLimit(bOnOff);
}

/*****************************************************************************************/
/************************************  Noise Reduction  **********************************/
/*****************************************************************************************/
void vHalVideo2DAutoNROnOff(UINT8 bOnOff)
{
    vDrvNRSet2DNRAuto(bOnOff);
}

void vHalVideo3DAutoNROnOff(UINT8 bOnOff)
{
    vDrvNRSet3DNRAuto(bOnOff);
}

void vHalVideo2DNRParam(UINT8 bOnOff)
{
    vDrvNRSetSNRParam();
    vDrvNRSet2DNRSNROnOff(bOnOff);
}

void vHalVideo3DNRParam(UINT8 bOnOff)
{
    vDrvNRSet3DNRParam();
    vDrvNRSet3DNROnOff(bOnOff);
}

void vHalVideoMPEGNRParam(UINT8 bOnOff)
{
    vDrvNRSetMpegNRParam();
    vDrvNRSet2DNRBNROnOff(bOnOff);
    vDrvNRSet2DNRMNROnOff(bOnOff);
}

void vHalVideo3DNRStrength(UINT8 u1CurUI)
{
    UINT8 u1MaxNL = MIN(60, (10 * u1CurUI));
    vDrvNRSet3DNRMaxNoiseLvl(u1MaxNL);
}

/******************************************************************************************/
/***********************************  Bypass PQ Item  *********************************/
/******************************************************************************************/
void vApiVideoAllPQItemOnOff(UINT8 bOnOff)
{
    PE_ARG_TYPE i;
        
    if (bOnOff)
    {
        bForceAllPQOff = SV_FALSE;
        for (i = PE_ARG_BRIGHTNESS; i < PE_ARG_NS; i++)
        {
            bApiVideoProc(SV_VP_MAIN, i);
        }
        vDrvSetDefaultMatrix(SV_VP_MAIN, bOnOff);        
        vDrvSCPQSetHCoeff(SV_VP_MAIN);
        vDrvSCPQSetVCoeff(SV_VP_MAIN);
        vDrvSCPQVCTIOnOff(bOnOff);
        vDrvSCPQIRROnOff(bOnOff);
    }
    else
    {        

        bForceAllPQOff = SV_TRUE;        
        // Pre-proc
		// vDrvNRSetForceBypass(!bOnOff);
		// 2013-01-21 : NR control is bind with PSCAN, so we modify it to On/Off PQ only ( do NOT bypass )
		vDrvNRSet2DNRSNROnOff(bOnOff);
		vDrvNRSet2DNRMNROnOff(bOnOff);
		vDrvNRSet2DNRBNROnOff(bOnOff);
		vDrvNRSet3DNROnOff(bOnOff);

        // Scalar
        vHalVideoSuperResolutionOnOff(SV_VP_MAIN, bOnOff);
        vDrvDeRingOnOff(SV_VP_MAIN, SCPQ_DIR_H, bOnOff);
        vDrvDeRingOnOff(SV_VP_MAIN, SCPQ_DIR_V, bOnOff);
        vDrvSCPQTapAdapOnOff(SV_VP_MAIN, SCPQ_DIR_H, bOnOff);
        vDrvSCPQTapAdapOnOff(SV_VP_MAIN, SCPQ_DIR_V, bOnOff);
        vDrvSCPQVCTIOnOff(bOnOff);
        vDrvSCPQIRROnOff(bOnOff);

        // Post-proc
        vHalVideoCTIOnOff(SV_VP_MAIN, bOnOff);
        vHalVideoLTIOnOff(SV_VP_MAIN, bOnOff);
        vHalVideoSharpnessOnOff(SV_VP_MAIN, bOnOff);
        vDrvSCEAllOnOff(bOnOff);

        // Output-stage
        vDrvSetDefaultMatrix(SV_VP_MAIN, bOnOff);
        vHalVideoBlueStretchOnOff(SV_VP_MAIN, bOnOff);
        vDrvxvYCCOnOff(bOnOff);
        vHalVideoRGBGain(SV_VP_MAIN, SV_RGB_R, 0x200);
        vHalVideoRGBGain(SV_VP_MAIN, SV_RGB_G, 0x200);
        vHalVideoRGBGain(SV_VP_MAIN, SV_RGB_B, 0x200);
        vHalVideoRGBOffset(SV_VP_MAIN, SV_RGB_R ,0);
        vHalVideoRGBOffset(SV_VP_MAIN, SV_RGB_G ,0);
        vHalVideoRGBOffset(SV_VP_MAIN, SV_RGB_B ,0);
        vDrvGammaOnOff(bOnOff);
        vDrvSetLcDimOnOff(bOnOff);
        vDrvODOnOff(bOnOff);
        vDrvODBypass(!bOnOff);
    }
}
/******************************************************************************************/
/****************************************  LED Motion  **************************************/
/******************************************************************************************/
void vHalVideoLEDMotion(UINT8 OnOff)
{    
	LOG(4, "vHalVideoLEDMotion = %d\n",OnOff);
	if (OnOff)
	{
		u1LEDMotionFlag = SV_ON;
	}
	else
	{
		u1LEDMotionFlag = SV_OFF;
	}	
}

