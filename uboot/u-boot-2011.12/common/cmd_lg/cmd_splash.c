/******************************************************************************
 *	 DTV LABORATORY, LG ELECTRONICS INC., SEOUL, KOREA
 *	 Copyright(c) 2009 by LG Electronics Inc.
 *
 *	 All rights reserved. No part of this work may be reproduced, stored in a
 *	 retrieval system, or transmitted by any means without prior written
 *	 permission of LG Electronics Inc.
 *****************************************************************************/

/** @file cmd_splash.c
 *
 *	This c file defines the functions to display splash window(logo window)
 *
 *	@author 	haeyong.pyun@lge.com
 *	@version	1.0
 *	@date		2011.06.28
 *	@note
 *	@see		cmd_splash.c in MTK's boot
 */


/*-------------------------------------------------------------
 * include files
 *------------------------------------------------------------*/
#include <common.h>
#include <x_typedef.h>
#include <splash_auxinfo_bmp.h>
//#include "../../../drv_lib/drv_inc/drvcust_if.h"

#include "x_ckgen.h"

//#include "cfe_nvm.h"
#include "cmd_splash.h"
#include "partinfo.h"
#include "cmd_polltimer.h"
#include <cmd_micom.h>
#include <cmnio_type.h>

/*-------------------------------------------------------------
 * constant definitions
 *------------------------------------------------------------*/
#define 	_RETRY_URSA3_WRITE	3
#define		SYS_ATSC	(0)
#define 	SYS_DVB		(1)
#define		NUM_TOOL_OPTION		(7)

/*-------------------------------------------------------------
 * macro function definitions
 *------------------------------------------------------------*/

/*-------------------------------------------------------------
 * type definitions
 *------------------------------------------------------------*/



/*
   URSA3 Configure of each LCD model size(inch)
   */
typedef	struct
{
	UINT8	inch;
	UINT8	data[3];

}	URSA3_CONF_INCH_T;

/******************************************************************************
  전역 형 정의 (Global Type Definitions)
 ******************************************************************************/
/******************************************************************************
  Static 변수와 함수 prototype 선언
  (Static Variables & Function Prototypes Declarations)
 ******************************************************************************/
UINT16 gToolOpt[NUM_TOOL_OPTION] = {0, };
static UINT32	_gModelOptions;
PANEL_INFO_T	gPanelInfo = {0, };

#ifndef NEW_BOOT_LOGO
extern void	Splash_GetImageInfo(SPLASH_BMP_TYPE_T bmpType, SPLASH_BMP_INFO_T *pSplashBmpInfo, void *argv, unsigned char systype );
#endif
extern void	Splash_MICOM_GetRTC( TIME_T *pTime );
extern void Splash_DrawImage(unsigned int x, unsigned int y, unsigned int u4Width, 
	unsigned int u4Height, unsigned int u4BmpAddr, int panelRes);
extern int read_blocks_with_bg_task (off_t ofs, size_t *len, u_char *buf, void (*bg_task(BGTASK_POWSEQ_FLAG)));

void poll_timer(void);
extern UINT8 DDI_NVM_GetSoundOutMode( void );





//extern int add_timer(unsigned int interval, unsigned int oneshot, void (*func)(void*), void *arg);
//extern int del_timer(int id);
//extern void poll_timer(void);
//extern void wait_oneshot_timer(void); 


//extern int add_timer(unsigned int interval, unsigned int oneshot, void (*func)(void*), void *arg);
//extern int del_timer(int id);
//extern void polling_timer(void);
//extern void wait_oneshot_timer(void); 

/******************************************************************************
  전역 변수와 함수 prototype 선언
  (Global Variables & Function Prototypes Declarations)
 ******************************************************************************/
uint8_t gDispType = 0xff;
/******************************************************************************
  로컬 변수와 함수 prototype 선언
  (Local Variables & Function Prototypes Declarations)
 ******************************************************************************/
UINT32 Splash_GetHWoption(HW_OPT_T option_mask);

/******************************************************************************
  함수 정의 (Function Definitions)
 ******************************************************************************/
#define	_RETRY_MICOM	3

//#define mdelay(x)		(udelay(x) * (1000))

extern char strModelOpt[];
extern MODELOPT_T gModelOpt;
unsigned short __InvertOn = 0; //for checking whether Invert is on

int Get_modelOpt(HW_OPT_T option_mask)
{
/***************************************
************** HW OPTION ***************
****************************************/

/*
	 ================ DV-S board 이전 option ====================
	 OPTION0 : FRC OPT 0
	 OPTION1 : FRC OPT 1
	 OPTION2 : PANEL RESOLUTION  		(LOW: HD(1366x768)	, HIGH: FHD(1920x1080)
	 OPTION3 : DISPLAY TYPE(OPTIC)	(LOW : LCD			, HIGH: OLED)
	 OPTION4 : RESERVED
	 OPTION5 : DDR					(LOW: 1G			, HIGH: 1.25G)
	 OPTION6 : CP BOX				(LOW: NOT SUPPORT	, HGIH: SUPPORT)
	 OPTION7 : T2	 TUNER				(LOW: NOT SUPPORT	, HIGH: SUPPORT)
	 OPTION8 : S TUNER				(LOW: NOT SUPPORT	, HIGH: SUPPORT)
	 OPTION9 : RESERVED				(LOW: DEFAULT		, HIGH: )
	 OPTION10 : EPI					(LOW: NOT SUPPORT	, HIGH: SUPPORT)

	 <FRC OPTION>
	   OPT0		OPT1
	   LOW		LOW			: NO FRC
	   LOW		HIGH		: SOC INTERNAL FRC
	   HIGH		LOW			: EXTERNAL LG FRC
	   HIGH		HIGH 		: EXTERNAL URSA5

	<Display Type from MICOM>
		LCD			: 0
		OLED		: 1
		PDP			: 2
	 ================ DV-S board 이후 option ====================
	 OPTION0 : RESERVED
	 OPTION1 : FRC					(LOW: NOT SUPPORT	, HIGH: FRC internal(120Hz)
	 OPTION2 : PANEL RESOLUTION 		(LOW: HD(1366x768)	, HIGH: FHD(1920x1080)
	 OPTION3 : DISPLAY TYPE(OPTIC)	(LOW: LCD			, HIGH: OLED)
	 OPTION4 : PANEL TYPE				(LOW: V12			, HIGH: V13)
	 OPTION5 : DDR0					
	 OPTION6 : CP BOX				(LOW: NOT SUPPORT	, HGIH: SUPPORT)
	 OPTION7 : T2	 TUNER				(LOW: NOT SUPPORT	, HIGH: SUPPORT)
	 OPTION8 : S TUNER				(LOW: NOT SUPPORT	, HIGH: SUPPORT)
	 OPTION9 : DDR1 				
	 OPTION10 : EPI 					(LOW: NOT SUPPORT	, HIGH: SUPPORT)

	<Display Type from MICOM>
		LCD 		: 0
		OLED		: 1
		PDP 			: 2	 

	<DDR Size option>
	DDR0	 DDR1
	LOW 	 LOW		 : 1.5G
	LOW 	 HIGH		 : Not defined
	HIGH	 LOW		 : 1.25G
	HIGH	 HIGH		 : 768M

*/
	switch(option_mask)
	{
		case FRC_OPT_SEL:
			return (int)gModelOpt.bSupport_frc;
			break;

		case PANEL_RES_OPT_SEL:
#ifndef CC_L_PANEL_FLOW
            printf ("Get_modelOpt force fhd\n");
            return 1;
#else
			return (gModelOpt.panel_resolution == MODELOPT_PANEL_RESOLUTION_FHD)? 1:0;
#endif
			break;

		case PANEL_TYPE_OPT_SEL:
			return (gModelOpt.panel_type != MODELOPT_PANEL_V12)? 1:0;
			break;

		case CPBOX_OPT_SEL:
			return (int)gModelOpt.bSupport_cp_box;
			break;

		case EPI_OPT_SEL:
#ifndef CC_L_PANEL_FLOW
            printf ("Get_modelOpt force not support epi\n");
            return 0;       //low, not support
            break;
#else
			return (gModelOpt.panel_interface == MODELOPT_PANEL_INTERFACE_EPI)? 1:0;
#endif
			break;

		case DISPLAY_TYPE_OPT_SEL:
#ifndef CC_L_PANEL_FLOW
		printf("Get_modelOpt force return LCD type\n");
		return 0;
#else
			if (0xff != gDispType)
			{
				return gDispType;
			}
			else
			{
				if ( 0 != MICOM_GetHWoption(&gDispType) )
					gDispType = 0;		//default LCD

				return gDispType;
			}
			break;
#endif
		case HW_OPT_PANEL_INTERFACE:
			return (int)gModelOpt.panel_interface;
			break;

		case HW_OPT_PANEL_RESOLUTION:
			return (int)gModelOpt.panel_resolution;
			break;

		default:
			return -1;
			break;
	}


}


void Splash_MICOM_GetRTC( TIME_T *pTime )
{

	UINT8	i;
	TIME_T 	tTime;

	const TIME_T	tInvalidTime = { 0, };

	*pTime = tInvalidTime;

	for( i = 0; i < _RETRY_MICOM; i ++ )
	{
		if( MICOM_GetRTStime( &tTime ) == 0 )
		{
			//printf("[%d,%d,%d] [%d,%d,%d] \n",tTime.year,tTime.month,tTime.day,tTime.hour,tTime.minute,tTime.second);

			if( 	(tTime.month  < 13)
					&&	(tTime.day    < 32)
					&&	(tTime.hour   < 24)
					&&	(tTime.minute < 60)
					&&	(tTime.second < 61)	)
			{
				*pTime = tTime;
				break;
			}
		}

		udelay(10000);
	}

	//printf("[%d,%d,%d] [%d,%d,%d] \n",pTime->year,pTime->month,pTime->day,pTime->hour,pTime->minute,pTime->second);
}


void Splash_MICOM_PanelOn( void )
{

	UINT8	i;
	UINT8	panel_on = 0;

	for( i = 0; i < _RETRY_MICOM; i ++ )
	{
		MICOM_TurnOnPanel();
		udelay(1000);
		MICOM_VerifyPanelOn( &panel_on );
		if( panel_on == 1)	break;

		udelay(10000);
	}

	if( ! panel_on )
	{
		printf("turn on PANEL: FAILED\n");
	}
}

void Splash_MICOM_InvOn( void )
{

	UINT8	i;
	UINT8	inv_on = 0;

	for( i = 0; i < _RETRY_MICOM; i ++ )
	{
		MICOM_TurnOnInv();
		udelay(1000);
		MICOM_VerifyInvertOn( &inv_on );

		if( inv_on == 1)	break;

		udelay(10000);
	}

	if( ! inv_on )
	{
		printf("turn on INV: FAILED\n");
	}

}

void Splash_GetPanelInfo(TOOL_OPTION1_T* opt1)
{
	gPanelInfo.panel_resolution	= Get_modelOpt(HW_OPT_PANEL_RESOLUTION);
	gPanelInfo.panel_interface	= Get_modelOpt(HW_OPT_PANEL_INTERFACE);
	gPanelInfo.bSupport_frc		= Get_modelOpt(FRC_OPT_SEL);
	gPanelInfo.eModelModuleType = opt1->flags.eModelModuleType;
	gPanelInfo.nLVDSBit			= opt1->flags.nLVDSBit;

	printf("######panel_resolution: %d\n",gPanelInfo.panel_resolution);
	printf("######panel_interface: %d\n",gPanelInfo.panel_interface);
	printf("######bSupport_frc: %d\n",gPanelInfo.bSupport_frc);
	printf("######eModelModuleType: %d\n",gPanelInfo.eModelModuleType);
	printf("######nLVDSBit: %d\n",gPanelInfo.nLVDSBit);
}

void Splash_DrawLogoImage(unsigned int loadaddr, unsigned char systype)
{
	SPLASH_BMP_INFO_T	splash_bmp_info;
	TIME_T				curRTCTime;

	TIME_T *pTimeSt;
	int time;
	unsigned char bIsTimeInvalid = FALSE;
	unsigned char hour;

#ifdef NEW_BOOT_LOGO
	Splash_DrawImage(0, 0, 0, 0, loadaddr, Get_modelOpt(PANEL_RES_OPT_SEL));
#else
	printf("DRAW OLD BOOT LOGO \n");
	/*-----------------------------------------------
	 * Get RTC time & Display time
	 *-----------------------------------------------*/
	Splash_MICOM_GetRTC( &curRTCTime );

	pTimeSt = (TIME_T*)&curRTCTime;
	hour = pTimeSt->hour;

	if( (pTimeSt != NULL) &&
			(pTimeSt->year == 0 || pTimeSt->month == 0 || pTimeSt->day == 0) )
	{
		bIsTimeInvalid = TRUE;
	}

	Splash_GetImageInfo(SPLASH_LG_LOG_BMP, &splash_bmp_info, (void *)&curRTCTime, systype);
	Splash_DrawImage(splash_bmp_info.xPos, splash_bmp_info.yPos, splash_bmp_info.w, splash_bmp_info.h, (loadaddr + splash_bmp_info.offset) );

	Splash_GetImageInfo(SPLASH_BG_GRAPHIC_BMP, &splash_bmp_info, (void *)&curRTCTime, systype);
	Splash_DrawImage(splash_bmp_info.xPos, splash_bmp_info.yPos, splash_bmp_info.w, splash_bmp_info.h, (loadaddr + splash_bmp_info.offset) );

	Splash_GetImageInfo(SPLASH_CLOCK_ICON_BMP, &splash_bmp_info, (void *)&curRTCTime, systype);
	if (systype == SYS_ATSC && !bIsTimeInvalid)
		splash_bmp_info.xPos = splash_bmp_info.xPos + 394;
	Splash_DrawImage(splash_bmp_info.xPos, splash_bmp_info.yPos, splash_bmp_info.w, splash_bmp_info.h, (loadaddr + splash_bmp_info.offset) );

	Splash_GetImageInfo(SPLASH_TIME_COLON_BMP, &splash_bmp_info, (void *)&curRTCTime, systype);
	if(systype == SYS_ATSC && !bIsTimeInvalid)
		splash_bmp_info.xPos = splash_bmp_info.xPos - 166;
	Splash_DrawImage(splash_bmp_info.xPos, splash_bmp_info.yPos, splash_bmp_info.w, splash_bmp_info.h, (loadaddr + splash_bmp_info.offset) );

	Splash_GetImageInfo(SPLASH_TIME_HH_BMP, &splash_bmp_info, (void *)&curRTCTime, systype);
	if(systype == SYS_ATSC && !bIsTimeInvalid)
		splash_bmp_info.xPos = splash_bmp_info.xPos - 166;
	Splash_DrawImage(splash_bmp_info.xPos, splash_bmp_info.yPos, splash_bmp_info.w, splash_bmp_info.h, (loadaddr + splash_bmp_info.offset) );

	Splash_GetImageInfo(SPLASH_TIME_H_BMP, &splash_bmp_info, (void *)&curRTCTime, systype);
	if(systype == SYS_ATSC && !bIsTimeInvalid)
		splash_bmp_info.xPos = splash_bmp_info.xPos - 166;
	Splash_DrawImage(splash_bmp_info.xPos, splash_bmp_info.yPos, splash_bmp_info.w, splash_bmp_info.h, (loadaddr + splash_bmp_info.offset) );

	Splash_GetImageInfo(SPLASH_TIME_MM_BMP, &splash_bmp_info, (void *)&curRTCTime, systype);
	if(systype == SYS_ATSC && !bIsTimeInvalid)
		splash_bmp_info.xPos = splash_bmp_info.xPos - 166;
	Splash_DrawImage(splash_bmp_info.xPos, splash_bmp_info.yPos, splash_bmp_info.w, splash_bmp_info.h, (loadaddr + splash_bmp_info.offset) );

	Splash_GetImageInfo(SPLASH_TIME_M_BMP, &splash_bmp_info, (void *)&curRTCTime, systype);
	if(systype == SYS_ATSC && !bIsTimeInvalid)
		splash_bmp_info.xPos = splash_bmp_info.xPos - 166;
	Splash_DrawImage(splash_bmp_info.xPos, splash_bmp_info.yPos, splash_bmp_info.w, splash_bmp_info.h, (loadaddr + splash_bmp_info.offset) );
#endif
}

#ifndef NEW_BOOT_LOGO
void Splash_GetImageInfo(SPLASH_BMP_TYPE_T bmpType, SPLASH_BMP_INFO_T *pSplashBmpInfo, void *argv, unsigned char systype )
{
	TIME_T *pTimeSt;
	int time;
	unsigned char bIsTimeInvalid = FALSE;
	unsigned char bIsPM = FALSE;
	unsigned char hour;

	pTimeSt = (TIME_T*)argv;
	hour = pTimeSt->hour;

	if( (pTimeSt != NULL) &&
			(pTimeSt->year == 0 || pTimeSt->month == 0 || pTimeSt->day == 0) )
	{
		bIsTimeInvalid = TRUE;
	}
	else
	{
		if(systype == SYS_ATSC)
		{
			if( hour >= 12 )
				bIsPM = TRUE;

			hour %= 12;
			if( hour == 0 )
				hour+=12;

		}
	}

	switch ( bmpType )
	{
		case SPLASH_BG_BOOTING_BMP:
#if 0
			pSplashBmpInfo->offset	= BOOTING_BG_OFFSET;
			pSplashBmpInfo->size	= BOOTING_BG_SIZE;
			pSplashBmpInfo->xPos	= BOOTING_BG_X_POS;
			pSplashBmpInfo->yPos	= BOOTING_BG_Y_POS;
#else
			pSplashBmpInfo->offset	= BG_LOGO_OFFSET;
			pSplashBmpInfo->size	= BG_LOGO_SIZE;
			pSplashBmpInfo->xPos	= BG_LOGO_X_POS;
			pSplashBmpInfo->yPos	= BG_LOGO_Y_POS;
			pSplashBmpInfo->w		= BG_LOGO_WIDTH;
			pSplashBmpInfo->h		= BG_LOGO_HEIGHT;
#endif
			break;

		case SPLASH_BG_GRAPHIC_BMP:
			pSplashBmpInfo->offset	= BG_GRAPHIC_OFFSET;
			pSplashBmpInfo->size	= BG_GRAPHIC_SIZE;
			pSplashBmpInfo->xPos	= BG_GRAPHIC_X_POS;
			pSplashBmpInfo->yPos	= BG_GRAPHIC_Y_POS;
			pSplashBmpInfo->w		= BG_GRAPHIC_WIDTH;
			pSplashBmpInfo->h		= BG_GRAPHIC_HEIGHT;
			break;

		case SPLASH_LG_LOG_BMP:
			pSplashBmpInfo->offset	= BG_LOGO_OFFSET;
			pSplashBmpInfo->size	= BG_LOGO_SIZE;
			pSplashBmpInfo->xPos	= BG_LOGO_X_POS;
			pSplashBmpInfo->yPos	= BG_LOGO_Y_POS;
			pSplashBmpInfo->w		= BG_LOGO_WIDTH;
			pSplashBmpInfo->h		= BG_LOGO_HEIGHT;
			break;

		case SPLASH_CLOCK_ICON_BMP:
			if(systype == SYS_ATSC)
			{
				if(!bIsTimeInvalid)
				{
					if(!bIsPM)
					{
						pSplashBmpInfo->offset	= TIME_AM_OFFSET;
						pSplashBmpInfo->size	= TIME_AM_SIZE;
						pSplashBmpInfo->xPos	= TIME_AM_X_POS;
						pSplashBmpInfo->yPos	= TIME_AM_Y_POS;
						pSplashBmpInfo->w		= TIME_AM_WIDTH;
						pSplashBmpInfo->h		= TIME_AM_HEIGHT;
					}
					else
					{
						pSplashBmpInfo->offset	= TIME_PM_OFFSET;
						pSplashBmpInfo->size	= TIME_PM_SIZE;
						pSplashBmpInfo->xPos	= TIME_PM_X_POS;
						pSplashBmpInfo->yPos	= TIME_PM_Y_POS;
						pSplashBmpInfo->w		= TIME_PM_WIDTH;
						pSplashBmpInfo->h		= TIME_PM_HEIGHT;
					}
				}
				else
				{
					pSplashBmpInfo->offset	= CLOCK_ICON_OFFSET;
					pSplashBmpInfo->size	= CLOCK_ICON_SIZE;
					pSplashBmpInfo->xPos	= CLOCK_ICON_X_POS;
					pSplashBmpInfo->yPos	= CLOCK_ICON_Y_POS;
					pSplashBmpInfo->w		= CLOCK_ICON_WIDTH;
					pSplashBmpInfo->h		= CLOCK_ICON_HEIGHT;
				}
			}
			else
			{
				pSplashBmpInfo->offset	= CLOCK_ICON_OFFSET;
				pSplashBmpInfo->size	= CLOCK_ICON_SIZE;
				pSplashBmpInfo->xPos	= CLOCK_ICON_X_POS;
				pSplashBmpInfo->yPos	= CLOCK_ICON_Y_POS;
				pSplashBmpInfo->w		= CLOCK_ICON_WIDTH;
				pSplashBmpInfo->h		= CLOCK_ICON_HEIGHT;
			}
			break;

		case SPLASH_TIME_HH_BMP:
			//TODO get time here
			if( bIsTimeInvalid == TRUE )
			{
				pSplashBmpInfo->offset	= INVALID_01_OFFSET;
				pSplashBmpInfo->size	= INVALID_01_SIZE;
				pSplashBmpInfo->xPos	= INVALID_01_X_POS;
				pSplashBmpInfo->yPos	= INVALID_01_Y_POS;
				pSplashBmpInfo->w		= INVALID_01_WIDTH;
				pSplashBmpInfo->h		= INVALID_01_HEIGHT;
				break;
			}

			time = hour/10; //pTimeSt->hour/10;
			if( time == 0 )
			{
				pSplashBmpInfo->offset	= TIME_HH_00_OFFSET;
				pSplashBmpInfo->size	= TIME_HH_00_SIZE;
				pSplashBmpInfo->xPos	= TIME_HH_00_X_POS;
				pSplashBmpInfo->yPos	= TIME_HH_00_Y_POS;
				pSplashBmpInfo->w		= TIME_HH_00_WIDTH;
				pSplashBmpInfo->h		= TIME_HH_00_HEIGHT;
			}
			else if( time == 1 )
			{
				pSplashBmpInfo->offset	= TIME_HH_10_OFFSET;
				pSplashBmpInfo->size	= TIME_HH_10_SIZE;
				pSplashBmpInfo->xPos	= TIME_HH_10_X_POS;
				pSplashBmpInfo->yPos	= TIME_HH_10_Y_POS;
				pSplashBmpInfo->w		= TIME_HH_10_WIDTH;
				pSplashBmpInfo->h		= TIME_HH_10_HEIGHT;
			}
			else if( time == 2 )
			{
				pSplashBmpInfo->offset	= TIME_HH_20_OFFSET;
				pSplashBmpInfo->size	= TIME_HH_20_SIZE;
				pSplashBmpInfo->xPos	= TIME_HH_20_X_POS;
				pSplashBmpInfo->yPos	= TIME_HH_20_Y_POS;
				pSplashBmpInfo->w		= TIME_HH_20_WIDTH;
				pSplashBmpInfo->h		= TIME_HH_20_HEIGHT;
			}
			else
			{
				pSplashBmpInfo->offset	= TIME_HH_00_OFFSET;
				pSplashBmpInfo->size	= TIME_HH_00_SIZE;
				pSplashBmpInfo->xPos	= TIME_HH_00_X_POS;
				pSplashBmpInfo->yPos	= TIME_HH_00_Y_POS;
				pSplashBmpInfo->w		= TIME_HH_00_WIDTH;
				pSplashBmpInfo->h		= TIME_HH_00_HEIGHT;
			}
			break;

		case SPLASH_TIME_H_BMP:
			if( bIsTimeInvalid == TRUE )
			{
				pSplashBmpInfo->offset	= INVALID_02_OFFSET;
				pSplashBmpInfo->size	= INVALID_02_SIZE;
				pSplashBmpInfo->xPos	= INVALID_02_X_POS;
				pSplashBmpInfo->yPos	= INVALID_02_Y_POS;
				pSplashBmpInfo->w		= INVALID_02_WIDTH;
				pSplashBmpInfo->h		= INVALID_02_HEIGHT;
				break;
			}

			time = hour%10; //pTimeSt->hour%10;
			if( time == 0 )
			{
				pSplashBmpInfo->offset	= TIME_HH_0_OFFSET;
				pSplashBmpInfo->size	= TIME_HH_0_SIZE;
				pSplashBmpInfo->xPos	= TIME_HH_0_X_POS;
				pSplashBmpInfo->yPos	= TIME_HH_0_Y_POS;
				pSplashBmpInfo->w		= TIME_HH_0_WIDTH;
				pSplashBmpInfo->h		= TIME_HH_0_HEIGHT;
			}
			else if( time == 1 )
			{
				pSplashBmpInfo->offset	= TIME_HH_1_OFFSET;
				pSplashBmpInfo->size	= TIME_HH_1_SIZE;
				pSplashBmpInfo->xPos	= TIME_HH_1_X_POS;
				pSplashBmpInfo->yPos	= TIME_HH_1_Y_POS;
				pSplashBmpInfo->w		= TIME_HH_1_WIDTH;
				pSplashBmpInfo->h		= TIME_HH_1_HEIGHT;
			}
			else if( time == 2 )
			{
				pSplashBmpInfo->offset	= TIME_HH_2_OFFSET;
				pSplashBmpInfo->size	= TIME_HH_2_SIZE;
				pSplashBmpInfo->xPos	= TIME_HH_2_X_POS;
				pSplashBmpInfo->yPos	= TIME_HH_2_Y_POS;
				pSplashBmpInfo->w		= TIME_HH_2_WIDTH;
				pSplashBmpInfo->h		= TIME_HH_2_HEIGHT;
			}
			else if( time == 3 )
			{
				pSplashBmpInfo->offset	= TIME_HH_3_OFFSET;
				pSplashBmpInfo->size	= TIME_HH_3_SIZE;
				pSplashBmpInfo->xPos	= TIME_HH_3_X_POS;
				pSplashBmpInfo->yPos	= TIME_HH_3_Y_POS;
				pSplashBmpInfo->w		= TIME_HH_3_WIDTH;
				pSplashBmpInfo->h		= TIME_HH_3_HEIGHT;
			}
			else if( time == 4 )
			{
				pSplashBmpInfo->offset	= TIME_HH_4_OFFSET;
				pSplashBmpInfo->size	= TIME_HH_4_SIZE;
				pSplashBmpInfo->xPos	= TIME_HH_4_X_POS;
				pSplashBmpInfo->yPos	= TIME_HH_4_Y_POS;
				pSplashBmpInfo->w		= TIME_HH_4_WIDTH;
				pSplashBmpInfo->h		= TIME_HH_4_HEIGHT;
			}
			else if( time == 5 )
			{
				pSplashBmpInfo->offset	= TIME_HH_5_OFFSET;
				pSplashBmpInfo->size	= TIME_HH_5_SIZE;
				pSplashBmpInfo->xPos	= TIME_HH_5_X_POS;
				pSplashBmpInfo->yPos	= TIME_HH_5_Y_POS;
				pSplashBmpInfo->w		= TIME_HH_5_WIDTH;
				pSplashBmpInfo->h		= TIME_HH_5_HEIGHT;
			}
			else if( time == 6 )
			{
				pSplashBmpInfo->offset	= TIME_HH_6_OFFSET;
				pSplashBmpInfo->size	= TIME_HH_6_SIZE;
				pSplashBmpInfo->xPos	= TIME_HH_6_X_POS;
				pSplashBmpInfo->yPos	= TIME_HH_6_Y_POS;
				pSplashBmpInfo->w		= TIME_HH_6_WIDTH;
				pSplashBmpInfo->h		= TIME_HH_6_HEIGHT;
			}
			else if( time == 7 )
			{
				pSplashBmpInfo->offset	= TIME_HH_7_OFFSET;
				pSplashBmpInfo->size	= TIME_HH_7_SIZE;
				pSplashBmpInfo->xPos	= TIME_HH_7_X_POS;
				pSplashBmpInfo->yPos	= TIME_HH_7_Y_POS;
				pSplashBmpInfo->w		= TIME_HH_7_WIDTH;
				pSplashBmpInfo->h		= TIME_HH_7_HEIGHT;
			}
			else if( time == 8 )
			{
				pSplashBmpInfo->offset	= TIME_HH_8_OFFSET;
				pSplashBmpInfo->size	= TIME_HH_8_SIZE;
				pSplashBmpInfo->xPos	= TIME_HH_8_X_POS;
				pSplashBmpInfo->yPos	= TIME_HH_8_Y_POS;
				pSplashBmpInfo->w		= TIME_HH_8_WIDTH;
				pSplashBmpInfo->h		= TIME_HH_8_HEIGHT;
			}
			else if( time == 9 )
			{
				pSplashBmpInfo->offset	= TIME_HH_9_OFFSET;
				pSplashBmpInfo->size	= TIME_HH_9_SIZE;
				pSplashBmpInfo->xPos	= TIME_HH_9_X_POS;
				pSplashBmpInfo->yPos	= TIME_HH_9_Y_POS;
				pSplashBmpInfo->w		= TIME_HH_9_WIDTH;
				pSplashBmpInfo->h		= TIME_HH_9_HEIGHT;
			}
			else
			{
				pSplashBmpInfo->offset	= TIME_HH_0_OFFSET;
				pSplashBmpInfo->size	= TIME_HH_0_SIZE;
				pSplashBmpInfo->xPos	= TIME_HH_0_X_POS;
				pSplashBmpInfo->yPos	= TIME_HH_0_Y_POS;
				pSplashBmpInfo->w		= TIME_HH_0_WIDTH;
				pSplashBmpInfo->h		= TIME_HH_0_HEIGHT;
			}
			break;

		case SPLASH_TIME_COLON_BMP:
			pSplashBmpInfo->offset	= TIME_COLON_OFFSET;
			pSplashBmpInfo->size	= TIME_COLON_SIZE;
			pSplashBmpInfo->xPos	= TIME_COLON_X_POS;
			pSplashBmpInfo->yPos	= TIME_COLON_Y_POS;
			pSplashBmpInfo->w	= TIME_COLON_WIDTH;
			pSplashBmpInfo->h	= TIME_COLON_HEIGHT;
			break;

		case SPLASH_TIME_MM_BMP:
			if( bIsTimeInvalid == TRUE )
			{
				pSplashBmpInfo->offset	= INVALID_03_OFFSET;
				pSplashBmpInfo->size	= INVALID_03_SIZE;
				pSplashBmpInfo->xPos	= INVALID_03_X_POS;
				pSplashBmpInfo->yPos	= INVALID_03_Y_POS;
				pSplashBmpInfo->w	= INVALID_03_WIDTH;
				pSplashBmpInfo->h	= INVALID_03_HEIGHT;
				break;
			}

			time = pTimeSt->minute/10;

			if( time == 0 )
			{
				pSplashBmpInfo->offset	= TIME_MM_00_OFFSET;
				pSplashBmpInfo->size	= TIME_MM_00_SIZE;
				pSplashBmpInfo->xPos	= TIME_MM_00_X_POS;
				pSplashBmpInfo->yPos	= TIME_MM_00_Y_POS;
				pSplashBmpInfo->w	= TIME_MM_00_WIDTH;
				pSplashBmpInfo->h	= TIME_MM_00_HEIGHT;
			}
			else if( time == 1 )
			{
				pSplashBmpInfo->offset	= TIME_MM_10_OFFSET;
				pSplashBmpInfo->size	= TIME_MM_10_SIZE;
				pSplashBmpInfo->xPos	= TIME_MM_10_X_POS;
				pSplashBmpInfo->yPos	= TIME_MM_10_Y_POS;
				pSplashBmpInfo->w	= TIME_MM_10_WIDTH;
				pSplashBmpInfo->h	= TIME_MM_10_HEIGHT;
			}
			else if( time == 2 )
			{
				pSplashBmpInfo->offset	= TIME_MM_20_OFFSET;
				pSplashBmpInfo->size	= TIME_MM_20_SIZE;
				pSplashBmpInfo->xPos	= TIME_MM_20_X_POS;
				pSplashBmpInfo->yPos	= TIME_MM_20_Y_POS;
				pSplashBmpInfo->w	= TIME_MM_20_WIDTH;
				pSplashBmpInfo->h	= TIME_MM_20_HEIGHT;
			}
			else if( time == 3 )
			{
				pSplashBmpInfo->offset	= TIME_MM_30_OFFSET;
				pSplashBmpInfo->size	= TIME_MM_30_SIZE;
				pSplashBmpInfo->xPos	= TIME_MM_30_X_POS;
				pSplashBmpInfo->yPos	= TIME_MM_30_Y_POS;
				pSplashBmpInfo->w	= TIME_MM_30_WIDTH;
				pSplashBmpInfo->h	= TIME_MM_30_HEIGHT;
			}
			else if( time == 4 )
			{
				pSplashBmpInfo->offset	= TIME_MM_40_OFFSET;
				pSplashBmpInfo->size	= TIME_MM_40_SIZE;
				pSplashBmpInfo->xPos	= TIME_MM_40_X_POS;
				pSplashBmpInfo->yPos	= TIME_MM_40_Y_POS;
				pSplashBmpInfo->w	= TIME_MM_40_WIDTH;
				pSplashBmpInfo->h	= TIME_MM_40_HEIGHT;
			}
			else if( time == 5 )
			{
				pSplashBmpInfo->offset	= TIME_MM_50_OFFSET;
				pSplashBmpInfo->size	= TIME_MM_50_SIZE;
				pSplashBmpInfo->xPos	= TIME_MM_50_X_POS;
				pSplashBmpInfo->yPos	= TIME_MM_50_Y_POS;
				pSplashBmpInfo->w	= TIME_MM_50_WIDTH;
				pSplashBmpInfo->h	= TIME_MM_50_HEIGHT;
			}
			else
			{
				pSplashBmpInfo->offset	= TIME_MM_00_OFFSET;
				pSplashBmpInfo->size	= TIME_MM_00_SIZE;
				pSplashBmpInfo->xPos	= TIME_MM_00_X_POS;
				pSplashBmpInfo->yPos	= TIME_MM_00_Y_POS;
				pSplashBmpInfo->w	= TIME_MM_00_WIDTH;
				pSplashBmpInfo->h	= TIME_MM_00_HEIGHT;
			}
			break;

		case SPLASH_TIME_M_BMP:

			if( bIsTimeInvalid == TRUE )
			{
				pSplashBmpInfo->offset	= INVALID_04_OFFSET;
				pSplashBmpInfo->size	= INVALID_04_SIZE;
				pSplashBmpInfo->xPos	= INVALID_04_X_POS;
				pSplashBmpInfo->yPos	= INVALID_04_Y_POS;
				pSplashBmpInfo->w	= INVALID_04_WIDTH;
				pSplashBmpInfo->h	= INVALID_04_HEIGHT;
				break;
			}

			time = pTimeSt->minute%10;

			if( time == 0 )
			{
				pSplashBmpInfo->offset	= TIME_MM_0_OFFSET;
				pSplashBmpInfo->size	= TIME_MM_0_SIZE;
				pSplashBmpInfo->xPos	= TIME_MM_0_X_POS;
				pSplashBmpInfo->yPos	= TIME_MM_0_Y_POS;
				pSplashBmpInfo->w	= TIME_MM_0_WIDTH;
				pSplashBmpInfo->h	= TIME_MM_0_HEIGHT;

			}
			else if( time == 1 )
			{
				pSplashBmpInfo->offset	= TIME_MM_1_OFFSET;
				pSplashBmpInfo->size	= TIME_MM_1_SIZE;
				pSplashBmpInfo->xPos	= TIME_MM_1_X_POS;
				pSplashBmpInfo->yPos	= TIME_MM_1_Y_POS;
				pSplashBmpInfo->w	= TIME_MM_1_WIDTH;
				pSplashBmpInfo->h	= TIME_MM_1_HEIGHT;
			}
			else if( time == 2 )
			{
				pSplashBmpInfo->offset	= TIME_MM_2_OFFSET;
				pSplashBmpInfo->size	= TIME_MM_2_SIZE;
				pSplashBmpInfo->xPos	= TIME_MM_2_X_POS;
				pSplashBmpInfo->yPos	= TIME_MM_2_Y_POS;
				pSplashBmpInfo->w	= TIME_MM_2_WIDTH;
				pSplashBmpInfo->h	= TIME_MM_2_HEIGHT;
			}
			else if( time == 3 )
			{
				pSplashBmpInfo->offset	= TIME_MM_3_OFFSET;
				pSplashBmpInfo->size	= TIME_MM_3_SIZE;
				pSplashBmpInfo->xPos	= TIME_MM_3_X_POS;
				pSplashBmpInfo->yPos	= TIME_MM_3_Y_POS;
				pSplashBmpInfo->w	= TIME_MM_3_WIDTH;
				pSplashBmpInfo->h	= TIME_MM_3_HEIGHT;
			}
			else if( time == 4 )
			{
				pSplashBmpInfo->offset	= TIME_MM_4_OFFSET;
				pSplashBmpInfo->size	= TIME_MM_4_SIZE;
				pSplashBmpInfo->xPos	= TIME_MM_4_X_POS;
				pSplashBmpInfo->yPos	= TIME_MM_4_Y_POS;
				pSplashBmpInfo->w	= TIME_MM_4_WIDTH;
				pSplashBmpInfo->h	= TIME_MM_4_HEIGHT;
			}
			else if( time == 5 )
			{
				pSplashBmpInfo->offset	= TIME_MM_5_OFFSET;
				pSplashBmpInfo->size	= TIME_MM_5_SIZE;
				pSplashBmpInfo->xPos	= TIME_MM_5_X_POS;
				pSplashBmpInfo->yPos	= TIME_MM_5_Y_POS;
				pSplashBmpInfo->w	= TIME_MM_5_WIDTH;
				pSplashBmpInfo->h	= TIME_MM_5_HEIGHT;
			}
			else if( time == 6 )
			{
				pSplashBmpInfo->offset	= TIME_MM_6_OFFSET;
				pSplashBmpInfo->size	= TIME_MM_6_SIZE;
				pSplashBmpInfo->xPos	= TIME_MM_6_X_POS;
				pSplashBmpInfo->yPos	= TIME_MM_6_Y_POS;
				pSplashBmpInfo->w	= TIME_MM_6_WIDTH;
				pSplashBmpInfo->h	= TIME_MM_6_HEIGHT;
			}
			else if( time == 7 )
			{
				pSplashBmpInfo->offset	= TIME_MM_7_OFFSET;
				pSplashBmpInfo->size	= TIME_MM_7_SIZE;
				pSplashBmpInfo->xPos	= TIME_MM_7_X_POS;
				pSplashBmpInfo->yPos	= TIME_MM_7_Y_POS;
				pSplashBmpInfo->w	= TIME_MM_7_WIDTH;
				pSplashBmpInfo->h	= TIME_MM_7_HEIGHT;
			}
			else if( time == 8 )
			{
				pSplashBmpInfo->offset	= TIME_MM_8_OFFSET;
				pSplashBmpInfo->size	= TIME_MM_8_SIZE;
				pSplashBmpInfo->xPos	= TIME_MM_8_X_POS;
				pSplashBmpInfo->yPos	= TIME_MM_8_Y_POS;
				pSplashBmpInfo->w	= TIME_MM_8_WIDTH;
				pSplashBmpInfo->h	= TIME_MM_8_HEIGHT;
			}
			else if( time == 9 )
			{
				pSplashBmpInfo->offset	= TIME_MM_9_OFFSET;
				pSplashBmpInfo->size	= TIME_MM_9_SIZE;
				pSplashBmpInfo->xPos	= TIME_MM_9_X_POS;
				pSplashBmpInfo->yPos	= TIME_MM_9_Y_POS;
				pSplashBmpInfo->w	= TIME_MM_9_WIDTH;
				pSplashBmpInfo->h	= TIME_MM_9_HEIGHT;
			}
			else
			{
				pSplashBmpInfo->offset	= TIME_MM_0_OFFSET;
				pSplashBmpInfo->size	= TIME_MM_0_SIZE;
				pSplashBmpInfo->xPos	= TIME_MM_0_X_POS;
				pSplashBmpInfo->yPos	= TIME_MM_0_Y_POS;
				pSplashBmpInfo->w	= TIME_MM_0_WIDTH;
				pSplashBmpInfo->h	= TIME_MM_0_HEIGHT;
			}
			break;

		default :
			pSplashBmpInfo->offset	= 0;
			pSplashBmpInfo->size	= 0;
			pSplashBmpInfo->xPos	= 0;
			pSplashBmpInfo->yPos	= 0;
	}

	return;
}
#endif

void Splash_set_frc_reset( FRC_RESET_T mode, uint8_t isHighActive )
{

}

void Splash_configure_gpio( void )
{
	uint32_t	lval, lval_dbg;
	int 		i;
	uint32_t	lTmp;
	uint32_t	lOpt;
	uint8_t 	u8HwOpt=0;

}

int Splash_copyimage( unsigned int loadaddress, unsigned int uncompaddress)
{
#define MAX_CPY_SZ				(7*1024*1024)
	struct partition_info	*mpi      = NULL;
	unsigned int				read_size;
	unsigned int				hdr_img_len;
	unsigned long				decomp_size;
	unsigned int				tick1, tick2;
	//	BMP_HEADER_INFO				bmpinfo ;
	unsigned char				*pCompData;
	int							ret;
	ulong	decomp_size1 = 0, decomp_size2 = 0;

	mpi = get_used_partition("logo");

	if (mpi == NULL)
	{
		printf("^R^failed to get partition free\n");
		return -1;
	}

	if (mpi->valid == NO) {
		printf("^R^Partition is Not valid! => Skip!");
		return -1;
	}

	if(!mpi->filesize) {
		printf("^g^File image size is Zero, Using partition size!!");
		read_size = mpi->size;
	}
	else
		read_size = mpi->filesize;

#if 1
#ifdef CC_EMMC_BOOT
	storage_read((ulong)mpi->offset, read_size, loadaddress);

#else
	ret = read_blocks_with_bg_task (mpi->offset, read_size, loadaddress, NULL);
#endif

	if (ret) {
		printf("block read failed..\n");
		return 1;
	}

	if (!lzo_compare_magic((unsigned char *)loadaddress)) //lzo
	{
		decomp_size1 = lzo_get_destsize((unsigned char *)loadaddress);
		if (decomp_size1 == 0) {
			printf("decompressed size is zero at LZO header\n");
			return 1;
		}

		printf("[%4d] [Lzo Img] Copy \"%s (%ld bytes)\" from 0x%08x to 0x%08x\n", readMsTicks(), mpi->name, read_size, (unsigned int)loadaddress, (unsigned int)uncompaddress);
		lzo_do_decomp((u_char *)uncompaddress, (u_char *)loadaddress, &decomp_size2);
		if (decomp_size2 != decomp_size1) {
			printf("decompressed size=%d is different with %d\n", (int)decomp_size2, (int)decomp_size1);
			return 1;
		}
		printf("[%4d] [Lzo Img] ... done\n", readMsTicks());

	}
	else //bmp
	{
		printf("^R^Invalid gzip header at %p -> change raw binary mode", loadaddress);
		memcpy((void*)uncompaddress, (void*)loadaddress, read_size);
	}

	return 0;

#else
printf("image copy start \n" );
#ifdef CC_EMMC_BOOT
	storage_read(mpi->offset, read_size, loadaddress);
#else
	ret = read_blocks_with_bg_task (mpi->offset, read_size, loadaddress, NULL);
#endif

	if (ret) {
		printf("block read failed..\n");
		return 1;
	}

printf("image copy end \n" );



	pCompData = (unsigned char *) loadaddress;


	printf("^R^Invalid gzip header at %p -> change raw binary mode", loadaddress);
	memcpy((void*)uncompaddress, (void*)loadaddress, read_size);

	return 0;
#endif
}

void Splash_PWM_Init( uint32_t WidthPortB,  PWM2_LVL_MODE_T mode)
{

}

UINT8	Splash_FRC_Reset( FRC_RESET_T mode )
{
	return 0;
}

/**
 * Set swing level
 *
 * @return void
 * @author Hong, Young Ho(longsky@lge.com)
 * @note	(from src/drvers/frc/ursa3/frc_ursa3.c)
 */

UINT8 Splash_FRC_Config(void)
{

	return 0;
}

int Splash_FRC_Freeze(void)
{
	return 0;
}

void Splash_FRC_Init( void )
{
}

UINT32 Splash_SetModelOption( UINT32 tModelOption )
{
	_gModelOptions = tModelOption | 0x80000000;
	return _gModelOptions;
}

UINT32 Splash_GetHWoption(HW_OPT_T option_mask)
{
	UINT32 ret=0;
	static int init = 0;

	return ret;
}


#define FRC_RESET_START	0
#define FRC_RSSET_END	1

extern u32 Get_DrawAddr(void);
extern UINT32 PmxDisplay(UINT32 u4Background, UINT8 colordepth, UINT8 epi,UINT8 frctype, UINT8 inch, UINT8 paneltype, UINT8 panelRes); //modified by LG
extern UINT32 PmxDisplay_PDP(UINT32 u4Background, UINT8 colordepth, UINT8 fhd); //modified by LG
extern const SYS_DB_T gSysNvmDB;
extern unsigned int readMsTicks(void);
extern void DDI_PWM_Init(PANEL_PWM_T panel_pwm);
extern int DDI_FRC_Reset(uint frctype, uint mode);

#define		IMAGE_OFFSET	0x500000



void BootSplash(void)
{
	static UINT32		_gTimeLvdsOut;
	static UINT32		_gTimeFRCreset;
	static UINT32		_gTimePwrOn;
	static UINT32		_gTimePwmInit;

	static PANEL_POWER_SEQ_T	pnlpwrseq;
	static PANEL_PWM_T       	pnlpwm;
	static UINT8              	systype = 0;
	static UINT8				bPWM_VSync_Enable;
	static UINT32           	nvmData;
	static UINT32				curTime;
	static UINT32				msDelay;
	static UINT8              	supportB0LVDSMAP;
	int							color_mode;
	int							led_bar_type;
	UINT8				inch;

	TOOL_OPTION1_T		toolOpt1;
	TOOL_OPTION2_T		toolOpt2;
	TOOL_OPTION3_T		toolOpt3;
	TOOL_OPTION4_T		toolOpt4;
	TOOL_OPTION5_T		toolOpt5;
	TOOL_OPTION6_T		toolOpt6;
	TOOL_OPTION7_T		toolOpt7;
	TOOL_OPTION8_T		toolOpt8;

	static unsigned int	_loadAddr;
	static unsigned int _uncompAddr;

	//return 0;
	//	_loadAddr  = Get_DrawAddr();
	//	_uncompAddr = _loadAddr + IMAGE_OFFSET;

	_uncompAddr = Get_DrawAddr();
	_loadAddr	= _uncompAddr - IMAGE_OFFSET;

	DDI_NVM_GetToolOpt1(&toolOpt1);
	DDI_NVM_GetToolOpt2(&toolOpt2);
	DDI_NVM_GetToolOpt3(&toolOpt3);
	DDI_NVM_GetToolOpt4(&toolOpt4);
	DDI_NVM_GetToolOpt5(&toolOpt5);
	DDI_NVM_GetToolOpt6(&toolOpt6);
	DDI_NVM_GetToolOpt7(&toolOpt7);

	Splash_GetPanelInfo(&toolOpt1);

	gToolOpt[0] = toolOpt1.all;
	gToolOpt[1] = toolOpt2.all;
	gToolOpt[2] = toolOpt3.all;
	gToolOpt[3] = toolOpt4.all;
	gToolOpt[4] = toolOpt5.all;
	gToolOpt[5] = toolOpt6.all;
	gToolOpt[6] = toolOpt7.all;

	//should be controlled by NVM
	//but I found that: tool1 could be controlled by luna command, but what we get in loader is a different loader. It seems we need to sync code from LG in order to have the same data base
	//    printf ("force 10bit\n");
	//    toolOpt1.flags.nLVDSBit=1;//10bit

#define	OSA_MD_GetModuleMakerType()		(toolOpt1.flags.eModelModuleType)
#define	OSA_MD_IsSupportMirrorMode()	(toolOpt5.flags.bMirrorMode)
#define	OSA_MD_GetModuleInch()			(toolOpt1.flags.eModelInchType)

	inch = OSA_MD_GetModuleInch();
	//#define OSA_MD_GetToolType()			(toolOpt1.flags.eModelToolType)

	/*-----------------------------------------------
	 * Read NVM data FOR power sequence data
	 *-----------------------------------------------*/
	DDI_NVM_Read(SYS_DB_BASE + (UINT32)&(gSysNvmDB.validMark) - (UINT32)&gSysNvmDB,	\
			sizeof(gSysNvmDB.validMark), (UINT8 *)&(nvmData));
	if(nvmData != 0xffffffff)
	{
		DDI_NVM_Read(SYS_DB_BASE + (UINT32)&(gSysNvmDB.panelpowerseq) - (UINT32)&gSysNvmDB,	\
				sizeof(gSysNvmDB.panelpowerseq), (UINT8 *)&(pnlpwrseq));
		DDI_NVM_Read(SYS_DB_BASE + (UINT32)&(gSysNvmDB.panelpwm) - (UINT32)&gSysNvmDB,	\
				sizeof(gSysNvmDB.panelpwm), (UINT8 *)&(pnlpwm));
		DDI_NVM_Read(SYS_DB_BASE + (UINT32)&(gSysNvmDB.systemtype) - (UINT32)&gSysNvmDB,	\
				sizeof(gSysNvmDB.systemtype), (UINT8 *)&(systype));
		DDI_NVM_Read(SYS_DB_BASE + (UINT32)&(gSysNvmDB.ColorDepth) - (UINT32)&gSysNvmDB,	\
				sizeof(gSysNvmDB.ColorDepth), (UINT8 *)&(gSysNvmDB.ColorDepth));
	}
	else
	{
		pnlpwrseq = gSysNvmDB.panelpowerseq;
		pnlpwm = gSysNvmDB.panelpwm;
	}

	printf("[%d]:SPDIF DETECT ENABLE sound out mode =%d \n",readMsTicks(),DDI_NVM_GetSoundOutMode);
	if(DDI_NVM_GetSoundOutMode() ==SOUNDOUT_OPTICAL_SOUNDBAR)
	{
		vIO32Write4B(0xf0005030,(u4IO32Read4B(0xf0005030) & 0xFFFFFFDF));
		vIO32WriteFldAlign(CKGEN_PMUX0, 1, FLD_PAD_ASPDIF0);
		vIO32WriteFldAlign(CKGEN_PMUX0, 1, FLD_PAD_ASPDIF1);
	}
	printf("panel pwm = 48hz=%d, 50hz=%d, 60hz=%d, vbrB duty=%d \n", pnlpwm.vbrBFreq48hz, pnlpwm.vbrBFreq50hz, pnlpwm.vbrBFreq60hz, pnlpwm.vbrBMaxDuty);
	printf("gSysNvmDB.ColorDepth = %d\n", gSysNvmDB.ColorDepth);
	printf("gSysNvmDB.systemtype = %d\n", systype);
	printf("bPWM_VSync_Enable = %d\n",pnlpwm.config&PWM_VRST_EN);
	printf("bPWM led current = %d, freq=%d\n",pnlpwm.vbrCLedCurrent, pnlpwm.vbrCFreq);
	printf("pnlpwrseq.panelPowOnToData = %d\n", pnlpwrseq.panelPowOnToData);
	printf("pnlpwrseq.dataToLampOn = %d\n", pnlpwrseq.dataToLampOn * 10);
	printf("pnlpwrseq.lampOffToData = %d\n", pnlpwrseq.lampOffToData * 10);
	printf("pnlpwrseq.dataToPanelPowOff = %d\n", pnlpwrseq.dataToPanelPowOff);
	printf("pnlpwrseq.nRLOntoDimSig = %d\n", pnlpwrseq.nRLOntoDimSig*10);


	printf("[ToolOpt1] = %d\n",toolOpt1.all);
	printf("[ToolOpt2] = %d\n",toolOpt2.all);
	printf("[ToolOpt3] = %d\n",toolOpt3.all);
	printf("[ToolOpt4] = %d\n",toolOpt4.all);
	printf("[ToolOpt5] = %d\n",toolOpt5.all);
	printf("[ToolOpt6] = %d\n",toolOpt6.all);
	printf("[ToolOpt7] = %d\n",toolOpt7.all);

	/* Panel On */
	if(!(inch == INCH_22 || inch == INCH_23 || inch == INCH_24 || inch == INCH_26 || inch == INCH_27)) // MTV model 이 아닌 경우에만 
	{
		printf("[%d]:PANEL ON \n",readMsTicks());
		Splash_MICOM_PanelOn();
		msDelay = (UINT32)(pnlpwrseq.panelPowOnToData);
		udelay(msDelay*1000);
	}
	printf("[%d]:FRC RESET START \n",readMsTicks());
	DDI_FRC_Reset(Get_modelOpt(FRC_OPT_SEL) ,0);//FRC_RESET_START);

	_gTimePwrOn = readMsTicks();

#if 1
	printf("_loadAddr = 0x%x, _uncompAddr = 0x%x \n",_loadAddr,_uncompAddr);
	printf("[%d]:COPY LOGO IMAGE \n",readMsTicks());
	printf("open splash_copyimage");

	Splash_copyimage(_loadAddr, _uncompAddr);
	printf("[%d]:COPY LOGO IMAGE end\n",readMsTicks());
#endif

	// 50inch AUO module T8 spec
	if( (OSA_MD_GetModuleMakerType() == MODULE_AUO) && (OSA_MD_GetModuleInch() == INCH_50) )
		DDI_SetLocalDimming_OS_Panel(1);
	/* LD SPI */
	if(toolOpt3.flags.bLocalDimming == 0)
		printf("[%d]:Not support Local dimming \n",readMsTicks());

	/* PWM	*/
	// 60inch sharp module T2 spec max 20ms 이므로 delay 아래 조건에서 제외시킴 

	curTime = readMsTicks();

	DDI_PWM_Init(pnlpwm);
	printf("[%d]:PWM1/2 ON \n", readMsTicks());

	/* LVDS */
	curTime = readMsTicks();

	printf("[%d]:SPDIF DETECT DISABLE \n",readMsTicks());
	if(DDI_NVM_GetSoundOutMode() ==SOUNDOUT_OPTICAL_SOUNDBAR)
	{
		vIO32Write4B(0xf0005030,(u4IO32Read4B(0xf0005030) | 0x20));
		vIO32WriteFldAlign(CKGEN_PMUX0, 0, FLD_PAD_ASPDIF0);
		vIO32WriteFldAlign(CKGEN_PMUX0, 0, FLD_PAD_ASPDIF1);
	}

	printf("[%d]:FRC RESET END \n",readMsTicks());
	DDI_FRC_Reset(Get_modelOpt(FRC_OPT_SEL) ,1);//FRC_RESET_END);

	printf("[%d]:LVDS OUT \n",readMsTicks());
	if(toolOpt1.flags.nLVDSBit)
		printf("Color Depth 10bit \n");
	else
		printf("Color Depth 8bit \n");

	if(Get_modelOpt(DISPLAY_TYPE_OPT_SEL) == 2) // PDP
	{
		if(Get_modelOpt(PANEL_RES_OPT_SEL) == 1) // FHD
		{
			PmxDisplay_PDP(0x212121, toolOpt1.flags.nLVDSBit, 1);
		}
		else // HD
		{
			PmxDisplay_PDP(0x212121, toolOpt1.flags.nLVDSBit, 0);
		}
	}
	else // LCD
	{
		PmxDisplay(0x212121, toolOpt1.flags.nLVDSBit, Get_modelOpt(EPI_OPT_SEL), Get_modelOpt(FRC_OPT_SEL), OSA_MD_GetModuleInch(), Get_modelOpt(PANEL_TYPE_OPT_SEL), Get_modelOpt(PANEL_RES_OPT_SEL));
	}
	_gTimeLvdsOut = readMsTicks();
#if 0
	printf("_loadAddr = 0x%x, _uncompAddr = 0x%x \n",_loadAddr,_uncompAddr);
	printf("[%d]:COPY LOGO IMAGE \n",readMsTicks());
	printf("open splash_copyimage");

	Splash_copyimage(_loadAddr, _uncompAddr);
	printf("[%d]:COPY LOGO IMAGE end\n",readMsTicks());
#endif

	if( Get_modelOpt(EPI_OPT_SEL) == 1 )
	{
		if( OSA_MD_IsSupportMirrorMode() )
		{
			printf("\n Mirror mode for EPI \n");
			u1SetFlipMirrorConfig(TRUE, FALSE);
		}
		else
		{
			printf("\n Normal mode for EPI \n");
			u1SetFlipMirrorConfig(FALSE, TRUE);
		}
	}
	else
	{
		if( OSA_MD_IsSupportMirrorMode() )
		{
			printf("\n Mirror mode for LVDS \n");
			u1SetFlipMirrorConfig(TRUE, TRUE);
		}
	}

	printf("[%d]:DRAW LOGO \n",readMsTicks());

	printf("open splash_copyimage");
	Splash_DrawLogoImage(_uncompAddr, systype);
	printf("[%d]:DRAW LOGO end \n",readMsTicks());
	msDelay = pnlpwrseq.dataToLampOn * 10;
	udelay(msDelay * 1000);

	printf("[%d]:INV ON \n",readMsTicks());
	Splash_MICOM_InvOn();
	__InvertOn = 1;
	printf("set __InvertOn : %d \n", __InvertOn);
}

