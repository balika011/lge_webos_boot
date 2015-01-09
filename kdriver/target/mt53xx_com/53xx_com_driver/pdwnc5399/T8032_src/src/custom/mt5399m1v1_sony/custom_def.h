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

/*************************************************************************
 *
 * Filename:
 * ---------
 *   $Workfile: timer.h $
 *
 * Project:
 * --------
 *   MT8205
 *
 * Description:
 * ------------
 * Timer function header
 *
 * Author:
 * -------
 *
 *
 * Last changed:
 * -------------
 * $Author: dtvbm11 $
 *
 * $Modtime: 04/05/28 4:40p $
 *
 * $Revision: #1 $
*************************************************************************/
/* 20091227
David: due to lots of customer development projects still on 0801, pls note the following 6 define
1. #define CUS_SH_HOTEL_MODE 0//For P_COMP platform, this define must be 0!!!!!!!!!!!!!!
2. #define RC56_IR_DRIVER 1 //For P_COMP platform, this define must be 1!!!!!!!!!!!!!!
4. //#define MT5387_CEC  1 //4 & 5 exculsive
5. #define MT5365_CEC  1
*/
#ifndef _CUSTOM_DEF_H_
#define _CUSTOM_DEF_H_

//#define CC_MT5881
#define CC_MT5399

//#define CC_MT8223
#define FW_VERSION 76
#define RC56_IR_DRIVER 0
#define CUST_P_DC_ON_SEQUENCE 0 // 1
#define CUST_P_SX_SUPPORT 0
#define UART_PD_RX_EN 0// 1
#define UP_ARM_TEST 0
#define IR_DEBOUNCE 0// 1
#define PDWNC_INT_POLLING 0
#define IR_MULTIKEY_WAKEUP 0

#define BUILD_IN_DEBUG_MSG 0
#define ETHERNET_TASK 1

#define S_HOTEL_MODE_COMMAND 1// 1// 1
#define CY8C20110_SUPPORT 0 // 1
#define STANDBY_AC_DETECTION 0
//#define HOTEL_MODE_DBG 0// 1
//#define SX_RS232_DBG 1
#define DEFAULT_DEBUG_LEVEL 0
#define HOTKEY_WAKEUP_SUPPORT 0
/***************************
*RW_REG_INTERFACE : by teraterm 
*UART_DBG : by Mtktool
***************************/
#define RW_REG_INTERFACE 0// 1 //for sif read and write test under standby(open it can r/w xdata under standby)
#define UART_DBG 0
#define SONY_IR_DBG 0

#define SUPPORT_ERROR_LED_CTRL  0//0
//david: always enable #define FW_UPGRATE_PWR_CTRL 0// 1
#define SUPORT_COLD_REBOOT 0// 1
#define SUPORT_DIGIT_KEY_WAKEUP 1 // 1

#define T8032_OCCUPIED_UART0_AT_NORMAL_POWER 0
#define CUS_S_MULTI_KEY_ECO 0 // 1

//david: always enable #define FW_UPGRATE_PWR_CTRL 0// 1
#define SIMPLE_LED_TOGGLE 0// 1
#define SIR_TASK 1 // 1
#define QUERY_IR_POWER_KEY_STATE_SUPPORT 1
#define DISABLE_WAKEUP_SOMETIME 0
#if DISABLE_WAKEUP_SOMETIME
	#define DISABLE_WAKEUP_CNT 35 // 200*20ms=4 seconds
#endif

#define CUS_SH_HOTEL_MODE 0//For P_COMP platform, this define must be 0!!!!!!!!!!!!!!
#define CUST_B_NEC_IR_DRIVER 0	//Add by Cust_B 20091215

#define SIF_DRV_SUPPORT 0// 1

#if SIF_DRV_SUPPORT
    #define SIF_BUFFER_SIZE 0x50     
    #undef EEPROM_DRV_SUPPORT 
    #define EEPROM_DRV_SUPPORT 1
#else
#define EEPROM_DRV_SUPPORT 0
#endif

#if defined VGA_WAKEUP_MANUAL_WAY || defined VGA_WAKEUP_HW_WAY
#else

#ifdef CC_MT5881
#define VGA_WAKEUP_MANUAL_WAY
#else
#define VGA_WAKEUP_HW_WAY
#endif

#endif




#if EEPROM_DRV_SUPPORT 
    #undef SIF_PDWNC_TEST
    #define SIF_PDWNC_TEST    1//for sif read and write test
    #define EEPROM_DEV_ADDR 0xA0
    #define EEPROM_SUB_ADDR_SIZE 2//24c02 only need one sub address (so call byte address), 24C128 needs 2 bytes for sub address...
    #define EEPROM_WP_GPIO_MASK (0x1 << 3) //opctrl3 for write protection pin
    #define EEPROM_WP_GPIO_POLARITY 1//HI : WP , LO: WRITE 
    #define EEPROM_CLK_DIV   0x100 //eeprom ==> 100Khz, 27M/0x100 =  100khz      
#else
#define SIF_PDWNC_TEST    0
#endif  

//#define SIF_ISR 0
#define UART_TASK 1	//Should always set it to 1, it is use for T8032 debug
#if UART_TASK
    #define MAX_UART_BUF_LEN 32
#endif    

#define IR_TASK 1// 1  //For P_COMP platform, this define must be 1!!!!!!!!!!!!!!
#define GPIO_INTERRUPT_ENABLE  1
#define T8032_GPIO_TEST 0	//just for test GPIO function

#if SIF_DRV_SUPPORT 
#define SIF_SCL_STRECH 0
#define SIF_ODRAIN 0
#endif

#define SIRC_IR 1
#define NEC_IR 2
#define RC56_IR 3
#define SHARP_IR 4

#define IR_PROTOCOL RC56_IR

#if (IR_PROTOCOL == SIRC_IR)
typedef UINT8 IR_DATA_TYPE;
#else
typedef UINT32 IR_DATA_TYPE;
#endif

/******* CEC *******************/
//#define MT5387_CEC  1
#define MT5365_CEC  1

#define CEC_FEATURE_1  	1
#define CEC_FEATURE_2  	0
#define CEC_FEATURE_3 	0
#define CEC_FEATURE_4   0

/******* CEC Feature 1 *************/
#if CEC_FEATURE_1
#define ENABLE_CEC 1
#define DISABLE_CEC_WITHOUT_ACK 1  
#define STORE_ALL_MSG_AFTER_WAKEUP 0
#endif
/******* CEC Feature 2 *************/
#if CEC_FEATURE_2
#define ENABLE_CEC 1
#define DISABLE_CEC_WITHOUT_ACK 1  
#define STORE_ALL_MSG_AFTER_WAKEUP 0
#endif
/******* CEC Feature 3 *************/
#if CEC_FEATURE_3
#define ENABLE_CEC 1
#define DISABLE_CEC_WITHOUT_ACK 1  
#define STORE_ALL_MSG_AFTER_WAKEUP 0
//#define CEC_FEATURE_3_1 
#endif
/******* CEC_FEATURE_4 *************/
#if CEC_FEATURE_4
#define ENABLE_CEC 0
#define DISABLE_CEC_WITHOUT_ACK 0  
#define STORE_ALL_MSG_AFTER_WAKEUP 0
#endif

/******Cust_B Debug define**************/
#define Cust_B_Debug_UART 0// 1

#define ENABLE_CBUS 1
#endif 
