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

extern void vPngVerify(INT32 i4Argc, const CHAR** aszArgv);
extern UINT32 _u4StartClock;
extern UINT32 _u4EndClock;
extern  BOOL fgTransVery;
INT32 _Cmd_PngVfyAutoTest(INT32 i4Argc, const CHAR** aszArgv)
{
  fgTransVery = FALSE;
  _u4StartClock =5;
  _u4EndClock =5;
  if(i4Argc>=3)
  {
      _u4StartClock = (UINT32)StrToInt(aszArgv[1]);
      _u4EndClock = (UINT32)StrToInt(aszArgv[2]);
	  if(i4Argc==4)
	  {
	    fgTransVery =(BOOL)StrToInt(aszArgv[3]);	  }
      
  }
  Printf("_ui4StartClock[%d] _ui4EndClock[%d], fgTransVery[%d]\n",_u4StartClock, _u4EndClock,fgTransVery);  
  vPngVerify(i4Argc,aszArgv);
  return 0;
}


#if 0
static INT32 _Cmd_PngVfyWT(INT32 i4Argc, const CHAR** aszArgv)
{
  vPNGWTMMUTest();
  return 0;
}
#endif
//===========================================================================

CLI_EXEC_T _arPngVfyCmdTbl[] =
{
  //Start to decode
  {"verify",   "vfy", _Cmd_PngVfyAutoTest, NULL, "verify", CLI_GUEST},
  #if 0
  {"verify all",   "va", _Cmd_PngVfyAutoTestAll, NULL, "verify all", CLI_GUEST},
  {"verifyWT",   "VT", _Cmd_PngVfyWT, NULL, "verifyWT", CLI_GUEST},
  #endif
  //CLIMOD_DEBUG_CLIENTRY(Video Decoder),
  //Last CLI command record, NULL
  {NULL, NULL, NULL, NULL, NULL, CLI_GUEST}
};






CLI_MAIN_COMMAND_ITEM(PNG_VFY)

{
  "pngvfy",
  NULL,
  NULL,
  _arPngVfyCmdTbl,
  "png verify command",
  CLI_GUEST
};




#if 0
CLI_EXEC_T _rPngVfyCmdTbl =
{
  "pngvfy",
  NULL,
  NULL,
  _arPngVfyCmdTbl,
  "Png decoder verify command",
  CLI_GUEST
};




CLI_EXEC_T* GetPngVfyCmdTbl(void)
{
  Printf("Enter %s\n", __FUNCTION__);
  return &_rPngVfyCmdTbl;
}
#endif
