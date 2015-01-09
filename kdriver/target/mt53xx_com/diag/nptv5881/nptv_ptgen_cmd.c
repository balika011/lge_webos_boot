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
 * $RCSfile: nptv_dec_cmd.c,v $
 * $Revision: #1 $
 *
 * Description:
 *         This file contains CLI implementation of NPTV Video.
 *---------------------------------------------------------------------------*/
#include "x_os.h"
#include "x_stl_lib.h"
#include "video_def.h"
#ifdef __MW_CLI_DEF__
#include "x_mid_cli.h"
#else
#include "x_drv_cli.h"
#endif

#include "x_assert.h"
#include "x_debug.h"

#include "drv_ycproc.h"
#include "drv_di.h"
#include "drv_scaler.h"
#include "drv_scpos.h"
#include "drv_tcon.h"
#include "vdo_misc.h"
#include "drv_od.h"

#include "drv_scaler_drvif.h"
#ifdef CC_MT5398
#include "drv_mjc.h"
#include "drv_mjc_if.h"
#endif

#ifndef CC_CLI
#define CC_CLI
#endif
#ifdef CC_CLI

/* Functions for Decoder */
static INT32 _i4PatAuto(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4PatDisableAll(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4PatOmux(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4PatPreProc(INT32 i4Argc, const CHAR **szArgv);


static INT32 _i4PatDIOut(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4PatDIIn(INT32 i4Argc, const CHAR **szArgv);

static INT32 _i4PatSharpness(INT32 i4Argc, const CHAR **szArgv);
extern INT32 _ScposSetInputSelfPtnCmd(INT32 i4Argc, const CHAR **szArgv);
extern INT32 _ScposSetInputPtnCmd(INT32 i4Argc, const CHAR **szArgv);
extern INT32 _ScposSetOutputPtnCmd(INT32 i4Argc, const CHAR **szArgv);
extern INT32 _i4YcprocSCEPatGen(INT32 i4Argc, const CHAR **szArgv);
#ifdef CC_MT5398
static INT32 _i4MjcPatGen(INT32 i4Argc, const CHAR **szArgv);
extern void vScpipSetPostScalerPattern(UINT32 u4InOut, UINT32 u4OnOff, UINT32 u4HTotal, UINT32 u4Width, UINT32 u4VTotal, UINT32 u4Height);
#endif

extern INT32 _PreprocEnablePattern(INT32 i4Argc, const CHAR **szArgv);
extern INT32 _i4YcprocSharpnessPatGen(INT32 i4Argc, const CHAR **szArgv);
extern INT32 _PmxPatternCmd(INT32 i4Argc, const CHAR ** szArgv);
extern INT32 _PmxSetCmdOSTGPt(INT32 i4Argc, const CHAR ** szArgv);
extern INT32 _OsdRgnCmdPattern(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _EHCmdPattern(INT32 i4Argc, const CHAR **szArgv);
extern INT32 _MlvdsCmdPattern(INT32 i4Argc, const CHAR ** szArgv);
extern INT32 _ODSetPatternGen(INT32 i4Argc, const CHAR ** szArgv);

extern void vOmuxPatternGenOnOff(UINT8,UINT8);
EXTERN UINT8 SerPollGetChar(void);

EXTERN INT32 _iDiSetOutputPattern(INT32 i4Argc, const CHAR **szArgv);
EXTERN INT32 _iDiSetInputPattern(INT32 i4Argc, const CHAR **szArgv);

#endif /*#ifdef CC_CLI*/
/******************************************************************************
* CLI Command Table
******************************************************************************/

// Table for Decoder command entry
CLI_EXEC_T arPatgenCmdTbl[] = {
#ifdef CC_CLI
    {"Pat Auto Mode"   , "auto"    , _i4PatAuto               , NULL , "Patterns from begin to end"   , CLI_GUEST}      ,
    {"Pat Disable All" , "da"      , _i4PatDisableAll         , NULL , "Diable all pattern"           , CLI_SUPERVISOR}      ,
    {"Pat Omux"        , "o"       , _i4PatOmux               , NULL , "Omux pattern on/off"          , CLI_SUPERVISOR}      ,
    {"Pat Pre-proc"    , "pre"     , _i4PatPreProc            , NULL , "Pre-proc pattern on/off"      , CLI_SUPERVISOR}      ,
    {"Pat DI In"       , "dii"     , _i4PatDIIn               , NULL , "MIB/DI Intput pattern"        , CLI_SUPERVISOR}      ,
    {"Pat DI Out"      , "dio"     , _i4PatDIOut              , NULL , "MIB/DI output pattern"        , CLI_SUPERVISOR}      ,
    {"Pat SC Input"    , "sci"     , _ScposSetInputPtnCmd     , NULL , "scaler input pattern"         , CLI_SUPERVISOR}      ,
    {"Pat SC Output"   , "sco"     , _ScposSetOutputPtnCmd    , NULL , "scaler output pattern"        , CLI_SUPERVISOR}      ,
    {"Pat SC self gen" , "selfi"   , _ScposSetInputSelfPtnCmd , NULL , "scaler input selfgen pattern" , CLI_SUPERVISOR}      ,
    {"Pat Sharpness"   , "sharp"   , _i4PatSharpness          , NULL , "2D sharpness pattern"         , CLI_SUPERVISOR}      ,
    {"Pat SCE"         , "sce"     , _i4YcprocSCEPatGen       , NULL , "SCE(MLC input) pattern"       , CLI_SUPERVISOR}      ,
    #ifdef CC_MT5398
    {"Pat MJC"         , "mjc"     , _i4MjcPatGen             , NULL , "MJC in/out pattern"           , CLI_SUPERVISOR}      ,
    #endif
    {"Pat OSTG Input"  , "ostgin"  , _PmxSetCmdOSTGPt         , NULL , "Output stage input pattern"   , CLI_SUPERVISOR}      ,
    {"Pat OSTG Output" , "ostgout" , _PmxPatternCmd           , NULL , "Output stage output pattern"  , CLI_SUPERVISOR}      ,
    {"Pat OSD"         , "osd"     , _OsdRgnCmdPattern        , NULL , "OSD pattern"                  , CLI_SUPERVISOR}      ,
    {"Error Handling"  , "eh"      , _EHCmdPattern            , NULL , "Error handling pattern gen"   , CLI_SUPERVISOR}      ,
    {"Mlvds"           , "mlvds"   , _MlvdsCmdPattern         , NULL , "MLVDS pattern gen"            , CLI_SUPERVISOR}      ,
    {"OD"              , "od"      , _ODSetPatternGen         , NULL , "OD pattern gen"               , CLI_SUPERVISOR}      ,
#endif
    {NULL              , NULL      , NULL                     , NULL , NULL                           , CLI_SUPERVISOR}
};

#ifdef CC_CLI
/******************************************************************************
* Local Function
******************************************************************************/
static void vPtgenDisableAll(UINT8 u1Path)
{
    vOmuxPatternGenOnOff(u1Path, 0);
    vDrvPreprocEnablePattern(u1Path, 0);
    vDrvDISetInputPattern(u1Path, 0);    
    vDrvDISetPattern(u1Path, 0);
	vScpipSelfInputPatternOnOff(SV_OFF);
    vScpipSetInputPattern(u1Path, VDP_DS_PTN_OFF, 0, 0);    
    vScpipSetOutputPattern(u1Path, VDP_DS_PTN_OFF, 0, 0);  
    vDrvMainSharpPatGen(0);
    vHalSetSCEPat(0,0);
    vDrvSetOSTGInPattern(u1Path, 0);
    vDrvSetOSTGOutPattern(0);
    #ifdef CC_MT5398
    vDrvMJCEnablePatGen(0, 0);
    vDrvMJCEnablePatGen(1, 0);
    vScpipSetPostScalerPattern(0, SCPIP_PTGEN_OFF, 0, 0, 0, 0);
    vScpipSetPostScalerPattern(1, SCPIP_PTGEN_OFF, 0, 0, 0, 0);
    #endif
    vDrvODSetPatternGen(SV_OFF, 4);
	u1DrvTconTP(0);
	vDrvMLVDSPatternGen(0);
	u1DrvPixelReorderTP(0);
}


static INT32 _i4PatDisableAll(INT32 i4Argc, const CHAR **szArgv)
{
	if(i4Argc != 2)
	{
        	Printf("usage :%s  <main:0/ sub:1> \n", szArgv[0]);
        	return 0;
	}
	else
	{
	    vPtgenDisableAll((UINT8)StrToInt(szArgv[1]));
	}

	return 0;
}

typedef enum
{
    PTGEN_NONE,
    PTGEN_OMUX,  
    PTGEN_PRE_PROC,
    PTGEN_MIB_IN,        
    PTGEN_MIB_OUT,    
    PTGEN_SC_IN,   
    PTGEN_SC_OUT,  
    //PTGEN_POST_SHARP,
    //PTGEN_POST_SCE,
    PTGEN_OSTG_IN,
    PTGEN_MJC_IN,
    PTGEN_MJC_OUT,
    PTGEN_POST_SCALER_IN,
    PTGEN_POST_SCALER_OUT,
    PTGEN_OD,
    PTGEN_OSTG_OUT,
    PTGEN_MLVDS,
    PTGEN_EH,
    PTGEN_PIXEL_REORDER,
    PTGEN_MAX
} E_BYP_MODULE;

static CHAR _aszPtGenStr[PTGEN_MAX][30] = 
{
    "Disable All Pattern",
    "OMux Pattern",
    "Pre-proc Pattern",
    "DI(MIB) Input Patttern",    
    "DI(MIB) Output Patttern",
    "Scaler Input Pattern",
    "Scaler Output Pattern",
    //"Post-sharpness Pattern",
    //"Post-SCE Input Pattern",
    "OSTG Input Pattern",
    "MJC Input Pattern",
    "MJC Output Pattern",
    "Post-scaler Input Pattern",
    "Post-scaler Output Pattern",
    "OD Input Pattern",
    "OSTG Output Pattern",
    "Mini-LVDS Pattern",
    "Error Handling Pattern",
    "Pixel Reorder Pattern",
};

static void _PrintPtgenItem(UINT32 u4Path)
{
    UINT32 u4PatIndex;
    UINT8  u1PatChar;

    for (u4PatIndex = 0; u4PatIndex < PTGEN_MAX; u4PatIndex ++)
    {
        if (u4PatIndex >= 10)
        {
            u1PatChar = 55 + u4PatIndex;
        }
        else
        {
            u1PatChar = 48 + u4PatIndex;
        }

        Printf("%c: %s\n", u1PatChar, _aszPtGenStr[u4PatIndex]);
    }

    Printf("Esc: Quit Auto Pattern Check\n");
}


#include "eepdef.h"
#include "api_eep.h"

static INT32 _i4PatAuto(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 u1Char, u1Path, u1PtGenNo, u1OldPtGenNo, u1PtType;

    u1PtGenNo = (UINT8)szArgv[2][0];
    u1OldPtGenNo = 0xff;
    u1PtType = 0;
    
    if ((i4Argc != 2) && (i4Argc != 3)&& (i4Argc != 4))
    {
        Printf("Usage: %s VdoPath [PatNo] EEpSave\n", szArgv[0]);
        Printf("<VdoPath>\n");
        Printf("0: Main\n");
        Printf("1: Sub\n");
        Printf("<PatNo>\n");
        _PrintPtgenItem(0);
        return 1;
    }

    u1Path = StrToInt(szArgv[1]);
    u1Path = (u1Path == 1) ? 1 : 0;

    if (i4Argc == 2) // auto pattern mode
    {
        Printf("Please select pattern:\n");
        _PrintPtgenItem(u1Path);
    }



    while (1)
    {
        if (i4Argc >= 3)
        {
            u1Char = (UINT8)szArgv[2][0];
        }
        else
        {
            u1Char = SerPollGetChar();
        }

        if ((u1Char >= '0') && (u1Char <= '9'))
        {
            u1PtGenNo = u1Char - 48;
        }
        else if ((u1Char >= 'A') && (u1Char <= 'Z'))
        {
            u1PtGenNo = u1Char - 55;
        }
        else if ((u1Char >= 'a') && (u1Char <= 'z'))
        {
            u1PtGenNo = u1Char - 87;
        }
        else if (u1Char == 27) // escape, end of auto pattern
        {
            Printf("Quit Auto Pattern Check\n");
        	return 0;
        }
        else if (u1Char == 10) // ncstool, enter
        {
            continue;
        }
        else
        {
            _PrintPtgenItem(u1Path);
            continue;
        }

        if (i4Argc == 4) // save to eep
        {
            UINT8 bBootCmd, bSaveEEp=0 ;
            bSaveEEp = StrToInt(szArgv[3]);
            if(bSaveEEp)
            {        
                fgApiEepromWriteByte(EEP_NPTV_BOOT_DEBUG_ENABLE, ((u1PtGenNo == 0) ? 0 :SV_BOOT_DEBUG_MAGIC));

                bBootCmd = bApiEepromReadByte(EEP_NPTV_BOOT_DEBUG_CMD);
                if(bBootCmd == 0xff)
                {
                    bBootCmd = 0 ;
                }                
                bBootCmd &= ~(0x7F) ; //Clear bit[6:0]
                bBootCmd |= ((u1Path& 0x1) << 6)  |  (1 << 5) |  (u1PtGenNo & 0x1f);
                fgApiEepromWriteByte(EEP_NPTV_BOOT_DEBUG_CMD, bBootCmd);
                
                Printf("Save boot debug cmd 0x%x\n", bBootCmd);       
            }
        }   

        if (u1PtGenNo < PTGEN_MAX)
        {
            vPtgenDisableAll(u1Path);
            Printf("-------------- PATTERN (%c) %s -----------------\n", u1Char, _aszPtGenStr[u1PtGenNo]);
        }
			
        switch (u1PtGenNo)
        {
        case PTGEN_NONE: // dont' enable any pattern gen
            Printf("Success: DISABLE all pattern gen of Path %d\n", u1Path);
            break;

        case PTGEN_OMUX:
            if (u1OldPtGenNo != u1PtGenNo)
            {
                u1PtType = 3; // default omux patgen type 
            }
            else
            {
                u1PtType = u1PtType%3 + 1; // next omux patgen type
            }

            vOmuxPatternGenOnOff(u1Path, u1PtType);
            Printf("Success: Omux pattern gen of Path %d\n",u1Path);
            Printf("If Omux pattern can make problem disappear ==> DECODER\n");
            break;
                    
        case PTGEN_PRE_PROC:
            if(u1OldPtGenNo != u1PtGenNo)
            {
                u1PtType = 3;  // default pre-proc patgen type 
            }
            else
            {
                u1PtType = (u1PtType + 1) % 7; // next pre-proc patgen type
            }
            vDrvPreprocEnablePattern(u1Path, u1PtType);
            Printf("Success: Pre-Porcess pattern gen of Path %d\n",u1Path);
            Printf("If Pre-Proc pattern can make problem disappear ==> DECODER\n");
            break;

        case PTGEN_MIB_IN:
            {
                if (!(bDrvVideoIsSrcInterlace(u1Path) || (0==u1Path)))
                {
                    Printf("\n(  ____ )(  ___  )(  ____ )\\(  ____ \\");
                    Printf("\n| (    )|| (   ) || (    \\/| (    \\/");
                    Printf("\n| (____)|| (___) || (_____ | (_____ ");
                    Printf("\n|  _____)|  ___  |(_____  )(_____  )");
                    Printf("\n| (      | (   ) |      ) |      ) |");
                    Printf("\n| )      | )   ( |/\\____) |/\\____) |");
                    Printf("\n|/       |/     \\|\\_______)\\_______)");                                                 
                    Printf("\nWarning: Please DON'T use PScan pattern when Scaler dram mode + progressive timing\n");
                }
                else
                {
                    Printf("Success: MIB(DI) PTGEN_MIB_IN of Path %d\n",u1Path);
                    Printf("If MIB In pattern is good but Pre-proc pattern is fail,\n");
                    printf("Please contact video integrator for further CR dispatch\n");
                }
                vDrvDISetInputPattern(u1Path, 1);
            }
            break;
            
        case PTGEN_MIB_OUT:
            {
                if (!(bDrvVideoIsSrcInterlace(u1Path) || (0==u1Path)))
                {
                    Printf("\n(  ____ )(  ___  )(  ____ )\\(  ____ \\");
                    Printf("\n| (    )|| (   ) || (    \\/| (    \\/");
                    Printf("\n| (____)|| (___) || (_____ | (_____ ");
                    Printf("\n|  _____)|  ___  |(_____  )(_____  )");
                    Printf("\n| (      | (   ) |      ) |      ) |");
                    Printf("\n| )      | )   ( |/\\____) |/\\____) |");
                    Printf("\n|/       |/     \\|\\_______)\\_______)");                                                 
                    Printf("\nWarning: Please DON'T use PScan pattern when Scaler dram mode + progressive timing\n");
                }
                else
                {
                    Printf("Success: MIB(DI) PTGEN_MIB_OUT of Path %d\n",u1Path);
                    Printf("If PTGEN_MIB_OUT is good but iPTGEN_MIB_IN is fail\n");
                    printf("please attach the CR log and issue the CR to MIB owner\n");
                }
                vDrvDISetPattern(u1Path, 1);
            }
            break;
                    
        case PTGEN_SC_IN:
            {   
#if 0
                UINT8 u1Mode = getScalerMode(u1Path);   
                if (u1Mode == VDP_SCPOS_DISPMODE_OFF)
                {
                    Printf("(  ____ )(  ___  )(  ____ \(  ____ \\");
                    Printf("| (    )|| (   ) || (    \\/| (    \\/");
                    Printf("| (____)|| (___) || (_____ | (_____ ");
                    Printf("|  _____)|  ___  |(_____  )(_____  )");
                    Printf("| (      | (   ) |      ) |      ) |");
                    Printf("| )      | )   ( |/\\____) |/\\____) |");
                    Printf("|/       |/     \\|\\_______)\\_______)");
                    Printf("Warning: Please DON'T use Down Scaler pattern when Display mode\n");
                }
                else
#endif 
                {
                    vScpipSetInputPattern(u1Path, VDP_DS_PTN_AUTO, 0, 0);    
                    Printf("Success: PTGEN_SC_IN of Path %d\n",u1Path);
                    Printf("If PTGEN_SC_IN is good but PTGEN_MIB_OUT is fail ==> MIB\n");
                }
            }
            break;
                    
        case PTGEN_SC_OUT:
            vScpipSetOutputPattern(u1Path, VDP_DS_PTN_AUTO, 0, 0);  
            Printf("Success: PTGEN_SC_OUT of Path %d\n",u1Path);
            Printf("If PTGEN_SC_OUT is good but PTGEN_SC_IN is fail ==> Scaler\n");
            break;
#if 0                    
        case PTGEN_POST_SHARP:
            if (u1Path == 0) //main only
            {
                vDrvMainSharpPatGen(1);
                Printf("Success: Sharpness pattern gen (Only in main path)\n");
                Printf("If Sharpness pattern is good but Up-Scaler pattern is fail  ==>  Scaler\n");
            }
            else
            {
                Printf("\n(  ____ )(  ___  )(  ____ )\\(  ____ \\");
                Printf("\n| (    )|| (   ) || (    \\/| (    \\/");
                Printf("\n| (____)|| (___) || (_____ | (_____ ");
                Printf("\n|  _____)|  ___  |(_____  )(_____  )");
                Printf("\n| (      | (   ) |      ) |      ) |");
                Printf("\n| )      | )   ( |/\\____) |/\\____) |");
                Printf("\n|/       |/     \\|\\_______)\\_______)");            
                Printf("\nWarning : \"No\" Sharpness pattern gen in sub path\n");
            }
            break;
                    
        case PTGEN_POST_SCE:
            if (u1Path == 0) //main only
            {
                vHalSetSCEPat(1,512);
                Printf("Success: SCE(MLC) pattern gen (Only in main path)\n");
                Printf("If SCE pattern is good but Sharpness pattern is fail ==> Post-Proc(PE1)\n");
            }
            else
            {
                Printf("\n(  ____ )(  ___  )(  ____ )\\(  ____ \\");
                Printf("\n| (    )|| (   ) || (    \\/| (    \\/");
                Printf("\n| (____)|| (___) || (_____ | (_____ ");
                Printf("\n|  _____)|  ___  |(_____  )(_____  )");
                Printf("\n| (      | (   ) |      ) |      ) |");
                Printf("\n| )      | )   ( |/\\____) |/\\____) |");
                Printf("\n|/       |/     \\|\\_______)\\_______)");            
                Printf("\nWarning : \"No\" SCE(MLC) pattern gen in sub path\n");
            }
            break;
#endif                    
        case PTGEN_OSTG_IN:
            vDrvSetOSTGInPattern(u1Path, 1);
            Printf("Success: PTGEN_OSTG_IN pattern gen of Path %d\n",u1Path);
            Printf("If PTGEN_OSTG_IN is good but PTGEN_SC_OUT is fail ==>PQ Owner\n");
            break;

#ifdef CC_MT5398
        case PTGEN_MJC_IN:
        	if (u1DrvMJCIsOnOff() == SV_FALSE)
    		{
            	Printf("Warning :MJC is bypassed. Can't display pattern!\n");
    		}
        	else
        	{
        		Printf("Success: PTGEN_MJC_IN pattern gen \n");
                Printf("If PTGEN_MJC_IN is good but PTGEN_OSTG_IN fail ==>Scaler Owner\n");                
		       	vDrvMJCEnablePatGen(0, 1);
        	}
        	break;

        case PTGEN_MJC_OUT:
        	if (u1DrvMJCIsOnOff() == SV_FALSE)
			{
	        	Printf("Warning :MJC is bypassed. Can't display pattern!\n");
			}
        	else
        	{
        		Printf("Success: PTGEN_MJC_OUT pattern gen\n");
                Printf("If PTGEN_MJC_OUT is good but PTGEN_MJC_IN fail ==>MJC Owner\n");                                
		       	vDrvMJCEnablePatGen(1, 1);
        	}
        	break;

        case PTGEN_POST_SCALER_IN:
            vScpipSetPostScalerPattern(0, SCPIP_PTGEN_AUTO, 0, 0, 0, 0);
            Printf("Success: PTGEN_POST_SCALER_IN pattern gen\n");
            Printf("If PTGEN_POST_SCALER_IN is good but PTGEN_MJC_OUT fail ==>MJC Owner\n");                                            
            break;
        case PTGEN_POST_SCALER_OUT:
            vScpipSetPostScalerPattern(1, SCPIP_PTGEN_AUTO, 0, 0, 0, 0);
            Printf("Success: PTGEN_POST_SCALER_OUT pattern gen\n");
            Printf("If PTGEN_POST_SCALER_OUT is good but PTGEN_POST_SCALER_IN fail ==>Scaler Owner\n");                                                        
            break;
#else
        case PTGEN_MJC_IN:
        case PTGEN_MJC_OUT:
        case PTGEN_POST_SCALER_IN:
        case PTGEN_POST_SCALER_OUT:
            Printf("\n(  ____ )(  ___  )(  ____ )\\(  ____ \\");
            Printf("| (    )|| (   ) || (    \\/| (    \\/");
            Printf("| (____)|| (___) || (_____ | (_____ ");
            Printf("|  _____)|  ___  |(_____  )(_____  )");
            Printf("| (      | (   ) |      ) |      ) |");
            Printf("| )      | )   ( |/\\____) |/\\____) |");
            Printf("|/       |/     \\|\\_______)\\_______)");
            Printf("This Chip don't have this module, please ignore this pattern gen\n");
            break;
#endif

        case PTGEN_OD:
            vDrvODSetPatternGen(SV_ON, 4);
            break;

        case PTGEN_OSTG_OUT:
            vDrvSetOSTGOutPattern(1);
            Printf("Success: Output Stage Output pattern gen of Path %d\n",u1Path);
            Printf("If Output stage Output pattern is good but Output stage Input pattern is fail ==> Output Stage(PE1)\n");
            break;

        case PTGEN_MLVDS: // Panel Interface Pattern
            vDrvMLVDSPatternGen(1);
            break;

        case PTGEN_EH:
            u1DrvTconTP(7);
            break;
			
		case PTGEN_PIXEL_REORDER:
			u1DrvPixelReorderTP(1);
			break;

        default:
            _PrintPtgenItem(u1Path);
            break;
        }

        if (i4Argc >= 3) // single pattern mode
        {
            return 0;
        }
        else
        {
            u1OldPtGenNo = u1PtGenNo;
        }
    }
}

#ifdef CC_MT5398
static INT32 _i4MjcPatGen(INT32 i4Argc, const CHAR **szArgv)
{
	if (i4Argc != 3)
	{
        printf("Usage: %s <input:0/output:1> <on:1/off:0>\n", szArgv[0]);
        return 1;
	}

	if (u1DrvMJCIsOnOff() == SV_FALSE)
	{
        printf("MJC is bypassed. Can't display pattern!\n");
        return 1;
	}

    vDrvMJCEnablePatGen(StrToInt(szArgv[1]), StrToInt(szArgv[2]));
    return 0;
}
#endif

static INT32 _i4PatOmux(INT32 i4Argc, const CHAR **szArgv)
{
	if(i4Argc != 3)
	{
		Printf("usage :%s  <main:0/ sub:1> <0:off/ramp:1/full screen:2/all:3>\n", szArgv[0] );
	}
	else
	{
	    vOmuxPatternGenOnOff((UINT16)StrToInt(szArgv[1]),(UINT16)StrToInt(szArgv[2]));
	}

	return 0;
}


static INT32 _EHCmdPattern(INT32 i4Argc, const CHAR **szArgv)
{
	UINT8 u1arg1;
	
	if (i4Argc != 2)
	{
		Printf("Usage: %s \n", szArgv[0]);

		Printf("[0] TCON_TP_OFF\n");
		Printf("[1] TCON_TP_R\n");
 	    Printf("[2] TCON_TP_G\n");
		Printf("[3] TCON_TP_B\n");
		Printf("[4] TCON_TP_BLACK\n");
		Printf("[5] TCON_TP_WHITE\n");
		Printf("[6] TCON_TP_COLORRAMP\n");
		Printf("[7] TCON_TP_MOVING\n");
		Printf("[8] TCON_TP_AUTO\n");

		return 0;
	}

	u1arg1 = (UINT8)StrToInt(szArgv[1]);

	u1DrvTconTP(u1arg1);

	return 0;
}

INT32 _MlvdsCmdPattern(INT32 i4Argc, const CHAR **szArgv)
{
	if (i4Argc != 2)
	{
        Printf("Usage: %s  OnOff\n", szArgv[0]);
        Printf("<OnOff>\n");
        Printf("0: OFF\n");
        Printf("1: ON\n");
        return 1;
	}

    vDrvMLVDSPatternGen(StrToInt(szArgv[1]));        
    return 0;
}

INT32 _ODSetPatternGen(INT32 i4Argc, const CHAR **szArgv)
{
	if (i4Argc != 3)
	{
        Printf("usage :%s  <Off:0/ O:1> <type : 0~4 >\n", szArgv[0] );
        return 1;
	}

    vDrvODSetPatternGen(StrToInt(szArgv[1]),StrToInt(szArgv[2]));        
    return 0;
}


static INT32 _i4PatPreProc(INT32 i4Argc, const CHAR **szArgv)
{
	return _PreprocEnablePattern(i4Argc, szArgv);
}


static INT32 _i4PatDIIn(INT32 i4Argc, const CHAR **szArgv)
{
	return _iDiSetInputPattern(i4Argc, szArgv);
}

static INT32 _i4PatDIOut(INT32 i4Argc, const CHAR **szArgv)
{
	return _iDiSetOutputPattern(i4Argc, szArgv);
}

static INT32 _i4PatSharpness(INT32 i4Argc, const CHAR **szArgv)
{
	return _i4YcprocSharpnessPatGen(i4Argc, szArgv);
}
#endif //#ifdef CC_CLI

