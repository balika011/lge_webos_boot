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
 * $Date: 2015/01/26 $
 * $RCSfile: drv_scaler.c,v $
 * $Revision: #2 $
 *
 *---------------------------------------------------------------------------*/

/** @file drv_scaler.c
 *  Brief of file drv_scaler.c.
 *  Details of file drv_scaler.c (optional).
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#include "x_lint.h"
LINT_EXT_HEADER_BEGIN
// Scaler related header files
#include "hw_ckgen.h"
#include "hw_scpos.h"
#include "drv_scpos.h"
#include "drv_scaler.h"
#include "drv_upscaler.h"
#include "drv_scaler_drvif.h"
#include "frametrack_drvif.h"
#include "scpos_debug.h"

// nptv header files
#include "general.h"       
#include "sv_const.h"       
#include "video_def.h"      
#include "vdo_misc.h"       
#include "vdp_display.h"
#include "drv_video.h"   
#include "source_select.h"
#include "drv_display.h"
#include "panel.h"
#include "mute_if.h"
#ifdef CC_MT5399
#include "drv_mjc.h"
#endif

#include "drv_tdtv_drvif.h"

// drvif header files
#include "fbm_drvif.h"
#include "srm_drvif.h"
#include "vdp_drvif.h"
#include "nptv_drvif.h"

// Other header files
#include "c_model.h"
#include "x_bim.h"
#include "x_mid.h"
#include "x_assert.h"
#include "x_util.h"

LINT_EXT_HEADER_END

static UINT32 _au4ForceWriteOff[VDP_NS] = {0, 0};
//static UINT8 _bIsDramBufInit[2]= {0, 0};  //MOVE to _arScalePrm??
UINT8  _bSRMStable[2]= {SV_FALSE, SV_FALSE};     //MOVE to _arScalePrm??

EXTERN VDP_SCALER_PRM_T _arScalePrm[VDP_NS];
EXTERN UINT32 _au4OriFrameNum[VDP_NS];

#define CC_CHECK_READ_POINTER
#ifdef CC_CHECK_READ_POINTER
//static UINT8 _u1WritePointer[VDP_NS] = {0, 0};
static UINT8 _u1ReadEnCnt[VDP_NS] = {0xff, 0xff};
#define READ_DELAY_COUNTER (0xff)
static UINT8 _fgReadEnOpening[VDP_NS] = {SV_FALSE, SV_FALSE};
#endif

//static function
static SCALER_DRAM_INIT_STATE _eScpipGetDramInitState(UINT32 u4VdpId);
static void _vScpipSetDramInitState(UINT32 u4VdpId,SCALER_DRAM_INIT_STATE state);
static void _vScpipUpdatePageNum(UINT32 u4VdpId);
static void _vScpipToggleForceOff(UINT32 u4VdpId);
static void _vScpipUpdateLinePitch(UINT32 u4VdpId);

EXTERN void vScpipUpdateTdtvInfo(UINT32 u4VdpId, UINT32 u4TdtvPrm);
#ifdef CC_SCPOS_3DTV_SUPPORT
EXTERN UINT32 _u4ScpipReCalDelayCounter(UINT32 u4VdpId, UINT32 u4Delay);
#endif

static SCALER_DRAM_INIT_STATE _eScpipGetDramInitState(UINT32 u4VdpId)
{
    if(u4VdpId >=VDP_NS)
    {
        return SCALER_DRAM_INIT_STATE_NOT_INIT;
    }

    return _arScalePrm[u4VdpId].eDramInitState;
}

static void _vScpipSetDramInitState(UINT32 u4VdpId,SCALER_DRAM_INIT_STATE state)
{
    if(u4VdpId >=VDP_NS)
    {
        return;
    }

    _arScalePrm[u4VdpId].eDramInitState = state;
}


/**
 * @cobra porting done
 * @brief Check write control status
 * @param bPath VDP_1/VDP_2
 * @cobra porting done
 * @note write off condition
 *		 1. signal unstable
 *		 2. snow screen & freeze is on (snow screen is a special case. It's under ATV and signal is unstable)
 */
void vScpipCheckWriteCtrl(UINT8 bPath)
{
    RVChannel *prChannel;
    UINT8 bSigStatus;
    UINT8 bWriteOnOff= SV_OFF;

    bSigStatus = bDrvVideoSignalStatus(bPath);
    prChannel = getChannel(bPath);

    if (prChannel->bIsChannelOn == (UINT8)SV_ON)
    {
		#if SUPPORT_ATV_SNOWSCREEN
        if(fgIsSrcAtv(bPath) && (u4ApiVideoGetSnowScreen() ==SV_TRUE))
        {
            bWriteOnOff = ((prChannel->bIsFreeze == SV_OFF)? SV_ON : SV_OFF);
        }
        else // non-atv input
		#endif
        {
            if ((bSigStatus == (UINT8)SV_VDO_STABLE) && (prChannel->bIsFreeze == (UINT8)SV_OFF))
            {
                bWriteOnOff = SV_ON;
            }
            else
            {
                bWriteOnOff = SV_OFF;
            }
        }
    }
    #ifdef CC_CHECK_READ_POINTER
    if(_fgReadEnOpening[bPath]== SV_TRUE)
    {
        vScpipReconfig(bPath);
        _fgReadEnOpening[bPath] = SV_FALSE;
    }
    #endif 
    vScpipWriteCtrlOnOff(bPath, bWriteOnOff);
  
}

UINT32 vScpipReCalDelayCounter(UINT32 u4VdpId, UINT32 u4Delay)
{
    UINT32 u4InputFrameRate;
    VRM_INFO_T* preVrmInfo;
    UINT8  bPath = getScalerPath(u4VdpId);
    UINT32 u4OutputFrameRate = vDrvGetLCDFreq();

    preVrmInfo = VRMGetVRMInfo(bPath, VRM_MODULE_FSC);
    u4InputFrameRate=preVrmInfo->u2FrameRate;
    
    if(u4InputFrameRate == 0)
    {
        return u4Delay;
    }

    if(PANEL_IsSupport120Hz())
    {
        u4OutputFrameRate >>= 1;
    }

    u4Delay = ((u4OutputFrameRate * u4Delay) + (u4InputFrameRate - 1))/u4InputFrameRate;
    
    return u4Delay;
}

/**
 * @cobra porting done
 * @brief Turn On/Off Main Channel Write Control 
 * @param u1OnOff On/Off
 */
void vScpipWriteCtrlOnOff(UCHAR ucVdpId, UCHAR ucOnOff)
{
    CRIT_STATE_T csState;
    RVChannel *prChannel;
    UINT8 bPath;
    UINT16 dramOffset;

    if (ucVdpId >= VDP_NS)
    {
        return;
    }

    #if defined(CC_SRM_ON)
    //don't do anything when dram buffer isn't init yet.
    if(_eScpipGetDramInitState(ucVdpId) != SCALER_DRAM_INIT_STATE_REG_INITED)
    {
        return;
    }
    #endif
    
    if (ucOnOff == SV_ON && _arScalePrm[ucVdpId].u4CntToRstDramPrm != 0)
    {
        //do nothing
        return;
    }    

    prChannel = getChannel(ucVdpId);
    csState = x_crit_start();   
    bPath = getScalerPath(ucVdpId);
    dramOffset = getCtrlOffset(bPath);    

    /* when ucOnOff==SV_ON, need to check following condition,
     * If anyone of following condition is true, write enable won't be enabled.
     */
    if(ucOnOff == SV_ON)
    {
        //condition 1. force write off
        if(_au4ForceWriteOff[ucVdpId] == SV_ON)
        {
            ucOnOff = SV_OFF;
        }
        #if defined(CC_SCPOS_EN)
        //condition 2. scops not enabled
        if(_arScalePrm[ucVdpId].u4Enable ==0)
        {
            ucOnOff = SV_OFF;
        }
        #endif
        //condition3. channel is OFF
        if(prChannel->bIsChannelOn == SV_OFF)
        {
            ucOnOff = SV_OFF;
        }
        //condition4. channel is Freezed
        if(prChannel->bIsFreeze == SV_ON)
        {
            ucOnOff = SV_OFF;
        }
        //condition5. Scaler is NOT in Dram mode
        if(u1ScpipGetDispMode(getScalerPath(ucVdpId)) != VDP_SCPOS_DISPMODE_OFF)
        {
            ucOnOff = SV_OFF;
        }
        //condition6. dram read/write width is not ready -> do not enable read/write
        if ((RegReadFldAlign(SCPIP_SCCTRL1_1C+dramOffset, SCCTRL1_1C_SC_DRAM_W_WIDTH_1) == 0) ||
        (RegReadFldAlign(SCPIP_SCCTRL1_1C+dramOffset, SCCTRL1_1C_SC_DRAM_W_HEIGHT_1) == 0))
        {
            ucOnOff = SV_OFF;
        }
        //condition7. When only doing Venc, Scaler should not write dram.
        if(u4DrvVrmGetAppFlag() == VRM_APP_VIDEO_ENC && ucVdpId == VDP_2 && (u1DrvScpipGetVencModule() == E_VENC_MODULE_SUB_DI))
        {
            ucOnOff = SV_OFF;
        }
		//condition8. When scaler is bypassed, turn off dram read/write
        #if defined(CC_SUPPORT_4K2K)||defined(CC_SUPPORT_HDMI_4K2K30)
        if(getScalerInfo(bPath)->u4Bypass == SV_TRUE)
        {
            ucOnOff = SV_OFF;
        }
        #endif

    }
    
    if (ucOnOff == SV_ON)
    {
        #ifdef CC_SCPOS_LOG_DRAM_PARAM
        if (bScpipGetDramWrite(ucVdpId) == SV_OFF)    
        {
            HAL_TIME_T dt;
            HAL_GetTime(&dt);
            _u4LogIdx = (_u4LogIdx >= (CC_SCPOS_LOG_DRAM_PARAM-1))? 0: (_u4LogIdx+1);
            _au4AddressLog[_u4LogIdx] = SCPIP_PIP1_09+pipOffset;
            _au4ValueLog[_u4LogIdx] = (1);
            _au4TimeLog[_u4LogIdx] = (dt.u4Seconds * 1000000) + dt.u4Micros;
        }
        #endif
#ifdef CC_SCPOS_3DTV_SUPPORT                    
        //if(ucVdpId == VDP_1)
        {
            UINT32 u4MuteDelay = _vDrvVideoGetMuteDelay(getScalerPath(ucVdpId), MUTE_MODULE_SCPOS_TV3D);

            if(u4MuteDelay >= 0xffff) // when mute delay is big, it's FOREVER_MUTE
            {
                _vDrvVideoSetMute(MUTE_MODULE_SCPOS_TV3D, ucVdpId, _u4ScpipReCalDelayCounter(ucVdpId, VDP_SCPOS_TV3D_MUTE_DELAY), FALSE);
            }
        }
#endif
        vScpipSetDramWrite(ucVdpId, SV_ON);
        
        #ifdef CC_CHECK_READ_POINTER
        if(_u1ReadEnCnt[ucVdpId]==READ_DELAY_COUNTER )
        {
           //read enable after write 3 frame buffer
           _u1ReadEnCnt[ucVdpId]=vScpipReCalDelayCounter(ucVdpId, 3);
           //mute counter more than read delay counter
           _vDrvVideoSetMute(MUTE_MODULE_SCPOS_DRAM, ucVdpId,(vScpipReCalDelayCounter(ucVdpId, 3)+2), TRUE);
        }
        
        if (!_u1ReadEnCnt[ucVdpId])
        {
             vScpipSetDramRead(ucVdpId, SV_ON);
        }
         if (_u1ReadEnCnt[ucVdpId] > 0)
        {
            _u1ReadEnCnt[ucVdpId]--;
            if(_u1ReadEnCnt[ucVdpId]==0)
            {
                _fgReadEnOpening[ucVdpId] = SV_TRUE;
            }  
        }

        #else
        vScpipSetDramRead(ucVdpId, SV_ON);
        #endif
    } 
    else
    {        
#ifdef CC_SCPOS_LOG_DRAM_PARAM
        if (bScpipGetDramWrite(ucVdpId) == SV_ON)
        {
            HAL_TIME_T dt;
            HAL_GetTime(&dt);
            _u4LogIdx = (_u4LogIdx >= (CC_SCPOS_LOG_DRAM_PARAM-1))? 0: (_u4LogIdx+1);
            _au4AddressLog[_u4LogIdx] = SCPIP_PIP1_09+pipOffset;
            _au4ValueLog[_u4LogIdx] = (0);
            _au4TimeLog[_u4LogIdx] = (dt.u4Seconds * 1000000) + dt.u4Micros;
        }
#endif
        vScpipSetDramWrite(ucVdpId, SV_OFF);
#ifndef __MODEL_slt__
        //if not in freeze mode, turn off read control
        if(prChannel->bIsFreeze == SV_OFF)
        {
            if((u1ScpipGetDispMode(ucVdpId) == VDP_SCPOS_DISPMODE_OFF) && (bScpipGetDramWrite(ucVdpId) == SV_ON) )
            {
                _vDrvVideoSetMute(MUTE_MODULE_SCPOS_DRAM, ucVdpId, VDP_SCPOS_RESCALE_TIME+VDP_SCPOS_WEN_WAIT, TRUE);
            }
            
            vScpipSetDramRead(ucVdpId, SV_OFF);   
            #ifdef CC_CHECK_READ_POINTER
//            _u1WritePointer[ucVdpId] = RegReadFldAlign(SCPIP_SCCTRL1_1B+dramOffset, SCCTRL1_1B_SC_STA_DRAM_WPTR_1);            
            _u1ReadEnCnt[ucVdpId] = READ_DELAY_COUNTER;
            #endif
        }
#endif        
    }
    x_crit_end(csState);
}

/**
 *	@cobra review done. 
 *  @note: 1. Turn on write relatch when count to 0??
 *         2. called in input vsync isr.
 */
void vScpipResetDramCountDown(UINT8 bPath)
{
    CRIT_STATE_T csState;
    csState = x_crit_start();
    {    
        //counter != 0 and not in parameter update
        if(_arScalePrm[bPath].u4CntToRstDramPrm !=0)
        {
            _arScalePrm[bPath].u4CntToRstDramPrm--;
            if(_arScalePrm[bPath].u4CntToRstDramPrm==0)
            {
                vScpipWriteRelatchEn(bPath, SV_ON);
            }
        }
    }    
    x_crit_end(csState);
}

/**
 *	@cobra porting done
 *  Turn off write enable immediately and set counter.
 *  Write enable will not be turned on before counter clear to 0.
 */
static void _vScpipToggleForceOff(UINT32 u4VdpId)
{
	UINT8 bPath;
	CRIT_STATE_T csState; 

	bPath = getScalerPath(u4VdpId);

    LOG(3, "vDrvScpipForceWENOff: Disable (%d) Write Relatch\n", u4VdpId); 
	    
    csState = x_crit_start(); 
    _arScalePrm[u4VdpId].u4CntToRstDramPrm = (_arScalePrm[u4VdpId].u4CntToRstDramPrm < 2)? 2: _arScalePrm[u4VdpId].u4CntToRstDramPrm;
	#ifdef CC_SUPPORT_PREPROC_TIMING
	if(bGetVideoDecType(u4VdpId) == SV_VD_PREPROC)
	{
	    _arScalePrm[u4VdpId].u4CntToRstDramPrm = 0;
	}
	#endif
    vScpipWriteRelatchEn(bPath, SV_OFF);
    x_crit_end(csState);
    vScpipWriteCtrlOnOff(u4VdpId, SV_OFF);      	
}

/**
 * @cobra review done
 * @brief Force to turn Off Channel Write Control 
 * @param u4VdpId VDP_1/VDP_2
 * @return u1OnOff On/Off
 */
void vScpipForceWriteOff(UINT32 u4VdpId, UINT8 u1OnOff) 
{
    CRIT_STATE_T csState;

    if (u1OnOff)
    {
        LOG(9, "Enable SCPOS(%d) Force-write-control-off\n", u4VdpId);
    }
    else
    {
        LOG(9, "Cancel SCPOS(%d) Force-write-control-off\n", u4VdpId);
    }
    csState = x_crit_start();    
    _au4ForceWriteOff[u4VdpId] = u1OnOff;
    x_crit_end(csState);   
}
/**
 * @cobra review done
**/

static void _vScpipSrmUpdateDramInfo(UINT32 u4VdpId, FBM_POOL_T* prPool, FBM_POOL_T* prMddiPool, UINT32 u4Mode)
{
    //update scaler dram info
    _arScalePrm[u4VdpId].rFbmInfo.u4Addr = prPool->u4Addr;
    _arScalePrm[u4VdpId].rFbmInfo.u4Width = prPool->u4Width;
    _arScalePrm[u4VdpId].rFbmInfo.u4Height = prPool->u4Height;
    _arScalePrm[u4VdpId].rFbmInfo.u4FbmSize = prPool->u4Size;       
    _arScalePrm[u4VdpId].rFbmInfo.fg2FB = ((u4Mode & FBM_POOL_MODE_SCPOS_2FB) > 0)? 1: 0;
    _arScalePrm[u4VdpId].rFbmInfo.fg10bit = ((u4Mode & FBM_POOL_MODE_10BIT) > 0)? 1: 0;
    _arScalePrm[u4VdpId].rFbmInfo.fg8bit = ((u4Mode & FBM_POOL_MODE_8BIT) > 0)? 1: 0;
    _arScalePrm[u4VdpId].rFbmInfo.fg422 = ((u4Mode & FBM_POOL_MODE_422_FB) > 0)? 1: 0;

    //When calculating pre-down factor, need to consider how much dram Pscan get from SRM
    _arScalePrm[u4VdpId].rFbmInfo.u4MddiDramWidth = prMddiPool->u4Width;
    
    LOG(3, "[SRM Callback]Set Scaler Buffer: addr=0x%x, w=%d, h=%d, 2FB=%d, 10bit=%d, DIWidth=%d\n", 
            (UINT32)_arScalePrm[u4VdpId].rFbmInfo.u4Addr, 
            (UINT32)_arScalePrm[u4VdpId].rFbmInfo.u4Width, 
            (UINT32)_arScalePrm[u4VdpId].rFbmInfo.u4Height, 
            (UINT32)_arScalePrm[u4VdpId].rFbmInfo.fg2FB,
            (UINT32)_arScalePrm[u4VdpId].rFbmInfo.fg10bit, 
            (UINT32)_arScalePrm[u4VdpId].rFbmInfo.u4MddiDramWidth);
}
/**
 * @cobra need review mjc dispmode
**/

static void _vDrvScpipSrmUpdateDispmode(UINT32 u4VdpId, UINT32 u4Mode)
{
    if (u4Mode & FBM_POOL_MODE_UNSTABLE)
    {
        return;
    }
    if (u4Mode & FBM_POOL_MODE_SCPOS_FULL)
    {
        _VDP_DispModeEnable((UCHAR)u4VdpId, (UCHAR)VDP_SCPOS_DISPMODE_OFF);
    }
    else if (u4Mode & FBM_POOL_MODE_SCPOS_PSCAN_DISP)
    {
        _VDP_DispModeEnable((UCHAR)u4VdpId, (UCHAR)VDP_SCPOS_PSCAN_DISPMODE);
    }
    else if (u4Mode & FBM_POOL_MODE_SCPOS_LINESYNC_DISP)
    {
        _VDP_DispModeEnable((UCHAR)u4VdpId, (UCHAR)VDP_SCPOS_LINESYNC_DISPMODE);
    }
    else if (u4Mode & FBM_POOL_MODE_SCPOS_MJC_DISP)
    {
        //@cobra review this?
        ASSERT(0); // no mjc display mode in 5365/5395        
    }
    else
    {
        ASSERT(0); // should never get in here!
    }    
}
/**
 *	@cobra porting done
**/
static void _vScpipUpdatePageNum(UINT32 u4VdpId)
{
    SCALER_DRAM_ADDR_T* dramAddrInfo;
    SCALER_FBM_POOL_T* fbmInfo;
    UINT32 u4FbmWidth;
    UINT32 u4FbmHeight;
    UINT32 u4FbTotalSize;
    UINT8 bIs10Bit;
    UINT8 bIs422;
    UINT32 u4FrameNum;
    UINT32 u4UsedPage;
    UINT32 u4Padding;
    
    dramAddrInfo = getDramAddrInfo(u4VdpId);
    fbmInfo = getFbmPoolInfo(u4VdpId);    
    u4FbmWidth = fbmInfo->u4Width;
    u4FbmHeight = fbmInfo->u4Height;
    u4FrameNum = dramAddrInfo->FrameNum;    
    u4FbTotalSize = (fbmInfo->u4FbmSize) >> 11;  //total page number from FBM
    
    bIs10Bit = fbmInfo->fg10bit;
    bIs422 = fbmInfo->fg422;
    
    dramAddrInfo->MsbPageNum = ((u4FbmWidth + 255) / 256) * (((u4FbmHeight* u4FrameNum) + 7) / 8);
    if(bIs10Bit == SV_TRUE)
    {
        dramAddrInfo->LsbPageNum = (((u4FbmWidth + 255) / 256) * (((u4FbmHeight* u4FrameNum) + 255) / 256)) * 8;
    }
    else
    {
        dramAddrInfo->LsbPageNum = 0;
    }
    
    if(bIs10Bit == SV_TRUE && bIs422 == SV_TRUE)    //422 10 bit
    {
        u4UsedPage = (dramAddrInfo->MsbPageNum * 2) + (dramAddrInfo->LsbPageNum * 2);
        u4Padding  = 8;
    }
    else if(bIs10Bit == SV_FALSE && bIs422 == SV_TRUE)    //422 8 bit
    {
        u4UsedPage = (dramAddrInfo->MsbPageNum * 2);
        u4Padding  = 8;
    }
    else if(bIs10Bit == SV_TRUE && bIs422 == SV_FALSE)    //444 10 bit
    {
        u4UsedPage = (dramAddrInfo->MsbPageNum * 3) + (dramAddrInfo->LsbPageNum * 3);
        u4Padding  = 12;
    }
    else if(bIs10Bit == SV_FALSE && bIs422 == SV_FALSE)    //444 8 bit
    {
        u4UsedPage = (dramAddrInfo->MsbPageNum * 3);
        u4Padding  = 12;
    }
    else
    {
        u4UsedPage = 0;
        u4Padding  = 0;
    }

    LOG(1, "Path(%d) FBM width (%d) height (%d)\n",u4VdpId, u4FbmWidth, fbmInfo->u4Height);
    LOG(1, "Path(%d) FBM allocated Page Num(%d) Used Page Num by Calculation(%d) Padding (%d)\n", u4VdpId, u4FbTotalSize, u4UsedPage, u4Padding);
    LOG(1, "Path(%d) MSB Page Num(%d) x %d\n", u4VdpId, dramAddrInfo->MsbPageNum, (bIs422==1)?2:3);
    if(bIs10Bit == SV_TRUE)
    {
        LOG(1, "Path(%d) LSB Page Num(%d) x %d\n",u4VdpId, dramAddrInfo->LsbPageNum, (bIs422==1)?2:3);
    }
    if(u4FbTotalSize < u4UsedPage + u4Padding)
    {
        LOG(1, "Path(%d) FBM buffer to small!!!\n",u4VdpId);
    }
}

#if 0
static void _vScpipUpdatePageNum(UINT32 u4VdpId)
{
    SCALER_DRAM_ADDR_T* dramAddrInfo;
    SCALER_FBM_POOL_T* fbmInfo;
    UINT32 u4FbTotalSize;
    UINT32 u4FbUnitSize;
    UINT8 u1Divider = 1;    //to avoid divide by 0
    UINT8 bIs10Bit;
    UINT8 bIs422;

    dramAddrInfo = getDramAddrInfo(u4VdpId);
    fbmInfo = getFbmPoolInfo(u4VdpId);    
    u4FbTotalSize = (fbmInfo->u4FbmSize) >> 11;  //total page number from FBM
    bIs10Bit = fbmInfo->fg10bit;
    bIs422 = fbmInfo->fg422;
    
    if(bIs10Bit == SV_TRUE && bIs422 == SV_TRUE)    //422 10 bit
    {
        u4FbTotalSize -= 8; // fbm size +padding
        u1Divider = 10;  // (Y,U) MSB 4 unit for each, (Y,U) LSB 1 unit for each
    }
    else if(bIs10Bit == SV_FALSE && bIs422 == SV_TRUE)    //422 8 bit
    {
        u4FbTotalSize -= 8; // fbm size +padding
        u1Divider = 8;  // (Y,U) MSB 4 unit for each
    }
    else if(bIs10Bit == SV_TRUE && bIs422 == SV_FALSE)    //444 10 bit
    {
        u4FbTotalSize -= 12; // fbm size +padding, FIXME
        u1Divider = 15; // (Y,U,V) MSB 4 unit for each, (Y,U,V) LSB 1 unit for each
    }
    else if(bIs10Bit == SV_FALSE && bIs422 == SV_FALSE)    //444 8 bit
    {
        u4FbTotalSize -= 12; // fbm size +padding
        u1Divider = 12; // (Y,U,V) MSB 4 unit for each
    }

    u4FbUnitSize = u4FbTotalSize / u1Divider;

    dramAddrInfo->MsbPageNum = u4FbUnitSize * 4;

    if(bIs10Bit == SV_TRUE)
    {
        dramAddrInfo->LsbPageNum = u4FbUnitSize;
    }
    else
    {
        dramAddrInfo->LsbPageNum = 0;
    }
    LOG(1,"Update Scaler Dram Page Info. MSB Page number (0x%x) LSB Page number(0x%x)\n ",
        dramAddrInfo->MsbPageNum,dramAddrInfo->LsbPageNum);    
}
#endif
/**
 *	@cobra porting done
**/

static void _vScpipCalBufferAddress(UINT32 u4VdpId)
{
    SCALER_DRAM_ADDR_T* dramAddrInfo;
    SCALER_FBM_POOL_T* fbmInfo;
    UINT32 u4MsbTotalPage;
    UINT32 u4LsbTotalPage;

    dramAddrInfo = getDramAddrInfo(u4VdpId);
    fbmInfo= getFbmPoolInfo(u4VdpId);
    u4MsbTotalPage = dramAddrInfo->MsbPageNum;
    u4LsbTotalPage = dramAddrInfo->LsbPageNum;

    //1. Low bound
    dramAddrInfo->LowAddr = fbmInfo->u4Addr >> 11;
    //2. Y Msb
    dramAddrInfo->YMsbAddr = fbmInfo->u4Addr >> 11; 
    //3. Y Lsb = Y Msb + Msb Total Page
    dramAddrInfo->YLsbAddr = MAKE_ALIGN_TO((dramAddrInfo->YMsbAddr + u4MsbTotalPage), 3, (((dramAddrInfo->LowAddr)%8)+2)%8);
    //4. U Msb = Y Lsb + Lsb Total Page
    dramAddrInfo->UMsbAddr = MAKE_ALIGN_TO((dramAddrInfo->YLsbAddr + u4LsbTotalPage), 3, (((dramAddrInfo->LowAddr)%8)+4)%8);
    //5. U Lsb = U Msb + Msb Total Page
    dramAddrInfo->ULsbAddr = MAKE_ALIGN_TO((dramAddrInfo->UMsbAddr + u4MsbTotalPage), 3, (((dramAddrInfo->LowAddr)%8)+6)%8);
    //6. V Msb & V Lsb
    if(fbmInfo->fg422 == SV_FALSE)  //444
    {
        dramAddrInfo->VMsbAddr = MAKE_ALIGN_TO((dramAddrInfo->ULsbAddr + u4LsbTotalPage), 3, (((dramAddrInfo->LowAddr)%8)+0)%8);
        dramAddrInfo->VLsbAddr = MAKE_ALIGN_TO((dramAddrInfo->VMsbAddr + u4MsbTotalPage), 3, (((dramAddrInfo->LowAddr)%8)+2)%8);
    }
    else    //422
    {
        dramAddrInfo->VMsbAddr = dramAddrInfo->UMsbAddr;
        dramAddrInfo->VLsbAddr = dramAddrInfo->ULsbAddr;
    }
    //8. High bound
    //dramAddrInfo->HighAddr = MAKE_ALIGN_TO(((fbmInfo->u4Addr+fbmInfo->u4FbmSize)>>11), 3, (((dramAddrInfo->LowAddr)%8)+4)%8);
    dramAddrInfo->HighAddr = (fbmInfo->u4Addr + fbmInfo->u4FbmSize)>>11; // FIXME

    if(u4VdpId == VDP_2 && bDrvVrmIsAppFlagSet(VRM_APP_VIDEO_ENC) && (u1DrvScpipGetVencModule() == E_VENC_MODULE_SUB_SCALER))
    {
        dramAddrInfo->LowAddr = u4ScpipGetDramLimit(E_VENC_MODULE_SUB_SCALER, 0);
        dramAddrInfo->HighAddr = u4ScpipGetDramLimit(E_VENC_MODULE_SUB_SCALER, 1);
        LOG(1,"VENC Low (0x%x) High(0x%x)\n",dramAddrInfo->LowAddr,  dramAddrInfo->HighAddr);  
    }
    LOG(1,"Update Path(%d) Scaler Dram Info\n",u4VdpId);
    LOG(1,"==========================\n");    
    LOG(1,"YMSB(0x%x) YLSB(0x%x)\n",dramAddrInfo->YMsbAddr, dramAddrInfo->YLsbAddr);
    LOG(1,"UMSB(0x%x) ULSB(0x%x)\n",dramAddrInfo->UMsbAddr, dramAddrInfo->ULsbAddr);
    LOG(1,"VMSB(0x%x) VLSB(0x%x)\n",dramAddrInfo->VMsbAddr, dramAddrInfo->VLsbAddr);
    LOG(1,"Low (0x%x) High(0x%x)\n",dramAddrInfo->LowAddr,  dramAddrInfo->HighAddr);    
    LOG(1,"==========================\n");    
}
/**
 *	@cobra porting done
**/

static void _vScpipUpdateFrameNumber(UINT32 u4VdpId)
{
    UINT32 u4FrameNum;
    
    // 1. scaler dram mode flip (4 frames)
#if defined(CC_FLIP_MIRROR_SUPPORT)
    if(u4GetFlipMirrorModule(u4VdpId)==FLIP_BY_SCALER)
    {	
        u4FrameNum = 4;       
    }
    else if(u1ScpipGetDispMode(u4VdpId) == VDP_SCPOS_DISPMODE_OFF) //dram mode
    {
        u4FrameNum = 3; 	  
    }
    else                                                              //disp mode
    {
        u4FrameNum = 2; 	  
    }
#else    
    if(u1ScpipGetDispMode(u4VdpId) == VDP_SCPOS_DISPMODE_OFF)      //dram mode
    {
        u4FrameNum = 3; 	  
    }
    else                                                              //disp mode
    {
        u4FrameNum = 2; 	  
    }
#endif

    //keep frame number
    _au4OriFrameNum[u4VdpId] = u4FrameNum;

    //game mode, force to 2 frame
    if ((_arScalePrm[u4VdpId].u4IsGameMode==SV_TRUE) && (u4FrameNum > 2))
    {
        LOG(3, "GameMode: Force scalar to 2 frame mode! (%d)\n", u4FrameNum);
        u4FrameNum = 2;
    }

    _arScalePrm[u4VdpId].rDramAddrInfo.FrameNum = u4FrameNum;
}
/**
 *	@cobra porting done
**/
static void _vScpipUpdateLinePitch(UINT32 u4VdpId)
{
    UINT8 bPath;
    VDP_SCALER_PRM_T* scalerInfo;
    SCALER_FBM_POOL_T* fbmInfo;
    UINT16 width;

    bPath = getScalerPath(u4VdpId);
    scalerInfo = getScalerInfo(bPath);
    fbmInfo= getFbmPoolInfo(u4VdpId);

    //width = MIN(wDrvVideoInputWidth(bPath), fbmInfo->u4Width);
    if (wDrvVideoInputWidth(bPath) == 0) // unstable
    {
        width = fbmInfo->u4Width;
    }
    else
    {
        width = MIN(wDrvVideoInputWidth(bPath), fbmInfo->u4Width);
    }
    
    //calculate dram write picth  (in cobra, line pitch change to 256 align)
    scalerInfo->u4DramLinePitch = DIVIDE_WITH_CARRIER((UINT16)width,256);
    LOG(1, "Path(%d) Line Pitch(%d) Width (%d)\n",u4VdpId, scalerInfo->u4DramLinePitch, width);
}
/**
 *	@cobra porting done
**/

void vScpipUpdateDramPrm(UINT32 u4VdpId, const SCALER_FBM_POOL_T* prFbmPool)
{
    UINT8 bPath;
    UINT8 bMode;
    
    if (u4VdpId >= VDP_NS)
    {
        return;
    }

    bPath = getScalerPath(u4VdpId);
    bMode = getScalerMode(bPath);
    if(bMode != VDP_SCPOS_DISPMODE_OFF)
    {
        return;
    }

    //1. update frame number to write
    _vScpipUpdateFrameNumber(u4VdpId);
    //2. update Msb & Lsb Page Number
    _vScpipUpdatePageNum(u4VdpId);
    //3. calculate Dram base address & Low/High limit
    _vScpipCalBufferAddress(u4VdpId);
    //4. update line pitch
    _vScpipUpdateLinePitch(u4VdpId);
    
}
/**
 *	@cobra porting done
**/

void vScpipUpdateDramReg(UINT32 u4VdpId)
{
    UINT8 bPath,bMode;
    UINT16 dramOffset;
    SCALER_DRAM_ADDR_T* dramInfo;
    VDP_SCALER_PRM_T* scalerInfo;
        
    bPath = getScalerPath(u4VdpId);
    dramOffset = getDramOffset(bPath);
    dramInfo = getDramAddrInfo(u4VdpId);
    scalerInfo = getScalerInfo(bPath);
	bMode = getScalerMode(bPath);
	
	if(bMode != VDP_SCPOS_DISPMODE_OFF)
    {
        return;
    }
    
    if(dramInfo->YMsbAddr == SCPIP_READ_FLD(SCPIP_DRAM_M_04+dramOffset, DRAM_M_04_DA_ADDR_BASE_MSB_Y) &&
       dramInfo->YLsbAddr == SCPIP_READ_FLD(SCPIP_DRAM_M_05+dramOffset, DRAM_M_05_DA_ADDR_BASE_LSB_Y) &&
       dramInfo->UMsbAddr == SCPIP_READ_FLD(SCPIP_DRAM_M_06+dramOffset, DRAM_M_06_DA_ADDR_BASE_MSB_U) &&
       dramInfo->ULsbAddr == SCPIP_READ_FLD(SCPIP_DRAM_M_07+dramOffset, DRAM_M_07_DA_ADDR_BASE_LSB_U) &&
       dramInfo->VMsbAddr == SCPIP_READ_FLD(SCPIP_DRAM_M_08+dramOffset, DRAM_M_08_DA_ADDR_BASE_MSB_V) &&
       dramInfo->VLsbAddr == SCPIP_READ_FLD(SCPIP_DRAM_M_09+dramOffset, DRAM_M_09_DA_ADDR_BASE_LSB_V) &&
       dramInfo->HighAddr  == SCPIP_READ_FLD(SCPIP_DRAM_M_0A+dramOffset, DRAM_M_0A_DA_WADDR_HI_LIMIT) &&
       dramInfo->LowAddr == SCPIP_READ_FLD(SCPIP_DRAM_M_0B+dramOffset, DRAM_M_0B_DA_WADDR_LO_LIMIT) &&
       dramInfo->FrameNum == u4ScpipGetBufNum(u4VdpId))
    {
        //all the same. just return
        return;
    }
	//if dram parameter chang ,set mute 
    _vDrvVideoSetMute(MUTE_MODULE_SCPOS_DRAM, u4VdpId, VDP_SCPOS_RESCALE_TIME+VDP_SCPOS_WEN_WAIT, TRUE);

    //embrace with critical section
    
    //1. turn off dram write & set counter
    _vScpipToggleForceOff(u4VdpId);

    //2. update base address
    SCPIP_WRITE_FLD(SCPIP_DRAM_M_04+dramOffset, dramInfo->YMsbAddr , DRAM_M_04_DA_ADDR_BASE_MSB_Y);
    SCPIP_WRITE_FLD(SCPIP_DRAM_M_05+dramOffset, dramInfo->YLsbAddr , DRAM_M_05_DA_ADDR_BASE_LSB_Y);
    SCPIP_WRITE_FLD(SCPIP_DRAM_M_06+dramOffset, dramInfo->UMsbAddr , DRAM_M_06_DA_ADDR_BASE_MSB_U);
    SCPIP_WRITE_FLD(SCPIP_DRAM_M_07+dramOffset, dramInfo->ULsbAddr , DRAM_M_07_DA_ADDR_BASE_LSB_U);
    SCPIP_WRITE_FLD(SCPIP_DRAM_M_08+dramOffset, dramInfo->VMsbAddr , DRAM_M_08_DA_ADDR_BASE_MSB_V);
    SCPIP_WRITE_FLD(SCPIP_DRAM_M_09+dramOffset, dramInfo->VLsbAddr , DRAM_M_09_DA_ADDR_BASE_LSB_V);
    SCPIP_WRITE_FLD(SCPIP_DRAM_M_0A+dramOffset, dramInfo->HighAddr , DRAM_M_0A_DA_WADDR_HI_LIMIT);
    SCPIP_WRITE_FLD(SCPIP_DRAM_M_0B+dramOffset, dramInfo->LowAddr , DRAM_M_0B_DA_WADDR_LO_LIMIT);
    //3. update frame number
    vScpipSetBufNum(u4VdpId,dramInfo->FrameNum);
    //4. set Dram line pitch
    vScpipSetDramLinePitch(u4VdpId, scalerInfo->u4DramLinePitch);

    //5. set this means dram register has been inited.
    if(_eScpipGetDramInitState(u4VdpId) == SCALER_DRAM_INIT_STATE_SRM_INITED)
    {
        _vScpipSetDramInitState(u4VdpId, SCALER_DRAM_INIT_STATE_REG_INITED);
    }
}

/**
 *   @cobra review done
 *  scaler control call back
 *
 * @param void
 * @return void
 */
void vDrvScpipSrmDispCb(UINT32 u4VdpId, UINT32 u4Mode, UINT32 u4PDSize, UINT32 u4SramMode)
{
#ifdef CC_SCPOS_EN
    FBM_POOL_T* prPool;
    FBM_POOL_T* prMddiPool;

    LOG(3, "[SRM] DISP Callback vdp%d mode=0x%x, pds=%d, sram=%d\n", u4VdpId, u4Mode, u4PDSize, u4SramMode);
    
    // if srm is unstable
    if (u4Mode & FBM_POOL_MODE_UNSTABLE)
    {    
        //if unstable, force dram write off
        vScpipForceWriteOff(u4VdpId, SV_ON);    
        vScpipWriteCtrlOnOff(u4VdpId, SV_OFF);
        
        //disable dispmode
        _VDP_DispModeEnable((UINT8)u4VdpId, (UCHAR)VDP_SCPOS_DISPMODE_OFF);        
        LOG(3, "[SRM] Unstable callback! Disable Dispmode!\n");        
        
        //set srm unstable flag
        _bSRMStable[u4VdpId]=SV_FALSE;
        vScpipSetTdtvInType(u4VdpId, SCALER_TDTV_IN_TYPE_2D);
        vScpipSetTdtvOutType( u4VdpId, SCALER_TDTV_OUT_TYPE_2D );
        return;
    }

    //1. update dispmode
    _vDrvScpipSrmUpdateDispmode(u4VdpId, u4Mode);

    //2. update TDTV info
    vScpipUpdateTdtvInfo(u4VdpId,u4SramMode);


    //get Scaler & Pscan Pool info
    if (u4VdpId == VDP_1)
    {
       
        #if defined(CC_SUPPORT_4K2K)||defined(CC_SUPPORT_HDMI_4K2K30)
        if(fgIs4k2kNativeTimingOut(VDP_1))
        {
            #if defined(CC_SUPPORT_HDMI_4K2K30)
            prPool = FBM_GetPoolInfoAuto((UCHAR)FBM_POOL_TYPE_SCPOS_MAIN, NULL);
            #else
            prPool = FBM_GetPoolInfoAuto((UCHAR)FBM_POOL_TYPE_4K_2K_SCALER, NULL);
            #endif
        }
        else
        #endif
        {
            prPool = FBM_GetPoolInfoAuto((UCHAR)FBM_POOL_TYPE_SCPOS_MAIN, NULL);
        }
        prMddiPool = FBM_GetPoolInfoAuto((UCHAR)FBM_POOL_TYPE_MDDI_MAIN, NULL);
    }
    else
    {
        prPool = FBM_GetPoolInfoAuto((UCHAR)FBM_POOL_TYPE_SCPOS_PIP, NULL);
        prMddiPool = FBM_GetPoolInfoAuto((UCHAR)FBM_POOL_TYPE_MDDI_PIP, NULL);
    }

    //3. pool is valid, update dram info.
    if ((prPool != NULL) && (((u4Mode & FBM_POOL_MODE_UNSTABLE) == 0) || (u4Mode & FBM_POOL_MODE_SCPOS_MM_BUFCHG)))
    {
        //srm is stable, disable "force dram write off"
        vScpipForceWriteOff(u4VdpId, SV_OFF);
        //srm is stable
        _bSRMStable[u4VdpId]=SV_TRUE;  

        _vScpipSrmUpdateDramInfo(u4VdpId, prPool, prMddiPool, u4Mode);
        
        #ifdef CC_FLIP_MIRROR_SUPPORT
        //@6896 review this
        vScpipConfigFlip(u4VdpId);
        #endif
        //vScpipSrmSetBuf(u4VdpId,&rScalerDramInfo);    //move this step to scaler mode change
        if(_eScpipGetDramInitState(u4VdpId) == SCALER_DRAM_INIT_STATE_NOT_INIT)
        {
            _vScpipSetDramInitState(u4VdpId, SCALER_DRAM_INIT_STATE_SRM_INITED);
        }

        vScpipUpdateDramPrm(u4VdpId,&_arScalePrm[u4VdpId].rFbmInfo);
        vScpipUpdateDramReg(u4VdpId);
    }
    else
    {
        if (prPool == NULL)
        {
            LOG(1, "_VdpSrmScalerWeCb: fail to get pool\n");
        }
    }

    //4. fire scaler mode change
    vScpipReconfig(u4VdpId);
    vDrvSetImportWaitFlag(u4VdpId, VRM_IMPORT_WAIT_FLAG_FSC);
#endif
}
/**
 * @cobra porting done
**/

#if 1 //for capture log
void vScpipGetDramInf(UINT8 bPath,SCALER_INFO_T* inf)
{ 
	SCALER_DRAM_ADDR_T* dramInfo;
	VDP_SCALER_PRM_T* scalerInfo;
	
	dramInfo = getDramAddrInfo(bPath);
	scalerInfo = getScalerInfo(bPath);
		
	inf->YMsbAddr = dramInfo->YMsbAddr << 11;
	inf->YLsbAddr = dramInfo->YLsbAddr << 11;
	inf->UMsbAddr = dramInfo->UMsbAddr << 11;
	inf->ULsbAddr = dramInfo->ULsbAddr << 11;
	inf->VMsbAddr = dramInfo->VMsbAddr << 11;
	inf->VLsbAddr = dramInfo->VLsbAddr << 11;
	inf->FrameNum = 3;
 
	inf->PictureWidth = scalerInfo->u4DramWidth;
	inf->PictureHeight = scalerInfo->u4DramHeight;
    //need review in cobra
	inf->DramPitch = (scalerInfo->u4DramLinePitch)<<8;
	
	inf->u1BitRes = (_arScalePrm[bPath].u410BitMode == 1) ? 10 : 8;
	inf->u1ColorMode = (_arScalePrm[bPath].u4Is444 == 1) ? 2 : 0;

	inf->BlockMode = 1; // 1:256*8 0:128*16  cobra always per block 256 pixels
	
	inf->bDramSnakeMapping = vScpipGetDramSnakeMap(bPath);

#ifdef CC_FLIP_MIRROR_SUPPORT
	inf->u1CfgFlipMirror	= (scalerInfo->u4FlipEnable << 1) + scalerInfo->u4MirrorEnable;
	if (inf->u1CfgFlipMirror > 0)
	{
		inf->u1DramFlipMirror = 1;
	}
	else
	{
		inf->u1DramFlipMirror = 0;
	}
#endif	
	if (bDrvVideoSignalStatus(bPath) != SV_VDO_STABLE)
	{
		inf->PictureWidth = 0;
		inf->PictureHeight = 0;
	}
	
}
#endif

