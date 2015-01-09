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

/*--------------------------------------------------------------------------
 * NVRAM 저장 장치
 *   - NVM_EEPROM_ONLY   : Support only I2C-EEPROM
 *   - NVM_EEPROM_N_NAND : Support both I2C-EEPROM and NAND flash
 *
 * 2010.07.27. by gunho.lee
 *-------------------------------------------------------------------------*/
#define	NVM_EEPROM_ONLY					0
#define	NVM_EEPROM_N_NAND				1
#define TNVM_MAGIC_CODE					0x20100118

#define EEPROM_SIZE						0x8000 // 32K

/*------------------------------------------
 * NVM DB Map
 *------------------------------------------*/

/* NVM Header */
#define NVM_HEADER_SIZE					16

/* NVM Magic */
#define TNVM_MAGIC_SIZE					4

/* NVM DB */
#define SYS_DB_SIZE						400		/** 0180(384)  : System DB, should be first DB	**/
#define ANA_DB_SIZE						832		/** 0340(832) : Analog Driver + Calibration DB	**/
#define TOOL_OPTION_DB_SIZE				128		/** 0080(128)  : Tool Option DB, reserved space = 108	**/
#define FACTORY_DB_SIZE					288		/** 0100(256)  : Factory DB, reserved space = 8	256-288-320**/
#define SUMODE_10POINTWB_DB_SIZE		128		/** 0080(128) : Sevice Mode 의 10 Point White Balance DB **/
#define MODEL_INFO_DB_SIZE				48		/** 0030(48) : Model Info DB **/
#define PRESERVE_DB_SIZE				16		/** 0010(16) : Preserve DB **/
#define EMP_DB_SIZE 					128		/** 0080(128)  : DivX DRM Information  			**/
#define SWU_DB_SIZE						4096
#define MHP_DB_SIZE						4171
#define MICOM_DB_SIZE					128		/** 0080(128)  : Internal MICOM DB				**/
#define THX_DB_SIZE						68		/** 0080(128) : THX 의 10 Point White Balance DB **/
#define NEW_FACTORY_DB_SIZE				64		/** 0040(64)  : New Factory DB		**/
#define	NETLOG_DB_SIZE					804		/** 0194(404) : Net Server Log DB. size 잘못됨. 804로 변경. jaehan.park 101213 **/
#define	TMP_CPLIST_DB_SIZE				700		/** 700  TEMP_CPLIST_DB. GP4에서 UI DB로 이동 **/
#define	MAC_ADDRESS_SIZE				32		/** MAC Address를 eeprom에 backup **/
#define SOC_VENDOR_SIZE					64		/** 0040(64)  : Vendor data 		**/
#define LAST_INPUT_INFO_DB_SIZE			60
#define	TMP_UI_DB_SIZE					32		/** MP 이후 추가되는 registry를 위함. **/

#define UI_DB_SIZE						102400	//50000	//38888	/** 9400(37888) : UI DB(edge_enhancer,xvYCC,thx)	**/	//23284 -> 37888 ->38888->50000->102400(100KB,12.08.22)
#define UI_EXPERT_DB_SIZE				81920	//38000	//33792	/** 8400(33792) : UI Expert DB(edge, xvYVV,		**/	//11700 -> 28672 -> 33792->38000->81920(80KB,12.08.22)

//#if 0 //#if(NVRAM_DEV  == NVM_EEPROM_ONLY)
#if 0 //#if (PLATFORM_TYPE == BCM_PLATFORM)
#define CH_DB_SIZE						(40000)	// in case of 128KB EEPROM
#else
#define CH_DB_SIZE						215040	//(108480  + 0x8000)	/** a200(41472): Channel DB(3k for DRRT)	**/	//141248 -> 215040(210KB,12.08.22)
#endif
#define DVBS_DB_SIZE					1048576	//(0xC8000 - 0x8000)	/** 800 K								**/	//786432 -> 1048576(1MB,12.08.22)
#ifdef INCLUDE_SYS_ISDB
#define CA_MAIL_DB_SIZE					800*30
#define UI_MAIL_DB_SIZE					17408
#endif




#define	BASIC_DB_SIZE					( NVM_HEADER_SIZE				\
										+ TNVM_MAGIC_SIZE				\
										+ SYS_DB_SIZE 					\
										+ ANA_DB_SIZE 					\
										+ TOOL_OPTION_DB_SIZE			\
										+ FACTORY_DB_SIZE				\
										+ SUMODE_10POINTWB_DB_SIZE		\
										+ MODEL_INFO_DB_SIZE			\
										+ PRESERVE_DB_SIZE				\
										+ EMP_DB_SIZE					\
										+ SWU_DB_SIZE					\
										+ MHP_DB_SIZE					\
										+ MICOM_DB_SIZE					\
										+ THX_DB_SIZE					\
										+ NEW_FACTORY_DB_SIZE			\
										+ NETLOG_DB_SIZE				\
										+ TMP_CPLIST_DB_SIZE			\
										+ MAC_ADDRESS_SIZE				\
										+ LAST_INPUT_INFO_DB_SIZE		\
										+ TMP_UI_DB_SIZE)


#define EXTEND_DVBS_DB_SIZE    			( UI_DB_SIZE					\
										+ UI_EXPERT_DB_SIZE				\
										+ CH_DB_SIZE					\
										+ DVBS_DB_SIZE )
#ifdef INCLUDE_SYS_ISDB
#define EXTEND_DB_SIZE    				( UI_DB_SIZE					\
										+ UI_EXPERT_DB_SIZE				\
										+ CA_MAIL_DB_SIZE				\
										+ UI_MAIL_DB_SIZE				\
										+ CH_DB_SIZE)

#else
#define EXTEND_DB_SIZE    				( UI_DB_SIZE					\
										+ UI_EXPERT_DB_SIZE				\
										+ CH_DB_SIZE )
#endif

#define TOTAL_DB_SIZE					(BASIC_DB_SIZE + EXTEND_DB_SIZE)

#define TOTAL_DVBS_DB_SIZE				(BASIC_DB_SIZE + EXTEND_DVBS_DB_SIZE)


/*
 * ANA_DB와 FACTORY_DB는 NVM MAP이 변경되더라도, 값을 유지해야 합니다.
 * NVM MAP 변경시 ANA_DB와 FACTORY_DB가 Reset되지 않도록 신중히 고려해 주십시오.
 */
/* BASE OFFSET */
#define NVM_HEADER_BASE   			( 0                                     )
#define TNVM_MAGIC_BASE				( NVM_HEADER_BASE   	+ NVM_HEADER_SIZE 	)
#define SYS_DB_BASE       			( TNVM_MAGIC_BASE 		+ TNVM_MAGIC_SIZE 	)
#define ANA_DB_BASE       			( SYS_DB_BASE 			+ SYS_DB_SIZE 		)
#define TOOL_OPTION_DB_BASE   		( ANA_DB_BASE       	+ ANA_DB_SIZE       )
#define FACTORY_DB_BASE   			( TOOL_OPTION_DB_BASE	+ TOOL_OPTION_DB_SIZE )
#define	SUMODE_10POINTWB_DB_BASE	( FACTORY_DB_BASE		+ FACTORY_DB_SIZE		)
#define	MODEL_INFO_DB_BASE			( SUMODE_10POINTWB_DB_BASE	+ SUMODE_10POINTWB_DB_SIZE	)
#define	PRESERVE_DB_BASE			( MODEL_INFO_DB_BASE	+ MODEL_INFO_DB_SIZE	)
#define	EMP_DB_BASE     			( PRESERVE_DB_BASE  	+ PRESERVE_DB_SIZE 	)
#define	SWU_DB_BASE	 				( EMP_DB_BASE			+ EMP_DB_SIZE 		)
#define	MHP_DB_BASE	 				( SWU_DB_BASE			+ SWU_DB_SIZE 		)
#define	MICOM_DB_BASE     			( MHP_DB_BASE			+ MHP_DB_SIZE		)
#define	THX_DB_BASE					( MICOM_DB_BASE			+ MICOM_DB_SIZE		)
#define NEW_FACTORY_DB_BASE   		( THX_DB_BASE			+ THX_DB_SIZE 		)
#define	NETLOG_DB_BASE				( NEW_FACTORY_DB_BASE 	+ NEW_FACTORY_DB_SIZE	)
#define	TMP_CPLIST_DB_BASE			( NETLOG_DB_BASE	  	+ NETLOG_DB_SIZE )
#define	MAC_ADDRESS_BASE			( TMP_CPLIST_DB_BASE  	+ TMP_CPLIST_DB_SIZE )
#define LAST_INPUT_INFO_DB_BASE		( MAC_ADDRESS_BASE		+ MAC_ADDRESS_SIZE	)
#define	TMP_UI_DB_BASE				( LAST_INPUT_INFO_DB_BASE  	+ LAST_INPUT_INFO_DB_SIZE)


/* NAND FLASH 영역 */
#define UI_DB_BASE        			( TMP_UI_DB_BASE		+ TMP_UI_DB_SIZE)
#define UI_EXPERT_DB_BASE 			( UI_DB_BASE 			+ UI_DB_SIZE 		)
#define	CH_DB_BASE        			( UI_EXPERT_DB_BASE		+ UI_EXPERT_DB_SIZE	)
#ifdef INCLUDE_SYS_ISDB
#define	CA_MAIL_DB_BASE        		( CH_DB_BASE			+ CH_DB_SIZE	)
#define	UI_MAIL_DB_BASE        		( CA_MAIL_DB_BASE		+ CA_MAIL_DB_SIZE	)
#define DVBS_DB_BASE				( UI_MAIL_DB_BASE  		+ UI_MAIL_DB_SIZE	)
#else
#define DVBS_DB_BASE				( CH_DB_BASE     		+ CH_DB_SIZE		)
#endif

/*ALEF EEPROM 영역 */
#define ALEF_DB_BASE				( 0											)
#define ALEF_THX_DB_BASE			( ALEF_DB_BASE			+ ANA_DB_SIZE		)


// request from L9 SIC
#define	SOC_VENDOR_BASE				( EEPROM_SIZE			- SOC_VENDOR_SIZE - 10)

#if ( BASIC_DB_SIZE > SOC_VENDOR_BASE)
#error "We need more EEPROM NVM size !!!!!"
#endif

/**
 *  This type defines system print mask.
    (from common/include/osa_debug.h)
 */
#define NUM_OF_PRNT_EACH			32
#define NUM_OF_PRNT_MODULE			9

/*-----------------------------------------------------------------------------
	형 정의
	(Type Definitions)
------------------------------------------------------------------------------*/
typedef struct
{
	char		*pName;
	UINT32		portNum;
	UINT32		slvAddr;
	UINT32		speed;
} I2C_DESC_T;

typedef struct
{
	I2C_DESC_T **ppI2cDev;
	char		*pName;
} I2C_DEV_LIST_TBL;


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
	UINT8 panelPowOnToData;	/** Power Sequence for T2, refer to CAS*/
	UINT8 dataToLampOn;	/** Power Sequence for T3, refer to CAS*/	/* 초기값은 10ms 단위로 설정하고, 실제 delay 함수 사용시 X10 할것 */
	UINT8 lampOffToData;	/** Power Sequence for T4, refer to CAS*/	/* 초기값은 10ms 단위로 설정하고, 실제 delay 함수 사용시 X10 할것 */
	UINT8 dataToPanelPowOff;	/** Power Sequence for T5, refer to CAS*/

	UINT8 SOEtoDPM;			/** Power Sequence for T6, refer to CAS*/
	UINT8 DPMtoVGH;			/** Power Sequence for T6, refer to CAS*/
	UINT8 VGHtoVST;			/** Power Sequence for T6, refer to CAS*/
	UINT8 VSTtoGCLK;		/** Power Sequence for T7, refer to CAS*/

	UINT8 nRLOntoDimSig;	/** Power Sequence for Power Input for BLU to Dimmng Control Signal, refer to CAS*/ /* 초기값은 10ms 단위로 설정하고, 실제 delay 함수 사용시 X10 할것 */
	UINT8 nDimSigtoLamp;	/** Power Sequence for Dimmng Control Signal to Lamp On, refer to CAS*/ /* 초기값은 10ms 단위로 설정하고, 실제 delay 함수 사용시 X10 할것 */
}	PANEL_POWER_SEQ_T;

#define DIMMING_CTRL_UI_3D_ENABLE		//KWON_1102 add for dimming UI open (for 3D state)  test

/**
 * panel pwm info.
 *
 * @see
*/
typedef struct
{
	UINT8 vbrAMinVdc;	/** refer to CAS*/
	UINT8 vbrATypVdc;	/** refer to CAS*/
	UINT8 vbrAMaxVdc;	/** refer to CAS*/
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
 *	Tuner Option struct.
 */
typedef enum
{
   	TUNER_PHASE_DEFAULT = 0,
	TUNER_ENHANCED_PHASE,
	TUNER_ENHANCED_GHOST,
	TUNER_ENHANCED_PHASE_HIGH
}	SYS_TUNER_OPT_T;

/**
    System DB 모듈을 위한 실제 사용 type 정의
*/
typedef struct
{
    UINT32          	validMark;
    OSA_PRINT_MASK_T	printMask;
    UINT32          	sys_utt;			/* PDP panel use time */
    UINT8           	powerOnMode;		/* internal micom:  POWER ON MODE	*/
    UINT8           	powerState;			/* internal micom:	POWER STATE		*/
    UINT8           	powerOnStatus;		/* internal micom:  HOTEL: POWER ON STATUS */
    UINT8           	setID;				/* internal micom:  SET ID			*/
    UINT8           	keyLocked;			/* internal micom:  HOTEL: KEY OPERATION MODE */
    UINT8           	irLocked; 			/* internal micom:	IR KEY LOCKED	*/
    UINT8           	localLocked;		/* internal micom:  LOCAL KEY LOCKED*/
    /* taburin : 20090205, reserved(addr : 0x137)를 atsc-saturn5에서 cec on/off 여부를 micom driver에서 확인 할 수 있도록 사용함. 수정 시 확인 바람.*/
    UINT8           	reserved;   		/* internal micom:  reserved */
    UINT8           	dbgCount;   		/* internal micom:  debug counter */
    UINT8          	    frcDownloadMode;	/* FRC auto download or not */
    PANEL_POWER_SEQ_T	panelpowerseq;		/* For panel power sequence timing */
    PANEL_PWM_T			panelpwm;			/* For panel pwm */
    UINT8               systemtype;         /*system type 0:atsc, 1: dvb*/
    UINT8	            ColorDepth;			/**COLOR_DEPTH_T */
    UINT8	            LVDSPixel;			/**LVDS_PIXEL_T */
	UINT8				vcomPgammaChecksum;	/*Vcom Pgamma value checksum */
	UINT8				nDebugStatus;
#ifndef _EMUL_WIN
    UINT8				reset20110129[0];	/* Uncomment and change name of member to reset values in NVRAM */
#endif
//yj74.kim	UINT8				bSupportB0LVDSMAP;	/* FOR BCM BOARD have reversed LVDS MAP */
	UINT8				swuMode;			/* swu mode */
	SYS_TUNER_OPT_T		nTunerOption;
	UINT8				make_hib;			/* for snapshot boot (make building) */
	UINT8				snapshot_onoff;		/* for snapshot support or not*/
	UINT32				adcMode;			/* for adc Mode 현재 ADC 설정 값을 저장  */
	UINT32				swum_magic;		/* swu mode magic */
	UINT8				soundout_mode;			/* soundout mode save 0:tv speaker, 1:external speaker 2:optical sound bar, 3:headphone,4:lineout,5;bluetooth soundbar */
	UINT32				zram;				/* zram */
} SYS_DB_T;


/******************************************************
 			TOOL_OPTION1 에 필요한 구조체 정의
*****************************************************/
// 아래 구조체 변경 시 반드시 각 항목의 MAX 값도 같이 변경할 것
#define MAX_MODULE_VER		4

/**
 *	Type of EU Area Option Country
 */
 typedef enum
{
	OPT_AREA_OLD_EU		=0,		//nordic 이외 국가 전체를 grouping 할 경우, 기양산 SW에 적용된 nordic이외에 국가 구분에 해당함
	OPT_AREA_EAST_EU	=1,		//EAST EU 국가 grouping
	OPT_AREA_WEST_EU	=2,		//WEST EU 국가 grouping
	OPT_AREA_ETC_EU		=3,		//Nordic/EAST/WEST 이외 국가 grouping (CIS 러시아 등)
	OPT_AREA_EU_MAX		=4,
} OPT_AREA_EU_COUNTRY_T;


/* This type should be same with ePanel_SPI_Type in drv_display.h */
typedef enum
{
	LED_BAR_REV_BLK	 = 0,
	LED_BAR_H_4BLK		 = 1,
	LED_BAR_H_12BLK		 = 2,
	LED_BAR_H_6BLK		 = 3,

	LED_BAR_D_10BLK		= 4,
	LED_BAR_REV1_BLK		= 5,
	LED_BAR_REV2_BLK		= 6,
	LED_BAR_REV3_BLK		= 7,
	LED_BAR_MAX	 		= 8,
} LED_BAR_TYPE_T;


typedef enum
{
	LD_10_16_BLK		 = 0, //V_16BLK, V10BLK, RES_I, RES_II  (for 2011 modules)
	LD_4_6_12_BLK		 = 1, //V_12BLK, V_6BLK, H_12BLK, H_6BLK (for 2012 modules)
	LD_BLK_MAX	 	 = 2,
} LOCAL_DIM_BLOCK_TYPE_T;


typedef enum
{
	LED_CUR_MINIMUN				= 0,
	LED_CUR_DEFAULT				= 1, //no-jackboard (only use mainboard)
	LED_CUR_DEFAULT_PLUS_I		= 2, //same gp1 venus jackboard
	LED_CUR_DEFAULT_PLUS_II		= 3,
	LED_CUR_MAX					= 4,
} LED_CURRENT_T;

/**
 * Named size of panel, comes from nvm_ddi.h
 */
typedef enum
{
	INCH_29=0,
	INCH_32,
	INCH_37,
	INCH_39,
	INCH_42,
	INCH_47,
	INCH_50,
	INCH_55,
	INCH_60,
	INCH_70,
	INCH_75,
	INCH_84,
	INCH_95,
	INCH_100,

	INCH_22,
	INCH_23,
	INCH_24,
	INCH_26,
	INCH_27, // Smart Monitor TV

	INCH_65, // UHD Model
	INCH_BASE
} INCH_TYPE_T;

/**
 * not used inch type
 */
typedef enum
{
	INCH_15= INCH_BASE,
	INCH_19,
//	INCH_22,
//	INCH_23,
//	INCH_26,
   	INCH_46,
	INCH_52,
//	INCH_65,
	INCH_72,
} OLD_INCH_TYPE_T;

typedef enum
{
	TOOL_EA88=0, 	// 1
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
	TOOL_LA97,		// 28
	TOOL_LA623,		// 29
	TOOL_LA71, 		// 30
	TOOL_LN61, 		// 31
	TOOL_LA88, 		// 32
	TOOL_LA63, 		// 33
	TOOL_GA68, 		// 34
	TOOL_LA697, 	// 35
	TOOL_LN571, 	// 36
	TOOL_LCD_END,

	//PDP Tool
	TOOL_PN670= TOOL_LCD_END,	// 1
	TOOL_PN470,					// 2
	TOOL_PH670,					// 3
	TOOL_PH470,					// 4
	TOOL_PN570,					// 5
	TOOL_PDP_END,
	TOOL_BASE	= TOOL_PDP_END,	// LCD Tool이 추가되어 PDP Tool의 수보다는 많을것으로 예상됨.
} TOOL_TYPE_T;


//not used tool type (old type)
typedef enum
{
	TOOL_LK31= TOOL_PDP_END,    // 1 - Analog
	TOOL_LK33,      // 2
	TOOL_LK43,		// 3
	TOOL_LK45,		// 4 - FHD
	TOOL_LK52,		// 5
	TOOL_LK53,		// 6
	TOOL_LK55,		// 7 - 120Hz
	TOOL_LV23,		// 8
	TOOL_LV25,		// 9
	TOOL_LV33,		// 10
	TOOL_LV34,		// 11
	TOOL_LV35, 		// 12
	TOOL_LV45,		// 13
	TOOL_LW45,		// 14
	TOOL_LV55,		// 15
	TOOL_LW55,		// 16
	TOOL_LW65,		// 17
	TOOL_LW75,		// 18
	TOOL_LV75,		// 19
	TOOL_LW77,		// 20
	TOOL_LW85,		// 21
	TOOL_LW95,		// 22
	TOOL_LW96,		// 23
	TOOL_LW97,		// 24 - 72" IOL, 60" IOP
	TOOL_LW98,		// 25 - 21:9
	TOOL_LW56,		// 26
	TOOL_LW57,		// 27
	TOOL_LV37,		// 28
	TOOL_LZ97,		// 29
	TOOL_LV54,		// 30
	TOOL_LV57,		// 31
	TOOL_LZ96,		// 32
	TOOL_LV375,		// 33
	TOOL_LV77,		// 34
	TOOL_LV95,		// 35
	TOOL_LV47,		// 36
	TOOL_LW47,		// 37
	TOOL_LK95,		// 38
	TOOL_LZ99,		// 39
	//NetCast3.0 Tool Option
	TOOL_CS41,		// 1
	TOOL_CS46,		// 2
	TOOL_CS56,		// 3
	TOOL_CS57,		// 4
	TOOL_EM96,		// 5
	TOOL_LM46,		// 6
	TOOL_LM58,		// 7
	TOOL_LM62,		// 8
	TOOL_LMG62, 	// 9
	TOOL_LM63,		// 10
	TOOL_LM66,		// 11
	TOOL_LM67,		// 12
	TOOL_LM76,		// 13
	TOOL_LM86,		// 14
	TOOL_LM96,		// 15
	TOOL_LM99,		// 16
	TOOL_LS22,		// 17
	TOOL_LS31,		// 18
	TOOL_LS35,		// 19
	TOOL_LS41,		// 20
	TOOL_LS46,		// 21
	TOOL_LS56,		// 22
	TOOL_LS57,		// 23
	TOOL_LM95,		// 24
	TOOL_LM64,		// 25 - cinema screen support		<LM6400>
	TOOL_LM645, 	// 26 - cinema screen not support	<LM6450>
	TOOL_LM61,		// 27
	TOOL_LS575, 	// 28
	TOOL_LM59,		// 29

	//PDP NetCast3.0 Tool
	TOOL_PM990,		// 1
	TOOL_PM980,		// 2
	TOOL_PM970,		// 3
	TOOL_PM950,		// 4
	TOOL_PM890,		// 5
	TOOL_PM880,		// 6
	TOOL_PM870,		// 7
	TOOL_PM850,		// 8
	TOOL_PM690,		// 9
	TOOL_PM680,		// 10
	TOOL_PM670,		// 11
	TOOL_PM650,		// 12
	TOOL_PM590,		// 13
	TOOL_PM580,		// 14
	TOOL_PM570,		// 15
	TOOL_PM550,		// 16
	TOOL_PM490,		// 17
	TOOL_PM480,		// 18
	TOOL_PM470,		// 19
	TOOL_PM450,		// 20
	TOOL_PA690,		// 21
	TOOL_PA670,		// 22
	TOOL_PA650,		// 23
	TOOL_PA590,		// 24
	TOOL_PA570,		// 25
	TOOL_PA550,		// 26
	TOOL_PA490,		// 27
	TOOL_PA470,		// 28
	TOOL_PA450,		// 29
	//End of NetCast3.0 Tool Option
} OLD_TOOL_TYPE_T;
typedef enum
{
	MODULE_LGD = 0,
	MODULE_CMI,
	MODULE_AUO,
	MODULE_SHARP,
	MODULE_IPS,
	MODULE_BOE,
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
		AMP_NTP7513 = 0,	/* NTP7513	*/
		AMP_TAS5733,		/* TAS5733	*/
		AMP_STA380, 		/* ST380	*/
		AMP_STA368, 		/* ST368	*/
		AMP_STA2AMP,		/* STA2AMP	*/

		// not use
		AMP_NTP7400,	/* NTP7400	*/
		AMP_NTP7500,		/* NTP7500	*/
		AMP_NTP2AMP,		/* NTP2AMP	*/
#if 0
		AMP_NTPSNDBAR,		/* Sound Bar 2AMP */ //LEX8
		AMP_TASSNDBAR,		/* TAS5709	*/
		AMP_TAS5727,		/* TAS5727	*/
		AMP_TAS2AMP,
#endif
}AMP_CHIP_TYPE_T;


/* Backlight type*/
typedef enum
{
	BL_ROW = 0,
	BL_NOR_LED,
	BL_EDGE_LED,
	//BL_IOP_LED,
	BL_ALEF_LED,
	BL_IOL_LED,
	BL_OLED,
	BL_POLA,
	BL_END,

	//Not use
	BL_CCFL,
	//BL_ALEF_LGD,
	//BL_ALEF_FPR,
}BACKLIGHT_TYPE_T;



/* Backlight type*/
typedef enum
{
	BL_IOP_LED = BL_END,
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


/******************************************************
 			Area Option 에 필요한 구조체 정의
******************************************************/
// 아래 구조체 변경 시 반드시 각 항목의 MAX 값도 같이 변경할 것
#define LANG_GROUP_MAX			3
#define DEFAULT_LANG_MAX		3
#define TTX_LANG_GROUP_MAX		6
#define LOCATION_MAX			8
#define WIFI_FREQ_MAX			8

typedef enum{
	LANG_GROUP_AJ_ALL = 0,
	LANG_GROUP_AJ_ARABIC,
	LANG_GROUP_AJ_HEBREW
}LANG_GROUP_T;

typedef enum{
	DEFAULT_LANG_ENGLISH = 0,
	DEFAULT_LANG_PORTUGUESE,
	DEFAULT_LANG_SPANISH
}DEFAULT_LANG_T;

typedef enum{
	TTX_LANG_GROUP_WES_EU = 0,
	TTX_LANG_GROUP_EST_EU,
	TTX_LANG_GROUP_RUS,
	TTX_LANG_GROUP_ARA,
	TTX_LANG_GROUP_FARSI,
	TTX_OFF
}TTX_LANG_GROUP_T;

// 090721 현재까지 요청한 Location 이 없음
typedef enum{
	LOCATION_OFF = 0
}LOCATION_T;

typedef enum{
	WIFI_FREQ_GROUP_0 = 0,
	WIFI_FREQ_GRUOP_1,
	WIFI_FREQ_GROUP_2,
	WIFI_FREQ_GROUP_3,
	WIFI_FREQ_GROUP_4,
	WIFI_FREQ_GROUP_5,
	WIFI_FREQ_GROUP_6,
	WIFI_FREQ_GROUP_7,
}WIFI_FREQ_T;



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
		UINT16 					eModelInchType  		: 4;
		UINT16					eModelToolType			: 6;
		UINT16					eModelInchType2			: 1;
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
		UINT16 					bTHX					: 1;
		UINT16					bISF					: 1;
		UINT16 					bSupportEPA				: 1;
		UINT16					bDualView				: 1;
		UINT16					bPip					: 1;
		UINT16					bCommercial				: 1;
		UINT16					bNfc					: 1;
		UINT16 					bLocalDimming			: 1;
		UINT16					bLocalDimMenu			: 1;
		UINT16					bMotionOPC				: 1;
		//UINT16					bToolDB3_reserved		: 1;
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
		UINT16					eDigitalDemod_S		: 3;
		UINT16					eDigitalDemod		: 4;
		UINT16					eAnalogDemod		: 3;
		UINT16					eDigitalDemod_Sub	: 4;
		UINT16					eAnalogDemod_Sub	: 2;
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
		UINT16					eSupportWiFi		: 2;
		UINT16					eSupportCameraReady	: 2;
		UINT16					eSupportMouseRC		: 3;
		UINT16					bTVLink 			: 1;
		UINT16					bMirrorMode 		: 1;
		UINT16					bOrangeService		: 1;
		UINT16					bSupportCNTV		: 1;
		UINT16					e3DModelType		: 1;
		UINT16					bLoalDim_BLK_Type	: 1;
		UINT16					bScanningBL			: 1;
		UINT16					bSupport3DTHX		: 1;
		UINT16					bXvYcc				: 1;
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
		UINT16 					nAudioPowerVoltage			: 2;
		UINT16 					nSpeakerWattage				: 2;
		UINT16 					nAudioPowerVoltage2			: 1; // voltage bit 부족으로 wattage bit 을 추가 할당받아 사용.
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
		UINT16					bToolDB7_reserved		: 1;
		UINT16					bSupportDigitalEye		: 1;
		//UINT16					eLEDCurrent				: 2;
		UINT16					bPowerBoardType			: 1;
		UINT16					eLEDBarType				: 2;
		UINT16					bMRCU					: 1;
		UINT16					bGesture				: 1;
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
		UINT16					eLocalKeyPos			: 9;
		UINT16					bToolDB8_reserved		: 1;
		UINT16					bToolDB8_reserved2		: 1;
		UINT16					bToolDB8_reserved3		: 1;
		UINT16 					bToolDB8_reserved4		: 1;
		UINT16					bToolDB8_reserved5		: 1;
		UINT16 					bToolDB8_reserved6		: 1;
		UINT16					bToolDB8_reserved7		: 1;
	} flags;
} TOOL_OPTION8_T;


// stonedef - 090717
// Area option 구조체 정의
typedef union
{
	UINT16 all;
	struct
	{
		UINT16					eWiFiFreq			: 3;		/* Wi-Fi Frequency */
		UINT16					eLangGroup			: 2;		/* Langage Group */
		UINT16					eTTXLangGroup		: 3;		/* Teletext Langage Group */
		UINT16					bI_II_Save			: 1;		/* I/II Save 지원유무 */
		UINT16					bHDEV				: 1;		/* Audio HDEV On/Off*/
		UINT16					bMONO				: 1;		/* 강제 모노 지원유무 */
		UINT16					eLocation			: 3;
		UINT16					bIsolator			: 1;		/* Isolator 지원 유무 */
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
	UINT32              validMark;
	TOOL_OPTION1_T		nToolOption1;
	TOOL_OPTION2_T		nToolOption2;
	TOOL_OPTION3_T		nToolOption3;
	TOOL_OPTION4_T		nToolOption4;
	TOOL_OPTION5_T		nToolOption5;
	TOOL_OPTION6_T		nToolOption6;
	TOOL_OPTION7_T		nToolOption7;
	AREA_OPTION_T		stAreaOptions;		/* Area Option for 아주/중아/중남미 */
	AREA_OPTION_EU_T	stEuAreaOptions;	/* Area Option for 구주 */
	EXT_VIDEO_INPUT_ADJ_T stExtVideoInputADJ;
	EXT_AUDIO_INPUT_ADJ_T stExtAudioInputADJ;
	TOOL_OPTION8_T		nToolOption8;
	UINT8 				nConfig;
}	TOOLOPTION_DB_T;



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
*	PRESERVE DB ( 양산 이후에 Reset 되거나 임의로 변경되지 말아야 하는 DB )
*/
typedef struct PRESERVE_DB
{
		UINT32		validMark;
		UINT8 		bInstopReserved;
		UINT8 		bInstopCompleted;
	   	SYS_BAUDRATE_T     	eBaudrate;
		UINT8		b1stBootAfterInstop;
}PRESERVE_DB_T;



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



#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /*_CMNIO_TYPE_H_*/




