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

// === INCLUDE =============================================================================
#include "x_os.h"	
#include "sv_const.h"
#include "hw_vdoin.h"
#include "hw_nr.h"
#include "vdo_misc.h"
#include "nptv_debug.h"
#include "drv_contrast.h"
#include "drv_meter.h"
#include "pe_if.h"
#include "drv_scaler.h"
#include "fbm_drvif.h"
#include "drv_display.h"


// === HW INCLUDE =============================================================================
#include "hw_tdsharp.h"
#include "hw_scpip_pq.h"
#include "hw_sw.h"
#include "hw_di_int.h"
#include "source_select.h"

// === DEFINE =============================================================================
#define TDS_GLOBAL_GAIN_UNIT 0x20

#define AbsDiff(a, b)           (((a)>(b))?((a)-(b)):((b)-(a)))
#define CLIP(val, min, max)     ((val>=max) ? max : ((val<=min) ? min : val)) 

// === EXTERN FUNCTION ====================================================================
EXTERN UINT8 bADLSceneChange;
EXTERN UINT8 bForceAllPQOff;


// === GLOBAL VARIABLE ====================================================================

// === Static VARIABLE ====================================================================
static BOOL fgAdapYLEVLock = SV_OFF;
static UINT16 TDS_ADAP_YLEV[17];
static UINT8 bCDS2OnOff;
static UINT8 bInitGetFlg = SV_TRUE, bInitSetFlg = SV_TRUE;

UINT8 TDS_YLEV[17] =
{
    0x40,0x4D,0x5A,0x66,0x73,0x76,0x7A,0x7D,
    0x80,0x80,0x80,0x80,0x80,0x7D,0x7A,0x76,
    0x72  
};

UINT8 TDS_YLEV_APL[17] =
{
    0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80,
    0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80,
    0x80       
};

UINT8 TDS_YLEV_CUST[17] =
{
    0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80,
    0x80, 0x80, 0x80, 0x80, 0x69, 0x51, 0x3D, 0x38,
    0x20
};
UINT32 TDS_YLEV_32BIT_VALUE[5];

static UINT32 const TDS_CURVE_REGADDR[TDS_BAND_NUM]=
{
	SHARP_03, //H1
	SHARP_07, //H2
	SHARP_0B, //H3
	SHARP_0F, //V1
	SHARP_13, //V2
	SHARP_17, //V3
	SHARP_1B, //D1
	SHARP_1F, //D2
	SHARP_23, //D3
	SHARP_50, //LTI_HL
	SHARP_54, //LTI_HH
	SHARP_58, //LTI_V
	CDS_06,   //CDS1_H1
	CDS_0A,   //CDS1_H2
	CDS_0E,   //CDS1_H3
	CDS_12,   //CDS1_V1
	CDS_16,   //CDS1_V2
	CDS_1A,   //CDS1_V3
	CDS_1E,   //CDS1_D1
	CDS_22,   //CDS1_D2
	CDS_26,   //CDS1_D3
	CDS_30,   //CDS2_H1
	CDS_34,   //CDS2_H2
	CDS_38,   //CDS2_H3
	CDS_3C,   //CDS2_V1
	CDS_40,   //CDS2_V2
	CDS_44,   //CDS2_V3
	CDS_48,   //CDS2_D1
	CDS_4C,   //CDS2_D2
	CDS_50,   //CDS2_D3
	CDS_59,   //CDS3_H1
	CDS_5D,   //CDS3_H2
	CDS_61,   //CDS3_H3
	CDS_65,   //CDS3_V1
	CDS_69,   //CDS3_V2
	CDS_6D,   //CDS3_V3
	CDS_71,   //CDS3_D1
	CDS_75,   //CDS3_D2
	CDS_79    //CDS3_D3
};

static UINT32 const TDS_Curve_SW_Reg[TDS_BAND_NUM][TDS_CURVE_SW_REG_END]=
{
    //Gain               //Coring_zero       //Coring_ratio            //Coring_th           //Limit              //Limit_ratio             //Bound
    {TDSHP_GAIN_H1,      TDSHP_CORE_H1,      TDSHP_CORING_RATIO_H1,    TDSHP_CORETH_H1,      TDSHARP_LIMIT_H1,    TDSHP_LIMIT_RATIO_H1,      TDSHARP_BOUND_H1}, 
    {TDSHP_GAIN_H2,      TDSHP_CORE_H2,      TDSHP_CORING_RATIO_H2,    TDSHP_CORETH_H2,      TDSHARP_LIMIT_H2,    TDSHP_LIMIT_RATIO_H2,      TDSHARP_BOUND_H2},
    {TDSHP_GAIN_H3,      TDSHP_CORE_H3,      TDSHP_CORING_RATIO_H3,    TDSHP_CORETH_H3,      TDSHARP_LIMIT_H3,    TDSHP_LIMIT_RATIO_H3,      TDSHARP_BOUND_H3},
    {TDSHP_GAIN_V1,      TDSHP_CORE_V1,      TDSHP_CORING_RATIO_V1,    TDSHP_CORETH_V1,      TDSHARP_LIMIT_V1,    TDSHP_LIMIT_RATIO_V1,      TDSHARP_BOUND_V1},
    {TDSHP_GAIN_V2,      TDSHP_CORE_V2,      TDSHP_CORING_RATIO_V2,    TDSHP_CORETH_V2,      TDSHARP_LIMIT_V2,    TDSHP_LIMIT_RATIO_V2,      TDSHARP_BOUND_V2},
    {TDSHP_GAIN_V3,      TDSHP_CORE_V3,      TDSHP_CORING_RATIO_V3,    TDSHP_CORETH_V3,      TDSHARP_LIMIT_V3,    TDSHP_LIMIT_RATIO_V3,      TDSHARP_BOUND_V3},
    {TDSHP_GAIN_D1,      TDSHP_CORE_D1,      TDSHP_CORING_RATIO_D1,    TDSHP_CORETH_D1,      TDSHARP_LIMIT_D1,    TDSHP_LIMIT_RATIO_D1,      TDSHARP_BOUND_D1},
    {TDSHP_GAIN_D2,      TDSHP_CORE_D2,      TDSHP_CORING_RATIO_D2,    TDSHP_CORETH_D2,      TDSHARP_LIMIT_D2,    TDSHP_LIMIT_RATIO_D2,      TDSHARP_BOUND_D2},
    {TDSHP_GAIN_D3,      TDSHP_CORE_D3,      TDSHP_CORING_RATIO_D3,    TDSHP_CORETH_D3,      TDSHARP_LIMIT_D3,    TDSHP_LIMIT_RATIO_D3,      TDSHARP_BOUND_D3},
    //LTI
    {TDSHP_GAIN_LTI_H1,  TDSHP_CORE_LTI_H1,  TDSHP_CORE_RATIO_LTI_H1,  TDSHP_CORETH_LTI_H1,  LTI_LIMIT_LTI_H1,    TDSHP_LIMIT_RATIO_LTI_H1,  LTI_BOUND_LTI_H1},
    {TDSHP_GAIN_LTI_H2,  TDSHP_CORE_LTI_H2,  TDSHP_CORE_RATIO_LTI_H2,  TDSHP_CORETH_LTI_H2,  LTI_LIMIT_LTI_H2,    TDSHP_LIMIT_RATIO_LTI_H2,  LTI_BOUND_LTI_H2},
    {TDSHP_GAIN_LTI_V,   TDSHP_CORE_LTI_V,   TDSHP_CORE_RATIO_LTI_V,   TDSHP_CORETH_LTI_V,   LTI_LIMIT_LTI_V,     TDSHP_LIMIT_RATIO_LTI_V,   LTI_BOUND_LTI_V},
    //PBC1
    {TDSHP_GAIN_PBC1_H1, TDSHP_CORE_PBC1_H1, TDSHP_CORE_RATIO_PBC1_H1, TDSHP_CORETH_PBC1_H1, TDSHP_LIMIT_PBC1_H1, TDSHP_LIMIT_RATIO_PBC1_H1, TDSHP_BOUND_PBC1_H1},
    {TDSHP_GAIN_PBC1_H2, TDSHP_CORE_PBC1_H2, TDSHP_CORE_RATIO_PBC1_H2, TDSHP_CORETH_PBC1_H2, TDSHP_LIMIT_PBC1_H2, TDSHP_LIMIT_RATIO_PBC1_H2, TDSHP_BOUND_PBC1_H2},
    {TDSHP_GAIN_PBC1_H3, TDSHP_CORE_PBC1_H3, TDSHP_CORE_RATIO_PBC1_H3, TDSHP_CORETH_PBC1_H3, TDSHP_LIMIT_PBC1_H3, TDSHP_LIMIT_RATIO_PBC1_H3, TDSHP_BOUND_PBC1_H3},
    {TDSHP_GAIN_PBC1_V1, TDSHP_CORE_PBC1_V1, TDSHP_CORE_RATIO_PBC1_V1, TDSHP_CORETH_PBC1_V1, TDSHP_LIMIT_PBC1_V1, TDSHP_LIMIT_RATIO_PBC1_V1, TDSHP_BOUND_PBC1_V1},
    {TDSHP_GAIN_PBC1_V2, TDSHP_CORE_PBC1_V2, TDSHP_CORE_RATIO_PBC1_V2, TDSHP_CORETH_PBC1_V2, TDSHP_LIMIT_PBC1_V2, TDSHP_LIMIT_RATIO_PBC1_V2, TDSHP_BOUND_PBC1_V2},
    {TDSHP_GAIN_PBC1_V3, TDSHP_CORE_PBC1_V3, TDSHP_CORE_RATIO_PBC1_V3, TDSHP_CORETH_PBC1_V3, TDSHP_LIMIT_PBC1_V3, TDSHP_LIMIT_RATIO_PBC1_V3, TDSHP_BOUND_PBC1_V3},
    {TDSHP_GAIN_PBC1_D1, TDSHP_CORE_PBC1_D1, TDSHP_CORE_RATIO_PBC1_D1, TDSHP_CORETH_PBC1_D1, TDSHP_LIMIT_PBC1_D1, TDSHP_LIMIT_RATIO_PBC1_D1, TDSHP_BOUND_PBC1_D1},
    {TDSHP_GAIN_PBC1_D2, TDSHP_CORE_PBC1_D2, TDSHP_CORE_RATIO_PBC1_D2, TDSHP_CORETH_PBC1_D2, TDSHP_LIMIT_PBC1_D2, TDSHP_LIMIT_RATIO_PBC1_D2, TDSHP_BOUND_PBC1_D2},
    {TDSHP_GAIN_PBC1_D3, TDSHP_CORE_PBC1_D3, TDSHP_CORE_RATIO_PBC1_D3, TDSHP_CORETH_PBC1_D3, TDSHP_LIMIT_PBC1_D3, TDSHP_LIMIT_RATIO_PBC1_D3, TDSHP_BOUND_PBC1_D3},
    //PBC2
    {TDSHP_GAIN_PBC2_H1, TDSHP_CORE_PBC2_H1, TDSHP_CORE_RATIO_PBC2_H1, TDSHP_CORETH_PBC2_H1, TDSHP_LIMIT_PBC2_H1, TDSHP_LIMIT_RATIO_PBC2_H1, TDSHP_BOUND_PBC2_H1},
    {TDSHP_GAIN_PBC2_H2, TDSHP_CORE_PBC2_H2, TDSHP_CORE_RATIO_PBC2_H2, TDSHP_CORETH_PBC2_H2, TDSHP_LIMIT_PBC2_H2, TDSHP_LIMIT_RATIO_PBC2_H2, TDSHP_BOUND_PBC2_H2},
    {TDSHP_GAIN_PBC2_H3, TDSHP_CORE_PBC2_H3, TDSHP_CORE_RATIO_PBC2_H3, TDSHP_CORETH_PBC2_H3, TDSHP_LIMIT_PBC2_H3, TDSHP_LIMIT_RATIO_PBC2_H3, TDSHP_BOUND_PBC2_H3},
    {TDSHP_GAIN_PBC2_V1, TDSHP_CORE_PBC2_V1, TDSHP_CORE_RATIO_PBC2_V1, TDSHP_CORETH_PBC2_V1, TDSHP_LIMIT_PBC2_V1, TDSHP_LIMIT_RATIO_PBC2_V1, TDSHP_BOUND_PBC2_V1},
    {TDSHP_GAIN_PBC2_V2, TDSHP_CORE_PBC2_V2, TDSHP_CORE_RATIO_PBC2_V2, TDSHP_CORETH_PBC2_V2, TDSHP_LIMIT_PBC2_V2, TDSHP_LIMIT_RATIO_PBC2_V2, TDSHP_BOUND_PBC2_V2},
    {TDSHP_GAIN_PBC2_V3, TDSHP_CORE_PBC2_V3, TDSHP_CORE_RATIO_PBC2_V3, TDSHP_CORETH_PBC2_V3, TDSHP_LIMIT_PBC2_V3, TDSHP_LIMIT_RATIO_PBC2_V3, TDSHP_BOUND_PBC2_V3},
    {TDSHP_GAIN_PBC2_D1, TDSHP_CORE_PBC2_D1, TDSHP_CORE_RATIO_PBC2_D1, TDSHP_CORETH_PBC2_D1, TDSHP_LIMIT_PBC2_D1, TDSHP_LIMIT_RATIO_PBC2_D1, TDSHP_BOUND_PBC2_D1},
    {TDSHP_GAIN_PBC2_D2, TDSHP_CORE_PBC2_D2, TDSHP_CORE_RATIO_PBC2_D2, TDSHP_CORETH_PBC2_D2, TDSHP_LIMIT_PBC2_D2, TDSHP_LIMIT_RATIO_PBC2_D2, TDSHP_BOUND_PBC2_D2},
    {TDSHP_GAIN_PBC2_D3, TDSHP_CORE_PBC2_D3, TDSHP_CORE_RATIO_PBC2_D3, TDSHP_CORETH_PBC2_D3, TDSHP_LIMIT_PBC2_D3, TDSHP_LIMIT_RATIO_PBC2_D3, TDSHP_BOUND_PBC2_D3},
    //PBC3
    {TDSHP_GAIN_PBC3_H1, TDSHP_CORE_PBC3_H1, TDSHP_CORE_RATIO_PBC3_H1, TDSHP_CORETH_PBC3_H1, TDSHP_LIMIT_PBC3_H1, TDSHP_LIMIT_RATIO_PBC3_H1, TDSHP_BOUND_PBC3_H1},
    {TDSHP_GAIN_PBC3_H2, TDSHP_CORE_PBC3_H2, TDSHP_CORE_RATIO_PBC3_H2, TDSHP_CORETH_PBC3_H2, TDSHP_LIMIT_PBC3_H2, TDSHP_LIMIT_RATIO_PBC3_H2, TDSHP_BOUND_PBC3_H2},
    {TDSHP_GAIN_PBC3_H3, TDSHP_CORE_PBC3_H3, TDSHP_CORE_RATIO_PBC3_H3, TDSHP_CORETH_PBC3_H3, TDSHP_LIMIT_PBC3_H3, TDSHP_LIMIT_RATIO_PBC3_H3, TDSHP_BOUND_PBC3_H3},
    {TDSHP_GAIN_PBC3_V1, TDSHP_CORE_PBC3_V1, TDSHP_CORE_RATIO_PBC3_V1, TDSHP_CORETH_PBC3_V1, TDSHP_LIMIT_PBC3_V1, TDSHP_LIMIT_RATIO_PBC3_V1, TDSHP_BOUND_PBC3_V1},
    {TDSHP_GAIN_PBC3_V2, TDSHP_CORE_PBC3_V2, TDSHP_CORE_RATIO_PBC3_V2, TDSHP_CORETH_PBC3_V2, TDSHP_LIMIT_PBC3_V2, TDSHP_LIMIT_RATIO_PBC3_V2, TDSHP_BOUND_PBC3_V2},
    {TDSHP_GAIN_PBC3_V3, TDSHP_CORE_PBC3_V3, TDSHP_CORE_RATIO_PBC3_V3, TDSHP_CORETH_PBC3_V3, TDSHP_LIMIT_PBC3_V3, TDSHP_LIMIT_RATIO_PBC3_V3, TDSHP_BOUND_PBC3_V3},
    {TDSHP_GAIN_PBC3_D1, TDSHP_CORE_PBC3_D1, TDSHP_CORE_RATIO_PBC3_D1, TDSHP_CORETH_PBC3_D1, TDSHP_LIMIT_PBC3_D1, TDSHP_LIMIT_RATIO_PBC3_D1, TDSHP_BOUND_PBC3_D1},
    {TDSHP_GAIN_PBC3_D2, TDSHP_CORE_PBC3_D2, TDSHP_CORE_RATIO_PBC3_D2, TDSHP_CORETH_PBC3_D2, TDSHP_LIMIT_PBC3_D2, TDSHP_LIMIT_RATIO_PBC3_D2, TDSHP_BOUND_PBC3_D2},
    {TDSHP_GAIN_PBC3_D3, TDSHP_CORE_PBC3_D3, TDSHP_CORE_RATIO_PBC3_D3, TDSHP_CORETH_PBC3_D3, TDSHP_LIMIT_PBC3_D3, TDSHP_LIMIT_RATIO_PBC3_D3, TDSHP_BOUND_PBC3_D3}
};

static UINT32 const TDS_Curve_SW_Reg_Base[TDS_BAND_NUM][TDS_CURVE_SW_REG_END]=
{
    //Gain           //Coring_zero   //Coring_ratio          //Coring_th       //Limit          //Limit_ratio          //Bound
    {VI_SHP_GAIN_01, VI_SHP_CORE_01, VI_SHP_CORING_RATIO_01, VI_SHP_CORETH_01, VI_SHP_LIMIT_00, VI_SHP_LIMIT_RATIO_01, VI_SHP_LIMIT_03},//H1
    {VI_SHP_GAIN_01, VI_SHP_CORE_01, VI_SHP_CORING_RATIO_01, VI_SHP_CORETH_01, VI_SHP_LIMIT_00, VI_SHP_LIMIT_RATIO_01, VI_SHP_LIMIT_03},//H2
    {VI_SHP_GAIN_01, VI_SHP_CORE_01, VI_SHP_CORING_RATIO_01, VI_SHP_CORETH_01, VI_SHP_LIMIT_00, VI_SHP_LIMIT_RATIO_01, VI_SHP_LIMIT_03},//H3
    {VI_SHP_GAIN_02, VI_SHP_CORE_02, VI_SHP_CORING_RATIO_02, VI_SHP_CORETH_02, VI_SHP_LIMIT_01, VI_SHP_LIMIT_RATIO_02, VI_SHP_LIMIT_04},//V1
    {VI_SHP_GAIN_02, VI_SHP_CORE_02, VI_SHP_CORING_RATIO_02, VI_SHP_CORETH_02, VI_SHP_LIMIT_01, VI_SHP_LIMIT_RATIO_02, VI_SHP_LIMIT_04},//V2
    {VI_SHP_GAIN_03, VI_SHP_CORE_03, VI_SHP_CORING_RATIO_03, VI_SHP_CORETH_03, VI_SHP_LIMIT_06, VI_SHP_LIMIT_RATIO_03, VI_SHP_LIMIT_06},//V3
    {VI_SHP_GAIN_01, VI_SHP_CORE_01, VI_SHP_CORING_RATIO_01, VI_SHP_CORETH_01, VI_SHP_LIMIT_00, VI_SHP_LIMIT_RATIO_01, VI_SHP_LIMIT_03},//D1
    {VI_SHP_GAIN_02, VI_SHP_CORE_02, VI_SHP_CORING_RATIO_02, VI_SHP_CORETH_02, VI_SHP_LIMIT_01, VI_SHP_LIMIT_RATIO_02, VI_SHP_LIMIT_04},//D2
    {VI_SHP_GAIN_02, VI_SHP_CORE_02, VI_SHP_CORING_RATIO_02, VI_SHP_CORETH_02, VI_SHP_LIMIT_01, VI_SHP_LIMIT_RATIO_02, VI_SHP_LIMIT_04},//D3
    //LTI
    {VI_SHP_GAIN_04, VI_SHP_CORE_04, VI_SHP_CORING_RATIO_04, VI_SHP_CORETH_04, VI_SHP_LIMIT_02, VI_SHP_LIMIT_RATIO_04, VI_SHP_LIMIT_05},//LTI_H1
    {VI_SHP_GAIN_04, VI_SHP_CORE_04, VI_SHP_CORING_RATIO_04, VI_SHP_CORETH_04, VI_SHP_LIMIT_02, VI_SHP_LIMIT_RATIO_04, VI_SHP_LIMIT_05},//LTI_H1
    {VI_SHP_GAIN_04, VI_SHP_CORE_04, VI_SHP_CORING_RATIO_04, VI_SHP_CORETH_04, VI_SHP_LIMIT_02, VI_SHP_LIMIT_RATIO_04, VI_SHP_LIMIT_05},//LTI_H1
    //PBC1
    {VI_SHP_GAIN_05, VI_SHP_CORE_05, VI_SHP_CORING_RATIO_05, VI_SHP_CORETH_05, VI_SHP_LIMIT_05_2, VI_SHP_LIMIT_RATIO_05, VI_SHP_BOUND_05},//H1
    {VI_SHP_GAIN_05, VI_SHP_CORE_05, VI_SHP_CORING_RATIO_05, VI_SHP_CORETH_05, VI_SHP_LIMIT_05_2, VI_SHP_LIMIT_RATIO_05, VI_SHP_BOUND_05},//H2
    {VI_SHP_GAIN_05, VI_SHP_CORE_05, VI_SHP_CORING_RATIO_05, VI_SHP_CORETH_05, VI_SHP_LIMIT_05_2, VI_SHP_LIMIT_RATIO_05, VI_SHP_BOUND_05},//H3
    {VI_SHP_GAIN_05, VI_SHP_CORE_05, VI_SHP_CORING_RATIO_05, VI_SHP_CORETH_05, VI_SHP_LIMIT_05_2, VI_SHP_LIMIT_RATIO_05, VI_SHP_BOUND_05},//V1
    {VI_SHP_GAIN_06, VI_SHP_CORE_06, VI_SHP_CORING_RATIO_06, VI_SHP_CORETH_06, VI_SHP_LIMIT_06_2, VI_SHP_LIMIT_RATIO_06, VI_SHP_BOUND_06},//V2
    {VI_SHP_GAIN_06, VI_SHP_CORE_06, VI_SHP_CORING_RATIO_06, VI_SHP_CORETH_06, VI_SHP_LIMIT_06_2, VI_SHP_LIMIT_RATIO_06, VI_SHP_BOUND_06},//V3
    {VI_SHP_GAIN_06, VI_SHP_CORE_06, VI_SHP_CORING_RATIO_06, VI_SHP_CORETH_06, VI_SHP_LIMIT_06_2, VI_SHP_LIMIT_RATIO_06, VI_SHP_BOUND_06},//D1
    {VI_SHP_GAIN_06, VI_SHP_CORE_06, VI_SHP_CORING_RATIO_06, VI_SHP_CORETH_06, VI_SHP_LIMIT_06_2, VI_SHP_LIMIT_RATIO_06, VI_SHP_BOUND_06},//D2
    {VI_SHP_GAIN_07, VI_SHP_CORE_07, VI_SHP_CORING_RATIO_07, VI_SHP_CORETH_07, VI_SHP_LIMIT_07, VI_SHP_LIMIT_RATIO_07, VI_SHP_BOUND_07},//D3
    //PBC2
    {VI_SHP_GAIN_07, VI_SHP_CORE_07, VI_SHP_CORING_RATIO_07, VI_SHP_CORETH_07, VI_SHP_LIMIT_07, VI_SHP_LIMIT_RATIO_07, VI_SHP_BOUND_07},//H1
    {VI_SHP_GAIN_07, VI_SHP_CORE_07, VI_SHP_CORING_RATIO_07, VI_SHP_CORETH_07, VI_SHP_LIMIT_07, VI_SHP_LIMIT_RATIO_07, VI_SHP_BOUND_07},//H2
    {VI_SHP_GAIN_07, VI_SHP_CORE_07, VI_SHP_CORING_RATIO_07, VI_SHP_CORETH_07, VI_SHP_LIMIT_07, VI_SHP_LIMIT_RATIO_07, VI_SHP_BOUND_07},//H3
    {VI_SHP_GAIN_08, VI_SHP_CORE_08, VI_SHP_CORING_RATIO_08, VI_SHP_CORETH_08, VI_SHP_LIMIT_08, VI_SHP_LIMIT_RATIO_08, VI_SHP_BOUND_08},//V1
    {VI_SHP_GAIN_08, VI_SHP_CORE_08, VI_SHP_CORING_RATIO_08, VI_SHP_CORETH_08, VI_SHP_LIMIT_08, VI_SHP_LIMIT_RATIO_08, VI_SHP_BOUND_08},//V2
    {VI_SHP_GAIN_08, VI_SHP_CORE_08, VI_SHP_CORING_RATIO_08, VI_SHP_CORETH_08, VI_SHP_LIMIT_08, VI_SHP_LIMIT_RATIO_08, VI_SHP_BOUND_08},//V3
    {VI_SHP_GAIN_08, VI_SHP_CORE_08, VI_SHP_CORING_RATIO_08, VI_SHP_CORETH_08, VI_SHP_LIMIT_08, VI_SHP_LIMIT_RATIO_08, VI_SHP_BOUND_08},//D1
    {VI_SHP_GAIN_09, VI_SHP_CORE_09, VI_SHP_CORING_RATIO_09, VI_SHP_CORETH_09, VI_SHP_LIMIT_09, VI_SHP_LIMIT_RATIO_09, VI_SHP_BOUND_09},//D2
    {VI_SHP_GAIN_09, VI_SHP_CORE_09, VI_SHP_CORING_RATIO_09, VI_SHP_CORETH_09, VI_SHP_LIMIT_09, VI_SHP_LIMIT_RATIO_09, VI_SHP_BOUND_09},//D3
    //PBC3
    {VI_SHP_GAIN_09, VI_SHP_CORE_09, VI_SHP_CORING_RATIO_09, VI_SHP_CORETH_09, VI_SHP_LIMIT_09, VI_SHP_LIMIT_RATIO_09, VI_SHP_BOUND_09},//H1
    {VI_SHP_GAIN_09, VI_SHP_CORE_09, VI_SHP_CORING_RATIO_09, VI_SHP_CORETH_09, VI_SHP_LIMIT_09, VI_SHP_LIMIT_RATIO_09, VI_SHP_BOUND_09},//H2
    {VI_SHP_GAIN_0A, VI_SHP_CORE_0A, VI_SHP_CORING_RATIO_0A, VI_SHP_CORETH_0A, VI_SHP_LIMIT_0A, VI_SHP_LIMIT_RATIO_0A, VI_SHP_BOUND_0A},//H3
    {VI_SHP_GAIN_0A, VI_SHP_CORE_0A, VI_SHP_CORING_RATIO_0A, VI_SHP_CORETH_0A, VI_SHP_LIMIT_0A, VI_SHP_LIMIT_RATIO_0A, VI_SHP_BOUND_0A},//V1
    {VI_SHP_GAIN_0A, VI_SHP_CORE_0A, VI_SHP_CORING_RATIO_0A, VI_SHP_CORETH_0A, VI_SHP_LIMIT_0A, VI_SHP_LIMIT_RATIO_0A, VI_SHP_BOUND_0A},//V2
    {VI_SHP_GAIN_0A, VI_SHP_CORE_0A, VI_SHP_CORING_RATIO_0A, VI_SHP_CORETH_0A, VI_SHP_LIMIT_0A, VI_SHP_LIMIT_RATIO_0A, VI_SHP_BOUND_0A},//V3
    {VI_SHP_GAIN_0B, VI_SHP_CORE_0B, VI_SHP_CORING_RATIO_0B, VI_SHP_CORETH_0B, VI_SHP_LIMIT_0B, VI_SHP_LIMIT_RATIO_0B, VI_SHP_BOUND_0B},//D1
    {VI_SHP_GAIN_0B, VI_SHP_CORE_0B, VI_SHP_CORING_RATIO_0B, VI_SHP_CORETH_0B, VI_SHP_LIMIT_0B, VI_SHP_LIMIT_RATIO_0B, VI_SHP_BOUND_0B},//D2
    {VI_SHP_GAIN_0B, VI_SHP_CORE_0B, VI_SHP_CORING_RATIO_0B, VI_SHP_CORETH_0B, VI_SHP_LIMIT_0B, VI_SHP_LIMIT_RATIO_0B, VI_SHP_BOUND_0B},//D3    
};

UINT32 bTDS_Curve_Points[TDS_BAND_NUM][TSD_CURVE_POINTSNUM];
UINT32 bTDS_Curve_Slopes[TDS_BAND_NUM][TSD_CURVE_SLOPESNUM];
UINT8 bTDS_UpdateFlg[TDS_BAND_NUM];

static REGTBL_T const REGTBL_HSHARP[] =
{
	{HSHARP_00, 0x804C4440, 0xF0FFFFFF},
	{HSHARP_01, 0x804C4440, 0x80FFFFFF},
	{HSHARP_02, 0x00020202, 0x70FFFFFF},
	{HSHARP_03, 0x00FFFFFF, 0x70FFFFFF},
	{HSHARP_04, 0x00FFFFFF, 0x70FFFFFF},
	{HSHARP_05, 0xF000FFFF, 0xFFC0FFFF},
	{HSHARP_06, 0x30000808, 0xF3FFFFFF},
	{HSHARP_07, 0x00444340, 0x00FFFFFF},
	{HSHARP_08, 0x00444340, 0x00FFFFFF},
	{HSHARP_09, 0x00010101, 0x70FFFFFF},
	{HSHARP_0A, 0x00FFFFFF, 0x70FFFFFF},
	{HSHARP_0B, 0x00FFFFFF, 0x70FFFFFF},
	{HSHARP_0C, 0x00001800, 0x00001800},
	{HSHARP_0E, 0x00000000, 0xFFFFFF00},
	{HSHARP_0F, 0x00000000, 0xFFFFFF0F},
	{HSHARP_10, 0x00800000, 0x1FFF0000},
	{REGTBL_END, 0x00000000, 0x00000000},
};

static REGTBL_T const REGTBL_TDSHARP[] =
{
	{SHARP_00, 0x00000000, 0x00000040},
	{SHARP_31, 0x00050000, 0x000F0000},
	{SHARP_32, 0x00020050, 0x0003F0FF},
	{SHARP_36, 0x00020000, 0x00021F1F},
    {SHARP_42, 0x0073F1F4, 0xFFFFFFFF},
    {SHARP_43, 0x10000000, 0x1F000000},
    {SHARP_44, 0x00001000, 0x00001F00},
    {SHARP_45, 0x80FF80FF, 0xFFFFFFFF},    
    {SHARP_46, 0x80FF80FF, 0xFFFFFFFF},  
    {SHARP_4C, 0x84284080, 0xBFFFFFC0},
    {SHARP_4D, 0x830C628A, 0xBFFFFFFF},
    {SHARP_4E, 0x28A20800, 0xFFFFFF00},
    {SHARP_5C, 0x80FF80FF, 0xFFFFFFFF},
    {SHARP_5D, 0x000080FF, 0x0000DFFF},
	{REGTBL_END, 0x00000000, 0x00000000}, 
};

static REGTBL_T const REGTBL_PBC[] =
{
	//PBC1 color range initial
	{CDS_00, 0x80800A20, 0x0000FFFF},
	{CDS_01, 0x7D048199, 0xFFFFFFFF},
	{CDS_02, 0xC2CAC080, 0xFFFFF0FF},
	{CDS_03, 0x00C03220, 0xFFF00000},
	//PBC2 color range initial
	{CDS_2A, 0x800C0A0A, 0x0000FFFF},
	{CDS_2B, 0x5406000C, 0xFFFFFFFF},
	{CDS_2C, 0x62C85080, 0xFFFFF0FF},
	{CDS_2D, 0x00C82020, 0xFFFFFFFF},
	//PBC3 color range initial
	{CDS_54, 0x800C0A0A, 0x0000FFFF},
	{CDS_55, 0xB806000C, 0xFFFFFFFF},
	{CDS_56, 0xE3987080, 0xFFFFF0FF},
	{CDS_57, 0x00C03220, 0xFFF00000},
	{REGTBL_END, 0x00000000, 0x00000000},
};

static REGTBL_T const REGTBL_ECTIF[] =
{	
	{ECTIF_00, 0x00001A10, 0xD0001A78},																																					
	{ECTIF_01, 0x00033714, 0x07777FFF},																																					
	{ECTIF_02, 0x1A0C0A01, 0xFFFFFF07},																																					
	{ECTIF_03, 0x1A0C0A01, 0xFFFFFF07},																																					
	{ECTIF_04, 0x0C080041, 0xFFFFFF77},																																					
	{ECTIF_05, 0x02000000, 0xFF000000},
	{REGTBL_END, 0x00000000, 0x00000000},
};

static REGTBL_T const REGTBL_ECTIR[] =
{
	{CTIR_00, 0x00001A61, 0x10001A79},
	{CTIR_01, 0x00003714, 0x07777FFF},
	{CTIR_02, 0x1A0C0A05, 0xFFFFFF07},
	{CTIR_03, 0x1A0C0A05, 0xFFFFFF07},
	{CTIR_04, 0x40000001, 0xFFFFFF77},
	{CTIR_05, 0x0000000B, 0x000000FF},
	{REGTBL_END, 0x00000000, 0x00000000},
};
#if defined(CC_MT5399)
static REGTBL_T const REGTBL_CBE[] = 
{
	{CBE_00, 0x00004040, 0x0F7F7F7F},
	{CBE_01, 0x00000416, 0x00007F7F},
	{CBE_02, 0x2020083F, 0x00007F7F},
	{REGTBL_END, 0x00000000, 0x00000000}
};
#endif
/*****************************************************************************************/
/*************************************   CTI   ******************************************/
/*****************************************************************************************/
void vDrvCTIFOnOff(UINT8 bOnOff)
{
	vIO32WriteFldAlign(ECTIF_00, bOnOff, ECTIF_ENA);
}

void vDrvCTIFInit(void)
{
	vDrvLoadRegTbl(REGTBL_ECTIF);
}

void vDrvCTIFParam(void)
{
	//No need to set this registers by quality table, this control by LGE HAL VPQ.
	return;
	
	vIO32WriteFldAlign(ECTIF_00, wReadQualityTable(QUALITY_CTIF_ENA),
	                  ECTIF_ENA);
	vIO32WriteFldAlign(ECTIF_00, wReadQualityTable(QUALITY_CTIF_HD),
	                  ECTIF_HD);
	vIO32WriteFldAlign(ECTIF_01, wReadQualityTable(QUALITY_CTIF_LPF1),
	                  ECTIF_LPF1);
	vIO32WriteFldAlign(ECTIF_01, wReadQualityTable(QUALITY_CTIF_LPF1_SEL),
	                  ECTIF_LPF1_SEL);
	vIO32WriteFldAlign(ECTIF_00, wReadQualityTable(QUALITY_CTIF_FLPF),
	                  ECTIF_FLPF);
	vIO32WriteFldAlign(ECTIF_00, wReadQualityTable(QUALITY_CTIF_FLPF_SEL),
	                  ECTIF_FLPF_SEL);
	vIO32WriteFldAlign(ECTIF_04, wReadQualityTable(QUALITY_CTIF_FLAT_GAIN),
	                  ECTIF_FLAT_GAIN);
	vIO32WriteFldAlign(ECTIF_04, wReadQualityTable(QUALITY_CTIF_FLAT_OFST1),
	                  ECTIF_FLAT_OFST1);
	vIO32WriteFldAlign(ECTIF_04, wReadQualityTable(QUALITY_CTIF_FLAT_OFST2),
	                  ECTIF_FLAT_OFST2);
	vIO32WriteFldAlign(ECTIF_02, wReadQualityTable(QUALITY_CTIF_U_WND_SZ),
	                  ECTIF_U_WND_SZ);
	vIO32WriteFldAlign(ECTIF_03, wReadQualityTable(QUALITY_CTIF_V_WND_SZ),
	                  ECTIF_V_WND_SZ);
	vIO32WriteFldAlign(ECTIF_02, wReadQualityTable(QUALITY_CTIF_U_STB_GAIN),
	                  ECTIF_U_STB_GAIN);
	vIO32WriteFldAlign(ECTIF_03, wReadQualityTable(QUALITY_CTIF_V_STB_GAIN),
	                  ECTIF_V_STB_GAIN);
	vIO32WriteFldAlign(ECTIF_02, wReadQualityTable(QUALITY_CTIF_U_STB_OFST1),
	                  ECTIF_U_STB_OFST1);
	vIO32WriteFldAlign(ECTIF_03, wReadQualityTable(QUALITY_CTIF_V_STB_OFST1),
	                  ECTIF_V_STB_OFST1);
	vIO32WriteFldAlign(ECTIF_02, wReadQualityTable(QUALITY_CTIF_U_STB_OFST2),
	                  ECTIF_U_STB_OFST2);
	vIO32WriteFldAlign(ECTIF_03, wReadQualityTable(QUALITY_CTIF_V_STB_OFST2),
	                  ECTIF_V_STB_OFST2);
	vIO32WriteFldAlign(ECTIF_05, wReadQualityTable(QUALITY_CTIF_COR),
	                  ECTIF_COR);                  
}

void vDrvCTIROnOff(UINT8 bOnOff)
{
    UINT8 bModuleOnOff = bOnOff && (!bForceAllPQOff) && wReadQualityTable(QUALITY_MODULE_ONOFF_CTI);           
	vIO32WriteFldAlign(CTIR_00, bModuleOnOff, CTIR_ENA);
}

void vDrvCTIRInit(void)
{
	vDrvLoadRegTbl(REGTBL_ECTIR);
}

void vDrvCTIRParam(void)
{
	//No need to set this registers by quality table, this control by LGE HAL VPQ.
	return;
	
	vIO32WriteFldAlign(CTIR_00, wReadQualityTable(QUALITY_CTIR_ENA),
	                  CTIR_ENA);
	vIO32WriteFldAlign(CTIR_00, wReadQualityTable(QUALITY_CTIR_HD),
	                  CTIR_HD);
	vIO32WriteFldAlign(CTIR_01, wReadQualityTable(QUALITY_CTIR_LPF1),
	                  CTIR_LPF1);
	vIO32WriteFldAlign(CTIR_01, wReadQualityTable(QUALITY_CTIR_LPF1_SEL),
	                  CTIR_LPF1_SEL);
	vIO32WriteFldAlign(CTIR_00, wReadQualityTable(QUALITY_CTIR_FLPF),
	                  CTIR_FLPF);
	vIO32WriteFldAlign(CTIR_00, wReadQualityTable(QUALITY_CTIR_FLPF_SEL),
	                  CTIR_FLPF_SEL);
	vIO32WriteFldAlign(CTIR_04, wReadQualityTable(QUALITY_CTIR_FLAT_GAIN),
	                  CTIR_FLAT_GAIN);
	vIO32WriteFldAlign(CTIR_04, wReadQualityTable(QUALITY_CTIR_FLAT_OFST1),
	                  CTIR_FLAT_OFST1);
	vIO32WriteFldAlign(CTIR_04, wReadQualityTable(QUALITY_CTIR_FLAT_OFST2),
	                  CTIR_FLAT_OFST2);
	vIO32WriteFldAlign(CTIR_02, wReadQualityTable(QUALITY_CTIR_U_WND_SZ),
	                  CTIR_U_WND_SZ);
	vIO32WriteFldAlign(CTIR_03, wReadQualityTable(QUALITY_CTIR_V_WND_SZ),
	                  CTIR_V_WND_SZ);
	vIO32WriteFldAlign(CTIR_02, wReadQualityTable(QUALITY_CTIR_U_STB_GAIN),
	                  CTIR_U_STB_GAIN);
	vIO32WriteFldAlign(CTIR_03, wReadQualityTable(QUALITY_CTIR_V_STB_GAIN),
	                  CTIR_V_STB_GAIN);
	vIO32WriteFldAlign(CTIR_02, wReadQualityTable(QUALITY_CTIR_U_STB_OFST1),
	                  CTIR_U_STB_OFST1);
	vIO32WriteFldAlign(CTIR_03, wReadQualityTable(QUALITY_CTIR_V_STB_OFST1),
	                  CTIR_V_STB_OFST1);
	vIO32WriteFldAlign(CTIR_02, wReadQualityTable(QUALITY_CTIR_U_STB_OFST2),
	                  CTIR_U_STB_OFST2);
	vIO32WriteFldAlign(CTIR_03, wReadQualityTable(QUALITY_CTIR_V_STB_OFST2),
	                  CTIR_V_STB_OFST2);
	vIO32WriteFldAlign(CTIR_05, wReadQualityTable(QUALITY_CTIR_COR),
	                  CTIR_COR);	                  
}

void vDrvInitHueTie(void)  //sync A2
{
    if (bGetSignalType(SV_VP_MAIN)==SV_ST_TV)
    {
        vRegWriteFldAlign(CTIR_00, 1, CTIR_HUE_TIE);
        vRegWriteFldAlign(ECTIF_00, 1, ECTIF_HUE_TIE);
    }
    else
    {
        vRegWriteFldAlign(CTIR_00, 0, CTIR_HUE_TIE);
        vRegWriteFldAlign(ECTIF_00, 0, ECTIF_HUE_TIE);
    }
}

/*****************************************************************************************/
/*************************************   CBE   ******************************************/
/*****************************************************************************************/
#if defined(CC_MT5399)
void vDrvCBEOnOff(UINT8 bOnOff)
{
    VRM_INFO_T in,out;
    UINT8 bLineInterleave;
	UINT8 bCBEBypass;
    UINT32   u4SCL  = u4DrvTDTVScalerModeQuery();
    E_TD_OUT eSCLOut= TD_SCL_OUT(u4SCL);
    E_TD_IN  eSCLIn = TD_SCL_IN(u4SCL);

    //scaler out line interleave or not
    bLineInterleave =((E_TD_OUT_3D_LI==eSCLOut)||
                     ((E_TD_IN_LI_P==eSCLIn)&&(E_TD_OUT_NATIVE==eSCLOut))); 

    //get fsc in & fsc out info
    VRMGetModuleVRMInfo(SV_VP_MAIN, VRM_MODULE_FSC, &in, &out);

    //Turn off this function when  front scaler out is
    //a. line interleave
    //b. width > 1920
    //c. height > 1080
    bCBEBypass = (out.u2Width > 1920  || 
                 out.u2Height > 1080 || 
                 bLineInterleave);
    vIO32WriteFldAlign(CBE_05, bCBEBypass, BYPASS_CBE);
    vIO32WriteFldAlign(CBE_00, bOnOff, CBEENABLE);
    vScpipUpdateDelay();
}

void vDrvCBEInit(void)
{
	vDrvLoadRegTbl(REGTBL_CBE);
}

void vDrvCBELoadParam(void)
{
	vIO32WriteFldAlign(CBE_00, wReadQualityTable(QUALITY_CBE_WEI_IT3), CBE_W_IT3);
	vIO32WriteFldAlign(CBE_00, wReadQualityTable(QUALITY_CBE_WEI_IT5), CBE_W_IT5);
}
#endif
/*****************************************************************************************/
/*************************************   AC Transform  ***************************************/
/*****************************************************************************************/
/**
 * @brief partialy update AC curve.
 * @param bType, bLimit, bLimit_Ratio, bBound
 * @dependency : Gain, Coring should be set before limit
 */
#define COR_RATIO_UNIT 0x10
#define GAIN_UNIT 0x20
#define LIMIT_RATIO_UNIT 0x8

static UINT8 bSetTDSharp(UINT8 bType, UINT8 bCor_Zero, UINT8 bCor_Ratio, UINT8 bCor_Th,
						 UINT8 bGain, UINT8 bLimit, UINT8 bLimit_Ratio, UINT8 bBound)
{
	TDSPoint P1, P2, P3, P4;
	INT32 wRatioS01, wRatioS12, wRatioS23, wRatioS34, wCor_Ratio, wLimit_Ratio, wGain;

	
	wCor_Ratio=(INT32)bCor_Ratio;
	wLimit_Ratio=(INT32)bLimit_Ratio;

	if(bType>=TDS_BAND_NUM)
	{	
		LOG(1, "\n\nError Type for sharpness Limit setting\n\n");
		return SV_FALSE;
	}
	LOG(2, "\n\n\nSharpness Parameters Input Type : %d\n", bType); 
	LOG(2, "bCor_Zero : %d, \n",bCor_Zero);
	LOG(2, "bCor_Ratio : %d, \n",bCor_Ratio);
	LOG(2, "bCor_Th : %d \n",bCor_Th);
	LOG(2, "bGain : %d \n",bGain);
	LOG(2, "bLimit : %d, \n",bLimit);
	LOG(2, "bLimit_Ratio : %d, \n",bLimit_Ratio);
	LOG(2, "bBound : %d, n", bBound);


	//resolution alignment with Cobra limit, bound (in 8 bit resolution)
	//bLimit=(bLimit>=128?0xFF:bLimit*2);
	//bBound=(bBound>=128?0xFF:bBound*2);

	/*Conditional check : 
	//a. bCor_Ratio, bLimit_Ratio must < unit ratio, and x gain//bCor_Ratio must < bGain
	//b. //bLimit_Ratio must < bGain
	//c. when bCor_Th < bCor_Zero, bCor_Zero works and bCor_Ratio 0
	*/
	if(bCor_Zero>=bCor_Th)
	{
		bCor_Th=bCor_Zero;
		wCor_Ratio=0;
	}
	if(bLimit>=bBound)
	{
		bLimit = bBound;
		wLimit_Ratio=0;
	}
	
	wGain=MIN(0x7F, bGain); // Max 4 times AC		
	wCor_Ratio=MIN(wCor_Ratio,COR_RATIO_UNIT);
	wLimit_Ratio=MIN(wLimit_Ratio,LIMIT_RATIO_UNIT);
	wCor_Ratio=(wGain*wCor_Ratio);
	wLimit_Ratio=(wGain*wLimit_Ratio);
	
	P1.bX = bCor_Zero;
	P1.bY = 0;
	P2.bX = bCor_Th;
	P2.bY = (((UINT16)bCor_Th-bCor_Zero)*wCor_Ratio*2+(COR_RATIO_UNIT*GAIN_UNIT))/(2*COR_RATIO_UNIT*GAIN_UNIT);
	if(P2.bY>=0xFF)
	{
		P2.bY=0xFF;
		wCor_Ratio=(0xFF*COR_RATIO_UNIT*GAIN_UNIT*2+((UINT16)bCor_Th-bCor_Zero))/(2*((UINT16)bCor_Th-bCor_Zero));
	}

	//resolution alignment with P2, P3
	//P1, P2 is 8 bit LSB in 12 bit resolution, P3, P4 is 8 bit LSB in 9 bit resolution
	if(wGain)
	{
		bLimit=MAX(bLimit, (P2.bY>>3));
		P3.bX = (UINT16)(bCor_Th>>3) + ((UINT16)(bLimit-(P2.bY>>3))*GAIN_UNIT*2+wGain)/(2*wGain);
		P3.bY = bLimit;
	}
	else
	{
		P3.bX = bLimit;
		P3.bY = P2.bY>>3;
	}
	if(wLimit_Ratio)
	{
		P4.bX = P3.bX+((bBound-bLimit)*LIMIT_RATIO_UNIT*GAIN_UNIT*2+wLimit_Ratio)/(2*wLimit_Ratio);
		P4.bY = bBound;
	}
	else
	{
		P4.bX=0xFF;
		P4.bY=P3.bY;
	}

	//Range check.
	if(P3.bX>0xFF)
	{
		P3.bX=0xFF;
		P4.bX=0xFF;
		P3.bY=(P2.bY>>3)+((UINT16)(0xFF-(bCor_Th>>3))*wGain*2+GAIN_UNIT)/(2*GAIN_UNIT);
		P4.bY=P3.bY;
		wLimit_Ratio=0;
	}
	else if(P4.bX>0xFF)
	{
		P4.bX=0xFF;
		P4.bY=P3.bY+(wLimit_Ratio*(0xFF-P3.bX)*2+(LIMIT_RATIO_UNIT*GAIN_UNIT))/(2*LIMIT_RATIO_UNIT*GAIN_UNIT);
	}

	wRatioS01 = 0;
	wRatioS12 = (wCor_Ratio*4096+COR_RATIO_UNIT*GAIN_UNIT)/(2*COR_RATIO_UNIT*GAIN_UNIT);
	wRatioS23 = (wGain*4096+GAIN_UNIT)/(2*GAIN_UNIT);
	wRatioS34 = (wLimit_Ratio*4096+LIMIT_RATIO_UNIT*GAIN_UNIT)/(2*LIMIT_RATIO_UNIT*GAIN_UNIT);

	//Protection
	vIO32WriteFldAlign(TDSHP_GLOBAL, SV_ON, TDS_WRT_PROTECT);
	
	bTDS_Curve_Points[bType][0]=(P1.bX<<24)+(P1.bY<<16)+(P2.bX<<8)+(P2.bY);
	bTDS_Curve_Points[bType][1]=(P3.bX<<24)+(P3.bY<<16)+(P4.bX<<8)+(P4.bY);
	
	bTDS_Curve_Slopes[bType][0]=(wRatioS01<<16)+wRatioS12;
	bTDS_Curve_Slopes[bType][1]=(wRatioS23<<16)+wRatioS34;

	bTDS_UpdateFlg[bType] = SV_ON;
	
	vIO32WriteFldAlign(TDSHP_GLOBAL, SV_OFF, TDS_WRT_PROTECT);
	vApiRegisterVideoEvent(PE_EVENT_SHARPNESS, SV_VP_MAIN, SV_ON);
	
	LOG(2, "Set Sharpness Parameters\n"); 
	LOG(2, "Pt1 : %d, %d, \n",P1.bX,P1.bY); 
	LOG(2, "Pt2 : %d, %d, \n",P2.bX,P2.bY); 
	LOG(2, "Pt3 : %d  %d, \n",P3.bX,P3.bY); 
	LOG(2, "Pt4 : %d  %d, \n",P4.bX,P4.bY); 
	LOG(2, "S01 : %d, \n",wRatioS01); 
	LOG(2, "S12 : %d, \n",wRatioS12); 
	LOG(2, "S23 : %d, \n",wRatioS23); 
	LOG(2, "S34 : %d, \n\n\n", wRatioS34);
 
	return SV_TRUE;
}
UINT8 vDrvTDSharpOffset(UINT8 bBand, UINT8 bItem, UINT8 bValue)
{
	UINT8 bOffset; 
	UINT8 bResult;
	
	if((bBand > TDS_D3) || (bItem > TDS_CURVE_SW_REG_COR_ZERO))
	{
		return bValue;
	}
	
	if(bBand < TDS_V1)
	{
		bOffset = (UINT8)(u4RegRead4B(u4SWReg(VI_SHP_EXT_OFST_00 + bItem*12)) >> bBand*8 );
	}
	else if(bBand < TDS_D1)
	{
		bOffset = (UINT8)(u4RegRead4B(u4SWReg(VI_SHP_EXT_OFST_01 + bItem*12)) >> (bBand - TDS_V1)*8 );
	}
	else
	{
		bOffset = (UINT8)(u4RegRead4B(u4SWReg(VI_SHP_EXT_OFST_02 + bItem*12)) >> (bBand - TDS_D1)*8 );
	}

	bResult = CLIP((bValue + bOffset - 0x80), 0, 0xFF);
	
    return bResult;
		
}

// This function is called at VSYNC to update TDS curve.
void vDrvLoadTDSCurve(void)
{
	UINT8 bType, bWriteRdy;
	UINT32 dwBaseAddr;
	
	bWriteRdy = (!IO32ReadFldAlign(TDSHP_GLOBAL, TDS_WRT_PROTECT));
    
	if(bWriteRdy)
	{
		for(bType=0; bType<TDS_BAND_NUM; bType++)
		{
			dwBaseAddr = TDS_CURVE_REGADDR[bType];
            
			if(bTDS_UpdateFlg[bType]==SV_ON)
			{
				vIO32Write4B((dwBaseAddr+0x0),  bTDS_Curve_Points[bType][0]);
				vIO32Write4B((dwBaseAddr+0x4),  bTDS_Curve_Points[bType][1]);	
				vIO32Write4B((dwBaseAddr+0x8),  bTDS_Curve_Slopes[bType][0]);
				vIO32Write4B((dwBaseAddr+0xC),  bTDS_Curve_Slopes[bType][1]); 
			
				bTDS_UpdateFlg[bType]=SV_OFF;
			}
		}
	}	
}

static void vDrvSharpCalcuACCurve(UINT8 bStartBand, UINT8 bEndBand)
{
	static UINT8 bPreSetting[TDS_BAND_NUM][TDS_CURVE_SW_REG_END];

    UINT8 bSetting[TDS_BAND_NUM][TDS_CURVE_SW_REG_END];

    UINT8 bBand, bItem, i, bGlobalCoring;

	static UINT8 bPreGlobalCoring = 0;

	bGlobalCoring = IO32ReadFldAlign(TDSHP_GLOBAL, TDS_GLOBAL_CORING);

    if (bStartBand > bEndBand)
    {
        LOG(2, "SharpCalc have wrong param, please check, bStartBand = %d, bEndBand = %d\n", bStartBand, bEndBand);
        return;
    }
    if (bEndBand >= TDS_ITEM_MAX)
    {
        LOG(2, "SharpCalc have wrong param, please check.bEndBand = %d is too large\n", bEndBand);
    }
    
    for (bBand=bStartBand; bBand<=bEndBand; bBand++)
    {
        for (bItem=0; bItem<TDS_CURVE_SW_REG_END; bItem++)
        {
            bSetting[bBand][bItem] = IO32ReadFldAlign(u4SWReg(TDS_Curve_SW_Reg_Base[bBand][bItem]), TDS_Curve_SW_Reg[bBand][bItem]);
			bSetting[bBand][bItem] = vDrvTDSharpOffset(bBand,bItem,bSetting[bBand][bItem]);
        }
    }

    for (bBand=bStartBand; bBand<=bEndBand; bBand++)
    {
        for (bItem=0; bItem<TDS_CURVE_SW_REG_END; bItem++)
        {
            if ((bSetting[bBand][bItem] != bPreSetting[bBand][bItem]) || (( bGlobalCoring != bPreGlobalCoring) && (bBand <= TDS_LTI_V)))
            {
                bSetTDSharp(bBand,MIN (0xFF,(bSetting[bBand][TDS_CURVE_SW_REG_COR_ZERO] + bGlobalCoring)), 
                                bSetting[bBand][TDS_CURVE_SW_REG_COR_RATIO], 
                                bSetting[bBand][TDS_CURVE_SW_REG_COR_TH], 
                                bSetting[bBand][TDS_CURVE_SW_REG_GAIN],
                                bSetting[bBand][TDS_CURVE_SW_REG_LIMIT], 
                                bSetting[bBand][TDS_CURVE_SW_REG_LIMIT_RATIO], 
                                bSetting[bBand][TDS_CURVE_SW_REG_LIMIT_BOUND]);
                
                for (i=0; i<TDS_CURVE_SW_REG_END; i++)
                {
                    bPreSetting[bBand][i] = bSetting[bBand][i];
                }
				bPreGlobalCoring = bGlobalCoring;
                break;
            }
        }
    }	
}

static void vDrv3DSharpParamCalcu(void)
{
	static UINT8 bPre3DGainH = 0x80;
	static UINT8 bPre3DGainD = 0x80;
	static UINT8 bPre3DGainV = 0x80;
	static UINT8 bPre3DClipH = 0x80;
	static UINT8 bPre3DClipD = 0x80;
	static UINT8 bPre3DClipV = 0x80;	
	UINT8 b3DHGain, b3DDGain, b3DVGain;
	UINT8 b3DHClip, b3DDClip, b3DVClip;
	
	vDrvGetSharp3DWeighting();
	b3DHGain = IO32ReadFldAlign(SHARPNESS_3D_13, SHARPNESS_3D_HGAIN);
	b3DDGain = IO32ReadFldAlign(SHARPNESS_3D_13, SHARPNESS_3D_DGAIN);
	b3DVGain = IO32ReadFldAlign(SHARPNESS_3D_13, SHARPNESS_3D_VGAIN);
	b3DHClip = IO32ReadFldAlign(SHARPNESS_3D_14, SHARPNESS_3D_HCLIP);
	b3DDClip = IO32ReadFldAlign(SHARPNESS_3D_14, SHARPNESS_3D_DCLIP);
	b3DVClip = IO32ReadFldAlign(SHARPNESS_3D_14, SHARPNESS_3D_VCLIP);
	if( (b3DHGain!=bPre3DGainH)||
		(b3DDGain!=bPre3DGainD)||
		(b3DVGain!=bPre3DGainV))
	{
		vDrvPostSharpParamHorizontal();
		vDrvPostSharpParamVertical();
		vDrvPostSharpParamDiagonal();
		bPre3DGainH = b3DHGain;
		bPre3DGainD = b3DDGain;
		bPre3DGainV = b3DVGain;
	}
    
	if(	(bPre3DClipH!=b3DHClip)||
		(bPre3DClipD!=b3DDClip)||
		(bPre3DClipV!=b3DVClip))
	{
		//Group Clip	   
		vIO32WriteFldAlign(SHARP_45, wDrvGetTDShp3DGain(wReadQualityTable(QUALITY_TDSHARP_CLIP_RATIO_H),b3DHClip)
								   , TDS_CLIP_RATIO_LH);			  
		vIO32WriteFldAlign(SHARP_45, wDrvGetTDShp3DGain(wReadQualityTable(QUALITY_TDSHARP_CLIP_RATIO_D),b3DDClip)
								   , TDS_CLIP_RATIO_MD); 
		vIO32WriteFldAlign(SHARP_46, wDrvGetTDShp3DGain(wReadQualityTable(QUALITY_TDSHARP_CLIP_RATIO_V),b3DVClip)
								   , TDS_CLIP_RATIO_HV);

		bPre3DClipH = b3DHClip;
		bPre3DClipD = b3DDClip;
		bPre3DClipV = b3DVClip;
	}
}


//**   Remove LTI
/*****************************************************************************************/
/*************************************   LTI  ***********************************************/
/*****************************************************************************************/
void vDrvLTIOnOff(UINT8 bOnOff)
{
    UINT8 bModuleOnOff = bOnOff && (!bForceAllPQOff) && wReadQualityTable(QUALITY_MODULE_ONOFF_LTI);                
	vIO32WriteFldAlign(SHARP_00, (!bModuleOnOff), TDS_DISABLE_LTI_V);	 
	vIO32WriteFldAlign(SHARP_00, (!bModuleOnOff), TDS_DISABLE_LTI_H1);	 
	vIO32WriteFldAlign(SHARP_00, (!bModuleOnOff), TDS_DISABLE_LTI_H2);		
}

void vDrvLTIParam(void)
{
	//No need to set this regster by mtk fw, this control by LGE HAL VPQ
	return;
	
	vIO32WriteFldMulti(SHARP_00, P_Fld(wReadQualityTable(QUALITY_LTI_FLT_LTI_H1), TDS_FLT_LTI_H1)|
								 P_Fld(wReadQualityTable(QUALITY_LTI_FLT_LTI_H2), TDS_FLT_LTI_H2)|
								 P_Fld((IO32ReadFldAlign(SHARP_00, TDS_FLT_V_3D)==0?wReadQualityTable(QUALITY_LTI_FLT_LTI_V):2), TDS_FLT_LTI_V));


	vIO32WriteFldMulti(SHARP_5C, P_Fld(wReadQualityTable(QUALITY_LTI_CLIP_RATIO_LTI_H1), TDS_CLIP_RATIO_LTI_H1)|
								 P_Fld(wReadQualityTable(QUALITY_LTI_CLIP_RATIO_LTI_H2), TDS_CLIP_RATIO_LTI_H2)|
								 P_Fld(wReadQualityTable(QUALITY_LTI_CLIP_THR_LTI_H1), TDS_CLIP_THR_LTI_H1)|
								 P_Fld(wReadQualityTable(QUALITY_LTI_CLIP_THR_LTI_H2), TDS_CLIP_THR_LTI_H2));

	vIO32WriteFldMulti(SHARP_46, P_Fld(wReadQualityTable(QUALITY_LTI_CLIP_RATIO_LTI_V), TDS_CLIP_RATIO_LTI_V)|
								 P_Fld(wReadQualityTable(QUALITY_LTI_CLIP_THR_LTI_V), TDS_CLIP_THR_LTI_V));
    
    //SW Proc
    vIO32WriteFldAlign(TDSHP_GAIN_04, wReadQualityTable(QUALITY_LTI_GAIN_LTI_H1), TDSHP_GAIN_LTI_H1);
    vIO32WriteFldAlign(TDSHP_GAIN_04, wReadQualityTable(QUALITY_LTI_GAIN_LTI_H2), TDSHP_GAIN_LTI_H2);
    vIO32WriteFldAlign(TDSHP_GAIN_04, wReadQualityTable(QUALITY_LTI_GAIN_LTI_V), TDSHP_GAIN_LTI_V);
    
    vIO32WriteFldAlign(TDSHP_CORE_04, wReadQualityTable(QUALITY_LTI_COR_ZERO_LTI_H1), TDSHP_CORE_LTI_H1);
    vIO32WriteFldAlign(TDSHP_CORE_04, wReadQualityTable(QUALITY_LTI_COR_ZERO_LTI_H2), TDSHP_CORE_LTI_H2);
    vIO32WriteFldAlign(TDSHP_CORE_04, wReadQualityTable(QUALITY_LTI_COR_ZERO_LTI_V), TDSHP_CORE_LTI_V);

    vIO32WriteFldAlign(TDSHP_CORING_RATIO_04, wReadQualityTable(QUALITY_LTI_COR_RATIO_LTI_H1), TDSHP_CORE_RATIO_LTI_H1);
    vIO32WriteFldAlign(TDSHP_CORING_RATIO_04, wReadQualityTable(QUALITY_LTI_COR_RATIO_LTI_H2), TDSHP_CORE_RATIO_LTI_H2);
    vIO32WriteFldAlign(TDSHP_CORING_RATIO_04, wReadQualityTable(QUALITY_LTI_COR_RATIO_LTI_V), TDSHP_CORE_RATIO_LTI_V);

    vIO32WriteFldAlign(TDSHP_CORETH_04, wReadQualityTable(QUALITY_LTI_COR_THR_LTI_H1), TDSHP_CORETH_LTI_H1);
    vIO32WriteFldAlign(TDSHP_CORETH_04, wReadQualityTable(QUALITY_LTI_COR_THR_LTI_H2), TDSHP_CORETH_LTI_H2);
    vIO32WriteFldAlign(TDSHP_CORETH_04, wReadQualityTable(QUALITY_LTI_COR_THR_LTI_V), TDSHP_CORETH_LTI_V);

    vIO32WriteFldAlign(TDSHP_LIMIT_02, wReadQualityTable(QUALITY_LTI_LIMIT_LTI_H1), LTI_LIMIT_LTI_H1);
    vIO32WriteFldAlign(TDSHP_LIMIT_02, wReadQualityTable(QUALITY_LTI_LIMIT_LTI_H2), LTI_LIMIT_LTI_H2);
    vIO32WriteFldAlign(TDSHP_LIMIT_02, wReadQualityTable(QUALITY_LTI_LIMIT_LTI_V), LTI_LIMIT_LTI_V);

    vIO32WriteFldAlign(TDSHP_LIMIT_05, wReadQualityTable(QUALITY_LTI_BOUND_LTI_H1), LTI_BOUND_LTI_H1);
    vIO32WriteFldAlign(TDSHP_LIMIT_05, wReadQualityTable(QUALITY_LTI_BOUND_LTI_H2), LTI_BOUND_LTI_H2);
    vIO32WriteFldAlign(TDSHP_LIMIT_05, wReadQualityTable(QUALITY_LTI_BOUND_LTI_V), LTI_BOUND_LTI_V);

    vIO32WriteFldAlign(TDSHP_LIMIT_RATIO_04, wReadQualityTable(QUALITY_LTI_LIMIT_RATIO_LTI_H1), TDSHP_LIMIT_RATIO_LTI_H1);
    vIO32WriteFldAlign(TDSHP_LIMIT_RATIO_04, wReadQualityTable(QUALITY_LTI_LIMIT_RATIO_LTI_H2), TDSHP_LIMIT_RATIO_LTI_H2);
    vIO32WriteFldAlign(TDSHP_LIMIT_RATIO_04, wReadQualityTable(QUALITY_LTI_LIMIT_RATIO_LTI_V), TDSHP_LIMIT_RATIO_LTI_V);
}

/*****************************************************************************************/
/***************************************  HSHARP *****************************************/
/****************************************************************************************/
void vDrvPreSharpOnOff(UINT8 bPath, UINT8 bOnOff)
{
    UINT8 bModuleOnOff = bOnOff && (!bForceAllPQOff) && wReadQualityTable(QUALITY_MODULE_ONOFF_SHARPNESS);            
    UNUSED(bPath);        
    vIO32WriteFldAlign(HSHARP_00, bModuleOnOff, SHARP_EN);
}

void vDrvPreSharpInit(void)
{
    vDrvLoadRegTbl(REGTBL_HSHARP);
}

void vDrvPreSharpParam(UINT8 bPath)
{
	//No need to set this regster by mtk fw, this control by LGE HAL VPQ
	return;
//  Global Gain
	vIO32WriteFldAlign(HSHARP_00 ,
	                  wReadQualityTable(QUALITY_HSHARP_HIGH), SHARP_HIGH);
	vIO32WriteFldAlign(HSHARP_00 ,
	                  wReadQualityTable(QUALITY_HSHARP_MID), SHARP_MID);
	vIO32WriteFldAlign(HSHARP_00 ,
	                  wReadQualityTable(QUALITY_HSHARP_LOW), SHARP_LOW);
	vIO32WriteFldAlign(HSHARP_01 ,
	                  wReadQualityTable(QUALITY_HSHARP_HIGH_NEG), SHARP_HIGH_NEG);
	vIO32WriteFldAlign(HSHARP_01 ,
	                  wReadQualityTable(QUALITY_HSHARP_MID_NEG), SHARP_MID_NEG);
	vIO32WriteFldAlign(HSHARP_01 ,
	                  wReadQualityTable(QUALITY_HSHARP_LOW_NEG), SHARP_LOW_NEG);

//  Global Coring
	vIO32WriteFldAlign(HSHARP_02 ,
	                  wReadQualityTable(QUALITY_HSHARP_HIGH_CORING),
	                  SHARP_HIGH_CORING);
	vIO32WriteFldAlign(HSHARP_02 ,
	                  wReadQualityTable(QUALITY_HSHARP_MID_CORING),
	                  SHARP_MID_CORING);
	vIO32WriteFldAlign(HSHARP_02 ,
	                  wReadQualityTable(QUALITY_HSHARP_LOW_CORING),
	                  SHARP_LOW_CORING);

//  Global Limit
	vIO32WriteFldAlign(HSHARP_03 ,
	                  wReadQualityTable(QUALITY_HSHARP_HIGH_LIMIT_POS),
	                  SHARP_HIGH_LIMIT_POS);
	vIO32WriteFldAlign(HSHARP_03 ,
	                  wReadQualityTable(QUALITY_HSHARP_MID_LIMIT_POS),
	                  SHARP_MID_LIMIT_POS);
	vIO32WriteFldAlign(HSHARP_03 ,
	                  wReadQualityTable(QUALITY_HSHARP_LOW_LIMIT_POS),
	                  SHARP_LOW_LIMIT_POS);
	vIO32WriteFldAlign(HSHARP_04 ,
	                  wReadQualityTable(QUALITY_HSHARP_HIGH_LIMIT_NEG),
	                  SHARP_HIGH_LIMIT_NEG);
	vIO32WriteFldAlign(HSHARP_04 ,
	                  wReadQualityTable(QUALITY_HSHARP_MID_LIMIT_NEG),
	                  SHARP_MID_LIMIT_NEG);
	vIO32WriteFldAlign(HSHARP_04 ,
	                  wReadQualityTable(QUALITY_HSHARP_LOW_LIMIT_NEG),
	                  SHARP_LOW_LIMIT_NEG);

//  Global Clip
	vIO32WriteFldAlign(HSHARP_05 ,
	                  wReadQualityTable(QUALITY_HSHARP_CLIP_POS_EN),
	                  CLIP_POS_EN);
	vIO32WriteFldAlign(HSHARP_05 ,
	                  wReadQualityTable(QUALITY_HSHARP_CLIP_NEG_EN),
	                  CLIP_NEG_EN);
	vIO32WriteFldAlign(HSHARP_05 ,
	                  wReadQualityTable(QUALITY_HSHARP_GB_CLIP_POS_EN_H),
	                  GB_CLIP_POS_EN_H);
	vIO32WriteFldAlign(HSHARP_05 ,
	                  wReadQualityTable(QUALITY_HSHARP_GB_CLIP_NEG_EN_H),
	                  GB_CLIP_NEG_EN_H);
	vIO32WriteFldAlign(HSHARP_05 ,
	                  wReadQualityTable(QUALITY_HSHARP_GB_CLIP_POS_EN_M),
	                  GB_CLIP_POS_EN_M);
	vIO32WriteFldAlign(HSHARP_05 ,
	                  wReadQualityTable(QUALITY_HSHARP_GB_CLIP_NEG_EN_M),
	                  GB_CLIP_NEG_EN_M);
	vIO32WriteFldAlign(HSHARP_05 ,
	                  wReadQualityTable(QUALITY_HSHARP_GB_CLIP_POS_EN_L),
	                  GB_CLIP_POS_EN_L);
	vIO32WriteFldAlign(HSHARP_05 ,
	                  wReadQualityTable(QUALITY_HSHARP_GB_CLIP_NEG_EN_L),
	                  GB_CLIP_NEG_EN_L);
	vIO32WriteFldAlign(HSHARP_06 ,
	                  wReadQualityTable(QUALITY_HSHARP_CLIP_FRONT_THR_HIGH),
	                  CLIP_FRONT_THR_HIGH);
	vIO32WriteFldAlign(HSHARP_06 ,
	                  wReadQualityTable(QUALITY_HSHARP_CLIP_FRONT_THR_LOW),
	                  CLIP_FRONT_THR_LOW);
	vIO32WriteFldAlign(HSHARP_05 ,
	                  wReadQualityTable(QUALITY_HSHARP_CLIP_POS),
	                  CLIP_POS_TH);
	vIO32WriteFldAlign(HSHARP_05 ,
	                  wReadQualityTable(QUALITY_HSHARP_CLIP_NEG),
	                  CLIP_NEG_TH);
	vIO32WriteFldAlign(HSHARP_10 ,
	                  wReadQualityTable(QUALITY_HSHARP_SOFT_CLIP_GAIN),
	                  SHARP_SOFT_GAIN);
	

//  Local Gain
	vIO32WriteFldAlign(HSHARP_07 ,
	                  wReadQualityTable(QUALITY_HSHARP_LC_HIGH),
	                  LC_SHARP_HIGH);
	vIO32WriteFldAlign(HSHARP_07 ,
	                  wReadQualityTable(QUALITY_HSHARP_LC_MID), LC_SHARP_MID);
	vIO32WriteFldAlign(HSHARP_07 ,
	                  wReadQualityTable(QUALITY_HSHARP_LC_LOW), LC_SHARP_LOW);
	vIO32WriteFldAlign(HSHARP_08 ,
	                  wReadQualityTable(QUALITY_HSHARP_LC_HIGH_NEG),
	                  LC_SHARP_HIGH_NEG);
	vIO32WriteFldAlign(HSHARP_08 ,
	                  wReadQualityTable(QUALITY_HSHARP_LC_MID_NEG), LC_SHARP_MID_NEG);
	vIO32WriteFldAlign(HSHARP_08 ,
	                  wReadQualityTable(QUALITY_HSHARP_LC_LOW_NEG), LC_SHARP_LOW_NEG);
	
//  Local Coring
	vIO32WriteFldAlign(HSHARP_09 ,
	                  wReadQualityTable(QUALITY_HSHARP_LC_HIGH_CORING),
	                  LC_SHARP_HIGH_CORING);
	vIO32WriteFldAlign(HSHARP_09 ,
	                  wReadQualityTable(QUALITY_HSHARP_LC_MID_CORING),
	                  LC_SHARP_MID_CORING);
	vIO32WriteFldAlign(HSHARP_09 ,
	                  wReadQualityTable(QUALITY_HSHARP_LC_LOW_CORING),
	                  LC_SHARP_LOW_CORING);

//  Local Limit
	vIO32WriteFldAlign(HSHARP_0A,
	                  wReadQualityTable(QUALITY_HSHARP_LC_HIGH_LIMIT_POS),
	                  LC_SHARP_HIGH_LIMIT_POS);
	vIO32WriteFldAlign(HSHARP_0A ,
	                  wReadQualityTable(QUALITY_HSHARP_LC_MID_LIMIT_POS),
	                  LC_SHARP_MID_LIMIT_POS);
	vIO32WriteFldAlign(HSHARP_0A ,
	                  wReadQualityTable(QUALITY_HSHARP_LC_LOW_LIMIT_POS),
	                  LC_SHARP_LOW_LIMIT_POS);
	vIO32WriteFldAlign(HSHARP_0B ,
	                  wReadQualityTable(QUALITY_HSHARP_LC_HIGH_LIMIT_NEG),
	                  LC_SHARP_HIGH_LIMIT_NEG);
	vIO32WriteFldAlign(HSHARP_0B ,
	                  wReadQualityTable(QUALITY_HSHARP_LC_MID_LIMIT_NEG),
	                  LC_SHARP_MID_LIMIT_NEG);
	vIO32WriteFldAlign(HSHARP_0B ,
	                  wReadQualityTable(QUALITY_HSHARP_LC_LOW_LIMIT_NEG),
	                  LC_SHARP_LOW_LIMIT_NEG);

//  Local Clip
	vIO32WriteFldAlign(HSHARP_05 ,
	                  wReadQualityTable(QUALITY_HSHARP_LC_CLIP_POS_EN),
	                  LC_CLIP_POS_EN);
	vIO32WriteFldAlign(HSHARP_05 ,
	                  wReadQualityTable(QUALITY_HSHARP_LC_CLIP_NEG_EN),
	                  LC_CLIP_NEG_EN);

//  Local Table Setting
	vIO32WriteFldAlign(HSHARP_06 ,
	                  wReadQualityTable(QUALITY_HSHARP_LC_TABLE_GAIN),
	                  LC_TABLE_GAIN);
	vIO32WriteFldAlign(HSHARP_06 ,
	                  wReadQualityTable(QUALITY_HSHARP_LC_TABLE_SEL),
	                  LC_TABLE_SEL);
	vIO32WriteFldAlign(HSHARP_06 ,
	                  wReadQualityTable(QUALITY_HSHARP_LC_TABLE_OFFSET),
	                  LC_TABLE_OFFSET);

//  FLPF
	vIO32WriteFldAlign(HSHARP_0F ,
	                  wReadQualityTable(QUALITY_HSHARP_AC_LPF_EN),
	                  SHARP_AC_EN);
	vIO32WriteFldAlign(HSHARP_10 ,
	                  wReadQualityTable(QUALITY_HSHARP_AC_LPF_COEF),
	                  SHARP_AC_COEF);
}

/*****************************************************************************************/
/************************************  TDSHARP *******************************************/
/*****************************************************************************************/
void vDrvPostSharpOnOff(UINT8 bOnOff)
{
	VRM_INFO_T in,out;
    UINT8 bNoVSHP; 
    UINT8 bLineInterleave;

    UINT32   u4SCL  = u4DrvTDTVScalerModeQuery();
    E_TD_OUT eSCLOut= TD_SCL_OUT(u4SCL);
    E_TD_IN  eSCLIn = TD_SCL_IN(u4SCL);

    //scaler out line interleave or not
    bLineInterleave = ((E_TD_OUT_3D_LI==eSCLOut) ||
                     ((E_TD_IN_LI_P==eSCLIn) && (E_TD_OUT_NATIVE==eSCLOut))); 
    
    vDrvLIPostSharpOnOff(bLineInterleave); 	

    //get fsc in & fsc out info
    VRMGetModuleVRMInfo(SV_VP_MAIN, VRM_MODULE_FSC, &in, &out);
	
    bNoVSHP = (out.u2Width > 1920);
    vIO32WriteFldAlign(TDPROC_MISC_00, bNoVSHP, NO_VERTICAL_SHARP);	    

    vIO32WriteFldAlign(SHARP_00, bOnOff, TDS_EN);
	//vIO32WriteFldAlign(CDS_00, bOnOff, CDS1_EN);
    //vIO32WriteFldAlign(CDS_2A, bOnOff, CDS2_EN);
	//vIO32WriteFldAlign(CDS_54, bOnOff, CDS3_EN);
}

void vDrvLIPostSharpOnOff(UINT8 bOnOff)
{
	vIO32WriteFldAlign(SHARP_00, (bOnOff?1:0), TDS_FLT_V_3D);
}

void vDrvPostSharpHorizontalOnOff(UINT8 bOnOff)
{

}

void vDrvPostSharpVerticalOnOff(UINT8 bOnOff)
{

}


void vDrvPostSharpDiagonalOnOff(UINT8 bOnOff)
{
	
}

/**
 * @brief Check Post sharpness parameters Constraint
 * @param
 * @dependency This function should be called by vApiVideoSharpness
 */
#define SAT_PROC_MAX 0x10
#define EDGE_DETAIL_MIN 0x10
#define EDGE_DETAIL_MAX 0x60

void vDrvTDSConstraint(void)
{
	UINT8 bV1Gain, bV2Gain, bEdgeGain, bDetailGain;

	//|V2-V1| <=0x20
	bV1Gain = wReadQualityTable(QUALITY_TDSHARP_GAIN_V1);
	bV2Gain = wReadQualityTable(QUALITY_TDSHARP_GAIN_V2);
	if(AbsDiff(bV1Gain, bV2Gain)>0x20)
	{
		printf("\n\nSharpness V1 Gain and V2 Gain Difference too large, cause side effect !!\n\n");
	}

	//Edge-detail Gain
	bEdgeGain = wReadQualityTable(QUALITY_TDSHARP_EDGE_GAIN_H);
	bDetailGain = wReadQualityTable(QUALITY_TDSHARP_DETAIL_GAIN_H);
	if(bEdgeGain >= EDGE_DETAIL_MAX ||
	   bEdgeGain <= EDGE_DETAIL_MIN ||
	   bDetailGain >= EDGE_DETAIL_MAX ||
	   bDetailGain <=EDGE_DETAIL_MIN)
	{
		printf("\n\nSharpness H EdgeGain and DetailGain danger value !!\n\n");
		wWriteQualityTable(QUALITY_TDSHARP_EDGE_GAIN_H, CLIP(bEdgeGain, EDGE_DETAIL_MIN, EDGE_DETAIL_MAX));
		wWriteQualityTable(QUALITY_TDSHARP_DETAIL_GAIN_H, CLIP(bDetailGain, EDGE_DETAIL_MIN, EDGE_DETAIL_MAX));		
	}	
	
	bEdgeGain = wReadQualityTable(QUALITY_TDSHARP_EDGE_GAIN_D);
	bDetailGain = wReadQualityTable(QUALITY_TDSHARP_DETAIL_GAIN_D);
	if(bEdgeGain >= EDGE_DETAIL_MAX ||
	   bEdgeGain <= EDGE_DETAIL_MIN ||
	   bDetailGain >= EDGE_DETAIL_MAX ||
	   bDetailGain <=EDGE_DETAIL_MIN)
	{
		printf("\n\nSharpness D EdgeGain and DetailGain danger value !!\n\n");	
		wWriteQualityTable(QUALITY_TDSHARP_EDGE_GAIN_D, CLIP(bEdgeGain, EDGE_DETAIL_MIN, EDGE_DETAIL_MAX));
		wWriteQualityTable(QUALITY_TDSHARP_DETAIL_GAIN_D, CLIP(bDetailGain, EDGE_DETAIL_MIN, EDGE_DETAIL_MAX));
	}
	
	bEdgeGain = wReadQualityTable(QUALITY_TDSHARP_EDGE_GAIN_V);
	bDetailGain = wReadQualityTable(QUALITY_TDSHARP_DETAIL_GAIN_V);
	if(bEdgeGain >= EDGE_DETAIL_MAX ||
	   bEdgeGain <= EDGE_DETAIL_MIN ||
	   bDetailGain >= EDGE_DETAIL_MAX ||
	   bDetailGain <=EDGE_DETAIL_MIN)
	{	
		printf("\n\nSharpness V EdgeGain and DetailGain danger value !!\n\n");	
		wWriteQualityTable(QUALITY_TDSHARP_EDGE_GAIN_V, CLIP(bEdgeGain, EDGE_DETAIL_MIN, EDGE_DETAIL_MAX));
		wWriteQualityTable(QUALITY_TDSHARP_DETAIL_GAIN_V, CLIP(bDetailGain, EDGE_DETAIL_MIN, EDGE_DETAIL_MAX));	
	}

/*
	//SAT_PROC
	bSAT_PROC = wReadQualityTable(QUALITY_TDSHARPALL_SAT_PROC);
	if(bSAT_PROC > SAT_PROC_MAX)
	{
		printf("\n\nSharpness SAT_PROC Gain too large !!\n\n");
		wWriteQualityTable(QUALITY_TDSHARPALL_SAT_PROC, MIN(bSAT_PROC, SAT_PROC_MAX));
	}
*/	

}

/**
 * @brief Set Post sharpness parameters (Main path only)
 * @param
 * @dependency This function should be called by vApiVideoSharpness
 */
void vDrvPostSharpParam(void)
{
	VRM_INFO_T in,out;
	UINT8 bNoVSHP; 
    UINT8 bLineInterleave;
    UINT32 u4SCL  = u4DrvTDTVScalerModeQuery();
    E_TD_OUT eSCLOut= TD_SCL_OUT(u4SCL);
    E_TD_IN  eSCLIn = TD_SCL_IN(u4SCL);

	//vDrvTDSConstraint();

    //scaler out line interleave or not
    bLineInterleave = ((E_TD_OUT_3D_LI==eSCLOut) ||
                        ((E_TD_IN_LI_P==eSCLIn)&&(E_TD_OUT_NATIVE==eSCLOut))); 
    
    //get fsc in & fsc out info
    VRMGetModuleVRMInfo(SV_VP_MAIN, VRM_MODULE_FSC, &in, &out);
    bNoVSHP = (out.u2Width > 1920);

	//No need to set this regster by mtk fw, this control by LGE HAL VPQ
	/*
	//PBC
    vIO32WriteFldAlign(CDS_00, wReadQualityTable(QUALITY_PBC_PBC1_EN)&& (!bForceAllPQOff) && wReadQualityTable(QUALITY_MODULE_ONOFF_SHARPNESS), CDS1_EN);                                                                                                                   
    vIO32WriteFldAlign(CDS_03, wReadQualityTable(QUALITY_PBC_PBC1_EDGE_EN), CDS1_EDGE_EN);                                                                                                          
    vIO32WriteFldAlign(CDS_03, wReadQualityTable(QUALITY_PBC_PBC1_EDGE_SLOPE), CDS1_EDGE_SLOPE);                              
    vIO32WriteFldAlign(CDS_04, wReadQualityTable(QUALITY_PBC_PBC1_LPF_LEVEL), CDS1_LPF_STRENGTH);

    vIO32WriteFldAlign(CDS_2A, wReadQualityTable(QUALITY_PBC_PBC2_EN)&& (!bForceAllPQOff) && wReadQualityTable(QUALITY_MODULE_ONOFF_SHARPNESS), CDS2_EN);                                                                                                                    
    vIO32WriteFldAlign(CDS_2D, wReadQualityTable(QUALITY_PBC_PBC2_EDGE_EN), CDS2_EDGE_EN);                                                                                                          
    vIO32WriteFldAlign(CDS_2D, wReadQualityTable(QUALITY_PBC_PBC2_EDGE_SLOPE), CDS2_EDGE_SLOPE);                               
    vIO32WriteFldAlign(CDS_2E, wReadQualityTable(QUALITY_PBC_PBC2_LPF_LEVEL), CDS2_LPF_STRENGTH);
	
    vIO32WriteFldAlign(CDS_54, wReadQualityTable(QUALITY_PBC_PBC3_EN)&& (!bForceAllPQOff) && wReadQualityTable(QUALITY_MODULE_ONOFF_SHARPNESS), CDS3_EN);                                                                                                                    
    vIO32WriteFldAlign(CDS_57, wReadQualityTable(QUALITY_PBC_PBC3_EDGE_EN), CDS3_EDGE_EN);                                                                                                          
    vIO32WriteFldAlign(CDS_57, wReadQualityTable(QUALITY_PBC_PBC3_EDGE_SLOPE), CDS3_EDGE_SLOPE);                               
    vIO32WriteFldAlign(CDS_58, wReadQualityTable(QUALITY_PBC_PBC3_LPF_LEVEL), CDS3_LPF_STRENGTH);

    //Group Clip       
    vIO32WriteFldMulti(SHARP_45, P_Fld(wReadQualityTable(QUALITY_TDSHARP_CLIP_RATIO_H), TDS_CLIP_RATIO_LH)|
    							 P_Fld(wReadQualityTable(QUALITY_TDSHARP_CLIP_THR_H), TDS_CLIP_THR_LH)|
    							 P_Fld(wReadQualityTable(QUALITY_TDSHARP_CLIP_RATIO_D), TDS_CLIP_RATIO_MD)|
    							 P_Fld(wReadQualityTable(QUALITY_TDSHARP_CLIP_THR_D), TDS_CLIP_THR_MD));              
     
	vIO32WriteFldMulti(SHARP_46, P_Fld(wReadQualityTable(QUALITY_TDSHARP_CLIP_RATIO_V), TDS_CLIP_RATIO_HV)|
								  P_Fld(wReadQualityTable(QUALITY_TDSHARP_CLIP_THR_V), TDS_CLIP_THR_HV));

	//Group Edge/Detail Gain
	vIO32WriteFldMulti(SHARP_31, P_Fld(wReadQualityTable(QUALITY_TDSHARP_DETAIL_GAIN_V), SHP_DETAIL_GAIN_HB)|     
								 P_Fld(wReadQualityTable(QUALITY_TDSHARP_EDGE_GAIN_V), SHP_EDGE_GAIN_HB));  
	vIO32WriteFldMulti(SHARP_30, P_Fld(wReadQualityTable(QUALITY_TDSHARP_DETAIL_GAIN_D), SHP_DETAIL_GAIN_MB)|     
								 P_Fld(wReadQualityTable(QUALITY_TDSHARP_EDGE_GAIN_D), SHP_EDGE_GAIN_MB)|  
								 P_Fld(wReadQualityTable(QUALITY_TDSHARP_DETAIL_GAIN_H), SHP_DETAIL_GAIN_LB)|     
								 P_Fld(wReadQualityTable(QUALITY_TDSHARP_EDGE_GAIN_H), SHP_EDGE_GAIN_LB));  

	//H1 Tap Adap 
	vIO32WriteFldMulti(SHARP_42, P_Fld(wReadQualityTable(QUALITY_TDSHARP_TDS_TAP_MODE), TDS_TAP_MODE )|     
								 P_Fld(wReadQualityTable(QUALITY_TDSHARP_TDS_TAP_EN), TDS_TAP_EN )|  
								 P_Fld(wReadQualityTable(QUALITY_TDSHARP_TDS_TAP_CLIP_EN), TDS_TAP_CLIP_EN )|     
								 P_Fld(wReadQualityTable(QUALITY_TDSHARP_TDS_TAP_LIMIT), TDS_TAP_LIMIT )|
								 P_Fld(wReadQualityTable(QUALITY_TDSHARP_TDS_TAP_RATIO), TDS_TAP_RATIO )|
								 P_Fld(wReadQualityTable(QUALITY_TDSHARP_TDS_TAP_SRATIO), TDS_TAP_SRATIO ));

	//H/V Tap Adap
	vIO32WriteFldMulti(SHARP_4C, P_Fld(wReadQualityTable(QUALITY_TDSHARP_TDS_VERT_TAP_APT_EN), TDS_VERT_TAP_APT_EN)|     
								 P_Fld(wReadQualityTable(QUALITY_TDSHARP_TDS_VERT_TAP_APT_SLOPE), TDS_VERT_TAP_APT_SLOPE)|  
								 P_Fld(wReadQualityTable(QUALITY_TDSHARP_TDS_VERT_TAP_APT_EDGE), TDS_VERT_TAP_APT_EDGE)|     
								 P_Fld(wReadQualityTable(QUALITY_TDSHARP_TDS_VERT_TAP_APT_DIFF), TDS_VERT_TAP_APT_DIFF)|
								 P_Fld(wReadQualityTable(QUALITY_TDSHARP_TDS_VERT_TAP_APT_BACK), TDS_VERT_TAP_APT_BACK));

	vIO32WriteFldMulti(SHARP_4D, P_Fld(wReadQualityTable(QUALITY_TDSHARP_TDS_HORIZ_TAP_APT_EN), TDS_HORIZ_TAP_APT_EN)|     
								 P_Fld(wReadQualityTable(QUALITY_TDSHARP_TDS_HORIZ_TAP_APT_SLOPE), TDS_HORIZ_TAP_APT_SLOPE)|  
								 P_Fld(wReadQualityTable(QUALITY_TDSHARP_TDS_HORIZ_TAP_APT_EDGE), TDS_HORIZ_TAP_APT_EDGE)|     
								 P_Fld(wReadQualityTable(QUALITY_TDSHARP_TDS_HORIZ_TAP_APT_DIFF), TDS_HORIZ_TAP_APT_DIFF)|
								 P_Fld(wReadQualityTable(QUALITY_TDSHARP_TDS_HORIZ_TAP_APT_BACK_0), TDS_HORIZ_TAP_APT_BACK_0)|
								 P_Fld(wReadQualityTable(QUALITY_TDSHARP_TDS_HORIZ_TAP_APT_BACK_1), TDS_HORIZ_TAP_APT_BACK_1));

	vIO32WriteFldMulti(SHARP_4E, P_Fld(wReadQualityTable(QUALITY_TDSHARP_TDS_HORIZ_TAP_APT_BACK_2), TDS_HORIZ_TAP_APT_BACK_2)|     
								 P_Fld(wReadQualityTable(QUALITY_TDSHARP_TDS_HORIZ_TAP_APT_BACK_3), TDS_HORIZ_TAP_APT_BACK_3)|  
								 P_Fld(wReadQualityTable(QUALITY_TDSHARP_TDS_HORIZ_TAP_APT_BACK_H2), TDS_HORIZ_TAP_APT_BACK_H2)|     
								 P_Fld(wReadQualityTable(QUALITY_TDSHARP_TDS_HORIZ_TAP_APT_EDGE_H2), TDS_HORIZ_TAP_APT_EDGE_H2));
	

    // TDSHARPALL
    vIO32WriteFldAlign(SHARP_27, wReadQualityTable(QUALITY_TDSHARPALL_GAIN), SHP_COMBINED_GAIN); 
    vIO32WriteFldMulti(SHARP_5D, P_Fld(wReadQualityTable(QUALITY_TDSHARPALL_CLIP_RATIO), TDS_CLIP_RATIO)|     
								 P_Fld(wReadQualityTable(QUALITY_TDSHARPALL_CLIP_POS), TDS_CLIP_THR));       
    vIO32WriteFldAlign(SHARP_36, wReadQualityTable(QUALITY_TDSHARPALL_SAT_PROC), TDS_SAT_PROC);       
    vIO32WriteFldAlign(SHARP_41, wReadQualityTable(QUALITY_TDSHARPALL_AC_LPF_COE), TDS_AC_LPF_COE);     
	// Y Level 
	if(bNoVSHP || bLineInterleave)
	{
		vIO32WriteFldAlign(SHARP_4B, SV_OFF, TDS_YLEV_EN); 
		vIO32WriteFldAlign(CDS_04, SV_OFF, CDS1_LPF_EN);
		vIO32WriteFldAlign(CDS_2E, SV_OFF, CDS2_LPF_EN);
		vIO32WriteFldAlign(CDS_58, SV_OFF, CDS3_LPF_EN);
	}
	else
	{
		vIO32WriteFldAlign(SHARP_4B, wReadQualityTable(QUALITY_TDSHARPALL_YLEV_EN), TDS_YLEV_EN);
		vIO32WriteFldAlign(CDS_04, wReadQualityTable(QUALITY_PBC_PBC1_LPF_EN), CDS1_LPF_EN);
		vIO32WriteFldAlign(CDS_2E, wReadQualityTable(QUALITY_PBC_PBC2_LPF_EN), CDS2_LPF_EN);
		vIO32WriteFldAlign(CDS_58, wReadQualityTable(QUALITY_PBC_PBC3_LPF_EN), CDS3_LPF_EN);
	}
	*/
    // Y Level 
    vIO32WriteFldAlign(TDPROC_YLEV_00, wReadQualityTable(QUALITY_TDSHARPALL_YLEV_ADAP_ENA), TDS_YLEV_ADAP_ENA);
    vIO32WriteFldAlign(TDPROC_YLEV_00, wReadQualityTable(QUALITY_TDSHARPALL_YLEV_ADL_ENA), TDS_YLEV_ADL_ENA);
    vIO32WriteFldAlign(TDPROC_YLEV_00, wReadQualityTable(QUALITY_TDSHARPALL_YLEV_APL_ENA), TDS_YLEV_APL_ENA);
    vIO32WriteFldAlign(TDPROC_YLEV_00, SV_ON, TDS_YLEV_CUST_ENA);	
    vIO32WriteFldAlign(TDPROC_YLEV_00, wReadQualityTable(QUALITY_TDSHARPALL_YLEV_ADL_GAIN), TDS_YLEV_ADL_GAIN);
	
	//No need to set this regster by mtk fw, this control by LGE HAL VPQ
	/*
    vIO32WriteFldAlign(TDPROC_YLEV_00, wReadQualityTable(QUALITY_TDSHARPALL_YLEV_APL_ZERO), TDS_YLEV_APL_ZERO);
    vIO32WriteFldAlign(TDPROC_YLEV_00, wReadQualityTable(QUALITY_TDSHARPALL_YLEV_APL_THR), TDS_YLEV_APL_THR);	
    vIO32WriteFldAlign(TDPROC_YLEV_01, wReadQualityTable(QUALITY_TDSHARPALL_YLEV_P1),  TDS_YLEV_P1);
    vIO32WriteFldAlign(TDPROC_YLEV_01, wReadQualityTable(QUALITY_TDSHARPALL_YLEV_P2),  TDS_YLEV_P2);
    vIO32WriteFldAlign(TDPROC_YLEV_01, wReadQualityTable(QUALITY_TDSHARPALL_YLEV_P3),  TDS_YLEV_P3);
    vIO32WriteFldAlign(TDPROC_YLEV_01, wReadQualityTable(QUALITY_TDSHARPALL_YLEV_P4),  TDS_YLEV_P4);
    vIO32WriteFldAlign(TDPROC_YLEV_02, wReadQualityTable(QUALITY_TDSHARPALL_YLEV_G1), TDS_YLEV_G1);
    vIO32WriteFldAlign(TDPROC_YLEV_02, wReadQualityTable(QUALITY_TDSHARPALL_YLEV_G2), TDS_YLEV_G2);
    vIO32WriteFldAlign(TDPROC_YLEV_02, wReadQualityTable(QUALITY_TDSHARPALL_YLEV_G3), TDS_YLEV_G3);
    vIO32WriteFldAlign(TDPROC_YLEV_02, wReadQualityTable(QUALITY_TDSHARPALL_YLEV_G4), TDS_YLEV_G4);
    */
	
    // TDSHARP_3D
	vIO32WriteFldAlign(SHARPNESS_3D_00, u4DrvTDTV3DModeQuery(), SHARPNESS_3D_MODE); 
    vIO32WriteFldAlign(SHARPNESS_3D_01, wReadQualityTable(QUALITY_TDSHARP_3D_FS_HGAIN), SHARPNESS_FS_HGAIN);    
    vIO32WriteFldAlign(SHARPNESS_3D_01, wReadQualityTable(QUALITY_TDSHARP_3D_FS_DGAIN), SHARPNESS_FS_DGAIN);      
    vIO32WriteFldAlign(SHARPNESS_3D_01, wReadQualityTable(QUALITY_TDSHARP_3D_FS_VGAIN), SHARPNESS_FS_VGAIN);      
    vIO32WriteFldAlign(SHARPNESS_3D_02, wReadQualityTable(QUALITY_TDSHARP_3D_TTD_HGAIN), SHARPNESS_TTD_HGAIN);     
    vIO32WriteFldAlign(SHARPNESS_3D_02, wReadQualityTable(QUALITY_TDSHARP_3D_TTD_DGAIN), SHARPNESS_TTD_DGAIN);     
    vIO32WriteFldAlign(SHARPNESS_3D_02, wReadQualityTable(QUALITY_TDSHARP_3D_TTD_VGAIN), SHARPNESS_TTD_VGAIN);     
    vIO32WriteFldAlign(SHARPNESS_3D_03, wReadQualityTable(QUALITY_TDSHARP_3D_SBS_HGAIN), SHARPNESS_SBS_HGAIN);     
    vIO32WriteFldAlign(SHARPNESS_3D_03, wReadQualityTable(QUALITY_TDSHARP_3D_SBS_DGAIN), SHARPNESS_SBS_DGAIN);     
    vIO32WriteFldAlign(SHARPNESS_3D_03, wReadQualityTable(QUALITY_TDSHARP_3D_SBS_VGAIN), SHARPNESS_SBS_VGAIN);     
    vIO32WriteFldAlign(SHARPNESS_3D_04, wReadQualityTable(QUALITY_TDSHARP_3D_TB_HGAIN), SHARPNESS_TB_HGAIN);      
    vIO32WriteFldAlign(SHARPNESS_3D_04, wReadQualityTable(QUALITY_TDSHARP_3D_TB_DGAIN), SHARPNESS_TB_DGAIN);      
    vIO32WriteFldAlign(SHARPNESS_3D_04, wReadQualityTable(QUALITY_TDSHARP_3D_TB_VGAIN), SHARPNESS_TB_VGAIN);      
    vIO32WriteFldAlign(SHARPNESS_3D_05, wReadQualityTable(QUALITY_TDSHARP_3D_SS_HGAIN), SHARPNESS_SS_HGAIN);      
    vIO32WriteFldAlign(SHARPNESS_3D_05, wReadQualityTable(QUALITY_TDSHARP_3D_SS_DGAIN), SHARPNESS_SS_DGAIN);      
    vIO32WriteFldAlign(SHARPNESS_3D_05, wReadQualityTable(QUALITY_TDSHARP_3D_SS_VGAIN), SHARPNESS_SS_VGAIN);      
    vIO32WriteFldAlign(SHARPNESS_3D_06, wReadQualityTable(QUALITY_TDSHARP_3D_RD_HGAIN), SHARPNESS_RD_HGAIN);      
    vIO32WriteFldAlign(SHARPNESS_3D_06, wReadQualityTable(QUALITY_TDSHARP_3D_RD_DGAIN), SHARPNESS_RD_DGAIN);      
    vIO32WriteFldAlign(SHARPNESS_3D_06, wReadQualityTable(QUALITY_TDSHARP_3D_RD_VGAIN), SHARPNESS_RD_VGAIN);      
    vIO32WriteFldAlign(SHARPNESS_3D_07, wReadQualityTable(QUALITY_TDSHARP_3D_LI_HGAIN), SHARPNESS_LI_HGAIN);      
    vIO32WriteFldAlign(SHARPNESS_3D_07, wReadQualityTable(QUALITY_TDSHARP_3D_LI_DGAIN), SHARPNESS_LI_DGAIN);      
    vIO32WriteFldAlign(SHARPNESS_3D_07, wReadQualityTable(QUALITY_TDSHARP_3D_LI_VGAIN), SHARPNESS_LI_VGAIN);      
    vIO32WriteFldAlign(SHARPNESS_3D_08, wReadQualityTable(QUALITY_TDSHARP_3D_CB_HGAIN), SHARPNESS_CB_HGAIN);      
    vIO32WriteFldAlign(SHARPNESS_3D_08, wReadQualityTable(QUALITY_TDSHARP_3D_CB_DGAIN), SHARPNESS_CB_DGAIN);      
    vIO32WriteFldAlign(SHARPNESS_3D_08, wReadQualityTable(QUALITY_TDSHARP_3D_CB_VGAIN), SHARPNESS_CB_VGAIN);      
    vIO32WriteFldAlign(SHARPNESS_3D_09, wReadQualityTable(QUALITY_TDSHARP_3D_DA_HGAIN), SHARPNESS_DA_HGAIN);      
    vIO32WriteFldAlign(SHARPNESS_3D_09, wReadQualityTable(QUALITY_TDSHARP_3D_DA_DGAIN), SHARPNESS_DA_DGAIN);      
    vIO32WriteFldAlign(SHARPNESS_3D_09, wReadQualityTable(QUALITY_TDSHARP_3D_DA_VGAIN), SHARPNESS_DA_VGAIN);  

    vIO32WriteFldAlign(SHARPNESS_3D_0A, wReadQualityTable(QUALITY_TDSHARP_3D_FS_HCLIP), SHARPNESS_FS_HCLIP);    
    vIO32WriteFldAlign(SHARPNESS_3D_0A, wReadQualityTable(QUALITY_TDSHARP_3D_FS_DCLIP), SHARPNESS_FS_DCLIP);      
    vIO32WriteFldAlign(SHARPNESS_3D_0A, wReadQualityTable(QUALITY_TDSHARP_3D_FS_VCLIP), SHARPNESS_FS_VCLIP);      
    vIO32WriteFldAlign(SHARPNESS_3D_0B, wReadQualityTable(QUALITY_TDSHARP_3D_TTD_HCLIP), SHARPNESS_TTD_HCLIP);     
    vIO32WriteFldAlign(SHARPNESS_3D_0B, wReadQualityTable(QUALITY_TDSHARP_3D_TTD_DCLIP), SHARPNESS_TTD_DCLIP);     
    vIO32WriteFldAlign(SHARPNESS_3D_0B, wReadQualityTable(QUALITY_TDSHARP_3D_TTD_VCLIP), SHARPNESS_TTD_VCLIP);     
    vIO32WriteFldAlign(SHARPNESS_3D_0C, wReadQualityTable(QUALITY_TDSHARP_3D_SBS_HCLIP), SHARPNESS_SBS_HCLIP);     
    vIO32WriteFldAlign(SHARPNESS_3D_0C, wReadQualityTable(QUALITY_TDSHARP_3D_SBS_DCLIP), SHARPNESS_SBS_DCLIP);     
    vIO32WriteFldAlign(SHARPNESS_3D_0C, wReadQualityTable(QUALITY_TDSHARP_3D_SBS_VCLIP), SHARPNESS_SBS_VCLIP);     
    vIO32WriteFldAlign(SHARPNESS_3D_0D, wReadQualityTable(QUALITY_TDSHARP_3D_TB_HCLIP), SHARPNESS_TB_HCLIP);      
    vIO32WriteFldAlign(SHARPNESS_3D_0D, wReadQualityTable(QUALITY_TDSHARP_3D_TB_DCLIP), SHARPNESS_TB_DCLIP);      
    vIO32WriteFldAlign(SHARPNESS_3D_0D, wReadQualityTable(QUALITY_TDSHARP_3D_TB_VCLIP), SHARPNESS_TB_VCLIP);      
    vIO32WriteFldAlign(SHARPNESS_3D_0E, wReadQualityTable(QUALITY_TDSHARP_3D_SS_HCLIP), SHARPNESS_SS_HCLIP);      
    vIO32WriteFldAlign(SHARPNESS_3D_0E, wReadQualityTable(QUALITY_TDSHARP_3D_SS_DCLIP), SHARPNESS_SS_DCLIP);      
    vIO32WriteFldAlign(SHARPNESS_3D_0E, wReadQualityTable(QUALITY_TDSHARP_3D_SS_VCLIP), SHARPNESS_SS_VCLIP);      
    vIO32WriteFldAlign(SHARPNESS_3D_0F, wReadQualityTable(QUALITY_TDSHARP_3D_RD_HCLIP), SHARPNESS_RD_HCLIP);      
    vIO32WriteFldAlign(SHARPNESS_3D_0F, wReadQualityTable(QUALITY_TDSHARP_3D_RD_DCLIP), SHARPNESS_RD_DCLIP);      
    vIO32WriteFldAlign(SHARPNESS_3D_0F, wReadQualityTable(QUALITY_TDSHARP_3D_RD_VCLIP), SHARPNESS_RD_VCLIP);      
    vIO32WriteFldAlign(SHARPNESS_3D_10, wReadQualityTable(QUALITY_TDSHARP_3D_LI_HCLIP), SHARPNESS_LI_HCLIP);      
    vIO32WriteFldAlign(SHARPNESS_3D_10, wReadQualityTable(QUALITY_TDSHARP_3D_LI_DCLIP), SHARPNESS_LI_DCLIP);      
    vIO32WriteFldAlign(SHARPNESS_3D_10, wReadQualityTable(QUALITY_TDSHARP_3D_LI_VCLIP), SHARPNESS_LI_VCLIP);      
    vIO32WriteFldAlign(SHARPNESS_3D_11, wReadQualityTable(QUALITY_TDSHARP_3D_CB_HCLIP), SHARPNESS_CB_HCLIP);      
    vIO32WriteFldAlign(SHARPNESS_3D_11, wReadQualityTable(QUALITY_TDSHARP_3D_CB_DCLIP), SHARPNESS_CB_DCLIP);      
    vIO32WriteFldAlign(SHARPNESS_3D_11, wReadQualityTable(QUALITY_TDSHARP_3D_CB_VCLIP), SHARPNESS_CB_VCLIP);      
    vIO32WriteFldAlign(SHARPNESS_3D_12, wReadQualityTable(QUALITY_TDSHARP_3D_DA_HCLIP), SHARPNESS_DA_HCLIP);      
    vIO32WriteFldAlign(SHARPNESS_3D_12, wReadQualityTable(QUALITY_TDSHARP_3D_DA_DCLIP), SHARPNESS_DA_DCLIP);      
    vIO32WriteFldAlign(SHARPNESS_3D_12, wReadQualityTable(QUALITY_TDSHARP_3D_DA_VCLIP), SHARPNESS_DA_VCLIP);  

	//3d weight can be customized here by flash pq
	vDrvGetSharp3DWeighting();

	// Gain
	vDrvPostSharpParamHorizontal();
	vDrvPostSharpParamVertical();
	vDrvPostSharpParamDiagonal();
    vDrvPostSharpParamPBC();
	vDrvLTIParam();
}


void vDrvPostSharpParamHorizontal(void)
{
	//No need to set this regster by mtk fw, this control by LGE HAL VPQ
	return;

	#if 0
	UINT8 b3DHGain = IO32ReadFldAlign(SHARPNESS_3D_13, SHARPNESS_3D_HGAIN);
	UINT8 bH1Gain = wDrvGetTDShp3DGain(wReadQualityTable(QUALITY_TDSHARP_GAIN_H1), b3DHGain);
	UINT8 bH2Gain = wDrvGetTDShp3DGain(wReadQualityTable(QUALITY_TDSHARP_GAIN_H2), b3DHGain);
	UINT8 bH3Gain = wDrvGetTDShp3DGain(wReadQualityTable(QUALITY_TDSHARP_GAIN_H3), b3DHGain);

	vIO32WriteFldAlign(TDSHP_GAIN_01, bH1Gain, TDSHP_GAIN_H1);
	vIO32WriteFldAlign(TDSHP_GAIN_01, bH2Gain, TDSHP_GAIN_H2);
	vIO32WriteFldAlign(TDSHP_GAIN_01, bH3Gain, TDSHP_GAIN_H3);
	vIO32WriteFldAlign(TDSHP_CORING_RATIO_01, wReadQualityTable(QUALITY_TDSHARP_COR_RATIO_H1), TDSHP_CORING_RATIO_H1);
	vIO32WriteFldAlign(TDSHP_CORING_RATIO_01, wReadQualityTable(QUALITY_TDSHARP_COR_RATIO_H2), TDSHP_CORING_RATIO_H2);
	vIO32WriteFldAlign(TDSHP_CORING_RATIO_01, wReadQualityTable(QUALITY_TDSHARP_COR_RATIO_H3), TDSHP_CORING_RATIO_H3); 
	vIO32WriteFldAlign(TDSHP_LIMIT_RATIO_01, wReadQualityTable(QUALITY_TDSHARP_LIMIT_RATIO_H1), TDSHP_LIMIT_RATIO_H1); 
	vIO32WriteFldAlign(TDSHP_LIMIT_RATIO_01, wReadQualityTable(QUALITY_TDSHARP_LIMIT_RATIO_H2), TDSHP_LIMIT_RATIO_H2); 
	vIO32WriteFldAlign(TDSHP_LIMIT_RATIO_01, wReadQualityTable(QUALITY_TDSHARP_LIMIT_RATIO_H3), TDSHP_LIMIT_RATIO_H3); 
	vIO32WriteFldAlign(TDSHP_CORE_01, wReadQualityTable(QUALITY_TDSHARP_COR_ZERO_H1), TDSHP_CORE_H1);
	vIO32WriteFldAlign(TDSHP_CORE_01, wReadQualityTable(QUALITY_TDSHARP_COR_ZERO_H2), TDSHP_CORE_H2);
	vIO32WriteFldAlign(TDSHP_CORE_01, wReadQualityTable(QUALITY_TDSHARP_COR_ZERO_H3), TDSHP_CORE_H3);
	vIO32WriteFldAlign(TDSHP_CORETH_01, wReadQualityTable(QUALITY_TDSHARP_COR_THR_H1), TDSHP_CORETH_H1);
	vIO32WriteFldAlign(TDSHP_CORETH_01, wReadQualityTable(QUALITY_TDSHARP_COR_THR_H2), TDSHP_CORETH_H2);
	vIO32WriteFldAlign(TDSHP_CORETH_01, wReadQualityTable(QUALITY_TDSHARP_COR_THR_H3), TDSHP_CORETH_H3);
	vIO32WriteFldAlign(TDSHP_LIMIT_00, wReadQualityTable(QUALITY_TDSHARP_LIMIT_H1), TDSHARP_LIMIT_H1); 
	vIO32WriteFldAlign(TDSHP_LIMIT_00, wReadQualityTable(QUALITY_TDSHARP_LIMIT_H2), TDSHARP_LIMIT_H2); 
	vIO32WriteFldAlign(TDSHP_LIMIT_00, wReadQualityTable(QUALITY_TDSHARP_LIMIT_H3), TDSHARP_LIMIT_H3);  
	vIO32WriteFldAlign(TDSHP_LIMIT_03, wReadQualityTable(QUALITY_TDSHARP_BOUND_H1), TDSHARP_BOUND_H1); 
	vIO32WriteFldAlign(TDSHP_LIMIT_03, wReadQualityTable(QUALITY_TDSHARP_BOUND_H2), TDSHARP_BOUND_H2); 
	vIO32WriteFldAlign(TDSHP_LIMIT_03, wReadQualityTable(QUALITY_TDSHARP_BOUND_H3), TDSHARP_BOUND_H3); 
	#endif
}

void vDrvPostSharpParamVertical(void)
{     
	//No need to set this regster by mtk fw, this control by LGE HAL VPQ
	return;
	#if 0
	UINT8 b3DVGain = IO32ReadFldAlign(SHARPNESS_3D_13, SHARPNESS_3D_VGAIN);
	UINT8 bV1Gain = wDrvGetTDShp3DGain(wReadQualityTable(QUALITY_TDSHARP_GAIN_V1), b3DVGain);
    UINT8 bV2Gain = wDrvGetTDShp3DGain(wReadQualityTable(QUALITY_TDSHARP_GAIN_V2), b3DVGain);
    UINT8 bV3Gain = wDrvGetTDShp3DGain(wReadQualityTable(QUALITY_TDSHARP_GAIN_V3), b3DVGain);

    vIO32WriteFldAlign(TDSHP_GAIN_02, bV1Gain, TDSHP_GAIN_V1);
    vIO32WriteFldAlign(TDSHP_GAIN_02, bV2Gain, TDSHP_GAIN_V2);
    vIO32WriteFldAlign(TDSHP_GAIN_03, bV3Gain, TDSHP_GAIN_V3);
    vIO32WriteFldAlign(TDSHP_CORING_RATIO_02, wReadQualityTable(QUALITY_TDSHARP_COR_RATIO_V1), TDSHP_CORING_RATIO_V1);
    vIO32WriteFldAlign(TDSHP_CORING_RATIO_02, wReadQualityTable(QUALITY_TDSHARP_COR_RATIO_V2), TDSHP_CORING_RATIO_V2);
    vIO32WriteFldAlign(TDSHP_CORING_RATIO_03, wReadQualityTable(QUALITY_TDSHARP_COR_RATIO_V3), TDSHP_CORING_RATIO_V3);
    vIO32WriteFldAlign(TDSHP_LIMIT_RATIO_02, wReadQualityTable(QUALITY_TDSHARP_LIMIT_RATIO_V1), TDSHP_LIMIT_RATIO_V1); 
    vIO32WriteFldAlign(TDSHP_LIMIT_RATIO_02, wReadQualityTable(QUALITY_TDSHARP_LIMIT_RATIO_V2), TDSHP_LIMIT_RATIO_V2); 
    vIO32WriteFldAlign(TDSHP_LIMIT_RATIO_03, wReadQualityTable(QUALITY_TDSHARP_LIMIT_RATIO_V3), TDSHP_LIMIT_RATIO_V3); 
    vIO32WriteFldAlign(TDSHP_CORE_02, wReadQualityTable(QUALITY_TDSHARP_COR_ZERO_V1), TDSHP_CORE_V1);
    vIO32WriteFldAlign(TDSHP_CORE_02, wReadQualityTable(QUALITY_TDSHARP_COR_ZERO_V2), TDSHP_CORE_V2);
    vIO32WriteFldAlign(TDSHP_CORE_03, wReadQualityTable(QUALITY_TDSHARP_COR_ZERO_V3), TDSHP_CORE_V3);
    vIO32WriteFldAlign(TDSHP_CORETH_02, wReadQualityTable(QUALITY_TDSHARP_COR_THR_V1), TDSHP_CORETH_V1);
    vIO32WriteFldAlign(TDSHP_CORETH_02, wReadQualityTable(QUALITY_TDSHARP_COR_THR_V2), TDSHP_CORETH_V2);
    vIO32WriteFldAlign(TDSHP_CORETH_03, wReadQualityTable(QUALITY_TDSHARP_COR_THR_V3), TDSHP_CORETH_V3);
    vIO32WriteFldAlign(TDSHP_LIMIT_01, wReadQualityTable(QUALITY_TDSHARP_LIMIT_V1), TDSHARP_LIMIT_V1); 
    vIO32WriteFldAlign(TDSHP_LIMIT_01, wReadQualityTable(QUALITY_TDSHARP_LIMIT_V2), TDSHARP_LIMIT_V2); 
    vIO32WriteFldAlign(TDSHP_LIMIT_06, wReadQualityTable(QUALITY_TDSHARP_LIMIT_V3), TDSHARP_LIMIT_V3); 
    vIO32WriteFldAlign(TDSHP_LIMIT_04, wReadQualityTable(QUALITY_TDSHARP_BOUND_V1), TDSHARP_BOUND_V1); 
    vIO32WriteFldAlign(TDSHP_LIMIT_04, wReadQualityTable(QUALITY_TDSHARP_BOUND_V2), TDSHARP_BOUND_V2); 
    vIO32WriteFldAlign(TDSHP_LIMIT_06, wReadQualityTable(QUALITY_TDSHARP_BOUND_V3), TDSHARP_BOUND_V3); 
	#endif
}

void vDrvPostSharpParamDiagonal(void)
{
	//No need to set this regster by mtk fw, this control by LGE HAL VPQ
	return;
	#if 0
	UINT8 b3DDGain = IO32ReadFldAlign(SHARPNESS_3D_13, SHARPNESS_3D_DGAIN);
	UINT8 bD1Gain = wDrvGetTDShp3DGain(wReadQualityTable(QUALITY_TDSHARP_GAIN_D1), b3DDGain);
	UINT8 bD2Gain = wDrvGetTDShp3DGain(wReadQualityTable(QUALITY_TDSHARP_GAIN_D2), b3DDGain);
	UINT8 bD3Gain = wDrvGetTDShp3DGain(wReadQualityTable(QUALITY_TDSHARP_GAIN_D3), b3DDGain);

    vIO32WriteFldAlign(TDSHP_GAIN_01, bD1Gain, TDSHP_GAIN_D1);
    vIO32WriteFldAlign(TDSHP_GAIN_02, bD2Gain, TDSHP_GAIN_D2);
    vIO32WriteFldAlign(TDSHP_GAIN_02, bD3Gain, TDSHP_GAIN_D3);
    vIO32WriteFldAlign(TDSHP_CORE_01, wReadQualityTable(QUALITY_TDSHARP_COR_ZERO_D1), TDSHP_CORE_D1);
    vIO32WriteFldAlign(TDSHP_CORE_02, wReadQualityTable(QUALITY_TDSHARP_COR_ZERO_D2), TDSHP_CORE_D2);
    vIO32WriteFldAlign(TDSHP_CORE_02, wReadQualityTable(QUALITY_TDSHARP_COR_ZERO_D3), TDSHP_CORE_D3);
    vIO32WriteFldAlign(TDSHP_CORING_RATIO_01, wReadQualityTable(QUALITY_TDSHARP_COR_RATIO_D1), TDSHP_CORING_RATIO_D1);
    vIO32WriteFldAlign(TDSHP_CORING_RATIO_02, wReadQualityTable(QUALITY_TDSHARP_COR_RATIO_D2), TDSHP_CORING_RATIO_D2);
    vIO32WriteFldAlign(TDSHP_CORING_RATIO_02, wReadQualityTable(QUALITY_TDSHARP_COR_RATIO_D3), TDSHP_CORING_RATIO_D3);
    vIO32WriteFldAlign(TDSHP_CORETH_01, wReadQualityTable(QUALITY_TDSHARP_COR_THR_D1), TDSHP_CORETH_D1);
    vIO32WriteFldAlign(TDSHP_CORETH_02, wReadQualityTable(QUALITY_TDSHARP_COR_THR_D2), TDSHP_CORETH_D2);
    vIO32WriteFldAlign(TDSHP_CORETH_02, wReadQualityTable(QUALITY_TDSHARP_COR_THR_D3), TDSHP_CORETH_D3);
    vIO32WriteFldAlign(TDSHP_LIMIT_00, wReadQualityTable(QUALITY_TDSHARP_LIMIT_D1), TDSHARP_LIMIT_D1); 
    vIO32WriteFldAlign(TDSHP_LIMIT_01, wReadQualityTable(QUALITY_TDSHARP_LIMIT_D2), TDSHARP_LIMIT_D2); 
    vIO32WriteFldAlign(TDSHP_LIMIT_01, wReadQualityTable(QUALITY_TDSHARP_LIMIT_D3), TDSHARP_LIMIT_D3); 
    vIO32WriteFldAlign(TDSHP_LIMIT_RATIO_01, wReadQualityTable(QUALITY_TDSHARP_LIMIT_RATIO_D1), TDSHP_LIMIT_RATIO_D1); 
    vIO32WriteFldAlign(TDSHP_LIMIT_RATIO_02, wReadQualityTable(QUALITY_TDSHARP_LIMIT_RATIO_D2), TDSHP_LIMIT_RATIO_D2); 
    vIO32WriteFldAlign(TDSHP_LIMIT_RATIO_02, wReadQualityTable(QUALITY_TDSHARP_LIMIT_RATIO_D3), TDSHP_LIMIT_RATIO_D3); 
    vIO32WriteFldAlign(TDSHP_LIMIT_03, wReadQualityTable(QUALITY_TDSHARP_BOUND_D1), TDSHARP_BOUND_D1); 
    vIO32WriteFldAlign(TDSHP_LIMIT_04, wReadQualityTable(QUALITY_TDSHARP_BOUND_D2), TDSHARP_BOUND_D2); 
    vIO32WriteFldAlign(TDSHP_LIMIT_04, wReadQualityTable(QUALITY_TDSHARP_BOUND_D3), TDSHARP_BOUND_D3);
	#endif
}

void vDrvPostSharpParamPBC(void)
{
	//No need to set this regster by mtk fw, this control by LGE HAL VPQ
/*
    vIO32WriteFldAlign(TDSHP_GAIN_05, wReadQualityTable(QUALITY_PBC_PBC1_GAIN_H1), TDSHP_GAIN_PBC1_H1);
    vIO32WriteFldAlign(TDSHP_GAIN_05, wReadQualityTable(QUALITY_PBC_PBC1_GAIN_H2), TDSHP_GAIN_PBC1_H2);
    vIO32WriteFldAlign(TDSHP_GAIN_05, wReadQualityTable(QUALITY_PBC_PBC1_GAIN_H3), TDSHP_GAIN_PBC1_H3);
    vIO32WriteFldAlign(TDSHP_GAIN_05, wReadQualityTable(QUALITY_PBC_PBC1_GAIN_V1), TDSHP_GAIN_PBC1_V1);
    vIO32WriteFldAlign(TDSHP_GAIN_06, wReadQualityTable(QUALITY_PBC_PBC1_GAIN_V2), TDSHP_GAIN_PBC1_V2);
    vIO32WriteFldAlign(TDSHP_GAIN_06, wReadQualityTable(QUALITY_PBC_PBC1_GAIN_V3), TDSHP_GAIN_PBC1_V3);
    vIO32WriteFldAlign(TDSHP_GAIN_06, wReadQualityTable(QUALITY_PBC_PBC1_GAIN_D1), TDSHP_GAIN_PBC1_D1);
    vIO32WriteFldAlign(TDSHP_GAIN_06, wReadQualityTable(QUALITY_PBC_PBC1_GAIN_D2), TDSHP_GAIN_PBC1_D2);
    vIO32WriteFldAlign(TDSHP_GAIN_07, wReadQualityTable(QUALITY_PBC_PBC1_GAIN_D3), TDSHP_GAIN_PBC1_D3);

    vIO32WriteFldAlign(TDSHP_GAIN_07, wReadQualityTable(QUALITY_PBC_PBC2_GAIN_H1), TDSHP_GAIN_PBC2_H1);
    vIO32WriteFldAlign(TDSHP_GAIN_07, wReadQualityTable(QUALITY_PBC_PBC2_GAIN_H2), TDSHP_GAIN_PBC2_H2);
    vIO32WriteFldAlign(TDSHP_GAIN_07, wReadQualityTable(QUALITY_PBC_PBC2_GAIN_H3), TDSHP_GAIN_PBC2_H3);
    vIO32WriteFldAlign(TDSHP_GAIN_08, wReadQualityTable(QUALITY_PBC_PBC2_GAIN_V1), TDSHP_GAIN_PBC2_V1);
    vIO32WriteFldAlign(TDSHP_GAIN_08, wReadQualityTable(QUALITY_PBC_PBC2_GAIN_V2), TDSHP_GAIN_PBC2_V2);
    vIO32WriteFldAlign(TDSHP_GAIN_08, wReadQualityTable(QUALITY_PBC_PBC2_GAIN_V3), TDSHP_GAIN_PBC2_V3);
    vIO32WriteFldAlign(TDSHP_GAIN_08, wReadQualityTable(QUALITY_PBC_PBC2_GAIN_D1), TDSHP_GAIN_PBC2_D1);
    vIO32WriteFldAlign(TDSHP_GAIN_09, wReadQualityTable(QUALITY_PBC_PBC2_GAIN_D2), TDSHP_GAIN_PBC2_D2);
    vIO32WriteFldAlign(TDSHP_GAIN_09, wReadQualityTable(QUALITY_PBC_PBC2_GAIN_D3), TDSHP_GAIN_PBC2_D3);

    vIO32WriteFldAlign(TDSHP_GAIN_09, wReadQualityTable(QUALITY_PBC_PBC3_GAIN_H1), TDSHP_GAIN_PBC3_H1);
    vIO32WriteFldAlign(TDSHP_GAIN_09, wReadQualityTable(QUALITY_PBC_PBC3_GAIN_H2), TDSHP_GAIN_PBC3_H2);
    vIO32WriteFldAlign(TDSHP_GAIN_0A, wReadQualityTable(QUALITY_PBC_PBC3_GAIN_H3), TDSHP_GAIN_PBC3_H3);
    vIO32WriteFldAlign(TDSHP_GAIN_0A, wReadQualityTable(QUALITY_PBC_PBC3_GAIN_V1), TDSHP_GAIN_PBC3_V1);
    vIO32WriteFldAlign(TDSHP_GAIN_0A, wReadQualityTable(QUALITY_PBC_PBC3_GAIN_V2), TDSHP_GAIN_PBC3_V2);
    vIO32WriteFldAlign(TDSHP_GAIN_0A, wReadQualityTable(QUALITY_PBC_PBC3_GAIN_V3), TDSHP_GAIN_PBC3_V3);
    vIO32WriteFldAlign(TDSHP_GAIN_0B, wReadQualityTable(QUALITY_PBC_PBC3_GAIN_D1), TDSHP_GAIN_PBC3_D1);
    vIO32WriteFldAlign(TDSHP_GAIN_0B, wReadQualityTable(QUALITY_PBC_PBC3_GAIN_D2), TDSHP_GAIN_PBC3_D2);
    vIO32WriteFldAlign(TDSHP_GAIN_0B, wReadQualityTable(QUALITY_PBC_PBC3_GAIN_D3), TDSHP_GAIN_PBC3_D3);
*/
    vIO32WriteFldAlign(TDSHP_CORING_RATIO_05, COR_RATIO_UNIT, TDSHP_CORE_RATIO_PBC1_H1);
    vIO32WriteFldAlign(TDSHP_CORING_RATIO_05, COR_RATIO_UNIT, TDSHP_CORE_RATIO_PBC1_H2);
    vIO32WriteFldAlign(TDSHP_CORING_RATIO_05, COR_RATIO_UNIT, TDSHP_CORE_RATIO_PBC1_H3);
    vIO32WriteFldAlign(TDSHP_CORING_RATIO_05, COR_RATIO_UNIT, TDSHP_CORE_RATIO_PBC1_V1);
    vIO32WriteFldAlign(TDSHP_CORING_RATIO_06, COR_RATIO_UNIT, TDSHP_CORE_RATIO_PBC1_V2);
    vIO32WriteFldAlign(TDSHP_CORING_RATIO_06, COR_RATIO_UNIT, TDSHP_CORE_RATIO_PBC1_V3);
    vIO32WriteFldAlign(TDSHP_CORING_RATIO_06, COR_RATIO_UNIT, TDSHP_CORE_RATIO_PBC1_D1);
    vIO32WriteFldAlign(TDSHP_CORING_RATIO_06, COR_RATIO_UNIT, TDSHP_CORE_RATIO_PBC1_D2);
    vIO32WriteFldAlign(TDSHP_CORING_RATIO_07, COR_RATIO_UNIT, TDSHP_CORE_RATIO_PBC1_D3);

    vIO32WriteFldAlign(TDSHP_CORING_RATIO_07, COR_RATIO_UNIT, TDSHP_CORE_RATIO_PBC2_H1);
    vIO32WriteFldAlign(TDSHP_CORING_RATIO_07, COR_RATIO_UNIT, TDSHP_CORE_RATIO_PBC2_H2);
    vIO32WriteFldAlign(TDSHP_CORING_RATIO_07, COR_RATIO_UNIT, TDSHP_CORE_RATIO_PBC2_H3);
    vIO32WriteFldAlign(TDSHP_CORING_RATIO_08, COR_RATIO_UNIT, TDSHP_CORE_RATIO_PBC2_V1);
    vIO32WriteFldAlign(TDSHP_CORING_RATIO_08, COR_RATIO_UNIT, TDSHP_CORE_RATIO_PBC2_V2);
    vIO32WriteFldAlign(TDSHP_CORING_RATIO_08, COR_RATIO_UNIT, TDSHP_CORE_RATIO_PBC2_V3);
    vIO32WriteFldAlign(TDSHP_CORING_RATIO_08, COR_RATIO_UNIT, TDSHP_CORE_RATIO_PBC2_D1);
    vIO32WriteFldAlign(TDSHP_CORING_RATIO_09, COR_RATIO_UNIT, TDSHP_CORE_RATIO_PBC2_D2);
    vIO32WriteFldAlign(TDSHP_CORING_RATIO_09, COR_RATIO_UNIT, TDSHP_CORE_RATIO_PBC2_D3);

    vIO32WriteFldAlign(TDSHP_CORING_RATIO_09, COR_RATIO_UNIT, TDSHP_CORE_RATIO_PBC3_H1);
    vIO32WriteFldAlign(TDSHP_CORING_RATIO_09, COR_RATIO_UNIT, TDSHP_CORE_RATIO_PBC3_H2);
    vIO32WriteFldAlign(TDSHP_CORING_RATIO_0A, COR_RATIO_UNIT, TDSHP_CORE_RATIO_PBC3_H3);
    vIO32WriteFldAlign(TDSHP_CORING_RATIO_0A, COR_RATIO_UNIT, TDSHP_CORE_RATIO_PBC3_V1);
    vIO32WriteFldAlign(TDSHP_CORING_RATIO_0A, COR_RATIO_UNIT, TDSHP_CORE_RATIO_PBC3_V2);
    vIO32WriteFldAlign(TDSHP_CORING_RATIO_0A, COR_RATIO_UNIT, TDSHP_CORE_RATIO_PBC3_V3);
    vIO32WriteFldAlign(TDSHP_CORING_RATIO_0B, COR_RATIO_UNIT, TDSHP_CORE_RATIO_PBC3_D1);
    vIO32WriteFldAlign(TDSHP_CORING_RATIO_0B, COR_RATIO_UNIT, TDSHP_CORE_RATIO_PBC3_D2);
    vIO32WriteFldAlign(TDSHP_CORING_RATIO_0B, COR_RATIO_UNIT, TDSHP_CORE_RATIO_PBC3_D3);
	
	//No need to set this regster by mtk fw, this control by LGE HAL VPQ
/*
    vIO32WriteFldAlign(TDSHP_CORE_05, wReadQualityTable(QUALITY_PBC_PBC1_CORING_H1), TDSHP_CORE_PBC1_H1);
    vIO32WriteFldAlign(TDSHP_CORE_05, wReadQualityTable(QUALITY_PBC_PBC1_CORING_H2), TDSHP_CORE_PBC1_H2);
    vIO32WriteFldAlign(TDSHP_CORE_05, wReadQualityTable(QUALITY_PBC_PBC1_CORING_H3), TDSHP_CORE_PBC1_H3);
    vIO32WriteFldAlign(TDSHP_CORE_05, wReadQualityTable(QUALITY_PBC_PBC1_CORING_V1), TDSHP_CORE_PBC1_V1);
    vIO32WriteFldAlign(TDSHP_CORE_06, wReadQualityTable(QUALITY_PBC_PBC1_CORING_V2), TDSHP_CORE_PBC1_V2);
    vIO32WriteFldAlign(TDSHP_CORE_06, wReadQualityTable(QUALITY_PBC_PBC1_CORING_V3), TDSHP_CORE_PBC1_V3);
    vIO32WriteFldAlign(TDSHP_CORE_06, wReadQualityTable(QUALITY_PBC_PBC1_CORING_D1), TDSHP_CORE_PBC1_D1);
    vIO32WriteFldAlign(TDSHP_CORE_06, wReadQualityTable(QUALITY_PBC_PBC1_CORING_D2), TDSHP_CORE_PBC1_D2);
    vIO32WriteFldAlign(TDSHP_CORE_07, wReadQualityTable(QUALITY_PBC_PBC1_CORING_D3), TDSHP_CORE_PBC1_D3);

    vIO32WriteFldAlign(TDSHP_CORE_07, wReadQualityTable(QUALITY_PBC_PBC2_CORING_H1), TDSHP_CORE_PBC2_H1);
    vIO32WriteFldAlign(TDSHP_CORE_07, wReadQualityTable(QUALITY_PBC_PBC2_CORING_H2), TDSHP_CORE_PBC2_H2);
    vIO32WriteFldAlign(TDSHP_CORE_07, wReadQualityTable(QUALITY_PBC_PBC2_CORING_H3), TDSHP_CORE_PBC2_H3);
    vIO32WriteFldAlign(TDSHP_CORE_08, wReadQualityTable(QUALITY_PBC_PBC2_CORING_V1), TDSHP_CORE_PBC2_V1);
    vIO32WriteFldAlign(TDSHP_CORE_08, wReadQualityTable(QUALITY_PBC_PBC2_CORING_V2), TDSHP_CORE_PBC2_V2);
    vIO32WriteFldAlign(TDSHP_CORE_08, wReadQualityTable(QUALITY_PBC_PBC2_CORING_V3), TDSHP_CORE_PBC2_V3);
    vIO32WriteFldAlign(TDSHP_CORE_08, wReadQualityTable(QUALITY_PBC_PBC2_CORING_D1), TDSHP_CORE_PBC2_D1);
    vIO32WriteFldAlign(TDSHP_CORE_09, wReadQualityTable(QUALITY_PBC_PBC2_CORING_D2), TDSHP_CORE_PBC2_D2);
    vIO32WriteFldAlign(TDSHP_CORE_09, wReadQualityTable(QUALITY_PBC_PBC2_CORING_D3), TDSHP_CORE_PBC2_D3);

    vIO32WriteFldAlign(TDSHP_CORE_09, wReadQualityTable(QUALITY_PBC_PBC3_CORING_H1), TDSHP_CORE_PBC3_H1);
    vIO32WriteFldAlign(TDSHP_CORE_09, wReadQualityTable(QUALITY_PBC_PBC3_CORING_H2), TDSHP_CORE_PBC3_H2);
    vIO32WriteFldAlign(TDSHP_CORE_0A, wReadQualityTable(QUALITY_PBC_PBC3_CORING_H3), TDSHP_CORE_PBC3_H3);
    vIO32WriteFldAlign(TDSHP_CORE_0A, wReadQualityTable(QUALITY_PBC_PBC3_CORING_V1), TDSHP_CORE_PBC3_V1);
    vIO32WriteFldAlign(TDSHP_CORE_0A, wReadQualityTable(QUALITY_PBC_PBC3_CORING_V2), TDSHP_CORE_PBC3_V2);
    vIO32WriteFldAlign(TDSHP_CORE_0A, wReadQualityTable(QUALITY_PBC_PBC3_CORING_V3), TDSHP_CORE_PBC3_V3);
    vIO32WriteFldAlign(TDSHP_CORE_0B, wReadQualityTable(QUALITY_PBC_PBC3_CORING_D1), TDSHP_CORE_PBC3_D1);
    vIO32WriteFldAlign(TDSHP_CORE_0B, wReadQualityTable(QUALITY_PBC_PBC3_CORING_D2), TDSHP_CORE_PBC3_D2);
    vIO32WriteFldAlign(TDSHP_CORE_0B, wReadQualityTable(QUALITY_PBC_PBC3_CORING_D3), TDSHP_CORE_PBC3_D3);
*/
    vIO32WriteFldAlign(TDSHP_CORETH_05, 0, TDSHP_CORETH_PBC1_H1);
    vIO32WriteFldAlign(TDSHP_CORETH_05, 0, TDSHP_CORETH_PBC1_H2);
    vIO32WriteFldAlign(TDSHP_CORETH_05, 0, TDSHP_CORETH_PBC1_H3);
    vIO32WriteFldAlign(TDSHP_CORETH_05, 0, TDSHP_CORETH_PBC1_V1);
    vIO32WriteFldAlign(TDSHP_CORETH_06, 0, TDSHP_CORETH_PBC1_V2);
    vIO32WriteFldAlign(TDSHP_CORETH_06, 0, TDSHP_CORETH_PBC1_V3);
    vIO32WriteFldAlign(TDSHP_CORETH_06, 0, TDSHP_CORETH_PBC1_D1);
    vIO32WriteFldAlign(TDSHP_CORETH_06, 0, TDSHP_CORETH_PBC1_D2);
    vIO32WriteFldAlign(TDSHP_CORETH_07, 0, TDSHP_CORETH_PBC1_D3);

    vIO32WriteFldAlign(TDSHP_CORETH_07, 0, TDSHP_CORETH_PBC2_H1);
    vIO32WriteFldAlign(TDSHP_CORETH_07, 0, TDSHP_CORETH_PBC2_H2);
    vIO32WriteFldAlign(TDSHP_CORETH_07, 0, TDSHP_CORETH_PBC2_H3);
    vIO32WriteFldAlign(TDSHP_CORETH_08, 0, TDSHP_CORETH_PBC2_V1);
    vIO32WriteFldAlign(TDSHP_CORETH_08, 0, TDSHP_CORETH_PBC2_V2);
    vIO32WriteFldAlign(TDSHP_CORETH_08, 0, TDSHP_CORETH_PBC2_V3);
    vIO32WriteFldAlign(TDSHP_CORETH_08, 0, TDSHP_CORETH_PBC2_D1);
    vIO32WriteFldAlign(TDSHP_CORETH_09, 0, TDSHP_CORETH_PBC2_D2);
    vIO32WriteFldAlign(TDSHP_CORETH_09, 0, TDSHP_CORETH_PBC2_D3);

    vIO32WriteFldAlign(TDSHP_CORETH_09, 0, TDSHP_CORETH_PBC3_H1);
    vIO32WriteFldAlign(TDSHP_CORETH_09, 0, TDSHP_CORETH_PBC3_H2);
    vIO32WriteFldAlign(TDSHP_CORETH_0A, 0, TDSHP_CORETH_PBC3_H3);
    vIO32WriteFldAlign(TDSHP_CORETH_0A, 0, TDSHP_CORETH_PBC3_V1);
    vIO32WriteFldAlign(TDSHP_CORETH_0A, 0, TDSHP_CORETH_PBC3_V2);
    vIO32WriteFldAlign(TDSHP_CORETH_0A, 0, TDSHP_CORETH_PBC3_V3);
    vIO32WriteFldAlign(TDSHP_CORETH_0B, 0, TDSHP_CORETH_PBC3_D1);
    vIO32WriteFldAlign(TDSHP_CORETH_0B, 0, TDSHP_CORETH_PBC3_D2);
    vIO32WriteFldAlign(TDSHP_CORETH_0B, 0, TDSHP_CORETH_PBC3_D3);

    vIO32WriteFldAlign(TDSHP_LIMIT_RATIO_05, 0, TDSHP_LIMIT_RATIO_PBC1_H1);
    vIO32WriteFldAlign(TDSHP_LIMIT_RATIO_05, 0, TDSHP_LIMIT_RATIO_PBC1_H2);
    vIO32WriteFldAlign(TDSHP_LIMIT_RATIO_05, 0, TDSHP_LIMIT_RATIO_PBC1_H3);
    vIO32WriteFldAlign(TDSHP_LIMIT_RATIO_05, 0, TDSHP_LIMIT_RATIO_PBC1_V1);
    vIO32WriteFldAlign(TDSHP_LIMIT_RATIO_06, 0, TDSHP_LIMIT_RATIO_PBC1_V2);
    vIO32WriteFldAlign(TDSHP_LIMIT_RATIO_06, 0, TDSHP_LIMIT_RATIO_PBC1_V3);
    vIO32WriteFldAlign(TDSHP_LIMIT_RATIO_06, 0, TDSHP_LIMIT_RATIO_PBC1_D1);
    vIO32WriteFldAlign(TDSHP_LIMIT_RATIO_06, 0, TDSHP_LIMIT_RATIO_PBC1_D2);
    vIO32WriteFldAlign(TDSHP_LIMIT_RATIO_07, 0, TDSHP_LIMIT_RATIO_PBC1_D3);

    vIO32WriteFldAlign(TDSHP_LIMIT_RATIO_07, 0, TDSHP_LIMIT_RATIO_PBC2_H1);
    vIO32WriteFldAlign(TDSHP_LIMIT_RATIO_07, 0, TDSHP_LIMIT_RATIO_PBC2_H2);
    vIO32WriteFldAlign(TDSHP_LIMIT_RATIO_07, 0, TDSHP_LIMIT_RATIO_PBC2_H3);
    vIO32WriteFldAlign(TDSHP_LIMIT_RATIO_08, 0, TDSHP_LIMIT_RATIO_PBC2_V1);
    vIO32WriteFldAlign(TDSHP_LIMIT_RATIO_08, 0, TDSHP_LIMIT_RATIO_PBC2_V2);
    vIO32WriteFldAlign(TDSHP_LIMIT_RATIO_08, 0, TDSHP_LIMIT_RATIO_PBC2_V3);
    vIO32WriteFldAlign(TDSHP_LIMIT_RATIO_08, 0, TDSHP_LIMIT_RATIO_PBC2_D1);
    vIO32WriteFldAlign(TDSHP_LIMIT_RATIO_09, 0, TDSHP_LIMIT_RATIO_PBC2_D2);
    vIO32WriteFldAlign(TDSHP_LIMIT_RATIO_09, 0, TDSHP_LIMIT_RATIO_PBC2_D3);

    vIO32WriteFldAlign(TDSHP_LIMIT_RATIO_09, 0, TDSHP_LIMIT_RATIO_PBC3_H1);
    vIO32WriteFldAlign(TDSHP_LIMIT_RATIO_09, 0, TDSHP_LIMIT_RATIO_PBC3_H2);
    vIO32WriteFldAlign(TDSHP_LIMIT_RATIO_0A, 0, TDSHP_LIMIT_RATIO_PBC3_H3);
    vIO32WriteFldAlign(TDSHP_LIMIT_RATIO_0A, 0, TDSHP_LIMIT_RATIO_PBC3_V1);
    vIO32WriteFldAlign(TDSHP_LIMIT_RATIO_0A, 0, TDSHP_LIMIT_RATIO_PBC3_V2);
    vIO32WriteFldAlign(TDSHP_LIMIT_RATIO_0A, 0, TDSHP_LIMIT_RATIO_PBC3_V3);
    vIO32WriteFldAlign(TDSHP_LIMIT_RATIO_0B, 0, TDSHP_LIMIT_RATIO_PBC3_D1);
    vIO32WriteFldAlign(TDSHP_LIMIT_RATIO_0B, 0, TDSHP_LIMIT_RATIO_PBC3_D2);
    vIO32WriteFldAlign(TDSHP_LIMIT_RATIO_0B, 0, TDSHP_LIMIT_RATIO_PBC3_D3);

	//No need to set this regster by mtk fw, this control by LGE HAL VPQ
/*
    vIO32WriteFldAlign(TDSHP_LIMIT_05_2, wReadQualityTable(QUALITY_PBC_PBC1_LIMIT_H1), TDSHP_LIMIT_PBC1_H1);
    vIO32WriteFldAlign(TDSHP_LIMIT_05_2, wReadQualityTable(QUALITY_PBC_PBC1_LIMIT_H2), TDSHP_LIMIT_PBC1_H2);
    vIO32WriteFldAlign(TDSHP_LIMIT_05_2, wReadQualityTable(QUALITY_PBC_PBC1_LIMIT_H3), TDSHP_LIMIT_PBC1_H3);
    vIO32WriteFldAlign(TDSHP_LIMIT_05_2, wReadQualityTable(QUALITY_PBC_PBC1_LIMIT_V1), TDSHP_LIMIT_PBC1_V1);
    vIO32WriteFldAlign(TDSHP_LIMIT_06_2, wReadQualityTable(QUALITY_PBC_PBC1_LIMIT_V2), TDSHP_LIMIT_PBC1_V2);
    vIO32WriteFldAlign(TDSHP_LIMIT_06_2, wReadQualityTable(QUALITY_PBC_PBC1_LIMIT_V3), TDSHP_LIMIT_PBC1_V3);
    vIO32WriteFldAlign(TDSHP_LIMIT_06_2, wReadQualityTable(QUALITY_PBC_PBC1_LIMIT_D1), TDSHP_LIMIT_PBC1_D1);
    vIO32WriteFldAlign(TDSHP_LIMIT_06_2, wReadQualityTable(QUALITY_PBC_PBC1_LIMIT_D2), TDSHP_LIMIT_PBC1_D2);
    vIO32WriteFldAlign(TDSHP_LIMIT_07, wReadQualityTable(QUALITY_PBC_PBC1_LIMIT_D3), TDSHP_LIMIT_PBC1_D3);

    vIO32WriteFldAlign(TDSHP_LIMIT_07, wReadQualityTable(QUALITY_PBC_PBC2_LIMIT_H1), TDSHP_LIMIT_PBC2_H1);
    vIO32WriteFldAlign(TDSHP_LIMIT_07, wReadQualityTable(QUALITY_PBC_PBC2_LIMIT_H2), TDSHP_LIMIT_PBC2_H2);
    vIO32WriteFldAlign(TDSHP_LIMIT_07, wReadQualityTable(QUALITY_PBC_PBC2_LIMIT_H3), TDSHP_LIMIT_PBC2_H3);
    vIO32WriteFldAlign(TDSHP_LIMIT_08, wReadQualityTable(QUALITY_PBC_PBC2_LIMIT_V1), TDSHP_LIMIT_PBC2_V1);
    vIO32WriteFldAlign(TDSHP_LIMIT_08, wReadQualityTable(QUALITY_PBC_PBC2_LIMIT_V2), TDSHP_LIMIT_PBC2_V2);
    vIO32WriteFldAlign(TDSHP_LIMIT_08, wReadQualityTable(QUALITY_PBC_PBC2_LIMIT_V3), TDSHP_LIMIT_PBC2_V3);
    vIO32WriteFldAlign(TDSHP_LIMIT_08, wReadQualityTable(QUALITY_PBC_PBC2_LIMIT_D1), TDSHP_LIMIT_PBC2_D1);
    vIO32WriteFldAlign(TDSHP_LIMIT_09, wReadQualityTable(QUALITY_PBC_PBC2_LIMIT_D2), TDSHP_LIMIT_PBC2_D2);
    vIO32WriteFldAlign(TDSHP_LIMIT_09, wReadQualityTable(QUALITY_PBC_PBC2_LIMIT_D3), TDSHP_LIMIT_PBC2_D3);

    vIO32WriteFldAlign(TDSHP_LIMIT_09, wReadQualityTable(QUALITY_PBC_PBC3_LIMIT_H1), TDSHP_LIMIT_PBC3_H1);
    vIO32WriteFldAlign(TDSHP_LIMIT_09, wReadQualityTable(QUALITY_PBC_PBC3_LIMIT_H2), TDSHP_LIMIT_PBC3_H2);
    vIO32WriteFldAlign(TDSHP_LIMIT_0A, wReadQualityTable(QUALITY_PBC_PBC3_LIMIT_H3), TDSHP_LIMIT_PBC3_H3);
    vIO32WriteFldAlign(TDSHP_LIMIT_0A, wReadQualityTable(QUALITY_PBC_PBC3_LIMIT_V1), TDSHP_LIMIT_PBC3_V1);
    vIO32WriteFldAlign(TDSHP_LIMIT_0A, wReadQualityTable(QUALITY_PBC_PBC3_LIMIT_V2), TDSHP_LIMIT_PBC3_V2);
    vIO32WriteFldAlign(TDSHP_LIMIT_0A, wReadQualityTable(QUALITY_PBC_PBC3_LIMIT_V3), TDSHP_LIMIT_PBC3_V3);
    vIO32WriteFldAlign(TDSHP_LIMIT_0B, wReadQualityTable(QUALITY_PBC_PBC3_LIMIT_D1), TDSHP_LIMIT_PBC3_D1);
    vIO32WriteFldAlign(TDSHP_LIMIT_0B, wReadQualityTable(QUALITY_PBC_PBC3_LIMIT_D2), TDSHP_LIMIT_PBC3_D2);
    vIO32WriteFldAlign(TDSHP_LIMIT_0B, wReadQualityTable(QUALITY_PBC_PBC3_LIMIT_D3), TDSHP_LIMIT_PBC3_D3);
*/
    vIO32WriteFldAlign(TDSHP_BOUND_05, 0xFF, TDSHP_BOUND_PBC1_H1);
    vIO32WriteFldAlign(TDSHP_BOUND_05, 0xFF, TDSHP_BOUND_PBC1_H2);
    vIO32WriteFldAlign(TDSHP_BOUND_05, 0xFF, TDSHP_BOUND_PBC1_H3);
    vIO32WriteFldAlign(TDSHP_BOUND_05, 0xFF, TDSHP_BOUND_PBC1_V1);
    vIO32WriteFldAlign(TDSHP_BOUND_06, 0xFF, TDSHP_BOUND_PBC1_V2);
    vIO32WriteFldAlign(TDSHP_BOUND_06, 0xFF, TDSHP_BOUND_PBC1_V3);
    vIO32WriteFldAlign(TDSHP_BOUND_06, 0xFF, TDSHP_BOUND_PBC1_D1);
    vIO32WriteFldAlign(TDSHP_BOUND_06, 0xFF, TDSHP_BOUND_PBC1_D2);
    vIO32WriteFldAlign(TDSHP_BOUND_07, 0xFF, TDSHP_BOUND_PBC1_D3);

    vIO32WriteFldAlign(TDSHP_BOUND_07, 0xFF, TDSHP_BOUND_PBC2_H1);
    vIO32WriteFldAlign(TDSHP_BOUND_07, 0xFF, TDSHP_BOUND_PBC2_H2);
    vIO32WriteFldAlign(TDSHP_BOUND_07, 0xFF, TDSHP_BOUND_PBC2_H3);
    vIO32WriteFldAlign(TDSHP_BOUND_08, 0xFF, TDSHP_BOUND_PBC2_V1);
    vIO32WriteFldAlign(TDSHP_BOUND_08, 0xFF, TDSHP_BOUND_PBC2_V2);
    vIO32WriteFldAlign(TDSHP_BOUND_08, 0xFF, TDSHP_BOUND_PBC2_V3);
    vIO32WriteFldAlign(TDSHP_BOUND_08, 0xFF, TDSHP_BOUND_PBC2_D1);
    vIO32WriteFldAlign(TDSHP_BOUND_09, 0xFF, TDSHP_BOUND_PBC2_D2);
    vIO32WriteFldAlign(TDSHP_BOUND_09, 0xFF, TDSHP_BOUND_PBC2_D3);

    vIO32WriteFldAlign(TDSHP_BOUND_09, 0xFF, TDSHP_BOUND_PBC3_H1);
    vIO32WriteFldAlign(TDSHP_BOUND_09, 0xFF, TDSHP_BOUND_PBC3_H2);
    vIO32WriteFldAlign(TDSHP_BOUND_0A, 0xFF, TDSHP_BOUND_PBC3_H3);
    vIO32WriteFldAlign(TDSHP_BOUND_0A, 0xFF, TDSHP_BOUND_PBC3_V1);
    vIO32WriteFldAlign(TDSHP_BOUND_0A, 0xFF, TDSHP_BOUND_PBC3_V2);
    vIO32WriteFldAlign(TDSHP_BOUND_0A, 0xFF, TDSHP_BOUND_PBC3_V3);
    vIO32WriteFldAlign(TDSHP_BOUND_0B, 0xFF, TDSHP_BOUND_PBC3_D1);
    vIO32WriteFldAlign(TDSHP_BOUND_0B, 0xFF, TDSHP_BOUND_PBC3_D2);
    vIO32WriteFldAlign(TDSHP_BOUND_0B, 0xFF, TDSHP_BOUND_PBC3_D3);
}

#if 0
void vDrvPostSharpParamLimit(void)
{

}
#endif

static void vDrvPBCProc(void)
{
	//can be removed later.
	static UINT16 wPBC1_ThetaC1 = 0xffff;
	static UINT16 wPBC1_ThetaC2 = 0xffff;
	static UINT16 wPBC1_RadiusC1 = 0xffff;
	static UINT16 wPBC1_RadiusC2 = 0xffff;
	static UINT16 wPBC1_Theta_R = 0xffff;
	static UINT16 wPBC1_Radius_R = 0xffff;
	
	static UINT16 wPBC2_ThetaC1 = 0xffff;
	static UINT16 wPBC2_ThetaC2 = 0xffff;
	static UINT16 wPBC2_RadiusC1 = 0xffff;
	static UINT16 wPBC2_RadiusC2 = 0xffff;
	static UINT16 wPBC2_Theta_R = 0xffff;
	static UINT16 wPBC2_Radius_R = 0xffff;
	
	static UINT16 wPBC3_ThetaC1 = 0xffff;
	static UINT16 wPBC3_ThetaC2 = 0xffff;
	static UINT16 wPBC3_RadiusC1 = 0xffff;
	static UINT16 wPBC3_RadiusC2 = 0xffff;	
	static UINT16 wPBC3_Theta_R = 0xffff;
	static UINT16 wPBC3_Radius_R = 0xffff;
	
	// update PBC 1 register
	UINT16 wThetaC1, wThetaC2, wRadiusC1, wRadiusC2, wTheta_R, wRadius_R;

	wThetaC1= IO32ReadFldAlign(CDS_01,CDS1_THETA_C1);
	wThetaC2= IO32ReadFldAlign(CDS_02,CDS1_THETA_C2);
	wRadiusC1= IO32ReadFldAlign(CDS_01,CDS1_RADIUS_C1);
	wRadiusC2= IO32ReadFldAlign(CDS_02,CDS1_RADIUS_C2);
	wTheta_R= IO32ReadFldAlign(CDS_00,CDS1_D_THETA);
	wRadius_R= IO32ReadFldAlign(CDS_00,CDS1_D_RADIUS);
	
	if ((wPBC1_ThetaC1  != wThetaC1) || 
		(wPBC1_ThetaC2  != wThetaC2) || 
		(wPBC1_RadiusC1 != wRadiusC1) ||
		(wPBC1_RadiusC2 != wRadiusC2) ||
		(wPBC1_Theta_R  != wTheta_R) ||		
		(wPBC1_Radius_R != wRadius_R))
	{
		wPBC1_ThetaC1= wThetaC1;
		wPBC1_ThetaC2= wThetaC2;
		wPBC1_RadiusC1= wRadiusC1;
		wPBC1_RadiusC2= wRadiusC2;
		wPBC1_Theta_R= wTheta_R;	
		wPBC1_Radius_R= wRadius_R;
		if ((wPBC1_Theta_R !=0 ) && (wPBC1_Radius_R != 0) && (wPBC1_RadiusC1 != 0))
		{
			vIO32WriteFldAlign(CDS_03, 4096/wPBC1_Radius_R , CDS1_ALPHA);
			vIO32WriteFldAlign(CDS_01, 4096/wPBC1_Theta_R, CDS1_BETA);
			vIO32WriteFldAlign(CDS_00, 4096/MIN(wPBC1_RadiusC1, wPBC1_Radius_R), CDS1_ALPHA_1);
		}
	}

	// update PBC 2 register
	wThetaC1= IO32ReadFldAlign(CDS_2B,CDS2_THETA_C1);
	wThetaC2= IO32ReadFldAlign(CDS_2C,CDS2_THETA_C2);
	wRadiusC1= IO32ReadFldAlign(CDS_2B,CDS2_RADIUS_C1);
	wRadiusC2= IO32ReadFldAlign(CDS_2C,CDS2_RADIUS_C2);
	wTheta_R= IO32ReadFldAlign(CDS_2A,CDS2_D_THETA);
	wRadius_R= IO32ReadFldAlign(CDS_2A,CDS2_D_RADIUS);

	if ((wPBC2_ThetaC1  != wThetaC1) || 
		(wPBC2_ThetaC2  != wThetaC2) || 
		(wPBC2_RadiusC1 != wRadiusC1) ||
		(wPBC2_RadiusC2 != wRadiusC2) ||
		(wPBC2_Theta_R  != wTheta_R) ||		
		(wPBC2_Radius_R != wRadius_R))
	{
		wPBC2_ThetaC1= wThetaC1;
		wPBC2_ThetaC2= wThetaC2;
		wPBC2_RadiusC1= wRadiusC1;
		wPBC2_RadiusC2= wRadiusC2;
		wPBC2_Theta_R= wTheta_R;	
		wPBC2_Radius_R= wRadius_R;	
		if ((wPBC2_Theta_R !=0 ) && (wPBC2_Radius_R != 0) && (wPBC2_RadiusC1 != 0))
		{
			vIO32WriteFldAlign(CDS_2D, 4096/wPBC2_Radius_R, CDS2_ALPHA);
			vIO32WriteFldAlign(CDS_2B, 4096/wPBC2_Theta_R , CDS2_BETA);
			vIO32WriteFldAlign(CDS_2A, 4096/MIN(wPBC2_RadiusC1, wPBC2_Radius_R), CDS2_ALPHA_1);
		}
	}

	// update PBC 3 register
	wThetaC1= IO32ReadFldAlign(CDS_55,CDS3_THETA_C1);
	wThetaC2= IO32ReadFldAlign(CDS_56,CDS3_THETA_C2);
	wRadiusC1= IO32ReadFldAlign(CDS_55,CDS3_RADIUS_C1);
	wRadiusC2= IO32ReadFldAlign(CDS_56,CDS3_RADIUS_C2);
	wTheta_R= IO32ReadFldAlign(CDS_54,CDS3_D_THETA);
	wRadius_R= IO32ReadFldAlign(CDS_54,CDS3_D_RADIUS);

	if ((wPBC3_ThetaC1  != wThetaC1) || 
		(wPBC3_ThetaC2  != wThetaC2) || 
		(wPBC3_RadiusC1 != wRadiusC1) ||
		(wPBC3_RadiusC2 != wRadiusC2) ||
		(wPBC3_Theta_R  != wTheta_R) ||		
		(wPBC3_Radius_R != wRadius_R))
	{
		wPBC3_ThetaC1= wThetaC1;
		wPBC3_ThetaC2= wThetaC2;
		wPBC3_RadiusC1= wRadiusC1;
		wPBC3_RadiusC2= wRadiusC2;
		wPBC3_Theta_R= wTheta_R;	
		wPBC3_Radius_R= wRadius_R;

		if ((wPBC3_Theta_R !=0 ) && (wPBC3_Radius_R != 0) && (wPBC3_RadiusC1 != 0))
		{
			vIO32WriteFldAlign(CDS_57, 4096/wPBC3_Radius_R, CDS3_ALPHA);
			vIO32WriteFldAlign(CDS_55, 4096/wPBC3_Theta_R , CDS3_BETA);
			vIO32WriteFldAlign(CDS_54, 4096/MIN(wPBC3_RadiusC1, wPBC3_Radius_R), CDS3_ALPHA_1);
		}
	}
}

/*****************************************************************************************/
/**********************************   TDSHARP YLEV  ****************************************/
/*****************************************************************************************/
/**
 * @brief Y Level sharpness initial parameters
 * @param
 */
void vDrvYlevSharpInit(void)
{
    vDrvLoadYLevTable();   

	vIO32WriteFldAlign(SHARP_4B, 0x01, TDS_YLEV_EN);
}

void vDrvSetYLevTable(UINT8* pYLev)
{  
	vRegWriteFldAlign(SHARP_47, pYLev[0], TDS_YLEV_P000);  
	vRegWriteFldAlign(SHARP_47, pYLev[1], TDS_YLEV_P064);  
	vRegWriteFldAlign(SHARP_47, pYLev[2], TDS_YLEV_P128);  
	vRegWriteFldAlign(SHARP_47, pYLev[3], TDS_YLEV_P192);
	vRegWriteFldAlign(SHARP_48, pYLev[4], TDS_YLEV_P256); 
	vRegWriteFldAlign(SHARP_48, pYLev[5], TDS_YLEV_P320);  
	vRegWriteFldAlign(SHARP_48, pYLev[6], TDS_YLEV_P384);  
	vRegWriteFldAlign(SHARP_48, pYLev[7], TDS_YLEV_P448);			
	vRegWriteFldAlign(SHARP_49, pYLev[8], TDS_YLEV_P512);  
	vRegWriteFldAlign(SHARP_49, pYLev[9], TDS_YLEV_P576);  
	vRegWriteFldAlign(SHARP_49, pYLev[10], TDS_YLEV_P640); 
	vRegWriteFldAlign(SHARP_49, pYLev[11], TDS_YLEV_P704); 
	vRegWriteFldAlign(SHARP_4A, pYLev[12], TDS_YLEV_P768); 
	vRegWriteFldAlign(SHARP_4A, pYLev[13], TDS_YLEV_P832); 
	vRegWriteFldAlign(SHARP_4A, pYLev[14], TDS_YLEV_P896); 
	vRegWriteFldAlign(SHARP_4A, pYLev[15], TDS_YLEV_P960); 
	vRegWriteFldAlign(SHARP_4B, pYLev[16], TDS_YLEV_P1024);  
	
}


// Write Post sharpness Y Level Table in VSYNC.
void vDrvLoadYLevTable(void)
{  
	vDrvSetYLevTable(TDS_YLEV);
	fgAdapYLEVLock = SV_OFF;
}

// Calculate manual gain curve with 4 control points
void vDrvCalYLevTable(void)
{
    UINT8 i, bIndex, bPoint[4], bGain[4];

    bPoint[0] = IO32ReadFldAlign(TDPROC_YLEV_01, TDS_YLEV_P1);
    bPoint[1] = IO32ReadFldAlign(TDPROC_YLEV_01, TDS_YLEV_P2);
    bPoint[2] = IO32ReadFldAlign(TDPROC_YLEV_01, TDS_YLEV_P3);
    bPoint[3] = IO32ReadFldAlign(TDPROC_YLEV_01, TDS_YLEV_P4);
    bGain[0] = IO32ReadFldAlign(TDPROC_YLEV_02, TDS_YLEV_G1);        
    bGain[1] = IO32ReadFldAlign(TDPROC_YLEV_02, TDS_YLEV_G2);        
    bGain[2] = IO32ReadFldAlign(TDPROC_YLEV_02, TDS_YLEV_G3);
    bGain[3] = IO32ReadFldAlign(TDPROC_YLEV_02, TDS_YLEV_G4);

    for (i = 0; i < 3; i ++)    // Make P1-P4 monotonic increasement
    {
        if (bPoint[i] >= bPoint[i+1])
        {
            bPoint[i] = bPoint[i+1] - 1;
        }
    }

    for (bIndex = 0; bIndex < 17; bIndex ++)
    {
        if (bIndex <= bPoint[0])
        {
            TDS_YLEV[bIndex] = bGain[0];
        }
        else if (bIndex >= bPoint[3])
        {
            TDS_YLEV[bIndex] = bGain[3];
        }
        else
        {
            for (i = 0; i <= 2; i ++)
            {
                if ((bIndex >= bPoint[i]) && ((bIndex <= bPoint[i+1])))
                {
                    TDS_YLEV[bIndex] = (UINT16)((bGain[i+1] - bGain[i]) *
                                    ((UINT16)bIndex - bPoint[i]) / (bPoint[i+1] - bPoint[i])
                                    + bGain[i]);
                    break;
                }
            }
        }
    }
}

/**
 * @brief Calculate adaptive gain curve 
 * @param
 */
void vDrvCalYLevAdlTable(void)
{ 
    UINT8 i;
    UINT16 wTmp;
    UINT8 ADL_GAIN;
      
    ADL_GAIN = IO32ReadFldAlign(TDPROC_YLEV_02, TDS_YLEV_ADL_GAIN); 
    
    // Calculate gain for even points
    for (i = 0; i < 16; i ++)
    {
        wTmp = waLumaArray[2*i + 1] - waLumaArray[2*i];

        // slope = 0x20/wTmp, Gain=((slope*0x80)-0x80)*(Adap_Gain/0x80)+0x80
        TDS_ADAP_YLEV[i ] = (wTmp == 0) ? 0xFF : 
            ((((0x1000/ wTmp) - 0x80) * ADL_GAIN)>> 7) + 0x80;
    }
    // Calculate gain for index 17
    TDS_ADAP_YLEV[16] = (2 * TDS_ADAP_YLEV[15] - TDS_ADAP_YLEV[14]);

    // Multiply adaptive gain curve by manual gain curve
    for (i = 0; i < 17; i ++)
    {
        wTmp = (TDS_ADAP_YLEV[i] * TDS_YLEV[i]) >> 7;
        TDS_YLEV[i] = (wTmp > 0x80) ? 0x80 : wTmp;
    }
}

void vDrvCalYLevAplTable(void)
{
    UINT8 bIndex;
    UINT8 bPoint[2];
    UINT16 wTmp;    	
    UINT8 bAPL = bDrvGetAPL();       
    // YLEV_APL_TBL 	
    bPoint[0] = IO32ReadFldAlign(TDPROC_YLEV_00, TDS_YLEV_APL_ZERO);
    bPoint[1] = IO32ReadFldAlign(TDPROC_YLEV_00, TDS_YLEV_APL_THR);  
    
    if ( bPoint[0]> bPoint[1])	// Make Table monotonic increasement
    {
         bPoint[0] =  bPoint[1];
    }
    
    for (bIndex = 0; bIndex < 17; bIndex ++)	// APLLUT Calculation 
    {
        if (bIndex <= bPoint[0])
        {
            TDS_YLEV_APL[bIndex] = 0x00;
        }
        else if (bIndex >= bPoint[1])
        {
            TDS_YLEV_APL[bIndex] = 0x80;
        }        
        else
        {
            TDS_YLEV_APL[bIndex] = (UINT16)(0x80 *((UINT16)bIndex - bPoint[0]) / (bPoint[1] - bPoint[0]));
        }    
    }
    	  
    for (bIndex = 0; bIndex < 17; bIndex++)	// YLEV Blending by YLEV_APL
    {
        wTmp = (TDS_YLEV[bIndex]*(0x80 - TDS_YLEV_APL[bAPL >> 4] ) + 0x80 * TDS_YLEV_APL[bAPL >> 4] ) >> 7;
        TDS_YLEV[bIndex] = (wTmp > 0x80) ? 0x80 : wTmp;   
    }
}

void DrvCustYLevTable(void)
{
	UINT8 Index;
	
	for(Index=0; Index<17; Index++)
	{
		TDS_YLEV[Index] = (UINT16)(TDS_YLEV[Index] * TDS_YLEV_CUST[Index]) >> 7;		
	}
}
#if 0
void vDrvGlobalGainbyYLEV(void)
{
	UINT32 Tmp1, Tmp2, Tmp3, Tmp4;
	UINT16 wGlobalGain = IO32ReadFldAlign(TDSHP_GLOBAL, TDS_GLOBAL_GAIN);
		
	Tmp1=MIN((wGlobalGain*TDS_YLEV[0])/TDS_GLOBAL_GAIN_UNIT, 0xFF);
	Tmp2=MIN((wGlobalGain*TDS_YLEV[1])/TDS_GLOBAL_GAIN_UNIT, 0xFF);
	Tmp3=MIN((wGlobalGain*TDS_YLEV[2])/TDS_GLOBAL_GAIN_UNIT, 0xFF);
	Tmp4=MIN((wGlobalGain*TDS_YLEV[3])/TDS_GLOBAL_GAIN_UNIT, 0xFF);
	TDS_YLEV_32BIT_VALUE[0]= (Tmp1<<24) + (Tmp2<<16) + (Tmp3<<8) + (Tmp4);
	//LOG("YLEV %d %d %d %d\n", Tmp1, Tmp2, Tmp3, Tmp4);
	Tmp1=MIN((wGlobalGain*TDS_YLEV[4])/TDS_GLOBAL_GAIN_UNIT, 0xFF);
	Tmp2=MIN((wGlobalGain*TDS_YLEV[5])/TDS_GLOBAL_GAIN_UNIT, 0xFF);
	Tmp3=MIN((wGlobalGain*TDS_YLEV[6])/TDS_GLOBAL_GAIN_UNIT, 0xFF);
	Tmp4=MIN((wGlobalGain*TDS_YLEV[7])/TDS_GLOBAL_GAIN_UNIT, 0xFF);
	TDS_YLEV_32BIT_VALUE[1]= (Tmp1<<24) + (Tmp2<<16) + (Tmp3<<8) + (Tmp4);

	Tmp1=MIN((wGlobalGain*TDS_YLEV[8])/TDS_GLOBAL_GAIN_UNIT, 0xFF);
	Tmp2=MIN((wGlobalGain*TDS_YLEV[9])/TDS_GLOBAL_GAIN_UNIT, 0xFF);
	Tmp3=MIN((wGlobalGain*TDS_YLEV[10])/TDS_GLOBAL_GAIN_UNIT, 0xFF);
	Tmp4=MIN((wGlobalGain*TDS_YLEV[11])/TDS_GLOBAL_GAIN_UNIT, 0xFF);
	TDS_YLEV_32BIT_VALUE[2]= (Tmp1<<24) + (Tmp2<<16) + (Tmp3<<8) + (Tmp4);

	Tmp1=MIN((wGlobalGain*TDS_YLEV[12])/TDS_GLOBAL_GAIN_UNIT, 0xFF);
	Tmp2=MIN((wGlobalGain*TDS_YLEV[13])/TDS_GLOBAL_GAIN_UNIT, 0xFF);
	Tmp3=MIN((wGlobalGain*TDS_YLEV[14])/TDS_GLOBAL_GAIN_UNIT, 0xFF);
	Tmp4=MIN((wGlobalGain*TDS_YLEV[15])/TDS_GLOBAL_GAIN_UNIT, 0xFF);
	TDS_YLEV_32BIT_VALUE[3]= (Tmp1<<24) + (Tmp2<<16) + (Tmp3<<8) + (Tmp4);
	
	Tmp1=MIN((wGlobalGain*TDS_YLEV[16])/TDS_GLOBAL_GAIN_UNIT, 0xFF);
	TDS_YLEV_32BIT_VALUE[4]= Tmp1; 
}
#endif

void vDrvAdapYLev(void)
{
    if (IO32ReadFldAlign(TDPROC_YLEV_00, TDS_YLEV_ADAP_ENA))
    {
    	if(fgAdapYLEVLock == SV_OFF)
    	{
    		fgAdapYLEVLock = SV_ON;
	        vDrvCalYLevTable();
	        
	        if (IO32ReadFldAlign(TDPROC_YLEV_00, TDS_YLEV_ADL_ENA))
	        {
	            vDrvCalYLevAdlTable(); 
	        }

	        if (IO32ReadFldAlign(TDPROC_YLEV_00, TDS_YLEV_APL_ENA))
	        {	
	            vDrvCalYLevAplTable();        
	        }              
	        if (IO32ReadFldAlign(TDPROC_YLEV_00, TDS_YLEV_CUST_ENA))
	        {
				if(bGetSignalType(SV_VP_MAIN) == SV_ST_TV)
				{
					DrvCustYLevTable();  // add for ATV ring control.
				}				
	        }
	    }
    }
		//vDrvGlobalGainbyYLEV();
		vApiRegisterVideoEvent(PE_EVENT_SHARPNESS, SV_VP_MAIN, SV_ON);
}

void vDrvTDSharpOffsetInit(void)
{
	vDrvPostSharpGainHOffset(0x80,0x80,0x80);
	vDrvPostSharpGainVOffset(0x80,0x80,0x80);
	vDrvPostSharpGainDOffset(0x80,0x80,0x80);
	vDrvPostSharpCorzHOffset(0x80,0x80,0x80);
	vDrvPostSharpCorzVOffset(0x80,0x80,0x80);
	vDrvPostSharpCorzDOffset(0x80,0x80,0x80);
}

void vDrvPostSharpGainHOffset(UINT8 u1GainH1, UINT8 u1GainH2, UINT8 u1GainH3)
{
    vIO32WriteFldMulti(TDSHP_EXT_OFST_00, 
        P_Fld(u1GainH1, TDS_GAIN_H1_OFST) | 
        P_Fld(u1GainH2, TDS_GAIN_H2_OFST) | 
        P_Fld(u1GainH3, TDS_GAIN_H3_OFST));  
}

void vDrvPostSharpGainVOffset(UINT8 u1GainV1, UINT8 u1GainV2, UINT8 u1GainV3)
{
    vIO32WriteFldMulti(TDSHP_EXT_OFST_01, 
        P_Fld(u1GainV1, TDS_GAIN_V1_OFST) | 
        P_Fld(u1GainV2, TDS_GAIN_V2_OFST) | 
        P_Fld(u1GainV3, TDS_GAIN_V3_OFST));  
}

void vDrvPostSharpGainDOffset(UINT8 u1GainD1, UINT8 u1GainD2, UINT8 u1GainD3)
{
    vIO32WriteFldMulti(TDSHP_EXT_OFST_02, 
        P_Fld(u1GainD1, TDS_GAIN_D1_OFST) | 
        P_Fld(u1GainD2, TDS_GAIN_D2_OFST) | 
        P_Fld(u1GainD3, TDS_GAIN_D3_OFST));  
}


void vDrvPostSharpCorzHOffset(UINT8 u1CorzH1, UINT8 u1CorzH2, UINT8 u1CorzH3)
{
    vIO32WriteFldMulti(TDSHP_EXT_OFST_03, 
        P_Fld(u1CorzH1, TDS_CORZ_H1_OFST) | 
        P_Fld(u1CorzH2, TDS_CORZ_H2_OFST) | 
        P_Fld(u1CorzH3, TDS_CORZ_H3_OFST));  
}

void vDrvPostSharpCorzVOffset(UINT8 u1CorzV1, UINT8 u1CorzV2, UINT8 u1CorzV3)
{
    vIO32WriteFldMulti(TDSHP_EXT_OFST_04, 
        P_Fld(u1CorzV1, TDS_CORZ_V1_OFST) | 
        P_Fld(u1CorzV2, TDS_CORZ_V2_OFST) | 
        P_Fld(u1CorzV3, TDS_CORZ_V3_OFST));  
}

void vDrvPostSharpCorzDOffset(UINT8 u1CorzD1, UINT8 u1CorzD2, UINT8 u1CorzD3)
{
    vIO32WriteFldMulti(TDSHP_EXT_OFST_05, 
        P_Fld(u1CorzD1, TDS_CORZ_D1_OFST) | 
        P_Fld(u1CorzD2, TDS_CORZ_D2_OFST) | 
        P_Fld(u1CorzD3, TDS_CORZ_D3_OFST));  
}

/*****************************************************************************************/
/************************************  3D SHARP ******************************************/
/*****************************************************************************************/
UINT16 wDrvGetTDShp3DGain(UINT16 wInputGain, UINT16 wWeighting)
{
    return ((wInputGain*wWeighting)>>7);
}

void vDrvGetSharp3DWeighting(void)
{
    UINT8 bMode;
	UINT8 b3DHGain, b3DDGain, b3DVGain;
	UINT8 b3DHClip, b3DDClip, b3DVClip;
    bMode = IO32ReadFldAlign(SHARPNESS_3D_00, SHARPNESS_3D_MODE);
    
    switch (bMode)
    {    
		   case E_TDTV_UI_3D_MODE_TTD:		   // 2D to 3D
			   b3DHGain = IO32ReadFldAlign(SHARPNESS_3D_02, SHARPNESS_TTD_HGAIN); 
			   b3DDGain = IO32ReadFldAlign(SHARPNESS_3D_02, SHARPNESS_TTD_DGAIN);				   
			   b3DVGain = IO32ReadFldAlign(SHARPNESS_3D_02, SHARPNESS_TTD_VGAIN); 
			   b3DHClip = IO32ReadFldAlign(SHARPNESS_3D_0B, SHARPNESS_TTD_HCLIP); 
			   b3DDClip = IO32ReadFldAlign(SHARPNESS_3D_0B, SHARPNESS_TTD_DCLIP); 
			   b3DVClip = IO32ReadFldAlign(SHARPNESS_3D_0B, SHARPNESS_TTD_VCLIP); 
			   break;				 
		   case E_TDTV_UI_3D_MODE_FS:		   //Frame Seq & Frame Packing
			   b3DHGain = IO32ReadFldAlign(SHARPNESS_3D_01, SHARPNESS_FS_HGAIN); 
			   b3DDGain = IO32ReadFldAlign(SHARPNESS_3D_01, SHARPNESS_FS_DGAIN);				  
			   b3DVGain = IO32ReadFldAlign(SHARPNESS_3D_01, SHARPNESS_FS_VGAIN);
			   b3DHClip = IO32ReadFldAlign(SHARPNESS_3D_0A, SHARPNESS_FS_HCLIP); 
			   b3DDClip = IO32ReadFldAlign(SHARPNESS_3D_0A, SHARPNESS_FS_DCLIP); 
			   b3DVClip = IO32ReadFldAlign(SHARPNESS_3D_0A, SHARPNESS_FS_VCLIP);			   
			   break;				 
		   case E_TDTV_UI_3D_MODE_TB:			   //Top and Bottom
			   b3DHGain = IO32ReadFldAlign(SHARPNESS_3D_04, SHARPNESS_TB_HGAIN); 
			   b3DDGain = IO32ReadFldAlign(SHARPNESS_3D_04, SHARPNESS_TB_DGAIN);				  
			   b3DVGain = IO32ReadFldAlign(SHARPNESS_3D_04, SHARPNESS_TB_VGAIN);
			   b3DHClip = IO32ReadFldAlign(SHARPNESS_3D_0D, SHARPNESS_TB_HCLIP); 
			   b3DDClip = IO32ReadFldAlign(SHARPNESS_3D_0D, SHARPNESS_TB_DCLIP); 
			   b3DVClip = IO32ReadFldAlign(SHARPNESS_3D_0D, SHARPNESS_TB_VCLIP);			   
			   break;
		   case E_TDTV_UI_3D_MODE_SBS:		   //Side by Side
			   b3DHGain = IO32ReadFldAlign(SHARPNESS_3D_03, SHARPNESS_SBS_HGAIN); 
			   b3DDGain = IO32ReadFldAlign(SHARPNESS_3D_03, SHARPNESS_SBS_DGAIN); 
			   b3DVGain = IO32ReadFldAlign(SHARPNESS_3D_03, SHARPNESS_SBS_VGAIN); 
			   b3DHClip = IO32ReadFldAlign(SHARPNESS_3D_0C, SHARPNESS_SBS_HCLIP); 
			   b3DDClip = IO32ReadFldAlign(SHARPNESS_3D_0C, SHARPNESS_SBS_DCLIP); 
			   b3DVClip = IO32ReadFldAlign(SHARPNESS_3D_0C, SHARPNESS_SBS_VCLIP);			   
			   break;
		   case E_TDTV_UI_3D_MODE_RD:		   //RealD
			   b3DHGain = IO32ReadFldAlign(SHARPNESS_3D_06, SHARPNESS_RD_HGAIN); 
			   b3DDGain = IO32ReadFldAlign(SHARPNESS_3D_06, SHARPNESS_RD_DGAIN);				  
			   b3DVGain = IO32ReadFldAlign(SHARPNESS_3D_06, SHARPNESS_RD_VGAIN);
			   b3DHClip = IO32ReadFldAlign(SHARPNESS_3D_0F, SHARPNESS_RD_HCLIP); 
			   b3DDClip = IO32ReadFldAlign(SHARPNESS_3D_0F, SHARPNESS_RD_DCLIP); 
			   b3DVClip = IO32ReadFldAlign(SHARPNESS_3D_0F, SHARPNESS_RD_VCLIP);			   
			   break;
		   case E_TDTV_UI_3D_MODE_SS:			   //Sensio
			   b3DHGain = IO32ReadFldAlign(SHARPNESS_3D_05, SHARPNESS_SS_HGAIN); 
			   b3DDGain = IO32ReadFldAlign(SHARPNESS_3D_05, SHARPNESS_SS_HGAIN);				  
			   b3DVGain = IO32ReadFldAlign(SHARPNESS_3D_05, SHARPNESS_SS_VGAIN); 
			   b3DHClip = IO32ReadFldAlign(SHARPNESS_3D_0E, SHARPNESS_SS_HCLIP); 
			   b3DDClip = IO32ReadFldAlign(SHARPNESS_3D_0E, SHARPNESS_SS_DCLIP); 
			   b3DVClip = IO32ReadFldAlign(SHARPNESS_3D_0E, SHARPNESS_SS_VCLIP);			   
			   break;
		case E_TDTV_UI_3D_MODE_LI:			   //Line Interleave		 
			   b3DHGain = IO32ReadFldAlign(SHARPNESS_3D_07, SHARPNESS_LI_HGAIN); 
			   b3DDGain = IO32ReadFldAlign(SHARPNESS_3D_07, SHARPNESS_LI_DGAIN);				  
			   b3DVGain = IO32ReadFldAlign(SHARPNESS_3D_07, SHARPNESS_LI_VGAIN); 
			   b3DHClip = IO32ReadFldAlign(SHARPNESS_3D_10, SHARPNESS_LI_HCLIP); 
			   b3DDClip = IO32ReadFldAlign(SHARPNESS_3D_10, SHARPNESS_LI_DCLIP); 
			   b3DVClip = IO32ReadFldAlign(SHARPNESS_3D_10, SHARPNESS_LI_VCLIP);			   
			   break;
		   case E_TDTV_UI_3D_MODE_CB:		   //Checker Board
			   b3DHGain = IO32ReadFldAlign(SHARPNESS_3D_08, SHARPNESS_CB_HGAIN); 
			   b3DDGain = IO32ReadFldAlign(SHARPNESS_3D_08, SHARPNESS_CB_DGAIN);				  
			   b3DVGain = IO32ReadFldAlign(SHARPNESS_3D_08, SHARPNESS_CB_VGAIN); 
			   b3DHClip = IO32ReadFldAlign(SHARPNESS_3D_11, SHARPNESS_CB_HCLIP); 
			   b3DDClip = IO32ReadFldAlign(SHARPNESS_3D_11, SHARPNESS_CB_DCLIP); 
			   b3DVClip = IO32ReadFldAlign(SHARPNESS_3D_11, SHARPNESS_CB_VCLIP);			   
			   break;
		   case E_TDTV_UI_3D_MODE_DA:		   //Dot Alternative
			   b3DHGain = IO32ReadFldAlign(SHARPNESS_3D_09, SHARPNESS_DA_HGAIN); 
			   b3DDGain = IO32ReadFldAlign(SHARPNESS_3D_09, SHARPNESS_DA_DGAIN);				  
			   b3DVGain = IO32ReadFldAlign(SHARPNESS_3D_09, SHARPNESS_DA_VGAIN);
			   b3DHClip = IO32ReadFldAlign(SHARPNESS_3D_12, SHARPNESS_DA_HCLIP); 
			   b3DDClip = IO32ReadFldAlign(SHARPNESS_3D_12, SHARPNESS_DA_DCLIP); 
			   b3DVClip = IO32ReadFldAlign(SHARPNESS_3D_12, SHARPNESS_DA_VCLIP);			   
			   break;
		   case E_TDTV_UI_3D_MODE_OFF:
		   default:
			   b3DHGain = 0x80; 
			   b3DDGain = 0x80; 
			   b3DVGain = 0x80; 
			   b3DHClip = 0x80; 
			   b3DDClip = 0x80; 
			   b3DVClip = 0x80;				   
			   break;
    }
	vIO32WriteFldAlign(SHARPNESS_3D_13, b3DHGain, SHARPNESS_3D_HGAIN);
	vIO32WriteFldAlign(SHARPNESS_3D_13, b3DDGain, SHARPNESS_3D_DGAIN);
	vIO32WriteFldAlign(SHARPNESS_3D_13, b3DVGain, SHARPNESS_3D_VGAIN);
	vIO32WriteFldAlign(SHARPNESS_3D_14, b3DHClip, SHARPNESS_3D_HCLIP);
	vIO32WriteFldAlign(SHARPNESS_3D_14, b3DDClip, SHARPNESS_3D_DCLIP);
	vIO32WriteFldAlign(SHARPNESS_3D_14, b3DVClip, SHARPNESS_3D_VCLIP);	
}

/*****************************************************************************************/
/**********************************   PBC MNR   ****************************************/
/*****************************************************************************************/

#define WINDOW_SIZE 4320
#define BIN_NUM 9
#define POSITION_MAX 5

enum
{
	UP_HIST =0,
	DOWN_HIST,		
	LEFT_HIST,	
	RIGHT_HIST,
	CENTER_HIST,
	WAIT,	
	UPDATE_PBC,
	STOP,
};

UINT16 wHueHist_WinPos[POSITION_MAX][4]=
{
    {0x0,   0x77F,  0x0,    0x8F},
    {0x0,   0x77F,  0x3A7,  0x437},
    {0x0,   0xFF,   0x0,    0x437},
    {0x67F, 0x77F,  0x0,    0x437},
    {0x2A0, 0x4DF,  0x12C,  0x30B}
};

UINT32 dwPBCSetting[BIN_NUM-1][6]=
{
    {0x48, 0x3F8, 0x8, 0x374, 0x38C, 4},
    {0x48, 0x3F8, 0x8, 0x394, 0x3A0, 4},
    {0x48, 0x3F8, 0x8, 0x3A8, 0x3B0, 4},
    {0x48, 0x3F8, 0x8, 0x3B8, 0x3C4, 4},
    {0x48, 0x3F8, 0x8, 0x3CC, 0x3D4, 4},
    {0x48, 0x3F8, 0x8, 0x3DC, 0x3E8, 4},
    {0x48, 0x3F8, 0x8, 0x3F0, 0xC,   4},
    {0x16, 0x2c,  0x8, 0x0,   0x3FF, 0},
};

static const UCHAR *WindowPosition[POSITION_MAX] =
{
    "Up ",    "Down ",    "Left ",    "Right ",    "Center ",
};

static UINT16 wHueHist[POSITION_MAX][BIN_NUM];

static void vDrvSetHueWinPos(UINT8 bWinPos)
{
	LOG(2, "Update Hist for %s Window\n", WindowPosition[bWinPos]);

	vIO32WriteFldMulti(SAT_HIST_CFG_MAIN_3,
	                  P_Fld(wHueHist_WinPos[bWinPos][0], COLOR_HIST_X_START) |
	                  P_Fld(wHueHist_WinPos[bWinPos][1], COLOR_HIST_X_END));
	vIO32WriteFldMulti(SAT_HIST_CFG_MAIN_4,
	                  P_Fld(wHueHist_WinPos[bWinPos][2], COLOR_HIST_Y_START) |
	                  P_Fld(wHueHist_WinPos[bWinPos][3], COLOR_HIST_Y_END));    
}

static UINT8 bDrvGetWeight(UINT8 bMin, UINT8 bMax, UINT8 bInput)
{
	UINT8 bAreaWeight=0;
	
	if(bInput>=bMin && bInput<=bMax && ((bMax-bMin)!=0))
	{
		bAreaWeight = ((bInput-bMin)*10)/(bMax-bMin);
	}
	else if(bInput<bMin)
	{
		bAreaWeight = 0;
	}
	else if(bInput>bMax)
	{
		bAreaWeight = 10;
	}
	else
	{
		LOG(2, "Error Happen !!! \n\n");
	}
	return bAreaWeight;
}

static void vDrvRemoveNoisebyPBC(void)
{
	UINT16 wTmp, wMaxBinCnt,wCenterBinCnt,wCenterDiff;
	UINT8 bIdxBin, bIdxPos, bMaxBinIdx, bBkgConf, bBkgArea, bAreaDiff, bAreaWeight, bAreaDiffWeight;
	UINT8 bTmpBottom, bTmpUp;

	LOG(2, "Do PBC PROC \n");
	//Find maximum position

	wMaxBinCnt=0x0;
	bMaxBinIdx=0x0;
	for(bIdxBin=0; bIdxBin<BIN_NUM; bIdxBin++)
	{
		wTmp=0x0;
		for(bIdxPos=0; bIdxPos<POSITION_MAX; bIdxPos++)
		{
			if(wTmp < wHueHist[bIdxPos][bIdxBin])
			{
				wTmp=wHueHist[bIdxPos][bIdxBin];					
			}
		}
		if(wMaxBinCnt<wTmp)
		{
			wMaxBinCnt = wTmp;
			bMaxBinIdx = bIdxBin;
		}
	}

	wCenterBinCnt= wHueHist[CENTER_HIST][bMaxBinIdx];
	wCenterDiff = ((wCenterBinCnt>wMaxBinCnt)?0:(wMaxBinCnt-wCenterBinCnt));
	LOG(2, "MaxBin : %d, MaxBinHist : %d\n", bMaxBinIdx, wMaxBinCnt);
	
	bAreaDiff = (wCenterDiff*100)/WINDOW_SIZE;
	bBkgArea = (wMaxBinCnt*100)/WINDOW_SIZE;

	bTmpBottom = IO32ReadFldAlign(TDSHP_PBCNR_00, AREA_BOTTOM);
	bTmpUp=		 IO32ReadFldAlign(TDSHP_PBCNR_00, AREA_UP);
	bAreaWeight = bDrvGetWeight(bTmpBottom, bTmpUp, bBkgArea);
	
	bTmpBottom = IO32ReadFldAlign(TDSHP_PBCNR_00, DIFF_BOTTOM);
	bTmpUp=		 IO32ReadFldAlign(TDSHP_PBCNR_00, DIFF_UP);		
	bAreaDiffWeight = bDrvGetWeight(bTmpBottom, bTmpUp, bAreaDiff);

	bBkgConf = bAreaDiffWeight*bAreaWeight;
	
	if(bBkgConf>IO32ReadFldAlign(TDSHP_PBCNR_01, CONF_TH) && bMaxBinIdx!=0)
	{
		vIO32WriteFldAlign(CDS_2B, dwPBCSetting[bMaxBinIdx-1][0], CDS2_RADIUS_C1);
		vIO32WriteFldAlign(CDS_2C, dwPBCSetting[bMaxBinIdx-1][1], CDS2_RADIUS_C2);
		vIO32WriteFldAlign(CDS_2A, dwPBCSetting[bMaxBinIdx-1][2], CDS2_D_RADIUS);			
		vIO32WriteFldAlign(CDS_2B, dwPBCSetting[bMaxBinIdx-1][3], CDS2_THETA_C1);
		vIO32WriteFldAlign(CDS_2C, dwPBCSetting[bMaxBinIdx-1][4], CDS2_THETA_C2);
		vIO32WriteFldAlign(CDS_2A, dwPBCSetting[bMaxBinIdx-1][5], CDS2_D_THETA);
		vIO32WriteFldAlign(CDS_2A, SV_ON, CDS2_EN);
	}
	else
	{
		vIO32WriteFldAlign(CDS_2A, SV_OFF, CDS2_EN);
	}

	LOG(2, "bAreaDiff : %d, bBkgArea : %d, bConfidence %d \n", bAreaDiff, bBkgArea, bBkgConf);
}

static void vDrvGetHueHistInfo(UINT8 bPos)
{
	UINT32 dwSum;

	wHueHist[bPos][0] = IO32ReadFldAlign(HUE_HIST_1_0_MAIN, HUE_HIST_0);
	wHueHist[bPos][1] = IO32ReadFldAlign(HUE_HIST_1_0_MAIN, HUE_HIST_1);
	wHueHist[bPos][2] = IO32ReadFldAlign(HUE_HIST_3_2_MAIN, HUE_HIST_2);
	wHueHist[bPos][3] = IO32ReadFldAlign(HUE_HIST_3_2_MAIN, HUE_HIST_3);
	wHueHist[bPos][4] = IO32ReadFldAlign(HUE_HIST_5_4_MAIN, HUE_HIST_4);
	wHueHist[bPos][5] = IO32ReadFldAlign(HUE_HIST_5_4_MAIN, HUE_HIST_5);
	wHueHist[bPos][6] = IO32ReadFldAlign(HUE_HIST_7_6_MAIN, HUE_HIST_6);
	wHueHist[bPos][7] = IO32ReadFldAlign(HUE_HIST_7_6_MAIN, HUE_HIST_7);
	dwSum = wHueHist[bPos][0]  
		  + wHueHist[bPos][1]
		  + wHueHist[bPos][2]
		  + wHueHist[bPos][3]
		  + wHueHist[bPos][4]
		  + wHueHist[bPos][5]
		  + wHueHist[bPos][6]
		  + wHueHist[bPos][7];
	wHueHist[bPos][8] = ((dwSum>WINDOW_SIZE)?0:(WINDOW_SIZE-dwSum)); 
	LOG(2, "Read Pos %d \n", (bPos));
	LOG(2, "Hist0 %d \n", wHueHist[bPos][0]);
	LOG(2, "Hist1 %d \n", wHueHist[bPos][1]);
	LOG(2, "Hist2 %d \n", wHueHist[bPos][2]);
	LOG(2, "Hist3 %d \n", wHueHist[bPos][3]);
	LOG(2, "Hist4 %d \n", wHueHist[bPos][4]);
	LOG(2, "Hist5 %d \n", wHueHist[bPos][5]);
	LOG(2, "Hist6 %d \n", wHueHist[bPos][6]);
	LOG(2, "Hist7 %d \n", wHueHist[bPos][7]);
	LOG(2, "Hist8 %d \n", wHueHist[bPos][8]);
	LOG(2, "\n");
}

void vDrvPBCMNRProc(void)
{
	static UINT8 bCnt=0;
	static UINT8 bHueHistWinPos = 0xFF;

    if(bInitGetFlg)
    {
        bCDS2OnOff = IO32ReadFldAlign(CDS_2A, CDS2_EN);
        bInitGetFlg = SV_FALSE; 
    }

	if(IO32ReadFldAlign(TDSHP_PBCNR_01, PBCNR_ONOFF))
	{
		if(bADLSceneChange || IO32ReadFldAlign(ADAPTIVE_SCE_REG, ADAPTIVE_FORCE_SHPSCENECHG))
		{
			bHueHistWinPos=UP_HIST;
			vIO32WriteFldAlign(ADAPTIVE_SCE_REG, SV_OFF, ADAPTIVE_FORCE_SHPSCENECHG);
		}
		//Need wait 4 loops for Histogram ready.
		if(bCnt%4==0)
		{
			switch(bHueHistWinPos)
			{
				case UP_HIST:
					vDrvSetHueWinPos(UP_HIST);				
					break;
				case DOWN_HIST:
					vDrvGetHueHistInfo(UP_HIST);				
					vDrvSetHueWinPos(DOWN_HIST);
					break;				
				case LEFT_HIST:
					vDrvGetHueHistInfo(DOWN_HIST);
					vDrvSetHueWinPos(LEFT_HIST);
					break;				
				case RIGHT_HIST:
					vDrvGetHueHistInfo(LEFT_HIST);
					vDrvSetHueWinPos(RIGHT_HIST);
					break;				
				case CENTER_HIST:	
					vDrvGetHueHistInfo(RIGHT_HIST);
					vDrvSetHueWinPos(CENTER_HIST);
					break;				
				case WAIT:
					vDrvGetHueHistInfo(CENTER_HIST);
					break;
				case UPDATE_PBC:
					vDrvRemoveNoisebyPBC();
					break;
				case STOP:
				default:
					break;
			}
			bHueHistWinPos =((bHueHistWinPos>=STOP)?(STOP):(bHueHistWinPos+1));
		}
		bCnt++;
		bInitSetFlg = SV_TRUE; 
	}
	else
	{
        if(bInitSetFlg)
        {
    		vIO32WriteFldAlign(CDS_2A, bCDS2OnOff, CDS2_EN);
            bInitSetFlg = SV_FALSE;
        }
	}
}

/*****************************************************************************************/
/**********************************   Sharpness Control   ****************************************/
/*****************************************************************************************/
void vDrvSharpProc(void)
{

#if defined(__MODEL_slt__)
	vDrvSharpnessBypass(SV_OFF);
	return ;
#endif

    // PBC Driving
    vDrvPBCProc();   

	// AC calculation
	vDrv3DSharpParamCalcu();
	vDrvSharpCalcuACCurve(TDS_H1, TDS_H3);
    vDrvSharpCalcuACCurve(TDS_V1, TDS_V3);	
    vDrvSharpCalcuACCurve(TDS_D1, TDS_D3);	
    vDrvSharpCalcuACCurve(TDS_LTI_HL, TDS_LTI_V);
    vDrvSharpCalcuACCurve(CDS1_H1, CDS3_D3);
    
    // Adaptive Sharpness : Y level(Pixel value), adaptive luma, APL
    vDrvAdapYLev(); 

    // Special Adaptive Sharpness
	vDrvPBCMNRProc();
}

void vDrvPostSharpInit(void)
{	
	UINT8 i;
    
	for(i=0; i< TDS_BAND_NUM; i++)
	{
		bTDS_UpdateFlg[i]=0;
	}
    
    vDrvLoadRegTbl(REGTBL_TDSHARP);
    vDrvLoadRegTbl(REGTBL_PBC);    
    vDrvYlevSharpInit();
	vDrvTDSharpOffsetInit();
    
	vIO32WriteFldAlign(TDPROC_MISC_00, SV_OFF, TDPROC_BYPASS_ALL);
	vIO32WriteFldMulti(TDSHP_PBCNR_00,
                  P_Fld(80, AREA_UP) |
                  P_Fld(50, AREA_BOTTOM) |
                  P_Fld(30, DIFF_UP) |
                  P_Fld(10, DIFF_BOTTOM));   
	vIO32WriteFldAlign(TDSHP_PBCNR_01, 32, CONF_TH);
	vIO32WriteFldAlign(TDSHP_PBCNR_01, SV_OFF, PBCNR_ONOFF);
}


/*****************************************************************************************/
/**********************************   Pattern Gen   ****************************************/
/*****************************************************************************************/
void vDrvMainSharpPatGen(UINT8 bOnOff)
{
	if (bOnOff == SV_ON)
	{
        vIO32WriteFldAlign(TDPROC_MISC_00, 1, TDPROC_PATGEN_DATA_EN);
		vIO32WriteFldAlign(TDPROC_MISC_00, 0, TDPROC_PATGEN_MODE);
		vIO32WriteFldAlign(TDPROC_MISC_00, 1, TDPROC_PATGEN_YSEL);
		vIO32WriteFldAlign(TDPROC_MISC_00, 1, TDPROC_PATGEN_USEL);
		vIO32WriteFldAlign(TDPROC_MISC_00, 1, TDPROC_PATGEN_VSEL);
	}
	else
	{
		vIO32WriteFldAlign(TDPROC_MISC_00, 0, TDPROC_PATGEN_DATA_EN);
		vIO32WriteFldAlign(TDPROC_MISC_00, 0, TDPROC_PATGEN_YSEL);
		vIO32WriteFldAlign(TDPROC_MISC_00, 0, TDPROC_PATGEN_USEL);
		vIO32WriteFldAlign(TDPROC_MISC_00, 0, TDPROC_PATGEN_VSEL);
	}
}
void vDrvSetGlobalCoring(UINT8 bValue)
{

	vIO32WriteFldAlign(TDSHP_GLOBAL, bValue, TDS_GLOBAL_CORING);

}

/*****************************************************************************************/
/*************************************   Post Sharpness Bypass ******************************/
/*****************************************************************************************/
// for CLI debug
void vDrvSharpnessBypass(UINT8 bOnOff)
{
    vIO32WriteFldAlign(TDPROC_MISC_00, !bOnOff, TDPROC_BYPASS_ALL);
    vIO32WriteFldAlign(CBE_05, !bOnOff, BYPASS_CBE);
    vScpipUpdateDelay();
}

/*****************************************************************************************/
/*********************************    Bypass Status (delay) ******************************/
/*****************************************************************************************/
UINT8 u1DrvSharpnessIsBypass(void)
{
    return (IO32ReadFldAlign(TDPROC_MISC_00, TDPROC_BYPASS_ALL));
}

UINT8 u1DrvCBEIsBypass(void)
{
#if defined(CC_MT5399)
    return (IO32ReadFldAlign(CBE_05, BYPASS_CBE));
#elif defined(CC_MT5882)
    return SV_TRUE;
#endif    
}
/*****************************************************************************************/
/**********************************   DEMO MODE   ****************************************/
/*****************************************************************************************/
void vDrvSetPostSharpDemoRange(Region rRegion)
{
    //Define demo region
    vIO32WriteFldAlign(SP_MASK_00, rRegion.wHStart, CH1_H_DMWINDOW_START);
    vIO32WriteFldAlign(SP_MASK_00, rRegion.wHEnd, CH1_H_DMWINDOW_END);// 2DSharp : start < active < End                                                                                                  
    vIO32WriteFldAlign(SP_MASK_01, rRegion.wVStart, CH1_V_DMWINDOW_START); // 2D Sharp Demo mode Voffset
    vIO32WriteFldAlign(SP_MASK_01, rRegion.wVEnd, CH1_V_DMWINDOW_END);// 2D Sharp Demo mode Voffset
}

void vDrv2DSharpDemo(UINT8 bOnOff, UINT8 bInside)
{
    #ifdef CC_FLIP_MIRROR_SUPPORT
	if((u4QueryFlipModule(SV_VP_MAIN) & (FLIP_BY_SCALER | FLIP_BY_PSCAN | FLIP_BY_B2R) )&&(u1GetFlipMirrorConfig()& SYS_MIRROR_CONFIG_ON))
	{bInside = !bInside;}
    #endif
    
	vIO32WriteFldAlign(TDPROC_MISC_00, bInside, TDS_DEMO_SWAP);
    vIO32WriteFldAlign(TDPROC_MISC_00, bOnOff, TDS_DEMO_ENA);  
}

void vDrvECTIDemo(UINT8 bOnOff, UINT8 bInside)
{
    #ifdef CC_FLIP_MIRROR_SUPPORT
	if((u4QueryFlipModule(SV_VP_MAIN) & (FLIP_BY_SCALER | FLIP_BY_PSCAN | FLIP_BY_B2R) )&&(u1GetFlipMirrorConfig()& SYS_MIRROR_CONFIG_ON))
	{bInside = !bInside;}
    #endif

    vIO32WriteFldAlign(TDPROC_MISC_00, bInside, ECTI_DEMO_SWAP);
    vIO32WriteFldAlign(TDPROC_MISC_00, bOnOff, ECTI_DEMO_ENA);
}
void vDrvHSharpDemo(UINT8 bOnOff, UINT8 bRegion, UINT16 wXCenter)
{
#ifdef CC_FLIP_MIRROR_SUPPORT
	if((u4QueryFlipModule(SV_VP_MAIN) & (FLIP_BY_PSCAN | FLIP_BY_B2R) )&&(u1GetFlipMirrorConfig()& SYS_MIRROR_CONFIG_ON))
	{bRegion = !bRegion;}
#endif

    // set demo side window left
    vIO32WriteFldMulti(HSHARP_0E, P_Fld(0, DEMO_LBOUND)|P_Fld(wXCenter, DEMO_RBOUND));
    vIO32WriteFldMulti(HSHARP_0D, P_Fld(bOnOff, DEMO_EN)|P_Fld(bRegion, HBOUND_INV));
}

