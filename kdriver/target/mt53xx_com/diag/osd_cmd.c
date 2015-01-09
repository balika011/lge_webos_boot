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
 * $RCSfile: osd_cmd.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file osd_cmd.c
 *  This header file includes CLI function definitions of OSD driver.
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "osd_cmd.h"
#include "osd_drvif.h"


#include "x_printf.h"
#include "x_stl_lib.h"
#include "x_hal_926.h"
#include "x_hal_5381.h"

#ifdef CC_5391_LOADER
#define DEFINE_IS_LOG(level, fmt...) Printf(fmt)
#else /* CC_5391_LOADER */
#define DEFINE_IS_LOG OSD_IsLog
#endif /* CC_5391_LOADER */
#include "x_debug.h"
#include "fbm_drvif.h"

#include "gfx_if.h"
#ifdef OSD_ENABLE_VERIFY
#include "x_bim.h"
#endif

#ifdef CC_TDTV_3D_OSD_ENABLE
extern BOOL gfgOsd_Sbs ;
extern BOOL gfgOsd_Tbb ;
extern UINT8 _u13DOsdShift;
#endif

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------

typedef struct _TEXT2ENUM_T
{
    CHAR* szText;
    INT32 i4Value;
} TEXT2ENUM_T;

typedef struct _OSD_PTTRN_DATA_T
{
    UINT8  ui1_mode;
    UINT32 ui4_pttrn_id;
    UINT32 ui4_bg_color;
    UINT32 ui4_grid_w;
    UINT32 ui4_grid_h;
    UINT32 u4ColorIdx1;
    UINT32 u4ColorIdx2;
    UINT32 u4ColorIdx3;
}OSD_PTTRN_DATA_T;


//-----------------------------------------------------------------------------
// Imported functions
//-----------------------------------------------------------------------------

EXTERN UINT32 PANEL_GetPanelHeight(void);
EXTERN UINT32 PANEL_GetPanelWidth(void);
EXTERN INT32 _OSD_BASE_UpdateHwReg(void);
EXTERN INT32 _OSD_PLA_GetReg(UINT32 u4Plane, UINT32 *pOsdPlaneReg);
EXTERN INT32 _OSD_PLA_SetReg(UINT32 u4Plane, const UINT32 *pOsdPlaneReg);
EXTERN INT32 _OSD_SC_UpdateHwReg(UINT32 u4Scaler);
EXTERN INT32 _OSD_BASE_SetOSD12Ex(UINT32 u4Value);

//#define _TEST_SCROLL__

//-----------------------------------------------------------------------------
// Static function forward declarations
//-----------------------------------------------------------------------------
#ifdef OSD_TRANS_CTRL
    #if LG_OSD_PATH_TEST
static INT32 _OsdCmdGetFRCHeader(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _OsdCmdLGOSDTest(INT32 i4Argc, const CHAR ** szArgv);
    #endif
#endif
#ifdef _TEST_SCROLL__
static INT32 _OsdCmdScroll(INT32 i4Argc, const CHAR ** szArgv);
#endif
static INT32 _OsdCmdInit(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _OsdCmdMode(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _OsdCmdQuery(INT32 i4Argc, const CHAR ** szArgv);

static INT32 _OsdPlaneCmdReset(INT32 i4Argc, const CHAR ** szArgv);
INT32 _OsdPlaneCmdEnable(INT32 i4Argc, const CHAR ** szArgv);
INT32 _OsdPlaneCmdPattern(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _OsdPlaneCmdFlip(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _OsdPlaneCmdAlpha(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _OsdPlaneCmdFading(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _OsdPlaneCmdHFilter(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _OsdPlaneCmdVFilter(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _OsdPlaneCmdFifo(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _OsdPlaneCmdDumpReg(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _OsdPlaneCmdDumpInfo(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _OsdPlaneCmdCompress(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _OsdSonyOSDPattern(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _OsdSonyOSDOrder(INT32 i4Argc, const CHAR ** szArgv);

static INT32 _OsdListCmdCreate(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _OsdListCmdList(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _OsdListCmdDump(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _OsdListCmdDelete(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _OsdListCmdDetach(INT32 i4Argc, const CHAR ** szArgv);

static INT32 _OsdRgnCmdHelp(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _OsdRgnCmdNew(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _OsdRgnCmdInsert(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _OsdRgnCmdDelete(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _OsdRgnCmdSet(INT32 i4Argc, const CHAR ** szArgv);
INT32 _OsdRgnCmdPattern(INT32 i4Argc, const CHAR ** szArgv);

static INT32 _OsdScCmdScale(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _OsdScCmdSetLpf(INT32 i4Argc, const CHAR ** szArgv);
//#ifdef CC_MT5392B
#if 1
static INT32 _OsdGpCmdMode(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _OsdGpCmdTx(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _OsdGpCmdWriteReg(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _OsdGpCmdWriteReg32bit(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _OsdGpCmdCopy(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _OsdGpCmdSet8281Reg(INT32 i4Argc, const CHAR ** szArgv);
#endif
static INT32 _OsdCmdSetLVDS3(INT32 i4Argc, const CHAR ** szArgv);

static INT32 _OsdCmdSetPowerDwn(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _OsdCmdSetTVEout(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _OsdCmdShowLogo(INT32 i4Argc, const CHAR ** szArgv);

#ifdef CC_MT5365
static INT32 _OsdCmdGetUnderflowStatus(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _OsdCmdClearUnderflowStatus(INT32 i4Argc, const CHAR ** szArgv);
#endif
static INT32 _OsdCmdPattern(INT32 i4Argc, const CHAR ** szArgv);
INT32 _OsdDrawPattern(INT32 i4Region, OSD_PTTRN_DATA_T* pt_data);
#ifdef CC_TDTV_3D_OSD_ENABLE
void _OsdCmdShow3DSbs(INT32 i4Argc, const CHAR ** szArgv);
void _OsdCmdShow3DTbb(INT32 i4Argc, const CHAR ** szArgv);
void _OsdCmdShow3DShutter(INT32 i4Argc, const CHAR ** szArgv);
#endif

static INT32  _OsdCmdSet3DPrLineShift(INT32 i4Argc, const CHAR ** szArgv);
static UINT8 _Osd_get_cm_size(UINT32 ui4_cm);
static INT32 _OsdCmdCmpressEnable(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _OsdCmdSbsEnable(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _OsdCmdCmpressBorrowBuf(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _OsdCmd3DEnable(INT32 i4Argc, const CHAR ** szArgv);

static INT32  _OsdCmdIsDisplaying(INT32 i4Argc, const CHAR ** szArgv);
#ifdef CC_ANDROID_3D_UI_SUPPORT
static INT32 _OsdCmdSetUiMode(INT32 i4Argc, const CHAR ** szArgv);
#endif

static INT32 _OsdScCmdSetDbgInfo(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _OsdScCmdGetDbgInfo(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _OsdCmdShowPattern(INT32 i4Argc, const CHAR ** szArgv);


#ifndef CC_5391_LOADER
CLIMOD_DEBUG_FUNCTIONS(OSD)
#endif

INT32 _ExOsdCmdPattern(INT32 i4pattern );
void _OsdExPatternClip(INT32 i4Vclip,INT32 _i4Delay);
void _ExPatternOff(void);

//----------------------------------------------------------------------------
// OSD Verification
//mtk70854
//----------------------------------------------------------------------------
//#define OSD_ENABLE_VERIFY

#ifdef OSD_ENABLE_VERIFY

#include "imgrz_if.h"
#include "imgrz_drvif.h"

typedef struct
{
    UINT32 u4PrepareAllocBuffer;
	UINT32 u4PrepareWorkBuffer;
	UINT32 u4PrepareWidth;
	UINT32 u4PrepareHeight;
	UINT32 u4PrepareColorMode;
}OSD_VERIFY_INFO_T;

typedef struct
{
    const UINT32 u4Plane;
	const UINT32 u4Scaler;
	
    UINT32 u4PrepareAllocBuffer;
	UINT32 u4PrepareWorkBuffer;
	UINT32 u4PrepareWidth;
	UINT32 u4PrepareHeight;
	UINT32 u4PrepareColorMode;

	UINT32 u4RgnList;
	UINT32 u4Region;
	UINT32 u4ShutterRgn;
	UINT32 u4RgnAllocBuffer;
	UINT32 u4RgnWorkBuffer;
	UINT32 u4RgnWidth;
	UINT32 u4RgnHeight;
	UINT32 u4RgnColorMode;
}OSD_VERIFY_PLANE_INFO_T;

typedef struct
{
    UINT8 *ucDesc;
    INT32 (*pfnVerifyFunc)(UINT32 u4Plane);
}OSD_VERIFY_FUNC_T;

static OSD_VERIFY_INFO_T rVerifyInfo = {
    .u4PrepareAllocBuffer = 0
};

static OSD_VERIFY_PLANE_INFO_T arVerifyPlane[] = 
{
    {
		.u4Plane = OSD_PLANE_1,
		.u4Scaler = OSD_SCALER_1,
		.u4PrepareAllocBuffer = 0,
		.u4RgnAllocBuffer = 0,
		.u4Region = (UINT32) (-1)
	},
    {
	    .u4Plane = OSD_PLANE_2,
		.u4Scaler = OSD_SCALER_2,
		.u4RgnAllocBuffer = 0,
		.u4PrepareAllocBuffer = 0,
		.u4Region = (UINT32) (-1)
    },
    {
		.u4Plane = OSD_PLANE_3,
		.u4Scaler = OSD_SCALER_3,
		.u4RgnAllocBuffer = 0,
		.u4PrepareAllocBuffer = 0,
		.u4Region = (UINT32) (-1)
	},
    {
		.u4Plane = OSD_PLANE_4,
		.u4PrepareAllocBuffer = 0,
		.u4RgnAllocBuffer = 0,
		.u4Region = (UINT32) (-1)
	},
    {
		.u4Plane = OSD_PLANE_5,
		.u4PrepareAllocBuffer = 0,
		.u4RgnAllocBuffer = 0,
		.u4Region = (UINT32) (-1)
	}
};

EXTERN UINT8 SerPollGetChar(void);
EXTERN INT32 _OSD_RGN_GetAddress(UINT32 u4Region, UINT32 *pu4Addr);
EXTERN INT32 _OSD_PLA_SetRightHeaderAddr(UINT32 u4Plane, UINT32 u4Value);

UINT32 GFX_Get_Bpp(UINT32 u4ColorMode)
{
    UINT32 u4Bpp = 4;
	
    switch (u4ColorMode)
    {
        case CM_RGB565_DIRECT16:
            u4Bpp = 2;
            break;
        case CM_ARGB4444_DIRECT16:
            u4Bpp = 2;
            break;
        case CM_ARGB8888_DIRECT32:
            u4Bpp = 4;
            break;
        default:
            printf("GFX_Get_IMGRZ_CM() error: bad eSrcCm %d!\n",u4ColorMode);
            break;
    }
    return u4Bpp;
}

static OSD_VERIFY_PLANE_INFO_T *OSD_Get_Verify_Plane(UINT32 u4Plane)
{
    return &arVerifyPlane[u4Plane];
}

static INT32 OSD_Verify_Blit(UINT32 u4SrcAddr, UINT32 u4SrcWidth, UINT32 u4SrcHeight, UINT32 u4SrcColorMode,
	UINT32 u4DstAddr, UINT32 u4DstWidth, UINT32 u4DstHeight, UINT32 u4DstColorMode)
{
    INT32 i4Ret = 0;
    RZ_OSD_SCL_PARAM_SET_T rSclParam;

    do
    {
        x_memset(&rSclParam, 0, sizeof(RZ_OSD_SCL_PARAM_SET_T));

        rSclParam.u4SrcCM = GFX_Get_IMGRZ_CM(u4SrcColorMode);
        rSclParam.u4TgCM  = GFX_Get_IMGRZ_CM(u4DstColorMode);
        //src original, absolute coordinate
        rSclParam.u4SrcBase = (UINT32) u4SrcAddr;
        rSclParam.u4IsSrcDirCM = (UINT32) 1;
        rSclParam.u4SrcBufLen = (UINT32) (u4SrcWidth * GFX_Get_Bpp(u4SrcColorMode) + 0xf) & 0xfffffff0;
        rSclParam.u4SrcHOffset = (UINT32) 0;
        rSclParam.u4SrcVOffset = (UINT32) 0;
        rSclParam.u4SrcW = u4SrcWidth;
        rSclParam.u4SrcH = u4SrcHeight;

        rSclParam.u4TgBase = u4DstAddr;
		
        //target original   , absolute coordinate
        rSclParam.u4TgHOffset = (UINT32) 0;
        rSclParam.u4TgVOffset = (UINT32) 0;
        rSclParam.u4TgW = (UINT32) u4DstWidth;
        rSclParam.u4TgH = (UINT32) u4DstHeight;
        rSclParam.u4TgBufLen = (UINT32) (u4DstWidth * GFX_Get_Bpp(u4DstColorMode) + 0xf) & 0xfffffff0;
        
        rSclParam.u4IOMMURead = 0x0;
        rSclParam.u4IOMMUWrite= 0x0;
        
        GFX_Stretchblit(&rSclParam);
    }while(0);

    return i4Ret;
}


static INT32 OSD_GFX_Compress(UINT32 u4SrcAddr, UINT32 u4SrcColorMode, UINT32 u4SrcPitch,
	UINT32 u4DstAddr, UINT32 u4Width, UINT32 u4Height, UINT32 u4Mode, UINT32 u4Line, UINT32 u4BurstRead, UINT32 u4RollbackEn)
{
    GFX_MMU_T rMMU;
	rMMU.u4_enable = 0;
	rMMU.u4_src_rw_mmu = 0;
	rMMU.u4_dst_rw_mmu = 0;
	
    GFX_Lock();

    GFX_NewCompressBlt((UINT8 *)u4SrcAddr, 0, 0, u4SrcColorMode,
            u4SrcPitch, (UINT8 *)u4DstAddr, u4Width, u4Height, u4Height,
            u4RollbackEn, u4Mode, 0xFFFFFFFF, 0, u4BurstRead, (BOOL) u4Line,(void*)&rMMU);

    GFX_Flush();
    GFX_Wait();
    GFX_Unlock();

	return 0;
}

UINT32 GetLineSeperateCompressPitch(UINT32 u4OriginalPitch, BOOL fgRollBackEn, BOOL fgBurstRead)
{
    UINT32 u4OriginalLineBit, u4CompressLineBit, u4CompressPitch;

	u4OriginalLineBit = u4OriginalPitch * 8;
	
	if(fgRollBackEn)
	{
	    u4CompressLineBit = u4OriginalLineBit + 4;
	}
	else
	{
	    u4CompressLineBit = u4OriginalLineBit + 4 + u4OriginalLineBit / 16;
	}

	if(fgBurstRead)
	{
	    u4CompressLineBit = (u4CompressLineBit % 512 == 0) ? u4CompressLineBit : (u4CompressLineBit / 512 + 1) * 512;
	}
	else
	{
	    u4CompressLineBit = (u4CompressLineBit % 128 == 0) ? u4CompressLineBit : (u4CompressLineBit / 128 + 1) * 128;
	}

	u4CompressPitch = u4CompressLineBit / 8;

	return u4CompressPitch;
}

static INT32 OSD_PLA_Verify_Decompress(UINT32 u4Plane)
{
    INT32 i, j, k, l;
    UINT32 u4SrcPitch, u4OriginalPitch;
    OSD_VERIFY_PLANE_INFO_T *prPlaneInfo;

    prPlaneInfo = OSD_Get_Verify_Plane(u4Plane);
    ASSERT((prPlaneInfo != NULL));

	u4OriginalPitch = u4SrcPitch = (UINT32) (prPlaneInfo->u4PrepareWidth * GFX_Get_Bpp(prPlaneInfo->u4PrepareColorMode) + 0xf) & 0xfffffff0;

	Printf("Please Make Sure load image into buffer[0x%x]....\n press any key to continue !\n", prPlaneInfo->u4PrepareWorkBuffer);

    OSD_DrawRamp((UINT8*)prPlaneInfo->u4PrepareWorkBuffer, prPlaneInfo->u4PrepareWidth, prPlaneInfo->u4PrepareHeight, prPlaneInfo->u4PrepareColorMode, 0xff, 0);
	SerPollGetChar();

    OSD_PLA_Enable(u4Plane, 0);
	OSD_RGN_Set(prPlaneInfo->u4Region, OSD_RGN_DEC_EN, 1);
	OSD_Verify_Blit(prPlaneInfo->u4PrepareWorkBuffer, prPlaneInfo->u4PrepareWidth, prPlaneInfo->u4PrepareHeight, prPlaneInfo->u4PrepareColorMode,
	    prPlaneInfo->u4RgnWorkBuffer, prPlaneInfo->u4RgnWidth, prPlaneInfo->u4RgnHeight, prPlaneInfo->u4RgnColorMode);;
	OSD_PLA_Enable(u4Plane, 1);

	Printf("Start.............\n press any key to continue ! \n");
	SerPollGetChar();

    for(k = 0; k < 2; k++)
    	{
		for(l = 0; l < 2; l++)
			{
	for(i = 0; i < 4; i++)
	{
	    for(j = 0; j < 2; j++)
	    {
	        OSD_PLA_Enable(u4Plane, 0);
			if(j == 1)
			{
			    u4SrcPitch = GetLineSeperateCompressPitch(u4SrcPitch, k, l);
			}
			else
			{
			    u4SrcPitch = u4OriginalPitch;
			}
			OSD_RGN_Set(prPlaneInfo->u4Region, OSD_RGN_BMP_PITCH, u4SrcPitch);
			OSD_RGN_Set(prPlaneInfo->u4Region, OSD_RGN_DEC_MODE, i);
			OSD_RGN_Set(prPlaneInfo->u4Region, OSD_RGN_DEC_LINE, j);
			OSD_PLA_Enable(u4Plane, 1);
	        OSD_GFX_Compress(prPlaneInfo->u4PrepareWorkBuffer, prPlaneInfo->u4PrepareColorMode, u4OriginalPitch, prPlaneInfo->u4RgnWorkBuffer,
		    prPlaneInfo->u4RgnWidth, prPlaneInfo->u4RgnHeight, i, j, l, k);

			Printf("dec=1, mode=%d, line=%d burst-read=%d rollbacken=%d \n press any key to continue ! \n", i, j, k, l);
			SerPollGetChar();
	    }
	}
			}
    	}

    OSD_PLA_Enable(u4Plane, 0);
	OSD_RGN_Set(prPlaneInfo->u4Region, OSD_RGN_DEC_EN, 0);
	OSD_RGN_Set(prPlaneInfo->u4Region, OSD_RGN_DEC_MODE, 0);
	OSD_RGN_Set(prPlaneInfo->u4Region, OSD_RGN_DEC_LINE, 0);

	return 0;
}


static INT32 OSD_PLA_Verify_MixSel(UINT32 u4Plane)
{
    INT32 i;
    OSD_VERIFY_PLANE_INFO_T *prPlaneInfo;
    prPlaneInfo = OSD_Get_Verify_Plane(u4Plane);
    ASSERT((prPlaneInfo != NULL));

	Printf("Start..Test MixSel \n press any key to continue ! \n");
	SerPollGetChar();

    OSD_PLA_Enable(u4Plane, 1);
    for(i = 1; i < 4; i++)
    {
        OSD_RGN_Set(prPlaneInfo->u4Region, OSD_RGN_MIX_SEL, i);
	    Printf("MixSel=%d \n press any key to continue ! \n", i);
	    SerPollGetChar();		
    }
	
    Printf("Test Finish!!!! \n");
	OSD_PLA_Enable(u4Plane, 0);

	return 0;
}


static INT32 OSD_PLA_Verify_ColorKey(UINT32 u4Plane)
{
    INT32 i, j, k;
    OSD_VERIFY_PLANE_INFO_T *prPlaneInfo;
    prPlaneInfo = OSD_Get_Verify_Plane(u4Plane);
    ASSERT((prPlaneInfo != NULL));

	Printf("Start..Test ColorKey \n press any key to continue ! \n");
	SerPollGetChar();

	OSD_RGN_Set(prPlaneInfo->u4Region, OSD_RGN_COLOR_KEY_EN, 1);
	OSD_PLA_Enable(u4Plane, 1);

	//Add Test Code

	return 0;
}


static INT32 OSD_PLA_Verify_ColorSel(UINT32 u4Plane)
{
    INT32 i, j, k;
    OSD_VERIFY_PLANE_INFO_T *prPlaneInfo;
    prPlaneInfo = OSD_Get_Verify_Plane(u4Plane);
    ASSERT((prPlaneInfo != NULL));

    Printf("Start..Test ColorSel \n press any key to continue ! \n");
    SerPollGetChar();  

	return 0;
}

static void vOsdHeaderUpdateCallbak1(UINT16 u2Vector)
{
    UINT32 u4Status;
    UNUSED(u2Vector);

    u4Status = IO_REG32(OSD_BASE, 0x34);

    if (0 == (u4Status & 0x20000)) //DMA done
    {
        printf("[OSD]Error, no interrupt status \n");
        if (!BIM_ClearIrq(VECTOR_OSD))
        {
        }
        return;
    }
    // check OSD1 header irq status
    u4Status = IO_READ32(OSD_PLA1_REG, 0);
    if ((u4Status & 0x1000000) !=0)
    {
        printf("[OSD]Receive OSD1 header update irq \n");
        // disable irq
        IO_WRITE32(OSD_PLA1_REG, 0, (IO_READ32(OSD_PLA1_REG, 0) & 0xffdfffff));
        // clea irq
        IO_WRITE32(OSD_PLA1_REG, 0, (IO_READ32(OSD_PLA1_REG, 0) | 0x400000));
    }

    // check OSD2 header irq status
    u4Status = IO_READ32(OSD_PLA2_REG, 0);
    if ((u4Status & 0x1000000) !=0)
    {
        printf("[OSD]Receive OSD2 header update irq \n");
        // disable irq
        IO_WRITE32(OSD_PLA2_REG, 0, (IO_READ32(OSD_PLA2_REG, 0) & 0xffdfffff));
        // clea irq
        IO_WRITE32(OSD_PLA2_REG, 0, (IO_READ32(OSD_PLA2_REG, 0) | 0x400000));
    }

    // check OSD3 header irq status
    u4Status = IO_READ32(OSD_PLA3_REG, 0);
    if ((u4Status & 0x1000000) !=0)
    {
        printf("[OSD]Receive OSD3 header update irq \n");
        // disable irq
        IO_WRITE32(OSD_PLA3_REG, 0, (IO_READ32(OSD_PLA3_REG, 0) & 0xffdfffff));
        // clea irq
        IO_WRITE32(OSD_PLA3_REG, 0, (IO_READ32(OSD_PLA3_REG, 0) | 0x400000));
    }
    // clear global isr
    if (!BIM_ClearIrq(VECTOR_OSD))
    {
    }
    // temp solution , disable all plane irq
    // disable irq
    #if 0
    IO_WRITE32(OSD_PLA1_REG, 0, (IO_READ32(OSD_PLA1_REG, 0) & 0xffdfffff));
        // clea irq
    IO_WRITE32(OSD_PLA1_REG, 0, (IO_READ32(OSD_PLA1_REG, 0) | 0x400000));

    IO_WRITE32(OSD_PLA2_REG, 0, (IO_READ32(OSD_PLA2_REG, 0) & 0xffdfffff));
        // clea irq
    IO_WRITE32(OSD_PLA2_REG, 0, (IO_READ32(OSD_PLA2_REG, 0) | 0x400000));

    IO_WRITE32(OSD_PLA3_REG, 0, (IO_READ32(OSD_PLA3_REG, 0) & 0xffdfffff));
        // clea irq
    IO_WRITE32(OSD_PLA3_REG, 0, (IO_READ32(OSD_PLA3_REG, 0) | 0x400000));
    #endif
    return;
}

static void vOsdHeaderUpdateCallbak2(UINT16 u2Vector)
{
    UINT32 u4Status;
    UNUSED(u2Vector);

    u4Status = IO_REG32(OSD_BASE, 0x34);

    if (0 == (u4Status & 0x40000)) //DMA done
    {
        printf("[OSD]Error, no interrupt status \n");
        if (!BIM_ClearIrq(VECTOR_OSD_INT2))
        {
        }
        //return;
    }
    // check OSD1 header irq status
    u4Status = IO_READ32(OSD_PLA1_REG, 0);
    if ((u4Status & 0x1000000) !=0)
    {
        printf("[OSD]Receive OSD1 header update irq \n");
        // disable irq
        IO_WRITE32(OSD_PLA1_REG, 0, (IO_READ32(OSD_PLA1_REG, 0) & 0xffdfffff));
        // clea irq
        IO_WRITE32(OSD_PLA1_REG, 0, (IO_READ32(OSD_PLA1_REG, 0) | 0x400000));
    }

    // check OSD2 header irq status
    u4Status = IO_READ32(OSD_PLA2_REG, 0);
    if ((u4Status & 0x1000000) !=0)
    {
        printf("[OSD]Receive OSD2 header update irq \n");
        // disable irq
        IO_WRITE32(OSD_PLA2_REG, 0, (IO_READ32(OSD_PLA2_REG, 0) & 0xffdfffff));
        // clea irq
        IO_WRITE32(OSD_PLA2_REG, 0, (IO_READ32(OSD_PLA2_REG, 0) | 0x400000));
    }

    // check OSD3 header irq status
    u4Status = IO_READ32(OSD_PLA3_REG, 0);
    if ((u4Status & 0x1000000) !=0)
    {
        printf("[OSD]Receive OSD3 header update irq \n");
        // disable irq
        IO_WRITE32(OSD_PLA3_REG, 0, (IO_READ32(OSD_PLA3_REG, 0) & 0xffdfffff));
        // clea irq
        IO_WRITE32(OSD_PLA3_REG, 0, (IO_READ32(OSD_PLA3_REG, 0) | 0x400000));
    }
    // clear global isr
    if (!BIM_ClearIrq(VECTOR_OSD_INT2))
    {
    }
    // temp solution , disable all plane irq
    // disable irq
    #if 0
    IO_WRITE32(OSD_PLA1_REG, 0, (IO_READ32(OSD_PLA1_REG, 0) & 0xffdfffff));
        // clea irq
    IO_WRITE32(OSD_PLA1_REG, 0, (IO_READ32(OSD_PLA1_REG, 0) | 0x400000));

    IO_WRITE32(OSD_PLA2_REG, 0, (IO_READ32(OSD_PLA2_REG, 0) & 0xffdfffff));
        // clea irq
    IO_WRITE32(OSD_PLA2_REG, 0, (IO_READ32(OSD_PLA2_REG, 0) | 0x400000));

    IO_WRITE32(OSD_PLA3_REG, 0, (IO_READ32(OSD_PLA3_REG, 0) & 0xffdfffff));
        // clea irq
    IO_WRITE32(OSD_PLA3_REG, 0, (IO_READ32(OSD_PLA3_REG, 0) | 0x400000));
    #endif
    return;
}


static void vOsdHeaderUpdateCallbak3(UINT16 u2Vector)
{
    UINT32 u4Status;
    UNUSED(u2Vector);

    u4Status = IO_REG32(OSD_BASE, 0x34);

    if (0 == (u4Status & 0x80000)) //DMA done
    {
        printf("[OSD]Error, no interrupt status \n");
        if (!BIM_ClearIrq(VECTOR_OSD_INT3))
        {
        }
        //return;
    }
    // check OSD1 header irq status
    u4Status = IO_READ32(OSD_PLA1_REG, 0);
    if ((u4Status & 0x1000000) !=0)
    {
        printf("[OSD]Receive OSD1 header update irq \n");
        // disable irq
        IO_WRITE32(OSD_PLA1_REG, 0, (IO_READ32(OSD_PLA1_REG, 0) & 0xffdfffff));
        // clea irq
        IO_WRITE32(OSD_PLA1_REG, 0, (IO_READ32(OSD_PLA1_REG, 0) | 0x400000));
    }

    // check OSD2 header irq status
    u4Status = IO_READ32(OSD_PLA2_REG, 0);
    if ((u4Status & 0x1000000) !=0)
    {
        printf("[OSD]Receive OSD2 header update irq \n");
        // disable irq
        IO_WRITE32(OSD_PLA2_REG, 0, (IO_READ32(OSD_PLA2_REG, 0) & 0xffdfffff));
        // clea irq
        IO_WRITE32(OSD_PLA2_REG, 0, (IO_READ32(OSD_PLA2_REG, 0) | 0x400000));
    }

    // check OSD3 header irq status
    u4Status = IO_READ32(OSD_PLA3_REG, 0);
    if ((u4Status & 0x1000000) !=0)
    {
        printf("[OSD]Receive OSD3 header update irq \n");
        // disable irq
        IO_WRITE32(OSD_PLA3_REG, 0, (IO_READ32(OSD_PLA3_REG, 0) & 0xffdfffff));
        // clea irq
        IO_WRITE32(OSD_PLA3_REG, 0, (IO_READ32(OSD_PLA3_REG, 0) | 0x400000));
    }
    // clear global isr
    if (!BIM_ClearIrq(VECTOR_OSD_INT3))
    {
    }
    // temp solution , disable all plane irq
    // disable irq
    #if 0
    IO_WRITE32(OSD_PLA1_REG, 0, (IO_READ32(OSD_PLA1_REG, 0) & 0xffdfffff));
        // clea irq
    IO_WRITE32(OSD_PLA1_REG, 0, (IO_READ32(OSD_PLA1_REG, 0) | 0x400000));

    IO_WRITE32(OSD_PLA2_REG, 0, (IO_READ32(OSD_PLA2_REG, 0) & 0xffdfffff));
        // clea irq
    IO_WRITE32(OSD_PLA2_REG, 0, (IO_READ32(OSD_PLA2_REG, 0) | 0x400000));

    IO_WRITE32(OSD_PLA3_REG, 0, (IO_READ32(OSD_PLA3_REG, 0) & 0xffdfffff));
        // clea irq
    IO_WRITE32(OSD_PLA3_REG, 0, (IO_READ32(OSD_PLA3_REG, 0) | 0x400000));
    #endif
    return;
}


static void OSD_PLA_1_Interrupt(UINT16 u2Vector)
{
    Printf("%s\n", __FUNCTION__);
	vOsdHeaderUpdateCallbak1(u2Vector);
}

static void OSD_PLA_2_Interrupt(UINT16 u2Vector)
{
    Printf("%s\n", __FUNCTION__);
	vOsdHeaderUpdateCallbak2(u2Vector);
}

static void OSD_PLA_3_Interrupt(UINT16 u2Vector)
{
    Printf("%s\n", __FUNCTION__);
	vOsdHeaderUpdateCallbak3(u2Vector);
}

#if defined(CC_MT5399) || defined(CC_MT5890) || defined(CC_MT5882)
static x_os_isr_fct pfnOldIsr1;
static x_os_isr_fct pfnOldIsr2;
static x_os_isr_fct pfnOldIsr3;
#endif

static INT32 OSD_PLA_Verify_Interrupt(INT32 i4Argc, const CHAR ** szArgv)
{
#if defined(CC_MT5399) || defined(CC_MT5890) || defined(CC_MT5882)
    UINT32 u4Plane;

    GET_PLANE(u4Plane);

    if(u4Plane == 0)
    {
        VERIFY(x_reg_isr(VECTOR_OSD, OSD_PLA_1_Interrupt, &pfnOldIsr1) == OSR_OK);
    }
	else if(u4Plane == 1)
	{
	    VERIFY(x_reg_isr(VECTOR_OSD_INT2, OSD_PLA_2_Interrupt, &pfnOldIsr2) == OSR_OK);
	}
	else if(u4Plane == 2)
	{
	     VERIFY(x_reg_isr(VECTOR_OSD_INT3, OSD_PLA_3_Interrupt, &pfnOldIsr3) == OSR_OK);
	}
#endif 

    return 0;
}


static OSD_VERIFY_FUNC_T arVerifyPlaneFunc[] = {
	{
		.ucDesc = "Test Compression",
		.pfnVerifyFunc = OSD_PLA_Verify_Decompress,
	}
};


static INT32 OSD_Verify_Prepare(INT32 i4Argc, const CHAR ** szArgv)
{
     UINT32 u4Width, u4Height, u4ColorMode, u4Addr;
	 OSD_VERIFY_INFO_T *prInfo;
	 
     u4Width = StrToInt(szArgv[1]);
     u4Height = StrToInt(szArgv[2]);
     u4ColorMode = StrToInt(szArgv[3]);

	 prInfo = &rVerifyInfo;

    if(prInfo->u4PrepareAllocBuffer == 0 || prInfo->u4PrepareWidth != u4Width || prInfo->u4PrepareHeight != u4Height 
		|| prInfo->u4PrepareColorMode != u4ColorMode)
    {
        u4Addr = (UINT32)BSP_AllocDmaMemory(OSD_DMA_ALIGN(u4Width * u4Height * 4)+ OSD_DMA_ALIGN_SIZE + OSD_PALETTE_SIZE);
        if(prInfo->u4PrepareAllocBuffer != 0)
        {
            BSP_FreeDmaMemory(prInfo->u4PrepareAllocBuffer);
        }
		prInfo->u4PrepareAllocBuffer = u4Addr;
		u4Addr = (u4Addr + 0xF) & 0xfffffff0;
		prInfo->u4PrepareWorkBuffer = u4Addr;
    }
	
	prInfo->u4PrepareWidth = u4Width;
	prInfo->u4PrepareHeight = u4Height;
	prInfo->u4PrepareColorMode = u4ColorMode;

	Printf("u4PrepareWorkBuffer = 0x%x \n", prInfo->u4PrepareWorkBuffer);
	
	return 0;
}

static INT32 OSD_Verify_Release(INT32 i4Argc, const CHAR ** szArgv)
{
	OSD_VERIFY_INFO_T *prInfo;
	
	prInfo = &rVerifyInfo;

    if(prInfo->u4PrepareAllocBuffer != 0)
    {
	    BSP_FreeDmaMemory(prInfo->u4PrepareAllocBuffer);
    }

	prInfo->u4PrepareAllocBuffer = 0;

	return 0;
}


static INT32 OSD_Verify_Plane_Prepare(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Plane, u4Width, u4Height, u4ColorMode, u4Addr;
    OSD_VERIFY_PLANE_INFO_T *prPlaneInfo;
	CHK_NUM_MIN_PARAM(4);

	GET_PLANE(u4Plane);
	u4Width = StrToInt(szArgv[2]);
	u4Height = StrToInt(szArgv[3]);
	u4ColorMode = StrToInt(szArgv[4]);

	prPlaneInfo = OSD_Get_Verify_Plane(u4Plane);
	ASSERT((prPlaneInfo != NULL));

    if(prPlaneInfo->u4PrepareAllocBuffer == 0 || prPlaneInfo->u4PrepareWidth != u4Width || prPlaneInfo->u4PrepareHeight != u4Height 
		|| prPlaneInfo->u4PrepareColorMode != u4ColorMode)
    {
        u4Addr = (UINT32)BSP_AllocDmaMemory(OSD_DMA_ALIGN(u4Width * u4Height * 4)+ OSD_DMA_ALIGN_SIZE + OSD_PALETTE_SIZE);
        if(prPlaneInfo->u4PrepareAllocBuffer != 0)
        {
            BSP_FreeDmaMemory(prPlaneInfo->u4PrepareAllocBuffer);
        }
		prPlaneInfo->u4PrepareAllocBuffer = u4Addr;
		u4Addr = (u4Addr + 0xF) & 0xfffffff0;
		prPlaneInfo->u4PrepareWorkBuffer = u4Addr;
    }
	
	prPlaneInfo->u4PrepareWidth = u4Width;
	prPlaneInfo->u4PrepareHeight = u4Height;
	prPlaneInfo->u4PrepareColorMode = u4ColorMode;
	
	return 0;
}


static INT32 OSD_Verify_Plane_Release(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 u4Plane;
	OSD_VERIFY_PLANE_INFO_T *prPlaneInfo;
	CHK_NUM_MIN_PARAM(2);

	GET_PLANE(u4Plane);
	
	prPlaneInfo = OSD_Get_Verify_Plane(u4Plane);
	ASSERT((prPlaneInfo != NULL));

    if(prPlaneInfo->u4PrepareAllocBuffer != 0)
    {
	    BSP_FreeDmaMemory(prPlaneInfo->u4PrepareAllocBuffer);
    }

	prPlaneInfo->u4PrepareAllocBuffer = 0;

	return 0;
}


static INT32 OSD_Verify_Create(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Plane, u4Width, u4Height, u4ColorMode, u4Addr, u4Region, u4RgnList, u4ShutterRgnAddr, u4PanelWidth, u4PanelHeight;
    OSD_VERIFY_PLANE_INFO_T *prPlaneInfo;
	CHK_NUM_MIN_PARAM(4);

	GET_PLANE(u4Plane);
	u4Width = StrToInt(szArgv[2]);
	u4Height = StrToInt(szArgv[3]);
	u4ColorMode = StrToInt(szArgv[4]);

	u4PanelWidth = PANEL_GetPanelWidth();
	u4PanelHeight = PANEL_GetPanelHeight();

	prPlaneInfo = OSD_Get_Verify_Plane(u4Plane);
	ASSERT((prPlaneInfo != NULL));
	//ASSERT((prPlaneInfo->u4PrepareAllocBuffer != 0));

    if(prPlaneInfo->u4RgnAllocBuffer == 0 || prPlaneInfo->u4RgnWidth != u4Width || prPlaneInfo->u4RgnHeight != u4Height 
		|| prPlaneInfo->u4RgnColorMode != u4ColorMode)
    {
        u4Addr = (UINT32)BSP_AllocDmaMemory(OSD_DMA_ALIGN(u4Width * u4Height * 4)+ OSD_DMA_ALIGN_SIZE + OSD_PALETTE_SIZE);
        if(prPlaneInfo->u4RgnAllocBuffer != 0)
        {
            BSP_FreeDmaMemory(prPlaneInfo->u4RgnAllocBuffer);
        }
		prPlaneInfo->u4RgnAllocBuffer = u4Addr;
		u4Addr = (u4Addr + 0xF) & 0xfffffff0;
		prPlaneInfo->u4RgnWorkBuffer = u4Addr;
    }

	prPlaneInfo->u4RgnWidth = u4Width;
	prPlaneInfo->u4RgnHeight = u4Height;
	prPlaneInfo->u4RgnColorMode = u4ColorMode;

	if(prPlaneInfo->u4Region == (UINT32) -1)
	{
	    OSD_PLA_Enable(u4Plane, 0);
	    OSD_RGN_Create(&u4Region, prPlaneInfo->u4RgnWidth, prPlaneInfo->u4RgnHeight, (void*)prPlaneInfo->u4RgnWorkBuffer,
			prPlaneInfo->u4RgnColorMode, 0, 0, 0, prPlaneInfo->u4RgnWidth, prPlaneInfo->u4RgnHeight);
		OSD_RGN_Set(u4Region, OSD_RGN_MIX_SEL, 1);
		OSD_RGN_Set(u4Region, OSD_RGN_BIG_ENDIAN, 1);
		OSD_RGN_LIST_Create(&u4RgnList);
		OSD_RGN_Insert(u4Region, u4RgnList);
		OSD_PLA_FlipTo(u4Plane, u4RgnList);
		if(u4Plane != OSD_PLANE_4 && u4Plane != OSD_PLANE_5)
		{
		    OSD_SC_Scale(prPlaneInfo->u4Scaler, 1, u4PanelWidth, u4PanelHeight, u4PanelWidth, u4PanelHeight);
		}
		OSD_PLA_SetFading(u4Plane, 255);

		prPlaneInfo->u4Region = u4Region;
		prPlaneInfo->u4RgnList = u4RgnList;

		OSD_RGN_Create(&u4Region, prPlaneInfo->u4RgnWidth, prPlaneInfo->u4RgnHeight, (void*)prPlaneInfo->u4RgnWorkBuffer,
			prPlaneInfo->u4RgnColorMode, 0, 0, 0, prPlaneInfo->u4RgnWidth, prPlaneInfo->u4RgnHeight);
		OSD_RGN_Set(u4Region, OSD_RGN_MIX_SEL, 1);
		OSD_RGN_Set(u4Region, OSD_RGN_BIG_ENDIAN, 1);
		prPlaneInfo->u4ShutterRgn = u4Region;
		_OSD_RGN_GetAddress(u4Region, &u4ShutterRgnAddr);
		_OSD_PLA_SetRightHeaderAddr(u4Plane, u4ShutterRgnAddr);
	}
	else
	{
	    OSD_PLA_FlipTo(u4Plane, prPlaneInfo->u4RgnList);
		_OSD_RGN_GetAddress(prPlaneInfo->u4ShutterRgn, &u4ShutterRgnAddr);
		_OSD_PLA_SetRightHeaderAddr(u4Plane, u4ShutterRgnAddr);
	}

	return 0;
}



static INT32 OSD_Verify_Plane_Help(VOID)
{
    int i = 0, i4FuncNum = 0;
	
    i4FuncNum = sizeof(arVerifyPlaneFunc) / sizeof(OSD_VERIFY_FUNC_T);

	Printf("Please Select Verify OSD Plane Index \n");
	for(i = 0; i < i4FuncNum; i++)
	{
	    Printf("[%d] %s \n", i, arVerifyPlaneFunc[i].ucDesc);
	}

	Printf("[q/Q] Quit \n");
	Printf("[a/A] Test All \n");

	return 0;
}


static INT32 OSD_Do_Plane_Verify(INT32 i4Argc, const CHAR ** szArgv)
{
     INT32 i, i4Index = -1, i4FuncNum = 0;
     UINT32 u4Plane;
	 OSD_VERIFY_PLANE_INFO_T *prPlaneInfo;
	 CHK_NUM_MIN_PARAM(1);

	 GET_PLANE(u4Plane);
	 prPlaneInfo = OSD_Get_Verify_Plane(u4Plane);
	 ASSERT((prPlaneInfo != NULL));
	 ASSERT((prPlaneInfo->u4PrepareAllocBuffer != 0));
	 ASSERT((prPlaneInfo->u4Region != (UINT32) -1));

     if(i4Argc > 2)
     {
         i4Index = StrToInt(szArgv[2]);
     }

	 i4FuncNum = sizeof(arVerifyPlaneFunc) / sizeof(OSD_VERIFY_FUNC_T);

	 if(i4Index >= 0 && i4Index < i4FuncNum)
	 {
	     arVerifyPlaneFunc[i4Index].pfnVerifyFunc(u4Plane);
	 }
	 else
	 {
	     UINT8 u8Char;
	     OSD_Verify_Plane_Help();
	     u8Char = (INT32)SerPollGetChar();
		 i4Index = u8Char - '0';

		 if(i4Index >= 0 && i4Index < i4FuncNum)
		 {
		     arVerifyPlaneFunc[i4Index].pfnVerifyFunc(u4Plane);
		 }
         else if(u8Char == 'Q' || u8Char == 'q')
         {
             return 0;
         }
		 else if(u8Char == 'A' || u8Char == 'a')
		 {
	         for(i = 0; i < i4FuncNum; i++)
	         {
	             UINT8 ucChar;
				 Printf("Start Test %s \n", arVerifyPlaneFunc[i].ucDesc);
	             arVerifyPlaneFunc[i].pfnVerifyFunc(u4Plane);
                 Printf("Finish Test %s \n", arVerifyPlaneFunc[i].ucDesc);
				 Printf("[r/R] Again!!! \n");
				 ucChar = (INT32)SerPollGetChar();

				 if(ucChar == 'r' || ucChar == 'R')
				 {
				     i--;
				 }
	         }
		 }
	 }
	 return 0;
}


INT32 OSD_Verify_PMX_Prepare(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 i, i4FuncNum = 0;
	UINT32 u4StepX, u4StepY;

	i4FuncNum = sizeof(arVerifyPlane) / sizeof(OSD_VERIFY_PLANE_INFO_T);
    u4StepX = StrToInt(szArgv[1]);
	u4StepY = StrToInt(szArgv[2]);
	ASSERT((rVerifyInfo.u4PrepareAllocBuffer != 0));

	Printf("Please Make Sure load image into buffer[0x%x]....\n press any key to continue !\n", rVerifyInfo.u4PrepareWorkBuffer);
	SerPollGetChar();

	for(i = 0; i < i4FuncNum; i++)
	{
	    if(arVerifyPlane[i].u4Region != (UINT32) -1)
	    {
	        OSD_Verify_Blit(rVerifyInfo.u4PrepareWorkBuffer, rVerifyInfo.u4PrepareWidth, rVerifyInfo.u4PrepareHeight, rVerifyInfo.u4PrepareColorMode,
				arVerifyPlane[i].u4RgnWorkBuffer, arVerifyPlane[i].u4RgnWidth, arVerifyPlane[i].u4RgnHeight, arVerifyPlane[i].u4RgnColorMode);

			OSD_RGN_Set(arVerifyPlane[i].u4Region, OSD_RGN_MIX_SEL, 3);

			OSD_RGN_Set(arVerifyPlane[i].u4Region, OSD_RGN_POS_X, i * u4StepX);
			OSD_RGN_Set(arVerifyPlane[i].u4Region, OSD_RGN_POS_Y, i * u4StepY);

			OSD_PLA_Enable(arVerifyPlane[i].u4Plane, 1);
	    }
	}

	return 0;
}

INT32 OSD_Verify_LVDS_Mode3_Test(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 i, j;
    UINT32 *pu4Addr;
	UINT32 u4Pitch, u4Bpp;
    OSD_VERIFY_PLANE_INFO_T *prPlane2Info;
	OSD_VERIFY_PLANE_INFO_T *prPlane3Info;
	OSD_VERIFY_PLANE_INFO_T *prPlaneInfo;

    prPlane2Info = OSD_Get_Verify_Plane(OSD_PLANE_2);
	prPlane3Info = OSD_Get_Verify_Plane(OSD_PLANE_3);
    ASSERT((prPlane2Info != NULL));
	ASSERT((prPlane3Info != NULL));
	ASSERT((prPlane2Info->u4RgnColorMode == OSD_CM_ARGB8888_DIRECT32));
	ASSERT((prPlane3Info->u4RgnColorMode == OSD_CM_ARGB8888_DIRECT32));

	prPlaneInfo = prPlane2Info;
	u4Bpp =  GFX_Get_Bpp(prPlaneInfo->u4RgnColorMode);
    u4Pitch = (UINT32) (prPlaneInfo->u4RgnWidth * u4Bpp + 0xf) & 0xfffffff0;
	for(i = 0; i < prPlaneInfo->u4RgnHeight; i++)
	{
	    for(j = 0; j < prPlaneInfo->u4RgnWidth; j++)
	    {
	        pu4Addr = (UINT32 *)(prPlaneInfo->u4RgnWorkBuffer + u4Pitch * i + u4Bpp * j);
			if(j % 2 == 0)
			{
	            *pu4Addr = 0x0000FFFF;
			}
			else
			{
			    *pu4Addr = 0x00000000;
			}
	    }
	}

    prPlaneInfo = prPlane3Info;
	u4Bpp =  GFX_Get_Bpp(prPlaneInfo->u4RgnColorMode);
    u4Pitch = (UINT32) (prPlaneInfo->u4RgnWidth * u4Bpp + 0xf) & 0xfffffff0;
	for(i = 0; i < prPlaneInfo->u4RgnHeight; i++)
	{
	    for(j = 0; j < prPlaneInfo->u4RgnWidth; j++)
	    {
	        pu4Addr = (UINT32 *)(prPlaneInfo->u4RgnWorkBuffer + u4Pitch * i + u4Bpp * j);
			if(j % 2 == 0)
			{
	            *pu4Addr = 0xFF0000FF;
			}
			else
			{
			    *pu4Addr = 0x00000000;
			}
	    }
	}

	GFX_Flush_Invalid_DCache();

	OSD_PLA_Enable(OSD_PLANE_2, 1);
	OSD_PLA_Enable(OSD_PLANE_3, 1);

	return 0;
}

INT32 OSD_Verify_PixelClock_Test(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4HTotal, u4VTotal, u4FrameRate, u4PixelClock;
    u4FrameRate = StrToInt(szArgv[1]);
	u4HTotal = StrToInt(szArgv[2]);
	u4VTotal = StrToInt(szArgv[3]);
	u4PixelClock = u4HTotal * u4VTotal * u4FrameRate;

	CLI_Parser_Arg("pmx.s.p %d 1920 1080 %d %d 0 0 0 0 %d", u4PixelClock, u4HTotal, u4VTotal, u4FrameRate);

	Printf("Test PixelClock [%d * %d * %d] = %d\n", u4HTotal, u4VTotal, u4FrameRate, u4PixelClock);

	return 0;
}


INT32 OSD_Verify_PMX(INT32 i4Argc, const CHAR ** szArgv)
{
    OSD_Verify_Prepare(i4Argc - 1, szArgv + 1);
}



static CLI_EXEC_T _arOsdVerifyCmdTbl[] =
{
    {
        "prepare", "pre", OSD_Verify_Prepare, NULL,
        "Prepare Verify{ width, height, colormode}", CLI_GUEST
    },
    {
        "release", "rel", OSD_Verify_Release, NULL,
        "OSD_Verify_Release{}", CLI_GUEST
    },
    {
        "prepare_plane", "pre_pla", OSD_Verify_Plane_Prepare, NULL,
        "prepare_plane{plane#, width, height, colormode}", CLI_GUEST
    },
    {
        "release_plane", "rel_pla", OSD_Verify_Plane_Release, NULL,
        "release_plane{plane#}", CLI_GUEST
    },
    {
        "create_plane", "c_pla", OSD_Verify_Create, NULL,
        "create_plane{plane#, width, height, colormode}", CLI_SUPERVISOR
    },
    {
        "do_plane_verify", "do_pla_vfy", OSD_Do_Plane_Verify, NULL,
        "do_plane_verify{plane, index}", CLI_SUPERVISOR
    },    
    {
        "pmx_prepare", "pmx_pre", OSD_Verify_PMX_Prepare, NULL,
        "pmx_prepare{stepx, stepy}", CLI_SUPERVISOR
    },
    {
        "osd2lvds_mode3", "osd2lvds_mode3", OSD_Verify_LVDS_Mode3_Test, NULL,
        "osd2lvds_mode3{}", CLI_SUPERVISOR
    },    
    {
        "pmx_s_p", "pmx_s_p", OSD_Verify_PixelClock_Test, NULL,
        "pmx_s_p{}", CLI_SUPERVISOR
    },
	{
		"interrupt", "int", OSD_PLA_Verify_Interrupt, NULL,
		"interrupt Verify{ plane}", CLI_GUEST
	},
    {NULL, NULL, NULL, NULL, NULL, CLI_GUEST}
};

#endif
//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------

static CLI_EXEC_T _arOsdPlaneCmdTbl[] =
{
    {
        "reset", "res", _OsdPlaneCmdReset, NULL,
        "Reset plane", CLI_GUEST
    },
    {
        "enable", "en", _OsdPlaneCmdEnable, NULL,
        "Set/get enable flag {plane#, on/off}", CLI_GUEST
    },
    {
        "cmpress", "cmp", _OsdCmdCmpressEnable, NULL,
        "osd compress enable flag {plane#, on/off}", CLI_GUEST
    },
    {
        "sbs", "sbs", _OsdCmdSbsEnable, NULL,
        "osd sbs enable flag {plane#, on/off}", CLI_GUEST
    },
    {
        "3ds", "3ds", _OsdCmd3DEnable, NULL,
        "osd 3d enable flag {plane#, on/off}", CLI_SUPERVISOR
    },
    {
        "borrow", "cbb", _OsdCmdCmpressBorrowBuf, NULL,
        "borrow osd compress buffer {plane,plane from}", CLI_SUPERVISOR
    },
    {
        "pattern", "pt", _OsdPlaneCmdPattern, NULL,
        "Show Plane pattern{Pid, Enable,W,H,CM,x,y}", CLI_SUPERVISOR
    },
    {
        "flip", "flp", _OsdPlaneCmdFlip, NULL,
        "Flip to region list {plane#, list#}", CLI_GUEST
    },
    {
        "alpha", "afa", _OsdPlaneCmdAlpha, NULL,
        "Set/get alpha value {0~255}", CLI_GUEST
    },
    {
        "fading", "fa", _OsdPlaneCmdFading, NULL,
        "Set/get fading value {0~255}", CLI_GUEST
    },
    {
        "hfilter", "hf", _OsdPlaneCmdHFilter, NULL,
        "Enable HFilter {plane#, on/off}", CLI_SUPERVISOR
    },
    {
        "vfilter", "vf", _OsdPlaneCmdVFilter, NULL,
        "Enable VFilter {plane#, on/off}", CLI_SUPERVISOR
    },
    {
        "fifo", "ff", _OsdPlaneCmdFifo, NULL,
        "Configure fifo {plane#, fast-req, ex-vacancy-thr, vacancy-thr, "
        "full-thr}", CLI_SUPERVISOR
    },
    {
        "reg", "reg", _OsdPlaneCmdDumpReg, NULL,
        "Dump plane register", CLI_GUEST
    },
    {
        "compress", "com", _OsdPlaneCmdCompress, NULL,
        "enter compression mode {plane#, enable[0/1], alpha-mode[0/1]}", CLI_SUPERVISOR
    },
    {
        "dump", "dump", _OsdPlaneCmdDumpInfo, NULL,
        "Dump plane detail", CLI_GUEST
    },
     {
        "sonypattern", "sp", _OsdSonyOSDPattern, NULL,
        "Dump plane detail", CLI_SUPERVISOR
    },
    {
        "planeorder", "po", _OsdSonyOSDOrder, NULL,
        "Dump plane detail", CLI_SUPERVISOR
    },
    {NULL, NULL, NULL, NULL, NULL, CLI_GUEST}
};

static CLI_EXEC_T _arOsdListCmdTbl[] =
{
    {
        "create", "cr", _OsdListCmdCreate, NULL,
        "Create region list", CLI_GUEST
    },
    {
        "list", "ls", _OsdListCmdList, NULL,
        "List region list", CLI_GUEST
    },
    {
        "dump", "dump", _OsdListCmdDump, NULL,
        "Dump all in list {list#}", CLI_GUEST
    },
    {
        "delete", "del", _OsdListCmdDelete, NULL,
        "Delete region list {list#}", CLI_GUEST
    },
    {
        "detach", "det", _OsdListCmdDetach, NULL,
        "Detach region(s) from list {list#} {(rgn# | all)}", CLI_GUEST
    },
    {NULL, NULL, NULL, NULL, NULL, CLI_GUEST}
};

static CLI_EXEC_T _arOsdRegionCmdTbl[] =
{
    {
        "help", "h", _OsdRgnCmdHelp, NULL,
        "Display manual pages", CLI_GUEST
    },
    {
        "new", "n", _OsdRgnCmdNew, NULL,
        "Allocate new region", CLI_GUEST
    },
    {
        "insert", "ins", _OsdRgnCmdInsert, NULL,
        "Insert region into list {rgn#, list#}", CLI_GUEST
    },
    {
        "delete", "del", _OsdRgnCmdDelete, NULL,
        "Delete region from list {rgn#}", CLI_GUEST
    },
    {
        "set", NULL, _OsdRgnCmdSet, NULL,
        "Set region property {rgn#, attr, value}", CLI_GUEST
    },
    {
        "pattern", "pt", _OsdRgnCmdPattern, NULL,
        "Draw pattern {region#, mode(0:V line, 1:H line, 2:border, "
        "3:colorbar)}", CLI_GUEST
    },
    {NULL, NULL, NULL, NULL, NULL, CLI_GUEST}
};

static CLI_EXEC_T _arOsdScalerCmdTbl[] =
{
    {
        "scale", "s", _OsdScCmdScale, NULL,
        "Scale OSD plane {scaler#, src_w, src_h, dst_w, dst_h}", CLI_GUEST
    },
    {
        "setlpf", "l", _OsdScCmdSetLpf, NULL,
        "Set low pass filter {scaler#, en}", CLI_SUPERVISOR
    },
    {NULL, NULL, NULL, NULL, NULL, CLI_GUEST}
};
//#ifdef CC_MT5392B
#if 1
static CLI_EXEC_T _arOsdGfxPortCmdTbl[] =
{
    {
        "mode", "m", _OsdGpCmdMode, NULL,
        "Switch OSD Mode: normal/DMA/GFX", CLI_SUPERVISOR
    },
    {
        "tx", NULL, _OsdGpCmdTx, NULL,
        "Trigger DMA mode {plane#}", CLI_SUPERVISOR
    },
    {
        "write", "w", _OsdGpCmdWriteReg, NULL,
        "Write 8280 Register {8280_16bit_addr, 8bit_value}", CLI_SUPERVISOR
    },
    {
        "write32", "w32", _OsdGpCmdWriteReg32bit, NULL,
        "Write 8280 Register {8280_16bit_addr, 32bit_value}", CLI_SUPERVISOR
    },
    {
        "copy", "c", _OsdGpCmdCopy, NULL,
        "DMA to 8280 {8280_28bit_addr, 5382P_28bit_addr, 28bit_size}", CLI_SUPERVISOR
    },
    {
        "set8281", "s82", _OsdGpCmdSet8281Reg, NULL,
        "set 8281 {addr, value}", CLI_SUPERVISOR
    },
    {NULL, NULL, NULL, NULL, NULL, CLI_GUEST}
};
#endif

static CLI_EXEC_T _arOsdCmdTbl[] =
{
    {
        "init", "i", _OsdCmdInit, NULL,
        "Init osd driver", CLI_GUEST
    },
#ifdef _TEST_SCROLL__    
    {
        "scroll", "roll", _OsdCmdScroll, NULL,
        "scroll", CLI_SUPERVISOR
    },
#endif    
    {
        "showplanedemo", "spd", _OsdCmdShowPattern, NULL,
        "show osd plane demo", CLI_SUPERVISOR
    },

    {
        "mode", "m", _OsdCmdMode, NULL,
        "Set osd output mode", CLI_SUPERVISOR
    },
    {
        "query", "q", _OsdCmdQuery, NULL,
        "Dump osd info", CLI_GUEST
    },
    {
        "sd", "sd", _OsdScCmdSetDbgInfo, NULL,
        "set osd dbg info", CLI_SUPERVISOR
    },
    {
        "gd", "gd", _OsdScCmdGetDbgInfo, NULL,
        "get osd dbg info", CLI_SUPERVISOR
    },  
#ifdef OSD_ENABLE_VERIFY
    {
        "verify", "vfy", NULL, _arOsdVerifyCmdTbl,
        "verify commands", CLI_GUEST
    },
#endif
    {
        "plane", "p", NULL, _arOsdPlaneCmdTbl,
        "Plane commands", CLI_GUEST
    },
    {
        "list", "l", NULL, _arOsdListCmdTbl,
        "Region list commands", CLI_GUEST
    },
    {
        "region", "r", NULL, _arOsdRegionCmdTbl,
        "Region commands", CLI_GUEST
    },
    {
        "scaler", "sc", NULL, _arOsdScalerCmdTbl,
        "Scaler commands", CLI_GUEST
    },
    {
        "setlvds", "sl", _OsdCmdSetLVDS3, NULL,
        "Set LVDS mode", CLI_SUPERVISOR
    },
    {
        "setpd", "pd", _OsdCmdSetPowerDwn, NULL,
        "Set power down{fgEnable}", CLI_GUEST
    },
    {
        "settve", "tve", _OsdCmdSetTVEout, NULL,
        "Set tve mode{fgEnable}", CLI_GUEST
    },
    {
        "showlogo", "logo", _OsdCmdShowLogo, NULL,
        "Show logo", CLI_GUEST
    },
#ifdef CC_MT5365
    {
        "getunderflow", "guf", _OsdCmdGetUnderflowStatus, NULL,
        "get underflow", CLI_GUEST
    },
    {
        "clearunderflow", "cuf", _OsdCmdClearUnderflowStatus, NULL,
        "clear underflow", CLI_GUEST
    },
#endif
    {
        "pattern", "pt", _OsdCmdPattern, NULL,
        "osd pattern", CLI_SUPERVISOR
    },
#ifdef CC_TDTV_3D_OSD_ENABLE
    {
        "3dsbs", "3dsbs", _OsdCmdShow3DSbs, NULL,
        "side by side 3D mode", CLI_GUEST
    },
    {
        "3dtbb", "3dtbb", _OsdCmdShow3DTbb, NULL,
        "top by bottom 3D mode", CLI_GUEST
    },
    {
        "3dshutter", "3dshutter", _OsdCmdShow3DShutter, NULL,
        "shutter 3D mode", CLI_GUEST
    },
#endif   //20101227
    {
        "3dprlineshift", "3dprls", _OsdCmdSet3DPrLineShift, NULL,
        "PR  3D mode", CLI_GUEST
    },
    {
        "isdisplaying", "isd", _OsdCmdIsDisplaying, NULL,
        "check osd is displaying now", CLI_GUEST
    },
#ifdef CC_ANDROID_3D_UI_SUPPORT
    {
        "UiMode", "uimode", _OsdCmdSetUiMode, NULL,
        "set PR 3D mode", CLI_GUEST
    },
#endif

//#ifdef CC_MT5392B
#if 1
    {
        "gp", NULL, NULL, _arOsdGfxPortCmdTbl,
        "Graphic Port commands", CLI_GUEST
    },
#endif
#ifdef OSD_TRANS_CTRL
    #if LG_OSD_PATH_TEST
    {
        "getFRCHeader", "getfh", _OsdCmdGetFRCHeader, NULL,
        "get FRC Header", CLI_GUEST
    },
    {
        " lgosdtest", "lgosd", _OsdCmdLGOSDTest, NULL,
        "lg osd path test", CLI_GUEST
    },
    #endif
#endif

#ifndef CC_5391_LOADER
    CLIMOD_DEBUG_CLIENTRY(OSD),
#endif
    {NULL, NULL, NULL, NULL, NULL, CLI_GUEST}
};

CLI_MAIN_COMMAND_ITEM(Osd)
{
    "osd", NULL, NULL, _arOsdCmdTbl,
    "Osd driver", CLI_GUEST
};

static TEXT2ENUM_T _arRegionFieldEnum[] =
{
    // alloc, prev, next, flags, pos_x, pos_y, src_w, src_h, src_addr, src_pitch
    // disp_w, disp_h, colormode, alpha, clip_v, clip_h, pal_load, pal_addr,
    // pal_len, step_v, step_h, colorkey, colorkey_en, mix_sel
    // 5391
    {"hpx", OSD_RGN_HPX_EXT},
    {"endian", OSD_RGN_ENDIAN},
    {"int_h", OSD_RGN_HINTP_MODE},
    {"int_v", OSD_RGN_VINTP_MODE},
    {"step_ho", OSD_RGN_HSTEP_OFFSET},
    {"step_vo", OSD_RGN_VSTEP_OFFSET},

    {"alloc", OSD_RGN_ALLOC},
    {"prev", OSD_RGN_PREV},
    {"next", OSD_RGN_NEXT},
    {"flags", OSD_RGN_FLAGS},
    {"colormode", OSD_RGN_COLORMODE},
    {"pos_x", OSD_RGN_POS_X},
    {"pos_y", OSD_RGN_POS_Y},
    {"out_w", OSD_RGN_OUT_W},
    {"out_h", OSD_RGN_OUT_H},
    {"src_addr", OSD_RGN_BMP_ADDR},
    {"src_pitch", OSD_RGN_BMP_PITCH},
    {"clip_h", OSD_RGN_CLIP_H},
    {"clip_v", OSD_RGN_CLIP_V},
    {"src_w", OSD_RGN_BMP_W},
    {"src_h", OSD_RGN_BMP_H},
    {"disp_w", OSD_RGN_DISP_W},
    {"disp_h", OSD_RGN_DISP_H},
    {"step_h", OSD_RGN_STEP_H},
    {"step_v", OSD_RGN_STEP_V},
    {"alpha", OSD_RGN_ALPHA},
    {"mix_sel", OSD_RGN_MIX_SEL},
    {"pal_load", OSD_RGN_PAL_LOAD},
    {"pal_addr", OSD_RGN_PAL_ADDR},
    {"pal_len", OSD_RGN_PAL_LEN},
    // note the order for strcmp()
    {"colorkey_en", OSD_RGN_COLOR_KEY_EN},
    {"colorkey", OSD_RGN_COLOR_KEY},
    {"big_endian", OSD_RGN_BIG_ENDIAN},
    {"vflip", OSD_RGN_V_FLIP},
    {"hmirror", OSD_RGN_H_MIRROR},
    {"a_sel", OSD_RGN_ALPHA_SEL},
    {"yr_sel", OSD_RGN_YR_SEL},
    {"ug_sel", OSD_RGN_UG_SEL},
    {"vb_sel", OSD_RGN_VB_SEL},
    {"dec", OSD_RGN_DEC_EN},
    {NULL, -1}
};


static UINT32 _tag_region = (UINT32)NULL;
static BOOL _tag_patternoff = FALSE;
static UINT32  _tag_Regionlist = 0xffffffff;
static UINT32 u4TopPosX = 20;
static UINT32 u4TopPosY = 20;
static UINT32 u4MidPosX = 60;
static UINT32 u4MidPosY = 60;
static UINT32 u4BotPosX = 100;
static UINT32 u4BotPosY = 100;

//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------

//-------------------------------------------------------------------------
/** _CLI_CMD
 *
 */
//-------------------------------------------------------------------------
static INT32 _CLI_CMD(const CHAR* szCmd, ...)
{
    CHAR szBuf[256];
    VA_LIST t_ap;

    VA_START(t_ap, szCmd);
   // x_vsprintf(szBuf, szCmd, t_ap);
    x_snprintf(szBuf,sizeof(szBuf),szCmd, t_ap);
    VA_END(t_ap);

    return CLI_Parser(szBuf);
}

//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
static INT32 _OsdPlaneCmdReset(INT32 i4Argc, const CHAR ** szArgv)
{
    // osd.pla.reset {plane (1,2,3)}
    INT32 i4Plane;

    CHK_NUM_MIN_PARAM(1);
    GET_PLANE(i4Plane);

    return OSD_PLA_Reset(i4Plane);
}

static INT32 _OsdScCmdSetDbgInfo(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4_idx = StrToInt(szArgv[1]);
    UINT32 u4_val = StrToInt(szArgv[2]);
    OSD_Set_Dbg_Info(u4_idx,u4_val);

    printf("_OsdScCmdSetDbgInfo[%d][0x%x]\n",u4_idx,u4_val);

    return OSD_CLI_RET_OK;
}

static INT32 _OsdScCmdGetDbgInfo(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4_val = 0x0;
    UINT32 u4_idx = StrToInt(szArgv[1]);

    u4_val = OSD_Get_Dbg_Info(u4_idx);

    printf("_OsdScCmdGetDbgInfo[%d][0x%x]\n",u4_idx,u4_val);

    return OSD_CLI_RET_OK;
}


//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
INT32 _OsdPlaneCmdEnable(INT32 i4Argc, const CHAR ** szArgv)
{
    // osd.plane.enable {plane (1,2,3)} [{enable (1,0)}]
    INT32 i4Plane;
    BOOL fgEnable;

    Printf("osd.plane.enable {plane (1,2,3)} [{enable (1,0)}]\n");
    CHK_NUM_MIN_PARAM(1);
    GET_PLANE(i4Plane);

    if (i4Argc >= 3)
    {
        // set enable flag
        switch (szArgv[2][0])
        {
            case 'e':
            case 'E':
            case 'y':
            case 'Y':
            case 't':
            case 'T':
            case '1':
                _OSD_PLA_SetEnable(i4Plane, 1);
                break;
            default:
                _OSD_PLA_SetEnable(i4Plane, 0);
        }

        _UpdatePlaneEnableState(i4Plane);

        return OSD_CLI_RET_OK;
    }
    else
    {
        OSD_PLA_UNION_T rOsdPlaneReg;
        x_memset(&rOsdPlaneReg, 0, sizeof(OSD_PLA_UNION_T)); //klocwork warning
        _OSD_PLA_GetReg(i4Plane - 1, rOsdPlaneReg.au4Reg);
        fgEnable = (BOOL)rOsdPlaneReg.rField.fgEn;
        Printf("OSD Plane %ld enable : %d\n", i4Plane + 1, fgEnable);
    }
    return OSD_CLI_RET_OK;
}

static INT32 _OsdCmdCmpressEnable(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32  i4_ret       = OSD_CLI_RET_OK;

    #ifdef LINUX_TURNKEY_SOLUTION
    UINT32 u4_plane     = 0x0;
    UINT32 u4_enable    = 0x0;

    CHK_NUM_MIN_PARAM(2);

    GET_PLANE(u4_plane);
    u4_enable = StrToInt(szArgv[2]);


    i4_ret = mtfb_compress_enable(u4_plane,u4_enable);
    #endif

    return i4_ret;
}

static INT32 _OsdCmdSbsEnable(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32  i4_ret       = OSD_CLI_RET_OK;

    #ifdef LINUX_TURNKEY_SOLUTION
    UINT32 u4_plane     = 0x0;
    UINT32 u4_enable    = 0x0;

    CHK_NUM_MIN_PARAM(2);

    GET_PLANE(u4_plane);
    u4_enable = StrToInt(szArgv[2]);

    i4_ret = OSD_SetCpblty(E_OSD_CPBLTY_SBS_PLANE,u4_plane,u4_enable);
    #endif

    return i4_ret;
}

static INT32 _OsdCmd3DEnable(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32  i4_ret       = OSD_CLI_RET_OK;

    #ifdef LINUX_TURNKEY_SOLUTION
    UINT32 u4_plane     = 0x0;
    UINT32 u4_enable    = 0x0;

    CHK_NUM_MIN_PARAM(2);

    GET_PLANE(u4_plane);
    u4_enable = StrToInt(szArgv[2]);

    i4_ret = OSD_SetCpblty(E_OSD_CPBLTY_3D_STATUS,u4_plane,u4_enable);

    i4_ret = fb_osd_reupdate_bmp_addr(u4_plane);
    #endif

    return i4_ret;
}

static INT32 _OsdCmdCmpressBorrowBuf(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32  i4_ret       = OSD_CLI_RET_OK;

    #ifdef LINUX_TURNKEY_SOLUTION
    UINT32 u4_plane     = 0x0;
    UINT32 u4_plane_cmp = 0x0;

    CHK_NUM_MIN_PARAM(2);

    GET_PLANE(u4_plane);
    u4_plane_cmp = StrToInt(szArgv[2]);
    u4_plane_cmp--;

    if(u4_plane_cmp == u4_plane)
    {
        return OSD_CLI_RET_ERR_PLANE;
    }


    i4_ret = mtfb_borrow_cmp_buf(u4_plane,u4_plane_cmp);
    #endif

    return i4_ret;
}

void _ExPatternOff(void)
{


    if(_tag_region)
    {
        OSD_RGN_Delete(_tag_region);
        printf("delete region\n");
    }
    if(_tag_Regionlist < OSD_MAX_NUM_RGN_LIST)
    {
        OSD_RGN_LIST_Delete(_tag_Regionlist);
    }

    CLI("osd.plane.enable %d 0", 3);

    _OSD_BASE_SetOSD12Ex(0);
    _tag_region = (UINT32)NULL;
    _tag_patternoff = FALSE;
    _tag_Regionlist = 0xffffffff;

    printf("<<<<<<<<<<<<<<<_ExPatternOff>>>>>>>>>>\n");
    return;
}


INT32 _ExOsdCmdPattern(INT32 i4pattern )
{
        INT32 plane = 3;
        INT32 w = 1368;
        INT32 h = 768;

        UINT32 u4Pitch = 1368*4;
        UINT32 u4BufOffset = u4Pitch * 768;
        UINT32 u4Regionlist = (UINT32) NULL;
        UINT32 u4PanelH = 1368;
        UINT32 u4PanelW = 768;
        UINT32 region = 0, sw = 0, sh = 0, cm, pos_x = 0, pos_y = 0;
        static BOOL createregion_tag = FALSE ;
        UINT32 align_buf;
        FBM_POOL_T* buf = 0;
        OSD_PTTRN_DATA_T t_pttrn_data;
        x_memset(&t_pttrn_data, 0, sizeof(OSD_PTTRN_DATA_T));

        t_pttrn_data.ui4_pttrn_id = i4pattern ;

        sw = 1368;
        sh = 768;
        u4PanelH = PANEL_GetPanelHeight();
        u4PanelW = PANEL_GetPanelWidth();

        cm = OSD_CM_ARGB8888_DIRECT32;


        //CLI_Parser("osd.plane.res %d",plane -1);
        CLI_Parser("osd.d_l -l");

        printf("buf =0x%x %-------------befor alloc\n",buf);

        printf("create buf\n");
        buf = FBM_GetPoolInfo(FBM_POOL_TYPE_JPEG_OSD2);

        if( buf == 0 )
        {
            printf("alloc false\n");

            return FALSE;
        }

        printf("buf =0x%x %-------------after alloc\n",buf);

        align_buf = OSD_DMA_ALIGN( buf->u4Addr );

        Printf("create color mode is cm= %d, w=%d, h=%d, sw=%d, sh=%d,align_buf =0x%x\n", cm,w,h,sw,sh,align_buf);
        printf("_tag_patternoff = %d \n",_tag_patternoff);
        if( !_tag_patternoff )
        {
            createregion_tag = 0;
        }

        printf("createregion_tag = %d \n",createregion_tag);
        if ( !(createregion_tag) )
        {
            OSD_RGN_Create(&region, w, h, (void*)align_buf,
                           cm, 0, 0, 0, w, h);
            _tag_region = region;

            OSD_RGN_LIST_Create( &u4Regionlist );
            _tag_Regionlist = u4Regionlist;

            printf("create region  :  0x%x \n",region);
            printf("create region list :  0x%x \n",u4Regionlist);
        }
        u4Regionlist = _tag_Regionlist;
        region = _tag_region ;
        OSD_RGN_SetBigEndian(region, 0);
        _OsdDrawPattern(region, &t_pttrn_data);
        _OSD_BASE_SetOSD12Ex(1);

        if ( !(createregion_tag) )
        {
            CLI("osd.region.insert %d %d", region, u4Regionlist);
            CLI("osd.plane.flip %d %d", plane, u4Regionlist);
        }



        printf("===============================\n");
        GFX_SetSrc((UINT8 *)align_buf, cm, u4Pitch);
        printf(">>set src: 0x%x\n",align_buf);
        GFX_SetDst((UINT8 *)(align_buf + u4BufOffset), cm, u4Pitch);
        printf(">>set dst: 0x%x\n",(align_buf + u4BufOffset));

        _CLI_CMD("gfx.set.bltopt 0 0 0");
        _CLI_CMD("gfx.op.bitblt  0 0 0 0 1368 768");
        printf(">>do bitblt\n");

        _CLI_CMD("gfx.flush");
        printf("===============================\n");

        OSD_RGN_Set(region,OSD_RGN_CLIP_V,0);
        OSD_RGN_Set(region,OSD_RGN_BMP_H,768);

        CLI("osd.sc.s %d %d %d %d %d", plane - 2, sw, sh, u4PanelW, u4PanelH);
        HalFlushDCache();

        OSD_PLA_SetGlobeEnable(plane - 1,1);
        OSD_BASE_SetOsdPosition(plane - 1, pos_x, pos_y);
        CLI("osd.plane.enable %d 1", plane);

        createregion_tag = TRUE;
        _tag_patternoff = TRUE;

        return OSD_CLI_RET_OK;

}

void _OsdExPatternClip(INT32 i4Vclip,INT32 _i4Delay)
{
    INT32 x = i4Vclip % 768 ;
    OSD_RGN_Set(_tag_region,OSD_RGN_CLIP_V,x);
    OSD_RGN_Set(_tag_region,OSD_RGN_BMP_H,768);
    printf(">>v_clip : %d   bmp_h: %d \n",x,768);

    //x_thread_delay(_i4Delay);
    HAL_Delay_us(_i4Delay);

    HalFlushDCache();

}

static INT32 _OsdCmdPattern(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 plane,w,h,bpp;
    UINT32 region, u4RegionList,sw = 0, sh = 0, fgScale = FALSE, cm, pos_x = 0, pos_y = 0;
    static UINT32 osd_buf[5] = {0, 0, 0,0,0};
    UINT32 align_buf, buf = 0;
    OSD_PTTRN_DATA_T t_pttrn_data;
    UINT32 loop_tag;
    CHK_NUM_MIN_PARAM(4);
    plane = StrToInt(szArgv[1]);
    w = StrToInt(szArgv[3]);
    h = StrToInt(szArgv[4]);
    bpp = StrToInt(szArgv[7]);



    //memset(&t_pttrn_data, 0, sizeof(OSD_PTTRN_DATA_T));

    t_pttrn_data.ui4_pttrn_id = (UINT32)StrToInt(szArgv[2]);

    if (i4Argc > 5)
    {
        fgScale = TRUE;
        sw = (UINT32)StrToInt(szArgv[5]);
        sh = (UINT32)StrToInt(szArgv[6]);
    }
    if (i4Argc > 7)
    {
        if (bpp == 16)
        {
            bpp = 2;
            cm = OSD_CM_ARGB4444_DIRECT16;
        }
        else if (bpp == 1555)
        {
            bpp = 2;
            cm = OSD_CM_ARGB1555_DIRECT16;
        }
        else if (bpp == 565)
        {
            bpp = 2;
            cm = OSD_CM_RGB565_DIRECT16;
        }
        else if (bpp == 8)
        {
            bpp = 1;
            cm = OSD_CM_RGB_CLUT8;
        }
        else if (bpp == 1)
        {
            bpp = 1;
            cm = OSD_CM_ALPHA_BIT;
        }
        else
        {
            bpp = 4;
            cm = OSD_CM_ARGB8888_DIRECT32;
        }
    }
    else
    {
        bpp = 4;
        cm = OSD_CM_ARGB8888_DIRECT32;
    }

    switch (t_pttrn_data.ui4_pttrn_id)
    {
    case 0:
        t_pttrn_data.ui4_bg_color = (UINT32)StrToInt(szArgv[8]);
        break;
    case 30:
        t_pttrn_data.ui4_bg_color = (UINT32)StrToInt(szArgv[8]);
        t_pttrn_data.ui4_grid_w   = (UINT32)StrToInt(szArgv[9]);
        t_pttrn_data.ui4_grid_h   = (UINT32)StrToInt(szArgv[10]);
        break;
    case 31:
        t_pttrn_data.ui4_bg_color = (UINT32)StrToInt(szArgv[8]);
        t_pttrn_data.ui1_mode     = (UINT32)StrToInt(szArgv[9]);
        break;
    case 32:
    case 33:
    case 34:
    case 35:
    case 64:
    case 65:
    case 66:
        t_pttrn_data.ui4_bg_color = (UINT32)StrToInt(szArgv[8]);
        break;
    case 54:
    case 55:
    case 56:
    case 57:
    case 58:
    case 59:
    case 60:
    case 61:
    case 62:
    case 63:
        t_pttrn_data.u4ColorIdx1= StrToInt(szArgv[8]);
        t_pttrn_data.u4ColorIdx2= StrToInt(szArgv[9]);
        t_pttrn_data.u4ColorIdx3= StrToInt(szArgv[10]);
        break;
    default:
        break;
    }

    OSD_Init();
    CLI_Parser("osd.d_l -l");
    //printf("t_pttrn_data.u4ColorIdx1= 0x%8x\n  t_pttrn_data.ui4_pttrn_id=%d",t_pttrn_data.u4ColorIdx1,t_pttrn_data.ui4_pttrn_id );
    #ifdef OSD_PLA5_SUPPORT
    if((plane <= 5) &&(plane >= 1)) //klocwork warning
    #else
    if((plane <= 3) &&(plane >= 1)) //klocwork warning
    #endif

    {
     buf = osd_buf[plane - 1];
    }

    if (buf != 0)
    {
        BSP_FreeDmaMemory((UINT32)buf);
    }
    buf = (UINT32)BSP_AllocDmaMemory(OSD_DMA_ALIGN(w * h * bpp)+ OSD_DMA_ALIGN_SIZE + OSD_PALETTE_SIZE);
    #ifdef OSD_PLA5_SUPPORT
    if((plane <= 5) &&(plane >= 1)) //klocwork warning
    #else
    if((plane <= 3) &&(plane >= 1)) //klocwork warning
    #endif
    {
     osd_buf[plane - 1] = buf;
    }

    align_buf = OSD_DMA_ALIGN(buf);
    OSD_RGN_LIST_Create(&u4RegionList);
    Printf("create color mode is cm= %ld, w=%ld, h=%ld, sw=%ld, sh=%ld ,align_buf =0x%lx\n", cm,w,h,sw,sh,align_buf);
    if (fgScale && (plane == 1))
    {
        OSD_RGN_Create(&region, w, h, (void*)align_buf,
                       cm, 0, 0, 0, sw, sh);
    }
    else
    {
        OSD_RGN_Create(&region, w, h, (void*)align_buf,
                       cm, 0, 0, 0, w, h);
    }

    //_OsdDrawPattern(region, pattern);
    _OsdDrawPattern(region, &t_pttrn_data);
    OSD_RGN_Insert(region, u4RegionList);
    OSD_BASE_SetOsdPosition(plane - 1, pos_x, pos_y);
    if (fgScale && plane > 1)
    {
         OSD_SC_Scale(plane - 2, TRUE, w, h, sw, sh);
    }
    //do scaling bottom to top for pattern 44
    if(t_pttrn_data.ui4_pttrn_id == 44)
    {

        OSD_RGN_Set(region,OSD_RGN_POS_Y, sh-1);
        OSD_PLA_FlipTo(plane-1, u4RegionList);
        VERIFY((INT32)OSD_RET_OK == _OSD_BASE_UpdateHwReg());
        OSD_PLA_Enable(plane-1, 1);

        for(loop_tag = 0;loop_tag < sh;loop_tag ++)
        {
            OSD_RGN_Set(region,OSD_RGN_POS_Y, sh-loop_tag-1);
            HalFlushDCache();
            HAL_Delay_us(2000);
        }
    }

    OSD_RGN_Set(region,OSD_RGN_POS_Y, 0);
    HalFlushDCache();
    OSD_PLA_FlipTo(plane-1, u4RegionList);

    if (fgScale && plane > 1)
    {
        OSD_SC_Scale(plane - 2, TRUE, w, h, sw, sh);
    }
    OSD_BASE_SetOsdPosition(plane - 1, pos_x, pos_y);
    OSD_PLA_Enable(plane-1, 1);
    return OSD_CLI_RET_OK;
}

INT32 _OsdDrawPattern(INT32 i4Region, OSD_PTTRN_DATA_T* pt_data)
{
    UINT32 u4RgnW, u4RgnH, u4ColorMode, u4BmpAddress,u4Pattern;

    if (NULL == pt_data)
    {
        return OSD_CLI_RET_ERR_PARAM;
    }

    u4Pattern = pt_data->ui4_pttrn_id;

    //CHK_NUM_MIN_PARAM(2);
    OSD_RGN_Get(i4Region, OSD_RGN_BMP_ADDR, &u4BmpAddress);
    OSD_RGN_Get(i4Region, OSD_RGN_BMP_W, &u4RgnW);
    OSD_RGN_Get(i4Region, OSD_RGN_BMP_H, &u4RgnH);
    OSD_RGN_Get(i4Region, OSD_RGN_COLORMODE, &u4ColorMode);
    printf("pt {region=%d, mode(0:V line, 1:H line, 2:u4RgnW = %ld, 3:u4ColorMode = %ld)}u4Pattern=%d\n", i4Region,u4RgnW, u4ColorMode,u4Pattern);
    u4BmpAddress=VIRTUAL(u4BmpAddress);
    switch (u4Pattern)
    {
    case 0:
        OSD_DrawPattern0((UINT8*)u4BmpAddress, u4RgnW, u4RgnH, u4ColorMode,
                         pt_data->ui4_bg_color);
        break;
    case 1:
    case 2:
    case 3:
    case 4:
    case 6:
    case 8:
    case 10:
    case 12:
    case 20:
    case 22:
    case 24:
    case 52:
        OSD_DrawPattern1((UINT8*)u4BmpAddress, u4RgnW, u4RgnH, u4ColorMode, u4Pattern);
        break;
    case 5:
    case 7:
        OSD_DrawPattern2((UINT8*)u4BmpAddress, u4RgnW, u4RgnH, u4ColorMode, u4Pattern);
        break;
    case 9:
    case 11:
    case 13:
    case 14:
    case 16:
    case 18:
    case 33:
    case 34:
        OSD_DrawPattern3((UINT8*)u4BmpAddress, u4RgnW, u4RgnH, u4ColorMode, u4Pattern,pt_data->ui4_bg_color);
        break;
    case 15:
    case 48:
    case 51:
        //OSD_RGN_SetBigEndian(i4Region, 1);
        OSD_DrawPattern4((UINT8*)u4BmpAddress, u4RgnW, u4RgnH, u4ColorMode, u4Pattern);
        break;
    case 17:
        OSD_RGN_SetBigEndian(i4Region, 1);
        OSD_DrawColorbar((UINT8*)u4BmpAddress, u4RgnW, u4RgnH, u4ColorMode, 0xff);
        break;
    case 19:
    case 21:
        OSD_DrawPattern2((UINT8*)u4BmpAddress, u4RgnW, u4RgnH, u4ColorMode, u4Pattern);
        break;
    case 23:
        OSD_DrawPattern5((UINT8*)u4BmpAddress, u4RgnW, u4RgnH, u4ColorMode);
        break;
    case 25:
        OSD_DrawPattern6((UINT8*)u4BmpAddress, u4RgnW, u4RgnH, u4ColorMode);
        break;
    case 26:
        OSD_DrawPattern7((UINT8*)u4BmpAddress, u4RgnW, u4RgnH, u4ColorMode);
        break;
    case 27:
    case 45:
    case 46:
    case 47:
        OSD_DrawPattern8((UINT8*)u4BmpAddress, u4RgnW, u4RgnH, u4ColorMode, u4Pattern);
        break;
    case 28:
        OSD_DrawPattern9((UINT8*)u4BmpAddress, u4RgnW, u4RgnH, u4ColorMode);
        break;
    case 30:
        OSD_DrawPatternL92((UINT8*)u4BmpAddress, u4RgnW, u4RgnH, u4ColorMode,
                           pt_data->ui4_grid_w,
                           pt_data->ui4_grid_h,
                           pt_data->ui4_bg_color);
        break;
    case 31:
        OSD_DrawPatternGomi((UINT8*)u4BmpAddress, u4RgnW, u4RgnH, u4ColorMode,
                            pt_data->ui4_bg_color,pt_data->ui1_mode);
        break;
    case 32:
        OSD_DrawPatternCrosstalk((UINT8*)u4BmpAddress, u4RgnW, u4RgnH, u4ColorMode,
                                 pt_data->ui4_bg_color);
        break;
    case 35:
        OSD_DrawPatternFlicker((UINT8*)u4BmpAddress, u4RgnW, u4RgnH, u4ColorMode,
                               pt_data->ui4_bg_color);
        break;
    case 36:
        OSD_DrawPattern10((UINT8*)u4BmpAddress, u4RgnW, u4RgnH, u4ColorMode);
        break;
    case 37:
        OSD_DrawPattern11((UINT8*)u4BmpAddress, u4RgnW, u4RgnH, u4ColorMode);
        break;
    case 38:
        OSD_DrawPattern12((UINT8*)u4BmpAddress, u4RgnW, u4RgnH, u4ColorMode);
        break;
    case 39:
        OSD_DrawPattern13((UINT8*)u4BmpAddress, u4RgnW, u4RgnH, u4ColorMode);
        break;
    case 40:
    case 41:
    case 42:
    case 43:
    case 44:
    case 50:
        OSD_DrawPattern14((UINT8*)u4BmpAddress, u4RgnW, u4RgnH, u4ColorMode, u4Pattern,pt_data->ui4_bg_color);
        break;
    case 49:
        OSD_DrawPattern15((UINT8*)u4BmpAddress, u4RgnW, u4RgnH, u4ColorMode);
        break;
    case 53:
        OSD_RGN_SetBigEndian(i4Region, 1);
    #if defined(CC_MT5368)||defined(CC_MT5396)||defined(CC_MT5389)
        OSD_DrawColorbar2((UINT8*)u4BmpAddress, u4RgnW, u4RgnH, u4ColorMode);
    #endif
        break;
    case 54:
        OSD_RGN_SetBigEndian(i4Region, 1);
        OSD_DrawPixLinePattern0((UINT8*)u4BmpAddress, u4RgnW, u4RgnH,
            pt_data->u4ColorIdx1,pt_data->u4ColorIdx2,pt_data->u4ColorIdx3);
        break;
    case 55:
        OSD_RGN_SetBigEndian(i4Region, 1);
        OSD_DrawPixLinePattern1((UINT8*)u4BmpAddress, u4RgnW, u4RgnH,
            pt_data->u4ColorIdx1,pt_data->u4ColorIdx2,pt_data->u4ColorIdx3);
        break;
    case 56:
        OSD_RGN_SetBigEndian(i4Region, 1);
        OSD_DrawPixLinePattern2((UINT8*)u4BmpAddress, u4RgnW, u4RgnH,
            pt_data->u4ColorIdx1,pt_data->u4ColorIdx2,pt_data->u4ColorIdx3);
        break;
    case 57:
        OSD_RGN_SetBigEndian(i4Region, 1);
        OSD_DrawPixLinePattern3((UINT8*)u4BmpAddress, u4RgnW, u4RgnH,
            pt_data->u4ColorIdx1,pt_data->u4ColorIdx2,pt_data->u4ColorIdx3);
        break;
    case 58:
        OSD_RGN_SetBigEndian(i4Region, 1);
        OSD_DrawPixLinePattern4((UINT8*)u4BmpAddress, u4RgnW, u4RgnH,
            pt_data->u4ColorIdx1,pt_data->u4ColorIdx2,pt_data->u4ColorIdx3);
        break;
    case 59:
        OSD_RGN_SetBigEndian(i4Region, 1);
        OSD_DrawPixLinePattern5((UINT8*)u4BmpAddress, u4RgnW, u4RgnH,
            pt_data->u4ColorIdx1,pt_data->u4ColorIdx2,pt_data->u4ColorIdx3);
        break;
    case 60:
        OSD_RGN_SetBigEndian(i4Region, 1);
        OSD_DrawPixLinePattern6((UINT8*)u4BmpAddress, u4RgnW, u4RgnH,
            pt_data->u4ColorIdx1,pt_data->u4ColorIdx2,pt_data->u4ColorIdx3);
        break;
    case 61:
        OSD_RGN_SetBigEndian(i4Region, 1);
        OSD_DrawPixLinePattern7((UINT8*)u4BmpAddress, u4RgnW, u4RgnH,
            pt_data->u4ColorIdx1,pt_data->u4ColorIdx2,pt_data->u4ColorIdx3);
        break;
    case 62:
        OSD_RGN_SetBigEndian(i4Region, 1);
        OSD_DrawPixLinePattern8((UINT8*)u4BmpAddress, u4RgnW, u4RgnH,
            pt_data->u4ColorIdx1,pt_data->u4ColorIdx2,pt_data->u4ColorIdx3);
        break;
    case 63:
        OSD_RGN_SetBigEndian(i4Region, 1);
        OSD_DrawPixLinePattern9((UINT8*)u4BmpAddress, u4RgnW, u4RgnH,
            pt_data->u4ColorIdx1,pt_data->u4ColorIdx2,pt_data->u4ColorIdx3);
        break;
    case 64:
        OSD_DrawPatternFlicker1((UINT8*)u4BmpAddress, u4RgnW, u4RgnH, u4ColorMode,
                               pt_data->ui4_bg_color);
        break;
    case 65:
        OSD_DrawPatternFlicker2((UINT8*)u4BmpAddress, u4RgnW, u4RgnH, u4ColorMode,
                               pt_data->ui4_bg_color);
        break;
    case 66:
        OSD_DrawPatternFlicker3((UINT8*)u4BmpAddress, u4RgnW, u4RgnH, u4ColorMode,
                               pt_data->ui4_bg_color);
        break;
    case 67:
        OSD_DrawPatternStripe1((UINT8*)u4BmpAddress, u4RgnW, u4RgnH, u4ColorMode);
        break;
    case 68:
        OSD_DrawPatternStripe2((UINT8*)u4BmpAddress, u4RgnW, u4RgnH, u4ColorMode);
        break;
    case 69:
        OSD_DrawPatternStripe3((UINT8*)u4BmpAddress, u4RgnW, u4RgnH, u4ColorMode);
        break;
    default:
        OSD_DrawMute((UINT8*)u4BmpAddress, u4RgnW, u4RgnH, u4ColorMode, u4Pattern);
        break;
            
    }


    HalFlushInvalidateDCache();

    return OSD_CLI_RET_OK;
}

static UINT8 _Osd_get_cm_size(UINT32 ui4_cm)
{
    UINT8 ui1_sz = 0x0;

    switch (ui4_cm)
    {
    case OSD_CM_ARGB8888_DIRECT32:
        ui1_sz = 4;
        break;
    case OSD_CM_ARGB4444_DIRECT16:
    case OSD_CM_ARGB1555_DIRECT16:
        ui1_sz = 2;
        break;
    case OSD_CM_RGB_CLUT8:
    case OSD_CM_ALPHA_BIT:
        ui1_sz = 1;
        break;
    default:
        break;
    }

    return ui1_sz;
}

void OSD_DrawPattern3(UINT8* pu1Canvas, UINT32 u4Width, UINT32 u4Height,
                      UINT32 u4ColorMode, UINT32 u4Pattern,UINT32 u4BgColor)
{

    UINT32  u4OsdPitch, u4Size, u4Bpp;
    UINT32 u4Color0 = 0xff000000, u4Color1 = 0xffffffff;
    INT32 i4Step0 = 3, i4GradX0[4] = {3, 3, 3, 3}, i4GradY0[4]  = {3, 3, 3, 3};
    INT32 i4Step1 = 3, i4GradX1[4] = { -3, -3, -3, -3}, i4GradY1[4] = { -3, -3, -3, -3};
    // INT8 ai1Dx[4], i;
    if (u4ColorMode == (UINT32)OSD_CM_RGB_CLUT8)
    {
        Printf("draw pattern not support OSD_CM_RGB_CLUT8");
        return;
    }
    if (u4Width < 100)
    {
        Printf("input width error");
        return;
    }
    // allocate memory for gfx canvas
    OSD_BYTE_PER_PIXEL(u4ColorMode, u4Bpp);
    u4Size = u4Width * u4Height * u4Bpp;
    u4OsdPitch = u4Bpp * u4Width;
    u4OsdPitch = (u4OsdPitch + 0xf) & 0xfffffff0;
    // _AllocateMemory(u4GfxPitch, 0xf, &pu1OrigGfxBuf, &pu1GfxBuf);
    switch (u4Pattern)
    {
    case 9:
        u4Color0 = 0x00000000;
        u4Color1 = 0xffffffff;
        // i4Step0 = 1;
        i4GradX0[0] = 0xFF / ((u4Width - 2) / 2);
        if (i4GradX0[0] == 0)
        {
            i4Step0 = 1;
            i4GradX0[0] = 1;
            i4GradX0[1] = 1;
            i4GradX0[2] = 1;
            i4GradX0[3] = 1;
            i4GradY0[0] = 1;
            i4GradY0[1] = 1;
            i4GradY0[2] = 1;
            i4GradY0[3] = 1;
            i4Step1 = i4Step0;
            i4GradX1[0] = -1;
            i4GradX1[1] = -1;
            i4GradX1[2] = -1;
            i4GradX1[3] = -1;
            i4GradY1[0] = -1;
            i4GradY1[1] = -1;
            i4GradY1[2] = -1;
            i4GradY1[3] = -1;
        }

        break;
    case 11:
        u4Color0 = 0x00000000;
        u4Color1 = 0xffff0000;
        // i4Step0 = 1;
        i4GradX0[0] = 0xFF / ((u4Width - 2) / 2);
        if (i4GradX0[0] == 0)
        {
            i4Step0 = 1;
            i4GradX0[0] = 1;
            i4GradX0[1] = 1;
            i4GradX0[2] = 0;
            i4GradX0[3] = 0;
            i4GradY0[0] = 1;
            i4GradY0[1] = 1;
            i4GradY0[2] = 0;
            i4GradY0[3] = 0;
            i4Step1 = i4Step0;
            i4GradX1[0] = -1;
            i4GradX1[1] = -1;
            i4GradX1[2] = 0;
            i4GradX1[3] = 0;
            i4GradY1[0] = -1;
            i4GradY1[1] = -1;
            i4GradY1[2] = 0;
            i4GradY1[3] = 0;
        }

        break;
    case 13:
        u4Color0 = 0x00000000;
        u4Color1 = 0xff00ff00;
        // i4Step0 = 1;
        i4GradX0[0] = 0xFF / ((u4Width - 2) / 2);
        if (i4GradX0[0] == 0)
        {
            i4Step0 = 1;
            i4GradX0[0] = 1;
            i4GradX0[1] = 0;
            i4GradX0[2] = 1;
            i4GradX0[3] = 0;
            i4GradY0[0] = 1;
            i4GradY0[1] = 0;
            i4GradY0[2] = 1;
            i4GradY0[3] = 0;
            i4Step1 = i4Step0;
            i4GradX1[0] = -1;
            i4GradX1[1] = 0;
            i4GradX1[2] = -1;
            i4GradX1[3] = 0;
            i4GradY1[0] = -1;
            i4GradY1[1] = 0;
            i4GradY1[2] = -1;
            i4GradY1[3] = 0;
        }

        break;
    case 14:
        u4Color0 = 0x00000000;
        u4Color1 = 0xff0000ff;
        // i4Step0 = 1;
        i4GradX0[0] = 0xFF / ((u4Width - 2) / 2);
        if (i4GradX0[0] == 0)
        {
            i4Step0 = 1;
            i4GradX0[0] = 1;
            i4GradX0[1] = 0;
            i4GradX0[2] = 0;
            i4GradX0[3] = 1;
            i4GradY0[0] = 1;
            i4GradY0[1] = 0;
            i4GradY0[2] = 0;
            i4GradY0[3] = 1;
            i4Step1 = i4Step0;
            i4GradX1[0] = -1;
            i4GradX1[1] = 0;
            i4GradX1[2] = 0;
            i4GradX1[3] = -1;
            i4GradY1[0] = -1;
            i4GradY1[1] = 0;
            i4GradY1[2] = 0;
            i4GradY1[3] = -1;
        }

        break;
    case 16:
        u4Color0 = 0xff008000;
        u4Color1 = 0xff008000;
        // i4Step0 = 1;
        i4GradX0[0] = 0xFF / ((u4Width - 2) / 2);
        if (i4GradX0[0] == 0)
        {
            i4Step0 = 1;
            i4GradX0[0] = 0;
            i4GradX0[1] = 0;
            i4GradX0[2] = 0;
            i4GradX0[3] = 0;
            i4GradY0[0] = 0;
            i4GradY0[1] = 0;
            i4GradY0[2] = 0;
            i4GradY0[3] = 0;
            i4Step1 = i4Step0;
            i4GradX1[0] = 0;
            i4GradX1[1] = 0;
            i4GradX1[2] = 0;
            i4GradX1[3] = 0;
            i4GradY1[0] = 0;
            i4GradY1[1] = 0;
            i4GradY1[2] = 0;
            i4GradY1[3] = 0;
        }
        break;
    case 18:
        u4Color0 = 0x00000000;
        u4Color1 = 0xffffffff;
        // i4Step0 = 1;
        i4GradX0[0] = 0xFF / ((u4Width - 2) / 2);
        if (i4GradX0[0] == 0)
        {
            i4Step0 = 1;
            i4Step1 = i4Step0;
            i4GradX1[0] = -1;
            i4GradX1[1] = -1;
            i4GradX1[2] = -1;
            i4GradX1[3] = -1;
            i4GradY1[0] = -1;
            i4GradY1[1] = -1;
            i4GradY1[2] = -1;
            i4GradY1[3] = -1;
        }
        x_memset((void*)pu1Canvas, 0, u4Size);
        GFX_SetDst((UINT8 *)pu1Canvas, u4ColorMode, u4OsdPitch);

        GFX_SetColor(u4Color1);
        CLI("gfx.set.gradopt %d %d %d %d %d %d %d %d %d %d", i4Step1, i4Step1, i4GradX1[0],
            i4GradX1[1], i4GradX1[2], i4GradX1[3], i4GradY1[0], i4GradY1[1], i4GradY1[2], i4GradY1[3]);
        CLI("gfx.op.grad 1  1 %d %d v",  (u4Width - 2) , (u4Height - 2) );
        GFX_Flush();
        GFX_Wait();
        return;
    case 33:
        u4Color0 = u4BgColor;
        GFX_SetDst((UINT8 *)pu1Canvas, u4ColorMode, u4OsdPitch);

        i4GradX1[0] = 0;
        i4GradX1[1] = 0;
        i4GradX1[2] = 0;
        i4GradX1[3] = 0;
        i4GradY1[0] = -1;
        i4GradY1[1] = -1;
        i4GradY1[2] = -1;
        i4GradY1[3] = -1;

        x_memset((void*)pu1Canvas, 0, u4Size);
        GFX_SetDst((UINT8 *)pu1Canvas, u4ColorMode, u4OsdPitch);

        GFX_SetColor(u4Color0);

        CLI("gfx.set.gradopt %d %d %d %d %d %d %d %d %d %d",
            2,
            2,
            i4GradX1[0],
            i4GradX1[1],
            i4GradX1[2],
            i4GradX1[3],
            i4GradY1[0],
            i4GradY1[1],
            i4GradY1[2],
            i4GradY1[3]);
        CLI("gfx.op.grad %d %d %d %d v",0,0,u4Width, u4Height);
        GFX_Flush();
        GFX_Wait();
        return;
    case 34:
    {
        UINT32  u4_x=0x0,u4_y=0x0,u4_len=0x0;
        UINT8   ui1_cm_sz=0x0;

        u4Color0 = u4BgColor;
        GFX_SetDst((UINT8 *)pu1Canvas, u4ColorMode, u4OsdPitch);

        i4GradX1[0] = 0;
        i4GradX1[1] = 0;
        i4GradX1[2] = 0;
        i4GradX1[3] = 0;
        i4GradY1[0] = -1;
        i4GradY1[1] = -1;
        i4GradY1[2] = -1;
        i4GradY1[3] = -1;

        x_memset((void*)pu1Canvas, 0, u4Size);
        GFX_SetDst((UINT8 *)pu1Canvas, u4ColorMode, u4OsdPitch);

        GFX_SetColor(u4Color0);
        ui1_cm_sz = _Osd_get_cm_size(u4ColorMode);

        CLI("gfx.set.gradopt %d %d %d %d %d %d %d %d %d %d",
            2,
            2,
            i4GradX1[0],
            i4GradX1[1],
            i4GradX1[2],
            i4GradX1[3],
            i4GradY1[0],
            i4GradY1[1],
            i4GradY1[2],
            i4GradY1[3]);
        CLI("gfx.op.grad %d %d %d %d v",0,0,u4Width, u4Height);
        GFX_Flush();
        GFX_Wait();

        for ( u4_y= 0x0; u4_y < u4Height; u4_y++)
        {
            u4_len = ((u4Width*u4_y)/u4Height);

            for (u4_x= 0x0; u4_x < u4_len; u4_x++)
            {
                x_memcpy(pu1Canvas+(u4_y*u4OsdPitch) + (u4_x*ui1_cm_sz),
                         pu1Canvas+((u4Height-u4_y-1)*u4OsdPitch) + (((u4Width-u4_x-1))*ui1_cm_sz),
                         ui1_cm_sz);
            }
        }
        GFX_Flush();
        GFX_Wait();
    }
    return;
    default:
        break;
    }
    x_memset((void*)pu1Canvas, 0, u4Size);
    // draw gfx canvas
    GFX_SetDst((UINT8 *)pu1Canvas, u4ColorMode, u4OsdPitch);

    {
        GFX_SetColor(u4Color0);
        CLI("gfx.set.gradopt %d %d %d %d %d %d %d %d %d %d", i4Step0, i4Step0, i4GradX0[0],
            i4GradX0[1], i4GradX0[2], i4GradX0[3], i4GradY0[0], i4GradY0[1], i4GradY0[2], i4GradY0[3]);
        CLI("gfx.op.grad 1  1 %d %d h", (u4Width - 2) / 2 - 20, (u4Height - 2) / 2 - 20); //from yellow to green

        CLI("gfx.op.grad %d  %d %d %d v", (u4Width - 2) / 2, (u4Height - 2) / 2, (u4Width - 2) / 2 - 20, (u4Height - 2) / 2 - 20);
        GFX_SetColor(u4Color1);
        CLI("gfx.set.gradopt %d %d %d %d %d %d %d %d %d %d", i4Step1, i4Step1, i4GradX1[0],
            i4GradX1[1], i4GradX1[2], i4GradX1[3], i4GradY1[0], i4GradY1[1], i4GradY1[2], i4GradY1[3]);
        CLI("gfx.op.grad %d  %d %d %d h", (u4Width - 2) / 2, 1, (u4Width - 2) / 2 - 20, (u4Height - 2) / 2 - 20);
        CLI("gfx.op.grad %d  %d %d %d v", 1, (u4Height - 2) / 2, (u4Width - 2) / 2 - 20, (u4Height - 2) / 2 - 20);
    }
    GFX_Flush();
    GFX_Wait();
}

//-----------------------------------------------



void OSD_DrawPattern14(UINT8* pu1Canvas, UINT32 u4Width, UINT32 u4Height,
                      UINT32 u4ColorMode, UINT32 u4Pattern,UINT32 u4BgColor)
{

    UINT32  u4OsdPitch, u4Size, u4Bpp;
    UINT32 u4Color0 = 0xff000000;//, u4Color1 = 0xffffffff;
    INT32 i4Stepx = 3, i4Stepy = 3,i4GradX[4] = {3, 3, 3, 3}, i4GradY[4]  = {3, 3, 3, 3};

    // INT8 ai1Dx[4], i;
    if (u4ColorMode == (UINT32)OSD_CM_RGB_CLUT8)
    {
        Printf("draw pattern not support OSD_CM_RGB_CLUT8");
        return;
    }
    if (u4Width < 100)
    {
        Printf("input width error");
        return;
    }
    // allocate memory for gfx canvas
    OSD_BYTE_PER_PIXEL(u4ColorMode, u4Bpp);
    u4Size = u4Width * u4Height * u4Bpp;
    u4OsdPitch = u4Bpp * u4Width;
    u4OsdPitch = (u4OsdPitch + 0xf) & 0xfffffff0;
    // _AllocateMemory(u4GfxPitch, 0xf, &pu1OrigGfxBuf, &pu1GfxBuf);

    switch (u4Pattern)
    {
    case 40: //gray ramp
        u4Color0 = 0x00000000;
       // u4Color1 = 0xffffffff;
        i4GradX[0] = 0xFF / ((u4Width - 2) / 2);
        i4Stepx = u4Width / (0xFF + 0x1);
        i4Stepy = u4Height / (0xFF + 0x1);

        if (i4GradX[0] == 0)
        {
            i4GradX[0] = 1;
            i4GradX[1] = 1;
            i4GradX[2] = 1;
            i4GradX[3] = 1;
            i4GradY[0] = 1;
            i4GradY[1] = 1;
            i4GradY[2] = 1;
            i4GradY[3] = 1;
        }
        break;

     case 41: //red ramp
         u4Color0 = 0x00000000;
         //u4Color1 = 0xffff0000;
         i4GradX[0] = 0xFF / ((u4Width - 2) / 2);
         i4Stepx = u4Width / (0xFF + 0x1);
         i4Stepy = u4Height / (0xFF + 0x1);

         if (i4GradX[0] == 0)
        {
            i4GradX[0] = 1;
            i4GradX[1] = 1;
            i4GradX[2] = 0;
            i4GradX[3] = 0;
            i4GradY[0] = 1;
            i4GradY[1] = 1;
            i4GradY[2] = 0;
            i4GradY[3] = 0;
         }
         break;

    case 42: //green ramp
        u4Color0 = 0x00000000;
        //u4Color1 = 0xff00ff00;
        i4GradX[0] = 0xFF / ((u4Width - 2) / 2);
        i4Stepx = u4Width / (0xFF + 0x1);
        i4Stepy = u4Height / (0xFF + 0x1);

        if (i4GradX[0] == 0)
        {
            i4GradX[0] = 1;
            i4GradX[1] = 0;
            i4GradX[2] = 1;
            i4GradX[3] = 0;
            i4GradY[0] = 1;
            i4GradY[1] = 0;
            i4GradY[2] = 1;
            i4GradY[3] = 0;
        }
         break;

    case 43: //blue ramp
        u4Color0 = 0x00000000;
        //u4Color1 = 0xff0000ff;
        i4GradX[0] = 0xFF / ((u4Width - 2) / 2);
        i4Stepx = u4Width / (0xFF + 0x1);
        i4Stepy = u4Height / (0xFF + 0x1);

        if (i4GradX[0] == 0)
       {
           i4GradX[0] = 1;
           i4GradX[1] = 0;
           i4GradX[2] = 0;
           i4GradX[3] = 1;
           i4GradY[0] = 1;
           i4GradY[1] = 0;
           i4GradY[2] = 0;
           i4GradY[3] = 1;
        }
        break;

    case 44: //gray ramp (c)
        u4Color0 = 0xffffffff;
        //u4Color1 = 0x00000000;
        i4GradX[0] = 0xFF / ((u4Width - 2) / 2);
        i4Stepx = u4Width / (0xFF + 0x1);
        i4Stepy = u4Height / (0xFF + 0x1);

        if (i4GradX[0] == 0)
       {
           i4GradX[0] = -1;
           i4GradX[1] = -1;
           i4GradX[2] = -1;
           i4GradX[3] = -1;
           i4GradY[0] = -1;
           i4GradY[1] = -1;
           i4GradY[2] = -1;
           i4GradY[3] = -1;
        }
        break;

    case 50:////gray ramp (h)
             u4Color0 = 0x00000000;
             //u4Color1 = 0x00000000;
             i4GradX[0] = 0xFF / ((u4Width - 2) / 2);
             i4Stepx = u4Width / (0xFF + 0x1);
             i4Stepy = u4Height / (0xFF + 0x1);

             if (i4GradX[0] == 0)
            {
                i4GradX[0] = 1;
                i4GradX[1] = 1;
                i4GradX[2] = 1;
                i4GradX[3] = 1;
                i4GradY[0] = 1;
                i4GradY[1] = 1;
                i4GradY[2] = 1;
                i4GradY[3] = 1;
             }

             x_memset((void*)pu1Canvas, 0, u4Size);
            // draw gfx canvas
            GFX_SetDst((UINT8 *)pu1Canvas, u4ColorMode, u4OsdPitch);

            GFX_SetColor(u4Color0);
            CLI("gfx.set.gradopt %d %d %d %d %d %d %d %d %d %d", i4Stepx, i4Stepy, i4GradX[0],i4GradX[1], i4GradX[2], i4GradX[3], i4GradY[0], i4GradY[1], i4GradY[2], i4GradY[3]);
            CLI("gfx.op.grad 1  1 %d %d h", (u4Width - 1) , (u4Height - 1));
            return;
    default:
            break;

    }
    x_memset((void*)pu1Canvas, 0, u4Size);
    // draw gfx canvas
    GFX_SetDst((UINT8 *)pu1Canvas, u4ColorMode, u4OsdPitch);

        GFX_SetColor(u4Color0);
        CLI("gfx.set.gradopt %d %d %d %d %d %d %d %d %d %d", i4Stepx, i4Stepy, i4GradX[0],i4GradX[1], i4GradX[2], i4GradX[3], i4GradY[0], i4GradY[1], i4GradY[2], i4GradY[3]);
        CLI("gfx.op.grad 1  1 %d %d v", (u4Width - 1) , (u4Height - 1)); //from yellow to green

//        CLI("gfx.op.grad %d  %d %d %d v", (u4Width - 2) / 2, (u4Height - 2) / 2, (u4Width - 2) / 2 - 20, (u4Height - 2) / 2 - 20);


    GFX_Flush();
    GFX_Wait();


}


//-----------------------------------------------
INT32 _OsdPlaneCmdPattern(INT32 i4Argc, const CHAR ** szArgv)
{

    UINT32 plane, w, h, x, y;
    UINT8 enable, cm;
    static UINT32 osd_buf[2] = {0,0};
    static UINT8 currentEnable[2]={0,0};
    static INT32 OldRgnList[2] = {(-1),(-1)};
    static UINT32 OldPLAEanle[2] = {0,0};
    static UINT32 OldSCEanle[2] = {0,0};
    static UINT32 OldSrcW[2] = {0,0};
    static UINT32 OldSrcH[2] = {0,0};
    static UINT32 OldDstW[2] = {0,0};
    static UINT32 OldDstH[2] = {0,0};
    static UINT32 OldIs16bpp[2] = {0,0};

    //UINT32 align_buf, bpp=4,rgnlist,rgnid,idx;
    UINT32 align_buf, bpp=4,idx;
    static UINT32 rgnlist[2],rgnid[2];
    CHK_NUM_MIN_PARAM(7);
     plane = (UINT32)StrToInt(szArgv[1]);
     enable = (UINT32)StrToInt(szArgv[2]);
     w = (UINT32)StrToInt(szArgv[3]);
     h = (UINT32)StrToInt(szArgv[4]);
     cm = (UINT8)StrToInt(szArgv[5]);
     x = (UINT32)StrToInt(szArgv[6]);
     y = (UINT32)StrToInt(szArgv[7]);
    if (plane<=1)
    {
        Printf("incorrect plane number\n");
        return OSD_CLI_RET_OK;
    }
    else
        idx=plane-2;


    if (currentEnable[idx]==enable)
    {
        Printf("Pattern enable state not change: %d\n",enable);
        return OSD_CLI_RET_OK;
    }

    if (currentEnable[idx]==0) //current plane pattern not enabled
    {
        OldRgnList[idx] = OSD_PLA_GetFirstRegionList(plane-1);
        OSD_PLA_GetEnable(plane-1, (BOOL *)&(OldPLAEanle[idx]));
        OSD_SC_GetScalerInfo(plane-2, &(OldSCEanle[idx]),
                             &(OldSrcW[idx]), &(OldSrcH[idx]),
                             &(OldDstW[idx]), &(OldDstH[idx]),
                             &(OldIs16bpp[idx]));

        if (cm==13)
        {
            bpp=2;
        }
        else if (cm==14)
        {
            bpp=4;
        }
        else
        {
            Printf("PlaneShowPattern- not support cm: %d\n",cm);
            return OSD_CLI_RET_OK;
        }

        osd_buf[idx] = (UINT32)BSP_AllocDmaMemory(OSD_DMA_ALIGN(w * h * bpp)
                       + OSD_DMA_ALIGN_SIZE);

        align_buf = OSD_DMA_ALIGN(osd_buf[idx] );

        if ( OSD_RGN_LIST_Create((UINT32*)&(rgnlist[idx])) != OSD_RET_OK)
            Printf("Create rgn list fail\n");

        if ( OSD_RGN_Create(&(rgnid[idx]) , w, h, (void*)align_buf,
                            cm, 0, 0, 0, w, h) != OSD_RET_OK)
        {
            OSD_RGN_LIST_Delete(rgnlist[idx]);
            Printf("Create rgn fail\n");
        }

        CLI("osd.region.pt %d 5", rgnid[idx]);

        OSD_RGN_SetBigEndian(rgnid[idx] , 1);

        OSD_RGN_Insert(rgnid[idx] ,rgnlist[idx] );

        OSD_PLA_FlipTo(plane-1,rgnlist[idx] );

        OSD_BASE_SetOsdPosition(plane - 1, x, y);

        OSD_PLA_Enable(plane-1, 1);

        OSD_SC_Scale(plane-2,1,w,h,w,h);
    }
    else //resstore old setting
    {
        if (osd_buf[idx] != 0)
        {
            BSP_FreeDmaMemory((UINT32)(osd_buf[idx]) );
        }
        //restoring original setting
        OSD_PLA_Enable(plane-1, FALSE);
        if (OldSCEanle[idx] != 0)
        {
            UNUSED(OSD_SC_Scale(idx, OldSCEanle[idx],
                                OldSrcW[idx], OldSrcH[idx],
                                OldDstW[idx], OldDstH[idx]));
        }
        if (OldRgnList[idx] >= 0)
        {
            OSD_PLA_FlipTo(plane-1, (UINT32)(OldRgnList[idx]));
        }
        else
        {
            OSD_PLA_FlipToNone(plane-1);
        }
        if (OldPLAEanle[idx])
        {
            OSD_PLA_Enable(plane-1, TRUE);
        }

        OSD_RGN_Detach(rgnid[idx],rgnlist[idx]);
        OSD_RGN_Delete(rgnid[idx]);
        OSD_RGN_LIST_Delete(rgnlist[idx]);
    }
    currentEnable[idx]=enable;

    return OSD_CLI_RET_OK;

    /*
    INT32 i4Plane;
    BOOL fgEnable;

    Printf("osd.plane.enable {plane (1,2,3)} [{enable (1,0)}]\n");
    CHK_NUM_MIN_PARAM(1);
    GET_PLANE(i4Plane);

    if (i4Argc == 4)
    {
        UINT32 i = 0;
        UINT32 loop = StrToInt(szArgv[2]);
        UINT32 delay = StrToInt(szArgv[3]);
        for (; i<loop; i++)
        {
            Printf("osd test[%d]\n", i);
            OSD_PLA_Enable(i4Plane, 0);
            x_thread_delay(delay);
            OSD_PLA_Enable(i4Plane, 1);
            x_thread_delay(delay);
        }
        return OSD_CLI_RET_OK;
    }

    if (i4Argc >= 3)
    {
        // set enable flag
        switch (szArgv[2][0])
        {
        case 'e':
        case 'E':
        case 'y':
        case 'Y':
        case 't':
        case 'T':
        case '1':
            OSD_PLA_SetWorkaround(i4Plane, 1);
            return OSD_PLA_Enable(i4Plane, 1);

        default:
            OSD_PLA_SetWorkaround(i4Plane, 0);
            return OSD_PLA_Enable(i4Plane, 0);
        }
    }
    else
    {
        OSD_PLA_UNION_T rOsdPlaneReg;
        _OSD_PLA_GetReg(i4Plane - 1, rOsdPlaneReg.au4Reg);
        fgEnable = (BOOL)rOsdPlaneReg.rField.fgEn;
        Printf("OSD Plane %d enable : %d\n", i4Plane + 1, fgEnable);
    }
    return OSD_CLI_RET_OK;
    */
}


//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
static INT32 _OsdPlaneCmdFlip(INT32 i4Argc, const CHAR ** szArgv)
{
    // osd.plane.flip {plane (1,2,3)} {rgnlist}
    INT32 i4Plane;
    INT32 i4RgnList;

    CHK_NUM_MIN_PARAM(2);
    GET_PLANE(i4Plane);
    GET_RGNLIST(i4RgnList, 2);

    return OSD_PLA_FlipTo(i4Plane, i4RgnList);
}


//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
static INT32 _OsdPlaneCmdAlpha(INT32 i4Argc, const CHAR ** szArgv)
{
    // osd.plane.alpha {plane (1,2,3)} [{alpha}]
    INT32 i4Plane;
    INT32 i4Alpha;

    CHK_NUM_MIN_PARAM(1);
    GET_PLANE(i4Plane);

    if (i4Argc >= 3)
    {
        // set alpha
        i4Alpha = StrToInt(szArgv[2]);
        if ((i4Alpha < 0) || (i4Alpha > 255))
        {
            return -(INT32)OSD_CLI_RET_ERR_PARAM;
        }
        return OSD_PLA_SetBlendLevel(i4Plane, (UINT8)i4Alpha);
    }
    else
    {
        // get alpha
        UINT8 u1Alpha;
        OSD_PLA_GetBlendLevel(i4Plane, &u1Alpha);
        Printf("OSD Plane %ld alpha : %d(0x%lx)\n", i4Plane, (UINT32)u1Alpha);
    }
    return OSD_CLI_RET_OK;
}


//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
static INT32 _OsdPlaneCmdFading(INT32 i4Argc, const CHAR ** szArgv)
{
    // osd.plane.alpha {plane (1,2,3)} [{alpha}]
    INT32 i4Plane;
    INT32 i4Alpha;

    CHK_NUM_MIN_PARAM(1);
    GET_PLANE(i4Plane);

    if (i4Argc >= 3)
    {
        // set alpha
        i4Alpha = StrToInt(szArgv[2]);
        if ((i4Alpha < 0) || (i4Alpha > 255))
        {
            return -(INT32)OSD_CLI_RET_ERR_PARAM;
        }
        return OSD_PLA_SetFading(i4Plane, (UINT8)i4Alpha);
    }
    else
    {
        // get alpha
        UINT8 u1Alpha;
        OSD_PLA_GetFading(i4Plane, &u1Alpha);
        Printf("OSD Plane %ld fading : %d(0x%lx)\n", i4Plane, (UINT32)u1Alpha);
    }
    return OSD_CLI_RET_OK;
}

static INT32 _OsdSonyOSDOrder(INT32 i4Argc, const CHAR ** szArgv)
{
	UINT32 u4Top=0,u4Middle =0,u4Bottom =0;
	u4Top = StrToInt(szArgv[1])-1;
	u4Middle = StrToInt(szArgv[2]) -1;
	u4Bottom = StrToInt(szArgv[3]) -1;
	OSD_BASE_SetPlaneSwitchOrder(u4Top,u4Middle,u4Bottom);
	OSD_BASE_SetOsdPosition(u4Top, u4TopPosX, u4TopPosY);
	OSD_BASE_SetOsdPosition(u4Middle, u4MidPosX, u4MidPosY);
	OSD_BASE_SetOsdPosition(u4Bottom, u4BotPosX, u4BotPosY);
	return OSD_CLI_RET_OK;
}

static INT32 _OsdSonyOSDPattern(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 plane[3] = {1,2,3};
    INT32 w = StrToInt(szArgv[1]);
    INT32 h = StrToInt(szArgv[2]);
    INT32 bpp = StrToInt(szArgv[3]);
    UINT32 region1,region2,region3,cm;
	static UINT32 regionList1 = 0,regionList2 = 0,regionList3 = 0;
	UINT32 osd1Buf=0,osd2Buf=0,osd3Buf=0;
	FBM_POOL_T* buf1 = 0;
    if (i4Argc > 3)
    {
        if (bpp == OSD_CM_YCBCR_CLUT2)
        {
            bpp = 1;
            cm = OSD_CM_YCBCR_CLUT2;
        }
        else if (bpp == OSD_CM_YCBCR_CLUT4)
        {
            bpp = 1;
            cm = OSD_CM_YCBCR_CLUT4;
        }
        else if (bpp == OSD_CM_YCBCR_CLUT8)
        {
            bpp = 1;
            cm = OSD_CM_YCBCR_CLUT8;
        }
        else if (bpp == OSD_CM_CBYCRY422_DIRECT16)
        {
            bpp = 2;
            cm = OSD_CM_CBYCRY422_DIRECT16;
        }
        else if (bpp == OSD_CM_YCBYCR422_DIRECT16)
        {
            bpp = 2;
            cm = OSD_CM_YCBYCR422_DIRECT16;
        }
        else if (bpp == OSD_CM_AYCBCR8888_DIRECT32)
        {
            bpp = 4;
            cm = OSD_CM_AYCBCR8888_DIRECT32;
        }
        else if (bpp == OSD_CM_RGB_CLUT2)
        {
            bpp = 1;
            cm = OSD_CM_RGB_CLUT2;
        }
        else if (bpp == OSD_CM_RGB_CLUT4)
        {
            bpp = 1;
            cm = OSD_CM_RGB_CLUT4;
        }
        else if (bpp == OSD_CM_ARGB4444_DIRECT16)
        {
            bpp = 2;
            cm = OSD_CM_ARGB4444_DIRECT16;
        }
        else if (bpp == OSD_CM_ARGB1555_DIRECT16)
        {
            bpp = 2;
            cm = OSD_CM_ARGB1555_DIRECT16;
        }
        else if (bpp == OSD_CM_RGB565_DIRECT16)
        {
            bpp = 2;
            cm = OSD_CM_RGB565_DIRECT16;
        }
        else if (bpp == OSD_CM_RGB_CLUT8)
        {
            bpp = 1;
            cm = OSD_CM_RGB_CLUT8;
        }
        else if (bpp == OSD_CM_ALPHA_BIT)
        {
            bpp = 1;
            cm = OSD_CM_ALPHA_BIT;
        }
        else
        {
            bpp = 4;
            cm = OSD_CM_ARGB8888_DIRECT32;
        }
    }
    else
    {
        bpp = 4;
        cm = OSD_CM_ARGB8888_DIRECT32;
    }
	buf1 = FBM_GetPoolInfo(FBM_POOL_TYPE_MPEG);
	if(buf1->u4Addr ==0)
	{
		printf("Can't get buffer to paint pattern.\n");
	}

	osd1Buf = OSD_DMA_ALIGN( buf1->u4Addr );
	osd2Buf = OSD_DMA_ALIGN(w * h * bpp+ OSD_DMA_ALIGN_SIZE + osd1Buf);
	osd3Buf = OSD_DMA_ALIGN(w * h * bpp+ OSD_DMA_ALIGN_SIZE + osd2Buf);
	osd1Buf=VIRTUAL(osd1Buf);
	osd2Buf=VIRTUAL(osd2Buf);
	osd3Buf=VIRTUAL(osd3Buf);
	
	
	CLI_Parser("osd.init");
	CLI_Parser("osd.d_l 9");
	if(!regionList1)
	{
		OSD_RGN_LIST_Create(&regionList1);
	}
	if(!regionList2)
	{
		OSD_RGN_LIST_Create(&regionList2);
	}
	if(!regionList3)
	{
		OSD_RGN_LIST_Create(&regionList3);
	}
    
    LOG(5, "osd1_buf=0x%x\n", osd1Buf);
	LOG(5, "osd2_buf=0x%x\n", osd2Buf);
	LOG(5, "osd3_buf=0x%x\n", osd3Buf);
    
    OSD_RGN_Create(&region1, w, h, (void*)osd1Buf,
                       cm, 0, 0, 0, w, h);
	OSD_RGN_Create(&region2, w, h, (void*)osd2Buf,
                       cm, 0, 0, 0, w, h);
	OSD_RGN_Create(&region3, w, h, (void*)osd3Buf,
                       cm, 0, 0, 0, w, h);
    // create palette here
    if ((cm == OSD_CM_RGB_CLUT8)||(cm == OSD_CM_RGB_CLUT4)||(cm == 
OSD_CM_RGB_CLUT2)
        ||(cm == OSD_CM_YCBCR_CLUT8)||(cm == OSD_CM_YCBCR_CLUT4)||(cm == 
OSD_CM_YCBCR_CLUT2))
    {
        return -1;
    }
    OSD_DrawSonyPattern((UINT8 *)osd1Buf,w,h,cm,0xffff0000);
	OSD_DrawSonyPattern((UINT8 *)osd2Buf,w,h,cm,0xff00ff00);
	OSD_DrawSonyPattern((UINT8 *)osd3Buf,w,h,cm,0xff0000ff);
	
    CLI("osd.region.insert %d %d", region1, regionList1);
	CLI("osd.region.insert %d %d", region2, regionList2);
	CLI("osd.region.insert %d %d", region3, regionList3);

    CLI("osd.plane.flip %d %d", plane[0], regionList1);
	CLI("osd.plane.flip %d %d", plane[1], regionList2);
	CLI("osd.plane.flip %d %d", plane[2], regionList3);
	
    CLI("osd.sc.s %d %d %d %d %d", plane[1] - 2, w, h, w, h);
	CLI("osd.sc.s %d %d %d %d %d", plane[2] - 2, w, h, w, h);
	CLI("osd.sc.s %d %d %d %d %d", plane[0] + 1, w, h, w, h);
	OSD_BASE_SetPlaneSwitchOrder(OSD_PLANE_2,OSD_PLANE_3,OSD_PLANE_1);
    OSD_BASE_SetOsdPosition(plane[0] - 1, u4TopPosX, u4TopPosY);
	OSD_BASE_SetOsdPosition(plane[1] - 1, u4MidPosX, u4MidPosY);
	OSD_BASE_SetOsdPosition(plane[2] - 1, u4BotPosX, u4BotPosY);

    CLI("osd.plane.enable %d 1", plane[0]);
	CLI("osd.plane.enable %d 1", plane[1]);
	CLI("osd.plane.enable %d 1", plane[2]);

    return OSD_CLI_RET_OK;
}

//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
static INT32 _OsdPlaneCmdHFilter(INT32 i4Argc, const CHAR ** szArgv)
{
    // osd.plane.hfilter {plane (1,2,3)} [{enable}]
    INT32 i4Plane,i4Enable;

    CHK_NUM_MIN_PARAM(2);
    GET_PLANE(i4Plane);
    i4Enable = StrToInt(szArgv[2]);

    if ((i4Enable < 0) || (i4Enable > 1))
    {
        return -(INT32)OSD_CLI_RET_ERR_PARAM;
    }
    return OSD_PLA_SetHFilter(i4Plane, (BOOL)i4Enable);
}

static INT32 _OsdPlaneCmdVFilter(INT32 i4Argc, const CHAR ** szArgv)
{
    // osd.plane.hfilter {plane (1,2,3)} [{enable}]
    INT32 i4Plane, i4Enable;

    CHK_NUM_MIN_PARAM(2);
    GET_PLANE(i4Plane);
    i4Enable = StrToInt(szArgv[2]);

    if ((i4Enable < 0) || (i4Enable > 1))
    {
        return -(INT32)OSD_CLI_RET_ERR_PARAM;
    }
    return OSD_PLA_SetVFilter(i4Plane, (BOOL)i4Enable);
}

//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
static INT32 _OsdPlaneCmdFifo(INT32 i4Argc, const CHAR ** szArgv)
{
    // osd.plane.fifo {plane (1,2,3)}
    // [{fast request, ex vacancy threshold, vacancy threshold, full threshold}]
    INT32 i4Plane, i4FastReq, i4ExVacThr, i4VacThr, i4FullThr;

    CHK_NUM_MIN_PARAM(5);
    GET_PLANE(i4Plane);
    i4FastReq = StrToInt(szArgv[2]);
    i4ExVacThr = StrToInt(szArgv[3]);
    i4VacThr = StrToInt(szArgv[4]);
    i4FullThr = StrToInt(szArgv[5]);
    if ((i4FastReq < 0) || (i4FastReq > 1))
    {
        return -(INT32)OSD_CLI_RET_ERR_PARAM;
    }
    else if ((i4ExVacThr < 0) || (i4ExVacThr > 0x3f))
    {
        return -(INT32)OSD_CLI_RET_ERR_PARAM;
    }
    if ((i4VacThr < 0) || (i4VacThr > 0xf))
    {
        return -(INT32)OSD_CLI_RET_ERR_PARAM;
    }
    if ((i4FullThr < 0) || (i4FullThr > 0xf))
    {
        return -(INT32)OSD_CLI_RET_ERR_PARAM;
    }
    return OSD_PLA_SetFifo(i4Plane, (BOOL)i4FastReq, (UINT8)i4ExVacThr,
                           (UINT8)i4VacThr, (UINT8)i4FullThr);
}


//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
static INT32 _OsdPlaneCmdDumpReg(INT32 i4Argc, const CHAR ** szArgv)
{
    // osd.plane.dumpreg
    OSD_PLA_UNION_T rOsdPlaneReg;
    INT32 i4Plane;

    x_memset(&rOsdPlaneReg, 0, sizeof(OSD_PLA_UNION_T)); //klocwork warning
    for (i4Plane = 0; i4Plane < OSD_PLANE_MAX_NUM; i4Plane++)
    {
        _OSD_PLA_GetReg(i4Plane, rOsdPlaneReg.au4Reg);
        Printf("OSD Plane %d : 0x%08x 0x%08x\n", i4Plane + 1,
               rOsdPlaneReg.au4Reg[0], rOsdPlaneReg.au4Reg[1]);
    }

    return OSD_CLI_RET_OK;
}


//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
static INT32 _OsdPlaneCmdDumpInfo(INT32 i4Argc, const CHAR ** szArgv)
{
    // osd.plane.dumpinfo
    UINT32 u4Plane;

    for (u4Plane = 0; u4Plane < OSD_PLANE_MAX_NUM; u4Plane++)
    {
        OSD_PLA_Dump(u4Plane);
        Printf("\n");
    }

    return OSD_CLI_RET_OK;
}


static INT32 _OsdPlaneCmdCompress(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 i4Plane;

    CHK_NUM_MIN_PARAM(3);
    GET_PLANE(i4Plane);

    OSD_PLA_SetCompressMode(i4Plane, PARAM(1), PARAM(2));

    return OSD_CLI_RET_OK;
}


//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
static INT32 _OsdListCmdCreate(INT32 i4Argc, const CHAR ** szArgv)
{
    // osd.list.create
    INT32 i4RgnList;
    INT32 i4Ret;
    INT32 i4Count = 1;

    if (i4Argc==2)
    {
        i4Count = StrToInt(szArgv[1]);
    }

    do
    {
        i4Ret = OSD_RGN_LIST_Create((UINT32*)&i4RgnList);

        if (i4Ret == OSD_RET_OK)
        {
            LOG(5, "OSD region list created : %d\n", i4RgnList);
        }
    }
    while (--i4Count != 0);

    return i4Ret;
}


//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
static INT32 _OsdListCmdList(INT32 i4Argc, const CHAR ** szArgv)
{
    // osd.list.list
    INT32 i4RgnList = 0;
    INT32 i4Ret = 0;
    INT32 i4ListHead=0, i4ListTail=0, i4Count=0;
    UINT32 fgFlags;
    INT32 i4UsedCount;

    Printf("List  Head    Tail    Count    Flags\n");
    Printf("-----------------------------------------\n");
    for (i4Ret = 0, i4UsedCount = 0; i4Ret == 0; i4RgnList++)
    {
        i4Ret = OSD_RGN_LIST_Get(i4RgnList, OSD_RGN_LIST_HEAD,
                                 (UINT32*)&i4ListHead);
        if (i4Ret)
        {
            continue;
        }
        i4Ret = OSD_RGN_LIST_Get(i4RgnList, OSD_RGN_LIST_TAIL,
                                 (UINT32*)&i4ListTail);
        if (i4Ret)
        {
            continue;
        }
        i4Ret = OSD_RGN_LIST_Get(i4RgnList, OSD_RGN_LIST_COUNT,
                                 (UINT32*)&i4Count);
        if (i4Ret)
        {
            continue;
        }
        i4Ret = OSD_RGN_LIST_Get(i4RgnList, OSD_LIST_FLAGS, &fgFlags);
        if (i4Ret)
        {
            continue;
        }

        Printf(" %ld  h[%ld] t[%ld] cnt(%ld) fg(0x%lx)\n",
               i4RgnList, i4ListHead, i4ListTail, i4Count, fgFlags);
        if (fgFlags)
        {
            i4UsedCount++;
        }
    }
    Printf("-----------------------------------------\n");
    Printf("Total %ld lists are currently used.\n", i4UsedCount);

    return OSD_CLI_RET_OK;
}


//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
static INT32 _OsdListCmdDump(INT32 i4Argc, const CHAR ** szArgv)
{
    // osd.list.dump {region_list}
    INT32 i4RgnList;
    INT32 i4Region = 0;
    INT32 i4Ret;
    INT32 i4Count =0 ;

    CHK_NUM_MIN_PARAM(1);
    GET_RGNLIST(i4RgnList, 1);

    i4Ret = OSD_RGN_LIST_Get(i4RgnList, OSD_RGN_LIST_COUNT, (UINT32*)&i4Count);
    if (i4Ret)
    {
        return -OSD_CLI_RET_ERR_LIST;
    }

    OSD_RGN_LIST_Get(i4RgnList,OSD_RGN_LIST_HEAD, (UINT32*)&i4Region);
    while (i4Count--)
    {
        UINT32 u4Next;
        if (OSD_RGN_Dump(i4Region) < 0)
        {
            break;
        }
        //*OSD_RGN_Get(OSD_RGN_NEXT, i4Region, &u4Next);
        OSD_RGN_Get(i4Region, OSD_RGN_NEXT, &u4Next);
        i4Region = (INT32)u4Next;
    }

    return OSD_CLI_RET_OK;
}


//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
static INT32 _OsdListCmdDelete(INT32 i4Argc, const CHAR ** szArgv)
{
    // osd.list.delete {region_list}
    INT32 i4RgnList;

    CHK_NUM_MIN_PARAM(1);
    GET_RGNLIST(i4RgnList, 1);

    return OSD_RGN_LIST_Delete(i4RgnList);
}


//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
static INT32 _OsdListCmdDetach(INT32 i4Argc, const CHAR ** szArgv)
{
    // osd.list.detach {region_list} {(region | all)}
    INT32 i4RgnList;

    CHK_NUM_MIN_PARAM(2);
    GET_RGNLIST(i4RgnList, 1);

    if (x_strncmp("all", szArgv[2], 3) == 0)
    {
        return OSD_RGN_LIST_DetachAll(i4RgnList);
    }

    if ((szArgv[2][0] <= '9') &&
            (szArgv[2][0] >= '0'))
    {
        INT32 i4Region;
        i4Region = StrToInt(szArgv[2]);

        return OSD_RGN_Detach(i4Region, i4RgnList);
    }

    return -(INT32)OSD_CLI_RET_ERR_PARAM;
}


//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
static INT32 _OsdRgnCmdHelp(INT32 i4Argc, const CHAR ** szArgv)
{
    Printf("osd.region.new {src_w} {src_h} "
           "{src_addr} {color_mode} {src_pitch} "
           "{dst_x} {dst_y} {dst_w} {dst_h}\n"
           "osd.region.append {region} {list}\n"
           "osd.region.insert {region} {list}\n"
           "osd.region.delete {region}\n"
           "osd.region.set {region} {field} {value}\n"
           "\tfields:"
           "\t\talloc, prev, next, flags, pos_x, pos_y,\n"
           "\t\tsrc_w, src_h, src_addr, src_pitch,\n"
           "\t\tout_w, out_h, disp_w, disp_h, colormode, alpha\n"
           "\t\tclip_v, clip_h, pal_load, pal_addr, pal_len\n"
           "\t\tvflip, hmirror\n"
           "\t\tstep_v, step_h, colorkey, colorkey_en, mix_sel\n"
           "\t\thpx, endian, int_h, int_v, step_ho, step_vo\n");

    return OSD_CLI_RET_OK;
}


//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
static INT32 _OsdRgnCmdNew(INT32 i4Argc, const CHAR ** szArgv)
{

    INT32 i4Ret;
    INT32 i4Region;
    UINT32 u4BmpWidth, u4BmpHeight;
    void * pvBitmap;
    UINT32 eColorMode, u4BmpPitch = 0;
    UINT32 u4DispX, u4DispY, u4DispW, u4DispH;

    CHK_NUM_MIN_PARAM(4);

    u4BmpWidth = (UINT32)StrToInt(szArgv[1]);
    u4BmpHeight = (UINT32)StrToInt(szArgv[2]);
    pvBitmap = (void *)StrToInt(szArgv[3]);
    eColorMode = (UINT32)StrToInt(szArgv[4]);

    if (i4Argc == 10)
    {
        u4BmpPitch = (UINT32)StrToInt(szArgv[5]);
        u4DispX = (UINT32)StrToInt(szArgv[6]);
        u4DispY = (UINT32)StrToInt(szArgv[7]);
        u4DispW = (UINT32)StrToInt(szArgv[8]);
        u4DispH = (UINT32)StrToInt(szArgv[9]);
    }
    else
    {
        OSD_GET_PITCH_SIZE(eColorMode, u4BmpWidth, u4BmpPitch);
        u4DispX = 0;
        u4DispY = 0;
        u4DispW = u4BmpWidth;
        u4DispH = u4BmpHeight;
    }

    i4Ret = OSD_RGN_Create
            ((UINT32*)&i4Region,
             u4BmpWidth, u4BmpHeight, pvBitmap,
             eColorMode, u4BmpPitch,
             u4DispX, u4DispY, u4DispW, u4DispH);

    if (i4Ret)
    {
        LOG(1, "OSD region create failed : %d\n", i4Ret);
    }
    else
    {
        LOG(5, "OSD region created : i4Region == %d\n", i4Region);

        if (eColorMode<=OSD_CM_RGB_CLUT8 ||
                eColorMode==OSD_CM_ARGB8888_DIRECT32)
        {
            OSD_RGN_SetBigEndian(i4Region, 1);
        }

        OSD_RGN_Dump(i4Region);
    }

    return i4Ret;
}


//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
static INT32 _OsdRgnCmdInsert(INT32 i4Argc, const CHAR ** szArgv)
{
    // osd.region.insert {region} {list}
    INT32 i4Region, i4RgnList;

    CHK_NUM_MIN_PARAM(2);
    GET_REGION(i4Region, 1);
    GET_RGNLIST(i4RgnList, 2);

    return OSD_RGN_Insert(i4Region, i4RgnList);
}


//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
static INT32 _OsdRgnCmdDelete(INT32 i4Argc, const CHAR ** szArgv)
{
    // osd.region.delete {region};
    INT32 i4Region;
    CHK_NUM_MIN_PARAM(1);
    GET_REGION(i4Region, 1);

    return OSD_RGN_Delete(i4Region);
}


//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
static INT32 _OsdRgnCmdSet(INT32 i4Argc, const CHAR ** szArgv)
{
    // osd.region.set {region} {field} {value}
    //
    // where filed in list:
    //      alloc, prev, next, flags, pos_x, pos_y, src_w, src_h, src_addr,
    //      src_pitch, disp_w, disp_h, colormode, alpha, clip_v, clip_h,
    //      pal_load, pal_addr, pal_len, step_v, step_h, colorkey, colorkey_en,
    //      mix_sel

    INT32 i4Region;
    INT32 eRegionSetCmd;
    CHK_NUM_MIN_PARAM(3);
    GET_REGION(i4Region, 1);

    eRegionSetCmd = OSD_FindField(szArgv[2]);
    if (eRegionSetCmd < 0)
    {
        return eRegionSetCmd;
    }

    return OSD_RGN_Set(i4Region, eRegionSetCmd, StrToInt(szArgv[3]));
}


//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
INT32 _OsdRgnCmdPattern(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 i4Region;
    UINT32 u4Pattern, u4RgnW, u4RgnH, u4ColorMode, u4BmpAddress;
    UINT32 u4Color = 0xffff0000;

    printf("pt {region#, mode(0:V line, 1:H line, 2:border, 3:colorbar)}\n");
    CHK_NUM_MIN_PARAM(2);
    GET_REGION(i4Region, 1);

    u4Pattern = (UINT32)StrToInt(szArgv[2]);
    OSD_RGN_Get(i4Region, OSD_RGN_BMP_ADDR, &u4BmpAddress);
    OSD_RGN_Get(i4Region, OSD_RGN_BMP_W, &u4RgnW);
    OSD_RGN_Get(i4Region, OSD_RGN_BMP_H, &u4RgnH);
    OSD_RGN_Get(i4Region, OSD_RGN_COLORMODE, &u4ColorMode);
    if (szArgv[3] != NULL)
    {
        u4Color = (UINT32)StrToInt(szArgv[3]);
    }
    switch (u4Pattern)
    {
    case 0:
        OSD_DrawVLine((UINT8*)u4BmpAddress, u4RgnW, u4RgnH, u4ColorMode, 0xff);
        break;
    case 1:
        OSD_DrawHLine((UINT8*)u4BmpAddress, u4RgnW, u4RgnH, u4ColorMode);
        break;
    case 2:
        OSD_DrawBorder((UINT8*)u4BmpAddress, u4RgnW, u4RgnH, u4ColorMode);
        break;
    case 3:
        OSD_DrawColorbar((UINT8*)u4BmpAddress, u4RgnW, u4RgnH, u4ColorMode, 0xff);
        break;
    case 4:
        OSD_DrawSlt((UINT8*)u4BmpAddress, u4RgnW, u4RgnH, u4ColorMode);
        break;
    case 5:
        OSD_DrawRamp((UINT8*)u4BmpAddress, u4RgnW, u4RgnH, u4ColorMode, 0xff, 0);
        break;
    case 6:
        OSD_DrawColor((UINT8*)u4BmpAddress, u4RgnW, u4RgnH, u4ColorMode, u4Color);
        break;
    case 7:
        OSD_DrawRamp((UINT8*)u4BmpAddress, u4RgnW, u4RgnH, u4ColorMode, (UINT8)u4Color, 1);
        break;
    case 8:
        OSD_DrawMute((UINT8*)u4BmpAddress, u4RgnW, u4RgnH, u4ColorMode, u4Pattern);
        break;
    default:
        break;
    }
    HalFlushInvalidateDCache();

    return OSD_CLI_RET_OK;
}


//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
static INT32 _OsdScCmdScale(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 sc, sw, sh, dw, dh;
    CHK_NUM_MIN_PARAM(5);
    sc = StrToInt(szArgv[1]);
    sw = StrToInt(szArgv[2]);
    sh = StrToInt(szArgv[3]);
    dw = StrToInt(szArgv[4]);
    dh = StrToInt(szArgv[5]);
    OSD_SC_Scale(sc, TRUE, sw, sh, dw, dh);
    return OSD_CLI_RET_OK;
}


//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
static INT32 _OsdScCmdSetLpf(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 sc, en;
    CHK_NUM_MIN_PARAM(2);
    sc = StrToInt(szArgv[1]);
    en = StrToInt(szArgv[2]);
    if (i4Argc > 3)
    {
        OSD_SC_SetLpfInfo(sc, en, StrToInt(szArgv[3]), StrToInt(szArgv[4]),
                          StrToInt(szArgv[5]), StrToInt(szArgv[6]),
                          StrToInt(szArgv[7]));
    }
    else
    {
        OSD_SC_SetLpf(sc, en);
    }

    return OSD_CLI_RET_OK;
}




//#ifdef CC_MT5392B
#if 1
//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
static INT32 _OsdGpCmdMode(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 mode;
    CHK_NUM_MIN_PARAM(1);
    mode = PARAM(0);
    //IC_VERSION_T eVer = BSP_GetIcVersion();
    //if (eVer >= IC_VER_5382P_AA)
    //{
    if (mode >= 10)
    {
        OSD_DMA_EnableI2C(TRUE);
        mode -= 10;
    }
    else
    {
        OSD_DMA_EnableI2C(FALSE);
    }
    if (i4Argc > 2)
    {
        OSD_DMA_SetUpdatePeriod(PARAM(1));
    }
    OSD_DMA_SetMode((OSD_OP_MODE_T)mode);
    //}
    //else
    //{
    //   Printf("this feature only suppot in 5382P\n");
    //}

    return OSD_CLI_RET_OK;
}

//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
static INT32 _OsdGpCmdTx(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 i4Plane;

    GET_PLANE(i4Plane);

    //IC_VERSION_T eVer = BSP_GetIcVersion();
    if (1/*eVer >= IC_VER_5382P_AA*/)
    {
        OSD_DMA_TransferRegion(i4Plane);
    }
    else
    {
        Printf("this feature only suppot in 5382P\n");
    }

    return OSD_CLI_RET_OK;
}

//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
static INT32 _OsdGpCmdWriteReg(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 reg, value, parity;
    CHK_NUM_MIN_PARAM(2);
    reg = StrToInt(szArgv[1]);
    value = StrToInt(szArgv[2]);
    parity = 0;

    //IC_VERSION_T eVer = BSP_GetIcVersion();
    if (1/*eVer >= IC_VER_5382P_AA*/)
    {
        if (i4Argc > 3)
        {
            parity = StrToInt(szArgv[3]);
        }
        OSD_DMA_WriteRegister(reg, value, TRUE, parity);
    }
    else
    {
        Printf("this feature only suppot in 5382P\n");
    }

    return OSD_CLI_RET_OK;
}

static INT32 _OsdGpCmdWriteReg32bit(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 reg, value, parity;
    CHK_NUM_MIN_PARAM(2);
    reg = StrToInt(szArgv[1]);
    value = StrToInt(szArgv[2]);
    parity = 0;

    //IC_VERSION_T eVer = BSP_GetIcVersion();
    if (1/*eVer >= IC_VER_5382P_AA*/)
    {
        if (i4Argc > 3)
        {
            parity = StrToInt(szArgv[3]);
        }
        OSD_DMA_WriteRegister(reg, value, FALSE, parity);
    }
    else
    {
        Printf("this feature only suppot in 5382P\n");
    }

    return OSD_CLI_RET_OK;
}

static INT32 _OsdGpCmdCopy(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4DstAddress, u4SrcAddress, u4Size;
    CHK_NUM_MIN_PARAM(3);
    u4DstAddress = (UINT32)StrToInt(szArgv[1]);
    u4SrcAddress = (UINT32)StrToInt(szArgv[2]);
    u4Size = (UINT32)StrToInt(szArgv[3]);

    //IC_VERSION_T eVer = BSP_GetIcVersion();
    if (1/*eVer >= IC_VER_5382P_AA*/)
    {
        OSD_DMA_Copy(u4DstAddress, u4SrcAddress, u4Size);
    }
    else
    {
        Printf("this feature only suppot in 5382P\n");
    }

    return OSD_CLI_RET_OK;
}

static INT32 _OsdGpCmdSet8281Reg(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Addr, u4Value;
    CHK_NUM_MIN_PARAM(2);
    u4Addr = (UINT32)StrToInt(szArgv[1]);
    u4Value = (UINT32)StrToInt(szArgv[2]);

    OSD_DMA_Set8281(u4Addr, u4Value);

    return OSD_CLI_RET_OK;
}

#endif
//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
static INT32 _OsdCmdInit(INT32 i4Argc, const CHAR ** szArgv)
{
    //OSD_Reset();
    OSD_Init();
    OSD_Reset();

    return OSD_CLI_RET_OK;
}


//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
static INT32 _OsdCmdMode(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 mode;
    CHK_NUM_MIN_PARAM(1);
    mode = StrToInt(szArgv[1]);
    OSD_VERIFY_DISPLAY_MODE(mode);
    OSD_BASE_SetDisplayMode(mode);

    return OSD_CLI_RET_OK;
}



#ifdef _TEST_SCROLL__
static INT32 _i4ScrollRgn = -1;
static INT32 _i4Step = 0;
static UINT32 _u4PosX = 0;

VOID  _OsdScrollIsrCB(VOID)
{
	if(_i4ScrollRgn >= 0)
	{
	    _u4PosX += _i4Step;
		if(_i4Step > 0 && _u4PosX >= 1800)
			_u4PosX = 0;
		if(_i4Step < 0 && _u4PosX <= 0)
		    _u4PosX = 1800;
		
        OSD_RGN_Set(_i4ScrollRgn, OSD_RGN_POS_X, _u4PosX);
	}
}

static INT32 _OsdCmdScroll(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4RgnList;
    UINT32 u4Region;
    UINT32 u4Plane = StrToInt(szArgv[1]) - 1;
	UINT32 u4Width = StrToInt(szArgv[2]);
	UINT32 u4Height = StrToInt(szArgv[3]);
	UINT32 u4Pitch = (u4Width * 4 + 0xf) & 0xfffffff0;
    UINT32 u4Buf = (UINT32)BSP_AllocDmaMemory(OSD_DMA_ALIGN(u4Width * u4Height * 4)+ OSD_DMA_ALIGN_SIZE + OSD_PALETTE_SIZE);
	u4Buf = (u4Buf + 0xF) & 0xfffffff0;
    OSD_RGN_Create(&u4Region, u4Width, u4Height, (void*)u4Buf, OSD_CM_ARGB8888_DIRECT32, 0, 0, 0, u4Width, u4Height);
    OSD_RGN_Set(u4Region, OSD_RGN_MIX_SEL, 1);
	OSD_RGN_Set(u4Region, OSD_RGN_POS_Y, 480);
	OSD_RGN_LIST_Create(&u4RgnList);
	OSD_RGN_Insert(u4Region, u4RgnList);
	OSD_PLA_FlipTo(u4Plane, u4RgnList);
	OSD_SC_Scale(u4Plane - 1, 1, 1920, 1080, 1920, 1080);
    OSD_PLA_SetFading(u4Plane, 255);

	{
		GFX_Lock();
		GFX_MMU_Set_Enable(FALSE);
		GFX_SetDst((UINT8 *)u4Buf, (UINT32) CM_ARGB8888_DIRECT32, u4Pitch);
		GFX_SetColor(0xFFFF0000);
		GFX_Fill(0, 0, u4Width, u4Height);
		GFX_Flush();
		GFX_Wait();
		GFX_Unlock();
	}


    OSD_PLA_Enable(u4Plane, 1);

	_i4ScrollRgn = (INT32) u4Region;
	_i4Step = StrToInt(szArgv[4]);

	return 0;
}
#endif
//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
static INT32 _OsdCmdQuery(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Tmp;

    for (u4Tmp = 0; u4Tmp < OSD_PLANE_MAX_NUM; u4Tmp++)
    {
        OSD_PLA_Dump(u4Tmp);
    }
    OSD_BASE_Dump();
#ifdef CC_MT5392B
    OSD_DMA_Dump();
#endif
    return OSD_CLI_RET_OK;
}

static INT32 _OsdCmdSetLVDS3(INT32 i4Argc, const CHAR ** szArgv)
{
    // enable DUAL bit
    //vRegWriteFldAlign(LVDS_00, 1, LVDS_00_DUAL);
    IO_WRITE32MSK(0x20027400,0,0x00080000,0x00080000);
    //0x20027400[19] = 1

    // enable Tx mode 3
    /*
    vRegWriteFldAlign(OUTSTG_OS_19, 1, OS_19_REG_YUV2YC_EN);
    0x20027464[24] = 1
    vRegWriteFldAlign(OUTSTG_OS_19, 1, OS_19_REG_LPF_EN);
    0x20027464[23] = 1
    vRegWriteFldAlign(OUTSTG_OS_19, 1, OS_19_REG_C_LINE_EXT);
    0x20027464[22] = 1
    vRegWriteFldAlign(OUTSTG_OS_19, 1, OS_19_REG_MERGE_OSD);
    0x20027464[21] = 1
    vRegWriteFldAlign(OUTSTG_OS_19, 1, OS_19_REG_2CH_MERGE);
    0x20027464[20] = 1
    vRegWriteFldAlign(OUTSTG_OS_19, 0, OS_19_RGB_444_MERGE);
    0x20027464[17] = 0
    */
    IO_WRITE32MSK(0x20027464,0,0x01f00000,0x01f20000);

    // bypass yuv2rgb
    // vRegWriteFldAlign(OUTSTG_OS_84, 1, OS_2A_YUV2RGB_BYPASS_MAIN);
    // 0x20027610[31] = 1
    // vRegWriteFldAlign(OUTSTG_OS_84, 1, OS_2A_YUV2RGB_BYPASS_PIP);
    // 0x20027610[30] = 1
    IO_WRITE32MSK(0x20027610,0,0xc0000000,0xc0000000);

    // Internal Protocal fix JEIDA
    //vRegWriteFldAlign(LVDS_00, 0, LVDS_00_MSB_INV);
    //0x20027400[12] = 0
    IO_WRITE32MSK(0x20027400,0,0x0000,0x1000);
    // enable 10-bit
    //vRegWriteFldAlign(OUTSTG_OS_29, 1, OS_29_10B_EN);
    //0x200274A4[4] = 1
    IO_WRITE32MSK(0x200274A4,0,0x10,0x10);

    // BGA with 10-bit LVDS (change pin mux)
    //vRegWriteFldAlign(OUTSTG_OS_19, 0, OS_19_REG_8BIT_DUAL);
    //0x20027464[19] = 0
    //vRegWriteFldAlign(OUTSTG_OS_19, 0, OS_19_REG_BGA_8BIT_DUAL);
    //0x20027464[18] = 0
    IO_WRITE32MSK(0x20027464,0,0x0000,0xc0000);

    // all LVDS power on
    //   analog related setting
    //vRegWriteFldAlign(LVDS_04, 0, LVDS_04_APD);
    //0x20027410[11:0] = 0
    IO_WRITE32MSK(0x20027410,0,0x0000,0xfff);
    //vRegWriteFldAlign(LVDS_01, 0, LVDS_01_BPD);

    //   digital related setting
    //vRegWriteFldAlign(OUTSTG_OS_8F, 0, OUTSTG_OS_8F_PD_23_0);     // Clock & Digital related setting
    //0x2002763C[23:0] = 0
    IO_WRITE32MSK(0x2002763C,0,0x0000,0xffffff);


    return OSD_CLI_RET_OK;
}


static INT32 _OsdCmdSetPowerDwn(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 fgEnable;
    CHK_NUM_MIN_PARAM(1);
    fgEnable = StrToInt(szArgv[1]);

    Printf("_OsdCmdSetPowerDwn: %d\n",fgEnable);
    OSD_SetPowerDwn(fgEnable);
    return OSD_CLI_RET_OK;
}
#ifdef __MODEL_slt__
#define OSD_TVE_OUT
#endif
static INT32 _OsdCmdSetTVEout(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 fgEnable;
    CHK_NUM_MIN_PARAM(1);
    fgEnable = StrToInt(szArgv[1]);
    Printf("_OsdCmdSetTVEout: %d\n",fgEnable);
#ifdef OSD_TVE_OUT
    OSD_SetTVEOut(fgEnable);
#endif
    return OSD_CLI_RET_OK;
}

static INT32 _OsdCmdShowLogo(INT32 i4Argc, const CHAR ** szArgv)
{
    Printf("_OsdCmdShowLogo\n");
    OSD_ShowLogo();
    return OSD_CLI_RET_OK;
}

#ifdef CC_MT5365
static INT32 _OsdCmdGetUnderflowStatus(INT32 i4Argc, const CHAR ** szArgv)
{
    if (OSD_GetUnderflowStatus()==1)
        Printf("OSD FIFO Underflow!\n");
    else
        Printf("OSD FIFO OK!\n");
    return OSD_CLI_RET_OK;
}
static INT32 _OsdCmdClearUnderflowStatus(INT32 i4Argc, const CHAR ** szArgv)
{
    Printf("OSD Clear Underflow!\n");
    OSD_ClearUnderflowStatus();
    return OSD_CLI_RET_OK;
}
#endif
//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
INT32 OSD_FindField(const CHAR *szText)
{
    TEXT2ENUM_T* prText2Enum = _arRegionFieldEnum;

    if (NULL == szText)
    {
        return -(INT32)OSD_CLI_RET_ERR_PARAM;
    }
    while (prText2Enum->szText)
    {
        if (x_strncmp(prText2Enum->szText, szText,
                      x_strlen(prText2Enum->szText)) == 0)
        {
            break;
        }
        else
        {
            prText2Enum++;
        }
    }

    return prText2Enum->i4Value;
}

//20101227
static INT32  _OsdCmdSet3DPrLineShift(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Enable = 1, u4Shift = 10;
    u4Enable = (UINT32)StrToInt(szArgv[1]);
    u4Shift = (UINT32)StrToInt(szArgv[2]);
    // Printf("osd 3d pr line shift enbale  [{enable (1,0)}]=%d  { shift 1-10} = %d\n",u4Enable,u4Shift);
    OSD_BASE_SetLineShift( u4Enable, u4Shift);
    return OSD_CLI_RET_OK;
}

static INT32  _OsdCmdIsDisplaying(INT32 i4Argc, const CHAR ** szArgv)
{

 #if defined(CC_MT5365) || defined(CC_MT5395)
 return OSD_CLI_RET_OK;
 #else
   UINT32 u4Enable;
   u4Enable = OSD_BASE_IsDisplaying();
   if(u4Enable)
   printf("Osd is displaying now\n");
   else
   printf("there is no osd displaying now\n");

   return OSD_CLI_RET_OK;
 #endif
}

#ifdef CC_TDTV_3D_OSD_ENABLE
void _OsdCmdShow3DSbs(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Enable = 0, u4Shift = 0;
    u4Enable= (UINT32)StrToInt(szArgv[1]);
    u4Shift = (UINT32)StrToInt(szArgv[2]);
    gfgOsd_Sbs = u4Enable ;
    _u13DOsdShift = u4Shift;
    printf("gfgOsd_Sbs = %d \n", gfgOsd_Sbs);
    return;
}
void _OsdCmdShow3DTbb(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 u4Enable = 0, u4Shift = 0;
    u4Enable= (UINT32)StrToInt(szArgv[1]);
    u4Shift = (UINT32)StrToInt(szArgv[2]);
    gfgOsd_Tbb = u4Enable ;
    _u13DOsdShift = u4Shift;
    printf("gfgOsd_Tbb = %d \n", gfgOsd_Tbb);
    return;
}

void _OsdCmdShow3DShutter(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Plane = 2, u4Enable = 1, u4Shift = 10;
    u4Enable = (UINT32)StrToInt(szArgv[1]);
    u4Shift = (UINT32)StrToInt(szArgv[2]);
    Printf("osd 3d shutter enbale  [{enable (1,0)}] { shift 1-10}\n");
    OSD_BASE_Enabe3DShutter( u4Enable, u4Shift);
    return;
}

#endif

#ifdef OSD_TRANS_CTRL
    #if LG_OSD_PATH_TEST
INT32 _OsdCmdGetFRCHeader(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Value;

    OSD_BASE_GetFRCHeader(OSD_FRCHD_FORMAT,&u4Value);
    printf("OSD_FRCHD_FORMAT  = %d\n",u4Value);
    OSD_BASE_GetFRCHeader(OSD_FRCHD_PIXORDER,&u4Value);
    printf("OSD_FRCHD_PIXORDER  = %d\n",u4Value);
    OSD_BASE_GetFRCHeader(OSD_FRCHD_LRINDEX,&u4Value);
    printf("OSD_FRCHD_LRINDEX  = %d\n",u4Value);
    OSD_BASE_GetFRCHeader(OSD_FRCHD_BMPINDEX,&u4Value);
    printf("OSD_FRCHD_BMPINDEX  = %d\n",u4Value);
    OSD_BASE_GetFRCHeader(OSD_FRCHD_FRAMENUM,&u4Value);
    printf("OSD_FRCHD_FRAMENUM  = %d\n",u4Value);
    OSD_BASE_GetFRCHeader(OSD_FRCHD_PREFIX,&u4Value);
    printf("OSD_FRCHD_PREFIX  = %d\n",u4Value);
    OSD_BASE_GetFRCHeader(OSD_FRCHD_BMPHEIGHT,&u4Value);
    printf("OSD_FRCHD_BMPHEIGHT  = %d\n",u4Value);
    OSD_BASE_GetFRCHeader(OSD_FRCHD_BMPWIDTH,&u4Value);
    printf("OSD_FRCHD_BMPWIDTH  = %d\n",u4Value);
    OSD_BASE_GetFRCHeader(OSD_FRCHD_GLOBALALPHA,&u4Value);
    printf("OSD_FRCHD_GLOBALALPHA  = %d\n",u4Value);
    OSD_BASE_GetFRCHeader(OSD_FRCHD_PIXELALPHAEN,&u4Value);
    printf("OSD_FRCHD_PIXELALPHAEN  = %d\n",u4Value);
    OSD_BASE_GetFRCHeader(OSD_FRCHD_GLOBALALPHAEN,&u4Value);
    printf("OSD_FRCHD_GLOBALALPHAEN  = %d\n",u4Value);
    OSD_BASE_GetFRCHeader(OSD_FRCHD_ODPUPPER,&u4Value);
    printf("OSD_FRCHD_ODPUPPER  = %d\n",u4Value);
    OSD_BASE_GetFRCHeader(OSD_FRCHD_DISPYOFF,&u4Value);
    printf("OSD_FRCHD_DISPYOFF  = %d\n",u4Value);
    OSD_BASE_GetFRCHeader(OSD_FRCHD_DISPXOFF,&u4Value);
    printf("OSD_FRCHD_DISPXOFF  = %d\n",u4Value);
    OSD_BASE_GetFRCHeader(OSD_FRCHD_DISPHEIGHT,&u4Value);
    printf("OSD_FRCHD_DISPHEIGHT  = %d\n",u4Value);
    OSD_BASE_GetFRCHeader(OSD_FRCHD_DISPWIDTH,&u4Value);
    printf("OSD_FRCHD_DISPWIDTH  = %d\n",u4Value);

    return OSD_CLI_RET_OK;
}

INT32 _OsdCmdLGOSDTest(INT32 i4Argc, const CHAR ** szArgv)
{
    static UINT32 buf=NULL;
    static UINT32 u4BmpAddrBack =NULL;
    UINT32 u4Region=0 , u4TransferCtl = 0;
    UINT32 align_buf = NULL;
    UINT32 buftag=NULL;
    UINT32 w=0, h=0;

    u4TransferCtl= (UINT32)StrToInt(szArgv[1]);

    w = PANEL_GetPanelWidth();
    h = PANEL_GetPanelHeight();
    if (buf != 0)
    {
        BSP_FreeDmaMemory((UINT32)buf);
    }
    buf = (UINT32)BSP_AllocDmaMemory(OSD_DMA_ALIGN(w * h * 4)
                                     + OSD_DMA_ALIGN_SIZE + OSD_PALETTE_SIZE);
    align_buf = OSD_DMA_ALIGN(buf);

    u4Region = OSD_PLA_GetFirstRegion(OSD_PLANE_2);

    OSD_RGN_Get(u4Region,OSD_RGN_BMP_ADDR,&buftag);
    if(buftag != align_buf)
    {
        u4BmpAddrBack = buftag;
    }

    OSD_BASE_SetPlaneSwitchOrder(OSD_PLANE_1, OSD_PLANE_3, OSD_PLANE_2);

    OSD_BASE_SetTransferCtl(u4TransferCtl);

    switch(u4TransferCtl)
    {
        case 0:
            OSD_RGN_Set(u4Region,OSD_RGN_BMP_ADDR,u4BmpAddrBack);
            break;
        case 1:
            OSD_RGN_Set(u4Region,OSD_RGN_BMP_ADDR,u4BmpAddrBack);
            break;
        case 2:
            OSD_BASE_TransferTo(u4BmpAddrBack,align_buf);
            break;
        case 3:
            OSD_BASE_TransferTo(u4BmpAddrBack,align_buf);
            break;
        default:
            printf("u4TransferCtl value not support\n");
            break;
    }

    return OSD_CLI_RET_OK;
}
    #endif
#endif
#ifdef CC_ANDROID_3D_UI_SUPPORT
static INT32 _OsdCmdSetUiMode(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 u4Mode = 0, u4Width = 0, u4Height = 0;
    u4Mode= (UINT32)StrToInt(szArgv[1]);
    u4Width = (UINT32)StrToInt(szArgv[2]);
    u4Height = (UINT32)StrToInt(szArgv[3]);
    OSD_PLA_SetUIMode(u4Mode,u4Width, u4Height);
    printf("u4Mode = %d \n", u4Mode);
    return OSD_CLI_RET_OK;
}
#endif

#include "drvcust_if.h"

static VOID OSD_FillRectangle(UINT32 u4Buf, UINT32 u4ColorMode, UINT32 u4Pitch, UINT32 u4DstX, UINT32 u4DstY, UINT32 u4Width, UINT32 u4Height, UINT32 u4Color)
{
	GFX_Lock();
	GFX_MMU_Set_Enable(FALSE);
	GFX_SetDst((UINT8 *)u4Buf, u4ColorMode, u4Pitch);
	GFX_SetColor(u4Color);
	GFX_Fill(u4DstX, u4DstY, u4Width, u4Height);
	GFX_Flush();
	GFX_Wait();
	GFX_Unlock();    
}

static VOID OSD_GenPattern(UINT32 u4Buf, UINT32 u4Pitch, UINT32 u4Width, UINT32 u4Height, UINT32 u4ColorMode)
{
    OSD_FillRectangle(u4Buf, u4ColorMode, u4Pitch, 0, 0, u4Width, u4Height, 0x7FFFFFFF);

	OSD_FillRectangle(u4Buf, u4ColorMode, u4Pitch, 0, 0, u4Width, 1, 0xFFFF0000);//top
	OSD_FillRectangle(u4Buf, u4ColorMode, u4Pitch, 0, u4Height - 1, u4Width, 1, 0xFFFF0000);//bottom
	OSD_FillRectangle(u4Buf, u4ColorMode, u4Pitch, 0, 0, 1, u4Height, 0xFFFF0000);//left
	OSD_FillRectangle(u4Buf, u4ColorMode, u4Pitch, u4Width - 1, 0, 1, u4Height, 0xFFFF0000);//right
    OSD_FillRectangle(u4Buf, u4ColorMode, u4Pitch, u4Width/2 - 1, 0, 2, u4Height, 0xFFFF0000);//middle
	OSD_FillRectangle(u4Buf, u4ColorMode, u4Pitch, u4Width/2 - 2, 0, 1, u4Height, 0xFF0000FF);//middle-left
	OSD_FillRectangle(u4Buf, u4ColorMode, u4Pitch, u4Width/2 + 1, 0, 1, u4Height, 0xFF0000FF);//middle-right
}

static VOID OSD_ShowPattern(UINT32 u4Plane, UINT32 u4Buf, UINT32 u4Width, UINT32 u4Height, UINT32 u4Pitch, UINT32 u4ColorMode)
{
    UINT32 u4RgnList;
    UINT32 u4Region;
    OSD_RGN_Create(&u4Region, u4Width, u4Height, (void*)u4Buf, u4ColorMode, u4Pitch, 0, 0, u4Width, u4Height);
    OSD_RGN_Set(u4Region, OSD_RGN_MIX_SEL, 1);
	OSD_RGN_LIST_Create(&u4RgnList);
	OSD_RGN_Insert(u4Region, u4RgnList);
	OSD_PLA_FlipTo(u4Plane, u4RgnList);
	OSD_SC_Scale(OSD_PLANE_TO_SCALER(u4Plane), 1, u4Width, u4Height, OSD_BASE_GetDisplayWidth(), OSD_BASE_GetDisplayHeight());
    OSD_PLA_SetFading(u4Plane, 255);

	OSD_PLA_Enable(u4Plane, 1);
}

static INT32 _OsdCmdShowPattern(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 i4Plane;
    UINT32 u4Width, u4Height, u4Pitch;
	UINT32 u4Addr;

	CHK_NUM_MIN_PARAM(3);
	
    GET_PLANE(i4Plane);
    u4Width= (UINT32)StrToInt(szArgv[2]);
    u4Height = (UINT32)StrToInt(szArgv[3]);
    u4Pitch = (u4Width * 4 + 0xf) & 0xfffffff0;
	
    u4Addr = BSP_GetFbmMemAddr() - DRVCUST_OptGet(eFBMemSize) - DRVCUST_OptGet(eDirectFBMemSize);
	u4Addr = (u4Addr + 0xF) & 0xfffffff0;
    OSD_GenPattern(u4Addr, u4Pitch, u4Width, u4Height, (UINT32) CM_ARGB8888_DIRECT32);
	OSD_ShowPattern((UINT32)i4Plane, u4Addr, u4Width, u4Height, u4Pitch, CM_ARGB8888_DIRECT32);

	return 0;
}


