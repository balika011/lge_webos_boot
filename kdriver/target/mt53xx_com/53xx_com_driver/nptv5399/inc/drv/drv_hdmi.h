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
 * $Date  $
 * $RCSfile: drv_hdmi.h,v $
 * $Revision: #3 $
 *
 *---------------------------------------------------------------------------*/
#ifndef _DRV_HDMI_H_
#define _DRV_HDMI_H_

#include "typedef.h"
#include "x_timer.h"

#define HDMI_8CH 0
#define MUTE_TEMP 1
#define Support_HW_Adaptive_EQ 1
#define IC_5365_VERIFY 1
#define PATCH_PIONEER_DV696_IR_OFF_ON_PURPLE_AND_SNOW_NOISE 1
#define Support_3D 1
#ifdef CC_Support_Sil9285
extern UINT8 _bHDMI9287switchstopflag;
#endif

extern UINT8 _bHPD_Indep_Ctrl;
extern UINT8 _bHDMIColorSpace;
// HDMI Event Flag
extern UINT8   _bHdmiFlag;
extern UINT8 _bHPDCycle;
extern UINT8 _bHdmiSrcGood;
extern UINT8 _bHDMIFastSW;
#define vSetHdmiFlg(arg) (_bHdmiFlag |= (arg))
#define vClrHdmiFlg(arg) (_bHdmiFlag &= (~(arg)))
#define fgIsHdmiFlgSet(arg) ((_bHdmiFlag & (arg)) > 0)
#define HDMI_CHECK (1<<0)
#define HDMI_OCLKDIV2 (1<<1)
#define HDMI_AUDIO_ON (1<<2)
#define HDMI_MODE_CHG (1<<3)

#define HDMI_RST_ALL	1
#define HDMI_RST_EQ		2
#define HDMI_RST_DEEPCOLOR	3
#define HDMI_RST_FIXEQ		4
#define HDMI_RST_RTCK		5
#define HDMI_RST_DIGITALPHY 6

#define MAX_HDMI_PORTS 4

typedef struct
{
  UINT8 _bHdmiCmd;
  UINT8 _bHDMIState;
  UINT8 _bHDMIStateOld;  
  UINT8 _u1HdmiPwrOn;
  UINT8 _u1HdmiSrcIn;
  UINT8 _u8SkipHpd;
  UINT8 _bHdmiPlug;
  UINT8 _bUnplugFlag;
  UINT8 _u8HpdLow;
  UINT8 _u1Tx5VActive;
  UINT8 _u1RxTermOn;
  
  UINT8 _bHdmiSyncActive;
  UINT8 _bCKDTMonitor;
  UINT8 _u1ClkChg;
  UINT8 _bSCDTdelay;

  UINT8 _bResStableCnt;
  
  UINT8 _u1HdcpGood;
  UINT8 _bHDMIFastSW;

  UINT8 u1HdcpStatus;
  UINT8 u1RiH;
  UINT8 u1RiL;
  UINT32 u4HdcpLoops;

  UINT8 _bRxShared;
  UINT8 _bEqTracking;
  
  UINT8 _bSyncLose;
  UINT8 _bAvMuting;
  UINT8 _bTxSTBY;
  
  HAL_TIME_T _rHdmiUnplugTime;
  HAL_TIME_T _rHdmiLowPlugWaitTime;
  
  HAL_TIME_T _rHdmiNoSyncStartTime;
  UINT8 _bNeedGetNoSyncStartTime;
  
  HAL_TIME_T _rHdmiSyncWaitTime;
  HAL_TIME_T _rHdmiAvMutingTime;
  UINT8  _bNonStdDev;
  UINT8  _bTmdsDetCnt;

#ifdef CC_SUPPORT_HDMI_CLK5VDET
	UINT8  _b5VActive;
	UINT8  _b5VDetCnt;
	UINT8  _bTmdsActive;
	UINT8  _bHpdPulse;

    UINT32 _wHpdCnt;
    UINT32 _wDeltaCur;
#endif
} HDMI_STATUS_T;

// AVI InfoFrame table.
typedef struct
{
    UINT8    type;
    UINT8    version;
    UINT8    length;
    UINT8    checksum;
    //byte1
    UINT8    s1s0: 2;
    UINT8    b1b0: 2;
    UINT8    a0: 1;
    UINT8    y1y0: 2;
    UINT8    f17: 1;
    //byte2
    UINT8    r3r0: 4;
    UINT8    m1m0: 2;
    UINT8    c1c0: 2;
    //byte3    
    UINT8    sc1sc0: 2;
    UINT8    q1q0: 2;
    UINT8    ec2ec0: 3;
    UINT8    itc: 1;
    //byte4    
    UINT8    vic6vic0: 7;
    UINT8    f47: 1;
    //byte5    
    UINT8    pr3pr0: 4;
    UINT8    cn1cn0: 2;
    UINT8    yq1yq0: 2;
    //byte 6 ~ 13
    UINT8    etb07etb00;   //Data Byte 6  ETB07-ETB00 (Line Number of End of Top Bar ¡V lower 8 bits) 
    UINT8    etb15etb08;  //Data Byte 7  ETB15-ETB08 (Line Number of End of Top Bar ¡V upper 8 bits) 
    UINT8    sbb07sbb00;  //Data Byte 8  SBB07-SBB00 (Line Number of Start of Bottom Bar ¡V lower 8 bits) 
    UINT8    sbb15sbb08;  //Data Byte 9  SBB15-SBB08 (Line Number of Start of Bottom Bar ¡V upper 8 bits) 
    UINT8    elb07elb00;  //Data Byte 10  ELB07-ELB00 (Pixel Number of End of Left Bar ¡V lower 8 bits) 
    UINT8    elb15elb08;   //Data Byte 11  ELB15-ELB08 (Pixel Number of End of Left Bar ¡V upper 8 bits) 
    UINT8    srb07srb00;  //Data Byte 12  SRB07-SRB00 (Pixel Number of Start of Right Bar ¡V lower 8 bits) 
    UINT8    srb15srb08;  //Data Byte 13  SRB15-SRB08 (Pixel Number of Start of Right Bar ¡V upper 8 bits)
}   HDMI_AVI_INFOFRAME_T;


// AUDIO InfoFrame table.
typedef struct
{
    UINT8    type;
    UINT8    version;
    UINT8    length;
    UINT8    checksum;
     //Data Byte 1  CT3  CT2  CT1  CT0  F13=0 CC2  CC1  CC0 
    UINT8    cc2cc0: 3;
    UINT8    f13: 1;
    UINT8    ct3ct0: 4;
    //Data Byte 2  F27=0  F26=0  F25=0 SF2  SF1  SF0 SS1  SS0 
    UINT8    ss1ss0: 2;
    UINT8    sf2sf0: 3;
    UINT8    f25: 1;
    UINT8    f26: 1;
    UINT8    f27: 1;    
    //Data Byte 3  F37=0  F36=0  F35=0 CXT4 CXT3 CXT2  CXT1  CXT0 
    UINT8    cxt4cxt0: 5;
    UINT8    f35: 1;
    UINT8    f36: 1;
    UINT8    f37: 1;
    //Data Byte 4  CA7  CA6 CA5 CA4 CA3 CA2  CA1  CA0     
    UINT8    ca7ca0: 8;
    //Data Byte 5  DM_INH  LSV3 LSV2 LSV1 LSV0  F52=0 LFEPBL1 LFEPBL0    
    UINT8    lfepbl1lfepbl0: 2;
    UINT8    f52: 1;
    UINT8    lsv3lsv0: 4;
    UINT8    dminh: 1;
    //Data Byte 6  F67=0  F66=0  F65=0  F64=0  F63=0  F62=0  F61=0  F60=0
    UINT8    data6;
    //Data Byte 7  F77=0  F76=0  F75=0  F74=0  F73=0  F72=0  F71=0  F70=0 
    UINT8    data7;
    //Data Byte 8  F87=0  F86=0  F85=0  F84=0  F83=0  F82=0  F81=0  F80=0
    UINT8    data8;
    //Data Byte 9  F97=0  F96=0  F95=0  F94=0  F93=0  F92=0  F91=0  F90=0 
    UINT8    data9;
    //Data Byte 10  F107=0  F106=0  F105=0  F104=0  F103=0  F102=0  F101=0  F100=0     
    UINT8    data10;
}   HDMI_AUDIO_INFOFRAME_T;

// Vendor Specific InfoFrame table.
typedef struct
{
    UINT8    type;
    UINT8    version;
    UINT8    length;
    //PB0  Checksum 
    UINT8    checksum;
    //24bit IEEE Registration Identifier (0x000C03)  ( least significant byte first ) 
    //PB1 
    UINT8    pb1;
    //PB2 
    UINT8    pb2;
    //PB3 
    UINT8    pb3;
    //PB4  HDMI_Video_Format 
    UINT8 rsvd4rsvd0: 5;
    UINT8 videoformat: 3;
    //PB5
    UINT8 rsvd2rsvd0: 3;
    UINT8 meta3d: 1;
    UINT8 struct3d: 4;
    //(PB6) 3D_Ext_Data  Reserved(0) 
    UINT8 rsvd3rsvd0: 4;
    UINT8 extdata3d: 4;
    //(PB7)  3D_Metadata_type  3D_Metadata_Length (= N) 
    UINT8 metadatalen3d: 5;
    UINT8 metatype3d: 3;
    //(PB8) 3D_Metadata_1 
    UINT8 metadata3d1;
    UINT8 metadata3d2;
    UINT8 metadata3d3;
    UINT8 metadata3d4;
    UINT8 metadata3d5;    
    //¡K ¡K 
    //B [7+N])  3D_Metadata_N 
    //8+N] ~ [Nv]  Reserved (0)  
}   HDMI_VSI_INFOFRAME_T;


typedef struct
{
  UINT8 Ch0;
  UINT8 Ch1;
  UINT8 Ch2;  
} HDMI_EQCH_T;

typedef struct
{
	UINT8 framesize;
	UINT32 framedata[10];
}HDMI_INFOFRAME_DESCRIPTION;

typedef struct
{
	UINT8 CopyRight;
	UINT8 Category_code_0_2;  // category code bit0 ~ bit2
	UINT8 Category_code_0_6;  // category code bit0 ~ bit6
	UINT8 Category_code_3;    // category code bit3
	UINT8 LBit;               // category code bit7
}HDMI_AUDIO_CHANNEL_STATUS_T;

enum 
{
	HDMI_INFOFRAME_AVI,
	HDMI_INFOFRAME_SPD,
	HDMI_INFOFRAME_AUDIO,
	HDMI_INFOFRAME_MPEG,
	HDMI_INFOFRAME_UNREC,
	HDMI_INFOFRAME_GAMUT,
	HDMI_INFOFRAME_ACP,
	HDMI_INFOFRAME_VSDB
};
enum 
{
	HDMI_STATE_NOTREADY,
	HDMI_STATE_INIT,
	HDMI_STATE_PWOFF,
	HDMI_STATE_PWON,
	HDMI_STATE_PWON2,
	HDMI_STATE_SCDT,
	HDMI_STATE_AUTH
};
enum
{
	HDMI_SPD_NONE_CHG,
	HDMI_SPD_CLEAR,//no SPD
	HDMI_SPD_CHG_TOUP,
	HDMI_SPD_CHG_TONONUP,
	HDMI_SPD_CHG_TOSV,
	HDMI_SPD_CHG_TONONSV,
};
enum
{
	HDMI_SPD_NONE_STATUS,
	HDMI_SPD_UP_UNDET,//Without specific pattern
	HDMI_SPD_UP_DETED,//With specific pattern
	HDMI_SPD_SV_UNDET,
	HDMI_SPD_SV_DETED,
};
#define HDMI_SUPPORT_SPD_IFOFRAME
#define UP_PATTERN 0xf0
#define UP_PATTERN_SONY 0x796e6f53//YNOS,mapped form ASIC Code
#define UP_PATTERN_SONY_NULL 0x00000000//NULL,mapped form ASIC Code
#define UP_PATTERN_SCEI 0X49454353//IECS,mapped form ASIC Code
#define UP_PATTERN_SCEI_NULL 0x00000000//NULL
#define SV_PATTERN 0xf
#define SV_PATTERN_SCEI 0X49454353//IECS,mapped form ASIC Code
#define SV_PATTERN_SCEI_NULL 0x00000000//NULL
#define SV_PATTERN_SCEI_PS3D 0X44335350 //D3SP,mapped form ASIC Code
typedef struct
{
	UINT32 VdrNam[2];//Vendor Name Chacter VN1 TO VN8, 8 bytes.
	UINT32 PrdDspt[4];//Product Description Chacter PD1 TO PD16,16 bytes.
	UINT8 SrcInfo;
}HDMI_SPD_DATA_STRUCT;

#if defined(CC_MT5880) 
typedef enum eHDMI_SWITCH_NO {
	HDMI_SWITCH_INIT = 0,
	HDMI_SWITCH_1,
	HDMI_SWITCH_2,
	HDMI_SWITCH_3,
	HDMI_SWITCH_MAX
}E_HDMI_SWITCH_NUM;
#else
typedef enum eHDMI_SWITCH_NO {
	HDMI_SWITCH_INIT = 0,
	HDMI_SWITCH_1,
	HDMI_SWITCH_2,
	HDMI_SWITCH_3,
	HDMI_SWITCH_4,
	HDMI_SWITCH_MAX
}E_HDMI_SWITCH_NUM;
#endif


enum eHDMI_EQ_CHANNEL {
	HDMI_EQ_CH0 = 0,
	HDMI_EQ_CH1,
	HDMI_EQ_CH2
};

enum 
{
	HDMI_3D_Structure_FramePacking=0,
	HDMI_3D_Structure_FieldAlternative,
	HDMI_3D_Structure_LineAlternative,
	HDMI_3D_Structure_SideBySideFull,
	HDMI_3D_Structure_LDepth,
	HDMI_3D_Structure_LDepthGraph,
	HDMI_3D_Structure_TopBottom,
	HDMI_3D_Structure_SideBySideHalf=8,
	HDMI_3D_Structure_Unknow
};
typedef struct
{
	UINT8 HDMI_3D_Enable;
	UINT8 HDMI_3D_Video_Format;
	UINT8 HDMI_3D_Structure;
	UINT8 HDMI_3D_EXTDATA;
}HDMI_3D_INFOFRAME;
#ifdef CC_HDMI_CONFIG_BOARD
typedef enum{
     ATSC_INT_EDID = 0,
	 ATSC_EXT_EDID,
	 DVB_INT_EDID,
	 DVB_EXT_EDID
}E_HDMI_BOARD_TYPE;
#endif
#define HDMI_AUD_Length_Unknown  	0
#define HDMI_AUD_Length_16bits  	1
#define HDMI_AUD_Length_17bits  	2
#define HDMI_AUD_Length_18bits  	3
#define HDMI_AUD_Length_19bits  	4
#define HDMI_AUD_Length_20bits  	5
#define HDMI_AUD_Length_21bits  	6
#define HDMI_AUD_Length_22bits  	7
#define HDMI_AUD_Length_23bits  	8
#define HDMI_AUD_Length_24bits  	9

#define INFORM_MDCHG 1
#ifndef LIMITED_TIMING
#define LIMITED_TIMING 0
#endif
#if 1
#define CHECHCKDT 1
#define PWR5V_INT 0
#else
#define CHECHCKDT 0
#define PWR5V_INT 1
#endif

extern UINT8  _bIsMhlDevice;//0:HDMI   1:MHL
extern UINT8  _bIsMhlPPMode;//0:24bit mode   1:packet pixel mode
extern UINT8   _bHdmiMode;
extern UINT8 _bHdmiMD;
extern UINT8 _bHdmiCmd;
extern BOOL  _fgForce3D;
extern BOOL  _fgForce2D;
extern UINT8 _bForce3DStructure;
extern UINT8 _bForce3DExtData;
extern UINT8 _bEQTuning;
extern UINT8 _bKPGain;
extern UINT8 _bRXMode;
extern UINT8 _bEQGain;
extern UINT8 _bZSel;
extern UINT8 _bVRef;

extern UINT32 u4ActiveAnalogRegBase;
extern UINT32 u4ActiveHdmiRegBase;
extern UINT32 u4AnalogRegBase;
extern UINT32 u4HdmiRegBase;
extern E_HDMI_SWITCH_NUM eActiveHdmiPort;
extern E_HDMI_SWITCH_NUM eHDMICurrSwitch;
extern E_HDMI_SWITCH_NUM eCurrHdmiPort;
#ifdef CC_SUPPORT_STR
extern UINT8 _au1ResumeHDCP[320];
#endif
extern void vHDMIInit(void) ;
extern void vHDMI_pm_resume(void) ;
void vHDMISel(E_HDMI_SWITCH_NUM eSel);
UINT8 u1IsHdcpGood(E_HDMI_SWITCH_NUM eSrc);
UINT8 u1IsResStable(E_HDMI_SWITCH_NUM eSrc);
extern void vHDMIMainLoop(void) ;
extern void vHDMISetOclkDIV2(UINT8 fgEnable) ;
extern void vHDMISetOutputDrv(UINT8 bStr) ;
extern void vHDMIPowerOff(void) ;
extern void vHDMIVideoOutOn(void) ;
extern void vHDMIVideoOutOff(void) ;
extern void vHDMIAudioOutOn(void) ;
extern BOOL fgIsHDMIPlugOn(void) ;
extern BOOL fgHDMIAudioClkOk(void);
extern BOOL fgHDMISupportAudio(void);
extern UINT8 bHDMIGetAudSampleFreq(void);
extern void vHDMIHandleAudFmtChange(void);
extern UINT8 u1HDMIGetAudioSamplingFreq(void);

void vSetHdmiState(UINT8 u1State);
UINT8 u1GetHdmiState(void);
UINT8 u1GetHdmiCmd(void);
UINT8 u1IsHdmiUnplug(E_HDMI_SWITCH_NUM ePort);
void vClrHdmiUnplug(E_HDMI_SWITCH_NUM ePort);
void vSetHdmiUnplug(E_HDMI_SWITCH_NUM ePort);
UINT8 u1IsHdmiTxSTBY(E_HDMI_SWITCH_NUM ePort);
void vSetHdmiFastSw(E_HDMI_SWITCH_NUM ePort, UINT8 u1Val);
UINT8 u1IsHdmiFastSw(E_HDMI_SWITCH_NUM ePort);
void vSetHdmiSrcIn(E_HDMI_SWITCH_NUM ePort, UINT8 u1Val);
UINT8 u1IsHdmiSrcIn(E_HDMI_SWITCH_NUM ePort);
UINT8 u1HdmiHdcpCnt(E_HDMI_SWITCH_NUM ePort);
UINT8 u1IsHdmiResStable(E_HDMI_SWITCH_NUM ePort);
UINT8 u1IsHdmiRxShared(E_HDMI_SWITCH_NUM ePort);
UINT8 u1IsHdmiMode(void);	
UINT8 u1IsHdmiHpdPulse(E_HDMI_SWITCH_NUM ePort);
void vHdmiSetHpdStatus(E_HDMI_SWITCH_NUM ePort, UINT8 u1Val);
UINT8 u1HDMI_Get5VStatus(E_HDMI_SWITCH_NUM ePort);
extern void vHDMISpdifOutEnable(BOOL fgEnable);
UINT8 bHDMIReadDevH(UINT8 bReg) ;
void vHDMIWriteDevH(UINT8 bReg, UINT8 bValue);
UINT8 bHDMIReadDevL(UINT8 bReg);
void vHDMIWriteDevL(UINT8 bReg, UINT8 bValue);
void vHDMISetSwitch(E_HDMI_SWITCH_NUM eSwitch);
extern UINT8 bHDMI3DPacketVaild(void);
#ifdef SUPPORT_HDMI_SWITCH_INIT
void vHDMISwitchInit(void);
#endif
void vHDMIGPIOInit(void);
// debug usage
#ifdef __MODEL_slt__
BOOL fgHDMICRCMulti(INT16 ntry);
#endif
BOOL fgHDMICRC(INT16 ntry);
BOOL fgHDMIPHYCRC(INT16 ntry);
BOOL fgCCIR2(INT16 ntry);
BOOL fgDVICRC(INT16 ntry);
void vHDMIBypassVdo(void);
void vHDMIHDCPKey(void);
void vHDMIReloadHDCPKey(void);
void vHDMIHDCPSelfBist(UINT8 mode);
#if 0
void vHDMIvLoadColorMatrix(BYTE idx);
#endif
#ifdef CC_SUPPORT_STR
extern void HDMI_pm_resume(void);
extern void HDMI_pm_suspend(void);
#endif
void vHDMISetHDMIState(UINT8 tmp);
void vHDMIStopFw(void);
void vHDMIResumeFw(void);
void vHDMIDebugMsg(void);
void vHDMIAudioStatus(void);
UINT16 wHDMIResoWidth(void);
UINT16 wHDMIResoHeight(void);
UINT16 wHDMIHTotal(void);
BOOL fgHDMIinterlaced(void);
BOOL fgHDMIHsyncPolarity(void);
BOOL fgHDMIVsyncPolarity(void);
UINT32 dwHDMILineFreq(void);
UINT8 bHDMIRefreshRate(void);
UINT8 bHDMIAVIPixelCount(void);
BOOL fgHDMIHsyncAct(void);
void  vHDMIGetInfoFrame(UINT8);
UINT32 bHDMIGetHPDAdjust(void);
void vHDMIHPDAdjust(UINT32);
UINT16 wHDMIInfoFrameMask(void);
UINT8 bHDMIDeepColorStatus(void);
UINT8 bHDMIMCMHdcpWrite(void);
UINT8 _bIsXpcStable(void);

UINT8 vHDMIEQ(UINT8);

UINT8 bHDMIInputType(void);
UINT8 bHDMIClocrimetry(void);
UINT8 bHDMIColorFormat(UINT8 method);
UINT8 bHDMIGetColorDepth(void);

#if INFORM_MDCHG
BOOL fgHDMIQueryModeChange(void);
#endif
void vHDMIHPDIndepCtrl(UINT8 cmd);
void vHDMITMDSIndepCTRL(E_HDMI_SWITCH_NUM ePort, UINT8 bOnOff);
void vHDMITMDSCTRL(E_HDMI_SWITCH_NUM ePort, UINT8 bOnOff);
extern void vHDMIHPDLow(E_HDMI_SWITCH_NUM eSwitch);
extern void vHDMIHPDHigh(E_HDMI_SWITCH_NUM eSwitch);
extern UINT16 wHDMIVTotal(void);
//HDMI switch related
void vHDMITMDSFloating(void);
void vHDMITMDSPullHigh(void);
UINT8 bHDMIScanInfo(void);
UINT8 bHDMIAspectRatio(void);
UINT8 bHDMIAFD(void);
UINT8 bHDMIRgbRange(void);
UINT8 bGetHDMIRangeMode(void);
void vSetHDMIRangeMode(UINT8 bMode);
HDMI_INFOFRAME_DESCRIPTION* API_HDMI_Get_InfoFrame(UINT8);
HDMI_3D_INFOFRAME* API_HDMI_3D_Get_InfoFrame(void);
void API_HDMI_Get_Aud_ChannelStatus(UINT8 u1DecId, HDMI_AUDIO_CHANNEL_STATUS_T *ChStatus);
UINT8 API_HDMI_Get_Aud_ChannelNum(void);
void bHDMIEQTwoGear(UINT8);
UINT8 bHDMI422Input(void);
UINT8 bHDMIITCFlag(void);
UINT8 bHDMIITCContent(void);
void vHDMISetEQRsel(UINT8);
void vHDMIPowerOnOff(UINT8);
UINT8 bHDMIHDCPKeyCheck(void);
UINT8 bHDMIHDCPStatusGet(void);
void vHDMIDebug(UINT8,UINT32);
void bHdmiPhyReset(E_HDMI_SWITCH_NUM ePort, UINT8 resettype);
void bHDMIAudioSigmaDeltaSetting(UINT32,UINT8,UINT8);
void bHDMIAPPL2Reset(void);
UINT32 wDVIDecoder3DForce2D(UINT8, UINT8);
 #ifdef CC_BYPASS_AVI_INFOFRAME
void vHDMIDVIBypassAVIinfo(UINT8);
UINT8 bHDMIGetDVIBypassAVIinfo(void);
#endif

UINT8 u1IsHdmiVsdbInfo(void);

#ifdef CC_AUD_HDMI_SPDIF_CFG
extern UINT8 bHDMIACPType(void);
extern UINT8 bHDMIChannelStCategoryCode(void);
extern UINT8 bHDMIChannelStCopyright(void);
extern UINT8 bHDMI_IsPCM(void);
#endif

extern void vHDMISwitchInit(void);
extern UINT8 bHDMIPort5VStatus(E_HDMI_SWITCH_NUM ePort);

extern UINT8 u1IsSyncLose(E_HDMI_SWITCH_NUM ePort);
extern void vClrSyncLose(E_HDMI_SWITCH_NUM ePort);
extern void vSetSyncLose(E_HDMI_SWITCH_NUM ePort);
extern UINT8 u1GetHDMIPort5VStatus(E_HDMI_SWITCH_NUM ePort);
extern UINT8 u1HDMI_Get4KPStatus(void);

UINT8 u1HDMI_SetHdcpKey(UINT8 *pData, UINT16 u2Length);
UINT8 u1HDMI_SetEdidData(UINT8 *pData, UINT16 u2Length);
UINT8 u1HDMI_GetEdidData(UINT8 u1Port, UINT8 *pData, UINT16 u2Length);
UINT8 u1HDMI_GetHdcpKey(UINT8 *pData, UINT16 u2Length);
void vHDMI_EdidOnOffCtrl(UINT8 u1Port, UINT8 u1OnOff);
void vHDMIGetSPDData(void);
UINT8 u1HDMISvUpPatType(void);
BOOL u1IsSPDSpecPatUP(void);
UINT8 u1HdmiGetUPStatus(void);
UINT8 u1HdmiGetUPStaChg(void);
BOOL u1IsSPDSpecPatSV(void);
UINT8 u1HdmiGetSVStatus(void);
UINT8 u1HdmiGetSVStaChg(void);
void vHDMISPDDebugMsg(void);

extern HDMI_SPD_DATA_STRUCT u4SpdData;
extern UINT8 u1UPStaNew,u1UPStaOld;
extern UINT8 u1SVStaNew,u1SVStaOld;
extern UINT8 u1HdmiSpdUPStatChk;
extern UINT8 u1HdmiSpdSVStatChk;
extern UINT8 u1IsSPDFlag;
extern UINT8 u1SpdTest;
#if 1
extern UINT8   _bHDMIAudioInit ;
extern UINT8   _bHDMISampleChange;


#endif
#if 0
extern UINT8   _bHDMIAudioForm;
extern UINT8   _bHDMIAudioForm2;
extern UINT8   _bHDMIAudioForm3;
#endif
//extern UINT8   _bHDMIAudioForm4;
extern void vHDMIUpdateAudParm(void) ;
#endif
void u1HDMIEQOffsetCali(E_HDMI_SWITCH_NUM ePort);
void vMHLPPMForceBypassGB(UINT8 bbypass);
void vMHLAdjustChannel(void);
void vMHLAdjustChanneValuel(UINT8 value1, UINT8 value2, UINT8 value3);
void vMHLHdcpErrHandle(void);

extern void vMHLSETRAPContentOnoff(BOOL bOnoff);
extern UINT8 vMHLGETRAPContentOnoff(void);
extern UINT8 fgHDMIActiveportNum(void);
extern void vMHLOpenRxTermination(BOOL bOnoff);
extern void vMHLDisDdcSwMode(void);
extern void vMHLSetHdcpAn(UINT8* pu1An);
extern void vMHLSetHdcpAksv(UINT8* pu1Aksv);
extern void vMHLGetHdcpBksv(UINT8* pu1Bksv);
extern void vMHLGetHdcpRi(UINT8 *pu1Ri);
extern UINT8 bIsMhlSource(void);
extern void vGetMhlPortId(UINT8 *pu1Port);
extern UINT8 vGetHDMIStatus(UINT8 u1Port);
extern UINT8 API_HDMI_Get_Hdcp_Ksv(UINT8* info_ksv);

#ifdef SYS_MHL_SUPPORT
#if defined(CC_MT5399) || defined(CC_MT5882)
extern UINT8 u1IsMhlSignal(void);
#endif
#endif
#if defined(CC_MT5882)
extern UINT8 u1HDMI_GetSource_KSV_Data(UINT8 port, UINT8 *pKSVData);
#endif
HDMI_INFOFRAME_DESCRIPTION *API_HDMI_Get_InfoFrameByPort(UINT8 u1port,UINT8 frame);
UINT8 u1HDMI_GetModeByPort(UINT8 u1port);
UINT16 wHDMIHTotalByPort(UINT8 u1Port);
UINT16 wHDMIVTotalByPort(UINT8 u1Port);
UINT16 wHDMIResoWidthByPort(UINT8 u1Port);
UINT16 wHDMIResoHeightByPort(UINT8 u1Port);
BOOL fgHDMIinterlacedByPort(UINT8 u1Port);
UINT8 bHDMIRefreshRateByPort(UINT8 u1Port);
UINT8 bHDMIScanInfoByPort(UINT8 u1Port);
UINT8 HDMI_3D_Get_InfoFrameByPort(UINT8 u1Port);
BOOL fgHDMIIsPixelRepeate(void);
#ifdef CC_HDMI_CONFIG_BOARD
E_HDMI_BOARD_TYPE eHDMIGetBoardType(VOID);
E_HDMI_BOARD_TYPE eHDMIBoardType(VOID);
#endif

