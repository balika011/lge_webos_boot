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
 * $RCSfile: msdc_cmd.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file msdc_cmd.c
 *  msdc_cmd.c provide cli for msdc module.
 */

#include "x_hal_arm.h"
#include "x_hal_5381.h"
#include "x_stl_lib.h"
#include "x_timer.h"
#include "x_os.h"
#include "x_drv_cli.h"
#include "x_util.h"
#include "x_gpio.h"
#include "x_ckgen.h"
#include "x_rand.h"
#include "x_debug.h"
#include "x_bim.h"
#include "x_timer.h"

#include "msdc_if.h"

#ifdef CC_MSDC_ENABLE
//---------------------------------------------------------------------------
// Macro definitions
//---------------------------------------------------------------------------
#define MSDC_TESTBUF1_ADDR    (0x1000000)
#define MSDC_TESTBUF2_ADDR    (0x2000000)
#define MSDC_TESTBUF3_ADDR    (0x3000000)
#define TEST_MEM_SIZE         (0x1000000)

MSDC_DevInfo_T MsdcDevInfo;

static INT32 _MSDCCLI_HostSet(INT32 i4Argc, const CHAR ** szArgv)
{
	  UINT32 u4Ch;

    if (i4Argc != 2)
    {
        LOG(0, "%s [idx]\n", szArgv[0]);
        return 0;
    }
    
    u4Ch = StrToInt(szArgv[1]);
    
    MSDC_HostSet(u4Ch);
    return 0;
}

static INT32 _MSDCCLI_HostGet(INT32 i4Argc, const CHAR ** szArgv)
{
	  UINT32 u4Ch;
    
    u4Ch = MSDC_HostGet();
    LOG(0, "Current Host is MSDC%d\n", u4Ch);
    
    return 0;
}

static INT32 _MSDCCLI_Pinmux(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Ch;

    if (i4Argc != 2)
    {
        LOG(0, "%s pnx [idx]\n", szArgv[0]);
        return 0;
    }
	
    u4Ch = StrToInt(szArgv[1]);	
    
    MSDC_PinMux(u4Ch);
    return 0;
}

static INT32 _MSDCCLI_SwitchVoltage(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4hV;

    if (i4Argc != 2)
    {
        LOG(0, "sv [0/1]\n", szArgv[0]);
        LOG(0, "     0 - 3.3V\n");
        LOG(0, "     1 - 1.8V\n");
        return 0;
    }

    u4hV = StrToInt(szArgv[1]);	
    
    MSDCSDSwitchVoltage(u4hV);
    return 0;
}

static INT32 _MSDCCLI_SysInit(INT32 i4Argc, const CHAR ** szArgv)
{    
    MsdcSysInit();
    return 0;    
}

static INT32 _MSDCCLI_Identify(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 i4Ret = 0;
    UINT32 u4Force;
	
    if (i4Argc != 2)
    {
        LOG(0, "%s id [0/1]\n", szArgv[0]);
        return 0;
    }	
    
    u4Force = StrToInt(szArgv[1]);
    
    if (u4Force == 0)
    {
        i4Ret = MsdcIdentyCard(FALSE);
    }
    else
    {
        i4Ret = MsdcIdentyCard(TRUE);    
    }
    
    LOG(0, "_MSDCCLI_Identify, i4Ret = 0x%X.\n", i4Ret);
    return 0;
}

static INT32 _MSDCCLI_SetDataMode(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 i4Ret = 0;
    UINT32 u4fgDataMode;

    if (i4Argc != 2)
    {
        LOG(0, "setdatamode [1/2/3/4]\n");
        LOG(0, "     1 - PIO Mode\n");
        LOG(0, "     2 - Basic DMA Mode\n");
        LOG(0, "     3 - Descriptor DMA Mode\n");
        LOG(0, "     4 - Enhanced DMA Mode\n");
        return 0;
    }

    u4fgDataMode = StrToInt(szArgv[1]);

    i4Ret = MsdcSetDataMode(u4fgDataMode);

    LOG(0, "_MSDCCLI_SetDataMode, i4Ret = 0x%X.\n", i4Ret);
    return 0;
}

static INT32 _MSDCCLI_SetDataEndMode(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 i4Ret = 0;
    UINT32 u4fgDataEndMode;

    if (i4Argc != 2)
    {
        LOG(0, "setdataendmode [0/1/2/3]\n");
        LOG(0, "     0 - cmd12 by hand\n");
        LOG(0, "     1 - cmd23 by hand\n");
        LOG(0, "     2 - cmd12 by auto\n");
        LOG(0, "     3 - cmd23 by auto\n");
        return 0;
    }

    u4fgDataEndMode = StrToInt(szArgv[1]);

    i4Ret = MsdcSetDataEndMode(u4fgDataEndMode);

    LOG(0, "_MSDCCLI_SetDataEndMode, i4Ret = 0x%X.\n", i4Ret);
    return 0;

}

static INT32 _MSDCCLI_SetDataBusWidth(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 i4Ret = 0;
    UINT32 u4BusWidth;

    if (i4Argc != 2)
    {
        LOG(0, "setbuswidth [1/4/8]\n");
        LOG(0, "     1 - 1bit Mode\n");
        LOG(0, "     4 - 4bit Mode\n");
        LOG(0, "     8 - 8bit Mode\n");
        return 0;
    }

    u4BusWidth = StrToInt(szArgv[1]);

    i4Ret = MsdcSetDataBusWidth(u4BusWidth);

    LOG(0, "_MSDCCLI_SetDataBusWidth, i4Ret = 0x%X.\n", i4Ret);
    return 0;
}

static INT32 _MSDCCLI_SetSpeedMode(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 i4Ret = 0;
    UINT32 u4SpeedMode;

    if (i4Argc != 2)
    {
        LOG(0, "setspeedmode [0/1]\n");
        LOG(0, "     0 - DS/SDR12(0~26/0~25(MHz))\n");
        LOG(0, "     1 - HS/SDR25(0~52/0~50(MHz))\n");
		LOG(0, "     2 - HS200/SDR50(0~200/0~100(MHz))\n");
        LOG(0, "     3 - SDR104(0~208(MHz))\n");
		LOG(0, "     4 - DDR50(0~50MHz)\n");
        return 0;
    }

    u4SpeedMode = StrToInt(szArgv[1]);

    i4Ret = MsdcSetSpeedMode(u4SpeedMode);

    LOG(0, "_MSDCCLI_SetSpeedMode, i4Ret = 0x%X.\n", i4Ret);
    return 0;	
}

static INT32 _MSDCCLI_SetDDRMode(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 i4Ret = 0;
    UINT32 u4DDRMode;

    if (i4Argc != 2)
    {
        LOG(0, "setddrmode [0/1]\n");
        LOG(0, "     0 - SDR\n");
        LOG(0, "     1 - DDR\n");
        return 0;
    }

    u4DDRMode = StrToInt(szArgv[1]);

    i4Ret = MsdcSetDDRMode(u4DDRMode);

    LOG(0, "_MSDCCLI_SetDDRMode, i4Ret = 0x%X.\n", i4Ret);
    return 0;	
   
}

static INT32 _MSDCCLI_SetBusClock(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 i4Ret = 0;
    UINT32 u4Clock;

    if (i4Argc != 2)
    {
    #if defined(CC_MT5399) || defined(CONFIG_ARCH_MT5399)
        LOG(0, "setbusclock [clock(MHz)]: 225, 216, 192, 172, 162, 144, 120, 108, 80, 48, 24, 13, 0\n");
    #else
        LOG(0, "setbusclock [clock(MHz)]: 54 48 43 40 36 30 27 24 18 13 0\n");
    #endif
        return 0;
    }

    u4Clock = StrToInt(szArgv[1]);

    i4Ret = MsdcSetBusClock(u4Clock);

    LOG(0, "_MSDCCLI_SetBusClock, i4Ret = 0x%X.\n", i4Ret);
    return 0;

}

static INT32 _MSDCCLI_PrintConfigInfo(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 i4Ret = 0;

    i4Ret = MsdcPrintConfigInfo();
	
    LOG(0, "_MSDCCLI_PrintConfigInfo, i4Ret = 0x%X.\n", i4Ret);
    return 0;
}

static INT32 _MSDCCLI_SetContinueClock(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 i4Ret = 0;
    UINT32 fgContinueClock;

    if (i4Argc != 2)
    {
        LOG(0, "setcontinueclock [0/1]\n");
        return 0;
    }

    fgContinueClock = StrToInt(szArgv[1]);

    i4Ret = MsdcSetContinueClock(fgContinueClock);
	
    LOG(0, "_MSDCCLI_SetContinueClock, i4Ret = 0x%X.\n", i4Ret);
    return 0;
}

static INT32 _MSDCCLI_GetRegisters(INT32 i4Argc, const CHAR ** szArgv) 
{
    INT32 i4Ret = 0;
    UINT32 u4Which;

    if (i4Argc != 2)
    {
        LOG(0, "getregisters [0/1/2/3/4]\n");
        LOG(0, "     0 - CSD\n");
        LOG(0, "     1 - EXT_CSD\n");
        LOG(0, "     2 - Status\n");
        LOG(0, "     3 - CID\n");
        LOG(0, "     4 - OCR\n");
        return 0;
    }
    
    u4Which = StrToInt(szArgv[1]);
    
    i4Ret = MsdcGetRegisters(u4Which);
    
    LOG(0, "_MSDCCLI_GetRegisters, i4Ret = 0x%X.\n", i4Ret);
    return 0;	
}

static INT32 _MSDCCLI_GetDevInfo(INT32 i4Argc, const CHAR ** szArgv) 
{
    INT32 i4Ret = 0;

    i4Ret = MsdcGetDevInfo((MSDC_DevInfo_T *)(&MsdcDevInfo));
    if(i4Ret == 0)
    {
        LOG(0, "Write Block Length: %08X(Bytes).\n", MsdcDevInfo.Write_BlkSz);
        LOG(0, "Erase Group Size: %08X(%d KBytes)(%d blocks).\n", MsdcDevInfo.Erase_GrpSz, 
                                                    (UINT32)(MsdcDevInfo.Erase_GrpSz>>10), 
                                         MsdcDevInfo.Erase_GrpSz/MsdcDevInfo.Write_BlkSz);
        LOG(0, "Write Protect Group Size: %08X(%d MBytes)(%d Erase Groups).\n", MsdcDevInfo.WP_GrpSz,
                                                                  (UINT32)(MsdcDevInfo.WP_GrpSz>>20),
                                                       MsdcDevInfo.WP_GrpSz/MsdcDevInfo.Erase_GrpSz);
        LOG(0, "Boot Partition Size: %08X(%d MBytes)(%d blocks).\n", MsdcDevInfo.Boot_Partition_Sz,
                                                       (UINT32)(MsdcDevInfo.Boot_Partition_Sz>>20),
                                                       (UINT32)(MsdcDevInfo.Boot_Partition_Sz>>9));
        LOG(0, "Device Size: 0x%X%X((%d blocks)).\n", (UINT32)(MsdcDevInfo.Dev_Sz>>32),
                                                          (UINT32)(MsdcDevInfo.Dev_Sz), 
                                                      (UINT32)(MsdcDevInfo.Dev_Sz>>9));
    }
    
    LOG(0, "_MSDCCLI_GetDevInfo, i4Ret = 0x%X.\n", i4Ret);
    return 0;	
}

#if 0
static INT32 _MSDCCLI_SetAccessRegion(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32  i4Ret = 0;
    UINT32 u4Flag;

    if (i4Argc != 2)
    {
        LOG(0, "%s [usr/boot1/boot2]\n", szArgv[0]);
        return 0;
    }
	
    if (x_strncmp(szArgv[1], "usr", sizeof("usr")) == 0)
    {
        u4Flag = 0;
    }
    else if (x_strncmp(szArgv[1], "boot1", sizeof("boot1")) == 0)
    {
        u4Flag = 1;
    }
    else if (x_strncmp(szArgv[1], "boot2", sizeof("boot2")) == 0)
    {
        u4Flag = 2;    
    }	
    
    i4Ret = MsdcSetAccessRegion(u4Flag);
    
    LOG(0, "_MSDCCLI_SetAccessRegion, i4Ret = 0x%X.\n", i4Ret);
    return 0;
}
#endif

static INT32 _MSDCCLI_Read(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 i4Ret = 0;
    UINT32 u4Offset, u4Len, u4DstPtr;
    UINT32 u4fgDev;

    if (i4Argc != 4)
    {
        LOG(0, "%s [memptr] [offset] [size]\n", szArgv[0]);
        return 0;
    }
    
    if((szArgv[1][0] == '-') && (szArgv[1][1] == 'd'))
    {
        u4DstPtr = StrToInt(szArgv[1] + 2);	
        u4fgDev = 1;
    }
    else
    {
    u4DstPtr = StrToInt(szArgv[1]);
        u4fgDev = 0;
    }
    u4Offset = StrToInt(szArgv[2]);
    u4Len    = StrToInt(szArgv[3]);
	
    LOG(0, "_MSDCCLI_Read, %sDstPtr:0x%08X, Offset:0x%08X, Len:0x%08X\n", 
                           u4fgDev?"(Dev)":"", u4DstPtr, u4Offset, u4Len);
	
	  if(u4fgDev == 1)
	  {
        i4Ret = MsdcReadCard(u4Offset, (UINT32 *)MSDC_TESTBUF2_ADDR, u4Len);
        if(0 != i4Ret)
        {
            goto End;	
        }  
         
        i4Ret = MsdcWriteCard((UINT32 *)MSDC_TESTBUF2_ADDR, u4DstPtr, u4Len); 
    }
    else
    {
    i4Ret = MsdcReadCard(u4Offset, (UINT32 *)u4DstPtr, u4Len);
    }
	
End:
    LOG(0, "_MSDCCLI_Read, i4Ret = 0x%X.\n", i4Ret);
    return 0;
}


#define PIO_DATA_MODE				    (1)
#define BASIC_DMA_DATA_MODE			    (2)
#define DESC_DMA_DATA_MODE			    (3)
#define ENHANCED_DMA_DATA_MODE		    (4)

static INT32 _MSDCCLI_Read_Perform(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32  i4Ret = 0;
    UINT32 u4Offset, u4Len, u4DstPtr;

	UINT32 U4Clock[3]={13,24,48};
	UINT32 U4BusWidth[3]={1,4,8};
	UINT32 i,j,k;
	UINT32 U4Speed;
    HAL_TIME_T rMsdTimeStart;
    HAL_TIME_T rMsdTimeEnd;
    HAL_TIME_T rMsdTimeDelta = {0};

	if (i4Argc != 3)
	{
        LOG(0, "%s  [offset] [size]\n", szArgv[0]);
		return 0;
	}
    
    u4DstPtr = 0x1700000;
    u4Offset = StrToInt(szArgv[1]);
    u4Len    = StrToInt(szArgv[2]);
	
	LOG(0, "_MSDCCLI_Read_Perform, DstPtr:0x%08X, Offset:0x%08X, Len:0x%08X\n", u4DstPtr, u4Offset, u4Len);
	MsdcSetSpeedMode(1);

	for(i=1;i<4;i++)
	{
		MsdcSetDataMode(i);//pio 1 ,DMA 2 ,Descriptor DMA 3

		
		for(j=0;j<3;j++)
		{
			MsdcSetDataBusWidth(U4BusWidth[j]);//BIT1 BIT4 BIT8
			

			for(k=0;k<3;k++)
			{
				MsdcSetBusClock(U4Clock[k]);//13M   24M  48 M

				if(i==1)
				{
					LOG(0, "Data Mode: PIO ,");
				}
				else if(i==2)
				{
					LOG(0, "Data Mode: Basic DMA ,");

				}
				else if(i==3)
				{
					LOG(0, "Data Mode: Descriptor DMA ,");

				}
				
				LOG(0, "DataBusWidth: %d Bit Mode ,",U4BusWidth[j]);
				
				LOG(0, "BusClock: %d MHZ.\n\n",U4Clock[k]);
	
   				HAL_GetTime(&rMsdTimeStart);				
				i4Ret = MsdcReadCard(u4Offset, (UINT32 *)u4DstPtr, u4Len);
				
				HAL_GetTime(&rMsdTimeEnd);
   	 			HAL_GetDeltaTime(&rMsdTimeDelta, &rMsdTimeStart, &rMsdTimeEnd);
				//LOG(0, "rMsdTimeDelta.u4Seconds = %d.\n", rMsdTimeDelta.u4Seconds);
				//LOG(0, "rMsdTimeDelta.u4Micros = %d. \n", rMsdTimeDelta.u4Micros);				
				U4Speed = (((u4Len/1048576)*10000)/((rMsdTimeDelta.u4Seconds)*1000+((rMsdTimeDelta.u4Micros)/1000)));
				
    			LOG(0,"Read Speed is %d M/10S,Read Cost Time is %d.%06d seconds, Read Buffer = 0x%x\n\n", U4Speed, rMsdTimeDelta.u4Seconds, rMsdTimeDelta.u4Micros, u4DstPtr);
			}

		}
 	}

	
	LOG(0, "_MSDCCLI_Read_Perform, i4Ret = 0x%X.\n", i4Ret);
	return 0;
}


static INT32 _MSDCCLI_Write_Perform(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32  i4Ret = 0, i;
	UINT8  u1Type;
    UINT8 *pu1SrcAddr;
    UINT32 u4Offset, u4Len, u4SrcPtr;

	UINT32 U4Clock[3]={13,24,48};
	UINT32 U4BusWidth[3]={1,4,8};
	UINT32 j,k;
	UINT32 U4Speed;
    HAL_TIME_T rMsdTimeStart;
    HAL_TIME_T rMsdTimeEnd;
    HAL_TIME_T rMsdTimeDelta = {0};


	if (i4Argc < 3)
	{
        LOG(0, "%s [offset] [size] [Type]\n", szArgv[0]);
		return 0;
	}

    u4SrcPtr = 0x1700000;
    u4Offset = StrToInt(szArgv[1]);
    u4Len    = StrToInt(szArgv[2]);

    if (i4Argc == 4)
    {
        u1Type = StrToInt(szArgv[3]);
    }
    else
    {
        u1Type = 0xFF;
    }
    	
	LOG(0, "_MSDCCLI_Write_Perform, SrcAddr:0x%X, Offset:0x%08X, Len:0x%08X, Type: 0x%02X\n", 
        u4SrcPtr, u4Offset, u4Len, u1Type);
	
    pu1SrcAddr = (UINT8 *)u4SrcPtr;
	if (u1Type == 0xFF)
	{
        for (i = 0; i < u4Len; i++)
        {
            *(pu1SrcAddr + i) = i; 
        }
	}
	else if (u1Type == 0x00)
	{
        for (i = 0; i < u4Len; i++)
        {
			*(pu1SrcAddr + i) = ((UINT32)rand()) % 0xFF;
        }
	}
	else
	{
        for (i = 0; i < u4Len; i++)
        {
            *(pu1SrcAddr + i) = u1Type; 
        }
	}



	for(i=1;i<4;i++)
	{
		MsdcSetDataMode(i);//pio 1 ,DMA 2 ,Descriptor DMA 3

		
		for(j=0;j<3;j++)
		{
			MsdcSetDataBusWidth(U4BusWidth[j]);//BIT1 BIT4 BIT8
			

			for(k=0;k<3;k++)
			{
				MsdcSetBusClock(U4Clock[k]);//13M   24M  48 M

				if(i==1)
				{
					LOG(0, "Data Mode: PIO ,");
				}
				else if(i==2)
				{
					LOG(0, "Data Mode: Basic DMA ,");

				}
				else if(i==3)
				{
					LOG(0, "Data Mode: Descriptor DMA ,");

				}
				
				LOG(0, "DataBusWidth: %d Bit Mode ,",U4BusWidth[j]);
				
				LOG(0, "BusClock: %d MHZ.\n\n",U4Clock[k]);
	
   				HAL_GetTime(&rMsdTimeStart);
				i4Ret = MsdcWriteCard((UINT32 *)u4SrcPtr, u4Offset, u4Len);
				HAL_GetTime(&rMsdTimeEnd);
   	 			HAL_GetDeltaTime(&rMsdTimeDelta, &rMsdTimeStart, &rMsdTimeEnd);
				//LOG(0, "rMsdTimeDelta.u4Seconds = %d.\n", rMsdTimeDelta.u4Seconds);
				//LOG(0, "rMsdTimeDelta.u4Micros = %d. \n", rMsdTimeDelta.u4Micros);
				U4Speed = (((u4Len/1048576)*10000)/((rMsdTimeDelta.u4Seconds)*1000+((rMsdTimeDelta.u4Micros)/1000)));
				
				LOG(0,"Write Speed is %d M/10S,Write Cost Time is %d.%06d seconds, Read Buffer = 0x%x\n", U4Speed, rMsdTimeDelta.u4Seconds, rMsdTimeDelta.u4Micros, u4SrcPtr);
			}

		}
 	}

	
	LOG(0, "_MSDCCLI_Write_Perform, i4Ret = 0x%X.\n", i4Ret);
	return 0;
}

static INT32 _MSDCCLI_Write(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 i4Ret = 0;
    UINT32 u4Offset, u4Len, u4SrcPtr;
    UINT32 u4fgDev;

    if (i4Argc != 4)
    {
        LOG(0, "%s [memptr] [offset] [size]\n", szArgv[0]);
        return 0;
    }

    if((szArgv[1][0] == '-') && (szArgv[1][1] == 'd'))
    {
        u4SrcPtr = StrToInt(szArgv[1] + 2);	
        u4fgDev = 1;
    }
    else
    {
    u4SrcPtr = StrToInt(szArgv[1]);
        u4fgDev = 0;
    }
    u4Offset = StrToInt(szArgv[2]);
    u4Len    = StrToInt(szArgv[3]);
    	
    LOG(0, "_MSDCCLI_Write, %sSrcAddr:0x%X, Offset:0x%08X, Len:0x%08X\n", 
                            u4fgDev?"(Dev)":"", u4SrcPtr, u4Offset, u4Len);
	
	  if(u4fgDev == 1)
	  {
        i4Ret = MsdcReadCard(u4SrcPtr, (UINT32 *)MSDC_TESTBUF2_ADDR, u4Len);
        if(0 != i4Ret)
        {
            goto End;	
        }  
         
        i4Ret = MsdcWriteCard((UINT32 *)MSDC_TESTBUF2_ADDR, u4Offset, u4Len); 
    }
    else
    {
    i4Ret = MsdcWriteCard((UINT32 *)u4SrcPtr, u4Offset, u4Len);
    }
	
End:	
    LOG(0, "_MSDCCLI_Write, i4Ret = 0x%X.\n", i4Ret);
    return 0;
}

static INT32 _MSDCCLI_Erase(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 i4Ret = 0;
    UINT32 u4Offset, u4Len, u4EraseType;

    if ((i4Argc < 3) || (i4Argc > 4))
    {
        LOG(0, "%s [offset] [size] [erasetype]\n", szArgv[0]);
        LOG(0, "erasetype:\n");
        LOG(0, "     0 - Erase\n");
        LOG(0, "     1 - Trim\n");
        LOG(0, "     2 - Secure Erase\n");
        LOG(0, "     3 - Secure Trim Step 1\n");
        LOG(0, "     4 - Secure Trim Step 2\n");
        return 0;
    }

    u4Offset = StrToInt(szArgv[1]);
    u4Len    = StrToInt(szArgv[2]);
    u4EraseType = 0;
    if (i4Argc == 4)
    {
        u4EraseType = StrToInt(szArgv[3]);
    }

    i4Ret = MsdcEraseCard(u4Offset, u4Len, u4EraseType);

    LOG(0, "_MSDCCLI_Erase, i4Ret = 0x%X.\n", i4Ret);
    return 0;
}

static INT32 _MSDCCLI_WriteProtect(INT32 i4Argc, const CHAR ** szArgv)
    {
    INT32 i4Ret = 0;
    UINT32 u4WPLevel, u4WPType, u4fgEn;
    UINT64 wpAddr = 0;

    if ((i4Argc < 4) || (i4Argc > 5))
    {
        LOG(0, "%s [WPLevel] [WPType] [fgEn] [wpAddr]\n", szArgv[0]);
        LOG(0, "WPLevel:\n");
        LOG(0, "     0 - All Card\n");
        LOG(0, "     1 - USER AREA\n");
        LOG(0, "     2 - BOOT PARTITION\n");
        LOG(0, "WPType:\n");
        LOG(0, "     0 - Temporary\n");
        LOG(0, "     1 - Power on\n");
        LOG(0, "     2 - Permenent\n");
        LOG(0, "WPType:\n");
        LOG(0, "     0 - Disable WP\n");
        LOG(0, "     1 - Enable WP \n"); 
        return 0;
    }

    u4WPLevel = StrToInt(szArgv[1]);
    u4WPType  = StrToInt(szArgv[2]);
    u4fgEn    = StrToInt(szArgv[3]);
    if((u4WPLevel != 0)&&(i4Argc == 4))
    {
        LOG(0, "Please specify the segment address for write protect.\n", i4Ret);
        return 0;
    }
    else if((u4WPLevel != 0) &&(i4Argc == 5))
    {
        wpAddr = StrToInt(szArgv[4]);	
    }

    i4Ret = MsdcWPCard(u4WPLevel, u4WPType, u4fgEn, wpAddr);

    LOG(0, "_MSDCCLI_WriteProtect, i4Ret = 0x%X.\n", i4Ret);
    return 0;
}

static INT32 _MSDCCLI_Measure(INT32 i4Argc, const CHAR ** szArgv)
{
    HAL_TIME_T time1, time2, result_time;
    UINT32 /*u4Micros1,*/ u4Micros2, u4Micros3;
    UINT32 u4TestAddr, u4Len, loop_count;
    UINT32 i, j, p;
    UINT8 *pu1TestBuf = NULL, *pu1GoldenBuf = NULL;

    if ((i4Argc < 3) || (i4Argc > 4))
    {
        Printf("%s [msdc_offset] [size] [loop count]\n", szArgv[0]);
        return 0;
    }

    u4TestAddr = StrToInt(szArgv[1]);
    u4Len      = StrToInt(szArgv[2]);
    loop_count = 3;
    if(i4Argc == 4)
    {
        loop_count = StrToInt(szArgv[3]);	
    }

    pu1TestBuf = (UINT8 *)MSDC_TESTBUF1_ADDR;
    pu1GoldenBuf = (UINT8 *)MSDC_TESTBUF2_ADDR;
    
    for (p = 0; p < loop_count; p++)
    {
        Printf("The LOOP %d------>\n", p);
        if ((p%3) == 0)
        {
            Printf("Measure pattern:0x00\n");
            x_memset(pu1GoldenBuf, 0x00, u4Len);
        }
        else if ((p%3) == 1)
        {
            Printf("Measure pattern:0x5A\n");
            x_memset(pu1GoldenBuf, 0x5A, u4Len);
        }
        else
        {
            Printf("Measure pattern:0xA5\n");
            x_memset(pu1GoldenBuf, 0xA5, u4Len);
        }

#if 0
        // Erase time
        Printf("Measure erase time:\n");
        HAL_GetTime(&time1);
        if (MsdcEraseCard(u4TestAddr, u4Len))
        {
            Printf("Erase eMMC error!\n");
            return -1;
        }
        HAL_GetTime(&time2);
        HAL_GetDeltaTime(&result_time, &time1, &time2);
        u4Micros1 = result_time.u4Seconds * 1000000 + result_time.u4Micros;
        Printf("Erase 0x%X elapse time: %d us\n", u4Len, u4Micros1);
#endif
        
        // Write time
        Printf("Measure write time:\n");
        HAL_GetTime(&time1);
        if (MsdcWriteCard((UINT32 *)pu1GoldenBuf, u4TestAddr, u4Len) != 0)
        {
            Printf("Write eMMC error!\n");
            return -1;
        }
        HAL_GetTime(&time2);
        HAL_GetDeltaTime(&result_time, &time1, &time2);
        u4Micros2 = result_time.u4Seconds * 1000000 + result_time.u4Micros;
        Printf("Write 0x%X elapse time: %d us\n", u4Len, u4Micros2);
    
        // Read time
        Printf("Measure read time:\n");
        x_memset(pu1TestBuf, 0xFF, u4Len);
        HAL_GetTime(&time1); 
        if (MsdcReadCard(u4TestAddr, (UINT32 *)pu1TestBuf, u4Len) != 0)
        {
            Printf("Read eMMC error!\n");
            return -1;
        }
        HAL_GetTime(&time2);
        HAL_GetDeltaTime(&result_time, &time1, &time2);
        u4Micros3 = result_time.u4Seconds * 1000000 + result_time.u4Micros;
        Printf("Read  0x%X elapse time: %d us\n", u4Len, u4Micros3);
    
        for (i = 0; i < u4Len; i++)
        {
            if (pu1GoldenBuf[i] != pu1TestBuf[i])
            {
                Printf("Read compare fail at offset 0x%X !\n", i);
                Printf("Write buffer: \n");
    
                for (j = 0; j < 0x20; j++)
                {
                    if (j % 16 == 0)
                    {
                        Printf("\n%03x: %02x ", j, pu1GoldenBuf[i + j]);
                    }
                    else
                    {
                        Printf("%02x ", pu1GoldenBuf[i + j]);
                    }
                }
                Printf("\n");
                
                Printf("Read buffer: \n");
                for (j = 0; j < 0x20; j++) 
                {
                    if (j % 16 == 0)
                    {
                        Printf("\n%03x: %02x ", j, pu1TestBuf[i + j]);
                    }
                    else
                    {
                        Printf("%02x ", pu1TestBuf[i + j]);
                    }
                }
    
                Printf("\n");
                return -1;
            }
        }
    }
    
    return 0;

}

static INT32 _MSDCCLI_PartInfo(INT32 i4Argc, const CHAR ** szArgv)
{
    MSDCPART_ShowPartition();
    return 0;
}

#if CC_MSDC_SDMMC_TEST
static INT32 _MSDCCLI_RegisterTest(INT32 i4Argc, const CHAR ** szArgv)
{
    return MSDCTest_Register();
}

static INT32 _MSDCCLI_DetectionTest(INT32 i4Argc, const CHAR ** szArgv)
{
    return MSDCTest_Detection();
}

static INT32 _MSDCCLI_ProtectionTest(INT32 i4Argc, const CHAR ** szArgv)
{
    return MSDCTest_Protection();
}

static INT32 _MSDCCLI_DataAutoTest(INT32 i4Argc, const CHAR ** szArgv)
{
    return MSDCTest_DataAutoTest((UINT32 *)MSDC_TESTBUF1_ADDR, (UINT32 *)MSDC_TESTBUF2_ADDR);
}

static INT32 _MSDCCLI_DataEndAutoTest(INT32 i4Argc, const CHAR ** szArgv)
{
    return MSDCTest_DataEndAutoTest((UINT32 *)MSDC_TESTBUF1_ADDR, (UINT32 *)MSDC_TESTBUF2_ADDR);
}

static INT32 _MSDCCLI_BootModeAutoTest(INT32 i4Argc, const CHAR ** szArgv)
{
    return MSDCTest_BootModeAutoTest((UINT32 *)MSDC_TESTBUF1_ADDR, (UINT32 *)MSDC_TESTBUF2_ADDR);
}

static INT32 _MSDCCLI_BootIntTest(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 test_mode = 0;
    
    if (i4Argc != 2)
    {
        LOG(0, "bootinttest [0/1/2/3/4]\n");
        LOG(0, "     0 - normal case\n");
        LOG(0, "     1 - boot ack timeout\n");
        LOG(0, "     2 - boot ack crc error\n");
        LOG(0, "     3 - data timeout\n");
        LOG(0, "     4 - data crc error\n");
        return 0;
    }
    
    test_mode = StrToInt(szArgv[1]);

    return MSDCTest_BootInt((UINT32 *)MSDC_TESTBUF1_ADDR, (UINT32 *)MSDC_TESTBUF2_ADDR, test_mode);
}

static INT32 _MSDCCLI_AutoTuning(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 target = 0;
    
    if (i4Argc != 2)
    {
        LOG(0, "at [0~12]\n");
        LOG(0, "     0 - pad_clk_txdly\n");
        LOG(0, "     1 - pad_cmd_rxdly\n");
        LOG(0, "     2 - pad_dat_rd_rxdly\n");
        LOG(0, "     3 - pad_dat_wr_rxdly\n");
        LOG(0, "     4 - dat0_rd_dly\n");
		LOG(0, "     5 - dat1_rd_dly\n");
        LOG(0, "     6 - dat2_rd_dly\n");
        LOG(0, "     7 - dat3_rd_dly\n");
        LOG(0, "     8 - dat4_rd_dly\n");
        LOG(0, "     9 - dat5_rd_dly\n");
        LOG(0, "     10 - dat6_rd_dly\n");
		LOG(0, "     11 - dat7_rd_dly\n");
		LOG(0, "     12 - cmd_resp_dly\n");
		LOG(0, "     13~15 - reserved\n");
        return 0;
    }
    
    target = StrToInt(szArgv[1]);
	if(target >= 13)
	{
        LOG(0, "invalid value for test!\n");
		return -1;
	}

    return MSDCTest_AutoTuning(target);
}
#endif

#ifdef MSDC_SDIO_TEST
static INT32 _MSDCCLI_SDIOInit(INT32 i4Argc, const CHAR ** szArgv)
{
    return MSDCSDIO_Init();
}

static INT32 _MSDCCLI_SDIOGetCIA(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 mode = 0;
    
    if (i4Argc != 2)
    {
        LOG(0, "sdiogc [0/1]\n");
        LOG(0, "     0 - cmd52 byte read\n");
        LOG(0, "     1 - cmd53 bytes read\n");
        return 0;
    }
    
    mode = StrToInt(szArgv[1]);
	if((mode != 0) &&
	   (mode != 1))
	{
        LOG(0, "invalid value for get cia!\n");
		return -1;
	}

    return MSDCSDIO_GetCIA(mode);
}


static INT32 _MSDCCLI_SDIOParseCIA(INT32 i4Argc, const CHAR ** szArgv)
{
    return MSDCSDIO_ParseCIA();
}

static INT32 _MSDCCLI_SDIOSetBusWidth(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 buswidth = 0;
    
    if (i4Argc != 2)
    {
        LOG(0, "sdiosbw [1/4]\n");
        LOG(0, "     1 - 1 bit\n");
        LOG(0, "     4 - 4 bit\n");
        return 0;
    }
    
    buswidth = StrToInt(szArgv[1]);
	if((buswidth != 1) &&
	   (buswidth != 4))
	{
        LOG(0, "invalid value for buswidth!\n");
		return -1;
	}

	return MSDCSDIO_SetBusWidth(buswidth);
}

static INT32 _MSDCCLI_SDIOSetBlockLen(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 blklen = 0;
    
    if (i4Argc != 2)
    {
        LOG(0, "sdiosbl [1~512]\n");
        return 0;
    }
    
    blklen = StrToInt(szArgv[1]);
	if((blklen < 1) ||
	   (blklen > 512))
	{
        LOG(0, "invalid value for buswidth!\n");
		return -1;
	}

	return MSDCSDIO_SetBlockLen(blklen);
}

static INT32 _MSDCCLI_SDIOReadTest(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 addr = 0, len = 0, offset = 0, mode = 0;
    
    if (i4Argc != 5)
    {
        LOG(0, "sdiort addr offset len mode\n");
        return 0;
    }
    
    addr = StrToInt(szArgv[1]);
	offset = StrToInt(szArgv[2]);
	len = StrToInt(szArgv[3]);
	mode = StrToInt(szArgv[4]);

	return MSDCSDIO_ReadTest(addr, offset, len, mode);
}

static INT32 _MSDCCLI_SDIOReadAutoTest(INT32 i4Argc, const CHAR ** szArgv)
{
	return MSDCSDIO_ReadAutoTest();
}
#endif
#endif

/******************************************************************************
* MSDC function list
******************************************************************************/
static CLI_EXEC_T arMsdcCmdTbl[] =
{
#ifdef CC_MSDC_ENABLE
    DECLARE_CMD(_MSDCCLI_HostSet, hostset, hs, "msdc.hs"),
    DECLARE_CMD(_MSDCCLI_HostGet, hostget, hg, "msdc.hg"),
    DECLARE_CMD(_MSDCCLI_Pinmux, pinmux, pnx, "msdc.pinmux"),
    DECLARE_CMD(_MSDCCLI_SwitchVoltage, switchvoltage, sv, "msdc.sv"),
    DECLARE_CMD(_MSDCCLI_SysInit, init, init, "msdc.init"),
    DECLARE_CMD(_MSDCCLI_Identify, identify, id, "msdc.identify"),
    DECLARE_CMD(_MSDCCLI_SetDataMode, setdatamode, sdm, "msdc.setdatamode"),
    DECLARE_CMD(_MSDCCLI_SetDataEndMode, setdataendmode, sdem, "msdc.setdataendmode"),
    DECLARE_CMD(_MSDCCLI_SetDataBusWidth, setbuswidth, sbw, "msdc.setbuswidth"),
    DECLARE_CMD(_MSDCCLI_SetSpeedMode, setspeedmode, ssm, "msdc.setspeedmode"),
    DECLARE_CMD(_MSDCCLI_SetDDRMode, setddrmode, sdddrm, "msdc.setddrmode"),
    DECLARE_CMD(_MSDCCLI_SetBusClock, setbusclock, sbc, "msdc.setbusclock"),
    DECLARE_CMD(_MSDCCLI_PrintConfigInfo, configinfo, pci, "msdc.configinfo"),
    DECLARE_CMD(_MSDCCLI_SetContinueClock, setcontinueclock, scc, "msdc.setcontinueclock"),
    DECLARE_CMD(_MSDCCLI_GetRegisters, getregisters, gr, "msdc.getregisters"),
    DECLARE_CMD(_MSDCCLI_GetDevInfo, getdevinfo, gdi, "msdc.getdevinfo"),
    DECLARE_CMD(_MSDCCLI_Read, read, r, "msdc.read"),	
    DECLARE_CMD(_MSDCCLI_Write, write, w, "msdc.write"),
    DECLARE_CMD(_MSDCCLI_Erase, erase, e, "msdc.erase"),
    DECLARE_CMD(_MSDCCLI_Read_Perform, readperform, rp, "msdc.readperform"),	
    DECLARE_CMD(_MSDCCLI_Write_Perform, writeperform, wpe, "msdc.writeperform"),
    DECLARE_CMD(_MSDCCLI_WriteProtect, writeprotect, wp, "msdc.writeprotect"),
    DECLARE_CMD(_MSDCCLI_Measure, measure, m, "msdc.measure"),
    DECLARE_CMD(_MSDCCLI_PartInfo, partinfo, partinfo, "msdc.partinfo"), 
#ifdef CC_MSDC_SDMMC_TEST
    DECLARE_CMD(_MSDCCLI_RegisterTest, registertest, rt, "msdc.registertest"),
    DECLARE_CMD(_MSDCCLI_DetectionTest, detectiontest, dt, "msdc.detectiontest"), 
    DECLARE_CMD(_MSDCCLI_ProtectionTest, protectiontest, pt, "msdc.protectiontest"), 
    DECLARE_CMD(_MSDCCLI_DataAutoTest, dataautotest, dat, "msdc.dataautotest"),
    DECLARE_CMD(_MSDCCLI_DataEndAutoTest, dataendautotest, deat, "msdc.dataendautotest"),
    DECLARE_CMD(_MSDCCLI_BootModeAutoTest, bootmodeautotest, bmat, "msdc.bootmodeautotest"),
    DECLARE_CMD(_MSDCCLI_BootIntTest, bootinttest, bit, "msdc.bootinttest"),
    DECLARE_CMD(_MSDCCLI_AutoTuning, autotuning, at, "msdc.autotuning"),
#endif
#ifdef CC_MSDC_SDIO_TEST
    DECLARE_CMD(_MSDCCLI_SDIOInit, sdioinit, sdioinit, "msdc.sdioinit"),
    DECLARE_CMD(_MSDCCLI_SDIOGetCIA, sdiogetcia, sdiogc, "msdc.sdiogetcia"),
    DECLARE_CMD(_MSDCCLI_SDIOParseCIA, sdioparsecia, sdiopc, "msdc.sdioparsecia"),
    DECLARE_CMD(_MSDCCLI_SDIOSetBusWidth, sdiosetbuswidth, sdiosbw, "msdc.sdiosetbuswidth"),
    DECLARE_CMD(_MSDCCLI_SDIOSetBlockLen, sdiosetblocklen, sdiosbl, "msdc.sdiosetblocklen"),
    DECLARE_CMD(_MSDCCLI_SDIOReadTest, sdioreadtest, sdiort, "msdc.sdioreadtest"),
    DECLARE_CMD(_MSDCCLI_SDIOReadAutoTest, sdioreadautotest, sdiorat, "msdc.sdioreadautotest"),
#endif
#endif
    DECLARE_END_ITEM(),
};

CLI_MAIN_COMMAND_ITEM(Msdc)
{
    "msdc",
    NULL,
    NULL,
    arMsdcCmdTbl,
    "Msdc command",
    CLI_GUEST
};


