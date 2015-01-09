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
 *   $Workfile: Services.c $ 
 *
 * Project:
 * --------
 *   MT5387
 *
 * Description:
 * ------------
 *   T8032 main loop
 *   
 * Author:
 * -------
 *   David Hsieh
 *
 * Last changed:
 * ------------- 
 * $Author: dtvbm11 $ 
 *
 * $Modtime: 04/05/27 10:47p $    
 *
 * $Revision: #1 $
****************************************************************************/
//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "general.h"
#include "hw_bim.h"
#include "drv_kr.h"
#include "hw_reg.h"
//#include "hw_rs232.h"
#include "custom_def.h"
#if ENABLE_CBUS
#include "hw_pdwnc.h"
#include "drv_mhlcbus.h"
#endif

//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------

#ifdef VGA_WAKEUP_MANUAL_WAY
UINT8 _bSP0MuxSate;
UINT16 _bSP0MuxCnt;
#endif

//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Imported functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Prototypes
//-----------------------------------------------------------------------------
static void vExInit(void);
static void _vCommandPush(void);
static void _IrRxHwBusyClr();
//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------
SYSEM_STATE data gSys_state;
#if IR_TASK
IR_DRV_OBJ data gIrObj;
#endif

#if ETHERNET_TASK
UINT8 data gEthernetWakeupFlag;
#endif

#if UART_TASK
UART_DRV_OBJ data gUartObj;
#endif
PDWNC_T8032_CMD_T data fgCmdObj;
//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------
static void vExInit(void)	//initial ext interrupt
{
	//IT0=1;	//Ext0 edge trigger
	IT0=0;	//Ext0 low level trigger
	EX0=1;	//Enable Ext0 interrupt
	PX0=1;	//Set Ext0 high priority
}	

void vConfigSrvad(UINT8 u1ch)
{
    if(u1ch >= CORE_POWER_SRV)//Never config ch6/7 !!!!!!!!!!!!!!
    {
        return;
    }
    MEnableSrvClk();//vRegWrite1B(0x0, u1RegRead1B(0x0) & (~0x04));//make sure SRV clock not stop 
#if 0    
    MPowerOnADC();//vRegWrite1B(0x0390, 0x03);//enable comparator ofset cancellation/adc power on
#else
	vRegWrite4B(0x3bc, u4RegRead4B(0x3bc) | 0x00000060);
#endif
    MSrvadHwMode();//vRegWrite1B(0x0300, 0x1);//enable hw mode 
    MEnableSrvad(u1ch);//vRegWrite1B(0x0304, 0xC8);//only enable adin3
    //MSelectSrvad(u1ch);//vRegWrite1B(0x038C, 0x03);//select ch3
}

UINT8 bReadSrvad(UINT8 u1ch)
{
    if(u1ch >= CORE_POWER_SRV)
    {
        return 0;
    }
    
    return MReadSrvad(u1ch);
}

void vResetSysStateVar(UINT8 u1state)
{
    if(u1state == STANDBY_STATE)
    {
        gSys_state.fgStandbyMode = STANDBY_STATE;   
        gSys_state.fgPowerOff = ARM_POWER_OFF;
        gSys_state.fgNewIrData = NO_EVENT;        
        gSys_state.fgServiceMode = 0;
        gSys_state.fgNewCmdCome = NO_EVENT;
        gSys_state.fgRs232Rx = NO_EVENT;
        gSys_state.fgWakeupReason = PDWNC_WAKE_UP_REASON_UNKNOWN;
        gSys_state.fgSubWakeupReason = 0;
        gSys_state.fgFWUpgrading = 0;
//        gSys_state.fgXdataSwitchReq = NO_EVENT;
//        gSys_state.fgXdataSwitchState = XDATA_BY_T8032;
    }
    else if(u1state == LOADER_STATE)
    {
        gSys_state.fgNewCmdCome = NO_EVENT;
        gSys_state.fgNewIrData = NO_EVENT;
        gSys_state.fgPowerOff = ARM_POWER_ON;
        gSys_state.fgRs232Rx = NO_EVENT;
        gSys_state.fgServiceMode = NO_SERVICE_MODE;
        gSys_state.fgStandbyMode = LOADER_STATE;
        gSys_state.fgWakeupReason = PDWNC_WAKE_UP_REASON_UNKNOWN;
        gSys_state.fgSubWakeupReason = 0;
        gSys_state.fgFWUpgrading = 0;
        gSys_state.fgSysUartBaudRate = BR_115200;
//        gSys_state.fgXdataSwitchState = XDATA_BY_T8032;
//        gSys_state.fgXdataSwitchReq = NO_EVENT;  
    }
}


void vSysCustWakeupProcess(void)
{

#ifdef vTask1WakeupProcess
    vTask1WakeupProcess();
#endif

#ifdef vTask2WakeupProcess
    vTask2WakeupProcess();
#endif

#ifdef vTask3WakeupProcess
    vTask3WakeupProcess();
#endif

#ifdef vTask4WakeupProcess
    vTask4WakeupProcess();
#endif

#ifdef vTask5WakeupProcess
    vTask5WakeupProcess();
#endif

#ifdef vTask6WakeupProcess
    vTask6WakeupProcess();
#endif

#ifdef vTask7WakeupProcess
    vTask7WakeupProcess();
#endif

#ifdef vTask8WakeupProcess
    vTask8WakeupProcess();
#endif

#ifdef vTask9WakeupProcess
    vTask9WakeupProcess();
#endif

#ifdef vTask10WakeupProcess
    vTask10WakeupProcess();
#endif

#ifdef vTask11WakeupProcess
    vTask11WakeupProcess();
#endif

#ifdef vTask12WakeupProcess
    vTask12WakeupProcess();
#endif

#ifdef vTask13WakeupProcess
    vTask13WakeupProcess();
#endif

#ifdef vTask14WakeupProcess
    vTask14WakeupProcess();
#endif

#ifdef vTask15WakeupProcess
    vTask15WakeupProcess();
#endif

}

void vSysCustEnterPowerDownProcess(void)
{
#ifdef vTask1StandbyProcess
    vTask1StandbyProcess();
#endif

#ifdef vTask2StandbyProcess
    vTask2StandbyProcess();
#endif

#ifdef vTask3StandbyProcess
    vTask3StandbyProcess();
#endif

#ifdef vTask4StandbyProcess
    vTask4StandbyProcess();
#endif

#ifdef vTask5StandbyProcess
    vTask5StandbyProcess();
#endif

#ifdef vTask6StandbyProcess
    vTask6StandbyProcess();
#endif

#ifdef vTask7StandbyProcess
    vTask7StandbyProcess();
#endif

#ifdef vTask8StandbyProcess
    vTask8StandbyProcess();
#endif

#ifdef vTask9StandbyProcess
    vTask9StandbyProcess();
#endif

#ifdef vTask10StandbyProcess
    vTask10StandbyProcess();
#endif

#ifdef vTask11StandbyProcess
    vTask11StandbyProcess();
#endif

#ifdef vTask12StandbyProcess
    vTask12StandbyProcess();
#endif

#ifdef vTask13StandbyProcess
    vTask13StandbyProcess();
#endif

#ifdef vTask14StandbyProcess
    vTask14StandbyProcess();
#endif
    
#ifdef vTask15StandbyProcess
    vTask15StandbyProcess();
#endif

}

void vWakeUpSystemTasks(void)
{
    MIR_DIS_INT();//vRegWrite1B(PDWNC_INT_ENABLE + 1, u1RegRead1B(PDWNC_INT_ENABLE + 1) & 0xdf);//disable IR interrupt
//    MOIRI_DIS_INT();//    vRegWrite1B(PDWNC_INT_ENABLE + 3, u1RegRead1B(PDWNC_INT_ENABLE + 3) & 0x7f);//disable OIRI interrupt    
    MSET_ARM_POWER_STATE(ARM_POWER_ON);
    vSysCustWakeupProcess();  
}


void vWakeUpSystem(UINT8 u1Reason, UINT8 u1SubReason) //reentrant
{
        MDisableINT() ;    
        
        MSET_SYS_WAKEUP_REASON(u1Reason);
        MSET_SYS_SUB_WAKEUP_REASON(u1SubReason);
#if ENABLE_CEC        
        fgCecStandbyToOn = 1; 
#endif
#if ENABLE_CBUS
	     fgCbus8032Ctrl = 0;
	     DISABLE_PINMUX_TO_CBUS();
		 vIO32WriteFldAlign(PDWNC_PADCFG14,0,FLD_HDMI_0_HPD_CBUS_PUPD);//280E8 bit 9 PUPD ---  SET to 1
		 vIO32WriteFldAlign(PDWNC_CBUS_LINK_07,0, FLD_ZCBUS_DISCOVER_EN);
		 vIO32WriteFldAlign(PDWNC_CBUS_LINK_07,0, FLD_ZCBUS_SINK_ON_CTRL);
	     vCbus_DisableInterrupt();
	     vCbusClrStatusAll();
#endif
        #if UART_DBG
        LogS("MWAKEUP"); 
        LogB(u1IO32Read1B(PDWNC_T8032_INTEN + 1));
        #endif        
        vWakeUpSystemTasks();        
        MT8032_EN_WAKEUP();//vRegWrite1B(0x0120 + 2, u1RegRead1B(0x0120 + 2)|0x02); //Enable 8032 wake up        
        MWakeUpSystem();
        SwitchBack_PORT0();

        MEnableINT() ;
}


#if HOTKEY_WAKEUP_SUPPORT
UINT8 HotKeyExp;
void vIrHotKeyWakeup_ExpValue(void)
{
	UINT8 i;
	IR_DATA_TYPE u4IrData;

	u4IrData = _ReadIrDataReg();
	for(i=0;i<16;i++)
	{
		 if(u4IO32Read4B(PDWNC_EXP_IR0 + i*4) == u4IrData)
		 {
		 	HotKeyExp = i;
			return;
		 }
	}
}
#endif


void vEnterPowerStandby(void)
{
/*    
   #if UART_DBG
        LogS("Standby mode 0\n");
    #endif
    if(!MGET_SYS_FW_UPGRADING())
*/    
if(1)
    {
#if UART_DBG    
        LogS("vEnterPowerStandby\n");    
#endif   
		//_IrRxHwBusyClr();
        MDisableINT() ;
        MWritePowerDownEntry();
        MClrSysInt(0xffffffff); //clean all interrupt

     #if IR_TASK    
        MIR_EN_INT();
        MIR_DIS_HW_WAKEUP();
        _MIR_FlushKey();
        vRegWriteFldAlign(PDWNC_IRCFGH, 1, FLD_DISPD);
     #else
        MIR_DIS_INT();     
        vRegWriteFldAlign(PDWNC_IRCFGH, 0, FLD_DISPD);
    #endif
        vRegWriteFldAlign(PDWNC_IRCLR, 1, FLD_IRCLR);

        vResetSysStateVar(STANDBY_STATE);
        SwitchUartPD_PORT0();
#if 0// RW_REG_INTERFACE	
                vConfigSrvad(4);
#endif
        vRs232Init(MGET_SYS_UART_BAUDRATE());
    #if ENABLE_CEC
        CEC_Init();
        #if CEC_FEATURE_3
        if ((fgCecEnRx == 1) && (MGET_SYS_PRE_STANDBY_STATUS() != STBY_FROM_NORMAL_POWER))     // 1:from AC on, 2:from Normal mode
        {
            CEC_Report_Physic_Addr();
            CEC_Device_Vendor_ID();		
        }
        #endif   
    #endif
#ifdef VGA_WAKEUP_MANUAL_WAY
	_bSP0MuxSate=0;	
	_bSP0MuxCnt = 900;
	vIO32WriteFldAlign(PDWNC_VGA_WAKE_UP_CONTROL_0_REGISTER, 0x0, FLD_AUTO_SWITCH_EN);
	LogS("Standby:VGA_WAKEUP_MANUAL_WAY init\n");
#endif
	
#if ENABLE_CBUS
		vCbusInit();
		LogS("vEnterPowerStandby: Cbus_Init\n");
#endif
        vSysCustEnterPowerDownProcess();
		#if GPIO_INTERRUPT_ENABLE
		vIO32Write4B(PDWNC_T8032_INTEN, u4IO32Read4B(PDWNC_T8032_INTEN) | 0x04); //OPCTRL2 enable interrupt
		vIO32Write4B(PDWNC_EXINT2ED,  u4IO32Read4B(PDWNC_EXINT2ED) | 0x04);
		#endif
        MEnableINT() ;
		
		#if HOTKEY_WAKEUP_SUPPORT
		HotKeyExp = 0xFF;
		#endif
        //vRegWrite4B(PDWNC_CONCFG0, 0x00010000);
    }
}


void vCmdTaskInit(void)
{
    memset(&fgCmdObj, 0, sizeof(fgCmdObj));
}

UINT8 bCommandPop(PDWNC_T8032_CMD_T* tCmd)
{
    BIT cmd_event;
    MARM_DIS_INT();
    cmd_event = MGET_CMD_STATE();
    if(cmd_event == NEW_EVENT)
    {
        tCmd->u1Cmd = fgCmdObj.u1Cmd;
        tCmd->u1SubCmd = fgCmdObj.u1SubCmd; 
        tCmd->au1Data[0] = fgCmdObj.au1Data[0];
        tCmd->au1Data[1] = fgCmdObj.au1Data[1];
        tCmd->au1Data[2] = fgCmdObj.au1Data[2];
        tCmd->au1Data[3] = fgCmdObj.au1Data[3];
        MSET_CMD_STATE(NO_EVENT); 
    }
    MARM_EN_INT();
    return cmd_event;
}

static void _vCommandPush(void)
{
    fgCmdObj.u1Cmd = u1IO32Read1B(T8032_CMD_FromARM);
    fgCmdObj.u1SubCmd = u1IO32Read1B(T8032_CMD_FromARM + 1); 
    fgCmdObj.au1Data[0] = u1IO32Read1B(T8032_DATA_FromARM);
    fgCmdObj.au1Data[1] = u1IO32Read1B(T8032_DATA_FromARM + 1);
    fgCmdObj.au1Data[2] = u1IO32Read1B(T8032_DATA_FromARM + 2);
    fgCmdObj.au1Data[3] = u1IO32Read1B(T8032_DATA_FromARM + 3);
    MSET_CMD_STATE(NEW_EVENT);    
}
//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------
#if IR_TASK
void vIrTask(void)
{
    IR_DATA_TYPE tmpIrData;
//    if(MGET_ARM_POWER_STATE() != ARM_POWER_OFF)
//        return;    
    MIR_DIS_INT();
    if(_MGET_IR_EVENT() != NEW_EVENT)
    {
        MIR_EN_INT();
        return;
    }
    tmpIrData = _MIR_PopKey();
#if 0//UART_DBG
        LogSD("vIrTask", tmpIrData);
#endif    
    _MIR_FlushKey();
    MIR_EN_INT();  

#ifdef vIR_Task1
    vIR_Task1(tmpIrData);
#endif

#ifdef vIR_Task2
    vIR_Task2(tmpIrData);
#endif

#ifdef vIR_Task3
    vIR_Task3(tmpIrData);
#endif

#ifdef vIR_Task4
    vIR_Task4(tmpIrData);
#endif

#ifdef vIR_Task7
    vIR_Task7(tmpIrData);
#endif


}

#endif

#if UART_TASK
UART_DRV_OBJ* pGetUartDrvObj(void)
{
    return &gUartObj;
}

void _vUART_PushString(void)
{
    UINT8 data uRxCnt;
    //check if TOUT interrupt
    if(RegReadFldAlign(PDWNC_INT_ID, FLD_TOUT))
    {
    //Get the number received..
        uRxCnt = RegReadFldAlign(PDWNC_UART_PD_STATUS, FLD_RXD_BUF_STATE);
        for( ;uRxCnt>0; uRxCnt--)
        {
            *(gUartObj.uRs232Data + gUartObj.uRs232Rxidx) = MGET_UART_CHAR();
            gUartObj.uRs232Rxidx++;

            if(gUartObj.uRs232Rxidx == MAX_UART_BUF_LEN)
                gUartObj.uRs232Rxidx = 0;
        }
               // _LogS(" new evt");
               
		vRs232PriorityProc();//8032 r/w xdata function
		
        MSET_UART_STATE(NEW_EVENT);
    }
}
#endif

static void _IrRxHwBusyClr()
{
    vIO32WriteFldAlign(PDWNC_IRCLR, 0x1, FLD_IRCLR);//IRRX_WRITE32(IRRX_CLEAR_BUSY_REG, IRRX_CLR_BSY_IR);
}

#if ETHERNET_TASK
void vEthernetWakeup()
{
    //UINT8 u1IrWakeupReason;
	if(MGET_ARM_POWER_STATE() != ARM_POWER_OFF)
		return;

    if(_MGET_ETHERNET_Wakeup())
    {
        #if UART_DBG
        LogS("vEthernetWakeup"); 
        #endif  
        
        _METHERNET_ClrWakeup();
        vWakeUpSystem(PDWNC_WAKE_UP_REASON_ETHERNET, 0);
    }
	
	return;

}

static void _vEthernetIntClr()
{
    vIO32Write1B(0x3c50, u1IO32Read1B(0x3c50));   // clear ethernet interrupt state bit   28040 (bit29)
    vIO32Write1B(0x3c51, 0);
    vIO32Write1B(0x3c52, 0);
    vIO32Write1B(0x3c53, 0);
}
#endif
/*
#if defined SIR_TASK
#if SIR_TASK
extern SIRC_CTRL_T idata gSIRC_ctrl;
#endif
#endif
*/
void vPDWNC_ISR(void) small interrupt EX0INT  
{
	UINT32 u4PDWNC_IntStatus;
#if GPIO_INTERRUPT_ENABLE
	UINT32 u4PDWNC_IntStatus2;
#endif

	u4PDWNC_IntStatus = MReadSysInt() & MReadSysIntEn();	
#if UART_DBG
	ISR_LogSD("ex0", u4IO32Read4B(PDWNC_INTSTA));
#endif
	
	if(u4PDWNC_IntStatus & INTERRUPT_RTC_INT_FLAG)
	{
		#if UART_DBG
			ISR_LogSD("RTC int min", u4IO32Read4B(PDWNC_MINUTE));
			ISR_LogSD("second", u4IO32Read4B(PDWNC_SECOND));
		#endif
	}

	
	if(u4PDWNC_IntStatus & INTERRUPT_ARM_INT_FLAG)
	{
		_vCommandPush();
	}
	
#if IR_TASK
	if(u4PDWNC_IntStatus & INTERRUPT_IR_INT_FLAG)
	{
	#if UART_DBG
		ISR_LogSD("ir data", _ReadIrDataReg());
	#endif
		if(_MIR_ACCEPTABLE())
		{
			_MIR_PushKey(_ReadIrDataReg());
			_MSetIR1stSample();
			_MSetIR2ndSample(); 		
			_MSetIR3rdSample(); 					
			_MSetIRBitCnt();						
		}
		_IrRxHwBusyClr();
	}
#endif
	
#if ENABLE_CEC
	if(u4PDWNC_IntStatus & INTERRUPT_CEC_INT_FLAG)
	{
		//vClearSysInt(CEC_CLR);
		CEC_Interrupt();
	}
#endif

#if ENABLE_CBUS  
	if(u4PDWNC_IntStatus & INTERRUPT_CBUS_INT_FLAG)
	{
		vCbus_Interrupt();
	}
#endif

#ifdef ENABLE_AVLINK
	if(u4PDWNC_IntStatus & INTERRUPT_AVLINK_INT_FLAG)
	{
		ISR_LogS("AVLINK_INT");
		//vClearSysInt(AVLINK_INT);
	}
#endif

#if UART_TASK
	if(u4PDWNC_IntStatus & INTERRUPT_UART_INT_FLAG)
	{
	   // if(_MUART_ACCEPTABLE())
	   //		  _LogS("push");
			_vUART_PushString();
		   MCLR_ALL_UART_PD_NOTIFICATION();
	}
#endif


#if ETHERNET_TASK 
	if(u4PDWNC_IntStatus & INTERRUPT_ETHERNET_INT_FLAG) //bit29  ethernet interrupt enable/state bit
    { 
        if(u1IO32Read1B(REG_ETH_INT_STATUS)&MAGIC_PKT_REC)//only wakeup when magic packet interrupt comes
        {  
            _vEthernetIntClr();
            _vEthernetDisableInt();
            _vEthernetDisableWakeup();
            _METHERNET_SetWakeup();
        }
    }
#endif

	//most of models do not support this feature, we divided vPDWNC_ISR() into two parts for implement a totally 8-bits operation (non-gpio) isr...
#if GPIO_INTERRUPT_ENABLE
	u4PDWNC_IntStatus2 = MReadSysInt2() & MReadSysIntEn2();
	if((u4PDWNC_IntStatus & INTERRUPT_GPIO_INT_FLAG) || u4PDWNC_IntStatus2)
	{
		vCustGpioIsr((u4PDWNC_IntStatus & INTERRUPT_GPIO_INT_FLAG),u4PDWNC_IntStatus2);
	}
	MClrSysInt2(u4PDWNC_IntStatus2);
#endif

	MClrSysInt(u4PDWNC_IntStatus);
	MClearT8032INT();
	IE0 = 0;
	
}


//-----------------------------------------------------------------------------
// Interface(exported) functions
//-----------------------------------------------------------------------------
void vPdwncInit(void)
{
    vExInit();
  
    vResetSysStateVar(LOADER_STATE);
#if IR_TASK
    gIrObj.fgCurIrKey = 0;
    gIrObj.fgNewIrData = NO_EVENT;
#endif
	#if HOTKEY_WAKEUP_SUPPORT
	HotKeyExp = 0xFF;
	#endif
    MSET_SYS_PRE_STANDBY_STATUS(STBY_FROM_UNKNOW); 

#if ENABLE_CBUS
    vCbusInit();
    LogS("pdwncinit: Cbus_Init");
#endif

#if ENABLE_CEC
    CEC_Init();
#endif    
LogS("CEC_Init");

#ifdef ENABLE_AVLINK
    i4AVL_init();
#endif  
}



