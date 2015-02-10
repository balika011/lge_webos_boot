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
#ifndef MTK_CUST_EEPROM_H
#define MTK_CUST_EEPROM_H
//#include "drv_default.h"
enum
{
	DRV_CUSTOM_EEPROM_START= DRVCUST_EEPROM_CUSTOM_DRIVER_OFFSET,
	EEP_PANEL_INDEX_OFFSET1= DRV_CUSTOM_EEPROM_START,
	EEP_PANEL_INDEX_OFFSET2,
	EEP_PANEL_INDEX_OFFSET3,
	EEP_PANEL_AUTO_DETECT,
	EEP_COUNTRY_OFFSET1,
	EEP_COUNTRY_OFFSET2,
	EEP_COUNTRY_OFFSET3,
	EEP_TUNER_TYPE_0,
	EEP_TUNER_TYPE_1,
	EEP_TUNER_TYPE_2,
	EEP_CEC_MENU_LANG_START,
	EEP_CEC_MENU_LANG_END= EEP_CEC_MENU_LANG_START + 4 -1 ,
	EEP_CEC_ENABLE,
	EEP_CEC_ONE_TOUCH_PLAY_ENABLE,
	EEP_HDMI_EDID_INDEX_OFFSET,
	EEP_VGA_EDID_INDEX_OFFSET,
	EEP_BRDCAST_SOURCE_OFFSET,                //Mingsheng 121009 add for demod warm/cool bootup refine 
	EEP_MHL_RCP_RECEIVE_ENABLE,//eggy add for cbus rcp receive
	EEP_DEMOD_TYPE, //wenming 131111 add for demod type selection
	
	EEP_HOTKEY_WAKEUP_OFFSET = HOTKEY_WAKEUP_EEP_OFFSET,
	EEP_HOTKEY_WAKEUP_NUM = DRVCUST_EEPROM_CUSTOM_DRIVER_OFFSET + DRVCUST_EEPROM_CUSTOM_DRIVER_SIZE -1,
	DRV_CUSTOM_EEPROM_END = EEP_HOTKEY_WAKEUP_NUM 
	//don't add the eeprom here,please add before EEP_HOTKEY_WAKEUP_OFFSET
};
#ifndef PANEL_INDEX_OFFSET1
#define PANEL_INDEX_OFFSET1         EEP_PANEL_INDEX_OFFSET1
#endif

#ifndef PANEL_INDEX_OFFSET2
#define PANEL_INDEX_OFFSET2         EEP_PANEL_INDEX_OFFSET2
#endif

#ifndef PANEL_INDEX_OFFSET3
#define PANEL_INDEX_OFFSET3         EEP_PANEL_INDEX_OFFSET3
#endif

#ifndef PANEL_AUTO_DETECT_OFFSET
#define PANEL_AUTO_DETECT_OFFSET    EEP_PANEL_AUTO_DETECT
#endif

#ifndef EEPROM_COUNTRY_OFFSET1
#define EEPROM_COUNTRY_OFFSET1      EEP_COUNTRY_OFFSET1
#endif

#ifndef EEPROM_COUNTRY_OFFSET2
#define EEPROM_COUNTRY_OFFSET2      EEP_COUNTRY_OFFSET2
#endif

#ifndef EEPROM_COUNTRY_OFFSET3
#define EEPROM_COUNTRY_OFFSET3      EEP_COUNTRY_OFFSET3
#endif

#ifndef DEFAULT_DEMOD_TYPE_OFST
#define DEFAULT_DEMOD_TYPE_OFST    EEP_DEMOD_TYPE
#endif

#ifndef DEFAULT_TUNER_TYPE_OFST0
#define DEFAULT_TUNER_TYPE_OFST0    EEP_TUNER_TYPE_0
#endif
#ifndef DEFAULT_TUNER_TYPE_OFST1
#define DEFAULT_TUNER_TYPE_OFST1    EEP_TUNER_TYPE_1
#endif
#ifndef DEFAULT_TUNER_TYPE_OFST2
#define DEFAULT_TUNER_TYPE_OFST2    EEP_TUNER_TYPE_2
#endif

#ifndef EEPROM_CEC_MENU_LANGUAGE_OFFSET
#define EEPROM_CEC_MENU_LANGUAGE_OFFSET         (EEP_CEC_MENU_LANG_START)
#endif

#ifndef EEPROM_CEC_ENABLE_OFFSET 
#define EEPROM_CEC_ENABLE_OFFSET              (EEP_CEC_ENABLE)
#endif
#ifndef EEPROM_CEC_ONE_TOUCH_PLAY_ENABLE_OFFSET 
#define EEPROM_CEC_ONE_TOUCH_PLAY_ENABLE_OFFSET (EEP_CEC_ONE_TOUCH_PLAY_ENABLE)
#endif

#ifndef EEPROM_HDMI_EDID_INDEX_OFFSET
#define EEPROM_HDMI_EDID_INDEX_OFFSET     (EEP_HDMI_EDID_INDEX_OFFSET)
#endif

#ifndef EEPROM_VGA_EDID_INDEX_OFFSET
#define EEPROM_VGA_EDID_INDEX_OFFSET     (EEP_VGA_EDID_INDEX_OFFSET)
#endif

#ifndef EEPROM_BRDCAST_SOURCE_OFFSET1
#define EEPROM_BRDCAST_SOURCE_OFFSET1      EEP_BRDCAST_SOURCE_OFFSET
#endif

#ifndef EEPROM_MHL_RCP_RECEIVE_OFFSET
#define EEPROM_MHL_RCP_RECEIVE_OFFSET EEP_MHL_RCP_RECEIVE_ENABLE
#endif


#endif /*MTK_CUST_EEPROM_H */

