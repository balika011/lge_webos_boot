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
#include "x_common.h"
#include "x_bim.h"
#include "x_rand.h"
#include "drvcust_if.h"
#include "gfx3d_vfy_if.h"

/*-----------------------------------------------------------------------------
                    function extension
 ----------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
                    data define
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    data declarations
 ----------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
                    code start
 ----------------------------------------------------------------------------*/

void vGraphics3DCmdInit(void)
{

}


INT32 _Cmd_Graphics3D_Init(INT32 i4Argc, const CHAR** aszArgv)
{    
    i4Graphics3D_Drv_Init();
    return 0;
}


INT32 _Cmd_Graphics3D_Uninit(INT32 i4Argc, const CHAR** aszArgv)
{
    i4Graphics3D_Drv_Uninit();
    return 0;
}


INT32 _Cmd_Graphics3D_Stop(INT32 i4Argc, const CHAR** aszArgv)
{    
    return 0;
}


INT32 _Cmd_Graphics3D_Emu(INT32 i4Argc, const CHAR** aszArgv)
{    

    Printf("Graphics 3D Emulation Start\n");
    i4Graphics3D_Drv_Emu();
    Printf("Graphics 3D Emulation End\n");
    return 0;
}

INT32 _Cmd_Graphics3D_RegRW(INT32 i4Argc, const CHAR** aszArgv)
{    

    Printf("Graphics 3D Register Read/Write Testing\n");
    i4Graphics3D_Drv_RegRW();
    return 0;
}
void vGraphics3DCmdThread(void *arg)
{
}


static BOOL _fgLooping = TRUE;
static HANDLE_T _hGfx3dThread = 0x0;

static void _GFX3DThread(void* pvArg)
{
    INT32 i4Ret;
    UNUSED(i4Ret);
    while(1)
    {
        if(_fgLooping)
        {
            i4Ret = i4Graphics3D_Drv_Emu();
        }
        else
        {
            x_thread_delay(100);
        }
    }
}

static INT32 _Cmd_Graphics3D_Looptest(INT32 i4Argc, const CHAR** aszArgv)
{
    INT32 i4Ret;
    if(i4Argc >= 2)
    {
        _fgLooping = (BOOL)StrToInt(aszArgv[1]);        
    }

    if(_fgLooping)
    {
        Printf("Start 3D GFX loop test\n");
    }
    else
    {
        Printf("Pause 3D GFX loop test\n");    
    }

    i4Ret = i4Graphics3D_Drv_Init();
	
    //reset
    IO_WRITE32(0xF0040000, 0x20, 0x20);
    IO_WRITE32(0xF0048000, 0x100c, 0x20);        
    x_thread_delay(100);    
    i4Ret = i4Graphics3D_Drv_Emu();
    //reset
    IO_WRITE32(0xF0040000, 0x20, 0x20);
    IO_WRITE32(0xF0048000, 0x100c, 0x20);        
    x_thread_delay(100);            
    if(_hGfx3dThread == 0x0)
    {
        if(x_thread_create(&_hGfx3dThread, "GFX3D", 4096, CLI_THREAD1_PRIORITY, _GFX3DThread, 0, NULL) != OSR_OK)
        {
            return -1;
        }
    }
    return i4Ret;
}




//===========================================================================

CLI_EXEC_T _arGraphics3DCmdTbl[] =
{
    {"Graphics3D Init",     "init",     _Cmd_Graphics3D_Init,       NULL,   "Graphics3D init",          CLI_SUPERVISOR},
    {"Graphics3D Uninit",   "uninit",   _Cmd_Graphics3D_Uninit,     NULL,   "Graphics3D uninit",        CLI_SUPERVISOR},    
    {"Graphics3D stop",     "stop",     _Cmd_Graphics3D_Stop,       NULL,   "Graphics3D stop",          CLI_SUPERVISOR},
    {"Graphics3D Emu",      "emu",     _Cmd_Graphics3D_Emu,         NULL,   "Graphics3D emu",           CLI_SUPERVISOR},
	{"Graphics3D looptest",	"looptest",	  _Cmd_Graphics3D_Looptest, NULL,	 "Graphics3D loop test",	CLI_SUPERVISOR},
	{"Graphics3D RegRW",     "regrw",     _Cmd_Graphics3D_RegRW,    NULL,   "Graphics3D reg r/w",       CLI_SUPERVISOR},
    // last cli command record, NULL
    {NULL, NULL, NULL, NULL, NULL, CLI_GUEST}
};




CLI_MAIN_COMMAND_ITEM(Gfx3d)
{
	"gfx3d",
	NULL,
	NULL,
	_arGraphics3DCmdTbl,
	"Gfx3d interface",
	 CLI_SUPERVISOR
};


