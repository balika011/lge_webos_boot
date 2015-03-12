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

/**
 * @file HDMI control driver
 *
 * @note PWR5V -> vHDMIHPDHigh()
 * @note !PWR5V -> vHDMIHPDLow()
 * @note INIT(see below) -> PWOFF -> PWR5V -> PWON -> PWON2 (TERM_ON, RX_EN, HPD_HIGH)
 * -> SCDT (reset fifo) -> AUTH
 * @note !PWR5V -> INIT (HPD_LOW, TERM_OFF, RESET, MUTE)
 * @note vVdoModeChgDone -> fgApiVideoIsVgaMode -> _rMChannel.bIs444 = SV_TURE or SV_FALSE
 *       fgApiVideoIsVgaMode -> _bHdmiMode -> _bDviTiming -> _rMChannel.bIs444 -> _bMAIN_COLOR_SYS = SV_COLOR_RGB
 *
 * @author chiachi_chiang@mtk.com.tw
 * @author darren_weng@mtk.com.tw
 * @author daniel_hsiao@mtk.com.tw
 */
#include "x_lint.h"

LINT_EXT_HEADER_BEGIN

#include "general.h"
#include "hw_hdmi.h"
#include "drv_hdmi.h"
#include "hw_dvi.h"
#include "hw_sys.h"
#include "drv_video.h"
#include "video_def.h"
#include "util.h"
#include "hw_vdoin.h"
//#include "hw_hdtv.h"
//#include "api_audio.h"
#include "util.h"
#include "aud_if.h"
#include "aud_drvif.h"
#include "drv_ycproc.h"
#include "nptv_debug.h"
#include "x_timer.h"
#include "x_pinmux.h"
#include "x_printf.h"
#include "x_hal_5381.h"
#include "eeprom_if.h"
#include "drv_dvi.h"
#include "x_ckgen.h"
#include "x_bim.h"
#include "drvcust_if.h"
#include "x_ckgen.h"
#include "hw_ckgen.h"
#include "eepdef.h"
#include "drv_dvi.h"
#include "hw_pdwnc.h"
#include "drv_tdtv_drvif.h"
#include "source_select.h"
#include "sif_sw_drvif.h"
#if defined(CC_BYPASS_AVI_INFOFRAME) || defined(CC_SUPPORT_HDMI_FBOOTING)
#include "api_eep.h"
#endif
#include "cbus_if.h"
#if defined(CC_MT5882)
#include "drv_cbus_cpi_if.h"
#endif

//#include "X_pdwnc.h"
LINT_EXT_HEADER_END
#define IC_3x3color_test 1
#define DEBUG_HDMI_STATE_CHG 0
#define DEBUG_HDMI_INT 0
#define HDMI_DEBUG 1
#define HDMI_PING 0
#define MT8202_ECN 1
#define HDMI_SELFTESTING 0
#define ECO_0914 1
#define TMDS_EQ_CUSTOM 1 // josh
#define CCIR_HV_Position 0
#define AUD_MUTE_PERIOD 2   // 2 second

#define HDMI_Audio_NewFlow  1

#define ANA_INTF_1_31_16 Fld(16, 16, AC_FULLW32) //31:16

#define PixelInvCutFreq      160 // 2 second

#define Enable_HW_Mute		1
#define FLD_HDMI_CLK_Crystral Fld(1, 20, AC_MSKB2) //20

#define Protect_XCLKInPCLk		1
#define 	XCLKInPCLkCNT		5
#define   XCLKInPCLkCNTMid    2	// (XCLKInPCLkCNT-1)/2
// SLT test
#ifdef __MODEL_slt__
#define __HDMI_SLT__ 1  // for SLT testing
#endif

#define HDMI_RES_STABLE 10
#define HDMI_HDCP_STABLE 30

#define AVMUTE_TIMEOUT 6 // unit: sec

#ifdef CC_SUPPORT_HDMI_FBOOTING
UINT32 _u4HdmiLoops = 0;
UINT8 u1HdmiInit = 0;
#endif
UINT8 u1EdidInit = 0;
UINT8 u1HdcpInit = 0;

extern UINT16 _wDviVTotal;
extern UINT16 _wDviHtotal;

extern UINT16 _wDviWidth;
extern UINT16 _wDviHeight;

static HDMI_EQCH_T _arEq[HDMI_SWITCH_MAX];
#define Diff(a, b)  (((a)>(b))?((a)-(b)):((b)-(a)))

#define ReHPD    0
UINT8   _bHDMIAudioInit;
UINT8   _bAudHdmiFlg;
UINT8   _bHDMISampleChange;
UINT8   _bSmpFrq = AUD_FS_44K;
UINT8   _bHDMIRangeMode = DRV_CUSTOM_HDMI_RANGE_AUTO;
UINT8   _bHDMIRange = SV_HDMI_RANGE_FORCE_AUTO;
static UINT8 _bHdmiAudFreq;
PRIVATE BOOL _fgVideoOn = FALSE;
//PRIVATE BOOL _fgVideoStableAudioUnstable = FALSE;
//Audio format configuration
PRIVATE AIN_CFG_T _rAudCfg;

#ifdef CC_SUPPORT_HDMI_CLK5VDET
UINT8  _bNonStdDev;
UINT8  _b5VActive;
UINT8  _b5VDetCnt;
UINT8  _bTmdsActive;
UINT8  _bTmdsDetCnt;
UINT8  _bHpdPulse;
#endif
UINT8 bHPDInitFlag = 0;
HAL_TIME_T bInitUnplugTime;

UINT8 _bHdmiSrcGood = 0;
UINT8 _bCcir2status = 0;
#if HDMI_Audio_NewFlow
UINT8 _bHDMIAudFIFOflag;
#endif
#ifdef SYS_MHL_SUPPORT
#if defined(CC_MT5399)//only for 99,except 82.
extern UINT8  _bIsMhlDeviceConnect;
#endif
#endif

/* Force 3D */
BOOL  _fgForce3D;
UINT8 _bForce3DStructure;
UINT8 _bForce3DExtData;
/* Force 3D to 2D */
BOOL  _fgForce2D;

UINT8 _bEQTuning;
UINT8 _bKPGain;
UINT8 _bRXMode;
UINT8 _bEQGain;
UINT8 _bZSel;
UINT8 _bVRef;
HDMI_SPD_DATA_STRUCT u4SpdData;
UINT8 u1UPStaNew,u1UPStaOld;
UINT8 u1SVStaNew,u1SVStaOld;
UINT8 u1HdmiSpdUPStatChk;
UINT8 u1HdmiSpdSVStatChk;
UINT8 u1IsSPDFlag;
UINT8 u1SpdTest;
UINT8 UPChgStaSave;
UINT8 SVChgStaSave;

#ifdef CC_Support_Sil9285
extern void vHDMISwitchMainLoop(void);
#endif
EXTERN INT32 OSD_SC_GetScalerInfo(UINT32 u4Scaler, UINT32* pu4Enable,
                                  UINT32* pu4SrcW, UINT32* pu4SrcH,
                                  UINT32* pu4DstW, UINT32* pu4DstH,
                                  UINT32* pu4Is16Bpp);
EXTERN void vDviInitial(void);

extern UINT8   _bIntMuteCnt ;

static BOOL _fgHdmiAudInit = FALSE;
static BOOL _fgAudMute = FALSE;
static UINT8 _u1mutecount = 0;
static UINT8 u1HWmute = 0;
#ifdef CC_HDMI_CONFIG_BOARD
static E_HDMI_BOARD_TYPE eBoardType = ATSC_INT_EDID;
#endif
#define IS_AUD_MUTE()   _fgAudMute

#define hdmi_audio 0

// Configuration
#define HDMI_AUD_FMT_CHG_PROTECT
#define SUPPORT_HDMI_AUDIO
// Constant define
#define HDMI_AUD_OK               0
#define HDMI_AUD_UNSTABLE_LVL_1   1
#define HDMI_AUD_UNSTABLE_LVL_2   2
#define HDMI_AUD_UNSTABLE_LVL_3   3

#define FREQCOUNTS_24MHZ ((UINT32)(1024 * 24 * 1000) * 100)
#define FREQCOUNTS_27MHZ ((UINT32)(1024 * 27 * 1000) * 100)

UINT32 u4XtalfreqCnt = FREQCOUNTS_24MHZ;

UINT32 u4AnalogRegBase = 0x100;
UINT32 u4HdmiRegBase = 0xc00;

UINT32 u4ActiveAnalogRegBase = 0x100;
UINT32 u4ActiveHdmiRegBase = 0xc00;

E_HDMI_SWITCH_NUM eActiveHdmiPort = HDMI_SWITCH_INIT;
E_HDMI_SWITCH_NUM eCurrHdmiPort = HDMI_SWITCH_INIT;
E_HDMI_SWITCH_NUM  eHDMICurrSwitch = HDMI_SWITCH_INIT;
E_HDMI_SWITCH_NUM eActivePort = HDMI_SWITCH_INIT;

extern UINT16 _wDviWidth;
extern UINT16 _wDviHeight;

extern UINT32   _bDviMdChgCnt;
extern UINT8   _bDviDeChgCnt;
extern UINT8   _bDviPixClkChgCnt;

extern UINT8 _bDviModeChged;
extern void vCloseIec(void);
extern void vOpenIec(void);

#ifdef CC_support_i2c_switch
extern void vHDMISwitchDDCBusCtrl(UINT8,UINT8);
extern void vHDMISwitchCLKTerm(UINT8,UINT8);
#endif


HDMI_STATUS_T _arHdmiRx[HDMI_SWITCH_MAX];

#define MT53XX_PREAMBLE_CTI_MASK  (0xf << 8)
#define MT53XX_AUD_MUTE     (0x1 << 25)
#define MT53XX_BYP_SYNC     (0x1 << 17)
#define MT53XX_BYP_DVIFILT  (0x1 << 18)

#define MT53XX_FIFO_RESET   (0x1 << 9)

#define Enable_TDFIFO_RESET	1    // WT.Chang command for deep color

#define DEFAULTKEYS 0


#define HD_KM0 0xf002C0F8
#define HD_KS0 0xf002C0FC
#define HD_KM1 0xf002CCF8
#define HD_KS1 0xf002CCFC
#define HD_KM2 0xf002C8F8
#define HD_KS2 0xf002C8FC
#define HD_KM3 0xf002C4F8
#define HD_KS3 0xf002C4FC
#define HD_DEV0 0xf0029300
#define HD_DEV1 0xf0029304
#define HD_DEV2 0xf0029308
#define HD_DEV3 0xf002930c
#define HD_ADDR 0xf0029310
#define HD_ADDA 0xf0029314
#define HD_SRS0 0xf002cc04
#define HD_SRS1 0xf002c804
#define HD_SRS2 0xf002c404
#define HD_SRS3 0xf002c004
#define HD_DDC0 0xf002cc08
#define HD_DDC1 0xf002c808
#define HD_DDC2 0xf002c408
#define HD_DDC3 0xf002c008

static UINT8 _au1Hdcp[320] =
{
0x00 ,0x14 ,0x7c ,0xe6 ,0x2c ,0x37 ,0xf3 ,0xe8 ,0xa8 ,0x4d ,0x27 ,0x66 ,0xa8 ,0xd0 ,0x2f ,0x13,
0x55 ,0x79 ,0x97 ,0xe7 ,0x87 ,0x37 ,0x18 ,0x04 ,0xd1 ,0x5f ,0x3b ,0x12 ,0x9f ,0x32 ,0xa4 ,0x6f,
0x58 ,0x1c ,0x8a ,0xbc ,0x8c ,0x7f ,0xe3 ,0xcb ,0x81 ,0x9e ,0xb4 ,0x45 ,0x7c ,0x66 ,0x6a ,0xcc,
0xdd ,0x5c ,0xc8 ,0x17 ,0xa3 ,0x90 ,0x43 ,0x91 ,0x8b ,0x01 ,0xf0 ,0x1d ,0xff ,0x8a ,0x1b ,0xde,
0x94 ,0xd6 ,0xb6 ,0xf6 ,0x6d ,0x15 ,0x7b ,0x42 ,0x13 ,0xa2 ,0x1b ,0x04 ,0xb5 ,0xdd ,0x11 ,0xcc,
0x42 ,0x28 ,0x66 ,0x65 ,0xf5 ,0x77 ,0xf1 ,0x65 ,0xc3 ,0x8c ,0x9b ,0x2c ,0xad ,0xb4 ,0xe9 ,0x7c,
0xd1 ,0xbc ,0xd6 ,0x4a ,0x5d ,0xf7 ,0x45 ,0x3e ,0x2a ,0x22 ,0xec ,0xa8 ,0xdf ,0x68 ,0x54 ,0x57,
0x5a ,0x10 ,0xc8 ,0x38 ,0x9f ,0x94 ,0xa0 ,0xa7 ,0xa0 ,0x71 ,0xa2 ,0x67 ,0x8e ,0x23 ,0xbd ,0x8d,
0x63 ,0x89 ,0x0d ,0x01 ,0x91 ,0x97 ,0x4c ,0xba ,0x5c ,0x4d ,0x94 ,0x73 ,0x36 ,0x68 ,0x12 ,0x6c,
0xe8 ,0xfa ,0xb1 ,0x51 ,0xc1 ,0x93 ,0xc6 ,0xce ,0x72 ,0x90 ,0xc1 ,0x6b ,0x4d ,0xf6 ,0x63 ,0x02,
0xd3 ,0xa6 ,0x9b ,0x80 ,0x35 ,0xb6 ,0xa9 ,0xff ,0x8e ,0xfd ,0xd9 ,0x6f ,0x24 ,0xa6 ,0xdb ,0x4c,
0xd2 ,0x0c ,0x0f ,0xcf ,0xcd ,0x1a ,0x19 ,0xe4 ,0x62 ,0x9c ,0x6d ,0x17 ,0x6b ,0x57 ,0x39 ,0xcb,
0x6a ,0x0d ,0x80 ,0x75 ,0xfa ,0xf3 ,0x69 ,0x7d ,0x9f ,0x79 ,0xe3 ,0xc0 ,0x8b ,0x5a ,0xd2 ,0xa8,
0xc9 ,0xd9 ,0x90 ,0x93 ,0xaa ,0xe6 ,0x1a ,0x1e ,0x17 ,0x93 ,0x03 ,0x2d ,0x43 ,0xc0 ,0xaf ,0x33,
0x94 ,0x66 ,0xa9 ,0x18 ,0x55 ,0xcc ,0x22 ,0xf5 ,0x23 ,0xc8 ,0xc5 ,0x37 ,0xf1 ,0x81 ,0xd2 ,0x96,
0xaf ,0x0a ,0x5a ,0xe5 ,0x8a ,0x13 ,0xef ,0x63 ,0x19 ,0x4c ,0xc6 ,0x3f ,0x6c ,0x9a ,0x7a ,0xb2,
0xa6 ,0xd4 ,0x31 ,0x5a ,0x30 ,0x01 ,0x21 ,0xcd ,0xa2 ,0x86 ,0x74 ,0x04 ,0x46 ,0x3e ,0x38 ,0x08,
0xe7 ,0x57 ,0xa7 ,0xb4 ,0x41 ,0x73 ,0x02 ,0x78 ,0x93 ,0x34 ,0x93 ,0xce ,0x93 ,0x5a ,0x46 ,0xd3,
0x0a ,0x14 ,0xf7 ,0x61 ,0x03 ,0xb7 ,0x8f ,0x22 ,0xd2 ,0xf1 ,0x3a ,0x98 ,0xba ,0x28 ,0x62 ,0x53,
0xca ,0x4c ,0x1d ,0x19 ,0xce ,0x49 ,0x87 ,0xf7 ,0xb5 ,0x26 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00
};
#if (DEFAULTKEYS == 1)
static UINT8 _au1Edid[260] =
{
  0x00, 0xFF, 0xFF, 0xFF,  0xFF, 0xFF, 0xFF, 0x00, 0x36, 0x8B, 0x01, 0x00,  0x01, 0x01, 0x01, 0x01,
  0x01, 0x0F, 0x01, 0x03,  0x80, 0x3C, 0x22, 0x78, 0x0A, 0x0D, 0xC9, 0xA0,  0x57, 0x47, 0x98, 0x27,
  0x12, 0x48, 0x4C, 0xBF,  0xEF, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,  0x01, 0x01, 0x01, 0x01,
  0x01, 0x01, 0x01, 0x01,  0x01, 0x01, 0x01, 0x1D, 0x00, 0x72, 0x51, 0xD0,  0x1E, 0x20, 0x6E, 0x28,
  0x55, 0x00, 0xC4, 0x8E,  0x21, 0x00, 0x00, 0x1E, 0x01, 0x1D, 0x80, 0x18,  0x71, 0x1C, 0x16, 0x20,
  0x58, 0x2C, 0x25, 0x00,  0xC4, 0x8E, 0x21, 0x00, 0x00, 0x9E, 0x00, 0x00,  0x00, 0xFC, 0x00, 0x4D,
  0x54, 0x4B, 0x20, 0x4C,  0x43, 0x44, 0x54, 0x56, 0x0A, 0x20, 0x20, 0x20,  0x00, 0x00, 0x00, 0xFD,
  0x00, 0x31, 0x4C, 0x0F,  0x50, 0x0E, 0x00, 0x0A, 0x20, 0x20, 0x20, 0x20,  0x20, 0x20, 0x01, 0x56,
  0x02, 0x03, 0x26, 0x74,  0x4B, 0x84, 0x10, 0x1F, 0x05, 0x13, 0x14, 0x01,  0x02, 0x11, 0x06, 0x15,
  0x26, 0x09, 0x7F, 0x03,  0x11, 0x7F, 0x18, 0x83, 0x01, 0x00, 0x00, 0x6A,  0x03, 0x0C, 0x00, 0x10,
  0x00, 0xB8, 0x2D, 0x2F,  0x80, 0x00, 0x01, 0x1D, 0x00, 0xBC, 0x52, 0xD0,  0x1E, 0x20, 0xB8, 0x28,
  0x55, 0x40, 0xC4, 0x8E,  0x21, 0x00, 0x00, 0x1E, 0x01, 0x1D, 0x80, 0xD0,  0x72, 0x1C, 0x16, 0x20,
  0x10, 0x2C, 0x25, 0x80,  0xC4, 0x8E, 0x21, 0x00, 0x00, 0x9E, 0x8C, 0x0A,  0xD0, 0x8A, 0x20, 0xE0,
  0x2D, 0x10, 0x10, 0x3E,  0x96, 0x00, 0x13, 0x8E, 0x21, 0x00, 0x00, 0x18,  0x8C, 0x0A, 0xD0, 0x90,
  0x20, 0x40, 0x31, 0x20,  0x0C, 0x40, 0x55, 0x00, 0x13, 0x8E, 0x21, 0x00,  0x00, 0x18, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  0x00, 0x00, 0x00, 0x9D,
  // PA address
  0x10, 0x20, 0x30, 0x40
};
#endif

enum
{
  HDMI_DEBUG_NONE,
  HDMI_DEBUG_HDCP_MONITOR = 85,
  HDMI_DEBUG_HDCP_STATUS,
  HDMI_DEBUG_EDID_DUMP,
  HDMI_DEBUG_MAX
};

PRIVATE void vHDMISwRst(void)
{
     vIO32WriteFldAlign(SRST + u4HdmiRegBase,1,SW_RST);
      vUtDelay2us(1);
     vIO32WriteFldAlign(SRST + u4HdmiRegBase,0,SW_RST);
}

PRIVATE void vHDMIHDCPRst(void)
{
    vIO32Write4BMsk(SRST + u4HdmiRegBase, Fld2Msk32(HDCP_RST), Fld2Msk32(HDCP_RST));
    vIO32Write4BMsk(SRST + u4HdmiRegBase, 0 , Fld2Msk32(HDCP_RST));
}


static UINT8 u1IsHdmiRxAvailable(E_HDMI_SWITCH_NUM ePort)
{
    UINT8 u1Ret = 0;
#if  defined(CC_MT5389)

    switch(ePort)
    {
        case HDMI_SWITCH_1:
        case HDMI_SWITCH_2:
        case HDMI_SWITCH_3:
            u1Ret = 1;
            break;

        default:
            break;
    }

#elif  defined(CC_MT5368)

    switch(ePort)
    {
        case HDMI_SWITCH_1:
        case HDMI_SWITCH_2:
        case HDMI_SWITCH_3:
        case HDMI_SWITCH_4:
            u1Ret = 1;
            break;

        default:
            break;
    }

#elif defined(CC_MT5396) || defined(CC_MT5398) || defined(CC_MT5399)

    switch(ePort)
    {
        case HDMI_SWITCH_1:
        case HDMI_SWITCH_2:
        case HDMI_SWITCH_3:
        case HDMI_SWITCH_4:
            u1Ret = 1;
            break;

        default:
            break;
    }

#elif defined(CC_MT5880)|| defined(CC_MT5882) /*Add CC_MT5882 George.yang 2014-7-17*/

    switch(ePort)
    {
        case HDMI_SWITCH_1:
        case HDMI_SWITCH_2:
        case HDMI_SWITCH_3:
            u1Ret = 1;
            break;

        default:
            break;
    }


//#else
//#error "No define for u1IsHdmiRxAvaliable"
#endif
    return (u1Ret);
}

static UINT32 u4GetHdmiRegBase(E_HDMI_SWITCH_NUM ePort)
{
    UINT32 u4TmpHdmiRegBase = 0xc00;
#if  defined(CC_MT5389)

    switch(ePort)
    {
        default:
        case HDMI_SWITCH_1:
            u4TmpHdmiRegBase = 0xc00;
            break;

        case HDMI_SWITCH_2:
            u4TmpHdmiRegBase = 0xc00;
            break;

        case HDMI_SWITCH_3:
            u4TmpHdmiRegBase = 0x800;
            break;
    }

#elif  defined(CC_MT5368)

    switch(ePort)
    {
        default:
        case HDMI_SWITCH_1:
            u4TmpHdmiRegBase = 0xc00;
            break;

        case HDMI_SWITCH_2:
            u4TmpHdmiRegBase = 0xc00;
            break;

        case HDMI_SWITCH_3:
            u4TmpHdmiRegBase = 0x800;
            break;

        case HDMI_SWITCH_4:
            u4TmpHdmiRegBase = 0x400;
            break;
    }

#elif defined(CC_MT5396) || defined(CC_MT5398) || \
      defined(CC_MT5880) || defined(CC_MT5399)||   \
      defined(CC_MT5882) /*Add CC_MT5882 George.yang 2014-7-17*/

    switch(ePort)
    {
        default:
        case HDMI_SWITCH_1:
            u4TmpHdmiRegBase = 0xc00;
            break;

        case HDMI_SWITCH_2:
            u4TmpHdmiRegBase = 0x800;
            break;

        case HDMI_SWITCH_3:
            u4TmpHdmiRegBase = 0x400;
            break;
#ifndef CC_MT5880
        case HDMI_SWITCH_4:
            u4TmpHdmiRegBase = 0x0;
            break;
#endif
    }

#endif
    return (u4TmpHdmiRegBase);
}

static UINT32 u4GetAnalogRegBase(E_HDMI_SWITCH_NUM ePort)
{
    UINT32 u4TmpAnalogRegBase = 0x0;
#if  defined(CC_MT5389)

    switch(ePort)
    {
        default:
        case HDMI_SWITCH_1:
            u4TmpAnalogRegBase = 0x100;
            break;

        case HDMI_SWITCH_2:
            u4TmpAnalogRegBase = 0x100;
            break;

        case HDMI_SWITCH_3:
            u4TmpAnalogRegBase = 0x0;
            break;
    }

#elif  defined(CC_MT5368)

    switch(ePort)
    {
        default:
        case HDMI_SWITCH_1:
            u4TmpAnalogRegBase = 0x100;
            break;

        case HDMI_SWITCH_2:
            u4TmpAnalogRegBase = 0x100;
            break;

        case HDMI_SWITCH_3:
            u4TmpAnalogRegBase = 0x200;
            break;

        case HDMI_SWITCH_4:
            u4TmpAnalogRegBase = 0x000;
            break;
    }

#elif defined(CC_MT5396) || defined(CC_MT5398) || \
      defined(CC_MT5880) || defined(CC_MT5399) 
 
    switch(ePort)
    {
        default:
        case HDMI_SWITCH_1:
            u4TmpAnalogRegBase = 0x0;
            break;

        case HDMI_SWITCH_2:
            u4TmpAnalogRegBase = 0x100;
            break;

        case HDMI_SWITCH_3:
            u4TmpAnalogRegBase = 0x200;
            break;

#ifndef CC_MT5880  
        case HDMI_SWITCH_4:
            u4TmpAnalogRegBase = 0x300;
            break;
#endif
    }

#elif defined (CC_MT5882)
    switch(ePort)
    {
        default:
        case HDMI_SWITCH_1:
            u4TmpAnalogRegBase = 0x0;
            break;

        case HDMI_SWITCH_2:
            u4TmpAnalogRegBase = 0x100;
            break;

        case HDMI_SWITCH_3:
            u4TmpAnalogRegBase = 0x200;
            break;
    }

//#else
//#error "No define for Rx analog base address"
#endif
    return (u4TmpAnalogRegBase);
}

static void vSetHdmiEqTracking(E_HDMI_SWITCH_NUM ePort, UINT8 u1Val)
{
    _arHdmiRx[ePort]._bEqTracking = u1Val;
    LOG(6,"!!vSetHdmiEqTracking %d\n",u1Val);
}

static UINT8 u1IsHdmiEqTracking(E_HDMI_SWITCH_NUM ePort)
{
    return (_arHdmiRx[ePort]._bEqTracking);
}

/**
 * Return the average of (VID_XPCNT*100).
 * NOTE: CAN NOT CALL wHDMIXPCCNT FREQUENTLY
 */

#define DATA_CNT_MAX 2
static UINT32 wHDMIXPCCNT(void)
{
    UINT32 datacnt[DATA_CNT_MAX];
    UINT32 idx, i, tmp;
#if Protect_XCLKInPCLk
    UINT32 j, k, tmp_clk[5], tmp_clk_buffer;
#endif

    for(i = 0 ; i < DATA_CNT_MAX ; i++)
    {
        datacnt[i] = 0;
    }

    idx = 0;

    // TODO
    for(i = 0; i < 10; i++)
    {
#if Protect_XCLKInPCLk

        for(j = 0; j < XCLKInPCLkCNT; j++)
        {
            tmp_clk[j] = ((((UINT32)u1IO32Read1B(VID_CRC_OUT_2 + u4ActiveHdmiRegBase) & 0x7) << 8) | (UINT32)u1IO32Read1B(VID_CRC_OUT_3 + u4ActiveHdmiRegBase)) + 1;
            vUtDelay2us(1);
        }

        for(k = XCLKInPCLkCNT; k > 0; k--)
        {
            for(j = 0; j < k - 1; j++)
            {
                if(tmp_clk[j] > tmp_clk[j+1])
                {
                    tmp_clk_buffer = tmp_clk[j];
                    tmp_clk[j] = tmp_clk[j+1];
                    tmp_clk[j+1] = tmp_clk_buffer;
                }
            }
        }

        tmp = tmp_clk[XCLKInPCLkCNTMid];
#else
        tmp = (((u1IO32Read1B(VID_CRC_OUT_2 + u4ActiveHdmiRegBase) & 0x7) << 8) | u1IO32Read1B(VID_CRC_OUT_3 + u4ActiveHdmiRegBase)) + 1;
#endif

        if(datacnt[0] == tmp)
        {
            continue;
        }

        if(datacnt[1] == tmp)
        {
            continue;
        }

        if(idx < DATA_CNT_MAX)
        {
            datacnt[idx] = tmp;
        }

        idx++;

        if(idx == 2)
        {
            break;
        }
    }

    if(idx == 2)
    {
        return (datacnt[0] + datacnt[1])*(100 / 2);
    }
    else
    {
        return ((datacnt[0])*(100));
    }
}

static UINT32 dwHDMIPixelFreq(void)
{
    UINT32 div;

    div = wHDMIXPCCNT();

    return ((div > 0) ? (u4XtalfreqCnt / div) : 1);
}

UINT8 bHDMIAUDIOSampleRateCal(void)
{
    UINT32 wCTS_HW, wN_HW;
    UINT32 wAudSampleRate;
    UINT8 btmp;
    static UINT8 Ori_audio_FS;
    btmp = 20;
    wCTS_HW = (IO32ReadFldAlign(CTS_HVAL + u4ActiveHdmiRegBase, CTS_VAL_HW19_16) << 16) | (IO32ReadFldAlign(CTS_HVAL + u4ActiveHdmiRegBase, CTS_VAL_HW15_8) << 8) | IO32ReadFldAlign(CTS_HVAL + u4ActiveHdmiRegBase, CTS_VAL_HW7_0);
    wN_HW = (IO32ReadFldAlign(N_HVAL + u4ActiveHdmiRegBase, N_VAL_HW) << 16) | (IO32ReadFldAlign(N_SVAL + u4ActiveHdmiRegBase, N_VAL_HW15_8) << 8) | IO32ReadFldAlign(N_SVAL + u4ActiveHdmiRegBase, N_VAL_HW7_0);
    //wAudSampleRate= (dwHDMIPixelFreq()*10/128) *(wN_HW/wCTS_HW);
    wAudSampleRate = (((dwHDMIPixelFreq() * 1000) / wCTS_HW) * ((wN_HW * 100) / 128)) / 10000;
    //LOG(6, " HW CTS =%d  and N =%d , AudSample rate =%d\n", wCTS_HW,wN_HW,wAudSampleRate);
    LOG(8, " HW CTS =%d  and N =%d\n", wCTS_HW, wN_HW);

    if(bHDMIDeepColorStatus() == 0x1)
    {
        wAudSampleRate = (wAudSampleRate * 10) / 8;
    }
    else if(bHDMIDeepColorStatus() == 0x2)
    {
        wAudSampleRate = (wAudSampleRate * 12) / 8;
    }
    else if(bHDMIDeepColorStatus() == 0x3)
    {
        wAudSampleRate = (wAudSampleRate * 16) / 8;
    }

    LOG(8, "AudSample rate =%d\n", wAudSampleRate);

    if(wAudSampleRate > (320 - btmp) && wAudSampleRate < (320 + btmp))
    {
        if(Ori_audio_FS != AUD_FS_32K)
        {
            Ori_audio_FS = AUD_FS_32K;
        }

        return AUD_FS_32K;
    }
    else if(wAudSampleRate > (441 - btmp) && wAudSampleRate < (441 + btmp))
    {
        if(Ori_audio_FS != AUD_FS_44K)
        {
            Ori_audio_FS = AUD_FS_44K;
        }

        return AUD_FS_44K;
    }
    else if(wAudSampleRate > (480 - btmp) && wAudSampleRate < (480 + btmp))
    {
        if(Ori_audio_FS != AUD_FS_48K)
        {
            Ori_audio_FS = AUD_FS_48K;
        }

        return AUD_FS_48K;
    }
    else if(wAudSampleRate > (880 - btmp) && wAudSampleRate < (880 + btmp))
    {
        if(Ori_audio_FS != AUD_FS_88K)
        {
            Ori_audio_FS = AUD_FS_88K;
        }

        return AUD_FS_88K;
    }
    else if(wAudSampleRate > (960 - btmp) && wAudSampleRate < (960 + btmp))
    {
        if(Ori_audio_FS != AUD_FS_96K)
        {
            Ori_audio_FS = AUD_FS_96K;
        }

        return AUD_FS_96K;
    }
    else if(wAudSampleRate > (1760 - btmp) && wAudSampleRate < (1760 + btmp))
    {
        if(Ori_audio_FS != AUD_FS_176K)
        {
            Ori_audio_FS = AUD_FS_176K;
        }

        return AUD_FS_176K;
    }
    else if(wAudSampleRate > (1920 - btmp) && wAudSampleRate < (1920 + btmp))
    {
        if(Ori_audio_FS != AUD_FS_192K)
        {
            Ori_audio_FS = AUD_FS_192K;
        }

        return AUD_FS_192K;
    }
    else
    {
        return AUD_FS_UNKNOWN;
    }
}
/**
 * @brief 	HDMI Audio Config (audio initial)
 * @param	None
 * @retval	None
 */
PRIVATE void vHDMIAudConfig(void)
{
    if(eCurrHdmiPort != eActiveHdmiPort)
    {
        return;
    }

    // Load audio configuration
    if(!AUD_GetAinCfg(AUD_STREAM_FROM_HDMI, &_rAudCfg))
    {
        // If audio input not initialed, use the following setting
        _rAudCfg.eStrSrc =      AUD_STREAM_FROM_HDMI;
        _rAudCfg.eFormat =      FORMAT_RJ;
        _rAudCfg.eBits =        DAC_24_BIT;
        _rAudCfg.eCycle =       LRCK_CYC_32;
        _rAudCfg.eSampleFreq =  MCLK_256FS;
        _rAudCfg.fgDataInvert = FALSE;
        _rAudCfg.fgLRInvert =   FALSE;
    }

    // Setup audio format
    if(_rAudCfg.fgLRInvert)
    {
        vIO32Write4BMsk(I2S_CTRL + u4ActiveHdmiRegBase, Fld2Msk32(WS), Fld2Msk32(WS));// AIN_LR_NonINV
    }
    else
    {
        vIO32Write4BMsk(I2S_CTRL + u4ActiveHdmiRegBase, 0, Fld2Msk32(WS));	// AIN_LR_INV
    }

    if(_rAudCfg.eFormat == FORMAT_RJ)
    {
        vIO32Write4BMsk(I2S_CTRL + u4ActiveHdmiRegBase, Fld2Msk32(JUSTIFY) | Fld2Msk32(HDMI_1ST_BIT), Fld2Msk32(JUSTIFY) | Fld2Msk32(HDMI_1ST_BIT));// Right-Justified
    }
    else if(_rAudCfg.eFormat == FORMAT_LJ)
    {
        vIO32Write4BMsk(I2S_CTRL + u4ActiveHdmiRegBase, Fld2Msk32(HDMI_1ST_BIT), Fld2Msk32(HDMI_1ST_BIT));// Left-Justified
    }
    else if(_rAudCfg.eFormat == FORMAT_I2S)
    {
        vIO32Write4BMsk(I2S_CTRL + u4ActiveHdmiRegBase, 0, Fld2Msk32(JUSTIFY) | Fld2Msk32(HDMI_1ST_BIT));// I2S
    }

    if(_rAudCfg.eCycle == LRCK_CYC_16)
    {
        vIO32Write4BMsk(I2S_CTRL + u4ActiveHdmiRegBase, Fld2Msk32(SIZE), Fld2Msk32(SIZE));//LR_Cycles 16
    }
    else if(_rAudCfg.eCycle == LRCK_CYC_32)
    {
        vIO32Write4BMsk(I2S_CTRL + u4ActiveHdmiRegBase, 0, Fld2Msk32(SIZE));//LR_Cycles 32
    }

    // Sample edge is falling
    vIO32Write4BMsk(I2S_CTRL + u4ActiveHdmiRegBase, Fld2Msk32(CLK_EDGE), Fld2Msk32(CLK_EDGE));

    // Setup sampling frequency MCLK
    if(_rAudCfg.eSampleFreq == MCLK_128FS)
    {
        vIO32WriteFldAlign(ACR_CTRL1 + u4ActiveHdmiRegBase, 0x0, FS_VAL_SW);
    }
    else if(_rAudCfg.eSampleFreq == MCLK_256FS)
    {
        vIO32WriteFldAlign(ACR_CTRL1 + u4ActiveHdmiRegBase, 0x1, FS_VAL_SW);
    }
    else if(_rAudCfg.eSampleFreq == MCLK_384FS)
    {
        vIO32WriteFldAlign(ACR_CTRL1 + u4ActiveHdmiRegBase, 0x2, FS_VAL_SW);
    }
    else if(_rAudCfg.eSampleFreq == MCLK_512FS)
    {
        vIO32WriteFldAlign(ACR_CTRL1 + u4ActiveHdmiRegBase, 0x3, FS_VAL_SW);
    }

    // Enable CLK
    vIO32Write4BMsk(AUDRX_CTRL + u4ActiveHdmiRegBase, Fld2Msk32(I2S_MODE), Fld2Msk32(I2S_MODE));
    vIO32Write4BMsk(I2S_CTRL + u4ActiveHdmiRegBase, Fld2Msk32(MCLK_EN) | Fld2Msk32(SD0_EN) | Fld2Msk32(SD1_EN) | Fld2Msk32(SD2_EN), Fld2Msk32(MCLK_EN) | Fld2Msk32(SD0_EN) | Fld2Msk32(SD1_EN) | Fld2Msk32(SD2_EN));
    vIO32Write4BMsk(I2S_CTRL + u4ActiveHdmiRegBase, 0, Fld2Msk32(PCM));
#ifdef HDMI_AUD_FMT_CHG_PROTECT
    // Enable auto audio configuration
    // => Auto mute as one of the following condition raised
    //    a. Audio FIFO Underrun INTR4[0]
    //    b. Audio FIFO Overrun INTR4[1]
    //    c. CTS Reused INTR4[2]
    //    d. Fs Changed INTR5[0]
    //
    //    Clear condtion by the following operation
    //    -> vIO32Write4BMsk(INTR_MASK1, Fld2Msk32(INTR5_AUDIO_MUTE), Fld2Msk32(INTR5_AUDIO_MUTE));
    // Setup auto mute condition a+b+d
    vIO32Write4BMsk(AEC_CTRL + u4ActiveHdmiRegBase, (0x16 << 24), Fld2Msk32(EXP_EN_15_8));
#if !HDMI_Audio_NewFlow
    // Enable auto audio configuration
    vIO32Write4BMsk(AEC_CTRL + u4ActiveHdmiRegBase, Fld2Msk32(AAC_EN), Fld2Msk32(AAC_EN));
#endif
    // Enable AAC to control SD0~3
    vIO32Write4BMsk(AEC_CTRL + u4ActiveHdmiRegBase, Fld2Msk32(AAC_OUT_OFF_EN), Fld2Msk32(AAC_OUT_OFF_EN));
#endif
    // FIXME
    vHDMIAudioOutOn();
}

void vHDMIAudChannelStatus(BOOL *pfgCopyright, UINT8 *pu1CategoryCode,
                           UINT8 *p1WordLength, BOOL *pfgIsPcm)
{
    if(pfgCopyright)
    {
        // 0: copyright is asserted, 1: No copyright is asserted
        *pfgCopyright = !((BOOL)(IO32ReadFldAlign(AUDRX_CTRL + u4ActiveHdmiRegBase, AUD_CH_STAT1_2)));
    }

    if(pfgIsPcm)
    {
        // 0: PCM sample, 1: other purpose
        *pfgIsPcm = !((BOOL)(IO32ReadFldAlign(AUDRX_CTRL + u4ActiveHdmiRegBase, AUD_CH_STAT1_1)));
    }

    if(pu1CategoryCode)
    {
        *pu1CategoryCode = IO32ReadFldAlign(AUDRX_CTRL + u4ActiveHdmiRegBase, AUD_CH_STAT2);
    }

    if(p1WordLength)
    {
        UINT8 u1Length;
        UINT8 u1WordLength = 24;
        u1Length = IO32ReadFldAlign(CHST1 + u4ActiveHdmiRegBase, AUD_LENGTH);

        if(IO32ReadFldAlign(CHST1 + u4ActiveHdmiRegBase, AUD_LENGTH_MAX))
        {
            switch(u1Length)
            {
                case 1:
                    u1WordLength = 16;
                    break;

                case 2:
                    u1WordLength = 18;
                    break;

                case 4:
                    u1WordLength = 19;
                    break;

                case 5:
                    u1WordLength = 20;
                    break;

                case 6:
                    u1WordLength = 17;
                    break;

                default:
                    u1WordLength = 24;
            }
        }
        else
        {
            switch(u1Length)
            {
                case 1:
                    u1WordLength = 20;
                    break;

                case 2:
                    u1WordLength = 22;
                    break;

                case 4:
                    u1WordLength = 23;
                    break;

                case 5:
                    u1WordLength = 24;
                    break;

                case 6:
                    u1WordLength = 21;
                    break;

                default:
                    u1WordLength = 24;
            }
        }

        *p1WordLength = u1WordLength;
    }
}

#if !HDMI_Audio_NewFlow
UINT8 bHDMIAudStatus(void)
{
    UINT8 u1UnstableLvl = HDMI_AUD_OK;

    // Note that the checking should based on the the level from low to high

    if(IO32ReadFldAlign(INTR_STATE0 + u4ActiveHdmiRegBase, INTR1_PLL_UNLOCKED))
    {
        vIO32Write4B(INTR_STATE0 + u4ActiveHdmiRegBase, Fld2Msk32(INTR1_PLL_UNLOCKED));
        LOG(8, "Audio PLL unlocked\n");
        //u1UnstableLvl = HDMI_AUD_UNSTABLE_LVL_1;
        u1UnstableLvl = HDMI_AUD_OK;
    }

    if(IO32ReadFldAlign(INTR_STATE1 + u4ActiveHdmiRegBase, INTR4_CTS_REUSED_ERR))
    {
        vIO32Write4B(INTR_STATE1 + u4ActiveHdmiRegBase, Fld2Msk32(INTR4_CTS_REUSED_ERR));
        //u1UnstableLvl = HDMI_AUD_UNSTABLE_LVL_1;
        u1UnstableLvl = HDMI_AUD_OK;
        LOG(8, "CTS Reused\n");
    }

    if(IO32ReadFldAlign(INTR_STATE0 + u4ActiveHdmiRegBase, INTR1_FIFO_ERR))
    {
        vIO32Write4B(INTR_STATE0 + u4ActiveHdmiRegBase, Fld2Msk32(INTR1_FIFO_ERR));
        LOG(8, "Audio FIFO error\n");
        u1UnstableLvl = HDMI_AUD_OK;
    }

    if(_bDviModeChged)
    {
        u1UnstableLvl = HDMI_AUD_UNSTABLE_LVL_1;
        LOG(8, "Video not stable.. mute audio..\n");
    }

    if(IO32ReadFldAlign(INTR_STATE1 + u4ActiveHdmiRegBase, INTR5_AUDIO_MUTE))
    {
        vIO32Write4B(INTR_STATE1 + u4ActiveHdmiRegBase, Fld2Msk32(INTR5_AUDIO_MUTE));
        u1UnstableLvl = HDMI_AUD_UNSTABLE_LVL_1;
        LOG(8, "Audio Auto Configure muted ...\n");
    }

    if(IO32ReadFldAlign(INTR_STATE1 + u4ActiveHdmiRegBase, INTR4_UNDERRUN))
    {
        vIO32Write4B(INTR_STATE1 + u4ActiveHdmiRegBase, Fld2Msk32(INTR4_UNDERRUN));
        u1UnstableLvl = HDMI_AUD_UNSTABLE_LVL_2;
        LOG(8, "FIFO Under-Run\n");
    }

    if(IO32ReadFldAlign(INTR_STATE1 + u4ActiveHdmiRegBase, INTR4_OVERRUN))
    {
        vIO32Write4B(INTR_STATE1 + u4ActiveHdmiRegBase, Fld2Msk32(INTR4_OVERRUN));
        u1UnstableLvl = HDMI_AUD_UNSTABLE_LVL_2;
        LOG(8, "FIFO Overrun\n");
    }

    if(IO32ReadFldAlign(INTR_STATE1 + u4ActiveHdmiRegBase, INTR5_AUD_SAMPLE_F))
    {
        vIO32Write4B(INTR_STATE1 + u4ActiveHdmiRegBase, Fld2Msk32(INTR5_AUD_SAMPLE_F));
        u1UnstableLvl = HDMI_AUD_UNSTABLE_LVL_2;
        LOG(8, "Audio Fs Sample Rate Change\n");
        vHDMIHandleAudFmtChange();
    }

    return u1UnstableLvl;
}
#else
UINT8 bHDMIAudStatus(void)
{
    return _bHDMIAudFIFOflag;
}
#endif
/**
 * @brief 	fgHDMIAudFifoFault
 * @param	None
 * @retval	None
 */
PRIVATE BOOL fgHDMIAudFifoFault(void)
{
    return (IO32ReadFldAlign(INTR_STATE1 + +u4ActiveHdmiRegBase, INTR4_UNDERRUN) | IO32ReadFldAlign(INTR_STATE1 + u4ActiveHdmiRegBase, INTR4_OVERRUN)
            | IO32ReadFldAlign(INTR_STATE1 + u4ActiveHdmiRegBase, INTR5_AUD_SAMPLE_F)) ? TRUE : FALSE;
}

PRIVATE void vHDMIMuteAudio(void)
{
#ifdef CC_SUPPORT_HDMI_FBOOTING

    if(!fgIsExtSrcHDMI())
    {
        return;
    }

#endif

    if(eCurrHdmiPort != eActiveHdmiPort)
    {
        return;
    }

    if(!IS_AUD_MUTE())
    {
        vIO32Write4BMsk(CHST1 + u4ActiveHdmiRegBase, Fld2Msk32(CH0_MUTE), Fld2Msk32(CH0_MUTE));
        _fgAudMute = TRUE;
#ifdef SUPPORT_HDMI_AUDIO
        //AUD_HdmiModeMute(AUD_DEC_MAIN, TRUE);
        AUD_HdmiDrvModeMute(AUD_DEC_MAIN, TRUE);
        LOG(9, " [AUD_HDMI] AUDIO HDMI MUTE AUDIO\n");
#endif
    }
}

PRIVATE void vHDMIUnMuteAudio(void)
{
#ifdef CC_SUPPORT_HDMI_FBOOTING

    if(!fgIsExtSrcHDMI())
    {
        return;
    }

#endif

    if(IS_AUD_MUTE())
    {
        vIO32Write4BMsk(CHST1 + u4ActiveHdmiRegBase, 0, Fld2Msk32(CH0_MUTE));
        _fgAudMute = FALSE;
#ifdef SUPPORT_HDMI_AUDIO
        //AUD_HdmiModeMute(AUD_DEC_MAIN, FALSE);
        AUD_HdmiDrvModeMute(AUD_DEC_MAIN, FALSE);
        LOG(9, " [AUD_HDMI] AUDIO HDMI UNMUTE AUDIO\n");
#endif
    }
}

/**
 * @brief 	vHDMIHandleAudFifoFault
 * @param	None
 * @retval	None
 */
PRIVATE void vHDMIHandleAudFifoFault(void)
{
    UINT32 HW_CTS_Value;
    UINT8 u1Fs;
    HW_CTS_Value = (UINT32)IO32ReadFldAlign(CTS_HVAL + u4ActiveHdmiRegBase, CTS_VAL_HW7_0) | ((UINT32)IO32ReadFldAlign(CTS_HVAL + u4ActiveHdmiRegBase, CTS_VAL_HW15_8) << 8) | ((UINT32)IO32ReadFldAlign(CTS_HVAL + u4ActiveHdmiRegBase, CTS_VAL_HW19_16) << 16);
#if !HDMI_Audio_NewFlow

    // Audio be muted ..
    if(u4IO32Read4B(AUDP_STAT + u4ActiveHdmiRegBase) & MT53XX_AUD_MUTE)
    {
        vIO32Write4B(AUDP_STAT + u4ActiveHdmiRegBase, (u4IO32Read4B(AUDP_STAT + u4ActiveHdmiRegBase) & (~MT53XX_AUD_MUTE)));
        LOG(8,  "##################### (Audio be muted) #####################\n");
    }

    if(fgHDMIAudFifoFault())
    {
        LOG(11, "Audio FIFO: %x\n", IO32ReadFldAlign(INTR_STATE1 + u4ActiveHdmiRegBase, INTR_STATE1_0));//LOG(11, "Audio FIFO: %x\n", IO32ReadFldAlign(INTR_MASK0,INTR_MASK0_0));
        LOG(11, "[AUD_HDMI] Reset audio fifo\n");
        // HDMI audio fifo reset
        vIO32Write4B(SRST + u4ActiveHdmiRegBase, u4IO32Read4B(SRST + u4ActiveHdmiRegBase) | MT53XX_FIFO_RESET);
        vIO32Write4B(SRST + u4ActiveHdmiRegBase, u4IO32Read4B(SRST + u4ActiveHdmiRegBase) & ~MT53XX_FIFO_RESET);
        // FOR san diego setop box channel change
        //	    vIO32WriteFldAlign(SRST, 1, AAC_RST);
        //   vUtDelay1ms(1);
        //   vIO32WriteFldAlign(SRST, 0, AAC_RST);
        //
        vUtDelay2us(500);
        // Clear interrupt
        vIO32Write4B(INTR_STATE1 + u4ActiveHdmiRegBase, Fld2Msk32(INTR4_OVERRUN) | Fld2Msk32(INTR4_UNDERRUN) | Fld2Msk32(INTR5_AUDIO_MUTE));
        //vIO32Write4BMsk(INTR_MASK0, Fld2Msk32(INTR4_OVERRUN), Fld2Msk32(INTR4_OVERRUN));
    }

#endif

    if(_fgVideoOn)
    {
        u1Fs = bHDMIAUDIOSampleRateCal();

        if(u1Fs == AUD_FS_UNKNOWN)
        {
            vHDMIMuteAudio();
        }
        else
        {
            if((IO32ReadFldAlign(SRST + u4ActiveHdmiRegBase, DEEP_STA) == 0x0) && (650000 < HW_CTS_Value || 81000 == HW_CTS_Value))
            {
                vHDMIMuteAudio();
            }
            else
            {
                vHDMIUnMuteAudio();
            }
        }
    }
}

UINT8 bHDMIGetAudSampleFreq(void)
{
    return IO32ReadFldAlign(LK_THRS_SVAL + u4ActiveHdmiRegBase, RHDMI_AUD_SAMPLE_F);//read sample rate
}

void vHDMIHandleAudFmtChange(void)
{
    UINT8 u1Fs;
    // Audio sampling rate change
    u1Fs = bHDMIAUDIOSampleRateCal();

    if(u1Fs == AUD_FS_32K)
    {
        AUD_AoutDacFs(AUD_DEC_MAIN, FS_32K); // FIXME ! Support dual decoder later
        vHDMIAudConfig();
        LOG(6, "HDMI audio sampling rate change to 32K\n");
    }
    else if(u1Fs == AUD_FS_44K)
    {
        AUD_AoutDacFs(AUD_DEC_MAIN, FS_44K); // FIXME ! Support dual decoder later
        vHDMIAudConfig();
        LOG(6, "HDMI audio sampling rate change to 44K\n");
    }
    else if(u1Fs == AUD_FS_48K)
    {
        AUD_AoutDacFs(AUD_DEC_MAIN, FS_48K); // FIXME ! Support dual decoder later
        vHDMIAudConfig();
        LOG(6, "HDMI audio sampling rate change to 48K\n");
    }
    else if(u1Fs == AUD_FS_88K)
    {
        AUD_AoutDacFs(AUD_DEC_MAIN, FS_88K); // FIXME ! Support dual decoder later
        vHDMIAudConfig();
        LOG(6, "HDMI audio sampling rate change to 88K\n");
    }
    else if(u1Fs == AUD_FS_96K)
    {
        AUD_AoutDacFs(AUD_DEC_MAIN, FS_96K); // FIXME ! Support dual decoder later
        vHDMIAudConfig();
        LOG(6, "HDMI audio sampling rate change to 96K\n");
    }
    else if(u1Fs == AUD_FS_176K)
    {
        AUD_AoutDacFs(AUD_DEC_MAIN, FS_176K); // FIXME ! Support dual decoder later
        vHDMIAudConfig();
        LOG(6, "HDMI audio sampling rate change to 176K\n");
    }
    else if(u1Fs == AUD_FS_192K)
    {
        AUD_AoutDacFs(AUD_DEC_MAIN, FS_192K); // FIXME ! Support dual decoder later
        vHDMIAudConfig();
        LOG(6, "HDMI audio sampling rate change to 192K\n");
    }
    else
    {
        // Default set to 48kHz
        AUD_AoutDacFs(AUD_DEC_MAIN, FS_48K); // FIXME ! Support dual decoder later
        vHDMIAudConfig();
        LOG(6, "HDMI audio sampling rate change to 48K\n");
    }
}



/*
UINT8 vHDMIAinGetMode(void)
{
    UINT8 u1Mode;
    u1Mode = IO32ReadFldAlign(AUDRX_CTRL, AUD_CH_STAT1_1);//read mode

    return (u1Mode);
}
*/
void vHDMISpdifOutEnable(BOOL fgEnable)
{
    if(fgEnable == (BOOL)TRUE)
    {
        vIO32Write4BMsk(AUDRX_CTRL + u4ActiveHdmiRegBase, Fld2Msk32(HW_MUTE_EN), Fld2Msk32(HW_MUTE_EN));
        LOG(3, "HDMI SPDIF output enable\n");
    }
    else
    {
        vIO32Write4BMsk(AUDRX_CTRL + u4ActiveHdmiRegBase, 0, Fld2Msk32(HW_MUTE_EN));
        LOG(3, "HDMI SPDIF output disable\n");
    }
}

/**
 * @brief 	read HDMI audio type
 * @param	None
 * @retval	0=PCM ,1 =none PCM
 */
UINT8 bHDMI_IsPCM(void)
{
    return IO32ReadFldAlign(AUDRX_CTRL + u4ActiveHdmiRegBase, AUD_CH_STAT1_1);
}

/**
 * @brief 	read HDMI audio length
 * @param	None
 * @retval
 */
UINT8 bHDMIGetAudioBitWidth(void)
{
    UINT8 length;
    length = IO32ReadFldAlign(CHST1 + u4ActiveHdmiRegBase, AUD_LENGTH);

    if(IO32ReadFldAlign(CHST1 + u4ActiveHdmiRegBase, AUD_LENGTH_MAX))
    {
        switch(length)
        {
            case 0x1:
                return HDMI_AUD_Length_20bits;

            case 0x6:
                return HDMI_AUD_Length_21bits;

            case 0x2:
                return HDMI_AUD_Length_22bits;

            case 0x4:
                return HDMI_AUD_Length_23bits;

            case 0x5:
                return HDMI_AUD_Length_24bits;

            default:
                return HDMI_AUD_Length_Unknown;
        }
    }
    else
    {
        switch(length)
        {
            case 0x1:
                return HDMI_AUD_Length_16bits;

            case 0x6:
                return HDMI_AUD_Length_17bits;

            case 0x2:
                return HDMI_AUD_Length_18bits;

            case 0x4:
                return HDMI_AUD_Length_19bits;

            case 0x5:
                return HDMI_AUD_Length_20bits;

            default:
                return HDMI_AUD_Length_Unknown;
        }
    }
}

//#define AUDIO_TEST

UINT8 u1ACR_DPLL_LOCK = 0;
UINT8 u1AudFifoFault  = 0;
UINT8 u1AudPacket  = 0;

#define AUDPACKET_INVALID 0
#define AUDPACKET_1 1
#define AUDPACKET_MAX 200
#define AUDPACKET_GOOD 201

#if HDMI_Audio_NewFlow
void vHDMIAudErrorHandler(void)
{
    // start HDMI audio after system ready(audio engine ready).
    if (!fgIsExtSrcHDMI())
    {
        return;
    }

    if(eCurrHdmiPort != eActiveHdmiPort)
    {
        return;
    }

    if (_fgHdmiAudInit == FALSE)
    {
        _fgHdmiAudInit = TRUE;
        vHDMIAudConfig();
     }

    if((_bHdmiMode == 0) || (_arHdmiRx[eActiveHdmiPort]._bHDMIState != HDMI_STATE_AUTH) || _bDviModeChged)
    {
        // mute audio
        vIO32WriteFldAlign(AUDP_STAT + u4ActiveHdmiRegBase, 1, AUDIO_MUTE);
        vIO32WriteFldAlign(AEC_CTRL + u4ActiveHdmiRegBase, 0, AAC_EN);
        vIO32Write4B(INTR_STATE1 + u4ActiveHdmiRegBase, Fld2Msk32(INTR5_AUDIO_MUTE));
        // back to integer mode

		#if defined(CC_MT5398) || defined(CC_MT5880) || defined(CC_MT5399) || defined(CC_MT5882)
        	vIO32WriteFldAlign(REG_PLL_GROUP_CFG3, 0, FLD_RG_HADDS2_DDSEN);
		#else
			vIO32WriteFldAlign(REG_PLL_GROUP_CFG5, 0, FLD_RG_HADDS2_DDSEN);
		#endif

        _bHDMIAudFIFOflag = 1;
        u1AudFifoFault = 0;
        u1AudPacket = AUDPACKET_INVALID;
    }
    else
    {
        AUD_DEC_STREAM_FROM_T eAudStreamFrom;
#ifdef CC_MT5882		
	#ifdef CC_AUD_SUPPORT_DUAL_DSP 
		AUD_GetStreamFrom(AUD_DSP0, AUD_DEC_MAIN, &eAudStreamFrom);
	#else        
		AUD_GetStreamFrom(AUD_DEC_MAIN, &eAudStreamFrom);
	#endif
#else
		AUD_GetStreamFrom(AUD_DEC_MAIN, &eAudStreamFrom);
#endif

		if(RegReadFldAlign(APLL1 + u4ActiveHdmiRegBase, SDINOVR) == 1)
		{
			vRegWriteFldAlign(APLL1 + u4ActiveHdmiRegBase, 0, SDINOVR);
		}

        if(fgHDMIAudFifoFault() || (eCurrHdmiPort != eActivePort))
        {
            eActivePort = eCurrHdmiPort;
            u1AudFifoFault ++;
            // back to integer mode

			#if defined(CC_MT5398) || defined(CC_MT5880) || defined(CC_MT5399)  || defined(CC_MT5882)
				vIO32WriteFldAlign(REG_PLL_GROUP_CFG3, 0, FLD_RG_HADDS2_DDSEN);
			#else
				vIO32WriteFldAlign(REG_PLL_GROUP_CFG5, 0, FLD_RG_HADDS2_DDSEN);
			#endif

            _bHDMIAudFIFOflag = 1;
            //##2 check for audio packet
            if(IO32ReadFldAlign(INTR_STATE0 + u4ActiveHdmiRegBase, INTR2_NEW_AUD_PKT))
            {
                vIO32Write4B(INTR_STATE0 + u4ActiveHdmiRegBase, Fld2Msk32(INTR2_NEW_AUD_PKT));

                if(u1AudPacket == AUDPACKET_INVALID || u1AudPacket == AUDPACKET_GOOD)
                {
                    u1AudPacket = AUDPACKET_1;
                }
                else if(u1AudPacket < AUDPACKET_MAX)
                {
                    u1AudPacket++;
                }

                if(u1AudPacket > AUDPACKET_1 && IO32ReadFldAlign(APLL1 + u4ActiveHdmiRegBase, ACR_DPLL_LOCK))
                {
                    u1ACR_DPLL_LOCK++;
                }
                else
                {
#ifdef AUDIO_TEST

                    if(u1ACR_DPLL_LOCK)
                    {
                        Printf("+++++ u1ACR_DPLL_LOCK unlock %d\n", u1ACR_DPLL_LOCK);
                    }

#endif
                    u1ACR_DPLL_LOCK = 0;
                }

                //            vUtDelay2us(500);

                //##3 check for DDS LOCK
                if(u1ACR_DPLL_LOCK > 1)    // no clock: 243MHz, normal 270 / 293 MHz.
                {
                    //  vIO32Write4B(INTR_STATE0 + u4ActiveHdmiRegBase, Fld2Msk32(INTR2_NEW_AUD_PKT));
                    //##4 APLL2 RST
#ifdef AUDIO_TEST
                    //Printf(" >>>> Apll2 Reset 1ms...\n");
#endif

#ifndef __HDMI_SLT__
                    //fix misc apll2 stress fail              
                    bHDMIAPPL2Reset();
#endif
                    //##5.1 AAC off
                    vIO32WriteFldAlign(AEC_CTRL + u4ActiveHdmiRegBase, 0, AAC_EN);
                    //vIO32WriteFldAlign(SRST, 1, ACR_RST);
                    //vIO32WriteFldAlign(SRST, 0, ACR_RST);
                    //##5.2 audio fifo RST
                    vIO32WriteFldAlign(SRST + u4ActiveHdmiRegBase, 1, FIFO_RST);
                    vUtDelay2us(10);
                    vIO32WriteFldAlign(SRST + u4ActiveHdmiRegBase, 0, FIFO_RST);
                    vUtDelay2us(500);

                    if(IO32ReadFldAlign(INTR_STATE1 + u4ActiveHdmiRegBase, INTR5_AUD_SAMPLE_F))
                    {
                        vHDMIHandleAudFmtChange();
#ifdef AUDIO_TEST
                        Printf("!!!!!INTR5_AUD_SAMPLE_F\n");
#endif
                    }
					x_thread_delay(1);
					
                    // Clear interrupt
                    vIO32Write4B(INTR_STATE1 + u4ActiveHdmiRegBase, Fld2Msk32(INTR4_OVERRUN) | Fld2Msk32(INTR4_UNDERRUN) | Fld2Msk32(INTR5_AUD_SAMPLE_F));
                    vIO32Write4B(INTR_STATE1 + u4ActiveHdmiRegBase, Fld2Msk32(INTR5_AUDIO_MUTE));
#ifdef AUDIO_TEST

                    vUtDelay2us(100);

                    if(fgHDMIAudFifoFault())
                    {
                        if(IO32ReadFldAlign(INTR_STATE1 + u4ActiveHdmiRegBase, INTR4_UNDERRUN))
                        {
#ifdef AUDIO_TEST
                            Printf("!!!!!INTR4_UNDERRUN\n");
#endif
                        }

                        if(IO32ReadFldAlign(INTR_STATE1 + u4ActiveHdmiRegBase, INTR4_OVERRUN))
                        {
#ifdef AUDIO_TEST
                            Printf("!!!!!INTR4_OVERRUN\n");
#endif
                        }

                        if(IO32ReadFldAlign(INTR_STATE1 + u4ActiveHdmiRegBase, INTR5_AUD_SAMPLE_F))
                        {
#ifdef AUDIO_TEST
                            Printf("!!!!!INTR5_AUD_SAMPLE_F\n");
#endif
                        }
                    }

#endif
#ifdef AUDIO_TEST
                    //  Printf("+++++ INTR2_NEW_AUD_PKT %d\n",u1INTR2_NEW_AUD_PKT);
                    Printf("+++++ bHDMIAPPL2Reset %d\n",u1AudFifoFault);
                    Printf("+++++ AUDPACKET %d\n",u1AudPacket);
                    Printf("+++++ ACR_DPLL_LOCK %d\n",u1ACR_DPLL_LOCK);
#endif
                }
            }
            else
            {
                u1AudPacket = AUDPACKET_INVALID;
            }
        }
        else
        {
            // normal case
            _bHDMIAudFIFOflag = 0;
            u1AudFifoFault = 0;

            if(u1AudPacket != AUDPACKET_GOOD)
            {
#ifdef AUDIO_TEST
                Printf("+++++ AUDPACKET_GOOD\n");
#endif
                u1AudPacket = AUDPACKET_GOOD;
            }

            vIO32WriteFldAlign(AEC_CTRL + u4ActiveHdmiRegBase, 1, AAC_EN);
            vIO32WriteFldAlign(AUDP_STAT + u4ActiveHdmiRegBase, 0, AUDIO_MUTE);

            if((u1IO32Read1B(I2S_CTRL_3 + u4ActiveHdmiRegBase) & 0xf8) == 0x0)
            {
                vIO32WriteFldAlign(AEC_CTRL + u4ActiveHdmiRegBase, 0, AAC_EN);
                vIO32WriteFldAlign(AEC_CTRL + u4ActiveHdmiRegBase, 1, AAC_EN);
            }
        }
    }

    if(_bHDMIAudFIFOflag == 1)
    {
        _bHdmiAudFreq = AUD_FS_48K;   // default: 48k
    }
    else
    {
        _bHdmiAudFreq = bHDMIAUDIOSampleRateCal();
    }
}
#endif
/*****************************************************************************
 * HDMI State Machine Declaration
*****************************************************************************/


static CHAR *_aszHdmiState[] =
{
    "HDMI_STATE_NOTREADY",
    "HDMI_STATE_INIT",
    "HDMI_STATE_PWOFF",
    "HDMI_STATE_PWON",
    "HDMI_STATE_PWON2",
    "HDMI_STATE_SCDT",
    "HDMI_STATE_AUTH"
};

/*****************************************************************************
 *If it want to use MT8202 internal audio clk 27mhz,please turn on this function.
*****************************************************************************/
#define INTER_AUDCLK 1


/*****************************************************************************
 * Variable Declaration
*****************************************************************************/

// JOSH UINT8   _bHDMIState;
UINT8   _bHdmiFlag;
UINT8   _bHdmiCnt;
UINT16  _bCKDTcnt;
UINT16  _bResDigPhy;


// HDMI Status
UINT8   _bHdmiAudFs;
UINT8   _bHdmiMode; // 0 - DVI, 1 - HDMI
UINT8   _bHdmiModeActive; // 0 - DVI, 1 - HDMI
UINT8   _bHdmiMD;
UINT8   _bHdmiVsdbActive;

UINT8  _bIsMhlDevice = 0;//0:HDMI   1:MHL
UINT8  _bIsMhlPPMode = 0;//0:24bit mode   1:packet pixel mode
UINT8  _bIsMhlDeviceold = 0;//0:HDMI   1:MHL
UINT8  _bIsMhlPPModeold = 0;//0:24bit mode   1:packet pixel mode

#if MUTE_TEMP
// PRIVATE UINT8 bHDMIdelayTDMSReset;
//#define HDMI_OFFON_MUTE 60 // off to on
#define HDMI_HPD_ONDELAY 3000
#define HDMI_MUTE_COUNT 20 // mode change
#endif
#define HDMI_XPC_STABLE_CNT 30 // unit, per Vsync, xclk in pclk stable count

#define SAVE_WEAK_IC 0
#if SAVE_WEAK_IC
UINT8 bHDMIBadSync;
#endif

UINT8 _bAVIInfo_tmp;
UINT8   _bHDMIScanInfo;
UINT8 _bHDMIAspectRatio;
UINT8 _bHDMIAFD;
UINT8 _bHDMIHDCPStatus;
UINT8 _bHDMI422Input;
UINT8 _bHDMIITCFlag;
UINT8 _bHDMIITCContent;
UINT8 _bIntr_CK_CHG;
UINT8 _bHDMIColorFormat;

UINT8 _bNEW_AVI_Info;
UINT8 _bACPCount;
UINT8 _bHPD_Indep_Ctrl;	//  1 is 5v detect , 0 is CKDT detect
UINT32 _wHDMI_OFFON_MUTE_COUNT = 0;
UINT32 _wDVI_WAIT_STABLE_COUNT;
UINT32 _wHDMIBypassFlag;
UINT32 _wDVI_WAIT_NOSIGNAL_COUNT;
UINT32 _wHDMI_WAIT_SCDT_STABLE_COUNT;
#if (TMDS_EQ_CUSTOM == 1)
UINT32 _wHDMI_EQ_ZERO_VALUE;
UINT32 _wHDMI_EQ_BOOST_VALUE;
UINT32 _wHDMI_EQ_SEL_VALUE;
UINT32 _wHDMI_EQ_GAIN_VALUE;
UINT32 _wHDMI_LBW_VALUE;
#endif
UINT32 _wHDMI_HDCP_MASk1;
UINT32 _wHDMI_HDCP_MASk2;
UINT32 _bHDP_Value;
UINT8 _bNodifyConnect;
UINT8 _bUnplugCount;
UINT8 _bForceHPDLow;
UINT8 _bHDMIColorSpace;
#define ATC_720p_fix 1
UINT8 _bFixedEQReset;
#ifdef CC_BYPASS_AVI_INFOFRAME
UINT8 _bDVIAVIinfoBypass;
#endif
#ifdef CC_Support_Sil9285
UINT8 _bHDMI9287switchstopflag;
#endif

static UINT32 u4PreXpcCnt;
static UINT32 u4CurXpcCnt;
static UINT8 _bXpcStableCnt;


void vHdmiMute(E_HDMI_SWITCH_NUM ePort)
{
    UINT32 u4TmpHdmiRegBase = 0;
    u4TmpHdmiRegBase = u4GetHdmiRegBase(ePort);
    vIO32WriteFldAlign(AUDP_STAT + u4TmpHdmiRegBase, 1, AUDP_VIDEO_MUTE);
}

void vHdmiUnMute(E_HDMI_SWITCH_NUM ePort)
{
    UINT32 u4TmpHdmiRegBase = 0;
    u4TmpHdmiRegBase = u4GetHdmiRegBase(ePort);
    vIO32WriteFldAlign(AUDP_STAT + u4TmpHdmiRegBase, 0, AUDP_VIDEO_MUTE);
}

UINT8 u1IsHdmiMute(E_HDMI_SWITCH_NUM ePort)
{
    UINT32 u4TmpHdmiRegBase = 0;
    u4TmpHdmiRegBase = u4GetHdmiRegBase(ePort);
    return (IO32ReadFldAlign(AUDP_STAT + u4TmpHdmiRegBase, AUDP_VIDEO_MUTE));
}

/**
 * @brief Enable HDMI Video Output
 * @param none
 * @retval none
 */
void vHDMIVideoOutOn(void)
{
    //_fgVideoOn = TRUE;
    //return;
    // UINT8 bI2CWriteData;
    if(_arHdmiRx[eActiveHdmiPort]._bHDMIState == HDMI_STATE_NOTREADY)
    {
        return;
    }

    _fgVideoOn = TRUE;
    vHdmiUnMute(eActiveHdmiPort);
}

/**
 * @brief	Disable HDMI Video Output
 * @param	None
 * @retval	None
 */
void vHDMIVideoOutOff(void)
{
    //_fgVideoOn = TRUE;
    //return;

    // UINT8 bI2CWriteData;
    if(_arHdmiRx[eActiveHdmiPort]._bHDMIState == HDMI_STATE_NOTREADY)
    {
        return;
    }

    LOG(8, "Block Video\n");
    _fgVideoOn = FALSE;
}

/**
 * @brief		Enable HDMI Video Output
 * @param	None
 * @retval	None
 */
void vHDMIAudioOutOn(void)
{
    //UINT8 bI2CWriteData;
    if(_arHdmiRx[eActiveHdmiPort]._bHDMIState == HDMI_STATE_NOTREADY)
    {
        return;
    }

    // Open APLL
    vIO32Write4BMsk(PD_SYS + u4ActiveHdmiRegBase, Fld2Msk32(PD_APLL) | Fld2Msk32(PD_AO), Fld2Msk32(PD_APLL) | Fld2Msk32(PD_AO));
    vSetHdmiFlg(HDMI_AUDIO_ON);
}

/**
 * @brief
 * @param	None
 * @retval	None
 */
void vHDMIAudioOutOff(void)
{
    //UINT8 bI2CWriteData;
    if(_arHdmiRx[eActiveHdmiPort]._bHDMIState == HDMI_STATE_NOTREADY)
    {
        return;
    }

    //LOG(9, "HDMI AudioOut Off\n");
    vClrHdmiFlg(HDMI_AUDIO_ON);
    vHDMIMuteAudio();
}

UINT8 bHDMIHDCPStatusGet(void)
{
    if(IO32ReadFldAlign(HDCP_STAT + u4ActiveHdmiRegBase, HDCP_AUTH) && IO32ReadFldAlign(HDCP_STAT + u4ActiveHdmiRegBase, HDCP_DECRYPT))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

UINT8 bHDMIHDCPKeyCheck(void)
{
    //    UINT32 u4Idx;
    UINT32 u4Data[4];
    INT32 fgSet;
    fgSet = EEPHDCP_Init();

    if(fgSet)
    {
        LOG(2, "EEPHDCP_Init fail\n");
        return 0;
    }

    // write to hdcp sram pointer.
    IO_WRITE32(BIM_BASE, REG_RW_HDCP_ADDR, 0);
    u4Data[0] = IO_READ32(BIM_BASE, REG_RW_HDCP_DATA);
    u4Data[1] = IO_READ32(BIM_BASE, REG_RW_HDCP_DATA);
    u4Data[2] = IO_READ32(BIM_BASE, REG_RW_HDCP_DATA);
    u4Data[3] = IO_READ32(BIM_BASE, REG_RW_HDCP_DATA);

    if((u4Data[0] == 0xffffffff) && (u4Data[1] == 0xffffffff))
    {
        LOG(2, "NO HDCP key in EEPROM\n");
        return 0;
    }

    return 1;
}
void  vHDMISetEQRsel(UINT8 u4Value)
{
}

UINT32  vHDMIGetEQSetting(void)
{
    //printf("EQ value=0x%x",IO32ReadFldAlign(TMDS_CTRL1,EQSEL));
#ifdef JOSH
#if !Support_HW_Adaptive_EQ  // fix EQ

    if((u1IO32Read1B(ANA_INTF_1_1) & 0x06) == 0x06)
    {
        printf("Fix EQ!\n");
        printf("HDMI_EQ_FILT: 0x%x\n", IO32ReadFldAlign(ANA_INTF_1, RG_HDMI_EQ_FILT));
        printf("Max->Min: 0x7, 0x6, 0x5, 0x4, 0x3, 0x2, 0x1, 0x0, 0xf, 0xe, 0xd, 0xc, 0xb, 0xa, 0x9, 0x8\n");
        printf("HDMI_EQBIAS(default=0x4): 0x%x\n", IO32ReadFldAlign(ANA_INTF_1,RG_HDMI_EQBIAS));
        printf("HDMI_PLLBIAS(default=0x2): 0x%x\n", IO32ReadFldAlign(ANA_INTF_2,RG_HDMI_PLLBIAS));
        printf("HDMI_EQ_GAIN(default=0x0): 0x%x\n", IO32ReadFldAlign(ANA_INTF_2,RG_HDMI_EQ_GAIN));
    }

#else

    if((u1IO32Read1B(ANA_INTF_1_1) & 0x06) == 0x0)
    {
        printf("Adaptive EQ!\n");
        printf("0x22E60[24]: 0x%x\n", ((IO32ReadFldAlign(TMDS_CTRL_STATUS1, HDMI_CH0_STATUS)&0x100)>>8));
        printf("R channel EQ: 0x%x\n", IO32ReadFldAlign(TMDS_CTRL_STATUS1, HDMI_CH0_EQERR));
        printf("G channel EQ: 0x%x\n", IO32ReadFldAlign(TMDS_CTRL_STATUS2, HDMI_CH1_EQERR));
        printf("B channel EQ: 0x%x\n", IO32ReadFldAlign(TMDS_CTRL_STATUS3, HDMI_CH2_EQERR));
        printf("Max->Min: 0x7, 0x6, 0x5, 0x4, 0x3, 0x2, 0x1, 0x0, 0xf, 0xe, 0xd, 0xc, 0xb, 0xa, 0x9, 0x8\n");
    }

#endif
#endif
    return 0;
}
#ifdef CC_BYPASS_AVI_INFOFRAME
/**
 * @brief 	Bypass AVI infoframe check when DVI decode search timing
 * @param	None
 * @retval	1=bypass ,0 =org
 */
void vHDMIDVIBypassAVIinfo(UINT8 value)
{
    //UNUSED(fgApiEepromWriteByte(EEP_HDMI_AVIINFO_BYPASS,value));
    _bDVIAVIinfoBypass = value;
}
UINT8 bHDMIGetDVIBypassAVIinfo(void)
{
    return _bDVIAVIinfoBypass;
}
#endif

UINT32 bHDMIGetHPDAdjust(void)
{
    return _bHDP_Value;
}
void vHDMIHPDAdjust(UINT32 value)
{
    _wHDMI_OFFON_MUTE_COUNT = value;
    _bHDP_Value = value ;
}

UINT8 bHDMIColorFormat(UINT8 method)
{
	UINT8 u1EcValue =0; 
    switch(method)
    {
        default:
        case 0:	//auto detect
            if(_bHdmiModeActive)
            {
                if(u1IO32Read1B(AVIRX0_0 + u4ActiveHdmiRegBase) != 0x82)
                {
                    return SV_COLOR_HDMI_RGB;
                }
            }

            if((u1IO32Read1B(AVIRX1_0 + u4ActiveHdmiRegBase)& 0x60) == 0x00)
            {
                return SV_COLOR_HDMI_RGB;
            }
            else if((u1IO32Read1B(AVIRX1_0 + u4ActiveHdmiRegBase) & 0x60) == 0x20)
            {
                if((u1IO32Read1B(AVIRX1_0 + u4ActiveHdmiRegBase)  & 0x7) == 0x1)
                {
                	if(bHDMIClocrimetry() == 1)
	                {
	                    //601
	                    return  SV_COLOR_HDMI_422_601;
	                }
	                else if(bHDMIClocrimetry() == 0)
	                {
	                    //709
	                    return  SV_COLOR_HDMI_422_709;
	                }
					else if(bHDMIClocrimetry() == 2)
					{
						u1EcValue = u1IO32Read1B(AVIRX1_2 + u4ActiveHdmiRegBase) >> 4;
						switch(u1EcValue &0x7)
						{
							case 0x00:
								return SV_COLOR_HDMI_XVYCC_601;
							case 0x01:
								return SV_COLOR_HDMI_XVYCC_709;
							case 0x02:
								return SV_COLOR_HDMI_sYCC_601;
							case 0x3:
								return SV_COLOR_HDMI_AdobeYCC_601;
							case 0x4:
								return SV_COLOR_HDMI_AdobeRGB;
							case 0x5:
								return SV_COLOR_HDMI_BT2020_YCC;
							case 0x6:
								return SV_COLOR_HDMI_BT2020_RGB;
							default:
								return SV_COLOR_YCBCR;
						}
					
					}
                }
				

            }
            else if((u1IO32Read1B(AVIRX1_0 + u4ActiveHdmiRegBase)  & 0x60) == 0x40)
            {
            	if(bHDMIClocrimetry() == 1)
                {
                    //601
                    return  SV_COLOR_HDMI_444_601;
                }
                else if(bHDMIClocrimetry() == 0)
                {
                    //709
                    return  SV_COLOR_HDMI_444_709;
                }
                else if(bHDMIClocrimetry() == 2)
				{
					u1EcValue = u1IO32Read1B(AVIRX1_2 + u4ActiveHdmiRegBase) >> 4;
					switch(u1EcValue &0x7)
					{
						case 0x00:
							return SV_COLOR_HDMI_XVYCC_601;
						case 0x01:
							return SV_COLOR_HDMI_XVYCC_709;
						case 0x02:
							return SV_COLOR_HDMI_sYCC_601;
						case 0x3:
							return SV_COLOR_HDMI_AdobeYCC_601;
						case 0x4:
							return SV_COLOR_HDMI_AdobeRGB;
						case 0x5:
							return SV_COLOR_HDMI_BT2020_YCC;
						case 0x6:
							return SV_COLOR_HDMI_BT2020_RGB;
						default:
							return SV_COLOR_YCBCR;
                    }
                }
            }

            break;

        case 1:	//force detect by AVI infoframe
            if((u1IO32Read1B(AVIRX1_0 + u4ActiveHdmiRegBase)& 0x60) == 0x00)
            {
                return SV_COLOR_HDMI_RGB;
            }
            else if((u1IO32Read1B(AVIRX1_0 + u4ActiveHdmiRegBase) & 0x60) == 0x20)
            {
                if(bHDMIClocrimetry())
                {
                    //601
                    return  SV_COLOR_HDMI_422_601;
                }
                else
                {
                    //709
                    return  SV_COLOR_HDMI_422_709;
                }
            }
            else if((u1IO32Read1B(AVIRX1_0 + u4ActiveHdmiRegBase)  & 0x60) == 0x40)
            {
                if(bHDMIClocrimetry())
                {
                    //601
                    return  SV_COLOR_HDMI_444_601;
                }
                else
                {
                    //709
                    return  SV_COLOR_HDMI_444_709;
                }
            }

            break;

        case 2:	//force detect by GAMUT infoframe
            if((u1IO32Read1B(GAMUTRX0_3 + u4ActiveHdmiRegBase)& 0x7) == 0x00)
            {
                return SV_COLOR_HDMI_RGB;
            }
            else if((u1IO32Read1B(AVIRX1_0 + u4ActiveHdmiRegBase) & 0x7) == 0x1)
            {
                return SV_COLOR_HDMI_XVYCC_601;
            }
            else if((u1IO32Read1B(AVIRX1_0 + u4ActiveHdmiRegBase)  & 0x7) == 0x2)
            {
                return SV_COLOR_HDMI_XVYCC_709;
            }

            break;
    }

    /* not auto detect & invalid info-frame color-space */
    return SV_COLOR_HDMI_RGB;
}

/**
 * @brief 	HDMI HPD control bypass
 * @param	None
 * @retval	1=bypass ,0 =org
 */
void vHDMIHPDIndepCtrl(UINT8 cmd)
{
    _bHPD_Indep_Ctrl = cmd;
}

/**
 * @brief 	HDMI input is RGB /Ycbcr
 * @param	None
 * @retval	1=RGB ,0 =YCBCR
 */
UINT8 bHDMIInputType(void)
{
    UINT8 bReadData;

    if(_bHdmiModeActive)
    {
        if(u1IO32Read1B(AVIRX0_0 + u4ActiveHdmiRegBase) != 0x82)
        {
            return 1;
        }

        if(IO32ReadFldAlign(INTR_STATE0 + u4ActiveHdmiRegBase, INTR3_NEW_AVI))
        {
            _bNEW_AVI_Info = 1;
            bReadData = u1IO32Read1B(AVIRX1_0 + u4ActiveHdmiRegBase);
            vIO32Write4B(INTR_STATE0 + u4ActiveHdmiRegBase, 0x1 << 24);
            _bAVIInfo_tmp = bReadData;
            LOG(6, "It has Received New AVI \n");
        }
        else
        {
            bReadData = _bAVIInfo_tmp;
        }

        if((bReadData & 0x60) == 0x00)  // RGB
        {
            return 1;
        }
        else //Ycbcr
        {
            return 0;
        }
    }
    else
    {
        return 1;
    }
}
/**
 * @brief 	HDMI Clocrimetry
 * @param	None
 * @retval	1=ITU601 ,0 =ITU709
 */
UINT8 bHDMIClocrimetry(void)
{
    UINT8 bReadData;
    bReadData = u1IO32Read1B(AVIRX1_1 + u4ActiveHdmiRegBase);

    if((bReadData&0xc0) == 0x40)
    {
        return 1;
    }
    else if((bReadData&0xc0) == 0xC0)
    {
        return 2;
    }
    else
    {
        return 0;
    }
}

/**
 * @brief 	vHDMIApllClkReset
 * @param	None
 * @retval	None
 */
void vHDMIApllClkReset(void)
{
    vIO32Write4BMsk(TMDS_CTRL0 + u4ActiveHdmiRegBase, 0, Fld2Msk32(PCLK_INV));
    vIO32Write4BMsk(TMDS_CTRL0 + u4ActiveHdmiRegBase, Fld2Msk32(PCLK_INV), Fld2Msk32(PCLK_INV));
    vIO32Write4BMsk(TMDS_CTRL0 + u4ActiveHdmiRegBase, 0, Fld2Msk32(PCLK_INV));
    vIO32Write4BMsk(TMDS_CTRL0 + u4ActiveHdmiRegBase, Fld2Msk32(PCLK_INV), Fld2Msk32(PCLK_INV));
    vIO32Write4BMsk(TMDS_CTRL0 + u4ActiveHdmiRegBase, 0, Fld2Msk32(PCLK_INV));	//add by ciwu
    //vIO32Write4BMsk(TMDS_CTRL0 + u4ActiveHdmiRegBase, Fld2Msk32(PCLK_INV),Fld2Msk32(PCLK_INV));
}

/**
 * @brief 	HDMI Clock Detect
 * @param	None
 * @retval	None
 */
void bHDMIClockDetect(void)
{
    UINT8 bReadData;
    bReadData = u1IO32Read1B(SRST_2 + u4ActiveHdmiRegBase);

    if((bReadData&0x02) != 0x02)
    {
        vHDMIApllClkReset();
    }
}

void vHDMIDDCBusCTRL(UINT8 bOnOff)
{
    if(bOnOff)
    {
        vIO32Write1B(SYS_CTRL_1 + u4HdmiRegBase, 0x95);
    }
    else
    {
        vIO32Write1B(SYS_CTRL_1 + u4HdmiRegBase, 0x85);
    }

#ifdef CC_support_i2c_switch
    vHDMISwitchDDCBusCtrl(eCurrHdmiPort,bOnOff);
#endif
}

static void vHDMIPhyTermCtrl(E_HDMI_SWITCH_NUM eSrc, UINT8 bOnOff)
{
    UINT32 u4RegOffset = 0x0;
    u4RegOffset = u4AnalogRegBase;
    vIO32WriteFldAlign(REG_HDMI_RX_Normal_0_CFG2 + u4RegOffset,bOnOff,  RG_HDMI_0_TERM_EN);
    _arHdmiRx[eSrc]._u1RxTermOn = bOnOff;
}

void vHDMIEQCIRCUITCTRL(E_HDMI_SWITCH_NUM ePort, UINT8 bOnOff)
{
    UINT32 u4RegOffset = 0;
    UNUSED(u4RegOffset);
    u4RegOffset = u4GetAnalogRegBase(ePort);

	if(bOnOff)
	{
		//turn on EQ circuit
		vIO32WriteFldAlign(REG_HDMI_RX_Normal_0_CFG1 + u4RegOffset, 1, RG_HDMI_0_FULL_EN);
		vIO32WriteFldAlign(REG_HDMI_RX_Normal_0_CFG2 + u4RegOffset, 1, RG_HDMI_0_PI_EN);
		vIO32WriteFldAlign(REG_HDMI_RX_Normal_0_CFG4 + u4RegOffset, 0, RG_HDMI_0_PLL_PWD);
		vIO32WriteFldAlign(REG_HDMI_RX_Normal_0_CFG1 + u4RegOffset, 1, RG_HDMI_0_CLKBUF_EN);
		vIO32WriteFldAlign(REG_HDMI_RX_Normal_0_CFG1 + u4RegOffset, 0, RG_HDMI_0_EQ_PD);
	}
}
void vHDMITMDSIndepCTRL(E_HDMI_SWITCH_NUM ePort, UINT8 bOnOff)
{
    UINT32 u4RegOffset = 0;
    UNUSED(u4RegOffset);
    u4RegOffset = u4GetAnalogRegBase(ePort);

    // PD_TERM: Enable TMDS-PHY termination 50-ohm resistance
    if(bOnOff)
    {
        /*turn on TMDS*/
        vIO32Write1B(PD_SYS_2 + u4HdmiRegBase, 0xff);
        vHDMIPhyTermCtrl(ePort, 1);
        //vIO32Write1B(SYS_CTRL_1, 0x95);
        vHDMIDDCBusCTRL(TRUE);
    }

//#ifndef __HDMI_SLT__
    else
    {
        /* turn off TMDS*/
        vIO32Write1B(PD_SYS_2 + u4HdmiRegBase, 0xcf);
    	//turn off EQ circuit
		vIO32WriteFldAlign(REG_HDMI_RX_Normal_0_CFG4 + u4RegOffset, 1, RG_HDMI_0_PLL_PWD);
		vIO32WriteFldAlign(REG_HDMI_RX_Normal_0_CFG2 + u4RegOffset, 0, RG_HDMI_0_PI_EN);
		vIO32WriteFldAlign(REG_HDMI_RX_Normal_0_CFG1 + u4RegOffset, 1, RG_HDMI_0_FULL_EN);
		
		vIO32WriteFldAlign(REG_HDMI_RX_Normal_0_CFG1 + u4RegOffset, 0, RG_HDMI_0_CLKBUF_EN);
		vIO32WriteFldAlign(REG_HDMI_RX_Normal_0_CFG1 + u4RegOffset, 1, RG_HDMI_0_EQ_PD);
        vHDMIPhyTermCtrl(ePort, 0);
        //vIO32Write1B(SYS_CTRL_1, 0x85);
        vHDMIDDCBusCTRL(FALSE);
    }

//#endif
}

void vHDMITMDSCTRL(E_HDMI_SWITCH_NUM ePort, UINT8 bOnOff)
{
    UINT32 u4RegOffset = 0;
    UNUSED(u4RegOffset);
    u4RegOffset = u4GetAnalogRegBase(ePort);

    if(!_bHPD_Indep_Ctrl)
    {
        // PD_TERM: Enable TMDS-PHY termination 50-ohm resistance
        if(bOnOff)
        {
            /*turn on TMDS*/
            vIO32Write1B(PD_SYS_2 + u4HdmiRegBase, 0xff);
            vHDMIPhyTermCtrl(ePort, 1);
        }

//#ifndef __HDMI_SLT__
        else
        {
            /* turn off TMDS*/
            vIO32Write1B(PD_SYS_2 + u4HdmiRegBase, 0xcf);
			//turn off EQ circuit
			vIO32WriteFldAlign(REG_HDMI_RX_Normal_0_CFG4 + u4RegOffset, 1, RG_HDMI_0_PLL_PWD);
			vIO32WriteFldAlign(REG_HDMI_RX_Normal_0_CFG2+u4RegOffset, 0, RG_HDMI_0_PI_EN);
			vIO32WriteFldAlign(REG_HDMI_RX_Normal_0_CFG1+u4RegOffset, 1, RG_HDMI_0_FULL_EN);
			vIO32WriteFldAlign(REG_HDMI_RX_Normal_0_CFG1 + u4RegOffset, 0, RG_HDMI_0_CLKBUF_EN);
			vIO32WriteFldAlign(REG_HDMI_RX_Normal_0_CFG1 + u4RegOffset, 1, RG_HDMI_0_EQ_PD);
			
            vHDMIPhyTermCtrl(ePort, 0);
        }

//#endif
#ifdef CC_support_i2c_switch
        vHDMISwitchCLKTerm(eCurrHdmiPort,bOnOff);
#endif
    }
}

void bHDMIAPPL2Reset(void)
{
    // CRIT_STATE_T rCritState;
    if(!fgIsExtSrcHDMI())
    {
        return;
    }

    //rCritState = x_crit_start();   // ??
#if defined(CC_MT5398) || defined(CC_MT5880) || defined(CC_MT5399)  || defined(CC_MT5882)
		//Reset Again
		//RG_APLL_DDS_RSTB (0=>1)
		vIO32WriteFldAlign(REG_PLL_GROUP_CFG3, 0, FLD_RG_HADDS2_DDS_RSTB);
		//RG_APLL_PCW_NCPO_CHG
		//RG_APLL_FIFO_START_MAN (0=>1)
		vIO32WriteFldAlign(REG_PLL_GROUP_CFG3, 0, FLD_RG_HADDS2_FIFO_START_MAN);
		//RG_APLL_NCPO_EN (0=>1)
		vIO32WriteFldAlign(REG_PLL_GROUP_CFG3, 0, FLD_RG_HADDS2_NCPO_EN);
		//RG_APLL_DDSEN (0=>1)
		vIO32WriteFldAlign(REG_PLL_GROUP_CFG3, 0, FLD_RG_HADDS2_DDSEN);

		vUtDelay2us(10);

		//Reset End

		//delay 100us
		//REG(RG_HADDS2_DDS_PWDB) = 1b
		vIO32WriteFldAlign(REG_PLL_GROUP_CFG4, 1, FLD_RG_HADDS2_DDS_PWDB);
		//REG(RG_HADDS2_DDS_RSTB) = 1b
		vIO32WriteFldAlign(REG_PLL_GROUP_CFG3, 1, FLD_RG_HADDS2_DDS_RSTB);

		//HADDS2 setting (digital)
		bHDMIAudioSigmaDeltaSetting(0, 0, 0);
		vUtDelay2us(500);  // dealy 1 ms

		//REG(RG_HADDS2_FIFO_START_MAN) = 1b
		vIO32WriteFldAlign(REG_PLL_GROUP_CFG3, 1, FLD_RG_HADDS2_FIFO_START_MAN);
		//REG(RG_HADDS2_DDS_DMY[4]) = 1b
		vIO32WriteFldAlign(REG_PLL_GROUP_CFG3, 1, FLD_RG_HADDS2_NCPO_EN);
		//REG(RG_HADDS2_DDSEN) = 1b
		vIO32WriteFldAlign(REG_PLL_GROUP_CFG3, 1, FLD_RG_HADDS2_DDSEN);

#elif defined(CC_MT5389) || defined(CC_MT5368) || defined(CC_MT5396)

    //Reset Again
    //RG_APLL_DDS_RSTB (0=>1)
    vIO32WriteFldAlign(REG_PLL_GROUP_CFG4, 0, FLD_RG_HADDS2_DDS_RSTB);
    //RG_APLL_PCW_NCPO_CHG
    //RG_APLL_FIFO_START_MAN (0=>1)
    vIO32WriteFldAlign(REG_PLL_GROUP_CFG4, 0, FLD_RG_HADDS2_FIFO_START_MAN);
    //RG_APLL_NCPO_EN (0=>1)
    vIO32WriteFldAlign(REG_PLL_GROUP_CFG4, 0, FLD_RG_HADDS2_RG_NCPO_EN);
    //RG_APLL_DDSEN (0=>1)
    vIO32WriteFldAlign(REG_PLL_GROUP_CFG5, 0, FLD_RG_HADDS2_DDSEN);

    vUtDelay2us(10);

    //Reset End

    //delay 100us
    //REG(RG_HADDS2_DDS_PWDB) = 1b
    vIO32WriteFldAlign(REG_PLL_GROUP_CFG4, 1, FLD_RG_HADDS2_DDS_PWDB);
    //REG(RG_HADDS2_DDS_RSTB) = 1b
    vIO32WriteFldAlign(REG_PLL_GROUP_CFG4, 1, FLD_RG_HADDS2_DDS_RSTB);

    //HADDS2 setting (digital)
    bHDMIAudioSigmaDeltaSetting(0, 0, 0);
    vUtDelay2us(500);  // dealy 1 ms

    //REG(RG_HADDS2_FIFO_START_MAN) = 1b
    vIO32WriteFldAlign(REG_PLL_GROUP_CFG4, 1, FLD_RG_HADDS2_FIFO_START_MAN);
    //REG(RG_HADDS2_DDS_DMY[4]) = 1b
    vIO32WriteFldAlign(REG_PLL_GROUP_CFG4, 1, FLD_RG_HADDS2_RG_NCPO_EN);
    //REG(RG_HADDS2_DDSEN) = 1b
    vIO32WriteFldAlign(REG_PLL_GROUP_CFG5, 1, FLD_RG_HADDS2_DDSEN);

#elif defined(CC_MT5365) || defined(CC_MT5395)
#if IC_5365_VERIFY
    vIO32WriteFldAlign(0xf000d134, 0x0, Fld(1, 30, AC_MSKB3));
    vIO32WriteFldAlign(0xf000d134, 0x1, Fld(1, 30, AC_MSKB3));
#endif
#endif
    // x_crit_end(rCritState); // ??
}

void bHDMIAudioSigmaDeltaSetting(UINT32 value1, UINT8 value2, UINT8 bSDINOVR)
{
    UINT32 u4HADDS2RegBase = 0xc00;

    vIO32WriteFldAlign(APLL0 + u4ActiveHdmiRegBase, 0x3, SDREF);
    vIO32WriteFldAlign(APLL1 + u4ActiveHdmiRegBase, bSDINOVR, SDINOVR);
    vIO32WriteFldAlign(CTS_HW_ADP3 + u4ActiveHdmiRegBase, value1, SDINOVRVAL_31_0);
    vIO32WriteFldAlign(CTS_HW_ADP4 + u4ActiveHdmiRegBase, value2, SDINOVRVAL_39_32);

    //The following is for software mode DDS Setting,
    if(value1 == 0x205bc01a)
    {
        //44.1KHz
        //for DDS mode HDMI setting
        vIO32WriteFldAlign(DDS + u4HADDS2RegBase, 0x131205bc, REG_HADDS2_PCW_SW); //fix 27MHz reference clock
#if 0
        if(IS_XTAL_27MHZ())
        {
            vIO32WriteFldAlign(DDS + u4HADDS2RegBase, 0x131205bc, REG_HADDS2_PCW_SW);
        }
	    else if(IS_XTAL_24MHZ())
	    {
	        vIO32WriteFldAlign(DDS + u4HADDS2RegBase, 0x159353f7, REG_HADDS2_PCW_SW);
	    }
#endif
    }
    else
    {
        //48KHz
        //for DDS mode HDMI setting
        vIO32WriteFldAlign(DDS + u4HADDS2RegBase, 0x14d867c3, REG_HADDS2_PCW_SW); //fix 27MHz reference clock
#if 0
        if(IS_XTAL_27MHZ())
        {
            vIO32WriteFldAlign(DDS + u4HADDS2RegBase, 0x14d867c3, REG_HADDS2_PCW_SW);
        }
	    else if(IS_XTAL_24MHZ())
	    {
            vIO32WriteFldAlign(DDS + u4HADDS2RegBase, 0x179374bc, REG_HADDS2_PCW_SW);
        }
#endif
    }

    //PCW_NCPO_CHG toggle
    vIO32WriteFldAlign(ACR_CTRL3 + u4HADDS2RegBase, 0, FBDIV);   //fbdiv multiply 2

    if((value1==0)&&(value2==0)&&(bSDINOVR==0))
    {
        vIO32WriteFldAlign(ACR_CTRL3 + u4HADDS2RegBase, 0, REG_HADDS2_PCW_SEL_SW);   // 0: HW mode PCW
    }
    else
    {
        vIO32WriteFldAlign(ACR_CTRL3 + u4HADDS2RegBase, 1, REG_HADDS2_PCW_SEL_SW);	// 1: SWmode PCW
    }

    vIO32WriteFldAlign(ACR_CTRL3 + u4HADDS2RegBase, 1, REG_HADDS2_PCW_CHG_SW);
    vIO32WriteFldAlign(ACR_CTRL3 + u4HADDS2RegBase, 0, REG_HADDS2_PCW_CHG_SW);
}

void bHDMIAudioSigmaDeltaFineTuneSetting(UINT32 value1, UINT8 value2, UINT32 target, UINT8 bSDINOVR)
{
    UINT32 u4HADDS2RegBase = 0xc00;

    vRegWriteFldAlign(APLL0 + u4ActiveHdmiRegBase, 0x3, SDREF);
    vRegWriteFldAlign(APLL1 + u4ActiveHdmiRegBase, bSDINOVR, SDINOVR);
    vRegWriteFldAlign(CTS_HW_ADP3 + u4ActiveHdmiRegBase, value1, SDINOVRVAL_31_0);
    vRegWriteFldAlign(CTS_HW_ADP4 + u4ActiveHdmiRegBase, value2, SDINOVRVAL_39_32);

    //The following is for software mode DDS Setting,
    if (value1 == 0x205bc01a)
    {
        //44.1KHz
        //for DDS mode HDMI setting
        vRegWriteFldAlign(DDS + u4HADDS2RegBase, target, REG_HADDS2_PCW_SW);
    }
    else
    {
        //48KHz
        //for DDS mode HDMI setting
        vRegWriteFldAlign(DDS + u4HADDS2RegBase, target, REG_HADDS2_PCW_SW);
    }

    //PCW_NCPO_CHG toggle

    vRegWriteFldAlign(ACR_CTRL3 + u4HADDS2RegBase, 0, FBDIV);   //fbdiv multiply 2

	vRegWriteFldAlign(ACR_CTRL3 + u4HADDS2RegBase, 1, REG_HADDS2_PCW_SEL_SW);	// 1: SWmode PCW

    vRegWriteFldAlign(ACR_CTRL3 + u4HADDS2RegBase, 1, REG_HADDS2_PCW_CHG_SW);
    vRegWriteFldAlign(ACR_CTRL3 + u4HADDS2RegBase, 0, REG_HADDS2_PCW_CHG_SW);


}


#define TMDS_CLOCK_UNKNOW 0
#define TMDS_CLOCK_LOW1 1
#define TMDS_CLOCK_LOW2 2
#define TMDS_CLOCK_MIDDLE 3
#define TMDS_CLOCK_HIGH 4

static void vHDMIPhyBandCtrl(E_HDMI_SWITCH_NUM ePortl, UINT8 u1Val)
{
    UINT8 bCH0EQ;
    UINT32 u4RegOffset = 0x0;
    UINT32 u4Buf = 0x0;

    u4RegOffset = u4GetAnalogRegBase(ePortl);
    vIO32WriteFldAlign(REG_HDMI_RX_Normal_0_CFG0 + u4RegOffset, 1, RG_HDMI_0_CDR_RST);
    vIO32WriteFldAlign(REG_HDMI_RX_Normal_0_CFG1 + u4RegOffset, 1, RG_HDMI_0_DATA_RST);
    vIO32WriteFldAlign(REG_HDMI_RX_Normal_0_CFG1 + u4RegOffset, 1, RG_HDMI_0_EQ_RST);
    vIO32WriteFldAlign(REG_HDMI_RX_Normal_0_CFG2 + u4RegOffset, 0, RG_HDMI_0_DEEPCLRCLK_RSTN);
    vIO32WriteFldAlign(REG_HDMI_RX_Normal_0_CFG4 + u4RegOffset, 1, RG_HDMI_0_PLL_PWD);
	vIO32WriteFldAlign(REG_HDMI_RX_Normal_0_CFG4 + u4RegOffset, 0, RG_HDMI_0_PLL_DIVCTRL);
#if defined(CC_MT5882)
	if((_bIsMhlDevice == 1) && (ePortl == HDMI_SWITCH_1))
#else
	if((_bIsMhlDevice == 1) && (ePortl == HDMI_SWITCH_1) && (IS_IC_5399_ES2()))
#endif
	{
		vIO32WriteFldAlign(REG_HDMI_RX_Normal_0_CFG1 + u4RegOffset, 1, RG_HDMI_0_HY);
	}
	else
	{
		vIO32WriteFldAlign(REG_HDMI_RX_Normal_0_CFG1 + u4RegOffset, 2, RG_HDMI_0_HY);
	}

	if((_bIsMhlDevice == 1) && (ePortl == HDMI_SWITCH_1))
	{
		vIO32WriteFldAlign(REG_HDMI_RX_Normal_0_CFG2 + u4RegOffset, 1, RG_HDMI_0_MHLEN);
		if(_bIsMhlPPMode == 1)
		{
			vIO32WriteFldAlign(REG_HDMI_RX_Normal_0_CFG2 + u4RegOffset, 0, RG_HDMI_0_MHLMODE);
			vIO32WriteFldAlign(REG_HDMI_RX_Normal_0_CFG4 + u4RegOffset, 2, RG_HDMI_0_PLL_DIVCTRL);
			vIO32WriteFldAlign(MHL_REG, 1, REG_MHL_PACKET_MODE_SEL);
			vMHLPPMForceBypassGB(1);
		}
		else
		{
			vIO32WriteFldAlign(REG_HDMI_RX_Normal_0_CFG2 + u4RegOffset, 1, RG_HDMI_0_MHLMODE);
			vIO32WriteFldAlign(REG_HDMI_RX_Normal_0_CFG4 + u4RegOffset, 3, RG_HDMI_0_PLL_DIVCTRL);
			vIO32WriteFldAlign(MHL_REG, 0, REG_MHL_PACKET_MODE_SEL);
			vMHLPPMForceBypassGB(0);
		}
		vIO32WriteFldAlign(REG_HDMI_RX_Normal_0_CFG2 + u4RegOffset, 0, RG_HDMI_0_FTMODE);
		vIO32WriteFldAlign(REG_HDMI_RX_Normal_0_CFG2 + u4RegOffset, 0, RG_HDMI_0_TERM_MODE);
		vIO32WriteFldAlign(REG_HDMI_RX_NORMAL_0_CFG6 + u4RegOffset, 1, RG_HDMI_0_TERM_ON_SEL);
	}
	else
	{
		vIO32WriteFldAlign(REG_HDMI_RX_Normal_0_CFG2 + u4RegOffset, 0, RG_HDMI_0_MHLEN);
		vIO32WriteFldAlign(REG_HDMI_RX_Normal_0_CFG2 + u4RegOffset, 0, RG_HDMI_0_MHLMODE);
		vIO32WriteFldAlign(REG_HDMI_RX_Normal_0_CFG2 + u4RegOffset, 1, RG_HDMI_0_FTMODE);
		vIO32WriteFldAlign(REG_HDMI_RX_Normal_0_CFG2 + u4RegOffset, 1, RG_HDMI_0_TERM_MODE);
		vIO32WriteFldAlign(REG_HDMI_RX_Normal_0_CFG4 + u4RegOffset, 0, RG_HDMI_0_PLL_DIVCTRL);
		vIO32WriteFldAlign(REG_HDMI_RX_Normal_0_CFG4 + u4RegOffset, 0, RG_HDMI_0_PLL_DIVCTRL);
		vIO32WriteFldAlign(REG_HDMI_RX_NORMAL_0_CFG6 + u4RegOffset, 0, RG_HDMI_0_TERM_ON_SEL);
    	// TODO, set 1 at MHL mode and set 0 at non-MHL mode
    	vIO32WriteFldAlign((MHL_HDCP_DDC + 0xc00), 0x0, RISC_ADDR_PAGE);
		vMHLPPMForceBypassGB(0);
	}
	if(_bIsMhlDevice == 1)
	{
		vIO32WriteFldAlign(CKGEN_REG_MHL_CKCFG, 1, FLD_MHL_DPCLK_SEL);
		vIO32WriteFldAlign(CKGEN_REG_MHL_CKCFG, 1, FLD_MHL_PCLK_SEL);
		vIO32WriteFldAlign(MHL_REG, 1, REG_MHL_MODE_SEL);
	}
	else
	{
		vIO32WriteFldAlign(CKGEN_REG_MHL_CKCFG, 0, FLD_MHL_DPCLK_SEL);
		vIO32WriteFldAlign(CKGEN_REG_MHL_CKCFG, 0, FLD_MHL_PCLK_SEL);
		vIO32WriteFldAlign(MHL_REG, 0, REG_MHL_MODE_SEL);
		//connect 300ohm between DIP and DINN
		vIO32WriteFldAlign(REG_HDMI_RX_NORMAL_0_CFG6, 0x0, RG_HDMI_0_300_OHM_OFF_SEL);
	}

    switch(u1Val)
    {
			case TMDS_CLOCK_LOW1:  // < 30MHz
				vIO32WriteFldAlign(REG_HDMI_RX_Normal_0_CFG1 + u4RegOffset, 0, RG_HDMI_0_BAND_SEL_SW);
				vIO32WriteFldAlign(REG_HDMI_RX_Normal_0_CFG2 + u4RegOffset, 3, RG_HDMI_0_EQBAND);
				vIO32Write4B(REG_HDMI_RX_Normal_0_CFG3 + u4RegOffset, 0x0031EF10);
				vIO32WriteFldAlign(REG_HDMI_RX_Normal_0_CFG4 + u4RegOffset, 0, RG_HDMI_0_PLL_DIVEN);
				vIO32Write1B(REG_HDMI_RX_Normal_0_CFG5_3 + u4RegOffset, 0xFC);
				vIO32WriteFldAlign(REG_HDMI_RX_NORMAL_0_CFG6 + u4RegOffset, 0, RG_HDMI_0_VCOBH);
						 if (_bEQTuning == 0)
						 {
							 if (DRVCUST_OptQuery(eHDMIEQ_LOW1_KPGAIN, &u4Buf) == 0)
							 {
								 _bKPGain = (UINT8) u4Buf;
							 } else {
								 _bKPGain = 3;
							 }
							 if (DRVCUST_OptQuery(eHDMIEQ_LOW1_RXMODE, &u4Buf) == 0)
							 {
								 _bRXMode = (UINT8) u4Buf;
							 } else {
								 _bRXMode = 3;
							 }
							 if (DRVCUST_OptQuery(eHDMIEQ_LOW1_VREF, &u4Buf) == 0)
							 {
								 _bVRef = (UINT8) u4Buf;
							 } else {
								 _bVRef = 2;
							 }
						 }
				break;

			case TMDS_CLOCK_LOW2:  // 30 ~ 40MHz
				vIO32WriteFldAlign(REG_HDMI_RX_Normal_0_CFG1 + u4RegOffset, 0, RG_HDMI_0_BAND_SEL_SW);
				if((_bIsMhlDevice == 1) && (ePortl == HDMI_SWITCH_1))
				{
					if(_bIsMhlPPMode == 1)
					{
						vIO32Write4B(REG_HDMI_RX_Normal_0_CFG3 + u4RegOffset, 0x0031EF10);
					}
					else
					{
						vIO32Write4B(REG_HDMI_RX_Normal_0_CFG3 + u4RegOffset, 0x0031FF30);
					}
					vIO32Write1B(REG_HDMI_RX_Normal_0_CFG5_3 + u4RegOffset, 0xc8);
					vIO32WriteFldAlign(REG_HDMI_RX_Normal_0_CFG2 + u4RegOffset, 2, RG_HDMI_0_EQBAND);
				}
				else
				{
					vIO32Write4B(REG_HDMI_RX_Normal_0_CFG3 + u4RegOffset, 0x0031EF30);
					vIO32Write1B(REG_HDMI_RX_Normal_0_CFG5_3 + u4RegOffset, 0x14);
					vIO32WriteFldAlign(REG_HDMI_RX_Normal_0_CFG2 + u4RegOffset, 3, RG_HDMI_0_EQBAND);					
				}
				vIO32WriteFldAlign(REG_HDMI_RX_Normal_0_CFG4 + u4RegOffset, 0, RG_HDMI_0_PLL_DIVEN);
				vIO32WriteFldAlign(REG_HDMI_RX_NORMAL_0_CFG6 + u4RegOffset, 0, RG_HDMI_0_VCOBH);
						 if (_bEQTuning == 0)
						 {
							 if (DRVCUST_OptQuery(eHDMIEQ_LOW2_KPGAIN, &u4Buf) == 0)
							 {
								 _bKPGain = (UINT8) u4Buf;
							 } else {
								 _bKPGain = 3;
							 }
							 if (DRVCUST_OptQuery(eHDMIEQ_LOW2_RXMODE, &u4Buf) == 0)
							 {
								 _bRXMode = (UINT8) u4Buf;
							 } else {
								 _bRXMode = 3;
							 }
							 if (DRVCUST_OptQuery(eHDMIEQ_LOW2_VREF, &u4Buf) == 0)
							 {
								 _bVRef = (UINT8) u4Buf;
							 } else {
								 _bVRef = 2;
							 }
						 }
				break;

			case TMDS_CLOCK_MIDDLE:  // 40 ~ 160MHz
				if((_bIsMhlDevice == 1) && (ePortl == HDMI_SWITCH_1))
				{
					if(_bIsMhlPPMode == 1)
					{
						vIO32Write4B(REG_HDMI_RX_Normal_0_CFG3 + u4RegOffset, 0x0831EF10);
					}
					else
					{
						vIO32Write4B(REG_HDMI_RX_Normal_0_CFG3 + u4RegOffset, 0x0831FF30);
					}
					vIO32WriteFldAlign(REG_HDMI_RX_Normal_0_CFG1 + u4RegOffset, 1, RG_HDMI_0_BAND_SEL_SW);
					vIO32Write1B(REG_HDMI_RX_Normal_0_CFG5_3 + u4RegOffset, 0xc8);
					vIO32WriteFldAlign(REG_HDMI_RX_NORMAL_0_CFG6 + u4RegOffset, 1, RG_HDMI_0_VCOBH);
					vIO32WriteFldAlign(REG_HDMI_RX_Normal_0_CFG2 + u4RegOffset, 0, RG_HDMI_0_EQBAND);
					vIO32WriteFldAlign(REG_HDMI_RX_Normal_0_CFG4 + u4RegOffset, 0, RG_HDMI_0_PLL_DIVEN);
				}
				else
				{
					vIO32WriteFldAlign(REG_HDMI_RX_Normal_0_CFG1 + u4RegOffset, 0, RG_HDMI_0_BAND_SEL_SW);
					vIO32Write4B(REG_HDMI_RX_Normal_0_CFG3 + u4RegOffset, 0x00212310);
					vIO32Write1B(REG_HDMI_RX_Normal_0_CFG5_3 + u4RegOffset, 0);
					vIO32WriteFldAlign(REG_HDMI_RX_NORMAL_0_CFG6 + u4RegOffset, 0, RG_HDMI_0_VCOBH);
					vIO32WriteFldAlign(REG_HDMI_RX_Normal_0_CFG2 + u4RegOffset, 2, RG_HDMI_0_EQBAND);
					vIO32WriteFldAlign(REG_HDMI_RX_Normal_0_CFG4 + u4RegOffset, 1, RG_HDMI_0_PLL_DIVEN);
				}
						 if (_bEQTuning == 0)
						 {
							 if (DRVCUST_OptQuery(eHDMIEQ_MIDDLE_KPGAIN, &u4Buf) == 0)
							 {
								 _bKPGain = (UINT8) u4Buf;
							 } else {
								 _bKPGain = 3;
							 }
							 if (DRVCUST_OptQuery(eHDMIEQ_MIDDLE_RXMODE, &u4Buf) == 0)
							 {
								 if((_bIsMhlDevice == 1) && (ePortl == HDMI_SWITCH_1))
								 {
									 _bRXMode = 1;
								 }
								 else
								 {
									 _bRXMode = (UINT8) u4Buf;
								 }
							 } else {
								 _bRXMode = 3;
							 }
							 if (DRVCUST_OptQuery(eHDMIEQ_MIDDLE_VREF, &u4Buf) == 0)
							 {
								 _bVRef = (UINT8) u4Buf;
							 } else {
								 _bVRef = 2;
							 }
						 }
				break;

			default:
			case TMDS_CLOCK_HIGH:  // > 160MHz
				vIO32WriteFldAlign(REG_HDMI_RX_Normal_0_CFG1 + u4RegOffset, 1, RG_HDMI_0_BAND_SEL_SW);
				vIO32WriteFldAlign(REG_HDMI_RX_Normal_0_CFG2 + u4RegOffset, 0, RG_HDMI_0_EQBAND);
				vIO32Write4B(REG_HDMI_RX_Normal_0_CFG3 + u4RegOffset, 0x08212310);
				vIO32WriteFldAlign(REG_HDMI_RX_Normal_0_CFG4 + u4RegOffset, 1, RG_HDMI_0_PLL_DIVEN);
				vIO32Write1B(REG_HDMI_RX_Normal_0_CFG5_3 + u4RegOffset, 0);
				vIO32WriteFldAlign(REG_HDMI_RX_NORMAL_0_CFG6 + u4RegOffset, 1, RG_HDMI_0_VCOBH);
						 if (_bEQTuning == 0)
						 {
							 if (DRVCUST_OptQuery(eHDMIEQ_HIGH_KPGAIN, &u4Buf) == 0)
							 {
								 _bKPGain = (UINT8) u4Buf;
							 } else {
								 _bKPGain = 3;
							 }
							 if (DRVCUST_OptQuery(eHDMIEQ_HIGH_RXMODE, &u4Buf) == 0)
							 {
								 _bRXMode = (UINT8) u4Buf;
							 } else {
								 _bRXMode = 1;
							 }
							 if (DRVCUST_OptQuery(eHDMIEQ_HIGH_VREF, &u4Buf) == 0)
							 {
								 _bVRef = (UINT8) u4Buf;
							 } else {
								 _bVRef = 2;
							 }
						 }
				break;
	}

	if (_bEQTuning == 0)
	{
		_bEQGain = 3;
	}
    vIO32WriteFldAlign(REG_HDMI_RX_Normal_0_CFG2 + u4RegOffset, _bKPGain, RG_HDMI_0_KPGAIN);
    vIO32WriteFldAlign(REG_HDMI_RX_Normal_0_CFG2 + u4RegOffset, _bRXMode, RG_HDMI_0_RXMODE);
    vIO32WriteFldAlign(REG_HDMI_RX_Normal_0_CFG2 + u4RegOffset, _bEQGain, RG_HDMI_0_EQ_GAIN);
    vIO32WriteFldAlign(REG_HDMI_RX_Normal_0_CFG1 + u4RegOffset, _bVRef, RG_HDMI_0_VREF);
    vUtDelay2us(10);
    vIO32WriteFldAlign(REG_HDMI_RX_Normal_0_CFG4 + u4RegOffset, 0, RG_HDMI_0_PLL_PWD);
    vUtDelay2us(15);
    vUtDelay2us(10);  // for CDR reset
    vIO32WriteFldAlign(REG_HDMI_RX_Normal_0_CFG0 + u4RegOffset, 0, RG_HDMI_0_CDR_RST);
    vUtDelay2us(5);
    vIO32WriteFldAlign(REG_HDMI_RX_Normal_0_CFG1 + u4RegOffset, 0, RG_HDMI_0_DATA_RST);
    vIO32WriteFldAlign(REG_HDMI_RX_Normal_0_CFG1 + u4RegOffset, 0, RG_HDMI_0_EQ_RST);
    //vIO32Write4B(REG_HDMI_RX_Normal_0_CFG2, 0x11210003);
    vIO32WriteFldAlign(REG_HDMI_RX_Normal_0_CFG2 + u4RegOffset, 1, RG_HDMI_0_DEEPCLRCLK_RSTN);
    // EQ tracking start point
    bCH0EQ = IO32ReadFldAlign(RGS_HDMI_RX_Normal_0_CFG0 + u4RegOffset,RGS_HDMI_0_CH0_EQERR);
    vIO32WriteFldAlign(RGS_HDMI_RX_Normal_0_CFG1 + u4RegOffset, bCH0EQ, RG_HDMI_0_EQ_FILT);

    // reset EQ again
    vIO32WriteFldAlign(REG_HDMI_RX_Normal_0_CFG1 + u4RegOffset,1,RG_HDMI_0_EQ_RST);
    vUtDelay2us(1);
    vIO32WriteFldAlign(REG_HDMI_RX_Normal_0_CFG1 + u4RegOffset,0,RG_HDMI_0_EQ_RST);
    // check if channel balance.
    // Printf("reset EQ again #3.12a 0428e\n");
    // Printf("Phy band %d\n", u1Val);
	    //tnds phy
	vMHLAdjustChanneValuel(0, 0, 0);
    bHdmiPhyReset(ePortl,HDMI_RST_DIGITALPHY);
}

static UINT8 au1TmdsClkRate[HDMI_SWITCH_MAX] = {TMDS_CLOCK_UNKNOW};
void vHdmiSaveTmdsClkRate(E_HDMI_SWITCH_NUM ePort, UINT8 u1Val)
{
    if(ePort < HDMI_SWITCH_MAX && ePort > HDMI_SWITCH_INIT)
    {
        au1TmdsClkRate[ePort] = u1Val;
    }
}

UINT8 u1HdmiGetTmdsClkRate(E_HDMI_SWITCH_NUM ePort)
{
    UINT8 u1Val = TMDS_CLOCK_UNKNOW;

    if(ePort < HDMI_SWITCH_MAX && ePort > HDMI_SWITCH_INIT)
    {
        u1Val = au1TmdsClkRate[ePort];
    }

    return (u1Val);
}

/**
 * @brief 	vHdmiPhySetting
 * @param	None
 * @retval	None
 */
static void vHdmiPhySetting(E_HDMI_SWITCH_NUM ePort)
{
    UINT8 u1Val = 0;
    UINT8 u1ClkRate= TMDS_CLOCK_UNKNOW;
    u1Val = IO32ReadFldAlign(N_HDMI_CTRL4 + u4HdmiRegBase,TMDS_CK_PERIOD);

	if(IS_IC_5399_ES1())
	{
		if(((u1Val < 0xf) && (u1Val > 0x7)) || (u1Val < 0x4))
		{
			vIO32WriteFldAlign(TMDS_CTRL0 + u4HdmiRegBase,0,C_PHY_CH0_CLK_INV);
			vIO32WriteFldAlign(TMDS_CTRL0 + u4HdmiRegBase,0,C_PHY_CH1_CLK_INV);
			vIO32WriteFldAlign(TMDS_CTRL0 + u4HdmiRegBase,0,C_PHY_CH2_CLK_INV);
		}
		else
		{
			vIO32WriteFldAlign(TMDS_CTRL0 + u4HdmiRegBase,1,C_PHY_CH0_CLK_INV);
			vIO32WriteFldAlign(TMDS_CTRL0 + u4HdmiRegBase,1,C_PHY_CH1_CLK_INV);
			vIO32WriteFldAlign(TMDS_CTRL0 + u4HdmiRegBase,1,C_PHY_CH2_CLK_INV);
		}
	}
    //27M    0x3f: 27    0x2b: 40   0x17:75   0x9:144
    //24M    0x37:27    0x25:40    0x1b:55   0x8:160
    if(IS_XTAL_24MHZ())
   	{
		if((_bIsMhlDevice == 1) && (ePort == HDMI_SWITCH_1))
		{
			if(_bIsMhlPPMode == 0)//noormal mode
			{
				if(u1Val > 0x26)
				{
					u1ClkRate = TMDS_CLOCK_LOW2;
				}
				else
				{
					u1ClkRate = TMDS_CLOCK_MIDDLE;
				}
			}
			else
			{
				if(u1Val > 0x25)
				{
					u1ClkRate = TMDS_CLOCK_LOW2;
				}
				else
				{
					u1ClkRate = TMDS_CLOCK_MIDDLE;
				}
			}
		}
		else
		{
			if(u1Val > 0x37)
			{
				u1ClkRate = TMDS_CLOCK_LOW1;
			}
			else if(u1Val > 0x25)
			{
				u1ClkRate = TMDS_CLOCK_LOW2;
			}
			else if(u1Val > 0x8)
			{
				u1ClkRate = TMDS_CLOCK_MIDDLE;
			}
			else
			{
				u1ClkRate = TMDS_CLOCK_HIGH;
			}
		}
	}

    if(u1HdmiGetTmdsClkRate(ePort) != u1ClkRate)
    {
		vHdmiSaveTmdsClkRate(ePort, u1ClkRate);
		vHDMIPhyBandCtrl(ePort,u1ClkRate);
		vSetHdmiEqTracking(ePort, 1);
    }
}


void bHdmiPhyReset(E_HDMI_SWITCH_NUM ePort, UINT8 resettype)
{
    UINT32 u4RegOffset = 0;
    UINT32 u4TmpHdmiRegBase = 0;
    UINT8 bCH0EQ;
    UNUSED(u4RegOffset);
    u4TmpHdmiRegBase = u4GetHdmiRegBase(ePort);
    u4RegOffset = u4GetAnalogRegBase(ePort);

    if(resettype == HDMI_RST_ALL)
    {
        bCH0EQ = IO32ReadFldAlign(RGS_HDMI_RX_Normal_0_CFG0 + u4RegOffset,RGS_HDMI_0_CH0_EQERR);
        vIO32WriteFldAlign(REG_HDMI_RX_Normal_0_CFG1 + u4RegOffset, bCH0EQ, RG_HDMI_0_EQ_FILT);
        // reset EQ again
        vIO32WriteFldAlign(REG_HDMI_RX_Normal_0_CFG1 + u4RegOffset, 1, RG_HDMI_0_EQ_RST);
        vUtDelay2us(1);
        vIO32WriteFldAlign(REG_HDMI_RX_Normal_0_CFG1 + u4RegOffset, 0, RG_HDMI_0_EQ_RST);
        vIO32WriteFldAlign(REG_HDMI_RX_Normal_0_CFG1 + u4RegOffset,1,RG_HDMI_0_DATA_RST);
        vUtDelay2us(1);
        // reset digital part
        vIO32WriteFldAlign(TMDS_CTRL0 + u4TmpHdmiRegBase, 0, TMDS_PHY_RST_);  // reset digital PHY
        vUtDelay2us(1);
        vIO32WriteFldAlign(TMDS_CTRL0 + u4TmpHdmiRegBase, 1, TMDS_PHY_RST_);
        //vIO32WriteFldAlign(SRST + u4RegOffset, 1, SW_RST);  // reset rx digital
        //vUtDelay2us(1);
        //vIO32WriteFldAlign(SRST + u4RegOffset, 0, SW_RST);
        vUtDelay2us(20);
        vIO32WriteFldAlign(REG_HDMI_RX_Normal_0_CFG1 + u4RegOffset,0,RG_HDMI_0_DATA_RST);
        vIO32WriteFldAlign(TMDS_CTRL0 + u4TmpHdmiRegBase, 0, TMDS_PHY_RST_);  // reset digital PHY
        vUtDelay2us(1);
        vIO32WriteFldAlign(TMDS_CTRL0 + u4TmpHdmiRegBase, 1, TMDS_PHY_RST_);
        vIO32WriteFldAlign(TMDS_CTRL0 + u4TmpHdmiRegBase, 1, C_DATA_SYNC_AUTO);
        vIO32WriteFldAlign(MUTE_SET + u4TmpHdmiRegBase, 1, TDFIFO_SYNC_EN);
        vUtDelay2us(1);
        vIO32WriteFldAlign(TMDS_CTRL0 + u4TmpHdmiRegBase, 0, C_DATA_SYNC_AUTO);
        vIO32WriteFldAlign(MUTE_SET + u4TmpHdmiRegBase, 0, TDFIFO_SYNC_EN);
    }

    if(resettype == HDMI_RST_EQ)
    {
        bCH0EQ = IO32ReadFldAlign(RGS_HDMI_RX_Normal_0_CFG0 + u4RegOffset,RGS_HDMI_0_CH0_EQERR);
        vIO32WriteFldAlign(REG_HDMI_RX_Normal_0_CFG1 + u4RegOffset, bCH0EQ, RG_HDMI_0_EQ_FILT);
        // reset EQ again
        vIO32WriteFldAlign(REG_HDMI_RX_Normal_0_CFG1 + u4RegOffset, 1, RG_HDMI_0_EQ_RST);
        vUtDelay2us(1);
        vIO32WriteFldAlign(REG_HDMI_RX_Normal_0_CFG1 + u4RegOffset, 0, RG_HDMI_0_EQ_RST);
    }

    if(resettype == HDMI_RST_DIGITALPHY)
    {
        vIO32WriteFldAlign(REG_HDMI_RX_Normal_0_CFG1 + u4RegOffset,1,RG_HDMI_0_DATA_RST);
        vUtDelay2us(1);
        // reset digital part
        vIO32WriteFldAlign(TMDS_CTRL0 + u4TmpHdmiRegBase, 0, TMDS_PHY_RST_);  // reset digital PHY
        vUtDelay2us(1);
        vIO32WriteFldAlign(TMDS_CTRL0 + u4TmpHdmiRegBase, 1, TMDS_PHY_RST_);
        //vIO32WriteFldAlign(SRST + u4RegOffset, 1, SW_RST);  // reset rx digital
        //vUtDelay2us(1);
        //vIO32WriteFldAlign(SRST + u4RegOffset, 0, SW_RST);
        vUtDelay2us(20);
        vIO32WriteFldAlign(REG_HDMI_RX_Normal_0_CFG1 + u4RegOffset,0,RG_HDMI_0_DATA_RST);
        vIO32WriteFldAlign(TMDS_CTRL0 + u4TmpHdmiRegBase, 0, TMDS_PHY_RST_);  // reset digital PHY
        vUtDelay2us(1);
        vIO32WriteFldAlign(TMDS_CTRL0 + u4TmpHdmiRegBase, 1, TMDS_PHY_RST_);
    }

    if(resettype == HDMI_RST_RTCK)
    {
        vIO32WriteFldAlign(TMDS_CTRL0 + u4TmpHdmiRegBase, 1, C_DATA_SYNC_AUTO);
        vIO32WriteFldAlign(TMDS_CTRL0 + u4TmpHdmiRegBase, 0, C_DATA_SYNC_AUTO);
    }

    if(resettype == HDMI_RST_DEEPCOLOR)
    {
        vIO32WriteFldAlign(MUTE_SET + u4TmpHdmiRegBase, 1, TDFIFO_SYNC_EN);
        vUtDelay2us(1);
        vIO32WriteFldAlign(MUTE_SET + u4TmpHdmiRegBase, 0, TDFIFO_SYNC_EN);
    }

}

static void vHdmiEqTracking(E_HDMI_SWITCH_NUM ePort)
{
    UINT8 bEQCH0_tmp, bEQCH1_tmp, bEQCH2_tmp;

    if((IO32ReadFldAlign(RGS_HDMI_RX_Normal_0_CFG0 + u4AnalogRegBase,RGS_HDMI_0_CH0_STATUS)&0x100) == 0x100)
    {
        bEQCH0_tmp = IO32ReadFldAlign(RGS_HDMI_RX_Normal_0_CFG0 + u4AnalogRegBase, RGS_HDMI_0_CH0_EQERR);
        bEQCH1_tmp = IO32ReadFldAlign(RGS_HDMI_RX_Normal_0_CFG1 + u4AnalogRegBase, RGS_HDMI_0_CH1_EQERR);
        bEQCH2_tmp = IO32ReadFldAlign(RGS_HDMI_RX_Normal_0_CFG2 + u4AnalogRegBase, RGS_HDMI_0_CH2_EQERR);

        //  LOG(6, "==== EQ_temp: 0x%x, 0x%x, 0x%x ====\n", bEQCH0_tmp, bEQCH1_tmp, bEQCH2_tmp);

        if((Diff(bEQCH0_tmp, _arEq[ePort].Ch0) >= 3) || \
           (Diff(bEQCH1_tmp, _arEq[ePort].Ch1) >= 3) || \
           (Diff(bEQCH2_tmp, _arEq[ePort].Ch2) >= 3))
        {
            bHdmiPhyReset(ePort, HDMI_RST_EQ);
            // LOG(6, "mc: Reset HDMI PHY EQ again #1, CH0:%d/%d, CH1:%d/%d, CH2:%d/%d \n", bEQCH0_tmp, _arEq[ePort].Ch0, bEQCH1_tmp, _arEq[ePort].Ch1, bEQCH2_tmp, _arEq[ePort].Ch2);
            _arEq[ePort].Ch0 = bEQCH0_tmp;
            _arEq[ePort].Ch1 = bEQCH1_tmp;
            _arEq[ePort].Ch2 = bEQCH2_tmp;
        }
        else if((Diff(bEQCH0_tmp, bEQCH1_tmp) >= 3) || \
                (Diff(bEQCH0_tmp, bEQCH2_tmp) >= 3) || \
                (Diff(bEQCH1_tmp, bEQCH2_tmp) >= 3))
        {
            bHdmiPhyReset(ePort, HDMI_RST_EQ);
            // LOG(6, "mc: Reset HDMI PHY EQ again #2, CH0:%d, CH1:%d, CH2:%d \n", bEQCH0_tmp, bEQCH1_tmp, bEQCH2_tmp);
            _arEq[ePort].Ch0 = bEQCH0_tmp;
            _arEq[ePort].Ch1 = bEQCH1_tmp;
            _arEq[ePort].Ch2 = bEQCH2_tmp;
        }
    }
    else
    {
        _arEq[ePort].Ch0 = 0xf;
        _arEq[ePort].Ch1 = 0xf;
        _arEq[ePort].Ch2 = 0xf;
        //LOG(6, "mc: !!! Reset HDMI PHY EQ again #0, CH0:%d, CH1:%d, CH2:%d \n", _arEq[ePort].Ch0, _arEq[ePort].Ch1, _arEq[ePort].Ch2);
    }
}


#if defined(CC_MT5396) || defined(CC_MT5398) || defined(CC_MT5880) || defined(CC_MT5399)
void vHDMISel(E_HDMI_SWITCH_NUM eSel)
{
    u4ActiveHdmiRegBase = u4GetHdmiRegBase(eSel);
    u4ActiveAnalogRegBase = u4GetAnalogRegBase(eSel);

    switch(eSel)
    {
        default:
        case HDMI_SWITCH_1:
            _arHdmiRx[eSel]._bRxShared = 0;
            break;

        case HDMI_SWITCH_2:
            _arHdmiRx[eSel]._bRxShared = 0;
            break;

        case HDMI_SWITCH_3:
            _arHdmiRx[eSel]._bRxShared = 0;
            break;
#ifndef CC_MT5880
        case HDMI_SWITCH_4:
            _arHdmiRx[eSel]._bRxShared = 0;
            break;
#endif
    }

    u4HdmiRegBase = u4ActiveHdmiRegBase;
    u4AnalogRegBase = u4ActiveAnalogRegBase;

#if defined(CC_MT5396) || defined(CC_MT5398) || defined(CC_MT5399)
    switch(eSel)
    {
        default:
        case HDMI_SWITCH_1:
            // main clk
            vIO32WriteFldAlign(HDMI_SEL, 1, REG_MAIN_CLK_SEL0);
			//pip clk
			 vIO32WriteFldAlign(HDMI_PIP_SEL,1,REG_PIP_CLK_SEL0);
            // int
            vIO32WriteFldAlign(HDMI_SEL, 1, REG_INT_OUT_RACHEL_SEL);
            // acr
            vIO32WriteFldAlign(HDMI_SEL, 1, REG_ACR_OUT_RACHEL_SEL);
            // macro
            vIO32WriteFldAlign(HDMI_SEL, 1, REG_REGISTER_OUT_RACHEL_SEL);
            // aout
            vIO32WriteFldAlign(HDMI_SEL, 1, REG_AUDIO_PATH_OUT_RACHEL_SEL);
            // video out
            vIO32WriteFldAlign(HDMI_SEL, 1, REG_VIDEO_PATH_OUT_RACHEL_SEL);
            // clk
            vIO32WriteFldAlign(HDMI_SEL, 1, REG_CLK_IN_RACHEL_SEL);
            // audio in
            vIO32WriteFldAlign(HDMI_SEL, 1, REG_AUDIO_PATH_IN_RACHEL_SEL);
            // video in
            vIO32WriteFldAlign(HDMI_SEL, 1, REG_VIDEO_PATH_IN_RACHEL_SEL);
			if(_bIsMhlDevice == 1)
			{
				// mhl clk
				vIO32WriteFldAlign(CKGEN_REG_MHL_CKCFG, 1, FLD_MHL_DPCLK_SEL);
				vIO32WriteFldAlign(CKGEN_REG_MHL_CKCFG, 1, FLD_MHL_PCLK_SEL);
				//sel work mode
				vIO32WriteFldAlign(MHL_REG, 1, REG_MHL_MODE_SEL);
			}
			else
			{
				// hdmi clk
				vIO32WriteFldAlign(CKGEN_REG_MHL_CKCFG, 0, FLD_MHL_DPCLK_SEL);
				vIO32WriteFldAlign(CKGEN_REG_MHL_CKCFG, 0, FLD_MHL_PCLK_SEL);
				//sel work mode
				vIO32WriteFldAlign(MHL_REG, 0, REG_MHL_MODE_SEL);
			}
            break;

        case HDMI_SWITCH_2:
            // main clk
            vIO32WriteFldAlign(HDMI_SEL, 0, REG_MAIN_CLK_SEL0);
            vIO32WriteFldAlign(HDMI_SEL, 1, REG_MAIN_CLK_SEL1);
			//pip clk
			vIO32WriteFldAlign(HDMI_PIP_SEL,0,REG_PIP_CLK_SEL0);
			vIO32WriteFldAlign(HDMI_PIP_SEL,1,REG_PIP_CLK_SEL1);
            // int
            vIO32WriteFldAlign(HDMI_SEL, 0, REG_INT_OUT_RACHEL_SEL);
            vIO32WriteFldAlign(HDMI_SEL, 1, REG_INT_OUT_MAVIS_SEL1);
            // acr
            vIO32WriteFldAlign(HDMI_SEL, 0, REG_ACR_OUT_RACHEL_SEL);
            vIO32WriteFldAlign(HDMI_SEL, 1, REG_ACR_OUT_MAVIS_SEL1);
            // macro
            vIO32WriteFldAlign(HDMI_SEL, 0, REG_REGISTER_OUT_RACHEL_SEL);
            vIO32WriteFldAlign(HDMI_SEL, 1, REG_REGISTER_OUT_MAVIS_SEL1);
            // aout
            vIO32WriteFldAlign(HDMI_SEL, 0, REG_AUDIO_PATH_OUT_RACHEL_SEL);
            vIO32WriteFldAlign(HDMI_SEL, 1, REG_AUDIO_PATH_OUT_MAVIS_SEL1);
            // video out
            vIO32WriteFldAlign(HDMI_SEL, 0, REG_VIDEO_PATH_OUT_RACHEL_SEL);
            vIO32WriteFldAlign(HDMI_SEL, 1, REG_VIDEO_PATH_OUT_MAVIS_SEL1);
            // clk
            vIO32WriteFldAlign(HDMI_SEL, 0, REG_CLK_IN_RACHEL_SEL);
            vIO32WriteFldAlign(HDMI_SEL, 1, REG_CLK_IN_MAVIS_SEL1);
            // audio in
            vIO32WriteFldAlign(HDMI_SEL, 0, REG_AUDIO_PATH_IN_RACHEL_SEL);
            vIO32WriteFldAlign(HDMI_SEL, 1, REG_AUDIO_PATH_IN_MAVIS_SEL1);
            // video in
            vIO32WriteFldAlign(HDMI_SEL, 0, REG_VIDEO_PATH_IN_RACHEL_SEL);
            vIO32WriteFldAlign(HDMI_SEL, 1, REG_VIDEO_PATH_IN_MAVIS_SEL1);
			// hdmi clk
			vIO32WriteFldAlign(CKGEN_REG_MHL_CKCFG, 0, FLD_MHL_DPCLK_SEL);
			vIO32WriteFldAlign(CKGEN_REG_MHL_CKCFG, 0, FLD_MHL_PCLK_SEL);
            break;

        case HDMI_SWITCH_3:
            // main clk
            vIO32WriteFldAlign(HDMI_SEL, 0, REG_MAIN_CLK_SEL0);
            vIO32WriteFldAlign(HDMI_SEL, 0, REG_MAIN_CLK_SEL1);
            vIO32WriteFldAlign(HDMI_SEL, 1, REG_MAIN_CLK_SEL2);
			//pip clk
			vIO32WriteFldAlign(HDMI_PIP_SEL,0,REG_PIP_CLK_SEL0);
			vIO32WriteFldAlign(HDMI_PIP_SEL,0,REG_PIP_CLK_SEL1);
			vIO32WriteFldAlign(HDMI_PIP_SEL,1,REG_PIP_CLK_SEL2);
            // int
            vIO32WriteFldAlign(HDMI_SEL, 0, REG_INT_OUT_RACHEL_SEL);
            vIO32WriteFldAlign(HDMI_SEL, 0, REG_INT_OUT_RACHEL_SEL);
            vIO32WriteFldAlign(HDMI_SEL, 1, REG_INT_OUT_MAVIS_SEL2);
            // acr
            vIO32WriteFldAlign(HDMI_SEL, 0, REG_ACR_OUT_RACHEL_SEL);
            vIO32WriteFldAlign(HDMI_SEL, 0, REG_ACR_OUT_MAVIS_SEL1);
            vIO32WriteFldAlign(HDMI_SEL, 1, REG_ACR_OUT_MAVIS_SEL2);
            // macro
            vIO32WriteFldAlign(HDMI_SEL, 0, REG_REGISTER_OUT_RACHEL_SEL);
            vIO32WriteFldAlign(HDMI_SEL, 0, REG_REGISTER_OUT_MAVIS_SEL1);
            vIO32WriteFldAlign(HDMI_SEL, 1, REG_REGISTER_OUT_MAVIS_SEL2);
            // aout
            vIO32WriteFldAlign(HDMI_SEL, 0, REG_AUDIO_PATH_OUT_RACHEL_SEL);
            vIO32WriteFldAlign(HDMI_SEL, 0, REG_AUDIO_PATH_OUT_MAVIS_SEL1);
            vIO32WriteFldAlign(HDMI_SEL, 1, REG_AUDIO_PATH_OUT_MAVIS_SEL2);
            // video out
            vIO32WriteFldAlign(HDMI_SEL, 0, REG_VIDEO_PATH_OUT_RACHEL_SEL);
            vIO32WriteFldAlign(HDMI_SEL, 0, REG_VIDEO_PATH_OUT_MAVIS_SEL1);
            vIO32WriteFldAlign(HDMI_SEL, 1, REG_VIDEO_PATH_OUT_MAVIS_SEL2);
            // clk
            vIO32WriteFldAlign(HDMI_SEL, 0, REG_CLK_IN_RACHEL_SEL);
            vIO32WriteFldAlign(HDMI_SEL, 0, REG_CLK_IN_MAVIS_SEL1);
            vIO32WriteFldAlign(HDMI_SEL, 1, REG_CLK_IN_MAVIS_SEL2);
            // audio in
            vIO32WriteFldAlign(HDMI_SEL, 0, REG_AUDIO_PATH_IN_RACHEL_SEL);
            vIO32WriteFldAlign(HDMI_SEL, 0, REG_AUDIO_PATH_IN_MAVIS_SEL1);
            vIO32WriteFldAlign(HDMI_SEL, 1, REG_AUDIO_PATH_IN_MAVIS_SEL2);
            // video in
            vIO32WriteFldAlign(HDMI_SEL, 0, REG_VIDEO_PATH_IN_RACHEL_SEL);
            vIO32WriteFldAlign(HDMI_SEL, 0, REG_VIDEO_PATH_IN_MAVIS_SEL1);
            vIO32WriteFldAlign(HDMI_SEL, 1, REG_VIDEO_PATH_IN_MAVIS_SEL2);
			// hdmi clk
			vIO32WriteFldAlign(CKGEN_REG_MHL_CKCFG, 0, FLD_MHL_DPCLK_SEL);
			vIO32WriteFldAlign(CKGEN_REG_MHL_CKCFG, 0, FLD_MHL_PCLK_SEL);
            break;

        case HDMI_SWITCH_4:
            // main clk
            vIO32WriteFldAlign(HDMI_SEL, 0, REG_MAIN_CLK_SEL0);
            vIO32WriteFldAlign(HDMI_SEL, 0, REG_MAIN_CLK_SEL1);
            vIO32WriteFldAlign(HDMI_SEL, 0, REG_MAIN_CLK_SEL2);
			//pip clk
			vIO32WriteFldAlign(HDMI_PIP_SEL,0,REG_PIP_CLK_SEL0);
			vIO32WriteFldAlign(HDMI_PIP_SEL,0,REG_PIP_CLK_SEL1);
			vIO32WriteFldAlign(HDMI_PIP_SEL,0,REG_PIP_CLK_SEL2);
            // int
            vIO32WriteFldAlign(HDMI_SEL, 0, REG_INT_OUT_RACHEL_SEL);
            vIO32WriteFldAlign(HDMI_SEL, 0, REG_INT_OUT_MAVIS_SEL1);
            vIO32WriteFldAlign(HDMI_SEL, 0, REG_INT_OUT_MAVIS_SEL2);
            // acr
            vIO32WriteFldAlign(HDMI_SEL, 0, REG_ACR_OUT_RACHEL_SEL);
            vIO32WriteFldAlign(HDMI_SEL, 0, REG_ACR_OUT_MAVIS_SEL1);
            vIO32WriteFldAlign(HDMI_SEL, 0, REG_ACR_OUT_MAVIS_SEL2);
            // macro
            vIO32WriteFldAlign(HDMI_SEL, 0, REG_REGISTER_OUT_RACHEL_SEL);
            vIO32WriteFldAlign(HDMI_SEL, 0, REG_REGISTER_OUT_MAVIS_SEL1);
            vIO32WriteFldAlign(HDMI_SEL, 0, REG_REGISTER_OUT_MAVIS_SEL2);
            // aout
            vIO32WriteFldAlign(HDMI_SEL, 0, REG_AUDIO_PATH_OUT_RACHEL_SEL);
            vIO32WriteFldAlign(HDMI_SEL, 0, REG_AUDIO_PATH_OUT_MAVIS_SEL1);
            vIO32WriteFldAlign(HDMI_SEL, 0, REG_AUDIO_PATH_OUT_MAVIS_SEL2);
            // video out
            vIO32WriteFldAlign(HDMI_SEL, 0, REG_VIDEO_PATH_OUT_RACHEL_SEL);
            vIO32WriteFldAlign(HDMI_SEL, 0, REG_VIDEO_PATH_OUT_MAVIS_SEL1);
            vIO32WriteFldAlign(HDMI_SEL, 0, REG_VIDEO_PATH_OUT_MAVIS_SEL2);
            // clk
            vIO32WriteFldAlign(HDMI_SEL, 0, REG_CLK_IN_RACHEL_SEL);
            vIO32WriteFldAlign(HDMI_SEL, 0, REG_CLK_IN_MAVIS_SEL1);
            vIO32WriteFldAlign(HDMI_SEL, 0, REG_CLK_IN_MAVIS_SEL2);
            // audio in
            vIO32WriteFldAlign(HDMI_SEL, 0, REG_AUDIO_PATH_IN_RACHEL_SEL);
            vIO32WriteFldAlign(HDMI_SEL, 0, REG_AUDIO_PATH_IN_MAVIS_SEL1);
            vIO32WriteFldAlign(HDMI_SEL, 0, REG_AUDIO_PATH_IN_MAVIS_SEL2);
            // video in
            vIO32WriteFldAlign(HDMI_SEL, 0, REG_VIDEO_PATH_IN_RACHEL_SEL);
            vIO32WriteFldAlign(HDMI_SEL, 0, REG_VIDEO_PATH_IN_MAVIS_SEL1);
            vIO32WriteFldAlign(HDMI_SEL, 0, REG_VIDEO_PATH_IN_MAVIS_SEL2);
			// hdmi clk
			vIO32WriteFldAlign(CKGEN_REG_MHL_CKCFG, 0, FLD_MHL_DPCLK_SEL);
			vIO32WriteFldAlign(CKGEN_REG_MHL_CKCFG, 0, FLD_MHL_PCLK_SEL);
            break;
    };

#elif defined(CC_MT5880)
switch(eSel)
{
	default:
	case HDMI_SWITCH_1:
		// main clk
		vIO32WriteFldAlign(HDMI_SEL, 1, REG_CLK_OUT_RACHEL_SEL);
		// pip clk
		if(bGetICInputType(SV_VP_PIP)!=P_OSDENCODE)
		{
			vIO32WriteFldAlign(HDMI_PIP_SEL, 1, REG_VIDEO_OUT_PIP_MAVIS_SEL1);
			vIO32WriteFldAlign(HDMI_PIP_SEL, 1, REG_VIDEO_OUT_PIP_MAVIS_SEL);
		}
		// int
		vIO32WriteFldAlign(HDMI_SEL, 1, REG_INT_OUT_RACHEL_SEL);
		vIO32WriteFldAlign(HDMI_SEL, 1, REG_INT_OUT_MAVIS_SEL);
		// acr out
		vIO32WriteFldAlign(HDMI_SEL, 1, REG_ACR_OUT_RACHEL_SEL);
		vIO32WriteFldAlign(HDMI_SEL, 1, REG_ACR_OUT_MAVIS_SEL);
		// macro
		vIO32WriteFldAlign(HDMI_SEL, 1, REG_REGISTER_OUT_RACHEL_SEL);
		vIO32WriteFldAlign(HDMI_SEL, 1, REG_REGISTER_OUT_MAVIS_SEL);
		// aout
		vIO32WriteFldAlign(HDMI_SEL, 1, REG_AUDIO_PATH_OUT_RACHEL_SEL);
		vIO32WriteFldAlign(HDMI_SEL, 1, REG_AUDIO_PATH_OUT_MAVIS_SEL);
		// video out
		vIO32WriteFldAlign(HDMI_SEL, 1, REG_VIDEO_PATH_OUT_RACHEL_SEL);
		vIO32WriteFldAlign(HDMI_SEL, 1, REG_VIDEO_PATH_OUT_MAVIS_SEL);
		// acr in
		vIO32WriteFldAlign(HDMI_SEL, 1, REG_ACR_IN_RACHEL_SEL);
		vIO32WriteFldAlign(HDMI_SEL, 1, REG_ACR_IN_MAVIS_SEL);
		// clk
		vIO32WriteFldAlign(HDMI_SEL, 1, REG_CLK_IN_RACHEL_SEL);
		vIO32WriteFldAlign(HDMI_SEL, 1, REG_CLK_IN_MAVIS_SEL);
		// audio in
		vIO32WriteFldAlign(HDMI_SEL, 1, REG_AUDIO_PATH_IN_RACHEL_SEL);
		vIO32WriteFldAlign(HDMI_SEL, 1, REG_AUDIO_PATH_IN_MAVIS_SEL);
		// video in
		vIO32WriteFldAlign(HDMI_SEL, 1, REG_VIDEO_PATH_IN_RACHEL_SEL);
		vIO32WriteFldAlign(HDMI_SEL, 1, REG_VIDEO_PATH_IN_MAVIS_SEL);
		break;

	case HDMI_SWITCH_2:
		// main clk
		vIO32WriteFldAlign(HDMI_SEL, 0, REG_CLK_OUT_RACHEL_SEL);
		vIO32WriteFldAlign(HDMI_SEL, 1, REG_CLK_OUT_MAVIS_SEL);
		// pip clk
		if(bGetICInputType(SV_VP_PIP)!=P_OSDENCODE)
		{
			vIO32WriteFldAlign(HDMI_PIP_SEL, 1, REG_VIDEO_OUT_PIP_MAVIS_SEL1);
			vIO32WriteFldAlign(HDMI_PIP_SEL, 0, REG_VIDEO_OUT_PIP_MAVIS_SEL);
		}
		// int
		vIO32WriteFldAlign(HDMI_SEL, 0, REG_INT_OUT_RACHEL_SEL);
		vIO32WriteFldAlign(HDMI_SEL, 1, REG_INT_OUT_MAVIS_SEL);
		// acr
		vIO32WriteFldAlign(HDMI_SEL, 0, REG_ACR_OUT_RACHEL_SEL);
		vIO32WriteFldAlign(HDMI_SEL, 1, REG_ACR_OUT_MAVIS_SEL);
		// macro
		vIO32WriteFldAlign(HDMI_SEL, 0, REG_REGISTER_OUT_RACHEL_SEL);
		vIO32WriteFldAlign(HDMI_SEL, 1, REG_REGISTER_OUT_MAVIS_SEL);
		// aout
		vIO32WriteFldAlign(HDMI_SEL, 0, REG_AUDIO_PATH_OUT_RACHEL_SEL);
		vIO32WriteFldAlign(HDMI_SEL, 1, REG_AUDIO_PATH_OUT_MAVIS_SEL);
		// video out
		vIO32WriteFldAlign(HDMI_SEL, 0, REG_VIDEO_PATH_OUT_RACHEL_SEL);
		vIO32WriteFldAlign(HDMI_SEL, 1, REG_VIDEO_PATH_OUT_MAVIS_SEL);
		// acr in
		vIO32WriteFldAlign(HDMI_SEL, 0, REG_ACR_IN_RACHEL_SEL);
		vIO32WriteFldAlign(HDMI_SEL, 1, REG_ACR_IN_MAVIS_SEL);
		// clk
		vIO32WriteFldAlign(HDMI_SEL, 0, REG_CLK_IN_RACHEL_SEL);
		vIO32WriteFldAlign(HDMI_SEL, 1, REG_CLK_IN_MAVIS_SEL);
		// audio in
		vIO32WriteFldAlign(HDMI_SEL, 0, REG_AUDIO_PATH_IN_RACHEL_SEL);
		vIO32WriteFldAlign(HDMI_SEL, 1, REG_AUDIO_PATH_IN_MAVIS_SEL);
		// video in
		vIO32WriteFldAlign(HDMI_SEL, 0, REG_VIDEO_PATH_IN_RACHEL_SEL);
		vIO32WriteFldAlign(HDMI_SEL, 1, REG_VIDEO_PATH_IN_MAVIS_SEL);
		break;

	case HDMI_SWITCH_3:
		vIO32WriteFldAlign(HDMI_SEL, 0, REG_CLK_OUT_RACHEL_SEL);
		vIO32WriteFldAlign(HDMI_SEL, 0, REG_CLK_OUT_MAVIS_SEL);
		// pip clk
		if(bGetICInputType(SV_VP_PIP)!=P_OSDENCODE)
		{
			vIO32WriteFldAlign(HDMI_PIP_SEL, 0, REG_VIDEO_OUT_PIP_MAVIS_SEL1);
			vIO32WriteFldAlign(HDMI_PIP_SEL, 0, REG_VIDEO_OUT_PIP_MAVIS_SEL);
		}
		// int
		vIO32WriteFldAlign(HDMI_SEL, 0, REG_INT_OUT_RACHEL_SEL);
		vIO32WriteFldAlign(HDMI_SEL, 0, REG_INT_OUT_MAVIS_SEL);
		// acr
		vIO32WriteFldAlign(HDMI_SEL, 0, REG_ACR_OUT_RACHEL_SEL);
		vIO32WriteFldAlign(HDMI_SEL, 0, REG_ACR_OUT_MAVIS_SEL);
		// macro
		vIO32WriteFldAlign(HDMI_SEL, 0, REG_REGISTER_OUT_RACHEL_SEL);
		vIO32WriteFldAlign(HDMI_SEL, 0, REG_REGISTER_OUT_MAVIS_SEL);
		// aout
		vIO32WriteFldAlign(HDMI_SEL, 0, REG_AUDIO_PATH_OUT_RACHEL_SEL);
		vIO32WriteFldAlign(HDMI_SEL, 0, REG_AUDIO_PATH_OUT_MAVIS_SEL);
		// video out
		vIO32WriteFldAlign(HDMI_SEL, 0, REG_VIDEO_PATH_OUT_RACHEL_SEL);
		vIO32WriteFldAlign(HDMI_SEL, 0, REG_VIDEO_PATH_OUT_MAVIS_SEL);
		// acr in
		vIO32WriteFldAlign(HDMI_SEL, 0, REG_ACR_IN_RACHEL_SEL);
		vIO32WriteFldAlign(HDMI_SEL, 0, REG_ACR_IN_MAVIS_SEL);
		// clk
		vIO32WriteFldAlign(HDMI_SEL, 0, REG_CLK_IN_RACHEL_SEL);
		vIO32WriteFldAlign(HDMI_SEL, 0, REG_CLK_IN_MAVIS_SEL);
		// audio in
		vIO32WriteFldAlign(HDMI_SEL, 0, REG_AUDIO_PATH_IN_RACHEL_SEL);
		vIO32WriteFldAlign(HDMI_SEL, 0, REG_AUDIO_PATH_IN_MAVIS_SEL);
		// video in
		vIO32WriteFldAlign(HDMI_SEL, 0, REG_VIDEO_PATH_IN_RACHEL_SEL);
		vIO32WriteFldAlign(HDMI_SEL, 0, REG_VIDEO_PATH_IN_MAVIS_SEL);
		break;
};

#endif
}

UINT8 u1IsHdcpGood(E_HDMI_SWITCH_NUM eSrc)
{
    UINT32 u4RegBase = 0;
    UINT8 u1Ret = 0;
    u4RegBase = u4GetHdmiRegBase(eSrc);

    if(IO32ReadFldAlign(HDCP_STAT + u4RegBase, HDCP_DECRYPT))
    {
        u1Ret = 1;
    }

    return (u1Ret);
}

UINT8 u1IsResStable(E_HDMI_SWITCH_NUM eSrc)
{
    UINT32 u4RegBase = 0;
    UINT8 u1Ret = 0;
    u4RegBase = u4GetHdmiRegBase(eSrc);

    if(IO32ReadFldAlign(VID_HRES + u4RegBase, VID_HRES_STB) && IO32ReadFldAlign(VID_VRES + u4RegBase, VID_VRES_STB))
    {
        u1Ret = 1;
    }

    return (u1Ret);
}

#elif  defined(CC_MT5368)
void vHDMISel(E_HDMI_SWITCH_NUM eSel)
{
    u4ActiveHdmiRegBase = u4GetHdmiRegBase(eSel);
    u4ActiveAnalogRegBase = u4GetAnalogRegBase(eSel);

    switch(eSel)
    {
        case HDMI_SWITCH_1:
            _arHdmiRx[eSel]._bRxShared = 0;
            _arHdmiRx[HDMI_SWITCH_2]._bRxShared = 1;
            vIO32WriteFldAlign(REG_HDMI_RX_Normal_0_CFG2 + 0x100, 4, RG_HDMI_0_SEL);
            vIO32WriteFldAlign(PDWNC_PDMISC, 0, FLD_HDMI_SEL);
            break;

        case HDMI_SWITCH_2:
            _arHdmiRx[eSel]._bRxShared = 0;
            _arHdmiRx[HDMI_SWITCH_1]._bRxShared = 1;
            vIO32WriteFldAlign(REG_HDMI_RX_Normal_0_CFG2 + 0x100, 2, RG_HDMI_0_SEL);
            vIO32WriteFldAlign(PDWNC_PDMISC, 1, FLD_HDMI_SEL);
            break;

        case HDMI_SWITCH_3:
            _arHdmiRx[eSel]._bRxShared = 0;
            break;

        case HDMI_SWITCH_4:
            _arHdmiRx[eSel]._bRxShared = 0;
            break;

        default:
            Printf("unknown vHDMISel %d",eSel);
            break;
    }

    u4HdmiRegBase = u4ActiveHdmiRegBase;
    u4AnalogRegBase = u4ActiveAnalogRegBase;

    switch(eSel)
    {
        default:
        case HDMI_SWITCH_1:
            // int
            vIO32WriteFldAlign(HDMI_SEL, 1, REG_INT_OUT_RACHEL_SEL);
            vIO32WriteFldAlign(HDMI_SEL, 1, REG_INT_OUT_MAVIS_SEL);
            // acr out
            vIO32WriteFldAlign(HDMI_SEL, 1, REG_ACR_OUT_RACHEL_SEL);
            vIO32WriteFldAlign(HDMI_SEL, 1, REG_ACR_OUT_MAVIS_SEL);
            // macro
            vIO32WriteFldAlign(HDMI_SEL, 1, REG_REGISTER_OUT_RACHEL_SEL);
            vIO32WriteFldAlign(HDMI_SEL, 1, REG_REGISTER_OUT_MAVIS_SEL);
            // aout
            vIO32WriteFldAlign(HDMI_SEL, 1, REG_AUDIO_PATH_OUT_RACHEL_SEL);
            vIO32WriteFldAlign(HDMI_SEL, 1, REG_AUDIO_PATH_OUT_MAVIS_SEL);
            // video out
            vIO32WriteFldAlign(HDMI_SEL, 1, REG_VIDEO_PATH_OUT_RACHEL_SEL);
            vIO32WriteFldAlign(HDMI_SEL, 1, REG_VIDEO_PATH_OUT_MAVIS_SEL);
            // acr in
            vIO32WriteFldAlign(HDMI_SEL, 1, REG_ACR_IN_RACHEL_SEL);
            vIO32WriteFldAlign(HDMI_SEL, 1, REG_ACR_IN_MAVIS_SEL);
            // clk
            vIO32WriteFldAlign(HDMI_SEL, 1, REG_CLK_IN_RACHEL_SEL);
            vIO32WriteFldAlign(HDMI_SEL, 1, REG_CLK_IN_MAVIS_SEL);
            // audio in
            vIO32WriteFldAlign(HDMI_SEL, 1, REG_AUDIO_PATH_IN_RACHEL_SEL);
            vIO32WriteFldAlign(HDMI_SEL, 1, REG_AUDIO_PATH_IN_MAVIS_SEL);
            // video in
            vIO32WriteFldAlign(HDMI_SEL, 1, REG_VIDEO_PATH_IN_RACHEL_SEL);
            vIO32WriteFldAlign(HDMI_SEL, 1, REG_VIDEO_PATH_IN_MAVIS_SEL);
            break;

        case HDMI_SWITCH_2:
            // int
            vIO32WriteFldAlign(HDMI_SEL, 1, REG_INT_OUT_RACHEL_SEL);
            vIO32WriteFldAlign(HDMI_SEL, 1, REG_INT_OUT_MAVIS_SEL);
            // acr
            vIO32WriteFldAlign(HDMI_SEL, 1, REG_ACR_OUT_RACHEL_SEL);
            vIO32WriteFldAlign(HDMI_SEL, 1, REG_ACR_OUT_MAVIS_SEL);
            // macro
            vIO32WriteFldAlign(HDMI_SEL, 1, REG_REGISTER_OUT_RACHEL_SEL);
            vIO32WriteFldAlign(HDMI_SEL, 1, REG_REGISTER_OUT_MAVIS_SEL);
            // aout
            vIO32WriteFldAlign(HDMI_SEL, 1, REG_AUDIO_PATH_OUT_RACHEL_SEL);
            vIO32WriteFldAlign(HDMI_SEL, 1, REG_AUDIO_PATH_OUT_MAVIS_SEL);
            // video out
            vIO32WriteFldAlign(HDMI_SEL, 1, REG_VIDEO_PATH_OUT_RACHEL_SEL);
            vIO32WriteFldAlign(HDMI_SEL, 1, REG_VIDEO_PATH_OUT_MAVIS_SEL);
            // acr in
            vIO32WriteFldAlign(HDMI_SEL, 1, REG_ACR_IN_RACHEL_SEL);
            vIO32WriteFldAlign(HDMI_SEL, 1, REG_ACR_IN_MAVIS_SEL);
            // clk
            vIO32WriteFldAlign(HDMI_SEL, 1, REG_CLK_IN_RACHEL_SEL);
            vIO32WriteFldAlign(HDMI_SEL, 1, REG_CLK_IN_MAVIS_SEL);
            // audio in
            vIO32WriteFldAlign(HDMI_SEL, 1, REG_AUDIO_PATH_IN_RACHEL_SEL);
            vIO32WriteFldAlign(HDMI_SEL, 1, REG_AUDIO_PATH_IN_MAVIS_SEL);
            // video in
            vIO32WriteFldAlign(HDMI_SEL, 1, REG_VIDEO_PATH_IN_RACHEL_SEL);
            vIO32WriteFldAlign(HDMI_SEL, 1, REG_VIDEO_PATH_IN_MAVIS_SEL);
            break;

        case HDMI_SWITCH_3:
            // int
            vIO32WriteFldAlign(HDMI_SEL, 0, REG_INT_OUT_RACHEL_SEL);
            vIO32WriteFldAlign(HDMI_SEL, 1, REG_INT_OUT_MAVIS_SEL);
            // acr
            vIO32WriteFldAlign(HDMI_SEL, 0, REG_ACR_OUT_RACHEL_SEL);
            vIO32WriteFldAlign(HDMI_SEL, 1, REG_ACR_OUT_MAVIS_SEL);
            // macro
            vIO32WriteFldAlign(HDMI_SEL, 0, REG_REGISTER_OUT_RACHEL_SEL);
            vIO32WriteFldAlign(HDMI_SEL, 1, REG_REGISTER_OUT_MAVIS_SEL);
            // aout
            vIO32WriteFldAlign(HDMI_SEL, 0, REG_AUDIO_PATH_OUT_RACHEL_SEL);
            vIO32WriteFldAlign(HDMI_SEL, 1, REG_AUDIO_PATH_OUT_MAVIS_SEL);
            // video out
            vIO32WriteFldAlign(HDMI_SEL, 0, REG_VIDEO_PATH_OUT_RACHEL_SEL);
            vIO32WriteFldAlign(HDMI_SEL, 1, REG_VIDEO_PATH_OUT_MAVIS_SEL);
            // acr in
            vIO32WriteFldAlign(HDMI_SEL, 0, REG_ACR_IN_RACHEL_SEL);
            vIO32WriteFldAlign(HDMI_SEL, 1, REG_ACR_IN_MAVIS_SEL);
            // clk
            vIO32WriteFldAlign(HDMI_SEL, 0, REG_CLK_IN_RACHEL_SEL);
            vIO32WriteFldAlign(HDMI_SEL, 1, REG_CLK_IN_MAVIS_SEL);
            // audio in
            vIO32WriteFldAlign(HDMI_SEL, 0, REG_AUDIO_PATH_IN_RACHEL_SEL);
            vIO32WriteFldAlign(HDMI_SEL, 1, REG_AUDIO_PATH_IN_MAVIS_SEL);
            // video in
            vIO32WriteFldAlign(HDMI_SEL, 0, REG_VIDEO_PATH_IN_RACHEL_SEL);
            vIO32WriteFldAlign(HDMI_SEL, 1, REG_VIDEO_PATH_IN_MAVIS_SEL);
            break;

        case HDMI_SWITCH_4:
            // int
            vIO32WriteFldAlign(HDMI_SEL, 0, REG_INT_OUT_RACHEL_SEL);
            vIO32WriteFldAlign(HDMI_SEL, 0, REG_INT_OUT_MAVIS_SEL);
            // acr
            vIO32WriteFldAlign(HDMI_SEL, 0, REG_ACR_OUT_RACHEL_SEL);
            vIO32WriteFldAlign(HDMI_SEL, 0, REG_ACR_OUT_MAVIS_SEL);
            // macro
            vIO32WriteFldAlign(HDMI_SEL, 0, REG_REGISTER_OUT_RACHEL_SEL);
            vIO32WriteFldAlign(HDMI_SEL, 0, REG_REGISTER_OUT_MAVIS_SEL);
            // aout
            vIO32WriteFldAlign(HDMI_SEL, 0, REG_AUDIO_PATH_OUT_RACHEL_SEL);
            vIO32WriteFldAlign(HDMI_SEL, 0, REG_AUDIO_PATH_OUT_MAVIS_SEL);
            // video out
            vIO32WriteFldAlign(HDMI_SEL, 0, REG_VIDEO_PATH_OUT_RACHEL_SEL);
            vIO32WriteFldAlign(HDMI_SEL, 0, REG_VIDEO_PATH_OUT_MAVIS_SEL);
            // acr in
            vIO32WriteFldAlign(HDMI_SEL, 0, REG_ACR_IN_RACHEL_SEL);
            vIO32WriteFldAlign(HDMI_SEL, 0, REG_ACR_IN_MAVIS_SEL);
            // clk
            vIO32WriteFldAlign(HDMI_SEL, 0, REG_CLK_IN_RACHEL_SEL);
            vIO32WriteFldAlign(HDMI_SEL, 0, REG_CLK_IN_MAVIS_SEL);
            // audio in
            vIO32WriteFldAlign(HDMI_SEL, 0, REG_AUDIO_PATH_IN_RACHEL_SEL);
            vIO32WriteFldAlign(HDMI_SEL, 0, REG_AUDIO_PATH_IN_MAVIS_SEL);
            // video in
            vIO32WriteFldAlign(HDMI_SEL, 0, REG_VIDEO_PATH_IN_RACHEL_SEL);
            vIO32WriteFldAlign(HDMI_SEL, 0, REG_VIDEO_PATH_IN_MAVIS_SEL);
            break;
    };
}

UINT8 u1IsHdcpGood(E_HDMI_SWITCH_NUM eSrc)
{
    UINT32 u4RegBase = 0;
    UINT8 u1Ret = 0;
    u4RegBase = u4GetHdmiRegBase(eSrc);

    if(IO32ReadFldAlign(HDCP_STAT + u4RegBase, HDCP_DECRYPT))
    {
        u1Ret = 1;
    }

    return (u1Ret);
}

UINT8 u1IsResStable(E_HDMI_SWITCH_NUM eSrc)
{
    UINT32 u4RegBase = 0;
    UINT8 u1Ret = 0;
    u4RegBase = u4GetHdmiRegBase(eSrc);

    if(IO32ReadFldAlign(VID_HRES + u4RegBase, VID_HRES_STB) && IO32ReadFldAlign(VID_VRES + u4RegBase, VID_VRES_STB))
    {
        u1Ret = 1;
    }

    return (u1Ret);
}

#elif  defined(CC_MT5389)

void vHDMISel(E_HDMI_SWITCH_NUM eSel)
{
    u4ActiveHdmiRegBase = u4GetHdmiRegBase(eSel);
    u4ActiveAnalogRegBase = u4GetAnalogRegBase(eSel);

    switch(eSel)
    {
        default:
        case HDMI_SWITCH_1:
            _arHdmiRx[eSel]._bRxShared = 0;
            _arHdmiRx[HDMI_SWITCH_2]._bRxShared = 1;
            vIO32WriteFldAlign(REG_HDMI_RX_Normal_0_CFG2 + 0x100, 5, RG_HDMI_0_SEL);
            vIO32WriteFldAlign(PDWNC_PDMISC, 0, FLD_HDMI_SEL);
            break;

        case HDMI_SWITCH_2:
            _arHdmiRx[eSel]._bRxShared = 0;
            _arHdmiRx[HDMI_SWITCH_1]._bRxShared = 1;
            vIO32WriteFldAlign(REG_HDMI_RX_Normal_0_CFG2 + 0x100, 3, RG_HDMI_0_SEL);
            vIO32WriteFldAlign(PDWNC_PDMISC, 1, FLD_HDMI_SEL);
            break;

        case HDMI_SWITCH_3:
            _arHdmiRx[eSel]._bRxShared = 0;
            break;
    }

    u4HdmiRegBase = u4ActiveHdmiRegBase;
    u4AnalogRegBase = u4ActiveAnalogRegBase;

    switch(eSel)
    {
        default:
        case HDMI_SWITCH_1:
            // int
            vIO32WriteFldAlign(HDMI_SEL, 1, REG_INT_OUT_RACHEL_SEL);
            // acr out
            vIO32WriteFldAlign(HDMI_SEL, 1, REG_ACR_OUT_RACHEL_SEL);
            // macro
            vIO32WriteFldAlign(HDMI_SEL, 1, REG_REGISTER_OUT_RACHEL_SEL);
            // aout
            vIO32WriteFldAlign(HDMI_SEL, 1, REG_AUDIO_PATH_OUT_RACHEL_SEL);
            // video out
            vIO32WriteFldAlign(HDMI_SEL, 1, REG_VIDEO_PATH_OUT_RACHEL_SEL);
            // acr in
            vIO32WriteFldAlign(HDMI_SEL, 1, REG_ACR_IN_RACHEL_SEL);
            // clk
            vIO32WriteFldAlign(HDMI_SEL, 1, REG_CLK_IN_RACHEL_SEL);
            // audio in
            vIO32WriteFldAlign(HDMI_SEL, 1, REG_AUDIO_PATH_IN_RACHEL_SEL);
            // video in
            vIO32WriteFldAlign(HDMI_SEL, 1, REG_VIDEO_PATH_IN_RACHEL_SEL);
            break;

        case HDMI_SWITCH_2:
            // int
            vIO32WriteFldAlign(HDMI_SEL, 1, REG_INT_OUT_RACHEL_SEL);
            // acr
            vIO32WriteFldAlign(HDMI_SEL, 1, REG_ACR_OUT_RACHEL_SEL);
            // macro
            vIO32WriteFldAlign(HDMI_SEL, 1, REG_REGISTER_OUT_RACHEL_SEL);
            // aout
            vIO32WriteFldAlign(HDMI_SEL, 1, REG_AUDIO_PATH_OUT_RACHEL_SEL);
            // video out
            vIO32WriteFldAlign(HDMI_SEL, 1, REG_VIDEO_PATH_OUT_RACHEL_SEL);
            // acr in
            vIO32WriteFldAlign(HDMI_SEL, 1, REG_ACR_IN_RACHEL_SEL);
            // clk
            vIO32WriteFldAlign(HDMI_SEL, 1, REG_CLK_IN_RACHEL_SEL);
            // audio in
            vIO32WriteFldAlign(HDMI_SEL, 1, REG_AUDIO_PATH_IN_RACHEL_SEL);
            // video in
            vIO32WriteFldAlign(HDMI_SEL, 1, REG_VIDEO_PATH_IN_RACHEL_SEL);
            break;

        case HDMI_SWITCH_3:
            // int
            vIO32WriteFldAlign(HDMI_SEL, 0, REG_INT_OUT_RACHEL_SEL);
            // acr
            vIO32WriteFldAlign(HDMI_SEL, 0, REG_ACR_OUT_RACHEL_SEL);
            // macro
            vIO32WriteFldAlign(HDMI_SEL, 0, REG_REGISTER_OUT_RACHEL_SEL);
            // aout
            vIO32WriteFldAlign(HDMI_SEL, 0, REG_AUDIO_PATH_OUT_RACHEL_SEL);
            // video out
            vIO32WriteFldAlign(HDMI_SEL, 0, REG_VIDEO_PATH_OUT_RACHEL_SEL);
            // acr in
            vIO32WriteFldAlign(HDMI_SEL, 0, REG_ACR_IN_RACHEL_SEL);
            // clk
            vIO32WriteFldAlign(HDMI_SEL, 0, REG_CLK_IN_RACHEL_SEL);
            // audio in
            vIO32WriteFldAlign(HDMI_SEL, 0, REG_AUDIO_PATH_IN_RACHEL_SEL);
            // video in
            vIO32WriteFldAlign(HDMI_SEL, 0, REG_VIDEO_PATH_IN_RACHEL_SEL);
            break;
    };
}


UINT8 u1IsHdcpGood(E_HDMI_SWITCH_NUM eSrc)
{
    UINT32 u4RegBase = 0;
    UINT8 u1Ret = 0;
    u4RegBase = u4GetHdmiRegBase(eSrc);

    if(IO32ReadFldAlign(HDCP_STAT + u4RegBase, HDCP_DECRYPT))
    {
        u1Ret = 1;
    }

    return (u1Ret);
}

UINT8 u1IsResStable(E_HDMI_SWITCH_NUM eSrc)
{
    UINT32 u4RegBase = 0;
    UINT8 u1Ret = 0;
    u4RegBase = u4GetHdmiRegBase(eSrc);

    if(IO32ReadFldAlign(VID_HRES + u4RegBase, VID_HRES_STB) && IO32ReadFldAlign(VID_VRES + u4RegBase, VID_VRES_STB))
    {
        u1Ret = 1;
    }

    return (u1Ret);
}
#elif defined (CC_MT5882)/*Add CC_MT5882 George.yang */
void vHDMISel(E_HDMI_SWITCH_NUM eSel)
{
    u4ActiveHdmiRegBase = u4GetHdmiRegBase(eSel);
    u4ActiveAnalogRegBase = u4GetAnalogRegBase(eSel);

    switch(eSel)
    {
        default:
        case HDMI_SWITCH_1:
            _arHdmiRx[eSel]._bRxShared = 0;
            break;

        case HDMI_SWITCH_2:
            _arHdmiRx[eSel]._bRxShared = 0;
            break;

        case HDMI_SWITCH_3:
            _arHdmiRx[eSel]._bRxShared = 0;
            break;
	}

    u4HdmiRegBase = u4ActiveHdmiRegBase;
    u4AnalogRegBase = u4ActiveAnalogRegBase;

    switch(eSel)
    {
        default:
        case HDMI_SWITCH_1:
            // main clk
            vIO32WriteFldAlign(HDMI_SEL, 1, REG_MAIN_CLK_SEL0);
			//pip clk
			 vIO32WriteFldAlign(HDMI_PIP_SEL,1,REG_PIP_CLK_SEL0);
            // int
            vIO32WriteFldAlign(HDMI_SEL, 1, REG_INT_OUT_RACHEL_SEL);
            // acr
            vIO32WriteFldAlign(HDMI_SEL, 1, REG_ACR_OUT_RACHEL_SEL);
            // macro
            vIO32WriteFldAlign(HDMI_SEL, 1, REG_REGISTER_OUT_RACHEL_SEL);
            // aout
            vIO32WriteFldAlign(HDMI_SEL, 1, REG_AUDIO_PATH_OUT_RACHEL_SEL);
            // video out
            vIO32WriteFldAlign(HDMI_SEL, 1, REG_VIDEO_PATH_OUT_RACHEL_SEL);
            // clk
            vIO32WriteFldAlign(HDMI_SEL, 1, REG_CLK_IN_RACHEL_SEL);
            // audio in
            vIO32WriteFldAlign(HDMI_SEL, 1, REG_AUDIO_PATH_IN_RACHEL_SEL);
            // video in
            vIO32WriteFldAlign(HDMI_SEL, 1, REG_VIDEO_PATH_IN_RACHEL_SEL);
			if(_bIsMhlDevice == 1)
			{
				// mhl clk
				vIO32WriteFldAlign(CKGEN_REG_MHL_CKCFG, 1, FLD_MHL_DPCLK_SEL);
				vIO32WriteFldAlign(CKGEN_REG_MHL_CKCFG, 1, FLD_MHL_PCLK_SEL);
				//sel work mode
				vIO32WriteFldAlign(MHL_REG, 1, REG_MHL_MODE_SEL);
			}
			else
			{
				// hdmi clk
				vIO32WriteFldAlign(CKGEN_REG_MHL_CKCFG, 0, FLD_MHL_DPCLK_SEL);
				vIO32WriteFldAlign(CKGEN_REG_MHL_CKCFG, 0, FLD_MHL_PCLK_SEL);
				//sel work mode
				vIO32WriteFldAlign(MHL_REG, 0, REG_MHL_MODE_SEL);
			}
            break;

        case HDMI_SWITCH_2:
            // main clk
            vIO32WriteFldAlign(HDMI_SEL, 0, REG_MAIN_CLK_SEL0);
            vIO32WriteFldAlign(HDMI_SEL, 1, REG_MAIN_CLK_SEL1);
			//pip clk
			vIO32WriteFldAlign(HDMI_PIP_SEL,0,REG_PIP_CLK_SEL0);
			vIO32WriteFldAlign(HDMI_PIP_SEL,1,REG_PIP_CLK_SEL1);
            // int
            vIO32WriteFldAlign(HDMI_SEL, 0, REG_INT_OUT_RACHEL_SEL);
            vIO32WriteFldAlign(HDMI_SEL, 1, REG_INT_OUT_MAVIS_SEL1);
            // acr
            vIO32WriteFldAlign(HDMI_SEL, 0, REG_ACR_OUT_RACHEL_SEL);
            vIO32WriteFldAlign(HDMI_SEL, 1, REG_ACR_OUT_MAVIS_SEL1);
            // macro
            vIO32WriteFldAlign(HDMI_SEL, 0, REG_REGISTER_OUT_RACHEL_SEL);
            vIO32WriteFldAlign(HDMI_SEL, 1, REG_REGISTER_OUT_MAVIS_SEL1);
            // aout
            vIO32WriteFldAlign(HDMI_SEL, 0, REG_AUDIO_PATH_OUT_RACHEL_SEL);
            vIO32WriteFldAlign(HDMI_SEL, 1, REG_AUDIO_PATH_OUT_MAVIS_SEL1);
            // video out
            vIO32WriteFldAlign(HDMI_SEL, 0, REG_VIDEO_PATH_OUT_RACHEL_SEL);
            vIO32WriteFldAlign(HDMI_SEL, 1, REG_VIDEO_PATH_OUT_MAVIS_SEL1);
            // clk
            vIO32WriteFldAlign(HDMI_SEL, 0, REG_CLK_IN_RACHEL_SEL);
            vIO32WriteFldAlign(HDMI_SEL, 1, REG_CLK_IN_MAVIS_SEL1);
            // audio in
            vIO32WriteFldAlign(HDMI_SEL, 0, REG_AUDIO_PATH_IN_RACHEL_SEL);
            vIO32WriteFldAlign(HDMI_SEL, 1, REG_AUDIO_PATH_IN_MAVIS_SEL1);
            // video in
            vIO32WriteFldAlign(HDMI_SEL, 0, REG_VIDEO_PATH_IN_RACHEL_SEL);
            vIO32WriteFldAlign(HDMI_SEL, 1, REG_VIDEO_PATH_IN_MAVIS_SEL1);
			// hdmi clk
			vIO32WriteFldAlign(CKGEN_REG_MHL_CKCFG, 0, FLD_MHL_DPCLK_SEL);
			vIO32WriteFldAlign(CKGEN_REG_MHL_CKCFG, 0, FLD_MHL_PCLK_SEL);
            break;

        case HDMI_SWITCH_3:
            // main clk
            vIO32WriteFldAlign(HDMI_SEL, 0, REG_MAIN_CLK_SEL0);
            vIO32WriteFldAlign(HDMI_SEL, 0, REG_MAIN_CLK_SEL1);
            vIO32WriteFldAlign(HDMI_SEL, 1, REG_MAIN_CLK_SEL2);
			//pip clk
			vIO32WriteFldAlign(HDMI_PIP_SEL,0,REG_PIP_CLK_SEL0);
			vIO32WriteFldAlign(HDMI_PIP_SEL,0,REG_PIP_CLK_SEL1);
			vIO32WriteFldAlign(HDMI_PIP_SEL,1,REG_PIP_CLK_SEL2);
            // int
            vIO32WriteFldAlign(HDMI_SEL, 0, REG_INT_OUT_RACHEL_SEL);
            vIO32WriteFldAlign(HDMI_SEL, 0, REG_INT_OUT_RACHEL_SEL);
            vIO32WriteFldAlign(HDMI_SEL, 1, REG_INT_OUT_MAVIS_SEL2);
            // acr
            vIO32WriteFldAlign(HDMI_SEL, 0, REG_ACR_OUT_RACHEL_SEL);
            vIO32WriteFldAlign(HDMI_SEL, 0, REG_ACR_OUT_MAVIS_SEL1);
            vIO32WriteFldAlign(HDMI_SEL, 1, REG_ACR_OUT_MAVIS_SEL2);
            // macro
            vIO32WriteFldAlign(HDMI_SEL, 0, REG_REGISTER_OUT_RACHEL_SEL);
            vIO32WriteFldAlign(HDMI_SEL, 0, REG_REGISTER_OUT_MAVIS_SEL1);
            vIO32WriteFldAlign(HDMI_SEL, 1, REG_REGISTER_OUT_MAVIS_SEL2);
            // aout
            vIO32WriteFldAlign(HDMI_SEL, 0, REG_AUDIO_PATH_OUT_RACHEL_SEL);
            vIO32WriteFldAlign(HDMI_SEL, 0, REG_AUDIO_PATH_OUT_MAVIS_SEL1);
            vIO32WriteFldAlign(HDMI_SEL, 1, REG_AUDIO_PATH_OUT_MAVIS_SEL2);
            // video out
            vIO32WriteFldAlign(HDMI_SEL, 0, REG_VIDEO_PATH_OUT_RACHEL_SEL);
            vIO32WriteFldAlign(HDMI_SEL, 0, REG_VIDEO_PATH_OUT_MAVIS_SEL1);
            vIO32WriteFldAlign(HDMI_SEL, 1, REG_VIDEO_PATH_OUT_MAVIS_SEL2);
            // clk
            vIO32WriteFldAlign(HDMI_SEL, 0, REG_CLK_IN_RACHEL_SEL);
            vIO32WriteFldAlign(HDMI_SEL, 0, REG_CLK_IN_MAVIS_SEL1);
            vIO32WriteFldAlign(HDMI_SEL, 1, REG_CLK_IN_MAVIS_SEL2);
            // audio in
            vIO32WriteFldAlign(HDMI_SEL, 0, REG_AUDIO_PATH_IN_RACHEL_SEL);
            vIO32WriteFldAlign(HDMI_SEL, 0, REG_AUDIO_PATH_IN_MAVIS_SEL1);
            vIO32WriteFldAlign(HDMI_SEL, 1, REG_AUDIO_PATH_IN_MAVIS_SEL2);
            // video in
            vIO32WriteFldAlign(HDMI_SEL, 0, REG_VIDEO_PATH_IN_RACHEL_SEL);
            vIO32WriteFldAlign(HDMI_SEL, 0, REG_VIDEO_PATH_IN_MAVIS_SEL1);
            vIO32WriteFldAlign(HDMI_SEL, 1, REG_VIDEO_PATH_IN_MAVIS_SEL2);
			// hdmi clk
			vIO32WriteFldAlign(CKGEN_REG_MHL_CKCFG, 0, FLD_MHL_DPCLK_SEL);
			vIO32WriteFldAlign(CKGEN_REG_MHL_CKCFG, 0, FLD_MHL_PCLK_SEL);
            break;
    }
}

UINT8 u1IsHdcpGood(E_HDMI_SWITCH_NUM eSrc)
{
    UINT32 u4RegBase = 0;
    UINT8 u1Ret = 0;
    u4RegBase = u4GetHdmiRegBase(eSrc);

    if(IO32ReadFldAlign(HDCP_STAT + u4RegBase, HDCP_DECRYPT))
    {
        u1Ret = 1;
    }

    return (u1Ret);
}

UINT8 u1IsResStable(E_HDMI_SWITCH_NUM eSrc)
{
    UINT32 u4RegBase = 0;
    UINT8 u1Ret = 0;
    u4RegBase = u4GetHdmiRegBase(eSrc);

    if(IO32ReadFldAlign(VID_HRES + u4RegBase, VID_HRES_STB) && IO32ReadFldAlign(VID_VRES + u4RegBase, VID_VRES_STB))
    {
        u1Ret = 1;
    }

    return (u1Ret);
}

#endif

void u1HDMI_MHL_Term_Value_Cali(void)
{
    E_HDMI_SWITCH_NUM eHdmiPort = HDMI_SWITCH_1;
    UINT32 u4RegBase = 0;
	UINT8 u1term = 0;
	
	//u1term = (UINT8)((u4IO32Read4B(0xf0008690)>>14) & 0x1f); //0xf0008690[18:14]
	u1term =  (UINT8)(u1IO32Read1B(0xf0008690)&0x1f); //0xf0008690[4:0]
	if(u1term == 0)
	{
		//calibrate data by fw
			u1term = 0x12;
		}
		else
		{
			u1term += 5;
		if(u1term >= 0x1f)
		{
			u1term = 0x1f;
		}
	}
	vIO32WriteFldAlign(MHL_REG, 1, REG_MHL_IMPCAL_EN);
	vIO32WriteFldAlign(MHL_REG, 1, REG_MHL_IMPCAL_REGEN);
	vIO32WriteFldAlign(MHL_REG, 1, REG_MHL_CALIB_SEL);
	//for port0,MHL and HDMI share the same register address
	vIO32WriteFldAlign(MHL_REG, u1term, REG_MHL_CALIB_DATA);
	
    for(eHdmiPort = HDMI_SWITCH_2; eHdmiPort < HDMI_SWITCH_MAX; eHdmiPort++)
    {
        u4RegBase = u4GetHdmiRegBase(eHdmiPort);
		vIO32WriteFldAlign(CALIBRATION_DATA + u4RegBase, 1, DA_HDMI_0_IMP_CALEN);
		vIO32WriteFldAlign(CALIBRATION_DATA + u4RegBase, u1term, DA_HDMI_0_IMPSEL);
    }
}
/**
 * @brief 	HDMI Initial function
 * @param	None
 * @retval	None
 */
void vHDMIInit(void)
{

    E_HDMI_SWITCH_NUM eHdmiPort = HDMI_SWITCH_1;
    UINT32 u4RegBase = 0;
    UINT32 u4AnaRegBase = 0;
    // 1. HDMI Chip Availability testing
#ifdef CC_SUPPORT_HDMI_FBOOTING

    if(u1HdmiInit == 1)
    {
        Printf("hdmi re-init\n");
        return;
    }

#endif

    ///////////////////////////////////////////////////////
    // 2. Software Initial

    for(eHdmiPort = HDMI_SWITCH_1 ; eHdmiPort < HDMI_SWITCH_MAX; eHdmiPort++)
    {
        _arHdmiRx[eHdmiPort]._bHDMIState = HDMI_STATE_INIT;
        _arHdmiRx[eHdmiPort]._bHdmiCmd = 0;
        _arHdmiRx[eHdmiPort]._bUnplugFlag = 0;
        _arHdmiRx[eHdmiPort]._bCKDTMonitor = 0;
        _arHdmiRx[eHdmiPort]._u1ClkChg = 0;
        _arHdmiRx[eHdmiPort]._bHDMIFastSW = 0;
        _arHdmiRx[eHdmiPort]._bResStableCnt = 0;
        _arHdmiRx[eHdmiPort]._u1Tx5VActive = 0;
        _arHdmiRx[eHdmiPort]._bSyncLose=0;
        _arHdmiRx[eHdmiPort]._bTxSTBY = 0;
		_arHdmiRx[eHdmiPort]._bNonStdDev = 0;
		_arHdmiRx[eHdmiPort]._bTmdsDetCnt = 0;

	#ifdef CC_SUPPORT_HDMI_CLK5VDET
		 _arHdmiRx[eHdmiPort]._bNonStdDev = 0;
    	 _arHdmiRx[eHdmiPort]._b5VActive = 0;
    	 _arHdmiRx[eHdmiPort]._b5VDetCnt = 0;
    	 _arHdmiRx[eHdmiPort]._bTmdsActive = 0;
    	 _arHdmiRx[eHdmiPort]._bTmdsDetCnt = 0;
    	 _arHdmiRx[eHdmiPort]._bHpdPulse = 0;
	#endif
    }

    _bEQTuning = 0;
    _bHPD_Indep_Ctrl = 0;
    _bHdmiFlag = 0;
    _bHdmiCnt = 0;
    _bHdmiAudFs = 0xf;
    _bHdmiMode = 0;
    _bHDMISampleChange = 0;
    _bHDMIAudioInit = 0;
    _bHdmiMD = 0;
    _bHDMIScanInfo = bHDMIScanInfo();
    _bHDMIAspectRatio = bHDMIAspectRatio();
    _bHDMIAFD = bHDMIAFD();
    _bHDMIHDCPStatus = bHDMIHDCPStatusGet();
    _bHDMI422Input = 0;
    _bHDMIITCFlag = 0;
    _bHDMIITCContent = 0;
    _bIntr_CK_CHG = 0;
	_bHDMIColorFormat =0;
    _bNEW_AVI_Info = 0;
    _bACPCount = 0;
    _bNodifyConnect = 0;
    //_bUnplugFlag = 0;
    _bUnplugCount = 0;
    _bHDMIColorSpace = 0;
    _bFixedEQReset = 0;
	u1UPStaNew = HDMI_SPD_NONE_STATUS;
	u1UPStaOld = HDMI_SPD_NONE_STATUS;
	u1SVStaNew = HDMI_SPD_NONE_STATUS;
	u1SVStaOld = HDMI_SPD_NONE_STATUS;
	u1HdmiSpdUPStatChk = HDMI_SPD_NONE_CHG;
	u1HdmiSpdSVStatChk = HDMI_SPD_NONE_CHG;
	u1IsSPDFlag = 0;
	u1SpdTest = 0;
	x_memset(&u4SpdData,0,sizeof(HDMI_SPD_DATA_STRUCT));
#ifdef CC_Support_Sil9285
    _bHDMI9287switchstopflag = 0;
#endif
#ifdef CC_BYPASS_AVI_INFOFRAME
    _bDVIAVIinfoBypass = bApiEepromReadByte(EEP_HDMI_AVIINFO_BYPASS); //wait EEP_HDMI_AVIINFO_BYPASS implement
#endif
#if HDMI_Audio_NewFlow
    _bHDMIAudFIFOflag = 0;
#endif

    _bHdmiVsdbActive = 0;

    _fgForce3D = FALSE;
    _bForce3DStructure = 0;  // FP
    _bForce3DExtData = 0;
    _fgForce2D = FALSE;

#ifdef CC_SUPPORT_HDMI_CLK5VDET
    _bNonStdDev = 0;
    _b5VActive = 0;
    _b5VDetCnt = 0;
    _bTmdsActive = 0;
    _bTmdsDetCnt = 0;
    _bHpdPulse = 0;
#endif
    u4PreXpcCnt = 0;
    u4CurXpcCnt = 0;
    _bXpcStableCnt = 0;
    _bHdmiAudFreq = AUD_FS_48K;  // 48 k
    eActiveHdmiPort = HDMI_SWITCH_INIT;

    u4XtalfreqCnt =  ((IS_XTAL_24MHZ()) ? FREQCOUNTS_24MHZ : FREQCOUNTS_27MHZ);

#if (TMDS_EQ_CUSTOM == 1)
    UNUSED(DRVCUST_OptQuery(eHDMIEQZEROVALUE, &_wHDMI_EQ_ZERO_VALUE));
    UNUSED(DRVCUST_OptQuery(eHDMIEQBOOSTVALUE, &_wHDMI_EQ_BOOST_VALUE));
    UNUSED(DRVCUST_OptQuery(eHDMIEQSELVALUE, &_wHDMI_EQ_SEL_VALUE));
    UNUSED(DRVCUST_OptQuery(eHDMIEQGAINVALUE, &_wHDMI_EQ_GAIN_VALUE));
    UNUSED(DRVCUST_OptQuery(eHDMILBWVALUE, &_wHDMI_LBW_VALUE));
#endif
    UNUSED(DRVCUST_OptQuery(eHDMIHDCPMASK1, &_wHDMI_HDCP_MASk1));
    UNUSED(DRVCUST_OptQuery(eHDMIHDCPMASK2, &_wHDMI_HDCP_MASk2));

    for(eHdmiPort = HDMI_SWITCH_1; eHdmiPort < HDMI_SWITCH_MAX; eHdmiPort++)
    {
        u4RegBase = u4GetHdmiRegBase(eHdmiPort);
        u4AnaRegBase = u4GetAnalogRegBase(eHdmiPort);
        ///////////////////////////////////////////////////////
        // 3. HW Initial
        /* SYS_CTRL1
           bit 0 power down mode
           bit 1 output clock invert
           bit 2 video bus mode
           bit 3 pixel bus mode
           bit 5:4 input pixel clock peplication
           bit 7:6 output pixel clock dvider
         */
        vIO32Write1B(SYS_CTRL_0 + u4RegBase, 0x07);
        vIO32Write4BMsk(SYS_CTRL + u4RegBase, Fld2Msk32(RX_EN), Fld2Msk32(RX_EN));
        /* power on all block */
        vIO32Write1B(PD_SYS_0 + u4RegBase, 0x01);
        vIO32Write1B(PD_SYS_2 + u4RegBase, 0xCf);
        //vIO32Write1B(PD_SYS_3,0x3f);  // except Tri-state Audio/Video output
        vIO32Write1B(PD_SYS_3 + u4RegBase, 0xff); // except Tri-state Audio/Video output
        vIO32Write1B(PD_SYS_3 + u4RegBase, 0xf3);	//solve analog PHY bandgap start-up
        vIO32Write1B(PD_SYS_3 + u4RegBase, 0xff); //solve analog PHY bandgap start-up
        /*analog setting*/
        vIO32Write4B(REG_HDMI_RX_Normal_0_CFG0 + u4AnaRegBase, 0x80000000);
        vIO32Write4B(REG_HDMI_RX_Normal_0_CFG1 + u4AnaRegBase, 0xD5A00D4A);
        vIO32Write4B(REG_HDMI_RX_Normal_0_CFG2 + u4AnaRegBase, 0x33215022);
        vIO32Write4B(REG_HDMI_RX_Normal_0_CFG3 + u4AnaRegBase, 0x08212310);
        vIO32Write4B(REG_HDMI_RX_Normal_0_CFG4 + u4AnaRegBase, 0x0120F000);
        vIO32Write4B(REG_HDMI_RX_Normal_0_CFG5 + u4AnaRegBase, 0x00000000);
        vUtDelay2us(1);
        vIO32Write4B(REG_HDMI_RX_Normal_0_CFG0 + u4AnaRegBase, 0x00000000);
        vUtDelay2us(2);
        vIO32Write4B(REG_HDMI_RX_Normal_0_CFG1 + u4AnaRegBase, 0xD5A0094A);
        vIO32Write4B(REG_HDMI_RX_Normal_0_CFG2 + u4AnaRegBase, 0x33215023);
		vIO32Write4B(REG_HDMI_RX_Normal_0_CFG1 + u4AnaRegBase, 0xD520094A);
		u1HDMIEQOffsetCali(eHdmiPort);
        // pll auto reset
        vIO32WriteFldAlign(REG_HDMI_RX_Normal_0_CFG4 + u4AnaRegBase, 1, RG_HDMI_0_PLL_DETEN);
        vHDMIPhyBandCtrl(eHdmiPort, TMDS_CLOCK_HIGH);
        /*TMDS setting*/
        vIO32Write4B(TMDS_CTRL0 + u4RegBase, 0x00000000);//reset TMDS PHY digital part
        vIO32Write4B(TMDS_CTRL0 + u4RegBase, 0x07000002);//invert CH 1,2,3
        vIO32WriteFldAlign(TMDS_CTRL0 + u4RegBase, 1, C_DATA_SYNC_AUTO);
        /* ACR_CTRL3
           bit 1 enable for simplified window for pll_unlock
           bit 2 MCLK Loop back.
           bit 6:3 Threshold for CTS Change
         */
        vIO32Write1B(ACR_CTRL3_0 + u4RegBase, 0x5C);
#if defined(CC_MT5389) || defined(CC_MT5398) || defined(CC_MT5880) || defined(CC_MT5399)  || defined(CC_MT5882)//CC_AUD_PLL_HALF
        vIO32WriteFldAlign(ACR_CTRL3_0 + u4RegBase, 1, POST_DIV2_ENA);
#else
        vIO32WriteFldAlign(ACR_CTRL3_0 + u4RegBase, 0, POST_DIV2_ENA);
#endif

#if defined(CC_MT5398)
		if(IS_XTAL_24MHZ())
		{
			vIO32WriteFldAlign(TMDS_CTRL_STATUS1 + u4RegBase, 0x7, HDMI_CH0_24M);
		}
#endif
        /*LKWIN_SVAL
          bit 4:1 most recent count must be within this margin to
          enable the stability counter to increment
         */
        //vIO32Write4B(UPLL_HVAL,0x1F060030);
        /*LKTHRESH
          PLL Lock Stability Threshold
         */
        vIO32Write1B(LK_THRS_SVAL_0 + u4RegBase, 0xff);
        vIO32Write1B(LK_THRS_SVAL_1 + u4RegBase, 0xff);
        /* I2S CTRL1
           bit 0 WS to SD shift First bit
           bit 1 SD data direction
           bit 2 SD Justification
           bit 3 UINT16 Select Left/right Polarity
           bit 4 MSB sign-extended
           bit 5 UINT16 size
           bit 6 sample clock edge
           bit 7 send invalid data enable
         */
        vIO32Write1B(I2S_CTRL_2 + u4RegBase, 0x49);
        /*HDMI_CRIT1
          Preamble Criteria: selects the required number of repetitions
          for a valid preamble
         */
        vIO32WriteFldAlign(AUDP_STAT + u4RegBase, 0x6, PREAMBLE_CRI);
        //vIO32WriteFldAlign(AUDP_STAT,0x0,PREAMBLE_CRI);
        /*HDMI_CRIT2
          Preamble Criteria: selects the required number of repetitions
          for a valid preamble
         */
        vIO32Write1B(TEST_CTRL_0 + u4RegBase, 0xC);
        /*ACR_CFG
          ACR PLL configuration
         */
        vIO32Write4B(APLL0 + u4RegBase, 0x21003680);
        /* int output polarity# */
        vIO32Write1B(INTR_STATE0_0 + u4RegBase, 0x2);//	vIO32Write1B(INTR_MASK1_1,0x2);
        /* set key mask to zero */
        vIO32Write1B(EPST_3 + u4RegBase, 0x00);
        vIO32Write1B(KS_MASK_0 + u4RegBase, 0x00);
        //Enable Audio SPDIF Output
        vIO32Write1B(AUDRX_CTRL_1 + u4RegBase, 0x1d);
        // KSCL_H
        vIO32Write4BMsk(VID_SET + u4RegBase, Fld2Msk32(KSCL_H), Fld2Msk32(KSCL_H));
        // HDCP Keymask
        vIO32Write1B(EPST_3 + u4RegBase, _wHDMI_HDCP_MASk1);		//vIO32Write1B(EPST_3,0xff);
        vIO32Write1B(KS_MASK_0 + u4RegBase, _wHDMI_HDCP_MASk2);	//vIO32Write1B(KS_MASK_0,0xc3);
        // setting to avoid TMDS reset -> vHDMITMDSReset
        vIO32Write4BMsk(AUDP_STAT + u4RegBase, Fld2Msk32(BYP_SYNC) | Fld2Msk32(BYP_DVIFILT) , Fld2Msk32(BYP_SYNC) | Fld2Msk32(BYP_DVIFILT));
        // Clear Audio mute
        vIO32Write4B(AUDP_STAT + u4RegBase, (u4IO32Read4B(AUDP_STAT + u4RegBase) & (~MT53XX_AUD_MUTE)));
        // increase the PCCNT resolution  from 128 to 1024 by adam
        vIO32Write4BMsk(VID_CRC_OUT + u4RegBase, Fld2Msk32(XCLK_IN_PCLK_SEL), Fld2Msk32(XCLK_IN_PCLK_SEL));
        //decode di_DE and vi_DE by both preamble and guard-band
        vIO32Write4BMsk(MUTE_SET + u4RegBase, 0x0, Fld2Msk32(BYPASS_DI_GB));
        //modify 1080p twinkling dot
        //CKGEN_WRITE32(REG_RGBCLK_CFG, CKGEN_READ32(REG_RGBCLK_CFG) |RGB_INV_SEL );
        vIO32WriteFldAlign(CKGEN_RGBCLK_CFG, 1, FLD_RGB_INV_SEL);
        //deep color mode must refer output video clock not TMDS clock
        vIO32WriteFldAlign(VID_CRC_OUT + u4RegBase, 0x1, XCLK_IN_DPCLK);
#ifdef JOSH
        vIO32Write4B(DIGITAL_CKDT, 0x102b1b38);
#endif
        //enable  HW GAMUT packet decoder
        vIO32WriteFldAlign(N_HDMI_CTRL + u4RegBase, 0x1, TT0_GAMUT_EN);
#if Enable_TDFIFO_RESET
        vIO32WriteFldAlign(MUTE_SET + u4RegBase, 0x1, TDFIFO_SYNC_EN);
#endif
#if Enable_HW_Mute
        vIO32WriteFldAlign(VID_VRES + u4RegBase, 0x1, VRES_MUTE_AUTO_CLR);
        /*disable external 27MHZ clk input when no signal, XBOX issue for plug/unplug */
        vIO32WriteFldAlign(CKGEN_MISC_CKCFG, 0x0, FLD_HDMI_CLK_Crystral);
        vIO32WriteFldAlign(VID_HRES + u4RegBase, 0x3, HCHG_CNT_THR);
        vIO32WriteFldAlign(VID_HRES + u4RegBase, 0xf, HSTB_CNT_THR);
        vIO32WriteFldAlign(VID_VRES + u4RegBase, 0x3, VSTB_CNT_THR);
        vIO32WriteFldAlign(VID_SET + u4RegBase, 0x1, MUTE_CKDT);
        vIO32WriteFldAlign(VID_HRES + u4RegBase, 0x4, HRES_THR); //for Iris project sony DSC T500 and Foxconn project  Setop Box modelGScienfitic Atlanta 8300HDC flash use
        vIO32WriteFldAlign(VID_VRES + u4RegBase, 0x2, VCHG_CNT_THR);//for TG 35 ,when change Vtotal in 1080i and 480i, HW auto mute enable and doesn't unmute
#endif
        //vIO32WriteFldAlign(HDCP_DEV,0x1,HDCP_SRAM_SW_RST);
        //vIO32WriteFldAlign(HDCP_DEV,0x0,HDCP_SRAM_SW_RST);
#ifdef CC_SUPPORT_STR
				if(_wHDMI_OFFON_MUTE_COUNT == 0)
				{
					UNUSED(DRVCUST_OptQuery(eHDMIOffOnMuteCount, &_wHDMI_OFFON_MUTE_COUNT));
					//LOG(0,":::::::::::::::::::::::::::::::::::::::::::::::::\n\n");
				}
#else
				UNUSED(DRVCUST_OptQuery(eHDMIOffOnMuteCount, &_wHDMI_OFFON_MUTE_COUNT));
#endif
        UNUSED(DRVCUST_OptQuery(eDVIWaitStableCount, &_wDVI_WAIT_STABLE_COUNT));
        UNUSED(DRVCUST_OptQuery(eHDMIBypassInitialFlow, &_wHDMIBypassFlag));
        UNUSED(DRVCUST_OptQuery(eDVIWaitNosignalCount, &_wDVI_WAIT_NOSIGNAL_COUNT));
        UNUSED(DRVCUST_OptQuery(eDVIWaitSCDTStableCount, &_wHDMI_WAIT_SCDT_STABLE_COUNT));
        vIO32WriteFldAlign(INTR_MASK0 + u4RegBase, 0x1, NEW_ACP_ONLY);
#if ReHPD
        _bCKDTcnt = 0;
#endif

        //vHDMI_INTERNAL_EDID_Init();
        vIO32WriteFldAlign(VID_HRES + u4RegBase, 0x1, VRES_CHG_DET_SEL);//detect both progressive and interlace input v resolution change
        vIO32WriteFldAlign(VID_HRES + u4RegBase, 0x1, INTERLACE_DET_SEL);//If this bit set to 1, Interlace out status is assert when the fallowing condition meets:1. 0 < line difference between 2 field < INTERLACE_LINE_DIFF_TH
        vIO32WriteFldAlign(VID_STAT + u4RegBase, 0x2, INTERLACE_LINE_DIFF_TH);//for INTERLACE_DET_SEL
        /*enable I2C deglitch*/
        vIO32WriteFldAlign(PDWNC_EDID_DEV0, 1, FLD_DEGLITCH0_EN);
        vIO32WriteFldAlign(PDWNC_EDID_DEV0, 3, FLD_DE_CNT0);
        vIO32WriteFldAlign(PDWNC_EDID_DEV1, 1, FLD_DEGLITCH1_EN);
        vIO32WriteFldAlign(PDWNC_EDID_DEV1, 3, FLD_DE_CNT1);
        vIO32WriteFldAlign(PDWNC_EDID_DEV2, 1, FLD_DEGLITCH2_EN);
        vIO32WriteFldAlign(PDWNC_EDID_DEV2, 3, FLD_DE_CNT2);
        vIO32WriteFldAlign(PDWNC_EDID_DEV3, 1, FLD_DEGLITCH3_EN);
        vIO32WriteFldAlign(PDWNC_EDID_DEV3, 3, FLD_DE_CNT3);
        //enable DDC line Schmitt trigger
        vIO32WriteFldAlign(PDWNC_PADCFG5, 1, FLD_HDMI_0_SDA_SMT);
        vIO32WriteFldAlign(PDWNC_PADCFG5, 1, FLD_HDMI_0_SCL_SMT);
        vIO32WriteFldAlign(PDWNC_PADCFG0, 1, FLD_HDMI_1_SDA_SMT);
        vIO32WriteFldAlign(PDWNC_PADCFG0, 1, FLD_HDMI_1_SCL_SMT);
        vIO32WriteFldAlign(PDWNC_PADCFG1, 1, FLD_HDMI_2_SDA_SMT);
        vIO32WriteFldAlign(PDWNC_PADCFG1, 1, FLD_HDMI_2_SCL_SMT);
		#if defined(CC_MT5399)
        vIO32WriteFldAlign(PDWNC_PADCFG8, 1, FLD_HDMI_3_SDA_SMT);
        vIO32WriteFldAlign(PDWNC_PADCFG6, 1, FLD_HDMI_3_SCL_SMT);
		#endif


#if Support_3D
        fpDrvRegisterTDTVModeChangeCB(E_TDTV_CB_HDMI, (vTDTVModeChangeCB)vDviTDTVCB);

        vIO32WriteFldAlign(N_HDMI_CTRL + u4RegBase, 0x1, TT2_EXT_PKT_EN);
        vIO32WriteFldAlign(N_HDMI_CTRL3 + u4RegBase, 0x81, EXT_PKT_DEC);
#endif

        vIO32WriteFldAlign(VID_CH_MAP + u4RegBase, 0x0, CHANNEL_MAP);
        vIO32WriteFldAlign(VID_SET + u4RegBase, 0, CEA_NEW_UNREC_SEL);
        vIO32WriteFldAlign(INTR_MASK0 + u4RegBase, 1, NEW_UNREC_ONLY);//interrupt at every 3D inforframe.

    vIO32WriteFldAlign(VID_CH_MAP + u4RegBase, 0x0, CHANNEL_MAP);

    vIO32WriteFldAlign(VID_SET + u4RegBase, 0, CEA_NEW_UNREC_SEL);
    vIO32WriteFldAlign(INTR_MASK0 + u4RegBase, 1, NEW_UNREC_ONLY);//interrupt
	vIO32WriteFldAlign(INTR_MASK0 + u4RegBase, 1, NEW_SPD_ONLY);//interrupt

#ifdef CC_AUD_HDMI_SPDIF_CFG
        vIO32WriteFldAlign(N_HDMI_CTRL3 + u4RegBase, 0x1, ACP_CLR_EN);
        vIO32WriteFldAlign(N_HDMI_CTRL3 + u4RegBase, 0x2, ACP_VS_SET); // clear ACP,spec is 600ms, set 2 is 16 x 22E28 bit[11:10] + 16=3x16=48frame , 48x16ms=768ms
#endif
		vIO32Write1B(VID_MODE_0 + u4RegBase, 0x00);

        // init hdmi sel
        vHDMISel(eHdmiPort);  // side port
        vHDMIHPDLow(eHdmiPort);
        vHdmiSetHpdStatus(eHdmiPort, 1);
        vHDMITMDSCTRL(eHdmiPort,FALSE);
		/*if(IS_XTAL_24MHZ())
		{
			//24M clk sel
			vIO32WriteFldAlign(TMDS_CTRL_STATUS1 + u4RegBase, 1, XTAL_24_SEL_ARC);
			vIO32WriteFldAlign(TMDS_CTRL_STATUS1 + u4RegBase, 1, XTAL_24_SEL_PCW);
		}
		else*/
		{
			//27M clk sel
			vIO32WriteFldAlign(TMDS_CTRL_STATUS1, 0, XTAL_24_SEL_ARC);
			vIO32WriteFldAlign(TMDS_CTRL_STATUS1, 0, XTAL_24_SEL_PCW);
		}
    }

#ifdef __HDMI_SLT__
//        vCbusInit();
#endif

    bHPDInitFlag = 0;
    HAL_GetTime(&bInitUnplugTime);
    // HPD driving current
#if defined(CC_MT5389)
    vIO32WriteFldAlign(PDWNC_PADCFG5,1,FLD_HDMI_0_HPD_E4);
    vIO32WriteFldAlign(PDWNC_PADCFG0,1,FLD_HDMI_1_HPD_E4);
    vIO32WriteFldAlign(PDWNC_PADCFG1,1,FLD_HDMI_2_SCL_E4);
#endif
    vHDMISwitchInit();

	//for IC back debug use
#if (DEFAULTKEYS == 1)
u1HDMI_SetHdcpKey(_au1Hdcp, sizeof(_au1Hdcp));
u1HDMI_SetEdidData(_au1Edid, sizeof(_au1Edid));
#endif

#ifdef CC_SUPPORT_HDMI_FBOOTING
    u1HdmiInit = 1;
#endif
	u1HDMI_MHL_Term_Value_Cali();
	//this bit is to open ctrl data, let them in always, it is eco item for hdcp dvi mode fail	
#if defined(CC_MT5399)
	if(IS_IC_5399_ES2())
	{
		vIO32WriteFldAlign(MHL_REG, 1, REG_HDMI_CTRL_SEL);
	}
#endif
#if defined(CC_MT5882)
	vIO32WriteFldAlign(MHL_REG, 1, REG_HDMI_CTRL_SEL);
#endif
	//change channel lock to 0, let HW lock it auto
	vMHLAdjustChanneValuel(0, 0, 0);
    //pull up cec pin
    vIO32WriteFldAlign(PDWNC_PADCFG0,1,FLD_HDMI_CEC_PU);
#ifdef DCC_MHL_VERIFY    
	vCbusInit();
#endif  
	u4SpdData.VdrNam[0] = 0;
	u4SpdData.VdrNam[1] = 0;
	u4SpdData.PrdDspt[0] = 0;
	u4SpdData.PrdDspt[1] = 0;
	u4SpdData.PrdDspt[2] = 0;
	u4SpdData.PrdDspt[3] = 0;
	u4SpdData.SrcInfo = 0;
#ifdef CC_HDMI_CONFIG_BOARD
    eBoardType = eHDMIGetBoardType();
#endif
}


/**
 * @brief 	HDMI resume Initial function
 * @param	None
 * @retval	None
 */
void vHDMI_pm_resume(void)
{
#ifdef CC_SUPPORT_HDMI_FBOOTING
    u1HdmiInit = 0;
#endif
    vHDMIInit();
}

/**
 * @brief HDMI Power Off function
 * @param None
 * @retval None
 */
void vHDMIPowerOff(void)
{
    if(_arHdmiRx[eActiveHdmiPort]._bHDMIState == HDMI_STATE_NOTREADY)
    {
        return;
    }

    _arHdmiRx[eActiveHdmiPort]._bHDMIState = HDMI_STATE_INIT;
    vHDMIHPDLow(eActiveHdmiPort);
    vHDMITMDSIndepCTRL(eActiveHdmiPort,FALSE);
    //vIO32WriteFldAlign(ANA_INTF_2,0x0,RG_HDMI_SW_EN);
    //vHDMITMDSIndepCTRL(0);
}


/**
 * @brief 	Update HDMI Audio Parameter
 * @param	None
 * @retval	None
 */
void vHDMIUpdateAudParm(void)
{
#ifdef SUPPORT_HDMI_AUDIO
    AIN_CFG_T rAudCfg;
    // Load audio configuration
    UNUSED(AUD_GetAinCfg(AUD_STREAM_FROM_HDMI, &rAudCfg));

    //To set bit number
    if(IO32ReadFldAlign(CHST1 + u4ActiveHdmiRegBase, AUD_LENGTH_MAX))		//max 24bits mode
    {
        switch(IO32ReadFldAlign(CHST1 + u4ActiveHdmiRegBase, AUD_LENGTH))
        {
            case 0x01:	//20bit
                rAudCfg.eBits = DAC_20_BIT;
                break;

            default:		//24bit
                rAudCfg.eBits = DAC_24_BIT;
                break;
        }
    }
    else					//max 20bits mode
    {
        switch(IO32ReadFldAlign(CHST1 + u4ActiveHdmiRegBase, AUD_LENGTH))
        {
            case 0x01:	//16bit
                rAudCfg.eBits = DAC_16_BIT;
                break;

            case 0x02:	//18bit
                rAudCfg.eBits = DAC_18_BIT;
                break;

            default:		//20bit
                rAudCfg.eBits = DAC_20_BIT;
                break;
        }
    }

    AUD_AinCfg(AUD_DEC_MAIN, AUD_STREAM_FROM_HDMI, (const AIN_CFG_T *)&rAudCfg);
#endif
}

/**
 * @brief 	HDMI Mode video setting
 * @param	None
 * @retval	None
 */
PRIVATE void vHDMIVideoHdmiSetting(void)
{
    UINT8 bReadData;
#if HDMI_DEBUG
    //LOG(6, "vHDMIVideoHdmiSetting\n");
#endif
    //LOG(3, "Check NO_AVI = %d\n",u1IO32Read1B(INTR_MASK0_0)&0x10);
    //LOG(3, "color space  = %d\n",u1IO32Read1B(AVIRX1_0)&0x60);
    /* HDMI mode */
    // Check NO_AVI
    bReadData = u1IO32Read1B(INTR_STATE1_0 + u4ActiveHdmiRegBase);

    if(bReadData&0x10)
    {
        vIO32Write1B(INTR_STATE1_0 + u4ActiveHdmiRegBase, bReadData);
        return;
    }

#if IC_3x3color_test
    //vIO32Write1B(VID_MODE_0 + u4ActiveHdmiRegBase, 0x00);
    vIO32Write1B(VID_MODE_1 + u4ActiveHdmiRegBase, 0x00);

    // check AVI infoframe packet type code
    if(u1IO32Read1B(AVIRX0_0 + u4ActiveHdmiRegBase) != 0x82)
    {
        // default: RGB
        //vIO32Write1B(VID_MODE_0,0x00);
        //vIO32Write1B(VID_MODE_1,0x00);
        //vIO32Write1B(VID_MODE_2,0x08);// RGB to YCbCr
        vIO32WriteFldAlign(VID_MODE + u4ActiveHdmiRegBase, 0x0, ENSYNCCODES);


        if(fgIsMainDVI())
        {
            vIO32WriteFldAlign(CCIR_00, 1, CCH_SRC24_SEL);
            vIO32WriteFldAlign(CCIR_00, 1, CCH_CBCR_SW);
			#ifndef CC_SUPPORT_PIPELINE
            vIO32WriteFldAlign(OMUX_00, 0x4, OMUX_MAIN_SEL);
			#endif
        }

        if(fgIsPipDVI())
        {
            vIO32WriteFldAlign(CCIR_00, 1, CCH_SRC24_SEL);
            vIO32WriteFldAlign(CCIR_00, 1, CCH_CBCR_SW);
			if(_bCcir2status == 0)
			{
				vIO32WriteFldAlign(OMUX_00, 0x4 , OMUX_PIP_SEL);//ccir1
			}
			else
			{
				vIO32WriteFldAlign(OMUX_00, 0x5 , OMUX_PIP_SEL);//ccir2
			}
        }

        vIO32WriteFldAlign(VID_MODE + u4ActiveHdmiRegBase, 0x00, ENUPSAMPLE);
        vIO32Write1B(VID_MODE_3 + u4ActiveHdmiRegBase, 0x0);//B
        vIO32Write1B(VID_BLANK_0 + u4ActiveHdmiRegBase, 0x0);//G
        vIO32Write1B(VID_BLANK_1 + u4ActiveHdmiRegBase, 0x0);//R
        return;
    }

    /*
    AVI DBYTE1
    [6:5] Y1 Y0
    	00 - RGB
    	01 - YCbCr 422
    	10 - YCbCr 444
    */
    bReadData = u1IO32Read1B(AVIRX1_0 + u4ActiveHdmiRegBase);

    if((bReadData & 0x60) == 0x00)  // RGB
    {
        /*
        AVI DBYTE2
        [7:6] C1 C0
        	00 - No Data
        	01 - ITU601
        	10 - ITU709
        */
        bReadData = u1IO32Read1B(AVIRX1_1 + u4ActiveHdmiRegBase);
        /* RGB to YCbCr Space Convert: 1=BT709, 0=BT601) */
        //vIO32Write1B(VID_MODE_0,((bReadData&0xc0)==0x40) ? 0x00 : 0x01);
        //vIO32Write1B(VID_MODE_1,0x00);
        // RGB to YCBCr Space Convert
        //vIO32Write1B(VID_MODE_2,0x08); // RGB to YCbCr
        vIO32WriteFldAlign(VID_MODE + u4ActiveHdmiRegBase, 0x0, ENSYNCCODES);

        if(fgIsMainDVI())
        {
            vIO32WriteFldAlign(CCIR_00, 1, CCH_SRC24_SEL);
            vIO32WriteFldAlign(CCIR_00, 1, CCH_CBCR_SW);
			#ifndef CC_SUPPORT_PIPELINE
            vIO32WriteFldAlign(OMUX_00, 0x4, OMUX_MAIN_SEL);
			#endif
        }

        //else if(fgIsPipDVI())
        if(fgIsPipDVI())
        {
            vIO32WriteFldAlign(CCIR_00, 1, CCH_SRC24_SEL);
            vIO32WriteFldAlign(CCIR_00, 1, CCH_CBCR_SW);
			if(_bCcir2status == 0)
			{
				vIO32WriteFldAlign(OMUX_00, 0x4 , OMUX_PIP_SEL);//ccir1
			}
			else
			{
				vIO32WriteFldAlign(OMUX_00, 0x5 , OMUX_PIP_SEL);//ccir2
			}
        }

        vIO32WriteFldAlign(VID_MODE + u4ActiveHdmiRegBase, 0x00, ENUPSAMPLE);
        //vRegWrite1B(VID_MODE_3 + u4ActiveHdmiRegBase, 0x10);
        //vRegWrite1B(VID_BLANK_0 + u4ActiveHdmiRegBase, 0x10);
        //vRegWrite1B(VID_BLANK_1 + u4ActiveHdmiRegBase, 0x10);

	    if ((u1RegRead1B(AVIRX1_2)  & 0x0c) == 0x04)   // Limited Range: 16 ~ 235.     
	    {	      
	       vIO32Write1B(VID_MODE_3, 0x10); 
		   vIO32Write1B(VID_BLANK_0, 0x10); 
		   vIO32Write1B(VID_BLANK_1, 0x10); 
		}        
		else if ((u1RegRead1B(AVIRX1_2)  & 0x0c) == 0x08)  // Full range: 0 ~ 255        
		{	      
		   vIO32Write1B(VID_MODE_3, 0x00); 
		   vIO32Write1B(VID_BLANK_0, 0x00);
		   vIO32Write1B(VID_BLANK_1, 0x00);
		}
    }
    else // YCbCR
    {
        //vIO32Write1B(VID_MODE_0,0x00);
        //vIO32Write1B(VID_MODE_1,0x00);
        // if 4:2:2 then do 4:2:2 to 4:4:4 up sample
        //vIO32WriteFldAlign(VID_MODE, ((bReadData&0x60)==0x20) ? 0x1: 0x00, ENUPSAMPLE);
        vIO32WriteFldAlign(VID_MODE + u4ActiveHdmiRegBase, 0x00, ENUPSAMPLE);

        if((u1IO32Read1B(AVIRX1_0 + u4ActiveHdmiRegBase)&0x60) == 0x20)	//422 is from HDMI to CCIR decoder
        {
            //vIO32WriteFldAlign(VID_MODE, 0x1, ENSYNCCODES);
            if(fgIsMainDVI())
            {
                vIO32WriteFldAlign(CCIR_00, 0, CCH_SRC24_SEL);
                vIO32WriteFldAlign(CCIR_00, 0, CCH_CBCR_SW);
				#ifndef CC_SUPPORT_PIPELINE
                vIO32WriteFldAlign(OMUX_00, 0x4, OMUX_MAIN_SEL);
				#endif
            }

            //	else if(fgIsPipDVI())
            if(fgIsPipDVI())
            {
                vIO32WriteFldAlign(CCIR_00, 0, CCH_SRC24_SEL);
                vIO32WriteFldAlign(CCIR_00, 0, CCH_CBCR_SW);
                if(_bCcir2status == 0)
				{
	                vIO32WriteFldAlign(OMUX_00, 0x4 , OMUX_PIP_SEL);//ccir1
                }
				else
				{
	                vIO32WriteFldAlign(OMUX_00, 0x5 , OMUX_PIP_SEL);//ccir2
				}
            }
        }
        else		//444
        {
            //vIO32WriteFldAlign(VID_MODE, 0x0, ENSYNCCODES);
            if(fgIsMainDVI())
            {
                vIO32WriteFldAlign(CCIR_00, 1, CCH_SRC24_SEL);
                vIO32WriteFldAlign(CCIR_00, 1, CCH_CBCR_SW);
				#ifndef CC_SUPPORT_PIPELINE
                vIO32WriteFldAlign(OMUX_00, 0x4, OMUX_MAIN_SEL);
				#endif
            }

            if(fgIsPipDVI())
            {
                vIO32WriteFldAlign(CCIR_00, 1, CCH_SRC24_SEL);
                vIO32WriteFldAlign(CCIR_00, 1, CCH_CBCR_SW);
                if(_bCcir2status == 0)
				{
	                vIO32WriteFldAlign(OMUX_00, 0x4 , OMUX_PIP_SEL);//ccir1
                }
				else
				{
	                vIO32WriteFldAlign(OMUX_00, 0x5 , OMUX_PIP_SEL);//ccir2
				}
            }
        }

        //vIO32Write1B(VID_MODE_2,((bReadData&0x60)==0x20) ? 0x04 : 0x00);
        vIO32Write1B(VID_MODE_3 + u4ActiveHdmiRegBase, 0x80);// Cr
        vIO32Write1B(VID_BLANK_1 + u4ActiveHdmiRegBase, 0x80);// Cb
        vIO32Write1B(VID_BLANK_0 + u4ActiveHdmiRegBase, 0x0);// Y
    }

#endif
}

static void vLogHdmiStateChange(E_HDMI_SWITCH_NUM ePort, UINT8 u1HdmiState)
{
    //   static UINT8 u1HdmiStateOld = 0;

    // Check if the state is correct
    if(u1HdmiState <= HDMI_STATE_AUTH)
    {
        if(_arHdmiRx[ePort]._bHDMIStateOld != u1HdmiState)
        {
            LOG(6, "Port %d _bHDMIState change from %s to %s\n",ePort, _aszHdmiState[_arHdmiRx[ePort]._bHDMIStateOld], _aszHdmiState[u1HdmiState]);
            _arHdmiRx[ePort]._bHDMIStateOld = u1HdmiState;
        }
    }
    else // Undefined state
    {
        if(_arHdmiRx[ePort]._bHDMIStateOld != u1HdmiState)
        {
            LOG(6, "Port %d _bHDMIState change from %s to UNDEFINED_STATE\n",ePort, _aszHdmiState[_arHdmiRx[ePort]._bHDMIStateOld]);
            _arHdmiRx[ePort]._bHDMIStateOld = HDMI_STATE_NOTREADY;
        }
    }

    // Suppress warning messages for release version
    UNUSED(_aszHdmiState);
}

void vHDMISetColorRalated(void)
{
    UINT8 bWriteData;
    UINT8 bReadData;

    /* VIDEO - check hdmi mode */
    //if (fgIsHdmiFlgSet(HDMI_MODE_CHG))
    {
        if(_bHdmiModeActive)
        {
            /* HDMI mode */
            /* Video Setting */
            vHDMIVideoHdmiSetting();
#if 1
            /* 2x pixel clock setting*/
            bReadData = u1IO32Read1B(AVIRX2_0 + u4ActiveHdmiRegBase) & 0xf;

            if(bReadData != 0)
            {
                bWriteData = 0x17;
            }
            else
            {
                bWriteData = 0x07;
            }

            vIO32Write1B(SYS_CTRL_0 + u4ActiveHdmiRegBase, bWriteData);// ICLK
#endif
        }
        else
        {
            /* DVI mode */
#if HDMI_DEBUG
            // LOG(6, "DVI mode\n");
#endif
            vIO32Write1B(SYS_CTRL_0 + u4ActiveHdmiRegBase, 0x07);// ICLK
            // ToDo - use VRes to detect HDTV input.
            //bReadData =u1IO32Read1B(AVIRX1_1);
            //vIO32Write1B(VID_MODE_0,((bReadData&0xc0)==0x40) ? 0x00 : 0x01);
            vIO32Write1B(VID_MODE_1 + u4ActiveHdmiRegBase, 0x0);
            // RGB to YCBCr Space Convert
            //vIO32Write1B(VID_MODE_2,fgIsVideoTiming(_bDviTiming) ? 0x08 : 0x00);// video timing first (customized)
            vIO32Write1B(VID_MODE_3 + u4ActiveHdmiRegBase, 0x10);
            vIO32Write1B(VID_BLANK_0 + u4ActiveHdmiRegBase, 0x10);
            vIO32Write1B(VID_BLANK_1 + u4ActiveHdmiRegBase, 0x10);
            vIO32WriteFldAlign(VID_MODE + u4ActiveHdmiRegBase, 0x0, ENSYNCCODES);

            if(fgIsMainDVI())
            {
                vIO32WriteFldAlign(CCIR_00, 1, CCH_SRC24_SEL);
                vIO32WriteFldAlign(CCIR_00, 1, CCH_CBCR_SW);
				#ifndef CC_SUPPORT_PIPELINE
                vIO32WriteFldAlign(OMUX_00, 0x4, OMUX_MAIN_SEL);
				#endif
            }
            if(fgIsPipDVI())
            {
                vIO32WriteFldAlign(CCIR_00, 1, CCH_SRC24_SEL);
                vIO32WriteFldAlign(CCIR_00, 1, CCH_CBCR_SW);
                if(_bCcir2status == 0)
				{
	                vIO32WriteFldAlign(OMUX_00, 0x4 , OMUX_PIP_SEL);//ccir1
                }
				else
				{
	                vIO32WriteFldAlign(OMUX_00, 0x5 , OMUX_PIP_SEL);//ccir2
				}
            }
        }

        // vClrHdmiFlg(HDMI_MODE_CHG);
    }
}



UINT32 vGetHDMIEQStatus(void)
{
    return(0);
}



UINT8 vHDMIVResCHG(void)
{
    static UINT16 HDMIVRes, tmp;
    tmp =
        (IO32ReadFldAlign(VID_VRES + u4ActiveHdmiRegBase, VID_VRES_11_8) << 8) +
        IO32ReadFldAlign(VID_VRES + u4ActiveHdmiRegBase, VID_VRES_7_0);

    if((tmp > HDMIVRes + 2) || (tmp < HDMIVRes - 2))	//for interlace mode
    {
        HDMIVRes = tmp;
        return(1);
    }
    else
    {
        return(0);
    }
}
UINT8 vHDMIClockCHG(void)
{
    static UINT16 HDMIClock, tmp;
    tmp = (IO32ReadFldAlign(VID_CRC_OUT + u4ActiveHdmiRegBase, AAC_XCLK_IN_PCLK_10_8) << 8) + \
          IO32ReadFldAlign(VID_CRC_OUT + u4ActiveHdmiRegBase, AAC_XCLK_IN_PCLK_7_0);

    if(tmp <= 1)					// 0xff + 1 = 0x100
    {
        tmp = tmp + 0x100;
    }

    if((tmp > HDMIClock + 2) || (tmp < HDMIClock - 2))
    {
        HDMIClock = tmp;
        return(1);
    }
    else
    {
        return (0);
    }
}

UINT8 bHDMIMCMHdcpWrite(void)
{
    return 1;
}

UINT8 vHDMIHResCHG(void)
{
    static UINT16 HDMIHRes = 0, tmp;
    tmp = IO32ReadFldAlign(VID_HRES + u4ActiveHdmiRegBase, VID_HRES_12_0);

    if((tmp > HDMIHRes + 6) || (tmp < HDMIHRes - 6))
    {
        HDMIHRes = tmp;
        return(1);
    }
    else
    {
        return(0);
    }
}

typedef enum
{
    PQL_HDMI_RGB = 0,
    PQL_HDMI_YCBCR,
    PQL_HDMI_XVYCC,
    PQL_DVI_RGB,
    PQL_HDMI_UNKNOWN,
} PQL_HDMI_COLOR_DOMAIN_T;

PQL_HDMI_COLOR_DOMAIN_T bGetHDMIColorDomain(void)
{
    if(_bHdmiModeActive)
    {
        UINT8 bReadData;

        if(u1IO32Read1B(AVIRX0_0 + u4ActiveHdmiRegBase) != 0x82)
        {
            return PQL_HDMI_RGB;
        }

        if(IO32ReadFldAlign(INTR_STATE0 + u4ActiveHdmiRegBase, INTR3_NEW_AVI))
        {
            _bNEW_AVI_Info = 1;
            bReadData = u1IO32Read1B(AVIRX1_0  + u4ActiveHdmiRegBase);
            vIO32Write4B(INTR_STATE0 + u4ActiveHdmiRegBase, 0x1 << 24);
            _bAVIInfo_tmp = bReadData;
        }
        else
        {
            bReadData = _bAVIInfo_tmp;
        }

        if((bReadData & 0x60) == 0x00)  // RGB
        {
            return PQL_HDMI_RGB;
        }
        else if((u1RegRead1B(AVIRX1_1 + u4ActiveHdmiRegBase) & 0xc0)==0xc0)  // extended color
        {
	     bReadData = u1RegRead1B(AVIRX1_2 + u4ActiveHdmiRegBase);
	     if (((bReadData >> 4) & 0x07) < 2)
	     {
	         return PQL_HDMI_XVYCC;
	     }
	     else
	     {
	         return PQL_HDMI_UNKNOWN;
	     }
        }
        /*
        else if((bReadData & 0x60)==0x60)  // xvYCC
        {
            bReadData=u1IO32Read1B(AVIRX1_2 + u4ActiveHdmiRegBase);

            if((bReadData&0x07)<2)
            {
                return PQL_HDMI_XVYCC;
            }
            else
            {
                return PQL_HDMI_UNKNOWN;
            }
        }
        */
        else  // YCBCR
        {
            return PQL_HDMI_YCBCR;
        }
    }
    else
    {
        return PQL_DVI_RGB;
    }
}

static  UINT8 u1VsdbInfo = 0;
static  UINT8 u1NoVsdbCnt = 0;
void vHdmiVsdbInfoCheck(void)
{
    if (1 == IO32ReadFldAlign(INTR_STATE0 + u4ActiveHdmiRegBase, INTR3_NEW_UNREC))
    {	
    	if((u1IO32Read1B(UNRECRX0_0 + u4ActiveHdmiRegBase) == 0x81))
		{
			if((u1IO32Read1B(UNRECRX1_3 + u4ActiveHdmiRegBase) >> 5) != 0x0)
			{
				u1VsdbInfo = 1;
				u1NoVsdbCnt = 0;
			}
		}
    	 vIO32Write4B(INTR_STATE0 + u4ActiveHdmiRegBase, Fld2Msk32(INTR3_NEW_UNREC));
    }

    if (u1VsdbInfo == 1)
    {
        if (u1NoVsdbCnt++ > 30)
        {
            u1VsdbInfo = 0;
      	     u1NoVsdbCnt = 0;
        }
    }
}

UINT8 u1IsHdmiVsdbInfo(void)
{
    return (u1VsdbInfo);
}

static HDMI_3D_INFOFRAME info_3Ddata;
HDMI_3D_INFOFRAME *API_HDMI_3D_Get_InfoFrame(void)
{
    HDMI_3D_INFOFRAME *info = &info_3Ddata;

    if(!_fgForce3D)
    {
        if((u1IO32Read1B(UNRECRX0_0 + u4ActiveHdmiRegBase) == 0x81) && ((1 == IO32ReadFldAlign(INTR_STATE0 + u4ActiveHdmiRegBase, INTR3_NEW_UNREC)) || (u1VsdbInfo == 1)))
        {
            if((u1IO32Read1B(UNRECRX1_3 + u4ActiveHdmiRegBase) >> 5) == 0x2)
            {
                //#ifndef CC_SCPOS_3DTV_SUPPORT
                // #if SUPPORT_SBS_CONVERT
                //info->HDMI_3D_Enable = 0;//force decoder send 2d information
                // #else
                //info->HDMI_3D_Enable = 1;
                //#endif
                //#else
                info->HDMI_3D_Enable = 1;
                //#endif
                info->HDMI_3D_Video_Format = u1IO32Read1B(UNRECRX1_3 + u4ActiveHdmiRegBase) >> 5;
                info->HDMI_3D_Structure = u1IO32Read1B(UNRECRX2_0 + u4ActiveHdmiRegBase) >> 4;

                if(info->HDMI_3D_Structure >= HDMI_3D_Structure_SideBySideHalf)
                {
                    info->HDMI_3D_EXTDATA = u1IO32Read1B(UNRECRX2_1 + u4ActiveHdmiRegBase) >> 4;
                }
                else
                {
                    info->HDMI_3D_EXTDATA = 0;
                }

             //DTV00388414 2D timing but with 3D frame packing VSI
	            if (info->HDMI_3D_Structure == HDMI_3D_Structure_FramePacking)
	            {
	               if (((_wDviWidth == 1920) && (_wDviHeight == 1080)) || \
                           ((_wDviWidth == 1920) && (_wDviHeight == 540)) || \
	                    ((_wDviWidth == 1280) && (_wDviHeight == 720)) || \
	                    ((_wDviWidth == 720) && (_wDviHeight == 480)) || \
	                    ((_wDviWidth == 720) && (_wDviHeight == 576)))
	               {
	                   info->HDMI_3D_Enable = 0;
	                   info->HDMI_3D_Video_Format = 0;
	                   info->HDMI_3D_EXTDATA = 0;
	               }
	            }
            }
			else if(((u1IO32Read1B(UNRECRX1_3 + u4ActiveHdmiRegBase) & 0x01) == 0x1) &&\
				(u1IO32Read1B(UNRECRX1_0 + u4ActiveHdmiRegBase) == 0x1d) &&\
				(u1IO32Read1B(UNRECRX1_1 + u4ActiveHdmiRegBase) == 0xa6) &&\
				(u1IO32Read1B(UNRECRX1_2 + u4ActiveHdmiRegBase) == 0x7c))
			{
					info->HDMI_3D_Enable = 1;
					//info->HDMI_3D_Video_Format = u1IO32Read1B(UNRECRX1_3 + u4ActiveHdmiRegBase) >> 5;
					LOG(6,"mhl 3d vsif\n");
					//MHL FS timing treat as FP in HDMI
					if(((u1IO32Read1B(UNRECRX1_3 + u4ActiveHdmiRegBase) >> 2) & 0x0f) == 00)
					{
						info->HDMI_3D_Structure = HDMI_3D_Structure_FramePacking;
					}
					else if(((u1IO32Read1B(UNRECRX1_3 + u4ActiveHdmiRegBase) >> 2) & 0x0f) == 01)
					{
						info->HDMI_3D_Structure = HDMI_3D_Structure_TopBottom;
					}
					else if(((u1IO32Read1B(UNRECRX1_3 + u4ActiveHdmiRegBase) >> 2) & 0x0f) == 02)
					{
						info->HDMI_3D_Structure = HDMI_3D_Structure_SideBySideHalf;
					}
					
					info->HDMI_3D_EXTDATA = 0;
			}
            else
            {
                info->HDMI_3D_Enable = 0;
            }
        }
        else
        {
            info->HDMI_3D_Enable = 0;
	        info->HDMI_3D_Video_Format = 0;
	        info->HDMI_3D_EXTDATA = 0;
        }
    }
    else  // force 3D
    {
        info->HDMI_3D_Enable = 1;
        info->HDMI_3D_Video_Format = 0x2;
        info->HDMI_3D_Structure = _bForce3DStructure;

        if(info->HDMI_3D_Structure >= HDMI_3D_Structure_SideBySideHalf)
        {
            info->HDMI_3D_EXTDATA = _bForce3DExtData;
        }
        else
        {
            info->HDMI_3D_EXTDATA = 0;
        }
    }

    return info;
}

static HDMI_INFOFRAME_DESCRIPTION info_data;
HDMI_INFOFRAME_DESCRIPTION *API_HDMI_Get_InfoFrame(UINT8 frame)
{
    UINT8 i;
    UINT32 u2startaddr=0;
    HDMI_INFOFRAME_DESCRIPTION *info = &info_data;

    if (_bHdmiModeActive)
    {

    switch(frame)
    {
        case HDMI_INFOFRAME_AVI:
               if (u1IO32Read1B(AVIRX0_0 + u4ActiveHdmiRegBase) != 0x82)
               {
                   u2startaddr = 0;
                   info->framesize = 0;
               }
               else
               {
            u2startaddr = AVIRX0;
            info->framesize = 5;
               }
            break;

        case HDMI_INFOFRAME_SPD:
            u2startaddr = SPDRX0;
            info->framesize = 8;
            break;

        case HDMI_INFOFRAME_AUDIO:
            u2startaddr = AUDRX0;
            info->framesize = 4;
            break;

        case HDMI_INFOFRAME_MPEG:
            u2startaddr = MPEGRX0;
            info->framesize = 8;
            break;

        case HDMI_INFOFRAME_UNREC:
            u2startaddr = UNRECRX0;
            info->framesize = 8;
            break;

        case HDMI_INFOFRAME_GAMUT:
            u2startaddr = GAMUTRX0;
            info->framesize = 8;
            break;

        case HDMI_INFOFRAME_ACP:
            info->framesize = 8;

            if(IO32ReadFldAlign(INTR_STATE1 + u4ActiveHdmiRegBase, INTR6_NEW_ACP))
            {
                vIO32Write4B(INTR_STATE1 + u4ActiveHdmiRegBase, Fld2Msk32(INTR6_NEW_ACP));
                u2startaddr = ACPRX0;
            }
            else
            {
                u2startaddr = 0;
            }

            break;

        case HDMI_INFOFRAME_VSDB:
            info->framesize = 8;

            if(bHDMI3DPacketVaild() == 0) //input 2D mode
            {
                u2startaddr = 0;
            }
            else
            {
                u2startaddr = UNRECRX0;
            }

            break;

        default:
            u2startaddr = AVIRX0;
            info->framesize = 5;
            break;

    }

    }
    else
    {
        u2startaddr = 0;
        info->framesize = 0;
    }

    LOG(10, "HDMIInfoFrame size =%d\n", info->framesize);

    for(i = 0; i < (info->framesize); i++)
    {
        if(i < 10)
        {
            if(u2startaddr == 0)
            {
                info->framedata[i] = 0;
            }
            else
            {
                info->framedata[i] = u4IO32Read4B(u2startaddr + u4ActiveHdmiRegBase + (i * 4));
            }

            LOG(10, "HDMIInfoFrame=%x\n", info->framedata[i]);
        }
        else
        {
            break;
        }
    }

    return info;
}
void API_HDMI_Get_Aud_ChannelStatus(UINT8 u1DecId, HDMI_AUDIO_CHANNEL_STATUS_T *ChStatus)
{
	UINT8 Temp;
	UINT32 u4HdmiRegBase=0x400;
	UNUSED(u1DecId);

	u4HdmiRegBase=u4GetHdmiRegBase(eHDMICurrSwitch);
	ChStatus->CopyRight = IO32ReadFldAlign(AUDRX_CTRL + u4HdmiRegBase, AUD_CH_STAT1_2);
	Temp = IO32ReadFldAlign(AUDRX_CTRL + u4HdmiRegBase, AUD_CH_STAT2);
	ChStatus->Category_code_0_2 = Temp & 0x7;
	ChStatus->Category_code_0_6 = Temp & 0x7f;
	ChStatus->Category_code_3 = Temp & 0x8;
	ChStatus->LBit = Temp & 0x80;
}
//channel_num =0 means refer to stream header, other value represent channel number
UINT8 API_HDMI_Get_Aud_ChannelNum(void)
{
	UINT8 channel_num = 0;
	UINT32 u4HdmiRegBase=0;

	u4HdmiRegBase=u4GetHdmiRegBase(eHDMICurrSwitch);
	channel_num = u1IO32Read1B(AUDRX1_0 + u4HdmiRegBase);
	channel_num &= 0x07;

	return channel_num;
}

UINT32 wHDMIInfoFrame[10];
void  vHDMIGetInfoFrame(UINT8 frame)
{
    UINT8 num, i,j;
    UINT32 startaddr=0;
	UINT8 u1Data=0;

    switch(frame)
    {
        case HDMI_INFOFRAME_AVI:
            startaddr = AVIRX0;
			 LOG(0, "DUMP AVI InfoFrame :\n");
            num = 5;
            break;

        case HDMI_INFOFRAME_SPD:
            startaddr = SPDRX0;
			LOG(0, "DUMP SPD InfoFrame :\n");
            num = 8;
            break;

        case HDMI_INFOFRAME_AUDIO:
            startaddr = AUDRX0;
			LOG(0, "DUMP AUD InfoFrame :\n");
            num = 4;
            break;

        case HDMI_INFOFRAME_MPEG:
            startaddr = MPEGRX0;
            num = 8;
            break;

        case HDMI_INFOFRAME_UNREC:
            startaddr = UNRECRX0;
			LOG(0, "DUMP VS InfoFrame :\n");
            num = 8;
            break;

        case HDMI_INFOFRAME_GAMUT:
            startaddr = GAMUTRX0;
            num = 8;
            break;

        case HDMI_INFOFRAME_ACP:
            startaddr = ACPRX0;
            num = 8;
            break;

        default:
            startaddr = AVIRX0;
            num = 5;
            break;
    }

    for(i = 0; i < num; i++)
    {
        if(i < 10)
        {
            for(j = 0; j < 4; j++)
            {
            	u1Data = u1IO32Read1B(startaddr + u4ActiveHdmiRegBase + (i * 4)+j);
            	LOG(0, "0x%x\n", u1Data);
            }
        }
        else
        {
            break;
        }
    }
}
UINT16 wHDMIInfoFrameMask(void)
{
    UINT16 PacketMask;

    if(_bNEW_AVI_Info)
    {
        _bNEW_AVI_Info = 0;
        PacketMask = ((u1IO32Read1B(INTR_STATE0_3 + u4ActiveHdmiRegBase) & 0x1f) | 0x1) | (IO32ReadFldAlign(INTR_STATE1 + u4ActiveHdmiRegBase, INTR7_GAMUT) << HDMI_INFOFRAME_GAMUT);
    }
    else
    {
        PacketMask = (u1IO32Read1B(INTR_STATE0_3 + u4ActiveHdmiRegBase) & 0x1f) | (IO32ReadFldAlign(INTR_STATE1 + u4ActiveHdmiRegBase, INTR7_GAMUT) << HDMI_INFOFRAME_GAMUT) ;
    }

    vIO32Write4B(INTR_STATE0 + u4ActiveHdmiRegBase, 0x1e << 24);
    vIO32Write4B(INTR_STATE1 + u4ActiveHdmiRegBase, 0x1 << 26);//vIO32Write4B(N_HDMI_CTRL,0x1<<2);
    return PacketMask;
}
UINT8 bHDMIDeepColorStatus(void)
{
    return IO32ReadFldAlign(SRST + u4ActiveHdmiRegBase, DEEP_STA);
}

/**
 * @brief 	vXpcCount
 * @param	None
 * @retval	None
 */
static void vXpcStableCount(void)
{
    if(IO32ReadFldAlign(SRST + u4ActiveHdmiRegBase, SCDT) && \
       ((_arHdmiRx[eActiveHdmiPort]._bHDMIState == HDMI_STATE_SCDT) || \
        (_arHdmiRx[eActiveHdmiPort]._bHDMIState == HDMI_STATE_AUTH)))
    {
        u4CurXpcCnt = wHDMIXPCCNT();

        // CKDT stable counting
        if(RANGE_CHECKING(u4CurXpcCnt, u4PreXpcCnt, 3))
        {
            if(_bXpcStableCnt < 255)
            {
                _bXpcStableCnt++;
            }
        }
        else
        {
            if(_bXpcStableCnt == 255)
            {
                _bXpcStableCnt--;
            }
            else
            {
                _bXpcStableCnt = 0;
            }
        }

        u4PreXpcCnt = u4CurXpcCnt;
    }
}

/**
 * @brief 	_bIsXpcStable
 * @param	None
 * @retval	1: stable, 0: unstable.
 */
UINT8 _bIsXpcStable(void)
{
    return ((_bXpcStableCnt > HDMI_XPC_STABLE_CNT) ? 1 : 0);
}

extern UINT8   _bIntMuteCnt  ;
extern UINT8   _bAdoFlowFlg  ;

UINT8 u1GetHdmiCmd(void)
{
    return (_arHdmiRx[eActiveHdmiPort]._bHdmiCmd);
}

UINT8 u1GetHdmiState(void)
{
    return (_arHdmiRx[eActiveHdmiPort]._bHDMIState);
}

void vSetHdmiState(UINT8 u1State)
{
    _arHdmiRx[eActiveHdmiPort]._bHDMIState = u1State;
}

UINT8 u1IsHdmiUnplug(E_HDMI_SWITCH_NUM ePort)
{
    return(_arHdmiRx[ePort]._bUnplugFlag);
}

void vClrHdmiUnplug(E_HDMI_SWITCH_NUM ePort)
{
    _arHdmiRx[ePort]._bUnplugFlag = 0;
}

void vSetHdmiUnplug(E_HDMI_SWITCH_NUM ePort)
{
    _arHdmiRx[ePort]._bUnplugFlag = 1;
}

UINT8 u1IsHdmiTxSTBY(E_HDMI_SWITCH_NUM ePort)
{
  return(_arHdmiRx[ePort]._bTxSTBY);
}

void vSetHdmiFastSw(E_HDMI_SWITCH_NUM ePort, UINT8 u1Val)
{
    _arHdmiRx[ePort]._bHDMIFastSW = u1Val;

    if(u1Val)
    {
        //_arHdmiRx[ePort]._bResStableCnt = 0;
        HAL_GetTime(&_arHdmiRx[ePort]._rHdmiSyncWaitTime);
    }
}

UINT8 u1IsHdmiFastSw(E_HDMI_SWITCH_NUM ePort)
{
    return(_arHdmiRx[ePort]._bHDMIFastSW);
}

void vSetHdmiSrcIn(E_HDMI_SWITCH_NUM ePort, UINT8 u1Val)
{
    _arHdmiRx[ePort]._u1HdmiSrcIn= u1Val;
}

UINT8 u1IsHdmiSrcIn(E_HDMI_SWITCH_NUM ePort)
{
    return(_arHdmiRx[ePort]._u1HdmiSrcIn);
}

UINT8 u1HdmiHdcpCnt(E_HDMI_SWITCH_NUM ePort)
{
    return(_arHdmiRx[ePort]._u1HdcpGood);
}

void vHdmiStableMonitor(E_HDMI_SWITCH_NUM ePort)
{
    if(u1IsResStable(ePort))
    {
        if(_arHdmiRx[ePort]._bResStableCnt < 255)
        {
            _arHdmiRx[ePort]._bResStableCnt += 1;
        }
    }
    else
    {
        _arHdmiRx[ePort]._bResStableCnt = 0;
    }
}

UINT8 u1IsHdmiResStable(E_HDMI_SWITCH_NUM ePort)
{
    return ((_arHdmiRx[ePort]._bResStableCnt > HDMI_RES_STABLE) ? 1:0);
}

UINT32 u4BitCount(UINT8 *pData, UINT8 Len)
{
   UINT32 count = 0 ;
   UINT8 i = 0;
   UINT8 u1Buf = 0 ;

   for ( i  = 0 ; i < Len ; i++)
  {
      u1Buf = *(pData+i);
      while (u1Buf)
      {
         count++ ;
         u1Buf &= (u1Buf - 1);
      }
   }

   return count ;
}

void vHdcpMonitor(E_HDMI_SWITCH_NUM ePort)
{
    UINT8 u1Buf = 0;
    UINT8 u1Buf1 = 0;
    UINT32 u4TmpHdmiResBase = 0;
    UINT8 pBuf[5] = {0};
    u4TmpHdmiResBase = u4GetHdmiRegBase(ePort);
    u1Buf = u1IO32Read1B(HDCP_STAT_2 + u4TmpHdmiResBase) & 0x30; //00: No HDCP; 01: Authenticating; 11: AUTH Done

    if (_arHdmiRx[ePort].u1HdcpStatus != u1Buf)
    {
        LOG(0, "Rx_%d hdcp status changed 0x%02x --> 0x%02x\n", ePort, _arHdmiRx[ePort].u1HdcpStatus, u1Buf);

        if (u1Buf == 0x30)
        {
           LOG(0, "An: 0x%2x, 0x%2x, 0x%2x, 0x%2x, 0x%2x, 0x%2x, 0x%2x, 0x%2x \n", \
           	         IO32ReadFldAlign(SHD_AN0+ u4TmpHdmiResBase, HDCP_AN1), \
           	         IO32ReadFldAlign(SHD_AN0+ u4TmpHdmiResBase, HDCP_AN2), \
           	         IO32ReadFldAlign(SHD_AN1+ u4TmpHdmiResBase, HDCP_AN3), \
           	         IO32ReadFldAlign(SHD_AN1+ u4TmpHdmiResBase, HDCP_AN4), \
           	         IO32ReadFldAlign(SHD_AN1+ u4TmpHdmiResBase, HDCP_AN5), \
           	         IO32ReadFldAlign(SHD_AN1+ u4TmpHdmiResBase, HDCP_AN6), \
           	         IO32ReadFldAlign(SHD_BSTATUS+ u4TmpHdmiResBase, HDCP_AN7), \
           	         IO32ReadFldAlign(SHD_BSTATUS+ u4TmpHdmiResBase, HDCP_AN8));

           pBuf[0] = IO32ReadFldAlign(SHD_AKSV+ u4TmpHdmiResBase, HDCP_AKSV1);
           pBuf[1] = IO32ReadFldAlign(SHD_AKSV+ u4TmpHdmiResBase, HDCP_AKSV2);
           pBuf[2] = IO32ReadFldAlign(SHD_AKSV+ u4TmpHdmiResBase, HDCP_AKSV3);
           pBuf[3] = IO32ReadFldAlign(SHD_AN0+ u4TmpHdmiResBase, HDCP_AKSV4);
           pBuf[4] = IO32ReadFldAlign(SHD_AN0+ u4TmpHdmiResBase, HDCP_AKSV5);
           LOG(0, "AKSV: 0x%2x, 0x%2x, 0x%2x, 0x%2x, 0x%2x, 0x%2x, 0x%2x, 0x%2x \n", \
           	         pBuf[0], pBuf[1], pBuf[2], pBuf[3], pBuf[4]);
           if (u4BitCount(pBuf, sizeof(pBuf)) != 20)
           {
                LOG(0, "Invalid AKSV !!!!!! \n");
           }
           else
           {
                LOG(0, "Valid AKSV \n");
           }

           pBuf[0] = IO32ReadFldAlign(SHD_BKSV0+ u4TmpHdmiResBase, HDCP_BKSV1);
           pBuf[1] = IO32ReadFldAlign(SHD_BKSV0+ u4TmpHdmiResBase, HDCP_BKSV2);
           pBuf[2] = IO32ReadFldAlign(SHD_BKSV1+ u4TmpHdmiResBase, HDCP_BKSV3);
           pBuf[3] = IO32ReadFldAlign(SHD_BKSV1+ u4TmpHdmiResBase, HDCP_BKSV4);
           pBuf[4] =  IO32ReadFldAlign(SHD_BKSV1+ u4TmpHdmiResBase, HDCP_BKSV5);
           LOG(0, "BKSV: 0x%2x, 0x%2x, 0x%2x, 0x%2x, 0x%2x, 0x%2x, 0x%2x, 0x%2x \n", \
           	         pBuf[0], pBuf[1], pBuf[2], pBuf[3], pBuf[4]);
           if (u4BitCount(pBuf, sizeof(pBuf)) != 20)
           {
                LOG(0, "Invalid BKSV !!! \n");
           }
           else
           {
                LOG(0, "Valid BKSV \n");
           }
            _arHdmiRx[ePort].u4HdcpLoops = 0;

        }

        _arHdmiRx[ePort].u1HdcpStatus = u1Buf;
    }

    u1Buf = IO32ReadFldAlign(SHD_BKSV1 + u4TmpHdmiResBase, HDCP_RI0_15_8);
    u1Buf1 = IO32ReadFldAlign(SHD_AKSV + u4TmpHdmiResBase, HDCP_RI0_7_0);
    if ((_arHdmiRx[ePort].u1RiH != u1Buf) || ( _arHdmiRx[ePort].u1RiL != u1Buf1))
    {
         _arHdmiRx[ePort].u4HdcpLoops++;
         LOG(0, "Rx_%d, loop: %07d R0 0x%02x, 0x%2x \n", ePort, _arHdmiRx[ePort].u4HdcpLoops, u1Buf, u1Buf1);
         _arHdmiRx[ePort].u1RiH = u1Buf;
         _arHdmiRx[ePort].u1RiL = u1Buf1;
    }
}

void vHdcpStableMonitor(E_HDMI_SWITCH_NUM ePort)
{
    UINT8 u1Buf = 0;
    UINT32 u4TmpHdmiResBase = 0;
    u4TmpHdmiResBase = u4GetHdmiRegBase(ePort);
    u1Buf = u1IO32Read1B(HDCP_STAT_2 + u4TmpHdmiResBase) & 0x30; //00: No HDCP; 01: Authenticating; 11: AUTH Done

    if(_arHdmiRx[ePort].u1HdcpStatus != u1Buf)
    {
        _arHdmiRx[ePort].u1HdcpStatus = u1Buf;
        HAL_GetTime(&_arHdmiRx[ePort]._rHdmiSyncWaitTime);
    }

    if(u1Buf == 0x30)
    {
        if(_arHdmiRx[ePort]._u1HdcpGood < 255)
        {
            _arHdmiRx[ePort]._u1HdcpGood++;
        }
    }
    else
    {
        _arHdmiRx[ePort]._u1HdcpGood = 0;
    }
}

UINT8 u1IsHdcpStable(E_HDMI_SWITCH_NUM ePort)
{
    return ((_arHdmiRx[ePort]._u1HdcpGood > HDMI_HDCP_STABLE) ? 1 : 0);
}

UINT8 u1IsHdmiRxShared(E_HDMI_SWITCH_NUM ePort)
{
    return (_arHdmiRx[ePort]._bRxShared);
}

UINT8 u1IsHdmiHpdPulse(E_HDMI_SWITCH_NUM ePort)
{
    return (_arHdmiRx[ePort]._u8HpdLow);
}

void vHdmiSetHpdStatus(E_HDMI_SWITCH_NUM ePort, UINT8 u1Val)
{
    _arHdmiRx[ePort]._u8HpdLow = u1Val;
}
#ifdef SYS_MHL_SUPPORT
UINT8 u1IsMhlSignal(void)
{
#if defined(CC_MT5399)// only for 99,except 82.
	if( (_bIsMhlDeviceConnect == 1) && (eActiveHdmiPort == HDMI_SWITCH_1) )
	{
		return 1;
	}
	else
#endif
	{
		return 0;
	}
}
#endif


UINT8 u1IsSyncLose(E_HDMI_SWITCH_NUM ePort)
{
    return(_arHdmiRx[ePort]._bSyncLose);
}

void vClrSyncLose(E_HDMI_SWITCH_NUM ePort)
{
    _arHdmiRx[ePort]._bSyncLose= 0;
}

void vSetSyncLose(E_HDMI_SWITCH_NUM ePort)
{
    _arHdmiRx[ePort]._bSyncLose= 1;
}

void vHdmiAvMuteMonitor(E_HDMI_SWITCH_NUM ePort)
{
    HAL_TIME_T rCurTime;
    UINT32 u4TmpHdmiResBase = 0;

    u4TmpHdmiResBase = u4GetHdmiRegBase(ePort);

    if (IO32ReadFldAlign(AUDP_STAT + u4TmpHdmiResBase, HDMI_MUTE)) {
        HAL_GetTime(&rCurTime);

        if (_arHdmiRx[ePort]._bAvMuting == 0) {
              // reset timer
        	HAL_GetTime(&_arHdmiRx[ePort]._rHdmiAvMutingTime);
        	_arHdmiRx[ePort]._bAvMuting = 1;
        }

        // if timeout, force removing AVMUTE
        if ((rCurTime.u4Seconds - _arHdmiRx[ePort]._rHdmiAvMutingTime.u4Seconds) >= AVMUTE_TIMEOUT) {
            vIO32WriteFldAlign(SRST + u4TmpHdmiResBase, 1,  SW_RST);
            vIO32WriteFldAlign(SRST + u4TmpHdmiResBase, 0,  SW_RST);
      	     _arHdmiRx[ePort]._bAvMuting = 0;
        }

    } else {
        if (_arHdmiRx[ePort]._bAvMuting == 1) {
            _arHdmiRx[ePort]._bAvMuting = 0;
        }
    }

}
#ifdef HDMI_SUPPORT_SPD_IFOFRAME
static HDMI_INFOFRAME_DESCRIPTION info_data_1;

void vHDMIGetSPDData(void)
{
	HDMI_INFOFRAME_DESCRIPTION *SPDInfoDataGet;
	if(u1SpdTest)
	{
		return;
	}
	SPDInfoDataGet = &info_data_1;

	if(IO32ReadFldAlign(INTR_STATE0 + u4ActiveHdmiRegBase, INTR3_NEW_SPD) == 1)
	{
		SPDInfoDataGet = API_HDMI_Get_InfoFrame(HDMI_INFOFRAME_SPD);
		u4SpdData.VdrNam[0] = SPDInfoDataGet->framedata[1];
		u4SpdData.VdrNam[1] = SPDInfoDataGet->framedata[2];
		u4SpdData.PrdDspt[0] = SPDInfoDataGet->framedata[3];
		u4SpdData.PrdDspt[1] = SPDInfoDataGet->framedata[4];
		u4SpdData.PrdDspt[2] = SPDInfoDataGet->framedata[5];
		u4SpdData.PrdDspt[3] = SPDInfoDataGet->framedata[6];
		u4SpdData.SrcInfo = SPDInfoDataGet->framedata[7] & 0xff;
		//LOG(0,"$$$$$$---NEW SPD INFOR GET---$$$$$$\n");
		vIO32Write4B(INTR_STATE0 + u4ActiveHdmiRegBase, Fld2Msk32(INTR3_NEW_SPD));
	}
#if 0	
	else
	{
		u4SpdData.VdrNam[0] = 0;
		u4SpdData.VdrNam[1] = 0;
		u4SpdData.PrdDspt[0] = 0;
		u4SpdData.PrdDspt[1] = 0;
		u4SpdData.PrdDspt[2] = 0;
		u4SpdData.PrdDspt[3] = 0;
		u4SpdData.SrcInfo = 0;
	}
#endif	
	if((IO32ReadFldAlign(SPDRX0 + u4ActiveHdmiRegBase, CEA_SPD_HEADER7_0) == 0x83)
		|| ((SPDInfoDataGet->framedata[0] & 0xff) == 0x83))
	{
		u1IsSPDFlag = 1;
	}
	else
	{
		u1IsSPDFlag = 0;
	}

}
UINT8 u1HDMISvUpPatType(void)
{
	if(u4SpdData.SrcInfo == 0x06)//UP SourceDevice Information Is 0X06
	{
		return UP_PATTERN;
	}
	else//SV SourceDevice Information Is NULL
	{
		return SV_PATTERN;
	}
}
BOOL u1IsSPDSpecPatUP(void)
{
	if(u1HDMISvUpPatType() == UP_PATTERN)
	{
		if((u4SpdData.VdrNam[0] == UP_PATTERN_SONY) && (u4SpdData.VdrNam[1] == UP_PATTERN_SONY_NULL))
		{
			return TRUE;
		}
		else if((u4SpdData.VdrNam[0] == UP_PATTERN_SCEI) && (u4SpdData.VdrNam[1] == UP_PATTERN_SCEI_NULL))
		{
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}
	else
	{
		return FALSE;
	}
}

UINT8 u1HdmiGetUPStatus(void)
{
	if(_bHdmiMode && u1IsSPDFlag)
	{
		if(u1IsSPDSpecPatUP() == TRUE)
		{
			u1UPStaNew = HDMI_SPD_UP_DETED;
			return HDMI_SPD_UP_DETED;
		}
		else
		{
			u1UPStaNew = HDMI_SPD_UP_UNDET;
			return HDMI_SPD_UP_UNDET;
		}
	}
	else
	{
		return HDMI_SPD_UP_UNDET;
	}
}
UINT8 u1HdmiGetUPStaChg(void)
{
	
	UNUSED(u1HdmiGetUPStatus());
	if(_bHdmiMode && u1IsSPDFlag)
	{
		if((u1UPStaOld == HDMI_SPD_UP_UNDET || u1UPStaOld == HDMI_SPD_NONE_STATUS) && (u1UPStaNew == HDMI_SPD_UP_DETED))
		{
			u1UPStaOld = u1UPStaNew;
			UPChgStaSave = HDMI_SPD_CHG_TOUP;
			return HDMI_SPD_CHG_TOUP;
		}
		else if((u1UPStaOld == HDMI_SPD_UP_DETED || u1UPStaOld == HDMI_SPD_NONE_STATUS) && (u1UPStaNew == HDMI_SPD_UP_UNDET))
		{
			u1UPStaOld = u1UPStaNew;
			UPChgStaSave = HDMI_SPD_CHG_TONONUP;
			return HDMI_SPD_CHG_TONONUP;
		}
		else
		{
			return UPChgStaSave;
			//return HDMI_SPD_NONE_CHG;
		}
	}
	else
	{
		u1UPStaOld = HDMI_SPD_UP_UNDET;
		u1UPStaNew = HDMI_SPD_UP_UNDET;
		return HDMI_SPD_CLEAR;
	}
}

BOOL u1IsSPDSpecPatSV(void)
{
#if 0
	if(u1HDMISvUpPatType() == SV_PATTERN)
	{
		if((u4SpdData.VdrNam[0] == SV_PATTERN_SCEI) \
		&& (u4SpdData.VdrNam[1] == SV_PATTERN_SCEI_NULL) \
		&& (u4SpdData.PrdDspt[0] == SV_PATTERN_SCEI_PS3D))
		{
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}
	else
#endif
	{
		return FALSE;
	}
}


UINT8 u1HdmiGetSVStatus(void)
{
#if 0
	if(_bHdmiMode && u1IsSPDFlag)
	{
		if(u1IsSPDSpecPatSV() == TRUE)
		{
			u1SVStaNew = HDMI_SPD_SV_DETED;
			return HDMI_SPD_SV_DETED;
		}
		else
		{
			u1SVStaNew = HDMI_SPD_SV_UNDET;
			return HDMI_SPD_SV_UNDET;
		}
	}
	else
#endif
	{
		return HDMI_SPD_SV_UNDET;
	}

}

UINT8 u1HdmiGetSVStaChg(void)
{
	UNUSED(u1HdmiGetSVStatus());
#if 0
	if(_bHdmiMode && u1IsSPDFlag)
	{
		if((u1SVStaOld == HDMI_SPD_SV_UNDET || u1SVStaOld == HDMI_SPD_NONE_STATUS) && (u1UPStaNew == HDMI_SPD_SV_DETED))
		{
			u1SVStaOld = u1SVStaNew;
			SVChgStaSave = HDMI_SPD_CHG_TOSV;
			return HDMI_SPD_CHG_TOSV;
		}
		else if((u1SVStaOld == HDMI_SPD_SV_DETED || u1SVStaOld == HDMI_SPD_NONE_STATUS) && (u1SVStaNew == HDMI_SPD_SV_UNDET))
		{
			u1SVStaOld = u1SVStaNew;
			SVChgStaSave = HDMI_SPD_CHG_TONONSV;
			return HDMI_SPD_CHG_TONONSV;
		}
		else
		{
			return SVChgStaSave;
			//return HDMI_SPD_NONE_CHG;
		}
	}
	else
#endif
	{
		u1SVStaOld = HDMI_SPD_SV_UNDET;
		u1SVStaNew = HDMI_SPD_SV_UNDET;
		return HDMI_SPD_CLEAR;
	}
}


#endif

UINT8 u1HDMI_Get4KPStatus(void)
{
	UINT32 u1VendorName;
	UINT32 u1ProductDescription; 
	UINT8 u1Is4KPs=0;

	if(!_bHdmiModeActive)
	{
		u1Is4KPs =0;
	}
	if(IO32ReadFldAlign(INTR_STATE0 + u4ActiveHdmiRegBase, INTR3_NEW_SPD) && (u1IO32Read1B(SPDRX0_0 + u4ActiveHdmiRegBase) == 0x83))
	{

		u1VendorName = u4IO32Read4B(SPDRX1 + u4ActiveHdmiRegBase);
		u1ProductDescription = u4IO32Read4B(SPDRX3 + u4ActiveHdmiRegBase);
		if((u1VendorName == SV_PATTERN_SCEI) &&(u1VendorName == SV_PATTERN_SCEI_PS3D))
		{
			u1Is4KPs =1;
		}
	}

	return u1Is4KPs;
}
UINT8 u1HDMI_Get5VStatus(E_HDMI_SWITCH_NUM ePort)
{
#if  defined(CC_MT5396)

	UINT32 dUIHDMIPortMappingHDMI1;
	UINT32 dUIHDMIPortMappingHDMI2;
	UINT32 dUIHDMIPortMappingHDMI3;
	UINT32 dUIHDMIPortMappingHDMI4;



	if (DRVCUST_OptQuery(eHDMIPortMappingHDMI1, &dUIHDMIPortMappingHDMI1) != 0)
	{
		dUIHDMIPortMappingHDMI1 = HDMI_SWITCH_1;
	}
	if (DRVCUST_OptQuery(eHDMIPortMappingHDMI2, &dUIHDMIPortMappingHDMI2)!=0)
	{
		dUIHDMIPortMappingHDMI2 = HDMI_SWITCH_2;
	}
	if (DRVCUST_OptQuery(eHDMIPortMappingHDMI3, &dUIHDMIPortMappingHDMI3)!=0)
	{
		dUIHDMIPortMappingHDMI3=HDMI_SWITCH_3;
	}
	if (DRVCUST_OptQuery(eHDMIPortMappingHDMI4, &dUIHDMIPortMappingHDMI4)!=0)
	{
		dUIHDMIPortMappingHDMI4=HDMI_SWITCH_4;
	}

	if (ePort > HDMI_SWITCH_INIT && ePort < HDMI_SWITCH_MAX)
	{
	switch(ePort)
	{
		case HDMI_SWITCH_1:
			return _arHdmiRx[dUIHDMIPortMappingHDMI1]._u1Tx5VActive;//bHDMIPort5VStatus(dUIHDMIPortMappingHDMI1);
		case HDMI_SWITCH_2:
			return _arHdmiRx[dUIHDMIPortMappingHDMI2]._u1Tx5VActive;//bHDMIPort5VStatus(dUIHDMIPortMappingHDMI2);
		case HDMI_SWITCH_3:
			return _arHdmiRx[dUIHDMIPortMappingHDMI3]._u1Tx5VActive;//bHDMIPort5VStatus(dUIHDMIPortMappingHDMI3);
		case HDMI_SWITCH_4:
			return _arHdmiRx[dUIHDMIPortMappingHDMI4]._u1Tx5VActive;//bHDMIPort5VStatus(dUIHDMIPortMappingHDMI4);
		default:
			return 0;
	}

	}
	else
	{
		return (0);
	}

#elif  defined(CC_MT5389)

	UINT32 dUIHDMIPortMappingHDMI1;
	UINT32 dUIHDMIPortMappingHDMI2;
	UINT32 dUIHDMIPortMappingHDMI3;


	if (DRVCUST_OptQuery(eHDMIPortMappingHDMI1, &dUIHDMIPortMappingHDMI1) != 0)
	{
		dUIHDMIPortMappingHDMI1 = HDMI_SWITCH_1;
	}
	if (DRVCUST_OptQuery(eHDMIPortMappingHDMI2, &dUIHDMIPortMappingHDMI2)!=0)
	{
		dUIHDMIPortMappingHDMI2 = HDMI_SWITCH_2;
	}
	if (DRVCUST_OptQuery(eHDMIPortMappingHDMI3, &dUIHDMIPortMappingHDMI3)!=0)
	{
		dUIHDMIPortMappingHDMI3 = HDMI_SWITCH_3;
	}

	if (ePort > HDMI_SWITCH_INIT && ePort < HDMI_SWITCH_MAX)
	{
	switch(ePort)
	{
		case HDMI_SWITCH_1:
			return bHDMIPort5VStatus((E_HDMI_SWITCH_NUM)dUIHDMIPortMappingHDMI1);
		case HDMI_SWITCH_2:
			return bHDMIPort5VStatus((E_HDMI_SWITCH_NUM)dUIHDMIPortMappingHDMI2);
		case HDMI_SWITCH_3:
			return _arHdmiRx[dUIHDMIPortMappingHDMI3]._u1Tx5VActive;
		default:
			return 0;
	}

	}
	else
	{
		return (0);
	}
#elif defined(CC_MT5881)
	UINT32 dUIHDMIPortMappingHDMI1;
	UINT32 dUIHDMIPortMappingHDMI2;
	UINT32 dUIHDMIPortMappingHDMI3;


	if (DRVCUST_OptQuery(eHDMIPortMappingHDMI1, &dUIHDMIPortMappingHDMI1) != 0)
	{
		dUIHDMIPortMappingHDMI1 = HDMI_SWITCH_1;
	}
	if (DRVCUST_OptQuery(eHDMIPortMappingHDMI2, &dUIHDMIPortMappingHDMI2)!=0)
	{
		dUIHDMIPortMappingHDMI2 = HDMI_SWITCH_2;
	}
	if (DRVCUST_OptQuery(eHDMIPortMappingHDMI3, &dUIHDMIPortMappingHDMI3)!=0)
	{
		dUIHDMIPortMappingHDMI3 = HDMI_SWITCH_3;
	}

	if (ePort > HDMI_SWITCH_INIT && ePort < HDMI_SWITCH_MAX)
	{
	switch(ePort)
	{
		case HDMI_SWITCH_1:
			return _arHdmiRx[dUIHDMIPortMappingHDMI1]._u1Tx5VActive;
		case HDMI_SWITCH_2:
			return bHDMIPort5VStatus((E_HDMI_SWITCH_NUM)dUIHDMIPortMappingHDMI2);
		case HDMI_SWITCH_3:
			return bHDMIPort5VStatus((E_HDMI_SWITCH_NUM)dUIHDMIPortMappingHDMI3);;
		default:
			return 0;
	}

	}
	else
	{
		return (0);
	}
#elif defined(CC_MT5399) || defined(CC_MT5882)
	UINT32 dUIHDMIPortMappingHDMI1;
	UINT32 dUIHDMIPortMappingHDMI2;
	UINT32 dUIHDMIPortMappingHDMI3;
    #ifndef CC_MT5882
	UINT32 dUIHDMIPortMappingHDMI4;
    #endif
	if (DRVCUST_OptQuery(eHDMIPortMappingHDMI1, &dUIHDMIPortMappingHDMI1) != 0)
	{
		dUIHDMIPortMappingHDMI1 = HDMI_SWITCH_1;
	}
	if (DRVCUST_OptQuery(eHDMIPortMappingHDMI2, &dUIHDMIPortMappingHDMI2)!=0)
	{
		dUIHDMIPortMappingHDMI2 = HDMI_SWITCH_2;
	}
	if (DRVCUST_OptQuery(eHDMIPortMappingHDMI3, &dUIHDMIPortMappingHDMI3)!=0)
	{
		dUIHDMIPortMappingHDMI3 = HDMI_SWITCH_3;
	}
	#ifndef CC_MT5882
	if (DRVCUST_OptQuery(eHDMIPortMappingHDMI4, &dUIHDMIPortMappingHDMI4)!=0)
	{
		dUIHDMIPortMappingHDMI4 = HDMI_SWITCH_4;
	}
	#endif
	if (ePort > HDMI_SWITCH_INIT && ePort < HDMI_SWITCH_MAX)
	{
	switch(ePort)
	{
		case HDMI_SWITCH_1:
#ifndef DEMO_BOARD
			return u1GetHDMIPort5VStatus((E_HDMI_SWITCH_NUM)dUIHDMIPortMappingHDMI1);//
#else
			return bHDMIPort5VStatus((E_HDMI_SWITCH_NUM)dUIHDMIPortMappingHDMI1);

#endif
		case HDMI_SWITCH_2:
#ifndef DEMO_BOARD
			return u1GetHDMIPort5VStatus((E_HDMI_SWITCH_NUM)dUIHDMIPortMappingHDMI2);
#else
			return bHDMIPort5VStatus((E_HDMI_SWITCH_NUM)dUIHDMIPortMappingHDMI2);
#endif

		case HDMI_SWITCH_3:
#ifndef DEMO_BOARD
			return u1GetHDMIPort5VStatus((E_HDMI_SWITCH_NUM)dUIHDMIPortMappingHDMI3);
#else
			return bHDMIPort5VStatus((E_HDMI_SWITCH_NUM)dUIHDMIPortMappingHDMI3);
#endif
#ifndef CC_MT5882
		case HDMI_SWITCH_4:
#ifndef DEMO_BOARD
			return u1GetHDMIPort5VStatus((E_HDMI_SWITCH_NUM)dUIHDMIPortMappingHDMI4);
#else
			return bHDMIPort5VStatus((E_HDMI_SWITCH_NUM)dUIHDMIPortMappingHDMI4);
#endif
#endif
		default:
			return 0;

	}

	}
	else
	{
		return (0);
	}	  
#endif
}

/**
 * @brief 	HDMI Mainloop
 * @param	None
 * @retval	None
 */
static UINT8 bSwitchHDMIMHLRG = 0;
extern void _Cec_Notify5VStatus(UINT8 u1Port, UINT8 u15vStatus);
#ifdef CC_FOR_POC_DEMO
extern void  _Cec_PhilipsNotify5VStatus(UINT8 u1Port, UINT8 u15vStatus);
#endif
void vHDMIMainLoop(void)
{
    //UINT8 bReadData;//,bPHYCount;
    //UINT8 bI2CWriteData;
#if (DEFAULTKEYS == 0)
    //UINT32 i4_ret;
#endif
    static UINT8 bInitHDCP = 0;
    //static UINT8 bResChange = 0;
    HAL_TIME_T rDelta;
#ifndef CC_Support_Sil9285
    HAL_TIME_T rTimeTmp;
#endif
#ifdef CC_SUPPORT_HDMI_CLK5VDET
    HAL_TIME_T rTtmp;
#endif
    HAL_TIME_T rCurTime;
    HAL_TIME_T rCurHPDLowTime;
    E_HDMI_SWITCH_NUM eHdmiPort;
    UINT32 u4TmpHdmiRegBase;

#ifdef CC_SUPPORT_HDMI_FBOOTING

    if(_u4HdmiLoops < 500)
    {
        _u4HdmiLoops++;

        if((_u4HdmiLoops % 30) == 0)
        {
            LOG(6,"## _u4HdmiLoops %d\n",_u4HdmiLoops);
        }
    }

#endif

    if(bInitHDCP == 0)
    {
        bInitHDCP++;
        return;
    }
    else if(bInitHDCP == 1)
    {
#if (DEFAULTKEYS == 0)
        // load HDCP key form EEP to SRAM
        bInitHDCP++;
        /*i4_ret = (UINT32)EEPHDCP_Init();	//need to modify ??

        if(i4_ret != 0)
        {
            LOG(6, "fail on load HDCP\n");
        }

        UNUSED(EEPHDCP_SetSramDevice(1, 0xA0));*/
        vHDMIHDCPRst();

#ifdef SYS_MHL_SUPPORT
#ifdef CC_MHL_HDMI_SWITCH
		vMhlGo();
#endif
#endif
        return;
#endif
    }

        // check for EDID ready
#if (DEFAULTKEYS == 1)
    if (u1EdidInit == 0)
    {
       return;
    }
#endif

    for(eHdmiPort = HDMI_SWITCH_1 ; eHdmiPort < HDMI_SWITCH_MAX ; eHdmiPort++)
    {
        // check if rx available
        if(u1IsHdmiRxAvailable(eHdmiPort) == 0)
        {
            // LOG(3, "Hdmi Rx %d unavailable\n", eHdmiPort);
            continue;
        }

        // check for rx sharing ports
        if(_arHdmiRx[eHdmiPort]._bRxShared == 1)
        {
            // sharing ports and with active port
            if(eHdmiPort != eActiveHdmiPort)
            {
                //     LOG(3, "Hdmi Rx %d is not active port\n", eHdmiPort);
                continue;
            }
        }

        eCurrHdmiPort = eHdmiPort;
        u4TmpHdmiRegBase = u4GetHdmiRegBase(eHdmiPort);
        u4AnalogRegBase = u4GetAnalogRegBase(eHdmiPort);
        u4HdmiRegBase = u4TmpHdmiRegBase;

        if((_arHdmiRx[eHdmiPort]._bHdmiCmd == 6) || \
           (_arHdmiRx[eHdmiPort]._bHdmiCmd == 7) || \
           (_arHdmiRx[eHdmiPort]._bHdmiCmd == 0x40))
        {
            return;
        }

        if (_arHdmiRx[eHdmiPort]._bHdmiCmd == HDMI_DEBUG_HDCP_MONITOR)
        {
            vHdcpMonitor(eHdmiPort);
        }

#if HDMI_Audio_NewFlow
        vHDMIAudErrorHandler();
#endif

#ifdef CC_SUPPORT_HDMI_CLK5VDET

    // check 5V status
    if (bHDMIPort5VStatus(eHdmiPort))
    {
        if (_arHdmiRx[eHdmiPort]._b5VDetCnt < 6)
        {
            _arHdmiRx[eHdmiPort]._b5VDetCnt++;
        }
        else
        {
            _arHdmiRx[eHdmiPort]._b5VActive = 1;
            _arHdmiRx[eHdmiPort]._bNonStdDev = 0;
        }
    }
    else
    {
        if (_arHdmiRx[eHdmiPort]._b5VDetCnt > 5)
        {
            _arHdmiRx[eHdmiPort]._b5VDetCnt--;
        }
        else
        {
            _arHdmiRx[eHdmiPort]._b5VActive = 0;
        }
    }


    // check Sync status
    if (RegReadFldAlign(SRST + u4TmpHdmiRegBase, SCDT))
    {
        if (_arHdmiRx[eHdmiPort]._bTmdsDetCnt < 180)
        {
            _arHdmiRx[eHdmiPort]._bTmdsDetCnt++;
        }

        if (_arHdmiRx[eHdmiPort]._bTmdsDetCnt > 10)
        {
            if (_arHdmiRx[eHdmiPort]._b5VActive == 0)
            {
                _arHdmiRx[eHdmiPort]._bNonStdDev = 1;
            }

            _arHdmiRx[eHdmiPort]._bTmdsActive = 1;
        }
    }
    else
    {
        if (_arHdmiRx[eHdmiPort]._bTmdsDetCnt > 0)
        {
            _arHdmiRx[eHdmiPort]._bTmdsDetCnt--;
        }
        else
        {
            _arHdmiRx[eHdmiPort]._bTmdsActive = 0;
            _arHdmiRx[eHdmiPort]._bNonStdDev = 0;
        }
    }

    // HPD pulse
    if (_arHdmiRx[eHdmiPort]._bHpdPulse == 0)
    {
        HAL_GetTime(&rTtmp);
        HAL_GetDeltaTime(&rDelta, &_arHdmiRx[eHdmiPort]._rHdmiUnplugTime, &rTtmp);
        _arHdmiRx[eHdmiPort]._wDeltaCur = (1000000 * rDelta.u4Seconds) + rDelta.u4Micros;

        if (_arHdmiRx[eHdmiPort]._b5VActive == 0)
        {
            _arHdmiRx[eHdmiPort]._wHpdCnt = 2000000;  //us
        }
        else
        {
            _arHdmiRx[eHdmiPort]._wHpdCnt = _wHDMI_OFFON_MUTE_COUNT * 1000;
        }

        if (_arHdmiRx[eHdmiPort]._wDeltaCur > _arHdmiRx[eHdmiPort]._wHpdCnt)
        {
            vHDMIDDCBusCTRL(TRUE);
            vHDMITMDSCTRL(eHdmiPort, TRUE);
            vHDMIHPDHigh(eHdmiPort);
            _arHdmiRx[eHdmiPort]._bHpdPulse = 1;
        }
        else
        {
            return;
        }

    }

    if (_arHdmiRx[eHdmiPort]._b5VActive || ((_arHdmiRx[eHdmiPort]._bNonStdDev == 1) && _arHdmiRx[eHdmiPort]._bTmdsActive))
    {
        if (_arHdmiRx[eHdmiPort]._bHDMIState == HDMI_STATE_PWOFF)
        {
            LOG(6, "HDMI_STATE_PWON @ CLK_DET\n");
            _arHdmiRx[eHdmiPort]._bHDMIState = HDMI_STATE_PWON;
	     _arHdmiRx[eHdmiPort]._bResStableCnt = 0;
            vClrHdmiUnplug(eHdmiPort);
        }
    }
    else
    {
        if (_arHdmiRx[eHdmiPort]._bHDMIState == HDMI_STATE_PWOFF)
        {
            if (eHdmiPort >= HDMI_SWITCH_MAX)
            {
               // LOG(3, "Hdmi Rx %d  return\n", eHdmiPort);
            	return;
        	}
        	else
        	{
               // LOG(3, "Hdmi Rx %d  continue\n", eHdmiPort);
                continue;
            }
        }
        else
        {
            LOG(6, "HDMI_STATE_INIT  @ CLK_DET\n");
            _arHdmiRx[eHdmiPort]._bHDMIState = HDMI_STATE_INIT;
	     _arHdmiRx[eHdmiPort]._bResStableCnt = 0;
            vSetHdmiUnplug(eHdmiPort);
        }

    }
#else
        //-----------------------------------5V detect ----------------------------------------
		// check Sync status for covering non-std device
		if ((RegReadFldAlign(SRST + u4TmpHdmiRegBase, SCDT)) && (RegReadFldAlign(SRST + u4TmpHdmiRegBase, CKDT)) && (bHDMIPort5VStatus(eHdmiPort) == 0))
		{
			if (_arHdmiRx[eHdmiPort]._bTmdsDetCnt < 180)
			{
				_arHdmiRx[eHdmiPort]._bTmdsDetCnt++;
			}

			if (_arHdmiRx[eHdmiPort]._bTmdsDetCnt > 10)
			{
				if (bHDMIPort5VStatus(eHdmiPort) == 0)
				{
					_arHdmiRx[eHdmiPort]._bNonStdDev = 1;
				}
			}
		}
		else
		{
			if (_arHdmiRx[eHdmiPort]._bTmdsDetCnt > 0)
			{
				_arHdmiRx[eHdmiPort]._bTmdsDetCnt--;
			}
			else
			{
				_arHdmiRx[eHdmiPort]._bNonStdDev = 0;
			}
		}


		if(u1IsHdmiHpdPulse(eHdmiPort) == 1)
        {
            // LOG(3, "Hdmi Rx %d  hpd pulse\n", eHdmiPort);
            // hpd pulse, do nothing
        }
        else if((bHDMIPort5VStatus(eHdmiPort) == 0) && (_arHdmiRx[eHdmiPort]._bNonStdDev == 0))
        {
			#ifdef CC_FOR_POC_DEMO
				_Cec_PhilipsNotify5VStatus(eHdmiPort,0);
			#else
				#ifdef CECNFY5VSTATUS
				_Cec_Notify5VStatus(eHdmiPort,0);
				#endif
			#endif
            _arHdmiRx[eHdmiPort]._u1Tx5VActive = 0;

            if(_arHdmiRx[eHdmiPort]._bHDMIState == HDMI_STATE_PWOFF)
            {
                if(_arHdmiRx[eHdmiPort]._u8SkipHpd == 0)
                {
                    HAL_GetTime(&rTimeTmp);
                    HAL_GetDeltaTime(&rDelta, &_arHdmiRx[eHdmiPort]._rHdmiUnplugTime, &rTimeTmp);

                    if(rDelta.u4Seconds >= 1)
                    {
                        _arHdmiRx[eHdmiPort]._u8SkipHpd = 1;
                        // for less power consumption at standby mode
                        vHDMIHPDHigh(eHdmiPort);
                    }
                }

                if(eHdmiPort >= HDMI_SWITCH_MAX)
                {
                    // LOG(3, "Hdmi Rx %d  return\n", eHdmiPort);
                    return;
                }
                else
                {
                    // LOG(3, "Hdmi Rx %d  continue\n", eHdmiPort);
                    continue;
                }
            }
            else
            {
                LOG(6, "Port %d HDMI_STATE_INIT #1\n",eHdmiPort);
            HAL_GetTime(&_arHdmiRx[eHdmiPort]._rHdmiUnplugTime);
                _arHdmiRx[eHdmiPort]._bHDMIState = HDMI_STATE_INIT;
                _arHdmiRx[eHdmiPort]._bResStableCnt = 0;
                vSetHdmiUnplug(eHdmiPort);
                // mute AV here
            }
        }
        else
        {

		 #ifdef CC_FOR_POC_DEMO
		 	_Cec_PhilipsNotify5VStatus(eHdmiPort,1);
		 #else
		 	#ifdef CECNFY5VSTATUS
			_Cec_Notify5VStatus(eHdmiPort,1);
		 	#endif
		 #endif
        if (_arHdmiRx[eHdmiPort]._bHDMIState == HDMI_STATE_PWOFF)
        {
			if ((_bIsMhlDevice == 1) && (eHdmiPort == HDMI_SWITCH_1))
            {
	            _arHdmiRx[eHDMICurrSwitch]._u1Tx5VActive = 1;

	            if(_arHdmiRx[eHDMICurrSwitch]._bHDMIState == HDMI_STATE_PWOFF)
	            {
	                _arHdmiRx[eHDMICurrSwitch]._bHDMIState = HDMI_STATE_PWON;
	                _arHdmiRx[eHDMICurrSwitch]._bResStableCnt = 0;
					LOG(6, "eHDMICurrSwitch %d HDMI_STATE_PWON\n",eHDMICurrSwitch);

	                if (eHDMICurrSwitch != eActiveHdmiPort)
	                {
	                    vClrHdmiUnplug(eHdmiPort);
	                }
					continue;
	            }
	        }

            HAL_GetTime(&rTimeTmp);
	        if(bHPDInitFlag == 0)
	        {
	           HAL_GetDeltaTime(&rDelta,&bInitUnplugTime,&rTimeTmp);
	           bHPDInitFlag = 1;
	        }
	        else
	        {
	             HAL_GetDeltaTime(&rDelta, &_arHdmiRx[eHdmiPort]._rHdmiUnplugTime, &rTimeTmp);
	        }
            if (rDelta.u4Micros >= 500)
            {
	            _arHdmiRx[eHdmiPort]._u1Tx5VActive = 1;

	            if(_arHdmiRx[eHdmiPort]._bHDMIState == HDMI_STATE_PWOFF)
	            {
	                  _arHdmiRx[eHdmiPort]._u1Tx5VActive = 1;
	                _arHdmiRx[eHdmiPort]._bHDMIState = HDMI_STATE_PWON;
	                _arHdmiRx[eHdmiPort]._bResStableCnt = 0;
	                //   HAL_GetTime(&_arHdmiRx[eHdmiPort]._rHdmiLowPlugWaitTime);
	                  LOG(6, "Port %d HDMI_STATE_PWON\n",eHdmiPort);

	                if (eHdmiPort != eActiveHdmiPort)
	                {
	                    vClrHdmiUnplug(eHdmiPort);
	                }

	            }
       		}
        }
    }
#endif

        /*---------CKDT monitor----------------*/

        if(!IO32ReadFldAlign(SRST + u4TmpHdmiRegBase, CKDT))
        {
            vHdmiSaveTmdsClkRate(eHdmiPort, TMDS_CLOCK_UNKNOW);

            if(_arHdmiRx[eHdmiPort]._bCKDTMonitor == 0)
            {
                //Printf("HDMI CKDT lost ........\n");
                _arHdmiRx[eHdmiPort]._bCKDTMonitor = 1;
            }
        }
        else
        {
            vHdmiPhySetting(eHdmiPort);

            if(u1IsHdmiEqTracking(eHdmiPort))
            {
                vHdmiEqTracking(eHdmiPort);

                // check if timing stable
                if(u1IsHdmiResStable(eHdmiPort))
                {
                    vSetHdmiEqTracking(eHdmiPort, 0);
                }
            }

            // if CLK change
            if(IO32ReadFldAlign(INTR_STATE0 + u4TmpHdmiRegBase, INTR2_CLK_CHG))
            {
                bHdmiPhyReset(eHdmiPort,HDMI_RST_ALL);
                vUtDelay2us(50);
                vIO32Write4B(INTR_STATE0 + u4TmpHdmiRegBase, Fld2Msk32(INTR2_CLK_CHG));
                //         LOG(3, "\n>>>> DVI_WAIT_STABLE #1, HDMI_RTCK_AUTO Toggle\n");
                _arHdmiRx[eHdmiPort]._u1ClkChg = 1;
            }

            // check if HRES change
            if(IO32ReadFldAlign(INTR_STATE1 + u4TmpHdmiRegBase, INTR5_HRESCHG))
            {
                bHdmiPhyReset(eHdmiPort, HDMI_RST_DEEPCOLOR);
                vIO32Write4B(INTR_STATE1 + u4TmpHdmiRegBase, Fld2Msk32(INTR5_HRESCHG));
            }
            else if(IO32ReadFldAlign(VID_HRES + u4TmpHdmiRegBase, VID_HRES_STB) && _arHdmiRx[eHdmiPort]._u1ClkChg == 1)
            {
                _arHdmiRx[eHdmiPort]._u1ClkChg = 0;
                bHdmiPhyReset(eHdmiPort, HDMI_RST_DEEPCOLOR);
                //           LOG(3, "\n>>>> DVI_WAIT_STABLE #5, HDMI_RTCK_AUTO Toggle\n");
            }
        }

		if((_bIsMhlDevice == 1) && (eHdmiPort == HDMI_SWITCH_1))
		{
			vMHLAdjustChannel();
		}
		
		#if defined(CC_MT5882)    //just for Capri Fast boot issue
		if((eActiveHdmiPort == HDMI_SWITCH_1) && (_bIsMhlDevice == 1) && (vCbusGetState() == MHL_STATE_CONNECTED))
		{
			vIO32WriteFldAlign(REG_HDMI_RX_Normal_0_CFG2 + u4GetAnalogRegBase(HDMI_SWITCH_1), 0, RG_HDMI_0_FTMODE);
		}
        #endif
		
		if((eActiveHdmiPort == HDMI_SWITCH_1)||(eActiveHdmiPort == HDMI_SWITCH_INIT))
		{
			if((_bIsMhlDevice == 1) && (bSwitchHDMIMHLRG == 0))
			{
				bSwitchHDMIMHLRG = 1;
			}

			if(_bIsMhlDeviceold != _bIsMhlDevice)
			{
				_bIsMhlDeviceold = _bIsMhlDevice;
				bSwitchHDMIMHLRG = 1;
			}

			if(_bIsMhlPPModeold != _bIsMhlPPMode)
			{
				_bIsMhlPPModeold = _bIsMhlPPMode;
				bSwitchHDMIMHLRG = 1;
			}

			if(bSwitchHDMIMHLRG == 1)
			{
				vHdmiSaveTmdsClkRate(HDMI_SWITCH_1, TMDS_CLOCK_UNKNOW);
				vHdmiPhySetting(HDMI_SWITCH_1);

				LOG(6, "bSwitchHDMIMHLRG\n");
				bSwitchHDMIMHLRG = 2;
			}

		}
		else
		{
			bSwitchHDMIMHLRG = 0;
		}
        //--------------------------------------------------------------------------
        vXpcStableCount();
        vLogHdmiStateChange(eHdmiPort, _arHdmiRx[eHdmiPort]._bHDMIState);

        switch(_arHdmiRx[eHdmiPort]._bHDMIState)
        {
            case HDMI_STATE_INIT:
#ifdef CC_Support_Sil9285
                vHDMITMDSCTRL(eHdmiPort,TRUE);
#else
                vHDMIHPDLow(eHdmiPort);
                vHDMITMDSCTRL(eHdmiPort,FALSE);
                vHDMIDDCBusCTRL(FALSE);
#endif

                vHDMISwRst();
                vHDMIHDCPRst();
#if MUTE_TEMP
                vHDMIVideoOutOff();
#endif
                _arHdmiRx[eHdmiPort]._bHdmiSyncActive = 0;
                _arHdmiRx[eHdmiPort]._bHDMIFastSW = 0;
                _arHdmiRx[eHdmiPort]._bResStableCnt = 0;
                _arHdmiRx[eHdmiPort]._bTxSTBY = 0;
                _arHdmiRx[eHdmiPort]._bNeedGetNoSyncStartTime = 1;

#ifdef CC_SUPPORT_HDMI_FBOOTING
                vSetHdmiSrcIn(eHdmiPort, 0);
                _arHdmiRx[eHdmiPort]._u1HdcpGood = 0;
#endif
                vHDMIMuteAudio();
                vHDMIAudConfig();
                // Toggle TMDS_SRS_SEL bit 0 (PCLK_INV) to force HDMI output audio MCLK
                // This workaround is for MT537X that have clock switching problem
                //vHDMIApllClkReset();
                _arHdmiRx[eHdmiPort]._u8SkipHpd = 0;
                HAL_GetTime(&_arHdmiRx[eHdmiPort]._rHdmiUnplugTime);
                HAL_GetTime(&_arHdmiRx[eHdmiPort]._rHdmiLowPlugWaitTime);

                _arHdmiRx[eHdmiPort]._bHDMIState = HDMI_STATE_PWOFF;
#if defined(CC_SUPPORT_HDMI_CLK5VDET)
                _arHdmiRx[eHdmiPort]._bHpdPulse = 0;
                //vIO32WriteFldAlign(TMDS_CTRL0 + u4HdmiRegBase, 1, HDMI_RTCK_AUTO);
#endif
#if HDMI_DEBUG
                LOG(6, "HDMI_STATE_PWOFF\n");
#endif
                break;

            case HDMI_STATE_PWOFF:
                if(u1IsHdmiHpdPulse(eHdmiPort) == 1)
                {
                    HAL_GetTime(&rCurHPDLowTime);
                    HAL_GetDeltaTime(&rDelta, &_arHdmiRx[eHdmiPort]._rHdmiLowPlugWaitTime, &rCurHPDLowTime);

                    if(((1000000*rDelta.u4Seconds) + rDelta.u4Micros) > ((330)*1000))  // 330ms
                    {
						if((_bIsMhlDevice == 1) && (eHdmiPort == HDMI_SWITCH_1))
						{
							LOG(6, "Jump tmds ctrl, it was controlled by cbus when in MHL port\n");
						}
						else
						{
							vHDMITMDSCTRL(eHdmiPort,TRUE);
						}
                        vHDMIDDCBusCTRL(TRUE);
                    }

                    if(((1000000*rDelta.u4Seconds) + rDelta.u4Micros) > ((_wHDMI_OFFON_MUTE_COUNT + 0)*1000))
                    {
                        vHDMIHPDHigh(eHdmiPort);
                        vHDMIDDCBusCTRL(TRUE);
                        vHdmiSetHpdStatus(eHdmiPort, 0);
                    }
                }

                break;

            case HDMI_STATE_PWON:
#if defined(CC_SUPPORT_HDMI_CLK5VDET)
                _arHdmiRx[eHdmiPort]._bHDMIState = HDMI_STATE_PWON2;
#else
                vIO32WriteFldAlign(TMDS_CTRL0 + u4TmpHdmiRegBase, 1, C_DATA_SYNC_AUTO);
                vHDMIDDCBusCTRL(TRUE);
				vHDMIEQCIRCUITCTRL(eHdmiPort,TRUE);
				if((_bIsMhlDevice == 1) && (eHdmiPort == HDMI_SWITCH_1))
				{
					LOG(6, "Jump tmds ctrl, it was controlled by cbus when in MHL port\n");
				}
				else
				{
					vHDMITMDSCTRL(eHdmiPort,TRUE);
				}
#if 1
                _arHdmiRx[eHdmiPort]._bHDMIState = HDMI_STATE_PWON2;
#else
                HAL_GetTime(&rCurHPDLowTime);
                HAL_GetDeltaTime(&rDelta, &_arHdmiRx[eHdmiPort]._rHdmiLowPlugWaitTime, &rCurHPDLowTime);

                if(((1000000*rDelta.u4Seconds) + rDelta.u4Micros) > ((330)*1000) || (_arHdmiRx[eHdmiPort]._u8SkipHpd == 1))  // 340ms
                {
                    vHDMITMDSCTRL(eHdmiPort,TRUE);
                }

                if((((1000000*rDelta.u4Seconds) + rDelta.u4Micros) > ((_wHDMI_OFFON_MUTE_COUNT + 0)*1000)) || (_arHdmiRx[eHdmiPort]._u8SkipHpd == 1))
                {
                    //vIO32Write1B(SYS_CTRL_1, 0x95);// enable DDCDLY DDC_EN RX_EN
                    vHDMIDDCBusCTRL(TRUE);
                    vHDMITMDSCTRL(eHdmiPort,TRUE);
                    _arHdmiRx[eHdmiPort]._bHDMIState = HDMI_STATE_PWON2;
                }

#endif
#if HDMI_DEBUG
                LOG(6, "HDMI_STATE_PWON\n");
#endif
                vHDMIMuteAudio();
#endif
                break;

            case HDMI_STATE_PWON2:
                vHDMIHPDHigh(eHdmiPort);
                _arHdmiRx[eHdmiPort]._bHDMIState = HDMI_STATE_SCDT;
                vHdmiSaveTmdsClkRate(eHdmiPort,TMDS_CLOCK_UNKNOW);
#if HDMI_DEBUG
                LOG(6, "HDMI_STATE_PWON2\n");
#endif
                break;

            case HDMI_STATE_SCDT:
                if (RegReadFldAlign(SRST + u4HdmiRegBase, SCDT) && \
				            RegReadFldAlign(SRST + u4HdmiRegBase, CKDT))
                {

                    _arHdmiRx[eHdmiPort]._bTxSTBY = 0;

                    HAL_GetTime(&rCurTime);

                    // vIO32Write4B(INTR_STATE0 + u4HdmiRegBase, 0x1 << 16);

                    if(!_arHdmiRx[eHdmiPort]._bHdmiSyncActive)
                    {
                        _arHdmiRx[eHdmiPort]._bHdmiSyncActive = 1;
                        HAL_GetTime(&_arHdmiRx[eHdmiPort]._rHdmiSyncWaitTime);
                    }

                    vHDMIAudConfig();
                    vHDMIMuteAudio();
                    vHDMIVideoOutOff();
                    vHdcpStableMonitor(eHdmiPort);
                    vHdmiStableMonitor(eHdmiPort);

                    if(((rCurTime.u4Seconds - _arHdmiRx[eHdmiPort]._rHdmiSyncWaitTime.u4Seconds) > (_wHDMI_WAIT_SCDT_STABLE_COUNT))	 || \
                       u1IsHdmiResStable(eHdmiPort)  || u1IsHdcpStable(eHdmiPort))
                    {
                        LOG(6, "Port %d eHdmiPortHDMI_STATE_AUTH wstable sec: %d,ResStable %d hflag: %d, fastSw %d\n", \
                            eHdmiPort,rCurTime.u4Seconds - _arHdmiRx[eHdmiPort]._rHdmiSyncWaitTime.u4Seconds, \
                            _arHdmiRx[eHdmiPort]._bResStableCnt,\
                            _arHdmiRx[eHdmiPort]._u1HdcpGood, \
                            _arHdmiRx[eHdmiPort]._bHDMIFastSW);
                        _arHdmiRx[eHdmiPort]._bSCDTdelay = 0;
                        _arHdmiRx[eHdmiPort]._bAvMuting = 0;
						vMHLSETRAPContentOnoff(1);
                        _arHdmiRx[eHdmiPort]._bHDMIState = HDMI_STATE_AUTH;
#ifdef CC_SUPPORT_HDMI_FBOOTING
                        vSetHdmiSrcIn(eHdmiPort, 1);
#endif

                        // ??
                        if(eHdmiPort == eActiveHdmiPort)
                        {
                            //when 3x3 matrix is RGB but player output is Ycbcr, AVI infofrmae has changed  to Ycbcr but FW doesn't change
                            if(u1IO32Read1B(AVIRX0_0 + u4ActiveHdmiRegBase) == 0x82)
                            {
                                _bAVIInfo_tmp = u1IO32Read1B(AVIRX1_0 + u4ActiveHdmiRegBase);
                                vIO32Write4B(INTR_STATE0 + u4ActiveHdmiRegBase, 0x1 << 24);
                                _bNEW_AVI_Info = 1;
                            }
                        }

                    }
                }
                else
                {
                    // 5V active but no sync present
                    if (_arHdmiRx[eHdmiPort]._bTxSTBY == 0)
                    {
                        HAL_GetTime(&rCurTime);
                        if ((rCurTime.u4Seconds - _arHdmiRx[eHdmiPort]._rHdmiNoSyncStartTime.u4Seconds) > 5)
                        {
                         _arHdmiRx[eHdmiPort]._bTxSTBY = 1;
                        }
                    }
                    if(_arHdmiRx[eHdmiPort]._bNeedGetNoSyncStartTime)
                    {
                        _arHdmiRx[eHdmiPort]._bNeedGetNoSyncStartTime = 0;
                        HAL_GetTime(&_arHdmiRx[eHdmiPort]._rHdmiNoSyncStartTime);                                       
                    }
                    _arHdmiRx[eHdmiPort]._bHdmiSyncActive = 0; 

                }

                break;

            case HDMI_STATE_AUTH:

	         if (eHdmiPort == eActiveHdmiPort)
    	         {
	             vHdmiVsdbInfoCheck();
				 vHDMIGetSPDData();
	         }

                vHdcpStableMonitor(eHdmiPort);
                vHdmiStableMonitor(eHdmiPort);

                 vHdmiAvMuteMonitor(eHdmiPort);

                if(_bDviModeChged)
                {
                    vHDMIMuteAudio();
                }

                if((!(IO32ReadFldAlign(SRST + u4HdmiRegBase, SCDT))) || \
                   (!(IO32ReadFldAlign(SRST + u4HdmiRegBase, CKDT))))
                {
                    if(_arHdmiRx[eHdmiPort]._bSCDTdelay++)
                    {
                        
                        u1HWmute = 1;
						_u1mutecount = 0;
                        vHdmiMute(eHdmiPort);
                        vHDMIMuteAudio();
                        bHdmiPhyReset(eHdmiPort,HDMI_RST_ALL);
                        vHdmiSaveTmdsClkRate(eHdmiPort, TMDS_CLOCK_UNKNOW);
                        // reset to clean timing info
                        vIO32WriteFldAlign(SRST + u4HdmiRegBase,1,DEEP_RST);
                        vUtDelay2us(1);
                        vIO32WriteFldAlign(SRST + u4HdmiRegBase,0,DEEP_RST);
                        //Set the SyncLose Flag
                        vSetSyncLose(eHdmiPort);

	             vHDMISwRst();

#if HDMI_DEBUG
                        LOG(6, "SCDT =%x\n", IO32ReadFldAlign(SRST + u4HdmiRegBase, SCDT));
                        LOG(6, "HDMI_STATE_SCDT #2\n");
#endif
                        _arHdmiRx[eHdmiPort]._bHDMIState = HDMI_STATE_SCDT;
#ifdef CC_SUPPORT_HDMI_FBOOTING
                        vSetHdmiSrcIn(eHdmiPort, 0);
                        _arHdmiRx[eHdmiPort]._u1HdcpGood = 0;
                        _u4HdmiLoops = 0;
                        _arHdmiRx[eHdmiPort]._bHdmiSyncActive = 0;
                        _arHdmiRx[eHdmiPort]._bResStableCnt = 0;
#endif
                    }

                    break;
                }
                else
                {
                    _arHdmiRx[eHdmiPort]._bSCDTdelay = 0;
                }

                // Check HDMI Mode
                _bHdmiMode = u1IO32Read1B(AUDP_STAT_0 + u4ActiveHdmiRegBase) & 0x1;
                _bHdmiModeActive = u1IO32Read1B(AUDP_STAT_0 + u4ActiveHdmiRegBase) & 0x1;

                if(!_bHdmiMode && IO32ReadFldAlign(AUDP_STAT + u4HdmiRegBase, HDMI_MUTE))
                {
                    vIO32WriteFldAlign(SRST + u4HdmiRegBase, 1,  SW_RST);
                    vIO32WriteFldAlign(SRST + u4HdmiRegBase, 0,  SW_RST);
                }

                if(eHdmiPort == eActiveHdmiPort)
                {
   		             if((eActiveHdmiPort == HDMI_SWITCH_1) && (_bIsMhlDevice == 1)&& (vMHLGETRAPContentOnoff() == 0))
                	{
						LOG(6, "rap conten off, didnot turn on again\n");
					}
					else 
					{
					        _u1mutecount ++;
					        if(((_u1mutecount > 45)&&(u1HWmute == 1))||(u1HWmute == 0))
					        {
					            _u1mutecount = 0;
								u1HWmute = 0;
					            vHDMIVideoOutOn();	
					        }
					}

                    if((u1IO32Read1B(ACPRX0_0 + u4ActiveHdmiRegBase) == 0x4) && (u1IO32Read1B(ACPRX0_1 + u4ActiveHdmiRegBase) > 1))
                    {
                        //LOG(3, " INTR6_NEW_ACP=%d\n",IO32ReadFldAlign(INTR_STATE1,INTR6_NEW_ACP));
                        if(IO32ReadFldAlign(INTR_STATE1 + u4ActiveHdmiRegBase, INTR6_NEW_ACP))
                        {
                            _bACPCount = 0;
                            vIO32Write4B(INTR_STATE1 + u4ActiveHdmiRegBase, Fld2Msk32(INTR6_NEW_ACP));
                            //LOG(3, " INTR6_NEW_ACP in mute=%d\n",IO32ReadFldAlign(INTR_STATE1,INTR6_NEW_ACP));
                            //vHDMIMuteAudio();
                            vCloseIec();
                            //LOG(3, "ACP >=2 ,Mute now\n");
                        }
                        else
                        {
                            _bACPCount++;
                        }

                        //LOG(3, " _bACPCount=%d\n",_bACPCount);
                        if(_bACPCount > 50)
                        {
                            vOpenIec();
                            _bACPCount = 60;
                        }
                    }

                    if(_bHdmiModeActive)
                    {
                        // Check audio underrun & overrun
                        vHDMIHandleAudFifoFault();
                        //_bACPCount=0;
                        // Move to bHDMIAudStatus() to avoid race condition between audio/hdmi thread
                        //LOG(3, "ACP <2 ,UNMute now\n");
                    }

                    vHDMISetColorRalated();
                }

                break;

            default:
                _arHdmiRx[eHdmiPort]._bHDMIState = HDMI_STATE_INIT;
                break;
        }
    }
}

BOOL fgHDMIHsyncAct(void)
{

    if (RegReadFldAlign(SRST + u4ActiveHdmiRegBase, SCDT))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

#ifdef __HDMI_SLT__
// CRC check for all
#define CRC_HDMI_PORTS 4
BOOL fgHDMICRCMulti(INT16 ntry)
{
    UINT8 idx[CRC_HDMI_PORTS];
    UINT8 result[CRC_HDMI_PORTS][3][3];
    UINT8 tmp[CRC_HDMI_PORTS][3];
    UINT8 i;
    UINT32 u4RegBase = 0x400;
    UINT8 u1Ret = 0;

    for ( i = 0 ; i < 4 ; i++)
    {
        idx[i] = 0;

        result[i][0][0] = 0;
        result[i][0][1] = 0;
        result[i][0][2] = 0;
        result[i][1][0] = 0;
        result[i][1][1] = 0;
        result[i][1][2] = 0;

        tmp[i][0] = 0;
        tmp[i][1] = 0;
        tmp[i][2] = 0;
    }

    LOG(6, "fgHDMICRC: %d\n", ntry);

    if (ntry == 0)
    {
        LOG(0, "zero loop %d\n", ntry);
    }

    while (ntry > 0)
    {
        ntry--;

        vUtDelay10ms(1); // NOTE: IT IS NECESSARY

#if defined(CC_MT5880)
        for ( i = 1 ; i < 4 ; i++)
#else
		for ( i = 0 ; i < 4 ; i++)
#endif
       {
            if (RegReadFldAlign(SRST + u4RegBase*i, SCDT) == 0)
            {
                LOG(0, "port %d sync not active, fail\n", i);
                //return 0;
            }

            if (idx[i] > 2)
            {
                LOG(0, "CRC fail %d\n", i);
                return 0;
            }

			//fix change resolution, multi crc fail
			vIO32WriteFldAlign(AUDP_STAT + u4RegBase*i, 0, AUDP_VIDEO_MUTE);
            vRegWrite1B((VID_CRC_CHK_2 + u4RegBase*i), 0x8c);// clr
            LOG(6, "CRC clear status %d\n",i);

            while (RegReadFldAlign(VID_CRC_CHK + u4RegBase*i, STA_CRC_RDY) != 0x0)
            {
                vUtDelay1ms(1);
                if (RegReadFldAlign(SRST + u4RegBase*i, SCDT) == 0)
                {
                    break;
                }
            }
            while (u1RegRead1B(VID_CRC_CHK_3 + u4RegBase*i) != 0x00)
            {
                vUtDelay1ms(1);
                if (RegReadFldAlign(SRST + u4RegBase*i, SCDT) == 0)
                {
                    break;
                }
            }
            while (u1RegRead1B(VID_CRC_CHK_0 + u4RegBase*i)  != 0x00)
            {
                vUtDelay1ms(1);
                if (RegReadFldAlign(SRST + u4RegBase*i, SCDT) == 0)
                {
                    break;
                }
            }
            while (u1RegRead1B(VID_CRC_CHK_1 + u4RegBase*i) != 0x00)
            {
                vUtDelay1ms(1);
                if (RegReadFldAlign(SRST + u4RegBase*i, SCDT) == 0)
                {
                    break;
                }
            }

            if (( u1RegRead1B(VID_CRC_CHK_2 +  u4RegBase*i) &0x81) == 0x0)
            {

                vRegWrite1B(VID_CRC_CHK_2 +  u4RegBase*i, 0x0d);// start trigger
                //LOG(6, "CRC start\n");
                //while (u1RegRead1B(VID_CRC_CHK_2)  != 0x8d)
                while (RegReadFldAlign(VID_CRC_CHK +  u4RegBase*i, STA_CRC_RDY) != 0x1)
                {
                    vUtDelay1ms(1);
                    //LOG(6, "delay 1ms for ready bit\n");
                    if (RegReadFldAlign(SRST + u4RegBase*i, SCDT) == 0)
                    {
                        break;
                    }
                }
                vRegWrite1B(VID_CRC_CHK_2 +  u4RegBase*i, 0x0c);

                if ( RegReadFldAlign(VID_CRC_CHK +  u4RegBase*i, STA_CRC_RDY) == 0x1 )
                {
                    //LOG(6, "CRC ready\n");
                    tmp[i][0] = u1RegRead1B(VID_CRC_CHK_3 +  u4RegBase*i);
                    tmp[i][1] = u1RegRead1B(VID_CRC_CHK_0 +  u4RegBase*i);
                    tmp[i][2] = u1RegRead1B(VID_CRC_CHK_1 +  u4RegBase*i) ;
                    // vUtDelay10ms(2);
                    // compare and update result if necessary
                    if ((tmp[i][0] == result[i][0][0]) && (tmp[i][1] == result[i][0][1]) && (tmp[i][2] == result[i][0][2]))
                    {
                        continue;
                    }
                    if ((tmp[i][0] == result[i][1][0]) && (tmp[i][1] == result[i][1][1]) && (tmp[i][2] == result[i][1][2]))
                    {
                        continue;
                    }
                    VERIFY((idx[i] < 3));

                    /*lint -e661*/ /*lint -e662*/
                    if (idx[i] <= 2)
                    {
                        result[i][(idx[i])][0] = tmp[i][0];
                        result[i][(idx[i])][1] = tmp[i][1];
                        result[i][(idx[i])][2] = tmp[i][2];
                    }
                    /*lint +e661*/ /*lint +e662*/
                    idx[i]++;
                    continue;
                }
                else
                {
                    LOG(0, "CRC is not ready\n");
                    return 0;
                }
            }
            else
            {
                LOG(0, "reset CRC fail");
                return 0;
            }
        }

     }

#if defined(CC_MT5880)
			for ( i = 1 ; i < 4 ; i++)
#else
			for ( i = 0 ; i < 4 ; i++)
#endif
   {
        if (u1RegRead1B(VID_CH_MAP_1 +  u4RegBase*i) & 0x04)
        {
            LOG(0, "interlace signal %d\n",i);
        }
        else
        {
            LOG(0, "progressive signal %d\n",i);
        }

        if (idx[i] == 1)
        {
            LOG(0, "assume progressive signal %d\n",i);
            LOG(0, "CRC result %d:\n", i);
            LOG(0, "%x %x %x\n", result[i][0][0], result[i][0][1], result[i][0][2]);
        }
        else if (idx[i] == 2)
        {
            LOG(0, "assume interlaced signal %d\n",i);
            LOG(0, "CRC result %d:\n",i);
            LOG(0, "%x %x %x\n", result[i][0][0], result[i][0][1], result[i][0][2]);
            LOG(0, "%x %x %x\n", result[i][1][0], result[i][1][1], result[i][1][2]);
        }
    }

#if defined(CC_MT5880)
			for ( i = 1 ; i < 3 ; i++)
#else
			for ( i = 0 ; i < 3 ; i++)
#endif
    {
       if (result[i][0][0] == result[i+1][0][0] && \
           result[i][0][1] == result[i+1][0][1] && \
           result[i][0][2] == result[i+1][0][2])
       {
           u1Ret = 1;
       }
       else
       {
           u1Ret = 0;
           break;
       }
    }

    if (u1Ret)
    {
        LOG(0,"Multi CRC PASS\n");
        return (1);
    }
    else
    {
        LOG(0,"Multi CRC FAIL\n");
        return (0);
    }

}
#endif
/**
 * HDMI CRC check
 *
 * @NOTE units test: fgHDMICRC(1); under interlaced signal
 * @NOTE units test: fgHDMICRC(100); under interlaced signal
 */
BOOL fgHDMICRC(INT16 ntry)
{
    UINT8 idx;
    UINT8 result[3][3];
    UINT8 tmp[3] = {0};

    idx = 0;
    result[0][0] = 0;
    result[0][1] = 0;
    result[0][2] = 0;
    result[1][0] = 0;
    result[1][1] = 0;
    result[1][2] = 0;
    LOG(0, "fgHDMICRC: %d\n", ntry);

    while(ntry > 0)
    {
        ntry--;
        vUtDelay10ms(1); // NOTE: IT IS NECESSARY

        if(idx > 2)
        {
            LOG(0, "CRC fail\n");
            return 0;
        }

        vIO32Write1B(VID_CRC_CHK_2 + u4ActiveHdmiRegBase, 0x8c);// clr
        LOG(3, "CRC clear status\n");

        while(IO32ReadFldAlign(VID_CRC_CHK + u4ActiveHdmiRegBase, STA_CRC_RDY) != 0x0)
        {
            vUtDelay1ms(1);
            if (RegReadFldAlign(SRST +u4ActiveHdmiRegBase, SCDT) == 0)
            {
                break;
            }
        }
        while(u1IO32Read1B(VID_CRC_CHK_3 + u4ActiveHdmiRegBase) != 0x00)
        {
            vUtDelay1ms(1);
            if (RegReadFldAlign(SRST +u4ActiveHdmiRegBase, SCDT) == 0)
            {
                break;
            }
        }

        while(u1IO32Read1B(VID_CRC_CHK_0 + u4ActiveHdmiRegBase)  != 0x00)
        {
            vUtDelay1ms(1);
            if (RegReadFldAlign(SRST +u4ActiveHdmiRegBase, SCDT) == 0)
            {
                break;
            }
        }

        while(u1IO32Read1B(VID_CRC_CHK_1 + u4ActiveHdmiRegBase) != 0x00)
        {
            vUtDelay1ms(1);
            if (RegReadFldAlign(SRST +u4ActiveHdmiRegBase, SCDT) == 0)
            {
                break;
            }
        }

        if((u1IO32Read1B(VID_CRC_CHK_2 + u4ActiveHdmiRegBase)&0x81) == 0x0)
        {
            vIO32Write1B(VID_CRC_CHK_2 + u4ActiveHdmiRegBase, 0x0d);// start trigger

            //LOG(6, "CRC start\n");
            //while (u1IO32Read1B(VID_CRC_CHK_2)  != 0x8d)
            while(IO32ReadFldAlign(VID_CRC_CHK + u4ActiveHdmiRegBase, STA_CRC_RDY) != 0x1)
            {
                vUtDelay1ms(1);
                //LOG(6, "delay 1ms for ready bit\n");
				if (RegReadFldAlign(SRST + u4ActiveHdmiRegBase, SCDT) == 0)
				{
					break;
				}
            }

            vIO32Write1B(VID_CRC_CHK_2 + u4ActiveHdmiRegBase, 0x0c);

            if(IO32ReadFldAlign(VID_CRC_CHK + u4ActiveHdmiRegBase, STA_CRC_RDY) == 0x1)
            {
                //LOG(6, "CRC ready\n");
                tmp[0] = u1IO32Read1B(VID_CRC_CHK_3 + u4ActiveHdmiRegBase);
                tmp[1] = u1IO32Read1B(VID_CRC_CHK_0 + u4ActiveHdmiRegBase);
                tmp[2] = u1IO32Read1B(VID_CRC_CHK_1 + u4ActiveHdmiRegBase) ;

                // vUtDelay10ms(2);
                // compare and update result if necessary
                if((tmp[0] == result[0][0]) && (tmp[1] == result[0][1]) && (tmp[2] == result[0][2]))
                {
                    continue;
                }

                if((tmp[0] == result[1][0]) && (tmp[1] == result[1][1]) && (tmp[2] == result[1][2]))
                {
                    continue;
                }

                VERIFY((idx < 3));
                /*lint -e661*/ /*lint -e662*/
                if (idx <= 2)
                {
                result[idx][0] = tmp[0];
                result[idx][1] = tmp[1];
                result[idx][2] = tmp[2];
                }
                /*lint +e661*/ /*lint +e662*/
                idx++;
                continue;
            }
            else
            {
                LOG(0, "CRC is not ready\n");
                return 0;
            }
        }
        else
        {
            LOG(0, "reset CRC fail");
            return 0;
        }
    }

    if(u1IO32Read1B(VID_CH_MAP_1 + u4ActiveHdmiRegBase) & 0x04)
    {
        LOG(0, "interlace signal\n");
    }
    else
    {
        LOG(0, "progressive signal\n");
    }

    if(idx == 1)
    {
        LOG(0, "assume progressive signal\n");
        LOG(0, "CRC result:\n");
        LOG(0, "%x %x %x\n", result[0][0], result[0][1], result[0][2]);
    }
    else if(idx == 2)
    {
        LOG(0, "assume interlaced signal\n");
        LOG(0, "CRC result:\n");
        LOG(0, "%x %x %x\n", result[0][0], result[0][1], result[0][2]);
        LOG(0, "%x %x %x\n", result[1][0], result[1][1], result[1][2]);
    }

    return 1;
}

BOOL fgCCIR2(INT16 ntry)
{
#if defined(CC_MT5880)
	if(ntry == 1)//open ccir2 for wifi push
	{
		UINT32 width,hight;
		UINT32 tmp;

		_bCcir2status = 1;
		OSD_SC_GetScalerInfo(1,&tmp,&tmp,&tmp,&width,&hight,&tmp);

		printf("width=%ld\n",width);
		printf("hight=%ld\n",hight);

		vIO32Write4B(CCIR_00_NEW,0xf2866380);
		vIO32Write4B(CCIR_01_NEW,0xb02ae000);
		vIO32WriteFldAlign(CCIR_03_NEW,width,CCH_AV_WIDTH_PIP);
		vIO32WriteFldAlign(CCIR_03_NEW,hight,CCH_VDE_WIDTH_PIP);
		vIO32Write4B(CCIR_04_NEW,0x6040);

		//pip dta from OSTG
		vIO32WriteFldAlign(HDMI_PIP_SEL,0,REG_VIDEO_OUT_PIP_DATA_SEL);
		//pip clk from OSTG
		vIO32WriteFldAlign(HDMI_PIP_SEL,0,REG_VIDEO_OUT_PIP_MAVIS_SEL2);
		vIO32WriteFldAlign(HDMI_PIP_SEL,0,REG_VIDEO_OUT_PIP_MAVIS_SEL1);
		vIO32WriteFldAlign(HDMI_PIP_SEL,0,REG_VIDEO_OUT_PIP_MAVIS_SEL);
		//pip from CCIR1
		vIO32WriteFldAlign(OMUX_00, 0x5, OMUX_PIP_SEL);

		printf("Turn on wifi push function\n");
	}
	else//off ccir2, main/pip for normal use
	{
		_bCcir2status = 0;
		printf("Turn off wifi push function\n");
	}
#endif
	return 0;
}


/**
 * DVI CRC check
 *
 * @NOTE units test: fgDVICRC(1); under interlaced signal
 * @NOTE units test: fgDVICRC(100); under interlaced signal
 */
BOOL fgDVICRC(INT16 ntry)
{
    UINT8 idx;
    UINT8 result[3][3];
    UINT8 tmp[3] = {0};
    idx = 0;
    result[0][0] = 0;
    result[0][1] = 0;
    result[0][2] = 0;
    result[1][0] = 0;
    result[1][1] = 0;
    result[1][2] = 0;
    LOG(3, "fgDVICRC: %d\n", ntry);

    while(ntry > 0)
    {
        ntry--;
        vUtDelay1ms(15); // NOTE: IT IS NECESSARY

        if(idx > 2)
        {
            LOG(3, "CRC fail\n");
            return 0;
        }

        // calculate CRC

        // select crc path
        if(fgIsMainDVI())
        {
            vIO32WriteFldAlign(SYS_04, 0, CRC_SRC_SEL);
        }
        else
        {
            vIO32WriteFldAlign(SYS_04, 1, CRC_SRC_SEL);
        }

        // CRC init
        vIO32Write4BMsk(DVI_02, 0x0, Fld2Msk32(DVI_CRC_START) | Fld2Msk32(DVI_CRC_CLR));
        // CRC clear
        vIO32Write4BMsk(DVI_02, Fld2Msk32(DVI_CRC_CLR), Fld2Msk32(DVI_CRC_CLR));
        vIO32Write4BMsk(DVI_02, 0x0, Fld2Msk32(DVI_CRC_CLR));

        while(!(IO32ReadFldAlign(DVI_HDTV_05, DVI_STA_CRC_OUT_23_16) == 0) && (IO32ReadFldAlign(DVI_HDTV_05, DVI_STA_CRC_OUT_15_8) == 0) && (IO32ReadFldAlign(DVI_HDTV_05, DVI_STA_CRC_OUT_7_0) == 0))
        {
            vUtDelay1ms(1);
        }

        // CRC start
        vIO32Write4BMsk(DVI_02, Fld2Msk32(DVI_CRC_START), Fld2Msk32(DVI_CRC_START));

        while(!(IO32ReadFldAlign(DVI_HDTV_05, DVI_STA_CRC_RDY)))
        {
            vUtDelay1ms(1);
        }

        // LOG(6, "%x %x %x\n", bReadDirVIN(STA26_2), bReadDirVIN(STA26_1), bReadDirVIN(STA26_0));
        // store CRC result
        tmp[0] = IO32ReadFldAlign(DVI_HDTV_05, DVI_STA_CRC_OUT_23_16);
        tmp[1] = IO32ReadFldAlign(DVI_HDTV_05, DVI_STA_CRC_OUT_15_8);
        tmp[2] = IO32ReadFldAlign(DVI_HDTV_05, DVI_STA_CRC_OUT_7_0);

        // compare and update result if necessary
        if((tmp[0] == result[0][0]) && (tmp[1] == result[0][1]) && (tmp[2] == result[0][2]))
        {
            continue;
        }

        if((tmp[0] == result[1][0]) && (tmp[1] == result[1][1]) && (tmp[2] == result[1][2]))
        {
            continue;
        }

        VERIFY(idx < 3);
        /*lint -e661*/ /*lint -e662*/
        if (idx <= 2)
        {
        result[idx][0] = tmp[0];
        result[idx][1] = tmp[1];
        result[idx][2] = tmp[2];
        }
        /*lint +e661*/ /*lint +e662*/
        idx++;
        continue;
    }

    if(idx == 1)
    {
        LOG(3, "assume progressive signal\n");
        LOG(3, "CRC result:\n");
        LOG(3, "%x %x %x\n", result[0][0], result[0][1], result[0][2]);
    }
    else if(idx == 2)
    {
        LOG(3, "assume interlaced signal\n");
        LOG(3, "CRC result:\n");
        LOG(3, "%x %x %x\n", result[0][0], result[0][1], result[0][2]);
        LOG(3, "%x %x %x\n", result[1][0], result[1][1], result[1][2]);
    }

    return 1;
}
/**
 * HDMI analog PHY CRC check
 *
 * @NOTE units test: fgHDMIPHYCRC(1); under interlaced signal
 * @NOTE units test: fgHDMIPHYCRC(100); under interlaced signal
 */
BOOL fgHDMIPHYCRC(INT16 ntry)
{
    UINT8 idx;
    UINT8 result[3][3];
    UINT8 tmp[3] = {0};
    idx = 0;
    result[0][0] = 0;
    result[0][1] = 0;
    result[0][2] = 0;
    result[1][0] = 0;
    result[1][1] = 0;
    result[1][2] = 0;
    LOG(6, "fgHDMIPHYCRC: %d\n", ntry);

    while(ntry > 0)
    {
        ntry--;
        vUtDelay10ms(1); // NOTE: IT IS NECESSARY

        if(idx > 2)
        {
            LOG(6, "HDMI analog PHY CRC fail\n");
            return 0;
        }

        vIO32Write4B(HDMI_SPH + u4ActiveHdmiRegBase, Fld2Msk32(HDMI_PHY_CRC_RDY));
        vIO32WriteFldAlign(TMDS_CTRL1 + u4ActiveHdmiRegBase, 3, PHY_C_CRC_MODE);
        LOG(6, "HDMI analog PHY CRC clear status\n");

        while(IO32ReadFldAlign(HDMI_SPH + u4ActiveHdmiRegBase, HDMI_PHY_CRC_RDY) != 0x0)
        {
            vUtDelay1ms(1);
        }

        while(u1IO32Read1B(HDMI_SPH_0 + u4ActiveHdmiRegBase) != 0x00)
        {
            vUtDelay1ms(1);
        }

        while(u1IO32Read1B(HDMI_SPH_1 + u4ActiveHdmiRegBase)  != 0x00)
        {
            vUtDelay1ms(1);
        }

        while(u1IO32Read1B(HDMI_SPH_2 + u4ActiveHdmiRegBase) != 0x00)
        {
            vUtDelay1ms(1);
        }

        if((IO32ReadFldAlign(HDMI_SPH + u4ActiveHdmiRegBase, HDMI_PHY_CRC_RDY) == 0x0) && (IO32ReadFldAlign(TMDS_CTRL1, PHY_C_CRC_START) == 0x0))
        {
            vIO32Write1B(TMDS_CTRL1_1 + u4ActiveHdmiRegBase, 0x07);// start trigger

            //LOG(6, "CRC start\n");
            //while (u1IO32Read1B(VID_CRC_CHK_2)  != 0x8d)
            while(IO32ReadFldAlign(HDMI_SPH + u4ActiveHdmiRegBase, HDMI_PHY_CRC_RDY) != 0x1)
            {
                vUtDelay1ms(1);
                //LOG(6, "delay 1ms for ready bit\n");
            }

            vIO32Write1B(TMDS_CTRL1_1 + u4ActiveHdmiRegBase, 0x06);

            if(IO32ReadFldAlign(HDMI_SPH + u4ActiveHdmiRegBase, HDMI_PHY_CRC_RDY) == 0x1)
            {
                //LOG(6, "CRC ready\n");
                tmp[0] = u1IO32Read1B(HDMI_SPH_0 + u4ActiveHdmiRegBase);
                tmp[1] = u1IO32Read1B(HDMI_SPH_1 + u4ActiveHdmiRegBase);
                tmp[2] = u1IO32Read1B(HDMI_SPH_2 + u4ActiveHdmiRegBase) ;

                // vUtDelay10ms(2);
                // compare and update result if necessary
                if((tmp[0] == result[0][0]) && (tmp[1] == result[0][1]) && (tmp[2] == result[0][2]))
                {
                    continue;
                }

                if((tmp[0] == result[1][0]) && (tmp[1] == result[1][1]) && (tmp[2] == result[1][2]))
                {
                    continue;
                }

                VERIFY((idx < 3));
                /*lint -e661*/ /*lint -e662*/
                if (idx <= 2)
                {
                result[idx][0] = tmp[0];
                result[idx][1] = tmp[1];
                result[idx][2] = tmp[2];
                }
                /*lint +e661*/ /*lint +e662*/
                idx++;
                continue;
            }
            else
            {
                LOG(6, "HDMI analog PHY CRC is not ready\n");
                return 0;
            }
        }
        else
        {
            LOG(6, "HDMI analog PHY reset CRC fail");
            return 0;
        }
    }

    if(u1IO32Read1B(VID_CH_MAP_1 + u4ActiveHdmiRegBase) & 0x04)
    {
        LOG(6, "interlace signal\n");
    }
    else
    {
        LOG(6, "progressive signal\n");
    }

    if(idx == 1)
    {
        LOG(6, "assume progressive signal\n");
        LOG(6, "HDMI analog PHY CRC result:\n");
        LOG(6, "%x %x %x\n", result[0][0], result[0][1], result[0][2]);
    }
    else if(idx == 2)
    {
        LOG(6, "assume interlaced signal\n");
        LOG(6, "HDMI analog PHY CRC result:\n");
        LOG(6, "%x %x %x\n", result[0][0], result[0][1], result[0][2]);
        LOG(6, "%x %x %x\n", result[1][0], result[1][1], result[1][2]);
    }

    return 1;
}

UINT8 bHDMIAVIPixelCount(void)
{
    return (u1IO32Read1B(AVIRX2_0 + u4ActiveHdmiRegBase)  & 0x0f);
}

UINT16 wHDMIResoWidth(void)
{
    UINT16 tmp;
    //tmp = (((UINT16)u1IO32Read1B(VID_BLANK_3) << 8) | ((UINT16)u1IO32Read1B(VID_BLANK_2)));
    tmp = ((UINT16)IO32ReadFldAlign(VID_BLANK + u4ActiveHdmiRegBase, VID_DEPIXELS_11_0));

    if((u1IO32Read1B(SYS_CTRL_0 + u4ActiveHdmiRegBase) & 0x30) == 0x10)  // ICLK
    {
        tmp <<= 1;
    }

    return tmp;
}

UINT16 wHDMIResoHeight(void)
{
    UINT16 tmp;
    //tmp = ((( (UINT16)u1IO32Read1B(VID_STAT_1) & 0x0f) << 8) | (UINT16)u1IO32Read1B(VID_STAT_0) );
    tmp = ((UINT16)IO32ReadFldAlign(VID_STAT + u4ActiveHdmiRegBase, VID_DELINES_11_0));
    return tmp;
}

UINT16 wHDMIHTotal(void)
{
    UINT16 tmp;
    //tmp = ((( (UINT16)u1IO32Read1B(VID_HRES_3) & 0x1f) << 8) |  (UINT16)u1IO32Read1B(VID_HRES_2));
    tmp = ((UINT16)IO32ReadFldAlign(VID_HRES + u4ActiveHdmiRegBase, VID_HRES_12_0));

    if((u1IO32Read1B(SYS_CTRL_0 + u4ActiveHdmiRegBase) & 0x30) == 0x10)  // ICLK
    {
        tmp <<= 1;
    }

    return tmp;
}

UINT16 wHDMIVTotal(void)
{
    UINT16 tmp;
    tmp = ((((UINT16)u1IO32Read1B(VID_VRES_1 + u4ActiveHdmiRegBase) & 0x0f) << 8) | (UINT16)u1IO32Read1B(VID_VRES_0 + u4ActiveHdmiRegBase));
    return tmp;
}

#ifdef CC_AUD_HDMI_SPDIF_CFG
UINT8 bHDMIACPType(void)
{
    if((u1IO32Read1B(ACPRX0_0 + u4ActiveHdmiRegBase) != 0x4) || (!_bHdmiMode))
    {
        return 0;
    }
    else
    {
        return (u1IO32Read1B(ACPRX0_1 + u4ActiveHdmiRegBase));
    }
}
UINT8 bHDMIChannelStCategoryCode(void)
{
    return IO32ReadFldAlign(AUDRX_CTRL + u4ActiveHdmiRegBase,AUD_CH_STAT2);
}

UINT8 bHDMIChannelStCopyright(void)
{
    return IO32ReadFldAlign(AUDRX_CTRL + u4ActiveHdmiRegBase,AUD_CH_STAT1_2);
}

#endif

UINT8 bHDMIScanInfo(void)
{
    if((u1IO32Read1B(AVIRX0_0 + u4ActiveHdmiRegBase) != 0x82) || (!_bHdmiModeActive))
    {
        return 0;
    }
    else
    {
        return (u1IO32Read1B(AVIRX1_0 + u4ActiveHdmiRegBase)  & 0x03);
    }
}
UINT8 bHDMIAspectRatio(void)
{
    if((u1IO32Read1B(AVIRX0_0 + u4ActiveHdmiRegBase) != 0x82) || (!_bHdmiModeActive))
    {
        return 0;
    }
    else
    {
        return ((u1IO32Read1B(AVIRX1_1 + u4ActiveHdmiRegBase)  & 0x30) >> 4);
    }
}
UINT8 bHDMIAFD(void)
{
    if((u1IO32Read1B(AVIRX0_0 + u4ActiveHdmiRegBase) != 0x82) || (!_bHdmiModeActive))
    {
        return 0;
    }
    else
    {
        return (u1IO32Read1B(AVIRX1_1 + u4ActiveHdmiRegBase)  & 0xf);
    }
}
UINT8 bHDMI422Input(void)
{
    if((u1IO32Read1B(AVIRX0_0 + u4ActiveHdmiRegBase) != 0x82) || (!_bHdmiModeActive))
    {
        return 0;
    }
    else
    {
        if((u1IO32Read1B(AVIRX1_0 + u4ActiveHdmiRegBase)&0x60) == 0x20)
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
}

UINT8 bHDMIGetColorDepth(void)
{
    return (RegReadFldAlign(SRST + u4ActiveHdmiRegBase, DEEP_STA));
}

UINT8 bHDMIITCFlag(void)
{
    if((u1IO32Read1B(AVIRX0_0 + u4ActiveHdmiRegBase) != 0x82) || (!_bHdmiModeActive))
    {
        return 0;
    }
    else
    {
        if((u1IO32Read1B(AVIRX1_2 + u4ActiveHdmiRegBase)&0x80) == 0x80)
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
}
UINT8 bHDMIITCContent(void)
{
    if((u1IO32Read1B(AVIRX0_0 + u4ActiveHdmiRegBase) != 0x82) || (!_bHdmiModeActive))
    {
        return 0;
    }
    else
    {
        return (u1IO32Read1B(AVIRX2_0 + u4ActiveHdmiRegBase)&0x30) >> 4 ;
    }
}
/*
full range: 0 ~ 255.
limited range: 16 ~ 235.
   00: Default, depend on video format.
         PC timing: full range.
         Video timing: limited range.
   01: limited range.
   10: full range.
   11: Reserved.
*/
UINT8 bHDMIRgbRange(void)
{
    UINT8 bMode;

    if((u1IO32Read1B(AVIRX0_0 + u4ActiveHdmiRegBase) != 0x82) || (!_bHdmiModeActive))
    {
        _bHDMIRange = SV_HDMI_RANGE_FORCE_AUTO;
        return SV_HDMI_RANGE_FORCE_AUTO;
    }
    else
    {
        bMode = bGetHDMIRangeMode();
        _bHDMIRange = (u1IO32Read1B(AVIRX1_2 + u4ActiveHdmiRegBase) & 0x0c) >> 2;

        switch(bMode)
        {
            default:
            case SV_HDMI_RANGE_FORCE_AUTO:
                return _bHDMIRange;
            case SV_HDMI_RANGE_FORCE_LIMIT:
                return SV_HDMI_RANGE_FORCE_LIMIT;

            case SV_HDMI_RANGE_FORCE_FULL:
                return SV_HDMI_RANGE_FORCE_FULL;
        }
    }
}

UINT8 bGetHDMIRangeMode(void)
{
    return _bHDMIRangeMode;
}

void vSetHDMIRangeMode(UINT8 bMode)
{
    _bHDMIRangeMode = bMode;
}

PRIVATE void fgHDMIAVIFrame(void)
{
    if((u1IO32Read1B(INTR_STATE1_0 + u4ActiveHdmiRegBase) & 0x10) == 0x10)
    {
        printf("no avi infoframe\n");
    }
    else
    {
        if(u1IO32Read1B(AVIRX0_0 + u4ActiveHdmiRegBase) != 0x82)
        {
            printf("invalid avi infoframe\n");
            return;
        }

        LOG(3, "detect avi infoframe\n");

        if((u1IO32Read1B(AVIRX1_0 + u4ActiveHdmiRegBase)& 0xE0) == 0x00)
        {
            printf("RGB color space\n");
        }
        else if((u1IO32Read1B(AVIRX1_0 + u4ActiveHdmiRegBase) & 0xE0) == 0x20)
        {
            printf("YCbCr 422 color space\n");
        }
        else if((u1IO32Read1B(AVIRX1_0 + u4ActiveHdmiRegBase)  & 0xE0) == 0x40)
        {
            printf("YCbCr 444 color space\n");
        }
		else if((u1IO32Read1B(AVIRX1_0 + u4ActiveHdmiRegBase)  & 0xE0) == 0x60)
		{
			printf("YCbCr 420 color space\n");
        }

        if((u1IO32Read1B(AVIRX1_0 + u4ActiveHdmiRegBase)  & 0x03) == 0x01)
        {
            printf("Overscanned(TV)\n");
        }
        else if((u1IO32Read1B(AVIRX1_0 + u4ActiveHdmiRegBase)  & 0x03) == 0x02)
        {
            printf("Underscanned(PC)\n");
        }

        if((u1IO32Read1B(AVIRX1_1 + u4ActiveHdmiRegBase)  & 0xc0) == 0x40)
        {
            printf("ITU 601(SDTV)\n");
        }
        else if((u1IO32Read1B(AVIRX1_1 + u4ActiveHdmiRegBase) & 0xc0) == 0x80)
        {
            printf("ITU 709(HDTV)\n");
        }

        if((u1IO32Read1B(AVIRX1_1 + u4ActiveHdmiRegBase)  & 0x30) == 0x10)
        {
            printf("4:3\n");
        }
        else if((u1IO32Read1B(AVIRX1_1 + u4ActiveHdmiRegBase)  & 0x30) == 0x20)
        {
            printf("16:9\n");
        }

        if((u1IO32Read1B(AVIRX1_2 + u4ActiveHdmiRegBase)  & 0x0c) == 0x04)
        {
            printf("Limited Range\n");
        }
        else if((u1IO32Read1B(AVIRX1_2 + u4ActiveHdmiRegBase)  & 0x0c) == 0x08)
        {
            printf("Full range\n");
        }

        printf("pixel replication %x\n", u1IO32Read1B(AVIRX2_0 + u4ActiveHdmiRegBase));
    }
}

PRIVATE void fgHDMISyncDetect(void)
{
    if(u1IO32Read1B(SRST_2 + u4ActiveHdmiRegBase)& 0x01)
    {
        LOG(6, "sync detect\n");
    }

    if(u1IO32Read1B(SRST_2 + u4ActiveHdmiRegBase)& 0x02)
    {
        LOG(6, "clock detect\n");
    }

    if(u1IO32Read1B(SRST_2 + u4ActiveHdmiRegBase) & 0x04)
    {
        LOG(6, "vsync detect\n");
    }

    if(u1IO32Read1B(SRST_2 + u4ActiveHdmiRegBase)& 0x08)
    {
        LOG(6, "RX0 pwr5v detect\n");
    }

    if(u1IO32Read1B(SRST_2 + u4ActiveHdmiRegBase)& 0x40)
    {
        LOG(6, "RX1 pwr5v detect\n");
    }

    if(u1IO32Read1B(SRST_2 + u4ActiveHdmiRegBase)& 0x80)
    {
        LOG(6, "RX2 pwr5v detect\n");
    }
}

BOOL fgHDMIinterlaced(void)
{
    UINT8 u1Vid = 0;
    UINT8 u1Vid392D = 0;

    if (u1IO32Read1B(AVIRX0_0 + u4ActiveHdmiRegBase) == 0x82)
    {
       u1Vid = u1IO32Read1B(AVIRX1_3 + u4ActiveHdmiRegBase);

		if (((u1Vid == 20) || (u1Vid == 39)) && (_wDviHeight == 1080) && (_wDviHtotal == 2304))
		{
			u1Vid392D = 1;
		}
		else
		{
			u1Vid392D = 0;
		}
    }
	else
	{
		//when test using pattern gen to output timing, there is no avi infoframe 
		if (((_wDviHeight == 1080) || (_wDviHeight == 540))&& (_wDviHtotal == 2304) && (bHDMIRefreshRate() == 50))
		{
			u1Vid392D = 1;		  
		}
		else
		{
			u1Vid392D = 0;
		}
	}

    if (((u1IO32Read1B(VID_CH_MAP_1 + u4ActiveHdmiRegBase)  & 0x04) == 0x04) || (u1Vid392D == 1))  // 2D 1080i50 with 1250 Vtotal
    {
        // LOG(6, "interface signal\n");
        return 1;
    }
    else
    {
        // LOG(6, "progressive signal\n");
        return 0;
    }
}


/**
 * @brief bHDMIRefreshRate
 *
 * return the frame rate.
 * @note range: 49~51 -> 50; 59~61 -> 60
 */
UINT8 bHDMIRefreshRate(void)
{
    UINT32 pfreq;
    UINT8 rate;
    UINT32 dwtmp;
    dwtmp = wHDMIHTotal() * wHDMIVTotal();

    if(dwtmp == 0)  // avoid divide by zero
    {
        return 1;
    }

    pfreq = dwHDMIPixelFreq();
    //rate = (pfreq*1000) / dwtmp;
    rate = ((pfreq * 1000) + (dwtmp>>3)) / dwtmp;  // add 1/8 ~= 12.5% for xx.97 Hz frame rates
    // LOG(6, "rate: %d\n", rate);

    if((rate <= 51) && (rate >= 49))
    {
        rate = 50;
    }
    else if((rate <= 57) && (rate >= 55))
    {
        rate = 56;
    }
    else if((rate <= 61) && (rate >= 59))
    {
        rate = 60;
    }
    else if((rate <= 67) && (rate >= 65))
    {
        rate = 66;
    }
	else if((rate <= 68) && (rate >= 67))
    {
        rate = 68;
    }
    else if((rate <= 71) && (rate >= 69))
    {
        rate = 70;
    }
    else if((rate <= 73) && (rate >= 71))
    {
        rate = 72;
    }
    else if((rate <= 76) && (rate >= 74))
    {
        rate = 75;
    }
    else if((rate <= 86) && (rate >= 84))
    {
        rate = 85;
    }

    return rate;
}

UINT32 dwHDMILineFreq(void)
{
    UINT32 ret;
    UINT16 wDiv;
    wDiv = wHDMIHTotal();

    if(wDiv == 0)
    {
        return 1;
    }

    ret = ((dwHDMIPixelFreq() * 10) / (wDiv));
    return ret;
}

PRIVATE void vHDMIHDMIMode(void)
{
    if(u1IO32Read1B(AUDP_STAT_0 + u4ActiveHdmiRegBase)& 0x01)
    {
        printf("HDMI mode\n");
    }
    else
    {
        printf("DVI mode\n");
    }
}

PRIVATE void vHDMIAVMuteSta(void)
{
    if(u1IO32Read1B(AUDP_STAT_0 + u4ActiveHdmiRegBase)& 0x04)
    {
        LOG(0, "set Avmute\n");
    }
	else
	{
		printf("CLK Avmute\n");
    }
}

PRIVATE void vHDMIPolSta(void)
{
    if(u1IO32Read1B(VID_CH_MAP_1 + u4ActiveHdmiRegBase) & 0x01)
    {
        LOG(6, "HSYNC Positive Polarity\n");
    }
    else
    {
        LOG(6, "HSYNC Negative Polarity\n");
    }

    if(u1IO32Read1B(VID_CH_MAP_1 + u4ActiveHdmiRegBase) & 0x02)
    {
        LOG(6, "VSYNC Positive Polarity\n");
    }
    else
    {
        LOG(6, "VSYNC Negative Polarity\n");
    }
}

BOOL fgHDMIHsyncPolarity(void)
{
    if(u1IO32Read1B(VID_CH_MAP_1 + u4ActiveHdmiRegBase) & 0x01)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

BOOL fgHDMIVsyncPolarity(void)
{
    if(u1IO32Read1B(VID_CH_MAP_1 + u4ActiveHdmiRegBase)  & 0x02)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/**
 * CORE_ISO_EN	Bypass output data from TMDS_CORE.
 */
void vHDMIBypassVdo(void)
{
    // UINT8 bI2CWriteData;
    // CORE_ISO_EN: Bypass output data from TMDS_CORE
    // vHDMIWriteDevH(0x3a, bHDMIReadDevH(0x3a)|0x01);
    // LOG(6, "0x683a[0]\n");
    // VIDEO_MUTE: Video Mute (send blanking value to output)
    // bI2CWriteData = bHDMIReadDevH(0x37);
    // vHDMIWriteDevH(0x37, (bI2CWriteData | 0x01));
    // reset FIFO
    //vIO32Write4BMsk(TMDS_CTRL0, 0x0, Fld2Msk32(PCLK_INV) |Fld2Msk32(TMDS_SP_RST_) |Fld2Msk32(TMDS_RT_RST_) |Fld2Msk32(TMDS_RX_RST_));
    //vIO32Write4BMsk(TMDS_CTRL0,
    // Fld2Msk32(TMDS_SP_RST_) |Fld2Msk32(TMDS_RT_RST_) |Fld2Msk32(TMDS_RX_RST_),
    //  Fld2Msk32(TMDS_SP_RST_) |Fld2Msk32(TMDS_RT_RST_) |Fld2Msk32(TMDS_RX_RST_));
}

PRIVATE void vHDMIColorRelated(void)
{
    if(IO32ReadFldAlign(VID_MODE + u4ActiveHdmiRegBase, ENDOWNSAMPLE))
    {
        LOG(3, "down sample: 444 to 422\n");
    }

    if(IO32ReadFldAlign(VID_MODE + u4ActiveHdmiRegBase, ENUPSAMPLE))
    {
        LOG(3, "up sample: 422 to 444\n");
    }

    LOG(3, "no color transform\n");
}

PRIVATE void vHDMIIntSta1(void)
{
    UINT8 sta;
    sta = u1IO32Read1B(INTR_STATE0_1 + u4ActiveHdmiRegBase);
    UNUSED(sta);

    if(IO32ReadFldAlign(INTR_STATE0 + u4ActiveHdmiRegBase, INTR1_AUTH_DONE))
    {
        LOG(3, "AUTH DONE\n");
    }

    if(IO32ReadFldAlign(INTR_STATE0 + u4ActiveHdmiRegBase, INTR1_AUTH_START))
    {
        LOG(3, "AUTHSTART\n");
    }

    if(IO32ReadFldAlign(INTR_STATE0 + u4ActiveHdmiRegBase, INTR1_BCH_PKT_ERR_ALERT))
    {
        LOG(3, "ECCERR\n");
    }

    if(IO32ReadFldAlign(INTR_STATE0 + u4ActiveHdmiRegBase, INTR1_FIFO_ERR))
    {
        LOG(3, "Audio FIFO error\n");
    }

    if(IO32ReadFldAlign(INTR_STATE0 + u4ActiveHdmiRegBase, INTR1_PLL_UNLOCKED))
    {
        LOG(3, "Audio PLL unlocked\n");
    }

    if(IO32ReadFldAlign(INTR_STATE0 + u4ActiveHdmiRegBase, INTR1_PKT_ERR))
    {
        LOG(3, "Audio N/CTS packet decode error\n");
    }

    if(IO32ReadFldAlign(INTR_STATE0 + u4ActiveHdmiRegBase, INTR1_HW_N_CHG))
    {
        LOG(3, "ACRHWN\n");
    }

    if(IO32ReadFldAlign(INTR_STATE0 + u4ActiveHdmiRegBase, INTR1_HW_CTS_CHG))
    {
        LOG(3, "ACRHWCTS\n");
    }

    vIO32Write4B(INTR_STATE0 + u4ActiveHdmiRegBase, 0xff << 8);
}
PRIVATE void vHDMIIntSta2(void)
{
    UINT8 sta;
    sta = u1IO32Read1B(INTR_STATE0_2 + u4ActiveHdmiRegBase);
    UNUSED(sta);

    if(IO32ReadFldAlign(INTR_STATE0 + u4ActiveHdmiRegBase, INTR2_CLK_CHG))
    {
        LOG(3, "Video Clock Frequency Changed\n");
    }

    if(IO32ReadFldAlign(INTR_STATE0 + u4ActiveHdmiRegBase, INTR2_NEW_AUD_PKT))
    {
        LOG(3, "Received Audio Packet\n");
    }

    if(IO32ReadFldAlign(INTR_STATE0 + u4ActiveHdmiRegBase, INTR2_GOT_CTS))
    {
        LOG(3, "Received CTS Packet\n");
    }

    if(IO32ReadFldAlign(INTR_STATE0 + u4ActiveHdmiRegBase, INTR2_SCDT))
    {
        LOG(3, "Sync Detect change detected\n");
    }

    if(IO32ReadFldAlign(INTR_STATE0 + u4ActiveHdmiRegBase, INTR2_CKDT))
    {
        LOG(3, "Clock Detect change detected\n");
    }

    if(IO32ReadFldAlign(INTR_STATE0 + u4ActiveHdmiRegBase, INTR2_SOFT_INTR_EN))
    {
        LOG(3, "Software-Induced Interrupt\n");
    }

    if(IO32ReadFldAlign(INTR_STATE0 + u4ActiveHdmiRegBase, INTR2_VSYNC))
    {
        LOG(3, "VSYNC Active Edge recognized\n");
    }

    if(IO32ReadFldAlign(INTR_STATE0 + u4ActiveHdmiRegBase, INTR2_HDMI_MODE))
    {
        LOG(3, "HDMI Mode change detected\n");
    }

    vIO32Write4B(INTR_STATE0 + u4ActiveHdmiRegBase, 0xff << 16);
}
PRIVATE void vHDMIIntSta3(void)
{
    UINT8 sta;
    sta = u1IO32Read1B(INTR_STATE0_3 + u4ActiveHdmiRegBase);
    UNUSED(sta);

    if(IO32ReadFldAlign(INTR_STATE0 + u4ActiveHdmiRegBase, INTR3_NEW_AVI))
    {
        LOG(3, "NEW_AVI\n");
    }

    if(IO32ReadFldAlign(INTR_STATE0 + u4ActiveHdmiRegBase, INTR3_NEW_SPD))
    {
        LOG(3, "NEWSP\n");
    }

    if(IO32ReadFldAlign(INTR_STATE0 + u4ActiveHdmiRegBase, INTR3_NEW_AUD))
    {
        LOG(3, "NEW_AUD\n");
    }

    if(IO32ReadFldAlign(INTR_STATE0 + u4ActiveHdmiRegBase, INTR3_NEW_MPEG))
    {
        LOG(3, "NEW_MPEG\n");
    }

    if(IO32ReadFldAlign(INTR_STATE0 + u4ActiveHdmiRegBase, INTR3_NEW_UNREC))
    {
        LOG(3, "NEW_UNR\n");
    }

    if(IO32ReadFldAlign(INTR_STATE0 + u4ActiveHdmiRegBase, INTR3_P_ERR))
    {
        LOG(3, "SPDIFERR\n");
    }

    if(IO32ReadFldAlign(INTR_STATE0 + u4ActiveHdmiRegBase, INTR3_CP_SET_MUTE))
    {
        LOG(3, "SETMUTE\n");
    }

    if(IO32ReadFldAlign(INTR_STATE0 + u4ActiveHdmiRegBase, INTR3_CEA_NEW_CP))
    {
        LOG(3, "NEW_CP\n");
    }

    vIO32Write4B(INTR_STATE0 + u4ActiveHdmiRegBase, (UINT32)0xff << 24);
}
PRIVATE void vHDMIIntSta4(void)
{
    UINT8 sta;
    sta = u1IO32Read1B(INTR_STATE1_0 + u4ActiveHdmiRegBase);

    if(IO32ReadFldAlign(INTR_STATE1 + u4ActiveHdmiRegBase, INTR4_UNDERRUN))
    {
        printf("FIFO Under-Run\n");
    }

    if(IO32ReadFldAlign(INTR_STATE1 + u4ActiveHdmiRegBase, INTR4_OVERRUN))
    {
        printf("FIFO Overrun\n");
    }

    if(IO32ReadFldAlign(INTR_STATE1 + u4ActiveHdmiRegBase, INTR4_CTS_REUSED_ERR))
    {
        LOG(3, "CTS Reused\n");
    }

    if(IO32ReadFldAlign(INTR_STATE1 + u4ActiveHdmiRegBase, INTR4_CTS_DROPPED_ERR))
    {
        LOG(3, "CTS Dropped\n");
    }

    if(IO32ReadFldAlign(INTR_STATE1 + u4ActiveHdmiRegBase, INTR4_NO_AVI))
    {
        LOG(3, "NO AVI Received\n");
    }

    if(IO32ReadFldAlign(INTR_STATE1 + u4ActiveHdmiRegBase, INTR4_T4_PKT_ERR_ALERT))
    {
        LOG(3, "TERC4 Error\n");
    }

    if(IO32ReadFldAlign(INTR_STATE1 + u4ActiveHdmiRegBase, INTR4_HDCP_PKT_ERR_ALERT))
    {
        LOG(3, "HDCP Error\n");
    }

    vIO32Write1B(INTR_STATE1_0 + u4ActiveHdmiRegBase, sta);
}
PRIVATE void vHDMIIntSta5(void)
{
    UINT8 sta;
    sta = u1IO32Read1B(INTR_STATE1_1 + u4ActiveHdmiRegBase);

    /**
     * VRCHG, HRCHG has unstable status, which depending on CSC_R2Y.
     * See P.29
     */
    if(IO32ReadFldAlign(INTR_STATE1 + u4ActiveHdmiRegBase, INTR5_AUD_SAMPLE_F))
    {
        printf("Audio Fs Sample Rate Change\n");
    }

    if(IO32ReadFldAlign(INTR_STATE1 + u4ActiveHdmiRegBase, INTR5_INTERLACEOUT))
    {
        LOG(3, "Interlaced Status Change\n");
    }

    /*
    if (IO32ReadFldAlign(INTR_MASK1,INTR5_POLCHG))
    {
    	LOG(6, "Palarity Change on VSYNC or HSYNC\n");
    }
    */
    if(IO32ReadFldAlign(INTR_STATE1 + u4ActiveHdmiRegBase, INTR5_HRESCHG))
    {
        printf("Horizontal resolution change\n");
    }

    if(IO32ReadFldAlign(INTR_STATE1 + u4ActiveHdmiRegBase, INTR5_VRESCHG))
    {
        printf("Vertical resolution change\n");
    }

    if(IO32ReadFldAlign(INTR_STATE1 + u4ActiveHdmiRegBase, INTR5_BCH_AUDIO_ALERT))
    {
        LOG(3, "Audio-Related Link Error\n");
    }

    if(IO32ReadFldAlign(INTR_STATE1 + u4ActiveHdmiRegBase, INTR5_AUDIO_MUTE))
    {
        printf("Audio Auto Configure muted ...\n");
    }

    if(IO32ReadFldAlign(INTR_STATE1 + u4ActiveHdmiRegBase, INTR5_FN_CHG))
    {
        printf("Fn Change\n");
    }

    vIO32Write1B(INTR_STATE1_1 + u4ActiveHdmiRegBase, sta);
}
PRIVATE void vHDMIIntSta6(void)
{
    if(IO32ReadFldAlign(INTR_STATE1 + u4ActiveHdmiRegBase, INTR6_PWR5V_RX0))
    {
        LOG(3, "Cable1 Unplug Interrupt\n");
    }

    if(IO32ReadFldAlign(INTR_STATE1 + u4ActiveHdmiRegBase, INTR6_PWR5V_RX1))
    {
        LOG(3, "Cable2 Unplug Interrupt\n");
    }

    if(IO32ReadFldAlign(INTR_STATE1 + u4ActiveHdmiRegBase, INTR6_PWR5V_RX2))
    {
        LOG(3, "Cable3 Unplug Interrupt\n");
    }

    if(IO32ReadFldAlign(INTR_STATE1 + u4ActiveHdmiRegBase, INTR6_NEW_ACP))
    {
        LOG(3, "NEW_ACP\n");
    }

    vIO32Write1B(INTR_STATE1_0 + u4ActiveHdmiRegBase, u1IO32Read1B(INTR_STATE1_0));
}

void vHDMIHDCPKey(void)
{
    LOG(6, "Key Mask %x %x\n", u1IO32Read1B(EPST_3 + u4ActiveHdmiRegBase),  u1IO32Read1B(KS_MASK_0 + u4ActiveHdmiRegBase));
    LOG(6, "0x6032: [5]: DECRYPT, [4]: AUTH_EN\n");
    LOG(6, "%x\n", u1IO32Read1B(HDCP_STAT_2 + u4ActiveHdmiRegBase));
    LOG(6, "BKSV: Receiver's KSV\n");
    LOG(6, "%x\n", u1IO32Read1B(SHD_BKSV0_2 + u4ActiveHdmiRegBase));
    LOG(6, "%x\n", u1IO32Read1B(SHD_BKSV0_3 + u4ActiveHdmiRegBase));
    LOG(6, "%x\n", u1IO32Read1B(SHD_BKSV1_0 + u4ActiveHdmiRegBase));
    LOG(6, "%x\n", u1IO32Read1B(SHD_BKSV1_1 + u4ActiveHdmiRegBase));
    LOG(6, "%x\n", u1IO32Read1B(SHD_BKSV1_2 + u4ActiveHdmiRegBase));
    LOG(6, "AKSV: Transmitter's KSV\n");
    LOG(6, "%x\n", u1IO32Read1B(SHD_AKSV_1 + u4ActiveHdmiRegBase));
    LOG(6, "%x\n", u1IO32Read1B(SHD_AKSV_2 + u4ActiveHdmiRegBase));
    LOG(6, "%x\n", u1IO32Read1B(SHD_AKSV_3 + u4ActiveHdmiRegBase));
    LOG(6, "%x\n", u1IO32Read1B(SHD_AN0_0 + u4ActiveHdmiRegBase));
    LOG(6, "%x\n", u1IO32Read1B(SHD_AN0_1 + u4ActiveHdmiRegBase));
    LOG(6, "Ri\n");
    LOG(6, "%x\n", u1IO32Read1B(SHD_BKSV1_3 + u4ActiveHdmiRegBase));
    LOG(6, "%x\n", u1IO32Read1B(SHD_AKSV_0 + u4ActiveHdmiRegBase));
    LOG(6, "AN\n");
    LOG(6, "%x\n", u1IO32Read1B(SHD_AN0_2 + u4ActiveHdmiRegBase));
    LOG(6, "%x\n", u1IO32Read1B(SHD_AN0_3 + u4ActiveHdmiRegBase));
    LOG(6, "%x\n", u1IO32Read1B(SHD_AN1_0 + u4ActiveHdmiRegBase));
    LOG(6, "%x\n", u1IO32Read1B(SHD_AN1_1 + u4ActiveHdmiRegBase));
    LOG(6, "%x\n", u1IO32Read1B(SHD_AN1_2 + u4ActiveHdmiRegBase));
    LOG(6, "%x\n", u1IO32Read1B(SHD_AN1_3 + u4ActiveHdmiRegBase));
    LOG(6, "%x\n", u1IO32Read1B(SHD_BSTATUS_0 + u4ActiveHdmiRegBase));
    LOG(6, "%x\n", u1IO32Read1B(SHD_BSTATUS_1 + u4ActiveHdmiRegBase));
}

void vHDMIReloadHDCPKey(void)
{
    vIO32Write1B(EPST_2 + u4ActiveHdmiRegBase, 0x40);
    vIO32Write1B(EPST_1 + u4ActiveHdmiRegBase, 0x00);
    vIO32Write1B(EPST_2 + u4ActiveHdmiRegBase, 0x20);
    LOG(6, "60fa: %x\n", u1IO32Read1B(EPST_2 + u4ActiveHdmiRegBase));
}

/**
 * @example: mode=3, vHDMIHDCPSelfBist(3)
 */
void vHDMIHDCPSelfBist(UINT8 mode)
{
    vIO32Write1B(EPST_1 + u4ActiveHdmiRegBase, 0x00);
    vIO32Write1B(EPST_2 + u4ActiveHdmiRegBase, 0x00);
    LOG(6, "0x60f9: %d\n",  u1IO32Read1B(EPST_1 + u4ActiveHdmiRegBase));
    vIO32Write1B(EPST_2 + u4ActiveHdmiRegBase, (u1IO32Read1B(EPST_2 + u4ActiveHdmiRegBase)&0xe0) | mode);
    vUtDelay1s(2);
    LOG(6, "60f9: %x\n", u1IO32Read1B(EPST_1 + u4ActiveHdmiRegBase)); // Return 1 if hdcp bist is successful
}

/**
 * mode == 3
 */
UINT8 bHDMIHDCPSelfBist(UINT8 mode)
{
    vIO32Write1B(EPST_1 + u4ActiveHdmiRegBase, 0x00);
    vIO32Write1B(EPST_2 + u4ActiveHdmiRegBase, 0x00);
    //LOG(6, "0x60f9: %d\n", bHDMIReadDevL(0xf9));
    vIO32Write1B(EPST_2 + u4ActiveHdmiRegBase, (u1IO32Read1B(EPST_2 + u4ActiveHdmiRegBase)&0xe0) | mode);
    vUtDelay1s(2);

    //LOG(6, "60f9: %x\n", bHDMIReadDevL(0xf9)); // Return 1 if hdcp bist is successful
    if(u1IO32Read1B(EPST_1 + u4ActiveHdmiRegBase) == 0x01)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

PRIVATE void vHDMIIntPattern(void)
{
    LOG(6, "0x6012: [7]:H, [6]:V, [2]:R, [1]:G, [0]:B\n");
}

PRIVATE void vHDMIReset(void)
{
    LOG(6, "0x6005:[0]:SW_RST, 0x5:[1]:FIFO_RST, 0x90:[1]:TMDS_RESETB\n");
    LOG(6, "0x6090:[1]: TMDS_RESETB\n");
}

PRIVATE void vHDMIQuerybHDMIState(void)
{
    printf("_bHDMIState[%d]: %x\n", eActiveHdmiPort,_arHdmiRx[eActiveHdmiPort]._bHDMIState);
}

void vHDMISetHDMIState(UINT8 tmp)
{
    _arHdmiRx[eActiveHdmiPort]._bHDMIState = tmp;
    vHDMIQuerybHDMIState();
}

void vHDMIStopFw(void)
{
    _arHdmiRx[eActiveHdmiPort]._bHdmiCmd = 6;
}

void vHDMIResumeFw(void)
{
    _arHdmiRx[eActiveHdmiPort]._bHdmiCmd = 0;
}
/*
MT5387 -> CC_MT5387
MT5363 -> CC_MT5387 and CC_MT5363
MT5365 -> CC_MT5365
MT5395 -> CC_MT5395
*/
void vHDMIDebug(UINT8 cmd, UINT32 value)
{
    //UINT32 EQvalue;
    switch(cmd)
    {
        case 0:
            _arHdmiRx[eActiveHdmiPort]._bHdmiCmd = 0;
            break;

            /*1. toggle TDFIFO and RTCKAUTO*/
        case 1:
            vIO32WriteFldAlign(MUTE_SET + u4ActiveHdmiRegBase, 1, TDFIFO_SYNC_EN);
            vIO32WriteFldAlign(MUTE_SET + u4ActiveHdmiRegBase, 0, TDFIFO_SYNC_EN);
            break;

            /*2. HDMI reset*/
        case 2:
            vIO32Write1B(SRST_1 + u4ActiveHdmiRegBase, 0x0f);
            vIO32Write1B(SRST_1 + u4ActiveHdmiRegBase, 0x00);
            break;

            /*3. HDMI Core  reset*/
        case 3:
            vIO32WriteFldAlign((IO_VIRT + 0x22400), 0x1, Fld(1, 5, AC_MSKB0));
            vIO32WriteFldAlign((IO_VIRT + 0x22400), 0x0, Fld(1, 5, AC_MSKB0));
            break;

            /*4. DVI pattern*/
        case 4:
            if(fgIsMainDVI())
            {
                vIO32WriteFldAlign(OMUX_00, value, OMUX_MAIN_RAMP_EN);
            }

            if(fgIsPipDVI())
            {
                vIO32WriteFldAlign(OMUX_00, value, OMUX_PIP_RAMP_EN);
            }

            break;

            /*5. EQ adjust*/
        case 5:
            break;

            /*6. Disable HW auto mute*/
        case 6:
            vIO32WriteFldAlign(VID_VRES + u4ActiveHdmiRegBase, 0x1, VRES_MUTE_CLR);
            _arHdmiRx[eActiveHdmiPort]._bHdmiCmd = 0x20;
            break;

            /*7. Reset audio FIFO*/
        case 7:
            vIO32WriteFldAlign(AEC_CTRL + u4ActiveHdmiRegBase, 0, AAC_EN);
            vIO32WriteFldAlign(SRST + u4ActiveHdmiRegBase, 1, FIFO_RST);
            vUtDelay2us(3);
            vIO32WriteFldAlign(SRST + u4ActiveHdmiRegBase, 0, FIFO_RST);
            vUtDelay2us(500);
            vIO32Write4B(INTR_STATE1 + u4ActiveHdmiRegBase, Fld2Msk32(INTR4_OVERRUN) | Fld2Msk32(INTR4_UNDERRUN) | Fld2Msk32(INTR5_AUD_SAMPLE_F));
            vIO32Write4B(INTR_STATE1 + u4ActiveHdmiRegBase, Fld2Msk32(INTR5_AUDIO_MUTE));
            vIO32WriteFldAlign(AEC_CTRL + u4ActiveHdmiRegBase, 1, AAC_EN);
            vIO32WriteFldAlign(AUDP_STAT + u4ActiveHdmiRegBase, 0, AUDIO_MUTE);
            break;

            /*8. Stop DVI detect*/
        case 8:
            _arHdmiRx[eActiveHdmiPort]._bHdmiCmd = 0x30;
            break;

            /*9. Stop HDMI and DVI detect*/
        case 9:
            _arHdmiRx[eActiveHdmiPort]._bHdmiCmd = 0x40;
            break;

            /*10. Get EQ setting*/
        case 10:
            vHDMIGetEQSetting();
            break;

            /*11. HDMI PHY Reset*/
        case 11:
            bHdmiPhyReset(eActiveHdmiPort,value);
            break;

            /*12. No Vendor Specific InfoFrame*/
        case 12:
            vIO32WriteFldAlign(N_HDMI_CTRL + u4ActiveHdmiRegBase, 0x0, TT2_EXT_PKT_EN);
            vIO32WriteFldAlign(N_HDMI_CTRL3 + u4ActiveHdmiRegBase, 0x00, EXT_PKT_DEC);
            vIO32WriteFldAlign(VID_SET + u4ActiveHdmiRegBase, 1, CEA_NEW_UNREC_SEL);
            break;

            /*13.force set mhl mode*/
        case 13:
			if(value == 1)
			{
				_bIsMhlDevice = 1;
			}
			else
			{
				_bIsMhlDevice = 0;
			}
            break;

            /*14.force set mhl packet pixel mode*/
        case 14:
			if(value == 1)
			{
				_bIsMhlPPMode = 1;
			}
			else
			{
				_bIsMhlPPMode = 0;
			}
            break;

            /*15.Equalizer offset calibration 7--0--8--15*/
        case 15:
			u1HDMIEQOffsetCali((E_HDMI_SWITCH_NUM)value);
            break;

			//set HDCP key
		case 16:
			u1HDMI_SetHdcpKey(_au1Hdcp, sizeof(_au1Hdcp));
			break;
			//value = port num
		case 17:
			printf("bHDMIPort5VStatus(%d) = %d\n", value, bHDMIPort5VStatus((E_HDMI_SWITCH_NUM)value));
			break;

        case 85:
            if (value == 1)
            {
                _arHdmiRx[eActiveHdmiPort]._bHdmiCmd = HDMI_DEBUG_HDCP_MONITOR;
            }
            else if (value == 2)
            {
                _arHdmiRx[HDMI_SWITCH_1]._bHdmiCmd = HDMI_DEBUG_HDCP_MONITOR;
                _arHdmiRx[HDMI_SWITCH_2]._bHdmiCmd = HDMI_DEBUG_HDCP_MONITOR;
                _arHdmiRx[HDMI_SWITCH_3]._bHdmiCmd = HDMI_DEBUG_HDCP_MONITOR;
                _arHdmiRx[HDMI_SWITCH_4]._bHdmiCmd = HDMI_DEBUG_HDCP_MONITOR;
            }
            else if (value == 3)
            {
                _arHdmiRx[HDMI_SWITCH_1]._bHdmiCmd = 0;
                _arHdmiRx[HDMI_SWITCH_2]._bHdmiCmd = 0;
                _arHdmiRx[HDMI_SWITCH_3]._bHdmiCmd = 0;
                _arHdmiRx[HDMI_SWITCH_4]._bHdmiCmd = 0;
            }
            else
            {
                _arHdmiRx[eActiveHdmiPort]._bHdmiCmd = 0;
            }
            break;

        default:
            break;
    }
}
void vHDMISPDDebugMsg(void)
{
	printf("show SPD infor begin=========>\n\n");
	if(TRUE == u1IsSPDSpecPatSV())
	{
		printf("HDMI detected the specific SV pattern\n");
	}
	else
	{
		printf("HDMI detected the NONE SV pattern\n");
	}
	if(TRUE == u1IsSPDSpecPatUP())
	{
		printf("HDMI detected the specific UP pattern\n");
	}
	else
	{
		printf("HDMI detected the NONE UP pattern\n");
	}
	printf("UP STATUS:%d,u1UPStaNew = %d,u1UPStaOld = %d\n",u1HdmiGetUPStatus(),u1UPStaNew,u1UPStaOld);
	//printf("SV STATUS:%d,u1SVStaNew = %d,u1SVStaOld = %d\n",u1HdmiGetSVStatus(),u1SVStaNew,u1SVStaOld);
	printf("u1HdmiSpdUPStatChk:%d\n",u1HdmiSpdUPStatChk);
	//printf("u1HdmiSpdSVStatChk:%d\n",u1HdmiSpdSVStatChk);
	printf("NOW,PRINT SPD DATA:\n\n");
	printf("SPD DATA:u4SpdData.VdrNam[0] = 0x%x\n",u4SpdData.VdrNam[0]);
	printf("SPD DATA:u4SpdData.VdrNam[1] = 0x%x\n",u4SpdData.VdrNam[1]);
	printf("SPD DATA:u4SpdData.PrdDspt[0] = 0x%x\n",u4SpdData.PrdDspt[0]);
	printf("SPD DATA:u4SpdData.PrdDspt[1] = 0x%x\n",u4SpdData.PrdDspt[1]);
	printf("SPD DATA:u4SpdData.PrdDspt[2] = 0x%x\n",u4SpdData.PrdDspt[2]);
	printf("SPD DATA:u4SpdData.PrdDspt[3] = 0x%x\n",u4SpdData.PrdDspt[3]);
	printf("SPD DATA:u4SpdData.SrcInfo = 0x%x\n",u4SpdData.SrcInfo);
	printf("<=========show SPD infor end\n\n");
}

void vHDMIDebugMsg(void)
{
	UINT8 u1Val = 0;
    UINT8 u1ClkRate= TMDS_CLOCK_UNKNOW;
	UINT8 u1DeepDepth =0;
    u1Val = IO32ReadFldAlign(N_HDMI_CTRL4 + u4HdmiRegBase,TMDS_CK_PERIOD);
	
    fgHDMISyncDetect();
    printf("width: %d\n", wHDMIResoWidth());
    printf("height: %d\n", wHDMIResoHeight());
    printf("htotal: %d\n", wHDMIHTotal());
    printf("vtotal: %d\n", wHDMIVTotal());
    printf("AVG XPCCNT: %d\n", wHDMIXPCCNT());
    printf("pixel freq: %d\n", dwHDMIPixelFreq());
    printf("line freq: %d\n", dwHDMILineFreq());
    printf("frame rate: %d\n", bHDMIRefreshRate());

	if(IS_XTAL_24MHZ())
	{
		u1ClkRate = 24*64 / u1Val;
		printf("TMDS clock is %d\n",u1ClkRate);
	}
	else
	{
		u1ClkRate = 27*64 / u1Val;
		printf("TMDS clock is %d\n",u1ClkRate);
	}

    //LOG(3, ":EQ BOOST %x\n", IO32ReadFldAlign(TMDS_CTRL1, EQSEL));
    if(fgHDMIinterlaced())
    {
        printf("interface signal\n");
    }
    else
    {
        printf("progressive signal\n");
    }

    if(u1IsHdcpStable(eActiveHdmiPort))
    {
        printf("##HDCP Stable## \n");
    }
    else
    {
        printf("!!HDCP not Stable## \n");
    }

	u1DeepDepth = bHDMIDeepColorStatus();
	switch(u1DeepDepth)
	{
		case 0:
			printf("no deep color\n");
			break;
		case 1:
			printf("10 bit deep color\n");
			break;
		case 2:
			printf("12 bit deep color\n");
			break;
		case 3:
			printf("16 bit deep color\n");
			break;
		default:
			printf("no deep color\n");
			break;
	}
    printf(" HDCP authu status %x \n ",(u1IO32Read1B(HDCP_STAT_2 + u4ActiveHdmiRegBase)& 0x30));
    vHDMIPolSta();
    vHDMIColorRelated();
    vHDMIHDMIMode();
    fgHDMIAVIFrame();
    vHDMIAVMuteSta();
    vHDMIQuerybHDMIState();
    vHDMIIntPattern();
    vHDMIReset();
    vHDMIIntSta1();
    vHDMIIntSta2();
    vHDMIIntSta3();
    vHDMIIntSta4();
    vHDMIIntSta5();
    vHDMIIntSta6();
    LOG(3, "sw: _bHdmiMode: %d\n", _bHdmiModeActive);
	LOG(0, "_bIsMhlDevice : %d\n", _bIsMhlDevice);
    LOG(0, "_bIsMhlPPMode : %d\n", _bIsMhlPPMode);
    LOG(3, "audio channel number : %d\n", API_HDMI_Get_Aud_ChannelNum());
}

void vHDMIAudioStatus(void)
{
	UINT32 wCTS_HW, wN_HW;
    UINT32 wAudSampleRate;
	UINT8 u1AudFormat =0;


  	if(IS_AUD_MUTE())
  	{
  		printf("audio mute\n");
  	}
	else
	{
		printf("audio unmute\n");
	}
    wCTS_HW = (IO32ReadFldAlign(CTS_HVAL + u4ActiveHdmiRegBase, CTS_VAL_HW19_16) << 16) | (IO32ReadFldAlign(CTS_HVAL + u4ActiveHdmiRegBase, CTS_VAL_HW15_8) << 8) | IO32ReadFldAlign(CTS_HVAL + u4ActiveHdmiRegBase, CTS_VAL_HW7_0);
    wN_HW = (IO32ReadFldAlign(N_HVAL + u4ActiveHdmiRegBase, N_VAL_HW) << 16) | (IO32ReadFldAlign(N_SVAL + u4ActiveHdmiRegBase, N_VAL_HW15_8) << 8) | IO32ReadFldAlign(N_SVAL + u4ActiveHdmiRegBase, N_VAL_HW7_0);
    //wAudSampleRate= (dwHDMIPixelFreq()*10/128) *(wN_HW/wCTS_HW);
    wAudSampleRate =IO32ReadFldAlign(LK_THRS_SVAL + u4ActiveHdmiRegBase, RHDMI_AUD_SAMPLE_F);
    //LOG(6, " HW CTS =%d  and N =%d , AudSample rate =%d\n", wCTS_HW,wN_HW,wAudSampleRate);
    printf("audio CTS =0x%x  and N = 0x%x\n", wCTS_HW, wN_HW);

	switch(wAudSampleRate)
	{
		case 0:
			printf("audio Fs : 44K\n");
			break;
		case 1:
			printf("audio Fs : unknown\n");
			break;
		case 2:
			printf("audio Fs : 48K\n");
			break;
		case 3:
			printf("audio Fs : 32K\n");
			break;
		case 8:
			printf("audio Fs : 88K\n");
			break;
		case 0xa:
			printf("audio Fs : 96K\n");
			break;
		case 0xe:
			printf("audio Fs : 192K\n");
			break;
		case 0xc:
			printf("audio Fs : 176K\n");
			break;
		default:
			break;
	}

	u1AudFormat = u1IO32Read1B(AUDRX1_0 + u4ActiveHdmiRegBase) & 0xF0;
	switch(u1AudFormat)
	{
		case 0:
			printf("audio Format: refer to Stream Header\n");
			break;
		case 1:
			printf("audio Format: LPCM\n");
			break;
		case 2:
			printf("audio Format: AC-3\n");
			break;
		case 3:
			printf("audio Format: MPEG1\n");
			break;
		case 4:
			printf("audio Format: MP3\n");
			break;
		case 5:
			printf("audio Format: MPEG2\n");
			break;
		case 6:
			printf("audio Format: AAC\n");
			break;
		case 7:
			printf("audio Format: DTS\n");
			break;
		case 9:
			printf("audio Format: One Bit Audio\n");
			break;
		case 0xA:
			printf("audio Format: Dolby Digital +\n");
			break;
		default:
			printf("audio Format: refer to Stream Header\n");
			break;
			
	}




	#if 0
	if((u1IO32Read1B(AUDRX1_0 + u4ActiveHdmiRegBase) & 0x07) == 0x0)
	{
		printf("audio channel number: refer to Stream Header\n");
	}
	else
	{
		printf("audio channel number: %d\n",(u1IO32Read1B(AUDRX1_0 + u4ActiveHdmiRegBase) & 0x07));
	}
	#endif

	if(IO32ReadFldAlign(AUDP_STAT + u4ActiveHdmiRegBase,HDMI_LAYOUT) == 0)
	{
		printf("audio channel number:2\n");
	}
	else
	{
		printf("audio channel number: %d\n",IO32ReadFldAlign(CHST0 + u4ActiveHdmiRegBase,CH_NUM1));
	}

	printf("audio channel allocation: 0x%x\n",u1IO32Read1B(AUDRX1_3 + u4ActiveHdmiRegBase));

	

}

#if HDMI_SELFTESTING
PRIVATE void vHDMICheckSMT(void)
{
    if((CKGEN_READ32(0x340) & 0x00000180) != 0x00000180)
    {
        LOG(6, "SMT setting error\n");
    }
}

PRIVATE void vHDMICheckI2CPINMUX(void)
{
    // HDCP I2C
    if((CKGEN_READ32(0x400) & 0x03000000) == 0x01000000)
    {
        LOG(6, "HDCP I2C\n");
    }
    else if((CKGEN_READ32(0x400) & 0x03000000) == 0x00000000)
    {
        LOG(6, "HDCP GPIO\n");
    }

    // EDID I2C
    if((CKGEN_READ32(0x400) & 0x00C00000) == 0x00400000)
    {
        LOG(6, "EDID I2C\n");
    }
    else if((CKGEN_READ32(0x400) & 0x00C00000) == 0x00000000)
    {
        LOG(6, "EDID GPIO\n");
    }
}

PRIVATE void vHDMICheckHDCPSram(void)
{
    // check EN first
    // check content
}

PRIVATE void vHDMICheckDVIClk(void)
{
    // pre_rgb_clk_sel
    if((CKGEN_READ32(0x2a4) & 0x10000000) != 0x10000000)
    {
        LOG(6, "pre_rgb_clk_sel error\n");
    }

    // rgb_inv_sel
    if((CKGEN_READ32(0x2a4) & 0x00001000) != 0x00001000)
    {
        LOG(6, "rgb_inv_sel: 1\n");
    }
    else
    {
        LOG(6, "rgb_inv_sel: 0\n");
    }

    if((u1IO32Read1B(TMDS_CTRL0_0 + u4ActiveHdmiRegBase) & 0x01) == 0x01)
    {
        LOG(6, "PCLK_INV: 1\n");
    }
    else
    {
        LOG(6, "PCLK_INV: 0\n");
    }

    // rgb_delay_set
    LOG(6, "rgb_delay_set: %x\n", (CKGEN_READ32(0x2a4)&0x0000007f));

    // cch_clk_stop
    if((CKGEN_READ32(0x29c) & 0x00000020) == 0x00000020)
    {
        LOG(6, "CCH_CLK STOP!\n");
    }

    // dvi_clk_stop
    if((CKGEN_READ32(0x29c) & 0x00000010) == 0x00000010)
    {
        LOG(6, "DVI_CLK STOP!\n");
    }

    // dvick_in_sel ?
    // 0x6093[6][5][4]
}

PRIVATE void vHDMICheckInterface(void)
{
    // byte order
    // 0x6056
    // bit order
    // 0x6010
}
#endif



/**
 *
 */
BOOL fgHDMISupportAudio(void)
{
    if(_bHdmiModeActive== 1)
    {
        return 1;
    }

    return 0;
}

/**
 *
 */
UINT8 u1IsHdmiMode(void)
{
    return (_bHdmiModeActive ? 1:0);
}

/**
 * HDMI audio clock check
 *
 * @retval	TRUE: have audio clock
 * @retval	FALSE: no audio clock
 */
BOOL fgHDMIAudioClkOk(void)
{
    BOOL ret;

    if(u1IO32Read1B(AUDP_STAT_0 + u4ActiveHdmiRegBase) == 0)
    {
        ret = 0;
    }
    else
    {
        ret = 1;
    }

    return ret;
    //return (IO32ReadFldAlign(AUDP_STAT,AUDP_STAT_0) == 0) ? FALSE : TRUE;
}


/**
 * HDMI get audio sampling frequency
 *
 * @retval	AUD_SAMPLE_F value in 0x22d30
 */
UINT8 u1HDMIGetAudioSamplingFreq(void)
{
    return (_bHdmiAudFreq);
}


UINT8 u1HDMI_SetHdcpKey(UINT8 *pData, UINT16 u2Length)
{
    UINT8 u1Ret = 0;
    UINT32 i = 0;
    UINT32 u4Data;

    vIO32Write4B(HDCP_ADDR,0);

    LOG(3," write hdcp key to sram...\n");

    for (i = 0; i < 320; i = i + 4)
    {
        u4Data = (pData[i+3] << 24) + (pData[i+2] << 16) + (pData[i+1] << 8) + pData[i];

        // write to hdcp sram pointer.
		vIO32Write4B(HDCP_DATA,u4Data);

        LOG(3,"0x%8x",u4Data);

        if (i % 16) { LOG(3,"\n");}
    }

    // en/adr
    vIO32Write4B(HD_DEV0, 0x1a0);
    vIO32Write4B(HD_DEV1, 0x1a0);
    vIO32Write4B(HD_DEV2, 0x1a0);
    vIO32Write4B(HD_DEV3, 0x1a0);
    // kskm
    vIO32Write4B(HD_KM0, 0xff001800);
    vIO32Write4B(HD_KS0, 0x000000C3);
    vIO32Write4B(HD_KM1, 0xff001800);
    vIO32Write4B(HD_KS1, 0x000000C3);
    vIO32Write4B(HD_KM2, 0xff001800);
    vIO32Write4B(HD_KS2, 0x000000C3);
    vIO32Write4B(HD_KM3, 0xff001800);
    vIO32Write4B(HD_KS3, 0x000000C3);
    // reload
    vIO32Write4B(HD_KM0, 0xff201800);
    vIO32Write4B(HD_KM1, 0xff201800);
    vIO32Write4B(HD_KM2, 0xff201800);
    vIO32Write4B(HD_KM3, 0xff201800);
    vIO32Write4B(HD_KM0, 0xff001800);
    vIO32Write4B(HD_KM1, 0xff001800);
    vIO32Write4B(HD_KM2, 0xff001800);
    vIO32Write4B(HD_KM3, 0xff001800);
    // rst
    vIO32Write4B(HD_SRS0,0x18070800);
    vIO32Write4B(HD_SRS0,0x18070000);
    vIO32Write4B(HD_SRS1,0x18070800);
    vIO32Write4B(HD_SRS1,0x18070000);
    vIO32Write4B(HD_SRS2,0x18070800);
    vIO32Write4B(HD_SRS2,0x18070000);
    vIO32Write4B(HD_SRS3,0x18070800);
    vIO32Write4B(HD_SRS3,0x18070000);
    // ddc en
    vIO32Write4B(HD_DDC0, 0x00009507);
    vIO32Write4B(HD_DDC1, 0x00009507);
    vIO32Write4B(HD_DDC2, 0x00009507);
    vIO32Write4B(HD_DDC3, 0x00009507);

    u1HdcpInit = 1;

    return (u1Ret);
}
static UINT8 u1PAOffset1 = 255;
static UINT8 u1write_edid_1st = 0;
UINT8 u1HDMI_SetEdidDataByPort(UINT8 port, UINT8 * pData)
{
    UINT32 i = 0;

	if(u1write_edid_1st == 0)
	{
		u1write_edid_1st = 1;
		//look for PA address
		for (i = 128 ; i < 251 ; i++)
		{
			if ((pData[i] == 0x3) && \
				(pData[i+1] == 0xc) && \
				 (pData[i+2] == 0x0))
			{
				u1PAOffset1 = i+3;
			}
		}
		
		Printf(" write hdmi edid to sram...\n");
		
		for ( i = 0 ; i < 256 ; i ++)
		{
			vIO32Write1B(PDWNC_EDID_ADDR,i);
			/*can't use vIO32Write1B to write data,because 1B will read first then write into,data address will increase 1 */
			vIO32Write4B(PDWNC_EDID_DATA, pData[i]);
		}

		Printf("HDMI RX0 PA 0x%x\n",pData[u1PAOffset1]);
		Printf("HDMI1 RX0 checksum 0x%x\n",pData[255]);
	}
	else
	{
		if(port == HDMI_SWITCH_2)
		{
			vIO32WriteFldAlign(PDWNC_EDID_CTL1, pData[u1PAOffset1], FLD_PHY_ADDR1);
			vIO32WriteFldAlign(PDWNC_EDID_CTL1, u1PAOffset1, FLD_OFFSET1);
			vIO32WriteFldAlign(PDWNC_EDID_CTL1, pData[255], FLD_CHKSUM1);
			
			Printf("HDMI RX1 PA 0x%x\n",pData[u1PAOffset1]);
			Printf("HDMI RX1 checksum 0x%x\n",pData[255]);
		}
		else if(port == HDMI_SWITCH_3)
		{
			vIO32WriteFldAlign(PDWNC_EDID_CTL2, pData[u1PAOffset1], FLD_PHY_ADDR2);
			vIO32WriteFldAlign(PDWNC_EDID_CTL2, u1PAOffset1, FLD_OFFSET2);
			vIO32WriteFldAlign(PDWNC_EDID_CTL2, pData[255], FLD_CHKSUM2);
			
			Printf("HDMI RX2 PA 0x%x\n",pData[u1PAOffset1]);
			Printf("HDMI RX2 checksum 0x%x\n",pData[255]);
		}
		
		else if(port == HDMI_SWITCH_4)
		{
			vIO32WriteFldAlign(PDWNC_EDID_CTL3, pData[u1PAOffset1], FLD_PHY_ADDR3);
			vIO32WriteFldAlign(PDWNC_EDID_CTL3, u1PAOffset1, FLD_OFFSET3);
			vIO32WriteFldAlign(PDWNC_EDID_CTL3, pData[255], FLD_CHKSUM3);
			
			Printf("HDMI RX3 PA 0x%x\n",pData[u1PAOffset1]);
			Printf("HDMI RX3 checksum 0x%x\n",pData[255]);
		}
		else
		{
			Printf("error port number: port = %d\n", port);
		}
	}
    #ifdef CC_HDMI_CONFIG_BOARD
	if((eHDMIBoardType() == ATSC_EXT_EDID)||(eHDMIBoardType() == DVB_EXT_EDID))
	{
        vIO32WriteFldAlign(PDWNC_EDID_DEV0, 1, FLD_EDID0_DIS);
	}
	else
	{
		vIO32WriteFldAlign(PDWNC_EDID_DEV0, 0, FLD_EDID0_DIS);
	}
	vIO32WriteFldAlign(PDWNC_EDID_DEV1, 0, FLD_EDID1_DIS);
	vIO32WriteFldAlign(PDWNC_EDID_DEV2, 0, FLD_EDID2_DIS);
	vIO32WriteFldAlign(PDWNC_EDID_DEV3, 0, FLD_EDID3_DIS);
	#else
	vIO32WriteFldAlign(PDWNC_EDID_DEV0, 0, FLD_EDID0_DIS);
	vIO32WriteFldAlign(PDWNC_EDID_DEV1, 0, FLD_EDID1_DIS);
	vIO32WriteFldAlign(PDWNC_EDID_DEV2, 0, FLD_EDID2_DIS);
	vIO32WriteFldAlign(PDWNC_EDID_DEV3, 0, FLD_EDID3_DIS);
	#endif

		// switch to DDC function
#ifndef CC_ENABLE_HDMI_JTAG
		BSP_PinSet(PIN_HDMI_0_SDA, 1);
		BSP_PinSet(PIN_HDMI_0_SCL, 1);
		BSP_PinSet(PIN_HDMI_1_SDA, 1);
		BSP_PinSet(PIN_HDMI_1_SCL, 1);
		BSP_PinSet(PIN_HDMI_2_SDA, 1);
		BSP_PinSet(PIN_HDMI_2_SCL, 1);
#if !defined(CC_MT5882) //MT5882 only three ports, no PIN_HDMI_3_SDA,PIN_HDMI_3_SCL.
		BSP_PinSet(PIN_HDMI_3_SDA, 1);
		BSP_PinSet(PIN_HDMI_3_SCL, 1);
#endif
#endif
    u1EdidInit = 1;

    return 1;
}

#define INTERNAL_EDID_DMSG
UINT8 u1HDMI_SetEdidData(UINT8 *pData, UINT16 u2Length)
{
    UINT8 u1Ret = 0;
    UINT32 i = 0;
    UINT8 u1PAOffset = 255;
    UINT8 u1Checksum;
    UINT8 u1Buf;
    UINT8 au1RxPA[4] = {0x10,0x20,0x30,0x40};

    if (u2Length == 260)
    {
        for ( i = 0 ; i < 4 ; i++)
        {
        	//when pdata[]=0 imply this port not use
        	if(pData[256+i]==0)
        	{
				continue;
			}
            u1Buf = (pData[256+i]  + 3) % 4;
            au1RxPA[u1Buf] = (i + 1) << 4;
            Printf(" rx%d PA= 0x%x \n", u1Buf, au1RxPA[u1Buf]);
        }
    }

    Printf(" write hdmi edid to sram...\n");

    // last byte will be block 2 checksum, three for IEEE header, two for PA, 256 - 6 = 250
    for (i = 128 ; i < 251 ; i++)
    {
        if ((pData[i] == 0x3) && \
        	(pData[i+1] == 0xc) && \
        	 (pData[i+2] == 0x0))
        {
            u1PAOffset = i+3;
        }
    }

    if (u1PAOffset < 255)
    {
#if 1
        pData[u1PAOffset] = au1RxPA[0];
#endif
        au1RxPA[0] = pData[u1PAOffset];

        Printf("HDMI RX0 PA offset %d\n",u1PAOffset);
        Printf("HDMI RX0 PA 0x%x\n",au1RxPA[0]);
    }
    else
    {
        Printf("!!!Invalid CEC Physical address offset\n");
//        return;
    }

#if 1
    u1Checksum = 0;
    for (i = 128; i < 255 ; i++)
    {
      u1Checksum +=  pData[i];
    }
    pData[255] = 0x100 - u1Checksum;
#endif

    Printf("HDMI RX0 PA 0x%x\n",au1RxPA[0]);
    Printf("HDMI1 RX0 checksum 0x%x\n",pData[255]);

    for ( i = 0 ; i < 256 ; i ++)
    {
	vIO32Write1B(PDWNC_EDID_ADDR,i);
	/*can't use vIO32Write1B to write data,because 1B will read first then write into,data address will increase 1 */
	vIO32Write4B(PDWNC_EDID_DATA, pData[i]);
    }

    u1Checksum = (pData[255] + au1RxPA[0])%256;
    /*
	0x0020,159,0x42,
	0x0030,159,0x32,
	0x0040,159,0x22
	*/
	if (u1Checksum >= au1RxPA[1])
	{
	    u1Buf = u1Checksum - au1RxPA[1];
	}
	else
	{
	    u1Buf = u1Checksum + (0x100 - au1RxPA[1]);
	}

    Printf("HDMI RX1 PA 0x%x\n",au1RxPA[1]);
    Printf("HDMI RX1 checksum 0x%x\n",u1Buf);

	vIO32WriteFldAlign(PDWNC_EDID_CTL1, au1RxPA[1], FLD_PHY_ADDR1);
	vIO32WriteFldAlign(PDWNC_EDID_CTL1, u1PAOffset, FLD_OFFSET1);
	vIO32WriteFldAlign(PDWNC_EDID_CTL1, u1Buf, FLD_CHKSUM1);

	if (u1Checksum >= au1RxPA[2])
	{
	    u1Buf = u1Checksum - au1RxPA[2];
	}
	else
	{
	    u1Buf = u1Checksum + (0x100 - au1RxPA[2]);
	}

    Printf("HDMI RX2 PA 0x%x\n",au1RxPA[2]);
    Printf("HDMI RX2 checksum 0x%x\n",u1Buf);

	vIO32WriteFldAlign(PDWNC_EDID_CTL2, au1RxPA[2], FLD_PHY_ADDR2);
	vIO32WriteFldAlign(PDWNC_EDID_CTL2, u1PAOffset, FLD_OFFSET2);
	vIO32WriteFldAlign(PDWNC_EDID_CTL2, u1Buf, FLD_CHKSUM2);

	if (u1Checksum >= au1RxPA[3])
	{
	    u1Buf = u1Checksum - au1RxPA[3];
	}
	else
	{
	    u1Buf = u1Checksum + (0x100 - au1RxPA[3]);
	}

    Printf("HDMI RX3 PA 0x%x\n",au1RxPA[3]);
    Printf("HDMI RX3 checksum 0x%x\n",u1Buf);

	vIO32WriteFldAlign(PDWNC_EDID_CTL3, au1RxPA[3], FLD_PHY_ADDR3);
	vIO32WriteFldAlign(PDWNC_EDID_CTL3, u1PAOffset, FLD_OFFSET3);
	vIO32WriteFldAlign(PDWNC_EDID_CTL3, u1Buf, FLD_CHKSUM3);


	vIO32WriteFldAlign(PDWNC_EDID_DEV0, 0, FLD_EDID0_DIS);
	vIO32WriteFldAlign(PDWNC_EDID_DEV1, 0, FLD_EDID1_DIS);
	vIO32WriteFldAlign(PDWNC_EDID_DEV2, 0, FLD_EDID2_DIS);
	vIO32WriteFldAlign(PDWNC_EDID_DEV3, 0, FLD_EDID3_DIS);


    // switch to DDC function
#ifndef CC_ENABLE_HDMI_JTAG
    BSP_PinSet(PIN_HDMI_0_SDA, 1);
    BSP_PinSet(PIN_HDMI_0_SCL, 1);
    BSP_PinSet(PIN_HDMI_1_SDA, 1);
    BSP_PinSet(PIN_HDMI_1_SCL, 1);
    BSP_PinSet(PIN_HDMI_2_SDA, 1);
    BSP_PinSet(PIN_HDMI_2_SCL, 1);
#if !defined(CC_MT5882) //MT5882 only three ports, no PIN_HDMI_3_SDA,PIN_HDMI_3_SCL.
    BSP_PinSet(PIN_HDMI_3_SDA, 1);
    BSP_PinSet(PIN_HDMI_3_SCL, 1);
#endif
#endif
    u1EdidInit = 1;

    return (u1Ret);
}

UINT8 u1HDMI_GetEdidData(UINT8 u1Port, UINT8 *pData, UINT16 u2Length)
{
    UINT8 u1Ret = 0;
    UINT8 u1Buf = 0;
    UINT16 i = 0;

    LOG(6,"built-in EDID reading, port %d:\n", u1Port);

    // check EDID data length
    if (u2Length > 256)
    {
        u2Length = 256;
    }

    // clear buffer data
    for (i = 0 ; i < u2Length ; i++)
    {
        pData[i] = 0;
    }

    // check for port mapping
    if (u1Port == 0)
    {
        LOG(6,"!! port number for edid reading should start from 1\n");
        return (u1Ret);
    }
    else
    {
        u1Port -= 1;
    }

    for (i = 0 ; i < u2Length ; i++)
    {
        if ((i%16) == 0)
        {
            LOG(6,"0x%08X ", i);
        }

        vIO32Write1B(PDWNC_EDID_ADDR,i);

        if ( i == IO32ReadFldAlign(PDWNC_EDID_CTL1 + (u1Port-1) * 4, FLD_OFFSET1) && (u1Port != 0))
        {
            u1Buf = IO32ReadFldAlign(PDWNC_EDID_CTL1 + (u1Port - 1) * 4, FLD_PHY_ADDR1);
            LOG(6,"0x%02X ", u1Buf);
        }
        else if((i == 255) && (u1Port != 0))
        {
            u1Buf = IO32ReadFldAlign(PDWNC_EDID_CTL1 + (u1Port - 1) * 4, FLD_CHKSUM1);
            LOG(6,"0x%02X ", u1Buf);
        }
        else
	 {
	     u1Buf = IO32ReadFldAlign(PDWNC_EDID_DATA, FLD_EDID_DATA);
            LOG(6,"0x%02X ", u1Buf);
	 }

        pData[i] = u1Buf;

        if (((i + 1)%16) == 0)
        {
            LOG(6,"\n");
        }

    }

    return(u1Ret);
}

UINT8 u1HDMI_GetHdcpKey(UINT8 *pData, UINT16 u2Length)
{
    UINT32 u4Idx;
    UINT32 u4Data[4];


    // write to hdcp sram pointer.
    vIO32Write4B(HDCP_ADDR,0);

    for (u4Idx = 0; u4Idx < u2Length; u4Idx += 16)
    {
  	 u4Data[0] = u4IO32Read4B(HDCP_DATA);
        u4Data[1] = u4IO32Read4B(HDCP_DATA);
        u4Data[2] = u4IO32Read4B(HDCP_DATA);
        u4Data[3] = u4IO32Read4B(HDCP_DATA);

        LOG(6,"0x%08X : 0x%08X, 0x%08X, 0x%08X, 0x%08X.\n", u4Idx, u4Data[0], u4Data[1], u4Data[2], u4Data[3]);

        pData[u4Idx] = (UINT8)(u4Data[0] >> 24) & 0xff;
        pData[u4Idx + 1] = (UINT8)(u4Data[0] >> 16) & 0xff;
        pData[u4Idx + 2] = (UINT8)(u4Data[0] >> 8) & 0xff;
        pData[u4Idx + 3] = (UINT8)u4Data[0] & 0xff;

        pData[u4Idx + 4] = (UINT8)(u4Data[1] >> 24) & 0xff;
        pData[u4Idx + 5] = (UINT8)(u4Data[1] >> 16) & 0xff;
        pData[u4Idx + 6] = (UINT8)(u4Data[1] >> 8) & 0xff;
        pData[u4Idx + 7] = (UINT8)u4Data[1] & 0xff;

        pData[u4Idx + 8] = (UINT8)(u4Data[2] >> 24) & 0xff;
        pData[u4Idx + 9] = (UINT8)(u4Data[2] >> 16) & 0xff;
        pData[u4Idx + 10] = (UINT8)(u4Data[2] >> 8) & 0xff;
        pData[u4Idx + 11] = (UINT8)u4Data[2] & 0xff;

        pData[u4Idx + 12] = (UINT8)(u4Data[3] >> 24) & 0xff;
        pData[u4Idx + 13] = (UINT8)(u4Data[3] >> 16) & 0xff;
        pData[u4Idx + 14] = (UINT8)(u4Data[3] >> 8) & 0xff;
        pData[u4Idx + 15] = (UINT8)u4Data[3] & 0xff;
    }

    return 0;
}

/*
u1OnOff
1: enable
0: disable
*/
void vHDMI_EdidOnOffCtrl(UINT8 u1Port, UINT8 u1OnOff)
{
    UINT8 u1Val = 0;

    u1Val = (u1OnOff ? 0:1);

    switch(u1Port)
    {
#ifdef CC_MT5880
	case HDMI_SWITCH_1:
		vIO32WriteFldAlign(PDWNC_EDID_DEV1, u1Val, FLD_EDID0_DIS);
		break;

	case HDMI_SWITCH_2:
	    vIO32WriteFldAlign(PDWNC_EDID_DEV2, u1Val, FLD_EDID1_DIS);
	       break;

	case HDMI_SWITCH_3:
		vIO32WriteFldAlign(PDWNC_EDID_DEV3, u1Val, FLD_EDID2_DIS);
		break;
#else
	case HDMI_SWITCH_1:
		vIO32WriteFldAlign(PDWNC_EDID_DEV0, u1Val, FLD_EDID0_DIS);
		break;

	case HDMI_SWITCH_2:
	    vIO32WriteFldAlign(PDWNC_EDID_DEV1, u1Val, FLD_EDID1_DIS);
	       break;

	case HDMI_SWITCH_3:
		vIO32WriteFldAlign(PDWNC_EDID_DEV2, u1Val, FLD_EDID2_DIS);
		break;

	case HDMI_SWITCH_4:
	    vIO32WriteFldAlign(PDWNC_EDID_DEV3, u1Val, FLD_EDID3_DIS);
		break;
#endif

	default:
		break;
    }

}

void vHDMIPowerOnOff(UINT8 power)	// 1: Normal 0: Power Down
{
    if(power)
    {
        vIO32WriteFldAlign(PD_SYS + u4ActiveHdmiRegBase, 1, PD_TOTAL);
    }
    else
    {
        vIO32WriteFldAlign(PD_SYS + u4ActiveHdmiRegBase, 0, PD_TOTAL);
    }
}

#ifdef CC_SUPPORT_STR
void HDMI_pm_suspend(void)
{
	vIO32WriteFldAlign(PD_SYS + 0xc00, 0, PD_TOTAL);
	vIO32WriteFldAlign(PD_SYS + 0x800, 0, PD_TOTAL);
	vIO32WriteFldAlign(PD_SYS + 0x400, 0, PD_TOTAL);
	vIO32WriteFldAlign(PD_SYS + 0, 0, PD_TOTAL);
}
UINT8 _au1ResumeHDCP[320];

void HDMI_pm_resume(void)
{
#ifdef CC_SUPPORT_HDMI_FBOOTING
		u1HdmiInit = 0;
#endif
		//open clock
		vIO32WriteFldAlign(CKGEN_VPCLK_STOP, 0, FLD_VDOIN_MPCLK_STOP);
		vIO32WriteFldAlign(CKGEN_VPCLK_STOP, 0, FLD_CCH_CLK_STOP);
		//open HDMI digital
		vIO32WriteFldAlign(PD_SYS + 0xc00, 1, PD_TOTAL);
		vIO32WriteFldAlign(PD_SYS + 0x800, 1, PD_TOTAL);
		vIO32WriteFldAlign(PD_SYS + 0x400, 1, PD_TOTAL);
		vIO32WriteFldAlign(PD_SYS + 0, 1, PD_TOTAL);
		//init
		vHDMIInit();
		vDviInitial();
		//load HDCP key
		u1HDMI_SetHdcpKey(_au1ResumeHDCP, sizeof(_au1ResumeHDCP));
		vUtDelay1ms(1);
		//sel port
		vHDMISel(eHDMICurrSwitch);
		eActiveHdmiPort = eHDMICurrSwitch;

		if(bHDMIPort5VStatus(eActiveHdmiPort))
		{
			_arHdmiRx[eActiveHdmiPort]._bHDMIState = HDMI_STATE_PWON;
		}
		else
		{
			_arHdmiRx[eActiveHdmiPort]._bHDMIState = HDMI_STATE_PWOFF;
		}
}
#endif

//Equalizer offset calibration 7--0--8--15
void u1HDMIEQOffsetCali(E_HDMI_SWITCH_NUM ePort)
{
	UINT8 i;
	UINT8 u1ch_lock_value[3] = {0,0,0};
	UINT8 u1ch_locked[3] = {0,0,0};
	UINT8 u1ch_value[3];
	UINT32 u4RegOffset = 0;

    u4RegOffset = u4GetAnalogRegBase(ePort);

	vIO32WriteFldAlign((REG_HDMI_RX_Normal_0_CFG1 + u4RegOffset) ,1,RG_HDMI_0_CKDT_SET);
	vIO32WriteFldAlign((REG_HDMI_RX_Normal_0_CFG5 + u4RegOffset) ,1,RG_HDMI_0_EQ_CALEN);

	//7--1
	for(i=7; i>0; i--)
	{
		vIO32WriteFldAlign((REG_HDMI_RX_Normal_0_CFG5 + u4RegOffset) ,i,RG_HDMI_0_CH0_EQMC);
		vIO32WriteFldAlign((REG_HDMI_RX_Normal_0_CFG5 + u4RegOffset) ,i,RG_HDMI_0_CH1_EQMC);
		vIO32WriteFldAlign((REG_HDMI_RX_Normal_0_CFG5 + u4RegOffset) ,i,RG_HDMI_0_CH2_EQMC);

		if((IO32ReadFldAlign((RGS_HDMI_RX_Normal_0_CFG3 + u4RegOffset), RGS_HDMI_0_CH0_EQMM) == 1) && (u1ch_locked[0] == 0))
		{
			u1ch_lock_value[0] = i;
			u1ch_locked[0] = 1;
			LOG(3, "RGS_HDMI_0_CH0_EQMM %x\n", u1ch_lock_value[0]);
		}

		if((IO32ReadFldAlign((RGS_HDMI_RX_Normal_0_CFG3 + u4RegOffset), RGS_HDMI_0_CH1_EQMM) == 1) && (u1ch_locked[1] == 0))
		{
			u1ch_lock_value[1] = i;
			u1ch_locked[1] = 1;
			LOG(3, "RGS_HDMI_0_CH1_EQMM %x\n", u1ch_lock_value[1]);
		}

		if((IO32ReadFldAlign((RGS_HDMI_RX_Normal_0_CFG3 + u4RegOffset), RGS_HDMI_0_CH2_EQMM) == 1) && (u1ch_locked[2] == 0))
		{
			u1ch_lock_value[2] = i;
			u1ch_locked[2] = 1;
			LOG(3, "RGS_HDMI_0_CH2_EQMM %x\n", u1ch_lock_value[2]);
		}

		if((u1ch_locked[0] == 1) && (u1ch_locked[1] == 1) && (u1ch_locked[2] == 1))
		{
			break;
		}
	}

	//0;8--15
	if((u1ch_locked[0] == 0) || (u1ch_locked[1] == 0) || (u1ch_locked[2] == 0))
	{
		for(i=0; i<=15; i++)
		{
			vIO32WriteFldAlign((REG_HDMI_RX_Normal_0_CFG5 + u4RegOffset) ,i,RG_HDMI_0_CH0_EQMC);
			vIO32WriteFldAlign((REG_HDMI_RX_Normal_0_CFG5 + u4RegOffset) ,i,RG_HDMI_0_CH1_EQMC);
			vIO32WriteFldAlign((REG_HDMI_RX_Normal_0_CFG5 + u4RegOffset) ,i,RG_HDMI_0_CH2_EQMC);

			if((IO32ReadFldAlign((RGS_HDMI_RX_Normal_0_CFG3 + u4RegOffset), RGS_HDMI_0_CH0_EQMM) == 1) && (u1ch_locked[0] == 0))
			{
				u1ch_lock_value[0] = i;
				u1ch_locked[0] = 1;
				LOG(3, "RGS_HDMI_0_CH0_EQMM %x\n", u1ch_lock_value[0]);
			}

			if((IO32ReadFldAlign((RGS_HDMI_RX_Normal_0_CFG3 + u4RegOffset), RGS_HDMI_0_CH1_EQMM) == 1) && (u1ch_locked[1] == 0))
			{
				u1ch_lock_value[1] = i;
				u1ch_locked[1] = 1;
				LOG(3, "RGS_HDMI_0_CH1_EQMM %x\n", u1ch_lock_value[1]);
			}

			if((IO32ReadFldAlign((RGS_HDMI_RX_Normal_0_CFG3 + u4RegOffset), RGS_HDMI_0_CH2_EQMM) == 1) && (u1ch_locked[2] == 0))
			{
				u1ch_lock_value[2] = i;
				u1ch_locked[2] = 1;
				LOG(3, "RGS_HDMI_0_CH2_EQMM %x\n", u1ch_lock_value[2]);
			}

			if((u1ch_locked[0] == 1) && (u1ch_locked[1] == 1) && (u1ch_locked[2] == 1))
			{
				break;
			}

			if(i == 0)
			{
				i = 7;
			}
		}
	}

	for(i=0; i<3; i++)
	{
		if(u1ch_lock_value[i] == 7)
		{
			u1ch_value[i] = 7;
		}
		else if(u1ch_lock_value[i] == 0)
		{
			u1ch_value[i] = 1;
		}
		else if(u1ch_lock_value[i] == 8)
		{
			u1ch_value[i] = 0;
		}
		else if(u1ch_lock_value[i] == 15)
		{
			u1ch_value[i] = 15;
		}
		else if((u1ch_lock_value[i] < 7) && (u1ch_lock_value[i] > 0))
		{
			u1ch_value[i] = u1ch_lock_value[i] + 1;
		}
		else
		{
			u1ch_value[i] = u1ch_lock_value[i] - 1;
		}
	}

	vIO32WriteFldAlign((REG_HDMI_RX_Normal_0_CFG5 + u4RegOffset) ,u1ch_value[0],RG_HDMI_0_CH0_EQMC);
	vIO32WriteFldAlign((REG_HDMI_RX_Normal_0_CFG5 + u4RegOffset) ,u1ch_value[1],RG_HDMI_0_CH1_EQMC);
	vIO32WriteFldAlign((REG_HDMI_RX_Normal_0_CFG5 + u4RegOffset) ,u1ch_value[2],RG_HDMI_0_CH2_EQMC);


	vIO32WriteFldAlign((REG_HDMI_RX_Normal_0_CFG1 + u4RegOffset) ,0,RG_HDMI_0_CKDT_SET);
	vIO32WriteFldAlign((REG_HDMI_RX_Normal_0_CFG5 + u4RegOffset) ,0,RG_HDMI_0_EQ_CALEN);
}

/**
 *
 */
 UINT8 fgHDMIActiveportNum(void)
{
    if(eHDMICurrSwitch == HDMI_SWITCH_1)
    {
        return HDMI_SWITCH_1;
    }
	else if(eHDMICurrSwitch == HDMI_SWITCH_2)
	{
        return HDMI_SWITCH_2;
	}
	else if(eHDMICurrSwitch == HDMI_SWITCH_3)
	{
        return HDMI_SWITCH_3;
	}
	else if(eHDMICurrSwitch == HDMI_SWITCH_4)
	{
        return HDMI_SWITCH_4;
	}
    return 0;
}

//for mhl
static UINT8 u1rap_onoff = 1;//0:off     1:on
void vMHLSETRAPContentOnoff(BOOL bOnoff)
{
	if((_bIsMhlDevice == 1) && (eActiveHdmiPort == HDMI_SWITCH_1))
	{
		if(bOnoff)
		{
			vHdmiUnMute(eActiveHdmiPort);
			u1rap_onoff = 1;
		}
		else
		{
			vHdmiMute(eActiveHdmiPort);
			u1rap_onoff = 0;
		}
	}
}

UINT8 vMHLGETRAPContentOnoff(void)
{
	return u1rap_onoff;
}

void vMHLAdjustChanneValuel(UINT8 value1, UINT8 value2, UINT8 value3)
{
	vIO32WriteFldAlign(MHL_REG, value1, REG_CH_ADJUST1);
	vIO32WriteFldAlign(MHL_REG, value2, REG_CH_ADJUST2);
	vIO32WriteFldAlign(MHL_REG, value3, REG_CH_ADJUST3);
}
void vMHLAdjustChannel(void)
{
	if((_bIsMhlDevice == 1) && (eActiveHdmiPort == HDMI_SWITCH_1))
	{
		if(_bIsMhlPPMode == 1)
		{
			//if(u1IsResStable(eActiveHdmiPort))
			if((IO32ReadFldAlign(VID_HRES + 0xc00, VID_HRES_STB)) && (wHDMIResoHeight() > 200) && (wHDMIVTotal() > 200))
			{
				_bResDigPhy = 0;
				if(IS_IC_5399_ES1())
				{
					vMHLHdcpErrHandle();
				}
			}
			else
			{
				_bResDigPhy++;			
			}
						
			if(_bResDigPhy > 20)
			{
				LOG(6, "vMHLAdjustChannel for PP mode...\n");
				if((IO32ReadFldAlign(MHL_REG, REG_CH_ADJUST2) == 1) ||\
				((IO32ReadFldAlign(MHL_REG, REG_CH_ADJUST1) == 0) && (IO32ReadFldAlign(MHL_REG, REG_CH_ADJUST2) == 0) && (IO32ReadFldAlign(MHL_REG, REG_CH_ADJUST3) == 0)))
				{
					vMHLAdjustChanneValuel(1, 0, 0);
					_bResDigPhy = 0;
					return;
				}
				else
				{
					vMHLAdjustChanneValuel(0, 1, 0);
					_bResDigPhy = 0;
					return;
				}
			}
		}
		else
		{
			//if(u1IsResStable(eActiveHdmiPort))
			if((IO32ReadFldAlign(VID_HRES + 0xc00, VID_HRES_STB)) && (wHDMIResoHeight() > 200) && (wHDMIVTotal() > 200))
			{
				_bResDigPhy = 0;
				if(IS_IC_5399_ES1())
				{
					vMHLHdcpErrHandle();
				}
			}
			else
			{
				_bResDigPhy++;			
			}
			
			if(_bResDigPhy > 20)
			{
				LOG(6, "vMHLAdjustChannel for DVI mode...\n");
				if((IO32ReadFldAlign(MHL_REG, REG_CH_ADJUST3) == 1) ||\
					((IO32ReadFldAlign(MHL_REG, REG_CH_ADJUST1) == 0) && (IO32ReadFldAlign(MHL_REG, REG_CH_ADJUST2) == 0) && (IO32ReadFldAlign(MHL_REG, REG_CH_ADJUST3) == 0)))
				{
					vMHLAdjustChanneValuel(1, 0, 0);
					_bResDigPhy = 0;
					LOG(6, "vMHLAdjustChannel for 1, 0, 0\n");
					return;
				}
				else if(IO32ReadFldAlign(MHL_REG, REG_CH_ADJUST2) == 1)
				{
					vMHLAdjustChanneValuel(0, 0, 1);
					_bResDigPhy = 0;
					LOG(6, "vMHLAdjustChannel for 0, 0, 1\n");
					return;
					}
				else if(IO32ReadFldAlign(MHL_REG, REG_CH_ADJUST1) == 1)
				{
					vMHLAdjustChanneValuel(0, 1, 0);
					_bResDigPhy = 0;
					LOG(6, "vMHLAdjustChannel for 0, 1, 0\n");
					return;
				}
			}
		}
	}		
}

void vMHLOpenRxTermination(BOOL bOnoff)
{
	if(bOnoff)
	{
		vIO32WriteFldAlign(REG_HDMI_RX_Normal_0_CFG2, 1, RG_HDMI_0_TERM_EN);   //61108bit16
		//connect 300ohm between DIP and DINN
		vIO32WriteFldAlign(REG_HDMI_RX_NORMAL_0_CFG6, 0x0, RG_HDMI_0_300_OHM_OFF_SEL);
	}
	else
	{
		vIO32WriteFldAlign(REG_HDMI_RX_Normal_0_CFG2, 0, RG_HDMI_0_TERM_EN);   //61108bit16
		if(_bIsMhlDevice == 1)
		{
			//disconnect 300ohm between DIP and DINN
			vIO32WriteFldAlign(REG_HDMI_RX_NORMAL_0_CFG6, 0x1, RG_HDMI_0_300_OHM_OFF_SEL);
		}
	}
}
void vMHLDisDdcSwMode(void)
{
	vIO32WriteFldAlign((MHL_HDCP_DDC + 0xc00), 0, RISC_ADDR_PAGE);
	vIO32WriteFldAlign(PDWNC_EDID_DEV0, 0, FLD_DEGLITCH0_EN);
	vIO32WriteFldAlign((SYS_CTRL + 0xc00), 0, DDC_DEL_EN);
	vIO32WriteFldAlign((MUTE_SET + 0xc00), 0, DDC_DEGLITCH_EN);
}

//MHL PPM bypass guard band
void vMHLPPMForceBypassGB(UINT8 bbypass)
{
	if(bbypass)
	{
		vIO32WriteFldAlign((MUTE_SET + 0xc00) ,1,BYPASS_DI_GB);
	}
	else
	{
		vIO32WriteFldAlign((MUTE_SET + 0xc00) ,0,BYPASS_DI_GB);
	}
}

void vMHLSetHdcpAn(UINT8* pu1An)
{
    UINT8 i;
    UINT32 u4An = 0;
    if(pu1An)
    {
    	// clock divider
    	vIO32WriteFldAlign((MHL_HDCP_DDC + 0xc00), 0x20, RISC_HDCP_CLK_DIV);

	    // TODO, set 1 at MHL mode and set 0 at non-MHL mode
    	vIO32WriteFldAlign((MHL_HDCP_DDC + 0xc00), 0x1, RISC_ADDR_PAGE);

        for (i=0;i<8;i++)
		{
        	 u4An =  ((0x18+i)<<8) + pu1An[i];
             vIO32Write4B(MHL_HDCP_REG + 0xc00,u4An);
             vUtDelay2us(25);   // T.B.D, need some time for writing into HDCP engine.
             // TODO, read back for checking.
             // note, TMDS clock must be active while writing Aksv values to hdcp engine
            /* LOG(6,"An%d:0x%x, 0x%x\n",i,pu1An[i], u4An);*/
        }

            /* LOG(6,"An:0x%x\n",IO32ReadFldAlign((SHD_AN0 + 0xc00),HDCP_AN1));
             LOG(6,"An:0x%x\n",IO32ReadFldAlign((SHD_AN0 + 0xc00),HDCP_AN2));
             LOG(6,"An:0x%x\n",IO32ReadFldAlign((SHD_AN1 + 0xc00),HDCP_AN3));
             LOG(6,"An:0x%x\n",IO32ReadFldAlign((SHD_AN1 + 0xc00),HDCP_AN4));
             LOG(6,"An:0x%x\n",IO32ReadFldAlign((SHD_AN1 + 0xc00),HDCP_AN5));
             LOG(6,"An:0x%x\n",IO32ReadFldAlign((SHD_AN1 + 0xc00),HDCP_AN6));
             LOG(6,"An:0x%x\n",IO32ReadFldAlign((SHD_BSTATUS + 0xc00),HDCP_AN7));
             LOG(6,"An:0x%x\n",IO32ReadFldAlign((SHD_BSTATUS + 0xc00),HDCP_AN8));  */           
  	}
    else
    {
        LOG(6,"set hdcp An error, null pointer\n");
    }

}

void vMHLSetHdcpAksv(UINT8* pu1Aksv)
{
    UINT8 i;
    UINT32 u4Aksv = 0;
    if(pu1Aksv)
    {

        for (i=0;i<5;i++)
		{
        	 u4Aksv =  ((0x10+i)<<8) + pu1Aksv[i];
             vIO32Write4B(MHL_HDCP_REG + 0xc00, u4Aksv);
             vUtDelay2us(25);   // T.B.D, need some time for writing into HDCP engine.
             // TODO, read back for checking.
             // note, TMDS clock must be active while writing Aksv values to hdcp engine
            // LOG(6,"AKSV%d:0x%x,0x%x \n",i,pu1Aksv[i],u4Aksv);
        }
            /* LOG(6,"Aksv:0x%x\n",IO32ReadFldAlign((SHD_AKSV + 0xc00),HDCP_AKSV1));
             LOG(6,"Aksv:0x%x\n",IO32ReadFldAlign((SHD_AKSV + 0xc00),HDCP_AKSV2));
             LOG(6,"Aksv:0x%x\n",IO32ReadFldAlign((SHD_AKSV + 0xc00),HDCP_AKSV3));
             LOG(6,"Aksv:0x%x\n",IO32ReadFldAlign((SHD_AN0 + 0xc00),HDCP_AKSV4));
             LOG(6,"Aksv:0x%x\n",IO32ReadFldAlign((SHD_AN0 + 0xc00),HDCP_AKSV5));      */  

    }
    else
    {
        LOG(6,"set hdcp Aksv error, null pointer\n");
    }

}

void vMHLGetHdcpBksv(UINT8* pu1Bksv)
{
    if(pu1Bksv)
    {
        pu1Bksv[0] = (UINT8)IO32ReadFldAlign((SHD_BKSV0 + 0xc00),HDCP_BKSV1);
        pu1Bksv[1] = (UINT8)IO32ReadFldAlign((SHD_BKSV0 + 0xc00),HDCP_BKSV2);
        pu1Bksv[2] = (UINT8)IO32ReadFldAlign((SHD_BKSV1 + 0xc00),HDCP_BKSV3);
        pu1Bksv[3] = (UINT8)IO32ReadFldAlign((SHD_BKSV1 + 0xc00),HDCP_BKSV4);
        pu1Bksv[4] = (UINT8)IO32ReadFldAlign((SHD_BKSV1 + 0xc00),HDCP_BKSV5);
        LOG(6,"Bksv:0x%x\n",IO32ReadFldAlign((SHD_BKSV0 + 0xc00),HDCP_BKSV1));
        LOG(6,"Bksv:0x%x\n",IO32ReadFldAlign((SHD_BKSV0 + 0xc00),HDCP_BKSV2));
        LOG(6,"Bksv:0x%x\n",IO32ReadFldAlign((SHD_BKSV1 + 0xc00),HDCP_BKSV3));
        LOG(6,"Bksv:0x%x\n",IO32ReadFldAlign((SHD_BKSV1 + 0xc00),HDCP_BKSV4));
        LOG(6,"Bksv:0x%x\n",IO32ReadFldAlign((SHD_BKSV1 + 0xc00),HDCP_BKSV5));		
    }
    else
    {
        LOG(0,"get hdcp Bksv error, null pointer\n");
    }
}

static UINT8 u1err_cnt = 0;
static UINT8 u1_value = 0;
void vMHLHdcpErrHandle(void)
{
    UINT8 u1Buf = 0;

    u1Buf = u1IO32Read1B(HDCP_STAT_2+ 0xc00) & 0x30; //00: No HDCP; 01: Authenticating; 11: AUTH Done

	 if((IO32ReadFldAlign(AUDP_STAT + 0xc00,STA_VID_PRMBL_DET) == 0) && (u1Buf == 0x10) && (IO32ReadFldAlign(AUDP_STAT + 0xc00,HDMI_MODE_DET) == 0))
	 {
		 u1err_cnt ++;
		 LOG(6,"detect video premble error count +\n");
	 }
	 else
	 {
		 u1err_cnt = 0;
	 }
	 
	 if(u1err_cnt > 15)
	 {
		 u1err_cnt = 0;
		 if(u1_value > 2)
		 {
			u1_value = 1;
		 }
		 else
		 {
			 u1_value++;
		 }
		 vIO32WriteFldAlign(REG_HDMI_RX_Normal_0_CFG3, u1_value, RG_HDMI_0_PLL_BAND);
		 //vUtDelay2us(5);
		 //vIO32WriteFldAlign(REG_HDMI_RX_Normal_0_CFG3 + 0x100, 0, RG_HDMI_0_PLL_BAND);
		 LOG(6,"Touch pll for dvi mode hdcp error\n");
	 }
}

void vMHLGetHdcpRi(UINT8 *pu1Ri)
{
    if(pu1Ri)
    {
        pu1Ri[1] = (UINT8)IO32ReadFldAlign((SHD_AKSV + 0xc00),HDCP_RI0_7_0);
        pu1Ri[0] = (UINT8)IO32ReadFldAlign((SHD_BKSV1 + 0xc00),HDCP_RI0_15_8);
		LOG(6,"Ri:0x%x   0x%x\n",pu1Ri[1],pu1Ri[0]);
    }
    else
    {
        LOG(6,"get hdcp Ri error, null pointer\n");
    }

}

/******************************************************************************/
UINT8 bIsMhlSource(void)
{
   if((eActiveHdmiPort == HDMI_SWITCH_1) &&(_bIsMhlDevice == 1))
   {
      return TRUE;
   }
   else
   {
      return FALSE;
   }
}
void vGetMhlPortId(UINT8 *pu1Port)
{
    if(pu1Port != NULL)
    {
        *pu1Port = HDMI_SWITCH_1;
    }
    return;
}
UINT8 vGetHDMIStatus(UINT8 u1Port)
{
	return _arHdmiRx[u1Port]._bHDMIState;
}

UINT8 API_HDMI_Get_Hdcp_Ksv(UINT8* info_ksv)
{	
    UINT8 *info_hdcp = info_ksv;
	if (!info_hdcp)
	{
	    LOG(0, "%s:%d pointer is NULL.\n", __FUNCTION__, __LINE__);
	    return 0;
	}
	
	info_hdcp[0]= u1RegRead1B(0xc00+SHD_AKSV_1);
	info_hdcp[1]= u1RegRead1B(0xc00+SHD_AKSV_2);
	info_hdcp[2]= u1RegRead1B(0xc00+SHD_AKSV_3);
	info_hdcp[3]= u1RegRead1B(0xc00+SHD_AN0_0);
	info_hdcp[4]= u1RegRead1B(0xc00+SHD_AN0_1);

	info_hdcp[5]= u1RegRead1B(0xc00+SHD_BKSV0_2);
	info_hdcp[6]= u1RegRead1B(0xc00+SHD_BKSV0_3);
	info_hdcp[7]= u1RegRead1B(0xc00+SHD_BKSV1_0);
	info_hdcp[8]= u1RegRead1B(0xc00+SHD_BKSV1_1);
	info_hdcp[9]= u1RegRead1B(0xc00+SHD_BKSV1_2);

	return 1;
}

UINT8 u1HDMI_GetSource_KSV_Data(UINT8 port, UINT8 *pKSVData)
{
    UINT32 u4RegOffset = 0x0;
    u4RegOffset = u4GetHdmiRegBase(port);

	pKSVData[0] = IO32ReadFldAlign((SHD_AKSV + u4RegOffset),HDCP_AKSV1);
	pKSVData[1] = IO32ReadFldAlign((SHD_AKSV + u4RegOffset),HDCP_AKSV2);
	pKSVData[2] = IO32ReadFldAlign((SHD_AKSV + u4RegOffset),HDCP_AKSV3);
	pKSVData[3] = IO32ReadFldAlign((SHD_AN0 + u4RegOffset),HDCP_AKSV4);
	pKSVData[4] = IO32ReadFldAlign((SHD_AN0 + u4RegOffset),HDCP_AKSV5);

	LOG(6,"the ksv data is %d,%d,%d,%d,%d",pKSVData[0],pKSVData[1],pKSVData[2],pKSVData[3],pKSVData[4]);
	return 1;
}
static UINT32 wHDMIXPCCNTByPort(E_HDMI_SWITCH_NUM u1HdmiPort)
{
    UINT32 datacnt[DATA_CNT_MAX];
    UINT32 idx, i, tmp;
	
	UINT32 u4HdmiBasebyPort;
	
#if Protect_XCLKInPCLk
    UINT32 j, k, tmp_clk[5], tmp_clk_buffer;
#endif
	u4HdmiBasebyPort = u4GetHdmiRegBase(u1HdmiPort);
    for(i = 0 ; i < DATA_CNT_MAX ; i++)
    {
        datacnt[i] = 0;
    }

    idx = 0;

    // TODO
    for(i = 0; i < 10; i++)
    {
#if Protect_XCLKInPCLk

        for(j = 0; j < XCLKInPCLkCNT; j++)
        {
            tmp_clk[j] = ((((UINT32)u1IO32Read1B(VID_CRC_OUT_2 + u4HdmiBasebyPort) & 0x7) << 8) | (UINT32)u1IO32Read1B(VID_CRC_OUT_3 + u4HdmiBasebyPort)) + 1;
            vUtDelay2us(1);
        }

        for(k = XCLKInPCLkCNT; k > 0; k--)
        {
            for(j = 0; j < k - 1; j++)
            {
                if(tmp_clk[j] > tmp_clk[j+1])
                {
                    tmp_clk_buffer = tmp_clk[j];
                    tmp_clk[j] = tmp_clk[j+1];
                    tmp_clk[j+1] = tmp_clk_buffer;
                }
            }
        }

        tmp = tmp_clk[XCLKInPCLkCNTMid];
#else
        tmp = (((u1IO32Read1B(VID_CRC_OUT_2 + u4HdmiBasebyPort) & 0x7) << 8) | u1IO32Read1B(VID_CRC_OUT_3 + u4HdmiBasebyPort)) + 1;
#endif

        if(datacnt[0] == tmp)
        {
            continue;
        }

        if(datacnt[1] == tmp)
        {
            continue;
        }

        if(idx < DATA_CNT_MAX)
        {
            datacnt[idx] = tmp;
        }

        idx++;

        if(idx == 2)
        {
            break;
        }
    }

    if(idx == 2)
    {
        return (datacnt[0] + datacnt[1])*(100 / 2);
    }
    else
    {
        return ((datacnt[0])*(100));
    }
}

static UINT8 SifHDMIWriteEeprom(UINT32 u4SDA, UINT32 u4SCL, UINT8* pu1Data)
{
    UINT8 u1Ret = 0;
    INT32 fgSet;
    UINT32 u4Index = 0;
    UINT32 u4Cnt;
    UINT32 u4DeviceAddr;

    UINT32 u4EdidWP, u4EdidWPEnable, u4Flag = 0;
    UINT16 u2edidsize=256;
    // disable write protect.
    if (0 == DRVCUST_OptQuery(eEdidEepromWPGpio, &u4EdidWP) &&
        (0 == DRVCUST_OptQuery(eEdidEepromWPEnablePolarity, &u4EdidWPEnable)))
    {
        u4Flag = 1;
        fgSet = !(INT32)u4EdidWPEnable;
        VERIFY(GPIO_Output((INT32)u4EdidWP, &fgSet) == fgSet);
    }

    Printf("Write EDID EEPROM SDA = GPIO%d, SCL = GPIO%d.\n", u4SDA, u4SCL);

    // initial SIF sda, scl pin.
    SIFSW_Init(u4SDA, u4SCL);

    u4Index = 0;
    // EEPROM page length = 8 bytes.
    while (u4Index < u2edidsize)
    {
        u4Cnt = ((u2edidsize -  u4Index) > 8) ? 8 : (u2edidsize -  u4Index);

        // lower device address [b3, b2, b1] are from address [A10, A9, A8].
        u4DeviceAddr = (u4Index >> 7) & 0x0E;
        u4DeviceAddr |= 0xA0;

        u1Ret = SIFSW_WriteData(0xA0, u4Index, u4Cnt, (UINT8*)&pu1Data[u4Index], 0xFF);

        if (u1Ret == 0)
        {
            Printf("write fail.\n");

            // enable write protect.
            if (u4Flag)
            {
                fgSet = (INT32)u4EdidWPEnable;
                VERIFY(GPIO_Output((INT32)u4EdidWP, &fgSet) == fgSet);
            }

            SIFSW_Remove();
            return 0;
        }
        // Wait EEPROM to program.
        x_thread_delay(20);

        u4Index += u4Cnt;
    }

    Printf("Write successfully !\n");

    // enable write protect.
    if (u4Flag)
    {
        fgSet = (INT32)u4EdidWPEnable;
        VERIFY(GPIO_Output((INT32)u4EdidWP, &fgSet) == fgSet);
    }

    SIFSW_Remove();

    return 1;
}

static UINT8 SifHDMIReadEeprom(UINT32 u4SDA, UINT32 u4SCL, UINT8* pu1Data, UINT32 u2Length)
{
	UINT16 i = 0;
	
    UINT32 u4Dev;
    UINT32 u4Addr;
    UINT8* pu1Buf;
    UINT32 u4ReturnValue;

    u4Dev = 0xA0;
    u4Addr = 0;
	if (u2Length > 256)
	{
		u2Length = 256;
	}
	// clear buffer data
	for (i = 0 ; i < u2Length ; i++)
	{
		pu1Data[i] = 0;
	}

    // initial SIF sda, scl pin.
    SIFSW_Init(u4SDA, u4SCL);

    pu1Buf = pu1Data;

    u4ReturnValue = SIFSW_ReadData((UINT8)u4Dev, (UINT8)u4Addr, (UINT16)u2Length, pu1Buf, 0xFFU);

    if (!u4ReturnValue)
    {
       Printf("Read error !\n");
       SIFSW_Remove();
       return 0;
    }
	for (i = 0 ; i < u2Length ; i++)
    {
	   if ((i%16) == 0)
	   {
		   LOG(6,"0x%08X ", i);
	   }
	   LOG(6,"0x%02X ", pu1Buf[i]);
	   if (((i + 1)%16) == 0)
	   {
		   LOG(6,"\n");
	   }
	}
	SIFSW_Remove();
	return 1;
}


/* for LGE port3 need extenal EDID */
UINT8 u1HDMI_SetExternalEdidDataByPort(UINT8 u1Port, UINT8 * pData)
{
    if((u1Port != 1)||(u1Port != 3))
    {
       LOG(6,"Port number not 1 or 3, return directly\n");
	   return 0;
	}
	if(u1Port == 3)
	{
		if(SifHDMIWriteEeprom(226, 227, (UINT8*)pData))
		{
			LOG(6,"Write external EDID success\n");
			return 1;	   
		}
		else
		{
			LOG(6,"Write external EDID fail\n");
			return 0;
		}
	}
	else
	{
	    if(SifHDMIWriteEeprom(220, 221, (UINT8*)pData))
	    {
            LOG(6,"Write external EDID success\n");
	        return 1;	   
	    }
	    else
	    {
	        LOG(6,"Write external EDID fail\n");
            return 0;
	    }	  
	}
}

UINT8 u1HDMI_GetExternalEdidDataByPort(UINT8 u1Port, UINT8 *pData, UINT16 u2Length)
{
    if((u1Port != 1)||(u1Port != 3))
    {
       LOG(6,"Port number not 3, return directly\n");
	   return 0;
	}
	if(u1Port == 3)
	{
		if(SifHDMIReadEeprom(226, 227, (UINT8*)pData, u2Length))
		{
			LOG(6,"Read external EDID success\n");
			return 1;	   
		}
		else
		{
			LOG(6,"Read external EDID fail\n");
			return 0;
		}
	}
	else
	{
	    if(SifHDMIReadEeprom(220, 221, (UINT8*)pData, u2Length))
	    {
            LOG(6,"Read external EDID success\n");
	        return 1;	   
	    }
	    else
	    {
	        LOG(6,"Read external EDID fail\n");
            return 0;
	    }
	}
}
static HDMI_INFOFRAME_DESCRIPTION info_data_port;
HDMI_INFOFRAME_DESCRIPTION *API_HDMI_Get_InfoFrameByPort(UINT8 u1port,UINT8 frame)
{
    UINT8 i;
    UINT32 u2startaddr=0;
    HDMI_INFOFRAME_DESCRIPTION *info = &info_data_port;
    UINT32 u4CurrentHdmiRegBase=0;
	u4CurrentHdmiRegBase = u4GetHdmiRegBase((E_HDMI_SWITCH_NUM)u1port);
	if(vGetHDMIStatus(u1port) >= HDMI_STATE_SCDT)
	{
    switch(frame)
    {
        case HDMI_INFOFRAME_AVI:
               if (u1IO32Read1B(AVIRX0_0 + u4CurrentHdmiRegBase) != 0x82)
               {
                   u2startaddr = 0;
                   info->framesize = 0;
               }
               else
               {
            u2startaddr = AVIRX0;
            info->framesize = 5;
               }
            break;

        case HDMI_INFOFRAME_SPD:
            u2startaddr = SPDRX0;
            info->framesize = 8;
            break;

        case HDMI_INFOFRAME_AUDIO:
            u2startaddr = AUDRX0;
            info->framesize = 4;
            break;

        case HDMI_INFOFRAME_MPEG:
            u2startaddr = MPEGRX0;
            info->framesize = 8;
            break;

        case HDMI_INFOFRAME_UNREC:
            u2startaddr = UNRECRX0;
            info->framesize = 8;
            break;

        case HDMI_INFOFRAME_GAMUT:
            u2startaddr = GAMUTRX0;
            info->framesize = 8;
            break;

        case HDMI_INFOFRAME_ACP:
            info->framesize = 8;

            if(IO32ReadFldAlign(INTR_STATE1 + u4CurrentHdmiRegBase, INTR6_NEW_ACP))
            {
                vIO32Write4B(INTR_STATE1 + u4CurrentHdmiRegBase, Fld2Msk32(INTR6_NEW_ACP));
                u2startaddr = ACPRX0;
            }
            else
            {
                u2startaddr = 0;
            }

            break;

        case HDMI_INFOFRAME_VSDB:
            info->framesize = 8;

            if(bHDMI3DPacketVaild() == 0) //input 2D mode
            {
                u2startaddr = 0;
            }
            else
            {
                u2startaddr = UNRECRX0;
            }

            break;

        default:
            u2startaddr = AVIRX0;
            info->framesize = 5;
            break;

    }
	}
    else
    {
        u2startaddr = 0;
        info->framesize = 0;
    }

    LOG(10, "HDMIInfoFrame size =%d\n", info->framesize);

    for(i = 0; i < (info->framesize); i++)
    {
        if(i < 10)
        {
            if(u2startaddr == 0)
            {
                info->framedata[i] = 0;
            }
            else
            {
                info->framedata[i] = u4IO32Read4B(u2startaddr + u4CurrentHdmiRegBase + (i * 4));
            }

            LOG(10, "HDMIInfoFrame=%x\n", info->framedata[i]);
        }
        else
        {
            break;
        }
    }

    return info;
}

UINT8 u1HDMI_GetModeByPort(UINT8 u1port)
{
    UINT8 ret = 0;
	UINT32 u4CurrentHdmiRegBase=0;
	u4CurrentHdmiRegBase = u4GetHdmiRegBase((E_HDMI_SWITCH_NUM)u1port);
	if(vGetHDMIStatus(u1port) >= HDMI_STATE_SCDT)
	{
        if(u1IO32Read1B(AUDP_STAT_0 + u4CurrentHdmiRegBase)& 0x01)
        {
           ret = 1;
           LOG(6,"HDMI mode\n");
        }
        else
        {
           ret = 2;
           LOG(6,"DVI mode\n");
        }
	}
	else
	{
       ret = 0;
	   LOG(6,"No signal\n");
	}
	return ret;
}
static UINT32 dwHDMIPixelFreqByPort(E_HDMI_SWITCH_NUM u1HdmiPort)
{
    UINT32 div;

    div = wHDMIXPCCNTByPort(u1HdmiPort);
	
    return ((div > 0) ? (u4XtalfreqCnt / div) : 1);
}

UINT16 wHDMIHTotalByPort(UINT8 u1Port)
{
    UINT16 tmp;
	
	UINT32 u4HdmiBasebyPort;
	u4HdmiBasebyPort = u4GetHdmiRegBase((E_HDMI_SWITCH_NUM)u1Port);
    //tmp = ((( (UINT16)u1IO32Read1B(VID_HRES_3) & 0x1f) << 8) |  (UINT16)u1IO32Read1B(VID_HRES_2));
    tmp = ((UINT16)IO32ReadFldAlign(VID_HRES + u4HdmiBasebyPort, VID_HRES_12_0));

    if((u1IO32Read1B(SYS_CTRL_0 + u4HdmiBasebyPort) & 0x30) == 0x10)  // ICLK
    {
        tmp <<= 1;
    }

    return tmp;
}
UINT16 wHDMIVTotalByPort(UINT8 u1Port)
{
    UINT16 tmp;
	
	UINT32 u4HdmiBasebyPort;
	u4HdmiBasebyPort = u4GetHdmiRegBase((E_HDMI_SWITCH_NUM)u1Port);
    tmp = ((((UINT16)u1IO32Read1B(VID_VRES_1 + u4HdmiBasebyPort) & 0x0f) << 8) | (UINT16)u1IO32Read1B(VID_VRES_0 + u4HdmiBasebyPort));
    return tmp;
}

UINT16 wHDMIResoWidthByPort(UINT8 u1Port)
{
    UINT16 tmp;
	
	UINT32 u4HdmiBasebyPort;
	u4HdmiBasebyPort = u4GetHdmiRegBase((E_HDMI_SWITCH_NUM)u1Port);
    //tmp = (((UINT16)u1IO32Read1B(VID_BLANK_3) << 8) | ((UINT16)u1IO32Read1B(VID_BLANK_2)));
    tmp = ((UINT16)IO32ReadFldAlign(VID_BLANK + u4HdmiBasebyPort, VID_DEPIXELS_11_0));

    if((u1IO32Read1B(SYS_CTRL_0 + u4HdmiBasebyPort) & 0x30) == 0x10)  // ICLK
    {
        tmp <<= 1;
    }

    return tmp;
}
UINT16 wHDMIResoHeightByPort(UINT8 u1Port)
{
    UINT16 tmp;
	
	UINT32 u4HdmiBasebyPort;
	u4HdmiBasebyPort = u4GetHdmiRegBase((E_HDMI_SWITCH_NUM)u1Port);
    //tmp = ((( (UINT16)u1IO32Read1B(VID_STAT_1) & 0x0f) << 8) | (UINT16)u1IO32Read1B(VID_STAT_0) );
    tmp = ((UINT16)IO32ReadFldAlign(VID_STAT + u4HdmiBasebyPort, VID_DELINES_11_0));
    return tmp;
}

BOOL fgHDMIinterlacedByPort(UINT8 u1Port)
{
    UINT8 u1Vid = 0;
    UINT8 u1Vid392D = 0;
	UINT32 u4HdmiBasebyPort;
	u4HdmiBasebyPort = u4GetHdmiRegBase((E_HDMI_SWITCH_NUM)u1Port);

    if (u1IO32Read1B(AVIRX0_0 + u4HdmiBasebyPort) == 0x82)
    {
       u1Vid = u1IO32Read1B(AVIRX1_3 + u4HdmiBasebyPort);

     if (((u1Vid == 20) || (u1Vid == 39)) && (wHDMIResoHeightByPort((E_HDMI_SWITCH_NUM)u1Port) == 1080) && (wHDMIHTotalByPort((E_HDMI_SWITCH_NUM)u1Port) == 2304))
     {
          u1Vid392D = 1;
     }
      else
      {
          u1Vid392D = 0;
      }
    }  
	else
	{
		//when test using pattern gen to output timing, there is no avi infoframe 
		if (((_wDviHeight == 1080) || (_wDviHeight == 540))&& (_wDviHtotal == 2304) && (bHDMIRefreshRate() == 50))
		{
			u1Vid392D = 1;		  
		}
		else
		{
			u1Vid392D = 0;
		}
	}
    if (((u1IO32Read1B(VID_CH_MAP_1 + u4HdmiBasebyPort)  & 0x04) == 0x04) || (u1Vid392D == 1))  // 2D 1080i50 with 1250 Vtotal
    {
        // LOG(6, "interface signal\n");
        return 1;
    }
    else
    {
        // LOG(6, "progressive signal\n");
        return 0;
    }
}
UINT8 bHDMIRefreshRateByPort(UINT8 u1Port)

{
    UINT32 pfreq;
    UINT8 rate;
    UINT32 dwtmp;
	
    dwtmp = wHDMIHTotalByPort((E_HDMI_SWITCH_NUM)u1Port) * wHDMIVTotalByPort((E_HDMI_SWITCH_NUM)u1Port);

    if(dwtmp == 0)  // avoid divide by zero
    {
        return 1;
    }

    pfreq = dwHDMIPixelFreqByPort((E_HDMI_SWITCH_NUM)u1Port);
    //rate = (pfreq*1000) / dwtmp;
    rate = ((pfreq * 1000) + (dwtmp>>3)) / dwtmp;  // add 1/8 ~= 12.5% for xx.97 Hz frame rates
    // LOG(6, "rate: %d\n", rate);

    if((rate <= 51) && (rate >= 49))
    {
        rate = 50;
    }
    else if((rate <= 57) && (rate >= 55))
    {
        rate = 56;
    }
    else if((rate <= 61) && (rate >= 59))
    {
        rate = 60;
    }
    else if((rate <= 68) && (rate >= 65))
    {
        rate = 67;
    }
    else if((rate <= 71) && (rate >= 69))
    {
        rate = 70;
    }
    else if((rate <= 73) && (rate >= 71))
    {
        rate = 72;
    }
    else if((rate <= 76) && (rate >= 74))
    {
        rate = 75;
    }
    else if((rate <= 86) && (rate >= 84))
    {
        rate = 85;
    }

    return rate;
}
/*UINT32 dwHDMILineFreqByPort(UINT8 u1Port)
{
    UINT32 ret;
    UINT16 wDiv;
    wDiv = wHDMIHTotalByPort((E_HDMI_SWITCH_NUM)u1Port);

    if(wDiv == 0)
    {
        return 1;
    }

    ret = ((dwHDMIPixelFreqByPort((E_HDMI_SWITCH_NUM)u1Port) * 10) / (wDiv));
    return ret;
}*/

UINT8 bHDMIScanInfoByPort(UINT8 u1Port)
{

	UINT32 u4HdmiBasebyPort;
	u4HdmiBasebyPort = u4GetHdmiRegBase((E_HDMI_SWITCH_NUM)u1Port);
    if((u1IO32Read1B(AVIRX0_0 + u4HdmiBasebyPort) != 0x82))
    {
        return 0;
    }
    else
    {
        return (u1IO32Read1B(AVIRX1_0 + u4HdmiBasebyPort)  & 0x03);
    }
}

UINT8 HDMI_3D_Get_InfoFrameByPort(UINT8 u1Port)
{
	UINT32 u4HdmiBasebyPort;
	UINT8 HDMI_3D_Structure = 0;
	
	u4HdmiBasebyPort = u4GetHdmiRegBase((E_HDMI_SWITCH_NUM)u1Port);
    if((u1IO32Read1B(UNRECRX0_0 + u4HdmiBasebyPort) == 0x81))
    {
         HDMI_3D_Structure = u1IO32Read1B(UNRECRX2_0 + u4HdmiBasebyPort) >> 4;
    }
    return HDMI_3D_Structure;
}
BOOL fgHDMIIsPixelRepeate(void)
{

    if((u1IO32Read1B(AVIRX0_0 + u4ActiveHdmiRegBase) != 0x82) || (!_bHdmiModeActive))
    {
	    return 0;
    }
    else
    {
	    return ((u1IO32Read1B(AVIRX2_0 + u4ActiveHdmiRegBase)  & 0xf));
    }
}

#ifdef CC_HDMI_CONFIG_BOARD
E_HDMI_BOARD_TYPE eHDMIBoardType(void)
{
    return eBoardType;
}
#endif
