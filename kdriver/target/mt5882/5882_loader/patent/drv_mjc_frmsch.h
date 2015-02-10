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
 * $Author: p4admin $
 * $Date: 2015/02/10 $
 * $RCSfile: drv_mjc_frmsch.h $
 * $Revision: #1 $
 *
 *----------------------------------------------------------------------------*/
#ifndef _DRV_MJC_FRMSCH_H_
#define _DRV_MJC_FRMSCH_H_

/*----------------------------------------------------------------------------*
 * Header Files
 *----------------------------------------------------------------------------*/
#include "hw_mjc.h"
#include "sv_const.h"
#include "typedef.h"
#include "video_def.h"

#ifndef SUPPORT_UNIVERSAL_CADENCE
#define SUPPORT_UNIVERSAL_CADENCE 1
#endif

#ifndef SUPPORT_LOGO_SHOOT_HANDLING
#define SUPPORT_LOGO_SHOOT_HANDLING 1
#endif

//#define MOVE_FMDET_TO_OVSYNC

#define SUPPORT_3D 1

typedef struct
{
    UINT8 u1ParamRdy;
    UINT8 u1LDetFilmMode;
    UINT8 u1RDetFilmMode;
    UINT8 u1FilmType;
    UINT8 u1PdInfo;             /*PdInfo before serveral frame delays, say 7, and shifted by PD_INFO_SFT*/
    UINT8 u1PdInfoNormalize;
    UINT8 u1InWrpt;             /*Write Ptr before serveral frame delays, say 7*/
    UINT8 u1InWrptBufIdx;       /*Current index of u1InWrpt*/
    UINT8 u1FrmIn;
    UINT8 u1FrmOut;
    UINT8 u1LatestPdInfo;       /*The latest PdInfo*/
    UINT8 u1PriorLstPdInfo;     /*The PdInfo prior to the latest one*/
    UINT8 u1PDSccCnt;
    UINT8 u1DFIEnter;
} MJC_FRM_SCH_IN_PARAM_T;

typedef struct
{
    UINT8 u1IntpNumPrev;
    UINT8 u1OutId;

    UINT8 u1FrmPrev;
    UINT8 u1FrmCurr;
    UINT8 u1FrmNext;
    UINT8 u1MeFilmIdx;
    UINT8 u1MeFilmPhs;
    UINT8 u1MeFilmFlg;
    UINT8 u1ScenChg;
    UINT8 u1IntpNum;
    UINT8 u1IntpDen;

    UINT8 u1RescanFrmEn;
    UINT8 u1PPCModeSel;
    UINT8 u1PPCFrmEn;

    UINT8 u1McOriEn;
    UINT8 u1Me2En;
    UINT8 u1Me3En;
    UINT8 u1MePosSel;
    UINT8 u1DaBwrdcEn;
    UINT8 u1DaRdctype;
    UINT8 u1McNum;

    UINT8 u1TdLrSelOut;
    UINT8 u1TdLrToggle;

    UINT8 u1WrtCtrlEn;
    UINT8 u1WrtFrmPtr;
    UINT8 u1WrtFrmEn;

    UINT8 u1PairFirst;
    UINT8 u1PPCIntlvSel;
    UINT8 u1Fallback64;
    UINT8 u1MvexpFlag;
    UINT8 u1Me2460Prebi;

    UINT8 u1MemcCcoDeci;
    UINT8 u1MemcPcoDeci;
    UINT8 u1Fallback55P2;
    UINT8 u1HqrptChg;
    UINT8 u1SecHqrptChg;
    UINT8 u1FstHqrptGp;
    UINT8 u1HqrptLst;
    UINT8 u1FrmDysw;
    UINT8 u1VideoNoIntp;
    UINT8 u1FrmChgTtd;
} MJC_FRM_SCH_OUT_PARAM_T;

typedef struct
{
    UINT32 u1Fm05NoCtrl          : 3; // LSB
    UINT32 u1FwRescanFrmEn       : 1;
    UINT32 u1FwPrevShift         : 5;
    UINT32 u1FwCurrShift         : 5;
    UINT32 u1FwNextShift         : 5;
    UINT32 u1FwMeFilmIdx         : 4;
    UINT32 u1FwMeFilmPhs         : 1;
    UINT32 u1FwMeFilmFlg         : 2;
    UINT32 u1FwIntNum            : 5;
    UINT32 u1FwTdLrSelOut        : 1; // MSB
} MJC_FM_05_T;

typedef union
{
    UINT32      u4Fm05Reg;
    MJC_FM_05_T arFm05;
} MJC_FM_05_U;

typedef struct
{
    UINT32 u1FwMe2En             : 1; // LSB
    UINT32 u1FwMe3En             : 1;
    UINT32 u1FwDaBwrdcEn         : 1;
    UINT32 u1FwDaRdctype         : 2;
    UINT32 u1FwPairFirstDly      : 8;
    UINT32 u1FwMePosOthrSel      : 2;
    UINT32 u1FwPPCModeSel        : 1;
    UINT32 u1FwMcOriEn           : 1;
    UINT32 u1FwPPCFrmEn          : 1;
    UINT32 u1FwMe2460Prebi       : 1;
    UINT32 u1FwMvexpFlag         : 1;
    UINT32 u1Fm06NoCtrl1         : 1;
    UINT32 u1FwFallback64        : 1;
    UINT32 u1FwPPCIntlvSel       : 1;
    UINT32 u1FwPairFirst         : 1;
    UINT32 u1Fm06NoCtrl2         : 7;
    UINT32 u1FwTdLrToggle        : 1; // MSB
} MJC_FM_06_T;

typedef union
{
    UINT32      u4Fm06Reg;
    MJC_FM_06_T arFm06;
} MJC_FM_06_U;

typedef struct
{
    UINT32 u1Fm07NoCtrl1         :17; // LSB
    UINT32 u1FwMemcPcoDeciEn     : 1;
    UINT32 u1FwMemcCcoDeciEn     : 1;
    UINT32 u1FwIntDen            : 4;
    UINT32 u1FwFallback55p2      : 1;
    UINT32 u1FwHqrptChg          : 1;
    UINT32 u1FwSecHqrptChg       : 1;
    UINT32 u1FwFstHqrptGp        : 1;
    UINT32 u1FwHqrptLst          : 1;
    UINT32 u1Fm07NoCtrl2         : 2;
    UINT32 u1FwVideoNoIntp       : 1;
    UINT32 u1FwFrmChgTtd         : 1; // MSB
} MJC_FM_07_T;

typedef union
{
    UINT32      u4Fm07Reg;
    MJC_FM_07_T arFm07;
} MJC_FM_07_U;

typedef struct
{
    UINT32 u1FwFrmPrevR          : 5; // LSB
    UINT32 u1FwFrmCurrR          : 5;
    UINT32 u1FwFrmNextR          : 5;
    UINT32 u1Fm0ANoCtrl          :17; // MSB
} MJC_FM_0A_T;

typedef union
{
    UINT32      u4Fm0AReg;
    MJC_FM_0A_T arFm0A;
} MJC_FM_0A_U;

typedef struct
{
    UINT32 u1FmMcNumRgn0         : 8; // LSB
    UINT32 u1FmMcNumRgn1         : 8;
    UINT32 u1FmMcNumRgn2         : 8;
    UINT32 u1Fm16NoCtrl1         : 8; // MSB
} MJC_FM_16_T;

typedef struct
{
    UINT32 u1FmWrpt                    : 4;
	UINT32 u1FmPdInfoBufIdx            : 4; 
    UINT32 u1FmWrptDouble              : 4;
	UINT32 u1FmPdInfoBufIdxDouble      : 4; 
	UINT32 u1FmTblIdxSet0              : 1; 
    UINT32 u1Fm08NoCtrl1               : 15; // MSB
} MJC_FM_CTRL_T;

typedef union
{
    UINT32      u4Fm16Reg;
    MJC_FM_16_T arFm16;
} MJC_FM_16_U;

enum
{
    PD_INFO_VIDEO_0,
    PD_INFO_VIDEO_1,

    PD_INFO_32_1_0,
    PD_INFO_32_1_1,
    PD_INFO_32_2_0,
    PD_INFO_32_2_1,
    PD_INFO_32_3_0,
    PD_INFO_32_3_1,
    PD_INFO_32_4_0,
    PD_INFO_32_4_1,
    PD_INFO_32_5_0,
    PD_INFO_32_5_1,

    PD_INFO_22_6_0,
    PD_INFO_22_6_1,
    PD_INFO_22_7_0,
    PD_INFO_22_7_1,
/*
    PD_INFO_22323_08_0,
    PD_INFO_22323_08_1,
    PD_INFO_22323_09_0,
    PD_INFO_22323_09_1,
    PD_INFO_22323_10_0,
    PD_INFO_22323_10_1,
    PD_INFO_22323_11_0,
    PD_INFO_22323_11_1,
    PD_INFO_22323_12_0,
    PD_INFO_22323_12_1,
    PD_INFO_22323_13_0,
    PD_INFO_22323_13_1,
    PD_INFO_22323_14_0,
    PD_INFO_22323_14_1,
    PD_INFO_22323_15_0,
    PD_INFO_22323_15_1,
    PD_INFO_22323_16_0,
    PD_INFO_22323_16_1,
    PD_INFO_22323_17_0,
    PD_INFO_22323_17_1,
    PD_INFO_22323_18_0,
    PD_INFO_22323_18_1,
    PD_INFO_22323_19_0,
    PD_INFO_22323_19_1,
*/
    PD_INFO_MAX
};
typedef enum
{	
	FILM_TYPE_42222 = 0x00,
	FILM_TYPE_22323 = 0x01,
    FILM_TYPE_21111 = 0x02,
	FILM_TYPE_3223	= 0x03,
	FILM_TYPE_2224	= 0x04,
	FILM_TYPE_32	= 0x05,
	FILM_TYPE_22	= 0x06,
	VIDEO_TYPE		= 0x07,
	FILM_TYPE_MAX
} E_FILM_MODE_TYPE;

#define SW_FILM_AUTO_42222  (1 << FILM_TYPE_42222)
#define SW_FILM_AUTO_22323  (1 << FILM_TYPE_22323)
#define SW_FILM_AUTO_21111  (1 << FILM_TYPE_21111)
#define SW_FILM_AUTO_3223   (1 << FILM_TYPE_3223)
#define SW_FILM_AUTO_2224   (1 << FILM_TYPE_2224)
#define SW_FILM_AUTO_32     (1 << FILM_TYPE_32)
#define SW_FILM_AUTO_22     (1 << FILM_TYPE_22)
#define SW_FILE_AUTO_ALL    ((1 << VIDEO_TYPE) - 1)

void vDrvMJCFrmSchInit(void);
void vDrvMJCFrmSchHWLog(void* pvArgv);
void vDrvMJCFrmSchFWLog(void* pvArgv);
void vDrvMJCFrmSchCadenceLog(void* pvArgv);
void vDrvMJCFrmSchHWFWLog(void* pvArgv);
void vDrvMJCFrmSchInputVsyncProc(void);
void vDrvMJCFrmSchOutputVsyncProc(void);
void vDrvMJCFrmSchSetPdInfoDly(UINT32 u4Delay);
void vDrvMJCFrmSchSetFWOnOff(UINT8 u1OnOff);
void vDrvMJCSetFrmSchPdInfoSwDly(UINT8 u2PdInfo22Shift, UINT8 u2PdInfo32Shift);
UINT8 fgDrvMJCFilmDetectDIOrMJC(void);
UINT8 u1DrvMJCFrmSchGetFWOnOff(void);

#endif //_DRV_MJC_FRMSCH_H_

