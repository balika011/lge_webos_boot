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
 * $RCSfile: drv_mjc.c $
 * $Revision: #1 $
 *
 *----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*
 * Header Files
 *----------------------------------------------------------------------------*/
#include "feature.h"
#include "hw_vdoin.h"
#include "hw_ckgen.h"
#include "hw_mjc.h"
#include "hw_ttd.h"
#include "hw_sw.h"
#include "hw_mjcfd.h"
#include "hw_scpos.h"
#include "drv_css.h"
#include "drv_mjc.h"
#include "drv_mfd.h"
#include "drv_ttd.h"
#include "drv_mjc_int.h"
#include "drv_mjc_frmsch.h"
#include "drv_mjc_fbck.h"
#include "drv_mjc_if.h"
#include "drv_mjc_dsr.h"
#include "drv_meter.h"
#include "drv_di.h"
#include "drv_scaler.h"
#include "drv_scaler_drvif.h"
#include "drv_display.h"
#include "drv_video.h"
#include "drv_tdtv.h"
#include "drv_tdtv_drvif.h"
#include "fbm_drvif.h"
#include "srm_drvif.h"
#include "mute_if.h"
#include "pe_if.h"
#include "vdo_misc.h"
#include "vdo_rm.h"
#include "vdp_drvif.h"
#include "vdp_display.h"
#include "vdp_if.h"
#include "video_timing.h"
#include "video_def.h"
#include "mjc_debug.h"
#include "x_bim.h"
#include "x_os.h"
#include "x_assert.h"
#include "x_hal_5381.h"
#include "x_gpio.h"
#include "x_ckgen.h"
#include "hw_di.h"
#include "frametrack_drvif.h"
#include "util.h"
#include "panel.h"
#include "hw_ddds.h"
#include "drvcust_if.h"

#include "hw_nr.h"

/*----------------------------------------------------------------------------*
 * Definitions/Macros
 *----------------------------------------------------------------------------*/
#define _ME(x,n,m,val) P_Fld(val, MJC_PROG_ME##x##EN_##n##_##m)
#define _POS(n,val) ((UINT32)(val)<<(n*2))

#define _ME_04(x,v0,v1,v2,v3) _ME(x,0,4,v0)|_ME(x,1,4,v1)|_ME(x,2,4,v2)|_ME(x,3,4,v3)
#define _ME_08(x,v0,v1,v2,v3,v4,v5,v6,v7)\
    _ME(x,0,8,v0)|_ME(x,1,8,v1)|_ME(x,2,8,v2)|_ME(x,3,8,v3)|_ME(x,4,8,v4)|_ME(x,5,8,v5)|_ME(x,6,8,v6)|_ME(x,7,8,v7)
#define _ME_10(x,v0,v1,v2,v3,v4,v5,v6,v7,v8,v9)\
    _ME(x,0,10,v0)|_ME(x,1,10,v1)|_ME(x,2,10,v2)|_ME(x,3,10,v3)|_ME(x,4,10,v4)|\
    _ME(x,5,10,v5)|_ME(x,6,10,v6)|_ME(x,7,10,v7)|_ME(x,8,10,v8)|_ME(x,9,10,v9)
#define _ME_L_20(x,v0,v1,v2,v3,v4,v5,v6,v7,v8,v9)\
    _ME(x,0,20,v0)|_ME(x,1,20,v1)|_ME(x,2,20,v2)|_ME(x,3,20,v3)|_ME(x,4,20,v4)|\
    _ME(x,5,20,v5)|_ME(x,6,20,v6)|_ME(x,7,20,v7)|_ME(x,8,20,v8)|_ME(x,9,20,v9)
#define _ME_H_20(x,v10,v11,v12,v13,v14,v15,v16,v17,v18,v19)\
    _ME(x,10,20,v10)|_ME(x,11,20,v11)|_ME(x,12,20,v12)|_ME(x,13,20,v13)|_ME(x,14,20,v14)|\
    _ME(x,15,20,v15)|_ME(x,16,20,v16)|_ME(x,17,20,v17)|_ME(x,18,20,v18)|_ME(x,19,20,v19)

#define _POS_04(v0,v1,v2,v3) (_POS(0,v0)|_POS(1,v1)|_POS(2,v2)|_POS(3,v3))
#define _POS_08(v0,v1,v2,v3,v4,v5,v6,v7)\
    (_POS(0,v0)|_POS(1,v1)|_POS(2,v2)|_POS(3,v3)|_POS(4,v4)|_POS(5,v5)|_POS(6,v6)|_POS(7,v7))
#define _POS_10(v0,v1,v2,v3,v4,v5,v6,v7,v8,v9)\
    (_POS(0,v0)|_POS(1,v1)|_POS(2,v2)|_POS(3,v3)|_POS(4,v4)|_POS(5,v5)|_POS(6,v6)|_POS(7,v7)|_POS(8,v8)|_POS(9,v9))


// Full ME mode (only for 120Hz/60Hz MEMC)
#define MJC_PROG_ME2_04_FULL        _ME_04(2,   1,1,0,0)
#define MJC_PROG_ME3_04_FULL        _ME_04(3,   1,1,0,0)
#define MJC_PROG_ME2_08_FULL        _ME_08(2,   1,1,1,1,0,0,0,0)
#define MJC_PROG_ME3_08_FULL        _ME_08(3,   1,1,1,1,0,0,0,0)
#define MJC_PROG_ME2_10_FULL        _ME_10(2,   1,1,1,1,1,0,0,0,0,0)
#define MJC_PROG_ME3_10_FULL        _ME_10(3,   1,1,1,1,1,0,0,0,0,0)
#define MJC_PROG_ME2_L_20_FULL      _ME_L_20(2, 1,1,1,1,1,1,1,1,1,1)
#define MJC_PROG_ME3_L_20_FULL      _ME_L_20(3, 1,1,1,1,1,1,1,1,1,0)  // prebi when pos = 9 (64 mode)
#define MJC_PROG_ME2_H_20_FULL      _ME_H_20(2, 0,0,0,0,0,0,0,0,0,0)
#define MJC_PROG_ME3_H_20_FULL      _ME_H_20(3, 0,0,0,0,0,0,0,0,0,0)
#define MJC_PROG_ME3_55P2_L_20_FULL _ME_L_20(3, 1,1,1,1,0,1,1,1,1,1)  // prebi when pos = 4 (55p2 mode)

#define MJC_PROG_POS_04_FULL        _POS_04(0,2,0,0)
#define MJC_PROG_POS_08_FULL        _POS_08(0,2,2,0,0,0,0,0)
#define MJC_PROG_POS_10_FULL        _POS_10(0,0,2,2,2,0,0,0,0,0)
#define MJC_PROG_POS_H_20_FULL      _POS_10(0,0,0,0,0,0,0,0,0,0)
#define MJC_PROG_POS_L_20_FULL      _POS_10(0,0,0,2,2,2,2,2,2,0)
#define MJC_PROG_POS_55P2_L_20_FULL _POS_10(0,0,2,2,0,0,0,2,2,2)

// Half run ME mode (0 can be full, 5395 mode, only for 120Hz MEMC)
#define MJC_PROG_ME2_04_HALF        _ME_04(2,   1,1,0,0)
#define MJC_PROG_ME3_04_HALF        _ME_04(3,   1,0,0,0)
#define MJC_PROG_ME2_08_HALF        _ME_08(2,   1,1,0,1,0,0,0,0)
#define MJC_PROG_ME3_08_HALF        _ME_08(3,   1,0,1,0,0,0,0,0)
#define MJC_PROG_ME2_10_HALF        _ME_10(2,   1,1,0,1,0,0,0,0,0,0)
#define MJC_PROG_ME3_10_HALF        _ME_10(3,   1,0,1,0,1,0,0,0,0,0)
#define MJC_PROG_ME2_L_20_HALF      _ME_L_20(2, 1,1,0,1,0,1,0,1,0,1)
#define MJC_PROG_ME3_L_20_HALF      _ME_L_20(3, 1,0,1,0,1,0,1,0,1,0)  // prebi when pos = 9 (64 mode)
#define MJC_PROG_ME2_H_20_HALF      _ME_H_20(2, 0,0,0,0,0,0,0,0,0,0)
#define MJC_PROG_ME3_H_20_HALF      _ME_H_20(3, 0,0,0,0,0,0,0,0,0,0)
#define MJC_PROG_ME2_55P2_L_20_HALF _ME_L_20(2, 1,1,0,0,1,0,1,0,1,0)
#define MJC_PROG_ME3_55P2_L_20_HALF _ME_L_20(3, 1,0,1,1,0,1,0,1,0,1)  // prebi when pos = 4 (55p2 mode)

#define MJC_PROG_POS_04_HALF        _POS_04(0,2,0,0)
#define MJC_PROG_POS_08_HALF        _POS_08(0,2,2,0,0,0,0,0)
#define MJC_PROG_POS_10_HALF        _POS_10(0,0,2,2,2,0,0,0,0,0)
#define MJC_PROG_POS_H_20_HALF      _POS_10(0,0,0,0,0,0,0,0,0,0)
#define MJC_PROG_POS_L_20_HALF      _POS_10(0,0,0,2,2,2,2,2,2,0)
#define MJC_PROG_POS_55P2_L_20_HALF _POS_10(0,0,2,2,0,0,0,2,2,2)

// Single ME mode with ME2 reverse (for 120Hz MEMC)
#define MJC_PROG_ME2_04_SGME        _ME_04(2,   1,0,0,0)
#define MJC_PROG_ME3_04_SGME        _ME_04(3,   0,1,0,0)
#define MJC_PROG_ME2_08_SGME        _ME_08(2,   1,0,1,0,0,0,0,0)
#define MJC_PROG_ME3_08_SGME        _ME_08(3,   0,1,0,1,0,0,0,0)
#define MJC_PROG_ME2_10_SGME        _ME_10(2,   1,0,1,0,1,0,0,0,0,0)
#define MJC_PROG_ME3_10_SGME        _ME_10(3,   0,1,0,1,0,0,0,0,0,0)
#define MJC_PROG_ME2_L_20_SGME      _ME_L_20(2, 1,0,0,1,1,0,0,1,1,1)
#define MJC_PROG_ME3_L_20_SGME      _ME_L_20(3, 0,1,1,0,0,1,1,0,0,0)  // prebi when pos = 9 (64 mode)
#define MJC_PROG_ME2_H_20_SGME      _ME_H_20(2, 0,0,0,0,0,0,0,0,0,0)
#define MJC_PROG_ME3_H_20_SGME      _ME_H_20(3, 0,0,0,0,0,0,0,0,0,0)
#define MJC_PROG_ME2_55P2_L_20_SGME _ME_L_20(2, 1,0,1,0,1,0,1,0,1,0)
#define MJC_PROG_ME3_55P2_L_20_SGME _ME_L_20(3, 0,1,0,1,0,1,0,1,0,1)  // prebi when pos = 4 (55p2 mode)

#define MJC_PROG_POS_04_SGME        _POS_04(0,0,0,0)
#define MJC_PROG_POS_08_SGME        _POS_08(0,2,2,0,0,0,0,0)
#define MJC_PROG_POS_10_SGME        _POS_10(0,0,2,2,2,0,0,0,0,0)
#define MJC_PROG_POS_H_20_SGME      _POS_10(0,0,0,0,0,0,0,0,0,0)
#define MJC_PROG_POS_L_20_SGME      _POS_10(0,0,0,2,2,2,0,2,0,0)
#define MJC_PROG_POS_55P2_L_20_SGME _POS_10(0,0,2,2,0,0,0,2,2,2)

// 240Hz MEMC Mode
#define MJC_PROG_ME2_04_240         _ME_04(2,   0,1,0,1)
#define MJC_PROG_ME3_04_240         _ME_04(3,   1,0,1,0)
#define MJC_PROG_ME2_08_240         _ME_08(2,   0,1,0,1,0,1,0,1)
#define MJC_PROG_ME3_08_240         _ME_08(3,   1,0,1,0,1,0,1,0)
#define MJC_PROG_ME2_10_240         _ME_10(2,   0,1,0,1,0,1,0,1,0,1)
#define MJC_PROG_ME3_10_240         _ME_10(3,   1,0,1,0,1,0,1,0,1,0)
#define MJC_PROG_ME2_L_20_240       _ME_L_20(2, 0,1,0,1,0,1,0,1,0,1)
#define MJC_PROG_ME3_L_20_240       _ME_L_20(3, 1,0,1,0,1,0,1,0,1,0)  // prebi when pos = 9 (64 mode)
#define MJC_PROG_ME2_H_20_240       _ME_H_20(2, 0,1,0,1,0,1,0,1,0,1)
#define MJC_PROG_ME3_H_20_240       _ME_H_20(3, 1,0,1,0,1,0,1,0,1,0)

#define MJC_PROG_POS_04_240         _POS_04(0,2,2,0)
#define MJC_PROG_POS_08_240         _POS_08(0,0,0,2,2,2,2,0)
#define MJC_PROG_POS_10_240         _POS_10(0,0,0,0,0,2,2,2,2,0)
#define MJC_PROG_POS_L_20_240       _POS_10(0,0,0,0,0,0,0,2,0,2)
#define MJC_PROG_POS_H_20_240       _POS_10(2,2,2,2,2,0,2,2,2,0)
#define MJC_PROG_POS_55P2_L_20_240  _POS_10(0,0,0,0,2,2,2,2,2,0)
#define MJC_PROG_POS_55P2_H_20_240  _POS_10(0,0,0,0,2,2,2,2,2,0)

// rdc type
#define MJC_PROG_BWRDC_N_04_120   0x0           // 0000
#define MJC_PROG_BWRDC_N_08_120   0xA0          // 00002200
#define MJC_PROG_BWRDC_N_10_120   0x280         // 0000022000
#define MJC_PROG_BWRDC_N_20_120   0x00000AA800  // 00000000002222200000
#define MJC_PROG_BWRDC_55P2_N_20_120   0x00000A0280    // 00000000002200022000


#define MJC_PROG_BWRDC_N_04_240   0xA0          // 2200
#define MJC_PROG_BWRDC_N_08_240   0xAA0         // 22220000
#define MJC_PROG_BWRDC_N_10_240   0xAA000       // 2222000000
#define MJC_PROG_BWRDC_N_20_240   0xAAAAA00000  // 22222222220000000000
#define MJC_PROG_BWRDC_55P2_N_20_240   0xAA800AA800    // 22222000002222200000

#define MJC_INIT_MUTE_COUNT    20    // Set MJC Init mute to show loader logo

/*----------------------------------------------------------------------------*
 * Global/Static variables
 *----------------------------------------------------------------------------*/
CODE REGTBL_T MJC_SYS_INIT[] =
{
    {MJC_SYS_00, 0x00171001, 0xFFFFFFFF},
    {MJC_SYS_01, 0x11000000, 0xFF000000},
    {MJC_SYS_02, 0x00438780, 0x00FFFFFF},
    {MJC_SYS_03, 0x00000000, 0x18000000},
    {MJC_SYS_04, 0x00438780, 0xFFFFFFFF},
    {MJC_SYS_05, 0x00465898, 0xFFFFFFFF},
    {MJC_SYS_07, 0x02000000, 0x02000000},
    {MJC_SYS_08, 0x00000000, 0xFFFFFFFF},
    {MJC_SYS_09, 0x40000FFF, 0x4000FFFF},
    {MJC_SYS_0E, 0x00003795, 0xFFFFFFFF},
    {MJC_SYS_0F, 0x04565F95, 0xFFFFFFFF},
    {MJC_SYS_D8, 0x00004050, 0x0000FFFF},
    {MJC_REGTBL_END, 0x00000000, 0x00000000}
};

CODE REGTBL_T MJC_FB_INIT[] =
{
    {MJC_FB_00, 0x03001000, 0xFFFFFFFF},
    {MJC_FB_01, 0x80000000, 0xFF000000},
    {MJC_FB_03, 0x80190010, 0xFFFFFFFF},
    {MJC_FB_0A, 0x00000001, 0xFFFFFFFF},
    {MJC_FB_0C, 0xC05A0C16, 0xFFFFFFFF},
    {MJC_FB_10, 0x07000000, 0x07000000},
    //{MJC_FB_14, 0x03F0CD00, 0x03FFFF00},  deflaut value is ok  need not to change ; MJC_SG_HS_NUM: self h_sync cnt
    {MJC_REGTBL_END, 0x00000000, 0x00000000}
};

CODE REGTBL_T MJC_FM_INIT[] =
{
    {MJC_FM_00, 0x01E1E300, 0xFFFFF3FF}, // FC by moment, MJC_FM_EXTRA_DLY = 1
    {MJC_FM_01, 0x00000000, 0x00000000},
    {MJC_FM_04, 0x03800000, 0x03800000},
    {MJC_FM_14, 0x00800000, 0x00800000}, // freeze setting for 420 mode
    {MJC_REGTBL_END, 0x00000000, 0x00000000}
};

CODE REGTBL_T MJC_ME_INIT[] =
{
    {MJC_ME_00, 0xCC04005A, 0xFFFFFFFF},    //adap_wdv_blk=1
    {MJC_ME_01, 0xE2200200, 0xFFFFFFFF},
    {MJC_ME_02, 0xA1004800, 0xFFFFFFFF},
    {MJC_ME_03, 0x9868c800, 0xFFFFFFFF},
    {MJC_ME_04, 0xF8681000, 0xFFFFFFFF},
    {MJC_ME_05, 0x38000040, 0xFFFFFFFF},
    {MJC_ME_06, 0x01020202, 0xFFFFFFFF},
    {MJC_ME_07, 0x70021C00, 0xFFFFFFFF},
    {MJC_ME_08, 0x16016B00, 0xFFFFFFFF}, // ME_ST_DLY_TH = 16
    {MJC_ME_09, 0x50103655, 0xFFFFFFFF},    //MJC_ME2_STIL_MV_PNLTY_TH=A
    {MJC_ME_0A, 0x04058838, 0xFFFFFFFF},
    {MJC_ME_0B, 0x00000808, 0xFFFFFFFF},
    {MJC_ME_0C, 0x08080808, 0xFFFFFFFF},
    {MJC_ME_0D, 0x800410D0, 0xFFFFFFFF},
    {MJC_ME_0E, 0x00000020, 0xFFFFFFFF},
    {MJC_ME_0F, 0x00552548, 0xFFFFFFFF},
    {MJC_ME_10, 0x88080020, 0xFFFFFFFF},
    {MJC_ME_16, 0x00800080, 0xFFFFFFFF},
    {MJC_ME_17, 0x00800080, 0xFFFFFFFF},
    {MJC_ME_18, 0x37701384, 0xFFFFFFFF},
    {MJC_ME_19, 0x7D081010, 0xFFFFFFFF},
    {MJC_ME_1A, 0x80DF180A, 0xFFFFFFFF},
    {MJC_ME_1B, 0x83001E08, 0xFFFFFFFF},
    {MJC_ME_1C, 0x55400100, 0xFFFFFFFF},
    {MJC_ME_1D, 0x9E801010, 0xFFFFFFFF},
    {MJC_ME_1E, 0x9A050060, 0xFFFFFFFF},    //disable ME LPF
    {MJC_ME_1F, 0x02020202, 0xFFFFFFFF},
    {MJC_ME_20, 0x0AF80100, 0xFFFFFFFF},
    {MJC_ME_21, 0x001D2244, 0xFFFFFFFF},    //occ_other_sel=1
    {MJC_ME_22, 0x08081CD0, 0xFFFFFFFF},    //vlrg_nzmv_th=0x1c
    {MJC_ME_23, 0x83001E08, 0xFFFFFFFF},
    {MJC_ME_24, 0x5A000040, 0xFFFFFFFF},
    {MJC_ME_25, 0x3C001E0F, 0xFFFFFFFF},
    {MJC_ME_26, 0x1F281F07, 0xFFFFFFFF},    //me_ppc_core_en=1
    {MJC_ME_27, 0x1E015157, 0xFFFFFFFF},
    {MJC_ME_28, 0x1E06000C, 0xFFFFFFFF},
    {MJC_ME_29, 0x30444304, 0xFFFFFFFF},
    {MJC_ME_2A, 0x04041010, 0xFFFFFFFF},
    {MJC_ME_2B, 0x00010120, 0xFFFFFFFF},
    {MJC_ME_2C, 0xF00A1020, 0xFFFFFFFF},
    {MJC_ME_2D, 0xA50008F8, 0xFFFFFFFF},
    {MJC_ME_2E, 0x00000244, 0x400003FF},
    {MJC_ME_2F, 0x0430BC60, 0x07FFFFFF},
    {MJC_ME_30, 0x08410841, 0xFFFFFFFF},
    //NEW
    {MJC_ME_PPCGMV, 0x310A1000, 0xFFFFFFFF},
    {MJC_CA_PAD, 0x00000000, 0x07000000},
    {MJC_MV_PAD, 0x00000000, 0x000000FF},
    {MJC_MV_PAD2, 0x00000018, 0x000000FF},
    {MJC_PPC_GMV_5, 0x00000000, 0x03FF0000},
    {MJC_REGTBL_END, 0x00000000, 0x00000000}
};

CODE REGTBL_T MJC_MV_INIT[] =
{
    {MJC_MV_00, 0x08110000, 0xFFFFFFFF},
    {MJC_MV_01, 0x4411FFFF, 0xFFFFFFFF},
    {MJC_MV_02, 0x035ABC3C, 0xFFFFFFFF},
    {MJC_MV_03, 0x20401C00, 0xFFFFFFFF},
    {MJC_MV_04, 0x60124434, 0xFFFFFFFF}, // MV_04:
    {MJC_MV_05, 0x03811022, 0xFFFFFFFF}, // MV_05: 00000022
    {MJC_MV_06, 0x00001818, 0xFFFFFFFF}, // MV_06: 00001605
    {MJC_MV_07, 0x48202141, 0xFFFFFFFF}, // MV_07: vsi sel center
    {MJC_MV_08, 0x33881A68, 0xFFFFFFFF}, // MV_08: 33881C8A
    {MJC_MV_09, 0x30198020, 0xFFFFFFFF}, // MV_09:
    {MJC_MV_0A, 0x10220534, 0xFFFFFFFF}, // MV_0A:
    {MJC_MV_0B, 0x00010630, 0xFFFFFFFF}, // MV_0B: 42a setting
    #ifdef DISABLE_MUSTANG_FPGA
    {MJC_MV_0C, 0x0A10E000, 0xFFFFFFFF}, // MV_0C: bmval enable [1:0]
    {MJC_MV_0D, 0x08410841, 0xFFFFFFFF},
    #endif
    {MJC_MV_0E, 0x00120000, 0xFFFFFFFF}, // MV_TF_START_ADDR=0x120000
    {MJC_MV_0F, 0x0012FFFF, 0xFFFFFFFF}, // MV_TF_END_ADDR=0x12FFFF
    #ifdef DISABLE_MUSTANG_FPGA
    {MJC_MV_10, 0x00000000, 0xFFFFFFFF}, // MV_10:
    #endif
    {MJC_MV_11, 0x00000028, 0xFFFFFFFF}, // MV_11:

    {MJC_MV_22, 0x08080808, 0xFFFFFFFF},
    {MJC_MV_24, 0x00050131, 0xFFFFFFFF}, // APL setting
    {MJC_MV_25, 0x0010005E, 0xFFFFFFFF}, // 3*1920*1080/(256*256)
    {MJC_MV_26, 0x0010017B, 0xFFFFFFFF}, // 12*1920*1080/(2^16)
    {MJC_MV_27, 0x0000011C, 0xFFFFFFFF}, // 9*1920*1080/(2^16)
    {MJC_MV_2A, 0x00000000, 0xFFFFFFFF}, // MV_2A:
    {MJC_MV_2B, 0x50130202, 0xFFFFFFFF},
    {MJC_MV_2C, 0x84888844, 0xFFFFFFFF}, // MV_2C:
    {MJC_MV_2D, 0x1000A051, 0xFFFFFFFF}, // MV_2D: [6]:bgmv_fgbg_chk=1,[7]:bg_keep_sel=0,
    {MJC_MV_2E, 0x34343018, 0xFFFFFFFF}, // MV_2E: [4]:mvc_tpos_en=1, [25:24]:mvc_sml_det_mod=0, mvc_vrng_th=0x1c->0x10
    {MJC_MV_2F, 0x44441224, 0xFFFFFFFF}, // MV_2F: toc_ex gain
    {MJC_MV_30, 0x0A0A6844, 0xFFFFFFFF}, // MV_30: fg err detect
    {MJC_MV_31, 0x443A0404, 0xFFFFFFFF}, // MV_31: [17]: range_tops_en [19]:toc_tpos=1,[20]:bgf_tpos=1,
    {MJC_MV_32, 0x00301777, 0xFFFFFFFF}, // MV_32: osd protection
    {MJC_MV_33, 0x24484461, 0xFFFFFFFF}, // MV_33: gain2,   //toc_bi_out_bnd_chk=1
    {MJC_MV_34, 0x80808080, 0xFFFFFFFF}, // MV_34: toc_ex max/min,
    {MJC_MV_36, 0x00000002, 0xFFFFFFFF}, // MV_36: [1]:exp_mvc=1,
    {MJC_MV_37, 0x23455653, 0xFFFFFFFF}, // MV_37:
    {MJC_MV_38, 0x00682824, 0xFFFFFFFF}, // MV_38:
    {MJC_MV_39, 0x00030A68, 0xFFFFFFFF}, // MV_39: BND_HLR for right and down (ECO)
    {MJC_MV_3B, 0x38686543, 0xFFFFFFFF}, // MV_3B:
    {MJC_MV_3C, 0x1F0000CC, 0xFFFFFFFF}, // MV_3C: disable near clst sel
    {MJC_MV_3D, 0x23686868, 0xFFFFFFFF}, // MV_3D:
    {MJC_DSR_FW_BND_0, 0x00000000, 0xFFFFFFFF},
    {MJC_DSR_FW_BND_1, 0x00000000, 0x1FFFFFFF},
    {MJC_REGTBL_END, 0x00000000, 0x00000000}
};

CODE REGTBL_T MJC_PSG_INIT[] =
{
    #ifdef CC_FPGA
    // BDR VSI
    {MJC_MV_04, 0x40023322, 0xFFFFFFFF}, // MV_04:
    #endif
    // PSG
    {MJC_MV_13, 0x02020013, 0xFFFFFCFF}, // MV_13:
    {MJC_MV_14, 0xA22A0088, 0xFFFFFFFF}, // MV_14:
    {MJC_MV_15, 0x75321018, 0xFFFFFFFF}, // MV_15:
    {MJC_MV_16, 0xCA865432, 0xFFFFFFFF}, // MV_16:
    {MJC_MV_17, 0x0020200F, 0xFFFFFFFF}, // MV_17:
    {MJC_MV_18, 0xF1F7F2F7, 0xFFFFFFFF}, // MV_18:
    {MJC_MV_19, 0x0000F03F, 0xFFFFFFFF}, // MV_19:
    {MJC_MV_1A, 0x00070F77, 0xFFFFFFFF}, // MV_1A:
    {MJC_MV_1B, 0x00668FAC, 0xFFFFFFFF}, // MV_1B:
    {MJC_REGTBL_END, 0x00000000, 0x00000000}
};

CODE REGTBL_T MJC_CA_INIT[] =
{
    {MJC_CA_00, 0x200C0202, 0xFFFFFFFF}, // CA_00:
    {MJC_CA_01, 0x08081000, 0xFFFFFFFF}, // CA_01:
    {MJC_CA_02, 0x00102000, 0xFFFFFFFF}, // CA_02:
    {MJC_CA_03, 0x04040701, 0xFFFFFFFF}, // CA_03:
    {MJC_CA_04, 0x88003301, 0xFFFFFFFF}, // CA_04: 88222251
    {MJC_CA_05, 0x68461125, 0xFFFFFFFF}, // CA_05:
    {MJC_CA_06, 0x71181818, 0xFFFFFFFF}, // CA_06:
    {MJC_CA_07, 0x66660001, 0xFFFFFFFF}, // CA_07: [0]:fgbg_msad_chk,
    {MJC_CA_08, 0x03041000, 0xFFFFFFFF}, // CA_08:
    {MJC_CA_09, 0x00056868, 0xFFFFFFFF}, // CA_09:
    {MJC_CA_0A, 0xC1228833, 0xFFFFFFFF}, // CA_0A:
    {MJC_CA_0B, 0x33090909, 0xFFFFFFFF}, // CA_0B:
    {MJC_CA_0C, 0x05000028, 0xFFFFFFFF}, // CA_0C:  CA_OSD_STL_CNT_DEC=5
    {MJC_CA_0D, 0x3F1F0004, 0xFFFFFFFF}, // CA_0D:
    {MJC_CA_0E, 0x0FFF7F00, 0xFFFFFFFF}, // CA_0E:
    {MJC_CA_0F, 0x00000822, 0xFFFFFFFF}, // CA_0F:
    {MJC_CA_AFB_1, 0x00000000, 0x30000000}, //CA_AFB_EFBC_CHK =0
    {MJC_CA_AFB_2, 0x00000460, 0x20000FFF},
    {MJC_CA_OSD_STL_1, 0x00000000, 0x08000000}, //CA_OSD_OCC_DET_3=0
    {MJC_REGTBL_END, 0x00000000, 0x00000000}
};

CODE REGTBL_T MJC_MC_INIT[] =
{
    {MJC_MC_00, 0x00000606, 0xFFFFFFFF}, // MC_00:
    {MJC_MC_01, 0x06003025, 0xFFFFFFFF}, // MC_01:
    {MJC_MC_02, 0x79000000, 0xFFFFFFFF}, // MC_02: intp mirror ,opint_en=1
    {MJC_MC_03, 0x00003340, 0xFFFFFFFF}, // MC_03: 42a setting (current: still only)
    {MJC_MC_04, 0x02552222, 0xFFFFFFFF}, // MC_04: MC_1B mc lbox offset (wait debug)
    {MJC_MC_05, 0x10FB8802, 0xFFFFFFFF}, // MC_05: MC_05 ovl_uni_en, occ_mirror_off
    {MJC_MC_06, 0x00010000, 0xFFFFFFFF}, // MC_06: vdb ctrl
    {MJC_MC_07, 0xFFFF2D50, 0xFFFFFFFF}, // MC_07: vdb v,u,y,x
    {MJC_MC_08, 0xFFFF0044, 0xFFFFFFFF}, // MC_08: vdb code
    {MJC_MC_09, 0x00000000, 0xFFFFFFFF}, // MC_09: vdb misc
    {MJC_MC_0A, 0x00000000, 0xFFFFFFFF}, // MC_0A: force mv
    {MJC_MC_0B, 0x28000000, 0xFFFFFFFF}, // MC_0B: demo bound
    {MJC_MC_0C, 0x00000000, 0xFFFFFFFF}, // MC_0C: demo bar
    {MJC_MC_16, 0x00000000, 0xFFFFFFFF}, // MC_16: osd
    {MJC_MC_17, 0x00000000, 0xFFFFFFFF}, // MC_17: osd
    {MJC_MC_18, 0x00000000, 0xFFFFFFFF}, // MC_18: osd
    {MJC_MC_19, 0x00000000, 0xFFFFFFFF}, // MC_19: osd
    {MJC_MC_1A, 0x00000000, 0xFFFFFFFF}, // MC_1A: osd
    {MJC_MC_1B, 0x00000000, 0xFFFFFFFF}, // MC_1B: osd
    {MJC_MC_1C, 0x00000000, 0xFFFFFFFF}, // MC_1C: osd
    {MJC_MC_1D, 0x00000000, 0xFFFFFFFF}, // MC_1D: osd
    {MJC_MC_1E, 0x00002ACA, 0xFFFFFFFF}, // MC_1E: bd_clip
    {MJC_MC_1F, 0xAA01AA01, 0xFFFFFFFF}, // MC_1F: bd_clip 0x201CC0BE
    {MJC_MC_20, 0x00008003, 0xFFFFFFFF}, // MC_20:video PartFbk
    {MJC_MC_21, 0xC0804000, 0xFFFFFFFF}, // MC_21:pd22 PartFbk
    {MJC_MC_22, 0x00000000, 0xFFFFFFFF}, //
    {MJC_MC_23, 0x99663300, 0xFFFFFFFF}, // MC_24:pd32 55mode PartFbk
    {MJC_MC_24, 0x000000CC, 0xFFFFFFFF}, //
    {MJC_MC_25, 0x00000000, 0xFFFFFFFF}, //
    {MJC_MC_26, 0x66333300, 0xFFFFFFFF}, // MC_36:pd32 64mode PartFbk
    {MJC_MC_27, 0xCC999966, 0xFFFFFFFF}, //
    {MJC_MC_28, 0x0000FFCC, 0xFFFFFFFF}, //
    {MJC_MC_29, 0x00000000, 0xFFFFFFFF}, //
    {MJC_MC_2A, 0x00000000, 0xFFFFFFFF}, //
    {MJC_REGTBL_END, 0x00000000, 0x00000000}
};

CODE REGTBL_T MJC_TBA_INIT[] =
{
    {MJC_ME_TBA_0, 0x00232A06, 0xFFFFFFFF}, //MJC_GMV_BI_M3_VSYNC_SEL=0
    {MJC_ME_TBA_1, 0x34680468, 0xFFFFFFFF},
    {MJC_ME_TBA_2, 0x0340205A, 0x0000000F}, //MJC_ME_BND_FG_ZERO_SAD_EN=0
    {MJC_MV_TBA_0, 0x04020808, 0xFFFFFFFF},
    {MJC_MV_TBA_1, 0x00030000, 0xFFFFFFFF},
    {MJC_MV_TBA_2, 0x00000000, 0xFFFFFFFF},
    {MJC_MV_TBA_3, 0x35460256, 0xFFFFFFFF},
    {MJC_MV_TBA_4, 0x666625AD, 0xFFFFFFFF},
    {MJC_MV_TBA_5, 0x01606000, 0xFFFFFFFF},
    {MJC_MV_TBA_6, 0x00606FFF, 0xFFFFFFFF},
    {MJC_MV_TBA_7, 0x0FF03323, 0xFFFFFFFF},
    {MJC_MV_TBA_8, 0x00000636, 0xFFFFFFFF},
    {MJC_MV_TBA_9, 0x00800000, 0xFFFFFFFF},
    {MJC_CA_TBA_0, 0x6856AC45, 0xFFFFFFFF},
    {MJC_CA_TBA_1, 0x00440058, 0xFFFFFFFF},
    {MJC_MV_TBA_A, 0x05607000, 0xFFFFFFFF},
    {MJC_MV_TBA_B, 0x00607FFF, 0xFFFFFFFF},
    {MJC_MV_TBA_C, 0x000000FF, 0xFFFFFFFF},
    {MJC_MV_TBA_D, 0x81291061, 0xFFFFFFFF},
    {MJC_MV_TBA_E, 0x00000F00, 0xFFFFFFFF}, //DIR enable
    {MJC_MV_TBA_F, 0xC7FFF703, 0xFFFFFFFF},
    {MJC_MV_TBA_10, 0xC7FFD7C3, 0xFFFFFFFF},
    {MJC_MV_DSC_2,  0x00001410, 0x0000FFFF},
    {MJC_ME_CLP_2 , 0x18000000, 0xFF000000},    //MJC_ME_BND_FG_SAD_TH3=0x18

    {MJC_REGTBL_END, 0x00000000, 0x00000000}
};

MJC_SP_THREAD MJCSPThread[] =
{
    {vDrvMJCFDStatusPolling,    "MJCFD Status Polling"},
    {vDrvMJCRegStatusPolling,   "MJC Register Status Polling"},
    {vDrvMJCMEStaPolling,       "MJC ME Status Polling"},
    {vDrvMJCFrmSchHWLog,        "MJC HW FrmSch Polling, Please use Vsync(1)"},
    {vDrvMJCFrmSchFWLog,        "MJC FW FrmSch Polling, Please use Vsync(1)"},
    {vDrvMJCFrmSchHWFWLog,      "MJC HWFW FrmSch Polling, Please use Vsync(1)"},
    {vDrvMJCFrmSchCadenceLog,   "vDrvMJCFrmSchCadenceLog"},
    {NULL,                      NULL},
    {vDrvMJCDramOvfUdfPolling,  "MJC DRAM Over/Underflow Monitoring"},
    {vDrvMJCInputLRCheck,       "MJC Input 3D FS LR Check"},
    {vDrvMJCInOutCRCCheck,      "MJC IN/OUT CRC check, must open front scaler patgen"}
};

UINT8 fgMJCStatusPolling;
UINT8 u1TestDelayCnt     = 20;
UINT8 fgMJCModeChg       = SV_FALSE;
UINT8 fgVdoModeChg       = SV_FALSE;
UINT8 fgMJCChangeFrmSch  = SV_FALSE;
UINT8 fgMJCChangePicSize = SV_FALSE;
UINT8 u1PdInfoDly        = 0;

MJC_DRAM_ALLOC_T arMJCDramAlloc;
MJC_PRM_T        arMJCPrm = {SV_FALSE};
MJC_SP_REG_T     arMJCSPReg[8];
MJC_DEMO_CFG_T   arMJCDemo =
{
    E_MJC_DEMO_FULL,
    {E_MJC_RGN_NF, E_MJC_RGN_NF, E_MJC_RGN_NF},
    0, 0, 0, 0
};

UINT8 bSupportDejudder = FALSE;
static UINT8 _u1TrigSetPsAuto = 0;

static UINT8 _u1DelaySetCnt[E_MJC_DELAY_MAX] = {0};

static UINT32 _u4DramStaMonitorCount = 0xFFFFFFFF;

static BOOL _fgIsInitialized = FALSE;
//static HANDLE_T _hDrvMJCSema;

static HANDLE_T _hMJCOutputPollingSema;
static PThreadFunc _pfnMJCAdjInputVsync = NULL;
static PThreadFunc _pfnMJCOutputVsync   = NULL;
static PThreadFunc _pfnMJCInputVsync    = NULL;
static PThreadFunc _pfnMJCProcFinished  = NULL;

static UINT32 _u4MJCStatusPollingEnable = 0;

UINT32 u4MJCStatusPollingCount = 0;
UINT8  u1MJCStatusCheck = 0;
UINT8  u1TrigSetUniCandeceTrg = 0;
UINT32 u4InputTrigger = 0;
UINT32 fgOutputTimingChange = SV_FALSE;

extern LCDTIMING_INFO_T lcdtimingInfo;

MJC_SP_VSYNC_T MJCSPVsyncIsr[] =
{
    {&_pfnMJCAdjInputVsync, "MJC Adjusted Input Vsync End"},
    {&_pfnMJCOutputVsync,   "MJC Output Vsync End"},
    {&_pfnMJCInputVsync,    "MJC Input Vsync End"},
    {&_pfnMJCProcFinished,  "MJC Processing Finished"},
    {NULL,                  NULL}
};

static MJC_DRAM_MODE_T _arMJCAllDramMode[MJC_DRAM_ALL_MODE_NUM + 1] =
{
    {MJC_Y10_C10_422_0RDC_3FRM, "MJC_Y10_C10_422_0RDC_3FRM"},
    {MJC_Y10_C10_422_1RDC_3FRM, "MJC_Y10_C10_422_1RDC_3FRM"},
    {MJC_Y10_C10_420_0RDC_3FRM, "MJC_Y10_C10_420_0RDC_3FRM"},
    {MJC_Y10_C10_420_1RDC_3FRM, "MJC_Y10_C10_420_1RDC_3FRM"},
    {MJC_Y10_C10_422_0RDC_2FRM, "MJC_Y10_C10_422_0RDC_2FRM"},
    {MJC_Y10_C10_422_1RDC_2FRM, "MJC_Y10_C10_422_1RDC_2FRM"},
    {MJC_Y10_C10_420_0RDC_2FRM, "MJC_Y10_C10_420_0RDC_2FRM"},
    {MJC_Y10_C10_420_1RDC_2FRM, "MJC_Y10_C10_420_1RDC_2FRM"},
    {MJC_Y10_C10_422_0RDC_1FRM, "MJC_Y10_C10_422_0RDC_1FRM"},
    {MJC_Y10_C10_420_0RDC_1FRM, "MJC_Y10_C10_420_0RDC_1FRM"},
    {MJC_Y10_C08_422_0RDC_3FRM, "MJC_Y10_C08_422_0RDC_3FRM"},
    {MJC_Y10_C08_422_1RDC_3FRM, "MJC_Y10_C08_422_1RDC_3FRM"},
    {MJC_Y10_C08_420_0RDC_3FRM, "MJC_Y10_C08_420_0RDC_3FRM"},
    {MJC_Y10_C08_420_1RDC_3FRM, "MJC_Y10_C08_420_1RDC_3FRM"},
    {MJC_Y10_C08_422_0RDC_2FRM, "MJC_Y10_C08_422_0RDC_2FRM"},
    {MJC_Y10_C08_422_1RDC_2FRM, "MJC_Y10_C08_422_1RDC_2FRM"},
    {MJC_Y10_C08_420_0RDC_2FRM, "MJC_Y10_C08_420_0RDC_2FRM"},
    {MJC_Y10_C08_420_1RDC_2FRM, "MJC_Y10_C08_420_1RDC_2FRM"},
    {MJC_Y10_C08_422_0RDC_1FRM, "MJC_Y10_C08_422_0RDC_1FRM"},
    {MJC_Y10_C08_420_0RDC_1FRM, "MJC_Y10_C08_420_0RDC_1FRM"},
    {MJC_Y08_C08_422_0RDC_3FRM, "MJC_Y08_C08_422_0RDC_3FRM"},
    {MJC_Y08_C08_422_1RDC_3FRM, "MJC_Y08_C08_422_1RDC_3FRM"},
    {MJC_Y08_C08_420_0RDC_3FRM, "MJC_Y08_C08_420_0RDC_3FRM"},
    {MJC_Y08_C08_420_1RDC_3FRM, "MJC_Y08_C08_420_1RDC_3FRM"},
    {MJC_Y08_C08_422_0RDC_2FRM, "MJC_Y08_C08_422_0RDC_2FRM"},
    {MJC_Y08_C08_422_1RDC_2FRM, "MJC_Y08_C08_422_1RDC_2FRM"},
    {MJC_Y08_C08_420_0RDC_2FRM, "MJC_Y08_C08_420_0RDC_2FRM"},
    {MJC_Y08_C08_420_1RDC_2FRM, "MJC_Y08_C08_420_1RDC_2FRM"},
    {MJC_Y08_C08_422_0RDC_1FRM, "MJC_Y08_C08_422_0RDC_1FRM"},
    {MJC_Y08_C08_420_0RDC_1FRM, "MJC_Y08_C08_420_0RDC_1FRM"},
    {FRC_Y10_C10_444, "FRC_Y10_C10_444"},
    {FRC_Y08_C08_444, "FRC_Y08_C08_444"},
    {FRC_Y10_C10_422RPT, "FRC_Y10_C10_RPT422"},
    {FRC_Y08_C08_422RPT, "FRC_Y08_C08_RPT422"},
    {0xFF, "Unknow"}
};

static UINT32 fgMJCFrmDlyChk = SV_OFF;

//EXTERN void vScpipOnMjcOutVsync(void);


/*----------------------------------------------------------------------------*
 * Function Members
 *----------------------------------------------------------------------------*/
static UINT32 u4DrvMJCCalcSizeYC(UINT32 u4Width, UINT32 u4Height, UINT32 u4Dram444, UINT32 u4DramRpt422, UINT32 u4Y8, UINT32 u4C8, UINT32 u4Dyn)
{
    UINT8 u1Unit, u1FracQMBNum, u1FracQMBMbNum, u1FullBit, u1FracBit ,u1Bit;
    UINT32 u4BlkNum, u4FldNum ,u4FullQMBNum;
    UINT32 u4Size;

    u4Height = ((FBM_MJC_DRAM_3D_CFG & (FBM_MJC_DRAM_OSMP_FPR | FBM_MJC_DRAM_3D_HPR))? (u4Dram444 ? (u4Height >> 1) : ALIGN_8_CEIL(u4Height >> 1)) : u4Height );
    u4Width = ALIGN_16_CEIL(u4Width);

    if (u4Dram444)
    {
        u4FullQMBNum = (u4Width >> 4) >> 2;
        u1FracQMBMbNum = (u4Width >> 4) % 4;
        u1FracQMBNum = (u1FracQMBMbNum != 0)? 1 : 0;
        u1Bit = (u4Dyn? (u4DramRpt422? 2 : 3) : ((u4Y8? 0 : 1) + (u4C8? 0 : 1)));
        u1FullBit = (u4DramRpt422? 8 : 12) + u1Bit;
        u1FracBit = (u4DramRpt422? (u1FracQMBMbNum * 2) : (u1FracQMBMbNum * 3)) + u1Bit;
        u4Size = u4FullQMBNum * u1FullBit + u1FracQMBNum * u1FracBit;
        u4Size = ALIGN_4_CEIL(u4Size);
        u4Size = 3 * u4Size * u4Height;
    }
    else                                                               // 420 same size to 422
    {
        u1Unit = (u4Dyn == SV_OFF)? 8 : 6;
        u1Unit = IO32ReadFldAlign(MJC_FB_21 , MJC_C_DA_PG_SIZE)? (u1Unit << 1) : u1Unit;
        u4BlkNum = (ALIGN_64_CEIL(u4Width) >> 6) * (ALIGN_8_CEIL(u4Height) >> 3);
        u4FldNum = ((u4BlkNum / u1Unit + ((u4BlkNum % u1Unit)? 1 : 0)) << 2) << 7;
        u4FldNum = u4FldNum << (IO32ReadFldAlign(MJC_FB_21 , MJC_C_DA_PG_SIZE)? 1 : 0);
        u4Size  = 9 * u4FldNum;
    }
    u4Size = u4Size << ((FBM_MJC_DRAM_3D_CFG & FBM_MJC_DRAM_OSMP_FPR)? 2 : (FBM_MJC_DRAM_3D_CFG & (FBM_MJC_DRAM_3D_SG | FBM_MJC_DRAM_3D_HPR))? 1 : 0); //  base 128 bits
    u4Size = u4Size << 4;         // tranfer to base byte

    return (u4Size);
}

UINT32 u4DrvMJCCalcSizeFilm(UINT32 u4Width, UINT32 u4Height, UINT32 u4Dram444)
{
    UINT32 u4Size = (u4Dram444) ? 0 : (ALIGN_8_CEIL(u4Width) * ALIGN_8_CEIL(u4Height));

    return u4Size;
}

UINT32 u4DrvMJCCalcSizeOSD(UINT32 u4Width, UINT32 u4Height, UINT32 u4Dram444)
{
    UINT32 u4Size = (u4Dram444) ? 0 : (ALIGN_64_CEIL(ALIGN_8_CEIL(u4Width) >> 2) * (ALIGN_8_CEIL(u4Height) >> 3));
    return u4Size;
}

static UINT32 u4DrvMJCCalcSizeMV(UINT32 u4Width, UINT32 u4Height, UINT32 u4Dram444)
{
    UINT32 u4Size = (u4Dram444) ? 0 : (ALIGN_256_CEIL(u4Width) >> 3) * (ALIGN_8_CEIL(u4Height) >> 3) * 2;

    return u4Size;
}

UINT32 u4DrvMJCCalcSizeTBA(UINT32 u4Width, UINT32 u4Height, UINT32 u4Dram444)
{
    UINT32 u4Size = (u4Dram444) ? 0 : (ALIGN_64_CEIL(ALIGN_8_CEIL(u4Width) >> 3) * (ALIGN_8_CEIL(u4Height) >> 3));

    return u4Size;
}

UINT32 u4DrvMJCRemapMJCFbmMode(UINT32 u4FbmMode)
{
    #ifndef CC_FPGA
    switch (u4FbmMode & 0x7FFF0000)
    {
    case FBM_POOL_MODE_MJC_Y10_C10_422_0RDC_3FRM: return MJC_Y10_C10_422_0RDC_3FRM;
    case FBM_POOL_MODE_MJC_Y10_C10_422_1RDC_3FRM: return MJC_Y10_C10_422_1RDC_3FRM;
    case FBM_POOL_MODE_MJC_Y10_C10_420_0RDC_3FRM: return MJC_Y10_C10_420_0RDC_3FRM;
    case FBM_POOL_MODE_MJC_Y10_C10_420_1RDC_3FRM: return MJC_Y10_C10_420_1RDC_3FRM;
    case FBM_POOL_MODE_MJC_Y10_C08_422_0RDC_3FRM: return MJC_Y10_C08_422_0RDC_3FRM;
    case FBM_POOL_MODE_MJC_Y10_C08_422_1RDC_3FRM: return MJC_Y10_C08_422_1RDC_3FRM;
    case FBM_POOL_MODE_MJC_Y10_C08_420_0RDC_3FRM: return MJC_Y10_C08_420_0RDC_3FRM;
    case FBM_POOL_MODE_MJC_Y10_C08_420_1RDC_3FRM: return MJC_Y10_C08_420_1RDC_3FRM;
    case FBM_POOL_MODE_MJC_Y08_C08_422_0RDC_3FRM: return MJC_Y08_C08_422_0RDC_3FRM;
    case FBM_POOL_MODE_MJC_Y08_C08_422_1RDC_3FRM: return MJC_Y08_C08_422_1RDC_3FRM;
    default:
    case FBM_POOL_MODE_FRC_Y10_C10_444: return ((arMJCPrm.arVrmSrmConfig.u1MJC3DModeOut == E_VDO_MJC_OUT_2D)? FRC_Y10_C10_444 : FRC_Y10_C10_422RPT);
    case FBM_POOL_MODE_FRC_Y08_C08_444: return ((arMJCPrm.arVrmSrmConfig.u1MJC3DModeOut == E_VDO_MJC_OUT_2D)? FRC_Y08_C08_444 : FRC_Y08_C08_422RPT);
    case FBM_POOL_MODE_FRC_Y08_C08_422: return FRC_Y08_C08_422RPT;
    case FBM_POOL_MODE_MJC_Y08_C08_420_1RDC_3FRM: return MJC_Y08_C08_420_1RDC_3FRM;
    case FBM_POOL_MODE_FRC_Y10_C10_422: return FRC_Y10_C10_422RPT;
    }
    #else
    return FRC_Y10_C10_444;
    #endif
}

UINT32 u4DrvMJCGetSpEn(void)
{
    return _u4MJCStatusPollingEnable;
}

void vDrvMJCSetSpEn(UINT32 u4Msk, UINT32 u4Count)
{
    _u4MJCStatusPollingEnable |= u4Msk;
    u4MJCStatusPollingCount = u4Count;
}

void vDrvMJCClearSpEn(UINT32 u4Msk)
{
    _u4MJCStatusPollingEnable &= ~u4Msk;
}

BOOL fgDrvMJCIsInitialized(void)
{
    return _fgIsInitialized;
}

UINT32 fgDrvMJCIsMEMCSupported(VOID)
{
    UINT32 u4InputFrameRate;
    UINT32 u4TDTVMJCMode;
    UINT32 u4TDTVTTTMode;
    SRM_VDP_INFO_T * prMainVdpInfo;

    u4InputFrameRate = bDrvVideoGetRefreshRate(SV_VP_MAIN);
    u4TDTVMJCMode = u4DrvTDTVMJCModeQuery();
    u4TDTVTTTMode = u4DrvTDTVTTTModeQuery(SV_VP_MAIN);
    prMainVdpInfo = SRM_GetVdpInfo(VDP_1);

    if (u4InputFrameRate > 60 && u4DrvTDTV3DModeQuery() == E_TDTV_UI_3D_MODE_OFF)
    {
        return FALSE;
    }

    if (fgDrvDisplayGetPentouchMode())
    {
        return FALSE;
    }
    if(SRM_IsEpgMode())
    {
      return FALSE;
    }
    // Not support 48 -> 60 MEMC
    if (u4InputFrameRate == 48)
    {
        if (PANEL_IsSupport120Hz())
        {
            if ((TD_TTT_IN(u4TDTVTTTMode) | 0x1) != E_TD_IN_FS_P && TD_MJC_OUT(u4TDTVMJCMode) == E_TD_OUT_3D_FS)
            {
                return FALSE;
            }
        }
        else
        {
            if ((TD_TTT_IN(u4TDTVTTTMode) | 0x1) == E_TD_IN_2D_P)
            {
                return FALSE;
            }
            if ((TD_TTT_IN(u4TDTVTTTMode) | 0x1) != E_TD_IN_FS_P && TD_TTT_OUT(u4TDTVTTTMode) == E_TD_OUT_3D_2_2D)
            {
                return FALSE;
            }
        }
    }

    if ((u4InputFrameRate < 48) && ((TD_TTT_IN(u4TDTVTTTMode) | 0x1) == E_TD_IN_FS_P))
    {
        return FALSE;
    }

    if (u4DrvTDTVIs3DPhoto(NULL) == SV_TRUE)
    {
        return FALSE;
    }

    {
        UINT32 u4MJCInputWidth;
        UINT32 u4MJCInputHeight;

        if (TD_MJC_IN(u4TDTVMJCMode) == E_TD_IN_2D_P)
        {
            u4MJCInputWidth = wDISPLAY_WIDTH;
            u4MJCInputHeight = wDISPLAY_HEIGHT;
        }
        else
        {
            if (VDP_GetScalerPixelBased())
            {
                u4MJCInputWidth = prMainVdpInfo->u4OutputWidth;
                u4MJCInputHeight = prMainVdpInfo->u4OutputHeight;
            }
            else
            {
                u4MJCInputWidth = (wDISPLAY_WIDTH * prMainVdpInfo->u4OutputWidth) / VDP_MAX_REGION_WIDTH;
                u4MJCInputHeight = (wDISPLAY_HEIGHT * prMainVdpInfo->u4OutputHeight) / VDP_MAX_REGION_HEIGHT;
            }
        }


        if ((TD_MJC_IN(u4TDTVMJCMode) == E_TD_IN_SBS_P))
        {
            if (u4MJCInputWidth < 512)
            {
                return FALSE;
            }
        }
        else
        {
        
            if (u4MJCInputWidth < 256 || u4MJCInputWidth > 1920 || u4MJCInputHeight < 64)
            {
                return FALSE;
            }
        }
    }

    return TRUE;
}

void vDrvMJCSetPsAutoTrigOn(void)
{
    _u1TrigSetPsAuto = 1;
}

void vDrvMJCRunMbist(void)    // Cobra mbist has been removed
{
    /*
    static UINT32 fgMbistDone = 0xFF;
    #if MJC_2ND_MBIST_CHK
    UINT32 fg2ndTest = SV_TRUE;
    #endif

    if (fgMbistDone == 0xFF)
    {
        #if defined(CC_FPGA) || !defined(MJC_INIT_MBIST_FLOW)
        fgMbistDone = SV_FALSE;
        return;
        #endif
    }

    if (fgMbistDone == SV_TRUE)
    {
        return;
    }

    fgMbistDone = SV_TRUE;


    // disable MBIST reset
    vIO32WriteFld(CKGEN_MBIST_CTRL, SV_ON, FLD_MBIST_RSTB);

    // enable mbist repair function
    vIO32WriteFldAlign(MJC_SA_01, 1, MJC_FW_RP_ENABLE);

#if MJC_2ND_MBIST_CHK
START_BIST_PROC:
#endif
    // start bist proc
    vIO32WriteFldAlign(MJC_SA_01, 0, MJC_FW_MJCRP_RST_B);
    vIO32WriteFldAlign(MJC_SA_01, 1, MJC_FW_MJCRP_BISTEN);
    vIO32WriteFldAlign(MJC_SA_01, 1, MJC_FW_MJCRP_RST_B);

    // wait mbist done
    while (IO32ReadFldAlign(MJC_STA_SA_03, MJC_STA_ALL_RP_MBIST_DONE) != SV_TRUE)
    {
        LOG(0, "MBIST is not ready\n");
    }

    // check mbist fail
    if (IO32ReadFldAlign(MJC_STA_SA_03, MJC_STA_ALL_RP_MBIST_FAIL))
    {
        LOG(0, "MJC mbist is FAILED!\n");
        return;
    }

    #if MJC_2ND_MBIST_CHK
    // check mbist repair bits
    if (IO32ReadFldAlign(MJC_STA_SA_00, MJC_STA_RP_OK0) ||
        IO32ReadFldAlign(MJC_STA_SA_01, MJC_STA_RP_OK1) ||
        IO32ReadFldAlign(MJC_STA_SA_02, MJC_STA_RP_OK2))
    {
        if (fg2ndTest == SV_FALSE)
        {
            vIO32WriteFldAlign(MJC_SA_01, 0, MJC_FW_MJCRP_BISTEN);
            LOG(0, "MJC mbist repair OK.\n");
            return;
        }
        fg2ndTest = SV_FALSE;
        goto START_BIST_PROC; // avoid repair memory fail
    }
    #endif

    vIO32WriteFldAlign(MJC_SA_01, 0, MJC_FW_MJCRP_BISTEN);

    vIO32WriteFld(CKGEN_MBIST_CTRL, SV_OFF, FLD_MBIST_RSTB);
    LOG(0, "MJC mbist is unnecessary.\n");
    return;
    */
}

void vDrvMJCSetFBOneFrmOnOff(BOOL fgOnOff)
{
    //UINT8 u1IntMsk = 0;

    arMJCPrm.u1OneFrm = fgOnOff;

    //u1IntMsk = IO32ReadFldAlign(MJC_SYS_INTR_CTRL, MJC_INT_MASK);

    if (fgOnOff)
    {
    //1.  Rescan OFF
    //2.  Set to display previous frame
    //3.  Set BW reduction type to NOT dropping previous fields
    //4.  Set one_frame register

    // turn off rescan, bandwidth reduction, letter box, film mode
    // MC intp mode = repeat
        vDrvMJCSetPSAutoOnOff(SV_OFF);
        vIO32WriteFldAlign(MJC_SA_00, 0, MJC_RESCAN_EN);

        //if (u1IntMsk & MJC_INT_3)
        {
            vIO32WriteFldMulti(MJC_MC_00,
                P_Fld(0, MJC_MC_INTP_MODE_RGN2) |
                P_Fld(0, MJC_MC_INTP_MODE_RGN1) |
                P_Fld(0, MJC_MC_INTP_MODE_RGN0));
        }

        vIO32WriteFldMulti(MJC_SA_00,
            P_Fld(0, MJC_ME_LBOX_HALO_REDUCE_EN) |
            P_Fld(0, MJC_MC_LBOX_HALO_REDUCE_EN));

        //vIO32WriteFldAlign(MJC_FB_0A, 0, MJC_BANDWIDTH_REDUCTION_EN);
        vIO32WriteFldMulti(MJC_FM_08,
            P_Fld(0, MJC_PROG_RDCTYPE_0_4)|P_Fld(0, MJC_PROG_RDCTYPE_1_4)|
            P_Fld(0, MJC_PROG_RDCTYPE_2_4)|P_Fld(0, MJC_PROG_RDCTYPE_3_4));

        //x_thread_delay(100);

        vIO32WriteFldAlign(MJC_FB_01, 1, MJC_FB_ONE_FRM_EN);
    }
    else
    {
        vIO32WriteFldAlign(MJC_FB_01, 0, MJC_FB_ONE_FRM_EN);

        //x_thread_delay(100);

        vIO32WriteFldMulti(MJC_FM_08,
            P_Fld(0, MJC_PROG_RDCTYPE_0_4)|P_Fld(0, MJC_PROG_RDCTYPE_1_4)|
            P_Fld(2, MJC_PROG_RDCTYPE_2_4)|P_Fld(2, MJC_PROG_RDCTYPE_3_4));

        vIO32WriteFldMulti(MJC_SA_00,
            P_Fld(1, MJC_ME_LBOX_HALO_REDUCE_EN) |
            P_Fld(1, MJC_MC_LBOX_HALO_REDUCE_EN));

		
        vIO32WriteFldMulti(MJC_MC_00,
                P_Fld(6, MJC_MC_INTP_MODE_RGN2) |
                P_Fld(6, MJC_MC_INTP_MODE_RGN1) |
                P_Fld(6, MJC_MC_INTP_MODE_RGN0));
		

        vIO32WriteFldAlign(MJC_SA_00, arMJCPrm.u1Rscn, MJC_RESCAN_EN);
        //vIO32WriteFldAlign(MJC_FB_0A, arMJCPrm.u1Rdc, MJC_BANDWIDTH_REDUCTION_EN);

        vDrvMJCSetPsAutoTrigOn();
    }

    vDrvMJCUpdateIntpCtrl();
}

void vDrvMJCInit(UINT8 bForceInit)
{
    #if !(MJC_FPGA)
    if (_fgIsInitialized && (bForceInit!=TRUE))
    {
        LOG(0, "MJC had been initialized.\n");
        return;
    }

    LOG(0, "MJC Initialize.\n");

    vDrvMJCSetClock();

    vIO32WriteFldAlign(MJC_SYS_MUTE_0, 0x2, MJC_MUTE_1TYPE);
    vIO32WriteFldAlign(MJC_SYS_MUTE_1, 0xFFF, MJC_MUTE_1DN);
    vIO32WriteFldAlign(MJC_SYS_MUTE_2, 0xFFF, MJC_MUTE_1RT);
    vIO32WriteFldAlign(MJC_SYS_MUTE_1, SV_ON, MJC_MUTE_SET);

    vDrvLoadRegTbl(MJC_SYS_INIT);
    vDrvLoadRegTbl(MJC_FB_INIT);
    vDrvLoadRegTbl(MJC_FM_INIT);
    vDrvLoadRegTbl(MJC_ME_INIT);
    vDrvLoadRegTbl(MJC_MV_INIT);
    vDrvLoadRegTbl(MJC_CA_INIT);
    vDrvLoadRegTbl(MJC_MC_INIT);
    vDrvLoadRegTbl(MJC_TBA_INIT);
    vDrvLoadRegTbl(MJC_PSG_INIT);

    vIO32WriteFldMulti(MJC_SYS_04,
        P_Fld(wDISPLAY_HEIGHT, MJC_OUT_HEIGHT) |
        P_Fld(wDISPLAY_WIDTH, MJC_OUT_WIDTH));

    vIO32WriteFldMulti(MJC_SYS_05,
        P_Fld(lcdtimingInfo.u4VTotal, MJC_OUT_V_TOTAL) |
        P_Fld(((lcdtimingInfo.u4HTotal) & 0xFFF), MJC_OUT_H_TOTAL));
    vIO32WriteFldAlign(MJC_SYS_H_MSB,
        ((lcdtimingInfo.u4HTotal & 0x1000) >> 12), MJC_OUT_H_TOTAL_MSB);



    //TBD :check hw default is 0
    //IO32WriteFldAlign(MJC_PPC_02, 0, MJC_PPC_RGNMV_SEL); 

    if ((!PANEL_IsSupport120Hz()) &&(!PANEL_IsSupport240Hz()))
    {
        vIO32Write4B(MJC_ME_03, 0x9848c858);    //ME penalty
    }

    vIO32WriteFldMulti(MJC_SYS_DISP,
        P_Fld(SV_OFF, MJC_DISP_SYNC_SELF) |
        P_Fld(SV_ON, MJC_DISP_MODE) |
        P_Fld(0x1F, MJC_DISP_VPOS));
    vIO32WriteFldAlign(MJC_SYS_INTR_CTRL, 0x2, MJC_INT_POL_SEL);

    vIO32Write4B(MJC_MISC_00, 0x10142820);
    vIO32Write4B(MJC_PD_INFO_01, 0x00010000);
    
    //mjc dither init
    vIO32Write4B(MJC_FB_0B, 0x44880000);
    vIO32Write4B(MJC_SYS_D9, 0x18055000);
    vIO32Write4B(MJC_SYS_DA, 0xA0000000);

    x_memset((void *)&arMJCPrm, 0, sizeof(MJC_PRM_T));
    x_memset((void *)&arMJCSPReg, 0, 8 * sizeof(MJC_SP_REG_T));

    arMJCPrm.u1OnOff      = E_MJC_OFF;
    arMJCPrm.u1CtrlParam  = MJC_CTRL_LT_ADAPT | MJC_CTRL_LT_ON;
    arMJCPrm.u1CtrlParam |= MJC_CTRL_RT_ADAPT | MJC_CTRL_RT_ON;
    arMJCPrm.u1CtrlParam |= MJC_CTRL_FM_ON;
    arMJCPrm.eMjcEffect   = E_MJC_EFFECT_HIGH;
    //vIO32WriteFldAlign(MJC_SYS_09, SV_ON, MJC_IN_LR_INV);

    vIO32WriteFldAlign(MJC_FW_SCH_08, 20,  MJC_UNICADENCE_TRIGER_CNT);
    vIO32WriteFldAlign(MJC_MISC_00, SV_TRUE, MJC_IS_44PULLDOWN);
    vDrvMJCSetUniCandeceEn(SV_FALSE);

    #if (defined(SUPPORT_DISP_WEB_VIDEO_SMOOTHER))
    if (IS_SUPPORT_DISP_6X3X_WEB == SV_TRUE)
    {
        vDrvMJCSetUniCandeceEn(SV_TRUE);
    }
    #endif

    vDrvMFDInit();
    vDrvMJCInitQuality();
    vDrvMJCFrmSchInit();
    vDrvMJCInitISR();
    vDrvMJCOSMPInit(MJC_OSMP_PR_MODE);

    // vDrvMJCRunMbist();      // Cobra mbist has been removed
    if (u4DrvMJCBufferInit() == SV_FAIL)
    {
        _fgIsInitialized = TRUE;
        MJC_SetOnOff(E_MJC_BYPASS);
        return;
    }

    arMJCDramAlloc.u2Mode = FRC_Y08_C08_444;
    vDrvMJCSetSystemMode(arMJCDramAlloc.u2Mode);
    _fgIsInitialized = TRUE;

    {
        UINT32 fgMuteVsyncLatch = fgDrvVideoMuteIsVsyncLatch();
        _vDrvVideoSetMute(MUTE_MODULE_MJC, SV_VP_MAIN, MJC_INIT_MUTE_COUNT, SV_TRUE);
        if (fgMuteVsyncLatch == SV_ON)
        {
            vDrvVideoMuteVsyncLatch(SV_OFF);
            vDrvVideoMuteVsyncLatch(SV_ON);
        }
    }

    vDrvVideoMuteVsyncLatch(SV_ON);
    MJC_SetOnOff(E_MJC_ON);

    vDrvMJCDIROnOff(SV_ON);
    vDrvMJCSet32PdInfoShiftProc(0, 1, 0);
    u1DrvMJCSet32PdInfoShiftJudgmentNew(0, 1, 0);
    #else
    //vDrvMJCFrmSchInit();
    vDrvMJCInitISR();
    #endif
}

void vDrvMJCResetSW(void)
{
}

void vDrvMJCInitISR(void)
{
    static BOOL u1MJCISRInited = SV_FALSE;

    x_os_isr_fct    pfnOldIsr;

    if (!u1MJCISRInited)
    {
        VERIFY(x_sema_create(&_hMJCOutputPollingSema, X_SEMA_TYPE_BINARY, X_SEMA_STATE_UNLOCK) == OSR_OK);
            
        if (x_reg_isr(VECTOR_MJC, vDrvMJCISR, &pfnOldIsr) != OSR_OK)
        {
            LOG(0, "Error: Fail to register MJC ISR!\n");
            return;
        }

        u1MJCISRInited = SV_TRUE;
    }

    LOG(0, "MJC ISR registered!\n");
}

void vDrvMJCOSMPInit(UINT8 u1MJCOSMPPRMode)
{
    if (u1MJCOSMPPRMode & MJC_OSMP_IS_HPR)
    {
        if(IS_PANEL_L12R12)
        {
            arMJCPrm.arMJCPRConfig.u1MJCOSMPHPRMode = E_MJC_HPR2160;         //set half pr 2160 for pdp panel
        }
        else
        {
            arMJCPrm.arMJCPRConfig.u1MJCOSMPHPRMode = E_MJC_HPR_NM;
        }
    }
    else if (u1MJCOSMPPRMode & 0xF)   // full pr setting
    {
        arMJCPrm.arMJCPRConfig.u1MJCOSMPFPRMode = (u1MJCOSMPPRMode & 0x8) ? E_MJC_FPRV3 :
                                                  (u1MJCOSMPPRMode & 0x4) ? E_MJC_FPRV2 :
                                                  (u1MJCOSMPPRMode & 0x2) ? E_MJC_FPRV1 :
                                                  (u1MJCOSMPPRMode & 0x1) ? E_MJC_FPRV0 : E_MJC_FPRV1;
    }
}

void vDrvMJCSetClock(void)
{
    #ifndef CC_FPGA   // FILM DO IN PSCAN ,SVP OFF/ O CLK DO IN TTD
    vIO32WriteFldMulti(MJC_SYS_CLK_CTRL,
        P_Fld(SV_OFF, MJC_CLK_I_EN) |
        P_Fld(SV_ON, MJC_CLK_M_EN));
    vIO32WriteFldAlign(CKGEN_MJC_CKCFG, 1, FLD_MJC_SEL);
    #endif
}

void vDrvMJCSetCoreClock(BOOL fg216MHz)
{
    /*
    if (fg216MHz)
    {
        vIO32WriteFldMulti(CKGEN_PLLGP_00, P_Fld(0x7, FLD_RG_MJCPLL_FBDIV)|P_Fld(1, FLD_RG_MJCPLL_PWD));
    }
    else
    {
        vIO32WriteFldMulti(CKGEN_PLLGP_00, P_Fld(0xB, FLD_RG_MJCPLL_FBDIV)|P_Fld(1, FLD_RG_MJCPLL_PWD));
    }

    vIO32WriteFld(CKGEN_PLLGP_00, 0, FLD_RG_MJCPLL_PWD);
    vIO32WriteFldAlign(MJC_MISC_00, 1, MJC_FLMR_CHG_TRIG);
    vDrvMJCModeChgDone();
    */
}

void vDrvMJCSetSACtrlMode(UINT8 u1Mode)
{
    switch (u1Mode)
    {
    case 0:
        vIO32WriteFldMulti(MJC_SA_00,
            P_Fld(0, MJC_RW_TRANS_PATCH)|
            P_Fld(0, MJC_LADDER_PATCH));
        break;
    case 1:
        vIO32WriteFldMulti(MJC_SA_00,
            P_Fld(0, MJC_RW_TRANS_PATCH)|
            P_Fld(1, MJC_LADDER_PATCH));
        break;
    case 2:
        vIO32WriteFldMulti(MJC_SA_00,
            P_Fld(1, MJC_RW_TRANS_PATCH)|
            P_Fld(0, MJC_LADDER_PATCH));
        break;
    case 0xff:
    default:
        vIO32WriteFldMulti(MJC_SA_00,
            P_Fld(1, MJC_RW_TRANS_PATCH)|
            P_Fld(1, MJC_LADDER_PATCH));
        break;
    }
}

void vDrvMJCSetDRAMAccessOnOff(BOOL fgOnOff, BOOL fgDRAM444)
{
    if (fgOnOff == SV_OFF)
    {
        vIO32WriteFldAlign(MJC_SYS_00, 0, MJC_DRAM_RW_EN);
        vIO32WriteFldAlign(MJC_MV_TBA_5, 0, MJC_MV_TBA_WT_EN);
    }
    else
    {
        vIO32WriteFldAlign(MJC_SYS_00, (fgDRAM444 ? 0x3 : 0x1F), MJC_DRAM_RW_EN);
        {
            vIO32WriteFldAlign(MJC_MV_TBA_5, (fgDRAM444 == SV_FALSE), MJC_MV_TBA_WT_EN);
        }
    }
}

void vDrvMJCSetBuf(MJC_DRAM_ALLOC_T * prDramAlloc)
{
    UINT32 fgDram444, fgDramRpt422, fgY8, fgC8, fgDyn;
    UINT16 u2Mode;
    UINT32 u4BaseAddr, u4FrameDramSize, u4MVDramSize, u4MVDramTotalSize;

    u2Mode       = prDramAlloc->u2InitMode;
    fgDram444    = (u2Mode & MJC_444_MODE) ? 1 : 0;
    fgDramRpt422 = (u2Mode & MJC_422RPT_MODE) ? 1 :0;
    fgY8         = (u2Mode & MJC_Y08_MODE) ? 1 : 0;
    fgC8         = (fgDram444) ? fgY8 : ((u2Mode & MJC_C08_MODE) ? 1 : 0);
    fgDyn        = (fgY8 && fgC8)? 0 : 1;

    u4BaseAddr        = prDramAlloc->u4StartAddr >> 4;
    u4FrameDramSize   = u4DrvMJCCalcSizeYC(prDramAlloc->u4Width, prDramAlloc->u4Height, fgDram444, fgDramRpt422, fgY8, fgC8 ,fgDyn);
    u4MVDramSize      = u4DrvMJCCalcSizeMV(prDramAlloc->u4Width, prDramAlloc->u4Height+8, fgDram444);
    u4MVDramTotalSize = u4MVDramSize * 6 + 6;

    vIO32WriteFldAlign(MJC_FB_0E, (u4BaseAddr >> 24), MJC_DRAM_BASE_ADDR_MSB);
    vIO32WriteFldAlign(MJC_FB_00, u4BaseAddr, MJC_WADDR_LO_LIMIT);
    vIO32WriteFldAlign(MJC_FB_08, u4BaseAddr, MJC_DRAM_BASE_ADDR);
    u4BaseAddr += (u4FrameDramSize >> 4);

    vIO32WriteFldAlign(MJC_MV_DRAM0, u4BaseAddr, MJC_MV_DRAM_STR_ADR_L);
    u4BaseAddr += (u4MVDramTotalSize >> 4);
    vIO32WriteFldAlign(MJC_MV_DRAM1, u4BaseAddr, MJC_MV_DRAM_STR_ADR_R);
    u4BaseAddr += (u4MVDramTotalSize >> 4)+1;
    
    vIO32WriteFldMulti(MJC_MV_DRAM2, 
        P_Fld(u4MVDramSize >> 4, MJC_MV_DRAM_SIZE) |
        P_Fld(((u4MVDramSize >> 4)+1), MJC_MV_DRAM_LIMIT));

    vIO32WriteFldAlign(MJC_FB_01, u4BaseAddr, MJC_WADDR_HI_LIMIT);

    if ((u4BaseAddr << 4) > prDramAlloc->u4EndAddr)
    {
        LOG(0, "vDrvMJCSetBuf: SRM MJC DRAM allocation error\n");
    }
}

void vDrvMJCSetSystemModeProc(void)
{
    vDrvMJCSetFrmFreeze(SV_OFF);
    vDrvMJCSetDRAMAccessOnOff(SV_OFF, 0);
    vDrvMJCSetSystemMode(arMJCDramAlloc.u2Mode);
    vDrvMJCSetDRAMAccessOnOff(SV_ON, arMJCPrm.u1Dram444);
    fgMJCModeChg = SV_TRUE;
}

void vDrvMJCSetSystemMode(UINT32 u4MjcSystemMode)
{
    UINT8 u1Dram444, u1Dram420, u1Dram422RPT, u1Y8, u1C8, u1TwoFrm, u1OneFrm, u1Rdc, u1Rscn, u1Dyn, u1Rpt;

    u1Dram422RPT = (u4MjcSystemMode & MJC_422RPT_MODE) ? 1 : 0;
    u1Dram444 = (u4MjcSystemMode & MJC_444_MODE) ? 1 : 0;
    u1Dram420 = (u4MjcSystemMode & MJC_420_MODE) ? 1 : 0;
    u1Dyn     = (u4MjcSystemMode & MJC_DYN_MODE) ? 1 : 0;
    u1Y8      = (u4MjcSystemMode & MJC_Y08_MODE) ? 1 : 0;
    u1C8      = (u4MjcSystemMode & MJC_C08_MODE) ? 1 : 0;
    u1TwoFrm  = (u4MjcSystemMode & MJC_2FRM_MODE) ? 1 : 0;
    u1OneFrm  = (u4MjcSystemMode & MJC_1FRM_MODE) ? 1 : 0;
    u1Rdc     = (u4MjcSystemMode & MJC_1RDC_MODE) ? (!u1OneFrm) : 0;
    u1Rscn    = 1; //(u4MjcSystemMode & MJC_RSCN_MODE) ? 1 : 0;
    u1Rpt     = (u4MjcSystemMode & MJC_RPT_MODE) ? (!u1Dram420) : 0;

    if (arMJCPrm.u1Dram444 != u1Dram444)
    {
        vDrvMJCSetPSAutoOnOff(SV_OFF);
        if ((u1Dram444 == SV_FALSE) && (u1DrvMJCIsOnOff() == SV_TRUE))
        {
            vDrvMJCSetPsAutoTrigOn();
        }
    }

    //Keep information
    arMJCPrm.u1Dram444 = u1Dram444;
    arMJCPrm.u1Dram422RPT = u1Dram422RPT;
    arMJCPrm.u1Y8 = u1Y8;
    arMJCPrm.u1YDithEn = u1Y8;
    arMJCPrm.u1TwoFrm = u1TwoFrm;
    arMJCPrm.u1OneFrm = u1OneFrm;
    arMJCPrm.u1Rdc = u1Rdc;
    arMJCPrm.u1Dyn = u1Dyn;
    arMJCPrm.u1Rscn = u1Rscn;

    // ddp setting
    vIO32WriteFldAlign(MJC_FB_LR3D, ((u1Dram444)? SV_OFF : arMJCPrm.arPicSize.fgDdp), MJC_3D_DDP_EN);
    // dyn
    u1Dyn = (u1Y8 && u1C8)? 0 : 1;
    vIO32WriteFldAlign(MJC_FB_0C, u1Dyn, MJC_DRAM_DYNAMIC_SWITCH);
    // rescan
    vIO32WriteFldAlign(MJC_SA_00, u1Rscn, MJC_RESCAN_EN);

    if (u1Dram444)
    {
        if ((arMJCPrm.arVrmSrmConfig.u1MJC3DModeIn == E_VDO_MJC_2D) && (arMJCPrm.u1Dram422RPT == SV_OFF))
        {
            // 444->422 Off
            vIO32WriteFldMulti(MJC_SYS_01,
                P_Fld(0, MJC_IN_LINE_EXT)|
                P_Fld(0, MJC_IN_LPF_EN)|
                P_Fld(0, MJC_IN_YUV2YC_EN));

            // 422->444 Off
            // DRAM-444 Mirror Chroma delay 0T
            vIO32WriteFldMulti(MJC_SYS_03,
                P_Fld(0, MJC_OUT_CHROMADLY_EN) |
                P_Fld(0, MJC_OUT_YC2YUV_EN));
            vIO32WriteFldMulti(MJC_FB_01,
                P_Fld(1, MJC_DRAM_RPT)|P_Fld(0, MJC_DRAM_RPT_422)|P_Fld(0, MJC_FB_TWO_FRM_EN));

            // Processing Color Space is RGB444
            // Disable MV dram access for repeat mode
            vIO32WriteFldMulti(MJC_SYS_00,
                P_Fld(1, MJC_BY_IN_TRANSFORM)|
                P_Fld(1, MJC_BY_OUT_TRANSFORM));  // DRAM access should be enabled after address change.
        }
        else
        {
            // 444->422 On
            vIO32WriteFldMulti(MJC_SYS_01,
                P_Fld(1, MJC_IN_LINE_EXT)|
                P_Fld(1, MJC_IN_LPF_EN)|
                P_Fld(1, MJC_IN_YUV2YC_EN));

            // 422->444 On
            // DRAM-42x Mirror Chroma delay 1T (if Mirror On)
            vIO32WriteFldMulti(MJC_SYS_03,
                P_Fld(arMJCPrm.u1Mirror, MJC_OUT_CHROMADLY_EN) |
                P_Fld(1, MJC_OUT_YC2YUV_EN));
            vIO32WriteFldMulti(MJC_FB_01,
                P_Fld(1, MJC_DRAM_RPT)|P_Fld(1, MJC_DRAM_RPT_422)|P_Fld(0, MJC_FB_TWO_FRM_EN));

            // Processing Color Space is RGB444
            // Disable MV dram access for repeat mode
            vIO32WriteFldMulti(MJC_SYS_00,
                P_Fld(0, MJC_BY_IN_TRANSFORM)|
                P_Fld(0, MJC_BY_OUT_TRANSFORM));  // DRAM access should be enabled after address change.
        }
        // Y8/Y_Dither
        vIO32WriteFldMulti(MJC_FB_00,
            P_Fld(u1Y8, MJC_DRAM_Y_8BIT)|
            P_Fld(u1Y8, MJC_DRAM_C_8BIT)|   // DRAM-444 C8: determined by u1Y8
            P_Fld(0, MJC_DRAM_420));        // DRAM 444 frame repeat
        vIO32WriteFldMulti(MJC_SYS_D9,    //
            P_Fld(((u1Y8) ? 2 :0) , MJC_DITH_DRMOD_Y)|     // Y8/Y_Dither
            P_Fld(((u1Y8) ? 2 :0), MJC_DITH_DRMOD_U));   // UV_Dither : determined by u1Y8

        // DRAM 444 frame repeat
        vIO32WriteFldMulti(MJC_FB_0A,
            P_Fld(0, MJC_BANDWIDTH_REDUCTION_EN));

        //Keep information
        arMJCPrm.u1Dram420 = 0;
        arMJCPrm.u1C8 = u1Y8;
        arMJCPrm.u1CDithEn = u1Y8;
        arMJCPrm.u1Rpt = 1;
    }
    else
    {
        // 444->422 On
        vIO32WriteFldMulti(MJC_SYS_01,
            P_Fld(1, MJC_IN_LINE_EXT)|
            P_Fld(1, MJC_IN_LPF_EN)|
            P_Fld(1, MJC_IN_YUV2YC_EN));

        // 422->444 On
        // DRAM-42x Mirror Chroma delay 1T (if Mirror On)
        vIO32WriteFldMulti(MJC_SYS_03,
            P_Fld(arMJCPrm.u1Mirror, MJC_OUT_CHROMADLY_EN) |
            P_Fld(1, MJC_OUT_YC2YUV_EN));

        // Y8/Y_Dither
        vIO32WriteFldMulti(MJC_FB_00,
            P_Fld(u1Y8, MJC_DRAM_Y_8BIT)|
            P_Fld(u1C8, MJC_DRAM_C_8BIT)|       // DRAM-42x C8: determined by u1C8
            P_Fld(u1Dram420, MJC_DRAM_420));    // DRAM 42x MEMC

        vIO32WriteFldMulti(MJC_SYS_D9,
            P_Fld(((u1Y8) ? 2 :0), MJC_DITH_DRMOD_Y)|     // Y8/Y_Dither
            P_Fld(((u1C8) ? 2 :0), MJC_DITH_DRMOD_U));   // UV_Dither : determined by u1C8

        // DRAM 42x MEMC
        vIO32WriteFldMulti(MJC_FB_0A,
            P_Fld(u1Rdc, MJC_BANDWIDTH_REDUCTION_EN));
        vIO32WriteFldMulti(MJC_FB_01,
            P_Fld(u1Rpt, MJC_DRAM_RPT)|P_Fld(u1Rpt, MJC_DRAM_RPT_422)|P_Fld(u1TwoFrm, MJC_FB_TWO_FRM_EN));

        // Processing Color Space is YUV422
        // Enable MV dram access for repeat mode
        vIO32WriteFldMulti(MJC_SYS_00,
            P_Fld(0, MJC_BY_IN_TRANSFORM)|
            P_Fld(0, MJC_BY_OUT_TRANSFORM)); // DRAM access should be enabled after address change.

        vDrvMJCSetFBOneFrmOnOff(u1OneFrm);

        //Keep information
        arMJCPrm.u1Dram420 = u1Dram420;
        arMJCPrm.u1C8 = u1C8;
        arMJCPrm.u1CDithEn = u1C8;
        arMJCPrm.u1Rpt = u1Rpt;
    }
}

void vDrvMJCSetFlipMirror(UINT32 u4FlipOnOff, UINT32 u4MirrorOnOff)
{
    arMJCPrm.u1Flip = u4FlipOnOff;
    arMJCPrm.u1Mirror = u4MirrorOnOff;

    // Flip/Mirror
    vIO32WriteFldAlign(MJC_FB_0A, (u4FlipOnOff << 1) | (u4MirrorOnOff), MJC_FLIPMRR);

    // If Mirror On & DRAM-42x : Chroma delay 1T (if Mirror On)
    if (arMJCPrm.u1Dram444)
    {
        vIO32WriteFldAlign(MJC_SYS_03, 0, MJC_OUT_CHROMADLY_EN);
    }
    else
    {
        vIO32WriteFldAlign(MJC_SYS_03, u4MirrorOnOff, MJC_OUT_CHROMADLY_EN);
    }

    if (IO32ReadFldAlign(MJC_SYS_0F, MJC_PTG_PAT_EN) == SV_ON)
    {
        vDrvMJCEnablePatGen(0 , SV_ON);
    }

    vIO32WriteFldAlign(MJC_FB_LBOX_0, u4FlipOnOff, MJC_LBOX_RD_FLIP);

    vDrvMJCSetMCFPR(arMJCPrm.arVrmSrmConfig.u1MJC3DModeOut == E_VD0_MJC_OUT_FPR, arMJCPrm.fgDispRSwp != arMJCPrm.fgInLRSwp, arMJCPrm.u1Flip, arMJCPrm.arMJCPRConfig.u1MJCOSMPFPRMode);

    fgMJCModeChg = SV_TRUE;
}

static void vDrvMJCSetProgFrmRate(UINT8 u1InFrmRate, UINT8 u1OutFrmRate)
{
    UINT8 bFrmMultiple = 1;
    UINT8 bDivBy2 = 0;

    if (u1InFrmRate == 0)
    {
        LOG(3, "vDrvMJCSetProgFrmRate Failed: Invalid Input Frame Rate.\n");
        return;
    }

    if (!((u1InFrmRate % 25 == 0) && (u1OutFrmRate % 30 == 0)))
    {
        bFrmMultiple = u1OutFrmRate / u1InFrmRate;

        if (u1OutFrmRate != (u1InFrmRate * bFrmMultiple))
        {
            bDivBy2 = 1;
            bFrmMultiple = (bFrmMultiple << 1) + 1;
        }
    }

    vIO32WriteFldMulti(MJC_SYS_04,
        P_Fld(bDivBy2, MJC_OUT_VSYNC_DIV) |
        P_Fld(bFrmMultiple, MJC_OUT_FRM_NO));
}

static void vDrvMJCSetPsAutoOnProc(void)
{
    UINT32 fgUIFmOn;

    if (_u1TrigSetPsAuto &&
        (IO32ReadFldAlign(MJC_STA_FM_02, MJC_STA_ME_FILM_FLG_HW) == 0))
    {
        _u1TrigSetPsAuto = 0;

        fgUIFmOn = (arMJCPrm.u1CtrlParam & MJC_CTRL_FM_ON) ? SV_ON : SV_OFF;
        vIO32WriteFldMulti(MJC_FM_00,
            P_Fld((arMJCPrm.u1PSAuto32OnOff && fgUIFmOn), MJC_PSAUTO_32)|
            P_Fld((arMJCPrm.u1PSAuto22OnOff && fgUIFmOn), MJC_PSAUTO_22));
    }
}

void vDrvMJCSetPSAutoOnOff(BOOL fgOnOff)
{
    if ((fgOnOff == SV_OFF) || (arMJCPrm.u1OneFrm))
    {
        vIO32WriteFldMulti(MJC_FM_00,
            P_Fld(SV_OFF, MJC_PSAUTO_32) |
            P_Fld(SV_OFF, MJC_PSAUTO_22));
    }
    else
    {
        _u1TrigSetPsAuto = 1;
    }
}

void vDrvMJCSetPdInfoShift(UINT32 u4PdInfo32Shift, UINT32 u4PdInfo22Shift)
{
    if (u4PdInfo32Shift != IO32ReadFldAlign(MJC_SYS_00, MJC_PD_INFO_32_SHIFT))
    {
        vDrvMJCSetPSAutoOnOff(SV_OFF);
        vIO32WriteFldMulti(MJC_SYS_00, 
            P_Fld(u4PdInfo32Shift, MJC_PD_INFO_32_SHIFT) |
            P_Fld(u4PdInfo22Shift, MJC_PD_INFO_22_SHIFT));
        vDrvMJCSetPSAutoOnOff(SV_ON);
    }
}

void vDrvMJCSet32PdInfoShiftJudgment(void)
{
    UINT8 fgIsInterlace        = 0;
    UINT8 fgMJCIs3DFSMode      = 0;
    UINT8 u1ScalerMode         = 0;
    UINT8 fgPdInfoShiftTrigger = SV_OFF;

    fgIsInterlace   = bDrvVideoIsSrcInterlace(SV_VP_MAIN);
    u1ScalerMode    = getScalerMode(SV_VP_MAIN);
    fgMJCIs3DFSMode = (arMJCPrm.arVrmSrmConfig.u1MJC3DModeIn == E_VDO_MJC_FS);

    if (fgIsInterlace != arMJCPrm.fgIsInterlace)
    {
        fgPdInfoShiftTrigger = SV_ON;
    }

    if (u1ScalerMode != arMJCPrm.u1ScalerMode)
    {
        fgPdInfoShiftTrigger = SV_ON;
    }
    if (fgMJCIs3DFSMode != arMJCPrm.fgMJCFSIn)
    {
        fgPdInfoShiftTrigger = SV_ON;
    }
    if (fgPdInfoShiftTrigger == SV_ON)
    {
        LOG(5, "vDrvMJCSet32PdInfoShift: fgIsInterlace = %d, u1ScalerMode = %d\n", fgIsInterlace, u1ScalerMode);
        vDrvMJCSet32PdInfoShiftProc(fgIsInterlace, u1ScalerMode, fgMJCIs3DFSMode);
        fgPdInfoShiftTrigger = SV_OFF;
    }
}

void vDrvMJCSet32PdInfoShiftProc(UINT8 fgIsInterlace, UINT8 u1ScalerMode, UINT8 fgMJCIs3DFSMode)
{
    UINT32 u4PdInfo32Shift = 0;
    UINT32 u4PdInfo22Shift = 0;

    arMJCPrm.fgIsInterlace = fgIsInterlace;
    arMJCPrm.u1ScalerMode  = u1ScalerMode;
    arMJCPrm.fgMJCFSIn     = fgMJCIs3DFSMode;

    LOG(1, "vDrvMJCSet32PdInfoShift: %d, %d, %d\n", fgIsInterlace, u1ScalerMode , fgMJCIs3DFSMode,u1PdInfoDly);
    //vIO32WriteFldAlign(MJC_SYS_00, SV_OFF, MJC_PD_INFO_DLY);
    vIO32WriteFldAlign(MJC_SYS_01, SV_OFF, MJC_IN_TEST_V_DLY);
    if (fgIsInterlace == SV_TRUE)
    {
        if (fgMJCIs3DFSMode == E_VDO_MJC_FS)
        {
            LOG(2, "vDrvMJCSet32PdInfoShiftProc:fgIsInterlace == SV_TRUE(pscan not support 3d fs film dectect;so no sw do it)\n");
            u4PdInfo32Shift = 4;  // pscan not support 3d fs film dectect;so no will use prog pd_ en and sw to do it
            vDrvMJCFrmSchSetPdInfoDly(4);
        }
        else
        {
            if (u1ScalerMode == VDP_SCPOS_PSCAN_DISPMODE)
            {
                u4PdInfo32Shift = 0;
                vDrvMJCFrmSchSetPdInfoDly(0);
            }
            else if (u1ScalerMode == VDP_SCPOS_DISPMODE_OFF)
            {
                u4PdInfo32Shift = 4;
                vDrvMJCFrmSchSetPdInfoDly(4);
            }
        }
        vDrvMJCSetPdInfoShift(u4PdInfo32Shift, u4PdInfo22Shift);
        vDrvMJCSetFrmSchPdInfoSwDly(0, 0);
        vIO32WriteFldAlign(MJCFD_00, SV_ON , MJCFD_C_FD_INFO_SEL);
        vIO32WriteFldAlign(MJCFD_00, SV_OFF , MJCFD_C_FD_SRC_SEL);
    }
    else
    {
        if (fgMJCIs3DFSMode == E_VDO_MJC_FS)
        {
            vIO32WriteFldAlign(MJC_SYS_00, SV_OFF, MJC_PD_INFO_DLY);     //base of front scaler frame track :NO RULE IN THIS CASE(MJC_PD_INFO_DLY:sv_on+  shift:0  or MJC_PD_INFO_DLY:sv_off +  shift:4)
            u4PdInfo32Shift = 4;
            vDrvMJCSetPdInfoShift(u4PdInfo32Shift, u4PdInfo22Shift);
            vDrvMJCFrmSchSetPdInfoDly(4);
            vDrvMJCSetFrmSchPdInfoSwDly(0, 3);
        }
        else
        {
            if (u1ScalerMode == VDP_SCPOS_DISPMODE_OFF)
            {
                u4PdInfo32Shift = 1;
                vDrvMJCFrmSchSetPdInfoDly(0);
            }
            else if (u1ScalerMode == VDP_SCPOS_PSCAN_DISPMODE)
            {
                u4PdInfo32Shift =  2;
                vDrvMJCFrmSchSetPdInfoDly(1);
            }
            else
            {
                u4PdInfo32Shift = 1;
                vDrvMJCFrmSchSetPdInfoDly(1);
                LOG(2, "vDrvMJCSet32PdInfoShiftProc:no use u1ScalerMode(VDP_SCPOS_LINESYNC_DISPMODE)\n");// may be no use and need check when use this mode
            }

            vDrvMJCSetPdInfoShift(u4PdInfo32Shift, u4PdInfo22Shift);
            vDrvMJCSetFrmSchPdInfoSwDly(1, 2);
        }
        vIO32WriteFldAlign(MJCFD_00, SV_OFF , MJCFD_C_FD_INFO_SEL);
        vIO32WriteFldAlign(MJCFD_00, SV_OFF , MJCFD_C_FD_SRC_SEL);
    }
}

static void _vDrvMJCSetProgME_Full(BOOL fgIs55p2)
{
    if (fgIs55p2 == 0)
    {
        vIO32WriteFldMulti(MJC_FM_02, MJC_PROG_ME2_04_FULL|MJC_PROG_ME2_08_FULL|MJC_PROG_ME2_10_FULL|MJC_PROG_ME2_L_20_FULL);
        vIO32WriteFldMulti(MJC_FM_03, MJC_PROG_ME2_H_20_FULL|MJC_PROG_ME3_04_FULL|MJC_PROG_ME3_08_FULL|MJC_PROG_ME3_L_20_FULL);
        vIO32WriteFldMulti(MJC_FM_04, MJC_PROG_ME3_H_20_FULL|MJC_PROG_ME3_10_FULL);

        vIO32Write4B(MJC_FM_10,
            ((MJC_PROG_POS_10_FULL & 0xFF) << 24)|(MJC_PROG_POS_08_FULL << 8)|MJC_PROG_POS_04_FULL);
        vIO32Write4B(MJC_FM_11, (MJC_PROG_POS_L_20_FULL << 12)|(MJC_PROG_POS_10_FULL >> 8));
        vIO32Write4B(MJC_FM_12, MJC_PROG_POS_H_20_FULL);
    }
    else
    {
        vIO32WriteFldMulti(MJC_FM_02, MJC_PROG_ME2_04_FULL|MJC_PROG_ME2_08_FULL|MJC_PROG_ME2_10_FULL|MJC_PROG_ME2_L_20_FULL);
        vIO32WriteFldMulti(MJC_FM_03, MJC_PROG_ME2_H_20_FULL|MJC_PROG_ME3_04_FULL|MJC_PROG_ME3_08_FULL|MJC_PROG_ME3_55P2_L_20_FULL);
        vIO32WriteFldMulti(MJC_FM_04, MJC_PROG_ME3_H_20_FULL|MJC_PROG_ME3_10_FULL);

        vIO32Write4B(MJC_FM_10,
            ((MJC_PROG_POS_10_FULL & 0xFF) << 24)|(MJC_PROG_POS_08_FULL << 8)|MJC_PROG_POS_04_FULL);
        vIO32Write4B(MJC_FM_11, (MJC_PROG_POS_55P2_L_20_FULL << 12)|(MJC_PROG_POS_10_FULL >> 8));
        vIO32Write4B(MJC_FM_12, MJC_PROG_POS_H_20_FULL);
    }
}

static void _vDrvMJCSetProgME_Half(BOOL fgIs55p2)
{
    if (fgIs55p2 == 0)
    {
        vIO32WriteFldMulti(MJC_FM_02, MJC_PROG_ME2_04_HALF|MJC_PROG_ME2_08_HALF|MJC_PROG_ME2_10_HALF|MJC_PROG_ME2_L_20_HALF);
        vIO32WriteFldMulti(MJC_FM_03, MJC_PROG_ME2_H_20_HALF|MJC_PROG_ME3_04_HALF|MJC_PROG_ME3_08_HALF|MJC_PROG_ME3_L_20_HALF);
        vIO32WriteFldMulti(MJC_FM_04, MJC_PROG_ME3_H_20_HALF|MJC_PROG_ME3_10_HALF);

        vIO32Write4B(MJC_FM_10,
            ((MJC_PROG_POS_10_HALF & 0xFF) << 24)|(MJC_PROG_POS_08_HALF << 8)|MJC_PROG_POS_04_HALF);
        vIO32Write4B(MJC_FM_11, (MJC_PROG_POS_L_20_HALF << 12)|(MJC_PROG_POS_10_HALF >> 8));
        vIO32Write4B(MJC_FM_12, MJC_PROG_POS_H_20_HALF);
    }
    else
    {
        vIO32WriteFldMulti(MJC_FM_02, MJC_PROG_ME2_04_HALF|MJC_PROG_ME2_08_HALF|MJC_PROG_ME2_10_HALF|MJC_PROG_ME2_55P2_L_20_HALF);
        vIO32WriteFldMulti(MJC_FM_03, MJC_PROG_ME2_H_20_HALF|MJC_PROG_ME3_04_HALF|MJC_PROG_ME3_08_HALF|MJC_PROG_ME3_55P2_L_20_HALF);
        vIO32WriteFldMulti(MJC_FM_04, MJC_PROG_ME3_H_20_HALF|MJC_PROG_ME3_10_HALF);

        vIO32Write4B(MJC_FM_10,
            ((MJC_PROG_POS_10_HALF & 0xFF) << 24)|(MJC_PROG_POS_08_HALF << 8)|MJC_PROG_POS_04_HALF);
        vIO32Write4B(MJC_FM_11, (MJC_PROG_POS_55P2_L_20_HALF << 12)|(MJC_PROG_POS_10_HALF >> 8));
        vIO32Write4B(MJC_FM_12, MJC_PROG_POS_H_20_HALF);
    }
}

static void _vDrvMJCSetProgME_SingleME(BOOL fgIs55p2)
{
    if (fgIs55p2 == 0)
    {
        vIO32WriteFldMulti(MJC_FM_02, MJC_PROG_ME2_04_SGME|MJC_PROG_ME2_08_SGME|MJC_PROG_ME2_10_SGME|MJC_PROG_ME2_L_20_SGME);
        vIO32WriteFldMulti(MJC_FM_03, MJC_PROG_ME2_H_20_SGME|MJC_PROG_ME3_04_SGME|MJC_PROG_ME3_08_SGME|MJC_PROG_ME3_L_20_SGME);
        vIO32WriteFldMulti(MJC_FM_04, MJC_PROG_ME3_H_20_SGME|MJC_PROG_ME3_10_SGME);

        vIO32Write4B(MJC_FM_10,
            ((MJC_PROG_POS_10_SGME & 0xFF) << 24)|(MJC_PROG_POS_08_SGME << 8)|MJC_PROG_POS_04_SGME);
        vIO32Write4B(MJC_FM_11, (MJC_PROG_POS_L_20_SGME << 12)|(MJC_PROG_POS_10_SGME >> 8));
        vIO32Write4B(MJC_FM_12, MJC_PROG_POS_H_20_SGME);
    }
    else
    {
        vIO32WriteFldMulti(MJC_FM_02, MJC_PROG_ME2_04_SGME|MJC_PROG_ME2_08_SGME|MJC_PROG_ME2_10_SGME|MJC_PROG_ME2_55P2_L_20_SGME);
        vIO32WriteFldMulti(MJC_FM_03, MJC_PROG_ME2_H_20_SGME|MJC_PROG_ME3_04_SGME|MJC_PROG_ME3_08_SGME|MJC_PROG_ME3_55P2_L_20_SGME);
        vIO32WriteFldMulti(MJC_FM_04, MJC_PROG_ME3_H_20_SGME|MJC_PROG_ME3_10_SGME);

        vIO32Write4B(MJC_FM_10,
            ((MJC_PROG_POS_10_SGME & 0xFF) << 24)|(MJC_PROG_POS_08_SGME << 8)|MJC_PROG_POS_04_SGME);
        vIO32Write4B(MJC_FM_11, (MJC_PROG_POS_55P2_L_20_SGME << 12)|(MJC_PROG_POS_10_SGME >> 8));
        vIO32Write4B(MJC_FM_12, MJC_PROG_POS_H_20_SGME);
    }
}

static void _vDrvMJCSetProgME_240(BOOL fgIs55p2)
{
    vIO32WriteFldMulti(MJC_FM_02, MJC_PROG_ME2_04_240|MJC_PROG_ME2_08_240|MJC_PROG_ME2_10_240|MJC_PROG_ME2_L_20_240);
    vIO32WriteFldMulti(MJC_FM_03, MJC_PROG_ME2_H_20_240|MJC_PROG_ME3_04_240|MJC_PROG_ME3_08_240|MJC_PROG_ME3_L_20_240);
    vIO32WriteFldMulti(MJC_FM_04, MJC_PROG_ME3_H_20_240|MJC_PROG_ME3_10_240);

    if (fgIs55p2 == 0)
    {
        vIO32Write4B(MJC_FM_10,
            ((MJC_PROG_POS_10_240 & 0xFF) << 24)|(MJC_PROG_POS_08_240 << 8)|MJC_PROG_POS_04_240);
        vIO32Write4B(MJC_FM_11, (MJC_PROG_POS_L_20_240 << 12)|(MJC_PROG_POS_10_240 >> 8));
        vIO32Write4B(MJC_FM_12, MJC_PROG_POS_H_20_240);
    }
    else
    {
        vIO32Write4B(MJC_FM_10,
            ((MJC_PROG_POS_10_240 & 0xFF) << 24)|(MJC_PROG_POS_08_240 << 8)|MJC_PROG_POS_04_240);
        vIO32Write4B(MJC_FM_11, (MJC_PROG_POS_55P2_L_20_240 << 12)|(MJC_PROG_POS_10_240 >> 8));
        vIO32Write4B(MJC_FM_12, MJC_PROG_POS_55P2_H_20_240);
    }
}

void vDrvMJCSetProgME(UINT8 u1PanelType, UINT8 fgIs55p2)
{
    UINT8 u1ProgMEMode = IO32ReadFldAlign(MJC_MISC_00, MJC_PROG_ME_MODE);
    BOOL fgPPCIntlv = SV_TRUE;
    BOOL fgFWMvExpEn = SV_FALSE;

    switch (u1PanelType)
    {
    case E_MJC_60HZ:
        _vDrvMJCSetProgME_Full(0);
        break;
    case E_MJC_120HZ:
        switch (u1ProgMEMode)
        {
        default:
        case 0: // half run 324MHz (5395)
            _vDrvMJCSetProgME_Half(fgIs55p2);
            fgPPCIntlv = SV_FALSE;
            break;
        case 2: // full run 324MHz
            _vDrvMJCSetProgME_Full(fgIs55p2);
            fgPPCIntlv = SV_FALSE;
            break;
        case 1: // single ME mode
            _vDrvMJCSetProgME_SingleME(fgIs55p2);
            fgFWMvExpEn = SV_TRUE;
            break;
        }
        break;
    case E_MJC_240HZ:
        // 240Hz
        if (arMJCPrm.arVrmSrmConfig.u1MJC3DModeOut == E_VD0_MJC_OUT_HPR)
        {   // single ME mode for half pr 240HZ (120Hz MEMC)
            _vDrvMJCSetProgME_SingleME(fgIs55p2);
            fgFWMvExpEn = SV_TRUE;
        }
        else
        {   // 240Hz MEMC
            _vDrvMJCSetProgME_240(fgIs55p2);
        }
        break;
    }

    vIO32WriteFldAlign(MJC_MISC_01, fgFWMvExpEn, MJC_FW_MVEXP_EN);

    vIO32WriteFldMulti(MJC_FM_00,
        P_Fld((((arMJCPrm.arVrmSrmConfig.u1MJC3DModeOut == E_VD0_MJC_OUT_HPR) && (arMJCPrm.arFRCConfig.u1OutFrmRate == 240))? 1 : 0), MJC_ME_PRLST_PREBI) |
        P_Fld(fgPPCIntlv, MJC_PPC_FORCE_INTLV) | P_Fld(fgPPCIntlv, MJC_PPC_VIDEO_OFF));
}

void vDrvMJCSetRdcType(BOOL fgDft)
{
    if (fgDft == SV_FALSE)
    {
        vIO32Write4B(MJC_FM_08, 0);
        vIO32Write4B(MJC_FM_13, 0);
        vIO32Write4BMsk(MJC_FM_14, 0, 0xFFFFF);
        vIO32WriteFldMulti(MJC_MC_01, P_Fld(0, MJC_MC_TRUE_RPT_MAN_PV)|P_Fld(0, MJC_MC_TRUE_RPT_MAN_CU));
        return;
    }

    vIO32WriteFldMulti(MJC_MC_01, P_Fld(1, MJC_MC_TRUE_RPT_MAN_PV)|P_Fld(1, MJC_MC_TRUE_RPT_MAN_CU));

    if (arMJCPrm.arFRCConfig.u1OutFrmRate < 192)
    {
        vIO32Write4B(MJC_FM_08,
            (((UINT32)MJC_PROG_BWRDC_N_10_120 & 0xFF) << 24) |
            (((UINT32)MJC_PROG_BWRDC_N_08_120) << 8) |
            (((UINT32)MJC_PROG_BWRDC_N_04_120) << 0));

        if (arMJCPrm.arFRCConfig.u1FrmSchMode != FALLBACK_MODE_55P2)
        {
            vIO32Write4B(MJC_FM_13,
                (((UINT32)MJC_PROG_BWRDC_N_20_120 & 0xFFFFF) << 12) |
                (((UINT32)MJC_PROG_BWRDC_N_10_120 >> 8) << 0));
            vIO32Write4B(MJC_FM_14,
                (((UINT32)MJC_PROG_BWRDC_N_20_120 >> 20) << 0));
        }
        else
        {
           vIO32Write4B(MJC_FM_13,
                (((UINT32)MJC_PROG_BWRDC_55P2_N_20_120 & 0xFFFFF) << 12) |
                (((UINT32)MJC_PROG_BWRDC_N_10_120 >> 8) << 0));
           vIO32Write4B(MJC_FM_14,
                (((UINT32)MJC_PROG_BWRDC_55P2_N_20_120 >> 20) << 0));
        }
    }
    else
    {
        vIO32Write4B(MJC_FM_08,
            (((UINT32)MJC_PROG_BWRDC_N_10_240 & 0xFF) << 24) |
            (((UINT32)MJC_PROG_BWRDC_N_08_240) << 8) |
            (((UINT32)MJC_PROG_BWRDC_N_04_240) << 0));

        if(arMJCPrm.arFRCConfig.u1FrmSchMode != FALLBACK_MODE_55P2)
        {
            vIO32Write4B(MJC_FM_13,
                (((UINT32)MJC_PROG_BWRDC_N_20_240 & 0xFFFFF) << 12) |
                (((UINT32)MJC_PROG_BWRDC_N_10_240 >> 8) << 0));
            vIO32Write4B(MJC_FM_14,
                (((UINT32)MJC_PROG_BWRDC_N_20_240 >> 20) << 0));
        }
        else
        {
            vIO32Write4B(MJC_FM_13,
                (((UINT32)MJC_PROG_BWRDC_55P2_N_20_240 & 0xFFFFF) << 12) |
                (((UINT32)MJC_PROG_BWRDC_N_10_240 >> 8) << 0));
            vIO32Write4B(MJC_FM_14,
                (((UINT32)MJC_PROG_BWRDC_55P2_N_20_240 >> 20) << 0));
        }
    }
    vIO32WriteFldAlign(MJC_FM_14 , SV_ON, MJC_FRZ_MEMC_CCO_DECI_EN);  // freeze setting for 420 mode
}

void vDrvCliCallPME(BOOL fgIs120Hz, UINT8 fgIs55p2)
{
    vDrvMJCSetProgME(fgIs120Hz, fgIs55p2);
}

void vDrvMJCSetDbgInk(UINT8 u1INKIdex)
{
    if(u1INKIdex == 1)
    {
        vIO32WriteFldAlign(MJC_MC_01 , 1, MJC_MC_DBG_EN);
    }
    else if(u1INKIdex == 2)
    {
        vIO32WriteFldAlign(MCVP_KC_21, 9, OSD_MODE);
        vIO32WriteFldAlign(MCVP_KC_21, 8, OSD_Y_POS);
        vIO32WriteFldAlign(MCVP_KC_21, 8, OSD_X_POS);
    }
    else if(u1INKIdex == 3)
    {
        vIO32WriteFldAlign(MJC_MC_01 , 1, MJC_APL_DBG_EN);
    }
    else if(u1INKIdex == 4)
    {
        vIO32WriteFldAlign(MJC_MC_01 , 1, MJC_SCN_DBG_EN);
    }
    else
    {
        vIO32WriteFldAlign(MJC_MC_01 , 0, MJC_MC_DBG_EN);
        vIO32WriteFldAlign(MCVP_KC_21, 0, OSD_MODE);
        vIO32WriteFldAlign(MCVP_KC_21, 0, OSD_Y_POS);
        vIO32WriteFldAlign(MCVP_KC_21, 0, OSD_X_POS);
        vIO32WriteFldAlign(MJC_MC_01 , 0, MJC_APL_DBG_EN);
        vIO32WriteFldAlign(MJC_MC_01 , 0, MJC_SCN_DBG_EN);
    }
}

extern UINT32 GCD(UINT32 a, UINT32 b);

void vDrvMJCUpdateFrameTrack(void)
{
    UINT32 u4FrmTrkLine;
    UINT32 u4InOutGCD;

    u4FrmTrkLine = ((UINT32)arMJCPrm.arVrmSrmConfig.u2MJCInVTotal) * ((UINT32)arMJCPrm.arFRCConfig.u1InFrmRate) / ((UINT32)arMJCPrm.arFRCConfig.u1OutFrmRate);
    u4FrmTrkLine = u4FrmTrkLine >> 3;
    vIO32WriteFldAlign(MJC_SYS_D8, u4FrmTrkLine , MJC_FRM_TRK_LINE);
    vIO32WriteFldAlign(MJC_FM_REV0, E_MJC_TRK_REF_LATE, MJC_FRM_TRK_REF_SEL);
    vIO32WriteFldAlign(MJC_SYS_DDDS, SV_ON, MJC_DDDS_TRK_INV);

    if (arMJCPrm.arFRCConfig.u13DModeIn == E_VDO_MJC_FS)
    {
        u4InOutGCD = GCD(arMJCPrm.arVrmSrmConfig.u2MJCInFrameRate >> 1, arMJCPrm.arVrmSrmConfig.u2MJCOutFrameRate);
        vDrvMJCSetFrmMask((arMJCPrm.arVrmSrmConfig.u2MJCInFrameRate >> 1)/u4InOutGCD, 0);
        vIO32WriteFldMulti(MJC_FM_TRK, 
            P_Fld(SV_ON, MJC_FRM_MASK_LR_EN) |
            P_Fld(SV_ON, MJC_FRM_MASK_LR_SEL));
    }
    else
    {
        u4InOutGCD = GCD(arMJCPrm.arVrmSrmConfig.u2MJCInFrameRate, arMJCPrm.arVrmSrmConfig.u2MJCOutFrameRate);
        vDrvMJCSetFrmMask(arMJCPrm.arVrmSrmConfig.u2MJCInFrameRate/u4InOutGCD, 0);
        vIO32WriteFldMulti(MJC_FM_TRK, 
            P_Fld(SV_OFF, MJC_FRM_MASK_LR_EN) |
            P_Fld(SV_OFF, MJC_FRM_MASK_LR_SEL));
    }

}

void vDrvMJCSetFrmSch(MJC_FRC_CONFIG_T * arMJCFRC)
{
    UINT32 fgProgFrmRateEn;
    UINT32 fg2XRateEn;
    UINT32 fg4XRateEn;
    UINT32 fgAuto32En;
    UINT32 fgAuto22En;
    UINT32 u4DramFrmCnt;
    UINT32 fgFallback64;
    UINT32 fgFallback55p2;
   	#ifdef MJC_SW_SUPPORT_FRM_DELAY
    UINT32 fgProgFrmRateMaxEn;
   	#endif

    switch(arMJCFRC->u13DModeIn)
    {
    default:
    case E_VDO_MJC_FS:
        if (arMJCFRC->u1OSMPFlg == SV_OFF)
        {
            if (arMJCFRC->u1InFrmRate > 60)
            {
                fgProgFrmRateEn = SV_OFF;
                fg2XRateEn      = (arMJCFRC->u1OutFrmRate < 125) ? SV_OFF : SV_ON;
                fg4XRateEn      = SV_OFF;
                fgAuto32En      = (arMJCFRC->u1InFrmRate == 100) ? SV_OFF : SV_ON;
                fgAuto22En      = SV_ON;
                fgFallback64    = ((arMJCFRC->u1InFrmRate == 120) && (arMJCFRC->u1OutFrmRate >= 192)) ? SV_ON : SV_OFF;
                fgFallback55p2  = SV_OFF;
            }
            else
            {
                fgProgFrmRateEn = ((arMJCFRC->u1InFrmRate == 24 || arMJCFRC->u1InFrmRate == 30 || arMJCFRC->u1InFrmRate == 40) && IS_LVDS_DISP_3D_SHUTTER) ? SV_OFF : SV_ON;
                fg2XRateEn      = SV_OFF;
                fg4XRateEn      = SV_OFF;
                vDrvMJCSetProgFrmRate(arMJCFRC->u1InFrmRate, arMJCFRC->u1OutFrmRate);
                fgAuto32En      = SV_OFF;
                fgAuto22En      = SV_OFF;
                fgFallback64    = ((arMJCFRC->u1InFrmRate == 48) && (arMJCFRC->u1OutFrmRate > 125)) ? SV_ON : SV_OFF;
                fgFallback55p2  = SV_OFF;
                #ifdef SUPPORT_DISP_WEB_VIDEO_SMOOTHER
                fgFallback64    = ((arMJCFRC->u1InFrmRate == 24) && (arMJCFRC->u1OutFrmRate == 120)) ? SV_ON : fgFallback64;
                #endif
            }
            break;
        }
        else
        {
            if (arMJCFRC->u1InFrmRate > 60)
            {
                fgProgFrmRateEn = SV_OFF;
                fg2XRateEn      = ((arMJCFRC->u1OutFrmRate > 125) || ((arMJCFRC->u1OutFrmRate == 60) || (arMJCFRC->u1OutFrmRate == 50))) ? SV_OFF : SV_ON;
                fg4XRateEn      = ((fg2XRateEn == SV_OFF) && ((arMJCFRC->u1OutFrmRate != 60) && (arMJCFRC->u1OutFrmRate != 50)));
                fgAuto32En      = (arMJCFRC->u1InFrmRate == 100) ? SV_OFF : SV_ON;
                fgAuto22En      = SV_ON;
                fgFallback64    = ((arMJCFRC->u1InFrmRate == 120) && (arMJCFRC->u1OutFrmRate > 96)) ? SV_ON : SV_OFF;
                fgFallback55p2  = SV_OFF;
            }
            else
            {
                fgProgFrmRateEn = SV_ON;
                fg2XRateEn      = SV_OFF;
                fg4XRateEn      = SV_OFF;
                vDrvMJCSetProgFrmRate((arMJCFRC->u1InFrmRate >> 1), arMJCFRC->u1OutFrmRate);
                fgAuto32En      = SV_OFF;
                fgAuto22En      = SV_OFF;
                fgFallback64    = ((arMJCFRC->u1InFrmRate == 48) && (arMJCFRC->u1OutFrmRate > 96)) ? SV_ON : SV_OFF;
                fgFallback55p2  = SV_OFF;
                #ifdef SUPPORT_DISP_WEB_VIDEO_SMOOTHER
                fgFallback64    = ((arMJCFRC->u1InFrmRate == 24) && (arMJCFRC->u1OutFrmRate == 60 || arMJCFRC->u1OutFrmRate == 120)) ? SV_ON : fgFallback64;
                #endif
            }
            break;
        }
    case E_VDO_MJC_SBS:
    case E_VDO_MJC_TAB:
    case E_VDO_MJC_LI:
        if (arMJCFRC->u1OSMPFlg == SV_OFF)
        {
            if (arMJCFRC->u1InFrmRate > 30)
            {
                fgProgFrmRateEn = SV_OFF;
                fg2XRateEn      = (arMJCFRC->u1OutFrmRate < 125) ? SV_OFF : SV_ON;
                fg4XRateEn      = SV_OFF;
                fgAuto32En      = (arMJCFRC->u1InFrmRate == 50) ? SV_OFF : SV_ON;
                fgAuto22En      = SV_ON;
                fgFallback64    = ((arMJCFRC->u1OutFrmRate < 125) || (arMJCFRC->u1InFrmRate == 50)) ? SV_OFF : SV_ON;
                fgFallback55p2  = SV_OFF;
            }
            else
            {
                fgProgFrmRateEn = SV_ON;
                fg2XRateEn      = SV_OFF;
                fg4XRateEn      = SV_OFF;
                vDrvMJCSetProgFrmRate(arMJCFRC->u1InFrmRate, arMJCFRC->u1OutFrmRate >> 1);
                fgAuto32En      = SV_OFF;
                fgAuto22En      = SV_OFF;
                fgFallback64    = ((arMJCFRC->u1InFrmRate == 24) && (arMJCFRC->u1OutFrmRate > 125)) ? SV_ON : SV_OFF;
                fgFallback55p2  = SV_OFF;
            }
            break;
        }
    case E_VDO_MJC_2D:
    case E_VDO_MJC_SG_OSMP_2D:
        if (arMJCFRC->u1InFrmRate > 30)
        {
            fgProgFrmRateEn = SV_OFF;
            fg2XRateEn      = ((arMJCFRC->u1OutFrmRate > 125) || ((arMJCFRC->u1OutFrmRate == 60) || (arMJCFRC->u1OutFrmRate == 50))) ? SV_OFF : SV_ON;
            fg4XRateEn      = ((fg2XRateEn == SV_OFF) && ((arMJCFRC->u1OutFrmRate != 60) && (arMJCFRC->u1OutFrmRate != 50)));
            fgAuto32En      = (arMJCFRC->u1InFrmRate == 50) ? SV_OFF : SV_ON;
            fgAuto22En      = SV_ON;
            fgFallback64    = ((arMJCFRC->u1InFrmRate == 60) && (arMJCFRC->u1OutFrmRate > 96)) ? SV_ON : SV_OFF;
            fgFallback55p2  = ((arMJCFRC->u1OutFrmRate > 125) ) ? SV_OFF : SV_OFF;
        }
        else
        {
            fgProgFrmRateEn = SV_ON;
            fg2XRateEn      = SV_OFF;
            fg4XRateEn      = SV_OFF;
            vDrvMJCSetProgFrmRate(arMJCFRC->u1InFrmRate, arMJCFRC->u1OutFrmRate);
            fgAuto32En      = SV_OFF;
            fgAuto22En      = SV_OFF;
            fgFallback64    = ((arMJCFRC->u1InFrmRate == 24) && (arMJCFRC->u1OutFrmRate == 120)) ? SV_ON : SV_OFF;
            fgFallback55p2  = ((arMJCFRC->u1InFrmRate == 24) && (arMJCFRC->u1OutFrmRate > 125)) ? SV_OFF: SV_OFF;
            #ifdef SUPPORT_DISP_WEB_VIDEO_SMOOTHER
            fgFallback64    = (arMJCFRC->u1InFrmRate == 12)? SV_ON : fgFallback64;
            fgFallback64    = ((arMJCFRC->u1InFrmRate == 20) && (arMJCFRC->u1OutFrmRate > 96)) ? SV_ON : fgFallback64;
            #endif
        }
        break;
    }

    arMJCFRC->fgProgFrtEn    = fgProgFrmRateEn;
    arMJCPrm.u1PSAuto32OnOff = fgAuto32En;
    arMJCPrm.u1PSAuto22OnOff = fgAuto22En;
    vIO32WriteFldAlign(MJC_SYS_04, ((arMJCFRC->u1OSMPFlg == SV_ON)? SV_ON : SV_OFF), MJC_B2R_INIT_FAST);
    vIO32WriteFldAlign(MJC_ME_00, (((arMJCFRC->u13DModeIn == E_VDO_MJC_2D) || (arMJCFRC->u13DModeIn == E_VDO_MJC_SG_OSMP_2D))? SV_OFF : SV_ON) ,
                       MJC_3D_SCNCHG_MODE);

    vIO32WriteFldAlign(MJC_SYS_06, (arMJCFRC->u1OSMPFlg == SV_ON)? 2 : 0, MJC_SA_PRELOAD_MODE);   // 1. 21:9;2. OSMP;3. OTHER  MJC TO_DO  21:9 ADD

    if ((arMJCFRC->u13DModeIn == E_VDO_MJC_SG_OSMP_2D) && ((arMJCFRC->u1InFrmRate == 60) || (arMJCFRC->u1InFrmRate == 24)) && (arMJCFRC->u1OutFrmRate > 96))
    {
        fgFallback64 = SV_ON;
        fgFallback55p2 = SV_OFF;
    }

    if (fgFallback64)
    {
        arMJCFRC->u1FrmSchMode = FALLBACK_MODE_64;
    }
    else
    {
        arMJCFRC->u1FrmSchMode = (fgFallback55p2) ? FALLBACK_MODE_55P2 : FALLBACK_MODE_55;
    }

   #ifdef MJC_SW_SUPPORT_FRM_DELAY
     fgProgFrmRateMaxEn = IO32ReadFldAlign(MJC_FW_SCH_08,MJC_ENABLE_DRAM_FRM_MAX);
     u4DramFrmCnt =  (fgProgFrmRateMaxEn) ? 14 :
    				(fgProgFrmRateEn  ? 5 : 8);
   #else
     u4DramFrmCnt = (fgProgFrmRateEn  ? 5 : 8);
   #endif

    vIO32WriteFldAlign(MJC_SYS_04, fgProgFrmRateEn, MJC_PROG_FRM_EN);
    vIO32WriteFldAlign(MJC_FB_03, u4DramFrmCnt, MJC_DRAM_FRM_CNT_M1);
    vIO32WriteFldMulti(MJC_FM_00,
        P_Fld((arMJCFRC->u1OutFrmRate > 192 ? SV_ON : SV_OFF), MJC_PPC_FORCE_INTLV) |
        P_Fld(fgFallback64, MJC_FALLBACK_64) |
        P_Fld(fgFallback55p2, MJC_FALLBACK_55P2) |
        P_Fld(fg2XRateEn, MJC_OUT_DB_RATE_EN) |
        P_Fld(fg4XRateEn, MJC_OUT_4X_RATE_EN));

    if (arMJCFRC->u13DModeIn != E_VDO_MJC_FS)
    {
        vIO32WriteFldAlign(MJC_SYS_09, SV_ON, MJC_IN_LR_FORCE_ZERO);//cc:2 bit
    }
    else
    {
        vIO32WriteFldAlign(MJC_SYS_09, SV_OFF, MJC_IN_LR_FORCE_ZERO);
    }

    vDrvMJCUpdateIntpCtrl();

}

void vDrvMJCSetFrmschUniCandeceProc(const MJC_FRC_CONFIG_T * arMJCFRC)
{
    UINT8 u1UniDouble;
    UINT8 u1CinemaType;
    #ifdef MJC_SW_SUPPORT_FRM_DELAY
    UINT32 u4ProgFrmRateMaxEn;
    #endif

    if (arMJCFRC->u1OSMPFlg == SV_OFF)
    {
        if (arMJCFRC->u1PanelType == E_MJC_60HZ)
        {
            u1UniDouble = (arMJCFRC->u1UniCadenceEn &&
                          (arMJCFRC->fgProgFrtEn && ((arMJCFRC->u13DModeIn == E_VDO_MJC_2D) || (arMJCFRC->u13DModeIn == E_VDO_MJC_SG_OSMP_2D))))? SV_ON : SV_OFF;
        }
        else if (arMJCFRC->u1PanelType == E_MJC_120HZ)
        {
            u1UniDouble = (arMJCFRC->u1UniCadenceEn &&
                          (!arMJCFRC->fgProgFrtEn && ((arMJCFRC->u13DModeIn != E_VDO_MJC_2D) && (arMJCFRC->u13DModeIn != E_VDO_MJC_SG_OSMP_2D))))? SV_OFF : SV_ON;
        }
        else
        {
            u1UniDouble = SV_OFF;
        }
    }
    else
    {
        if (arMJCFRC->u1PanelType == E_MJC_60HZ)
        {
            u1UniDouble = (arMJCFRC->u1UniCadenceEn && arMJCFRC->fgProgFrtEn )? SV_ON : SV_OFF;
        }
        else if (arMJCFRC->u1PanelType == E_MJC_120HZ)
        {
            u1UniDouble = (arMJCFRC->u1UniCadenceEn)? SV_ON : SV_OFF;
        }
        else
        {
            u1UniDouble = SV_OFF;
        }
    }

    u1CinemaType = (arMJCFRC->u1InFrmRate  == 12) ? E_MJC_CINEMA_12 :
                   (arMJCFRC->u1InFrmRate  == 15) ? E_MJC_CINEMA_15 :
                   (arMJCFRC->u1InFrmRate  == 20) ? E_MJC_CINEMA_20 :
                   ((arMJCFRC->u1InFrmRate == 24) || ((arMJCFRC->u1InFrmRate == 48) && (arMJCFRC->u13DModeIn == E_VDO_MJC_FS))) ? E_MJC_CINEMA_24 :
                   ((arMJCFRC->u1InFrmRate == 30) || ((arMJCFRC->u1InFrmRate == 60) && (arMJCFRC->u13DModeIn == E_VDO_MJC_FS))) ? E_MJC_CINEMA_30 :
                   ((arMJCFRC->u1InFrmRate == 40) && (arMJCFRC->u13DModeIn == E_VDO_MJC_FS)) ? E_MJC_CINEMA_40 :
                   ((arMJCFRC->u1InFrmRate == 25) || ((arMJCFRC->u1InFrmRate == 50) && (arMJCFRC->u13DModeIn == E_VDO_MJC_FS))) ? E_MJC_CINEMA_25 : E_MJC_NO_CINEMA;

    vIO32WriteFldAlign(MJC_SYS_04,    (arMJCFRC->fgProgFrtEn && !(arMJCFRC->u1UniCadenceEn)),              MJC_PROG_FRM_EN);
    #ifdef MJC_SW_SUPPORT_FRM_DELAY
     u4ProgFrmRateMaxEn = IO32ReadFldAlign(MJC_FW_SCH_08,MJC_ENABLE_DRAM_FRM_MAX); 
     vIO32WriteFldAlign(MJC_FB_03,     (u4ProgFrmRateMaxEn ? 14 : ((arMJCFRC->fgProgFrtEn && !(arMJCFRC->u1UniCadenceEn)) ? 5 : 8)),    MJC_DRAM_FRM_CNT_M1);
    #else
     vIO32WriteFldAlign(MJC_FB_03,     ((arMJCFRC->fgProgFrtEn && !(arMJCFRC->u1UniCadenceEn)) ? 5 : 8),    MJC_DRAM_FRM_CNT_M1);
    #endif
    vIO32WriteFldAlign(MJC_FW_SCH_03, u1CinemaType, MJC_CINEMA_TYPE);
    vIO32WriteFldAlign(MJC_FW_SCH_03, ((IS_LVDS_DISP_3D_SHUTTER == 0) && arMJCFRC->u1UniCadenceEn && ((arMJCFRC->u1InFrmRate % 24 == 0) || (arMJCFRC->u1InFrmRate % 30 == 0))), MJC_60TO100_EN);
    vIO32WriteFldAlign(MJC_FW_SCH_03, (arMJCFRC->u13DModeIn == E_VDO_MJC_SG_OSMP_2D), MJC_UNICADENCE_TTD);
    vIO32WriteFldAlign(MJC_FW_SCH_03,  u1UniDouble, MJC_UNICADENCE_DOUBLE);
    vIO32WriteFldMulti(MJC_FW_SCH_01, P_Fld(arMJCFRC->u1UniCadenceEn ? 1 : (MJC_SUPPORT_MULTI_CADENCE&&(arMJCFRC->u1InFrmRate == 60)
                                      ? 2 : 0) /* if muticadence, set 2*/, MJC_FW_FRM_SCH_SWITCH)|
                                      P_Fld(arMJCFRC->u1PanelType == E_MJC_60HZ, MJC_FW_FRM_SCH_1X)|
                                      P_Fld(arMJCFRC->fgProgFrtEn ? 0 : ((arMJCFRC->u1InFrmRate % 50) == 0 ? SW_FILM_AUTO_22 : SW_FILE_AUTO_ALL), MJC_FW_FRM_SCH_AUTO_DET) |
                                      P_Fld(0xF, MJC_FW_FRM_SCH_TST_DLY)|
                                      P_Fld(arMJCFRC->u1InFrmRate % 25 == 0 ? 5 : 6, MJC_FW_FRM_SCH_IN_DLY));

}

void vDrvMJCSetPicSize(const MJC_PIC_SIZE_T * arMjcPicSize)
{
    vIO32WriteFldMulti(MJC_SYS_02,
         P_Fld(arMjcPicSize->u2InWidth, MJC_MJC_WIDTH)|
         P_Fld(arMjcPicSize->u2InHeight, MJC_MJC_HEIGHT_FX));
    vIO32WriteFldAlign(MJC_SYS_0C, arMjcPicSize->u2WBlank, MJC_IN_BLACK_W);
    vIO32WriteFldAlign(MJC_SYS_0C, arMjcPicSize->u2HBlank , MJC_IN_BLACK_H);
    vIO32WriteFldAlign(MJC_FB_LR3D, arMjcPicSize->fgDdp, MJC_3D_DDP_EN);
    vIO32WriteFldAlign(MJC_SYS_08, arMjcPicSize->u2InVsyncDly, MJC_MJC_VSYNC_DLY);
    vIO32WriteFldAlign(MJC_SYS_DISP, arMjcPicSize->u1DispVpos, MJC_DISP_VPOS);
}

void vDrvMJCSetDispRDly(UINT32 u4FrmDly)
{
    vIO32WriteFldAlign(MJC_SYS_03, u4FrmDly, MJC_OUT_LR3D_DLY);
}

void vDrvMJCSetMinDly(UINT32 fgOnOff, UINT32 u4LineThd)
{
    vIO32WriteFldMulti(MJC_FB_RPT,
        P_Fld(fgOnOff, MJC_RPT_SHRTDLY_MODE) |
        P_Fld(u4LineThd, MJC_RPT_SHRTDLY_LINE_THD));
}

void MJC_Set3DModeDbg(UINT8 u13DModeIn , UINT8 u13DModeOut)
{
    if(u13DModeIn < 6)
    {
        arMJCPrm.arVrmSrmConfig.u1MJC3DModeIn = u13DModeIn;
    }
    else
    {
        LOG(0 , "Unknow MJC 3D In Fomat\n");
    }
    if (u13DModeOut < 4)
    {
        arMJCPrm.arVrmSrmConfig.u1MJC3DModeOut = u13DModeOut;
    }
    else
    {
        LOG(0 , "Unknow MJC 3D Out Fomat\n");
    }
    LOG(3,"fgMJC3DMode u13DModeIn:%d u13DModeOut:%d\n" , u13DModeIn ,u13DModeOut);
    fgMJCModeChg = SV_TRUE;

    if (fgDrvMJCModeChgJudgment(SV_VP_MAIN))
    {
        if (arMJCPrm.u1Dram444 == SV_FALSE)
        {
            _vDrvVideoSetMute(MUTE_MODULE_MJC, SV_VP_MAIN, IO32ReadFldAlign(MJC_MISC_00, MODE_CHG_MUTE_COUNT), SV_TRUE);
        }
    }
    u1TrigSetUniCandeceTrg = (((u1DrvMJCIsSupportUniCandece() == SV_TRUE) && (arMJCPrm.u1Dram444 == SV_FALSE))? IO32ReadFldAlign(MJC_MISC_00, MJC_MUTE_COUNT) : 0);
    vDrvMJCChangeFrmSch();
    vDrvMJCChangePicSize();
}

void vDrvMJCSetSG3D(MJC_FRC_CONFIG_T* arMJCFRC)
{
    vIO32WriteFldAlign(MJC_SYS_06, SV_OFF, MJC_OSMP_MODE);
    vIO32WriteFldAlign(MJC_FW_SCH_05, SV_OFF, MJC_FW_SCH_OSMP_MODE);
    vIO32WriteFldAlign(MJC_FW_SCH_01, SV_OFF, MJC_FW_FRM_SCH_3D_MODE);
    vIO32WriteFldAlign(MJC_SYS_00, SV_OFF, MJC_GRP_RPT_EN);
    vIO32WriteFldMulti(MJC_FB_LR3D, P_Fld(SV_OFF, MJC_3D_MODE) | P_Fld(0, MJC_3D_TYPE));
    vIO32WriteFldAlign(MJC_FB_09, SV_OFF, MJC_MID_VSYNC_IN_EN);
    vIO32WriteFldAlign(MJC_FM_00, SV_OFF, MJC_LR3D_IN_FREERUN);
    vIO32WriteFldAlign(MJC_MV_24, 0x5, MJC_MV_YAPL_FRM_CNT);

    arMJCFRC->u1OSMPFlg = SV_OFF;
    arMJCDemo.arRgn[0] = E_MJC_RGN_NF;
    arMJCDemo.arRgn[1] = E_MJC_RGN_NF;
    arMJCDemo.arRgn[2] = E_MJC_RGN_NF;
    switch (arMJCFRC->u13DModeIn)
    {
        case E_VDO_MJC_SG_OSMP_2D:
            vIO32WriteFldAlign(MJC_SYS_00, SV_ON, MJC_GRP_RPT_EN);
            arMJCDemo.arRgn[0] = E_MJC_RGN_HF;
            arMJCDemo.arRgn[1] = E_MJC_RGN_HF;
            arMJCDemo.arRgn[2] = E_MJC_RGN_HF;
            break;
        case E_VDO_MJC_SBS:
        case E_VDO_MJC_TAB:
        case E_VDO_MJC_LI:
            vIO32WriteFldMulti(MJC_FB_LR3D, P_Fld(SV_ON, MJC_3D_MODE) | P_Fld(arMJCFRC->u13DModeIn - 1, MJC_3D_TYPE));
            vIO32WriteFldAlign(MJC_FM_00, SV_ON, MJC_LR3D_IN_FREERUN);
            vIO32WriteFldAlign(MJC_FB_09, SV_ON, MJC_MID_VSYNC_IN_EN);
            vIO32WriteFldAlign(MJC_FW_SCH_01, SV_ON, MJC_FW_FRM_SCH_3D_MODE);
            vIO32WriteFldAlign(MJC_MV_24, 0xA, MJC_MV_YAPL_FRM_CNT);
            break;
        case E_VDO_MJC_FS:
            vIO32WriteFldMulti(MJC_FB_LR3D, P_Fld(SV_ON, MJC_3D_MODE) | P_Fld(arMJCFRC->u13DModeIn - 1, MJC_3D_TYPE));
            vIO32WriteFldAlign(MJC_FW_SCH_01, SV_ON, MJC_FW_FRM_SCH_3D_MODE);
            vIO32WriteFldAlign(MJC_MV_24, 0xA, MJC_MV_YAPL_FRM_CNT);
            break;
        default :
            break;
    }
}

void vDrvMJCSetMCFPR(UINT32 fgOnOff, UINT32 fgLRSwap, UINT32 fgFlip, UINT32 u4FPRType)
{
    UINT32 u4MCFprLineNum;
    UINT32 fgMCFprLineSel;
    UINT32 fgMCFprIdx0En;
    UINT32 fgMCFprIdx1En;
    UINT32 fgMCFprLEn;
    UINT32 fgMCFprREn;

    UINT32 fgOutLRPhaseSwap = 0;
    UINT32 fgFprInvOrderLbox = 0;
    UINT32 fgLboxRdFlip;

    LOG(3, "vDrvMJCSetMCFPR: %d, %d, %d, %d\n", fgOnOff, fgLRSwap, fgFlip, u4FPRType);

    u4MCFprLineNum = (fgFlip ? (arMJCPrm.arPicSize.u2InHeight - (arMJCPrm.arPicSize.u2OutHeight >> 1)) : 0) + (u4FPRType == E_MJC_FPRV3 ? (arMJCPrm.arPicSize.u2OutHeight >> 1) - 1 : 0);
    fgMCFprLineSel = u4FPRType == E_MJC_FPRV2 ? SV_ON : SV_OFF;
    fgMCFprIdx0En  = SV_OFF;
    fgMCFprIdx1En  = SV_ON;
    fgMCFprLEn     = (u4FPRType == E_MJC_FPRV2)? SV_ON : SV_OFF;
    fgMCFprREn     = fgMCFprLEn == SV_OFF;

    if (fgOnOff == SV_OFF)
    {
        fgMCFprLEn = SV_OFF;
        fgMCFprREn = SV_OFF;
    }

    LOG(3, "vDrvMJCSetMCFPR: %d, %d, %d, %d, %d, %d\n", u4MCFprLineNum, fgMCFprLineSel, fgMCFprIdx0En, fgMCFprIdx1En, fgMCFprLEn, fgMCFprREn);

     vIO32WriteFldMulti(MJC_MC_2F,
         P_Fld(u4MCFprLineNum, MJC_MC_FPR_LINE_NUM) |
         P_Fld(fgMCFprLineSel, MJC_MC_FPR_LINE_SEL) |
         P_Fld(fgMCFprIdx0En, MJC_MC_FPR_IDX0_EN) |
         P_Fld(fgMCFprIdx1En, MJC_MC_FPR_IDX1_EN) |
         P_Fld(fgMCFprLEn, MJC_MC_FPR_L_EN) |
         P_Fld(fgMCFprREn, MJC_MC_FPR_R_EN));

    if ((fgFlip == SV_TRUE && fgLRSwap == SV_FALSE) ||
        (fgFlip == SV_FALSE && fgLRSwap == SV_TRUE))
    {
        fgOutLRPhaseSwap = SV_ON;
    }

    if (u4FPRType == E_MJC_FPRV0)
    {
        fgOutLRPhaseSwap = SV_OFF;
        fgFprInvOrderLbox = fgFlip ? SV_ON : SV_OFF;
    }
    else if (u4FPRType == E_MJC_FPRV1)
    {
        fgFprInvOrderLbox = fgOutLRPhaseSwap;
        fgOutLRPhaseSwap = SV_OFF;
    }

    fgLboxRdFlip = fgFlip ? SV_ON : SV_OFF;

    vIO32WriteFldMulti(MJC_FB_05,
        P_Fld(fgOutLRPhaseSwap, MJC_OUT_LR_PHAS_SWP) |
        P_Fld(fgFprInvOrderLbox, MJC_FPR_INV_ORDER_LBOX));

    vIO32WriteFldAlign(MJC_FB_LBOX_0, fgLboxRdFlip, MJC_LBOX_RD_FLIP);
    
}

void vDrvMJCSetRescan(UINT8  u1RescanOnOff)
{
  vIO32WriteFldAlign(MJC_SA_00, u1RescanOnOff, MJC_RESCAN_EN);

  arMJCPrm.u1Rscn = u1RescanOnOff;
}
void vDrvMJCSetOSMP(MJC_FRC_CONFIG_T* arMJCFRC)
{
    vIO32WriteFldAlign(MJC_SYS_06, SV_OFF, MJC_OSMP_MODE);
    vIO32WriteFldAlign(MJC_FW_SCH_05, SV_OFF, MJC_FW_SCH_OSMP_MODE);
    vIO32WriteFldAlign(MJC_FW_SCH_01, SV_OFF, MJC_FW_FRM_SCH_3D_MODE);
    vIO32WriteFldAlign(MJC_SYS_06, SV_OFF, MJC_OSMP_HPR);
    vIO32WriteFldAlign(MJC_SYS_06, SV_OFF, MJC_OSMP_2D_IN);
    vIO32WriteFldAlign(MJC_FB_LR3D, 0, MJC_3D_TYPE);
    vIO32WriteFldAlign(MJC_SYS_00, SV_OFF, MJC_GRP_RPT_EN);
    vIO32WriteFldAlign(MJC_FM_00, SV_OFF, MJC_LR3D_IN_FREERUN);
    vIO32WriteFldAlign(MJC_FB_LR3D, SV_OFF, MJC_3D_MODE);
    vIO32WriteFldAlign(MJC_FB_09, SV_OFF, MJC_MID_VSYNC_IN_EN);     // MID_SYNV NEET == SV_OFF  WHEN OSMP MODE;
    

    vIO32WriteFldAlign(MJC_ME_2F, 0x30, MJC_MV_3FRM_Y_BOUND);
    vIO32WriteFldAlign(MJC_ME_2F, 0x60, MJC_MV_Y_BOUND);
    //vIO32WriteFldAlign(MJC_STA_PPC_00, 8, MJC_PPC_YLRG_TH);
    vIO32WriteFldAlign(MJC_MV_2E, 0x30, MJC_MVC_VRNG_TH);
    vIO32WriteFldAlign(MJC_ME_29, 0x30, MJC_ME_MF_GLBV_TH);
    
    if (((arMJCFRC->u13DModeOut == E_VD0_MJC_OUT_FS) || (arMJCFRC->u13DModeOut == E_VDO_MJC_OUT_2D)) && (arMJCPrm.arMJCPRConfig.u1MJCOSMPHPRMode != E_MJC_HPR2160))
    {
        arMJCFRC->u1OSMPFlg = SV_OFF;
        arMJCFRC->u1VRgn= 64;
		vDrvMJCSetRescan(1);
    }
    else
    {
        arMJCFRC->u1OSMPFlg = SV_ON;
        arMJCFRC->u1VRgn= 32;
        vDrvMJCSetRescan(0);
        if ((arMJCFRC->u13DModeOut == E_VD0_MJC_OUT_HPR) || (arMJCPrm.arMJCPRConfig.u1MJCOSMPHPRMode == E_MJC_HPR2160))
        {
            vIO32WriteFldAlign(MJC_SYS_06, SV_ON, MJC_OSMP_MODE);
            vIO32WriteFldAlign(MJC_FW_SCH_05, SV_ON, MJC_FW_SCH_OSMP_MODE);
            vIO32WriteFldAlign(MJC_SYS_06, SV_ON, MJC_OSMP_HPR);
        }
        else
        {
            vIO32WriteFldAlign(MJC_SYS_06, SV_ON, MJC_OSMP_MODE);
            vIO32WriteFldAlign(MJC_FW_SCH_05, SV_ON, MJC_FW_SCH_OSMP_MODE);
            vIO32WriteFldAlign(MJC_SYS_06, SV_OFF, MJC_OSMP_HPR);
            vIO32WriteFldAlign(MJC_SYS_06, arMJCPrm.arMJCPRConfig.u1MJCOSMPFPRMode, MJC_OSMP_FPR_TYPE);
        }
    }
    switch (arMJCFRC->u13DModeIn)
    {
        case E_VDO_MJC_2D:
            if (arMJCPrm.arMJCPRConfig.u1MJCOSMPHPRMode != E_MJC_HPR2160)
            {
                break;
            }
        case E_VDO_MJC_SG_OSMP_2D:
            vIO32WriteFldAlign(MJC_SYS_06, SV_ON, MJC_OSMP_2D_IN);
            break;
        case E_VDO_MJC_SBS:
        case E_VDO_MJC_TAB:
        case E_VDO_MJC_LI:
        case E_VDO_MJC_FS:
            vIO32WriteFldAlign(MJC_FB_LR3D, arMJCFRC->u13DModeIn - 1, MJC_3D_TYPE);
            break;
        default :
            break;
    }

    vIO32WriteFldAlign(MJC_FW_SCH_01, (arMJCFRC->u13DModeIn == E_VDO_MJC_FS), MJC_FW_FRM_SCH_3D_MODE);
}

void vDrvMJCSetPRMode(UINT8 u1PRMode , UINT8 u1FHPRMode)
{
    if(u1PRMode == E_VD0_MJC_OUT_FPR)
    {
        arMJCPrm.arMJCPRConfig.u1MJCOSMPFPRMode = u1FHPRMode;
        arMJCPrm.arVrmSrmConfig.u1MJC3DModeOut = E_VD0_MJC_OUT_FPR;
    }
    else if (u1PRMode == E_VD0_MJC_OUT_HPR)
    {
        arMJCPrm.arMJCPRConfig.u1MJCOSMPHPRMode = u1FHPRMode;
        arMJCPrm.arVrmSrmConfig.u1MJC3DModeOut = E_VD0_MJC_OUT_HPR;
    }
    else
    {
        LOG(0, "Not PR Mode");
    }
    fgMJCModeChg = SV_TRUE;
}

UINT8 u1DrvMJCIsNeedInverseLR(void)
{
    return 0; //mjc need lr inverse front mjc When is es and input is fs format
}

void vDrvMJCSet3DLRDly(void)
{
    static UINT8 fgTTDBypassPre = SV_ON;
    UINT8 fgTTDBypassCurr;

    fgTTDBypassCurr = IO32ReadFldAlign(MJC_SYS_DB, MJC_TTD_BYPASS);
    if ((fgTTDBypassCurr != fgTTDBypassPre))
    {
        MJC_SetOutputLRSwap(arMJCPrm.fgDispRSwp);
        fgTTDBypassPre = fgTTDBypassCurr;
    }
}

void vDrvMJCSetDispVpos(UINT32 u4PscanPrelen)
{
    UINT32 u4DispVpos;
    u4DispVpos = u4PscanPrelen - 2;
    arMJCPrm.arPicSize.u1DispVpos = (UINT8)MIN(28, u4DispVpos);
    vIO32WriteFldAlign(MJC_SYS_DISP, arMJCPrm.arPicSize.u1DispVpos, MJC_DISP_VPOS);
}

void vDrvMJCSetDispMode(UINT32 fgOnOff)
{
    vIO32WriteFldAlign(MJC_SYS_DISP, fgOnOff, MJC_DISP_MODE);
}

UINT32 fgDrvMJCGetDispMode(void)
{
    UINT32 fgMJCDispMode;
    fgMJCDispMode = IO32ReadFldAlign(MJC_SYS_DISP, MJC_DISP_MODE) ?  E_MJC_DISP_MODE : E_MJC_SELF_MODE;

    return fgMJCDispMode;
}

UINT32 fgDrvMJCGetFlipMirror(void)
{
    return IO32ReadFldAlign(MJC_FB_0A, MJC_FLIPMRR);
}

static void vDrvMJCUnicadenceEnJudgment(MJC_FRC_CONFIG_T* arMJCFRC)
{
    arMJCFRC->u1UniCadenceEn = SV_OFF;

    if (arMJCFRC->u1InFrmRate % 25 == 0 && arMJCFRC->u1OutFrmRate % 60 == 0)
    {
        arMJCFRC->u1UniCadenceEn = SV_ON;
    }

    if (((arMJCFRC->u1InFrmRate % 24 == 0) || (arMJCFRC->u1InFrmRate % 30 == 0)) &&
        arMJCFRC->u1OutFrmRate % 50 == 0)
    {
         arMJCFRC->u1UniCadenceEn = SV_ON;
    }

    #if 0
    arMJCFRC->u1UniCadenceEn |= ((arMJCFRC->u1InFrmRate == 20) && (arMJCFRC->u1OutFrmRate % 60 == 0));
    arMJCFRC->u1UniCadenceEn |= ((arMJCFRC->u1InFrmRate == 24) && arMJCFRC->u13DModeIn == E_VDO_MJC_FS && IS_LVDS_DISP_3D_SHUTTER);
    arMJCFRC->u1UniCadenceEn |= ((arMJCFRC->u1InFrmRate == 30) && arMJCFRC->u13DModeIn == E_VDO_MJC_FS && IS_LVDS_DISP_3D_SHUTTER);
    arMJCFRC->u1UniCadenceEn |= ((arMJCFRC->u1InFrmRate == 40) && arMJCFRC->u13DModeIn == E_VDO_MJC_FS);
    #endif
}

static BOOL fgDrvMJCGetFRCConfig(MJC_FRC_CONFIG_T* arMJCFRC)
{
    arMJCFRC->u1OutFrmRate   = arMJCPrm.arVrmSrmConfig.u2MJCOutFrameRate;
    arMJCFRC->u1VdoFrmRate   = bDrvVideoGetRefreshRate(SV_VP_MAIN);
    arMJCFRC->u13DModeIn     = arMJCPrm.arVrmSrmConfig.u1MJC3DModeIn;
    arMJCFRC->u13DModeOut    = arMJCPrm.arVrmSrmConfig.u1MJC3DModeOut;
    arMJCFRC->u1InFrmRate    = arMJCPrm.arVrmSrmConfig.u2MJCInFrameRate;
    arMJCFRC->u1Dram444      = arMJCPrm.u1Dram444;

    vDrvMJCUnicadenceEnJudgment(arMJCFRC);

    if (arMJCFRC->u1OutFrmRate < 96)
    {
        arMJCFRC->u1PanelType = E_MJC_60HZ;
    }
    else if (arMJCFRC->u1OutFrmRate < 192)
    {
        arMJCFRC->u1PanelType = E_MJC_120HZ;
    }
    else
    {
        arMJCFRC->u1PanelType = E_MJC_240HZ;
    }

    if ((arMJCFRC->u1OutFrmRate   != arMJCPrm.arFRCConfig.u1OutFrmRate) ||
        (arMJCFRC->u1InFrmRate    != arMJCPrm.arFRCConfig.u1InFrmRate) ||
        (arMJCFRC->u13DModeIn     != arMJCPrm.arFRCConfig.u13DModeIn) ||
        (arMJCFRC->u13DModeOut    != arMJCPrm.arFRCConfig.u13DModeOut) ||
        (arMJCFRC->u1Dram444      != arMJCPrm.arFRCConfig.u1Dram444) ||
        (arMJCFRC->u1UniCadenceEn != arMJCPrm.arFRCConfig.u1UniCadenceEn))
    {
        LOG(2, "fgDrvMJCGetFRCConfig: (%d, %d, %d, %d, %d, %d) -> (%d, %d, %d, %d, %d, %d)\n",
            arMJCPrm.arFRCConfig.u1OutFrmRate,
            arMJCPrm.arFRCConfig.u1InFrmRate,
            arMJCPrm.arFRCConfig.u13DModeIn,
            arMJCPrm.arFRCConfig.u13DModeOut,
            arMJCPrm.arFRCConfig.u1Dram444,
            arMJCPrm.arFRCConfig.u1UniCadenceEn,
            arMJCFRC->u1OutFrmRate,
            arMJCFRC->u1InFrmRate,
            arMJCFRC->u13DModeIn,
            arMJCFRC->u13DModeOut,
            arMJCFRC->u1Dram444,
            arMJCFRC->u1UniCadenceEn
            );
        return SV_TRUE;
    }
    else
    {
        return SV_FALSE;
    }
}

static BOOL fgDrvMJCGetPicSize(MJC_PIC_SIZE_T * arPicSize)
{
    UINT16 u2Height, u2Width;
    UINT16 u2WBlank, u2HBlank;

    arPicSize->u2InHStart   = 0;
    arPicSize->u2InVStart   = 0;
    arPicSize->u2OutHStart  = 0;
    arPicSize->u2OutVStart  = 0;
    arPicSize->u2WBlank     = 0;
    arPicSize->u2HBlank     = 0;
    arPicSize->u2InVsyncDly = 0;
    arPicSize->fgDdp        = SV_OFF;

    if (fgCSSOnOff)
    {
        u2Height = arMJCPrm.arVrmSrmConfig.u2Height - CSS_MAX_COMPENSATION * 2;
        u2Width  = arMJCPrm.arVrmSrmConfig.u2Width - CSS_MAX_COMPENSATION * 2;
    }
    else
    {
        u2Height = arMJCPrm.arVrmSrmConfig.u2Height;
        u2Width  = arMJCPrm.arVrmSrmConfig.u2Width;
    }

    arPicSize->u2OutWidth = u2Width;
    u2WBlank              = arPicSize->u2OutWidth % 16;
    arPicSize->u2WBlank   = ((u2WBlank != 0) ? (16 - u2WBlank) : 0);
    arPicSize->u2InWidth   = ALIGN_16_CEIL(arPicSize->u2OutWidth);
    arPicSize->u2OutHeight = u2Height;
    arPicSize->u2InHeight  = ((arMJCPrm.arFRCConfig.u13DModeOut == E_VD0_MJC_OUT_HPR) || (arMJCPrm.arFRCConfig.u13DModeOut == E_VD0_MJC_OUT_FPR)
                               || (arMJCPrm.arMJCPRConfig.u1MJCOSMPHPRMode == E_MJC_HPR2160))? (u2Height >> 1) : u2Height;

    if (arMJCPrm.arFRCConfig.u13DModeIn == E_VDO_MJC_TAB)
    {
        arPicSize->fgDdp = ((arPicSize->u2InHeight % 8 != 0) && (arMJCPrm.u1Dram444 == SV_FALSE))? SV_ON : SV_OFF;
    }
    else
    {
        arPicSize->fgDdp    = SV_OFF;  // UDP4  REMOVED  AND DDP SETING ONLY IN NOT 2D CASE
        u2HBlank            = arPicSize->u2InHeight % 8 ;
        arPicSize->u2HBlank = (((u2HBlank != 0) && (arMJCPrm.u1Dram444 == SV_FALSE))? (8 - u2HBlank) : 0);

        if ((arMJCPrm.u1Flip == 1) && (u2HBlank != 0) && (arMJCPrm.u1Dram444 == SV_FALSE))
        {
            arPicSize->u2OutVStart += ((8 - u2HBlank) << (((arMJCPrm.arFRCConfig.u13DModeOut == E_VD0_MJC_OUT_HPR) || (arMJCPrm.arFRCConfig.u13DModeOut == E_VD0_MJC_OUT_FPR))? 1 : 0));
        }
    }

    arPicSize->u2InHeight   = (arMJCPrm.u1Dram444 == SV_TRUE) ? arPicSize->u2InHeight : ALIGN_8_CEIL(arPicSize->u2InHeight);
    arPicSize->u2InWidth	= MAX(arPicSize->u2InWidth, 256);
    if (arMJCPrm.u1Mirror == 1)
    {
        arPicSize->u2OutHStart = arPicSize->u2InWidth - arPicSize->u2OutWidth;
    }
    arPicSize->u2OutHTotal  = arMJCPrm.arVrmSrmConfig.u2HTotal;
    //arPicSize->u2OutHTotal  = arPicSize->u2OutHTotal >= 4095 ? 4095 : arPicSize->u2OutHTotal;
    arPicSize->u2OutVTotal  = arMJCPrm.arVrmSrmConfig.u2VTotal;
    arPicSize->u2InVsyncDly = 0; // no need to set in vsync dly to cover OSMP mode preload fail case

    if ((arMJCPrm.u1Flip == 1) && (arPicSize->fgDdp == SV_ON))
    {
        arPicSize->u2OutVStart += 8;
    }

    arPicSize->u1DispVpos = ((arMJCPrm.arFRCConfig.u1PanelType == E_MJC_60HZ) && (arMJCPrm.arMJCPRConfig.u1MJCOSMPHPRMode != E_MJC_HPR2160))? 21 : 33;
    arPicSize->u1DispVpos += arPicSize->u2InVsyncDly;

    if ((arPicSize->u2InHStart              != arMJCPrm.arPicSize.u2InHStart) ||
        (arPicSize->u2InVStart              != arMJCPrm.arPicSize.u2InVStart) ||
        (arPicSize->u2InWidth               != arMJCPrm.arPicSize.u2InWidth) ||
        (arPicSize->u2InHeight              != arMJCPrm.arPicSize.u2InHeight) ||
        (arPicSize->u2InVsyncDly            != arMJCPrm.arPicSize.u2InVsyncDly) ||
        (arPicSize->u2OutWidth              != arMJCPrm.arPicSize.u2OutWidth) ||
        (arPicSize->u2OutHeight             != arMJCPrm.arPicSize.u2OutHeight) ||
        (arPicSize->u2OutHTotal             != arMJCPrm.arPicSize.u2OutHTotal) ||
        (arPicSize->u2OutVTotal             != arMJCPrm.arPicSize.u2OutVTotal) ||
        (arPicSize->u2OutVStart             != arMJCPrm.arPicSize.u2OutVStart) ||
        (arPicSize->u2OutHStart             != arMJCPrm.arPicSize.u2OutHStart) ||
        (arPicSize->u2WBlank                != arMJCPrm.arPicSize.u2WBlank) ||
        (arPicSize->u2HBlank                != arMJCPrm.arPicSize.u2HBlank) ||
        (arPicSize->u1Dram444               != arMJCPrm.arPicSize.u1Dram444) ||
        (arPicSize->fgDdp                   != arMJCPrm.arPicSize.fgDdp) ||
        (arPicSize->u1DispVpos              != arMJCPrm.arPicSize.u1DispVpos))
    {
        return SV_TRUE;
    }
    else
    {
        return SV_FALSE;
    }
}

UINT8 u1DrvMJCGetVrmInfo(UINT8 bPath)
{
    VRM_INFO_T arMJCIn;
    VRM_INFO_T arMJCOut;

    VRMGetModuleVRMInfo(bPath, VRM_MODULE_MJC, &arMJCIn, &arMJCOut);

    LOG(2, "u1DrvMJCGetVrmInfo: (%d, %d, %d), (%d, %d, %d).\n",
        arMJCIn.u2Width, arMJCIn.u2Height, arMJCIn.u2FrameRate,
        arMJCOut.u2Width, arMJCOut.u2Height, arMJCOut.u2FrameRate);

    if (arMJCIn.u2Width == 0 || arMJCIn.u2Height == 0 ||arMJCOut.u2Width == 0 || arMJCOut.u2Height == 0)
    {
        return SV_FAIL;
    }

    arMJCPrm.arVrmSrmConfig.u2MJCInVTotal = arMJCIn.u2VTotal;
    arMJCPrm.arVrmSrmConfig.u2MJCInFrameRate = arMJCIn.u2FrameRate == 0 ? arMJCPrm.arVrmSrmConfig.u2MJCInFrameRate : arMJCIn.u2FrameRate;
    arMJCPrm.arVrmSrmConfig.u2MJCOutFrameRate = (arMJCPrm.arMJCPRConfig.u1MJCOSMPHPRMode == E_MJC_HPR2160)? arMJCOut.u2FrameRate >> 1 : arMJCOut.u2FrameRate;
    arMJCPrm.arVrmSrmConfig.u2Width = arMJCOut.u2Width;
    arMJCPrm.arVrmSrmConfig.u2Width = arMJCOut.u2Width;
    arMJCPrm.arVrmSrmConfig.u2Height = arMJCOut.u2Height;
    arMJCPrm.arVrmSrmConfig.u2HTotal = arMJCOut.u2HTotal;
    arMJCPrm.arVrmSrmConfig.u2VTotal = arMJCOut.u2VTotal;
    return SV_SUCCESS;
}

UINT32 fgDrvMJCModeChgJudgment(UINT32 u4Path)
{
    MJC_FRC_CONFIG_T arMJCFRC = {SV_FALSE};
    MJC_PIC_SIZE_T arMJCPicSize = {SV_FALSE};
    UINT32 fgModeChg = SV_FALSE;

    if ((u4Path != SV_VP_MAIN) || (_fgIsInitialized == SV_FALSE) || (IO32ReadFldAlign(MJC_MISC_00, MJC_MODE_CHG_OFF)))
    {
        return FALSE;
    }

    if (!u1DrvMJCGetVrmInfo(SV_VP_MAIN))
    {
         return FALSE;
    }

    if (fgDrvMJCGetFRCConfig(&arMJCFRC) || IO32ReadFldAlign(MJC_MISC_00, MJC_FLMR_CHG_TRIG))
    {
        vIO32WriteFldAlign(MJC_MISC_00, 0, MJC_FLMR_CHG_TRIG);
        vIO32WriteFldAlign(MJC_FW_SCH_04, ((arMJCPrm.arFRCConfig.u1UniCadenceEn != arMJCFRC.u1UniCadenceEn) &&
                                           (arMJCPrm.arFRCConfig.u1InFrmRate == arMJCFRC.u1InFrmRate)), MJC_UNICADENCE_CHA_CLK_FLG);
        arMJCPrm.arFRCConfig = arMJCFRC;
        fgMJCChangeFrmSch = SV_TRUE;
        fgModeChg = SV_TRUE;
    }

    if (fgDrvMJCGetPicSize(&arMJCPicSize))
    {
        arMJCPrm.arPicSize = arMJCPicSize;
        fgMJCChangePicSize = SV_TRUE;
        fgModeChg = SV_TRUE;
    }

    vDrvMFDModeChgDone(SV_VP_MAIN);

    return fgModeChg;
}

void vDrvMJCModeChgProc(void)
{
    if (fgMJCChangeFrmSch || fgMJCChangePicSize)
    {
        if (fgMJCChangeFrmSch)
        {
            vDrvMJCChangeFrmSch();
        }
        if (fgMJCChangePicSize)
        {
            vDrvMJCChangePicSize();
        }

        vDrvMJCUpdateFrameTrack();
        fgMJCChangeFrmSch = SV_FALSE;
        fgMJCChangePicSize = SV_FALSE;
    }

    if (fgMJCModeChg)
    {
        vIO32WriteFldAlign(MJC_FW_SCH_03, SV_OFF, MJC_UNICADENCE_EN);
        if (fgDrvMJCModeChgJudgment(SV_VP_MAIN))
        {
            if ((arMJCPrm.u1Dram444 == SV_FALSE) || (IO32ReadFldAlign(MJC_FB_01, MJC_DRAM_RPT_422)) || (((arMJCDramAlloc.u2Mode & MJC_422RPT_MODE)? SV_ON : SV_OFF) != IO32ReadFldAlign(MJC_FB_01, MJC_DRAM_RPT_422)))
            {
                _vDrvVideoSetMute(MUTE_MODULE_MJC, SV_VP_MAIN, IO32ReadFldAlign(MJC_MISC_00, MODE_CHG_MUTE_COUNT), SV_TRUE);
            }
        }
        u1TrigSetUniCandeceTrg = (((u1DrvMJCIsSupportUniCandece() == SV_TRUE) && (arMJCPrm.u1Dram444 == SV_FALSE))? IO32ReadFldAlign(MJC_FW_SCH_08, MJC_UNICADENCE_TRIGER_CNT) : 0);
        fgMJCModeChg = SV_FALSE;
    }
}

void vDrvMJCChangeFrmSch(void)
{
    if ((arMJCPrm.arFRCConfig.u13DModeOut == E_VD0_MJC_OUT_FS) && (arMJCPrm.arMJCPRConfig.u1MJCOSMPHPRMode != E_MJC_HPR2160))
    {
        vDrvMJCSetSG3D(&(arMJCPrm.arFRCConfig));
    }
    else
    {
        vDrvMJCSetOSMP(&(arMJCPrm.arFRCConfig));
    }

    vDrvMJCSetPSAutoOnOff(SV_OFF);
    vDrvMJCSetFrmSch(&(arMJCPrm.arFRCConfig));

    if (arMJCPrm.u1Dram444 == SV_FALSE)
    {
        vDrvMJCSetFrmschUniCandeceProc(&(arMJCPrm.arFRCConfig));
    }

    vDrvMJCSetProgME(arMJCPrm.arFRCConfig.u1PanelType, arMJCPrm.arFRCConfig.u1FrmSchMode == FALLBACK_MODE_55P2);
    vIO32WriteFldAlign(MJC_FBCK_13, 1, FBCK_THR_TRIG);
    vDrvMJCSetPsAutoTrigOn();
    LOG(2, "vDrvMJCChangeFrmSch.\n");
}

void vDrvMJCChangePicSize(void)
{
    vDrvMJCSetPicSize(&(arMJCPrm.arPicSize));
    fgOutputTimingChange = SV_ON;
    LOG(2, "vDrvMJCChangePicSize.\n");
}

void vDrvMJCModeChangeDone(UINT32 u4Path)
{
    if (u4Path == SV_VP_MAIN)
    {
		//VDP_RTNeedSuperGameMode will return true when WFD.
		MJC_SetOnOff(!VDP_RTNeedSuperGameMode(u4Path));
		
        vIO32WriteFldAlign(MJC_SYS_MUTE_0, 0, MJC_MUTE_1TYPE);
        LOG(2, "vDrvMJCModeChangeDone\n");

        if (fgCSSOnOff)
        {
            vDrvCSSInitialize();
        }
        fgMJCModeChg = SV_TRUE;
        fgVdoModeChg = SV_TRUE;
    }
}

void vDrvMJCSetModeChangeTrig(void)
{
    fgMJCModeChg = SV_TRUE;
    LOG(2, "vDrvMJCSetModeChangeTrig\n");
}

void vDrvMJCSetOutputTiming(const MJC_PIC_SIZE_T * arMjcPicSize)
{
    vIO32WriteFldMulti(MJC_SYS_03,
        P_Fld(arMjcPicSize->u2OutHStart, MJC_OUT_H_START) |
        P_Fld(arMjcPicSize->u2OutVStart, MJC_OUT_V_START));
    vIO32WriteFldMulti(MJC_SYS_04,
        P_Fld(arMjcPicSize->u2OutWidth, MJC_OUT_WIDTH) |
        P_Fld(arMjcPicSize->u2OutHeight, MJC_OUT_HEIGHT));
    //vIO32WriteFldMulti(MJC_SYS_05,
       // P_Fld(((arMjcPicSize->u2OutHTotal) & 0xFFF), MJC_OUT_H_TOTAL) |
       // P_Fld(arMjcPicSize->u2OutVTotal, MJC_OUT_V_TOTAL));
   // vIO32WriteFldAlign(MJC_SYS_H_MSB,
       // ((arMjcPicSize->u2OutHTotal & 0x1000) >> 12), MJC_OUT_H_TOTAL_MSB);
    vIO32WriteFldAlign(MJC_SYS_CLK_CTRL, SV_ON, MJC_OUTPUT_RST);
    vIO32WriteFldAlign(MJC_SYS_CLK_CTRL, SV_OFF, MJC_OUTPUT_RST);
}


void vDrvMJCSetVsyncDelay(UINT32 u4FrontPorch, UINT32 u4VTotal)
{
    vIO32WriteFldAlign(MJC_SYS_DISP, u4VTotal - u4FrontPorch, MJC_DISP_VSYNC_DLY);
}

void vDrvMJCSetOutputHTotal(UINT32 u4HTotal)
{
    vIO32WriteFldAlign(MJC_SYS_05,
        (u4HTotal & 0xFFF), MJC_OUT_H_TOTAL);
    vIO32WriteFldAlign(MJC_SYS_H_MSB,
        ((u4HTotal & 0x1000) >> 12), MJC_OUT_H_TOTAL_MSB);
}

void vDrvMJCSetOutputVTotal(UINT32 u4VTotal)
{
    vIO32WriteFldAlign(MJC_SYS_05, u4VTotal, MJC_OUT_V_TOTAL);
}

UINT16 u2DrvMJCGetOutputHTotal(void)
{
    return IO32ReadFldAlign(MJC_SYS_05, MJC_OUT_H_TOTAL);
}

UINT16 u2DrvMJCGetOutputVTotal(void)
{
    return IO32ReadFldAlign(MJC_SYS_05, MJC_OUT_V_TOTAL);
}

//UINT32 u4MJCBaseAddress = (UINT32)NULL;

UINT32 u4DrvMJCBufferInit(void)
{
    #ifndef CC_FPGA
    FBM_POOL_T * prPool;
    UINT32 pu4Data;
    prPool = FBM_GetPoolInfoAuto((UCHAR)FBM_POOL_TYPE_MJC , NULL);
    if (prPool == NULL)
    {
        LOG(0, "u4DrvMJCBufferInit: Fail to get pool\n");
        return SV_FAIL;
    }
    if (prPool->u4Size == 0)
    {
        return SV_FAIL;
    }

    if (DRVCUST_OptQuery(eTotalMemSize, &pu4Data) == 0)
    {
        LOG(0, "u4DrvMJCBufferInit: prPool->u4Addr:%x , pu4Data:%x\n" , prPool->u4Addr ,pu4Data);
        vIO32WriteFldAlign(MJC_FB_21, ((prPool->u4Addr < pu4Data)? 1 : 0) ,MJC_C_DA_PG_SIZE);// 1.4k for channel a,  0. 2k for channel b
    }

    arMJCDramAlloc.u4StartAddr = prPool->u4Addr;
    arMJCDramAlloc.u4EndAddr   = prPool->u4Addr + prPool->u4Size;
    arMJCDramAlloc.u4Width     = ALIGN_16_CEIL(prPool->u4Width);
    arMJCDramAlloc.u4Height    = ALIGN_8_CEIL(prPool->u4Height);
    arMJCDramAlloc.u2InitMode  = u4DrvMJCRemapMJCFbmMode(prPool->u4Mode);
    arMJCDramAlloc.u2DebugMode = 0xFFFF;

    #if MJC_SET_BUF_FULL
    arMJCDramAlloc.u2InitMode  = MJC_Y10_C10_422_0RDC_3FRM;
    #endif

    vDrvMJCSetBuf(&arMJCDramAlloc);
    #endif

    return SV_SUCCESS;
}

UINT8 u1DrvMJCGet3DInfoChg(UINT32 u4SramMode)
{
    MJC_VRM_SRM_CONFIG_T arMJC3D ;
    UINT8 u1TDTV3DModeIn ;
    UINT8 u1TDTV3DModeOut ;
    UINT8 fgMJC3DModeChg = SV_OFF;

    if (IO32ReadFldAlign(MJC_MISC_00, MJC_PR_MODE_CHG_OFF) == SV_OFF)
    {
        u1TDTV3DModeIn = TD_MJC_IN(u4SramMode);
        u1TDTV3DModeOut = TD_MJC_OUT(u4SramMode);

        arMJC3D.u1MJC3DModeIn = (u1TDTV3DModeIn == E_TD_IN_LI_P) ? E_VDO_MJC_LI :
                                (u1TDTV3DModeIn == E_TD_IN_TB_P) ? E_VDO_MJC_TAB :
                                (u1TDTV3DModeIn == E_TD_IN_SBS_P) ? E_VDO_MJC_SBS :
                                (u1TDTV3DModeIn == E_TD_IN_FS_P) ? E_VDO_MJC_FS :
                                (u1TDTV3DModeIn == E_TD_IN_2D_P) ? E_VDO_MJC_2D : E_VDO_MJC_OTHER ;

        if(u1TDTV3DModeIn == E_VDO_MJC_OTHER)
        {
            LOG(3,"vDrvMJCSrmCb get wrong mjc input 3d format ->arMJC3D.u1MJC3DModeIn:%d\n", arMJC3D.u1MJC3DModeIn);
            return SV_FALSE;
        }

        arMJC3D.u1MJC3DModeOut = (u1TDTV3DModeOut == E_TD_OUT_3D_FPR) ? E_VD0_MJC_OUT_FPR :
                               (u1TDTV3DModeOut == E_TD_OUT_3D_LI) ? E_VD0_MJC_OUT_HPR :
                               (u1TDTV3DModeOut == E_TD_OUT_3D_FS) ? E_VD0_MJC_OUT_FS :
                               (u1TDTV3DModeOut == E_TD_OUT_NATIVE) ? E_VDO_MJC_OUT_2D : E_VD0_MJC_OUT_OTHER;

        if(arMJC3D.u1MJC3DModeOut == E_VD0_MJC_OUT_OTHER)
        {
            LOG(3,"vDrvMJCSrmCb get wrong mjc output 3d format ->arMJC3D.u1MJC3DModeOut:%d\n", arMJC3D.u1MJC3DModeOut);
            return SV_FALSE;
        }

        if ((arMJC3D.u1MJC3DModeIn == E_VDO_MJC_2D) && (arMJC3D.u1MJC3DModeOut != E_VDO_MJC_OUT_2D))
        {
            arMJC3D.u1MJC3DModeIn = E_VDO_MJC_SG_OSMP_2D;
        }

        if(arMJC3D.u1MJC3DModeIn != arMJCPrm.arVrmSrmConfig.u1MJC3DModeIn)
        {
            arMJCPrm.arVrmSrmConfig.u1MJC3DModeIn = arMJC3D.u1MJC3DModeIn;
            fgMJC3DModeChg = SV_ON;
            fgMJCModeChg = SV_TRUE;
        }

        if(arMJC3D.u1MJC3DModeOut != arMJCPrm.arVrmSrmConfig.u1MJC3DModeOut)
        {
            arMJCPrm.arVrmSrmConfig.u1MJC3DModeOut = arMJC3D.u1MJC3DModeOut;
            fgMJC3DModeChg = SV_ON;
            fgMJCModeChg = SV_TRUE;
        }
        arMJCPrm.arMJCPRConfig.u1MJCOSMPHPRMode = (((IS_PANEL_L12R12) && (arMJC3D.u1MJC3DModeOut == E_VD0_MJC_OUT_HPR))? E_MJC_HPR2160 : E_MJC_HPR_NM);
    }

    return ((fgMJC3DModeChg == SV_ON)? SV_TRUE : SV_FALSE);
}

void vDrvMJCSrmCb(UINT32 u4VdpId, UINT32 u4Mode, UINT32 u4PDSize, UINT32 u4SramMode)
{
    UINT32 u4MuteDelay    = 0;
    UINT32 u4MJCDramMode  = FRC_Y10_C10_444;

    if (_fgIsInitialized == FALSE)
    {
        return;
    }

    if ((u4Mode & 0x7FFF8000) == FBM_POOL_MODE_UNSTABLE && u1DrvMJCGet3DInfoChg(u4SramMode) == SV_FALSE)
    {
        return;
    }

    if (u1DrvMJCGet3DInfoChg(u4SramMode) == SV_TRUE)
    {
        LOG(2,"vDrvMJCSrmCb mjc  3d format mode change;\n");
    }

    if (IO32ReadFldAlign(MJC_MISC_00, SRM_CB_OFF))
    {
        return;
    }

    #if !(MJC_FORCE_RPT_MODE)
    u4MJCDramMode  = u4DrvMJCRemapMJCFbmMode(u4Mode);
    #endif

    if ((arMJCDramAlloc.u2DebugMode != 0xFFFF) || (arMJCDramAlloc.u2Mode != u4MJCDramMode))
    {
        arMJCDramAlloc.u2Mode      = u4MJCDramMode;
        arMJCDramAlloc.u2DebugMode = 0xFFFF;
        u4MuteDelay = IO32ReadFldAlign(MJC_MISC_00, MJC_MUTE_COUNT);

        LOG(2, "vDrvMJCSrmCb: 0x%x \n", arMJCDramAlloc.u2Mode);

        if (u1DrvMJCIsOnOff())
        {
            if (IO32ReadFldAlign(MJC_MISC_00, MJC_SRM_CB_MUTE) == SV_OFF)
            {
                vDrvMJCSetSystemModeProc();
            }
            else
            {
                _vDrvVideoSetMute(MUTE_MODULE_MJC, SV_VP_MAIN, u4MuteDelay, SV_TRUE);
                vIO32WriteFldAlign(MJC_MISC_00, 0x2, MJC_SRM_CB_TRIG);
            }
        }
        else
        {
            vDrvMJCSetSystemMode(arMJCDramAlloc.u2Mode);
            fgMJCModeChg = SV_TRUE;
        }
    }

    #ifdef CC_FLIP_MIRROR_SUPPORT
    if(u4GetFlipMirrorModule(VDP_1) == FLIP_BY_MJC)
    {
        vDrvMJCSetFlipMirror(u1GetFlipMirrorConfig() & SYS_FLIP_CONFIG_ON,
        ((u1GetFlipMirrorConfig() & SYS_MIRROR_CONFIG_ON) >> 1));
    }
    #endif
}

void vDrvMJCGetSPReg(void)
{
    UINT32 u4Count;

    for (u4Count = 0; u4Count < 8; u4Count++)
    {
        Printf("(%d) 0x%08X[%02d:%02d]\n", u4Count, arMJCSPReg[u4Count].u4Addr, arMJCSPReg[u4Count].u1Msb, arMJCSPReg[u4Count].u1Lsb);
    }
}

UINT32 u4DrvMJCSetSPReg(UINT32 u4Index, UINT32 u4Addr, UINT32 u4Msb, UINT32 u4Lsb)
{
    if (u4Index >= 8)
    {
        return SV_FALSE;
    }

    arMJCSPReg[u4Index].u4Addr = u4Addr;
    arMJCSPReg[u4Index].u1Msb  = u4Msb;
    arMJCSPReg[u4Index].u1Lsb  = u4Lsb;

    return SV_TRUE;
}

void vDrvMJCRegStatusPolling(void *pvArgv)
{
    UINT32 u4Status[8] = {0};
    UINT32 u4Count = 0;
    
    while ((u4Count < 8) && (arMJCSPReg[u4Count].u4Addr != 0))
    {
        if ((arMJCSPReg[u4Count].u4Addr & 0xFFFFF000) == 0x20006000)
        {
            u4Status[u4Count] = IO32ReadFldAlign(arMJCSPReg[u4Count].u4Addr&0x0000FFFF, Fld((arMJCSPReg[u4Count].u1Msb - arMJCSPReg[u4Count].u1Lsb + 1), arMJCSPReg[u4Count].u1Lsb, AC_MSKDW));
        }
        else if (arMJCSPReg[u4Count].u4Addr < 0x10000)
        {
            u4Status[u4Count] = IO32ReadFldAlign(u4SWReg(arMJCSPReg[u4Count].u4Addr), Fld((arMJCSPReg[u4Count].u1Msb - arMJCSPReg[u4Count].u1Lsb + 1), arMJCSPReg[u4Count].u1Lsb, AC_MSKDW));
        }
        else
        {
            u4Status[u4Count] = IO32ReadFldAlign(arMJCSPReg[u4Count].u4Addr, Fld((arMJCSPReg[u4Count].u1Msb - arMJCSPReg[u4Count].u1Lsb + 1), arMJCSPReg[u4Count].u1Lsb, AC_MSKDW));
        }
        u4Count++;
    }
    Printf("%08X %08X %08X %08X %08X %08X %08X %08X\n",
        u4Status[0], u4Status[1], u4Status[2], u4Status[3],
        u4Status[4], u4Status[5], u4Status[6], u4Status[7]);
}

void vDrvMJCSetDramStaMonitorCount(UINT32 u4Val)
{
    _u4DramStaMonitorCount = u4Val;
}

UINT32 u4DrvMJCGetDramStaMonitorCount(void)
{
    return _u4DramStaMonitorCount;
}

void vDrvMJCClearDramStatus(void)
{
    // clear
    vIO32WriteFldAlign(MJC_FB_01, 1, MJC_DRAM_STATUS_CLR);
    vIO32WriteFldAlign(MJC_SYS_00, 1, MJC_RD_UNDER_RST);
    vIO32WriteFldAlign(MJC_FB_01, 0, MJC_DRAM_STATUS_CLR);
    vIO32WriteFldAlign(MJC_SYS_00, 0, MJC_RD_UNDER_RST);
}

UINT8 u1DrvMJCChkDramOvfUdf(UINT32 * pu4StaFB01, UINT32 * pu4StaFB02, UINT32 * pu4StaSys00)
{
    UINT8 u1Ret = 0;
    UINT32 u4Msk42X_0, u4Msk42X_1, u4Msk42X_2;
    UINT32 u4Sta0, u4Sta1, u4Sta2;

    if (!arMJCPrm.u1Dram444)
    {
        u4Msk42X_0 = Fld2Msk32(MJC_STA_WADDR_OOB)|
            Fld2Msk32(MJC_STA_WFIFO_OVF);
        // u4Msk42X_1 = Fld2Msk32(MJC_STA_FILM_WOVF)|Fld2Msk32(MJC_STA_FILM_WUDF)|
        //    Fld2Msk32(MJC_STA_FILM_ROVF)|Fld2Msk32(MJC_STA_FILM_RUDF);                                 // not use when film detect by pscan
        u4Msk42X_1 = 0;
        u4Msk42X_2 = Fld2Msk32(MJC_STA_INIT_UNDER)|Fld2Msk32(MJC_STA_PROC_UNDER);
    }
    else
    {

        u4Msk42X_0 = Fld2Msk32(MJC_STA_WADDR_OOB);
        u4Msk42X_1 = Fld2Msk32(MJC_STA_RPT_MB_UDF) | Fld2Msk32(MJC_STA_RPT_QMB_UDF);
        u4Msk42X_2 = 0;
    }

    u4Sta0 = u4IO32Read4B(MJC_STA_FB_01) & u4Msk42X_0;
    if (u4Sta0)
    {
        u1Ret ++;
    }

    if (pu4StaFB01)
        (*pu4StaFB01) = u4Sta0;

    u4Sta1 = (!arMJCPrm.u1Dram444)? (u4IO32Read4B(MJC_STA_FB_02) & u4Msk42X_1)  :  (u4IO32Read4B(MJC_FB_05) & u4Msk42X_2);
    if (u4Sta1)
    {
        u1Ret ++;
    }

    if (pu4StaFB02)
        (*pu4StaFB02) = u4Sta1;

    u4Sta2 = u4IO32Read4B(MJC_STA_SYS_00) & u4Msk42X_2;
    if (u4Sta2)
    {
        u1Ret ++;
    }

    if (pu4StaSys00)
        (*pu4StaSys00) = u4Sta2;

    return u1Ret;
}

void vDrvMJCSetMVSortOnOff(UINT32 u4OnOff)
{
    vIO32WriteFldAlign(MJC_MV_00, u4OnOff, MJC_MV_SORT_EN);
}

void vDrvMJCInputLRCheck(void *pvArgv)
{
    static UINT8 fg;
    static UINT8 u1MJCLR_IN1, u1MJCLR_IN2;
    if((u1MJCStatusCheck == 1) || (u1MJCStatusCheck == 6))
    {
        u1MJCLR_IN1 = u1MJCLR_IN2;
        u1MJCLR_IN2 = IO32ReadFldAlign(MJC_STA_LR3D, MJC_STA_LR3D_INFO_I);
        if(((u1MJCLR_IN1 + u1MJCLR_IN2) != 1) && (fg == 0))
        {
            printf("LR ERROR!\n");
            fg = 1;

        }
        if(fg)
        {
            if(fg == 8)
            {
                u1MJCStatusCheck = 0;
            }
            fg++;
        }
        Printf("u1MJCLR_IN:%d \n ", u1MJCLR_IN2);
        if(u1MJCStatusCheck == 6)
        {
            u1MJCStatusCheck = 0;
            printf("MJC Output FS LR Check interrupt \n");
        }
    }
    if(u1MJCStatusCheck == 2)
    {
        u1MJCLR_IN2 = IO32ReadFldAlign(MJC_STA_LR3D, MJC_STA_LR3D_INFO_I);
        u1MJCStatusCheck = 1;
        Printf("u1MJCLR_IN:%d \n ", u1MJCLR_IN2);
    }
    if(u1MJCStatusCheck == 3)
    {
        u1MJCLR_IN1 = IO32ReadFldAlign(MJC_STA_LR3D, MJC_STA_LR3D_INFO_I);
        u1MJCStatusCheck = 2;
        fg = 0;
        Printf("u1MJCLR_IN:%d \n ", u1MJCLR_IN1);
    }
}

void vDrvMJCInOutCRCCheck(void *pvArgv)
{
    static UINT32 _u4MJCCRC_OUT1, _u4MJCCRC_OUT2;
    static UINT32 _u4MJCCRC_IN1, _u4MJCCRC_IN2 ;
    UINT32 u4MJCCRC_OUT = 0;
    UINT32 u4MJCCRC_IN = 0;
    if ((u1MJCStatusCheck == 1) || (u1MJCStatusCheck == 6))
    {
        u4MJCCRC_IN = IO32ReadFldAlign(MJC_STA_CRC_2, MJC_CRC_OUT_1);
        u4MJCCRC_OUT = IO32ReadFldAlign(MJC_STA_CRC_1, MJC_CRC_OUT_0);
        if ((u4MJCCRC_OUT != _u4MJCCRC_OUT1) && (u4MJCCRC_OUT != _u4MJCCRC_OUT2))
        {
            u1MJCStatusCheck = 0;
            printf("MJC Output CRC is Error \n");
        }
        if ((u4MJCCRC_IN != _u4MJCCRC_IN1) && (u4MJCCRC_IN != _u4MJCCRC_IN2))
        {

            u1MJCStatusCheck = 0;
            printf("MJC Output CRC is Error \n");
        }
        if (u1MJCStatusCheck == 6)
        {
            u1MJCStatusCheck = 0;
            printf("MJC Output CRC Check interrupt \n");
        }
    }
    if (u1MJCStatusCheck == 2)
    {
        _u4MJCCRC_OUT2 = IO32ReadFldAlign(MJC_STA_CRC_1, MJC_CRC_OUT_0);
        _u4MJCCRC_IN2 = IO32ReadFldAlign(MJC_STA_CRC_2, MJC_CRC_OUT_1);
        u1MJCStatusCheck = 1;
    }
    if (u1MJCStatusCheck == 3)
    {
        _u4MJCCRC_OUT1 = IO32ReadFldAlign(MJC_STA_CRC_1, MJC_CRC_OUT_0);
        _u4MJCCRC_IN1 = IO32ReadFldAlign(MJC_STA_CRC_2, MJC_CRC_OUT_1);
        u1MJCStatusCheck = 2;
    }

    Printf("u1MJCRC_IN:%d      u4MJCCRC_OUT:%d  \n", u4MJCCRC_IN , u4MJCCRC_OUT);
}

void vDrvMJCDramOvfUdfPolling(void *pvArgv)
{
    UINT32 u4StaFB01, u4StaFB02, u4StaSYS00;

    if (u1DrvMJCChkDramOvfUdf(&u4StaFB01, &u4StaFB02, &u4StaSYS00))
    {
        Printf("\nvDrvMJCDramOvfUdfPolling is stopped! There are bandwidth issues!\n");
        Printf("Remaining Polling Count = %d\n", u4MJCStatusPollingCount);
        Printf("Current DRAM %s Mode\n", (arMJCPrm.u1Dram444 ? "444" : "42x"));
        if (u4StaSYS00 & Fld2Msk32(MJC_STA_INIT_UNDER))
        {
            Printf("Issue Flag: MJC_STA_INIT_UNDER\n");
        }
        if (u4StaSYS00 & Fld2Msk32(MJC_STA_PROC_UNDER))
        {
            Printf("Issue Flag: MJC_STA_PROC_UNDER\n");
        }

        if (u4StaFB01)
        {
            Printf("Issue Flag: STA_FB_01 = 0x%08X\n", u4StaFB01);
        }
        if (u4StaFB02)
        {
            Printf("Issue Flag: STA_FB_02(42X) / MJC_FB_05(444) = 0x%08X\n", u4StaFB02);
        }


        u4MJCStatusPollingCount = 1;
        return;
    }

    if ((u4MJCStatusPollingCount & 0x1F) == 0)
    {
        Printf(".");
    }

    if (u4MJCStatusPollingCount == 1)
    {
        Printf("\nvDrvMJCDramOvfUdfPolling finished. No Ovf/Udf is found\n");
        Printf("Current DRAM %s Mode\n", (arMJCPrm.u1Dram444 ? "444" : "42x"));
        vDrvMJCSetMVSortOnOff(SV_ON);
    }
}

UINT32 u4DrvMJCGetVDist(void)
{
    return IO32ReadFldAlign(MJC_STA_FM_01, MJC_STA_FRM_VSYNC_IN_EARLY);
}

void vDrvMJCSetMuteColor(UINT8 u1R, UINT8 u1G, UINT8 u1B)
{
    vIO32WriteFldMulti(MJC_SYS_MUTE_0,
        P_Fld(u1R, MJC_MUTE_R)|P_Fld(u1G, MJC_MUTE_G)|P_Fld(u1B, MJC_MUTE_B));
}

void vDrvMJCSetOutMuteOnOff(BOOL fgOnOff)
{
    LOG(3, "MJC Output Data Mute: %d\n", fgOnOff);

    if (fgOnOff)
    {
        vIO32WriteFldAlign(MJC_SYS_MUTE_0, 2, MJC_MUTE_1TYPE);
    }
    else
    {
        vIO32WriteFldAlign(MJC_SYS_MUTE_0, 0, MJC_MUTE_1TYPE);
    }

    vIO32WriteFldAlign(MJC_SYS_MUTE_1, SV_OFF, MJC_MUTE_SET);
    vIO32WriteFldAlign(MJC_SYS_MUTE_1, SV_ON, MJC_MUTE_SET);
}

void vDrvMJCSetAutoMute(UINT8 u1Count)
{
    vDrvMJCSetOutMuteOnOff(SV_ON);
    vDrvMJCDelaySet(E_MJC_DELAY_UNMUTE, u1Count);
    x_thread_delay(20);
}

void vDrvMJCEnablePatGen(UINT32 u4InOut, UINT32 u4OnOff)
{
    if (u4InOut == 0)
    {
        vIO32WriteFldAlign(MJC_SYS_0A, arMJCPrm.arFRCConfig.u13DModeIn == E_VDO_MJC_FS ? 1 : 0, MJC_PTG_3D_MODE);
        vIO32WriteFldAlign(MJC_SYS_0F, u4OnOff, MJC_PTG_PAT_EN);
        // set up bnd ptg
        vIO32WriteFldMulti(MJC_SYS_0A,
        P_Fld(SV_ON, MJC_PTG_BND_EN) |
        P_Fld(IO32ReadFldAlign(MJC_SYS_04, MJC_OUT_WIDTH), MJC_PTG_BND_W) |
        P_Fld(((arMJCPrm.arFRCConfig.u13DModeOut == E_VD0_MJC_OUT_HPR) || (arMJCPrm.arFRCConfig.u13DModeOut == E_VD0_MJC_OUT_FPR))?
               (IO32ReadFldAlign(MJC_SYS_04, MJC_OUT_HEIGHT) >> 1) : IO32ReadFldAlign(MJC_SYS_04, MJC_OUT_HEIGHT), MJC_PTG_BND_H));

        vIO32WriteFldAlign(MJC_SYS_0F, SV_ON, MJC_PTG_FREERUN_EN);
        vIO32WriteFldAlign(MJC_SYS_0F, (arMJCPrm.u1Mirror == SV_ON)? 0x8 : 0x7, MJC_PTG_FREERUN_SPEED);
        vIO32WriteFldAlign(MJC_SYS_0F, 0x5, MJC_PTG_ANGLE_SEL);

        vIO32WriteFldAlign(MJC_SYS_0F, ((arMJCPrm.u1Flip == SV_ON) != (arMJCPrm.u1Mirror == SV_ON)), MJC_PTG_LEFT_SIDE);

        vIO32WriteFldAlign(MJC_FM_00, !u4OnOff, MJC_PSAUTO_22);
        vIO32WriteFldAlign(MJC_FM_00, !u4OnOff, MJC_PSAUTO_32);
    }
    else if (u4InOut == 1)
    {
        vIO32WriteFldAlign(MJC_SYS_0A, arMJCPrm.arFRCConfig.u13DModeIn != E_VDO_MJC_2D ? (arMJCPrm.arFRCConfig.u1OutFrmRate >= 192 ? 2 : 1) : 0, MJC_PTG_3D_MODE);
        vIO32WriteFldAlign(MJC_SYS_0E, u4OnOff, MJC_PTG2_PAT_EN);
        // set up bnd ptg
        vIO32WriteFldMulti(MJC_SYS_0A,
        P_Fld(SV_ON, MJC_PTG_BND_EN) |
        P_Fld(IO32ReadFldAlign(MJC_SYS_04, MJC_OUT_WIDTH), MJC_PTG_BND_W) |
        P_Fld(IO32ReadFldAlign(MJC_SYS_04, MJC_OUT_HEIGHT), MJC_PTG_BND_H));

        vIO32WriteFldAlign(MJC_SYS_0E, SV_ON, MJC_PTG2_FREERUN_EN);
        vIO32WriteFldAlign(MJC_SYS_0E, 0x5, MJC_PTG2_FREERUN_SPEED);
        vIO32WriteFldAlign(MJC_SYS_0E, 0x6, MJC_PTG2_ANGLE_SEL);
        vIO32WriteFldAlign(MJC_SYS_0E, 0x5, MJC_PTG2_FREQ_SEL);
        vIO32WriteFldAlign(MJC_SYS_0E, 0x5, MJC_PTG2_YPAT1);
        vIO32WriteFldAlign(MJC_SYS_0E, 0x9, MJC_PTG2_YPAT1);
    }
    else
    {
        LOG(0, "Wrong MJC PTG Setting!\n");
    }

    if ((arMJCPrm.u1Dram444 == SV_TRUE) &&( arMJCPrm.u1Dram422RPT == SV_FALSE))
    {
        vIO32WriteFldAlign(MJC_SYS_00, !u4OnOff, MJC_BY_IN_TRANSFORM);
        vIO32WriteFldAlign(MJC_SYS_00, !u4OnOff, MJC_BY_OUT_TRANSFORM);
    }
}

static UINT32 _u4PrevCRC0 = 0;
static UINT32 _u4PrevCRC1 = 0;
void vDrvMJCDelaySet(E_MJC_DELAY_SET eDelayType, UINT8 u1Count)
{
    UINT32 u4DelayType = eDelayType;

    _u1DelaySetCnt[eDelayType] = u1Count;

    switch (u4DelayType)
    {
    case E_MJC_DELAY_DM_BW_CHK:
        vDrvMJCClearDramStatus();
        break;
    case E_MJC_DELAY_DM_CRC_CHK:
        _u4PrevCRC0 = IO32ReadFldAlign(MJC_STA_CRC_1, MJC_CRC_OUT_0);
        _u4PrevCRC1 = IO32ReadFldAlign(MJC_STA_CRC_2, MJC_CRC_OUT_1);
        break;
    }
}

void vDrvMJCDelaySetProc(void)
{
    if (_u1DelaySetCnt[E_MJC_DELAY_UNMUTE])
    {
        #if 0
        if (u1DrvMJCChkDramOvfUdf(NULL, NULL))
        {
            vDrvMJCClearDramStatus();
        }
        else
        #endif
        {
            _u1DelaySetCnt[E_MJC_DELAY_UNMUTE]--;
            if (_u1DelaySetCnt[E_MJC_DELAY_UNMUTE] == 0)
                vDrvMJCSetOutMuteOnOff(SV_OFF);
        }
    }
}

UINT8 u1DrvMJCGetMuteCount(void)
{
    return _u1DelaySetCnt[E_MJC_DELAY_UNMUTE];
}

void vDrvMJCSetMuteCount(UINT32 u4MuteCount)
{
    _u1DelaySetCnt[E_MJC_DELAY_UNMUTE] = (UINT8)u4MuteCount;

    if (u4MuteCount == 0)
    {
        vDrvMJCSetOutMuteOnOff(SV_OFF);
    }
    else
    {
        vDrvMJCSetOutMuteOnOff(SV_ON);
        u1TestDelayCnt = (UINT8)u4MuteCount;
    }
}

void vDrvMJCSetDemoCfg(const MJC_DEMO_CFG_T* prDemo)
{
    arMJCDemo = (*prDemo);
    vDrvMJCUpdateIntpCtrl();
}

void vDrvMJCForceUniCandeceModeChg(void)
{
    if (bDrvVideoSignalStatus(SV_VP_MAIN) == SV_VDO_STABLE)
    {
        if (fgVdoModeChg)
        {
            fgVdoModeChg = SV_FALSE;
        }
        else if (IO32ReadFldAlign(MJC_FW_SCH_04,  MJC_UNICADENCE_CHA_CLK_FLG) == SV_ON)
        {
            vIO32WriteFldAlign(MJC_FW_SCH_04, SV_OFF , MJC_UNICADENCE_CHA_CLK_FLG);
            //vSetMainFlg(MAIN_FLG_MODE_CHG);
            //vSetMainFlg(MAIN_FLG_MODE_DET_DONE);
            fgMJCModeChg = SV_TRUE;
            vDrvSetMJCUniCandeceModeChangeFlg(SV_ON);
            LOG(2,"vDrvMJCForce1UniCandeceModeChg: mjc trigger vSetMainFlg\n");
        }
    }

    if ((u1TrigSetUniCandeceTrg ) && (bDrvVideoSignalStatus(SV_VP_MAIN) == SV_VDO_STABLE))
    {
        u1TrigSetUniCandeceTrg --;
        if (u1TrigSetUniCandeceTrg == 0)
        {
            vIO32WriteFldAlign(MJC_FW_SCH_03, arMJCPrm.arFRCConfig.u1UniCadenceEn, MJC_UNICADENCE_EN);
            u1TrigSetUniCandeceTrg = 0;
            LOG(2,"vDrvMJCForce1UniCandeceModeChg: Set MJC_FW_SCH MJC_MJC_UNICADENCE_EN\n");
        }
    }
}


void vDrvMJCMainloop(void)
{
    vDrvMJCModeChgProc();

    vDrvMJCSet3DLRDly();

    //vDrvMJCSet32PdInfoShiftJudgment();

    vDrvMJCForceUniCandeceModeChg();

    if (IO32ReadFldAlign(MJC_MISC_00, MJC_SRM_CB_TRIG) != 0)
    {
        if (IO32ReadFldAlign(MJC_MISC_00, MJC_SRM_CB_TRIG) == 1)
        {
            vDrvMJCSetSystemModeProc();
            vIO32WriteFldAlign(MJC_MISC_00, 0, MJC_SRM_CB_TRIG);
        }
        else
        {
            vIO32WriteFldAlign(MJC_MISC_00, 1, MJC_SRM_CB_TRIG);
        }
    }

    vDrvMJCDelaySetProc();

    vDrvMJCFbckModeChg();

    vDrvMJCDIRProc();
    vDrvMJCAdapPPCLbox();
}

void vDrvSetMJCUniCandeceModeChangeFlg(UINT8 u1OnOff)
{
    vIO32WriteFldAlign(MJC_FW_SCH_04, u1OnOff , MJC_UNICADENCE_CHA_CLK_TTDFLG);
}

UINT8 u1DrvGetMJCUniCandeceModeChangeFlg(void)
{
    return(IO32ReadFldAlign(MJC_FW_SCH_04,  MJC_UNICADENCE_CHA_CLK_TTDFLG));
}

void vDrvMJCFrmDlyChkOnOff(UINT32 u4OnOff)
{
    fgMJCFrmDlyChk = u4OnOff;
}

void vDrvMJCFrmDlyChk(UINT32 fgInVsync)
{
    static UINT32 u4Index = 0;
    static UINT32 fg1stFrm = SV_FALSE;
    static UINT32 u4Prev1CRCOut0, u4Prev1CRCOut1;
    static UINT32 u4Prev2CRCOut0, u4Prev2CRCOut1;
    UINT32 u4CRCOut0 = 0, u4CRCOut1 = 0;

    if (fgMJCFrmDlyChk == SV_OFF)
    {
        return;
    }

    if (fgInVsync == SV_TRUE)
    {
        if (u4Index == 0)
        {
            vIO32WriteFldMulti(MJC_SYS_0F,
                P_Fld(SV_OFF, MJC_PTG_PAT_EN) |
                P_Fld(SV_OFF, MJC_PTG_FREERUN_EN));
            Printf("\n");
        }
        else if (u4Index == 9)
        {
            vIO32WriteFldAlign(MJC_SYS_0F, SV_ON, MJC_PTG_PAT_EN);
            Printf("==========================\n");
        }
        u4Index ++;
        fg1stFrm = SV_TRUE;
    }
    else
    {
        if (u4Index > 0)
        {
            u4CRCOut0 = u4IO32Read4B(MJC_STA_CRC_1);
            u4CRCOut1 = u4IO32Read4B(MJC_STA_CRC_2);
            Printf("%02d, 0x%08X, 0x%08X\n", u4Index, u4CRCOut0, u4CRCOut1);
        }

        if ((u4Index > 10) && (u4CRCOut0 == u4Prev1CRCOut0) && (u4CRCOut1 == u4Prev1CRCOut1) &&
            (u4CRCOut0 == u4Prev2CRCOut0) && (u4CRCOut1 == u4Prev2CRCOut1))
        {
            Printf("Frame Delay = %d.%d\n", (u4Index - 11), (fg1stFrm == SV_FALSE ? 5 : 0));
            fgMJCFrmDlyChk = SV_OFF;
            u4Index        = 0;
            vIO32WriteFldMulti(MJC_SYS_0F,
                P_Fld(SV_OFF, MJC_PTG_PAT_EN) |
                P_Fld(SV_OFF, MJC_PTG_FREERUN_EN));
        }

        u4Prev2CRCOut0 = u4Prev1CRCOut0;
        u4Prev2CRCOut1 = u4Prev1CRCOut1;
        u4Prev1CRCOut0 = u4CRCOut0;
        u4Prev1CRCOut1 = u4CRCOut1;

        if (fg1stFrm == SV_TRUE)
        {
            fg1stFrm = SV_FALSE;
        }
    }
}

void vDrvMJCDramModeAutoTestCheck(void)
{
    UINT32 u4Crc0 = 0, u4Crc1 = 0;
    UINT32 u4StaFB01 = 0, u4StaFB02 = 0, u4StaSYS00 = 0;

    if (_u1DelaySetCnt[E_MJC_DELAY_DM_START] == 0)
    {
        if (_u1DelaySetCnt[E_MJC_DELAY_DM_CRC_CHK])
        {
            _u1DelaySetCnt[E_MJC_DELAY_DM_CRC_CHK] --;

            u4Crc0 = IO32ReadFldAlign(MJC_STA_CRC_1, MJC_CRC_OUT_0);
            u4Crc1 = IO32ReadFldAlign(MJC_STA_CRC_2, MJC_CRC_OUT_1);

            if ((u4Crc0 != _u4PrevCRC0) || (u4Crc1 != _u4PrevCRC1))
            {
                Printf("Current CRC = (0x%08x, 0x%08x)\n", u4Crc0, u4Crc1);
                Printf("Previous CRC = (0x%08x, 0x%08x)\n", _u4PrevCRC0, _u4PrevCRC1);
                _u1DelaySetCnt[E_MJC_DELAY_DM_CRC_CHK] = 0;
            }
            else
            {
                if (_u1DelaySetCnt[E_MJC_DELAY_DM_CRC_CHK] == 0)
                {
                    Printf("CRC OK.\n");
                }
            }

            _u4PrevCRC0 = u4Crc0;
            _u4PrevCRC1 = u4Crc1;
        }

        if (_u1DelaySetCnt[E_MJC_DELAY_DM_BW_CHK])
        {
            _u1DelaySetCnt[E_MJC_DELAY_DM_BW_CHK] --;
            if (u1DrvMJCChkDramOvfUdf(&u4StaFB01, &u4StaFB02, &u4StaSYS00))
            {
                Printf("=====MJC BW issue: MJC_STA_FB_01 = 0x%08X\n", u4StaFB01);
                Printf("=====MJC BW issue: MJC_STA_FB_02 = 0x%08X\n", u4StaFB02);
                Printf("=====MJC BW issue: MJC_STA_SYS_00 =0x%08X\n", u4StaSYS00);
                Printf("vDrvMJCDramOvfUdfPolling stopped! There are BW issues!\n");
                Printf("Current DRAM %s Mode\n", (arMJCPrm.u1Dram444 ? "444" : "42x"));
                _u1DelaySetCnt[E_MJC_DELAY_DM_BW_CHK] = 0;
            }
            else
            {
                if (_u1DelaySetCnt[E_MJC_DELAY_DM_BW_CHK] == 0)
                {
                    Printf("BW Check OK.\n");
                }
            }
        }
    }
    else
    {
        _u1DelaySetCnt[E_MJC_DELAY_DM_START] --;
    }
}

static void _vDrvMJCBadRstPatch(void)
{
    UINT32 u4FdInfo = IO32ReadFldAlign(MJCFD_08, MJCFD_FD_INFO);

    if ((u4FdInfo >= 1) && (u4FdInfo <= 5))
    {
        vIO32WriteFldAlign(MJCFD_06, 1, MJCFD_C_FD_SEQ_BAD_RST);
    }
    else
    {
        vIO32WriteFldAlign(MJCFD_06, 0, MJCFD_C_FD_SEQ_BAD_RST);
    }
}

static void _vDrvMJCSpAdjInputVsync(void)
{
    if (_pfnMJCAdjInputVsync && u4MJCStatusPollingCount)
    {
        Printf("\n");
        (*_pfnMJCAdjInputVsync)(NULL);

        u4MJCStatusPollingCount --;
        if (u4MJCStatusPollingCount == 0)
        {
            _pfnMJCAdjInputVsync = NULL;
            Printf("\n");
        }
    }
}

static void _vDrvMJCSpInputVsync(void)
{
    if (_pfnMJCInputVsync && u4MJCStatusPollingCount)
    {
        //Printf("\n");
        (*_pfnMJCInputVsync)(NULL);

        u4MJCStatusPollingCount --;
        if (u4MJCStatusPollingCount == 0)
        {
            _pfnMJCInputVsync = NULL;
            Printf("\n");
        }
    }
    if (_pfnMJCInputVsync && u1MJCStatusCheck)
    {
        Printf("\n");
        (*_pfnMJCInputVsync)(NULL);

        if (u1MJCStatusCheck == 0)
        {
            _pfnMJCInputVsync = NULL;
            Printf("\n");
        }
    }
}

static void _vDrvMJCSpOutputVsync(void)
{
    if (_pfnMJCOutputVsync && u4MJCStatusPollingCount)
    {
        //if (_pfnMJCOutputVsync != vDrvMJCDramOvfUdfPolling)
        //    Printf("\n");

        (*_pfnMJCOutputVsync)(NULL);

        u4MJCStatusPollingCount --;
        if (u4MJCStatusPollingCount == 0)
        {
            _pfnMJCOutputVsync = NULL;
            Printf("\n");
        }
    }
    if (_pfnMJCOutputVsync && u1MJCStatusCheck)
    {
        Printf("\n");
        (*_pfnMJCOutputVsync)(NULL);

        if (u1MJCStatusCheck == 0)
        {
            _pfnMJCOutputVsync = NULL;
            Printf("\n");
        }
    }
}

static void _vDrvMJCSpProcFinished(void)
{
    if (_pfnMJCProcFinished && u4MJCStatusPollingCount)
    {
        Printf("\n");
        (*_pfnMJCProcFinished)(NULL);

        u4MJCStatusPollingCount --;
        if (u4MJCStatusPollingCount == 0)
        {
            _pfnMJCProcFinished = NULL;
            Printf("\n");
        }
    }
}

#if 0
static void _vDrvMJCProcFrmFreeze(void)
{
    static UINT8 _u1FrzMode = 0xFF;
    static UINT8 _u1UnFrzCnt = 18;

    if (arMJCPrm.fgFreeze != _u1FrzMode)
    {
        if (arMJCPrm.fgFreeze == SV_ON)
        {
            if (IO32ReadFldAlign(MJC_STA_FM_02, MJC_STA_MEMC_NUM_HW) == 0 &&
                IO32ReadFldAlign(MJC_STA_FM_02, MJC_STA_ME_FILM_FLG_HW) == 0)
            {
                vIO32WriteFldAlign(MJC_MC_01, 0, MJC_MC_INTP_NUM);
                vIO32WriteFldAlign(MJC_SYS_00, 1, MJC_FRM_FREEZE);
                _u1FrzMode = arMJCPrm.fgFreeze;
                _u1UnFrzCnt = 60;
            }
        }
        else if (arMJCPrm.fgFreeze == SV_OFF)
        {
            vIO32WriteFldAlign(MJC_SYS_00, 0, MJC_FRM_FREEZE);
            if (--_u1UnFrzCnt == 0)
            {
                vDrvMJCSetPsAutoTrigOn();
                _u1FrzMode = arMJCPrm.fgFreeze;
            }
        }
    }
}
#endif

void vDrvMJCImportProtection(UINT8 u1OnOff)
{
    /*if (u1OnOff)
    {
        vIO32WriteFldAlign(MJC_SYS_D4, SV_ON, MJC_OUT_TOTAL_MATCH_SET);//[31:24]in 8283
    }
    else
    {
        vIO32WriteFldAlign(MJC_SYS_D4, SV_OFF, MJC_OUT_TOTAL_MATCH_SET);
    }
    */
}

void vDrvMJCFireImportProtection(void)
{
    /*
    vIO32WriteFldAlign(MJC_SYS_D4, SV_ON, MJC_SET_DATA_ONE_FRM);
    vIO32WriteFldAlign(MJC_SYS_D4, SV_OFF, MJC_SET_DATA_ONE_FRM);
    */
}

void vDrvMJCSetInputWidthHeight(UINT32 u4Width, UINT32 u4Height)
{
    arMJCPrm.arPicSize.u2InWidth= (UINT16)u4Width;
    arMJCPrm.arPicSize.u2InHeight = (UINT16)u4Height;
    vIO32WriteFldMulti(MJC_SYS_02,
        P_Fld(u4Width, MJC_MJC_WIDTH) |
        P_Fld(u4Height, MJC_MJC_HEIGHT_FX));
}

void vDrvMJCSetOutputWidthHeight(UINT32 u4Width, UINT32 u4Height)
{
    arMJCPrm.arPicSize.u2OutWidth= (UINT16)u4Width;
    arMJCPrm.arPicSize.u2OutHeight = (UINT16)u4Height;
    vIO32WriteFldMulti(MJC_SYS_04,
        P_Fld(u4Width, MJC_OUT_WIDTH) |
        P_Fld(u4Height, MJC_OUT_HEIGHT));
}

void vDrvMJCSwitchFrameTrackOnOff(UINT32 u4OnOff)
{
    static UINT32 u4PrevOnOffState = 0xFF;

    if (u4PrevOnOffState == u4OnOff)
    {
        return;
    }

    u4PrevOnOffState = u4OnOff;

    if (u4OnOff)
    {
        vIO32WriteFldAlign(MJC_SYS_DDDS, SV_ON, MJC_DDDS_TRK_EN);
    }
    else
    {
        vIO32WriteFldAlign(MJC_SYS_DDDS, SV_OFF, MJC_DDDS_TRK_EN);
    }
}

void vDrvMJCSetFrmMask(UINT32 u4FrmMaskI, UINT32 u4FrmMaskO)
{
    vIO32WriteFldMulti(MJC_FM_TRK,
        P_Fld(u4FrmMaskI, MJC_FRM_MASK_I) |
        P_Fld(u4FrmMaskO, MJC_FRM_MASK_O));
}

UINT8 u1DrvMJCIsOnOff(void)
{
    return arMJCPrm.u1OnOff != E_MJC_BYPASS;
}

void vDrvMJCSetFrmFreeze(UINT32 u4OnOff)
{
    arMJCPrm.fgFreeze = (UINT8)u4OnOff;

    vIO32WriteFldAlign(MJC_FM_07, arMJCPrm.arFRCConfig.u13DModeIn ? u4OnOff : SV_OFF, MJC_FREEZE_3D_EN);
    vIO32WriteFldAlign(MJC_SYS_00, u4OnOff, MJC_FRM_FREEZE);
    vDrvMJCSetPSAutoOnOff(u4OnOff);
}

UINT8 fgDrvMJCPscanSplitFS(void)
{
    UINT32 u4Mib;
    u4Mib =  u4DrvTDTVDIModeQuery();

    if ((TD_MIB_OUT(u4Mib) == E_TD_OUT_3D_FS) && ((TD_MIB_IN(u4Mib) != E_TD_IN_FS_I) && (TD_MIB_IN(u4Mib) != E_TD_IN_FS_P)))
    {
        return SV_TRUE;
    }

    return SV_FALSE;
}

void vDrvMJCApplyPdInfoProgSetting(UINT8 u1MJCIsrType)
{
    static UINT8 _u1MJCProgPdInfoNo = 0;
    static UINT8 _fgTrigger         = SV_OFF;

    if (u1MJCIsrType == MJC_INT_2)
    {
        if (IO32ReadFldAlign(MJC_SYS_06, MJC_PD_INFO_PROG_EN) == SV_ON)
        {
            _fgTrigger = SV_ON;
        }
    }
    if ((u1MJCIsrType == MJC_INT_1) && (_fgTrigger == SV_ON))
    {
        if (IO32ReadFldAlign(MJC_SYS_06, MJC_PD_INFO_PROG_EN) == SV_ON)
        {
            if (IO32ReadFldAlign(MJC_STA_FM_03, MJC_STA_LR3D_I) == 0)
            {
                _u1MJCProgPdInfoNo = u1DrvGetSWFilmInfoFromDI();
            }
            vIO32WriteFldAlign(MJC_SYS_06, _u1MJCProgPdInfoNo, MJC_PD_INFO_PROG_NO);
        }
        else
        {
            vIO32WriteFldAlign(MJC_SYS_06, 0, MJC_PD_INFO_PROG_NO);
        }
        _fgTrigger = SV_OFF;
    }
}

UINT8 u1DrvMJCSet32PdInfoShiftJudgmentNew(UINT8 fgIsInterlace, UINT8 u1ScalerMode, UINT8 fgMJCIs3DFSMode)
{
    UINT8 u1MJCPs32Shift   = 0;
    UINT8 u1MJCPs22Shift   = 0;
    arMJCPrm.fgIsInterlace = fgIsInterlace;
    arMJCPrm.u1ScalerMode  = u1ScalerMode;
    arMJCPrm.fgMJCFSIn     = fgMJCIs3DFSMode;

    LOG(2, "u1DrvMJCSet32PdInfoShiftJudgmentNew fgIsInterlace : %d, u1ScalerMode: %d  fgMJCIs3DFSMode: %d \n", fgIsInterlace, u1ScalerMode, fgMJCIs3DFSMode);
    vIO32WriteFldAlign(MJC_SYS_06, (fgMJCIs3DFSMode &&  fgIsInterlace)? SV_ON : SV_OFF,  MJC_PD_INFO_PROG_EN);

    if (fgIsInterlace == SV_TRUE)
    {
        vIO32WriteFldAlign(MJCFD_00, SV_ON , MJCFD_C_FD_INFO_SEL);
        vIO32WriteFldAlign(MJCFD_00, SV_OFF , MJCFD_C_FD_SRC_SEL);
        vDrvMJCSetFrmSchPdInfoSwDly(0, 0);
        if (u1ScalerMode == VDP_SCPOS_PSCAN_DISPMODE)
        {
            if (fgMJCIs3DFSMode)
            {
                //vDrvMJCFrmSchSetPdInfoDly(4);
                vDrvMJCFrmSchSetPdInfoDly(0);
                u1MJCPs32Shift = 4;
                u1MJCPs22Shift = 0;
            }
            else
            {
                //vDrvMJCFrmSchSetPdInfoDly(0);;//orginal 
                //u1MJCPs32Shift = 0;//orginal 
                vDrvMJCFrmSchSetPdInfoDly(1);
                u1MJCPs32Shift = 4;
                u1MJCPs22Shift = 0;
            }
            vIO32WriteFldAlign(MJC_SYS_00, SV_OFF, MJC_PD_INFO_DLY);
            vDrvMJCSetPdInfoShift(u1MJCPs32Shift, u1MJCPs22Shift);
            return SV_FALSE;
        }
        else
        {
            vDrvMJCFrmSchSetPdInfoDly(4);
			vDrvMJCSetPdInfoShift(3, 0);
			//vDrvMJCSetPdInfoShift(0, 0);//orginal 
            return SV_FALSE;
        }
    }
    else
    {
        vIO32WriteFldAlign(MJCFD_00, SV_OFF , MJCFD_C_FD_INFO_SEL);
        vIO32WriteFldAlign(MJCFD_00, SV_OFF , MJCFD_C_FD_SRC_SEL);
        vDrvMJCSetFrmSchPdInfoSwDly(1, 2);
        if (fgMJCIs3DFSMode)
        {
            //vDrvMJCFrmSchSetPdInfoDly(4);//orginal
            vDrvMJCFrmSchSetPdInfoDly(0);
            vDrvMJCSetFrmSchPdInfoSwDly(0, 3);
        }
        else
        {
            if (u1ScalerMode == VDP_SCPOS_PSCAN_DISPMODE)
            {
                vDrvMJCFrmSchSetPdInfoDly(0);
            }
            else
            {
                vDrvMJCFrmSchSetPdInfoDly(0);
				vDrvMJCSetPdInfoShift(0, 0);
				//vDrvMJCFrmSchSetPdInfoDly(1);//orginal 
                //vDrvMJCSetPdInfoShift(2, 0);//orginal 
                return SV_FALSE;
            }
        }
    }
    return SV_TRUE;
}

void vDrvMJCSet32PdInfoShiftProcNew(UINT8 u1MJCIsrType)
{
    static UINT8 _u1WaitApplyCntL   = 0;
    static UINT8 _u1MJCPdInfoInR    = 0;
    static UINT8 _u1MIBInPdInfoP    = 0;
    static UINT8 _u1MIBInPdInfoR    = 0;
    static UINT8 _fgTrigger         = SV_OFF;
    static UINT8 _fgMJCPsAutoEn     = SV_ON;
    UINT8 u1MJCPdInfoInL     = 0;
    UINT8 u1MIBPdInfoIByOut  = 0;
    UINT8 u1MIBInPdInfoP     = 0;
    UINT8 u1MIBPdInfoReslut  = 0;
    UINT8 u1MIBMJCPdInfoDiff = 0;
    UINT8 u1MJCPdInfoDelay   = 0;
    UINT8 u1MJCInLRInfo      = 0;
    UINT8 fgIsInterlace      = 0;
    UINT8 u1ScalerMode       = 0;
    UINT8 fgMJCIs3DFSMode    = 0;

    fgIsInterlace   = bDrvVideoIsSrcInterlace(SV_VP_MAIN);
    u1ScalerMode    = getScalerMode(SV_VP_MAIN);
    fgMJCIs3DFSMode = (arMJCPrm.arVrmSrmConfig.u1MJC3DModeIn == E_VDO_MJC_FS);

    if (IO32ReadFldAlign(MJC_MISC_01, MJC_PD_SHIFT_UPDATE_DISABLE) == SV_ON)
    {
        return;
    }
    if ((fgIsInterlace != arMJCPrm.fgIsInterlace) || (u1ScalerMode != arMJCPrm.u1ScalerMode) || (fgMJCIs3DFSMode != arMJCPrm.fgMJCFSIn))
    {
        if (u1DrvMJCSet32PdInfoShiftJudgmentNew(fgIsInterlace, u1ScalerMode, fgMJCIs3DFSMode) == SV_FALSE)
        {
            _fgMJCPsAutoEn =  SV_FALSE;
        }
        else
        {
            _fgMJCPsAutoEn = SV_TRUE;
        }
    }
    if (_fgMJCPsAutoEn == SV_FALSE)
    {
        return;
    }
    if (arMJCPrm.u1PSAuto32OnOff == SV_OFF)
    {
        return;
    }

    if (u1MJCIsrType == MJC_INT_2)
    {
        _u1MIBInPdInfoP    = IO32ReadFldAlign(MJCFD_08, MJCFD_FD_INFO);
        _fgTrigger         = SV_ON;
    }

    if ((u1MJCIsrType == MJC_INT_1) && (_fgTrigger == SV_ON))
    {
        _fgTrigger = SV_OFF;
        u1MJCInLRInfo     = IO32ReadFldAlign(MJC_STA_FM_03, MJC_STA_LR3D_I);

        if ((u1MJCInLRInfo == 0) || (!fgMJCIs3DFSMode))
        {

            u1MIBPdInfoIByOut = u1DrvGetSWFilmInfoFromDI();
            u1MJCPdInfoInL    = IO32ReadFldAlign(MJC_STA_FM_01, MJC_STA_PS_FILM_ID_HW);
            u1MIBInPdInfoP    = IO32ReadFldAlign(MJCFD_08, MJCFD_FD_INFO);
            _u1MIBInPdInfoP    = IO32ReadFldAlign(MJC_PD_INFO_01, MJC_PD_INFO_SEL) == 1 ? IO32ReadFldAlign(MJC_PD_INFO_01, MJC_PD_INFO_SCL_IN) : IO32ReadFldAlign(MJCFD_08, MJCFD_FD_INFO);
            LOG(6, "LR: %d  DLY: %d  MIB: %d %d %d %d MJC:%d %d \n",u1MJCInLRInfo, u1PdInfoDly,
                                                                    _u1MIBInPdInfoP,u1MIBInPdInfoP,_u1MIBInPdInfoR,u1MIBPdInfoIByOut,
                                                                    u1MJCPdInfoInL,_u1MJCPdInfoInR);
            if (IO32ReadFldAlign(MJC_MISC_01, MJC_AUTO_PD_SHIFT_DESABLE) == SV_ON)
            {
                return;
            }
            if ((u1MJCPdInfoInL < 0x1) ||  (u1MJCPdInfoInL > 0x5))
            {
                return;
            }

            if (_u1WaitApplyCntL != 0)
            {
                _u1WaitApplyCntL --;
                return;
            }

            if (fgMJCIs3DFSMode)
            {
                if (u1MJCPdInfoInL == _u1MJCPdInfoInR)
                {
                    vIO32WriteFldAlign(MJC_SYS_00, (!IO32ReadFldAlign(MJC_SYS_00, MJC_PD_INFO_DLY)), MJC_PD_INFO_DLY);
                    _u1WaitApplyCntL = 1;
                }
            }
            else
            {
                vIO32WriteFldAlign(MJC_SYS_00, SV_OFF, MJC_PD_INFO_DLY);
            }

            if (u1ScalerMode == VDP_SCPOS_PSCAN_DISPMODE)
            {
                if (fgMJCIs3DFSMode)
                {
                    u1MJCPdInfoDelay =  (fgDrvMJCPscanSplitFS() == SV_FALSE)? (0x2 - (u1MIBInPdInfoP == _u1MIBInPdInfoR)) : 0x1;
                }
                else
                {
                    u1MJCPdInfoDelay = IO32ReadFldAlign(MJC_PD_INFO_01, MJC_PD_INFO_SEL) == 1 ? IO32ReadFldAlign(MJC_PD_INFO_01, MJC_PD_INFO_DELAY) : 0x0;
                }
            }
            else
            {
                 u1MJCPdInfoDelay = fgIsInterlace? 0x3 : 0x1;
            }

            if (fgIsInterlace)
            {
                u1MIBPdInfoReslut = u1MIBPdInfoIByOut;
            }
            else
            {
                u1MIBPdInfoReslut = (fgMJCIs3DFSMode)? u1MIBInPdInfoP : _u1MIBInPdInfoP;
            }

            u1MIBMJCPdInfoDiff = (u1MIBPdInfoReslut > u1MJCPdInfoInL) ?  (u1MIBPdInfoReslut - u1MJCPdInfoInL) : ((u1MIBPdInfoReslut + 0x5) - u1MJCPdInfoInL);
            if (u1MIBMJCPdInfoDiff == u1MJCPdInfoDelay)
            {
                return;
            }
            u1PdInfoDly = (u1MIBMJCPdInfoDiff > u1MJCPdInfoDelay) ? (u1MIBMJCPdInfoDiff - u1MJCPdInfoDelay) : ((u1MIBMJCPdInfoDiff + 0x5) - u1MJCPdInfoDelay);
            u1PdInfoDly += IO32ReadFldAlign(MJC_SYS_00, MJC_PD_INFO_32_SHIFT);
            u1PdInfoDly %= 0x5;
            vDrvMJCSetPdInfoShift(u1PdInfoDly, 0);
            _u1WaitApplyCntL = 1;
        }
        else
        {
            _u1MJCPdInfoInR = IO32ReadFldAlign(MJC_STA_FM_01, MJC_STA_PS_FILM_ID_HW);
            _u1MIBInPdInfoR = IO32ReadFldAlign(MJCFD_08, MJCFD_FD_INFO);
        }
    }
}

void vDrvMJCISR(UINT16 u2Vector)
{
    UINT32  u4StaInt;
    #if MJC_FPGA
    UINT32  u4IntMask;
    #endif

    ASSERT(u2Vector == VECTOR_MJC);
    u4StaInt = IO32ReadFldAlign(MJC_STA_SYS_00, MJC_STA_INT);

    #if MJC_FPGA
    u4IntMask = IO32ReadFldAlign(MJC_SYS_INTR_CTRL, MJC_INT_MASK);
    vIO32WriteFldAlign(MJC_SYS_INTR_CTRL, 0xF, MJC_INT_MASK);
    #endif

    if (u4StaInt & MJC_INT_0) // Branking first line of output vsync
    {
        // status polling
        _vDrvMJCSpAdjInputVsync();
    }

    if (u4StaInt & MJC_INT_2) // Input vsync end
    {
        //#ifdef MOVE_FMDET_TO_OVSYNC
        vDrvMJCFrmSchInputVsyncProc();
        //#endif
        
        #if !(MJC_FPGA)
        vDrvMJCSet32PdInfoShiftProcNew(MJC_INT_2);
        vDrvMJCApplyPdInfoProgSetting(MJC_INT_2);
        //vScpipOnMJCInputVSync();
        _vDrvMJCBadRstPatch();
        vDrvMJCSetPsAutoOnProc();
        vApiApplyVideoEvent(PEQTY_APPLY_TIME_MJC_INPUT_VSYNC_ISR);
        vDrvMJCFrmDlyChk(SV_TRUE);

        // output vsync isr
        vDrvMeterISR();    
        vDrvADLProc();        
        DRVCUST_PostOutVSyncISR();
        #endif

#if 0
        #ifndef MOVE_FMDET_TO_OVSYNC
        //sw  frmsch action 
        if (arMJCPrm.arFRCConfig.u13DModeIn == E_VDO_MJC_FS)
        {
            u4InputTrigger = 1;
        }
        else
        {
            vDrvMJCFrmSchInputVsyncProc();
        }        
        #endif
#endif
        // status polling
        _vDrvMJCSpInputVsync();        

    }


    if (u4StaInt & MJC_INT_1) // Output vsync end
    {
        if (fgOutputTimingChange)
        {
            vDrvMJCSetOutputTiming(&(arMJCPrm.arPicSize));
            fgOutputTimingChange = 0;
        }

        if (u4InputTrigger)
        {
            vDrvMJCFrmSchInputVsyncProc();
            u4InputTrigger = 0;
        }
        #if !(MJC_FPGA)
        vDrvMJCSet32PdInfoShiftProcNew(MJC_INT_1);
        vDrvMJCApplyPdInfoProgSetting(MJC_INT_1);
        if (fgCSSOnOff == SV_ON)
        {
            vDrvCSSProcess((INT32)IO32ReadFldAlign(MJC_STA_FM_02, MJC_STA_MEMC_NUM_HW),
                           (INT32)IO32ReadFldAlign(MJC_STA_FM_02, MJC_STA_MEMC_DEN_HW));
        }

        #if 0
        TIME_StopMeasure(0);
        TIME_PrintElapsedTime(0);
        TIME_StartMeasure(0);
        #endif
        //if (fgCSSOnOff == SV_ON && IO32ReadFldAlign(MJC_STA_FM_02, MJC_STA_ME_FILM_IDX_HW) == 0)
        vDrvTTDSetQualityISR();
        vDrvLcdimProc();

        // for verification of DRAM mode
        //vDrvMJCDramModeAutoTestCheck();

        vDrvMJCFrmDlyChk(SV_FALSE);

        // MJC freeze
        //_vDrvMJCProcFrmFreeze();
        vDrvMJCSetQualityISR();

        //sw  frmsch action
        vDrvMJCFrmSchOutputVsyncProc();
        #endif

        vDrvMJCDsrFrmStr();

        // status polling
        _vDrvMJCSpOutputVsync();

        VERIFY(x_sema_unlock(_hMJCOutputPollingSema) == OSR_OK);
    }


    if (u4StaInt & MJC_INT_3) // MJC frame processing finished
    {
        // status polling
        _vDrvMJCSpProcFinished();
    }

    if (u4StaInt)
    {
        vIO32WriteFldAlign(MJC_SYS_INTR_CTRL, u4StaInt, MJC_INT_CLEAR);
        vIO32WriteFldAlign(MJC_SYS_INTR_CTRL, 0x0, MJC_INT_CLEAR);
        #if MJC_FPGA
        vIO32WriteFldAlign(MJC_SYS_INTR_CTRL, u4IntMask, MJC_INT_MASK);
        #endif
    }
}

UINT8 u1DrvMJCGetFrmDelay(void)
{
    UINT8 u1Delay = 0;
    VRM_INFO_T arMJCIn;
    VRM_INFO_T arMJCOut;

    VRMGetModuleVRMInfo(SV_VP_MAIN, VRM_MODULE_MJC, &arMJCIn, &arMJCOut);

    if (u1DrvMJCIsOnOff())
    {
        if (arMJCDramAlloc.u2Mode & MJC_444_MODE)
        {
            u1Delay = 1;
        }
        else
        {
            if (arMJCIn.u2FrameRate <= 30)
            {
                u1Delay = 3;
            }
            else
            {
                u1Delay = 7;
            }
        }
    }

    return u1Delay;
}

UINT8 u1DrvMJCGetFrmDelay10x(void)
{
    UINT8 u1Delay = 0;
    VRM_INFO_T arMJCIn;
    VRM_INFO_T arMJCOut;

    VRMGetModuleVRMInfo(SV_VP_MAIN, VRM_MODULE_MJC, &arMJCIn, &arMJCOut);

    if (u1DrvMJCIsOnOff())
    {
        if (arMJCDramAlloc.u2Mode & MJC_444_MODE)
        {
            u1Delay = 15;
        }
        else
        {
            if (arMJCIn.u2FrameRate <= 30)
            {
                u1Delay = 30;
            }
            else
            {
                u1Delay = 70;
            }
        }
    }

    return u1Delay;
}

UINT8 u1DrvMJCIsSupportUniCandece(void)
{
    UINT8 fgIsUnicandece = SV_FALSE;

    fgIsUnicandece = (u4DrvGetCustFRC() & SUPPORT_60TO50_2D) || (u4DrvGetCustFRC() & SUPPORT_60TO50_3D) ||
                     (u4DrvGetCustFRC() & SUPPORT_50TO60_2D) || (u4DrvGetCustFRC() & SUPPORT_50TO60_3D);
    return (arMJCPrm.fgSupportUniCandece || fgIsUnicandece);
}

void vDrvMJCSetUniCandeceEn(UINT8 u1SupportUniCandece)
{
    arMJCPrm.fgSupportUniCandece = u1SupportUniCandece;
}

const MJC_DRAM_MODE_T * rDrvMJCGetDramMode(UINT32 u4DramModeIdx)
{
    if (u4DramModeIdx < MJC_DRAM_ALL_MODE_NUM)
    {
        return &_arMJCAllDramMode[u4DramModeIdx];
    }

    for (u4DramModeIdx = 0; u4DramModeIdx < MJC_DRAM_ALL_MODE_NUM; u4DramModeIdx++)
    {
        if (_arMJCAllDramMode[u4DramModeIdx].u2DramMode == arMJCDramAlloc.u2Mode)
            break;
    }

    return &_arMJCAllDramMode[u4DramModeIdx];
}

UINT8 fgDrvRealCinemaModeTrgModeChg(UINT32 fg44PullDownUpdated)
{
    UINT8 u1OutputFrameRate;
    UINT8 u1RefreshRate;
    UINT8 fgModeChgTrgEN;

    fgModeChgTrgEN   = SV_OFF;
    u1RefreshRate    = bDrvVideoGetRefreshRate(SV_VP_MAIN);
    if (u1RefreshRate == 24)
    {
        u1OutputFrameRate  = 60;
        if (SUPPORT_PANEL_48HZ)
        {
            if (IS_PANEL_L12R12)
            {
                if (fgDrvSupport24Hz())
                {
                    u1OutputFrameRate  = 48;
                }
                else
                {
                    u1OutputFrameRate  = 60;
                }
            }
            else
            {
                if (PANEL_IsSupport120Hz())
                {
                    if ((u4DrvTDTV3DModeQuery() == E_TDTV_UI_3D_MODE_SBS) && (fg44PullDownUpdated == SV_TRUE))
                    {
                        u1OutputFrameRate  = 48;
                    }
                    else
                    {
                        u1OutputFrameRate  = 60;
                    }
                }
                else
                {
                    if (fg44PullDownUpdated == SV_TRUE)
                    {
                        u1OutputFrameRate  = 48;
                    }
                    else
                    {
                        u1OutputFrameRate  = 60;
                    }
                }
            }
        }
    }
    else if (u1RefreshRate == 48)
    {
        u1OutputFrameRate  = 60;
        if (SUPPORT_PANEL_48HZ)
        {
            if(IS_PANEL_L12R12)
            {
                u1OutputFrameRate  = 48;
            }
            else
            {
                if ((u4DrvTDTV3DModeQuery() == E_TDTV_UI_3D_MODE_FS) && (fg44PullDownUpdated == SV_FALSE))
                {
                    u1OutputFrameRate  = 60;
                }
                else
                {
                    u1OutputFrameRate  = 48;
                }
            }
        }
    }
    else
    {
        return (SV_FALSE);
    }

    if ((vDrvGetLCDFreq() % u1OutputFrameRate) != 0)
    {
        fgModeChgTrgEN   = SV_ON;
    }

    return (fgModeChgTrgEN);
}

void vDrvUpdateRealCinemaMode(UINT16 u2Effect, UINT16 fgRealCinema, UINT16 u2TDTVPacking)
{
    static UINT16 u2EffectLevel = 0xFFFF;
    static UINT16 fgRealCinemaOnOff = 0xFFFF;
    static UINT16 u2TDTVPackingValue = 0xFFFF;
    UINT32 fg44PullDownUpdated;

    if (IS_PANEL_L12R12)
    {
        return;
    }
    if (u2Effect == 0xFFFF)
    {
        u2Effect = u2EffectLevel;
    }
    if (fgRealCinema == 0xFFFF)
    {
        fgRealCinema = fgRealCinemaOnOff;
    }
    if (u2TDTVPacking == 0xFFFF)
    {
        u2TDTVPacking = u2TDTVPackingValue;
    }

    if (PANEL_IsSupport120Hz() || (bSupportDejudder == TRUE))
    {
        fg44PullDownUpdated = (u2Effect == 0 && fgRealCinema == SV_ON) ? SV_ON : SV_OFF;
    }
    else
    {
        fg44PullDownUpdated = (fgRealCinema == SV_ON) ? SV_ON : SV_OFF;
    }
    LOG(2, "vDrvUpdateRealCinemaMode: u2Effect=%d, fgRealCinema=%d, u2TDTVPacking=%d.\n", u2Effect, fgRealCinema, u2TDTVPacking);

    if (bDrvVideoSignalStatus(SV_VP_MAIN) != SV_VDO_STABLE)
    {
        return;
    }

    if (fgDrvRealCinemaModeTrgModeChg(fg44PullDownUpdated) == SV_TRUE)
    {
        _vDrvVideoSetMute(MUTE_MODULE_MJC, SV_VP_MAIN, 20, SV_TRUE);
        vSetMainFlg(MAIN_FLG_MODE_CHG);
        vSetMainFlg(MAIN_FLG_MODE_DET_DONE);
        LOG(2, "vDrvUpdateRealCinemaMode: Mode Change.\n");
    }

    vIO32WriteFldAlign(MJC_MISC_00, fg44PullDownUpdated, MJC_IS_44PULLDOWN);
    u2EffectLevel = u2Effect;
    fgRealCinemaOnOff = fgRealCinema;
    u2TDTVPackingValue = u2TDTVPacking;
}

void vDrvMJCSetMEMCOnOff(UINT8 u1OnOff)
{
   vDrvMJCSetMCOnOff(u1OnOff);

}

#ifdef CC_SUPPORT_STR
void vDrvMJCResume(void)
{
    //MEMC clock configuration
    vDrvMJCSetClock();

    //MEMC buffer configuration
    if (u4DrvMJCBufferInit() == SV_FAIL)
    {
        MJC_SetOnOff(E_MJC_BYPASS);
        return;
    }
    vDrvMJCSetBuf(&arMJCDramAlloc);
	 //MJC System
    vDrvLoadRegTbl(MJC_SYS_INIT);
    vDrvMJCSetSystemMode(arMJCDramAlloc.u2Mode);

    //MEMC Quality
    vDrvMFDInit();
    vDrvMJCInitQuality();
    vDrvMJCFrmSchInit();

    //MJC PQ
    vDrvLoadRegTbl(MJC_FM_INIT);
    vDrvLoadRegTbl(MJC_ME_INIT);
    vDrvLoadRegTbl(MJC_MV_INIT);
    vDrvLoadRegTbl(MJC_CA_INIT);
    vDrvLoadRegTbl(MJC_MC_INIT);
    vDrvLoadRegTbl(MJC_TBA_INIT);
    vDrvLoadRegTbl(MJC_PSG_INIT);

    if ((!PANEL_IsSupport120Hz()) &&(!PANEL_IsSupport240Hz()))
    {
        vIO32Write4B(MJC_ME_03, 0x9848c858);    //ME penalty
    }

    vIO32WriteFldMulti(MJC_SYS_DISP,
        P_Fld(SV_OFF, MJC_DISP_SYNC_SELF) |
        P_Fld(SV_ON, MJC_DISP_MODE) |
        P_Fld(0x1F, MJC_DISP_VPOS));
    vIO32WriteFldAlign(MJC_SYS_INTR_CTRL, 0x2, MJC_INT_POL_SEL);

    //MJC dither init
    vIO32Write4B(MJC_FB_0B, 0x44880000);
    vIO32Write4B(MJC_SYS_D9, 0x18055000);
    vIO32Write4B(MJC_SYS_DA, 0xA0000000);

    MJC_SetOnOff(arMJCPrm.u1OnOff & (~MJC_SUSPEND_MODE));

    fgMJCChangeFrmSch = SV_TRUE;
    fgMJCChangePicSize = SV_TRUE;
}

void vDrvMJCSuspend(void)
{
    arMJCPrm.u1OnOff |= MJC_SUSPEND_MODE;
   
    vIO32WriteFldMulti(MJC_SYS_CLK_CTRL,
        P_Fld(SV_OFF, MJC_CLK_I_EN) |
        P_Fld(SV_OFF, MJC_CLK_M_EN));
   
}
#endif

#ifdef MJC_SW_SUPPORT_FRM_DELAY
void vDrvMJCSetSWFrameDelay(UINT8 u1FrameDelayCnt)
{
   LOG(0,"Setting SW frame delay to %d\n",u1FrameDelayCnt);
   vIO32WriteFldAlign(MJC_FW_SCH_01, u1FrameDelayCnt,MJC_FW_FRM_SCH_IN_DLY);  
}
#endif

void vDrvMJCQueryInOutCtrl(void)
{
    Printf("MJC Input/Output Control\n\n");
    Printf("\tInput WxH HTxVT ........: (%dx%d, %dx%d)\n", IO32ReadFldAlign(MJC_SYS_02, MJC_MJC_WIDTH), IO32ReadFldAlign(MJC_SYS_02, MJC_MJC_HEIGHT_FX), IO32ReadFldAlign(MJC_STA_SYS_01, MJC_STA_H_TOTAL) + 1, IO32ReadFldAlign(MJC_STA_SYS_01, MJC_STA_V_TOTAL)+1);
    Printf("\tInput Start (H,V) ......: (%d, %d)\n", IO32ReadFldAlign(MJC_SYS_01, MJC_IN_H_START), IO32ReadFldAlign(MJC_SYS_01, MJC_IN_V_START));
    Printf("\tOutput WxH HTxVT .......: (%dx%d, %dx%d)\n", IO32ReadFldAlign(MJC_SYS_04, MJC_OUT_WIDTH), IO32ReadFldAlign(MJC_SYS_04, MJC_OUT_HEIGHT), (IO32ReadFldAlign(MJC_SYS_H_MSB , MJC_OUT_H_TOTAL_MSB)<<12)+IO32ReadFldAlign(MJC_SYS_05, MJC_OUT_H_TOTAL), IO32ReadFldAlign(MJC_SYS_05, MJC_OUT_V_TOTAL));
    Printf("\tOutput Start (H,V)......: (%d, %d)\n", IO32ReadFldAlign(MJC_SYS_03, MJC_OUT_H_START), IO32ReadFldAlign(MJC_SYS_03, MJC_OUT_V_START));
    Printf("\n");
}

void vDrvMJCQueryFrmSchCtrl(void)
{
    Printf("MJC Frame Schedule Control\n\n");
    #ifndef CC_FPGA
    //Printf("\tOCLK ...................: %d Hz\n", BSP_GetDomainClock(CAL_SRC_VOPLL));
    #endif
    Printf("\tInput Frame Rate .......: %d\n", arMJCPrm.arFRCConfig.u1InFrmRate);
    Printf("\tOutput Frame Rate ......: %d\n", arMJCPrm.arFRCConfig.u1OutFrmRate);
    Printf("\t(2X, 4X) Rate ..........: (%d, %d)\n", IO32ReadFldAlign(MJC_FM_00, MJC_OUT_DB_RATE_EN), IO32ReadFldAlign(MJC_FM_00, MJC_OUT_4X_RATE_EN));
    Printf("\tProgram Frame Rate .....: (%d, %d/%d)\n", IO32ReadFldAlign(MJC_SYS_04, MJC_PROG_FRM_EN), IO32ReadFldAlign(MJC_SYS_04, MJC_OUT_FRM_NO), IO32ReadFldAlign(MJC_SYS_04, MJC_OUT_VSYNC_DIV) + 1);
    Printf("\tPsAuto(32,22) ..........: (%d, %d)\n", IO32ReadFldAlign(MJC_FM_00, MJC_PSAUTO_32), IO32ReadFldAlign(MJC_FM_00, MJC_PSAUTO_22));
    Printf("\tPdShif(32,22) ..........: (%d, %d)\n", IO32ReadFldAlign(MJC_SYS_00, MJC_PD_INFO_32_SHIFT), IO32ReadFldAlign(MJC_SYS_00, MJC_PD_INFO_22_SHIFT));
    Printf("\tIntp Mode (64, 55P2) ...: (%d, %d)\n", IO32ReadFldAlign(MJC_FM_00, MJC_FALLBACK_64), IO32ReadFldAlign(MJC_FM_00, MJC_FALLBACK_55P2));
    Printf("\n");
}

void vDrvMJCQueryDramCtrl(void)
{
    const MJC_DRAM_MODE_T * prDramMode;
    prDramMode = rDrvMJCGetDramMode(MJC_DRAM_MODE_QUERY);

    Printf("MJC DRAM Control\n\n");
    Printf("\tDRAM Mode ..............: %s(0x%02X)\n", prDramMode->strDramMode, prDramMode->u2DramMode);
    Printf("\tDATA Mode ..............: %s\n", IO32ReadFldAlign(MJC_FB_01, MJC_DRAM_RPT) ? ((IO32ReadFldAlign(MJC_FB_01, MJC_DRAM_RPT_422))? "RPT422":"RPT444") : (IO32ReadFldAlign(MJC_FB_00, MJC_DRAM_420) ? "420" : "422"));
    Printf("\tY/C ....................: %s/%s\n", IO32ReadFldAlign(MJC_FB_00, MJC_DRAM_Y_8BIT) ? "8-bit" : "10-bit", IO32ReadFldAlign(MJC_FB_00, MJC_DRAM_C_8BIT) ? "8-bit" : "10-bit");
    Printf("\tDRAM Start Addr ........: 0x%X%06X0\n", IO32ReadFldAlign(MJC_FB_0E, MJC_DRAM_BASE_ADDR_MSB), IO32ReadFldAlign(MJC_FB_08, MJC_DRAM_BASE_ADDR));
    Printf("\n");
}

void vDrvMJCQuery3DCtrl(void)
{
    Printf("MJC 3D Control\n\n");
    Printf("\tInput 3D Mode ..........: %s\n", (arMJCPrm.arFRCConfig.u13DModeIn== E_VDO_MJC_LI)? "3D_LI" :
                                               (arMJCPrm.arFRCConfig.u13DModeIn  == E_VDO_MJC_FS) ? "3D_FS" :
                                               (arMJCPrm.arFRCConfig.u13DModeIn == E_VDO_MJC_SBS) ? "3D_SBS" :
                                               (arMJCPrm.arFRCConfig.u13DModeIn == E_VDO_MJC_TAB)? "3D_TAB" : "2D");

    Printf("\tOutput 3D Mode .........: %s\n", (arMJCPrm.arFRCConfig.u13DModeOut == E_VD0_MJC_OUT_HPR)? "OUT_HPR" :
                                               (arMJCPrm.arFRCConfig.u13DModeOut == E_VD0_MJC_OUT_FPR)? "OUT_PR" :
                                               (arMJCPrm.arFRCConfig.u13DModeOut == E_VD0_MJC_OUT_FS)? "OUT_FS" :
                                               (arMJCPrm.arFRCConfig.u13DModeOut == E_VDO_MJC_OUT_2D)? "OUT_2D" : "OUT_2D");
    Printf("\tPanel PR ...............: %d %s\n", IS_LVDS_DISP_3D_POLARIZED, (IS_LVDS_DISP_3D_POLARIZED) ? (LVDS_DISP_3D == LVDS_DISP_3D_POLARIZED_LRLR ? "LRLR" : "RLRL") : "");
    Printf("\tPanel SG ...............: %d\n", IS_LVDS_DISP_3D_SHUTTER);
    Printf("\t3D_MODE ................: %d\n", IO32ReadFldAlign(MJC_FB_LR3D, MJC_3D_MODE));
    Printf("\tPR Type ................: %s\n", ((arMJCPrm.arMJCPRConfig.u1MJCOSMPFPRMode == E_MJC_FPRV3)? "FPRV3" :
                                                (arMJCPrm.arMJCPRConfig.u1MJCOSMPFPRMode == E_MJC_FPRV2)? "FPRV2" :
                                                (arMJCPrm.arMJCPRConfig.u1MJCOSMPFPRMode == E_MJC_FPRV1)? "FPRV1" :
                                                (arMJCPrm.arMJCPRConfig.u1MJCOSMPFPRMode == E_MJC_FPRV0)? "FPRV0" : "UNKNOWN"));
    Printf("\tHPR Type ...............: %s\n", ((arMJCPrm.arMJCPRConfig.u1MJCOSMPHPRMode == E_MJC_HPR_NM)? "HPR NORMAL" :
                                                (arMJCPrm.arMJCPRConfig.u1MJCOSMPHPRMode == E_MJC_HPR2160)? "HPR 2160" : "UNKNOWN"));
    Printf("\n");
}

void vDrvMJCDbgLogThread(void* pvArgv)
{
    PThreadFunc pfnFunc = (*((PThreadFunc*) pvArgv));
	
	while (u4MJCStatusPollingCount--)
    {
        VERIFY(x_sema_lock(_hMJCOutputPollingSema, X_SEMA_OPTION_WAIT) == OSR_OK);
        
        (*pfnFunc)(NULL);
        Printf("\n");
	}
}

