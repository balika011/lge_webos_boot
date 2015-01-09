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
 * $RCSfile: drv_scpos.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

#include "general.h"
#include "video_def.h"
#include "drv_video.h"
#include "drv_dram.h"
#include "drv_scaler.h"
#include "drv_scaler_drvif.h"
#include "drv_scpos.h"
#include "scpos_debug.h"
#include "hw_ckgen.h"
#include "hw_mlvds.h"
#include "hw_sw.h"
#if defined(CC_MT5399)
#include "drv_mjc_if.h"
#endif
#include "drv_di.h"
#include "x_assert.h"

#ifdef SUPPORT_DRAM_DUMP
#define ADDR_STACK_SIZE 20

/**************************************************************************
 * Global/Static variables
 *************************************************************************/
//@6896 review this
#if 0
static UINT8  _u1StackSize = 0;
static UINT16  _au2AddrStack[ADDR_STACK_SIZE];
static UINT32  _au4ValueStack[ADDR_STACK_SIZE];
#endif

/**************************************************************************
 * Function Members
 *************************************************************************/
PRIVATE UINT8 bRecoverDRAMDumpStack(void);
//PRIVATE UINT8 bGetScalerDRAMData(UINT32 wX, UINT32 wY, UINT8 bIs10bitMode, UINT8 bIs444, UINT32 dwAddr, UINT32 wMHL, UINT32 *pu2VR, UINT32 *pu2UB, UINT32 *pu2YG);


// for dram dump
PRIVATE UINT8 bRecoverDRAMDumpStack(void)
{
    //@6896 review this
    #if 0
    if(_u1StackSize > ADDR_STACK_SIZE)
    {
        return SV_FALSE;
    }
    
    while (_u1StackSize > 0)
    {
        _u1StackSize --;

        if (_au2AddrStack[_u1StackSize] == 0xd2c4) //recover
        {
            vIO32Write4B(CKGEN_DISP_CKCFG, _au4ValueStack[_u1StackSize]);
        }
        else
        {
            vIO32Write4B(_au2AddrStack[_u1StackSize], _au4ValueStack[_u1StackSize]);
        }
    }
    vScpipSetDumpCtrl(SV_OFF);
    #endif
    
    return SV_TRUE;
}

#if defined(CC_MT5396)
EXTERN MJC_DRAM_ALLOC_T arMJCDramAlloc;
#endif
//EXTERN void _vScpipUpdateDramParameter(UINT32 u4VdpId, UINT32 u4Addr, UINT32 u4FrameSize, UINT32 u4FrameNum);
EXTERN void vScpipSetDispModeVar(UINT8 bPath, UINT8 u1Dispmode);

BOOL fgForceScalerDramDump(UINT32 u4VdpId, BOOL fgOnOff)
{
#if 0 //@6896 remove?
    static UINT8 bDramMode = 0xff;
    static UINT8 bIsMJCOn = 0xff;

    UNUSED(bIsMJCOn);
    
    // if pip or main is dram mode, do nothing
    if ((u4VdpId == VDP_2))
    {
        LOG(3, "Not support sub-path dump\n");
        return FALSE;
    }

    if (!fgOnOff) // dump disable
    {
        if(bDramMode == VDP_SCPOS_PSCAN_DISPMODE) // originally pscan display mode
        {
            #ifdef CC_MT5396
            if(bIsMJCOn == SV_ON)
            {
                // turn on MJC
                MJC_SetOnOff(SV_ON); 

                // scaler set to pscan display mode
                vScpipSetDispModeVar(SV_VP_MAIN, bDramMode); 

                // trigger mode change
                vSetScposFlg(MAIN_DISP_PRM_CHG);
                vSetScposFlg(SCALER_MAIN_MODE_CHG);    

                // TODO: porting for 5368/5396
                #if 0
                // turn off pscan special dump setting
                vDrvDISetPSCANDisplayForDump(VDP_1, SV_OFF);
                #endif
            }
            #endif
        }
        bIsMJCOn = 0xff;
        bDramMode = 0xff;
    }
    else //dump enable
    {
        bDramMode = getScalerMode(VDP_1);
        if (bDramMode == VDP_SCPOS_PSCAN_DISPMODE)
        {
            #ifdef CC_MT5396
            bIsMJCOn = u1DrvMJCIsOnOff();
            if(bIsMJCOn == SV_ON)
            {
                UINT32 u4FrameNum = 3;
                UINT32 u4FrameSize = (arMJCDramAlloc.u4EndAddr - arMJCDramAlloc.u4StartAddr)/u4FrameNum;
                
                // turn off mjc
                MJC_SetOnOff(SV_OFF);

                // set scaler to dram mode
//                _vScpipUpdateDramParameter(VDP_1, arMJCDramAlloc.u4StartAddr, u4FrameSize, u4FrameNum); // use mjc dram buffer
                vScpipSetDispModeVar(SV_VP_MAIN, VDP_SCPOS_DISPMODE_OFF);            
                vSetScposFlg(SCALER_MAIN_MODE_CHG);
                
                // TODO: porting for 5368/5396
                #if 0
                // turn on pscan special dump setting
                vDrvDISetPSCANDisplayForDump(VDP_1, SV_ON);
                #endif
            }
            else
            #endif
            {
                return FALSE;
            }
        }
    }

    if(fgOnOff == TRUE)
    {
        HAL_Delay_us(500000);
    }
#endif    
    return TRUE;
    
}


UINT8 bInitScalerDump(void)
{
//@6896 review this
#if 0
    //_u1StackSize = 0;

    UNUSED(_au2AddrStack);
    UNUSED(_au4ValueStack);

    if(u1ScpipGetDispMode(SV_VP_MAIN) != VDP_SCPOS_DISPMODE_OFF)
    {
        LOG(3, "Must enter dram mode to dump scaler dram!\n");
        return SV_FALSE;
    }
    else
    {
        /* Turn off main channel write to external memory */
        vScpipSetDumpCtrl(SV_ON);
        vUtDelay10ms(10);
        return SV_TRUE;
    }
#endif    
    return SV_FALSE;    
}


UINT8 bReleaseScalerDump(void)
{
    //@6896 review this
    #if 0
        vScpipSetDumpCtrl(SV_OFF);
    #endif
        return SV_TRUE;    
}

UINT8 bInitLastStageDump(void)
{
    //@6896 review this
    #if 0
    UINT32 dwFrameSize, dwWriteAddr;
    UINT32 wHTotal, wHL, wVL, wMoSyncLen, wVsync_Delsel2, Vs_width, wDatalen;
    UINT8 bIs10bitMode, bDSIs10bitMode, bDSIs444Mode;
    vScpipSetDumpCtrl(SV_ON);
    _u1StackSize = 0;
    /* 1. Set frame num to 1 */
    // TODO: FIXME!consider if we use fsel_dump
    _au2AddrStack[_u1StackSize] = SCPIP_PIP1_09;
    _au4ValueStack[_u1StackSize++] = u4IO32Read4B(SCPIP_PIP1_09);
    //vIO32WriteFldAlign(SCPIP_PIP1_09, 0, PIP1_09_FSEL_1);
    vScpipSetBufNum(VDP_1, 1);
 
    /* 2. Bypass down scaling */
    _au2AddrStack[_u1StackSize] = SCPIP_DS1_00;
    _au4ValueStack[_u1StackSize++] = u4IO32Read4B(SCPIP_DS1_00);
    vIO32WriteFldAlign(SCPIP_DS1_00, 0x8000, DS1_00_DSCALER_H_1);
    vIO32WriteFldAlign(SCPIP_DS1_00, 0x8000, DS1_00_DSCALER_V_1);
        
    /* 3. Freeze screen */
    _au2AddrStack[_u1StackSize] = SCPIP_PIP1_09;
    _au4ValueStack[_u1StackSize++] = u4IO32Read4B(SCPIP_PIP1_09);
    vScpipSetDramWrite(VDP_1,SV_OFF); 

    /* 4. Get color depth and Set to DUMP_VD10BSEL and DUMP_SEL444 */
    bDSIs10bitMode = IO32ReadFldAlign(SCPIP_DS1_02, DS1_02_VD10BSEL_1);
    bDSIs444Mode = IO32ReadFldAlign(SCPIP_DS1_02, DS1_02_SEL444_1);
    _au2AddrStack[_u1StackSize] = SCPIP_PIP1_11;
    _au4ValueStack[_u1StackSize++] = u4IO32Read4B(SCPIP_PIP1_11);
    vIO32WriteFldAlign(SCPIP_PIP1_11, bDSIs10bitMode, PIP1_11_R10B_SEL_DUMP);
    vIO32WriteFldAlign(SCPIP_PIP1_11, bDSIs444Mode, PIP1_11_SEL444_DUMP);

    /* 5. Set VL_DUMP and Data_Length_DUMP */
    _au2AddrStack[_u1StackSize] = SCPIP_PIP1_10;
    _au4ValueStack[_u1StackSize++] = u4IO32Read4B(SCPIP_PIP1_10);
    vIO32WriteFldAlign(SCPIP_PIP1_10, IO32ReadFldAlign(SCPIP_PIP1_00, PIP1_00_VL_1), PIP1_10_VL_DUMP);
    vIO32WriteFldAlign(SCPIP_PIP1_10, IO32ReadFldAlign(SCPIP_PIP1_08, PIP1_08_DATA_LENGTH_1), PIP1_10_DATA_LENGTH_DUMP);
    vUtDelay10ms(10);
    
    /* 6. TURN ON dump mode */
    _au2AddrStack[_u1StackSize] = SCPIP_PIP_OA_02;
    _au4ValueStack[_u1StackSize++] = u4IO32Read4B(SCPIP_PIP_OA_02);
    vIO32WriteFldAlign(SCPIP_PIP_OA_02, 1, PIP_OA_02_DUMP_EN);

    /* 7. Set new color depth */
    _au2AddrStack[_u1StackSize] = SCPIP_DS1_02;
    _au4ValueStack[_u1StackSize++] = u4IO32Read4B(SCPIP_DS1_02);
    bIs10bitMode = IO32ReadFldAlign(MLVDS_SO0, MLVDS_SO0_RG_10B_EN);
    vIO32WriteFldAlign(SCPIP_DS1_02, bIs10bitMode, DS1_02_VD10BSEL_1);
    vIO32WriteFldAlign(SCPIP_DS1_02, 1, DS1_02_SEL444_1);
    
   
    /* 8. Get HTotal and VTotal  and set HL , VL and Data_len*/
    wHTotal = IO32ReadFldAlign(SCPIP_PIP_OA_00, PIP_OA_00_PANEL_HTOTAL);
    wMoSyncLen  = IO32ReadFldAlign(SCPIP_PIP_OA_01, PIP_OA_01_MOSYNCLEN);
    wVsync_Delsel2 = IO32ReadFldAlign(OSTG_PTGEN_00, FLD_VSYNC_DELSEL2);
    Vs_width = IO32ReadFldAlign(OUTSTG_OS_18, OS_18_VS_WIDTH);
    wHTotal = wHTotal - wMoSyncLen - 1; // only keep active region
    _au2AddrStack[_u1StackSize] = SCPIP_PIP1_01;
    _au4ValueStack[_u1StackSize++] = u4IO32Read4B(SCPIP_PIP1_01);
    _au2AddrStack[_u1StackSize] = SCPIP_PIP1_00;
    _au4ValueStack[_u1StackSize++] = u4IO32Read4B(SCPIP_PIP1_00);
    _au2AddrStack[_u1StackSize] = SCPIP_PIP1_08;
    _au4ValueStack[_u1StackSize++] = u4IO32Read4B(SCPIP_PIP1_08);
    if (bIs10bitMode) // 10 bit
    {
        wHL = (wHTotal + 3)/4;
        vIO32WriteFldAlign(SCPIP_PIP1_01, wHL, PIP1_01_HL_1);
    }
    else // 8 bit
    {
        wHL = (wHTotal*3 + 15)/16;
        vIO32WriteFldAlign(SCPIP_PIP1_01, wHL, PIP1_01_HL_1);
    }
    
    wDatalen = wHL;

    vIO32WriteFldAlign(SCPIP_PIP1_08, wDatalen, PIP1_08_DATA_LENGTH_1);
    wVL = PANEL_GetPanelHeight();
  //  wVL = wVTotal + 1;
    vIO32WriteFldAlign(SCPIP_PIP1_00, wVL, PIP1_00_VL_1);
  

    /* 9. Set new write base address and WINC*/
    _au2AddrStack[_u1StackSize] = SCPIP_PIP1_07;
    _au4ValueStack[_u1StackSize++] = u4IO32Read4B(SCPIP_PIP1_07);
    dwWriteAddr = u4ScpipGetRAddr(VDP_1);
    dwFrameSize = IO32ReadFldAlign(SCPIP_PIP1_07, PIP1_07_WINC_1);
    dwWriteAddr = dwWriteAddr + dwFrameSize;
    _au2AddrStack[_u1StackSize] = SCPIP_PIP1_05;
    _au4ValueStack[_u1StackSize++] = u4IO32Read4B(SCPIP_PIP1_05);
    vScpipSetWAddr(VDP_1, dwWriteAddr);
    dwFrameSize = wDatalen*wVL; 
    vIO32WriteFldAlign(SCPIP_PIP1_07, dwFrameSize, PIP1_07_WINC_1);
    

    /* 10. Set new clock domain to DS */
     _au2AddrStack[_u1StackSize] = 0xd2c4;
    _au4ValueStack[_u1StackSize++] = u4IO32Read4B(CKGEN_DISP_CKCFG);
     //vIO32WriteFldAlign(CKGEN_DISP_CKCFG, 1, FLD_SCPIP_MAIN_CLK_SEL);
     #ifdef CC_MT5396
     vIO32WriteFldAlign(CKGEN_DISP_CKCFG, 0, FLD_POCLK_SEL);
     #endif
    /* 11. Set HPos and VPos */
    _au2AddrStack[_u1StackSize] = OUTSTG_OS_16;
    _au4ValueStack[_u1StackSize++] = u4IO32Read4B(OUTSTG_OS_16);
    vIO32WriteFldAlign(OUTSTG_OS_16, (0x800 + wMoSyncLen - 29), OS_16_H_POS);
    _au2AddrStack[_u1StackSize] = OUTSTG_OS_18;
    _au4ValueStack[_u1StackSize++] = u4IO32Read4B(OUTSTG_OS_18);
    vIO32WriteFldAlign(OUTSTG_OS_18, (0x400 + wVsync_Delsel2 - Vs_width - 1), OS_18_V_POS);

    /* 12. Turn on Main write */
    vScpipSetDramWrite(VDP_1,SV_ON);
    vUtDelay10ms(10);
    vScpipSetDramWrite(VDP_1,SV_OFF);
    vUtDelay10ms(10);
    vIO32WriteFldAlign(SCPIP_PIP_OA_02, 0, PIP_OA_02_DUMP_EN);
    
    /* 13.  set new read address */
    _au2AddrStack[_u1StackSize] = SCPIP_PIP1_06;
    _au4ValueStack[_u1StackSize++] = u4IO32Read4B(SCPIP_PIP1_06);
    vScpipSetRAddr(VDP_1, dwWriteAddr);
    /* 14.  set YUV2RGB_BYPASS_MAIN */
    _au2AddrStack[_u1StackSize] = YCBCR2RGB_00;
    _au4ValueStack[_u1StackSize++] = u4IO32Read4B(YCBCR2RGB_00);
    vIO32WriteFldAlign(YCBCR2RGB_00, 1, YUV2RGB_BYPASS_MAIN);
    
    /* 15. bypass us */
    _au2AddrStack[_u1StackSize] = SCPIP_US1_00;
    _au4ValueStack[_u1StackSize++] = u4IO32Read4B(SCPIP_US1_00);
    vIO32WriteFldAlign(SCPIP_US1_00, 0x8000, US1_00_USCALE_H_1);
    vIO32WriteFldAlign(SCPIP_US1_00, 0x8000, US1_00_USCALE_V_1);
    
    /* 16.  set new boudary */
    _au2AddrStack[_u1StackSize] = SCPIP_PIP1_0F;
    _au4ValueStack[_u1StackSize++] = u4IO32Read4B(SCPIP_PIP1_0F);
    vIO32WriteFldAlign(SCPIP_PIP1_0F, PANEL_GetPanelWidth(), PIP1_0F_BOUNDARY_H_POS_1);
    #endif
    return SV_TRUE;
}

void vScpipSetDumpCtrl(UINT8 bOnOff)
{
    //@6896 review this
    #if 0
    vIO32WriteFldAlign(SCALER_DUMP_00 ,bOnOff, SCALER_DUMP_EN);
    #endif
}

UINT8 bReleaseLastStageDump(void)
{
    //@6896 review this
    return bRecoverDRAMDumpStack();
}

UINT8 bGetScalerDRAMAverage(UINT32 u4X, UINT32 u4Y, UINT32 u4Width, UINT32 u4Height, UINT32 *pu4VR, UINT32 *pu4UB, UINT32 *pu4YG)
{
    //@6896 review this
    #if 0
    UINT32 u4TotalPixel;
    UINT32 u4XIndex, u4YIndex;
    UINT32 u4DataVR, u4DataUB, u4DataYG;
    UINT32 u4TotalVR=0, u4TotalUB=0, u4TotalYG=0;
    
    if(u1ScpipGetDispMode(SV_VP_MAIN) != VDP_SCPOS_DISPMODE_OFF)
    {
        LOG(3, "Must enter dram mode to dump scaler dram!\n");
        return SV_FALSE;
    }
    else if (vScpipGetDumpCtrl() == SV_OFF)
    {
        LOG(3, "Dump control must be turn on first!\n");
        return SV_FALSE;            
    }
    
    if((u4Width ==0) || (u4Height==0))
        return SV_FALSE;
    
    for(u4XIndex=0; u4XIndex<u4Width; u4XIndex++)
    {
        for(u4YIndex=0; u4YIndex<u4Height; u4YIndex++)
        {
            if(bGetScalerDRAM(u4X+u4XIndex, u4Y+u4YIndex, &u4DataVR, &u4DataUB, &u4DataYG)== SV_FALSE)
            {
                return SV_FALSE;
            }
            else
            {
                u4TotalVR +=u4DataVR;
                u4TotalUB +=u4DataUB;
                u4TotalYG +=u4DataYG;
            }
        }
    }

    u4TotalPixel= (UINT32)(u4Width * u4Height);
    if(IO32ReadFldAlign(SCPIP_DS1_02, DS1_02_VD10BSEL_1))
    {
        *pu4VR = (UINT32)(u4TotalVR/ u4TotalPixel);
        *pu4UB = (UINT32)(u4TotalUB/ u4TotalPixel);
        *pu4YG = (UINT32)(u4TotalYG/ u4TotalPixel);
    }
    else
    {
        *pu4VR = (UINT32)(u4TotalVR/ u4TotalPixel)<<2;
        *pu4UB = (UINT32)(u4TotalUB/ u4TotalPixel)<<2;
        *pu4YG = (UINT32)(u4TotalYG/ u4TotalPixel)<<2;
    }
return SV_TRUE;
    #endif
    return SV_FALSE;
}



UINT8 bGetScalerDRAM(UINT32 u2X, UINT32 u2Y, UINT32 *pu2VR, UINT32 *pu2UB, UINT32 *pu2YG)
{
    //@6896 review this
    #if 0
    UINT8 bIs10bitMode, bIs444;
    UINT32 dwAddr, u4HpsFactor;
    UINT32 wMHL;
    UINT32 u2XOffset, u2YOffset;
    UINT32 u2OverscanX, u2OverscanY;
    UINT32 dwDHFac, dwDVFac;

    bIs10bitMode = IO32ReadFldAlign(SCPIP_DS1_02, DS1_02_VD10BSEL_1);
    bIs444 = IO32ReadFldAlign(SCPIP_DS1_02, DS1_02_SEL444_1);

    // get Dump Address

    dwAddr = u4ScpipGetWAddr(VDP_1) << 4;    
    // get MHL
    //wMHL = IO32ReadFldAlign(SCPIP_PIP1_01, PIP1_01_HL_1);
    wMHL = IO32ReadFldAlign(SCPIP_PIP1_08, PIP1_08_DATA_LENGTH_1);

    // get x, y offset in DRAM
    u2OverscanX = IO32ReadFldAlign(SCPIP_PIP1_04, PIP1_04_HP_OFST_1);
    u2OverscanY = IO32ReadFldAlign(SCPIP_PIP1_04, PIP1_04_WSTARTP_OFST_1);
    u4HpsFactor = u4PDSGetHPSFactorReg(SV_VP_MAIN);
    u2XOffset = (UINT16)((UINT32)u2X * u4HpsFactor/DFT_DS_FACTOR);   // HPS

    if ((u2OverscanX > u2XOffset) || (u2OverscanY > u2Y))
    {
        // the target pixel is not in DRAM
        LOG(3, "Error: The target pixel is not in DRAM!!\n");
        return SV_FALSE;
    }
    u2XOffset -= u2OverscanX;
    u2YOffset = u2Y - u2OverscanY;

    // Down-scaling factor
    dwDHFac = IO32ReadFldAlign(SCPIP_DS1_00, DS1_00_DSCALER_H_1);
    dwDVFac = IO32ReadFldAlign(SCPIP_DS1_00, DS1_00_DSCALER_V_1);
    u2XOffset = (UINT16)((UINT32)u2XOffset * dwDHFac/DFT_DS_FACTOR);
    u2YOffset = (UINT16)((UINT32)u2YOffset * dwDVFac/DFT_DS_FACTOR);

    // Read DRAM
    if (bGetScalerDRAMData(u2XOffset, u2YOffset, bIs10bitMode, bIs444, dwAddr, wMHL, pu2VR, pu2UB, pu2YG) == SV_FALSE)
    {
        return SV_FALSE;
    }
    #endif

    return SV_TRUE;
}

UINT8 bGetLastStageDRAM(UINT32 u2X, UINT32 u2Y, UINT32 *pu2R, UINT32 *pu2B, UINT32 *pu2G)
{
    //@6896 review this
    #if 0
    UINT8 bIs10bitMode;
    UINT32 dwAddr;
    UINT32 wHL;// wHPos, wVPos;

    bIs10bitMode = IO32ReadFldAlign(MLVDS_SO0, MLVDS_SO0_RG_10B_EN);
    // get Dump Address

    dwAddr = u4ScpipGetWAddr(VDP_1) << 4;    
    
    // get SHL
    //wHL = IO32ReadFldAlign(SCPIP_PIP1_01, PIP1_01_HL_1);
    wHL = IO32ReadFldAlign(SCPIP_PIP1_08, PIP1_08_DATA_LENGTH_1);
    
    // Read DRAM
    if (bGetScalerDRAMData(u2X, u2Y, bIs10bitMode, 1, dwAddr, wHL, pu2R, pu2B, pu2G) == SV_FALSE)
    {
        return SV_FALSE;
    }
    #endif
    return SV_TRUE;
}

//@6896 review this
#if 0
PRIVATE UINT8 bGetScalerDRAMData(UINT32 wX, UINT32 wY, UINT8 bIs10bitMode, UINT8 bIs444, UINT32 dwAddr, UINT32 wMHL, UINT32 *pu2VR, UINT32 *pu2UB, UINT32 *pu2YG)
{
    UINT32 dwOffset, dwData;

    if ((pu2VR == NULL) || (pu2UB == NULL) || (pu2YG == NULL))
    {
        return SV_FALSE;
    }

    // 10 bit mode
    if (bIs10bitMode == 1)
    {
        // RGB
        if (bIs444)
        {
            LOG(3, "<RGB 10 bit Mode>\n");
            dwOffset = ((UINT32)wMHL*16*wY) + ((wX/4)*16);

            switch (wX%4)
            {
            case 3:
                dwOffset += 0;
                dwData = dwReadDramD(VIRTUAL(dwAddr+dwOffset)); // read a word
                *pu2VR = (dwData&0x3FF);
                *pu2UB = (dwData>>10)&0x3FF;
                *pu2YG = (dwData>>20)&0x3FF;
                break;

            case 2:
                dwOffset += 30/8;
                dwData = dwReadDramD(VIRTUAL(dwAddr+dwOffset)); // read a word
                *pu2VR = (dwData>>6)&0x3FF;
                *pu2UB = (dwData>>16)&0x3FF;
                dwOffset += 2;
                dwData = dwReadDramD(VIRTUAL(dwAddr+dwOffset)); // read a word
                *pu2YG = (dwData>>10)&0x3FF;
                break;

            case 1:
                dwOffset += (30*2)/8;
                dwData = dwReadDramD(VIRTUAL(dwAddr+dwOffset)); // read a word
                *pu2VR = (dwData>>4)&0x3FF;
                *pu2UB = (dwData>>14)&0x3FF;
                dwOffset += 3;
                dwData = dwReadDramD(VIRTUAL(dwAddr+dwOffset)); // read a word
                *pu2YG = (dwData&0x3FF);
                break;

            default:
                dwOffset += (30*3)/8;
                dwData = dwReadDramD(VIRTUAL(dwAddr+dwOffset)); // read a word
                *pu2VR = (dwData>>2)&0x3FF;
                *pu2UB = (dwData>>12)&0x3FF;
                *pu2YG = (dwData>>22)&0x3FF;
                break;
            }
        }

        // YCbCr
        else
        {
            LOG(3, "<YCbCr 10 bit Mode>\n");
            dwOffset = ((UINT32)wMHL*16*wY) + ((wX/6)*16);

            switch (wX%6)
            {
            case 5:
            case 4:
                dwOffset += 0;
                break;

            case 3:
            case 2:
                dwOffset += 5;
                break;

            default:
                dwOffset += 10;
                break;
            }
            dwData = dwReadDramD(VIRTUAL(dwAddr+dwOffset)); // read a word

            if ((wX%2) == 0) // Y0Cb0Cr0
            {
                *pu2VR = (dwData&0x3FF);
                dwOffset += 10/8;
                dwData = dwReadDramD(VIRTUAL(dwAddr+dwOffset)); // xY0Cb0Y1
                *pu2UB = (dwData>>12)&0x3FF;
                *pu2YG = (dwData>>22)&0x3FF;
            }
            else // Y1Cb0Cr0
            {
                *pu2VR = (dwData&0x3FF);
                *pu2YG = (dwData>>10)&0x3FF;
                *pu2UB = (dwData>>20)&0x3FF;
            }
        }
    }

    // 8bit mode
    else
    {
        // RGB
        if (bIs444)
        {
            LOG(3, "<RGB 8 bit Mode>\n");
            dwOffset = ((UINT32)wMHL*16*wY) + ((wX/16)*16*3);

            switch (wX%16)
            {
            case 0:
                dwData = dwReadDramD(VIRTUAL(dwAddr+dwOffset+12)); // read a word
                *pu2VR = (dwData>>8)&0xFF;
                *pu2UB = (dwData>>16)&0xFF;
                *pu2YG = (dwData>>24)&0xFF;
                break;

            case 1:
                dwData = dwReadDramD(VIRTUAL(dwAddr+dwOffset+10)); // read a word
                *pu2VR = (dwData&0xFF);
                *pu2UB = (dwData>>8)&0xFF;
                *pu2YG = (dwData>>16)&0xFF;
                break;

            case 2:
                dwData = dwReadDramD(VIRTUAL(dwAddr+dwOffset+7)); // read a word
                *pu2VR = (dwData&0xFF);
                *pu2UB = (dwData>>8)&0xFF;
                *pu2YG = (dwData>>16)&0xFF;
                break;

            case 3:
                dwData = dwReadDramD(VIRTUAL(dwAddr+dwOffset+4)); // read a word
                *pu2VR = (dwData&0xFF);
                *pu2UB = (dwData>>8)&0xFF;
                *pu2YG = (dwData>>16)&0xFF;
                break;

            case 4:
                dwData = dwReadDramD(VIRTUAL(dwAddr+dwOffset+1)); // read a word
                *pu2VR = (dwData&0xFF);
                *pu2UB = (dwData>>8)&0xFF;
                *pu2YG = (dwData>>16)&0xFF;
                break;

            case 5:
                dwData = dwReadDramD(VIRTUAL(dwAddr+dwOffset)); // read a word
                *pu2YG = (dwData&0xFF);
                dwData = dwReadDramD(VIRTUAL(dwAddr+dwOffset+28)); // read a word
                *pu2VR = (dwData>>16)&0xFF;
                *pu2UB = (dwData>>24)&0xFF;
                break;

            case 6:
                dwData = dwReadDramD(VIRTUAL(dwAddr+dwOffset+27)); // read a word
                *pu2VR = (dwData&0xFF);
                *pu2UB = (dwData>>8)&0xFF;
                *pu2YG = (dwData>>16)&0xFF;
                break;

            case 7:
                dwData = dwReadDramD(VIRTUAL(dwAddr+dwOffset+24)); // read a word
                *pu2VR = (dwData&0xFF);
                *pu2UB = (dwData>>8)&0xFF;
                *pu2YG = (dwData>>16)&0xFF;
                break;

            case 8:
                dwData = dwReadDramD(VIRTUAL(dwAddr+dwOffset+21)); // read a word
                *pu2VR = (dwData&0xFF);
                *pu2UB = (dwData>>8)&0xFF;
                *pu2YG = (dwData>>16)&0xFF;
                break;

            case 9:
                dwData = dwReadDramD(VIRTUAL(dwAddr+dwOffset+18)); // read a word
                *pu2VR = (dwData&0xFF);
                *pu2UB = (dwData>>8)&0xFF;
                *pu2YG = (dwData>>16)&0xFF;
                break;

            case 10:
                dwData = dwReadDramD(VIRTUAL(dwAddr+dwOffset+16)); // read a word
                *pu2UB = (dwData&0xFF);
                *pu2YG = (dwData>>8)&0xFF;
                dwData = dwReadDramD(VIRTUAL(dwAddr+dwOffset+44)); // read a word
                *pu2VR = (dwData>>24)&0xFF;
                break;

            case 11:
                dwData = dwReadDramD(VIRTUAL(dwAddr+dwOffset+44)); // read a word
                *pu2VR = (dwData&0xFF);
                *pu2UB = (dwData>>8)&0xFF;
                *pu2YG = (dwData>>16)&0xFF;
                break;

            case 12:
                dwData = dwReadDramD(VIRTUAL(dwAddr+dwOffset+41)); // read a word
                *pu2VR = (dwData&0xFF);
                *pu2UB = (dwData>>8)&0xFF;
                *pu2YG = (dwData>>16)&0xFF;
                break;

            case 13:
                dwData = dwReadDramD(VIRTUAL(dwAddr+dwOffset+38)); // read a word
                *pu2VR = (dwData&0xFF);
                *pu2UB = (dwData>>8)&0xFF;
                *pu2YG = (dwData>>16)&0xFF;
                break;

            case 14:
                dwData = dwReadDramD(VIRTUAL(dwAddr+dwOffset+35)); // read a word
                *pu2VR = (dwData&0xFF);
                *pu2UB = (dwData>>8)&0xFF;
                *pu2YG = (dwData>>16)&0xFF;
                break;

            case 15:
                dwData = dwReadDramD(VIRTUAL(dwAddr+dwOffset+32)); // read a word
                *pu2VR = (dwData&0xFF);
                *pu2UB = (dwData>>8)&0xFF;
                *pu2YG = (dwData>>16)&0xFF;
                break;
            }
        }

        // YCbCr
        else
        {
            LOG(3, "<YCbCr 10 bit Mode>\n");
            dwOffset = ((UINT32)wMHL*16*wY) + ((wX/8)*16);

            switch (wX%8)
            {
            case 7:
            case 6:
                dwOffset += 0;
                break;

            case 5:
            case 4:
                dwOffset += 4;
                break;

            case 3:
            case 2:
                dwOffset += 8;
                break;

            default:
                dwOffset += 12;
                break;
            }
            dwData = dwReadDramD(VIRTUAL(dwAddr+dwOffset)); // read a word

            if ((wX%2) == 0) // Y0Cb0Cr0
            {
                *pu2VR = (dwData&0xFF);
                *pu2UB = (dwData>>16)&0xFF;
                *pu2YG = (dwData>>24)&0xFF;
            }
            else // Y1Cb0Cr0
            {
                *pu2VR = (dwData&0xFF);
                *pu2YG = (dwData>>8)&0xFF;
                *pu2UB = (dwData>>16)&0xFF;
            }
        }
    }

    return SV_TRUE;
}
#endif

#endif

