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
 * $RCSfile: nptv_cmd.c,v $
 * $Revision: #1 $
 *
 * Description:
 *         This file contains CLI implementation of NPTV Video.
 *---------------------------------------------------------------------------*/
#include "drv_scaler_drvif.h"
     
     
#include "x_os.h"
#include "x_stl_lib.h"
     
#include "x_assert.h"
#include "x_pinmux.h"
#include "x_debug.h"
#include "x_drv_cli.h"

#include "video_def.h"    
#include "drv_mon_out.h"

#include "drv_tve.h"

//#include "drv_mon_out_regen.h"
//#include "drv_mon_out_predown.h"
     
#ifndef CC_CLI
#define CC_CLI
#endif
     static INT32 MonOutSetRegen(INT32 i4Argc, const CHAR **szArgv);
     static INT32 MonOutCfgQurey(INT32 i4Argc, const CHAR **szArgv);


//extern  fuction--
EXTERN void vMonOutSetMuxSelIn(E_MON_OUT_IN_SELECT MonOutMuxIn);
EXTERN void vVRMUpdateVdpInforForMonOut( UINT8 u1MonOutMuxIn );
EXTERN void vMonOutUpdatePreDownParameter(void);
EXTERN void vMonOutUpdateDramParameter( void );
EXTERN void vVRMMonOutUpdateTTTResoluton( void );




void InitialMonOutRegen( UINT32 x_offset, UINT32 y_offset, UINT32 x_size , UINT32 y_size );

//=====================================================
//mt5399 monitor out  
//
//=====================================================
//----start-----
EXTERN void vVRMSetMonOutEventFlg( UINT32 u4MonOutFlag );
EXTERN void vVRMUpdateMonOutRegenResolution( void );
EXTERN void vMonOutUpdateRegen( void ); 
EXTERN void vVRMMonOutUpdatePredownResolution(void);
EXTERN void vMonOutUpdateTveTg( void );

static INT32 MonOutSetRegen(INT32 i4Argc, const CHAR **szArgv);
static INT32 MonOutCfgQurey(INT32 i4Argc, const CHAR **szArgv);
static INT32 MonOutSetMuxSelIn( INT32 i4Argc, const CHAR **szArgv);
static INT32 MonOutSetMuxOSDSel( INT32 i4Argc, const CHAR **szArgv);
static INT32 MonOutSetOutput( INT32 i4Argc, const CHAR **szArgv);
static INT32 MonOutSetOutputRes( INT32 i4Argc, const CHAR **szArgv);
//static INT32 MonOutSetCmdOverScan (INT32 i4Argc, const CHAR ** szArgv);
static INT32 MonOutSetDotByDot( INT32 i4Argc, const CHAR **szArgv);
static INT32 MonOutTimeGen( INT32 i4Argc, const CHAR **szArgv);
static INT32 MonOutInitial( INT32 i4Argc, const CHAR **szArgv);


EXTERN  MonOutPicInfo   _rMonitorOutInfo;
EXTERN  MonOutPicInfo   _vrm_mon_out_info;

// Table for scpos command entry
CLI_EXEC_T arMonOutCmdTbl[] = { 
#ifdef CC_CLI
	{"MonOut initial","i", MonOutInitial, NULL, "MonOut query", CLI_SUPERVISOR},
	{"MonOut qurey", "q", MonOutCfgQurey, NULL, "MonOut query", CLI_GUEST},
	{"Set MuxSelInput", "in", MonOutSetMuxSelIn, NULL, "Set monitor out input source", CLI_GUEST},
	{"Set OSDSelectt", "osdin", MonOutSetMuxOSDSel, NULL, "Set monitor out OSD select", CLI_GUEST},
	{"Set Monoutput", "out", MonOutSetOutput, NULL, "Set monitor out output source", CLI_GUEST},
	{"Set output w/h","outres", MonOutSetOutputRes, NULL, "Set monitor out output resolution", CLI_GUEST},
	//{"Set overscan", "overscan", MonOutSetCmdOverScan, NULL, "Set overscan", CLI_SUPERVISOR},
	{"Set dot by dot", "121", MonOutSetDotByDot, NULL, "Set dot by dot output", CLI_SUPERVISOR},	
	{"Set regen", "regen", MonOutSetRegen, NULL, "Set overscan", CLI_SUPERVISOR},
	{"set Mon TimeGen","tg", MonOutTimeGen, NULL, "Monitorout time generator", CLI_SUPERVISOR},
#endif
	{NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};

//----end-----


//=====================================================
//mt5399 monitor out  
//
//=====================================================
static INT32 MonOutInitial( INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 Mon_Out_In_Select; 
    
    if (i4Argc != 2 )
    {       
       Printf("Usage: %s  [mux_in] \n", szArgv[0]);
       Printf("MON_OUT_MIXER         =0\n");
       Printf("MON_OUT_MAIN_FSC_OUT  =1\n");
       Printf("MON_OUT_SUB_FSC_OUT   =2\n"); 
       Printf("MON_OUT_MAIN_MIB_OUT  =3\n");
       Printf("MON_OUT_SUB_MIB_OUT   =4\n");
       Printf("MON_OUT_MAIN_PREDOWN  =5\n");
       Printf("MON_OUT_SUB_PREDOWN   =6\n");   
       Printf("MON_OUT_PANEL         =7\n");
       Printf("FSC_SUB_TO_TVE        =8\n");
       return 0;
    }

    Mon_Out_In_Select=i4StrToInt(szArgv[1]);
    
    Printf("mon_out_mux_in = %d\n",Mon_Out_In_Select );
    //Printf("Muxin =>[0: Mixer, 1:FSC_main,2: FSC_SUB,3:Mib_main 4:Mib_sub, 5:pds_main,6:pds_sub,7:Panel_oout] to mon_out , 8: FSC_sub to TVE\n"); 
	Mon_Out_In_Select = i4StrToInt(szArgv[1]);
	vMonOutSetMuxSelIn((E_MON_OUT_IN_SELECT)Mon_Out_In_Select);
	vMonOutSetEnable(SV_TRUE);
	return 0;
    
}

void InitialMonOutRegen( UINT32 x_offset, UINT32 y_offset, UINT32 x_size , UINT32 y_size )
{
   MonOutPicInfo* pMonOutInfo;
   pMonOutInfo = getMonOutInfo();

   pMonOutInfo->rRegenScan.u4X = x_offset;  //x offset
   pMonOutInfo->rRegenScan.u4Y = y_offset;  //y offset
   pMonOutInfo->rRegenScan.u4Width = x_size;   //x size
   pMonOutInfo->rRegenScan.u4Height = y_size; //y_size
}

static INT32 MonOutSetMuxSelIn( INT32 i4Argc, const CHAR **szArgv)
{
	UINT8 u1MuxSelIn;
	if (i4Argc != 2 )
	{       
	   Printf("Usage: %s  MonOutMuxSelIn \n", szArgv[0]);
       Printf("MON_OUT_MIXER         =0\n");
       Printf("MON_OUT_MAIN_FSC_OUT  =1\n");
       Printf("MON_OUT_SUB_FSC_OUT   =2\n"); 
       Printf("MON_OUT_MAIN_MIB_OUT  =3\n");
       Printf("MON_OUT_SUB_MIB_OUT   =4\n");
       Printf("MON_OUT_MAIN_PREDOWN  =5\n");
       Printf("MON_OUT_SUB_PREDOWN   =6\n");   
       Printf("MON_OUT_PANEL         =7\n");
       Printf("FSC_SUB_TO_TVE        =8\n");
	   return 0;
	}

	u1MuxSelIn = i4StrToInt(szArgv[1]);
    Printf("MonOutSetMuxSelIn = %d\n",u1MuxSelIn );
    
	vMonOutSetMuxSelIn((E_MON_OUT_IN_SELECT)u1MuxSelIn);
	return 0;
}

static INT32 MonOutSetMuxOSDSel( INT32 i4Argc, const CHAR **szArgv)
{
	UINT8 u1MuxOSDSelIn;
	if (i4Argc != 2 )
	{       
	   Printf("Usage: %s  MonOutSetMuxOSDSel \n", szArgv[0]);
       Printf("E_MON_OUT_WITH_ALL_OSD  =0\n");
       Printf("E_MON_OUT_WITH_ONE_OSD  =1\n");
       Printf("E_MON_OUT_WITH_NO_OSD   =2\n"); 
	   return 0;
	}

	u1MuxOSDSelIn = i4StrToInt(szArgv[1]);
    Printf("MonOutSetMuxOSDSel = %d\n",u1MuxOSDSelIn );
    
	vMonOutSetOSDSelIn((E_MON_OUT_IN_SELECT_2)u1MuxOSDSelIn);
	return 0;
}

static INT32 MonOutSetOutput( INT32 i4Argc, const CHAR **szArgv)
{
	UINT8 u1Output;
	if (i4Argc < 2 )
	{       
		Printf("Usage: %s  output(0:tve  1:venc ) \n", szArgv[0]);
		return 0;
	}

	u1Output = i4StrToInt(szArgv[1]);
	vMonOutSetOutput((E_MON_OUT_OUTPUT_SELECT)u1Output);
	return 0;
}


static INT32 MonOutSetOutputRes( INT32 i4Argc, const CHAR **szArgv)
{
	UINT16 u1OutputWidth;
	UINT16 u1OutputHeight;
	if (i4Argc < 3 )
	{       
		Printf("Usage: %s  OutputWidth OutputHeight   \n", szArgv[0]);
		return 0;
	}

	u1OutputWidth  = i4StrToInt(szArgv[1]);
	u1OutputHeight = i4StrToInt(szArgv[2]);
	vMonOutSetOutputRes(u1OutputWidth,u1OutputHeight);
	return 0;
}

static INT32 MonOutSetRegen(INT32 i4Argc, const CHAR **szArgv)
{		
    VDP_DISP_REGION_T rMonRegen;
	if(i4Argc < 5)
	{
		Printf("Usage: x y width height\n");
		return 0;
	}

	rMonRegen.u4X      = (UINT32)StrToInt(szArgv[1]);
	rMonRegen.u4Y      = (UINT32)StrToInt(szArgv[2]);
	rMonRegen.u4Width  = (UINT32)StrToInt(szArgv[3]);
	rMonRegen.u4Height = (UINT32)StrToInt(szArgv[4]);
	Printf("set regen (%u,%u,%u,%u)\n", rMonRegen.u4X, rMonRegen.u4Y,rMonRegen.u4Width, rMonRegen.u4Height );

	vMonOutSetRegen(rMonRegen);
	return 0;
}

static INT32 MonOutCfgQurey(INT32 i4Argc, const CHAR **szArgv)
{
    UNUSED(i4Argc);
    UNUSED(szArgv);
    vMonOutModuleCfg(); 
    
    return 0;      
}

static INT32 MonOutTimeGen( INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 u1TveTimeGen;

    if (i4Argc < 2 )
    {       
        Printf("Usage: %s  0:NTSC/1:PAL \n", szArgv[0]);
        return 0;
    }
    u1TveTimeGen = i4StrToInt(szArgv[1]);

    if ( u1TveTimeGen < 2 )
    {
		vMonOutSetTveFormat(u1TveTimeGen);
    }
    else
    {
        Printf("Usage: %s  0:NTSC/1:PAL \n", szArgv[0]);
    }
    return 0;
}

static INT32 MonOutSetDotByDot( INT32 i4Argc, const CHAR **szArgv)
{
	BOOL bEnable = 0;

    if (i4Argc != 2 )
    {       
        Printf("Usage: %s  0:disale/1:Enable \n", szArgv[0]);
        return 0;
    }

	bEnable = (BOOL)i4StrToInt(szArgv[1]);

	vMonOutSetDotByDotOutput(bEnable);

	return 0;
	
}


