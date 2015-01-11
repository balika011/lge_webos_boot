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

#ifndef DEFAULT_H
#define DEFAULT_H


//#ifndef DSP_SUPPORT_SRSTSHD_LEVELINDEP
//#define DSP_SUPPORT_SRSTSHD_LEVELINDEP
//#endif
//#ifndef DSP_SUPPORT_SRS_PURESOUND
//#define DSP_SUPPORT_SRS_PURESOUND
//#endif


//----------------------------------------------------------------------------
// Initial settings before dram config
//----------------------------------------------------------------------------
#ifndef DEFAULT_BOOT_VERBOSE
#ifdef NDEBUG
#define DEFAULT_BOOT_VERBOSE        0
#else
#define DEFAULT_BOOT_VERBOSE        1
#endif
#endif /* DEFAULT_BOOT_VERBOSE */

#ifndef DEFAULT_DRAM_PCB_CHK
#define DEFAULT_DRAM_PCB_CHK        (0)
#endif

#ifndef PCB_VER_CHECK_FUNC
#define PCB_VER_CHECK_FUNC          (NULL)
#endif

#ifndef DEFAULT_NOR_CPU_CLK
#define DEFAULT_NOR_CPU_CLK         (200000000)
#endif

#ifndef DEFAULT_DRAM_TYPE
#define DEFAULT_DRAM_TYPE   (DDR_III_x4)
#endif

#ifndef DEFAULT_DRAM_COLADDR
#define DEFAULT_DRAM_COLADDR        (COL_ADDR_BIT_10)
#endif

#ifndef DEFAULT_DDR_CLOCK
#define DEFAULT_DDR_CLOCK           (1350000000)
#endif

#ifndef DEFAULT_DDR_CL
#define DEFAULT_DDR_CL              (11)
#endif

#ifndef DEFAULT_DDR_BUS_X8
#define DEFAULT_DDR_BUS_X8              (0)  // Default is BUS X 16.
#endif

#ifndef DMPLL_SPECTRUM_PERMILLAGE
//#define DMPLL_SPECTRUM_PERMILLAGE   (100) // +- 1%.
#define DMPLL_SPECTRUM_PERMILLAGE   (50) // +- 0.5%.
//#define DMPLL_SPECTRUM_PERMILLAGE   (0) // disable DRAM spread specturm.
#endif

#ifndef DMPLL_SPECTRUM_FREQUENCY
//#define DMPLL_SPECTRUM_FREQUENCY   (60) // Khz.
#define DMPLL_SPECTRUM_FREQUENCY   (30) // Khz.
//#define DMPLL_SPECTRUM_FREQUENCY   (0) // disable DRAM spread specturm.
#endif

#ifndef DRAM_PRIORITY_LIST
// Audio/0 > B2R/5 > VBI/3D/TVE/2 > SCPOS/7 > NR/PSCAN/4 > OSD/OD/MMU/3 > MJC_IN/13 > MJC_OUT/14 > DEMUX/GCPU/1 > CPU/6 > VLD1/9 > VDEC_MC/8 > Test0/15 > GFX/11 > VENC/12 > 3DGFX/10
#define DRAM_PRIORITY_LIST          ((UINT8*)"08254193bafde67c")
#endif

#ifndef DRAM_BURSTLEN
#define DRAM_BURSTLEN               0
#define AGTIM0 0xFFFFFFFF
#define AGTIM1 0xFFFF77FF
#endif

#ifndef DRAM_GROUP1ARBITERTIME
#define DRAM_GROUP1ARBITERTIME        5
#endif
#ifndef DRAM_GROUP2ARBITERTIME
#define DRAM_GROUP2ARBITERTIME        4
#endif
#ifndef DRAM_GROUP3ARBITERTIME
#define DRAM_GROUP3ARBITERTIME        15
#endif

#ifndef DEFAULT_DRAM_8_BANKS
#define DEFAULT_DRAM_8_BANKS      1
#endif

#ifndef FLAG_DDR_QFP
#define FLAG_DDR_QFP                0
#endif

#ifndef FLAG_DDR_DCBALANCE
#define FLAG_DDR_DCBALANCE		 0
#endif

#ifndef FLAG_DDR_16BITSWAP
#define FLAG_DDR_16BITSWAP    (FLAG_DDR_QFP)
#endif

#if (FLAG_DDR_QFP == 1)

    #ifndef DEFAULT_DRAM_RODT
    #define DEFAULT_DRAM_RODT      150
    #endif

    #ifndef DEFAULT_DRAM_WODT
    #define DEFAULT_DRAM_WODT      120
    #endif

#else /* #if (FLAG_DDR_QFP == 1) */

    #ifndef DEFAULT_DRAM_RODT
    #define DEFAULT_DRAM_RODT      150
    #endif

    #ifndef DEFAULT_DRAM_WODT
    #define DEFAULT_DRAM_WODT      60
    #endif

#endif /* #if (FLAG_DDR_QFP == 1) */

#ifndef FLAG_RUN_ON_NOR
#define FLAG_RUN_ON_NOR             0
#endif

#ifndef SYS_RESET_TIMER
#define SYS_RESET_TIMER             (IS_XTAL_27MHZ()? 0xCDFE60 :0xB71B00)//0xcdfe60 <=> 500ms under 27MHz //0x59019 <=>13.5ms under 27MHz
#endif

#ifndef FBM_MEM_CFG_SIZE
#ifdef __MODEL_slt__
#define FBM_MEM_CFG_SIZE            (0x3000000+0x100000)     // add 1MB  for audio slt
#else
#define FBM_MEM_CFG_SIZE            (0x3000000)     // 49 MB
#endif
#endif

#ifndef FBM_MJC_RSV_POOL_SIZE
#define FBM_MJC_RSV_POOL_SIZE       (0x7800000)
#endif

#ifndef PDWNC_POLARITY
#define PDWNC_POLARITY              (0)
#endif

#ifndef LVDS_A_CTRL_EVEN
#define LVDS_A_CTRL_EVEN (0)
#endif
#ifndef LVDS_A_CTRL_ODD
#define LVDS_A_CTRL_ODD (1)
#endif
#ifndef LVDS_B_CTRL_EVEN
#define LVDS_B_CTRL_EVEN (2)
#endif
#ifndef LVDS_B_CTRL_ODD
#define LVDS_B_CTRL_ODD (3)
#endif

//----------------------------------------------------------------------------
// USB port support setting.
//----------------------------------------------------------------------------
#ifndef USB0_PORT_USING
#define USB0_PORT_USING     (1)
#endif
#ifndef USB1_PORT_USING
#define USB1_PORT_USING     (1)
#endif
#ifndef USB2_PORT_USING
#define USB2_PORT_USING     (1)
#endif
#ifndef USB3_PORT_USING
#define USB3_PORT_USING     (1)
#endif

//----------------------------------------------------------------------------
// Default usb upgrade setting
//----------------------------------------------------------------------------
#ifndef AUTO_USB_UPGRADE_ENABLE
#define AUTO_USB_UPGRADE_ENABLE     (0)
#endif
#ifndef EEPROM_USB_UPGRADE_ENABLE
#define EEPROM_USB_UPGRADE_ENABLE     (1)
#endif
#ifndef USB_UPGRADE_FILE_NAME
#define USB_UPGRADE_FILE_NAME       ("upgrade.pkg")
#endif
#ifndef USB_UPGRADE_FILE_NAME_PREFIX
#define USB_UPGRADE_FILE_NAME_PREFIX    ("upgrade")
#endif
#ifndef USB_UPGRADE_FILE_NAME_SUFFIX
#define USB_UPGRADE_FILE_NAME_SUFFIX    (".pkg")
#endif

#ifdef CC_MTK_AUTO_TEST

#ifndef USB_UPGRADE_GOLDEN_FILE_NAME
#define USB_UPGRADE_GOLDEN_FILE_NAME       ("upgrade.golden")
#endif

#ifndef USB_UPGRADE_GOLDEN_FILE_NAME_SUFFIX
#define USB_UPGRADE_GOLDEN_FILE_NAME_SUFFIX    (".golden")
#endif

#ifndef DRVCUST_EEPROM_AUTOTEST_UPGRADED_PKG_OFFSET
#define DRVCUST_EEPROM_AUTOTEST_UPGRADED_PKG_OFFSET    (0x10000)
#endif



#endif

#ifndef LOADER_STANDBY_ENABLE_IF_UPGRADE_BREAK
#define LOADER_STANDBY_ENABLE_IF_UPGRADE_BREAK (0)
#endif
#ifndef LOADER_UPGRADE_BREAK_HANDLE_FUNC
#define LOADER_UPGRADE_BREAK_HANDLE_FUNC (NULL)
#endif
#ifndef LOADER_UPGRADE_VER_CMP_FUNC
#define LOADER_UPGRADE_VER_CMP_FUNC (NULL)
#endif
#ifndef LOADER_UPGRADE_DLM_CB_FUNC
#define LOADER_UPGRADE_DLM_CB_FUNC (NULL)
#endif
#ifndef LOADER_UPGRADE_DLM_GET_TAG_VER_CB_FUNC
#define LOADER_UPGRADE_DLM_GET_TAG_VER_CB_FUNC (NULL)
#endif
#ifndef LOADER_UPGRADE_DLM_SET_TAG_VER_CB_FUNC
#define LOADER_UPGRADE_DLM_SET_TAG_VER_CB_FUNC (NULL)
#endif
#ifndef LOADER_UPGRADE_DLM_CMP_TAG_VER_CB_FUNC
#define LOADER_UPGRADE_DLM_CMP_TAG_VER_CB_FUNC (NULL)
#endif
#ifndef LOADER_UPGRADE_DLM_VER_DATA_OFF
#define LOADER_UPGRADE_DLM_VER_DATA_OFF 12
#endif
#ifndef LOADER_UPGRADE_DLM_VER_DATA_LEN
#define LOADER_UPGRADE_DLM_VER_DATA_LEN 8
#endif
#ifndef LOADER_UPGRADE_DLM_FORCE_VER
#define LOADER_UPGRADE_DLM_FORCE_VER "99999999"
#endif
#ifndef LOADER_UPGRADE_ONLY_ACON
#define LOADER_UPGRADE_ONLY_ACON 1
#endif
//----------------------------------------------------------------------------
// Loader Misc
//----------------------------------------------------------------------------
#ifndef LOADER_LOGO_CHECK_FUNC
#define LOADER_LOGO_CHECK_FUNC       (NULL)
#endif
#ifndef LOADER_ENV_INIT_FUNC
#define LOADER_ENV_INIT_FUNC        (NULL)
#endif
#ifndef LOADER_CEC_CONFIG_QUERY_FUNC
#define LOADER_CEC_CONFIG_QUERY_FUNC    (NULL)
#endif
#ifndef LOADER_Standby_CHECK_FUNC
#define LOADER_Standby_CHECK_FUNC       (NULL)
#endif
#ifndef LOADER_USB_UPGRADE_CHECK_FUNC
#define LOADER_USB_UPGRADE_CHECK_FUNC   (NULL)
#endif

//----------------------------------------------------------------------------
// Loader logo
//----------------------------------------------------------------------------
#ifndef LOADER_LOGO_FLASHADDR
#define LOADER_LOGO_FLASHADDR       (0)
#endif
#ifndef LOADER_LOGO_NUMBER
#define LOADER_LOGO_NUMBER          (0)
#endif
#ifndef LOADER_LOGO_BACKGROUND
#define LOADER_LOGO_BACKGROUND      (0xeeeeee)
#endif
#ifndef LOADER_MSG_BACKGROUND
#define LOADER_MSG_BACKGROUND       (0xeeeeee)
#endif

//----------------------------------------------------------------------------
// Driver logo
//----------------------------------------------------------------------------
#ifndef DRV_LOGO_ENABLE
#define DRV_LOGO_ENABLE       (0)
#endif

//----------------------------------------------------------------------------
// Default Tuner setting
//----------------------------------------------------------------------------
#ifndef DEFAULT_TUNER_CTRL
#define DEFAULT_TUNER_CTRL            (0)
#endif
#ifndef DEFAULT_TUNER_TYPE_DEFAULT
#define DEFAULT_TUNER_TYPE_DEFAULT    (0)
#endif
/*
#ifndef DEFAULT_TUNER_TYPE_OFST0
#define DEFAULT_TUNER_TYPE_OFST0    (0x109)
#endif
#ifndef DEFAULT_TUNER_TYPE_OFST1
#define DEFAULT_TUNER_TYPE_OFST1    (0x4DE)
#endif
#ifndef DEFAULT_TUNER_TYPE_OFST2
#define DEFAULT_TUNER_TYPE_OFST2    (0x7F6)
#endif

#ifndef DEFAULT_TUNER_CTRL1
#define DEFAULT_TUNER_CTRL1        (OPCTRL(1))
#endif
#ifndef DEFAULT_DEMOD_RESET_GPIO
#define DEFAULT_DEMOD_RESET_GPIO    (GPIO(65))
#endif
*/
/*SIF_BUS_SYSTEM_I2C = 0,
    SIF_BUS_TUNER_I2C  = 1,
    SIF_BUS_SECOND_I2C = 2,
    SIF_BUS_PDWNC_I2C  = 3,
    SIF_BUS_8295B_I2C_0 = 4,
    SIF_BUS_8295B_I2C_1 = 5,
    SIF_BUS_MAX
*/
//Tuner I2C channel ID
#ifndef DEFAULT_TUNER_I2C_BUS_ID
#define DEFAULT_TUNER_I2C_BUS_ID  1//default use sif_bus_tuner_i2c
#endif

//Demod I2C channel ID
#ifndef DEFAULT_DEMOD_I2C_BUS_ID
#define DEFAULT_DEMOD_I2C_BUS_ID  1//default use sif_bus_tuner_i2c
#endif


#ifndef DEFAULT_DEMOD_RESET_GPIO
#define DEFAULT_DEMOD_RESET_GPIO    (0xffffffff)
#endif


#ifndef DEFAULT_DVBS_DEMOD_RESET_GPIO
#define DEFAULT_DVBS_DEMOD_RESET_GPIO    (GPIO(0xffffffff))
#endif

//----------------------------------------------------------------------------
// Default Panel setting
//----------------------------------------------------------------------------
#ifndef DEFAULT_PANEL_SELECT
#define DEFAULT_PANEL_SELECT        (PANEL_DEFAULT)
#endif
// backlight power on/off gpio pin
// #ifndef BACKLIGHT_GPIO
// #define BACKLIGHT_GPIO              (GPIO(3))
// #endif
// gpio output value when turn on backlight
// #ifndef BACKLIGHT_ON_VAL
// #define BACKLIGHT_ON_VAL            (0)
// #endif
// lvds power on/off gpio pin
// #ifndef PANELPOWER_GPIO
// #define PANELPOWER_GPIO             (GPIO_OPWM2)//(GPIO(3))
// #endif
// gpio output value when turn on lvds
// #ifndef PANELPOWER_ON_VAL
// #define PANELPOWER_ON_VAL           (0)
// #endif

// spread spectrum range, default is 1% for customer model
#ifndef SPREAD_SPECTRUM_PERMILLAGE
#define SPREAD_SPECTRUM_PERMILLAGE  (10)
#endif

//back light dimming (pwm0/1/2)
#ifndef PANEL_BACKLIGHT_PWM_PORT
#define PANEL_BACKLIGHT_PWM_PORT            (1)
#endif

// customer could define second backlight gpio pin
// #define BACKLIGHT_GPIO2            (GPIO_OPWM2)

// customer could define LVDS_MSB_SW to overwrite panel setting
// {MSB 5,4,3,2,1,0,7,6 LSB}
// #define LVDS_MSB_SW                (LVDS_MSB_SW_ON)
// {MSB 7,6,5,4,3,2,1,0 LSB}
// #define LVDS_MSB_SW                (LVDS_MSB_SW_OFF)

// customer could define different pwm polarity
// #define PWM_POLARITY               (PWM_LOW_PANEL_BRIGHT)

// dimming frequency in 50/60hz frame rate, 0 means use system default
#define DIMMING_FREQ_50         (100)
#define DIMMING_FREQ_60         (120)

// customer could have different backlight range request
// #define BACKLIGHT_HIGH             (0x80)
// #define BACKLIGHT_MIDDLE           (0x80)
// #define BACKLIGHT_LOW              (0x80)

#ifndef PANEL_CHANNEL_SWAP
#define PANEL_CHANNEL_SWAP          (0)
#endif

#ifndef PANEL_INDEX_FROM_GPIO_NUM
#define PANEL_INDEX_FROM_GPIO_NUM   (0)
#define PANEL_INDEX_FROM_GPIO_IDX   (NULL)
#define PANEL_INDEX_FROM_GPIO_MAP   (NULL)
#endif

// code segment for turning on the option
/*
static const UINT32 _au4PanelGpioIdx[3] = {13, 14, 15};
static const UINT32 _au4PanelGpioMap[8] = {24, 25, 26, 27, 28, 15, 30, 31};

#define PANEL_INDEX_FROM_GPIO_NUM   (sizeof(_au4PanelGpioIdx)/sizeof(UINT32))
#define PANEL_INDEX_FROM_GPIO_IDX   (&_au4PanelGpioIdx)
#define PANEL_INDEX_FROM_GPIO_MAP   (&_au4PanelGpioMap)
*/

#ifndef PANEL_LVDS_8BIT_PIN_MAP
#define PANEL_LVDS_8BIT_PIN_MAP    (NULL)
#endif

#ifndef PANEL_LVDS_10BIT_PIN_MAP
#ifdef CC_LGE_PROTO_PCBA
//#define PANEL_LVDS_10BIT_PIN_MAP    (NULL)
#else
#define PANEL_LVDS_10BIT_PIN_MAP    (NULL)
#endif
#endif

#ifndef PANEL_LVDS_10BIT_PN_SWAP
#ifdef CC_LGE_PROTO_PCBA
//#define PANEL_LVDS_10BIT_PN_SWAP    (NULL)
#else
#define PANEL_LVDS_10BIT_PN_SWAP    (NULL)
#endif

#endif

//#define PANEL_VB1_LANE_SWAP (_pu4PanelVB1LaneSwap)
#ifndef PANEL_VB1_LANE_SWAP
#define PANEL_VB1_LANE_SWAP    (NULL)
#endif

//#define PANEL_VB1_LANE_PN_SWAP  (_pu4PanelVB1LanePNSwap)
#ifndef PANEL_VB1_LANE_PN_SWAP
#define PANEL_VB1_LANE_PN_SWAP    (NULL)
#endif

// sample for lvds pin map
/*
5382p verification board:
static const UINT32 _au4PanelLvds8bitPinMap[12] =
{LVDS_A0, LVDS_A1, LVDS_A2, LVDS_A3, LVDS_DEFAULT,
 LVDS_DEFAULT, LVDS_A5, LVDS_A6, LVDS_ACLK2, LVDS_A8,
 LVDS_ACLK1, LVDS_A7};

5382p product board:
static const UINT32 _au4PanelLvds8bitPinMap[12] =
{LVDS_A0, LVDS_A1, LVDS_A2, LVDS_A3, LVDS_DEFAULT,
 LVDS_A5, LVDS_A6, LVDS_A7, LVDS_A8, LVDS_DEFAULT,
 LVDS_ACLK1, LVDS_ACLK2};

#define PANEL_LVDS_8BIT_PIN_MAP   (&_au4PanelLvds8bitPinMap)
#define PANEL_LVDS_10BIT_PIN_MAP   (&_au4PanelLvds10bitPinMap)

*/

// use ttl output instead of LVDS
#ifndef PANEL_TTL_OUTPUT
#define PANEL_TTL_OUTPUT            (0)
#endif

// number of table configuration in flash
#ifndef PANEL_FLASH_TABLE_SIZE
#define PANEL_FLASH_TABLE_SIZE         (0)
#endif
#ifndef PANEL_FLASH_TABLE_ADDRESS
#define PANEL_FLASH_TABLE_ADDRESS      (0x28760100)
#endif
#ifndef PANEL_EEPROM_TO_FLASH_INDEX_MAP
#define PANEL_EEPROM_TO_FLASH_INDEX_MAP (NULL)
#endif

/*
// adjust LVDS driving current, 1 means use default value (3mA)
#ifndef LVDS_DRIVING_CURRENT
#define LVDS_DRIVING_CURRENT        (1)
#endif
*/

#ifndef PANEL_PIXEL_SHIFT_H_MAX
#define PANEL_PIXEL_SHIFT_H_MAX (0)
#endif
#ifndef PANEL_PIXEL_SHIFT_V_MAX
#define PANEL_PIXEL_SHIFT_V_MAX (0)
#endif

#ifndef PANEL_SUPPORT_24_25_30_OUTPUT
#define PANEL_SUPPORT_24_25_30_OUTPUT (0)
#endif

#ifndef PWM0_PORT
#define PWM0_PORT    PAD_PWM0_PWM0
#endif
#ifndef PWM1_PORT
#define PWM1_PORT    PAD_PWM1_PWM1
#endif
#ifndef PWM2_PORT
#define PWM2_PORT    PAD_PWM2_PWM2
#endif

#ifndef PANEL_DISPLAY_TYPE_ALLOWED_MASK
#define PANEL_DISPLAY_TYPE_ALLOWED_MASK ((1<<PANEL_DISPLAY_TYPE_LVDS)|(1<<PANEL_DISPLAY_TYPE_VB1))
#endif
//----------------------------------------------------------------------------
// Audio Customization
//----------------------------------------------------------------------------

#ifndef AUDIO_APROC_ENABLE
#if defined(CC_AUD_ARM11_SUPPORT)
#define AUDIO_APROC_ENABLE              (TRUE)
#else
#define AUDIO_APROC_ENABLE              (FALSE)
#endif // CC_AUD_ARM11_SUPPORT
#endif

#ifndef AUDIO_OUT_PAD_NORMAL
#define AUDIO_OUT_PAD_NORMAL            (0)
#endif
// Output pad driving current in unit of mA, could be set as 2/4/6/8
#ifndef AUDIO_OUT_PAD_DRV_CUR
#define AUDIO_OUT_PAD_DRV_CUR           (2)
#endif
// Adac MCLK clock, MCLK_256FS/MCLK384FS/..
#ifndef AUDIO_DAC_MCLK
#define AUDIO_DAC_MCLK MCLK_256FS
#endif

// Bass management cutoff freqency
#ifndef AUDIO_BASS_MNG_CUTOFF_FREQ
#define AUDIO_BASS_MNG_CUTOFF_FREQ      (100)
#endif
// DSP softmute_num = 128 * 2^(softmute_order)
#ifndef AUDIO_SOFT_MUTE_ORDER
#define AUDIO_SOFT_MUTE_ORDER           (3)
#endif
//BBE level
#ifndef AUDIO_BBE_LEVEL
#define AUDIO_BBE_LEVEL                 (0)
#endif
//BBE Process
#ifndef AUDIO_BBE_PROCESS
#define AUDIO_BBE_PROCESS               (14)
#endif
//BBE Lo Contour
#ifndef AUDIO_BBE_LO_CONTOUR
#define AUDIO_BBE_LO_CONTOUR            (14)
#endif
//BBE 3D gain
#ifndef AUDIO_BBE_3D_GAIN
#define AUDIO_BBE_3D_GAIN               (80)
#endif
// L/R speaker type, large/small
#ifndef AUDIO_SPK_TYPE_LARGE
#define AUDIO_SPK_TYPE_LARGE            (TRUE)
#endif
// LS/RS speaker type, large/small
#ifndef AUDIO_SURROUND_SPK_TYPE_LARGE
#define AUDIO_SURROUND_SPK_TYPE_LARGE   (TRUE)
#endif
// C speaker type, large/small
#ifndef AUDIO_CENTER_SPK_TYPE_LARGE
#define AUDIO_CENTER_SPK_TYPE_LARGE     (TRUE)
#endif
// CH9/10 speaker type, large/small
#ifndef AUDIO_CH9_10_SPK_TYPE_LARGE
#define AUDIO_CH9_10_SPK_TYPE_LARGE     (TRUE)
#endif
#ifndef AUDIO_SUBWOOFER_ON
#define AUDIO_SUBWOOFER_ON              (FALSE)
#endif


// Audio Description setting
#ifndef AUDIO_AUDIO_DESCRIPTION_FLAG
#define AUDIO_AUDIO_DESCRIPTION_FLAG     0x2  //0x2:If Audio description is on, then Audio description data is only added  in CH9/10
#endif                                                                 //0x4:If Audio descritpion is on , then Audio description data is added for all channel

// Downmix channel(CH9/10) postion setting
#ifndef AUDIO_DOWNMIX_CHANNEL_POSITION
#define AUDIO_DOWNMIX_CHANNEL_POSITION  (DOWNMIX_CH_DO_ALL_POST_PROC)
#endif                                                                      //0x1:doing downmix after input source gain and before AVC
                                                                               //0x3:Downmix channel do not do post processing
// Downmix signal to L/R ?
#ifndef AUDIO_DOWNMIX_LR
#define AUDIO_DOWNMIX_LR     0x0                          //0x0: didn't  Downmix to L/R channel after Reverbation and before BassManagement
#endif                                                                      //0x1:Downmix to L/R channel after Reverbation and before BassManagement

// Downmix signal with balance
#ifndef AUDIO_DOWNMIX_WITH_BALANCE
#define AUDIO_DOWNMIX_WITH_BALANCE     (TRUE)
#endif


#ifndef AUDIO_PANFADE_CONTROL
#define AUDIO_PANFADE_CONTROL     0x1              //0x0: Pan disable, Fade disable
                                                                                //0x1: Pan enable, Fade CH7/8, CH9/10, CHL/R
                                                                                //0x2: Pan enable, Fade CH9/10, CHL/R
                                                                                //0x3: Pan enable, Fade CHL/R
#endif

// Downmix signal to LS/RS/C/LFE ?
#ifndef AUDIO_MULTI_PAIR_OUTPUT
#define AUDIO_MULTI_PAIR_OUTPUT     0x0                  //0x0: didn't  Copy Downmix channel to all the other channels(LS/RS/C/SUB)
#endif                                                                      //0x1:Copy Downmix channel to all the other channels(LS/RS/C/SUB)

// BI2S from GPIO 2~5
#ifndef AUDIO_BI2S_ENABLE
#define AUDIO_BI2S_ENABLE               (FALSE)
#endif

//AI2S use for speaker
#ifndef AUDIO_AI2S_USE_FOR_SPEAKER
#define AUDIO_AI2S_USE_FOR_SPEAKER    (FALSE)
#endif

//HE-AAC only multichannel enable DDCO
#ifndef AUDIO_HEAAC_MULTICHL_DDCO
#define AUDIO_HEAAC_MULTICHL_DDCO    (TRUE)
#endif

// PCM prebuf bank number. 256 sample/bank
#ifndef AUDIO_PCM_PREBUF_BANK_NUM
#define AUDIO_PCM_PREBUF_BANK_NUM       (0)
#endif

// Bypass DSP post processing, only used in specified model (ex. SLT, testing)
#ifndef AUDIO_BYPASS_POST_PROC
#define AUDIO_BYPASS_POST_PROC          (FALSE)
#endif

#ifndef AUDIO_ADAC_USAGE
#define AUDIO_ADAC_USAGE_DEFAULT
#define AUDIO_ADAC_USAGE                (&_arMtkAdacUsage)
#endif

#ifndef AUDIO_AOUT_PAD_CFG
#define AUDIO_AOUT_PAD_CFG_DEFAULT
#define AUDIO_AOUT_PAD_CFG              (&_arMtkAoutPadTable)
#endif

#ifndef AUDIO_INTER_DAC_USAGE
#define AUDIO_INTER_DAC_USAGE_DEFAULT
#define AUDIO_INTER_DAC_USAGE           (&_arMtkInterDacUsage)
#endif

// If GPIO NUM = 0xffffffff, then don't use that
#ifndef AUDIO_Opctrl_Mute_Gpio_Num
#define AUDIO_Opctrl_Mute_Gpio_Num (0xffffffff)
#endif

#ifndef AUDIO_Mute_Gpio_Num
#define AUDIO_Mute_Gpio_Num             (0xffffffff)
#endif

#ifndef AUDIO_Amp_Gpio_Num
#define AUDIO_Amp_Gpio_Num              (0xffffffff)
#endif

#ifndef AUDIO_OPCTRL_MUTE_GPIO_POLARITY
#define AUDIO_OPCTRL_MUTE_GPIO_POLARITY AUD_OPCTRL_MUTE_GPIO_HIGH_ENALBE
#endif

#ifndef AUDIO_MUTE_GPIO_POLARITY
#define AUDIO_MUTE_GPIO_POLARITY        AUD_MUTE_GPIO_HIGH_ENALBE
#endif

#ifndef AUDIO_AMP_GPIO_POLARITY
#define AUDIO_AMP_GPIO_POLARITY         AUD_AMP_GPIO_HIGH_ENALBE
#endif

#ifndef AUDIO_AMP_RESET_GPIO_NUM
#define AUDIO_AMP_RESET_GPIO_NUM        (0xffffffff)
#endif

#ifndef AUDIO_SCART_Gpio_Num
#define AUDIO_SCART_Gpio_Num              (0xffffffff)
#endif

#ifndef AUDIO_SCART_GPIO_POLARITY
#define AUDIO_SCART_GPIO_POLARITY         AUD_GPIO_HIGH_ENALBE
#endif

#ifndef AUDIO_Arc_Gpio_Num
#define AUDIO_Arc_Gpio_Num              (0xffffffff)
#endif

#ifndef AUDIO_ARC_GPIO_POLARITY
#define AUDIO_ARC_GPIO_POLARITY         AUD_GPIO_HIGH_ENALBE
#endif

#ifndef AUDIO_AMP_CUST_FILE_NAME
#define AUDIO_AMP_CUST_FILE_NAME        "mtk/amp_cust.c"
#endif

#ifndef AUDIO_AMP_RESET_GPIO_POLARITY
#define AUDIO_AMP_RESET_GPIO_POLARITY   AUD_GPIO_LOW_ENALBE
#endif

#ifndef AUDIO_INPUT_SW_GPIO_SEL
#define AUDIO_INPUT_SW_GPIO_SEL_DEFAULT
#define AUDIO_INPUT_SW_GPIO_SEL         (&_arMtkInputSwGpio)
#endif

#ifndef AUDIO_INPUT_MUX_SEL_TABLE
#define AUDIO_INPUT_MUX_SEL_TABLE_DEFAULT
#define AUDIO_INPUT_MUX_SEL_TABLE       (&_rMtkAudInputMuxTable)
#endif

#ifndef AUDIO_OUT_FMT_TABLE
#define AUDIO_OUT_FMT_TABLE_DEFAULT
#define AUDIO_OUT_FMT_TABLE             (&_arMtkAudOutFmtTable)
#endif

#ifndef AUDIO_OUT_FMT_TABLE2
#define AUDIO_OUT_FMT_TABLE2_DEFAULT
#define AUDIO_OUT_FMT_TABLE2            (&_arMtkAudOutFmtTable2)
#endif

#ifndef AUDIO_IN_FMT_TABLE
#define AUDIO_IN_FMT_TABLE_DEFAULT
#define AUDIO_IN_FMT_TABLE              (&_arMtkAudInFmtTable)
#endif

#ifndef AUDIO_INPUT_SRC_VOL
#define AUDIO_INPUT_SRC_VOL_DEFAULT
#define AUDIO_INPUT_SRC_VOL             (&_aai2MtkAudInputSrcVol)
#endif

#ifndef AUDIO_AV_INPUT_SRC_VOL
#define AUDIO_AV_INPUT_SRC_VOL_DEFAULT
#define AUDIO_AV_INPUT_SRC_VOL          (&_ai2MtkAudAvInputSrcVol)
#endif

#ifndef AUDIO_INPUT_SRC_LINE_OUT_VOL
#define AUDIO_INPUT_SRC_LINE_OUT_VOL_DEFAULT
#define AUDIO_INPUT_SRC_LINE_OUT_VOL    (&_aai2MtkAudInputSrcLineOutVol)
#endif

#ifndef AUDIO_CHL_EQ_CFG
#define AUDIO_CHL_EQ_CFG_DEFAULT
#define AUDIO_CHL_EQ_CFG                (&_aai1MtkAudChlEqCfg)
#endif

#ifndef AUDIO_CHL_EQ_COEF_TBL
#define AUDIO_CHL_EQ_COEF_TBL_DEFAULT
#define AUDIO_CHL_EQ_COEF_TBL               (&_aau4MtkAudChlEqCoefTbl)
#endif

#ifndef AUDIO_CHL_PEQ_FQ_CFG
#define AUDIO_CHL_PEQ_FQ_CFG_DEFAULT
#define AUDIO_CHL_PEQ_FQ_CFG                (&_aai1MtkAudPeqFq)
#endif

#ifndef AUDIO_CHL_PEQ_BW_CFG
#define AUDIO_CHL_PEQ_BW_CFG_DEFAULT
#define AUDIO_CHL_PEQ_BW_CFG                (&_aai1MtkAudPeqBw)
#endif

#ifndef AUDIO_CHL_PEQ_GAIN_CFG
#define AUDIO_CHL_PEQ_GAIN_CFG_DEFAULT
#define AUDIO_CHL_PEQ_GAIN_CFG                (&_aai1MtkAudPeqGain)
#endif

#ifndef AUDIO_CHL_BASS_TREBLE_CFG
#define AUDIO_CHL_BASS_TREBLE_CFG_DEFAULT
#define AUDIO_CHL_BASS_TREBLE_CFG                (&_aai1MtkAudBassTrebleTable)
#endif

#ifndef AUDIO_CHL_BASS_TREBLE_COEF_TBL
#define AUDIO_CHL_BASS_TREBLE_COEF_TBL_DEFAULT
#define AUDIO_CHL_BASS_TREBLE_COEF_TBL          (&_aau4MtkAudChlBassTrebleCoefTbl)
#endif

#ifndef AUD_CHL_SOUND_MODE_CFG
#define AUD_CHL_SOUND_MODE_CFG_DEFAULT
#define AUD_CHL_SOUND_MODE_CFG                (&_arMtkSoundModeCase)
#endif
#ifndef AUDIO_CHL_VS_MODE_CFG
#define AUDIO_CHL_VS_MODE_CFG_DEFAULT
#define AUDIO_CHL_VS_MODE_CFG                (&_aai1MtkAudVSTable)
#endif

#ifndef AUDIO_INPUT_SRC_AGC_FLAG
#define AUDIO_INPUT_SRC_AGC_FLAG_DEFAULT
#define AUDIO_INPUT_SRC_AGC_FLAG        (&_aafgMtkAudInputSrcAgcFlag)
#endif

#ifndef AUDIO_INPUT_SRC_AGC_VOL
#define AUDIO_INPUT_SRC_AGC_VOL_DEFAULT
#define AUDIO_INPUT_SRC_AGC_VOL         (&_aau4MtkAudInputSrcAgcVol)
#endif

#ifndef AUDIO_VOL_BASE_LEVEL
#define AUDIO_VOL_BASE_LEVEL            (100)
#endif

#ifndef AUDIO_LINE_OUT_FIX_VOL
#define AUDIO_LINE_OUT_FIX_VOL            (100)
#endif

#ifndef AUDIO_POWER_ON_MUSIC_VOL
#define AUDIO_POWER_ON_MUSIC_VOL            (50)
#endif

#ifndef AUDIO_VOL_TABLE
#define AUDIO_VOL_TABLE_DEFAULT
#define AUDIO_VOL_TABLE                 (&_au4VolumeTable)
#endif

#ifndef AUDIO_CHANNEL_VOL_TABLE
#define AUDIO_CHANNEL_VOL_TABLE_DEFAULT
#define AUDIO_CHANNEL_VOL_TABLE         (&_au4ChannelVolumeTable)
#endif

#ifndef AUDIO_AVC_TARGET_LEV
#define AUDIO_AVC_TARGET_LEV_DEFAULT
#define AUDIO_AVC_TARGET_LEV            (&_ai2MtkAudAvcTarget)
#endif

#ifndef AUDIO_AVC_GAIN_UP
#define AUDIO_AVC_GAIN_UP_DEFAULT
#define AUDIO_AVC_GAIN_UP               (&_ai2MtkAudAvcGainUp)
#endif

#ifndef AUDIO_AVC_ADJUST_RATE
#define AUDIO_AVC_ADJUST_RATE_DEFAULT
#define AUDIO_AVC_ADJUST_RATE           (&_ai2MtkAudAvcAdjRate)
#endif

#ifndef AUDIO_AVC_UI_ADJUST_RATE
#define AUDIO_AVC_UI_ADJUST_RATE_DEFAULT
#define AUDIO_AVC_UI_ADJUST_RATE        (&_ai2MtkAudAvcUiAdjRate)
#endif

#ifndef AUDIO_AVC_ATTACK_THRES
#define AUDIO_AVC_ATTACK_THRES          (0)
#endif

#ifndef AUDIO_CHL_VOL_EXTRA_GAIN
#define AUDIO_CHL_VOL_EXTRA_GAIN_DEFAULT
#define AUDIO_CHL_VOL_EXTRA_GAIN        (&_aai1MtkAudChVolExtraGain)
#endif

#ifndef AUDIO_CHL_VOLUME_OFFSET_MAP
#define AUDIO_CHL_VOLUME_OFFSET_MAP_DEFAULT
#define AUDIO_CHL_VOLUME_OFFSET_MAP     (&_aai4MtkAudVolOffsetMap)
#endif

#ifndef AUDIO_CHANNEL_DELAY
#define AUDIO_CHANNEL_DELAY_DEFAULT
#define AUDIO_CHANNEL_DELAY             (&_au1MtkAudChDelay)
#endif

#if 1  //MT5387Merge ... check later
#ifndef AUDIO_IEC_COPYRIGHT
#define AUDIO_IEC_COPYRIGHT_DEFAULT
#define AUDIO_IEC_COPYRIGHT             (&_au1MtkAudIecCopyright)
#endif

#ifndef AUDIO_IEC_CATEGORY
#define AUDIO_IEC_CATEGORY_DEFAULT
#define AUDIO_IEC_CATEGORY              (&_au1MtkAudIecCategory)
#endif

#ifndef AUDIO_IEC_WORD_LENGTH
#define AUDIO_IEC_WORD_LENGTH_DEFAULT
#define AUDIO_IEC_WORD_LENGTH           (&_au1MtkAudIecWordLength)
#endif
#endif

#ifndef AUDIO_IEC_CHANNEL_STATUS_BYSOURCE
#define AUDIO_IEC_CHANNEL_STATUS_BYSOURCE_DEFAULT
#define AUDIO_IEC_CHANNEL_STATUS_BYSOURCE             (&_au1MtkAudIecChannelStatusBySource)
#endif

#ifndef AUDIO_IECRAW_DELAY_FLAG
#define AUDIO_IECRAW_DELAY_FLAG           (TRUE) // default raw delay is synced with PCM channel delay
#endif

#ifndef AUDIO_IECRAW_DELAY
#define AUDIO_IECRAW_DELAY               (0) // in ms, default raw delay is 0 ms
#endif


// If GPIO NUM = 0xffffffff, then don't use that
#ifndef AUDIO_HP_PLUG_IN_GPIO
#define AUDIO_HP_PLUG_IN_GPIO           (0xffffffff)
#endif

#ifndef AUDIO_HP_PLUG_IN_POLARITY
#define AUDIO_HP_PLUG_IN_POLARITY       (AUD_GPIO_HIGH_ENALBE)
#endif

// If GPIO NUM = 0xffffffff, then don't use that
#ifndef AUDIO_DC_DETECT_GPIO
#define AUDIO_DC_DETECT_GPIO           (0xffffffff)
#endif

#ifndef AUDIO_DC_DETECT_POLARITY
#define AUDIO_DC_DETECT_POLARITY       AUD_GPIO_HIGH_ENALBE
#endif

#ifndef AUDIO_HP_PLUG_IN_LINEOUT_MUTE
#define AUDIO_HP_PLUG_IN_LINEOUT_MUTE   (FALSE)
#endif

#ifndef AUDIO_MUTE_ALL_SOURCE
#define AUDIO_MUTE_ALL_SOURCE           (FALSE)
#endif

#ifndef AUDIO_LINEOUT_DAC_USAGE
#define AUDIO_LINEOUT_DAC_USAGE         (FALSE)
#endif

// If GPIO NUM = 0xffffffff, then don't use that
#ifndef AUDIO_DVD_GPIO
#define AUDIO_DVD_GPIO (0xffffffff)
#endif

#ifndef AUDIO_DVD_POLARITY
#define AUDIO_DVD_POLARITY AUD_DVD_GPIO_HIGH_ENALBE
#endif

#ifndef AUDIO_DVD_CHANNEL
#define AUDIO_DVD_CHANNEL (0xffffffff)
#endif

#ifndef AUDIO_SPK_AUTO_OFF_WHEN_VOL_MUTE
#define AUDIO_SPK_AUTO_OFF_WHEN_VOL_MUTE (NULL)
#endif

// If GPIO NUM = 0xffffffff, then don't use that
#ifndef AUDIO_CUST_SPEC_GPIO_CTRL
#define AUDIO_CUST_SPEC_GPIO_CTRL (0xffffffff)
#endif

#ifndef AUDIO_CUST_SPEC_POLARITY
#define AUDIO_CUST_SPEC_POLARITY  0
#endif

#ifndef AUDIO_CUST_SPEC2_GPIO_CTRL
#define AUDIO_CUST_SPEC2_GPIO_CTRL (0xffffffff)
#endif

#ifndef AUDIO_CUST_SPEC2_POLARITY
#define AUDIO_CUST_SPEC2_POLARITY  0
#endif

#ifndef AUDIO_CUST_HP_GPIO_CTRL
#define AUDIO_CUST_HP_GPIO_CTRL (0xffffffff)
#endif

#ifndef AUDIO_CUST_HP_POLARITY
#define AUDIO_CUST_HP_POLARITY  0
#endif

#ifndef AUDIO_AMP_FADE_OUT_TIME
#define AUDIO_AMP_FADE_OUT_TIME 0
#endif

#ifndef AUDIO_Digital_Data_TABLE
#define AUDIO_Digital_Data_TABLE_DEFAULT
#define AUDIO_Digital_Data_TABLE     (&_afgMtkAudDigitalDataUsage)
#endif

#ifndef AUDIO_TUNER_ATD
#define AUDIO_TUNER_ATD     (TRUE)
#endif

// IF path =0, dac0 : -6 dB, path =1, dac1 : -6 dB, path =2, dac0 & dac1 : -6 dB
#ifndef AUDIO_SPK_CODEC_PATH
#define AUDIO_SPK_CODEC_PATH  1
#endif

#ifndef AUDIO_MT8292_PGA0_GAIN
#define AUDIO_MT8292_PGA0_GAIN (0x1d)
#endif

#ifndef AUDIO_MT8292_PGA1_GAIN
#define AUDIO_MT8292_PGA1_GAIN (0x1d)
#endif

#ifndef AUDIO_MT8292_PGA2_GAIN
#define AUDIO_MT8292_PGA2_GAIN (0x1d)
#endif

#ifndef AUDIO_MT8292_HP_GAIN
#define AUDIO_MT8292_HP_GAIN (0x1d)
#endif

#ifndef AUDIO_LINE_IN_LR_SWAP
#define AUDIO_LINE_IN_LR_SWAP (FALSE)
#endif

#ifndef AUDIO_TV_SYS_FINE_TUNE_VOL
#define AUDIO_TV_SYS_FINE_TUNE_VOL_DEFAULT
#define AUDIO_TV_SYS_FINE_TUNE_VOL        (&_au1MtkTVFineTuneVol)
#endif

#ifndef AUDIO_MTS_FACTORY_MODE
#define AUDIO_MTS_FACTORY_MODE_DEFAULT
#define AUDIO_MTS_FACTORY_MODE        (&_arMtsFactory)
#endif

#ifndef AUDIO_A2_FACTORY_MODE
#define AUDIO_A2_FACTORY_MODE_DEFAULT
#define AUDIO_A2_FACTORY_MODE        (&_arA2Factory)
#endif

#ifndef AUDIO_PAL_FACTORY_MODE
#define AUDIO_PAL_FACTORY_MODE_DEFAULT
#define AUDIO_PAL_FACTORY_MODE        (&_arPalFactory)
#endif

// Limiter mode. 0: Off, 1: Adaptive mode, 2: Fix mode
#ifndef AUDIO_LIMITER_MODE
#define AUDIO_LIMITER_MODE  (2)
#endif

// Limiter fix mode threshold, the max. value is 0x100000
#ifndef AUDIO_LIMITER_FIX_THRESHOLD
#define AUDIO_LIMITER_FIX_THRESHOLD     (0x100000)
#endif

#ifndef AUDIO_NVM_ENABLE
#define AUDIO_NVM_ENABLE (FALSE)
#endif

#ifndef AUDIO_NVM_LENGTH
#define AUDIO_NVM_LENGTH (0)
#endif

#ifndef AUDIO_NVM_DATA
#define AUDIO_NVM_DATA (0)
#endif

#ifndef AUDIO_NVM_GET_COUNTRY
#define AUDIO_NVM_GET_COUNTRY (FALSE)
#endif

#ifndef AUDIO_PROFILE_DEFAULT
#define AUDIO_PROFILE_DEFAULT (0)
#endif

#ifndef AUDIO_PROFILE_START
#define AUDIO_PROFILE_START (0)
#endif

#ifndef AUDIO_PROFILE_END
#define AUDIO_PROFILE_END (0)
#endif

#ifndef AUDIO_ADC_NEG_GAIN
#define AUDIO_ADC_NEG_GAIN                   (TRUE)     ///[Joel]20101026
#endif

#ifndef AUDIO_AL1AR1_PHASE_INVERSE
///Mustang M1V1 AL0/AR0 for HP direct driving:
///pwmdac gain(-) + internal OPA(-), pwmdac gain need set negative(-).
#define AUDIO_AL1AR1_PHASE_INVERSE           (TRUE)
#endif

#ifndef AUDIO_AL2AR2_PHASE_INVERSE
///Mustang M1V1 AL1/AR1 for Lineout:
///pwmdac gain(+) + internal OPA(-) + external OPA(-), pwmdac gain need set positive(+).
#define AUDIO_AL2AR2_PHASE_INVERSE           (FALSE)
#endif

#ifndef AUDIO_AL3AR3_PHASE_INVERSE
///Mustang M1V1 AL2/AR2 for Lineout:
///pwmdac gain(+) + internal OPA(-) + external OPA(-), pwmdac gain need set positive(+).
#define AUDIO_AL3AR3_PHASE_INVERSE           (FALSE)
#endif

#ifndef AUDIO_AL4AR4_PHASE_INVERSE
#define AUDIO_AL4AR4_PHASE_INVERSE           (FALSE)
#endif

#ifndef AUDIO_POWERSTAGE_USE_AL0AR0
#define AUDIO_POWERSTAGE_USE_AL0AR0          (FALSE)
#endif

#ifndef AUDIO_POWERSTAGE_USE_AL1AR1
#define AUDIO_POWERSTAGE_USE_AL1AR1          (FALSE)
#endif

#ifndef AUDIO_POWERSTAGE_USE_AL2AR2
#define AUDIO_POWERSTAGE_USE_AL2AR2          (FALSE)
#endif

#ifndef AUDIO_POWERSTAGE_USE_AL3AR3
#define AUDIO_POWERSTAGE_USE_AL3AR3          (FALSE)
#endif

#ifdef CC_AUDIO_EU_PWM_OUTPUT_AUXDEC
  #ifdef FLAG_DDR_QFP
    #if (FLAG_DDR_QFP == 1)    ///MT5365
    #ifndef AUDIO_AUX_OUT_AT_AL0AR0
    ///If TRUE, ARL0=Aux, ARL1=5204[2:0], ARL2=5204[5:3], ARL3=5204[8:6]
    #define AUDIO_AUX_OUT_AT_AL0AR0          (TRUE)     ///MT5365p2v1 AUX output at AL0/AR0
    #endif

    #ifndef AUDIO_AUX_OUT_AT_AL1AR1
    #define AUDIO_AUX_OUT_AT_AL1AR1          (FALSE)
    #endif

    #ifndef AUDIO_AUX_OUT_AT_AL2AR2
    #define AUDIO_AUX_OUT_AT_AL2AR2          (FALSE)
    #endif

    #ifndef AUDIO_AUX_OUT_AT_AL3AR3
    #define AUDIO_AUX_OUT_AT_AL3AR3          (FALSE)
    #endif

    #else               ///MT5366, (FLAG_DDR_QFP == 0)
    #ifndef AUDIO_AUX_OUT_AT_AL0AR0
    #define AUDIO_AUX_OUT_AT_AL0AR0          (FALSE)
    #endif

    #ifndef AUDIO_AUX_OUT_AT_AL1AR1
    #define AUDIO_AUX_OUT_AT_AL1AR1          (FALSE)
    #endif

    #ifndef AUDIO_AUX_OUT_AT_AL2AR2
    ///If TRUE, ARL0=5204[2:0], ARL1=5204[5:3], ARL2=Aux, ARL3=5204[8:6]
    #define AUDIO_AUX_OUT_AT_AL2AR2          (TRUE)     ///MT5366p2v1 AUX output at AL2/AR2
    #endif

    #ifndef AUDIO_AUX_OUT_AT_AL3AR3
    #define AUDIO_AUX_OUT_AT_AL3AR3          (FALSE)
    #endif

    #endif
  #else                 ///Non-define FLAG_DDR_QFP
    #ifndef AUDIO_AUX_OUT_AT_AL0AR0
    ///If TRUE, ARL0=Aux, ARL1=5204[2:0], ARL2=5204[5:3], ARL3=5204[8:6]
    #define AUDIO_AUX_OUT_AT_AL0AR0          (FALSE)
    #endif

    #ifndef AUDIO_AUX_OUT_AT_AL1AR1
    ///If TRUE, ARL0=5204[2:0], ARL1=Aux, ARL2=5204[5:3], ARL3=5204[8:6]
    #define AUDIO_AUX_OUT_AT_AL1AR1          (FALSE)
    #endif

    #ifndef AUDIO_AUX_OUT_AT_AL2AR2
    ///If TRUE, ARL0=5204[2:0], ARL1=5204[5:3], ARL2=Aux, ARL3=5204[8:6]
    #define AUDIO_AUX_OUT_AT_AL2AR2          (TRUE)
    #endif

    #ifndef AUDIO_AUX_OUT_AT_AL3AR3
    ///If TRUE, ARL0=5204[2:0], ARL1=5204[5:3], ARL2=5204[8:6], ARL3=Aux
    #define AUDIO_AUX_OUT_AT_AL3AR3          (FALSE)
    #endif
  #endif
#endif

#ifndef AUDIO_DAC_AL0AR0_ENABLE
#define AUDIO_DAC_AL0AR0_ENABLE           (TRUE)
#endif

#ifndef AUDIO_DAC_AL1AR1_ENABLE
#define AUDIO_DAC_AL1AR1_ENABLE           (TRUE)
#endif

#ifndef AUDIO_DAC_AL2AR2_ENABLE
#define AUDIO_DAC_AL2AR2_ENABLE           (TRUE)
#endif

#ifndef AUDIO_DAC_AL3AR3_ENABLE
#define AUDIO_DAC_AL3AR3_ENABLE           (TRUE)
#endif

#ifndef AUDIO_InternalLDO_ENABLE
#define AUDIO_InternalLDO_ENABLE           (TRUE)
#endif

#ifndef AUDIO_DIGITAL_AMP_RESET_GPIO
#define AUDIO_DIGITAL_AMP_RESET_GPIO (0xffffffff)
#endif

#ifndef AUDIO_DIGITAL_AMP_RESET_GPIO_POLARITY
#define AUDIO_DIGITAL_AMP_RESET_GPIO_POLARITY  AUD_GPIO_HIGH_ENALBE
#endif

//FLASH_AQ for 5387 customer model

#ifdef SUPPORT_FLASH_AQ
    #ifndef FLASH_AQ_BASE_PARTITION
        #ifdef LINUX_TURNKEY_SOLUTION
            #define FLASH_AQ_BASE_PARTITION 	(16)
        #else
            #ifndef CC_NAND_ENABLE
            #define FLASH_AQ_BASE_PARTITION     (8)
            #else
            #define FLASH_AQ_BASE_PARTITION     (9)
            #endif
        #endif
    #endif
#else
    #ifndef FLASH_AQ_BASE_PARTITION
        #define FLASH_AQ_BASE_PARTITION     (0xFF)
    #endif
#endif

#ifndef AUDIO_FLASH_AQ_DATA
#define AUDIO_FLASH_AQ_DATA         (0)
#endif

#ifndef AUDIO_FLASH_AQ_DATA_SIZE
#define AUDIO_FLASH_AQ_DATA_SIZE    (0)
#endif

#ifndef AUDIO_FLASH_AQ_DATA_DSCRPT
#define AUDIO_FLASH_AQ_DATA_DSCRPT  (0)
#endif

#ifndef AUDIO_FLASH_AQ_DATA_DSCRPT_SIZE
#define AUDIO_FLASH_AQ_DATA_DSCRPT_SIZE (0)
#endif
//End

#ifndef FLASH_ADSP_BIN_PARTITION
#define FLASH_ADSP_BIN_PARTITION     (16)
#endif

//TSHD //sunman_tshd
#ifndef AUDIO_TSHD_INPUTGAIN
#if defined (DSP_SUPPORT_SRS_PURESOUND)
#define AUDIO_TSHD_INPUTGAIN (0x33333333)//0.4
#else
#define AUDIO_TSHD_INPUTGAIN (0x40000000)//-6dB,32bit dsp
#endif
#endif

#ifndef AUDIO_TSHD_OUTPUTGAIN
#define AUDIO_TSHD_OUTPUTGAIN (0x7FFFFFFF)//0dB,32bit dsp
#endif

#ifndef AUDIO_TSHD_TBCTRL
#if defined (DSP_SUPPORT_SRS_PURESOUND)
#define AUDIO_TSHD_TBCTRL (0x33333333)//0.4
#else
#define AUDIO_TSHD_TBCTRL (0x40000000)  //50%,32bit dsp
#endif
#endif

#ifndef AUDIO_TSHD_SPKSIZE
#define AUDIO_TSHD_SPKSIZE (5) //150Hz
#endif

#ifndef AUDIO_TSHD_SURRLVL
#define AUDIO_TSHD_SURRLVL (0x004ccccd) //60%
#endif

#ifndef AUDIO_TSHD_DEFCTRL
#define AUDIO_TSHD_DEFCTRL (0x004ccccd)//60% [ian] fit WOWHD cmodel
#endif

#ifndef AUDIO_TSHD_ELEVCTRL
    #if defined (DSP_SUPPORT_SRSTSHD)  //0x00400000
      #define AUDIO_TSHD_ELEVCTRL (0x00400000)
    #else
      #define AUDIO_TSHD_ELEVCTRL (0x00333333)
    #endif
#endif

#ifndef AUDIO_TSHD_ANASIZE
#define AUDIO_TSHD_ANASIZE (5) //150Hz
#endif
#ifndef AUDIO_TSHD_COMPRCTRL
#if defined (DSP_SUPPORT_SRS_PURESOUND)
#define AUDIO_TSHD_COMPRCTRL (0x11eb851f)//0.14
#else
#define AUDIO_TSHD_COMPRCTRL (0x20000000) //0.25,32bit dsp
#endif
#endif

#ifndef AUDIO_TSHD_LEVELDSP
#if defined (DSP_SUPPORT_SRSTB_LEVELINDEP)
#define AUDIO_TSHD_LEVELDSP (1) //ENABLE
#else
#define AUDIO_TSHD_LEVELDSP (0) //ENABLE
#endif
#endif


 //bofeng_cc3d
#ifndef AUDIO_CC3D_FLAG
#define AUDIO_CC3D_FLAG (0x00007bfb)//
#endif

#ifndef AUDIO_CC3D_MODE
//#define AUDIO_CC3D_MODE (0x00000000)//SRS_CC3D_MODE_3D
#define AUDIO_CC3D_MODE (0x00000001)//SRS_CC3D_MODE_MUSIC
#endif

#ifndef AUDIO_CC3D_INPUTGAIN
#define AUDIO_CC3D_INPUTGAIN (0x40000000)//-6dB,32bit dsp
#endif

#ifndef AUDIO_CC3D_OUTPUTGAIN
#define AUDIO_CC3D_OUTPUTGAIN (0x7FFFFFFF)//0dB,32bit dsp
#endif

#ifndef AUDIO_CC3D_TBCTRL
#define AUDIO_CC3D_TBCTRL (0x40000000)  //50%,32bit dsp
#endif

#ifndef AUDIO_CC3D_SPKSIZE
#define AUDIO_CC3D_SPKSIZE (5) //150Hz
#endif

#ifndef AUDIO_CC3D_ANASIZE
#define AUDIO_CC3D_ANASIZE (5) //150Hz
#endif
#ifndef AUDIO_CC3D_COMPRCTRL
#define AUDIO_CC3D_COMPRCTRL (0x20000000) //0.25,32bit dsp
#endif

#ifndef AUDIO_CC3D_LEVELDSP
#if defined (DSP_SUPPORT_SRSTB_LEVELINDEP)
#define AUDIO_CC3D_LEVELDSP (1) //ENABLE
#else
#define AUDIO_CC3D_LEVELDSP (0) //ENABLE
#endif
#endif

#ifndef AUDIO_CC3D_HPFSIZE
#define AUDIO_CC3D_HPFSIZE (0) //80HZ
#endif

#ifndef AUDIO_CC3D_INISIZE
#define AUDIO_CC3D_INISIZE (2) //InitNHFilter
#endif



//VIQ //sunman_viq
#ifndef AUDIO_VIQ_INPUTGAIN
#define AUDIO_VIQ_INPUTGAIN (0x00200000)//default 1 (0.25) scale 2
#endif

#ifndef AUDIO_VIQ_OUTPUTGAIN
#define AUDIO_VIQ_OUTPUTGAIN (0x00200000)//default (0.25)
#endif

#ifndef AUDIO_VIQ_MAXGAIN
#define AUDIO_VIQ_MAXGAIN (0x00080000)  //default 16 scale 8
#endif

#ifndef AUDIO_VIQ_REFLVL
#define AUDIO_VIQ_REFLVL (0x400000) //initial as minus for master vol
#endif

#ifndef AUDIO_VIQ_MODE
#define AUDIO_VIQ_MODE (0x01100) //default noise manager on // Normal Mode//sunman_viq2
#endif

#ifndef AUDIO_VIQ_NOISETH
//#define AUDIO_VIQ_NOISETH (0xCCCCD)
#define AUDIO_VIQ_NOISETH (0x133333)            // low th sunman_viq2 update 2.1.0.0
#endif


//sunman_peq
#ifndef AUDIO_SRS_PEQ_COEF
#define AUDIO_SRS_PEQ_COEF_DEFAULT
#define AUDIO_SRS_PEQ_COEF        (&_arSRSPEQHPFCoef_DEF)
#endif

#ifndef AUDIO_SRSPEQ_INPUTGAIN
#define AUDIO_SRSPEQ_INPUTGAIN (0x80000)
#endif

#ifndef AUDIO_SRSPEQ_OUTPUTGAIN
#define AUDIO_SRSPEQ_OUTPUTGAIN (0x80000)
#endif


#ifndef AUDIO_SRSPEQ_BANDS
#define AUDIO_SRSPEQ_BANDS (7)
#endif

#ifndef AUDIO_MTK_HPF_FC
#define AUDIO_MTK_HPF_FC (100)
#endif
#ifndef AUDIO_MTK_HPF2_FC
#define AUDIO_MTK_HPF2_FC (120)
#endif


//sunman_lmt
#ifndef AUDIO_SRSLMT_INPUTGAIN
#define AUDIO_SRSLMT_INPUTGAIN (0x200000)
#endif

#ifndef AUDIO_SRSLMT_OUTPUTGAIN
#define AUDIO_SRSLMT_OUTPUTGAIN (0x200000)
#endif


#ifndef AUDIO_SRSLMT_BOOST
//#define AUDIO_SRSLMT_BOOST (0x168f6)
//#define AUDIO_SRSLMT_BOOST (0x17333)
#if defined (DSP_SUPPORT_SRS_PURESOUND)
#define AUDIO_SRSLMT_BOOST (0x0a0000)
#else
#define AUDIO_SRSLMT_BOOST (0x040000)
#endif

#endif

#ifndef AUDIO_SRSLMT_LEVEL
//#define AUDIO_SRSLMT_LEVEL (0x7fffff)
//#define AUDIO_SRSLMT_LEVEL (0x2eb851)
#define AUDIO_SRSLMT_LEVEL (0x7FFFFF)
#endif

#ifndef AUDIO_SRSLMT_DELAY
//#define AUDIO_SRSLMT_DELAY (22)
//#define AUDIO_SRSLMT_DELAY (30)
#if defined (DSP_SUPPORT_SRS_PURESOUND)
#define AUDIO_SRSLMT_DELAY (14)
#else
#define AUDIO_SRSLMT_DELAY (30)
#endif

#endif

#ifndef AUDIO_SRSLMT_COEFF
#define AUDIO_SRSLMT_COEFF (0xd1b)
#endif

#ifndef AUDIO_SRSLMT_THRESH
#define AUDIO_SRSLMT_THRESH (0)
#endif

//bofeng_aeq
#ifndef AUDIO_SRSAEQ_INPUTGAIN
#define AUDIO_SRSAEQ_INPUTGAIN (0x7fffffff)
#endif

#ifndef AUDIO_SRSAEQ_OUTPUTGAIN
#define AUDIO_SRSAEQ_OUTPUTGAIN (0x7fffffff)
#endif

#ifndef AUDIO_SRSAEQ_FILTERSIZE
#define AUDIO_SRSAEQ_FILTERSIZE (0x00000000)
#endif


#ifndef AUDIO_MTK_VS_CLARITY
#define AUDIO_MTK_VS_CLARITY (0xa0000)
#endif

#ifndef AUDIO_MTK_VS_WIDTH
#define AUDIO_MTK_VS_WIDTH (0xa0000)
#endif

#ifndef AUDIO_MTK_VS_LRGAIN
#define AUDIO_MTK_VS_LRGAIN (0x200000)
#endif

#ifndef AUDIO_MTK_VS_XTALK
#define AUDIO_MTK_VS_XTALK (0xa0000)
#endif

#ifndef AUDIO_MTK_VS_OUTPUTGAIN
#define AUDIO_MTK_VS_OUTPUTGAIN (0xc0000)
#endif

#ifndef AUDIO_MTK_VS_BASSGAIN
#define AUDIO_MTK_VS_BASSGAIN (0x150000)
#endif

#ifndef AUDIO_MTK_VS_FO
#define AUDIO_MTK_VS_FO (0x07f4300)
#endif

#ifdef CC_AUD_DSP_SUPPORT_AUDYSSEY
#ifndef AUDIO_ADV_CH_CALB_GAIN_L
#define AUDIO_ADV_CH_CALB_GAIN_L        (0xb707)
#endif

#ifndef AUDIO_ADV_CH_CALB_GAIN_R
#define AUDIO_ADV_CH_CALB_GAIN_R        (0xb707)
#endif

#ifndef AUDIO_ABX_FILTER_SET
#define AUDIO_ABX_FILTER_SET            (0)
#endif

#ifndef AUDIO_ABX_DRY_GAIN_INDX
#define AUDIO_ABX_DRY_GAIN_INDX         (0)
#endif

#ifndef AUDIO_ABX_WET_GAIN_INDX
#define AUDIO_ABX_WET_GAIN_INDX         (6)
#endif

#ifndef AUDIO_AEQ_5BAND_TABLE
#define AUDIO_AEQ_5BAND_TABLE_DEFAULT
#define AUDIO_AEQ_5BAND_TABLE           (&_aau4MtkAeq5BandTable)
#endif
#endif

#ifndef AUDIO_MIX_SOUND_GAIN
#define AUDIO_MIX_SOUND_GAIN  (0x800) //0x20000->0
#endif

#ifndef AUDIO_MIX_SOUND_LENGTH
#define AUDIO_MIX_SOUND_LENGTH  (64)
#endif

#ifdef CC_AUD_SILENCE_SUPPORT
#ifndef AUDIO_SILENCE_MODE
#define AUDIO_SILENCE_MODE   (0)
#endif

#ifndef AUDIO_SILENCE_FIX_THRESHOLD
#define AUDIO_SILENCE_FIX_THRESHOLD    (0x1000)
#endif

#ifndef AUDIO_SILENCE_ATTACK_WAIT
#define AUDIO_SILENCE_ATTACK_WAIT    (2000)
#endif

#ifndef AUDIO_SILENCE_ATTACK_FIX_GAIN
#define AUDIO_SILENCE_ATTACK_FIX_GAIN    (0x200000)
#endif

#ifndef AUDIO_SILENCE_RELEASE_STEP
#define AUDIO_SILENCE_RELEASE_STEP    (350)
#endif

#ifndef AUDIO_SILENCE_ATTACK_STEP
#define AUDIO_SILENCE_ATTACK_STEP    (700)
#endif
#endif

#ifndef AUDIO_VDO_DEALY_CUST_TBL_DTV
#define AUDIO_VDO_DEALY_CUST_TBL_DTV (0) //CC_AUD_QUERY_VDO_INFO
#endif
#ifndef AUDIO_VDO_DEALY_CUST_TBL_HDMI
#define AUDIO_VDO_DEALY_CUST_TBL_HDMI (0)
#endif

#ifndef AUDIO_EXTRA_MUTE
#define AUDIO_EXTRA_MUTE                (0)
#endif

#ifndef AUDIO_PEQ_ENABLE
#define AUDIO_PEQ_ENABLE                (0)
#endif



//====================================


//----------------------------------------------------------------------------
// Other GPIO customization
//----------------------------------------------------------------------------
#ifndef FLAG_EXT_STATUS
#define FLAG_EXT_STATUS             (0)
#endif
// #ifndef STANDBY_SET_GPIO
// #define STANDBY_SET_GPIO            (OPCTRL(7))
// #endif
// #ifndef POWER_STATUS_GPIO
// #define POWER_STATUS_GPIO           (OPCTRL(8))
// #endif
#ifndef STDBY_USE_PWR_BTN_CNT
#define STDBY_USE_PWR_BTN_CNT       (0)
#endif
#ifndef STDBY_USE_PREV_PWR_STATE
#define STDBY_USE_PREV_PWR_STATE    (0)
#endif
#ifndef STDBY_USE_VGA_STATUS
#define STDBY_USE_VGA_STATUS        (0)
#endif
#ifndef GPIO_TIMER_INIT_FUNC
#define GPIO_TIMER_INIT_FUNC        (NULL)
#endif

// #ifndef POWER_LED_GPIO
// #define POWER_LED_GPIO              (OPCTRL(0))
// #endif
// #ifndef POWER_LED_ON_VAL
// #define POWER_LED_ON_VAL            (0)
// #endif
// #define BACKUP_LED_GPIO
// #define BACKUP_LED_ON_VAL
// #define POWER_CTRL_GPIO
// #define POWER_CTRL_ON_VAL
#ifndef POWER_CTRL_DELAY_MS
#define POWER_CTRL_DELAY_MS         (5)
#endif
// #define DVD_CTRL_GPIO
// #define DVD_CTRL_ON_VAL
#ifndef FLAG_SUPPORT_DARK_LED
#define FLAG_SUPPORT_DARK_LED        (0)
#endif
// #define POWER_LED_DARK_VAL
// #define BACKUP_LED_DARK_VAL
// #define SPECIAL_WAKEUP_GPIO
// #define SPECIAL_WAKEUP_ON_VAL
// #define COMPANION_CHIP_GPIO_NUM
// #define COMPANION_CHIP_GPIO_POLARITY

#ifndef DEFAULT_ETHERNET_WAKEUP_SOPPORT
#define DEFAULT_ETHERNET_WAKEUP_SOPPORT      (0)
#endif


#ifndef FLAG_SUPPORT_POWER_LED_OFF
#define FLAG_SUPPORT_POWER_LED_OFF      (0)
#endif

//----------------------------------------------------------------------------
// EEPROM customization
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// SYSTEM EEPROM customization
//----------------------------------------------------------------------------
#ifndef SYSTEM_EEPROM_BUS
#define SYSTEM_EEPROM_BUS                 (0)
#endif
#ifndef SYSTEM_EEPROM_ADDRESS
#define SYSTEM_EEPROM_ADDRESS             (0xA0)
#endif
#ifndef SYSTEM_EEPROM_SIZE
#define SYSTEM_EEPROM_SIZE                (2048)
#endif
#ifndef SYSTEM_EEPROM_CLKDIV
#define SYSTEM_EEPROM_CLKDIV              (0x100)
#endif
#ifndef SYSTEM_EEPROM_PAGESIZE
#define SYSTEM_EEPROM_PAGESIZE              (8)
#endif
// #ifndef SYSTEM_EEPROM_WP_GPIO
// #define SYSTEM_EEPROM_WP_GPIO             (OPCTRL(5))
// #endif
// #ifndef SYSTEM_EEPROM_WP_ENABLE_POLARITY
// #define SYSTEM_EEPROM_WP_ENABLE_POLARITY  (0)
// #endif
#ifndef SYSTEM_EEPROM_WAIT_TIME
#define SYSTEM_EEPROM_WAIT_TIME              (20)
#endif

#ifndef SYSTEM_EEPROM_ENABLE_RETRY
#define SYSTEM_EEPROM_ENABLE_RETRY            (10)
#endif

//----------------------------------------------------------------------------
// EDID EEPROM customization
//----------------------------------------------------------------------------
// #ifndef EDID_EEPROM_WP_GPIO
// #define EDID_EEPROM_WP_GPIO               (OPCTRL(3))
// #endif
// #ifndef EDID_EEPROM_WP_ENABLE_POLARITY
// #define EDID_EEPROM_WP_ENABLE_POLARITY    (0)
// #endif

//----------------------------------------------------------------------------
// HDCP EEPROM customization
//----------------------------------------------------------------------------
#ifndef HDCP_EEPROM_BUS
#define HDCP_EEPROM_BUS                   (0)
#endif
#ifndef HDCP_EEPROM_ADDRESS
#define HDCP_EEPROM_ADDRESS               (0xA0)
#endif
#ifndef HDCP_EEPROM_SIZE
#define HDCP_EEPROM_SIZE                  (2048)
#endif
#ifndef HDCP_EEPROM_CLKDIV
#define HDCP_EEPROM_CLKDIV                (0x100)
#endif
#ifndef HDCP_EEPROM_OFFSET
#define HDCP_EEPROM_OFFSET                  (0)
#endif

// #ifndef HDCP_EEPROM_WP_GPIO
// #define HDCP_EEPROM_WP_GPIO               (OPCTRL(5))
// #endif
// #ifndef HDCP_EEPROM_WP_ENABLE_POLARITY
// #define HDCP_EEPROM_WP_ENABLE_POLARITY    (0)
// #endif

//Initially for SGP, now open for all customers
#ifndef DRVCUST_EEPROM_HDMI_HDCP_OFFSET
#define DRVCUST_EEPROM_HDMI_HDCP_OFFSET  ((UINT32)0)
#endif
#ifndef DRVCUST_EEPROM_HDMI_HDCP_SIZE
#define DRVCUST_EEPROM_HDMI_HDCP_SIZE  ((UINT32)320)
#endif
#ifndef DRVCUST_EEPROM_ERROR_CODE_OFFSET
#define DRVCUST_EEPROM_ERROR_CODE_OFFSET    ((UINT32)0)
#endif
#ifndef DRVCUST_EEPROM_ERROR_CODE_SIZE
#define DRVCUST_EEPROM_ERROR_CODE_SIZE        ((UINT32)0)
#endif
#ifndef DRVCUST_EEPROM_STATE_CODE_OFFSET
#define DRVCUST_EEPROM_STATE_CODE_OFFSET    ((UINT32)0)
#endif
#ifndef DRVCUST_EEPROM_STATE_CODE_SIZE
#define DRVCUST_EEPROM_STATE_CODE_SIZE      ((UINT32)0)
#endif
#ifndef DRVCUST_EEPROM_DTV_DRIVER_OFFSET
#define DRVCUST_EEPROM_DTV_DRIVER_OFFSET    ((UINT32)(DRVCUST_EEPROM_HDMI_HDCP_OFFSET + DRVCUST_EEPROM_HDMI_HDCP_SIZE))
#endif
#ifndef DRVCUST_EEPROM_DTV_DRIVER_SIZE
#define DRVCUST_EEPROM_DTV_DRIVER_SIZE    ((UINT32)96)
#endif
#ifndef DRVCUST_EEPROM_NPTV_DRIVER_OFFSET
#define DRVCUST_EEPROM_NPTV_DRIVER_OFFSET    ((UINT32)(DRVCUST_EEPROM_DTV_DRIVER_OFFSET + DRVCUST_EEPROM_DTV_DRIVER_SIZE))
#endif
#ifndef DRVCUST_EEPROM_NPTV_DRIVER_SIZE
#define DRVCUST_EEPROM_NPTV_DRIVER_SIZE    ((UINT32)448)
#endif

#ifndef DRVCUST_EEPROM_CUSTOM_DRIVER_OFFSET
#define DRVCUST_EEPROM_CUSTOM_DRIVER_OFFSET ((UINT32)(DRVCUST_EEPROM_NPTV_DRIVER_OFFSET + DRVCUST_EEPROM_NPTV_DRIVER_SIZE))
#endif

#ifndef DRVCUST_EEPROM_CUSTOM_DRIVER_SIZE
#define DRVCUST_EEPROM_CUSTOM_DRIVER_SIZE ((UINT32)64)
#endif

#ifndef DRVCUST_VGA_EDID_SIZE
#define DRVCUST_VGA_EDID_SIZE           ((UINT32)256)
#endif

#ifndef DRVCUST_HDMI_EDID_SIZE
#define DRVCUST_HDMI_EDID_SIZE	    ((UINT32)256)
#endif


#ifndef DRVCUST_EEPROM_VGA_EDID_OFFSET
#ifdef CC_EDID_DATA_LOCATED_IN_SYS_EEPROM_DRIVER_BLOCK
/* CC_EDID_DATA_LOCATED_IN_SYS_EEPROM_DRIVER_BLOCK, (2010-10-12)
   It should be defined in new project.
   this compile option for back-ward compatible with old AP/MW/factory code, they might
   access eeprom with fixed address.  */
#define DRVCUST_EEPROM_VGA_EDID_OFFSET	((UINT32)DRVCUST_EEPROM_CUSTOM_DRIVER_OFFSET+DRVCUST_EEPROM_CUSTOM_DRIVER_SIZE)
#else
//#define DRVCUST_EEPROM_VGA_EDID_OFFSET	((UINT32)0xde0)
#define DRVCUST_EEPROM_VGA_EDID_OFFSET	((UINT32)SYSTEM_EEPROM_SIZE -DRVCUST_VGA_EDID_SIZE- DRVCUST_HDMI_EDID_SIZE - 32)
#endif
#endif


#ifndef DRVCUST_VGA_EDID_STORAGE_SIZE
#define DRVCUST_VGA_EDID_STORAGE_SIZE	    ((UINT32)DRVCUST_VGA_EDID_SIZE)
#endif

#ifndef DRVCUST_EEPROM_HDMI_EDID_OFFSET
#define DRVCUST_EEPROM_HDMI_EDID_OFFSET	((UINT32)(DRVCUST_EEPROM_VGA_EDID_OFFSET + DRVCUST_VGA_EDID_STORAGE_SIZE))
#endif


#ifndef DRVCUST_HDMI_EDID_STORAGE_SIZE
#define DRVCUST_HDMI_EDID_STORAGE_SIZE	    ((UINT32)DRVCUST_HDMI_EDID_SIZE+16) /* 16 for PhyAddr, actually need 4 only */
#endif

#ifndef DRVCUST_EEPROM_DRIVER_SIZE
#ifdef CC_EDID_DATA_LOCATED_IN_SYS_EEPROM_DRIVER_BLOCK
#define DRVCUST_EEPROM_DRIVER_SIZE          ((UINT32)(DRVCUST_EEPROM_HDMI_EDID_OFFSET + DRVCUST_HDMI_EDID_STORAGE_SIZE))
#else
#define DRVCUST_EEPROM_DRIVER_SIZE            ((UINT32)(DRVCUST_EEPROM_CUSTOM_DRIVER_OFFSET + DRVCUST_EEPROM_CUSTOM_DRIVER_SIZE))
#endif
#endif

#ifndef DRVCUST_EEPROM_MW_OFFSET
#ifdef CC_EDID_DATA_LOCATED_IN_SYS_EEPROM_DRIVER_BLOCK
#define DRVCUST_EEPROM_MW_OFFSET          ((UINT32)(DRVCUST_EEPROM_HDMI_EDID_OFFSET + DRVCUST_HDMI_EDID_STORAGE_SIZE))
#else
#define DRVCUST_EEPROM_MW_OFFSET    ((UINT32)(DRVCUST_EEPROM_CUSTOM_DRIVER_OFFSET + DRVCUST_EEPROM_CUSTOM_DRIVER_SIZE))
#endif
#endif

#ifndef DRVCUST_EEPROM_MW_SIZE
#define DRVCUST_EEPROM_MW_SIZE        ((UINT32)192)
#endif

#ifndef DRVCUST_EEPROM_MW_DRIVER_SIZE
#define DRVCUST_EEPROM_MW_DRIVER_SIZE    ((UINT32)192)
#endif

#ifndef	DRVCUST_EEPROM_HDCP2X_OFFSET
#define DRVCUST_EEPROM_HDCP2X_OFFSET	((UINT32)(DRVCUST_EEPROM_VGA_EDID_OFFSET-880))
//#define	DRVCUST_EEPROM_HDCP2X_OFFSET	((UINT32)2672)
#endif

#ifndef DRVCUST_EEPROM_HDCP2X_SIZE
#define	DRVCUST_EEPROM_HDCP2X_SIZE		((UINT32)880)
#endif

#ifndef DRVCUST_EEPROM_DATA_MEM_OFFSET
#define DRVCUST_EEPROM_DATA_MEM_OFFSET        ((UINT32)0x1F00000)   //35MB
#endif

// read panel from eeprom
#ifndef PANEL_INDEX_FROM_EEPROM
#define PANEL_INDEX_FROM_EEPROM     (1)
#endif

#ifndef FRAME_TRACK_MODE
#define FRAME_TRACK_MODE  (2)
#endif
#ifndef FRAME_TRACK_STABLE_COUNT
#define FRAME_TRACK_STABLE_COUNT  (3)
#endif

//----------------------------------------------------------------------------
// NOR customization
//----------------------------------------------------------------------------
#ifndef NOR_FLASH_TOTAL_SIZE
    #ifdef CC_DUAL_IMAGE_FLASH
        #ifdef NDEBUG
            #define NOR_FLASH_TOTAL_SIZE                (8*1024*1024)
        #else /* NDEBUG */
            #define NOR_FLASH_TOTAL_SIZE                (16*1024*1024)
        #endif /* NDEBUG */
    #else /* CC_DUAL_IMAGE_FLASH */
            #define NOR_FLASH_TOTAL_SIZE                (8*1024*1024)
    #endif /* CC_DUAL_IMAGE_FLASH */
#endif

#ifndef NOR_FLASH_LOADER_SIZE
    #ifdef LOADER_MAX_SIZE
        #define NOR_FLASH_LOADER_SIZE             (LOADER_MAX_SIZE)
    #else /* LOADER_MAX_SIZE */
        #define NOR_FLASH_LOADER_SIZE             (128*1024)
    #endif /* LOADER_MAX_SIZE */
#endif

    #ifndef NOR_FLASH_PQ_SIZE
        #define NOR_FLASH_PQ_SIZE               (128*1024)
    #endif

    #ifndef NOR_FLASH_NTFS_SIZE
        #ifdef FM_SUPPORT_DYNAMIC_NTFS
        #define NOR_FLASH_NTFS_SIZE                     (128 * 1024)
        #else
        #define NOR_FLASH_NTFS_SIZE                     (0)
        #endif
    #endif

    #ifndef NOR_FLASH_CAPTURE_SIZE
        #ifdef CAPTURE_LOGO_SUPPORT
        #define NOR_FLASH_CAPTURE_SIZE               (1*1024*1024)
        #else
        #define NOR_FLASH_CAPTURE_SIZE               (0)
        #endif
    #endif

    #ifndef NOR_FLASH_AQ_SIZE
        #ifdef SUPPORT_FLASH_AQ
        #define NOR_FLASH_AQ_SIZE                   (64*1024)
        #else
        #define NOR_FLASH_AQ_SIZE                   (0)
        #endif
    #endif

    #ifndef NOR_FLASH_CIPLUS_SIZE
        #ifdef CI_PLUS_SUPPORT
            #define NOR_FLASH_CIPLUS_SIZE              (128*1024)
        #else
            #define NOR_FLASH_CIPLUS_SIZE              (0)
        #endif
    #endif

#ifndef NOR_FLASH_APPIMAGE_SIZE
    #ifdef CC_DUAL_IMAGE_FLASH
        #ifdef DUAL_CHANNEL_SUPPORT
            #define NOR_FLASH_APPIMAGE_SIZE             (((NOR_FLASH_TOTAL_SIZE >> 1) - NOR_FLASH_LOADER_SIZE) - ((2*2)*(64*1024)) - NOR_FLASH_PQ_SIZE - NOR_FLASH_NTFS_SIZE - NOR_FLASH_CAPTURE_SIZE - NOR_FLASH_AQ_SIZE - NOR_FLASH_CIPLUS_SIZE)
        #else /* DUAL_CHANNEL_SUPPORT */
            #define NOR_FLASH_APPIMAGE_SIZE             (((NOR_FLASH_TOTAL_SIZE >> 1) - NOR_FLASH_LOADER_SIZE) - ((1*2)*(64*1024)) - NOR_FLASH_PQ_SIZE - NOR_FLASH_NTFS_SIZE - NOR_FLASH_CAPTURE_SIZE - NOR_FLASH_AQ_SIZE - NOR_FLASH_CIPLUS_SIZE)
        #endif /* DUAL_CHANNEL_SUPPORT */
    #else /* CC_DUAL_IMAGE_FLASH */
        #ifdef DUAL_CHANNEL_SUPPORT
            #define NOR_FLASH_APPIMAGE_SIZE             ((NOR_FLASH_TOTAL_SIZE - NOR_FLASH_LOADER_SIZE) - ((2*2)*(64*1024)) - NOR_FLASH_PQ_SIZE - NOR_FLASH_NTFS_SIZE - NOR_FLASH_CAPTURE_SIZE - NOR_FLASH_AQ_SIZE - NOR_FLASH_CIPLUS_SIZE)
        #else /* DUAL_CHANNEL_SUPPORT */
            #define NOR_FLASH_APPIMAGE_SIZE             ((NOR_FLASH_TOTAL_SIZE - NOR_FLASH_LOADER_SIZE) - ((1*2)*(64*1024)) - NOR_FLASH_PQ_SIZE - NOR_FLASH_NTFS_SIZE - NOR_FLASH_CAPTURE_SIZE - NOR_FLASH_AQ_SIZE - NOR_FLASH_CIPLUS_SIZE)
        #endif /* DUAL_CHANNEL_SUPPORT */
    #endif /* CC_DUAL_IMAGE_FLASH */
#endif

// if NOR_FLASH_DYNAMIC_PART_SIZE = 0, all partitions are all static partition type.
#ifndef NOR_FLASH_DYNAMIC_PART_SIZE
    #define NOR_FLASH_DYNAMIC_PART_SIZE         (0)
#endif

#ifndef NOR_FLASH_STATIC_PART_SIZE
    #define NOR_FLASH_STATIC_PART_SIZE \
        ((NOR_FLASH_TOTAL_SIZE) - (NOR_FLASH_LOADER_SIZE) - \
        (NOR_FLASH_APPIMAGE_SIZE) - (NOR_FLASH_DYNAMIC_PART_SIZE))
#endif

#ifndef HDCP_NOR_FLASH_PART_ID
    #define HDCP_NOR_FLASH_PART_ID              (0)
#endif

#ifndef VGAEDID_NOR_FLASH_PART_ID
    #define VGAEDID_NOR_FLASH_PART_ID           (0)
#endif

#ifndef VGAEDID_NOR_FLASH_OFFSET
    #define VGAEDID_NOR_FLASH_OFFSET            (0)
#endif

#ifndef HDMIEDID_NOR_FLASH_PART_ID
    #define HDMIEDID_NOR_FLASH_PART_ID          (0)
#endif

#ifndef HDMIEDID_NOR_FLASH_OFFSET
    #define HDMIEDID_NOR_FLASH_OFFSET           (0)
#endif

// size > 64KB: static partition type.
// size = 64KB: dynamic partition type.
// size = 0:       unused partition type.
    #ifndef NOR_PART_SIZE_0
        #define NOR_PART_SIZE_0            (NOR_FLASH_LOADER_SIZE)              // loader
    #endif
    #ifndef NOR_PART_SIZE_1
        #define NOR_PART_SIZE_1            (NOR_FLASH_APPIMAGE_SIZE)          // appimg
    #endif
    #ifndef NOR_PART_SIZE_2
        #define NOR_PART_SIZE_2            (2*64*1024)  // channel list
    #endif
    #ifndef NOR_PART_SIZE_3
        #ifdef CC_DUAL_IMAGE_FLASH
            #define NOR_PART_SIZE_3            (NOR_FLASH_APPIMAGE_SIZE)          // appimg x 2
        #else
            #define NOR_PART_SIZE_3            (0)          // appimg x 2
        #endif
    #endif
    #ifndef NOR_PART_SIZE_4
        #ifdef DUAL_CHANNEL_SUPPORT
            #define NOR_PART_SIZE_4            (2*64*1024)  // channel list
        #else
            #define NOR_PART_SIZE_4            (0)
        #endif
    #endif
    #ifndef NOR_PART_SIZE_5
        #define NOR_PART_SIZE_5            (0)
    #endif
    #ifndef NOR_PART_SIZE_6
        #define NOR_PART_SIZE_6            (NOR_FLASH_NTFS_SIZE)
    #endif
    #ifndef NOR_PART_SIZE_7
        #define NOR_PART_SIZE_7            (NOR_FLASH_PQ_SIZE)
    #endif
    #ifndef NOR_PART_SIZE_8
        #define NOR_PART_SIZE_8            (NOR_FLASH_AQ_SIZE)
    #endif
    #ifndef NOR_PART_SIZE_9
        #define NOR_PART_SIZE_9            (NOR_FLASH_CAPTURE_SIZE)
    #endif
    #ifndef NOR_PART_SIZE_10
        #define NOR_PART_SIZE_10            (NOR_FLASH_CIPLUS_SIZE)
    #endif
    #ifndef NOR_PART_SIZE_11
        #define NOR_PART_SIZE_11            (0)
    #endif
    #ifndef NOR_PART_SIZE_12
        #define NOR_PART_SIZE_12            (0)
    #endif
    #ifndef NOR_PART_SIZE_13
        #define NOR_PART_SIZE_13            (0)
    #endif
    #ifndef NOR_PART_SIZE_14
        #define NOR_PART_SIZE_14            (0)
    #endif
    #ifndef NOR_PART_SIZE_15
        #define NOR_PART_SIZE_15            (0)
    #endif
    #ifndef NOR_PART_SIZE_16
        #define NOR_PART_SIZE_16            (0)
    #endif
    #ifndef NOR_PART_SIZE_17
        #define NOR_PART_SIZE_17            (0)
    #endif
    #ifndef NOR_PART_SIZE_18
        #define NOR_PART_SIZE_18            (0)
    #endif
    #ifndef NOR_PART_SIZE_19
        #define NOR_PART_SIZE_19            (0)
    #endif
    #ifndef NOR_PART_SIZE_20
        #define NOR_PART_SIZE_20            (0)
    #endif
    #ifndef NOR_PART_SIZE_21
        #define NOR_PART_SIZE_21            (0)
    #endif

    #define NOR_PART_OFFSET_1  NOR_FLASH_LOADER_SIZE
    #define NOR_PART_OFFSET_2  (NOR_PART_OFFSET_1+NOR_PART_SIZE_1)
    #define NOR_PART_OFFSET_3  (NOR_PART_OFFSET_2+NOR_PART_SIZE_2)
    #define NOR_PART_OFFSET_4  (NOR_PART_OFFSET_3+NOR_PART_SIZE_3)
    #define NOR_PART_OFFSET_5  (NOR_PART_OFFSET_4+NOR_PART_SIZE_4)
    #define NOR_PART_OFFSET_6  (NOR_PART_OFFSET_5+NOR_PART_SIZE_5)
    #define NOR_PART_OFFSET_7  (NOR_PART_OFFSET_6+NOR_PART_SIZE_6)
    #define NOR_PART_OFFSET_8  (NOR_PART_OFFSET_7+NOR_PART_SIZE_7)
    #define NOR_PART_OFFSET_9  (NOR_PART_OFFSET_8+NOR_PART_SIZE_8)
    #define NOR_PART_OFFSET_10 (NOR_PART_OFFSET_9+NOR_PART_SIZE_9)
    #define NOR_PART_OFFSET_11 (NOR_PART_OFFSET_10+NOR_PART_SIZE_10)
    #define NOR_PART_OFFSET_12 (NOR_PART_OFFSET_11+NOR_PART_SIZE_11)
    #define NOR_PART_OFFSET_13 (NOR_PART_OFFSET_12+NOR_PART_SIZE_12)
    #define NOR_PART_OFFSET_14 (NOR_PART_OFFSET_13+NOR_PART_SIZE_13)
    #define NOR_PART_OFFSET_15 (NOR_PART_OFFSET_14+NOR_PART_SIZE_14)
    #define NOR_PART_OFFSET_16 (NOR_PART_OFFSET_15+NOR_PART_SIZE_15)
    #define NOR_PART_OFFSET_17 (NOR_PART_OFFSET_16+NOR_PART_SIZE_16)
    #define NOR_PART_OFFSET_18 (NOR_PART_OFFSET_16+NOR_PART_SIZE_17)
    #define NOR_PART_OFFSET_19 (NOR_PART_OFFSET_16+NOR_PART_SIZE_18)
    #define NOR_PART_OFFSET_20 (NOR_PART_OFFSET_16+NOR_PART_SIZE_19)
    #define NOR_PART_OFFSET_21 (NOR_PART_OFFSET_16+NOR_PART_SIZE_20)

    // static partition must be the multiple of 64 KB.
    #if ((NOR_PART_SIZE_0 % (64*1024)) ||\
           (NOR_PART_SIZE_1 % (64*1024)) ||\
           (NOR_PART_SIZE_2 % (64*1024)) ||\
           (NOR_PART_SIZE_3 % (64*1024)) ||\
           (NOR_PART_SIZE_4 % (64*1024)) ||\
           (NOR_PART_SIZE_5 % (64*1024)))
    #error "nor Partition must be the multiple of 64 KB"
    #endif

    // check the summation of each partition must larger than total static size.
    #if ((NOR_PART_SIZE_0 + \
            NOR_PART_SIZE_1 + \
            NOR_PART_SIZE_2 + \
            NOR_PART_SIZE_3 + \
            NOR_PART_SIZE_4 + \
            NOR_PART_SIZE_5 + \
            NOR_PART_SIZE_6 + \
            NOR_PART_SIZE_7 + \
            NOR_PART_SIZE_8 + \
            NOR_PART_SIZE_9 + \
            NOR_PART_SIZE_10) \
            > (NOR_FLASH_TOTAL_SIZE))
    #error "NOR_FLASH_TOTAL_SIZE too small !!"
    #endif

//----------------------------------------------------------------------------
// Dual image customization
//----------------------------------------------------------------------------
#ifndef LOADER_DUAL_BOOT_OFFSET
    #ifdef CC_DUAL_IMAGE_FLASH
        #if defined(CC_EMMC_BOOT)
            #define LOADER_DUAL_BOOT_OFFSET             (0x640000)
        #elif defined(CC_NAND_LOADER)
            #if defined(CI_PLUS_SUPPORT)
                #define LOADER_DUAL_BOOT_OFFSET         (0x740000)
            #else
                #define LOADER_DUAL_BOOT_OFFSET         (0x640000)
            #endif
        #else
            #define LOADER_DUAL_BOOT_OFFSET             (0x640000)
        #endif
    #else
        #define LOADER_DUAL_BOOT_OFFSET                 (0)
    #endif
#endif

//----------------------------------------------------------------------------
// NAND customization
//----------------------------------------------------------------------------
#ifndef NAND_FLASH_TOTAL_SIZE
    #ifdef LINUX_TURNKEY_SOLUTION
        #define NAND_FLASH_TOTAL_SIZE                   (256*1024*1024)
    #else
        #define NAND_FLASH_TOTAL_SIZE                   (16*1024*1024)
    #endif
#endif

#ifndef NAND_FLASH_LOADER_SIZE
    #ifdef LOADER_MAX_SIZE
        #define NAND_FLASH_LOADER_SIZE                  (LOADER_MAX_SIZE)
    #else
        #define NAND_FLASH_LOADER_SIZE                  (128*1024)
    #endif
#endif

#if defined(CC_NAND_ENABLE) || defined(CC_EMMC_BOOT)
    #ifndef NAND_FLASH_APPIMAGE_SIZE
        #if defined(DLNA_SUPPORT) || defined(CI_PLUS_SUPPORT)
            #define NAND_FLASH_APPIMAGE_SIZE            (7*1024*1024)
        #else
            #define NAND_FLASH_APPIMAGE_SIZE            (6*1024*1024)
        #endif
    #endif

    #ifndef NAND_FLASH_UP_SIZE
        #define NAND_FLASH_UP_SIZE                      (0)
    #endif

    #ifndef NAND_FLASH_CHANNELLIST_SIZE
        #define NAND_FLASH_CHANNELLIST_SIZE             (128*1024)
    #endif

    #ifndef NAND_FLASH_PQ_SIZE
        #define NAND_FLASH_PQ_SIZE                      (128*1024)
    #endif

    #ifndef NAND_FLASH_AQ_SIZE
        #ifdef SUPPORT_FLASH_AQ
            #define NAND_FLASH_AQ_SIZE                  (128*1024)
        #else
            #define NAND_FLASH_AQ_SIZE                  (0)
        #endif
    #endif

    #ifndef NAND_FLASH_CIPLUS_SIZE
        #ifdef CI_PLUS_SUPPORT
            #define NAND_FLASH_CIPLUS_SIZE              (128*1024)
        #else
            #define NAND_FLASH_CIPLUS_SIZE              (0)
        #endif
    #endif

    #ifndef NAND_FLASH_NTFS_SIZE
        #ifdef FM_SUPPORT_DYNAMIC_NTFS
            #define NAND_FLASH_NTFS_SIZE                (128 * 1024)
        #else
            #define NAND_FLASH_NTFS_SIZE                (0)
        #endif
    #endif

    #ifndef NAND_FLASH_CAPTURE_SIZE
        #ifdef CAPTURE_LOGO_SUPPORT
            #define NAND_FLASH_CAPTURE_SIZE             (1024*1024)
        #else
            #define NAND_FLASH_CAPTURE_SIZE             (0)
        #endif
    #endif

    #ifndef NAND_FLASH_DEFAULT_DB_SIZE
        #ifdef DEFAULT_DB_SUPPORT
            #define NAND_FLASH_DEFAULT_DB_SIZE          (128*1024)
        #else
            #define NAND_FLASH_DEFAULT_DB_SIZE          (0)
        #endif
    #endif

    #ifndef NAND_FLASH_MHEG5_NVM_SIZE
        #ifdef MHEG5_NVM_SUPPORT
            #define NAND_FLASH_MHEG5_NVM_SIZE           (2*1024*1024)
        #else
            #define NAND_FLASH_MHEG5_NVM_SIZE           (0)
        #endif
    #endif

    #ifndef LINUX_TURNKEY_SOLUTION
        #ifndef NAND_PART_SIZE_0
            #define NAND_PART_SIZE_0        (NAND_FLASH_LOADER_SIZE)                // loader
        #endif
        #ifndef NAND_PART_SIZE_1
            #define NAND_PART_SIZE_1        (NAND_FLASH_APPIMAGE_SIZE)              // appimg
        #endif
        #ifndef NAND_PART_SIZE_2
            #define NAND_PART_SIZE_2        (NAND_FLASH_UP_SIZE)                    // up image
        #endif
        #ifndef NAND_PART_SIZE_3
            #define NAND_PART_SIZE_3        (NAND_FLASH_APPIMAGE_SIZE)              // reserved
        #endif
        #ifndef NAND_PART_SIZE_4
            #define NAND_PART_SIZE_4        (NAND_FLASH_UP_SIZE)                    // appimg
        #endif
        #ifndef NAND_PART_SIZE_5
            #define NAND_PART_SIZE_5        (NAND_FLASH_CHANNELLIST_SIZE)           // channel list
        #endif
        #ifndef NAND_PART_SIZE_6
            #define NAND_PART_SIZE_6        (NAND_FLASH_CHANNELLIST_SIZE)           // channel list
        #endif
        #ifndef NAND_PART_SIZE_7
            #define NAND_PART_SIZE_7        (NAND_FLASH_PQ_SIZE)                    // flash PQ
        #endif

        #ifndef NAND_PART_SIZE_8
            #ifdef CI_PLUS_SUPPORT
                #define NAND_PART_SIZE_8    (NAND_FLASH_CIPLUS_SIZE)
            #else
                #define NAND_PART_SIZE_8    (NAND_FLASH_NTFS_SIZE)
            #endif
        #endif

        #ifndef NAND_PART_SIZE_9
            #define NAND_PART_SIZE_9        (NAND_FLASH_AQ_SIZE)
        #endif
        #ifndef NAND_PART_SIZE_10
            #define NAND_PART_SIZE_10       (NAND_FLASH_CAPTURE_SIZE)
        #endif
        #ifndef NAND_PART_SIZE_11
            #define NAND_PART_SIZE_11       (0)
        #endif
        #ifndef NAND_PART_SIZE_12
            #define NAND_PART_SIZE_12       (NAND_FLASH_MHEG5_NVM_SIZE)
        #endif
        #ifndef NAND_PART_SIZE_13
            #define NAND_PART_SIZE_13       (0)
        #endif
        #ifndef NAND_PART_SIZE_14
            #define NAND_PART_SIZE_14       (0)
        #endif
        #ifndef NAND_PART_SIZE_15
            #define NAND_PART_SIZE_15       (0)
        #endif
        #ifndef NAND_PART_SIZE_16
            #define NAND_PART_SIZE_16       (0)
        #endif
        #ifndef NAND_PART_SIZE_17
            #define NAND_PART_SIZE_17       (0)
        #endif
    #else /* LINUX_TURNKEY_SOLUTION */
        #ifdef CC_FAST_INIT
            #define FAST_INIT_SUSP_SIZE     (90*1024*1024)
        #endif

        #ifndef NAND_PART_SIZE_0
            #define NAND_PART_SIZE_0        (2*1024*1024)                      // uboot
        #endif
        #ifndef NAND_PART_SIZE_1
            #define NAND_PART_SIZE_1        (2*1024*1024)                      // uboot_env
        #endif
        #ifndef NAND_PART_SIZE_2
            #define NAND_PART_SIZE_2        (256*1024)                      // reserved
        #endif
        #ifndef NAND_PART_SIZE_3
            #define NAND_PART_SIZE_3        (256*1024)                      // reserved
        #endif
        #ifndef NAND_PART_SIZE_4
            #define NAND_PART_SIZE_4        (3*1024*1024)                      // kernelA
        #endif
        #ifndef NAND_PART_SIZE_5
            #define NAND_PART_SIZE_5        (3*1024*1024)                      // kernelB
        #endif
        #ifndef NAND_PART_SIZE_6
            #ifndef CC_DISABLE_BASIC_PART
            #define NAND_PART_SIZE_6        (10*1024*1024)                     // rootfsA
            #else
            #define NAND_PART_SIZE_6        (25*1024*1024)                     // rootfsA
            #endif
        #endif
        #ifndef NAND_PART_SIZE_7
            #ifndef CC_DISABLE_BASIC_PART
            #define NAND_PART_SIZE_7        (10*1024*1024)                     // rootfsB
            #else
            #define NAND_PART_SIZE_7        (25*1024*1024)                     // rootfsB
            #endif
        #endif
        #ifndef NAND_PART_SIZE_8
            #ifndef CC_DISABLE_BASIC_PART
                #define NAND_PART_SIZE_8        (25*1024*1024)                     // basic
            #else
                #define NAND_PART_SIZE_8        (128*1024)                         // reserved2
            #endif
        #endif

        #ifndef NAND_PART_SIZE_9
            #ifdef CC_FAST_INIT
                #define NAND_PART_SIZE_9    (135*1024*1024 - FAST_INIT_SUSP_SIZE)   // rw_area
            #else
                #define NAND_PART_SIZE_9    (135*1024*1024)                         // rw_area
            #endif
        #endif

        #ifndef NAND_PART_SIZE_10
            #ifdef CC_PERM_DISABLE
                #define NAND_PART_SIZE_10   (128*1024)                      // perm
            #else
                #define NAND_PART_SIZE_10   (5*1024*1024)                   // perm
            #endif
        #endif

        #ifndef NAND_PART_SIZE_11
            #ifdef CC_FAST_INIT
                #define NAND_PART_SIZE_11   (FAST_INIT_SUSP_SIZE)           // reserved
            #else
                #define NAND_PART_SIZE_11   (128*1024)                      // reserved
            #endif
        #endif

        #ifndef NAND_PART_SIZE_12
            #define NAND_PART_SIZE_12       (256*1024)                      // channelsA
        #endif

        #ifndef NAND_PART_SIZE_13
            #define NAND_PART_SIZE_13       (256*1024)                      // channelsA
        #endif
        #ifndef NAND_PART_SIZE_14
            #define NAND_PART_SIZE_14       (256*1024)                      // channelsB
        #endif
        #ifndef NAND_PART_SIZE_15
            #define NAND_PART_SIZE_15       (128*1024)                      // pq
        #endif
        #ifndef NAND_PART_SIZE_16
            #define NAND_PART_SIZE_16       (128*1024)                      // aq
        #endif
        #ifndef NAND_PART_SIZE_17
            #define NAND_PART_SIZE_17       (1024*1024)                      // logo
        #endif
    #endif /* LINUX_TURNKEY_SOLUTION */

    #ifndef NAND_PART_SIZE_18
        #define NAND_PART_SIZE_18            (0)
    #endif
    #ifndef NAND_PART_SIZE_19
        #define NAND_PART_SIZE_19            (0)
    #endif
    #ifndef NAND_PART_SIZE_20
        #define NAND_PART_SIZE_20            (0)
    #endif
    #ifndef NAND_PART_SIZE_21
        #define NAND_PART_SIZE_21            (0)
    #endif
    #ifndef NAND_PART_SIZE_22
        #define NAND_PART_SIZE_22           (0)
    #endif
    #ifndef NAND_PART_SIZE_23
        #define NAND_PART_SIZE_23           (0)
    #endif
    #ifndef NAND_PART_SIZE_24
        #define NAND_PART_SIZE_24           (0)
    #endif
    #ifndef NAND_PART_SIZE_25
        #define NAND_PART_SIZE_25           (0)
    #endif
    #ifndef NAND_PART_SIZE_26
        #define NAND_PART_SIZE_26           (0)
    #endif
    #ifndef NAND_PART_SIZE_27
        #define NAND_PART_SIZE_27           (0)
    #endif
    #ifndef NAND_PART_SIZE_28
        #define NAND_PART_SIZE_28           (0)
    #endif
    #ifndef NAND_PART_SIZE_29
        #define NAND_PART_SIZE_29           (0)
    #endif
    #ifndef NAND_PART_SIZE_30
        #define NAND_PART_SIZE_30           (0)
    #endif
    #ifndef NAND_PART_SIZE_31
        #define NAND_PART_SIZE_31           (0)
    #endif
#ifdef CC_MTD_SUPPORT_64_PARTITION    
    #ifndef NAND_PART_SIZE_32
        #define NAND_PART_SIZE_32           (0)
    #endif    
	#ifndef NAND_PART_SIZE_33
        #define NAND_PART_SIZE_33           (0)
    #endif    
	#ifndef NAND_PART_SIZE_34
        #define NAND_PART_SIZE_34           (0)
    #endif    
	#ifndef NAND_PART_SIZE_35
        #define NAND_PART_SIZE_35           (0)
    #endif    
	#ifndef NAND_PART_SIZE_36
        #define NAND_PART_SIZE_36           (0)
    #endif    
	#ifndef NAND_PART_SIZE_37
        #define NAND_PART_SIZE_37           (0)
    #endif    
	#ifndef NAND_PART_SIZE_38
        #define NAND_PART_SIZE_38           (0)
    #endif    
	#ifndef NAND_PART_SIZE_39
        #define NAND_PART_SIZE_39           (0)
    #endif    
	#ifndef NAND_PART_SIZE_40
        #define NAND_PART_SIZE_40           (0)
    #endif    
	#ifndef NAND_PART_SIZE_41
        #define NAND_PART_SIZE_41           (0)
    #endif    
	#ifndef NAND_PART_SIZE_42
        #define NAND_PART_SIZE_42           (0)
    #endif    
	#ifndef NAND_PART_SIZE_43
        #define NAND_PART_SIZE_43           (0)
    #endif    
	#ifndef NAND_PART_SIZE_44
        #define NAND_PART_SIZE_44           (0)
    #endif    
	#ifndef NAND_PART_SIZE_45
        #define NAND_PART_SIZE_45           (0)
    #endif    
	#ifndef NAND_PART_SIZE_46
        #define NAND_PART_SIZE_46           (0)
    #endif    
	#ifndef NAND_PART_SIZE_47
        #define NAND_PART_SIZE_47           (0)
    #endif    
	#ifndef NAND_PART_SIZE_48
        #define NAND_PART_SIZE_48           (0)
    #endif    
	#ifndef NAND_PART_SIZE_49
        #define NAND_PART_SIZE_49           (0)
    #endif    
	#ifndef NAND_PART_SIZE_50
        #define NAND_PART_SIZE_50           (0)
    #endif    
	#ifndef NAND_PART_SIZE_51
        #define NAND_PART_SIZE_51           (0)
    #endif    
	#ifndef NAND_PART_SIZE_52
        #define NAND_PART_SIZE_52           (0)
    #endif    
	#ifndef NAND_PART_SIZE_53
        #define NAND_PART_SIZE_53           (0)
    #endif    
	#ifndef NAND_PART_SIZE_54
        #define NAND_PART_SIZE_54           (0)
    #endif    
	#ifndef NAND_PART_SIZE_55
        #define NAND_PART_SIZE_55           (0)
    #endif	
	#ifndef NAND_PART_SIZE_56
        #define NAND_PART_SIZE_56           (0)
    #endif    
	#ifndef NAND_PART_SIZE_57
        #define NAND_PART_SIZE_57           (0)
    #endif    
	#ifndef NAND_PART_SIZE_58
        #define NAND_PART_SIZE_58           (0)
    #endif    
	#ifndef NAND_PART_SIZE_59
        #define NAND_PART_SIZE_59           (0)
    #endif    
	#ifndef NAND_PART_SIZE_60
        #define NAND_PART_SIZE_60           (0)
    #endif    
	#ifndef NAND_PART_SIZE_61
        #define NAND_PART_SIZE_61           (0)
    #endif    
	#ifndef NAND_PART_SIZE_62
        #define NAND_PART_SIZE_62           (0)
    #endif    
	#ifndef NAND_PART_SIZE_63
        #define NAND_PART_SIZE_63           (0)
    #endif
#endif

    #define NAND_PART_OFFSET_1              (NAND_PART_SIZE_0)
    #define NAND_PART_OFFSET_2              (NAND_PART_OFFSET_1+NAND_PART_SIZE_1)
    #define NAND_PART_OFFSET_3              (NAND_PART_OFFSET_2+NAND_PART_SIZE_2)
    #define NAND_PART_OFFSET_4              (NAND_PART_OFFSET_3+NAND_PART_SIZE_3)
    #define NAND_PART_OFFSET_5              (NAND_PART_OFFSET_4+NAND_PART_SIZE_4)
    #define NAND_PART_OFFSET_6              (NAND_PART_OFFSET_5+NAND_PART_SIZE_5)
    #define NAND_PART_OFFSET_7              (NAND_PART_OFFSET_6+NAND_PART_SIZE_6)
    #define NAND_PART_OFFSET_8              (NAND_PART_OFFSET_7+NAND_PART_SIZE_7)
    #define NAND_PART_OFFSET_9              (NAND_PART_OFFSET_8+NAND_PART_SIZE_8)
    #define NAND_PART_OFFSET_10             (NAND_PART_OFFSET_9+NAND_PART_SIZE_9)
    #define NAND_PART_OFFSET_11             (NAND_PART_OFFSET_10+NAND_PART_SIZE_10)
    #define NAND_PART_OFFSET_12             (NAND_PART_OFFSET_11+NAND_PART_SIZE_11)
    #define NAND_PART_OFFSET_13             (NAND_PART_OFFSET_12+NAND_PART_SIZE_12)
    #define NAND_PART_OFFSET_14             (NAND_PART_OFFSET_13+NAND_PART_SIZE_13)
    #define NAND_PART_OFFSET_15             (NAND_PART_OFFSET_14+NAND_PART_SIZE_14)
    #define NAND_PART_OFFSET_16             (NAND_PART_OFFSET_15+NAND_PART_SIZE_15)
    #define NAND_PART_OFFSET_17             (NAND_PART_OFFSET_16+NAND_PART_SIZE_16)
    #define NAND_PART_OFFSET_18             (NAND_PART_OFFSET_17+NAND_PART_SIZE_17)
    #define NAND_PART_OFFSET_19             (NAND_PART_OFFSET_18+NAND_PART_SIZE_18)
    #define NAND_PART_OFFSET_20             (NAND_PART_OFFSET_19+NAND_PART_SIZE_19)
    #define NAND_PART_OFFSET_21             (NAND_PART_OFFSET_20+NAND_PART_SIZE_20)
    #define NAND_PART_OFFSET_22             (NAND_PART_OFFSET_21+NAND_PART_SIZE_21)
    #define NAND_PART_OFFSET_23             (NAND_PART_OFFSET_22+NAND_PART_SIZE_22)
    #define NAND_PART_OFFSET_24             (NAND_PART_OFFSET_23+NAND_PART_SIZE_23)
    #define NAND_PART_OFFSET_25             (NAND_PART_OFFSET_24+NAND_PART_SIZE_24)
    #define NAND_PART_OFFSET_26             (NAND_PART_OFFSET_25+NAND_PART_SIZE_25)
    #define NAND_PART_OFFSET_27             (NAND_PART_OFFSET_26+NAND_PART_SIZE_26)
    #define NAND_PART_OFFSET_28             (NAND_PART_OFFSET_27+NAND_PART_SIZE_27)
    #define NAND_PART_OFFSET_29             (NAND_PART_OFFSET_28+NAND_PART_SIZE_28)
    #define NAND_PART_OFFSET_30             (NAND_PART_OFFSET_29+NAND_PART_SIZE_29)
    #define NAND_PART_OFFSET_31             (NAND_PART_OFFSET_30+NAND_PART_SIZE_30)
#ifdef CC_MTD_SUPPORT_64_PARTITION    
    #define NAND_PART_OFFSET_32             (NAND_PART_OFFSET_31+NAND_PART_SIZE_31)
	#define NAND_PART_OFFSET_33             (NAND_PART_OFFSET_32+NAND_PART_SIZE_32)
	#define NAND_PART_OFFSET_34             (NAND_PART_OFFSET_33+NAND_PART_SIZE_33)
	#define NAND_PART_OFFSET_35             (NAND_PART_OFFSET_34+NAND_PART_SIZE_34)    
	#define NAND_PART_OFFSET_36             (NAND_PART_OFFSET_35+NAND_PART_SIZE_35)
	#define NAND_PART_OFFSET_37             (NAND_PART_OFFSET_36+NAND_PART_SIZE_36)
	#define NAND_PART_OFFSET_38             (NAND_PART_OFFSET_37+NAND_PART_SIZE_37)
	#define NAND_PART_OFFSET_39             (NAND_PART_OFFSET_38+NAND_PART_SIZE_38)    
	#define NAND_PART_OFFSET_40             (NAND_PART_OFFSET_39+NAND_PART_SIZE_39)
	#define NAND_PART_OFFSET_41             (NAND_PART_OFFSET_40+NAND_PART_SIZE_40)
	#define NAND_PART_OFFSET_42             (NAND_PART_OFFSET_41+NAND_PART_SIZE_41)
	#define NAND_PART_OFFSET_43             (NAND_PART_OFFSET_42+NAND_PART_SIZE_42)    
	#define NAND_PART_OFFSET_44             (NAND_PART_OFFSET_43+NAND_PART_SIZE_43)
	#define NAND_PART_OFFSET_45             (NAND_PART_OFFSET_44+NAND_PART_SIZE_44)
	#define NAND_PART_OFFSET_46             (NAND_PART_OFFSET_45+NAND_PART_SIZE_45)
	#define NAND_PART_OFFSET_47             (NAND_PART_OFFSET_46+NAND_PART_SIZE_46)	
	#define NAND_PART_OFFSET_48             (NAND_PART_OFFSET_47+NAND_PART_SIZE_47)
	#define NAND_PART_OFFSET_49             (NAND_PART_OFFSET_48+NAND_PART_SIZE_48)    
	#define NAND_PART_OFFSET_50             (NAND_PART_OFFSET_49+NAND_PART_SIZE_49)
	#define NAND_PART_OFFSET_51             (NAND_PART_OFFSET_50+NAND_PART_SIZE_50)
	#define NAND_PART_OFFSET_52             (NAND_PART_OFFSET_51+NAND_PART_SIZE_51)
	#define NAND_PART_OFFSET_53             (NAND_PART_OFFSET_52+NAND_PART_SIZE_52)    
	#define NAND_PART_OFFSET_54             (NAND_PART_OFFSET_53+NAND_PART_SIZE_53)
	#define NAND_PART_OFFSET_55             (NAND_PART_OFFSET_54+NAND_PART_SIZE_54)
	#define NAND_PART_OFFSET_56             (NAND_PART_OFFSET_55+NAND_PART_SIZE_55)
	#define NAND_PART_OFFSET_57             (NAND_PART_OFFSET_56+NAND_PART_SIZE_56)	
	#define NAND_PART_OFFSET_58             (NAND_PART_OFFSET_57+NAND_PART_SIZE_57)
	#define NAND_PART_OFFSET_59             (NAND_PART_OFFSET_58+NAND_PART_SIZE_58)    
	#define NAND_PART_OFFSET_60             (NAND_PART_OFFSET_59+NAND_PART_SIZE_59)
	#define NAND_PART_OFFSET_61             (NAND_PART_OFFSET_60+NAND_PART_SIZE_60)
	#define NAND_PART_OFFSET_62             (NAND_PART_OFFSET_61+NAND_PART_SIZE_61)
	#define NAND_PART_OFFSET_63             (NAND_PART_OFFSET_62+NAND_PART_SIZE_62)  
#endif

    #ifndef NAND_PART_NAME_0
        #ifdef LINUX_TURNKEY_SOLUTION
            #define NAND_PART_NAME_0            "uboot"
        #else
            #define NAND_PART_NAME_0            "part_00"
        #endif
    #endif

    #ifndef NAND_PART_NAME_1
        #ifdef LINUX_TURNKEY_SOLUTION
            #define NAND_PART_NAME_1            "uboot_env"
        #else
            #define NAND_PART_NAME_1            "part_01"
        #endif
    #endif

    #ifndef NAND_PART_NAME_2
        #ifdef LINUX_TURNKEY_SOLUTION
            #define NAND_PART_NAME_2            "part_02"
        #else
            #define NAND_PART_NAME_2            "part_02"
        #endif
    #endif

    #ifndef NAND_PART_NAME_3
        #ifdef LINUX_TURNKEY_SOLUTION
            #define NAND_PART_NAME_3            "part_03"
        #else
            #define NAND_PART_NAME_3            "part_03"
        #endif
    #endif

    #ifndef NAND_PART_NAME_4
        #ifdef LINUX_TURNKEY_SOLUTION
            #define NAND_PART_NAME_4            "kernelA"
        #else
            #define NAND_PART_NAME_4            "part_04"
        #endif
    #endif

    #ifndef NAND_PART_NAME_5
        #ifdef LINUX_TURNKEY_SOLUTION
            #define NAND_PART_NAME_5            "kernelB"
        #else
            #define NAND_PART_NAME_5            "part_05"
        #endif
    #endif

    #ifndef NAND_PART_NAME_6
        #ifdef LINUX_TURNKEY_SOLUTION
            #define NAND_PART_NAME_6            "rootfsA"
        #else
            #define NAND_PART_NAME_6            "part_06"
        #endif
    #endif

    #ifndef NAND_PART_NAME_7
        #ifdef LINUX_TURNKEY_SOLUTION
            #define NAND_PART_NAME_7            "rootfsB"
        #else
            #define NAND_PART_NAME_7            "part_07"
        #endif
    #endif

    #ifndef NAND_PART_NAME_8
        #ifdef LINUX_TURNKEY_SOLUTION
            #define NAND_PART_NAME_8            "basic"
        #else
            #define NAND_PART_NAME_8            "part_08"
        #endif
    #endif

    #ifndef NAND_PART_NAME_9
        #ifdef LINUX_TURNKEY_SOLUTION
            #define NAND_PART_NAME_9           "perm"
        #else
            #define NAND_PART_NAME_9            "part_09"
        #endif
    #endif

    #ifndef NAND_PART_NAME_10
        #ifdef LINUX_TURNKEY_SOLUTION
	#ifdef CC_3RD_RO
		#define NAND_PART_NAME_10		"3rd_ro"
	#else
		#define NAND_PART_NAME_10		"reserved"
	#endif
        #else
            #define NAND_PART_NAME_10           "part_10"
        #endif
    #endif
    
    #ifndef NAND_PART_NAME_11
        #ifdef LINUX_TURNKEY_SOLUTION
            #define NAND_PART_NAME_11            "rw_area"
        #else
            #define NAND_PART_NAME_11            "part_11"
        #endif
    #endif
    
    #ifndef NAND_PART_NAME_12
        #ifdef LINUX_TURNKEY_SOLUTION
            #ifdef CC_FAST_INIT
                #define NAND_PART_NAME_12       "susp"
            #else
                #define NAND_PART_NAME_12       "reserved"
            #endif
        #else
            #define NAND_PART_NAME_12           "part_12"
        #endif
    #endif

    #ifndef NAND_PART_NAME_13
        #ifdef LINUX_TURNKEY_SOLUTION
		#define NAND_PART_NAME_13			"channelA"
        #else
            #define NAND_PART_NAME_13           "part_13"
        #endif
    #endif

    #ifndef NAND_PART_NAME_14
        #ifdef LINUX_TURNKEY_SOLUTION
		#define NAND_PART_NAME_14			"channelB"
        #else
            #define NAND_PART_NAME_14           "part_14"
        #endif
    #endif

    #ifndef NAND_PART_NAME_15
        #ifdef LINUX_TURNKEY_SOLUTION
		#define NAND_PART_NAME_15			"pq"
        #else
            #define NAND_PART_NAME_15           "part_15"
        #endif
    #endif

    #ifndef NAND_PART_NAME_16
        #ifdef LINUX_TURNKEY_SOLUTION
		#define NAND_PART_NAME_16			"aq"
        #else
            #define NAND_PART_NAME_16           "part_16"
        #endif
    #endif

    #ifndef NAND_PART_NAME_17
        #ifdef LINUX_TURNKEY_SOLUTION
		#define NAND_PART_NAME_17			"logo"
        #else
            #define NAND_PART_NAME_17           "part_17"
        #endif
    #endif

    #ifndef NAND_PART_NAME_18
	#ifdef LINUX_TURNKEY_SOLUTION
		#ifdef CI_PLUS_SUPPORT
			#define NAND_PART_NAME_18		"ci"
		#else
			#define NAND_PART_NAME_18		"part_18"
		#endif
	#else
        #define NAND_PART_NAME_18               "part_18"
    #endif
#endif

    #ifndef NAND_PART_NAME_19
        #define NAND_PART_NAME_19               "part_19"
    #endif

    #ifndef NAND_PART_NAME_20
        #define NAND_PART_NAME_20               "part_20"
    #endif

    #ifndef NAND_PART_NAME_21
        #define NAND_PART_NAME_21               "part_21"
    #endif

    #ifndef NAND_PART_NAME_22
        #define NAND_PART_NAME_22               "part_22"
    #endif

    #ifndef NAND_PART_NAME_23
        #define NAND_PART_NAME_23               "part_23"
    #endif

    #ifndef NAND_PART_NAME_24
        #define NAND_PART_NAME_24               "part_24"
    #endif

    #ifndef NAND_PART_NAME_25
        #define NAND_PART_NAME_25               "part_25"
    #endif

    #ifndef NAND_PART_NAME_26
        #define NAND_PART_NAME_26               "part_26"
    #endif

    #ifndef NAND_PART_NAME_27
        #define NAND_PART_NAME_27               "part_27"
    #endif

    #ifndef NAND_PART_NAME_28
        #define NAND_PART_NAME_28               "part_28"
    #endif

    #ifndef NAND_PART_NAME_29
        #define NAND_PART_NAME_29               "part_29"
    #endif

    #ifndef NAND_PART_NAME_30
        #define NAND_PART_NAME_30               "part_30"
    #endif

    #ifndef NAND_PART_NAME_31
        #define NAND_PART_NAME_31               "part_31"
    #endif

#ifdef CC_MTD_SUPPORT_64_PARTITION    
    #ifndef NAND_PART_NAME_32
        #define NAND_PART_NAME_32               "part_32"
    #endif    
	#ifndef NAND_PART_NAME_33
        #define NAND_PART_NAME_33               "part_33"
    #endif    
	#ifndef NAND_PART_NAME_34
        #define NAND_PART_NAME_34               "part_34"
    #endif    
	#ifndef NAND_PART_NAME_35
        #define NAND_PART_NAME_35               "part_35"
    #endif    
	#ifndef NAND_PART_NAME_36
        #define NAND_PART_NAME_36               "part_36"
    #endif    
	#ifndef NAND_PART_NAME_37
        #define NAND_PART_NAME_37               "part_37"
    #endif    
	#ifndef NAND_PART_NAME_38
        #define NAND_PART_NAME_38               "part_38"
    #endif    
	#ifndef NAND_PART_NAME_39
        #define NAND_PART_NAME_39               "part_39"
    #endif    
	#ifndef NAND_PART_NAME_40
        #define NAND_PART_NAME_40               "part_40"
    #endif    
	#ifndef NAND_PART_NAME_41
        #define NAND_PART_NAME_41               "part_41"
    #endif    
	#ifndef NAND_PART_NAME_42
        #define NAND_PART_NAME_42               "part_42"
    #endif    
	#ifndef NAND_PART_NAME_43
        #define NAND_PART_NAME_43               "part_43"
    #endif    
	#ifndef NAND_PART_NAME_44
        #define NAND_PART_NAME_44               "part_44"
    #endif    
	#ifndef NAND_PART_NAME_45
        #define NAND_PART_NAME_45               "part_45"
    #endif    
	#ifndef NAND_PART_NAME_46
        #define NAND_PART_NAME_46               "part_46"
    #endif    
	#ifndef NAND_PART_NAME_47
        #define NAND_PART_NAME_47               "part_47"
    #endif    
	#ifndef NAND_PART_NAME_48
        #define NAND_PART_NAME_48               "part_48"
    #endif    
	#ifndef NAND_PART_NAME_49
        #define NAND_PART_NAME_49               "part_49"
    #endif    
	#ifndef NAND_PART_NAME_50
        #define NAND_PART_NAME_50               "part_50"
    #endif    
	#ifndef NAND_PART_NAME_51
        #define NAND_PART_NAME_51               "part_51"
    #endif    
	#ifndef NAND_PART_NAME_52
        #define NAND_PART_NAME_52               "part_52"
    #endif    
	#ifndef NAND_PART_NAME_53
        #define NAND_PART_NAME_53               "part_53"
    #endif    
	#ifndef NAND_PART_NAME_54
        #define NAND_PART_NAME_54               "part_54"
    #endif    
	#ifndef NAND_PART_NAME_55
        #define NAND_PART_NAME_55               "part_55"
    #endif	
	#ifndef NAND_PART_NAME_56
        #define NAND_PART_NAME_56               "part_56"
    #endif    
	#ifndef NAND_PART_NAME_57
        #define NAND_PART_NAME_57               "part_57"
    #endif    
	#ifndef NAND_PART_NAME_58
        #define NAND_PART_NAME_58               "part_58"
    #endif    
	#ifndef NAND_PART_NAME_59
        #define NAND_PART_NAME_59               "part_59"
    #endif    
	#ifndef NAND_PART_NAME_60
        #define NAND_PART_NAME_60               "part_60"
    #endif    
	#ifndef NAND_PART_NAME_61
        #define NAND_PART_NAME_61               "part_61"
    #endif    
	#ifndef NAND_PART_NAME_62
        #define NAND_PART_NAME_62               "part_62"
    #endif    
	#ifndef NAND_PART_NAME_63
        #define NAND_PART_NAME_63               "part_63"
    #endif  
#endif

    #ifdef CC_MTD_ENCRYPT_SUPPORT
        #ifndef NAND_PART_ENCRYPT_0
            #define NAND_PART_ENCRYPT_0             (0)
        #endif
        #ifndef NAND_PART_ENCRYPT_1
            #define NAND_PART_ENCRYPT_1             (1) /* Uenv */
        #endif
        #ifndef NAND_PART_ENCRYPT_2
            #define NAND_PART_ENCRYPT_2             (0)
        #endif
        #ifndef NAND_PART_ENCRYPT_3
            #define NAND_PART_ENCRYPT_3             (0)
        #endif
        #ifndef NAND_PART_ENCRYPT_4
            #define NAND_PART_ENCRYPT_4             (1) /* KernelA */
        #endif
        #ifndef NAND_PART_ENCRYPT_5
            #define NAND_PART_ENCRYPT_5             (1) /* KernelB */
        #endif
        #ifndef NAND_PART_ENCRYPT_6
            #define NAND_PART_ENCRYPT_6             (0) /* rootfsA */
        #endif
        #ifndef NAND_PART_ENCRYPT_7
            #define NAND_PART_ENCRYPT_7             (0) /* rootfsB */
        #endif
        #ifndef NAND_PART_ENCRYPT_8
            #define NAND_PART_ENCRYPT_8             (1) /* Basic */
        #endif
        #ifndef NAND_PART_ENCRYPT_9
            #define NAND_PART_ENCRYPT_9             (1) /* Perm */
        #endif
        #ifndef NAND_PART_ENCRYPT_10
            #define NAND_PART_ENCRYPT_10            (1) /*3rd_ro*/
        #endif
        #ifndef NAND_PART_ENCRYPT_11
            #define NAND_PART_ENCRYPT_11            (1)/* 3rd */
        #endif
        #ifndef NAND_PART_ENCRYPT_12
            #define NAND_PART_ENCRYPT_12            (0) 
        #endif
        #ifndef NAND_PART_ENCRYPT_13
            #define NAND_PART_ENCRYPT_13            (0)
        #endif
        #ifndef NAND_PART_ENCRYPT_14
            #define NAND_PART_ENCRYPT_14            (0)
        #endif
        #ifndef NAND_PART_ENCRYPT_15
            #define NAND_PART_ENCRYPT_15            (0)
        #endif
        #ifndef NAND_PART_ENCRYPT_16
            #define NAND_PART_ENCRYPT_16            (0)
        #endif
        #ifndef NAND_PART_ENCRYPT_17
            #define NAND_PART_ENCRYPT_17            (0)
        #endif
        #ifndef NAND_PART_ENCRYPT_18
            #define NAND_PART_ENCRYPT_18            (0)
        #endif
        #ifndef NAND_PART_ENCRYPT_19
            #define NAND_PART_ENCRYPT_19            (0)
        #endif
        #ifndef NAND_PART_ENCRYPT_20
            #define NAND_PART_ENCRYPT_20            (0)
        #endif
        #ifndef NAND_PART_ENCRYPT_21
            #define NAND_PART_ENCRYPT_21            (0)
        #endif
        #ifndef NAND_PART_ENCRYPT_22
            #define NAND_PART_ENCRYPT_22            (0)
        #endif
        #ifndef NAND_PART_ENCRYPT_23
            #define NAND_PART_ENCRYPT_23            (0)
        #endif
        #ifndef NAND_PART_ENCRYPT_24
            #define NAND_PART_ENCRYPT_24            (0)
        #endif
        #ifndef NAND_PART_ENCRYPT_25
            #define NAND_PART_ENCRYPT_25            (0)
        #endif
        #ifndef NAND_PART_ENCRYPT_26
            #define NAND_PART_ENCRYPT_26            (0)
        #endif
        #ifndef NAND_PART_ENCRYPT_27
            #define NAND_PART_ENCRYPT_27            (0)
        #endif
        #ifndef NAND_PART_ENCRYPT_28
            #define NAND_PART_ENCRYPT_28            (0)
        #endif
        #ifndef NAND_PART_ENCRYPT_29
            #define NAND_PART_ENCRYPT_29            (0)
        #endif
        #ifndef NAND_PART_ENCRYPT_30
            #define NAND_PART_ENCRYPT_30            (0)
        #endif
        #ifndef NAND_PART_ENCRYPT_31
            #define NAND_PART_ENCRYPT_31            (0)
        #endif
		#ifdef CC_MTD_SUPPORT_64_PARTITION       
		#ifndef NAND_PART_ENCRYPT_32
            #define NAND_PART_ENCRYPT_32            (0)
        #endif
        #ifndef NAND_PART_ENCRYPT_33
            #define NAND_PART_ENCRYPT_33            (0)
        #endif
        #ifndef NAND_PART_ENCRYPT_34
            #define NAND_PART_ENCRYPT_34            (0)
        #endif
        #ifndef NAND_PART_ENCRYPT_35
            #define NAND_PART_ENCRYPT_35            (0)
        #endif
        #ifndef NAND_PART_ENCRYPT_36
            #define NAND_PART_ENCRYPT_36            (0)
        #endif
        #ifndef NAND_PART_ENCRYPT_37
            #define NAND_PART_ENCRYPT_37            (0)
        #endif
        #ifndef NAND_PART_ENCRYPT_38
            #define NAND_PART_ENCRYPT_38            (0)
        #endif
        #ifndef NAND_PART_ENCRYPT_39
            #define NAND_PART_ENCRYPT_39            (0)
        #endif
        #ifndef NAND_PART_ENCRYPT_40
            #define NAND_PART_ENCRYPT_40            (0)
        #endif
        #ifndef NAND_PART_ENCRYPT_41
            #define NAND_PART_ENCRYPT_41            (0)
        #endif
        #ifndef NAND_PART_ENCRYPT_42
            #define NAND_PART_ENCRYPT_42            (0)
        #endif
        #ifndef NAND_PART_ENCRYPT_43
            #define NAND_PART_ENCRYPT_43            (0)
        #endif
        #ifndef NAND_PART_ENCRYPT_44
            #define NAND_PART_ENCRYPT_44            (0)
        #endif
        #ifndef NAND_PART_ENCRYPT_45
            #define NAND_PART_ENCRYPT_45            (0)
        #endif
        #ifndef NAND_PART_ENCRYPT_46
            #define NAND_PART_ENCRYPT_46            (0)
        #endif
        #ifndef NAND_PART_ENCRYPT_47
            #define NAND_PART_ENCRYPT_47            (0)
        #endif
        #ifndef NAND_PART_ENCRYPT_48
            #define NAND_PART_ENCRYPT_48            (0)
        #endif
        #ifndef NAND_PART_ENCRYPT_49
            #define NAND_PART_ENCRYPT_49            (0)
        #endif
        #ifndef NAND_PART_ENCRYPT_50
            #define NAND_PART_ENCRYPT_50            (0)
        #endif
        #ifndef NAND_PART_ENCRYPT_51
            #define NAND_PART_ENCRYPT_51            (0)
        #endif		
		#ifndef NAND_PART_ENCRYPT_52
            #define NAND_PART_ENCRYPT_52            (0)
        #endif
        #ifndef NAND_PART_ENCRYPT_53
            #define NAND_PART_ENCRYPT_53            (0)
        #endif
        #ifndef NAND_PART_ENCRYPT_54
            #define NAND_PART_ENCRYPT_54            (0)
        #endif
        #ifndef NAND_PART_ENCRYPT_55
            #define NAND_PART_ENCRYPT_55            (0)
        #endif
        #ifndef NAND_PART_ENCRYPT_56
            #define NAND_PART_ENCRYPT_56            (0)
        #endif
        #ifndef NAND_PART_ENCRYPT_57
            #define NAND_PART_ENCRYPT_57            (0)
        #endif
        #ifndef NAND_PART_ENCRYPT_58
            #define NAND_PART_ENCRYPT_58            (0)
        #endif
        #ifndef NAND_PART_ENCRYPT_59
            #define NAND_PART_ENCRYPT_59            (0)
        #endif
        #ifndef NAND_PART_ENCRYPT_60
            #define NAND_PART_ENCRYPT_60            (0)
        #endif
        #ifndef NAND_PART_ENCRYPT_61
            #define NAND_PART_ENCRYPT_61            (0)
        #endif
        #ifndef NAND_PART_ENCRYPT_62
            #define NAND_PART_ENCRYPT_62            (0)
        #endif
        #ifndef NAND_PART_ENCRYPT_63
            #define NAND_PART_ENCRYPT_63            (0)
        #endif
		#endif
    #endif /* CC_MTD_ENCRYPT_SUPPORT */

    #ifndef NAND_PART_ID_SYSIMG_A
        #define NAND_PART_ID_SYSIMG_A           (1)
    #endif

    #ifndef NAND_PART_ID_UP_A
        #define NAND_PART_ID_UP_A               (2)
    #endif

    #ifndef NAND_PART_ID_SYSIMG_B
        #define NAND_PART_ID_SYSIMG_B           (3)
    #endif

    #ifndef NAND_PART_ID_UP_B
        #define NAND_PART_ID_UP_B               (4)
    #endif

    #ifndef NAND_PART_ID_CH_A
        #define NAND_PART_ID_CH_A               (5)
    #endif

    #ifndef NAND_PART_ID_CH_B
        #define NAND_PART_ID_CH_B               (6)
    #endif

    #ifndef HDCP_NAND_FLASH_PART_ID
        #define HDCP_NAND_FLASH_PART_ID         (0)
    #endif

    #ifndef VGAEDID_NAND_FLASH_PART_ID
        #define VGAEDID_NAND_FLASH_PART_ID      (0)
    #endif

    #ifndef VGAEDID_NAND_FLASH_OFFSET
        #define VGAEDID_NAND_FLASH_OFFSET       (0)
    #endif

	#ifndef VGAEDID_NAND_FLASH_PART_SIZE
        #define VGAEDID_NAND_FLASH_PART_SIZE    (256)
    #endif

	#ifndef HDMIEDID_NAND_FLASH_PART_ID
        #define HDMIEDID_NAND_FLASH_PART_ID      (0)
    #endif

    #ifndef HDMIEDID_NAND_FLASH_OFFSET
        #define HDMIEDID_NAND_FLASH_OFFSET       (0)
    #endif

	#ifndef HDMIEDID_NAND_FLASH_PART_SIZE
        #define HDMIEDID_NAND_FLASH_PART_SIZE    (256)
    #endif

    #ifndef NAND_PART_ID_EEPROM_A
        #define NAND_PART_ID_EEPROM_A           (0)
    #endif

    #ifndef NAND_PART_ID_EEPROM_B
        #define NAND_PART_ID_EEPROM_B           (0)
    #endif

    #ifndef LINUX_TURNKEY_SOLUTION
        #ifndef NAND_FLASH_STATIC_PART_SIZE
            #define NAND_FLASH_STATIC_PART_SIZE \
                ((NAND_PART_SIZE_0)  + (NAND_PART_SIZE_1) + \
                 (NAND_PART_SIZE_2)  + (NAND_PART_SIZE_3) + \
                 (NAND_PART_SIZE_4))
        #endif

        #ifndef NAND_FLASH_STATIC_LAST_PART_ID
            #define NAND_FLASH_STATIC_LAST_PART_ID      (4)
        #endif
    #else
        #ifndef NAND_FLASH_STATIC_PART_SIZE
            #define NAND_FLASH_STATIC_PART_SIZE \
                ((NAND_PART_SIZE_0)  + (NAND_PART_SIZE_1) + \
                 (NAND_PART_SIZE_2)  + (NAND_PART_SIZE_3) + \
                 (NAND_PART_SIZE_4)  + (NAND_PART_SIZE_5) + \
                 (NAND_PART_SIZE_6)  + (NAND_PART_SIZE_7) + \
                 (NAND_PART_SIZE_8)  + (NAND_PART_SIZE_9) + \
                 (NAND_PART_SIZE_10) + (NAND_PART_SIZE_11))
        #endif

        #ifndef NAND_FLASH_STATIC_LAST_PART_ID
            #define NAND_FLASH_STATIC_LAST_PART_ID      (11)
        #endif
    #endif

    #ifndef NAND_FLASH_DYNAMIC_PART_SIZE
        #define NAND_FLASH_DYNAMIC_PART_SIZE        ((NAND_FLASH_TOTAL_SIZE) - (NAND_FLASH_STATIC_PART_SIZE))
    #endif

    #ifndef NAND_FLASH_DYNAMIC_PART_ID
        #define NAND_FLASH_DYNAMIC_PART_ID          (NAND_FLASH_STATIC_LAST_PART_ID + 1)
    #endif

    #ifndef NAND_PART_END
	    #ifdef CC_MTD_SUPPORT_64_PARTITION
		#define NAND_PART_END                       (63)    // (NANDPART_PARTNUM - 1)
		#else
        #define NAND_PART_END                       (31)    // (NANDPART_PARTNUM - 1)
    #endif
    #endif

#endif /* CC_NAND_ENABLE */

//----------------------------------------------------------------------------
// 8032uP Customization
//----------------------------------------------------------------------------
#ifndef T8032UP_OFFSET
#define T8032UP_OFFSET             (0)
#endif
#ifndef T8032UP_DUAL_OFFSET
#define T8032UP_DUAL_OFFSET        (0)
#endif
#ifndef T8032UP_SIZE
#define T8032UP_SIZE        (0)
#endif

#ifndef SUPPORT_HOTKEY_WAKEUP
#define SUPPORT_HOTKEY_WAKEUP		(0)
#endif

#ifndef HOTKEY_WAKEUP_NUM
#define HOTKEY_WAKEUP_NUM        	(4)
#endif

#ifndef HOTKEY_WAKEUP_NUM_EEP_OFFSET
/* the last byte of driver custom part. */
#define HOTKEY_WAKEUP_NUM_EEP_OFFSET        	(DRVCUST_EEPROM_CUSTOM_DRIVER_OFFSET + DRVCUST_EEPROM_CUSTOM_DRIVER_SIZE - 1)
#endif

#ifndef HOTKEY_WAKEUP_EEP_OFFSET
/* the last (n*byte + 1) of driver custom part. It should be calculate by the number of keys. */
#define HOTKEY_WAKEUP_EEP_OFFSET  	(DRVCUST_EEPROM_CUSTOM_DRIVER_OFFSET + DRVCUST_EEPROM_CUSTOM_DRIVER_SIZE - 1 - HOTKEY_WAKEUP_NUM*4)
#endif

//----------------------------------------------------------------------------
// Button Customization
//----------------------------------------------------------------------------
#ifndef FLAG_BUTTON_SUPPORT
#define FLAG_BUTTON_SUPPORT         (0)
#endif
#ifndef SINGLE_GPIO_KEY_LIST
#define SINGLE_GPIO_KEY_LIST        (NULL)
#endif
#ifndef SINGLE_SERVO_KEY_LIST
#define SINGLE_SERVO_KEY_LIST       (NULL)
#endif
#ifndef GREED_GPIO_KEY_LIST
#define GREED_GPIO_KEY_LIST         (NULL)
#endif
#ifndef MULTI_KEY_LIST
#define MULTI_KEY_LIST              (NULL)
#endif
#ifndef COMB_KEY_LIST
#define COMB_KEY_LIST              (NULL)
#endif
#ifndef KEYPAD_WAKEUP_BIT
#define KEYPAD_WAKEUP_BIT           (DKBIT_POWER)
#endif
#ifndef KEYPAD_POLLING_TIME
#define KEYPAD_POLLING_TIME         (50)
#endif
#ifndef KEYPAD_CHECK_TIME
#define KEYPAD_CHECK_TIME           (2)
#endif

//----------------------------------------------------------------------------
// PIGain Customization
//----------------------------------------------------------------------------
#ifndef PIGAIN_MAP_ARRAY
#define PIGAIN_MAP_ARRAY    (NULL)       //PIGain table Modify by W.C Shih
#endif
#ifndef PIGAIN_MAP_SIZE
#define PIGAIN_MAP_SIZE 0
#endif

//----------------------------------------------------------------------------
// VGA Auto Polarity Check Customerization
//----------------------------------------------------------------------------
#ifndef FLAG_VGAPOLARITY_CHECK
#define FLAG_VGAPOLARITY_CHECK 0  // VGA Politary Check Modify by W.C Shih2006/10/30
#endif

#ifndef VGA_AD_SPEC
#define VGA_AD_SPEC    1700 //170MHz
#define VGA_HFH_SPEC   1000 //100KHz
#define VGA_HFL_SPEC   130 //13KHz
#define VGA_VFH_SPEC   86 //86Hz
#define VGA_VFL_SPEC   49 //49Hz
#endif

//----------------------------------------------------------------------------
//SCART RGB Order Customerization
//----------------------------------------------------------------------------
#ifndef SCART_RGB_ORDER
#define SCART_RGB_ORDER (2)
#endif

//----------------------------------------------------------------------------
// SCART Pin8 Threshold Servo ADC Customization
//----------------------------------------------------------------------------
#ifndef SCART_PIN8_LOW_THRES
#define SCART_PIN8_LOW_THRES (11)
#endif

#ifndef SCART_PIN8_HI_THRES
#define SCART_PIN8_HI_THRES (31)
#endif

//----------------------------------------------------------------------------
// SCART FS Status Servo ADC Customization
//----------------------------------------------------------------------------
#ifndef SCART1_FS_SERVO_ADC
#define SCART1_FS_SERVO_ADC     0
#endif

#ifndef SCART2_FS_SERVO_ADC
#define SCART2_FS_SERVO_ADC     0
#endif

//----------------------------------------------------------------------------
// SCART Fast Blanking In Select
//----------------------------------------------------------------------------
#ifndef SCART1_FB_IN_SELECT
#define SCART1_FB_IN_SELECT SCART_FB_IN_SOY1
#endif

#ifndef SCART2_FB_IN_SELECT
#define SCART2_FB_IN_SELECT SCART_FB_IN_NONE
#endif

//----------------------------------------------------------------------------
// Scart Setting
//----------------------------------------------------------------------------
#ifndef SUPPORT_SCART1_TYPE
#define SUPPORT_SCART1_TYPE SCART_FULL_TYPE
#endif

#ifndef SUPPORT_SCART2_TYPE
#define SUPPORT_SCART2_TYPE SCART_HALF_TYPE
#endif

//----------------------------------------------------------------------------
// Scart Setting
//----------------------------------------------------------------------------
#ifndef SCART1_INT_SC_CH
#define SCART1_INT_SC_CH SCART_SC_FROM_CH3
#endif

#ifndef SCART2_INT_SC_CH
#define SCART2_INT_SC_CH SCART_SC_FROM_EXT
#endif

//----------------------------------------------------------------------------
// IRRX customization
//----------------------------------------------------------------------------
#ifndef DEFAULT_IRRX_KEY_UP_TIMEOUT
#define DEFAULT_IRRX_KEY_UP_TIMEOUT         (200)   // ms
#endif
#ifndef DEFAULT_IRRX_REPEAT_VALID_TIME
#define DEFAULT_IRRX_REPEAT_VALID_TIME      (200)   // ms
#endif
#ifndef IRRX_NEC_PULSE1
#define IRRX_NEC_PULSE1             (8)
#endif
#ifndef IRRX_NEC_PULSE2
#define IRRX_NEC_PULSE2             (0)
#endif
#ifndef IRRX_NEC_PULSE3
#define IRRX_NEC_PULSE3             (0)
#endif
#ifndef IRRX_RC5_KEYMAP_FUNC
#define IRRX_RC5_KEYMAP_FUNC            (NULL)
#endif
#ifndef IRRX_RC6_KEYMAP_FUNC
#define IRRX_RC6_KEYMAP_FUNC            (NULL)
#endif
#ifndef IRRX_XFER_USR_TO_CRYSTAL_FUNC
#define IRRX_XFER_USR_TO_CRYSTAL_FUNC   (NULL)
#endif
#ifndef IRRX_SET_USR_WAKEUP_KEY_FUNC
#define IRRX_SET_USR_WAKEUP_KEY_FUNC    (NULL)
#endif
#ifndef IRRX_USR_CONFIG
#define IRRX_USR_CONFIG         0x4121
#endif
#ifndef IRRX_USR_SAPERIOD
#define IRRX_USR_SAPERIOD       0x001A
#endif
#ifndef IRRX_USR_THRESHOLD
#define IRRX_USR_THRESHOLD      0x0001
#endif
#ifndef IRRX_REPEAT_PULSE_UP
#define IRRX_REPEAT_PULSE_UP    5
#endif
#ifndef IRRX_REPEAT_PULSE_DOWN
#define IRRX_REPEAT_PULSE_DOWN  2
#endif
#ifndef IRRX_FLAG_IRRX_BLINKING
#define IRRX_FLAG_IRRX_BLINKING     (0)
#endif
#ifndef IRRC_IS_USR_LONG_PRESS_FUNC
#define IRRC_IS_USR_LONG_PRESS_FUNC   (NULL)
#endif
#ifndef IRRC_CHECK_REPEAT_BY_RAW
#define IRRC_CHECK_REPEAT_BY_RAW  (1)
#endif
#ifndef FLAG_IR_PRGUPDOWN_WAKEUP
#define FLAG_IR_PRGUPDOWN_WAKEUP    (0)
#endif
#ifndef FLAG_IR_ALL_WAKEUP
#define FLAG_IR_ALL_WAKEUP    (0)
#endif
#ifndef FLAG_IR_POWERON_WAKEUP
#define FLAG_IR_POWERON_WAKEUP    (0)
#endif
#ifndef FLAG_IR_USER_DEFINE_WAKEUP
#define FLAG_IR_USER_DEFINE_WAKEUP    (0)
#endif
#ifndef IRRX_SET_USR_NOTIFY_FUNC
#define IRRX_SET_USR_NOTIFY_FUNC        (NULL)
#endif

#ifndef SET_SIGNAL_USR_NOTIFY_FUNC
#define SET_SIGNAL_USR_NOTIFY_FUNC       (NULL)
#endif

#ifndef FLAG_IRRX_SOUND_SUPPORT
#define FLAG_IRRX_SOUND_SUPPORT     (0)
#endif

#ifndef FLAG_IRRX_SOUND_SUPPORT_ALWAYSE_ENABLE
#define FLAG_IRRX_SOUND_SUPPORT_ALWAYSE_ENABLE     (1)
#endif
//----------------------------------------------------------------------------
// SIF customization
//----------------------------------------------------------------------------
#ifndef SIF_ENABLE_SCL_STRETCH
#define SIF_ENABLE_SCL_STRETCH      (0)
#endif
#ifndef SIF_DISABLE_OPEN_DRAIN
#define SIF_DISABLE_OPEN_DRAIN      (0)
#endif
#ifndef SIF_ENABLE_PDWNC_MASTER
#ifdef CC_LGE_PROTO_PCBA
#define SIF_ENABLE_PDWNC_MASTER      (1)
#else
#define SIF_ENABLE_PDWNC_MASTER      (0)
#endif
#endif

#ifndef SIF_ENABLE_PDWNC_MASTER1
#ifdef CC_LGE_PROTO_PCBA
#define SIF_ENABLE_PDWNC_MASTER1      (1)
#else
#define SIF_ENABLE_PDWNC_MASTER1      (0)
#endif
#endif

#ifndef SIF_ENABLE_8295B_MASTER0
#define SIF_ENABLE_8295B_MASTER0        (0)
#endif

#ifndef SIF_ENABLE_8295B_MASTER1
#define SIF_ENABLE_8295B_MASTER1        (0)
#endif

//----------------------------------------------------------------------------
// IC work around flag
//----------------------------------------------------------------------------
#ifndef FLAG_VGA_WAKE_WORKAROUND
#define FLAG_VGA_WAKE_WORKAROUND        (0)
#endif
#ifndef FLAG_CPUPLL_DMPLL_WORKAROUND
#define FLAG_CPUPLL_DMPLL_WORKAROUND    (0)
#endif


//----------------------------------------------------------------------------
// Other default value
//----------------------------------------------------------------------------

#ifndef FLAG_FACTORY_EDID
#define FLAG_FACTORY_EDID           (0)
#endif
#ifndef FLAG_FACOTRY_MODE2_SUPPORT
#define FLAG_FACOTRY_MODE2_SUPPORT  (0)
#endif

#ifndef RELEASE_ENABLE_WATCHDOG
#ifndef NO_REBOOT
#define RELEASE_ENABLE_WATCHDOG     (1)
#else
#define RELEASE_ENABLE_WATCHDOG     (0)
#endif
#endif

#ifndef RELEASE_WATCHDOG_TIMEOUT
#define RELEASE_WATCHDOG_TIMEOUT    (5)
#endif
#ifndef RELEASE_WATCHDOG_PRIORITY
#define RELEASE_WATCHDOG_PRIORITY   (250)//(100)
#endif
#ifndef WATCHDOG_RESET_WAITTIME
#define WATCHDOG_RESET_WAITTIME    (4)	//max is 9.48s
#endif

#ifndef RELEASE_LOADER_ENABLE_WATCHDOG
#define RELEASE_LOADER_ENABLE_WATCHDOG     (0)
#endif

#ifndef RELEASE_LOADER_WATCHDOG_TIMEOUT
#define RELEASE_LOADER_WATCHDOG_TIMEOUT    (10)
#endif


#ifndef SW_POWER_DETECT_VOLTAGE
#define SW_POWER_DETECT_VOLTAGE    (0x0) //0:0V, 0xFF:2.8V
#endif

#ifndef SET_640_480_VIDEO_MODE
#define SET_640_480_VIDEO_MODE     (0)
#endif

#ifndef DISPLAY_OFF_CTRL_BY_AP
#define DISPLAY_OFF_CTRL_BY_AP     (1)
#endif

#ifndef DISPLAY_OFF_CTRL_BY_DRIVER
#define DISPLAY_OFF_CTRL_BY_DRIVER (1)
#endif
//--------------------------------------------------------------------------------------------
//#define SCPOS_SPECIAL_VGA_DISABLE        0
//#define SCPOS_SPECIAL_VGA_USE_DRAM        1    :both Main/Sub go through normal path
//#define SCPOS_SPECIAL_VGA_USE_DISPMODE    2    :main=>display mode, sub=>normal mode
#ifndef SCPOS_SUPPORT_SPECIAL_VGA
#define SCPOS_SUPPORT_SPECIAL_VGA   (0)
#endif

#ifndef CC_MUTE_WHEN_CHANGE_DISP_FMT
#define CC_MUTE_WHEN_CHANGE_DISP_FMT   (0) //mute 15 v sync when change display format, support seamless switch after cobra
#endif
//#ifndef SCPOS_FIXED_NON_LINEAR_FACTOR
//#define SCPOS_FIXED_NON_LINEAR_FACTOR   (0)
//#endif
#ifndef SCPOS_NON_LINEAR_SLOPE
#define SCPOS_NON_LINEAR_SLOPE   (10)
#endif
#ifndef SCPOS_NON_LINEAR_END_POINT_RATIO
#define SCPOS_NON_LINEAR_END_POINT_RATIO   (1024)
#endif

#ifndef VGA_MAX_SUPPORT_PIXEL_RATE
#define VGA_MAX_SUPPORT_PIXEL_RATE  (0xFFFFFFFF)
#endif

#ifndef VGA_EXT_MAX_HSYNC_FREQ
#define VGA_EXT_MAX_HSYNC_FREQ      (0)
#endif

#ifndef VGA_EXT_MIN_HSYNC_FREQ
#define VGA_EXT_MIN_HSYNC_FREQ      (0)
#endif

#ifndef VGA_EXT_MAX_VSYNC_FREQ
#define VGA_EXT_MAX_VSYNC_FREQ      (0)
#endif

#ifndef VGA_EXT_MIN_VSYNC_FREQ
#define VGA_EXT_MIN_VSYNC_FREQ      (0)
#endif

//----------------------------------------------------------------------------
// TVD option flag
//----------------------------------------------------------------------------
#ifndef TVD_ONLY_SUPPORT_NTSC
#define TVD_ONLY_SUPPORT_NTSC (0)
#endif

#ifndef TVD_SUPPORT_PEDESTAL
#define TVD_SUPPORT_PEDESTAL (0)
#endif

#ifndef TVD_PAL_NTSC_MIX_AUTOSEARCH
#define TVD_PAL_NTSC_MIX_AUTOSEARCH (0)
#endif

#ifndef TVD_LLOCK_FREERUN
#define TVD_LLOCK_FREERUN (0)
#endif

#ifndef TVD_CC_NRL_THRESHOLD_BLOCK
#define TVD_CC_NRL_THRESHOLD_BLOCK       (60)
#endif

#ifndef TVD_CC_NRL_THRESHOLD_UNBLOCK
#define TVD_CC_NRL_THRESHOLD_UNBLOCK     (40)
#endif

#ifndef TVD_CC_PER_THRESHOLD_BLOCK
#define TVD_CC_PER_THRESHOLD_BLOCK       (100)
#endif

#ifndef TVD_CC_PER_THRESHOLD_UNBLOCK
#define TVD_CC_PER_THRESHOLD_UNBLOCK     (90)
#endif

#ifndef TVD_SUPPORT_OUTPUT_216
#define TVD_SUPPORT_OUTPUT_216     (0)
#endif

#ifndef TVD_SUPPORT_EFUSE_CVBS_ADC
#define TVD_SUPPORT_EFUSE_CVBS_ADC     (0)
#endif

#ifndef TVD_EFUSE_CVBS_ADC_OFFSET
#define TVD_EFUSE_CVBS_ADC_OFFSET     (0)
#endif

#ifndef TVD_USE_VPRES4
#define TVD_USE_VPRES4     (1)
#endif

#ifndef TVD_SCAN_DIFF_MIN_TH
#define TVD_SCAN_DIFF_MIN_TH     (0x1C0)
#endif

#ifndef TVD_MENU_AV_DELAY_PAL_N
#define TVD_MENU_AV_DELAY_PAL_N     (0xBC)
#endif

#ifndef TVD_MENU_AV_DELAY_PAL
#define TVD_MENU_AV_DELAY_PAL     (0xB0)
#endif

#ifndef TVD_MENU_AV_DELAY_PAL_M
#define TVD_MENU_AV_DELAY_PAL_M     (0xB1)
#endif

#ifndef TVD_MENU_AV_DELAY_NTSC358
#define TVD_MENU_AV_DELAY_NTSC358     (0xB2)
#endif

#ifndef TVD_MENU_AV_DELAY_SECAM
#define TVD_MENU_AV_DELAY_SECAM     (0xA0)
#endif

#ifndef TVD_MENU_AV_DELAY_PAL_60
#define TVD_MENU_AV_DELAY_PAL_60     (0xA6)
#endif

#ifndef TVD_MENU_AV_DELAY_NTSC443
#define TVD_MENU_AV_DELAY_NTSC443     (0xA6)
#endif

#ifndef TVD_MENU_AV_DELAY_SV_CS_UNSTABLE
#define TVD_MENU_AV_DELAY_SV_CS_UNSTABLE     (0xA6)
#endif

#ifndef TVD_MENU_AV_DELAY_SHNA_PAL_N
#define TVD_MENU_AV_DELAY_SHNA_PAL_N     (0xC7)
#endif

#ifndef TVD_MENU_AV_DELAY_SHNA_PAL
#define TVD_MENU_AV_DELAY_SHNA_PAL     (0xB7)
#endif

#ifndef TVD_MENU_AV_DELAY_SHNA_PAL_M
#define TVD_MENU_AV_DELAY_SHNA_PAL_M     (0xB2)
#endif

#ifndef TVD_MENU_AV_DELAY_SHNA_NTSC358
#define TVD_MENU_AV_DELAY_SHNA_NTSC358     (0xB9)
#endif

#ifndef TVD_MENU_AV_DELAY_SHNA_SECAM
#define TVD_MENU_AV_DELAY_SHNA_SECAM     (0xA3)
#endif

#ifndef TVD_MENU_AV_DELAY_SHNA_PAL_60
#define TVD_MENU_AV_DELAY_SHNA_PAL_60     (0xAD)
#endif

#ifndef TVD_MENU_AV_DELAY_SHNA_NTSC443
#define TVD_MENU_AV_DELAY_SHNA_NTSC443     (0xA7)
#endif

#ifndef TVD_MENU_AV_DELAY_SHNA_SV_CS_UNSTABLE
#define TVD_MENU_AV_DELAY_SHNA_SV_CS_UNSTABLE     (0xB0)
#endif

#ifndef TVD_MENU_AV_V_DELAY
#define TVD_MENU_AV_V_DELAY     (0x0)
#endif

#ifndef TVD_TGT_SYNC_HEIGHT_625
#define TVD_TGT_SYNC_HEIGHT_625     (0x3A0)
#endif

#ifndef TVD_TGT_SYNC_HEIGHT_525
#define TVD_TGT_SYNC_HEIGHT_525     (0x380)
#endif

#ifndef TVD_PAL_IP_TGT_A
#define TVD_PAL_IP_TGT_A     (0x6E)
#endif

#ifndef TVD_PAL_IP_TGT_B
#define TVD_PAL_IP_TGT_B     (0x66)
#endif

#ifndef TVD_NTSC_IP_TGT_A
#define TVD_NTSC_IP_TGT_A     (0x73)
#endif

#ifndef TVD_NTSC_IP_TGT_B
#define TVD_NTSC_IP_TGT_B     (0x6B)
#endif

#ifndef TVD_NTSC_J_IP_TGT_A
#define TVD_NTSC_J_IP_TGT_A     (0x73)
#endif

#ifndef TVD_NTSC_J_IP_TGT_B
#define TVD_NTSC_J_IP_TGT_B     (0x6B)
#endif

#ifndef TVD_SCM_IP_TGT_A
#define TVD_SCM_IP_TGT_A     (0x3A)
#endif

#ifndef TVD_SCM_IP_TGT_B
#define TVD_SCM_IP_TGT_B     (0x38)
#endif

#ifndef TVD_BSTLOCK_THR1_LN_525
#define TVD_BSTLOCK_THR1_LN_525     (0x1A0) //Low NA burst lock --> unlock
#endif

#ifndef TVD_BSTLOCK_THR2_LN_525
#define TVD_BSTLOCK_THR2_LN_525     (0x13A) //Low NA burst unlock --> lock
#endif

#ifndef TVD_BSTLOCK_THR1_HN_525
#define TVD_BSTLOCK_THR1_HN_525     (0x160) //High&Super NA burst lock --> unlock
#endif

#ifndef TVD_BSTLOCK_THR2_HN_525
#define TVD_BSTLOCK_THR2_HN_525     (0x150) //High&Super NA burst unlock --> lock
#endif

#ifndef TVD_BSTLOCK_THR1_LN_625
#define TVD_BSTLOCK_THR1_LN_625     (0x1A0) //Low NA burst lock --> unlock
#endif

#ifndef TVD_BSTLOCK_THR2_LN_625
#define TVD_BSTLOCK_THR2_LN_625     (0x15A) //Low NA burst unlock --> lock
#endif

#ifndef TVD_BSTLOCK_THR1_HN_625
#define TVD_BSTLOCK_THR1_HN_625     (0x1d0) //High&Super NA burst lock --> unlock
#endif

#ifndef TVD_BSTLOCK_THR2_HN_625
#define TVD_BSTLOCK_THR2_HN_625     (0x1c0) //High&Super NA burst unlock --> lock
#endif

#ifndef TVD_BSTLOCK_THR1_PAL_N
#define TVD_BSTLOCK_THR1_PAL_N     (0x150) //High&Super NA burst lock --> unlock
#endif

#ifndef TVD_BSTLOCK_THR2_PAL_N
#define TVD_BSTLOCK_THR2_PAL_N     (0x120) //High&Super NA burst unlock --> lock
#endif

#ifndef TVD_SCKILL_UP
#define TVD_SCKILL_UP     (0x3) //Secam CKILL UP TH
#endif

#ifndef TVD_SCKILL_DN
#define TVD_SCKILL_DN     (0x1) //Secam CKILL DN TH
#endif
//----------------------------------------------------------------------------
// CEC option flag
//----------------------------------------------------------------------------
#ifndef CEC_FUNCTON_PINMUX_EN
#define CEC_FUNCTON_PINMUX_EN (1)
#endif

//----------------------------------------------------------------------------
// HDMI option flag
//----------------------------------------------------------------------------
#ifndef HDMI_SUPPORT_EXT_SWITCH
#define HDMI_SUPPORT_EXT_SWITCH (0)
#endif
#ifndef HDMI_BYPASS_INITIAL_FLOW
#define HDMI_BYPASS_INITIAL_FLOW (1)
#endif
#ifndef HDMI_OFFON_MUTE_COUNT
#define HDMI_OFFON_MUTE_COUNT (400)    //ms
#endif
#ifndef DVI_WAIT_STABLE_COUNT
#define DVI_WAIT_STABLE_COUNT (120)
#endif
#ifndef DVI_WAIT_NOSIGNAL_COUNT
#define DVI_WAIT_NOSIGNAL_COUNT (12)    //second
#endif
#ifndef HDMI_WAIT_SCDT_STABLE_COUNT
#define HDMI_WAIT_SCDT_STABLE_COUNT (1)    //second
#endif
#ifndef HDMI_TMDS_EQ_ZERO_VALUE  // josh
#define HDMI_TMDS_EQ_ZERO_VALUE (0x1)
#endif
#ifndef HDMI_TMDS_EQ_BOOST_VALUE
#define HDMI_TMDS_EQ_BOOST_VALUE (0xd)
#endif
#ifndef HDMI_TMDS_EQ_SEL_VALUE
#define HDMI_TMDS_EQ_SEL_VALUE (0xd) //default
#endif
#ifndef HDMI_TMDS_EQ_GAIN_VALUE
#define HDMI_TMDS_EQ_GAIN_VALUE (0x2)
#endif
#ifndef HDMI_TMDS_HDMI_LBW_VALUE
#define HDMI_TMDS_HDMI_LBW_VALUE (0x2)
#endif

#ifndef HDMI_HDCP_Mask1
#define HDMI_HDCP_Mask1 (0xff)
#endif

#ifndef HDMI_HDCP_Mask2
#define HDMI_HDCP_Mask2 (0xc3)
#endif
#ifndef UI_HDMI_PORT_MAPPING_HDMI1 //fll
#define UI_HDMI_PORT_MAPPING_HDMI1 1
#endif

#ifndef UI_HDMI_PORT_MAPPING_HDMI2 //fll
#define UI_HDMI_PORT_MAPPING_HDMI2 2
#endif

#ifndef UI_HDMI_PORT_MAPPING_HDMI3 //fll
#define UI_HDMI_PORT_MAPPING_HDMI3 3
#endif

#ifndef UI_HDMI_PORT_MAPPING_HDMI4 //fll
#define UI_HDMI_PORT_MAPPING_HDMI4 4
#endif

// EQ settings for 89/96/corba
// EQ setting for clcok rate low1
#ifndef HDMI_EQ_LOW1_KPGAIN
#define HDMI_EQ_LOW1_KPGAIN (0x3)
#endif
#ifndef HDMI_EQ_LOW1_RXMODE
#define HDMI_EQ_LOW1_RXMODE (0x3)
#endif
#ifndef HDMI_EQ_LOW1_EQGAIN
#define HDMI_EQ_LOW1_EQGAIN (0x3)
#endif
#ifndef HDMI_EQ_LOW1_ZSEL
#define HDMI_EQ_LOW1_ZSEL (0x0)
#endif
#ifndef HDMI_EQ_LOW1_VREF
#define HDMI_EQ_LOW1_VREF (0x2)
#endif
// EQ setting for clcok rate low2
#ifndef HDMI_EQ_LOW2_KPGAIN
#define HDMI_EQ_LOW2_KPGAIN (0x3)
#endif
#ifndef HDMI_EQ_LOW2_RXMODE
#define HDMI_EQ_LOW2_RXMODE (0x3)
#endif
#ifndef HDMI_EQ_LOW2_EQGAIN
#define HDMI_EQ_LOW2_EQGAIN (0x3)
#endif
#ifndef HDMI_EQ_LOW2_ZSEL
#define HDMI_EQ_LOW2_ZSEL (0x0)
#endif
#ifndef HDMI_EQ_LOW2_VREF
#define HDMI_EQ_LOW2_VREF (0x2)
#endif
// EQ setting for clcok rate middle
#ifndef HDMI_EQ_MIDDLE_KPGAIN
#define HDMI_EQ_MIDDLE_KPGAIN (0x3)
#endif
#ifndef HDMI_EQ_MIDDLE_RXMODE
#define HDMI_EQ_MIDDLE_RXMODE (0x3)
#endif
#ifndef HDMI_EQ_MIDDLE_EQGAIN
#define HDMI_EQ_MIDDLE_EQGAIN (0x3)
#endif
#ifndef HDMI_EQ_MIDDLE_ZSEL
#define HDMI_EQ_MIDDLE_ZSEL (0x0)
#endif
#ifndef HDMI_EQ_MIDDLE_VREF
#define HDMI_EQ_MIDDLE_VREF (0x2)
#endif
// EQ setting for clcok rate high
#ifndef HDMI_EQ_HIGH_KPGAIN
#define HDMI_EQ_HIGH_KPGAIN (0x3)
#endif
#ifndef HDMI_EQ_HIGH_RXMODE
#define HDMI_EQ_HIGH_RXMODE (0x1)
#endif
#ifndef HDMI_EQ_HIGH_EQGAIN
#define HDMI_EQ_HIGH_EQGAIN (0x3)
#endif
#ifndef HDMI_EQ_HIGH_ZSEL
#define HDMI_EQ_HIGH_ZSEL (0x0)
#endif
#ifndef HDMI_EQ_HIGH_VREF
#define HDMI_EQ_HIGH_VREF (0x2)
#endif

//----------------------------------------------------------------------------
// RTC HW option flag
//----------------------------------------------------------------------------
#ifndef RTC_HW
#define RTC_HW        (RTC_INTERNAL)
#endif
#ifndef RTC_EXT_CLKDIV
#define RTC_EXT_CLKDIV 0x100
#endif

//----------------------------------------------------------------------------
// PQ Related Settings
//----------------------------------------------------------------------------
#ifndef SUPPORT_FLASH_PQ
    #define SUPPORT_FLASH_PQ         (0)
#endif
#ifndef FLASH_PQ_USE_NAND_FLASH
    #ifdef CC_NAND_ENABLE
        #define FLASH_PQ_USE_NAND_FLASH         (1)
    #else
        #define FLASH_PQ_USE_NAND_FLASH         (0)
    #endif
#endif
#ifndef FLASH_PQ_BASE_PARTITION
    #ifdef LINUX_TURNKEY_SOLUTION
        #define FLASH_PQ_BASE_PARTITION 			(15) // Reference to NAND_PART_SIZExxx
    #else
        #define FLASH_PQ_BASE_PARTITION             (7) // Reference to NAND_PART_SIZExxx
#endif
#endif
#ifndef FLASH_PQ_BASE_ADDRESS
    #define FLASH_PQ_BASE_ADDRESS           (0x0)
#endif
#ifndef FLASH_PQ_BASE_OFFSET
    #define FLASH_PQ_BASE_OFFSET                (0x0)
#endif
#ifndef FLASH_PQ_BLOCK_SIZE
    #define FLASH_PQ_BLOCK_SIZE                 (1)    // Unit: 64KB.
#endif
#ifndef SUPPORT_FLASH_PQ_ENABLE_GAMMA
    #define SUPPORT_FLASH_PQ_ENABLE_GAMMA           (1)
#endif
#ifndef SUPPORT_FLASH_PQ_ENABLE_SCE
    #define SUPPORT_FLASH_PQ_ENABLE_SCE             (1)
#endif
#ifndef SUPPORT_FLASH_PQ_ENABLE_QTY
    #define SUPPORT_FLASH_PQ_ENABLE_QTY             (1)
#endif
#ifndef FLASH_PQ_SCE_MIN
    #define FLASH_PQ_SCE_MIN                    (0)
#endif
#ifndef SUPPORT_FLASH_PQ_ENABLE_QTY_MAX_MIN
    #define SUPPORT_FLASH_PQ_ENABLE_QTY_MAX_MIN         (0)
#endif
#ifndef SUPPORT_FLASH_PQ_ENABLE_QTY_SMART_PIC
        #define SUPPORT_FLASH_PQ_ENABLE_QTY_SMART_PIC           (0)
#endif
#ifndef SUPPORT_FLASH_PQ_SMART_PIC_NEW_MODE
	#define SUPPORT_FLASH_PQ_SMART_PIC_NEW_MODE 		(0)
#endif
#ifndef FLASH_PQ_FLASH_PANEL
	#define FLASH_PQ_FLASH_PANEL                       (1)
#endif

//------------------------------------------------------------
#ifdef SUPPORT_2ND_FLASHPQ
#ifndef FLASH_PQ_2ND_PART_BASE_ADDRESS
    #define FLASH_PQ_2ND_PART_BASE_ADDRESS           (0x0)
#endif
#ifndef FLASH_PQ_2ND_PART_BASE_OFFSET
    #define FLASH_PQ_2ND_PART_BASE_OFFSET                (0x0)//(0x20000)//(0x0)
#endif
#ifndef FLASH_PQ_2ND_PART_BLOCK_SIZE
    #define FLASH_PQ_2ND_PART_BLOCK_SIZE                 (1)    // Unit: 64KB.
#endif
#ifndef FLASH_PQ_2ND_PART_ENABLE_GAMMA
    #define FLASH_PQ_2ND_PART_ENABLE_GAMMA               (1)
#endif
#ifndef FLASH_PQ_2ND_PART_ENABLE_SCE
    #define FLASH_PQ_2ND_PART_ENABLE_SCE                 (1)
#endif
#ifndef SUPPORT_FLASH_PQ_2ND_PART_ENABLE_QTY
    #define SUPPORT_FLASH_PQ_2ND_PART_ENABLE_QTY             (1)
#endif
#ifndef SUPPORT_FLASH_PQ_2ND_PART_ENABLE_QTY_MAX_MIN
    #define SUPPORT_FLASH_PQ_2ND_PART_ENABLE_QTY_MAX_MIN         (0)
#endif
#ifndef SUPPORT_FLASH_PQ_2ND_PART_ENABLE_QTY_SMART_PIC
    #define SUPPORT_FLASH_PQ_2ND_PART_ENABLE_QTY_SMART_PIC           (0)
#endif
#ifndef SUPPORT_FLASH_PQ_2ND_PART_ENABLE_QTY_SMART_PIC_NEW_MODE
    #define SUPPORT_FLASH_PQ_2ND_PART_ENABLE_QTY_SMART_PIC_NEW_MODE   (0)
#endif
#endif
//------------------------------------------------------------


#ifndef BIRGHT_INTERVAL
    #define BIRGHT_INTERVAL         (2)  // SCE Register Value = 0x400 + (Dummy - 0x80) << (BIRGHT_INTERVAL)
#endif
#ifndef HUE_INTERVAL
    #define HUE_INTERVAL            (1)  // SCE Register Value = 0x200 + (Dummy - 0x80) << (HUE_INTERVAL)
#endif
#ifndef BIND_CONTRAST_SATURATION
    #define BIND_CONTRAST_SATURATION            (1)
#endif
#ifndef USE_MATRIX_PQ_ADJ
    #define USE_MATRIX_PQ_ADJ           (1)
#endif
#ifndef OS_OPTION_POS
    #define OS_OPTION_POS                   (0)  // Mustang:0:before mixer, 1:between mixer, 2:after mixer, 3:after OD
#endif
#ifndef XVYCC_CLIP_ENABLE
    #define XVYCC_CLIP_ENABLE            (1)
#endif
#ifndef MATRIX_HD_YCBCR709
    #define MATRIX_HD_YCBCR709          (0)
#endif
#ifndef SUPPORT_HDMI_YCBCR444
    #define SUPPORT_HDMI_YCBCR444       (0)
#endif
#ifndef SUPPORT_VGA_YCBCR444
    #define SUPPORT_VGA_YCBCR444        (0)
#endif
#ifndef SHARE_BNR_HW_CFG
    #define SHARE_BNR_HW_CFG            (1)
#endif

//----------------------------------------------------------------------------
// MT5396 SPI pinmux select
//----------------------------------------------------------------------------
#ifndef LDM_PINMUX_SEL
      #define LDM_PINMUX_SEL   (0)
#endif


//----------------------------------------------------------------------------
// Default TVE
//----------------------------------------------------------------------------
#ifndef TVE_SUPPORT_AFD
#define TVE_SUPPORT_AFD (1)
#endif

//----------------------------------------------------------------------------
// Default UART
//----------------------------------------------------------------------------
#ifndef UART1_SUPPORT_ENABLE
#define UART1_SUPPORT_ENABLE    0
#endif

#ifndef UART2_SUPPORT_ENABLE
#define UART2_SUPPORT_ENABLE    0
#endif

//----------------------------------------------------------------------------
// MT8292 GPIO support
//----------------------------------------------------------------------------
#ifndef FLAG_MT8292_GPIO_SUPPORT
#define FLAG_MT8292_GPIO_SUPPORT 0
#endif
#ifndef MT8292_RESET_GPIO
#define MT8292_RESET_GPIO (0xFFFFFFFF)
#endif
#ifndef MT8292_MUTE_GPIO
#define MT8292_MUTE_GPIO (0xFFFFFFFF)
#endif
#ifndef MT8280_RESET_GPIO
#define MT8280_RESET_GPIO (0xFFFF)
#endif
#ifndef MT8280_RESET_GPIO_POLARITY
#define MT8280_RESET_GPIO_POLARITY (0)
#endif
#ifndef MT8280_CMD_ACK_GPIO
#define MT8280_CMD_ACK_GPIO  (0xFFFFFFFF)
#endif
#ifndef MT8280_CMD_ACK_GPIO_POLARITY
#define MT8280_CMD_ACK_GPIO_POLARITY (1)
#endif
#ifndef MT8280_SEND_PANEL
#define MT8280_SEND_PANEL   (0)
#endif

//----------------------------------------------------------------------------
// MT8295 GPIO support
//----------------------------------------------------------------------------
// #ifndef CI_8295_RESET_GPIO
// #define CI_8295_RESET_GPIO  OPCTRL(5)
// #endif
#ifndef CI_PMX_CFG
#define CI_PMX_CFG      (mt5360_NAND_PMX)
#endif
#ifndef CI_5V_POWERON_POLARITY
#define CI_5V_POWERON_POLARITY      (1)
#endif
#ifndef CI_5V_POWERON_POLARITY
#define CI_5V_POWERON_POLARITY      (1)
#endif
#ifndef CI_EXTERNAL_IC_DRIVING_CURRENT
#define CI_EXTERNAL_IC_DRIVING_CURRENT      (8) // 4mA
#endif
#ifndef CI_TS_DRIVING_CURRENT
#define CI_TS_DRIVING_CURRENT        (6) // 6mA
#endif
#ifndef CI_PBI_DRIVING_CURRENT
#define CI_PBI_DRIVING_CURRENT       (6) // 6mA
#endif
#ifndef CI_CUSTOM_QUERY_FUNC
#define CI_CUSTOM_QUERY_FUNC  (NULL)
#endif

//----------------------------------------------------------------------------
// Smart card support
//----------------------------------------------------------------------------
#ifndef SMC_HOTPLUG_GPIO
#define SMC_HOTPLUG_GPIO GPIO(72)
#endif
#ifndef SMC_HOTPLUG_GPIO_POLARITY
#define SMC_HOTPLUG_GPIO_POLARITY (0)
#endif

//-----------------------------------------------------------------------------
// Notification Function
//-----------------------------------------------------------------------------
#ifndef NOTIFY_MUTE_FUNC
#define NOTIFY_MUTE_FUNC (NULL)
#endif
#ifndef NOTIFY_MIXER_VIDEOLAYER_FUNC
#define NOTIFY_MIXER_VIDEOLAYER_FUNC (NULL)
#endif
#ifndef NOTIFY_MIXER_FUNC
#define NOTIFY_MIXER_FUNC (NULL)
#endif
#ifndef NOTIFY_VIDEO_FUNC
#define NOTIFY_VIDEO_FUNC (NULL)
#endif
#ifndef NOTIFY_VIDEO_AUTO_DONE_FUNC
#define NOTIFY_VIDEO_AUTO_DONE_FUNC (NULL)
#endif

#endif /* DEFAULT_H */

//----------------------------------------------------------------------------
// ON_CHIP_AUTO_COLOR support
//------------------------------------------------------------------------------
#ifndef ON_CHIP_AUTO_COLOR
#define ON_CHIP_AUTO_COLOR    0   // 1:enable  0:disable
#endif
#ifndef MTK_PUBLIC_VERSION_ON_CHIP_AUTO_COLOR
#define MTK_PUBLIC_VERSION_ON_CHIP_AUTO_COLOR  1
#endif

#ifndef VFE_HW_SPECIAL_TARGET
#define VFE_HW_SPECIAL_TARGET  0   // 1 : SPECIAL_TARGET
#endif

#ifndef VFE_HW_YPbPrMapToVGA
#define VFE_HW_YPbPrMapToVGA  0   // 1 : YPbPr map to VGA
#endif

#ifndef VFE_HW_075
#define VFE_HW_075  0   // 1 : HW is 0,75 ohm;  0:H/W is 18,56 ohm. mt5362 default is 18,56 ohm
#endif

#ifndef YPBPR_ADC_SUPPORT_120
#define YPBPR_ADC_SUPPORT_120  0   // 1 : support none standard 120IRE;  0: not support 120%
#endif

#ifndef DoAutoPhaseAfterAutoColor
#define DoAutoPhaseAfterAutoColor  0   // 1 : support none standard 120IRE;  0: not support 120%
#endif

#ifndef SCART_DO_AUTOCOLOR             //MC20081115 add for SCART ***
#define SCART_DO_AUTOCOLOR 0
#endif                                                   //MC20081115 add for SCART &&&

#ifndef EFUSE_AUTOCOLOR
#define EFUSE_AUTOCOLOR 1  // 1 :Use E Fuse Autocolor
#endif

#ifndef AUTOCOLOR_FAIL_SHOW_PATTERN
#define AUTOCOLOR_FAIL_SHOW_PATTERN 0
#endif

#ifdef CC_YPBPR_ADAPTIVE_SLICER_REDUCE_TIME
		#ifndef YDetect_GPIO
		#define YDetect_GPIO GPIO(205)  //must set by PCB, a1---209 ,a2---205
		#endif
#endif


//----------------------------------------------------------------------------
// Linux memory usage related customization
//------------------------------------------------------------------------------
#ifndef LINUX_KERNEL_MEM_SIZE
#define LINUX_KERNEL_MEM_SIZE 54 * 1024 * 1024
#endif
#ifndef TOTAL_MEM_SIZE
#define TOTAL_MEM_SIZE 128 * 1024 * 1024
#endif
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

#ifndef FB_MEM_SIZE
#define FB_MEM_SIZE 4 * 1024 * 1024
#endif
#ifndef DIRECT_FB_MEM_SIZE
#define DIRECT_FB_MEM_SIZE 8 * 1024 * 1024
#endif

//#define YPBPR_COUNTER_ADDR    0   // define the EEPROM absolute address for the counter of YPBPR online offset calibration
//#define VGA_COUNTER_ADDR      0   // define the EEPROM absolute address for the counter of VGA online offset calibration
//#define OFFSET_CALI_COUNTER   0xff  //0xff means ignore the above two counter

//move to custom config file for next IC
#ifndef CUSTOM_EEPROM_FILE
#define CUSTOM_EEPROM_FILE                    "mtk/mtk_cust_eeprom.h"
#endif

//----------------------------------------------------------------------------
// DMX related customization
//------------------------------------------------------------------------------
#ifndef DMX_FRAMER_SET0_POSITIVE_EDGE
#define DMX_FRAMER_SET0_POSITIVE_EDGE       (1)     //0: negative 1: positive
#endif

#ifndef DMX_FRAMER_SET0_EXTERNAL_SYNC
#define DMX_FRAMER_SET0_EXTERNAL_SYNC       (0)     //0: internal 1: external
#endif

#ifndef DMX_FRAMER_SET0_PARALLEL_INPUT
#define DMX_FRAMER_SET0_PARALLEL_INPUT      (0)     //0: serial 1: parallel
#endif

#ifndef DMX_FRAMER_SET1_POSITIVE_EDGE
#define DMX_FRAMER_SET1_POSITIVE_EDGE       (1)     //0: negative 1: positive
#endif

#ifndef DMX_FRAMER_SET1_EXTERNAL_SYNC
#define DMX_FRAMER_SET1_EXTERNAL_SYNC       (0)     //0: internal 1: external
#endif

#ifndef DMX_FRAMER_SET1_PARALLEL_INPUT
#define DMX_FRAMER_SET1_PARALLEL_INPUT      (0)     //0: serial 1: parallel
#endif

#ifndef DMX_FRONTEND_CONFIG_TABLE
#define DMX_FRONTEND_CONFIG_TABLE           (&_arMtkFeCfgTable)
#endif

//----------------------------------------------------------------------------
// Tool License Check and Password Protection
//------------------------------------------------------------------------------
#ifndef CUSTOMER_NAME
#define CUSTOMER_NAME        "mtk"
#endif

#ifndef CUSTOMER_PASSWORD
#define CUSTOMER_PASSWORD    ""
#endif

//----------------------------------------------------------------------------
// Trustzone
//------------------------------------------------------------------------------
#ifndef TRUSTZONE_CODE_BASE
#define TRUSTZONE_CODE_BASE     (0x5000)
#endif

#ifndef TRUSTZONE_MEM_SIZE
#define TRUSTZONE_MEM_SIZE      (0x0)
#endif

#ifndef ETHDES_MEM_SIZE
#define ETHDES_MEM_SIZE      (0x0)
#endif
#ifndef TRUSTZONE_FLASH_PARTITION
#define TRUSTZONE_FLASH_PARTITION    (-1)
#endif
