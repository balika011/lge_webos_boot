
#include "drv_scaler_drvif.h"


#include "x_os.h"
#include "x_stl_lib.h"

#include "x_assert.h"
#include "x_pinmux.h"
#include "x_debug.h"
#include "x_drv_cli.h"

#include "drv_mon_out.h"
#include "drv_mon_out_regen.h"
#include "drv_mon_out_predown.h"

#ifndef CC_CLI
#define CC_CLI
#endif
static INT32 MonOutSetRegen(INT32 i4Argc, const CHAR **szArgv);
static INT32 MonModuleDump(INT32 i4Argc, const CHAR **szArgv);
static INT32 MonOutCfgQurey(INT32 i4Argc, const CHAR **szArgv);


/******************************************************************************
* CLI Command Table
******************************************************************************/

CLIMOD_DEBUG_FUNCTIONS(SCPOS)

// Table for scpos command entry

// Table for scpos command entry
CLI_EXEC_T arMonOutCmdTbl[] = { 
  CLIMOD_DEBUG_CLIENTRY(SCPOS),
  #ifdef CC_CLI
  {"MonOut qurey", "q", MonOutCfgQurey, NULL, "MonOut query", CLI_SUPERVISOR},
  {"MonOut dump", "d", MonModuleDump, NULL, "MonOut dump", CLI_SUPERVISOR},
  {"Set regen", "regen", MonOutSetRegen, NULL, "Set overscan", CLI_SUPERVISOR},
  #endif
  {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};


#ifdef CC_CLI
static INT32 MonOutSetRegen(INT32 i4Argc, const CHAR **szArgv)
{		
    UINT32 u4X;
    UINT32 u4Y;
    UINT32 u4W;
    UINT32 u4H;
	if(i4Argc < 5)
	{
		Printf("Usage: x y width height\n");
		return 0;
	}

	u4X = (UINT32)StrToInt(szArgv[1]);
	u4Y = (UINT32)StrToInt(szArgv[2]);
	u4W = (UINT32)StrToInt(szArgv[3]);
	u4H = (UINT32)StrToInt(szArgv[4]);
	Printf("set regen (%u,%u,%u,%u)\n", u4X, u4Y, u4W, u4H);
    vMonOutPDSSetModeChanged();
    _vMonOutRegenUpdateOutputResolution(u4ScpipGetHsyncBp() + u4X, u4ScpipGetVsyncBp() + u4Y, u4W, u4H);
    _vMonOutRegenUpdateReg();
    vMonOutPDS();
	return 0;
}

static INT32 MonOutCfgQurey(INT32 i4Argc, const CHAR **szArgv)
{
    UNUSED(i4Argc);
    UNUSED(szArgv);
    vMonOutModuleCfg(); 
    return 0;      
}

static INT32 MonModuleDump(INT32 i4Argc, const CHAR **szArgv)
{
    UNUSED(i4Argc);
    UNUSED(szArgv);
    vMonOutDumpAllModuleInfo();
    return 0;
}

#endif



