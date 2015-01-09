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
 * $RCSfile: drv_di.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/
/** @file drv_di.h
 *  Brief of file drv_di.h.
 *  Details of file drv_di.h (optional).
 */

#ifndef DRV_DI_H
#define DRV_DI_H


//////////////////////////////////////////////////////////////////////////////// 
// Include files
//////////////////////////////////////////////////////////////////////////////// 

#include "x_typedef.h"
#include "general.h"
#include "feature.h"
#include "drv_tdtv_drvif.h"
#include "vdp_if.h"
#include "vdo_rm.h"
////////////////////////////////////////////////////////////////////////////////
// FW PORTING CFG , SHOULD BE REMOVE ON IC
////////////////////////////////////////////////////////////////////////////////

#define MIB_PORTING_TODO (0)

// workaround define
#define MIB_LI_INPUT_WA
#define MIB_COBRA_ECO1_PIP_GARBAGE
#define MIB_FLIP_MIRROR_COLOR_SHIFT_WA
#define MIB_WIDTH_PREDOWN_LOW_256_WA
#define MIB_PROG_IN_SYNC_WA  

//counter number define
#define MIB_STABLE_MAX_CNT                  (255)
#define MIB_STABLE_CNT_RESTART_NUM          (128)
#define MIB_SET_RES_FINISH_CNT              (0xB)
#define MIB_SET_RES_TIME_OUT                (0xF)
#define MIB_STATUS_CNT_MASK                 (0x1F)
#define DRAM_REFILL_DELAY                   (6)
#ifdef MIB_PROG_IN_SYNC_WA
#define MIB_PROG_IN_WA_STABLE_CNT           (0x4)
#endif

#define MDDI_REG_NUM            0x800
#define MDDI_LINE_LENGHT_UNIT   256
//for progressive
#define MIB_DISP_HBLANK_MIN       120
//for interlace
#define MIB_DISPMODE_HBLANK       0x80
#define MIB_PREFETCH_KDELOWMIN    2

#define MIB_SD_K_DE_LOW_MIN     (4) //576P K_DE_LOW_MIN

#define MPY_AND_TRUN(value, x, y) \
	(((UINT32)(value) * (UINT32)(x)) / (UINT32)(y))

#define MPY_AND_CEIL(value, x, y) \
	(((UINT32)(value) * (UINT32)(x) + (UINT32)(y) - (UINT32)1) / (UINT32)(y))

#define MAKE_ALIGN(value, align) \
	(MPY_AND_CEIL((value), 1, (align)) * (align))

#define MAKE_ALIGN_TO(value, align, fix) \
	((((value-((value>>align)<<align))>fix)) \
	? ((((value>>align)+1)<<align)+fix) \
	: (((value>>align)<<align)+fix))
	
#define MPY_RATIO_SUM(value, x, y) \
	((x + y*(value-1) + (value/2)) / value)

#define MDDI_READ_FLD(u1VdpId, u2Addr, u4Fld) \
     IO32ReadFldAlign((u2Addr + ((u1VdpId == VDP_1) ? 0 : MDDI_REG_NUM)), u4Fld)

#ifndef LOG_MDDI_REG
#define MDDI_WRITE_FLD(u1VdpId, u2Addr, u4Value, u4Fld) \
    vIO32WriteFldAlign((u2Addr + ((u1VdpId == VDP_1) ? 0 : MDDI_REG_NUM)), u4Value, u4Fld)

#define MDDI_WRITE32_MSK(u1VdpId, u2Addr, u4Value,  u4Msk) \
    vIO32Write4BMsk((u2Addr + ((u1VdpId == VDP_1) ? 0 : MDDI_REG_NUM)), u4Value, u4Msk)
#else
void MDDI_WRITE_FLD(UINT8 u1VdpId, UINT16 u2Addr, UINT32 u4Value, UINT32 u4Fld);
void MDDI_WRITE32_MSK(UINT8 u1VdpId, UINT16 u2Addr, UINT32 u4Value, UINT32 u4Msk);
#endif

#define NUM_OF_PATH (SV_VP_PIP+1)

#define PATH_CHECK(x) \
    do{ ASSERT(x <= NUM_OF_PATH); \
        x = x >= NUM_OF_PATH ? SV_VP_MAIN : x; }while(0)

#define MIB_NR_MODE(u1Id)                ((E_MIB_NR_DMODE)((rMIBState[u1Id].eBandwidthDataMode>>8)&0xF))
    
#define MIB_DI_MODE(u1Id)                ((E_MIB_DI_DMODE)((rMIBState[u1Id].eBandwidthDataMode>>4)&0xF))

#define MIB_FRC_MODE(u1Id)               ((E_MIB_FRC_DMODE)((_arMDDiPrm[u1Id].eDataMode)&0xF))

#define IS_INTERLACE(u1Id)               ( _arMDDiPrm[u1Id].u1Interlace)

#define IS_PROGRESSIVE(u1Id)             ( !_arMDDiPrm[u1Id].u1Interlace)

#define IS_POP(u1Id)                     ( _arMDDiPrm[u1Id].u1IsPOP)

#define IS_DQ(u1Id)                      ( _arMDDiPrm[u1Id].u1IsDQ)

#define IS_420TO422(u1Id)                ( _arMDDiPrm[u1Id].u1420Mode)

#define IS_FLIP_MIRROR(u1Id)             ( _arMDDiPrm[u1Id].u1FlipMorrorByMIB)

#define IS_NR_ON(u1Id)                   ((MIB_NR_MODE(u1Id) != E_MIB_NR_OFF)&&(E_MIB_DMODE_UNKNOWN!=_arMDDiPrm[u1Id].eDataMode))

#define IS_DI_ON(u1Id)                   ((MIB_DI_MODE(u1Id) != E_MIB_DI_OFF)&&(E_MIB_DMODE_UNKNOWN!=_arMDDiPrm[u1Id].eDataMode))

#define IS_DI_BOB(u1Id)                  ((MIB_DI_MODE(u1Id) == E_MIB_DI_Y1C1)&&(E_MIB_DMODE_UNKNOWN!=rMIBState[u1Id].eBandwidthDataMode))

#define IS_DI_Y4C1(u1Id)                 ((MIB_DI_MODE(u1Id) == E_MIB_DI_Y4C1)&&(E_MIB_DMODE_UNKNOWN!=rMIBState[u1Id].eBandwidthDataMode))

#define IS_DI_Y4C2(u1Id)                 ((MIB_DI_MODE(u1Id) == E_MIB_DI_Y4C2)&&(E_MIB_DMODE_UNKNOWN!=rMIBState[u1Id].eBandwidthDataMode))

#define IS_DI_Y4C4(u1Id)                 ((MIB_DI_MODE(u1Id) == E_MIB_DI_Y4C4)&&(E_MIB_DMODE_UNKNOWN!=rMIBState[u1Id].eBandwidthDataMode))

#define IS_MIB_FRC(u1Id)                 ((MIB_FRC_MODE(u1Id) == E_MIB_FRC_MIB)&&(E_MIB_DMODE_UNKNOWN!=rMIBState[u1Id].eBandwidthDataMode))

//#define IS_COBRA_ECO_1_IC(void)          ((IS_IC_5398()&&(BSP_GetIcVersion() == IC_VER_5398_AB)))

#define IS_OVERSAMPLE(u1Id)              (bDrvVideoGetWidthOverSample(u1Id) &&( _arMDDiPrm[u1Id].u2Width > 720) &&( _arMDDiPrm[u1Id].u2Width <= 1440))

#define IS_SD_HEIGHT(u1Id)               ((_arMDDiPrm[u1Id].u2Height <= 576)&& \
                                         ((_arMDDiPrm[u1Id].u2HTotal <= 864) || bDrvVideoGetWidthOverSample(u1Id) || IS_PROGRESSIVE(u1Id)))

#define IS_SD_TIMING(u1Id)               (rMIBState[u1Id].u4SetResNewKDE >= MIB_SD_K_DE_LOW_MIN)

#define IS_MPEG(u1Id)                    ((_arMDDiPrm[u1Id].u1DecType == SV_VD_MPEGSD) ||( _arMDDiPrm[u1Id].u1DecType == SV_VD_MPEGHD))

#define IS_PROG_DISPMODE(u1Id)           ((_arMDDiPrm[u1Id].u1FrcMode == E_MIB_FRC_MIB)&&IS_PROGRESSIVE(u1Id))

#define IS_FRAME_SEQ(u1Id)               ((E_TD_IN_FS_I==_arMDDiPrm[u1Id].eTDInput)||(E_TD_IN_FS_P==_arMDDiPrm[u1Id].eTDInput))

#define IS_3D_FMT_CONV(u1Id)             (E_TD_OUT_NATIVE!=_arMDDiPrm[u1Id].eTDOutput)

#define IS_PR_MODE(u1Id)                 (((E_TD_OUT_3D_LI==_arMDDiPrm[u1Id].eTDOutput)\
                                                &&(IS_INTERLACE(u1Id))) \
                                            ||(E_TD_IN_LI_P==_arMDDiPrm[u1Id].eTDInput))

#define IS_SCL_BOB(eDataMode)            ((((eDataMode>>12)&0xF) == E_MIB_I) && ((eDataMode>>4)&0xF) == E_MIB_DI_OFF)

#define DATAMODE_ARA(datamode,bandwidth,flag) {(datamode),#datamode,bandwidth,flag}

#define IS_FORCE420MODE(u1Id)                   (_arMDDiPrm[u1Id].u1Force420Mode)



//////////////////////////////////////////////////////////////////////////////// 
// HW Register Enum , Should be matched by Each Different Generation
//////////////////////////////////////////////////////////////////////////////// 
#define MIB_ETHERNET_PLL 337500000
#define MIB_SYSTEM_PLL   648000000/2
#define MIB_XTAL_PLL     54000000


typedef enum{
    E_AF_Y = 0x01,
    E_AF_C = 0x02,
    E_BF_Y = 0x04,    
    E_BF_C = 0x08,
    E_CF_Y = 0x10,
    E_CF_C = 0x20,
    E_DF_Y = 0x40,    
    E_DF_C = 0x80,
    E_ALL_FLD = 0XFF,
} E_DI_FLD_R_EN;

typedef enum{
    E_ALL_RW_OFF = 0x0,
    E_SRC_W      = 0x1<<0,
    E_SRC_R      = 0x1<<1,
    E_LM_W       = 0x1<<2,    
    E_LM_R       = 0x1<<3,
    E_MV_W       = 0x1<<4,
    E_MV_R       = 0x1<<5,
    E_RNR_W      = 0x1<<6,
    E_RNR_R      = 0x1<<7,    
    E_NR_W       = 0x1<<8,
    E_ALL_RW     = 0x1FF,
} E_DI_DRAM_RW_EN;

typedef enum
{
    E_TG_FREE_RUN =0,
    E_TG_MIB_DISP =1,     
    E_TG_RSV      =2,     
} E_TIME_GEN_MODE;

typedef enum
{
    E_STA_PXL_OVERFLOW,     
    E_STA_PXL_UNDERFLOW,
    E_STA_NR_OVERFLOW,
    E_STA_SC_UNDERFLOW,
    E_STA_FLG_OVERFLOW,     
    E_STA_FLG_UNDERFLOW,
    E_PSCAN_STA_NUM
} E_PSCAN_STATUS;

typedef enum
{
    E_SRAM_SHARE_SD_I        =0,//Support POP YC3D
    E_SRAM_SHARE_SD_I_OS     =1,
    E_SRAM_SHARE_SD_P        =2,//Support POP YC3D
    E_SRAM_SHARE_SD_P_OS     =3,//Support POP YC3D
    E_SRAM_SHARE_HD_I        =4,
    E_SRAM_SAHRE_HD_P        =5,//Support POP YC3D
    E_SRAM_SHARE_HD_I_HALF_DQ=6,//Support POP YC3D
    E_SRAM_SHARE_HD_P_FULL_DQ=7,
    E_SRAM_SHARE_HD_I_FULL_FM=8,//Support POP YC3D
    E_SRAM_SHARE_HD_I_3D     =9,//Support POP YC3D
    E_SRAM_SHARE_HD_P_3D     =10,//Support POP YC3D    
}E_SRAM_SHARE_MODE;

typedef enum
{
    MIB_CLK_TYPEXTAL_CK         =0,
    MIB_CLK_TYPESYSPLL_D2_CK    =1,
    MIB_CLK_TYPEENETPLL_D2_CK   =2,
    MIB_CLK_RSV                 =3,
} HW_MIB_OUTPUT_CLK_TYPE;


typedef enum
{
    MIB_YUV_MODE_Y_ONLY,
    MIB_YUV_MODE_C_ONLY,
    MIB_YUV_MODE_YC,
    MIB_YUV_MODE_YUV
} HW_MIB_YUV_MODE;

typedef enum
{
    E_MIB_SYNC_OUTPUT = 0x0,
    E_MIB_SYNC_INPUT  = 0x1,
} HW_MIB_SYNC_MODE;

typedef enum
{
    HW_MIB_AF_FROM_DRAM = 0x0,
    HW_MIB_AF_FROM_SRC  = 0x1,
} HW_MIB_AF_SRC_SEL;

//////////////////////////////////////////////////////////////////////////////// 
// Data Mode Flag
//////////////////////////////////////////////////////////////////////////////// 

typedef enum
{
    DI_NORMAL_MODE,
    DI_MIRROR_ON_MODE,
    DI_FLIP_ON_MODE,
    DI_FLIP_MIRROR_ON_MODE,
    DI_FLIP_MIRROR_UNKOWN
} DI_FLIP_MIRROR_MODE;

typedef enum
{
    E_MIB_NR_OFF     =0x0, // NR Off mode   , Abbrev NRNN
    E_MIB_NR_Y       =0x1, // NR Y Only mode, Abbrev NRYN
    E_MIB_NR_YC      =0x2, // NR YC 422 mode, Abbrev NRYC
    E_MIB_NR_UNKNOWN =0x3,
} E_MIB_NR_DMODE;

typedef enum
{
    E_MIB_RNR_INTR,  
    E_MIB_RNR_PROG_INPT_SYNC,       
    E_MIB_RNR_PROG_OUTPT_SYNC,    
    E_MIB_RNR_DISABLE, 
} E_MIB_DA_RNR_MODE;

typedef enum
{
    E_MIB_DI_OFF     =0x0, 
    E_MIB_DI_Y1C1    =0x1, 
    E_MIB_DI_Y2C1    =0x2,
    E_MIB_DI_Y2C2    =0x3,
    E_MIB_DI_Y3C1    =0x4, 
    E_MIB_DI_Y3C2    =0x5,
    E_MIB_DI_Y4C1    =0x6, 
    E_MIB_DI_Y4C2    =0x7, 
    E_MIB_DI_Y4C4    =0x8, 
    E_MIB_DI_UNKNOWN =0x9, 
} E_MIB_DI_DMODE;

typedef enum
{
    E_MIB_FRC_SCL    =0x0,
    E_MIB_FRC_MIB    =0x1,
    E_MIB_FRC_NON    =0x2,
    E_MIB_FRC_UNKNOWN=0x3,
} E_MIB_FRC_DMODE;

typedef enum
{
    E_MIB_P       =0x0,//Progressive
    E_MIB_I       =0x1,//Interlace    
} E_MIB_IP_DMODE;

#define E(IP,NR,DI,FRC) E_MIB_DMODE_##FRC##_##NR##_##DI##_##IP
#define DMODE(IP,NR,DI,FRC) (UINT32)((((UINT32)IP) <<12)|(((UINT32)NR) <<8)|(((UINT32)DI) <<4)|(((UINT32)FRC) <<0))
#define E_DCL(IP,NR,DI,FRC) E(IP,NR,DI,FRC) = DMODE(E_MIB_##IP,E_MIB_##NR,E_MIB_##DI,E_MIB_##FRC)

typedef enum
{
    E_MIB_DMODE_UNKNOWN= 0xFF00,
    //Scaler FRC
    E_DCL(I, NR_Y   ,DI_OFF   ,FRC_SCL),
    E_DCL(I, NR_YC  ,DI_OFF   ,FRC_SCL),
    E_DCL(I, NR_OFF ,DI_Y1C1  ,FRC_SCL),
    E_DCL(I, NR_OFF ,DI_Y2C1  ,FRC_SCL),
    E_DCL(I, NR_OFF ,DI_Y2C2  ,FRC_SCL),
    E_DCL(I, NR_OFF ,DI_Y4C1  ,FRC_SCL),
	E_DCL(I, NR_YC  ,DI_Y4C1  ,FRC_SCL),//This Mode Only Used under Sub  
    E_DCL(I, NR_YC  ,DI_Y4C2  ,FRC_SCL),
    E_DCL(I, NR_OFF ,DI_Y4C4  ,FRC_SCL),//This Mode Only Used under sub
    E_DCL(I, NR_YC  ,DI_Y4C4  ,FRC_SCL),
    //MIB FRC
	E_DCL(I, NR_Y   ,DI_OFF   ,FRC_MIB),
	E_DCL(I, NR_YC  ,DI_OFF   ,FRC_MIB),
    E_DCL(I, NR_OFF ,DI_Y1C1  ,FRC_MIB),
    E_DCL(I, NR_OFF ,DI_Y2C1  ,FRC_MIB),
    E_DCL(I, NR_OFF ,DI_Y2C2  ,FRC_MIB),
    E_DCL(I, NR_OFF ,DI_Y3C1  ,FRC_MIB),
    E_DCL(I, NR_OFF ,DI_Y3C2  ,FRC_MIB),
    E_DCL(I, NR_OFF ,DI_Y4C1  ,FRC_MIB),
    E_DCL(I, NR_Y   ,DI_Y4C1  ,FRC_MIB),
    E_DCL(I, NR_YC  ,DI_Y4C1  ,FRC_MIB),
    E_DCL(I, NR_OFF ,DI_Y4C2  ,FRC_MIB),
    E_DCL(I, NR_Y   ,DI_Y4C2  ,FRC_MIB),
    E_DCL(I, NR_YC  ,DI_Y4C2  ,FRC_MIB),
    E_DCL(I, NR_OFF ,DI_Y4C4  ,FRC_MIB),
	E_DCL(I, NR_Y   ,DI_Y4C4  ,FRC_MIB),
	E_DCL(I, NR_YC  ,DI_Y4C4  ,FRC_MIB),
    //Scaler FRC
    E_DCL(P,NR_OFF   ,DI_OFF ,FRC_SCL),
    E_DCL(P,NR_Y     ,DI_OFF ,FRC_SCL),
    E_DCL(P,NR_YC    ,DI_OFF ,FRC_SCL),
    //MIB FRC
    E_DCL(P,NR_OFF   ,DI_OFF ,FRC_MIB),
    E_DCL(P,NR_Y     ,DI_OFF ,FRC_MIB),
    E_DCL(P,NR_YC    ,DI_OFF ,FRC_MIB),
    E_MIB_DMODE_END= 0x00FF
} E_MIB_DMODE;//Please Reference MIB Scenerio.xls for the implementation

//////////////////////////////////////////////////////////////////////////////// 
// MIB PQ Engine 
//////////////////////////////////////////////////////////////////////////////// 

typedef enum{
    E_DI_QUALITY_AGGRESSIVE, // MA Tend to 3D, Film Mode Tend to be entering, Edge Tend to be connected
    E_DI_QUALITY_DEFENSIVE,
    E_DI_QUALITY_NUM_MODE
} E_DI_QUALITY_MODE;

typedef enum{
    E_DI_ENGINE_IF,
    E_DI_ENGINE_PE,
    E_DI_ENGINE_FILM,    
    E_DI_ENGINE_TYPE_NUM
} E_DI_ENGINE_TYPE;

//////////////////////////////////////////////////////////////////////////////// 
// MIB Status Structure
//////////////////////////////////////////////////////////////////////////////// 

#define MIB_ELE_DECL(type,element) type element;

#undef MIB_ELE_CMD
#define MIB_ELE_CMD(type,element) MIB_ELE_DECL(type,element)

#define MIB_FBM_ELE_LIST \
    MIB_ELE_CMD(BOOL,fg10bit)\
    MIB_ELE_CMD(BOOL,fg422)\
    MIB_ELE_CMD(UINT32,u4Addr)\
    MIB_ELE_CMD(UINT32,u4Size)\
    MIB_ELE_CMD(UINT32,u4Width)\
    MIB_ELE_CMD(UINT32,u4Height)\
    MIB_ELE_CMD(UINT32,u4Mode)

typedef struct
{
    MIB_FBM_ELE_LIST
} MDDI_FBM_POOL_T;

// MIB State which will NOT changed during video playing
#define MIB_SYS_STATIC_ELE_LIST \
    MIB_ELE_CMD(UINT8,u1OnOff)\
    MIB_ELE_CMD(UINT8,u1BitRes)\
    MIB_ELE_CMD(UINT8,u1DecType)\
    MIB_ELE_CMD(UINT8,u1Interlace)\
    MIB_ELE_CMD(UINT8,u1420Mode)\
    MIB_ELE_CMD(UINT8,u1Force420Mode)\
    MIB_ELE_CMD(UINT8,u1IsDQ)\
    MIB_ELE_CMD(UINT8,u1IsOversample)\
    MIB_ELE_CMD(UINT8,u1IsPOP)\
    MIB_ELE_CMD(UINT8,u1IsDualFPI)\
    MIB_ELE_CMD(UINT8,u1FlipMorrorByMIB)\
    MIB_ELE_CMD(UINT8,u1IsNSTDSignal)\
    MIB_ELE_CMD(UINT16,u2HTotal)\
    MIB_ELE_CMD(UINT16,u2VTotal)\
    MIB_ELE_CMD(UINT16,u2Width)\
    MIB_ELE_CMD(UINT16,u2Height)\
    MIB_ELE_CMD(UINT16,u2HClip)\
    MIB_ELE_CMD(UINT16,u2VClip)\
    MIB_ELE_CMD(UINT16,u2SigWidth)\
    MIB_ELE_CMD(UINT16,u2SigHeight)\
    MIB_ELE_CMD(UINT32,u4BufAddr)\
    MIB_ELE_CMD(UINT16,u2BufWidth)\
    MIB_ELE_CMD(UINT16,u2BufHeight)\
    MIB_ELE_CMD(UINT32,u4BufSize)\
    MIB_ELE_CMD(UINT8,u1InFR)\
    MIB_ELE_CMD(UINT32,u4OutVT)\
    MIB_ELE_CMD(UINT32,u4OutHT)\
    MIB_ELE_CMD(UINT32,u4OutHeight)\
    MIB_ELE_CMD(UINT8,u1OutFR)\
    MIB_ELE_CMD(UINT32,DispX)\
    MIB_ELE_CMD(UINT32,DispY)\
    MIB_ELE_CMD(UINT32,DispW)\
    MIB_ELE_CMD(UINT32,DispH)\
    MIB_ELE_CMD(E_MIB_DMODE, eDataMode)\
    MIB_ELE_CMD(E_MIB_FRC_DMODE, u1FrcMode)\
    MIB_ELE_CMD(E_TD_IN, eTDInput)\
    MIB_ELE_CMD(E_TD_OUT, eTDOutput)\
    MIB_ELE_CMD(E_MIB_DMODE, eDebugDataMode)\
    MIB_ELE_CMD(E_MIB_NR_DMODE, u1DebugNRDataMode)\
    MIB_ELE_CMD(E_MIB_FRC_DMODE, u1DebugFrcMode)\
    MIB_ELE_CMD(UINT8,u1DebugFlipMorrorByMIB)\
    MIB_ELE_CMD(UINT8,u1VencOn)\
    MIB_ELE_CMD(UINT8,u1ForceDIBypass)
typedef struct {
    MIB_SYS_STATIC_ELE_LIST
}MIB_STATIC_STATE;

EXTERN MIB_STATIC_STATE _arMDDiPrm[VDP_NS];

// MIB State which will changed during video playing

#define MIB_SYS_DYN_ELE_LIST \
    MIB_ELE_CMD(UINT8,u1AToCFldCnt)\
    MIB_ELE_CMD(UINT16,u2Vcnt)\
    MIB_ELE_CMD(UINT8,bAllowedAutoStop)\
    MIB_ELE_CMD(UINT8,u1AutoStop)\
    MIB_ELE_CMD(UINT8,u1DataModeIdx)\
    MIB_ELE_CMD(E_MIB_DMODE,eBandwidthDataMode)\
    MIB_ELE_CMD(UINT8,u1BandwidthDataModeIdx)\
    MIB_ELE_CMD(UINT8,u1ModeChg)\
    MIB_ELE_CMD(UINT8,u1SeamlessSwitch)\
    MIB_ELE_CMD(UINT8,u4SetResNewKDE)\
    MIB_ELE_CMD(UINT8,u1DynRes)\
    MIB_ELE_CMD(UINT8,u1SetResFinishedCnt)\
    MIB_ELE_CMD(UINT32,u1StableCnt)\
    MIB_ELE_CMD(UINT32,u4DADebugStatus0)\
    MIB_ELE_CMD(UINT32,u4DADebugStatus1)\
    MIB_ELE_CMD(UINT32,u4DADebugStatus2)\
    MIB_ELE_CMD(UINT32,u4DADebugStatus3)\

typedef struct
{
    MIB_SYS_DYN_ELE_LIST
} MIB_DYN_STATE;

EXTERN MIB_DYN_STATE rMIBState[VDP_NS];

typedef struct
{
    E_MIB_DMODE eMIBMode;
    const char * const szModeName;
    UINT8 u1Bandwith; //Bandwidth Per Pixel
    UINT8 u1Flag;
} rDataMode;

/************** for Dram Dump**************************************/
typedef struct
{
    UINT32 u4MsbY;
    UINT32 u4LsbY;
    UINT32 u4MsbU;
    UINT32 u4LsbU;
    UINT32 u4MsbV;
    UINT32 u4LsbV;
    UINT32 u4Size;    
    UINT32 u4Width;
    UINT32 u4Height;
    UINT8 u1FrameNumY;
    UINT8 u1FrameNumU;
    UINT8 bIs444;
    UINT8 bIs10Bits;
}DI_DRAM_DUMP_INTO_T;

typedef struct
{
	UINT32 u4VR;
	UINT32 u4UB;
	UINT32 u4YG;
}DI_BUFFER_DATA;

typedef struct
{
    UINT8       u1IsDualFPI;
    E_TD_IN    	eTDInput;
    E_TD_OUT    eTDOutput;
}DI_3D_IN_OUT_FORMAT;

EXTERN MDDI_FBM_POOL_T rMIBFbmPool[VDP_NS];

EXTERN UINT8 u1FlipMirrorConfig;

EXTERN UINT32 u4FlagStaCnt[VDP_MAX_NS][E_PSCAN_STA_NUM];
EXTERN const rDataMode aDataMode[];
EXTERN UINT8  _bVgaTiming;


// PSCAN CTRL API
void vDrvDITrickModeOnOff(UINT8 u1VdpId, UINT8 u1OnOff);
void vDrvDIFreezeOnOff(UINT8 u1VdpId, UINT8 u1OnOff);
//void vDrvDIFrmEncModeOnOff(UINT8 u1VdpId, UINT8 u1OnOff, UINT8 u1BtmFirst);
void vDrvDISetDtvClip(UINT8 u1VdpId, UINT16 u2DTVClip);
void vDrvDISwInit(UINT8 u1VdpId);
void vDrvDIHwInit(UINT8 u1VdpId);
void vDrvDIFrameRepeat(UINT8 u1VdpId, UINT8 u1OnOff);
void vDrvMIBProc(void);
void vDrvMIBSuspend(void);
void vDrvMIBResume(void);

void vDrvDISrmCb(UINT32 u4Arg1, UINT32 u4Arg2, UINT32 u4PDSize, UINT32 u4SramMode);
void vDrvDIModeChangeDone(UINT8 u1VdpId);
void vDrvDIDemo(UINT8 u1OnOff, UINT8 u1Region, UINT16 u2Center);
void vDrvDISetNRDebugDataMode(UINT8 u1VdpId, E_MIB_NR_DMODE u1DataMode);
void vDrvDISetCrc(UINT16 u2Mode, UINT16 u2Arg);
UINT8 u1DrvDIFreezable(UINT8 u1VdpId);
UINT8 u1DrvDIGetFrameDelay10x(UINT8 u1VdpId);
UINT32 u4DrvDIMiniInputWidth(UINT8 u1VdpId);
UINT32 vDrvDIGetDADebugStatus(UINT32 u4Index);
void vDrvDISetMuteCtrl(UINT8 u1VdpId);

/**
 * @brief Return the maximum horizontal width that PSCAN can support
 * @param u1VdpId VDP_1/VDP_2
 * @return u4MaxInputWidth The maximum horizontal width that PSCAN can support
 */

UINT32 u4DrvDIMaxInputWidth(UINT8 u1VdpId);

/**
 * @brief Return the maximum output width cause by the clock rate limitation
 * @param u1VdpId VDP_1/VDP_2
 * @return the maximum output width cause by the clock rate limitation
 */
UINT32 u4DrvDIMaxOutWidth(UINT8 u1VdpId,UINT32 u4InputVActive,UINT32 u4DispVActive,UINT32 u4DispVTotal,UINT32 u4OutFR);

UINT32 u4DrvDIChkAllStatus(UINT8 u1VdpId);
void vDrvDIRstStatusCnt(UINT8 u1VdpId);
void vDrvDIClearStatus(UINT8 u1VdpId, UINT8 u1OnOff);
UINT16 u2DrvDIGetVdpWidth(UINT8 u1VdpId);
UINT16 u2DrvDIGetVdpHeight(UINT8 u1VdpId);

void vDrvDISetFrameBuf(UINT8 u1VdpId);

// PSCAN INT API
void vDrvDIB2RFilmModeFreeze(UINT8 bPath, UINT8 u1OnOff);

void vDrvDIFilmModeOnOff(UINT8 u1VdpId, UINT8 u1OnOff);
UINT8 bDrvGetFilmStatus(void);
void vDrvDISetBob(UINT8 u1VdpId, UINT8 u1OnOff);
void vDrvDISetFilmModeOnOff(UINT8 u1Path, UINT8 u1FilmType, UINT8 u1OnOff);
void vDrvDISetQualityMode(E_DI_ENGINE_TYPE eDIEngineType, E_DI_QUALITY_MODE eDIQualityMode);
void vDrvDISetEvent(UINT8 bNum,UINT8 bMode,UINT32 u4Value);
void vDrvDISetInk(UINT8 bMode, UINT16 u2Type);
UINT32 u4DrvDIGetMotionLevel(void);

#ifdef CC_SUPPORT_VENC
void vDrvDIVEncISR(void);
void vDrvDIVEncWOffISR(void);
void vDrvDISupportH264Init(void);
void vDrvDIStopSendBufToVENC(BOOL bFlag);
void vDrvDIVencModeChgPause(void);
void vDrvDIVencDropFrame(UINT8 fgOnOff);
UINT8 _u1DrvDIVencRunning(void);
void vDrvDISetVEncHandle(void *handle, void *ptFrmBuffMgr, void *ptResizeFrmBuffMgr, void *ptFilterPts);

typedef enum{
    E_DI_VENC_NORMAL,
    E_DI_VENC_TOP_FLD,
    E_DI_VENC_BOT_FLD,
} E_DI_VENC_RECORD_MODE; 

void vDrvDIVencFldRecordMode(E_DI_VENC_RECORD_MODE eMode);

/*********function for encoder input pattern gen*******************/

typedef enum{
    E_VENC_PAT_MOVING_SQUARE,
    E_VENC_PAT_MAX,
}E_VENC_PAT_TYPE;

void vDrvVencPatDrawProc(void);
void u1DrvVencPatOnOff(UINT8 fgOnOff);
UINT8 u1DrvVencPatIsOnOff(void);
void u1DrvVencPatSetParam(UINT32 u4YAddr, UINT32 u4CAddr, UINT32 u4Width, UINT32 u4Height, UINT8 fgWrite);
void u1DrvVencPatSetPatType(E_VENC_PAT_TYPE ePatType);

#endif

void vDrvDIDumpSetSize(UINT8 bPath);
void vDrvDIDumpDisSnakeMapping(UINT8 bPath);

void vDrvDIDumpInit(UINT8 bPath);
void vDrvDIDumpStart(UINT8 bPath);
void vDrvDIDumpEnd(UINT8 bPath);

UINT32 fgDrvDIIsMIBDramAvaliable(UINT8 u1VdpId);
void vDrvDIFlushDramForTDNAVI(UINT8 u1VdpId, UINT16 x, UINT16 y, UINT16 width, UINT16 height);
void vDrvDIDumpDramForTDNAVI(UINT8 u1VdpId, UINT16 x, UINT16 y, UINT16 width, UINT16 height,UINT32 *pu4Data);

void vDrvDIWritePixelValue(UINT8 u1VdpId, UINT16 x, UINT16 y, DI_BUFFER_DATA* value);

// PSCAN CLI API
void vDrvDIOnOff(UINT8 u1VdpId, UINT8 bOnOff);
void vDrvDIFWOnOff(UINT8 u1VdpId, UINT8 u1OnOff);
void vDrvDISetDebugDataMode(UINT8 bPath, E_MIB_DMODE bMode);
void vDrvDISetDebugPSCANDisplay(UINT8 u1VdpId, UINT8 u1OnOff);
void vDrvDISetPattern(UINT8 u1VdpId, UINT8 u1OnOff);
void vDrvDISetInputPattern(UINT8 u1VdpId, UINT8 u1OnOff);
void vDrvDIForceRead8bit(UINT8 u1VdpId, UINT8 b8bit);
void vDrvDIClearStatus(UINT8 u1VdpId, UINT8 u1OnOff);
void vDrvDISetDispmodeOff(UINT8 u1VdpId);
void vDrvDISetFieldInverse(UINT8 u1VdpId, UINT8 u1Inv);
void vDrvDISetColorMode(UINT8 u1VdpId, UINT8 u1ColorMode);
void vDrvDISetFlipMirror(UINT8 u1VdpId, UINT8 u1Mode);
void vDrvDISetDbgFlipMirror(UINT8 u1VdpId, UINT8 u1Mode);
void vDrvDISetFieldInverse(UINT8 u1VdpId, UINT8 u1Inv);
void vDrvDISetROIPixel(UINT8 u1VdpId, UINT32 u4Roi_H, UINT32 u4Roi_V);
void vDrvDISetWritePrecision(UINT8 u1VdpId, UINT8 b8bit);
void vDrvDIGetInfo(UINT8 u1VdpId, UINT8 u1DbgIdx);
void vDrvDISetOSD(UINT8 u1Item, UINT8 u1OnOff);
void vDrvDIGetBufAddr(UINT8 u1VdpId, UINT32 *u4AddrStart, UINT32 *u4AddrEnd);
void vDrvDIInitCRC(UINT8 u1VdpId);
void vDrvDIInitPscanCRC(UINT8 u1VdpId);
const CHAR* szDrvDataModeToString(UINT8 u1VdpId);
UINT8 u1DrvDIGetDebugPSCANDisplay(UINT8 u1VdpId);
UINT32 u4DrvDIGetHBlank(UINT8 u1VdpId);
UINT16 u2DrvDIGetVcnt(UINT8 u1VdpId);
UINT32 u4DrvDIGetInputCRC(UINT8 u1VdpId);
UINT32 u4DrvDIGetOutputCRC(UINT8 u1VdpId);
UINT16 u2DrvDIGetWidth(void);
void vDrvDIDispInfoForCapLog(UINT8 bPath, DI_DISP_INFO_T* pDispInfo); 
void vDrvDISetBitRes(UINT8 u1VdpId, UINT8 u18bit);
void vDrvDISetErrDefusion(UINT8 u1VdpId, UINT8 u18bit);

void vDrvDISetClock(UINT8 u1VdpId, HW_MIB_OUTPUT_CLK_TYPE eType);
void vDrvDISetClockEnable(UINT8 u1VdpId, UINT8 u1OnOff);


void vDrvDiSetDispmodeHBlank(UINT8 u1VdpId, UINT16 u2Value);
UINT32 u4DrvDiGetDispHBlank(UINT8 u1VdpId);
UINT8 u1DrvDiIs10Bit(UINT8 u1VdpId);
UINT8 u1DrvDiIs422Mode(UINT8 u1VdpId);
UINT8 u1DrvDiIsTopField(UINT8 u1VdpId);
UINT32 u4DrvDIIsAutoStopped(UINT8 u1VdpId);

void vDrvDISetDataMode(UINT8 u1VdpId, E_MIB_FRC_DMODE eFrc, E_MIB_DI_DMODE eDI, E_MIB_NR_DMODE eNR);
UINT16 u2DrvDIGetMotionLevel(void);
UINT32 u4DrvDIGetFusionMotionLevel(void);
BOOL bDrvDIIsScalerBob(UINT8 u1VdpId);

void vDrvDIDispmodeLRLockISR(void);

typedef enum{
    E_DI_LR_SWAP_IN,
    E_DI_LR_SWAP_OUT,
    E_DI_LR_SWAP_UPDATE,
}E_DI_LR_SWAP;

void vDrvDISetLRSwap(UINT8 u1VdpId,E_DI_LR_SWAP InOut , UINT32 u4OnOff);

UINT8 u1DrvDIIsSupportedSignal(UINT8 u1VdpId);
UINT8 u1vDrvDIMPEGForce22FilmMode(UINT8 fgOnOff, UINT8 u1Mode);
void vDrvDIInOutStatusDump(UINT8 bPath);

/**
 * @brief Get DI output clock rate
 * @param u1VdpId VDP_1/VDP_2
 * @return u4ClkRate clock rate
 */

UINT32 u4DrvDIGetClkRate(UINT8 u1VdpId);
void vDrvDIStopCtrlFw(UINT8 u1VdpId,UINT8 fgOnOff);
void vDrvDIInOutSelect(UINT8 u1VdpId,UINT8 fgInOut, UINT8 fgPath);
void vDrvDISetDumpRegion(UINT32 u4StartX, UINT32 u4StartY, UINT32 u4Width, UINT32 u4Height);
void vDrvDISetMuteCtrl(UINT8 u1VdpId);

BOOL bDrvDIIsSetResDone(UINT8 u1VdpId, SET_RES_STATUS_TYPE_E eStatusType);
void vDrvDIClrSetRes(UINT8 u1VdpId, SET_RES_STATUS_TYPE_E eStatusType);
void vDrvDISetResMode(UINT8 u1VdpId,UINT8 fgOnOff);
void vDrvDISetResTrigger(UINT8 u1VdpId);
void vDrvDISetResEnable(UINT8 u1VdpId,UINT8 fgOnOff);
void _vDrvDIQualityCreateThread(void);

/**
 * @brief PDinfo co-work between MJC & DI
 */
UINT8 u1DrvGetSWFilmInfoFromDI(void);
UINT8 u1GetPDInfoDelayAtProgDispMode(void);
void vSetPDInfoDelayAtProgDispMode(void);
BOOL bDrvDIRepeatFldFound(UINT8 u1VdpId);

void u1DrvDIForceBypass(UINT8 u1VdpId, UINT8 u1BypassOnOff);
UINT8 u1DrvDIIsBypass(UINT8 u1VdpId);
UINT8 u1DrvDIIsForceBypass(UINT8 u1VdpId);
#endif    // DRV_DI_H
