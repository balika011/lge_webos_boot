/*----------------------------------------------------------------------------*
 * No Warranty                                                                *
 * Except as may be otherwise agreed to in writing, no warranties of any      *
 * kind, whether express or implied, are given by MTK with respect to any MTK *
 * Deliverables or any use thereof, and MTK Deliverables are provided on an   *
 * "AS IS" basis.  MTK hereby expressly disclaims all such warranties,        *
 * including any implied warranties of merchantability, non-infringement and  *
 * fitness for a particular purpose and any warranties arising out of course  *
 * of performance, course of dealing or usage of trade.  Parties further      *
 * acknowledge that Company may, either presently and/or in the future,       *
 * instruct MTK to assist it in the development and the implementation, in    *
 * accordance with Company's designs, of certain softwares relating to        *
 * Company's product(s) (the "Services").  Except as may be otherwise agreed  *
 * to in writing, no warranties of any kind, whether express or implied, are  *
 * given by MTK with respect to the Services provided, and the Services are   *
 * provided on an "AS IS" basis.  Company further acknowledges that the       *
 * Services may contain errors, that testing is important and Company is      *
 * solely responsible for fully testing the Services and/or derivatives       *
 * thereof before they are used, sublicensed or distributed.  Should there be *
 * any third party action brought against MTK, arising out of or relating to  *
 * the Services, Company agree to fully indemnify and hold MTK harmless.      *
 * If the parties mutually agree to enter into or continue a business         *
 * relationship or other arrangement, the terms and conditions set forth      *
 * hereunder shall remain effective and, unless explicitly stated otherwise,  *
 * shall prevail in the event of a conflict in the terms in any agreements    *
 * entered into between the parties.                                          *
 *----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * Copyright (c) 2005, MediaTek, Inc.
 * All rights reserved.
 *
 * Unauthorized use, practice, perform, copy, distribution, reproduction,
 * or disclosure of this information in whole or in part is prohibited.
 *-----------------------------------------------------------------------------
 *
 * $Author: dtvbm11 $
 * $Date: 2015/01/09 $
 * $RCSfile: drv_mjc.h $
 * $Revision: #1 $
 *
 *----------------------------------------------------------------------------*/

#ifndef _DRV_MJC_H_
#define _DRV_MJC_H_

/*----------------------------------------------------------------------------*
 * Header Files
 *----------------------------------------------------------------------------*/
#include "hw_mjc.h"
#include "sv_const.h"
#include "typedef.h"
#include "video_def.h"
#include "nptv_if.h"
#include "drv_scaler.h"
#include "drv_di.h"
#include "drv_di_int.h"

/*----------------------------------------------------------------------------*
 * Definitions
 *----------------------------------------------------------------------------*/
#define MJC_FPGA                    (0)

#define MJC_3D_MV_WA                0   // for OSMP mv work-around, ES only
#define MJC_OSMP_LBOX_AUTO_OFF      0   // diable lbox auto detection at OSMP 2D mode, fix L or R judder, ES only

#define V_Fld(u4Val, fld)           ((u4Val)<<Fld_shft(fld))

// read field
#define R_Fld(u4Val, fld)           (((u4Val)&Fld2Msk32(fld))>>Fld_shft(fld))

// mjc frmsch 
#define MJC_FRM_SCH_MODE	MJC_PSEUDO_DROP_ON_SCN_CHG
#define MJC_SUPPORT_MULTI_CADENCE 1

//#define MJC_SW_SUPPORT_FRM_DELAY  1

//mjc isr mask

#define MJC_INT_0           (1 << 0) // Adjusted input vsync end
#define MJC_INT_1           (1 << 1) // Output vsync end
#define MJC_INT_2           (1 << 2) // Input vsync end
#define MJC_INT_3           (1 << 3) // MJC frame processing finished

//mjc 3d define

#define MJC_WA_OUT_LR3D_DLY         (1)
#define MJC_OSMP_FLPRV0             (1 << 0)
#define MJC_OSMP_FLPRV1             (1 << 1)
#define MJC_OSMP_FLPRV2             (1 << 2)
#define MJC_OSMP_FLPRV3             (1 << 3)
#define MJC_OSMP_IS_HPR             (1 << 4)
#define MJC_OSMP_PR_MODE            (MJC_OSMP_FLPRV2) //(MJC_OSMP_IS_HPR && ((IS_LVDS_DISP_3D_POLARIZED) || (IS_PANEL_L12R12)))
#define MJC_H_SERCH_RANGE   96
//

#define MJC_SET_BUF_FULL    0
#define MJC_FORCE_RPT_MODE  0
#define MJC_DISP_VPOS_120_TARGET 1631 // 28 * (65536/1125)
#define MJC_DISP_VPOS_060_TARGET 1048 // 18 * (65536/1125)
#define MJC_SP_LOG_FRMSCH (1 << 0)
#define MJC_INIT_MBIST_FLOW
#define MJC_2ND_MBIST_CHK   0
#define MJC_REGTBL_END 0xffffffff

//mjc fbk control

#define MJC_MC_LF_HALF_FBK_ON  (1 << 1)
#define MJC_MC_RT_HALF_FBK_ON  (1 << 0)
#define MJC_MC_LF_HALF_FBK_OFF (0 << 1)
#define MJC_MC_RT_HALF_FBK_OFF (0 << 0)

//
#define MJC_BIT_SEL Fld(3, 8, AC_MSKW10) //[10:8]
#define MJC_FLPMRR_SEL Fld(2, 6, AC_MSKB0) //[7:6]
#define MJC_COLOR_SEL Fld(2, 4, AC_MSKB0) //[5:4]
#define MJC_RSCNRDC_SEL Fld(2, 2, AC_MSKB0) //[3:2]
#define MJC_NFRM_SEL Fld(2, 0, AC_MSKB0) //[1:0]

// mjc align

#define ALIGN_MASK(VAL, MSK) (((VAL) + (MSK)) & (~(MSK)))
#define ALIGN_64BYTE(val) (((val) + 0x0000003f) & (~0x0000003f))
#define ALIGN_256_CEIL(val) ALIGN_MASK((val), 255)
#define ALIGN_128_CEIL(val) ALIGN_MASK((val), 127)
#define ALIGN_64_CEIL(val) ALIGN_MASK((val), 63)
#define ALIGN_32_CEIL(val) ALIGN_MASK((val), 31)
#define ALIGN_16_CEIL(val) ALIGN_MASK((val), 15)
#define ALIGN_8_CEIL(val) ALIGN_MASK((val), 7)
#define ALIGN_4_CEIL(val) ALIGN_MASK((val), 3)
#define ALIGN_2_CEIL(val) ALIGN_MASK((val), 1)

#define MJC_CTRL_RT_ADAPT   (1 << 0)
#define MJC_CTRL_LT_ADAPT   (1 << 1)
#define MJC_CTRL_RT_HFBACK  (1 << 2)
#define MJC_CTRL_LT_HFBACK  (1 << 3)
#define MJC_CTRL_RT_ON      (1 << 4)
#define MJC_CTRL_LT_ON      (1 << 5)
#define MJC_CTRL_FM_ON      (1 << 6)

//mjc mode items

#define MJC_RPT_MODE    (1 << 11)

#define MJC_DYN_MODE    (1 << 10)
#define MJC_Y10_MODE	(0 << 0) 	
#define MJC_Y08_MODE	(1 << 9) 	
#define MJC_C10_MODE	(0 << 0) 	
#define MJC_C08_MODE	(1 << 8) 	

#define MJC_422_MODE	(0 << 0) 	
#define MJC_420_MODE	(1 << 5) 	
#define MJC_444_MODE	(1 << 4) 	
#define MJC_422RPT_MODE	(1 << 6) 

#define MJC_NRSCN_MODE  (0 << 0)
#define MJC_RSCN_MODE   (1 << 3)
#define MJC_0RDC_MODE	(0 << 0) 	
#define MJC_1RDC_MODE	(1 << 2) 	

#define MJC_3FRM_MODE	(0 << 0) 	
#define MJC_2FRM_MODE	(1 << 0) 	
#define MJC_1FRM_MODE	(1 << 1) 	

#define MJC_SUSPEND_MODE    (1 << 7)

//Unicadence setting
#ifdef MJC_UNICADENCE_CTRL_API
#define MJC_NO_UNI          (0 << 0) 
#define MJC_UNI_50TO120_2D  (1 << 0) 
#define MJC_UNI_50TO120_3D  (1 << 1) 
#define MJC_UNI_60TO100_2D  (1 << 2) 
#define MJC_UNI_60TO100_3D  (1 << 3) 
#define MJC_UNI_6X3X_WEB    (1 << 4) 
#define MJC_PANEL_48HZ      (1 << 5) 

#define IS_SUPPORT_DISP_50TO120_2D_API  (vDrvMJCGetUnicadenceType() & MJC_UNI_50TO120_2D)
#define IS_SUPPORT_DISP_50TO120_3D_API  (vDrvMJCGetUnicadenceType() & MJC_UNI_50TO120_3D)
#define IS_SUPPORT_DISP_60TO100_2D_API  (vDrvMJCGetUnicadenceType() & MJC_UNI_60TO100_2D)
#define IS_SUPPORT_DISP_60TO100_3D_API  (vDrvMJCGetUnicadenceType() & MJC_UNI_60TO100_3D)
#define IS_SUPPORT_DISP_6X3X_WEB_API    (vDrvMJCGetUnicadenceType() & MJC_UNI_6X3X_WEB)
#define IS_SUPPORT_PANEL_48HZ_API       (vDrvMJCGetUnicadenceType() & MJC_PANEL_48HZ)
#endif
// enum 

enum
{
    MJC_Y10_C10_422_0RDC_3FRM = MJC_Y10_MODE | MJC_C10_MODE | MJC_422_MODE | MJC_0RDC_MODE | MJC_3FRM_MODE,
    MJC_Y10_C10_422_1RDC_3FRM = MJC_Y10_MODE | MJC_C10_MODE | MJC_422_MODE | MJC_1RDC_MODE | MJC_3FRM_MODE,
    MJC_Y10_C10_420_0RDC_3FRM = MJC_Y10_MODE | MJC_C10_MODE | MJC_420_MODE | MJC_0RDC_MODE | MJC_3FRM_MODE,
    MJC_Y10_C10_420_1RDC_3FRM = MJC_Y10_MODE | MJC_C10_MODE | MJC_420_MODE | MJC_1RDC_MODE | MJC_3FRM_MODE,
    MJC_Y10_C10_422_0RDC_2FRM = MJC_Y10_MODE | MJC_C10_MODE | MJC_422_MODE | MJC_0RDC_MODE | MJC_2FRM_MODE,
    MJC_Y10_C10_422_1RDC_2FRM = MJC_Y10_MODE | MJC_C10_MODE | MJC_422_MODE | MJC_1RDC_MODE | MJC_2FRM_MODE,
    MJC_Y10_C10_420_0RDC_2FRM = MJC_Y10_MODE | MJC_C10_MODE | MJC_420_MODE | MJC_0RDC_MODE | MJC_2FRM_MODE,
    MJC_Y10_C10_420_1RDC_2FRM = MJC_Y10_MODE | MJC_C10_MODE | MJC_420_MODE | MJC_1RDC_MODE | MJC_2FRM_MODE,
    MJC_Y10_C10_422_0RDC_1FRM = MJC_Y10_MODE | MJC_C10_MODE | MJC_422_MODE | MJC_0RDC_MODE | MJC_1FRM_MODE,
    MJC_Y10_C10_420_0RDC_1FRM = MJC_Y10_MODE | MJC_C10_MODE | MJC_420_MODE | MJC_0RDC_MODE | MJC_1FRM_MODE,
    MJC_Y10_C08_422_0RDC_3FRM = MJC_Y10_MODE | MJC_C08_MODE | MJC_422_MODE | MJC_0RDC_MODE | MJC_3FRM_MODE,
    MJC_Y10_C08_422_1RDC_3FRM = MJC_Y10_MODE | MJC_C08_MODE | MJC_422_MODE | MJC_1RDC_MODE | MJC_3FRM_MODE,
    MJC_Y10_C08_420_0RDC_3FRM = MJC_Y10_MODE | MJC_C08_MODE | MJC_420_MODE | MJC_0RDC_MODE | MJC_3FRM_MODE,
    MJC_Y10_C08_420_1RDC_3FRM = MJC_Y10_MODE | MJC_C08_MODE | MJC_420_MODE | MJC_1RDC_MODE | MJC_3FRM_MODE,
    MJC_Y10_C08_422_0RDC_2FRM = MJC_Y10_MODE | MJC_C08_MODE | MJC_422_MODE | MJC_0RDC_MODE | MJC_2FRM_MODE,
    MJC_Y10_C08_422_1RDC_2FRM = MJC_Y10_MODE | MJC_C08_MODE | MJC_422_MODE | MJC_1RDC_MODE | MJC_2FRM_MODE,
    MJC_Y10_C08_420_0RDC_2FRM = MJC_Y10_MODE | MJC_C08_MODE | MJC_420_MODE | MJC_0RDC_MODE | MJC_2FRM_MODE,
    MJC_Y10_C08_420_1RDC_2FRM = MJC_Y10_MODE | MJC_C08_MODE | MJC_420_MODE | MJC_1RDC_MODE | MJC_2FRM_MODE,
    MJC_Y10_C08_422_0RDC_1FRM = MJC_Y10_MODE | MJC_C08_MODE | MJC_422_MODE | MJC_0RDC_MODE | MJC_1FRM_MODE,
    MJC_Y10_C08_420_0RDC_1FRM = MJC_Y10_MODE | MJC_C08_MODE | MJC_420_MODE | MJC_0RDC_MODE | MJC_1FRM_MODE,
    MJC_Y08_C08_422_0RDC_3FRM = MJC_Y08_MODE | MJC_C08_MODE | MJC_422_MODE | MJC_0RDC_MODE | MJC_3FRM_MODE,
    MJC_Y08_C08_422_1RDC_3FRM = MJC_Y08_MODE | MJC_C08_MODE | MJC_422_MODE | MJC_1RDC_MODE | MJC_3FRM_MODE,
    MJC_Y08_C08_420_0RDC_3FRM = MJC_Y08_MODE | MJC_C08_MODE | MJC_420_MODE | MJC_0RDC_MODE | MJC_3FRM_MODE,
    MJC_Y08_C08_420_1RDC_3FRM = MJC_Y08_MODE | MJC_C08_MODE | MJC_420_MODE | MJC_1RDC_MODE | MJC_3FRM_MODE,
    MJC_Y08_C08_422_0RDC_2FRM = MJC_Y08_MODE | MJC_C08_MODE | MJC_422_MODE | MJC_0RDC_MODE | MJC_2FRM_MODE,
    MJC_Y08_C08_422_1RDC_2FRM = MJC_Y08_MODE | MJC_C08_MODE | MJC_422_MODE | MJC_1RDC_MODE | MJC_2FRM_MODE,
    MJC_Y08_C08_420_0RDC_2FRM = MJC_Y08_MODE | MJC_C08_MODE | MJC_420_MODE | MJC_0RDC_MODE | MJC_2FRM_MODE,
    MJC_Y08_C08_420_1RDC_2FRM = MJC_Y08_MODE | MJC_C08_MODE | MJC_420_MODE | MJC_1RDC_MODE | MJC_2FRM_MODE,
    MJC_Y08_C08_422_0RDC_1FRM = MJC_Y08_MODE | MJC_C08_MODE | MJC_422_MODE | MJC_0RDC_MODE | MJC_1FRM_MODE,
    MJC_Y08_C08_420_0RDC_1FRM = MJC_Y08_MODE | MJC_C08_MODE | MJC_420_MODE | MJC_0RDC_MODE | MJC_1FRM_MODE,
    FRC_Y10_C10_444 = MJC_Y10_MODE | MJC_C10_MODE | MJC_444_MODE,
    FRC_Y08_C08_444 = MJC_Y08_MODE | MJC_C08_MODE | MJC_444_MODE,
    FRC_Y10_C10_422RPT = MJC_Y10_MODE | MJC_C10_MODE | MJC_444_MODE | MJC_422RPT_MODE,
    FRC_Y08_C08_422RPT = MJC_Y08_MODE | MJC_C08_MODE | MJC_444_MODE | MJC_422RPT_MODE
};

enum
{
	MJC_FRAME_NO_DROP,
	MJC_PSEUDO_DROP_ALL,
	MJC_PSEUDO_DROP_ON_SCN_CHG, // default frame No drop
	MJC_FRAME_DROP_LIKE			// default frame drop
};

enum
{
	E_MJC_SELF_MODE,
	E_MJC_DISP_MODE
};

enum
{
    FALLBACK_MODE_55,
    FALLBACK_MODE_64,
    FALLBACK_MODE_55P2
};

enum
{
    E_MJC_60HZ = 0,
    E_MJC_120HZ = 1,
    E_MJC_240HZ = 2
};

enum
{
    E_MJC_BYPASS = 0,
    E_MJC_ON = 1,
    E_MJC_OFF = 2
};

enum
{
    E_MJC_HPR_NM     = 0,
    E_MJC_HPR2160 = 1
};


enum
{
    E_MJC_FPRV0 = 0,
	E_MJC_FPRV1 = 1,
	E_MJC_FPRV2 = 2,
	E_MJC_FPRV3 = 3
};

enum
{
    E_MJC_NO_CINEMA = 0,
    E_MJC_CINEMA_12 = 1,
    E_MJC_CINEMA_15 = 2,
    E_MJC_CINEMA_20 = 3,
    E_MJC_CINEMA_24 = 4,
    E_MJC_CINEMA_25 = 5,
    E_MJC_CINEMA_30 = 6,
    E_MJC_CINEMA_40 = 7
};


typedef enum
{
    E_MJC_INTP_UNKNOWN = 0,
    E_MJC_2D_60        = 1,
    E_MJC_2D_120       = 2,
    E_MJC_2D_240       = 3,
    E_MJC_3D_60        = 4, // 50Hz (2x), 60Hz (2.5x)
    E_MJC_3D_120       = 5, // 100Hz 44 mode (4x), 120Hz 64 mode (5x)
    E_MJC_TTD_120      = 6,
    E_MJC_TTD_240      = 7    
} MJC_FBK_INTP_MODE_T;

typedef enum
{
    E_MJC_EFFECT_OFF = 0,
    E_MJC_EFFECT_LOW,
    E_MJC_EFFECT_MIDDLE,
    E_MJC_EFFECT_HIGH,
    E_MJC_EFFECT_USER,
    E_MJC_EFFECT_NUM
} E_MJC_EFFECT;

typedef enum
{
    E_MJC_DELAY_PSAUTO = 0,
    E_MJC_DELAY_UNMUTE,
    E_MJC_DELAY_DM_BW_CHK,
    E_MJC_DELAY_DM_CRC_CHK,
    E_MJC_DELAY_DM_START,
    E_MJC_DELAY_MAX
} E_MJC_DELAY_SET;

typedef enum
{
    E_MJC_DEMO_FULL = 0,    // demo off
    E_MJC_DEMO_COLUMN,      // split demo mode = 1
    E_MJC_DEMO_ROW,         // split demo mode = 2
    E_MJC_DEMO_WINDOW       // split demo mode = 3
} E_MJC_DEMO;

typedef enum
{
    E_MJC_RGN_NF = 0,       // intp = 6, hqf = 0
    E_MJC_RGN_HF = 1,       // intp = 6, hqf = 1
    E_MJC_RGN_QF = 2,       // intp = 6, hqf = 2
    E_MJC_RGN_RPT = 3       // intp = 0    
} E_MJC_DEMO_RGN_TYPE;

typedef enum {
    E_VDO_MJC_2D           = 0,
    E_VDO_MJC_FS           = 1,
    E_VDO_MJC_SBS          = 2,
    E_VDO_MJC_TAB          = 3,
    E_VDO_MJC_LI           = 4,
    E_VDO_MJC_SG_OSMP_2D   = 5, 
    E_VDO_MJC_OTHER        = 6
}E_VDO_MJC_IN;

typedef enum
{
    E_VDO_MJC_OUT_2D    = 0,
    E_VD0_MJC_OUT_FS    = 1,
    E_VD0_MJC_OUT_HPR   = 2,
    E_VD0_MJC_OUT_FPR   = 3,
    E_VD0_MJC_OUT_OTHER = 4
}E_VDO_MJC_OUT;

typedef enum
{
    E_MJC_TRK_REF_EARLY = 0,
    E_MJC_TRK_REF_LATE = 1
}E_MJC_FRM_TRK_REF;

//struct

typedef struct
{
    UINT32  u4StartAddr;
    UINT32  u4EndAddr;
    UINT32  u4Width;
    UINT32  u4Height;
    UINT16  u2InitMode;
    UINT16  u2Mode;
    UINT16  u2DebugMode;
} MJC_DRAM_ALLOC_T;

typedef struct
{
    UINT16  u2InHStart;
    UINT16  u2InVStart;
    UINT16  u2InWidth;
    UINT16  u2InHeight;
	UINT16  u2InVsyncDly;
    UINT16  u2OutHStart;
    UINT16  u2OutVStart;
    UINT16  u2OutWidth;
    UINT16  u2OutHeight;
    UINT16  u2OutHTotal;
    UINT16  u2OutVTotal;
    UINT8   u2HBlank;
    UINT8   u2WBlank;
	UINT8   u1Dram444;
    UINT8   fgDdp;
    UINT8   u1DispVpos;
} MJC_PIC_SIZE_T;

typedef struct
{
    UINT8   u1InFrmRate;
    UINT8   u1OutFrmRate;
    UINT8   u1VdoFrmRate;
    UINT8   u13DModeIn;
    UINT8   u13DModeOut;
    UINT8   u1FHPRMode;
    UINT8   u1FrmSchMode;
    UINT8   u1PanelType;
    UINT8   u1OSMPFlg;
    UINT8   u1UniCadenceEn;
    UINT8   fgProgFrtEn;
    UINT8   u1Dram444;
    UINT8   u1VRgn;
} MJC_FRC_CONFIG_T;

typedef struct
{
    UINT8   u1MJC3DModeIn;
	UINT8   u1MJC3DModeOut;
    UINT16  u2VRMID;
    UINT16  u2X;
    UINT16  u2Y;
    UINT16  u2Width;
    UINT16  u2Height;
    UINT16  u2HTotal;
    UINT16  u2VTotal;
    UINT16  u2MJCInVTotal;
    UINT16  u2MJCInFrameRate;
    UINT16  u2MJCOutFrameRate;
} MJC_VRM_SRM_CONFIG_T;


typedef struct
{
    E_MJC_DEMO          eDemoMode;
    E_MJC_DEMO_RGN_TYPE arRgn[3];
    UINT16  u2BndLeft;
    UINT16  u2BndRight;
    UINT16  u2BndTop;
    UINT16  u2BndBottom;
} MJC_DEMO_CFG_T;

typedef struct
{
    UINT8   u1MJCOSMPHPRMode;
    UINT8   u1MJCOSMPFPRMode;

}MJC_PR_CONFIG_T;


typedef struct
{
    UINT8   u1OnOff;

    UINT8   u1Dyn;
    UINT8   u1Rpt;

    UINT8   u1Dram444;
    UINT8   u1Dram422RPT;
    UINT8   u1Dram420;
    UINT8   u1Y8;
    UINT8   u1C8;

    UINT8   u1YDithEn;
    UINT8   u1CDithEn;
    UINT8   u1Flip;
    UINT8   u1Mirror;

    UINT8   u1TwoFrm;
    UINT8   u1OneFrm;
    UINT8   u1Rdc;
    UINT8   u1Rscn;

    UINT8   u1CtrlParam;
    UINT8   u1EffectParam;
    UINT8   u1UIOnOff;
    UINT8   u1PSAuto22OnOff;
    UINT8   u1PSAuto32OnOff;
    UINT8   fgIsInterlace;
    UINT8   u1ScalerMode;
    UINT8   fgMJCFSIn;
    UINT8   fgFreeze;
    UINT8   fgSupportUniCandece;
    UINT8   u1DispRDly;
    UINT8   fgDispRSwp;
    UINT8   fgInLRSwp;

    UINT16  u2Center;
    UINT8   u1DIROnOff;

    E_MJC_EFFECT eMjcEffect;
    MJC_PIC_SIZE_T arPicSize;
    MJC_FRC_CONFIG_T arFRCConfig;
    MJC_VRM_SRM_CONFIG_T arVrmSrmConfig;
    MJC_PR_CONFIG_T  arMJCPRConfig;
}MJC_PRM_T;


typedef struct{
    PThreadFunc pThreadFunc;
    CHAR*       szThreadDescription;
} MJC_SP_THREAD;

typedef struct
{
    HANDLE_T* pVsyncSema;
    CHAR*     szVsyncSema;
} MJC_SP_VSYNC;

typedef struct
{
    PThreadFunc* ptrPfnVSyncFunc;
    CHAR*     szVsyncDesc;
} MJC_SP_VSYNC_T;

typedef struct
{
    UINT32 u4Addr;
    UINT8  u1Msb;
    UINT8  u1Lsb;
} MJC_SP_REG_T;

typedef struct
{
    UINT16 u2DramMode;
    char* strDramMode;
} MJC_DRAM_MODE_T;

typedef struct
{
    UINT16 u4SrcWidth;
    UINT16 u4SrcHeight;    
    UINT32 u4SrcAddr;    
    UINT16 u4DesWidth;
    UINT16 u4DesHeight;    
    UINT32 u4DesAddrL;    
    UINT32 u4DesAddrR;	
    UINT32 u4SrcStartX;
    UINT32 u4SrcStartY;
    UINT32 u4Subsample;
    UINT32 u4DumpSize;
    UINT8 u4Index;
} MJC_DUMP_INFO_T;

typedef struct
{
    UINT16 u2PosUp;
    UINT16 u2PosDn;
    UINT16 u2PosLf;
    UINT16 u2PosRt;
    BOOL fgIsYSmall;
} MJC_LBOX_STA_T;

/*----------------------------------------------------------------------------*
 * Global/Static variables
 *----------------------------------------------------------------------------*/
extern MJC_DRAM_ALLOC_T arMJCDramAlloc;
extern MJC_PRM_T arMJCPrm;
extern MJC_DEMO_CFG_T arMJCDemo;
extern MJC_SP_THREAD MJCSPThread[];
extern MJC_SP_VSYNC MJCSPVsync[];
extern MJC_SP_VSYNC_T MJCSPVsyncIsr[];
extern MJC_LBOX_STA_T _rMJCLboxSta;

extern UINT8 u1MJCFbckCtrl;
extern UINT8 u1MJCStatusCheck;
extern UINT8 fgMJCStatusPolling;
extern UINT8 u1TestDelayCnt;

extern UINT32 u4MJCStatusPollingCount;


/*----------------------------------------------------------------------------*
 * Function Members
 *----------------------------------------------------------------------------*/

void vDrvMJCSetSpEn(UINT32 u4Msk, UINT32 u4Count);
void vDrvMJCClearSpEn(UINT32 u4Msk);
void vDrvMJCSetPsAutoTrigOn(void);
void vDrvMJCInit(UINT8 bForceInit);
void vDrvMJCResetSW(void);
void vDrvMJCInitISR(void);
void vDrvMJCISR(UINT16 u2Vector);
void vDrvMJCSetClock(void);
void vDrvMJCSetDemoCfg(const MJC_DEMO_CFG_T* prDemo);
void MJC_Set3DModeDbg(UINT8 u13DModeIn , UINT8 u13DModeOut);
void vDrvMJCRunMbist(void);
void vDrvMJCSetSACtrlMode(UINT8 u1Mode);
void vDrvMJCSetDRAMAccessOnOff(BOOL fgOnOff, BOOL fgDRAM444);
void vDrvMJCSrmCb(UINT32 u4VdpId, UINT32 u4Mode, UINT32 u4PDSize, UINT32 u4SramMode);
void vDrvMJCSetBuf(MJC_DRAM_ALLOC_T * prDramAlloc);
void vDrvMJCSetSystemMode(UINT32 u4MjcSystemMode);
void vDrvMJCSetSystemModeProc(void);
void vDrvMJCSetDispMode(UINT32 fgOnOff);
void vDrvMJCSetFlipMirror(UINT32 u4FlipOnOff, UINT32 u4MirrorOnOff);
void vDrvMJCSetPdInfoShift(UINT32 u4PdInfo32Shift, UINT32 u4PdInfo22Shift);
void vDrvMJCSet32PdInfoShiftJudgment(void);
void vDrvMJCSet32PdInfoShiftProc(UINT8 fgIsInterlace, UINT8 u1ScalerMode, UINT8 fgMJCIs3DFSMode);
void vDrvMJCSetPSAutoOnOff(BOOL fgOnOff);
void vDrvMJCSetFrmSch(MJC_FRC_CONFIG_T * arMJCFRC);
void vDrvMJCSetFrmschUniCandeceProc(const MJC_FRC_CONFIG_T * arMJCFRC);
void vDrvMJCSetPicSize(const MJC_PIC_SIZE_T * arMjcPicSize);
void vDrvMJCModeChgProc(void);
void vDrvMJCModeChangeDone(UINT32 u4Path);
void vDrvMJCChangePicSize(void);
void vDrvMJCChangeFrmSch(void);
void vDrvMJCSetModeChangeTrig(void);
void vDrvMJCSetOutputTiming(const MJC_PIC_SIZE_T * arMjcPicSize);
void vDrvMJCSetVsyncDelay(UINT32 u4FrontPorch, UINT32 u4VTotal);
void vDrvMJCGetSPReg(void);
void vDrvMJCSetMVSortOnOff(UINT32 u4OnOff);
void vDrvMJCRegStatusPolling(void *pvArgv);
void vDrvMJCSetDramStaMonitorCount(UINT32 u4Val);
void vDrvMJCEnablePatGen(UINT32 u4InOut, UINT32 u4OnOff);
void vDrvMJCClearDramStatus(void);
void vDrvMJCDramOvfUdfPolling(void *pvArgv);
void vDrvMJCInOutCRCCheck(void *pvArgv);
void vDrvMJCInputLRCheck(void *pvArgv);
void vDrvMJCDelaySetProc(void);
void vDrvMJCDIRProc(void);
void vDrvMJCAdapPPCLbox(void);
void vDrvMJCSetFrmFreeze(UINT32 u4OnOff);
void vDrvMJCSetFBOneFrmOnOff(BOOL fgOnOff);
void vDrvMJCSetDispVpos(UINT32 u4PscanPrelen);
void vDrvMJCSet3DLRDly(void);
void vDrvCliCallPME(BOOL fgIs120Hz,UINT8 fgIs55p2);
void vDrvMJCSetMuteColor(UINT8 u1R, UINT8 u1G, UINT8 u1B);
void vDrvMJCSetAutoMute(UINT8 u1Count);
void vDrvMJCSetOutMuteOnOff(BOOL fgOnOff);
void vDrvMJCDelaySet(E_MJC_DELAY_SET eDelayType, UINT8 u1Count);
void vDrvMJCSetMuteCount(UINT32 u4MuteCount);
void vDrvMJCMainloop(void);
void vDrvMJCFrmDlyChkOnOff(UINT32 u4OnOff);
void vDrvMJCFrmDlyChk(UINT32 fgInVsync);
void vDrvMJCQueryInOutCtrl(void);
void vDrvMJCQueryFrmSchCtrl(void);
void vDrvMJCQueryDramCtrl(void);
void vDrvMJCQuery3DCtrl(void);
void vDrvMJCImportProtection(UINT8 u1OnOff);
void vDrvMJCFireImportProtection(void);
void vDrvMJCSwitchFrameTrackOnOff(UINT32 u4OnOff);
void vDrvMJCSetFrmMask(UINT32 u4FrmMaskI, UINT32 u4FrmMaskO);
void vDrvMJCSetPRMode(UINT8 u1PRMode , UINT8 u1FHPRMode);
void vDrvMJCSetDbgInk(UINT8 u1INKIdex);
void vDrvMJCSetUniCandeceEn(UINT8 u1Support50To120);
void vDrvSetMJCUniCandeceModeChangeFlg(UINT8 u1OnOff);
void vDrvMJCOSMPInit(UINT8 u1MJCOSMPPRMode);
void vDrvMJCSetSG3D(MJC_FRC_CONFIG_T* arMJCFRC);
void vDrvMJCSetMCFPR(UINT32 fgOnOff, UINT32 fgLRSwap, UINT32 fgFlip, UINT32 u4FPRType);
void  vDrvMJCSetOSMP(MJC_FRC_CONFIG_T* arMJCFRC);
void vDrvMJCForceUniCandeceModeChg(void);
void vDrvUpdateRealCinemaMode(UINT16 u2Effect, UINT16 fgRealCinema, UINT16 u2TDTVPacking);
void vDrvMJCSetInputWidthHeight(UINT32 u4Width, UINT32 u4Height);
void vDrvMJCSetOutputWidthHeight(UINT32 u4Width, UINT32 u4Height);
void vDrvMJCSetMEMCOnOff(UINT8 u1OnOff);
void vDrvMJCSetOutputHTotal(UINT32 u4HTotal);
void vDrvMJCSetOutputVTotal(UINT32 u4VTotal);



BOOL fgDrvMJCIsInitialized(void);
UINT8 fgDrvMJCPscanSplitFS(void);
UINT32 fgDrvMJCIsMEMCSupported(VOID);

UINT8 u1DrvMJCGet1W1Rmode(void);
UINT8 u1DrvGetMJCUniCandeceModeChangeFlg(void);
UINT8 u1DrvMJCIsSupportUniCandece(void);
UINT8 u1DrvMJCIsNeedInverseLR(void);
UINT8 u1DrvMJCGet3DInfoChg(UINT32 u4SramMode);
UINT8 u1DrvMJCGetMuteCount(void);
UINT8 u1DrvMJCIsOnOff(void);
UINT8 u1DrvMJCGetFrmDelay(void);
UINT8 u1DrvMJCGetFrmDelay10x(void);
UINT8 u1DrvMJCChkDramOvfUdf(UINT32 * pu4StaFB01, UINT32 * pu4StaFB02, UINT32 * pu4StaSys00);
UINT8 u1DrvMJCSet32PdInfoShiftJudgmentNew(UINT8 fgIsInterlace, UINT8 u1ScalerMode, UINT8 fgMJCIs3DFSMode);


UINT16 u2DrvMJCGetOutputHTotal(void);
UINT16 u2DrvMJCGetOutputVTotal(void);

UINT32 u4DrvMJCBufferInit(void);
UINT32 u4DrvMJCSetSPReg(UINT32 u4Index, UINT32 u4Addr, UINT32 u4Msb, UINT32 u4Lsb);
UINT32 u4DrvMJCGetDramStaMonitorCount(void);
UINT32 u4DrvMJCGetVDist(void);
UINT32 fgDrvMJCGetDispMode(void);
UINT32 fgDrvMJCGetFlipMirror(void);
UINT32 u4DrvMJCRemapMJCFbmMode(UINT32 u4FbmMode);
UINT32 fgDrvMJCModeChgJudgment(UINT32 u4Path);
UINT32 u4DrvMJCGetFilmModeStatus(void);
UINT32 u4DrvMJCGetSpEn(void);

#ifdef CC_SUPPORT_STR
void vDrvMJCSuspend(void);
void vDrvMJCResume(void);
#endif

const MJC_DRAM_MODE_T * rDrvMJCGetDramMode(UINT32 u4DramModeIdx);

EXTERN UINT8 u1DrvMJCGetIntpMode(void);
#define MJC_INC_CNT ((E_MJC_2D_60 == u1DrvMJCGetIntpMode()) || (E_MJC_3D_60 == u1DrvMJCGetIntpMode()) ? 2 : 1)

extern UINT8 fgIsSupport120HzPanel(void);

void vDrvMJCGetGMV(INT32 * piGmvX, INT32 * piGmvY);

void vDrvMJCDbgLogThread(void* pvArgv);

#ifdef CC_FPGA
UINT8  u1DrvMJCRead1B(UINT32 u4Addr);
UINT16 u2DrvMJCRead2B(UINT32 u4Addr);
UINT32 u4DrvMJCRead4B(UINT32 u4Addr);
UINT32 u4DrvMJCReadFldAlign(UINT32 u4Addr, UINT32 u4Fld);
#endif

#endif //#ifndef _DRV_MJC_H_

