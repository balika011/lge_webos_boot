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
 *-----------------------------------------------------------------------------
 * $Author: dtvbm11 $
 * $Date: 2015/01/09 $
 * $RCSfile: nptv_scpos_cmd.c,v $
 * $Revision: #1 $
 *
 * Description:
 *         This file contains CLI implementation of NPTV Video.
 *---------------------------------------------------------------------------*/

#include "x_os.h"
#include "x_stl_lib.h"
#include "drv_scaler_drvif.h"


#ifdef __MW_CLI_DEF__
#include "x_mid_cli.h"
#else
#include "x_drv_cli.h"
#endif

#include "x_assert.h"
#include "x_pinmux.h"
#include "x_debug.h"
#include "x_mid.h"

#include "source_table.h"
#include "sv_const.h"
#include "drv_scpos.h"
#include "drv_upscaler.h"
#include "vdp_display.h"
#include "drv_scaler.h"

#include "video_def.h"
#include "frametrack_drvif.h"
#include "hw_scpos.h"

#ifndef CC_CLI
#define CC_CLI
#endif

INT32 _ScposSetInputSelfPtnCmd(INT32 i4Argc, const CHAR **szArgv);
INT32 _ScposSetInputPtnCmd(INT32 i4Argc, const CHAR **szArgv);
INT32 _ScposSetOutputPtnCmd(INT32 i4Argc, const CHAR **szArgv);
static INT32 _ScposSetFrameTrackTarget(INT32 i4Argc, const CHAR **szArgv);
static INT32 _ScposSetHPorchCmd(INT32 i4Argc, const CHAR **szArgv);
static INT32 _ScposSetVPorchCmd(INT32 i4Argc, const CHAR **szArgv);

#ifdef CC_CLI
static INT32 _NptvImportProtectionOnOff(INT32 i4Argc, const CHAR **szArgv);
static INT32 _ScpipCheckFifoOverCmd(INT32 i4Argc, const CHAR **szArgv);
static INT32 _ScpipQueryCmd(INT32 i4Argc, const CHAR **szArgv);
static INT32 _ScpipSetMirrorCmd(INT32 i4Argc, const CHAR **szArgv);
static INT32 _ScpipSetFlipCmd(INT32 i4Argc, const CHAR **szArgv);
static INT32 _ScpipSetMirrorFlipAllCmd(INT32 i4Argc, const CHAR **szArgv);
static INT32 _ScpipSetTdtvInOutCmd(INT32 i4Argc, const CHAR **szArgv);
static INT32 _ScpipSetTdtvResolutionCmd(INT32 i4Argc, const CHAR **szArgv);

static INT32 ScpipAutoUT(INT32 i4Argc, const CHAR **szArgv);
static INT32 ScpipSourceRegionUT(INT32 i4Argc, const CHAR **szArgv);
static INT32 ScpipOutputRegionUT(INT32 i4Argc, const CHAR **szArgv);

static INT32 _ScpipSetForcedFrontBackDramMode(INT32 i4Argc, const CHAR **szArgv);
static INT32 _ScpipSetForcedScalingMode(INT32 i4Argc, const CHAR **szArgv);
static INT32 _ScpipSetForced444Mode(INT32 i4Argc, const CHAR **szArgv);
static INT32 _ScpipSetForcedDramWriteFmt(INT32 i4Argc, const CHAR **szArgv);
static INT32 _ScpipSetForcedVScalingMode(INT32 i4Argc, const CHAR **szArgv);
static INT32 _ScpipGetForcedVScalingMode(INT32 i4Argc, const CHAR **szArgv);
static INT32 _ScpipPollingStatus(INT32 i4Argc, const CHAR **szArgv);
static INT32 _ScpipScalingCoef(INT32 i4Argc, const CHAR **szArgv);
static INT32 _ScpipSetCRC(INT32 i4Argc, const CHAR **szArgv);
static INT32 _ScpipQueryCRC(INT32 i4Argc, const CHAR **szArgv);
static INT32 _ScpipSetISR(INT32 i4Argc, const CHAR **szArgv);
static INT32 _ScpipClrISR(INT32 i4Argc, const CHAR **szArgv);
static INT32 _ScpipQueryISR(INT32 i4Argc, const CHAR **szArgv);
static INT32 _ScpipMid(INT32 i4Argc, const CHAR **szArgv);
#endif /*#ifdef CC_CLI*/

static INT32 _ScpipSubSrcSelect(INT32 i4Argc, const CHAR **szArgv);
static INT32 _ScpipTveSrcSelect(INT32 i4Argc, const CHAR **szArgv);

/******************************************************************************
* CLI Command Table
******************************************************************************/

CLIMOD_DEBUG_FUNCTIONS(SCPOS)

// Table for scpos command entry
CLI_EXEC_T arScposCmdTbl[] = {
  {"sc in pattern selfgen", "selfi", _ScposSetInputSelfPtnCmd, NULL, "Scalar input Pattern self gen", CLI_SUPERVISOR},
  {"sc in pattern", "sci", _ScposSetInputPtnCmd, NULL, "Scalar input Pattern", CLI_SUPERVISOR},    //porting done
  {"sc out pattern", "sco", _ScposSetOutputPtnCmd, NULL, "Scalar output Pattern",  CLI_SUPERVISOR},      //porting done
  {"frametrackTarget",  "ftt",  _ScposSetFrameTrackTarget,  NULL,  "Set Frame Track TARGET value",  CLI_SUPERVISOR},  
  {"sethporch",  "hpos",  _ScposSetHPorchCmd,  NULL,  "Set H Porch",  CLI_SUPERVISOR},  
  {"setvporch",  "vpos",  _ScposSetVPorchCmd,  NULL,  "Set V Porch",  CLI_SUPERVISOR},
  {"setScpipSubSrcSel",  "pipsel",  _ScpipSubSrcSelect,  NULL,  "PIP source select",  CLI_SUPERVISOR},
  {"setScpipTveSrcSel",  "tvesel",  _ScpipTveSrcSelect,  NULL,  "TVE source select",  CLI_SUPERVISOR},
  
  CLIMOD_DEBUG_CLIENTRY(SCPOS),
  #ifdef CC_CLI
  {"ImportProtectionOnOff",  "ipoo",  _NptvImportProtectionOnOff,  NULL,  "Nptv Import Protection on/off",  CLI_SUPERVISOR},  
  {"fifoover", "fover", _ScpipCheckFifoOverCmd, NULL, "Check scaler dram fifo overflow status", CLI_SUPERVISOR},
  {"query", "q", _ScpipQueryCmd, NULL, "Query scaler status", CLI_SUPERVISOR},    
  {"Mirror", "mir", _ScpipSetMirrorCmd, NULL, "Set Scaler Mirror in H dir on/off", CLI_GUEST},
  {"Flip", "flip", _ScpipSetFlipCmd, NULL, "Set Scaler Flip in V dir on/off", CLI_GUEST},
  {"MirrorFlip", "mf", _ScpipSetMirrorFlipAllCmd, NULL, "Set Scaler Mirror and Flip in both H/V for main and sub", CLI_GUEST},  
  {"3D input/output", "3dio", _ScpipSetTdtvInOutCmd, NULL, "Set 3D input/ output format", CLI_SUPERVISOR},  
  {"3D resolution", "3dres", _ScpipSetTdtvResolutionCmd, NULL, "Set 3D pre/pst resolution", CLI_SUPERVISOR},   
  {"Srcr Test", "aut", ScpipAutoUT, NULL, "Auto check", CLI_SUPERVISOR},
  {"Srcr Test", "srcrut", ScpipSourceRegionUT, NULL, "Source Region UT", CLI_SUPERVISOR},
  {"Outr Test", "outrut", ScpipOutputRegionUT, NULL, "Output Region UT", CLI_SUPERVISOR},   
  {"Read status", "poll", _ScpipPollingStatus, NULL, "Read Status", CLI_SUPERVISOR},
  {"Scaling Coef", "coef", _ScpipScalingCoef, NULL, "Set Scaling Coef", CLI_SUPERVISOR},
  {"force input", "f4", _ScpipSetForced444Mode, NULL, "Forced Input 444/422 mode", CLI_SUPERVISOR},  
  {"force scl mode", "fsm", _ScpipSetForcedScalingMode, NULL, "Forced 444/422 scaling mode", CLI_SUPERVISOR},  
  {"force dram fmt", "fdf", _ScpipSetForcedDramWriteFmt, NULL, "Force dram 444/422 write format", CLI_SUPERVISOR},  
  {"force dram mode", "fdm", _ScpipSetForcedFrontBackDramMode, NULL, "Force front/back dram mode", CLI_SUPERVISOR},  
  {"force V scl mode", "fvs", _ScpipSetForcedVScalingMode, NULL, "Force V FIR/Partial sum scaling", CLI_SUPERVISOR}, 
  {"force V scl mode", "gvs", _ScpipGetForcedVScalingMode, NULL, "Get V FIR/Partial sum scaling", CLI_SUPERVISOR},
  {"crc set enable", "crcs", _ScpipSetCRC, NULL, "Enable scaler CRC", CLI_SUPERVISOR},    
  {"crc query status", "crcq", _ScpipQueryCRC, NULL, "Query scaler CRC status", CLI_SUPERVISOR},    
  {"isr set enable", "isrs", _ScpipSetISR, NULL, "Enable scaler ISR", CLI_SUPERVISOR},    
  {"isr clear status", "isrc", _ScpipClrISR, NULL, "Clear scaler ISR status", CLI_SUPERVISOR},    
  {"isr query status", "isrq", _ScpipQueryISR, NULL, "Query scaler ISR status", CLI_SUPERVISOR},    
  {"mid region", "mid", _ScpipMid, NULL, "Set scaler MID region from FBM", CLI_SUPERVISOR},    
  #endif
  {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};

/******************************************************************************
* Local Function
******************************************************************************/

static INT32 _ScpipSetMirrorCmd(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 bPath, bOnOff;
    
    if (i4Argc < 2)
    {
        Printf("Usage: <main:0/sub:1> <on:1/off:0>\n");
        return 0;
    }

    bPath = (UINT8)StrToInt(szArgv[1]);
    bOnOff= (UINT8)StrToInt(szArgv[2]);

    vScpipSetMirror(bPath,bOnOff);

    vScpipReconfig(getScalerVDP(bPath));
    
    printf("[SetMirror]  path [%d], onoff[%d]\n", (INT32)bPath,(INT32)bOnOff);
    printf("Please change timing to trigger mode change\n");
    
    return 0;
}


static INT32 _ScpipSetFlipCmd(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 bPath, bOnOff;
    
    if (i4Argc < 2)
    {
        Printf("Usage: <main:0/sub:1> <on:1/off:0>\n");
        return 0;
    }

    bPath = (UINT8)StrToInt(szArgv[1]);
    bOnOff= (UINT8)StrToInt(szArgv[2]);

    vScpipSetFlip(bPath,bOnOff);
    vScpipReconfig(getScalerVDP(bPath));

    printf("[SetFlip]  path [%d], onoff[%d]\n",(INT32)bPath,(INT32)bOnOff);
    printf("Please change timing to trigger mode change\n");
    
    return 0;
}


static INT32 _ScpipSetMirrorFlipAllCmd(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8  bOnOff;
    
    if (i4Argc < 2)
    {
        Printf("Usage: <on:1/off:0>\n");
        return 0;
    }

    bOnOff= (UINT8)StrToInt(szArgv[1]);

    vScpipSetMirrorFlipAll(bOnOff);
    vSetScposFlg(MAIN_DISP_PRM_CHG);
    vSetScposFlg(PIP_DISP_PRM_CHG);

    printf("[SetMirrorFlipAll] onoff[%d]\n",(INT32)bOnOff);
    printf("Please change timing to trigger mode change\n");
 
    return 0;
}

static INT32 _ScpipSetTdtvResolutionCmd(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4VdpId;
    E_TDTV_RES_OUTPUT preH, preV, pstH, pstV;

    if (i4Argc < 6 || StrToInt(szArgv[1])> 1 || 
        StrToInt(szArgv[2])>3 || StrToInt(szArgv[3])>3 ||
        StrToInt(szArgv[4])>3 || StrToInt(szArgv[5])>3)
    {
        Printf("Usage: <main:0/sub:1> <PreH> <PreV> <PstH> <PstV>\n");
        Printf("===============================\n");
        Printf("<PreH> <PreV> <PstH> <PstV>\n");
        Printf("0: E_TD_RES_NORMAL\n");
        Printf("1: E_TD_RES_HALF\n");
        Printf("2: E_TD_RES_DOUBLE\n");
        Printf("===============================\n");
        return 0;
    }    

    u4VdpId = StrToInt(szArgv[1]);
    preH = (E_TDTV_RES_OUTPUT)StrToInt(szArgv[2]);
    preV = (E_TDTV_RES_OUTPUT)StrToInt(szArgv[3]);
    pstH = (E_TDTV_RES_OUTPUT)StrToInt(szArgv[4]);
    pstV = (E_TDTV_RES_OUTPUT)StrToInt(szArgv[5]);

    vScpipSetTdtvResolutionType(u4VdpId, SCALER_TDTV_RES_POSITION_PRE_H, preH);
    vScpipSetTdtvResolutionType(u4VdpId, SCALER_TDTV_RES_POSITION_PRE_V, preV);
    vScpipSetTdtvResolutionType(u4VdpId, SCALER_TDTV_RES_POSITION_PST_H, pstH);
    vScpipSetTdtvResolutionType(u4VdpId, SCALER_TDTV_RES_POSITION_PST_V, pstV);

    return 0;
}

static INT32 _ScpipSetTdtvInOutCmd(INT32 i4Argc, const CHAR **szArgv)
{
    SCALER_TDTV_IN_TYPE in;
    SCALER_TDTV_OUT_TYPE out;
    UINT32 path;
    if (i4Argc < 4 || StrToInt(szArgv[1])> 1 || StrToInt(szArgv[2])>4 || StrToInt(szArgv[3])>5)
    {
        Printf("Usage: <main:0/sub:1> <3D in> <3D out>\n");
        Printf("===============================\n");
        Printf("<3D in>\n");
        Printf("0: SCALER_TDTV_IN_TYPE_2D\n");
        Printf("1: SCALER_TDTV_IN_TYPE_FS\n");
        Printf("2: SCALER_TDTV_IN_TYPE_SBS\n");
        Printf("3: SCALER_TDTV_IN_TYPE_TAB\n");        
        Printf("4: SCALER_TDTV_IN_TYPE_LI\n");
        Printf("===============================\n");
        Printf("<3D out>\n");
        Printf("0: SCALER_TDTV_OUT_TYPE_2D\n");
        Printf("1: SCALER_TDTV_OUT_TYPE_PR\n");
        Printf("2: SCALER_TDTV_OUT_TYPE_FPR\n");
        Printf("3: SCALER_TDTV_OUT_TYPE_SBS\n");        
        Printf("4: SCALER_TDTV_OUT_TYPE_SG\n");        
        Printf("5: SCALER_TDTV_OUT_TYPE_3D_TO_2D\n");        
        Printf("===============================\n");
        return 0;
    }
    path = (UINT32)StrToInt(szArgv[1]);
    in = (SCALER_TDTV_IN_TYPE)StrToInt(szArgv[2]);
    out = (SCALER_TDTV_OUT_TYPE)StrToInt(szArgv[3]);

    vScpipSetTdtvIO(path,in,out);

    return 0;
}

static INT32 _ScposSetFrameTrackTarget(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4FrameTrackTarget;
    
    if(i4Argc < 2)
    {
        Printf("Usage:<frame track target value>\n");
    }

    u4FrameTrackTarget = StrToInt(szArgv[1]);
    vScpipSetFrameTrackTargetVar(u4FrameTrackTarget);

    vScpipReconfig(SV_VP_MAIN);
    vScpipReconfig(SV_VP_PIP);

    return 0;
}

static INT32 _ScposSetHPorchCmd(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4Path = 0;
    UINT16 u2Current = 0xffff;
    UINT16 u2Value;

    if (i4Argc > 1)
    {
        u4Path = StrToInt(szArgv[1]);
        u2Current = _VDP_ScposGetPorch((UINT8)u4Path, SV_HPORCH_MIN);        
    }
        
    if (i4Argc < 3)
    {
        Printf("Usage: <u4VdpId> <value>\n");
        if (u2Current != 0xffff)
        {
            Printf("VDP(%d) Current H Porch = %d\n", (INT32)u4Path, (INT32)(1000 - u2Current));
        }
        return 0;
    }

    u2Value = (UINT16)StrToInt(szArgv[2]);
    u2Value += u2Current;
    _VDP_ScposSetPorch((UINT8)u4Path, SV_HPORCH_CURRENT, u2Value); 

    return 0;
}

//-----------------------------------------------------------------
//
//-----------------------------------------------------------------
static INT32 _ScpipSubSrcSelect(INT32 i4Argc, const CHAR **szArgv)
{
   UINT8 u1PipSrcSel;
   
   if(i4Argc != 2)
   {
       Printf("Usage: %s <SubSrcSel> (sub scaler input source select)\n", szArgv[0]);
       Printf("SUB_FSC       =0\n");
       Printf("SUB_MAIN_PDS  =1\n");
       Printf("SUB_SUB_PDS   =2\n");
       Printf("SUB_OSTG      =3\n");
    
       return 0;
   }

   u1PipSrcSel = (UINT8)StrToInt(szArgv[1]);

    if ( u1PipSrcSel > 3 )
    {
       Printf("Usage error : u1PipSrcSel must less than 4.\n" );
       return 0;
    }

   vScpipSubSrcSel( u1PipSrcSel );
   return 0;
}

//-----------------------------------------------------------------
//
//-----------------------------------------------------------------
static INT32 _ScpipTveSrcSelect(INT32 i4Argc, const CHAR **szArgv)
{
   UINT8 u1TveSrcSel;
   
   if(i4Argc != 2)
   {
       Printf("Usage: %s <TveSrcSel> (tve input source select)\n", szArgv[0]);
       Printf("Mon Out To TVE  =0\n");
       Printf("Sub FSC To TVE  =1\n");
       return 0;
   }

   u1TveSrcSel = (UINT8)StrToInt(szArgv[1]);

    if ( u1TveSrcSel > 1 )
    {
       Printf("Usage error : u1TveSrcSel must less than 2.\n" );
       return 0;
    }

   vScpipTveSrcSel( u1TveSrcSel );
   return 0;
}

//-----------------------------------------------------------------
//
//-----------------------------------------------------------------
static INT32 _ScposSetVPorchCmd(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4Path = 0;
    UINT16 u2Current = 0xffff;
    UINT16 u2Value;

    if (i4Argc > 1)
    {
        u4Path = StrToInt(szArgv[1]);
        u2Current = _VDP_ScposGetPorch((UINT8)u4Path, SV_VPORCH_MIN);        
    }
        
    if (i4Argc < 3)
    {
        Printf("Usage: <u4VdpId> <value>\n");
        if (u2Current != 0xffff)
        {
            Printf("VDP(%d) Current V Porch = %d\n", (INT32)u4Path, (INT32)(1000 - u2Current));
        }
        return 0;
    }

    u2Value = (UINT16)StrToInt(szArgv[2]);
    u2Value += u2Current;
    _VDP_ScposSetPorch((UINT8)u4Path, SV_VPORCH_CURRENT, u2Value); 

    return 0;
}

INT32 _ScposSetInputSelfPtnCmd(INT32 i4Argc, const CHAR **szArgv)
{
	UINT32 u4OnOff;

	if(i4Argc != 2)
	{
		Printf("Usage: %s <OnOff> (main scaler input pattern selfgen)\n", szArgv[0]);
		return 0;
	}

	u4OnOff = (UINT32)StrToInt(szArgv[1]);

	if(0 == u4OnOff)
	{
		u4OnOff = SV_OFF;
	}
	else
	{
		u4OnOff = SV_ON;
	}

	vScpipSelfInputPatternOnOff(u4OnOff);
	return 0;
}


/**
 *  @6896 porting done
 */ 
INT32 _ScposSetInputPtnCmd(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4Path;
    UINT32 u4Mode;
    UINT32 u4Invalid = 0;

    if (i4Argc < 3)
    {
        u4Invalid = 1;
    }
    else if (i4Argc < 7)
    {
        u4Path = StrToInt(szArgv[1]);
        u4Mode = StrToInt(szArgv[2]);

        if ((u4Mode != VDP_DS_PTN_OFF) && (u4Mode !=  VDP_DS_PTN_AUTO))
        {
            u4Invalid = 2;
        }
    }

    if (u4Invalid)
    {
        Printf("Usage: %s <u4VdpId> <OnOff> <Width> <Height>\n", szArgv[0]);
        return 0;
    }

    u4Path = StrToInt(szArgv[1]);
    u4Mode = StrToInt(szArgv[2]);

    if ((u4Mode != SCPIP_PTGEN_OFF) && (u4Mode !=  SCPIP_PTGEN_AUTO))
    {
        vScpipSetInputPattern(u4Path, SCPIP_PTGEN_MANUAL, StrToInt(szArgv[3]), StrToInt(szArgv[4]));
    }
    else
    {
        vScpipSetInputPattern(u4Path, u4Mode, 0, 0);
    }

    return 0;
}
/**
 *  @6896 porting done
 */
INT32 _ScposSetOutputPtnCmd(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4Path;
    UINT32 u4Mode;
    UINT32 u4Invalid = 0;
  //  UINT32 u4HTotal, u4Width, u4VTotal, u4Height;

    if (i4Argc < 3)
    {
        u4Invalid = 1;
    }
    else if (i4Argc < 5)
    {
        u4Path = StrToInt(szArgv[1]);
        u4Mode = StrToInt(szArgv[2]);

        if ((u4Mode != VDP_US_PTN_OFF) && (u4Mode !=  VDP_US_PTN_AUTO))
        {
            u4Invalid = 2;
        }
    }

    if (u4Invalid)
    {
        Printf("Usage: %s <u4VdpId> <OnOff> <Width> <Height>\n", szArgv[0]);
        return 0;
    }

    u4Path = StrToInt(szArgv[1]);
    u4Mode = StrToInt(szArgv[2]);

    if ((u4Mode != VDP_US_PTN_OFF) && (u4Mode !=  VDP_US_PTN_AUTO))
    {
#if defined(CC_MT5363) || defined(CC_MT5387)
        vDrvSetUSPattern(u4Path, VDP_US_PTN_MANUAL, StrToInt(szArgv[3]), StrToInt(szArgv[4]));            
#else
        vScpipSetOutputPattern(u4Path, VDP_US_PTN_MANUAL, StrToInt(szArgv[3]), StrToInt(szArgv[4])); 
#endif
    }
    else
    {
#if defined(CC_MT5363) || defined(CC_MT5387)
        vDrvSetUSPattern(u4Path, u4Mode, 0, 0);            
#else
        vScpipSetOutputPattern(u4Path, u4Mode, 0, 0);            
#endif
    }

    
    return 0;
}

#ifdef CC_CLI


static INT32 _NptvImportProtectionOnOff(INT32 i4Argc, const CHAR **szArgv)
{
	return 0;
}

static INT32 _ScpipCheckFifoOverCmd(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 bPath;
    UINT32 u4Ret;
    if (i4Argc < 2)
    {
        Printf("Usage: <main:0/sub:1>\n");
        return 0;
    }

    bPath = (UINT8)StrToInt(szArgv[1]);

    vScpipResetFifoOverflow((UINT32)bPath);
    u4Ret = u4ScpipCheckFifoOverflow((UINT32)bPath);
    Printf("[CheckFifoOver]  path [%d], u4Ret[%d]\n", (INT32)bPath,(INT32)u4Ret);    
    if(u4Ret == 0)	// 0: no overflow
    {
        Printf("No fifo overflow!\n");
    }
    else if(u4Ret == 1)	//1: overflow
    {
        Printf("Fifo overflow!\n");
    }
    else    // not support
    {
        Printf("Fifo overflow/underflow flag not support in this mode\n");
    }

    vScpipClearUnderflow((UINT32)bPath);
    if (u4ScpipGetUnderflowStatus((UINT32)bPath))
    {
        Printf("Underflow!\n");
    }
    else
    {
        Printf("No underflow\n");
    }

    return 0;
}

/**
 *  @6896 porting done
 */
static INT32 _ScpipQueryCmd(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4VdpId;
    UINT32 u4Mode;
    UINT32 pipOffset;
    if (i4Argc < 2)
    {
        Printf("Usage: <main:0/sub:1> <reg:0/FSC global:1/PSC global:2>\n");
        return 0;
    }

    u4VdpId = (UINT32)StrToInt(szArgv[1]);
    if(i4Argc >2)
    {
        u4Mode = (UINT32)StrToInt(szArgv[2]);
    }
    else
    {
        u4Mode = 0;
    }
    
    if(u4VdpId == VDP_1)
    {
        Printf("[Main Path]\n");
    }
    else
    {
        Printf("[Sub Path]\n");        
    }

    if(u4Mode == 0)
    {
        pipOffset = getPIPOffset((UINT8)u4VdpId);

        Printf("\n<Dram Setting>\n\n");
        NPTV_REG_PRINT_HEX(SCPIP_DRAM_M_04+pipOffset, DRAM_M_04_DA_ADDR_BASE_MSB_Y);   
        NPTV_REG_PRINT_HEX(SCPIP_DRAM_M_05+pipOffset, DRAM_M_05_DA_ADDR_BASE_LSB_Y);   
        NPTV_REG_PRINT_HEX(SCPIP_DRAM_M_06+pipOffset, DRAM_M_06_DA_ADDR_BASE_MSB_U);   
        NPTV_REG_PRINT_HEX(SCPIP_DRAM_M_07+pipOffset, DRAM_M_07_DA_ADDR_BASE_LSB_U);   
        NPTV_REG_PRINT_HEX(SCPIP_DRAM_M_08+pipOffset, DRAM_M_08_DA_ADDR_BASE_MSB_V);   
        NPTV_REG_PRINT_HEX(SCPIP_DRAM_M_09+pipOffset, DRAM_M_09_DA_ADDR_BASE_LSB_V);
        NPTV_REG_PRINT_HEX(SCPIP_DRAM_M_0B+pipOffset, DRAM_M_0B_DA_WADDR_LO_LIMIT);
        NPTV_REG_PRINT_HEX(SCPIP_DRAM_M_0A+pipOffset, DRAM_M_0A_DA_WADDR_HI_LIMIT);     
        NPTV_REG_PRINT(SCPIP_DRAM_M_00+pipOffset, DRAM_M_00_DA_DRAM_LINE_PITCH);   
        NPTV_REG_PRINT(SCPIP_DRAM_M_00+pipOffset, DRAM_M_00_DA_FRAME_NUM);   
        NPTV_REG_PRINT(SCPIP_DRAM_M_00+pipOffset, DRAM_M_00_DA_DISPLAY_MODE);   
        NPTV_REG_PRINT(SCPIP_DRAM_M_00+pipOffset, DRAM_M_00_DA_READ_ENABLE);   
        NPTV_REG_PRINT(SCPIP_DRAM_M_00+pipOffset, DRAM_M_00_DA_WRITE_ENABLE);               

        pipOffset = getCtrlOffset((UINT8)u4VdpId);

        Printf("\n<Control Setting>\n\n");
        NPTV_REG_PRINT(SCPIP_SCCTRL1_05+pipOffset, SCCTRL1_05_SC_NONL_H_ENABLE);
        NPTV_REG_PRINT(SCPIP_SCCTRL1_09+pipOffset, SCCTRL1_09_SC_INPUT_WIDTH_1);
        NPTV_REG_PRINT(SCPIP_SCCTRL1_09+pipOffset, SCCTRL1_09_SC_INPUT_HEIGHT_1);
        NPTV_REG_PRINT(SCPIP_SCCTRL1_11+pipOffset, SCCTRL1_11_SC_OUTPUT_WIDTH_1);
        NPTV_REG_PRINT(SCPIP_SCCTRL1_11+pipOffset, SCCTRL1_11_SC_OUTPUT_HEIGHT_1);
        NPTV_REG_PRINT(SCPIP_SCCTRL1_1E+pipOffset, SCCTRL1_1E_SC_RPT_GEN_VCNT_1);
        NPTV_REG_PRINT(SCPIP_SCCTRL1_1B+pipOffset, SCCTRL1_1B_SC_DRAM_W_444_1);
        NPTV_REG_PRINT(SCPIP_SCCTRL1_1B+pipOffset, SCCTRL1_1B_SC_DRAM_WR_10B_SEL_1);
        NPTV_REG_PRINT(SCPIP_SCCTRL1_13+pipOffset, SCCTRL1_13_SC_FRN_DRAM_MODE_1);
        NPTV_REG_PRINT(SCPIP_SCCTRL1_13+pipOffset, SCCTRL1_13_SC_BCK_DRAM_MODE_1);

        Printf("\n<System Setting>\n\n");
        NPTV_REG_PRINT(SCPIP_SCSYS_01, SCSYS_01_SC_LINESYNC_MODE);
        NPTV_REG_PRINT(SCPIP_SCSYS_10, SCSYS_10_SC_FRM_TRK_LINE);
        NPTV_REG_PRINT(SCPIP_SCSYS_10, SCSYS_10_SC_FRM_MASK_I);
        NPTV_REG_PRINT(SCPIP_SCSYS_11, SCSYS_11_SC_FRM_TRK_DDDS_EN);
        NPTV_REG_PRINT(SCPIP_SCSYS_14, SCSYS_14_SC_FRM_TRK_STEP);
    }
    else if(u4Mode == 1)
    {
        vScpipDumpResInfo(u4VdpId);
    }
    
    return 0;
}

EXTERN UINT8 SerPollGetChar(void);

static INT32 ScpipAutoUT(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 vdpID;
    UINT32 fromW;
    UINT32 fromH;
    UINT32 HStep;
    UINT32 VStep;
    UCHAR rFullRegion;
    VDP_REGION_T rCurRegion;        
    UINT8 u1Char;

    if(i4Argc != 6)
    {
        Printf("Usage: vdpId fromWidth fromHeight HStep VStep\n");
        return 0;
    }

    vdpID = (UINT32)StrToInt(szArgv[1]);
    fromW = (UINT32)StrToInt(szArgv[2]);
    fromH = (UINT32)StrToInt(szArgv[3]);
    HStep = (UINT32)StrToInt(szArgv[4]);
    VStep = (UINT32)StrToInt(szArgv[5]);

    if (fromW == 0 && fromH == 0)
    {
        VDP_GetOutRegion(vdpID, &rFullRegion, &rCurRegion);
        fromW = rCurRegion.u4Width;
        fromH = rCurRegion.u4Height;
    }

    _XdataAutoScanMode = 0;
    _XdataAutoScan = 1;
    vScpipXdataAutoCheck(vdpID, fromW, fromH, HStep, VStep);

    while (1)
    {
        u1Char = SerPollGetChar();

        Printf("Input(%d)\n",u1Char);
        if (u1Char == 27) // escape, end of auto pattern
        {
            _XdataAutoScan = 0;
            Printf("Quit Auto Scan UT\n");
        	return 0;
        }
        
        switch(u1Char)
        {
            case 'u': // check underflow
                u1ScpipIsTGUnderflow(vdpID);
            break;
                
            case 's': // start/stop
                _XdataAutoScan = (_XdataAutoScan == 0)?1:0;
            break;

            case '=': // forward
                _XdataAutoScanMode &= ~(0x02);
            break;

            case '-': // backward
                _XdataAutoScanMode |= 0x02;
            break;

            case 'm':
                _XdataAutoScanMode ^= 0x04;
                break;

            case 'r':
                _XdataAutoScanMode |= 0x08;
                break;
            
            case ' ': // step by step
                _XdataAutoScan = 0;
                _XdataAutoScanMode |= 0x01;
            break;
        }
    }
}

static INT32 ScpipSourceRegionUT(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4VdpId;
    UINT32 u4HStep;
    UINT32 u4VStep;
    UINT32 u4T;
    UINT32 u4B;
    UINT32 u4L;
    UINT32 u4R;
    UINT8 u1Char;    

	VDP_OVERSCAN_REGION_T rSrcRegion;

	if(i4Argc != 4)
	{
		Printf("Usage: vdpId HStep VStep\n");
		return 0;
	}
	
    u4VdpId = (UINT32)StrToInt(szArgv[1]);
    u4HStep = (UINT32)StrToInt(szArgv[2]);
    u4VStep = (UINT32)StrToInt(szArgv[3]);
    u4T = 0;
    u4B = 0;
    u4L = 0;
    u4R = 0;
    
    while (1)
    {
        u1Char = SerPollGetChar();

        Printf("Input(%d)\n",u1Char);
        if (u1Char == 27) // escape, end of auto pattern
        {
            Printf("Quit Output Region UT\n");
        	return 0;
        }

        switch(u1Char)
        {        
            case 'u':
                //check underflow
                u1ScpipIsTGUnderflow(u4VdpId);
                break;
            case 'y':
                if(u4T + u4B + u4VStep < VDP_MAX_REGION_HEIGHT)
                {
                    u4T = u4T + u4VStep;
                }
                break;
            case 'Y':
                if(u4T >= u4VStep)
                {
                    u4T = u4T - u4VStep;
                }
                break;
            case 'n':
                if(u4T + u4B + u4VStep < VDP_MAX_REGION_HEIGHT)
                {
                    u4B = u4B + u4VStep;
                }
                break;
            case 'N':
                if(u4B >= u4VStep)
                {
                    u4B = u4B - u4VStep;
                }
                break;
            case 'g':
                if(u4L + u4R + u4HStep < VDP_MAX_REGION_WIDTH)
                {
                    u4L = u4L + u4HStep;
                }
                break;
            case 'G':
                if(u4L >= u4HStep)
                {
                    u4L = u4L - u4HStep;
                }
                break;                
            case 'j':
                if(u4L + u4R + u4HStep < VDP_MAX_REGION_WIDTH)
                {
                    u4R = u4R + u4HStep;
                }
                break;
            case 'J':
                if(u4R >= u4HStep)
                {
                    u4R = u4R - u4HStep;
                }
                break;
            case 'h':
                if( (u4L + u4R + (2 * u4HStep) < VDP_MAX_REGION_WIDTH) &&
                    (u4T + u4B + (2 * u4VStep) < VDP_MAX_REGION_HEIGHT))
                {
                    u4L = u4L + u4HStep;
                    u4R = u4R + u4HStep;
                    u4T = u4T + u4VStep;
                    u4B = u4B + u4VStep;
                }
                break;
            case 'H':
                if(u4T >= u4VStep && u4B >= u4VStep &&
                   u4L >= u4HStep && u4R >= u4HStep)
                {
                    u4L = u4L - u4HStep;
                    u4R = u4R - u4HStep;                    
                    u4T = u4T - u4VStep;
                    u4B = u4B - u4VStep;
                }
                break;                
            case '=':        //for increase HStep
                u4HStep++;
                Printf("HStep(%d) VStep(%d)\n", u4HStep, u4VStep);
                break;
            case '-':        //for decrease HStep
                if(u4HStep > 0)
                {
                    u4HStep--;
                }
                Printf("HStep(%d) VStep(%d)\n", u4HStep, u4VStep);
                break;
            case '0':
                u4VStep++;
                Printf("HStep(%d) VStep(%d)\n", u4HStep, u4VStep);
                break;
            case '9':
                if(u4VStep > 0)
                {
                    u4VStep--;
                }
                Printf("HStep(%d) VStep(%d)\n", u4HStep, u4VStep);
                break;                
        }        

        Printf("Overscan UI(T,B,L,R)=(%d,%d,%d,%d)\n",u4T,u4B,u4L,u4R);

        rSrcRegion.u4Top    = u4T;
        rSrcRegion.u4Bottom = u4B;
        rSrcRegion.u4Left   = u4L;
        rSrcRegion.u4Right  = u4R;
    
        VDP_SetOverScan(u4VdpId, rSrcRegion);
    }
}

static INT32 ScpipOutputRegionUT(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 fromW;
    UINT32 toW;
    UINT32 fromH;
    UINT32 toH;
    UINT32 HStep;
    UINT32 VStep;
    UINT32 vdpID;

    UINT32 x;
    UINT32 y;
    UINT32 width;
    UINT32 height;
       
    UINT8 u1Char;
	UINT8 ucOutFullRegion = 0;
	VDP_REGION_T rOutRegion;

	if(i4Argc != 8)
	{
		Printf("Usage: vdpId fromWidth fromHeight toWidth toHeight HStep VStep\n");
		return 0;
	}

    vdpID = (UINT32)StrToInt(szArgv[1]);
    fromW = (UINT32)StrToInt(szArgv[2]);
    fromH = (UINT32)StrToInt(szArgv[3]);
    toW = (UINT32)StrToInt(szArgv[4]);
    toH = (UINT32)StrToInt(szArgv[5]);
    HStep = (UINT32)StrToInt(szArgv[6]);
    VStep = (UINT32)StrToInt(szArgv[7]);

    x = 0;
    y = 0;
    width = fromW;
    height = fromH;

    while (1)
    {
        u1Char = SerPollGetChar();

        Printf("Input(%d)\n",u1Char);
        if (u1Char == 27) // escape, end of auto pattern
        {
            Printf("Quit Output Region UT\n");
        	return 0;
        }

        switch(u1Char)
        {
            case 'u':
                //check underflow
                u1ScpipIsTGUnderflow(vdpID);
                break;
            case 'g':   // for decrease Width
                if (width > toW)
                {
                    width -= HStep;
                }
                break;
            case 'j':   // for increase Width
                if (width < fromW)
                {
                    width += HStep;
                }
                break;
            case 'y':   // for decrease Height
                if (height > toH)
                {
                    height -= VStep;
                }
                break;
            case 'n':   // for increase Height
                if (height < fromH)
                {
                    height += VStep;
                }
                break;
            case 't': // for decrease Width & Height
                if (width > toW && height > toH)
                {
                    width -= HStep;
                    height -= VStep;
                }     
                break;
            case 'm': // for increase Width & Height
                if (width < fromW && height < fromH)
                {
                    width += HStep;
                    height += VStep;
                }
                break;
            case 'w': // for move up
                if (y >= VStep)
                {
                    y-=VStep;
                }                
                break;
            case 'a': // for move left
                if (x >= HStep)
                {
                    x-= HStep;
                }
                break;
            case 'd': // for move right
                if (x+HStep <= 10000)
                {
                    x+= HStep;
                }
                break;
            case 'x': // for move down
                if (y+VStep <= 10000)
                {
                    y+= VStep;
                }
                break;
            case 'q':
                if (x >= HStep && y >= VStep)
                {
                    x-= HStep;
                    y-= VStep;
                }
                break;
            case 'c':
                if (x+HStep <= 10000 && y+VStep <= 10000)
                {
                    x+= HStep;
                    y+= VStep;
                }
                break;
            case 'e':
                if (x+HStep <= 10000 && y >= VStep)
                {
                    x+= HStep;
                    y-= VStep;                
                }
                break;
            case 'z':
                if (x >= HStep && y+VStep <= 10000)
                {
                    x-= HStep;
                    y+= VStep;                    
                }
                break;
            case '=':        //for add HStep
                HStep++;
                Printf("HStep(%d) VStep(%d)\n", HStep, VStep);
                break;
            case '-':        //for decrease HStep
                if(HStep > 0)
                {
                    HStep--;
                }
                Printf("HStep(%d) VStep(%d)\n", HStep, VStep);
                break;
            case '0':
                VStep++;
                Printf("HStep(%d) VStep(%d)\n", HStep, VStep);
                break;
            case '9':
                if(VStep > 0)
                {
                    VStep--;
                }
                Printf("HStep(%d) VStep(%d)\n", HStep, VStep);
                break;
        }

        Printf("outr UI x=(%d) y(%d) widht=(%d) height=(%d)\n",x,y,width,height);
        
        rOutRegion.u4X = x;
        rOutRegion.u4Y = y;
        rOutRegion.u4Width  = width;
        rOutRegion.u4Height = height;
        VDP_SetOutRegion(vdpID, ucOutFullRegion, rOutRegion);
    }
}

static INT32 _ScpipSetForcedFrontBackDramMode(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4VdpId;  
    UINT32 u4ForcedDramMode;  
    if(i4Argc != 3)	
    {		
        Printf("Usage: vdpId forcedDramMode\n");		
        Printf("[forcedDramMode]\n");		
        Printf("0:Auto\n");		
        Printf("1:Front Dram Mode\n");		
        Printf("2:Back Dram Mode\n");				
        return 0;	
    }

    u4VdpId = (UINT32)StrToInt(szArgv[1]);
    u4ForcedDramMode = (UINT32)StrToInt(szArgv[2]);
    vScpipSetForcedFrontBackDramMode(u4VdpId,u4ForcedDramMode);
    return 0;

}

static INT32 _ScpipSetForcedVScalingMode(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4VdpId;
    UINT32 u4ForcedVScalingMode;
    if(i4Argc != 3)
    {		
        Printf("Usage: vdpId VScalingMode\n");		
        Printf("[VScalingMode]\n");		
        Printf("0:Auto\n");
        Printf("1:FIR Mode\n");
        Printf("2:Partial Sum Mode\n");
        return 0;	
    }

    u4VdpId = (UINT32)StrToInt(szArgv[1]);
    u4ForcedVScalingMode = (UINT32)StrToInt(szArgv[2]);
    vScpipSetForcedVScalingMode((UINT8)u4VdpId,u4ForcedVScalingMode);   
    return 0;
}

static INT32 _ScpipGetForcedVScalingMode(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4VdpId;
    UINT8 vScalingMode;
    if(i4Argc != 2)
    {		
        Printf("Usage:main/sub\n");				
        return 0;	
    }

    u4VdpId = (UINT32)StrToInt(szArgv[1]);
    vScalingMode = vScpipGetForcedVScalingMode((UINT8)u4VdpId);  
    
    Printf("Current Path(%d) Vscaliing mode is :%d  0:Auto 1:FIR 2:Partial Sum\n",u4VdpId,vScalingMode);
    return 0;
}
static INT32 _ScpipSetForcedScalingMode(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4VdpId;
    UINT32 u4ForcedScalingMode;
    if(i4Argc != 3)
    {		
        Printf("Usage: vdpId scalingMode\n");		
        Printf("[scalingMode]\n");		
        Printf("0:Auto\n");
        Printf("1:422 Scaling Mode\n");
        Printf("2:444 Scaling Mode\n");
        return 0;	
    }

    u4VdpId = (UINT32)StrToInt(szArgv[1]);
    u4ForcedScalingMode = (UINT32)StrToInt(szArgv[2]);
    vScpipSetForced422Scaling((UINT8)u4VdpId,u4ForcedScalingMode);   
    return 0;
}
static INT32 _ScpipSetForcedDramWriteFmt(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4VdpId;
    UINT32 u4ForcedDramWriteFmt;

    if(i4Argc != 3)
    {		
        Printf("Usage: vdpId dramWriteFmt\n");		
        Printf("[dramWriteFmt]\n");		
        Printf("0:Auto\n");
        Printf("1:444 format\n");
        Printf("2:422 format\n");
        return 0;	
    }

    u4VdpId = (UINT32)StrToInt(szArgv[1]);
    u4ForcedDramWriteFmt = (UINT32)StrToInt(szArgv[2]);

    if(u4ForcedDramWriteFmt>2)
    {
        Printf("Invalid parameter.\n");
        return 0;
    }
    
    vScpipSetForcedDramWriteFmt((UINT8)u4VdpId, u4ForcedDramWriteFmt);
    return 0;
}


static INT32 _ScpipSetForced444Mode(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4VdpId;
    UINT32 u4Forced444Mode;
    if(i4Argc != 3)
    {		
        Printf("Usage: vdpId forced444Mode\n");		
        Printf("[forced444Mode]\n");		
        Printf("0:Auto\n");
        Printf("1:444 Mode\n");
        Printf("2:422 Mode\n");
        return 0;	
    }

    u4VdpId = (UINT32)StrToInt(szArgv[1]);
    u4Forced444Mode = (UINT32)StrToInt(szArgv[2]);
    vScpipSetForced444Mode(u4VdpId,u4Forced444Mode);
    return 0;
}

static INT32 _ScpipPollingStatus(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4VdpId = 0;
    UINT32 u4Module = 0;
    UINT32 u4Delay = 30;
    
    if(i4Argc != 3 && i4Argc != 4)
    {
        Printf("Usage: vdpid module [delay]\n");
        Printf("0x01:PD\n");
        Printf("0x02:RG\n");
        Printf("0x04:FS\n");
        Printf("0x08:PS\n");
        Printf("0x10:FT\n");
        return 0;
    }

    u4VdpId = (UINT32)StrToInt(szArgv[1]);
    u4Module = (UINT32)StrToInt(szArgv[2]);
    if (i4Argc == 4)
    {
        u4Delay = (UINT32)StrToInt(szArgv[3]);
    }

    vScpipLOGStatus(u4VdpId, u4Module, u4Delay);
    return 0;
}


static INT32 _ScpipScalingCoef(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4Enable = 0;
    
    if(i4Argc != 2)
    {
        Printf("Usage: enable: 1/disable: 0\n");
        return 0;
    }

    u4Enable = (UINT32)StrToInt(szArgv[1]);

    vScpipSetScalingCoef(u4Enable);
    return 0;
}

static INT32 _ScpipSetCRC(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u1Path, u1InOut, u1En;

    if (i4Argc < 4)
    {
        Printf("Usage: <u1Path 0/1> <u1InOut 0/1> <u1En>\n");
        return 0;
    }

    u1Path  = (UINT8)StrToInt(szArgv[1]);
    u1InOut = (UINT8)StrToInt(szArgv[2]);
    u1En    = (UINT8)StrToInt(szArgv[3]);

    vScpipCRCsysEn(u1Path, u1InOut, u1En);

    printf("[SCPIP_CRC] Set path(%d) I/O (%d) enable(%d)\n", u1Path, u1InOut, u1En);
    return 0;
}

static INT32 _ScpipQueryCRC(INT32 i4Argc, const CHAR **szArgv)
{
    Printf("[SCPIP_CRC]H: MI %8X MO %8X SI %8X SO %8X\n", u4ScpipCRCsysHstatus(0, 0), u4ScpipCRCsysHstatus(0, 1), u4ScpipCRCsysHstatus(1, 0), u4ScpipCRCsysHstatus(1, 1));
    Printf("[SCPIP_CRC]V: MI %8X MO %8X SI %8X SO %8X\n", u4ScpipCRCsysVstatus(0, 0), u4ScpipCRCsysVstatus(0, 1), u4ScpipCRCsysVstatus(1, 0), u4ScpipCRCsysVstatus(1, 1));
    return 0;
}

static INT32 _ScpipSetISR(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4Sel, u4En;
    
    if (i4Argc < 3)
    {
        Printf("Usage: <u4Sel 0~15> <0:disable 1:enable>\n");
        return 0;
    }

    u4Sel = (UINT8)StrToInt(szArgv[1]);
    u4En  = (UINT8)StrToInt(szArgv[2]);

    vScpipSetISREn(u4Sel, u4En);
    
    printf("[SCPIP_ISR] Set ISR[%d] enable (%d)\n", u4Sel, u4En);
    return 0;
}

static INT32 _ScpipClrISR(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4Sel = 0;
    
    if(i4Argc != 2)
    {
        Printf("Usage: <u4Sel 0~15>\n");
        return 0;
    }

    u4Sel = (UINT32)StrToInt(szArgv[1]);

    vScpipClearISRstatus(u4Sel);
    return 0;
}

static INT32 _ScpipQueryISR(INT32 i4Argc, const CHAR **szArgv)
{
    vScpipQueryISR();
    return 0;
}

static INT32 _ScpipMid(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4VdpId = 0;
    SCALER_FBM_POOL_T* fbmInfo;
    UINT32 u4LowAddr, u4HighAddr, u4Region;
    MID_AGENT_ID_T eAgentId = MID_AGENT_SCPOS;
    BOOL fgOk;
    
    if(i4Argc != 2)
    {
        Printf("Usage: <u4VdpId 0:main/1:sub>\n");
        return 0;
    }
    u4VdpId = (UINT32)StrToInt(szArgv[1]);

    fbmInfo = getFbmPoolInfo(u4VdpId);
    u4LowAddr  = fbmInfo->u4Addr;
    u4HighAddr = fbmInfo->u4Addr + fbmInfo->u4FbmSize;
    u4Region = 4;

    Printf("Setting to region %u (0x%08x - 0x%08x)\n", u4Region, u4LowAddr, u4HighAddr);
    
    fgOk = MID_SetRegionEx2(u4Region, eAgentId, u4LowAddr, u4HighAddr, 0, 0);   
    Printf("%s\n", fgOk ? "Succeeded" : "Failed");

    return 0;
}


#endif //#ifdef CC_CLI
