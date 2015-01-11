/******************************************************************************
 *   DTV LABORATORY, LG ELECTRONICS INC., SEOUL, KOREA
 *   Copyright(c) 1999 by LG Electronics Inc.
 *
 *   All rights reserved. No part of this work may be reproduced, stored in a
 *   retrieval system, or transmitted by any means without prior written
 *   Permission of LG Electronics Inc.
 *****************************************************************************/

/** @file 	cmnio_type.h
 *
 *  Definitions for global configurations
 *  DESCRIPTION : In this file, all global configurations, enumerations, data
			   or structures should be shared between modules positioned in
			   different layers.
			   For example, enumeration for external video supported in the system,
			   should be referenced in User Interface module (APP layer) and,
			   at the same time, referenced Video module(Driver Layer).
			   So, these kind of definition or declaration should be stated
			   here.
 *  @author	Baekwon Choi (최배권, bk1472@lge.com)
 *  @version 	1.1
 *  @date	2008.06.20
 *  @see
 */

#ifndef _CMNIO_TYPE_H_

#define _CMNIO_TYPE_H_

/*-----------------------------------------------------------------------------
	제어 상수
	(Control Constants)
------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	#include 파일들
	(File Inclusions)
------------------------------------------------------------------------------*/
//#include "branches.h"

#include <common.h>
#include <command.h>
#include <x_typedef.h>
#include <linux/types.h>
#include <linux/string.h>
#include <linux/ctype.h>
#include <exports.h>


#ifdef	__cplusplus
extern "C"
{
#endif /* __cplusplus */

/*-----------------------------------------------------------------------------
	매크로 함수  정의
	(Macros Definitions)
------------------------------------------------------------------------------*/
#ifdef _CMN_VAR_CONF_C_
#define CMN_VAR(vType, vName, vValue)	vType vName = (vType) vValue
#define CMN_EXT
#else
#define CMN_VAR(vType, vName, vValue)	extern vType vName
#define CMN_EXT                         extern
#endif

/*-----------------------------------------------------------------------------
	상수 정의
	(Constant Definitions)
------------------------------------------------------------------------------*/


#define OK	1
#define NOT_OK 0

/*********************************************************************
	매크로 정의(Macro Definitions)
**********************************************************************/

/******************************************************************************
	상수 정의(Constant Definitions)
******************************************************************************/
#define EEPROM_SIZE						0x8000 // 32K


/* NVM Header */
#define NVM_HEADER_SIZE					16

/* NVM Magic */
#define TNVM_MAGIC_SIZE					4

/* NVM DB */
#define SYS_DB_SIZE						640		/** System DB, should be first DB			**/	// 400->640
#define FACTORY_DB_SIZE					592		/** Factory DB 							**/	// 256->288->592
#define TOOL_OPTION_DB_SIZE				128		/** Tool Option DB							**/	// 128
#define ANA_DB_SIZE						832		/** Analog Driver + Calibration DB			**/	// 832
#define SUMODE_10POINTWB_DB_SIZE		128		/** Sevice Mode 의 10 Point White Balance DB	**/	// 128
#define MODEL_INFO_DB_SIZE				96		/** Model Info DB 							**/	// 48->96
#define PRESERVE_DB_SIZE				32		/** Preserve DB							**/	// 16->32
#define EMP_DB_SIZE 					128		/** DivX DRM Information					**/	// 128
#define SWU_DB_SIZE						4096	/** SWU									**/	// 4096
#define MICOM_DB_SIZE					128		/** Internal MICOM DB						**/	// 128
#define THX_DB_SIZE						68		/** THX 의 10 Point White Balance DB			**/	// 68
#define NEW_FACTORY_DB_SIZE				64		/** New Factory DB						**/	// 64
#define	MAC_ADDRESS_SIZE				32		/** MAC Address를 eeprom에 backup			**/	// 32
#define SOC_VENDOR_SIZE					64		/** Vendor data							**/	// 64
#define LAST_INPUT_INFO_DB_SIZE			140		/** (sizeof(LAST_INPUT_INFO_DB_T))			**/	// 60->140
#define CONTINENT_OPTION_DB_SIZE		60		/** Continent option						**/ // 60
#define	TMP_UI_DB_SIZE					32		/** MP 이후 추가되는 registry를 위함.			**/	// 32


#define	BASIC_DB_SIZE					( NVM_HEADER_SIZE				\
										+ TNVM_MAGIC_SIZE				\
										+ SYS_DB_SIZE 					\
										+ FACTORY_DB_SIZE				\
										+ TOOL_OPTION_DB_SIZE			\
										+ ANA_DB_SIZE 					\
										+ SUMODE_10POINTWB_DB_SIZE		\
										+ MODEL_INFO_DB_SIZE			\
										+ PRESERVE_DB_SIZE				\
										+ EMP_DB_SIZE					\
										+ SWU_DB_SIZE					\
										+ MICOM_DB_SIZE					\
										+ THX_DB_SIZE					\
										+ NEW_FACTORY_DB_SIZE			\
										+ MAC_ADDRESS_SIZE				\
										+ LAST_INPUT_INFO_DB_SIZE		\
										+ CONTINENT_OPTION_DB_SIZE		\
										+ TMP_UI_DB_SIZE)


/*
 * ANA_DB와 FACTORY_DB는 NVM MAP이 변경되더라도, 값을 유지해야 합니다.
 * NVM MAP 변경시 ANA_DB와 FACTORY_DB가 Reset되지 않도록 신중히 고려해 주십시오.
 */
/* BASE OFFSET */
#define NVM_HEADER_BASE   			( 0                                     )
#define TNVM_MAGIC_BASE				( NVM_HEADER_BASE   + NVM_HEADER_SIZE 	)
#define SYS_DB_BASE       			( TNVM_MAGIC_BASE 	+ TNVM_MAGIC_SIZE 	)
#define FACTORY_DB_BASE				( SYS_DB_BASE		+ SYS_DB_SIZE		)
#define TOOL_OPTION_DB_BASE			( FACTORY_DB_BASE   + FACTORY_DB_SIZE   )
#define ANA_DB_BASE					( TOOL_OPTION_DB_BASE + TOOL_OPTION_DB_SIZE )
#define	SUMODE_10POINTWB_DB_BASE	( ANA_DB_BASE		+ ANA_DB_SIZE		)
#define	MODEL_INFO_DB_BASE			( SUMODE_10POINTWB_DB_BASE	+ SUMODE_10POINTWB_DB_SIZE	)
#define	PRESERVE_DB_BASE			( MODEL_INFO_DB_BASE	+ MODEL_INFO_DB_SIZE	)
#define	EMP_DB_BASE     			( PRESERVE_DB_BASE  + PRESERVE_DB_SIZE 	)
#define	SWU_DB_BASE	 				( EMP_DB_BASE		+ EMP_DB_SIZE 		)
#define	MICOM_DB_BASE     			( SWU_DB_BASE		+ SWU_DB_SIZE		)
#define	THX_DB_BASE					( MICOM_DB_BASE		+ MICOM_DB_SIZE		)
#define NEW_FACTORY_DB_BASE   		( THX_DB_BASE		+ THX_DB_SIZE 		)
#define	MAC_ADDRESS_BASE			( NEW_FACTORY_DB_BASE  + NEW_FACTORY_DB_SIZE )
#define LAST_INPUT_INFO_DB_BASE		( MAC_ADDRESS_BASE	  + MAC_ADDRESS_SIZE)
#define CONTINENT_OPTION_DB_BASE	( LAST_INPUT_INFO_DB_BASE + LAST_INPUT_INFO_DB_SIZE)
#define	TMP_UI_DB_BASE				( CONTINENT_OPTION_DB_BASE + CONTINENT_OPTION_DB_SIZE)



#define	SOC_VENDOR_BASE				( EEPROM_SIZE			- SOC_VENDOR_SIZE - 10)

#if ( BASIC_DB_SIZE > SOC_VENDOR_BASE)
#error "We need more EEPROM NVM size !!!!!"
#endif

/**
 *  This type defines system print mask.
    (from common/include/osa_debug.h)
 */
#define NUM_OF_PRNT_EACH			32
#define NUM_OF_PRNT_MODULE			10
#define BAR_CODE_SIZE_MAX           17 // same in dil_nvm.h

/*-----------------------------------------------------------------------------
	형 정의
	(Type Definitions)
------------------------------------------------------------------------------*/
/**
 *  This type defines system print mask.
    (from common/include/osa_debug.h)
 */
typedef struct {
    UINT8   color[NUM_OF_PRNT_EACH];
    UINT32  bitmask;
} OSA_PRINT_ALLOC_T;

typedef struct
{
    UINT32              msk;
    OSA_PRINT_ALLOC_T   mod[NUM_OF_PRNT_MODULE];
} OSA_PRINT_MASK_T;

/**
 * PWM configuration
 * (from os/common_linux/dil/include/dil_lcdport.h)
 */
#define PWM_OUT_EN	0x01	/* pwm out enable */
#define PWM_DB_EN	0x02	/* pwm double buffer enable */
#define PWM_VRST_EN	0x04	/* pwm v-sync reset enable */
#define PWM_VDB_EN	0x08	/* pwm v-sync double buffer enable */
#define PWM_VAR_EN	0x10	/* pwm variable freq mode enable */
#define PWM_CLEAR_PLUST 0x20 /* PWM Clear Plus Support by LED Current */
#define PWM_CLEAR_PLUST_ON_OFF 0x40  /* PWM Clear Plus On/off by UI Menu */
#define PWM_PHASE_SHIFT 0x80 /* PWM Phase shift for Backlgith scanning */


/**
 * panel power seq.
    (from drivers/include/paneldb_ddi.h)
 *
 * @see
*/
typedef struct
{
	UINT8 panelPowOnToData; /** Power Sequence for T2, refer to CAS*/
	UINT8 dataToLampOn; /** Power Sequence for T3, refer to CAS*/	/* 초기값은 10ms 단위로 설정하고, 실제 delay 함수 사용시 X10 할것 */
	UINT8 lampOffToData;	/** Power Sequence for T4, refer to CAS*/	/* 초기값은 10ms 단위로 설정하고, 실제 delay 함수 사용시 X10 할것 */
	UINT8 dataToPanelPowOff;	/** Power Sequence for T5, refer to CAS*/

	UINT8 nRLOntoDimSig;	/** Power Sequence for Power Input for BLU to Dimmng Control Signal, refer to CAS*/ /* 초기값은 10ms 단위로 설정하고, 실제 delay 함수 사용시 X10 할것 */
	UINT8 nDimSigtoLamp;	/** Power Sequence for Dimmng Control Signal to Lamp On, refer to CAS*/ /* 초기값은 10ms 단위로 설정하고, 실제 delay 함수 사용시 X10 할것 */
}	PANEL_POWER_SEQ_T;


/**
 * panel pwm info.
 * (from os/common_linux/dil/include/dil_vbe_disp.h)
 * @see
*/
typedef struct
{
	UINT8 reserved1;
	UINT8 reserved2;
	UINT8 vbrBBootlogo;
	UINT8 vbrBMinDuty;	/** refer to CAS*/
	UINT8 vbrBMaxDuty;	/** refer to CAS*/
	UINT8 vbrBMaxDutyStore; /** Store Mode duty*/
	UINT8 vbrBDCRDuty;/** refer to CAS*/
	UINT8 vbrBNoSignalDuty;	/** refer to CAS*/
	UINT8 vbrBFreq48hz;
	UINT8 vbrBFreq60hz; /** refer to CAS*/
	UINT8 vbrBFreq50hz; /** refer to CAS*/
	UINT8 vbrCLedCurrent;
	UINT8 vbrCFreq;
	UINT8 config;
}PANEL_PWM_T;



/**
  *  System DB 모듈을 위한 실제 사용 type 정의
  * (from os/common_linux/dil/include/dil_nvm.h)
*/
typedef struct
{
    UINT32          	validMark;
    OSA_PRINT_MASK_T	printMask;
    UINT32          	sys_utt;			/* PDP panel use time */
    UINT8          	    frcDownloadMode;	/* FRC auto download or not */
    PANEL_POWER_SEQ_T	panelpowerseq;		/* For panel power sequence timing */
    PANEL_PWM_T			panelpwm;			/* For panel pwm */
    UINT8               systemtype;         /*system type 0:atsc, 1: dvb*/
    UINT8	            ColorDepth;			/**COLOR_DEPTH_T */
    UINT8	            LVDSPixel;			/**LVDS_PIXEL_T */
	UINT8				vcomPgammaChecksum;	/*Vcom Pgamma value checksum */
	UINT8				nDebugStatus;
	UINT8				swuMode;			/* swu mode */
	UINT8				make_hib;			/* for snapshot boot (make building) */
	UINT8				snapshot_onoff;		/* for snapshot support or not*/
	UINT32				adcMode;			/* for adc Mode 현재 ADC 설정 값을 저장  */
	UINT32				swum_magic;			/* swu mode magic */
	UINT8				soundout_mode;		/* soundout mode save 0:tv speaker, 1:external speaker 2:optical sound bar, 3:headphone,4:lineout,5;bluetooth soundbar */
	UINT32				zram;				/* zram */
	UINT8				eMMCUsage;			/* for eMMC Usage */
	UINT8				bDPortEnable;
	UINT8				fullVerify;			/* Secureboot full verify flag */
	UINT8				pwm_mode;			/* PWM mode */
	int					timeDifference;
	UINT8				instant_onoff;		/* for instant boot */
	UINT8				snapshot_art		/* for snapshot boot art*/

} SYS_DB_T;



/******************************************************
 			TOOL_OPTION1 에 필요한 구조체 정의
*****************************************************/
// 아래 구조체 변경 시 반드시 각 항목의 MAX 값도 같이 변경할 것
#define MAX_MODULE_VER		4


/* This type should be same with ePanel_SPI_Type in drv_display.h */
typedef enum
{
	LED_BAR_DIRECTM_8	 	= 0,
	LED_BAR_DIRECTM_6	 	= 1,
	LED_BAR_DIRECTM_10	= 2,
	LED_BAR_DIRECTM_12	= 3,
	LED_BAR_DIRECTM_15	= 4,
	LED_BAR_DIRECTM_16	= 5,
	LED_BAR_DIRECTM_288	= 6,

	LED_BAR_EDGE_6	 	= 7,
	LED_BAR_EDGE_12		= 8,
	LED_BAR_EDGE_16		= 9,
	LED_BAR_EDGE_20		= 10,
	LED_BAR_EDGE_32		= 11,
	LED_BAR_EDGE_80		= 12,

	LED_BAR_MAX,
} LED_BAR_TYPE_T;


typedef enum
{
	LD_10_16_BLK	 = 0, //V_16BLK, V10BLK, RES_I, RES_II  (for 2011 modules)
	LD_6_12_BLK		 = 1, //V_12BLK, V_6BLK, H_12BLK, H_6BLK (for 2012 modules)
	LD_BLK_MAX	 	 = 2,
} LOCAL_DIM_BLOCK_TYPE_T;



/**
 * Named size of panel, comes from src/common/include/osa_modeldef.h
 */
typedef enum
{
	INCH_32 = 0,
	INCH_39,
	INCH_42,
	INCH_47,
	INCH_49,
	INCH_50,
	INCH_55,
	INCH_58,
	INCH_60,
	INCH_65,
	INCH_70,
	INCH_77,
	INCH_79,
	INCH_84,
	INCH_98,
	INCH_105,

	// Smart Monitor TV
	INCH_23,
	INCH_24,
	INCH_26,
	INCH_27,

	// 추가
	INCH_22,
	INCH_28,

	INCH_BASE
} INCH_TYPE_T;


/**
 * not used inch type
 */
typedef enum
{
	INCH_15= INCH_BASE,
	INCH_19,
	INCH_29,
	INCH_37,
   	INCH_46,
	INCH_52,
	INCH_72,
	INCH_75,
	INCH_95,
	INCH_100, // TV model

} OLD_INCH_TYPE_T;


typedef enum
{
	TOOL_EB97=0, 	// 1
	TOOL_EB95,		// 2
	TOOL_LB87,		// 3
	TOOL_LB86,		// 4
	TOOL_LB72,		// 5
	TOOL_LB65,		// 6
	TOOL_LB69,		// 7
	TOOL_LB67,		// 8
	TOOL_LB63,		// 9
	TOOL_LB58,		// 10
	TOOL_LB57,		// 11
	TOOL_LB56,		// 12
	TOOL_UB9,		// 13
	TOOL_UB97,		// 14
	TOOL_UB95,		// 15
	TOOL_UB93,		// 16
	TOOL_UB86,		// 17
	TOOL_UB83,		// 18
	TOOL_GB69,		// 19

	// 소형.
	TOOL_MT95,		// 20
	TOOL_MT75,		// 21

	// 추가.
	TOOL_LB49,		// 22
	TOOL_LB575,		// 23
	TOOL_LB58B,		// 24
	TOOL_LB658,		// 25
	TOOL_LB68,		// 26
	TOOL_LB70,		// 27
	TOOL_LB71,		// 28
	TOOL_LB73,		// 29
	TOOL_LB75,		// 30
	TOOL_UB81,		// 31
	TOOL_UB85,		// 32
	TOOL_UB88,		// 33
	TOOL_LA965,		// 34
	TOOL_LA97,		// 35
	TOOL_EC93,		// 36
	TOOL_EC97,		// 37
	TOOL_EC98,		// 38
	TOOL_UB98,		// 39
	TOOL_UB82,		// 40
	TOOL_LB757,		// 41
	TOOL_LB635,		// 42
	TOOL_UC97,		// 43
	TOOL_UC9,		// 44

	TOOL_LCD_END,
	TOOL_PDP_END = TOOL_LCD_END + 1,
	TOOL_BASE	= TOOL_PDP_END,

} TOOL_TYPE_T;



//not used tool type (old type)
typedef enum
{
	TOOL_EA88=TOOL_BASE, 	// 1
	TOOL_EA98,		// 2
	TOOL_G3A, 		// 3
	TOOL_G3S, 		// 4
	TOOL_LA58, 		// 5
	TOOL_LA62, 		// 6
	TOOL_LA64, 		// 7
	TOOL_LA65, 		// 8
	TOOL_LA66,		// 9
	TOOL_LA68,		// 10
	TOOL_LA69,		// 11
	TOOL_LA74,		// 12
	TOOL_LA79,		// 13
	TOOL_LA86,		// 14
	TOOL_LA96,		// 15
	TOOL_LN51,		// 16
	TOOL_LN54,		// 17
	TOOL_LN56,		// 18
	TOOL_LN57,		// 19
	TOOL_MT93,		// 20
	TOOL_MS73,		// 21
	TOOL_MS53,		// 22
	TOOL_LN46,		// 23
	TOOL_LA625,		// 24
	TOOL_LA6205,	// 25
	TOOL_LN615,		// 26
	TOOL_LN575,		// 27
	//TOOL_LA97,		// 28
	TOOL_LA623,		// 29
	TOOL_LA71,		// 30
	TOOL_LN61,		// 31
	TOOL_LA88,		// 32
	TOOL_LA63,		// 33
	TOOL_GA68,		// 34
	TOOL_LA697, 	// 35
	TOOL_LN571,		// 36

	TOOL_LM63,
	TOOL_LM96,
	TOOL_LM95,
	TOOL_LM86,

	TOOL_LCD_END_OLD,

	//PDP Tool
	TOOL_PN670= TOOL_LCD_END_OLD,	// 1
	TOOL_PN470,					// 2
	TOOL_PM470,
	TOOL_PH670,					// 3
	TOOL_PH470,					// 4
	TOOL_PN570,					// 5
	TOOL_PH660,					// 6
	TOOL_PDP_END_OLD,
	TOOL_BASE_OLD	= TOOL_PDP_END_OLD,	// LCD Tool이 추가되어 PDP Tool의 수보다는 많을것으로 예상됨.
} OLD_TOOL_TYPE_T;


typedef enum
{
	MODULE_LGD = 0,
	MODULE_CMI,
	MODULE_AUO,
	MODULE_SHARP,
	MODULE_IPS,
	MODULE_BOE,
	MODULE_CSOT,
	MODULE_INX,
	MODULE_LCD_END,

	MODULE_LGE = MODULE_LCD_END,
	MODULE_PANASONIC,
	MODULE_PDP_END,
	MODULE_BASE	= MODULE_PDP_END
} MODULE_MAKER_TYPE_T;


//balup_090626
typedef enum
{
	MODULE_VER0		=0,		//not support error out, not support scanning backlight
	MODULE_VER1		=1,     //support error out, not support scanning backlight
	MODULE_VER2		=2,     //support scanning backlight, not support error out
	MODULE_VER3	 	=3		//support error out, support scanning backlight
} MODULE_VERSION_TYPE_T;


/******************************************************
 			TOOL_OPTION3 에 필요한 구조체 정의
******************************************************/
#define AMP_CHIP_TYPE_MAX			5
#define BACKLIGHT_TYPE_MAX			7

/* AMP type */
typedef enum
{
	AMP_NTP6AMP = 0,	/* NTP7514 6AMP	*/
	AMP_TAS5733,		/* TAS5733	*/
	AMP_STA380,			/* ST380	*/
	AMP_NTP7514,
	AMP_NTP2AMP,		/* NTP7514 2AMP */
	AMP_NTP3AMP,		/* NTP7514 3AMP */
	AMP_NTP4AMP,		/* NTP7514 4AMP */
	AMP_NTP5AMP,		/* NTP7514 5AMP */

	// not use
	AMP_NTP7513,	/* NTP7513	*/
	AMP_STA2AMP,		/* STA2AMP	*/
	AMP_TAS2AMP,		/* TAS5733 2AMP */
	AMP_TAS3AMP,		/* TAS5733 3AMP */
	AMP_NTP7400,	/* NTP7400  */
	AMP_NTP7500,		/* NTP7500	*/
}AMP_CHIP_TYPE_T;



/* Backlight type*/
typedef enum
{
	BL_DIRECT_M=0,
	BL_DIRECT_L,
	BL_EDGE_LED,
	BL_ALEF_LED,
	BL_OLED,
	BL_END,
}BACKLIGHT_TYPE_T;


/* Backlight type*/
typedef enum
{
	BL_ROW = BL_END,
	BL_NOR_LED,
	BL_IOL_LED,
	BL_POLA,
	BL_CCFL,

	BL_IOP_LED,
	BL_ALEF_LGD,
	BL_ALEF_FPR,
}OLD_BACKLIGHT_TYPE_T;


/* EDID type*/
typedef enum
{
	EDID_PCM = 0,
	EDID_AC3,
#ifdef INCLUDE_SUPPORT_HDMI_DTS
	EDID_DTS,
#else
	EDID_RESERVED,
#endif
	SUPPORT_EDID_TYPE_MAX,
}EDID_DATA_TYPE_T;


typedef enum
{
	PWM_FREQ_50_60_HZ				= 0,
	PWM_FREQ_100_120_HZ				= 1,
	PWM_FREQ_LVDS_LINK_MAX			= 2,
} PWM_FREQ_T;


/**
* sw.byeon - 100910 : 툴옵션 Union 값 순서 재정렬함.
* 각 Tool Option에 항목 추가시, 맨 아래에 추가할것.
* 위에 추가할 경우 Bit가 밀리기 때문에 ToolOption Valid mark를 변경하여 초기화하지 않으면 값이 틀어지게됨.
* 또한 순서대로 추가해야 OSD 및 툴옵션 enum의 순서와 동일하게 됨.
**/

/* Ext input Adjust */
typedef struct
{
	UINT8					nHDMI1;
	UINT8					nHDMI2;
	UINT8					nHDMI3;
	UINT8					nHDMI4;
	UINT8					nCOMP1;
	UINT8					nCOMP2;
	UINT8					nCOMP3;
	UINT8					nRGB1;
	UINT8 					nVideoScartFullportA;
	UINT8					nRCA_AV1;
	UINT8					nRCA_AV2;
	UINT8					nRCA_AV3;
	UINT8					nReserved1;
	UINT8					nReserved2;
	UINT8					nReserved3;
} EXT_VIDEO_INPUT_ADJ_T;

typedef struct
{
	UINT8					nCOMP1;
	UINT8					nCOMP2;
	UINT8					nCOMP3;
	UINT8					nRGB1;
	UINT8 					nAudioScartFullportA;
	UINT8					nRCA_AV1;
	UINT8					nRCA_AV2;
	UINT8					nRCA_AV3;
	UINT8 					nReserved1;
	UINT8					nReserved2;
	UINT8					nReserved3;
} EXT_AUDIO_INPUT_ADJ_T;


/**
 *  Tool Option1 of Ez Adjust Menu(Service Menu).
 */
typedef union
{
	UINT16 all;
	struct
	{
		UINT16					eModelInchType			: 5;
		UINT16					eModelToolType			: 6;
		UINT16					eModelModuleType		: 4;
		UINT16					nLVDSBit				: 1;
	} flags;

} TOOL_OPTION1_T;



/**
 *  Tool Option2 of Ez Adjust Menu(Service Menu).
 */
typedef union
{
	UINT16 all;
	struct
	{
		UINT16 					nExtVideoInputIndex		: 4;
		UINT16					nExtAudioInputIndex		: 4;
		UINT16					eBackLight				: 3;
		UINT16					nNumOfUSBInput			: 2;
		UINT16					eHDMISwitchIC			: 2;
		UINT16					bEmmcRecord				: 1;
	} flags;
} TOOL_OPTION2_T;


/**
 *  Tool Option3 of Ez Adjust Menu(Service Menu).
 */
typedef union
{
	UINT16 all;
	struct
	{
		UINT16					eAudioAmp				: 3;
		UINT16					bHeadphone				: 1;
		UINT16					bLogoLight				: 1;
		UINT16					bDVRReady				: 1;
		UINT16					bTHX					: 1;
		UINT16					bISF					: 1;
		UINT16					bSupportEPA 			: 1;
		UINT16					bDualView				: 1;
		UINT16					bPip					: 1;
		UINT16					bCommercial 			: 1;
		UINT16					bToolDB3_reserved1		: 1;
		UINT16					bLocalDimming			: 1;
		UINT16					bLocalDimMenu			: 1;
		UINT16					bMotionOPC				: 1;
	} flags;
} TOOL_OPTION3_T;


/**
 *  Tool Option4 of Ez Adjust Menu(Service Menu).
 */
	typedef union
	{
		UINT16 all;
		struct
		{
			UINT16					eDigitalDemod_S 	: 2;
			UINT16					eDigitalDemod		: 4;
			UINT16					eAnalogDemod		: 3;
			UINT16					eDigitalDemod_Sub	: 4;
			UINT16					eAnalogDemod_Sub	: 3;
		} flags;

	} TOOL_OPTION4_T;


/**
 *  Tool Option5 of Ez Adjust Menu(Service Menu).
 */
typedef union
{
	UINT16 all;
	struct
	{
		UINT16					eSupportWiFi			: 2;
		UINT16					eSupportCameraReady		: 2;
		UINT16					eSupportMouseRC			: 3;
		UINT16					bTVLink 				: 1;
		UINT16					bMirrorMode 			: 1;
		UINT16					bSupportCompRCACommon	: 1;
		UINT16					bSupportCNTV			: 1;
		UINT16					e3DModelType			: 1;
		UINT16					bSupportHDMI2ExtEdid	: 1;
		UINT16					bScanningBL				: 1;
		UINT16					bToolDB5_reserved2		: 1;
		UINT16					bClearPlus				: 1;
	} flags;

} TOOL_OPTION5_T;


/**
 *  Tool Option6 of Ez Adjust Menu(Service Menu).
 */
typedef union
{
	UINT16 all;
	struct
	{
		UINT16					nDefaultStdBacklight		: 4;
		UINT16					nMotionEyeCare				: 4;
		UINT16 					nAudioPowerVoltage			: 3;
		UINT16 					nSpeakerWattage				: 2;
		UINT16 					eMaxPwmType					: 3;
	} flags;
} TOOL_OPTION6_T;


/**
 *  Tool Option7 of Ez Adjust Menu(Service Menu).
 */
typedef union
{
	UINT16 all;
	struct
	{
		UINT16					bMAC_AP					: 1;
		UINT16					bESN_AP					: 1;
		UINT16					bCI_AP					: 1;
		UINT16					bWIDEVINE_AP			: 1;
		UINT16 					bMARLIN_AP				: 1;
		UINT16					bSupportDTCPKey			: 1;
		UINT16 					eEDID_Type				: 2;
		UINT16					bToolDB7_reserved0		: 1;
		UINT16					bSupportDigitalEye		: 1;
		UINT16					bPowerBoardType			: 1;
		UINT16					eLEDBarType				: 3;
//		UINT16					bLoalDim_BLK_Type		: 1;
		UINT16					bToolDB7_reserved1		: 1;
		UINT16					bPWMFreq				: 1;
	} flags;
} TOOL_OPTION7_T;



/**
 *  Tool Option8 of Ez Adjust Menu(Service Menu).
 */
typedef union
{
	UINT16 all;
	struct
	{
		UINT16					bToolDB8_reserved0		: 4;
		UINT16					bToolDB8_reserved1		: 4;
		UINT16					bToolDB8_reserved2		: 4;
		UINT16					bToolDB8_reserved3		: 4;
	} flags;
} TOOL_OPTION8_T;



// stonedef - 090717
// Area option 구조체 정의
typedef union
{
	UINT16 all;
	struct
	{
		UINT16					eWiFiFreq		: 5;		/* Wi-Fi Frequency */
		UINT16					eLocation		: 3;		/* 아주 국가 중 특별한 setting이 필요할 경우 option으로 사용, 1 : India(for audio setting), 2 ~ 7 : reserved */
		UINT16					bNordicUI		: 1;		/* 00:Non-Nordic(UK/Ireland+Others), 01:Nordic */
		UINT16					bHwOption		: 1;
		UINT16					bIsolator		: 1;		/* Isolator 지원 유무 */
		UINT16					bAJJAArea		: 1;		/* 아주중아 option */
		UINT16					eTTXLangGroup	: 3;		/* Teletext Language */
		UINT16					bC2				: 1;		/* C2 지원 모델 Check */
	}flags;
}AREA_OPTION_T;



typedef union
{
	UINT16 all;
	struct
	{
		UINT16					nNordicUI			: 1;			/* 00:Non-Nordic(UK/Ireland+Others), 01:Nordic */
		UINT16 					bHwOption			: 1;
		UINT16					bT2					: 1;			/* T2 지원 모델 Check_Hardware_Option */
		UINT16					bC2					: 1;			/* C2 지원 모델 Check_Hardware_Option */
		UINT16					bSatellite			: 1;			/* 위성지원 모델 Check_Hardware_Option */
		UINT16					bMHP				: 1;			/* MHP 지원 모델 */
		UINT16					nHbbtv				: 2;			/* 00:Hbbtv Disable, 01:미정, 10:Enable, 11:reserve */
		UINT16					bIsolator			: 1;			/* isolator 지원 유무 */
		UINT16					eEastWest_EU		: 2;			/* 동/서 유럽 구분 */
		UINT16					bArabicLang			: 1;			/* ArabicLang support */
		UINT16					bAJJAArea			: 1;			/* 아주중아 option */
		UINT16					nTTXLang			: 3;			/* 아주중아 Teletext Language */
	}flags;
}AREA_OPTION_EU_T;





/**
 *	TOOL OPTION struct type
 */
typedef struct
{
	UINT32              	validMark;
	TOOL_OPTION1_T			nToolOption1;
	TOOL_OPTION2_T			nToolOption2;
	TOOL_OPTION3_T			nToolOption3;
	TOOL_OPTION4_T			nToolOption4;
	TOOL_OPTION5_T			nToolOption5;
	TOOL_OPTION6_T			nToolOption6;
	TOOL_OPTION7_T			nToolOption7;
	TOOL_OPTION8_T			nToolOption8;
	AREA_OPTION_T			stAreaOptions;		/* Area Option for 아주/중아/중남미 */
	AREA_OPTION_EU_T		stEuAreaOptions;	/* Area Option for 구주 */
	EXT_VIDEO_INPUT_ADJ_T	stExtVideoInputADJ;
	EXT_AUDIO_INPUT_ADJ_T	stExtAudioInputADJ;

}	TOOLOPTION_DB_T;



/**
*	MODEL INFO DB ( 모델 정보 관련 DB )
*/
typedef struct MODEL_INFO_DB
{
	UINT32		validMark;
	UINT8		aModelName[13]; //090212 odrie20 Model Name을 eeprom에 저장하고 초기 data만 inch, tool의 조합으로 만들도록 洋
	UINT8		aSerialNum[BAR_CODE_SIZE_MAX];
	UINT8		group_code;      /* country group code which is defined in "common/include/country.h" */
	UINT32		country_code;    /* country code which is defined in "common/include/country_codes.h" */
	UINT32		city_code;    /* city code which is defined in "common/include/country_codes.h" *//*zhangze 0718*/
}MODEL_INFO_DB_T;



/**
 *	BAUDRATE struct.
 */
typedef enum
{
	BAUDRATE_2400		= 0,
	BAUDRATE_4800		= 1,
	BAUDRATE_9600		= 2,
	BAUDRATE_14400		= 3,
	BAUDRATE_19200		= 4,
	BAUDRATE_38400		= 5,
	BAUDRATE_57600		= 6,
	BAUDRATE_115200 	= 7,
	BAUDRATE_460800 	= 8

}	SYS_BAUDRATE_T;



/**
* PRESERVE DB ( 양산 이후에 Reset 되거나 임의로 변경되지 말아야 하는 DB )
*/
typedef struct PRESERVE_DB
{
	UINT32			validMark;
	UINT8 			bInstopReserved;
	UINT8 			bInstopCompleted;
	SYS_BAUDRATE_T	eBaudrate;
	UINT8			b1stBootAfterInstop;
	UINT8			b1stBootAfterFactoryReset;
}	PRESERVE_DB_T;


/**
*	MAC ADDRESS back-up data
*	by ieeum, zaewon.lee.
*/
typedef struct MAC_ADDRESS
{
	UINT32		validMark;
	UINT8		macAddr[17];
}MAC_ADDRESS_T;

extern MAC_ADDRESS_T gMacAddress;


/**	1st Boot After Instop State enum */
enum
{
	BOOT_STATE_NOT_1ST_BOOT = 0,	// Not First boot after instop!!!
	//BOOT_STATE_AFTER_SYSTEM_INIT,		// not use
	//BOOT_STATE_20S_AFTER_UI_INIT,		// not use
	BOOT_STATE_AFTER_INSTOP,		// First boo after instop!!!
};


#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /*_CMNIO_TYPE_H_*/




