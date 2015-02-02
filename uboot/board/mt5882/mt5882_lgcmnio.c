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

#include <common.h>

#include "x_bim.h"
#include "x_timer.h"
#include "x_assert.h"
#include "x_ckgen.h"
#include "x_pinmux.h"
#include "x_typedef.h"
#include "x_ldr_env.h"
#include "x_gpio.h"

#include "sif_if.h"
#include "drvcust_if.h"

#include <mt5882_lgcmnio.h>
#include <cmnio_type.h>

//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Constant definitions
//---------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Macros
//-----------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Type definitions
//---------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Local variables
//-----------------------------------------------------------------------------
UINT32 u4ScanPWM_DblFreq = 0;

//---------------------------------------------------------------------------
// Static variables
//---------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Local Functions
//-----------------------------------------------------------------------------

//-------------------------------------------------------------------------
//
// U-BOOT API Implementation (declared in uboot-xxx/include)
//
//-------------------------------------------------------------------------
int DDI_CMNIO_I2C_Init()
{
    SIF_Init();
    return 0;
}

int DDI_CMNIO_I2C_Read(uchar chNum, uchar transMode, uchar slaveAddr, uint subAddrMode, uchar *subAddr, ushort nBytes, uchar *rxBuf, uint retry)
{
    uint  i;
    int   ret;
    uchar ucSifId;
    uint  uiClkVal=100;
    uint  uiAddr;
    switch(chNum)
    {
		case 0:
			ucSifId = SIF_BUS_SYSTEM_I2C;
			break;
		case 1: 
			ucSifId = SIF_BUS_TUNER_I2C;
			break;
		case 2:
			ucSifId = SIF_BUS_PDWNC_I2C;
			break;
		case 3:
			ucSifId = SIF_BUS_PDWNC_I2C_1;
			break;
        default:
			printf("Error !! I2C bus mapping error , please check ....\n");
            return -1;
        break;
    }


    for(i=0;i<subAddrMode;i++)
    {
        uiAddr = uiAddr << 8;
        uiAddr |= *(subAddr+i);
    }

    for(i=0;i<=retry;i++)
    {
        ret = SIF_X_Read(ucSifId, (27000/uiClkVal), slaveAddr, subAddrMode, uiAddr, rxBuf, nBytes);
        if(ret)
        {
            break;
        }
    }
    return 0;

}

int DDI_CMNIO_I2C_Write(uchar chNum, uchar transMode, uchar slaveAddr, uint subAddrMode, uchar *subAddr, ushort nBytes, uchar *txBuf, uint retry)
{
    uint  i;
    int   ret;
    uchar ucSifId;
    uint  uiClkVal=100;
    uint  uiAddr = 0;
    switch(chNum)
    {
		case 0:
			ucSifId = SIF_BUS_SYSTEM_I2C;
			break;
		case 1: 
			ucSifId = SIF_BUS_TUNER_I2C;
			break;
		case 2:
			ucSifId = SIF_BUS_PDWNC_I2C;
			break;
		case 3:
			ucSifId = SIF_BUS_PDWNC_I2C_1;
			break;
        default:
			printf("Error !! I2C bus mapping error , please check ....\n");
            return -1;
        break;
    }

    for(i=0;i<subAddrMode;i++)
    {
        uiAddr = uiAddr << 8;
        uiAddr |= *(subAddr+i);
    }

    for(i=0;i<=retry;i++)
    {
        ret = SIF_X_Write(ucSifId, (27000/uiClkVal), slaveAddr, subAddrMode, uiAddr, txBuf, nBytes);
        if(ret)
        {
            break;
        }
    }

	if(i > retry)
	{
		printf("I2C CH : %x\n",chNum);
		printf("retry(%d) out!!!!!\n",retry);
		return -1;
	}

    return 0;
}

int CMNIO_GPIO_Init()
{
    GPIO_Init();

	// change pinmux for GPIO49, 50
	#ifdef	BOARD_PT
  //  BSP_PinSet(PIN_GPIO49, PINMUX_FUNCTION0);	// gpio
  //  BSP_PinSet(PIN_GPIO50, PINMUX_FUNCTION0);	// gpio
	#else	// BOARD_DV
   // BSP_PinSet(PIN_GPIO49, PINMUX_FUNCTION3);	// u2_cts
   // BSP_PinSet(PIN_GPIO50, PINMUX_FUNCTION3);	// u2_rts
	#endif

    return 0;
}

int CMNIO_GPIO_SetPortDirection(uint portIndex, char direction)
{
    GPIO_Enable(portIndex, &direction);
    return 0;
}

int CMNIO_GPIO_SetOutputPort(uint portIndex, uchar data)
{
    GPIO_SetOut(portIndex, data);
    return 0;
}

int CMNIO_GPIO_GetInputPort(uint portIndex, uchar *pData)
{
    uint i4Mode=0;
    int i4Val;
    if(pData == NULL || portIndex == 0xffffffff) return -1;
    if(GPIO_Enable(portIndex, &i4Mode) == i4Mode)
    {
        i4Val = GPIO_GetIn(portIndex);
        if(i4Val >=0)
        {
            *pData = i4Val;
        }
        else
        {
            return -1;
        }
    }
    return 0;
}

int CMNIO_GPIO_SetOutputPortArray(uint numArray, uint portArray[], uchar dataArray[])
{
    uint i = 0;
    for(i=0;i<numArray;i++)
    {
        GPIO_SetOut(portArray[i], dataArray[i]);
    }
    return 0;
}

int CMNIO_GPIO_GetInputPortArray(uint numArray, uint portArray[], uchar dataArray[])
{
    uint i = 0;
    int i4Val;
    for(i=0;i<numArray;i++)
    {
        i4Val = GPIO_GetIn(portArray[i]);
        if(i4Val>=0)
        {
            dataArray[i] = i4Val;
        }
        else
        {
            return -1;
        }
    }
    return 0;
}



char aHW_model_opt[NUM_MODEL_OPT+1] = {0,};
char strModelOpt[NUM_MODEL_OPT+1];
MODELOPT_T  gModelOpt = {0,};
char strHWOption[NUM_HW_OPT+1] = {0,};

// panel_interface,	resolution,	panel_type,	cp_box, smalm smart, optic
static MODELOPT_BITCOMB_T gmodelopt_bitcomb[NUM_MODELOPT_BITCOMB] =
{
	{MODELOPT_PANEL_INTERFACE_LVDS,		MODELOPT_PANEL_RESOLUTION_FHD,	0,	MODELOPT_PANEL_TYPE_NONE,	0,	0,	0},
	{MODELOPT_PANEL_INTERFACE_EPI,		MODELOPT_PANEL_RESOLUTION_FHD,	1,	MODELOPT_PANEL_V14_32inch,	0,	0,	0},
	{MODELOPT_PANEL_INTERFACE_EPI,		MODELOPT_PANEL_RESOLUTION_FHD,	1,	MODELOPT_PANEL_V13,			0,	0,	0},
	{MODELOPT_PANEL_INTERFACE_EPI,		MODELOPT_PANEL_RESOLUTION_FHD,	1,	MODELOPT_PANEL_V12,			0,	0,	0},
	{MODELOPT_PANEL_INTERFACE_EPI,		MODELOPT_PANEL_RESOLUTION_FHD,	0,	MODELOPT_PANEL_V14_32inch,	0,	0,	0},
	{MODELOPT_PANEL_INTERFACE_LVDS,		MODELOPT_PANEL_RESOLUTION_FHD,	1,	MODELOPT_PANEL_TYPE_NONE,	0,	0,	0},
	{MODELOPT_PANEL_INTERFACE_EPI,		MODELOPT_PANEL_RESOLUTION_FHD,	1,	MODELOPT_PANEL_V14,			0,	0,	0},
	{MODELOPT_PANEL_INTERFACE_LVDS,		MODELOPT_PANEL_RESOLUTION_HD,	0,	MODELOPT_PANEL_TYPE_NONE,	0,	0,	0},
	{MODELOPT_PANEL_INTERFACE_LVDS,		MODELOPT_PANEL_RESOLUTION_FHD,	0,	MODELOPT_PANEL_TYPE_NONE,	1,	0,	0},
	{MODELOPT_PANEL_INTERFACE_LVDS,		MODELOPT_PANEL_RESOLUTION_HD,	0,	MODELOPT_PANEL_TYPE_NONE,	0,	1,	0},
	{MODELOPT_PANEL_INTERFACE_VBYONE,	MODELOPT_PANEL_RESOLUTION_FHD,	1,	MODELOPT_PANEL_TYPE_NONE,	0,	0,	1},
	{MODELOPT_PANEL_INTERFACE_LVDS,		MODELOPT_PANEL_RESOLUTION_FHD,	1,	MODELOPT_PANEL_TYPE_NONE,	0,	0,	1},
	{0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0}
};

void _modelOpt_parsing(void)
{
	uint8_t bitcomb = ((aHW_model_opt[2]&0x1)<<0x3) | ((aHW_model_opt[3]&0x1)<<0x2) | ((aHW_model_opt[4]&0x1)<<0x1) | (aHW_model_opt[5]&0x1);

	if(aHW_model_opt[0] == 0)
		gModelOpt.country_type =        (!aHW_model_opt[1])? 0:1;   // Bit(0/1): 00->0, 01->1
	else
		gModelOpt.country_type =        (!aHW_model_opt[1])? 2:3;   // Bit(0/1): 10->2, 11->3

	if(bitcomb < NUM_MODELOPT_BITCOMB)
	{
		gModelOpt.panel_interface		= gmodelopt_bitcomb[bitcomb].panel_interface;
		gModelOpt.panel_resolution		= gmodelopt_bitcomb[bitcomb].panel_resolution;
		gModelOpt.bSupport_frc			= gmodelopt_bitcomb[bitcomb].bSupport_frc;
		gModelOpt.panel_type			= gmodelopt_bitcomb[bitcomb].panel_type;
		gModelOpt.bSupport_cp_box		= gmodelopt_bitcomb[bitcomb].bSupport_cp_box;
		gModelOpt.bSupport_small_smart	= gmodelopt_bitcomb[bitcomb].bSupport_small_smart;
		gModelOpt.bSupportOptic			= gmodelopt_bitcomb[bitcomb].bSupportOptic;
	}

	if(aHW_model_opt[6] == 0)
		gModelOpt.tuner_type =          (!aHW_model_opt[7])? 0:1;   // Bit(2/3): 00->0, 01->1
	else
		gModelOpt.tuner_type =          (!aHW_model_opt[7])? 2:3;   //  Bit(2/3): 10->2, 11->3

	gModelOpt.ddr_size =				(aHW_model_opt[8])? DDR_SIZE_768M : DDR_SIZE_1G;
	gModelOpt.direction_HDMI =			(aHW_model_opt[9])? MODELOPT_HDMI_1SIDE_1COMMON : MODELOPT_HDMI_1REAR_1COMMON;
	gModelOpt.bSupport_external_EDID =	(aHW_model_opt[10])? TRUE : FALSE;

	// Do not follow up the HW option. Default Setting
	gModelOpt.type_URSA = MODELOPT_FRC_URSA_NONE;
	gModelOpt.bSupport_URSA = FALSE;
	gModelOpt.bSupport_U14 = FALSE;
	gModelOpt.bSupport_D9 = FALSE;
	gModelOpt.graphic_resolution = MODELOPT_GRAPHIC_1280X720;

	sprintf(strHWOption, "%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d",
			gModelOpt.country_type,             // 1. HW_OPT_COUNTRY
			gModelOpt.bSupport_external_EDID,   // 2. HW_OPT_EXTARNAL_EDID
			gModelOpt.panel_interface,          // 3. HW_OPT_PANEL_INTERFACE
			gModelOpt.panel_resolution,         // 4. HW_OPT_PANEL_RESOLUTION
			gModelOpt.bSupport_frc,             // 5. HW_OPT_FRC
			gModelOpt.panel_type,               // 6. HW_OPT_PANEL_TYPE
			gModelOpt.bSupport_cp_box,          // 7. HW_OPT_CP_BOX
			gModelOpt.bSupport_small_smart,     // 8. HW_OPT_SMALL_SMART
			gModelOpt.tuner_type,               // 9. HW_OPT_TUNER
			gModelOpt.direction_HDMI,           // 10. HW_OPT_HDMI_DIRECTION
			gModelOpt.reserved,                 // 11. HW_OPT_RESERVED1 for sync tvservice
			gModelOpt.ddr_size,                 // 12. HW_OPT_DDR_SIZE
			gModelOpt.bSupportOptic,            // 13. HW_OPT_OPTIC
			gModelOpt.bSupport_URSA,            // 14. HW_OPT_URSA_SUPPORT(LM15U + URSA / LM15U Only)
			gModelOpt.graphic_resolution,       // 15. HW_OPT_GRAPHIC_RESOLUTION
			gModelOpt.bSupport_U14,             // 16. HW_OPT_U14
			gModelOpt.bSupport_D9,              // 17. HW_OPT_D9
			gModelOpt.type_URSA                 // 18. HW_OPT_URSA_TYPE
		   );

	printf("HW_OPT_COUNTRY: %d\n", gModelOpt.country_type);// 1. HW_OPT_COUNTRY
	printf("HW_OPT_EXTARNAL_EDID: %d\n", gModelOpt.bSupport_external_EDID);// 2. HW_OPT_EXTARNAL_EDID
	printf("HW_OPT_PANEL_INTERFACE: %d\n", gModelOpt.panel_interface);// 3. HW_OPT_PANEL_INTERFACE
	printf("HW_OPT_PANEL_RESOLUTION: %d\n", gModelOpt.panel_resolution);// 4. HW_OPT_PANEL_RESOLUTION
	printf("HW_OPT_FRC: %d\n", gModelOpt.bSupport_frc);// 5. HW_OPT_FRC
	printf("HW_OPT_PANEL_TYPE: %d\n", gModelOpt.panel_type);// 6. HW_OPT_PANEL_TYPE
	printf("HW_OPT_CP_BOX: %d\n", gModelOpt.bSupport_cp_box);// 7. HW_OPT_CP_BOX
	printf("HW_OPT_SMALL_SMART: %d\n", gModelOpt.bSupport_small_smart);// 8. HW_OPT_SMALL_SMART
	printf("HW_OPT_TUNER: %d\n", gModelOpt.tuner_type);// 9. HW_OPT_TUNER
	printf("HW_OPT_HDMI_DIRECTION: %d\n", gModelOpt.direction_HDMI);// 10. HW_OPT_HDMI_DIRECTION
	printf("HW_OPT_RESERVED: %d\n", gModelOpt.reserved);// 11. HW_OPT_RESERVED3 for sync tvservice
	printf("HW_OPT_DDR_SIZE: %d\n", gModelOpt.ddr_size);// 12. HW_OPT_DDR_SIZE
	printf("HW_OPT_OPTIC: %d\n", gModelOpt.bSupportOptic);// 13. HW_OPT_OPTIC
	printf("HW_OPT_MODEL: %d\n", gModelOpt.bSupport_URSA);// 14. HW_OPT_URSA_SUPPORT(LM15U + URSA / LM15U Only)
	printf("HW_OPT_GRAPHIC_RESOLUTION: %d\n", gModelOpt.graphic_resolution);// 15. HW_OPT_GRAPHIC_RESOLUTION
	printf("HW_OPT_U14: %d\n", gModelOpt.bSupport_U14);// 16. HW_OPT_U14
	printf("HW_OPT_D9: %d\n", gModelOpt.bSupport_D9);// 17. HW_OPT_D9
	printf("HW_OPT_URSA: %d\n", gModelOpt.type_URSA);// 18. HW_OPT_URSA
}


void initHW_model_option()
{
	int i = 0;
	char aModelOptionGPIO[NUM_MODEL_OPT]={
		GPIO_MODEL_OPT0,
		GPIO_MODEL_OPT1,
		GPIO_MODEL_OPT2,
		GPIO_MODEL_OPT3,
		GPIO_MODEL_OPT4,
		GPIO_MODEL_OPT5,
		GPIO_MODEL_OPT6,
		GPIO_MODEL_OPT7,
		GPIO_MODEL_OPT8,
		GPIO_MODEL_OPT9,
		GPIO_MODEL_OPT10};

	for(i=0; i<NUM_MODEL_OPT; i++)
	{
		uchar val = 0;
		if(CMNIO_GPIO_GetInputPort((uint)aModelOptionGPIO[i], &val) < 0)
		{
			printf("Fail to read H/W model option (%d)!!!!!\n", aModelOptionGPIO[i]);
			return;
		}
		aHW_model_opt[i] = (char)val;
	}

	// Set init OK
	aHW_model_opt[NUM_MODEL_OPT] = 1;

	// parsing model option
	_modelOpt_parsing();

	//make model option string for sending to lgapps through command line
	for(i=0; i<NUM_MODEL_OPT; i++)
	{
		if(aHW_model_opt[i] == 1)
			strModelOpt[i] = '1';
		else
			strModelOpt[i] = '0';
	}

	strModelOpt[i] = '\0';

	// MODEL_OPTION4 && DEMOD_RESET
	// MODEL_OPTION 읽은 후에 DEMOD_RESET을 위해 Output port(LOW) 설정.
	CMNIO_GPIO_SetOutputPort(GPIO_MODEL_OPT4, 0);
//	printf(" strModelOpt : %s \n", strModelOpt);
}

#ifdef CFG_LG_CHG
int DDI_GPIO_SetDefault(void)
{
	//OS Panel Local Dimming
	CMNIO_GPIO_SetOutputPort(GPIO_PORT_WR_LOCAL_DIM_OS_PANEL_CTRL, 0);
}

#define FRC_RESET_START	0
#define FRC_RESET_END	1

int DDI_FRC_Reset(uint frctype, uint mode)
{
	printf("DDI_FRC_Reset: frctype = %d \n",frctype);

	// Netcast4.0 external FRC 사용하지 않음. (향후, UD에서 사용할 수 있으므로 함수 prototype은 유지함.)
	#if 0
	if(frctype == 2) //LG FRC3
	{
		if(mode == 0)//FRC_RESET_START)
		{
			printf("DDI_FRC_Reset: gpio port30 low \n");
			CMNIO_GPIO_SetOutputPort(30,0);//LOW		
		}	
		else if(mode == 1)//FRC_RESET_END)	
		{
			printf("DDI_FRC_Reset:gpio port30 high \n");
			CMNIO_GPIO_SetOutputPort(30,1);//HIGH		
		}	
	}
	#endif
	return 0;
}


int DDI_OTP_WR_Enable(uchar ctrl)
{
	CMNIO_GPIO_SetOutputPort(GPIO_PORT_WR_OTP_WR_CTRL, (uchar)ctrl);
	
	return 0;
}

int DDI_SetLocalDimming_OS_Panel(uchar ctrl)
{
	CMNIO_GPIO_SetOutputPort(GPIO_PORT_WR_LOCAL_DIM_OS_PANEL_CTRL, (uchar)ctrl);
}
#endif

typedef struct pwm_setting_s
{
    uint enable;
    uint freq;
    uint dutyOn;
} pwm_setting_t;

static pwm_setting_t _pwm[3];
#ifndef UBOOT_HACK
 extern UINT32 u4Support_ScanPWM ;
#endif

#define PWM_PORT_SCAN_SRC   1
#define PWM_PORT_SCAN_COPY  2

static int _PwmSet(uint src, pwm_setting_t *pwm)
{
    uint u4PwmRsn, u4PwmP, u4PwmH, u4ScanPwmH;
    uint u4BusClk;

    if(src>2)
        return -1;

    if(pwm != NULL && pwm->enable != 0)
    {
        u4BusClk = BSP_GetDomainClock(SRC_BUS_CLK);

        // Check Rsn value.
        u4PwmRsn = (pwm->dutyOn == 255) ? 0xfe : 0xff;
        u4PwmP = ((u4BusClk >> 8)/ pwm->freq);
        u4PwmH = (pwm->dutyOn >= 0xff) ? 0xff : (pwm->dutyOn);
#ifndef UBOOT_HACK     
        if(u4Support_ScanPWM)
        {
            u4ScanPwmH = (u4PwmH*1000)/255;
            if(src == PWM_PORT_SCAN_SRC)
            {
                UNUSED(BSP_PinSet(PIN_OPWM1, PINMUX_FUNCTION3));
                UNUSED(BSP_PinSet(PIN_OPWM2, PINMUX_FUNCTION3));
                if(u4ScanPWM_DblFreq)
                {
                    vDrvSetScanPWM(PWM_PORT_SCAN_SRC, 0, (u4ScanPwmH+1)/2 ,((1000 - u4ScanPwmH) + 1)/2);
                    vDrvSetScanPWM(PWM_PORT_SCAN_COPY, 250, (u4ScanPwmH+1)/2 ,((1000 - u4ScanPwmH) + 1)/2);
                }
                else
                {
                    vDrvSetScanPWM(PWM_PORT_SCAN_SRC, 0, u4ScanPwmH ,1000 - u4ScanPwmH);
                    vDrvSetScanPWM(PWM_PORT_SCAN_COPY, 500, u4ScanPwmH ,1000 - u4ScanPwmH);                
                }
            }
        }
        else
        {
        	vDrvSetPWM(src,u4PwmP,u4PwmH,u4PwmRsn);
	    }
#endif
    }
    else
    {
        GPIO_SetOut(86+src, 0);
    }
    return 0;
}


#define PWM_PORT_A	0
#define PWM_PORT_B	1
#define PWM_PORT_C	2


void DDI_CMNIO_PWM_Init(UINT32 bEnableSCANPWM, UINT32 bDoubleFreqSCANPWM)
{
    /*
    Load u4Support_ScanPWM setting from NVRAM
    */
#ifndef UBOOT_HACK
    u4Support_ScanPWM = bEnableSCANPWM;
	u4ScanPWM_DblFreq = bDoubleFreqSCANPWM;

    if(u4Support_ScanPWM)
    {
        UNUSED(BSP_PinSet(PIN_OPWM1, PINMUX_FUNCTION3));
        UNUSED(BSP_PinSet(PIN_OPWM2, PINMUX_FUNCTION3));        
        vDrvInitSCANPWM(PWM_PORT_SCAN_SRC);
        vDrvInitSCANPWM(PWM_PORT_SCAN_COPY);
        if(u4ScanPWM_DblFreq)
        {
            vDrvSetScanPWMSW(PWM_PORT_SCAN_SRC, 0, 0, 500);
            vDrvSetScanPWMSW(PWM_PORT_SCAN_COPY, 250, 0, 500);
        }
        else
        {
            vDrvSetScanPWMSW(PWM_PORT_SCAN_SRC, 0, 0, 1000);
            vDrvSetScanPWMSW(PWM_PORT_SCAN_COPY, 500, 0, 1000);
        }
    }
//    else
//    {
//        UNUSED(BSP_PinSet(PIN_OPWM1, PINMUX_FUNCTION2));
//    }
#endif
}

#ifdef CFG_LG_CHG
int DDI_PWM_Init(UINT8 sys_type, PANEL_PWM_T *ppanel_pwm, UINT8 frcType)
{
	TOOL_OPTION1_T		toolOpt1;
	TOOL_OPTION2_T		toolOpt2;
	TOOL_OPTION5_T		toolOpt5;
	TOOL_OPTION7_T		toolOpt7;
	
	uint				PowerBoardtype = 0;
	uint				BackLightType = 0;
	uint				ScanningBL = 0;
	uint				ScanningDoubleFreq = 0;
	
	DDI_NVM_GetToolOpt1(&toolOpt1);
	DDI_NVM_GetToolOpt2(&toolOpt2);
	DDI_NVM_GetToolOpt5(&toolOpt5);
	DDI_NVM_GetToolOpt7(&toolOpt7);

	PowerBoardtype = toolOpt7.flags.bPowerBoardType;	// TRUE:PSU, FALSE:LPB
	BackLightType = toolOpt2.flags.eBackLight;
	ScanningBL = toolOpt5.flags.bScanningBL;

	// Nor FRC(60hz) 출력에서만 DB값에 따라 ScanPWM Double Frequency 유무 판단
	if(!frcType && (ppanel_pwm->vbrBFreq50hz == 100))
		ScanningDoubleFreq = TRUE;
	else
		ScanningDoubleFreq = FALSE;
	
	// PWM2 180도 Shift 기능은 POLA or ROW module 중에 ScanningBacklight option on 인 경우에만 수행함.
	if(((BackLightType == BL_ROW) || (BackLightType == BL_POLA))|| (ppanel_pwm->config & PWM_PHASE_SHIFT))
	{
		if(ScanningBL)
			DDI_CMNIO_PWM_Init(TRUE, ScanningDoubleFreq);
		else
			DDI_CMNIO_PWM_Init(FALSE, ScanningDoubleFreq);
	}
	else
	{
		DDI_CMNIO_PWM_Init(FALSE, ScanningDoubleFreq);
	}

#if 0	
	if(sys_type) //dvb
	{
		if(panel_pwm.vbrBFreq50hz == 100) 
		{
			DDI_CMNIO_PWM_SetFrequency(PWM_PORT_B, 120);
		}
		else
		{
			DDI_CMNIO_PWM_SetFrequency(PWM_PORT_B, panel_pwm.vbrBFreq50hz);
		}

	}
	else //atsc
	{		
		DDI_CMNIO_PWM_SetFrequency(PWM_PORT_B, panel_pwm.vbrBFreq60hz); 
	}	
#else
	if((ppanel_pwm->config & PWM_VRST_EN) || (ppanel_pwm->config & PWM_VAR_EN))
	{
		// App에서 video init전까지 DVB/ATSC 구분없이 Vsync는 60hz 계열로 출력됨.
		// Align mode 사용하기 위해서는 PWM 주파수 60hz 계열로 셋팅해야 함.
		DDI_CMNIO_PWM_SetFrequency(PWM_PORT_B, ppanel_pwm->vbrBFreq60hz);
	}
	else
	{
		if(sys_type)	// DVB
			DDI_CMNIO_PWM_SetFrequency(PWM_PORT_B, ppanel_pwm->vbrBFreq50hz);
		else
			DDI_CMNIO_PWM_SetFrequency(PWM_PORT_B, ppanel_pwm->vbrBFreq60hz);
	}
#endif
	DDI_CMNIO_PWM_SetDutyCycle(PWM_PORT_B, ppanel_pwm->vbrBMaxDuty);
	DDI_CMNIO_PWM_SetEnable(PWM_PORT_B,1);

	if((PowerBoardtype == FALSE) && (BackLightType == BL_EDGE_LED))
	{
        printf("[BAL][Tool] LPB Power..pwm #2 = %d percentage \n",ppanel_pwm->vbrCLedCurrent);
        DDI_CMNIO_PWM_SetFrequency(PWM_PORT_C, ppanel_pwm->vbrCFreq);
        DDI_CMNIO_PWM_SetDutyCycle(PWM_PORT_C, ppanel_pwm->vbrCLedCurrent);
        DDI_CMNIO_PWM_SetEnable(PWM_PORT_C,1);
    }

	//disable boot Vsync align
	//if(panel_pwm.config & PWM_VRST_EN) DDI_CMNIO_PWM_SetVsyncAlign(PWM_PORT_B, 1);

	return 0;

}

int DDI_PWM_PostInit(PANEL_PWM_T *ppanel_pwm)
{
	if(ppanel_pwm->config & PWM_VRST_EN)
	{
		DDI_CMNIO_PWM_SetVsyncAlign(PWM_PORT_B, 1);
		printf("[%d]:PWM Lock mode\n", readMsTicks());
	}
	else
		printf("[%d]:PWM Freerun mode\n", readMsTicks());
}
#endif

int DDI_SPI_Init(uint idx, uint u4double)
{
    DDI_CMNIO_SPI_Init(idx, u4double);
    return 0;
}

int DDI_CMNIO_PWM_SetFrequency(uint pwmIndex, uint data)
{
    if(pwmIndex > 2) return -1;
    _pwm[pwmIndex].freq = data;
    _PwmSet(pwmIndex, &_pwm[pwmIndex]);
    return 0;
}

int DDI_CMNIO_PWM_SetDutyCycle(uint pwmIndex, uint data)
{
    if(pwmIndex > 2) return -1;

    _pwm[pwmIndex].dutyOn = data* 255/ 100;
    _PwmSet(pwmIndex, &_pwm[pwmIndex]);
    return 0;
}



int DDI_CMNIO_PWM_SetEnable(uint pwmIndex, uint data)
{
    if(pwmIndex > 2) return -1;
    _pwm[pwmIndex].enable = data;
    _PwmSet(pwmIndex, &_pwm[pwmIndex]);
    return 0;
}

int DDI_CMNIO_PWM_SetPulseWidth(uint pwmIndex, uint data)
{
    /* skip */ return -1;
}

int DDI_CMNIO_PWM_SetVsyncAlign(uint pwmIndex, uint data)
{
    if(pwmIndex > 2) return -1;

	vDrvSetLock(pwmIndex, data);
    return 0;
}



int DDI_CMNIO_SPI_Init(uint idx, uint u4double)
{
//LOADER_WORKAROUND_FOR_BUILD_ERROR
//    vDrvSPIVsyncInit(idx, u4double);
    return 0;
}

int DDI_CMNIO_PWM_PreInit(void)
{
	//If there is something to initialize in MTK driver, use this function.
	vDrvInitSCANPWM(PWM_PORT_A+2);
	vDrvInitSCANPWM(PWM_PORT_B+2);
	vDrvSetScanPWMLatchMode(0,0,0);

	return 0;
}
int DDI_CMNIO_PWM_ApplyParamSet(UINT8 pwmIndex, UINT8 m_pwm_enable,	UINT8 m_pwm_duty, 
	UINT32 m_pwm_frequency, UINT32 m_pwm_lock, UINT32 m_pwm_pos_start, UINT32 pwm_scanning_enable)
{
	//These parameters are similar to HAL parameters.
	//Apply the parameters.

	if(pwm_scanning_enable == 0) //OPWM ?
	{
		UINT32 u4PwmRsn, u4PwmP, u4PwmH, u4PwmExtP;
	    UINT32 u4BusClk;
		UINT32 u4Val = 0;

		UNUSED(u4Val);

	    // Validate duty cycle
	    if(m_pwm_duty > 255)
	    {
	        return -1;
	    }
	    u4BusClk = BSP_GetDomainClock(SRC_BUS_CLK);
	    // Validate frequency upper bound is ((Bus clock)/256)
	    if ((u4BusClk >> 8) <= m_pwm_frequency)
	    {
	        return -2;
	    }
	    // Check Rsn value.
	    u4PwmRsn = 0xff;
		if(m_pwm_frequency == 0)
		{
			u4PwmP = 0;
			u4PwmH = 0;
		}
		else
		{
		    u4PwmP = ((u4BusClk >> 8)/ m_pwm_frequency);
			u4PwmH = m_pwm_duty;
		}
		vDrvSetPWM(pwmIndex,u4PwmP,u4PwmH,u4PwmRsn);
		vDrvSetLock(pwmIndex, m_pwm_lock);
	}
	else //Scan PWM ??
	{
		if(m_pwm_enable==0)
		{
			vDrvInitSCANPWM(pwmIndex+2);
			GPIO_SetOut(80+pwmIndex, 0);
	
		}
		else

		{		    	
				UINT32 u4Devider=1;
				UINT32 u4High=0;
				UINT32 u4Low=0;
				UINT32 u4Port=0;
				UINT32 u4Start=0;
				
				Printf( "%d,%d,%d,%d\n", pwmIndex, m_pwm_duty, m_pwm_frequency, m_pwm_pos_start);
				
				u4Port=pwmIndex+2;
				if (pwmIndex==0)
				BSP_PinSet(PIN_OPWM1, PINMUX_FUNCTION3);
				if (pwmIndex==1)
				BSP_PinSet(PIN_OPWM2, PINMUX_FUNCTION3);
				
				
		    	u4High = m_pwm_duty*1000/256;
				u4Low = 1000-u4High;
			    u4Start =m_pwm_pos_start*1000/256;
				
			    u4Devider = m_pwm_frequency/vDrvGetLCDFreq();
			   
			    Printf( "_|-|_  freq:%dHz,Devider:%d,start(permillage):%d%,high(permillage):%d%,low(permillage):%d%\n", m_pwm_frequency, u4Devider, u4Start,u4High, u4Low);

				u4High = u4High/u4Devider;
				u4Low = u4Low/u4Devider;
			  	u4Start=u4Start/u4Devider;
				
		    
			    vDrvSetScanPWM(u4Port,u4Start,u4High,u4Low);

			    vDrvScanPWMDataFire();
			
		 }
		}
	
	return 0;
}

