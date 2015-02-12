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
 * $Date: 2015/02/12 $
 * $RCSfile: fbm_pool_config.c,v $
 * $Revision: #9 $
 *
 *---------------------------------------------------------------------------*/

/** @file fbm_pool_config.c
 *  Brief of file fbm_pool_config.c.
 *  FBM Pool Management
 */

//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------

#include "x_lint.h"
#include "fbm_pool.h"
#include "fbm_pool_config.h"
#include "fbm_debug.h"
#include "vdo_misc.h"
#include "../tve/tve_if.h"

#ifdef CC_SRM_ON
#include "srm_drvif.h"
#endif

LINT_EXT_HEADER_BEGIN
#include "source_table.h"
#include "vdp_drvif.h"
#include "x_assert.h"
#include "x_hal_5381.h"
#include "drvcust_if.h"
#include "x_mid.h"
#ifdef CC_SUPPORT_PIPELINE
#include "drv_tvd.h"
#endif
LINT_EXT_HEADER_END

#include "drv_display.h"
#include "drv_tdtv_drvif.h"

#undef min   
#define min(x, y) ((x < y) ? x : y)

extern UINT32 SRM_GetScposMode(UINT32 u4VdpId);
extern UINT32 SRM_GetMddiMode(UINT32 u4VdpId);
extern UINT32 SRM_GetTDCMode(UINT32 u4VdpId);
extern UINT32 SRM_GetScposHPDSize(UINT32 u4VdpId);

#if defined(CC_EDB_FBM_SHARE_MEM)
extern UINT32 _gu4EDBSize;
extern UINT32 _gu4EDBAddr;
#endif
#if defined(CC_TRUSTZONE_SUPPORT) && defined(CC_SVP_SUPPORT)
extern FBM_POOL_T _arPoolTZ[];
#endif
//---------------------------------------------------------------------------
// Static variables
//---------------------------------------------------------------------------

static FBM_POOL_TYPE_T _eCurrentScposMain = FBM_POOL_TYPE_SCPOS_MAIN;
static FBM_POOL_TYPE_T _eCurrentScposPip = FBM_POOL_TYPE_SCPOS_PIP;
static FBM_POOL_TYPE_T _eCurrentMddiMain = FBM_POOL_TYPE_MDDI_MAIN;
static FBM_POOL_TYPE_T _eCurrentMddiPip = FBM_POOL_TYPE_MDDI_PIP;
static FBM_POOL_TYPE_T _eCurrentMjc = FBM_POOL_TYPE_MJC_1;
static FBM_POOL_TYPE_T _eCurrentTdc = FBM_POOL_TYPE_TDC;
static FBM_POOL_TYPE_T _eCurrentVbi = FBM_POOL_TYPE_VBI;

// NPTV dynamically-allocated FBM
static UINT32 _fbm_u4NptvBeAddr;
#if defined(CC_MT5890)
static UINT32 _fbm_u4NptvSubAddr;
#endif
#if defined(CC_MT5882)
static UINT32 _fbm_u4NptvSubPopAddr;
static UINT32 _fbm_u4NptvSubPipAddr;
#endif
typedef enum
{
    FBM_MODE_1080I,
    FBM_MODE_576I,
    FBM_MODE_1080P_422,
    FBM_MODE_1080P_444
} FBM_POOL_SCENARIO_T;

typedef struct
{
    UINT8 u1fgPIP;
    UINT8 u1fgPOP;
    UINT8 u1fgSingle;
    UINT8 u1mode[2];
    UINT8 u1Interlace[2];
    UINT8 u1IsMPEG[2];
    UINT8 u1IsTDC[2];
    UINT32 u4Resolution[2];
    UINT32 u4InputWidth[2];
    UINT32 u4InputHeight[2];
} FBM_AUTO_INC_ENV_T;

typedef struct
{
    UINT32 u4MainPipMaxSize;
    UINT32 u4MainPopMaxSize;
    UINT32 u4SubPipMaxSize;
    UINT32 u4SubPopMaxSize;
    UINT32 u4MainPipMaxEnd;
    UINT32 u4MainPopMaxEnd;
    UINT32 u4SubPipMaxEnd;
    UINT32 u4SubPopMaxEnd;
    UINT8 u1MainPipMaxMode;
    UINT8 u1MainPopMaxMode;
    UINT8 u1SubPipMaxMode;
    UINT8 u1SubPopMaxMode;
} FBM_VIDEO_MAX_T;

FBM_POOL_T _FBM_NPTV_VIDEO_CONF[FBM_TYPE_ENV_MAX][FBM_VIDEO_AGENT_MAX];
/* get the max range of main and sub video agents */
FBM_VIDEO_MAX_T _FBM_NPTV_VideoRange;

//For ATVPVR to make scaler dram address fixed.
static UINT8  fgFBMFixedScalerDram = FALSE; 

// for Pscan testing
BOOL fgFBM_PscanTest=0;
BOOL fgFBM_PscanTestSub=0;

void FBM_SetPscanTestMode(BOOL fgIsTestMode)
{
    //Printf("---------FBM_SetPscanTestMode: %x----------\n",fgIsTestMode);
    fgFBM_PscanTest=fgIsTestMode;
}

void FBM_SetPscanTestModeSubPIP(BOOL fgIsTestMode)
{
    //Printf("---------FBM_SetPscanTestMode: %x----------\n",fgIsTestMode);
    fgFBM_PscanTestSub=fgIsTestMode;
}

void FBM_FixedScalerDram(UINT8 fgOnOff)
{
    fgFBMFixedScalerDram = fgOnOff;
}

UINT8 FBM_GetFixedScalerDram(void)
{
	return fgFBMFixedScalerDram;
}

static FBM_AUTO_INC_ENV_T* _FbmGetEnv(FBM_AUTO_INC_ENV_T *env)
{
    UINT8 u1fgPIP = SRM_IsPipVideo();
    UINT8 u1fgPOP = SRM_IsPopVideo();
    UINT8 u1fgSingle = !(u1fgPIP || u1fgPOP);
    UINT8 u1mode[2];
    UINT8 u1Interlace[2];
    UINT8 u1IsMPEG[2] = {0, 0};
    UINT8 u1IsTDC[2] = {0, 0};
    SRM_VDP_INFO_T* prVdpInfo[2];
    UINT32 u4Resolution[2];

    prVdpInfo[VDP_1] = SRM_GetVdpInfo(VDP_1);
    prVdpInfo[VDP_2] = SRM_GetVdpInfo(VDP_2);

    u4Resolution[VDP_1] = prVdpInfo[VDP_1]->u4Resolution;
    if (prVdpInfo[VDP_1]->u4Interlace == 1)
    {
        u1Interlace[VDP_1] = 1;
        if (prVdpInfo[VDP_1]->u4Resolution < SRM_VDP_720HD_RESOLUTION)
        {
            u1mode[VDP_1] = FBM_MODE_576I;
        }
        else
        {
            u1mode[VDP_1] = FBM_MODE_1080I;
        }
    }
    else
    {
        u1Interlace[VDP_1] = 0;
        if (prVdpInfo[VDP_1]->u4VgaTiming == 1)
        {
            u1mode[VDP_1] = FBM_MODE_1080P_444;
        }
        else
        {
            u1mode[VDP_1] = FBM_MODE_1080P_422;
        }
    }

    u4Resolution[VDP_2] = prVdpInfo[VDP_2]->u4Resolution;
    if (prVdpInfo[VDP_2]->u4Interlace == 1)
    {
        u1Interlace[VDP_2] = 1;
        if (prVdpInfo[VDP_2]->u4Resolution < SRM_VDP_720HD_RESOLUTION)
        {
            u1mode[VDP_2] = FBM_MODE_576I;
        }
        else
        {
            u1mode[VDP_2] = FBM_MODE_1080I;
        }
    }
    else
    {
        u1Interlace[VDP_2] = 0;
        if (prVdpInfo[VDP_2]->u4VgaTiming == 1)
        {
            u1mode[VDP_2] = FBM_MODE_1080P_444;
        }
		
        else
        {
            u1mode[VDP_2] = FBM_MODE_1080P_422;
        }
    }

    /**
     * manual scan mode workaround: during manual scan, these scpos events
     * will confuse the fbm configuration.
     */
    if (SRM_IsScanMode(VDP_1))
    {
        u1mode[VDP_1] = FBM_MODE_576I;
    }
    if (SRM_IsScanMode(VDP_2))
    {
        u1mode[VDP_2] = FBM_MODE_576I;
    }

    if (prVdpInfo[VDP_1]->u4Source == (UINT32)VSS_DTV)
    {
        u1IsMPEG[VDP_1] = 1;
    }
    if (prVdpInfo[VDP_2]->u4Source == (UINT32)VSS_DTV)
    {
        u1IsMPEG[VDP_2] = 1;
    }

    if ((prVdpInfo[VDP_1]->u4Source == (UINT32)VSS_ATV) ||
            (prVdpInfo[VDP_1]->u4Source == (UINT32)VSS_ATD) ||
            (prVdpInfo[VDP_1]->u4Source == (UINT32)VSS_SV) ||
            (prVdpInfo[VDP_1]->u4Source == (UINT32)VSS_CVBS) ||
            (prVdpInfo[VDP_1]->u4Source == (UINT32)VSS_SCART))
    {
        u1IsTDC[VDP_1] = 1;
    }
    if ((prVdpInfo[VDP_2]->u4Source == (UINT32)VSS_ATV) ||
            (prVdpInfo[VDP_2]->u4Source == (UINT32)VSS_ATD) ||
            (prVdpInfo[VDP_2]->u4Source == (UINT32)VSS_SV) ||
            (prVdpInfo[VDP_2]->u4Source == (UINT32)VSS_CVBS) ||
            (prVdpInfo[VDP_2]->u4Source == (UINT32)VSS_SCART))
    {
        u1IsTDC[VDP_2] = 1;
    }

    env->u1fgPIP = u1fgPIP;
    env->u1fgPOP = u1fgPOP;
    env->u1fgSingle = u1fgSingle;
    env->u1mode[0] = u1mode[0];
    env->u1mode[1] = u1mode[1];
    env->u1Interlace[0] = u1Interlace[0];
    env->u1Interlace[1] = u1Interlace[1];
    env->u1IsMPEG[0] = u1IsMPEG[0];
    env->u1IsMPEG[1] = u1IsMPEG[1];
    env->u1IsTDC[0] = u1IsTDC[0];
    env->u1IsTDC[1] = u1IsTDC[1];
    env->u4Resolution[0] = u4Resolution[0];
    env->u4Resolution[1] = u4Resolution[1];
    env->u4InputWidth[0]  = prVdpInfo[0]->u4InputWidth;
    env->u4InputWidth[1]  = prVdpInfo[1]->u4InputWidth;
    env->u4InputHeight[0] = prVdpInfo[0]->u4InputHeight;
    env->u4InputHeight[1] = prVdpInfo[1]->u4InputHeight;
    return env;
}


//---------------------------------------------------------------------------
// Public functions
//---------------------------------------------------------------------------
#if defined(CC_DYNAMIC_FBMSRM_CONF)
static UINT32 _srmfbm_conf_value = FBM_MEM_CFG_MT5890_3DTV;
static UINT32 _srmfbm_chb_conf_value = FBM_MEM_CFG_MT5890_3DTV_CHB;
static UINT32 _srmfbm_chc_conf_value = FBM_MEM_CFG_MT5890_3DTV_CHC;

void SRMFBM_SetConf(UINT32 conf)
{
    _srmfbm_conf_value = conf;
}
UINT32 SRMFBM_GetConf(void)
{
    return _srmfbm_conf_value;
}
void SRMFBM_CHB_SetConf(UINT32 conf)
{
    _srmfbm_chb_conf_value = conf;
}
UINT32 SRMFBM_CHB_GetConf(void)
{
    return _srmfbm_chb_conf_value;
}
void SRMFBM_CHC_SetConf(UINT32 conf)
{
    _srmfbm_chc_conf_value = conf;
}
UINT32 SRMFBM_CHC_GetConf(void)
{
    return _srmfbm_chc_conf_value;
}


#else
UINT32 SRMFBM_GetConf(void)
{
    return DRVCUST_OptGet(eFbmMemSize);
}
#endif

#if defined(CC_MT5882)

#if !defined(CC_TVP_SUPPORT)
static VOID _FbmConfigurePool_MT5882_SCPOS_MAIN(POOL_LIST_T* prPoolList, POOL_ARRANGE_INFO_T* prPoolArrangeInfo)
{
    FBM_POOL_TYPE_T u4AheadType = FBM_POOL_TYPE_SCPOS_MAIN, u4BehindType, u4VdoBehindType = FBM_POOL_TYPE_NS;
    UNUSED(u4AheadType);
    UNUSED(u4BehindType);
    UNUSED(u4VdoBehindType);
    // arrange root pool ---------------------------------------------------

    // DMX | MPEG | PIP | MDDI | DSP

    //----- DMX-----
    prPoolList[FBM_POOL_TYPE_DMX].u4Inherit = FBM_POOL_ROOT;
    prPoolList[FBM_POOL_TYPE_DMX].u4PoolSize = FBM_DMX1_SIZE;
    prPoolArrangeInfo[FBM_POOL_TYPE_DMX].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_DMX].ePool1 = FBM_POOL_TYPE_TOTAL;   
#if defined(CC_TRUSTZONE_SUPPORT)&& defined(CC_SVP_SUPPORT)
    prPoolList[FBM_POOL_TYPE_DMX].u4PoolSize = FBM_ALIGN_MASK((0x300000), FBM_DMX_SIZE_ALIGMENT);
#endif

    //----- MPEG-----
    prPoolList[FBM_POOL_TYPE_MPEG].u4Inherit = FBM_POOL_ROOT;
    prPoolList[FBM_POOL_TYPE_MPEG].u4Mode = (FBM_POOL_MODE_MPEG_DBK | FBM_POOL_MODE_SHARE);
    prPoolList[FBM_POOL_TYPE_MPEG].u4PoolSize = FBM_MPEG_Y_SIZE; 
    prPoolArrangeInfo[FBM_POOL_TYPE_MPEG].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_MPEG].ePool1 = FBM_POOL_TYPE_DMX;

    //----- SCPOS MAIN-----
    prPoolList[FBM_POOL_TYPE_SCPOS_MAIN].u4Inherit = FBM_POOL_ROOT;
#if defined(CC_SUPPORT_SCART_OUT_IN_MMP)
    // allocate 7MB for sub modules, no consider flip/mirror
    prPoolList[FBM_POOL_TYPE_SCPOS_MAIN].u4PoolSize = 0x2F00000;
#endif

    if (SRMFBM_GetConf() == FBM_MEM_CFG_MT5882_3DTV)
    {
        prPoolList[FBM_POOL_TYPE_SCPOS_MAIN].u4PoolSize = FBM_SCPOS_MAIN_SIZE; //main 1080i 422 0xe9c0000*2+sub 0x880000
    }
    else // for ic-verification
    {
        prPoolList[FBM_POOL_TYPE_SCPOS_MAIN].u4PoolSize = 0x2400000 + 0x80000; /* add extra 500K to avoid sub buffer overlapped with dsp in SLT environment */
    }

    prPoolArrangeInfo[FBM_POOL_TYPE_SCPOS_MAIN].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_SCPOS_MAIN].ePool1 = FBM_POOL_TYPE_MPEG;
    u4AheadType = FBM_POOL_TYPE_SCPOS_MAIN;
#if defined(CC_IC_VERIFY_FBM_MODE)
    prPoolList[FBM_POOL_TYPE_SCPOS_MAIN].u4PoolSize = 0x2b00000;
#endif
	prPoolList[FBM_POOL_TYPE_FEEDER].u4Inherit = FBM_POOL_INHERIT;
	prPoolList[FBM_POOL_TYPE_FEEDER].u4PoolSize = FBM_FEEDER1_SIZE;
	prPoolArrangeInfo[FBM_POOL_TYPE_FEEDER].eMode = FBM_POOL_ARRANGE_ALLIGN_END;
	prPoolArrangeInfo[FBM_POOL_TYPE_FEEDER].ePool1 = FBM_POOL_TYPE_SCPOS_MAIN;

    #if defined(CC_FBM_TWO_FBP_SHARED_WITH_DFB) || defined(CC_VOMX_TV_COEXIST) 
    if (u4VdoBehindType == FBM_POOL_TYPE_NS)
    {
        u4VdoBehindType = FBM_POOL_TYPE_DMX2;
    }
    prPoolList[FBM_POOL_TYPE_DMX2].u4Inherit = FBM_POOL_ROOT;
    {
        prPoolList[FBM_POOL_TYPE_DMX2].u4PoolSize = FBM_ALIGN_MASK((FBM_DMX2_SIZE), FBM_DMX_SIZE_ALIGMENT);

#if defined(CC_TRUSTZONE_SUPPORT)&& defined(CC_SVP_SUPPORT)
		prPoolList[FBM_POOL_TYPE_DMX2].u4PoolSize = 0;
#endif
    }
    prPoolArrangeInfo[FBM_POOL_TYPE_DMX2].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_DMX2].ePool1 = u4AheadType;
    u4AheadType = FBM_POOL_TYPE_DMX2;

    prPoolList[FBM_POOL_TYPE_MPEG2].u4Inherit = FBM_POOL_ROOT;
    //prPoolList[FBM_POOL_TYPE_MPEG2].u4Mode = (FBM_POOL_MODE_MPEG_DBK | FBM_POOL_MODE_SHARE);
    #if defined(CC_VOMX_TV_COEXIST)
    prPoolList[FBM_POOL_TYPE_MPEG2].u4Mode = (FBM_POOL_MODE_MPEG_DBK);
    //prPoolList[FBM_POOL_TYPE_MPEG2].u4PoolSize = 0x2F00000; // 23.5Mx2
    prPoolList[FBM_POOL_TYPE_MPEG2].u4PoolSize = FBM_MPEG_2_SIZE; // Jason rewrite
    #else
    prPoolList[FBM_POOL_TYPE_MPEG2].u4Mode = (FBM_POOL_MODE_MPEG_DBK | FBM_POOL_MODE_SHARE);
    prPoolList[FBM_POOL_TYPE_MPEG2].u4PoolSize = FBM_MPEG_2_SIZE; // Jason rewrite
    #endif
    prPoolArrangeInfo[FBM_POOL_TYPE_MPEG2].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_MPEG2].ePool1 = u4AheadType;
    u4AheadType = FBM_POOL_TYPE_MPEG2;
   #if 0
    prPoolList[FBM_POOL_TYPE_FEEDER2].u4Inherit = FBM_POOL_ROOT;
    prPoolList[FBM_POOL_TYPE_FEEDER2].u4PoolSize = FBM_FEEDER_POOL_SIZE;
    prPoolArrangeInfo[FBM_POOL_TYPE_FEEDER2].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_FEEDER2].ePool1 = u4AheadType;
    u4AheadType = FBM_POOL_TYPE_FEEDER2;
	#endif
#endif
   
#if defined(CC_FBM_FOUR_INST_SUPPORT)
    prPoolList[FBM_POOL_TYPE_DMX3].u4Inherit = FBM_POOL_ROOT;
    prPoolList[FBM_POOL_TYPE_DMX3].u4PoolSize = FBM_DMX_H264_SIZE;
    prPoolArrangeInfo[FBM_POOL_TYPE_DMX3].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_DMX3].ePool1 = u4AheadType;
    u4AheadType = FBM_POOL_TYPE_DMX3;

	prPoolList[FBM_POOL_TYPE_DMX4].u4Inherit = FBM_POOL_ROOT;
    prPoolList[FBM_POOL_TYPE_DMX4].u4PoolSize = FBM_DMX_H264_SIZE;
    prPoolArrangeInfo[FBM_POOL_TYPE_DMX4].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_DMX4].ePool1 = u4AheadType;
    u4AheadType = FBM_POOL_TYPE_DMX4;

	prPoolList[FBM_POOL_TYPE_MPEG3].u4Inherit = FBM_POOL_ROOT;
    prPoolList[FBM_POOL_TYPE_MPEG3].u4Mode = (FBM_POOL_MODE_MPEG_DBK);
    prPoolList[FBM_POOL_TYPE_MPEG3].u4PoolSize = 0x1780000;
    prPoolArrangeInfo[FBM_POOL_TYPE_MPEG3].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_MPEG3].ePool1 = u4AheadType;
    u4AheadType = FBM_POOL_TYPE_MPEG3;

	prPoolList[FBM_POOL_TYPE_MPEG4].u4Inherit = FBM_POOL_ROOT;
    prPoolList[FBM_POOL_TYPE_MPEG4].u4Mode = (FBM_POOL_MODE_MPEG_DBK);
    prPoolList[FBM_POOL_TYPE_MPEG4].u4PoolSize = 0x1780000;
    prPoolArrangeInfo[FBM_POOL_TYPE_MPEG4].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_MPEG4].ePool1 = u4AheadType;
    u4AheadType = FBM_POOL_TYPE_MPEG4;

	prPoolList[FBM_POOL_TYPE_FEEDER3].u4Inherit = FBM_POOL_ROOT;
    prPoolList[FBM_POOL_TYPE_FEEDER3].u4PoolSize = FBM_FEEDER_POOL_SIZE;
    prPoolArrangeInfo[FBM_POOL_TYPE_FEEDER3].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_FEEDER3].ePool1 = u4AheadType;
    u4AheadType = FBM_POOL_TYPE_FEEDER3;

	prPoolList[FBM_POOL_TYPE_FEEDER4].u4Inherit = FBM_POOL_ROOT;
    prPoolList[FBM_POOL_TYPE_FEEDER4].u4PoolSize = FBM_FEEDER_POOL_SIZE;
    prPoolArrangeInfo[FBM_POOL_TYPE_FEEDER4].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_FEEDER4].ePool1 = u4AheadType;
    u4AheadType = FBM_POOL_TYPE_FEEDER4;
#endif
#if defined(CC_FBM_SECURE_FEEDER)
		prPoolList[FBM_POOL_TYPE_SECURE_FEEDER].u4PoolSize = FBM_SECURE_FEEDER_2K_SIZE;
		prPoolList[FBM_POOL_TYPE_SECURE_FEEDER].u4Inherit = FBM_POOL_ROOT;			 
		prPoolArrangeInfo[FBM_POOL_TYPE_SECURE_FEEDER].eMode = FBM_POOL_ARRANGE_AFTER; 
		prPoolArrangeInfo[FBM_POOL_TYPE_SECURE_FEEDER].ePool1 = u4AheadType;			 
		u4AheadType = FBM_POOL_TYPE_SECURE_FEEDER;
#endif

    // pq tool
    prPoolList[FBM_POOL_TYPE_PQ_TOOL].u4Inherit = FBM_POOL_ROOT;
    prPoolList[FBM_POOL_TYPE_PQ_TOOL].u4PoolSize = FBM_PQ_TOOL_POOL_SIZE; // A3 needs 8MB for OD dump
    
    prPoolArrangeInfo[FBM_POOL_TYPE_PQ_TOOL].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_PQ_TOOL].ePool1 = u4AheadType;
    u4AheadType = FBM_POOL_TYPE_PQ_TOOL;
    if (u4VdoBehindType == FBM_POOL_TYPE_NS)
    {
        u4VdoBehindType = FBM_POOL_TYPE_PQ_TOOL;
    }
#if defined(CC_53XX_FACE_DETECTION)
    prPoolList[FBM_POOL_TYPE_FACEDET].u4Inherit = FBM_POOL_ROOT;
    prPoolList[FBM_POOL_TYPE_FACEDET].u4PoolSize = FBM_FD_POOL_SIZE;
    prPoolArrangeInfo[FBM_POOL_TYPE_FACEDET].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_FACEDET].ePool1 = u4AheadType;
    u4AheadType = FBM_POOL_TYPE_FACEDET;
#endif

    // vbi
    prPoolList[FBM_POOL_TYPE_VBI].u4Inherit = FBM_POOL_ROOT;
    prPoolArrangeInfo[FBM_POOL_TYPE_VBI].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_VBI].ePool1 = u4AheadType;
    u4AheadType = FBM_POOL_TYPE_VBI;

#if defined(CC_FBM_SUPPORT_SWDMX)||defined(CC_FBM_SUPPORT_LITE_SWDMX)
    prPoolList[FBM_POOL_TYPE_SWDMX].u4Inherit = FBM_POOL_ROOT;
    prPoolList[FBM_POOL_TYPE_SWDMX].u4PoolSize = FBM_SWDMX_POOL_SIZE;
    prPoolArrangeInfo[FBM_POOL_TYPE_SWDMX].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_SWDMX].ePool1 = u4AheadType;
    u4AheadType = FBM_POOL_TYPE_SWDMX;
#endif
#if defined(CC_FBM_SUPPORT_PNG)		
		    prPoolList[FBM_POOL_TYPE_PNG].u4Inherit = FBM_POOL_ROOT;
			prPoolList[FBM_POOL_TYPE_PNG].u4PoolSize = FBM_PNG_POOL_SIZE;
    		prPoolArrangeInfo[FBM_POOL_TYPE_PNG].eMode = FBM_POOL_ARRANGE_AFTER;
    		prPoolArrangeInfo[FBM_POOL_TYPE_PNG].ePool1 = u4AheadType;
    		u4AheadType = FBM_POOL_TYPE_PNG;
#endif
#if defined(CC_FBM_3D_OSD)||defined(CC_MHP_SUPPORT)
	prPoolList[FBM_POOL_TYPE_JPG_VDP].u4Inherit = FBM_POOL_ROOT;
    prPoolList[FBM_POOL_TYPE_JPG_VDP].u4PoolSize = FBM_JPEVDP_POOL_SIZE;
    prPoolArrangeInfo[FBM_POOL_TYPE_JPG_VDP].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_JPG_VDP].ePool1 = u4AheadType;
	u4AheadType = FBM_POOL_TYPE_JPG_VDP;
#endif
#if defined(CC_SUPPORT_VSS)
    prPoolList[FBM_POOL_TYPE_VSS].u4Inherit = FBM_POOL_ROOT;
    prPoolList[FBM_POOL_TYPE_VSS].u4PoolSize = FBM_VSS_POOL_SIZE;
    prPoolArrangeInfo[FBM_POOL_TYPE_VSS].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_VSS].ePool1 = u4AheadType;
    u4AheadType = FBM_POOL_TYPE_VSS;
#endif
#if defined(CC_FBM_HW_DEMOD)
    prPoolList[FBM_POOL_TYPE_HW_DEMOD].u4Inherit = FBM_POOL_ROOT;
    prPoolList[FBM_POOL_TYPE_HW_DEMOD].u4PoolSize = FBM_HW_DEMOD_SIZE;
    prPoolArrangeInfo[FBM_POOL_TYPE_HW_DEMOD].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_HW_DEMOD].ePool1 = u4AheadType;
    u4AheadType = FBM_POOL_TYPE_HW_DEMOD;
#endif

#if defined(CC_SUPPORT_MONITOR_OUT_TVE)
    // ----TVE----
    prPoolList[FBM_POOL_TYPE_TVE].u4Inherit = FBM_POOL_ROOT;
    prPoolList[FBM_POOL_TYPE_TVE].u4PoolSize = FBM_TVE_POOL_SIZE;
    prPoolArrangeInfo[FBM_POOL_TYPE_TVE].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_TVE].ePool1 = u4AheadType;
    u4AheadType = FBM_POOL_TYPE_TVE;
#endif

#ifdef CC_SUPPORT_RECORD_AV
	  prPoolList[FBM_POOL_TYPE_TDC].u4Inherit = FBM_POOL_ROOT;
	  prPoolList[FBM_POOL_TYPE_TDC].u4PoolSize = FBM_TDCAV_POOL_SIZE;
	  prPoolArrangeInfo[FBM_POOL_TYPE_TDC].eMode = FBM_POOL_ARRANGE_AFTER;
	  prPoolArrangeInfo[FBM_POOL_TYPE_TDC].ePool1 = u4AheadType;
	  u4AheadType = FBM_POOL_TYPE_TDC;
#endif

#if defined(SUPPORT_DIIVA)
    prPoolList[FBM_POOL_TYPE_DIVA].u4Inherit = FBM_POOL_ROOT;
    prPoolList[FBM_POOL_TYPE_DIVA].u4PoolSize = FBM_SCPOS_8BIT_SIZE_FLIP(1920, 1080, 4);
    prPoolList[FBM_POOL_TYPE_DIVA].u4Width = 1920;
    prPoolList[FBM_POOL_TYPE_DIVA].u4Height = 1080;
    prPoolArrangeInfo[FBM_POOL_TYPE_DIVA].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_DIVA].ePool1 = u4AheadType;
    u4AheadType = FBM_POOL_TYPE_DIVA;
#endif

#if defined(CC_FBM_SUPPORT_DMXPID)
    prPoolList[FBM_POOL_TYPE_DMXPID].u4Inherit = FBM_POOL_ROOT;
    prPoolList[FBM_POOL_TYPE_DMXPID].u4PoolSize = FBM_DMX_PID_BUF_SIZE;
    prPoolArrangeInfo[FBM_POOL_TYPE_DMXPID].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_DMXPID].ePool1 = u4AheadType;
    u4AheadType = FBM_POOL_TYPE_DMXPID;
#endif

#if defined(CC_FBM_SUPPORT_BEIC)
    prPoolList[FBM_POOL_TYPE_BEIC].u4Inherit = FBM_POOL_ROOT;
    prPoolList[FBM_POOL_TYPE_BEIC].u4PoolSize = FBM_BEIC_BUF_SIZE;
    prPoolArrangeInfo[FBM_POOL_TYPE_BEIC].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_BEIC].ePool1 = u4AheadType;
    u4AheadType = FBM_POOL_TYPE_BEIC;
#endif

#if defined(CC_FBM_SUPPORT_PVR)
    if(SRMFBM_GetConf() == FBM_MEM_CFG_MT5399_A3)
    {
        prPoolList[FBM_POOL_TYPE_PVR].u4Inherit = FBM_POOL_ROOT;
        //prPoolList[FBM_POOL_TYPE_PVR].u4PoolSize = FBM_PVR_POOL_SIZE;
        prPoolList[FBM_POOL_TYPE_PVR].u4PoolSize = 0x680000;
        prPoolArrangeInfo[FBM_POOL_TYPE_PVR].eMode = FBM_POOL_ARRANGE_AFTER;
        prPoolArrangeInfo[FBM_POOL_TYPE_PVR].ePool1 = u4AheadType;
        u4AheadType = FBM_POOL_TYPE_PVR;

        // PVR2
        prPoolList[FBM_POOL_TYPE_PVR2].u4Inherit = FBM_POOL_ROOT;
        prPoolList[FBM_POOL_TYPE_PVR2].u4PoolSize = FBM_PVR2_POOL_SIZE;
        prPoolArrangeInfo[FBM_POOL_TYPE_PVR2].eMode = FBM_POOL_ARRANGE_AFTER;
        prPoolArrangeInfo[FBM_POOL_TYPE_PVR2].ePool1 = u4AheadType;
        u4AheadType = FBM_POOL_TYPE_PVR2;
    }
    else
    {
        prPoolList[FBM_POOL_TYPE_PVR].u4Inherit = FBM_POOL_ROOT;
        prPoolList[FBM_POOL_TYPE_PVR].u4PoolSize = FBM_PVR_POOL_SIZE;
        prPoolArrangeInfo[FBM_POOL_TYPE_PVR].eMode = FBM_POOL_ARRANGE_AFTER;
        prPoolArrangeInfo[FBM_POOL_TYPE_PVR].ePool1 = u4AheadType;
        u4AheadType = FBM_POOL_TYPE_PVR;
    }
#endif

    #if defined(CC_SUPPORT_DUAL_3D_BROADCASTING)
    prPoolList[FBM_POOL_TYPE_IMGRZ_3D_KR].u4Inherit = FBM_POOL_ROOT;
	prPoolList[FBM_POOL_TYPE_IMGRZ_3D_KR].u4PoolSize = FBM_IMGRZ_3D_KR_POOL_SIZE;
    prPoolArrangeInfo[FBM_POOL_TYPE_IMGRZ_3D_KR].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_IMGRZ_3D_KR].ePool1 = u4AheadType;
    u4AheadType = FBM_POOL_TYPE_IMGRZ_3D_KR;
    #endif

    if(SRMFBM_GetConf() == FBM_MEM_CFG_MT5399_A3)
    {
    	prPoolList[FBM_POOL_TYPE_PNG].u4Inherit = FBM_POOL_ROOT;
		prPoolList[FBM_POOL_TYPE_PNG].u4PoolSize = FBM_PNG_POOL_SIZE;
    	prPoolArrangeInfo[FBM_POOL_TYPE_PNG].eMode = FBM_POOL_ARRANGE_AFTER;
    	prPoolArrangeInfo[FBM_POOL_TYPE_PNG].ePool1 = u4AheadType;
    	u4AheadType = FBM_POOL_TYPE_PNG;

    	prPoolList[FBM_POOL_TYPE_JPEG].u4Inherit = FBM_POOL_ROOT;
    	prPoolArrangeInfo[FBM_POOL_TYPE_JPEG].eMode = FBM_POOL_ARRANGE_AFTER;
    	prPoolArrangeInfo[FBM_POOL_TYPE_JPEG].ePool1 = u4AheadType;
    	u4AheadType = FBM_POOL_TYPE_JPEG;
    }
    prPoolList[FBM_POOL_TYPE_DRM].u4Inherit = FBM_POOL_ROOT;
    prPoolList[FBM_POOL_TYPE_DRM].u4PoolSize = FBM_DRM_BUF_SIZE;
    prPoolArrangeInfo[FBM_POOL_TYPE_DRM].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_DRM].ePool1 = u4AheadType;
    u4AheadType = FBM_POOL_TYPE_DRM;

#if defined(CC_SUPPORT_VENC)
    prPoolList[FBM_POOL_TYPE_VENC].u4Inherit = FBM_POOL_ROOT;
    prPoolList[FBM_POOL_TYPE_VENC].u4PoolSize = FBM_VENC_POOL_SIZE;
    prPoolArrangeInfo[FBM_POOL_TYPE_VENC].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_VENC].ePool1 = u4AheadType;
    u4AheadType = FBM_POOL_TYPE_VENC;
#endif

#if defined(CC_FBM_SUPPORT_MUXER)
    prPoolList[FBM_POOL_TYPE_MUXER].u4Inherit = FBM_POOL_ROOT;
    prPoolList[FBM_POOL_TYPE_MUXER].u4PoolSize = FBM_MUXER_SIZE;
    prPoolArrangeInfo[FBM_POOL_TYPE_MUXER].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_MUXER].ePool1 = u4AheadType;
    u4AheadType = FBM_POOL_TYPE_MUXER;
#endif
#if 0
    //-----PQ_3DC-----
    prPoolList[FBM_POOL_TYPE_PQ_3DC].u4Inherit = FBM_POOL_ROOT;
    prPoolList[FBM_POOL_TYPE_PQ_3DC].u4PoolSize = FBM_PQ_3DC_POOL_SIZE;
    prPoolArrangeInfo[FBM_POOL_TYPE_PQ_3DC].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_PQ_3DC].ePool1 = u4AheadType;
    u4AheadType = FBM_POOL_TYPE_PQ_3DC;
#endif
    u4BehindType = FBM_POOL_TYPE_NS;
#ifdef CC_SUPPORT_OD    //if (DRVCUST_OptGet(eFlagSupportOD) == TRUE)
    prPoolList[FBM_POOL_TYPE_OD].u4Inherit = FBM_POOL_ROOT;
    prPoolList[FBM_POOL_TYPE_OD].u4PoolSize = FBM_OD_POOL_SIZE;
    prPoolArrangeInfo[FBM_POOL_TYPE_OD].eMode = FBM_POOL_ARRANGE_BEFORE;
    prPoolArrangeInfo[FBM_POOL_TYPE_OD].ePool1 = u4BehindType;
    u4BehindType = FBM_POOL_TYPE_OD;
#endif
#ifdef ADSP_BIN_SUPPORT
	prPoolList[FBM_POOL_TYPE_DSP_BIN].u4Inherit = FBM_POOL_ROOT;
	prPoolList[FBM_POOL_TYPE_DSP_BIN].u4PoolSize = FBM_DSP_BIN_POOL_SIZE;
	prPoolArrangeInfo[FBM_POOL_TYPE_DSP_BIN].eMode = FBM_POOL_ARRANGE_BEFORE;
	prPoolArrangeInfo[FBM_POOL_TYPE_DSP_BIN].ePool1 = u4BehindType;
	u4BehindType = FBM_POOL_TYPE_DSP_BIN;
#endif		 

    //-----DSP-----
    prPoolList[FBM_POOL_TYPE_DSP].u4Inherit = FBM_POOL_ROOT;
    prPoolArrangeInfo[FBM_POOL_TYPE_DSP].eMode = FBM_POOL_ARRANGE_BEFORE;
    prPoolArrangeInfo[FBM_POOL_TYPE_DSP].ePool1 = u4BehindType;
#if defined(CC_SUPPORT_5_SEC_PTS_PCR_OFFSET)
    prPoolList[FBM_POOL_TYPE_DSP].u4PoolSize = FBM_DSP_POOL_SIZE + 0x400000;
#elif defined(CC_SUPPORT_2_SEC_PTS_PCR_OFFSET)
    prPoolList[FBM_POOL_TYPE_DSP].u4PoolSize = FBM_DSP_POOL_SIZE + 0x1A0000;
#else
    prPoolList[FBM_POOL_TYPE_DSP].u4PoolSize = FBM_DSP_POOL_SIZE;
#endif    	

#if 0//defined(CC_FBM_INT_TWO_FBP)
    prPoolList[FBM_POOL_TYPE_FEEDER].u4PoolSize = prPoolList[FBM_POOL_TYPE_FEEDER].u4PoolSize * 2;
#endif

    // handle Image Viewer Pool
#ifdef CC_OSD_USE_FBM
    // JPEG | OSD1 | OSD2 | OSD3 | OSD4 (Share all except DSP)
    //
    prPoolList[FBM_POOL_TYPE_FEEDER_MMP].u4PoolSize = FBM_FEEDER_POOL_SIZE;
#if 0//defined(CC_FBM_INT_TWO_FBP)
    prPoolList[FBM_POOL_TYPE_FEEDER_MMP].u4PoolSize = prPoolList[FBM_POOL_TYPE_FEEDER_MMP].u4PoolSize * 2;
    #endif
    prPoolArrangeInfo[FBM_POOL_TYPE_FEEDER_MMP].eMode = FBM_POOL_ARRANGE_ALLIGN_END;
    prPoolArrangeInfo[FBM_POOL_TYPE_FEEDER_MMP].ePool1 = FBM_POOL_TYPE_SCPOS_MAIN;

#ifndef CC_FBM_3D_OSD
    prPoolList[FBM_POOL_TYPE_JPEG_OSD1].u4PoolSize = FBM_OSD_POOL_SIZE(1920, 1088, 1, 4); // FHD, 32bpp, x1
    prPoolList[FBM_POOL_TYPE_JPEG_OSD1].u4Width = 1920;
    prPoolList[FBM_POOL_TYPE_JPEG_OSD1].u4Height = 1088;
    prPoolArrangeInfo[FBM_POOL_TYPE_JPEG_OSD1].eMode = FBM_POOL_ARRANGE_BEFORE;
    prPoolArrangeInfo[FBM_POOL_TYPE_JPEG_OSD1].ePool1 = FBM_POOL_TYPE_FEEDER_MMP;

    prPoolList[FBM_POOL_TYPE_JPEG_OSD2].u4PoolSize = FBM_OSD_POOL_SIZE(1920, 1088, 1, 4); // FHD, 32bpp, x1
    prPoolList[FBM_POOL_TYPE_JPEG_OSD2].u4Width = 1920;
    prPoolList[FBM_POOL_TYPE_JPEG_OSD2].u4Height = 1088;
    prPoolArrangeInfo[FBM_POOL_TYPE_JPEG_OSD2].eMode = FBM_POOL_ARRANGE_BEFORE;
    prPoolArrangeInfo[FBM_POOL_TYPE_JPEG_OSD2].ePool1 = FBM_POOL_TYPE_JPEG_OSD1;
#if !defined(CC_REMOVE_JPEG_OSD34)
    prPoolList[FBM_POOL_TYPE_JPEG_OSD3].u4PoolSize = FBM_OSD_POOL_SIZE(1920, 1088, 1, 4); // FHD, 32bpp, x1
    prPoolList[FBM_POOL_TYPE_JPEG_OSD3].u4Width = 1920;
    prPoolList[FBM_POOL_TYPE_JPEG_OSD3].u4Height = 1088;
    prPoolArrangeInfo[FBM_POOL_TYPE_JPEG_OSD3].eMode = FBM_POOL_ARRANGE_BEFORE;
    prPoolArrangeInfo[FBM_POOL_TYPE_JPEG_OSD3].ePool1 = FBM_POOL_TYPE_JPEG_OSD2;

    prPoolList[FBM_POOL_TYPE_JPEG_OSD4].u4PoolSize = FBM_OSD_POOL_SIZE(1920, 1088, 1, 4); // FHD, 32bpp, x1
    prPoolList[FBM_POOL_TYPE_JPEG_OSD4].u4Width = 1920;
    prPoolList[FBM_POOL_TYPE_JPEG_OSD4].u4Height = 1088;
    prPoolArrangeInfo[FBM_POOL_TYPE_JPEG_OSD4].eMode = FBM_POOL_ARRANGE_BEFORE;
    prPoolArrangeInfo[FBM_POOL_TYPE_JPEG_OSD4].ePool1 = FBM_POOL_TYPE_JPEG_OSD3;
	
    u4AheadType = FBM_POOL_TYPE_JPEG_OSD4;
#else
    u4AheadType = FBM_POOL_TYPE_JPEG_OSD2;
#endif
#else
	u4AheadType = FBM_POOL_TYPE_FEEDER_MMP;
#endif

    // JPEG (Between Start & OSD1)
    if(SRMFBM_GetConf() != FBM_MEM_CFG_MT5399_A3)
    {
    	prPoolArrangeInfo[FBM_POOL_TYPE_JPEG].eMode = FBM_POOL_ARRANGE_BEFORE;
    	prPoolArrangeInfo[FBM_POOL_TYPE_JPEG].ePool1 = u4AheadType;
    }

#else
    // JPEG + B2R share DMX + MPEG
    // JPEG
    prPoolArrangeInfo[FBM_POOL_TYPE_JPEG].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_JPEG].ePool1 = FBM_POOL_TYPE_TOTAL;

    // B2R
    prPoolList[FBM_POOL_TYPE_B2R].u4PoolSize = FBM_B2R_POOL_SIZE(1920, 1088, 2, 2); // FHD, 422, x2
    prPoolList[FBM_POOL_TYPE_B2R].u4Mode = FBM_POOL_MODE_422_FB | FBM_POOL_MODE_SCPOS_2FB;
    prPoolArrangeInfo[FBM_POOL_TYPE_B2R].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_B2R].ePool1 = FBM_POOL_TYPE_JPEG;
#endif
}

#else
static VOID _FbmConfigurePool_MT5882_TVP_SCPOS_MAIN(POOL_LIST_T* prPoolList, POOL_ARRANGE_INFO_T* prPoolArrangeInfo)
{
    FBM_POOL_TYPE_T u4AheadType = FBM_POOL_TYPE_SCPOS_MAIN, u4BehindType, u4VdoBehindType = FBM_POOL_TYPE_NS;
    UNUSED(u4AheadType);
    UNUSED(u4BehindType);
    UNUSED(u4VdoBehindType);
    // arrange root pool ---------------------------------------------------

   //==================NoSecure FBM===================

u4AheadType = FBM_POOL_TYPE_TOTAL;	

#ifdef ADSP_BIN_SUPPORT
	prPoolList[FBM_POOL_TYPE_DSP_BIN].u4Inherit = FBM_POOL_ROOT;
	prPoolList[FBM_POOL_TYPE_DSP_BIN].u4PoolSize = FBM_DSP_BIN_POOL_SIZE;
	prPoolArrangeInfo[FBM_POOL_TYPE_DSP_BIN].eMode = FBM_POOL_ARRANGE_AFTER;
	prPoolArrangeInfo[FBM_POOL_TYPE_DSP_BIN].ePool1 = u4AheadType;
	u4AheadType = FBM_POOL_TYPE_DSP_BIN;
#endif		 

    // vbi
    prPoolList[FBM_POOL_TYPE_VBI].u4Inherit = FBM_POOL_ROOT;
    prPoolArrangeInfo[FBM_POOL_TYPE_VBI].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_VBI].ePool1 = u4AheadType;
    u4AheadType = FBM_POOL_TYPE_VBI;		
	
	//-----DSP-----
	prPoolList[FBM_POOL_TYPE_DSP].u4Inherit = FBM_POOL_ROOT;
	prPoolList[FBM_POOL_TYPE_DSP].u4AddrAlign= (0x1000000-1);
	prPoolArrangeInfo[FBM_POOL_TYPE_DSP].eMode = FBM_POOL_ARRANGE_AFTER;
	prPoolArrangeInfo[FBM_POOL_TYPE_DSP].ePool1 = u4AheadType;
#if defined(CC_SUPPORT_5_SEC_PTS_PCR_OFFSET)
	prPoolList[FBM_POOL_TYPE_DSP].u4PoolSize = FBM_DSP_POOL_SIZE + 0x400000;
#elif defined(CC_SUPPORT_2_SEC_PTS_PCR_OFFSET)
	prPoolList[FBM_POOL_TYPE_DSP].u4PoolSize = FBM_DSP_POOL_SIZE + 0x1A0000;
#else
	prPoolList[FBM_POOL_TYPE_DSP].u4PoolSize = FBM_DSP_POOL_SIZE;
#endif 
	u4AheadType = FBM_POOL_TYPE_DSP;


// pq tool
    prPoolList[FBM_POOL_TYPE_PQ_TOOL].u4Inherit = FBM_POOL_ROOT;
    prPoolList[FBM_POOL_TYPE_PQ_TOOL].u4PoolSize = FBM_PQ_TOOL_POOL_SIZE; // A3 needs 8MB for OD dump
    
    prPoolArrangeInfo[FBM_POOL_TYPE_PQ_TOOL].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_PQ_TOOL].ePool1 = u4AheadType;
    u4AheadType = FBM_POOL_TYPE_PQ_TOOL;
    if (u4VdoBehindType == FBM_POOL_TYPE_NS)
    {
        u4VdoBehindType = FBM_POOL_TYPE_PQ_TOOL;
    }

#if defined(CC_53XX_FACE_DETECTION)
    prPoolList[FBM_POOL_TYPE_FACEDET].u4Inherit = FBM_POOL_ROOT;
    prPoolList[FBM_POOL_TYPE_FACEDET].u4PoolSize = FBM_FD_POOL_SIZE;
    prPoolArrangeInfo[FBM_POOL_TYPE_FACEDET].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_FACEDET].ePool1 = u4AheadType;
    u4AheadType = FBM_POOL_TYPE_FACEDET;
#endif

#if defined(CC_FBM_SUPPORT_SWDMX)||defined(CC_FBM_SUPPORT_LITE_SWDMX)
    prPoolList[FBM_POOL_TYPE_SWDMX].u4Inherit = FBM_POOL_ROOT;
    prPoolList[FBM_POOL_TYPE_SWDMX].u4PoolSize = FBM_SWDMX_POOL_SIZE;
    prPoolArrangeInfo[FBM_POOL_TYPE_SWDMX].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_SWDMX].ePool1 = u4AheadType;
    u4AheadType = FBM_POOL_TYPE_SWDMX;
#endif
#if defined(CC_FBM_SUPPORT_PNG)		
		    prPoolList[FBM_POOL_TYPE_PNG].u4Inherit = FBM_POOL_ROOT;
			prPoolList[FBM_POOL_TYPE_PNG].u4PoolSize = FBM_PNG_POOL_SIZE;
    		prPoolArrangeInfo[FBM_POOL_TYPE_PNG].eMode = FBM_POOL_ARRANGE_AFTER;
    		prPoolArrangeInfo[FBM_POOL_TYPE_PNG].ePool1 = u4AheadType;
    		u4AheadType = FBM_POOL_TYPE_PNG;
#endif
#if defined(CC_FBM_3D_OSD)||defined(CC_MHP_SUPPORT)
	prPoolList[FBM_POOL_TYPE_JPG_VDP].u4Inherit = FBM_POOL_ROOT;
    prPoolList[FBM_POOL_TYPE_JPG_VDP].u4PoolSize = FBM_JPEVDP_POOL_SIZE;
    prPoolArrangeInfo[FBM_POOL_TYPE_JPG_VDP].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_JPG_VDP].ePool1 = u4AheadType;
	u4AheadType = FBM_POOL_TYPE_JPG_VDP;
#endif
#if defined(CC_SUPPORT_VSS)
    prPoolList[FBM_POOL_TYPE_VSS].u4Inherit = FBM_POOL_ROOT;
    prPoolList[FBM_POOL_TYPE_VSS].u4PoolSize = FBM_VSS_POOL_SIZE;
    prPoolArrangeInfo[FBM_POOL_TYPE_VSS].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_VSS].ePool1 = u4AheadType;
    u4AheadType = FBM_POOL_TYPE_VSS;
#endif
#if defined(CC_FBM_HW_DEMOD)
    prPoolList[FBM_POOL_TYPE_HW_DEMOD].u4Inherit = FBM_POOL_ROOT;
    prPoolList[FBM_POOL_TYPE_HW_DEMOD].u4PoolSize = FBM_HW_DEMOD_SIZE;
    prPoolArrangeInfo[FBM_POOL_TYPE_HW_DEMOD].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_HW_DEMOD].ePool1 = u4AheadType;
    u4AheadType = FBM_POOL_TYPE_HW_DEMOD;
#endif

#ifdef CC_SUPPORT_RECORD_AV
	  prPoolList[FBM_POOL_TYPE_TDC].u4Inherit = FBM_POOL_ROOT;
	  prPoolList[FBM_POOL_TYPE_TDC].u4PoolSize = FBM_TDCAV_POOL_SIZE;
	  prPoolArrangeInfo[FBM_POOL_TYPE_TDC].eMode = FBM_POOL_ARRANGE_AFTER;
	  prPoolArrangeInfo[FBM_POOL_TYPE_TDC].ePool1 = u4AheadType;
	  u4AheadType = FBM_POOL_TYPE_TDC;
#endif

#if defined(SUPPORT_DIIVA)
    prPoolList[FBM_POOL_TYPE_DIVA].u4Inherit = FBM_POOL_ROOT;
    prPoolList[FBM_POOL_TYPE_DIVA].u4PoolSize = FBM_SCPOS_8BIT_SIZE_FLIP(1920, 1080, 4);
    prPoolList[FBM_POOL_TYPE_DIVA].u4Width = 1920;
    prPoolList[FBM_POOL_TYPE_DIVA].u4Height = 1080;
    prPoolArrangeInfo[FBM_POOL_TYPE_DIVA].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_DIVA].ePool1 = u4AheadType;
    u4AheadType = FBM_POOL_TYPE_DIVA;
#endif

#if defined(CC_FBM_SUPPORT_DMXPID)
    prPoolList[FBM_POOL_TYPE_DMXPID].u4Inherit = FBM_POOL_ROOT;
    prPoolList[FBM_POOL_TYPE_DMXPID].u4PoolSize = FBM_DMX_PID_BUF_SIZE;
    prPoolArrangeInfo[FBM_POOL_TYPE_DMXPID].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_DMXPID].ePool1 = u4AheadType;
    u4AheadType = FBM_POOL_TYPE_DMXPID;
#endif

#if defined(CC_FBM_SUPPORT_BEIC)
    prPoolList[FBM_POOL_TYPE_BEIC].u4Inherit = FBM_POOL_ROOT;
    prPoolList[FBM_POOL_TYPE_BEIC].u4PoolSize = FBM_BEIC_BUF_SIZE;
    prPoolArrangeInfo[FBM_POOL_TYPE_BEIC].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_BEIC].ePool1 = u4AheadType;
    u4AheadType = FBM_POOL_TYPE_BEIC;
#endif

#if defined(CC_FBM_SUPPORT_PVR)
    if(SRMFBM_GetConf() == FBM_MEM_CFG_MT5399_A3)
    {
        prPoolList[FBM_POOL_TYPE_PVR].u4Inherit = FBM_POOL_ROOT;
        //prPoolList[FBM_POOL_TYPE_PVR].u4PoolSize = FBM_PVR_POOL_SIZE;
        prPoolList[FBM_POOL_TYPE_PVR].u4PoolSize = 0x680000;
        prPoolArrangeInfo[FBM_POOL_TYPE_PVR].eMode = FBM_POOL_ARRANGE_AFTER;
        prPoolArrangeInfo[FBM_POOL_TYPE_PVR].ePool1 = u4AheadType;
        u4AheadType = FBM_POOL_TYPE_PVR;

        // PVR2
        prPoolList[FBM_POOL_TYPE_PVR2].u4Inherit = FBM_POOL_ROOT;
        prPoolList[FBM_POOL_TYPE_PVR2].u4PoolSize = FBM_PVR2_POOL_SIZE;
        prPoolArrangeInfo[FBM_POOL_TYPE_PVR2].eMode = FBM_POOL_ARRANGE_AFTER;
        prPoolArrangeInfo[FBM_POOL_TYPE_PVR2].ePool1 = u4AheadType;
        u4AheadType = FBM_POOL_TYPE_PVR2;
    }
    else
    {
        prPoolList[FBM_POOL_TYPE_PVR].u4Inherit = FBM_POOL_ROOT;
        prPoolList[FBM_POOL_TYPE_PVR].u4PoolSize = FBM_PVR_POOL_SIZE;
        prPoolArrangeInfo[FBM_POOL_TYPE_PVR].eMode = FBM_POOL_ARRANGE_AFTER;
        prPoolArrangeInfo[FBM_POOL_TYPE_PVR].ePool1 = u4AheadType;
        u4AheadType = FBM_POOL_TYPE_PVR;
    }
#endif

    #if defined(CC_SUPPORT_DUAL_3D_BROADCASTING)
    prPoolList[FBM_POOL_TYPE_IMGRZ_3D_KR].u4Inherit = FBM_POOL_ROOT;
	prPoolList[FBM_POOL_TYPE_IMGRZ_3D_KR].u4PoolSize = FBM_IMGRZ_3D_KR_POOL_SIZE;
    prPoolArrangeInfo[FBM_POOL_TYPE_IMGRZ_3D_KR].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_IMGRZ_3D_KR].ePool1 = u4AheadType;
    u4AheadType = FBM_POOL_TYPE_IMGRZ_3D_KR;
    #endif

    if(SRMFBM_GetConf() == FBM_MEM_CFG_MT5399_A3)
    {
    	prPoolList[FBM_POOL_TYPE_PNG].u4Inherit = FBM_POOL_ROOT;
		prPoolList[FBM_POOL_TYPE_PNG].u4PoolSize = FBM_PNG_POOL_SIZE;
    	prPoolArrangeInfo[FBM_POOL_TYPE_PNG].eMode = FBM_POOL_ARRANGE_AFTER;
    	prPoolArrangeInfo[FBM_POOL_TYPE_PNG].ePool1 = u4AheadType;
    	u4AheadType = FBM_POOL_TYPE_PNG;

    	prPoolList[FBM_POOL_TYPE_JPEG].u4Inherit = FBM_POOL_ROOT;
    	prPoolArrangeInfo[FBM_POOL_TYPE_JPEG].eMode = FBM_POOL_ARRANGE_AFTER;
    	prPoolArrangeInfo[FBM_POOL_TYPE_JPEG].ePool1 = u4AheadType;
    	u4AheadType = FBM_POOL_TYPE_JPEG;
    }

#if defined(CC_SUPPORT_VENC)
    prPoolList[FBM_POOL_TYPE_VENC].u4Inherit = FBM_POOL_ROOT;
    prPoolList[FBM_POOL_TYPE_VENC].u4PoolSize = FBM_VENC_POOL_SIZE;
    prPoolArrangeInfo[FBM_POOL_TYPE_VENC].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_VENC].ePool1 = u4AheadType;
    u4AheadType = FBM_POOL_TYPE_VENC;
#endif

#if defined(CC_FBM_SUPPORT_MUXER)
    prPoolList[FBM_POOL_TYPE_MUXER].u4Inherit = FBM_POOL_ROOT;
    prPoolList[FBM_POOL_TYPE_MUXER].u4PoolSize = FBM_MUXER_SIZE;
    prPoolArrangeInfo[FBM_POOL_TYPE_MUXER].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_MUXER].ePool1 = u4AheadType;
    u4AheadType = FBM_POOL_TYPE_MUXER;
#endif
#if 0
    //-----PQ_3DC-----
    prPoolList[FBM_POOL_TYPE_PQ_3DC].u4Inherit = FBM_POOL_ROOT;
    prPoolList[FBM_POOL_TYPE_PQ_3DC].u4PoolSize = FBM_PQ_3DC_POOL_SIZE;
    prPoolArrangeInfo[FBM_POOL_TYPE_PQ_3DC].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_PQ_3DC].ePool1 = u4AheadType;
    u4AheadType = FBM_POOL_TYPE_PQ_3DC;
#endif
	prPoolList[FBM_POOL_TYPE_FEEDER].u4Inherit = FBM_POOL_ROOT;
	prPoolList[FBM_POOL_TYPE_FEEDER].u4PoolSize = FBM_FEEDER1_SIZE;
	prPoolArrangeInfo[FBM_POOL_TYPE_FEEDER].eMode = FBM_POOL_ARRANGE_AFTER;
	prPoolArrangeInfo[FBM_POOL_TYPE_FEEDER].ePool1 = u4AheadType;

	
	// handle Image Viewer Pool
#ifdef CC_OSD_USE_FBM
		// JPEG | OSD1 | OSD2 | OSD3 | OSD4 (Share all except DSP)
		//
		prPoolList[FBM_POOL_TYPE_FEEDER_MMP].u4PoolSize = FBM_FEEDER_POOL_SIZE;
#if 0//defined(CC_FBM_INT_TWO_FBP)
		prPoolList[FBM_POOL_TYPE_FEEDER_MMP].u4PoolSize = prPoolList[FBM_POOL_TYPE_FEEDER_MMP].u4PoolSize * 2;
#endif
		prPoolArrangeInfo[FBM_POOL_TYPE_FEEDER_MMP].eMode = FBM_POOL_ARRANGE_AFTER;
		prPoolArrangeInfo[FBM_POOL_TYPE_FEEDER_MMP].ePool1 = u4AheadType;

		if(prPoolList[FBM_POOL_TYPE_FEEDER].u4PoolSize >= prPoolList[FBM_POOL_TYPE_FEEDER_MMP].u4PoolSize)
			u4AheadType = FBM_POOL_TYPE_FEEDER;
		else
			u4AheadType = FBM_POOL_TYPE_FEEDER_MMP;

		// JPEG (Between Start & OSD1)
		if(SRMFBM_GetConf() != FBM_MEM_CFG_MT5399_A3)
		{
			prPoolList[FBM_POOL_TYPE_JPEG].u4Inherit = FBM_POOL_ROOT;
			prPoolList[FBM_POOL_TYPE_JPEG].u4PoolSize = FBM_JPEG_POOL_SIZE;
			prPoolArrangeInfo[FBM_POOL_TYPE_JPEG].eMode = FBM_POOL_ARRANGE_AFTER;
			prPoolArrangeInfo[FBM_POOL_TYPE_JPEG].ePool1 = u4AheadType;
			u4AheadType = FBM_POOL_TYPE_JPEG;
		}
	
#ifndef CC_FBM_3D_OSD
		prPoolList[FBM_POOL_TYPE_JPEG_OSD1].u4Inherit = FBM_POOL_ROOT;
		prPoolList[FBM_POOL_TYPE_JPEG_OSD1].u4PoolSize = FBM_OSD_POOL_SIZE(1920, 1088, 1, 4); // FHD, 32bpp, x1
		prPoolList[FBM_POOL_TYPE_JPEG_OSD1].u4Width = 1920;
		prPoolList[FBM_POOL_TYPE_JPEG_OSD1].u4Height = 1088;
		prPoolArrangeInfo[FBM_POOL_TYPE_JPEG_OSD1].eMode = FBM_POOL_ARRANGE_AFTER;
		prPoolArrangeInfo[FBM_POOL_TYPE_JPEG_OSD1].ePool1 = u4AheadType;
		u4AheadType = FBM_POOL_TYPE_JPEG_OSD1;

		prPoolList[FBM_POOL_TYPE_JPEG_OSD2].u4Inherit = FBM_POOL_ROOT;
		prPoolList[FBM_POOL_TYPE_JPEG_OSD2].u4PoolSize = FBM_OSD_POOL_SIZE(1920, 1088, 1, 4); // FHD, 32bpp, x1
		prPoolList[FBM_POOL_TYPE_JPEG_OSD2].u4Width = 1920;
		prPoolList[FBM_POOL_TYPE_JPEG_OSD2].u4Height = 1088;
		prPoolArrangeInfo[FBM_POOL_TYPE_JPEG_OSD2].eMode = FBM_POOL_ARRANGE_AFTER;
		prPoolArrangeInfo[FBM_POOL_TYPE_JPEG_OSD2].ePool1 = u4AheadType;
		u4AheadType = FBM_POOL_TYPE_JPEG_OSD2;
#if !defined(CC_REMOVE_JPEG_OSD34)
		prPoolList[FBM_POOL_TYPE_JPEG_OSD3].u4Inherit = FBM_POOL_ROOT;
		prPoolList[FBM_POOL_TYPE_JPEG_OSD3].u4PoolSize = FBM_OSD_POOL_SIZE(1920, 1088, 1, 4); // FHD, 32bpp, x1
		prPoolList[FBM_POOL_TYPE_JPEG_OSD3].u4Width = 1920;
		prPoolList[FBM_POOL_TYPE_JPEG_OSD3].u4Height = 1088;
		prPoolArrangeInfo[FBM_POOL_TYPE_JPEG_OSD3].eMode = FBM_POOL_ARRANGE_AFTER;
		prPoolArrangeInfo[FBM_POOL_TYPE_JPEG_OSD3].ePool1 = u4AheadType;
		u4AheadType = FBM_POOL_TYPE_JPEG_OSD3;

		prPoolList[FBM_POOL_TYPE_JPEG_OSD4].u4Inherit = FBM_POOL_ROOT;
		prPoolList[FBM_POOL_TYPE_JPEG_OSD4].u4PoolSize = FBM_OSD_POOL_SIZE(1920, 1088, 1, 4); // FHD, 32bpp, x1
		prPoolList[FBM_POOL_TYPE_JPEG_OSD4].u4Width = 1920;
		prPoolList[FBM_POOL_TYPE_JPEG_OSD4].u4Height = 1088;
		prPoolArrangeInfo[FBM_POOL_TYPE_JPEG_OSD4].eMode = FBM_POOL_ARRANGE_AFTER;
		prPoolArrangeInfo[FBM_POOL_TYPE_JPEG_OSD4].ePool1 = u4AheadType;		
		u4AheadType = FBM_POOL_TYPE_JPEG_OSD4;
#endif
#endif
	
#else
		// JPEG
		prPoolList[FBM_POOL_TYPE_JPEG].u4Inherit = FBM_POOL_ROOT;
		prPoolList[FBM_POOL_TYPE_JPEG].u4PoolSize = FBM_JPEG_POOL_SIZE;
		prPoolArrangeInfo[FBM_POOL_TYPE_JPEG].eMode = FBM_POOL_ARRANGE_AFTER;
		prPoolArrangeInfo[FBM_POOL_TYPE_JPEG].ePool1 = u4AheadType;
		u4AheadType = FBM_POOL_TYPE_JPEG;
#endif

#if 0//defined(CC_FBM_TWO_FBP_SHARED_WITH_DFB) || defined(CC_VOMX_TV_COEXIST) 
	
	prPoolList[FBM_POOL_TYPE_FEEDER2].u4Inherit = FBM_POOL_ROOT;
    prPoolList[FBM_POOL_TYPE_FEEDER2].u4PoolSize = FBM_FEEDER_POOL_SIZE;
    prPoolArrangeInfo[FBM_POOL_TYPE_FEEDER2].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_FEEDER2].ePool1 = u4AheadType;
    u4AheadType = FBM_POOL_TYPE_FEEDER2;
#endif

#if defined(CC_FBM_FOUR_INST_SUPPORT)
prPoolList[FBM_POOL_TYPE_FEEDER3].u4Inherit = FBM_POOL_ROOT;
prPoolList[FBM_POOL_TYPE_FEEDER3].u4PoolSize = FBM_FEEDER_POOL_SIZE;
prPoolArrangeInfo[FBM_POOL_TYPE_FEEDER3].eMode = FBM_POOL_ARRANGE_AFTER;
prPoolArrangeInfo[FBM_POOL_TYPE_FEEDER3].ePool1 = u4AheadType;
u4AheadType = FBM_POOL_TYPE_FEEDER3;

prPoolList[FBM_POOL_TYPE_FEEDER4].u4Inherit = FBM_POOL_ROOT;
prPoolList[FBM_POOL_TYPE_FEEDER4].u4PoolSize = FBM_FEEDER_POOL_SIZE;
prPoolArrangeInfo[FBM_POOL_TYPE_FEEDER4].eMode = FBM_POOL_ARRANGE_AFTER;
prPoolArrangeInfo[FBM_POOL_TYPE_FEEDER4].ePool1 = u4AheadType;
u4AheadType = FBM_POOL_TYPE_FEEDER4;
#endif

//==================Secure FBM=====================

#if defined(CC_FBM_FOUR_INST_SUPPORT)
    prPoolList[FBM_POOL_TYPE_DMX3].u4Inherit = FBM_POOL_ROOT;
    prPoolList[FBM_POOL_TYPE_DMX3].u4PoolSize = FBM_DMX_H264_SIZE;
    prPoolArrangeInfo[FBM_POOL_TYPE_DMX3].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_DMX3].ePool1 = u4AheadType;
    u4AheadType = FBM_POOL_TYPE_DMX3;

	prPoolList[FBM_POOL_TYPE_DMX4].u4Inherit = FBM_POOL_ROOT;
    prPoolList[FBM_POOL_TYPE_DMX4].u4PoolSize = FBM_DMX_H264_SIZE;
    prPoolArrangeInfo[FBM_POOL_TYPE_DMX4].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_DMX4].ePool1 = u4AheadType;
    u4AheadType = FBM_POOL_TYPE_DMX4;

	prPoolList[FBM_POOL_TYPE_MPEG3].u4Inherit = FBM_POOL_ROOT;
    prPoolList[FBM_POOL_TYPE_MPEG3].u4Mode = (FBM_POOL_MODE_MPEG_DBK);
    prPoolList[FBM_POOL_TYPE_MPEG3].u4PoolSize = 0x1780000;
    prPoolArrangeInfo[FBM_POOL_TYPE_MPEG3].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_MPEG3].ePool1 = u4AheadType;
    u4AheadType = FBM_POOL_TYPE_MPEG3;

	prPoolList[FBM_POOL_TYPE_MPEG4].u4Inherit = FBM_POOL_ROOT;
    prPoolList[FBM_POOL_TYPE_MPEG4].u4Mode = (FBM_POOL_MODE_MPEG_DBK);
    prPoolList[FBM_POOL_TYPE_MPEG4].u4PoolSize = 0x1780000;
    prPoolArrangeInfo[FBM_POOL_TYPE_MPEG4].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_MPEG4].ePool1 = u4AheadType;
    u4AheadType = FBM_POOL_TYPE_MPEG4;
#endif

	
#ifndef CC_OSD_USE_FBM
	// B2R
	prPoolList[FBM_POOL_TYPE_B2R].u4Inherit = FBM_POOL_ROOT;
	prPoolList[FBM_POOL_TYPE_B2R].u4PoolSize = FBM_B2R_POOL_SIZE(1920, 1088, 2, 2); // FHD, 422, x2
	prPoolList[FBM_POOL_TYPE_B2R].u4Mode = FBM_POOL_MODE_422_FB | FBM_POOL_MODE_SCPOS_2FB;
	prPoolArrangeInfo[FBM_POOL_TYPE_B2R].eMode = FBM_POOL_ARRANGE_AFTER;
	prPoolArrangeInfo[FBM_POOL_TYPE_B2R].ePool1 = u4AheadType;
	u4AheadType = FBM_POOL_TYPE_B2R;
#endif

	prPoolList[FBM_POOL_TYPE_DRM].u4Inherit = FBM_POOL_ROOT;
	prPoolList[FBM_POOL_TYPE_DRM].u4PoolSize = FBM_DRM_BUF_SIZE;
	prPoolArrangeInfo[FBM_POOL_TYPE_DRM].eMode = FBM_POOL_ARRANGE_AFTER;
	prPoolArrangeInfo[FBM_POOL_TYPE_DRM].ePool1 = u4AheadType;
	u4AheadType = FBM_POOL_TYPE_DRM;


#if defined(CC_FBM_SECURE_FEEDER)
    prPoolList[FBM_POOL_TYPE_SECURE_FEEDER].u4PoolSize = FBM_SECURE_FEEDER_2K_SIZE;
    prPoolList[FBM_POOL_TYPE_SECURE_FEEDER].u4Inherit = FBM_POOL_ROOT;			 
    prPoolArrangeInfo[FBM_POOL_TYPE_SECURE_FEEDER].eMode = FBM_POOL_ARRANGE_AFTER; 
    prPoolArrangeInfo[FBM_POOL_TYPE_SECURE_FEEDER].ePool1 = u4AheadType;			 
    u4AheadType = FBM_POOL_TYPE_SECURE_FEEDER;
#endif

//----- DMX-----
    prPoolList[FBM_POOL_TYPE_DMX].u4Inherit = FBM_POOL_ROOT;
    prPoolList[FBM_POOL_TYPE_DMX].u4PoolSize = FBM_DMX1_SIZE;
    prPoolArrangeInfo[FBM_POOL_TYPE_DMX].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_DMX].ePool1 = u4AheadType;   
#if defined(CC_TRUSTZONE_SUPPORT)&& defined(CC_SVP_SUPPORT)
    prPoolList[FBM_POOL_TYPE_DMX].u4PoolSize = 0;
#endif
	u4AheadType = FBM_POOL_TYPE_DMX;

    //----- MPEG-----
    prPoolList[FBM_POOL_TYPE_MPEG].u4Inherit = FBM_POOL_ROOT;
    prPoolList[FBM_POOL_TYPE_MPEG].u4Mode = (FBM_POOL_MODE_MPEG_DBK | FBM_POOL_MODE_SHARE);
    prPoolList[FBM_POOL_TYPE_MPEG].u4PoolSize = FBM_MPEG_Y_SIZE; 
    prPoolArrangeInfo[FBM_POOL_TYPE_MPEG].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_MPEG].ePool1 = u4AheadType;
	u4AheadType = FBM_POOL_TYPE_MPEG;

#if defined(CC_FBM_TWO_FBP_SHARED_WITH_DFB) || defined(CC_VOMX_TV_COEXIST) 
		if (u4VdoBehindType == FBM_POOL_TYPE_NS)
		{
			u4VdoBehindType = FBM_POOL_TYPE_DMX2;
		}
		prPoolList[FBM_POOL_TYPE_DMX2].u4Inherit = FBM_POOL_ROOT;
		{
			prPoolList[FBM_POOL_TYPE_DMX2].u4PoolSize = FBM_ALIGN_MASK((FBM_DMX2_SIZE), FBM_DMX_SIZE_ALIGMENT);
	
#if defined(CC_TRUSTZONE_SUPPORT)&& defined(CC_SVP_SUPPORT)
			prPoolList[FBM_POOL_TYPE_DMX2].u4PoolSize = 0;
#endif
		}
		prPoolArrangeInfo[FBM_POOL_TYPE_DMX2].eMode = FBM_POOL_ARRANGE_AFTER;
		prPoolArrangeInfo[FBM_POOL_TYPE_DMX2].ePool1 = u4AheadType;
		u4AheadType = FBM_POOL_TYPE_DMX2;
	
		prPoolList[FBM_POOL_TYPE_MPEG2].u4Inherit = FBM_POOL_ROOT;
		//prPoolList[FBM_POOL_TYPE_MPEG2].u4Mode = (FBM_POOL_MODE_MPEG_DBK | FBM_POOL_MODE_SHARE);
    #if defined(CC_VOMX_TV_COEXIST)
		prPoolList[FBM_POOL_TYPE_MPEG2].u4Mode = (FBM_POOL_MODE_MPEG_DBK);
		//prPoolList[FBM_POOL_TYPE_MPEG2].u4PoolSize = 0x2F00000; // 23.5Mx2
		prPoolList[FBM_POOL_TYPE_MPEG2].u4PoolSize = FBM_MPEG_2_SIZE; // Jason rewrite
    #else
		prPoolList[FBM_POOL_TYPE_MPEG2].u4Mode = (FBM_POOL_MODE_MPEG_DBK | FBM_POOL_MODE_SHARE);
		prPoolList[FBM_POOL_TYPE_MPEG2].u4PoolSize = FBM_MPEG_2_SIZE; // Jason rewrite
    #endif
		prPoolArrangeInfo[FBM_POOL_TYPE_MPEG2].eMode = FBM_POOL_ARRANGE_AFTER;
		prPoolArrangeInfo[FBM_POOL_TYPE_MPEG2].ePool1 = u4AheadType;
		u4AheadType = FBM_POOL_TYPE_MPEG2;
#endif

    //----- SCPOS MAIN-----
    prPoolList[FBM_POOL_TYPE_SCPOS_MAIN].u4Inherit = FBM_POOL_ROOT;
#if defined(CC_SUPPORT_SCART_OUT_IN_MMP)
    // allocate 7MB for sub modules, no consider flip/mirror
    prPoolList[FBM_POOL_TYPE_SCPOS_MAIN].u4PoolSize = 0x2F00000;
#endif

    prPoolList[FBM_POOL_TYPE_SCPOS_MAIN].u4PoolSize = FBM_SCPOS_MAIN_SIZE; //main 1080i 422 0xe9c0000*2+sub 0x880000

    prPoolArrangeInfo[FBM_POOL_TYPE_SCPOS_MAIN].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_SCPOS_MAIN].ePool1 = u4AheadType;
#if defined(CC_IC_VERIFY_FBM_MODE)
    prPoolList[FBM_POOL_TYPE_SCPOS_MAIN].u4PoolSize = 0x2b00000;
#endif
	u4AheadType = FBM_POOL_TYPE_SCPOS_MAIN;
    
#if defined(CC_SUPPORT_MONITOR_OUT_TVE)
    // ----TVE----
    prPoolList[FBM_POOL_TYPE_TVE].u4Inherit = FBM_POOL_ROOT;
    prPoolList[FBM_POOL_TYPE_TVE].u4PoolSize = FBM_TVE_POOL_SIZE;
    prPoolArrangeInfo[FBM_POOL_TYPE_TVE].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_TVE].ePool1 = u4AheadType;
    u4AheadType = FBM_POOL_TYPE_TVE;
#endif


#ifdef CC_SUPPORT_OD    //if (DRVCUST_OptGet(eFlagSupportOD) == TRUE)
    prPoolList[FBM_POOL_TYPE_OD].u4Inherit = FBM_POOL_ROOT;
    prPoolList[FBM_POOL_TYPE_OD].u4PoolSize = FBM_OD_POOL_SIZE;
    prPoolArrangeInfo[FBM_POOL_TYPE_OD].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_OD].ePool1 = u4AheadType;
    u4AheadType = FBM_POOL_TYPE_OD;
#endif

}
#endif //!defined(CC_TVP_SUPPORT)

#endif//defined(CC_MT5882)

#if  defined(CC_MT5399) ||defined(CC_MT5890)
static VOID _FbmConfigurePool_MT5890_2K80_SCPOS_MAIN(POOL_LIST_T* prPoolList, POOL_ARRANGE_INFO_T* prPoolArrangeInfo)
{
    FBM_POOL_TYPE_T u4AheadType = FBM_POOL_TYPE_SCPOS_MAIN, u4BehindType, u4VdoBehindType = FBM_POOL_TYPE_NS;
    UNUSED(u4AheadType);
    UNUSED(u4BehindType);
    UNUSED(u4VdoBehindType);
    // arrange root pool ---------------------------------------------------

    // DMXPID |DSP|VBI|PVR|VENC|SCPOS_SUB|
    
	u4AheadType = FBM_POOL_TYPE_TOTAL;
#if defined(CC_FBM_SUPPORT_DMXPID)
		prPoolList[FBM_POOL_TYPE_DMXPID].u4Inherit = FBM_POOL_ROOT;
		prPoolList[FBM_POOL_TYPE_DMXPID].u4PoolSize = FBM_DMX_PID_BUF_SIZE;
		prPoolArrangeInfo[FBM_POOL_TYPE_DMXPID].eMode = FBM_POOL_ARRANGE_AFTER;
		prPoolArrangeInfo[FBM_POOL_TYPE_DMXPID].ePool1 = u4AheadType;
		u4AheadType = FBM_POOL_TYPE_DMXPID;
#endif

    // vbi
    prPoolList[FBM_POOL_TYPE_VBI].u4Inherit = FBM_POOL_ROOT;
    prPoolArrangeInfo[FBM_POOL_TYPE_VBI].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_VBI].ePool1 = u4AheadType;
    u4AheadType = FBM_POOL_TYPE_VBI;

#if defined(CC_FBM_SUPPORT_PVR)
	prPoolList[FBM_POOL_TYPE_PVR].u4Inherit = FBM_POOL_ROOT;
	prPoolList[FBM_POOL_TYPE_PVR].u4PoolSize = FBM_PVR_2K_SIZE;
	prPoolArrangeInfo[FBM_POOL_TYPE_PVR].eMode = FBM_POOL_ARRANGE_AFTER;
	prPoolArrangeInfo[FBM_POOL_TYPE_PVR].ePool1 = u4AheadType;
	u4AheadType = FBM_POOL_TYPE_PVR;
#endif

#if defined(CC_SUPPORT_VENC)
	prPoolList[FBM_POOL_TYPE_VENC].u4Inherit = FBM_POOL_ROOT;
	prPoolList[FBM_POOL_TYPE_VENC].u4PoolSize = FBM_VENC_POOL_SIZE;
	prPoolArrangeInfo[FBM_POOL_TYPE_VENC].eMode = FBM_POOL_ARRANGE_AFTER;
	prPoolArrangeInfo[FBM_POOL_TYPE_VENC].ePool1 = u4AheadType;
	u4AheadType = FBM_POOL_TYPE_VENC;
#endif
#ifdef CC_SUPPORT_RECORD_AV
	prPoolList[FBM_POOL_TYPE_TDC].u4Inherit = FBM_POOL_ROOT;
	prPoolList[FBM_POOL_TYPE_TDC].u4PoolSize = FBM_TDCAV_POOL_SIZE;
	prPoolArrangeInfo[FBM_POOL_TYPE_TDC].eMode = FBM_POOL_ARRANGE_AFTER;
	prPoolArrangeInfo[FBM_POOL_TYPE_TDC].ePool1 = u4AheadType;
	u4AheadType = FBM_POOL_TYPE_TDC;
#endif
#ifdef CC_SUPPORT_OD    //if (DRVCUST_OptGet(eFlagSupportOD) == TRUE)
	prPoolList[FBM_POOL_TYPE_OD].u4Inherit = FBM_POOL_ROOT;
	prPoolList[FBM_POOL_TYPE_OD].u4PoolSize = FBM_OD_POOL_SIZE;
	prPoolArrangeInfo[FBM_POOL_TYPE_OD].eMode = FBM_POOL_ARRANGE_AFTER;
	prPoolArrangeInfo[FBM_POOL_TYPE_OD].ePool1 = u4AheadType;
	u4AheadType = FBM_POOL_TYPE_OD;
#endif
	
	//-----PQ_3DC-----
	prPoolList[FBM_POOL_TYPE_PQ_3DC].u4Inherit = FBM_POOL_ROOT;
	prPoolList[FBM_POOL_TYPE_PQ_3DC].u4PoolSize = FBM_PQ_3DC_POOL_SIZE;
	prPoolArrangeInfo[FBM_POOL_TYPE_PQ_3DC].eMode = FBM_POOL_ARRANGE_AFTER;
	prPoolArrangeInfo[FBM_POOL_TYPE_PQ_3DC].ePool1 = u4AheadType;
	u4AheadType = FBM_POOL_TYPE_PQ_3DC;
	
#if 1
		prPoolList[FBM_POOL_TYPE_JPEG].u4Inherit = FBM_POOL_ROOT;
		prPoolList[FBM_POOL_TYPE_JPEG].u4PoolSize = FBM_JPEG_POOL_SIZE;
		prPoolArrangeInfo[FBM_POOL_TYPE_JPEG].eMode = FBM_POOL_ARRANGE_AFTER;
		prPoolArrangeInfo[FBM_POOL_TYPE_JPEG].ePool1 = u4AheadType;
		u4AheadType = FBM_POOL_TYPE_JPEG;
#endif	

//----- SCPOS PIP-----
 prPoolList[FBM_POOL_TYPE_SCPOS_PIP].u4Inherit = FBM_POOL_ROOT;
 prPoolList[FBM_POOL_TYPE_SCPOS_PIP].u4PoolSize = FBM_SCPOS_SUB_2K_SIZE; 
 prPoolArrangeInfo[FBM_POOL_TYPE_SCPOS_PIP].eMode = FBM_POOL_ARRANGE_AFTER;
 prPoolArrangeInfo[FBM_POOL_TYPE_SCPOS_PIP].ePool1 = u4AheadType;
 u4AheadType = FBM_POOL_TYPE_SCPOS_PIP;

#if defined(CC_SUPPORT_VSS)
		prPoolList[FBM_POOL_TYPE_VSS].u4Inherit = FBM_POOL_ROOT;
		prPoolList[FBM_POOL_TYPE_VSS].u4PoolSize = FBM_VSS_POOL_SIZE;
		prPoolArrangeInfo[FBM_POOL_TYPE_VSS].eMode = FBM_POOL_ARRANGE_AFTER;
		prPoolArrangeInfo[FBM_POOL_TYPE_VSS].ePool1 = u4AheadType;
		u4AheadType = FBM_POOL_TYPE_VSS;
#endif
#ifdef ADSP_BIN_SUPPORT
		prPoolList[FBM_POOL_TYPE_DSP_BIN].u4Inherit = FBM_POOL_ROOT;
		prPoolList[FBM_POOL_TYPE_DSP_BIN].u4PoolSize = FBM_DSP_BIN_POOL_SIZE;
		prPoolArrangeInfo[FBM_POOL_TYPE_DSP_BIN].eMode = FBM_POOL_ARRANGE_AFTER;
		prPoolArrangeInfo[FBM_POOL_TYPE_DSP_BIN].ePool1 = u4AheadType;
		u4AheadType = FBM_POOL_TYPE_DSP_BIN;
#endif		 

    //-----DSP-----
    prPoolList[FBM_POOL_TYPE_DSP].u4Inherit = FBM_POOL_ROOT;
    prPoolArrangeInfo[FBM_POOL_TYPE_DSP].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_DSP].ePool1 = u4AheadType;
    prPoolList[FBM_POOL_TYPE_DSP].u4PoolSize = FBM_DSP_POOL_SIZE;
    u4AheadType = FBM_POOL_TYPE_DSP;
	

//channel B
//DMX|MPEG|SCPOS_MAIN
#if defined(CC_FBM_2ND_CHANNEL) // 2 Channel
    //----- DMX-----
    prPoolList[FBM_POOL_TYPE_DMX].u4Inherit = FBM_POOL_ROOT;
    prPoolList[FBM_POOL_TYPE_DMX].u4PoolSize = FBM_DMX1_2K_SIZE;
    prPoolArrangeInfo[FBM_POOL_TYPE_DMX].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_DMX].ePool1 = FBM_POOL_TYPE_TOTAL2;   
    u4AheadType = FBM_POOL_TYPE_DMX;
	
    //----- DMX2-----
    prPoolList[FBM_POOL_TYPE_DMX2].u4Inherit = FBM_POOL_ROOT;
    prPoolList[FBM_POOL_TYPE_DMX2].u4PoolSize = FBM_DMX2_2K_SIZE;
    prPoolArrangeInfo[FBM_POOL_TYPE_DMX2].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_DMX2].ePool1 = u4AheadType;   
    u4AheadType = FBM_POOL_TYPE_DMX2;
	
#if defined(CC_FBM_SECURE_FEEDER)
	 prPoolList[FBM_POOL_TYPE_SECURE_FEEDER].u4PoolSize = FBM_SECURE_FEEDER_2K_SIZE;
	 prPoolList[FBM_POOL_TYPE_SECURE_FEEDER].u4Inherit = FBM_POOL_ROOT; 			
	 prPoolArrangeInfo[FBM_POOL_TYPE_SECURE_FEEDER].eMode = FBM_POOL_ARRANGE_AFTER; 
	 prPoolArrangeInfo[FBM_POOL_TYPE_SECURE_FEEDER].ePool1 = u4AheadType;			
	 u4AheadType = FBM_POOL_TYPE_SECURE_FEEDER;
#endif

    //----- MPEG-----
	prPoolList[FBM_POOL_TYPE_MPEG].u4Inherit = FBM_POOL_ROOT;
	prPoolList[FBM_POOL_TYPE_MPEG].u4PoolSize = FBM_MPEG1_2K_Y_SIZE;	
	prPoolList[FBM_POOL_TYPE_MPEG].u4Mode = (FBM_POOL_MODE_MPEG_DBK | FBM_POOL_MODE_SHARE);
	prPoolArrangeInfo[FBM_POOL_TYPE_MPEG].eMode = FBM_POOL_ARRANGE_AFTER;
	prPoolArrangeInfo[FBM_POOL_TYPE_MPEG].ePool1 = u4AheadType;
	u4AheadType = FBM_POOL_TYPE_MPEG;
    
	//----- MPEG2-----
    prPoolList[FBM_POOL_TYPE_MPEG2].u4Inherit = FBM_POOL_ROOT;
	prPoolList[FBM_POOL_TYPE_MPEG2].u4PoolSize = FBM_MPEG2_2K_Y_SIZE;
    prPoolList[FBM_POOL_TYPE_MPEG2].u4Mode = (FBM_POOL_MODE_MPEG_DBK | FBM_POOL_MODE_SHARE);
    prPoolArrangeInfo[FBM_POOL_TYPE_MPEG2].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_MPEG2].ePool1 = u4AheadType;
    u4AheadType = FBM_POOL_TYPE_MPEG2;
	
	prPoolList[FBM_POOL_TYPE_FEEDER].u4PoolSize = FBM_FEEDER1_2K_SIZE;
	prPoolList[FBM_POOL_TYPE_FEEDER].u4Inherit = FBM_POOL_ROOT; 		   
	prPoolArrangeInfo[FBM_POOL_TYPE_FEEDER].eMode = FBM_POOL_ARRANGE_AFTER; 
	prPoolArrangeInfo[FBM_POOL_TYPE_FEEDER].ePool1 = u4AheadType;  

	prPoolList[FBM_POOL_TYPE_FEEDER_MMP].u4PoolSize = FBM_FEEDER_POOL_SIZE;
	prPoolArrangeInfo[FBM_POOL_TYPE_FEEDER_MMP].eMode = FBM_POOL_ARRANGE_AFTER;
	prPoolArrangeInfo[FBM_POOL_TYPE_FEEDER_MMP].ePool1 = u4AheadType;
	
	u4AheadType = FBM_POOL_TYPE_FEEDER;
	
	prPoolList[FBM_POOL_TYPE_FEEDER2].u4PoolSize = FBM_FEEDER2_2K_SIZE;
	prPoolList[FBM_POOL_TYPE_FEEDER2].u4Inherit = FBM_POOL_ROOT;		   
	prPoolArrangeInfo[FBM_POOL_TYPE_FEEDER2].eMode = FBM_POOL_ARRANGE_AFTER; 
	prPoolArrangeInfo[FBM_POOL_TYPE_FEEDER2].ePool1 = u4AheadType;		   
	u4AheadType = FBM_POOL_TYPE_FEEDER2;

    #if defined(CC_FBM_SUPPORT_SRS)
	//----- SRS-----
	prPoolList[FBM_POOL_TYPE_SRS].u4Inherit = FBM_POOL_ROOT;
	prPoolList[FBM_POOL_TYPE_SRS].u4PoolSize = FBM_SRS_SIZE;
	prPoolArrangeInfo[FBM_POOL_TYPE_SRS].eMode = FBM_POOL_ARRANGE_AFTER;
	prPoolArrangeInfo[FBM_POOL_TYPE_SRS].ePool1 = u4AheadType;
	u4AheadType = FBM_POOL_TYPE_SRS;
   #endif
   
#if defined(CC_SUPPORT_CIPLUS14)
	   //----- Remux-----
	   prPoolList[FBM_POOL_TYPE_REMUX].u4Inherit = FBM_POOL_ROOT;
	   prPoolList[FBM_POOL_TYPE_REMUX].u4PoolSize = FBM_REMUX_POOL_SIZE;
	   prPoolArrangeInfo[FBM_POOL_TYPE_REMUX].eMode = FBM_POOL_ARRANGE_AFTER;
	   prPoolArrangeInfo[FBM_POOL_TYPE_REMUX].ePool1 = u4AheadType;   
	   u4AheadType = FBM_POOL_TYPE_REMUX;
#endif
   
#if defined(CC_SUPPORT_W2DR2R)
	   //----- W2D R2R-----
	   prPoolList[FBM_POOL_TYPE_W2D_R2R].u4Inherit = FBM_POOL_ROOT;
	   prPoolList[FBM_POOL_TYPE_W2D_R2R].u4PoolSize = FBM_W2DR2R_POOL_2K_SIZE;
	   prPoolArrangeInfo[FBM_POOL_TYPE_W2D_R2R].eMode = FBM_POOL_ARRANGE_AFTER;
	   prPoolArrangeInfo[FBM_POOL_TYPE_W2D_R2R].ePool1 = u4AheadType;
	   u4AheadType = FBM_POOL_TYPE_W2D_R2R;
#endif
#if defined(CC_FBM_3D_OSD) || defined(CC_MHP_SUPPORT) //MHP use jpeg_vdp buffer
	prPoolList[FBM_POOL_TYPE_JPG_VDP].u4Inherit = FBM_POOL_ROOT;
	prPoolList[FBM_POOL_TYPE_JPG_VDP].u4PoolSize = FBM_JPEVDP_POOL_SIZE;
	prPoolArrangeInfo[FBM_POOL_TYPE_JPG_VDP].eMode = FBM_POOL_ARRANGE_AFTER;
	prPoolArrangeInfo[FBM_POOL_TYPE_JPG_VDP].ePool1 = u4AheadType;
	u4AheadType = FBM_POOL_TYPE_JPG_VDP;
#endif
	 	 
#if defined(CC_FBM_SUPPORT_SWDMX)||defined(CC_FBM_SUPPORT_LITE_SWDMX)
	 prPoolList[FBM_POOL_TYPE_SWDMX].u4Inherit = FBM_POOL_ROOT;
	 prPoolList[FBM_POOL_TYPE_SWDMX].u4PoolSize = FBM_SWDMX_POOL_SIZE;
	 prPoolArrangeInfo[FBM_POOL_TYPE_SWDMX].eMode = FBM_POOL_ARRANGE_AFTER;
	 prPoolArrangeInfo[FBM_POOL_TYPE_SWDMX].ePool1 = u4AheadType;
	 u4AheadType = FBM_POOL_TYPE_SWDMX;
#endif
#if defined(CC_FBM_SUPPORT_PNG)		
			prPoolList[FBM_POOL_TYPE_PNG].u4Inherit = FBM_POOL_ROOT;
			prPoolList[FBM_POOL_TYPE_PNG].u4PoolSize = FBM_PNG_POOL_SIZE;
			prPoolArrangeInfo[FBM_POOL_TYPE_PNG].eMode = FBM_POOL_ARRANGE_AFTER;
			prPoolArrangeInfo[FBM_POOL_TYPE_PNG].ePool1 = u4AheadType;
			u4AheadType = FBM_POOL_TYPE_PNG;
#endif	

	prPoolList[FBM_POOL_TYPE_DRM].u4Inherit = FBM_POOL_ROOT;
	prPoolList[FBM_POOL_TYPE_DRM].u4PoolSize = FBM_DRM_BUF_SIZE;
	prPoolArrangeInfo[FBM_POOL_TYPE_DRM].eMode = FBM_POOL_ARRANGE_AFTER;
	prPoolArrangeInfo[FBM_POOL_TYPE_DRM].ePool1 = u4AheadType;
	u4AheadType = FBM_POOL_TYPE_DRM;

    // pq tool
    prPoolList[FBM_POOL_TYPE_PQ_TOOL].u4Inherit = FBM_POOL_ROOT;
	prPoolList[FBM_POOL_TYPE_PQ_TOOL].u4PoolSize = FBM_PQ_TOOL_POOL_SIZE; 
    prPoolArrangeInfo[FBM_POOL_TYPE_PQ_TOOL].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_PQ_TOOL].ePool1 = u4AheadType;
    u4AheadType = FBM_POOL_TYPE_PQ_TOOL;
	
		// handle Image Viewer Pool
		// JPEG | OSD1 | OSD2 | OSD3 | OSD4 (Share all except DSP)
		//
#ifndef CC_FBM_3D_OSD
		prPoolList[FBM_POOL_TYPE_JPEG_OSD1].u4PoolSize = FBM_OSD_POOL_SIZE(1920, 1088, 1, 4); // FHD, 32bpp, x1
		prPoolList[FBM_POOL_TYPE_JPEG_OSD1].u4Width = 1920;
		prPoolList[FBM_POOL_TYPE_JPEG_OSD1].u4Height = 1088;
		prPoolArrangeInfo[FBM_POOL_TYPE_JPEG_OSD1].eMode = FBM_POOL_ARRANGE_BEFORE;
		prPoolArrangeInfo[FBM_POOL_TYPE_JPEG_OSD1].ePool1 = FBM_POOL_TYPE_FEEDER_MMP;
	
		prPoolList[FBM_POOL_TYPE_JPEG_OSD2].u4PoolSize = FBM_OSD_POOL_SIZE(1920, 1088, 1, 4); // FHD, 32bpp, x1
		prPoolList[FBM_POOL_TYPE_JPEG_OSD2].u4Width = 1920;
		prPoolList[FBM_POOL_TYPE_JPEG_OSD2].u4Height = 1088;
		prPoolArrangeInfo[FBM_POOL_TYPE_JPEG_OSD2].eMode = FBM_POOL_ARRANGE_BEFORE;
		prPoolArrangeInfo[FBM_POOL_TYPE_JPEG_OSD2].ePool1 = FBM_POOL_TYPE_JPEG_OSD1;
#if !defined(CC_REMOVE_JPEG_OSD34)
		prPoolList[FBM_POOL_TYPE_JPEG_OSD3].u4PoolSize = FBM_OSD_POOL_SIZE(1920, 1088, 1, 4); // FHD, 32bpp, x1
		prPoolList[FBM_POOL_TYPE_JPEG_OSD3].u4Width = 1920;
		prPoolList[FBM_POOL_TYPE_JPEG_OSD3].u4Height = 1088;
		prPoolArrangeInfo[FBM_POOL_TYPE_JPEG_OSD3].eMode = FBM_POOL_ARRANGE_BEFORE;
		prPoolArrangeInfo[FBM_POOL_TYPE_JPEG_OSD3].ePool1 = FBM_POOL_TYPE_JPEG_OSD2;
	
		prPoolList[FBM_POOL_TYPE_JPEG_OSD4].u4PoolSize = FBM_OSD_POOL_SIZE(1920, 1088, 1, 4); // FHD, 32bpp, x1
		prPoolList[FBM_POOL_TYPE_JPEG_OSD4].u4Width = 1920;
		prPoolList[FBM_POOL_TYPE_JPEG_OSD4].u4Height = 1088;
		prPoolArrangeInfo[FBM_POOL_TYPE_JPEG_OSD4].eMode = FBM_POOL_ARRANGE_BEFORE;
		prPoolArrangeInfo[FBM_POOL_TYPE_JPEG_OSD4].ePool1 = FBM_POOL_TYPE_JPEG_OSD3;
		
		u4AheadType = FBM_POOL_TYPE_JPEG_OSD4;
#else
		u4AheadType = FBM_POOL_TYPE_JPEG_OSD2;
#endif
#else
		u4AheadType = FBM_POOL_TYPE_FEEDER_MMP;
#endif
	
#endif
#if defined(CC_FBM_3ND_CHANNEL) // 3 Channel
#ifdef CC_BYPASS_MJC
       u4AheadType = FBM_POOL_TYPE_TOTAL3;
#else
		prPoolList[FBM_POOL_TYPE_MJC_1].u4Inherit = FBM_POOL_ROOT;
		prPoolList[FBM_POOL_TYPE_MJC_1].u4PoolSize = FBM_MJC_RSV_POOL_SIZE;
		LOG(3, "FBMCFG CHANEL B: prPoolList[FBM_POOL_TYPE_MJC_1].u4PoolSize :%d \n", prPoolList[FBM_POOL_TYPE_MJC_1].u4PoolSize);
		prPoolList[FBM_POOL_TYPE_MJC_1].u4AddrAlign = FBM_MJC_ADDR_ALIGMENT;
		prPoolList[FBM_POOL_TYPE_MJC_1].u4Mode= FBM_POOL_MODE_MJC_Y10_C10_422_1RDC_3FRM;
		prPoolArrangeInfo[FBM_POOL_TYPE_MJC_1].eMode = FBM_POOL_ARRANGE_AFTER;
		prPoolArrangeInfo[FBM_POOL_TYPE_MJC_1].ePool1 = FBM_POOL_TYPE_TOTAL3;
		u4AheadType = FBM_POOL_TYPE_MJC_1;
#endif
   //----- SCPOS MAIN-----
   prPoolList[FBM_POOL_TYPE_SCPOS_MAIN].u4Inherit = FBM_POOL_ROOT;
   prPoolList[FBM_POOL_TYPE_SCPOS_MAIN].u4PoolSize = FBM_SCPOS_MAIN_2K_SIZE; //main 1080i 422 0xe9c0000*2+sub 0x880000
   prPoolArrangeInfo[FBM_POOL_TYPE_SCPOS_MAIN].eMode = FBM_POOL_ARRANGE_AFTER;
   prPoolArrangeInfo[FBM_POOL_TYPE_SCPOS_MAIN].ePool1 = u4AheadType;
   u4AheadType = FBM_POOL_TYPE_SCPOS_MAIN;

   prPoolList[FBM_POOL_TYPE_MPEG_C].u4Inherit = FBM_POOL_ROOT;
     prPoolList[FBM_POOL_TYPE_MPEG_C].u4PoolSize = FBM_MPEG1_2K_C_SIZE;
     prPoolArrangeInfo[FBM_POOL_TYPE_MPEG_C].eMode = FBM_POOL_ARRANGE_AFTER;
     prPoolArrangeInfo[FBM_POOL_TYPE_MPEG_C].ePool1 = u4AheadType;
 	u4AheadType = FBM_POOL_TYPE_MPEG_C;
	
 	prPoolList[FBM_POOL_TYPE_MPEG2_C].u4Inherit = FBM_POOL_ROOT;
     prPoolList[FBM_POOL_TYPE_MPEG2_C].u4PoolSize = FBM_MPEG2_2K_C_SIZE;
     prPoolArrangeInfo[FBM_POOL_TYPE_MPEG2_C].eMode = FBM_POOL_ARRANGE_AFTER;
     prPoolArrangeInfo[FBM_POOL_TYPE_MPEG2_C].ePool1 = u4AheadType;
 	u4AheadType = FBM_POOL_TYPE_MPEG2_C;
	
#endif
}

static VOID _FbmConfigurePool_MT5890_FHD_SCPOS_MAIN(POOL_LIST_T* prPoolList, POOL_ARRANGE_INFO_T* prPoolArrangeInfo)
{
    FBM_POOL_TYPE_T u4AheadType = FBM_POOL_TYPE_SCPOS_MAIN, u4BehindType, u4VdoBehindType = FBM_POOL_TYPE_NS;
    UNUSED(u4AheadType);
    UNUSED(u4BehindType);
    UNUSED(u4VdoBehindType);
    // arrange root pool ---------------------------------------------------

    // DMXPID |DSP|VBI|PVR|VENC|SCPOS_SUB|
    
	u4AheadType = FBM_POOL_TYPE_TOTAL;
#if defined(CC_FBM_SUPPORT_DMXPID)
		prPoolList[FBM_POOL_TYPE_DMXPID].u4Inherit = FBM_POOL_ROOT;
		prPoolList[FBM_POOL_TYPE_DMXPID].u4PoolSize = FBM_DMX_PID_BUF_SIZE;
		prPoolArrangeInfo[FBM_POOL_TYPE_DMXPID].eMode = FBM_POOL_ARRANGE_AFTER;
		prPoolArrangeInfo[FBM_POOL_TYPE_DMXPID].ePool1 = u4AheadType;
		u4AheadType = FBM_POOL_TYPE_DMXPID;
#endif

    // vbi
    prPoolList[FBM_POOL_TYPE_VBI].u4Inherit = FBM_POOL_ROOT;
    prPoolArrangeInfo[FBM_POOL_TYPE_VBI].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_VBI].ePool1 = u4AheadType;
    u4AheadType = FBM_POOL_TYPE_VBI;

#if defined(CC_FBM_SUPPORT_PVR)
	prPoolList[FBM_POOL_TYPE_PVR].u4Inherit = FBM_POOL_ROOT;
	prPoolList[FBM_POOL_TYPE_PVR].u4PoolSize = FBM_PVR_2K_SIZE;
	prPoolArrangeInfo[FBM_POOL_TYPE_PVR].eMode = FBM_POOL_ARRANGE_AFTER;
	prPoolArrangeInfo[FBM_POOL_TYPE_PVR].ePool1 = u4AheadType;
	u4AheadType = FBM_POOL_TYPE_PVR;
#endif

#if defined(CC_SUPPORT_VENC)
	prPoolList[FBM_POOL_TYPE_VENC].u4Inherit = FBM_POOL_ROOT;
	prPoolList[FBM_POOL_TYPE_VENC].u4PoolSize = FBM_VENC_POOL_SIZE;
	prPoolArrangeInfo[FBM_POOL_TYPE_VENC].eMode = FBM_POOL_ARRANGE_AFTER;
	prPoolArrangeInfo[FBM_POOL_TYPE_VENC].ePool1 = u4AheadType;
	u4AheadType = FBM_POOL_TYPE_VENC;
#endif
#ifdef CC_SUPPORT_RECORD_AV
	prPoolList[FBM_POOL_TYPE_TDC].u4Inherit = FBM_POOL_ROOT;
	prPoolList[FBM_POOL_TYPE_TDC].u4PoolSize = FBM_TDCAV_POOL_SIZE;
	prPoolArrangeInfo[FBM_POOL_TYPE_TDC].eMode = FBM_POOL_ARRANGE_AFTER;
	prPoolArrangeInfo[FBM_POOL_TYPE_TDC].ePool1 = u4AheadType;
	u4AheadType = FBM_POOL_TYPE_TDC;
#endif
#ifdef CC_SUPPORT_OD    //if (DRVCUST_OptGet(eFlagSupportOD) == TRUE)
	prPoolList[FBM_POOL_TYPE_OD].u4Inherit = FBM_POOL_ROOT;
	prPoolList[FBM_POOL_TYPE_OD].u4PoolSize = FBM_OD_POOL_SIZE;
	prPoolArrangeInfo[FBM_POOL_TYPE_OD].eMode = FBM_POOL_ARRANGE_AFTER;
	prPoolArrangeInfo[FBM_POOL_TYPE_OD].ePool1 = u4AheadType;
	u4AheadType = FBM_POOL_TYPE_OD;
#endif
	
	//-----PQ_3DC-----
	prPoolList[FBM_POOL_TYPE_PQ_3DC].u4Inherit = FBM_POOL_ROOT;
	prPoolList[FBM_POOL_TYPE_PQ_3DC].u4PoolSize = FBM_PQ_3DC_POOL_SIZE;
	prPoolArrangeInfo[FBM_POOL_TYPE_PQ_3DC].eMode = FBM_POOL_ARRANGE_AFTER;
	prPoolArrangeInfo[FBM_POOL_TYPE_PQ_3DC].ePool1 = u4AheadType;
	u4AheadType = FBM_POOL_TYPE_PQ_3DC;
	
#if 1
		prPoolList[FBM_POOL_TYPE_JPEG].u4Inherit = FBM_POOL_ROOT;
		prPoolList[FBM_POOL_TYPE_JPEG].u4PoolSize = FBM_JPEG_POOL_SIZE;
		prPoolArrangeInfo[FBM_POOL_TYPE_JPEG].eMode = FBM_POOL_ARRANGE_AFTER;
		prPoolArrangeInfo[FBM_POOL_TYPE_JPEG].ePool1 = u4AheadType;
		u4AheadType = FBM_POOL_TYPE_JPEG;
#endif	
     //----- SCPOS PIP-----
     prPoolList[FBM_POOL_TYPE_SCPOS_PIP].u4Inherit = FBM_POOL_ROOT;
     prPoolList[FBM_POOL_TYPE_SCPOS_PIP].u4PoolSize = FBM_SCPOS_SUB_2K_SIZE; 
     prPoolArrangeInfo[FBM_POOL_TYPE_SCPOS_PIP].eMode = FBM_POOL_ARRANGE_AFTER;
     prPoolArrangeInfo[FBM_POOL_TYPE_SCPOS_PIP].ePool1 = u4AheadType;
     u4AheadType = FBM_POOL_TYPE_SCPOS_PIP;

#if defined(CC_SUPPORT_VSS)
		prPoolList[FBM_POOL_TYPE_VSS].u4Inherit = FBM_POOL_ROOT;
		prPoolList[FBM_POOL_TYPE_VSS].u4PoolSize = FBM_VSS_POOL_SIZE;
		prPoolArrangeInfo[FBM_POOL_TYPE_VSS].eMode = FBM_POOL_ARRANGE_AFTER;
		prPoolArrangeInfo[FBM_POOL_TYPE_VSS].ePool1 = u4AheadType;
		u4AheadType = FBM_POOL_TYPE_VSS;
#endif
#ifdef ADSP_BIN_SUPPORT
		prPoolList[FBM_POOL_TYPE_DSP_BIN].u4Inherit = FBM_POOL_ROOT;
		prPoolList[FBM_POOL_TYPE_DSP_BIN].u4PoolSize = FBM_DSP_BIN_POOL_SIZE;
		prPoolArrangeInfo[FBM_POOL_TYPE_DSP_BIN].eMode = FBM_POOL_ARRANGE_AFTER;
		prPoolArrangeInfo[FBM_POOL_TYPE_DSP_BIN].ePool1 = u4AheadType;
		u4AheadType = FBM_POOL_TYPE_DSP_BIN;
#endif		 

    //-----DSP-----
    prPoolList[FBM_POOL_TYPE_DSP].u4Inherit = FBM_POOL_ROOT;
    prPoolArrangeInfo[FBM_POOL_TYPE_DSP].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_DSP].ePool1 = u4AheadType;
    prPoolList[FBM_POOL_TYPE_DSP].u4PoolSize = FBM_DSP_POOL_SIZE;
    u4AheadType = FBM_POOL_TYPE_DSP;
	

//channel B
//DMX|MPEG|SCPOS_MAIN
#if defined(CC_FBM_2ND_CHANNEL) // 2 Channel
    //----- DMX-----
    prPoolList[FBM_POOL_TYPE_DMX].u4Inherit = FBM_POOL_ROOT;
    prPoolList[FBM_POOL_TYPE_DMX].u4PoolSize = FBM_DMX1_2K_SIZE;
    prPoolArrangeInfo[FBM_POOL_TYPE_DMX].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_DMX].ePool1 = FBM_POOL_TYPE_TOTAL2;   
    u4AheadType = FBM_POOL_TYPE_DMX;
	
    //----- DMX2-----
    prPoolList[FBM_POOL_TYPE_DMX2].u4Inherit = FBM_POOL_ROOT;
    prPoolList[FBM_POOL_TYPE_DMX2].u4PoolSize = FBM_DMX2_2K_SIZE;
    prPoolArrangeInfo[FBM_POOL_TYPE_DMX2].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_DMX2].ePool1 = u4AheadType;   
    u4AheadType = FBM_POOL_TYPE_DMX2;
	
#if defined(CC_FBM_SECURE_FEEDER)
	 prPoolList[FBM_POOL_TYPE_SECURE_FEEDER].u4PoolSize = FBM_SECURE_FEEDER_2K_SIZE;
	 prPoolList[FBM_POOL_TYPE_SECURE_FEEDER].u4Inherit = FBM_POOL_ROOT; 			
	 prPoolArrangeInfo[FBM_POOL_TYPE_SECURE_FEEDER].eMode = FBM_POOL_ARRANGE_AFTER; 
	 prPoolArrangeInfo[FBM_POOL_TYPE_SECURE_FEEDER].ePool1 = u4AheadType;			
	 u4AheadType = FBM_POOL_TYPE_SECURE_FEEDER;
#endif
    //----- MPEG-----
	prPoolList[FBM_POOL_TYPE_MPEG].u4Inherit = FBM_POOL_ROOT;
	prPoolList[FBM_POOL_TYPE_MPEG].u4PoolSize = FBM_MPEG1_2K_Y_SIZE;	
	prPoolList[FBM_POOL_TYPE_MPEG].u4Mode = (FBM_POOL_MODE_MPEG_DBK | FBM_POOL_MODE_SHARE);
	prPoolArrangeInfo[FBM_POOL_TYPE_MPEG].eMode = FBM_POOL_ARRANGE_AFTER;
	prPoolArrangeInfo[FBM_POOL_TYPE_MPEG].ePool1 = u4AheadType;
	u4AheadType = FBM_POOL_TYPE_MPEG;
    
	//----- MPEG2-----
    prPoolList[FBM_POOL_TYPE_MPEG2].u4Inherit = FBM_POOL_ROOT;
	prPoolList[FBM_POOL_TYPE_MPEG2].u4PoolSize = FBM_MPEG2_2K_Y_SIZE;
    prPoolList[FBM_POOL_TYPE_MPEG2].u4Mode = (FBM_POOL_MODE_MPEG_DBK | FBM_POOL_MODE_SHARE);
    prPoolArrangeInfo[FBM_POOL_TYPE_MPEG2].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_MPEG2].ePool1 = u4AheadType;
    u4AheadType = FBM_POOL_TYPE_MPEG2;

	prPoolList[FBM_POOL_TYPE_FEEDER].u4PoolSize = FBM_FEEDER1_2K_SIZE;
    prPoolList[FBM_POOL_TYPE_FEEDER].u4Inherit = FBM_POOL_ROOT; 			
    prPoolArrangeInfo[FBM_POOL_TYPE_FEEDER].eMode = FBM_POOL_ARRANGE_AFTER; 
    prPoolArrangeInfo[FBM_POOL_TYPE_FEEDER].ePool1 = u4AheadType;	
   
    prPoolList[FBM_POOL_TYPE_FEEDER_MMP].u4PoolSize = FBM_FEEDER_POOL_SIZE;
    prPoolArrangeInfo[FBM_POOL_TYPE_FEEDER_MMP].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_FEEDER_MMP].ePool1 = u4AheadType;
    
    u4AheadType = FBM_POOL_TYPE_FEEDER;
    
    prPoolList[FBM_POOL_TYPE_FEEDER2].u4PoolSize = FBM_FEEDER2_2K_SIZE;
    prPoolList[FBM_POOL_TYPE_FEEDER2].u4Inherit = FBM_POOL_ROOT; 			
    prPoolArrangeInfo[FBM_POOL_TYPE_FEEDER2].eMode = FBM_POOL_ARRANGE_AFTER; 
    prPoolArrangeInfo[FBM_POOL_TYPE_FEEDER2].ePool1 = u4AheadType;			
    u4AheadType = FBM_POOL_TYPE_FEEDER2;
	 
    #if defined(CC_SUPPORT_CIPLUS14)
		//----- Remux-----
		prPoolList[FBM_POOL_TYPE_REMUX].u4Inherit = FBM_POOL_ROOT;
		prPoolList[FBM_POOL_TYPE_REMUX].u4PoolSize = FBM_REMUX_POOL_SIZE;
		prPoolArrangeInfo[FBM_POOL_TYPE_REMUX].eMode = FBM_POOL_ARRANGE_AFTER;
		prPoolArrangeInfo[FBM_POOL_TYPE_REMUX].ePool1 = u4AheadType;   
		u4AheadType = FBM_POOL_TYPE_REMUX;
    #endif
	
    #if defined(CC_SUPPORT_W2DR2R)
		//----- W2D R2R-----
		prPoolList[FBM_POOL_TYPE_W2D_R2R].u4Inherit = FBM_POOL_ROOT;
		prPoolList[FBM_POOL_TYPE_W2D_R2R].u4PoolSize = FBM_W2DR2R_POOL_2K_SIZE;
		prPoolArrangeInfo[FBM_POOL_TYPE_W2D_R2R].eMode = FBM_POOL_ARRANGE_AFTER;
		prPoolArrangeInfo[FBM_POOL_TYPE_W2D_R2R].ePool1 = u4AheadType;
		u4AheadType = FBM_POOL_TYPE_W2D_R2R;
    #endif

    #if defined(CC_FBM_SUPPORT_SRS)
	//----- SRS-----
	prPoolList[FBM_POOL_TYPE_SRS].u4Inherit = FBM_POOL_ROOT;
	prPoolList[FBM_POOL_TYPE_SRS].u4PoolSize = FBM_SRS_SIZE;
	prPoolArrangeInfo[FBM_POOL_TYPE_SRS].eMode = FBM_POOL_ARRANGE_AFTER;
	prPoolArrangeInfo[FBM_POOL_TYPE_SRS].ePool1 = u4AheadType;
	u4AheadType = FBM_POOL_TYPE_SRS;
   #endif
   
#if defined(CC_FBM_3D_OSD) || defined(CC_MHP_SUPPORT) //MHP use jpeg_vdp buffer
	prPoolList[FBM_POOL_TYPE_JPG_VDP].u4Inherit = FBM_POOL_ROOT;
	prPoolList[FBM_POOL_TYPE_JPG_VDP].u4PoolSize = FBM_JPEVDP_POOL_SIZE;
	prPoolArrangeInfo[FBM_POOL_TYPE_JPG_VDP].eMode = FBM_POOL_ARRANGE_AFTER;
	prPoolArrangeInfo[FBM_POOL_TYPE_JPG_VDP].ePool1 = u4AheadType;
	u4AheadType = FBM_POOL_TYPE_JPG_VDP;
#endif

    //----- SCPOS MAIN-----
    prPoolList[FBM_POOL_TYPE_SCPOS_MAIN].u4Inherit = FBM_POOL_ROOT;
    prPoolList[FBM_POOL_TYPE_SCPOS_MAIN].u4PoolSize = FBM_SCPOS_MAIN_2K_SIZE; //main 1080i 422 0xe9c0000*2+sub 0x880000
    prPoolArrangeInfo[FBM_POOL_TYPE_SCPOS_MAIN].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_SCPOS_MAIN].ePool1 = u4AheadType;
    u4AheadType = FBM_POOL_TYPE_SCPOS_MAIN;

   
     prPoolList[FBM_POOL_TYPE_MPEG_C].u4Inherit = FBM_POOL_ROOT;
     prPoolList[FBM_POOL_TYPE_MPEG_C].u4PoolSize = FBM_MPEG1_2K_C_SIZE;
     prPoolArrangeInfo[FBM_POOL_TYPE_MPEG_C].eMode = FBM_POOL_ARRANGE_AFTER;
     prPoolArrangeInfo[FBM_POOL_TYPE_MPEG_C].ePool1 = u4AheadType;
 	u4AheadType = FBM_POOL_TYPE_MPEG_C;
	
 	prPoolList[FBM_POOL_TYPE_MPEG2_C].u4Inherit = FBM_POOL_ROOT;
     prPoolList[FBM_POOL_TYPE_MPEG2_C].u4PoolSize = FBM_MPEG2_2K_C_SIZE;
     prPoolArrangeInfo[FBM_POOL_TYPE_MPEG2_C].eMode = FBM_POOL_ARRANGE_AFTER;
     prPoolArrangeInfo[FBM_POOL_TYPE_MPEG2_C].ePool1 = u4AheadType;
 	u4AheadType = FBM_POOL_TYPE_MPEG2_C;
	 
#if defined(CC_FBM_SUPPORT_SWDMX)||defined(CC_FBM_SUPPORT_LITE_SWDMX)
	 prPoolList[FBM_POOL_TYPE_SWDMX].u4Inherit = FBM_POOL_ROOT;
	 prPoolList[FBM_POOL_TYPE_SWDMX].u4PoolSize = FBM_SWDMX_POOL_SIZE;
	 prPoolArrangeInfo[FBM_POOL_TYPE_SWDMX].eMode = FBM_POOL_ARRANGE_AFTER;
	 prPoolArrangeInfo[FBM_POOL_TYPE_SWDMX].ePool1 = u4AheadType;
	 u4AheadType = FBM_POOL_TYPE_SWDMX;
#endif
#if defined(CC_FBM_SUPPORT_PNG)		
			prPoolList[FBM_POOL_TYPE_PNG].u4Inherit = FBM_POOL_ROOT;
			prPoolList[FBM_POOL_TYPE_PNG].u4PoolSize = FBM_PNG_POOL_SIZE;
			prPoolArrangeInfo[FBM_POOL_TYPE_PNG].eMode = FBM_POOL_ARRANGE_AFTER;
			prPoolArrangeInfo[FBM_POOL_TYPE_PNG].ePool1 = u4AheadType;
			u4AheadType = FBM_POOL_TYPE_PNG;
#endif	

	prPoolList[FBM_POOL_TYPE_DRM].u4Inherit = FBM_POOL_ROOT;
	prPoolList[FBM_POOL_TYPE_DRM].u4PoolSize = FBM_DRM_BUF_SIZE;
	prPoolArrangeInfo[FBM_POOL_TYPE_DRM].eMode = FBM_POOL_ARRANGE_AFTER;
	prPoolArrangeInfo[FBM_POOL_TYPE_DRM].ePool1 = u4AheadType;
	u4AheadType = FBM_POOL_TYPE_DRM;

    // pq tool
    prPoolList[FBM_POOL_TYPE_PQ_TOOL].u4Inherit = FBM_POOL_ROOT;
	prPoolList[FBM_POOL_TYPE_PQ_TOOL].u4PoolSize = FBM_PQ_TOOL_POOL_SIZE; 
    prPoolArrangeInfo[FBM_POOL_TYPE_PQ_TOOL].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_PQ_TOOL].ePool1 = u4AheadType;
    u4AheadType = FBM_POOL_TYPE_PQ_TOOL;
	
		// handle Image Viewer Pool
		// JPEG | OSD1 | OSD2 | OSD3 | OSD4 (Share all except DSP)
		//
#ifndef CC_FBM_3D_OSD
		prPoolList[FBM_POOL_TYPE_JPEG_OSD1].u4PoolSize = FBM_OSD_POOL_SIZE(1920, 1088, 1, 4); // FHD, 32bpp, x1
		prPoolList[FBM_POOL_TYPE_JPEG_OSD1].u4Width = 1920;
		prPoolList[FBM_POOL_TYPE_JPEG_OSD1].u4Height = 1088;
		prPoolArrangeInfo[FBM_POOL_TYPE_JPEG_OSD1].eMode = FBM_POOL_ARRANGE_BEFORE;
		prPoolArrangeInfo[FBM_POOL_TYPE_JPEG_OSD1].ePool1 = FBM_POOL_TYPE_FEEDER_MMP;
	
		prPoolList[FBM_POOL_TYPE_JPEG_OSD2].u4PoolSize = FBM_OSD_POOL_SIZE(1920, 1088, 1, 4); // FHD, 32bpp, x1
		prPoolList[FBM_POOL_TYPE_JPEG_OSD2].u4Width = 1920;
		prPoolList[FBM_POOL_TYPE_JPEG_OSD2].u4Height = 1088;
		prPoolArrangeInfo[FBM_POOL_TYPE_JPEG_OSD2].eMode = FBM_POOL_ARRANGE_BEFORE;
		prPoolArrangeInfo[FBM_POOL_TYPE_JPEG_OSD2].ePool1 = FBM_POOL_TYPE_JPEG_OSD1;
#if !defined(CC_REMOVE_JPEG_OSD34)
		prPoolList[FBM_POOL_TYPE_JPEG_OSD3].u4PoolSize = FBM_OSD_POOL_SIZE(1920, 1088, 1, 4); // FHD, 32bpp, x1
		prPoolList[FBM_POOL_TYPE_JPEG_OSD3].u4Width = 1920;
		prPoolList[FBM_POOL_TYPE_JPEG_OSD3].u4Height = 1088;
		prPoolArrangeInfo[FBM_POOL_TYPE_JPEG_OSD3].eMode = FBM_POOL_ARRANGE_BEFORE;
		prPoolArrangeInfo[FBM_POOL_TYPE_JPEG_OSD3].ePool1 = FBM_POOL_TYPE_JPEG_OSD2;
	
		prPoolList[FBM_POOL_TYPE_JPEG_OSD4].u4PoolSize = FBM_OSD_POOL_SIZE(1920, 1088, 1, 4); // FHD, 32bpp, x1
		prPoolList[FBM_POOL_TYPE_JPEG_OSD4].u4Width = 1920;
		prPoolList[FBM_POOL_TYPE_JPEG_OSD4].u4Height = 1088;
		prPoolArrangeInfo[FBM_POOL_TYPE_JPEG_OSD4].eMode = FBM_POOL_ARRANGE_BEFORE;
		prPoolArrangeInfo[FBM_POOL_TYPE_JPEG_OSD4].ePool1 = FBM_POOL_TYPE_JPEG_OSD3;
		
		u4AheadType = FBM_POOL_TYPE_JPEG_OSD4;
#else
		u4AheadType = FBM_POOL_TYPE_JPEG_OSD2;
#endif
#else
		u4AheadType = FBM_POOL_TYPE_FEEDER_MMP;
#endif
	
#endif
#if defined(CC_FBM_3ND_CHANNEL) // 3 Channel
#ifdef CC_BYPASS_MJC
       u4AheadType = FBM_POOL_TYPE_TOTAL3;
#else
		prPoolList[FBM_POOL_TYPE_MJC_1].u4Inherit = FBM_POOL_ROOT;
		prPoolList[FBM_POOL_TYPE_MJC_1].u4PoolSize = FBM_MJC_RSV_POOL_SIZE;
		LOG(3, "FBMCFG CHANEL B: prPoolList[FBM_POOL_TYPE_MJC_1].u4PoolSize :%d \n", prPoolList[FBM_POOL_TYPE_MJC_1].u4PoolSize);
		prPoolList[FBM_POOL_TYPE_MJC_1].u4AddrAlign = FBM_MJC_ADDR_ALIGMENT;
		prPoolList[FBM_POOL_TYPE_MJC_1].u4Mode= FBM_POOL_MODE_MJC_Y10_C10_422_1RDC_3FRM;
		prPoolArrangeInfo[FBM_POOL_TYPE_MJC_1].eMode = FBM_POOL_ARRANGE_AFTER;
		prPoolArrangeInfo[FBM_POOL_TYPE_MJC_1].ePool1 = FBM_POOL_TYPE_TOTAL3;
		u4AheadType = FBM_POOL_TYPE_MJC_1;
#endif
#endif
}

static VOID _FbmConfigurePool_MT5890_4K_SCPOS_MAIN(POOL_LIST_T* prPoolList, POOL_ARRANGE_INFO_T* prPoolArrangeInfo)
{
    FBM_POOL_TYPE_T u4AheadType = FBM_POOL_TYPE_SCPOS_MAIN, u4BehindType, u4VdoBehindType = FBM_POOL_TYPE_NS;
    UNUSED(u4AheadType);
    UNUSED(u4BehindType);
    UNUSED(u4VdoBehindType);
    // arrange root pool ---------------------------------------------------

    // DMXPID |DSP|VBI|PVR|VENC|SCPOS_SUB|
    
	u4AheadType = FBM_POOL_TYPE_TOTAL;
#if defined(CC_FBM_SUPPORT_DMXPID)
		prPoolList[FBM_POOL_TYPE_DMXPID].u4Inherit = FBM_POOL_ROOT;
		prPoolList[FBM_POOL_TYPE_DMXPID].u4PoolSize = FBM_DMX_PID_BUF_SIZE;
		prPoolArrangeInfo[FBM_POOL_TYPE_DMXPID].eMode = FBM_POOL_ARRANGE_AFTER;
		prPoolArrangeInfo[FBM_POOL_TYPE_DMXPID].ePool1 = u4AheadType;
		u4AheadType = FBM_POOL_TYPE_DMXPID;
#endif

    // vbi
    prPoolList[FBM_POOL_TYPE_VBI].u4Inherit = FBM_POOL_ROOT;
    prPoolArrangeInfo[FBM_POOL_TYPE_VBI].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_VBI].ePool1 = u4AheadType;
    u4AheadType = FBM_POOL_TYPE_VBI;

#if defined(CC_FBM_SUPPORT_PVR)
	prPoolList[FBM_POOL_TYPE_PVR].u4Inherit = FBM_POOL_ROOT;
	prPoolList[FBM_POOL_TYPE_PVR].u4PoolSize = FBM_PVR_4K_SIZE;
	prPoolArrangeInfo[FBM_POOL_TYPE_PVR].eMode = FBM_POOL_ARRANGE_AFTER;
	prPoolArrangeInfo[FBM_POOL_TYPE_PVR].ePool1 = u4AheadType;
	u4AheadType = FBM_POOL_TYPE_PVR;
#endif

#ifdef CC_SUPPORT_RECORD_AV
	prPoolList[FBM_POOL_TYPE_TDC].u4Inherit = FBM_POOL_ROOT;
	prPoolList[FBM_POOL_TYPE_TDC].u4PoolSize = FBM_TDCAV_POOL_SIZE;
	prPoolArrangeInfo[FBM_POOL_TYPE_TDC].eMode = FBM_POOL_ARRANGE_AFTER;
	prPoolArrangeInfo[FBM_POOL_TYPE_TDC].ePool1 = u4AheadType;
	u4AheadType = FBM_POOL_TYPE_TDC;
#endif
	
	//-----PQ_3DC-----
	prPoolList[FBM_POOL_TYPE_PQ_3DC].u4Inherit = FBM_POOL_ROOT;
	prPoolList[FBM_POOL_TYPE_PQ_3DC].u4PoolSize = FBM_PQ_3DC_POOL_SIZE;
	prPoolArrangeInfo[FBM_POOL_TYPE_PQ_3DC].eMode = FBM_POOL_ARRANGE_AFTER;
	prPoolArrangeInfo[FBM_POOL_TYPE_PQ_3DC].ePool1 = u4AheadType;
	u4AheadType = FBM_POOL_TYPE_PQ_3DC;
	
    prPoolList[FBM_POOL_TYPE_SCPOS_PIP].u4Inherit = FBM_POOL_ROOT;
    prPoolList[FBM_POOL_TYPE_SCPOS_PIP].u4PoolSize = FBM_SCPOS_SUB_4K_SIZE;  
	prPoolArrangeInfo[FBM_POOL_TYPE_SCPOS_PIP].eMode = FBM_POOL_ARRANGE_AFTER;
	prPoolArrangeInfo[FBM_POOL_TYPE_SCPOS_PIP].ePool1 = u4AheadType;
	u4AheadType = FBM_POOL_TYPE_SCPOS_PIP;
	
	prPoolList[FBM_POOL_TYPE_JPEG].u4Inherit = FBM_POOL_ROOT;
	prPoolList[FBM_POOL_TYPE_JPEG].u4PoolSize = FBM_JPEG_POOL_SIZE;
	prPoolArrangeInfo[FBM_POOL_TYPE_JPEG].eMode = FBM_POOL_ARRANGE_AFTER;
	prPoolArrangeInfo[FBM_POOL_TYPE_JPEG].ePool1 = u4AheadType;
	u4AheadType = FBM_POOL_TYPE_JPEG;

#if defined(CC_SUPPORT_CIPLUS14)
	//----- Remux-----
	prPoolList[FBM_POOL_TYPE_REMUX].u4Inherit = FBM_POOL_ROOT;
	prPoolList[FBM_POOL_TYPE_REMUX].u4PoolSize = FBM_REMUX_POOL_SIZE;
	prPoolArrangeInfo[FBM_POOL_TYPE_REMUX].eMode = FBM_POOL_ARRANGE_AFTER;
	prPoolArrangeInfo[FBM_POOL_TYPE_REMUX].ePool1 = u4AheadType;   
	u4AheadType = FBM_POOL_TYPE_REMUX;
   #endif

   
#if defined(CC_SUPPORT_VSS)
   prPoolList[FBM_POOL_TYPE_VSS].u4Inherit = FBM_POOL_ROOT;
   prPoolList[FBM_POOL_TYPE_VSS].u4PoolSize = FBM_VSS_POOL_SIZE;
   prPoolArrangeInfo[FBM_POOL_TYPE_VSS].eMode = FBM_POOL_ARRANGE_AFTER;
   prPoolArrangeInfo[FBM_POOL_TYPE_VSS].ePool1 = u4AheadType;
   u4AheadType = FBM_POOL_TYPE_VSS;
#endif

#ifdef ADSP_BIN_SUPPORT
		prPoolList[FBM_POOL_TYPE_DSP_BIN].u4Inherit = FBM_POOL_ROOT;
		prPoolList[FBM_POOL_TYPE_DSP_BIN].u4PoolSize = FBM_DSP_BIN_POOL_SIZE;
		prPoolArrangeInfo[FBM_POOL_TYPE_DSP_BIN].eMode = FBM_POOL_ARRANGE_AFTER;
		prPoolArrangeInfo[FBM_POOL_TYPE_DSP_BIN].ePool1 = u4AheadType;
		u4AheadType = FBM_POOL_TYPE_DSP_BIN;
#endif		 

    //-----DSP-----
    prPoolList[FBM_POOL_TYPE_DSP].u4Inherit = FBM_POOL_ROOT;
    prPoolArrangeInfo[FBM_POOL_TYPE_DSP].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_DSP].ePool1 = u4AheadType;
    prPoolList[FBM_POOL_TYPE_DSP].u4PoolSize = FBM_DSP_POOL_SIZE;
    u4AheadType = FBM_POOL_TYPE_DSP;
	

//channel B
//DMX|MPEG|SCPOS_MAIN
#if defined(CC_FBM_2ND_CHANNEL) // 2 Channel
	
		 prPoolList[FBM_POOL_TYPE_FEEDER].u4PoolSize = FBM_FEEDER1_4K_SIZE;
		 prPoolList[FBM_POOL_TYPE_FEEDER].u4Inherit = FBM_POOL_ROOT;			
		 prPoolArrangeInfo[FBM_POOL_TYPE_FEEDER].eMode = FBM_POOL_ARRANGE_AFTER; 
		 prPoolArrangeInfo[FBM_POOL_TYPE_FEEDER].ePool1 = FBM_POOL_TYPE_TOTAL2;	
	
		 prPoolList[FBM_POOL_TYPE_FEEDER_MMP].u4PoolSize = FBM_FEEDER_POOL_SIZE;
		 prPoolArrangeInfo[FBM_POOL_TYPE_FEEDER_MMP].eMode = FBM_POOL_ARRANGE_AFTER;
		 prPoolArrangeInfo[FBM_POOL_TYPE_FEEDER_MMP].ePool1 = FBM_POOL_TYPE_TOTAL2;
		 
		 u4AheadType = FBM_POOL_TYPE_FEEDER;
		 
		 prPoolList[FBM_POOL_TYPE_FEEDER2].u4PoolSize = FBM_FEEDER2_4K_SIZE;
		 prPoolList[FBM_POOL_TYPE_FEEDER2].u4Inherit = FBM_POOL_ROOT;			
		 prPoolArrangeInfo[FBM_POOL_TYPE_FEEDER2].eMode = FBM_POOL_ARRANGE_AFTER; 
		 prPoolArrangeInfo[FBM_POOL_TYPE_FEEDER2].ePool1 = u4AheadType; 		
		 u4AheadType = FBM_POOL_TYPE_FEEDER2;
		 
		 prPoolList[FBM_POOL_TYPE_FEEDER3].u4PoolSize = FBM_FEEDER3_4K_SIZE;
		 prPoolList[FBM_POOL_TYPE_FEEDER3].u4Inherit = FBM_POOL_ROOT;			
		 prPoolArrangeInfo[FBM_POOL_TYPE_FEEDER3].eMode = FBM_POOL_ARRANGE_AFTER; 
		 prPoolArrangeInfo[FBM_POOL_TYPE_FEEDER3].ePool1 = u4AheadType; 		
		 u4AheadType = FBM_POOL_TYPE_FEEDER3;

#if defined(CC_FBM_SUPPORT_SWDMX)||defined(CC_FBM_SUPPORT_LITE_SWDMX)
		 prPoolList[FBM_POOL_TYPE_SWDMX].u4Inherit = FBM_POOL_ROOT;
		 prPoolList[FBM_POOL_TYPE_SWDMX].u4PoolSize = FBM_SWDMX_POOL_SIZE;
		 prPoolArrangeInfo[FBM_POOL_TYPE_SWDMX].eMode = FBM_POOL_ARRANGE_AFTER;
		 prPoolArrangeInfo[FBM_POOL_TYPE_SWDMX].ePool1 = u4AheadType;
		 u4AheadType = FBM_POOL_TYPE_SWDMX;
#endif
#if defined(CC_FBM_SUPPORT_PNG)		
		 prPoolList[FBM_POOL_TYPE_PNG].u4Inherit = FBM_POOL_ROOT;
		 prPoolList[FBM_POOL_TYPE_PNG].u4PoolSize = FBM_PNG_POOL_SIZE;
		 prPoolArrangeInfo[FBM_POOL_TYPE_PNG].eMode = FBM_POOL_ARRANGE_AFTER;
		 prPoolArrangeInfo[FBM_POOL_TYPE_PNG].ePool1 = u4AheadType;
		 u4AheadType = FBM_POOL_TYPE_PNG;
#endif	
    #if defined(CC_FBM_SUPPORT_SRS)
			//----- SRS-----
			prPoolList[FBM_POOL_TYPE_SRS].u4Inherit = FBM_POOL_ROOT;
			prPoolList[FBM_POOL_TYPE_SRS].u4PoolSize = FBM_SRS_SIZE;
			prPoolArrangeInfo[FBM_POOL_TYPE_SRS].eMode = FBM_POOL_ARRANGE_AFTER;
			prPoolArrangeInfo[FBM_POOL_TYPE_SRS].ePool1 = u4AheadType;
			u4AheadType = FBM_POOL_TYPE_SRS;
   #endif
   #if defined(CC_FBM_3D_OSD) || defined(CC_MHP_SUPPORT) //MHP use jpeg_vdp buffer
			prPoolList[FBM_POOL_TYPE_JPG_VDP].u4Inherit = FBM_POOL_ROOT;
			prPoolList[FBM_POOL_TYPE_JPG_VDP].u4PoolSize = FBM_JPEVDP_POOL_SIZE;
			prPoolArrangeInfo[FBM_POOL_TYPE_JPG_VDP].eMode = FBM_POOL_ARRANGE_AFTER;
			prPoolArrangeInfo[FBM_POOL_TYPE_JPG_VDP].ePool1 = u4AheadType;
			u4AheadType = FBM_POOL_TYPE_JPG_VDP;
#endif
    
#if defined(CC_SUPPORT_W2DR2R)
	//----- W2D R2R-----
    prPoolList[FBM_POOL_TYPE_W2D_R2R].u4Inherit = FBM_POOL_ROOT;
	prPoolList[FBM_POOL_TYPE_W2D_R2R].u4PoolSize = FBM_W2DR2R_POOL_SIZE;
    prPoolArrangeInfo[FBM_POOL_TYPE_W2D_R2R].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_W2D_R2R].ePool1 = u4AheadType;
	u4AheadType = FBM_POOL_TYPE_W2D_R2R;
#endif
		// pq tool
		prPoolList[FBM_POOL_TYPE_PQ_TOOL].u4Inherit = FBM_POOL_ROOT;
        prPoolList[FBM_POOL_TYPE_PQ_TOOL].u4PoolSize = FBM_PQ_TOOL_POOL_SIZE; 
		prPoolArrangeInfo[FBM_POOL_TYPE_PQ_TOOL].eMode = FBM_POOL_ARRANGE_AFTER;
		prPoolArrangeInfo[FBM_POOL_TYPE_PQ_TOOL].ePool1 = u4AheadType;
		u4AheadType = FBM_POOL_TYPE_PQ_TOOL;
		
			// handle Image Viewer Pool
			// JPEG | OSD1 | OSD2 | OSD3 | OSD4 (Share all except DSP)
			//
#ifndef CC_FBM_3D_OSD
			prPoolList[FBM_POOL_TYPE_JPEG_OSD1].u4PoolSize = FBM_OSD_POOL_SIZE(1920, 1088, 1, 4); // FHD, 32bpp, x1
			prPoolList[FBM_POOL_TYPE_JPEG_OSD1].u4Width = 1920;
			prPoolList[FBM_POOL_TYPE_JPEG_OSD1].u4Height = 1088;
			prPoolArrangeInfo[FBM_POOL_TYPE_JPEG_OSD1].eMode = FBM_POOL_ARRANGE_BEFORE;
			prPoolArrangeInfo[FBM_POOL_TYPE_JPEG_OSD1].ePool1 = FBM_POOL_TYPE_FEEDER_MMP;
		
			prPoolList[FBM_POOL_TYPE_JPEG_OSD2].u4PoolSize = FBM_OSD_POOL_SIZE(1920, 1088, 1, 4); // FHD, 32bpp, x1
			prPoolList[FBM_POOL_TYPE_JPEG_OSD2].u4Width = 1920;
			prPoolList[FBM_POOL_TYPE_JPEG_OSD2].u4Height = 1088;
			prPoolArrangeInfo[FBM_POOL_TYPE_JPEG_OSD2].eMode = FBM_POOL_ARRANGE_BEFORE;
			prPoolArrangeInfo[FBM_POOL_TYPE_JPEG_OSD2].ePool1 = FBM_POOL_TYPE_JPEG_OSD1;
#if !defined(CC_REMOVE_JPEG_OSD34)
			prPoolList[FBM_POOL_TYPE_JPEG_OSD3].u4PoolSize = FBM_OSD_POOL_SIZE(1920, 1088, 1, 4); // FHD, 32bpp, x1
			prPoolList[FBM_POOL_TYPE_JPEG_OSD3].u4Width = 1920;
			prPoolList[FBM_POOL_TYPE_JPEG_OSD3].u4Height = 1088;
			prPoolArrangeInfo[FBM_POOL_TYPE_JPEG_OSD3].eMode = FBM_POOL_ARRANGE_BEFORE;
			prPoolArrangeInfo[FBM_POOL_TYPE_JPEG_OSD3].ePool1 = FBM_POOL_TYPE_JPEG_OSD2;
		
			prPoolList[FBM_POOL_TYPE_JPEG_OSD4].u4PoolSize = FBM_OSD_POOL_SIZE(1920, 1088, 1, 4); // FHD, 32bpp, x1
			prPoolList[FBM_POOL_TYPE_JPEG_OSD4].u4Width = 1920;
			prPoolList[FBM_POOL_TYPE_JPEG_OSD4].u4Height = 1088;
			prPoolArrangeInfo[FBM_POOL_TYPE_JPEG_OSD4].eMode = FBM_POOL_ARRANGE_BEFORE;
			prPoolArrangeInfo[FBM_POOL_TYPE_JPEG_OSD4].ePool1 = FBM_POOL_TYPE_JPEG_OSD3;
			
			u4AheadType = FBM_POOL_TYPE_JPEG_OSD4;
#else
			u4AheadType = FBM_POOL_TYPE_JPEG_OSD2;
#endif
#else
			u4AheadType = FBM_POOL_TYPE_FEEDER_MMP;
#endif
		
#endif
    u4AheadType = FBM_POOL_TYPE_PQ_TOOL;
    //----- DMX-----
    prPoolList[FBM_POOL_TYPE_DMX].u4Inherit = FBM_POOL_ROOT;
    prPoolList[FBM_POOL_TYPE_DMX].u4PoolSize = FBM_DMX1_4K_SIZE;
    prPoolArrangeInfo[FBM_POOL_TYPE_DMX].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_DMX].ePool1 = u4AheadType;   
    u4AheadType = FBM_POOL_TYPE_DMX;
    //----- DMX2-----
    prPoolList[FBM_POOL_TYPE_DMX2].u4Inherit = FBM_POOL_ROOT;
    prPoolList[FBM_POOL_TYPE_DMX2].u4PoolSize = FBM_DMX2_4K_SIZE;
    prPoolArrangeInfo[FBM_POOL_TYPE_DMX2].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_DMX2].ePool1 = u4AheadType;   
    u4AheadType = FBM_POOL_TYPE_DMX2;
	
	//----- DMX3-----
    prPoolList[FBM_POOL_TYPE_DMX3].u4Inherit = FBM_POOL_ROOT;
    prPoolList[FBM_POOL_TYPE_DMX3].u4PoolSize = FBM_DMX3_4K_SIZE;
    prPoolArrangeInfo[FBM_POOL_TYPE_DMX3].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_DMX3].ePool1 = u4AheadType;   
	u4AheadType = FBM_POOL_TYPE_DMX3;

#if defined(CC_FBM_SECURE_FEEDER)
		  prPoolList[FBM_POOL_TYPE_SECURE_FEEDER].u4PoolSize = FBM_SECURE_FEEDER_4K_SIZE;
		  prPoolList[FBM_POOL_TYPE_SECURE_FEEDER].u4Inherit = FBM_POOL_ROOT;			 
		  prPoolArrangeInfo[FBM_POOL_TYPE_SECURE_FEEDER].eMode = FBM_POOL_ARRANGE_AFTER; 
		  prPoolArrangeInfo[FBM_POOL_TYPE_SECURE_FEEDER].ePool1 = u4AheadType;			 
		  u4AheadType = FBM_POOL_TYPE_SECURE_FEEDER;
#endif
#ifdef CC_SUPPORT_OD    //if (DRVCUST_OptGet(eFlagSupportOD) == TRUE)
		prPoolList[FBM_POOL_TYPE_OD].u4Inherit = FBM_POOL_ROOT;
		prPoolList[FBM_POOL_TYPE_OD].u4PoolSize = FBM_OD_POOL_SIZE;
		prPoolArrangeInfo[FBM_POOL_TYPE_OD].eMode = FBM_POOL_ARRANGE_AFTER;
		prPoolArrangeInfo[FBM_POOL_TYPE_OD].ePool1 = u4AheadType;
		u4AheadType = FBM_POOL_TYPE_OD;
#endif
    //----- MPEG-----
	prPoolList[FBM_POOL_TYPE_MPEG].u4Inherit = FBM_POOL_ROOT;
	prPoolList[FBM_POOL_TYPE_MPEG].u4PoolSize = FBM_MPEG1_4K_Y_SIZE;	
	prPoolList[FBM_POOL_TYPE_MPEG].u4Mode = (FBM_POOL_MODE_MPEG_DBK | FBM_POOL_MODE_SHARE);
	prPoolArrangeInfo[FBM_POOL_TYPE_MPEG].eMode = FBM_POOL_ARRANGE_AFTER;
	prPoolArrangeInfo[FBM_POOL_TYPE_MPEG].ePool1 = u4AheadType;
	u4AheadType = FBM_POOL_TYPE_MPEG;
    
	//----- MPEG2-----
    prPoolList[FBM_POOL_TYPE_MPEG2].u4Inherit = FBM_POOL_ROOT;
	prPoolList[FBM_POOL_TYPE_MPEG2].u4PoolSize = FBM_MPEG2_4K_Y_SIZE;
    prPoolList[FBM_POOL_TYPE_MPEG2].u4Mode = (FBM_POOL_MODE_MPEG_DBK | FBM_POOL_MODE_SHARE);
    prPoolArrangeInfo[FBM_POOL_TYPE_MPEG2].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_MPEG2].ePool1 = u4AheadType;
    u4AheadType = FBM_POOL_TYPE_MPEG2;


    //----- MPEG3-----
    prPoolList[FBM_POOL_TYPE_MPEG3].u4Inherit = FBM_POOL_ROOT;
	prPoolList[FBM_POOL_TYPE_MPEG3].u4PoolSize = FBM_MPEG3_4K_Y_SIZE;
    prPoolList[FBM_POOL_TYPE_MPEG3].u4Mode = (FBM_POOL_MODE_MPEG_DBK | FBM_POOL_MODE_SHARE);
    prPoolArrangeInfo[FBM_POOL_TYPE_MPEG3].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_MPEG3].ePool1 = u4AheadType;
	u4AheadType = FBM_POOL_TYPE_MPEG3;
	
    // -----MPEG4-----
	prPoolList[FBM_POOL_TYPE_MPEG4].u4Inherit = FBM_POOL_ROOT;
    prPoolList[FBM_POOL_TYPE_MPEG4].u4PoolSize = FBM_MPEG4_4K_Y_SIZE;
    prPoolArrangeInfo[FBM_POOL_TYPE_MPEG4].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_MPEG4].ePool1 = u4AheadType;
	u4AheadType = FBM_POOL_TYPE_MPEG4;

    //----- SCPOS MAIN-----
    prPoolList[FBM_POOL_TYPE_SCPOS_MAIN].u4Inherit = FBM_POOL_ROOT;
    prPoolList[FBM_POOL_TYPE_SCPOS_MAIN].u4PoolSize = FBM_SCPOS_MAIN_4K_SIZE; //main 1080i 422 0xe9c0000*2+sub 0x880000
    prPoolArrangeInfo[FBM_POOL_TYPE_SCPOS_MAIN].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_SCPOS_MAIN].ePool1 = u4AheadType;
    u4AheadType = FBM_POOL_TYPE_SCPOS_MAIN;

	prPoolList[FBM_POOL_TYPE_DRM].u4Inherit = FBM_POOL_ROOT;
	prPoolList[FBM_POOL_TYPE_DRM].u4PoolSize = FBM_DRM_BUF_SIZE;
	prPoolArrangeInfo[FBM_POOL_TYPE_DRM].eMode = FBM_POOL_ARRANGE_AFTER;
	prPoolArrangeInfo[FBM_POOL_TYPE_DRM].ePool1 = u4AheadType;
	u4AheadType = FBM_POOL_TYPE_DRM;


// MPEG_C|MJC|OD|PNG|JPEG|VSS|FEEDER|DRM|PQ_3DC|SWDMX
#if defined(CC_FBM_3ND_CHANNEL) // 3 Channel
#ifdef CC_BYPASS_MJC
       u4AheadType = FBM_POOL_TYPE_TOTAL3;
#else
		prPoolList[FBM_POOL_TYPE_MJC_1].u4Inherit = FBM_POOL_ROOT;
		prPoolList[FBM_POOL_TYPE_MJC_1].u4PoolSize = FBM_MJC_RSV_POOL_SIZE;
		LOG(3, "FBMCFG CHANEL B: prPoolList[FBM_POOL_TYPE_MJC_1].u4PoolSize :%d \n", prPoolList[FBM_POOL_TYPE_MJC_1].u4PoolSize);
		prPoolList[FBM_POOL_TYPE_MJC_1].u4AddrAlign = FBM_MJC_ADDR_ALIGMENT;
		prPoolList[FBM_POOL_TYPE_MJC_1].u4Mode= FBM_POOL_MODE_MJC_Y10_C10_422_1RDC_3FRM;
		prPoolArrangeInfo[FBM_POOL_TYPE_MJC_1].eMode = FBM_POOL_ARRANGE_AFTER;
		prPoolArrangeInfo[FBM_POOL_TYPE_MJC_1].ePool1 = FBM_POOL_TYPE_TOTAL3;
		u4AheadType = FBM_POOL_TYPE_MJC_1;
#endif

        prPoolList[FBM_POOL_TYPE_MPEG_C].u4Inherit = FBM_POOL_ROOT;
        prPoolList[FBM_POOL_TYPE_MPEG_C].u4PoolSize = FBM_MPEG1_4K_C_SIZE;
        prPoolArrangeInfo[FBM_POOL_TYPE_MPEG_C].eMode = FBM_POOL_ARRANGE_AFTER;
        prPoolArrangeInfo[FBM_POOL_TYPE_MPEG_C].ePool1 = u4AheadType;
		u4AheadType = FBM_POOL_TYPE_MPEG_C;
		
		prPoolList[FBM_POOL_TYPE_MPEG2_C].u4Inherit = FBM_POOL_ROOT;
        prPoolList[FBM_POOL_TYPE_MPEG2_C].u4PoolSize = FBM_MPEG2_4K_C_SIZE;
        prPoolArrangeInfo[FBM_POOL_TYPE_MPEG2_C].eMode = FBM_POOL_ARRANGE_AFTER;
        prPoolArrangeInfo[FBM_POOL_TYPE_MPEG2_C].ePool1 = u4AheadType;
		u4AheadType = FBM_POOL_TYPE_MPEG2_C;

		prPoolList[FBM_POOL_TYPE_MPEG3_C].u4Inherit = FBM_POOL_ROOT;
        prPoolList[FBM_POOL_TYPE_MPEG3_C].u4PoolSize = FBM_MPEG3_4K_C_SIZE;
        prPoolArrangeInfo[FBM_POOL_TYPE_MPEG3_C].eMode = FBM_POOL_ARRANGE_AFTER;
        prPoolArrangeInfo[FBM_POOL_TYPE_MPEG3_C].ePool1 = u4AheadType;
		u4AheadType = FBM_POOL_TYPE_MPEG3_C;

		prPoolList[FBM_POOL_TYPE_MPEG4_C].u4Inherit = FBM_POOL_ROOT;
        prPoolList[FBM_POOL_TYPE_MPEG4_C].u4PoolSize = FBM_MPEG4_4K_C_SIZE;
        prPoolArrangeInfo[FBM_POOL_TYPE_MPEG4_C].eMode = FBM_POOL_ARRANGE_AFTER;
        prPoolArrangeInfo[FBM_POOL_TYPE_MPEG4_C].ePool1 = u4AheadType;
		u4AheadType = FBM_POOL_TYPE_MPEG4_C;
		
#if defined(CC_SUPPORT_W2DR2R)
		//----- W2D R2R Y-----
		prPoolList[FBM_POOL_TYPE_W2D_R2R_Y].u4Inherit = FBM_POOL_ROOT;
		prPoolList[FBM_POOL_TYPE_W2D_R2R_Y].u4PoolSize = FBM_W2DR2R_POOL_Y_SIZE;
		prPoolArrangeInfo[FBM_POOL_TYPE_W2D_R2R_Y].eMode = FBM_POOL_ARRANGE_AFTER;
		prPoolArrangeInfo[FBM_POOL_TYPE_W2D_R2R_Y].ePool1 = u4AheadType;
		u4AheadType = FBM_POOL_TYPE_W2D_R2R_Y;
#endif  

#if defined(CC_SUPPORT_VENC)
	prPoolList[FBM_POOL_TYPE_VENC].u4Inherit = FBM_POOL_ROOT;
	prPoolList[FBM_POOL_TYPE_VENC].u4PoolSize = FBM_VENC_POOL_SIZE;
	prPoolArrangeInfo[FBM_POOL_TYPE_VENC].eMode = FBM_POOL_ARRANGE_AFTER;
	prPoolArrangeInfo[FBM_POOL_TYPE_VENC].ePool1 = u4AheadType;
	u4AheadType = FBM_POOL_TYPE_VENC;
#endif

#endif
}

static VOID _FbmConfigurePool_MT5890_MJC_SCPOS_MAIN(POOL_LIST_T* prPoolList, POOL_ARRANGE_INFO_T* prPoolArrangeInfo)
{
    FBM_POOL_TYPE_T u4AheadType = FBM_POOL_TYPE_SCPOS_MAIN, u4BehindType, u4VdoBehindType = FBM_POOL_TYPE_NS;
    UNUSED(u4AheadType);
    UNUSED(u4BehindType);
    UNUSED(u4VdoBehindType);
    // arrange root pool ---------------------------------------------------

    // DMXPID |DSP|VBI|PVR|VENC|SCPOS_SUB|
    
	u4AheadType = FBM_POOL_TYPE_TOTAL;
#if defined(CC_FBM_SUPPORT_DMXPID)
		prPoolList[FBM_POOL_TYPE_DMXPID].u4Inherit = FBM_POOL_ROOT;
		prPoolList[FBM_POOL_TYPE_DMXPID].u4PoolSize = FBM_DMX_PID_BUF_SIZE;
		prPoolArrangeInfo[FBM_POOL_TYPE_DMXPID].eMode = FBM_POOL_ARRANGE_AFTER;
		prPoolArrangeInfo[FBM_POOL_TYPE_DMXPID].ePool1 = u4AheadType;
		u4AheadType = FBM_POOL_TYPE_DMXPID;
#endif

    // vbi
    prPoolList[FBM_POOL_TYPE_VBI].u4Inherit = FBM_POOL_ROOT;
    prPoolArrangeInfo[FBM_POOL_TYPE_VBI].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_VBI].ePool1 = u4AheadType;
    u4AheadType = FBM_POOL_TYPE_VBI;

#if defined(CC_FBM_SUPPORT_PVR)
	prPoolList[FBM_POOL_TYPE_PVR].u4Inherit = FBM_POOL_ROOT;
	prPoolList[FBM_POOL_TYPE_PVR].u4PoolSize = FBM_PVR_POOL_SIZE;
	prPoolArrangeInfo[FBM_POOL_TYPE_PVR].eMode = FBM_POOL_ARRANGE_AFTER;
	prPoolArrangeInfo[FBM_POOL_TYPE_PVR].ePool1 = u4AheadType;
	u4AheadType = FBM_POOL_TYPE_PVR;
#endif

#if defined(CC_SUPPORT_VENC)
	prPoolList[FBM_POOL_TYPE_VENC].u4Inherit = FBM_POOL_ROOT;
	prPoolList[FBM_POOL_TYPE_VENC].u4PoolSize = FBM_VENC_POOL_SIZE;
	prPoolArrangeInfo[FBM_POOL_TYPE_VENC].eMode = FBM_POOL_ARRANGE_AFTER;
	prPoolArrangeInfo[FBM_POOL_TYPE_VENC].ePool1 = u4AheadType;
	u4AheadType = FBM_POOL_TYPE_VENC;
#endif
#ifdef CC_SUPPORT_RECORD_AV
	prPoolList[FBM_POOL_TYPE_TDC].u4Inherit = FBM_POOL_ROOT;
	prPoolList[FBM_POOL_TYPE_TDC].u4PoolSize = FBM_TDCAV_POOL_SIZE;
	prPoolArrangeInfo[FBM_POOL_TYPE_TDC].eMode = FBM_POOL_ARRANGE_AFTER;
	prPoolArrangeInfo[FBM_POOL_TYPE_TDC].ePool1 = u4AheadType;
	u4AheadType = FBM_POOL_TYPE_TDC;
#endif
	
	//-----PQ_3DC-----
	prPoolList[FBM_POOL_TYPE_PQ_3DC].u4Inherit = FBM_POOL_ROOT;
	prPoolList[FBM_POOL_TYPE_PQ_3DC].u4PoolSize = FBM_PQ_3DC_POOL_SIZE;
	prPoolArrangeInfo[FBM_POOL_TYPE_PQ_3DC].eMode = FBM_POOL_ARRANGE_AFTER;
	prPoolArrangeInfo[FBM_POOL_TYPE_PQ_3DC].ePool1 = u4AheadType;
	u4AheadType = FBM_POOL_TYPE_PQ_3DC;
	
#if 1
		prPoolList[FBM_POOL_TYPE_JPEG].u4Inherit = FBM_POOL_ROOT;
		prPoolList[FBM_POOL_TYPE_JPEG].u4PoolSize = FBM_JPEG_POOL_SIZE;
		prPoolArrangeInfo[FBM_POOL_TYPE_JPEG].eMode = FBM_POOL_ARRANGE_AFTER;
		prPoolArrangeInfo[FBM_POOL_TYPE_JPEG].ePool1 = u4AheadType;
		u4AheadType = FBM_POOL_TYPE_JPEG;
#endif	
#ifdef ADSP_BIN_SUPPORT
		prPoolList[FBM_POOL_TYPE_DSP_BIN].u4Inherit = FBM_POOL_ROOT;
		prPoolList[FBM_POOL_TYPE_DSP_BIN].u4PoolSize = FBM_DSP_BIN_POOL_SIZE;
		prPoolArrangeInfo[FBM_POOL_TYPE_DSP_BIN].eMode = FBM_POOL_ARRANGE_AFTER;
		prPoolArrangeInfo[FBM_POOL_TYPE_DSP_BIN].ePool1 = u4AheadType;
		u4AheadType = FBM_POOL_TYPE_DSP_BIN;
#endif		 

    //-----DSP-----
    prPoolList[FBM_POOL_TYPE_DSP].u4Inherit = FBM_POOL_ROOT;
    prPoolArrangeInfo[FBM_POOL_TYPE_DSP].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_DSP].ePool1 = u4AheadType;
    prPoolList[FBM_POOL_TYPE_DSP].u4PoolSize = FBM_DSP_POOL_SIZE;
    u4AheadType = FBM_POOL_TYPE_DSP;
	

//channel B
//DMX|MPEG|SCPOS_MAIN
#if defined(CC_FBM_2ND_CHANNEL) // 2 Channel
	
		 prPoolList[FBM_POOL_TYPE_FEEDER].u4PoolSize = FBM_FEEDER1_SIZE;
		 prPoolList[FBM_POOL_TYPE_FEEDER].u4Inherit = FBM_POOL_ROOT;			
		 prPoolArrangeInfo[FBM_POOL_TYPE_FEEDER].eMode = FBM_POOL_ARRANGE_AFTER; 
		 prPoolArrangeInfo[FBM_POOL_TYPE_FEEDER].ePool1 = FBM_POOL_TYPE_TOTAL2;	
	
		 prPoolList[FBM_POOL_TYPE_FEEDER_MMP].u4PoolSize = FBM_FEEDER_POOL_SIZE;
		 prPoolArrangeInfo[FBM_POOL_TYPE_FEEDER_MMP].eMode = FBM_POOL_ARRANGE_AFTER;
		 prPoolArrangeInfo[FBM_POOL_TYPE_FEEDER_MMP].ePool1 = FBM_POOL_TYPE_TOTAL2;
		 
		 u4AheadType = FBM_POOL_TYPE_FEEDER;
     #if defined(CC_FBM_TWO_FBP)
		 prPoolList[FBM_POOL_TYPE_FEEDER2].u4PoolSize = FBM_FEEDER2_SIZE;
		 prPoolList[FBM_POOL_TYPE_FEEDER2].u4Inherit = FBM_POOL_ROOT;			
		 prPoolArrangeInfo[FBM_POOL_TYPE_FEEDER2].eMode = FBM_POOL_ARRANGE_AFTER; 
		 prPoolArrangeInfo[FBM_POOL_TYPE_FEEDER2].ePool1 = u4AheadType; 		
		 u4AheadType = FBM_POOL_TYPE_FEEDER2;
	 #endif
		 
     #if defined(CC_FBM_TWO_4K2K)||defined(CC_FBM_FOUR_INST_SUPPORT)
		 prPoolList[FBM_POOL_TYPE_FEEDER3].u4PoolSize = FBM_FEEDER3_SIZE;
		 prPoolList[FBM_POOL_TYPE_FEEDER3].u4Inherit = FBM_POOL_ROOT;			
		 prPoolArrangeInfo[FBM_POOL_TYPE_FEEDER3].eMode = FBM_POOL_ARRANGE_AFTER; 
		 prPoolArrangeInfo[FBM_POOL_TYPE_FEEDER3].ePool1 = u4AheadType; 		
		 u4AheadType = FBM_POOL_TYPE_FEEDER3;
	
		 prPoolList[FBM_POOL_TYPE_FEEDER4].u4PoolSize = FBM_FEEDER4_SIZE;
		 prPoolList[FBM_POOL_TYPE_FEEDER4].u4Inherit = FBM_POOL_ROOT;			
		 prPoolArrangeInfo[FBM_POOL_TYPE_FEEDER4].eMode = FBM_POOL_ARRANGE_AFTER; 
		 prPoolArrangeInfo[FBM_POOL_TYPE_FEEDER4].ePool1 = u4AheadType; 		
		 u4AheadType = FBM_POOL_TYPE_FEEDER4;
	 #endif
		 
#if defined(CC_FBM_SUPPORT_SWDMX)||defined(CC_FBM_SUPPORT_LITE_SWDMX)
		 prPoolList[FBM_POOL_TYPE_SWDMX].u4Inherit = FBM_POOL_ROOT;
		 prPoolList[FBM_POOL_TYPE_SWDMX].u4PoolSize = FBM_SWDMX_POOL_SIZE;
		 prPoolArrangeInfo[FBM_POOL_TYPE_SWDMX].eMode = FBM_POOL_ARRANGE_AFTER;
		 prPoolArrangeInfo[FBM_POOL_TYPE_SWDMX].ePool1 = u4AheadType;
		 u4AheadType = FBM_POOL_TYPE_SWDMX;
#endif
#if defined(CC_FBM_SUPPORT_PNG)		
		 prPoolList[FBM_POOL_TYPE_PNG].u4Inherit = FBM_POOL_ROOT;
		 prPoolList[FBM_POOL_TYPE_PNG].u4PoolSize = FBM_PNG_POOL_SIZE;
		 prPoolArrangeInfo[FBM_POOL_TYPE_PNG].eMode = FBM_POOL_ARRANGE_AFTER;
		 prPoolArrangeInfo[FBM_POOL_TYPE_PNG].ePool1 = u4AheadType;
		 u4AheadType = FBM_POOL_TYPE_PNG;
#endif	
#if defined(CC_SUPPORT_CIPLUS14)
				//----- Remux-----
				prPoolList[FBM_POOL_TYPE_REMUX].u4Inherit = FBM_POOL_ROOT;
				prPoolList[FBM_POOL_TYPE_REMUX].u4PoolSize = FBM_REMUX_POOL_SIZE;
				prPoolArrangeInfo[FBM_POOL_TYPE_REMUX].eMode = FBM_POOL_ARRANGE_AFTER;
				prPoolArrangeInfo[FBM_POOL_TYPE_REMUX].ePool1 = u4AheadType;   
				u4AheadType = FBM_POOL_TYPE_REMUX;
   #endif
    #if defined(CC_FBM_SUPPORT_SRS)
			//----- SRS-----
			prPoolList[FBM_POOL_TYPE_SRS].u4Inherit = FBM_POOL_ROOT;
			prPoolList[FBM_POOL_TYPE_SRS].u4PoolSize = FBM_SRS_SIZE;
			prPoolArrangeInfo[FBM_POOL_TYPE_SRS].eMode = FBM_POOL_ARRANGE_AFTER;
			prPoolArrangeInfo[FBM_POOL_TYPE_SRS].ePool1 = u4AheadType;
			u4AheadType = FBM_POOL_TYPE_SRS;
   #endif
   #if defined(CC_FBM_3D_OSD) || defined(CC_MHP_SUPPORT) //MHP use jpeg_vdp buffer
			prPoolList[FBM_POOL_TYPE_JPG_VDP].u4Inherit = FBM_POOL_ROOT;
			prPoolList[FBM_POOL_TYPE_JPG_VDP].u4PoolSize = FBM_JPEVDP_POOL_SIZE;
			prPoolArrangeInfo[FBM_POOL_TYPE_JPG_VDP].eMode = FBM_POOL_ARRANGE_AFTER;
			prPoolArrangeInfo[FBM_POOL_TYPE_JPG_VDP].ePool1 = u4AheadType;
			u4AheadType = FBM_POOL_TYPE_JPG_VDP;
#endif
    
#if defined(CC_SUPPORT_W2DR2R)
	//----- W2D R2R-----
    prPoolList[FBM_POOL_TYPE_W2D_R2R].u4Inherit = FBM_POOL_ROOT;
	prPoolList[FBM_POOL_TYPE_W2D_R2R].u4PoolSize = FBM_W2DR2R_POOL_SIZE;
    prPoolArrangeInfo[FBM_POOL_TYPE_W2D_R2R].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_W2D_R2R].ePool1 = u4AheadType;
	u4AheadType = FBM_POOL_TYPE_W2D_R2R;
#endif
		// pq tool
		prPoolList[FBM_POOL_TYPE_PQ_TOOL].u4Inherit = FBM_POOL_ROOT;
        prPoolList[FBM_POOL_TYPE_PQ_TOOL].u4PoolSize = FBM_PQ_TOOL_POOL_SIZE;
		prPoolArrangeInfo[FBM_POOL_TYPE_PQ_TOOL].eMode = FBM_POOL_ARRANGE_AFTER;
		prPoolArrangeInfo[FBM_POOL_TYPE_PQ_TOOL].ePool1 = u4AheadType;
		u4AheadType = FBM_POOL_TYPE_PQ_TOOL;
		
			// handle Image Viewer Pool
			// JPEG | OSD1 | OSD2 | OSD3 | OSD4 (Share all except DSP)
	#if 0		//
#ifndef CC_FBM_3D_OSD
			prPoolList[FBM_POOL_TYPE_JPEG_OSD1].u4PoolSize = FBM_OSD_POOL_SIZE(1920, 1088, 1, 4); // FHD, 32bpp, x1
			prPoolList[FBM_POOL_TYPE_JPEG_OSD1].u4Width = 1920;
			prPoolList[FBM_POOL_TYPE_JPEG_OSD1].u4Height = 1088;
			prPoolArrangeInfo[FBM_POOL_TYPE_JPEG_OSD1].eMode = FBM_POOL_ARRANGE_BEFORE;
			prPoolArrangeInfo[FBM_POOL_TYPE_JPEG_OSD1].ePool1 = FBM_POOL_TYPE_FEEDER_MMP;
		
			prPoolList[FBM_POOL_TYPE_JPEG_OSD2].u4PoolSize = FBM_OSD_POOL_SIZE(1920, 1088, 1, 4); // FHD, 32bpp, x1
			prPoolList[FBM_POOL_TYPE_JPEG_OSD2].u4Width = 1920;
			prPoolList[FBM_POOL_TYPE_JPEG_OSD2].u4Height = 1088;
			prPoolArrangeInfo[FBM_POOL_TYPE_JPEG_OSD2].eMode = FBM_POOL_ARRANGE_BEFORE;
			prPoolArrangeInfo[FBM_POOL_TYPE_JPEG_OSD2].ePool1 = FBM_POOL_TYPE_JPEG_OSD1;
#if !defined(CC_REMOVE_JPEG_OSD34)
			prPoolList[FBM_POOL_TYPE_JPEG_OSD3].u4PoolSize = FBM_OSD_POOL_SIZE(1920, 1088, 1, 4); // FHD, 32bpp, x1
			prPoolList[FBM_POOL_TYPE_JPEG_OSD3].u4Width = 1920;
			prPoolList[FBM_POOL_TYPE_JPEG_OSD3].u4Height = 1088;
			prPoolArrangeInfo[FBM_POOL_TYPE_JPEG_OSD3].eMode = FBM_POOL_ARRANGE_BEFORE;
			prPoolArrangeInfo[FBM_POOL_TYPE_JPEG_OSD3].ePool1 = FBM_POOL_TYPE_JPEG_OSD2;
		
			prPoolList[FBM_POOL_TYPE_JPEG_OSD4].u4PoolSize = FBM_OSD_POOL_SIZE(1920, 1088, 1, 4); // FHD, 32bpp, x1
			prPoolList[FBM_POOL_TYPE_JPEG_OSD4].u4Width = 1920;
			prPoolList[FBM_POOL_TYPE_JPEG_OSD4].u4Height = 1088;
			prPoolArrangeInfo[FBM_POOL_TYPE_JPEG_OSD4].eMode = FBM_POOL_ARRANGE_BEFORE;
			prPoolArrangeInfo[FBM_POOL_TYPE_JPEG_OSD4].ePool1 = FBM_POOL_TYPE_JPEG_OSD3;
			
			u4AheadType = FBM_POOL_TYPE_JPEG_OSD4;
#else
			u4AheadType = FBM_POOL_TYPE_JPEG_OSD2;
#endif
#else
			u4AheadType = FBM_POOL_TYPE_FEEDER_MMP;
#endif
#endif		
    u4AheadType = FBM_POOL_TYPE_PQ_TOOL;
    //----- DMX-----
    prPoolList[FBM_POOL_TYPE_DMX].u4Inherit = FBM_POOL_ROOT;
    prPoolList[FBM_POOL_TYPE_DMX].u4PoolSize = FBM_ALIGN_MASK((FBM_DMX1_SIZE), FBM_DMX_SIZE_ALIGMENT);
    prPoolArrangeInfo[FBM_POOL_TYPE_DMX].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_DMX].ePool1 = u4AheadType;   
    u4AheadType = FBM_POOL_TYPE_DMX;
	#if defined(CC_FBM_TWO_FBP)
    //----- DMX2-----
    prPoolList[FBM_POOL_TYPE_DMX2].u4Inherit = FBM_POOL_ROOT;
    prPoolList[FBM_POOL_TYPE_DMX2].u4PoolSize = FBM_ALIGN_MASK((FBM_DMX2_SIZE), FBM_DMX_SIZE_ALIGMENT);
    prPoolArrangeInfo[FBM_POOL_TYPE_DMX2].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_DMX2].ePool1 = u4AheadType;   
    u4AheadType = FBM_POOL_TYPE_DMX2;
	#endif
	#if defined(CC_FBM_TWO_4K2K)|| defined(CC_FBM_FOUR_INST_SUPPORT)
	//----- DMX3-----
    prPoolList[FBM_POOL_TYPE_DMX3].u4Inherit = FBM_POOL_ROOT;
    prPoolList[FBM_POOL_TYPE_DMX3].u4PoolSize = FBM_ALIGN_MASK((FBM_DMX3_SIZE), FBM_DMX_SIZE_ALIGMENT);
    prPoolArrangeInfo[FBM_POOL_TYPE_DMX3].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_DMX3].ePool1 = u4AheadType;   
	u4AheadType = FBM_POOL_TYPE_DMX3;
	//----- DMX4-----
    prPoolList[FBM_POOL_TYPE_DMX4].u4Inherit = FBM_POOL_ROOT;
    prPoolList[FBM_POOL_TYPE_DMX4].u4PoolSize = FBM_ALIGN_MASK((FBM_DMX4_SIZE), FBM_DMX_SIZE_ALIGMENT);
    prPoolArrangeInfo[FBM_POOL_TYPE_DMX4].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_DMX4].ePool1 = u4AheadType;   
	u4AheadType = FBM_POOL_TYPE_DMX4;
	#endif
	
#if defined(CC_FBM_SECURE_FEEDER)
	 prPoolList[FBM_POOL_TYPE_SECURE_FEEDER].u4PoolSize = FBM_SECURE_FEEDER_SIZE;
	 prPoolList[FBM_POOL_TYPE_SECURE_FEEDER].u4Inherit = FBM_POOL_ROOT; 			
	 prPoolArrangeInfo[FBM_POOL_TYPE_SECURE_FEEDER].eMode = FBM_POOL_ARRANGE_AFTER; 
	 prPoolArrangeInfo[FBM_POOL_TYPE_SECURE_FEEDER].ePool1 = u4AheadType;			
	 u4AheadType = FBM_POOL_TYPE_SECURE_FEEDER;
#endif
#ifdef CC_SUPPORT_OD    //if (DRVCUST_OptGet(eFlagSupportOD) == TRUE)
		prPoolList[FBM_POOL_TYPE_OD].u4Inherit = FBM_POOL_ROOT;
		prPoolList[FBM_POOL_TYPE_OD].u4PoolSize = FBM_OD_POOL_SIZE;
		prPoolArrangeInfo[FBM_POOL_TYPE_OD].eMode = FBM_POOL_ARRANGE_AFTER;
		prPoolArrangeInfo[FBM_POOL_TYPE_OD].ePool1 = u4AheadType;
		u4AheadType = FBM_POOL_TYPE_OD;
#endif
    //----- MPEG-----
	prPoolList[FBM_POOL_TYPE_MPEG].u4Inherit = FBM_POOL_ROOT;
	prPoolList[FBM_POOL_TYPE_MPEG].u4PoolSize = FBM_MPEG_Y_SIZE;	
	prPoolList[FBM_POOL_TYPE_MPEG].u4Mode = (FBM_POOL_MODE_MPEG_DBK | FBM_POOL_MODE_SHARE);
	prPoolArrangeInfo[FBM_POOL_TYPE_MPEG].eMode = FBM_POOL_ARRANGE_AFTER;
	prPoolArrangeInfo[FBM_POOL_TYPE_MPEG].ePool1 = u4AheadType;
	u4AheadType = FBM_POOL_TYPE_MPEG;
    
    #if defined(CC_FBM_TWO_FBP)||defined(CC_FBM_SUPPORT_4K2K)
	//----- MPEG2-----
    prPoolList[FBM_POOL_TYPE_MPEG2].u4Inherit = FBM_POOL_ROOT;
	prPoolList[FBM_POOL_TYPE_MPEG2].u4PoolSize = FBM_MPEG_Y_SIZE;
    prPoolList[FBM_POOL_TYPE_MPEG2].u4Mode = (FBM_POOL_MODE_MPEG_DBK | FBM_POOL_MODE_SHARE);
    prPoolArrangeInfo[FBM_POOL_TYPE_MPEG2].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_MPEG2].ePool1 = u4AheadType;
    u4AheadType = FBM_POOL_TYPE_MPEG2;
    #endif	
    #if defined(CC_FBM_TWO_4K2K)||defined(CC_FBM_FOUR_INST_SUPPORT)
    //----- MPEG3-----
    prPoolList[FBM_POOL_TYPE_MPEG3].u4Inherit = FBM_POOL_ROOT;
	prPoolList[FBM_POOL_TYPE_MPEG3].u4PoolSize = FBM_MPEG_Y_SIZE;
    prPoolList[FBM_POOL_TYPE_MPEG3].u4Mode = (FBM_POOL_MODE_MPEG_DBK | FBM_POOL_MODE_SHARE);
    prPoolArrangeInfo[FBM_POOL_TYPE_MPEG3].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_MPEG3].ePool1 = u4AheadType;
	u4AheadType = FBM_POOL_TYPE_MPEG3;
	
    // -----MPEG4-----
	prPoolList[FBM_POOL_TYPE_MPEG4].u4Inherit = FBM_POOL_ROOT;
    prPoolList[FBM_POOL_TYPE_MPEG4].u4PoolSize = FBM_MPEG_Y_SIZE;
    prPoolArrangeInfo[FBM_POOL_TYPE_MPEG4].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_MPEG4].ePool1 = u4AheadType;
	u4AheadType = FBM_POOL_TYPE_MPEG4;
	#endif

    //----- SCPOS MAIN-----
    prPoolList[FBM_POOL_TYPE_SCPOS_MAIN].u4Inherit = FBM_POOL_ROOT;
    prPoolList[FBM_POOL_TYPE_SCPOS_MAIN].u4PoolSize = FBM_SCPOS_MAIN_SIZE; //main 1080i 422 0xe9c0000*2+sub 0x880000
    prPoolArrangeInfo[FBM_POOL_TYPE_SCPOS_MAIN].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_SCPOS_MAIN].ePool1 = u4AheadType;
    u4AheadType = FBM_POOL_TYPE_SCPOS_MAIN;

	//----- SCPOS PIP-----
    prPoolList[FBM_POOL_TYPE_SCPOS_PIP].u4Inherit = FBM_POOL_ROOT;
    prPoolList[FBM_POOL_TYPE_SCPOS_PIP].u4PoolSize = FBM_SCPOS_SUB_SIZE; //main 1080i 422 0xe9c0000*2+sub 0x880000
    prPoolArrangeInfo[FBM_POOL_TYPE_SCPOS_PIP].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_SCPOS_PIP].ePool1 = u4AheadType;
	u4AheadType = FBM_POOL_TYPE_SCPOS_PIP;

	prPoolList[FBM_POOL_TYPE_DRM].u4Inherit = FBM_POOL_ROOT;
	prPoolList[FBM_POOL_TYPE_DRM].u4PoolSize = FBM_DRM_BUF_SIZE;
	prPoolArrangeInfo[FBM_POOL_TYPE_DRM].eMode = FBM_POOL_ARRANGE_AFTER;
	prPoolArrangeInfo[FBM_POOL_TYPE_DRM].ePool1 = u4AheadType;
	u4AheadType = FBM_POOL_TYPE_DRM;
#endif


// MPEG_C|MJC|OD|PNG|JPEG|VSS|FEEDER|DRM|PQ_3DC|SWDMX
#if defined(CC_FBM_3ND_CHANNEL) // 3 Channel
#ifdef CC_BYPASS_MJC
       u4AheadType = FBM_POOL_TYPE_TOTAL3;
#else
		prPoolList[FBM_POOL_TYPE_MJC_1].u4Inherit = FBM_POOL_ROOT;
		prPoolList[FBM_POOL_TYPE_MJC_1].u4PoolSize = FBM_MJC_RSV_POOL_SIZE;
		LOG(3, "FBMCFG CHANEL B: prPoolList[FBM_POOL_TYPE_MJC_1].u4PoolSize :%d \n", prPoolList[FBM_POOL_TYPE_MJC_1].u4PoolSize);
		prPoolList[FBM_POOL_TYPE_MJC_1].u4AddrAlign = FBM_MJC_ADDR_ALIGMENT;
		prPoolList[FBM_POOL_TYPE_MJC_1].u4Mode= FBM_POOL_MODE_MJC_Y10_C10_422_1RDC_3FRM;
		prPoolArrangeInfo[FBM_POOL_TYPE_MJC_1].eMode = FBM_POOL_ARRANGE_AFTER;
		prPoolArrangeInfo[FBM_POOL_TYPE_MJC_1].ePool1 = FBM_POOL_TYPE_TOTAL3;
		u4AheadType = FBM_POOL_TYPE_MJC_1;
#endif

        prPoolList[FBM_POOL_TYPE_MPEG_C].u4Inherit = FBM_POOL_ROOT;
        prPoolList[FBM_POOL_TYPE_MPEG_C].u4PoolSize = FBM_MPEG_C_SIZE;
        prPoolArrangeInfo[FBM_POOL_TYPE_MPEG_C].eMode = FBM_POOL_ARRANGE_AFTER;
        prPoolArrangeInfo[FBM_POOL_TYPE_MPEG_C].ePool1 = u4AheadType;
		u4AheadType = FBM_POOL_TYPE_MPEG_C;
		#if defined(CC_FBM_TWO_FBP)||defined(CC_FBM_SUPPORT_4K2K)
		prPoolList[FBM_POOL_TYPE_MPEG2_C].u4Inherit = FBM_POOL_ROOT;
        prPoolList[FBM_POOL_TYPE_MPEG2_C].u4PoolSize = FBM_MPEG_C_SIZE;
        prPoolArrangeInfo[FBM_POOL_TYPE_MPEG2_C].eMode = FBM_POOL_ARRANGE_AFTER;
        prPoolArrangeInfo[FBM_POOL_TYPE_MPEG2_C].ePool1 = u4AheadType;
		u4AheadType = FBM_POOL_TYPE_MPEG2_C;
        #endif

       #if defined(CC_FBM_TWO_4K2K)||defined(CC_FBM_FOUR_INST_SUPPORT)
		prPoolList[FBM_POOL_TYPE_MPEG3_C].u4Inherit = FBM_POOL_ROOT;
        prPoolList[FBM_POOL_TYPE_MPEG3_C].u4PoolSize = FBM_MPEG_C_SIZE;
        prPoolArrangeInfo[FBM_POOL_TYPE_MPEG3_C].eMode = FBM_POOL_ARRANGE_AFTER;
        prPoolArrangeInfo[FBM_POOL_TYPE_MPEG3_C].ePool1 = u4AheadType;
		u4AheadType = FBM_POOL_TYPE_MPEG3_C;

		prPoolList[FBM_POOL_TYPE_MPEG4_C].u4Inherit = FBM_POOL_ROOT;
        prPoolList[FBM_POOL_TYPE_MPEG4_C].u4PoolSize = FBM_MPEG_C_SIZE;
        prPoolArrangeInfo[FBM_POOL_TYPE_MPEG4_C].eMode = FBM_POOL_ARRANGE_AFTER;
        prPoolArrangeInfo[FBM_POOL_TYPE_MPEG4_C].ePool1 = u4AheadType;
		u4AheadType = FBM_POOL_TYPE_MPEG4_C;
 		#endif
		
#if defined(CC_SUPPORT_W2DR2R)
		//----- W2D R2R Y-----
		prPoolList[FBM_POOL_TYPE_W2D_R2R_Y].u4Inherit = FBM_POOL_ROOT;
		prPoolList[FBM_POOL_TYPE_W2D_R2R_Y].u4PoolSize = FBM_W2DR2R_POOL_Y_SIZE;
		prPoolArrangeInfo[FBM_POOL_TYPE_W2D_R2R_Y].eMode = FBM_POOL_ARRANGE_AFTER;
		prPoolArrangeInfo[FBM_POOL_TYPE_W2D_R2R_Y].ePool1 = u4AheadType;
		u4AheadType = FBM_POOL_TYPE_W2D_R2R_Y;
#endif        

#if defined(CC_SUPPORT_VSS)
		prPoolList[FBM_POOL_TYPE_VSS].u4Inherit = FBM_POOL_ROOT;
		prPoolList[FBM_POOL_TYPE_VSS].u4PoolSize = FBM_VSS_POOL_SIZE;
		prPoolArrangeInfo[FBM_POOL_TYPE_VSS].eMode = FBM_POOL_ARRANGE_AFTER;
		prPoolArrangeInfo[FBM_POOL_TYPE_VSS].ePool1 = u4AheadType;
		u4AheadType = FBM_POOL_TYPE_VSS;
#endif
#endif
}

static VOID _FbmConfigurePool_MT5861_MJC_SCPOS_MAIN(POOL_LIST_T* prPoolList, POOL_ARRANGE_INFO_T* prPoolArrangeInfo)
{
    FBM_POOL_TYPE_T u4AheadType = FBM_POOL_TYPE_SCPOS_MAIN, u4BehindType, u4VdoBehindType = FBM_POOL_TYPE_NS;
    UNUSED(u4AheadType);
    UNUSED(u4BehindType);
    UNUSED(u4VdoBehindType);
    // arrange root pool ---------------------------------------------------

    // DMXPID |DSP|VBI|PVR|VENC|SCPOS_SUB|
    
	u4AheadType = FBM_POOL_TYPE_TOTAL;
#if defined(CC_FBM_SUPPORT_DMXPID)
		prPoolList[FBM_POOL_TYPE_DMXPID].u4Inherit = FBM_POOL_ROOT;
		prPoolList[FBM_POOL_TYPE_DMXPID].u4PoolSize = FBM_DMX_PID_BUF_SIZE;
		prPoolArrangeInfo[FBM_POOL_TYPE_DMXPID].eMode = FBM_POOL_ARRANGE_AFTER;
		prPoolArrangeInfo[FBM_POOL_TYPE_DMXPID].ePool1 = u4AheadType;
		u4AheadType = FBM_POOL_TYPE_DMXPID;
#endif

    // vbi
    prPoolList[FBM_POOL_TYPE_VBI].u4Inherit = FBM_POOL_ROOT;
    prPoolArrangeInfo[FBM_POOL_TYPE_VBI].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_VBI].ePool1 = u4AheadType;
    u4AheadType = FBM_POOL_TYPE_VBI;

#if defined(CC_FBM_SUPPORT_PVR)
	prPoolList[FBM_POOL_TYPE_PVR].u4Inherit = FBM_POOL_ROOT;
	prPoolList[FBM_POOL_TYPE_PVR].u4PoolSize = FBM_PVR_POOL_SIZE;
	prPoolArrangeInfo[FBM_POOL_TYPE_PVR].eMode = FBM_POOL_ARRANGE_AFTER;
	prPoolArrangeInfo[FBM_POOL_TYPE_PVR].ePool1 = u4AheadType;
	u4AheadType = FBM_POOL_TYPE_PVR;
#endif

#if defined(CC_SUPPORT_VENC)
	prPoolList[FBM_POOL_TYPE_VENC].u4Inherit = FBM_POOL_ROOT;
	prPoolList[FBM_POOL_TYPE_VENC].u4PoolSize = FBM_VENC_POOL_SIZE;
	prPoolArrangeInfo[FBM_POOL_TYPE_VENC].eMode = FBM_POOL_ARRANGE_AFTER;
	prPoolArrangeInfo[FBM_POOL_TYPE_VENC].ePool1 = u4AheadType;
	u4AheadType = FBM_POOL_TYPE_VENC;
#endif
#ifdef CC_SUPPORT_RECORD_AV
	prPoolList[FBM_POOL_TYPE_TDC].u4Inherit = FBM_POOL_ROOT;
	prPoolList[FBM_POOL_TYPE_TDC].u4PoolSize = FBM_TDCAV_POOL_SIZE;
	prPoolArrangeInfo[FBM_POOL_TYPE_TDC].eMode = FBM_POOL_ARRANGE_AFTER;
	prPoolArrangeInfo[FBM_POOL_TYPE_TDC].ePool1 = u4AheadType;
	u4AheadType = FBM_POOL_TYPE_TDC;
#endif
	
	//-----PQ_3DC-----
	prPoolList[FBM_POOL_TYPE_PQ_3DC].u4Inherit = FBM_POOL_ROOT;
	prPoolList[FBM_POOL_TYPE_PQ_3DC].u4PoolSize = FBM_PQ_3DC_POOL_SIZE;
	prPoolArrangeInfo[FBM_POOL_TYPE_PQ_3DC].eMode = FBM_POOL_ARRANGE_AFTER;
	prPoolArrangeInfo[FBM_POOL_TYPE_PQ_3DC].ePool1 = u4AheadType;
	u4AheadType = FBM_POOL_TYPE_PQ_3DC;
	
#if 1
		prPoolList[FBM_POOL_TYPE_JPEG].u4Inherit = FBM_POOL_ROOT;
		prPoolList[FBM_POOL_TYPE_JPEG].u4PoolSize = FBM_JPEG_POOL_SIZE;
		prPoolArrangeInfo[FBM_POOL_TYPE_JPEG].eMode = FBM_POOL_ARRANGE_AFTER;
		prPoolArrangeInfo[FBM_POOL_TYPE_JPEG].ePool1 = u4AheadType;
		u4AheadType = FBM_POOL_TYPE_JPEG;
#endif	

#if defined(CC_FBM_HW_DEMOD)
    prPoolList[FBM_POOL_TYPE_HW_DEMOD].u4Inherit = FBM_POOL_ROOT;
    prPoolList[FBM_POOL_TYPE_HW_DEMOD].u4PoolSize = FBM_HW_DEMOD_SIZE;
    prPoolArrangeInfo[FBM_POOL_TYPE_HW_DEMOD].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_HW_DEMOD].ePool1 = u4AheadType;
    u4AheadType = FBM_POOL_TYPE_HW_DEMOD;
#endif

#if defined(CC_TVP_SUPPORT)
#ifdef ADSP_BIN_SUPPORT
		prPoolList[FBM_POOL_TYPE_DSP_BIN].u4Inherit = FBM_POOL_ROOT;
		prPoolList[FBM_POOL_TYPE_DSP_BIN].u4PoolSize = FBM_DSP_BIN_POOL_SIZE;
		prPoolArrangeInfo[FBM_POOL_TYPE_DSP_BIN].eMode = FBM_POOL_ARRANGE_AFTER;
		prPoolArrangeInfo[FBM_POOL_TYPE_DSP_BIN].ePool1 = u4AheadType;
		u4AheadType = FBM_POOL_TYPE_DSP_BIN;
#endif		 

    //-----DSP-----
    prPoolList[FBM_POOL_TYPE_DSP].u4Inherit = FBM_POOL_ROOT;
    prPoolList[FBM_POOL_TYPE_DSP].u4AddrAlign= (0x1000000-1);
    prPoolArrangeInfo[FBM_POOL_TYPE_DSP].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_DSP].ePool1 = u4AheadType;
    prPoolList[FBM_POOL_TYPE_DSP].u4PoolSize = FBM_DSP_POOL_SIZE;
    u4AheadType = FBM_POOL_TYPE_DSP;
#endif	

#if defined(CC_SUPPORT_VSS)
		prPoolList[FBM_POOL_TYPE_VSS].u4Inherit = FBM_POOL_ROOT;
		prPoolList[FBM_POOL_TYPE_VSS].u4PoolSize = FBM_VSS_POOL_SIZE;
		prPoolArrangeInfo[FBM_POOL_TYPE_VSS].eMode = FBM_POOL_ARRANGE_AFTER;
		prPoolArrangeInfo[FBM_POOL_TYPE_VSS].ePool1 = u4AheadType;
		u4AheadType = FBM_POOL_TYPE_VSS;
#endif
#if defined(CC_SUPPORT_CIPLUS14)
		//----- Remux-----
		prPoolList[FBM_POOL_TYPE_REMUX].u4Inherit = FBM_POOL_ROOT;
		prPoolList[FBM_POOL_TYPE_REMUX].u4PoolSize = FBM_REMUX_POOL_SIZE;
		prPoolArrangeInfo[FBM_POOL_TYPE_REMUX].eMode = FBM_POOL_ARRANGE_AFTER;
		prPoolArrangeInfo[FBM_POOL_TYPE_REMUX].ePool1 = u4AheadType;   
		u4AheadType = FBM_POOL_TYPE_REMUX;
#endif
    prPoolList[FBM_POOL_TYPE_MPEG_C].u4Inherit = FBM_POOL_ROOT;
    prPoolList[FBM_POOL_TYPE_MPEG_C].u4PoolSize = FBM_MPEG_C_SIZE;
    prPoolArrangeInfo[FBM_POOL_TYPE_MPEG_C].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_MPEG_C].ePool1 = u4AheadType;
	u4AheadType = FBM_POOL_TYPE_MPEG_C;
	#if defined(CC_FBM_TWO_FBP)||defined(CC_FBM_SUPPORT_4K2K)
	prPoolList[FBM_POOL_TYPE_MPEG2_C].u4Inherit = FBM_POOL_ROOT;
    prPoolList[FBM_POOL_TYPE_MPEG2_C].u4PoolSize = FBM_MPEG_C_SIZE;
    prPoolArrangeInfo[FBM_POOL_TYPE_MPEG2_C].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_MPEG2_C].ePool1 = u4AheadType;
	u4AheadType = FBM_POOL_TYPE_MPEG2_C;
    #endif

   #if defined(CC_FBM_TWO_4K2K)||defined(CC_FBM_FOUR_INST_SUPPORT)
	prPoolList[FBM_POOL_TYPE_MPEG3_C].u4Inherit = FBM_POOL_ROOT;
    prPoolList[FBM_POOL_TYPE_MPEG3_C].u4PoolSize = FBM_MPEG_C_SIZE;
    prPoolArrangeInfo[FBM_POOL_TYPE_MPEG3_C].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_MPEG3_C].ePool1 = u4AheadType;
	u4AheadType = FBM_POOL_TYPE_MPEG3_C;

	prPoolList[FBM_POOL_TYPE_MPEG4_C].u4Inherit = FBM_POOL_ROOT;
    prPoolList[FBM_POOL_TYPE_MPEG4_C].u4PoolSize = FBM_MPEG_C_SIZE;
    prPoolArrangeInfo[FBM_POOL_TYPE_MPEG4_C].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_MPEG4_C].ePool1 = u4AheadType;
	u4AheadType = FBM_POOL_TYPE_MPEG4_C;
	#endif
	//----- SCPOS PIP-----
    prPoolList[FBM_POOL_TYPE_SCPOS_PIP].u4Inherit = FBM_POOL_ROOT;
    prPoolList[FBM_POOL_TYPE_SCPOS_PIP].u4PoolSize = FBM_SCPOS_SUB_SIZE; //main 1080i 422 0xe9c0000*2+sub 0x880000
    prPoolArrangeInfo[FBM_POOL_TYPE_SCPOS_PIP].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_SCPOS_PIP].ePool1 = u4AheadType;
    u4AheadType = FBM_POOL_TYPE_SCPOS_PIP;

#if !defined(CC_TVP_SUPPORT)
#ifdef ADSP_BIN_SUPPORT
		prPoolList[FBM_POOL_TYPE_DSP_BIN].u4Inherit = FBM_POOL_ROOT;
		prPoolList[FBM_POOL_TYPE_DSP_BIN].u4PoolSize = FBM_DSP_BIN_POOL_SIZE;
		prPoolArrangeInfo[FBM_POOL_TYPE_DSP_BIN].eMode = FBM_POOL_ARRANGE_AFTER;
		prPoolArrangeInfo[FBM_POOL_TYPE_DSP_BIN].ePool1 = u4AheadType;
		u4AheadType = FBM_POOL_TYPE_DSP_BIN;
#endif		 

    //-----DSP-----
    prPoolList[FBM_POOL_TYPE_DSP].u4Inherit = FBM_POOL_ROOT;
    prPoolArrangeInfo[FBM_POOL_TYPE_DSP].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_DSP].ePool1 = u4AheadType;
    prPoolList[FBM_POOL_TYPE_DSP].u4PoolSize = FBM_DSP_POOL_SIZE;
    u4AheadType = FBM_POOL_TYPE_DSP;
#endif	

//channel B
//DMX|MPEG|SCPOS_MAIN
#if defined(CC_FBM_2ND_CHANNEL) // 2 Channel

u4AheadType = FBM_POOL_TYPE_TOTAL2;

#if defined(CC_TVP_SUPPORT)
#ifndef CC_BYPASS_MJC
    prPoolList[FBM_POOL_TYPE_MJC_1].u4Inherit = FBM_POOL_ROOT;
    prPoolList[FBM_POOL_TYPE_MJC_1].u4PoolSize = FBM_MJC_RSV_POOL_SIZE;
    LOG(3, "FBMCFG CHANEL B: prPoolList[FBM_POOL_TYPE_MJC_1].u4PoolSize :%d \n", prPoolList[FBM_POOL_TYPE_MJC_1].u4PoolSize);
    prPoolList[FBM_POOL_TYPE_MJC_1].u4AddrAlign = FBM_MJC_ADDR_ALIGMENT;
    prPoolList[FBM_POOL_TYPE_MJC_1].u4Mode= FBM_POOL_MODE_MJC_Y10_C10_422_1RDC_3FRM;
    prPoolArrangeInfo[FBM_POOL_TYPE_MJC_1].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_MJC_1].ePool1 = u4AheadType;
    u4AheadType = FBM_POOL_TYPE_MJC_1;
#endif
#endif

	
#ifdef CC_SUPPORT_OD    //if (DRVCUST_OptGet(eFlagSupportOD) == TRUE)
	prPoolList[FBM_POOL_TYPE_OD].u4Inherit = FBM_POOL_ROOT;
	prPoolList[FBM_POOL_TYPE_OD].u4PoolSize = FBM_OD_POOL_SIZE;
	prPoolArrangeInfo[FBM_POOL_TYPE_OD].eMode = FBM_POOL_ARRANGE_AFTER;
	prPoolArrangeInfo[FBM_POOL_TYPE_OD].ePool1 = u4AheadType;
	u4AheadType = FBM_POOL_TYPE_OD;
#endif
    //----- MPEG-----
	prPoolList[FBM_POOL_TYPE_MPEG].u4Inherit = FBM_POOL_ROOT;
	prPoolList[FBM_POOL_TYPE_MPEG].u4PoolSize = FBM_MPEG_Y_SIZE;	
	prPoolList[FBM_POOL_TYPE_MPEG].u4Mode = (FBM_POOL_MODE_MPEG_DBK | FBM_POOL_MODE_SHARE);
	prPoolArrangeInfo[FBM_POOL_TYPE_MPEG].eMode = FBM_POOL_ARRANGE_AFTER;
	prPoolArrangeInfo[FBM_POOL_TYPE_MPEG].ePool1 = u4AheadType;
	u4AheadType = FBM_POOL_TYPE_MPEG;
    
    #if defined(CC_FBM_TWO_FBP)||defined(CC_FBM_SUPPORT_4K2K)
	//----- MPEG2-----
    prPoolList[FBM_POOL_TYPE_MPEG2].u4Inherit = FBM_POOL_ROOT;
	prPoolList[FBM_POOL_TYPE_MPEG2].u4PoolSize = FBM_MPEG_Y_SIZE;
    prPoolList[FBM_POOL_TYPE_MPEG2].u4Mode = (FBM_POOL_MODE_MPEG_DBK | FBM_POOL_MODE_SHARE);
    prPoolArrangeInfo[FBM_POOL_TYPE_MPEG2].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_MPEG2].ePool1 = u4AheadType;
    u4AheadType = FBM_POOL_TYPE_MPEG2;
    #endif	
    #if defined(CC_FBM_TWO_4K2K)||defined(CC_FBM_FOUR_INST_SUPPORT)
    //----- MPEG3-----
    prPoolList[FBM_POOL_TYPE_MPEG3].u4Inherit = FBM_POOL_ROOT;
	prPoolList[FBM_POOL_TYPE_MPEG3].u4PoolSize = FBM_MPEG_Y_SIZE;
    prPoolList[FBM_POOL_TYPE_MPEG3].u4Mode = (FBM_POOL_MODE_MPEG_DBK | FBM_POOL_MODE_SHARE);
    prPoolArrangeInfo[FBM_POOL_TYPE_MPEG3].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_MPEG3].ePool1 = u4AheadType;
	u4AheadType = FBM_POOL_TYPE_MPEG3;
	
    // -----MPEG4-----
	prPoolList[FBM_POOL_TYPE_MPEG4].u4Inherit = FBM_POOL_ROOT;
    prPoolList[FBM_POOL_TYPE_MPEG4].u4PoolSize = FBM_MPEG_Y_SIZE;
    prPoolArrangeInfo[FBM_POOL_TYPE_MPEG4].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_MPEG4].ePool1 = u4AheadType;
	u4AheadType = FBM_POOL_TYPE_MPEG4;
	#endif

    //----- SCPOS MAIN-----
    prPoolList[FBM_POOL_TYPE_SCPOS_MAIN].u4Inherit = FBM_POOL_ROOT;
    prPoolList[FBM_POOL_TYPE_SCPOS_MAIN].u4PoolSize = FBM_SCPOS_MAIN_SIZE; //main 1080i 422 0xe9c0000*2+sub 0x880000
    prPoolArrangeInfo[FBM_POOL_TYPE_SCPOS_MAIN].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_SCPOS_MAIN].ePool1 = u4AheadType;
    u4AheadType = FBM_POOL_TYPE_SCPOS_MAIN;

	prPoolList[FBM_POOL_TYPE_DRM].u4Inherit = FBM_POOL_ROOT;
	prPoolList[FBM_POOL_TYPE_DRM].u4PoolSize = FBM_DRM_BUF_SIZE;
	prPoolArrangeInfo[FBM_POOL_TYPE_DRM].eMode = FBM_POOL_ARRANGE_AFTER;
	prPoolArrangeInfo[FBM_POOL_TYPE_DRM].ePool1 = u4AheadType;
	u4AheadType = FBM_POOL_TYPE_DRM;


			 //----- DMX-----
		 prPoolList[FBM_POOL_TYPE_DMX].u4Inherit = FBM_POOL_ROOT;
		 prPoolList[FBM_POOL_TYPE_DMX].u4PoolSize = FBM_ALIGN_MASK((FBM_DMX1_SIZE), FBM_DMX_SIZE_ALIGMENT);
		 prPoolArrangeInfo[FBM_POOL_TYPE_DMX].eMode = FBM_POOL_ARRANGE_AFTER;
		 prPoolArrangeInfo[FBM_POOL_TYPE_DMX].ePool1 = u4AheadType;   
		 u4AheadType = FBM_POOL_TYPE_DMX;
#if defined(CC_FBM_TWO_FBP)
		 //----- DMX2-----
		 prPoolList[FBM_POOL_TYPE_DMX2].u4Inherit = FBM_POOL_ROOT;
		 prPoolList[FBM_POOL_TYPE_DMX2].u4PoolSize = FBM_ALIGN_MASK((FBM_DMX2_SIZE), FBM_DMX_SIZE_ALIGMENT);
		 prPoolArrangeInfo[FBM_POOL_TYPE_DMX2].eMode = FBM_POOL_ARRANGE_AFTER;
		 prPoolArrangeInfo[FBM_POOL_TYPE_DMX2].ePool1 = u4AheadType;   
		 u4AheadType = FBM_POOL_TYPE_DMX2;
#endif
#if defined(CC_FBM_TWO_4K2K)|| defined(CC_FBM_FOUR_INST_SUPPORT)
		 //----- DMX3-----
		 prPoolList[FBM_POOL_TYPE_DMX3].u4Inherit = FBM_POOL_ROOT;
		 prPoolList[FBM_POOL_TYPE_DMX3].u4PoolSize = FBM_ALIGN_MASK((FBM_DMX3_SIZE), FBM_DMX_SIZE_ALIGMENT);
		 prPoolArrangeInfo[FBM_POOL_TYPE_DMX3].eMode = FBM_POOL_ARRANGE_AFTER;
		 prPoolArrangeInfo[FBM_POOL_TYPE_DMX3].ePool1 = u4AheadType;   
		 u4AheadType = FBM_POOL_TYPE_DMX3;
		 //----- DMX4-----
		 prPoolList[FBM_POOL_TYPE_DMX4].u4Inherit = FBM_POOL_ROOT;
		 prPoolList[FBM_POOL_TYPE_DMX4].u4PoolSize = FBM_ALIGN_MASK((FBM_DMX4_SIZE), FBM_DMX_SIZE_ALIGMENT);
		 prPoolArrangeInfo[FBM_POOL_TYPE_DMX4].eMode = FBM_POOL_ARRANGE_AFTER;
		 prPoolArrangeInfo[FBM_POOL_TYPE_DMX4].ePool1 = u4AheadType;   
		 u4AheadType = FBM_POOL_TYPE_DMX4;
#endif


		 prPoolList[FBM_POOL_TYPE_FEEDER].u4PoolSize = FBM_FEEDER1_SIZE;
		 prPoolList[FBM_POOL_TYPE_FEEDER].u4Inherit = FBM_POOL_ROOT;			
		 prPoolArrangeInfo[FBM_POOL_TYPE_FEEDER].eMode = FBM_POOL_ARRANGE_AFTER; 
		 prPoolArrangeInfo[FBM_POOL_TYPE_FEEDER].ePool1 = u4AheadType;	
	
		 prPoolList[FBM_POOL_TYPE_FEEDER_MMP].u4PoolSize = FBM_FEEDER_POOL_SIZE;
		 prPoolArrangeInfo[FBM_POOL_TYPE_FEEDER_MMP].eMode = FBM_POOL_ARRANGE_AFTER;
		 prPoolArrangeInfo[FBM_POOL_TYPE_FEEDER_MMP].ePool1 = u4AheadType;
		 
		 u4AheadType = FBM_POOL_TYPE_FEEDER;
     #if defined(CC_FBM_TWO_FBP)
		 prPoolList[FBM_POOL_TYPE_FEEDER2].u4PoolSize = FBM_FEEDER2_SIZE;
		 prPoolList[FBM_POOL_TYPE_FEEDER2].u4Inherit = FBM_POOL_ROOT;			
		 prPoolArrangeInfo[FBM_POOL_TYPE_FEEDER2].eMode = FBM_POOL_ARRANGE_AFTER; 
		 prPoolArrangeInfo[FBM_POOL_TYPE_FEEDER2].ePool1 = u4AheadType; 		
		 u4AheadType = FBM_POOL_TYPE_FEEDER2;
	 #endif
		 
     #if defined(CC_FBM_TWO_4K2K)||defined(CC_FBM_FOUR_INST_SUPPORT)
		 prPoolList[FBM_POOL_TYPE_FEEDER3].u4PoolSize = FBM_FEEDER3_SIZE;
		 prPoolList[FBM_POOL_TYPE_FEEDER3].u4Inherit = FBM_POOL_ROOT;			
		 prPoolArrangeInfo[FBM_POOL_TYPE_FEEDER3].eMode = FBM_POOL_ARRANGE_AFTER; 
		 prPoolArrangeInfo[FBM_POOL_TYPE_FEEDER3].ePool1 = u4AheadType; 		
		 u4AheadType = FBM_POOL_TYPE_FEEDER3;
	
		 prPoolList[FBM_POOL_TYPE_FEEDER4].u4PoolSize = FBM_FEEDER4_SIZE;
		 prPoolList[FBM_POOL_TYPE_FEEDER4].u4Inherit = FBM_POOL_ROOT;			
		 prPoolArrangeInfo[FBM_POOL_TYPE_FEEDER4].eMode = FBM_POOL_ARRANGE_AFTER; 
		 prPoolArrangeInfo[FBM_POOL_TYPE_FEEDER4].ePool1 = u4AheadType; 		
		 u4AheadType = FBM_POOL_TYPE_FEEDER4;
	 #endif
		 
#if defined(CC_FBM_SUPPORT_SWDMX)||defined(CC_FBM_SUPPORT_LITE_SWDMX)
		 prPoolList[FBM_POOL_TYPE_SWDMX].u4Inherit = FBM_POOL_ROOT;
		 prPoolList[FBM_POOL_TYPE_SWDMX].u4PoolSize = FBM_SWDMX_POOL_SIZE;
		 prPoolArrangeInfo[FBM_POOL_TYPE_SWDMX].eMode = FBM_POOL_ARRANGE_AFTER;
		 prPoolArrangeInfo[FBM_POOL_TYPE_SWDMX].ePool1 = u4AheadType;
		 u4AheadType = FBM_POOL_TYPE_SWDMX;
#endif
#if defined(CC_FBM_SUPPORT_PNG)		
		 prPoolList[FBM_POOL_TYPE_PNG].u4Inherit = FBM_POOL_ROOT;
		 prPoolList[FBM_POOL_TYPE_PNG].u4PoolSize = FBM_PNG_POOL_SIZE;
		 prPoolArrangeInfo[FBM_POOL_TYPE_PNG].eMode = FBM_POOL_ARRANGE_AFTER;
		 prPoolArrangeInfo[FBM_POOL_TYPE_PNG].ePool1 = u4AheadType;
		 u4AheadType = FBM_POOL_TYPE_PNG;
#endif	
    #if defined(CC_FBM_SUPPORT_SRS)
			//----- SRS-----
			prPoolList[FBM_POOL_TYPE_SRS].u4Inherit = FBM_POOL_ROOT;
			prPoolList[FBM_POOL_TYPE_SRS].u4PoolSize = FBM_SRS_SIZE;
			prPoolArrangeInfo[FBM_POOL_TYPE_SRS].eMode = FBM_POOL_ARRANGE_AFTER;
			prPoolArrangeInfo[FBM_POOL_TYPE_SRS].ePool1 = u4AheadType;
			u4AheadType = FBM_POOL_TYPE_SRS;
   #endif
   #if defined(CC_FBM_3D_OSD) || defined(CC_MHP_SUPPORT) //MHP use jpeg_vdp buffer
			prPoolList[FBM_POOL_TYPE_JPG_VDP].u4Inherit = FBM_POOL_ROOT;
			prPoolList[FBM_POOL_TYPE_JPG_VDP].u4PoolSize = FBM_JPEVDP_POOL_SIZE;
			prPoolArrangeInfo[FBM_POOL_TYPE_JPG_VDP].eMode = FBM_POOL_ARRANGE_AFTER;
			prPoolArrangeInfo[FBM_POOL_TYPE_JPG_VDP].ePool1 = u4AheadType;
			u4AheadType = FBM_POOL_TYPE_JPG_VDP;
#endif
    
		// pq tool
		prPoolList[FBM_POOL_TYPE_PQ_TOOL].u4Inherit = FBM_POOL_ROOT;
        prPoolList[FBM_POOL_TYPE_PQ_TOOL].u4PoolSize = FBM_PQ_TOOL_POOL_SIZE;
		prPoolArrangeInfo[FBM_POOL_TYPE_PQ_TOOL].eMode = FBM_POOL_ARRANGE_AFTER;
		prPoolArrangeInfo[FBM_POOL_TYPE_PQ_TOOL].ePool1 = u4AheadType;
		u4AheadType = FBM_POOL_TYPE_PQ_TOOL;

		
#if !defined(CC_TVP_SUPPORT)
#ifndef CC_BYPASS_MJC
			prPoolList[FBM_POOL_TYPE_MJC_1].u4Inherit = FBM_POOL_ROOT;
			prPoolList[FBM_POOL_TYPE_MJC_1].u4PoolSize = FBM_MJC_RSV_POOL_SIZE;
			LOG(3, "FBMCFG CHANEL B: prPoolList[FBM_POOL_TYPE_MJC_1].u4PoolSize :%d \n", prPoolList[FBM_POOL_TYPE_MJC_1].u4PoolSize);
			prPoolList[FBM_POOL_TYPE_MJC_1].u4AddrAlign = FBM_MJC_ADDR_ALIGMENT;
			prPoolList[FBM_POOL_TYPE_MJC_1].u4Mode= FBM_POOL_MODE_MJC_Y10_C10_422_1RDC_3FRM;
			prPoolArrangeInfo[FBM_POOL_TYPE_MJC_1].eMode = FBM_POOL_ARRANGE_AFTER;
			prPoolArrangeInfo[FBM_POOL_TYPE_MJC_1].ePool1 = u4AheadType;
			u4AheadType = FBM_POOL_TYPE_MJC_1;
#endif
#endif

#endif
}

#endif

#define UPDATE_PRPOOL(type, module) \
    do {\
        u4NextDramAddr = FBM_ALIGN_MASK(u4NextDramAddr, FBM_ ## module ## _ADDR_ALIGMENT);\
        prPool = FBM_GetPoolInfo(type);\
        prPool->u4Addr = u4NextDramAddr;\
        prPool->u4Size = u4Size;\
        prPool->u4Width = u4Width;\
        prPool->u4Height = u4Height;\
        prPool->u4Mode = u4Mode;\
        prPool->u4Inherit = FBM_POOL_INHERIT;\
        u4NextDramAddr = prPool->u4Addr + prPool->u4Size;\
    } while (0)

#define UPDATE_POP_WIDTH() \
    do {\
        if (env->u1fgPOP)\
        {\
            u4Width = MIN(u4Width,u4HPDSize/2);\
        }\
    } while (0)

#define UPDATE_DI_FIFO_WIDTH(u4VdpId) \
    do {\
        if (fgFBM_MDDI_FIFO_LIMIT(u4VdpId))\
        {\
            if (env->u1fgPOP)\
            {\
                u4DIFIFO = FBM_DI_POP_FIFO_WIDTH;\
            }\
            else if ( (u4ScalerMode & FBM_POOL_MODE_SCPOS_HYBRID_DISP) ||\
             (u4ScalerMode & FBM_POOL_MODE_SCPOS_DUAL_HYBRID_DISP) )\
            {\
                u4DIFIFO = FBM_DI_HYBRID_WIDTH;\
            }\
            else\
            {\
                u4DIFIFO = FBM_DI_PIP_FIFO_WIDTH;\
            }\
        }\
        else\
        {\
            u4DIFIFO = 65535;\
        }\
    } while (0)
    
#define UPDATE_POP_HEIGHT() \
    do {\
        if (env->u1fgPOP)\
        {\
            u4Height /= 2;\
        }\
    } while (0)

//Check Panel Display Size        
#define UPDATE_HPD_WIDTH()\
    do {\
        if (u4VdpId == VDP_1)\
        {\
            if(u4HPDSize != 0)\
            {\
                u4Width = MIN(u4HPDSize,u4Width);\
            }\
        }\
    } while (0)
        
#define CALCULATE_PSCAN_NR_NUM()\
	do {\
    if (u4MddiMode & FBM_POOL_MODE_MDDI_DISP)\
    {\
    	u1NRNum = 2;\
        if(u1IsMpeg)\
   	    u1NRNum+=1;\
    }\
    else if ((u4MddiMode & FBM_POOL_MODE_MDDI_HYBRID) || (u4MddiMode & FBM_POOL_MODE_MDDI_DUAL_HYBRID))\
    {\
        u1NRNum = FBM_POOL_HYBRID_DRAM_FRAME_NUM;\
        if (u4FlipModule & FLIP_BY_SCALER)\
            u1NRNum += 1;\
    }\
    else if (u4MddiMode & FBM_POOL_MODE_MDDI_NR_Y_C)\
    u1NRNum = 1;\
    else\
    u1NRNum = 0;\
    if(u4FlipModule & FLIP_BY_PSCAN)\
    u1NRNum += 1;\
    if(E_TDTV_DI_DRAM_DOUBLE==u4PscanModeEx)\
    u1NRNum *= 2;\
    if(E_TDTV_DI_DRAM_0FB==u4PscanModeEx)\
    u1NRNum = 0;\
    if(fgFBM_PscanTest)\
    u1NRNum = 10;\
    }while(0)

#define CALCULATE_PROG_DISP_444_SIZE()\
    do {\
        CALCULATE_PSCAN_NR_NUM();\
        if (u4MddiMode & FBM_POOL_MODE_10BIT)\
        {\
            u4Size = FBM_MDDI_BI_NR_10BIT_444_SIZE(u4Width, u4Height, u1NRNum);\
        }\
        else\
        {\
            u4Size = FBM_MDDI_BI_NR_8BIT_444_SIZE(u4Width, u4Height, u1NRNum);\
        }\
    } while (0)

#define CALCULATE_PSCAN_NR_YC_SIZE_3D_422() \
    do {\
        CALCULATE_PSCAN_NR_NUM();\
        if (u4MddiMode & FBM_POOL_MODE_10BIT)\
        {\
            u4Size = FBM_MDDI_BI_NR_10BIT_SIZE(u4Width, u4Height, u1NRNum);\
        }\
        else\
        {\
            u4Size = FBM_MDDI_BI_NR_8BIT_SIZE(u4Width, u4Height, u1NRNum);\
        }\
    } while (0)

#define CALCULATE_PSCAN_NR_YC_SIZE_3D_420() \
    do {\
        CALCULATE_PSCAN_NR_NUM();\
        if (u4MddiMode & FBM_POOL_MODE_10BIT)\
        {\
            if(u4MddiMode & FBM_POOL_MODE_MDDI_NR_OFF)\
                u4Size = FBM_MDDI_BI_NR_OFF_10BIT_420_SIZE(u4Width, u4Height, u1NRNum);\
            else \
                u4Size = FBM_MDDI_BI_NR_10BIT_420_SIZE(u4Width, u4Height, u1NRNum);\
        }\
        else\
        {\
            if(u4MddiMode & FBM_POOL_MODE_MDDI_NR_OFF)\
                u4Size = FBM_MDDI_BI_NR_OFF_8BIT_420_SIZE(u4Width, u4Height, u1NRNum);\
            else \
                u4Size = FBM_MDDI_BI_NR_8BIT_420_SIZE(u4Width, u4Height, u1NRNum);\
        }\
    } while (0)

#define CALCULATE_PSCAN_NR_YC_SIZE_3D()\
    do {\
        if (env->u1IsMPEG[u4VdpId]&&(!(u4MddiMode & FBM_POOL_MODE_MDDI_HYBRID))&&(!(u4MddiMode & FBM_POOL_MODE_MDDI_DUAL_HYBRID)))\
        {\
            CALCULATE_PSCAN_NR_YC_SIZE_3D_420();\
        }\
        else\
        {\
            CALCULATE_PSCAN_NR_YC_SIZE_3D_422();\
        }\
        if(0==u1NRNum)\
            u4Size = 0;\
    }while (0)
        
#define CALCULATE_SCALER_444_PARAMETERS() \
    do {\
        u1ScalerNum = 3;\
        if (u4FlipModule & FLIP_BY_SCALER)\
        {\
            u1ScalerNum += 1;\
        }\
        if (u4FlipModule & FLIP_BY_3D)\
        {\
            u1ScalerNum += 3;\
        }\
        u4Size = FBM_SCPOS_444_3FB_POOL_SIZE_FLIP(u4Width, u4Height, u1ScalerNum);\
        u4Mode = FBM_POOL_MODE_8BIT | FBM_POOL_MODE_444_FB | FBM_POOL_MODE_SCPOS_3FB;\
    } while (0)
#define CALCULATE_SCALER_422_PARAMETERS() \
    do {\
        u1ScalerNum = 3;\
        if (u4FlipModule & FLIP_BY_SCALER)\
        {\
            u1ScalerNum += 1;\
        }\
        if (u4FlipModule & FLIP_BY_3D)\
        {\
            u1ScalerNum += 3;\
        }\
        if (u4ScalerMode & FBM_POOL_MODE_10BIT)\
        {\
            u4Size = FBM_SCPOS_10BIT_SIZE_FLIP(u4Width, u4Height, u1ScalerNum);\
        }\
        else\
        {\
            u4Size = FBM_SCPOS_8BIT_SIZE_FLIP(u4Width, u4Height, u1ScalerNum);\
        }\
        u4Mode = FBM_POOL_MODE_422_FB | FBM_POOL_MODE_SCPOS_3FB;\
        if (u4ScalerMode & FBM_POOL_MODE_10BIT)\
        {\
            u4Mode |= FBM_POOL_MODE_10BIT;\
        }\
        else\
        {\
            u4Mode |= FBM_POOL_MODE_8BIT;\
        }\
    } while (0)

//4K2K EPG Mode dram always config 3840X2160
#if defined(CC_MT5399)||defined(CC_MT5890)    
#define UPDATE_EPG_WxH(u4VdpId, width, height)\
    do {\
        if (u4VdpId == VDP_1)\
        {\
            if (SRM_IsEpgMode() && !fgIs4k2kNativeTimingOut(VDP_1))\
            {\
                u4EPGWidth = MAX(u4EPGWidth,width);\
                u4EPGHeight = MAX(u4EPGHeight,height);\
                u4Width = MIN(u4EPGWidth,u4Width);\
                u4Height = MIN(u4EPGHeight,u4Height);\
            }\
        }\
    } while (0)
#else
  #define UPDATE_EPG_WxH(u4VdpId, width, height)\
    do {\
        if (u4VdpId == VDP_1)\
        {\
            if (0)\
            {\
                u4Width = MIN(width,u4Width);\
                u4Height = MIN(height,u4Height);\
            }\
        }\
    } while (0)
#endif
    
#define UPDATE_WxH_BY_RESOLUTION(u4VdpId) \
    do {\
	   if((env->u4Resolution[u4VdpId] == 0)||(SRM_IsScanMode(u4VdpId)))\
        {\
            u4Width = 720;\
            u4Height = 576;\
        }\
        else\
       {\
            u4Width = env->u4InputWidth[u4VdpId] ;\
            u4Height = env->u4InputHeight[u4VdpId] ;\
        }\
        u4Width = MIN(u4Width, u4DIFIFO);\
        UPDATE_POP_WIDTH();\
        UPDATE_HPD_WIDTH();\
    } while (0)

#define CHECK_PSCAN_SIZE_TO_0()\
    do {\
        UINT32 ModeFlags = 0;\
        ModeFlags = (ModeFlags | FBM_POOL_MODE_MDDI_DISP | FBM_POOL_MODE_MDDI_HYBRID | FBM_POOL_MODE_MDDI_DUAL_HYBRID);\
        if (((u4MddiMode & ModeFlags) == 0) && (u4MddiMode & FBM_POOL_MODE_MDDI_BOB) && (u4MddiMode & FBM_POOL_MODE_MDDI_NR_OFF))\
        {\
            u4Size = 0;\
        }\
    } while (0)
#define CHECK_SCALER_SIZE_TO_0()\
    do {\
        if ((u4ScalerMode & FBM_POOL_MODE_SCPOS_PSCAN_DISP) || (u4ScalerMode & FBM_POOL_MODE_SCPOS_HYBRID_DISP)|| (u4ScalerMode & FBM_POOL_MODE_SCPOS_DUAL_HYBRID_DISP)||(u4ScalerMode == FBM_POOL_MODE_UNSTABLE))\
        {\
            u4Size = 0;\
        }\
    } while (0)

static UINT32 FBM_FlipModule[VDP_NS];

static inline void v3DWxHAlign(const UINT32 u4DIMode, UINT32* pu4Width, UINT32* pu4Height)
{
    UINT32 u4Width = *pu4Width;
    UINT32 u4Height = *pu4Height;
    E_TD_OUT eDIOUT = TD_MIB_OUT(u4DIMode);
    
#if defined(CC_MT5882)||defined(CC_MT5399)||defined(CC_MT5890)
    UINT32 u4DisplayH = PANEL_GetPanelHeight();    
    E_TD_IN eDIIn = TD_MIB_IN(u4DIMode);
    
    if(E_TD_OUT_3D_LI == eDIOUT)
    {
        if((E_TD_IN_SBS_P == eDIIn) || (E_TD_IN_SBS_I == eDIIn) || 
            (E_TD_IN_FS_P == eDIIn) || (E_TD_IN_FS_I == eDIIn))
        {//According to Predown rule, it will be predown to half of panel height in these conditions.
            u4Height = MIN(u4DisplayH/2 , u4Height); 
        }
        if((E_TD_IN_TB_I == eDIIn) || (E_TD_IN_TB_P == eDIIn) ||
            (E_TD_IN_LI_P== eDIIn))
        {
            u4Height = MIN(u4DisplayH , u4Height);
        }
    }
    
    if(E_TD_OUT_NATIVE == eDIOUT &&(E_TD_IN_LI_P== eDIIn))
    {
        u4Height = MIN(u4DisplayH , u4Height);
    }

#endif
   
    if(E_TD_OUT_NATIVE != eDIOUT)
    {
        u4Width = (u4Width+511)/512*512;
    }

    *pu4Width = u4Width;
    *pu4Height= u4Height;
}

static inline void vCalcInterlaceFldNum(UINT32 u4VdpId, 
    UINT32 u4MddiMode, UINT32 u4flip, UINT8* u1Ynum, UINT8* u1Cnum,
    UINT32 u4PscanModeEx,UINT8 u1IsMpeg)
{
    if (u4MddiMode & FBM_POOL_MODE_MDDI_FULL)
    {
        *u1Ynum = 4;
        *u1Cnum = 4;
    }
    else if (u4MddiMode & FBM_POOL_MODE_MDDI_CBOB)
    {
        *u1Ynum = 4;
        *u1Cnum = 3;
    }
    else /* if (u4MddiMode & FBM_POOL_MODE_MDDI_BOB) */
    {//one more buffer for Pscan display mode BOB to solve tear issue
        *u1Ynum = 2;
        *u1Cnum = 2;
    }
    
    if ((u4MddiMode & FBM_POOL_MODE_MDDI_DISP))
    {
        *u1Ynum += 1;
        *u1Cnum += 1;
        if ((u4MddiMode & FBM_POOL_MODE_MDDI_BOB))
        {// when output sync bob mode and pr mode,A field have not line buffer,so it need to use the C field. Y/C num +1
            if(E_TD_OUT_3D_LI == TD_MIB_OUT(u4DrvTDTVDIModeQuery()))       
            {
                *u1Ynum += 2;
                *u1Cnum += 2;
            }
        }        
        if (u4flip & FLIP_BY_PSCAN)
        {
            *u1Ynum += 1;
            *u1Cnum += 1;
        }        
		if(u1IsMpeg)//for epg mode
    	{
            *u1Ynum += 1;
            *u1Cnum += 1;
    	}
    }

    if(E_TDTV_DI_DRAM_DOUBLE==u4PscanModeEx)
    {
    	*u1Ynum *=2;
    	*u1Cnum *=2;
    }
    else if(E_TDTV_DI_DRAM_0FB==u4PscanModeEx)
    {
    	*u1Ynum=0;
    	*u1Cnum=0;
    }
    
    if(fgFBM_PscanTest)
    {
        *u1Ynum = 10;
        *u1Cnum = 10;
    }
}

inline UINT32 u4AllocMIBSizeProgressive(const UINT32 u4VdpId, 
    const UINT32 u4MddiMode,const UINT32 u4FlipModule,
    const UINT32 u4Width,const UINT32 u4Height,const UINT32 u4PscanModeEx, const UINT8 u1IsMpeg)
{
    UINT32 u4Size;
    UINT8 u1NRNum;
    UINT32 u4DIMode = u4DrvTDTVDIModeQuery();

    CALCULATE_PSCAN_NR_NUM();

    //No 420 Support for Line interleave Src
    if((u4MddiMode & FBM_POOL_MODE_MDDI_NR_OFF)||(E_TDTV_DI_DRAM_DOUBLE==u4PscanModeEx))
    {
        if(u1IsMpeg&&!(E_TD_IN_LI_P==TD_MIB_IN(u4DIMode)))
        {
            if (u4MddiMode & FBM_POOL_MODE_10BIT)
            {
                u4Size = FBM_MDDI_NR_10BIT_420_SIZE(u4Width, u4Height, u1NRNum);
            }
            else
            {
                u4Size = FBM_MDDI_NR_8BIT_420_SIZE(u4Width, u4Height, u1NRNum);
            }
        }
        else
        {
            if (u4MddiMode & FBM_POOL_MODE_10BIT)
            {
                u4Size = FBM_MDDI_NR_10BIT_SIZE(u4Width, u4Height, u1NRNum);
            }
            else
            {
                u4Size = FBM_MDDI_NR_8BIT_SIZE(u4Width, u4Height, u1NRNum);
            }
        }
    }
    else
    {
        if(u1IsMpeg&&!(E_TD_IN_LI_P==TD_MIB_IN(u4DIMode)))
        {
            if (u4MddiMode & FBM_POOL_MODE_10BIT)
            {
                u4Size = FBM_MDDI_BI_NR_10BIT_420_SIZE(u4Width, u4Height, u1NRNum);
            }
            else
            {
                u4Size = FBM_MDDI_BI_NR_8BIT_420_SIZE(u4Width, u4Height, u1NRNum);
            }
        }
        else
        {
            if (u4MddiMode & FBM_POOL_MODE_10BIT)
            {
                u4Size = FBM_MDDI_BI_NR_10BIT_SIZE(u4Width, u4Height, u1NRNum);
            }
            else
            {
                u4Size = FBM_MDDI_BI_NR_8BIT_SIZE(u4Width, u4Height, u1NRNum);
            }
        }
    }

    if(0==u1NRNum)
        u4Size = 0;

    return u4Size;
}

static inline UINT32 u4AllocMIBSizeInterlace(const UINT32 u4VdpId, 
    const UINT32 u4MddiMode,const UINT32 u4FlipModule,
    const UINT32 u4Width,const UINT32 u4Height,const UINT32 u4PscanModeEx, const UINT8 fgIsMpeg)
{
    UINT8 u1Ynum, u1Cnum;
    UINT32 u4Size = 0;
    
    vCalcInterlaceFldNum(u4VdpId, u4MddiMode, u4FlipModule, &u1Ynum, &u1Cnum,u4PscanModeEx,fgIsMpeg);
   
    if(VDP_1==u4VdpId)
    {
#if defined(CC_MT5399)||defined(CC_MT5890)||defined(CC_MT5882)  
// Only applied in MT5399 because it's One of worst condition of MT5399,
// keep untouched for saving QA effort, 
// This Definition check should be removed in next generation for unify system flow

        if(E_TDTV_DI_DRAM_DOUBLE==u4PscanModeEx)
        {
            if (u4MddiMode & FBM_POOL_MODE_10BIT)
            {
                if (fgIsMpeg)
                {
                    u4Size = FBM_ALIGN_MASK(FBM_MDDI_10BIT_420_SIZE(u4Width, u4Height, u1Ynum, u1Cnum), FBM_MDDI_SIZE_ALIGMENT);
                }
                else
                {
                    u4Size = FBM_ALIGN_MASK(FBM_MDDI_10BIT_422_SIZE(u4Width, u4Height, u1Ynum, u1Cnum), FBM_MDDI_SIZE_ALIGMENT);
                }
            }
            else // if (u4MddiMode & FBM_POOL_MODE_8BIT)
            {
                if (fgIsMpeg)
                {
                    u4Size = FBM_ALIGN_MASK(FBM_MDDI_8BIT_420_SIZE(u4Width, u4Height, u1Ynum, u1Cnum), FBM_MDDI_SIZE_ALIGMENT);
                }
                else
                {
                    u4Size = FBM_ALIGN_MASK(FBM_MDDI_8BIT_422_SIZE(u4Width, u4Height, u1Ynum, u1Cnum), FBM_MDDI_SIZE_ALIGMENT);
                }
            }
        }
        else
#endif        
        {
            if (u4MddiMode & FBM_POOL_MODE_10BIT)
            {
                if (fgIsMpeg)
                {
                    u4Size = FBM_ALIGN_MASK(FBM_MDDI_BI_10BIT_420_SIZE(u4Width, u4Height, u1Ynum, u1Cnum), FBM_MDDI_SIZE_ALIGMENT);
                }
                else
                {
                    u4Size = FBM_ALIGN_MASK(FBM_MDDI_BI_10BIT_422_SIZE(u4Width, u4Height, u1Ynum, u1Cnum), FBM_MDDI_SIZE_ALIGMENT);
                }
            }
            else // if (u4MddiMode & FBM_POOL_MODE_8BIT)
            {
                if (fgIsMpeg)
                {
                    u4Size = FBM_ALIGN_MASK(FBM_MDDI_BI_8BIT_420_SIZE(u4Width, u4Height, u1Ynum, u1Cnum), FBM_MDDI_SIZE_ALIGMENT);
                }
                else
                {
                    u4Size = FBM_ALIGN_MASK(FBM_MDDI_BI_8BIT_422_SIZE(u4Width, u4Height, u1Ynum, u1Cnum), FBM_MDDI_SIZE_ALIGMENT);
                }
            }
        }
    }
    else
    {
        u4Size = FBM_ALIGN_MASK(FBM_MDDI_SUB_BI_8BIT_422_SIZE(u4Width, u4Height, u1Ynum, u1Cnum), FBM_MDDI_SIZE_ALIGMENT);
    }
    
    CHECK_PSCAN_SIZE_TO_0();

    return u4Size;
}

UINT32 u4QueryFlipModule(UINT32 u4VdpId)
{
    return FBM_FlipModule[u4VdpId];
}

static inline UINT32 u4DecideFlipModule(UINT32 u4VdpId, UINT32 u4ScalerMode, UINT8 u1IsMPEG, FBM_AUTO_INC_ENV_T* env)
{
    UINT32 u4FlipModule = FLIP_NONE;
    UINT8 ucTVEEnable;

    TVE_GetEnable(TVE_1, &ucTVEEnable);
    #if (defined(CC_MT5399)||defined(CC_MT5890)||defined(CC_MT5882) ) && !defined(CC_BYPASS_MJC)  
        if (TRUE == fgIsMJCToOSTG())
        {
            u4FlipModule = FLIP_BY_MJC; 
        }
        else
    #endif
        {
            if (IS_IC_5861() && (u4ScalerMode & FBM_POOL_MODE_SCPOS_HYBRID_DISP) )
            {
                u4FlipModule = FLIP_BY_SCALER;
            }
            else if (IS_IC_5861() && (u4ScalerMode & FBM_POOL_MODE_SCPOS_DUAL_HYBRID_DISP) )
            {
                u4FlipModule = FLIP_BY_PSCAN;
            }
            else if((u1IsMPEG) && (u4VdpId == VDP_1))
            {
                u4FlipModule = FLIP_BY_B2R;
            }
            else if (u4VdpId == VDP_2)
            {
                u4FlipModule = FLIP_BY_SCALER;
            }
            else
            {
                if (u4ScalerMode & FBM_POOL_MODE_SCPOS_FULL)
                {
                    u4FlipModule = FLIP_BY_SCALER;
                }
                else if ( (u4ScalerMode & FBM_POOL_MODE_SCPOS_PSCAN_DISP)||(u4ScalerMode & FBM_POOL_MODE_SCPOS_LINESYNC_DISP))
                {
                    u4FlipModule = FLIP_BY_PSCAN;
                }
            }
            if((ucTVEEnable)&&(!env->u1IsMPEG[VDP_1])&&(!env->u1IsMPEG[VDP_2])&&(u4VdpId==VDP_2))
            {
                u4FlipModule = FLIP_NONE;
            }
            
        }

#if !defined(CC_FLIP_MIRROR_SUPPORT)
    u4FlipModule = FLIP_NONE;
#endif

    FBM_FlipModule[u4VdpId] = u4FlipModule;

    return u4FlipModule;
}

UINT32 u4SizeOfCurrent[2];

BOOL fgFBM_MDDI_FIFO_LIMIT(UINT32 u4VdpId)
{
    UINT32 u4MddiMode = SRM_GetMddiMode(u4VdpId);
    UINT32 u4ScposMode = SRM_GetScposMode(u4VdpId);
    FBM_AUTO_INC_ENV_T env;

    _FbmGetEnv(&env);
    
    if ( !(u4ScposMode & FBM_POOL_MODE_SCPOS_FULL)     ||
          (env.u1Interlace[u4VdpId])                                     ||
          !(u4MddiMode & FBM_POOL_MODE_MDDI_NR_OFF)    )
    {
        return TRUE;
    }

    return FALSE;
}


void ExpandFBM4SinglexPOP(UINT32 u4VdpId, FBM_AUTO_INC_ENV_T* env, UINT32 u4BaseDramAddr)
{
    UINT32 u4NextDramAddr, u4MddiMode, u4ScalerMode, u4Width, u4Height, u4Size, u4Mode, u4HPDSize;
    UINT8 u1ScalerNum, u1NRNum;
    UINT32 u4FlipModule;
	UINT32 u4PscanModeEx;
    FBM_POOL_T* prPool;
    UINT32 u4EPGWidth;
    UINT32 u4EPGHeight;
    UINT8 u1IsMpeg =env->u1IsMPEG[u4VdpId];
    
    UINT32 u4DIMode = u4DrvTDTVDIModeQuery();
    static UINT32 u4PreMainScalerWidth = 0, u4PreMainScalerHeight = 0;
    UINT32 u4DIFIFO = 65535;

    //should same as SRM_IsEpgMode  srm_if.c
    u4EPGWidth = PANEL_GetPanelWidth()/3 + 20;
    u4EPGHeight = PANEL_GetPanelHeight()*2/5 + 20;
    
    u4PscanModeEx = u4DrvTDTVForceDIDramModeQueryEx(u4VdpId);
    u4MddiMode = SRM_GetMddiMode(u4VdpId);
    u4ScalerMode = SRM_GetScposMode(u4VdpId);
    u4HPDSize = SRM_GetScposHPDSize(u4VdpId); //Miss Nameing, it's Panel Display Size
    u4FlipModule = u4DecideFlipModule(u4VdpId, u4ScalerMode, u1IsMpeg, env);

    u4NextDramAddr = u4BaseDramAddr;
    UPDATE_DI_FIFO_WIDTH(u4VdpId);
      
    // reset value
    #if defined(CC_MT5890)
    if (u4VdpId == VDP_1 && bDrvVideoIsDecDualMode())
    {
        u4PscanModeEx = E_TDTV_DI_DRAM_DOUBLE;
    }
    #endif
    
    if ((E_TD_IN_SBS_P == TD_MIB_IN(u4DIMode) || E_TD_IN_SBS_I == TD_MIB_IN(u4DIMode)) && 
          E_TD_OUT_NATIVE != TD_MIB_OUT(u4DIMode) && 
          VDP_1 == u4VdpId)
    {
        u4HPDSize <<= 1;
        u4DIFIFO   <<= 1;
    }
    
    UPDATE_WxH_BY_RESOLUTION(u4VdpId);
    
    u4Size = 0;
    u4Mode = 0;
    
    if (u4VdpId == VDP_1)
    {
        UPDATE_PRPOOL(FBM_POOL_TYPE_MDDI_DYN_MAIN, MDDI);
        UPDATE_PRPOOL(FBM_POOL_TYPE_SCPOS_DYN_MAIN, SCPOS);
    }
    else
    {
        UPDATE_PRPOOL(FBM_POOL_TYPE_MDDI_DYN_SUB, MDDI);
        UPDATE_PRPOOL(FBM_POOL_TYPE_SCPOS_DYN_SUB, SCPOS);
    }

#ifdef CC_SUPPORT_PIPELINE
    if (env->u1IsTDC[u4VdpId]|| bTvd3dSignalStatus())
    {
        u4Width = 780;
        u4Height = 578;
        u4Mode = FBM_POOL_MODE_10BIT;
        u4Size = FBM_TDC_POOL_SIZE;
        UPDATE_PRPOOL(FBM_POOL_TYPE_TDC_DYN, TDC);
    }
#else
    if (env->u1IsTDC[u4VdpId])
    {
        UPDATE_PRPOOL(FBM_POOL_TYPE_TDC_DYN, TDC);
    }
#endif

    if ((env->u1mode[u4VdpId] == FBM_MODE_1080I) || (env->u1mode[u4VdpId] == FBM_MODE_576I))
    {   
        // scaler   
        if((u4VdpId == VDP_2)&&(env->u1mode[u4VdpId] == FBM_MODE_1080I))
        {
        #if defined(CC_FBM_SUB_SUPPORT_FHD)
		if(env->u1mode[u4VdpId] == FBM_MODE_1080I)
			{
		    u4Width = 1920;
            u4Height = 1080;
			}
		else
			{
			u4Width = 960;
            u4Height = 540;
			}
		#else
            u4Width = 960;
            u4Height = 1080;
		#endif
        }

        if (u4VdpId == VDP_1)
        {//For ATV PVR, it's to keep Scaler dram address.
            if (fgFBMFixedScalerDram)
            {
                u4Width  = u4PreMainScalerWidth;
                u4Height = u4PreMainScalerHeight;
            }
            else
            {
                u4PreMainScalerWidth  = u4Width;
                u4PreMainScalerHeight = u4Height;   
            }
        }
        
        UPDATE_EPG_WxH(u4VdpId, 720, 576);
            
        CALCULATE_SCALER_422_PARAMETERS();
        CHECK_SCALER_SIZE_TO_0();

        if (u4VdpId == VDP_1)
        {
            UPDATE_PRPOOL(FBM_POOL_TYPE_SCPOS_DYN_MAIN, SCPOS);
        }
        else
        {
            UPDATE_PRPOOL(FBM_POOL_TYPE_SCPOS_DYN_SUB, SCPOS);
        }

        // pscan        
        UPDATE_WxH_BY_RESOLUTION(u4VdpId);
                      
        UPDATE_EPG_WxH(u4VdpId, 720, 1082);
        
        v3DWxHAlign(u4DIMode,&u4Width,&u4Height);

        u4Size = u4AllocMIBSizeInterlace(u4VdpId,u4MddiMode,u4FlipModule
            ,u4Width,u4Height,u4PscanModeEx,u1IsMpeg);
                
        if (u4VdpId == VDP_1)
        {
            UPDATE_PRPOOL(FBM_POOL_TYPE_MDDI_DYN_MAIN, MDDI);
        }
        else
        {
            UPDATE_PRPOOL(FBM_POOL_TYPE_MDDI_DYN_SUB, MDDI);
        }
        
#ifndef CC_SUPPORT_RECORD_AV
#ifndef CC_SUPPORT_PIPELINE
        // tdc
        if(env->u1IsTDC[u4VdpId])
        {
            u4Width = 780;
            u4Height = 578;
            u4Mode = FBM_POOL_MODE_10BIT;
            u4Size = FBM_TDC_POOL_SIZE;
            UPDATE_PRPOOL(FBM_POOL_TYPE_TDC_DYN, TDC);
        }
#endif		
#endif
    }
    else if (env->u1mode[u4VdpId] == FBM_MODE_1080P_422)
    {      
        UPDATE_EPG_WxH(u4VdpId, 720, 576);
        v3DWxHAlign(u4DIMode,&u4Width,&u4Height);
        
        if (u4MddiMode & FBM_POOL_MODE_MDDI_DISP)
        {
#if defined(CC_MT5399)||defined(CC_MT5890) ||defined(CC_MT5882) 
// Only applied in MT5399 because it's One of worst condition of MT5399,
// keep untouched for saving QA effort, 
// This Definition check should be removed in next generation for unify system flow
            u4Size = u4AllocMIBSizeProgressive(u4VdpId,u4MddiMode,u4FlipModule
                ,u4Width,u4Height,u4PscanModeEx,u1IsMpeg);
#else            
            //No 420 Support for Line interleave Src
            if (E_TD_IN_LI_P==TD_MIB_IN(u4DIMode))
            {
                CALCULATE_PSCAN_NR_YC_SIZE_3D_422();
            }
            else
            {
                CALCULATE_PSCAN_NR_YC_SIZE_3D();
            } 
#endif

            if (u4VdpId == VDP_1)
            {
                UPDATE_PRPOOL(FBM_POOL_TYPE_MDDI_DYN_MAIN, MDDI);
            }
            else
            {
                UPDATE_PRPOOL(FBM_POOL_TYPE_MDDI_DYN_SUB, MDDI);
            }
        }
        else
        {
	        if (u4VdpId == VDP_1)
	        {//For ATV PVR, it's to keep Scaler dram address.
	            if (fgFBMFixedScalerDram)
	            {
	                u4Width  = u4PreMainScalerWidth;
	                u4Height = u4PreMainScalerHeight;
	            }
	            else
	            {
	                u4PreMainScalerWidth  = u4Width;
	                u4PreMainScalerHeight = u4Height;   
	            }
	        }
			
            CALCULATE_SCALER_422_PARAMETERS();
            CHECK_SCALER_SIZE_TO_0();
            
            if (u4VdpId == VDP_1)
            {
                UPDATE_PRPOOL(FBM_POOL_TYPE_SCPOS_DYN_MAIN, SCPOS);
            }
            else
            {
                UPDATE_PRPOOL(FBM_POOL_TYPE_SCPOS_DYN_SUB, SCPOS);
            }

            CALCULATE_PSCAN_NR_YC_SIZE_3D();
            CHECK_PSCAN_SIZE_TO_0();

            if (u4VdpId == VDP_1)
            {
                UPDATE_PRPOOL(FBM_POOL_TYPE_MDDI_DYN_MAIN, MDDI);
            }
            else
            {
                UPDATE_PRPOOL(FBM_POOL_TYPE_MDDI_DYN_SUB, MDDI);
            }
        }
    }
    else if (env->u1mode[u4VdpId] == FBM_MODE_1080P_444)
    {         
        UPDATE_EPG_WxH(u4VdpId, 720, 576);
        
        if (u4MddiMode & FBM_POOL_MODE_MDDI_DISP)
        {            
            v3DWxHAlign(u4DIMode,&u4Width,&u4Height);            
            
            CALCULATE_PROG_DISP_444_SIZE();
           
            if (u4VdpId == VDP_1)
            {
                UPDATE_PRPOOL(FBM_POOL_TYPE_MDDI_DYN_MAIN, MDDI);
            }
            else
            {
                UPDATE_PRPOOL(FBM_POOL_TYPE_MDDI_DYN_SUB, MDDI);
            }
        }
        else
        {
            CALCULATE_SCALER_444_PARAMETERS();
            CHECK_SCALER_SIZE_TO_0();
            
            if (u4VdpId == VDP_1)
            {
                UPDATE_PRPOOL(FBM_POOL_TYPE_SCPOS_DYN_MAIN, SCPOS);
            }
            else
            {
                UPDATE_PRPOOL(FBM_POOL_TYPE_SCPOS_DYN_SUB, SCPOS);
            }
        }
    }

    u4SizeOfCurrent[u4VdpId] = u4NextDramAddr-u4BaseDramAddr;
    LOG(3, "vdp:%d, occupied size: 0x%x\n", u4VdpId, u4SizeOfCurrent[u4VdpId]);
}

void ExpandFBM4SubPIP(UINT32 u4VdpId, FBM_AUTO_INC_ENV_T* env, UINT32 u4BaseDramAddr)
{
    UINT32 u4NextDramAddr, u4MddiMode, u4ScalerMode,u4Width, u4Height, u4Size, u4Mode;
    UINT32 u4FlipModule;
    FBM_POOL_T* prPool;
    UINT8 u1ScalerNum;

    u4NextDramAddr = u4BaseDramAddr;
    u4MddiMode = SRM_GetMddiMode(u4VdpId);
    u4ScalerMode = SRM_GetScposMode(u4VdpId);

    u4FlipModule = u4DecideFlipModule(u4VdpId, u4ScalerMode, env->u1IsMPEG[u4VdpId], env);

    // reset value
    u4Size = 0;
    u4Width = 720;
    u4Height = 576;
    u4Mode = 0;
    
#if defined(CC_FBM_SUB_SUPPORT_FHD)
    u4Width = env->u4InputWidth[u4VdpId] ;
    u4Height = env->u4InputHeight[u4VdpId] ;
#endif
    UPDATE_PRPOOL(FBM_POOL_TYPE_MDDI_DYN_SUB, MDDI);
    UPDATE_PRPOOL(FBM_POOL_TYPE_SCPOS_DYN_SUB, SCPOS);
    if (env->u1IsTDC[u4VdpId])
    {
        UPDATE_PRPOOL(FBM_POOL_TYPE_TDC_DYN, TDC);
    }

    if ((env->u1mode[u4VdpId] == FBM_MODE_1080I) || (env->u1mode[u4VdpId] == FBM_MODE_576I))
    {       
        // pscan
#if !defined(CC_FBM_SUB_SUPPORT_FHD)
        if (env->u1mode[u4VdpId] == FBM_MODE_1080I)
        {
            #ifdef CC_MT5882
            if(IS_IC_5882_ES1())
            {
                u4Width = 720;
            }
            else
            {
                u4Width = 960;
            }
            #else
            u4Width = 720;
            #endif
            u4Height = 1082;
        }
        else
        {
            u4Width = 780;
            u4Height = 576; 
        }
#endif

        u4Size = u4AllocMIBSizeInterlace(u4VdpId,u4MddiMode,u4FlipModule
            ,u4Width,u4Height,E_TDTV_DI_DRAM_NORMAL,env->u1IsMPEG[u4VdpId]);
        
        UPDATE_PRPOOL(FBM_POOL_TYPE_MDDI_DYN_SUB, MDDI);

        // scaler
        #if !defined(CC_FBM_SUB_SUPPORT_FHD)
        u4Width = 780;
        u4Height = 578;
		#endif
        CALCULATE_SCALER_422_PARAMETERS();
        UPDATE_PRPOOL(FBM_POOL_TYPE_SCPOS_DYN_SUB, SCPOS);

    	//tdc
        if(env->u1IsTDC[u4VdpId])
        {
            u4Width = 780;
            u4Height = 578;
            u4Mode = FBM_POOL_MODE_10BIT;
            u4Size = FBM_TDC_POOL_SIZE;
            UPDATE_PRPOOL(FBM_POOL_TYPE_TDC_DYN, TDC);
        }      
    }
    else if ((env->u1mode[u4VdpId] == FBM_MODE_1080P_422) || (env->u1mode[u4VdpId] == FBM_MODE_1080P_444))
    {
        // scaler
        u4NextDramAddr = u4BaseDramAddr;
		#if !defined(CC_FBM_SUB_SUPPORT_FHD)
        u4Width = 780;
        u4Height = 576;
		#endif
        CALCULATE_SCALER_444_PARAMETERS();
        UPDATE_PRPOOL(FBM_POOL_TYPE_SCPOS_DYN_SUB, SCPOS);
    }

    u4SizeOfCurrent[u4VdpId] = u4NextDramAddr-u4BaseDramAddr;
    LOG(3, "vdp:%d, occupied size: 0x%x\n", u4VdpId, u4SizeOfCurrent[u4VdpId]);
}


#if defined(CC_FBM_SPLIT_MPEG_FREE_PART)
UINT32 _FbmGetDyamicMPEG_NptvMaxSize(void)
{
    return 0x34a000; // 720P
}

UINT32 _FbmGetDyamicMPEG_MPEGMaxSize(void)
{
    return 0x750000; // RMVB 720P
}
#endif

#if defined(CC_MT5882)
static FBM_POOL_T* _FbmGetPoolInfoAuto_MT5882_1GB(UCHAR ucPoolType, const FBM_POOL_MODULE_INFO_T* prInfo)
{
    FBM_POOL_T* prPool=NULL;
    static UINT8 fgInited = FALSE;
    //FBM_POOL_T* prPrevPool;
    FBM_AUTO_INC_ENV_T env;
    UINT8 IsNoSub = FALSE;
    UINT8 u1CheckDSP = 1;
    UNUSED(u1CheckDSP);

    if (!fgInited) // calculate max size of main, sub path in pip/pop
    {
        fgInited = TRUE;

        prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_SCPOS_MAIN);
        _fbm_u4NptvBeAddr = prPool->u4Addr;
        //prPrevPool = FBM_GetPoolInfo(FBM_POOL_TYPE_DSP);
        //_fbm_u4NptvSubPopAddr = (_fbm_u4NptvBeAddr + (prPrevPool->u4Addr))/2;
        _fbm_u4NptvSubPopAddr = _fbm_u4NptvBeAddr + (prPool->u4Size)/2;
        _fbm_u4NptvSubPopAddr = _fbm_u4NptvSubPopAddr +0x200000;
        /* set _fbm_u4NptvSubPipAddr in the following case */

        if (SRMFBM_GetConf() == FBM_MEM_CFG_MT5882_3DTV)
        {
        #if defined(CC_FBM_SUPPORT_HDMI_4K2K30)
            _fbm_u4NptvSubPipAddr = _fbm_u4NptvBeAddr + 0x1e80000 + 0x2E00000;
        #else
            _fbm_u4NptvSubPipAddr = _fbm_u4NptvBeAddr + 0x1a00000;
		#endif
        }
        else
        {
#if defined(CC_FLIP_MIRROR_SUPPORT)
            _fbm_u4NptvSubPipAddr = _fbm_u4NptvBeAddr + 0x1310000;
#else
            // MT5365_128Mx2
            _fbm_u4NptvSubPipAddr = _fbm_u4NptvBeAddr + 0x1180000;
            //_fbm_u4NptvSubPipAddr = _fbm_u4NptvBeAddr + 0x1c00000;
#endif
        }

        //#if defined(CC_IC_VERIFY_FBM_MODE)
#if 0
        _fbm_u4NptvSubPipAddr = _fbm_u4NptvBeAddr + 0x2000000;
#endif

        // reset value
        prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_MDDI_DYN_MAIN);
        prPool->u4Addr = FBM_ALIGN_MASK(_fbm_u4NptvBeAddr, FBM_MDDI_ADDR_ALIGMENT);
        prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_MDDI_DYN_SUB);
        prPool->u4Addr = FBM_ALIGN_MASK(_fbm_u4NptvBeAddr, FBM_MDDI_ADDR_ALIGMENT);
        prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_SCPOS_DYN_MAIN);
        prPool->u4Addr = FBM_ALIGN_MASK(_fbm_u4NptvBeAddr, FBM_SCPOS_ADDR_ALIGMENT);
        prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_SCPOS_DYN_SUB);
        prPool->u4Addr = FBM_ALIGN_MASK(_fbm_u4NptvBeAddr, FBM_SCPOS_ADDR_ALIGMENT);
        prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_TDC_DYN);
        prPool->u4Addr = FBM_ALIGN_MASK(_fbm_u4NptvBeAddr, FBM_SCPOS_ADDR_ALIGMENT);        
#ifndef CC_SUPPORT_RECORD_AV
        prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_TDC);
        prPool->u4Addr = FBM_ALIGN_MASK(_fbm_u4NptvBeAddr, FBM_SCPOS_ADDR_ALIGMENT);
#endif
        
    }

    _FbmGetEnv(&env);

#if defined(CC_FBM_NO_SUBPATH)
    IsNoSub = TRUE;
#endif

    if (IsNoSub)
    {
        if (env.u1IsMPEG[VDP_1])
        {
#if defined(CC_DYNAMIC_MPEG_SIZE)
            prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_MPEG);
#else
            prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_FEEDER);
#endif
            _fbm_u4NptvBeAddr = prPool->u4Addr + prPool->u4Size;
#if defined(CC_DYNAMIC_MPEG_SIZE) // unmark
            _fbm_u4NptvBeAddr = prPool->u4Addr + SRM_GetMMSizeMode(VDP_1);
#endif
        }
        else
        {
            prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_SCPOS_MAIN);
            _fbm_u4NptvBeAddr = prPool->u4Addr;
        }

        if (env.u1fgPIP || env.u1fgPOP)
        {
            Printf("This model does NOT support PIP/POP\n");
            //ASSERT(0);
        }
    }

    ExpandFBM4SinglexPOP(VDP_1, &env, _fbm_u4NptvBeAddr);
#if !defined(CC_FBM_NO_SUBPATH)
    if (env.u1fgPIP || env.u1fgPOP)
    {
        if (env.u1fgPOP)
        {
            // u4NextDramAddr = _fbm_u4NptvSubPopAddr; // todo
            ExpandFBM4SinglexPOP(VDP_2, &env, _fbm_u4NptvSubPopAddr);
        }
        else
        {
            // u4NextDramAddr = _fbm_u4NptvSubPipAddr; // todo
            ExpandFBM4SubPIP(VDP_2, &env, _fbm_u4NptvSubPipAddr);
        }
    }
#endif
    if ((ucPoolType == (UCHAR) FBM_POOL_TYPE_TDC) ||
            (ucPoolType == (UCHAR) FBM_POOL_TYPE_MDDI_MAIN) ||
            (ucPoolType == (UCHAR) FBM_POOL_TYPE_SCPOS_MAIN) ||
            (ucPoolType == (UCHAR) FBM_POOL_TYPE_MDDI_PIP) ||
            (ucPoolType == (UCHAR) FBM_POOL_TYPE_SCPOS_PIP))
    {
        if (ucPoolType == (UCHAR) FBM_POOL_TYPE_TDC)
        {
            _eCurrentTdc = FBM_POOL_TYPE_TDC_DYN;
            prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_TDC_DYN);
        }
        if (ucPoolType == (UCHAR) FBM_POOL_TYPE_MDDI_MAIN)
        {
            _eCurrentMddiMain = FBM_POOL_TYPE_MDDI_DYN_MAIN;
            prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_MDDI_DYN_MAIN);
        }
        if (ucPoolType == (UCHAR) FBM_POOL_TYPE_SCPOS_MAIN)
        {
            _eCurrentScposMain = FBM_POOL_TYPE_SCPOS_DYN_MAIN;
            prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_SCPOS_DYN_MAIN);
        }
        if (ucPoolType == (UCHAR) FBM_POOL_TYPE_MDDI_PIP)
        {
            _eCurrentMddiPip = FBM_POOL_TYPE_MDDI_DYN_SUB;
            prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_MDDI_DYN_SUB);
        }
        if (ucPoolType == (UCHAR) FBM_POOL_TYPE_SCPOS_PIP)
        {
            _eCurrentScposPip = FBM_POOL_TYPE_SCPOS_DYN_SUB;
            prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_SCPOS_DYN_SUB);
        }
        // sub pip error checking
        // sub pop error checking
#if !defined(CC_FBM_NO_SUBPATH)
        if ((ucPoolType == (UCHAR) FBM_POOL_TYPE_MDDI_MAIN) ||
                (ucPoolType == (UCHAR) FBM_POOL_TYPE_SCPOS_MAIN))
        {
            if (env.u1fgPIP)
            {
                if ((prPool->u4Addr + prPool->u4Size) > _fbm_u4NptvSubPipAddr)
                {
                    Printf("FBM main overwrite subpip\n");
                }
            }
            else if (env.u1fgPOP)
            {
                if ((prPool->u4Addr + prPool->u4Size) > _fbm_u4NptvSubPopAddr)
                {
                    Printf("FBM main overwrite subpop\n");
                }
            }
        }
#endif

        /*
            if ((SRMFBM_GetConf() == FBM_MEM_CFG_MT5395_DDR2x2)
             || (SRMFBM_GetConf() == FBM_MEM_CFG_MT5396_DDR3x4)
                || (SRMFBM_GetConf() == FBM_MEM_CFG_MT5395_3DTVCHB_NOMJC))
            {
                // only SCPOS_MAIN in channel 2, so it need not to check
                u1CheckDSP = 0;
            }
            */
        // dsp error checking
        #if !defined(CC_TVP_SUPPORT)
        if (u1CheckDSP)
        {
            if ((prPool->u4Addr + prPool->u4Size) > (FBM_GetPoolInfo(FBM_POOL_TYPE_DSP)->u4Addr))
            {
                Printf("FBM nptv overlap with DSP: 0x%x\n", ucPoolType);
                Printf("occupied range: 0x%x--0x%x\n", prPool->u4Addr, (prPool->u4Addr + prPool->u4Size));
                Printf("valid range: 0x%x--0x%x\n", _fbm_u4NptvBeAddr, (FBM_GetPoolInfo(FBM_POOL_TYPE_DSP)->u4Addr));
                #ifndef CC_TVP_SUPPORT
                VERIFY(0);
                #endif
            }
        }
		#endif
        return prPool;
    }

#if defined(CC_FBM_SPLIT_MPEG_FREE_PART)
    prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_MPEG_FREE);
    prPool->u4Addr = FBM_GetPoolInfo(FBM_POOL_TYPE_MPEG)->u4Addr;
    prPool->u4Addr += _FbmGetDyamicMPEG_NptvMaxSize();
    prPool->u4Addr += _FbmGetDyamicMPEG_MPEGMaxSize();
    prPool->u4Size = FBM_GetPoolInfo(FBM_POOL_TYPE_DSP)->u4Addr - prPool->u4Addr;
#endif

		if(ucPoolType >= FBM_POOL_TYPE_NS)
    {
    	Printf("input type error (%d)\n",ucPoolType);
    	return (FBM_GetPoolInfo(FBM_POOL_TYPE_TOTAL));
    }
    prPool = FBM_GetPoolInfo(ucPoolType);

    return prPool;
}

#endif

#if  defined(CC_MT5399)||defined(CC_MT5890)
static FBM_POOL_T* _FbmGetPoolInfoAuto_MT5890_1GB(UCHAR ucPoolType, const FBM_POOL_MODULE_INFO_T* prInfo)
{
    FBM_POOL_T* prPool=NULL;
    static UINT8 fgInited = FALSE;
    //FBM_POOL_T* prPrevPool;
    FBM_AUTO_INC_ENV_T env;
    UINT8 IsNoSub = FALSE;
    UINT8 u1CheckDSP = 1;
    UNUSED(u1CheckDSP);

    if (!fgInited) // calculate max size of main, sub path in pip/pop
    {
        fgInited = TRUE;

        prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_SCPOS_MAIN);
        _fbm_u4NptvBeAddr = prPool->u4Addr;
        
        prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_SCPOS_PIP);
        _fbm_u4NptvSubAddr = prPool->u4Addr;
        
        // reset value
        prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_MDDI_DYN_MAIN);
        prPool->u4Addr = FBM_ALIGN_MASK(_fbm_u4NptvBeAddr, FBM_MDDI_ADDR_ALIGMENT);
        prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_MDDI_DYN_SUB);
        prPool->u4Addr = FBM_ALIGN_MASK(_fbm_u4NptvBeAddr, FBM_MDDI_ADDR_ALIGMENT);
        prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_SCPOS_DYN_MAIN);
        prPool->u4Addr = FBM_ALIGN_MASK(_fbm_u4NptvBeAddr, FBM_SCPOS_ADDR_ALIGMENT);
        prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_SCPOS_DYN_SUB);
        prPool->u4Addr = FBM_ALIGN_MASK(_fbm_u4NptvBeAddr, FBM_SCPOS_ADDR_ALIGMENT);
        prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_TDC_DYN);
        prPool->u4Addr = FBM_ALIGN_MASK(_fbm_u4NptvBeAddr, FBM_SCPOS_ADDR_ALIGMENT);        
#ifndef CC_SUPPORT_RECORD_AV
        prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_TDC);
        prPool->u4Addr = FBM_ALIGN_MASK(_fbm_u4NptvBeAddr, FBM_SCPOS_ADDR_ALIGMENT);
#endif
        
    }

    _FbmGetEnv(&env);

#if defined(CC_FBM_NO_SUBPATH)
    IsNoSub = TRUE;
#endif

    if (IsNoSub)
    {
        if (env.u1IsMPEG[VDP_1])
        {
#if defined(CC_DYNAMIC_MPEG_SIZE)
            prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_MPEG);
#else
            prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_FEEDER);
#endif
            _fbm_u4NptvBeAddr = prPool->u4Addr + prPool->u4Size;
#if defined(CC_DYNAMIC_MPEG_SIZE) // unmark
            _fbm_u4NptvBeAddr = prPool->u4Addr + SRM_GetMMSizeMode(VDP_1);
#endif
        }
        else
        {
            prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_SCPOS_MAIN);
            _fbm_u4NptvBeAddr = prPool->u4Addr;
        }

        if (env.u1fgPIP || env.u1fgPOP)
        {
            Printf("This model does NOT support PIP/POP\n");
            //ASSERT(0);
        }
    }

    ExpandFBM4SinglexPOP(VDP_1, &env, _fbm_u4NptvBeAddr);
#if !defined(CC_FBM_NO_SUBPATH)
    if (env.u1fgPIP || env.u1fgPOP)
    {
        if (env.u1fgPOP)
        {
            // u4NextDramAddr = _fbm_u4NptvSubPopAddr; // todo
            ExpandFBM4SinglexPOP(VDP_2, &env, _fbm_u4NptvSubAddr);
        }
        else
        {
            // u4NextDramAddr = _fbm_u4NptvSubPipAddr; // todo
            ExpandFBM4SubPIP(VDP_2, &env, _fbm_u4NptvSubAddr);
        }
    }
#endif
    if ((ucPoolType == (UCHAR) FBM_POOL_TYPE_TDC) ||
            (ucPoolType == (UCHAR) FBM_POOL_TYPE_MDDI_MAIN) ||
            (ucPoolType == (UCHAR) FBM_POOL_TYPE_SCPOS_MAIN) ||
            (ucPoolType == (UCHAR) FBM_POOL_TYPE_MDDI_PIP) ||
            (ucPoolType == (UCHAR) FBM_POOL_TYPE_SCPOS_PIP))
    {
        if (ucPoolType == (UCHAR) FBM_POOL_TYPE_TDC)
        {
            _eCurrentTdc = FBM_POOL_TYPE_TDC_DYN;
            prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_TDC_DYN);
        }
        if (ucPoolType == (UCHAR) FBM_POOL_TYPE_MDDI_MAIN)
        {
            _eCurrentMddiMain = FBM_POOL_TYPE_MDDI_DYN_MAIN;
            prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_MDDI_DYN_MAIN);
        }
        if (ucPoolType == (UCHAR) FBM_POOL_TYPE_SCPOS_MAIN)
        {
            _eCurrentScposMain = FBM_POOL_TYPE_SCPOS_DYN_MAIN;
            prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_SCPOS_DYN_MAIN);
        }
        if (ucPoolType == (UCHAR) FBM_POOL_TYPE_MDDI_PIP)
        {
            _eCurrentMddiPip = FBM_POOL_TYPE_MDDI_DYN_SUB;
            prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_MDDI_DYN_SUB);
        }
        if (ucPoolType == (UCHAR) FBM_POOL_TYPE_SCPOS_PIP)
        {
            _eCurrentScposPip = FBM_POOL_TYPE_SCPOS_DYN_SUB;
            prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_SCPOS_DYN_SUB);
        }

        /*
            if ((SRMFBM_GetConf() == FBM_MEM_CFG_MT5395_DDR2x2)
             || (SRMFBM_GetConf() == FBM_MEM_CFG_MT5396_DDR3x4)
                || (SRMFBM_GetConf() == FBM_MEM_CFG_MT5395_3DTVCHB_NOMJC))
            {
                // only SCPOS_MAIN in channel 2, so it need not to check
                u1CheckDSP = 0;
            }
            */
        // dsp error checking
        
        return prPool;
    }

#if defined(CC_FBM_SPLIT_MPEG_FREE_PART)
    prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_MPEG_FREE);
    prPool->u4Addr = FBM_GetPoolInfo(FBM_POOL_TYPE_MPEG)->u4Addr;
    prPool->u4Addr += _FbmGetDyamicMPEG_NptvMaxSize();
    prPool->u4Addr += _FbmGetDyamicMPEG_MPEGMaxSize();
    prPool->u4Size = FBM_GetPoolInfo(FBM_POOL_TYPE_DSP)->u4Addr - prPool->u4Addr;
#endif

		if(ucPoolType >= FBM_POOL_TYPE_NS)
    {
    	Printf("input type error (%d)\n",ucPoolType);
    	return (FBM_GetPoolInfo(FBM_POOL_TYPE_TOTAL));
    }
    prPool = FBM_GetPoolInfo(ucPoolType);

    return prPool;
}

#endif

//-------------------------------------------------------------------------
/** FBM_GetPoolInfoAuto
 *  Get Pool Info
 *  @param  ucPoolType
 *  @param  prInfo
 *  @return NONE.
 */
//-------------------------------------------------------------------------
FBM_POOL_T* FBM_GetPoolInfoAuto(UCHAR ucPoolType, const FBM_POOL_MODULE_INFO_T* prInfo)
{
    FBM_POOL_MODULE_INFO_T rInfo;
    if (prInfo == NULL)
    {
        SRM_VDP_INFO_T* prVdpInfo;
        prVdpInfo = SRM_GetVdpInfo(VDP_1);
        x_memset((VOID*)&rInfo, 0, sizeof(FBM_POOL_MODULE_INFO_T));
        rInfo.u4SourceType = prVdpInfo->u4Source;
        rInfo.u4Resolution = prVdpInfo->u4Resolution;
        rInfo.u4Interlace = prVdpInfo->u4Interlace;
        rInfo.u4VgaTiming = prVdpInfo->u4VgaTiming;
        rInfo.u4ColorFormat444 = prVdpInfo->u4ColorFormat444;

        if (SRM_IsPopVideo())
        {
            rInfo.u4PipPop = SRM_TV_MODE_TYPE_POP;
        }
        else if (SRM_IsPipVideo())
        {
            rInfo.u4PipPop = SRM_TV_MODE_TYPE_PIP;
        }
        prInfo = &rInfo;
    }
    else
    {
        x_memcpy((VOID*)&rInfo, (VOID*)prInfo, sizeof(FBM_POOL_MODULE_INFO_T));
        rInfo.u4Resolution = prInfo->u4Width * prInfo->u4Height;
        prInfo = &rInfo;
    }

    LOG(3, "FBM Get Pool (%s) s(%d) w(%d) h(%d), p(%d), i(%d), r(%d)\n",
        FBM_GetPoolName(ucPoolType),
        prInfo->u4SourceType,
        prInfo->u4Width,
        prInfo->u4Height,
        prInfo->u4PipPop,
        prInfo->u4Interlace,
        prInfo->u4Resolution);
#if defined(CC_MT5399)||defined(CC_MT5890)
     if ((SRMFBM_GetConf() == FBM_MEM_CFG_MT5890_3DTV)
	 	||(SRMFBM_GetConf() == FBM_MEM_CFG_MT5890_3DTV_4K)
	 	||(SRMFBM_GetConf() == FBM_MEM_CFG_MT5890_3DTV_FHD)
	 	||(SRMFBM_GetConf() == FBM_MEM_CFG_MT5861_3DTV)
	 	||(SRMFBM_GetConf() == FBM_MEM_CFG_MT5890_3DTV_2K80)
    	)
    {
        return _FbmGetPoolInfoAuto_MT5890_1GB(ucPoolType, prInfo);
    }
#endif
#if defined(CC_MT5882)
    if (SRMFBM_GetConf() == FBM_MEM_CFG_MT5882_3DTV)
    {
        return _FbmGetPoolInfoAuto_MT5882_1GB(ucPoolType, prInfo);
    }
#endif
    else
    {
        Printf("no fbm table fit \n");
        VERIFY(0);
    }
    return FBM_GetPoolInfo(ucPoolType);
}


//-------------------------------------------------------------------------
/** FBM_GetPoolCurrent
 *  Get Pool Current Use
 *  @param ucPoolType
 *  @return ucPoolType.
 */
//-------------------------------------------------------------------------

UCHAR FBM_GetPoolCurrent(UCHAR ucPoolType)
{
    if (ucPoolType == (UCHAR) FBM_POOL_TYPE_SCPOS_MAIN)
    {
        return (UCHAR) _eCurrentScposMain;
    }
    else if (ucPoolType == (UCHAR) FBM_POOL_TYPE_MJC_1)
    {
        return (UCHAR) _eCurrentMjc;
    }

    return ucPoolType;
}


//-------------------------------------------------------------------------
/** FBM_PrintPoolInfo
 *  Print FBM pool status
 *  @return NONE
 */
//-------------------------------------------------------------------------
#ifdef CC_MT5882
VOID FBM_PrintPoolInfo(FBM_POOL_T* prPool, POOL_LIST_T* prPoolList)
{
    UCHAR ucIdx, ucLoop;

    for (ucLoop = 0; ucLoop < 3; ucLoop++)
    {
        for (ucIdx = 0; ucIdx < (UINT32)FBM_POOL_TYPE_NS; ucIdx++)
        {
            if ((((ucLoop == 0) && (prPool[ucIdx].u4Size > 0))&&(prPool[ucIdx].u4Inherit!= FBM_POOL_ROOT)) ||
                    (((ucLoop == 1) && (prPool[ucIdx].u4Size > 0))&&(prPool[ucIdx].u4Inherit == FBM_POOL_ROOT))||
                    ((ucLoop == 2) && (prPool[ucIdx].u4Size == 0)))
            {
#ifdef FBM_PRINT_STATUS_TOTAL
                LOG(0, "Addr(0x%08x) Size(0x%08x) Total (0x%08x) %s\n",
                    prPool[ucIdx].u4Addr,
                    prPool[ucIdx].u4Size,
                    (prPool[ucIdx].u4Addr + prPool[ucIdx].u4Size),
                    prPoolList[ucIdx].szName);
#else
                if (prPool[ucIdx].u4Size > 0)
                {
                    LOG(0, "Addr(0x%08x) Size(0x%08x) End(0x%08x) Mode(%c, 0x%08x, %04d, %04d) %s %s\n",
                        prPool[ucIdx].u4Addr,
                        prPool[ucIdx].u4Size,
                        (prPool[ucIdx].u4Addr + prPool[ucIdx].u4Size),
                        (prPool[ucIdx].u4Inherit == FBM_POOL_ROOT)?'R':'I',
                        prPool[ucIdx].u4Mode,
                        prPool[ucIdx].u4Width,
                        prPool[ucIdx].u4Height,
                        prPoolList[ucIdx].szName,
                        prPool[ucIdx].fgAlloced ? "(Alloc)" : "");
                }
                else
                {
                    LOG(8, "Addr(0x%08x) Size(0x%08x) End(0x%08x) Mode(%c, 0x%08x, %04d, %04d) %s\n",
                        prPool[ucIdx].u4Addr,
                        prPool[ucIdx].u4Size,
                        (prPool[ucIdx].u4Addr + prPool[ucIdx].u4Size),
                        (prPool[ucIdx].u4Inherit == FBM_POOL_ROOT)?'R':'I',
                        prPool[ucIdx].u4Mode,
                        prPool[ucIdx].u4Width,
                        prPool[ucIdx].u4Height,
                        prPoolList[ucIdx].szName);
                }
#endif
            }
        }
    }
#if defined(CC_TRUSTZONE_SUPPORT) && defined(CC_SVP_SUPPORT)
    for (ucIdx = 0; ucIdx < (UINT32)FBM_POOL_TYPE_TZ_NS; ucIdx++)
    {
        if (_arPoolTZ[ucIdx].u4Size > 0)
        {
            LOG(3, "[TrustZone]Addr(0x%08x) Size(0x%08x) End(0x%08x) %s %s\n",
                    _arPoolTZ[ucIdx].u4Addr,
                    _arPoolTZ[ucIdx].u4Size,
                    (_arPoolTZ[ucIdx].u4Addr + _arPoolTZ[ucIdx].u4Size),
                    (ucIdx == 0) ? "FBM_POOL_TYPE_TZ_DMX" : "FBM_POOL_TYPE_TZ_DMX2",
                    _arPoolTZ[ucIdx].fgAlloced ? "(Alloc)" : "");
                    
        }
    }
#endif
    LOG(1, "Current pool (SCPOS-M=%s, SCPOS-P=%s, MDDI-M=%s, MDDI-P=%s, TDC=%s)\n",
        FBM_GetPoolName((UCHAR)_eCurrentScposMain),
        FBM_GetPoolName((UCHAR)_eCurrentScposPip),
        FBM_GetPoolName((UCHAR)_eCurrentMddiMain),
        FBM_GetPoolName((UCHAR)_eCurrentMddiPip),
        FBM_GetPoolName((UCHAR)_eCurrentTdc));

    LOG(1, "Current pool (MJC=%s, VBI=%s)\n",
        FBM_GetPoolName((UCHAR)_eCurrentMjc),
        FBM_GetPoolName((UCHAR)_eCurrentVbi));

#if defined(CC_MT5882) 
		if (SRMFBM_GetConf() == FBM_MEM_CFG_MT5882_3DTV)
		{
			#if defined(CC_TVP_SUPPORT)
			LOG(0, "FBM_MEM_CFG_MT5882_TVP_3DTV Model\n");
			#else
			LOG(0, "FBM_MEM_CFG_MT5882_3DTV Model\n");
			#endif
		}
#endif

    LOG(9, "0x%08x = FBM_POOL_MODE_8BIT\n", FBM_POOL_MODE_8BIT);
    LOG(9, "0x%08x = FBM_POOL_MODE_10BIT\n", FBM_POOL_MODE_10BIT);
    LOG(9, "0x%08x = FBM_POOL_MODE_C_10BIT\n", FBM_POOL_MODE_C_10BIT);
    LOG(9, "0x%08x = FBM_POOL_MODE_420_FB\n", FBM_POOL_MODE_420_FB);
    LOG(9, "0x%08x = FBM_POOL_MODE_422_FB\n", FBM_POOL_MODE_422_FB);
    LOG(9, "0x%08x = FBM_POOL_MODE_444_FB\n", FBM_POOL_MODE_444_FB);
    LOG(9, "0x%08x = FBM_POOL_MODE_SD\n", FBM_POOL_MODE_SD);
    LOG(9, "0x%08x = FBM_POOL_MODE_720HD\n", FBM_POOL_MODE_720HD);
    LOG(9, "0x%08x = FBM_POOL_MODE_1080HD\n", FBM_POOL_MODE_1080HD);
    LOG(9, "0x%08x = FBM_POOL_MODE_SHARE\n", FBM_POOL_MODE_SHARE);
    LOG(9, "0x%08x = FBM_POOL_MODE_TOGGLE_TRIGGER\n", FBM_POOL_MODE_TOGGLE_TRIGGER);
    LOG(9, "0x%08x = FBM_POOL_MODE_MPEG_NO_COMP\n", FBM_POOL_MODE_MPEG_NO_COMP);
    LOG(9, "0x%08x = FBM_POOL_MODE_MPEG_COMP_75\n", FBM_POOL_MODE_MPEG_COMP_75);
    LOG(9, "0x%08x = FBM_POOL_MODE_MPEG_COMP_50\n", FBM_POOL_MODE_MPEG_COMP_50);
    LOG(9, "0x%08x = FBM_POOL_MODE_MPEG_DBK\n", FBM_POOL_MODE_MPEG_DBK);
    LOG(9, "0x%08x = FBM_POOL_MODE_MJC_DROP\n", FBM_POOL_MODE_MJC_DROP);
    LOG(9, "0x%08x = FBM_POOL_MODE_MJC_NO_DROP\n", FBM_POOL_MODE_MJC_NO_DROP);
    LOG(9, "0x%08x = FBM_POOL_MODE_MJC_NO_COMP\n", FBM_POOL_MODE_MJC_NO_COMP);
    LOG(9, "0x%08x = FBM_POOL_MODE_MJC_Y_COMP4\n", FBM_POOL_MODE_MJC_Y_COMP4);
    LOG(9, "0x%08x = FBM_POOL_MODE_MJC_Y_COMP3\n", FBM_POOL_MODE_MJC_Y_COMP3);
    LOG(9, "0x%08x = FBM_POOL_MODE_NR_3D_Y_C\n", FBM_POOL_MODE_NR_3D_Y_C);
    LOG(9, "0x%08x = FBM_POOL_MODE_NR_3D_Y_ONLY\n", FBM_POOL_MODE_NR_3D_Y_ONLY);
    LOG(9, "0x%08x = FBM_POOL_MODE_NR_2D\n", FBM_POOL_MODE_NR_2D);
    LOG(9, "0x%08x = FBM_POOL_MODE_NR_BYPASS\n", FBM_POOL_MODE_NR_BYPASS);
    LOG(9, "0x%08x = FBM_POOL_MODE_SCPOS_1FB\n", FBM_POOL_MODE_SCPOS_1FB);
    LOG(9, "0x%08x = FBM_POOL_MODE_SCPOS_2FB\n", FBM_POOL_MODE_SCPOS_2FB);
    LOG(9, "0x%08x = FBM_POOL_MODE_SCPOS_3FB\n", FBM_POOL_MODE_SCPOS_3FB);
    LOG(9, "0x%08x = FBM_POOL_MODE_SCPOS_DISP\n", FBM_POOL_MODE_SCPOS_DISP);
    LOG(9, "0x%08x = FBM_POOL_MODE_SCPOS_FULL\n", FBM_POOL_MODE_SCPOS_FULL);
    LOG(9, "0x%08x = FBM_POOL_MODE_SCPOS_PSCAN_DISP\n", FBM_POOL_MODE_SCPOS_PSCAN_DISP);
    LOG(9, "0x%08x = FBM_POOL_MODE_SCPOS_MJC_DISP\n", FBM_POOL_MODE_SCPOS_MJC_DISP);
    LOG(9, "0x%08x = FBM_POOL_MODE_SCPOS_LINESYNC_DISP\n", FBM_POOL_MODE_SCPOS_LINESYNC_DISP);
    LOG(9, "0x%08x = FBM_POOL_MODE_MDDI_BOB\n", FBM_POOL_MODE_MDDI_BOB);
    LOG(9, "0x%08x = FBM_POOL_MODE_MDDI_FULL\n", FBM_POOL_MODE_MDDI_FULL);
    LOG(9, "0x%08x = FBM_POOL_MODE_MDDI_CBOB\n", FBM_POOL_MODE_MDDI_CBOB);
    LOG(9, "0x%08x = FBM_POOL_MODE_MDDI_NR_Y_C\n", FBM_POOL_MODE_MDDI_NR_Y_C);
    LOG(9, "0x%08x = FBM_POOL_MODE_MDDI_NR_Y_ONLY\n", FBM_POOL_MODE_MDDI_NR_Y_ONLY);
    LOG(9, "0x%08x = FBM_POOL_MODE_MDDI_NR_OFF\n", FBM_POOL_MODE_MDDI_NR_OFF);
    LOG(9, "0x%08x = FBM_POOL_MODE_MDDI_NR_10BIT\n", FBM_POOL_MODE_MDDI_NR_10BIT);
    LOG(9, "0x%08x = FBM_POOL_MODE_MDDI_COMP\n", FBM_POOL_MODE_MDDI_COMP);
    LOG(9, "0x%08x = FBM_POOL_MODE_MDDI_DISP\n", FBM_POOL_MODE_MDDI_DISP);
    LOG(9, "0x%08x = FBM_POOL_MODE_MDDI_P2I\n", FBM_POOL_MODE_MDDI_P2I);
    LOG(9, "0x%08x = FBM_TV_MODE_TYPE_NORMAL\n", FBM_TV_MODE_TYPE_NORMAL);
    LOG(9, "0x%08x = FBM_TV_MODE_TYPE_PIP\n", FBM_TV_MODE_TYPE_PIP);
    LOG(9, "0x%08x = FBM_TV_MODE_TYPE_POP\n", FBM_TV_MODE_TYPE_POP);

    LOG(5, "NPTV Main Begin Addr:          0x%08x\n", _fbm_u4NptvBeAddr);
	#ifndef CC_MT5881
    LOG(5, "NPTV Sub PIP Begin Addr:       0x%08x\n", _fbm_u4NptvSubPipAddr);
    LOG(5, "NPTV Sub POP Begin Addr:       0x%08x\n", _fbm_u4NptvSubPopAddr);
    LOG(5, "Main pip: (EndAddr,Size,Mode): 0x%08x, 0x%08x, 0x%08x\n", _FBM_NPTV_VideoRange.u4MainPipMaxEnd, _FBM_NPTV_VideoRange.u4MainPipMaxSize, _FBM_NPTV_VideoRange.u1MainPipMaxMode);
    LOG(5, "Sub  pip: (EndAddr,Size,Mode): 0x%08x, 0x%08x, 0x%08x\n", _FBM_NPTV_VideoRange.u4SubPipMaxEnd, _FBM_NPTV_VideoRange.u4SubPipMaxSize, _FBM_NPTV_VideoRange.u1SubPipMaxMode);
    LOG(5, "Main pop: (EndAddr,Size,Mode): 0x%08x, 0x%08x, 0x%08x\n", _FBM_NPTV_VideoRange.u4MainPopMaxEnd, _FBM_NPTV_VideoRange.u4MainPopMaxSize, _FBM_NPTV_VideoRange.u1MainPopMaxMode);
    LOG(5, "Sub  pop: (EndAddr,Size,Mode): 0x%08x, 0x%08x, 0x%08x\n", _FBM_NPTV_VideoRange.u4SubPopMaxEnd, _FBM_NPTV_VideoRange.u4SubPopMaxSize, _FBM_NPTV_VideoRange.u1SubPopMaxMode);
    #endif
}

#else
VOID FBM_PrintPoolInfo(FBM_POOL_T* prPool, POOL_LIST_T* prPoolList)
{
    UCHAR ucIdx, ucLoop,bchannelA=0,bchannelB=0,bchannelc=0;

    for (ucLoop = 0; ucLoop < 3; ucLoop++)
    {
        for (ucIdx = 0; ucIdx < (UINT32)FBM_POOL_TYPE_NS; ucIdx++)
        {
            if ((((ucLoop == 0) && (prPool[ucIdx].u4Addr < DRAM_CHB_START_ADDR)) ||
                    ((ucLoop == 1) &&((prPool[ucIdx].u4Addr >= DRAM_CHB_START_ADDR)&&(prPool[ucIdx].u4Addr < DRAM_CHC_START_ADDR)))
                    ||((ucLoop == 2)&&(prPool[ucIdx].u4Addr >= DRAM_CHC_START_ADDR))
                    )
				)
            {
             		if((ucLoop == 0) && (bchannelA==0)&&((prPool[ucIdx].u4Addr > 0)&&(prPool[ucIdx].u4Addr < DRAM_CHB_START_ADDR)))
             		{
             		LOG(0,"-------------CHANNEL A ----------------\n");
					bchannelA =1;
             		}
				else if((ucLoop == 1) && (bchannelB==0)&&((prPool[ucIdx].u4Addr >= DRAM_CHB_START_ADDR)&&(prPool[ucIdx].u4Addr < DRAM_CHC_START_ADDR)))
             		{
             		LOG(0,"-------------CHANNEL B ---------------- \n");
					bchannelB =1;
             		}
				else if((ucLoop == 2) && (bchannelc==0)&&(prPool[ucIdx].u4Addr >= DRAM_CHC_START_ADDR)&&!IS_IC_5861())
             		{
             		LOG(0,"-------------CHANNEL C ---------------- \n");
					bchannelc =1;
             		}
#ifdef FBM_PRINT_STATUS_TOTAL
                LOG(0, "Addr(0x%08x) Size(0x%08x) Total (0x%08x) %s %s\n",
                    prPool[ucIdx].u4Addr,
                    prPool[ucIdx].u4Size,
                    (prPool[ucIdx].u4Addr + prPool[ucIdx].u4Size),
                    prPoolList[ucIdx].szName,
                    prPool[ucIdx].fgAlloced ? "(Alloc)" : "");
#else
                if (prPool[ucIdx].u4Size > 0)
                {
                    LOG(0, "Addr(0x%08x) Size(0x%08x) End(0x%08x) Mode(%c, 0x%08x, %04d, %04d) %s %s\n",
                        prPool[ucIdx].u4Addr,
                        prPool[ucIdx].u4Size,
                        (prPool[ucIdx].u4Addr + prPool[ucIdx].u4Size),
                        (prPool[ucIdx].u4Inherit == FBM_POOL_ROOT)?'R':'I',
                        prPool[ucIdx].u4Mode,
                        prPool[ucIdx].u4Width,
                        prPool[ucIdx].u4Height,
                        prPoolList[ucIdx].szName,
                        prPool[ucIdx].fgAlloced ? "(Alloc)" : "");
                }
                else
                {
                    LOG(8, "Addr(0x%08x) Size(0x%08x) End(0x%08x) Mode(%c, 0x%08x, %04d, %04d) %s\n",
                        prPool[ucIdx].u4Addr,
                        prPool[ucIdx].u4Size,
                        (prPool[ucIdx].u4Addr + prPool[ucIdx].u4Size),
                        (prPool[ucIdx].u4Inherit == FBM_POOL_ROOT)?'R':'I',
                        prPool[ucIdx].u4Mode,
                        prPool[ucIdx].u4Width,
                        prPool[ucIdx].u4Height,
                        prPoolList[ucIdx].szName);
                }
             		
             	}
#endif
            }
    }
#if defined(CC_TRUSTZONE_SUPPORT) && defined(CC_SVP_SUPPORT)
    for (ucIdx = 0; ucIdx < (UINT32)FBM_POOL_TYPE_TZ_NS; ucIdx++)
    {
        if (_arPoolTZ[ucIdx].u4Size > 0)
        {
            LOG(3, "[TrustZone]Addr(0x%08x) Size(0x%08x) End(0x%08x) %s %s\n",
                    _arPoolTZ[ucIdx].u4Addr,
                    _arPoolTZ[ucIdx].u4Size,
                    (_arPoolTZ[ucIdx].u4Addr + _arPoolTZ[ucIdx].u4Size),
                    (ucIdx == 0) ? "FBM_POOL_TYPE_TZ_DMX" : "FBM_POOL_TYPE_TZ_DMX2",
                    _arPoolTZ[ucIdx].fgAlloced ? "(Alloc)" : "");
                    
        }
    }
#endif
    LOG(0, "Current pool (SCPOS-M=%s, SCPOS-P=%s, MDDI-M=%s, MDDI-P=%s, TDC=%s)\n",
        FBM_GetPoolName((UCHAR)_eCurrentScposMain),
        FBM_GetPoolName((UCHAR)_eCurrentScposPip),
        FBM_GetPoolName((UCHAR)_eCurrentMddiMain),
        FBM_GetPoolName((UCHAR)_eCurrentMddiPip),
        FBM_GetPoolName((UCHAR)_eCurrentTdc));

    LOG(0, "Current pool (MJC=%s, VBI=%s)\n",
        FBM_GetPoolName((UCHAR)_eCurrentMjc),
        FBM_GetPoolName((UCHAR)_eCurrentVbi));

#if defined(CC_MT5399)||defined(CC_MT5890)
	if (SRMFBM_GetConf() == FBM_MEM_CFG_MT5890_3DTV)
	{
		LOG(0, "FBM_MEM_CFG_MT5890_3DTV Model\n");
	}
	else if (SRMFBM_GetConf() == FBM_MEM_CFG_MT5890_3DTV_4K)
	{
		LOG(0, "FBM_MEM_CFG_MT5890_3DTV_4K Model\n");
	}
	else if (SRMFBM_GetConf() == FBM_MEM_CFG_MT5890_3DTV_FHD)
	{
		LOG(0, "FBM_MEM_CFG_MT5890_3DTV_FHD Model\n");
	}
	else if (SRMFBM_GetConf() == FBM_MEM_CFG_MT5861_3DTV)
	{
		LOG(0, "FBM_MEM_CFG_MT5861_3DTV Model\n");
	}
	else if (SRMFBM_GetConf() == FBM_MEM_CFG_MT5890_3DTV_2K80)
	{
		LOG(0, "FBM_MEM_CFG_MT5890_3DTV_2K80 Model\n");
	}
#endif
#if defined(CC_MT5882) 
	if (SRMFBM_GetConf() == FBM_MEM_CFG_MT5882_3DTV)
	{
		LOG(0, "FBM_MEM_CFG_MT5882_3DTV Model\n");
	}		
#endif

    LOG(9, "0x%08x = FBM_POOL_MODE_8BIT\n", FBM_POOL_MODE_8BIT);
    LOG(9, "0x%08x = FBM_POOL_MODE_10BIT\n", FBM_POOL_MODE_10BIT);
    LOG(9, "0x%08x = FBM_POOL_MODE_C_10BIT\n", FBM_POOL_MODE_C_10BIT);
    LOG(9, "0x%08x = FBM_POOL_MODE_420_FB\n", FBM_POOL_MODE_420_FB);
    LOG(9, "0x%08x = FBM_POOL_MODE_422_FB\n", FBM_POOL_MODE_422_FB);
    LOG(9, "0x%08x = FBM_POOL_MODE_444_FB\n", FBM_POOL_MODE_444_FB);
    LOG(9, "0x%08x = FBM_POOL_MODE_SD\n", FBM_POOL_MODE_SD);
    LOG(9, "0x%08x = FBM_POOL_MODE_720HD\n", FBM_POOL_MODE_720HD);
    LOG(9, "0x%08x = FBM_POOL_MODE_1080HD\n", FBM_POOL_MODE_1080HD);
    LOG(9, "0x%08x = FBM_POOL_MODE_SHARE\n", FBM_POOL_MODE_SHARE);
    LOG(9, "0x%08x = FBM_POOL_MODE_TOGGLE_TRIGGER\n", FBM_POOL_MODE_TOGGLE_TRIGGER);
    LOG(9, "0x%08x = FBM_POOL_MODE_MPEG_NO_COMP\n", FBM_POOL_MODE_MPEG_NO_COMP);
    LOG(9, "0x%08x = FBM_POOL_MODE_MPEG_COMP_75\n", FBM_POOL_MODE_MPEG_COMP_75);
    LOG(9, "0x%08x = FBM_POOL_MODE_MPEG_COMP_50\n", FBM_POOL_MODE_MPEG_COMP_50);
    LOG(9, "0x%08x = FBM_POOL_MODE_MPEG_DBK\n", FBM_POOL_MODE_MPEG_DBK);
    LOG(9, "0x%08x = FBM_POOL_MODE_MJC_DROP\n", FBM_POOL_MODE_MJC_DROP);
    LOG(9, "0x%08x = FBM_POOL_MODE_MJC_NO_DROP\n", FBM_POOL_MODE_MJC_NO_DROP);
    LOG(9, "0x%08x = FBM_POOL_MODE_MJC_NO_COMP\n", FBM_POOL_MODE_MJC_NO_COMP);
    LOG(9, "0x%08x = FBM_POOL_MODE_MJC_Y_COMP4\n", FBM_POOL_MODE_MJC_Y_COMP4);
    LOG(9, "0x%08x = FBM_POOL_MODE_MJC_Y_COMP3\n", FBM_POOL_MODE_MJC_Y_COMP3);
    LOG(9, "0x%08x = FBM_POOL_MODE_NR_3D_Y_C\n", FBM_POOL_MODE_NR_3D_Y_C);
    LOG(9, "0x%08x = FBM_POOL_MODE_NR_3D_Y_ONLY\n", FBM_POOL_MODE_NR_3D_Y_ONLY);
    LOG(9, "0x%08x = FBM_POOL_MODE_NR_2D\n", FBM_POOL_MODE_NR_2D);
    LOG(9, "0x%08x = FBM_POOL_MODE_NR_BYPASS\n", FBM_POOL_MODE_NR_BYPASS);
    LOG(9, "0x%08x = FBM_POOL_MODE_SCPOS_1FB\n", FBM_POOL_MODE_SCPOS_1FB);
    LOG(9, "0x%08x = FBM_POOL_MODE_SCPOS_2FB\n", FBM_POOL_MODE_SCPOS_2FB);
    LOG(9, "0x%08x = FBM_POOL_MODE_SCPOS_3FB\n", FBM_POOL_MODE_SCPOS_3FB);
    LOG(9, "0x%08x = FBM_POOL_MODE_SCPOS_DISP\n", FBM_POOL_MODE_SCPOS_DISP);
    LOG(9, "0x%08x = FBM_POOL_MODE_SCPOS_FULL\n", FBM_POOL_MODE_SCPOS_FULL);
    LOG(9, "0x%08x = FBM_POOL_MODE_SCPOS_PSCAN_DISP\n", FBM_POOL_MODE_SCPOS_PSCAN_DISP);
    LOG(9, "0x%08x = FBM_POOL_MODE_SCPOS_MJC_DISP\n", FBM_POOL_MODE_SCPOS_MJC_DISP);
    LOG(9, "0x%08x = FBM_POOL_MODE_SCPOS_LINESYNC_DISP\n", FBM_POOL_MODE_SCPOS_LINESYNC_DISP);
    LOG(9, "0x%08x = FBM_POOL_MODE_MDDI_BOB\n", FBM_POOL_MODE_MDDI_BOB);
    LOG(9, "0x%08x = FBM_POOL_MODE_MDDI_FULL\n", FBM_POOL_MODE_MDDI_FULL);
    LOG(9, "0x%08x = FBM_POOL_MODE_MDDI_CBOB\n", FBM_POOL_MODE_MDDI_CBOB);
    LOG(9, "0x%08x = FBM_POOL_MODE_MDDI_NR_Y_C\n", FBM_POOL_MODE_MDDI_NR_Y_C);
    LOG(9, "0x%08x = FBM_POOL_MODE_MDDI_NR_Y_ONLY\n", FBM_POOL_MODE_MDDI_NR_Y_ONLY);
    LOG(9, "0x%08x = FBM_POOL_MODE_MDDI_NR_OFF\n", FBM_POOL_MODE_MDDI_NR_OFF);
    LOG(9, "0x%08x = FBM_POOL_MODE_MDDI_NR_10BIT\n", FBM_POOL_MODE_MDDI_NR_10BIT);
    LOG(9, "0x%08x = FBM_POOL_MODE_MDDI_COMP\n", FBM_POOL_MODE_MDDI_COMP);
    LOG(9, "0x%08x = FBM_POOL_MODE_MDDI_DISP\n", FBM_POOL_MODE_MDDI_DISP);
    LOG(9, "0x%08x = FBM_POOL_MODE_MDDI_P2I\n", FBM_POOL_MODE_MDDI_P2I);
    LOG(9, "0x%08x = FBM_TV_MODE_TYPE_NORMAL\n", FBM_TV_MODE_TYPE_NORMAL);
    LOG(9, "0x%08x = FBM_TV_MODE_TYPE_PIP\n", FBM_TV_MODE_TYPE_PIP);
    LOG(9, "0x%08x = FBM_TV_MODE_TYPE_POP\n", FBM_TV_MODE_TYPE_POP);
#ifndef CC_MT5881
    LOG(5, "Main pip: (EndAddr,Size,Mode): 0x%08x, 0x%08x, 0x%08x\n", _FBM_NPTV_VideoRange.u4MainPipMaxEnd, _FBM_NPTV_VideoRange.u4MainPipMaxSize, _FBM_NPTV_VideoRange.u1MainPipMaxMode);
    LOG(5, "Sub  pip: (EndAddr,Size,Mode): 0x%08x, 0x%08x, 0x%08x\n", _FBM_NPTV_VideoRange.u4SubPipMaxEnd, _FBM_NPTV_VideoRange.u4SubPipMaxSize, _FBM_NPTV_VideoRange.u1SubPipMaxMode);
    LOG(5, "Main pop: (EndAddr,Size,Mode): 0x%08x, 0x%08x, 0x%08x\n", _FBM_NPTV_VideoRange.u4MainPopMaxEnd, _FBM_NPTV_VideoRange.u4MainPopMaxSize, _FBM_NPTV_VideoRange.u1MainPopMaxMode);
    LOG(5, "Sub  pop: (EndAddr,Size,Mode): 0x%08x, 0x%08x, 0x%08x\n", _FBM_NPTV_VideoRange.u4SubPopMaxEnd, _FBM_NPTV_VideoRange.u4SubPopMaxSize, _FBM_NPTV_VideoRange.u1SubPopMaxMode);
#endif
    LOG(5, "NPTV Main Begin Addr:          0x%08x\n", _fbm_u4NptvBeAddr);
}
#endif
//-------------------------------------------------------------------------
/** FBM_ConfigurePool
 *  Initialize this FBM pool
 *  @retval NONE
 */
//-------------------------------------------------------------------------
VOID FBM_ConfigurePool(POOL_LIST_T* prPoolList, POOL_ARRANGE_INFO_T* prPoolArrangeInfo)
{
#if defined(CC_MT5399)||defined(CC_MT5890)
	 if ((SRMFBM_GetConf() == FBM_MEM_CFG_MT5890_3DTV)
	 		)
    {
        _FbmConfigurePool_MT5890_MJC_SCPOS_MAIN(prPoolList, prPoolArrangeInfo);
    }
	else if(SRMFBM_GetConf() == FBM_MEM_CFG_MT5890_3DTV_4K)
		{
		_FbmConfigurePool_MT5890_4K_SCPOS_MAIN(prPoolList, prPoolArrangeInfo);
		}
	else if(SRMFBM_GetConf() == FBM_MEM_CFG_MT5890_3DTV_FHD)
		{
		_FbmConfigurePool_MT5890_FHD_SCPOS_MAIN(prPoolList, prPoolArrangeInfo);
		}
	else if(SRMFBM_GetConf() == FBM_MEM_CFG_MT5890_3DTV_2K80)
		{
		_FbmConfigurePool_MT5890_2K80_SCPOS_MAIN(prPoolList, prPoolArrangeInfo);
		}
	else if(SRMFBM_GetConf() == FBM_MEM_CFG_MT5861_3DTV)
		{
		_FbmConfigurePool_MT5861_MJC_SCPOS_MAIN(prPoolList, prPoolArrangeInfo);
		}
#endif
#if defined(CC_MT5882)
    if (SRMFBM_GetConf() == FBM_MEM_CFG_MT5882_3DTV)
    {
    	#if defined(CC_TVP_SUPPORT)
		_FbmConfigurePool_MT5882_TVP_SCPOS_MAIN( prPoolList,prPoolArrangeInfo);
		#else
		_FbmConfigurePool_MT5882_SCPOS_MAIN( prPoolList,prPoolArrangeInfo);
		#endif
	}
#endif
    else
    {
        VERIFY(0);
    }
}

