
/******************************************************************************
 *   DTV LABORATORY, LG ELECTRONICS INC., SEOUL, KOREA
 *   Copyright(c) 2009 by LG Electronics Inc.
 *
 *   All rights reserved. No part of this work may be reproduced, stored in a
 *   retrieval system, or transmitted by any means without prior written
 *   permission of LG Electronics Inc.
 *****************************************************************************/

/** @file cfe_nvm.c
 *
 *  Micom Task.
 *
 *  @author		dreamer@lge.com
 *  @version	1.0
 *  @date    	2009.10.08
 *  @note		copy from S7's cmd_nvm.c
 */

/*---------------------------------------------------------
    #include 파일들
    (File Inclusions)
---------------------------------------------------------*/

#include <malloc.h>
#include <common.h>
#include <command.h>
#include <asm/types.h>
#include <cmnio_type.h> // nvmdb
#include <partinfo.h>
#include <nand.h>
#include <lg_modeldef.h>
#include <linux/types.h>
#include <linux/string.h>
#include <linux/ctype.h>
#include <cmnio_type.h>
#include <mt5882_lgcmnio.h>

/*---------------------------------------------------------
    Control 상수 정의
    (Control Constants)
---------------------------------------------------------*/

#define	_INCLUDE_READ_OPTS_AT_ONCE

#define	DEBUG_INVALID_COMMAND	(0xFFFFFFFFUL)

#define DEFAULT_PRINT_MASK		{					\
		0,											\
		{ {{0,},0}, {{0,},0}, {{0,},0}, {{0,},0},	\
		  {{0,},0}, {{0,},0}, {{0,},0}, {{0,},0}	\
		},											\
}


/*---------------------------------------------------------
    매크로 함수 정의 (Macro Definitions)
---------------------------------------------------------*/

#define OSA_DBG_PrintMenu	printf
#define rprint1n			printf


#undef DEBUG
/* note: prints function name for you */
#ifdef DEBUG
#define dprintf(fmt, args...)	printf("%s: " fmt, __FUNCTION__, ## args)
#define DEBUGF(fmt, args...)	printf(fmt ,##args)
#else
#define dprintf(fmt, args...)
#define DEBUGF(fmt, args...)
#endif	/* DEBUG */

/*---------------------------------------------------------
    Local 변수 및 함수 Prototype 정의
    (Local Variables & Function Prototypes Declarations)
---------------------------------------------------------*/

/* comming from nvmdb_ddi.c */
/** System DB Default Value 설정 및 global 변수 선언 */
const SYS_DB_T gSysNvmDB =
{
	0x44eb4cca,
	DEFAULT_PRINT_MASK,
	0,		/* PDP panel use time */
	0,		/* FRC auto download or not */
	{50, 57, 33, 50, 50, 50}, /* panel powerseq */
	{ 0, 0, 80,  5, 99, 99,  5, 35, 96, 120, 100, 22, 200,  0x0C }, /* panel pwm */
#if 0 //(SYS_ATSC)
	0,      /*system type 0:atsc, 1: dvb*/
#else
	1,      /*system type 0:atsc, 1: dvb*/
#endif
    1,      /**COLOR_DEPTH_T */
    1,      /**LVDS_PIXEL_T */
    0,		/*Vcom Pgamma value checksum */
	RELEASE_LEVEL,
	0xFF,   /* SoftwareUpdate Mode */
	0xFF,   /* Make Hibernation Mode */
	0xFF,	 /* Snap Shot Support or not */
	1,		/* ADC Mode [L9] Internal */
	0xFFFFFFFF, /* SWU Mode Magic Key */
	0,	/* sound out mode  */
	0xFFFFFFFF,	/* zram */
	0,			/* eMMCUsage */
	0,			/* bDPortEnable */
	0xff,		/* Secureboot full verify flag */
	0,			/* PWM Mode */
	0,			/* timeDifference */
	0xFF,		/* Instant boot support or not */
	0,		/* Snapshot boot ART support or not */
};


const TOOLOPTION_DB_T gToolOptionDB ={
	0x20130002,
	{INCH_47|(TOOL_LB67<<5)|(MODULE_LGD<<11)|(1<<15)},		/* Tool Option 1 */
	{0},		/* Tool Option 2 */
	{0},		/* Tool Option 3 */
	{0},		/* Tool Option 4 */
	{0},		/* Tool Option 5 */
	{0},		/* Tool Option 6 */
	{0},		/* Tool Option 7 */
	{0},		/* Tool Option 8 */
	{0},		/* Analog Area Option */
	{0},		/* EU Area Option */
	{2},		/* stExtVideoInputADJ */
	{1},			/* stExtAudioInputADJ */
};


MODEL_INFO_DB_T gModelInfoDB = {
	0x20130002,				/* validMark */
	{'W', 'E', 'B', 'O' ,'S' ,'2', '\0',},	/* aModelName[13] */
	{'S','K','J','Y','1','1','0','7',},		/* aSerialNum[BAR_CODE_SIZE_MAX] */
	0,		/* group_code */
	0,		/* country_code */
	0,		/* city_code */
};


const PRESERVE_DB_T gPreserveDB = {
	0x20130001,				/* validMark */
	0,						/* bInstopReserved */
	0,						/* bInstopCompleted */
	BAUDRATE_115200,		/* eBaudrate - 115200 */
	0,						/* b1stBootAfterInstop */
	0,						/* b1stBootAfterFactoryReset */
};


MAC_ADDRESS_T gMacAddress = {
	0x20130001,
	{'F','F',':','F','F',':','F','F',':','F','F',':','F','F',':','F','F'}
};

#define NVM_PAGE_SIZE				32


#define NVM_I2C_CH					0
#define MICOM_I2C_CH				1

#define NVM_I2C_ADDR				0xA0

extern int storage_erase(unsigned long long offset, unsigned long long len);

UINT8 DDI_NVM_Write (UINT32 offset, UINT32 nData, UINT8 *pTxBuf)
{
	UINT8 subAddr[2];
	int i = 0;
	int tStartIndex, tLastIndex;
	int tLast = offset + nData -1;
	int tStartOfs, tLength;
	int cacheOfs;
	char *pCacheBuf;  // 나중에 구현..

	subAddr[0] = offset >> 8;
	subAddr[1] = offset & 0xff;

	tStartIndex = offset / NVM_PAGE_SIZE;
	tLastIndex = tLast / NVM_PAGE_SIZE;

	for(i=tStartIndex; i<= tLastIndex; i++ )
	{
		UINT8 stOfs[2];

		tStartOfs  = ( (i == tStartIndex) ? (offset  ) : (   i  * NVM_PAGE_SIZE) );
	    tLength    = ( (i == tLastIndex ) ? (tLast+1) : ((i+1) * NVM_PAGE_SIZE) ) - tStartOfs;

		cacheOfs = i * NVM_PAGE_SIZE;

		stOfs[0] = tStartOfs >> 8;
		stOfs[1] = tStartOfs & 0xff;

		if( DDI_CMNIO_I2C_Write(NVM_I2C_CH, 100, NVM_I2C_ADDR, 2, stOfs, tLength, pTxBuf, 3) == -1)
			return NOT_OK;

		udelay(5000);
	}

	return OK;
}


UINT8 DDI_NVM_Read (UINT32 offset, UINT32 nData, UINT8 *pTxBuf)
{
	UINT8 subAddr[2];
	int tLength = nData;

	subAddr[0] = offset >> 8;
	subAddr[1] = offset & 0xff;

	if( DDI_CMNIO_I2C_Read(NVM_I2C_CH, 100, NVM_I2C_ADDR, 2, subAddr, tLength, pTxBuf, 3) == -1)
		return NOT_OK;

	return OK;
}

int	DDI_NVM_GetDebugStatus( void )
{
	static UINT8	debugStatus = (UINT8) -1;
	UINT32		validMark;
	/*
	 * 2011.11.28 zaewon.lee
	 * It is better let nDebugStatus DEBUG_LEVEL until DV event finish.
	 * It can be very helpful when user want to change event mode to debug mode.
	 */
	//UINT8		nDebugStatus = DEBUG_LEVEL;
	UINT8		nDebugStatus = RELEASE_LEVEL;

	if( debugStatus == (UINT8) -1 )
	{
		DDI_NVM_Read(SYS_DB_BASE + (UINT32)&(gSysNvmDB.validMark) - (UINT32)&gSysNvmDB,	\
				sizeof(gSysNvmDB.validMark), (UINT8 *)&(validMark));
		if(validMark != 0xffffffff)
		{
	//		printf("*** valid mark is 0x%x\n",validMark);
			DDI_NVM_Read(SYS_DB_BASE + (UINT32)&(gSysNvmDB.nDebugStatus) - (UINT32)&gSysNvmDB,	\
					sizeof(gSysNvmDB.nDebugStatus), (UINT8 *)&(nDebugStatus));
	//		printf("*** DebugStatus : %d\n", nDebugStatus);
			if( nDebugStatus < DEBUG_LEVEL || nDebugStatus > RELEASE_LEVEL)
				debugStatus = RELEASE_LEVEL;
			else
				debugStatus = nDebugStatus;
		}
		else
		{
	//		printf("*** valid mark is 0xffffffff\n");
			debugStatus = nDebugStatus;
		}
	}

	rprint1n("*** Final debugStatus %d(%x)\n", debugStatus, validMark);

	return	debugStatus;
}


#if 0
#define MAGIC_FULL_VERIFY 0X78
int	DDI_NVM_GetOTPStatus( void )
{
	static UINT8	nvm_fullVerify = 0xff;
	UINT32		validMark;
	UINT8		nfullVerify = 0;


	if( nvm_fullVerify == 0xff )
	{
		DDI_NVM_Read(SYS_DB_BASE + (UINT32)&(gSysNvmDB.validMark) - (UINT32)&gSysNvmDB,	\
				sizeof(gSysNvmDB.validMark), (UINT8 *)&(validMark));
		if(validMark != 0xffffffff)
		{
	//		printf("*** valid mark is 0x%x\n",validMark);
			DDI_NVM_Read(SYS_DB_BASE + (UINT32)&(gSysNvmDB.fullVerify) - (UINT32)&gSysNvmDB,	\
					sizeof(gSysNvmDB.fullVerify), (UINT8 *)&(nfullVerify));
			if((nfullVerify & 0xff) == MAGIC_FULL_VERIFY)
				{
					nvm_fullVerify=0xff;
				}
			else
				{
					nvm_fullVerify=0x0;
				}
		}
		else
		{
	//		printf("*** valid mark is 0xffffffff\n");
		}
	}

	rprint1n("*** Final fullVerify value %d(%x)\n", nvm_fullVerify, validMark);

	return	nvm_fullVerify;
}

void DDI_NVM_SetOTPStatus( UINT8 nfullVerify )
{
	UINT32 nvm_support_snapshot = SYS_DB_BASE + (UINT32)&(gSysNvmDB.fullVerify) - (UINT32)&gSysNvmDB;
	nfullVerify = (nfullVerify & 0xff) ? MAGIC_FULL_VERIFY : 0xff;
	DDI_NVM_Write(nvm_support_snapshot, sizeof(gSysNvmDB.fullVerify), &nfullVerify);
}
#endif


#define NVM_PRESERVE_DB_LOAD_ITEM(item, pValue)   \
		(DDI_NVM_Read (PRESERVE_DB_BASE + (UINT32)&(gPreserveDB.item) - (UINT32)&gPreserveDB,	\
											sizeof(gPreserveDB.item), (UINT8 *)(pValue)))

int	DDI_NVM_GetInstopStatus( void )
{
	static UINT8	instopStatus = 0xff;
	UINT32		_validMark;
	UINT8		nInstopStatus = 0; //default : not instop mode

	printf(" *** %s start **** \n",__func__);
	if( instopStatus == 0xff )
	{
		NVM_PRESERVE_DB_LOAD_ITEM(validMark, &_validMark);

		if(_validMark != 0xffffffff)
		{
			printf("*** instop valid mark is 0x%x\n",_validMark);
			NVM_PRESERVE_DB_LOAD_ITEM(bInstopCompleted, &nInstopStatus);
			printf("*** Instop Status : %d\n", nInstopStatus);
			instopStatus = nInstopStatus;
		}
		else
		{
			printf("*** instop valid mark is 0xffffffff\n");
			instopStatus = nInstopStatus;
		}
	}
	rprint1n("*** Final instopStatus %d(%x)\n", instopStatus, _validMark);

	return	instopStatus;
}



static void _NVM_PrintMap(void)
{
/**
 * NVM DB MAP
 */
	struct DB_MAPS_T {
		char	*db_name;
		int		db_base;
		int		db_size;
	};

	struct DB_MAPS_T basicDbMaps[] =
	{
		{ "NVM header"		, NVM_HEADER_BASE			, NVM_HEADER_SIZE			},
		{ "TNVM_MAGIC"		, TNVM_MAGIC_BASE			, TNVM_MAGIC_SIZE			},
		{ "System Data"		, SYS_DB_BASE				, SYS_DB_SIZE				},
		{ "Factory DB" 		, FACTORY_DB_BASE			, FACTORY_DB_SIZE			},
		{ "Tool Option" 		, TOOL_OPTION_DB_BASE		, TOOL_OPTION_DB_SIZE		},
		{ "Analog"				, ANA_DB_BASE				, ANA_DB_SIZE				},
		{ "SUMODE WB"		, SUMODE_10POINTWB_DB_BASE	, SUMODE_10POINTWB_DB_SIZE	},
		{ "ModelInfo DB"	, MODEL_INFO_DB_BASE		, MODEL_INFO_DB_SIZE		},
		{ "Preserve DB" 	, PRESERVE_DB_BASE			, PRESERVE_DB_SIZE			},
		{ "EMP(For DRM)"	, EMP_DB_BASE				, EMP_DB_SIZE				},
		{ "SWU,SSU,NSU"		, SWU_DB_BASE				, SWU_DB_SIZE				},
		{ "MICOM"			, MICOM_DB_BASE				, MICOM_DB_SIZE				},
		{ "THX DB"			, THX_DB_BASE				, THX_DB_SIZE				},
		{ "NEW_FACTORY_DB"	, NEW_FACTORY_DB_BASE		, NEW_FACTORY_DB_SIZE		},
		{ "MAC ADDRESS DB"	, MAC_ADDRESS_BASE			, MAC_ADDRESS_SIZE			},
		{ "PWRON_LASTINFO_DB" , LAST_INPUT_INFO_DB_BASE	, LAST_INPUT_INFO_DB_SIZE	},
		{ "CONTINENT_OPTION_DB" , CONTINENT_OPTION_DB_BASE	, CONTINENT_OPTION_DB_SIZE	},
		{ "TMP_UI_DB"		, TMP_UI_DB_BASE			, TMP_UI_DB_SIZE			},
		{ "SOC VENDOR DB"	, SOC_VENDOR_BASE			, SOC_VENDOR_SIZE			},
	};

	int	i;

	printf(" Basic DB size(%4d/0x%04x)\n\n", BASIC_DB_SIZE, BASIC_DB_SIZE);
	for (i = 0; i < (sizeof(basicDbMaps)/sizeof(struct DB_MAPS_T)); i++)
	{
		printf("    %-12s: offset(0x%04x), size(%5d/0x%04x)\n",
				basicDbMaps[i].db_name, basicDbMaps[i].db_base, basicDbMaps[i].db_size, basicDbMaps[i].db_size);
	}

	printf("==========================================\n");
}


#ifdef	_INCLUDE_READ_OPTS_AT_ONCE
static TOOLOPTION_DB_T * _NVM_GetToolOpts( void )
{
	static TOOLOPTION_DB_T _gToolOptionDB = {0xFFFFFFFF,0,};

#define	_SIZE_OF_OPTS	(	sizeof(gToolOptionDB.validMark)		\
						+	sizeof(gToolOptionDB.nToolOption1)	\
						+	sizeof(gToolOptionDB.nToolOption2)	\
						+	sizeof(gToolOptionDB.nToolOption3)	\
						+	sizeof(gToolOptionDB.nToolOption4)	\
						+	sizeof(gToolOptionDB.nToolOption5)	\
						+	sizeof(gToolOptionDB.nToolOption6)	\
						+	sizeof(gToolOptionDB.nToolOption7)	)

	if( _gToolOptionDB.validMark == 0xFFFFFFFF )
	{
		DDI_NVM_Read(TOOL_OPTION_DB_BASE + (UINT32)&(gToolOptionDB.validMark) - (UINT32)&gToolOptionDB,
				_SIZE_OF_OPTS, (UINT8 *)&(_gToolOptionDB));

		if( _gToolOptionDB.validMark == 0xFFFFFFFF )
		{
			_gToolOptionDB = gToolOptionDB;
		}
	}

	return &_gToolOptionDB;
}
#endif

static SYS_DB_T * _NVM_GetSysDb(void)
{
	static SYS_DB_T _sysdb = {0xFFFFFFFF,0,};

	if( _sysdb.validMark == 0xFFFFFFFF )
	{
		DDI_NVM_Read(SYS_DB_BASE + (UINT32)&(gSysNvmDB.validMark) - (UINT32)&gSysNvmDB,	\
						sizeof(gSysNvmDB), (UINT8 *)&(_sysdb));

		if( _sysdb.validMark == 0xFFFFFFFF )
		{
			_sysdb = gSysNvmDB;
		}
	}

	return &_sysdb;
}

int DDI_NVM_GetToolOpt1(TOOL_OPTION1_T *pOpt1)
{
	TOOLOPTION_DB_T *	pToolOptionDB = _NVM_GetToolOpts();

	*pOpt1 = pToolOptionDB->nToolOption1;

	return 0;
}

int DDI_NVM_GetToolOpt2(TOOL_OPTION2_T *pOpt2)
{
	TOOLOPTION_DB_T *	pToolOptionDB = _NVM_GetToolOpts();

	*pOpt2 = pToolOptionDB->nToolOption2;

	return 0;
}

int DDI_NVM_GetToolOpt3(TOOL_OPTION3_T *pOpt3)
{
	TOOLOPTION_DB_T *	pToolOptionDB = _NVM_GetToolOpts();

	*pOpt3 = pToolOptionDB->nToolOption3;

	return 0;
}

int DDI_NVM_GetToolOpt4(TOOL_OPTION4_T *pOpt4)
{
	TOOLOPTION_DB_T *	pToolOptionDB = _NVM_GetToolOpts();

	*pOpt4 = pToolOptionDB->nToolOption4;

	return 0;
}

int DDI_NVM_GetToolOpt5(TOOL_OPTION5_T *pOpt5)
{
	TOOLOPTION_DB_T *	pToolOptionDB = _NVM_GetToolOpts();

	*pOpt5 = pToolOptionDB->nToolOption5;

	return 0;
}

int DDI_NVM_GetToolOpt6(TOOL_OPTION6_T *pOpt6)
{
	TOOLOPTION_DB_T *	pToolOptionDB = _NVM_GetToolOpts();

	*pOpt6 = pToolOptionDB->nToolOption6;

	return 0;
}

int DDI_NVM_GetToolOpt7(TOOL_OPTION7_T *pOpt7)
{
	TOOLOPTION_DB_T *	pToolOptionDB = _NVM_GetToolOpts();

	*pOpt7 = pToolOptionDB->nToolOption7;

	return 0;
}

int DDI_NVM_GetToolOpt8(TOOL_OPTION8_T *pOpt8)
{
	TOOLOPTION_DB_T *	pToolOptionDB = _NVM_GetToolOpts();

	*pOpt8 = pToolOptionDB->nToolOption8;

	return 0;
}


int DDI_NVM_GetSysDb(SYS_DB_T *sysdb)
{
	sysdb = _NVM_GetSysDb();

	return 0;

}

#ifdef CFG_LG_CHG
extern int gets(char *s);
#endif

#define ASCII_0	0x30
#define ASCII_9	0x39
#define ASCII_S_A	0x61
#define ASCII_S_F	0x66
#define ASCII_L_A	0x41
#define ASCII_L_F	0x46

UINT32  OSA_DBG_GetHexInput(const char *pStr, unsigned int *pVal)
{
	char rbuff[32] = {0,};
	int i=0;
	char chkCH = 0;
	int chkVal = 0;
	int resultVal = 0;

	printf(pStr);
	printf(": ");

	gets(rbuff);

	for(i=0;i<5 && rbuff[i] != 0; i++)
	{
		chkCH = rbuff[i];

		if(chkCH >= ASCII_0 && chkCH <= ASCII_9)
		{
			chkVal = chkCH - ASCII_0;
		}
		else if(chkCH >= ASCII_S_A && chkCH <= ASCII_S_F)
		{
			chkVal = chkCH - ASCII_S_A + 10;
		}
		else if(chkCH >= ASCII_L_A && chkCH <= ASCII_L_F)
		{
			chkVal = chkCH - ASCII_L_A + 10;
		}
		else
			break;
		resultVal = resultVal*16 + chkVal;
	}

	*pVal = resultVal;

	printf("\n");

    return 0;
}

void DDI_NVM_Debug(void)
{
	UINT32			command;
	UINT32			addr, data, size;
	UINT8			*pBuf;
	UINT32			i;
	UINT32			db_base;
	UINT32			db_size;
	UINT32			nvm_size;
	UINT8			tRetVal = OK;

	/* command processing */
	do
	{
		OSA_DBG_PrintMenu ("\n\n");
		OSA_DBG_PrintMenu ("==========	[NVM Debug Menu]  ============\n");
		_NVM_PrintMap();
		OSA_DBG_PrintMenu ("==========================================\n");
		OSA_DBG_PrintMenu ("\n");
		OSA_DBG_PrintMenu ("\t0x01: Read Byte\n");
		OSA_DBG_PrintMenu ("\t0x02: Write Byte\n");
		OSA_DBG_PrintMenu ("\t0x10: Clear NVM\n");
		OSA_DBG_PrintMenu ("\t0x11: Fill Block\n");
		OSA_DBG_PrintMenu ("\t0x12: Read Block\n");
		OSA_DBG_PrintMenu ("\t0x13: Write Block\n");
		OSA_DBG_PrintMenu ("\t0xff: Exit from this menu\n");
		OSA_DBG_PrintMenu ("\n");
		OSA_DBG_PrintMenu ("------------------------------------------\n");
		OSA_DBG_PrintMenu ("\t0x20: PWM Debug Freq\n");
		OSA_DBG_PrintMenu ("\t0x21: PWM Debug Duty\n");
		OSA_DBG_PrintMenu ("\t0x22: PWM Debug Enable\n");
		OSA_DBG_PrintMenu ("\t0x23: PWM Debug Align\n");
		OSA_DBG_PrintMenu ("------------------------------------------\n");
		OSA_DBG_PrintMenu ("\n");
		OSA_DBG_PrintMenu ("------------------------------------------\n");
		OSA_DBG_PrintMenu ("\t0x30: GPIO Debug Read\n");
		OSA_DBG_PrintMenu ("\t0x31: GPIO Debug Write\n");
		OSA_DBG_PrintMenu ("------------------------------------------\n");
		OSA_DBG_PrintMenu ("\n");
		OSA_DBG_PrintMenu ("\t0x41: Inv-panel On\n");
		OSA_DBG_GetHexInput ("Enter command", &command);
		OSA_DBG_PrintMenu ("\n\n");

		switch (command)
		{
			case 0x01 :
				pBuf = (UINT8 *) malloc (4);
				OSA_DBG_GetHexInput ("Addr", &addr);
				DDI_NVM_Read (addr, 1, pBuf);
				printf ("Data : 0x%x\n", pBuf[0]);
				free (pBuf);
				break;

			case 0x02 :
				pBuf = (UINT8 *) malloc (4);
				OSA_DBG_GetHexInput ("Addr", &addr);
				OSA_DBG_GetHexInput ("Data", &data);
				pBuf[0] = (UINT8) data;
				tRetVal = DDI_NVM_Write(addr, 1, pBuf);
				free (pBuf);
				break;

			case 0x10 :
				db_base  = 0;
				db_size  = BASIC_DB_SIZE;

				tRetVal = OK;

				if ((pBuf = (UINT8 *) malloc (db_size)) == NULL)
				{
					tRetVal = NOT_OK;
					break;
				}

				memset(pBuf, 0xff, db_size);
				tRetVal = DDI_NVM_Write (db_base, db_size, pBuf);

				free(pBuf);

				if(tRetVal == OK)
					printf("=== Success to erase NVM-EEPROM!!!\n");
				else
					printf("=== Fail to erase NVM-EEPROM!!!\n");

				load_partinfo();

				for(i=0; i<(partinfo.npartition); i++)
				{
					struct partition_info *partition  = NULL;

					partition = GET_PART_INFO(i);

					if(strncmp("nvram", partition->name, 5) == 0)
					{
						printf(" nvram partition is %d \n",i);
						partition->filesize = 0;
						partition->sw_ver = 0;
						storage_erase(partition->offset, partition->size);
						break;
					}
				}

				if(i == (partinfo.npartition) )
					printf("=== Fail to erase NVM-Storage!!!\n");
				else
					printf("=== Success to erase NVM-Storage!!!\n");

				break;

			case 0x11 :
				break;

			case 0x12 :
				OSA_DBG_GetHexInput("Addr", &addr);
				OSA_DBG_GetHexInput("Size", &size);
				pBuf = (UINT8 *) malloc (size);
				DDI_NVM_Read (addr, size, pBuf);
				for (i=0; i<size; i++)
				{
					printf ("Data[0x%x] : 0x%x\n", addr+i, pBuf[i]);
				}
				free (pBuf);
				break;

			case 0x13 :
				break;

			case 0x20:
				{
					UINT32 freq = 0;
					UINT32 channel = 0;
					OSA_DBG_GetHexInput("Set PWM channel", &channel);
					OSA_DBG_GetHexInput("DDI_CMNIO_PWM_SetFrequency", &freq);
					DDI_CMNIO_PWM_SetFrequency(channel, freq);
				}
				break;
			case 0x21:
				{
					UINT32 duty = 0;
					UINT32 channel = 0;
					OSA_DBG_GetHexInput("Set PWM channel", &channel);
					OSA_DBG_GetHexInput("DDI_CMNIO_PWM_SetDutyCycle", &duty);
					DDI_CMNIO_PWM_SetDutyCycle(channel, duty);
				}
				break;
			case 0x22:
				{
					UINT32 enb = 0;
					UINT32 channel = 0;
					OSA_DBG_GetHexInput("Set PWM channel", &channel);
					OSA_DBG_GetHexInput("DDI_CMNIO_PWM_SetEnable", &enb);
					DDI_CMNIO_PWM_SetEnable(channel, enb);
				}
				break;

			case 0x23:
				{
					UINT32 enable =0;
					OSA_DBG_GetHexInput("PWM Align", &enable);
					DDI_CMNIO_PWM_SetVsyncAlign(1, enable);
				}
				break;

			case 0x30 :
			{
				UINT32	portIndex = 0;
				uchar	val = 0;

				OSA_DBG_GetHexInput("Port Index", &portIndex);
				CMNIO_GPIO_GetInputPort(portIndex,&val);
				printf("Data : %d\n", val);
			}
				break;

			case 0x31 :
			{
				UINT32	portIndex = 0;
				uchar	val = 0;

				OSA_DBG_GetHexInput("Port Index", &portIndex);
				OSA_DBG_GetHexInput("Data", &val);
				CMNIO_GPIO_SetOutputPort(portIndex, val);
			}
				break;

				case 0x41 :
			{
				UINT8	i;
				UINT8	panel_on = 0;

				for( i = 0; i < 3; i ++ )
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

				udelay(500*1000);

				UINT8	inv_on = 0;

				for( i = 0; i < 3; i ++ )
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
				break;

			case 0xFF :
				/* Exit this menu */
				command = 0xff;
				break;

			default :
				break;
		}

		if (tRetVal != OK)
			OSA_DBG_PrintMenu ("DDI_NVM_ Error!!! command[%d]: %d\n",command, tRetVal);

	} while (command != 0xff);

}

int do_nvm_dbg (cmd_tbl_t *cmdtp, int flag, int argc, char *argv[]) // should be used integer type function
{

	DDI_NVM_Debug();

	return 0;
}

U_BOOT_CMD(
		nvmdbg,      1,      0,      do_nvm_dbg,
		"nvmdbg\t- eeprom test progrm\n",
		" - \n"
		);

#if 1	/* for swu mode by junorion */

#define MAGIC_SWU_KEY	0x53	/* 'S' */
#define MAGIC_SWU_MODE	0x20121205

UINT8 DDI_NVM_GetSWUMode( void )
{
	UINT32 off_swumode 	= SYS_DB_BASE + (UINT32)&(gSysNvmDB.swuMode) - (UINT32)&gSysNvmDB;
	UINT32 off_swum_magic	= SYS_DB_BASE + (UINT32)&(gSysNvmDB.swum_magic) - (UINT32)&gSysNvmDB;
	UINT8 mode;
	UINT32 swum_magic;

	// check swu mode magic
	DDI_NVM_Read (off_swum_magic, sizeof(UINT32), &swum_magic);
	if(swum_magic != MAGIC_SWU_MODE) {
		printf("Get SWUM Magic ( %08x : %08x ) failed...\n", swum_magic, MAGIC_SWU_MODE);
		return 0;
	}

	DDI_NVM_Read(off_swumode, sizeof(UINT8), &mode);
	mode = ((mode & 0xff) != MAGIC_SWU_KEY) ? (UINT8)FALSE : (UINT8)TRUE;

	return mode;
}

void DDI_NVM_SetSWUMode( UINT8 mode )
{
	UINT32 off_swumode 	= SYS_DB_BASE + (UINT32)&(gSysNvmDB.swuMode) - (UINT32)&gSysNvmDB;
	UINT32 off_swum_magic	= SYS_DB_BASE + (UINT32)&(gSysNvmDB.swum_magic) - (UINT32)&gSysNvmDB;
	UINT32 swum_magic = (UINT32)MAGIC_SWU_MODE;

	// write swu mode magic
	DDI_NVM_Write(off_swum_magic, sizeof(gSysNvmDB.swum_magic), &swum_magic);

	mode = (mode & 0xff) ? MAGIC_SWU_KEY : 0xff;
	DDI_NVM_Write(off_swumode, sizeof(gSysNvmDB.swuMode), &mode);
}

#define MAGIC_HIB		0x48	/* 'H' */
UINT8 DDI_NVM_GetMakeHib( void )
{
	UINT32 nvm_make_hib = SYS_DB_BASE + (UINT32)&(gSysNvmDB.make_hib) - (UINT32)&gSysNvmDB;
	UINT8 mode;

	DDI_NVM_Read(nvm_make_hib, sizeof(UINT8), &mode);

	mode = ((mode & 0xff) != MAGIC_HIB) ? 0x0 : 0xff;
	printf("%s offset:%x, mode:%d\n", __FUNCTION__, nvm_make_hib, mode);
	return mode;
}

void DDI_NVM_SetMakeHib( UINT8 mode )
{
	UINT32 nvm_make_hib = SYS_DB_BASE + (UINT32)&(gSysNvmDB.make_hib) - (UINT32)&gSysNvmDB;

	mode = (mode & 0xff) ? MAGIC_HIB : 0xff;

	DDI_NVM_Write(nvm_make_hib, sizeof(gSysNvmDB.make_hib), &mode);
}


#define MAGIC_SNAP		0x53	/* 'S' */
UINT8 DDI_NVM_GetSnapShot_Support( void )
{
	UINT32 nvm_support_snapshot = SYS_DB_BASE + (UINT32)&(gSysNvmDB.snapshot_onoff) - (UINT32)&gSysNvmDB;
	UINT8 mode;

	DDI_NVM_Read(nvm_support_snapshot, sizeof(UINT8), &mode);

	mode = ((mode & 0xff) != MAGIC_SNAP) ? 0x0 : 0xff;
	printf("%s offset:%x, mode:%d\n", __FUNCTION__, nvm_support_snapshot, mode);
	return mode;
}

void DDI_NVM_SetSnapShot_Support( UINT8 mode )
{
	UINT32 nvm_support_snapshot = SYS_DB_BASE + (UINT32)&(gSysNvmDB.snapshot_onoff) - (UINT32)&gSysNvmDB;
	mode = (mode & 0xff) ? MAGIC_SNAP : 0xff;
	DDI_NVM_Write(nvm_support_snapshot, sizeof(gSysNvmDB.snapshot_onoff), &mode);
}

UINT8 DDI_NVM_GetSnapShotART( void )
{
	UINT32 nvm_art = SYS_DB_BASE + (UINT32)&(gSysNvmDB.snapshot_art) - (UINT32)&gSysNvmDB;
	UINT8 mode;
	DDI_NVM_Read(nvm_art , sizeof(UINT8), &mode);
	return mode;
}

void DDI_NVM_SetSnapShotART( UINT8 mode )
{
	UINT32 nvm_art = SYS_DB_BASE + (UINT32)&(gSysNvmDB.snapshot_art) - (UINT32)&gSysNvmDB;
	DDI_NVM_Write(nvm_art , sizeof(gSysNvmDB.snapshot_art), &mode);
}

#endif

UINT8 DDI_NVM_GetSoundOutMode( void )
{
	UINT32 nvm_soundout_mode = SYS_DB_BASE + (UINT32)&(gSysNvmDB.soundout_mode) - (UINT32)&gSysNvmDB;
	UINT8 mode;

	DDI_NVM_Read(nvm_soundout_mode, sizeof(UINT8), &mode);

	return mode;
}

/*
 * for zram boot by junorion
 */
UINT32 DDI_NVM_GetZramValue( void )
{
	UINT32 nvm_zram = SYS_DB_BASE + (UINT32)&(gSysNvmDB.zram) - (UINT32)&gSysNvmDB;
	UINT32 value;
	DDI_NVM_Read(nvm_zram, sizeof(gSysNvmDB.zram), &value);
	return value;
}

void DDI_NVM_SetZramValue( UINT32 nZram )
{
	UINT32 nvm_zram = SYS_DB_BASE + (UINT32)&(gSysNvmDB.zram) - (UINT32)&gSysNvmDB;
	DDI_NVM_Write(nvm_zram, sizeof(gSysNvmDB.zram), &nZram);
}

UINT8 DDI_NVM_GetPWM_mode( void )
{
	UINT32	validMark;
	UINT8	mode;

	DDI_NVM_Read(SYS_DB_BASE + (ulong)&(gSysNvmDB.validMark) - (ulong)&gSysNvmDB,	\
			sizeof(gSysNvmDB.validMark), (UINT8 *)&(validMark));

	if(validMark != 0xffffffff)
	{
		DDI_NVM_Read(SYS_DB_BASE + (ulong)&(gSysNvmDB.pwm_mode) - (ulong)&gSysNvmDB,	\
				sizeof(gSysNvmDB.pwm_mode), (UINT8 *)&(mode));
	}
	else
	{
		mode = gSysNvmDB.pwm_mode;	// default value from DB table
	}

	return mode;
}

#define MAGIC_FULL_VERIFY 0x7a

UINT8 DDI_NVM_GetFullVerifyFlag(void)
{
	UINT32 nvm_fullVerify = SYS_DB_BASE + (ulong)&(gSysNvmDB.fullVerify) - (ulong)&gSysNvmDB;
	UINT32 value;

	DDI_NVM_Read(nvm_fullVerify, sizeof(gSysNvmDB.fullVerify), (UINT8*)&value);
	value = ( (value & 0xff) == MAGIC_FULL_VERIFY ) ? (0xff) : (0);
	
	rprint1n("^y^%s:get value = 0x%x",__FUNCTION__,value);
	return value;
}

void DDI_NVM_SetFullVerifyFlag(UINT8 nfullVerify)
{
     UINT32 nvm_fullVerify = SYS_DB_BASE + (ulong)&(gSysNvmDB.fullVerify) - (ulong)&gSysNvmDB;

	 nfullVerify = ( nfullVerify & 0xff	) ? ( MAGIC_FULL_VERIFY) : (0xff);
 	 rprint1n("^y^%s:set value = 0x%x",__FUNCTION__,nfullVerify);
     DDI_NVM_Write(nvm_fullVerify, sizeof(gSysNvmDB.fullVerify), (UINT8*)&nfullVerify);
}


static int make_zram_cmd(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
#define ZRAM_MAGIC		(0x7a)
	UINT32	zram, size;
	UINT8	magic, is_on;

	dprintf("sys_db size = %d\n", sizeof(SYS_DB_T));
	zram = DDI_NVM_GetZramValue();

	magic = (UINT8)(zram >> 24);
	is_on = (UINT8)((zram & 0x00ff0000) >> 16);
	size  = (UINT32)(zram & 0x0000ffff);

	dprintf("zram(%x), magic(%x), on/off(%x), size(%x)\n", zram, magic, is_on, size);

	if(magic != ZRAM_MAGIC)
		is_on = FALSE;

	if(size == 0xffff)
		size = 0;

	printf("[make zram status]\n");
	printf("current : %s (%d)MB\n", (is_on) ? "on" : "off", size);

	if(argc == 1) {
		printf("\n");
		return 0;
	}

	if (argc >= 2) {
		UINT32 temp;

		if(strncmp(argv[1], "clear", 4) == 0) {
			temp = 0xffffffff;
			DDI_NVM_SetZramValue(temp);
			printf(" => done.\n");
			return 0;
		}

		if(strcmp(argv[1], "on") == 0)
			is_on = TRUE;
		else
			is_on = FALSE;

		if(argc == 3)
			size = (UINT32)simple_strtoul(argv[2], NULL, 0 );

		temp = (UINT32)(((ZRAM_MAGIC) << 24)	|
						 ((is_on) << 16)		|
						 (size & 0x0000ffff));

		dprintf("zram = %x, temp = %x\n", zram, temp);
		if(zram != temp) {
			printf(" => %s, %d(MB)\n", argv[1], size);
			DDI_NVM_SetZramValue((UINT32)(temp));
			dprintf("DDI_NVM_SetZramValue(%x)\n", temp);
		}
	}
	else
		return -1;

	return 0;
}

U_BOOT_CMD(
	zram,	3,	0,	make_zram_cmd,
	"zram - set zram\n",
	"zram [on/off/clear]\n"
	"zram on [size]\n"
	);

