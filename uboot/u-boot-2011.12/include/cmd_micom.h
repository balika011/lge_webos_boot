#ifndef _CMD_MICOM_H_
#define _CMD_MICOM_H_

/**
 *	Time structure type definitions
 *
 *	\- year		: 1970 ~ 65535 <P>
 *	\- month	: 1 ~ 12 <P>
 *	\- day		: 1 ~ 31 <P>
 *	\- hour		: 0 ~ 23 <P>
 *	\- minute	: 0 ~ 59 <P>
 *	\- second	: 0 ~ 59 <P>
 */
typedef	struct
{
	uint16_t		year;
	uint8_t			month;
	uint8_t 		day;
	uint8_t 		hour;
	uint8_t 		minute;
	uint8_t 		second;
}	TIME_T;

typedef enum
{
	MICOM_PDP_SATURN_BOARD 				= 0,		/* PDP S7 Board NEC 0513A 32KB Flash */
	MICOM_PDP_BCM_BOARD 				= 1,		/* PDP BCM Board NEC 0513A 32KB Flash */
	MICOM_LCD_SATURN_BOARD				= 2,		/* LCD S7 Board NEC 0513A 32KB Flash */
	MICOM_LCD_BCM_BOARD_NEC32KB			= 3, 		/* LCD BCM Board NEC 0513A 32KB Flash */
	MICOM_LCD_BCM_BOARD_NEC48KB			= 4,		/* LCD BCM Board NEC 0514A 48KB Flash */
	MICOM_LCD_BCM_BOARD_SANYO48KB 		= 5,
} 	MICOM_IMAGE_TYPE_T;
/* 2011.09.04 zaewon.lee : end */


void micom_init(void);
int micom_read_command(uint8_t cmd, uint8_t* data, int len);
int micom_write_command(uint8_t cmd, uint8_t* data, int len);
int micom_write(uint8_t* data, int len);

extern int MICOM_IsPowerOnly(void);
extern int MICOM_GetHWoption(unsigned char *data);
extern int MICOM_VerifyInvertOn(unsigned char *pOnoff);
extern int MICOM_TurnOnInv(void);
extern int MICOM_VerifyPanelOn(unsigned char *pOnoff);
extern int MICOM_TurnOnPanel(void);

/* 2011.09.04 zaewon.lee : add for MICOM image downloading */
unsigned char SWU_MICOM_BinUpdate(char* pImg_start, u32 size);
/* 2011.09.04 zaewon.lee : end */

#endif /* _CMD_MICOM_H_ */
