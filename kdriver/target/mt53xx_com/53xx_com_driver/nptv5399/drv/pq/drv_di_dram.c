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
 * $Date: 2015/04/01 $
 * $RCSfile: drv_di.c,v $
 * $Revision: #3 $
 *
 *---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 *
 * MIB Help 264 Decoder to Write DRAM
 *
 *---------------------------------------------------------------------------*/


////////////////////////////////////////////////////////////////////////////////
// Include files
////////////////////////////////////////////////////////////////////////////////
#include "hw_di.h"
#include "hw_sw.h"
#include "hw_ycproc.h"

#include "vdo_misc.h"

#include "drv_di.h"
#include "drv_di_int.h"
#include "drv_tdnavi.h"
#include "fbm_drvif.h"
#include "srm_drvif.h"

#include "x_assert.h"
//#include "drv_predown.h"

#ifndef min
#define min(a,b) (a < b ? a : b)
#endif

#ifndef max
#define max(a,b) (a > b ? a : b)
#endif
typedef struct
{
    UINT32 u4HMsbPgNum;
    UINT32 u4VMsbYPgNum;
    UINT32 u4VMsbCPgNum;
    UINT32 u4HLsbPgNum;
    UINT32 u4VLsbYPgNum;
    UINT32 u4VLsbCPgNum;
} MDDI_PAGE_NUM_T;

MDDI_PAGE_NUM_T rPgNum[VDP_NS];

/**
 * @brief Set DI Buffer Address
 * @param u1VdpId VDP_1/VDP_2
 * @param u4AddrYMsb/u4AddrYLsb/u4AddrUMsb/u4AddrULsb/u4AddrVMsb/u4AddrVLsb/u4AddrMVFlg/u4AddrLMFlg
 */
static inline void vDrvDISetBufferAddr(UINT8 u1VdpId, 
    UINT32 u4AddrYMsb, UINT32 u4AddrYLsb, 
    UINT32 u4AddrUMsb, UINT32 u4AddrULsb, 
    UINT32 u4AddrVMsb, UINT32 u4AddrVLsb, 
    UINT32 u4AddrMVFlg,UINT32 u4AddrLMFlg,UINT32 u4AddrRNRFlg)
{
#ifndef CC_FPGA_SUPPORT 
    MDDI_WRITE_FLD(u1VdpId , MCVP_DRAM_0A , u4AddrYMsb    >>11 , DA_ADDR_BASE_MSB_Y);
    MDDI_WRITE_FLD(u1VdpId , MCVP_DRAM_0B , u4AddrYLsb    >>11 , DA_ADDR_BASE_LSB_Y);
    MDDI_WRITE_FLD(u1VdpId , MCVP_DRAM_0C , u4AddrUMsb    >>11 , DA_ADDR_BASE_MSB_U);
    MDDI_WRITE_FLD(u1VdpId , MCVP_DRAM_0D , u4AddrULsb    >>11 , DA_ADDR_BASE_LSB_U);
    MDDI_WRITE_FLD(u1VdpId , MCVP_DRAM_0E , u4AddrVMsb    >>11 , DA_ADDR_BASE_MSB_V);
    MDDI_WRITE_FLD(u1VdpId , MCVP_DRAM_0F , u4AddrVLsb    >>11 , DA_ADDR_BASE_LSB_V);
    MDDI_WRITE_FLD(u1VdpId , MCVP_DRAM_02 , u4AddrMVFlg   >>4  , DA_FLAG_MV_ADDR);
    MDDI_WRITE_FLD(u1VdpId , MCVP_DRAM_03 , u4AddrLMFlg   >>4  , DA_FLAG_LM_ADDR);
    MDDI_WRITE_FLD(u1VdpId , MCVP_DRAM_04 , u4AddrRNRFlg  >>4  , DA_FLAG_RNR_ADDR);
#endif
}

/**
 * @brief Set DI Buffer Protect
 * @param u1VdpId VDP_1/VDP_2
 * @param u4Lo/u4Hi/u4FlgLo/u4FlgHi limit
 */
static inline void vDrvDISetBufferProtect(UINT8 u1VdpId, UINT32 u4Lo, UINT32 u4Hi, UINT32 u4FlgLo, UINT32 u4FlgHi)
{
#ifndef CC_FPGA_SUPPORT
    MDDI_WRITE_FLD(u1VdpId , MCVP_DRAM_10 , u4Hi >>11  , DA_WADDR_HI_LIMIT);
    MDDI_WRITE_FLD(u1VdpId , MCVP_DRAM_11 , u4Lo >>11  , DA_WADDR_LO_LIMIT);
    MDDI_WRITE_FLD(u1VdpId , MCVP_DRAM_05 , u4FlgHi>>4 , DA_FLAG_WADDR_HI_LIMIT);
    MDDI_WRITE_FLD(u1VdpId , MCVP_DRAM_06 , u4FlgLo>>4 , DA_FLAG_WADDR_LO_LIMIT);
#endif
}

/**
 * @brief Set DI Frame Number
 * @param u1VdpId VDP_1/VDP_2
 * @param u1YNum/u1UNum/u1VNum
 */
static inline void vDrvDISetFrmNum(UINT8 u1VdpId, UINT8 u1YNum, UINT8 u1UNum, UINT8 u1VNum)
{
    MDDI_WRITE_FLD(u1VdpId, MCVP_DRAM_08, u1YNum, DA_FRAME_NUM_Y);
    MDDI_WRITE_FLD(u1VdpId, MCVP_DRAM_08, u1UNum, DA_FRAME_NUM_U);
    MDDI_WRITE_FLD(u1VdpId, MCVP_DRAM_08, u1VNum, DA_FRAME_NUM_V);	
}


/**
 * @brief Set DI Dram dram number
 * @param u1VdpId VDP_1/VDP_2
 */
static inline void _vDrvDISetFrameNum(UINT8 u1VdpId)
{
    UINT8 u1YFieldNum, u1CFieldNum;

    switch (_arMDDiPrm[u1VdpId].eDataMode)
    {    
        //Scaler FRC
        case E(I, NR_Y   ,DI_OFF   ,FRC_SCL):
        case E(I, NR_YC  ,DI_OFF   ,FRC_SCL):
        //MIB FRC	
        case E(I, NR_Y   ,DI_OFF   ,FRC_MIB):
        case E(I, NR_YC  ,DI_OFF   ,FRC_MIB):
        	u1YFieldNum = 2;
            u1CFieldNum = 2;
        	break;
        case E(I, NR_OFF ,DI_Y1C1  ,FRC_SCL):
            u1YFieldNum = 0;
            u1CFieldNum = 0;
            break;
        case E(I, NR_OFF ,DI_Y2C1  ,FRC_SCL):	
        case E(I, NR_OFF ,DI_Y2C1  ,FRC_MIB):
			u1YFieldNum = 2;
            u1CFieldNum = 1;
        	break;
        case E(I, NR_OFF ,DI_Y2C2  ,FRC_SCL):	
        case E(I, NR_OFF ,DI_Y2C2  ,FRC_MIB):
			u1YFieldNum = 2;
            u1CFieldNum = 2;
			break;
        //Scaler FRC    
        case E(I, NR_OFF ,DI_Y4C1  ,FRC_SCL)://This Mode only used under Sub            
        case E(I, NR_YC  ,DI_Y4C1  ,FRC_SCL):
        case E(I, NR_YC  ,DI_Y4C2  ,FRC_SCL):
        //Scaler MIB
        case E(I, NR_OFF ,DI_Y3C1  ,FRC_MIB):
        case E(I, NR_OFF ,DI_Y3C2  ,FRC_MIB):
        case E(I, NR_OFF ,DI_Y4C1  ,FRC_MIB):
        case E(I, NR_Y   ,DI_Y4C1  ,FRC_MIB):
        case E(I, NR_YC  ,DI_Y4C1  ,FRC_MIB):
        case E(I, NR_OFF ,DI_Y4C2  ,FRC_MIB):
        case E(I, NR_Y   ,DI_Y4C2  ,FRC_MIB):
        case E(I, NR_YC  ,DI_Y4C2  ,FRC_MIB):
            u1YFieldNum = 4;
            u1CFieldNum = 3;
            break;
        //Scaler FRC    
        case E(I, NR_OFF ,DI_Y4C4  ,FRC_SCL)://This Mode only used under sub
        case E(I, NR_YC  ,DI_Y4C4  ,FRC_SCL):
        //Scaler MIB
        case E(I, NR_OFF ,DI_Y4C4  ,FRC_MIB):
        case E(I, NR_Y   ,DI_Y4C4  ,FRC_MIB):
        case E(I, NR_YC  ,DI_Y4C4  ,FRC_MIB):
            u1YFieldNum = 4;
            u1CFieldNum = 4;
            break;
        case E(I, NR_OFF ,DI_Y1C1  ,FRC_MIB):	
            if(rMIBState[u1VdpId].u1DataModeIdx!=rMIBState[u1VdpId].u1BandwidthDataModeIdx ||
               IS_PR_MODE(u1VdpId))
            { //BandWidth Savinf Mode, DRAM Size Enough       
                u1YFieldNum = 3;
            	u1CFieldNum = 3;
            }
            else
            {
            	u1YFieldNum = 1;
            	u1CFieldNum = 1;
            }
			break;
        /////////////////  progressive
        //Scaler FRC
        case E(P,NR_OFF   ,DI_OFF ,FRC_SCL):
            u1YFieldNum = 0;
            u1CFieldNum = 0;
            break;            
        case E(P,NR_Y     ,DI_OFF ,FRC_SCL):
            u1YFieldNum = 1;
            u1CFieldNum = 0;
            break;         
        case E(P,NR_YC    ,DI_OFF ,FRC_SCL):
        //MIB FRC
        case E(P,NR_OFF   ,DI_OFF ,FRC_MIB):      
        case E(P,NR_Y     ,DI_OFF ,FRC_MIB):       
        case E(P,NR_YC    ,DI_OFF ,FRC_MIB):
            u1YFieldNum = 1;
            u1CFieldNum = 1;
            break;            
        default:
            u1YFieldNum = 0;
            u1CFieldNum = 0;
            break;
    }
    
    if(IS_FLIP_MIRROR(u1VdpId))
    {
        u1YFieldNum++;
        u1CFieldNum++;
    }
    
    if(IS_MIB_FRC(u1VdpId))
    {
        //Frame Number usage can be reduced if input frame rate = output frame rate,
        //And not flip mode
        u1YFieldNum++;
        u1CFieldNum++;
        if (SRM_IsEpgMode())
        {
            u1YFieldNum++;
            u1CFieldNum++;
        }
    }

#ifdef CC_FPGA_SUPPORT
    if (IS_INTERLACE(u1VdpId) & &IS_MIB_FRC(u1VdpId) && _arMDDiPrm[u1VdpId].u2Width < 1280)//SD
    {
        u1YFieldNum = 8;
        u1CFieldNum = 8;
    }
    else
    {
        u1YFieldNum++;
        u1CFieldNum++;
    }
#endif

    //Set the number of frame buffer
    u1YFieldNum = u1YFieldNum ? (u1YFieldNum - 1) : 0;
    u1CFieldNum = u1CFieldNum ? (u1CFieldNum - 1) : 0;
    vDrvDISetFrmNum(u1VdpId, u1YFieldNum, u1CFieldNum, u1CFieldNum);
}

/**
 * @brief Calculate Msb page number
 * @param u1VdpId VDP_1/VDP_2
 * @param u2Width
 * @param u2Height
 * @param u1YNum
 * @param u1CNum
 */
static void _vDrvDICalMsbPageNum(UINT8 u1VdpId, UINT32 u4Width, UINT32 u4Height, UINT8 u1YNum, UINT8 u1CNum)
{
    UINT32 u4VMsbCPgNum;
    
    rPgNum[u1VdpId].u4HMsbPgNum = MPY_AND_CEIL(u4Width, 1, 256)*256;
    rPgNum[u1VdpId].u4VMsbYPgNum = MPY_AND_CEIL((u4Height * u1YNum), 1, 8)*8;

    u4VMsbCPgNum = MPY_AND_CEIL((u4Height * u1CNum), 1, 8)*8;
    
    if(IS_420TO422(u1VdpId))
    {
        u4VMsbCPgNum = MPY_AND_CEIL(u4VMsbCPgNum, 1, 2);
        u4VMsbCPgNum = MPY_AND_CEIL(u4VMsbCPgNum, 1, 8)*8;
    }
    rPgNum[u1VdpId].u4VMsbCPgNum = u4VMsbCPgNum;
}

/**
 * @brief Calculate Lsb page number
 * @param u1VdpId VDP_1/VDP_2
 * @param u2Width
 * @param u2Height
 * @param u1YNum
 * @param u1CNum
 */
static void _vDrvDICalLsbPageNum(UINT8 u1VdpId, UINT32 u4Width, UINT32 u4Height, UINT8 u1YNum, UINT8 u1CNum)
{
    UINT8 u18bit = (_arMDDiPrm[u1VdpId].u1BitRes == 8) ? 1 : 0; 
    
    rPgNum[u1VdpId].u4HLsbPgNum = u18bit ? 0 : (MPY_AND_CEIL(u4Width, 1, 256)*256);

    rPgNum[u1VdpId].u4VLsbYPgNum = u18bit ? 0 : (MPY_AND_CEIL((u4Height * u1YNum), 1, 256)*256)/4;

    rPgNum[u1VdpId].u4VLsbCPgNum = u18bit ? 0 : (MPY_AND_CEIL(
        IS_420TO422(u1VdpId) ?  MPY_AND_CEIL(u4Height*u1CNum, 1, 2) : u4Height* u1CNum , 1, 256)*256)/4;    
}

/**
 * @brief Calculate buffer address 
 * @param u1VdpId VDP_1/VDP_2
 * @param u2Width
 * @param u2Height
 * @param u4StartAddr
 * @param u4EndAddr
 */
static void _vDrvDICalBufferAddress(UINT8 u1VdpId, UINT32 u4Width, UINT32 u4Height, 
        UINT32 u4StartAddr, UINT32 u4EndAddr)
{
    UINT16 u1AlignBit1, u1AlignBit2;
    UINT32 u4YAddrLsb, u4UAddrMsb, u4UAddrLsb, u4VAddrMsb, u4VAddrLsb;
    UINT32 u4FlgMVAddr,u4FlgRNRAddr, u4FlgLMAddr, u4FlgEndAddr;
    UINT32 u4FlgMVPerLine,u4FlagRNRPerLine, u4FlgLMPerLine;

    UINT32 u4u4HMsbPgNum = rPgNum[u1VdpId].u4HMsbPgNum;
    UINT32 u4u4HLsbPgNum = rPgNum[u1VdpId].u4HLsbPgNum;
    
    UINT32 u4VMsbCPgNum = rPgNum[u1VdpId].u4VMsbCPgNum;
    UINT32 u4VLsbCPgNum = rPgNum[u1VdpId].u4VLsbCPgNum;

    // Calculate data address
    u4YAddrLsb = MAKE_ALIGN_TO((u4StartAddr + (u4u4HMsbPgNum * rPgNum[u1VdpId].u4VMsbYPgNum))
        , 14, ((u4StartAddr%(1<<14)) +(2<<11))%(1<<14));
    u4UAddrMsb = MAKE_ALIGN_TO((u4YAddrLsb + (u4u4HLsbPgNum * rPgNum[u1VdpId].u4VLsbYPgNum))
        , 14, ((u4StartAddr%(1<<14)) +(4<<11))%(1<<14));
    u4UAddrLsb = MAKE_ALIGN_TO((u4UAddrMsb + (u4u4HMsbPgNum * u4VMsbCPgNum))
        , 14, ((u4StartAddr%(1<<14)) +(6<<11))%(1<<14));

    if ((bIsScalerInput444(u1VdpId) && (IS_PROG_DISPMODE(u1VdpId))))
    {
        u4VAddrMsb = MAKE_ALIGN_TO((u4UAddrLsb + (u4u4HLsbPgNum * u4VLsbCPgNum))
            , 14, ((u4StartAddr%(1<<14))+(0<<11))%(1<<14));
        u4VAddrLsb = MAKE_ALIGN_TO((u4VAddrMsb + (u4u4HMsbPgNum * u4VMsbCPgNum))
            , 14, ((u4StartAddr%(1<<14))+(2<<11))%(1<<14));
    }
    else
    {
        u4VAddrMsb = u4UAddrMsb;
        u4VAddrLsb = u4UAddrLsb;
    }

    u4FlgMVAddr = (u4VAddrLsb + (u4u4HLsbPgNum * u4VLsbCPgNum));

    // Calculate flag address    

    // Motion Vector Flag
    if (IS_3D_FMT_CONV(u1VdpId))
    {

        u4FlgRNRAddr = u4FlgMVAddr ;        
        u4FlgLMAddr = u4FlgRNRAddr ;        
        u4FlgEndAddr = u4FlgLMAddr ;                    
    }
    else
    {
        u1AlignBit1 = IS_OVERSAMPLE(u1VdpId) ? 512 : 256;
        u1AlignBit2 = IS_OVERSAMPLE(u1VdpId) ? 16 : 8;
        u4FlgMVPerLine = MPY_AND_CEIL(u4Width, 1, u1AlignBit1) * u1AlignBit1 / u1AlignBit2;
        u4FlgMVPerLine = (VDP_1==u1VdpId) ? u4FlgMVPerLine : 0;
        u4FlgRNRAddr = u4FlgMVAddr + MPY_AND_CEIL((u4FlgMVPerLine * u4Height * 16 / 8), 1, 16) * 16;        

        //RNR Flag   
        u4FlagRNRPerLine = MPY_AND_CEIL(u4Width, 1, u1AlignBit1) * u1AlignBit1/ 4;
        u4FlagRNRPerLine = ((VDP_1==u1VdpId)&& (IS_INTERLACE(VDP_1) || IS_MIB_FRC(VDP_1)))? u4FlagRNRPerLine : 0;
        u4FlgLMAddr = u4FlgRNRAddr + MPY_AND_CEIL((u4FlagRNRPerLine * u4Height * 16 / 8), 1, 16) * 16;        
        
        // Local Motion Flag

        u4FlgLMPerLine = MPY_AND_CEIL(u4Width, 1, u1AlignBit1) * u1AlignBit1 / 8;
        u4FlgLMPerLine = (IS_INTERLACE(VDP_1)&&(VDP_1==u1VdpId)) ?u4FlgLMPerLine : 0;
        u4FlgEndAddr = u4FlgLMAddr + MPY_AND_CEIL((u4FlgLMPerLine * MPY_AND_CEIL(u4Height, 1, 4) * 16 / 8), 1, 16) * 16;                    

    }

    u4FlgEndAddr = min(u4FlgEndAddr,u4EndAddr);
    u4FlgMVAddr  = min(u4FlgMVAddr, u4EndAddr);

    vDrvDISetBufferProtect(u1VdpId, u4StartAddr, u4FlgMVAddr, u4FlgMVAddr, u4FlgEndAddr);

    vDrvDISetBufferAddr(u1VdpId, 
            u4StartAddr, u4YAddrLsb, u4UAddrMsb, u4UAddrLsb,u4VAddrMsb, u4VAddrLsb, 
            u4FlgMVAddr, u4FlgLMAddr,u4FlgRNRAddr);    
}

/**
 * @brief Set DI Buffer
 * @param prFbmPool fbm configure
 */
void vDrvDISetFrameBuf(UINT8 u1VdpId)
{
    UINT8 u1YFldNum, u1CFldNum;
    UINT32 u4Width, u4Height;
    UINT32 u4StartAddr =  _arMDDiPrm[u1VdpId].u4BufAddr;
    UINT32 u4EndAddr   =  _arMDDiPrm[u1VdpId].u4BufAddr + _arMDDiPrm[u1VdpId].u4BufSize;

    PATH_CHECK(u1VdpId);
    
#ifdef CC_FPGA_SUPPORT
    u4Width = (_arMDDiPrm[u1VdpId].u2Width + MDDI_LINE_LENGHT_UNIT - 1);
#else
    u4Width = (_arMDDiPrm[u1VdpId].u2BufWidth + MDDI_LINE_LENGHT_UNIT -1) / MDDI_LINE_LENGHT_UNIT * MDDI_LINE_LENGHT_UNIT;        
#endif
    u4Height = _arMDDiPrm[u1VdpId].u2BufHeight;

    //TODO: Should be fixed, confict with predown and FBM configuration
    //u4Width=MAX(u4Width,_arMDDiPrm[u1VdpId].u2SigWidth);

    u4Width = MIN(2048,u4Width);

    u4Height = IS_INTERLACE(u1VdpId) ? (u4Height>>1) :u4Height;
    u4Height = IS_420TO422(u1VdpId) ? (((u4Height+1)>>1)<<1) : u4Height;
    
    MDDI_WRITE_FLD(u1VdpId, MCVP_DRAM_09,MIN(u4Width /MDDI_LINE_LENGHT_UNIT,0xF), DA_DRAM_LINE_LENGTH);     

    vDrvDISetBitRes(u1VdpId, (_arMDDiPrm[u1VdpId].u1BitRes == 8) ? 1 : 0);

    _vDrvDISetFrameNum(u1VdpId);

    u1YFldNum = MDDI_READ_FLD(u1VdpId, MCVP_DRAM_08, DA_FRAME_NUM_Y) + 1;
    u1CFldNum = MDDI_READ_FLD(u1VdpId, MCVP_DRAM_08, DA_FRAME_NUM_U) + 1;

    if(IS_FRAME_SEQ(u1VdpId)||IS_3D_FMT_CONV(u1VdpId))
    {
        u1YFldNum*=2;
        u1CFldNum*=2;      
    }

    _vDrvDICalMsbPageNum(u1VdpId, u4Width, u4Height, u1YFldNum, u1CFldNum);
    _vDrvDICalLsbPageNum(u1VdpId, u4Width, u4Height, u1YFldNum, u1CFldNum);
    _vDrvDICalBufferAddress(u1VdpId, u4Width, u4Height, u4StartAddr, u4EndAddr);
}


#define DRAM_ALIGN_WIDTH 256
#define DRAM_ALIGN_HEIGHT 8
EXTERN void HalFlushInvalidateDCacheSingleLine(UINT32 u4Addr);
EXTERN void HalInvalidateDCacheMultipleLine(UINT32 u4Addr, UINT32 u4Size);

void vDrvDIDumpDramForTDNAVI(UINT8 u1VdpId, UINT16 x, UINT16 y, UINT16 width, UINT16 height,UINT32 *pu4Data)
{
    UINT32 u4Addr = MDDI_READ_FLD(u1VdpId,MCVP_DRAM_0A, DA_ADDR_BASE_MSB_Y) << 11;
    UINT32 u4Pitch = MDDI_READ_FLD(u1VdpId ,MCVP_DRAM_09, DA_DRAM_LINE_LENGTH)*MDDI_LINE_LENGHT_UNIT;
    UINT32 u4FrmNum  = MDDI_READ_FLD(u1VdpId ,MCVP_DRAM_08, DA_FRAME_NUM_Y)+1;

	UINT32 u4HPageMsb;
	UINT32 blkNumMsb, lineNumMsb, posMsb;
    UINT32 u4Tmp;
    UINT32 i,j;

    if(MDDI_READ_FLD(u1VdpId,MCVP_MULTI_00, EN_NRW)==SV_OFF)
    {
        return;
    }

    width = width &(~0x3);
    u4HPageMsb = (u4Pitch+DRAM_ALIGN_WIDTH-1)/DRAM_ALIGN_WIDTH;
    
    for(i = y; i < y+height ; i++)
    {
        u4Tmp = i * u4FrmNum / DRAM_ALIGN_HEIGHT * u4HPageMsb;
        lineNumMsb = i *   u4FrmNum % DRAM_ALIGN_HEIGHT;
        
        for(j = x ; j < x + width ; j+=4)
        {   
           blkNumMsb = u4Tmp + j /DRAM_ALIGN_WIDTH;
           posMsb = j % DRAM_ALIGN_WIDTH;            
            *pu4Data++ =  *(UINT32 *)VIRTUAL((u4Addr + blkNumMsb*DRAM_ALIGN_WIDTH*DRAM_ALIGN_HEIGHT + lineNumMsb*DRAM_ALIGN_WIDTH + posMsb));
        }
    }

}

void vDrvDIFlushDramForTDNAVI(UINT8 u1VdpId, UINT16 x, UINT16 y, UINT16 width, UINT16 height)
{  
    UINT32 u4Addr = MDDI_READ_FLD(u1VdpId,MCVP_DRAM_0A, DA_ADDR_BASE_MSB_Y) << 11;
    UINT32 u4Pitch = MDDI_READ_FLD(u1VdpId ,MCVP_DRAM_09, DA_DRAM_LINE_LENGTH)*MDDI_LINE_LENGHT_UNIT;
    UINT32 u4FrmNum  = MDDI_READ_FLD(u1VdpId ,MCVP_DRAM_08, DA_FRAME_NUM_Y)+1;
    
    UINT32 u4AddrTmp;
    UINT32 u4Step;
    UINT32 u4HPageMsb;
    UINT32 blkNumMsb;
    UINT32 u4Tmp;
    UINT32 i;

    if(MDDI_READ_FLD(u1VdpId,MCVP_MULTI_00, EN_NRW)==SV_OFF)
    {
        return;
    }

    u4Step = u4FrmNum==1 ? 8 :
             u4FrmNum==2 ? 4 :
             u4FrmNum==3 ? 2 :
             u4FrmNum==4 ? 2 : 1;   
    u4HPageMsb = (u4Pitch+DRAM_ALIGN_WIDTH-1)/DRAM_ALIGN_WIDTH;

    for(i = y&(~(DRAM_ALIGN_HEIGHT-1)); i < y+height ; i+=u4Step)
    {
        u4Tmp = i * u4FrmNum/ DRAM_ALIGN_HEIGHT * u4HPageMsb;
        blkNumMsb = u4Tmp + x/DRAM_ALIGN_WIDTH ;          
        u4AddrTmp = VIRTUAL(u4Addr + blkNumMsb*DRAM_ALIGN_WIDTH*DRAM_ALIGN_HEIGHT);
        HalInvalidateDCacheMultipleLine(u4AddrTmp,DRAM_ALIGN_WIDTH*DRAM_ALIGN_HEIGHT*2);
    }
}

////////////////////////////////////////////////////////////////////////////////
//DI DRAM Dump for NCSTool
////////////////////////////////////////////////////////////////////////////////


static UINT32 u4SnkMp, u4SnkMpSC, u4SnkMpNR;

void vDrvDIDumpDisSnakeMapping(UINT8 bPath)
{   
	u4SnkMpNR = MDDI_READ_FLD(bPath, MCVP_DRAM_14, DA_NR_SNAKE_MAPPING);
	u4SnkMp   = MDDI_READ_FLD(bPath, MCVP_DRAM_08, DA_SNAKE_MAPPING);
	u4SnkMpSC = MDDI_READ_FLD(bPath, MCVP_DRAM_18, DA_SC_SNAKE_MAPPING);  

	MDDI_WRITE_FLD(bPath, MCVP_DRAM_14, 0, DA_NR_SNAKE_MAPPING);
	MDDI_WRITE_FLD(bPath, MCVP_DRAM_08, 0, DA_SNAKE_MAPPING);
	MDDI_WRITE_FLD(bPath, MCVP_DRAM_18, 0, DA_SC_SNAKE_MAPPING);    
    
} 

void vDrvDIDumpSetSize(UINT8 bPath) 
{
    UINT8 bIsInterlace = MDDI_READ_FLD(bPath, MCVP_KC_00, I2P_EN);
    UINT32 u4ReqWidth = IO32ReadFldAlign(SWRW_07, DUMP_REQ_WIDTH);
    UINT32 u4ReqHeight = IO32ReadFldAlign(SWRW_07, DUMP_REQ_HEIGHT);  
    UINT32 u4ImgWidth = MDDI_READ_FLD(bPath, MCVP_KC_0A, HDEW);
    UINT32 u4ImgHeight = MDDI_READ_FLD(bPath, MCVP_KC_0A, VDEW);  
    UINT32 u4StartW = IO32ReadFldAlign(SWRW_06, DUMP_CTRL_START_X);
    UINT32 u4StartH = IO32ReadFldAlign(SWRW_06, DUMP_CTRL_START_Y);
    UINT32 u4DumpSize;
    FBM_POOL_T* prPool;    
    
    if (bIsInterlace || (bPath == VDP_2)) // sub path always interlace, progresive will bypass Pscan
    {
        u4ImgHeight = u4ImgHeight/2;
    }
    if(_arMDDiPrm[bPath].u2SigWidth==0||_arMDDiPrm[bPath].u2SigHeight==0)
    {
        LOG(0,"vDrvDIDumpSetSize->input resolution = 0\n");
        return;
    }
    //if((u4PDSGetHPDSFactor(bPath) != 0x8000)||(u4PDSGetVPDSFactor(bPath) != 0x8000))
    u4StartW=u4ImgWidth*u4StartW/_arMDDiPrm[bPath].u2SigWidth;
    u4StartH=u4ImgHeight*u4StartH/_arMDDiPrm[bPath].u2SigHeight;
    
    // not define request width height, use full size
    u4ReqWidth = (u4ReqWidth == 0) ? u4ImgWidth : u4ReqWidth;
    u4ReqHeight = (u4ReqHeight == 0) ? u4ImgHeight : u4ReqHeight;        

    // dump offset range protection
    u4StartW = (u4StartW > u4ImgWidth) ? u4ImgWidth : u4StartW;
    u4StartH = (u4StartH > u4ImgHeight) ? u4ImgHeight : u4StartH;

    if ((u4StartW+u4ReqWidth) > u4ImgWidth)
    {
        u4ReqWidth = u4ImgWidth-u4StartW;
    }

    if ((u4StartH+u4ReqHeight) > u4ImgHeight)
    {
        u4ReqHeight = u4ImgHeight-u4StartH;
    }    
    
    u4DumpSize = u4ReqWidth*u4ReqHeight*4;

    LOG(4, "PSCAN Dump Start \n");    
    LOG(4, "Pos W %d  H %d  Length W %d  H %d  Size %d \n", 
        u4StartW, u4StartH, u4ReqWidth, u4ReqHeight, u4DumpSize);
    
    prPool = FBM_GetPoolInfoAuto((UCHAR)FBM_POOL_TYPE_VSS, NULL);

    // in case there is no suitable space allocated in FBM, add new way to force allocate (known) unused space for dump usage
    if(u4IO32Read4B(SWRW_08) != 0)
    {
        prPool->u4Addr = u4IO32Read4B(SWRW_08);
    }
        
    if ((u4DumpSize > prPool->u4Size) || (u4DumpSize == 0))
    {
        LOG(0, "PSCAN Dump buffer too small  W %d  H %d  Size %d  FBM size %d\n", 
            u4ReqWidth, u4ReqHeight, u4DumpSize, prPool->u4Size);        
        vIO32WriteFldAlign(SWRW_04, DUMP_ENG_NONE, DUMP_CTRL_ENG_SEL);
        vIO32WriteFldAlign(SWRW_04, DUMP_STATE_INIT, DUMP_CTRL_STATE);
        return;
    }
    
    vIO32Write4B(SWRW_01, prPool->u4Addr);
    vIO32Write4B(SWRW_02, prPool->u4Size);

    vIO32WriteFldAlign(SWRW_03, u4ReqWidth, DUMP_CTRL_WIDTH);
    vIO32WriteFldAlign(SWRW_03, u4ReqHeight, DUMP_CTRL_HEIGHT);
    vIO32WriteFldAlign(SWRW_03, DUMP_FMT_YCBCR_444_10BIT, DUMP_CTRL_FORMAT);    
    vIO32WriteFldAlign(SWRW_04, u4DumpSize, DUMP_CTRL_SIZE); 
    
}

void vDrvDIDumpInit(UINT8 bPath)
{
    vDrvDIDumpSetSize(bPath);
    vDrvDIDumpDisSnakeMapping(bPath);      
}

void vDrvDIDumpStart(UINT8 bPath)
{
    UINT8* Y_MSB_Addr = (UINT8 *)VIRTUAL(MDDI_READ_FLD(bPath, MCVP_DRAM_0A, DA_ADDR_BASE_MSB_Y)<<11);
    UINT8* Y_LSB_Addr = (UINT8 *)VIRTUAL(MDDI_READ_FLD(bPath, MCVP_DRAM_0B, DA_ADDR_BASE_LSB_Y)<<11);
    UINT8* U_MSB_Addr = (UINT8 *)VIRTUAL(MDDI_READ_FLD(bPath, MCVP_DRAM_0C, DA_ADDR_BASE_MSB_U)<<11);
    UINT8* U_LSB_Addr = (UINT8 *)VIRTUAL(MDDI_READ_FLD(bPath, MCVP_DRAM_0D, DA_ADDR_BASE_LSB_U)<<11);
    UINT8* V_MSB_Addr = (UINT8 *)VIRTUAL(MDDI_READ_FLD(bPath, MCVP_DRAM_0E, DA_ADDR_BASE_MSB_V)<<11);
    //UINT8* V_LSB_Addr = (UINT8 *)VIRTUAL(MDDI_READ_FLD(bPath, MCVP_DRAM_0F, DA_ADDR_BASE_LSB_V)<<11);
    UINT8 bYFrmNum = MDDI_READ_FLD(bPath, MCVP_DRAM_08, DA_FRAME_NUM_Y);
    UINT8 bCFrmNum = MDDI_READ_FLD(bPath, MCVP_DRAM_08, DA_FRAME_NUM_U);
    UINT8 bIs10bit = MDDI_READ_FLD(bPath, MCVP_DRAM_08, DA_BIT_SEL_WY) ? 0 : 1; 
    UINT8 bIs444 = MDDI_READ_FLD(bPath, MCVP_KC_1E, M444);     
	UINT8 bIsRGB = bIsScalerInputRGB(bPath); 
	UINT8 bIsInterlace = MDDI_READ_FLD(bPath, MCVP_KC_00, I2P_EN);
    UINT32 u4DramWidth = MDDI_READ_FLD(bPath, MCVP_DRAM_09, DA_DRAM_LINE_LENGTH)*MDDI_LINE_LENGHT_UNIT;
	UINT32 u4SrcWidth = IO32ReadFldAlign(SWRW_03, DUMP_CTRL_WIDTH);
    UINT32 u4SrcHeight = IO32ReadFldAlign(SWRW_03, DUMP_CTRL_HEIGHT);
    UINT32 u4StartW = IO32ReadFldAlign(SWRW_06, DUMP_CTRL_START_X);
    UINT32 u4StartH = IO32ReadFldAlign(SWRW_06, DUMP_CTRL_START_Y);
    UINT32* pBuf = (UINT32 *)VIRTUAL(u4IO32Read4B(SWRW_01));
    
    UINT32 u4HPxlPerPgMsb = 1 << 8;
    UINT32 u4VPxlPerPgMsb = 1 << 3;  	
	UINT32 u4HPxlPerPgLsb = 64*4; // 2bit data
    UINT32 u4VPxlPerPgLsb = 32;  
	UINT32 HPageMsb = (u4DramWidth+u4HPxlPerPgMsb-1)/u4HPxlPerPgMsb;
	UINT32 HPageLsb = (u4DramWidth+u4HPxlPerPgLsb-1)/u4HPxlPerPgLsb;  
	UINT32 blkNumMsb, lineNumMsb, posMsb;
	UINT32 blkNumLsb, lineNumLsb, posLsb, Lsbshf, pLsbM16;
    UINT32 Y0 = 0, Y1 = 0, Cb = 0, Cr = 0;
    UINT32 YMsb, YLsb, CMsb, CLsb;
    UINT32 VR, YG, UB;
    UINT32 x, y;
    UINT32 u4EvenStartW, u4EvenEndW;

    vDrvDIFreezeOnOff(bPath, SV_ON);
    
    if ((bYFrmNum == 0) && bIsInterlace)
    {
        LOG(0, "PSCAN 0W0R mode");
        return;
    }
    else
    {
        bYFrmNum++;
        bCFrmNum++;
    }
    
    if (u4StartW%2 != 0)
    {
        u4EvenStartW = u4StartW - 1;
    }
    else
    {
        u4EvenStartW = u4StartW;
    }

    if ((u4SrcWidth+u4StartW)%2 != 0)//odd
    {
        u4EvenEndW = u4SrcWidth+u4StartW+1;
    }
    else
    {
        u4EvenEndW = u4SrcWidth+u4StartW;    
    }
    
    if (bIs444 == 0)
    {   
        for (y=u4StartH; y < (u4SrcHeight+u4StartH); y++)
        {
            for (x=u4EvenStartW; x < (u4EvenEndW); x++)
            {
                blkNumMsb = y*bYFrmNum/u4VPxlPerPgMsb*HPageMsb+x/u4HPxlPerPgMsb;
                lineNumMsb = y*bYFrmNum%u4VPxlPerPgMsb;

                blkNumLsb = y*bYFrmNum/u4VPxlPerPgLsb*HPageLsb+x/u4HPxlPerPgLsb;
                lineNumLsb = y*bYFrmNum%u4VPxlPerPgLsb;
                
                posMsb = x%u4HPxlPerPgMsb;          
                posLsb = x%u4HPxlPerPgLsb;  
                pLsbM16 = posLsb % 16;
	            posLsb = posLsb - pLsbM16 + ((3 - (pLsbM16 >> 2)) << 2) + (posLsb & 3);
                Lsbshf = (x & 3) << 1;
                
                
                YMsb = (*(Y_MSB_Addr+blkNumMsb*2048+lineNumMsb*u4HPxlPerPgMsb+posMsb))<<2;               
                CMsb = (*(U_MSB_Addr+blkNumMsb*2048+lineNumMsb*u4HPxlPerPgMsb+posMsb))<<2;    
                
                if (bIs10bit)
                {
                    YLsb = ((*(Y_LSB_Addr+blkNumLsb*2048+lineNumLsb*u4HPxlPerPgLsb/4+posLsb))>>Lsbshf)&0x3; 
                    CLsb = ((*(U_LSB_Addr+blkNumLsb*2048+lineNumLsb*u4HPxlPerPgLsb/4+posLsb))>>Lsbshf)&0x3; 
                }
                else
                {                   
                    YLsb = 0;   
                    CLsb = 0;   
                }
                
                if ((x % 2) == 0)
                {
                    Y0 = YMsb+YLsb;
                    Cb = CMsb+CLsb; 
                }
                else
                {
                    Y1 = YMsb+YLsb;
                    Cr = CMsb+CLsb; 
                    if (x != u4StartW)
                    {
                        pBuf[(y-u4StartH)*u4SrcWidth+(x-u4StartW)-1] = (Y0<<20) | (Cr<<10) | Cb;
                    }
                    if (x != u4SrcWidth+u4StartW)
                    {
                        pBuf[(y-u4StartH)*u4SrcWidth+(x-u4StartW)] = (Y1<<20) | (Cr<<10) | Cb; 
                    }
                }
            }
        }
        
        vIO32WriteFldAlign(SWRW_03, 3, DUMP_CTRL_FORMAT);    
    }
    else
    {       
        for (y=u4StartH; y < (u4SrcHeight+u4StartH); y++)
        {
            for (x=u4StartW; x < (u4SrcWidth+u4StartW); x++)
            {
                blkNumMsb = y*bYFrmNum/u4VPxlPerPgMsb*HPageMsb+x/u4HPxlPerPgMsb;
                lineNumMsb = y*bYFrmNum%u4VPxlPerPgMsb;
                
                posMsb = x%u4HPxlPerPgMsb;          

                YG = (*(Y_MSB_Addr+blkNumMsb*2048+lineNumMsb*u4HPxlPerPgMsb+posMsb));         
                UB = (*(U_MSB_Addr+blkNumMsb*2048+lineNumMsb*u4HPxlPerPgMsb+posMsb));               
                VR = (*(V_MSB_Addr+blkNumMsb*2048+lineNumMsb*u4HPxlPerPgMsb+posMsb));
		if (bIsRGB)
		{
                    pBuf[(y-u4StartH)*u4SrcWidth+(x-u4StartW)] = (VR<<20) | (YG<<10) | UB;
		}
		else
		{
		    pBuf[(y-u4StartH)*u4SrcWidth+(x-u4StartW)] = (YG<<20) | (VR<<10) | UB;
		} 
            }
        }
        
        vIO32WriteFldAlign(SWRW_03, 1, DUMP_CTRL_FORMAT);    
    }    
}

void vDrvDIDumpEnd(UINT8 bPath)
{
    MDDI_WRITE_FLD(bPath, MCVP_DRAM_14, u4SnkMpNR, DA_NR_SNAKE_MAPPING); 
    MDDI_WRITE_FLD(bPath, MCVP_DRAM_08, u4SnkMp, DA_SNAKE_MAPPING); 
    MDDI_WRITE_FLD(bPath, MCVP_DRAM_18, u4SnkMpSC, DA_SC_SNAKE_MAPPING); 
    vDrvDIFreezeOnOff(bPath, SV_OFF);
}

void vDrvDISetDumpRegion(UINT8 u1VdpId,UINT32 u4StartX, UINT32 u4StartY, UINT32 u4Width, UINT32 u4Height)
{
    UINT32 u4ImgWidth = MDDI_READ_FLD(u1VdpId, MCVP_KC_0A, HDEW);
    UINT32 u4ImgHeight = MDDI_READ_FLD(u1VdpId, MCVP_KC_0A, VDEW); 

    if(_arMDDiPrm[u1VdpId].u2SigWidth==0||_arMDDiPrm[u1VdpId].u2SigHeight==0)
    {
        LOG(0,"vDrvDISetDumpRegion->input resolution = 0\n");
        return;
    }
    // if((u4PDSGetHPDSFactor(bPath) != 0x8000)||(u4PDSGetVPDSFactor(bPath) != 0x8000))
    u4StartX=u4ImgWidth*u4StartX/_arMDDiPrm[u1VdpId].u2SigWidth;
    u4StartY=u4ImgHeight*u4StartY/_arMDDiPrm[u1VdpId].u2SigHeight;
    
    vIO32WriteFldAlign(SWRW_07, u4Width, DUMP_REQ_WIDTH);
    vIO32WriteFldAlign(SWRW_07, u4Height, DUMP_REQ_HEIGHT);  
    vIO32WriteFldAlign(SWRW_06, u4StartX, DUMP_CTRL_START_X);
    vIO32WriteFldAlign(SWRW_06, u4StartY, DUMP_CTRL_START_Y);
}

void vDrvDIWritePixelValue(UINT8 bPath, UINT16 x, UINT16 y, DI_BUFFER_DATA* value)
{
    UINT8* Y_MSB_Addr = (UINT8 *)VIRTUAL(MDDI_READ_FLD(bPath, MCVP_DRAM_0A, DA_ADDR_BASE_MSB_Y)<<11);
    UINT8* Y_LSB_Addr = (UINT8 *)VIRTUAL(MDDI_READ_FLD(bPath, MCVP_DRAM_0B, DA_ADDR_BASE_LSB_Y)<<11);
    UINT8* U_MSB_Addr = (UINT8 *)VIRTUAL(MDDI_READ_FLD(bPath, MCVP_DRAM_0C, DA_ADDR_BASE_MSB_U)<<11);
    UINT8* U_LSB_Addr = (UINT8 *)VIRTUAL(MDDI_READ_FLD(bPath, MCVP_DRAM_0D, DA_ADDR_BASE_LSB_U)<<11);
    UINT8* V_MSB_Addr = (UINT8 *)VIRTUAL(MDDI_READ_FLD(bPath, MCVP_DRAM_0E, DA_ADDR_BASE_MSB_V)<<11);
    //UINT8* V_LSB_Addr = (UINT8 *)VIRTUAL(MDDI_READ_FLD(bPath, MCVP_DRAM_0D, DA_ADDR_BASE_LSB_V)<<11);

    UINT8 bYFrmNum = MDDI_READ_FLD(bPath, MCVP_DRAM_08, DA_FRAME_NUM_Y);
    UINT8 bCFrmNum = MDDI_READ_FLD(bPath, MCVP_DRAM_08, DA_FRAME_NUM_U);
    UINT8 bIs10bit = MDDI_READ_FLD(bPath, MCVP_DRAM_08, DA_BIT_SEL_WY) ? 0 : 1; 
    UINT8 bIs444 = MDDI_READ_FLD(bPath, MCVP_KC_1E, M444);     
    UINT8 bIsInterlace = MDDI_READ_FLD(bPath, MCVP_KC_00, I2P_EN);
    UINT32 u4DramWidth = MDDI_READ_FLD(bPath, MCVP_DRAM_09, DA_DRAM_LINE_LENGTH)*MDDI_LINE_LENGHT_UNIT;

    UINT32 u4HPxlPerPgMsb = 1 << 8;
    UINT32 u4VPxlPerPgMsb = 1 << 3;  	
    UINT32 u4HPxlPerPgLsb = 64*4; // 2bit data
    UINT32 u4VPxlPerPgLsb = 32;  
    UINT32 HPageMsb = (u4DramWidth+u4HPxlPerPgMsb-1)/u4HPxlPerPgMsb;
    UINT32 HPageLsb = (u4DramWidth+u4HPxlPerPgLsb-1)/u4HPxlPerPgLsb;  
    UINT32 blkNumMsb, lineNumMsb, posMsb, TmpLineNum;
    UINT32	blkNumLsb, lineNumLsb, posLsb, Lsbshf, pLsbM16;
    UINT32 YMsb, YLsb, CMsb, CLsb;		
    UINT32 Y, C;
    UINT8 u1Y,u1C,u1Clr; 
    UINT32 i;		
		
    if ((bYFrmNum == 0) && bIsInterlace)
    {
        LOG(0, "PSCAN 0W0R mode");
        return;
    }
    else 
    {
        bYFrmNum++;
        bCFrmNum++;
    }

    if (bIs444 == 0)
    {			
    	blkNumMsb = y*bYFrmNum/u4VPxlPerPgMsb*HPageMsb+x/u4HPxlPerPgMsb;
       lineNumMsb = y*bYFrmNum%u4VPxlPerPgMsb;
	
       blkNumLsb = y*bYFrmNum/u4VPxlPerPgLsb*HPageLsb+x/u4HPxlPerPgLsb;
       lineNumLsb = y*bYFrmNum%u4VPxlPerPgLsb;

	posMsb = x%u4HPxlPerPgMsb;
       posLsb = x%u4HPxlPerPgLsb;  
	pLsbM16 = posLsb % 16;
	posLsb = posLsb - pLsbM16 + (3 - pLsbM16/4)*4 + posLsb%4;
	Lsbshf = (x%4)*2;

       Y = value->u4YG ;
       C = value->u4UB;
              
       YLsb = Y & 0x3;
       CLsb = C & 0x3;

       YMsb = Y >> 2;
       CMsb = C >> 2;

       for (i = 0; i < bYFrmNum ; i ++)
       {
	    //write MSB
	    TmpLineNum = (lineNumMsb + i) % u4VPxlPerPgMsb;
           *(Y_MSB_Addr+blkNumMsb*2048+TmpLineNum*u4HPxlPerPgMsb+posMsb) = (UINT8)YMsb; 
           *(U_MSB_Addr+blkNumMsb*2048+TmpLineNum*u4HPxlPerPgMsb+posMsb) = (UINT8)CMsb;
  
	    if (bIs10bit)
	    {
                u1Clr = ~(0x3 << Lsbshf);
		  TmpLineNum = (lineNumLsb + i) % u4VPxlPerPgMsb;
		
                u1Y = *(Y_LSB_Addr+blkNumLsb*2048+TmpLineNum*u4HPxlPerPgLsb/4+posLsb); 
                u1C = *(U_LSB_Addr+blkNumLsb*2048+TmpLineNum*u4HPxlPerPgLsb/4+posLsb); 
		   
                YLsb = (YLsb << Lsbshf);
                CLsb = (CLsb << Lsbshf);

                u1Y &= u1Clr;
                u1C &= u1Clr;
                u1Y |= YLsb;
                u1C |= CLsb;  

                *(Y_LSB_Addr+blkNumLsb*2048+TmpLineNum*u4HPxlPerPgLsb/4+posLsb) = u1Y; 
                *(U_LSB_Addr+blkNumLsb*2048+TmpLineNum*u4HPxlPerPgLsb/4+posLsb) = u1C; 
            }
        }		
    }
    else
    {	        
        blkNumMsb = y*bYFrmNum/u4VPxlPerPgMsb*HPageMsb+x/u4HPxlPerPgMsb;
        lineNumMsb = y*bYFrmNum%u4VPxlPerPgMsb;	
        posMsb = x%u4HPxlPerPgMsb;			

        TmpLineNum = (lineNumMsb) % u4VPxlPerPgMsb;

       *(Y_MSB_Addr+blkNumMsb*2048+TmpLineNum*u4HPxlPerPgMsb+posMsb) = (UINT8)value->u4YG;         
       *(U_MSB_Addr+blkNumMsb*2048+TmpLineNum*u4HPxlPerPgMsb+posMsb) = (UINT8)value->u4UB;               
       *(V_MSB_Addr+blkNumMsb*2048+TmpLineNum*u4HPxlPerPgMsb+posMsb) = (UINT8)value->u4VR;
        
    }
}


