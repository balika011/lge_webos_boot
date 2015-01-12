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
#include "general.h"
#include "util.h"
#include "pe_if.h"
#include "drv_nr.h"
#include "drv_nr_int.h"
#include "video_def.h"
#include "drv_video.h"
#include "api_backlt.h"
#include "drv_contrast.h"
#include "api_eep.h"
#include "eepdef.h"
#include "drv_tvd.h"
#include "vdo_misc.h"
#include "vdp_drvif.h"
#include "drv_scpos.h"
#include "eeprom_if.h"
#include "u_drv_cust.h"
#include "drvcust_if.h"
#include "drv_hdtv.h"
#include "drv_mpeg.h"
#include "x_debug.h"
#include "x_hal_5381.h"
#include "drv_vdoclk.h"
#include "drv_adcset.h"
#include "drv_autocolor.h"
#include "source_select.h"
#include "drv_pq_cust.h"
#include "drv_dvi.h"
#include "srm_drvif.h"
#include "drv_meter.h"
#include "drv_od.h"
#include "nptv_debug.h"
#include "panel.h"
#include "drv_default.h"
#include "drv_ldmspi.h"
#include "drv_upscaler.h"

#if defined(CC_SUPPORT_4K2K)||defined(CC_SUPPORT_HDMI_4K2K30)
#include "mtk_video_drv_4k2k.c"
#else
#include "mtk_video_drv.c"
#endif
#include "hw_ycproc.h"
#include "hw_ttd.h"
#include "hw_scpip_pq.h"
#ifndef CC_MT5882
#include "hw_mjc.h"
#endif
#include "hw_sw.h"
#include "drv_lcdim_v2.h"


typedef enum
{
    E_NR_CUST_ADAP_NL    = 0,
    E_NR_CUST_ADAP_XNR   = 1,
    E_NR_CUST_ADAP_RNR   = 2,
    E_NR_CUST_ADAP_MOANR = 3,
    E_NR_CUST_ADAP_FUNC_MAX
} E_NR_DFT_ADAP_FUNC;

static UINT8 gbSmartPicIndex = 0;
static UINT8 gbSceUi = 0;
static UINT8 gbOdUi = 0;
static UINT8 gbGammaUi = 0;
static UINT16 gu2RegDataTblIndex = 0;
static UINT16 gu2RawDataTblIndex = 0;
static UINT16 gu2RawDataCaseIndex = 0;


#ifdef SUPPORT_LCDIM_AVG_DEMO
UINT8 *bLcDimBlkInfo,*pu1BLBlockDataTbl;
UINT8 u1BlockNumH,u1BlockNumV;
UINT8 u1BlkStartNumIdx;
#endif
extern MMAppInfo sMMAppInfo;
#ifdef SCE_BY_SOURCE
extern UINT8 bSrcTimingInverseTbl[SOURCE_TYPE_TIMING_MAX+1];
#endif
#define MAX_COEF_TPYE 5
#define MAX_VC_COEF_IDX    11
#define MAX_VY_COEF_IDX    9
#define MAX_VC_COEF_HD_IDX    5
#define MAX_VY_COEF_HD_IDX    5

#ifdef CUST_PQ_SPLIT_DEMO
enum
{
	DEMO_PQ_OFF			= 0,
	DEMO_PQ_ON_RIGHT 	= 1,
	DEMO_PQ_ON_LEFT 	= 2
};
//DEMO modules

#define DEMO_TDC			1	<<	0	//driver not support now
#define DEMO_MDDI			1	<<	1	//driver not support now
#define DEMO_HSHARP			1	<<	2	//driver not support now
#define DEMO_2DNR			1	<<	3
#define DEMO_3DNR			1	<<	4
#define DEMO_TDSHARP		1	<<	5
#define DEMO_COLOR			1	<<	6
#define DEMO_ECTI			1	<<	7
#endif


#define EN_PEDASTAL_IN_SUB_PATH 1

enum
{
    SCAL_COEF_HY,
    SCAL_COEF_HC,
    SCAL_COEF_VY,
    SCAL_COEF_VC,
    SCAL_COEF_MAX,
};

UINT8 bSharpUITableForScaCoef[][SCAL_COEF_MAX][MAX_COEF_TPYE] =
{
    //HY                                           //HC                                      //VY                                         //VC
    {{255, 255, 255, 255, 255}, {255, 255, 255, 255, 255}, {10, 25, 255, 255, 255},   {10, 25, 255, 255,255},},//480
    {{255, 255, 255, 255, 255}, {255, 255, 255, 255, 255}, {10, 25, 255, 255, 255},   {10, 25, 255, 255, 255},},//576
    {{255, 255, 255, 255, 255}, {255, 255, 255, 255, 255}, {255, 255, 255, 255, 255}, {255, 255, 255, 255, 255},},//others
};

UINT8 bScaCoefIndex[][SCAL_COEF_MAX][MAX_COEF_TPYE] =
{
    //HY                //HC                     //VY                       //VC
    {{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {6, 8, 8, 8, 8}, {4, 7, 7 ,7, 7},},//480
    {{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {5, 7, 7, 7, 7}, {5, 7, 7, 7, 7},},//576
    {{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {4, 4, 4, 4, 4}, {4, 4, 4, 4, 4},},//others
};

UINT8 u1SatHistBoundary[] = {5, 13, 21, 29, 37, 45, 58}; //{1, 4, 9, 16, 25, 36, 49};

UINT32 u4SCVCCustCoeff[][SCPQ_COEF_TBLSIZE] =
{
    #include "Cust_Coef/CustCoef_VYC_p10.txt"  // 0
    #include "Cust_Coef/CustCoef_VYC_p10.txt"  // 1
    #include "Cust_Coef/CustCoef_VYC_p10.txt"  // 2
    #include "Cust_Coef/CustCoef_VYC_p10.txt"  // 3
    #include "Cust_Coef/CustCoef_VYC_p30.txt"  // 4
    #include "Cust_Coef/CustCoef_VYC_p50.txt"  // 5
    #include "Cust_Coef/CustCoef_VYC_p70.txt"   // 6
    #include "Cust_Coef/CustCoef_VC_New2_V6.txt"   // 7
    #include "Cust_Coef/CustCoef_VC_New3_V6.txt"   // 8
    #include "Cust_Coef/CustCoef_VC_New4_V6.txt"   // 9
    #include "Cust_Coef/CustCoef_VC_New5_V6.txt"   // 10
};

UINT32 u4SCVYCustCoeff[][SCPQ_COEF_TBLSIZE] =
{
    #include "Cust_Coef/CustCoef_VYC_p20.txt"  // 0
    #include "Cust_Coef/CustCoef_VYC_p25.txt"  // 1
    #include "Cust_Coef/CustCoef_VYC_p30.txt"  // 2
    #include "Cust_Coef/CustCoef_VYC_p35.txt"  // 3
    #include "Cust_Coef/CustCoef_VY_1.txt"  // 4
    #include "Cust_Coef/CustCoef_VY_4.txt"  // 5
    #include "Cust_Coef/CustCoef_VY_5.txt"  // 6
    #include "Cust_Coef/CustCoef_VY_6.txt"  // 7
    #include "Cust_Coef/CustCoef_VY_7.txt"  // 8
};

UINT32 u4SCVCCustCoeffHD[][SCPQ_COEF_TBLSIZE] =
{
    #include "Cust_Coef/CustCoef_VYC_p60.txt"  // 0
    #include "Cust_Coef/CustCoef_VYC_p70.txt"  // 1
    #include "Cust_Coef/CustCoef_VYC_p80.txt"  // 2
    #include "Cust_Coef/CustCoef_VYC_p90.txt"  // 3
    #include "Cust_Coef/CustCoef_VYC_Def.txt" // 4
};

UINT32 u4SCVYCustCoeffHD[][SCPQ_COEF_TBLSIZE] =
{
    #include "Cust_Coef/CustCoef_VYC_p60.txt"  // 0
    #include "Cust_Coef/CustCoef_VYC_p70.txt"  // 1
    #include "Cust_Coef/CustCoef_VYC_p80.txt"  // 2
    #include "Cust_Coef/CustCoef_VYC_p90.txt"  // 3
    #include "Cust_Coef/CustCoef_VYC_Def.txt" // 4
};

UINT32 u4FilterCoeff_HYDEF[SCPQ_COEF_TBLSIZE] =
{
	#include "Cust_Coef/HYDef.txt"
};
UINT32 u4FilterCoeff_HCDEF[SCPQ_COEF_TBLSIZE] =
{
	#include "Cust_Coef/HCDef.txt"
};
UINT32 u4FilterCoeff_VYCDEF[SCPQ_COEF_TBLSIZE] =
{
	#include "Cust_Coef/VYCDef.txt"
};

UINT32 u4FilterCoeff_HY3[SCPQ_COEF_TBLSIZE] =
{
	#include "Cust_Coef/HY3.txt"
};


UINT32 u4FilterCoeff_HY_CVBS[SCPQ_COEF_TBLSIZE] =
{
	#include "Cust_Coef/HY_CVBS.txt"
};

UINT32 u4FilterCoeff_VYC7[SCPQ_COEF_TBLSIZE] =
{
	#include "Cust_Coef/VY7.txt"
};
UINT32 u4FilterCoeff_HC7[SCPQ_COEF_TBLSIZE] =
{
	#include "Cust_Coef/HC7.txt"
};

UINT32 u4FilterCoeff_HC5[SCPQ_COEF_TBLSIZE] =
{
	#include "Cust_Coef/HC5.txt"
};

UINT32 u4FilterCoeff_HC1[SCPQ_COEF_TBLSIZE] =
{
	#include "Cust_Coef/HC1.txt"
};

UINT32 u4FilterCoeff_VYC6[SCPQ_COEF_TBLSIZE] =
{
	#include "Cust_Coef/VY6.txt"
};

UINT32 u4FilterCoeff_VYC5[SCPQ_COEF_TBLSIZE] =
{
	#include "Cust_Coef/VY5.txt"
};
UINT32 u4FilterCoeff_VYC1[SCPQ_COEF_TBLSIZE] =
{
	#include "Cust_Coef/VY1.txt"
};

UINT32 u4FilterCoeff_4Tap_HYC4[SCPQ_COEF_TBLSIZE] =
{
	#include "Cust_Coef/HY4_4Tap.txt"
};
UINT32 u4FilterCoeff_4Tap_VYC4[SCPQ_COEF_TBLSIZE] =
{
	#include "Cust_Coef/VY4_4Tap.txt"
};

UINT32 u4FilterCoeff_H3[SCPQ_COEF_TBLSIZE] =
{
	#include "Cust_Coef/HY3.txt"
};

UINT32 u4FilterCoeff_V_TTD[SCPQ_COEF_TBLSIZE] =
{
	#include "Cust_Coef/V_down_for_TTD.txt"
};


#ifdef DRV_SUPPORT_CUST_ANR
static NR_ADAP_WREG_BASE_T _rCustNrWRegState[E_NR_CUST_ADAP_FUNC_MAX+1] =
{
    {fgDrvAnrAutoOnNL,  fgDrvAnrFromSWOff, vDrvAnrSetNL,  vDrvAnrResetNL,  NR_WR_ST_IDLE},
    {fgDrvAnrAutoOnXNR, fgDrvAnrFromSWOff, vDrvAnrSetXNR, vDrvAnrResetXNR, NR_WR_ST_IDLE},
    {fgDrvAnrAutoOnRNR, fgDrvAnrFromSWRNR, vDrvAnrSetRNR, vDrvAnrResetRNR, NR_WR_ST_IDLE},
    {fgDrvAnrAutoOnMOANR, fgDrvAnrFromSWMOANR, vDrvAnrSetMOANR, vDrvAnrResetMOANR, NR_WR_ST_IDLE},
    {NULL, NULL, NULL, NULL, NR_WR_ST_NULL}
};

static void DRVCUST_AnrProc(const NR_NM_STA_T* prNmSta, const NR_NM_ANA_T* prNmAna)
{
    UINT32 u4NoiseLvl;
    UINT32 u4MaxNL;

    u4MaxNL = _SWR(NR_NM_03, MAX_NOISE_LVL);
    u4NoiseLvl = ((_SWR(NR_NM_00, FORCE_NOISE_LVL_EN)) ? _SWR(NR_NM_00, FORCE_NOISE_LVL) : prNmAna->u1NoiseLevel);
    u4NoiseLvl = MIN(u4NoiseLvl, u4MaxNL);

    if (_rCustNrWRegState[E_NR_CUST_ADAP_NL].fgAutoOn())
    {
        vDrvAnrNLProc(u4NoiseLvl);
    }

    if (_rCustNrWRegState[E_NR_CUST_ADAP_XNR].fgAutoOn())
    {
        vDrvAnrDCAlphaProc(u4NoiseLvl);
    }

    if (_rCustNrWRegState[E_NR_CUST_ADAP_RNR].fgAutoOn())
    {
        vDrvAnrRnrProc(u4NoiseLvl);
    }

    if (_rCustNrWRegState[E_NR_CUST_ADAP_MOANR].fgAutoOn())
    {
        vDrvAnrMoAnrProc(u4NoiseLvl, prNmAna->u1MoIdx);
    }
}
#endif

//extern PE_UI_RANGE_TBL rPEUITable;

UINT16 vApiFlashPqGetSceId()
{
    //This is sample code : SCE ID can be decided 
    //by panel id , ui setting, picture mode .....
    //UINT8 bPanelIdx = GetCurrentPanelIndex();
	UINT8 u1SrcTypTmg,u1SceSrcIdx,u2PanelIndex;
	UINT8 u2SceIndex=0;

	UNUSED(u1SrcTypTmg);
	UNUSED(u1SceSrcIdx);
    switch (gbSceUi)
    {
        case 0:
        default:
			u2PanelIndex=0;
	}
#ifdef SCE_BY_SOURCE
	if(bApiFlashPqIsSupportSCEbySource(0))
	{
		u1SrcTypTmg = bDrvVideoGetSourceTypeTiming(SV_VP_MAIN);

		u1SceSrcIdx = DRVCUST_wApiFlashPqGetSceIndexBySrcTiming(u1SrcTypTmg);
		
		if(u1SceSrcIdx >= uApiFlashPqGetSceSrcNum(0))
		{
			u1SceSrcIdx = (uApiFlashPqGetSceSrcNum(0)-1);
		}	
		
		u2SceIndex = u2PanelIndex * uApiFlashPqGetSceSrcNum(0)+ u1SceSrcIdx;		
	}
	else
	{
		u2SceIndex = u2PanelIndex;
	}
#endif
	
	return u2SceIndex;
}

UINT16 vApiFlashPqGetMinSceId()
{
    //This is sample code : SCE ID can be decided 
    //by panel id , ui setting, picture mode .....
    //UINT8 bPanelIdx = GetCurrentPanelIndex();
	UINT8 u2SceIndex=vApiFlashPqGetSceId();
    return (u2SceIndex+1);
}

void vApiFlashPqSetSceId(UINT8 bIndex)
{
    gbSceUi = bIndex;
}

UINT16 vApiFlashPqGetOdId(void)
{
    //UINT8 bPanelIdx = GetCurrentPanelIndex();   
    
    return gbOdUi;
}

void vApiFlashPqSetOdId(UINT8 bIndex)
{
    gbOdUi = bIndex;
}

UINT16 vApiFlashPqGetGammaId()
{
    //This is sample code : GAMMA ID can be deside 
    //by panel id , ui setting, picture mode .....
    //UINT8 bPanelIdx = GetCurrentPanelIndex();

    switch (gbGammaUi)
    {
        case 1:
            return 0;
        case 2:
            return 1;
        case 3:
            return 2;
        default:
    		return 0;
	}

#if 0 // This is sample code for different gamma table under 3D PR Panel
    switch (gbGammaUi)
    {
        case 1:
            // 3D Using Shutter Glass : IS_LVDS_DISP_3D_SHUTTER
            // 3D Using Polarized Panel : IS_LVDS_DISP_3D_POLARIZED
            // 
            // u4DrvTDTV3DModeQuery() : Query 3D Mode
            
            if((u4DrvTDTV3DModeQuery() != E_TDTV_UI_3D_MODE_OFF) && (IS_LVDS_DISP_3D_POLARIZED))
            {               
                return 0;       // gamma table for PR panel
            }
            else
			{
                return 1;       // gamma table for normal state
            }
        default:
    		return 0;
	}
#endif    
}

void vApiFlashPqSetGammaId(UINT8 bIndex)
{
    gbGammaUi = bIndex;
}

UINT16 vApiFlashPqGetQtyId(UINT8 bIndex)
{
	UNUSED(bIndex);
    if (PANEL_GetPanelWidth() <= 1440)
    {
        return (1);
    }
    else
    {
        return (0);
    }
}


void vApiFlashPqSetSmartPicId(UINT8 bIndex)
{
#if 0    
#ifndef CC_DRIVER_PROGRAM    
	UINT8 i4UiIndex, j;

	Printf("Smart Pic XXXXX %d\n", bIndex);
#endif
#endif
    gbSmartPicIndex = bIndex;

#if 0
#ifndef CC_DRIVER_PROGRAM
#ifdef __KERNEL__   
	ASSERT(rPEUITable.ptUiPicMinMaxDftTbl!=NULL && gbSmartPicIndex<rPEUITable.bSmartPicNum);   
	// update all source UI default table, since the table is same
	for (i4UiIndex = 0; i4UiIndex < (UINT8)EVDP_VIS_MAX; i4UiIndex++)
	{
		for(j=0; j< rPEUITable.bUIPQItemNum; j++)
		{
			if(rPEUITable.ptUiPicMinMaxDftTbl[gbSmartPicIndex][j].rPQItemType >=PE_ARG_BRIGHTNESS &&
				rPEUITable.ptUiPicMinMaxDftTbl[gbSmartPicIndex][j].rPQItemType < PE_ARG_NS)
			{
/*				printf("rPEUITable.ptUiPicMinMaxDftTbl[%d][%d].rPQItemType| %x, Min %x, Max %x, Dft %x \n", gbSmartPicIndex,j,rPEUITable.ptUiPicMinMaxDftTbl[gbSmartPicIndex][j].rPQItemType,
					rPEUITable.ptUiPicMinMaxDftTbl[gbSmartPicIndex][j].rPQItemRange.i4Min, rPEUITable.ptUiPicMinMaxDftTbl[gbSmartPicIndex][j].rPQItemRange.i4Max,
					rPEUITable.ptUiPicMinMaxDftTbl[gbSmartPicIndex][j].rPQItemRange.i4Dft);
*/
				PE_SetUiRange(i4UiIndex, rPEUITable.ptUiPicMinMaxDftTbl[gbSmartPicIndex][j].rPQItemType, 
														 &(rPEUITable.ptUiPicMinMaxDftTbl[gbSmartPicIndex][j].rPQItemRange));
			}
		}
}
#endif
#endif
#endif
}

UINT8 vApiFlashPqGetSmartPicId()
{
    return (gbSmartPicIndex);
}

UINT16 u2ApiFlashPqGetDataTblId(void)
{
    return gu2RegDataTblIndex;
}

void vApiFlashPqSetDataTblId(UINT16 u2Index)
{
    gu2RegDataTblIndex = u2Index;
}

UINT16 u2ApiFlashPqGetRawDataTblId(void)
{
    return gu2RawDataTblIndex;
}

UINT16 u2ApiFlashPqGetRawDataCaseId(void)
{
    return gu2RawDataCaseIndex;
}

void vApiFlashPqSetRawDataTblId(UINT16 u2Index)
{
    gu2RawDataTblIndex = u2Index;
}

void vApiFlashPqSetRawDataCaseId(UINT16 u2Index)
{
    gu2RawDataCaseIndex = u2Index;
}

UINT8 bApiFlashPqUpdateRawDataEx(UINT16 u2TblId, UINT16 u2CaseId)
{
    vApiFlashPqSetRawDataTblId(u2TblId);
    vApiFlashPqSetRawDataCaseId(u2CaseId);

    return bApiFlashPqUpdateRawData();
}

void DRVCUST_HwInit(void)
{
#ifdef DRV_SUPPORT_CUST_ANR
    NR_FW_ANR_RegisterCustFunc(DRVCUST_AnrProc, _rCustNrWRegState);
#endif

    return;
}

void DRVCUST_VideoInit(void)
{
    DRVCUST_AdaptiveBacklightInit();
#ifdef SUPPORT_LCDIM_AVG_DEMO
    DRVCUST_LcDimBlkInit(10,6);
#endif
}

void vApiCheckSharpnessUIValueForScalerCoef(void)
{
    UINT8 i,j;
    UINT8 bCurrentSharpUI = IO32ReadFldAlign(VDP_SCPQ_02, SCPQ_CUST_SHARP_UI);
    UINT8 reMappingTiming;
    UINT8 bCurrentCoefIndex[SCAL_COEF_MAX] = {0, 0, 0, 0};
    UINT8 bIdxStep;
   
    switch (wDrvVideoInputHeight(SV_VP_MAIN))
    {
        case 480:
            reMappingTiming = 0;
            break;
        case 576:
            reMappingTiming = 1;
            break;
        case 720:
        case 1080:
        default:
            reMappingTiming = 2;
            break;
    }

    for (j = SCAL_COEF_VY; j<=SCAL_COEF_VC; j++)
    {
        for(i=0; i<MAX_COEF_TPYE; i++)
        {
            if (bCurrentSharpUI < bSharpUITableForScaCoef[reMappingTiming][j][i])
            {
                if (i==0)
                {
                    //bIdxStep = bSharpUITableForScaCoef[reMappingTiming][j][i];
                    //bCurrentCoefIndex[j] = (bScaCoefIndex[reMappingTiming][j][i]*bCurrentSharpUI)/bIdxStep ;
                    bCurrentCoefIndex[j] = bScaCoefIndex[reMappingTiming][j][i];
                }
                else
                {
                    bIdxStep = bSharpUITableForScaCoef[reMappingTiming][j][i] - bSharpUITableForScaCoef[reMappingTiming][j][i-1];
                    bCurrentCoefIndex[j] = (bScaCoefIndex[reMappingTiming][j][i-1]*(bSharpUITableForScaCoef[reMappingTiming][j][i]-bCurrentSharpUI) + 
                                                   bScaCoefIndex[reMappingTiming][j][i]*(bCurrentSharpUI - bSharpUITableForScaCoef[reMappingTiming][j][i-1]))/bIdxStep ;
               }                                
               break;
            }
        }
    }
    
    if ((bCurrentCoefIndex[SCAL_COEF_VY] != IO32ReadFldAlign(VDP_SCPQ_02, SCPQ_CUST_VY_FILTER_ID))
        ||(bCurrentCoefIndex[SCAL_COEF_VC] != IO32ReadFldAlign(VDP_SCPQ_02, SCPQ_CUST_VC_FILTER_ID)))
    {
        vIO32WriteFldAlign(VDP_SCPQ_02, bCurrentCoefIndex[SCAL_COEF_VY], SCPQ_CUST_VY_FILTER_ID);
        vIO32WriteFldAlign(VDP_SCPQ_02, bCurrentCoefIndex[SCAL_COEF_VC], SCPQ_CUST_VC_FILTER_ID);
        vApiRegisterVideoEvent(PE_EVENT_SCALER_V, SV_VP_MAIN, SV_ON);
    }

}

UINT8 vDrvCustSetVCoeff(UINT8 bPath, UINT8 Y_Coef_Index, UINT8 C_Coef_Index)
{    
    if(bPath!=SV_VP_MAIN)
        return SV_FALSE;

    if (Y_Coef_Index < MAX_VY_COEF_IDX)
    {
    	vDrvSCPQSetSramCoeff(COEFTYP_VER_Y, u4SCVYCustCoeff[Y_Coef_Index]);
    }
    if (C_Coef_Index < MAX_VC_COEF_IDX)
    {
    	vDrvSCPQSetSramCoeff(COEFTYP_VER_C, u4SCVCCustCoeff[C_Coef_Index]);					
    }
    return SV_TRUE;
}

UINT8 vDrvCustSetVCoeffHD(UINT8 bPath, UINT8 Y_Coef_Index, UINT8 C_Coef_Index)
{    
    if(bPath!=SV_VP_MAIN)
        return SV_FALSE;

    if (Y_Coef_Index < MAX_VY_COEF_HD_IDX)
    {
    	vDrvSCPQSetSramCoeff(COEFTYP_VER_Y, u4SCVYCustCoeffHD[Y_Coef_Index]);
    }
    if (C_Coef_Index < MAX_VC_COEF_HD_IDX)
    {
    	vDrvSCPQSetSramCoeff(COEFTYP_VER_C, u4SCVCCustCoeffHD[C_Coef_Index]);					
    }
    return SV_TRUE;
}

void DRVCUST_SetVCTIGain(UINT32 u4CoefVCOFST)
{
    UINT32 u4VCTIGain, u4Gain;

    u4VCTIGain = IO32ReadFldAlign(VDP_SCPQ_02, SCPQ_VCTI_GAIN);
    u4Gain = 0x80 - u4CoefVCOFST * 0x20;    // OFST = 0~4; Gain = 0x80~0x0
    u4VCTIGain = (u4VCTIGain * u4Gain) / 0x80;
    vIO32WriteFldAlign(SCFIR_11, u4VCTIGain, VERT_CTI_GAIN);
}

void DRVCUST_HUPSCaleCoeff(UINT8 bPath)
{
    UINT32 u4SrcType = bGetSignalType(bPath);
    UINT32 u4SrcHeight = wDrvVideoInputHeight(bPath);

    // Set up-scaler coefficient
    if (bIsScalerInput444(bPath))
    {
        if (u4IO32Read4B(SCALER_COEF_MAIN_H_IN) >= 1280 )
        {
            vDrvSCPQSetSramCoeff(COEFTYP_HOR_Y, u4FilterCoeff_HYDEF);
            vDrvSCPQSetSramCoeff(COEFTYP_HOR_C, u4FilterCoeff_HCDEF);   
            LOG(2, "Scaler Coefficient RGB444: u4FilterCoeff_4Tap_HDEF\n");

        }
        else
        {
		vDrvSCPQSetSramCoeff(COEFTYP_HOR_Y, u4FilterCoeff_4Tap_HYC4);
		vDrvSCPQSetSramCoeff(COEFTYP_HOR_C, u4FilterCoeff_4Tap_HYC4);
            LOG(2, "Scaler Coefficient RGB444: u4FilterCoeff_4Tap_HYC4\n");
        }
    }
    else if (u4SrcHeight >= 720)
    {   // HD source
		vDrvSCPQSetSramCoeff(COEFTYP_HOR_Y, u4FilterCoeff_HYDEF);
		vDrvSCPQSetSramCoeff(COEFTYP_HOR_C, u4FilterCoeff_HCDEF);	
        LOG(2, "Scaler Coefficient: u4FilterCoeff_DEF HYC\n");
    }
    else if (u4SrcHeight >= 500)
    {   // SD 576 source
        if ((u4SrcType == SV_ST_DVI)  || (u4SrcType == SV_ST_MPEG))
        {   // HDMI, DTV
			vDrvSCPQSetSramCoeff(COEFTYP_HOR_Y, u4FilterCoeff_HY3);
			vDrvSCPQSetSramCoeff(COEFTYP_HOR_C, u4FilterCoeff_HC5);
            LOG(2, "Scaler Coefficient: u4FilterCoeff_HY3 HC5 576\n");
        }
        else if (u4SrcType == SV_ST_YP)
        {   // Component
			vDrvSCPQSetSramCoeff(COEFTYP_HOR_Y, u4FilterCoeff_HY3);
			vDrvSCPQSetSramCoeff(COEFTYP_HOR_C, u4FilterCoeff_HC1);			
            LOG(2, "Scaler Coefficient: u4FilterCoeff_HY3_HC1 576\n");
        }
        else
		{   // ATV, AV, SV
			vDrvSCPQSetSramCoeff(COEFTYP_HOR_Y, u4FilterCoeff_HY_CVBS);
			vDrvSCPQSetSramCoeff(COEFTYP_HOR_C, u4FilterCoeff_HC7);		
            LOG(2, "Scaler Coefficient: u4FilterCoeff_HY3_HC7 576\n");
        }
    }
    else
    {   // SD 480 source
        if ((u4SrcType == SV_ST_DVI) || (u4SrcType == SV_ST_MPEG))
        {   // HDMI, DTV
			vDrvSCPQSetSramCoeff(COEFTYP_HOR_Y, u4FilterCoeff_HY3);
			vDrvSCPQSetSramCoeff(COEFTYP_HOR_C, u4FilterCoeff_HC5);		
            LOG(2, "Scaler Coefficient: u4FilterCoeff_HY3_HC5 480\n");
        }
        else if (u4SrcType == SV_ST_YP)
        {   // Component
			vDrvSCPQSetSramCoeff(COEFTYP_HOR_Y, u4FilterCoeff_HY3);
			vDrvSCPQSetSramCoeff(COEFTYP_HOR_C, u4FilterCoeff_HC1);		
            LOG(2, "Scaler Coefficient: u4FilterCoeff_HY3_HC1 480\n");
        }
        else
        {   // ATV, AV, SV
			vDrvSCPQSetSramCoeff(COEFTYP_HOR_Y, u4FilterCoeff_HY_CVBS);
			vDrvSCPQSetSramCoeff(COEFTYP_HOR_C, u4FilterCoeff_HC7);		
            LOG(2, "Scaler Coefficient: u4FilterCoeff_HY3_HC7 480\n");
        }
    }
}

void DRVCUST_VUPSCaleCoeff(UINT8 bPath)
{
    UINT32 u4SrcHeight = wDrvVideoInputHeight(bPath);
    UINT32 u4CoefVYID, u4CoefVCID, u4CoefVYOFST, u4CoefVCOFST;

    u4CoefVYID = IO32ReadFldAlign(VDP_SCPQ_02, SCPQ_CUST_VY_FILTER_ID);
    u4CoefVCID = IO32ReadFldAlign(VDP_SCPQ_02, SCPQ_CUST_VC_FILTER_ID);
    u4CoefVYOFST = IO32ReadFldAlign(VDP_SCPQ_03, SCPQ_CUST_VY_OFST);
    u4CoefVCOFST = IO32ReadFldAlign(VDP_SCPQ_03, SCPQ_CUST_VC_OFST);

    u4CoefVYID = (u4CoefVYID >= u4CoefVYOFST) ? (u4CoefVYID - u4CoefVYOFST) : 0;
    u4CoefVCID = (u4CoefVCID >= u4CoefVCOFST) ? (u4CoefVCID - u4CoefVCOFST) : 0;

    // Set up-scaler coefficient
    if (bIsScalerInput444(bPath))
    {
        if (u4IO32Read4B(SCALER_COEF_MAIN_V_IN) >= 720 )
        {
            vDrvSCPQSetSramCoeff(COEFTYP_VER_Y, u4FilterCoeff_VYCDEF);
            vDrvSCPQSetSramCoeff(COEFTYP_VER_C, u4FilterCoeff_VYCDEF);
            LOG(2, "Scaler Coefficient RGB444: u4FilterCoeff_4Tap VYCDEF\n");
        }
        else
        {
            vDrvSCPQSetSramCoeff(COEFTYP_VER_Y, u4FilterCoeff_4Tap_VYC4);
            vDrvSCPQSetSramCoeff(COEFTYP_VER_C, u4FilterCoeff_4Tap_VYC4);
            LOG(2, "Scaler Coefficient RGB444: u4FilterCoeff_4Tap VYC4\n");
        }
    }
    else if (u4DrvTDTV3DModeQuery() == E_TDTV_UI_3D_MODE_TTD)
    {
        vDrvSCPQSetSramCoeff(COEFTYP_VER_Y, u4FilterCoeff_V_TTD);
        vDrvSCPQSetSramCoeff(COEFTYP_VER_C, u4FilterCoeff_V_TTD);		
        LOG(2, "Scaler Coefficient: u4FilterCoeff VYC for TTD\n");
    }
    else if (u4DrvTDTV3DModeQuery() == E_TDTV_UI_3D_MODE_TB)
    {
        vDrvSCPQSetSramCoeff(COEFTYP_VER_Y, u4FilterCoeff_VYC7);
        vDrvSCPQSetSramCoeff(COEFTYP_VER_C, u4FilterCoeff_VYC7);
        LOG(2, "Scaler Coefficient: u4FilterCoeff VYC for TAB\n");
    }
    else if (u4DrvTDTV3DModeQuery() == E_TDTV_UI_3D_MODE_OFF)
    {
        if (u4SrcHeight >= 720)
        {   // HD source
            vDrvCustSetVCoeffHD(bPath, u4CoefVYID, u4CoefVCID);
            LOG(2, "Scaler Coefficient: u4FilterCoeff HD, VY %d, VC %d\n", u4CoefVYID, u4CoefVCID);
        }
        else if (u4SrcHeight >= 500)
        {
            // SD PAL
            vDrvCustSetVCoeff(bPath, u4CoefVYID, u4CoefVCID);
            LOG(2, "Scaler Coefficient: u4FilterCoeff 576, VY %d, VC %d\n", u4CoefVYID, u4CoefVCID);
        }
        else
        {  
            // SD NTSC
            vDrvCustSetVCoeff(bPath, u4CoefVYID, u4CoefVCID);
            LOG(2, "Scaler Coefficient: u4FilterCoeff 480, VY %d, VC %d\n", u4CoefVYID, u4CoefVCID);
        }
    }
    else
    {
        vDrvSCPQSetSramCoeff(COEFTYP_VER_Y, u4FilterCoeff_VYCDEF);
        vDrvSCPQSetSramCoeff(COEFTYP_VER_C, u4FilterCoeff_VYCDEF);	
        LOG(2, "Scaler Coefficient: u4FilterCoeff VYC DEF for other 3D mode\n");
    }

    if (bPath == SV_VP_MAIN)
    {
        DRVCUST_SetVCTIGain(u4CoefVCOFST);
    }
}

void DRVCUST_VdoModeChgDone(UINT8 bPath)
{
    UNUSED(bPath);
}

void DRVCUST_VideoMainloop()
{
    DRVCUST_AdaptiveBacklightProc();
    vApiCheckSharpnessUIValueForScalerCoef();    
#ifdef SUPPORT_LCDIM_AVG_DEMO
	if(IO32ReadFldAlign(DEMO_02,DEMO_LCDIM_AVG_EN))
	{	
		vDrvUpdateLcDimBlockContentInfo();
	}
#endif
}

void DRVCUST_OutVSyncISR()
{
/*
    vDrvLcdimProc();
    if( IO32ReadFldAlign(ADAP_LCDIM_SPI, SPI_AUTO_VSYNC_EN)== SV_TRUE)
    {
        vDrvLcdimSpiCommandGen(E_SPI_PAGE_WRITE, 0);
    }
    */
    return;
}

void DRVCUST_PostOutVSyncISR()
{

}

void DRVCUST_PostDEEndISR(void)
{

}

void DRVCUST_VdoInISR()
{
    return;
}

UINT32 DRVCUST_RGBOfstRemap(UINT32 u4Value)
{
    return u4Value<<2;
}

UINT16 DRVCUST_Y2RMap(void)
{
    //HSI SPEC change , Always -16 For video mode / Special Mode , 
    //because of DEMO Mode Support
    return  0;
}

/* @ Auto NR default Initial setting
 *      1: low
 *      2: Middle
 */
UINT8 DRVCUST_AutoNRDefSet(void)
{
    return 2;
}

/* @ Auto NR noise level implement
 * @ return noise level
 */
UINT8 DRVCUST_NoiseLevel(void)
{
#if 0
    UINT8 bNoiseLevel = NM_INVALIDLEVEL;
    UINT8 bRFNMLevel;

    if((bGetVideoDecType(bDrvNRGetCurrentPath())==SV_VD_TVD3D)&&
        	 (bGetSignalType(bDrvNRGetCurrentPath())==SV_ST_TV))// TVD RF source
    {

        bRFNMLevel = bTvdSignalStrengthLevel(1);   //RF level 0 ~ 80

        if(bRFNMLevel > 75)
        {
            bNoiseLevel = NM_LEVEL1_3;
        }
        else if(bRFNMLevel > 69)
        {
            bNoiseLevel = NM_LEVEL2;
        }
        else if(bRFNMLevel > 59)
        {
            bNoiseLevel = NM_LEVEL3;
        }
        else if(bRFNMLevel > 55)
        {
            bNoiseLevel = NM_LEVEL4;
        }
        else
        {
            bNoiseLevel = NM_LEVEL5;
        }
    }
    else
    {
        bNoiseLevel = vDrvNMLevel();   //content NR, Noise level 0 ~ 4
    }
    
    return bNoiseLevel;
#endif
    return 0;
}

void DRVCUST_PANEL_GAMMA_REMAP(UINT32 u4GammaSel)
{
    UNUSED(u4GammaSel);
}

void DRVCUST_SetBlackLvlCtrl(UINT8 bPath)
{    
#if 0 // Below is SAMPLE CODE for function customization.
    UINT8 bLevel = GET_BLK_LVL(bPath);
    if (bPath == SV_VP_MAIN)
    {
        if(fgIsMainDVI())
        {
            SET_MATRIX_PED(SV_OFF);
            switch (bLevel)
            {
                case SV_OFF:
                    vSetHDMIRangeMode(SV_HDMI_RANGE_FORCE_FULL);
                    break;
                case SV_ON:
                    vSetHDMIRangeMode(SV_HDMI_RANGE_FORCE_LIMIT);
                    break;
                default:
                    vSetHDMIRangeMode(SV_HDMI_RANGE_FORCE_AUTO);
                    break;                
            }
        }
        else if (fgIsMainTvd3d() || fgIsMainYPbPr())
        {
            SET_MATRIX_PED(bLevel);
            vSetHDMIRangeMode(SV_HDMI_RANGE_FORCE_AUTO);
        }
        else
        {
            SET_MATRIX_PED(SV_OFF);
            vSetHDMIRangeMode(SV_HDMI_RANGE_FORCE_AUTO);
        }
    }
    else
    {
        SET_MATRIX_PED(SV_OFF);
        vSetHDMIRangeMode(SV_HDMI_RANGE_FORCE_AUTO);
    }
#endif
}

UINT32 DRVCUST_HDMIRange(UINT8 bPath)
{
    return (bDviIsVideoTiming() ? SV_HDMI_RANGE_LIMIT : SV_HDMI_RANGE_FULL);
}

UINT32 DRVCUST_GetOSMatrix709(UINT8 bPath)
{
    UINT32 u4Matrix709;

#if 1
    u4Matrix709 = GET_MATRIX_HD_709 && (wDrvVideoInputHeight(bPath) >= 720);
#else
    UINT32 u4ScrTiming = bDrvVideoGetSourceTypeTiming(bPath);
    u4Matrix709 = ((u4ScrTiming >= SOURCE_TYPE_TIMING_MM_SD_I) && (u4ScrTiming <= SOURCE_TYPE_TIMING_MM_1080IP_HD)) ? 
                    SV_FALSE : (GET_MATRIX_HD_709 && (wDrvVideoInputHeight(bPath) >= 720));
#endif
    return u4Matrix709;

}

UINT8 DRVCUST_MMAPPGetTiming(UINT8 bOutputTiming)
{
    if (sMMAppInfo.u1AppType == 0xFF) //Pretend for Netflix App
    {
        // Temp threshold
        bOutputTiming = (sMMAppInfo.u4BitRate < 100) ?  
            SOURCE_TYPE_TIMING_MM_720P_SD : SOURCE_TYPE_TIMING_MM_720P_HD;
    }

    LOG(5, "DRVCUST_MMAPPGetTiming %d %d\n", sMMAppInfo.u4BitRate, bOutputTiming);

    return (bOutputTiming);
}

extern OD_PARAM_T _ODParam;
void DRVCUST_SetODTable(void)
{
    //get OD table from flashPQ
    bApiFlashPqUpdateOD();

    //change OD table DMA, only support OD
    vDrvODSetDMA(OD_TABLE_CONFIG,OD_Table, sizeof(OD_Table));
    vDrvODMode(_ODParam.u1OD_ON);
    
    //channge OD table in Vsync
    //vApiRegisterVideoEvent(PE_EVENT_OD, SV_VP_MAIN, SV_ON);
}


void DRVCUST_SET_GAMMA(void)
{
    /* ============================= Sample code ===================================
    
    1. Read gamma data from EEPROM, maybe need some check flow
        => Example 8bit 6 control points with 12bit gamma data
        INT32 iCtlPos[6] =   {0x000, 0x020, 0x040, 0x080, 0x0C0, 0x0FF};
        INT32 iCtlValue_R[6] = {0x013, 0x233, 0x3E5, 0x7C4, 0xB20, 0xEF4};

    2. Interplaotion to 257 red gamma table
        static INT32 OutputLut[257];
        vDrvGammaInterpolation(6, iCtlPos, iCtlValue_R, OutputLut);        

    3. Copy 257 data to u2GammaDecodedTable[] 
        
    ============================= Sample code End ===================================*/        
}
UINT8 DRVCUST_GET_CTMode(void)
{
      return FALSE;
}

void DRVCUST_SET_YGAMMA(UINT32 i1Mode)
{

}

void DRVCUST_AutoColorTempUpdate(AUTO_COLORTEMP_SETS_INFO_T *aCTInfo)
{

}

void DRVCUST_MMAPProc(void)
{
    if (sMMAppInfo.u1AppType == 0xFF) /*Pretend for Netflix App */
    {
        //Update new QtyTbl
        bApiVideoProc(SV_VP_MAIN, PE_ARG_3D_NR);
        bApiVideoProc(SV_VP_MAIN, PE_ARG_NR);
        bApiVideoProc(SV_VP_MAIN, PE_ARG_MPEG_NR);
        bApiVideoProc(SV_VP_MAIN, PE_ARG_SHARPNESS);
    }
}

void DRVCUST_GetLcdimPanelOrder(UINT8 *u1PanelOrder, UINT8 u1BlockNum)
{
	UINT8 bIndex;
	UINT16 i;
    bIndex = GetCurrentPanelIndex();

    if(bIndex == PANEL_EPI_LGDV12GIPFHD120)          // 1 * 12, LGD protocal
    {
        UINT8 u1PanelOrderCust[12] = {0, 2, 4, 6, 8, 10, 11, 9, 7, 5, 3, 1};      // MTK IC HW Sequence            
        x_memcpy(u1PanelOrder, u1PanelOrderCust, sizeof(u1PanelOrderCust));
    }   
    else if(bIndex == PANEL_LG_47_EUH_PR)          // 8 * 2, AMS protocal
    {
        UINT8 u1PanelOrderCust[16] = {0, 15, 1, 14, 2, 13, 3, 12, 4, 11, 5, 10, 6, 9, 7, 8};      // MTK IC HW Sequence
        x_memcpy(u1PanelOrder, u1PanelOrderCust, sizeof(u1PanelOrderCust));
    }       
    else
    {
        for(i = 0; i < u1BlockNum; i++)
        {
            u1PanelOrder[i] = i;
        }
    }        
    
    LOG(0, "Panel Order = ");
    
    for(i = 0; i < u1BlockNum; i++)
    {
        LOG(0, "%d  ", u1PanelOrder[i]);
    }    
}

void DRVCUST_GetLcdimPanelDimGain(UINT8 *u1PanelDimGain, UINT8 u1BlockNum)
{
	UINT8 bIndex;
	UINT16 i;
    bIndex = GetCurrentPanelIndex();

    if(bIndex == PANEL_EPI_LGDV12GIPFHD120)          // 1 * 12, LGD protocal
    {
        UINT8 u1PanelDimGainCust[12] = {0x98, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x98};
        x_memcpy(u1PanelDimGain, u1PanelDimGainCust, sizeof(u1PanelDimGainCust));
    }
    else if(bIndex == PANEL_LG_47_EUH_PR)          // 8 * 2, AMS protocal
    {
        UINT8 u1PanelDimGainCust[16] = {0xFF, 0xFF, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0xFF, 0xFF};
        x_memcpy(u1PanelDimGain, u1PanelDimGainCust, sizeof(u1PanelDimGainCust));
    }       
    else
    {
        for(i = 0; i < u1BlockNum; i++)
        {
            u1PanelDimGain[i] = 0x80;   //0x80 means gain 1
        }
    }        
    
    LOG(0, "Panel Dim Gain = ");
    
    for(i = 0; i < u1BlockNum; i++)
    {
        LOG(0, "%d  ", u1PanelDimGain[i]);
    }    
}

void DRVCUST_PqModeChange(UINT8 bPath)
{
    UINT32 i;
    
    for (i=0; i < (UINT8)PE_ARG_NS; i++)
    {
        switch (i)
        {
            // List all the quality items that do not update after mode change done.
            //  the back light is not relative to video timing, nor main/pip change.
            case PE_ARG_ADAPTIVE_BACK_LIGHT:
                break;
            default:
                UNUSED(bApiVideoProc(bPath, (PE_ARG_TYPE)i));
                break;
        }
    }
}
#ifdef SCE_BY_SOURCE
void DRVCUST_vMappingSCETiming(void)
{
	UINT8 bCustomSCETypTmg, bSCEBegin, bSCEEnd;
	bCustomSCETypTmg = 0;
	do
	{
		//update SCE by source group
		bSCEBegin = READ_Sce_SRC_GROUP(bCustomSCETypTmg, QTY_IDX_RANGE_BEGIN);
		bSCEEnd = READ_Sce_SRC_GROUP(bCustomSCETypTmg, QTY_IDX_RANGE_END);
		
		if(bSCEBegin <= SOURCE_TYPE_TIMING_MAX && bSCEEnd <= SOURCE_TYPE_TIMING_MAX)
		{
			WRITE_Sce_SRC_GROUP(bCustomSCETypTmg, QTY_IDX_RANGE_BEGIN,
			   bSrcTimingInverseTbl[bSCEBegin]);
		
			WRITE_Sce_SRC_GROUP(bCustomSCETypTmg, QTY_IDX_RANGE_END,
			   bSrcTimingInverseTbl[bSCEEnd]);
		
			bCustomSCETypTmg++ ;
		}
		else
		{
			LOG(0, "API_PEUI Error: _vMappingSrcTiming on bBegin/bEnd <= SOURCE_TYPE_TIMING_MAX");
			return;
		}
	}while((bSCEBegin != SOURCE_TYPE_TIMING_MAX) && (bSCEEnd !=SOURCE_TYPE_TIMING_MAX));

}
UINT16 DRVCUST_wApiFlashPqGetSceIndexBySrcTiming(UINT16 u1SrcTypTmg)
{
	UINT16 bCustomSrcTypTmg;

	for(bCustomSrcTypTmg=0;
		READ_Sce_SRC_GROUP(bCustomSrcTypTmg,QTY_IDX_RANGE_BEGIN)!=
			SOURCE_TYPE_TIMING_MAX
		&&READ_Sce_SRC_GROUP(bCustomSrcTypTmg,QTY_IDX_RANGE_END)!=
			SOURCE_TYPE_TIMING_MAX;
		bCustomSrcTypTmg++)
	{
		if(READ_Sce_SRC_GROUP(bCustomSrcTypTmg,QTY_IDX_RANGE_BEGIN)
			>READ_Sce_SRC_GROUP(bCustomSrcTypTmg,QTY_IDX_RANGE_END))
		{
        	WRITE_Sce_SRC_GROUP(bCustomSrcTypTmg,QTY_IDX_RANGE_END,
            READ_Sce_SRC_GROUP(bCustomSrcTypTmg,QTY_IDX_RANGE_BEGIN));

		}
		if((u1SrcTypTmg>=READ_Sce_SRC_GROUP(bCustomSrcTypTmg,QTY_IDX_RANGE_BEGIN))
			&&(u1SrcTypTmg<=READ_Sce_SRC_GROUP(bCustomSrcTypTmg,QTY_IDX_RANGE_END)))
		{
			return bCustomSrcTypTmg;
		}
	}
	return SOURCE_TYPE_TIMING_MAX;
}
#endif
#ifdef SUPPORT_LCDIM_AVG_DEMO
void DRVCUST_LcDimBlkInit(UINT32 u4BlockNumH, UINT32 u4BlockNumV)
{
    UINT32 h, v,s;

	if(u4BlockNumH==0||u4BlockNumH>100||u4BlockNumV==0||u4BlockNumV>8)
	{
		LOG(0,"[BlockNumH:1~100,BlockNumV:1~8] /n");
		return;
	}
	if(fgIsSWRegInit()==SV_FALSE)
	{
		return;
	}
   if(bDrvLcDimSetPanelInfo(u4BlockNumH,u4BlockNumV,0) == SV_FAIL)        
    {
    	LOG(0,"{%s %d}\n", __FUNCTION__, __LINE__);
        return ;
    }
    u1BlockNumV=IO32ReadFldAlign(LD2_MODE_00,C_LD_BLK_VERT_NUM);
    u1BlockNumH=IO32ReadFldAlign(LD2_MODE_00,C_LD_BLK_HOR_NUM);
	u1BlkStartNumIdx=u1BlockNumH*u1BlockNumV/32+1;
    vDrvLcdimQualityHWInit();
    if (bLcDimBlkInfo == NULL)
    {
        bLcDimBlkInfo = (UINT16*)x_mem_alloc(u1BlkStartNumIdx*32);
    }
	if (pu1BLBlockDataTbl == NULL)
	{
        pu1BLBlockDataTbl = (UINT16*)x_mem_alloc(u1BlockNumH*u1BlockNumV);
    }

}
UINT8 DRVCUST_Get_BLBlockData(UINT8 * pu1BLBlockTbl)
{
	UINT8 u1BlkNum;
	u1BlkNum=u1BlockNumH*u1BlockNumV;
	
	LOG(2,"{%s %d}\n", __FUNCTION__, __LINE__);
    if (pu1BLBlockDataTbl == NULL)
    {
		Printf("++pu1BLBlockDataTbl not inited\n");
        return 0;
    }
	x_memcpy(pu1BLBlockTbl,pu1BLBlockDataTbl,u1BlkNum);

	return 1;
}
void vDrvUpdateLcDimBlockContentInfo()
{
	static UINT8 u1BlkStaNumIdx=0;

	if((u1BlockNumV!=IO32ReadFldAlign(LD2_MODE_00,C_LD_BLK_VERT_NUM))||
		(u1BlockNumH!=IO32ReadFldAlign(LD2_MODE_00,C_LD_BLK_HOR_NUM)))
    {
		u1BlockNumV=IO32ReadFldAlign(LD2_MODE_00,C_LD_BLK_VERT_NUM);
		u1BlockNumH=IO32ReadFldAlign(LD2_MODE_00,C_LD_BLK_HOR_NUM);			
		u1BlkStartNumIdx=u1BlockNumH*u1BlockNumV/32+1;
		x_mem_free(pu1BLBlockDataTbl);
		pu1BLBlockDataTbl=NULL;
		x_mem_free(bLcDimBlkInfo);
		bLcDimBlkInfo=NULL;
	}
    if (bLcDimBlkInfo == NULL)
    {
        bLcDimBlkInfo = (UINT16*)x_mem_alloc(u1BlkStartNumIdx*32);
    }
	if (pu1BLBlockDataTbl == NULL)
	{
        pu1BLBlockDataTbl = (UINT16*)x_mem_alloc(u1BlockNumH*u1BlockNumV);
    }
	if(u1BlkStaNumIdx==u1BlkStartNumIdx)
	{
		x_memcpy(pu1BLBlockDataTbl,bLcDimBlkInfo,(u1BlockNumH*u1BlockNumV));
		u1BlkStaNumIdx=0;
	}
	else
	{
		if(u4DrvLcdimReadBlkInfoV3(2,(UINT8 *)bLcDimBlkInfo,u1BlkStaNumIdx)==
SV_FALSE)
		{
		    return ;//keep previous idx
		}
		u1BlkStaNumIdx++;
	}
}

#endif

#ifdef CUST_PQ_SPLIT_DEMO
UINT32 DRVCUST_DemoRegion(void)
{  
    return DEMO_PQ_ON_RIGHT;
}

UINT32 DRVCUST_DemoModule(void)
{
    return (DEMO_3DNR | DEMO_TDSHARP | DEMO_COLOR | DEMO_ECTI );
}
#endif
void DRVCUST_SendEvent(E_CUST_EVENT eCustEvent, UINT8 u1Option)
{

}

void DRVCUST_SetRealCinema(UINT8 bRealCinema)
{
    if(IO32ReadFldAlign(DRVCUST_REAL_CINEMA, REAL_CINEMA) != bRealCinema)
    (
        vIO32WriteFldAlign(DRVCUST_REAL_CINEMA, bRealCinema, REAL_CINEMA);
        vDrvCalPanelFrameRate(wDrvVideoGetVTotal(SV_VP_MAIN), bDrvVideoGetRefreshRate(SV_VP_MAIN));
    )
}

UINT8 DRVCUST_GetRealCinema(void)
{
    return bIsRealCinema;
}
