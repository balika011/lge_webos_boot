#ifndef _MHL_IF_H_
#define _MHL_IF_H_

#include "x_typedef.h"
#include "x_os.h"

#ifndef SUPPORT_ARM_CC
#define SUPPORT_ARM_CC
#endif

#define SIL_DEBUG(lvl,fmt...)

#define SIL_OK 1
#define SIL_FAIL 0

#define SIL_TRUE  TRUE
#define SIL_FALSE FALSE

#ifndef UNUSED
#define UNUSED(x)               (void)x
#endif

//#define USE_BOARD_CP1292
#define USE_BOARD_JULIPORT
#if !defined(USE_BOARD_CP1292)
#define DETECT_OCP	//enable for detection GPIO0 ; diable for no detection GPIO0
#endif

#define TMDS_MAPPING_For_Cp1292//1292pin22pin23#D2+/-, 1292pin24pin25#D1+/-, 1292pin26pin27#D0+/-, 1292pin28pin29#Clk+/-
#ifndef TMDS_MAPPING_For_Cp1292
#define TMDS_MAPPING_For_Rk1292//1292pin22pin23#Clk-/+, 1292pin24pin25#D0-/+, 1292pin26pin27#D1-/+, 1292pin28pin29#D2-/+
#endif

//Keno11MAY2012
//propose: HDMI CTS8-7 fail on 27MHz, disable EQ to fix.
#define EyeDigm_Magnification

#ifdef EyeDigm_Magnification
//#define DebugMode_1//Page0Reg0x13=0x80 for HDMI, Page0Reg0x13=0x01 for MHL
#ifndef DebugMode_1
#define DebugMode_2//Page0Reg0x13=0x80 for HDMI/MHL
#endif
#endif

#if 0 //
#define PsCtrl_High//PS_CTRL# is high when power on
#endif

#ifndef PsCtrl_High
#define PsCtrl_Low//PS_CTRL# is low when power on
#endif

#define MHL_RAP_NOERROR	   0x00
#define MHL_RAP_UNRECOGNIZED 0x01
#define MHL_RAP_UNSUPPORTED  0x02
#define MHL_RAP_BUSY		   0x03

//when initially, set CBUS to Hi-Z and VBUS pull low.
#define SI_Cable_RESET

#define RAP_OSD_NAME_ROLL_BACK

#define SIL_DEV_PAGE_0 0xD0
#define SIL_DEV_PAGE_8 0xC0
#define SIL_DEV_PAGE_C 0xC8

//extern uint32_t g_pass;



/*****************************************************************************************/
/*****************************************************************************************/
//TB: Top and Bottom ; LR: Left and Right ; FS: Frame Sequential
//1292 no support FS
#if 1
#define Support true
#define Unsupport false

typedef struct
{
    //byte1
    UINT8    EDID_1st_VIC_TB: 1;	// 1:  support, 0: Not support
    UINT8    EDID_1st_VIC_LR: 1;	// 1:  support, 0: Not support
    UINT8    EDID_1st_VIC_FS: 1;	// 1:  support, 0: Not support
    UINT8    EDID_1st_VIC: 1;    	// 1:  support, 0: Not support ; 
    UINT8    EDID_2nd_VIC_TB: 1;	// 1:  support, 0: Not support
    UINT8    EDID_2nd_VIC_LR: 1;	// 1:  support, 0: Not support
    UINT8    EDID_2nd_VIC_FS: 1;	// 1:  support, 0: Not support
    UINT8    EDID_2nd_VIC: 1;    	// 1:  support, 0: Not support
	
    //byte2
    UINT8    EDID_3rd_VIC_TB: 1;	// 1:  support, 0: Not support
    UINT8    EDID_3rd_VIC_LR: 1;	// 1:  support, 0: Not support
    UINT8    EDID_3rd_VIC_FS: 1;	// 1:  support, 0: Not support
    UINT8    EDID_3rd_VIC: 1;    	// 1:  support, 0: Not support
    UINT8    EDID_4th_VIC_TB: 1;	// 1:  support, 0: Not support
    UINT8    EDID_4th_VIC_LR: 1;	// 1:  support, 0: Not support
    UINT8    EDID_4th_VIC_FS: 1;	// 1:  support, 0: Not support
    UINT8    EDID_4th_VIC: 1;    	// 1:  support, 0: Not support
	
    //byte3
    UINT8    EDID_5th_VIC_TB: 1;	// 1:  support, 0: Not support
    UINT8    EDID_5th_VIC_LR: 1;	// 1:  support, 0: Not support
    UINT8    EDID_5th_VIC_FS: 1;	// 1:  support, 0: Not support
    UINT8    EDID_5th_VIC: 1;    	// 1:  support, 0: Not support
    UINT8    EDID_6th_VIC_TB: 1;	// 1:  support, 0: Not support
    UINT8    EDID_6th_VIC_LR: 1;	// 1:  support, 0: Not support
    UINT8    EDID_6th_VIC_FS: 1;	// 1:  support, 0: Not support
    UINT8    EDID_6th_VIC: 1;    	// 1:  support, 0: Not support
	
    //byte4
    UINT8    EDID_7th_VIC_TB: 1;	// 1:  support, 0: Not support
    UINT8    EDID_7th_VIC_LR: 1;	// 1:  support, 0: Not support
    UINT8    EDID_7th_VIC_FS: 1;	// 1:  support, 0: Not support
    UINT8    EDID_7th_VIC: 1;    	// 1:  support, 0: Not support
    UINT8    EDID_8th_VIC_TB: 1;	// 1:  support, 0: Not support
    UINT8    EDID_8th_VIC_LR: 1;	// 1:  support, 0: Not support
    UINT8    EDID_8th_VIC_FS: 1;	// 1:  support, 0: Not support
    UINT8    EDID_8th_VIC: 1;    	// 1:  support, 0: Not support
	
    //byte5
    UINT8    EDID_9th_VIC_TB: 1;	// 1:  support, 0: Not support
    UINT8    EDID_9th_VIC_LR: 1;	// 1:  support, 0: Not support
    UINT8    EDID_9th_VIC_FS: 1;	// 1:  support, 0: Not support
    UINT8    EDID_9th_VIC: 1;    	// 1:  support, 0: Not support
    UINT8    EDID_10th_VIC_TB: 1;	// 1:  support, 0: Not support
    UINT8    EDID_10th_VIC_LR: 1;	// 1:  support, 0: Not support
    UINT8    EDID_10th_VIC_FS: 1;	// 1:  support, 0: Not support
    UINT8    EDID_10th_VIC: 1;    // 1:  support, 0: Not support
	
    //byte6
    UINT8    EDID_11th_VIC_TB: 1;	// 1:  support, 0: Not support
    UINT8    EDID_11th_VIC_LR: 1;	// 1:  support, 0: Not support
    UINT8    EDID_11th_VIC_FS: 1;	// 1:  support, 0: Not support
    UINT8    EDID_11th_VIC: 1;    // 1:  support, 0: Not support
    UINT8    EDID_12th_VIC_TB: 1;	// 1:  support, 0: Not support
    UINT8    EDID_12th_VIC_LR: 1;	// 1:  support, 0: Not support
    UINT8    EDID_12th_VIC_FS: 1;	// 1:  support, 0: Not support
    UINT8    EDID_12th_VIC: 1;   // 1:  support, 0: Not support
	
    //byte7
    UINT8    EDID_13th_VIC_TB: 1;	// 1:  support, 0: Not support
    UINT8    EDID_13th_VIC_LR: 1;	// 1:  support, 0: Not support
    UINT8    EDID_13th_VIC_FS: 1;	// 1:  support, 0: Not support
    UINT8    EDID_13th_VIC: 1;   // 1:  support, 0: Not support
    UINT8    EDID_14th_VIC_TB: 1;	// 1:  support, 0: Not support
    UINT8    EDID_14th_VIC_LR: 1;	// 1:  support, 0: Not support
    UINT8    EDID_14th_VIC_FS: 1;	// 1:  support, 0: Not support
    UINT8    EDID_14th_VIC: 1;    // 1:  support, 0: Not support
	
    //byte8
    UINT8    EDID_15th_VIC_TB: 1;	// 1:  support, 0: Not support
    UINT8    EDID_15th_VIC_LR: 1;	// 1:  support, 0: Not support
    UINT8    EDID_15th_VIC_FS: 1;	// 1:  support, 0: Not support
    UINT8    EDID_15th_VIC: 1;    // 1:  support, 0: Not support
    UINT8    EDID_16th_VIC_TB: 1;	// 1:  support, 0: Not support
    UINT8    EDID_16th_VIC_LR: 1;	// 1:  support, 0: Not support
    UINT8    EDID_16th_VIC_FS: 1;	// 1:  support, 0: Not support
    UINT8    EDID_16th_VIC: 1;    	// 1:  support, 0: Not support
	
    //byte9
    UINT8    EDID_17th_VIC_TB: 1;	// 1:  support, 0: Not support
    UINT8    EDID_17th_VIC_LR: 1;	// 1:  support, 0: Not support
    UINT8    EDID_17th_VIC_FS: 1;	// 1:  support, 0: Not support
    UINT8    EDID_17th_VIC: 1;    // 1:  support, 0: Not support
    UINT8    EDID_18th_VIC_TB: 1;	// 1:  support, 0: Not support
    UINT8    EDID_18th_VIC_LR: 1;	// 1:  support, 0: Not support
    UINT8    EDID_18th_VIC_FS: 1;	// 1:  support, 0: Not support
    UINT8    EDID_18th_VIC: 1;    // 1:  support, 0: Not support
	
    //byte10
    UINT8    EDID_19th_VIC_TB: 1;	// 1:  support, 0: Not support
    UINT8    EDID_19th_VIC_LR: 1;	// 1:  support, 0: Not support
    UINT8    EDID_19th_VIC_FS: 1;	// 1:  support, 0: Not support
    UINT8    EDID_19th_VIC: 1;    // 1:  support, 0: Not support
    UINT8    EDID_20th_VIC_TB: 1;	// 1:  support, 0: Not support
    UINT8    EDID_20th_VIC_LR: 1;	// 1:  support, 0: Not support
    UINT8    EDID_20th_VIC_FS: 1;	// 1:  support, 0: Not support
    UINT8    EDID_20th_VIC: 1;   // 1:  support, 0: Not support
	
    //byte11
    UINT8    EDID_21th_VIC_TB: 1;	// 1:  support, 0: Not support
    UINT8    EDID_21th_VIC_LR: 1;	// 1:  support, 0: Not support
    UINT8    EDID_21th_VIC_FS: 1;	// 1:  support, 0: Not support
    UINT8    EDID_21th_VIC: 1;    // 1:  support, 0: Not support
    UINT8    EDID_22th_VIC_TB: 1;	// 1:  support, 0: Not support
    UINT8    EDID_22th_VIC_LR: 1;	// 1:  support, 0: Not support
    UINT8    EDID_22th_VIC_FS: 1;	// 1:  support, 0: Not support
    UINT8    EDID_22th_VIC: 1;   // 1:  support, 0: Not support
	
    //byte12
    UINT8    EDID_23th_VIC_TB: 1;	// 1:  support, 0: Not support
    UINT8    EDID_23th_VIC_LR: 1;	// 1:  support, 0: Not support
    UINT8    EDID_23th_VIC_FS: 1;	// 1:  support, 0: Not support
    UINT8    EDID_23th_VIC: 1;    // 1:  support, 0: Not support
    UINT8    EDID_24th_VIC_TB: 1;	// 1:  support, 0: Not support
    UINT8    EDID_24th_VIC_LR: 1;	// 1:  support, 0: Not support
    UINT8    EDID_24th_VIC_FS: 1;	// 1:  support, 0: Not support
    UINT8    EDID_24th_VIC: 1;   // 1:  support, 0: Not support
	
    //byte13
    UINT8    EDID_25th_VIC_TB: 1;	// 1:  support, 0: Not support
    UINT8    EDID_25th_VIC_LR: 1;	// 1:  support, 0: Not support
    UINT8    EDID_25th_VIC_FS: 1;	// 1:  support, 0: Not support
    UINT8    EDID_25th_VIC: 1;    // 1:  support, 0: Not support
    UINT8    EDID_26th_VIC_TB: 1;	// 1:  support, 0: Not support
    UINT8    EDID_26th_VIC_LR: 1;	// 1:  support, 0: Not support
    UINT8    EDID_26th_VIC_FS: 1;	// 1:  support, 0: Not support
    UINT8    EDID_26th_VIC: 1;    // 1:  support, 0: Not support
	
    //byte14
    UINT8    EDID_27th_VIC_TB: 1;	// 1:  support, 0: Not support
    UINT8    EDID_27th_VIC_LR: 1;	// 1:  support, 0: Not support
    UINT8    EDID_27th_VIC_FS: 1;	// 1:  support, 0: Not support
    UINT8    EDID_27th_VIC: 1;    // 1:  support, 0: Not support
    UINT8    EDID_28th_VIC_TB: 1;	// 1:  support, 0: Not support
    UINT8    EDID_28th_VIC_LR: 1;	// 1:  support, 0: Not support
    UINT8    EDID_28th_VIC_FS: 1;	// 1:  support, 0: Not support
    UINT8    EDID_28th_VIC: 1;    // 1:  support, 0: Not support
	
    //byte15
    UINT8    EDID_29th_VIC_TB: 1;	// 1:  support, 0: Not support
    UINT8    EDID_29th_VIC_LR: 1;	// 1:  support, 0: Not support
    UINT8    EDID_29th_VIC_FS: 1;	// 1:  support, 0: Not support
    UINT8    EDID_29th_VIC: 1;    // 1:  support, 0: Not support
    UINT8    EDID_30th_VIC_TB: 1;	// 1:  support, 0: Not support
    UINT8    EDID_30th_VIC_LR: 1;	// 1:  support, 0: Not support
    UINT8    EDID_30th_VIC_FS: 1;	// 1:  support, 0: Not support
    UINT8    EDID_30th_VIC: 1;    // 1:  support, 0: Not support
	
    //byte16
    UINT8    EDID_31th_VIC_TB: 1;	// 1:  support, 0: Not support
    UINT8    EDID_31th_VIC_LR: 1;	// 1:  support, 0: Not support
    UINT8    EDID_31th_VIC_FS: 1;	// 1:  support, 0: Not support
    UINT8    EDID_31th_VIC: 1;   // 1:  support, 0: Not support
    UINT8    EDID_32th_VIC_TB: 1;	// 1:  support, 0: Not support
    UINT8    EDID_32th_VIC_LR: 1;	// 1:  support, 0: Not support
    UINT8    EDID_32th_VIC_FS: 1;	// 1:  support, 0: Not support
    UINT8    EDID_32th_VIC: 1;    // 1:  support, 0: Not support
} MHL_3D_VIC_Structuration_T;

typedef struct
{
    //byte1
    UINT8    EDID_1st_DTD_TB: 1;	// 1:  support, 0: Not support
    UINT8    EDID_1st_DTD_LR: 1;	// 1:  support, 0: Not support
    UINT8    EDID_1st_DTD_FS: 1;	// 1:  support, 0: Not support
    UINT8    EDID_1st_DTD: 1;    	// 1:  support, 0: Not support
    UINT8    EDID_2nd_DTD_TB: 1;	// 1:  support, 0: Not support
    UINT8    EDID_2nd_DTD_LR: 1;	// 1:  support, 0: Not support
    UINT8    EDID_2nd_DTD_FS: 1;	// 1:  support, 0: Not support
    UINT8    EDID_2nd_DTD: 1;    // 1:  support, 0: Not support
    
    //byte2
    UINT8    EDID_3rd_DTD_TB: 1;	// 1:  support, 0: Not support
    UINT8    EDID_3rd_DTD_LR: 1;	// 1:  support, 0: Not support
    UINT8    EDID_3rd_DTD_FS: 1;	// 1:  support, 0: Not support
    UINT8    EDID_3rd_DTD: 1;    	// 1:  support, 0: Not support
    UINT8    EDID_4th_DTD_TB: 1;	// 1:  support, 0: Not support
    UINT8    EDID_4th_DTD_LR: 1;	// 1:  support, 0: Not support
    UINT8    EDID_4th_DTD_FS: 1;	// 1:  support, 0: Not support
    UINT8    EDID_4th_DTD: 1;    	// 1:  support, 0: Not support
    
    //byte3
    UINT8    EDID_5th_DTD_TB: 1;	// 1:  support, 0: Not support
    UINT8    EDID_5th_DTD_LR: 1;	// 1:  support, 0: Not support
    UINT8    EDID_5th_DTD_FS: 1;	// 1:  support, 0: Not support
    UINT8    EDID_5th_DTD: 1;    	// 1:  support, 0: Not support
    UINT8    EDID_6th_DTD_TB: 1;	// 1:  support, 0: Not support
    UINT8    EDID_6th_DTD_LR: 1;	// 1:  support, 0: Not support
    UINT8    EDID_6th_DTD_FS: 1;	// 1:  support, 0: Not support
    UINT8    EDID_6th_DTD: 1;    // 1:  support, 0: Not support
    
    //byte4
    UINT8    EDID_7th_DTD_TB: 1;	// 1:  support, 0: Not support
    UINT8    EDID_7th_DTD_LR: 1;	// 1:  support, 0: Not support
    UINT8    EDID_7th_DTD_FS: 1;	// 1:  support, 0: Not support
    UINT8    EDID_7th_DTD: 1;    	// 1:  support, 0: Not support
    UINT8    EDID_8th_DTD_TB: 1;	// 1:  support, 0: Not support
    UINT8    EDID_8th_DTD_LR: 1;	// 1:  support, 0: Not support
    UINT8    EDID_8th_DTD_FS: 1;	// 1:  support, 0: Not support
    UINT8    EDID_8th_DTD: 1;    // 1:  support, 0: Not support
    
    //byte5
    UINT8    EDID_9th_DTD_TB: 1;	// 1:  support, 0: Not support
    UINT8    EDID_9th_DTD_LR: 1;	// 1:  support, 0: Not support
    UINT8    EDID_9th_DTD_FS: 1;	// 1:  support, 0: Not support
    UINT8    EDID_9th_DTD: 1;    // 1:  support, 0: Not support
    UINT8    EDID_10th_DTD_TB: 1;	// 1:  support, 0: Not support
    UINT8    EDID_10th_DTD_LR: 1;	// 1:  support, 0: Not support
    UINT8    EDID_10th_DTD_FS: 1;	// 1:  support, 0: Not support
    UINT8    EDID_10th_DTD: 1;   // 1:  support, 0: Not support
} MHL_3D_DTD_Structuration_T;
#endif

/*****************************************************************************************/
#if defined(CC_MHL_HDMI_SWITCH)
  
enum eSIL_CHIP_ID
{
  SIL_DEVICE_NONE,
  SIL_DEVICE_9181A,
  SIL_DEVICE_9185,
  SIL_DEVICE_9189,
  SIL_DEVICE_9287,  
  SIL_DEVICE_9381A,
  SIL_DEVICE_9387,
  SIL_DEVICE_9387A,  
  SIL_DEVICE_9389,  
  SIL_DEVICE_9489,  
  SIL_DEVICE_1292,
  SIL_DEVICE_MAX
};
#endif


enum eSIL_PORT_ID
{
  SIL_PORT_NONE,
  SIL_PORT_1,
  SIL_PORT_2,
  SIL_PORT_3,
  SIL_PORT_4,
  SIL_PORT_5,
  SIL_PORT_6,
  SIL_PORT_MAX
};

typedef struct
{
    //byte1
    UINT8    SupportEdid: 1;    	//0: no support
    UINT8    SupportHdcp: 1;	//0: no support
    UINT8    SupportMhl: 1;    // 1:  support, 0: Not support
    UINT8    HdcpKeyGood: 1;	//0: no support
    UINT8    EdidKeyGood: 1;
    UINT8    Reserved5: 1;    // reserved
    UINT8    Reserved6: 1;    // reserved
    UINT8    Reserved7: 1;    // reserved
    //byte2
    UINT8    RxCableDetected: 1;
    UINT8    RxSync: 1;  // 1: Sync active
    UINT8    RxStable: 1;  // 1: Rx signal stable
    UINT8    HdcpActive: 1;  // 1: hdcp active
    UINT8    MhlHdmiDevice: 8;    // 1: MHL device, 0: HDMI/DVI device
    UINT8    HdmiMode: 1;  // 1: HDMI mode, 0: DVI mode
    UINT8    Reserved14: 1;  // reserved
    UINT8    Reserved15: 1;
    //byte3
    UINT8    TxCableDetected : 1;
    UINT8    TxOutputing: 1;
    UINT8    TxBlackMute: 1;
    UINT8    PowerSaving: 1;    // 1: in power saving mode
    UINT8    Reserved20: 1;    // reserved
    UINT8    Reserved21: 1;    // reserved
    UINT8    Reserved22: 1;    // reserved
    UINT8    Reserved23: 1;    // reserved
    //byte4
    UINT8    Reserved24: 1;    // reserved
    UINT8    Reserved25: 1;    // reserved
    UINT8    Reserved26: 1;    // reserved
    UINT8    Reserved27: 1;    // reserved
    UINT8    Reserved28: 1;    // reserved
    UINT8    Reserved29: 1;    // reserved
    UINT8    Reserved30: 1;    // reserved
    UINT8    Reserved31: 1;    // reserved
}   HDMI_SIL_STATUS_T;

typedef struct
{
    UINT8    DevCapState_DEVICE_STATE: 1;            //0=no avail ; 1=avail
    UINT8    DevCapState_MHL_VERSION: 1;             //0=no avail ; 1=avail
    UINT8    DevCapState_MHL_DEVICE_CATEGORY: 1;     //0=no avail ; 1=avail
    UINT8    DevCapState_ADOPTER_ID_H: 1;            //0=no avail ; 1=avail
    UINT8    DevCapState_ADOPTER_ID_L: 1;            //0=no avail ; 1=avail
    UINT8    DevCapState_VID_LINK_MODE: 1;           //0=no avail ; 1=avail
    UINT8    DevCapState_AUD_LINK_MODE: 1;           //0=no avail ; 1=avail
    UINT8    DevCapState_VIDEO_TYPE: 1;              //0=no avail ; 1=avail
    //byte1: DevCap0x08 to 0x0F
    UINT8    DevCapState_LOG_DEV_MAP: 1;             //0=no avail ; 1=avail
    UINT8    DevCapState_BANDWIDTH: 1;               //0=no avail ; 1=avail
    UINT8    DevCapState_FEATURE_FLAG: 1;            //0=no avail ; 1=avail
    UINT8    DevCapState_DEVICE_ID_H: 1;             //0=no avail ; 1=avail
    UINT8    DevCapState_DEVICE_ID_L: 1;             //0=no avail ; 1=avail
    UINT8    DevCapState_SCRATCHPAD_SIZE: 1;         //0=no avail ; 1=avail
    UINT8    DevCapState_INT_STAT_SIZE: 1;           //0=no avail ; 1=avail
    UINT8    DevCapState_Reserved_for_Future_Use: 1; //0=no avail ; 1=avail
    UINT8    DevCapValue_DEVICE_STATE: 8;            //byte2
    UINT8    DevCapValue_MHL_VERSION: 8;             //byte3
    UINT8    DevCapValue_MHL_DEVICE_CATEGORY: 8;     //byte4
    UINT8    DevCapValue_ADOPTER_ID_H: 8;            //byte5
    UINT8    DevCapValue_ADOPTER_ID_L: 8;            //byte6
    UINT8    DevCapValue_VID_LINK_MODE: 8;           //byte7
    UINT8    DevCapValue_AUD_LINK_MODE: 8;           //byte8
    UINT8    DevCapValue_VIDEO_TYPE: 8;              //byte9
    UINT8    DevCapValue_LOG_DEV_MAP: 8;             //byte10
    UINT8    DevCapValue_BANDWIDTH: 8;               //byte11
    UINT8    DevCapValue_FEATURE_FLAG: 8;            //byte12
    UINT8    DevCapValue_DEVICE_ID_H: 8;             //byte13
    UINT8    DevCapValue_DEVICE_ID_L: 8;             //byte11
    UINT8    DevCapValue_SCRATCHPAD_SIZE: 8;         //byte12
    UINT8    DevCapValue_INT_STAT_SIZE: 8;           //byte13
    UINT8    DevCapValue_Reserved_for_Future_Use: 8; //byte14
}HDMI_SIL_DevCap_STATUS_T;

typedef struct
{
  UINT8 MhlOCStatus:4;
  UINT8 MhlDeviceConnect:4;
}MHL_OC_STATUS;

typedef struct
{
    UINT32 port;
    UINT32 flag;
    UINT32 on;   
}MHL_SET_OPTION_T;

enum {
    /* When set to false, TV doesn't react to MHL <RAP> Content On/Off.
     * True by default. */
    DRV_MHL_OPTION_ALLOW_INPUT_SWITCHING = 101,

    /* When set to false, disables power charging through MHL cable.
     * True by default.
     */
    DRV_MHL_OPTION_POWER_CHARGE = 102,

    /* When set to false, all the CEC commands are discarded. Used when
     * user changes the TV settings to disable CEC functionality.
     * True by default.
     */
    DRV_MHL_OPTION_ENABLE_MHL = 103,
};


typedef struct
{
  UINT8    major;     // ¡§major¡¨.x.x
  UINT8    second;   //x.¡¨second¡¨.x
  UINT8    third;        //x.x.¡¨third¡¨
} HDMI_SIL_FW_VER_T;
#ifdef SYS_ANDROID_TV_TIF_SUPPORT
typedef struct
{
  UINT8 MscMsg;
  UINT8 Opcode;
  UINT8 u1Port;
  UINT8 offset;
  UINT8 Size;
  UINT8 Data[16];
  void * arg;
} CBUS_MSG_DESCRIPTION_T;
#else
typedef struct
{
  UINT8 MscMsg;
  UINT8 Opcode;
  UINT8 Size;
  UINT8 Data[16];
} CBUS_MSG_DESCRIPTION_T;
#endif
void vCbusRequestSendSratchPad(CBUS_MSG_DESCRIPTION_T *msg);

typedef struct
{
    UINT8 opcode;
    UINT8 data;
	UINT8 u1Port;
} CBUS_MSC_MSG_COMMAND_T;


typedef struct _RCP_KEY_CODE_T
{
    UINT8 u1_key_code;                /**< cbus control key code. */
    UINT32 u4_key_code;  /**< RCU code. */
} RCP_KEY_CODE_T;


#if defined(CC_MHL_HDMI_SWITCH)
// init
BOOL fgSil_Init(void);

//main loop
void vSil_MainLoop(void);
extern VOID fgSil_Enable_Cbus(BOOL bOn);
extern void fgSil_NTF_VBUS_Low(void);
extern void vSil_Delay1ms(UINT32 ms);
extern VOID fgSil_Enable_Termination(BOOL bOn);
extern void vfgSiI_MhlSetState(BOOL port, BOOL chargeCapa);
extern BOOL fgSil_Get_Chip_ID(HDMI_SIL_FW_VER_T *prSilFwVer);
extern BOOL fgSil_Get_FW_Version(HDMI_SIL_FW_VER_T *prSilVersion);
extern BOOL fgSil_GetStatus(uint8_t port, HDMI_SIL_STATUS_T *prSilStatus);
extern BOOL fgSil_Sif_ReadBytes(UINT8 DeviceID, UINT8 Offset, UINT8 *pData, UINT8 ByteCount);
extern BOOL fgSil_SetVBus(UINT8 value);
extern void vMhlSetPanelPower(UINT8 val);
extern BOOL fgSil_Set_3D_State( BOOL support);
#endif

typedef void (*x_cbus_drv_nfy_msc_subcommand) (UINT8 u1Port,UINT8 u1Opcode,UINT8 data,void *arg);
typedef void (*x_cbus_drv_nfy_scratchpad_data) (CBUS_MSG_DESCRIPTION_T* data);
typedef void (*x_cbus_drv_nfy_hot_plug) (UINT8 u1Port,UINT8 connect, void *arg);
typedef void (*x_cbus_drv_nfy_cbus_mode) (UINT8 u1Port,UINT8 mode, void *arg);
typedef void (*x_cbus_drv_nfy_vbus_overcurrent) (UINT8 u1Port,UINT8 ocStatus,void *arg);

typedef void (*x_cbus_drv_nfy_data_arrival) (CBUS_MSG_DESCRIPTION_T* msg);
typedef void (*x_cbus_drv_nfy_tx_result) (UINT8 result, void* txtag);
typedef void (*x_cbus_drv_nfy_rapk)(UINT8 RapkStatus);

typedef void (*x_vbus_drv_nfy_ocp_status) (UINT8 port, MHL_OC_STATUS result);

void vCbus_NotifyTxResult(UINT8 result, void *tag);
void vCbus_NotifyRxResult(CBUS_MSG_DESCRIPTION_T *msg);
#ifdef SYS_ANDROID_TV_TIF_SUPPORT
void vCbus_SetMscCallback(x_cbus_drv_nfy_msc_subcommand nfyFp,void *arg);
void vCbus_SetScratchPadCallback(x_cbus_drv_nfy_scratchpad_data nfyFp,void *arg);
void vCbus_SetHotPlugCallback(x_cbus_drv_nfy_hot_plug nfyFp,void *arg);
void vCbus_SetCbusModeCallback(x_cbus_drv_nfy_cbus_mode nfyFp,void *arg);
void vCbus_SetOverCurrentCallback(x_cbus_drv_nfy_vbus_overcurrent nfyFp,void *arg);
void vSendMscSubMsg(CBUS_MSC_MSG_COMMAND_T *msg);
void vSendScratchPadData(CBUS_MSG_DESCRIPTION_T *msg);
UINT8 u1MhlSetOption(MHL_SET_OPTION_T eMhlSetOption);
UINT8 u1CbusGetPeerMhlVersion(int *version);
typedef struct
{
	BOOL rapNeedNtf;
	BOOL enAbleAllNtf;
}DRV_MHL_NTF_CTRL_T;
BOOL drvMhlNtfEnable(void);
BOOL drvMhlNtfRapEnable(void);
BOOL drvMhlStandbyNeedCharge(void);
#endif

void vCbus_SetRxCallback(x_cbus_drv_nfy_data_arrival nfyfp);
void vCbus_SetTxCallback(x_cbus_drv_nfy_tx_result nfyfp);

void vCbus_SetRAPKCallback(x_cbus_drv_nfy_rapk nfyfp);
void MHL_SetOCPCallback(x_vbus_drv_nfy_ocp_status nfyfp);
#if defined(CC_MHL_HDMI_SWITCH)
BOOL fgVbus_GetOcpStatus(UINT8 port);
#elif defined(CC_MT5399) || defined(CC_MT5881) || defined(CC_MT5890) || defined(CC_MT5882) 
BOOL mhlVbus_GetOcpStatus(UINT8 port);
#endif


void _CheckMhlStatus(UINT8 val);
#ifdef SYS_MHL_SUPPORT
UINT8 _IsMhlDevice(void);
UINT8 bSendRcpKeys(UINT8 Key, UINT8 event);
#endif
#ifdef CC_MHL_HDMI_SWITCH
// power control
BOOL fgSil_PowerSaving(UINT8 value);     // 1: into power saving mode, 0: wakeup from power saving mode

// get chip ID
BOOL fgSil_Get_Chip_ID(HDMI_SIL_FW_VER_T *prSilFwVer);

// firmware version
BOOL fgSil_Get_FW_Version(HDMI_SIL_FW_VER_T *prSilVersion);

// status
//BOOL fgSil_GetStatus(UINT8 port, HDMI_SIL_STATUS_T *prSilStatus);

// status
BOOL fgSil_PortSel(UINT8 sel);

// SIF Function
BOOL Sif_WriteBytes(UINT8 DeviceID, UINT8 Offset, UINT8 *pData, UINT8 ByteCount);
BOOL Sif_ReadBytes(UINT8 DeviceID, UINT8 Offset, UINT8 *pData, UINT8 ByteCount);

// timer
BOOL Timer_Set(UINT8 Index, UINT32 MilliSecond);  // in ms
UINT32 u4Timer_Get(UINT8 Index);     // Return Value=Timer Count Value
BOOL Timer_Expired(UINT8 Index);    // 0=Timer Out, 0!=Timer Keep Going
void Delay1ms(UINT32 ms);    // Delay in millisecond
void Delay1us(UINT32 us);   // Delay in microsecond
#endif

#if defined(CC_MHL_HDMI_SWITCH)
// hardwaare reset
BOOL fgSil_ChipRst(UINT8 Polarity, UINT32 MicroSecond);   // 1: normal high, 0: normal low

BOOL fgSil_NFYRcp(UINT8 key);//nofity RCP:: Select:0x00...etc.

BOOL fgSil_NFYRAP(UINT8 key);//notify RAP:: POLL:0x00, CONTENT_ON:0x10, CONTENT_OFF: 0x11
BOOL fgSil_NFYRAPK(UINT8 key);//notify RAPK::no error:0x00,UnRecongnized:0x01,keyUnsupport:0x02,Busy:0x03
BOOL IsRevRapK(void);
BOOL fgSil_RapKeys(UINT8 key);

BOOL fgSil_NFYWriteBurstData(UINT8 key, UINT8 *pdata, UINT8 size);//notify MSC:: MSC_WRITE_BURST = 0x6c ...etc.

BOOL fgSil_SendWriteBurstData(UINT8 key, UINT8 *pdata, UINT8 size);//notify MSC:: MSC_WRITE_BURST = 0x6c ...etc.

void fgSil_PrintAllRegs(void);
#elif defined(CC_MT5399) || defined(CC_MT5881) || defined(CC_MT5890) || defined(CC_MT5882)
BOOL MhlRst(UINT8 Polarity, UINT32 MicroSecond);   // 1: normal high, 0: normal low

BOOL Mhl_NFYRcp(UINT8 key);//nofity RCP:: Select:0x00...etc.
BOOL Mhl_NFYRcpK(UINT8 key);
BOOL Mhl_NFYRcpE(UINT8 u1ErrorCode);
BOOL Mhl_NFYUcp(UINT8 key);
BOOL Mhl_NFYUcpK(UINT8 key);
BOOL Mhl_NFYUcpE(UINT8 u1ErrorCode);
BOOL Mhl_NFYRAP(UINT8 key);//notify RAP:: POLL:0x00, CONTENT_ON:0x10, CONTENT_OFF: 0x11
BOOL Mhl_NFYRAPK(UINT8 key);//notify RAPK::no error:0x00,UnRecongnized:0x01,keyUnsupport:0x02,Busy:0x03
BOOL IsRevRapK(void);
BOOL Mhl_RapKeys(UINT8 key);
#if defined(CC_MT5399) || defined(CC_MT5881) 
BOOL Mhl_NFYWriteBurstData(UINT8 key, UINT8 *pdata, UINT8 size);//notify MSC:: MSC_WRITE_BURST = 0x6c ...etc.
#elif defined(CC_MT5890) || defined(CC_MT5882)
BOOL Mhl_NFYWriteBurstData(UINT8 key, UINT16 *pdata, UINT16 size);//notify MSC:: MSC_WRITE_BURST = 0x6c ...etc.
#endif
BOOL Mhl_SendWriteBurstData(UINT8 key, UINT8 *pdata, UINT8 size);//notify MSC:: MSC_WRITE_BURST = 0x6c ...etc.

void Mhl_PrintAllRegs(void);
void Mhl_NotifyHotPlug(UINT8 u1Port,UINT8 u1Oc);
#endif
#ifdef SYS_MHL_SUPPORT
#ifdef CC_MHL_HDMI_SWITCH
UINT8 u1MhlGetOcpStatus(UINT8 ports);
void vMhlGo(void);
void vIntEn(INT32 En);
#else
UINT8 u1MhlGetOcpStatus(UINT8 ports);
#endif
#endif
void vMhlInit(void);

#if defined(CC_MHL_HDMI_SWITCH)
#ifdef DETECT_OCP
BOOL fgSil_NFYOCPStatus(UINT8 port, UINT8 status);//notify GPIO0 status:: 0x01: GPIO0=1, 0x00: GPIO0=0. For Over Current Protaction.
BOOL fgSil_GetOCPStatus(UINT8 port); // OCP: true, Non-OCP: false.

//Set PS_CTRL to 0 or 1. For control VBUS power out or not.
//Control Confition: If current is MHL mode and OCP(GPIO0) was happened event.
BOOL MhlSetVBus(UINT8 value);
#endif
#elif defined(CC_MT5399) || defined(CC_MT5881) || defined(CC_MT5890) || defined(CC_MT5882)
BOOL Mhl_NFYOCPStatus(UINT8 port, UINT8 status);//notify GPIO0 s tatus:: 0x01: GPIO0=1, 0x00: GPIO0=0. For Over Current Protaction.
BOOL Mhl_GetOCPStatus(UINT8 port); // OCP: true, Non-OCP: false.
#ifdef SYS_MHL_SUPPORT
void vOcpHandler(void);
#endif

//Set PS_CTRL to 0 or 1. For control VBUS power out or not.
//Control Confition: If current is MHL mode and OCP(GPIO0) was happened event.
BOOL MhlSetVBus(UINT8 value);

#endif

#if defined(CC_MHL_HDMI_SWITCH)
//DevCap
//void fgSil_NTFDevCap(HDMI_SIL_DevCap_STATUS_T *pHDMI_SIL_DevCap_STATUS);//notify Device Capability for SoC know.
BOOL fgSil_MoificationPage0Reg0x13(UINT8 port, UINT8 data);

BOOL fgSil_NTF_MSC_CMD(UINT8 port, UINT8 MscCmd, UINT8 OpCode, UINT8 Data);

#ifdef SI_Cable_RESET
//Keno20120608, let SoC to decide MHL toggle time, like HPD toggle
//Rlease mean PS_CTRL# low & CBUS Hi-Z & MHL termination discable
//Recovery mean PS_CTRL high & CBUS default 1kohm & MHL termination enable
BOOL fgSil_MHLSignalControl(UINT8 HiZ, UINT8 TermOff, UINT8 VbusDisable);
BOOL fgSil_RecoveryMHLCable(void);
#endif

UINT8 u1MhlGetCableStatus(UINT8 ports);
BOOL vIsSonyMHLDevice(VOID);
void u1ClearMHLDeviceAdapterID(VOID);
UINT8 u1MhlGetPortId(UINT8 ports);

void vMhlSetSourceStatus(UINT8 port, UINT8 status);
void vMhlSetRapOnOff(UINT8 port, UINT8 OnOff);
UINT8 fgSil_GetSystemPortStatus(UINT8 port);//ON will return 0x00 ; OFF will return0x02 ; BUSY will return0x03
void vMhlSetRapContentOn(UINT8 OnOff);
UINT8 vGetMhlRapContentON(void);

//mhl 2.0
BOOL fgSil_Set_MHL2_State( BOOL MHL2_Support,BOOL Support_Charger_900mA_Or_1500mA);
BOOL fgSil_Set_3D_State(BOOL support);//when system initially, SoC calls fgSil_Set_3D_State for desiction DUT supports 3D or not.
//BOOL SI_3D_EDID_Configuration_VIC_DTD(MHL_3D_VIC_Structuration_T *p3D_VIC_Struct, MHL_3D_DTD_Structuration_T *p3D_DTD_Struct);
void fgSil_NTF_CBUS_Connected(void);
void fgSil_NTF_CBUS_Disconnected(void);
void fgSil_NTF_3D_Communicated(void);
BOOL SI_3DConfig_For_3D_Not_Supported(BOOL support);
void _MHL_CheckDeviceStatus(void);
UINT8 bMHLDeviceState(UINT8 port);
BOOL Mhl_SendWriteBurstRequest(void);

#elif defined(CC_MT5399) || defined(CC_MT5881) || defined(CC_MT5890) || defined(CC_MT5882)
//DevCap
void Mhl_NTFDevCap(HDMI_SIL_DevCap_STATUS_T *pHDMI_SIL_DevCap_STATUS);//notify Device Capability for SoC know.
BOOL Mhl_NTF_MSC_CMD(UINT8 port, UINT8 MscCmd, UINT8 OpCode, UINT8 Data);
#ifdef SI_Cable_RESET
//Keno20120608, let SoC to decide MHL toggle time, like HPD toggle
//Rlease mean PS_CTRL# low & CBUS Hi-Z & MHL termination discable
//Recovery mean PS_CTRL high & CBUS default 1kohm & MHL termination enable
BOOL Mhl_MHLSignalControl(UINT8 HiZ, UINT8 TermOff, UINT8 VbusDisable);
BOOL Mhl_RecoveryMHLCable(void);
#endif

UINT8 u1MhlGetCableStatus(UINT8 ports);
BOOL vIsSonyMHLDevice(VOID);
void u1ClearMHLDeviceAdapterID(VOID);
UINT8 u1MhlGetPortId(UINT8 ports);

void vMhlSetSourceStatus(UINT8 port, UINT8 status);
void vMhlSetRapOnOff(UINT8 port, UINT8 OnOff);
UINT8 Mhl_GetSystemPortStatus(UINT8 port);//ON will return 0x00 ; OFF will return0x02 ; BUSY will return0x03
void vMhlSetRapContentOn(UINT8 OnOff);
UINT8 vGetMhlRapContentON(void);

extern void vCbus_NotifyMscResult(UINT8 u1Port,UINT8 u1Opcode,UINT8 data,void *arg);
extern void vCbus_NotifyScratchPadResult(CBUS_MSG_DESCRIPTION_T *msg);
extern void vCbus_NotifyHotPlugResult(UINT8 u1Port,UINT8 connect, void *arg);
extern void vCbus_NotifyCbusModeResult(UINT8 u1Port,UINT8 mode, void *arg);
extern void vCbus_NotifyOverCurrentResult(UINT8 u1Port,UINT8 ocStatus,void *arg);
extern void Mhl_NotifyHotPlug(UINT8 u1Port,UINT8 u1Oc);
extern void Mhl_NotifyCbusMode(UINT8 u1Port,UINT8 u1CbusMode);

//mhl 2.0
BOOL Mhl_Set_MHL2_State( BOOL MHL2_Support,BOOL Support_Charger_900mA_Or_1500mA);
BOOL Mhl_Set_3D_State(BOOL support);//when system initially, SoC calls fgSil_Set_3D_State for desiction DUT supports 3D or not.
#ifdef CC_MHL_3D_SUPPORT
extern BOOL Mhl_3D_EDID_Configuration_VIC_DTD(MHL_3D_VIC_Structuration_T *p3D_VIC_Struct, MHL_3D_DTD_Structuration_T *p3D_DTD_Struct);
void vMHLParsingEDIDForMHL3D(UINT8 *pu1Data);
#endif
void Mhl_NTF_CBUS_Connected(void);
void Mhl_NTF_CBUS_Disconnected(void);
void Mhl_NTF_3D_Communicated(void);
BOOL Mhl_3DConfig_For_3D_Not_Supported(BOOL support);
//static void _MHL_CheckDeviceStatus(void);
UINT8 bMHLDeviceState(UINT8 port);
BOOL Mhl_SendWriteBurstRequest(void);

#endif

#endif     
