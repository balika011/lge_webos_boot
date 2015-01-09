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
/*-----------------------------------------------------------------------------
 *
 * $Author: dtvbm11 $
 * $Date: 2015/01/09 $
 * $RCSfile: x_pdwnc.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file x_pdwnc.h
 *  x_pdwnc.h The PDWNC (Power Down Interface Module) interface header file
 */


#ifndef X_PDWNC_H
#define X_PDWNC_H

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "x_hal_5381.h"
#include "x_gpio.h"
#include "x_lint.h"
#include "hw_vdoin.h"
#include "hw_pdwnc.h"
LINT_EXT_HEADER_BEGIN
#ifdef CC_MTK_LOADER
#ifndef CC_UBOOT
#ifndef __GCC__   /*gcc compile */
typedef VOID (*x_os_isr_fct) (UINT16  ui2_vector_id);
#endif
#endif /* CC_UBOOT */
#else
#ifndef CC_SECURE_WORLD
#include "x_os.h"
#endif
#endif
LINT_EXT_HEADER_END

#ifndef CC_SECURE_WORLD
#include "x_os.h"
#include "sif_if.h"
#endif

//===========================================================================
#if defined(CC_MT5881)
    #define PDWNC_INTNO_SIFM 		    	8	// SIFM0 interrupt number
    #define PDWNC_INTNO_UART_DBG 		    9	// UART debug interrupt number
    #define PDWNC_INTNO_AVLINK0			    10	// AVLINK0 interrupt number
    #define PDWNC_INTNO_CEC      		    11  // CECINT0 interrupt number
    #define PDWNC_INTNO_RTC 			    12	// RTC interrupt number
    #define PDWNC_INTNO_IRRX			    13	// IRRX interrupt number
    #define PDWNC_INTNO_ARM_INT_0    		14  // ARM interrupt number
    #define PDWNC_INTNO_ARM_INT_1    		15  // ARM interrupt number
    #define PDWNC_INTNO_ARM_INT_2    		16  // ARM interrupt number
    #define PDWNC_INTNO_ARM_INT_3    		17  // ARM interrupt number
    #define PDWNC_INTNO_AUX_INT_0     	    18  // AUX interrupt number
    #define PDWNC_INTNO_AUX_INT_1         	19  // AUX interrupt number
    #define PDWNC_INTNO_AUX_INT_2        	20  // AUX interrupt number
    #define PDWNC_INTNO_AUX_INT_3         	21  // AUX interrupt number
    #define PDWNC_INTNO_DDCCI        		22  // ADDCCI interrupt number
    #define PDWNC_INTNO_AVLINK_INT      	23  // AVLINK interrupt number
    #define PDWNC_INTNO_CEC1_INT        	24  // CEC1 interrupt number
    #define PDWNC_INTNO_DEV_DET_INT     	25  // HDMI edid device address detection interrupt number
    #define PDWNC_INTNO_CBUS_INT        	26  // CBUS interrupt number
    #define PDWNC_INTNO_SIFM1_INT        	27  // SIFM1 interrupt number
    #define MAX_PDWNC_INT_ID				28	 // Maximum value of PDWNC interrupt id
#elif defined(CC_MT5398) || defined(CC_MT5880)
	#define PDWNC_INTNO_OPCTRL0 			0	// OPCTRL 0 interrupt number
	#define PDWNC_INTNO_OPCTRL1 			1	// OPCTRL 1 interrupt number
	#define PDWNC_INTNO_OPCTRL2 			2	// OPCRTL 2 interrupt number
	#define PDWNC_INTNO_OPCTRL3 			3	// OPCTRL 3 interrupt number
	#define PDWNC_INTNO_OPCTRL4 			4	// OPCTRL 4 interrupt number
	#if defined(CC_MT5398)
	#define PDWNC_INTNO_OPCTRL5 			5	// OPCTRL 5 interrupt number
	#define PDWNC_INTNO_OPCTRL6 			6	// OPCTRL 6 interrupt number
	#define PDWNC_INTNO_OPCTRL7 			7	// OPCTRL 7 interrupt number
	#else
	#define PDWNC_PINTNO_VGA_SDA    		5            // VGA SDA interrupt number
	#define PDWNC_PINTNO_VGA_SCL    		6            // VGA SCL interrupt number
	#define PDWNC_PINTNO_OIRI       		7            // OIRI interrupt number
	#endif
    #define PDWNC_INTNO_SIFM				8	// SIFM0 interrupt number
    #define PDWNC_INTNO_UART_DBG			9	// UART debug interrupt number
    #define PDWNC_INTNO_AVLINK0 			10	// AVLINK0 interrupt number
    #define PDWNC_INTNO_CEC 				11	// CECINT0 interrupt number
    #define PDWNC_INTNO_RTC 				12	// RTC interrupt number
    #define PDWNC_INTNO_IRRX				13	// IRRX interrupt number
    #define PDWNC_INTNO_ARM_INT 			14	// ARM interrupt number
    #define PDWNC_INTNO_AUX_INT 			15	// AUX interrupt number
	#if defined(CC_MT5398)
	#define PDWNC_INTNO_OPCTRL8 			16	// OPCTRL 8 interrupt number
	#define PDWNC_INTNO_OPCTRL9 			17	// OPCTRL 9 interrupt number
	#define PDWNC_INTNO_OPCTRL10 			18	// OPCTRL 10 interrupt number
	#define PDWNC_INTNO_OPCTRL11 			19	// OPCTRL 11 interrupt number
	#endif
    #define PDWNC_INTNO_SIFM1				20	 // SIFM1 interrupt number
    #define PDWNC_INTNO_DDCCI				21	 // DDCCI interrupt number
    #define PDWNC_INTNO_AVLINKINT1			22	 // AVLINKINT1 interrupt number
    #define PDWNC_INTNO_CECINT1 			23	 // CECINT1 interrupt number	
    #define PDWNC_INTNO_GSPI 				24
    #define PDWNC_INTNO_DEV_DET 			25	// HDMI edid device address detection interrupt number
    #if defined(CC_MT5398) 	
	#define PDWNC_INTNO_OIRI           		26 // OIRI interrupt number
	#define PDWNC_INTNO_LED_PWM1	 		27 	 // LED_PWM1 interrupt number
	#define PDWNC_INTNO_SRVAD4				28 	 // SRVAD 4 interrupt number
	#define PDWNC_INTNO_SRVAD5	 			29 	 // SRVAD 5 interrupt number
	#define PDWNC_INTNO_SRVAD6	 			30 	 // SRVAD 6 interrupt number
	#define PDWNC_INTNO_SRVAD7	 			31 	 // SRVAD 7 interrupt number
	#endif
    #define MAX_PDWNC_INT_ID				32	 // Maximum value of PDWNC interrupt id
    
#elif defined(CC_MT5399) || defined(CC_MT5890) || defined(CC_MT5882)  /*Add CC_MT5882 George.yang 2014-7-17*/
	#if defined(CC_MT5399)
    #define PDWNC_INTNO_OPCTRL0 		0	// OPCTRL 0 interrupt number
    #define PDWNC_INTNO_OPCTRL1 		1	// OPCTRL 1 interrupt number
    #define PDWNC_INTNO_OPCTRL2 		2	// OPCRTL 2 interrupt number
    #define PDWNC_INTNO_OPCTRL3 		3	// OPCTRL 3 interrupt number
    #define PDWNC_INTNO_OPCTRL4 		4	// OPCTRL 4 interrupt number
    #define PDWNC_INTNO_OPCTRL5 		5	// OPCTRL 5 interrupt number
    #define PDWNC_INTNO_OPCTRL6 		6	// OPCTRL 6 interrupt number
    #define PDWNC_INTNO_OPCTRL7 		7	// OPCTRL 7 interrupt number
    #else
	#define PDWNC_INTNO_RS232_HT 		5	// RS232 HT interrupt number
    #define PDWNC_INTNO_RS232_BT  		6	// RS232 BT interrupt number
    #define PDWNC_INTNO_PWERT_RDY		7	// POWER READY interrupt number
    #endif
    #define PDWNC_INTNO_SIFM			8	// SIFM0 interrupt number
    #define PDWNC_INTNO_UART_DBG		9	// UART debug interrupt number
    #define PDWNC_INTNO_AVLINK0 		10	// AVLINK0 interrupt number
    #define PDWNC_INTNO_CEC 			11	// CECINT0 interrupt number
    #define PDWNC_INTNO_RTC 			12	// RTC interrupt number
    #define PDWNC_INTNO_IRRX			13	// IRRX interrupt number
    #define PDWNC_INTNO_ARM_INT 		14	// ARM interrupt number
    #define PDWNC_INTNO_AUX_INT 		15	// AUX interrupt number
    #define PDWNC_INTNO_SIFM1			20	 // SIFM1 interrupt number
    #define PDWNC_INTNO_DDCCI			21	 // DDCCI interrupt number
    #define PDWNC_INTNO_AVLINKINT1		22	 // AVLINKINT1 interrupt number
    #define PDWNC_INTNO_CECINT1 		23	 // CECINT1 interrupt number	
    #define PDWNC_INTNO_GSPI 			24
    #define PDWNC_INTNO_DEV_DET 		25	// HDMI edid device address detection interrupt number
    #if defined(CC_MT5399)
    #define PDWNC_INTNO_OIRI			26	 // OIRI interrupt number
    #define PDWNC_INTNO_LED_PWM1		27	 // LED_PWM1 interrupt number
    #endif
    #define PDWNC_INTNO_CBUS			30	// CBUS interrupt number
    #define PDWNC_INTNO_SIFM2			31	// SIFM1 interrupt number
    #define MAX_PDWNC_INT_ID			32	 // Maximum value of PDWNC interrupt id
    #if defined(CC_MT5399)||defined(CC_MT5882) // CC_MT5882_TODO
    #define PDWNC_INTNO_GPIO50			32  //gpio 252
    #define PDWNC_INTNO_GPIO51			33	//gpio 253
    #define PDWNC_INTNO_GPIO52			34	//gpio 254
    #define PDWNC_INTNO_GPIO53			35	//gpio 255
    #define PDWNC_INTNO_GPIO54			36	//gpio 256
    #define PDWNC_INTNO_GPIO55			37	//gpio 257
    #define MAX_PDWNC_INT_ID_2			38	 // Maximum value of PDWNC_2 interrupt id
    #endif
#endif

#define PDWNC_OK                0
#define PDWNC_FAIL              1

#if defined(CC_MT5890) 
	#define TOTAL_PDWNC_GPIO_NUM    75
	#define TOTAL_PDWNC_GPIO_INT_NUM    53
#elif defined(CC_MT5882)
	#define TOTAL_PDWNC_GPIO_NUM    42
	#define TOTAL_PDWNC_GPIO_INT_NUM    22
#elif defined(CC_MT5399)
	#define TOTAL_PDWNC_GPIO_NUM    60//47//49
	#define TOTAL_PDWNC_GPIO_INT_NUM    20//14
#elif defined(CC_MT5881) 
	#define TOTAL_PDWNC_GPIO_NUM    42
	#define TOTAL_PDWNC_GPIO_INT_NUM    32
#elif defined(CC_MT5880)	
	#define TOTAL_PDWNC_GPIO_NUM    25
#elif defined(CC_MT5398)	
	#define TOTAL_PDWNC_GPIO_NUM    47
#else
    #define TOTAL_PDWNC_GPIO_NUM    20
#endif
#if defined(CC_MT5890)
#define REG_RW_PDWNC_INTEN_GPIO_MASK 0x3C0F00FF 
#elif defined(CC_MT5399)
#define REG_RW_PDWNC_INTEN_GPIO_MASK 0x0C0F00FF 
#elif defined(CC_MT5882)
#define REG_RW_PDWNC_INTEN_GPIO_MASK 0x000F00FF 
#elif defined(CC_MT5881)
#define REG_RW_PDWNC_INTEN_GPIO_MASK 0xFB0000FF    
#elif defined(CC_MT5880)
#define REG_RW_PDWNC_INTEN_GPIO_MASK 0x000000FF 
#elif defined(CC_MT5398)
#define REG_RW_PDWNC_INTEN_GPIO_MASK 0xFC0F00FF
#else
#define REG_RW_PDWNC_INTEN_GPIO_MASK 0xFF0000FF
#endif    


#define TOTAL_PDWNC_DEDICATE_GPI_NUM    7//check 28120[7:0], only the first 8 pwdnc gpio(EXTIO) can be set as wakeup pins
#define TOTAL_SERVOADC_NUM      TOTAL_SERVOADC_NUMBER// servoadc ch6,7 is dedicated used power detection

//#define PDANC_PAD_SERVOADC_EN(x)  (1U << (x))    // SERVO ADC PAD output enable, not use from 5365...    
//#define PDWNC_PAD_SRVIO_OUT(x)    (0x1U << (x)) 

#define FLD_HDMI_SDA1_MASK	((0x3fU) << 16)
#define FLD_HDMI_SCL1_MASK	((0x3fU) << 24)    
#define FLD_HDMI_SDA2_MASK	((0x3fU) << 16)
#define FLD_HDMI_SCL2_MASK	((0x3fU) << 24)    

#define FLD_HDMI_SDA_MASK  ((0x3fU) << 16)
#define FLD_HDMI_SCL_MASK  ((0x3fU) << 24) 

#define FLD_WDT0_RST 	(1U << 24)
#define FLD_WDT1_RST 	(1U << 25)
#define FLD_WDT2_RST 	(1U << 26)    
#define FLD_WDTALL_RST 	(FLD_WDT0_RST | FLD_WDT1_RST | FLD_WDT2_RST)  

//#define REG_RW_PDWAKE_EN    0x0120      // Power Down Wakeup Enable Register
//#define WAK_GPIO0       (1U << 0)
//#define WAK_GPIO1       (1U << 1)
//#define WAK_GPIO2       (1U << 2)
//#define WAK_GPIO3       (1U << 3)
//#define WAK_GPIO4       (1U << 4)
//#define WAK_GPIO5       (1U << 5)
//#define WAK_GPIO6       (1U << 6)
//#define WAK_GPIO7       (1U << 7)
#define WAK_GPIOX(x)    (1U << (x))             // Set GPIO wakeup bit.
#define GPIO_WAKEN(x)   (((x) & 0xFF) << 0)     // GPIO wakeup enable bits
#if defined(CC_MT5890)
#define GPIO_WAKEN_1(x)   (((x) & 0x7FFFFFFF) << 0) // GPIO wakeup enable bits
#endif
#define WAK_ALLGPIO     GPIO_WAKEN(0xFF)
#define WAK_IRRX        (1U << 8)
#define WAK_VGA         (1U << 9)
#if defined(CC_MT5890)
#define WAK_HT_RS232    (1U << 10)
#define WAK_BT_RS232    (1U << 11)
#endif
#if defined(CC_MT5399)
#define WAK_GPIO8       (1U << 10)
#define WAK_GPIO9       (1U << 11)
#define WAK_GPIO10      (1U << 12)
#define WAK_GPIO11      (1U << 13)
#endif
#define WAK_RTC         (1U << 15)   
#define WAK_T8032       (1U << 17)
#if defined(CC_MT5881)	
#define WAK_UNOR		(1U << 18)	  
//#define WAK_SRVAD0		(1U << 19)	
//#define WAK_SRVAD1		(1U << 20)		
//#define WAK_SRVAD2		(1U << 21)	
//#define WAK_SRVAD3		(1U << 22)			
//#define WAK_SRVAD4		(1U << 23)			
//#define WAK_SRVAD5		(1U << 24)
//#define WAK_SRVAD6		(1U << 25)			
//#define WAK_SRVAD7		(1U << 26)
#else  
#define WAK_HDMI        (1U << 18) 
#define WAK_UNOR        (1U << 19)    
//#define WAK_SRVAD0      (1U << 20)    
//#define WAK_SRVAD1      (1U << 21)        
//#define WAK_SRVAD2      (1U << 22)    
//#define WAK_SRVAD3      (1U << 23)            
//#define WAK_SRVAD4      (1U << 24)            
//#define WAK_SRVAD5      (1U << 25)
//#define WAK_SRVAD6      (1U << 26)            
//#define WAK_SRVAD7      (1U << 27)
#endif
#define WAK_SRVADX(x)   (1U << (x))             // Set ServoADC wakeup bit.
#if defined(CC_MT5890)
#define SRVAD_WAKEN(x)  (((x) & 0x3FF) << 20)    // Servo ADC wakeup enable bits, Oryx have channel 8,9.
#elif defined(CC_MT5881)
#define SRVAD_WAKEN(x)  (((x) & 0xFF) << 19)    // Servo ADC wakeup enable bits, Oryx have channel 8,9.
#else
#define SRVAD_WAKEN(x)  (((x) & 0xFF) << 20)    // Servo ADC wakeup enable bits
#endif
#if defined(CC_MT5890)
#define WAK_ALLSRVAD    SRVAD_WAKEN(0x3FF) 
#else
#define WAK_ALLSRVAD    SRVAD_WAKEN(0xFF) 
#endif

#define PDWNC_WAK_SRVADX(x)   (1U << (x))             // Set ServoADC wakeup bit.
#if defined(CC_MT5881)    
#define PDWNC_SRVAD_WAKEN(x)  (((x) & 0xFF) << 19)    // Servo ADC wakeup enable bits    
#else
#define PDWNC_SRVAD_WAKEN(x)  (((x) & 0xFF) << 20)	  // Servo ADC wakeup enable bits	 
#endif
#define PDWNC_WAK_ALLSRVAD    PDWNC_SRVAD_WAKEN(0xff)
//----------------------------------------------------------------------------------------------------------------------
//#define PDWNC_SRVCH_EN_SRVCFG     (0xFFU)    // Servo ADC channel enable bits.
//#define PDWNC_SRVIO_GP_O(x)     (1U << (x))  // Servo ADC PAD used to be GPIO                      
// Servo ADC Interface Configuration Register
#if defined(CC_MT5881)||defined(CC_MT5399) ||defined(CC_MT5882) 
#define SRVCH_EN_SRVCFG     (0x1ffU)    // Servo ADC channel enable bits,Remy and mustang add channel8.
#elif defined(CC_MT5890)
#define SRVCH_EN_SRVCFG     (0x7ffU) 
#else
#define SRVCH_EN_SRVCFG 	(0xffU)    // (CC_MT5398 , CC_MT5880)Servo ADC channel enable bits.
#endif
#define PDWNC_SRVCH_EN_CH(x)      ((1 << (x)) & SRVCH_EN_SRVCFG)

//#define SRVCH_EN_CH(x)		((1 << (x)) & SRVCH_EN_SRVCFG)
//#define REG_RW_SRVSWT              0x030c      // Servo ADC Interface Software Trigger Register
//#define REG_RW_SRVCLR              0x0310      // Servo ADC Interface Clear Register
//#define ADCOUT_CLR          (1U << 1)   // Clear ADC output data and channel status
#define CHBTM_MASK          (0xffU)// bottom voltage value
#define GET_CHBTM(x)        ((x) & 0xffU)
#define CHTOP_MASK          (0xffU)    // top voltage value
#define GET_CHTOP(x)        ((x) & 0xffU)

#define CHBTM_VAL(x)        ((x) & CHBTM_MASK)
#define CHTOP_VAL(x)        ((x) & CHTOP_MASK)
#define CH_MASK             (0x7U)   // interrupt or wakeup mask
#define CH_WAK_TOP_EN       (1U << 2)
#define CH_WAK_MID_EN       (1U << 1)
#define CH_WAK_BTM_EN       (1U << 0)

#if defined(CC_MT5398)
#define ADC2GPIO_CH_ID_MAX  7
#define ADC2GPIO_CH_ID_MIN  2
#elif defined(CC_MT5399) || defined(CC_MT53882)
#define ADC2GPIO_CH_ID_MAX  7
#define ADC2GPIO_CH_ID_MIN  1
#elif defined(CC_MT5890)
#define ADC2GPIO_CH_ID_MAX  9
#define ADC2GPIO_CH_ID_MIN  1
#else
#define ADC2GPIO_CH_ID_MAX	5    
#define ADC2GPIO_CH_ID_MIN	2
#endif

//#define REG_RW_PWDIOCFG     0x009c      // Power Down IO Configuration Register
//#define GPIO0_POL       (1U << 0)   // GPIO 0 wake-up polarity, 0 active low
//#define GPIO1_POL       (1U << 1)   // GPIO 1 wake-up polarity, 0 active low
//#define GPIO2_POL       (1U << 2)   // GPIO 2 wake-up polarity, 0 active low
//#define GPIO3_POL       (1U << 3)   // GPIO 3 wake-up polarity, 0 active low
//#define GPIO4_POL       (1U << 4)   // GPIO 4 wake-up polarity, 0 active low
//#define GPIO5_POL       (1U << 5)   // GPIO 5 wake-up polarity, 0 active low
//#define GPIO6_POL       (1U << 6)   // GPIO 6 wake-up polarity, 0 active low
//#define GPIO7_POL       (1U << 7)   // GPIO 7 wake-up polarity, 0 active low
#define GPIOX_POL(x)    (1U << (x)) // GPIO x wake-up polarity
#define GPIOALL_POL     (0xFFU)     // GPIO all wake-up polarity, all active low
#define GPIO_DEGCNT(x)  (((x) & 0x0FF) << 8)    // debounce period default 60ms   

#define PWRDWN_CODE1    0x14
#define PWRDWN_CODE2    0x04

// PDWNC Interrupt status/enable/clear registers
#define _PINT(v)        (1U << (v))

//REG_RW_PDWNC_INTEN: 0xF0028044 , PDWNC Interrupt Enable Register
#define OPCTRL_INTEN(x) (0x1U << (x))
#define PDWNC_INTEN(x)	vIO32Write4B(PDWNC_ARM_INTEN, u4IO32Read4B(PDWNC_ARM_INTEN) | _PINT(x)) 
#define PDWNC_INTDIS(x) vIO32Write4B(PDWNC_ARM_INTEN, u4IO32Read4B(PDWNC_ARM_INTEN) & (~_PINT(x))) 

#define IS_PINTNO_INTABLE_GPIO(x)	  ((REG_RW_PDWNC_INTEN_GPIO_MASK & (0x1UL << (x))) ? 1 : 0) //
// IRRX readonly registers
#define IREXP_EN(x)         (1U << x)   // Enable IR Expect value register x
#define IREXP_BIT_EN        (1U << 16) // Bit count expectation enable bit
#define MAX_EXP_KEY_NUM     (16)

//----------------------------------------------------------------------------
//RTC
    #define RTCALM_FULL_EN  (0xffU << 0)
    #define RTCALM_YEAR_EN  (1U << 7)   // Alarm compare YEAR enable
    #define RTCALM_MONTH_EN (1U << 6)   // Alarm compare MONTH enable
    #define RTCALM_DAY_EN   (1U << 5)   // Alarm compare DAY enable
    #define RTCALM_WEEK_EN  (1U << 4)   // Alarm compare WEEK enable
    #define RTCALM_HOUR_EN  (1U << 3)   // Alarm compare HOUR enable
    #define RTCALM_MINU_EN  (1U << 2)   // Alarm compare MINU enable
    #define RTCALM_SEC_EN   (1U << 1)   // Alarm compare SEC enable
    #define RTCALM_INTR_EN  (1U << 0)   // Alarm interrupt enable
#define REG_RW_RTC_CTRL     0x073c      // RTC Control Register
    #define RTCTRL_STOP     (1U << 0)   // Stop RTC, to save power
    #define RTCTRL_H24      (1U << 1)   // 1 for 24-hour clock mode
    #define RTCTRL_SIM      (1U << 4)   // Pass write protection
    #define RTCTRL_PASS     (1U << 6)   // Pass write protection
    #define RTCTRL_FLAG     (1U << 7)   // Clean wrong access (by writing '0')
//#define REG_RW_RTC_KEY      0x0740      // RTC Key Register - NOT USED
//#define REG_RW_RTC_LEARYEAR 0x0744      // RTC Leap Year    - NOT USED
//#define REG_RW_RTC_PROT     0x0748      // RTC Write Protection Register
    #define RTC_WRITE_PROTECT_CODE1          0x87
    #define RTC_WRITE_PROTECT_CODE2          0x78             


// Error code
#define ERR_NO_ERROR                0
#define ERR_DC_PROTECT              1
#define ERR_POWER_DOWN              2
#define ERR_I2C_STANDBY_CTRL        3
#define ERR_I2C_GENERAL             4
#define ERR_I2C_MT8280              5
#define ERR_I2C_SYS_EEPROM          6
#define ERR_I2C_TUNER               7
#define ERR_I2C_DEMOD               8   // TDA9886
#define ERR_I2C_FPGA                9
#define ERR_I2C_HDMI_SWITCH         13
#define ERR_DRAM_MT8280_1           14
#define ERR_DRAM_MT8280_2           15
#define ERR_I2C_HDMI_EDID           16
#define ERR_I2C_DFI_BOLT_ON         17
#define ERR_I2C_MT5112              18
#define ERR_I2C_PROLDIOM            19
#define ERR_I2C_BDS_BOLT_ON         21

// Power down command with T8032
#define    PDWNC_CMD_ARMT8032_UNKNOWN 						0x0 
#define    PDWNC_CMD_ARMT8032_ENTER_STANDBY 				0x1
#define    PDWNC_CMD_ARMT8032_ENTER_NORMAL 					0x2 
#define    PDWNC_CMD_ARMT8032_QUERY_SW_VERSION 				0x3
#define    PDWNC_CMD_ARMT8032_XDATA_SWITCH 					0x4
#define    PDWNC_CMD_ARMT8032_READ_WAKEUP_REASON 			0x5
    #define    T8032_WAKEUP_REASON_CEC_ONE_TOUCH_PLAY 	0x1    
    #define    T8032_WAKEUP_REASON_COLD_REBOOT 			0x2
    #define T8032_WAKEUP_REASON_DIGIT_KEY 				0x6    
#define    PDWNC_CMD_ARMT8032_SET_LIGHT_GUIDE_LEVEL 		0x6
#define    PDWNC_CMD_ARMT8032_ALIVE_CHECK 					0x7
#define    PDWNC_CMD_ARMT8032_SET_ERROR_CODE 				0x8
#define    PDWNC_CMD_ARMT8032_SET_ERROR_CODE_BLINKING 		0x9
#define    PDWNC_CMD_ARMT8032_SET_MENU_LANGUAGE 			0xa
#define    PDWNC_CMD_ARMT8032_ENABLE_CEC 					0xb
#define    PDWNC_CMD_ARMT8032_ENABLE_ONE_TOUCH_PLAY 		0xc
#define    PDWNC_CMD_ARMT8032_SET_LED_BLINKER 				0xd
#define    PDWNC_CMD_ARMT8032_SET_IR_POWER_DOWN_ENABLE 		0xe
#define    PDWNC_CMD_ARMT8032_SET_POWER_DOWN_CONTROL 		0xf
#define    PDWNC_CMD_ARMT8032_CLEAR_POWER_DOWN_CONTROL 		0x10
#define    PDWNC_CMD_ARMT8032_COLD_REBOOT 					0x11
#define    PDWNC_CMD_ARMT8032_UART_PD_BAUDRATE 				0x16
#define    PDWNC_CMD_ARMT8032_LED_BLINKING 					0x17
#define    PDWNC_CMD_ARMT8032_GET_IRWAKEUP_VALUE  			0x18

#define    PDWNC_CMD_ARMT8032_SET_IR_FILTER_SYSTEM_CODE 	0x41
#define    PDWNC_CMD_ARMT8032_ENABLE_USER_CONTROL_PRESSED 	0x50
#define    PDWNC_CMD_ARMT8032_SET_VENDOR_ID 				0x51
#define    PDWNC_CMD_ARMT8032_SET_CEC_VERSION 				0x52
#define    PDWNC_CMD_ARMT8032_SET_OSD_NAME_1 				0x53
#define    PDWNC_CMD_ARMT8032_SET_OSD_NAME_2 				0x54
#define    PDWNC_CMD_ARMT8032_SET_OSD_NAME_3 				0x55
#define    PDWNC_CMD_ARMT8032_SET_OSD_NAME_4 				0x56
#define    PDWNC_CMD_ARMT8032_SET_WAKEUP_OPCODE_1 			0x57
#define    PDWNC_CMD_ARMT8032_SET_WAKEUP_OPCODE_2 			0x58
#define    PDWNC_CMD_ARMT8032_SET_WAKEUP_OPCODE_3 			0x59
#define    PDWNC_CMD_ARMT8032_SET_T8032_CTL_CEC	  			0x5a

#define 	SIF_PDWNC_TEST    1
#if SIF_PDWNC_TEST
#define    PDWNC_CMD_ARMT8032_PDSIF_READ	 				0x60
#define    PDWNC_CMD_ARMT8032_PDSIF_WRITE	  				0x61
#define    PDWNC_CMD_ARMT8032_PDSIF_EEP_READ	  			0x62
#define    PDWNC_CMD_ARMT8032_PDSIF_EEP_WRITE	  			0x63
#endif
#ifdef CC_SUPPORT_STR   // suspend to dram
#define    PDWNC_CMD_ARM8032_ENTER_SUSPEND	           		0x70
#define    PDWNC_CMD_ARM_WAKEUP_FROM_SUSPEND	       		0x71
#endif
#ifdef RCMM
#define    PDWNC_CMD_RCMM_WAKEUPKEY	       					0x72
#endif
#define    PDWNC_CMD_ARMT8032_SET_GPIO_ENABLE 				0x80
#define    PDWNC_CMD_ARMT8032_SET_GPIO_OUTPUT 				0x81
#define    PDWNC_CMD_ARMT8032_GET_GPIO_INPUT 				0x82
#define    PDWNC_CMD_ARMT8032_SET_SERVO_GPIO_ENABLE 		0x83
#define    PDWNC_CMD_ARMT8032_SET_SERVO_GPIO_OUTPUT 		0x84
#define    PDWNC_CMD_ARMT8032_GET_SERVO_GPIO_INPUT 			0x85
#define    PDWNC_CMD_ARMT8032_ENGINE_SWITCH 				0x86
//for cbus
#define    PDWNC_CMD_ARMT8032_CTRL_CBUS 					0x87
#define    PDWNC_CMD_ARMT8032_MAX 							0x88

#define    PDWNC_POWER_STATE_NORMAL 						0x200
#define    PDWNC_POWER_STATE_SEMI_STANDBY 					0x100
#define    PDWNC_POWER_STATE_CHAGNE_MASK 					0x300
#define    PDWNC_POWER_STATE_EVENT_MASK 					0xFF

// Power down event
typedef enum
{
    PDWNC_POWER_DOWN_UNKNOWN = 0, 
    PDWNC_POWER_DOWN_VGA,
    PDWNC_POWER_DOWN_RTC,
    PDWNC_POWER_DOWN_KEYPAD,
    PDWNC_POWER_DOWN_IRRC,
    PDWNC_POWER_DOWN_AC_POWER = 6
} PDWNC_POWER_DOWN_REASON_T;

typedef enum
{
    PDWNC_WAKE_UP_REASON_UNKNOWN = 0,		//0
    PDWNC_WAKE_UP_REASON_VGA,				//1
    PDWNC_WAKE_UP_REASON_RTC,				//2
    PDWNC_WAKE_UP_REASON_KEYPAD,			//3
    PDWNC_WAKE_UP_REASON_IRRC,				//4
    PDWNC_WAKE_UP_REASON_UART,				//5
    PDWNC_WAKE_UP_REASON_AC_POWER,			//6
    PDWNC_WAKE_UP_REASON_HDMI,				//7
    PDWNC_WAKE_UP_REASON_UART_NORMAL,		//8
    PDWNC_WAKE_UP_REASON_RC_DIGIT_0,		//9  /* Remote controller */
    PDWNC_WAKE_UP_REASON_RC_DIGIT_1,		//10
    PDWNC_WAKE_UP_REASON_RC_DIGIT_2,		//11
    PDWNC_WAKE_UP_REASON_RC_DIGIT_3,		//12
    PDWNC_WAKE_UP_REASON_RC_DIGIT_4,		//13
    PDWNC_WAKE_UP_REASON_RC_DIGIT_5,		//14
    PDWNC_WAKE_UP_REASON_RC_DIGIT_6,		//15
    PDWNC_WAKE_UP_REASON_RC_DIGIT_7,		//16
    PDWNC_WAKE_UP_REASON_RC_DIGIT_8,		//17
    PDWNC_WAKE_UP_REASON_RC_DIGIT_9,		//18
    PDWNC_WAKE_UP_REASON_RC_DIGIT_11,		//19
    PDWNC_WAKE_UP_REASON_RC_DIGIT_12,		//20
    PDWNC_WAKE_UP_REASON_RC_PRG_UP,			//21
    PDWNC_WAKE_UP_REASON_RC_PRG_DOWN,		//22
    PDWNC_WAKE_UP_REASON_RC_INP_SRC,		//23
    PDWNC_WAKE_UP_REASON_RC_ANALOG,			//24
    PDWNC_WAKE_UP_REASON_RC_DIGITAL,		//25
    PDWNC_WAKE_UP_REASON_RC_DIGITAL_ANALOG,	//26
    PDWNC_WAKE_UP_REASON_FP_PRG_UP, 		//27  /* Front panel */
    PDWNC_WAKE_UP_REASON_FP_PRG_DOWN,		//28
    PDWNC_WAKE_UP_REASON_FP_INP_SRC,		//29
    PDWNC_WAKE_UP_REASON_DVD,				//30
    PDWNC_WAKE_UP_REASON_RTC_SPECIAL,		//31
    PDWNC_WAKE_UP_REASON_WATCHDOG,			//32
    PDWNC_WAKE_UP_REASON_SCART,				//33
    PDWNC_WAKE_UP_REASON_ETHERNET,			//34
    PDWNC_WAKE_UP_REASON_RESUME_FROM_SUSPEND,//35
    PDWNC_WAKE_UP_REASON_WIFI,				//36
    PDWNC_WAKE_UP_REASON_BT,				//37
    PDWNC_WAKE_UP_REASON_RC_NETFLIX,		//38
    PDWNC_WAKE_UP_REASON_CUSTOM_1 = 61, 
    PDWNC_WAKE_UP_REASON_CUSTOM_2 = 62, 
    PDWNC_WAKE_UP_REASON_CUSTOM_3 = 63, 
    PDWNC_WAKE_UP_REASON_CUSTOM_4 = 64     
}   PDWNC_WAKE_UP_REASON_T;

typedef enum
{
    PDWNC_NORMAL = 0,
    PDWNC_WAIT_CEC_TRANSACTION,
    PDWNC_CEC_TRANSACTION_FINISH
}   PDWNC_CEC_HANDSHAKE_T;

typedef struct
{
    UINT8 u1Cmd;
    UINT8 u1SubCmd;    
    UINT8 au1Data[4];        
} PDWNC_T8032_CMD_T;

typedef struct
{
    UINT8 u1Ack;
    UINT8 au1Data[3];        
} PDWNC_T8032_RESPONSE_T;

typedef struct 
{
    UINT32 u4Reason;
    UINT32 u4Btn;
} PDWNC_SET_POWER_DOWN_REASON_T;

#define PDWNC_T8032CMD_SET_CMD(x) 		vIO32Write4B(PDWNC_SHREG1, (x))
#define PDWNC_T8032CMD_SET_DATA(x) 		vIO32Write4B(PDWNC_SHREG2, (x))
#define PDWNC_T8032CMD_GET_RESPONSE() 	u4IO32Read4B(PDWNC_SHREG0) 

#define MSET_XDATA_TO_ARM_FLAG() 	vIO32Write4B(PDWNC_SHREG3, (u4IO32Read4B(PDWNC_SHREG3) & 0xffffff00) | 0x000000a5)
#define MCLR_XDATA_TO_ARM_FLAG() 	vIO32Write4B(PDWNC_SHREG3, u4IO32Read4B(PDWNC_SHREG3) & 0xffffff00)
#define IS_XDATA_AT_ARM() 			((u4IO32Read4B(PDWNC_SHREG3) & 0x000000ff) == 0) ? 0 : 1
 
#define XDATA_ADDR          0x8000
/*NOTES:How to get the CEC_XDATA_OFFSET value:
1.Check XDATA ADDR  in "system.cfg", mark as A1.
2.Check the first variable address at 8032 in drv_hdmicec.c, mark as A2. eg:
u1CecPendingMsgIdx
3.Calculate the CEC_XDATA_OFFSET = A2 - A1.@Bo.
*/
#if defined(CC_MT5399) || defined(CC_MT5890) || defined(CC_MT5882)
#define CEC_XDATA_OFFSET    0x600
#elif  defined(CC_MT5881)
#define CEC_XDATA_OFFSET    0x100
#elif defined(CC_MT5565)
#define CEC_XDATA_OFFSET    0x200
#else  
#define CEC_XDATA_OFFSET 	0x00
#endif
#define CEC_XDATA_SIZE      0x200

//-----------------------------------------------------------------------------
#define GSPI_CPHA		(0x01)
#define GSPI_CPOL		(0x02)
#define GSPI_MODE_0		(0|0)
#define GSPI_MODE_1		(0|GSPI_CPHA)
#define GSPI_MODE_2		(GSPI_CPOL|0)
#define GSPI_MODE_3		(GSPI_CPOL|GSPI_CPHA)
#define GSPI_LSB_FIRST	(0x08)

#define GSPI_READ	(1U << 0)
#define GSPI_WRITE	(1U << 1)

typedef struct
{
    UINT32 u4Mode;
	UINT32 uBits;
	UINT32 u4Freq;
    UINT32 u4CsLead; //The latency between CS low and valid DATA output.
	UINT32 u4CsLag;  //The latency between valid DATA output and CS high.
	UINT32 u4CsHold; //The output CS hold time.
} PDWNC_GSPI_MODE_T;
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------
typedef void (* PFN_WDT_CALLBACK)(INT32 i4idx);

// PDWNC functions.
EXTERN INT32 PDWNC_Init(void);
EXTERN void PDWNC_pm_resume(void);
EXTERN void PDWNC_pm_suspend(void);
EXTERN void PDWNC_Reboot(void);
EXTERN void PDWNC_Reboot1(void);
EXTERN void PDWNC_WatchDogReboot(void);
EXTERN void PDWNC_T8032Reboot(UINT32 u4Seconds);
EXTERN void PDWNC_IrLedCheck(INT32 fgOn);
EXTERN UINT32 PDWNC_RequestSuspendEvent(UINT32 u4SuspendEn, UINT32 u4SuspendEvent);
EXTERN UINT32 PDWNC_EnterPowerDown(UINT32 u4PowerDownEn, UINT32 u4PowerDownEvent);
EXTERN UINT32 PDWNC_ReadServoADCChannelValue(UINT32 u4Channel);
EXTERN UINT32 PDWNC_IsWakeupByPowerButton(void);
EXTERN UINT32 PDWNC_IsBootByWakeup(void);
EXTERN UINT32 PDWNC_PowerDownByAcCut(void);
EXTERN void   PDWNC_CreateWatchDogThread(void);
EXTERN UINT32 PDWNC_ReadWakeupReason(void);
EXTERN UINT32 PDWNC_SetupPseudoWatchdogAssert(UINT32 *pu4Flag);

// Interrupt functions.
EXTERN BOOL PDWNC_ClearInt(UINT16 u2IntNo);
#ifndef CC_UBOOT
#if !defined(CC_SECURE_WORLD)
EXTERN INT32 PDWNC_RegIsr(UINT16 u2IntNo, x_os_isr_fct pfnIsr,x_os_isr_fct *ppfnOld);
#endif
#endif /* CC_UBOOT */

// GPIO functions
EXTERN INT32 PDWNC_GpioEnable(INT32 i4Gpio, INT32 *pfgSet);
EXTERN INT32 PDWNC_GpioOutput(INT32 i4Gpio, INT32 *pfgSet);
EXTERN INT32 PDWNC_GpioInput(INT32 i4Gpio);
EXTERN INT32 PDWNC_GpioIntrq(INT32 i4Gpio, INT32 *pfgSet);
EXTERN INT32 PDWNC_GpioQuery(INT32 i4Gpio, INT32 *pi4Intr,
                            INT32 *pi4Fall, INT32 *pi4Rise);
EXTERN INT32 PDWNC_GpioReg(INT32 i4Gpio, GPIO_TYPE eType,
                            PFN_GPIO_CALLBACK pfnCallback);
EXTERN void PDWNC_GPIO_Suspend(void);
EXTERN void PDWNC_GPIO_Resume(void);
EXTERN INT32 PDWNC_GpioResumeINTEN(INT32 i4Gpio, GPIO_TYPE eType);

EXTERN INT32 PDWNC_WriteErrorCode(UINT8 u1ErrorCode);

EXTERN void PDWNC_SetPowerDownFlag(UINT8 u1Flag);
EXTERN UINT8 PDWNC_GetPowerDownFlag(void);
EXTERN UINT32 PDWNC_EnterPowerState(UINT32 u4PowerDownEvent);
EXTERN UINT32 PDWNC_SetPowerDownReason(PDWNC_SET_POWER_DOWN_REASON_T rReason);
EXTERN UINT32 PDWNC_QueryPowerDownReason(PDWNC_SET_POWER_DOWN_REASON_T *prReason);
EXTERN void _PdwncSetupVGA(UINT32 u4PowerDownEn);
EXTERN INT32 PDWNC_T8032G381pioReg(INT32 i4Gpio, GPIO_TYPE eType, PFN_GPIO_CALLBACK pfnCallback);
EXTERN INT32 WDT_ISR_Reg(UINT32 u4Idx, PFN_WDT_CALLBACK pfnCallback);

EXTERN INT32 PDWNC_GspiInit(PDWNC_GSPI_MODE_T *prGspiMode);
EXTERN INT32 PDWNC_GspiReadWrite(UINT32 fgReadWrite, UINT32 u4Nob,UINT8 *u1RdBuf, UINT8 *u1WrBuf);
EXTERN void  IRTX_SetRealDMode(UINT32 uA1Dealy,UINT32 uB1Dealy,UINT8 uMode,UINT8 uFreq);


#ifndef CC_MTK_LOADER
EXTERN UINT32 _PDWNC_SetWatchDogDetectState(BOOL u1Enable);
EXTERN UINT32 _PDWNC_ReadWatchDogDetectStatus(void);
#endif /* CC_MTK_LOADER */

#if defined(CC_SUPPORT_STR_CORE_OFF)
EXTERN void PDWNC_SetSuspendMode(UINT32 SuspendMode);
EXTERN UINT32 PDWNC_IsSuspend(void);
#endif

#endif /* X_PDWNC_H */

