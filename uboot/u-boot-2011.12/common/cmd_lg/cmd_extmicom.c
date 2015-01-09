//by redcloud00
#include <common.h>
#include <command.h>
#include <micom.h>



S32	MICOM_GetVersion (MICOM_VERSION_T *pVersion)
{
	S32				retVal;
	UINT8			cmd;
	UINT8			data[7];

	cmd = CP_READ_MICOM_VERSION;

	retVal = MICOM_READ_COMMAND ( cmd, 7, data );
	if (retVal > 0)
	{
		printf("MICOM_GetVersion \n");
		printf("data[0] = %d, data[1] = %d, data[2] = %d, data[3] = %d, data[4] = %d \n",data[0],data[1],data[2],data[3],data[4]);
		pVersion->number[0] = data[0];
		pVersion->number[1] = data[1];
		pVersion->number[2] = data[2];
		pVersion->number[3] = data[3];
		pVersion->number[4] = data[4];
		pVersion->number[5] = data[5];
		pVersion->number[6] = data[6];
		pVersion->number[7] = 0;
	}

	return( retVal );
}

S32 MICOM_GetHWoption (UINT8 *pHWption)
{
	S32	retVal;
	S32			retry_cnt = 0;
	UINT8		cmd;
	UINT8		data[1];

	/* Send HWoption read command */
	cmd = CP_READ_MICOM_HWOPTION;

	do
	{
		retVal = MICOM_READ_COMMAND ( cmd, 1, data );
		*pHWption = data[0];
		retry_cnt++;
	}
	while( (retVal < 0) && (retry_cnt <=10));

	if(!((((*pHWption) && 0xff) == 0x00) || (((*pHWption) && 0xff) == 0x01) || (((*pHWption) && 0xff) == 0x02))) (*pHWption) = 0x00;
	
	return( retVal );
}

S32 MICOM_GetRTCTime (TIME_T *pTime)
{
	S32			retVal;
	UINT8		cmd;
	UINT8		data[8];

	/* Send RTC read command */
	cmd = CP_READ_RTC_YMDHMS;

	retVal = MICOM_READ_COMMAND ( cmd, 7, data );
	if(retVal > 0)
	{
		pTime->year		= (UINT16)data[0]*100 + data[1];
		pTime->month  	= data[2];
		pTime->day	 	= data[3];
		pTime->hour	 	= data[4];
		pTime->minute 	= data[5];
		pTime->second 	= data[6];

		if (pTime->hour == 24)
		{
			/* Host에서는 MICOM과 달리 24시 대신 0시를 사용함 */
			pTime->hour = 0;
		}
	}

	return( retVal );
}

S32	MICOM_TurnOnInv(void)
{
	S32		retVal;
	U8	data[1];

	data[0] = CP_WRITE_INVERT_OR_VAVS_ON;
	retVal = MICOM_WRITE_COMMAND ( (U8*)&data, 1);

	return( retVal );
}

S32	MICOM_VerifyInverterOn(UINT8 *pRData)
{
	S32			retVal;
	UINT8		cmd;
	UINT8		data[1];

	/* Send Verity Inv On read command */
	cmd = CP_READ_MICOM_INV_ONOFF;

	retVal = MICOM_READ_COMMAND ( cmd, 1, data );
	if(retVal > 0)
	{
		*pRData = data[0];
		//printf("Inv On response = 0x%x \n",*pRData);
	}

	return( retVal );
}

S32	MICOM_TurnOnPanelOn(void)
{
	S32		retVal;
	UINT8	data[2];

	data[0] = CP_WRITE_PANEL_ONOFF;
	data[1] = 1; //panel on
	retVal = MICOM_WRITE_COMMAND ( (U8*)&data, 2);

	return( retVal );
}

S32	MICOM_VerifyPanelOn(UINT8 *pRData)
{
	S32			retVal;
	UINT8		cmd;
	UINT8		data[1];

	/* Send Verity Panel On read command */
	cmd = CP_READ_MICOM_PANEL_ONOFF;

	retVal = MICOM_READ_COMMAND ( cmd, 1, data );
	if(retVal > 0)
	{
		*pRData = data[0];
		//printf("Panel On response = 0x%x \n",*pRData);
	}

	return( retVal );
}

int do_MICOM_I2C_Test( cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
	U8 cmd;
	U8 data[8];
	S32	  retVal;
	U8 i;
	ulong temp[5];
	U8 len;
	//argv[1] : r/w
	//argv[2] : command
	//argv[3] : number of data
	//argv[4] ~ [5]: data
	if( (strncmp(argv[1],"r",1)==0) || (strncmp(argv[1],"R",1)==0) )
	{
		printf("read op \n ");
		temp[2] = simple_strtoul(argv[2], NULL, 0);
		cmd = (UINT8)temp[2];
		temp[3] = simple_strtoul(argv[3], NULL, 0);
		len = (UINT8)temp[3];

		printf("cmd = 0x%x \n",cmd);
		retVal = MICOM_READ_COMMAND ( cmd, len, data );
		for(i=0; i<len; i++)
		{
			printf("data[%d] = %d \n",i,data[i]);
		}
	}
	else if( (strncmp(argv[1],"w",1)==0) || (strncmp(argv[1],"W",1)==0) )
	{
		printf("write op \n ");
		temp[2] = simple_strtoul(argv[2], NULL, 0);
		data[0] = (UINT8)temp[2];

		printf("cmd = 0x%x \n",data[0]);
		temp[3] = simple_strtoul(argv[3], NULL, 0);
		len = (UINT8)temp[3];
		for(i=1;i<len;i++)
		{
			temp[3+i] = simple_strtoul(argv[3+i], NULL, 0);
			data[i]= temp[3+i];
			printf("data[%d] = %d \n",i,data[i]);
		}
		retVal = MICOM_WRITE_COMMAND ( (U8*)&data, len);
	}
	else
		printf("no r/w \n");

	return 0;

}

U_BOOT_CMD(
	micom_i2c,	6,	1,	do_MICOM_I2C_Test,
	"micom_i2c\t- micom i2c test\n",
	"command: micom_i2c [r/w] [command] [number of data] [data1] [data2] .... \n"
);
