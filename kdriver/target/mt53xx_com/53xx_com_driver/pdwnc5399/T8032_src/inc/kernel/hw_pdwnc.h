/*----------------------------------------------------------------------------*
 * No Warranty                                                                *
 * Except as may be otherwise agreed to in writing, no warranties of any      *
 * kind, whether express or implied, are given by MTK with respect to any MTK *
 * Deliverables or any use thereof, and MTK Deliverables are provided on an   *
 * "AS IS" basis.  MTK hereby expressly disclaims all such warranties,        *
 * including any implied warranties of merchantability, non-infringement and  *
 * fitness for a particular purpose and any warranties arising out of course  *
 * of performance, course of dealing or usage of trade.  Parties further      *
 * acknowledge that Company may, either presently and/or in the future,       *
 * instruct MTK to assist it in the development and the implementation, in    *
 * accordance with Company's designs, of certain softwares relating to        *
 * Company's product(s) (the "Services").  Except as may be otherwise agreed  *
 * to in writing, no warranties of any kind, whether express or implied, are  *
 * given by MTK with respect to the Services provided, and the Services are   *
 * provided on an "AS IS" basis.  Company further acknowledges that the       *
 * Services may contain errors, that testing is important and Company is      *
 * solely responsible for fully testing the Services and/or derivatives       *
 * thereof before they are used, sublicensed or distributed.  Should there be *
 * any third party action brought against MTK, arising out of or relating to  *
 * the Services, Company agree to fully indemnify and hold MTK harmless.      *
 * If the parties mutually agree to enter into or continue a business         *
 * relationship or other arrangement, the terms and conditions set forth      *
 * hereunder shall remain effective and, unless explicitly stated otherwise,  *
 * shall prevail in the event of a conflict in the terms in any agreements    *
 * entered into between the parties.                                          *
 *---------------------------------------------------------------------------*/
/*************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of MediaTek Inc. (C) 2005
**************************************************************************/

/**
 * @file PDWNC HW register map
 *
 * @author dragon_chen@mtk.com.tw
 */

 
//#include"x_hal_5381.h"

#ifndef _HW_PDWNC_H_
#define _HW_PDWNC_H_
#define IO_VIRT 0
#ifdef CC_MT8223
#define IO_PDWNC_BASE (0x1000)
#else
#define IO_PDWNC_BASE (0x0)
#endif

//Page VDOIN_SYS
#define PDWNC_CLKPDN (IO_VIRT+IO_PDWNC_BASE + 0x000)
    #define FLD_CBUSCLKPD Fld(1,10,AC_MSKB1)//[10:10]
    #define FLD_MT8193CLKPD Fld(1,9,AC_MSKB1)//[9:9]
    #define FLD_HDMIEDIDPD Fld(1,8,AC_MSKB1)//[8:8]
    #define FLD_VGADDCPD Fld(1,7,AC_MSKB0)//[7:7]
    #define FLD_SIFPD Fld(1,6,AC_MSKB0)//[6:6]
    #define FLD_T8032PD Fld(1,5,AC_MSKB0)//[5:5]
    #define FLD_RTCPD Fld(1,4,AC_MSKB0)//[4:4]
    #define FLD_VGAPD Fld(1,3,AC_MSKB0)//[3:3]
    #define FLD_SRVPD Fld(1,2,AC_MSKB0)//[2:2]
    #define FLD_IRRXPD Fld(1,1,AC_MSKB0)//[1:1]
    #define FLD_CECPD Fld(1,0,AC_MSKB0)//[0:0]
#define PDWNC_IRCKSEL (IO_VIRT+IO_PDWNC_BASE + 0x004)
    #define FLD_IRCLKSEL Fld(4,0,AC_MSKB0)//[3:0]
#define PDWNC_PDMISC (IO_VIRT+IO_PDWNC_BASE + 0x008)
    #define FLD_ETH_RST_B Fld(1,29,AC_MSKB3)//[29:29]
    #define FLD_ETH_DEBUG_MODE Fld(1,28,AC_MSKB3)//[28:28]
    #define FLD_PDWNC_DEBUG_MODE Fld(1,27,AC_MSKB3)//[27:27]
    #if defined(CC_MT5882)
	#define FLD_SET_WAKECNT Fld(1,21,AC_MSKB2)//[21:21]
	#define FLD_DIS_RS232_UART Fld(1,19,AC_MSKB2)//[19:19]
	#define FLD_DIS_RS232_HDMI2 Fld(1,18,AC_MSKB2)//[18:18]
	#define FLD_DIS_RS232_HDMI1 Fld(1,17,AC_MSKB2)//[17:17]
	#define FLD_SYNC_RTC Fld(1,15,AC_MSKB1)//[15:15]
	#endif
    #define FLD_PDWN_POL Fld(1,14,AC_MSKB1)//[14:14]
    #define FLD_DIS_RS232_U0RX Fld(1,13,AC_MSKB1)//[13:13]
    #define FLD_DIS_RS232_VGA Fld(1,12,AC_MSKB1)//[12:12]
    #define FLD_VGA_SYNC_DIS Fld(1,11,AC_MSKB1)//[11:11]
    #define FLD_PDWN_EN Fld(1,10,AC_MSKB1)//[10:10]
    #if defined(CC_MT5399)
    #define FLD_CBUS_100K_ADJUST Fld(1,9,AC_MSKB1)//[9:9]
	#endif
#define PDWNC_PDMODE (IO_VIRT+IO_PDWNC_BASE + 0x010)
    #define FLD_DEEP_SLEEP Fld(1,5,AC_MSKB0)//[5:5]
#define PDWNC_WAKE_STONLY (IO_VIRT+IO_PDWNC_BASE + 0x020)
    #define FLD_WAK_STA_MUL Fld(1,31,AC_MSKB3)//[31:31]
    #define FLD_WAK_STON_EN Fld(31,0,AC_MSKDW)//[30:0]
#define PDWNC_TOUT_RSTCNT (IO_VIRT+IO_PDWNC_BASE + 0x024)
    #define FLD_TOUT_RSTCNT Fld(32,0,AC_FULLDW)//[31:0]
#define PDWNC_TOUT_CNT_MON (IO_VIRT+IO_PDWNC_BASE + 0x028)
    #define FLD_TOUT_CNT_MON Fld(32,0,AC_FULLDW)//[31:0]
#define PDWNC_MT8193_CLK_SEL (IO_VIRT+IO_PDWNC_BASE + 0x02C)
    #define FLD_MT8193_CLK_SEL Fld(2,0,AC_MSKB0)//[1:0]
#define PDWNC_HDMI_EDID_DET (IO_VIRT+IO_PDWNC_BASE + 0x030)
    #define FLD_DEV3 Fld(7,25,AC_MSKB3)//[31:25]
    #define FLD_DEV2 Fld(7,17,AC_MSKB2)//[23:17]
    #define FLD_DEV1 Fld(7,9,AC_MSKB1)//[15:9]
    #define FLD_DEV0 Fld(7,1,AC_MSKB0)//[7:1]
#define PDWNC_HDMI_EDID_DET_INTSTA (IO_VIRT+IO_PDWNC_BASE + 0x034)
    #define FLD_DEV3_DET_INT Fld(1,3,AC_MSKB0)//[3:3]
    #define FLD_DEV2_DET_INT Fld(1,2,AC_MSKB0)//[2:2]
    #define FLD_DEV1_DET_INT Fld(1,1,AC_MSKB0)//[1:1]
    #define FLD_DEV0_DET_INT Fld(1,0,AC_MSKB0)//[0:0]
#define PDWNC_HDMI_EDID_DET_INTEN (IO_VIRT+IO_PDWNC_BASE + 0x038)
    #define FLD_DEV3_DET_INTEN Fld(1,3,AC_MSKB0)//[3:3]
    #define FLD_DEV2_DET_INTEN Fld(1,2,AC_MSKB0)//[2:2]
    #define FLD_DEV1_DET_INTEN Fld(1,1,AC_MSKB0)//[1:1]
    #define FLD_DEV0_DET_INTEN Fld(1,0,AC_MSKB0)//[0:0]
#define PDWNC_HDMI_EDID_DET_INTCLR (IO_VIRT+IO_PDWNC_BASE + 0x03C)
    #define FLD_DEV3_DET_INTCLR Fld(1,3,AC_MSKB0)//[3:3]
    #define FLD_DEV2_DET_INTCLR Fld(1,2,AC_MSKB0)//[2:2]
    #define FLD_DEV1_DET_INTCLR Fld(1,1,AC_MSKB0)//[1:1]
    #define FLD_DEV0_DET_INTCLR Fld(1,0,AC_MSKB0)//[0:0]
#define PDWNC_INTSTA (IO_VIRT+IO_PDWNC_BASE + 0x040)
    #define FLD_SIFM2_INT_STA Fld(1,31,AC_MSKB3)//[31:31]
    #define FLD_CBUS_INT_STA Fld(1,30,AC_MSKB3)//[30:30]
    #define FLD_ETHERNET_INT_STA Fld(2,28,AC_MSKB3)//[29:28]
	#if defined(CC_MT5882)
	#define FLD_POWER_RDY_INT Fld(1,27,AC_MSKB3)//[27:27]
	#else //MT5399
    #define FLD_EXTIO_INT_STA_15 Fld(1,27,AC_MSKB3)//[27:27]
    #define FLD_EXTIO_INT_STA_13 Fld(1,26,AC_MSKB3)//[26:26]
    #endif
    #define FLD_DEV_DET_INT Fld(1,25,AC_MSKB3)//[25:25]
    #define FLD_GSPI_INT_STA Fld(1,24,AC_MSKB3)//[24:24]
    #define FLD_CEC_INT1_STA Fld(1,23,AC_MSKB2)//[23:23]
    #define FLD_AVLINK_INT1_STA Fld(1,22,AC_MSKB2)//[22:22]
    #define FLD_DDCCI_INT_STA Fld(1,21,AC_MSKB2)//[21:21]
    #define FLD_SIFM1_INT_STA Fld(1,20,AC_MSKB2)//[20:20]
    #define FLD_EXTIO_INT_STA_11_8 Fld(4,16,AC_MSKB2)//[19:16]
    #define FLD_AUX_INT_STA Fld(1,15,AC_MSKB1)//[15:15]
    #define FLD_ARM_INT_STA Fld(1,14,AC_MSKB1)//[14:14]
    #define FLD_IR_INT_STA Fld(1,13,AC_MSKB1)//[13:13]
    #define FLD_RTC_INT_STA Fld(1,12,AC_MSKB1)//[12:12]
    #define FLD_CEC_INT0_STA Fld(1,11,AC_MSKB1)//[11:11]
    #define FLD_AVLINK_INT0_STA Fld(1,10,AC_MSKB1)//[10:10]
    #define FLD_UART_DBG_INT_STA Fld(1,9,AC_MSKB1)//[9:9]
    #define FLD_SIFM0_INT_STA Fld(1,8,AC_MSKB1)//[8:8]
    #if defined(CC_MT5399)
    #define FLD_EXTIO_INT_STA_7_0 Fld(8,0,AC_FULLB0)//[7:0]
    #endif
#define PDWNC_ARM_INTEN (IO_VIRT+IO_PDWNC_BASE + 0x044)
    #define FLD_SIFM2_INTEN Fld(1,31,AC_MSKB3)//[31:31]
    #define FLD_CBUS_INTEN Fld(1,30,AC_MSKB3)//[30:30]
    #if defined(CC_MT5882)
	#define FLD_POWER_READY Fld(1,27,AC_MSKB3)//[27:27]
	#else
    #define FLD_EXTIO_INTEN_15 Fld(1,27,AC_MSKB3)//[27:27]
    #define FLD_EXTIO_INTEN_13 Fld(1,26,AC_MSKB3)//[26:26]
    #endif
    #define FLD_DEV_DET_INTEN Fld(1,25,AC_MSKB3)//[25:25]
    #define FLD_GSPI_INTEN Fld(1,24,AC_MSKB3)//[24:24]
    #define FLD_CEC_INT1EN Fld(1,23,AC_MSKB2)//[23:23]
    #define FLD_AVLINK_INT1EN Fld(1,22,AC_MSKB2)//[22:22]
    #define FLD_DDCCI_INT_EN Fld(1,21,AC_MSKB2)//[21:21]
    #define FLD_SIFM1_INTEN Fld(1,20,AC_MSKB2)//[20:20]
    #define FLD_EXTIO_INTEN_11_8 Fld(4,16,AC_MSKB2)//[19:16]
    #define FLD_AUX_INTEN Fld(1,15,AC_MSKB1)//[15:15]
    #define FLD_ARM_INTEN Fld(1,14,AC_MSKB1)//[14:14]
    #define FLD_IR_INTEN Fld(1,13,AC_MSKB1)//[13:13]
    #define FLD_RTC_INTEN Fld(1,12,AC_MSKB1)//[12:12]
    #define FLD_CEC_INT0EN Fld(1,11,AC_MSKB1)//[11:11]
    #define FLD_AVLINK_INT0EN Fld(1,10,AC_MSKB1)//[10:10]
    #define FLD_UART_DBG_INTEN Fld(1,9,AC_MSKB1)//[9:9]
    #define FLD_SIFM0_INTEN Fld(1,8,AC_MSKB1)//[8:8]
    #if defined(CC_MT5399)
    #define FLD_EXTIO_INTEN_7_0 Fld(8,0,AC_FULLB0)//[7:0]
    #endif
#define PDWNC_INTCLR (IO_VIRT+IO_PDWNC_BASE + 0x048)
    #define FLD_SIFM2_INTCLR Fld(1,31,AC_MSKB3)//[31:31]
    #define FLD_CBUS_INTCLR Fld(1,30,AC_MSKB3)//[30:30]
    #if defined(CC_MT5882)
	#define FLD_POWER_RDY_CLR Fld(1,27,AC_MSKB3)//[27:27]
	#else
    #define FLD_EXTIO_INTCLR_15 Fld(1,27,AC_MSKB3)//[27:27]
    #define FLD_EXTIO_INTCLR_13 Fld(1,26,AC_MSKB3)//[26:26]
    #endif
    #define FLD_CEC_INT1CLR Fld(1,23,AC_MSKB2)//[23:23]
    #define FLD_AVLINK_INT1CLR Fld(1,22,AC_MSKB2)//[22:22]
    #define FLD_SIFM1_INTCLR Fld(1,20,AC_MSKB2)//[20:20]
    #define FLD_EXTIO_INTCLR_11_8 Fld(4,16,AC_MSKB2)//[19:16]
    #define FLD_AUX_INTCLR Fld(1,15,AC_MSKB1)//[15:15]
    #define FLD_ARM_INTCLR Fld(1,14,AC_MSKB1)//[14:14]
    #define FLD_IR_INTCLR Fld(1,13,AC_MSKB1)//[13:13]
    #define FLD_RTC_INTCLR Fld(1,12,AC_MSKB1)//[12:12]
    #define FLD_CEC_INT0CLR Fld(1,11,AC_MSKB1)//[11:11]
    #define FLD_AVLINK_INT0CLR Fld(1,10,AC_MSKB1)//[10:10]
    #define FLD_UART_DBG_INTCLR Fld(1,9,AC_MSKB1)//[9:9]
    #define FLD_SIFM0_INTCLR Fld(1,8,AC_MSKB1)//[8:8]
    #if defined(CC_MT5399)
    #define FLD_EXTIO_INTCLR_7_0 Fld(8,0,AC_FULLB0)//[7:0]
    #endif
#define PDWNC_T8032_INTEN (IO_VIRT+IO_PDWNC_BASE + 0x04C)
    #define FLD_SIFM2_INTEN Fld(1,31,AC_MSKB3)//[31:31]
    #define FLD_CBUS_INTEN Fld(1,30,AC_MSKB3)//[30:30]
    #define FLD_T8032_ETHERNET_INTEN Fld(2,28,AC_MSKB3)//[29:28]
    #if defined(CC_MT5882)
	#define FLD_T8032_POWER_INTEN Fld(1,27,AC_MSKB3)//[27:27]
	#else
    #define FLD_T8032_EXTIO_INTEN_15 Fld(1,27,AC_MSKB3)//[27:27]
    #define FLD_T8032_EXTIO_INTEN_13 Fld(1,26,AC_MSKB3)//[26:26]
    #endif
    #define FLD_T8032_DEV_DET_INTEN Fld(1,25,AC_MSKB3)//[25:25]
    #define FLD_T8032_GSPI_INTEN Fld(1,24,AC_MSKB3)//[24:24]
    #define FLD_T8032_CEC_INT1EN Fld(1,23,AC_MSKB2)//[23:23]
    #define FLD_T8032_AVLINK_INT1EN Fld(1,22,AC_MSKB2)//[22:22]
    #define FLD_T8032_DDCCI_INT_EN Fld(1,21,AC_MSKB2)//[21:21]
    #define FLD_T8032_SIFM1_INTEN Fld(1,20,AC_MSKB2)//[20:20]
    #define FLD_T8032_EXTIO_INTEN_11_8 Fld(4,16,AC_MSKB2)//[19:16]
    #define FLD_T8032_AUX_INTEN Fld(1,15,AC_MSKB1)//[15:15]
    #define FLD_T8032_ARM_INTEN Fld(1,14,AC_MSKB1)//[14:14]
    #define FLD_T8032_IR_INTEN Fld(1,13,AC_MSKB1)//[13:13]
    #define FLD_T8032_RTC_INTEN Fld(1,12,AC_MSKB1)//[12:12]
    #define FLD_T8032_CEC_INT0EN Fld(1,11,AC_MSKB1)//[11:11]
    #define FLD_T8032_AVLINK_INT0EN Fld(1,10,AC_MSKB1)//[10:10]
    #define FLD_T8032_UART_DBG_INTEN Fld(1,9,AC_MSKB1)//[9:9]
    #define FLD_T8032_SIFM0_INTEN Fld(1,8,AC_MSKB1)//[8:8]
    #if defined(CC_MT5399)
    #define FLD_T8032_EXTIO_INTEN_7_0 Fld(8,0,AC_FULLB0)//[7:0]
    #endif
#define PDWNC_WDTINT_STA (IO_VIRT+IO_PDWNC_BASE + 0x050)
    #define FLD_WDT_INT_STA Fld(3,0,AC_MSKB0)//[2:0]
#define PDWNC_WDTINT_EN (IO_VIRT+IO_PDWNC_BASE + 0x054)
    #define FLD_WDT_INT_EN Fld(3,0,AC_MSKB0)//[2:0]
#define PDWNC_WDTINT_CLR (IO_VIRT+IO_PDWNC_BASE + 0x058)
    #define FLD_WDT_INT_CLR Fld(3,0,AC_MSKB0)//[2:0]
#define PDWNC_INTSTA_2 (IO_VIRT+IO_PDWNC_BASE + 0x05C)
    //#define FLD_EXTIO_INT_STA Fld(1,5,AC_MSKB0)//[5:5]
    //#define FLD_EXTIO_INT_STA Fld(1,4,AC_MSKB0)//[4:4]
    //#define FLD_EXTIO_INT_STA Fld(1,3,AC_MSKB0)//[3:3]
    //#define FLD_EXTIO_INT_STA Fld(1,2,AC_MSKB0)//[2:2]
    //#define FLD_EXTIO_INT_STA Fld(1,1,AC_MSKB0)//[1:1]
    #if defined(CC_MT5882)
	#define FLD_EXTIO_INT_STA Fld(22,0,AC_MSKDW)//[21:0]
	#else
    #define FLD_EXTIO_INT_STA Fld(6,0,AC_MSKB0)//[5:0]
    #endif
#define PDWNC_ARM_INTEN_2 (IO_VIRT+IO_PDWNC_BASE + 0x060)
    //#define FLD_EXTIO_INTEN Fld(1,5,AC_MSKB0)//[5:5]
    //#define FLD_EXTIO_INTEN Fld(1,4,AC_MSKB0)//[4:4]
    //#define FLD_EXTIO_INTEN Fld(1,3,AC_MSKB0)//[3:3]
    //#define FLD_EXTIO_INTEN Fld(1,2,AC_MSKB0)//[2:2]
    //#define FLD_EXTIO_INTEN Fld(1,1,AC_MSKB0)//[1:1]
    #if defined(CC_MT5882)
	#define FLD_ARM_EXTIO_INTEN Fld(22,0,AC_MSKDW)//[21:0]
	#else
    #define FLD_EXTIO_INTEN Fld(6,0,AC_MSKB0)//[0:0]
    #endif
#define PDWNC_INTCLR_2 (IO_VIRT+IO_PDWNC_BASE + 0x064)
    //#define FLD_EXTIO_INTCLR Fld(1,5,AC_MSKB0)//[5:5]
    //#define FLD_EXTIO_INTCLR Fld(1,4,AC_MSKB0)//[4:4]
    //#define FLD_EXTIO_INTCLR Fld(1,3,AC_MSKB0)//[3:3]
    //#define FLD_EXTIO_INTCLR Fld(1,2,AC_MSKB0)//[2:2]
    //#define FLD_EXTIO_INTCLR Fld(1,1,AC_MSKB0)//[1:1]
    #if defined(CC_MT5882)
	#define FLD_EXTIO_INTCLR Fld(22,0,AC_MSKDW)//[21:0]
	#else
    #define FLD_EXTIO_INTCLR Fld(6,0,AC_MSKB0)//[0:0]
    #endif
#define PDWNC_T8032_INTEN_2 (IO_VIRT+IO_PDWNC_BASE + 0x068)
    //#define FLD_T8032_EXTIO_INTEN Fld(1,5,AC_MSKB0)//[5:5]
    //#define FLD_T8032_EXTIO_INTEN Fld(1,4,AC_MSKB0)//[4:4]
    //#define FLD_T8032_EXTIO_INTEN Fld(1,3,AC_MSKB0)//[3:3]
    //#define FLD_T8032_EXTIO_INTEN Fld(1,2,AC_MSKB0)//[2:2]
    //#define FLD_T8032_EXTIO_INTEN Fld(1,1,AC_MSKB0)//[1:1]
    #if defined(CC_MT5882)
	#define FLD_T8032_EXTIO_INTEN Fld(22,0,AC_MSKDW)//[21:0]
	#else
    #define FLD_T8032_EXTIO_INTEN Fld(6,0,AC_MSKB0)//[0:0]
    #endif
#define PDWNC_GPIOOUT0 (IO_VIRT+IO_PDWNC_BASE + 0x074)
    #define FLD_PD_GPIO_OUT0 Fld(32,0,AC_FULLDW)//[31:0]
#define PDWNC_GPIOOUT1 (IO_VIRT+IO_PDWNC_BASE + 0x078)
	#if defined(CC_MT5882)
	#define FLD_PD_GPIO_OUT1 Fld(10,0,AC_MSKW10)//[9:0]
	#else
    #define FLD_PD_GPIO_OUT1 Fld(28,0,AC_MSKDW)//[27:0]
    #endif
#define PDWNC_GPIOEN0 (IO_VIRT+IO_PDWNC_BASE + 0x07C)
    #define FLD_PD_GPIO_EN0 Fld(32,0,AC_FULLDW)//[31:0]
#define PDWNC_GPIOEN1 (IO_VIRT+IO_PDWNC_BASE + 0x080)
	#if defined(CC_MT5399)
    #define FLD_PD_GPIO_EN1_HI Fld(13,16,AC_MSKW32)//[28:16]
    #endif
    #define FLD_UART_TX_EN Fld(1,15,AC_MSKB1)//[15:15]
    #if defined(CC_MT5882)
	#define FLD_PD_GPIO_IN1 Fld(10,0,AC_MSKW10)//[9:0]
	#else
    #define FLD_PD_GPIO_EN1_LO Fld(15,0,AC_MSKW10)//[14:0]
    #endif
#define PDWNC_GPIOIN0 (IO_VIRT+IO_PDWNC_BASE + 0x084)
    #define FLD_PD_GPIO_IN0 Fld(32,0,AC_FULLDW)//[31:0]
#define PDWNC_GPIOIN1 (IO_VIRT+IO_PDWNC_BASE + 0x088)
	#if defined(CC_MT5882)
	#define FLD_PD_GPIO_IN1 Fld(10,0,AC_MSKW10)//[9:0]
	#else
    #define FLD_PD_GPIO_IN1 Fld(28,0,AC_MSKDW)//[27:0]
    #endif
#if defined(CC_MT5882)
#define PDWNC_TOUT_CNT_MON2 (IO_VIRT+IO_PDWNC_BASE + 0x090)
    #define FLD_TOUT_CNT_MON2 Fld(32,0,AC_FULLDW)//[31:0]
#endif
#define PDWNC_PDIO (IO_VIRT+IO_PDWNC_BASE + 0x09C)
    #define FLD_DEGCNT Fld(8,16,AC_FULLB2)//[23:16]
    #if defined(CC_MT5882)
	#define FLD_EXTIO_POL Fld(13,0,AC_MSKW10)//[12:0]
	#else
    #define FLD_EXTIO_POL Fld(12,0,AC_MSKW10)//[11:0]
    #endif
#define PDWNC_PADRDSEL (IO_VIRT+IO_PDWNC_BASE + 0x0A0)
    #define FLD_RDSEL Fld(12,0,AC_MSKW10)//[11:0]
#define PDWNC_PADTDSEL (IO_VIRT+IO_PDWNC_BASE + 0x0A4)
    #define FLD_TDSEL Fld(24,0,AC_MSKDW)//[23:0]
#define PDWNC_EXINT2ED (IO_VIRT+IO_PDWNC_BASE + 0x0A8)
    #define FLD_EXINT_2ED Fld(32,0,AC_FULLDW)//[31:0]
#define PDWNC_EXINTLEV (IO_VIRT+IO_PDWNC_BASE + 0x0AC)
    #define FLD_EXINT_LEV Fld(32,0,AC_FULLDW)//[31:0]
#define PDWNC_EXINTPOL (IO_VIRT+IO_PDWNC_BASE + 0x0B0)
    #define FLD_EXINT_POL Fld(32,0,AC_FULLDW)//[31:0]
#define PDWNC_PINMUX0 (IO_VIRT+IO_PDWNC_BASE + 0x0B4)
    #define FLD_PDMUX0 Fld(32,0,AC_FULLDW)//[31:0]
#define PDWNC_PINMUX1 (IO_VIRT+IO_PDWNC_BASE + 0x0B8)
    #define FLD_PDMUX1 Fld(32,0,AC_FULLDW)//[31:0]
#define PDWNC_PINMUX2 (IO_VIRT+IO_PDWNC_BASE + 0x0BC)
    #define FLD_PDMUX2 Fld(14,0,AC_MSKW10)//[13:0]
    #if defined(CC_MT5399)
    #define FLD_PAD_GPIO55_PDMUX Fld(1,23,AC_MSKB2)//[23:23]
    #define FLD_PAD_GPIO54_PDMUX Fld(1,22,AC_MSKB2)//[22:22]
    #define FLD_PAD_GPIO53_PDMUX Fld(1,21,AC_MSKB2)//[21:21]
    #define FLD_PAD_GPIO52_PDMUX Fld(1,20,AC_MSKB2)//[20:20]
    #define FLD_PAD_GPIO51_PDMUX Fld(1,19,AC_MSKB2)//[19:19]
    #define FLD_PAD_GPIO50_PDMUX Fld(1,15,AC_MSKB1)//[15:15]
    #endif
#if defined(CC_MT5399)
#define PDWNC_PADCFG0 (IO_VIRT+IO_PDWNC_BASE + 0x0C0)
    #define FLD_HDMI_1_HPD_SMT Fld(1,29,AC_MSKB3)//[29:29]
    #define FLD_HDMI_1_HPD_PD Fld(1,28,AC_MSKB3)//[28:28]
    #define FLD_HDMI_1_HPD_PU Fld(1,27,AC_MSKB3)//[27:27]
    #define FLD_HDMI_1_HPD_SR Fld(1,26,AC_MSKB3)//[26:26]
    #define FLD_HDMI_1_HPD_E4 Fld(1,25,AC_MSKB3)//[25:25]
    #define FLD_HDMI_1_HPD_E2 Fld(1,24,AC_MSKB3)//[24:24]
    #define FLD_HDMI_1_SDA_SMT Fld(1,21,AC_MSKB2)//[21:21]
    #define FLD_HDMI_1_SDA_PD Fld(1,20,AC_MSKB2)//[20:20]
    #define FLD_HDMI_1_SDA_PU Fld(1,19,AC_MSKB2)//[19:19]
    #define FLD_HDMI_1_SDA_SR Fld(1,18,AC_MSKB2)//[18:18]
    #define FLD_HDMI_1_SDA_E4 Fld(1,17,AC_MSKB2)//[17:17]
    #define FLD_HDMI_1_SDA_E2 Fld(1,16,AC_MSKB2)//[16:16]
    #define FLD_HDMI_1_SCL_SMT Fld(1,13,AC_MSKB1)//[13:13]
    #define FLD_HDMI_1_SCL_PD Fld(1,12,AC_MSKB1)//[12:12]
    #define FLD_HDMI_1_SCL_PU Fld(1,11,AC_MSKB1)//[11:11]
    #define FLD_HDMI_1_SCL_SR Fld(1,10,AC_MSKB1)//[10:10]
    #define FLD_HDMI_1_SCL_E4 Fld(1,9,AC_MSKB1)//[9:9]
    #define FLD_HDMI_1_SCL_E2 Fld(1,8,AC_MSKB1)//[8:8]
    #define FLD_HDMI_CEC_SMT Fld(1,5,AC_MSKB0)//[5:5]
    #define FLD_HDMI_CEC_PD Fld(1,4,AC_MSKB0)//[4:4]
    #define FLD_HDMI_CEC_PU Fld(1,3,AC_MSKB0)//[3:3]
    #define FLD_HDMI_CEC_SR Fld(1,2,AC_MSKB0)//[2:2]
    #define FLD_HDMI_CEC_E4 Fld(1,1,AC_MSKB0)//[1:1]
    #define FLD_HDMI_CEC_E2 Fld(1,0,AC_MSKB0)//[0:0]
#define PDWNC_PADCFG1 (IO_VIRT+IO_PDWNC_BASE + 0x0C4)
    #define FLD_OPCTRL0_SMT Fld(1,29,AC_MSKB3)//[29:29]
    #define FLD_OPCTRL0_PD Fld(1,28,AC_MSKB3)//[28:28]
    #define FLD_OPCTRL0_PU Fld(1,27,AC_MSKB3)//[27:27]
    #define FLD_OPCTRL0_SR Fld(1,26,AC_MSKB3)//[26:26]
    #define FLD_OPCTRL0_E4 Fld(1,25,AC_MSKB3)//[25:25]
    #define FLD_OPCTRL0_E2 Fld(1,24,AC_MSKB3)//[24:24]
    #define FLD_HDMI_2_HPD_SMT Fld(1,21,AC_MSKB2)//[21:21]
    #define FLD_HDMI_2_HPD_PD Fld(1,20,AC_MSKB2)//[20:20]
    #define FLD_HDMI_2_HPD_PU Fld(1,19,AC_MSKB2)//[19:19]
    #define FLD_HDMI_2_HPD_SR Fld(1,18,AC_MSKB2)//[18:18]
    #define FLD_HDMI_2_HPD_E4 Fld(1,17,AC_MSKB2)//[17:17]
    #define FLD_HDMI_2_HPD_E2 Fld(1,16,AC_MSKB2)//[16:16]
    #define FLD_HDMI_2_SDA_SMT Fld(1,13,AC_MSKB1)//[13:13]
    #define FLD_HDMI_2_SDA_PD Fld(1,12,AC_MSKB1)//[12:12]
    #define FLD_HDMI_2_SDA_PU Fld(1,11,AC_MSKB1)//[11:11]
    #define FLD_HDMI_2_SDA_SR Fld(1,10,AC_MSKB1)//[10:10]
    #define FLD_HDMI_2_SDA_E4 Fld(1,9,AC_MSKB1)//[9:9]
    #define FLD_HDMI_2_SDA_E2 Fld(1,8,AC_MSKB1)//[8:8]
    #define FLD_HDMI_2_SCL_SMT Fld(1,5,AC_MSKB0)//[5:5]
    #define FLD_HDMI_2_SCL_PD Fld(1,4,AC_MSKB0)//[4:4]
    #define FLD_HDMI_2_SCL_PU Fld(1,3,AC_MSKB0)//[3:3]
    #define FLD_HDMI_2_SCL_SR Fld(1,2,AC_MSKB0)//[2:2]
    #define FLD_HDMI_2_SCL_E4 Fld(1,1,AC_MSKB0)//[1:1]
    #define FLD_HDMI_2_SCL_E2 Fld(1,0,AC_MSKB0)//[0:0]
#define PDWNC_PADCFG2 (IO_VIRT+IO_PDWNC_BASE + 0x0C8)
    #define FLD_OPCTRL4_SMT Fld(1,29,AC_MSKB3)//[29:29]
    #define FLD_OPCTRL4_PD Fld(1,28,AC_MSKB3)//[28:28]
    #define FLD_OPCTRL4_PU Fld(1,27,AC_MSKB3)//[27:27]
    #define FLD_OPCTRL4_SR Fld(1,26,AC_MSKB3)//[26:26]
    #define FLD_OPCTRL4_E4 Fld(1,25,AC_MSKB3)//[25:25]
    #define FLD_OPCTRL4_E2 Fld(1,24,AC_MSKB3)//[24:24]
    #define FLD_OPCTRL3_SMT Fld(1,21,AC_MSKB2)//[21:21]
    #define FLD_OPCTRL3_PD Fld(1,20,AC_MSKB2)//[20:20]
    #define FLD_OPCTRL3_PU Fld(1,19,AC_MSKB2)//[19:19]
    #define FLD_OPCTRL3_SR Fld(1,18,AC_MSKB2)//[18:18]
    #define FLD_OPCTRL3_E4 Fld(1,17,AC_MSKB2)//[17:17]
    #define FLD_OPCTRL3_E2 Fld(1,16,AC_MSKB2)//[16:16]
    #define FLD_OPCTRL2_SMT Fld(1,13,AC_MSKB1)//[13:13]
    #define FLD_OPCTRL2_PD Fld(1,12,AC_MSKB1)//[12:12]
    #define FLD_OPCTRL2_PU Fld(1,11,AC_MSKB1)//[11:11]
    #define FLD_OPCTRL2_SR Fld(1,10,AC_MSKB1)//[10:10]
    #define FLD_OPCTRL2_E4 Fld(1,9,AC_MSKB1)//[9:9]
    #define FLD_OPCTRL2_E2 Fld(1,8,AC_MSKB1)//[8:8]
    #define FLD_OPCTRL1_SMT Fld(1,5,AC_MSKB0)//[5:5]
    #define FLD_OPCTRL1_PD Fld(1,4,AC_MSKB0)//[4:4]
    #define FLD_OPCTRL1_PU Fld(1,3,AC_MSKB0)//[3:3]
    #define FLD_OPCTRL1_SR Fld(1,2,AC_MSKB0)//[2:2]
    #define FLD_OPCTRL1_E4 Fld(1,1,AC_MSKB0)//[1:1]
    #define FLD_OPCTRL1_E2 Fld(1,0,AC_MSKB0)//[0:0]
#define PDWNC_PADCFG3 (IO_VIRT+IO_PDWNC_BASE + 0x0CC)
    #define FLD_U0TX_SMT Fld(1,29,AC_MSKB3)//[29:29]
    #define FLD_U0TX_PD Fld(1,28,AC_MSKB3)//[28:28]
    #define FLD_U0TX_PU Fld(1,27,AC_MSKB3)//[27:27]
    #define FLD_U0TX_SR Fld(1,26,AC_MSKB3)//[26:26]
    #define FLD_U0TX_E4 Fld(1,25,AC_MSKB3)//[25:25]
    #define FLD_U0TX_E2 Fld(1,24,AC_MSKB3)//[24:24]
    #define FLD_U0RX_SMT Fld(1,21,AC_MSKB2)//[21:21]
    #define FLD_U0RX_PD Fld(1,20,AC_MSKB2)//[20:20]
    #define FLD_U0RX_PU Fld(1,19,AC_MSKB2)//[19:19]
    #define FLD_U0RX_SR Fld(1,18,AC_MSKB2)//[18:18]
    #define FLD_U0RX_E4 Fld(1,17,AC_MSKB2)//[17:17]
    #define FLD_U0RX_E2 Fld(1,16,AC_MSKB2)//[16:16]
    #define FLD_VGA_SDA_SMT Fld(1,13,AC_MSKB1)//[13:13]
    #define FLD_VGA_SDA_PD Fld(1,12,AC_MSKB1)//[12:12]
    #define FLD_VGA_SDA_PU Fld(1,11,AC_MSKB1)//[11:11]
    #define FLD_VGA_SDA_SR Fld(1,10,AC_MSKB1)//[10:10]
    #define FLD_VGA_SDA_E4 Fld(1,9,AC_MSKB1)//[9:9]
    #define FLD_VGA_SDA_E2 Fld(1,8,AC_MSKB1)//[8:8]
    #define FLD_VGA_SCL_SMT Fld(1,5,AC_MSKB0)//[5:5]
    #define FLD_VGA_SCL_PD Fld(1,4,AC_MSKB0)//[4:4]
    #define FLD_VGA_SCL_PU Fld(1,3,AC_MSKB0)//[3:3]
    #define FLD_VGA_SCL_SR Fld(1,2,AC_MSKB0)//[2:2]
    #define FLD_VGA_SCL_E4 Fld(1,1,AC_MSKB0)//[1:1]
    #define FLD_VGA_SCL_E2 Fld(1,0,AC_MSKB0)//[0:0]
#define PDWNC_PADCFG4 (IO_VIRT+IO_PDWNC_BASE + 0x0D0)
    #define FLD_LED_PWM1_SMT Fld(1,29,AC_MSKB3)//[29:29]
    #define FLD_LED_PWM1_PD Fld(1,28,AC_MSKB3)//[28:28]
    #define FLD_LED_PWM1_PU Fld(1,27,AC_MSKB3)//[27:27]
    #define FLD_LED_PWM1_SR Fld(1,26,AC_MSKB3)//[26:26]
    #define FLD_LED_PWM1_E4 Fld(1,25,AC_MSKB3)//[25:25]
    #define FLD_LED_PWM1_E2 Fld(1,24,AC_MSKB3)//[24:24]
    #define FLD_LED_PWM0_SMT Fld(1,21,AC_MSKB2)//[21:21]
    #define FLD_LED_PWM0_PD Fld(1,20,AC_MSKB2)//[20:20]
    #define FLD_LED_PWM0_PU Fld(1,19,AC_MSKB2)//[19:19]
    #define FLD_LED_PWM0_SR Fld(1,18,AC_MSKB2)//[18:18]
    #define FLD_LED_PWM0_E4 Fld(1,17,AC_MSKB2)//[17:17]
    #define FLD_LED_PWM0_E2 Fld(1,16,AC_MSKB2)//[16:16]
    #define FLD_OPWRSB_SMT Fld(1,13,AC_MSKB1)//[13:13]
    #define FLD_OPWRSB_PD Fld(1,12,AC_MSKB1)//[12:12]
    #define FLD_OPWRSB_PU Fld(1,11,AC_MSKB1)//[11:11]
    #define FLD_OPWRSB_SR Fld(1,10,AC_MSKB1)//[10:10]
    #define FLD_OPWRSB_E4 Fld(1,9,AC_MSKB1)//[9:9]
    #define FLD_OPWRSB_E2 Fld(1,8,AC_MSKB1)//[8:8]
    #define FLD_OIRI_SMT Fld(1,5,AC_MSKB0)//[5:5]
    #define FLD_OIRI_PD Fld(1,4,AC_MSKB0)//[4:4]
    #define FLD_OIRI_PU Fld(1,3,AC_MSKB0)//[3:3]
    #define FLD_OIRI_SR Fld(1,2,AC_MSKB0)//[2:2]
    #define FLD_OIRI_E4 Fld(1,1,AC_MSKB0)//[1:1]
    #define FLD_OIRI_E2 Fld(1,0,AC_MSKB0)//[0:0]
#define PDWNC_PADCFG5 (IO_VIRT+IO_PDWNC_BASE + 0x0D4)
    #define FLD_HDMI_0_5V_SMT Fld(1,29,AC_MSKB3)//[29:29]
    #define FLD_HDMI_0_5V_PD Fld(1,28,AC_MSKB3)//[28:28]
    #define FLD_HDMI_0_5V_PU Fld(1,27,AC_MSKB3)//[27:27]
    #define FLD_HDMI_0_5V_SR Fld(1,26,AC_MSKB3)//[26:26]
    #define FLD_HDMI_0_5V_E4 Fld(1,25,AC_MSKB3)//[25:25]
    #define FLD_HDMI_0_5V_E2 Fld(1,24,AC_MSKB3)//[24:24]
    #define FLD_HDMI_0_HPD_SMT Fld(1,21,AC_MSKB2)//[21:21]
    #define FLD_HDMI_0_HPD_PD Fld(1,20,AC_MSKB2)//[20:20]
    #define FLD_HDMI_0_HPD_PU Fld(1,19,AC_MSKB2)//[19:19]
    #define FLD_HDMI_0_HPD_SR Fld(1,18,AC_MSKB2)//[18:18]
    #define FLD_HDMI_0_HPD_E4 Fld(1,17,AC_MSKB2)//[17:17]
    #define FLD_HDMI_0_HPD_E2 Fld(1,16,AC_MSKB2)//[16:16]
    #define FLD_HDMI_0_SDA_SMT Fld(1,13,AC_MSKB1)//[13:13]
    #define FLD_HDMI_0_SDA_PD Fld(1,12,AC_MSKB1)//[12:12]
    #define FLD_HDMI_0_SDA_PU Fld(1,11,AC_MSKB1)//[11:11]
    #define FLD_HDMI_0_SDA_SR Fld(1,10,AC_MSKB1)//[10:10]
    #define FLD_HDMI_0_SDA_E4 Fld(1,9,AC_MSKB1)//[9:9]
    #define FLD_HDMI_0_SDA_E2 Fld(1,8,AC_MSKB1)//[8:8]
    #define FLD_HDMI_0_SCL_SMT Fld(1,5,AC_MSKB0)//[5:5]
    #define FLD_HDMI_0_SCL_PD Fld(1,4,AC_MSKB0)//[4:4]
    #define FLD_HDMI_0_SCL_PU Fld(1,3,AC_MSKB0)//[3:3]
    #define FLD_HDMI_0_SCL_SR Fld(1,2,AC_MSKB0)//[2:2]
    #define FLD_HDMI_0_SCL_E4 Fld(1,1,AC_MSKB0)//[1:1]
    #define FLD_HDMI_0_SCL_E2 Fld(1,0,AC_MSKB0)//[0:0]
#define PDWNC_PADCFG6 (IO_VIRT+IO_PDWNC_BASE + 0x0D8)
    #define FLD_STB_SDA_SMT Fld(1,29,AC_MSKB3)//[29:29]
    #define FLD_STB_SDA_PD Fld(1,28,AC_MSKB3)//[28:28]
    #define FLD_STB_SDA_PU Fld(1,27,AC_MSKB3)//[27:27]
    #define FLD_STB_SDA_SR Fld(1,26,AC_MSKB3)//[26:26]
    #define FLD_STB_SDA_E4 Fld(1,25,AC_MSKB3)//[25:25]
    #define FLD_STB_SDA_E2 Fld(1,24,AC_MSKB3)//[24:24]
    #define FLD_STB_SCL_SMT Fld(1,21,AC_MSKB2)//[21:21]
    #define FLD_STB_SCL_PD Fld(1,20,AC_MSKB2)//[20:20]
    #define FLD_STB_SCL_PU Fld(1,19,AC_MSKB2)//[19:19]
    #define FLD_STB_SCL_SR Fld(1,18,AC_MSKB2)//[18:18]
    #define FLD_STB_SCL_E4 Fld(1,17,AC_MSKB2)//[17:17]
    #define FLD_STB_SCL_E2 Fld(1,16,AC_MSKB2)//[16:16]
    #define FLD_SRVADC_SMT Fld(1,13,AC_MSKB1)//[13:13]
    #define FLD_SRVADC_PD Fld(1,12,AC_MSKB1)//[12:12]
    #define FLD_SRVADC_PU Fld(1,11,AC_MSKB1)//[11:11]
    #define FLD_SRVADC_SR Fld(1,10,AC_MSKB1)//[10:10]
    #define FLD_SRVADC_E8 Fld(1,9,AC_MSKB1)//[9:9]
    #define FLD_SRVADC_E4 Fld(1,8,AC_MSKB1)//[8:8]
    #define FLD_HDMI_3_SCL_SMT Fld(1,5,AC_MSKB0)//[5:5]
    #define FLD_HDMI_3_SCL_PD Fld(1,4,AC_MSKB0)//[4:4]
    #define FLD_HDMI_3_SCL_PU Fld(1,3,AC_MSKB0)//[3:3]
    #define FLD_HDMI_3_SCL_SR Fld(1,2,AC_MSKB0)//[2:2]
    #define FLD_HDMI_3_SCL_E4 Fld(1,1,AC_MSKB0)//[1:1]
    #define FLD_HDMI_3_SCL_E2 Fld(1,0,AC_MSKB0)//[0:0]
#define PDWNC_PADCFG7 (IO_VIRT+IO_PDWNC_BASE + 0x0DC)
    #define FLD_HDMI_1_5V_SMT Fld(1,29,AC_MSKB3)//[29:29]
    #define FLD_HDMI_1_5V_PD Fld(1,28,AC_MSKB3)//[28:28]
    #define FLD_HDMI_1_5V_PU Fld(1,27,AC_MSKB3)//[27:27]
    #define FLD_HDMI_1_5V_SR Fld(1,26,AC_MSKB3)//[26:26]
    #define FLD_HDMI_1_5V_E4 Fld(1,25,AC_MSKB3)//[25:25]
    #define FLD_HDMI_1_5V_E2 Fld(1,24,AC_MSKB3)//[24:24]
    #define FLD_OPCTRL7_SMT Fld(1,21,AC_MSKB2)//[21:21]
    #define FLD_OPCTRL7_PD Fld(1,20,AC_MSKB2)//[20:20]
    #define FLD_OPCTRL7_PU Fld(1,19,AC_MSKB2)//[19:19]
    #define FLD_OPCTRL7_SR Fld(1,18,AC_MSKB2)//[18:18]
    #define FLD_OPCTRL7_E4 Fld(1,17,AC_MSKB2)//[17:17]
    #define FLD_OPCTRL7_E2 Fld(1,16,AC_MSKB2)//[16:16]
    #define FLD_OPCTRL6_SMT Fld(1,13,AC_MSKB1)//[13:13]
    #define FLD_OPCTRL6_PD Fld(1,12,AC_MSKB1)//[12:12]
    #define FLD_OPCTRL6_PU Fld(1,11,AC_MSKB1)//[11:11]
    #define FLD_OPCTRL6_SR Fld(1,10,AC_MSKB1)//[10:10]
    #define FLD_OPCTRL6_E4 Fld(1,9,AC_MSKB1)//[9:9]
    #define FLD_OPCTRL6_E2 Fld(1,8,AC_MSKB1)//[8:8]
    #define FLD_OPCTRL5_SMT Fld(1,5,AC_MSKB0)//[5:5]
    #define FLD_OPCTRL5_PD Fld(1,4,AC_MSKB0)//[4:4]
    #define FLD_OPCTRL5_PU Fld(1,3,AC_MSKB0)//[3:3]
    #define FLD_OPCTRL5_SR Fld(1,2,AC_MSKB0)//[2:2]
    #define FLD_OPCTRL5_E4 Fld(1,1,AC_MSKB0)//[1:1]
    #define FLD_OPCTRL5_E2 Fld(1,0,AC_MSKB0)//[0:0]
#define PDWNC_PADCFG8 (IO_VIRT+IO_PDWNC_BASE + 0x0E0)
    #define FLD_HDMI_3_SDA_SMT Fld(1,29,AC_MSKB3)//[29:29]
    #define FLD_HDMI_3_SDA_PD Fld(1,28,AC_MSKB3)//[28:28]
    #define FLD_HDMI_3_SDA_PU Fld(1,27,AC_MSKB3)//[27:27]
    #define FLD_HDMI_3_SDA_SR Fld(1,26,AC_MSKB3)//[26:26]
    #define FLD_HDMI_3_SDA_E4 Fld(1,25,AC_MSKB3)//[25:25]
    #define FLD_HDMI_3_SDA_E2 Fld(1,24,AC_MSKB3)//[24:24]
    #define FLD_HDMI_3_HPD_SMT Fld(1,21,AC_MSKB2)//[21:21]
    #define FLD_HDMI_3_HPD_PD Fld(1,20,AC_MSKB2)//[20:20]
    #define FLD_HDMI_3_HPD_PU Fld(1,19,AC_MSKB2)//[19:19]
    #define FLD_HDMI_3_HPD_SR Fld(1,18,AC_MSKB2)//[18:18]
    #define FLD_HDMI_3_HPD_E4 Fld(1,17,AC_MSKB2)//[17:17]
    #define FLD_HDMI_3_HPD_E2 Fld(1,16,AC_MSKB2)//[16:16]
    #define FLD_HDMI_3_5V_SMT Fld(1,13,AC_MSKB1)//[13:13]
    #define FLD_HDMI_3_5V_PD Fld(1,12,AC_MSKB1)//[12:12]
    #define FLD_HDMI_3_5V_PU Fld(1,11,AC_MSKB1)//[11:11]
    #define FLD_HDMI_3_5V_SR Fld(1,10,AC_MSKB1)//[10:10]
    #define FLD_HDMI_3_5V_E4 Fld(1,9,AC_MSKB1)//[9:9]
    #define FLD_HDMI_3_5V_E2 Fld(1,8,AC_MSKB1)//[8:8]
    #define FLD_HDMI_2_5V_SMT Fld(1,5,AC_MSKB0)//[5:5]
    #define FLD_HDMI_2_5V_PD Fld(1,4,AC_MSKB0)//[4:4]
    #define FLD_HDMI_2_5V_PU Fld(1,3,AC_MSKB0)//[3:3]
    #define FLD_HDMI_2_5V_SR Fld(1,2,AC_MSKB0)//[2:2]
    #define FLD_HDMI_2_5V_E4 Fld(1,1,AC_MSKB0)//[1:1]
    #define FLD_HDMI_2_5V_E2 Fld(1,0,AC_MSKB0)//[0:0]
#define PDWNC_PADCFG9 (IO_VIRT+IO_PDWNC_BASE + 0x0E4)
    #define FLD_OPCTRL11_SMT Fld(1,29,AC_MSKB3)//[29:29]
    #define FLD_OPCTRL11_PD Fld(1,28,AC_MSKB3)//[28:28]
    #define FLD_OPCTRL11_PU Fld(1,27,AC_MSKB3)//[27:27]
    #define FLD_OPCTRL11_SR Fld(1,26,AC_MSKB3)//[26:26]
    #define FLD_OPCTRL11_E4 Fld(1,25,AC_MSKB3)//[25:25]
    #define FLD_OPCTRL11_E2 Fld(1,24,AC_MSKB3)//[24:24]
    #define FLD_OPCTRL10_SMT Fld(1,21,AC_MSKB2)//[21:21]
    #define FLD_OPCTRL10_PD Fld(1,20,AC_MSKB2)//[20:20]
    #define FLD_OPCTRL10_PU Fld(1,19,AC_MSKB2)//[19:19]
    #define FLD_OPCTRL10_SR Fld(1,18,AC_MSKB2)//[18:18]
    #define FLD_OPCTRL10_E4 Fld(1,17,AC_MSKB2)//[17:17]
    #define FLD_OPCTRL10_E2 Fld(1,16,AC_MSKB2)//[16:16]
    #define FLD_OPCTRL9_SMT Fld(1,13,AC_MSKB1)//[13:13]
    #define FLD_OPCTRL9_PD Fld(1,12,AC_MSKB1)//[12:12]
    #define FLD_OPCTRL9_PU Fld(1,11,AC_MSKB1)//[11:11]
    #define FLD_OPCTRL9_SR Fld(1,10,AC_MSKB1)//[10:10]
    #define FLD_OPCTRL9_E4 Fld(1,9,AC_MSKB1)//[9:9]
    #define FLD_OPCTRL9_E2 Fld(1,8,AC_MSKB1)//[8:8]
    #define FLD_OPCTRL8_SMT Fld(1,5,AC_MSKB0)//[5:5]
    #define FLD_OPCTRL8_PD Fld(1,4,AC_MSKB0)//[4:4]
    #define FLD_OPCTRL8_PU Fld(1,3,AC_MSKB0)//[3:3]
    #define FLD_OPCTRL8_SR Fld(1,2,AC_MSKB0)//[2:2]
    #define FLD_OPCTRL8_E4 Fld(1,1,AC_MSKB0)//[1:1]
    #define FLD_OPCTRL8_E2 Fld(1,0,AC_MSKB0)//[0:0]
#define PDWNC_PADCFG10 (IO_VIRT+IO_PDWNC_BASE + 0x0E8)
    #define FLD_GPIO59_SMT Fld(1,29,AC_MSKB3)//[29:29]
    #define FLD_GPIO59_PD Fld(1,28,AC_MSKB3)//[28:28]
    #define FLD_GPIO59_PU Fld(1,27,AC_MSKB3)//[27:27]
    #define FLD_GPIO59_SR Fld(1,26,AC_MSKB3)//[26:26]
    #define FLD_GPIO59_E4 Fld(1,25,AC_MSKB3)//[25:25]
    #define FLD_GPIO59_E2 Fld(1,24,AC_MSKB3)//[24:24]
    #define FLD_GPIO56_SMT Fld(1,21,AC_MSKB2)//[21:21]
    #define FLD_GPIO56_PD Fld(1,20,AC_MSKB2)//[20:20]
    #define FLD_GPIO56_PU Fld(1,19,AC_MSKB2)//[19:19]
    #define FLD_GPIO56_SR Fld(1,18,AC_MSKB2)//[18:18]
    #define FLD_GPIO56_E4 Fld(1,17,AC_MSKB2)//[17:17]
    #define FLD_GPIO56_E2 Fld(1,16,AC_MSKB2)//[16:16]
    #define FLD_PHYLED1_SMT Fld(1,13,AC_MSKB1)//[13:13]
    #define FLD_PHYLED1_PD Fld(1,12,AC_MSKB1)//[12:12]
    #define FLD_PHYLED1_PU Fld(1,11,AC_MSKB1)//[11:11]
    #define FLD_PHYLED1_SR Fld(1,10,AC_MSKB1)//[10:10]
    #define FLD_PHYLED1_E4 Fld(1,9,AC_MSKB1)//[9:9]
    #define FLD_PHYLED1_E2 Fld(1,8,AC_MSKB1)//[8:8]
    #define FLD_PHYLED0_SMT Fld(1,5,AC_MSKB0)//[5:5]
    #define FLD_PHYLED0_PD Fld(1,4,AC_MSKB0)//[4:4]
    #define FLD_PHYLED0_PU Fld(1,3,AC_MSKB0)//[3:3]
    #define FLD_PHYLED0_SR Fld(1,2,AC_MSKB0)//[2:2]
    #define FLD_PHYLED0_E4 Fld(1,1,AC_MSKB0)//[1:1]
    #define FLD_PHYLED0_E2 Fld(1,0,AC_MSKB0)//[0:0]
#define PDWNC_PADCFG11 (IO_VIRT+IO_PDWNC_BASE + 0x0EC)
    #define FLD_GPIO50_SMT Fld(1,29,AC_MSKB3)//[29:29]
    #define FLD_GPIO50_PD Fld(1,28,AC_MSKB3)//[28:28]
    #define FLD_GPIO50_PU Fld(1,27,AC_MSKB3)//[27:27]
    #define FLD_GPIO50_SR Fld(1,26,AC_MSKB3)//[26:26]
    #define FLD_GPIO50_E4 Fld(1,25,AC_MSKB3)//[25:25]
    #define FLD_GPIO50_E2 Fld(1,24,AC_MSKB3)//[24:24]
    #define FLD_GPIO60_SMT Fld(1,21,AC_MSKB2)//[21:21]
    #define FLD_GPIO60_PD Fld(1,20,AC_MSKB2)//[20:20]
    #define FLD_GPIO60_PU Fld(1,19,AC_MSKB2)//[19:19]
    #define FLD_GPIO60_SR Fld(1,18,AC_MSKB2)//[18:18]
    #define FLD_GPIO60_E4 Fld(1,17,AC_MSKB2)//[17:17]
    #define FLD_GPIO60_E2 Fld(1,16,AC_MSKB2)//[16:16]
    #define FLD_GPIO58_SMT Fld(1,13,AC_MSKB1)//[13:13]
    #define FLD_GPIO58_PD Fld(1,12,AC_MSKB1)//[12:12]
    #define FLD_GPIO58_PU Fld(1,11,AC_MSKB1)//[11:11]
    #define FLD_GPIO58_SR Fld(1,10,AC_MSKB1)//[10:10]
    #define FLD_GPIO58_E4 Fld(1,9,AC_MSKB1)//[9:9]
    #define FLD_GPIO58_E2 Fld(1,8,AC_MSKB1)//[8:8]
    #define FLD_GPIO57_SMT Fld(1,5,AC_MSKB0)//[5:5]
    #define FLD_GPIO57_PD Fld(1,4,AC_MSKB0)//[4:4]
    #define FLD_GPIO57_PU Fld(1,3,AC_MSKB0)//[3:3]
    #define FLD_GPIO57_SR Fld(1,2,AC_MSKB0)//[2:2]
    #define FLD_GPIO57_E4 Fld(1,1,AC_MSKB0)//[1:1]
    #define FLD_GPIO57_E2 Fld(1,0,AC_MSKB0)//[0:0]
#define PDWNC_PADCFG12 (IO_VIRT+IO_PDWNC_BASE + 0x0F0)
    #define FLD_GPIO54_SMT Fld(1,29,AC_MSKB3)//[29:29]
    #define FLD_GPIO54_PD Fld(1,28,AC_MSKB3)//[28:28]
    #define FLD_GPIO54_PU Fld(1,27,AC_MSKB3)//[27:27]
    #define FLD_GPIO54_SR Fld(1,26,AC_MSKB3)//[26:26]
    #define FLD_GPIO54_E4 Fld(1,25,AC_MSKB3)//[25:25]
    #define FLD_GPIO54_E2 Fld(1,24,AC_MSKB3)//[24:24]
    #define FLD_GPIO53_SMT Fld(1,21,AC_MSKB2)//[21:21]
    #define FLD_GPIO53_PD Fld(1,20,AC_MSKB2)//[20:20]
    #define FLD_GPIO53_PU Fld(1,19,AC_MSKB2)//[19:19]
    #define FLD_GPIO53_SR Fld(1,18,AC_MSKB2)//[18:18]
    #define FLD_GPIO53_E4 Fld(1,17,AC_MSKB2)//[17:17]
    #define FLD_GPIO53_E2 Fld(1,16,AC_MSKB2)//[16:16]
    #define FLD_GPIO52_SMT Fld(1,13,AC_MSKB1)//[13:13]
    #define FLD_GPIO52_PD Fld(1,12,AC_MSKB1)//[12:12]
    #define FLD_GPIO52_PU Fld(1,11,AC_MSKB1)//[11:11]
    #define FLD_GPIO52_SR Fld(1,10,AC_MSKB1)//[10:10]
    #define FLD_GPIO52_E4 Fld(1,9,AC_MSKB1)//[9:9]
    #define FLD_GPIO52_E2 Fld(1,8,AC_MSKB1)//[8:8]
    #define FLD_GPIO51_SMT Fld(1,5,AC_MSKB0)//[5:5]
    #define FLD_GPIO51_PD Fld(1,4,AC_MSKB0)//[4:4]
    #define FLD_GPIO51_PU Fld(1,3,AC_MSKB0)//[3:3]
    #define FLD_GPIO51_SR Fld(1,2,AC_MSKB0)//[2:2]
    #define FLD_GPIO51_E4 Fld(1,1,AC_MSKB0)//[1:1]
    #define FLD_GPIO51_E2 Fld(1,0,AC_MSKB0)//[0:0]
#define PDWNC_PADCFG13 (IO_VIRT+IO_PDWNC_BASE + 0x0F4)
    #define FLD_XMHL_SENSE_SMT Fld(1,13,AC_MSKB1)//[13:13]
    #define FLD_XMHL_SENSE_PD Fld(1,12,AC_MSKB1)//[12:12]
    #define FLD_XMHL_SENSE_PU Fld(1,11,AC_MSKB1)//[11:11]
    #define FLD_XMHL_SENSE_SR Fld(1,10,AC_MSKB1)//[10:10]
    #define FLD_XMHL_SENSE_E4 Fld(1,9,AC_MSKB1)//[9:9]
    #define FLD_XMHL_SENSE_E2 Fld(1,8,AC_MSKB1)//[8:8]
    #define FLD_GPIO55_SMT Fld(1,5,AC_MSKB0)//[5:5]
    #define FLD_GPIO55_PD Fld(1,4,AC_MSKB0)//[4:4]
    #define FLD_GPIO55_PU Fld(1,3,AC_MSKB0)//[3:3]
    #define FLD_GPIO55_SR Fld(1,2,AC_MSKB0)//[2:2]
    #define FLD_GPIO55_E4 Fld(1,1,AC_MSKB0)//[1:1]
    #define FLD_GPIO55_E2 Fld(1,0,AC_MSKB0)//[0:0]
#define PDWNC_PADCFG14 (IO_VIRT+IO_PDWNC_BASE + 0x0F8)
    #define FLD_HDMI_0_HPD_CBUS_RESERVE Fld(7,18,AC_MSKW32)//[24:18]
    #define FLD_HDMI_0_HPD_CBUS_VREF_SEL Fld(1,16,AC_MSKB2)//[16:16]
    #define FLD_HDMI_0_HPD_CBUS_IBIAS Fld(2,14,AC_MSKB1)//[15:14]
    #define FLD_HDMI_0_HPD_CBUS_VREF Fld(3,11,AC_MSKB1)//[13:11]
    #define FLD_HDMI_0_HPD_CBUS_SMT Fld(1,6,AC_MSKB0)//[6:6]
    #define FLD_HDMI_0_HPD_CBUS_PUPD Fld(1,5,AC_MSKB0)//[5:5]
    #define FLD_HDMI_0_HPD_CBUS_IES Fld(1,4,AC_MSKB0)//[4:4]
    #define FLD_HDMI_0_HPD_CBUS_PU Fld(1,3,AC_MSKB0)//[3:3]
    #define FLD_HDMI_0_HPD_CBUS_SR Fld(1,2,AC_MSKB0)//[2:2]
    #define FLD_HDMI_0_HPD_CBUS_E8 Fld(1,1,AC_MSKB0)//[1:1]
    #define FLD_HDMI_0_HPD_CBUS_E4 Fld(1,0,AC_MSKB0)//[0:0]
#else //CC_MT5399
	#define PDWNC_PADCFG0 (IO_VIRT+IO_PDWNC_BASE + 0x0C0)
    #define FLD_HDMI_0_SCL_SMT Fld(1,29,AC_MSKB3)//[29:29]
    #define FLD_HDMI_0_SCL_PD Fld(1,28,AC_MSKB3)//[28:28]
    #define FLD_HDMI_0_SCL_PU Fld(1,27,AC_MSKB3)//[27:27]
    #define FLD_HDMI_0_SCL_SR Fld(1,26,AC_MSKB3)//[26:26]
    #define FLD_HDMI_0_SCL_E4 Fld(1,25,AC_MSKB3)//[25:25]
    #define FLD_HDMI_0_SCL_E2 Fld(1,24,AC_MSKB3)//[24:24]
    #define FLD_HDMI_0_SDA_SMT Fld(1,21,AC_MSKB2)//[21:21]
    #define FLD_HDMI_0_SDA_PD Fld(1,20,AC_MSKB2)//[20:20]
    #define FLD_HDMI_0_SDA_PU Fld(1,19,AC_MSKB2)//[19:19]
    #define FLD_HDMI_0_SDA_SR Fld(1,18,AC_MSKB2)//[18:18]
    #define FLD_HDMI_0_SDA_E4 Fld(1,17,AC_MSKB2)//[17:17]
    #define FLD_HDMI_0_SDA_E2 Fld(1,16,AC_MSKB2)//[16:16]
    #define FLD_HDMI_0_5V_SMT Fld(1,13,AC_MSKB1)//[13:13]
    #define FLD_HDMI_0_5V_PD Fld(1,12,AC_MSKB1)//[12:12]
    #define FLD_HDMI_0_5V_PU Fld(1,11,AC_MSKB1)//[11:11]
    #define FLD_HDMI_0_5V_SR Fld(1,10,AC_MSKB1)//[10:10]
    #define FLD_HDMI_0_5V_E4 Fld(1,9,AC_MSKB1)//[9:9]
    #define FLD_HDMI_0_5V_E2 Fld(1,8,AC_MSKB1)//[8:8]
    #define FLD_HDMI_CEC_SMT Fld(1,5,AC_MSKB0)//[5:5]
    #define FLD_HDMI_CEC_PD Fld(1,4,AC_MSKB0)//[4:4]
    #define FLD_HDMI_CEC_PU Fld(1,3,AC_MSKB0)//[3:3]
    #define FLD_HDMI_CEC_SR Fld(1,2,AC_MSKB0)//[2:2]
    #define FLD_HDMI_CEC_E4 Fld(1,1,AC_MSKB0)//[1:1]
    #define FLD_HDMI_CEC_E2 Fld(1,0,AC_MSKB0)//[0:0]
#define PDWNC_PADCFG1 (IO_VIRT+IO_PDWNC_BASE + 0x0C4)
    #define FLD_HDMI_2_SCL_SMT Fld(1,29,AC_MSKB3)//[29:29]
    #define FLD_HDMI_2_SCL_PD Fld(1,28,AC_MSKB3)//[28:28]
    #define FLD_HDMI_2_SCL_PU Fld(1,27,AC_MSKB3)//[27:27]
    #define FLD_HDMI_2_SCL_SR Fld(1,26,AC_MSKB3)//[26:26]
    #define FLD_HDMI_2_SCL_E4 Fld(1,25,AC_MSKB3)//[25:25]
    #define FLD_HDMI_2_SCL_E2 Fld(1,24,AC_MSKB3)//[24:24]
    #define FLD_HDMI_2_SDA_SMT Fld(1,21,AC_MSKB2)//[21:21]
    #define FLD_HDMI_2_SDA_PD Fld(1,20,AC_MSKB2)//[20:20]
    #define FLD_HDMI_2_SDA_PU Fld(1,19,AC_MSKB2)//[19:19]
    #define FLD_HDMI_2_SDA_SR Fld(1,18,AC_MSKB2)//[18:18]
    #define FLD_HDMI_2_SDA_E4 Fld(1,17,AC_MSKB2)//[17:17]
    #define FLD_HDMI_2_SDA_E2 Fld(1,16,AC_MSKB2)//[16:16]
    #define FLD_HDMI_1_SCL_SMT Fld(1,13,AC_MSKB1)//[13:13]
    #define FLD_HDMI_1_SCL_PD Fld(1,12,AC_MSKB1)//[12:12]
    #define FLD_HDMI_1_SCL_PU Fld(1,11,AC_MSKB1)//[11:11]
    #define FLD_HDMI_1_SCL_SR Fld(1,10,AC_MSKB1)//[10:10]
    #define FLD_HDMI_1_SCL_E4 Fld(1,9,AC_MSKB1)//[9:9]
    #define FLD_HDMI_1_SCL_E2 Fld(1,8,AC_MSKB1)//[8:8]
    #define FLD_HDMI_1_SDA_SMT Fld(1,5,AC_MSKB0)//[5:5]
    #define FLD_HDMI_1_SDA_PD Fld(1,4,AC_MSKB0)//[4:4]
    #define FLD_HDMI_1_SDA_PU Fld(1,3,AC_MSKB0)//[3:3]
    #define FLD_HDMI_1_SDA_SR Fld(1,2,AC_MSKB0)//[2:2]
    #define FLD_HDMI_1_SDA_E4 Fld(1,1,AC_MSKB0)//[1:1]
    #define FLD_HDMI_1_SDA_E2 Fld(1,0,AC_MSKB0)//[0:0]
#define PDWNC_PADCFG2 (IO_VIRT+IO_PDWNC_BASE + 0x0C8)
    #define FLD_XMHL_SENSE_SMT Fld(1,29,AC_MSKB3)//[29:29]
    #define FLD_XMHL_SENSE_PD Fld(1,28,AC_MSKB3)//[28:28]
    #define FLD_XMHL_SENSE_PU Fld(1,27,AC_MSKB3)//[27:27]
    #define FLD_XMHL_SENSE_SR Fld(1,26,AC_MSKB3)//[26:26]
    #define FLD_XMHL_SENSE_E4 Fld(1,25,AC_MSKB3)//[25:25]
    #define FLD_XMHL_SENSE_E2 Fld(1,24,AC_MSKB3)//[24:24]
    #define FLD_HDMI_2_HPD_SMT Fld(1,21,AC_MSKB2)//[21:21]
    #define FLD_HDMI_2_HPD_PD Fld(1,20,AC_MSKB2)//[20:20]
    #define FLD_HDMI_2_HPD_PU Fld(1,19,AC_MSKB2)//[19:19]
    #define FLD_HDMI_2_HPD_SR Fld(1,18,AC_MSKB2)//[18:18]
    #define FLD_HDMI_2_HPD_E4 Fld(1,17,AC_MSKB2)//[17:17]
    #define FLD_HDMI_2_HPD_E2 Fld(1,16,AC_MSKB2)//[16:16]
    #define FLD_HDMI_1_HPD_SMT Fld(1,13,AC_MSKB1)//[13:13]
    #define FLD_HDMI_1_HPD_PD Fld(1,12,AC_MSKB1)//[12:12]
    #define FLD_HDMI_1_HPD_PU Fld(1,11,AC_MSKB1)//[11:11]
    #define FLD_HDMI_1_HPD_SR Fld(1,10,AC_MSKB1)//[10:10]
    #define FLD_HDMI_1_HPD_E4 Fld(1,9,AC_MSKB1)//[9:9]
    #define FLD_HDMI_1_HPD_E2 Fld(1,8,AC_MSKB1)//[8:8]
#define PDWNC_PADCFG3 (IO_VIRT+IO_PDWNC_BASE + 0x0CC)
    #define FLD_U0TX_SMT Fld(1,29,AC_MSKB3)//[29:29]
    #define FLD_U0TX_PD Fld(1,28,AC_MSKB3)//[28:28]
    #define FLD_U0TX_PU Fld(1,27,AC_MSKB3)//[27:27]
    #define FLD_U0TX_SR Fld(1,26,AC_MSKB3)//[26:26]
    #define FLD_U0TX_E4 Fld(1,25,AC_MSKB3)//[25:25]
    #define FLD_U0TX_E2 Fld(1,24,AC_MSKB3)//[24:24]
    #define FLD_VGA_SCL_SMT Fld(1,21,AC_MSKB2)//[21:21]
    #define FLD_VGA_SCL_PD Fld(1,20,AC_MSKB2)//[20:20]
    #define FLD_VGA_SCL_PU Fld(1,19,AC_MSKB2)//[19:19]
    #define FLD_VGA_SCL_SR Fld(1,18,AC_MSKB2)//[18:18]
    #define FLD_VGA_SCL_E4 Fld(1,17,AC_MSKB2)//[17:17]
    #define FLD_VGA_SCL_E2 Fld(1,16,AC_MSKB2)//[16:16]
    #define FLD_VGA_SDA_SMT Fld(1,13,AC_MSKB1)//[13:13]
    #define FLD_VGA_SDA_PD Fld(1,12,AC_MSKB1)//[12:12]
    #define FLD_VGA_SDA_PU Fld(1,11,AC_MSKB1)//[11:11]
    #define FLD_VGA_SDA_SR Fld(1,10,AC_MSKB1)//[10:10]
    #define FLD_VGA_SDA_E4 Fld(1,9,AC_MSKB1)//[9:9]
    #define FLD_VGA_SDA_E2 Fld(1,8,AC_MSKB1)//[8:8]
    #define FLD_OIRI_SMT Fld(1,5,AC_MSKB0)//[5:5]
    #define FLD_OIRI_PD Fld(1,4,AC_MSKB0)//[4:4]
    #define FLD_OIRI_PU Fld(1,3,AC_MSKB0)//[3:3]
    #define FLD_OIRI_SR Fld(1,2,AC_MSKB0)//[2:2]
    #define FLD_OIRI_E4 Fld(1,1,AC_MSKB0)//[1:1]
    #define FLD_OIRI_E2 Fld(1,0,AC_MSKB0)//[0:0]

//Page VDOIN_SYS_2
#define PDWNC_PADCFG4 (IO_VIRT+IO_PDWNC_BASE + 0x0D0)
    #define FLD_OPCTRL0_SMT Fld(1,29,AC_MSKB3)//[29:29]
    #define FLD_OPCTRL0_PD Fld(1,28,AC_MSKB3)//[28:28]
    #define FLD_OPCTRL0_PU Fld(1,27,AC_MSKB3)//[27:27]
    #define FLD_OPCTRL0_SR Fld(1,26,AC_MSKB3)//[26:26]
    #define FLD_OPCTRL0_E4 Fld(1,25,AC_MSKB3)//[25:25]
    #define FLD_OPCTRL0_E2 Fld(1,24,AC_MSKB3)//[24:24]
    #define FLD_LED_PWM1_SMT Fld(1,21,AC_MSKB2)//[21:21]
    #define FLD_LED_PWM1_PD Fld(1,20,AC_MSKB2)//[20:20]
    #define FLD_LED_PWM1_PU Fld(1,19,AC_MSKB2)//[19:19]
    #define FLD_LED_PWM1_SR Fld(1,18,AC_MSKB2)//[18:18]
    #define FLD_LED_PWM1_E4 Fld(1,17,AC_MSKB2)//[17:17]
    #define FLD_LED_PWM1_E2 Fld(1,16,AC_MSKB2)//[16:16]
    #define FLD_LED_PWM0_SMT Fld(1,13,AC_MSKB1)//[13:13]
    #define FLD_LED_PWM0_PD Fld(1,12,AC_MSKB1)//[12:12]
    #define FLD_LED_PWM0_PU Fld(1,11,AC_MSKB1)//[11:11]
    #define FLD_LED_PWM0_SR Fld(1,10,AC_MSKB1)//[10:10]
    #define FLD_LED_PWM0_E4 Fld(1,9,AC_MSKB1)//[9:9]
    #define FLD_LED_PWM0_E2 Fld(1,8,AC_MSKB1)//[8:8]
    #define FLD_U0RX_SMT Fld(1,5,AC_MSKB0)//[5:5]
    #define FLD_U0RX_PD Fld(1,4,AC_MSKB0)//[4:4]
    #define FLD_U0RX_PU Fld(1,3,AC_MSKB0)//[3:3]
    #define FLD_U0RX_SR Fld(1,2,AC_MSKB0)//[2:2]
    #define FLD_U0RX_E4 Fld(1,1,AC_MSKB0)//[1:1]
    #define FLD_U0RX_E2 Fld(1,0,AC_MSKB0)//[0:0]
#define PDWNC_PADCFG5 (IO_VIRT+IO_PDWNC_BASE + 0x0D4)
    #define FLD_OPCTRL4_SMT Fld(1,29,AC_MSKB3)//[29:29]
    #define FLD_OPCTRL4_PD Fld(1,28,AC_MSKB3)//[28:28]
    #define FLD_OPCTRL4_PU Fld(1,27,AC_MSKB3)//[27:27]
    #define FLD_OPCTRL4_SR Fld(1,26,AC_MSKB3)//[26:26]
    #define FLD_OPCTRL4_E4 Fld(1,25,AC_MSKB3)//[25:25]
    #define FLD_OPCTRL4_E2 Fld(1,24,AC_MSKB3)//[24:24]
    #define FLD_OPCTRL3_SMT Fld(1,21,AC_MSKB2)//[21:21]
    #define FLD_OPCTRL3_PD Fld(1,20,AC_MSKB2)//[20:20]
    #define FLD_OPCTRL3_PU Fld(1,19,AC_MSKB2)//[19:19]
    #define FLD_OPCTRL3_SR Fld(1,18,AC_MSKB2)//[18:18]
    #define FLD_OPCTRL3_E4 Fld(1,17,AC_MSKB2)//[17:17]
    #define FLD_OPCTRL3_E2 Fld(1,16,AC_MSKB2)//[16:16]
    #define FLD_OPCTRL2_SMT Fld(1,13,AC_MSKB1)//[13:13]
    #define FLD_OPCTRL2_PD Fld(1,12,AC_MSKB1)//[12:12]
    #define FLD_OPCTRL2_PU Fld(1,11,AC_MSKB1)//[11:11]
    #define FLD_OPCTRL2_SR Fld(1,10,AC_MSKB1)//[10:10]
    #define FLD_OPCTRL2_E4 Fld(1,9,AC_MSKB1)//[9:9]
    #define FLD_OPCTRL2_E2 Fld(1,8,AC_MSKB1)//[8:8]
    #define FLD_OPCTRL1_SMT Fld(1,5,AC_MSKB0)//[5:5]
    #define FLD_OPCTRL1_PD Fld(1,4,AC_MSKB0)//[4:4]
    #define FLD_OPCTRL1_PU Fld(1,3,AC_MSKB0)//[3:3]
    #define FLD_OPCTRL1_SR Fld(1,2,AC_MSKB0)//[2:2]
    #define FLD_OPCTRL1_E4 Fld(1,1,AC_MSKB0)//[1:1]
    #define FLD_OPCTRL1_E2 Fld(1,0,AC_MSKB0)//[0:0]
#define PDWNC_PADCFG6 (IO_VIRT+IO_PDWNC_BASE + 0x0D8)
    #define FLD_OPCTRL8_SMT Fld(1,29,AC_MSKB3)//[29:29]
    #define FLD_OPCTRL8_PD Fld(1,28,AC_MSKB3)//[28:28]
    #define FLD_OPCTRL8_PU Fld(1,27,AC_MSKB3)//[27:27]
    #define FLD_OPCTRL8_SR Fld(1,26,AC_MSKB3)//[26:26]
    #define FLD_OPCTRL8_E4 Fld(1,25,AC_MSKB3)//[25:25]
    #define FLD_OPCTRL8_E2 Fld(1,24,AC_MSKB3)//[24:24]
    #define FLD_OPCTRL7_SMT Fld(1,21,AC_MSKB2)//[21:21]
    #define FLD_OPCTRL7_PD Fld(1,20,AC_MSKB2)//[20:20]
    #define FLD_OPCTRL7_PU Fld(1,19,AC_MSKB2)//[19:19]
    #define FLD_OPCTRL7_SR Fld(1,18,AC_MSKB2)//[18:18]
    #define FLD_OPCTRL7_E4 Fld(1,17,AC_MSKB2)//[17:17]
    #define FLD_OPCTRL7_E2 Fld(1,16,AC_MSKB2)//[16:16]
    #define FLD_OPCTRL6_SMT Fld(1,13,AC_MSKB1)//[13:13]
    #define FLD_OPCTRL6_PD Fld(1,12,AC_MSKB1)//[12:12]
    #define FLD_OPCTRL6_PU Fld(1,11,AC_MSKB1)//[11:11]
    #define FLD_OPCTRL6_SR Fld(1,10,AC_MSKB1)//[10:10]
    #define FLD_OPCTRL6_E4 Fld(1,9,AC_MSKB1)//[9:9]
    #define FLD_OPCTRL6_E2 Fld(1,8,AC_MSKB1)//[8:8]
    #define FLD_OPCTRL5_SMT Fld(1,5,AC_MSKB0)//[5:5]
    #define FLD_OPCTRL5_PD Fld(1,4,AC_MSKB0)//[4:4]
    #define FLD_OPCTRL5_PU Fld(1,3,AC_MSKB0)//[3:3]
    #define FLD_OPCTRL5_SR Fld(1,2,AC_MSKB0)//[2:2]
    #define FLD_OPCTRL5_E4 Fld(1,1,AC_MSKB0)//[1:1]
    #define FLD_OPCTRL5_E2 Fld(1,0,AC_MSKB0)//[0:0]
#define PDWNC_PADCFG7 (IO_VIRT+IO_PDWNC_BASE + 0x0DC)
    #define FLD_OPCTRL12_SMT Fld(1,29,AC_MSKB3)//[29:29]
    #define FLD_OPCTRL12_PD Fld(1,28,AC_MSKB3)//[28:28]
    #define FLD_OPCTRL12_PU Fld(1,27,AC_MSKB3)//[27:27]
    #define FLD_OPCTRL12_SR Fld(1,26,AC_MSKB3)//[26:26]
    #define FLD_OPCTRL12_E4 Fld(1,25,AC_MSKB3)//[25:25]
    #define FLD_OPCTRL12_E2 Fld(1,24,AC_MSKB3)//[24:24]
    #define FLD_OPCTRL11_SMT Fld(1,21,AC_MSKB2)//[21:21]
    #define FLD_OPCTRL11_PD Fld(1,20,AC_MSKB2)//[20:20]
    #define FLD_OPCTRL11_PU Fld(1,19,AC_MSKB2)//[19:19]
    #define FLD_OPCTRL11_SR Fld(1,18,AC_MSKB2)//[18:18]
    #define FLD_OPCTRL11_E4 Fld(1,17,AC_MSKB2)//[17:17]
    #define FLD_OPCTRL11_E2 Fld(1,16,AC_MSKB2)//[16:16]
    #define FLD_OPCTRL10_SMT Fld(1,13,AC_MSKB1)//[13:13]
    #define FLD_OPCTRL10_PD Fld(1,12,AC_MSKB1)//[12:12]
    #define FLD_OPCTRL10_PU Fld(1,11,AC_MSKB1)//[11:11]
    #define FLD_OPCTRL10_SR Fld(1,10,AC_MSKB1)//[10:10]
    #define FLD_OPCTRL10_E4 Fld(1,9,AC_MSKB1)//[9:9]
    #define FLD_OPCTRL10_E2 Fld(1,8,AC_MSKB1)//[8:8]
    #define FLD_OPCTRL9_SMT Fld(1,5,AC_MSKB0)//[5:5]
    #define FLD_OPCTRL9_PD Fld(1,4,AC_MSKB0)//[4:4]
    #define FLD_OPCTRL9_PU Fld(1,3,AC_MSKB0)//[3:3]
    #define FLD_OPCTRL9_SR Fld(1,2,AC_MSKB0)//[2:2]
    #define FLD_OPCTRL9_E4 Fld(1,1,AC_MSKB0)//[1:1]
    #define FLD_OPCTRL9_E2 Fld(1,0,AC_MSKB0)//[0:0]
#define PDWNC_PADCFG8 (IO_VIRT+IO_PDWNC_BASE + 0x0E0)
    #define FLD_SRVADC_SMT Fld(1,29,AC_MSKB3)//[29:29]
    #define FLD_SRVADC_PD Fld(1,28,AC_MSKB3)//[28:28]
    #define FLD_SRVADC_PU Fld(1,27,AC_MSKB3)//[27:27]
    #define FLD_SRVADC_SR Fld(1,26,AC_MSKB3)//[26:26]
    #define FLD_SRVADC_E4 Fld(1,25,AC_MSKB3)//[25:25]
    #define FLD_SRVADC_E2 Fld(1,24,AC_MSKB3)//[24:24]
    #define FLD_PHYLED1_SMT Fld(1,21,AC_MSKB2)//[21:21]
    #define FLD_PHYLED1_PD Fld(1,20,AC_MSKB2)//[20:20]
    #define FLD_PHYLED1_PU Fld(1,19,AC_MSKB2)//[19:19]
    #define FLD_PHYLED1_SR Fld(1,18,AC_MSKB2)//[18:18]
    #define FLD_PHYLED1_E4 Fld(1,17,AC_MSKB2)//[17:17]
    #define FLD_PHYLED1_E2 Fld(1,16,AC_MSKB2)//[16:16]
    #define FLD_PHYLED0_SMT Fld(1,13,AC_MSKB1)//[13:13]
    #define FLD_PHYLED0_PD Fld(1,12,AC_MSKB1)//[12:12]
    #define FLD_PHYLED0_PU Fld(1,11,AC_MSKB1)//[11:11]
    #define FLD_PHYLED0_SR Fld(1,10,AC_MSKB1)//[10:10]
    #define FLD_PHYLED0_E4 Fld(1,9,AC_MSKB1)//[9:9]
    #define FLD_PHYLED0_E2 Fld(1,8,AC_MSKB1)//[8:8]
    #define FLD_OPWRSB_SMT Fld(1,5,AC_MSKB0)//[5:5]
    #define FLD_OPWRSB_PD Fld(1,4,AC_MSKB0)//[4:4]
    #define FLD_OPWRSB_PU Fld(1,3,AC_MSKB0)//[3:3]
    #define FLD_OPWRSB_SR Fld(1,2,AC_MSKB0)//[2:2]
    #define FLD_OPWRSB_E4 Fld(1,1,AC_MSKB0)//[1:1]
    #define FLD_OPWRSB_E2 Fld(1,0,AC_MSKB0)//[0:0]
#define PDWNC_PADCFG14 (IO_VIRT+IO_PDWNC_BASE + 0x0F8)
    #define FLD_HDMI_0_HPD_CBUS_RESERVE Fld(7,18,AC_MSKW32)//[24:18]
    #define FLD_HDMI_0_HPD_CBUS_VREF_SEL Fld(1,16,AC_MSKB2)//[16:16]
    #define FLD_HDMI_0_HPD_CBUS_IBIAS Fld(2,14,AC_MSKB1)//[15:14]
    #define FLD_HDMI_0_HPD_CBUS_VREF Fld(3,11,AC_MSKB1)//[13:11]
    #define FLD_HDMI_0_HPD_CBUS_SMT Fld(1,6,AC_MSKB0)//[6:6]
    #define FLD_HDMI_0_HPD_CBUS_PUPD Fld(1,5,AC_MSKB0)//[5:5]
    #define FLD_HDMI_0_HPD_CBUS_IES Fld(1,4,AC_MSKB0)//[4:4]
    #define FLD_HDMI_0_HPD_CBUS_PU Fld(1,3,AC_MSKB0)//[3:3]
    #define FLD_HDMI_0_HPD_CBUS_SR Fld(1,2,AC_MSKB0)//[2:2]
    #define FLD_HDMI_0_HPD_CBUS_E8 Fld(1,1,AC_MSKB0)//[1:1]
    #define FLD_HDMI_0_HPD_CBUS_E4 Fld(1,0,AC_MSKB0)//[0:0]
#endif //CC_MT5399
#define PDWNC_WDTCTL (IO_VIRT+IO_PDWNC_BASE + 0x100)
    #define FLD_WDT2RST Fld(1,26,AC_MSKB3)//[26:26]
    #define FLD_WDT1RST Fld(1,25,AC_MSKB3)//[25:25]
    #define FLD_WDT0RST Fld(1,24,AC_MSKB3)//[24:24]
    #define FLD_WDT2E Fld(8,16,AC_FULLB2)//[23:16]
    #define FLD_WDT1E Fld(8,8,AC_FULLB1)//[15:8]
    #define FLD_WDT0E Fld(8,0,AC_FULLB0)//[7:0]
#define PDWNC_WDT0 (IO_VIRT+IO_PDWNC_BASE + 0x104)
    #define FLD_PWR_RDY Fld(1,31,AC_MSKB3)//[31:31]
    #define FLD_WDT0 Fld(31,0,AC_MSKDW)//[30:0]
#define PDWNC_WKRSC (IO_VIRT+IO_PDWNC_BASE + 0x108)
    #define FLD_WKRSC Fld(32,0,AC_FULLDW)//[31:0]
#define PDWNC_WDT1 (IO_VIRT+IO_PDWNC_BASE + 0x10C)
    #define FLD_PDWNC_STATUS_0 Fld(1,31,AC_MSKB3)//[31:31]
    #define FLD_WDT1 Fld(31,0,AC_MSKDW)//[30:0]
#define PDWNC_WDT2 (IO_VIRT+IO_PDWNC_BASE + 0x110)
    #define FLD_PDWNC_STATUS_1 Fld(1,31,AC_MSKB3)//[31:31]
    #define FLD_WDT2 Fld(31,0,AC_MSKDW)//[30:0]
#define PDWNC_WDTRST0 (IO_VIRT+IO_PDWNC_BASE + 0x114)
    #define FLD_WDTRST0 Fld(28,0,AC_MSKDW)//[27:0]
#define PDWNC_WDTRST1 (IO_VIRT+IO_PDWNC_BASE + 0x118)
    #define FLD_WDTRST1 Fld(28,0,AC_MSKDW)//[27:0]
#define PDWNC_WDTRST2 (IO_VIRT+IO_PDWNC_BASE + 0x11C)
    #define FLD_WDTRST2 Fld(28,0,AC_MSKDW)//[27:0]
#define PDWNC_WAKEN (IO_VIRT+IO_PDWNC_BASE + 0x120)
    #define FLD_SRVAD_WAKEN Fld(8,20,AC_MSKW32)//[27:20]
    #define FLD_UNOR_WAKEN Fld(1,19,AC_MSKB2)//[19:19]
    #define FLD_T8032_WAKEN Fld(1,17,AC_MSKB2)//[17:17]
    #define FLD_RTC_WAKEN Fld(1,15,AC_MSKB1)//[15:15]
    #if defined(CC_MT5882)
	#define FLD_EXTIO_WAKEN_12_8 Fld(5,10,AC_MSKB1)//[14:10]
	#else
    #define FLD_EXTIO_WAKEN_11_8 Fld(4,10,AC_MSKB1)//[13:10]
    #endif
    #define FLD_VGA_WAKEN Fld(1,9,AC_MSKB1)//[9:9]
    #define FLD_IR_WAKEN Fld(1,8,AC_MSKB1)//[8:8]
    #define FLD_EXTIO_WAKEN Fld(8,0,AC_FULLB0)//[7:0]
#define PDWNC_PDCODE (IO_VIRT+IO_PDWNC_BASE + 0x124)
    #define FLD_PDCODE Fld(8,0,AC_FULLB0)//[7:0]
#define PDWNC_PDSTAT (IO_VIRT+IO_PDWNC_BASE + 0x128)
    #define FLD_SRVAD_WAK Fld(8,20,AC_MSKW32)//[27:20]
    #define FLD_UNOR_WAK Fld(1,19,AC_MSKB2)//[19:19]
    #define FLD_T8032_WAK Fld(1,17,AC_MSKB2)//[17:17]
    #define FLD_UART_WAK Fld(1,16,AC_MSKB2)//[16:16]
    #define FLD_RTC_WAK Fld(1,15,AC_MSKB1)//[15:15]
    #if defined(CC_MT5882)
	#define FLD_EXTIO_WAK_12_8 Fld(5,10,AC_MSKB1)//[14:10]
	#else
    #define FLD_EXTIO_WAK_11_8 Fld(4,10,AC_MSKB1)//[13:10]
    #endif
    #define FLD_VGA_WAK Fld(1,9,AC_MSKB1)//[9:9]
    #define FLD_IR_WAK Fld(1,8,AC_MSKB1)//[8:8]
    #define FLD_EXTIO_WAK Fld(8,0,AC_FULLB0)//[7:0]
#define PDWNC_PDSTCLR (IO_VIRT+IO_PDWNC_BASE + 0x12C)
    #define FLD_STCLR Fld(1,0,AC_MSKB0)//[0:0]
#define PDWNC_UPWAK (IO_VIRT+IO_PDWNC_BASE + 0x130)
    #define FLD_WAKCODE Fld(8,0,AC_FULLB0)//[7:0]
#define PDWNC_OPDELAY (IO_VIRT+IO_PDWNC_BASE + 0x134)
    #define FLD_WDT_RST_CLR_EN Fld(1,31,AC_MSKB3)//[31:31]
    #define FLD_OPWRSB_DELAY Fld(28,0,AC_MSKDW)//[27:0]
#define PDWNC_PD_PWM0C0 (IO_VIRT+IO_PDWNC_BASE + 0x138)
    #define FLD_PWM0RSN Fld(8,24,AC_FULLB3)//[31:24]
    #define FLD_PWM0H Fld(8,16,AC_FULLB2)//[23:16]
    #define FLD_PWM0CYC Fld(8,8,AC_FULLB1)//[15:8]
    #define FLD_PWM0AUTO Fld(1,5,AC_MSKB0)//[5:5]
    #define FLD_PWM0FLAG Fld(1,4,AC_MSKB0)//[4:4]
    #define FLD_PWM0E Fld(1,0,AC_MSKB0)//[0:0]
#define PDWNC_PD_PWM0C1 (IO_VIRT+IO_PDWNC_BASE + 0x13C)
    #define FLD_PWM0P Fld(25,0,AC_MSKDW)//[24:0]
#define PDWNC_RESRV0 (IO_VIRT+IO_PDWNC_BASE + 0x140)
    #define FLD_RESRV0 Fld(32,0,AC_FULLDW)//[31:0]
#define PDWNC_RESRV1 (IO_VIRT+IO_PDWNC_BASE + 0x144)
    #define FLD_RESRV1 Fld(32,0,AC_FULLDW)//[31:0]
#define PDWNC_RESRV2 (IO_VIRT+IO_PDWNC_BASE + 0x148)
    #define FLD_RESRV2 Fld(32,0,AC_FULLDW)//[31:0]
#define PDWNC_RESRV3 (IO_VIRT+IO_PDWNC_BASE + 0x14C)
    #define FLD_RESRV3 Fld(32,0,AC_FULLDW)//[31:0]
#define PDWNC_AC_TIMER (IO_VIRT+IO_PDWNC_BASE + 0x154)
    #define FLD_TIME Fld(20,0,AC_MSKDW)//[19:0]
#define PDWNC_PD_PWM1C0 (IO_VIRT+IO_PDWNC_BASE + 0x158)
    #define FLD_PWM1RSN Fld(8,24,AC_FULLB3)//[31:24]
    #define FLD_PWM1H Fld(8,16,AC_FULLB2)//[23:16]
    #define FLD_PWM1CYC Fld(8,8,AC_FULLB1)//[15:8]
    #define FLD_PWM1AUTO Fld(1,5,AC_MSKB0)//[5:5]
    #define FLD_PWM1FLAG Fld(1,4,AC_MSKB0)//[4:4]
    #define FLD_PWM1E Fld(1,0,AC_MSKB0)//[0:0]
#define PDWNC_PD_PWM1C1 (IO_VIRT+IO_PDWNC_BASE + 0x15C)
    #define FLD_PWM1P Fld(25,0,AC_MSKDW)//[24:0]
#define PDWNC_SM1CTL0 (IO_VIRT+IO_PDWNC_BASE + 0x160)
    #define FLD_SIFM1_ODRAIN Fld(1,31,AC_MSKB3)//[31:31]
    #if defined(CC_MT5882)
	#define FLD_SIFM1_VSYNC_MODE Fld(2,28,AC_MSKB3)//[29:28]
	#endif
    #define FLD_SIFM1_CLK_DIV Fld(12,16,AC_MSKW32)//[27:16]
    #define FLD_SIFM1_DE_CNT Fld(8,8,AC_FULLB1)//[15:8]
    #define FLD_SIFM1_WAIT_LEVEL Fld(1,6,AC_MSKB0)//[6:6]
    #define FLD_SIFM1_DEGLITCH_EN Fld(1,5,AC_MSKB0)//[5:5]
    #define FLD_SIFM1_CS_STATUS Fld(1,4,AC_MSKB0)//[4:4]
    #define FLD_SIFM1_SCL_STATE Fld(1,3,AC_MSKB0)//[3:3]
    #define FLD_SIFM1_SDA_STATE Fld(1,2,AC_MSKB0)//[2:2]
    #if defined(CC_MT5882)
	#define FLD_SIFM1_EN Fld(1,1,AC_MSKB0)//[1:1]
	#endif
    #define FLD_SIFM1_SCL_STRECH Fld(1,0,AC_MSKB0)//[0:0]
#define PDWNC_SM1CTL1 (IO_VIRT+IO_PDWNC_BASE + 0x164)
	#if defined(CC_MT5882)
	#define FLD_SIFM1_ACK Fld(16,16,AC_FULLW32)//[31:16]
	#define FLD_SIFM1_PGLEN Fld(6,8,AC_MSKB1)//[13:8]
	#else
    #define FLD_SIFM1_ACK Fld(8,16,AC_FULLB2)//[23:16]
    #define FLD_SIFM1_PGLEN Fld(3,8,AC_MSKB1)//[10:8]
    #endif
    #define FLD_SIFM1_MODE Fld(3,4,AC_MSKB0)//[6:4]
    #define FLD_SIFM1_TRI Fld(1,0,AC_MSKB0)//[0:0]
#define PDWNC_SM1D0 (IO_VIRT+IO_PDWNC_BASE + 0x168)
    #define FLD_SIFM1_DATA3 Fld(8,24,AC_FULLB3)//[31:24]
    #define FLD_SIFM1_DATA2 Fld(8,16,AC_FULLB2)//[23:16]
    #define FLD_SIFM1_DATA1 Fld(8,8,AC_FULLB1)//[15:8]
    #define FLD_SIFM1_DATA0 Fld(8,0,AC_FULLB0)//[7:0]
#define PDWNC_SM1D1 (IO_VIRT+IO_PDWNC_BASE + 0x16C)
    #define FLD_SIFM1_DATA7 Fld(8,24,AC_FULLB3)//[31:24]
    #define FLD_SIFM1_DATA6 Fld(8,16,AC_FULLB2)//[23:16]
    #define FLD_SIFM1_DATA5 Fld(8,8,AC_FULLB1)//[15:8]
    #define FLD_SIFM1_DATA4 Fld(8,0,AC_FULLB0)//[7:0]
#define PDWNC_UP_ADDR (IO_VIRT+IO_PDWNC_BASE + 0x170)
    #define FLD_UP_ADDR Fld(16,0,AC_FULLW10)//[15:0]
#define PDWNC_UP_DATA (IO_VIRT+IO_PDWNC_BASE + 0x174)
    #define FLD_UP_DATA Fld(32,0,AC_FULLDW)//[31:0]
#define PDWNC_UP_CFG (IO_VIRT+IO_PDWNC_BASE + 0x178)
    #define FLD_RAM_SPL_SEL Fld(2,6,AC_MSKB0)//[7:6]
    #define FLD_RAM_SP_SEL Fld(2,4,AC_MSKB0)//[5:4]
    #define FLD_BLOCK_GPIO_ACC Fld(1,3,AC_MSKB0)//[3:3]
    #define FLD_XDATA_ACC Fld(1,2,AC_MSKB0)//[2:2]
    #define FLD_VIR_EN Fld(1,1,AC_MSKB0)//[1:1]
    #define FLD_T8032_RST Fld(1,0,AC_MSKB0)//[0:0]
#define PDWNC_SHREG0 (IO_VIRT+IO_PDWNC_BASE + 0x180)
    #define FLD_SHREG0 Fld(32,0,AC_FULLDW)//[31:0]
#define PDWNC_SHREG1 (IO_VIRT+IO_PDWNC_BASE + 0x184)
    #define FLD_SHREG1 Fld(32,0,AC_FULLDW)//[31:0]
#define PDWNC_SHREG2 (IO_VIRT+IO_PDWNC_BASE + 0x188)
    #define FLD_SHREG2 Fld(32,0,AC_FULLDW)//[31:0]
#define PDWNC_SHREG3 (IO_VIRT+IO_PDWNC_BASE + 0x18C)
    #define FLD_SHREG3 Fld(32,0,AC_FULLDW)//[31:0]
#define PDWNC_EDID_DEV0 (IO_VIRT+IO_PDWNC_BASE + 0x190)
    #define FLD_SW_RST0 Fld(1,31,AC_MSKB3)//[31:31]
    #define FLD_DE_CNT0 Fld(8,20,AC_MSKW32)//[27:20]
    #define FLD_DEGLITCH0_EN Fld(1,17,AC_MSKB2)//[17:17]
    #define FLD_EDID0_DIS Fld(1,16,AC_MSKB2)//[16:16]
    #define FLD_DEVICE_ADDR0 Fld(7,1,AC_MSKB0)//[7:1]
#define PDWNC_EDID_DEV1 (IO_VIRT+IO_PDWNC_BASE + 0x194)
    #define FLD_SW_RST1 Fld(1,31,AC_MSKB3)//[31:31]
    #define FLD_DE_CNT1 Fld(8,20,AC_MSKW32)//[27:20]
    #define FLD_DEGLITCH1_EN Fld(1,17,AC_MSKB2)//[17:17]
    #define FLD_EDID1_DIS Fld(1,16,AC_MSKB2)//[16:16]
    #define FLD_DEVICE_ADDR1 Fld(7,1,AC_MSKB0)//[7:1]
#define PDWNC_EDID_DEV2 (IO_VIRT+IO_PDWNC_BASE + 0x198)
    #define FLD_SW_RST2 Fld(1,31,AC_MSKB3)//[31:31]
    #define FLD_DE_CNT2 Fld(8,20,AC_MSKW32)//[27:20]
    #define FLD_DEGLITCH2_EN Fld(1,17,AC_MSKB2)//[17:17]
    #define FLD_EDID2_DIS Fld(1,16,AC_MSKB2)//[16:16]
    #define FLD_DEVICE_ADDR2 Fld(7,1,AC_MSKB0)//[7:1]
#define PDWNC_EDID_DEV3 (IO_VIRT+IO_PDWNC_BASE + 0x19C)
    #define FLD_SW_RST3 Fld(1,31,AC_MSKB3)//[31:31]
    #define FLD_DE_CNT3 Fld(8,20,AC_MSKW32)//[27:20]
    #define FLD_DEGLITCH3_EN Fld(1,17,AC_MSKB2)//[17:17]
    #define FLD_EDID3_DIS Fld(1,16,AC_MSKB2)//[16:16]
    #define FLD_DEVICE_ADDR3 Fld(7,1,AC_MSKB0)//[7:1]
#define PDWNC_EDID_CTL1 (IO_VIRT+IO_PDWNC_BASE + 0x1A4)
    #define FLD_PHY_ADDR1 Fld(16,16,AC_FULLW32)//[31:16]
    #define FLD_OFFSET1 Fld(8,8,AC_FULLB1)//[15:8]
    #define FLD_CHKSUM1 Fld(8,0,AC_FULLB0)//[7:0]
#define PDWNC_EDID_CTL2 (IO_VIRT+IO_PDWNC_BASE + 0x1A8)
    #define FLD_PHY_ADDR2 Fld(16,16,AC_FULLW32)//[31:16]
    #define FLD_OFFSET2 Fld(8,8,AC_FULLB1)//[15:8]
    #define FLD_CHKSUM2 Fld(8,0,AC_FULLB0)//[7:0]
#define PDWNC_EDID_CTL3 (IO_VIRT+IO_PDWNC_BASE + 0x1AC)
    #define FLD_PHY_ADDR3 Fld(16,16,AC_FULLW32)//[31:16]
    #define FLD_OFFSET3 Fld(8,8,AC_FULLB1)//[15:8]
    #define FLD_CHKSUM3 Fld(8,0,AC_FULLB0)//[7:0]
#define PDWNC_EDID_ADDR (IO_VIRT+IO_PDWNC_BASE + 0x1B0)
    #define FLD_ADDR_FIX Fld(1,8,AC_MSKB1)//[8:8]
    #define FLD_EDID_ADDR Fld(8,0,AC_FULLB0)//[7:0]
#define PDWNC_EDID_DATA (IO_VIRT+IO_PDWNC_BASE + 0x1B4)
    #define FLD_EDID_DATA Fld(8,0,AC_FULLB0)//[7:0]
#define PDWNC_SM0CTL0 (IO_VIRT+IO_PDWNC_BASE + 0x1C0)
    #define FLD_SIFM0_ODRAIN Fld(1,31,AC_MSKB3)//[31:31]
    #if defined(CC_MT5882)
	#define FLD_SIFM0_VSYNC_MODE Fld(2,28,AC_MSKB3)//[29:28]
	#endif
    #define FLD_SIFM0_CLK_DIV Fld(12,16,AC_MSKW32)//[27:16]
    #define FLD_SIFM0_DE_CNT Fld(8,8,AC_FULLB1)//[15:8]
    #define FLD_SIFM0_WAIT_LEVEL Fld(1,6,AC_MSKB0)//[6:6]
    #define FLD_SIFM0_DEGLITCH_EN Fld(1,5,AC_MSKB0)//[5:5]
    #define FLD_SIFM0_CS_STATUS Fld(1,4,AC_MSKB0)//[4:4]
    #define FLD_SIFM0_SCL_STATE Fld(1,3,AC_MSKB0)//[3:3]
    #define FLD_SIFM0_SDA_STATE Fld(1,2,AC_MSKB0)//[2:2]
    #if defined(CC_MT5882)
	#define FLD_SIFM0_EN Fld(1,1,AC_MSKB0)//[1:1]
	#endif
    #define FLD_SIFM0_SCL_STRECH Fld(1,0,AC_MSKB0)//[0:0]
#define PDWNC_SM0CTL1 (IO_VIRT+IO_PDWNC_BASE + 0x1C4)
	#if defined(CC_MT5882)
	#define FLD_SIFM0_ACK Fld(16,16,AC_FULLW32)//[31:16]
	#define FLD_SIFM0_PGLEN Fld(6,8,AC_MSKB1)//[13:8]
	#else
    #define FLD_SIFM0_ACK Fld(8,16,AC_FULLB2)//[23:16]
    #define FLD_SIFM0_PGLEN Fld(3,8,AC_MSKB1)//[10:8]
    #endif
    #define FLD_SIFM0_MODE Fld(3,4,AC_MSKB0)//[6:4]
    #define FLD_SIFM0_TRI Fld(1,0,AC_MSKB0)//[0:0]
#define PDWNC_SM0D0 (IO_VIRT+IO_PDWNC_BASE + 0x1C8)
    #define FLD_SIFM0_DATA3 Fld(8,24,AC_FULLB3)//[31:24]
    #define FLD_SIFM0_DATA2 Fld(8,16,AC_FULLB2)//[23:16]
    #define FLD_SIFM0_DATA1 Fld(8,8,AC_FULLB1)//[15:8]
    #define FLD_SIFM0_DATA0 Fld(8,0,AC_FULLB0)//[7:0]
#define PDWNC_SM0D1 (IO_VIRT+IO_PDWNC_BASE + 0x1CC)
    #define FLD_SIFM0_DATA7 Fld(8,24,AC_FULLB3)//[31:24]
    #define FLD_SIFM0_DATA6 Fld(8,16,AC_FULLB2)//[23:16]
    #define FLD_SIFM0_DATA5 Fld(8,8,AC_FULLB1)//[15:8]
    #define FLD_SIFM0_DATA4 Fld(8,0,AC_FULLB0)//[7:0]
#define PDWNC_VGA_DEV (IO_VIRT+IO_PDWNC_BASE + 0x1D0)
    #define FLD_DE_CNT Fld(8,16,AC_FULLB2)//[23:16]
    #define FLD_DEGLITCH_EN Fld(1,9,AC_MSKB1)//[9:9]
    #define FLD_VGA_EN Fld(1,8,AC_MSKB1)//[8:8]
    #define FLD_DEV_ADDR Fld(6,2,AC_MSKB0)//[7:2]
#define PDWNC_VGA_ADDR (IO_VIRT+IO_PDWNC_BASE + 0x1D4)
    #define FLD_VGA_ADDR Fld(6,0,AC_MSKB0)//[5:0]
#define PDWNC_VGA_DATA (IO_VIRT+IO_PDWNC_BASE + 0x1D8)
    #define FLD_VGA_DATA Fld(32,0,AC_FULLDW)//[31:0]
#define PDWNC_ARM_INT (IO_VIRT+IO_PDWNC_BASE + 0x1E8)
    #define FLD_ARM_INT Fld(1,0,AC_MSKB0)//[0:0]
#define PDWNC_AUX_INT (IO_VIRT+IO_PDWNC_BASE + 0x1F4)
    #define FLD_AUX_INT Fld(1,0,AC_MSKB0)//[0:0]
#define PDWNC_IRCNT (IO_VIRT+IO_PDWNC_BASE + 0x200)
    #define FLD_RES_THREE Fld(8,24,AC_FULLB3)//[31:24]
    #define FLD_RES_TWO Fld(8,16,AC_FULLB2)//[23:16]
    #define FLD_RES_ONE Fld(8,8,AC_FULLB1)//[15:8]
    #define FLD_BIT_CNT Fld(8,0,AC_FULLB0)//[7:0]
#define PDWNC_IRDATA0 (IO_VIRT+IO_PDWNC_BASE + 0x204)
    #define FLD_BIT_REG3 Fld(8,24,AC_FULLB3)//[31:24]
    #define FLD_BIT_REG2 Fld(8,16,AC_FULLB2)//[23:16]
    #define FLD_BIT_REG1 Fld(8,8,AC_FULLB1)//[15:8]
    #define FLD_BIT_REG0 Fld(8,0,AC_FULLB0)//[7:0]
#define PDWNC_IRDATA1 (IO_VIRT+IO_PDWNC_BASE + 0x208)
    #define FLD_BIT_REG7 Fld(8,24,AC_FULLB3)//[31:24]
    #define FLD_BIT_REG6 Fld(8,16,AC_FULLB2)//[23:16]
    #define FLD_BIT_REG5 Fld(8,8,AC_FULLB1)//[15:8]
    #define FLD_BIT_REG4 Fld(8,0,AC_FULLB0)//[7:0]
#define PDWNC_IRCFGH (IO_VIRT+IO_PDWNC_BASE + 0x20C)
    #define FLD_IREND Fld(7,16,AC_MSKB2)//[22:16]
    #define FLD_DISPD Fld(1,15,AC_MSKB1)//[15:15]
    #define FLD_IGB0 Fld(1,14,AC_MSKB1)//[14:14]
    #define FLD_CHK_EN Fld(1,13,AC_MSKB1)//[13:13]
    #define FLD_DISCH Fld(1,7,AC_MSKB0)//[7:7]
    #define FLD_DISCL Fld(1,6,AC_MSKB0)//[6:6]
    #define FLD_IGSYN Fld(1,5,AC_MSKB0)//[5:5]
    #define FLD_ORDINV Fld(1,4,AC_MSKB0)//[4:4]
    #define FLD_RC5_1ST Fld(1,3,AC_MSKB0)//[3:3]
    #define FLD_RC5 Fld(1,2,AC_MSKB0)//[2:2]
    #define FLD_IRI Fld(1,1,AC_MSKB0)//[1:1]
#define PDWNC_IRCFGL (IO_VIRT+IO_PDWNC_BASE + 0x210)
    #define FLD_SAPERIOD Fld(8,0,AC_FULLB0)//[7:0]
    #define FLD_CHK_SAPERIOD Fld(13,8,AC_MSKW21)//[20:8]
#define PDWNC_IRTHD (IO_VIRT+IO_PDWNC_BASE + 0x214)
    #define FLD_DG_DEL Fld(2,8,AC_MSKB1)//[9:8]
    #define FLD_ICLR Fld(1,7,AC_MSKB0)//[7:7]
    #define FLD_IRTHD Fld(7,0,AC_MSKB0)//[6:0]
#define PDWNC_IRCLR (IO_VIRT+IO_PDWNC_BASE + 0x218)
    #define FLD_IRCLR Fld(1,0,AC_MSKB0)//[0:0]
#define PDWNC_IREXP_EN (IO_VIRT+IO_PDWNC_BASE + 0x21C)
    #define FLD_BCEPEN Fld(1,16,AC_MSKB2)//[16:16]
    #define FLD_IREXPEN Fld(16,0,AC_FULLW10)//[15:0]
#define PDWNC_ENEXP_IRM (IO_VIRT+IO_PDWNC_BASE + 0x220)
    #define FLD_ENEXP_BIT_REG3 Fld(8,24,AC_FULLB3)//[31:24]
    #define FLD_ENEXP_BIT_REG2 Fld(8,16,AC_FULLB2)//[23:16]
    #define FLD_ENEXP_BIT_REG1 Fld(8,8,AC_FULLB1)//[15:8]
    #define FLD_ENEXP_BIT_REG0 Fld(8,0,AC_FULLB0)//[7:0]
#define PDWNC_EXP_IR0 (IO_VIRT+IO_PDWNC_BASE + 0x228)
    #define FLD_EXP0_BITREG3 Fld(8,24,AC_FULLB3)//[31:24]
    #define FLD_EXP0_BITREG2 Fld(8,16,AC_FULLB2)//[23:16]
    #define FLD_EXP0_BITREG1 Fld(8,8,AC_FULLB1)//[15:8]
    #define FLD_EXP0_BITREG0 Fld(8,0,AC_FULLB0)//[7:0]
#define PDWNC_EXP_IR1 (IO_VIRT+IO_PDWNC_BASE + 0x22C)
    #define FLD_EXP1_BITREG3 Fld(8,24,AC_FULLB3)//[31:24]
    #define FLD_EXP1_BITREG2 Fld(8,16,AC_FULLB2)//[23:16]
    #define FLD_EXP1_BITREG1 Fld(8,8,AC_FULLB1)//[15:8]
    #define FLD_EXP1_BITREG0 Fld(8,0,AC_FULLB0)//[7:0]
#define PDWNC_EXP_IR2 (IO_VIRT+IO_PDWNC_BASE + 0x230)
    #define FLD_EXP2_BITREG3 Fld(8,24,AC_FULLB3)//[31:24]
    #define FLD_EXP2_BITREG2 Fld(8,16,AC_FULLB2)//[23:16]
    #define FLD_EXP2_BITREG1 Fld(8,8,AC_FULLB1)//[15:8]
    #define FLD_EXP2_BITREG0 Fld(8,0,AC_FULLB0)//[7:0]
#define PDWNC_EXP_IR3 (IO_VIRT+IO_PDWNC_BASE + 0x234)
    #define FLD_EXP3_BITREG3 Fld(8,24,AC_FULLB3)//[31:24]
    #define FLD_EXP3_BITREG2 Fld(8,16,AC_FULLB2)//[23:16]
    #define FLD_EXP3_BITREG1 Fld(8,8,AC_FULLB1)//[15:8]
    #define FLD_EXP3_BITREG0 Fld(8,0,AC_FULLB0)//[7:0]
#define PDWNC_EXP_IR4 (IO_VIRT+IO_PDWNC_BASE + 0x238)
    #define FLD_EXP4_BITREG3 Fld(8,24,AC_FULLB3)//[31:24]
    #define FLD_EXP4_BITREG2 Fld(8,16,AC_FULLB2)//[23:16]
    #define FLD_EXP4_BITREG1 Fld(8,8,AC_FULLB1)//[15:8]
    #define FLD_EXP4_BITREG0 Fld(8,0,AC_FULLB0)//[7:0]
#define PDWNC_EXP_IR5 (IO_VIRT+IO_PDWNC_BASE + 0x23C)
    #define FLD_EXP5_BITREG3 Fld(8,24,AC_FULLB3)//[31:24]
    #define FLD_EXP5_BITREG2 Fld(8,16,AC_FULLB2)//[23:16]
    #define FLD_EXP5_BITREG1 Fld(8,8,AC_FULLB1)//[15:8]
    #define FLD_EXP5_BITREG0 Fld(8,0,AC_FULLB0)//[7:0]
#define PDWNC_EXP_IR6 (IO_VIRT+IO_PDWNC_BASE + 0x240)
    #define FLD_EXP6_BITREG3 Fld(8,24,AC_FULLB3)//[31:24]
    #define FLD_EXP6_BITREG2 Fld(8,16,AC_FULLB2)//[23:16]
    #define FLD_EXP6_BITREG1 Fld(8,8,AC_FULLB1)//[15:8]
    #define FLD_EXP6_BITREG0 Fld(8,0,AC_FULLB0)//[7:0]
#define PDWNC_EXP_IR7 (IO_VIRT+IO_PDWNC_BASE + 0x244)
    #define FLD_EXP7_BITREG3 Fld(8,24,AC_FULLB3)//[31:24]
    #define FLD_EXP7_BITREG2 Fld(8,16,AC_FULLB2)//[23:16]
    #define FLD_EXP7_BITREG1 Fld(8,8,AC_FULLB1)//[15:8]
    #define FLD_EXP7_BITREG0 Fld(8,0,AC_FULLB0)//[7:0]
#define PDWNC_EXP_IR8 (IO_VIRT+IO_PDWNC_BASE + 0x248)
    #define FLD_EXP8_BITREG3 Fld(8,24,AC_FULLB3)//[31:24]
    #define FLD_EXP8_BITREG2 Fld(8,16,AC_FULLB2)//[23:16]
    #define FLD_EXP8_BITREG1 Fld(8,8,AC_FULLB1)//[15:8]
    #define FLD_EXP8_BITREG0 Fld(8,0,AC_FULLB0)//[7:0]
#define PDWNC_EXP_IR9 (IO_VIRT+IO_PDWNC_BASE + 0x24C)
    #define FLD_EXP9_BITREG3 Fld(8,24,AC_FULLB3)//[31:24]
    #define FLD_EXP9_BITREG2 Fld(8,16,AC_FULLB2)//[23:16]
    #define FLD_EXP9_BITREG1 Fld(8,8,AC_FULLB1)//[15:8]
    #define FLD_EXP9_BITREG0 Fld(8,0,AC_FULLB0)//[7:0]
#define PDWNC_EXP_IR10 (IO_VIRT+IO_PDWNC_BASE + 0x250)
    #define FLD_EXP10_BITREG3 Fld(8,24,AC_FULLB3)//[31:24]
    #define FLD_EXP10_BITREG2 Fld(8,16,AC_FULLB2)//[23:16]
    #define FLD_EXP10_BITREG1 Fld(8,8,AC_FULLB1)//[15:8]
    #define FLD_EXP10_BITREG0 Fld(8,0,AC_FULLB0)//[7:0]
#define PDWNC_EXP_IR11 (IO_VIRT+IO_PDWNC_BASE + 0x254)
    #define FLD_EXP11_BITREG3 Fld(8,24,AC_FULLB3)//[31:24]
    #define FLD_EXP11_BITREG2 Fld(8,16,AC_FULLB2)//[23:16]
    #define FLD_EXP11_BITREG1 Fld(8,8,AC_FULLB1)//[15:8]
    #define FLD_EXP11_BITREG0 Fld(8,0,AC_FULLB0)//[7:0]
#define PDWNC_EXP_IR12 (IO_VIRT+IO_PDWNC_BASE + 0x258)
    #define FLD_EXP12_BITREG3 Fld(8,24,AC_FULLB3)//[31:24]
    #define FLD_EXP12_BITREG2 Fld(8,16,AC_FULLB2)//[23:16]
    #define FLD_EXP12_BITREG1 Fld(8,8,AC_FULLB1)//[15:8]
    #define FLD_EXP12_BITREG0 Fld(8,0,AC_FULLB0)//[7:0]
#define PDWNC_EXP_IR13 (IO_VIRT+IO_PDWNC_BASE + 0x25C)
    #define FLD_EXP13_BITREG3 Fld(8,24,AC_FULLB3)//[31:24]
    #define FLD_EXP13_BITREG2 Fld(8,16,AC_FULLB2)//[23:16]
    #define FLD_EXP13_BITREG1 Fld(8,8,AC_FULLB1)//[15:8]
    #define FLD_EXP13_BITREG0 Fld(8,0,AC_FULLB0)//[7:0]
#define PDWNC_EXP_IR14 (IO_VIRT+IO_PDWNC_BASE + 0x260)
    #define FLD_EXP14_BITREG3 Fld(8,24,AC_FULLB3)//[31:24]
    #define FLD_EXP14_BITREG2 Fld(8,16,AC_FULLB2)//[23:16]
    #define FLD_EXP14_BITREG1 Fld(8,8,AC_FULLB1)//[15:8]
    #define FLD_EXP14_BITREG0 Fld(8,0,AC_FULLB0)//[7:0]
#define PDWNC_EXP_IR15 (IO_VIRT+IO_PDWNC_BASE + 0x264)
    #define FLD_EXP15_BITREG3 Fld(8,24,AC_FULLB3)//[31:24]
    #define FLD_EXP15_BITREG2 Fld(8,16,AC_FULLB2)//[23:16]
    #define FLD_EXP15_BITREG1 Fld(8,8,AC_FULLB1)//[15:8]
    #define FLD_EXP15_BITREG0 Fld(8,0,AC_FULLB0)//[7:0]
#define PDWNC_EXP_BCNT (IO_VIRT+IO_PDWNC_BASE + 0x268)
    #define FLD_EXP_BITCNT Fld(6,0,AC_MSKB0)//[5:0]
#define PDWNC_RCMM_THD0 (IO_VIRT+IO_PDWNC_BASE + 0x26C)
    #define FLD_RCMM_ENABLE Fld(1,31,AC_MSKB3)//[31:31]
    #define FLD_RCMM_THD11 Fld(7,21,AC_MSKW32)//[27:21]
    #define FLD_RCMM_THD10 Fld(7,14,AC_MSKW21)//[20:14]
    #define FLD_RCMM_THD01 Fld(7,7,AC_MSKW10)//[13:7]
    #define FLD_RCMM_THD00 Fld(7,0,AC_MSKB0)//[6:0]
#define PDWNC_RCMM_THD1 (IO_VIRT+IO_PDWNC_BASE + 0x270)
    #define FLD_RCMM_THD21 Fld(7,7,AC_MSKW10)//[13:7]
    #define FLD_RCMM_THD20 Fld(7,0,AC_MSKB0)//[6:0]
#define PDWNC_IRDATA2 (IO_VIRT+IO_PDWNC_BASE + 0x290)
    #define FLD_BIT_REG11 Fld(8,24,AC_FULLB3)//[31:24]
    #define FLD_BIT_REG10 Fld(8,16,AC_FULLB2)//[23:16]
    #define FLD_BIT_REG9 Fld(8,8,AC_FULLB1)//[15:8]
    #define FLD_BIT_REG8 Fld(8,0,AC_FULLB0)//[7:0]
#define PDWNC_IRDATA3 (IO_VIRT+IO_PDWNC_BASE + 0x294)
    #define FLD_BIT_REG15 Fld(8,24,AC_FULLB3)//[31:24]
    #define FLD_BIT_REG14 Fld(8,16,AC_FULLB2)//[23:16]
    #define FLD_BIT_REG13 Fld(8,8,AC_FULLB1)//[15:8]
    #define FLD_BIT_REG12 Fld(8,0,AC_FULLB0)//[7:0]
#define PDWNC_CHKDATA0 (IO_VIRT+IO_PDWNC_BASE + 0x2BC)
    #define FLD_CHK_BYTE3 Fld(8,24,AC_FULLB3)//[31:24]
    #define FLD_CHK_BYTE2 Fld(8,16,AC_FULLB2)//[23:16]
    #define FLD_CHK_BYTE1 Fld(8,8,AC_FULLB1)//[15:8]
    #define FLD_CHK_BYTE0 Fld(8,0,AC_FULLB0)//[7:0]
#define PDWNC_CHKDATA1 (IO_VIRT+IO_PDWNC_BASE + 0x2C0)
    #define FLD_CHK_BYTE7 Fld(8,24,AC_FULLB3)//[31:24]
    #define FLD_CHK_BYTE6 Fld(8,16,AC_FULLB2)//[23:16]
    #define FLD_CHK_BYTE5 Fld(8,8,AC_FULLB1)//[15:8]
    #define FLD_CHK_BYTE4 Fld(8,0,AC_FULLB0)//[7:0]
#define PDWNC_CHKDATA2 (IO_VIRT+IO_PDWNC_BASE + 0x2C4)
    #define FLD_CHK_BYTE11 Fld(8,24,AC_FULLB3)//[31:24]
    #define FLD_CHK_BYTE10 Fld(8,16,AC_FULLB2)//[23:16]
    #define FLD_CHK_BYTE9 Fld(8,8,AC_FULLB1)//[15:8]
    #define FLD_CHK_BYTE8 Fld(8,0,AC_FULLB0)//[7:0]
#define PDWNC_CHKDATA3 (IO_VIRT+IO_PDWNC_BASE + 0x2C8)
    #define FLD_CHK_BYTE15 Fld(8,24,AC_FULLB3)//[31:24]
    #define FLD_CHK_BYTE14 Fld(8,16,AC_FULLB2)//[23:16]
    #define FLD_CHK_BYTE13 Fld(8,8,AC_FULLB1)//[15:8]
    #define FLD_CHK_BYTE12 Fld(8,0,AC_FULLB0)//[7:0]
#define PDWNC_CHKDATA4 (IO_VIRT+IO_PDWNC_BASE + 0x2CC)
    #define FLD_CHK_BYTE19 Fld(8,24,AC_FULLB3)//[31:24]
    #define FLD_CHK_BYTE18 Fld(8,16,AC_FULLB2)//[23:16]
    #define FLD_CHK_BYTE17 Fld(8,8,AC_FULLB1)//[15:8]
    #define FLD_CHK_BYTE16 Fld(8,0,AC_FULLB0)//[7:0]
#define PDWNC_CHKDATA5 (IO_VIRT+IO_PDWNC_BASE + 0x2D0)
    #define FLD_CHK_BYTE23 Fld(8,24,AC_FULLB3)//[31:24]
    #define FLD_CHK_BYTE22 Fld(8,16,AC_FULLB2)//[23:16]
    #define FLD_CHK_BYTE21 Fld(8,8,AC_FULLB1)//[15:8]
    #define FLD_CHK_BYTE20 Fld(8,0,AC_FULLB0)//[7:0]
#define PDWNC_CHKDATA6 (IO_VIRT+IO_PDWNC_BASE + 0x2D4)
    #define FLD_CHK_BYTE27 Fld(8,24,AC_FULLB3)//[31:24]
    #define FLD_CHK_BYTE26 Fld(8,16,AC_FULLB2)//[23:16]
    #define FLD_CHK_BYTE25 Fld(8,8,AC_FULLB1)//[15:8]
    #define FLD_CHK_BYTE24 Fld(8,0,AC_FULLB0)//[7:0]
#define PDWNC_CHKDATA7 (IO_VIRT+IO_PDWNC_BASE + 0x2D8)
    #define FLD_CHK_BYTE31 Fld(8,24,AC_FULLB3)//[31:24]
    #define FLD_CHK_BYTE30 Fld(8,16,AC_FULLB2)//[23:16]
    #define FLD_CHK_BYTE29 Fld(8,8,AC_FULLB1)//[15:8]
    #define FLD_CHK_BYTE28 Fld(8,0,AC_FULLB0)//[7:0]
#define PDWNC_CHKDATA8 (IO_VIRT+IO_PDWNC_BASE + 0x2DC)
    #define FLD_CHK_BYTE35 Fld(8,24,AC_FULLB3)//[31:24]
    #define FLD_CHK_BYTE34 Fld(8,16,AC_FULLB2)//[23:16]
    #define FLD_CHK_BYTE33 Fld(8,8,AC_FULLB1)//[15:8]
    #define FLD_CHK_BYTE32 Fld(8,0,AC_FULLB0)//[7:0]
#define PDWNC_CHKDATA9 (IO_VIRT+IO_PDWNC_BASE + 0x2E0)
    #define FLD_CHK_BYTE39 Fld(8,24,AC_FULLB3)//[31:24]
    #define FLD_CHK_BYTE38 Fld(8,16,AC_FULLB2)//[23:16]
    #define FLD_CHK_BYTE37 Fld(8,8,AC_FULLB1)//[15:8]
    #define FLD_CHK_BYTE36 Fld(8,0,AC_FULLB0)//[7:0]
#define PDWNC_CHKDATA10 (IO_VIRT+IO_PDWNC_BASE + 0x2E4)
    #define FLD_CHK_BYTE43 Fld(8,24,AC_FULLB3)//[31:24]
    #define FLD_CHK_BYTE42 Fld(8,16,AC_FULLB2)//[23:16]
    #define FLD_CHK_BYTE41 Fld(8,8,AC_FULLB1)//[15:8]
    #define FLD_CHK_BYTE40 Fld(8,0,AC_FULLB0)//[7:0]
#define PDWNC_CHKDATA11 (IO_VIRT+IO_PDWNC_BASE + 0x2E8)
    #define FLD_CHK_BYTE47 Fld(8,24,AC_FULLB3)//[31:24]
    #define FLD_CHK_BYTE46 Fld(8,16,AC_FULLB2)//[23:16]
    #define FLD_CHK_BYTE45 Fld(8,8,AC_FULLB1)//[15:8]
    #define FLD_CHK_BYTE44 Fld(8,0,AC_FULLB0)//[7:0]
#define PDWNC_CHKDATA12 (IO_VIRT+IO_PDWNC_BASE + 0x2EC)
    #define FLD_CHK_BYTE51 Fld(8,24,AC_FULLB3)//[31:24]
    #define FLD_CHK_BYTE50 Fld(8,16,AC_FULLB2)//[23:16]
    #define FLD_CHK_BYTE49 Fld(8,8,AC_FULLB1)//[15:8]
    #define FLD_CHK_BYTE48 Fld(8,0,AC_FULLB0)//[7:0]
#define PDWNC_CHKDATA13 (IO_VIRT+IO_PDWNC_BASE + 0x2F0)
    #define FLD_CHK_BYTE55 Fld(8,24,AC_FULLB3)//[31:24]
    #define FLD_CHK_BYTE54 Fld(8,16,AC_FULLB2)//[23:16]
    #define FLD_CHK_BYTE53 Fld(8,8,AC_FULLB1)//[15:8]
    #define FLD_CHK_BYTE52 Fld(8,0,AC_FULLB0)//[7:0]
#define PDWNC_CHKDATA14 (IO_VIRT+IO_PDWNC_BASE + 0x2F4)
    #define FLD_CHK_BYTE59 Fld(8,24,AC_FULLB3)//[31:24]
    #define FLD_CHK_BYTE58 Fld(8,16,AC_FULLB2)//[23:16]
    #define FLD_CHK_BYTE57 Fld(8,8,AC_FULLB1)//[15:8]
    #define FLD_CHK_BYTE56 Fld(8,0,AC_FULLB0)//[7:0]
#define PDWNC_CHKDATA15 (IO_VIRT+IO_PDWNC_BASE + 0x2F8)
    #define FLD_CHK_BYTE63 Fld(8,24,AC_FULLB3)//[31:24]
    #define FLD_CHK_BYTE62 Fld(8,16,AC_FULLB2)//[23:16]
    #define FLD_CHK_BYTE61 Fld(8,8,AC_FULLB1)//[15:8]
    #define FLD_CHK_BYTE60 Fld(8,0,AC_FULLB0)//[7:0]
#define PDWNC_CHKDATA16 (IO_VIRT+IO_PDWNC_BASE + 0x2FC)
    #define FLD_CHK_BYTE67 Fld(8,24,AC_FULLB3)//[31:24]
    #define FLD_CHK_BYTE66 Fld(8,16,AC_FULLB2)//[23:16]
    #define FLD_CHK_BYTE65 Fld(8,8,AC_FULLB1)//[15:8]
    #define FLD_CHK_BYTE64 Fld(8,0,AC_FULLB0)//[7:0]
#define PDWNC_SRVCFG0 (IO_VIRT+IO_PDWNC_BASE + 0x300)
    #define FLD_SRVCH_SEL Fld(4,4,AC_MSKB0)//[7:4]
    #define FLD_ABIST_MODE Fld(1,3,AC_MSKB0)//[3:3]
    #define FLD_SRVWAK_HD Fld(1,2,AC_MSKB0)//[2:2]
    #define FLD_SWEN Fld(1,1,AC_MSKB0)//[1:1]
    #define FLD_HWEN Fld(1,0,AC_MSKB0)//[0:0]
#define PDWNC_SRVCFG1 (IO_VIRT+IO_PDWNC_BASE + 0x304)
    #define FLD_SRVCH_EN Fld(16,0,AC_FULLW10)//[15:0]
#define PDWNC_SRVSWT (IO_VIRT+IO_PDWNC_BASE + 0x30C)
    #define FLD_SWTRG Fld(1,0,AC_MSKB0)//[0:0]
#define PDWNC_SRVCLR (IO_VIRT+IO_PDWNC_BASE + 0x310)
    #define FLD_ADCLR Fld(1,1,AC_MSKB0)//[1:1]
#define PDWNC_SRVRAT (IO_VIRT+IO_PDWNC_BASE + 0x314)
    #define FLD_SMP_RATE Fld(16,0,AC_FULLW10)//[15:0]
#define PDWNC_SRVTOTEN (IO_VIRT+IO_PDWNC_BASE + 0x320)
    #define FLD_TOUT_CNT Fld(8,8,AC_FULLB1)//[15:8]
    #define FLD_SRVHD_ST Fld(1,3,AC_MSKB0)//[3:3]
    #define FLD_DATA_RDST Fld(1,2,AC_MSKB0)//[2:2]
    #define FLD_TOUT_ST Fld(1,1,AC_MSKB0)//[1:1]
    #define FLD_TOUT_EN Fld(1,0,AC_MSKB0)//[0:0]
#define PDWNC_WAKTOP0 (IO_VIRT+IO_PDWNC_BASE + 0x328)
    #define FLD_CH0TOP Fld(8,2,AC_MSKW10)//[9:2]
#define PDWNC_WAKBTM0 (IO_VIRT+IO_PDWNC_BASE + 0x32C)
    #define FLD_CH0BTM Fld(8,2,AC_MSKW10)//[9:2]
#define PDWNC_CHCFG0 (IO_VIRT+IO_PDWNC_BASE + 0x330)
    #define FLD_CH0WKC Fld(3,0,AC_MSKB0)//[2:0]
#define PDWNC_WAKTOP1 (IO_VIRT+IO_PDWNC_BASE + 0x334)
    #define FLD_CH1TOP Fld(8,2,AC_MSKW10)//[9:2]
#define PDWNC_WAKBTM1 (IO_VIRT+IO_PDWNC_BASE + 0x338)
    #define FLD_CH1BTM Fld(8,2,AC_MSKW10)//[9:2]
#define PDWNC_CHCFG1 (IO_VIRT+IO_PDWNC_BASE + 0x33C)
    #define FLD_CH1WKC Fld(3,0,AC_MSKB0)//[2:0]
#define PDWNC_WAKTOP2 (IO_VIRT+IO_PDWNC_BASE + 0x340)
    #define FLD_CH2TOP Fld(8,2,AC_MSKW10)//[9:2]
#define PDWNC_WAKBTM2 (IO_VIRT+IO_PDWNC_BASE + 0x344)
    #define FLD_CH2BTM Fld(8,2,AC_MSKW10)//[9:2]
#define PDWNC_CHCFG2 (IO_VIRT+IO_PDWNC_BASE + 0x348)
    #define FLD_CH2WKC Fld(3,0,AC_MSKB0)//[2:0]
#define PDWNC_WAKTOP3 (IO_VIRT+IO_PDWNC_BASE + 0x34C)
    #define FLD_CH3TOP Fld(8,2,AC_MSKW10)//[9:2]
#define PDWNC_WAKBTM3 (IO_VIRT+IO_PDWNC_BASE + 0x350)
    #define FLD_CH3BTM Fld(8,2,AC_MSKW10)//[9:2]
#define PDWNC_CHCFG3 (IO_VIRT+IO_PDWNC_BASE + 0x354)
    #define FLD_CH3WKC Fld(3,0,AC_MSKB0)//[2:0]
#define PDWNC_WAKTOP4 (IO_VIRT+IO_PDWNC_BASE + 0x358)
    #define FLD_CH4TOP Fld(8,2,AC_MSKW10)//[9:2]
#define PDWNC_WAKBTM4 (IO_VIRT+IO_PDWNC_BASE + 0x35C)
    #define FLD_CH4BTM Fld(8,2,AC_MSKW10)//[9:2]
#define PDWNC_CHCFG4 (IO_VIRT+IO_PDWNC_BASE + 0x360)
    #define FLD_CH4WKC Fld(3,0,AC_MSKB0)//[2:0]
#define PDWNC_WAKTOP5 (IO_VIRT+IO_PDWNC_BASE + 0x364)
    #define FLD_CH5TOP Fld(8,2,AC_MSKW10)//[9:2]
#define PDWNC_WAKBTM5 (IO_VIRT+IO_PDWNC_BASE + 0x368)
    #define FLD_CH5BTM Fld(8,2,AC_MSKW10)//[9:2]
#define PDWNC_CHCFG5 (IO_VIRT+IO_PDWNC_BASE + 0x36C)
    #define FLD_CH5WKC Fld(3,0,AC_MSKB0)//[2:0]
#define PDWNC_SRVCST (IO_VIRT+IO_PDWNC_BASE + 0x370)
    #define FLD_SRVCST Fld(4,0,AC_MSKB0)//[3:0]
#define PDWNC_ADOUT0 (IO_VIRT+IO_PDWNC_BASE + 0x374)
    #define FLD_ADOUT0 Fld(8,2,AC_MSKW10)//[9:2]
#define PDWNC_ADOUT1 (IO_VIRT+IO_PDWNC_BASE + 0x378)
    #define FLD_ADOUT1 Fld(8,2,AC_MSKW10)//[9:2]
#define PDWNC_ADOUT2 (IO_VIRT+IO_PDWNC_BASE + 0x37C)
    #define FLD_ADOUT2 Fld(8,2,AC_MSKW10)//[9:2]
#define PDWNC_ADOUT3 (IO_VIRT+IO_PDWNC_BASE + 0x380)
    #define FLD_ADOUT3 Fld(8,2,AC_MSKW10)//[9:2]
#define PDWNC_ADOUT4 (IO_VIRT+IO_PDWNC_BASE + 0x384)
    #define FLD_ADOUT4 Fld(8,2,AC_MSKW10)//[9:2]
#define PDWNC_ADOUT5 (IO_VIRT+IO_PDWNC_BASE + 0x388)
    #define FLD_ADOUT5 Fld(8,2,AC_MSKW10)//[9:2]
#define PDWNC_ADOUT6 (IO_VIRT+IO_PDWNC_BASE + 0x38C)
    #define FLD_ADOUT6 Fld(8,2,AC_MSKW10)//[9:2]
#define PDWNC_ADOUT7 (IO_VIRT+IO_PDWNC_BASE + 0x390)
    #define FLD_ADOUT7 Fld(8,2,AC_MSKW10)//[9:2]

#define PDWNC_ADOUT8 (IO_VIRT+IO_PDWNC_BASE + 0x394)    //add by zuyu for TVE Calibration
    #define FLD_ADOUT8 Fld(8,2,AC_MSKW10)//[9:2]

#define PDWNC_WAKTOP6 (IO_VIRT+IO_PDWNC_BASE + 0x3A0)
    #define FLD_CH6TOP Fld(8,2,AC_MSKW10)//[9:2]
#define PDWNC_WAKBTM6 (IO_VIRT+IO_PDWNC_BASE + 0x3A4)
    #define FLD_CH6BTM Fld(8,2,AC_MSKW10)//[9:2]
#define PDWNC_CHCFG6 (IO_VIRT+IO_PDWNC_BASE + 0x3A8)
    #define FLD_CH6WKC Fld(3,0,AC_MSKB0)//[2:0]
#define PDWNC_WAKTOP7 (IO_VIRT+IO_PDWNC_BASE + 0x3AC)
    #define FLD_CH7TOP Fld(8,2,AC_MSKW10)//[9:2]
#define PDWNC_WAKBTM7 (IO_VIRT+IO_PDWNC_BASE + 0x3B0)
    #define FLD_CH7BTM Fld(8,2,AC_MSKW10)//[9:2]
#define PDWNC_CHCFG7 (IO_VIRT+IO_PDWNC_BASE + 0x3B4)
    #define FLD_CH7WKC Fld(3,0,AC_MSKB0)//[2:0]
#define PDWNC_PRST (IO_VIRT+IO_PDWNC_BASE + 0x3C0)
    #define FLD_PRST_EN Fld(1,31,AC_MSKB3)//[31:31]
    #define FLD_VOLTAGE_CHD Fld(10,20,AC_MSKW32)//[29:20]
    #define FLD_VOLTAGE_CHC Fld(10,10,AC_MSKW21)//[19:10]
    #define FLD_VOLTAGE_CHB Fld(10,0,AC_MSKW10)//[9:0]
#define PDWNC_PRST_1 (IO_VIRT+IO_PDWNC_BASE + 0x3C4)
    #define FLD_VOLTAGE_CHF Fld(10,10,AC_MSKW21)//[19:10]
    #define FLD_VOLTAGE_CHE Fld(10,0,AC_MSKW10)//[9:0]
#define PDWNC_PRST_2 (IO_VIRT+IO_PDWNC_BASE + 0x3C8)
    #define FLD_CHF_FILTER Fld(5,20,AC_MSKW32)//[24:20]
    #define FLD_CHE_FILTER Fld(5,15,AC_MSKW21)//[19:15]
    #define FLD_CHD_FILTER Fld(5,10,AC_MSKB1)//[14:10]
    #define FLD_CHC_FILTER Fld(5,5,AC_MSKW10)//[9:5]
    #define FLD_CHB_FILTER Fld(5,0,AC_MSKB0)//[4:0]
#define PDWNC_ADOUTB (IO_VIRT+IO_PDWNC_BASE + 0x3CC)
    #define FLD_ADOUTB Fld(8,2,AC_MSKW10)//[9:2]
#define PDWNC_ADOUTC (IO_VIRT+IO_PDWNC_BASE + 0x3D0)
    #define FLD_ADOUTC Fld(8,2,AC_MSKW10)//[9:2]
#define PDWNC_ADOUTD (IO_VIRT+IO_PDWNC_BASE + 0x3D4)
    #define FLD_ADOUTD Fld(8,2,AC_MSKW10)//[9:2]
#define PDWNC_ADOUTE (IO_VIRT+IO_PDWNC_BASE + 0x3D8)
    #define FLD_ADOUTE Fld(8,2,AC_MSKW10)//[9:2]
#define PDWNC_ADOUTF (IO_VIRT+IO_PDWNC_BASE + 0x3DC)
    #define FLD_ADOUTF Fld(8,2,AC_MSKW10)//[9:2]
#if defined(CC_MT5882)
#define PDWNC_SM2CTL0 (IO_VIRT+IO_PDWNC_BASE + 0x3E0)
    #define FLD_SIFM2_ODRAIN Fld(1,31,AC_MSKB3)//[31:31]
    #define FLD_SIFM2_VSYNC_MODE Fld(2,28,AC_MSKB3)//[29:28]
    #define FLD_SIFM2_CLK_DIV Fld(12,16,AC_MSKW32)//[27:16]
    #define FLD_SIFM2_DE_CNT Fld(8,8,AC_FULLB1)//[15:8]
    #define FLD_SIFM2_WAIT_LEVEL Fld(1,6,AC_MSKB0)//[6:6]
    #define FLD_SIFM2_DEGLITCH_EN Fld(1,5,AC_MSKB0)//[5:5]
    #define FLD_SIFM2_CS_STATUS Fld(1,4,AC_MSKB0)//[4:4]
    #define FLD_SIFM2_SCL_STATE Fld(1,3,AC_MSKB0)//[3:3]
    #define FLD_SIFM2_SDA_STATE Fld(1,2,AC_MSKB0)//[2:2]
    #define FLD_SIFM2_EN Fld(1,1,AC_MSKB0)//[1:1]
    #define FLD_SIFM2_SCL_STRECH Fld(1,0,AC_MSKB0)//[0:0]
#define PDWNC_SM2CTL1 (IO_VIRT+IO_PDWNC_BASE + 0x3E4)
    #define FLD_SIFM2_ACK Fld(16,16,AC_FULLW32)//[31:16]
    #define FLD_SIFM2_PGLEN Fld(6,8,AC_MSKB1)//[13:8]
    #define FLD_SIFM2_MODE Fld(3,4,AC_MSKB0)//[6:4]
    #define FLD_SIFM2_TRI Fld(1,0,AC_MSKB0)//[0:0]
#define PDWNC_SM2D0 (IO_VIRT+IO_PDWNC_BASE + 0x3E8)
    #define FLD_SIFM2_DATA3 Fld(8,24,AC_FULLB3)//[31:24]
    #define FLD_SIFM2_DATA2 Fld(8,16,AC_FULLB2)//[23:16]
    #define FLD_SIFM2_DATA1 Fld(8,8,AC_FULLB1)//[15:8]
    #define FLD_SIFM2_DATA0 Fld(8,0,AC_FULLB0)//[7:0]
#define PDWNC_SM2D1 (IO_VIRT+IO_PDWNC_BASE + 0x3EC)
    #define FLD_SIFM2_DATA7 Fld(8,24,AC_FULLB3)//[31:24]
    #define FLD_SIFM2_DATA6 Fld(8,16,AC_FULLB2)//[23:16]
    #define FLD_SIFM2_DATA5 Fld(8,8,AC_FULLB1)//[15:8]
    #define FLD_SIFM2_DATA4 Fld(8,0,AC_FULLB0)//[7:0]
#endif
#define PDWNC_VGA_WAKE_UP_CONTROL_0_REGISTER (IO_VIRT+IO_PDWNC_BASE + 0x400)
    #define FLD_DBG_SEL Fld(4,28,AC_MSKB3)//[31:28]
    #define FLD_AUTO_SWITCH_EN Fld(1,27,AC_MSKB3)//[27:27]
    #define FLD_C_DGLITCH_EN Fld(1,26,AC_MSKB3)//[26:26]
    #define FLD_V_DGLITCH_EN Fld(1,25,AC_MSKB3)//[25:25]
    #define FLD_H_DGLITCH_EN Fld(1,24,AC_MSKB3)//[24:24]
    #define FLD_HRANGE_L Fld(12,12,AC_MSKW21)//[23:12]
    #define FLD_HRANGE_U Fld(12,0,AC_MSKW10)//[11:0]
#define PDWNC_VGA_WAKE_UP_CONTROL_1_REGISTER (IO_VIRT+IO_PDWNC_BASE + 0x404)
    #define FLD_SOG_WAK_DIS Fld(1,31,AC_MSKB3)//[31:31]
    #define FLD_C_SYNC_WAK_DIS Fld(1,30,AC_MSKB3)//[30:30]
    #define FLD_S_SYNC_WAK_DIS Fld(1,29,AC_MSKB3)//[29:29]
    #define FLD_STABLE_RESET Fld(1,28,AC_MSKB3)//[28:28]
    #define FLD_HLEN_VLEN_RESET Fld(1,27,AC_MSKB3)//[27:27]
    #define FLD_RESERVE_VGA01 Fld(1,26,AC_MSKB3)//[26:26]
    #define FLD_WAKEUP_INT_EN Fld(2,24,AC_MSKB3)//[25:24]
    #define FLD_VRANGE_L Fld(12,12,AC_MSKW21)//[23:12]
    #define FLD_VRANGE_U Fld(12,0,AC_MSKW10)//[11:0]
#define PDWNC_VGA_WAKE_UP_CONTROL_2_REGISTER (IO_VIRT+IO_PDWNC_BASE + 0x408)
    #define FLD_VSYNC_SEL Fld(2,30,AC_MSKB3)//[31:30]
    #define FLD_HSYNC_SEL Fld(2,28,AC_MSKB3)//[29:28]
    #define FLD_CSYNC_SOG_SEL Fld(1,27,AC_MSKB3)//[27:27]
    #define FLD_C_ACTEN Fld(1,26,AC_MSKB3)//[26:26]
    #define FLD_V_ACTEN Fld(1,25,AC_MSKB3)//[25:25]
    #define FLD_H_ACTEN Fld(1,24,AC_MSKB3)//[24:24]
    #define FLD_VWIDTH_POL Fld(1,23,AC_MSKB2)//[23:23]
    #define FLD_VWIDTH_EN Fld(1,22,AC_MSKB2)//[22:22]
    #define FLD_VWIDTH_DIFF_TH Fld(6,16,AC_MSKB2)//[21:16]
    #define FLD_VWIDTH_SEL Fld(2,14,AC_MSKB1)//[15:14]
    #define FLD_VWIDTH_STABLE_TH Fld(6,8,AC_MSKB1)//[13:8]
    #define FLD_VWIDTH_U Fld(8,0,AC_FULLB0)//[7:0]
#define PDWNC_VGA_WAKE_UP_CONTROL_3_REGISTER (IO_VIRT+IO_PDWNC_BASE + 0x40C)
    #define FLD_RESERVE_VGA03 Fld(2,30,AC_MSKB3)//[31:30]
    #define FLD_H_DIFF_TH Fld(6,24,AC_MSKB3)//[29:24]
    #define FLD_V_STABLE_TH Fld(12,12,AC_MSKW21)//[23:12]
    #define FLD_H_STABLE_TH Fld(12,0,AC_MSKW10)//[11:0]
#define PDWNC_VGA_WAKE_UP_CONTROL_4_REGISTER (IO_VIRT+IO_PDWNC_BASE + 0x410)
    #define FLD_STATUS_HOLD Fld(1,31,AC_MSKB3)//[31:31]
    #define FLD_RESERVE_VGA04 Fld(1,30,AC_MSKB3)//[30:30]
    #define FLD_V_DIFF_TH Fld(6,24,AC_MSKB3)//[29:24]
    #define FLD_VACT_MP_TH Fld(12,12,AC_MSKW21)//[23:12]
    #define FLD_C_STABLE_TH Fld(12,0,AC_MSKW10)//[11:0]
#define PDWNC_VGA_WAKE_UP_CONTROL_5_REGISTER (IO_VIRT+IO_PDWNC_BASE + 0x414)
    #define FLD_RESERVE_VGA05 Fld(12,20,AC_MSKW32)//[31:20]
    #define FLD_CSYNC_CT_THU Fld(2,18,AC_MSKB2)//[19:18]
    #define FLD_CYSNC_CT_THL Fld(2,16,AC_MSKB2)//[17:16]
    #define FLD_CSYNC_CONT_THU Fld(8,8,AC_FULLB1)//[15:8]
    #define FLD_CSYNC_CONT_THL Fld(8,0,AC_FULLB0)//[7:0]
#define PDWNC_VGA_WAKE_UP_CONTROL_6_REGISTER (IO_VIRT+IO_PDWNC_BASE + 0x418)
    #define FLD_RESERVE_VGA06 Fld(7,25,AC_MSKB3)//[31:25]
    #define FLD_HLEN_USE_VMASKX Fld(1,24,AC_MSKB3)//[24:24]
    #define FLD_VMASK_S Fld(12,12,AC_MSKW21)//[23:12]
    #define FLD_VMASK_E Fld(12,0,AC_MSKW10)//[11:0]
#define PDWNC_VGA_WAKE_UP_CONTROL_7_REGISTER (IO_VIRT+IO_PDWNC_BASE + 0x41C)
    #define FLD_CSYNC_CONT_SLICE_THH Fld(8,24,AC_FULLB3)//[31:24]
    #define FLD_V_IIR_INIT Fld(12,12,AC_MSKW21)//[23:12]
    #define FLD_H_IIR_INIT Fld(12,0,AC_MSKW10)//[11:0]
#define PDWNC_VGA_WAKE_UP_CONTROL_8_REGISTER (IO_VIRT+IO_PDWNC_BASE + 0x420)
    #define FLD_RESERVE_VGA08 Fld(5,27,AC_MSKB3)//[31:27]
    #define FLD_FREERUN_CNT_UNLOCK Fld(1,26,AC_MSKB3)//[26:26]
    #define FLD_CSYNC_CONT_THH Fld(2,24,AC_MSKB3)//[25:24]
    #define FLD_FREERUN_CNT_TH Fld(24,0,AC_MSKDW)//[23:0]
#define PDWNC_VGA_WAKE_UP_CONTROL_9_REGISTER (IO_VIRT+IO_PDWNC_BASE + 0x424)
    #define FLD_RESERVE_VGA09 Fld(5,27,AC_MSKB3)//[31:27]
    #define FLD_HVLEN_BND_EN Fld(1,26,AC_MSKB3)//[26:26]
    #define FLD_HVLEN_BND_TYPE Fld(2,24,AC_MSKB3)//[25:24]
    #define FLD_HLEN_LOBND Fld(12,12,AC_MSKW21)//[23:12]
    #define FLD_HLEN_UPBND Fld(12,0,AC_MSKW10)//[11:0]
#define PDWNC_VGA_WAKE_UP_CONTROL_A_REGISTER (IO_VIRT+IO_PDWNC_BASE + 0x428)
    #define FLD_RESERVE_VGA0A Fld(8,24,AC_FULLB3)//[31:24]
    #define FLD_VLEN_LOBOND Fld(12,12,AC_MSKW21)//[23:12]
    #define FLD_VLEN_UPBOND Fld(12,0,AC_MSKW10)//[11:0]
#define PDWNC_VGA_WAKE_UP_STATUS_0_REGISTER (IO_VIRT+IO_PDWNC_BASE + 0x430)
    #define FLD_SYNC_STABLE Fld(1,31,AC_MSKB3)//[31:31]
    #define FLD_H_STABLE Fld(1,30,AC_MSKB3)//[30:30]
    #define FLD_V_STABLE Fld(1,29,AC_MSKB3)//[29:29]
    #define FLD_CV_STABLE Fld(1,28,AC_MSKB3)//[28:28]
    #define FLD_SOGH_STABLE Fld(1,27,AC_MSKB3)//[27:27]
    #define FLD_CSYNC_ACT Fld(1,26,AC_MSKB3)//[26:26]
    #define FLD_HSYNC_ACT Fld(1,25,AC_MSKB3)//[25:25]
    #define FLD_VSYNC_ACT Fld(1,24,AC_MSKB3)//[24:24]
    #define FLD_H_LEN Fld(12,12,AC_MSKW21)//[23:12]
    #define FLD_V_LEN Fld(12,0,AC_MSKW10)//[11:0]
#define PDWNC_VGA_WAKE_UP_STATUS_1_REGISTER (IO_VIRT+IO_PDWNC_BASE + 0x434)
    #define FLD_VWIDTH_MEAS_7_0 Fld(8,24,AC_FULLB3)//[31:24]
    #define FLD_H_LEN_AVG Fld(12,12,AC_MSKW21)//[23:12]
    #define FLD_V_LEN_AVG Fld(12,0,AC_MSKW10)//[11:0]
#define PDWNC_VGA_WAKE_UP_STATUS_2_REGISTER (IO_VIRT+IO_PDWNC_BASE + 0x438)
    #define FLD_SEP_STABLE Fld(1,31,AC_MSKB3)//[31:31]
    #define FLD_CSYNC_STABLE Fld(1,30,AC_MSKB3)//[30:30]
    #define FLD_SOG_STABLE Fld(1,29,AC_MSKB3)//[29:29]
    #define FLD_SOGV_STABLE Fld(1,28,AC_MSKB3)//[28:28]
    #define FLD_VWIDTH_MEAS_11_8 Fld(4,24,AC_MSKB3)//[27:24]
    #define FLD_CV_LEN_AVG Fld(12,12,AC_MSKW21)//[23:12]
    #define FLD_CV_LEN Fld(12,0,AC_MSKW10)//[11:0]
#define PDWNC_VGA_WAKE_UP_STATUS_3_REGISTER (IO_VIRT+IO_PDWNC_BASE + 0x43C)
    #define FLD_VGA_WAK_MODE_STATUS Fld(2,0,AC_MSKB0)//[1:0]
#if defined(CC_MT5399) 
#define PDWNC_PADCFG17 (IO_VIRT+IO_PDWNC_BASE + 0x6D8)
	#define FLD_CBUS_SMT Fld(1,6,AC_MSKB0)//[6:6]
	#define FLD_CBUS_PUPD Fld(1,5,AC_MSKB0)//[5:5]
	#define FLD_CBUS_IES Fld(1,4,AC_MSKB0)//[4:4]
	#define FLD_CBUS_PU Fld(1,3,AC_MSKB0)//[3:3]
	#define FLD_CBUS_SR Fld(1,2,AC_MSKB0)//[2:2]
	#define FLD_CBUS_E8 Fld(1,1,AC_MSKB0)//[1:1]
	#define FLD_CBUS_E4 Fld(1,0,AC_MSKB0)//[0:0]
#endif
#define PDWNC_YEAR (IO_VIRT+IO_PDWNC_BASE + 0x700)
    #define FLD_YEAR_HI Fld(4,4,AC_MSKB0)//[7:4]
    #define FLD_YEAR_LO Fld(4,0,AC_MSKB0)//[3:0]
#define PDWNC_MONTH (IO_VIRT+IO_PDWNC_BASE + 0x704)
    #define FLD_MN_HI Fld(1,4,AC_MSKB0)//[4:4]
    #define FLD_MN_LO Fld(4,0,AC_MSKB0)//[3:0]
#define PDWNC_DAY (IO_VIRT+IO_PDWNC_BASE + 0x708)
    #define FLD_DAY_HI Fld(2,4,AC_MSKB0)//[5:4]
    #define FLD_DAY_LO Fld(4,0,AC_MSKB0)//[3:0]
#define PDWNC_WEEK (IO_VIRT+IO_PDWNC_BASE + 0x70C)
    #define FLD_WEEK Fld(3,0,AC_MSKB0)//[2:0]
#define PDWNC_HOUR (IO_VIRT+IO_PDWNC_BASE + 0x710)
    #define FLD_HR_HI Fld(2,4,AC_MSKB0)//[5:4]
    #define FLD_HR_LO Fld(4,0,AC_MSKB0)//[3:0]
#define PDWNC_MINUTE (IO_VIRT+IO_PDWNC_BASE + 0x714)
    #define FLD_MIN_HI Fld(3,4,AC_MSKB0)//[6:4]
    #define FLD_MIN_LO Fld(4,0,AC_MSKB0)//[3:0]
#define PDWNC_SECOND (IO_VIRT+IO_PDWNC_BASE + 0x718)
    #define FLD_SEC_HI Fld(3,4,AC_MSKB0)//[6:4]
    #define FLD_SEC_LO Fld(4,0,AC_MSKB0)//[3:0]
#define PDWNC_AYEAR (IO_VIRT+IO_PDWNC_BASE + 0x71C)
    #define FLD_AYEAR_HI Fld(4,4,AC_MSKB0)//[7:4]
    #define FLD_AYEAR_LO Fld(4,0,AC_MSKB0)//[3:0]
#define PDWNC_AMONTH (IO_VIRT+IO_PDWNC_BASE + 0x720)
    #define FLD_AMN_HI Fld(1,4,AC_MSKB0)//[4:4]
    #define FLD_AMN_LO Fld(4,0,AC_MSKB0)//[3:0]
#define PDWNC_ADAY (IO_VIRT+IO_PDWNC_BASE + 0x724)
    #define FLD_ADAY_HI Fld(2,4,AC_MSKB0)//[5:4]
    #define FLD_ADAY_LO Fld(4,0,AC_MSKB0)//[3:0]
#define PDWNC_AWEEK (IO_VIRT+IO_PDWNC_BASE + 0x728)
    #define FLD_AWEEK Fld(3,0,AC_MSKB0)//[2:0]
#define PDWNC_AHOUR (IO_VIRT+IO_PDWNC_BASE + 0x72C)
    #define FLD_AHR_HI Fld(2,4,AC_MSKB0)//[5:4]
    #define FLD_AHR_LO Fld(4,0,AC_MSKB0)//[3:0]
#define PDWNC_AMINUTE (IO_VIRT+IO_PDWNC_BASE + 0x730)
    #define FLD_AMIN_HI Fld(3,4,AC_MSKB0)//[6:4]
    #define FLD_AMIN_LO Fld(4,0,AC_MSKB0)//[3:0]
#define PDWNC_ASECOND (IO_VIRT+IO_PDWNC_BASE + 0x734)
    #define FLD_ASEC_HI Fld(3,4,AC_MSKB0)//[6:4]
    #define FLD_ASEC_LO Fld(4,0,AC_MSKB0)//[3:0]
#define PDWNC_ALMINT (IO_VIRT+IO_PDWNC_BASE + 0x738)
    #define FLD_ACMP_EN Fld(7,1,AC_MSKB0)//[7:1]
    #define FLD_AI_EN Fld(1,0,AC_MSKB0)//[0:0]
#define PDWNC_RTCCTL (IO_VIRT+IO_PDWNC_BASE + 0x73C)
    #define FLD_FLAG Fld(1,7,AC_MSKB0)//[7:7]
    #define FLD_PASS Fld(1,6,AC_MSKB0)//[6:6]
    #define FLD_RTCSIM Fld(1,4,AC_MSKB0)//[4:4]
    #define FLD_H24 Fld(1,1,AC_MSKB0)//[1:1]
    #define FLD_STOP Fld(1,0,AC_MSKB0)//[0:0]
#define PDWNC_LEAPYEAR (IO_VIRT+IO_PDWNC_BASE + 0x744)
    #define FLD_LY_REG Fld(2,0,AC_MSKB0)//[1:0]
#define PDWNC_PROT (IO_VIRT+IO_PDWNC_BASE + 0x748)
    #define FLD_PROT Fld(8,0,AC_FULLB0)//[7:0]
#define PDWNC_AWEEK2 (IO_VIRT+IO_PDWNC_BASE + 0x750)
    #define FLD_AWEEK_2 Fld(3,0,AC_MSKB0)//[2:0]
#define PDWNC_AWEEK3 (IO_VIRT+IO_PDWNC_BASE + 0x754)
    #define FLD_AWEEK_3 Fld(3,0,AC_MSKB0)//[2:0]
#define PDWNC_AWEEK4 (IO_VIRT+IO_PDWNC_BASE + 0x758)
    #define FLD_AWEEK_4 Fld(3,0,AC_MSKB0)//[2:0]
#define PDWNC_AWEEK5 (IO_VIRT+IO_PDWNC_BASE + 0x75C)
    #define FLD_AWEEK_5 Fld(3,0,AC_MSKB0)//[2:0]
#define PDWNC_AWEEK6 (IO_VIRT+IO_PDWNC_BASE + 0x760)
    #define FLD_AWEEK_6 Fld(3,0,AC_MSKB0)//[2:0]
#define PDWNC_AWEEK7 (IO_VIRT+IO_PDWNC_BASE + 0x764)
    #define FLD_AWEEK_7 Fld(3,0,AC_MSKB0)//[2:0]
#define PDWNC_AWEEK_ENABLE (IO_VIRT+IO_PDWNC_BASE + 0x768)
    #define FLD_AWEEK_ENABLE Fld(7,0,AC_MSKB0)//[6:0]



//rs232
#ifdef CC_MT8223
#define PDWNC_DATAW (0x200)
	#define FLD_DATA Fld(32, 0, AC_FULLDW)//31:0
	
#define PDWNC_STAB (0x201)
	#define FLD_TRANS_MODE Fld(1, 6, AC_MSKB0)//6
	#define FLD_PARITY_ERR Fld(1, 5, AC_MSKB0)//5
	#define FLD_END_ERR Fld(1, 4, AC_MSKB0)//4
	#define FLD_WR_ALLOW Fld(1, 1, AC_MSKB0)//1//
	#define FLD_RD_ALLOW Fld(1, 0, AC_MSKB0)//0
	
#else
#define PDWNC_DATAW (IO_VIRT+IO_PDWNC_BASE+0x800)
		#define FLD_DATA Fld(32, 0, AC_FULLDW)//31:0
#define PDWNC_STAB (IO_VIRT+IO_PDWNC_BASE+0x804)
		#define FLD_TRANS_MODE Fld(1, 6, AC_MSKB0)//6
		#define FLD_PARITY_ERR Fld(1, 5, AC_MSKB0)//5
		#define FLD_END_ERR Fld(1, 4, AC_MSKB0)//4
		#define FLD_WR_ALLOW Fld(1, 1, AC_MSKB0)//1//
		#define FLD_RD_ALLOW Fld(1, 0, AC_MSKB0)//0
#endif
	
#define PDWNC_UART_PD_INT_EN (IO_VIRT+IO_PDWNC_BASE+0x80C)
		#define FLD_T8032DATAE Fld(1, 7, AC_MSKB0)//7
		#define FLD_RISCDATAE Fld(1, 6, AC_MSKB0)//6
		#define FLD_OVRUN_EN Fld(1, 5, AC_MSKB0)//5
	#if !defined(CC_MT5880)
		#define FLD_MRIE Fld(1, 4, AC_MSKB0)//4
	#endif
		#define FLD_TBUFE Fld(1, 3, AC_MSKB0)//3
		#define FLD_TOUTE Fld(1, 2, AC_MSKB0)//2
		#define FLD_RBUFE Fld(1, 1, AC_MSKB0)//1
		#define FLD_RXD_ERRE Fld(1, 0, AC_MSKB0)//0
#define PDWNC_INT_ID (IO_VIRT+IO_PDWNC_BASE+0x810)
		#define FLD_T8032DATA Fld(1, 7, AC_MSKB0)//7
		#define FLD_RISCDATA Fld(1, 6, AC_MSKB0)//6
		#define FLD_OVRUN Fld(1, 5, AC_MSKB0)//5
		#define FLD_MRI Fld(1, 4, AC_MSKB0)//4
		#define FLD_TBUF Fld(1, 3, AC_MSKB0)//3
		#define FLD_TOUT Fld(1, 2, AC_MSKB0)//2
		#define FLD_RBUF Fld(1, 1, AC_MSKB0)//1
		#define FLD_RXD_ERR Fld(1, 0, AC_MSKB0)//0
#define PDWNC_RS232_MODE (IO_VIRT+IO_PDWNC_BASE+0x818)
	#if !defined(CC_MT5880)
		#define FLD_PDGPIO_AS_UART Fld(1, 2, AC_MSKB0)//2
	#endif
		#define FLD_RS232_MODE Fld(2, 0, AC_MSKB0)//1:0
#define PDWNC_BUFFER_SIZE (IO_VIRT+IO_PDWNC_BASE+0x81C)
		#define FLD_BUFFER_SIZE Fld(2, 0, AC_MSKB0)//1:0
#define PDWNC_CCR (IO_VIRT+IO_PDWNC_BASE+0x830)
		#define FLD_BAUD_RATE Fld(4, 8, AC_MSKB1)//11:8
		#define FLD_BREAK Fld(1, 7, AC_MSKB0)//7
		#define FLD_P_STICK Fld(1, 6, AC_MSKB0)//6
		#define FLD_P_EVEN Fld(1, 5, AC_MSKB0)//5
		#define FLD_CHECK_PARITY Fld(1, 4, AC_MSKB0)//4
    #define FLD_DIS_RX_ZERO Fld(1,3,AC_MSKB0)//[3:3]
		#define FLD_STOP_BIT Fld(1, 2, AC_MSKB0)//2
		#define FLD_BIT_LENGTH Fld(2, 0, AC_MSKB0)//1:0
#define PDWNC_UART_PD_STATUS (IO_VIRT+IO_PDWNC_BASE+0x834)
		#define FLD_BREAK_IN4 Fld(1, 15, AC_MSKB1)//15
		#define FLD_END_ERR4 Fld(1, 14, AC_MSKB1)//14
		#define FLD_P_ERR4 Fld(1, 13, AC_MSKB1)//13
		#define FLD_TXD_BUF_STATE Fld(5, 8, AC_MSKB1)//12:8
		#define FLD_BREAK_IN Fld(1, 7, AC_MSKB0)//7
		#define FLD_END_CHECK_ERR Fld(1, 6, AC_MSKB0)//6
		#define FLD_P_ERR Fld(1, 5, AC_MSKB0)//5
		#define FLD_RXD_BUF_STATE Fld(5, 0, AC_MSKB0)//4:0
#define PDWNC_BCR (IO_VIRT+IO_PDWNC_BASE+0x838)
		#define FLD_TIME_OUT_CYCLE Fld(8, 8, AC_FULLB1)//15:8
		#define FLD_CLEAR_TBUF Fld(1, 7, AC_MSKB0)//7
		#define FLD_CLEAR_RBUF Fld(1, 6, AC_MSKB0)//6
		#define FLD_RXD_BUF_TRIG_LEVEL Fld(5, 0, AC_MSKB0)//4:0


	
#define PDWNC_CONCFG0 (IO_VIRT+IO_PDWNC_BASE + 0x900)
    #define FLD_RG_PDREV_1 Fld(8,16,AC_FULLB2)//[23:16]
    #define FLD_RG_PDREV_0 Fld(8,0,AC_FULLB0)//[7:0]
    #define FLD_RG_DMSUS_RESERVED Fld(1,16,AC_MSKB2)//[16:16]
#define PDWNC_CONCFG1 (IO_VIRT+IO_PDWNC_BASE + 0x904)
    #define FLD_RG_DMSUS Fld(1,0,AC_MSKB0)//[0:0]
#define PDWNC_LDOCFG0 (IO_VIRT+IO_PDWNC_BASE + 0x908)
    #define FLD_RG_LDO_BIAS Fld(2,12,AC_MSKB1)//[13:12]
    #define FLD_RG_LDO_IPBIAS Fld(2,10,AC_MSKB1)//[11:10]
    #define FLD_RG_LDO_OPBIAS Fld(2,8,AC_MSKB1)//[9:8]
    #define FLD_RG_LDO_ABIST_PWD Fld(1,1,AC_MSKB0)//[1:1]
    #define FLD_RG_LDO_TESTE Fld(1,0,AC_MSKB0)//[0:0]
#define PDWNC_SADCCFG0 (IO_VIRT+IO_PDWNC_BASE + 0x90C)
    #define FLD_RG_BIAS_OPTION Fld(3,21,AC_MSKB2)//[23:21]
    #define FLD_RG_SRV_INBUFF_PD Fld(1,11,AC_MSKB1)//[11:11]
    #define FLD_RG_SRV_INBUFF_BP Fld(1,10,AC_MSKB1)//[10:10]
    #define FLD_RG_SRV_OOSEN Fld(1,9,AC_MSKB1)//[9:9]
    #define FLD_RG_SRV_PWD_B Fld(1,8,AC_MSKB1)//[8:8]
    #define FLD_RG_SRVADC_TESTIN Fld(4,4,AC_MSKB0)//[7:4]
    #define FLD_RG_SRV_RES Fld(1,3,AC_MSKB0)//[3:3]
#define PDWNC_SADCCFG1 (IO_VIRT+IO_PDWNC_BASE + 0x910)
    #define FLD_RG_SRVADC_TESTE Fld(1,31,AC_MSKB3)//[31:31]
    #define FLD_RG_TPBUFF_EN Fld(1,30,AC_MSKB3)//[30:30]
    #define FLD_RG_TNPASS_EN Fld(1,29,AC_MSKB3)//[29:29]
    #define FLD_RG_TPBUFF_SEL Fld(4,25,AC_MSKB3)//[28:25]
#define PDWNC_VGACFG0 (IO_VIRT+IO_PDWNC_BASE + 0x914)
    #define FLD_RG_VMUX_PWD Fld(1,31,AC_MSKB3)//[31:31]
    #define FLD_RG_RGB_EN Fld(1,30,AC_MSKB3)//[30:30]
    #define FLD_RG_HDTV1_EN Fld(1,29,AC_MSKB3)//[29:29]
    #define FLD_RG_HDTV0_EN Fld(1,28,AC_MSKB3)//[28:28]
    #define FLD_RGBHDTV01_EN Fld(3, 28, AC_MSKB3)  //[30:28]   
    #define FLD_RG_SC_SCART Fld(1,26,AC_MSKB3)//[26:26]
    #define FLD_RG_DESPIKE Fld(1,25,AC_MSKB3)//[25:25]
    #define FLD_RG_AUTOBIAS_EN Fld(1,24,AC_MSKB3)//[24:24]
    #define FLD_RG_CVBSINVGA Fld(2,20,AC_MSKB2)//[21:20]
    #define FLD_RG_RGB_REVERSE Fld(1,18,AC_MSKB2)//[18:18]
    #define FLD_RG_HDTV1_REVERSE Fld(1,17,AC_MSKB2)//[17:17]
    #define FLD_RG_HDTV0_REVERSE Fld(1,16,AC_MSKB2)//[16:16]
    #define FLD_RG_HDMIIB_PWD Fld(1,3,AC_MSKB0)//[3:3]
    #define FLD_RG_VDACIB_PWD Fld(1,2,AC_MSKB0)//[2:2]
    #define VGAMUX_RGB_EN 4
    #define VGAMUX_HDTV1_EN 2
    #define VGAMUX_HDTV0_EN 1
    #define FLD_RG_HSYNC_GPI_EN Fld(1,1,AC_MSKB0)//[1:1]
    #define FLD_RG_VSYNC_GPI_EN Fld(1,0,AC_MSKB0)//[0:0]
#define PDWNC_VGACFG1 (IO_VIRT+IO_PDWNC_BASE + 0x918)
    #define FLD_RG_VSYNC_EN Fld(1,29,AC_MSKB3)//[29:29]
    #define FLD_RG_HSYNC_EN Fld(1,28,AC_MSKB3)//[28:28]
    #define FLD_RG_CVBS_EN Fld(1,27,AC_MSKB3)//[27:27]
    #define FLD_RG_SCART_EN Fld(3,24,AC_MSKB3)//[26:24]
    #define FLD_RG_SOY1_EN Fld(1,22,AC_MSKB2)//[22:22]
    #define FLD_RG_SOY0_EN Fld(1,21,AC_MSKB2)//[21:21]
    #define FLD_SOY1_SOY0_EN Fld(2, 21, AC_MSKB2) //[22:21]
    #define FLD_RG_SOG_EN Fld(1,20,AC_MSKB2)//[20:20]
    #define FLD_RG_SYNC_PWD Fld(1,18,AC_MSKB2)//[18:18]
    #define FLD_RG_SYNC_DESPK_EN Fld(1,17,AC_MSKB2)//[17:17]
    #define FLD_RG_FB_AB_EN Fld(1,16,AC_MSKB2)//[16:16]
    #define FLD_RG_BYPS_SYNCPROSR Fld(1,13,AC_MSKB1)//[13:13]
    #define FLD_RG_BYPS_SOGYPGA Fld(1,12,AC_MSKB1)//[12:12]
    #define FLD_RG_VGA_TEST_EN Fld(1,11,AC_MSKB1)//[11:11]
    #define FLD_RG_SYNC_TESTO_EN Fld(1,10,AC_MSKB1)//[10:10]
    #define FLD_RG_TSOGY_EN Fld(1,9,AC_MSKB1)//[9:9]
    #define FLD_RG_DIG_TST_EN Fld(1,8,AC_MSKB1)//[8:8]
    #define FLD_RG_SYNC_TSTSEL Fld(4,4,AC_MSKB0)//[7:4]
#define PDWNC_VGACFG2 (IO_VIRT+IO_PDWNC_BASE + 0x91C)
    #define FLD_RG_VREFMON Fld(8,24,AC_FULLB3)//[31:24]
    #define FLD_RG_FEOUT_MONEN Fld(1,23,AC_MSKB2)//[23:23]
    #define FLD_RG_CLAMP_MONEN Fld(1,22,AC_MSKB2)//[22:22]
    #define FLD_RG_VGAVREF_MONEN Fld(1,21,AC_MSKB2)//[21:21]
    #define FLD_RG_VGA_TESTBUF_PWD Fld(1,20,AC_MSKB2)//[20:20]
    #define FLD_RG_VREF_CVBS Fld(2,18,AC_MSKB2)//[19:18]
    #define FLD_RG_SHIFT_PWD Fld(1,17,AC_MSKB2)//[17:17]
    #define FLD_RG_OFFCUR_PWD Fld(1,16,AC_MSKB2)//[16:16]
    #define FLD_RG_OFFCUR Fld(4,12,AC_MSKB1)//[15:12]
    #define FLD_RG_PSAV_REV Fld(12,0,AC_MSKW10)//[11:0]
    #define FLD_RG_PSAV_REV_BG Fld(2,6,AC_MSKB0)//[7:6]
    #define FLD_RG_PSAV_REV_INPUT_PORT Fld(1,4,AC_MSKB0)//[4:4]
#define PDWNC_VGACFG3 (IO_VIRT+IO_PDWNC_BASE + 0x920)
    #define FLD_RG_SOGY_SINK Fld(6,24,AC_MSKB3)//[29:24]
    #define FLD_RG_SOGY_SOURCE Fld(4,20,AC_MSKB2)//[23:20]
    #define FLD_RG_SOGY_SINK_PWD Fld(1,17,AC_MSKB2)//[17:17]
    #define FLD_RG_SOGY_SORS_PWD Fld(1,16,AC_MSKB2)//[16:16]
    #define FLD_RG_SOGY_RGAIN Fld(2,12,AC_MSKB1)//[13:12]
    #define FLD_RG_SOGY_BW Fld(2,8,AC_MSKB1)//[9:8]
    #define FLD_RG_CLAMPREFSEL Fld(2,4,AC_MSKB0)//[5:4]
#define PDWNC_VGACFG4 (IO_VIRT+IO_PDWNC_BASE + 0x924)
    #define FLD_RG_SYNCREV Fld(16,16,AC_FULLW32)//[31:16]
    #define FLD_RG_SYNC1_VTH Fld(4,12,AC_MSKB1)//[15:12]
    #define FLD_RG_SYNC1_VTL Fld(4,8,AC_MSKB1)//[11:8]
    #define FLD_RG_SYNC2_VTH Fld(4,4,AC_MSKB0)//[7:4]
    #define FLD_RG_SYNC2_VTL Fld(4,0,AC_MSKB0)//[3:0]
    #define FLD_RG_SYNC2_VHLSEL Fld(8,0, AC_MSKB0) //[7:0]
#define PDWNC_VGACFG5 (IO_VIRT+IO_PDWNC_BASE + 0x928)
    #define FLD_RG_SYNCREV2 Fld(16,0,AC_FULLW10)//[15:0]
#define PDWNC_XTALCFG0 (IO_VIRT+IO_PDWNC_BASE + 0x92C)
    #define FLD_RG_XTAL_GM Fld(4,16,AC_MSKB2)//[19:16]
    #define FLD_RG_XTAL_RESERVED Fld(8,8,AC_FULLB1)//[15:8]
    #define FLD_RG_XTAL_BIASR Fld(1,3,AC_MSKB0)//[3:3]
    #define FLD_RG_XTAL_TMEN Fld(1,2,AC_MSKB0)//[2:2]
    #define FLD_RG_XTAL_RC_BYPASS Fld(1,1,AC_MSKB0)//[1:1]
    #define FLD_RG_XTAL_AAC_EN Fld(1,0,AC_MSKB0)//[0:0]
#define PDWNC_ETHLDOCFG0 (IO_VIRT+IO_PDWNC_BASE + 0x930)
    #define FLD_RG_ETHLDO_TESTE Fld(1,31,AC_MSKB3)//[31:31]
    #define FLD_RG_ETHLDO_PWD Fld(1,30,AC_MSKB3)//[30:30]
#define PDWNC_DDCCI_CFG (IO_VIRT+IO_PDWNC_BASE + 0xA00)
    #define FLD_DDCCI_DE_CNT Fld(8,24,AC_FULLB3)//[31:24]
    #define FLD_DDCCI_CNT_RST Fld(1,19,AC_MSKB2)//[19:19]
    #define FLD_DDCCI_ODRAIN Fld(1,18,AC_MSKB2)//[18:18]
    #define FLD_DDCCI_DEG_EN Fld(1,17,AC_MSKB2)//[17:17]
    #define FLD_DDCCI_OFF Fld(1,16,AC_MSKB2)//[16:16]
    #define FLD_DDCCI_DATALEN Fld(8,8,AC_FULLB1)//[15:8]
    #define FLD_DDCCI_DEV_ADDR Fld(7,1,AC_MSKB0)//[7:1]
#define PDWNC_DDCCI_INTEN (IO_VIRT+IO_PDWNC_BASE + 0xA04)
    #define FLD_INT_R16_EN Fld(1,5,AC_MSKB0)//[5:5]
    #define FLD_INT_R8_EN Fld(1,4,AC_MSKB0)//[4:4]
    #define FLD_INT_RDONE_EN Fld(1,3,AC_MSKB0)//[3:3]
    #define FLD_INT_W16_EN Fld(1,2,AC_MSKB0)//[2:2]
    #define FLD_INT_W8_EN Fld(1,1,AC_MSKB0)//[1:1]
    #define FLD_INT_WDONE_EN Fld(1,0,AC_MSKB0)//[0:0]
#define PDWNC_DDCCI_INTCLR (IO_VIRT+IO_PDWNC_BASE + 0xA08)
    #define FLD_INT_R16_CLR Fld(1,5,AC_MSKB0)//[5:5]
    #define FLD_INT_R8_CLR Fld(1,4,AC_MSKB0)//[4:4]
    #define FLD_INT_RDONE_CLR Fld(1,3,AC_MSKB0)//[3:3]
    #define FLD_INT_W16_CLR Fld(1,2,AC_MSKB0)//[2:2]
    #define FLD_INT_W8_CLR Fld(1,1,AC_MSKB0)//[1:1]
    #define FLD_INT_WDONE_CLR Fld(1,0,AC_MSKB0)//[0:0]
#define PDWNC_DDCCI_INTST (IO_VIRT+IO_PDWNC_BASE + 0xA0C)
    #define FLD_INT_R16_ST Fld(1,5,AC_MSKB0)//[5:5]
    #define FLD_INT_R8_ST Fld(1,4,AC_MSKB0)//[4:4]
    #define FLD_INT_RDONE_ST Fld(1,3,AC_MSKB0)//[3:3]
    #define FLD_INT_W16_ST Fld(1,2,AC_MSKB0)//[2:2]
    #define FLD_INT_W8_ST Fld(1,1,AC_MSKB0)//[1:1]
    #define FLD_INT_WDONE_ST Fld(1,0,AC_MSKB0)//[0:0]
#define PDWNC_DDCCI_WDATA0 (IO_VIRT+IO_PDWNC_BASE + 0xA10)
    #define FLD_WDATA0 Fld(32,0,AC_FULLDW)//[31:0]
#define PDWNC_DDCCI_WDATA1 (IO_VIRT+IO_PDWNC_BASE + 0xA14)
    #define FLD_WDATA1 Fld(32,0,AC_FULLDW)//[31:0]
#define PDWNC_DDCCI_WDATA2 (IO_VIRT+IO_PDWNC_BASE + 0xA18)
    #define FLD_WDATA2 Fld(32,0,AC_FULLDW)//[31:0]
#define PDWNC_DDCCI_WDATA3 (IO_VIRT+IO_PDWNC_BASE + 0xA1C)
    #define FLD_WDATA3 Fld(32,0,AC_FULLDW)//[31:0]
#define PDWNC_DDCCI_RDATA0 (IO_VIRT+IO_PDWNC_BASE + 0xA20)
    #define FLD_RDATA0 Fld(32,0,AC_FULLDW)//[31:0]
#define PDWNC_DDCCI_RDATA1 (IO_VIRT+IO_PDWNC_BASE + 0xA24)
    #define FLD_RDATA1 Fld(32,0,AC_FULLDW)//[31:0]
#define PDWNC_DDCCI_RDATA2 (IO_VIRT+IO_PDWNC_BASE + 0xA28)
    #define FLD_RDATA2 Fld(32,0,AC_FULLDW)//[31:0]
#define PDWNC_DDCCI_RDATA3 (IO_VIRT+IO_PDWNC_BASE + 0xA2C)
    #define FLD_RDATA3 Fld(32,0,AC_FULLDW)//[31:0]
#define PDWNC_GSPI_DATA0 (IO_VIRT+IO_PDWNC_BASE + 0xB00)
    #define FLD_GSPI_DATA0 Fld(32,0,AC_FULLDW)//[31:0]
#define PDWNC_GSPI_DATA1 (IO_VIRT+IO_PDWNC_BASE + 0xB04)
    #define FLD_GSPI_DATA1 Fld(32,0,AC_FULLDW)//[31:0]
#define PDWNC_GSPI_DATA2 (IO_VIRT+IO_PDWNC_BASE + 0xB08)
    #define FLD_GSPI_DATA2 Fld(32,0,AC_FULLDW)//[31:0]
#define PDWNC_GSPI_DATA3 (IO_VIRT+IO_PDWNC_BASE + 0xB0C)
    #define FLD_GSPI_DATA3 Fld(32,0,AC_FULLDW)//[31:0]
#define PDWNC_GSPI_DATA4 (IO_VIRT+IO_PDWNC_BASE + 0xB10)
    #define FLD_GSPI_DATA4 Fld(32,0,AC_FULLDW)//[31:0]
#define PDWNC_GSPI_DATA5 (IO_VIRT+IO_PDWNC_BASE + 0xB14)
    #define FLD_GSPI_DATA5 Fld(32,0,AC_FULLDW)//[31:0]
#define PDWNC_GSPI_DATA6 (IO_VIRT+IO_PDWNC_BASE + 0xB18)
    #define FLD_GSPI_DATA6 Fld(32,0,AC_FULLDW)//[31:0]
#define PDWNC_GSPI_DATA7 (IO_VIRT+IO_PDWNC_BASE + 0xB1C)
    #define FLD_GSPI_DATA7 Fld(32,0,AC_FULLDW)//[31:0]
#define PDWNC_GSPI_CKCFG (IO_VIRT+IO_PDWNC_BASE + 0xB20)
    #define FLD_CLK_DIV Fld(12,16,AC_MSKW32)//[27:16]
    #define FLD_CPOL Fld(1,4,AC_MSKB0)//[4:4]
    #define FLD_CPHA Fld(1,0,AC_MSKB0)//[0:0]
#define PDWNC_GSPI_CSACC (IO_VIRT+IO_PDWNC_BASE + 0xB24)
    #define FLD_CS_LEAD_LATENCY Fld(12,16,AC_MSKW32)//[27:16]
    #define FLD_CS_LAG_LATENCY Fld(12,0,AC_MSKW10)//[11:0]
#define PDWNC_GSPI_CSACC2 (IO_VIRT+IO_PDWNC_BASE + 0xB28)
    #define FLD_CS_HOLD_LATENCY Fld(12,0,AC_MSKW10)//[11:0]
#define PDWNC_GSPI_NOB (IO_VIRT+IO_PDWNC_BASE + 0xB30)
    #define FLD_NDATA_LEN Fld(8,16,AC_FULLB2)//[23:16]
    #define FLD_MDATA_LEN Fld(8,0,AC_FULLB0)//[7:0]
#define PDWNC_GSPI_CON (IO_VIRT+IO_PDWNC_BASE + 0xB34)
    #define FLD_LSB_MODE Fld(1,4,AC_MSKB0)//[4:4]
    #define FLD_GSPI_EN Fld(1,1,AC_MSKB0)//[1:1]
    #define FLD_GSPI_TRIG Fld(1,0,AC_MSKB0)//[0:0]
#define PDWNC_GSPI_INTR_EN (IO_VIRT+IO_PDWNC_BASE + 0xB40)
    #define FLD_GSPI_DONE_INTR_EN Fld(1,0,AC_MSKB0)//[0:0]
#define PDWNC_GSPI_INTR (IO_VIRT+IO_PDWNC_BASE + 0xB44)
    #define FLD_GSPI_DONE_INTR Fld(1,0,AC_MSKB0)//[0:0]
#define PDWNC_GSPI_SHREG0 (IO_VIRT+IO_PDWNC_BASE + 0xB50)
    #define FLD_GSPI_SHREG0 Fld(32,0,AC_FULLDW)//[31:0]
#define PDWNC_GSPI_SHREG1 (IO_VIRT+IO_PDWNC_BASE + 0xB54)
    #define FLD_GSPI_SHREG1 Fld(32,0,AC_FULLDW)//[31:0]
#define PDWNC_GSPI_SHREG2 (IO_VIRT+IO_PDWNC_BASE + 0xB58)
    #define FLD_GSPI_SHREG2 Fld(32,0,AC_FULLDW)//[31:0]
#define PDWNC_GSPI_SHREG3 (IO_VIRT+IO_PDWNC_BASE + 0xB5C)
    #define FLD_GSPI_SHREG3 Fld(32,0,AC_FULLDW)//[31:0]
#define PDWNC_GSPI_SHREG4 (IO_VIRT+IO_PDWNC_BASE + 0xB60)
    #define FLD_GSPI_SHREG4 Fld(32,0,AC_FULLDW)//[31:0]
#define PDWNC_GSPI_SHREG5 (IO_VIRT+IO_PDWNC_BASE + 0xB64)
    #define FLD_GSPI_SHREG5 Fld(32,0,AC_FULLDW)//[31:0]
#define PDWNC_GSPI_SHREG6 (IO_VIRT+IO_PDWNC_BASE + 0xB68)
    #define FLD_GSPI_SHREG6 Fld(32,0,AC_FULLDW)//[31:0]
#define PDWNC_GSPI_SHREG7 (IO_VIRT+IO_PDWNC_BASE + 0xB6C)
    #define FLD_GSPI_SHREG7 Fld(32,0,AC_FULLDW)//[31:0]
#if defined(CC_MT5882)
#define PDWNC_SM0CTL2 (IO_VIRT+IO_PDWNC_BASE + 0xC00)
    #define FLD_SIFM0_SCL_RISE_VALUE Fld(12,16,AC_MSKW32)//[27:16]
    #define FLD_SIFM0_SCL_MODE Fld(1,0,AC_MSKB0)//[0:0]
#define PDWNC_SM0CTL3 (IO_VIRT+IO_PDWNC_BASE + 0xC04)
    #define FLD_SIFM0_ACK1 Fld(21,11,AC_MSKDW)//[31:11]
    #define FLD_SIFM0_WD_STOP_MODE Fld(1,6,AC_MSKB0)//[6:6]
    #define FLD_SIFM0_STR_WD_MODE Fld(1,5,AC_MSKB0)//[5:5]
    #define FLD_SIFM0_STR_WD_STOP_MODE Fld(1,4,AC_MSKB0)//[4:4]
    #define FLD_SIFM0_RDNACK_STOP_MODE Fld(1,3,AC_MSKB0)//[3:3]
    #define FLD_SIFM0_STR_RDACK_MODE Fld(1,2,AC_MSKB0)//[2:2]
    #define FLD_SIFM0_STR_RDNACK_STOP_MODE Fld(1,1,AC_MSKB0)//[1:1]
    #define FLD_SIFM0_SDEV_MODE Fld(1,0,AC_MSKB0)//[0:0]
#define PDWNC_SM0D2 (IO_VIRT+IO_PDWNC_BASE + 0xC08)
    #define FLD_SIFM0_DATA11 Fld(8,24,AC_FULLB3)//[31:24]
    #define FLD_SIFM0_DATA10 Fld(8,16,AC_FULLB2)//[23:16]
    #define FLD_SIFM0_DATA9 Fld(8,8,AC_FULLB1)//[15:8]
    #define FLD_SIFM0_DATA8 Fld(8,0,AC_FULLB0)//[7:0]
#define PDWNC_SM0D3 (IO_VIRT+IO_PDWNC_BASE + 0xC0C)
    #define FLD_SIFM0_DATA15 Fld(8,24,AC_FULLB3)//[31:24]
    #define FLD_SIFM0_DATA14 Fld(8,16,AC_FULLB2)//[23:16]
    #define FLD_SIFM0_DATA13 Fld(8,8,AC_FULLB1)//[15:8]
    #define FLD_SIFM0_DATA12 Fld(8,0,AC_FULLB0)//[7:0]
#define PDWNC_SM0D4 (IO_VIRT+IO_PDWNC_BASE + 0xC10)
    #define FLD_SIFM0_DATA19 Fld(8,24,AC_FULLB3)//[31:24]
    #define FLD_SIFM0_DATA18 Fld(8,16,AC_FULLB2)//[23:16]
    #define FLD_SIFM0_DATA17 Fld(8,8,AC_FULLB1)//[15:8]
    #define FLD_SIFM0_DATA16 Fld(8,0,AC_FULLB0)//[7:0]
#define PDWNC_SM0D5 (IO_VIRT+IO_PDWNC_BASE + 0xC14)
    #define FLD_SIFM0_DATA23 Fld(8,24,AC_FULLB3)//[31:24]
    #define FLD_SIFM0_DATA22 Fld(8,16,AC_FULLB2)//[23:16]
    #define FLD_SIFM0_DATA21 Fld(8,8,AC_FULLB1)//[15:8]
    #define FLD_SIFM0_DATA20 Fld(8,0,AC_FULLB0)//[7:0]
#define PDWNC_SM0D6 (IO_VIRT+IO_PDWNC_BASE + 0xC18)
    #define FLD_SIFM0_DATA27 Fld(8,24,AC_FULLB3)//[31:24]
    #define FLD_SIFM0_DATA26 Fld(8,16,AC_FULLB2)//[23:16]
    #define FLD_SIFM0_DATA25 Fld(8,8,AC_FULLB1)//[15:8]
    #define FLD_SIFM0_DATA24 Fld(8,0,AC_FULLB0)//[7:0]
#define PDWNC_SM0D7 (IO_VIRT+IO_PDWNC_BASE + 0xC1C)
    #define FLD_SIFM0_DATA31 Fld(8,24,AC_FULLB3)//[31:24]
    #define FLD_SIFM0_DATA30 Fld(8,16,AC_FULLB2)//[23:16]
    #define FLD_SIFM0_DATA29 Fld(8,8,AC_FULLB1)//[15:8]
    #define FLD_SIFM0_DATA28 Fld(8,0,AC_FULLB0)//[7:0]
#define PDWNC_SM0D8 (IO_VIRT+IO_PDWNC_BASE + 0xC20)
    #define FLD_SIFM0_DATA35 Fld(8,24,AC_FULLB3)//[31:24]
    #define FLD_SIFM0_DATA34 Fld(8,16,AC_FULLB2)//[23:16]
    #define FLD_SIFM0_DATA33 Fld(8,8,AC_FULLB1)//[15:8]
    #define FLD_SIFM0_DATA32 Fld(8,0,AC_FULLB0)//[7:0]
#define PDWNC_SM0SDEV (IO_VIRT+IO_PDWNC_BASE + 0xC24)
    #define FLD_SIFM0_DEV_ADDR Fld(8,0,AC_FULLB0)//[7:0]
#define PDWNC_SM1CTL2 (IO_VIRT+IO_PDWNC_BASE + 0xC28)
    #define FLD_SIFM1_SCL_RISE_VALUE Fld(12,16,AC_MSKW32)//[27:16]
    #define FLD_SIFM1_SCL_MODE Fld(1,0,AC_MSKB0)//[0:0]
#define PDWNC_SM1CTL3 (IO_VIRT+IO_PDWNC_BASE + 0xC2C)
    #define FLD_SIFM1_ACK1 Fld(21,11,AC_MSKDW)//[31:11]
    #define FLD_SIFM1_WD_STOP_MODE Fld(1,6,AC_MSKB0)//[6:6]
    #define FLD_SIFM1_STR_WD_MODE Fld(1,5,AC_MSKB0)//[5:5]
    #define FLD_SIFM1_STR_WD_STOP_MODE Fld(1,4,AC_MSKB0)//[4:4]
    #define FLD_SIFM1_RDNACK_STOP_MODE Fld(1,3,AC_MSKB0)//[3:3]
    #define FLD_SIFM1_STR_RDACK_MODE Fld(1,2,AC_MSKB0)//[2:2]
    #define FLD_SIFM1_STR_RDNACK_STOP_MODE Fld(1,1,AC_MSKB0)//[1:1]
    #define FLD_SIFM1_SDEV_MODE Fld(1,0,AC_MSKB0)//[0:0]
#define PDWNC_SM1D2 (IO_VIRT+IO_PDWNC_BASE + 0xC30)
    #define FLD_SIFM1_DATA11 Fld(8,24,AC_FULLB3)//[31:24]
    #define FLD_SIFM1_DATA10 Fld(8,16,AC_FULLB2)//[23:16]
    #define FLD_SIFM1_DATA9 Fld(8,8,AC_FULLB1)//[15:8]
    #define FLD_SIFM1_DATA8 Fld(8,0,AC_FULLB0)//[7:0]
#define PDWNC_SM1D3 (IO_VIRT+IO_PDWNC_BASE + 0xC34)
    #define FLD_SIFM1_DATA15 Fld(8,24,AC_FULLB3)//[31:24]
    #define FLD_SIFM1_DATA14 Fld(8,16,AC_FULLB2)//[23:16]
    #define FLD_SIFM1_DATA13 Fld(8,8,AC_FULLB1)//[15:8]
    #define FLD_SIFM1_DATA12 Fld(8,0,AC_FULLB0)//[7:0]
#define PDWNC_SM1D4 (IO_VIRT+IO_PDWNC_BASE + 0xC38)
    #define FLD_SIFM1_DATA19 Fld(8,24,AC_FULLB3)//[31:24]
    #define FLD_SIFM1_DATA18 Fld(8,16,AC_FULLB2)//[23:16]
    #define FLD_SIFM1_DATA17 Fld(8,8,AC_FULLB1)//[15:8]
    #define FLD_SIFM1_DATA16 Fld(8,0,AC_FULLB0)//[7:0]
#define PDWNC_SM1D5 (IO_VIRT+IO_PDWNC_BASE + 0xC3C)
    #define FLD_SIFM1_DATA23 Fld(8,24,AC_FULLB3)//[31:24]
    #define FLD_SIFM1_DATA22 Fld(8,16,AC_FULLB2)//[23:16]
    #define FLD_SIFM1_DATA21 Fld(8,8,AC_FULLB1)//[15:8]
    #define FLD_SIFM1_DATA20 Fld(8,0,AC_FULLB0)//[7:0]
#define PDWNC_SM1D6 (IO_VIRT+IO_PDWNC_BASE + 0xC40)
    #define FLD_SIFM1_DATA27 Fld(8,24,AC_FULLB3)//[31:24]
    #define FLD_SIFM1_DATA26 Fld(8,16,AC_FULLB2)//[23:16]
    #define FLD_SIFM1_DATA25 Fld(8,8,AC_FULLB1)//[15:8]
    #define FLD_SIFM1_DATA24 Fld(8,0,AC_FULLB0)//[7:0]
#define PDWNC_SM1D7 (IO_VIRT+IO_PDWNC_BASE + 0xC44)
    #define FLD_SIFM1_DATA31 Fld(8,24,AC_FULLB3)//[31:24]
    #define FLD_SIFM1_DATA30 Fld(8,16,AC_FULLB2)//[23:16]
    #define FLD_SIFM1_DATA29 Fld(8,8,AC_FULLB1)//[15:8]
    #define FLD_SIFM1_DATA28 Fld(8,0,AC_FULLB0)//[7:0]
#define PDWNC_SM1D8 (IO_VIRT+IO_PDWNC_BASE + 0xC48)
    #define FLD_SIFM1_DATA35 Fld(8,24,AC_FULLB3)//[31:24]
    #define FLD_SIFM1_DATA34 Fld(8,16,AC_FULLB2)//[23:16]
    #define FLD_SIFM1_DATA33 Fld(8,8,AC_FULLB1)//[15:8]
    #define FLD_SIFM1_DATA32 Fld(8,0,AC_FULLB0)//[7:0]
#define PDWNC_SM1SDEV (IO_VIRT+IO_PDWNC_BASE + 0xC4C)
    #define FLD_SIFM1_DEV_ADDR Fld(8,0,AC_FULLB0)//[7:0]
#define PDWNC_SM2CTL2 (IO_VIRT+IO_PDWNC_BASE + 0xC50)
    #define FLD_SIFM2_SCL_RISE_VALUE Fld(12,16,AC_MSKW32)//[27:16]
    #define FLD_SIFM2_SCL_MODE Fld(1,0,AC_MSKB0)//[0:0]
#define PDWNC_SM2CTL3 (IO_VIRT+IO_PDWNC_BASE + 0xC54)
    #define FLD_SIFM2_ACK1 Fld(21,11,AC_MSKDW)//[31:11]
    #define FLD_SIFM2_WD_STOP_MODE Fld(1,6,AC_MSKB0)//[6:6]
    #define FLD_SIFM2_STR_WD_MODE Fld(1,5,AC_MSKB0)//[5:5]
    #define FLD_SIFM2_STR_WD_STOP_MODE Fld(1,4,AC_MSKB0)//[4:4]
    #define FLD_SIFM2_RDNACK_STOP_MODE Fld(1,3,AC_MSKB0)//[3:3]
    #define FLD_SIFM2_STR_RDACK_MODE Fld(1,2,AC_MSKB0)//[2:2]
    #define FLD_SIFM2_STR_RDNACK_STOP_MODE Fld(1,1,AC_MSKB0)//[1:1]
    #define FLD_SIFM2_SDEV_MODE Fld(1,0,AC_MSKB0)//[0:0]
#define PDWNC_SM2D2 (IO_VIRT+IO_PDWNC_BASE + 0xC58)
    #define FLD_SIFM2_DATA11 Fld(8,24,AC_FULLB3)//[31:24]
    #define FLD_SIFM2_DATA10 Fld(8,16,AC_FULLB2)//[23:16]
    #define FLD_SIFM2_DATA9 Fld(8,8,AC_FULLB1)//[15:8]
    #define FLD_SIFM2_DATA8 Fld(8,0,AC_FULLB0)//[7:0]
#define PDWNC_SM2D3 (IO_VIRT+IO_PDWNC_BASE + 0xC5C)
    #define FLD_SIFM2_DATA15 Fld(8,24,AC_FULLB3)//[31:24]
    #define FLD_SIFM2_DATA14 Fld(8,16,AC_FULLB2)//[23:16]
    #define FLD_SIFM2_DATA13 Fld(8,8,AC_FULLB1)//[15:8]
    #define FLD_SIFM2_DATA12 Fld(8,0,AC_FULLB0)//[7:0]
#define PDWNC_SM2D4 (IO_VIRT+IO_PDWNC_BASE + 0xC60)
    #define FLD_SIFM2_DATA19 Fld(8,24,AC_FULLB3)//[31:24]
    #define FLD_SIFM2_DATA18 Fld(8,16,AC_FULLB2)//[23:16]
    #define FLD_SIFM2_DATA17 Fld(8,8,AC_FULLB1)//[15:8]
    #define FLD_SIFM2_DATA16 Fld(8,0,AC_FULLB0)//[7:0]
#define PDWNC_SM2D5 (IO_VIRT+IO_PDWNC_BASE + 0xC64)
    #define FLD_SIFM2_DATA23 Fld(8,24,AC_FULLB3)//[31:24]
    #define FLD_SIFM2_DATA22 Fld(8,16,AC_FULLB2)//[23:16]
    #define FLD_SIFM2_DATA21 Fld(8,8,AC_FULLB1)//[15:8]
    #define FLD_SIFM2_DATA20 Fld(8,0,AC_FULLB0)//[7:0]
#define PDWNC_SM2D6 (IO_VIRT+IO_PDWNC_BASE + 0xC68)
    #define FLD_SIFM2_DATA27 Fld(8,24,AC_FULLB3)//[31:24]
    #define FLD_SIFM2_DATA26 Fld(8,16,AC_FULLB2)//[23:16]
    #define FLD_SIFM2_DATA25 Fld(8,8,AC_FULLB1)//[15:8]
    #define FLD_SIFM2_DATA24 Fld(8,0,AC_FULLB0)//[7:0]
#define PDWNC_SM2D7 (IO_VIRT+IO_PDWNC_BASE + 0xC6C)
    #define FLD_SIFM2_DATA31 Fld(8,24,AC_FULLB3)//[31:24]
    #define FLD_SIFM2_DATA30 Fld(8,16,AC_FULLB2)//[23:16]
    #define FLD_SIFM2_DATA29 Fld(8,8,AC_FULLB1)//[15:8]
    #define FLD_SIFM2_DATA28 Fld(8,0,AC_FULLB0)//[7:0]
#define PDWNC_SM2D8 (IO_VIRT+IO_PDWNC_BASE + 0xC70)
    #define FLD_SIFM2_DATA35 Fld(8,24,AC_FULLB3)//[31:24]
    #define FLD_SIFM2_DATA34 Fld(8,16,AC_FULLB2)//[23:16]
    #define FLD_SIFM2_DATA33 Fld(8,8,AC_FULLB1)//[15:8]
    #define FLD_SIFM2_DATA32 Fld(8,0,AC_FULLB0)//[7:0]
#define PDWNC_SM2SDEV (IO_VIRT+IO_PDWNC_BASE + 0xC74)
    #define FLD_SIFM2_DEV_ADDR Fld(8,0,AC_FULLB0)//[7:0]
#endif
#define PDWNC_CBUS_WBUF0 (IO_VIRT+IO_PDWNC_BASE + 0xD00)
    #define FLD_CBUS_WBUF0 Fld(11,0,AC_MSKW10)//[10:0]
    #define FLD_CBUS_WBUF1 Fld(11,16,AC_MSKW32)//[26:16]
#define PDWNC_CBUS_WBUF2 (IO_VIRT+IO_PDWNC_BASE + 0xD04)
    #define FLD_CBUS_WBUF2 Fld(11,0,AC_MSKW10)//[10:0]
    #define FLD_CBUS_WBUF3 Fld(11,16,AC_MSKW32)//[26:16]
#define PDWNC_CBUS_WBUF4 (IO_VIRT+IO_PDWNC_BASE + 0xD08)
    #define FLD_CBUS_WBUF4 Fld(11,0,AC_MSKW10)//[10:0]
    #define FLD_CBUS_WBUF5 Fld(11,16,AC_MSKW32)//[26:16]
#define PDWNC_CBUS_WBUF6 (IO_VIRT+IO_PDWNC_BASE + 0xD0C)
    #define FLD_CBUS_WBUF6 Fld(11,0,AC_MSKW10)//[10:0]
    #define FLD_CBUS_WBUF7 Fld(11,16,AC_MSKW32)//[26:16]
#define PDWNC_CBUS_WBUF8 (IO_VIRT+IO_PDWNC_BASE + 0xD10)
    #define FLD_CBUS_WBUF8 Fld(11,0,AC_MSKW10)//[10:0]
    #define FLD_CBUS_WBUF9 Fld(11,16,AC_MSKW32)//[26:16]
#define PDWNC_CBUS_WBUF10 (IO_VIRT+IO_PDWNC_BASE + 0xD14)
    #define FLD_CBUS_WBUF10 Fld(11,0,AC_MSKW10)//[10:0]
    #define FLD_CBUS_WBUF11 Fld(11,16,AC_MSKW32)//[26:16]
#define PDWNC_CBUS_WBUF12 (IO_VIRT+IO_PDWNC_BASE + 0xD18)
    #define FLD_CBUS_WBUF12 Fld(11,0,AC_MSKW10)//[10:0]
    #define FLD_CBUS_WBUF13 Fld(11,16,AC_MSKW32)//[26:16]
#define PDWNC_CBUS_WBUF14 (IO_VIRT+IO_PDWNC_BASE + 0xD1C)
    #define FLD_CBUS_WBUF14 Fld(11,0,AC_MSKW10)//[10:0]
    #define FLD_CBUS_WBUF15 Fld(11,16,AC_MSKW32)//[26:16]
#define PDWNC_CBUS_WBUF16 (IO_VIRT+IO_PDWNC_BASE + 0xD20)
    #define FLD_CBUS_WBUF16 Fld(11,0,AC_MSKW10)//[10:0]
    #define FLD_CBUS_WBUF17 Fld(11,16,AC_MSKW32)//[26:16]
#define PDWNC_CBUS_WBUF18 (IO_VIRT+IO_PDWNC_BASE + 0xD24)
    #define FLD_CBUS_WBUF18 Fld(11,0,AC_MSKW10)//[10:0]
    #define FLD_CBUS_WBUF19 Fld(11,16,AC_MSKW32)//[26:16]
#define PDWNC_CBUS_WBUF20 (IO_VIRT+IO_PDWNC_BASE + 0xD28)
    #define FLD_CBUS_WBUF20 Fld(11,0,AC_MSKW10)//[10:0]
    #define FLD_CBUS_WBUF21 Fld(11,16,AC_MSKW32)//[26:16]
#define PDWNC_CBUS_WBUF22 (IO_VIRT+IO_PDWNC_BASE + 0xD2C)
    #define FLD_CBUS_WBUF22 Fld(11,0,AC_MSKW10)//[10:0]
    #define FLD_CBUS_WBUF23 Fld(11,16,AC_MSKW32)//[26:16]
#define PDWNC_CBUS_RBUF (IO_VIRT+IO_PDWNC_BASE + 0xD30)
    #define FLD_CBUS_RBUF Fld(11,0,AC_MSKW10)//[10:0]
#define PDWNC_CBUS_LINK_00 (IO_VIRT+IO_PDWNC_BASE + 0xD34)
    #define FLD_RBUF_LVL_THR Fld(5,27,AC_MSKB3)//[31:27]
    #define FLD_IGNORE_PAR Fld(1,25,AC_MSKB3)//[25:25]
    #define FLD_DUPLEX Fld(1,24,AC_MSKB3)//[24:24]
    #define FLD_NRETRY Fld(6,18,AC_MSKB2)//[23:18]
    #define FLD_TRESP_HOLD Fld(4,14,AC_MSKW21)//[17:14]
    #define FLD_TWAIT Fld(4,10,AC_MSKB1)//[13:10]
    #define FLD_TREQ_HOLD Fld(4,6,AC_MSKW10)//[9:6]
    #define FLD_TX_NUM Fld(5,1,AC_MSKB0)//[5:1]
    #define FLD_TX_TRIG Fld(1,0,AC_MSKB0)//[0:0]
#define PDWNC_CBUS_LINK_01 (IO_VIRT+IO_PDWNC_BASE + 0xD38)
    #define FLD_ADP_BITIME_EN Fld(1,31,AC_MSKB3)//[31:31]
    #define FLD_ADP_BITIME_RST Fld(1,30,AC_MSKB3)//[30:30]
    #define FLD_ADP_CONFIG1 Fld(1,29,AC_MSKB3)//[29:29]
    #define FLD_ADP_CONFIG2 Fld(1,28,AC_MSKB3)//[28:28]
    #define FLD_LINKTX_BITIME Fld(7,21,AC_MSKW32)//[27:21]
    #define FLD_CBUS_ACK_0_MIN Fld(7,14,AC_MSKW21)//[20:14]
    #define FLD_CBUS_ACK_0_MAX Fld(7,7,AC_MSKW10)//[13:7]
    #define FLD_CBUS_ACK_FALL_MAX Fld(7,0,AC_MSKB0)//[6:0]
#define PDWNC_CBUS_LINK_02 (IO_VIRT+IO_PDWNC_BASE + 0xD3C)
    #define FLD_LINKRX_EN Fld(1,31,AC_MSKB3)//[31:31]
    #define FLD_FAKE_SOURCE Fld(1,28,AC_MSKB3)//[28:28]
    #define FLD_TREQ_ACK_CONT Fld(7,21,AC_MSKW32)//[27:21]
    #define FLD_LINK_TXDECISION Fld(7,14,AC_MSKW21)//[20:14]
    #define FLD_LINK_HALFTRAN_MAX Fld(7,7,AC_MSKW10)//[13:7]
    #define FLD_LINK_HALFTRAN_MIN Fld(7,0,AC_MSKB0)//[6:0]
#define PDWNC_CBUS_LINK_03 (IO_VIRT+IO_PDWNC_BASE + 0xD40)
    #define FLD_SW_RESET_MISC Fld(1,31,AC_MSKB3)//[31:31]
    #define FLD_SW_RESET_WAKE Fld(1,30,AC_MSKB3)//[30:30]
    #define FLD_SW_RESET_RX Fld(1,29,AC_MSKB3)//[29:29]
    #define FLD_SW_RESET_TX Fld(1,28,AC_MSKB3)//[28:28]
    #define FLD_LINK_BITIME_MAX Fld(7,21,AC_MSKW32)//[27:21]
    #define FLD_LINK_BITIME_MIN Fld(7,14,AC_MSKW21)//[20:14]
    #define FLD_LINK_SYNCDUTY_MAX Fld(7,7,AC_MSKW10)//[13:7]
    #define FLD_LINK_SYNCDUTY_MIN Fld(7,0,AC_MSKB0)//[6:0]
#define PDWNC_CBUS_LINK_04 (IO_VIRT+IO_PDWNC_BASE + 0xD44)
    #define FLD_LINK_ACK_MANU_EN Fld(1,28,AC_MSKB3)//[28:28]
    #define FLD_LINK_ACK_WIDTH Fld(7,21,AC_MSKW32)//[27:21]
    #define FLD_LINK_BITIME_MID Fld(7,14,AC_MSKW21)//[20:14]
    #define FLD_LINK_RXDECISION Fld(7,7,AC_MSKW10)//[13:7]
    #define FLD_RX_BT_TIMEOUT Fld(7,0,AC_MSKB0)//[6:0]
#define PDWNC_CBUS_LINK_05 (IO_VIRT+IO_PDWNC_BASE + 0xD48)
    #define FLD_DISCOVERY_EN Fld(1,31,AC_MSKB3)//[31:31]
    #define FLD_WAKEUP_EN Fld(1,30,AC_MSKB3)//[30:30]
    #define FLD_WAKE_PUL_WID2_MAX Fld(10,20,AC_MSKW32)//[29:20]
    #define FLD_WAKE_PUL_WID1_MIN Fld(10,10,AC_MSKW21)//[19:10]
    #define FLD_WAKE_PUL_WID1_MAX Fld(10,0,AC_MSKW10)//[9:0]
#define PDWNC_CBUS_LINK_06 (IO_VIRT+IO_PDWNC_BASE + 0xD4C)
    #define FLD_WAKE_PUL_WID2_MIN Fld(10,22,AC_MSKW32)//[31:22]
    #define FLD_WAKE_TO_DISC_MIN Fld(11,11,AC_MSKW21)//[21:11]
    #define FLD_WAKE_TO_DISC_MAX Fld(11,0,AC_MSKW10)//[10:0]
#define PDWNC_CBUS_LINK_07 (IO_VIRT+IO_PDWNC_BASE + 0xD50)
    #if defined(CC_MT5882)
	    #define FLD_CBUS_DISCONN_CNT_EN Fld(1,31,AC_MSKB3)//[31:31]
    #endif
    #define FLD_CBUS_OE Fld(1,29,AC_MSKB3)//[29:29]
    #define FLD_CBUS_DO Fld(1,28,AC_MSKB3)//[28:28]
    #define FLD_CBUS_DOE_SW Fld(1,27,AC_MSKB3)//[27:27]
    #define FLD_ZCBUS_DISCOVER_EN Fld(1,26,AC_MSKB3)//[26:26]
    #define FLD_ZCBUS_SINK_ON_CTRL Fld(3,23,AC_MSKW32)//[25:23]
    #define FLD_ZCBUS_HW Fld(1,22,AC_MSKB2)//[22:22]
    #define FLD_LDO_SWITCH Fld(1,21,AC_MSKB2)//[21:21]
    #define FLD_LDO_SWITCH_HW Fld(1,20,AC_MSKB2)//[20:20]
    #define FLD_DISC_PUL_WID_MIN Fld(10,10,AC_MSKW21)//[19:10]
    #define FLD_DISC_PUL_WID_MAX Fld(10,0,AC_MSKW10)//[9:0]
#define PDWNC_CBUS_LINK_08 (IO_VIRT+IO_PDWNC_BASE + 0xD54)
    #define FLD_LINKRX_TIMEOUT_INT_CLR Fld(1,17,AC_MSKB2)//[17:17]
    #define FLD_WBUF_TRIG_INT_CLR Fld(1,16,AC_MSKB2)//[16:16]
    #define FLD_RBUF_TRIG_INT_CLR Fld(1,15,AC_MSKB1)//[15:15]
    #define FLD_CBUS_LOW_DISCONN_INT_CLR Fld(1,14,AC_MSKB1)//[14:14]
    #define FLD_CBUS_NEG_INT_CLR Fld(1,13,AC_MSKB1)//[13:13]
    #define FLD_CBUS_POS_INT_CLR Fld(1,12,AC_MSKB1)//[12:12]
    #define FLD_MONITOR_CMP_INT_CLR Fld(1,11,AC_MSKB1)//[11:11]
    #define FLD_CABLE_DISCONNECT_INT_CLR Fld(1,10,AC_MSKB1)//[10:10]
    #define FLD_CABLE_DETECT_INT_CLR Fld(1,9,AC_MSKB1)//[9:9]
    #define FLD_TX_ARB_FAIL_INT_CLR Fld(1,8,AC_MSKB1)//[8:8]
    #define FLD_TX_OK_INT_CLR Fld(1,7,AC_MSKB0)//[7:7]
    #define FLD_TX_RETRY_TO_INT_CLR Fld(1,6,AC_MSKB0)//[6:6]
    #define FLD_RBUF_LVL_THR_INT_CLR Fld(1,5,AC_MSKB0)//[5:5]
    #define FLD_WK_TIMEOUT_ST_INT_CLR Fld(1,4,AC_MSKB0)//[4:4]
    #define FLD_WAKEUP_ILL_WID_INT_CLR Fld(1,3,AC_MSKB0)//[3:3]
    #define FLD_ILL_WAKE2DISC_INT_CLR Fld(1,2,AC_MSKB0)//[2:2]
    #define FLD_WAKEUP_DET_INT_CLR Fld(1,1,AC_MSKB0)//[1:1]
    #define FLD_DISC_DET_INT_CLR Fld(1,0,AC_MSKB0)//[0:0]
#define PDWNC_CBUS_LINK_09 (IO_VIRT+IO_PDWNC_BASE + 0xD58)
    #define FLD_TREQ_ARB_CONT Fld(7,25,AC_MSKB3)//[31:25]
    #define FLD_CBUS_DRV_H_PRD Fld(5,20,AC_MSKW32)//[24:20]
    #define FLD_DISC_WID_CNT_TIMEOUT Fld(10,10,AC_MSKW21)//[19:10]
    #define FLD_WAKE_CNT_TIMEOUT Fld(10,0,AC_MSKW10)//[9:0]
#define PDWNC_CBUS_LINK_0A (IO_VIRT+IO_PDWNC_BASE + 0xD5C)
    #define FLD_LINKRX_ACK1_SYNC Fld(7,25,AC_MSKB3)//[31:25]
    #define FLD_LINKRX_DIS_TO Fld(1,19,AC_MSKB2)//[19:19]
    #define FLD_TX_ARB_BYPASS Fld(1,18,AC_MSKB2)//[18:18]
    #define FLD_RX_ARB_BYPASS Fld(1,17,AC_MSKB2)//[17:17]
    #define FLD_FORCE_NAK_EN Fld(1,16,AC_MSKB2)//[16:16]
    #define FLD_FORCE_NAK_CNT Fld(5,11,AC_MSKB1)//[15:11]
    #define FLD_MON_CMP_SEL Fld(3,8,AC_MSKB1)//[10:8]
    #define FLD_MON_CMP_DATA Fld(8,0,AC_FULLB0)//[7:0]
#define PDWNC_CBUS_LINK_0B (IO_VIRT+IO_PDWNC_BASE + 0xD60)
    #if defined(CC_MT5399)
	    #define FLD_INT_STA_MASK Fld(18,14,AC_MSKDW)//[31:14]
	    #define FLD_CBUS_DISCONN_CNT_EN Fld(1,13,AC_MSKB1)//[13:13]
    #else
	    #define FLD_INT_STA_MASK Fld(19,13,AC_MSKDW)//[31:14]
    #endif
    #define FLD_RETRY_DISCONN_THR Fld(6,7,AC_MSKW10)//[12:7]
    #define FLD_CBUS_DISCONN_THR Fld(7,0,AC_MSKB0)//[6:0]
#define PDWNC_CBUS_LINK_0C (IO_VIRT+IO_PDWNC_BASE + 0xD64)
	#define FLD_CBUS_1K_CTRL Fld(2,30,AC_MSKB3)
    #define FLD_CDSENSE_FORCE1 Fld(1,25,AC_MSKB3)//[25:25]
    #define FLD_CDSENSE_GATED Fld(1,24,AC_MSKB3)//[24:24]
    #define FLD_CDSENSE_OFF_DEGLITCH Fld(1,23,AC_MSKB2)//[23:23]
    #define FLD_CDSENSE_ON_DEGLITCH Fld(1,22,AC_MSKB2)//[22:22]
    #define FLD_CDSENSE_P_DEGLITCH Fld(1,21,AC_MSKB2)//[21:21]
    #define FLD_LINKRX_BITIME Fld(7,14,AC_MSKW21)//[20:14]
    #define FLD_ADP_BITIME_MIN Fld(7,7,AC_MSKW10)//[13:7]
    #define FLD_ADP_BITIME_MAX Fld(7,0,AC_MSKB0)//[6:0]
#define PDWNC_CBUS_LINK_0D (IO_VIRT+IO_PDWNC_BASE + 0xD68)
    #define FLD_INT_MASK Fld(17,0,AC_MSKDW)//[16:0]
#define PDWNC_CBUS_STA_00 (IO_VIRT+IO_PDWNC_BASE + 0xD6C)
    #define FLD_LINK_BITIME Fld(7,25,AC_MSKB3)//[31:25]
    #define FLD_RBUF_LVL_LAT Fld(5,20,AC_MSKW32)//[24:20]
    #define FLD_LINKRX_TIMEOUT_INT Fld(1,17,AC_MSKB2)//[17:17]
    #define FLD_WBUF_PULS1_INT Fld(1,16,AC_MSKB2)//[16:16]
    #define FLD_RBUF_PULS1_INT Fld(1,15,AC_MSKB1)//[15:15]
    #define FLD_CBUS_LOW_DISCONN_INT Fld(1,14,AC_MSKB1)//[14:14]
    #define FLD_CBUS_NEG_INT Fld(1,13,AC_MSKB1)//[13:13]
    #define FLD_CBUS_POS_INT Fld(1,12,AC_MSKB1)//[12:12]
    #define FLD_MONITOR_CMP_INT Fld(1,11,AC_MSKB1)//[11:11]
    #define FLD_CABLE_DISCONNECT_INT Fld(1,10,AC_MSKB1)//[10:10]
    #define FLD_CABLE_DETECT_INT Fld(1,9,AC_MSKB1)//[9:9]
    #define FLD_TX_ARB_FAIL_INT Fld(1,8,AC_MSKB1)//[8:8]
    #define FLD_TX_OK_INT Fld(1,7,AC_MSKB0)//[7:7]
    #define FLD_TX_RETRY_TO_INT Fld(1,6,AC_MSKB0)//[6:6]
    #define FLD_RBUF_LVL_THR_INT Fld(1,5,AC_MSKB0)//[5:5]
    #define FLD_WK_TIMEOUT_ST_INT Fld(1,4,AC_MSKB0)//[4:4]
    #define FLD_WAKEUP_ILL_WID_INT Fld(1,3,AC_MSKB0)//[3:3]
    #define FLD_ILL_WAKE2DISC_INT Fld(1,2,AC_MSKB0)//[2:2]
    #define FLD_WAKEUP_DET_INT Fld(1,1,AC_MSKB0)//[1:1]
    #define FLD_DISC_DET_INT Fld(1,0,AC_MSKB0)//[0:0]
#define PDWNC_CBUS_STA_01 (IO_VIRT+IO_PDWNC_BASE + 0xD70)
    #define FLD_CBUS_IN Fld(1,31,AC_MSKB3)//[31:31]
    #define FLD_CBUS_CDSENSE Fld(1,30,AC_MSKB3)//[30:30]
    #define FLD_LINKRX_FSM Fld(5,12,AC_MSKW21)//[16:12]
    #define FLD_LINKTX_FSM Fld(5,7,AC_MSKW10)//[11:7]
    #define FLD_DISC_FSM Fld(3,4,AC_MSKB0)//[6:4]
    #define FLD_WAKEUP_FSM Fld(4,0,AC_MSKB0)//[3:0]
#define PDWNC_CBUS_LINK_BAK (IO_VIRT+IO_PDWNC_BASE + 0xD74)
    #define FLD_CBUS_DRV_H_PRD_ARB Fld(5,16,AC_MSKB2)//[16:21]
    #define FLD_CBUS_DRV_H_SEL Fld(1,15,AC_MSKB1)//[15:15]
    #define FLD_CBUS_OE_FAST1 Fld(1,14,AC_MSKB1)//[14:14]
    #define FLD_ADP_SYNC Fld(1,13,AC_MSKB1)//[13:13]
    #define FLD_CBUS_OE_FAST Fld(1,12,AC_MSKB1)//[12:12]
    #define FLD_CBUS_DEGLITCH Fld(1,11,AC_MSKB1)//[11:11]    
    #define FLD_LINKRX_DIS_TO_ARB23 Fld(1,10,AC_MSKB1)//[10:10]
    #define FLD_LINK_ACK_WIDTH_UPPER Fld(7,3,AC_MSKB0)//[3:10]
    #define FLD_ARB_DRIVEH Fld(1,2,AC_MSKB0)//[2:2]
    #define FLD_TX_ARB2_DIS_CHK Fld(1,1,AC_MSKB0)//[1:1]
    #define FLD_TX_ARB1_DIS_CHK Fld(1,0,AC_MSKB0)//[0:0]    
#define PDWNC_CBUS_TRAN_00 (IO_VIRT+IO_PDWNC_BASE + 0xD80)
    #define FLD_RG_TRAN_TIMEOUT Fld(16,16,AC_MSKW32)//[11:11]    
    #define FLD_RG_SCLK_DIV Fld(12,4,AC_MSKW10)//[10:10]
    #define FLD_RG_WAIT_LEVEL Fld(1,3,AC_MSKB0)//[9:3]
    #define FLD_RG_TRAN_EN Fld(1,2,AC_MSKB0)//[2:2]
    #define FLD_RG_TRAN_SPEED Fld(1,1,AC_MSKB0)//[1:1]
    #define FLD_CBUS_TRAN_HW_EN Fld(1,0,AC_MSKB0)//[0:0]
#define PDWNC_CBUS_TRAN_01 (IO_VIRT+IO_PDWNC_BASE + 0xD84)
    #define FLD_RG_TRAN_INIT_STA_MASK Fld(13,16,AC_MSKW32)//[11:11]    
    #define FLD_RG_CBUS_HW_SEL Fld(1,4,AC_MSKB0)//[04:04]
    #define FLD_RG_HW_TX_STOP Fld(1,3,AC_MSKB0)//[9:3]
    #define FLD_RG_SW_RESET_TRAN Fld(1,2,AC_MSKB0)//[2:2]
    #define FLD_RG_I2C_FORCE_OK Fld(1,1,AC_MSKB0)//[1:1]
    #define FLD_CBUS_MON_SEL Fld(1,0,AC_MSKB0)//[0:0]

#endif //_HW_PDWNC_H_


