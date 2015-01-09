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
#define DEFAULT_DRAM_TYPE                   DDR_III_x2
#define DEFAULT_DDR_CLOCK                   (1596000000)

//#ifndef DRAM_PRIORITY_LIST
// Audio/0 > B2R/5 > NR/PSCAN/4 > Scpip/7 > OSD/3 > Mjc_in/13 > Mjc_out/14 > VLD1/9 > Vdec_mc/8 > CPU/6 > Gfx/11 > Demux/PVR/1 > VBI/3D/TVE/2 >VENC/12 > Test/15 > Unused/10
//#define DRAM_PRIORITY_LIST                  ((UINT8*)"0c9421a387fbd56e")
//#endif

#define TOTAL_MEM_SIZE                      (1024 * 1024 * 1024)
#define DEFAULT_CHANNEL_A_SIZE					(1024 * 1024 * 1024)

//----------------------------------------------------------------------------
// DFB setup
//----------------------------------------------------------------------------
#if 1
#define CC_CONFIG_OSD_CM_SIZE               (4) // 4:argb 8888 ;2:argb 4444
#define CC_CONFIG_OSD_BUF_MODE              (2) // 2:double  buffer ;1:sngle buffer
        
#ifndef CC_CONFIG_OSD_PLANE1_MAX_W
#define CC_CONFIG_OSD_PLANE1_MAX_W          (0)
#endif
#ifndef CC_CONFIG_OSD_PLANE1_MAX_H
#define CC_CONFIG_OSD_PLANE1_MAX_H          (0)
#endif
#define CC_CONFIG_OSD_PLANE1_MEM_SIZE       (((CC_CONFIG_OSD_PLANE1_MAX_W*     \
                                        CC_CONFIG_OSD_PLANE1_MAX_H*        \
                                        CC_CONFIG_OSD_CM_SIZE*             \
                                        CC_CONFIG_OSD_BUF_MODE)+0x1000)&(0xFFFFF000))
        
        // osd 3d buffer size
#if (defined(CC_CONFIG_OSD_SBS_PLANE1)||defined(CC_CONFIG_OSD_TB_PLANE1))
#define CC_CONFIG_OSD_3D_PLANE1_MEM_SIZE    (CC_CONFIG_OSD_PLANE1_MEM_SIZE)
#else
#define CC_CONFIG_OSD_3D_PLANE1_MEM_SIZE    (0)
#endif
        
        // osd decompress  buffer size
#ifdef CONFIG_OSD_PLANE1_COMPRESSION
#define CC_CONFIG_OSD_DEC_PLANE1_MEM_SIZE   (CC_CONFIG_OSD_PLANE1_MEM_SIZE)
#else
#define CC_CONFIG_OSD_DEC_PLANE1_MEM_SIZE   (0)
#endif
        
#ifndef CC_CONFIG_OSD_PLANE2_MAX_W
#define CC_CONFIG_OSD_PLANE2_MAX_W          (960)
#endif
#ifndef CC_CONFIG_OSD_PLANE2_MAX_H
#define CC_CONFIG_OSD_PLANE2_MAX_H          (540)
#endif
#define CC_CONFIG_OSD_PLANE2_MEM_SIZE       (((CC_CONFIG_OSD_PLANE2_MAX_W*     \
                                            CC_CONFIG_OSD_PLANE2_MAX_H*        \
                                            CC_CONFIG_OSD_CM_SIZE*             \
                                            CC_CONFIG_OSD_BUF_MODE)+0x1000)&(0xFFFFF000))
        
#if (defined(CC_CONFIG_OSD_SBS_PLANE2)||defined(CC_CONFIG_OSD_TB_PLANE2))
#define CC_CONFIG_OSD_3D_PLANE2_MEM_SIZE    (CC_CONFIG_OSD_PLANE2_MEM_SIZE)
#else
#define CC_CONFIG_OSD_3D_PLANE2_MEM_SIZE    (0)
#endif
        
#ifndef CONFIG_OSD_PLANE2_COMPRESSION
        //#define  CONFIG_OSD_PLANE2_COMPRESSION
#endif
        
        // osd decompress  buffer size
#ifdef CONFIG_OSD_PLANE2_COMPRESSION
#define CC_CONFIG_OSD_DEC_PLANE2_MEM_SIZE   (CC_CONFIG_OSD_PLANE2_MEM_SIZE)
#else
#define CC_CONFIG_OSD_DEC_PLANE2_MEM_SIZE   (0)
#endif
        
#ifndef CC_CONFIG_OSD_PLANE3_MAX_W
#define CC_CONFIG_OSD_PLANE3_MAX_W          (1920)
#endif
#ifndef CC_CONFIG_OSD_PLANE3_MAX_H
#define CC_CONFIG_OSD_PLANE3_MAX_H          (1080)
#endif
#define CC_CONFIG_OSD_PLANE3_MEM_SIZE       (((CC_CONFIG_OSD_PLANE3_MAX_W*     \
                                            CC_CONFIG_OSD_PLANE3_MAX_H*        \
                                            CC_CONFIG_OSD_CM_SIZE*             \
                                            CC_CONFIG_OSD_BUF_MODE)+0x1000)&(0xFFFFF000))
        
#if (defined(CC_CONFIG_OSD_SBS_PLANE3)||defined(CC_CONFIG_OSD_TB_PLANE3))
#define CC_CONFIG_OSD_3D_PLANE3_MEM_SIZE    (CC_CONFIG_OSD_PLANE3_MEM_SIZE)
#else
#define CC_CONFIG_OSD_3D_PLANE3_MEM_SIZE    (0)
#endif
        
        // osd decompress  buffer size
#ifndef CONFIG_OSD_PLANE3_COMPRESSION
        // #define  CONFIG_OSD_PLANE3_COMPRESSION
#endif
        
#ifdef CONFIG_OSD_PLANE3_COMPRESSION
#define CC_CONFIG_OSD_DEC_PLANE3_MEM_SIZE   (CC_CONFIG_OSD_PLANE3_MEM_SIZE)
#else
#define CC_CONFIG_OSD_DEC_PLANE3_MEM_SIZE   (0)
#endif
        
#ifndef CC_CONFIG_OSD_PLANE4_MAX_W
#define CC_CONFIG_OSD_PLANE4_MAX_W          (0)
#endif
#ifndef CC_CONFIG_OSD_PLANE4_MAX_H
#define CC_CONFIG_OSD_PLANE4_MAX_H          (0)
#endif
#define CC_CONFIG_OSD_PLANE4_MEM_SIZE       (((CC_CONFIG_OSD_PLANE4_MAX_W*     \
                                            CC_CONFIG_OSD_PLANE4_MAX_H*        \
                                            CC_CONFIG_OSD_CM_SIZE*             \
                                            CC_CONFIG_OSD_BUF_MODE)+0x1000)&(0xFFFFF000))
        
#if (defined(CC_CONFIG_OSD_SBS_PLANE4)||defined(CC_CONFIG_OSD_TB_PLANE4))
#define CC_CONFIG_OSD_3D_PLANE4_MEM_SIZE    (CC_CONFIG_OSD_PLANE4_MEM_SIZE)
#else
#define CC_CONFIG_OSD_3D_PLANE4_MEM_SIZE    (0)
#endif
        
        // osd decompress  buffer size
#ifdef CONFIG_OSD_PLANE4_COMPRESSION
#define CC_CONFIG_OSD_DEC_PLANE4_MEM_SIZE   (CC_CONFIG_OSD_PLANE4_MEM_SIZE)
#else
#define CC_CONFIG_OSD_DEC_PLANE4_MEM_SIZE   (0)
#endif
        
#ifndef CC_CONFIG_OSD_PLANE5_MAX_W
#define CC_CONFIG_OSD_PLANE5_MAX_W          (100)
#endif
#ifndef CC_CONFIG_OSD_PLANE5_MAX_H
#define CC_CONFIG_OSD_PLANE5_MAX_H          (100)
#endif
        
#define CC_CONFIG_OSD_PLANE5_MEM_SIZE       (((CC_CONFIG_OSD_PLANE5_MAX_W*     \
                                    CC_CONFIG_OSD_PLANE5_MAX_H*        \
                                    CC_CONFIG_OSD_CM_SIZE)+0x1000)&(0xFFFFF000))
        
        // osd 3d buffer size
#if (defined(CC_CONFIG_OSD_SBS_PLANE5)||defined(CC_CONFIG_OSD_TB_PLANE5))
#define CC_CONFIG_OSD_3D_PLANE5_MEM_SIZE    (CC_CONFIG_OSD_PLANE5_MEM_SIZE)
#else
#define CC_CONFIG_OSD_3D_PLANE5_MEM_SIZE    (0)
#endif
        
        // osd decompress  buffer size
#ifdef CONFIG_OSD_PLANE5_COMPRESSION
#define CC_CONFIG_OSD_DEC_PLANE5_MEM_SIZE   (CC_CONFIG_OSD_PLANE5_MEM_SIZE)
#else
#define CC_CONFIG_OSD_DEC_PLANE5_MEM_SIZE   (0)
#endif
        
#define CC_CONFIG_OSD_PLANE_BASIC_MEM_SIZE  (CC_CONFIG_OSD_PLANE1_MEM_SIZE+ \
                                        CC_CONFIG_OSD_PLANE2_MEM_SIZE+  \
                                        CC_CONFIG_OSD_PLANE3_MEM_SIZE+  \
                                        CC_CONFIG_OSD_PLANE4_MEM_SIZE+  \
                                        CC_CONFIG_OSD_PLANE5_MEM_SIZE)
        
#define CC_CONFIG_OSD_3D_PLANE_MEM_SIZE     (CC_CONFIG_OSD_3D_PLANE1_MEM_SIZE+  \
                                            CC_CONFIG_OSD_3D_PLANE2_MEM_SIZE+   \
                                            CC_CONFIG_OSD_3D_PLANE3_MEM_SIZE+   \
                                            CC_CONFIG_OSD_3D_PLANE4_MEM_SIZE+   \
                                            CC_CONFIG_OSD_3D_PLANE5_MEM_SIZE)
        
#define CC_CONFIG_OSD_DEC_PLANE_MEM_SIZE    (CC_CONFIG_OSD_DEC_PLANE1_MEM_SIZE+ \
                                            CC_CONFIG_OSD_DEC_PLANE2_MEM_SIZE+  \
                                            CC_CONFIG_OSD_DEC_PLANE3_MEM_SIZE+  \
                                            CC_CONFIG_OSD_DEC_PLANE4_MEM_SIZE+  \
                                            CC_CONFIG_OSD_DEC_PLANE5_MEM_SIZE)
        
#define CC_CONFIG_GFX_TEMP_MEM_SIZE         (CC_CONFIG_OSD_PLANE3_MAX_W*    \
                                            CC_CONFIG_OSD_PLANE3_MAX_H*     \
                                            CC_CONFIG_OSD_CM_SIZE)

#ifdef CC_LINUX_GPU_SUPPORT
#define LINUX_GPU_BUFFER_SIZE            (8 * 1024 * 1028)
#else
#define LINUX_GPU_BUFFER_SIZE            (0)
#endif

#define DFB_SUM_MEM_SIZE                  (CC_CONFIG_OSD_PLANE_BASIC_MEM_SIZE+\
                                            CC_CONFIG_OSD_3D_PLANE_MEM_SIZE+    \
                                            CC_CONFIG_OSD_DEC_PLANE_MEM_SIZE+   \
                                            CC_CONFIG_GFX_TEMP_MEM_SIZE+        \
                                            LINUX_GPU_BUFFER_SIZE)
#endif                                                    

#ifdef CC_GFX_MMU
#define DIRECT_FB_MEM_SIZE                  ((DFB_SUM_MEM_SIZE + 4*1024)&(0xFFFFF000)) 
#else
#define DIRECT_FB_MEM_SIZE                  (76 * 1024 * 1024)
#endif

//----------------------------------------------------------------------------
// FB setup
//----------------------------------------------------------------------------

#define FB_MEM_SIZE                         (0x0)

//----------------------------------------------------------------------------
// FBM setup
//----------------------------------------------------------------------------
#ifndef FBM_MEM_CFG_SIZE
#define FBM_MEM_CFG_SIZE                    (FBM_MEM_CFG_MT5882_3DTV)
#endif /* FBM_MEM_CFG_SIZE */

#ifndef FBM_MEM_CHB_CFG_SIZE
#define FBM_MEM_CHB_CFG_SIZE                    (FBM_MEM_CFG_MT5882_3DTV_CHB)
#endif /* FBM_MEM_CHB_CFG_SIZE */

#ifndef FBM_MEM_CHC_CFG_SIZE
#define FBM_MEM_CHC_CFG_SIZE                    (FBM_MEM_CFG_MT5882_3DTV_CHC)
#endif /* FBM_MEM_CHC_CFG_SIZE */

//----------------------------------------------------------------------------
// Trustzone setup
//----------------------------------------------------------------------------
#ifdef CC_TRUSTZONE_SUPPORT
#define TRUSTZONE_CODE_BASE                 (0x5000)
#define TRUSTZONE_MEM_SIZE                  (18 * 1024 * 1024)
#else
#define TRUSTZONE_MEM_SIZE                  (0)
#endif

#if defined(CC_TRUSTZONE_IN_CHB)
#define TRUSTZONE_MEM_SIZE_CHA (0)
#else
#define TRUSTZONE_MEM_SIZE_CHA TRUSTZONE_MEM_SIZE
#endif

//----------------------------------------------------------------------------
// Kernel mem setup
//----------------------------------------------------------------------------


#define LINUX_KERNEL_MEM_SIZE               ((TOTAL_MEM_SIZE > (FBM_MEM_CFG_SIZE + DIRECT_FB_MEM_SIZE + FB_MEM_SIZE + TRUSTZONE_MEM_SIZE_CHA)) ?\
                                            (TOTAL_MEM_SIZE - FBM_MEM_CFG_SIZE - DIRECT_FB_MEM_SIZE - FB_MEM_SIZE - TRUSTZONE_MEM_SIZE_CHA) : (54 * 1024 * 1024))                                   

#define DEFAULT_TARGET_COUNTRY              (COUNTRY_BZ)  

//----------------------------------------------------------------------------
// Default Panel setting
//----------------------------------------------------------------------------
#ifndef DEFAULT_PANEL_SELECT
#define DEFAULT_PANEL_SELECT                (PANEL_DEFAULT)
#endif
#define BACKLIGHT_GPIO                      (OPCTRL(10))
#define BACKLIGHT_ON_VAL                    (0)
#define PANELPOWER_GPIO                     (OPCTRL(8))
#define PANELPOWER_ON_VAL                   (1)

// disable spread spectrum for vga output
#define SPREAD_SPECTRUM_PERMILLAGE          (0)

// pwm backlight adjustment setting
#define PANEL_BACKLIGHT_PWM_PORT            (1)

#define PDWNC_POLARITY                      (0)

//----------------------------------------------------------------------------
// Suspend power control GPIO Number, it must be PDWNC GPIO
//----------------------------------------------------------------------------
#define  SUSPEND_GPIO_NUMBER   	(212) //DDR power control.
#define  SUSPEND_GPIO_POL   	(0) //0:active low,1:active high.

//----------------------------------------------------------------------------
// Loader logo 
//----------------------------------------------------------------------------
#ifdef CC_LOADER_LOGO_SUPPORT
#define LOADER_LOGO_NUMBER                  (1)
#define LOADER_LOGO_H_FILE                  "mtk/MediaTek_600x100x5.h"
#endif

//----------------------------------------------------------------------------
// Audio Customization
//----------------------------------------------------------------------------
#define AUDIO_INCLUDE_FILE                  "mtk/Mustang_aud.h"
#define AUDIO_Mute_Gpio_Num                 (OPCTRL(9))
#define AUDIO_Amp_Gpio_Num                  (OPCTRL(4))
#define AUDIO_HP_PLUG_IN_GPIO               (OPCTRL(29))
#define AUDIO_MUTE_GPIO_POLARITY            AUD_MUTE_GPIO_LOW_ENALBE
#define AUDIO_AMP_GPIO_POLARITY             AUD_AMP_GPIO_LOW_ENALBE
#define AUDIO_HP_PLUG_IN_POLARITY           (AUD_GPIO_LOW_ENALBE)
#define AUDIO_INPUT_MUX_SEL_TABLE           (&_rMustangAudInputMuxTable)
#define AUDIO_APROC_ENABLE                  (TRUE)

//----------------------------------------------------------------------------
// Other GPIO customization
#define DEFAULT_TUNER_RFAGC_SWITCH_GPIO     (81)//d120203_Haibo:OPWM2 is for RFAGC switch
//----------------------------------------------------------------------------
#define FLAG_EXT_STATUS                     (0)

#define DEFAULT_DEMOD_RESET_GPIO            (GPIO(57))

//----------------------------------------------------------------------------
//Tuner I2C channel ID+Demod I2C channel ID
//----------------------------------------------------------------------------
#ifdef CC_DUAL_TUNER_SUPPORT
//Tuner I2C channel ID
#define DEFAULT_TUNER_I2C_BUS_ID  4//default use sif_bus_tuner_i2c
//Demod I2C channel ID
#define DEFAULT_DEMOD_I2C_BUS_ID  4//default use sif_bus_tuner_i2c
//For internal Demod use
#define SIF_ENABLE_PDWNC_MASTER      (1)
#define SIF_ENABLE_PDWNC_MASTER1      (1)
#endif
//----------------------------------------------------------------------------
// EEPROM customization
//----------------------------------------------------------------------------
#define SYSTEM_EEPROM_SIZE                  (16384) //16KBytes
#define SYSTEM_EEPROM_WP_GPIO               (GPIO(79)) // OPWMO.
#define SYSTEM_EEPROM_WP_ENABLE_POLARITY    (1)
//#define EDID_EEPROM_WP_GPIO               (GPIO(4))   // GPIO14.
//#define EDID_EEPROM_WP_ENABLE_POLARITY    (1)
#ifdef CC_HOTKEY_WAKEUP_SUPPORT
#define SUPPORT_HOTKEY_WAKEUP				(1)
#endif

#ifdef CC_ENABLE_REBOOT_REASON
    #ifndef DRVCUST_EEPROM_ANDROID_OTA_SIZE
        #define DRVCUST_EEPROM_ANDROID_OTA_SIZE    ((UINT32)16)
    #endif /* DRVCUST_EEPROM_ANDROID_OTA_SIZE */

    #ifndef DRVCUST_EEPROM_ANDROID_OTA_OFFSET
        #define DRVCUST_EEPROM_ANDROID_OTA_OFFSET    ((UINT32)0x800) /* Reserved for OTA in APP space */
    #endif /* DRVCUST_EEPROM_ANDROID_OTA_OFFSET */
#endif /* CC_ENABLE_REBOOT_REASON */

#ifdef CC_MTK_AUTO_TEST
#ifndef DRVCUST_EEPROM_LINUX_OTA_SIZE
#define DRVCUST_EEPROM_LINUX_OTA_SIZE    ((UINT32)16)
#endif /* DRVCUST_EEPROM_ANDROID_OTA_SIZE */

#ifndef DRVCUST_EEPROM_LINUX_OTA_OFFSET
#define DRVCUST_EEPROM_LINUX_OTA_OFFSET    ((UINT32)(DRVCUST_EEPROM_HDCP2X_OFFSET-DRVCUST_EEPROM_LINUX_OTA_SIZE))
#endif

#ifndef DRVCUST_EEPROM_AUTOTEST_UPGRADED_PKG_SIZE
#define DRVCUST_EEPROM_AUTOTEST_UPGRADED_PKG_SIZE      ((UINT32)1)
#endif /* DRVCUST_EEPROM_AUTOTEST_UPGRADED_PKG_SIZE */

#ifndef DRVCUST_EEPROM_AUTOTEST_UPGRADED_PKG_OFFSET
#define DRVCUST_EEPROM_AUTOTEST_UPGRADED_PKG_OFFSET    ((UINT32)(DRVCUST_EEPROM_LINUX_OTA_OFFSET-DRVCUST_EEPROM_AUTOTEST_UPGRADED_PKG_SIZE))
#endif /* DRVCUST_EEPROM_AUTOTEST_UPGRADED_PKG_OFFSET */
#endif /* CC_MTK_AUTO_TEST */


//----------------------------------------------------------------------------
// USB Customization
//----------------------------------------------------------------------------
// "USBx_PORT_USING_LOADER & USBx_PORT_USING" related define is only for USB2.0 port
// 1. Loader, USB port using define (1)=used (0)=not used
#define USB0_PORT_USING_LOADER              (1)
#define USB1_PORT_USING_LOADER              (1)
#define USB2_PORT_USING_LOADER              (1)
#define USB3_PORT_USING_LOADER              (1)

// 2. Nuclues/Linux/Android System USB port using define (1)=used (0)=not used
#define USB0_PORT_USING                     (1)
#define USB1_PORT_USING                     (1) 
#define USB2_PORT_USING                     (1) 
#define USB3_PORT_USING                     (1)

// 3. USB +5V power switch Vbus GPIO control, if vbus not control by GPIO, mark off it.
#define USB0_VBUS_GPIO                      GPIO(44)    // GPIO66.
#define USB0_VBUS_GPIO_POLARITY		        (1)
#define USB1_VBUS_GPIO                      GPIO(45)    // GPIO43
#define USB1_VBUS_GPIO_POLARITY		        (1)
#define USB2_VBUS_GPIO                      GPIO(46)    // GPIO44
#define USB2_VBUS_GPIO_POLARITY		        (1)
#define USB3_VBUS_GPIO                      GPIO(47)  // OPCTRL10
#define USB3_VBUS_GPIO_POLARITY		        (1)

// 4. USB power switch overcurrent GPIO control, if have not ic to control by GPIO, mark off it.
#define USB0_OC_GPIO                        ADIN(4)     // ADIN 4.
#define USB0_OC_GPIO_POLARITY		        (0)
#define USB1_OC_GPIO                        ADIN(4)     // ADIN 4.
#define USB1_OC_GPIO_POLARITY		        (0)
#define USB2_OC_GPIO                        ADIN(5)     // ADIN 5.
#define USB2_OC_GPIO_POLARITY		        (0)
#define USB3_OC_GPIO                        ADIN(5)     // ADIN 5.
#define USB3_OC_GPIO_POLARITY		        (0)

// 5. Detect USB upgrade time, default is 0, but some device maybe can't detect when upgrade.
// If possible, set it to (300)ms when in customer's module.
#define USB_LOADER_WAIT_TIME                (1000)

//#define USB_DISABLE_SUSPEND_MODE /* Disable USB suspend, share 240 Mhz with H.264.*/

//----------------------------------------------------------------------------
// sdcard gpio define
//----------------------------------------------------------------------------
#define MSDC_DETECT_GPIO  GPIO(234)	//ADIN7
#define MSDC_VOLTAGE_GPIO GPIO(43)	//SDIO_PWR_CTRL

//#define USB_DISABLE_SUSPEND_MODE /* Disable USB suspend, share 240 Mhz with H.264.*/
//----------------------------------------------------------------------------
// Button Customization
//----------------------------------------------------------------------------
#define FLAG_BUTTON_SUPPORT                 (0)

#ifndef T8032UP_INCLUDE_FILE
#define T8032UP_INCLUDE_FILE                "mtk/mtk_t8032fw.h"
#define T8032UP_OFFSET                      au1T8032FW
#ifdef NDEBUG
#define T8032UP_SIZE                        T8032FW_SIZE
#else
#define T8032UP_SIZE                        24576//16384
#endif
#endif

//----------------------------------------------------------------------------
// MTK IR only setting
//----------------------------------------------------------------------------
#define IR_INCLUDE_FILE                     "mtk/mtk_ir.h"

#ifndef NPTV_SOURCE_TABLE
#define NPTV_SOURCE_TABLE                   "mtk/source_table.c"
#endif

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
#define VGA_INCLUDE_FILE                    "mtk/mtk_vga.h"
#endif

//----------------------------------------------------------------------------
// Tcon Customization
//----------------------------------------------------------------------------
// support LVDS and MLVDS both
#define PANEL_DISPLAY_TYPE_ALLOWED_MASK     ((1<<PANEL_DISPLAY_TYPE_LVDS) | (1 << PANEL_DISPLAY_TYPE_MLVDS)| (1 << PANEL_DISPLAY_TYPE_EPI) | (1 << PANEL_DISPLAY_TYPE_VB1))

#ifndef TCON_CUST_FILE
#define TCON_CUST_FILE                      "mtk/Capri_tcon_cust.c"
#endif
#define SUPPORT_LOCK_FROM_SRVAD 

//----------------------------------------------------------------------------
// SCART out configuration
//----------------------------------------------------------------------------
#ifndef TVE_INCLUDE_FILE
#define TVE_INCLUDE_FILE                    "mtk/mtk_tve.c"
#endif

//----------------------------------------------------------------------------
// DMX configuration
//----------------------------------------------------------------------------
#ifndef DMX_INCLUDE_FILE
#define DMX_INCLUDE_FILE                    "mtk/mtk_dmx.h"
#endif

//----------------------------------------------------------------------------
// Loader configuration
//----------------------------------------------------------------------------
#ifdef CC_USE_256MB_NAND
#define MAX_UPGRADE_IMAGE_FILE_SIZE         (256*1024*1024)     /* 256 MB */
#else
#define MAX_UPGRADE_IMAGE_FILE_SIZE         (512*1024*1024) //512MB
#endif

//----------------------------------------------------------------------------
// Flash Partition configuration
//----------------------------------------------------------------------------
#ifdef CC_USE_256MB_NAND
    #define NAND_FLASH_TOTAL_SIZE               (256*1024*1024)     /* 256 MB */
#else
    #define NAND_FLASH_TOTAL_SIZE               (512*1024*1024)     
#endif

#define PART_RESERVED_SIZE              (0x0040000)             /* reserved part size 256KB     */
#define PART_SDM_BAD_SECTOR_SIZE        (0x0500000)             /* Bad setor size of SDM part 5MB  */

#define PART_LOADER_SIZE                (0x0200000)             /* Loader size 2M   */
#define PART_UBOOT_ENV_SIZE             (0x0200000)             /* Uboot env size 2M  */
#define PART_KERNEL_SIZE                (0x0400000)             /* Kernel size  4M */

#ifdef CC_DISABLE_BASIC_PART
	
	#ifndef CC_MSDC_ENABLE
	#define PART_ROOTFS_SIZE			(0x2300000)
	#else
    #define PART_ROOTFS_SIZE            (0x2800000)				/* rootfs size 40MB */
	#endif
	#define PART_BASIC_SIZE             (PART_RESERVED_SIZE)	/* Basic size 256K  */
#else
    #ifndef CC_EMMC_SUPPORT
    #define PART_ROOTFS_SIZE            (0x0F00000)         /* rootfs szie 15MB */
    #else
    #define PART_ROOTFS_SIZE            (0x5000000)         /* rootfs size 80MB */
    #endif
    #define PART_BASIC_SIZE             (0x1E00000)         /* Basic size 30MB  */
#endif

#define PART_PERM_SIZE                  (0x0800000)         /* perm size 8MB */
#define PART_CHANNEL_SIZE               (0x0040000)         /* channel list size 256KB */
#define PART_PQ_SIZE                    (0x0040000)         /* PQ size 256KB */
#define PART_AQ_SIZE                    (0x0040000)         /* AQ size 256KB */
#define PART_CAPTURE_LOGO_SIZE          (0x0100000)         /* Logo size 1MB */
#define PART_ACFG_DESC_SIZE 			(0x0040000)         /* Acfg description	size 256KB */
#define PART_SCHED_PVR_SIZE             (0x0040000)         /* Schedule PVR size 256K */
#ifdef CC_HDMI_2_0_HDCP_BIN
    #define PART_FLASH_HDCP_BIN_SIZE         (256*1024)         /* HDMI 2.0 driver partition size 256K */ 
#endif
    
#ifdef ADSP_BIN_SUPPORT
#define PART_ADSP_SIZE                  (0x00300000)        /* audio dsp size 3MB */
#else
#define PART_ADSP_SIZE  PART_RESERVED_SIZE
#endif

/* BDM without 3rd size */
#define BDM_WO_3RD_SIZE (NAND_PART_SIZE_0 + NAND_PART_SIZE_1 + \
                         NAND_PART_SIZE_2 + NAND_PART_SIZE_3 + \
                         NAND_PART_SIZE_4 + NAND_PART_SIZE_5 + \
                         NAND_PART_SIZE_6 + NAND_PART_SIZE_7 + \
                         NAND_PART_SIZE_8 + NAND_PART_SIZE_9 + \
                         NAND_PART_SIZE_12)
                         
/* SDM size, part 13 - 22 + bad setor */
#define SDM_TOTAL_SIZE (NAND_PART_SIZE_13 + NAND_PART_SIZE_14 + \
                        NAND_PART_SIZE_15 + NAND_PART_SIZE_16 + \
                        NAND_PART_SIZE_17 + NAND_PART_SIZE_18 + \
                        NAND_PART_SIZE_19 + NAND_PART_SIZE_20 + \
                        NAND_PART_SIZE_21 +	NAND_PART_SIZE_22 + \
                        PART_SDM_BAD_SECTOR_SIZE)


#ifdef GINGA_SUPPORT_STICKER
#define PART_GINGA_SC_SIZE	0x1400000
#define NAND_PART_NAME_11		"ginga_app"
#define NAND_PART_ENCRYPT_11	(1)
#else
#define PART_GINGA_SC_SIZE	PART_RESERVED_SIZE
#endif

#ifdef CC_3RD_RO
	#ifdef GINGA_SUPPORT_STICKER
	#define PART_3RD_RO_SIZE                (0xC800000 - PART_GINGA_SC_SIZE)                 /* 3rd read only size 192MB */
	#elif CC_MSDC_ENABLE
	#define PART_3RD_RO_SIZE				(0x8700000) 				/* 135MB */
	#else
	#define PART_3RD_RO_SIZE                (0xC800000)                 /* 3rd read only size 200 MB */
	#endif
#else
    #define PART_3RD_RO_SIZE                (PART_RESERVED_SIZE)
#endif  

#define PART_3RD_RW_SIZE                (NAND_FLASH_TOTAL_SIZE - SDM_TOTAL_SIZE - BDM_WO_3RD_SIZE  - PART_3RD_RO_SIZE)

#define NAND_PART_SIZE_0                (PART_LOADER_SIZE)          /* nand 0 - loader image     */
#define NAND_PART_SIZE_1                (PART_UBOOT_ENV_SIZE)       /* nand_1 - uboot env        */
#define NAND_PART_SIZE_2                (PART_RESERVED_SIZE)      /* nand_2 - reserve A            */
#define NAND_PART_SIZE_3                (PART_RESERVED_SIZE)      /* nand_3 - reserve B            */
#define NAND_PART_SIZE_4                (PART_KERNEL_SIZE)          /* nand_4 - kernel A         */
#define NAND_PART_SIZE_5                (PART_KERNEL_SIZE)          /* nand_5 - kernel B         */
#define NAND_PART_SIZE_6                (PART_ROOTFS_SIZE)          /* nand_6 - rootfs A         */
#define NAND_PART_SIZE_7                (PART_ROOTFS_SIZE)          /* nand_7 - rootfs B         */
#define NAND_PART_SIZE_8                (PART_BASIC_SIZE)           /* nand_8 - basic            */
#define NAND_PART_SIZE_9                (PART_PERM_SIZE)            /* nand_9 - perm            */
#define NAND_PART_SIZE_10               (PART_3RD_RO_SIZE)          /* nand_10 - 3rd ro   */
#define NAND_PART_SIZE_11               (PART_3RD_RW_SIZE)          /* nand_11 - 3rd rw           */
#define NAND_PART_SIZE_12               (PART_GINGA_SC_SIZE)        /* nand_12 - reserved or StickerCenter */

#define NAND_PART_SIZE_13               (PART_CHANNEL_SIZE)         /* nand_13 - channel list A  */
#define NAND_PART_SIZE_14               (PART_CHANNEL_SIZE)         /* nand_14 - channel list B  */
#define NAND_PART_SIZE_15               (PART_PQ_SIZE)              /* nand_15 - Flash PQ        */
#define NAND_PART_SIZE_16               (PART_AQ_SIZE)              /* nand_16 - Flash AQ        */
#define NAND_PART_SIZE_17               (PART_CAPTURE_LOGO_SIZE)    /* nand_17 - Capture Logo    */
#define NAND_PART_SIZE_18				(PART_ACFG_DESC_SIZE)	    /* nand_18- Acfg description	 */
#define NAND_PART_SIZE_19               (PART_SCHED_PVR_SIZE)       /* nand_19 - Schedule PVR     */
#define NAND_PART_SIZE_20               (PART_ADSP_SIZE)            /* nand_20 - audio dsp */
#ifdef CC_HDMI_2_0_HDCP_BIN
    #define NAND_PART_SIZE_21               (PART_FLASH_HDCP_BIN_SIZE)      /* nand_21 - HDMI 2.0 driver */
#else
	#define NAND_PART_SIZE_21				(PART_RESERVED_SIZE)			/* nand_21 - reserve */
#endif
#define	NAND_PART_SIZE_22				(PART_RESERVED_SIZE)		/* nand_22 - reserve */

#ifdef CC_HDMI_2_0_HDCP_BIN
    #define FLASH_HDCP_BIN_PARTITION        (21)
#endif

#define NAND_FLASH_STATIC_LAST_PART_ID  (12)

#define NAND_FLASH_STATIC_PART_SIZE \
        ((NAND_PART_SIZE_0) + \
        (NAND_PART_SIZE_1) + (NAND_PART_SIZE_2) + (NAND_PART_SIZE_3) + \
        (NAND_PART_SIZE_4) + (NAND_PART_SIZE_5) + (NAND_PART_SIZE_6) + \
        (NAND_PART_SIZE_7) + (NAND_PART_SIZE_8) + (NAND_PART_SIZE_9) + \
        (NAND_PART_SIZE_10) + (NAND_PART_SIZE_11) + (NAND_PART_SIZE_12))


#ifdef CC_FBM_MAPPING_ONE_BY_ONE
#ifndef FBM_BOOT_MMAP_INCLUDE_FILE
#define FBM_BOOT_MMAP_INCLUDE_FILE           "mtk/mtk_fbm_mmap.h"
#endif
#endif
        
#ifdef SUPPORT_FLASH_PQ
#define FLASH_PQ_BASE_PARTITION         (15) 
#endif

#ifdef SUPPORT_FLASH_AQ
#ifdef LINUX_TURNKEY_SOLUTION
#define FLASH_AQ_BASE_PARTITION         (16)
#endif
#endif

#ifdef ADSP_BIN_SUPPORT
#ifdef LINUX_TURNKEY_SOLUTION
#define FLASH_ADSP_BIN_PARTITION                (20)
#endif
#endif
  

#if defined(LINUX_TURNKEY_SOLUTION) || defined(__KERNEL__)
#ifdef CC_FBM_MAPPING_ONE_BY_ONE
#ifndef FBM_BOOT_MMAP_INCLUDE_FILE
#define FBM_BOOT_MMAP_INCLUDE_FILE           "mtk/mtk_fbm_mmap.h"
#endif
#endif
#endif

/*
 * default quality table
*/
#ifndef QTYTBL_INCLUDE_FILE
//#define QTYTBL_INCLUDE_FILE               "mtk/QTY_DEFAULT/mtk_QtyTbl.h"
#endif

/*
 * default color parameter
*/
#ifndef COLOR_PARAM_INCLUDE_FILE
//#define COLOR_PARAM_INCLUDE_FILE          "mtk/PANEL_DEFAULT/color_param.h"
#endif

/*
 * dynamic backlight parameter
*/

/*
 * OD setting
*/

#define SUPPORT_OD                          (0)

/*
 *  ENABLE WOW for WiFi
*/
#ifdef CC_WIFI_WOWLAN_SUPPORT
#define WAKEUP_ON_WLAN_GPIO 	(202)
#define RESET_ON_WLAN_GPIO 	(205)
    #ifdef CC_WIFI_WOWLAN_LOW_ACTIVE
        #define WAKEUP_ON_WLAN_POL 		(0)
    #else
        #define WAKEUP_ON_WLAN_POL 		(1)
    #endif
#endif

#else /* CONFIG_DEFAULT_H */
#error "Cannot include multi-config files"
#endif /* CONFIG_DEFAULT_H */

