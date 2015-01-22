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

#ifndef CONFIG_DEFAULT_H
#define CONFIG_DEFAULT_H


//----------------------------------------------------------------------------
// Initial dram settings
//----------------------------------------------------------------------------
#define DEFAULT_DRAM_TYPE               DDR_III_x2
#define DEFAULT_DDR_CLOCK               (1532000000)

//----------------------------------------------------------------------------
// FBM setup
//----------------------------------------------------------------------------
#ifndef FBM_MEM_CFG_SIZE
#define FBM_MEM_CFG_SIZE                FBM_MEM_CFG_MT5881_128Mx1
#endif /* FBM_MEM_CFG_SIZE */

//----------------------------------------------------------------------------
// Default Panel setting
//----------------------------------------------------------------------------
#ifndef DEFAULT_PANEL_SELECT
#define DEFAULT_PANEL_SELECT            (PANEL_DEFAULT)
#endif
#define BACKLIGHT_GPIO                  (GPIO(17))
#define BACKLIGHT_ON_VAL                (0)
#define PANELPOWER_GPIO                 (GPIO(18))
#define PANELPOWER_ON_VAL               (1)
// disable spread spectrum for vga output
#define SPREAD_SPECTRUM_PERMILLAGE      (0)
// pwm backlight adjustment setting
#define PANEL_BACKLIGHT_PWM_PORT        (1)

//----------------------------------------------------------------------------
// Audio Customization
//----------------------------------------------------------------------------
#define AUDIO_Mute_Gpio_Num             (OPCTRL(11))
#define AUDIO_Amp_Gpio_Num              (OPCTRL(5))
#define AUDIO_HP_PLUG_IN_GPIO           (OPCTRL(6))
#define AUDIO_MUTE_GPIO_POLARITY        AUD_MUTE_GPIO_LOW_ENALBE
#define AUDIO_AMP_GPIO_POLARITY         AUD_AMP_GPIO_HIGH_ENALBE
#define AUDIO_HP_PLUG_IN_POLARITY       (AUD_GPIO_LOW_ENALBE)

//----------------------------------------------------------------------------
// Other GPIO customization
//----------------------------------------------------------------------------
#define FLAG_EXT_STATUS                 (0)
// #define POWER_LED_GPIO               (OPCTRL(0))
// #define POWER_LED_ON_VAL             (0)
// #define BACKUP_LED_GPIO              (OPCTRL(0))
// #define BACKUP_LED_ON_VAL            (0)
// #define POWER_CTRL_GPIO
// #define POWER_CTRL_ON_VAL

//----------------------------------------------------------------------------
// EEPROM customization
//----------------------------------------------------------------------------
#define SYSTEM_EEPROM_SIZE                  (2048)//(4096)
#define SYSTEM_EEPROM_BUS                 (0)
#define SYSTEM_EEPROM_ADDRESS             (0xA0)
#define SYSTEM_EEPROM_CLKDIV              (0x100)
#define SYSTEM_EEPROM_PAGESIZE              (8)
#define SYSTEM_EEPROM_WP_GPIO               (18)   // GPIO18.
#define SYSTEM_EEPROM_WP_ENABLE_POLARITY    (1)
#define EDID_EEPROM_WP_GPIO			        (14)   // GPIO14.
#define EDID_EEPROM_WP_ENABLE_POLARITY      (1)
//----------------------------------------------------------------------------
// SIF customization
//----------------------------------------------------------------------------
#define SIF_ENABLE_SCL_STRETCH      (0)
#define SIF_DISABLE_OPEN_DRAIN      (0)
#define SIF_ENABLE_PDWNC_MASTER      (0)
#define SIF_ENABLE_PDWNC_MASTER1      (0)

//----------------------------------------------------------------------------
// USB Customization
//----------------------------------------------------------------------------
#define USB0_VBUS_GPIO                      (0)    // GPIO0.
#define USB0_VBUS_GPIO_POLARITY		        (1)
#define USB1_VBUS_GPIO                      (2)    // GPIO2
#define USB1_VBUS_GPIO_POLARITY		        (1)

#define USB0_OC_GPIO                        (1)    // GPIO1.
#define USB0_OC_GPIO_POLARITY		        (0)
#define USB1_OC_GPIO                        (3)    // GPIO3.
#define USB1_OC_GPIO_POLARITY		        (0)

//#define USB_DISABLE_SUSPEND_MODE /* Disable USB suspend, share 240 Mhz with H.264.*/
//----------------------------------------------------------------------------
// Button Customization
//----------------------------------------------------------------------------
#define FLAG_BUTTON_SUPPORT                 (0)


//----------------------------------------------------------------------------
// MTK IR only setting
//----------------------------------------------------------------------------
#define IR_INCLUDE_FILE                     "mtk/mtk_ir.h"

#ifndef NPTV_SOURCE_TABLE
#define NPTV_SOURCE_TABLE                   "mtk/source_table.c"
#endif

#ifndef T8032UP_INCLUDE_FILE
#define T8032UP_INCLUDE_FILE                "mtk/mtk_t8032fw.h"
#define T8032UP_OFFSET                      au1T8032FW
#define T8032UP_SIZE                        24576//12288//16384
#endif
//----------------------------------------------------------------------------
// srvad wakeup customization
//----------------------------------------------------------------------------
//for srvad wakeup test
#define KEYPAD_WAKEUP_BIT                   (DKBIT_ENTER | DKBIT_POWER | DKBIT_PRG_UP | DKBIT_PRG_DOWN | DKBIT_VOL_DOWN | DKBIT_VOL_UP | DKBIT_INPUT_SRC | DKBIT_MENU)

//----------------------------------------------------------------------------
// default HDMI GPIO control
//----------------------------------------------------------------------------
#ifndef HDMI_GPIO_CONF
#define HDMI_GPIO_CONF                      "mtk/hdmi_conf.c"
#endif

/*
 * default VGA table
*/
#ifndef VGA_INCLUDE_FILE
#define VGA_INCLUDE_FILE                    "LGE/mtk_vga.h"
#endif

//----------------------------------------------------------------------------
// Tcon Customization
//----------------------------------------------------------------------------
// support LVDS and MLVDS both
#define PANEL_DISPLAY_TYPE_ALLOWED_MASK ((1<<PANEL_DISPLAY_TYPE_LVDS) | (1 << PANEL_DISPLAY_TYPE_MLVDS))

#ifndef TCON_CUST_FILE
#define TCON_CUST_FILE                      "mtk/Capri_tcon_cust.c"
#endif

//----------------------------------------------------------------------------
// SCART out configuration
//----------------------------------------------------------------------------
#ifndef TVE_INCLUDE_FILE
#define TVE_INCLUDE_FILE                "mtk/mtk_tve.c"
#endif

//----------------------------------------------------------------------------
// DMX configuration
//----------------------------------------------------------------------------
#ifndef DMX_INCLUDE_FILE
#define DMX_INCLUDE_FILE               "mtk/mtk_dmx.h"
#endif

/*
 * default quality table
*/
#ifndef QTYTBL_INCLUDE_FILE
//#define QTYTBL_INCLUDE_FILE                 "mtk/QTY_DEFAULT/mtk_QtyTbl.h"
#endif

#ifdef SUPPORT_FLASH_PQ
#endif // #if SUPPORT_FLASH_PQ

/*
 * default color parameter
*/
#ifndef COLOR_PARAM_INCLUDE_FILE
//#define COLOR_PARAM_INCLUDE_FILE            "mtk/PANEL_DEFAULT/color_param.h"
#endif

/*
 * dynamic backlight parameter
*/

#else /* CONFIG_DEFAULT_H */
#error "Cannot include multi-config files"
#endif /* CONFIG_DEFAULT_H */

