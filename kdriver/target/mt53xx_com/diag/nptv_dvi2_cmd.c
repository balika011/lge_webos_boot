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
 * $RCSfile: nptv_dvi_cmd.c,v $
 * $Revision: #1 $
 *
 * Description:
 *         This file contains CLI implementation of NPTV Video.
 *---------------------------------------------------------------------------*/

#include "x_os.h"
#include "x_stl_lib.h"
#include "video_def.h"
#include "dvi_debug.h"

#ifdef __MW_CLI_DEF__
#include "x_mid_cli.h"
#else
#include "x_drv_cli.h"
#endif

#include "x_assert.h"
#include "x_pinmux.h"
#include "x_debug.h"

#include "sv_const.h"
#include "drv_dvi2.h"

#ifndef CC_CLI
#define CC_CLI
#endif
extern void vDvi2SetModeDone(UINT8 u1Type);
extern void vDvi2SetModeChange(UINT8 u1Type);

//#ifdef CC_CLI
/* Functions for DVI */
static INT32 _u1vDviConnect(INT32 i4Argc, const CHAR **szArgv);
static INT32 _u1vDviInitial(INT32 i4Argc, const CHAR **szArgv);
static INT32 _u1vDviSetModeChange(INT32 i4Argc, const CHAR **szArgv);
static INT32 _u1vDviSetModeDone(INT32 i4Argc, const CHAR **szArgv);
static INT32 _u1vCCIRGetStatus(INT32 i4Argc, const CHAR **szArgv);
static INT32 _u1vCCIRDebugMessage(INT32 i4Argc, const CHAR **szArgv);
static INT32 _u1vCCIRSetAvStart(INT32 i4Argc, const CHAR **szArgv);
static INT32 _u1wCCIRGetAvStart(INT32 i4Argc, const CHAR **szArgv);
static INT32 _u1vCCIRSetAvWidth(INT32 i4Argc, const CHAR **szArgv);
static INT32 _u1wCCIRGetAvWidth(INT32 i4Argc, const CHAR **szArgv);
static INT32 _u1vCCIRSetVsStart(INT32 i4Argc, const CHAR **szArgv);
static INT32 _u1wCCIRGetVsStart(INT32 i4Argc, const CHAR **szArgv);
static INT32 _u1vCCIRSetVsWidth(INT32 i4Argc, const CHAR **szArgv);
static INT32 _u1wCCIRGetVsWidth(INT32 i4Argc, const CHAR **szArgv);

//#endif /*#ifdef CC_CLI*/
/******************************************************************************
* CLI Command Table
******************************************************************************/
CLIMOD_DEBUG_FUNCTIONS(DVI)

// Table for DVI command entry
CLI_EXEC_T arDVI2CmdTbl[] = {
	CLIMOD_DEBUG_CLIENTRY(DVI),

  // #ifdef CC_CLI
  {"connect", "c", _u1vDviConnect, NULL, "DVI connect", CLI_SUPERVISOR},
  {"initial", "i", _u1vDviInitial, NULL, "DVI initial", CLI_SUPERVISOR},
  {"mode change", "mcg", _u1vDviSetModeChange, NULL, "DVI mode change", CLI_SUPERVISOR},
  {"mode done", "mdn", _u1vDviSetModeDone, NULL, "DVI mode done", CLI_SUPERVISOR},
  {"get status", "gs", _u1vCCIRGetStatus, NULL, "CCIR get status", CLI_SUPERVISOR},
  {"debug message", "dmsg", _u1vCCIRDebugMessage, NULL, "CCIR get status", CLI_SUPERVISOR},
  {"set av start", "sas", _u1vCCIRSetAvStart, NULL, "CCIR set av start", CLI_SUPERVISOR},
  {"get av start", "gas", _u1wCCIRGetAvStart, NULL, "CCIR get av start", CLI_SUPERVISOR},
  {"set av width", "saw", _u1vCCIRSetAvWidth, NULL, "CCIR set av width", CLI_SUPERVISOR},
  {"get av width", "gaw", _u1wCCIRGetAvWidth, NULL, "CCIR get av width", CLI_SUPERVISOR},
  {"set vs start", "svs", _u1vCCIRSetVsStart, NULL, "CCIR set vs start", CLI_SUPERVISOR},
  {"get vs start", "gvs", _u1wCCIRGetVsStart, NULL, "CCIR get vs start", CLI_SUPERVISOR},
  {"set vs width", "svw", _u1vCCIRSetVsWidth, NULL, "CCIR set vs width", CLI_SUPERVISOR},
  {"get vs width", "gvw", _u1wCCIRGetVsWidth, NULL, "CCIR get vs width", CLI_SUPERVISOR},
  //#endif
  {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};

CLI_MAIN_COMMAND_ITEM(Dvi)
{
    "dvi", NULL, NULL, arDVI2CmdTbl, "DVI test", CLI_GUEST
};

//#ifdef CC_CLI
/******************************************************************************
* Local Function
******************************************************************************/

static INT32 _u1vDviConnect(INT32 i4Argc, const CHAR **szArgv)
{
	if (i4Argc != 2)
	{
		Printf("Usage: connect on(1)/off(0)\n");
		return 0;
	}

	if ((UINT8)StrToInt(szArgv[1]) == 1)
	{
		vDvi2Connect(SV_VP_MAIN, SV_ON);
	}
	else
	{
		vDvi2Connect(SV_VP_PIP, SV_OFF);
	}

	return 0;
}

static INT32 _u1vDviInitial(INT32 i4Argc, const CHAR **szArgv)
{
	//vDvi2Initial();

	return 0;
}

static INT32 _u1vDviSetModeChange(INT32 i4Argc, const CHAR **szArgv)
{
	if (i4Argc <= 2)
	{
		LOG(0, "Usage: %s type(main/sub)\n", szArgv[0]);
		return 0;
	}
	//vDvi2SetModeChange((UINT16)StrToInt(szArgv[2]));
	return 0;
}
static INT32 _u1vDviSetModeDone(INT32 i4Argc, const CHAR **szArgv)
{
	if (i4Argc <= 2)
	{
		LOG(0, "Usage: %s type(main/sub)\n", szArgv[0]);
		return 0;
	}
	//vDvi2SetModeDone((UINT16)StrToInt(szArgv[2]));
	return 0;
}

static INT32 _u1vCCIRGetStatus(INT32 i4Argc, const CHAR **szArgv)
{
	if (i4Argc != 2){
	LOG(0, "Usage: %s type(main/sub)\n", szArgv[0]);
	return 0;
	}
	vCCIR2GetStatus((UINT16)StrToInt(szArgv[1]));
	return 0;
}

static INT32 _u1vCCIRDebugMessage(INT32 i4Argc, const CHAR **szArgv)
{
	if (i4Argc != 2){
	LOG(0, "Usage: %s type(main/sub)\n", szArgv[0]);
	return 0;
	}
	vDvi2GetDebugMessage((UINT16)StrToInt(szArgv[1]));
	return 0;
}


static INT32 _u1vCCIRSetAvStart(INT32 i4Argc, const CHAR **szArgv)
{
	if (i4Argc != 3){
		LOG(0, "Usage: %s type(main/sub) value\n", szArgv[0]);
		return 0;
	}
	if((UINT16)StrToInt(szArgv[1]) == 1)
	{
		vCCIR2SetAvStart((UINT16)StrToInt(szArgv[2]));
	}
	else
	{
		vCCIR2SetNewAvStart((UINT16)StrToInt(szArgv[2]));
	}
	
	return 0;
}

static INT32 _u1wCCIRGetAvStart(INT32 i4Argc,const CHAR **szArgv)
{
	INT16 data;

	if (i4Argc != 2){
		LOG(0, "Usage: %s type(main/sub)\n", szArgv[0]);
		return 0;
	}

	if((UINT16)StrToInt(szArgv[1]) == 1)
	{
		data = wCCIR2GetAvStart();
	}
	else
	{
		data = wCCIR2GetNewAvStart();
	}
	
	LOG(0, "The AvStart value is: %d\n", data);

	return 0;
}

static INT32 _u1vCCIRSetAvWidth(INT32 i4Argc, const CHAR **szArgv)
{
	if (i4Argc != 3){
		LOG(0, "Usage: %s type(main/sub) value\n", szArgv[0]);
		return 0;
	}
	
	if((UINT16)StrToInt(szArgv[1]) == 1)
	{
		vCCIR2SetAvWidth((UINT16)StrToInt(szArgv[2]));
	}
	else
	{
		vCCIR2SetNewAvWidth((UINT16)StrToInt(szArgv[2]));
	}

	return 0;
}

static INT32 _u1wCCIRGetAvWidth(INT32 i4Argc, const CHAR **szArgv)
{
	INT16 data;

	if (i4Argc != 2){
		LOG(0, "Usage: %s type(main/sub)\n", szArgv[0]);
		return 0;
	}

	if((UINT16)StrToInt(szArgv[1]) == 1)
	{
		data = wCCIR2GetAvWidth();
	}
	else
	{
		data = wCCIR2GetNewAvWidth();
	}
	
	LOG(0, "The AvWIDTH value is: %d\n", data);

	return 0;
}

static INT32 _u1vCCIRSetVsStart(INT32 i4Argc, const CHAR **szArgv)
{
	if (i4Argc != 3){
		LOG(0, "Usage: %s type(main/sub) value\n", szArgv[0]);
		return 0;
	}
	
	if((UINT16)StrToInt(szArgv[1]) == 1)
	{
		//vCCIRSetAvWidth((UINT16)StrToInt(szArgv[2]));
		vCCIR2SetVsStart((UINT16)StrToInt(szArgv[2]));
	}
	else
	{
		vCCIR2SetNewVsStart((UINT16)StrToInt(szArgv[2]));
	}


	return 0;
}

static INT32 _u1wCCIRGetVsStart(INT32 i4Argc, const CHAR **szArgv)
{
	INT16 data;
	
	if (i4Argc != 2){
	LOG(0, "Usage: %s type(main/sub)\n", szArgv[0]);
	return 0;
	}

	if((UINT16)StrToInt(szArgv[1]) == 1)
	{
		data = wCCIR2GetVsStart();
	}
	else
	{
		data = wCCIR2GetNewVsStart();
	}
	
	LOG(0, "The CCIR_VS_Start is: %d\n", data);

	return 0;
}

static INT32 _u1vCCIRSetVsWidth(INT32 i4Argc, const CHAR **szArgv)
{
	if (i4Argc != 3){
		LOG(0, "Usage: %s type(main/sub) value\n", szArgv[0]);
		return 0;
	}
	
	if((UINT16)StrToInt(szArgv[1]) == 1)
	{
		vCCIR2SetVdeWidth((UINT16)StrToInt(szArgv[2]));
	}
	else
	{
		vCCIR2SetNewVdeWidth((UINT16)StrToInt(szArgv[2]));
	}
	

	return 0;
}

static INT32 _u1wCCIRGetVsWidth(INT32 i4Argc, const CHAR **szArgv)
{
	INT16 data;

	if (i4Argc != 2){
	LOG(0, "Usage: %s type(main/sub)\n", szArgv[0]);
	return 0;
	}

	if((UINT16)StrToInt(szArgv[1]) == 1)
	{
		data = wCCIR2GetVdeWidth();
	}
	else
	{
		data = wCCIR2GetNewVdeWidth();
	}
	
	LOG(0, "The CCIR_VDE_width is: %d\n", data);

	return 0;
}



