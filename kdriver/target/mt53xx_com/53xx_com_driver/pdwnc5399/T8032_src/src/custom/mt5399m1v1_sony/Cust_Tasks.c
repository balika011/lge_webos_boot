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
#include "general.h"
//#include "drv_hdmicec.h"
void vWDTRebootProcess(void)
{
     LogS("  WDT");
 }

#if S_HOTEL_MODE_COMMAND
UINT8 data u1HotelModule2OnLine;
UINT8 data u1HotelModeModule;
#endif

#if SIR_TASK
UINT8 data gIRWakeupKeyList;
SIRC_CTRL_T idata gSIRC_ctrl;
SIRC_ServiceMode_Ctrl gSIRC_ServiceMode;
UINT16 SIRC_TogglePeriod[VALID_OIRI_TOGGLE_CNT];
#define vSIR_FrameTimerReset() vSetTimer(SIR_FRAME_WAIT_TIMER_ID, 0)
#define CheckSIRC_BitCntValid()   (/*(_MGetIRBitCnt() == 15) ||*/ (_MGetIRBitCnt() == 12)) ? 1 : 0
//#define CheckSIRC_Category0()	(_ReadIrDataReg() & 0x0000ff00)? 0 : 1
//#define CheckSIRC_DATV_BTN()  ((_ReadIrDataReg() & 0x0000ffff) == 0x3b8d)? 1 : 0

BIT _SonyIrDataCorrection(IR_DATA_TYPE irdata) small
{
    SYS_PrintString_DW(1, "IrDataCorrection", _ReadIrDataReg());
    SYS_PrintString_B(1, "bitcnt", _MGetIRBitCnt());
    SYS_PrintString_B(1, "irdata", irdata);    
/*
    if((irdata == SIR_KEY_ADTV_SW) && (_MGetIRBitCnt() == 15))
    {
        SYS_PrintString(1, "DataCorrection pass1");
        return 1;
    }
*/    
	if(CheckSIRC_BitCntValid() || ((gIRWakeupKeyList & T8032_IR_WAK_ADTV_SW) && (irdata == SIR_KEY_ADTV_SW) && (_MGetIRBitCnt() == 15)))
	{
		SYS_PrintString(1, "DataCorrection pass2");
		return 1;
	}
	else
    {
        SYS_PrintString(1, "bincnt invalid");
		#if SONY_IR_DBG
		LogSD("e:BitCnt2", RegReadFldAlign(PDWNC_IRCNT, FLD_BIT_CNT)); //sony ir debug
		#endif 
        return 0; // to ignore
    }
}


#if DISABLE_WAKEUP_SOMETIME
Delay_Wakeup_Ctrl gDelayWakeupCtrl;
#endif

#if DISABLE_WAKEUP_SOMETIME
void vTask7SIR_Task_StateMachine(void)
{
	if(MGET_ARM_POWER_STATE() == ARM_POWER_OFF)
	{
		if((gDelayWakeupCtrl.u1Enable) && (fgIsTimerUp(DISABLE_WAKEUP_TIMER)))
		{
#if S_HOTEL_MODE_COMMAND
			if((u1HotelModeModule == MODULE2) && (u1HotelModule2OnLine))
			{
				if(
					(u1RegRead1B(PDWNC_WAKEN) != gDelayWakeupCtrl.u1WakeupEnReg0) ||
/*					(u1RegRead1B(PDWNC_WAKEN + 1) != gDelayWakeupCtrl.u1WakeupEnReg1) ||*/
					(u1RegRead1B(PDWNC_WAKEN + 2) != gDelayWakeupCtrl.u1WakeupEnReg2) ||
					(u1RegRead1B(PDWNC_WAKEN + 3) != gDelayWakeupCtrl.u1WakeupEnReg3)
				)
				{
					SYS_PrintString(1, "WAKEN timeout!");
					SYS_PrintString_DW(1, "now waken reg", u4IO32Read4B(PDWNC_WAKEN));				
					vIO32Write1B(PDWNC_WAKEN, gDelayWakeupCtrl.u1WakeupEnReg0);
/*					vIO32Write1B(PDWNC_WAKEN + 1, gDelayWakeupCtrl.u1WakeupEnReg1);*/
					vIO32Write1B(PDWNC_WAKEN + 2, gDelayWakeupCtrl.u1WakeupEnReg2);
					vIO32Write1B(PDWNC_WAKEN + 3, gDelayWakeupCtrl.u1WakeupEnReg3);			
					SYS_PrintString_DW(1, "restore waken reg", u4IO32Read4B(PDWNC_WAKEN));
				}			
				return ;
			}
#endif		
			
			if(
				(u1RegRead1B(PDWNC_WAKEN) != gDelayWakeupCtrl.u1WakeupEnReg0) ||
				(u1RegRead1B(PDWNC_WAKEN + 1) != gDelayWakeupCtrl.u1WakeupEnReg1) ||
				(u1RegRead1B(PDWNC_WAKEN + 2) != gDelayWakeupCtrl.u1WakeupEnReg2) ||
				(u1RegRead1B(PDWNC_WAKEN + 3) != gDelayWakeupCtrl.u1WakeupEnReg3)
			)
			{
				SYS_PrintString(1, "WAKEN timeout!");
				SYS_PrintString_DW(1, "now waken reg", u4IO32Read4B(PDWNC_WAKEN));				
				vIO32Write1B(PDWNC_WAKEN, gDelayWakeupCtrl.u1WakeupEnReg0);
				vIO32Write1B(PDWNC_WAKEN + 1, gDelayWakeupCtrl.u1WakeupEnReg1);
				vIO32Write1B(PDWNC_WAKEN + 2, gDelayWakeupCtrl.u1WakeupEnReg2);
				vIO32Write1B(PDWNC_WAKEN + 3, gDelayWakeupCtrl.u1WakeupEnReg3);			
				SYS_PrintString_DW(1, "restore waken reg", u4IO32Read4B(PDWNC_WAKEN));
			}
		}
	}
}
#endif	


void vTask7SIR_Task_WakeupProcess(void)
{
#if QUERY_IR_POWER_KEY_STATE_SUPPORT    
	//david: do not disable IR interrupt for monitoring if user is keeping pressing
#else
		#if SONY_IR_DBG //SONY_IR_BEHAVIOR_DBG
			SYS_PrintString(1, "DI"); //sony ir debug	   
    	#endif 
		MIR_DIS_INT();//disable IR interrupt
#endif
#if DISABLE_WAKEUP_SOMETIME
	memset(&(gDelayWakeupCtrl), 0, sizeof(gDelayWakeupCtrl));
#endif

}
void vTask7SIR_Task_StandbyProcess(void)
{//if IR_TASK is defined as 1, MIR_EN_INT() has already executed in system standby process
//	MIR_EN_INT();
	memset(&(gSIRC_ctrl), 0, sizeof(gSIRC_ctrl));
	gSIRC_ctrl.u2PreIrRxData0 = NO_SIRC_DATA;

	memset(&(gSIRC_ServiceMode), 0, sizeof(gSIRC_ServiceMode));
	gSIRC_ServiceMode.PrevEvent	= NO_SIRC_DATA;
#if QUERY_IR_POWER_KEY_STATE_SUPPORT 
	vSetTimer(SIR_LONG_PRESSING_CHK_TIMER_ID, 0);
	gSIRC_ctrl._gu1ARM_IR_WORKING = 0;
#endif
	vSIR_FrameTimerReset();
#if SONY_IR_DBG
	SetGPIOEN(0x10);
#endif
#if DISABLE_WAKEUP_SOMETIME
	if(gDelayWakeupCtrl.u1Enable)
	{
		if(gDelayWakeupCtrl.u1WakeupEnReg0 + gDelayWakeupCtrl.u1WakeupEnReg1 + gDelayWakeupCtrl.u1WakeupEnReg2 + gDelayWakeupCtrl.u1WakeupEnReg3)
		{
//			vRegWrite4B(PDWNC_WAKEN, 0); //ARM should set PDWNC_WAKEN register as 0 before enter standby!
			vSetTimer(DISABLE_WAKEUP_TIMER, gDelayWakeupCtrl.u1DelayTimeCnt);
		}
		else
		{
			//something wrong!!!
		}
	}
	SYS_PrintString_B(1, "gDelayWakeupCtrl",gDelayWakeupCtrl.u1Enable);
	SYS_PrintString_B(1, "u1DelayTimeCnt",gDelayWakeupCtrl.u1DelayTimeCnt);
	SYS_PrintString_B(1, "u1WakeupEnReg0",gDelayWakeupCtrl.u1WakeupEnReg0);
	SYS_PrintString_B(1, "u1WakeupEnReg1",gDelayWakeupCtrl.u1WakeupEnReg1);
	SYS_PrintString_B(1, "u1WakeupEnReg2",gDelayWakeupCtrl.u1WakeupEnReg2);
	SYS_PrintString_B(1, "u1WakeupEnReg3",gDelayWakeupCtrl.u1WakeupEnReg3);	
	SYS_PrintString_DW(1, "waken reg", u4IO32Read4B(PDWNC_WAKEN));	
    SYS_PrintString_B(1, "WakeupKeyList", gIRWakeupKeyList);    
#endif

}

void vTask7SIR_Task_Init(void)
{
	vSIR_FrameTimerReset();
	
	memset(&(gSIRC_ctrl), 0, sizeof(gSIRC_ctrl));
	gSIRC_ctrl.u2PreIrRxData0 = NO_SIRC_DATA;
	
	memset(&(gSIRC_ServiceMode), 0, sizeof(gSIRC_ServiceMode));
	gSIRC_ServiceMode.PrevEvent = NO_SIRC_DATA;
#if QUERY_IR_POWER_KEY_STATE_SUPPORT 
	vSetTimer(SIR_LONG_PRESSING_CHK_TIMER_ID, 0);
	gSIRC_ctrl._gu1ARM_IR_WORKING = 1;
#endif
    gIRWakeupKeyList = T8032_IR_WAK_POWER;
#if SONY_IR_DBG
	gSIRC_ctrl.u1DeepDbg = 0;
#endif	
#if DISABLE_WAKEUP_SOMETIME
	memset(&(gDelayWakeupCtrl), 0, sizeof(gDelayWakeupCtrl));
#endif

}


UINT8 IsSonyPowerKey(IR_DATA_TYPE data u2key)
{
    SYS_PrintString_W(1, "IsSonyPowerKey", u2key);
    SYS_PrintString_B(1, "WakeupKeyList", gIRWakeupKeyList);
#if S_HOTEL_MODE_COMMAND
	if((u1HotelModeModule == MODULE2) && (u1HotelModule2OnLine))
	{
		return 0;
	}
#endif

#if 0
		if(u1key == SIR_KEY_POWER)
			return 1;
#if DISABLE_WAKUP_1_SECOND//rafflesia only
		else if((u1Region != LATIN) && (u1key == SIR_KEY_TV))
			return 1;
		else if((u1Region != LATIN) && ((u1key == SIR_KEY_PRG_UP) || (u1key == SIR_KEY_PRG_DOWN)))
			return 1;
#endif
		else if(fgPowerOff == ARM_POWER_OFF && u1key == 0xAE)
			return 1;
		else if(fgPowerOff == ARM_POWER_ON && u1key == 0xAF)
			return 1;
		
			return 0;
#else
	if(((u2key == SIR_KEY_POWER || u2key == SIR_KEY_POWER_ON)) && (gIRWakeupKeyList & T8032_IR_WAK_POWER))
		return 1;
	else if(((u2key == SIR_KEY_PRG_UP) || (u2key == SIR_KEY_PRG_DOWN)) && (gIRWakeupKeyList & T8032_IR_WAK_CH_UP_DOWN))
	{
		return 1;
	}
	else if(((u2key >= SIR_KEY_1) && (u2key <= SIR_KEY_0)) && (gIRWakeupKeyList & T8032_IR_WAK_NUMERIC))
	{/* 20101015, add u1WakeUp_waiting_idx = 2, since this idx will be changed to 2 while the 1st '5' comming and system will wakeup by 2nd '5' (5_) */
	    if(((gSIRC_ServiceMode.u1WakeUp_waiting_idx == 1) || (gSIRC_ServiceMode.u1WakeUp_waiting_idx == 2)) && (u2key == SIR_KEY_DIGIT5))
	    {
		#if SONY_IR_DBG //SONY_IR_BEHAVIOR_DBG
			SYS_PrintString(1, "D5"); //sony ir debug	   
    	#endif		
	        return 0;
	    }
	    else
		{
#if SONY_IR_DBG //SONY_IR_BEHAVIOR_DBG
			SYS_PrintString(1, "5_"); //sony ir debug    
#endif		
		    return 1;
	    }
	}
	else if((u2key == SIR_KEY_DTV) && (gIRWakeupKeyList & T8032_IR_WAK_DTV))
		return 1;
	else if((u2key == SIR_KEY_ATV) && (gIRWakeupKeyList & T8032_IR_WAK_ATV))
		return 1;
	else if((u2key == SIR_KEY_JUMP) && (gIRWakeupKeyList & T8032_IR_WAK_JUMP))
		return 1;
	else if((u2key == SIR_KEY_ADTV_SW) && (gIRWakeupKeyList & T8032_IR_WAK_ADTV_SW))
		return 1;
		
	return 0;
#endif
}
void vSIRC_EventProcess(IR_DATA_TYPE irData)
{
    SYS_PrintString_W(0, "vSIRC_Event", irData);
    SYS_PrintString_B(0, "vSIRC_Event delay_wak", gDelayWakeupCtrl.u1Enable);
    SYS_PrintString_B(0, "vSIRC_Event timeup?", fgIsTimerUp(DISABLE_WAKEUP_TIMER));    
// 1. always accept power on event
#if DISABLE_WAKEUP_SOMETIME
	if((gDelayWakeupCtrl.u1Enable == 0) || ((gDelayWakeupCtrl.u1Enable) && (fgIsTimerUp(DISABLE_WAKEUP_TIMER))))
	{
#endif
		if(IsSonyPowerKey(irData))
		{
			#if SONY_IR_DBG //SONY_IR_BEHAVIOR_DBG
				SYS_PrintString_W(0, "Wak", (UINT16)irData); //sony ir debug	   
	    	#endif	
			//vKrDelay10ms(10);
	        vWakeUpSystem(PDWNC_WAKE_UP_REASON_IRRC, (UINT8)irData);
		}
#if DISABLE_WAKEUP_SOMETIME		
	}		
#endif	

// 2. as for the service mode booting,  due to we don't need to handle long press operation in standby mode, we discard the same event.
	if(gSIRC_ServiceMode.PrevEvent == irData)
	{
		#if SONY_IR_DBG //SONY_IR_BEHAVIOR_DBG
			SYS_PrintString(0, "thr"); //sony ir debug	   
    	#endif
		return;
	}	
	gSIRC_ServiceMode.PrevEvent = irData;
	if(irData == SIR_KEY_DISPLAY)
	{
		gSIRC_ServiceMode.u1WakeUp_waiting_idx = 1;
		MSET_SYS_SERVICE_MODE(0);
		gSIRC_ServiceMode.fgIsHotelCommand = 0;
		#if SONY_IR_DBG //SONY_IR_BEHAVIOR_DBG
			SYS_PrintString(0, "disp"); //sony ir debug	   
    	#endif		
	}
	else
	{
		switch (gSIRC_ServiceMode.u1WakeUp_waiting_idx)
		{
			case 1:
				if((irData == SIR_KEY_DIGIT5) || (irData == SIR_KEY_TEST) || (irData == SIR_KEY_MUTE))
				{
#if SONY_IR_DBG //SONY_IR_BEHAVIOR_DBG
					SYS_PrintString(1, "Service2"); //sony ir debug	   
#endif		
				   	if((irData == SIR_KEY_TEST) ||(irData == SIR_KEY_MUTE))
				   	{
						gSIRC_ServiceMode.fgIsHotelCommand = 2;
				   	}
				   	else//(irData==SIR_KEY_DIGIT5) 
					{
						gSIRC_ServiceMode.fgIsHotelCommand = 0;
					}
					gSIRC_ServiceMode.u1WakeUp_waiting_idx = 2;
				}
				else
				{
					gSIRC_ServiceMode.u1WakeUp_waiting_idx = 0;
#if SONY_IR_DBG //SONY_IR_BEHAVIOR_DBG
					SYS_PrintString(0, "Ser_res1"); //sony ir debug	   
#endif		
				}
				break;
			case 2:
				if(irData == SIR_KEY_VOL_PLUS)
				{
#if SONY_IR_DBG //SONY_IR_BEHAVIOR_DBG
					SYS_PrintString(0, "Service3"); //sony ir debug	   
#endif		
					gSIRC_ServiceMode.u1WakeUp_waiting_idx = 3;
				}
				else if(irData == SIR_KEY_VOL_MINUS)
				{
#if SONY_IR_DBG //SONY_IR_BEHAVIOR_DBG
					SYS_PrintString(0, "Service3"); //sony ir debug	   
#endif		
					gSIRC_ServiceMode.u1WakeUp_waiting_idx = 4;
				}
				else
				{
#if SONY_IR_DBG //SONY_IR_BEHAVIOR_DBG
					SYS_PrintString(0, "Ser_res2"); //sony ir debug	   
#endif		
					gSIRC_ServiceMode.u1WakeUp_waiting_idx = 0;
				}
				break;
			case 3:
				if(irData == SIR_KEY_POWER)
				{
					MSET_SYS_SERVICE_MODE(1 + gSIRC_ServiceMode.fgIsHotelCommand); //service mode
					#if SONY_IR_DBG//#if 0// SONY_IR_BEHAVIOR_DBG
					SYS_PrintString(0, "_Mode_");
					LogB(MGET_SYS_SERVICE_MODE());
					LogB(gSIRC_ServiceMode.fgIsHotelCommand);
					#endif
				}
				else
				{
#if SONY_IR_DBG //SONY_IR_BEHAVIOR_DBG
					SYS_PrintString(0, "Ser_res3"); //sony ir debug	   
#endif		
				
					MSET_SYS_SERVICE_MODE(0);
					gSIRC_ServiceMode.fgIsHotelCommand = 0;
				}
				gSIRC_ServiceMode.u1WakeUp_waiting_idx = 0;
				break;
			case 4:
				if(irData == SIR_KEY_POWER)
				{
					MSET_SYS_SERVICE_MODE(2 + gSIRC_ServiceMode.fgIsHotelCommand); //self-diagnostic mode
					#if 0// UART_DBG
					vRegWrite1B(PDWNC_DATAW,0xcd); //debug
					#endif							
					#if SONY_IR_DBG//#if 0// SONY_IR_BEHAVIOR_DBG
					SYS_PrintString(0, "Mode");
					LogB(gSIRC_ServiceMode.fgServiceMode);
					#endif
				}
				else
				{
#if SONY_IR_DBG //SONY_IR_BEHAVIOR_DBG
					SYS_PrintString(0, "Ser_res4"); //sony ir debug	   
#endif		
					MSET_SYS_SERVICE_MODE(0);
					gSIRC_ServiceMode.fgIsHotelCommand = 0;
				}
				gSIRC_ServiceMode.u1WakeUp_waiting_idx=0;
				break;
			default:
				gSIRC_ServiceMode.u1WakeUp_waiting_idx=0;
#if SONY_IR_DBG //SONY_IR_BEHAVIOR_DBG
				SYS_PrintString(0, "Ser_res0"); //sony ir debug	   
#endif		
				//david : if power on, these information should be reserved until ARM issues the Cmd_responseServiceMode...
				if(MGET_ARM_POWER_STATE() == ARM_POWER_OFF)
				{
					MSET_SYS_SERVICE_MODE(0);
					gSIRC_ServiceMode.fgIsHotelCommand = 0;
				}
				break;
		}
	}			

}



void vTask7SIR_Task(IR_DATA_TYPE irData)
{
	if(MGET_ARM_POWER_STATE() == ARM_POWER_OFF)
	{
		#if 0//For the newest power state spec
		if(((u4IO32Read4B(PDWNC_WAKEN))& (1<<14))== 0)//Bit 14 is used for record status 0 for no use
		{
			return;
		}
		#endif
		if(_SonyIrDataCorrection(irData) || (gSIRC_ctrl.u1SoftcatDontCareFlag == 1))
		{    /* Validate if 1st frame equals to 2nd frame */
    		SYS_PrintString_W(0, "SIR_Task u2PreIrRxData0", gSIRC_ctrl.u2PreIrRxData0);
			if((gSIRC_ctrl.u2PreIrRxData0 == irData) && (!fgIsTimerUp(SIR_FRAME_WAIT_TIMER_ID)))
			{
				vSIRC_EventProcess(irData);
			}
			gSIRC_ctrl.u2PreIrRxData0 = irData;			
			vSetTimer(SIR_FRAME_WAIT_TIMER_ID, 5);//100ms
		}
	}
	else
	{
#if QUERY_IR_POWER_KEY_STATE_SUPPORT
		if(IsSonyPowerKey(irData))
		{
			vSetTimer(SIR_LONG_PRESSING_CHK_TIMER_ID, 25);
		}
#endif
	}
}

#endif

#if GPIO_INTERRUPT_ENABLE
#define OPCTRL2_INT_ST 0x04
void BTGpioIntWakeup(void)
{
//    vIO32Write4B(PDWNC_T8032_INTEN, u4IO32Read4B(PDWNC_ARM_INTEN) & (~0x04)); //OPCTRL2 enable interrupt
    vWakeUpSystem(PDWNC_WAKE_UP_REASON_KEYPAD, 0);  //SW trigger system wakeup.
}

//0: stb
//1: normal
UINT8 u1MyLedStatus;
void enter_led_state(UINT8 s)
{
    if (0 == s)
    {
        GPIO_SetOut(215, 1);  //LED, green off
        GPIO_SetOut(214, 0);  //LED, red on
        u1MyLedStatus = 0;
    }
    else
    {
        GPIO_SetOut(215, 0);  //LED, green on
        GPIO_SetOut(214, 1);  //LED, red off
        u1MyLedStatus = 1;
    }

}
void vCustGpioIsr(UINT32 u4IntStatus,UINT32 u4IntStatus2)
{
	if(u4IntStatus||u4IntStatus2)
	{
	#if UART_DBG
	LogS("GpioInt");
	LogSD("IntStatus",u4IntStatus);	
	LogSD("IntStatus2",u4IntStatus2);
	LogSD("GPIOIN0",u4RegRead4B(0x84));	
	LogSD("GPIOIN1",u4RegRead4B(0x88));		
	#endif
	}
	if(u4IntStatus & OPCTRL2_INT_ST)
    {
        if (MGET_SYS_STATE () == STANDBY_STATE)
        {
            enter_led_state(1);
            BTGpioIntWakeup();
        }
        else
        {
            if (u1MyLedStatus == 0)  //stb, switch to norm
            {
                enter_led_state(1);
            }
            else  //norm to stb
            {
                enter_led_state(0);
            }
        }
    }
}
#endif

#if RC56_IR_DRIVER
#define MRC56_TOGGLE_BIT_MASK(x) ((x) & 0xffffff00)
#define fgIsTimerAtInitialState(x) fgIsTimerUp(x)
LPIR_struct gLPIR;
UINT8 RC6_B2_KEY[18];/* = {0x80,0xC0,0xA0,0xE0,0x90,0xD0,0xB0,0xF0,0x88,0xC8,0xD8,0x9D,0x98,0xf8,0x82,0xc2,0xa8,0x8e};*/
UINT8 RC6_B1_KEY[4];/* = {0x0, 0x4C, 0x24,0x58};*/
UINT16 RC5_B2B3_KEY[18];/* = {0x0eaa,0x1daa,0x0e6a,0x1d6a,0x0e9a,0x1d9a,0x0e5a,0x1d5a,0x0ea6,0x1da6,0x1d96,0x0eac,0x0e96,0x1d6a,0x0eaa,0x1daa,0x0e6a,0x0ea5};*/
UINT8 RC5_BLUE_B1_KEY[18];/* = {0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xD5,0xAA,0x56,0x6a,0x6a,0x6a,0x6a};*/
UINT8 SYS7_B1_KEY[18];/* = 		{0x95,0x95,0x95,0x95,0x95,0x95,0x95,0x95,0x95,0x95,0x95,0xff,0x95,0xff,0x55,0x55,0x55,0x6a};*/


//David: need to copy DATA segment by ourself...
void InitIR_KeyMap(void)
{		
/*
typedef enum
{
    IR_DIGIT0_IDX = 0,
    IR_DIGIT1_IDX ,    	
    IR_DIGIT2_IDX ,    	
    IR_DIGIT3_IDX ,    	
    IR_DIGIT4_IDX ,    	
    IR_DIGIT5_IDX,
    IR_DIGIT6_IDX ,    	
    IR_DIGIT7_IDX ,    	
    IR_DIGIT8_IDX ,    	
    IR_DIGIT9_IDX,
    IR_MUTE_IDX ,    	
    IR_OK_IDX ,    	
    IR_POWER_IDX ,    	
    IR_AMBILIGHT_IDX,
    IR_CH_UP_IDX ,    	
    IR_CH_DOWN_IDX ,    	
    IR_CH_PREVCH_IDX,
    IR_CH_SOURCE_IDX ==>17    
}CUST_P_IR_KEY_IDX;

*/    
	SYS7_B1_KEY[0] = 0x95;
	SYS7_B1_KEY[1] = 0x95;
	SYS7_B1_KEY[2] = 0x95;
	SYS7_B1_KEY[3] = 0x95;
	SYS7_B1_KEY[4] = 0x95;
	SYS7_B1_KEY[5] = 0x95;
	SYS7_B1_KEY[6] = 0x95;
	SYS7_B1_KEY[7] = 0x95;
	SYS7_B1_KEY[8] = 0x95;
	SYS7_B1_KEY[9] = 0x95;
	SYS7_B1_KEY[10] = 0x95;
	SYS7_B1_KEY[11] = 0xFF;
	SYS7_B1_KEY[12] = 0x95;
	SYS7_B1_KEY[13] = 0xFF;
	SYS7_B1_KEY[14] = 0x55;
	SYS7_B1_KEY[15] = 0x55;
	SYS7_B1_KEY[16] = 0x55;
	SYS7_B1_KEY[17] = 0x6A;
	
	RC5_BLUE_B1_KEY[0] = 0xAA;
	RC5_BLUE_B1_KEY[1] = 0xAA;
	RC5_BLUE_B1_KEY[2] = 0xAA;
	RC5_BLUE_B1_KEY[3] = 0xAA;
	RC5_BLUE_B1_KEY[4] = 0xAA;
	RC5_BLUE_B1_KEY[5] = 0xAA;
	RC5_BLUE_B1_KEY[6] = 0xAA;
	RC5_BLUE_B1_KEY[7] = 0xAA;
	RC5_BLUE_B1_KEY[8] = 0xAA;
	RC5_BLUE_B1_KEY[9] = 0xAA;
	RC5_BLUE_B1_KEY[10] = 0xAA;
	RC5_BLUE_B1_KEY[11] = 0xD5;
	RC5_BLUE_B1_KEY[12] = 0xAA;
	RC5_BLUE_B1_KEY[13] = 0x56;
	RC5_BLUE_B1_KEY[14] = 0x6A;
	RC5_BLUE_B1_KEY[15] = 0x6A;
	RC5_BLUE_B1_KEY[16] = 0x6A;
	RC5_BLUE_B1_KEY[17] = 0x6A;

	RC5_B2B3_KEY[0] = 0x0EAA;
	RC5_B2B3_KEY[1] = 0x1DAA;
	RC5_B2B3_KEY[2] = 0x0E6A;
	RC5_B2B3_KEY[3] = 0x1D6A;
	RC5_B2B3_KEY[4] = 0x0E9A;
	RC5_B2B3_KEY[5] = 0x1D9A;
	RC5_B2B3_KEY[6] = 0x0E5A;
	RC5_B2B3_KEY[7] = 0x1D5A;
	RC5_B2B3_KEY[8] = 0x0EA6;
	RC5_B2B3_KEY[9] = 0x1DA6;
	RC5_B2B3_KEY[10] = 0x1D96;
	RC5_B2B3_KEY[11] = 0x0EAC;
	RC5_B2B3_KEY[12] = 0x0E96;
	RC5_B2B3_KEY[13] = 0x1D6A;
	RC5_B2B3_KEY[14] = 0x0EAA;
	RC5_B2B3_KEY[15] = 0x1DAA;
	RC5_B2B3_KEY[16] = 0x0E6A;	
	RC5_B2B3_KEY[17] = 0x0EA5;

	RC6_B2_KEY[0] = 0x80;
	RC6_B2_KEY[1] = 0xC0;
	RC6_B2_KEY[2] = 0xA0;
	RC6_B2_KEY[3] = 0xE0;
	RC6_B2_KEY[4] = 0x90;
	RC6_B2_KEY[5] = 0xD0;
	RC6_B2_KEY[6] = 0xB0;
	RC6_B2_KEY[7] = 0xF0;
	RC6_B2_KEY[8] = 0x88;
	RC6_B2_KEY[9] = 0xC8;
	RC6_B2_KEY[10] = 0xD8;
	RC6_B2_KEY[11] = 0x9D;
	RC6_B2_KEY[12] = 0x98;
	RC6_B2_KEY[13] = 0xF8;
	RC6_B2_KEY[14] = 0x82;
	RC6_B2_KEY[15] = 0xC2;
	RC6_B2_KEY[16] = 0xA8;
	RC6_B2_KEY[17] = 0x8E;

	RC6_B1_KEY[0] = 0x0;
	RC6_B1_KEY[1] = 0x4C;
	RC6_B1_KEY[2] = 0x24;
	RC6_B1_KEY[3] = 0x58;
}
UINT8 GetColorIdx(UINT8 flag)
{
	switch(flag)
	{
		case IR_BLUE_CFG:
			return 0;
		case IR_YELLOW_CFG:			
			return 1;		
		case IR_RED_CFG:
			return 2;
		case IR_GREEN_CFG:
			return 3;
		default:
			break;
	}
	return 0;
}

UINT8 XferToColorCfg(UINT32 u4SysCode)
{
	switch(u4SysCode)
	{
		case 0:
		case 3:
			return IR_BLUE_CFG;
		case 11:
		case 14:			
		case 25:
			return IR_YELLOW_CFG;
		case 4:
		case 12:			
		case 18:
			return IR_RED_CFG;
		case 13:
		case 31:			
			return IR_GREEN_CFG;
			
		default:
			break;
	}
	return 0;
}

UINT8 Check_IR_Key(UINT32 u4Code, UINT8 key_map_idx)
{
	UINT32 u4tmp;
	UINT8 u1color_idx;
	UINT16 rc5_b2b3;
// For Aambilight button... there is not a rule between the raw code of each color
/*
	if(key_map_idx == IR_AMBILIGHT_IDX)
		return IS_KEY_AMBILIGHT(u4Code);
*/	
//Since we have the BYTE1 information of P_COMP RC6 yellow/red/green, RC6 is support MULIT RC now			
	u1color_idx = GetColorIdx(gLPIR.fgIR_FilterFlag);	
	u4tmp = 0x03000000 | ((UINT32)(RC6_B2_KEY[key_map_idx]) << 16) | (((UINT32)RC6_B1_KEY[u1color_idx]) << 8);	
        #if 0// UART_DBG
        if(key_map_idx == IR_POWER_IDX)
        {
            LogSD("Comp", u4tmp);
            LogD(u4Code);
            LogB(RC6_B2_KEY[key_map_idx]);
            LogB(RC6_B1_KEY[u1color_idx]);
        }
	#endif 	
	if(u4Code == u4tmp)
		return 1;

//system code7 always valid...		
	rc5_b2b3 = RC5_B2B3_KEY[key_map_idx];
	u4tmp = (((UINT32)rc5_b2b3) << 16) | (((UINT32)SYS7_B1_KEY[key_map_idx]) << 8);
	if(u4Code == u4tmp)
		return 1;
		
//Since we have not the BYTE1 information of P_COMP RC5 yellow/red/green, RC5 is not support MULIT RC now		
	u4tmp = (((UINT32)rc5_b2b3) << 16) | (((UINT32)RC5_BLUE_B1_KEY[key_map_idx]) << 8);
	if(u4Code == u4tmp)
		return 1;

	return 0;	
}

UINT8 bWakeupReasonFromKey(UINT32 u4Tmp)
{

	if(Check_IR_Key(u4Tmp, IR_DIGIT0_IDX))
	{
            //LogS("Digital0");
            return PDWNC_WAKE_UP_REASON_RC_DIGIT_0;
	}
	else if(Check_IR_Key(u4Tmp, IR_DIGIT1_IDX))
	{
            //LogS("Digital1");
		return PDWNC_WAKE_UP_REASON_RC_DIGIT_1;
	}		
	else if(Check_IR_Key(u4Tmp, IR_DIGIT2_IDX))
	{
	     //LogS("Digital2");
		return PDWNC_WAKE_UP_REASON_RC_DIGIT_2;
	}		
	else if(Check_IR_Key(u4Tmp, IR_DIGIT3_IDX))
	{
	     //LogS("Digital3");
		return PDWNC_WAKE_UP_REASON_RC_DIGIT_3;
	}		
	else if(Check_IR_Key(u4Tmp, IR_DIGIT4_IDX))
	{
	     //LogS("Digital4");
		return PDWNC_WAKE_UP_REASON_RC_DIGIT_4;
	}
	else if(Check_IR_Key(u4Tmp, IR_DIGIT5_IDX))
	{
	     //LogS("Digital5");
		return PDWNC_WAKE_UP_REASON_RC_DIGIT_5;
	}		
	else if(Check_IR_Key(u4Tmp, IR_DIGIT6_IDX))
	{
	     //LogS("Digital6");
		return PDWNC_WAKE_UP_REASON_RC_DIGIT_6;
	}
	else if(Check_IR_Key(u4Tmp, IR_DIGIT7_IDX))
	{
	     //LogS("Digital7");
		return PDWNC_WAKE_UP_REASON_RC_DIGIT_7;
	}
	else if(Check_IR_Key(u4Tmp, IR_DIGIT8_IDX))
	{
            //LogS("Digital8");
		return PDWNC_WAKE_UP_REASON_RC_DIGIT_8;
	}		
	else if(Check_IR_Key(u4Tmp, IR_DIGIT9_IDX))
	{
            //LogS("Digital9");
		return PDWNC_WAKE_UP_REASON_RC_DIGIT_9;
	}		
	else if(Check_IR_Key(u4Tmp, IR_CH_UP_IDX)) 
	{
            //LogS("ch_up");
		return PDWNC_WAKE_UP_REASON_RC_PRG_UP;
	}		
	else if(Check_IR_Key(u4Tmp, IR_CH_DOWN_IDX))
	{
            //LogS("ch_down");
		return PDWNC_WAKE_UP_REASON_RC_PRG_DOWN;
	}
/*	
	else if(Check_IR_Key(u4Tmp, IR_CH_PREVCH_IDX))
	{
	     LogS("prev_Ch");
		return 4;
	}
	else if(Check_IR_Key(u4Tmp, IR_CH_SOURCE_IDX))
	{
	     LogS("source");
		return PC_WAKE_UP_REASON_RC_INP_SRC;
	}
*/
    return 0;
}



void vTask1_InitCust_P_IrInit()
{
    if(MGET_SYS_PRE_STANDBY_STATUS() == STBY_FROM_AC_ON)
    {
        gLPIR.fgIR_FilterFlag = IR_BLUE_CFG;
    }
    InitIR_KeyMap();

    gLPIR.u1LPIR_CHK = 0;
    gLPIR.u4CurrentPowerCode = 0;
    gLPIR.u4PreviousPowerCode = 0;
    vSetTimer(LPIR_SHORT_TIMER_ID, 0);
    vSetTimer(LPIR_LONG_TIMER_ID, 0);    
}

void vTask1_Cust_P_IrStateMachine()
{
    if(MGET_ARM_POWER_STATE() != ARM_POWER_OFF)
        return;
    if(gLPIR.u1LPIR_CHK == 0)
        return;
    if(fgIsTimerUp(LPIR_SHORT_TIMER_ID))
    {
        MIR_DIS_INT();        
#if UART_DBG        
        LogS("S_timeout"); 
#endif
        gLPIR.u1LPIR_CHK = 0;
        vSetTimer(LPIR_LONG_TIMER_ID, 0);
        gLPIR.u4PreviousPowerCode = gLPIR.u4CurrentPowerCode;
        vWakeUpSystem(PDWNC_WAKE_UP_REASON_IRRC, 0);
        MIR_EN_INT();
//        vTask1_InitCust_P_IrInit();
    }
#if 0

    else if(fgIsTimerUp(LPIR_LONG_TIMER_ID))
    {
            MIR_DIS_INT();
            #if UART_DBG        
            LogSD("L_timeout", gLPIR.u4CurrentPowerCode);         
            #endif
            gLPIR.u1LPIR_CHK = 0;                    
            vSetTimer(LPIR_SHORT_TIMER_ID, 0);
            gLPIR.u4PreviousPowerCode = gLPIR.u4CurrentPowerCode;
#if CEC_FEATURE_3            
            CEC_System_Standby();
#endif
            MIR_EN_INT();

//        vTask1_InitCust_P_IrInit();
    }
    #endif
}

static void _CustPCheckPowerKeyTask(void)
{
    if(gLPIR.u1LPIR_CHK == 0)
        return;
#if 0
    if(fgIsTimerUp(LPIR_SHORT_TIMER_ID))
    {
   //     MIR_DIS_INT();        
#if UART_DBG        
        LogS("S_timeout"); 
#endif
        gLPIR.u1LPIR_CHK = 0;
        vSetTimer(LPIR_LONG_TIMER_ID, 0);
        gLPIR.u4PreviousPowerCode = gLPIR.u4CurrentPowerCode;
        vWakeUpSystem(PDWNC_WAKE_UP_REASON_IRRC, 0);
     //   MIR_EN_INT();
//        vTask1_InitCust_P_IrInit();
    }

    else
#endif        
        if(fgIsTimerUp(LPIR_LONG_TIMER_ID))
    {
         //   MIR_DIS_INT();
            #if UART_DBG        
            LogSD("L_timeout", gLPIR.u4CurrentPowerCode);         
            #endif
            gLPIR.u1LPIR_CHK = 0;                    
            vSetTimer(LPIR_SHORT_TIMER_ID, 0);
            gLPIR.u4PreviousPowerCode = gLPIR.u4CurrentPowerCode;
#if CEC_FEATURE_3            
            CEC_System_Standby();
#endif
           // MIR_EN_INT();

//        vTask1_InitCust_P_IrInit();
    }
}



void vCUST_P_IrTask(IR_DATA_TYPE irData)
{
    UINT8 u1IrWakeupReason;
    if(MGET_ARM_POWER_STATE() != ARM_POWER_OFF)
        return;

    u1IrWakeupReason = bWakeupReasonFromKey(MRC56_TOGGLE_BIT_MASK(irData));
    if(u1IrWakeupReason)
    {
        vWakeUpSystem(u1IrWakeupReason, 0);
        return;
    }

#if 1// P_COMP_LONG_POWER_CEC
#if  UART_DBG
        LogSD("vCUST_P_IrTask", bGetTimer(LPIR_LONG_TIMER_ID));
#endif    

//    if(irData == CUST_P_RC6_POWER)
    MIR_DIS_INT();
    _CustPCheckPowerKeyTask();            
    if(Check_IR_Key(MRC56_TOGGLE_BIT_MASK(irData), IR_POWER_IDX))
    {
        vSetTimer(LPIR_SHORT_TIMER_ID, 5);
        gLPIR.u4CurrentPowerCode = irData;
#if UART_DBG        
                LogSB("Power",GetColorIdx(gLPIR.fgIR_FilterFlag));
#endif
        if(gLPIR.u4CurrentPowerCode != gLPIR.u4PreviousPowerCode) // RC56 has toggle bit to detect if user has relseased the key 
        {
#if UART_DBG            
            LogSD("accept", gLPIR.u4PreviousPowerCode);
#endif
            if(fgIsTimerAtInitialState(LPIR_LONG_TIMER_ID) && (gLPIR.u1LPIR_CHK == 0))
            {
                gLPIR.u1LPIR_CHK = 1;
#if UART_DBG            
            LogS("Set Ltimer");
#endif                
                vSetTimer(LPIR_LONG_TIMER_ID, 60);
            }
        }
#if UART_DBG                
        else
        {
            LogS("discard");
        }
#endif       
    }

    MIR_EN_INT();
#else//mtk ir remoter
    if(irData == 0xfd02e31c)
    {
        LogS("IR Pwr");        
        gLPIR.u1LPIR_CHK = 1;
        gLPIR.u4CurrentPowerCode = irData;
        vSetTimer(LPIR_SHORT_TIMER_ID, 5);
        vSetTimer(LPIR_LONG_TIMER_ID, 30);
    }
    else if((gLPIR.u1LPIR_CHK == 1) && (irData == 0x00000000))// repeat key
    {
        LogS("IR Repeat");
        vSetTimer(LPIR_SHORT_TIMER_ID, 5);
    }
    else if(gLPIR.u4CurrentPowerCode != irData)
    {
        vTask1_InitCust_P_IrInit();
    }
#endif

}

#endif


#if SUPORT_COLD_REBOOT
UINT8 u1ColdRebootChk;

void vTask3Init_cold_rebootInit(void)
{
    u1ColdRebootChk = 0;
    vSetTimer(COLD_REBOOT_TIMER_ID, 0);
}

void vTask3_cold_reboot_state_machine(void)
{
    if((MGET_ARM_POWER_STATE() != ARM_POWER_OFF) || (u1ColdRebootChk == 0))
        return;
    if(fgIsTimerUp(COLD_REBOOT_TIMER_ID))
    {
#if UART_DBG        
        LogS("ColdReboot TimeOut"); 
#endif
        vTask3Init_cold_rebootInit();
        vWakeUpSystem(PDWNC_WAKE_UP_REASON_CUSTOM_4, 0);
    }

}
#endif


#if CUST_B_NEC_IR_DRIVER
static CUST_B_IR_COMBINATION_KEY eIR_COMBINATION_KEY_t;
static CUST_B_IR_TIMER_STAUTS eIR_TIMER_STATUS_t;
//Add by Cust_B 20091215
void vTask5_InitCUST_B_IrInit()
{	
	LogS("[vTask5_InitCUST_B_IrInit] Initial Reg ");
	vSetTimer(POWER_ON_MUTIL_KEY_TIMER_ID, 0);
	eIR_COMBINATION_KEY_t = IR_COMBINATION_NoMatch;
	eIR_TIMER_STATUS_t = IR_TIMER_STAUTS_OFF;
}

/*-----------------------------------------------------------------------------
 * Name: vCUST_B_IrTask 
 *
 * Description: 8032 parse IR remote code while 5363 standby
 *
 * Inputs:  -
 			irData:Ir key code
 *
 * Outputs: -
 *
 * Returns: -
 * Remark:	Add by Cust_B 20091215
 ----------------------------------------------------------------------------*/
void vCUST_B_IrTask(IR_DATA_TYPE irData)
{
	if(MGET_ARM_POWER_STATE() != ARM_POWER_OFF)
        return;
	
	LogSD("vCUST_B_IrTask", irData);
//	#if 0
	switch(irData)
	{
		case CUST_B_NEC_POWER:
			LogS("[8032 IR command]Power on");
			vWakeUpSystem(PDWNC_WAKE_UP_REASON_IRRC,0);
			break;
		case CUST_B_NEC_MENU:
			LogS("[8032 IR command] MENU KEY ");
			eIR_COMBINATION_KEY_t = IR_COMBINATION_KEY1_MATCH;
			eIR_TIMER_STATUS_t = IR_TIMER_STAUTS_ON;
			vSetTimer(POWER_ON_MUTIL_KEY_TIMER_ID, 80);		//Per 4s reset eIR_COMBINATION_KEY_t			
			break;
		case CUST_B_NEC_OK:
			LogS("[8032 IR command] OK KEY ");
			
			if(eIR_COMBINATION_KEY_t == IR_COMBINATION_KEY1_MATCH)
			{
				eIR_COMBINATION_KEY_t = IR_COMBINATION_KEY2_MATCH;
			}
			else
			{
				vTask5_InitCUST_B_IrInit();	
			}
			
			break;
		case CUST_B_NEC_INFO:
			LogS("[8032 IR command] INFO KEY ");
			if(eIR_COMBINATION_KEY_t == IR_COMBINATION_KEY2_MATCH)
			{
				eIR_COMBINATION_KEY_t = IR_COMBINATION_KEY3_MATCH;
			}
			else if(eIR_COMBINATION_KEY_t == IR_COMBINATION_KEY3_MATCH) 
			{
				vTask5_InitCUST_B_IrInit();
				vWakeUpSystem(PDWNC_WAKE_UP_REASON_IRRC,0);		
			}
			else
			{
				vTask5_InitCUST_B_IrInit();	
			}
			break;
		default:
			vTask5_InitCUST_B_IrInit();
			break;
	
	}	
//	#endif
}

void vTask5_CUST_B_IrStateMachine()
{
	if (eIR_TIMER_STATUS_t == IR_TIMER_STAUTS_ON)
	{
		if(fgIsTimerUp(POWER_ON_MUTIL_KEY_TIMER_ID))
	    {
	    	LogS("vTask5 timer out");
			vTask5_InitCUST_B_IrInit();
		}
	}	
}

#endif


#if 0// CUST_P_DC_ON_SEQUENCE
//david: need to use a struct to replace the following variables...
UINT8 Chk_ARM_Alive_Retry;
UINT8 fgEnableDisableMonitorPowerDrop;
UINT8 gTICK_3_DC_ON_Timer_Cnt;
UINT8 fgDC_ON_TimerTimeout;
UINT8 fgEnableDisableMonitorPowerDrop_CMD;
UINT8 fgPHILIPS_SPACE_DC_ON_STATE ;
#define STANDBY_5S_STATE 0xf2
#define WAIT_ARM_TO_NOTIFY_STATE 0xf1
#define DC_ON_SEQUENCE_STATE 0xf3
#define WAIT_FOR_DC_ON_STATE 0xf4
#define SetOPCTRL5_asGPIO() vRegWrite4B(0x00b4,u4RegRead4B(0x00b4) & (~0x200));
#define ENABLE_OPCTRL5_INT() vRegWrite4B(0x0044,0x00000020); //need to enable opctrl5 interrupt
#define IS_DC_POWER_OK() (u1RegRead1B(0x0088) & 0x20)? 1:0
#define IS_ARM_ALIVE() (u4RegRead4B(0x827c))? 1:0


void vTask2Init_CUST_P_dc_on_state_machine_init(void)
{
    SetOPCTRL5_asGPIO();
    ENABLE_OPCTRL5_INT();//vRegWrite4B(0x0044,0x00000020); //need to enable opctrl5 interrupt
    fgDC_ON_TimerTimeout = 0;// 1;
    DC_ON_Timer_Cnt = 0;
    fgEnableDisableMonitorPowerDrop = ENABLE_MONITOR_POWER_DROP;
    fgEnableDisableMonitorPowerDrop_CMD = 0;
    Chk_ARM_Alive_Retry = 0;
    fgPHILIPS_SPACE_DC_ON_STATE = DC_ON_SEQUENCE_STATE;
    
}


static void vTask2_CUST_P_dc_on_state_machine(void)
{
    
        UINT8 u1Opctrl5Var;
    switch(fgPHILIPS_SPACE_DC_ON_STATE)
    {

        case WAIT_FOR_DC_ON_STATE:
                if(fgDC_ON_TimerTimeout == 1)
                 {
            		#if UART_DBG
                        vRegWrite1B(REG_DATAW, 0xfb);
            		#endif
                 
                        fg2StageColdReboot = 0;
                        ClrGPIOEN(0x20);
                        u1Opctrl5Var =  ReadGPIOIN(0x20) ;
                        if((Chk_ARM_Alive_Retry >= 1) ||(u1Opctrl5Var == 0))
                        {
                            u1WakeupReason=2;//WAKEUP_BY_COLD_REBOOT;
                            fgPHILIPS_SPACE_DC_ON_STATE = DC_ON_SEQUENCE_STATE;
                            Chk_ARM_Alive_Retry = 0;
                            SetDC_ON_Timer(0);
//the following code snippet is copied form if(u1SubCode==Cmd_Sub_EnableErrCode) in ext0.c                   
                            fgErrCodeLedOn=0;
                            vWritePowerDownEntry(0x14);    
                            vWritePowerDownEntry(0x04);  
                            vInit_LED_ErrCode();
                            for(u1Opctrl5Var = 0; u1Opctrl5Var < LEDcodeMax; u1Opctrl5Var++)
                            {
                             LED_Array[u1Opctrl5Var] = 0;
                            }
                                                        
                               LED_Array[0]= 0x02;
                            fgErrCodeLedOn=1;
                        }
                        else
                        {
                            SetDC_ON_Timer(CNT_5S);
                            fgPHILIPS_SPACE_DC_ON_STATE = STANDBY_5S_STATE;
                            Chk_ARM_Alive_Retry++;
                          //0629  ERR_LED_ON();
                            EnterPowerStandby();
                        }
                 }
            break;
        case WAIT_ARM_TO_NOTIFY_STATE:
            if(fgStandbyMode)//if fgStandbyMode!=0, we've gotten the enterloader command
            {
                fgPHILIPS_SPACE_DC_ON_STATE = DC_ON_SEQUENCE_STATE;
                SetDC_ON_Timer(0);                
                Chk_ARM_Alive_Retry = 0;
            }
            else
            {
                //if(ChkDC_ON_TimerTimeOut())
                if(fgDC_ON_TimerTimeout == 1)
                {
                    if(Chk_ARM_Alive_Retry >= 1/*3*/)
                    {
                            u1WakeupReason=2;//WAKEUP_BY_COLD_REBOOT;
                            fgPHILIPS_SPACE_DC_ON_STATE = DC_ON_SEQUENCE_STATE;
                            Chk_ARM_Alive_Retry = 0;
                            SetDC_ON_Timer(0);
//the following code snippet is copied form if(u1SubCode==Cmd_Sub_EnableErrCode) in ext0.c                   
                            fgErrCodeLedOn=0;
                            vWritePowerDownEntry(0x14);    
                            vWritePowerDownEntry(0x04);  
                            vInit_LED_ErrCode();
                            for(u1Opctrl5Var = 0; u1Opctrl5Var < LEDcodeMax; u1Opctrl5Var++)
                            {
                             LED_Array[u1Opctrl5Var] = 0;
                            }
                               LED_Array[0]= 0x02;                            
                            fgErrCodeLedOn=1;
                    }
                    else
                    {
                            SetDC_ON_Timer(CNT_5S);
                            fgPHILIPS_SPACE_DC_ON_STATE = STANDBY_5S_STATE;
                            Chk_ARM_Alive_Retry++;
             //0629               ERR_LED_ON();
                            EnterPowerStandby();
//                            while(1);
                    }                    
                }
            }
            break;
        case STANDBY_5S_STATE:
            if(fgStandbyMode)//if fgStandbyMode!=0, we've gotten the enterloader command
            {
                fgPHILIPS_SPACE_DC_ON_STATE = DC_ON_SEQUENCE_STATE;
                SetDC_ON_Timer(0);                
                Chk_ARM_Alive_Retry = 0;
            }
            else if(fgDC_ON_TimerTimeout == 1) //if(ChkDC_ON_TimerTimeOut())
            {
                vRegWrite4B(0x0120, u4RegRead4B(0x0120)|0x00020000); //Enable 8032 wake up
                #if RC56_IR_DRIVER                        
                u4PreviousPowerCode = 0;
                #endif
                vWakeUpSystem();
            }
            break;            
        case DC_ON_SEQUENCE_STATE:
        default:
            break;
            
    }

}

#endif

#if CUST_P_SX_SUPPORT
#define RES_ACK 0
#define RES_NACK 1
#define RES_NAV 2
UINT8 fgSerialXpressEnable;
UART_DRV_OBJ* fgSerialXpressData;
#if CUS_SH_HOTEL_MODE 
UINT8 fgCust_SH_HotelModeUartWakeupEn;    
#endif

void CUS_P_SX_send(UINT8* pPAyload, UINT8 count)
{
    UINT8 i;
    UINT8 Cmd_Array[8];
    UINT8 u1Checksum;
    u1Checksum=0;


    Cmd_Array[0]=0xe;
    Cmd_Array[1]=count+10;
    Cmd_Array[2]=0;
    Cmd_Array[3]=0;
    Cmd_Array[4]=5;
    Cmd_Array[5]=count+1;
    Cmd_Array[6]=0;
    Cmd_Array[7]=0xc;

    
    for(i=0;i<8;i++)
        vIO32Write1B(PDWNC_DATAW, Cmd_Array[i]);

    for(i=0;i<(count-1);i++,pPAyload++)
    {
        vIO32Write1B(PDWNC_DATAW,  *pPAyload);
        u1Checksum ^=(*pPAyload);
    }
    vIO32Write1B(PDWNC_DATAW,  u1Checksum);

    vIO32Write1B(PDWNC_DATAW,  0xa5);
    vIO32Write1B(PDWNC_DATAW,  0xa5);    
}

void CUS_P_SX_ReportEvent(UINT8 response)
{
    UINT8 Report_array[2];
    
    Report_array[0]=0x16;
    Report_array[1]=response;
    CUS_P_SX_send(Report_array,3); //bema add check sum 2-->3
}

void CUS_P_SX_ReportPowerState(UINT8 power_state)
{
    UINT8 Report_array[3];

    Report_array[0]=0x22;
    Report_array[1]=0x18;
    Report_array[2]=power_state;
    CUS_P_SX_send(Report_array,4); //bema add check sum 2-->3
}

#if RW_REG_INTERFACE
static MemoryRW_DbgTask()
{

		UINT16 data u2Addr;
		UINT32 data u4Data;

		 if(
		   (fgSerialXpressData->uRs232Data[0] == 'w') &&
		  (fgSerialXpressData->uRs232Data[1] == ' ') &&
		  (fgSerialXpressData->uRs232Data[2] == '0') &&
		  (fgSerialXpressData->uRs232Data[3] == 'x') &&
//		  (fgSerialXpressData->uRs232Data[4] == '0') &&
		  (fgSerialXpressData->uRs232Data[8] == ' ')  &&
		  (fgSerialXpressData->uRs232Data[9] == '0')  &&	  
		  (fgSerialXpressData->uRs232Data[10] == 'x') 
		  )
	  {
		 u2Addr = ((fgSerialXpressData->uRs232Data[4]) >= 'a') ? ((UINT16)((fgSerialXpressData->uRs232Data[4]) - 87) << 12) : ((UINT16)((fgSerialXpressData->uRs232Data[4]) - 48) << 12);
		u2Addr += ((fgSerialXpressData->uRs232Data[5]) >= 'a') ? ((UINT16)((fgSerialXpressData->uRs232Data[5]) - 87) << 8) : ((UINT16)((fgSerialXpressData->uRs232Data[5]) - 48) << 8);
		u2Addr += ((fgSerialXpressData->uRs232Data[6]) >= 'a') ? ((UINT16)((fgSerialXpressData->uRs232Data[6]) - 87) << 4) : ((UINT16)((fgSerialXpressData->uRs232Data[6]) - 48) << 4);
		u2Addr += ((fgSerialXpressData->uRs232Data[7]) >= 'a') ? ((UINT16)(fgSerialXpressData->uRs232Data[7]) - 87)  : ((UINT16)(fgSerialXpressData->uRs232Data[7]) - 48) ;
	
		 u4Data = ((fgSerialXpressData->uRs232Data[11]) >= 'a') ?  ((UINT32)((fgSerialXpressData->uRs232Data[11]) - 87) << 28) : ((UINT32)((fgSerialXpressData->uRs232Data[11]) - 48) << 28);
		 u4Data += ((fgSerialXpressData->uRs232Data[12]) >= 'a') ? ((UINT32)((fgSerialXpressData->uRs232Data[12]) - 87) << 24) : ((UINT32)((fgSerialXpressData->uRs232Data[12]) - 48) << 24);
		 u4Data += ((fgSerialXpressData->uRs232Data[13]) >= 'a') ? ((UINT32)((fgSerialXpressData->uRs232Data[13]) - 87) << 20) : ((UINT32)((fgSerialXpressData->uRs232Data[13]) - 48) << 20);
		 u4Data += ((fgSerialXpressData->uRs232Data[14]) >= 'a') ? ((UINT32)((fgSerialXpressData->uRs232Data[14]) - 87) << 16) : ((UINT32)((fgSerialXpressData->uRs232Data[14]) - 48) << 16);
		 u4Data += ((fgSerialXpressData->uRs232Data[15]) >= 'a') ? ((UINT32)((fgSerialXpressData->uRs232Data[15]) - 87) << 12) : ((UINT32)((fgSerialXpressData->uRs232Data[15]) - 48) << 12);
		 u4Data += ((fgSerialXpressData->uRs232Data[16]) >= 'a') ? ((UINT32)((fgSerialXpressData->uRs232Data[16]) - 87) << 8) : ((UINT32)((fgSerialXpressData->uRs232Data[16]) - 48) << 8); 	
		 u4Data += ((fgSerialXpressData->uRs232Data[17]) >= 'a') ? ((UINT32)((fgSerialXpressData->uRs232Data[17]) - 87) << 4) : ((UINT32)((fgSerialXpressData->uRs232Data[17]) - 48) << 4); 	
		 u4Data += ((fgSerialXpressData->uRs232Data[18]) >= 'a') ? ((UINT32)((fgSerialXpressData->uRs232Data[18]) - 87) ) : ((UINT32)((fgSerialXpressData->uRs232Data[18]) - 48) ); 		 
	
		while (!(RegReadFldAlign(PDWNC_STAB, FLD_WR_ALLOW))){};    vIO32Write1B(PDWNC_DATAW, 0x0D);
		while (!(RegReadFldAlign(PDWNC_STAB, FLD_WR_ALLOW))){};    vIO32Write1B(PDWNC_DATAW, 0x0A);    
		_LogS("Write Addr :");
		while (!(RegReadFldAlign(PDWNC_STAB, FLD_WR_ALLOW))){}; 	 vIO32Write1B(PDWNC_DATAW, '0');
		while (!(RegReadFldAlign(PDWNC_STAB, FLD_WR_ALLOW))){}; 	 vIO32Write1B(PDWNC_DATAW, 'x');	 
		_Print8bitsHex((UINT8)(u2Addr >> 8));
		_Print8bitsHex((UINT8)(u2Addr & 0x00ff));
		while (!(RegReadFldAlign(PDWNC_STAB, FLD_WR_ALLOW))){};    vIO32Write1B(PDWNC_DATAW, 0x0D);
		while (!(RegReadFldAlign(PDWNC_STAB, FLD_WR_ALLOW))){};    vIO32Write1B(PDWNC_DATAW, 0x0A);    
		_LogS("Data:");
		while (!(RegReadFldAlign(PDWNC_STAB, FLD_WR_ALLOW))){}; 	 vIO32Write1B(PDWNC_DATAW, '0');
		while (!(RegReadFldAlign(PDWNC_STAB, FLD_WR_ALLOW))){}; 	 vIO32Write1B(PDWNC_DATAW, 'x'); 
	  _Print8bitsHex((UINT8)((u4Data >> 24) & 0x000000ff));
	  _Print8bitsHex((UINT8)((u4Data >> 16) & 0x000000ff));
	  _Print8bitsHex((UINT8)((u4Data >> 8) & 0x000000ff));
		_Print8bitsHex((UINT8)(u4Data & 0x000000ff));
		while (!(RegReadFldAlign(PDWNC_STAB, FLD_WR_ALLOW))){};    vIO32Write1B(PDWNC_DATAW,  0x0D);
		while (!(RegReadFldAlign(PDWNC_STAB, FLD_WR_ALLOW))){};    vIO32Write1B(PDWNC_DATAW,  0x0A);	
		vRegWrite4B(u2Addr, u4Data);
	  }
	  else	if(
		   (fgSerialXpressData->uRs232Data[0] == 'r') &&
		  (fgSerialXpressData->uRs232Data[1] == ' ') &&
		  (fgSerialXpressData->uRs232Data[2] == '0') &&
		  (fgSerialXpressData->uRs232Data[3] == 'x') /*&&
		  (fgSerialXpressData->uRs232Data[4] == '0')*/
		  ) 	 
	  {
		//	u2Addr	= (fgSerialXpressData->uRs232Data[5]) ;
		  u2Addr = ((fgSerialXpressData->uRs232Data[4]) >= 'a') ? ((UINT16)((fgSerialXpressData->uRs232Data[4]) - 87) << 12) : ((UINT16)((fgSerialXpressData->uRs232Data[4]) - 48) << 12);		
		  u2Addr += ((fgSerialXpressData->uRs232Data[5]) >= 'a') ? ((UINT16)((fgSerialXpressData->uRs232Data[5]) - 87) << 8) : ((UINT16)((fgSerialXpressData->uRs232Data[5]) - 48) << 8);
		  u2Addr += ((fgSerialXpressData->uRs232Data[6]) >= 'a') ? ((UINT16)((fgSerialXpressData->uRs232Data[6]) - 87) << 4) : ((UINT16)((fgSerialXpressData->uRs232Data[6]) - 48) << 4);
		  u2Addr += ((fgSerialXpressData->uRs232Data[7]) >= 'a') ? ((UINT16)(fgSerialXpressData->uRs232Data[7]) - 87) : ((UINT16)(fgSerialXpressData->uRs232Data[7]) - 48) ;
		  u4Data = u4RegRead4B(u2Addr);
	
		while (!(RegReadFldAlign(PDWNC_STAB, FLD_WR_ALLOW))){}; 	 vIO32Write1B(PDWNC_DATAW, 0x0D);
		while (!(RegReadFldAlign(PDWNC_STAB, FLD_WR_ALLOW))){}; 	 vIO32Write1B(PDWNC_DATAW, 0x0A);	 
	  _LogS("Read Addr ");
		while (!(RegReadFldAlign(PDWNC_STAB, FLD_WR_ALLOW))){};  vIO32Write1B(PDWNC_DATAW, '0');
		while (!(RegReadFldAlign(PDWNC_STAB, FLD_WR_ALLOW))){};  vIO32Write1B(PDWNC_DATAW, 'x');  
		while (!(RegReadFldAlign(PDWNC_STAB, FLD_WR_ALLOW))){}; 	 
	  _Print8bitsHex((UINT8)(u2Addr >> 8));
	  _Print8bitsHex((UINT8)(u2Addr & 0x00ff));
		while (!(RegReadFldAlign(PDWNC_STAB, FLD_WR_ALLOW))){};  vIO32Write1B(PDWNC_DATAW, 0x0D);
		while (!(RegReadFldAlign(PDWNC_STAB, FLD_WR_ALLOW))){};  vIO32Write1B(PDWNC_DATAW, 0x0A);	 
	  _LogS("==> ");
		while (!(RegReadFldAlign(PDWNC_STAB, FLD_WR_ALLOW))){};  vIO32Write1B(PDWNC_DATAW, '0');
		while (!(RegReadFldAlign(PDWNC_STAB, FLD_WR_ALLOW))){};  vIO32Write1B(PDWNC_DATAW, 'x');	
	  _Print8bitsHex((UINT8)((u4Data >> 24) & 0x000000ff));
	  _Print8bitsHex((UINT8)((u4Data >> 16) & 0x000000ff));
	  _Print8bitsHex((UINT8)((u4Data >> 8) & 0x000000ff));
	  _Print8bitsHex((UINT8)(u4Data & 0x000000ff));
		while (!(RegReadFldAlign(PDWNC_STAB, FLD_WR_ALLOW))){};  vIO32Write1B(PDWNC_DATAW,	0x0D);
		while (!(RegReadFldAlign(PDWNC_STAB, FLD_WR_ALLOW))){};  vIO32Write1B(PDWNC_DATAW,	0x0A);	  
		
	  }
	
}
#endif

void CUS_P_SX_parse(void)
{
#if CUS_SH_HOTEL_MODE 
#else
    UINT8 data i = 0;
    UINT8 data num_payload_size;
    UINT8 data bchecksum;
    bchecksum=0;
#endif    
    #if 0// SX_DBG
    
    for(i=0;i<32;i++)
        vIO32Write1B(PDWNC_DATAW, fgSerialXpressData->uRs232Data[i]);

        fgSerialXpressData->uRs232Rxidx=0;
    #endif
#if RW_REG_INTERFACE
	if(fgSerialXpressData->uRs232Rxidx == 0)//david
	{
		return;
	}
#endif        
    fgSerialXpressData->uRs232Rxidx=0;

#if RW_REG_INTERFACE
	MemoryRW_DbgTask();
#endif        
    
//for T_comp H_proj project
  if( (fgSerialXpressData->uRs232Data[0] == 0x57) &&
         (fgSerialXpressData->uRs232Data[1] == 0x01) &&
         (fgSerialXpressData->uRs232Data[2] == 0x02) &&
         (fgSerialXpressData->uRs232Data[3] == 0x00) &&
         (fgSerialXpressData->uRs232Data[4] == 0x00) &&
    (fgSerialXpressData->uRs232Data[5] == 0x5a)
    )
  {
       vWakeUpSystem(PDWNC_WAKE_UP_REASON_UART_NORMAL, 0);
  }

//for A_COMP BXX project
  if( (fgSerialXpressData->uRs232Data[0] == 0xbe) &&
         (fgSerialXpressData->uRs232Data[1] == 0x41) &&
         (fgSerialXpressData->uRs232Data[2] == 0x01) &&
         (fgSerialXpressData->uRs232Data[3] == 0x06) &&
         (fgSerialXpressData->uRs232Data[4] == 0x50) &&
         (fgSerialXpressData->uRs232Data[5] == 0x00) &&
         (fgSerialXpressData->uRs232Data[6] == 0x00) &&
         (fgSerialXpressData->uRs232Data[7] == 0x00) &&
         (fgSerialXpressData->uRs232Data[8] == 0x00) &&
         (fgSerialXpressData->uRs232Data[9] == 0x00) &&         
         (fgSerialXpressData->uRs232Data[10] == 0x00) &&                  
    (fgSerialXpressData->uRs232Data[11] == 0xa8)
    )
  {
       vWakeUpSystem(PDWNC_WAKE_UP_REASON_UART_NORMAL, 0);
  }

#if Cust_B_Debug_UART
	LogSB("Urx_len:",fgSerialXpressData->uRs232Rxidx);
	LogSB("Urx[0]:", fgSerialXpressData->uRs232Data[0]);
	LogSB("Urx[1]:", fgSerialXpressData->uRs232Data[1]);
	LogSB("Urx[2]:", fgSerialXpressData->uRs232Data[2]);
	LogSB("Urx[3]:", fgSerialXpressData->uRs232Data[3]);
	LogSB("Urx[4]:", fgSerialXpressData->uRs232Data[4]);
	LogSB("Urx[5]:", fgSerialXpressData->uRs232Data[5]);
	LogSB("Urx[6]:", fgSerialXpressData->uRs232Data[6]);
	LogSB("Urx[7]:", fgSerialXpressData->uRs232Data[7]);
	LogSB("Urx[8]:", fgSerialXpressData->uRs232Data[8]);
	LogSB("Urx[9]:", fgSerialXpressData->uRs232Data[9]);
	LogSB("Urx[10]:",fgSerialXpressData->uRs232Data[10]);
	LogSB("Urx[11]:",fgSerialXpressData->uRs232Data[11]);
	LogSB("Urx[12]:",fgSerialXpressData->uRs232Data[12]);
#endif	
/* A_COMP Hotel Op code: LCD TV ON
    Cust_B 20091215	
*/
	if( (fgSerialXpressData->uRs232Data[0] == '#') &&
         (fgSerialXpressData->uRs232Data[1] == 'S') &&
         (fgSerialXpressData->uRs232Data[2] == '0') &&
         (fgSerialXpressData->uRs232Data[3] == '0') &&
         (fgSerialXpressData->uRs232Data[4] == '1') &&
    (fgSerialXpressData->uRs232Data[5] == '#')
    )	
  {
       LogS(" [Hotel Op code]: LCD TV ON");	//Add by Cust_B 20091215
	   vWakeUpSystem(PDWNC_WAKE_UP_REASON_UART_NORMAL, 0);
  }
  
#if CUS_SH_HOTEL_MODE  
  //only for 9600 baud rate 
  if( (fgCust_SH_HotelModeUartWakeupEn == 1) &&
        (fgSerialXpressData->uRs232Data[0] == 'P') &&
       (fgSerialXpressData->uRs232Data[1] == 'O') &&
       (fgSerialXpressData->uRs232Data[2] == 'W') &&
       (fgSerialXpressData->uRs232Data[3] == 'R') &&
       (fgSerialXpressData->uRs232Data[4] == '1') &&
       (fgSerialXpressData->uRs232Data[5] == '#') &&
       (fgSerialXpressData->uRs232Data[6] == '#') &&
       (fgSerialXpressData->uRs232Data[7] == '#') &&
        (fgSerialXpressData->uRs232Data[8] == '<'))
    {
        vIO32Write1B(PDWNC_DATAW,  'O');
        vIO32Write1B(PDWNC_DATAW,  'K'); 
        vIO32Write1B(PDWNC_DATAW,  '<'); 
        vWakeUpSystem(PDWNC_WAKE_UP_REASON_UART_NORMAL, 0);
  }
  else
  {
      vIO32Write1B(PDWNC_DATAW,  'E');
      vIO32Write1B(PDWNC_DATAW,  'R'); 
      vIO32Write1B(PDWNC_DATAW,  'R'); 
      vIO32Write1B(PDWNC_DATAW,  '<');             
  }
#else
    if( (fgSerialXpressData->uRs232Data[0] == 0xE) &&
         (fgSerialXpressData->uRs232Data[2] == 0) &&
         (fgSerialXpressData->uRs232Data[3] == 0) &&
         (fgSerialXpressData->uRs232Data[4] == 5) &&
         (fgSerialXpressData->uRs232Data[6] == 0) &&
    (fgSerialXpressData->uRs232Data[7] == 0xc)
    )
    {
        num_payload_size = (fgSerialXpressData->uRs232Data[1]-10);

        for(i=0; i<(num_payload_size-1); i++)
        bchecksum ^= fgSerialXpressData->uRs232Data[8+i];

        if( (fgSerialXpressData->uRs232Data[5] == num_payload_size+1) &&
        (fgSerialXpressData->uRs232Data[8+num_payload_size-1] == bchecksum) &&
        (fgSerialXpressData->uRs232Data[8+num_payload_size] == 0xa5) &&
        (fgSerialXpressData->uRs232Data[8+num_payload_size+1] == 0xa5)
        )
        {
        
            switch(fgSerialXpressData->uRs232Data[8])
            {
                case 0x20:  //Set command
                if(fgSerialXpressData->uRs232Data[9] == 0xA3) //set RS232 control
                {

                   // CUS_P_SX_ReportEvent(RES_NAV); //Currently not support, since uP cannot access NVM
                    if(fgSerialXpressData->uRs232Data[10] == 0x01)    //Set Power on
                    {
                    
			    fgSerialXpressEnable = 1;
                    }
                    else
                    {
                
			    fgSerialXpressEnable = 0;
                    	}
                    CUS_P_SX_ReportEvent(RES_ACK);
                }
                else if(fgSerialXpressData->uRs232Data[9] == 0x18 && fgSerialXpressEnable == 1)    //set power state
                {
                
                    if(fgSerialXpressData->uRs232Data[10] == 0x01)    //Set Power on
                    {
                    
                        CUS_P_SX_ReportEvent(RES_ACK);
                        //SwitchBack_PORT0(); //switch back port0 to ARM
                        //set 0x28818 bit 2 low
		//vRegWrite1B(REG_RS232_MODE, 0); 
                        //vRegWrite4B(0x0120, u4RegRead4B(0x0120)|0x00020000); //Enable 8032 wake up
                       // SwitchBack_PORT0(); //switch back port0 to ARM                                    
/*                        vWakeUpSystem();
                        LED1_ON;
                        LED2_OFF;
                        u1WakeupReason = 3;
*/
                        vWakeUpSystem(PDWNC_WAKE_UP_REASON_UART_NORMAL, 0);
                    }
                }
                else    //command not support in standby mode
                {
                    CUS_P_SX_ReportEvent(RES_NAV);
                }
                break;

                case 0x21:  //Get command
                if(fgSerialXpressData->uRs232Data[9] == 0x18) //Get power state
                {
                    CUS_P_SX_ReportEvent(RES_ACK);
                    CUS_P_SX_ReportPowerState(0x0); //power state is standby
                }
                else    //command not support in standby mode
                {
                    CUS_P_SX_ReportEvent(RES_NAV);
                }
                break;

                default:    //not set/get command
                CUS_P_SX_ReportEvent(RES_NAV);
                break;
            }
        }
        else //protocol error, should response Nack
        {
            CUS_P_SX_ReportEvent(RES_NACK);
        }
    }
    else //protocol error, should response Nack
    {
    	if(fgSerialXpressData->uRs232Data[0] == 0xE)
        CUS_P_SX_ReportEvent(RES_NACK);
    }
#endif //CUS_SH_HOTEL_MODE
}
#endif

#if S_HOTEL_MODE_COMMAND
Led_Toggle_Ctrl gLedToggleCtrl;
UINT8 data u1HotelDbg;
UINT8 xdata u1Module2Enable;
//UINT8 data u1HotelModeModule;
UINT8 xdata u1CustomData1_HotelModule;
UINT8 xdata u1CustomData3_Module1ReplyData;
UINT8 xdata u1hotel_module2_init_mode;
UINT8 data u1Hotel_module2_state;
UINT8 bModule2Status;
UART_DRV_OBJ xdata fgHotelCmdData;
UINT8 u1Module2AliveByte1;
UINT8 u1Module2AliveByte3;
UINT8 u1Module2AvSelectPW;
UINT8 u1Module2AvSelectSrc;
UINT8 u1Module2HdmiPortSel;

//UART_DRV_OBJ* idata pSysUartData;

extern void TurnOffToggleLed(void);
extern void TurnOnToggleLed(void);
#define NO_SEND_TEST 0
/*
static void vFlushHotelCmdBuf(void)
{
	fgHotelCmdData.uRs232Rxidx = 0;
	fgHotelCmdData.fgRs232Rx = 0;
	fgHotelCmdData.uRs232Data[0] = 0;
	SYS_PrintString(1, " vTask8S clenn buffer");
}
*/
UINT8 VALID_LOCATEL_CMD(void)
{
	UINT8 i, u1Len, u1Sum = 0;
	
// Max data lengthe is (31 bytes + chk), so max fgHotelCmdData.uRs232Data[1] is 31
	if(fgHotelCmdData.uRs232Data[1] >=1 && fgHotelCmdData.uRs232Data[1] <= MAX_UART_BUF_LEN - 1)
	{
		u1Len = fgHotelCmdData.uRs232Data[1];
		for(i = 0; i < u1Len; i++)
		{
			u1Sum += fgHotelCmdData.uRs232Data[i];
		}
		if(u1Sum == fgHotelCmdData.uRs232Data[u1Len])
		{
			SYS_PrintString(1, "valid cmd"); 
			return 1;
		}
		else
		{
			SYS_PrintString(1, "no valid cmd1"); 
			return 0;
		}
	}
	else
	{
		SYS_PrintString(1, "no valid cmd2");
		return 0;
	}

}

#define PERFORMANCE_DBG 0

#if PERFORMANCE_DBG
bit cmd_chg;
bit w_cmd;
#endif
void vHotelMode2_StateMachine(void)
{
	UINT8 power_on_av = UART_WAKEUP_SUB_REASON_ILLEGAL;

//	SYS_PrintString_B(3, "vHotelMode2_StateMachine", u1Hotel_module2_state); 			   
	if(MGET_ARM_POWER_STATE() == ARM_POWER_OFF)
	{
		bModule2Status = u1HotelModule2OnLine;
	}
    switch(u1Hotel_module2_state)
    {
        case CHANGING_TO_10SEC_STATE:
			SYS_PrintString(1, "CHANGING_TO_10SEC_STATE");
			u1HotelModule2OnLine = 1;
			MIR_DIS_HW_WAKEUP();
            u1Hotel_module2_state = START_100_MSEC_TIMER_STATE;
            u1Module2AliveByte1 = 0;
			u1Module2AliveByte3 = 0;
#if 0
vRegWrite1B(PDWNC_DATAW,0x01);
vRegWrite1B(PDWNC_DATAW,0x05);
vRegWrite1B(PDWNC_DATAW, u1Module2AliveByte1);
vRegWrite1B(PDWNC_DATAW,0x00);
vRegWrite1B(PDWNC_DATAW, u1Module2AliveByte3);
vRegWrite1B(PDWNC_DATAW, 0x06 + u1Module2AliveByte1 + u1Module2AliveByte3);
#endif
			vSetTimer(HOTEL_MODULE2_STATE_MACHINE_LONG_TIMER_ID, TIMER_10_SEC);
#if PERFORMANCE_DBG
if(w_cmd)
{
		vRegWrite1B(PDWNC_DATAW,0xdd);
		vRegWrite1B(PDWNC_DATAW,0xdd);
		vRegWrite1B(PDWNC_DATAW,0xdd);
		vRegWrite1B(PDWNC_DATAW,0xdd);
		vRegWrite1B(PDWNC_DATAW,0xdd);
}
#endif
            break;
        case START_100_MSEC_TIMER_STATE:
			SYS_PrintString(3, "START_100_MSEC_TIMER_STATE"); 			   
			vSetTimer(HOTEL_MODULE2_STATE_MACHINE_SHORT_TIMER_ID, TIMER_100_MSEC);
            u1Hotel_module2_state = WAIT_100_MSEC_TIMER_TIMEOUT_STATE;
#if PERFORMANCE_DBG
if(w_cmd)
{
		vRegWrite1B(PDWNC_DATAW,0xee);
		vRegWrite1B(PDWNC_DATAW,0xee);
		vRegWrite1B(PDWNC_DATAW,0xee);
		vRegWrite1B(PDWNC_DATAW,0xee);
}
#endif			
            break;
        case WAIT_100_MSEC_TIMER_TIMEOUT_STATE:
			SYS_PrintString(3, "WAIT_100_MSEC_TIMER_TIMEOUT_STATE"); 
            if(fgIsTimerUp(HOTEL_MODULE2_STATE_MACHINE_SHORT_TIMER_ID))
            {
                u1Hotel_module2_state = _1SEC_RECEIVE_MSG_STATE;
                SYS_PrintString(3, "_1SEC_RECEIVE_MSG_STATE");
                #if NO_SEND_TEST

                #else
				//if(u4RegRead4B(PDWNC_RESRV0)!= 0x12345678)				
				if(u1HotelDbg)//if(MGET_SYS_UART_BAUDRATE() == BR_9600)
				{
					SYS_PrintString(1, "0105");
					SYS_PrintString_B(1, " ", u1Module2AliveByte1);
					SYS_PrintString(1, "00");
					SYS_PrintString_B(1, " ", u1Module2AliveByte3);					
					SYS_PrintString_B(1, " ", 0x06 + u1Module2AliveByte1 + u1Module2AliveByte3);										
				}
				else
				{
					vRegWrite1B(PDWNC_DATAW,0x01);
					vRegWrite1B(PDWNC_DATAW,0x05);
					vRegWrite1B(PDWNC_DATAW, u1Module2AliveByte1);
					vRegWrite1B(PDWNC_DATAW,0x00);
					vRegWrite1B(PDWNC_DATAW, u1Module2AliveByte3);
					vRegWrite1B(PDWNC_DATAW, 0x06 + u1Module2AliveByte1 + u1Module2AliveByte3);   					
				}
                #endif
            }
#if PERFORMANCE_DBG
			if(w_cmd)
			{
					vRegWrite1B(PDWNC_DATAW,0xff);
					vRegWrite1B(PDWNC_DATAW,0xff);
					vRegWrite1B(PDWNC_DATAW,0xff);
					vRegWrite1B(PDWNC_DATAW,0xff);
			}
#endif			
			if(!fgHotelCmdData.fgRs232Rx)
			{
            	break;
			}
        case _1SEC_RECEIVE_MSG_STATE:
            if(fgHotelCmdData.fgRs232Rx)
            {
				SYS_PrintString(3, "_1SEC_RECEIVE_MSG_STATE"); 
                if( (fgHotelCmdData.uRs232Data[0]==0x81) && (fgHotelCmdData.uRs232Data[1]==0x03) && (fgHotelCmdData.uRs232Data[2]==0x80) && (fgHotelCmdData.uRs232Data[3]==0x04))	    
                {
					SYS_PrintString(1, "reset 10sec");
			        u1Module2AliveByte1 = 0;
					u1Module2AliveByte3 = 0;
                    vSetTimer(HOTEL_MODULE2_STATE_MACHINE_LONG_TIMER_ID, TIMER_10_SEC);
                }
                else if((fgHotelCmdData.uRs232Data[0] == 0x87) && (fgHotelCmdData.uRs232Data[1] == 0x04) && (fgHotelCmdData.uRs232Data[3] == 0x0))
                {
					SYS_PrintString(1, "LED ctrl"); 
                    if((0x8b + fgHotelCmdData.uRs232Data[2]) == fgHotelCmdData.uRs232Data[4])
                    {
						SYS_PrintString(1, "LED data valid"); 
						u1Module2AliveByte1 = 0x8;
                        gLedToggleCtrl.u1TimerLedMode = fgHotelCmdData.uRs232Data[2];
                        if(gLedToggleCtrl.u1TimerLedMode == SLOW_BLINKING)
                        {
							SYS_PrintString(1, "S_BLINK"); 
                            gLedToggleCtrl.u1LedTogglePeriod = TIMER_1_SEC;
							gLedToggleCtrl.u1LedCurrentValue = 0;
                            gLedToggleCtrl.u1LedToggleEnable = 1;
                        }
                        else if(gLedToggleCtrl.u1TimerLedMode == FAST_BLINKING)
                        {
							SYS_PrintString(1, "F_BLINK"); 
                            gLedToggleCtrl.u1LedTogglePeriod = TIMER_500_MSEC;
							gLedToggleCtrl.u1LedCurrentValue = 0;
                            gLedToggleCtrl.u1LedToggleEnable = 1;							
                        }
                        else  if(gLedToggleCtrl.u1TimerLedMode == ALWAYS_OFF)
                        {
							SYS_PrintString(1, "off"); 
                            gLedToggleCtrl.u1LedTogglePeriod = 0;
							gLedToggleCtrl.u1LedCurrentValue = 0;
                            gLedToggleCtrl.u1LedToggleEnable = 0;	
							TurnOffToggleLed();
                        }
                        else  if(gLedToggleCtrl.u1TimerLedMode == ALWAYS_ON)
                        {
							SYS_PrintString(1, "on"); 
                            gLedToggleCtrl.u1LedTogglePeriod = 0;
							gLedToggleCtrl.u1LedCurrentValue = 1;
                            gLedToggleCtrl.u1LedToggleEnable = 0; 
                           	TurnOnToggleLed();
                        }
/*
                        #if NO_SEND_TEST

                        #else
						if(u1HotelDbg)//if(MGET_SYS_UART_BAUDRATE() == BR_9600)
						{
							SYS_PrintString(1, "01050800000e");
						}
						else
						{
	                        vRegWrite1B(PDWNC_DATAW,0x01);
	                        vRegWrite1B(PDWNC_DATAW,0x05);            
	                        vRegWrite1B(PDWNC_DATAW,0x08);            
	                        vRegWrite1B(PDWNC_DATAW,0x00);    
	                        vRegWrite1B(PDWNC_DATAW,0x00);  //1030   							
	                        vRegWrite1B(PDWNC_DATAW,0x0e);            
						}
                        #endif
*/
						u1Module2AliveByte1 = 0x08;
                    }
                }
				else if((fgHotelCmdData.uRs232Data[0] == 0x9d) && (fgHotelCmdData.uRs232Data[1] == 0x03) && (fgHotelCmdData.uRs232Data[2] == 0x00) && (fgHotelCmdData.uRs232Data[3] == 0xa0))
				{

					if(u1HotelDbg)//if(MGET_SYS_UART_BAUDRATE() == BR_9600)
					{
						SYS_PrintString(1, "0d031f2f");						
					}
					else
					{
						vRegWrite1B(PDWNC_DATAW,0x0D);
						vRegWrite1B(PDWNC_DATAW,0x03);
						vRegWrite1B(PDWNC_DATAW,0x1f);
						vRegWrite1B(PDWNC_DATAW,0x2f);
						u1Module2AliveByte1 = 0x8;
					}
				}
				else if((fgHotelCmdData.uRs232Data[0] == 0x9d) && (fgHotelCmdData.uRs232Data[1] == 0x02) && (fgHotelCmdData.uRs232Data[2] == 0x9f))
				{

					if(u1HotelDbg)//if(MGET_SYS_UART_BAUDRATE() == BR_9600)
					{
						SYS_PrintString(1, "0d031f2f");						
					}
					else
					{
						vRegWrite1B(PDWNC_DATAW,0x0D);
						vRegWrite1B(PDWNC_DATAW,0x03);
						vRegWrite1B(PDWNC_DATAW,0x1f);
						vRegWrite1B(PDWNC_DATAW,0x2f);
						u1Module2AliveByte1 = 0x8;
					}
				}
				else if((fgHotelCmdData.uRs232Data[0] == 0x82) && (fgHotelCmdData.uRs232Data[1] == 0x04) && (fgHotelCmdData.uRs232Data[2] == 0x80))
                {
					SYS_PrintString(1, "Wakeup2souce...");
                    if((0x06 + fgHotelCmdData.uRs232Data[3]) == fgHotelCmdData.uRs232Data[4])
                    {
						SYS_PrintString_B(1, "wakeup 2 souce? valid", fgHotelCmdData.uRs232Data[4]);
                    if((fgHotelCmdData.uRs232Data[3] == 0) || (fgHotelCmdData.uRs232Data[3] == u1Module2AvSelectPW))
                    {
                            switch(u1Module2AvSelectSrc)
                            {
                                case 0:
                                    power_on_av = UART_WAKEUP_SUB_REASON_CVBS_1;
                                    SYS_PrintString(1, "AV1");
                                    break;
                                case 1:
                                    power_on_av = UART_WAKEUP_SUB_REASON_CVBS_2;
                                    SYS_PrintString(1, "AV2");
                                    break;
                                case 2:
                                    power_on_av = UART_WAKEUP_SUB_REASON_CVBS_3;                                
                                    SYS_PrintString(1, "AV3");
                                     break;
                                case 4:
                                    power_on_av = UART_WAKEUP_SUB_REASON_PC_1;                                                                
                                    SYS_PrintString(1, "pc1");
                                    break;
                                case 5:
                                    power_on_av = UART_WAKEUP_SUB_REASON_YPBPR_1; 
                                    SYS_PrintString(1, "Ypbpr");
                                    break;
                                case 6:
                                    if(u1Module2HdmiPortSel == 0)
                                    {
                                        power_on_av = UART_WAKEUP_SUB_REASON_HDMI_1; 
                                        SYS_PrintString(1, "HDMI1");
                                    }
                                    else if(u1Module2HdmiPortSel == 1)
                                    {
                                        power_on_av = UART_WAKEUP_SUB_REASON_HDMI_2; 
                                        SYS_PrintString(1, "HDMI2");
                                    }
                                    else if(u1Module2HdmiPortSel == 2)
                                    {
                                        power_on_av = UART_WAKEUP_SUB_REASON_HDMI_3; 
                                        SYS_PrintString(1, "HDMI3");
                                    }
                                    else if(u1Module2HdmiPortSel == 3)
                                    {
                                        power_on_av = UART_WAKEUP_SUB_REASON_HDMI_4; 
                                        SYS_PrintString(1, "HDMI4");
                                    }
                                    else
                                    {
                                        power_on_av = UART_WAKEUP_SUB_REASON_ILLEGAL; 
                                        SYS_PrintString(1, "Illegal0");
                                    }
                                    break;
                                case 16:
                                    power_on_av = UART_WAKEUP_SUB_REASON_HDMI_1; 
                                    SYS_PrintString(1, "HDMI16");
                                    break;
                                default:
                                    power_on_av = UART_WAKEUP_SUB_REASON_ILLEGAL;
                                    SYS_PrintString(1, "Illegal1");
                                    break;
                            }//switch
                            if(power_on_av != UART_WAKEUP_SUB_REASON_ILLEGAL)
                            {
#if NO_SEND_TEST 

#else
									if(u1HotelDbg)//if(MGET_SYS_UART_BAUDRATE() == BR_9600)
									{
										SYS_PrintString(1, "010500ff0005"); 					
									}
                                    vRegWrite1B(PDWNC_DATAW,0x01);
                                    vRegWrite1B(PDWNC_DATAW,0x05);            
                                    vRegWrite1B(PDWNC_DATAW,0x00);            
                                    vRegWrite1B(PDWNC_DATAW,0xff);            
                                    vRegWrite1B(PDWNC_DATAW,0x00);  //1030                                                  
                                    vRegWrite1B(PDWNC_DATAW,0x05);                                    
#endif
									vKrDelay10ms(2);
#if PERFORMANCE_DBG
									if(w_cmd)
									{
										if(cmd_chg)
										{
											vRegWrite1B(PDWNC_DATAW,0xaa);
											vRegWrite1B(PDWNC_DATAW,0xaa);
											vRegWrite1B(PDWNC_DATAW,0xaa);
											vRegWrite1B(PDWNC_DATAW,0xaa);
											vRegWrite1B(PDWNC_DATAW,0xaa);
											vRegWrite1B(PDWNC_DATAW,0xaa);
											vRegWrite1B(PDWNC_DATAW,0xaa);
											vRegWrite1B(PDWNC_DATAW,0xaa);
										}
										else
										{
											vRegWrite1B(PDWNC_DATAW,0xcc);
											vRegWrite1B(PDWNC_DATAW,0xcc);
											vRegWrite1B(PDWNC_DATAW,0xcc);
											vRegWrite1B(PDWNC_DATAW,0xcc);
											vRegWrite1B(PDWNC_DATAW,0xcc);
											vRegWrite1B(PDWNC_DATAW,0xcc);
											vRegWrite1B(PDWNC_DATAW,0xcc);
											vRegWrite1B(PDWNC_DATAW,0xcc);
										}
										w_cmd = 0;
									}
#else
									vWakeUpSystem(PDWNC_WAKE_UP_REASON_UART, power_on_av);
#endif
                            }//if(power_on_av != UART_WAKEUP_SUB_REASON_ILLEGAL)
                        }//if((uRs232Data[3] == 0) || (uRs232Data[3] == u1Module2AvSelectPW))
                        else
                        {
                            #if NO_SEND_TEST
    
                            #else
    						//if(u4RegRead4B(PDWNC_RESRV0)!= 0x12345678)				
    						if(u1HotelDbg)//if(MGET_SYS_UART_BAUDRATE() == BR_9600)
    						{
    							SYS_PrintString(1, "010500ff0005");						
    						}
    						else
    						{
    	                        vRegWrite1B(PDWNC_DATAW,0x01);
    	                        vRegWrite1B(PDWNC_DATAW,0x05);            
    	                        vRegWrite1B(PDWNC_DATAW,0x00);            
    	                        vRegWrite1B(PDWNC_DATAW,0xff); 
    	                        vRegWrite1B(PDWNC_DATAW,0x00);  //1030  							
    	                        vRegWrite1B(PDWNC_DATAW,0x05);            
    						}							
                            #endif
							vKrDelay10ms(2);
                            vWakeUpSystem(PDWNC_WAKE_UP_REASON_UART, fgHotelCmdData.uRs232Data[3]);                            
                        }
                    }//if((0x06 + fgHotelCmdData.uRs232Data[3]) == fgHotelCmdData.uRs232Data[4])
                }//((fgHotelCmdData.uRs232Data[0] == 0x82) && (fgHotelCmdData.uRs232Data[1] == 0x04) && (fgHotelCmdData.uRs232Data[2] == 0x80))
				else
				{
        			if(VALID_LOCATEL_CMD())
        			{
						u1Module2AliveByte1 = 0x8;
						u1Module2AliveByte3 = 0x4;
        			}
				}
				fgHotelCmdData.uRs232Rxidx = 0;
				fgHotelCmdData.fgRs232Rx = 0;
            }//        case _1SEC_RECEIVE_MSG_STATE:            if(fgHotelCmdData.fgRs232Rx)
            if(!fgIsTimerUp(HOTEL_MODULE2_STATE_MACHINE_LONG_TIMER_ID))
            {
                u1Hotel_module2_state = START_100_MSEC_TIMER_STATE;
            }
            else
            {
				SYS_PrintString(1, "jump to CHANGING_TO_2SEC_STATE"); 
                u1Hotel_module2_state = CHANGING_TO_2SEC_STATE;
            }
#if PERFORMANCE_DBG
			if(w_cmd)
			{
					vRegWrite1B(PDWNC_DATAW,0x11);
					vRegWrite1B(PDWNC_DATAW,0x11);
					vRegWrite1B(PDWNC_DATAW,0x11);
					vRegWrite1B(PDWNC_DATAW,0x11);
			}
#endif			
            break;
        case CHANGING_TO_2SEC_STATE:
			u1HotelModule2OnLine = 0;
			MIR_EN_HW_WAKEUP();
            u1Hotel_module2_state = WAIT_2SEC_TIMER_TIMEOUT_STATE;
			vSetTimer(HOTEL_MODULE2_STATE_MACHINE_LONG_TIMER_ID, TIMER_2_SEC);
			SYS_PrintString(1, "CHANGING_TO_2SEC_STATE"); 
#if PERFORMANCE_DBG
			if(w_cmd)
			{
					vRegWrite1B(PDWNC_DATAW,0x22);
					vRegWrite1B(PDWNC_DATAW,0x22);
					vRegWrite1B(PDWNC_DATAW,0x22);
					vRegWrite1B(PDWNC_DATAW,0x22);
			}
#endif			
	         break;
        case WAIT_2SEC_TIMER_TIMEOUT_STATE:
            if(fgIsTimerUp(HOTEL_MODULE2_STATE_MACHINE_LONG_TIMER_ID))
            {
				SYS_PrintString(1, "WAIT_2SEC_TIMER_TIMEOUT_STATE"); 
                u1Hotel_module2_state = _2SEC_RECEIVE_MSG_STATE;
                #if NO_SEND_TEST

                #else
//				if(u4RegRead4B(PDWNC_RESRV0)!= 0x12345678)
				if(u1HotelDbg)//if(MGET_SYS_UART_BAUDRATE() == BR_9600)
				{
					SYS_PrintString(1, "0104544fa7");
				}
				else
				{
	                vRegWrite1B(PDWNC_DATAW,0x00);
	                vRegWrite1B(PDWNC_DATAW,0x04);            
	                vRegWrite1B(PDWNC_DATAW,0x54);            
	                vRegWrite1B(PDWNC_DATAW,0x4f);            
	                vRegWrite1B(PDWNC_DATAW,0xA7);
				}
                #endif                
                vSetTimer(HOTEL_MODULE2_STATE_MACHINE_SHORT_TIMER_ID, TIMER_100_MSEC);//david : spec. is not define this timing
            }
#if PERFORMANCE_DBG
			if(w_cmd)
			{
					vRegWrite1B(PDWNC_DATAW,0x33);
					vRegWrite1B(PDWNC_DATAW,0x33);
					vRegWrite1B(PDWNC_DATAW,0x33);
					vRegWrite1B(PDWNC_DATAW,0x33);
			}
#endif			
            break;
        case _2SEC_RECEIVE_MSG_STATE:
            if(fgHotelCmdData.fgRs232Rx)
            {
				SYS_PrintString(3, "_2SEC_RECEIVE_MSG_STATE");
                if((fgHotelCmdData.uRs232Data[0] == 0x80) && (fgHotelCmdData.uRs232Data[1] == 0x08) && (fgHotelCmdData.uRs232Data[2] == 0x32) && (fgHotelCmdData.uRs232Data[3] == 0x4d) && (fgHotelCmdData.uRs232Data[4] == 0x47) && (fgHotelCmdData.uRs232Data[5] == 0x54) && (fgHotelCmdData.uRs232Data[6] == 0x49) && (fgHotelCmdData.uRs232Data[7] == 0x50) && (fgHotelCmdData.uRs232Data[8] == 0x3b)) 
                {
					SYS_PrintString(1, "F2T");
                    u1Hotel_module2_state = CHANGING_TO_10SEC_STATE;
                }
				fgHotelCmdData.uRs232Rxidx = 0;
				fgHotelCmdData.fgRs232Rx = 0;
            }
            if(fgIsTimerUp(HOTEL_MODULE2_STATE_MACHINE_SHORT_TIMER_ID))
            {
                u1Hotel_module2_state = CHANGING_TO_2SEC_STATE;
                SYS_PrintString(3, "2_retry\n"); //sony ir debug
            }
#if PERFORMANCE_DBG
			if(w_cmd)
			{
					vRegWrite1B(PDWNC_DATAW,0x44);
					vRegWrite1B(PDWNC_DATAW,0x44);
					vRegWrite1B(PDWNC_DATAW,0x44);
					vRegWrite1B(PDWNC_DATAW,0x44);
			}
#endif			
            break;
        default:
#if PERFORMANCE_DBG
			if(w_cmd)
			{
					vRegWrite1B(PDWNC_DATAW,0x55);
					vRegWrite1B(PDWNC_DATAW,0x55);
					vRegWrite1B(PDWNC_DATAW,0x55);
					vRegWrite1B(PDWNC_DATAW,0x55);
			}
#endif			
            break;
    }
}

void Sony_uart_command_parse_module1(void)
{
//    if(u1CustomData1_HotelModule == 0x02 || u1CustomData1_HotelModule == 0x03)
	   if(fgHotelCmdData.fgRs232Rx)
	   {
		   fgHotelCmdData.uRs232Rxidx = 0;
		   fgHotelCmdData.fgRs232Rx = 0;
        //	if( (uRs232Data[0]==0x70) && (uRs232Data[1]==0) && (uRs232Data[2]==0x70))
            if((fgHotelCmdData.uRs232Data[0]==0x83) && (fgHotelCmdData.uRs232Data[1]==0) && (fgHotelCmdData.uRs232Data[2]==0)&& (fgHotelCmdData.uRs232Data[3]==0xff)&& (fgHotelCmdData.uRs232Data[4]==0xff)&& (fgHotelCmdData.uRs232Data[5]==0x81))	    
        	{
//				if(u4RegRead4B(PDWNC_RESRV0)!= 0x12345678)	
				if(u1HotelDbg)//if(MGET_SYS_UART_BAUDRATE() == BR_9600)
				{
					SYS_PrintString(1, "7000020072");
				}
				else
				{        	
                    vRegWrite1B(PDWNC_DATAW,0x70);
                    vRegWrite1B(PDWNC_DATAW,0x00);            
                    vRegWrite1B(PDWNC_DATAW,0x02);            
                    vRegWrite1B(PDWNC_DATAW,0x00);            
                    vRegWrite1B(PDWNC_DATAW,0x72);            
				}
            }
            else if(((u1CustomData3_Module1ReplyData & 0x01) == 1) && (fgHotelCmdData.uRs232Data[0]==0x8c) && (fgHotelCmdData.uRs232Data[1]==0) && (fgHotelCmdData.uRs232Data[2]==0) && (fgHotelCmdData.uRs232Data[3]==0x02)&& (fgHotelCmdData.uRs232Data[4]==0x01)&& (fgHotelCmdData.uRs232Data[5]==0x8f))	    
        	{
				   //if(u4RegRead4B(PDWNC_RESRV0)!= 0x12345678)			   
				   if(u1HotelDbg)//if(MGET_SYS_UART_BAUDRATE() == BR_9600)
				   {
						SYS_PrintString(1, "700070");
				   }
				   else
				   {
	                    vRegWrite1B(PDWNC_DATAW ,0x70);
	                    vRegWrite1B(PDWNC_DATAW ,0x00);            
	                    vRegWrite1B(PDWNC_DATAW ,0x70);
				   	}
                  // vRegWrite4B(0x0120, u4RegRead4B(0x0120)|0x00020000); //Enable 8032 wake up
                  vKrDelay10ms(2);
				  MSET_SYS_SERVICE_MODE(5);//
                  vWakeUpSystem(/*WAKEUP_UART*/PDWNC_WAKE_UP_REASON_IRRC, 0); //david: can't use uart as wakeup reason since we don't have ch information here
            }
        	//SYS_PrintString(3, "Sony_uart_command_parse_module1");
    }
}

static void S_command_read_in(void)
{
	//	memcpy(&fgHotelCmdData, pGetUartDrvObj(), sizeof(UART_DRV_OBJ));
#if 1
	UINT8 i, j, k;
	UART_DRV_OBJ *pUartObj;
	if(!(fgHotelCmdData.fgRs232Rx))
	{
		pUartObj = pGetUartDrvObj();
#if PERFORMANCE_DBG
w_cmd = 0;
for(k = 0; k < 	pUartObj->uRs232Rxidx; k++)
{
	if(pUartObj->uRs232Data[k] == 0x82)
		w_cmd = 1;
}
#endif
		fgHotelCmdData.uRs232Rxidx = pUartObj->uRs232Rxidx;
		fgHotelCmdData.fgRs232Rx = pUartObj->fgRs232Rx;
		SYS_PrintString_B(1, "S_command_read_in", fgHotelCmdData.uRs232Rxidx);
		//pin pon buffer...
		i = 0;
		j = pUartObj->uRs232Rxidx;
		if( (pUartObj->uRs232Data[0]==0x81) && (pUartObj->uRs232Data[1]==0x03) && (pUartObj->uRs232Data[2]==0x80) && (pUartObj->uRs232Data[3]==0x04) && ((pUartObj->uRs232Rxidx) > 4))		
		{
			i = 4;
			j = (pUartObj->uRs232Rxidx) - 4;
			fgHotelCmdData.uRs232Rxidx = (pUartObj->uRs232Rxidx) - 4;
			
#if  PERFORMANCE_DBG
cmd_chg = 1;
#endif
#if 0
vRegWrite1B(PDWNC_DATAW,0xbb);
vRegWrite1B(PDWNC_DATAW,0xbb);
vRegWrite1B(PDWNC_DATAW,0xbb);
vRegWrite1B(PDWNC_DATAW,0xbb);
vRegWrite1B(PDWNC_DATAW,0xbb);
vRegWrite1B(PDWNC_DATAW,0xbb);
vRegWrite1B(PDWNC_DATAW,0xbb);
vRegWrite1B(PDWNC_DATAW,0xbb);
#endif
		}
#if  PERFORMANCE_DBG		
		else
		{
			cmd_chg = 0;
		}
#endif		
		for(k = 0; k < j; i++, k++)
		{
			fgHotelCmdData.uRs232Data[k] = pUartObj->uRs232Data[i];
			SYS_Print8bitsHex(1, fgHotelCmdData.uRs232Data[k]);
//vRegWrite1B(PDWNC_DATAW,0xbb);						
		}
		//SYS_PrintString(1, "S_command_read_in");
		//SYS_PrintString_B(1," HotelModeCmdFlag", fgHotelCmdData.fgRs232Rx);
		//SYS_PrintString_B(1, " u1Hotel_module2_state", u1Hotel_module2_state); 
		//SYS_PrintString_B(1, " u1HotelModeModule", u1HotelModeModule); 		
	}
	else
	{
		SYS_PrintString(1, "some data lost!");
	}
#else
UINT8 i;	
UART_DRV_OBJ *pUartObj; 
if(!(fgHotelCmdData.fgRs232Rx)) 
{
	pUartObj = pGetUartDrvObj();
	fgHotelCmdData.uRs232Rxidx = pUartObj->uRs232Rxidx;
	fgHotelCmdData.fgRs232Rx = pUartObj->fgRs232Rx;
	SYS_PrintString_B(1, "S_command_read_in", fgHotelCmdData.uRs232Rxidx);
	//pin pon buffer... 
	for(i = 0; i < (pUartObj->uRs232Rxidx); i++)
	{
		fgHotelCmdData.uRs232Data[i] = pUartObj->uRs232Data[i];
		SYS_Print8bitsHex(1, fgHotelCmdData.uRs232Data[i]); 	
	}
	//SYS_PrintString(1, "S_command_read_in");
	//SYS_PrintString_B(1," HotelModeCmdFlag", fgHotelCmdData.fgRs232Rx);
	//SYS_PrintString_B(1, " u1Hotel_module2_state", u1Hotel_module2_state);
	//SYS_PrintString_B(1, " u1HotelModeModule", u1HotelModeModule);
}
else
{
	SYS_PrintString(1, "some data lost!");	
}
#endif
}

void vTask8S_command_parse(void)
{
	//SYS_PrintString_B(3, "vTask8S_command_parse", u1HotelModeModule );

		if(u1HotelModeModule == MODULE1)
		{
			Sony_uart_command_parse_module1();
		}
		else if(u1HotelModeModule == MODULE2)
		{
			vHotelMode2_StateMachine();
		}
	}
#endif


#if UART_TASK
void vRS232CustTaskInit(void)
{
#if S_HOTEL_MODE_COMMAND
    u1CustomData1_HotelModule = 0;
    u1CustomData3_Module1ReplyData = 0;
    u1hotel_module2_init_mode = IDLE_MODE;
    u1Hotel_module2_state = INIT_STATE;
    u1HotelModeModule = NO_MODULE;
	u1Module2Enable = 0;
	u1HotelDbg = 0;
	u1Module2AvSelectPW = 0;
    u1Module2AvSelectSrc = 0;
	u1Module2HdmiPortSel = 0;
	u1HotelModule2OnLine = 0;
/*    fgTimerLedControlEn = 0; 
    u1TimerLedMode = DEPEND_ON_TV;
    u1TimerLedCnt = 0;    
    fgTimerLedStatus = 0;
*/    
	vSetTimer(HOTEL_MODULE2_STATE_MACHINE_LONG_TIMER_ID, 0);
	vSetTimer(HOTEL_MODULE2_STATE_MACHINE_SHORT_TIMER_ID, 0);
//#if HAVE_HOTEL_GPIO_INDICATED	
//    ClrGPIOEN(MODULE2_STATUS_PIN_MASK);
//    bModule2Status = 0;//(ReadGPIOIN(MODULE2_STATUS_PIN_MASK)) ? 1: 0;//(UINT8)(ReadGPIOIN(MODULE2_STATUS_PIN_MASK));
//#endif    
#endif    
}

void vRS232CustTaskWakeup(void)
{
#if S_HOTEL_MODE_COMMAND
	//#if HAVE_HOTEL_GPIO_INDICATED 
	//	ClrGPIOEN(MODULE2_STATUS_PIN_MASK);
//(ReadGPIOIN(MODULE2_STATUS_PIN_MASK)) ? 1: 0;//(UINT8)(ReadGPIOIN(MODULE2_STATUS_PIN_MASK));
	//#endif	
	vRS232CustTaskInit();
	memset(&fgHotelCmdData, 0, sizeof(UART_DRV_OBJ));
#endif
}

void vRS232CustTaskStandby(void)
{
#if S_HOTEL_MODE_COMMAND
//            vRegWrite1B(PDWNC_INT_ENABLE + 1, u1RegRead1B(PDWNC_INT_ENABLE + 1)  | 0x02);//david: uart enable
//vRS232CustTaskInit();

#if 0//only for debug!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
u1CustomData1_HotelModule = 0x04;
u1hotel_module2_init_mode = 1;
u1CustomData3_Module1ReplyData = 1;
#endif   
		memset(&fgHotelCmdData, 0, sizeof(UART_DRV_OBJ));
        if((u1CustomData1_HotelModule == 0x02) || (u1CustomData1_HotelModule == 0x03) /*&& ((u1CustomData2 & 0x01) == 1)*/)
        {
            SYS_PrintString(1, "Hotel1\n"); //sony ir debug
            u1HotelModeModule = MODULE1;
        }
        else if((u1CustomData1_HotelModule == 0x04) && (u1Module2Enable))
        {
            SYS_PrintString(1, "Hotel2\n"); //sony ir debug
            u1HotelModeModule = MODULE2;
            
            if(u1hotel_module2_init_mode == 1)
            {
                SYS_PrintString(1, "online_t\n"); //sony ir debug
                u1Hotel_module2_state = CHANGING_TO_10SEC_STATE;
            }
            else
            {
                SYS_PrintString(1, "online_f\n"); //sony ir debug
                u1Hotel_module2_state = CHANGING_TO_2SEC_STATE;
            }
        }
        else
        {
            SYS_PrintString(1,  "No_HOTEL_MODULE\n"); //sony ir debug
            u1HotelModeModule = NO_MODULE;
        }
		bModule2Status = 0;
//		SYS_PrintString(1, "vRS232CustTaskStandby");
		SYS_PrintString_B(1, "u1CustomData1_HotelModule",u1CustomData1_HotelModule);
		SYS_PrintString_B(1, "u1CustomData3_Module1ReplyData",u1CustomData3_Module1ReplyData);
		SYS_PrintString_B(1, "u1hotel_module2_init_mode",u1hotel_module2_init_mode);
		SYS_PrintString_B(1, "u1Hotel_module2_state",u1Hotel_module2_state);
		SYS_PrintString_B(1, "u1HotelModeModule",u1HotelModeModule);
		SYS_PrintString_B(1, "u1Module2Enable", u1Module2Enable);
		SYS_PrintString_B(1, "u1HotelDbg", u1HotelDbg);		
//		SYS_PrintString_B(1, "fgRs232Rx", fgHotelCmdData.fgRs232Rx);
//		SYS_PrintString_B(1, "uRs232Rxidx", fgHotelCmdData.uRs232Rxidx);
		SYS_PrintString_B(1, "u1Module2AvSelectPW", u1Module2AvSelectPW);
		SYS_PrintString_B(1, "u1Module2AvSelectSrc", u1Module2AvSelectSrc);
		SYS_PrintString_B(1, "u1Module2HdmiPortSel", u1Module2HdmiPortSel);		
/*		
    fgTimerLedControlEn = 0; 
    u1TimerLedMode = DEPEND_ON_TV;
	vSetTimer(HOTEL_MODULE2_STATE_MACHINE_LONG_TIMER_ID, 0);
    u1Hotel_module2_short_cnt = 0;
    fgTimerLedStatus = 0;
*/
#endif
}

void vRS232CustTask(void)
{
//Due to UART Data will be flushed in each funciton, only one define can be enabled !!!
#if S_HOTEL_MODE_COMMAND
      S_command_read_in();
#endif
}
#endif


#if SUPPORT_ERROR_LED_CTRL
Error_Code_Ctrl gErrCodeCtrl;
/*
void SetCtrlLedEnable(void)
{
    if(gErrCodeCtrl.fgErrCodeLedType == 1)
    {
        SetSERVOEN((UINT32)(gErrCodeCtrl.fgErrCodeLedGpioIdx));
        SetSERVOPADEN((UINT32)(gErrCodeCtrl.fgErrCodeLedGpioIdx));
    }
    else
    {
        SetGPIOEN((UINT32)(gErrCodeCtrl.fgErrCodeLedGpioIdx));
    }
}
*/
static void Sony_GPIO_SetOut(INT16 i4Gpio, INT16 i4fgSet) reentrant
{
	UINT32 u4Val;
	INT8 i4Idx;

	i4Idx = i4Gpio - 200;
	if (i4Idx <= 31)
	{
	u4Val = u4IO32Read4B(PDWNC_GPIOOUT0);
	}
	else
	{
		u4Val = u4IO32Read4B(PDWNC_GPIOOUT1);
	}
		
	i4Idx &= 0x1f;
	
	u4Val = (i4fgSet) ?
		(u4Val | (1L << i4Idx)) :
	(u4Val & ~(1L << i4Idx));
	
	if (i4Idx <= 31)
	{
		vIO32Write4B(PDWNC_GPIOOUT0, u4Val);
	}
	else
	{
		vIO32Write4B(PDWNC_GPIOOUT1, u4Val);
	}
}

static void TurnOnCtrlLed(void) //reentrant
{
	//SYS_PrintString(1, "TurnOnCtrlLed");
    if(gErrCodeCtrl.fgErrCodeLedPolarity)
        Sony_GPIO_SetOut((INT16)(gErrCodeCtrl.fgErrCodeLedGpioIdx), 1);
    else
        Sony_GPIO_SetOut((INT16)(gErrCodeCtrl.fgErrCodeLedGpioIdx), 0); 
}

static void TurnOffCtrlLed(void)
{
	//SYS_PrintString(1, "TurnOffCtrlLed");
    if(gErrCodeCtrl.fgErrCodeLedPolarity)        
        Sony_GPIO_SetOut((INT16)(gErrCodeCtrl.fgErrCodeLedGpioIdx), 0);
    else
        Sony_GPIO_SetOut((INT16)(gErrCodeCtrl.fgErrCodeLedGpioIdx), 1);
}

  
void vSonyErrCodeLedReset(void)
{
    vSetTimer(ERROR_LED_TIMER_ID, gErrCodeCtrl.fgErrCodeLedPeriod);
    gErrCodeCtrl.fgErrCodeBlinkingTimeCnt =gErrCodeCtrl.fgErrCodeBlinkingTime;
}

void vTask4Init_error_led_ctrl(void)
{
    memset(&(gErrCodeCtrl), 0, sizeof(Error_Code_Ctrl));
    gErrCodeCtrl.fgSonyErrLedStateMachine = SONY_ERROR_CODE_STATE_1;    
}

void vTask4_error_led_ctrl_state_machine(void)
{
    if((MGET_ARM_POWER_STATE() == ARM_POWER_OFF) && (gErrCodeCtrl.fgErrCodeBlinkingTime))		
    {
        switch (gErrCodeCtrl.fgSonyErrLedStateMachine)
        {
		    case SONY_ERROR_CODE_STATE_INIT:
			     if(fgIsTimerUp(ERROR_LED_TIMER_ID))
                {
                    vSetTimer(ERROR_LED_TIMER_ID, gErrCodeCtrl.fgErrCodeLedPeriod);
                    gErrCodeCtrl.fgSonyErrLedStateMachine = SONY_ERROR_CODE_STATE_1;
    				SYS_PrintString(1, "st0");
					TurnOnCtrlLed();
                }
			break;
            case SONY_ERROR_CODE_STATE_1:
                if(fgIsTimerUp(ERROR_LED_TIMER_ID))
                {
                    vSetTimer(ERROR_LED_TIMER_ID, gErrCodeCtrl.fgErrCodeLedPeriod);
                    gErrCodeCtrl.fgSonyErrLedStateMachine = SONY_ERROR_CODE_STATE_2;
    				SYS_PrintString(1, "st1");
					TurnOffCtrlLed();
                }
                break;
            case SONY_ERROR_CODE_STATE_2:
                if(fgIsTimerUp(ERROR_LED_TIMER_ID))
                {
                    if(gErrCodeCtrl.fgErrCodeBlinkingTimeCnt)
                    {
                         gErrCodeCtrl.fgErrCodeBlinkingTimeCnt--;
                    }
                    if(gErrCodeCtrl.fgErrCodeBlinkingTimeCnt)
                    {
                         vSetTimer(ERROR_LED_TIMER_ID, gErrCodeCtrl.fgErrCodeLedPeriod);
                         gErrCodeCtrl.fgSonyErrLedStateMachine = SONY_ERROR_CODE_STATE_1;
		                 TurnOnCtrlLed();
                    }
                    else
                    {
                        vSetTimer(ERROR_LED_TIMER_ID, gErrCodeCtrl.fgErrCodeLedInterval);
                        gErrCodeCtrl.fgSonyErrLedStateMachine = SONY_ERROR_CODE_STATE_4;
		                TurnOffCtrlLed();            
                     }
				SYS_PrintString(1, "st2");
                }
                break;
    /*            
            case SONY_ERROR_CODE_STATE_3:
                TurnOnCtrlLed();            
                if(fgSonyErrLedTimerCnt == 0)
                {
                     fgSonyErrLedTimerCnt =  fgErrCodeLedInterval;
                     fgSonyErrLedStateMachine = SONY_ERROR_CODE_STATE_4;
    	#if UART_DBG
    		vRegWrite1B(PDWNC_DATAW,0xA3); //debug
    		vRegWrite1B(PDWNC_DATAW,fgSonyErrLedStateMachine); //debug
    		vRegWrite1B(PDWNC_DATAW,0xA3); //debug		
            #endif                 
                     
                }
                break;
    */            
            case SONY_ERROR_CODE_STATE_4:
//                TurnOffCtrlLed();            
                if(fgIsTimerUp(ERROR_LED_TIMER_ID))
                {
                    vSonyErrCodeLedReset();
					gErrCodeCtrl.fgSonyErrLedStateMachine = SONY_ERROR_CODE_STATE_1;
#if UART_DBG
					SYS_PrintString(1, "st4");
#endif				   
                    TurnOnCtrlLed();
                }                
                break;

            default:
                break;
        }
    }
}
void vTask4WakeupProcess_error_led_ctrl(void)
{
    if(gErrCodeCtrl.fgErrCodeBlinkingTime)//david: error code may be light on at this monent
    {
        TurnOffCtrlLed();
    }
}

void vTask4StandbyProcess_error_led_ctrl(void)
{
  
}

#endif




#if S_HOTEL_MODE_COMMAND
Led_Toggle_Ctrl gLedToggleCtrl;
void vTask6LedToggleCtrl_Init(void)
{
	memset(&(gLedToggleCtrl), 0, sizeof(Led_Toggle_Ctrl));
}

void TurnOnToggleLed(void)
{
	if(MGET_ARM_POWER_STATE() == ARM_POWER_OFF)
    {
        if(gLedToggleCtrl.u1LedPolarity)
            GPIO_SetOut((INT16)(gLedToggleCtrl.u2LedGpio), 1);
        else
            GPIO_SetOut((INT16)(gLedToggleCtrl.u2LedGpio), 0); 
    }
}

void vTask6LedToggleCtrl_Wakeup_Process(void)
{
	vTask6LedToggleCtrl_Init();
}

void vTask6LedToggleCtrl_Standby_Process(void)
{
	//vTask6LedToggleCtrl_Init();
	 SYS_PrintString_B(1,"TimerLED on_off", gLedToggleCtrl.u1LedToggleEnable);
	 SYS_PrintString_W(1,"TimerLED gpio", gLedToggleCtrl.u2LedGpio);
	 SYS_PrintString_B(1,"TimerLED polarity", gLedToggleCtrl.u1LedPolarity);
}

void TurnOffToggleLed(void)
{
	if(MGET_ARM_POWER_STATE() == ARM_POWER_OFF)
    {
        if(gLedToggleCtrl.u1LedPolarity)
            GPIO_SetOut((INT16)(gLedToggleCtrl.u2LedGpio), 0);
        else
            GPIO_SetOut((INT16)(gLedToggleCtrl.u2LedGpio), 1); 
    }

}

void vTask6LedToggleCtrl_state_machine(void)
{
	if(gLedToggleCtrl.u1LedToggleEnable)
	{
		 if(fgIsTimerUp(LED_TOGGLE_TIMER_ID))
	 	{
			vSetTimer(LED_TOGGLE_TIMER_ID, gLedToggleCtrl.u1LedTogglePeriod);
			if(gLedToggleCtrl.u1LedCurrentValue == 1)
			{
				gLedToggleCtrl.u1LedCurrentValue = 0;
				TurnOnToggleLed();
			}
			else
			{
				gLedToggleCtrl.u1LedCurrentValue = 1;
				TurnOffToggleLed();
			}

	 	}
	}
}
/*
void vTask6LedToggleCtrl_Wakeup_Process(void)
{

}
void vTask6LedToggleCtrl_Standby_Process(void)
{

}
*/
void vSetToggleLedEnable(void)
{
/*
    if(gLedToggleCtrl.u1LedType == 1)
    {
        SetSERVOEN((UINT8)(gLedToggleCtrl.u2LedGpio));
        SetSERVOPADEN((UINT8)(gLedToggleCtrl.u2LedGpio));
    }
    else
    {
        SetGPIOEN(gLedToggleCtrl.u2LedGpio);
    }
*/
	gLedToggleCtrl.u1LedToggleEnable = 1;
	SYS_PrintString(1,"vSetToggleLedEnable");

}

void vSetToggleLedDisable(void)
{
	gLedToggleCtrl.u1LedToggleEnable = 0;
	TurnOffToggleLed();
	SYS_PrintString(1,"vSetToggleLedDisable");	
}

#endif

#if CY8C20110_SUPPORT
UINT8 u1Cypdata[3], u1Cypval;
SIF_REQ_OBJ pCypObj;
UINT8 u1CapSenseExist;
void vTask9KeypadCtrl_Standby_Process(void)
{
	pCypObj.u1DevAddr = 0x04;
	pCypObj.u1WordAddrNum = 1;
	pCypObj.u1DevClkDiv = 270;//0x100;
	pCypObj.u1WordAddr1st = 0x7a; 
	pCypObj.u1WordAddr2nd = 0; 
	pCypObj.bDataLen = 1; //pdbDataLen should lest than SIF_BUFFER_SIZE  
	pCypObj.pDataBuf = u1Cypdata;
	u1Cypval = bSIF_Read(&pCypObj);
	u1CapSenseExist = 0;
	if(u1Cypval == 0)
	{
		SYS_PrintString(1, "Cyp test fail");
	}
	else
	{
		SYS_PrintString_B(1, "Cyp read", u1Cypdata[0]);
		if(u1Cypdata[0] == 0x10)
		{
			u1CapSenseExist = 1;
		}
	}

}


void vTask9KeypadPolling(void)
{
	if(MGET_ARM_POWER_STATE() == ARM_POWER_OFF && u1CapSenseExist)
	{
		if(fgIsTimerUp(KEYPAD_POLLING_TIMER_ID))
		{

		    pCypObj.u1DevAddr = 0x04;
		    pCypObj.u1WordAddrNum = 1;
		    pCypObj.u1DevClkDiv = 270;//0x100;
		    pCypObj.u1WordAddr1st = 0; 
		    pCypObj.u1WordAddr2nd = 0; 
		    pCypObj.bDataLen = 1; //pdbDataLen should lest than SIF_BUFFER_SIZE  
		    pCypObj.pDataBuf = u1Cypdata;
		    u1Cypval = bSIF_Read(&pCypObj);
			if(u1Cypval == 0)
			{
				SYS_PrintString(1, "Cyp read fail");
			}
			else
			{
				SYS_PrintString_B(1, "Cyp read", u1Cypdata[0]);
				if(u1Cypdata[0] & 0x10)
				{
#if DISABLE_WAKEUP_SOMETIME
					if((gDelayWakeupCtrl.u1Enable == 0) || ((gDelayWakeupCtrl.u1Enable) && (fgIsTimerUp(DISABLE_WAKEUP_TIMER))))
					{
#endif				
						vWakeUpSystem(PDWNC_WAKE_UP_REASON_KEYPAD, 0);
#if DISABLE_WAKEUP_SOMETIME
					}
#endif					
				}
			}
			vSetTimer(KEYPAD_POLLING_TIMER_ID, 2);
		}
	}
}

#endif

void vTask_ex_rtc(void)
{
	//LogS("Get RTC OPCTRL13");
	if(MGET_ARM_POWER_STATE() == ARM_POWER_OFF)
	{
		#if 1//For the newest power state spec
		if(((u4IO32Read4B(PDWNC_WAKEN))& (1<<14))== 0)//Bit 14 is used for record status 0 for no use
		{
			return;
		}
		#endif
		if (GPIO_GetIn(213) == 0)
		{
			//LogS("Get the EX RTC low");
			vWakeUpSystem(PDWNC_WAKE_UP_REASON_RTC, 0);
		}
	}
}

