/******************************************************************************

	LGE. DTV RESEARCH LABORATORY
	COPYRIGHT(c) LGE CO.,LTD. 2009. SEOUL, KOREA.
	All rights are reserved.

******************************************************************************/

/** @file cfe_splash.h
 *
 *	This c file defines the DDI functions related to SPLASH(LOGO)
 *
 *	@author 	dreamer@lge.com
 *	@version	0.1
 *	@date		2009.10.29
 *	@note
 *	@see
 */

#ifndef _CFE_SPLASH_H_
#define _CFE_SPLASH_H_

/*********************************************************************
	매크로 정의(Macro Definitions)
**********************************************************************/

/******************************************************************************
	상수 정의(Constant Definitions)
******************************************************************************/


/******************************************************************************
    형 정의 (Type Definitions)
******************************************************************************/

typedef enum
{
	SPLASH_INIT,
	SPLASH_START,
	SPLASH_TRY2END0,
	SPLASH_TRY2END,
	SPLASH_END
}	SPLASH_MODE_T;

/* PANEL_TYPE_OPT_SEL, CPBOX_OPT_SEL, EPI_OPT_SEL */
typedef enum
{
	FRC_OPT_SEL,
	PANEL_RES_OPT_SEL,
	DISP_OPT_SEL,
	PANEL_TYPE_OPT_SEL,
	DDRSPEED_OPT_SEL,
	CPBOX_OPT_SEL,
	T2_TUN_OPT_SEL,
	S_TUN_OPT_SEL,
	EPI_OPT_SEL,
	PHM_OPT_SEL,
	DISPLAY_TYPE_OPT_SEL,
}	HW_OPT_T;

/* enum for HardWare Display Option */
typedef enum
{
	LCD = 0,
	OLED,
	PLASMA,
	UNKNOWN,
}	DISPLAY_TYPE_T;

typedef enum
{
	FRC_RESET_INIT	= 0,
	FRC_RESET_START,
	FRC_RESET_END

} 	FRC_RESET_T;

typedef enum
{
	PWM2_LVL_ZERO,
	PWM2_LVL_HALF,
	PWM2_LVL_FULL,
} PWM2_LVL_MODE_T;


typedef enum
{
/*
	OPT0	OPT1
	LOW		LOW			: NO FRC        		(0)
	LOW		HIGH		: BCM internal FRC		(1)
	HIGH	LOW			: LG FRC-II			(2)
	HIGH	HIGH 		: External URSA5		(3)
*/
	FRC_OPT_NO_FRC,
	FRC_OPT_BCM_INTERNAL,
	FRC_OPT_LG_FRC2,
	FRC_OPT_URSA5_EXTERNAL

}	FRC_OPT_T;

typedef enum
{
	SOUNDOUT_TVSPEAKER = 0,
	SOUNDOUT_EXTERNALSPEAKER,
	SOUNDOUT_OPTICAL_SOUNDBAR,
	SOUNDOUT_HEADPHONE,
	SOUNDOUT_LINEOUT,
	SOUNDOUT_BLUETOOTH_SOUNDBAR,
} SOUNDOUT_MODE_T;

/*********************************************************************
	Defines
**********************************************************************/

/*********************************************************************
	Function Prototypes
**********************************************************************/
extern int  _splashMode;
extern void BootSplash(void);
extern void BootSplash_PDP(void);

#endif  /* End of _CFE_SPLASH_H_ */
