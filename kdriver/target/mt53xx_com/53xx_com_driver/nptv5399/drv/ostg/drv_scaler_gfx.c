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
 * $Author: p4admin $
 * $Date: 2015/01/13 $
 * $RCSfile: drv_scaler_gfx.c,v $
 * $Revision: #3 $
 *
 *---------------------------------------------------------------------------*/

/** @file drv_scaler_nonlinear.c
 *  Brief of file drv_scaler_nonlinear.c.
 *  Details of file drv_scaler_nonlinear.c (optional).
 */

//-----------------------------------------------------------------------------
// Include files
//----------------------------------------------------------------------------- 
#include "scpos_debug.h"
//#include "hw_scpos.h"
#include "drv_scaler_gfx.h"
#include "sv_const.h"  
#include "vdo_misc.h"
#include "drv_scaler_drvif.h"
#include "drv_scaler.h"
#include "drv_display.h"


#if defined(CC_MT5399)||defined(CC_MT5882)
#include "drv_mon_out.h"
#endif

R3DGfxInf _arGfxInf=
{
	0xff,	//	UINT8 u1PathIn;
	0xff,	//	UINT8 u1Sel444;
	0xff,	//	UINT8 bypass3x3;

	0,      //    UINT32 u4Width
	0,      //    UINT32 u4Height
    
	0xff,	//	UINT8 u1FrameNumber;
	0xff,	//	UINT8 u1WriteMode;
	0,		//	UINT8 u1WriteEnable;
	0,		//	UINT8 u13D22DEnable;

	0xffffffff,	//	UINT32 u4BassAddr0;
	0xffffffff,	//	UINT32 u4BassAddrOffset;
	0xffffffff,	//	UINT32 u4WAddrHigh;
	0xffffffff,	//	UINT32 u4WAddrLow;

	FORMAT_NONE,	//	SCPIP_GFX_FORMAT Format;
	0xff,		//	UINT8 u1AlphaPos;
	0x7f,		//	UINT8 u1AlphaValue;
	
	0,			//	UINT8 u1HBoundEn;
	0xffff,	//	UINT32 u2HBoundValue;
	
	0,			//	UINT8 u1FrameDropEn;
	0xff,	//	UINT32 u4DropM;
	0xff,	//	UINT32 u4DropN;

	1,		//	UINT8 u1MuxB;
	0,		//	UINT8 u1MuxG;
	2,		//	UINT8 u1MuxR;
};

UINT8 VssWriteFreeze = SV_FALSE;
UINT8 VssWriteUnFreeze[GFX_SRC_SEL_MAX] = {SV_FALSE,SV_FALSE,SV_FALSE,SV_FALSE,SV_FALSE,SV_FALSE,SV_FALSE};


static UINT8 VssInited=0;

/**
 * @brief Graphic dump initial function.
 * @param void
 * @return void
*/
void  vScpip_GFX_init(void)
{
	UINT8 bsrc = GFX_SRC_SEL_MAX;
    VssInited=1;
	VssWriteFreeze = SV_FALSE;
	    
    for(bsrc = 0;bsrc < GFX_SRC_SEL_MAX; bsrc++)
    {
    	VssWriteUnFreeze[bsrc] = SV_FALSE;
    }
	
    LOG(4,"Vss Inited\n");
}

/**
 * @brief select which path write gfx
 * @param bPath (SCPIP_GFX_SRC_SEL) 0:Main / 1:Sub / 2:Main_DI / 3:Sub_DI / 4:Main_PDS / 5:Sub_PDS / 6:Monitor_out
 * @return true
 */
UINT8 u1Scpip_GFX_Source_Sel(UINT8 bPath)
{	
	MonOutPicInfo* pMonOutInfo;
	UINT8  MonOutMuxSelIn;

	_arGfxInf.u1PathIn = bPath;
	pMonOutInfo = getMonOutInfo();
	MonOutMuxSelIn = pMonOutInfo->u1MonOutInSel;
  
	vScpipGfxSrcSel(bPath);
	
	if(_arGfxInf.u1PathIn == GFX_MON_OUT)
	{
		if(MonOutMuxSelIn == E_MON_OUT_MAIN_FSC_OUT || MonOutMuxSelIn == E_MON_OUT_SUB_FSC_OUT || MonOutMuxSelIn == E_MON_OUT_MIXER)
		{
			if (fgIsMJCToOSTG())
			{
				vScpipSetGfxCLKSelReg(FLD_POCLK_SEL_DPLL);
			}
			else
			{
				vScpipSetGfxCLKSelReg(FLD_POCLK_SEL_OCLK);
			}	
		}
		else if(MonOutMuxSelIn == E_MON_OUT_MAIN_MIB_OUT || MonOutMuxSelIn == E_MON_OUT_SUB_MIB_OUT ||
				MonOutMuxSelIn == E_MON_OUT_SUB_PREDOWN || MonOutMuxSelIn == E_MON_OUT_MAIN_PREDOWN)
		{
			vScpipSetGfxCLKSelReg(FLD_POCLK_SEL_MIB_OCLK);
		}
		else if(MonOutMuxSelIn == E_MON_OUT_PANEL)
		{
			vScpipSetGfxCLKSelReg(FLD_POCLK_SEL_OCLK);
		}	 
	}
	else
	{
		vScpipSetGfxCLKSelReg(FLD_POCLK_SEL_MIB_OCLK);
	}

	if(u4DrvVrmGetAppFlag() & VRM_APP_GRAPHIC_DUMP) // VRM trigger
	{
		if (bPath == GFX_MAIN_SCL || bPath == GFX_MAIN_DI || bPath == GFX_MAIN_PDS)
		{
			vVRMSetEventFlg(SV_VP_MAIN, VRM_EVENT_BY_APP);
		}
		else if (bPath == GFX_SUB_SCL || bPath == GFX_SUB_DI || bPath == GFX_SUB_PDS)
		{
			vVRMSetEventFlg(SV_VP_PIP,	VRM_EVENT_BY_APP);
		}
		else  // monitor out
		{
			vMonOutSetOutput(E_MON_OUT_GFX);
		}
	}

	return TRUE;
}


/**
 * @brief Gfx update resolution
 * @param u4Width
 * @param u4Height
 */
UINT8 u1Scpip_GFX_Update_Resolution(UINT32 u4Width, UINT32 u4Height)
{	
    _arGfxInf.u4Width  = u4Width;
    _arGfxInf.u4Height = u4Height;

    vScpipGfxResolution(u4Width, u4Height);
    vScpipGfxSetResolution(0);

    return TRUE;
}

/**
 * @brief Get current Gfx source sel
 * @param void
 * @return 0:Main / 1:Sub / 2:Main_DI / 3:Sub_DI / 4:Main_PDS / 5:Sub_PDS / 6:Monitor_out
 */
UINT8 u1Scpip_GFX_GetSource(void)
{
    return _arGfxInf.u1PathIn;
}

/**
 * @brief Get current Gfx width
 * @param void
 * @return Gfx width setting
 */
UINT32 u4Scpip_GFX_GetWidth(void)
{
    return _arGfxInf.u4Width;
}

/**
 * @brief Get current Gfx height
 * @param void
 * @return Gfx height setting
 */
UINT32 u4Scpip_GFX_GetHeight(void)
{
    return _arGfxInf.u4Height;
}

/**
 * @brief Get current write frame index
 * @param void
 * @return 0~(N-1), N is frame number.
 */
UINT8 u1Scpip_GFX_GetCurrentFrameIdx(void)
{
    return u1ScpipGfxGetFrameIdx();
}

/**
 * @brief Get current write status
 * @param void
 * @return 0: stop writing, 1: still writing
 */
UINT8 u1Scpip_GFX_GetWriteStatus(void)
{
    return u1ScpipGFXGetWriteStatus();
}


/**
 * @brief set frame number
 * @param frame number
 * @return true:OK, FLASE: frame number limited to 4.
 */
UINT8 u1Scpip_GFX_Set_Frame_Num(UINT8 u1Number)
{
	if(u1Number > 4)
	{
		return FALSE;
	}
	else
	{
		_arGfxInf.u1FrameNumber = u1Number;
		
		vScpipGfxSetFrameNum(u1Number-1);
		
		return TRUE;
	}
}

/**
 * @brief init buffer to "0" 
 * @param fu4BaseAddr 
  * @param u4Size means one frame size 
 * @return none
 */
#include "x_hal_arm.h"
void u1Scpip_GFX_Init_Dram(void)
{
	UINT32 u4BaseAddr,u4TotalFrameSize ,u4FrameSize;

	if(VssInited == 0) 
	{
		 LOG(2,"Vss not Inited\n");
		 return;
	}

	u4BaseAddr	= _arGfxInf.u4BassAddr0 << 4 ;
	u4FrameSize = _arGfxInf.u4BassAddrOffset << 4 ;
	
	u4TotalFrameSize = u4FrameSize * _arGfxInf.u1FrameNumber;
	
	LOG(2,"~~~ init GFX dram to 0 ~~~\n");

	x_memset((VOID*)(VIRTUAL(u4BaseAddr)) , 0, u4TotalFrameSize);
	
	HalFlushInvalidateDCacheMultipleLine((VIRTUAL(u4BaseAddr)),u4TotalFrameSize);
	
}

/**
 * @brief set address
 * @param fu4BaseAddr 
  * @param u4Size means one frame size 
 * @return true
 */
UINT8 u1Scpip_GFX_Set_Dram_Address(UINT32 u4BaseAddr, UINT32 u4FrameSize, UINT8 u1FrameNum)
{
	UINT8 u1Idx=0;
	
	u4BaseAddr	>>= 4;
	u4FrameSize >>= 4;

	if(u1FrameNum > 4)
	{
		u1FrameNum = 4;  // maximum 4 frames.
	}

	_arGfxInf.u4BassAddr0 = u4BaseAddr;
	_arGfxInf.u4BassAddrOffset = u4FrameSize;
	_arGfxInf.u1FrameNumber = u1FrameNum;
	_arGfxInf.u4WAddrLow = u4BaseAddr;
	_arGfxInf.u4WAddrHigh = u4BaseAddr + (u1FrameNum * u4FrameSize);

	vScpipGfxSetFrameNum(u1FrameNum-1);

	// update frame addresses
	for(u1Idx=0; u1Idx<u1FrameNum; u1Idx++)
	{
		vScpipGfxSetAddrBase(u1Idx, u4BaseAddr+(u4FrameSize*u1Idx));
	}
	
	vScpipGfxSetAddrLimit(_arGfxInf.u4WAddrHigh, _arGfxInf.u4WAddrLow);

	//u1Scpip_GFX_Init_Dram();

	return TRUE;
	
}

/**
 * @brief select trigger mode or flag mode
 * @param 0 continue mode ,  1 trigger mode
 * @return true
 */
UINT8 u1Scpip_GFX_Set_Write_Mode(UINT8 u1Select)
{
    _arGfxInf.u1WriteMode = u1Select;
    vScpipGfxSetWriteEnMode(u1Select);
    return TRUE;
}

/**
 * @brief trigger 0->1
 * @return true
 */
#if defined(SCALER_WA1_GFX)
extern UINT8 _VSS_SW_Trigger;
#endif
UINT8 u1Scpip_GFX_Write_Trigger(void)
{
	if(_arGfxInf.u1WriteMode)
	{
#if defined(SCALER_WA1_GFX)
        UINT8 u1TriggerCnt = 0;
		_VSS_SW_Trigger = 1;
#endif
		vScpipGfxWriteTrigger(1);
#if defined(SCALER_WA1_GFX)
        while((u1ScpipGFXGetWriteStatus() == 0) && u1TriggerCnt < 10)
        {
            LOG(1, "DEBUG trigger %d polling\n", u1TriggerCnt++);
            vScpipGfxWriteTrigger(1);
        }
        if ((u1ScpipGFXGetWriteStatus() == 0) && u1TriggerCnt >= 10)
        {
            LOG(0, "DEBUG trigger FAILED!!");
            return FALSE;
        }
#endif
		return TRUE;
	}
	else
	{	
		LOG(4,"warning: GFX not trigger mode \n");
		return FALSE;
	}
}

/**
 * @brief enable 3DGFX write
 * @param on/off
 * @return true
 */
UINT8 u1Scpip_GFX_Write_Enable(UINT8 u1Enable)
{
	if(u1Enable == TRUE)
	{
		if(_arGfxInf.u1PathIn == GFX_MAIN_PDS)
		{
			_arGfxInf.u1Sel444 = 1;
			_arGfxInf.u1Bypass3x3 = (bIsScalerInputRGB(SV_VP_MAIN) == SV_TRUE)? 1 :0;
		}
		else if( _arGfxInf.u1PathIn == GFX_SUB_PDS || _arGfxInf.u1PathIn == GFX_SUB_SCL)
		{
			_arGfxInf.u1Sel444 = 1;
			_arGfxInf.u1Bypass3x3 = (bIsScalerInputRGB(SV_VP_PIP) == SV_TRUE)? 1 :0;
		}
		else if(_arGfxInf.u1PathIn == GFX_MAIN_SCL)
		{
			_arGfxInf.u1Sel444 = (bIsScalerInput444(SV_VP_MAIN) == SV_TRUE)? 1 :0;
			_arGfxInf.u1Bypass3x3 = (bIsScalerInputRGB(SV_VP_MAIN) == SV_TRUE)? 1 :0;
		}
		else if(_arGfxInf.u1PathIn == GFX_MAIN_DI)
		{
		   _arGfxInf.u1Bypass3x3 = (bIsScalerInputRGB(SV_VP_MAIN) == SV_TRUE)? 1 :0;
		   if(_arGfxInf.u1Bypass3x3)
		   {
				_arGfxInf.u1Sel444 = 1;
		   }
		   else
		   {
				_arGfxInf.u1Sel444 = 0;
		   }
		}
		else if(_arGfxInf.u1PathIn == GFX_SUB_DI)
		{
			_arGfxInf.u1Bypass3x3 = (bIsScalerInputRGB(SV_VP_PIP) == SV_TRUE)? 1 :0;
			if(_arGfxInf.u1Bypass3x3)
			{
				 _arGfxInf.u1Sel444 = 1;
			}
			else
			{
				 _arGfxInf.u1Sel444 = 0;
			}
		}
		else		//if(_arGfxInf.u1PathIn == GFX_MON_OUT)
		{
			_arGfxInf.u1Sel444 = 1;//monitor out outout to vss point before 444to4222
			_arGfxInf.u1Bypass3x3 = 0;//monitout out output always YUV
		}
	}
	else
	{
		_arGfxInf.u1Sel444 = 1;
		_arGfxInf.u1Bypass3x3 = 0;
	}

	if(_arGfxInf.Format == YUVA2101010)
	{
		_arGfxInf.u1WriteEnable = u1Enable;
		 _arGfxInf.u1HBoundEn = !(_arGfxInf.u1Sel444);

		//if input RGB444 how to dump YUV by VSS, change to video mode?
		//vScpipGfx422To444Enable(_arGfxInf.u1Sel444);
		//vScpipGfxBypass3x3(!(_arGfxInf.u1Bypass3x3));
		vScpipGfxWriteEn(u1Enable);	
	}
	else
	{
		_arGfxInf.u1WriteEnable = u1Enable;
		 _arGfxInf.u1HBoundEn = !(_arGfxInf.u1Sel444);
			
		vScpipGfx422To444Enable(!(_arGfxInf.u1Sel444));
		vScpipGfxBypass3x3(_arGfxInf.u1Bypass3x3);
		vScpipGfxWriteEn(u1Enable);
		
	}
		
	return TRUE;
	
}

void u1Scpip_GFX_Write_Freeze(UINT8 u1Freeze)
{
	UINT8 u1PathIn ;
	
	u1PathIn = u1Scpip_GFX_GetSource();
	
	if(u1Freeze == SV_TRUE)
	{
		VssWriteFreeze = SV_TRUE;
	}
	else
	{
		VssWriteUnFreeze[u1PathIn] = SV_TRUE;
	}
}


/**
 * @brief set  3d to 2d enable 
 * @param path 
 * @param enable
 * @return true
 */
UINT8 u1Scpip_GFX_Set_3DTo2D_En(UINT8 u1VdpId, UINT8 u1Enable)
{
    _arGfxInf.u13D22DEnable = u1Enable;

    vScpipGfx3DTo2DEnable(u1VdpId, u1Enable);
    return TRUE;
}

/**
 * @brief set  3d to 2d enable 
 * @param path 
 * @param enable
 * @param u4DropM  u4DropN  ,deside drop ratio
 * @return true
 */
UINT8 u1Scpip_GFX_Set_Frame_Drop(UINT8 u1VdpId, UINT8 u1Enable, UINT8 u1DropM, UINT8 u1DropN)
{
	_arGfxInf.u1FrameDropEn = u1Enable;
	_arGfxInf.u1DropM = u1DropM;
	_arGfxInf.u1DropN = u1DropN;
	vScpipGfxFrameDropEn(u1VdpId, u1Enable, u1DropM, u1DropN);

	return TRUE;
}

/**
 * @brief set date format 
 * @param u1Mode mode select
 * @param u1AlphaPos alpha positon ,ARGB,RGBA
 * @param u1Alpha  set alpha value 
 * @return true
 */
UINT8 u1Scpip_GFX_Set_Alpha(UINT8 u1Alpha)
{
    _arGfxInf.u1AlphaValue = u1Alpha;

    vScpipGfxSetAlpha(u1Alpha);
    return TRUE;
}

/**
 * @brief set  dump format
 * @param Format ,ARGB8888 / RGB565
 * @return true
 */
UINT8 u1Scpip_GFX_Set_Dump_Format(SCPIP_GFX_FORMAT Format)
{
    switch(Format) 
    {
        case RGBA8888:
			vScpipGfxSetMode(1);
			vScpipGfxAlphaPosSel(0);
			vScpipGfxSetMux(1, 0, 2);
            _arGfxInf.Format = RGBA8888;
            _arGfxInf.u1AlphaPos = 0;  //Scaler format( 0:ARGB , 1:RGBA) different order with OpenGL.
            _arGfxInf.u1MuxB = 2;
            _arGfxInf.u1MuxR = 1;
			
            break;
       case RGBA2101010:
			vScpipGfxSetMode(3);
			vScpipGfxAlphaPosSel(0);
			vScpipGfxSetMux(2, 0, 1);
            _arGfxInf.Format = RGBA2101010;
            _arGfxInf.u1AlphaPos = 0;  //Scaler format( 0:ARGB , 1:RGBA) different order with OpenGL.
            _arGfxInf.u1MuxB = 1;
            _arGfxInf.u1MuxR = 2;
			
            break;

		case YUVA2101010:
			vScpipGfxSetMode(3);
			vScpipGfxAlphaPosSel(0);
			vScpipGfxSetMux(1, 2, 0);
            _arGfxInf.Format = RGBA2101010;
            _arGfxInf.u1AlphaPos = 0;  //Scaler format( 0:ARGB , 1:RGBA) different order with OpenGL.
            _arGfxInf.u1MuxR = 1;
            _arGfxInf.u1MuxG = 2;
			_arGfxInf.u1Sel444 = 0 ;
			
            break;

        case RGB565:
            vScpipGfxSetMode(2);
            vScpipGfxSetMux(2, 0, 1);
            _arGfxInf.Format = RGB565;
            _arGfxInf.u1MuxB = 1;
            _arGfxInf.u1MuxR = 2;
            break;
        default:
            break;
    }
    return TRUE;
}

/**
 * @brief query 3DGFX setting for cli cmd
 * @param bPath
 * @return true
 */
void vScpip_GFX_Query(void)
{
	//LOG(level,fmt...)
	
	LOG(0,"path(%d)=(%dx%d), WEnable(%d),WMode(%d),3Dto2D(%d)\n",_arGfxInf.u1PathIn,_arGfxInf.u4Width,_arGfxInf.u4Height,_arGfxInf.u1WriteEnable,_arGfxInf.u1WriteMode,_arGfxInf.u13D22DEnable);
	LOG(0,"Is444:(%d), Bypss3x3:(%d)\n",_arGfxInf.u1Sel444, _arGfxInf.u1Bypass3x3);
	LOG(0,"FrameNumber(%d),bassaddr(0x%x),offset(0x%x),addrlimit(0x%x,0x%x)\n",_arGfxInf.u1FrameNumber,_arGfxInf.u4BassAddr0,_arGfxInf.u4BassAddrOffset,_arGfxInf.u4WAddrLow,_arGfxInf.u4WAddrHigh);
	LOG(0,"boundary:en(%d),value(%d)\n",_arGfxInf.u1HBoundEn,_arGfxInf.u2HBoundValue);
	LOG(0,"Format:format(%d),alphapos(%d),alphavalue(%d)\n",_arGfxInf.Format,_arGfxInf.u1AlphaPos,_arGfxInf.u1AlphaValue);
	LOG(0,"Frame drop: en(%d),M(%d),N(%d)\n",_arGfxInf.u1FrameDropEn,_arGfxInf.u1DropM,_arGfxInf.u1DropN);
	LOG(0,"R-G-B:(%d,%d,%d)\n",_arGfxInf.u1MuxR,_arGfxInf.u1MuxG,_arGfxInf.u1MuxB);
	LOG(0,"idx:(%d), sts:(%d)\n",u1Scpip_GFX_GetCurrentFrameIdx(), u1Scpip_GFX_GetWriteStatus());
}
#ifdef CC_CDC_SUPPORT
#include "hw_ycproc.h"
#include "vdp_if.h"
UINT32 u4GfxDumpAddr ;
UINT32 u4GfxLen ;

void vScpipDumpInit(void)
{
    UINT32 u4Width, u4Height;    
    UINT32 u4DumpSize;    
    FBM_POOL_T* prPool;
	UINT32 u4ShiftBit = 16;    
	VDP_REGION_T VSSSrcRegion;
	VDP_OVERSCAN_REGION_T rRegion ;
	UINT8 bPath = SV_VP_MAIN;

	VSSSrcRegion.u4X = 0;
	VSSSrcRegion.u4Y = 0;
	VSSSrcRegion.u4Width  = 10000;
	VSSSrcRegion.u4Height = 10000;	
	x_memset(&rRegion, 0, sizeof(VDP_OVERSCAN_REGION_T));

	VDP_SetSrcRegion(bPath , 1, VSSSrcRegion); // Full screen
	VDP_SetOutRegion(bPath , 1, VSSSrcRegion); // Full screen
	VDP_SetOverScan(bPath, rRegion);
	

	//VDP_GetSrcSize(bPath, &u4LineSize, &u4Width, &u4Height);
	u4Width = _rMPicInfo.wTargetW ;
	u4Height = _rMPicInfo.wTargetH ;
	u4DumpSize = ((u4Width*u4Height*4+(1<<u4ShiftBit))>>u4ShiftBit)<<u4ShiftBit;

	prPool = FBM_GetPoolInfoAuto((UCHAR)FBM_POOL_TYPE_PQ_TOOL, NULL);	

    // in case there is no suitable space allocated in FBM, add new way to force allocate (known) unused space for dump usage
    if(u4IO32Read4B(SWRW_08) != 0)
    {
        prPool->u4Addr = u4IO32Read4B(SWRW_08);
    }

    vIO32Write4B(SWRW_01, prPool->u4Addr);
    //vIO32Write4B(SWRW_02, prPool->u4Size);
    vIO32Write4B(SWRW_02, u4DumpSize);
	///u4DumpSize = ((u4Width*u4Height*4+(1<<u4ShiftBit))>>u4ShiftBit)<<u4ShiftBit;
	LOG(0, " u4DumpSize =%d pool size =%d\n", u4DumpSize, prPool->u4Size);
	u4GfxDumpAddr = prPool->u4Addr;
	u4GfxLen = u4DumpSize;

    vIO32WriteFldAlign(SWRW_03, u4Width, DUMP_CTRL_WIDTH);
    vIO32WriteFldAlign(SWRW_03, u4Height, DUMP_CTRL_HEIGHT);
    vIO32WriteFldAlign(SWRW_03, DUMP_FMT_RGB_444_10BIT, DUMP_CTRL_FORMAT);    
    vIO32WriteFldAlign(SWRW_04, u4DumpSize, DUMP_CTRL_SIZE); 
}
#include "x_hal_arm.h"
void vScposGfxDumpStart(UINT8 u1Path)
{
	//vScpip_GFX_init();		
	LOG(0, "graphic dump start\n");
    HalFlushInvalidateDCache();  
	u1Scpip_GFX_Source_Sel(GFX_MAIN_SCL);
	u1Scpip_GFX_Set_Dram_Address(u4GfxDumpAddr, u4GfxLen, 3); //3 frame 
#if 0
	//u1Scpip_GFX_Set_Dump_Format(RGBA2101010);
#else
	u1Scpip_GFX_Set_Dump_Format(YUVA2101010);
	vScpipGfxBypass3x3(1);
#endif
#if 0
	u1Scpip_GFX_Set_Write_Mode(1); //trigger mode
#else
	u1Scpip_GFX_Set_Write_Mode(0); //continue mode
	vScpipGfxWriteEn(1);	
#endif
    vScpipGfx422To444Enable(0);
	u1Scpip_GFX_Set_Alpha(0);
	u1Scpip_GFX_Write_Trigger();
}

void vScposGfxDumpEnd(UINT8 u1Path)
{
	vScpipGfxWriteEn(0);	
}

void vScposGfxDumpPostProc(void)
{     
	LOG(0, "graphic dump end\n");
    // finish dump
//    vIO32WriteFldAlign(SWRW_04, 0, DUMP_CTRL_STATE);
 //   vIO32WriteFldAlign(SWRW_04, DUMP_ENG_NONE, DUMP_CTRL_ENG_SEL);  
}
#endif //#ifdef CC_CDC_SUPPORT

