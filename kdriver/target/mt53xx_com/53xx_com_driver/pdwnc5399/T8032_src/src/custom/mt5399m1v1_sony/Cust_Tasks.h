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
#ifndef _CUST_TASKS_H_
#define _CUST_TASKS_H_

void vWDTRebootProcess(void);
void vSysCustWakeupProcess(void);
void vSysCustEnterPowerDownProcess(void);
#if UART_TASK
extern void vRS232CustTaskInit(void);
extern void vRS232CustTask(void);
extern void vRS232CustTaskWakeup(void);
extern void vRS232CustTaskStandby(void);
#endif
#if S_HOTEL_MODE_COMMAND
#define UART_WAKEUP_SUB_REASON_CVBS_1    200
#define UART_WAKEUP_SUB_REASON_CVBS_2    201
#define UART_WAKEUP_SUB_REASON_CVBS_3    202
#define UART_WAKEUP_SUB_REASON_CVBS_4    203
#define UART_WAKEUP_SUB_REASON_HDMI_1    204
#define UART_WAKEUP_SUB_REASON_HDMI_2    205
#define UART_WAKEUP_SUB_REASON_HDMI_3    206
#define UART_WAKEUP_SUB_REASON_HDMI_4    207
#define UART_WAKEUP_SUB_REASON_YPBPR_1   208
#define UART_WAKEUP_SUB_REASON_YPBPR_2   209
#define UART_WAKEUP_SUB_REASON_YPBPR_3   210
#define UART_WAKEUP_SUB_REASON_YPBPR_4   211
#define UART_WAKEUP_SUB_REASON_PC_1      212
#define UART_WAKEUP_SUB_REASON_ILLEGAL      255

enum _HOTEL_UART_MODULE
{
    NO_MODULE = 0x0,
    MODULE1,
    MODULE2
};


enum _HOTEL_MODULE2_MODE
{
    IDLE_MODE = 0x0,
    _1000MS_MODE,
    _2000MS_MODE
};

enum _HOTEL_MODULE2_STATE_MACHINE
{
    INIT_STATE = 0x0,
    CHANGING_TO_10SEC_STATE,        
    CHANGING_TO_2SEC_STATE,            
    START_100_MSEC_TIMER_STATE,
    WAIT_100_MSEC_TIMER_TIMEOUT_STATE,
    _1SEC_RECEIVE_MSG_STATE,
//    START_2SEC_TIMER_STATE,
    WAIT_2SEC_TIMER_TIMEOUT_STATE,
    _2SEC_RECEIVE_MSG_STATE
};

enum _TIMER_LED_CONTRL_MODE
{
    DEPEND_ON_TV = 0,
    SLOW_BLINKING,
    FAST_BLINKING,
    DO_NOTHING,
    ALWAYS_OFF,
    ALWAYS_ON
};

#define TIMER_10_SEC 500
#define TIMER_2_SEC 100
#define TIMER_100_MSEC 5
#define TIMER_1_SEC 50
#define TIMER_500_MSEC 25

extern void vTask8S_command_parse(void);
#define vTask8          vTask8S_command_parse
#endif

#if SUPPORT_ERROR_LED_CTRL
typedef struct _ErrorCodeCtrl
{
    UINT8 fgErrCodeLedPeriod;
    UINT8 fgErrCodeLedInterval;
    UINT8 fgErrCodeBlinkingTimeCnt; // 0: disable, non-0: blinking times
    UINT8 fgErrCodeBlinkingTime; // 0: disable, non-0: blinking times
    UINT8 fgErrCodeLedType; // 1: Servo 0: Opctrl
    UINT16 fgErrCodeLedGpioIdx;
    UINT8 fgErrCodeLedPolarity;
    UINT8 fgSonyErrLedStateMachine;
  }Error_Code_Ctrl;


typedef enum
{
    SONY_ERROR_CODE_STATE_INIT,
    SONY_ERROR_CODE_STATE_1,
    SONY_ERROR_CODE_STATE_2,
    SONY_ERROR_CODE_STATE_3,
    SONY_ERROR_CODE_STATE_4
}Error_Code_State;

extern void vTask4Init_error_led_ctrl(void);
extern void vTask4_error_led_ctrl_state_machine(void);
extern void vTask4WakeupProcess_error_led_ctrl(void);
extern void vTask4StandbyProcess_error_led_ctrl(void);
extern void vSonyErrCodeLedReset(void);
//extern void SetCtrlLedEnable(void);
void vSIR_RawTimerHwReset(void);

#define vTask4Init      vTask4Init_error_led_ctrl
#define vTask4          vTask4_error_led_ctrl_state_machine
#define vTask4WakeupProcess vTask4WakeupProcess_error_led_ctrl
#define vTask4StandbyProcess vTask4StandbyProcess_error_led_ctrl
#endif

#if CUST_B_NEC_IR_DRIVER
	extern void vCUST_B_IrTask(IR_DATA_TYPE a);	//Add by Cust_B 20091215
	extern void vTask5_InitCUST_B_IrInit(void);	//Add by Cust_B 20091215
	extern void vTask5_CUST_B_IrStateMachine(void);//Add by Cust_B 20091215
	#define vTask5Init         vTask5_InitCUST_B_IrInit	//Add by Cust_B 20091215
	#define vTask5      vTask5_CUST_B_IrStateMachine	//Add by Cust_B 20091215
    #define vIR_Task5	vCUST_B_IrTask		//Add by Cust_B 20091215
    #define CUST_B_NEC_POWER 0xFF009F60	//Add by Cust_B 20091215
	#define CUST_B_NEC_MENU 0xED129F60	//Add by Cust_B 20091215
	#define CUST_B_NEC_OK 0xAE519F60		//Add by Cust_B 20091215
	#define CUST_B_NEC_INFO 0xBA459F60	//Add by Cust_B 20091215

typedef enum
{
	IR_COMBINATION_KEY1_MATCH = 0,
	IR_COMBINATION_KEY2_MATCH,
	IR_COMBINATION_KEY3_MATCH,
	IR_COMBINATION_KEY4_MATCH,
	IR_COMBINATION_NoMatch
} CUST_B_IR_COMBINATION_KEY;	

typedef enum
{
	IR_TIMER_STAUTS_OFF = 0,
	IR_TIMER_STAUTS_ON	
} CUST_B_IR_TIMER_STAUTS;	
#endif	
#if CUS_SH_HOTEL_MODE 
#define PDWNC_CMD_ARMT8032_CUST_SH_CUSTDATA  (Cmd_Turnkey_Max + 1)
#endif



#if RC56_IR_DRIVER
    extern void vTask1_InitCust_P_IrInit(void);
    extern void vTask1_Cust_P_IrStateMachine(void);
    extern void vCUST_P_IrTask(IR_DATA_TYPE a);
    extern UINT8 XferToColorCfg(UINT32 u4SysCode);
    #define vTask1Init         vTask1_InitCust_P_IrInit
    #define vTask1              vTask1_Cust_P_IrStateMachine
    #define vIR_Task1       vCUST_P_IrTask
    #define vTask1WakeupProcess vTask1_InitCust_P_IrInit
    #define vTask1StandbyProcess    vTask1_InitCust_P_IrInit
    
    #define CUST_P_RC6_POWER 0x03980000

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
        IR_CH_SOURCE_IDX    
    }CUST_P_IR_KEY_IDX;


    typedef struct 
    {
        IR_DATA_TYPE u4PreviousPowerCode;
        IR_DATA_TYPE u4CurrentPowerCode;
        UINT8 fgIR_FilterFlag;
        UINT8 u1LPIR_CHK;
    } LPIR_struct;
    extern LPIR_struct gLPIR;
    #define IR_BLUE_CFG 		0x01
    #define IR_YELLOW_CFG 	0x02
    #define IR_RED_CFG 		0x04
    #define IR_GREEN_CFG 	0x08

    #define RC6_KEY_MUTE 		0x03D80000
    #define RC6_KEY_DIGIT0		0x03800000
    #define RC6_KEY_DIGIT1		0x03C00000
    #define RC6_KEY_DIGIT2		0x03A00000
    #define RC6_KEY_DIGIT3		0x03E00000
    #define RC6_KEY_DIGIT4		0x03900000
    #define RC6_KEY_DIGIT5		0x03D00000
    #define RC6_KEY_DIGIT6		0x03B00000
    #define RC6_KEY_DIGIT7		0x03F00000
    #define RC6_KEY_DIGIT8		0x03880000
    #define RC6_KEY_DIGIT9		0x03C80000
    #define RC6_KEY_OK			0x039D0000
    #define RC6_KEY_POWER		0x03980000
    #define RC6_KEY_AMBILIGHT	0x03f88000  // raw key.
    #define RC6_KEY_CH_UP       0x03820000
    #define RC6_KEY_CH_DOWN     0x03C20000
    #define RC6_KEY_PREVCH      0x03A80000
    #define RC6_KEY_SOURCE      0x038E0000
#endif

#if CUST_P_DC_ON_SEQUENCE
extern void vTask2Init_CUST_P_dc_on_state_machine_init(void);
extern void vTask2_CUST_P_dc_on_state_machine(void);
#define vTask2Init          vTask2Init_CUST_P_dc_on_state_machine_init
#define vTask2               vTask2_CUST_P_dc_on_state_machine    
#define vTask2StandbyProcess               vTask2Init_CUST_P_dc_on_state_machine_init    
#endif    

#if SUPORT_COLD_REBOOT
#define vTask3Init          vTask3Init_cold_rebootInit
#define vTask3               vTask3_cold_reboot_state_machine    
extern void vTask3Init_cold_rebootInit(void);
extern void vTask3_cold_reboot_state_machine(void);
#endif

#if S_HOTEL_MODE_COMMAND
typedef struct _LedToggleCtrl
{
    UINT8 u1LedTogglePeriod;
    UINT8 u1LedToggleEnable;
    UINT8 u1LedType; // 1: Servo 0: Opctrl
    UINT16 u2LedGpio;
    UINT8 u1LedPolarity;
    UINT8 u1LedCurrentValue;
    UINT8 u1TimerLedMode;
}Led_Toggle_Ctrl;
extern void vTask6LedToggleCtrl_Init(void);
extern void vTask6LedToggleCtrl_state_machine(void);
extern void vTask6LedToggleCtrl_Wakeup_Process(void);
extern void vTask6LedToggleCtrl_Standby_Process(void);
extern void vSetToggleLedEnable(void);
extern void vSetToggleLedDisable(void);

#define vTask6Init      vTask6LedToggleCtrl_Init
#define vTask6          vTask6LedToggleCtrl_state_machine
#define vTask6WakeupProcess vTask6LedToggleCtrl_Wakeup_Process
#define vTask6StandbyProcess vTask6LedToggleCtrl_Standby_Process
#endif
#if DISABLE_WAKEUP_SOMETIME
typedef struct _DelayWakeupCtrl
{
    UINT8 u1Enable;
    UINT8 u1DelayTimeCnt;
    UINT8 u1WakeupEnReg0; // 1: Servo 0: Opctrl
    UINT8 u1WakeupEnReg1;
    UINT8 u1WakeupEnReg2;
    UINT8 u1WakeupEnReg3;
}Delay_Wakeup_Ctrl;
#endif

#if SIR_TASK

#define SIR_KEY_DISPLAY 	0x00ba//0xba
#define SIR_KEY_DIGIT5		0x0084//0x84
#define SIR_KEY_TEST		0x00ff//0xff
#define SIR_KEY_PRG_UP	    0x0090//    0x90
#define SIR_KEY_PRG_DOWN	0x0091//0x91
#define SIR_KEY_VOL_PLUS	0x0092//0x92
#define SIR_KEY_VOL_MINUS	0x0093//0x93
#define SIR_KEY_MUTE	0x0094//0x94
#define SIR_KEY_POWER		0x0095//0x95
#define SIR_KEY_POWER_ON		0x00ae//0xAE
#define SIR_KEY_TV		0x00b8//0xb8
#define SIR_KEY_1		0x0080//0x80
#define SIR_KEY_0		0x0089//0x89
#define SIR_KEY_ADTV_SW	0x3b8d//0x8d
#define SIR_KEY_DTV		0x3bd2//0xd2//0xaa
#define SIR_KEY_ATV		SIR_KEY_TV
#define SIR_KEY_JUMP	0x00bb//0xbb
#define NO_SIRC_DATA    0x007e//0x7e//just take a number without used


#define TIME_100us 225
#define TIME_50us 112

#define TIME_400us (TIME_100us*4)
#define TIME_450us (TIME_400us + TIME_50us)
#define TIME_600us (TIME_100us*6)
#define TIME_700us (TIME_100us*7)

#define TIME_900us (TIME_100us*9)
#define TIME_500us (TIME_100us*5)
#define TIME_800us (TIME_100us*8)
#define TIME_1000us (TIME_100us*10)
#define TIME_1050us (TIME_1000us + TIME_50us)
#define TIME_1100us (TIME_100us*11)
#define TIME_1400us (TIME_100us*14)
#define TIME_1900us (TIME_100us*19)
#define TIME_2800us (TIME_100us*28)
#define VALID_OIRI_TOGGLE_CNT 26

typedef struct _SIRC_ServicModeCtrl
{
    IR_DATA_TYPE PrevEvent;
    UINT8 fgIsHotelCommand;
	UINT8 fgServiceMode;
    UINT8 u1WakeUp_waiting_idx;
}SIRC_ServiceMode_Ctrl;


typedef union IR_HW_TIMER
{
	struct IR_HW_TIMER_BYTES
	{
        UINT8 u1Ticks;
        UINT8 u1Cycles;
	}BYTES;

	UINT16	u2IrTimerVal;

}IR_HW_TIMER;


typedef struct 
{
    UINT16 u2Iteration;
    IR_HW_TIMER u2IrHwTimer;
}IR_Timer_T;


typedef struct 
{
	UINT16 u2fgTimer2Iteration;
#if QUERY_IR_POWER_KEY_STATE_SUPPORT	
	UINT8 _gu1ARM_IR_WORKING;
#endif

	IR_DATA_TYPE u2PreIrRxData0;
	UINT8 u1SoftcatDontCareFlag;
#if SONY_IR_DBG	
	UINT8 u1DeepDbg;
#endif
}SIRC_CTRL_T;

#define vTask7Init      		vTask7SIR_Task_Init
#if DISABLE_WAKEUP_SOMETIME
#define vTask7          		vTask7SIR_Task_StateMachine
#endif

#define vIR_Task7				vTask7SIR_Task
#define vTask7WakeupProcess 	vTask7SIR_Task_WakeupProcess
#define vTask7StandbyProcess 	vTask7SIR_Task_StandbyProcess
extern void _SonyIrParsing_OIRI(BIT fgPinStatus) small;
extern void _SonyIrParsing(void);
extern UINT16 wIrDiffRawTime(UINT8) small;
BIT _SonyIrDataCorrection(IR_DATA_TYPE) small;
BIT _IsGuidePulseInvalid(void) small;
BIT _IsTimeOn1Invalid(void) small;
BIT _IsTimeOn0Invalid(void) small;
BIT _IsTimeOffInvalid(void) small;
UINT8 IsSonyPowerKey(IR_DATA_TYPE data u1key);
extern void vTask7SIR_Task_WakeupProcess(void);
extern void vTask7SIR_Task(IR_DATA_TYPE a);
extern void vTask7SIR_Task_Init(void);
extern void vTask7SIR_Task_StandbyProcess(void);
#if DISABLE_WAKEUP_SOMETIME
extern void vTask7SIR_Task_StateMachine(void);
#endif

#endif


#ifdef ENABLE_CBUS
enum
{
    MHL_DDC_TIMER_ID,
	MHL_MSC_TIMER_ID,	
	MHL_ABORT_TIMER_ID,
	MHL_OCP_TIMER_ID,
	MHL_BOOTUP_TIMER_ID,
	MAX_TIMER_ID1
};
#endif

enum 
{
//    TEST_TIMER_ID,
	#if SUPPORT_ERROR_LED_CTRL
        ERROR_LED_TIMER_ID,
	#endif
	#if	S_HOTEL_MODE_COMMAND
		HOTEL_MODULE2_STATE_MACHINE_LONG_TIMER_ID,
		HOTEL_MODULE2_STATE_MACHINE_SHORT_TIMER_ID,		
	#endif
	#if S_HOTEL_MODE_COMMAND
		LED_TOGGLE_TIMER_ID,
	#endif
	#if QUERY_IR_POWER_KEY_STATE_SUPPORT
		SIR_LONG_PRESSING_CHK_TIMER_ID,
		SIR_FRAME_WAIT_TIMER_ID,
	#endif
	#if DISABLE_WAKEUP_SOMETIME
		DISABLE_WAKEUP_TIMER,
	#endif
    #if RC56_IR_DRIVER	    
        LPIR_SHORT_TIMER_ID,
        LPIR_LONG_TIMER_ID,
    #endif
    #if CUST_P_DC_ON_SEQUENCE
        DC_ON_Timer_Cnt,
    #endif
    #if SUPORT_COLD_REBOOT    
        COLD_REBOOT_TIMER_ID,
    #endif
#if CUST_B_NEC_IR_DRIVER      //Add by Cust_B 20091215
        POWER_ON_MUTIL_KEY_TIMER_ID,
#endif
	#if SUSPEND_MODE
	SUSPEND_MODE_TIMER_ID,
	#endif

	MAX_TIMER_ID
};

#endif

