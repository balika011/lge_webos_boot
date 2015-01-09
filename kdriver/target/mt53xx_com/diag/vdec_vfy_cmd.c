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
#include "x_printf.h"
#include "x_stl_lib.h"
//#include "drv_config.h"
#include "slt_if.h"

#ifdef __MW_CLI_DEF__
#include "x_mid_cli.h"
#else
#include "x_drv_cli.h"
#endif

#include "x_vid_dec.h"

#define DEFINE_IS_LOG	CLI_IsLog
#include "x_debug.h"
#include "vdec_debug.h"
//#include "drv_config.h"
#include "x_timer.h"


extern void _VDecSetModeLogCmd(UCHAR ucEsId, UINT32 u4ModeLog);
extern void vVDecVerifyThread(void *param_array);
extern void VP9_Verify_Thread(void *pParam_Array);
extern void hevc_test( int inst_id,int frame_start , int frame_end );
extern void vVDecTouch(UINT32  u4InstID,UCHAR ucVldID);
extern void vVdecUDTVInit(UINT32 u4InstID);
extern void vp9_test(int inst_id, bool fgDualCore, bool fgUFOMode,int frame_start , int frame_end );


///globale v
extern char bitstream_name[200];

#if 1 //(CONFIG_DRV_VERIFY_SUPPORT)
//#include "vdec_verify_vdec.h"

static INT32 _Cmd_VdecAutoTest(INT32 i4Argc, const CHAR** aszArgv)
{
  UINT32  u4InstanceId;
  HANDLE_T hPInsertMainThread;
  VOID* param[8];
  UINT32  u4Mode,u4LaeMode,u4DualCoreEnable, u4Main10Mode,u4UFOMode,u4CRCEnable;


  CHAR szBuf[16] = "MPV0";;
  u4Mode = 0;
  u4LaeMode = 0;
  u4DualCoreEnable = 0;
  u4Main10Mode = 0;
  u4InstanceId = (UINT32)StrToInt(aszArgv[1]);
  
  if (u4InstanceId > 1)
  {
      //UTIL_Printf("VDEC Need to Support PIP\n");
      u4InstanceId = 0;
  }

  szBuf[3] += u4InstanceId;

  u4Mode = 0;
  
  if(i4Argc >= 7)
  {
    u4Mode = (UINT32)StrToInt(aszArgv[2]);
    u4LaeMode = (UINT32)StrToInt(aszArgv[3]);
    u4DualCoreEnable = (UINT32)StrToInt(aszArgv[4]);
    u4Main10Mode = (UINT32)StrToInt(aszArgv[5]);
    u4UFOMode = (UINT32)StrToInt(aszArgv[6]);
    u4CRCEnable = (UINT32)StrToInt(aszArgv[7]);
  }
  else if(i4Argc >= 6)
  {
    u4Mode = (UINT32)StrToInt(aszArgv[2]);
    u4LaeMode = (UINT32)StrToInt(aszArgv[3]);
    u4DualCoreEnable = (UINT32)StrToInt(aszArgv[4]);
    u4Main10Mode = (UINT32)StrToInt(aszArgv[5]);
  }
  else if(i4Argc >= 5)
  {
    u4Mode = (UINT32)StrToInt(aszArgv[2]);
    u4LaeMode = (UINT32)StrToInt(aszArgv[3]);
    u4DualCoreEnable = (UINT32)StrToInt(aszArgv[4]);
  }
  else if(i4Argc >= 4)
  {
    u4Mode = (UINT32)StrToInt(aszArgv[2]);
    u4LaeMode = (UINT32)StrToInt(aszArgv[3]);
  }
  else if (i4Argc >= 3)
  {
       u4Mode = (UINT32)StrToInt(aszArgv[2]);
  }
  

  param[0] = (VOID*) u4InstanceId;
  param[1] = (VOID*) u4Mode;
  param[2] = (VOID*) u4LaeMode;
  param[3] = (VOID*) u4DualCoreEnable;
  param[4] = (VOID*) u4Main10Mode;
  param[5] = (VOID*) u4UFOMode;
  param[6] = (VOID*) u4CRCEnable;
  
  x_thread_create(&hPInsertMainThread, szBuf, MPV_VPRS_STACK_SIZE*10, MPV_VPRS_THREAD_PRIORITY,
      vVDecVerifyThread, sizeof(VOID*)*8, param);

  return 0;
}

static INT32 _Cmd_VdecVP9SimTest(INT32 i4Argc, const CHAR** aszArgv)
{
    UINT32  u4InstanceId;
    UINT32  u4Mode;
    UINT32  u4StartFrame,u4EndFrame;
    UINT32  u4UFOMode,u4DualCore;
    CHAR    ucBitstream[200] = {0};


    printk("**************************\n");
    printk("CLI : VP9 SimTest\n");
    printk("USAGE: bd_vdec.vp9simtest [InstID] [Bitstream_Path] [UFO Mode] [DualCore] [StartFrame] [EndFrame]\n");
    
    switch(i4Argc)
    {
        case 1:
        case 2:
        case 3:
            printk("USAGE: bd_vdec.vp9simtest [InstID] [Bitstream_Path] [UFO Mode] [DualCore] [StartFrame] [EndFrame]\n");
            break;
        case 4:
            u4InstanceId = (UINT32)StrToInt(aszArgv[1]);
            if (u4InstanceId > 1)
            {
              printk("VDEC Need to Support PIP\n");
              u4InstanceId = 0;
            }
            strcpy(ucBitstream,aszArgv[2]);
            u4StartFrame = (UINT32)StrToInt(aszArgv[3]);
            u4EndFrame   = 0xffffffff;
            u4UFOMode = 0;
            u4DualCore = 0;
            break;
        case 5:
            u4InstanceId = (UINT32)StrToInt(aszArgv[1]);
            if (u4InstanceId > 1)
            {
              printk("VDEC Need to Support PIP\n");
              u4InstanceId = 0;
            }
            strcpy(ucBitstream,aszArgv[2]);
            u4StartFrame = (UINT32)StrToInt(aszArgv[3]);
            u4EndFrame   = (UINT32)StrToInt(aszArgv[4]);
            break;
        case 6:
            u4InstanceId = (UINT32)StrToInt(aszArgv[1]);
            if (u4InstanceId > 1)
            {
              printk("VDEC Need to Support PIP\n");
              u4InstanceId = 0;
            }
            strcpy(ucBitstream,aszArgv[2]);
            u4UFOMode = (UINT32)StrToInt(aszArgv[3]);
            u4DualCore   = (UINT32)StrToInt(aszArgv[4]);
            u4StartFrame = (UINT32)StrToInt(aszArgv[5]);
            u4EndFrame   = 0xffffffff;
            break;
        case 7:
            u4InstanceId = (UINT32)StrToInt(aszArgv[1]);
            if (u4InstanceId > 1)
            {
              printk("VDEC Need to Support PIP\n");
              u4InstanceId = 0;
            }
            strcpy(ucBitstream,aszArgv[2]);
            u4UFOMode = (UINT32)StrToInt(aszArgv[3]);
            u4DualCore   = (UINT32)StrToInt(aszArgv[4]);
            u4StartFrame = (UINT32)StrToInt(aszArgv[5]);
            u4EndFrame   = (UINT32)StrToInt(aszArgv[6]);
            break;
    }
    
    if(i4Argc < 4)
    {
        printk("Please input param as \n");
        printk("bd_vdec.vp9simtest [InstID] [Bitstream_Path] [UFO Mode] [DualCore] [StartFrame] [EndFrame]\n");
        printk("***********break**************\n");
        return 1;
    }
    
    printk("stream %s ucVdecId [%d]-- start frame [%d]-- end frame [%d]\n",ucBitstream,u4InstanceId,u4StartFrame,u4EndFrame);
    printk("UFO [%d]-- DualMode [%d]\n",u4UFOMode,u4DualCore);
    
    printk("************go*************\n");
    strcpy(bitstream_name,ucBitstream);

    vp9_test(u4InstanceId, FALSE, FALSE, u4StartFrame, u4EndFrame);
    
    return 0;
}

static INT32 _Cmd_VdecVP9AutoTest(INT32 i4Argc, const CHAR** aszArgv)
{
    UINT32  u4InstanceId;
    UINT32  u4UFOMode, u4DualCore;
    VOID* param[8];
    HANDLE_T hPInsertMainThread;
    CHAR szBuf[16] = "VP90";

    
    szBuf[3] += u4InstanceId;

    printk("**************************\n");
    printk("CLI : VP9 AutoTest\n");
 
    switch(i4Argc)
    {
        case 1:
            printk("USAGE: bd_vdec.vp9autotest [InstID] [UFOMode] [DualCore]\n");
            break;            
        case 2:
            u4InstanceId = (UINT32)StrToInt(aszArgv[1]);
            if (u4InstanceId > 1)
            {
              printk("VDEC Need to Support PIP\n");
              u4InstanceId = 0;
            }
            u4UFOMode = 0;
            u4DualCore = 0;
            break;
        case 3:
            u4InstanceId = (UINT32)StrToInt(aszArgv[1]);
            if (u4InstanceId > 1)
            {
              printk("VDEC Need to Support PIP\n");
              u4InstanceId = 0;
            }
            u4UFOMode = (UINT32)StrToInt(aszArgv[2]);
            u4DualCore = 0;
            break;
        case 4:
            u4InstanceId = (UINT32)StrToInt(aszArgv[1]);
            if (u4InstanceId > 1)
            {
              printk("VDEC Need to Support PIP\n");
              u4InstanceId = 0;
            }
            u4UFOMode = (UINT32)StrToInt(aszArgv[2]);
            u4DualCore = (UINT32)StrToInt(aszArgv[3]);
            break;
        default:
            u4InstanceId = 0;
            u4UFOMode = 0;
            u4DualCore = 0;
        break;
    }
    
    if(i4Argc < 2)
    {
        printk("USAGE: bd_vdec.vp9autotest [InstID] [UFOMode] [DualCore]\n");
        printk("***********break**************\n");
        return 1;
    }
    printk("ucVdecId [%d]-- u4UFOMode [%d]-- u4DualCore [%d]\n",u4InstanceId, u4UFOMode, u4DualCore);
    printk("************go*************\n");
    
    param[0] = (VOID*) u4InstanceId;
    param[1] = (VOID*) u4UFOMode;
    param[2] = (VOID*) u4DualCore;
    
    x_thread_create(&hPInsertMainThread, szBuf, MPV_VPRS_STACK_SIZE*10, MPV_VPRS_THREAD_PRIORITY,
      VP9_Verify_Thread, sizeof(VOID*)*5, param);
    
    return 0;
}

static INT32 _Cmd_VdecHEVCSimTest(INT32 i4Argc, const CHAR** aszArgv)
{
    UINT32  u4InstanceId;
    UINT32  u4Mode;
    UINT32  u4StartFrame,u4EndFrame;
    CHAR    ucBitstream[200];

    printk("**************************\n");
    printk("CLI : HEVC VdecSimTest\n");
 
    switch(i4Argc)
    {
        case 1:
        case 2:
        case 3:
            break;
        case 4:
            u4InstanceId = (UINT32)StrToInt(aszArgv[1]);
            if (u4InstanceId > 1)
            {
              printk("VDEC Need to Support PIP\n");
              u4InstanceId = 0;
            }
            strcpy(ucBitstream,aszArgv[2]);
            u4StartFrame = (UINT32)StrToInt(aszArgv[3]);
            u4EndFrame   = 0xffffffff;
            break;
        case 5:
            u4InstanceId = (UINT32)StrToInt(aszArgv[1]);
            if (u4InstanceId > 1)
            {
              printk("VDEC Need to Support PIP\n");
              u4InstanceId = 0;
            }
            strcpy(ucBitstream,aszArgv[2]);
            u4StartFrame = (UINT32)StrToInt(aszArgv[3]);
            u4EndFrame   = (UINT32)StrToInt(aszArgv[4]);;
            break;
    }
    
    if(i4Argc < 4)
    {
        printk("Please input param as /instid -> bitstream ->start ->end/ \n");
        printk("***********break**************\n");
        return 1;
    }
    printk("stream %s ucVdecId [%d]-- start frame [%d]-- end frame [%d]\n",ucBitstream,u4InstanceId,u4StartFrame,u4EndFrame);
    printk("************go*************\n");
    strcpy(bitstream_name,ucBitstream);
    hevc_test(u4InstanceId,u4StartFrame,u4EndFrame);
    
    return 0;
}

static INT32 _Cmd_VdecTouch(INT32 i4Argc, const CHAR** aszArgv)
{
    UINT32  u4InstanceId;
    //u4InstanceId = (UINT32)StrToInt(aszArgv[1]);
    vVDecTouch(0,0);
}

static INT32 _Cmd_VdecInit(INT32 i4Argc, const CHAR** aszArgv)
{
    UINT32  u4InstanceId;
    u4InstanceId = (UINT32)StrToInt(aszArgv[1]);
    vVdecUDTVInit(u4InstanceId);
}

#endif


/******************************************************************************
* CLI Command Table
******************************************************************************/

static CLI_EXEC_T arMpvCmdTbl[] = {

 	#if 1//(CONFIG_DRV_VERIFY_SUPPORT)
 	{
 	 	"autotest",
 	 	"verify",
 	 	_Cmd_VdecAutoTest,
 	 	NULL,
 	 	"burn in test",
 	 	CLI_SUPERVISOR
 	},
 	
 	{
 	 	"vp9simtest",
 	 	"verify",
 	 	_Cmd_VdecVP9SimTest,
 	 	NULL,
 	 	"simulation pattern test",
 	 	CLI_SUPERVISOR
 	},
 	
 	{
 	 	"vp9autotest",
 	 	"verify",
 	 	_Cmd_VdecVP9AutoTest,
 	 	NULL,
 	 	"Init",
 	 	CLI_GUEST
 	},
 	
 	{
 	 	"hevcsimtest",
 	 	"verify",
 	 	_Cmd_VdecHEVCSimTest,
 	 	NULL,
 	},
 	
 	{
 	 	"TouchVdec",
 	 	"verify",
 	 	_Cmd_VdecTouch,
 	 	NULL,
 	 	"simple test vdec",
 	 	CLI_SUPERVISOR
 	},
 	{
 	 	"init",
 	 	"verify",
 	 	vVdecUDTVInit,
 	 	NULL,
 	 	"Init",
 	 	CLI_SUPERVISOR
 	},
 	{NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
 	#endif
};


CLI_MAIN_COMMAND_ITEM(Vdec)
{
    "bd_vdec",
    NULL,
    NULL,
    arMpvCmdTbl,
    "BDP Vdec command",
    CLI_SUPERVISOR
};


/******************************************************************************
* Local Function
******************************************************************************/

/******************************************************************************
* Global Function
******************************************************************************/





