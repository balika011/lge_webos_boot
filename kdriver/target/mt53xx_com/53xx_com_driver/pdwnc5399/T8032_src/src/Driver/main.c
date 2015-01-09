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
 *   $Workfile: main.c $ 
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
//#include "hw_rs232.h"
//#include "Hw_reg.h"
//#include "drv_hdmicec.h"
//#include "custom_def.h"
//#include "hw_bim.h"
//#include "drv_pdwnc.h"
//#include "drv_pdwnc.h"
//#include "Command_Task.h"
//#include "watchdog.h"
#include <stdio.h>
//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Imported functions
//-----------------------------------------------------------------------------
#if defined SIR_TASK
#if SIR_TASK
extern SIRC_CTRL_T idata gSIRC_ctrl;
#endif
#endif

//-----------------------------------------------------------------------------
// Prototypes
//-----------------------------------------------------------------------------
static void main(void);
static void _drv_init(void);
static void UNCALL_FUNS(void);
//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------


#if SUSPEND_MODE

UINT32 PreStandbySta;
UINT32 GoingSupend;
void vSuspendTaskInit(void)
{
	PreStandbySta = STBY_FROM_UNKNOW;
	GoingSupend = SUSPEND_STAGE0;
}	

void vSuspendTask(void)
{
	if((GoingSupend == SUSPEND_STAGE1) && (RegReadFldAlign(PDWNC_CONCFG0,FLD_RG_DMSUS_RESERVED) == 0))
	{
		#if UART_DBG
		LogS("enter suspend stage 1");
		#endif
		//set DMSUS =1 
		vRegWriteFldAlign(PDWNC_CONCFG1,1,FLD_RG_DMSUS);
		//wait 10 ms, but I must set timer 40ms at least
		vSetTimer(SUSPEND_MODE_TIMER_ID,SUSPEND_DELAY_POWER_SWITCH);
		GoingSupend = SUSPEND_STAGE2;
	}	
	else if((GoingSupend == SUSPEND_STAGE2) && fgIsTimerUp(SUSPEND_MODE_TIMER_ID))
	{
		#if UART_DBG
		LogS("enter suspend stage 2");
		#endif
		//suspend mode flag 
		//vRegWriteFldAlign(PDWNC_CONCFG0,1,FLD_RG_DMSUS_RESERVED);
		MSET_SYS_PRE_STANDBY_STATUS(PreStandbySta);
		vEnterPowerStandby();

		GoingSupend = SUSPEND_STAGE0;
	}
}	
#endif

//-----------------------------------------------------------------------------
/** _drv_init() is to initialize all drivers
  */
//-----------------------------------------------------------------------------
static void vDrvInit(void)
{
    vTimerInit();
    vPdwncInit();
    vWatchDogInit();
    vRs232Init(MGET_SYS_UART_BAUDRATE()); // init RS232 and enable RS232 interrupt

#ifdef ENABLE_AVLINK
    i4AVL_init();
#endif
}

//-----------------------------------------------------------------------------
/** task_init() is service initialization.
 */
//-----------------------------------------------------------------------------

void vTaskInit(void)
{
    vCmdTaskInit();

#if UART_TASK
    vRS232CustTaskInit();
#endif

#if SUSPEND_MODE
    vSuspendTaskInit();
#endif

#ifdef vTask1Init
    vTask1Init();
#endif

#ifdef vTask2Init
    vTask2Init();
#endif

#ifdef vTask3Init
    vTask3Init();
#endif

#ifdef vTask4Init
    vTask4Init();
#endif

#ifdef vTask5Init
    vTask5Init();
#endif

#ifdef vTask6Init
    vTask6Init();
#endif

#ifdef vTask7Init
    vTask7Init();
#endif

#ifdef vTask8Init
    vTask8Init();
#endif

#ifdef vTask9Init
    vTask9Init();
#endif

#ifdef vTask10Init
    vTask10Init();
#endif

#ifdef vTask11Init
    vTask11nit();
#endif

#ifdef vTask12Init
    vTask12Init();
#endif

#ifdef vTask13Init
    vTask13Init();
#endif

#ifdef vTask14Init
    vTask14Init();
#endif

#ifdef vTask15Init
    vTask15Init();
#endif

    
}
//-----------------------------------------------------------------------------
/** vCustTask() is customization of main body
 */
//-----------------------------------------------------------------------------
void vCustTask()
{
#ifdef vTask1
    vTask1();
#endif

#ifdef vTask2
    vTask2();
#endif

#ifdef vTask3
    vTask3();
#endif

#ifdef vTask4
    vTask4();
#endif

#ifdef vTask5
    vTask5();
#endif

#ifdef vTask6
    vTask6();
#endif

#ifdef vTask7
    vTask7();
#endif

#ifdef vTask8
    vTask8();
#endif

#ifdef vTask9
    vTask9();
#endif

#ifdef vTask10
    vTask10();
#endif

#ifdef vTask11
    vTask11();
#endif

#ifdef vTask12
    vTask12();
#endif

#ifdef vTask13
    vTask13();
#endif

#ifdef vTask14
    vTask14();
#endif

#ifdef vTask15
    vTask15();
#endif

}

#if ENABLE_CEC
void CECTask(void)
{
   CEC_Mainloop();
}
#endif

#if ENABLE_CBUS
void CBUSTask(void)
{
	vCbusMainLoop();
}
#endif


#if UART_TASK
void vRS232Task(void)
{
    MUART_DIS_INT();
    if(MGET_UART_STATE() == NEW_EVENT)
    {
        vRS232CustTask();
		MRESET_UART_RX_BUF_IDX();
     //   _LogS(" no evt ");
        MSET_UART_STATE(NO_EVENT);
    }
    MUART_EN_INT();    
}
#endif

#if (RW_REG_INTERFACE&&SIF_PDWNC_TEST)
extern UINT8 xdata u1pdsiffg; //_at_ 0xF800
extern UINT8 xdata u1pdsiflen; //_at_ 0xF801
extern UINT8 xdata u1pdsifdata; //_at_ 0xF802

void PDSIFTask(void)
{
if(u1pdsiffg == 1)
    PD_SIF_EEP_Write_Test((UINT8)0,(UINT8)0,u1pdsiflen,u1pdsifdata);
if(u1pdsiffg == 2)
    PD_SIF_EEP_Read_Test((UINT8)0,(UINT8)0,u1pdsiflen);
}
#endif






#ifdef VGA_WAKEUP_MANUAL_WAY
extern UINT8 _bSP0MuxSate;
extern UINT16 _bSP0MuxCnt;

void vPDSwitchVGAMux(void)
{		
	if( !(MGET_ARM_POWER_STATE() == ARM_POWER_OFF) || !(RegReadFldAlign(PDWNC_WAKEN, FLD_VGA_WAKEN)==1) )
	 	return;
	
	vIO32WriteFldAlign(PDWNC_VGA_WAKE_UP_CONTROL_1_REGISTER, 0x1, FLD_HLEN_VLEN_RESET );
	_bSP0MuxCnt = 0 ;
    
    switch (_bSP0MuxSate) 
    {
        case 0:
            _bSP0MuxSate = 1 ;
            
            vIO32WriteFldMulti(PDWNC_VGACFG1, P_Fld(1,FLD_RG_HSYNC_EN)|P_Fld(1,FLD_RG_VSYNC_EN)|P_Fld(0,FLD_RG_SOG_EN));
			vIO32WriteFldMulti(PDWNC_VGACFG4,P_Fld(4,FLD_RG_SYNC1_VTH)|P_Fld(2,FLD_RG_SYNC1_VTL));//HSYNC
                vIO32WriteFldAlign(PDWNC_VGA_WAKE_UP_CONTROL_1_REGISTER, 0x01, FLD_WAKEUP_INT_EN);
				vIO32WriteFldAlign(PDWNC_VGA_WAKE_UP_CONTROL_2_REGISTER, 0x0, FLD_HSYNC_SEL);
				vIO32WriteFldAlign(PDWNC_VGA_WAKE_UP_CONTROL_2_REGISTER, 0x0, FLD_VSYNC_SEL);
				vIO32WriteFldAlign(PDWNC_VGA_WAKE_UP_CONTROL_2_REGISTER, 0x0, FLD_CSYNC_SOG_SEL);				
				#if UART_DBG
				LogS("Ss");
				#endif
				vKrDelay1ms(5);
				if(RegReadFldAlign(PDWNC_VGA_WAKE_UP_STATUS_0_REGISTER, FLD_HSYNC_ACT) && RegReadFldAlign(PDWNC_VGA_WAKE_UP_STATUS_0_REGISTER, FLD_VSYNC_ACT))
                break;
        case 1:
            _bSP0MuxSate = 2 ;
           
		   vIO32WriteFldMulti(PDWNC_VGACFG1, P_Fld(0,FLD_RG_HSYNC_EN)|P_Fld(1,FLD_RG_VSYNC_EN)|P_Fld(1,FLD_RG_SOG_EN));
		   vIO32WriteFldMulti(PDWNC_VGACFG4,P_Fld(8,FLD_RG_SYNC1_VTH)|P_Fld(8,FLD_RG_SYNC1_VTL));//SOY
			   vIO32WriteFldAlign(PDWNC_VGA_WAKE_UP_CONTROL_1_REGISTER, 0x03, FLD_WAKEUP_INT_EN); 
			   vIO32WriteFldAlign(PDWNC_VGA_WAKE_UP_CONTROL_2_REGISTER, 0x2, FLD_HSYNC_SEL);
			   vIO32WriteFldAlign(PDWNC_VGA_WAKE_UP_CONTROL_2_REGISTER, 0x0, FLD_VSYNC_SEL);
			   vIO32WriteFldAlign(PDWNC_VGA_WAKE_UP_CONTROL_2_REGISTER, 0x1, FLD_CSYNC_SOG_SEL);
			   #if UART_DBG
			   LogS("Sog");
			   #endif
			   vKrDelay1ms(5);
			   if(RegReadFldAlign(PDWNC_VGA_WAKE_UP_STATUS_0_REGISTER, FLD_CSYNC_ACT) && !(RegReadFldAlign(PDWNC_VGA_WAKE_UP_STATUS_0_REGISTER, FLD_HSYNC_ACT)) 
			   																		&& !(RegReadFldAlign(PDWNC_VGA_WAKE_UP_STATUS_0_REGISTER, FLD_VSYNC_ACT)))
			   break;
			
         case 2:
            _bSP0MuxSate = 0 ;
                            
			vIO32WriteFldMulti(PDWNC_VGACFG1, P_Fld(1,FLD_RG_HSYNC_EN)|P_Fld(1,FLD_RG_VSYNC_EN)|P_Fld(0,FLD_RG_SOG_EN));
			vIO32WriteFldMulti(PDWNC_VGACFG4,P_Fld(4,FLD_RG_SYNC1_VTH)|P_Fld(2,FLD_RG_SYNC1_VTL));//HSYNC
                vIO32WriteFldAlign(PDWNC_VGA_WAKE_UP_CONTROL_1_REGISTER, 0x02, FLD_WAKEUP_INT_EN);
				vIO32WriteFldAlign(PDWNC_VGA_WAKE_UP_CONTROL_2_REGISTER, 0x0, FLD_HSYNC_SEL);
				vIO32WriteFldAlign(PDWNC_VGA_WAKE_UP_CONTROL_2_REGISTER, 0x0, FLD_VSYNC_SEL);
				vIO32WriteFldAlign(PDWNC_VGA_WAKE_UP_CONTROL_2_REGISTER, 0x1, FLD_CSYNC_SOG_SEL);
				#if UART_DBG
				LogS("Cs");
				#endif
				vKrDelay1ms(5);
				if(RegReadFldAlign(PDWNC_VGA_WAKE_UP_STATUS_0_REGISTER, FLD_HSYNC_ACT) && RegReadFldAlign(PDWNC_VGA_WAKE_UP_STATUS_0_REGISTER, FLD_VSYNC_ACT))
                break;
           
    }
	vIO32WriteFldAlign(PDWNC_VGA_WAKE_UP_CONTROL_1_REGISTER, 0x0, FLD_HLEN_VLEN_RESET );
	#if UART_DBG
	LogSD("_bSP0MuxSate\n",_bSP0MuxSate);
	#endif    
}
#endif

#if defined VGA_WAKEUP_MANUAL_WAY || defined VGA_WAKEUP_HW_WAY
void vVGAWakeupTask(void)
{
	if( (MGET_ARM_POWER_STATE() == ARM_POWER_OFF) && (RegReadFldAlign(PDWNC_WAKEN, FLD_VGA_WAKEN)==1) )
	{
#ifdef VGA_WAKEUP_MANUAL_WAY
						
		{
			if(_bSP0MuxCnt++ > 1000)
			{	
				if(_bSP0MuxSate==1)
				{
					vPDSwitchVGAMux();
				}
				else
				{
					_bSP0MuxSate=0;
					vPDSwitchVGAMux();
				}
			}
		}
#endif
#ifdef VGA_WAKEUP_HW_WAY
		{
			if(RegReadFldAlign(PDWNC_VGA_WAKE_UP_STATUS_3_REGISTER,FLD_VGA_WAK_MODE_STATUS)<=1)
			{
			#if UART_DBG
			LogS("ss or cs \n");
			#endif
			vIO32WriteFldMulti(PDWNC_VGACFG1, P_Fld(1,FLD_RG_HSYNC_EN)|P_Fld(1,FLD_RG_VSYNC_EN)|P_Fld(0,FLD_RG_SOG_EN));
			vIO32WriteFldMulti(PDWNC_VGACFG4,P_Fld(4,FLD_RG_SYNC1_VTH)|P_Fld(2,FLD_RG_SYNC1_VTL));//HSYNC
			}		
			else if(RegReadFldAlign(PDWNC_VGA_WAKE_UP_STATUS_3_REGISTER,FLD_VGA_WAK_MODE_STATUS)==2)		
			{
			#if UART_DBG
			LogS("sog \n");
			#endif
			vIO32WriteFldMulti(PDWNC_VGACFG1, P_Fld(0,FLD_RG_HSYNC_EN)|P_Fld(1,FLD_RG_VSYNC_EN)|P_Fld(1,FLD_RG_SOG_EN));
		  	vIO32WriteFldMulti(PDWNC_VGACFG4,P_Fld(8,FLD_RG_SYNC1_VTH)|P_Fld(8,FLD_RG_SYNC1_VTL));//SOY
		  	}
		}
#endif 
	}
	else
	{
#if 0//UART_DBG
			if(!(MGET_ARM_POWER_STATE() == ARM_POWER_OFF))
				LogS("!ARM_POWER_OFF\n");
			if(!(RegReadFldAlign(PDWNC_WAKEN, FLD_VGA_WAKEN)==1))
				LogS("!FLD_VGA_WAKEN\n");
#endif
	}
  
}
#endif	

//-----------------------------------------------------------------------------
/** task() is mainloop of system
 */
//-----------------------------------------------------------------------------
void vTask(void)
{
	while(1)
    {
#if defined VGA_WAKEUP_MANUAL_WAY || defined VGA_WAKEUP_HW_WAY
    vVGAWakeupTask();
#endif

        vCommandTask();
#if defined SIR_TASK && defined QUERY_IR_POWER_KEY_STATE_SUPPORT
	#if (QUERY_IR_POWER_KEY_STATE_SUPPORT == 1)		
		if(gSIRC_ctrl._gu1ARM_IR_WORKING == 0)
    #else
	    if(MGET_ARM_POWER_STATE() == ARM_POWER_OFF)
    #endif			

#else
	    if(MGET_ARM_POWER_STATE() == ARM_POWER_OFF)
#endif
        {
            #if IR_TASK        
            vIrTask();
            #endif
        }
        #if ETHERNET_TASK
        vEthernetWakeup();
        #endif
        
		#if UART_TASK
		vRS232Task();
		#endif
        #if ENABLE_CEC
        CECTask();
        #endif 
#if ENABLE_CBUS
        CBUSTask();
#endif	  
        #if (RW_REG_INTERFACE&&SIF_PDWNC_TEST)
        PDSIFTask();            
        #endif
              
		#if SUSPEND_MODE
        vSuspendTask();
		#endif
        vCustTask();  
    }
}

void vSysInit_DisableInt(void)
{
    MDisableMainINT(); //diable EX0
    MDisableINT(); //disable EA0
    vIO32Write4B(PDWNC_T8032_INTEN, 0);//disable PDWNC all interrrupt
    MDisableT8032INT();
}

void vSysInit_EnableInt(void)
{
    IP = 0xc1;//set EX0 as highest priority
    MEnableINT(); //Enable EA0
//Only enable ARM interrrupt here...the other interrupt source is enabled in their initialization function

    vRegWriteFldAlign(PDWNC_T8032_INTEN, 0x1, FLD_ARM_INTEN);

    MEnableT8032INT();
#ifdef CC_MT8223
	MEnableMainINT();
#endif
}

//-----------------------------------------------------------------------------
/** main() is C code entry.
  */
//-----------------------------------------------------------------------------
void main(void)
{
    vRs232Init(0);
    vSysInit_DisableInt();
#if 0// UART_DBG
	LogS("Hang");
#endif	
//		while(1);
#if UART_DBG
    LogS("  Main 090929");
#endif
//check if watcg dog reboot...
   if(WTRF)
    {
        vWDTRebootProcess();
        WTRF=0;
    }
    vDrvInit();
#if UART_DBG
    LogS("vDrvInit");
#endif   
    vTaskInit();
#if UART_DBG
    LogS("vTaskInit");
#endif   
    vSysInit_EnableInt();

    vTask();

    UNCALL_FUNS();
}

//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------
static void UNCALL_FUNS(void)
{
    vTaskInit();
    vTask();

    vKrInit();
    vKrDelay1ms(0);
    vKrDelay10ms(0);
    u4RegRead4B(0);
    u2RegRead2B(0);
    vRegWrite2B(0, 0);
    vRegWrite4B(0, 0);
    vRegWrite4B_S(0, 0, 0, 0, 0);
    vRegWrite4BMsk(0, 0, 0);
    vRegWrite2BMsk(0, 0, 0);
//    vTestAsm();
    //vTestFunc();
}

//-----------------------------------------------------------------------------
// Interface(exported) functions
//-----------------------------------------------------------------------------
