/********************************************************************************************
 *     LEGAL DISCLAIMER 
 *
 *     (Header of MediaTek Software/Firmware Release or Documentation)
 *
 *     BY OPENING OR USING THIS FILE, BUYER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES 
 *     THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE") RECEIVED 
 *     FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO BUYER ON AN "AS-IS" BASIS 
 *     ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES, EXPRESS OR IMPLIED, 
 *     INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR 
 *     A PARTICULAR PURPOSE OR NONINFRINGEMENT. NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY 
 *     WHATSOEVER WITH RESPECT TO THE SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, 
 *     INCORPORATED IN, OR SUPPLIED WITH THE MEDIATEK SOFTWARE, AND BUYER AGREES TO LOOK 
 *     ONLY TO SUCH THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. MEDIATEK SHALL ALSO
 *     NOT BE RESPONSIBLE FOR ANY MEDIATEK SOFTWARE RELEASES MADE TO BUYER'S SPECIFICATION 
 *     OR TO CONFORM TO A PARTICULAR STANDARD OR OPEN FORUM.
 *     
 *     BUYER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND CUMULATIVE LIABILITY WITH 
 *     RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE, AT MEDIATEK'S OPTION, 
 *     TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE, OR REFUND ANY SOFTWARE LICENSE 
 *     FEES OR SERVICE CHARGE PAID BY BUYER TO MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE. 
 *     
 *     THE TRANSACTION CONTEMPLATED HEREUNDER SHALL BE CONSTRUED IN ACCORDANCE WITH THE LAWS 
 *     OF THE STATE OF CALIFORNIA, USA, EXCLUDING ITS CONFLICT OF LAWS PRINCIPLES.  
 ************************************************************************************************/

/*-----------------------------------------------------------------------------
 *
 * Description:
 *         The CLI commands sample
 *
 *---------------------------------------------------------------------------*/

//#include "vdec_verify_vdec.h"
#include "x_drv_cli.h"
#include "x_serial.h"
#include "x_printf.h"
#include "x_os.h"
#include "x_assert.h"
#include "x_util.h"
#include "x_stl_lib.h"

//---------------------------------------------------------------------
// LOG macro related

#define DEFINE_IS_LOG	CLI_IsLog
#include "x_debug.h"
#include "x_ckgen.h"
#include "x_debug.h"
#include "x_drv_cli.h"
extern void vImgReszVfyInit(void);
extern void vImgReszVfyUninit(void);
extern void vImgReszVerify(UINT32 u4TestCase);
extern void vImgReszVfyAutoTest(UINT32 u4TestType,UINT32 u4StartCase,char *pbCaseInfoFile);
extern void vImgreszForWT(void);
extern void vImgreszForWT1(void);

extern UINT32 _u4MaxImg8192HVMode;// add by chunze

static INT32 _Cmd_ImgReszVfyInit(INT32 i4Argc, const CHAR** aszArgv)
{
  vImgReszVfyInit();
  return 0;
}


static INT32 _Cmd_ImgReszVfyUninit(INT32 i4Argc, const CHAR** aszArgv)
{
  vImgReszVfyUninit();
  return 0;
}


static INT32 _Cmd_ImgReszVfyVerifyTest(INT32 i4Argc, const CHAR** aszArgv)
{
  UINT32 u4TestCase;

  vImgReszVfyInit();
  if(i4Argc < 2)
  {
    u4TestCase = 0;
  } else if(i4Argc == 2)
  {
    u4TestCase = (UINT32)StrToInt(aszArgv[1]);
	  _u4MaxImg8192HVMode = 0; 
  }
  else
  {
    u4TestCase = (UINT32)StrToInt(aszArgv[1]);
	  _u4MaxImg8192HVMode = (UINT32)StrToInt(aszArgv[2]);
  }

  vImgReszVerify(u4TestCase);
    vImgReszVfyUninit();
  return 0;
}
static INT32 _Cmd_ImgreszClk(INT32 i4Argc, const CHAR** aszArgv)
{
   UINT32 u4SysClock= (UINT32)StrToInt(aszArgv[1]);

   if(u4SysClock >= 0 && u4SysClock <= 12)
   {
        IO_WRITE32(CKGEN_BASE, 0x3c4, u4SysClock);
        Printf("[img] Vfy clock[0x%x]\n",u4SysClock);
   }
   else
   {
        Printf("[img]Err Vfy clock[0x%x]\n",u4SysClock);
   }

    return 0;
}


static INT32 _Cmd_ImgReszVfyAutoTest(INT32 i4Argc, const CHAR** aszArgv)
{
  if(i4Argc > 1)
  {
    vImgReszVfyAutoTest((UINT32)StrToInt(aszArgv[1]),(UINT32)StrToInt(aszArgv[2]),(char *)(aszArgv[3]));
  }

  return 0;
}

static INT32 _Cmd_ImgreszForWT(INT32 i4Argc, const CHAR** aszArgv)
{
    vImgreszForWT();
    return 0;
}

static INT32 _Cmd_ImgreszForWTtest(INT32 i4Argc, const CHAR** aszArgv)
{
   vImgreszForWT1();

    return 0;
}


//===========================================================================

CLI_EXEC_T _arImgReszVfyCmdTbl[] =
{
  // Init
  {"init",       "init", 	_Cmd_ImgReszVfyInit,     NULL, "init", CLI_GUEST},
  // Init
  {"uninit",     "uninit", 	_Cmd_ImgReszVfyUninit,     NULL, "uninit", CLI_GUEST},
  //Start to verify
  {"verify",     "vfy", 	_Cmd_ImgReszVfyVerifyTest,     NULL, "verify", CLI_GUEST},
  //Start to auto test
  {"autotest",   "autotest",_Cmd_ImgReszVfyAutoTest,     NULL, "autotest", CLI_GUEST},

  {"Clock",   "clk", _Cmd_ImgreszClk,     NULL, "ClockTest", CLI_GUEST},

  {"WTtest",   "WTtest", _Cmd_ImgreszForWT,     NULL, "WTtest", CLI_GUEST},
  {"WTtest1",   "WTtest1", _Cmd_ImgreszForWTtest,     NULL, "WTtest1", CLI_GUEST},



  //CLIMOD_DEBUG_CLIENTRY(Video Decoder),
  //Last CLI command record, NULL
  {NULL, NULL, NULL, NULL, NULL, CLI_GUEST}
};

//CLI_EXEC_T _rImgReszVfyCmdTbl =
CLI_MAIN_COMMAND_ITEM(IMGRZ_VFY)

{
  "imgreszvfy",
  NULL,
  NULL,
  _arImgReszVfyCmdTbl,
  "Image resizer verify command",
  CLI_GUEST
};
#if 0
CLI_EXEC_T* GetImgReszVfyCmdTbl(void)
{
  return &_rImgReszVfyCmdTbl;
}
#endif
