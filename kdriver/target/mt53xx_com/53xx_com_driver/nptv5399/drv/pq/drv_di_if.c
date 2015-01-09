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
 * $RCSfile: drv_di_cli.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

#include "hw_di.h"

#include "drv_di.h"
#include "drv_di_int.h"
#include "drv_nr.h"
#include "drv_scaler.h"
#include "drv_scpos.h"
#include "di_debug.h"

#include "eeprom_if.h"
#include "hw_ckgen.h"
#include "hw_ycproc.h"
#include "hw_scpos.h"
#include "hw_sys.h"
#include "vdo_misc.h"

#include "drv_tdtv.h"

#include "drv_scaler_drvif.h"
#include "fbm_drvif.h"
#include "srm_drvif.h"
#include "drv_display.h"
#include "mute_if.h"
#include "x_hal_5381.h"

EXTERN void vDrvDISetAutoStop(UINT8 u1VdpId, UINT8 u1OnOff);

/**
* @brief Set DI Debug Mode
* @param u1VdpId VDP_1/VDP_2
* @param bMode DI Mode
*/
void vDrvDISetDebugDataMode(UINT8 u1VdpId, E_MIB_DMODE bMode)
{
    _arMDDiPrm[u1VdpId].eDebugDataMode = bMode;
    
    if (bMode == E_MIB_DMODE_UNKNOWN) // reset data mode to follow srm setting
    {
        _arMDDiPrm[u1VdpId].u1DebugFrcMode = E_MIB_FRC_UNKNOWN;
    }
    else
    {
        _arMDDiPrm[u1VdpId].u1DebugFrcMode = (E_MIB_FRC_DMODE)((UINT8)bMode&0xF);
    }
}

/**
* @brief Disable display mode
* @param u1VdpId VDP_1/VDP_2
*/
void vDrvDISetDispmodeOff(UINT8 u1VdpId)
{
    LOG(0,"MT5396 / 68 New Flow Not Support Direct Setting FRC mode , Please Use Data Mode");
}

/**
* @brief Set DI Debug Display Mode
* @param u1VdpId VDP_1/VDP_2
* @param u1OnOff On/Off
*/
void vDrvDISetDebugPSCANDisplay(UINT8 u1VdpId, UINT8 u1OnOff)
{
    LOG(0,"MT5396 / 68 New Flow Not Support Direct Setting FRC mode , Please Use Data Mode");
    return;
}

/**
* @brief Get DI Debug Display Mode
* @param u1VdpId VDP_1/VDP_2
* @return DI Debug Display Mode
*/
UINT8 u1DrvDIGetDebugPSCANDisplay(UINT8 u1VdpId)
{
    return _arMDDiPrm[u1VdpId].u1DebugFrcMode;
}

/**
* @brief DI FW On/Off
* @param u1VdpId VDP_1/VDP_2
* @param u1OnOff On/Off
*/
void vDrvDIFWOnOff(UINT8 u1Item, UINT8 u1OnOff)
{
    switch (u1Item)
    {
        case 0:
            vIO32WriteFldAlign(ADAPTIVE_PSCAN_REG, u1OnOff, ADAPTIVE_PSCAN_MAIN);
            Printf("PSCAN main Turn %s FW control \n", u1OnOff? "on" : "off");
            break;
        case 1:
            vIO32WriteFldAlign(ADAPTIVE_PSCAN_REG, u1OnOff, ADAPTIVE_PSCAN_SUB);
            Printf("PSCAN sub Turn %s FW control \n", u1OnOff ? "on": "off");
            break;
        case 2:
            vIO32WriteFldAlign(ADAPTIVE_PSCAN_REG, u1OnOff, ADAPTIVE_PSCAN_QTY);
            Printf("PSCAN Turn %s FW quality \n", u1OnOff ? "on" : "off");
            break;
        default:
            Printf("Unknown item %d ", u1Item);
            break;
    }
}

/**
* @brief Translate data mode to string for log
* @param eMode DI Data Mode
*/
const CHAR* szDrvDataModeToString(UINT8 u1VdpId)
{
    if (u1VdpId >= VDP_NS)
    {
        u1VdpId = VDP_2;
    }    
    return aDataMode[rMIBState[u1VdpId].u1DataModeIdx].szModeName;
}

/**
* @brief Translate data mode to string for log
* @param eMode DI Data Mode
*/
const CHAR* szDrvBWDataModeToString(UINT8 u1VdpId)
{
    if (u1VdpId >= VDP_NS)
    {
        u1VdpId = VDP_2;
    }
    return aDataMode[rMIBState[u1VdpId].u1BandwidthDataModeIdx].szModeName;
}

/**
* @brief Get DI Status info
* @param u1VdpId VDP_1/VDP_2
* @param u1DbgIdx debug level
*/
void vDrvDIGetInfo(UINT8 u1VdpId, UINT8 u1DbgIdx)
{
    #define MIB_ELE_LOG(cotainer,element) LOG(0,"%28s :%8x\n",#element,cotainer.element);

    switch (u1DbgIdx)
    {
        case 0:

            LOG(0,"==MIB SYSTEM Static State==\n");            
            #undef MIB_ELE_CMD
            #define MIB_ELE_CMD(type,element) MIB_ELE_LOG(_arMDDiPrm[u1VdpId],element)
            do{ MIB_SYS_STATIC_ELE_LIST } while(0);

            LOG(0,"==MIB FBM State==\n");            
            #undef MIB_ELE_CMD
            #define MIB_ELE_CMD(type,element) MIB_ELE_LOG(rMIBFbmPool[u1VdpId],element)
            do{ MIB_FBM_ELE_LIST } while(0);                      

            #undef MIB_ELE_CMD            
            #define MIB_ELE_CMD(type,element) MIB_ELE_LOG(rMIBState[u1VdpId],element)
            LOG(0,"==MIB SYSTEM Dynamic State==\n");
            do{ MIB_SYS_DYN_ELE_LIST } while(0);                            

            break;
                        
        case 1:
            Printf("\nPSCAN ==MIB SYSTEM Static State==\n");
            Printf("PSCAN %s Debug Information\n", u1VdpId ? "Sub" : "Main");
            Printf("Data Mode               %s\n", szDrvDataModeToString(u1VdpId));  
            Printf("u1onoff                 %s\n",  _arMDDiPrm[u1VdpId].u1OnOff ? "on" : "off");
            Printf("Bit Res                 %s\n", (_arMDDiPrm[u1VdpId].u1BitRes == 10) ? "10 bit" : "8 bit");
            Printf("DecType                 %d\n\n", _arMDDiPrm[u1VdpId].u1DecType);
            Printf("Source                  %s\n",  _arMDDiPrm[u1VdpId].u1Interlace ? "Interlace" : "Progressive");
            Printf("Color Mode              %s\n", (_arMDDiPrm[u1VdpId].u1420Mode) ? "420" : "422");
            Printf("Force Color Mode        %s\n", (_arMDDiPrm[u1VdpId].u1Force420Mode) ? "420" : "None");
            Printf("u1IsDQ                  %d\n", _arMDDiPrm[u1VdpId].u1IsDQ);
            Printf("u1IsOversample          %d\n", _arMDDiPrm[u1VdpId].u1IsOversample);
            Printf("u1IsPOP                 %s\n", (_arMDDiPrm[u1VdpId].u1IsPOP)?"POP":"Not POP");
            Printf("u1IsDualFPI             %d\n", _arMDDiPrm[u1VdpId].u1IsDualFPI);
            Printf("FlipMorrorByMIB         %d\n", _arMDDiPrm[u1VdpId].u1FlipMorrorByMIB);
            Printf("u1IsNSTDSignal          %d\n", _arMDDiPrm[u1VdpId].u1IsNSTDSignal);
            Printf("HTotal                  %d\n", _arMDDiPrm[u1VdpId].u2HTotal);
            Printf("VTotal                  %d\n", _arMDDiPrm[u1VdpId].u2VTotal);
            Printf("Width                   %d\n", _arMDDiPrm[u1VdpId].u2Width);
            Printf("Height                  %d\n", _arMDDiPrm[u1VdpId].u2Height);
            Printf("HClip                   %d\n", _arMDDiPrm[u1VdpId].u2HClip);
            Printf("VClip                   %d\n", _arMDDiPrm[u1VdpId].u2VClip);
            Printf("Input soure Width       %d\n", _arMDDiPrm[u1VdpId].u2SigWidth);
            Printf("Input soure Height      %d\n", _arMDDiPrm[u1VdpId].u2SigHeight);
            Printf("u4Bufaddress            0x%x\n", _arMDDiPrm[u1VdpId].u4BufAddr);
            Printf("u2BufWidth              %d\n", _arMDDiPrm[u1VdpId].u2BufWidth);
            Printf("u2BufHeight             %d\n", _arMDDiPrm[u1VdpId].u2BufHeight);
            Printf("u2BufSize               0x%x\n", _arMDDiPrm[u1VdpId].u4BufSize);
            Printf("u1InFR                  %d\n", _arMDDiPrm[u1VdpId].u1InFR);
            Printf("u4OutVT                 %d\n", _arMDDiPrm[u1VdpId].u4OutVT);
            Printf("u4OutHT                 %d\n", _arMDDiPrm[u1VdpId].u4OutHT);
            Printf("u4OutHeight             %d\n", _arMDDiPrm[u1VdpId].u4OutHeight);
            Printf("u1OutFR                 %d\n", _arMDDiPrm[u1VdpId].u1OutFR);
            Printf("3D Input                %d\n", _arMDDiPrm[u1VdpId].eTDInput);
            Printf("3D Outout               %d\n", _arMDDiPrm[u1VdpId].eTDOutput);
            Printf("u1FrcMode               %d\n", _arMDDiPrm[u1VdpId].u1FrcMode);
            Printf("eDebugDataMode          0x%x\n", _arMDDiPrm[u1VdpId].eDebugDataMode);
            Printf("u1DebugNRDataMode       %d\n", _arMDDiPrm[u1VdpId].u1DebugNRDataMode);
            Printf("u1DebugFrcMode          0x%x\n", _arMDDiPrm[u1VdpId].u1DebugFrcMode);
            Printf("u1DebugFlipMorrorByMIB  %d\n", _arMDDiPrm[u1VdpId].u1DebugFlipMorrorByMIB);
            Printf("u1VencOn                %d\n", _arMDDiPrm[u1VdpId].u1VencOn);
      
            Printf("\n===FBM Infromation====\n");
            Printf("fg10bit                 %s\n", (rMIBFbmPool[u1VdpId].fg10bit) ? "10 bit" : "8 bit");
            Printf("fbm Color Mode          %s\n", (rMIBFbmPool[u1VdpId].fg422) ? "420" : "422");
            Printf("u4Addr                  0x%x\n", rMIBFbmPool[u1VdpId].u4Addr);
            Printf("u4Size                  0x%x\n", rMIBFbmPool[u1VdpId].u4Size);
            Printf("u4Width                 %d\n", rMIBFbmPool[u1VdpId].u4Width);
            Printf("u4Height                %d\n", rMIBFbmPool[u1VdpId].u4Height);
            Printf("u4Mode                  0x%x\n", rMIBFbmPool[u1VdpId].u4Mode);

            Printf("\n==MIB SYSTEM Dynamic State==\n");
            Printf("u1AToCFldCnt                %d\n", rMIBState[u1VdpId].u1AToCFldCnt);
            Printf("u2Vcnt                    %d\n", rMIBState[u1VdpId].u2Vcnt);
            Printf("bAllowedAutoStop          %d\n", rMIBState[u1VdpId].bAllowedAutoStop);
            Printf("u1DataModeIdx             %d\n", rMIBState[u1VdpId].u1DataModeIdx);
            Printf("eBandwidthDataMode        0x%x\n", rMIBState[u1VdpId].eBandwidthDataMode);
            Printf("u1ModeChg                 %d\n", rMIBState[u1VdpId].u1ModeChg);
            Printf("u4SetResNewKDE            %d\n", rMIBState[u1VdpId].u4SetResNewKDE);
            Printf("u1DynRes                  %d\n", rMIBState[u1VdpId].u1DynRes);
            Printf("u1SetResFinishedCnt       %d\n", rMIBState[u1VdpId].u1SetResFinishedCnt);
            Printf("u1StableCnt               %d\n", rMIBState[u1VdpId].u1StableCnt);
                        
            LOG(0,"Pixel Overflow  Cnt               %d\n", u4FlagStaCnt[u1VdpId][E_STA_PXL_OVERFLOW]);
            LOG(0,"Pixel Underflow Cnt               %d\n", u4FlagStaCnt[u1VdpId][E_STA_PXL_UNDERFLOW]);
            LOG(0,"Flag  Overflow  Cnt               %d\n", u4FlagStaCnt[u1VdpId][E_STA_FLG_OVERFLOW]);
            LOG(0,"Flag  Underflow Cnt               %d\n", u4FlagStaCnt[u1VdpId][E_STA_FLG_UNDERFLOW]);
            LOG(0,"SC    Underflow Cnt               %d\n", u4FlagStaCnt[u1VdpId][E_STA_SC_UNDERFLOW]);
            LOG(0,"NR    Overflow Cnt               %d\n", u4FlagStaCnt[u1VdpId][E_STA_NR_OVERFLOW]);
            break;
        
        default:
            break;
    }
}

/**
* @brief Set DI OSD
* @param u1Item OSD type
* @param u1OnOff On/Off
*/
void vDrvDISetOSD(UINT8 u1Item, UINT8 u1OnOff)
{
    switch (u1Item)
    {
    	case 0: // Probe
            vIO32WriteFld(MCVP_KC_21, (u1OnOff ? 6 : 0), OSD_MODE);
            vIO32WriteFldMulti(MCVP_KC_21,	P_Fld(5, OSD_Y_POS) | P_Fld(3, OSD_X_POS));
            break;
    	case 1: // Film
            vIO32WriteFld(MCVP_KC_21, (u1OnOff ? 1 : 0), OSD_MODE);
            vIO32WriteFldMulti(MCVP_KC_21,	P_Fld(5, OSD_Y_POS) | P_Fld(3, OSD_X_POS));
            break;
    	case 2: // CS
            vIO32WriteFld(MCVP_KC_21, (u1OnOff ? 3 : 0), OSD_MODE);
            vIO32WriteFldMulti(MCVP_KC_21,	P_Fld(5, OSD_Y_POS) | P_Fld(3, OSD_X_POS));
            break;
    	case 3: // MEMA
            vIO32WriteFld(MCVP_KC_21, (u1OnOff ? 4 : 0), OSD_MODE);
            vIO32WriteFldMulti(MCVP_KC_21,	P_Fld(5, OSD_Y_POS) | P_Fld(3, OSD_X_POS));
            break;
    	case 4: // KERNEL
            vIO32WriteFld(MCVP_KC_21, (u1OnOff ? 5 : 0), OSD_MODE);
            vIO32WriteFldMulti(MCVP_KC_21,	P_Fld(5, OSD_Y_POS) | P_Fld(3, OSD_X_POS));
            break;
    	case 5: // Input Border
            vIO32WriteFld(MCVP_KC_04, u1OnOff, IPTG_BORDER_EN);
            break;
    	case 6: // Output Border
            vIO32WriteFld(MCVP_KC_1E, u1OnOff, BORDER_EN);
            vIO32WriteFld(MCVP_BC_06, u1OnOff, FINAL_BORDER_EN);
            break;
    	default:
            break;
    }
}

/**
* @brief Get DI hblank
* @param u1VdpId VDP_1/VDP_2
* @return Hblank
*/
UINT32 u4DrvDIGetHBlank(UINT8 u1VdpId)
{
    return MDDI_READ_FLD(u1VdpId, MCVP_KC_2B, I2P_H_BLANK);
}

/**
* @brief Get DI main width
* @return DI main width
*/
UINT16 u2DrvDIGetWidth(void)
{
    return MDDI_READ_FLD(SV_VP_MAIN, MCVP_KC_0A, HDEW);
}


/**
 * @brief Return the minimum horizontal width that PSCAN can support
 * @param u1VdpId VDP_1/VDP_2
 * @return u4MiniInputWidth The minimum horizontal width that PSCAN can support
 */
UINT32 u4DrvDIMiniInputWidth(UINT8 u1VdpId)
{
    return 64;
}

/**
 * @brief Get status of DA_DEBUG
 * @param u4Index
 * @return status
 */
UINT32 vDrvDIGetDADebugStatus(UINT32 u4Index)
{
    switch (u4Index)
    {
        case 0:
            return rMIBState[VDP_1].u4DADebugStatus0;
        case 1:
            return rMIBState[VDP_1].u4DADebugStatus1;
        case 2:
            return rMIBState[VDP_1].u4DADebugStatus2;
        case 3:
            return rMIBState[VDP_1].u4DADebugStatus3;
        default:
            LOG(0, "vDrvDIGetDADebugStatus index error");
            return 0xFFFFFFFF;
    }
}

/**
 * @brief Return the maximum horizontal width that PSCAN can support
 * @param u1VdpId VDP_1/VDP_2
 * @return u4MaxInputWidth The maximum horizontal width that PSCAN can support
 */
UINT32 u4DrvDIMaxInputWidth(UINT8 u1VdpId)
{
    UINT32 u4MaxInputWidth;
    SRM_TV_MODE_T eMode = SRM_GetTvMode();

    if (eMode == SRM_TV_MODE_TYPE_POP)
    {
        u4MaxInputWidth = 960;
    }
    else if ((eMode == SRM_TV_MODE_TYPE_PIP) && (u1VdpId == VDP_2))
    {
        #ifdef CC_MT5882
        if(IS_IC_5882_ES1())
        {
            u4MaxInputWidth = 720;
        }
        else
        {
            u4MaxInputWidth = 960;
        }
        #else
    	u4MaxInputWidth = 720;
    	#endif
    }
    else
    {
    	u4MaxInputWidth = 1920;
    }

    return u4MaxInputWidth;
}

/**
 * @brief Return the maximum output width cause by the clock rate limitation
 * @param u1VdpId VDP_1/VDP_2
 * @return the maximum output width cause by the clock rate limitation
 */
UINT32 u4DrvDIMaxOutWidth(UINT8 u1VdpId,UINT32 u4InputVActive,UINT32 u4DispVActive,UINT32 u4DispVTotal,UINT32 u4OutFR)
{
    UINT32 u4MaxOutputWidth=1920;

    #define INTERLACE_CLK_TOLERANCE 0x40
    #define LI_INPUT_UPPER_BOUND 960

    if(IS_MIB_FRC(u1VdpId)&&(0!=u4DispVTotal)&&(0!=u4OutFR)&&(0!=u4InputVActive))
    {
        u4MaxOutputWidth = 
            u4DrvDIGetClkRate(u1VdpId) / u4DispVTotal * u4DispVActive  / u4InputVActive / u4OutFR - MIB_DISP_HBLANK_MIN - INTERLACE_CLK_TOLERANCE;
    }

    if((!u4DrvTDTVIsDualFPI())&& E_TD_IN_FS_I==TD_MIB_IN(u4DrvTDTVDIModeQuery()))
    {
        u4MaxOutputWidth = MIN(u4MaxOutputWidth,960);
    }

#ifdef MIB_LI_INPUT_WA
    if(E_TD_IN_LI_P==TD_MIB_IN(u4DrvTDTVDIModeQuery()))
    {
        u4MaxOutputWidth = MIN(u4MaxOutputWidth,LI_INPUT_UPPER_BOUND);
    }
#endif
    
    return (u4MaxOutputWidth&(~(0x1)));
}

/**
* @brief Get PSCAN delay
* @param u1VdpId VDP_1/VDP_2
* @return PSCAN delay value
*/
UINT8 u1DrvDIGetFrameDelay10x(UINT8 u1VdpId)
{   
    UINT32 u4Mode = rMIBFbmPool[u1VdpId].u4Mode;
    UINT16 u2Delay = 0; 

    if ((u4Mode & FBM_POOL_MODE_MDDI_FULL) || (u4Mode & FBM_POOL_MODE_MDDI_CBOB))
    {
        u2Delay += 20;
    }

    if (u4Mode & FBM_POOL_MODE_MDDI_DISP)
    {
        u2Delay += 5;
    }

#ifdef CC_FLIP_MIRROR_SUPPORT
    if (u4GetFlipMirrorModule(u1VdpId) == FLIP_BY_PSCAN)
    {
        u2Delay += 10;
    }
#endif
    return u2Delay;
}

/**
* @brief Get input/output sync distance
* @param u1VdpId VDP_1/VDP_2
*/
UINT16 u2DrvDIGetVcnt(UINT8 u1VdpId)
{
    return rMIBState[u1VdpId].u2Vcnt;
}

/**
 * @brief Set NR Data mode
 * @param u1VdpId VDP_1/VDP_2
 * @param u1DataMode datamode
 */
void vDrvDISetNRDebugDataMode(UINT8 u1VdpId, E_MIB_NR_DMODE u1DataMode)
{
    if ((u1VdpId != VDP_1) && (u1VdpId != VDP_2))
    {
        _arMDDiPrm[VDP_1].u1DebugNRDataMode = E_MIB_NR_UNKNOWN;        
        _arMDDiPrm[VDP_2].u1DebugNRDataMode = E_MIB_NR_UNKNOWN;                
    }
    else
    {    
        _arMDDiPrm[u1VdpId].u1DebugNRDataMode = u1DataMode;
    }
}

/**
 * @brief Set demo mode active region
 * @param u1OnOff SV_ON/SV_OFF
 * @param u1Region SV_LEFT/SV_RIGHT
 * @param u2Center Center line position
 */
void vDrvDIDemo(UINT8 u1OnOff, UINT8 u1Region, UINT16 u2Center)
{
    // Not support in MT5365
}

/**
 * @brief Set total number of lines to be clipped for ONLY DTV case
 * @param u1VdpId VDP_1/VDP_2
 * @param u2VClip Total number of lines to be clipped
 */
void vDrvDISetDtvClip(UINT8 u1VdpId, UINT16 u2DTVClip)
{
    //outdated FW Walkaround
}

/**
 * @brief Turn on/off trick mode
 * @param u1VdpId VDP_1/VDP_2
 * @param u1OnOff SV_ON/SV_OFF
 * @warning This should be invoked by TVD only
 */
void vDrvDITrickModeOnOff(UINT8 u1VdpId, UINT8 u1OnOff)
{
    if (u1VdpId == VDP_2 && _arMDDiPrm[VDP_2].u1VencOn)
    {
        return;
    }
    MDDI_WRITE_FLD(u1VdpId, MCVP_KC_17, (u1OnOff ? 1 : 0), AUTO_TRICK_ON);

    //Trick mode is an one-field-only mode. PSCAN has to work in 2D mode under this case
    vDrvDISetBob(u1VdpId,u1OnOff);
}

/**
 * @brief DI Frame Repeat
 * @param u1VdpId VDP_1/VDP_2
 * @param u1OnOff On/Off
 * @warning This should be invoked by B2R only
 */
void vDrvDIFrameRepeat(UINT8 u1VdpId, UINT8 u1OnOff)
{   
    if (u1VdpId >= VDP_NS)
    {
        return;
    }

    rMIBState[u1VdpId].u1AutoStop = u1OnOff;

    if (rMIBState[u1VdpId].bAllowedAutoStop)
    {
        vDrvDISetAutoStop(u1VdpId, u1OnOff);
    }
}

/**
 * @brief Is repeat field found
 * @param u1VdpId VDP_1/VDP_2
 * @return 0/1  1 repeat field found
 */
BOOL bDrvDIRepeatFldFound(UINT8 u1VdpId)
{
    return (BOOL)(rMIBState[u1VdpId].u1AutoStop && 
                  MDDI_READ_FLD(u1VdpId, MCVP_KC_0D, REPEAT_FIELD_FOUND));
}


/**
 * @brief Enable/disable PSCAN freeze mode
 * @param u1VdpId VDP_1/VDP_2
 * @param u1OnOff SV_ON/SV_OFF
 */
void vDrvDIFreezeOnOff(UINT8 u1VdpId, UINT8 u1OnOff)
{
       	
	if((!IS_MIB_FRC(u1VdpId)) && IS_INTERLACE(u1VdpId))
	{
	    MDDI_WRITE_FLD(u1VdpId, MCVP_KC_33, u1OnOff, AUTO_ISYNC_FREEZE_CS_OFF);
	}
	else
	{
	    MDDI_WRITE_FLD(u1VdpId, MCVP_KC_33, SV_OFF, AUTO_ISYNC_FREEZE_CS_OFF);
	}
	
    MDDI_WRITE_FLD(u1VdpId, MCVP_KC_0A, u1OnOff, FREEZE_ON);

}

/**
 * @brief Check if freeze function works under the current configuration
 * @param u1VdpId VDP_1/VDP_2
 * @return SV_TRUE/SV_FALSE
 */
UINT8 u1DrvDIFreezable(UINT8 u1VdpId)
{
    return IS_MIB_FRC(u1VdpId) ? SV_TRUE : SV_FALSE;
}

/**
 * @brief DI info for capture logo
 * @param u1VdpId VDP_1/VDP_2
 * @param pDispInfo DI info
 */
void vDrvDIDispInfoForCapLog(UINT8 bPath, DI_DISP_INFO_T* pDispInfo)
{
	UINT32 u4YStartMSBAddr,u4YStartLSBAddr,u4UStartMSBAddr,u4UStartLSBAddr,u4VStartMSBAddr,u4VStartLSBAddr; 
	UINT8  u1BitRes;
	UINT16 u2Width, u2Height;
	UINT32 AF_Index;
	UINT8  AF_Bottom;
			
	if (bPath >= VDP_NS)
	{
		return;
	}
		
	u1BitRes = _arMDDiPrm[bPath].u1BitRes;
	u2Width = _arMDDiPrm[bPath].u2Width;
	u2Height = IS_INTERLACE(bPath) ? (_arMDDiPrm[bPath].u2Height/2) : _arMDDiPrm[bPath].u2Height;
	u2Height = IS_420TO422(bPath) ? (((u2Height+1)>>1)<<1) : u2Height;
		
	pDispInfo->u1BlockMode = 1;
	pDispInfo->u2Width = u2Width;
	pDispInfo->u2Height = u2Height;
	pDispInfo->u1BitRes = u1BitRes;
	pDispInfo->u1ColorMode = _arMDDiPrm[bPath].u1420Mode; //420 or 422
	pDispInfo->u2DramPitch = MDDI_READ_FLD(bPath, MCVP_DRAM_09, DA_DRAM_LINE_LENGTH)*MDDI_LINE_LENGHT_UNIT;
		
	u4YStartMSBAddr = MDDI_READ_FLD(bPath, MCVP_DRAM_0A, DA_ADDR_BASE_MSB_Y);
	u4YStartLSBAddr = MDDI_READ_FLD(bPath, MCVP_DRAM_0B, DA_ADDR_BASE_LSB_Y);
	u4UStartMSBAddr = MDDI_READ_FLD(bPath, MCVP_DRAM_0C, DA_ADDR_BASE_MSB_U);
	u4UStartLSBAddr = MDDI_READ_FLD(bPath, MCVP_DRAM_0D, DA_ADDR_BASE_LSB_U);
	u4VStartMSBAddr = MDDI_READ_FLD(bPath, MCVP_DRAM_0E, DA_ADDR_BASE_MSB_V);
	u4VStartLSBAddr = MDDI_READ_FLD(bPath, MCVP_DRAM_0F, DA_ADDR_BASE_LSB_V);
		
	pDispInfo->u4YStartMSBAddr = u4YStartMSBAddr<<11;
	pDispInfo->u4YStartLSBAddr = u4YStartLSBAddr<<11;
	pDispInfo->u4UStartMSBAddr = u4UStartMSBAddr<<11;
	pDispInfo->u4UStartLSBAddr = u4UStartLSBAddr<<11;
	pDispInfo->u4VStartMSBAddr = u4VStartMSBAddr<<11;
	pDispInfo->u4VStartLSBAddr = u4VStartLSBAddr<<11;
				
	AF_Index = vDrvDIGetDADebugStatus(1);
	pDispInfo->u1AFYUVIndex = AF_Index & 0xFFF;
		
	AF_Bottom = MDDI_READ_FLD(bPath, MCVP_KC_19, CF_BOTTOM); // A=C  0:Top 1:Bottom
	pDispInfo->u1AFBottom = AF_Bottom;
		
	pDispInfo->u1YFrameNum = MDDI_READ_FLD(bPath, MCVP_DRAM_08, DA_FRAME_NUM_Y) + 1;
	pDispInfo->u1CFrameNUm = MDDI_READ_FLD(bPath, MCVP_DRAM_08, DA_FRAME_NUM_U) + 1;
		
	pDispInfo->u1CfgFlipMirror	= 0; //FlipMirror=Normal
	pDispInfo->u1DramFlipMirror = 0; //DramFlipMirror=Normal consequence.
    pDispInfo->bDramSnakeMapping = MDDI_READ_FLD(bPath, MCVP_DRAM_08, DA_SNAKE_MAPPING);//snake mapping or not.
			
	if(bIsScalerInput444(bPath))
	{
		pDispInfo->u1ColorMode = 2;//RGB444
		pDispInfo->u1OutMode = 2; //RGB444
				
	#ifdef CC_FLIP_MIRROR_SUPPORT
		if(u1FlipMirrorConfig != DI_NORMAL_MODE)
		{
			pDispInfo->u1CfgFlipMirror	= u1FlipMirrorConfig;
			pDispInfo->u1DramFlipMirror = 0; //SC Read Flip/Mirror
		}
	#endif		
	}
	else if(IS_INTERLACE(bPath)) //Interlace
	{
		pDispInfo->u1OutMode = 0; //Interlace based field
				
	#ifdef CC_FLIP_MIRROR_SUPPORT
		if(u1FlipMirrorConfig != DI_NORMAL_MODE)
		{
			pDispInfo->u1CfgFlipMirror	= u1FlipMirrorConfig;
			pDispInfo->u1DramFlipMirror = 1; //PSCAN Write Flip/Mirror
		}
	#endif
	}
	else //Progressive
	{
		pDispInfo->u1OutMode = 1; //Progressive based frame
				
	#ifdef CC_FLIP_MIRROR_SUPPORT
		if(u1FlipMirrorConfig != DI_NORMAL_MODE)
		{
			pDispInfo->u1CfgFlipMirror	= u1FlipMirrorConfig;
			if ((IS_PROGRESSIVE(bPath)) && IS_420TO422(bPath))
			{
				pDispInfo->u1DramFlipMirror = 1; //PSCAN Write Flip/Mirror
			}
			else
			{
				pDispInfo->u1DramFlipMirror = 0; //SC Read Flip/Mirror
			}
		}
	#endif		
	}
}

/**
* @brief  to tell whether overflow or underflow happen
* @param u1VdpId VDP_1/VDP_2
* @return 0 if no underflow or overflow happens else  total overflow/underflow number
*/
UINT32 u4DrvDIChkAllStatus(UINT8 u1VdpId)
{
    UINT32 u4Result = 0;

    u4Result += u4FlagStaCnt[u1VdpId][E_STA_PXL_OVERFLOW];
    u4Result += u4FlagStaCnt[u1VdpId][E_STA_PXL_UNDERFLOW];
    u4Result += u4FlagStaCnt[u1VdpId][E_STA_SC_UNDERFLOW];
    u4Result += u4FlagStaCnt[u1VdpId][E_STA_NR_OVERFLOW];
    u4Result += u4FlagStaCnt[u1VdpId][E_STA_FLG_OVERFLOW];
    u4Result += u4FlagStaCnt[u1VdpId][E_STA_FLG_UNDERFLOW];

    return u4Result;
}


/**
* @brief  Reset the overflow & underflow count
* @param u1VdpId VDP_1/VDP_2
*/
void vDrvDIRstStatusCnt(UINT8 u1VdpId)
{
    u4FlagStaCnt[u1VdpId][E_STA_PXL_OVERFLOW] = 0;
    u4FlagStaCnt[u1VdpId][E_STA_PXL_UNDERFLOW] = 0;
    u4FlagStaCnt[u1VdpId][E_STA_NR_OVERFLOW] = 0;
    u4FlagStaCnt[u1VdpId][E_STA_SC_UNDERFLOW] = 0;
    u4FlagStaCnt[u1VdpId][E_STA_FLG_OVERFLOW] = 0;
    u4FlagStaCnt[u1VdpId][E_STA_FLG_UNDERFLOW] = 0;
}


/**
 * @brief Clear DI status
 * @param u1VdpId VDP_1/VDP_2
 * @param u1OnOff clear On/Off
 */
void vDrvDIClearStatus(UINT8 u1VdpId, UINT8 u1OnOff)
{
    MDDI_WRITE_FLD(u1VdpId, MCVP_DRAM_08, u1OnOff, DA_OVERFLOW_CLR);
    MDDI_WRITE_FLD(u1VdpId, MCVP_DRAM_08, u1OnOff, DA_UNDERFLOW_CLR);
    MDDI_WRITE_FLD(u1VdpId, MCVP_DRAM_18, u1OnOff, DA_SC_UNDERFLOW_CLR);
    MDDI_WRITE_FLD(u1VdpId, MCVP_DRAM_01, u1OnOff, DA_FLAG_OVERFLOW_CLR);
    MDDI_WRITE_FLD(u1VdpId, MCVP_DRAM_01, u1OnOff, DA_FLAG_UNDERFLOW_CLR);
    MDDI_WRITE_FLD(u1VdpId, MCVP_DRAM_14, u1OnOff, DA_NR_OVERFLOW_CLR);

    MDDI_WRITE_FLD(u1VdpId, MCVP_DRAM_08, u1OnOff, DA_STATUS_CLR);            
    MDDI_WRITE_FLD(u1VdpId, MCVP_DRAM_14, u1OnOff, DA_NR_STATUS_CLR);
    MDDI_WRITE_FLD(u1VdpId, MCVP_DRAM_01, u1OnOff, DA_FLAG_STATUS_CLR);    
}


/**
* @brief Get DI vdp width
* @return DI vdp width
*/
UINT16 u2DrvDIGetVdpWidth(UINT8 u1VdpId)
{
    return MDDI_READ_FLD(u1VdpId, MCVP_KC_0A, HDEW);
}

/**
* @brief Get DI vdp height
* @return DI vdp height
*/
UINT16 u2DrvDIGetVdpHeight(UINT8 u1VdpId)
{
    UINT32 u4Height = MDDI_READ_FLD(u1VdpId, MCVP_KC_0A, VDEW);

    if((VDP_2==u1VdpId)&&IS_PROGRESSIVE(u1VdpId))
    {
        u4Height = u4Height >> 1;
    }
    
    return u4Height;
}

UINT32 fgDrvDIIsMIBDramAvaliable(UINT8 u1VdpId)
{
    return MDDI_READ_FLD(u1VdpId,MCVP_MULTI_00,EN_DRAM_RW)!=0;
}



void vDrvDISetDataMode(UINT8 u1VdpId, E_MIB_FRC_DMODE eFrc, E_MIB_DI_DMODE eDI, E_MIB_NR_DMODE  eNR)
{
    UINT32 u4MIBDateMode;
    UINT32 i;   
    E_MIB_IP_DMODE eIP;

    #define DMOD_M  (1<<0)    //Support Main
    #define DMOD_S  (1<<1)    //Support Sub

    PATH_CHECK(u1VdpId);

    eIP = IS_INTERLACE(u1VdpId) ? E_MIB_I : E_MIB_P;
    u4MIBDateMode = DMODE(eIP,eNR,eDI,eFrc);
    //Find MID Data Mode Configuration
    for(i=0;aDataMode[i].eMIBMode!= E_MIB_DMODE_END;++i)
    {
        if((aDataMode[i].eMIBMode == u4MIBDateMode) &&
            (((u1VdpId==VDP_1)&&(aDataMode[i].u1Flag&DMOD_M))||((u1VdpId==VDP_2)&&(aDataMode[i].u1Flag&DMOD_S)))
            )
        {           
            break;
        }    
    }

    if (aDataMode[i].eMIBMode == E_MIB_DMODE_END)
    {
        i = 0;
    }
    
    vDrvDISetDebugDataMode(u1VdpId, (aDataMode[i].eMIBMode));

    if(aDataMode[i].eMIBMode != E_MIB_DMODE_UNKNOWN)
    {
        VDP_SetDispmode(u1VdpId, (eFrc == E_MIB_FRC_MIB) ? 3 : 0 );
    }
    else
    {
        if (rMIBFbmPool[u1VdpId].u4Mode & FBM_POOL_MODE_MDDI_DISP)
        {
            VDP_SetDispmode(u1VdpId, 3);//FRC_MIB
        }
        else
        {
            VDP_SetDispmode(u1VdpId, 0);// FRC_SCL  
        }
    }
}

UINT8 u1DrvDiIs10Bit(UINT8 u1VdpId)
{
    return !MDDI_READ_FLD(u1VdpId, MCVP_DRAM_06, DA_BIT_SEL_WY);
}

UINT8 u1DrvDiIs422Mode(UINT8 u1VdpId)
{
   	return !MDDI_READ_FLD(u1VdpId, MCVP_KC_1E, M444);
}

UINT8 u1DrvDiIsTopField(UINT8 u1VdpId)
{
    return MDDI_READ_FLD(u1VdpId, MCVP_KC_0D, IN_FLD);
}

UINT32 u4DrvDIIsAutoStopped(UINT8 u1VdpId)
{
    PATH_CHECK(u1VdpId);
    return MDDI_READ_FLD(VDP_1, MCVP_KC_19, FRC_REPEAT_FIELD)&&IS_INTERLACE(u1VdpId)&&(!IS_FRAME_SEQ(u1VdpId));
}

/**
 * @brief Get DI output clock rate
 * @param u1VdpId VDP_1/VDP_2
 * @return u4ClkRate clock rate
 */

UINT32 u4DrvDIGetClkRate(UINT8 u1VdpId)
{
    UINT8 u1ClkType;
    u1ClkType = IO32ReadFldAlign(CKGEN_REG_MIB_OCLK_CKCFG, FLD_MIB_OCLK_SEL);    
    
    switch(u1ClkType)
    {        
        case MIB_CLK_TYPESYSPLL_D2_CK:  
            return MIB_SYSTEM_PLL/2;
        case MIB_CLK_TYPEENETPLL_D2_CK:
            return MIB_ETHERNET_PLL/2;
        case MIB_CLK_TYPEXTAL_CK:            
        case MIB_CLK_RSV:                   
        default:
            return MIB_XTAL_PLL;
    }
}


UINT8 u1DrvDIIsSupportedSignal(UINT8 u1VdpId)
{
    UINT32 u4InputClk;

    PATH_CHECK(u1VdpId);
    
    u4InputClk = wDrvVideoGetVTotal(u1VdpId) * wDrvVideoGetHTotal(u1VdpId) * bDrvVideoGetRefreshRate(u1VdpId);        

    if(SV_ON==_arMDDiPrm[u1VdpId].u1IsDualFPI)
    {
        u4InputClk/=2;
    }
    
    if (fgIs4k2kNativeTimingOut(u1VdpId))
    {
        return SV_OFF;
    }
    
    if((u4InputClk >= (u4DrvDIGetClkRate(u1VdpId))) 
        && (TDTV_TAG3D_2D == u4DrvTDTVGetTag3D(u1VdpId))) 
    {
        return SV_OFF;
    }
    else
    {
        return SV_ON;
    }
}

/**
 * @brief Enable pscan to force 22 film mode for DTV/MMP
 * @param fgOnOff  Enable forcing 22 film mode
 * @param u1Mode 0->top first  1->bottom first
 * @return  0->OK  1->can't force film mode
 */
UINT8 u1vDrvDIMPEGForce22FilmMode(UINT8 fgOnOff, UINT8 u1Mode)
{
    return 0;
}

void vDrvDIStopCtrlFw(UINT8 u1VdpId,UINT8 fgOnOff)
{
    if (u1VdpId == VDP_2)
    {        
        vIO32WriteFldAlign(ADAPTIVE_PSCAN_REG, fgOnOff, ADAPTIVE_PSCAN_SUB); 
    }
    else
    {
        vIO32WriteFldAlign(ADAPTIVE_PSCAN_REG, fgOnOff, ADAPTIVE_PSCAN_MAIN); 
    }
}

void vDrvDIInOutSelect(UINT8 u1VdpId,UINT8 fgInOut, UINT8 fgPath)
{
    if (u1VdpId == VDP_1)
    {
        if (fgInOut == 0) // in
        {
            vRegWriteFldAlign(MCVP_KC_17, (fgPath!=u1VdpId), MCVP_MAIN_IN_SEL);
        }
        else // out
        {
            vRegWriteFldAlign(MCVP_KC_17, (fgPath!=u1VdpId), MCVP_MAIN_OUT_SEL);
        }
    }
    else // VDP_2
    {
        if (fgInOut == 0) // in
        {
            vRegWriteFldAlign(MCVP_KC_17, (fgPath!=u1VdpId), MCVP_SUB_IN_SEL);
        }
        else // out
        {
            vRegWriteFldAlign(MCVP_KC_17, (fgPath!=u1VdpId), MCVP_SUB_OUT_SEL); // DI SRC MUX: sub out from main
        }
    }
}


/**
 * @brief whether DI is set to scaler bob mode
 * @param u1VdpId VDP_1/VDP_2
 * @return  whether it's scaler bob mode
 */
BOOL bDrvDIIsScalerBob(UINT8 u1VdpId)
{
    if (u1VdpId == VDP_2 || IS_PROGRESSIVE(u1VdpId))
    {
        return FALSE;
    }
    else if (IS_SCL_BOB(rMIBState[u1VdpId].eBandwidthDataMode))
    {
        return TRUE;
    }
    return FALSE;    
}

/**
 * @brief Clear DI set resolution status
 * @param u1VdpId VDP_1/VDP_2
 */
void vDrvDIClrSetRes(UINT8 u1VdpId, SET_RES_STATUS_TYPE_E eStatusType)
{
    if (eStatusType == SET_RES_INPUT || eStatusType == SET_RES_BOTH)
    {
        MDDI_WRITE_FLD(u1VdpId, MCVP_KC_18, 1, MCVP_SET_RES_CLEAR_I);
    }
    if (eStatusType == SET_RES_OUTPUT || eStatusType == SET_RES_BOTH)
    {
        MDDI_WRITE_FLD(u1VdpId, MCVP_KC_18, 1, MCVP_SET_RES_CLEAR_O);
    }    
}


/**
 * @brief Set Pscan Set Resolution Registers 
 * @param u1VdpId VDP_1/VDP_2
 * @param fgOnOff SV_ON/SV_OFF
 */
void vDrvDISetResEnable(UINT8 u1VdpId,UINT8 fgOnOff)
{
    if (u1VdpId == VDP_1)
    {
        MDDI_WRITE_FLD(VDP_1, MCVP_BC_00, fgOnOff, MAIN_WR_SET_RES_VP);
        MDDI_WRITE_FLD(VDP_1, MCVP_BC_00, fgOnOff, MAIN_RD_SET_RES_VP);
        
    }
    else
    {
        MDDI_WRITE_FLD(VDP_1, MCVP_BC_00, fgOnOff, PIP_WR_SET_RES_VP);
        MDDI_WRITE_FLD(VDP_1, MCVP_BC_00, fgOnOff, PIP_RD_SET_RES_VP);
    }
}

/**
 * @brief Turn on/off deinterlace set resolution mode
 * @param u1VdpId VDP_1/VDP_2
 * @param fgOnOff SV_ON/SV_OFF
 */
void vDrvDISetResMode(UINT8 u1VdpId,UINT8 fgOnOff)
{
    rMIBState[u1VdpId].u1DynRes = fgOnOff;
}

/**
 * @brief Trigger DI set resolution
 * @param u1VdpId VDP_1/VDP_2
 */
void vDrvDISetResTrigger(UINT8 u1VdpId)
{
    MDDI_WRITE_FLD(u1VdpId, MCVP_BC_00, 1, MCVP_SET_RESOLUTION);
}


/**
 * @brief Get DI set resolution status
 * @param u1VdpId VDP_1/VDP_2
 * @param eStatusType     SET_RES_INPUT, SET_RES_OUTPUT, SET_RES_BOTH,
 */
BOOL bDrvDIIsSetResDone(UINT8 u1VdpId, SET_RES_STATUS_TYPE_E eStatusType)
{
    UINT8 bInStatus, bOutStatus;
    bInStatus  = MDDI_READ_FLD(u1VdpId, MCVP_KC_18, MCVP_SET_RES_FINISH_I);
    bOutStatus = MDDI_READ_FLD(u1VdpId, MCVP_KC_18, MCVP_SET_RES_FINISH_O);

    if (eStatusType == SET_RES_INPUT)
    {
        return bInStatus;
    }
    else if (eStatusType == SET_RES_OUTPUT)
    {
        return bOutStatus;
    }
    else
    {
        return (bInStatus & bOutStatus);
    }
}


/**
 * @brief Get DI Set 3D L/R Swap
 * @param u1VdpId VDP_1/VDP_2
 * @param ON, OFF
 */
void vDrvDISetLRSwap(UINT8 u1VdpId,E_DI_LR_SWAP InOut , UINT32 u4OnOff)
{
    static UINT8 IsInInv=0;
    static UINT8 IsOutInv=0;

    UINT32 u4DA_3D_INPUT_LR_SWAP;
    UINT32 u4DA_NR_3D_INPUT_LR_SWAP;       
    UINT32 u4DA_3D_OUTPUT_LR_SWAP;
    UINT32 u4DA_SC_3D_OUTPUT_LR_SWAP;
    
    if(E_DI_LR_SWAP_IN==InOut)
    {
        IsInInv = u4OnOff;
    }
    else if(E_DI_LR_SWAP_OUT==InOut)
    {
        IsOutInv = u4OnOff;
    }

    u4DA_3D_INPUT_LR_SWAP       = IsInInv;
    u4DA_NR_3D_INPUT_LR_SWAP    = (IS_INTERLACE(u1VdpId)) ? IsOutInv : IsInInv;
    u4DA_3D_OUTPUT_LR_SWAP      = (IS_INTERLACE(u1VdpId)) ? IsOutInv : IsInInv;
    u4DA_SC_3D_OUTPUT_LR_SWAP   = (IS_INTERLACE(u1VdpId)) ? SV_OFF   : IsOutInv;

    if (u4DA_3D_INPUT_LR_SWAP     != MDDI_READ_FLD(u1VdpId, MCVP_DRAM_15, DA_3D_INPUT_LR_SWAP)   ||
        u4DA_NR_3D_INPUT_LR_SWAP  != MDDI_READ_FLD(u1VdpId, MCVP_DRAM_14, DA_NR_3D_INPUT_LR_SWAP)||
        u4DA_3D_OUTPUT_LR_SWAP    != MDDI_READ_FLD(u1VdpId, MCVP_DRAM_15, DA_3D_OUTPUT_LR_SWAP)  ||
        u4DA_SC_3D_OUTPUT_LR_SWAP != MDDI_READ_FLD(u1VdpId, MCVP_DRAM_19, DA_SC_3D_OUTPUT_LR_SWAP)
        )
    {
        vDrvDISetMuteCtrl(u1VdpId);
    }
    
    MDDI_WRITE_FLD(u1VdpId, MCVP_DRAM_15, u4DA_3D_INPUT_LR_SWAP     , DA_3D_INPUT_LR_SWAP);
    MDDI_WRITE_FLD(u1VdpId, MCVP_DRAM_14, u4DA_NR_3D_INPUT_LR_SWAP  , DA_NR_3D_INPUT_LR_SWAP);       
    MDDI_WRITE_FLD(u1VdpId, MCVP_DRAM_15, u4DA_3D_OUTPUT_LR_SWAP    , DA_3D_OUTPUT_LR_SWAP);
    MDDI_WRITE_FLD(u1VdpId, MCVP_DRAM_19, u4DA_SC_3D_OUTPUT_LR_SWAP , DA_SC_3D_OUTPUT_LR_SWAP);
}

//MIB
/**
 * @brief Dump DI In Out Status
 * @param u1VdpId VDP_1/VDP_2
 */
void vDrvDIInOutStatusDump(UINT8 bPath)
{
    LOG(0,"\tMIB  IN HT:%4d VT:%4d HA:%4d VA:%4d FRC_IN:0x%X FRC_OUT:0x%X\n",
        MDDI_READ_FLD(bPath,MCVP_KC_0E, IN_DE_HT),
        MDDI_READ_FLD(bPath,MCVP_KC_0C, IN_VT2)/2,
        MDDI_READ_FLD(bPath,MCVP_KC_0E, IN_DE_H_ACT),        
        MDDI_READ_FLD(bPath,MCVP_KC_0D, IN_V_ACT),
        MDDI_READ_FLD(bPath,MCVP_KC_0F, FRC_IN_VT),
        MDDI_READ_FLD(bPath,MCVP_KC_10, FRC_OUT_VT)
        );
}

/**
* @brief force Bypassing DI
* @param u1VdpId VDP_1/VDP_2
* @param u1BypassOnOff 1/0
* @return DI Debug Display Mode
*/
void u1DrvDIForceBypass(UINT8 u1VdpId, UINT8 u1BypassOnOff)
{
    PATH_CHECK(u1VdpId);

    _arMDDiPrm[u1VdpId].u1ForceDIBypass = u1BypassOnOff;
    
    vVRMSetEventFlg(u1VdpId, VRM_EVENT_BY_MIB);
    
}

/**
* @brief Query whether DI is force bypassed, it is for scaler interace venc currently.
* @param u1VdpId VDP_1/VDP_2
* @return True - force  bypassed  False - not force bypassed
*/
UINT8 u1DrvDIIsForceBypass(UINT8 u1VdpId)
{
    PATH_CHECK(u1VdpId);

    return _arMDDiPrm[u1VdpId].u1ForceDIBypass;
}

/**
* @brief Query whether DI is bypassed
* @param u1VdpId VDP_1/VDP_2
* @return True - bypassed  False - not bypassed
*/
UINT8 u1DrvDIIsBypass(UINT8 u1VdpId)
{
    UINT8 fgBYPASS_EN;
    PATH_CHECK(u1VdpId);

    fgBYPASS_EN = (
        !((_arMDDiPrm[u1VdpId].u1OnOff)
            &&((IS_420TO422(u1VdpId)&&(VDP_1==u1VdpId))
                || IS_NR_ON(u1VdpId) 
                || IS_MIB_FRC(u1VdpId) 
                || IS_INTERLACE(u1VdpId))))
        || (rMIBState[u1VdpId].eBandwidthDataMode == E_MIB_DMODE_UNKNOWN );
    
    return fgBYPASS_EN;
}

