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
 *   $Workfile: command.c $ 
 *
 * Project:
 * --------
 *   MT8226
 *
 * Description:
 * ------------
 *   BIM register definition
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
#include "general.h"
#include "Command_Task.h"
#include "Cust_Tasks.h"
#include "drv_hdmicec.h"
#include "custom_def.h"
#if ENABLE_CBUS
#include "drv_mhlcbus.h"
#endif

extern UINT8 GetColorIdx(UINT8 flag);

#if S_HOTEL_MODE_COMMAND
extern UINT8 bModule2Status;
extern UINT8 u1CustomData1_HotelModule;
extern UINT8 u1CustomData3_Module1ReplyData;
extern UINT8 u1hotel_module2_init_mode;
extern UINT8 xdata u1Module2Enable;
extern UINT8 data u1HotelDbg;
extern UINT8 u1Module2AvSelectPW;
extern UINT8 u1Module2AvSelectSrc;
extern UINT8 u1Module2HdmiPortSel;
#endif
void vCMD_RESONSE(UINT8 u1Cmd, UINT8 u1Para0, UINT8 u1Para1, UINT8 u1Para2)
{
    UINT32 u4Para;
    u4Para = (((UINT32)u1Para2) << 24) | (((UINT32)u1Para1) << 16) | (((UINT32)u1Para0) << 8) | u1Cmd;
    MWriteDataToARM(u4Para);
    MTriggerARMInterrupt();
}

#if ENABLE_CEC
void CECMenuLanguageOK(void)
{
//	vWriteDataToARM(Cmd_ResponseCEC_MenuLanguage);
//	vTriggerARMInterrupt();
    vCMD_RESONSE(Cmd_ResponseCEC_MenuLanguage, 0, 0, 0);
}
void CECEnableCECOK(void)
{
//	vWriteDataToARM(Cmd_ResponseCEC_EnableCEC);
//	vTriggerARMInterrupt();
    vCMD_RESONSE(Cmd_ResponseCEC_EnableCEC, 0, 0, 0);
}
void CECEnableOneTouchPlayOK(void)
{
//	vWriteDataToARM(Cmd_ResponseCEC_EnableOneTouch);
//	vTriggerARMInterrupt();
    vCMD_RESONSE(Cmd_ResponseCEC_EnableOneTouch, 0, 0, 0);
}
void CECT8032CtlCECOK(void)
{
//	vWriteDataToARM(Cmd_ResponseCEC_SetT8032CtlCEC);
//	vTriggerARMInterrupt();
    vCMD_RESONSE(Cmd_ResponseCEC_SetT8032CtlCEC, 0, 0, 0);	
}
void CECSetVendorIDOK(void)
{
//	vWriteDataToARM(Cmd_ResponseCEC_SetVendorID);
//	vTriggerARMInterrupt();
    vCMD_RESONSE(Cmd_ResponseCEC_SetVendorID, 0, 0, 0);	
}
void CECSetVersionOK(void)
{
//	vWriteDataToARM(Cmd_ResponseCEC_SetVersion);
//	vTriggerARMInterrupt();
    vCMD_RESONSE(Cmd_ResponseCEC_SetVersion, 0, 0, 0);	
}
#ifdef CEC_FEATURE_3_1 
void CECSetOSDName1OK(void)
{
//	vWriteDataToARM(Cmd_ResponseCEC_SetOSDName1);
//	vTriggerARMInterrupt();
    vCMD_RESONSE(Cmd_ResponseCEC_SetOSDName1, 0, 0, 0);	
}
void CECSetOSDName2OK(void)
{
//	vWriteDataToARM(Cmd_ResponseCEC_SetOSDName2);
//	vTriggerARMInterrupt();
    vCMD_RESONSE(Cmd_ResponseCEC_SetOSDName2, 0, 0, 0);	
}
void CECSetOSDName3OK(void)
{
//	vWriteDataToARM(Cmd_ResponseCEC_SetOSDName3);
//	vTriggerARMInterrupt();
    vCMD_RESONSE(Cmd_ResponseCEC_SetOSDName3, 0, 0, 0);	
}
void CECSetOSDName4OK(void)
{
//	vWriteDataToARM(Cmd_ResponseCEC_SetOSDName4);
//	vTriggerARMInterrupt();
    vCMD_RESONSE(Cmd_ResponseCEC_SetOSDName4, 0, 0, 0);	
}
void CECSetWkpCode1OK(void)
{
//	vWriteDataToARM(Cmd_ResponseCEC_SetWakeupOpCode1);
//	vTriggerARMInterrupt();
    vCMD_RESONSE(Cmd_ResponseCEC_SetWakeupOpCode1, 0, 0, 0);	
}
void CECSetWkpCode2OK(void)
{
//	vWriteDataToARM(Cmd_ResponseCEC_SetWakeupOpCode2);
//	vTriggerARMInterrupt();
    vCMD_RESONSE(Cmd_ResponseCEC_SetWakeupOpCode2, 0, 0, 0);	
}
void CECSetWkpCode3OK(void)
{
//	vWriteDataToARM(Cmd_ResponseCEC_SetWakeupOpCode3);
//	vTriggerARMInterrupt();
    vCMD_RESONSE(Cmd_ResponseCEC_SetWakeupOpCode3, 0, 0, 0);	
}
#endif
#endif

#if SUPORT_COLD_REBOOT
extern UINT8 u1ColdRebootChk;
#endif

#if CUS_SH_HOTEL_MODE 
extern UINT8 fgCust_SH_HotelModeUartWakeupEn;    
#endif

#if SUPPORT_ERROR_LED_CTRL
extern Error_Code_Ctrl gErrCodeCtrl;
#endif
#if SUSPEND_MODE
extern UINT32 PreStandbySta;
extern UINT32 GoingSupend;
#endif
#if S_HOTEL_MODE_COMMAND
extern Led_Toggle_Ctrl gLedToggleCtrl;
#endif

#if SUSPEND_MODE
extern UINT32 PreStandbySta;
extern UINT32 GoingSupend;
#endif

#if SIR_TASK
extern SIRC_CTRL_T idata gSIRC_ctrl;
extern UINT8 data gIRWakeupKeyList;
#endif


#if DISABLE_WAKEUP_SOMETIME
extern Delay_Wakeup_Ctrl gDelayWakeupCtrl;
#endif

void vCommandTask(void)
{
    PDWNC_T8032_CMD_T data tmpCmd;
    if(bCommandPop(&tmpCmd) == NO_EVENT)
    {
        return;//no new command
    }
#if UART_DBG    
    LogSB("Mcmd", tmpCmd.u1Cmd);
    LogSB("Scmd", tmpCmd.u1SubCmd);
#endif 

//    Cmd_ReadT8032WakeUp is the COMMAND most frequestly issued from ARM, so place it at 1st decision...
    if(tmpCmd.u1Cmd == Cmd_ReadT8032WakeUp)
    {
        vCMD_RESONSE(Cmd_ReadT8032WakeUp, MGET_SYS_WAKEUP_REASON(), MGET_SYS_SUB_WAKEUP_REASON(), MGET_SYS_SERVICE_MODE());
    }
    #if SIF_PDWNC_TEST  
    else if(tmpCmd.u1Cmd == Cmd_PDSIF_READ)
    {
        #if UART_DBG
        LogS("t8032 Cmd_PDSIF_READ");        
        #endif         
        vCMD_RESONSE(Cmd_PDSIF_READ, PD_SIF_Read_Test(tmpCmd.au1Data[0],tmpCmd.au1Data[1],tmpCmd.au1Data[2]), 0, 0);
    }
    else if(tmpCmd.u1Cmd == Cmd_PDSIF_WRITE)
    {
        #if UART_DBG
        LogS("t8032 Cmd_PDSIF_WRITE");
        #endif        
        vCMD_RESONSE(Cmd_PDSIF_WRITE, PD_SIF_Write_Test(tmpCmd.au1Data[0],tmpCmd.au1Data[1],tmpCmd.au1Data[2],tmpCmd.au1Data[3]), 0, 0);
    }
    else if(tmpCmd.u1Cmd == Cmd_PDSIF_EEP_READ)
    {
        #if UART_DBG
        LogS("t8032 Cmd_PDSIF_EEP_READ");        
        #endif        
        vCMD_RESONSE(Cmd_PDSIF_EEP_READ, PD_SIF_EEP_Read_Test(tmpCmd.au1Data[0],tmpCmd.au1Data[1],tmpCmd.au1Data[2]), 0, 0);
    }
    else if(tmpCmd.u1Cmd == Cmd_PDSIF_EEP_WRITE)
    {
        #if UART_DBG
        LogS("t8032 Cmd_PDSIF_EEP_WRITE");
        #endif       
        vCMD_RESONSE(Cmd_PDSIF_EEP_WRITE, PD_SIF_EEP_Write_Test(tmpCmd.au1Data[0],tmpCmd.au1Data[1],tmpCmd.au1Data[2],tmpCmd.au1Data[3]), 0, 0);
    }
    #endif

#if T8032_GPIO_TEST
	else if(tmpCmd.u1Cmd == Cmd_GPIOOutput)
	{
		//tmpCmd.au1Data[0]: number, OPCTRL(x)-OPCTRL0 or ADIN(x)-ADIN0. for example: OPCTRL0=>0, ADIN0=>0
		//tmpCmd.au1Data[1]: flag, OPCTRL is 0, ADIN is 1
		//tmpCmd.au1Data[2]: GPIO value (just for output)
		#if UART_DBG
        LogS("t8032 Cmd_GPIOOutput");
		#endif		
		if(tmpCmd.au1Data[1] == 0)			
        	{vCMD_RESONSE(Cmd_GPIOOutput, GPIO_SetOut(tmpCmd.au1Data[0]+200,tmpCmd.au1Data[2]), 0, 0);}
		else
			{vCMD_RESONSE(Cmd_GPIOOutput, GPIO_SetOut(tmpCmd.au1Data[0]+400,tmpCmd.au1Data[2]), 0, 0);}
	}
	
	else if(tmpCmd.u1Cmd == Cmd_GPIOInput)
	{
		#if UART_DBG
        LogS("t8032 Cmd_GPIOInput");
		#endif
		if(tmpCmd.au1Data[1] == 0)			
        	{vCMD_RESONSE(Cmd_GPIOInput, GPIO_GetIn(tmpCmd.au1Data[0]+200), 0, 0);}
		else
        	{vCMD_RESONSE(Cmd_GPIOInput, GPIO_GetIn(tmpCmd.au1Data[0]+400), 0, 0);}
	}
#endif	

#if SUPPORT_ERROR_LED_CTRL    
    else if(tmpCmd.u1Cmd == Cmd_SetClearErrCode)
    {
        #if UART_DBG
        LogS("Cmd_SetClearErrCode");
        #endif
        gErrCodeCtrl.fgErrCodeLedType = tmpCmd.u1SubCmd;
		if(gErrCodeCtrl.fgErrCodeLedType)
		{
	        gErrCodeCtrl.fgErrCodeLedGpioIdx = tmpCmd.au1Data[0] + SERVO(0);
		}
		else
		{
			gErrCodeCtrl.fgErrCodeLedGpioIdx = tmpCmd.au1Data[0] + OPCTRL(0);
		}
			
        gErrCodeCtrl.fgErrCodeLedPolarity =  tmpCmd.au1Data[1];
        gErrCodeCtrl.fgErrCodeLedPeriod =  tmpCmd.au1Data[2];
        gErrCodeCtrl.fgErrCodeLedInterval =  tmpCmd.au1Data[3];
/*		
        if(gErrCodeCtrl.fgErrCodeLedInterval > gErrCodeCtrl.fgErrCodeLedPeriod)
        {
            gErrCodeCtrl.fgErrCodeLedInterval -= gErrCodeCtrl.fgErrCodeLedPeriod;        
        }
*/        
        #if UART_DBG
        LogSB("set fgErrCodeLedType", gErrCodeCtrl.fgErrCodeLedType);
        LogSW("set fgErrCodeLedGpioIdx", gErrCodeCtrl.fgErrCodeLedGpioIdx);
        LogSB("set fgErrCodeLedPolarity", gErrCodeCtrl.fgErrCodeLedPolarity);
        LogSB("set fgErrCodeLedPeriod", gErrCodeCtrl.fgErrCodeLedPeriod);                
        LogSB("set fgErrCodeLedInterval", gErrCodeCtrl.fgErrCodeLedInterval);                                
        #endif
        vCMD_RESONSE(Cmd_SetClearErrCode, 0, 0, 0);
    }
    else if(tmpCmd.u1Cmd == Cmd_EnableDisableErrCode)
    {
        gErrCodeCtrl.fgErrCodeBlinkingTime = tmpCmd.u1SubCmd;
		gErrCodeCtrl.fgSonyErrLedStateMachine = SONY_ERROR_CODE_STATE_INIT;   
        vSonyErrCodeLedReset();                
//        SetCtrlLedEnable();        
        #if UART_DBG
        LogSB("set fgErrCodeLedType", gErrCodeCtrl.fgErrCodeLedType);
        LogSW("set fgErrCodeLedGpioIdx", gErrCodeCtrl.fgErrCodeLedGpioIdx);
        LogSB("set fgErrCodeLedPolarity", gErrCodeCtrl.fgErrCodeLedPolarity);
        LogSB("set fgErrCodeLedPeriod", gErrCodeCtrl.fgErrCodeLedPeriod);                
        LogSB("set fgErrCodeLedInterval", gErrCodeCtrl.fgErrCodeLedInterval);                                
        LogSB("Cmd_EnableDisableErrCode", gErrCodeCtrl.fgErrCodeBlinkingTime );
        #endif        
        vCMD_RESONSE(Cmd_EnableDisableErrCode, 0, 0, 0);
    }
#endif 

#if DISABLE_WAKEUP_SOMETIME
	else if(tmpCmd.u1Cmd == Cmd_SetDisableWakeupTime)
	{
		gDelayWakeupCtrl.u1DelayTimeCnt = tmpCmd.u1SubCmd;
		gDelayWakeupCtrl.u1Enable = (gDelayWakeupCtrl.u1DelayTimeCnt) ? 1 : 0;		
		gDelayWakeupCtrl.u1WakeupEnReg0 = tmpCmd.au1Data[0];
		gDelayWakeupCtrl.u1WakeupEnReg1 = tmpCmd.au1Data[1];
		gDelayWakeupCtrl.u1WakeupEnReg2 = tmpCmd.au1Data[2];
		gDelayWakeupCtrl.u1WakeupEnReg3 = tmpCmd.au1Data[3];		
		#if UART_DBG
		LogSB("Cmd_SetDisableWakeupTime", gDelayWakeupCtrl.u1DelayTimeCnt);
		#endif		  
		if(gDelayWakeupCtrl.u1Enable)
		{//ARM should be set PDWNC_WAKEN register as 0 before enter standby!
			if(gDelayWakeupCtrl.u1WakeupEnReg0 + gDelayWakeupCtrl.u1WakeupEnReg1 + gDelayWakeupCtrl.u1WakeupEnReg2 + gDelayWakeupCtrl.u1WakeupEnReg3)
			{
				vCMD_RESONSE(Cmd_SetDisableWakeupTime, 0, 0, 0);
			}
			else
			{
				//something wrong not respone!!
			}
		}
		else
		{
			vCMD_RESONSE(Cmd_SetDisableWakeupTime, 0, 0, 0);
		}
	}
#endif


#if SIR_TASK
	else if(tmpCmd.u1Cmd == Cmd_SetIRWakeupKEY)
	{
		gIRWakeupKeyList = tmpCmd.u1SubCmd;
		vCMD_RESONSE(Cmd_SetIRWakeupKEY, 0, 0, 0);
	}
#endif	
  
	#if SUSPEND_MODE
    else if(tmpCmd.u1Cmd == Cmd_EnterSuspend)
    {
	#if UART_DBG
        LogS("EnterSuspend");
	#endif		
		PreStandbySta = tmpCmd.u1SubCmd;
		GoingSupend = SUSPEND_STAGE1;
		//vSetTimer(SUSPEND_MODE_TIMER_ID,tmpCmd.au1Data[0]);
        //vCMD_RESONSE(Cmd_EnterSuspend, 0, 0, 0); //entery suspend mode without ack
        #if UART_TASK		
		vRS232CustTaskStandby();
        #endif	        
    }
	#endif
	
    else if(tmpCmd.u1Cmd == Cmd_EnterPowerDown)//the only one command without response to ARM
    {
        #if UART_DBG
            LogS("EnterStandby");
        #endif        
		#if SUSPEND_MODE		
		if(GoingSupend)
		{
			#if UART_DBG
			LogS("Error:already call entery suspend mode");
			#endif
			PreStandbySta = tmpCmd.u1SubCmd;
		}
		else
		#endif
		{
			MSET_SYS_PRE_STANDBY_STATUS(tmpCmd.u1SubCmd);
			vEnterPowerStandby();
		}
                #if UART_TASK		
		vRS232CustTaskStandby();
                #endif	
    }
    else if(tmpCmd.u1Cmd == Cmd_EnterPowerNormal)
    {
        if(tmpCmd.u1SubCmd == Cmd_Sub_PowerStage_loader)
        {
            #if CEC_log
                LogS("Loader mode \n");
            #endif	
            MSET_SYS_STATE(LOADER_STATE);
        }
        else if(tmpCmd.u1SubCmd==Cmd_Sub_PowerStage_normal)
        {
            #if CEC_log
            LogS("Normal mode \n");
            #endif	
            MSET_SYS_STATE(NORMAL_POWER_STATE);
        }
/*        
        #if T8032_OCCUPIED_UART0_AT_NORMAL_POWER        
            vCMD_RESONSE(Cmd_EnterPowerNormal, 0, 0, 1);
        #else
            vCMD_RESONSE(Cmd_EnterPowerNormal, 0, 0, 0);
        #endif
        vWakeUpSystemTasks();
*/
        vWakeUpSystemTasks();        
#if UART_TASK		
		vRS232CustTaskWakeup();
#endif

        //vRegWrite1B(PDWNC_BCR, u1RegRead1B(PDWNC_BCR)|0xC0); //flush RX/TX buffer;        
        MUART_FLUSH_TX_BUF();
        MUART_FLUSH_RX_BUF();
        #if T8032_OCCUPIED_UART0_AT_NORMAL_POWER        
            vCMD_RESONSE(Cmd_EnterPowerNormal, 0, 0, 1);
        #else
            #if UART_DBG    
                    LogS("SWITCH_ARM");
            #endif         
            vCMD_RESONSE(Cmd_EnterPowerNormal, 0, 0, 0);
        #endif        
    }
    else if(tmpCmd.u1Cmd == Cmd_QuerySwVersion)
    {
            #if UART_DBG
                LogSB("QuerySWversion", FW_VERSION);
            #endif   
            
        vCMD_RESONSE(Cmd_QuerySwVersion, FW_VERSION, 0, 0);
    }
    else if(tmpCmd.u1Cmd == Cmd_EnterXdataSwitch)
    {
        #if UART_DBG
                LogS("xdata :ARM");
            #endif       
//        MSET_XDATA_SWITCH_REQ_FLAG(NEW_EVENT);
        MDisableINT();//disable all interrupt
        EWDI = 0; //disable watch dog interrupt
//        EWT = 0;//disable watch dog reset

        vCMD_RESONSE(Cmd_EnterXdataSwitch, 0, 0, 0);//While arm get this respond, xdata will be switched to ARM right away
        while(MARM_CONTROL_XDATA()); //trap here until arm return access right of xdata
        MEnableINT();
        EWDI = 1; //restore watch dog interrupt
        #if UART_DBG
                LogS("xdata: uP");
        #endif       
        
    }    
#if RC56_IR_DRIVER
    else if(tmpCmd.u1Cmd == Cmd_SetClrIR_FILTER)
    {
        if(tmpCmd.u1SubCmd == Cmd_Sub_SetIR_FilterSysCode)
        {
            gLPIR.fgIR_FilterFlag = XferToColorCfg(tmpCmd.au1Data[0]);//			fgIR_FilterFlag |= XferToColorCfg((UINT8)(u4DataFromARM & 0x000000ff));
            #if UART_DBG
            LogSB("SetColor",GetColorIdx(gLPIR.fgIR_FilterFlag));            
            #endif
        }
        else if(tmpCmd.u1SubCmd==Cmd_Sub_ClrIR_FilterSysCode)
        {
            gLPIR.fgIR_FilterFlag=IR_BLUE_CFG;
#if UART_DBG
            LogS("ClrColor");            
#endif
            
        }
        vCMD_RESONSE(Cmd_SetClrIR_FILTER, 0, 0, 0);
     }
#endif    
    else if(tmpCmd.u1Cmd == Cmd_SetLightGuideLevel)
    {
    }  
    else if(tmpCmd.u1Cmd == Cmd_AliveCheck)
    {

    }
    else if(tmpCmd.u1Cmd == Cmd_SetClearErrCode)
    {

    }
    else if(tmpCmd.u1Cmd == Cmd_EnableDisableErrCode)
    {

    }
//#if ENABLE_CBUS
else if(tmpCmd.u1Cmd == Cmd_Cbus_SetT8032CtlCbus)
{
#if ENABLE_CBUS
	 fgCbus8032Ctrl = tmpCmd.au1Data[0];
#endif
     vCMD_RESONSE(Cmd_ResponseCbus_SetT8032CtlCbus, 0, 0, 0);	
	
}	  
//#endif
#if ENABLE_CEC    
    else if(tmpCmd.u1Cmd == Cmd_CEC_MenuLanguage)
    {
        u1CecMenuLanguage[0] = tmpCmd.au1Data[3];
        u1CecMenuLanguage[1] = tmpCmd.au1Data[2];
        u1CecMenuLanguage[2] = tmpCmd.au1Data[1];
        #if UART_DBG
                LogS("CECMENU");
                Log4B(0x55, u1CecMenuLanguage[2] , u1CecMenuLanguage[1] , u1CecMenuLanguage[0] );
        #endif       
        
        CECMenuLanguageOK();
    }
    else if(tmpCmd.u1Cmd == Cmd_CEC_EnableCEC)
    {
        if(tmpCmd.au1Data[0] == 0 && tmpCmd.au1Data[1] == 0 && tmpCmd.au1Data[2] == 0 && tmpCmd.au1Data[3] == 0)
        {
            fgCecEnRx = 0;
        }
        else
        {
        	if(tmpCmd.au1Data[0] == 2)//ARC Only, Disable OTP
        	{
				u1OtpFlag = 0;
			}
			else
			{
				u1OtpFlag = 1;
			}
			fgCecEnRx = 1;
        }
        #if UART_DBG
                LogSB("set fgEnCEC", fgCecEnRx);
        #endif
        CECEnableCECOK();        
    }
    else if(tmpCmd.u1Cmd == Cmd_CEC_EnableOneTouch)
    {
        if(tmpCmd.au1Data[0] == 0 && tmpCmd.au1Data[1] == 0 && tmpCmd.au1Data[2] == 0 && tmpCmd.au1Data[3] == 0)
        {
            fgCecEnOneTouchPlay = 0;
        }
        else
        {
            fgCecEnOneTouchPlay = 1;
        }
        #if UART_DBG
                LogSB("set fgEnOneTouchPlay", fgCecEnOneTouchPlay);
        #endif

        CECEnableOneTouchPlayOK();
    }        
#endif    
    else if(tmpCmd.u1Cmd == Cmd_SetLEDBlink)
    {

    }
    else if(tmpCmd.u1Cmd == Cmd_SetFWUpgrate)
    {
        MSET_SYS_FW_UPGRADING(tmpCmd.u1SubCmd);
        vCMD_RESONSE(Cmd_SetFWUpgrate, 0, 0, 0);
    }
    else if(tmpCmd.u1Cmd == Cmd_SetBaudrate)
    {
        MSET_SYS_UART_BAUDRATE(tmpCmd.u1SubCmd);
        vCMD_RESONSE(Cmd_SetBaudrate, 0, 0, 0);
    }
#if SUPORT_COLD_REBOOT
    else if(tmpCmd.u1Cmd == Cmd_ColdReboot)
    {
        if(tmpCmd.u1SubCmd == 0)
        {//force set a timer
            vSetTimer(COLD_REBOOT_TIMER_ID, 1);
        }
        else
        {
            vSetTimer(COLD_REBOOT_TIMER_ID, tmpCmd.u1SubCmd);
        }
        u1ColdRebootChk = 1;
        vEnterPowerStandby();   
    }
#endif
#if CUS_SH_HOTEL_MODE 
    else if(tmpCmd.u1Cmd == PDWNC_CMD_ARMT8032_CUST_SH_CUSTDATA  )
    {
        fgCust_SH_HotelModeUartWakeupEn = tmpCmd.u1SubCmd;    
        vCMD_RESONSE(PDWNC_CMD_ARMT8032_CUST_SH_CUSTDATA, 0, 0, 0);    
    }
#endif
    else if(tmpCmd.u1Cmd == Cmd_DigitKeyWakeup)
    {

    }
#if S_HOTEL_MODE_COMMAND
    else if(tmpCmd.u1Cmd == Cmd_SetCustData)
    {
		vSetToggleLedDisable();
		SYS_PrintString(1, "gLedToggleCtrl");
		SYS_PrintString_B(1, "u1SubCmd", tmpCmd.u1SubCmd);
		gLedToggleCtrl.u1LedType = ((tmpCmd.u1SubCmd) & 0x80) ? 1 : 0;
		SYS_PrintString_B(1, "u1LedType", gLedToggleCtrl.u1LedType);		
		gLedToggleCtrl.u1LedPolarity = ((tmpCmd.u1SubCmd) & 0x40) ? 1 : 0;
		SYS_PrintString_B(1, "u1LedPolarity", gLedToggleCtrl.u1LedPolarity);				
   		if(gLedToggleCtrl.u1LedType)
		{
	        gLedToggleCtrl.u2LedGpio  = ((tmpCmd.u1SubCmd) & 0x3f) + SERVO(0);
		}
		else
		{
			gLedToggleCtrl.u2LedGpio  = ((tmpCmd.u1SubCmd) & 0x3f) + OPCTRL(0);
		}
		SYS_PrintString_W(1, "u2LedGpio", gLedToggleCtrl.u2LedGpio);				

		u1CustomData3_Module1ReplyData = ((tmpCmd.au1Data[0]) & 0x1) ? 1 : 0;
		u1Module2Enable = ((tmpCmd.au1Data[0]) & 0x2) ? 1 : 0;
		u1hotel_module2_init_mode = ((tmpCmd.au1Data[0]) & 0x4) ? 1 : 0;
		u1HotelDbg = ((tmpCmd.au1Data[0]) & 0x8) ? 1 : 0;
		u1CustomData1_HotelModule = (tmpCmd.au1Data[0] & 0xf0) >> 4;

		u1Module2AvSelectPW = tmpCmd.au1Data[1];
        u1Module2AvSelectSrc = tmpCmd.au1Data[3];
		u1Module2HdmiPortSel = tmpCmd.au1Data[2];
	
		vCMD_RESONSE(Cmd_SetCustData, 0, 0, 0);
	}
	else if(tmpCmd.u1Cmd == Cmd_GetCustData)
	{
		vCMD_RESONSE(Cmd_GetCustData, bModule2Status, 0, 0);			
    }
#endif

    else if(tmpCmd.u1Cmd == Cmd_QueryServiceMode)
    {

    }
#if ENABLE_CEC    
    else if(tmpCmd.u1Cmd == Cmd_CEC_SupportUserControlPressed)
    {

    }
    else if(tmpCmd.u1Cmd == Cmd_CEC_SetVendorID)
    {
        #if UART_DBG
                LogS("Cmd_CEC_SetVendorID");
        #endif         
        u1CecVendorID[0] = tmpCmd.au1Data[0];
        u1CecVendorID[1] = tmpCmd.au1Data[1];
        u1CecVendorID[2] = tmpCmd.au1Data[2];
        CECSetVendorIDOK();
    }
    else if(tmpCmd.u1Cmd == Cmd_CEC_SetVersion)
    {
        #if UART_DBG
                LogS("Cmd_CEC_SetVersion");
        #endif         

        u1CecVersion = tmpCmd.au1Data[0]; 
        CECSetVersionOK();
    }
    else if(tmpCmd.u1Cmd == Cmd_CEC_SetT8032CtlCEC)
    {
         fgCec8032Ctrl = tmpCmd.au1Data[0];
         CECT8032CtlCECOK();
    }     
 #endif

 #ifdef CEC_FEATURE_3_1     
    else if(tmpCmd.u1Cmd == Cmd_CEC_SetOSDName1)
    {
        _rCECOsdName.u1Size = tmpCmd.au1Data[0];
        _rCECOsdName.au1OsdName[0] = tmpCmd.au1Data[1];
        _rCECOsdName.au1OsdName[1] = tmpCmd.au1Data[2];
        _rCECOsdName.au1OsdName[2] = tmpCmd.au1Data[3];
         CECSetOSDName1OK();
    }
    else if(tmpCmd.u1Cmd == Cmd_CEC_SetOSDName2)
    {
        _rCECOsdName.au1OsdName[3] = tmpCmd.au1Data[0];
        _rCECOsdName.au1OsdName[4] = tmpCmd.au1Data[1];
        _rCECOsdName.au1OsdName[5] = tmpCmd.au1Data[2];
        _rCECOsdName.au1OsdName[6] = tmpCmd.au1Data[3];
         CECSetOSDName2OK();
    }
    else if(tmpCmd.u1Cmd == Cmd_CEC_SetOSDName3)
    {
        _rCECOsdName.au1OsdName[7] = tmpCmd.au1Data[0];
        _rCECOsdName.au1OsdName[8] = tmpCmd.au1Data[1];
        _rCECOsdName.au1OsdName[9] = tmpCmd.au1Data[2];
        _rCECOsdName.au1OsdName[10] = tmpCmd.au1Data[3];
         CECSetOSDName3OK();
    }
    else if(tmpCmd.u1Cmd == Cmd_CEC_SetOSDName4)
    {
        _rCECOsdName.au1OsdName[11] = tmpCmd.au1Data[0];
        _rCECOsdName.au1OsdName[12] = tmpCmd.au1Data[1];
        _rCECOsdName.au1OsdName[13] = tmpCmd.au1Data[2];
         CECSetOSDName4OK();

    }    
    else if(tmpCmd.u1Cmd == Cmd_CEC_SetWakeupOpCode1)
    {
        _rCECWkpOpCode.u1Size = tmpCmd.au1Data[0];
        _rCECWkpOpCode.au1WkpCode[0] =  tmpCmd.au1Data[1];
        _rCECWkpOpCode.au1WkpCode[1] =  tmpCmd.au1Data[2];
        _rCECWkpOpCode.au1WkpCode[2] =  tmpCmd.au1Data[3];
        CECSetWkpCode1OK();
    }
    else if(tmpCmd.u1Cmd == Cmd_CEC_SetWakeupOpCode2)
    {
        _rCECWkpOpCode.au1WkpCode[3] = tmpCmd.au1Data[0];
        _rCECWkpOpCode.au1WkpCode[4] = tmpCmd.au1Data[1];
        _rCECWkpOpCode.au1WkpCode[5] = tmpCmd.au1Data[2];
        _rCECWkpOpCode.au1WkpCode[6] = tmpCmd.au1Data[3];
        CECSetWkpCode2OK();
    }
    else if(tmpCmd.u1Cmd == Cmd_CEC_SetWakeupOpCode3)
    {
        _rCECWkpOpCode.au1WkpCode[7] = tmpCmd.au1Data[0];
        _rCECWkpOpCode.au1WkpCode[8] = tmpCmd.au1Data[1];
        _rCECWkpOpCode.au1WkpCode[9] = tmpCmd.au1Data[2];
        _rCECWkpOpCode.au1WkpCode[10] = tmpCmd.au1Data[3];
        CECSetWkpCode3OK();
    } 
#endif    
    else if(tmpCmd.u1Cmd == Cmd_OtherEngineSwitch)
    {
        vCMD_RESONSE(Cmd_OtherEngineSwitch, 0, 0, 0);
    } 
#if SIR_TASK
#if QUERY_IR_POWER_KEY_STATE_SUPPORT	
    else if(tmpCmd.u1Cmd == Cmd_QueryIRPowerKeyState)
    {
	    gSIRC_ctrl._gu1ARM_IR_WORKING = 1;
		MIR_DIS_INT();//disable IR interrupt
		vCMD_RESONSE(Cmd_QueryIRPowerKeyState, bGetTimer(SIR_LONG_PRESSING_CHK_TIMER_ID), 0, 0);
    }
#endif
#endif
/*    
    else if(tmpCmd.u1Cmd == Cmd_QueryIRPowerKeyState)
    {

    }
*/    
    else
    {
#if UART_DBG
                LogSB("Not support command",  tmpCmd.u1Cmd);
#endif
    }
    
}
