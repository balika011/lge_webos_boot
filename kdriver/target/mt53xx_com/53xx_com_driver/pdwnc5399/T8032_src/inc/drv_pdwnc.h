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

/*****************************************************************************
 *
 * Filename:
 * ---------
 *   $Workfile: drv_pdwnc.h $ 
 *
 * Project:
 * --------
 *   MT5388
 *
 * Description:
 * ------------
 *    Kernel initailize functions.
 *   
 * Author:
 * -------
 *   yj_chiangyj@mtk.com.tw
 *
 * Last changed:
 * ------------- 
 * $Author: dtvbm11 $ 
 *
 * $Modtime: 04/05/27 10:47p $    
 *
 * $Revision: #1 $
****************************************************************************/
#ifndef _DRV_PDWNC_H_
#define _DRV_PDWNC_H_
#include "custom_def.h"
#include "Hw_pdwnc.h"

typedef enum
{
    NO_EVENT = 0,
    NEW_EVENT
}SYS_EVENT;

typedef enum
{
    ARM_POWER_ON = 0,
    ARM_POWER_OFF
}POEWR_STATE;

typedef enum
{
    NO_SERVICE_MODE = 0,
    CUST1_SERVICE_MODE ,    	
    CUST2_SERVICE_MODE ,    	
    CUST3_SERVICE_MODE ,    	
    CUST4_SERVICE_MODE ,    	
    CUST5_SERVICE_MODE
}SERVICE_MODE;

typedef enum
{
    BR_115200 = 0,
    BR_57600,    	
    BR_38400,    	
    BR_19200,    	
    BR_9600,    	
    BR_2400
}UART_BAUD_RATE;



typedef enum
{
    STANDBY_STATE = 0,
    LOADER_STATE,
    NORMAL_POWER_STATE    
}TV_SYSTEM_STATE;


typedef enum
{
    PDWNC_WAKE_UP_REASON_UNKNOWN = 0,
    PDWNC_WAKE_UP_REASON_VGA,
    PDWNC_WAKE_UP_REASON_RTC,
    PDWNC_WAKE_UP_REASON_KEYPAD,
    PDWNC_WAKE_UP_REASON_IRRC,
    PDWNC_WAKE_UP_REASON_UART,
    PDWNC_WAKE_UP_REASON_AC_POWER,
    PDWNC_WAKE_UP_REASON_HDMI,
    PDWNC_WAKE_UP_REASON_UART_NORMAL,
    PDWNC_WAKE_UP_REASON_RC_DIGIT_0,  /* Remote controller */
    PDWNC_WAKE_UP_REASON_RC_DIGIT_1,
    PDWNC_WAKE_UP_REASON_RC_DIGIT_2,
    PDWNC_WAKE_UP_REASON_RC_DIGIT_3,
    PDWNC_WAKE_UP_REASON_RC_DIGIT_4,
    PDWNC_WAKE_UP_REASON_RC_DIGIT_5,
    PDWNC_WAKE_UP_REASON_RC_DIGIT_6,
    PDWNC_WAKE_UP_REASON_RC_DIGIT_7,
    PDWNC_WAKE_UP_REASON_RC_DIGIT_8,
    PDWNC_WAKE_UP_REASON_RC_DIGIT_9,
    PDWNC_WAKE_UP_REASON_RC_PRG_UP,
    PDWNC_WAKE_UP_REASON_RC_PRG_DOWN,
    PDWNC_WAKE_UP_REASON_RC_INP_SRC,
    PDWNC_WAKE_UP_REASON_RC_ANALOG,
    PDWNC_WAKE_UP_REASON_RC_DIGITAL,
    PDWNC_WAKE_UP_REASON_RC_DIGITAL_ANALOG,
    PDWNC_WAKE_UP_REASON_FP_PRG_UP,   /* Front panel */
    PDWNC_WAKE_UP_REASON_FP_PRG_DOWN,
    PDWNC_WAKE_UP_REASON_FP_INP_SRC,
    PDWNC_WAKE_UP_REASON_DVD,
    PDWNC_WAKE_UP_REASON_RTC_SPECIAL,
    PDWNC_WAKE_UP_REASON_WATCHDOG,
    PDWNC_WAKE_UP_REASON_SCART,
    PDWNC_WAKE_UP_REASON_ETHERNET,
    PDWNC_WAKE_UP_REASON_RESUME_FROM_SUSPEND,
    PDWNC_WAKE_UP_REASON_CUSTOM_1 = 61, 
    PDWNC_WAKE_UP_REASON_CUSTOM_2 = 62, 
    PDWNC_WAKE_UP_REASON_CUSTOM_3 = 63, 
    PDWNC_WAKE_UP_REASON_CUSTOM_4 = 64     
}   PDWNC_WAKE_UP_REASON_T;

typedef struct _SysState
{
    UINT8 fgNewIrData;
    UINT8 fgNewCmdCome;
    UINT8 fgPowerOff;
    UINT8 fgRs232Rx;
//    UINT8 fgXdataSwitchReq;
    UINT8 fgPreStandbyStatus;//Standby condition :   // 1:from AC on, 2:from Normal mode
    
//    UINT8 fgXdataSwitchState;    
    UINT8 fgStandbyMode;  //0: standby, 1:loader, 2: normal...  if T8032 is in standby mode 
    UINT8 fgWakeupReason;
    UINT8 fgSubWakeupReason;    
    UINT8 fgServiceMode;
    UINT8 fgFWUpgrading;
    UINT8 fgSysUartBaudRate;
}
SYSEM_STATE;

typedef struct _IrDrvObj
{
    UINT8 fgNewIrData;
    IR_DATA_TYPE fgCurIrKey;
	UINT8 _1stSample;
	UINT8 _2ndSample;
	UINT8 _3rdSample;	
	UINT8 _BitCnt;		
}IR_DRV_OBJ;

typedef struct _UartDrvObj
{
    UINT8 uRs232Data[MAX_UART_BUF_LEN];
    UINT8 uRs232Rxidx;
    UINT8 fgRs232Rx;
}UART_DRV_OBJ;


typedef struct _SIF_BUS_STATUS_
{
    UINT8 SIF_BUS_SDA;
    UINT8 SIF_BUS_SCL;
    UINT8 SIF_BUS_LOCK;    
    UINT8 fgInit;    
} SIF_BUS_STATUS_T;


typedef struct _SifReqObj
{
    UINT16 u1DevClkDiv;       
    UINT8 u1DevAddr;
    UINT8 u1WordAddrNum;
    UINT8 u1WordAddr1st;
    UINT8 u1WordAddr2nd;    
    UINT8 *pDataBuf;
    UINT8 bDataLen;
}SIF_REQ_OBJ;

typedef struct _EeepromDrvObj
{
    SIF_REQ_OBJ SifReqObj;
    UINT8 u1WP_Gpio;    
    UINT8 u1WP_Gpio_polarity;        
    UINT8 fgInUse;    
    UINT8 fgEepInit;
}EEPROM_DRV_OBJ;




typedef struct
{
    UINT8 u1Cmd;
    UINT8 u1SubCmd;    
//    DATA_FROM_ARM    u4DataFromARM;
    UINT8 au1Data[4];        
} PDWNC_T8032_CMD_T;

typedef enum
{
    XDATA_BY_ARM,
    XDATA_SWITCHING,
    XDATA_BY_T8032    
}XDATA_STATE;

typedef enum
{
    STBY_FROM_UNKNOW,   
    STBY_FROM_AC_ON,        
    STBY_FROM_NORMAL_POWER
}PRE_STANDBY_STATUS;
extern SYSEM_STATE data gSys_state;
#if UART_TASK
extern UART_DRV_OBJ data gUartObj;
#define MRESET_UART_RX_BUF_IDX() gUartObj.uRs232Rxidx = 0;
#endif
#if IR_TASK
extern IR_DRV_OBJ data gIrObj;
#endif
//NON_GPIO_INTERRUPT_XXXX means only 8-bits operation
#define INTERRUPT_RTC_INT_FLAG 0x1000
#define INTERRUPT_ARM_INT_FLAG 0x4000
#define INTERRUPT_IR_INT_FLAG 0x2000
#define INTERRUPT_CEC_INT_FLAG 0x0800
#define INTERRUPT_CBUS_INT_FLAG 0x40000000
#define INTERRUPT_AVLINK_INT_FLAG 0x400
#define INTERRUPT_UART_INT_FLAG 0x200
#define INTERRUPT_ETHERNET_INT_FLAG 0x20000000
#define INTERRUPT_GPIO_INT_FLAG 0xf00ff


#if ENABLE_CBUS
#define MCBUS_DIS_INT() vIO32Write1B(PDWNC_T8032_INTEN + 3, u1IO32Read1B(PDWNC_T8032_INTEN + 3) & 0xbf)
#define MCBUS_EN_INT() vIO32Write1B(PDWNC_T8032_INTEN + 3, u1IO32Read1B(PDWNC_T8032_INTEN + 3) | 0x40)
#endif

#define MReadSysInt() u4IO32Read4B(PDWNC_INTSTA)
#define MReadSysIntEn() u4IO32Read4B(PDWNC_T8032_INTEN)
#define MClrSysInt(x)  vIO32Write4B(PDWNC_INTCLR, (x))

#define _ReadIrDataReg() u4IO32Read4B(PDWNC_IRDATA0)
#define MIR_DIS_INT() vRegWriteFldAlign(PDWNC_T8032_INTEN, 0, FLD_IR_INTEN)
#define MIR_EN_INT() vRegWriteFldAlign(PDWNC_T8032_INTEN, 1, FLD_IR_INTEN)

#define MReadSysInt2() u4IO32Read4B(PDWNC_INTSTA_2)
#define MReadSysIntEn2() u4IO32Read4B(PDWNC_T8032_INTEN_2)
#define MClrSysInt2(x)  vIO32Write4B(PDWNC_INTCLR_2, (x))


#define MCEC_DIS_INT() vRegWriteFldAlign(PDWNC_T8032_INTEN, 0, FLD_CEC_INT0EN)
#define MCEC_EN_INT() vRegWriteFldAlign(PDWNC_T8032_INTEN, 1, FLD_CEC_INT0EN)

#define MIR_DIS_HW_WAKEUP() vRegWriteFldAlign(PDWNC_WAKEN, 0, FLD_IR_WAKEN)
#define MIR_EN_HW_WAKEUP() vRegWriteFldAlign(PDWNC_WAKEN, 1, FLD_IR_WAKEN)
#define MKP_DIS_HW_WAKEUP() vRegWriteFldAlign(PDWNC_WAKEN, 0, FLD_SRVAD_WAKEN)

#define MT8032_DIS_WAKEUP() vRegWriteFldAlign(PDWNC_WAKEN, 0, FLD_T8032_WAKEN)
#define MT8032_EN_WAKEUP() vRegWriteFldAlign(PDWNC_WAKEN, 1, FLD_T8032_WAKEN)

#define MWritePowerDownEntry() vRegWriteFldAlign(PDWNC_PDCODE, 0x14, FLD_PDCODE);\
                               vRegWriteFldAlign(PDWNC_PDCODE, 0x04, FLD_PDCODE);
#define MWakeUpSystem() vRegWriteFldAlign(PDWNC_UPWAK, 0x91, FLD_WAKCODE)

#define MUART_DIS_INT() vRegWriteFldAlign(PDWNC_T8032_INTEN, 0, FLD_UART_DBG_INTEN)
#define MUART_EN_INT() vRegWriteFldAlign(PDWNC_T8032_INTEN, 1, FLD_UART_DBG_INTEN)

#define MUART_FLUSH_TX_BUF() vRegWriteFldAlign(PDWNC_BCR, 0x1, FLD_CLEAR_TBUF);
#define MUART_FLUSH_RX_BUF() vRegWriteFldAlign(PDWNC_BCR, 0x1, FLD_CLEAR_RBUF);

#define MGET_UART_CHAR() u1IO32Read1B(PDWNC_DATAW)
#define MSET_UART_STATE(x) gUartObj.fgRs232Rx = (x)
#define MGET_UART_STATE() gUartObj.fgRs232Rx
#define MSET_CMD_STATE(x) gSys_state.fgNewCmdCome = (x)
#define MGET_CMD_STATE() gSys_state.fgNewCmdCome
#define MSET_SYS_STATE(x) gSys_state.fgStandbyMode= (x)
#define MGET_SYS_STATE() gSys_state.fgStandbyMode
#define MSET_ARM_POWER_STATE(x) gSys_state.fgPowerOff= (x)
#define MGET_ARM_POWER_STATE() gSys_state.fgPowerOff
#define MARM_DIS_INT() vRegWriteFldAlign(PDWNC_T8032_INTEN, 0, FLD_ARM_INTEN)
#define MARM_EN_INT() vRegWriteFldAlign(PDWNC_T8032_INTEN, 1, FLD_ARM_INTEN)  //currently just use 1 of 4

#define T8032_CMD_FromARM PDWNC_SHREG1  //share register8
#define T8032_DATA_FromARM PDWNC_SHREG2 //share register9
#define T8032_DATA_ToARM PDWNC_SHREG0//0x01F0 share register0
#define MWriteDataToARM(val32) vIO32Write4B(T8032_DATA_ToARM,(val32))
#define MTriggerARMInterrupt() vRegWriteFldAlign(PDWNC_AUX_INT, 0x1, FLD_AUX_INT)
#define MSET_SYS_WAKEUP_REASON(x) gSys_state.fgWakeupReason = (x)    
#define MGET_SYS_WAKEUP_REASON() gSys_state.fgWakeupReason  
#define MSET_SYS_SUB_WAKEUP_REASON(x) gSys_state.fgSubWakeupReason = (x)
#define MGET_SYS_SUB_WAKEUP_REASON() gSys_state.fgSubWakeupReason
#define MGET_SYS_SERVICE_MODE()  gSys_state.fgServiceMode
#define MSET_SYS_SERVICE_MODE(x)  gSys_state.fgServiceMode = (x)
#define MSET_SYS_PRE_STANDBY_STATUS(x) gSys_state.fgPreStandbyStatus = (x)
#define MGET_SYS_PRE_STANDBY_STATUS()  gSys_state.fgPreStandbyStatus
#define MSET_SYS_FW_UPGRADING(x)  gSys_state.fgFWUpgrading = (x)
#define MGET_SYS_FW_UPGRADING()  gSys_state.fgFWUpgrading
#define MSET_SYS_UART_BAUDRATE(x)  gSys_state.fgSysUartBaudRate = (x)
#define MGET_SYS_UART_BAUDRATE()  gSys_state.fgSysUartBaudRate

#define XDATA_OWNER_FLAG_ADDR PDWNC_SHREG3
#define MARM_CONTROL_XDATA() (u1IO32Read1B(XDATA_OWNER_FLAG_ADDR) != 0)
#define MCLR_ALL_UART_PD_NOTIFICATION() vIO32Write1B(PDWNC_INT_ID, 0)



//todo ? there are some problems about gpio, currently just keep
#define GPIOOUT_EN 0x0080
#define SetGPIOEN(x) vRegWrite4B(GPIOOUT_EN,(u4RegRead4B(GPIOOUT_EN)|((UINT32)(x))))
#define SetGPIO_AS_OUTPUT(x) SetGPIOEN(x)
#define ClrGPIOEN(x) vRegWrite4B(GPIOOUT_EN,(u4RegRead4B(GPIOOUT_EN)&(~((UINT32)(x)))))
#define SetGPIO_AS_INPUT(x) ClrGPIOEN(x)
#define ReadGPIOEN(x) 	(u4RegRead4B(GPIOOUT_EN)&((UINT32)(x)))
#define GPIOOUT 0x0084
#define SetGPIOOUT(x) vRegWrite4B(GPIOOUT,(u4RegRead4B(GPIOOUT)|((UINT32)(x))))
#define ClrGPIOOUT(x) vRegWrite4B(GPIOOUT,(u4RegRead4B(GPIOOUT)&(~((UINT32)(x)))))
#define ReadGPIOOUT(x) 	(u4RegRead4B(GPIOOUT)&((UINT32)(x)))
#define GPIOIN 0x0088
#define ReadGPIOIN(x) 	(u4RegRead4B(GPIOIN)&((UINT32)(x)))
#if defined(CC_MT5368)||defined(CC_MT5396)||defined(CC_MT5398)
	#define ReadOIRI()	(u1RegRead1B(GPIOIN+1) & 0x20)? 1 : 0	//gpio 213
#elif defined(CC_MT5880)
	#define ReadOIRI()	(u1RegRead1B(GPIOIN) & 0x200)? 1 : 0	//gpio 209
#elif defined(CC_MT5389)
	#define ReadOIRI()	(u1RegRead1B(GPIOIN) & 0x20)? 1 : 0	//gpio 205
#else	//5395/5365
	#define ReadOIRI()	(u1RegRead1B(GPIOIN+1) & 0x40)? 1 : 0	//gpio 214
#endif


#define MEnableSrvClk() vRegWriteFldAlign(PDWNC_CLKPDN, 0, FLD_SRVPD)
#define MDisableSrvClk() vRegWriteFldAlign(PDWNC_CLKPDN, 1, FLD_SRVPD)
#define MSrvadHwMode() vRegWriteFldAlign(PDWNC_SRVCFG0, 1, FLD_HWEN);
//mustang channel b c,d,e,f always on for por
//to do 
#define MEnableSrvad(x) vRegWriteFldAlign(PDWNC_SRVCFG1, 0xF800 | RegReadFldAlign(PDWNC_SRVCFG1, FLD_SRVCH_EN) |(0x1 << x), FLD_SRVCH_EN)
//#define MSelectSrvad(x) vRegWriteFldAlign(PDWNC_ADCCFG0, x, FLD_SRV_SEL)
#define CORE_POWER_SRV (0x8)//mustang channel 0 ~ 7 for normal use
#define MReadSrvad(x) RegReadFldAlign(PDWNC_ADOUT0 + ((x) << 2), FLD_ADOUT0);//only support ad ch 0~9

//SIF control macro
#define MSetSifSclStrech(x) vRegWriteFldAlign(PDWNC_SM0CTL0, (x), FLD_SIFM0_SCL_STRECH)
#define MSetSifSclClkDIV(x) vRegWriteFldAlign(PDWNC_SM0CTL0, (x), FLD_SIFM0_CLK_DIV)
#define MSetSifODrain(x) vRegWriteFldAlign(PDWNC_SM0CTL0, (x), FLD_SIFM0_ODRAIN)
#define SIF_ACK() RegReadFldAlign(PDWNC_SM0CTL1, FLD_SIFM0_ACK) //u1RegRead1B(PDWNC_SM0CTL1 + 2)
#define SIF_PAGE_SIZE_POW           ((UINT8)3)// PAGE size is 8
#define SIF_PAGE_SIZE           (1 << SIF_PAGE_SIZE_POW)


//to do : set sif pin mux 
#if defined(CC_MT5395)
    #if SIF_DRV_SUPPORT
    #define MSetSifPinmux() vRegWrite1B(PDWNC_PINMUX,(u1RegRead1B(PDWNC_PINMUX) & 0xf0) | 0x05) //set [3~0] as [0101]   //set opcrtl0/1 as pd_sda/scl
		#if EEPROM_DRV_SUPPORT
    #define MSetSifEepromWpPinmux() vRegWrite1B((PDWNC_PINMUX),(u1RegRead1B(PDWNC_PINMUX) & 0x3f) | 0x00); //set [7~6] as [00]//set opcrtl3 as gpio(WP)
		#endif
    #else
    #define MSetSifPinmux() vRegWrite1B(PDWNC_PINMUX + 1,(u1RegRead1B(PDWNC_PINMUX + 1) & 0xf3) | 0x04) //set [11~10] as [01]
	#define MSetSifEepromWpPinmux()
	#endif
#elif defined(CC_MT5396)
    #if SIF_DRV_SUPPORT
    #define MSetSifPinmux() vRegWrite1B(PDWNC_PINMUX0,(u1RegRead1B(PDWNC_PINMUX0) & 0xf0) | 0x05) //set [3~0] as [0101]   //set opcrtl0/1 as pd_sda/scl
		#if EEPROM_DRV_SUPPORT
    	#define MSetSifEepromWpPinmux() //vRegWrite1B((PDWNC_PINMUX0),(u1RegRead1B(PDWNC_PINMUX0) & 0x3f) | 0x00); //set [7~6] as [00]//set opcrtl3 as gpio(WP)
		#endif
	#else
    #define MSetSifPinmux()
	#define MSetSifEepromWpPinmux()
	#endif
#else//#elif  defined(CC_MT8223) 	//MT5389_TODO
    #define MSetSifPinmux()
	#define MSetSifEepromWpPinmux()
#endif


//PDWNC Pinmux register
//#define PINMUX 0x00B4

//to do check pin mux
#if defined(CC_MT5882)
#define SwitchUartPD_PORT0() vRegWrite1B(PDWNC_PINMUX2+1,((u1RegRead1B(PDWNC_PINMUX2+1) & 0x81)|0x12));
#else
#define SwitchUartPD_PORT0() vRegWrite1B(PDWNC_PINMUX0+2,((u1RegRead1B(PDWNC_PINMUX0+2) & 0xfc)|0x01))  //UART0 pad as T8032 log //bit 16~17
#endif

#if T8032_OCCUPIED_UART0_AT_NORMAL_POWER
    #define SwitchBack_PORT0()
#else
    #define SwitchBack_PORT0() vRegWrite1B(PDWNC_PINMUX0+2,(u1RegRead1B(PDWNC_PINMUX0+2) & 0xfc)) //UART0 pad as ARM log 
#endif



typedef enum {
    MODE_START        = 0x1,
    MODE_WR_DATA      = 0x2,
    MODE_STOP         = 0x3,
    MODE_RD_DATA_LAST = 0x4,
    MODE_RD_DATA      = 0x5,
} SIF_MODE_T;
#define TRI                           ((UINT8)1 << 0)
typedef enum
{
    SIF_CTL_NORMAL    =  0,       /* Normal I2C operation         */
    SIF_CTL_NO_START  =  1 << 1,  /* Do not generate start signal */
    SIF_CTL_NO_STOP   =  1 << 2,  /* Do not generate stop signal  */
} SIF_BUS_CONTROL_T;


typedef enum
{
    SIF_ERROR_NODEV       = -1,       // SIF device doesn't ack to SIF master
    SIF_ERROR_SUBADDR     = -2,       // There is error when SIF master transmits subaddress data
    SIF_ERROR_DATA        = -3,       // There is error when SIF master transmits/receives data
    SIF_ERROR_BUS_BUSY    = -4,       // SIF bus is busy, can't access
    SIF_ERROR_PARAM       = -5,       // Function parameter is invalid
    SIF_ERROR_TIMEOUT     = -6,       // SIF semaphore timeout
} SIF_ERROR_T;


#if IR_TASK
#define _MIR_PushKey(x) gIrObj.fgCurIrKey = (IR_DATA_TYPE)(x);\
                                    gIrObj.fgNewIrData = NEW_EVENT;
#define _MIR_PopKey() gIrObj.fgCurIrKey
#define _MIR_FlushKey()  gIrObj.fgNewIrData = NO_EVENT
#define _MIR_ACCEPTABLE() gIrObj.fgNewIrData == NO_EVENT
#define _MGET_IR_EVENT() (gIrObj.fgNewIrData)

#define _MSetIR1stSample() gIrObj._1stSample = u1IO32Read1B(PDWNC_IRCNT + 1)
#define _MGetIR1stSample() (gIrObj._1stSample)

#define _MSetIR2ndSample() gIrObj._2ndSample = u1IO32Read1B(PDWNC_IRCNT + 2)
#define _MGetIR2ndSample() (gIrObj._2ndSample)

#define _MSetIR3rdSample() gIrObj._3rdSample = u1IO32Read1B(PDWNC_IRCNT + 3)
#define _MGetIR3rdSample() (gIrObj._3rdSample)

#define _MSetIRBitCnt()	gIrObj._BitCnt = u1IO32Read1B(PDWNC_IRCNT)
#define _MGetIRBitCnt()	(gIrObj._BitCnt)
#endif

#if ETHERNET_TASK
#define _METHERNET_SetWakeup() (gEthernetWakeupFlag = 1)
#define _METHERNET_ClrWakeup() (gEthernetWakeupFlag = 0)
#define _MGET_ETHERNET_Wakeup() (gEthernetWakeupFlag)
#define REG_ETH_INT_STATUS      (0x3c50)
#define MAGIC_PKT_REC           (1<<4)

#define _vEthernetDisableInt()     vIO32Write4B(PDWNC_T8032_INTEN, (u4IO32Read4B(PDWNC_T8032_INTEN) &~(0x20000000))) //bit29
#define _vEthernetDisableWakeup()  vIO32Write4B(PDWNC_PDMISC, (u4IO32Read4B(PDWNC_PDMISC) &~(0x20000000)))   //bit29
#endif
#if UART_TASK
#define _MUART_ACCEPTABLE() (gUartObj.fgRs232Rx == NO_EVENT)
#endif
extern void vWakeUpSystemTasks(void);
extern void vPdwncInit(void);
extern void vIrTask(void);
#if ETHERNET_TASK
extern void vEthernetWakeup();
#endif
extern void vRS232Task(void);
extern void vCmdTaskInit(void);
extern void CECTask(void);
#if ENABLE_CBUS
extern void CBUSTask(void);
#endif
extern void vEnterPowerStandby(void);
extern void ResponseWakeupReason(UINT8 bWakeupReason);
extern void ResponseSWVersion(void);
extern void XdataSwitchOutReady(void);
extern void vWakeUpSystem(UINT8 u1Reason, UINT8 u1SubReason);
extern UINT8 bCommandPop(PDWNC_T8032_CMD_T* tCmd);
extern UART_DRV_OBJ* pGetUartDrvObj(void);

extern void vConfigSrvad(UINT8 u1ch);
extern UINT8 bReadSrvad(UINT8 u1ch);

#if HOTKEY_WAKEUP_SUPPORT
void vIrHotKeyWakeup_ExpValue(void);
#endif


#if GPIO_INTERRUPT_ENABLE
extern void vCustGpioIsr(UINT32 u4IntStatus,UINT32 u4IntStatus2);
#endif

extern void T8032Alive(void);

#if ENABLE_CEC
extern void CECMenuLanguageOK(void);
extern void CECEnableCECOK(void);
extern void CECEnableOneTouchPlayOK(void);
extern void CECT8032CtlCECOK(void);
extern void CECSetVendorIDOK(void);
extern void CECSetVersionOK(void);
#ifdef CEC_FEATURE_3_1 
extern void CECSetOSDName1OK(void);
extern void CECSetOSDName2OK(void);
extern void CECSetOSDName3OK(void);
extern void CECSetOSDName4OK(void);
extern void CECSetWkpCode1OK(void);
extern void CECSetWkpCode2OK(void);
extern void CECSetWkpCode3OK(void);
#endif
#endif
extern void vExInit(void);


#if SIF_DRV_SUPPORT
extern void vSIF_Init(void);
extern INT8 bSIF_Write(SIF_REQ_OBJ* pSifObj);
extern INT8 bSIF_Read(SIF_REQ_OBJ* pSifObj);
    #if EEPROM_DRV_SUPPORT
    extern void vEEPROM_Init(void);
    extern INT8 bEEPROM_Write(UINT16 u2Offset, UINT8 bLen, UINT8* p1Buf);
    extern INT8 bEEPROM_Read(UINT16 u2Offset, UINT8 bLen, UINT8* p1Buf);
    #endif
#endif

#if SIF_PDWNC_TEST
extern UINT8 PD_SIF_Read_Test(UINT8 pdu1WordAddr1st,UINT8 pdu1WordAddr2nd,UINT8 pdbDataLen);
extern UINT8 PD_SIF_Write_Test(UINT8 pdu1WordAddr1st,UINT8 pdu1WordAddr2nd,UINT8 pdbDataLen,UINT8 pddata);
extern UINT8 PD_SIF_EEP_Read_Test(UINT8 pdu1WordAddr1st,UINT8 pdu1WordAddr2nd,UINT8 pdbDataLen);
extern UINT8 PD_SIF_EEP_Write_Test(UINT8 pdu1WordAddr1st,UINT8 pdu1WordAddr2nd,UINT8 pdbDataLen,UINT8 pddata);
#endif

#endif
