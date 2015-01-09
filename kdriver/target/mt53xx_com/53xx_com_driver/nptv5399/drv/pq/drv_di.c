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
 * $RCSfile: drv_di.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/


////////////////////////////////////////////////////////////////////////////////
// Include files
////////////////////////////////////////////////////////////////////////////////

#include "hw_ckgen.h"
#include "hw_di.h"
#include "hw_di_int.h"
#include "hw_nr.h"
#include "hw_sw.h"
#if defined(CC_MT5399)
#include "hw_mjcfd.h"
#endif
#include "hw_dvi.h"
#include "hw_ycproc.h"
#include "hw_regen.h"
#include "hw_tvd.h"

#include "sv_const.h"
#include "vdo_misc.h"
#include "video_def.h"
#include "video_timing.h"
#include "vga_table.h"

#include "drv_di.h"
#include "drv_di_int.h"
#include "di_debug.h"
#include "drv_hdtv.h"
#include "drv_nr.h"
#include "drv_scaler.h"
#include "drv_scpos.h"
#include "drv_tdtv.h"
#include "drv_tdtv_drvif.h"
#include "drv_display.h"
#include "vdo_rm.h"
#include "drv_predown.h"

#include "mute_if.h"
#include "source_select.h"

#include "fbm_drvif.h"
#include "srm_drvif.h"

#include "x_lint.h"
LINT_EXT_HEADER_BEGIN


#include "x_ckgen.h"
#include "x_os.h"
#include "x_assert.h"
#include "x_bim.h"
#include "x_timer.h"
#include "x_util.h"
#include "x_hal_5381.h"


////////////////////////////////////////////////////////////////////////////////
//  External Function
////////////////////////////////////////////////////////////////////////////////

static void _vDrvDICfgPQ(UINT8 u1VdpId);


////////////////////////////////////////////////////////////////////////////////
//  Macro & Define
////////////////////////////////////////////////////////////////////////////////

//VGA Source HTotal maybe adjust by UI, not follow VGA Table
#define MIB_VGA_HTOTAL_BOOST    (0x80)

#define DMOD_M  (1<<0)    //Support Main
#define DMOD_S  (1<<1)    //Support Sub


typedef enum{
    E_HW_KR_2D     =0x0,
    E_HW_KR_FS     =0x3,
    E_HW_KR_TB     =0x4,
    E_HW_KR_SBS    =0x8,
    E_HW_KR_LI     =0xC
}E_HW_KR_FMT;

typedef enum{
    E_HW_DA_FS     =0x0,
    E_HW_DA_SBS    =0x1,
    E_HW_DA_TB     =0x2,
    E_HW_DA_LI     =0x3
}E_HW_DA_FMT;

typedef enum{
    E_HW_HV_NO     =0x0,
    E_HW_HV_HALF   =0x2,
    E_HW_HV_DOUBLE =0x3
}E_HW_HV_FORCE;


////////////////////////////////////////////////////////////////////////////////
//  Global variables enum
////////////////////////////////////////////////////////////////////////////////
MIB_STATIC_STATE _arMDDiPrm[VDP_NS];
MIB_DYN_STATE rMIBState[VDP_NS];
MDDI_FBM_POOL_T rMIBFbmPool[VDP_NS];

static HANDLE_T _hDIPQIsrSema;


UINT8 u1FlipMirrorConfig = DI_NORMAL_MODE;

DI_3D_IN_OUT_FORMAT r3DInOutFormat[VDP_NS]=
{
    {0,E_TD_IN_2D_I,E_TD_OUT_NATIVE},
    {0,E_TD_IN_2D_I,E_TD_OUT_NATIVE}
};

#ifdef CC_FPGA_DEBUG
#define MDDI_Y_BASE_ADDR_M 0x3000000
#define MDDI_Y_BASE_ADDR_P 0x5000000
EXTERN UINT32 u4DiInWidth;
EXTERN UINT32 u4DiInHeight;
#endif

extern void vDrvDIAdaptivePDSemaUnlock(void);
extern void vDrvR2CIntervalPatch(UINT16 u2MIBWidth, UINT16 u2MIBHight);

///////////NR Still Need To Be Update ~~

const rDataMode aDataMode[]=
{
    //Interlace
    DATAMODE_ARA(E_MIB_DMODE_UNKNOWN,0xFF,0xFF),
    //Scaler FRC
    DATAMODE_ARA(E(I, NR_OFF ,DI_Y1C1  ,FRC_SCL),  0, DMOD_M | DMOD_S),
    DATAMODE_ARA(E(I, NR_Y   ,DI_OFF   ,FRC_SCL), 16, DMOD_M),// for Game Mode with NR, Scaler Bob 
    DATAMODE_ARA(E(I, NR_OFF ,DI_Y2C1  ,FRC_SCL), 16, DMOD_M),
    DATAMODE_ARA(E(I, NR_YC  ,DI_OFF   ,FRC_SCL), 32, DMOD_M),// for Game Mode with NR, Scaler Bob 
    DATAMODE_ARA(E(I, NR_OFF ,DI_Y2C2  ,FRC_SCL), 32, DMOD_M),
    DATAMODE_ARA(E(I, NR_OFF ,DI_Y4C1  ,FRC_SCL), 48, DMOD_S),
    DATAMODE_ARA(E(I, NR_YC  ,DI_Y4C1  ,FRC_SCL), 48, DMOD_M),
    DATAMODE_ARA(E(I, NR_YC  ,DI_Y4C2  ,FRC_SCL), 56, DMOD_M),
    DATAMODE_ARA(E(I, NR_OFF ,DI_Y4C4  ,FRC_SCL), 64, DMOD_M | DMOD_S), 
    DATAMODE_ARA(E(I, NR_YC  ,DI_Y4C4  ,FRC_SCL), 64, DMOD_M),  
    //MIB FRC
    DATAMODE_ARA(E(I, NR_OFF ,DI_Y1C1  ,FRC_MIB), 32, DMOD_M | DMOD_S),
    DATAMODE_ARA(E(I, NR_OFF ,DI_Y2C1  ,FRC_MIB), 40, DMOD_M),
    DATAMODE_ARA(E(I, NR_Y   ,DI_OFF   ,FRC_MIB), 48, DMOD_M),// for Game Mode with NR, Scaler Bob 
    DATAMODE_ARA(E(I, NR_OFF ,DI_Y2C2  ,FRC_MIB), 48, DMOD_M),
    DATAMODE_ARA(E(I, NR_OFF ,DI_Y3C1  ,FRC_MIB), 48, DMOD_M),
    DATAMODE_ARA(E(I, NR_OFF ,DI_Y3C2  ,FRC_MIB), 56, DMOD_M),
    DATAMODE_ARA(E(I, NR_OFF ,DI_Y4C1  ,FRC_MIB), 56, DMOD_M | DMOD_S),
    DATAMODE_ARA(E(I, NR_YC  ,DI_OFF   ,FRC_MIB), 64, DMOD_M),// for Game Mode with NR, Scaler Bob 
    DATAMODE_ARA(E(I, NR_Y   ,DI_Y4C1  ,FRC_MIB), 64, DMOD_M),
    DATAMODE_ARA(E(I, NR_OFF ,DI_Y4C2  ,FRC_MIB), 64, DMOD_M),
    DATAMODE_ARA(E(I, NR_Y   ,DI_Y4C2  ,FRC_MIB), 72, DMOD_M),
    DATAMODE_ARA(E(I, NR_YC  ,DI_Y4C1  ,FRC_MIB), 80, DMOD_M),
    DATAMODE_ARA(E(I, NR_OFF ,DI_Y4C4  ,FRC_MIB), 80, DMOD_M | DMOD_S),
    DATAMODE_ARA(E(I, NR_YC  ,DI_Y4C2  ,FRC_MIB), 88, DMOD_M),
    DATAMODE_ARA(E(I, NR_Y   ,DI_Y4C4  ,FRC_MIB), 88, DMOD_M),
    DATAMODE_ARA(E(I, NR_YC  ,DI_Y4C4  ,FRC_MIB), 96, DMOD_M),
    //Progressive
    DATAMODE_ARA(E(P,NR_OFF  ,DI_OFF   ,FRC_SCL),  0, DMOD_M | DMOD_S),
    DATAMODE_ARA(E(P,NR_Y    ,DI_OFF   ,FRC_SCL), 32, DMOD_M),
    DATAMODE_ARA(E(P,NR_YC   ,DI_OFF   ,FRC_SCL), 64, DMOD_M),
    DATAMODE_ARA(E(P,NR_OFF  ,DI_OFF   ,FRC_MIB), 64, DMOD_M | DMOD_S),    
    DATAMODE_ARA(E(P,NR_Y    ,DI_OFF   ,FRC_MIB), 80, DMOD_M),
    DATAMODE_ARA(E(P,NR_YC   ,DI_OFF   ,FRC_MIB), 96, DMOD_M),
    DATAMODE_ARA(E_MIB_DMODE_END,0xFF,0),
 };

/**
 * @brief Get NR Mode from SRM
 * @param u1VdpId VDP_1/VDP_2
 * @return NR mode
 */
static inline UINT8 _u1DrvDIGetNRDataMode(UINT8 u1VdpId)
{        
    UINT8 bVdpIdOther = (u1VdpId == VDP_1) ? VDP_2 : VDP_1;
    UINT32 u4Mode;
    E_MIB_NR_DMODE eNRMode=E_MIB_NR_OFF;

    PATH_CHECK(u1VdpId);

    if(u4DrvTDTVIsDualFPI())
    {
        return E_MIB_NR_OFF;
    }
    
    u4Mode = rMIBFbmPool[u1VdpId].u4Mode; 

    if (_arMDDiPrm[u1VdpId].u1DebugNRDataMode != E_MIB_NR_UNKNOWN)        
    {
        if (_arMDDiPrm[bVdpIdOther].u1DebugNRDataMode != E_MIB_NR_UNKNOWN)
        {
            _arMDDiPrm[bVdpIdOther].u1DebugNRDataMode = E_MIB_NR_UNKNOWN;
        }
        return _arMDDiPrm[u1VdpId].u1DebugNRDataMode;
    }

    if (u4Mode & FBM_POOL_MODE_MDDI_NR_OFF)
    {
        return E_MIB_NR_OFF;
    }
    else 
    {
        if (bIsScalerInput444(u1VdpId))
        {
            return E_MIB_NR_OFF;
        }
        else
        {
            if (u4Mode & FBM_POOL_MODE_MDDI_NR_Y_C)
            {
                eNRMode=E_MIB_NR_YC;
            }
            else if (u4Mode & FBM_POOL_MODE_MDDI_NR_Y_ONLY)
            {
                eNRMode=E_MIB_NR_Y; 
            }
            else
            {
                eNRMode=E_MIB_NR_UNKNOWN;
            }

            //Interlace timing,PR Mode only support Y Only, No Chroma C NR,
            if((E_TD_OUT_3D_LI==_arMDDiPrm[u1VdpId].eTDOutput)&&(E_MIB_NR_YC==eNRMode)&&IS_INTERLACE(u1VdpId))                        
            {
                eNRMode=E_MIB_NR_Y;
            }                 

            return eNRMode;
        }        
    }
}


/**
 * @brief Get DI Display Mode from SRM
 * @param u1VdpId VDP_1/VDP_2
 * @return DI Display mode
 */
static inline UINT8 _u1DrvDIGetFrcMode(UINT8 u1VdpId)
{
    PATH_CHECK(u1VdpId);
    
    if (_arMDDiPrm[u1VdpId].u1DebugFrcMode != E_MIB_FRC_UNKNOWN)
    {       
        return _arMDDiPrm[u1VdpId].u1DebugFrcMode;
    }

    if (rMIBFbmPool[u1VdpId].u4Mode & FBM_POOL_MODE_MDDI_DISP)
    {
        return E_MIB_FRC_MIB;
    }
    else
    {
        return E_MIB_FRC_SCL;
    }
}

static inline UINT8 _u1DrvDIGetDIDataMode(UINT8 u1VdpId)
{
    E_MIB_DI_DMODE eDiMode=E_MIB_DI_OFF;
    PATH_CHECK(u1VdpId);
    
    if(u4DrvTDTVIsDualFPI())
    {
        u1VdpId=VDP_1;         
    }
    
    if(bDrvVideoIsSrcInterlace(u1VdpId))
    {
        switch (rMIBFbmPool[u1VdpId].u4Mode & FBM_POOL_MODE_MDDI_MODEMSK)
        {
            case FBM_POOL_MODE_MDDI_FULL:
                eDiMode= E_MIB_DI_Y4C4;
                break;
            case FBM_POOL_MODE_MDDI_BOB:
                eDiMode= E_MIB_DI_Y1C1;
                break;
            case FBM_POOL_MODE_MDDI_CBOB:
                if(SRM_IsGameMode(u1VdpId) && ((E_TD_OUT_3D_LI==_arMDDiPrm[u1VdpId].eTDOutput) ||
				   (E_TD_IN_LI_I==_arMDDiPrm[u1VdpId].eTDInput)))
                {//DI out LI under game mode, BOB mode dram size same as CBOB, but kernel should BOB mode. 
                    eDiMode= E_MIB_DI_Y1C1;
                }
                else
				{
                    eDiMode= E_MIB_DI_Y4C1;
				}
                break;
            default:
                eDiMode= E_MIB_DI_OFF;
                break;
        }
        
        //MIB Only Support Y4C2 for PR out
        if((E_TD_OUT_3D_LI==_arMDDiPrm[u1VdpId].eTDOutput)&&(E_MIB_DI_Y4C4==eDiMode))                        
        {
            eDiMode = E_MIB_DI_Y4C2;
        }
        
        return eDiMode;
    }
    else
    {
        return E_MIB_DI_OFF;
    }    
}

/**
 * @brief Get DI Mode from SRM
 * @param u1VdpId VDP_1/VDP_2
 * @return DI mode
 */
static inline E_MIB_DMODE _u1DrvDIGetMIBDataMode(UINT8 u1VdpId)
{

    //Please Note that Data Mode Query should always use latest Info instead of MIB Batched Status
    UINT8 u1FrcMode = (UINT8)_u1DrvDIGetFrcMode(u1VdpId);
    UINT8 u1NRMode = (UINT8)_u1DrvDIGetNRDataMode(u1VdpId);
    UINT8 u1DIMode = (UINT8)_u1DrvDIGetDIDataMode(u1VdpId);
    UINT8 u1IsIntr = bDrvVideoIsSrcInterlace(u1VdpId);
    UINT32 u4MIBDateMode;        
    UINT32 i;
    
    PATH_CHECK(u1VdpId);

    if(u4DrvTDTVIsDualFPI()&&(SV_VP_PIP==u1VdpId))
    {
        rMIBState[SV_VP_PIP].u1DataModeIdx = rMIBState[SV_VP_MAIN].u1DataModeIdx;
        return _arMDDiPrm[SV_VP_MAIN].eDataMode;;
    }
    
    if ((u1VdpId == VDP_2)&&(!u1IsIntr))
    {
        rMIBState[u1VdpId].u1DataModeIdx = 0;
        return E_MIB_DMODE_UNKNOWN;
    }
       
    if (_arMDDiPrm[u1VdpId].eDebugDataMode != E_MIB_DMODE_UNKNOWN)
    {
        u4MIBDateMode =  _arMDDiPrm[u1VdpId].eDebugDataMode; 
    }
    else
    {
        u4MIBDateMode = DMODE(u1IsIntr,u1NRMode,u1DIMode,u1FrcMode);
    }

    //Find MID Data Mode Configuration
    for(i=0;aDataMode[i].eMIBMode!= E_MIB_DMODE_END;++i)
    {
        if((aDataMode[i].eMIBMode == u4MIBDateMode) &&
            (((u1VdpId==VDP_1)&&(aDataMode[i].u1Flag&DMOD_M))||((u1VdpId==VDP_2)&&(aDataMode[i].u1Flag&DMOD_S)))
            )
        {           
            rMIBState[u1VdpId].u1DataModeIdx=i;
            return aDataMode[i].eMIBMode;
        }    
    }

    LOG(6,"[MIB]FBM CONFIGURATION UNREASONABLE\n");
    rMIBState[u1VdpId].u1DataModeIdx = 0;    
    return E_MIB_DMODE_UNKNOWN;
}

static inline UINT32 _u4DrvDIGetBandwidthUpperBound(UINT8 u1VdpId)
{
    return u1VdpId==VDP_1 ?
        IO32ReadFldAlign(PSCAN_MISC_00, BANDWIDTH_PER_PXL_1) : IO32ReadFldAlign(PSCAN_MISC_00, BANDWIDTH_PER_PXL_2);
}

/**
 * @brief Get DI Mode from SRM
 * @param u1VdpId VDP_1/VDP_2
 * @return DI mode
 */
static inline E_MIB_DMODE _u1DrvDIGetMIBBandWidthMode(UINT8 u1VdpId)
{
   // UINT32 u4MIBDateMode = _arMDDiPrm[u1VdpId].eDataMode;        
    UINT32 u4BandWidthUpperBound = _u4DrvDIGetBandwidthUpperBound(u1VdpId);
    UINT8 i;
    UINT8 u1SizeCheck = sizeof(aDataMode) / sizeof(rDataMode);
    
    PATH_CHECK(u1VdpId);

    if ( rMIBState[u1VdpId].u1DataModeIdx > u1SizeCheck-1 )
    {
        return E_MIB_DMODE_UNKNOWN;
    }
    //Find MID Data Mode Configuration
    for(i=rMIBState[u1VdpId].u1DataModeIdx; 
        (aDataMode[i].eMIBMode!= E_MIB_DMODE_UNKNOWN)&&((aDataMode[i].eMIBMode&0xF) == MIB_FRC_MODE(u1VdpId));
        --i)
    {
        
        if((aDataMode[i].u1Bandwith <= u4BandWidthUpperBound)&&
            (((u1VdpId==VDP_1)&&(aDataMode[i].u1Flag&DMOD_M))||((u1VdpId==VDP_2)&&(aDataMode[i].u1Flag&DMOD_S)))
            )            
        {  
            rMIBState[u1VdpId].u1BandwidthDataModeIdx=i;
            return aDataMode[i].eMIBMode;
        }    
    }

    if(aDataMode[i].eMIBMode!= E_MIB_DMODE_UNKNOWN)
    {
        LOG(6,"UNABLE TO REACH MIB Bandwidth TARGET Restrict BY FBM !!\n");
    }
    
    rMIBState[u1VdpId].u1BandwidthDataModeIdx = rMIBState[u1VdpId].u1DataModeIdx;
    return _arMDDiPrm[u1VdpId].eDataMode;
}
/**
 * @brief Set DI Dram Control
 * @param u1VdpId VDP_1/VDP_2
 * @param u1DramCOnOff Enable/Disable
 */
static inline void _vDrvDISetKernalCtrl(UINT8 u1VdpId, E_MIB_DMODE eDataMode)
{
    UINT32 u4BYPASS_EN;  
    UINT32 u4AF_FROM_SRC;
    UINT32 u4I2P_EN; 
    UINT32 u4KERNEL_SYNC;
    UINT32 u4OUTBUF_SYNC;   
    UINT32 u4DA_SYNC_MODE;   
    UINT32 u4OUT_TG_MODE;
    UINT32 u4DA_SC_READ_ENABLE;  
    UINT32 u4DA_PROG_MODE;   
    UINT32 u4PROG_MODE;    
    
    PATH_CHECK(u1VdpId);
    
    switch (eDataMode)
    {                          
        //Scaler FRC
		case E(I, NR_Y   ,DI_OFF   ,FRC_SCL):
		case E(I, NR_YC  ,DI_OFF   ,FRC_SCL):
			u4AF_FROM_SRC   =HW_MIB_AF_FROM_SRC;
            u4I2P_EN        =SV_OFF;                               
            u4KERNEL_SYNC   =E_MIB_SYNC_INPUT;
            u4OUTBUF_SYNC   =E_MIB_SYNC_INPUT;   
            u4DA_SYNC_MODE  =E_MIB_SYNC_INPUT;   
            u4OUT_TG_MODE   =E_TG_FREE_RUN;
            u4DA_SC_READ_ENABLE=SV_OFF;
            u4DA_PROG_MODE  =SV_OFF;
			break;
        case E(I, NR_OFF ,DI_Y1C1  ,FRC_SCL):
        case E(I, NR_OFF ,DI_Y2C1  ,FRC_SCL): 
        case E(I, NR_OFF ,DI_Y2C2  ,FRC_SCL): 	
        case E(I, NR_OFF ,DI_Y4C1  ,FRC_SCL)://This Mode Only Used under Sub
        case E(I, NR_YC  ,DI_Y4C1  ,FRC_SCL):
        case E(I, NR_YC  ,DI_Y4C2  ,FRC_SCL):
        case E(I, NR_OFF ,DI_Y4C4  ,FRC_SCL)://This Mode Only Used under sub         
        case E(I, NR_YC  ,DI_Y4C4  ,FRC_SCL):
            u4AF_FROM_SRC   =HW_MIB_AF_FROM_SRC;
            u4I2P_EN        =SV_ON;                               
            u4KERNEL_SYNC   =E_MIB_SYNC_INPUT;
            u4OUTBUF_SYNC   =E_MIB_SYNC_INPUT;   
            u4DA_SYNC_MODE  =E_MIB_SYNC_INPUT;   
            u4OUT_TG_MODE   =E_TG_FREE_RUN;
            u4DA_SC_READ_ENABLE=SV_OFF;
            u4DA_PROG_MODE  =SV_OFF;
            break;             
        //MIB FRC
        case E(I, NR_Y   ,DI_OFF   ,FRC_MIB):           
        case E(I, NR_YC  ,DI_OFF   ,FRC_MIB):
            u4AF_FROM_SRC   =HW_MIB_AF_FROM_SRC;
            u4I2P_EN        =SV_OFF;                               
            u4KERNEL_SYNC   =E_MIB_SYNC_INPUT;
            u4OUTBUF_SYNC   =E_MIB_SYNC_OUTPUT;   
            u4DA_SYNC_MODE  =E_MIB_SYNC_OUTPUT;   
            u4OUT_TG_MODE   =E_TG_MIB_DISP;
            u4DA_SC_READ_ENABLE=SV_ON;
            u4DA_PROG_MODE  =SV_ON;
            break;      
        case E(I, NR_OFF ,DI_Y1C1  ,FRC_MIB):
        case E(I, NR_OFF ,DI_Y2C1  ,FRC_MIB):
        case E(I, NR_OFF ,DI_Y2C2  ,FRC_MIB):	
        case E(I, NR_OFF ,DI_Y3C1  ,FRC_MIB):
        case E(I, NR_OFF ,DI_Y3C2  ,FRC_MIB):
        case E(I, NR_OFF ,DI_Y4C1  ,FRC_MIB):
        case E(I, NR_YC  ,DI_Y4C1  ,FRC_MIB):    
        case E(I, NR_Y   ,DI_Y4C1  ,FRC_MIB):
        case E(I, NR_OFF ,DI_Y4C2  ,FRC_MIB):
        case E(I, NR_Y   ,DI_Y4C2  ,FRC_MIB):
		case E(I, NR_YC  ,DI_Y4C2  ,FRC_MIB):
        case E(I, NR_OFF ,DI_Y4C4  ,FRC_MIB):             
        case E(I, NR_Y   ,DI_Y4C4  ,FRC_MIB):            
        case E(I, NR_YC  ,DI_Y4C4  ,FRC_MIB):
            u4AF_FROM_SRC   =HW_MIB_AF_FROM_DRAM;
            u4I2P_EN        =SV_ON;                               
            u4KERNEL_SYNC   =E_MIB_SYNC_OUTPUT;
            u4OUTBUF_SYNC   =E_MIB_SYNC_OUTPUT;   
            u4DA_SYNC_MODE  =E_MIB_SYNC_OUTPUT;   
            u4OUT_TG_MODE   =E_TG_MIB_DISP;
            u4DA_SC_READ_ENABLE=SV_OFF;
            u4DA_PROG_MODE  =SV_OFF;
            break;                            
        //Progressive Scaler FRC
        case E(P,NR_OFF   ,DI_OFF ,FRC_SCL):
        case E(P,NR_Y     ,DI_OFF ,FRC_SCL):          
        case E(P,NR_YC    ,DI_OFF ,FRC_SCL):            
            u4AF_FROM_SRC   =HW_MIB_AF_FROM_SRC;
            u4I2P_EN        =SV_OFF;                               
            u4KERNEL_SYNC   =E_MIB_SYNC_INPUT;
            u4OUTBUF_SYNC   =E_MIB_SYNC_INPUT;   
            u4DA_SYNC_MODE  =E_MIB_SYNC_INPUT;   
            u4OUT_TG_MODE   =E_TG_FREE_RUN;
            u4DA_SC_READ_ENABLE=SV_OFF;
            u4DA_PROG_MODE  =SV_OFF;
            break;              
        //Progressive MIB FRC
        case E(P,NR_OFF   ,DI_OFF ,FRC_MIB):
        case E(P,NR_Y     ,DI_OFF ,FRC_MIB):    
        case E(P,NR_YC    ,DI_OFF ,FRC_MIB):            
            u4AF_FROM_SRC   =HW_MIB_AF_FROM_SRC;
            u4I2P_EN        =SV_OFF;                               
            u4KERNEL_SYNC   =E_MIB_SYNC_INPUT ;
            u4OUTBUF_SYNC   =E_MIB_SYNC_OUTPUT;   
            u4DA_SYNC_MODE  =E_MIB_SYNC_OUTPUT;   
            u4OUT_TG_MODE   =E_TG_MIB_DISP;
            u4DA_SC_READ_ENABLE=SV_ON;
            u4DA_PROG_MODE  =SV_ON;
            break;            
        case E_MIB_DMODE_UNKNOWN:                
        default:
            //Turn Off PSCAN DRAM Engine
            u4BYPASS_EN     =SV_ON;
            u4AF_FROM_SRC   =HW_MIB_AF_FROM_SRC;
            u4I2P_EN        =SV_OFF;                               
            u4KERNEL_SYNC   =E_MIB_SYNC_INPUT ;
            u4OUTBUF_SYNC   =E_MIB_SYNC_INPUT;   
            u4DA_SYNC_MODE  =E_MIB_SYNC_INPUT;   
            u4OUT_TG_MODE   =E_TG_MIB_DISP;
            u4DA_SC_READ_ENABLE=SV_OFF;
            u4DA_PROG_MODE  =SV_OFF;
    }

    //DRAM Engine 444/422 Mode
    if (bIsScalerInput444(u1VdpId)&&(eDataMode != E_MIB_DMODE_UNKNOWN)&&(IS_MIB_FRC(u1VdpId)))
    {
        u4BYPASS_EN     =SV_OFF;
        u4AF_FROM_SRC   =HW_MIB_AF_FROM_SRC;
        u4I2P_EN        =SV_OFF;                               
        u4KERNEL_SYNC   =E_MIB_SYNC_INPUT ;
        u4OUTBUF_SYNC   =E_MIB_SYNC_OUTPUT;   
        u4DA_SYNC_MODE  =E_MIB_SYNC_OUTPUT;   
        u4OUT_TG_MODE   =E_TG_MIB_DISP;
        u4DA_SC_READ_ENABLE=SV_ON;
        u4DA_PROG_MODE  =SV_ON;    
    }


    u4PROG_MODE = IS_INTERLACE(u1VdpId)? SV_OFF : SV_ON ;
    if ( eDataMode == E(I, NR_Y   ,DI_OFF   ,FRC_MIB) 
        || eDataMode == E(I, NR_YC  ,DI_OFF   ,FRC_MIB))
    {
        u4PROG_MODE = SV_ON;
    }
  
    u4BYPASS_EN = (
        !((_arMDDiPrm[u1VdpId].u1OnOff)
            &&((IS_420TO422(u1VdpId)&&(VDP_1==u1VdpId))
                || IS_NR_ON(u1VdpId) 
                || IS_MIB_FRC(u1VdpId) 
                || IS_INTERLACE(u1VdpId))))
        || (eDataMode ==E_MIB_DMODE_UNKNOWN );


    MDDI_WRITE_FLD(u1VdpId,MCVP_KC_17   ,u4AF_FROM_SRC      ,AF_FROM_SRC);
    MDDI_WRITE_FLD(u1VdpId,MCVP_KC_00   ,u4I2P_EN           ,I2P_EN);
 
    MDDI_WRITE_FLD(u1VdpId,MCVP_MULTI_00,u4OUTBUF_SYNC      ,OUTBUF_SYNC);                    
    MDDI_WRITE_FLD(u1VdpId,MCVP_MULTI_00,u4KERNEL_SYNC      ,KERNEL_SYNC);
    MDDI_WRITE_FLD(u1VdpId,MCVP_DRAM_15 ,u4DA_SYNC_MODE     ,DA_SYNC_MODE);
    MDDI_WRITE_FLD(u1VdpId,MCVP_KC_1F   ,u4OUT_TG_MODE      ,OUT_TG_MODE);    
    MDDI_WRITE_FLD(u1VdpId,MCVP_DRAM_18 ,u4DA_SC_READ_ENABLE,DA_SC_READ_ENABLE);
    MDDI_WRITE_FLD(u1VdpId,MCVP_DRAM_15 ,u4DA_PROG_MODE     ,DA_PROG_MODE);
    MDDI_WRITE_FLD(u1VdpId,MCVP_MULTI_00,u4PROG_MODE        ,PROG_MODE);

    MDDI_WRITE_FLD(u1VdpId,MCVP_KC_00   ,u4BYPASS_EN        ,BYPASS_EN);    
    MDDI_WRITE_FLD(u1VdpId,MCVP_KC_0A   ,u4BYPASS_EN        ,MUTE_EN);


    // Dual DI Ctrl Setting
    if (_arMDDiPrm[u1VdpId].u1IsDualFPI)
    {
        MDDI_WRITE_FLD(u1VdpId, MCVP_KC_33 , 2,DUALDI_OUTBUF_OPTION); 
        if (VDP_1==u1VdpId)
        {       
            MDDI_WRITE_FLD(VDP_1, MCVP_KC_33 , 1,FRAME_START_SLAVE_MODE);
            MDDI_WRITE_FLD(VDP_1, MCVP_KC_33 , 0,LINE_START_SLAVE_MODE);
        }
        else
        {
            MDDI_WRITE_FLD(VDP_2, MCVP_KC_33 , 0,FRAME_START_SLAVE_MODE);
            MDDI_WRITE_FLD(VDP_2, MCVP_KC_33 , 1,LINE_START_SLAVE_MODE);         
            MDDI_WRITE_FLD(VDP_2, MCVP_KC_33 , 1,OSYNC_CTRL_BY_SCL);
            MDDI_WRITE_FLD(VDP_2, MCVP_KC_1F , 1,MAIN_PIP_OUT_SYNC);
        }
        // force 422 if it's dual DI
        vDrvSet422(u1VdpId, TRUE);
    }
    else
    {       
        MDDI_WRITE_FLD(u1VdpId, MCVP_KC_33 , 0,DUALDI_OUTBUF_OPTION);
        MDDI_WRITE_FLD(u1VdpId, MCVP_KC_33 , 0,FRAME_START_SLAVE_MODE);
        MDDI_WRITE_FLD(u1VdpId, MCVP_KC_33 , 0,LINE_START_SLAVE_MODE);
        if (u1VdpId == VDP_2)
        {
            MDDI_WRITE_FLD(VDP_2, MCVP_KC_33 , 0,OSYNC_CTRL_BY_SCL);
            MDDI_WRITE_FLD(VDP_2, MCVP_KC_1F , 0,MAIN_PIP_OUT_SYNC);
        }
    }

    if(VDP_1==u1VdpId)
    {
        MDDI_WRITE_FLD(VDP_1,MCVP_GC_00 , (u4BYPASS_EN || (!u1DrvDIIsSupportedSignal(u1VdpId))), M_SYNC_FULL_SPEED);
        vIO32WriteFldAlign(TOP_MAIN_00, (u4BYPASS_EN || (!u1DrvDIIsSupportedSignal(u1VdpId))), BYPASS_DIV2_REGEN_M);
    }
    else
    {
        MDDI_WRITE_FLD(VDP_1,MCVP_GC_00 , (u4BYPASS_EN || (!u1DrvDIIsSupportedSignal(u1VdpId))), P_SYNC_FULL_SPEED);
        vIO32WriteFldAlign(TOP_PIP_00 , (u4BYPASS_EN || (!u1DrvDIIsSupportedSignal(u1VdpId))), BYPASS_DIV2_REGEN_P);
    }   
#if defined(CC_FPGA_SUPPORT)
    MDDI_WRITE_FLD(VDP_1,MCVP_GC_00 , 0, M_SYNC_FULL_SPEED);
    MDDI_WRITE_FLD(VDP_1,MCVP_GC_00 , 0, P_SYNC_FULL_SPEED);
#endif
}

/**
 * @brief Config CS flag 
 * @param u1VdpId VDP_1/VDP_2
 */
static inline void _vDrvDICSFlagCfg(UINT8 u1VdpId)
{
    if (u1VdpId == SV_VP_PIP)
    {
        return;
    }
    
    if(IS_SD_TIMING(SV_VP_MAIN) && MDDI_READ_FLD(VDP_1, MCVP_KC_19, SRAM_MODE) <= E_SRAM_SHARE_SD_P) //SD Timing
    {
        MDDI_WRITE_FLD(SV_VP_MAIN,MCVP_CS_00, 0, ME_MODE); 
        MDDI_WRITE_FLD(SV_VP_MAIN,MCVP_DRAM_01, 0, DA_FLAG_MV_HALF_MODE); 
    }
    else if (rMIBState[SV_VP_MAIN].u4SetResNewKDE == 0)//1080P
    {
        MDDI_WRITE_FLD(SV_VP_MAIN, MCVP_CS_00, 3, ME_MODE);
        MDDI_WRITE_FLD(SV_VP_MAIN, MCVP_DRAM_01, 1, DA_FLAG_MV_HALF_MODE); 
    }
    else //HD timing 
    {
        MDDI_WRITE_FLD(SV_VP_MAIN,MCVP_CS_00, 1, ME_MODE);
        MDDI_WRITE_FLD(SV_VP_MAIN,MCVP_DRAM_01, 0, DA_FLAG_MV_HALF_MODE); 
    }
        
}


/**
 * @brief Set DI Dram Control
 * @param u1VdpId VDP_1/VDP_2
 * @param u1DramCOnOff Enable/Disable
 */
static inline void _vDrvDISetDramCtrl(UINT8 u1VdpId, E_MIB_DMODE eDataMode)
{
    UINT32 u4DRAM_RW_EN;
    UINT32 u4AF_TO_CF;   
    UINT32 u4BF_TO_CF;   
    UINT32 u4ABCD_YC_EN; 
    UINT32 u4DA_MODE_YUV;
    UINT32 u4DA_NR_YUV_MODE;
    UINT32 u4DA_FLAG_RNR_MODE;

    static UINT32 u4PrevDMODE[VDP_NS]={0xFFFF,0xFFFF};
    static UINT32 u4PreAF_TO_CF[VDP_NS]={0xFF,0xFF};
    static UINT8  u1PreTvdMode = 0;
    PATH_CHECK(u1VdpId);
    
    switch (eDataMode)
    {                          
        //Scaler FRC
        case E(I, NR_Y   ,DI_OFF   ,FRC_SCL):
           	u4AF_TO_CF      =SV_OFF;
            u4BF_TO_CF      =SV_OFF;   
            u4ABCD_YC_EN    =0x10;
            u4DRAM_RW_EN    =E_NR_W | E_SRC_R | E_MV_W | E_MV_R | E_RNR_W | E_RNR_R;
            u4DA_MODE_YUV   =MIB_YUV_MODE_YC;
            u4DA_NR_YUV_MODE=MIB_YUV_MODE_Y_ONLY; 
            u4DA_FLAG_RNR_MODE = E_MIB_RNR_INTR;
        	break;
        case E(I, NR_YC  ,DI_OFF   ,FRC_SCL):
           	u4AF_TO_CF      =SV_OFF;
            u4BF_TO_CF      =SV_OFF;   
            u4ABCD_YC_EN    =0x30;
            u4DRAM_RW_EN    =E_NR_W | E_SRC_R | E_MV_W | E_MV_R | E_RNR_W | E_RNR_R;
            u4DA_MODE_YUV   =MIB_YUV_MODE_YC;
            u4DA_NR_YUV_MODE=MIB_YUV_MODE_YC; 
           u4DA_FLAG_RNR_MODE = E_MIB_RNR_INTR;
        	break;
        case E(I, NR_OFF ,DI_Y1C1  ,FRC_SCL):
            u4AF_TO_CF      =SV_ON;
            u4BF_TO_CF      =SV_OFF;   
            u4ABCD_YC_EN    =0x00;
            u4DRAM_RW_EN    =E_ALL_RW_OFF;   
            u4DA_MODE_YUV   =MIB_YUV_MODE_YC;
            u4DA_NR_YUV_MODE=MIB_YUV_MODE_YC;//don't care when NR_W is off
            u4DA_FLAG_RNR_MODE = E_MIB_RNR_DISABLE;
            break; 
        case E(I, NR_OFF ,DI_Y2C1  ,FRC_SCL):
        	u4AF_TO_CF      =SV_ON;
            u4BF_TO_CF      =SV_OFF;   
            u4ABCD_YC_EN    =0x04;
            u4DRAM_RW_EN    =E_NR_W | E_SRC_R;   
            u4DA_MODE_YUV   =MIB_YUV_MODE_YC;
            u4DA_NR_YUV_MODE=MIB_YUV_MODE_Y_ONLY; 
            u4DA_FLAG_RNR_MODE = E_MIB_RNR_DISABLE;
        	break;
        case E(I, NR_OFF ,DI_Y2C2  ,FRC_SCL):
        	u4AF_TO_CF      =SV_ON;
            u4BF_TO_CF      =SV_OFF;   
            u4ABCD_YC_EN    =0x0C;
            u4DRAM_RW_EN    =E_NR_W | E_SRC_R;   
            u4DA_MODE_YUV   =MIB_YUV_MODE_YC;
            u4DA_NR_YUV_MODE=MIB_YUV_MODE_YC; 
            u4DA_FLAG_RNR_MODE = E_MIB_RNR_DISABLE;
        	break;
        case E(I, NR_OFF ,DI_Y4C1  ,FRC_SCL)://This Mode Only Used under Sub
            u4AF_TO_CF      =SV_OFF;
            u4BF_TO_CF      =SV_OFF;   
            u4ABCD_YC_EN    =0x74;
            u4DRAM_RW_EN    =E_SRC_W | E_SRC_R | E_MV_W | E_MV_R | E_LM_W | E_LM_R;
            u4DA_MODE_YUV   =MIB_YUV_MODE_YC;
            u4DA_NR_YUV_MODE=MIB_YUV_MODE_YC; 
            u4DA_FLAG_RNR_MODE = E_MIB_RNR_DISABLE;
            break;        
        case E(I, NR_YC  ,DI_Y4C1  ,FRC_SCL):
            u4AF_TO_CF      =SV_OFF;
            u4BF_TO_CF      =SV_OFF;   
            u4ABCD_YC_EN    =0x74;
            u4DRAM_RW_EN    =E_NR_W | E_SRC_R | E_MV_W | E_MV_R | E_LM_W | E_LM_R | E_RNR_W | E_RNR_R;
            u4DA_MODE_YUV   =MIB_YUV_MODE_YC;
            u4DA_NR_YUV_MODE=MIB_YUV_MODE_YC; 
            u4DA_FLAG_RNR_MODE = E_MIB_RNR_INTR;
            break;
        case E(I, NR_YC  ,DI_Y4C2  ,FRC_SCL):
            u4AF_TO_CF      =SV_OFF;
            u4BF_TO_CF      =SV_OFF;   
            u4ABCD_YC_EN    =0x7C;
            u4DRAM_RW_EN    =E_NR_W | E_SRC_R | E_MV_W | E_MV_R | E_LM_W | E_LM_R | E_RNR_W | E_RNR_R;
            u4DA_MODE_YUV   =MIB_YUV_MODE_YC;
            u4DA_NR_YUV_MODE=MIB_YUV_MODE_YC; 
            u4DA_FLAG_RNR_MODE = E_MIB_RNR_INTR;
            break;
        case E(I, NR_OFF ,DI_Y4C4  ,FRC_SCL)://This Mode Only Used under sub
            u4AF_TO_CF      =SV_OFF;
            u4BF_TO_CF      =SV_OFF;   
            u4ABCD_YC_EN    =0xFC;
            u4DRAM_RW_EN    =E_SRC_R | E_MV_W | E_MV_R | E_LM_W | E_LM_R;
            u4DRAM_RW_EN    |= (SV_VP_MAIN==u1VdpId) ? E_NR_W: E_SRC_W;
            u4DA_MODE_YUV   =MIB_YUV_MODE_YC;
            u4DA_NR_YUV_MODE=MIB_YUV_MODE_YC; 
            u4DA_FLAG_RNR_MODE = E_MIB_RNR_DISABLE;
            break;          
        case E(I, NR_YC  ,DI_Y4C4  ,FRC_SCL):
            u4AF_TO_CF      =SV_OFF;
            u4BF_TO_CF      =SV_OFF;   
            u4ABCD_YC_EN    =0xFC;
            u4DRAM_RW_EN    =E_NR_W | E_SRC_R | E_MV_W | E_MV_R | E_LM_W | E_LM_R | E_RNR_W | E_RNR_R;
            u4DA_MODE_YUV   =MIB_YUV_MODE_YC;
            u4DA_NR_YUV_MODE=MIB_YUV_MODE_YC; 
            u4DA_FLAG_RNR_MODE = E_MIB_RNR_INTR;
            break;             
        //MIB FRC
        case E(I, NR_OFF ,DI_Y1C1  ,FRC_MIB):
            u4AF_TO_CF      =SV_ON;
            u4BF_TO_CF      =SV_OFF;   
            u4ABCD_YC_EN    =0x03;
            u4DRAM_RW_EN    =E_SRC_W | E_SRC_R;
            u4DA_MODE_YUV   =MIB_YUV_MODE_YC;
            u4DA_NR_YUV_MODE=MIB_YUV_MODE_YC;            
            if(rMIBState[u1VdpId].u1DataModeIdx!=rMIBState[u1VdpId].u1BandwidthDataModeIdx ||
               IS_PR_MODE(u1VdpId))
            { //BandWidth Savinf Mode, DRAM Size Enough       
                u4AF_TO_CF      =SV_OFF;                
                u4ABCD_YC_EN    =0x30;
            } 
            u4DA_FLAG_RNR_MODE = E_MIB_RNR_DISABLE;
            break;             
        case E(I, NR_Y   ,DI_OFF   ,FRC_MIB):
            u4AF_TO_CF      =SV_OFF;
            u4BF_TO_CF      =SV_ON;   
            u4ABCD_YC_EN    =0x4;
            u4DRAM_RW_EN    =E_NR_W | E_SRC_R | E_RNR_W | E_RNR_R;
            u4DA_MODE_YUV   =MIB_YUV_MODE_YC;
            u4DA_NR_YUV_MODE=MIB_YUV_MODE_YC; 
            u4DA_FLAG_RNR_MODE = E_MIB_RNR_PROG_OUTPT_SYNC;
            break;            
        case E(I, NR_YC  ,DI_OFF   ,FRC_MIB):
            u4AF_TO_CF      =SV_OFF;
            u4BF_TO_CF      =SV_ON;   
            u4ABCD_YC_EN    =0x0C;
            u4DRAM_RW_EN    =E_NR_W | E_SRC_R | E_RNR_W | E_RNR_R;
            u4DA_MODE_YUV   =MIB_YUV_MODE_YC;
            u4DA_NR_YUV_MODE=MIB_YUV_MODE_YC; 
            u4DA_FLAG_RNR_MODE = E_MIB_RNR_PROG_OUTPT_SYNC;
            break;  
        case E(I, NR_OFF ,DI_Y2C1  ,FRC_MIB):
        	u4AF_TO_CF      =SV_ON;
            u4BF_TO_CF      =SV_OFF;   
            u4ABCD_YC_EN    =0x07;
            u4DRAM_RW_EN    =E_SRC_W | E_SRC_R;   
            u4DA_MODE_YUV   =MIB_YUV_MODE_YC;
            u4DA_NR_YUV_MODE=MIB_YUV_MODE_YC; 
            u4DA_FLAG_RNR_MODE = E_MIB_RNR_DISABLE;
        	break;
        case E(I, NR_OFF ,DI_Y2C2  ,FRC_MIB):
        	u4AF_TO_CF      =SV_ON;
            u4BF_TO_CF      =SV_OFF;   
            u4ABCD_YC_EN    =0x0F;
            u4DRAM_RW_EN    =E_SRC_W | E_SRC_R;   
            u4DA_MODE_YUV   =MIB_YUV_MODE_YC;
            u4DA_NR_YUV_MODE=MIB_YUV_MODE_YC; 
            u4DA_FLAG_RNR_MODE = E_MIB_RNR_DISABLE;
        	break;    
        case E(I, NR_OFF ,DI_Y3C1  ,FRC_MIB):
        	u4AF_TO_CF      =SV_OFF;
            u4BF_TO_CF      =SV_OFF;   
            u4ABCD_YC_EN    =0x74;
            u4DRAM_RW_EN    =E_SRC_W | E_SRC_R | E_LM_W | E_LM_R;
            u4DA_MODE_YUV   =MIB_YUV_MODE_YC;
            u4DA_NR_YUV_MODE=MIB_YUV_MODE_YC; 
            u4DA_FLAG_RNR_MODE = E_MIB_RNR_DISABLE;
        	break;
        case E(I, NR_OFF ,DI_Y3C2  ,FRC_MIB):
        	u4AF_TO_CF      =SV_OFF;
            u4BF_TO_CF      =SV_OFF;   
            u4ABCD_YC_EN    =0x7C;
            u4DRAM_RW_EN    =E_SRC_W | E_SRC_R | E_LM_W | E_LM_R;
            u4DA_MODE_YUV   =MIB_YUV_MODE_YC;
            u4DA_NR_YUV_MODE=MIB_YUV_MODE_YC; 
            u4DA_FLAG_RNR_MODE = E_MIB_RNR_DISABLE;
        	break;
        case E(I, NR_OFF ,DI_Y4C1  ,FRC_MIB):
            u4AF_TO_CF      =SV_OFF;
            u4BF_TO_CF      =SV_OFF;   
            u4ABCD_YC_EN    =0x75;
            u4DRAM_RW_EN    =E_SRC_W | E_SRC_R | E_MV_W | E_MV_R | E_LM_W | E_LM_R | E_RNR_W | E_RNR_R;
            u4DA_MODE_YUV   =MIB_YUV_MODE_YC;
            u4DA_NR_YUV_MODE=MIB_YUV_MODE_YC; 
            u4DA_FLAG_RNR_MODE = E_MIB_RNR_INTR;
            break;
        case E(I, NR_OFF ,DI_Y4C2  ,FRC_MIB):
        	u4AF_TO_CF      =SV_OFF;
            u4BF_TO_CF      =SV_OFF;   
            u4ABCD_YC_EN    =0x7D;
            u4DRAM_RW_EN    =E_SRC_W | E_SRC_R | E_MV_W | E_MV_R | E_LM_W | E_LM_R | E_RNR_W | E_RNR_R;
            u4DA_MODE_YUV   =MIB_YUV_MODE_YC;
            u4DA_NR_YUV_MODE=MIB_YUV_MODE_YC; 
            u4DA_FLAG_RNR_MODE = E_MIB_RNR_INTR;
        	break;
        case E(I, NR_OFF ,DI_Y4C4  ,FRC_MIB):
            u4AF_TO_CF      =SV_OFF;
            u4BF_TO_CF      =SV_OFF;   
            u4ABCD_YC_EN    =0xFF;
            u4DRAM_RW_EN    =E_SRC_W | E_SRC_R | E_MV_W | E_MV_R | E_LM_W | E_LM_R | E_RNR_W | E_RNR_R;
            u4DA_MODE_YUV   =MIB_YUV_MODE_YC;
            u4DA_NR_YUV_MODE=MIB_YUV_MODE_YC; 
            u4DA_FLAG_RNR_MODE = E_MIB_RNR_INTR;
            break;              
        case E(I, NR_Y ,DI_Y4C1  ,FRC_MIB):
            u4AF_TO_CF      =SV_OFF;
            u4BF_TO_CF      =SV_OFF;   
            u4ABCD_YC_EN    =0x75;
            u4DRAM_RW_EN    =E_NR_W | E_SRC_W | E_SRC_R | E_MV_W | E_MV_R | E_LM_W | E_LM_R | E_RNR_W | E_RNR_R;
            u4DA_MODE_YUV   =MIB_YUV_MODE_YC;
            u4DA_NR_YUV_MODE=MIB_YUV_MODE_Y_ONLY;
            u4DA_FLAG_RNR_MODE = E_MIB_RNR_INTR;
            break;
        case E(I, NR_YC,DI_Y4C1  ,FRC_MIB):
            u4AF_TO_CF      =SV_OFF;
            u4BF_TO_CF      =SV_OFF;   
            u4ABCD_YC_EN    =0x77;
            u4DRAM_RW_EN    =E_NR_W | E_SRC_W | E_SRC_R | E_MV_W | E_MV_R | E_LM_W | E_LM_R | E_RNR_W | E_RNR_R;
            u4DA_MODE_YUV   =MIB_YUV_MODE_YC;
            u4DA_NR_YUV_MODE=MIB_YUV_MODE_YC;
            u4DA_FLAG_RNR_MODE = E_MIB_RNR_INTR;
            break;     
        case E(I, NR_Y ,DI_Y4C2  ,FRC_MIB):
        	u4AF_TO_CF      =SV_OFF;
            u4BF_TO_CF      =SV_OFF;   
            u4ABCD_YC_EN    =0x7D;
            u4DRAM_RW_EN    =E_NR_W | E_SRC_W | E_SRC_R | E_MV_W | E_MV_R | E_LM_W | E_LM_R | E_RNR_W | E_RNR_R;
            u4DA_MODE_YUV   =MIB_YUV_MODE_YC;
            u4DA_NR_YUV_MODE=MIB_YUV_MODE_Y_ONLY;
            u4DA_FLAG_RNR_MODE = E_MIB_RNR_INTR;
        	break;
        case E(I, NR_Y ,DI_Y4C4  ,FRC_MIB):
            u4AF_TO_CF      =SV_OFF;
            u4BF_TO_CF      =SV_OFF;   
            u4ABCD_YC_EN    =0xFF;
            u4DRAM_RW_EN    =E_NR_W | E_SRC_W | E_SRC_R | E_MV_W | E_MV_R | E_LM_W | E_LM_R | E_RNR_W | E_RNR_R;
            u4DA_MODE_YUV   =MIB_YUV_MODE_YC;
            u4DA_NR_YUV_MODE=MIB_YUV_MODE_Y_ONLY;
            u4DA_FLAG_RNR_MODE = E_MIB_RNR_INTR;
            break;     
  
         case E(I, NR_YC ,DI_Y4C2  ,FRC_MIB):
        	u4AF_TO_CF      =SV_OFF;
            u4BF_TO_CF      =SV_OFF;   
            u4ABCD_YC_EN    =0x7F;
            u4DRAM_RW_EN    =E_NR_W | E_SRC_W | E_SRC_R | E_MV_W | E_MV_R | E_LM_W | E_LM_R | E_RNR_W | E_RNR_R;
            u4DA_MODE_YUV   =MIB_YUV_MODE_YC;
            u4DA_NR_YUV_MODE=MIB_YUV_MODE_YC; 
            u4DA_FLAG_RNR_MODE = E_MIB_RNR_INTR;
        	break;    
        case E(I,NR_YC    ,DI_Y4C4,FRC_MIB):
            u4AF_TO_CF      =SV_OFF;
            u4BF_TO_CF      =SV_OFF;   
            u4ABCD_YC_EN    =0xFF;
            u4DRAM_RW_EN    =E_NR_W | E_SRC_W | E_SRC_R | E_MV_W | E_MV_R | E_LM_W | E_LM_R | E_RNR_W | E_RNR_R;
            u4DA_MODE_YUV   =MIB_YUV_MODE_YC;
            u4DA_NR_YUV_MODE=MIB_YUV_MODE_YC; 
            u4DA_FLAG_RNR_MODE = E_MIB_RNR_INTR;
            break;                            
        //Progressive Scaler FRC
        case E(P,NR_OFF   ,DI_OFF ,FRC_SCL):
            u4AF_TO_CF      =SV_OFF;
            u4BF_TO_CF      =SV_OFF;   
            u4ABCD_YC_EN    =0x0;
            u4DRAM_RW_EN    =E_ALL_RW_OFF;
            u4DA_MODE_YUV   =MIB_YUV_MODE_YC;
            u4DA_NR_YUV_MODE=MIB_YUV_MODE_YC; 
            u4DA_FLAG_RNR_MODE = E_MIB_RNR_DISABLE;
            break;              
        case E(P,NR_Y     ,DI_OFF ,FRC_SCL):
            u4AF_TO_CF      =SV_OFF;
            u4BF_TO_CF      =SV_ON;   
            u4ABCD_YC_EN    =0x4;
            u4DRAM_RW_EN    =E_NR_W | E_SRC_R | E_MV_W | E_MV_R | E_RNR_W | E_RNR_R;
            u4DA_MODE_YUV   =MIB_YUV_MODE_YC;
            u4DA_NR_YUV_MODE=MIB_YUV_MODE_Y_ONLY; 
            u4DA_FLAG_RNR_MODE = E_MIB_RNR_PROG_INPT_SYNC;
            break;            
        case E(P,NR_YC    ,DI_OFF ,FRC_SCL):
            u4AF_TO_CF      =SV_OFF;
            u4BF_TO_CF      =SV_ON;
            u4ABCD_YC_EN    =0xC;                 
            u4DRAM_RW_EN    =E_NR_W | E_SRC_R | E_MV_W | E_MV_R | E_RNR_W | E_RNR_R;
            u4DA_MODE_YUV   =MIB_YUV_MODE_YC;
            u4DA_NR_YUV_MODE=MIB_YUV_MODE_YC; 
            u4DA_FLAG_RNR_MODE = E_MIB_RNR_PROG_INPT_SYNC;
            break;             
        //Progressive MIB FRC
        case E(P,NR_OFF   ,DI_OFF ,FRC_MIB):
            u4AF_TO_CF      =SV_ON;
            u4BF_TO_CF      =SV_OFF;   
            u4ABCD_YC_EN    =0x0;
            u4DRAM_RW_EN    =E_NR_W;
            u4DA_MODE_YUV   =MIB_YUV_MODE_YC;
            u4DA_NR_YUV_MODE=MIB_YUV_MODE_YC; 
            u4DA_FLAG_RNR_MODE = E_MIB_RNR_DISABLE;
            break;            
        case E(P,NR_Y     ,DI_OFF ,FRC_MIB):
            u4AF_TO_CF      =SV_OFF;
            u4BF_TO_CF      =SV_ON;   
            u4ABCD_YC_EN    =0x4;
            u4DRAM_RW_EN    =E_NR_W | E_SRC_R | E_MV_W | E_MV_R | E_RNR_W | E_RNR_R;
            u4DA_MODE_YUV   =MIB_YUV_MODE_YC;
            u4DA_NR_YUV_MODE=MIB_YUV_MODE_YC; 
            u4DA_FLAG_RNR_MODE = E_MIB_RNR_PROG_OUTPT_SYNC;
            break;            
        case E(P,NR_YC    ,DI_OFF ,FRC_MIB):
            u4AF_TO_CF      =SV_OFF;
            u4BF_TO_CF      =SV_ON;   
            u4ABCD_YC_EN    =0x0C;
            u4DRAM_RW_EN    =E_NR_W | E_SRC_R | E_MV_W | E_MV_R | E_RNR_W | E_RNR_R;
            u4DA_MODE_YUV   =MIB_YUV_MODE_YC;
            u4DA_NR_YUV_MODE=MIB_YUV_MODE_YC; 
            u4DA_FLAG_RNR_MODE = E_MIB_RNR_PROG_OUTPT_SYNC;
            break; 
        case E_MIB_DMODE_UNKNOWN:                
        default:
            //Turn Off PSCAN DRAM Engine
            u4AF_TO_CF      =SV_OFF;
            u4BF_TO_CF      =SV_ON;   
            u4ABCD_YC_EN    =0x0;
            u4DRAM_RW_EN    =0x0;
            u4DA_MODE_YUV   =MIB_YUV_MODE_YC;
            u4DA_NR_YUV_MODE=MIB_YUV_MODE_YC; 
            u4DA_FLAG_RNR_MODE = E_MIB_RNR_DISABLE;
    }

    //RNR R/W Only needed when there is frame delay
    if(!((VDP_1==u1VdpId)&& (IS_INTERLACE(VDP_1) || IS_MIB_FRC(VDP_1))))
    {
        u4DRAM_RW_EN &= ~(E_RNR_W | E_RNR_R);
    }
    if (IS_3D_FMT_CONV(u1VdpId))
    {
        u4DRAM_RW_EN &= ~(E_MV_W | E_MV_R | E_LM_W | E_LM_R | E_RNR_W | E_RNR_R);
    }


    //DRAM Engine 444/422 Mode
    if ((bIsScalerInput444(u1VdpId) || (!u1DrvDIIsSupportedSignal(u1VdpId)))&&(eDataMode != E_MIB_DMODE_UNKNOWN)&&(IS_MIB_FRC(u1VdpId)))
    {
        u4AF_TO_CF      =SV_OFF;
        u4BF_TO_CF      =SV_OFF;   
        u4ABCD_YC_EN    =0x0;
        u4DRAM_RW_EN    =E_SRC_W;
        u4DA_MODE_YUV = bIsScalerInput444(u1VdpId)?MIB_YUV_MODE_YUV : MIB_YUV_MODE_YC;
        u4DA_NR_YUV_MODE=MIB_YUV_MODE_C_ONLY; 
        u4DA_FLAG_RNR_MODE = E_MIB_RNR_DISABLE;
    }

    u4DRAM_RW_EN = (_arMDDiPrm[u1VdpId].u1OnOff) ? u4DRAM_RW_EN : E_ALL_RW_OFF;

    //When Turn OFF A->C , Need to wait for DRAM Refill
    if((SV_ON==u4PreAF_TO_CF[u1VdpId])&&(SV_OFF==u4AF_TO_CF))
    {
        rMIBState[u1VdpId].u1AToCFldCnt=DRAM_REFILL_DELAY;  
    }    

    u4PreAF_TO_CF[u1VdpId] = u4AF_TO_CF;
    
    if((rMIBState[u1VdpId].u1AToCFldCnt==0)||(SV_ON==u4AF_TO_CF))
    {
        MDDI_WRITE_FLD(u1VdpId,MCVP_KC_17,u4AF_TO_CF        ,AF_TO_CF);
    }

#ifdef CC_SUPPORT_VENC
    if (_arMDDiPrm[u1VdpId].u1VencOn)
    {
        if (MDDI_READ_FLD(u1VdpId,MCVP_MULTI_00,EN_SRCW))
        {
            u4DRAM_RW_EN = E_SRC_W;
        }
        else
        {
            u4DRAM_RW_EN = 0;
        }
    }
#endif
#ifdef MIB_WIDTH_PREDOWN_LOW_256_WA
    if(_arMDDiPrm[u1VdpId].u2Width < 256)
    {
        u4DRAM_RW_EN &= ~(E_MV_W | E_MV_R | E_LM_W | E_LM_R );
    }
#endif
    MDDI_WRITE_FLD(u1VdpId,MCVP_KC_17   ,u4BF_TO_CF         ,BF_TO_CF);
    MDDI_WRITE_FLD(u1VdpId,MCVP_KC_17   ,u4ABCD_YC_EN       ,ABCD_YC_EN);
    MDDI_WRITE_FLD(u1VdpId,MCVP_MULTI_00,u4DRAM_RW_EN       ,EN_DRAM_RW);
    MDDI_WRITE_FLD(u1VdpId,MCVP_DRAM_08 ,u4DA_MODE_YUV      ,DA_MODE_YUV);    
	MDDI_WRITE_FLD(u1VdpId,MCVP_DRAM_14 ,u4DA_NR_YUV_MODE   ,DA_NR_YUV_MODE); 
    MDDI_WRITE_FLD(u1VdpId,MCVP_DRAM_14 ,(IS_420TO422(u1VdpId)),DA_NR_420); 
    MDDI_WRITE_FLD(u1VdpId,MCVP_DRAM_01 ,u4DA_FLAG_RNR_MODE ,DA_FLAG_RNR_MODE); 
	
	if(IS_420TO422(u1VdpId) && IS_PROG_DISPMODE(u1VdpId))
	{
        MDDI_WRITE_FLD(u1VdpId,MCVP_DRAM_1A  , 1, DA_SC_MODE_YUV); //YC 420
	}
	else if(bIsScalerInput444(u1VdpId)&& IS_PROG_DISPMODE(u1VdpId))
	{
	    MDDI_WRITE_FLD(u1VdpId,MCVP_DRAM_1A  , 3, DA_SC_MODE_YUV); //YUV
	}
	else
	{
	    MDDI_WRITE_FLD(u1VdpId,MCVP_DRAM_1A  , 2, DA_SC_MODE_YUV); //YC 422
	}
	
    if((u4PrevDMODE[u1VdpId]!=eDataMode)||rMIBState[u1VdpId].u1ModeChg ||
        ((u1PreTvdMode != bHwTvdMode()) && (u1VdpId == SV_VP_MAIN)))
    {
        if(u1VdpId == SV_VP_MAIN)
        {
            u1PreTvdMode = bHwTvdMode();
        }
        u4PrevDMODE[u1VdpId]=eDataMode;
        _vDrvDICfgPQ(u1VdpId);   
    }
    
}

/**
 * @brief Set DI Dram Control
 * @param u1VdpId VDP_1/VDP_2
 * @param u1DramCOnOff Enable/Disable
 */
static inline void _vDrvDISetFmtCtrl(UINT8 u1VdpId,E_TD_IN TdIn,E_TD_OUT TdOut)
{   
    PATH_CHECK(u1VdpId);

    if (u1VdpId == VDP_2)
    {
        if (SV_ON == _arMDDiPrm[VDP_2].u1IsDualFPI ||
            MDDI_READ_FLD(VDP_2, MCVP_KC_0D, IN_DISP_R) == 1)
        {
            MDDI_WRITE_FLD(VDP_2, MCVP_KC_00, 0, DISP_R_IN_P);
        }
        else
        {
            MDDI_WRITE_FLD(VDP_2, MCVP_KC_00, 1, DISP_R_IN_P);
        }
    }
    
    //Kernal Output
    MDDI_WRITE_FLD(u1VdpId, MCVP_BC_00,IS_PR_MODE(u1VdpId), PR_MODE_EN);

    if((E_TD_OUT_3D_LI==_arMDDiPrm[u1VdpId].eTDOutput)
        &&(!IS_PR_MODE(u1VdpId)))
    {
        MDDI_WRITE_FLD(u1VdpId, MCVP_KC_09, E_HW_HV_DOUBLE, O_VDEW_X2_FORCE);
    }
    else
    {
        MDDI_WRITE_FLD(u1VdpId, MCVP_KC_09, E_HW_HV_NO, O_VDEW_X2_FORCE);
    }

    if((SV_ON==_arMDDiPrm[u1VdpId].u1IsDualFPI)&&IS_MIB_FRC(u1VdpId))
    {
        MDDI_WRITE_FLD(u1VdpId, MCVP_BC_06, E_HW_KR_2D,IN_FORMAT);        
        MDDI_WRITE_FLD(u1VdpId, MCVP_BC_06, E_HW_KR_2D, OUT_FORMAT);           
    }
    else
    {
        //Kernel Out Format
    
         switch(TdOut)
         {
            case E_TD_OUT_3D_LI:
                MDDI_WRITE_FLD(u1VdpId, MCVP_BC_06, E_HW_KR_LI, OUT_FORMAT);            
                break;
            case E_TD_OUT_3D_FS:
                MDDI_WRITE_FLD(u1VdpId, MCVP_BC_06, E_HW_KR_FS, OUT_FORMAT);
                break;
            case E_TD_OUT_3D_TB:
                MDDI_WRITE_FLD(u1VdpId, MCVP_BC_06, E_HW_KR_TB, OUT_FORMAT);
                break;                
            default:    
                MDDI_WRITE_FLD(u1VdpId, MCVP_BC_06, E_HW_KR_2D, OUT_FORMAT);           
        }

        //Kernel Input
        switch(TdIn)
        {
            case E_TD_IN_FS_I:           
            case E_TD_IN_FS_P:            
                MDDI_WRITE_FLD(u1VdpId, MCVP_BC_06, E_HW_KR_FS,IN_FORMAT);                 
                break;            
            case E_TD_IN_SBS_I:
            case E_TD_IN_SBS_P:            
                MDDI_WRITE_FLD(u1VdpId, MCVP_BC_06, E_HW_KR_SBS,IN_FORMAT);        
                break;            
            case E_TD_IN_TB_I:
            case E_TD_IN_TB_P:            
                MDDI_WRITE_FLD(u1VdpId, MCVP_BC_06, E_HW_KR_TB,IN_FORMAT);        
                break;             
            case E_TD_IN_LI_I:
            case E_TD_IN_LI_P:            
                MDDI_WRITE_FLD(u1VdpId, MCVP_BC_06, E_HW_KR_LI,IN_FORMAT);        
                    break;                         
            default:
                MDDI_WRITE_FLD(u1VdpId, MCVP_BC_06, E_HW_KR_2D,IN_FORMAT);        
        }
    }
    
    //SrcW
    switch(TdIn)
    {
        case E_TD_IN_FS_I:
        case E_TD_IN_FS_P:                        
            MDDI_WRITE_FLD(u1VdpId, MCVP_DRAM_15, SV_ON, DA_3D_INPUT_EN);            
            MDDI_WRITE_FLD(u1VdpId, MCVP_DRAM_15, E_HW_DA_FS, DA_3D_INPUT_MODE);
            break;                        
        case E_TD_IN_SBS_I:
        case E_TD_IN_SBS_P:            
            MDDI_WRITE_FLD(u1VdpId, MCVP_DRAM_15, SV_ON, DA_3D_INPUT_EN);            
            MDDI_WRITE_FLD(u1VdpId, MCVP_DRAM_15, E_HW_DA_SBS, DA_3D_INPUT_MODE);
            break;            
        case E_TD_IN_TB_I:
        case E_TD_IN_TB_P:            
            MDDI_WRITE_FLD(u1VdpId, MCVP_DRAM_15, SV_ON, DA_3D_INPUT_EN);            
            MDDI_WRITE_FLD(u1VdpId, MCVP_DRAM_15, E_HW_DA_TB, DA_3D_INPUT_MODE);
            break;
        case E_TD_IN_LI_I:
        case E_TD_IN_LI_P:            
            MDDI_WRITE_FLD(u1VdpId, MCVP_DRAM_15, SV_ON, DA_3D_INPUT_EN);            
            MDDI_WRITE_FLD(u1VdpId, MCVP_DRAM_15, E_HW_DA_LI, DA_3D_INPUT_MODE);
            break;               
        case E_TD_IN_2D_I:
        case E_TD_IN_2D_P: 
        default:
            MDDI_WRITE_FLD(u1VdpId, MCVP_DRAM_15, SV_OFF, DA_3D_INPUT_EN);            
            MDDI_WRITE_FLD(u1VdpId, MCVP_DRAM_15, E_HW_DA_FS, DA_3D_INPUT_MODE);
    }

    //NR Write /Src read  
    if(IS_INTERLACE(u1VdpId)&&IS_MIB_FRC(u1VdpId))
    {                 
        switch(TdOut)
        {
            case E_TD_OUT_3D_LI:            
                MDDI_WRITE_FLD(u1VdpId, MCVP_DRAM_14, SV_ON     , DA_NR_3D_INPUT_EN);            
                MDDI_WRITE_FLD(u1VdpId, MCVP_DRAM_14, E_HW_DA_LI, DA_NR_3D_INPUT_MODE);
                MDDI_WRITE_FLD(u1VdpId, MCVP_DRAM_15, SV_ON     , DA_3D_OUTPUT_EN);            
                MDDI_WRITE_FLD(u1VdpId, MCVP_DRAM_15, E_HW_DA_LI, DA_3D_OUTPUT_MODE);                  
                break;           
            case E_TD_OUT_3D_FS:
                MDDI_WRITE_FLD(u1VdpId, MCVP_DRAM_14, SV_ON     , DA_NR_3D_INPUT_EN);            
                MDDI_WRITE_FLD(u1VdpId, MCVP_DRAM_14, E_HW_DA_FS, DA_NR_3D_INPUT_MODE);
                MDDI_WRITE_FLD(u1VdpId, MCVP_DRAM_15, SV_ON     , DA_3D_OUTPUT_EN);            
                MDDI_WRITE_FLD(u1VdpId, MCVP_DRAM_15, E_HW_DA_FS, DA_3D_OUTPUT_MODE);                
                break;
            case E_TD_OUT_3D_TB:
                MDDI_WRITE_FLD(u1VdpId, MCVP_DRAM_14, SV_ON     , DA_NR_3D_INPUT_EN);            
                MDDI_WRITE_FLD(u1VdpId, MCVP_DRAM_14, E_HW_DA_TB, DA_NR_3D_INPUT_MODE);
                MDDI_WRITE_FLD(u1VdpId, MCVP_DRAM_15, SV_ON     , DA_3D_OUTPUT_EN);            
                MDDI_WRITE_FLD(u1VdpId, MCVP_DRAM_15, E_HW_DA_TB, DA_3D_OUTPUT_MODE);                
                break;                
            default:
                MDDI_WRITE_FLD(u1VdpId, MCVP_DRAM_14, SV_OFF    , DA_NR_3D_INPUT_EN);            
                MDDI_WRITE_FLD(u1VdpId, MCVP_DRAM_14, E_HW_DA_FS, DA_NR_3D_INPUT_MODE);
                MDDI_WRITE_FLD(u1VdpId, MCVP_DRAM_15, SV_OFF    , DA_3D_OUTPUT_EN);            
                MDDI_WRITE_FLD(u1VdpId, MCVP_DRAM_15, E_HW_DA_FS, DA_3D_OUTPUT_MODE);                 
        }                 
    }
    else
    {
        //NR Write    
        switch(TdIn)
        {
            case E_TD_IN_FS_I:
            case E_TD_IN_FS_P:                            
                MDDI_WRITE_FLD(u1VdpId, MCVP_DRAM_14, SV_ON     , DA_NR_3D_INPUT_EN);            
                MDDI_WRITE_FLD(u1VdpId, MCVP_DRAM_14, E_HW_DA_FS, DA_NR_3D_INPUT_MODE); 
                MDDI_WRITE_FLD(u1VdpId, MCVP_DRAM_15, SV_ON     , DA_3D_OUTPUT_EN);            
                MDDI_WRITE_FLD(u1VdpId, MCVP_DRAM_15, E_HW_DA_FS, DA_3D_OUTPUT_MODE);                   
                break;           
            case E_TD_IN_SBS_I:
            case E_TD_IN_SBS_P:            
                MDDI_WRITE_FLD(u1VdpId, MCVP_DRAM_14, SV_ON     , DA_NR_3D_INPUT_EN);            
                MDDI_WRITE_FLD(u1VdpId, MCVP_DRAM_14, E_HW_DA_SBS, DA_NR_3D_INPUT_MODE);
                MDDI_WRITE_FLD(u1VdpId, MCVP_DRAM_15, SV_ON     , DA_3D_OUTPUT_EN);            
                MDDI_WRITE_FLD(u1VdpId, MCVP_DRAM_15, E_HW_DA_SBS, DA_3D_OUTPUT_MODE);                   
                break;  
            case E_TD_IN_TB_I:
            case E_TD_IN_TB_P:            
                MDDI_WRITE_FLD(u1VdpId, MCVP_DRAM_14, SV_ON     , DA_NR_3D_INPUT_EN);            
                MDDI_WRITE_FLD(u1VdpId, MCVP_DRAM_14, E_HW_DA_TB, DA_NR_3D_INPUT_MODE);
                MDDI_WRITE_FLD(u1VdpId, MCVP_DRAM_15, SV_ON     , DA_3D_OUTPUT_EN);            
                MDDI_WRITE_FLD(u1VdpId, MCVP_DRAM_15, E_HW_DA_TB, DA_3D_OUTPUT_MODE);                   
                break;
            case E_TD_IN_LI_I:
            case E_TD_IN_LI_P:            
                MDDI_WRITE_FLD(u1VdpId, MCVP_DRAM_14, SV_ON     , DA_NR_3D_INPUT_EN);            
                MDDI_WRITE_FLD(u1VdpId, MCVP_DRAM_14, E_HW_DA_LI, DA_NR_3D_INPUT_MODE);
                MDDI_WRITE_FLD(u1VdpId, MCVP_DRAM_15, SV_ON     , DA_3D_OUTPUT_EN);            
                MDDI_WRITE_FLD(u1VdpId, MCVP_DRAM_15, E_HW_DA_LI, DA_3D_OUTPUT_MODE);                   
                break;                  
            default:
                MDDI_WRITE_FLD(u1VdpId, MCVP_DRAM_14, SV_OFF    , DA_NR_3D_INPUT_EN);            
                MDDI_WRITE_FLD(u1VdpId, MCVP_DRAM_14, E_HW_DA_FS, DA_NR_3D_INPUT_MODE);
                MDDI_WRITE_FLD(u1VdpId, MCVP_DRAM_15, SV_OFF     , DA_3D_OUTPUT_EN);            
                MDDI_WRITE_FLD(u1VdpId, MCVP_DRAM_15, E_HW_DA_FS, DA_3D_OUTPUT_MODE);                   
        }
    }


    //SC read
    switch(TdOut)
    {
        case E_TD_OUT_3D_LI:            
            MDDI_WRITE_FLD(u1VdpId, MCVP_DRAM_19, SV_ON     , DA_SC_3D_OUTPUT_EN);            
            MDDI_WRITE_FLD(u1VdpId, MCVP_DRAM_19, E_HW_DA_LI, DA_SC_3D_OUTPUT_MODE);            
            break;           
        case E_TD_OUT_3D_FS:
            MDDI_WRITE_FLD(u1VdpId, MCVP_DRAM_19, SV_ON     , DA_SC_3D_OUTPUT_EN);            
            MDDI_WRITE_FLD(u1VdpId, MCVP_DRAM_19, E_HW_DA_FS, DA_SC_3D_OUTPUT_MODE);
            break;
        case E_TD_OUT_3D_TB:
            MDDI_WRITE_FLD(u1VdpId, MCVP_DRAM_19, SV_ON     , DA_SC_3D_OUTPUT_EN);            
            MDDI_WRITE_FLD(u1VdpId, MCVP_DRAM_19, E_HW_DA_TB, DA_SC_3D_OUTPUT_MODE);
            break;            
        default:
            MDDI_WRITE_FLD(u1VdpId, MCVP_DRAM_19, SV_OFF    , DA_SC_3D_OUTPUT_EN);            
            MDDI_WRITE_FLD(u1VdpId, MCVP_DRAM_19, E_HW_DA_FS, DA_SC_3D_OUTPUT_MODE);            
    }
    
    MDDI_WRITE_FLD(u1VdpId,MCVP_KC_09,
        (E_TD_OUT_3D_FS==_arMDDiPrm[u1VdpId].eTDOutput), SC2MIB_DISP_R_EN);

    
}


/**
 * @brief Thread Pending Until PSCAN Interrupt Finished
 * @param None
 */
void vDrvDIWaitDIVSyncSema(void)
{
    VERIFY (x_sema_lock(_hDIPQIsrSema, X_SEMA_OPTION_WAIT) == OSR_OK);
}

/**
 * @brief Config and Monintor Auto Stop during mode change flow
 * @param u1VdpId VDP_1/VDP_2
 */
static void _vDrvDIAutoStopCtrl(UINT8 u1VdpId,UINT32 u4OnOff)
{

    if((SV_OFF==u4OnOff)
        ||(!IS_MPEG(u1VdpId))
        ||(E_TD_IN_FS_I==TD_DQC_IN(u4DrvTDTVDQCModeQuery()))
        )
    {
        rMIBState[u1VdpId].bAllowedAutoStop = SV_FALSE;
        vDrvDISetAutoStop(u1VdpId, SV_OFF);
    }
    else
    {
        if (rMIBState[u1VdpId].u1StableCnt > DRAM_REFILL_DELAY)
        {
            rMIBState[u1VdpId].bAllowedAutoStop = SV_TRUE;        
            vDrvDISetAutoStop(u1VdpId, rMIBState[u1VdpId].u1AutoStop);
        }
    }
}

/**
 * @brief Config DI mute related setting.
 * @param u1VdpId VDP_1/VDP_2
 */
void vDrvDISetMuteCtrl(UINT8 u1VdpId)
{
    UINT32 u4MuteSWReg = IO32ReadFldAlign(PSCAN_MISC_01, MDDI_MUTE_PERIOD_SW);
    if(!u4MuteSWReg)
    {
        #define MDDI_MUTE_DI_I        5 
        #define MDDI_MUTE_DI_P        2
        #define MDDI_MUTE_SCALER      3

        UINT32 u4Offset;
        VRM_INFO_T in;
        VRM_INFO_T out;

        u4Offset  = (IS_INTERLACE(u1VdpId)  ? MDDI_MUTE_DI_I : MDDI_MUTE_DI_P);
        u4Offset += (IS_MIB_FRC(u1VdpId)    ? 0              : MDDI_MUTE_SCALER);
        if (_arMDDiPrm[u1VdpId].u1InFR != 0)
        {
            VRMGetModuleVRMInfo(u1VdpId, VRM_MODULE_MJC, &in, &out);
            // if out frame rate > 77, 2 vsync 1 mainloop   plz see vdo_isr.
            out.u2FrameRate = (out.u2FrameRate > 77) ? (out.u2FrameRate >> 1) : out.u2FrameRate;
            u4Offset = u4Offset * (UINT32)((out.u2FrameRate +  _arMDDiPrm[u1VdpId].u1InFR - 1) / _arMDDiPrm[u1VdpId].u1InFR);
        }
        _vDrvVideoSetMute(MUTE_MODULE_MDDI01, u1VdpId, u4Offset, FALSE);
    }
    else
    {
        _vDrvVideoSetMute(MUTE_MODULE_MDDI01, u1VdpId, u4MuteSWReg, FALSE);
    }

}


//workaround for predown's set resolution signal error
/**
 * @brief workaround for predown's set resolution signal error
          Set the trigger after V-sync
 * @param u1VdpId VDP_1/VDP_2
 */
static inline void vDrvDITriggerPredown(UINT8 u1Path)
{
    if (vDrvQueryPDSTriggerFlg(u1Path))
    {
        vDrvSetPDSTriggerFlg(u1Path, FALSE);
        vDrvFirePDSImportTrigger(u1Path);
    }
}

/**
 * @brief Update status of DA_DEBUG
 */
static void vDrvDIUpdateDADebugStatus(void)
{
    MDDI_WRITE_FLD(VDP_1, MCVP_DRAM_12, 0, DA_DEBUG);
    rMIBState[VDP_1].u4DADebugStatus0 = MDDI_READ_FLD(VDP_1, MCVP_DRAM_13, STA_DEBUG);

    MDDI_WRITE_FLD(VDP_1, MCVP_DRAM_12, 1, DA_DEBUG);
    rMIBState[VDP_1].u4DADebugStatus1 = MDDI_READ_FLD(VDP_1, MCVP_DRAM_13, STA_DEBUG);

    MDDI_WRITE_FLD(VDP_1, MCVP_DRAM_12, 2, DA_DEBUG);
    rMIBState[VDP_1].u4DADebugStatus2 = MDDI_READ_FLD(VDP_1, MCVP_DRAM_13, STA_DEBUG);

    MDDI_WRITE_FLD(VDP_1, MCVP_DRAM_12, 3, DA_DEBUG);
    rMIBState[VDP_1].u4DADebugStatus3 = MDDI_READ_FLD(VDP_1, MCVP_DRAM_13, STA_DEBUG);
}

/**
 * @brief Set DI interrupt
 * @param u2Vector
 */
extern UINT8 u1DlyAfterISREn[2];
static void _vDrvDIISR(UINT16 u2Vector)
{
    ASSERT(u2Vector == VECTOR_PSCAN);

    // Input Vsync Rising
    if (MDDI_READ_FLD(VDP_1, MCVP_BC_02, INT_MIVS_R_STA))
    {
        MDDI_WRITE_FLD(VDP_1, MCVP_BC_01, SV_ON, INT_MIVS_R_CLR);//Clear interrupt
        _vDrvDISpVsyncIsrStatus(E_INPUT_VSYNC_RISING);
        rMIBState[VDP_1].u2Vcnt = MDDI_READ_FLD(VDP_1, MCVP_KC_2D, OUT_VDE_CNT);
        rMIBState[VDP_1].u1AToCFldCnt = rMIBState[VDP_1].u1AToCFldCnt ? rMIBState[VDP_1].u1AToCFldCnt -1 : 0;

#ifdef PSCAN_AUTO_VERIFY
        vDrvDIAutoVerifyInputISR(VDP_1);
#endif        

    }
    if (MDDI_READ_FLD(VDP_1, MCVP_BC_02, INT_PIVS_R_STA))
    {
        rMIBState[VDP_2].u1AToCFldCnt = rMIBState[VDP_2].u1AToCFldCnt ? rMIBState[VDP_2].u1AToCFldCnt -1 : 0;
#ifdef CC_SUPPORT_VENC
        vDrvDIVEncISR();
#endif
        MDDI_WRITE_FLD(VDP_1, MCVP_BC_01, SV_ON, INT_PIVS_R_CLR);//Clear interrupt
    }

    // Input Vsync Falling
    if (MDDI_READ_FLD(VDP_1, MCVP_BC_02, INT_MIVS_F_STA))
    {
        #if defined(CC_MT5399)
        vIO32WriteFldAlign(MJC_PD_INFO_01, IO32ReadFldAlign(MJCFD_08, MJCFD_FD_INFO), MJC_PD_INFO_MIB_IN);
        vIO32WriteFldAlign(MJC_PD_INFO_02, IO32ReadFldAlign(MJCFD_SEQ, MJCFD_FD_DET_SEQ_L), MJC_FW_FRM_SCH_SEQ_MIB_IN);
        vIO32WriteFldAlign(MJC_PD_INFO_04, IO32ReadFldAlign(MJCFD_SEQ, MJCFD_FD_DET_SEQ_R), MJC_FW_FRM_SCH_SEQ_MIB_IN_R);
        #endif
#ifdef PSCAN_AUTO_VERIFY
        vDrvDIISRAutoVerifyChk();
#endif    
        vDrvDIUpdateDADebugStatus();
        if(u1DlyAfterISREn[VDP_1] != 0)
        {
            u1DlyAfterISREn[VDP_1]--;
        }
        else
        {
            vDrvDITriggerPredown(VDP_1);//workaround for predown's set resolution signal error
        }

        if(IS_PROG_DISPMODE(VDP_1))
        {
            vSetPDInfoDelayAtProgDispMode();
        }
        _vDrvDISpVsyncIsrStatus(E_INPUT_VSYNC_FALLING);
        MDDI_WRITE_FLD(VDP_1, MCVP_BC_01, SV_ON, INT_MIVS_F_CLR);//Clear interrupt
    }
    
    if (MDDI_READ_FLD(VDP_1, MCVP_BC_02, INT_PIVS_F_STA))
    {  
        if(u1DlyAfterISREn[VDP_2] != 0)
        {
            u1DlyAfterISREn[VDP_2]--;
        }
        else
        {
            vDrvDITriggerPredown(VDP_2);//workaround for predown's set resolution signal error
        }
#ifdef CC_SUPPORT_VENC
        vDrvDIVEncWOffISR();
#endif    
        MDDI_WRITE_FLD(VDP_1, MCVP_BC_01, SV_ON, INT_PIVS_F_CLR);//Clear interrupt
    }

    // Input Vsync Adj
    if (MDDI_READ_FLD(VDP_1, MCVP_BC_02, INT_MI_ADJ_STA))
    {
        _vDrvDISpVsyncIsrStatus(E_INPUT_VSYNC_ADJ);
        //vPDSUpdateVInitPhaseReg(VDP_1,(_arMDDiPrm[VDP_1].eTDInput == E_TD_IN_FS_I),
        //                            (MDDI_READ_FLD(VDP_1, MCVP_KC_0D, IN_DISP_R) == 0),
        //                            (MDDI_READ_FLD(VDP_1, MCVP_KC_0D, IN_FLD)));
        MDDI_WRITE_FLD(VDP_1, MCVP_BC_01, SV_ON, INT_MI_ADJ_CLR);//Clear interrupt
    }
    if (MDDI_READ_FLD(VDP_1, MCVP_BC_02, INT_PI_ADJ_STA))
    {
        //vPDSUpdateVInitPhaseReg(VDP_2,0,0,MDDI_READ_FLD(VDP_2, MCVP_KC_0D, IN_FLD));
        MDDI_WRITE_FLD(VDP_1, MCVP_BC_01, SV_ON, INT_PI_ADJ_CLR);//Clear interrupt
    }

    // Output Vsync Rising
    if (MDDI_READ_FLD(VDP_1, MCVP_BC_02, INT_MOVS_R_STA))
    {
#ifdef CC_CDC_SUPPORT 
		vTriggerScalerDumpCopyDram();
#endif
        _vDrvDISpVsyncIsrStatus(E_OUTPUT_VSYNC_RISING);
        //vDrvDIDispmodeLRLockISR();
        vScpipOnDIOutputISR();

        if(_arMDDiPrm[VDP_1].eTDOutput == E_TD_OUT_3D_FS)
        {
            if(MDDI_READ_FLD(VDP_1, MCVP_KC_22, PQ_SRC_DISP_R))
            {//pq source disp r is 1 in blank means following active is active of L eye.
                vDrvDISetQualityISR_Ex();
            }
        }
        else
        {
            vDrvDISetQualityISR_Ex();
        }

        if (!u1DrvNRBNMISR())
        {
            SYS_MEASURE_TIME_START(SV_NR_TIMER2);         
            vDrvNRVBlankProc();
            SYS_MEASURE_TIME_END(SV_NR_TIMER2);
            SYS_STORE_TIME_INFO_TO_REG(SV_NR_TIMER2); 
        }

        MDDI_WRITE_FLD(VDP_1, MCVP_BC_01, SV_ON, INT_MOVS_R_CLR);//Clear interrupt
    }
    if (MDDI_READ_FLD(VDP_1, MCVP_BC_02, INT_POVS_R_STA))
    {
        MDDI_WRITE_FLD(VDP_1, MCVP_BC_01, SV_ON, INT_POVS_R_CLR);//Clear interrupt
    }

    // Output Vsync Adj
    if (MDDI_READ_FLD(VDP_1, MCVP_BC_02, INT_MO_ADJ_STA))
    {
        if (u1DrvNRBNMISR())
        {
            SYS_MEASURE_TIME_START(SV_NR_TIMER2);         
            vDrvNRVBlankProc();
            SYS_MEASURE_TIME_END(SV_NR_TIMER2);
            SYS_STORE_TIME_INFO_TO_REG(SV_NR_TIMER2); 
        }
        _vDrvDISpVsyncIsrStatus(E_OUTPUT_VSYNC_ADJ);
        MDDI_WRITE_FLD(VDP_1, MCVP_BC_01, SV_ON, INT_MO_ADJ_CLR);//Clear interrupt
    }
    if (MDDI_READ_FLD(VDP_1, MCVP_BC_02, INT_PO_ADJ_STA))
    {
        MDDI_WRITE_FLD(VDP_1, MCVP_BC_01, SV_ON, INT_PO_ADJ_CLR);//Clear interrupt
    }

// Output Vsync Falling
if (MDDI_READ_FLD(VDP_1, MCVP_BC_02, INT_MOVS_F_STA))
{
    #if 0
    #if defined(CC_MT5399)
    vIO32WriteFldAlign(MJC_PD_INFO_01, IO32ReadFldAlign(MJC_PD_INFO_01, MJC_PD_INFO_MIB_IN), MJC_PD_INFO_SCL_IN);
    vIO32WriteFldAlign(MJC_PD_INFO_03, IO32ReadFldAlign(MJC_PD_INFO_03, MJC_FW_FRM_SCH_SEQ_SCL_IN_DL1), MJC_FW_FRM_SCH_SEQ_SCL_IN_DL2);
    vIO32WriteFldAlign(MJC_PD_INFO_03, IO32ReadFldAlign(MJC_PD_INFO_02, MJC_FW_FRM_SCH_SEQ_SCL_IN), MJC_FW_FRM_SCH_SEQ_SCL_IN_DL1);
    vIO32WriteFldAlign(MJC_PD_INFO_02, IO32ReadFldAlign(MJC_PD_INFO_02, MJC_FW_FRM_SCH_SEQ_MIB_IN), MJC_FW_FRM_SCH_SEQ_SCL_IN);
    vIO32WriteFldAlign(MJC_PD_INFO_05, IO32ReadFldAlign(MJC_PD_INFO_05, MJC_FW_FRM_SCH_SEQ_SCL_IN_DL1_R), MJC_FW_FRM_SCH_SEQ_SCL_IN_DL2_R);
    vIO32WriteFldAlign(MJC_PD_INFO_05, IO32ReadFldAlign(MJC_PD_INFO_04, MJC_FW_FRM_SCH_SEQ_SCL_IN_R), MJC_FW_FRM_SCH_SEQ_SCL_IN_DL1_R);
    vIO32WriteFldAlign(MJC_PD_INFO_04, IO32ReadFldAlign(MJC_PD_INFO_04, MJC_FW_FRM_SCH_SEQ_MIB_IN_R), MJC_FW_FRM_SCH_SEQ_SCL_IN_R);
    #endif
	#endif
    vDrvDIGetCSStatusISR();
    SYS_MEASURE_TIME_START(SV_PSCAN_TIMER);  // measure PSCAN run time in VSYNC
    if((_arMDDiPrm[VDP_1].eTDOutput == E_TD_OUT_3D_FS) 
         && IS_INTERLACE(VDP_1))
    {
        if((MDDI_READ_FLD(VDP_1, MCVP_KC_22, PQ_SRC_DISP_R)) == 0)
        {
            vDrvDISetQualityISR(); 
            vDrvDIAdaptivePDSemaUnlock();
            x_sema_unlock(_hDIPQIsrSema);
        }
        else
        {       
        }
    }
    else
    {
        vDrvDISetQualityISR(); 
        vDrvDIAdaptivePDSemaUnlock();
        x_sema_unlock(_hDIPQIsrSema);
    }
    _vDrvDISpVsyncIsrStatus(E_OUTPUT_VSYNC_FALLING);
    
    SYS_MEASURE_TIME_END(SV_PSCAN_TIMER);
    SYS_STORE_TIME_INFO_TO_REG(SV_PSCAN_TIMER);   

    SYS_MEASURE_TIME_START(SV_NR_TIMER1);         
    vDrvNRISR();
    SYS_MEASURE_TIME_END(SV_NR_TIMER1);
    SYS_STORE_TIME_INFO_TO_REG(SV_NR_TIMER1); 
	
#if MIB_PORTING_TODO        
        vApiIsrLog(NPTV_ISR_PSCAN); 
#endif

#ifdef PSCAN_AUTO_VERIFY
        vDrvDIAutoVerifyISR(VDP_1);
#endif
    

    MDDI_WRITE_FLD(VDP_1, MCVP_BC_01, SV_ON, INT_MOVS_F_CLR);//Clear interrupt
    }

    if (MDDI_READ_FLD(VDP_1, MCVP_BC_02, INT_POVS_F_STA))
    {
#ifdef PSCAN_AUTO_VERIFY
        vDrvDIAutoVerifyISR(VDP_2);
#endif
        MDDI_WRITE_FLD(VDP_1, MCVP_BC_01, SV_ON, INT_POVS_F_CLR);//Clear interrupt
    }

     VERIFY(BIM_ClearIrq(VECTOR_PSCAN));
}


/**
 * @brief Set DI init interrupt
 */
static void _vDrvDIInitISR(void)
{
    static UINT8 u1Initialized = SV_FALSE;
    x_os_isr_fct pfnOldIsr;

    if (!u1Initialized)
    {
        VERIFY(x_sema_create(&_hDIPQIsrSema, X_SEMA_TYPE_BINARY, X_SEMA_STATE_UNLOCK) == OSR_OK);
        
        if (x_reg_isr(VECTOR_PSCAN, _vDrvDIISR, &pfnOldIsr) != OSR_OK)
        {
            LOG(7, "Error: fail to register PSCAN ISR!\n");
        }
        else
        {
            LOG(7, "Register PSCAN ISR OK!\n");
        }

        u1Initialized = SV_TRUE;
        LOG(7, "PSCAN ISR registered!\n");
    }
}


/**
 * @brief Get DI output clock rate
 * @param u1VdpId VDP_1/VDP_2
 * @return u4ClkRate clock rate
 */

static inline VOID _u4DrvDICfgColorFmt(UINT8 u1VdpId)
{
    UINT32 u4Is444,u4Is422;
    UINT32 u4NR420;
    
    if(bIsScalerInput444(u1VdpId))
    {
        u4Is444=SV_ON;
        u4Is422=SV_OFF;
    }
    else if(IS_420TO422(u1VdpId))
    {
        u4Is444=SV_OFF;
        u4Is422=SV_OFF;
    }
    else
    {    
        u4Is444=SV_OFF;
        u4Is422=SV_ON;
    }

    MDDI_WRITE_FLD(u1VdpId, MCVP_KC_1E, u4Is444, M444);    
    MDDI_WRITE_FLD(u1VdpId, MCVP_KC_1E, u4Is422, M422);

    u4NR420 = (IS_420TO422(u1VdpId)&&(IS_PROGRESSIVE(u1VdpId))) ? SV_ON : SV_OFF;
	
	if(u1VdpId == VDP_1)
	{
	    vDrvNRSetChromaUpSample(u4NR420);
	}
}

/**
 * @brief Set DI Misc
 * @param u1VdpId VDP_1/VDP_2
 */
static void _vDrvDISetMisc(UINT8 u1VdpId)
{
    UINT32 u4FldRegenOnOff;
    
    PATH_CHECK(u1VdpId);

    MDDI_WRITE_FLD(u1VdpId, MCVP_KC_17, 0, AUTO_TRICK_ON);

    //Fld Info Regen
    u4FldRegenOnOff = ((SV_VD_MPEGHD ==bGetVideoDecType(u1VdpId)) 
        || (TDTV_TAG3D_FS==u4DrvTDTVGetTag3D(u1VdpId))
        || (TDTV_TAG3D_FP==u4DrvTDTVGetTag3D(u1VdpId))
        || (TDTV_TAG3D_MVC==u4DrvTDTVGetTag3D(u1VdpId))    
        )
        ? SV_OFF : SV_ON;
    
    if(SV_VP_MAIN==u1VdpId)
    {
        vIO32WriteFldAlign(LPF_PIP_00, u4FldRegenOnOff, MAIN_PROG_FLD_EN);
    }
    else
    {
        vIO32WriteFldAlign(LPF_PIP_00, u4FldRegenOnOff, PIP_PROG_FLD_EN);
    }

    if (_arMDDiPrm[u1VdpId].u1IsDualFPI)
    {
        MDDI_WRITE_FLD(u1VdpId, MCVP_KC_2B, ((u1VdpId == VDP_1) ? 0x60 : 0x70), DISPMODE_H_BLANK); 
    }
    else
    {
        MDDI_WRITE_FLD(u1VdpId, MCVP_KC_2B, MIB_DISPMODE_HBLANK, DISPMODE_H_BLANK); 
    }
    MDDI_WRITE_FLD(u1VdpId, MCVP_KC_2B, 0x20, I2P_H_BLANK); 

    
    vDrvDISetLRSwap(u1VdpId,E_DI_LR_SWAP_UPDATE,0);

    MDDI_WRITE_FLD(u1VdpId, MCVP_DRAM_08, IS_INTERLACE(u1VdpId), DA_SNAKE_MAPPING);
    MDDI_WRITE_FLD(u1VdpId, MCVP_DRAM_14, IS_INTERLACE(u1VdpId), DA_NR_SNAKE_MAPPING);
    MDDI_WRITE_FLD(u1VdpId, MCVP_DRAM_18, IS_INTERLACE(u1VdpId), DA_SC_SNAKE_MAPPING);

    //If MIB module output PR signal,the 3d pr doesn't effect and the video flicker,so it need to WA.
    if((E_TD_OUT_3D_LI==_arMDDiPrm[u1VdpId].eTDOutput)&&(IS_INTERLACE(u1VdpId)))
    {
        MDDI_WRITE_FLD(u1VdpId,MCVP_KC_33, SV_OFF,I2P_TOP_VS_FALL_EN);
    }
    else
    {
        MDDI_WRITE_FLD(u1VdpId,MCVP_KC_33, (IS_INTERLACE(u1VdpId) && IS_MIB_FRC(u1VdpId)),I2P_TOP_VS_FALL_EN);
    }
    
    MDDI_WRITE_FLD(u1VdpId,MCVP_KC_33,(SV_ON==_arMDDiPrm[u1VdpId].u1IsDualFPI) ,OS_RST_NO_DELAY);
    
    MDDI_WRITE_FLD(u1VdpId,MCVP_KC_33,!(IS_MIB_FRC(u1VdpId) && IS_PROGRESSIVE(u1VdpId)) ,HDEW_ALIGN8_INC_EN);

    if (IS_PROGRESSIVE(u1VdpId) && (!IS_MIB_FRC(u1VdpId)))
    {
        MDDI_WRITE_FLD(u1VdpId, MCVP_KC_2B, 0, DIS_BLK_DE_LIMIT);     
    }
    else
    {
        MDDI_WRITE_FLD(u1VdpId, MCVP_KC_2B, 1, DIS_BLK_DE_LIMIT);     
    }


    if (IS_PR_MODE(u1VdpId) && IS_PROGRESSIVE(u1VdpId))
    {
        MDDI_WRITE_FLD(u1VdpId, MCVP_KC_00, 0x5, SRCW_V_BGN);
    }
    else if (!IS_MIB_FRC(u1VdpId))// input sync mode, no transition garbage when changing display window
    {
        MDDI_WRITE_FLD(u1VdpId, MCVP_KC_00, 0xC, SRCW_V_BGN);
    }
    else
    {
        MDDI_WRITE_FLD(u1VdpId, MCVP_KC_00, 0x8, SRCW_V_BGN);
    }

    #ifdef MIB_PROG_IN_SYNC_WA
        MDDI_WRITE_FLD(u1VdpId,MCVP_KC_33, 0,PROG_LAST_PIXEL_OPT);
    #else
    // for no WA case
        MDDI_WRITE_FLD(u1VdpId,MCVP_KC_33, (IS_PROGRESSIVE(u1VdpId) && !IS_MIB_FRC(u1VdpId)) || IS_SCL_BOB(_arMDDiPrm[u1VdpId].eDataMode),PROG_LAST_PIXEL_OPT);
    #endif
    
#ifdef MIB_COBRA_ECO1_PIP_GARBAGE
    if (u1VdpId == VDP_2  && !_arMDDiPrm[u1VdpId].u1IsPOP)
    {
        MDDI_WRITE_FLD(u1VdpId, MCVP_DRAM_08, 1, DA_RD_SHORT_BURST_MODE);
    }
    else
    {
        MDDI_WRITE_FLD(u1VdpId, MCVP_DRAM_08, 0, DA_RD_SHORT_BURST_MODE);
    }
#endif	               
}

/**
 * @brief Set Resolution Setting
 * @param u1VdpId VDP_1/VDP_2
 * @param u1ModeChg SV_ON/SV_OFF
 */
static void inline _vDrvDISetResEnCfg(UINT8 u1VdpId, UINT8 u1ModeChg)
{
    PATH_CHECK(u1VdpId);

    if(rMIBState[u1VdpId].u1DynRes 
        &&(SV_FALSE==u1ModeChg)&& !_arMDDiPrm[u1VdpId].u1IsDualFPI)
    {
        vDrvDISetResEnable(u1VdpId,TRUE);
    }
    else
    {        
        if (rMIBState[u1VdpId].u1DynRes)
        {
            rMIBState[u1VdpId].u1DynRes = FALSE;
            //inform can not do set resolution            
            vVRMSetEventFlg(u1VdpId, VRM_EVENT_BY_MIB);
        } 
        
        vDrvDISetResEnable(u1VdpId,FALSE);        
    }
}

/**
 * @brief Set Resolution ModeChange Cnt
 * @param u1VdpId VDP_1/VDP_2
 */
static void inline _vDrvDISetResStart(UINT8 u1VdpId)
{
    PATH_CHECK(u1VdpId);
    
    rMIBState[u1VdpId].u1SetResFinishedCnt = rMIBState[u1VdpId].u1DynRes 
        ? MIB_SET_RES_FINISH_CNT : 0;

    rMIBState[u1VdpId].u1StableCnt = 0;

    //DRAM of MIB should be refilled.
    _vDrvDIAutoStopCtrl(u1VdpId, SV_OFF);     
    vDrvDINonStdVMaskCfg(u1VdpId);
    
    MDDI_WRITE_FLD(u1VdpId,MCVP_KC_33, !(IS_MIB_FRC(u1VdpId) && IS_PROGRESSIVE(u1VdpId)),DIS_REPEAT_SET_RES);
    
}


/**
 * @brief Set some registers after applying new setting
 * @param u1VdpId VDP_1/VDP_2
 */
static void inline _vDrvDISetResEnd(UINT8 u1VdpId)
{
    PATH_CHECK(u1VdpId);

    if (rMIBState[u1VdpId].u1DynRes && 
        MDDI_READ_FLD(u1VdpId, MCVP_KC_18, MCVP_SET_RES_FINISH_I))
    {
        if (rMIBState[u1VdpId].u1SetResFinishedCnt > 0)
        {
            rMIBState[u1VdpId].u1SetResFinishedCnt --;
        }            
    }
    

    #ifdef MIB_PROG_IN_SYNC_WA
    if (rMIBState[u1VdpId].u1StableCnt > MIB_PROG_IN_WA_STABLE_CNT)
    {
        MDDI_WRITE_FLD(u1VdpId,MCVP_KC_33, (IS_PROGRESSIVE(u1VdpId) && !IS_MIB_FRC(u1VdpId)) || IS_SCL_BOB(_arMDDiPrm[u1VdpId].eDataMode),PROG_LAST_PIXEL_OPT);
    }
    #endif
    if (rMIBState[u1VdpId].u1StableCnt >= MIB_SET_RES_TIME_OUT)
    {
        rMIBState[u1VdpId].u1SetResFinishedCnt = 0;
    }


    if(0 == rMIBState[u1VdpId].u1SetResFinishedCnt)
    {    
        MDDI_WRITE_FLD(u1VdpId,MCVP_KC_17, rMIBState[u1VdpId].u4SetResNewKDE , K_DE_LOW_MIN);
        MDDI_WRITE_FLD(u1VdpId,MCVP_KC_33, SV_ON ,DIS_REPEAT_SET_RES);
        _vDrvDICSFlagCfg(u1VdpId);
        _vDrvDIAutoStopCtrl(u1VdpId,SV_ON); 
        vDrvDINonStdVMaskCfg(u1VdpId);    
    }

    // increase the stable counter
    if (rMIBState[u1VdpId].u1StableCnt == MIB_STABLE_MAX_CNT)
    {//recount from 128 for DI status polling    < 128 means it's modechange/set resolution
        rMIBState[u1VdpId].u1StableCnt = MIB_STABLE_CNT_RESTART_NUM;
    }
    else
    {
        rMIBState[u1VdpId].u1StableCnt ++;
    }
    vDrvClrImportWaitFlag(u1VdpId, VRM_IMPORT_WAIT_FLAG_MIB);
}

/**
 * @brief Set DI field parameter
 * @param u1VdpId VDP_1/VDP_2
 */
static void inline _vDrvDICfgFlipMirrorParam(UINT8 u1VdpId)
{
    if(IS_FLIP_MIRROR(u1VdpId))
    {
        vDrvDISetFlipMirror(u1VdpId, u1FlipMirrorConfig);
    }
    else
    {
        vDrvDISetFlipMirror(u1VdpId, DI_NORMAL_MODE);
    }
}

/**
 * @brief Set DI Speed Configuration
 * @param u1VdpId VDP_1/VDP_2
 * @param u2HTotal
 * @param u2Width
 */
static inline void _vDrvDICfgDeRatio(UINT8 u1VdpId)
{
    #define DE_BASE_M 25
    #define DE_N_LOWER_BOUND 5
    #define CHROMA_LINE_AVG_LOWEST_SPEED 2
    #define NON_STD_SPEED_BOOST_IN_BLANK 4
    #define SHIFT_FOR_PREVENT_OVERFLOW   4
    
    UINT32 u4OutSigClk,u4InSigClk,u4OutDERatio,u4DERatio,u4DELmt;
    UINT32 u4OutHT, u2Height;
    UINT16 u2NSTDRatio;

    u4OutHT =  _arMDDiPrm[u1VdpId].u2Width + MDDI_READ_FLD(u1VdpId, MCVP_KC_2B, DISPMODE_H_BLANK);
    u2Height =  _arMDDiPrm[u1VdpId].u2Height;

    if((E_TD_OUT_3D_TB==_arMDDiPrm[u1VdpId].eTDOutput)
        ||(E_TD_OUT_3D_LI==_arMDDiPrm[u1VdpId].eTDOutput))
    {
        u2Height *=2;
    }

    //u2Height = MAX(u2Height,_arMDDiPrm[u1VdpId].u4OutHeight);
    
    //Output DE Config
    u4OutSigClk = (_arMDDiPrm[u1VdpId].u4OutHeight==0) ? 0 : u4OutHT  
        * (( u2Height * _arMDDiPrm[u1VdpId].u4OutVT + _arMDDiPrm[u1VdpId].u4OutHeight - 1) / _arMDDiPrm[u1VdpId].u4OutHeight)
        * _arMDDiPrm[u1VdpId].u1OutFR;
    
    //u4OutSigClk = (_arMDDiPrm[u1VdpId].u1IsDualFPI) ? (u4OutSigClk +1)>>1 : u4OutSigClk;
    
    u4OutDERatio = (UINT32)(((u4OutSigClk>>SHIFT_FOR_PREVENT_OVERFLOW) * DE_BASE_M +  ((u4DrvDIGetClkRate(u1VdpId)*2)>>SHIFT_FOR_PREVENT_OVERFLOW) -1)
        / ((u4DrvDIGetClkRate(u1VdpId)*2)>>SHIFT_FOR_PREVENT_OVERFLOW));    
    
    u4OutDERatio = MIN(DE_BASE_M/2,(u4OutDERatio * 11 + 9)/ 10); // Run A Little Faster than scl lower bound
    u4OutDERatio = MAX(u4OutDERatio, DE_N_LOWER_BOUND);

    MDDI_WRITE_FLD(u1VdpId, MCVP_KC_2E, DE_BASE_M    , OUT_TG_DE_M);
    MDDI_WRITE_FLD(u1VdpId, MCVP_KC_2E, u4OutDERatio , OUT_TG_DE_N);
#if defined(CC_FPGA_SUPPORT)
    MDDI_WRITE_FLD(u1VdpId, MCVP_KC_2E, 2, OUT_TG_DE_M);
    MDDI_WRITE_FLD(u1VdpId, MCVP_KC_2E, 1, OUT_TG_DE_N);
#endif    
    //Kernel DE Config
    u4InSigClk  = _arMDDiPrm[u1VdpId].u2VTotal * _arMDDiPrm[u1VdpId].u2HTotal * _arMDDiPrm[u1VdpId].u1InFR;
    u4InSigClk  = IS_INTERLACE(u1VdpId) ? (u4InSigClk +1)>>1 : u4InSigClk;
    u4InSigClk  = (_arMDDiPrm[u1VdpId].u1IsDualFPI) ? (u4InSigClk +1)>>1 : u4InSigClk;
   
    //59.94 Correction
    if(MODE_525I == bDrvVideoGetTiming(u1VdpId))
    {
        u4InSigClk = u4InSigClk / 1001 * 1000; //Using *1000/1001 will overflow
    }
    
    u4DERatio = MIN((u4InSigClk * DE_BASE_M +  u4DrvDIGetClkRate(u1VdpId) -1)/ u4DrvDIGetClkRate(u1VdpId),DE_BASE_M);
    
    if(_arMDDiPrm[u1VdpId].u1IsNSTDSignal)
    {
        u2NSTDRatio = (IS_PROGRESSIVE(u1VdpId)) ? DE_BASE_M : (DE_BASE_M/2);
        u4DERatio = MIN(u4DERatio+NON_STD_SPEED_BOOST_IN_BLANK, u2NSTDRatio);
    }
    
    //workaround for the flag dram write collision 
    if (IS_SD_TIMING(u1VdpId))
    {
        MDDI_WRITE_FLD(u1VdpId, MCVP_KC_03, u4DERatio-1, IPTG_DE_N); 
    }
    else
    {
        MDDI_WRITE_FLD(u1VdpId, MCVP_KC_03, u4DERatio, IPTG_DE_N); 
    }
    
    MDDI_WRITE_FLD(u1VdpId, MCVP_KC_04, DE_BASE_M, IPTG_DE_M); 

    // new larger K_DE_LOW_MIN
    if(0!=u4OutSigClk && 0!=u4InSigClk)
    {
        if (u1VdpId == VDP_2 || IS_DI_BOB(u1VdpId))
        {
            u4DELmt = 2;
        }
        else if(IS_MIB_FRC(u1VdpId)&&IS_INTERLACE(u1VdpId))
        {
            UINT32 u4InitDELmt;
            UINT32 u4DIClk = u4DrvDIGetClkRate(u1VdpId);
            #define PSCAN_DISPMODE_PREFETCH_LINES 8
            #define DISPMODE_SCALER_READY_LINES   9
            //NXNR 3 lines, Scaler 4 (max6) Lines, Scaler PIPE 0.5 lines, Out FIFO, 1.2 Llines

            u4OutSigClk/=2;     
            u4OutSigClk = (u4OutSigClk * 11 +9 )/10;
            MDDI_WRITE_FLD(u1VdpId, MCVP_KC_33, 1, FAST_PRE_FETCH); 
            u4DELmt = u4DIClk/u4OutSigClk;
            u4InitDELmt = (u4DIClk / u4OutHT / _arMDDiPrm[u1VdpId].u1OutFR * \
                (_arMDDiPrm[u1VdpId].u4OutVT - _arMDDiPrm[u1VdpId].u4OutHeight) / _arMDDiPrm[u1VdpId].u4OutVT \
                - PSCAN_DISPMODE_PREFETCH_LINES*MIB_PREFETCH_KDELOWMIN)/DISPMODE_SCALER_READY_LINES;
            u4DELmt = MIN( u4DELmt, u4InitDELmt);
            // has less line buffers under PR mode
            if ((IS_PR_MODE(u1VdpId)||(u1VdpId == VDP_2))&& u4DELmt > 1)
            {
                u4DELmt =u4DELmt-1 ;
            }
        }
        else
        {        
            MDDI_WRITE_FLD(u1VdpId, MCVP_KC_33, 0, FAST_PRE_FETCH); 
            u4DELmt =u4DrvDIGetClkRate(u1VdpId)/u4InSigClk;  
        }
    }
    else
    {
        u4DELmt =0;
    }

    // old smaller K_DE_LOW_MIN
   /* if(IS_MIB_FRC(u1VdpId))
    {
        u4OutDERatio = u4OutDERatio<<1;
        u4DERatio = MAX(u4DERatio,u4OutDERatio);
    }*/
      
    u4DELmt = u4DELmt ? (u4DELmt -1) : 0 ;    
    u4DELmt = IS_INTERLACE(u1VdpId) ? MAX(u4DELmt,1) : u4DELmt ;
    u4DELmt = MIN(u4DELmt,0xB); //Larger than 0xB can not improve PQ anymore
    

    if((MODE_625I == bDrvVideoGetTiming(u1VdpId))
        &&(SV_VD_TVD3D == bGetVideoDecType(u1VdpId))
        &&(SV_ON != (SV_ON==_arMDDiPrm[u1VdpId].u1IsDualFPI))
        &&(u1VdpId == VDP_1))
    {
        u4DELmt = MAX(u4DELmt,CHROMA_LINE_AVG_LOWEST_SPEED);       
    }

    rMIBState[u1VdpId].u4SetResNewKDE = u4DELmt;
#if defined(CC_FPGA_SUPPORT)
    rMIBState[u1VdpId].u4SetResNewKDE = 1;
#endif
    if(MDDI_READ_FLD(u1VdpId, MCVP_KC_17, K_DE_LOW_MIN) >= u4DELmt)
    {
        MDDI_WRITE_FLD(u1VdpId, MCVP_KC_17, u4DELmt , K_DE_LOW_MIN);    
        _vDrvDICSFlagCfg(u1VdpId);
    }
    
}



/**
 * @brief Set H/V clip
 * @param u1VdpId VDP_1/VDP_2
 * @param wSrcXOffset X_offset
 * @param wSrcYOffset Y_offset
 * @param wSrcWidth width
 * @param wSrcHeight height
 */
static inline void _vDrvDISetActiveWindow(UINT8 u1VdpId, UINT16 wSrcXOffset, UINT16 wSrcYOffset, UINT16 wSrcWidth, UINT16 wSrcHeight)
{
    //-----------------
    //Set Content Width    
    //-----------------

    MDDI_WRITE_FLD(u1VdpId, MCVP_KC_1E, 
        (IS_FRAME_SEQ(u1VdpId) && IS_INTERLACE(u1VdpId) && u1DrvDIIsSupportedSignal(u1VdpId)) ?
        SV_ON : SV_OFF, DISP_R_OUT_P);
    
    //Not Clip Right side Clipping because of MIB HW Limitation 
    MDDI_WRITE_FLD(u1VdpId, MCVP_KC_02, wSrcWidth, IPTG_H_ACT);    
    
    if(wSrcWidth == 0)// if HDEW/VDEW is zero,input vsync isr will not falling,set resolution trigger will fail.
    {
        MDDI_WRITE_FLD(u1VdpId, MCVP_KC_0A, 720, HDEW); 
    }
    else
    {
        MDDI_WRITE_FLD(u1VdpId, MCVP_KC_0A, wSrcWidth, HDEW); 
    }   

    MDDI_WRITE_FLD(u1VdpId, MCVP_KC_1F, MIB_DISP_HBLANK_MIN , OUT_TG_H_BLANK);

    //-----------------
    //Set Content Height    
    //-----------------
    
    if((VDP_2==u1VdpId)&&IS_PROGRESSIVE(u1VdpId))
    {   //SW WA for Sub Path Writing DRAM For Progressive Src Video Enc,
        wSrcHeight = wSrcHeight <<1;
    }
    
    if(wSrcHeight == 0)
    {
        MDDI_WRITE_FLD(u1VdpId, MCVP_KC_0A, 480, VDEW);
    }
    else
    {
        MDDI_WRITE_FLD(u1VdpId, MCVP_KC_0A, wSrcHeight , VDEW);
    }

    if (VDP_1==u1VdpId)
    {
        vDrvNRSetWindow(wSrcWidth, wSrcHeight);        
		vDrvR2CIntervalPatch(wSrcWidth, wSrcHeight);
        MDDI_WRITE_FLD(VDP_1, MCVP_KC_0B,
            (wSrcHeight + (IS_PROG_DISPMODE(VDP_1) ? 2 : (IS_SD_HEIGHT(VDP_1) ? 9 : 18))), OUT_ADJ_INT_LINE);
    }

    //Deinterlace PQ Active Window Configuration    
    vDrvDISetRegionMotionOnOff(u1VdpId);
    vDrvDISetROIPixel(u1VdpId, 4, 4);
    vDrvDISetFrcLine(u1VdpId);
    _vDrvDICfgDeRatio(u1VdpId);
#ifdef CC_SUPPORT_VENC
    if (u1VdpId == VDP_2)
    {
        vDrvDIVencModeChgPause();
    }    
#endif
    vDrvDIInitPscanCRC(u1VdpId);
    //fusion block motion and region motion error.after eco correct this issue.
    if(IS_INTERLACE(u1VdpId)&&u1VdpId==VDP_1)
    {
        UINT16 u2HeightB  = wSrcHeight/32;
        UINT16 u2WidthB   = wSrcWidth/16;
        UINT16 u2AreaSizeB = u2HeightB*u2WidthB;
        u2AreaSizeB = (u2AreaSizeB > 4095) ? 4095 : u2AreaSizeB;
        
        vIO32WriteFldAlign(MCVP_CS_3F , u2AreaSizeB, IF_REGION_MOTION_ECO_AREA);
        vIO32WriteFldAlign(MCVP_CS_3F ,SV_ON , IF_REGION_MOTION_ECO_ON);
    }
    //Update Self timing gen for debug pattern
    MDDI_WRITE_FLD(u1VdpId, MCVP_KC_01,
                       IS_INTERLACE(u1VdpId)? (_arMDDiPrm[u1VdpId].u2VTotal)/2 : (_arMDDiPrm[u1VdpId].u2VTotal), IPTG_SELF_VT);
    MDDI_WRITE_FLD(u1VdpId, MCVP_KC_01, (_arMDDiPrm[u1VdpId].u2HTotal), IPTG_SELF_HT);        
    MDDI_WRITE_FLD(u1VdpId, MCVP_KC_02,
                       IS_INTERLACE(u1VdpId)? (_arMDDiPrm[u1VdpId].u2Height)/2 : (_arMDDiPrm[u1VdpId].u2Height), IPTG_V_ACT);
    MDDI_WRITE_FLD(u1VdpId, MCVP_KC_02, (_arMDDiPrm[u1VdpId].u2Width) , IPTG_H_ACT);
}

/**
 * @brief Configure DI Flip-Mirror
 */
static inline void _vDrvDIFlipMirrorInit(void)
{
#ifdef CC_FLIP_MIRROR_SUPPORT    
    UINT8 u1Config = u1GetFlipMirrorConfig();

    if ((u1Config & SYS_MIRROR_CONFIG_ON) 
            && (u1Config & SYS_FLIP_CONFIG_ON))
    {
        u1FlipMirrorConfig = DI_FLIP_MIRROR_ON_MODE;
    }
    else if (u1Config & SYS_MIRROR_CONFIG_ON)
    {
        u1FlipMirrorConfig = DI_MIRROR_ON_MODE;
    }
    else if (u1Config & SYS_FLIP_CONFIG_ON)
    {
        u1FlipMirrorConfig = DI_FLIP_ON_MODE;
    }
    else
    {
        u1FlipMirrorConfig = DI_NORMAL_MODE;
    }
#else
        u1FlipMirrorConfig = DI_NORMAL_MODE;
#endif

}

/**
 * @brief Set DI OnOff
 * @param u1VdpId VDP_1/VDP_2
 */
static UINT8 _u1DrvDIMaskedOnOff(UINT8 u1VdpId)
{
    UINT8 u1IsChannelOn = (u1VdpId == VDP_1) ? _rMChannel.bIsChannelOn : _rPChannel.bIsChannelOn;
    UINT8 u1SigStatus = bDrvVideoSignalStatus(u1VdpId);
    UINT8 u1SigType = bGetSignalType(u1VdpId);
    UINT8 u1IsIntr = bDrvVideoIsSrcInterlace(u1VdpId);

    // 1. Channel is off
    if ((u1IsChannelOn == SV_FALSE)
        // 2. Timing is not supported
        || (u1SigStatus == SV_VDO_NOSUPPORT)
        // 3. Snow screen should be displayed even signal is not stable (TV)
        || ((u1SigType != SV_ST_TV) && ((u1SigStatus == SV_VDO_NOSIGNAL) || (u1SigStatus == SV_VDO_UNKNOWN)))
        // 4. Bypass DI of sub path when progressive signal.
        || ((u1VdpId == VDP_2)&&(!u1IsIntr))   
        // 5. Input height/width is zero.
        || (_arMDDiPrm[u1VdpId].u2Width == 0) || (_arMDDiPrm[u1VdpId].u2Height  == 0)
        // 6. Byass DI when signal is not support and input sync mode.
        || ((!u1DrvDIIsSupportedSignal(u1VdpId)) && (!IS_MIB_FRC(u1VdpId)))
        // 7. force to bypass DI
        || (_arMDDiPrm[u1VdpId].u1ForceDIBypass)
        )    
    {
        return SV_OFF;
    }
    else
    {
        return SV_ON;
    }
}

static UINT8 _u1DrvDIGetMIB420Mode(UINT8 u1VdpId)
{    
    return (IS_MPEG(u1VdpId)
            //Before ECO , Viper Can not support 420 under progressive
            && !((BSP_GetIcVersion() == IC_VER_5880_AA)&&IS_PROGRESSIVE(u1VdpId))
            && !((E_TD_IN_LI_I==_arMDDiPrm[u1VdpId].eTDInput)||(E_TD_IN_LI_P==_arMDDiPrm[u1VdpId].eTDInput)))
        || _arMDDiPrm[u1VdpId].u1Force420Mode;
}

static VOID _vDrvDICfgPQ(UINT8 u1VdpId)
{    
    E_2DNR_MODE e2DNR=NXNR_MODE_OFF;
    E_3DNR_MODE e3DNR=MCNR_MODE_OFF;

    //NR Quality Configration
    if(SV_VP_MAIN==u1VdpId)
    {
#if !defined(__MODEL_slt__)

        //2D NR Configuation
        if( 
            //NXNR Can't support 444 Mode
            bIsScalerInput444(u1VdpId)
            //NXNR Is used to merge Main/Sub for DualFPI mode
            ||(_arMDDiPrm[SV_VP_MAIN].u1IsDualFPI)
            //NXNR SRAM is used by DQC , Should be turn off
            ||(IS_DQ(SV_VP_MAIN)
                &&IS_INTERLACE(SV_VP_MAIN))
            //NXNR Height Can not be larger than 2047
            ||((((E_TD_OUT_3D_LI==_arMDDiPrm[u1VdpId].eTDOutput)
                ||(E_TD_OUT_3D_TB==_arMDDiPrm[u1VdpId].eTDOutput)) 
                ?_arMDDiPrm[u1VdpId].u2Height*2 : _arMDDiPrm[u1VdpId].u2Height)
                >2047)
            )            
        {   
            e2DNR =NXNR_MODE_OFF;
        }
        else
        {
            e2DNR =NXNR_MODE_ON;
        }    

        //3D NR Configuration
        if(bIsScalerInput444(u1VdpId))
        {
             e3DNR=MCNR_MODE_OFF;                           
        }
        else
        {
            if(E_MIB_NR_YC==MIB_NR_MODE(VDP_1))
            {                
                vDrvNRSet3DNRChromaPath(SV_ON);
            }
            else
            {
                vDrvNRSet3DNRChromaPath(SV_OFF);
            }
            
            switch(MIB_NR_MODE(VDP_1))
            {         
                case E_MIB_NR_YC:
                case E_MIB_NR_Y:    
                    if(IS_SD_TIMING(VDP_1))
                    {
                        e3DNR = MCNR_MODE_MC;         
                    }
                    else
                    {
                        e3DNR = MCNR_MODE_MA;
                    }
                    break;
                case E_MIB_NR_OFF:     
                case E_MIB_NR_UNKNOWN: 
                default:
                    e3DNR =MCNR_MODE_OFF;
            }
        }
#endif  
        if(e2DNR == NXNR_MODE_OFF)//bypass nxnr,the Height will lost a line.So WA
        {
            MDDI_WRITE_FLD(u1VdpId, MCVP_KC_2B, 1, DIS_BLK_DE_LIMIT);
        }
        
        vDrvNRSet2DNRMode(e2DNR);
        vDrvNRSet3DNRMode(e3DNR);
        
        vDrvNRSetQuality(IS_INTERLACE(VDP_1), _arMDDiPrm[VDP_1].u2Width, _arMDDiPrm[VDP_1].u2Height);            
    }

    vDrvNRChromaLineAvgCfg(u1VdpId);
    //Deinterlace Quality Configuation    
    vDrvDISetQuality(u1VdpId);
    vDrvDISetInterpMode(u1VdpId, MIB_DI_MODE(u1VdpId));    
    // NR workaround for 420 SD progressive bottom line chroma NR issue
    vDrvNRSet3DNRDemo(IO32ReadFldAlign(DEMO_00, SPLIT_ONOFF)&&(IO32ReadFldAlign(DEMO_02, DEMO_3DNR_EN)), IO32ReadFldAlign(DEMO_00, REGION_LEFT_RIGHT), _arMDDiPrm[VDP_1].u2Width/2);
}

/**
 * @brief DI mode change check loop
 * @param u1VdpId VDP_1/VDP_2
 */
static inline UINT8 _u1DrvDIModeChange(UINT8 u1VdpId)
{
    //In / out fmt
    VRM_INFO_T in;
    VRM_INFO_T out;
    
    UINT32 u4IsSeamlessSwithNeeded  = SV_OFF;
    UINT32 u4IsSystemRelayoytNeeded = SV_OFF;
    
    E_TD_IN eDQCIn= TD_DQC_IN(u4DrvTDTVDQCModeQuery());

    MIB_STATIC_STATE rNewMIBCfg;            

    PATH_CHECK(u1VdpId);    

    //Inital Value from previous state
    x_memcpy((void *)&rNewMIBCfg,(void *)&_arMDDiPrm[u1VdpId],sizeof(MIB_STATIC_STATE));

    //Get System Signal latest State
    rNewMIBCfg.u1OnOff          = _u1DrvDIMaskedOnOff(u1VdpId);
    rNewMIBCfg.u1DecType        = bGetVideoDecType(u1VdpId);
    rNewMIBCfg.u1Interlace      = bDrvVideoIsSrcInterlace(u1VdpId);
    rNewMIBCfg.u1FrcMode        = (E_MIB_FRC_DMODE)_u1DrvDIGetFrcMode(u1VdpId);
    rNewMIBCfg.eDataMode        = _u1DrvDIGetMIBDataMode(u1VdpId);    
    rNewMIBCfg.u1420Mode        = _u1DrvDIGetMIB420Mode(u1VdpId);
    rNewMIBCfg.u1IsNSTDSignal   = bDrvVideoIsNonStandardSignal(u1VdpId);
    rNewMIBCfg.u2SigWidth       = wDrvVideoInputWidth(u1VdpId);// Signal Native Width
    rNewMIBCfg.u2SigHeight      = wDrvVideoInputHeight(u1VdpId);  // Signal Native Width
#if defined(CC_FPGA_SUPPORT)
    rNewMIBCfg.u1OnOff          = 1;
    rNewMIBCfg.u1DecType        = SV_VD_DVI;
    rNewMIBCfg.u1420Mode        = 0;
    rNewMIBCfg.u1IsNSTDSignal   = 0;
#endif
#ifdef CC_SUPPORT_VENC  
    rNewMIBCfg.u1VencOn         = (u1VdpId == VDP_2 && _u1DrvDIVencRunning()) ? SV_TRUE : SV_FALSE;
#endif
    //Get SRM/ FBM Information
    rNewMIBCfg.u4BufAddr        = rMIBFbmPool[u1VdpId].u4Addr;
    rNewMIBCfg.u4BufSize        = rMIBFbmPool[u1VdpId].u4Size;
    rNewMIBCfg.u2BufWidth       = rMIBFbmPool[u1VdpId].u4Width;
    rNewMIBCfg.u2BufHeight      = rMIBFbmPool[u1VdpId].u4Height;
    rNewMIBCfg.u1BitRes         = rMIBFbmPool[u1VdpId].fg10bit ? 10 : 8;

    rNewMIBCfg.eTDInput         = r3DInOutFormat[u1VdpId].eTDInput;
    rNewMIBCfg.eTDOutput        = r3DInOutFormat[u1VdpId].eTDOutput;
    rNewMIBCfg.u1IsDualFPI      = u4DrvTDTVIsDualFPI(); //r3DInOutFormat[u1VdpId].u1IsDualFPI;

    if(u1VdpId == SV_VP_MAIN)
    {
        rNewMIBCfg.u1IsDQ           = (E_TD_IN_SS_P==eDQCIn)||(E_TD_IN_SS_I==eDQCIn);
    }
    else
    {
        rNewMIBCfg.u1IsDQ           = SV_FALSE;
    }
    rNewMIBCfg.u1IsPOP          = (SRM_IsPopVideo()==SV_ON)? SV_TRUE : SV_FALSE;
    
    //HT / VT / FR from pre-proc for data rate computation
    VRMGetModuleVRMInfo(u1VdpId, VRM_MODULE_PDS, &in, &out);
    rNewMIBCfg.u2HTotal  = in.u2HTotal;
    rNewMIBCfg.u2VTotal  = in.u2VTotal;
    rNewMIBCfg.u1InFR    = in.u2FrameRate;

    //WXH From the result of pre down
    VRMGetModuleVRMInfo(u1VdpId, VRM_MODULE_MIB, &in, &out);
    rNewMIBCfg.u2HClip   = in.u2X;
    rNewMIBCfg.u2VClip   = in.u2Y;
    rNewMIBCfg.u2Width   = in.u2Width;
    rNewMIBCfg.u2Height  = in.u2Height;

    //output Sync mode, MIB out timing depend on Scaler Time Gen
    if(IS_MIB_FRC(u1VdpId))
    {   
        VRMGetModuleVRMInfo(u1VdpId, VRM_MODULE_FSC, &in, &out);
    }
    rNewMIBCfg.u4OutVT     = out.u2VTotal;
    rNewMIBCfg.u4OutHT     = out.u2HTotal;
    rNewMIBCfg.u1OutFR     = out.u2FrameRate;
    rNewMIBCfg.u4OutHeight = out.u2Height;

    //For  interlace MM pause, switch aspect mode with only output window change.
    //Get output window setting to reset auto stop control
    VRMGetModuleVRMInfo(u1VdpId, VRM_MODULE_FSC, &in, &out);
    rNewMIBCfg.DispX        = out.u2X;
    rNewMIBCfg.DispY        = out.u2Y;
    rNewMIBCfg.DispW        = out.u2Width;
    rNewMIBCfg.DispH        = out.u2Height;

    if(SV_VD_VGA==bGetVideoDecType(u1VdpId))
    {
        rNewMIBCfg.u2HTotal+=MIB_VGA_HTOTAL_BOOST; 
    }

    if(IS_3D_FMT_CONV(SV_VP_MAIN))
    {
        if((E_TD_IN_TB_I==_arMDDiPrm[u1VdpId].eTDInput)||(E_TD_IN_TB_P==_arMDDiPrm[u1VdpId].eTDInput)
            ||(E_TD_IN_LI_I==_arMDDiPrm[u1VdpId].eTDInput)||(E_TD_IN_LI_P==_arMDDiPrm[u1VdpId].eTDInput))
        {
            rNewMIBCfg.u2Height/=2;
            rNewMIBCfg.u2BufHeight/=2;
            rNewMIBCfg.u2SigHeight/=2;
        }
        else if((E_TD_IN_SBS_I==_arMDDiPrm[u1VdpId].eTDInput)||(E_TD_IN_SBS_P==_arMDDiPrm[u1VdpId].eTDInput))
        {
            rNewMIBCfg.u2Width/=2;
            rNewMIBCfg.u2BufWidth/=2;
            rNewMIBCfg.u2SigWidth/=2;            
        }       
    }

    if(_arMDDiPrm[u1VdpId].u1DebugFlipMorrorByMIB == SV_TRUE)
    {
        rNewMIBCfg.u1FlipMorrorByMIB = SV_TRUE;
    }
    else
    {
#ifdef CC_FLIP_MIRROR_SUPPORT
        rNewMIBCfg.u1FlipMorrorByMIB = u4GetFlipMirrorModule(u1VdpId) == FLIP_BY_PSCAN;
#else
        rNewMIBCfg.u1FlipMorrorByMIB = SV_OFF;
#endif
    }

    //Dual FPI Mode, Use Sub Engine to collaberate 3DDI
    if(SV_ON==_arMDDiPrm[u1VdpId].u1IsDualFPI)
    {
        if(SV_VP_MAIN==u1VdpId)
        {
            rNewMIBCfg.u2SigWidth/=2;
            rNewMIBCfg.u2BufWidth/=2;
            rNewMIBCfg.u1IsPOP=SV_ON;
        }
        else
        {   
            x_memcpy((void*)&rNewMIBCfg, (void*)&_arMDDiPrm[SV_VP_MAIN], sizeof(MIB_STATIC_STATE));

            rNewMIBCfg.u4BufAddr =_arMDDiPrm[SV_VP_MAIN].u4BufAddr
                +_arMDDiPrm[SV_VP_MAIN].u4BufSize/2;
            rNewMIBCfg.u4BufSize =_arMDDiPrm[SV_VP_MAIN].u4BufSize/2;        
        }
    }
        
    //Check if Widht / Height need to be updated
    if (((rMIBState[u1VdpId].u1DynRes)
            &&((_arMDDiPrm[u1VdpId].u2HClip != rNewMIBCfg.u2HClip)
                || (_arMDDiPrm[u1VdpId].u2VClip != rNewMIBCfg.u2VClip)
                || (_arMDDiPrm[u1VdpId].u2Width != rNewMIBCfg.u2Width)
                || (_arMDDiPrm[u1VdpId].u2Height != rNewMIBCfg.u2Height)
                || (_arMDDiPrm[u1VdpId].u4OutVT != rNewMIBCfg.u4OutVT)
                || (_arMDDiPrm[u1VdpId].u4OutHT != rNewMIBCfg.u4OutHT)
                || (_arMDDiPrm[u1VdpId].u4OutHeight != rNewMIBCfg.u4OutHeight)
                || (_arMDDiPrm[u1VdpId].DispX != rNewMIBCfg.DispX)
                || (_arMDDiPrm[u1VdpId].DispY != rNewMIBCfg.DispY)
                || (_arMDDiPrm[u1VdpId].DispW != rNewMIBCfg.DispW)
                || (_arMDDiPrm[u1VdpId].DispH != rNewMIBCfg.DispH)))
        ||(_arMDDiPrm[u1VdpId].u1IsNSTDSignal != rNewMIBCfg.u1IsNSTDSignal)
                )
    {
        _arMDDiPrm[u1VdpId].u2HClip         = rNewMIBCfg.u2HClip;
        _arMDDiPrm[u1VdpId].u2VClip         = rNewMIBCfg.u2VClip;
        _arMDDiPrm[u1VdpId].u2Width         = rNewMIBCfg.u2Width;
        _arMDDiPrm[u1VdpId].u2Height        = rNewMIBCfg.u2Height;
        _arMDDiPrm[u1VdpId].u4OutVT         = rNewMIBCfg.u4OutVT;
        _arMDDiPrm[u1VdpId].u4OutHT         = rNewMIBCfg.u4OutHT;        
        _arMDDiPrm[u1VdpId].u4OutHeight     = rNewMIBCfg.u4OutHeight;
        _arMDDiPrm[u1VdpId].u1IsNSTDSignal  = rNewMIBCfg.u1IsNSTDSignal;
        _arMDDiPrm[u1VdpId].DispX           = rNewMIBCfg.DispX;
        _arMDDiPrm[u1VdpId].DispY           = rNewMIBCfg.DispY;
        _arMDDiPrm[u1VdpId].DispW           = rNewMIBCfg.DispW;
        _arMDDiPrm[u1VdpId].DispH           = rNewMIBCfg.DispH;

        u4IsSeamlessSwithNeeded  = SV_TRUE;        
    }

    // Check if MIB System re-layout is nessesary
    if (x_memcmp((const void *)&rNewMIBCfg,(const void *)&_arMDDiPrm[u1VdpId],sizeof(MIB_STATIC_STATE)))
    {       
        if((IO32ReadFldAlign(ADAPTIVE_PSCAN_REG, ADAPTIVE_MODECHG_LOG_M) && (VDP_1 == u1VdpId)) ||
		    (IO32ReadFldAlign(ADAPTIVE_PSCAN_REG, ADAPTIVE_MODECHG_LOG_S) && (VDP_2 == u1VdpId)))
    	{
    	    LOG(4,"==MIB Path %d Mode changed==\n",u1VdpId);

            #define MIB_ELE_CHG_LOG(pre,cur,element) if(pre.element != cur.element) \
                LOG(4,"Element-"#element": Pre %d != Cur %d\n",pre.element,cur.element);
            
            #undef MIB_ELE_CMD
            #define MIB_ELE_CMD(type,element) MIB_ELE_CHG_LOG(_arMDDiPrm[u1VdpId],rNewMIBCfg,element)
            do{ MIB_SYS_STATIC_ELE_LIST } while(0);
    	}

        if( IS_SCL_BOB(rNewMIBCfg.eDataMode)!=IS_SCL_BOB(_arMDDiPrm[u1VdpId].eDataMode))
        {   // notify VRM scaler Bob mode
            fireVRMModeChange(u1VdpId, VRM_EVENT_BY_MIB);
        }     

        x_memcpy((void *)&_arMDDiPrm[u1VdpId],(void *)&rNewMIBCfg,sizeof(MIB_STATIC_STATE));
        u4IsSystemRelayoytNeeded =SV_TRUE;        
    }

    //Check Bandwidth Limitation
    rMIBState[u1VdpId].eBandwidthDataMode  =_u1DrvDIGetMIBBandWidthMode(u1VdpId);
    
    //Handle Seamless Res Switch    
    _vDrvDISetResEnCfg(u1VdpId, u4IsSystemRelayoytNeeded);
    
    if((SV_ON==u4IsSeamlessSwithNeeded)||(SV_ON==u4IsSystemRelayoytNeeded))        
    {
        _vDrvDISetResStart(u1VdpId);
        //Change Input Active Data
        _vDrvDISetActiveWindow(u1VdpId, 
            _arMDDiPrm[u1VdpId].u2HClip,_arMDDiPrm[u1VdpId].u2VClip,
            _arMDDiPrm[u1VdpId].u2Width,_arMDDiPrm[u1VdpId].u2Height);
    }

    rMIBState[u1VdpId].u1ModeChg = u4IsSystemRelayoytNeeded;    
    rMIBState[u1VdpId].u1SeamlessSwitch = u4IsSeamlessSwithNeeded;

    return u4IsSystemRelayoytNeeded;
}

/**
 * @brief Set DI Parameters
 * @param u1VdpId VDP_1/VDP_2
 */
static void _vDrvDISetParam(UINT8 u1VdpId)
{
    PATH_CHECK(u1VdpId);

    vDrvDIFrameRepeat(u1VdpId, SV_OFF);

    _u4DrvDICfgColorFmt(u1VdpId);

    vDrvDISetFrameBuf(u1VdpId);

    _vDrvDICfgFlipMirrorParam(u1VdpId);

    _vDrvDISetMisc(u1VdpId);

}

/**
 * @brief Set DI buffer information
 * @param u1VdpId VDP_1/VDP_2
 * @param u4Mode fbm configure mode
 * @param u4Address
 * @param u4Size
 * @param u4Width
 * @param u4Height
 */
static void _vDrvSetDIBufferInfo(UINT8 u1VdpId, UINT32 u4Mode, 
        UINT32 u4Address, UINT32 u4Size, UINT32 u4Width, UINT32 u4Height)
{
    rMIBFbmPool[u1VdpId].u4Addr = u4Address;
    rMIBFbmPool[u1VdpId].u4Size = u4Size;
    rMIBFbmPool[u1VdpId].u4Width = u4Width;
    rMIBFbmPool[u1VdpId].u4Height = u4Height;
    rMIBFbmPool[u1VdpId].fg10bit = ((u4Mode & FBM_POOL_MODE_10BIT) > 0)&&(!_arMDDiPrm[VDP_1].u1IsDualFPI);
    rMIBFbmPool[u1VdpId].fg422 = (u4Mode & FBM_POOL_MODE_422_FB) > 0;
    rMIBFbmPool[u1VdpId].u4Mode = u4Mode;
}

static inline void vMIBStaMon(UINT8 bPath)
{   
    if(SV_VP_MAIN==bPath)
    {
        MON_STA(MIB_M,iHT,MDDI_READ_FLD(bPath,MCVP_KC_0E, IN_DE_HT));
        MON_STA(MIB_M,iVT,MDDI_READ_FLD(bPath,MCVP_KC_0C, IN_VT2));
        MON_STA(MIB_M,iVA,MDDI_READ_FLD(bPath,MCVP_KC_0D, IN_V_ACT));
        MON_STA(MIB_M,iHA,MDDI_READ_FLD(bPath,MCVP_KC_0E, IN_DE_H_ACT));
    }
    else
    {
        MON_STA(MIB_P,iHT,MDDI_READ_FLD(bPath,MCVP_KC_0E, IN_DE_HT));
        MON_STA(MIB_P,iVT,MDDI_READ_FLD(bPath,MCVP_KC_0C, IN_VT2));
        MON_STA(MIB_P,iVA,MDDI_READ_FLD(bPath,MCVP_KC_0D, IN_V_ACT));
        MON_STA(MIB_P,iHA,MDDI_READ_FLD(bPath,MCVP_KC_0E, IN_DE_H_ACT));
    }
}


/**
 * @brief Init DI Clock
 * @param u1VdpId VDP_1/VDP_2
 */
static void _vDrvDIInitClock(UINT8 u1VdpId)
{
    vDrvDISetClock(VDP_1, MIB_CLK_TYPESYSPLL_D2_CK);    
    vDrvDISetClockEnable(u1VdpId, SV_ON);
}

/**
 * @brief MIB Main / Sub configuration Loop
 * @param u1VdpId VDP_1/VDP_2
 */
static void inline _vDrvDIChkImpl(UINT8 u1VdpId)
{ 
    PATH_CHECK(u1VdpId);

#ifndef CC_FPGA_SUPPORT
    if ((IO32ReadFldAlign(ADAPTIVE_PSCAN_REG, ADAPTIVE_PSCAN_MAIN) && (VDP_1 == u1VdpId))
            || (IO32ReadFldAlign(ADAPTIVE_PSCAN_REG, ADAPTIVE_PSCAN_SUB) && (VDP_2 ==u1VdpId)))
#endif
    {
        vMIBStaMon(u1VdpId); 
        
        if (_u1DrvDIModeChange(u1VdpId))
        {                        
            vDrvDISetMuteCtrl(u1VdpId);
            
            if (_arMDDiPrm[u1VdpId].eDebugDataMode != E_MIB_DMODE_UNKNOWN)
            {
                VDP_SetDispmode(u1VdpId, IS_MIB_FRC(u1VdpId)? 3 : 0 );
            }

            vDrvDISetDoubleBuffer(u1VdpId, SV_OFF);

            _vDrvDISetDramCtrl(u1VdpId, E_MIB_DMODE_UNKNOWN);

            vDrvDISetClkReset(u1VdpId, SV_ON); 

            _vDrvDISetParam(u1VdpId);

            _vDrvDISetKernalCtrl(u1VdpId,_arMDDiPrm[u1VdpId].eDataMode);

            _vDrvDISetFmtCtrl(u1VdpId,_arMDDiPrm[u1VdpId].eTDInput,_arMDDiPrm[u1VdpId].eTDOutput);

            vDrvDISetClkReset(u1VdpId, SV_OFF);
             
            vDrvDISetDoubleBuffer(u1VdpId, SV_ON);

        }

        _vDrvDISetDramCtrl(u1VdpId, rMIBState[u1VdpId].eBandwidthDataMode);        
        
        _vDrvDISetResEnd(u1VdpId);

        //vDrvDIChkStatus(u1VdpId);
        
    }
}

/**
 * @brief MIB Sram Sharing Mode configuration
 * @param u1VdpId VDP_1/VDP_2
 */
static inline void _vDrvDISramShareCfg(void)
{
    E_SRAM_SHARE_MODE eSramMode;
    UINT32 u4Width =  MDDI_READ_FLD(SV_VP_MAIN, MCVP_DRAM_09, DA_DRAM_LINE_LENGTH) * MDDI_LINE_LENGHT_UNIT;

    if(((E_TD_IN_SBS_I==_arMDDiPrm[SV_VP_MAIN].eTDInput)
            ||(E_TD_IN_SBS_P==_arMDDiPrm[SV_VP_MAIN].eTDInput))
        &&(E_TD_OUT_NATIVE!=_arMDDiPrm[SV_VP_MAIN].eTDOutput))
    {
        u4Width *=2;
    }
            
#ifndef CC_FPGA_DEBUG
    if(IO32ReadFldAlign(ADAPTIVE_PSCAN_REG, ADAPTIVE_PSCAN_MAIN)&&
            ((SV_TRUE==rMIBState[SV_VP_MAIN].u1ModeChg) || (rMIBState[SV_VP_MAIN].u1SeamlessSwitch == SV_TRUE)))
#endif
    {
        //Dual Engine 3DDI
        if(_arMDDiPrm[SV_VP_MAIN].u1IsDualFPI)
        {
            vIO32WriteFldAlign(NXNR_00             ,SV_ON     , NX_FP_120_MODE);
            vIO32WriteFldAlign(PRE_RE_GEN_MAIN_00  ,SV_ON     , RG_M_EVEN_ONLY);            
            
            vIO32WriteFldAlign(PRE_RE_GEN_PIP_00   ,SV_ON     , RG_P_INPUT_SEL);
            vIO32WriteFldAlign(PRE_RE_GEN_PIP_00   ,SV_ON     , RG_P_ODD_ONLY);            
        }
        else
        {
            vIO32WriteFldAlign(NXNR_00             ,SV_OFF     , NX_FP_120_MODE);
            vIO32WriteFldAlign(PRE_RE_GEN_MAIN_00  ,SV_OFF     , RG_M_EVEN_ONLY);                        
            vIO32WriteFldAlign(PRE_RE_GEN_PIP_00   ,SV_OFF     , RG_P_INPUT_SEL);
            vIO32WriteFldAlign(PRE_RE_GEN_PIP_00   ,SV_OFF     , RG_P_ODD_ONLY);            
        }
    
        //Put Sub in Extra Buffer, But Sub only CBOB Avaliable under this mode
        MDDI_WRITE_FLD(VDP_1, MCVP_BC_00,   (IS_POP(SV_VP_MAIN)) ? 0 : 1, EXTRA_PIP_BUF_EN);

        //DRAM FIFO Control
        MDDI_WRITE_FLD(VDP_1, MCVP_DRAM_08, (IS_POP(SV_VP_MAIN)) ? 1 : 0, DA_PIP_POP);
 
        //Config SRAM Sharing Mode for other model
        if(IS_PR_MODE(SV_VP_MAIN))
        {
            eSramMode=(IS_INTERLACE(SV_VP_MAIN))?E_SRAM_SHARE_HD_I_3D : E_SRAM_SHARE_HD_P_3D;
        }
        else if(_arMDDiPrm[SV_VP_MAIN].u1IsDualFPI)
        {
            eSramMode=E_SRAM_SHARE_SD_I;
        }
        else if((u4Width < 960)|| bDrvVideoGetWidthOverSample(SV_VP_MAIN)) 
        {
            if(IS_INTERLACE(SV_VP_MAIN))
            {
                //SD Interlace
                eSramMode= IS_DQ(SV_VP_MAIN) ? E_SRAM_SHARE_HD_I_HALF_DQ
                 : IS_OVERSAMPLE(SV_VP_MAIN) ? E_SRAM_SHARE_SD_I_OS : E_SRAM_SHARE_SD_I;
            }
            else
            {
                //SD Progressive
                eSramMode= IS_DQ(SV_VP_MAIN) ? E_SRAM_SHARE_HD_P_FULL_DQ
                : IS_OVERSAMPLE(SV_VP_MAIN) ? E_SRAM_SHARE_SD_P_OS : E_SRAM_SHARE_SD_P;
            }
        }
        else 
        {
            if(IS_INTERLACE(SV_VP_MAIN))
            {
                //HD Interlace
                eSramMode= ((IS_POP(SV_VP_MAIN)) ? E_SRAM_SHARE_HD_I_HALF_DQ : E_SRAM_SHARE_HD_I);
            }
            else
            {
                //HD Progressive
                eSramMode= IS_DQ(SV_VP_MAIN) ? E_SRAM_SHARE_HD_P_FULL_DQ : E_SRAM_SAHRE_HD_P;
            }
        }
                
        MDDI_WRITE_FLD(VDP_1, MCVP_KC_19, eSramMode, SRAM_MODE);

    }    
}

/**
 * @brief PSCAN SRM callback function
 * @param u4Arg1 VDP_1/VDP_2
 * @param u4Arg2 configuration
 * @param u4SramMode sram mode
 */
void vDrvDISrmCb(UINT32 u4Arg1, UINT32 u4Arg2, UINT32 u4PDSize, UINT32 u4TDTV)
{
    FBM_POOL_T* prPool;
    E_TD_IN TdIn    =TD_MIB_IN(u4TDTV);
    E_TD_OUT TdOut  =TD_MIB_OUT(u4TDTV);

    //2D output mode, force MIB engine work under 2D mode
    if((E_TD_OUT_NATIVE==TdOut)
        &&(E_TD_IN_FS_I!=TdIn)
        &&(E_TD_IN_FS_P!=TdIn)
        &&(E_TD_IN_LI_I!=TdIn)
        &&(E_TD_IN_LI_P!=TdIn)
        )
    {
        TdIn=bDrvVideoIsSrcInterlace(u4Arg1) ? E_TD_IN_2D_I : E_TD_IN_2D_P;
    }

    //Translate Input / Output
    if(E_TD_OUT_NATIVE==TdOut)
    {
        switch(TdIn)
        {
            case E_TD_IN_FS_I:           
            case E_TD_IN_FS_P:            
                TdOut=E_TD_OUT_3D_FS;
                break;            
            case E_TD_IN_SBS_I:
            case E_TD_IN_SBS_P:            
                TdOut=E_TD_OUT_3D_SBS;
                break;            
            case E_TD_IN_TB_I:
            case E_TD_IN_TB_P:            
                TdOut=E_TD_OUT_3D_TB;
                break;
            case E_TD_IN_LI_I:
            case E_TD_IN_LI_P:            
                TdOut=E_TD_OUT_3D_LI;
                break;                
            default:
                TdOut=E_TD_OUT_NATIVE;
        }
    }

    if (u4Arg1 == VDP_1)
    {
#ifndef CC_FPGA_SUPPORT
        prPool = FBM_GetPoolInfoAuto((UCHAR)FBM_POOL_TYPE_MDDI_MAIN, NULL);
#endif

        r3DInOutFormat[VDP_1].eTDInput  = TdIn;
        r3DInOutFormat[VDP_1].eTDOutput = TdOut;

        if(u4DrvTDTVIsDualFPI())
        {
            r3DInOutFormat[VDP_2].eTDInput = r3DInOutFormat[VDP_1].eTDInput;
            r3DInOutFormat[VDP_2].eTDOutput = r3DInOutFormat[VDP_1].eTDOutput;

            r3DInOutFormat[VDP_1].u1IsDualFPI=SV_ON;
            r3DInOutFormat[VDP_2].u1IsDualFPI=SV_ON;            
        }
        else
        {
            r3DInOutFormat[VDP_1].u1IsDualFPI=SV_OFF;
            r3DInOutFormat[VDP_2].u1IsDualFPI=SV_OFF;            
        }
    }
    else
    {
#ifndef CC_FPGA_SUPPORT
        prPool = FBM_GetPoolInfoAuto((UCHAR)FBM_POOL_TYPE_MDDI_PIP, NULL);
#endif
        if(u4DrvTDTVIsDualFPI())
        {
            r3DInOutFormat[VDP_2].eTDInput = r3DInOutFormat[VDP_1].eTDInput;
            r3DInOutFormat[VDP_2].eTDOutput = r3DInOutFormat[VDP_1].eTDOutput;

            r3DInOutFormat[VDP_1].u1IsDualFPI=SV_ON;
            r3DInOutFormat[VDP_2].u1IsDualFPI=SV_ON;            
            
        }
        else
        {
            r3DInOutFormat[u4Arg1].eTDInput  = E_TD_IN_2D_I;
            r3DInOutFormat[u4Arg1].eTDOutput = E_TD_OUT_NATIVE;        

            r3DInOutFormat[VDP_1].u1IsDualFPI=SV_OFF;
            r3DInOutFormat[VDP_2].u1IsDualFPI=SV_OFF;            
            
        }
    }
    
#ifndef CC_FPGA_SUPPORT
    if(prPool != NULL)
    {
        //Lock the DRAM Write to prevent DRAM Overlap    
        if(u4DrvTDTVIsDualFPI())
        {
            if(VDP_1==u4Arg1)
            {
                if(prPool->u4Addr >  (MDDI_READ_FLD(VDP_1, MCVP_DRAM_11, DA_WADDR_LO_LIMIT)<<11))
                {
                    MDDI_WRITE_FLD(VDP_1, MCVP_DRAM_11, prPool->u4Addr>>11, DA_WADDR_LO_LIMIT);
                }
                
                if(prPool->u4Addr >  (MDDI_READ_FLD(VDP_1, MCVP_DRAM_06, DA_FLAG_WADDR_LO_LIMIT)<<4))
                {
                    MDDI_WRITE_FLD(VDP_1, MCVP_DRAM_06, prPool->u4Addr>>4, DA_FLAG_WADDR_LO_LIMIT);
                }            
                
                if((prPool->u4Addr + prPool->u4Size) >  MDDI_READ_FLD(VDP_1, MCVP_DRAM_10, DA_WADDR_HI_LIMIT)<<11)
                {
                    MDDI_WRITE_FLD(VDP_1, MCVP_DRAM_10, (prPool->u4Addr + prPool->u4Size)>>11, DA_WADDR_HI_LIMIT);
                }
                
                if((prPool->u4Addr + prPool->u4Size) >  MDDI_READ_FLD(VDP_1, MCVP_DRAM_05, DA_FLAG_WADDR_HI_LIMIT)<<4)
                {
                    MDDI_WRITE_FLD(VDP_1, MCVP_DRAM_05, (prPool->u4Addr + prPool->u4Size)>>4, DA_FLAG_WADDR_HI_LIMIT);
                }
                
                if(prPool->u4Addr >  (MDDI_READ_FLD(VDP_2, MCVP_DRAM_11, DA_WADDR_LO_LIMIT)<<11))
                {
                    MDDI_WRITE_FLD(VDP_2, MCVP_DRAM_11, prPool->u4Addr>>11, DA_WADDR_LO_LIMIT);
                }
                
                if(prPool->u4Addr >  (MDDI_READ_FLD(VDP_2, MCVP_DRAM_06, DA_FLAG_WADDR_LO_LIMIT)<<4))
                {
                    MDDI_WRITE_FLD(VDP_2, MCVP_DRAM_06, prPool->u4Addr>>4, DA_FLAG_WADDR_LO_LIMIT);
                }            
                
                if((prPool->u4Addr + prPool->u4Size) >  MDDI_READ_FLD(VDP_2, MCVP_DRAM_10, DA_WADDR_HI_LIMIT)<<11)
                {
                    MDDI_WRITE_FLD(VDP_2, MCVP_DRAM_10, (prPool->u4Addr + prPool->u4Size)>>11, DA_WADDR_HI_LIMIT);
                }
                
                if((prPool->u4Addr + prPool->u4Size) >  MDDI_READ_FLD(VDP_2, MCVP_DRAM_05, DA_FLAG_WADDR_HI_LIMIT)<<4)
                {
                    MDDI_WRITE_FLD(VDP_2, MCVP_DRAM_05, (prPool->u4Addr + prPool->u4Size)>>4, DA_FLAG_WADDR_HI_LIMIT);
                }        
            }
        }
        else
        {    
            if(prPool->u4Addr >  (MDDI_READ_FLD(u4Arg1, MCVP_DRAM_11, DA_WADDR_LO_LIMIT)<<11))
            {
                MDDI_WRITE_FLD(u4Arg1, MCVP_DRAM_11, prPool->u4Addr>>11, DA_WADDR_LO_LIMIT);
            }

            if(prPool->u4Addr >  (MDDI_READ_FLD(u4Arg1, MCVP_DRAM_06, DA_FLAG_WADDR_LO_LIMIT)<<4))
            {
                MDDI_WRITE_FLD(u4Arg1, MCVP_DRAM_06, prPool->u4Addr>>4, DA_FLAG_WADDR_LO_LIMIT);
            }            

            if((prPool->u4Addr + prPool->u4Size) >  MDDI_READ_FLD(u4Arg1, MCVP_DRAM_10, DA_WADDR_HI_LIMIT)<<11)
            {
                MDDI_WRITE_FLD(u4Arg1, MCVP_DRAM_10, (prPool->u4Addr + prPool->u4Size)>>11, DA_WADDR_HI_LIMIT);
            }
            
            if((prPool->u4Addr + prPool->u4Size) >  MDDI_READ_FLD(u4Arg1, MCVP_DRAM_05, DA_FLAG_WADDR_HI_LIMIT)<<4)
            {
                MDDI_WRITE_FLD(u4Arg1, MCVP_DRAM_05, (prPool->u4Addr + prPool->u4Size)>>4, DA_FLAG_WADDR_HI_LIMIT);
            }        
        }

        //update FBM Info
        _vDrvSetDIBufferInfo((UINT8)u4Arg1, u4Arg2, prPool->u4Addr, prPool->u4Size, prPool->u4Width, prPool->u4Height);
    }
    else
    {
        LOG(1, "vDrvDISrmCb: Fail to get pool\n");
    }
#endif
}

/**
 * @brief DI mode change done
 * @param u1VdpId VDP_1/VDP_2
 */
void vDrvDIModeChangeDone(UINT8 u1VdpId)
{
    // NR workaround for 420 SD progressive bottom line chroma NR issue
    // vDrvNRSet3DNRDemo(SV_OFF, SV_LEFT, _arMDDiPrm[VDP_1].u2Width);
}


/**
 * @brief Initialize PSCAN register values
 * @param u1VdpId VDP_1/VDP_2
 */
void vDrvDISwInit(UINT8 u1VdpId)
{
    //Initialize PSCAN structure
    _arMDDiPrm[u1VdpId].u1OnOff = SV_OFF;
    _arMDDiPrm[u1VdpId].u2Width = 0xFFFF;
    _arMDDiPrm[u1VdpId].u2Height = 0xFFFF;

    _arMDDiPrm[u1VdpId].eDebugDataMode = E_MIB_DMODE_UNKNOWN;
    _arMDDiPrm[u1VdpId].u1DebugNRDataMode = E_MIB_NR_UNKNOWN;
    _arMDDiPrm[u1VdpId].u1DebugFrcMode = E_MIB_FRC_UNKNOWN;
    _arMDDiPrm[u1VdpId].u1DebugFlipMorrorByMIB = SV_OFF;    
    _arMDDiPrm[u1VdpId].u2HClip = 0xFFFF;
    _arMDDiPrm[u1VdpId].u2VClip = 0xFFFF;
    _arMDDiPrm[u1VdpId].u1ForceDIBypass = SV_OFF;
#ifdef CC_SUPPORT_VENC    
    _arMDDiPrm[u1VdpId].u1VencOn = SV_FALSE;
#endif

    rMIBState[u1VdpId].u1StableCnt = 0;
    rMIBState[u1VdpId].bAllowedAutoStop= SV_ON;    
    rMIBState[u1VdpId].u4SetResNewKDE = 0;

}


/**
 * @brief Initialize PSCAN register values
 * @param u1VdpId VDP_1/VDP_2
 */
void vDrvDIHwInit(UINT8 u1VdpId)
{
    _vDrvDIInitClock(u1VdpId); 

    vDrvDISetDoubleBuffer(u1VdpId, SV_OFF);

    _vDrvDISetDramCtrl(u1VdpId, E_MIB_DMODE_UNKNOWN);

    vDrvDISetClkReset(u1VdpId, SV_ON);

    _vDrvDIFlipMirrorInit();

    vDrvDIInitQuality(u1VdpId);

    vDrvDISetAutoStop(u1VdpId, SV_OFF);

    if(u1VdpId == VDP_1)
    {
        _vDrvDIQualityCreateThread();
    }
#ifndef CC_FPGA_SUPPORT
    _vDrvDIInitISR();

    vDrvDIEnableISR(u1VdpId, SV_ON);
#endif
    vDrvDISetClkReset(u1VdpId, SV_OFF);

    MDDI_WRITE_FLD(u1VdpId, MCVP_KC_00, SV_ON, FLD_OUT_EN);
    MDDI_WRITE_FLD(u1VdpId, MCVP_KC_17, SV_ON, AUTO_TNR_OFF_EN);
    MDDI_WRITE_FLD(u1VdpId, MCVP_KC_17, SV_ON, AUTO_BOB_EN);
    MDDI_WRITE_FLD(u1VdpId, MCVP_KC_09, SV_OFF, HBLANK_EXT_EN);     
    MDDI_WRITE_FLD(u1VdpId, MCVP_KC_33, SV_ON, I2P_HT_DIV2_SEL);
    MDDI_WRITE_FLD(u1VdpId, MCVP_KC_0B, 100,  IN_ADJ_INT_LINE);

    MDDI_WRITE_FLD(u1VdpId, MCVP_DRAM_09, 0x2, DA_READ_REQ_SPACING);
    MDDI_WRITE_FLD(u1VdpId, MCVP_DRAM_18, 1, DA_SC_READ_REQ_SPACING);
    //MDDI_WRITE_FLD(u1VdpId, MCVP_DRAM_1A, 1, DA_SC_SHORT_BURST_MODE);
    //MDDI_WRITE_FLD(u1VdpId, MCVP_DRAM_08, 1, DA_RD_SHORT_BURST_MODE);   

    MDDI_WRITE_FLD(u1VdpId, MCVP_KC_33, MIB_PREFETCH_KDELOWMIN-1, FAST_K_DE_LOW_MIN); 
	
    vIO32WriteFldAlign(PSCAN_MISC_00, 0xFF, BANDWIDTH_PER_PXL_1); 
    vIO32WriteFldAlign(PSCAN_MISC_00, 0xFF, BANDWIDTH_PER_PXL_2);   

}

/**
 * @brief MIB main loop
 * @param u1VdpId VDP_1/VDP_2
 */
void vDrvMIBProc()
{
    //Ctrl main loop
    _vDrvDIChkImpl(VDP_1);
    _vDrvDIChkImpl(VDP_2);    
    _vDrvDISramShareCfg(); 
	
    //Quality main loop
#ifndef __MODEL_slt__
    vDrvDIQtyMainLoop(VDP_1);
    vDrvDIQtyMainLoop(VDP_2);    
#endif
}

#ifdef CC_SUPPORT_STR
//-----------------------------------------------------
//
// MIB ( DI/NR ) 
// added for MIB suspend/resume  function
//-----------------------------------------------------
#define  vDrvMibReset(x)         (vIO32WriteFldAlign(CKGEN_BLOCK_RST_CFG1, x, FLD_MIB_RST))
#define  vDrvMibSetClock(x)      (vIO32WriteFldAlign(CKGEN_BLOCK_CKEN_CFG1, x, FLD_MIB_CKEN))

void vDrvMIBSuspend(void)
{
    _vDrvDISetDramCtrl(SV_VP_MAIN, E_MIB_DMODE_UNKNOWN);    
    vDrvDISetClkReset(SV_VP_MAIN, SV_ON); 
#if SUPPORT_POP
    _vDrvDISetDramCtrl(SV_VP_PIP, E_MIB_DMODE_UNKNOWN);    
    vDrvDISetClkReset(SV_VP_PIP, SV_ON); 
#endif
    vDrvMibSetClock(SV_OFF);
}

void vDrvMIBResume(void)
{
    vDrvMibSetClock(SV_ON);
  //  vDrvMibReset(SV_ON);    
  //  vDrvMibReset(SV_OFF);   
    vDrvDIHwInit(SV_VP_MAIN);
    vDrvDISwInit(SV_VP_MAIN);
    vDrvDIHwInit(SV_VP_PIP);
    vDrvDISwInit(SV_VP_PIP);
}
#endif


