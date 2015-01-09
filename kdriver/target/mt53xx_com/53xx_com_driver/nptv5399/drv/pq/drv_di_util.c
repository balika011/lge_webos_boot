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


#include "drv_di.h"
#include "drv_di_int.h"
#include "drv_nr.h"
#include "drv_scaler.h"
#include "drv_scpos.h"
#include "di_debug.h"

#include "hw_di.h"
#include "hw_di_int.h"
#include "hw_ckgen.h"
#include "hw_ycproc.h"
#include "hw_scpos.h"
#include "hw_sys.h"

#include "eeprom_if.h"

#include "vdo_misc.h"
#include "drv_display.h"
#include "drv_tdnavi.h"
#include "source_select.h"
#include "panel.h"
#include "drv_tdtv.h"

#if defined(CC_MT5399)
#include "hw_mjc.h"
#include "drv_mjc.h"
#endif

UINT32 u4FlagStaCnt[VDP_MAX_NS][E_PSCAN_STA_NUM];

UINT32 u4DIStatusPollingCount = 0;
DI_SP_REG_T     arDISPReg[8];

static PThreadFunc _pfnDIInputVsyncRising = NULL;
static PThreadFunc _pfnDIInputVsyncFalling = NULL;
static PThreadFunc _pfnDIInputVsyncAdj   = NULL;
static PThreadFunc _pfnDIOutputVsyncRising = NULL;
static PThreadFunc _pfnDIOutputVsyncFalling = NULL;
static PThreadFunc _pfnDIOutputVsyncAdj   = NULL;

void vDrvDIRegStatusPolling(void *pvArgv);

DI_SP_VSYNC_T DISPVsyncIsr[] =
{
    {&_pfnDIInputVsyncRising,    "DI Input Vsync Rising"},
    {&_pfnDIInputVsyncFalling,   "DI Input Vsync Falling"},
    {&_pfnDIInputVsyncAdj,       "DI Input Vsync Adj"},
    {&_pfnDIOutputVsyncRising,   "DI Output Vsync Rising"},
    {&_pfnDIOutputVsyncFalling,  "DI Output Vsync Falling"},
    {&_pfnDIOutputVsyncAdj,      "DI Input Vsync Adj"},
    {NULL,                  NULL}
};


DI_SP_THREAD DISPThread[] =
{
    {vDrvDIRegStatusPolling, "DI Register Status Polling"},
    {NULL                 ,NULL}//End Of Function Table
};

void vDrvDIRegStatusPolling(void *pvArgv)
{
    UINT32 u4Status;
    UINT32 u4Count = 0;
    while ((u4Count < 8) && (arDISPReg[u4Count].u4Addr != 0))
    {
        if (arDISPReg[u4Count].u4Addr < 0x10000)
        {
            u4Status = IO32ReadFldAlign(u4SWReg(arDISPReg[u4Count].u4Addr), Fld((arDISPReg[u4Count].u1Msb - arDISPReg[u4Count].u1Lsb + 1), arDISPReg[u4Count].u1Lsb, AC_MSKDW));
        }
        else
        {
            u4Status = IO32ReadFldAlign(arDISPReg[u4Count].u4Addr, Fld((arDISPReg[u4Count].u1Msb - arDISPReg[u4Count].u1Lsb + 1), arDISPReg[u4Count].u1Lsb, AC_MSKDW));
        }
        Printf("0x%08X ", u4Status);
        u4Count++;
    }
}

void _vDrvDISpVsyncIsrStatus(UINT8 u1VsyncStatus)
{
    if(u1VsyncStatus >= E_VSYNC_STATUS_MAX)
    {
        return;
    }
    
    if((*DISPVsyncIsr[u1VsyncStatus].ptrPfnVSyncFunc) !=NULL&&u4DIStatusPollingCount)
    {
        Printf("\n");
        (*DISPVsyncIsr[u1VsyncStatus].ptrPfnVSyncFunc)(NULL);

        u4DIStatusPollingCount --;
        if (u4DIStatusPollingCount == 0)
        {
            (*DISPVsyncIsr[u1VsyncStatus].ptrPfnVSyncFunc) = NULL;
            Printf("\n");
        }
    }
}
void vDrvDIGetSPReg(void)
{
    UINT32 u4Count;

    for (u4Count = 0; u4Count < 8; u4Count++)
    {
        Printf("(%d) 0x%08X[%02d:%02d]\n", u4Count, arDISPReg[u4Count].u4Addr, arDISPReg[u4Count].u1Msb, arDISPReg[u4Count].u1Lsb);
    }
}

UINT32 u4DrvDISetSPReg(UINT32 u4Index, UINT32 u4Addr, UINT32 u4Msb, UINT32 u4Lsb)
{
    if (u4Index >= 8)
    {
        return SV_FALSE;
    }

    arDISPReg[u4Index].u4Addr = u4Addr;
    arDISPReg[u4Index].u1Msb  = u4Msb;
    arDISPReg[u4Index].u1Lsb  = u4Lsb;

    return SV_TRUE;
}
////////////////////////////////////////////////////////////////////////////////
//MDDI Debug Register Lor
////////////////////////////////////////////////////////////////////////////////

#ifdef LOG_MDDI_REG

#define DI_LOG_NUM 1024
#define DI_LOG_MSK (DI_LOG_NUM-1)
MDDI_REGTBL_T MDDI_REG_LOG[DI_LOG_NUM]; 
UINT32 u4LogReg = 0;

/**
 * @brief DI Write Field Macro
 * @param u1VdpId VDP_1/VDP_2
 * @param u2Addr Address
 * @param u4Value Value
 * @param u4Fld Field
 */
void MDDI_WRITE_FLD(UINT8 u1VdpId, UINT16 u2Addr, UINT32 u4Value, UINT32 u4Fld)
{
    vIO32WriteFldAlign((u2Addr + ((u1VdpId == VDP_1) ? 0 : MDDI_REG_NUM)), u4Value, u4Fld);
    //Avoid interupt clear bit access
    if (u2Addr == 0x402C) return;
    MDDI_REG_LOG[u4LogReg].u2Addr = (u2Addr + ((u1VdpId == VDP_1) ? 0 : MDDI_REG_NUM))|0x1;
    MDDI_REG_LOG[u4LogReg].u4Value = u4Value;
    MDDI_REG_LOG[u4LogReg].u4Mask = u4Fld;
    u4LogReg = (u4LogReg+1)&DI_LOG_MSK;
}

/**
 * @brief DI Write Mask Macro
 * @param u1VdpId VDP_1/VDP_2
 * @param u2Addr Address
 * @param u4Value Value
 * @param u4Msk Mask
 */
void MDDI_WRITE32_MSK(UINT8 u1VdpId, UINT16 u2Addr, UINT32 u4Value, UINT32 u4Msk) 
{
    vIO32Write4BMsk((u2Addr + ((u1VdpId == VDP_1) ? 0 : MDDI_REG_NUM)), u4Value, u4Msk);
    MDDI_REG_LOG[u4LogReg].u2Addr = (u2Addr + ((u1VdpId == VDP_1) ? 0 : MDDI_REG_NUM));
    MDDI_REG_LOG[u4LogReg].u4Value = u4Value;
    MDDI_REG_LOG[u4LogReg].u4Mask = u4Msk;
    u4LogReg = (u4LogReg+1)&DI_LOG_MSK;
}

/**
 * @brief DI Print Register Log
 */
void MDDI_PRINT_REG_LOG(void)
{
    UINT16 i, index, j = u4LogReg;

    Printf("\nIndex    u2Addr         u4Value        u4Mask\n");    

    for (i = j; i < (1024 + j); i++)
    {
        index = i&DI_LOG_MSK;
        if ((index&0xFF) == 0xFF)
        {
            x_thread_delay(20);
        }
        Printf("%4d    %6x        %8x        %08x\n", index,
                MDDI_REG_LOG[index].u2Addr&0xFFFC, MDDI_REG_LOG[index].u4Value,
                (MDDI_REG_LOG[u4LogReg].u2Addr & 0x1) 
                ? (Fld2Msk32(MDDI_REG_LOG[index].u4Mask))
                : MDDI_REG_LOG[index].u4Mask);
    }
}
#endif


/**
* @brief Check DI status
* @param u1VdpId VDP_1/VDP_2
*/
void vDrvDIChkStatus(UINT8 u1VdpId)
{
    if (SV_ON!=_arMDDiPrm[u1VdpId].u1OnOff)
    {
        return;
    }

    if ((rMIBState[u1VdpId].u1StableCnt > 0) &&
        (rMIBState[u1VdpId].u1StableCnt & MIB_STATUS_CNT_MASK) == 0)
    {  
  
        if(MDDI_READ_FLD(u1VdpId, MCVP_KC_17, ABCD_YC_EN))  	
        {
            if (MDDI_READ_FLD(u1VdpId, MCVP_DRAM_08, STA_OVERFLOW))
            {
                u4FlagStaCnt[u1VdpId][E_STA_PXL_OVERFLOW]++;
            }
        
            if (MDDI_READ_FLD(u1VdpId, MCVP_DRAM_08, STA_UNDERFLOW))
            {
                u4FlagStaCnt[u1VdpId][E_STA_PXL_UNDERFLOW]++;
            }
        }    

        if (u1VdpId == VDP_1)
        {        
            if (MDDI_READ_FLD(u1VdpId,MCVP_DRAM_18, DA_SC_READ_ENABLE) && MDDI_READ_FLD(u1VdpId, MCVP_DRAM_1A, STA_SC_UNDERFLOW))
            {
                u4FlagStaCnt[u1VdpId][E_STA_SC_UNDERFLOW]++;
            }

            if ((MDDI_READ_FLD(u1VdpId, MCVP_MULTI_00, EN_DRAM_RW) & E_NR_W) && MDDI_READ_FLD(u1VdpId, MCVP_DRAM_14, STA_NR_OVERFLOW))
            {
                u4FlagStaCnt[u1VdpId][E_STA_NR_OVERFLOW]++;
            }	

            if (MDDI_READ_FLD(u1VdpId, MCVP_MULTI_00, EN_DRAM_RW) & (E_MV_W | E_MV_R | E_LM_W | E_LM_R))
            {
	            if (MDDI_READ_FLD(u1VdpId, MCVP_DRAM_07, STA_FLAG_OVERFLOW))
	            {
	                u4FlagStaCnt[u1VdpId][E_STA_FLG_OVERFLOW]++;
	            }  
	            
	            if (MDDI_READ_FLD(u1VdpId, MCVP_DRAM_07, STA_FLAG_UNDERFLOW))
	            {
	                u4FlagStaCnt[u1VdpId][E_STA_FLG_UNDERFLOW]++;
	            }
            }
        }

        if(MDDI_READ_FLD(u1VdpId, MCVP_DRAM_08, STA_WRITE_OOB)
            ||(MDDI_READ_FLD(u1VdpId, MCVP_DRAM_14, STA_NR_WRITE_OOB))
            ||(MDDI_READ_FLD(u1VdpId, MCVP_DRAM_07, STA_FLAG_WRITE_OOB))
            )
        {
            LOG(1,"\nMIB(PSCAN) Path %d Dram Wrtie over boundary !!\n",u1VdpId);
        }
               
        vDrvDIClearStatus(u1VdpId, SV_ON);
        vDrvDIClearStatus(u1VdpId, SV_OFF);
    }
    
}


/**
 * @brief Get DI Buffer Address
 * @param u1VdpId VDP_1/VDP_2
 * @param u4AddrStart start address
 * @param u4AddrEnd end address
 */
void vDrvDIGetBufAddr(UINT8 u1VdpId, UINT32 *u4AddrStart, UINT32 *u4AddrEnd)
{
    *u4AddrStart = (MDDI_READ_FLD(u1VdpId, MCVP_DRAM_11, DA_WADDR_LO_LIMIT) << 11);

    if (u1VdpId == VDP_1) // Sub path no flag dram access
    {
        *u4AddrEnd = (MDDI_READ_FLD(u1VdpId, MCVP_DRAM_05, DA_FLAG_WADDR_HI_LIMIT) << 4);
    }
    else
    {
        *u4AddrEnd = (MDDI_READ_FLD(u1VdpId, MCVP_DRAM_10, DA_WADDR_HI_LIMIT) << 11);
    }    
}

/**
 * @brief Set DI On/Off
 * @param u1VdpId VDP_1/VDP_2
 * @param bOnOff On/Off
 */
void vDrvDIOnOff(UINT8 u1VdpId, UINT8 bOnOff)
{
    MDDI_WRITE_FLD(u1VdpId, MCVP_KC_00, !bOnOff, BYPASS_EN);
}

/**
 * @brief Set DI Color Mode
 * @param u1VdpId VDP_1/VDP_2
 * @param u1ColorMode Color Format 0: normal , 1: Force 420 Mode;
 */
void vDrvDISetColorMode(UINT8 u1VdpId, UINT8 u1ColorMode)
{
    _arMDDiPrm[u1VdpId].u1Force420Mode=u1ColorMode;
}

/**
 * @brief Set DI Field
 * @param u1VdpId VDP_1/VDP_2
 * @param u1Inv Normal/Inverse
 */
void vDrvDISetFieldInverse(UINT8 u1VdpId, UINT8 u1Inv)
{
    UINT8 u1Mode = MDDI_READ_FLD(u1VdpId, MCVP_DRAM_08, DA_DISPLAY_MODE);    
    if (IS_FLIP_MIRROR(u1VdpId) && IS_INTERLACE(u1VdpId))
    {
        if ((u1Mode == DI_FLIP_ON_MODE) || (u1Mode == DI_FLIP_MIRROR_ON_MODE))
        {
            u1Inv = !u1Inv;
        }   
    }

    if(vDrvTTTIsFldInv(u1VdpId)==SV_ON)
    {
        u1Inv = !u1Inv;
    }

    if(IS_PROGRESSIVE(u1VdpId))
    {
        u1Inv=0;
    }
    
    MDDI_WRITE_FLD(u1VdpId, MCVP_KC_00, u1Inv, FLD_IN_P);
}

/**
 * @brief Set DI write bit resolution
 * @param u1VdpId VDP_1/VDP_2
 * @param b8bit 1: 8bit/0: 10bit
 */
void vDrvDISetWritePrecision(UINT8 u1VdpId, UINT8 b8bit)
{
    MDDI_WRITE_FLD(u1VdpId, MCVP_DRAM_08, b8bit, DA_BIT_SEL_WY);
    MDDI_WRITE_FLD(u1VdpId, MCVP_DRAM_08, b8bit, DA_BIT_SEL_WU);
    MDDI_WRITE_FLD(u1VdpId, MCVP_DRAM_08, b8bit, DA_BIT_SEL_WV);
    MDDI_WRITE_FLD(u1VdpId, MCVP_DRAM_18, b8bit, DA_SC_BIT_SEL_RY);
    MDDI_WRITE_FLD(u1VdpId, MCVP_DRAM_18, b8bit, DA_SC_BIT_SEL_RU);
    MDDI_WRITE_FLD(u1VdpId, MCVP_DRAM_18, b8bit, DA_SC_BIT_SEL_RV);
}



/**
 * @brief Set DI Output Pattern
 * @param u1VdpId VDP_1/VDP_2
 * @param u1OnOff On/Off
 */
void vDrvDISetPattern(UINT8 u1VdpId, UINT8 u1OnOff)
{
    UINT8 u1Val = ((u1OnOff == 1) ? 0x7 : 0x0);
    MDDI_WRITE_FLD(u1VdpId, MCVP_KC_1E, u1Val, PAT_GEN_MODE_Y);
    MDDI_WRITE_FLD(u1VdpId, MCVP_KC_1E, u1Val, PAT_GEN_MODE_CB);
    MDDI_WRITE_FLD(u1VdpId, MCVP_KC_1E, u1Val, PAT_GEN_MODE_CR);
    MDDI_WRITE_FLD(u1VdpId, MCVP_KC_1E, u1OnOff, BORDER_EN);
}

/**
 * @brief Set DI Input Pattern
 * @param u1VdpId VDP_1/VDP_2
 * @param u1OnOff On/Off
 */
void vDrvDISetInputPattern(UINT8 u1VdpId, UINT8 u1OnOff)
{	
    //MDDI_WRITE_FLD(u1VdpId, MCVP_KC_04, u1OnOff, IPTG_BORDER_EN);
    MDDI_WRITE_FLD(u1VdpId, MCVP_KC_05, 3      , IPTG_HV_MODE);
	MDDI_WRITE_FLD(u1VdpId, MCVP_KC_03, (u1OnOff == 1)? ((u1VdpId == VDP_1)? 6 : 5):0, IPTG_PAT_TYPE);
	
    MDDI_WRITE_FLD(u1VdpId, MCVP_KC_05, 0x3FF  , IPTG_FC_Y);
    MDDI_WRITE_FLD(u1VdpId, MCVP_KC_05, 0x200  , IPTG_FC_CR);
	MDDI_WRITE_FLD(u1VdpId, MCVP_KC_05, 0x200  , IPTG_FC_CB);

	MDDI_WRITE_FLD(u1VdpId, MCVP_KC_06, 0x100  , IPTG_BC_Y);
    MDDI_WRITE_FLD(u1VdpId, MCVP_KC_06, 0x200  , IPTG_BC_CR);
	MDDI_WRITE_FLD(u1VdpId, MCVP_KC_06, 0x200  , IPTG_BC_CB);
	
	MDDI_WRITE_FLD(u1VdpId, MCVP_KC_02, MDDI_READ_FLD(u1VdpId, MCVP_KC_0A, HDEW), IPTG_H_ACT);
	MDDI_WRITE_FLD(u1VdpId, MCVP_KC_02, IS_PROGRESSIVE(u1VdpId)?MDDI_READ_FLD(u1VdpId, MCVP_KC_0A, VDEW):MDDI_READ_FLD(u1VdpId, MCVP_KC_0A, VDEW)/2, IPTG_V_ACT);
}

/**
 * @brief Set DI Flip-Mirror
 * @param u1VdpId VDP_1/VDP_2
 * @param u1Mode Flip-Mirror Mode
 */
void vDrvDISetFlipMirror(UINT8 u1VdpId, UINT8 u1Mode)
{    

    if (u1VdpId >= VDP_NS)
    {
        u1VdpId = VDP_1;
    }
    
    if (IS_INTERLACE(u1VdpId))
    {
        MDDI_WRITE_FLD(u1VdpId, MCVP_DRAM_08, u1Mode, DA_DISPLAY_MODE); //420            
        MDDI_WRITE_FLD(u1VdpId, MCVP_DRAM_18, DI_NORMAL_MODE, DA_SC_DISPLAY_MODE);
    }
    else
    {
        MDDI_WRITE_FLD(u1VdpId, MCVP_DRAM_08, DI_NORMAL_MODE, DA_DISPLAY_MODE);
        MDDI_WRITE_FLD(u1VdpId, MCVP_DRAM_18, u1Mode, DA_SC_DISPLAY_MODE); //422 & 444
    }
  
    vDrvDISetFieldInverse(u1VdpId, SV_OFF);
}


/**
 * @brief Set Force Flip-Mirror
 * @param u1VdpId VDP_1/VDP_2
 * @param u1Mode Flip-Mirror Mode
 */
void vDrvDISetDbgFlipMirror(UINT8 u1VdpId, UINT8 u1Mode)
{   
    if(u1Mode != DI_FLIP_MIRROR_UNKOWN)
    {           
        _arMDDiPrm[u1VdpId].u1DebugFlipMorrorByMIB=SV_ON;    

        u1FlipMirrorConfig = u1Mode; 
    }
    else
    {
        _arMDDiPrm[u1VdpId].u1DebugFlipMorrorByMIB=SV_OFF;    
    }
}


/**
 * @brief Set DI ROI Pixel
 * @param u1VdpId VDP_1/VDP_2
 * @param u4Roi_H left and right border : 1 = 4 pixels 
 * @param u4Roi_V top and bottom border : 1 = 4 pixels 
 */
void vDrvDISetROIPixel(UINT8 u1VdpId, UINT32 u4Roi_H, UINT32 u4Roi_V)
{
    UINT32 u4RoiPixel = MPY_AND_CEIL(_arMDDiPrm[u1VdpId].u2Width-u4Roi_H*4*2, _arMDDiPrm[u1VdpId].u2Height/2-u4Roi_V*2*2, 16);
    
    MDDI_WRITE_FLD(u1VdpId, MCVP_FUSION_0E, u4RoiPixel, IF_ROI_PIXELS);
    MDDI_WRITE_FLD(u1VdpId, MCVP_FUSION_0E, u4Roi_V, IF_ROI_V);
    MDDI_WRITE_FLD(u1VdpId, MCVP_FUSION_0E, u4Roi_H, IF_ROI_H);
}

/**
 * @brief Init DI CRC
 * @param u1VdpId VDP_1/VDP_2
 */
void vDrvDIInitPscanCRC(UINT8 u1VdpId)
{
    /* PSCAN CRC */
    MDDI_WRITE_FLD(u1VdpId, MCVP_KC_1A, 0x1, SRCRW_CRC_BOT_L_EN);
    MDDI_WRITE_FLD(u1VdpId, MCVP_KC_1A, 0x1, SRCRW_CRC_TOP_L_EN);
    MDDI_WRITE_FLD(u1VdpId, MCVP_KC_1A, 0x1, SRCRW_CRC_BOT_R_EN);
    MDDI_WRITE_FLD(u1VdpId, MCVP_KC_1A, 0x1, SRCRW_CRC_TOP_R_EN);
    MDDI_WRITE_FLD(u1VdpId, MCVP_KC_11, 0x3ff, SRCW_CRC_Y_MASK);
    MDDI_WRITE_FLD(u1VdpId, MCVP_KC_11, 0x3ff, SRCW_CRC_C_MASK);
    MDDI_WRITE_FLD(u1VdpId, MCVP_KC_11, 0x0, SRCW_CRC_V_MASK);    
    MDDI_WRITE_FLD(u1VdpId, MCVP_KC_13, 0x0, SRCW_CRC_CLIP_H_START);
    MDDI_WRITE_FLD(u1VdpId, MCVP_KC_13, _arMDDiPrm[u1VdpId].u2Width-1, SRCW_CRC_CLIP_H_END);
    MDDI_WRITE_FLD(u1VdpId, MCVP_KC_12, 0x0, SRCW_CRC_CLIP_V_START);
    MDDI_WRITE_FLD(u1VdpId, MCVP_KC_12, (_arMDDiPrm[u1VdpId].u2Height/2)-1, SRCW_CRC_CLIP_V_END);

    MDDI_WRITE_FLD(u1VdpId, MCVP_KC_1A, 0x3ff, SRCR_CRC_Y_MASK);
    MDDI_WRITE_FLD(u1VdpId, MCVP_KC_1A, 0x3ff, SRCR_CRC_C_MASK);
    MDDI_WRITE_FLD(u1VdpId, MCVP_KC_1A, 0x0, SRCR_CRC_SEL_FIELD);
    MDDI_WRITE_FLD(u1VdpId, MCVP_KC_1A, 0x1, SRCR_CRC_AUTO_FIELD_EN);
    MDDI_WRITE_FLD(u1VdpId, MCVP_KC_1C, 0x0, SRCR_CRC_CLIP_H_START);
    MDDI_WRITE_FLD(u1VdpId, MCVP_KC_1C, _arMDDiPrm[u1VdpId].u2Width-1, SRCR_CRC_CLIP_H_END);
    MDDI_WRITE_FLD(u1VdpId, MCVP_KC_1B, 0x0, SRCR_CRC_CLIP_V_START);
    MDDI_WRITE_FLD(u1VdpId, MCVP_KC_1B, (_arMDDiPrm[u1VdpId].u2Height/2)-1, SRCR_CRC_CLIP_V_END);

    MDDI_WRITE_FLD(u1VdpId, MCVP_KC_24, 0x3ff, OUT_CRC_Y_MASK);
    MDDI_WRITE_FLD(u1VdpId, MCVP_KC_24, 0x3ff, OUT_CRC_C_MASK);
    MDDI_WRITE_FLD(u1VdpId, MCVP_KC_24, 0x0, OUT_CRC_V_MASK);
    MDDI_WRITE_FLD(u1VdpId, MCVP_KC_26, 0x1, OUT_CRC_L_EN);
    MDDI_WRITE_FLD(u1VdpId, MCVP_KC_26, 0x1, OUT_CRC_R_EN);
    MDDI_WRITE_FLD(u1VdpId, MCVP_KC_26, 0x0, OUT_CRC_CLIP_H_START);
    MDDI_WRITE_FLD(u1VdpId, MCVP_KC_26, _arMDDiPrm[u1VdpId].u2Width-1, OUT_CRC_CLIP_H_END);
    MDDI_WRITE_FLD(u1VdpId, MCVP_KC_25, 0x0, OUT_CRC_CLIP_V_START);
    MDDI_WRITE_FLD(u1VdpId, MCVP_KC_25, _arMDDiPrm[u1VdpId].u2Height-1, OUT_CRC_CLIP_V_END);
    
}

/**
 * @brief Init ALL CRC
 * @param u1VdpId VDP_1/VDP_2
 */
void vDrvDIInitCRC(UINT8 u1VdpId)
{
    vDrvDIInitPscanCRC(u1VdpId);
    
    /* OMUX CRC */
    vIO32WriteFldAlign(SYS_04, (u1VdpId == VDP_1) ? 0 : 1, CRC_SRC_SEL);
    vIO32WriteFldAlign(PQCRC_00, 0x3FF, PQCRC_Y_MSK);
    vIO32WriteFldAlign(PQCRC_00, 0x3FF, PQCRC_C_MSK);
    vIO32WriteFldAlign(PQCRC_00, 0x3FF, PQCRC_V_MSK);
    vIO32WriteFldAlign(PQCRC_00, 0x1, PQCRC_TOP_EN);
    vIO32WriteFldAlign(PQCRC_00, 0x1, PQCRC_BOT_EN);
    vIO32WriteFldAlign(PQCRC_01, 0x1, PQCRC_EVEN_LINE_EN);
    vIO32WriteFldAlign(PQCRC_01, 0x1, PQCRC_ODD_LINE_EN);

    /* PREPROC CRC */
    vIO32WriteFldAlign(TOP_MAIN_00, 0x1, C_CRC_SEL); /* 0: main_in, 1: main_out, 2: pip_in, 3: pip_out */
    vIO32WriteFldAlign(CRC_00, 0x3FF, C_Y_IN_MASK_MAIN);
    vIO32WriteFldAlign(CRC_00, 0x3FF, C_C_IN_MASK_MAIN);
    vIO32WriteFldAlign(CRC_00, 0x3FF, C_V_IN_MASK_MAIN);
    vIO32WriteFldAlign(CRC_00, 0x1, C_TOP_L_EN_MAIN);
    vIO32WriteFldAlign(CRC_00, 0x1, C_BOT_L_EN_MAIN);
    vIO32WriteFldAlign(CRC_01, 0x1, C_TOP_R_EN_MAIN);
    vIO32WriteFldAlign(CRC_01, 0x1, C_BOT_R_EN_MAIN);
}

/**
 * @brief Init DI Input CRC
 * @param u1VdpId VDP_1/VDP_2
 */
UINT32 u4DrvDIGetInputCRC(UINT8 u1VdpId)
{
    return MDDI_READ_FLD(u1VdpId, MCVP_KC_15, SRCW_CRC_OUT_V);
}

/**
 * @brief Init DI Output CRC
 * @param u1VdpId VDP_1/VDP_2
 */
UINT32 u4DrvDIGetOutputCRC(UINT8 u1VdpId)
{
    return MDDI_READ_FLD(u1VdpId, MCVP_KC_28, OUT_CRC_OUT_V);
}

/**
 * @brief Set DI ISR Enable
 * @param u1Enable On/Off
 */
UINT8 u1DlyAfterISREn[2] = {0,0};
void vDrvDIEnableISR(UINT8 u1VdpId, UINT8 u1Enable)
{
    PATH_CHECK(u1VdpId);
    if (u1VdpId == VDP_1)
    {

        MDDI_WRITE_FLD(VDP_1, MCVP_BC_01, u1Enable, INT_MIVS_R_EN);           
        MDDI_WRITE_FLD(VDP_1, MCVP_BC_01, u1Enable, INT_MIVS_F_EN);      
        MDDI_WRITE_FLD(VDP_1, MCVP_BC_01, u1Enable, INT_MOVS_R_EN);
        MDDI_WRITE_FLD(VDP_1, MCVP_BC_01, u1Enable, INT_MOVS_F_EN);        
        MDDI_WRITE_FLD(VDP_1, MCVP_BC_01, u1Enable, INT_MO_ADJ_EN);
        MDDI_WRITE_FLD(VDP_1, MCVP_BC_01, u1Enable, INT_MI_ADJ_EN);
    }
    else
    {
        MDDI_WRITE_FLD(VDP_1, MCVP_BC_01, u1Enable, INT_PIVS_R_EN);                           
        MDDI_WRITE_FLD(VDP_1, MCVP_BC_01, u1Enable, INT_PIVS_F_EN);
        //MDDI_WRITE_FLD(VDP_1, MCVP_BC_01, u1Enable, INT_POVS_R_EN);
#ifdef PSCAN_AUTO_VERIFY                
        MDDI_WRITE_FLD(VDP_1, MCVP_BC_01, u1Enable, INT_POVS_F_EN);
#endif
        MDDI_WRITE_FLD(VDP_1, MCVP_BC_01, u1Enable, INT_PI_ADJ_EN);
    }
    
    if(u1Enable == SV_ON)
    {
        #define DEL_TRIG_PREDOWN_AFTER_ISREN 5
        u1DlyAfterISREn[u1VdpId] = DEL_TRIG_PREDOWN_AFTER_ISREN;
    }
}

/**
 * @brief Set DI Double Buffer
 * @param u1VdpId VDP_1/VDP_2
 * @param u1OnOff On/Off
 */
void vDrvDISetDoubleBuffer(UINT8 u1VdpId, UINT8 u1OnOff)
{
    if (u1VdpId == VDP_1)
    {
        MDDI_WRITE_FLD(VDP_1, MCVP_BC_00, u1OnOff, MAIN_RD_LOCAL_VP);
        MDDI_WRITE_FLD(VDP_1, MCVP_BC_00, u1OnOff, MAIN_WR_LOCAL_VP);
    }
    else
    {
        MDDI_WRITE_FLD(VDP_1, MCVP_BC_00, u1OnOff, PIP_RD_LOCAL_VP);
        MDDI_WRITE_FLD(VDP_1, MCVP_BC_00, u1OnOff, PIP_WR_LOCAL_VP);
    }
}

/**
 * @brief Set DI Clock Reset
 * @param u1VdpId VDP_1/VDP_2
 * @param u1OnOff On/Off
 */
void vDrvDISetClkReset(UINT8 u1VdpId, UINT8 u1OnOff)
{
    /* Note */
    /* Please follow the sequence when doing reset. */
    /* If the reset sequence is not correct, prefetch error may occur. */

    if ((u1VdpId == VDP_1)||(u4DrvTDTVIsDualFPI()))
    {        
        vDrvDIEnableISR(VDP_1, !u1OnOff);

        if(SV_ON==u1OnOff)
        { 
            MDDI_WRITE_FLD(VDP_1, MCVP_BC_00, SV_OFF, MCVP_K_M_CK_EN);
        }
    
        MDDI_WRITE_FLD(VDP_1, MCVP_BC_00, u1OnOff, DRAM_M_CK_RST);
        MDDI_WRITE_FLD(VDP_1, MCVP_BC_00, u1OnOff, MIB_IN_M_CK_RST);
        MDDI_WRITE_FLD(VDP_1, MCVP_BC_00, u1OnOff, MCVP_K_M_CK_RST);

        if(SV_OFF==u1OnOff)
        {
            MDDI_WRITE_FLD(VDP_1, MCVP_BC_00, SV_ON, MCVP_K_M_CK_EN);
        }        
    }

    if((u1VdpId == VDP_2)||(u4DrvTDTVIsDualFPI()))
    {
        vDrvDIEnableISR(VDP_2, !u1OnOff);

        if(SV_ON==u1OnOff)
        { 
            MDDI_WRITE_FLD(VDP_1, MCVP_BC_00, SV_OFF, MCVP_K_P_CK_EN);
        }    
    
        MDDI_WRITE_FLD(VDP_1, MCVP_BC_00, u1OnOff, DRAM_P_CK_RST);
        MDDI_WRITE_FLD(VDP_1, MCVP_BC_00, u1OnOff, MIB_IN_P_CK_RST);
        MDDI_WRITE_FLD(VDP_1, MCVP_BC_00, u1OnOff, MCVP_K_P_CK_RST);

        if(SV_OFF==u1OnOff)
        {
            MDDI_WRITE_FLD(VDP_1, MCVP_BC_00, SV_ON, MCVP_K_P_CK_EN);
        } 
    }
   
}

/**
 * @brief Set DI FRC line
 * @param u1VdpId VDP_1/VDP_2
 * @param u1FrcLine
 */
void vDrvDISetFrcLine(UINT8 u1VdpId)
{
    UINT32 u2InputLine = _arMDDiPrm[u1VdpId].u2Height;
    UINT32 u1OutFR = _arMDDiPrm[u1VdpId].u1OutFR;
    UINT32 u1InFR = _arMDDiPrm[u1VdpId].u1InFR;
    
    //#define FRC_LINE_TOLERENCE_N 5
    #define FRC_LINE_TOLERENCE_M 32
    #define SCL_LINE_BUF    1440

    UINT32 u4FrcLineTolN = 5;
    UINT32 u4DispV = 0;

    if (IS_MIB_FRC(u1VdpId))
    {
        VRM_INFO_T in;
        VRM_INFO_T out;
        VRMGetModuleVRMInfo(u1VdpId, VRM_MODULE_FSC, &in, &out);
        u4DispV = out.u2Height + out.u2Y;
    }

    if(0 != u4DispV)
    {
        u4FrcLineTolN = (_arMDDiPrm[u1VdpId].u4OutVT  *FRC_LINE_TOLERENCE_M + u4DispV - 1) / u4DispV;       
    }
    else
    {
        u4FrcLineTolN=40;        
    }

    u2InputLine = IS_INTERLACE(u1VdpId) ? u2InputLine>>1 : u2InputLine;


    //Consider the pair repeat , 2 frame complete 1 pair
    if((E_TD_IN_FS_I==_arMDDiPrm[u1VdpId].eTDInput)
        ||(E_TD_IN_FS_P==_arMDDiPrm[u1VdpId].eTDInput)
        )
    {
        u1InFR /=2;
    }
    
    if((E_TD_IN_LI_I==_arMDDiPrm[u1VdpId].eTDInput)
        ||(E_TD_IN_LI_P==_arMDDiPrm[u1VdpId].eTDInput)
        )
    {
        u2InputLine *=2;
    }   
         
    if(IS_FLIP_MIRROR(u1VdpId))
    {
        //DRAM Can NOT be read before whole frame being writed
        u2InputLine -=1;
    }
    else
    {
        if(u1OutFR!=0 && u4FrcLineTolN!=0)
        {   
            //Read Pointer faster than write pointer , should leave more space for write
            u2InputLine = 
                MAX(u2InputLine/4 ,
                    (u2InputLine 
                    *(u4FrcLineTolN * u1OutFR - FRC_LINE_TOLERENCE_M * u1InFR) + (u4FrcLineTolN * u1OutFR) -1)
                    /(u4FrcLineTolN * u1OutFR)
                );                

            if(_arMDDiPrm[u1VdpId].u2Width!=0)
            {
                UINT32 FifoLength = IS_420TO422(u1VdpId) ? (SCL_LINE_BUF  * 3+1)/2  : SCL_LINE_BUF;
                u2InputLine += (FifoLength + _arMDDiPrm[u1VdpId].u2Width -1) /_arMDDiPrm[u1VdpId].u2Width ;
            }
        }
        else
        {
            u2InputLine = u2InputLine/4;
        }
    }

    u2InputLine = MIN(IS_INTERLACE(u1VdpId) ? _arMDDiPrm[u1VdpId].u2Height>>1 : _arMDDiPrm[u1VdpId].u2Height ,u2InputLine);

    if((E_TD_IN_TB_I==_arMDDiPrm[u1VdpId].eTDInput)
        ||(E_TD_IN_TB_P==_arMDDiPrm[u1VdpId].eTDInput)
        )
    {
        u2InputLine += IS_INTERLACE(u1VdpId) ? _arMDDiPrm[u1VdpId].u2Height>>1 : _arMDDiPrm[u1VdpId].u2Height ;
    }    

#ifdef CC_FPGA_SUPPORT
    if(IS_INTERLACE(u1VdpId) && IS_MIB_FRC(u1VdpId) && _arMDDiPrm[u1VdpId].u2Width < 1280)
    {
        u2InputLine = 2;
    }
#endif
    MDDI_WRITE_FLD(u1VdpId, MCVP_KC_00, u2InputLine, FRC_LINE);        
}

/**
 * @brief Set DI standard vmask for set resolution
 * @param u1VdpId VDP_1/VDP_2
 */
void vDrvDINonStdVMaskCfg(UINT8 u1VdpId)
{
    UINT8 u1OnOff = IS_INTERLACE(u1VdpId); 
    u1OnOff = u1OnOff & (!IS_3D_FMT_CONV(u1VdpId));
	u1OnOff = u1OnOff & (rMIBState[u1VdpId].u1SetResFinishedCnt ==0);
    MDDI_WRITE_FLD(u1VdpId, MCVP_KC_1E, u1OnOff, NON_STD_VMASK_EN);
}

/**
 * @brief Set DI auto stop
 * @param u1VdpId VDP_1/VDP_2
 * @param u1OnOff On/Off
 */
void vDrvDISetAutoStop(UINT8 u1VdpId, UINT8 u1OnOff)
{
    if(IO32ReadFldAlign(ADAPTIVE_PSCAN_REG, ADAPTIVE_PSCAN_MAIN) 
        || (IO32ReadFldAlign(ADAPTIVE_PSCAN_REG, ADAPTIVE_PSCAN_SUB)))
    {
        if(u4DrvTDTVIsDualFPI())
        {
            if(VDP_1==u1VdpId)
            {
                MDDI_WRITE_FLD(VDP_1, MCVP_KC_00, IS_INTERLACE(u1VdpId) ? u1OnOff : SV_OFF, SRCW_AUTO_STOP);
                MDDI_WRITE_FLD(VDP_2, MCVP_KC_00, IS_INTERLACE(u1VdpId) ? u1OnOff : SV_OFF, SRCW_AUTO_STOP);
            }
        }
        else
        {
            MDDI_WRITE_FLD(u1VdpId, MCVP_KC_00, IS_INTERLACE(u1VdpId) ? u1OnOff : SV_OFF, SRCW_AUTO_STOP);
        }    
    }
}

/**
 * @brief Set DI Clock Enable
 * @param u1VdpId VDP_1/VDP_2
 * @param u1OnOff On/Off
 */
void vDrvDISetClockEnable(UINT8 u1VdpId, UINT8 u1OnOff)
{
    if (u1VdpId == VDP_1)
    {
        vIO32WriteFldAlign(CKGEN_VPCLK_STOP, !u1OnOff, FLD_MAIN_CLK_STOP);
    }
    else
    {
        vIO32WriteFldAlign(CKGEN_VPCLK_STOP, !u1OnOff, FLD_PIP_CLK_STOP);
    }

    MDDI_WRITE_FLD(VDP_1, MCVP_DRAM_00, u1OnOff, DA_MAIN_NR_CK_EN);
    MDDI_WRITE_FLD(VDP_1, MCVP_DRAM_00, u1OnOff, DA_MAIN_SC_CK_EN);
    MDDI_WRITE_FLD(VDP_1, MCVP_DRAM_00, u1OnOff, DA_MAIN_RD_CK_EN);
    MDDI_WRITE_FLD(VDP_1, MCVP_DRAM_00, u1OnOff, DA_PIP_RD_CK_EN);

    MDDI_WRITE_FLD(VDP_1, MCVP_BC_00, u1OnOff, DRAM_CK_EN);
    MDDI_WRITE_FLD(VDP_1, MCVP_BC_00, u1OnOff, (u1VdpId == VDP_1) ? MCVP_K_M_CK_EN : MCVP_K_P_CK_EN);
    MDDI_WRITE_FLD(VDP_1, MCVP_BC_00, u1OnOff, MCVP_K_M_PQ_CK_EN);
}

/**
 * @brief Set pscan output clock
 * @param bPath VDP_1/VDP_2
 * @param eType clock type
 */
void vDrvDISetClock(UINT8 u1VdpId, HW_MIB_OUTPUT_CLK_TYPE eType)
{
    vIO32WriteFldAlign(CKGEN_REG_MIB_OCLK_CKCFG, eType, FLD_MIB_OCLK_SEL);
}

/**
 * @brief Set DI Error Defusion
 * @param u1VdpId VDP_1/VDP_2
 * @param u18bit 1: 8bit/ 0: 10bit
 */
void vDrvDISetErrDefusion(UINT8 u1VdpId, UINT8 u18bit)
{
    //Always turn off because of performance issue
    u18bit =SV_OFF;
    MDDI_WRITE_FLD(u1VdpId, MCVP_KC_00, u18bit, ERR_DFU_EN);
    MDDI_WRITE_FLD(u1VdpId, MCVP_KC_00, u18bit, ERR_DFU_RAND_INI_EN);    
}

/**
 * @brief Set bit resolution
 * @param u1VdpId VDP_1/VDP_2
 * @param u1BitRes Bit resolution (8 or 10)
 */

void vDrvDISetBitRes(UINT8 u1VdpId, UINT8 u18bit)
{
    vDrvDISetWritePrecision(u1VdpId, u18bit);
    vDrvDISetErrDefusion(u1VdpId, u18bit);

    if (u1VdpId == VDP_1)
        vDrvNRSetBitRes(u18bit);
}

/**
 * @brief Set DI DISPMODE_H_BLANK
 * @param u1VdpId VDP_1/VDP_2
 * @param u2Value set the value to DISPMODE_H_BLANK
 */
void vDrvDiSetDispmodeHBlank(UINT8 u1VdpId, UINT16 u2Value)
{
    MDDI_WRITE_FLD(u1VdpId, MCVP_KC_2B, u2Value, DISPMODE_H_BLANK); 
}

UINT32 u4DrvDiGetDispHBlank(UINT8 u1VdpId)
{
    return MDDI_READ_FLD(u1VdpId, MCVP_KC_2B, DISPMODE_H_BLANK); 
}

void vDrvDIDispmodeLRLockISR(void)
{
#if defined(CC_MT5399)

    #define ISR_TOGGLE_STB_CNT 4
    #define VALID_LR_CONFIG_CNT 4
    #define ISR_VALIDATION 40
    
    static UINT8 u1StableCnt=0;
    static UINT8 u1LockCnt=0;
    
    UINT32 u4MIBInDisp_r      =MDDI_READ_FLD(VDP_1, MCVP_KC_0D, IN_DISP_R);
    UINT32 u4MIBCurLineCheck  =MDDI_READ_FLD(VDP_1, MCVP_KC_2D, OUT_VDE_CNT);
        
    UINT32 u4SCLOutDisp_r     =IO32ReadFldAlign(MJC_STA_LR3D, MJC_STA_LR3D_INFO_I);
    UINT32 u4SCLDispRIsInv    =IO32ReadFldAlign(SCPIP_SCSYS_08, SCSYS_08_SC_INV_OUTPUT_DISP_R);    

    UINT32 u4SCLFrmTrackLock;
    UINT32 u4SCLFrmTrackTarget;
    UINT32 u4CurrMIBINV;
    
    if(u4MIBCurLineCheck>ISR_VALIDATION)
    {
        return;
    }
    
    u4SCLFrmTrackTarget = (IO32ReadFldAlign(SCPIP_SCSYS_15, SCSYS_15_SC_FRM_LOCK_WIN) 
        + IO32ReadFldAlign(SCPIP_SCSYS_15, SCSYS_15_SC_FRM_LOCK_TOR))*2;

    u4SCLFrmTrackLock = ( IO32ReadFldAlign(SCPIP_SCSYS_2B, SCSYS_2B_STA_FRM_TRK_ABS_DIS) 
        < u4SCLFrmTrackTarget);

    u1LockCnt = u4SCLFrmTrackLock ? MIN(u1LockCnt+1 , 0x80) : 0;
        
    if ((u1StableCnt > ISR_TOGGLE_STB_CNT) && (u1LockCnt == VALID_LR_CONFIG_CNT))
    {    
        u4SCLOutDisp_r = u4SCLDispRIsInv ? !u4SCLOutDisp_r:u4SCLOutDisp_r;
        u4CurrMIBINV = (u4MIBInDisp_r!=u4SCLOutDisp_r);

        u1StableCnt = 0;
        
        vIO32WriteFldAlign(SCPIP_SCSYS_11, !u4CurrMIBINV ,SCSYS_11_SC_INV_MIB_DISP_R);
        vIO32WriteFldAlign(SCPIP_SCSYS_08, ((!u4CurrMIBINV) != u1DrvMJCIsNeedInverseLR()),SCSYS_08_SC_INV_OUTPUT_DISP_R);         
    }
    else
    {
        u1StableCnt = MIN(u1StableCnt+1,0x80);
    }
#endif    
}
