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
 * $Author: p4admin $
 * $Date: 2015/02/10 $
 * $RCSfile: panel_table.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file panel_tablc.c
 *  This file includes panel timing configuration.
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#ifndef CC_MTK_PRELOADER

#include "panel.h"
#include "eeprom_if.h"
#include "drvcust_if.h"
#include "drv_od.h"
#include "nand_if.h"
#include "x_lint.h"
LINT_EXT_HEADER_BEGIN
#include "x_typedef.h"
#include "x_printf.h"
#include "x_pinmux.h"
#include "x_hal_5381.h"
#include "x_assert.h"
#include "x_os.h"
LINT_EXT_HEADER_END


#ifdef CUSTOM_CFG_FILE
#include CUSTOM_CFG_FILE
#endif /* CUSTOM_CFG_FILE */

#include "drv_default.h"
//#ifdef SUPPORT_PANEL_CUSTOMER_SPEC
#include"panel_cust.h"
//#endif

#ifdef CUSTOM_EEPROM_FILE
#include CUSTOM_EEPROM_FILE
#endif

#ifdef CC_ENABLE_MTK_MODEL_INDEX
#include "modelIndex.h"
#endif
#ifdef CC_LGE_PROTO_PCBA
#ifdef CC_UBOOT
typedef enum
{
	MODULE_LGD = 0,
	MODULE_CMI,
	MODULE_AUO,
	MODULE_SHARP,
	MODULE_IPS,
	MODULE_BOE,
	MODULE_CSOT,
	MODULE_INX,
	MODULE_LCD_END,

	MODULE_LGE = MODULE_LCD_END,
	MODULE_PANASONIC,
	MODULE_PDP_END,
	MODULE_BASE	= MODULE_PDP_END
} MODULE_MAKER_TYPE_T;



 typedef enum
{
	MODELOPT_PANEL_INTERFACE_EPI	= 0,
	MODELOPT_PANEL_INTERFACE_LVDS	= 1,
	MODELOPT_PANEL_INTERFACE_VBYONE = 2,
	MODELOPT_PANEL_INTERFACE_MAX	= 3
}MODELOPT_PANEL_INTERFACE_T;



typedef enum
{
	MODELOPT_PANEL_RESOLUTION_HD	= 0,
	MODELOPT_PANEL_RESOLUTION_FHD	= 1,
	MODELOPT_PANEL_RESOLUTION_UD	= 2,
	MODELOPT_PANEL_RESOLUTION_MAX	= 3
}MODELOPT_PANEL_RESOLUTION_T;

typedef struct panelinfo_t
{
	MODELOPT_PANEL_RESOLUTION_T panel_resolution;
	MODELOPT_PANEL_INTERFACE_T	panel_interface;
	unsigned char				bSupport_frc;
	MODULE_MAKER_TYPE_T			eModelModuleType;
	unsigned short				nLVDSBit;
} PANEL_INFO_T;

extern PANEL_INFO_T gPanelInfo;

#endif
#define NOT_USE_EEP
#endif
//---------------------------------------------------------------------------
// Type definitions
//---------------------------------------------------------------------------

typedef struct
{
    UINT16 u2PanelWidth;
    UINT16 u2PanelHeight;

    // Clk60Hz = htotal*vtotal*60/1.001
    // Clk50Hz = htotal*vtotal*50
    UINT32 u4PixelClkMax;
    UINT32 u4PixelClk60Hz;
    UINT32 u4PixelClk50Hz;
    UINT32 u4PixelClkMin;

    // HTotal = spec value
    UINT16 u2HTotalMax;
    UINT16 u2HTotal60Hz;
    UINT16 u2HTotal50Hz;
    UINT16 u2HTotalMin;

    // VTotal = spec value
    UINT16 u2VTotalMax;
    UINT16 u2VTotal60Hz;
    UINT16 u2VTotal50Hz;
    UINT16 u2VTotalMin;
    UINT8 u1VClkMax;
    UINT8 u1VClkMin;

    UINT8 u1HSyncWidth;
    UINT8 u1VSyncWidth;
    UINT16 u2HPosition;
    UINT16 u2VPosition;
    UINT32 u4ControlWord;

    // backlight ragne
    UINT8 u1BacklightHigh;
    UINT8 u1BacklightMiddle;
    UINT8 u1BacklightLow;
    UINT8 u1Reserved;

    UINT16 u2DimmingFrequency60Hz;
    UINT16 u2DimmingFrequency50Hz;

    // delay unit: 10ms
    UINT8 u1LvdsOnDalay;
    UINT8 u1BacklightOnDelay;
    UINT8 u1BacklightOffDelay;
    UINT8 u1LvdsOffDalay;

    UINT16 u2MaxOverscan;
    UINT32 u4ControlWord2;
} PANEL_ATTRIBUTE_T;


typedef struct
{
    UINT16 u2PanelWidth;
    UINT16 u2PanelHeight;

    UINT32 u4PixelClkMax;
    UINT32 u4PixelClk60Hz;
    UINT32 u4PixelClk50Hz;
    UINT32 u4PixelClkMin;

    UINT32 u4Reserved1;

    UINT16 u2HTotalMax;
    UINT16 u2HTotal60Hz;
    UINT16 u2HTotal50Hz;
    UINT16 u2HTotalMin;

    UINT16 u2Reserved2;
    UINT16 u2VTotalMax;
    UINT16 u2VTotal60Hz;
    UINT16 u2VTotal50Hz;
    UINT16 u2VTotalMin;
    UINT16 u2Reserved3;

    UINT32 u4Reserved4;

    UINT16 u2Reserved5;
    UINT8 u1VClkMax;
    UINT8 u1VClkMin;

    UINT16 u2Reserved6;
    UINT8 u1HSyncWidth;
    UINT8 u1VSyncWidth;

    UINT8 u2HPosition;
    UINT8 u2VPosition;
    UINT16 u2Reserved7;

    UINT16 u4Reserved8;
    UINT8 u1LvdsMapping;
    UINT8 u1MaxOverscan1;

    UINT8 u1MaxOverscan2;
    UINT8 u1ControlWord1;
    UINT8 u1ControlWord2;
    UINT8 u1PanelOnTime1;

    UINT8 u1LvdsOnDalay;

    UINT8 u1BacklightOnDelay;
    UINT8 u1PanelOnTime4;
    UINT8 u1Reserved8;

    UINT8 u1PanelOffTime1;
    UINT8 u1BacklightOffDelay;
    UINT8 u1LvdsOffDalay;
    UINT8 u1PanelOffTime4;
} PANEL_FLASH_ATTRIBUTE_T;

//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------

/*lint -save -e960 */
#define PANEL_ATTRIBUTE_SET_TMPL(NAME, FIELD) \
    void PANEL_Set##NAME(UINT32 u4Value) \
    { \
        _prPanelAttribute->FIELD = u4Value; \
    } \

#define PANEL_ATTRIBUTE_GET_TMPL(NAME, FIELD) \
    UINT32 PANEL_Get##NAME(void) \
    { \
        return _prPanelAttribute->FIELD; \
    } \

#define PANEL_ATTRIBUTE_SETGET_TMPL(NAME, FIELD) \
    PANEL_ATTRIBUTE_SET_TMPL(NAME, FIELD) \
    PANEL_ATTRIBUTE_GET_TMPL(NAME, FIELD) \

/* above line is intendedly left blank */
/*lint -restore */

#define IS_PANEL_INDEX_INVALID(X) ((X) >= u4TotalNumber)
#define CHECK_ZERO(X) \
    { \
        if ((X) == 0) \
        { \
            Printf( "Line %d: is zero\r\n", __LINE__ ); \
            return FALSE; \
        } \
    }


#define EXT_PANEL_INDEX_INVALID 0xffff

#define SCALER_PSCAN_DISPMODE_MIN_VTOTAL_OFFSET (10)

//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------

#ifndef CC_MULTI_PANEL
// we build all panel timing in default
#define CC_MULTI_PANEL 1
#endif

#ifdef CC_NAND_LOADER
#define NAND_PAGE_SIZE  (2048)
#endif

// this is the panel_table_cust.c in each customer's directory
#include PANEL_TABLE_CUST_FILE

#if CC_MULTI_PANEL
static PANEL_ATTRIBUTE_T* _prPanelAttribute =
                          &_arPanelAttribute[DEFAULT_PANEL_SELECT];
static UINT32 _u4PanelIndex = DEFAULT_PANEL_SELECT;
static UINT32 _u4EepromPanelIndex = DEFAULT_PANEL_SELECT;

#if defined(SUPPORT_PANEL_CUSTOMER_SPEC) || defined(SUPPORT_PANEL_SS) || defined(SUPPORT_PANEL_DITHER)
static PANEL_ATTRIBUTE_EXT_T* _prPanelAttributeExt = & _arPanelAttributeExt[0];
#endif

#else
static PANEL_ATTRIBUTE_T* _prPanelAttribute = &_arPanelAttribute[0];
static UINT32 _u4PanelIndex = 0;
static UINT32 _u4EepromPanelIndex = 0;
#endif
static UINT32 u4TotalNumber = PANEL_TOTAL_NUMBER;
static INT32 _i4HPixelShift = 0;
static INT32 _i4VPixelShift = 0;
static BOOL _fgPixelShiftEnable = FALSE;
static UINT32 _fgPanelIdInEeprom = FALSE;

#if defined(SUPPORT_PANEL_CUSTOMER_SPEC) || defined(SUPPORT_PANEL_SS) || defined(SUPPORT_PANEL_DITHER)  || defined(SUPPORT_PANEL_3D_PIXEL_SIZE) || defined(SUPPORT_PANEL_SCAN_PWM)
static UINT32 PANEL_GetExtAttributeIndex(void);
#endif
//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------

#ifndef LOAD_PANEL_FROM_FLASH_CUSTOM
#ifndef NOT_USE_EEP
static BOOL _PanelLoadTableFromFlash(UINT32 u4Offset)
{
    PANEL_FLASH_ATTRIBUTE_T rFlashTable;
#ifndef CC_NAND_LOADER
    Printf("Load panel table from flash, address=0x%x\n", u4Offset);
    // FIXME!!
    // Need to replace with new interface of NOR flash access

    x_memcpy((void*)&rFlashTable, (void*)u4Offset, sizeof(PANEL_FLASH_ATTRIBUTE_T));
#else
    // FIXME!!
    // Need to replace with new interface of NAND flash access

    UINT64  u8Offset = (UINT64)u4Offset;
    UINT8   aui1_buf[sizeof(PANEL_FLASH_ATTRIBUTE_T)+NAND_PAGE_SIZE*2];

    /* must be aligned */
    UINT64  u8_algin_bytes;
    UINT32  ui4_read_size;  /* should be aligned */
    UINT8*  pui1_buf = NULL;
    UINT8   ui1_idx = 0;


    u8_algin_bytes = u8Offset % NAND_PAGE_SIZE;

    ui4_read_size = sizeof(PANEL_FLASH_ATTRIBUTE_T)+(UINT32)u8_algin_bytes;
    ui4_read_size += NAND_PAGE_SIZE - (ui4_read_size % NAND_PAGE_SIZE);

    pui1_buf  = (UINT8 *)&aui1_buf;
    //if (u8_algin_bytes != 0)
    {
        while (ui4_read_size > 0)
        {
            NAND_Read(u8Offset-u8_algin_bytes+ui1_idx*NAND_PAGE_SIZE, 
                     (UINT32)pui1_buf+ui1_idx*NAND_PAGE_SIZE, 
                      NAND_PAGE_SIZE);
            ui1_idx++;
            ui4_read_size -= NAND_PAGE_SIZE;
        }
        x_memcpy(&rFlashTable, pui1_buf+(UINT32)u8_algin_bytes, sizeof(PANEL_FLASH_ATTRIBUTE_T));
    }
#endif
    _arPanelAttribute[0].u2PanelWidth = rFlashTable.u2PanelWidth;
    CHECK_ZERO(_arPanelAttribute[0].u2PanelWidth);
    _arPanelAttribute[0].u2PanelHeight = rFlashTable.u2PanelHeight;
    CHECK_ZERO(_arPanelAttribute[0].u2PanelHeight);
    _arPanelAttribute[0].u4PixelClkMax = rFlashTable.u4PixelClkMax;
    CHECK_ZERO(_arPanelAttribute[0].u4PixelClkMax);
    _arPanelAttribute[0].u4PixelClk60Hz = rFlashTable.u4PixelClk60Hz;
    CHECK_ZERO(_arPanelAttribute[0].u4PixelClk60Hz);
    _arPanelAttribute[0].u4PixelClk50Hz = rFlashTable.u4PixelClk50Hz;
    CHECK_ZERO(_arPanelAttribute[0].u4PixelClk50Hz);
    _arPanelAttribute[0].u4PixelClkMin = rFlashTable.u4PixelClkMin;
    CHECK_ZERO(_arPanelAttribute[0].u4PixelClkMin);

    _arPanelAttribute[0].u2HTotalMax = rFlashTable.u2HTotalMax;
    CHECK_ZERO(_arPanelAttribute[0].u2HTotalMax);
    _arPanelAttribute[0].u2HTotal60Hz = rFlashTable.u2HTotal60Hz;
    CHECK_ZERO(_arPanelAttribute[0].u2HTotal60Hz);
    _arPanelAttribute[0].u2HTotal50Hz = rFlashTable.u2HTotal50Hz;
    CHECK_ZERO(_arPanelAttribute[0].u2HTotal50Hz);
    _arPanelAttribute[0].u2HTotalMin = rFlashTable.u2HTotalMin;
    CHECK_ZERO(_arPanelAttribute[0].u2HTotalMin);

    _arPanelAttribute[0].u2VTotalMax = rFlashTable.u2VTotalMax;
    CHECK_ZERO(_arPanelAttribute[0].u2VTotalMax);
    _arPanelAttribute[0].u2VTotal60Hz = rFlashTable.u2VTotal60Hz;
    CHECK_ZERO(_arPanelAttribute[0].u2VTotal60Hz);
    _arPanelAttribute[0].u2VTotal50Hz = rFlashTable.u2VTotal50Hz;
    CHECK_ZERO(_arPanelAttribute[0].u2VTotal50Hz);
    _arPanelAttribute[0].u2VTotalMin = rFlashTable.u2VTotalMin;
    CHECK_ZERO(_arPanelAttribute[0].u2VTotalMin);

    _arPanelAttribute[0].u1VClkMax = rFlashTable.u1VClkMax;
    CHECK_ZERO(_arPanelAttribute[0].u1VClkMax);
    _arPanelAttribute[0].u1VClkMin = rFlashTable.u1VClkMin;
    CHECK_ZERO(_arPanelAttribute[0].u1VClkMin);

    _arPanelAttribute[0].u1HSyncWidth = rFlashTable.u1HSyncWidth;
    CHECK_ZERO(_arPanelAttribute[0].u1HSyncWidth);
    _arPanelAttribute[0].u1VSyncWidth = rFlashTable.u1VSyncWidth;
    CHECK_ZERO(_arPanelAttribute[0].u1VSyncWidth);
    _arPanelAttribute[0].u2HPosition = rFlashTable.u2HPosition << 4;
    CHECK_ZERO(_arPanelAttribute[0].u2HPosition);
    _arPanelAttribute[0].u2VPosition = rFlashTable.u2VPosition << 3;
    CHECK_ZERO(_arPanelAttribute[0].u2VPosition);
    _arPanelAttribute[0].u2MaxOverscan = (UINT16)rFlashTable.u1MaxOverscan1;
    _arPanelAttribute[0].u2MaxOverscan |= (UINT16)rFlashTable.u1MaxOverscan2 << 8;
    _arPanelAttribute[0].u4ControlWord = (UINT32)rFlashTable.u1ControlWord1;
    _arPanelAttribute[0].u4ControlWord |= (UINT32)rFlashTable.u1ControlWord2 << 8;
    _arPanelAttribute[0].u4ControlWord |= (UINT32)rFlashTable.u4Reserved8 << 16;	

    _arPanelAttribute[0].u1LvdsOnDalay = rFlashTable.u1LvdsOnDalay;
    _arPanelAttribute[0].u1BacklightOnDelay = rFlashTable.u1BacklightOnDelay;
    _arPanelAttribute[0].u1BacklightOffDelay = rFlashTable.u1BacklightOffDelay;
    _arPanelAttribute[0].u1LvdsOffDalay = rFlashTable.u1LvdsOffDalay;

    SelectPanel(0);

#ifdef CC_EXTERNAL_LVDS_CHIP
    {
        PanelDataStructType rPanel;
        UINT8 bOutType;
        UINT8 bHSYNCP;
        UINT8 bVSYNCP;
        UINT8 bDataClk;
        UINT8 bDataEn;
        UINT8 bMSBOnOff;

        if (rFlashTable.u2PanelHeight == 768)
        {
            rPanel.PanelType_R = 0; // WXGA/768p = 0
        }
        else
        {
            rPanel.PanelType_R = 1; // WUXGA/1080p = 1
        }
        if (LVDS_OUTPUT_PORT == SINGLE_PORT)
        {
            rPanel.PanelPixPerClk_R = 0; // single
        }
        else
        {
            rPanel.PanelPixPerClk_R = 1; // dule
        }
        switch (DISP_BIT)
        {
            case DISP_30BIT:
                rPanel.PanelDepth_R = 10; // 10 bit
                break;

            case DISP_24BIT:
                rPanel.PanelDepth_R = 8; // 8 bit
                break;

            default:
                rPanel.PanelDepth_R = 6; // 6 bit
                break;
        }
        rPanel.PanelWidth_R = rFlashTable.u2PanelWidth;
        rPanel.PanelHeight_R = rFlashTable.u2PanelHeight;
        rPanel.PanelMinHTotal_R = rFlashTable.u2HTotalMin;
        rPanel.PanelMinHSyncWidth_R = rFlashTable.u1HSyncWidth;
        rPanel.PanelMinHSyncBackPorch_R = HSYNCLEN_TYPI - rFlashTable.u1HSyncWidth - rFlashTable.u2HPosition;
        rPanel.PanelTypVTotal_R = rFlashTable.u2VTotal60Hz;
        rPanel.PanelMinVSyncWidth_R = rFlashTable.u1VSyncWidth;
        rPanel.PanelMinVSyncBackPorch_R = rFlashTable.u1VSyncWidth + rFlashTable.u2VPosition + 1;

        #if (HSYNC_POLARITY == HSYNC_LOW)
        {
            bHSYNCP = 1;
        }
        #else
        {
            bHSYNCP = 0;
        }
        #endif

        #if (VSYNC_POLARITY == VSYNC_LOW)
        {
            bVSYNCP = 1;
        }
        #else
        {
            bVSYNCP = 0;
        }
        #endif

        bDataClk = 0;
        bDataEn = 0;

        if (LVDS_DISP_FORMAT == LVDS_MSB_SW_ON) // NS
        {
            bMSBOnOff = 1;
        }
        else // JEIDA
        {
            bMSBOnOff = 0;
        }
        rPanel.PanelOutputType_R = (bVSYNCP << 7) | (bHSYNCP << 6) | (bDataClk << 5) | (bDataEn << 4) | (bMSBOnOff << 3);

        IMpac3P_SetPanelParam(&rPanel, sizeof(rPanel));
        IMpac3P_SetPower(1);
    }
#endif /* CC_EXTERNAL_LVDS_CHIP */
    return TRUE;
}
#endif
#endif

//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

PANEL_ATTRIBUTE_SET_TMPL(PanelWidth, u2PanelWidth)
PANEL_ATTRIBUTE_SET_TMPL(PanelHeight, u2PanelHeight)
PANEL_ATTRIBUTE_SETGET_TMPL(PixelClkMax, u4PixelClkMax)
#if !SCALER_PSCAN_DISPMODE_MIN_VTOTAL_OFFSET 
PANEL_ATTRIBUTE_SETGET_TMPL(PixelClk60Hz, u4PixelClk60Hz)
PANEL_ATTRIBUTE_SETGET_TMPL(PixelClk50Hz, u4PixelClk50Hz)
#else
PANEL_ATTRIBUTE_SET_TMPL(PixelClk60Hz, u4PixelClk60Hz)
PANEL_ATTRIBUTE_SET_TMPL(PixelClk50Hz, u4PixelClk50Hz)
#endif
PANEL_ATTRIBUTE_SETGET_TMPL(PixelClkMin, u4PixelClkMin)
PANEL_ATTRIBUTE_SETGET_TMPL(HTotalMax, u2HTotalMax)
PANEL_ATTRIBUTE_SETGET_TMPL(HTotal60Hz, u2HTotal60Hz)
PANEL_ATTRIBUTE_SETGET_TMPL(HTotal50Hz, u2HTotal50Hz)
PANEL_ATTRIBUTE_SETGET_TMPL(HTotalMin, u2HTotalMin)
PANEL_ATTRIBUTE_SETGET_TMPL(VTotalMax, u2VTotalMax)
#if !SCALER_PSCAN_DISPMODE_MIN_VTOTAL_OFFSET 
PANEL_ATTRIBUTE_SETGET_TMPL(VTotal60Hz, u2VTotal60Hz)
PANEL_ATTRIBUTE_SETGET_TMPL(VTotal50Hz, u2VTotal50Hz)
#else
PANEL_ATTRIBUTE_SET_TMPL(VTotal60Hz, u2VTotal60Hz)
PANEL_ATTRIBUTE_SET_TMPL(VTotal50Hz, u2VTotal50Hz)
#endif
PANEL_ATTRIBUTE_SETGET_TMPL(VTotalMin, u2VTotalMin)
PANEL_ATTRIBUTE_SETGET_TMPL(VClkMax, u1VClkMax)
PANEL_ATTRIBUTE_SETGET_TMPL(VClkMin, u1VClkMin)
//#ifndef SUPPORT_PANEL_CUSTOMER_SPEC
PANEL_ATTRIBUTE_SETGET_TMPL(HSyncWidth, u1HSyncWidth)
PANEL_ATTRIBUTE_SETGET_TMPL(VSyncWidth, u1VSyncWidth)
//#else
//    PANEL_ATTRIBUTE_SET_TMPL(HSyncWidth, u1HSyncWidth)
//    PANEL_ATTRIBUTE_SET_TMPL(VSyncWidth, u1VSyncWidth)
//#endif
PANEL_ATTRIBUTE_SET_TMPL(HPosition60Hz, u2HPosition)
PANEL_ATTRIBUTE_SET_TMPL(VPosition, u2VPosition)
PANEL_ATTRIBUTE_SETGET_TMPL(ControlWord, u4ControlWord)
PANEL_ATTRIBUTE_SETGET_TMPL(DimmingFrequency60Hz, u2DimmingFrequency60Hz)
PANEL_ATTRIBUTE_SETGET_TMPL(DimmingFrequency50Hz, u2DimmingFrequency50Hz)
PANEL_ATTRIBUTE_SETGET_TMPL(LvdsOnDalay, u1LvdsOnDalay)
PANEL_ATTRIBUTE_SETGET_TMPL(BacklightOnDelay, u1BacklightOnDelay)
PANEL_ATTRIBUTE_SETGET_TMPL(BacklightOffDelay, u1BacklightOffDelay)
PANEL_ATTRIBUTE_SETGET_TMPL(LvdsOffDalay, u1LvdsOffDalay)
PANEL_ATTRIBUTE_SETGET_TMPL(MaxOverscan, u2MaxOverscan)
PANEL_ATTRIBUTE_SETGET_TMPL(ControlWord2, u4ControlWord2)

static UINT8 fgLVDSPCBSwap = 0;
//----------------------------------------------------------------------------
/** DRVCUST_PanelQuery() return customization paramter.
 */
//----------------------------------------------------------------------------
INT32 DRVCUST_PanelQuery(QUERY_TYPE_T eQryType, UINT32 *pu4Data)
{
#ifdef CC_BACKLIGHT_PWM_BY_MODEL
#ifdef PANEL_BACKLIGHT_PWM_PORT
    static INT32 i4ModelIdx = -1;
#endif // PANEL_BACKLIGHT_PWM_PORT
#endif

    ASSERT(pu4Data!=NULL);

    switch(eQryType)
    {
    // Panel
    case eDefaultPanelSelect:
        *pu4Data = DEFAULT_PANEL_SELECT;
        return 0;
    case eLvdsACtrlEven:
		if(fgLVDSPCBSwap)
        *pu4Data = LVDS_A_CTRL_ODD;
		else
        *pu4Data = LVDS_A_CTRL_EVEN;
        return 0;
    case eLvdsACtrlOdd:
		if(fgLVDSPCBSwap)		
        *pu4Data = LVDS_A_CTRL_EVEN;
		else
        *pu4Data = LVDS_A_CTRL_ODD;
        return 0;
    case eLvdsBCtrlEven:
		if(fgLVDSPCBSwap)				
        *pu4Data = LVDS_B_CTRL_ODD;
		else
        *pu4Data = LVDS_B_CTRL_EVEN;
        return 0;
    case eLvdsBCtrlOdd:
		if(fgLVDSPCBSwap)				
        *pu4Data = LVDS_B_CTRL_EVEN;
		else
        *pu4Data = LVDS_B_CTRL_ODD;
        return 0;
    case eBackLightGpio:
#ifdef BACKLIGHT_GPIO
        *pu4Data = BACKLIGHT_GPIO;
        return 0;
#else
        return -1;
#endif
    case eBackLightOnVal:
#ifdef BACKLIGHT_ON_VAL
        *pu4Data = BACKLIGHT_ON_VAL;
        return 0;
#else
        return -1;
#endif
    case ePanelPowerGpio:
#ifdef PANELPOWER_GPIO
        *pu4Data = PANELPOWER_GPIO;
        return 0;
#else
        return -1;
#endif
    case ePanelPowerOnVal:
#ifdef PANELPOWER_ON_VAL
        *pu4Data = PANELPOWER_ON_VAL;
        return 0;
#else
        return -1;
#endif
    case ePanelDCRGpio:
#ifdef PANEL_DCR_GPIO
        *pu4Data = PANEL_DCR_GPIO;
        return 0;
#else
        return -1;
#endif /* PANEL_DCR_GPIO */

    case ePanelDCROnVal:
#ifdef PANEL_DCR_ON_VAL
        *pu4Data = PANEL_DCR_ON_VAL;
        return 0;
#else
        return -1;
#endif /* PANEL_DCR_ON_VAL */

    case ePanelPdpVsGpio:
#ifdef PANEL_PDP_VS_GPIO
        *pu4Data = PANEL_PDP_VS_GPIO;
        return 0;
#else
        return -1;
#endif /* PANEL_PDP_VS_GPIO */

    case ePanelPdpVsOnVal:
#ifdef PANEL_PDP_VS_ON_VAL
        *pu4Data = PANEL_PDP_VS_ON_VAL;
        return 0;
#else
        return -1;
#endif /* PANEL_PDP_VS_ON_VAL */

    case eLvdsMsbSw:
#ifdef LVDS_MSB_SW
        *pu4Data = LVDS_MSB_SW;
        return 0;
#else
        return -1;
#endif /* LVDS_MSB_SW */

    case ePanelPwmPrescalar:
#ifdef PWM_PRESCALAR
        *pu4Data = PWM_PRESCALAR;
        return 0;
#else
        return -1;
#endif /* PWM_PRESCALAR */

    case ePanelDisableDimming:
#ifdef PANEL_DISABLE_DIMMING
        *pu4Data = PANEL_DISABLE_DIMMING;
        return 0;
#else
        return -1;
#endif /* PANEL_DISABLE_DIMMING */

    case ePanelChannelSwap:
        *pu4Data = PANEL_CHANNEL_SWAP;
        return 0;

    case ePanelPwmPolarity:
#ifdef PWM_POLARITY
        *pu4Data = PWM_POLARITY;
        return 0;
#else
        return -1;
#endif /* PWM_POLARITY */
    case ePanelIndexFromEeprom:
#ifdef PANEL_INDEX_FROM_EEPROM
        *pu4Data = PANEL_INDEX_FROM_EEPROM;
        return 0;
#else
        return -1;
#endif /* PANEL_INDEX_FROM_EEPROM */

    case ePanelIndexOffset1:
#ifdef PANEL_INDEX_OFFSET1
        *pu4Data = PANEL_INDEX_OFFSET1;
        return 0;
#else
        return -1;
#endif /* PANEL_INDEX_OFFSET1 */

    case ePanelIndexOffset2:
#ifdef PANEL_INDEX_OFFSET2
        *pu4Data = PANEL_INDEX_OFFSET2;
        return 0;
#else
        return -1;
#endif /* PANEL_INDEX_OFFSET2 */

    case ePanelIndexOffset3:
#ifdef PANEL_INDEX_OFFSET3
        *pu4Data = PANEL_INDEX_OFFSET3;
        return 0;
#else
        return -1;
#endif /* PANEL_INDEX_OFFSET3 */
    case ePanelIndexFromGpioNum:
        *pu4Data = PANEL_INDEX_FROM_GPIO_NUM;
        return 0;

    case ePanelIndexFromGpioIdx:
        *pu4Data = (UINT32)PANEL_INDEX_FROM_GPIO_IDX;
        return 0;

    case ePanelIndexFromGpioMap:
        *pu4Data = (UINT32)PANEL_INDEX_FROM_GPIO_MAP;
        return 0;

    case ePanelLvds8bitPinMap:
        *pu4Data = (UINT32)PANEL_LVDS_8BIT_PIN_MAP;
        return 0;

    case ePanelLvds10bitPinMap:
        *pu4Data = (UINT32)PANEL_LVDS_10BIT_PIN_MAP;
        return 0;

    case ePanelLvds10bitPNSwap:
        *pu4Data = (UINT32)PANEL_LVDS_10BIT_PN_SWAP;
        return 0;

    case ePanelVB1LaneSwap:
        *pu4Data = (UINT32)PANEL_VB1_LANE_SWAP;
        return 0;

    case ePanelVB1LanePNSwap:
        *pu4Data = (UINT32)PANEL_VB1_LANE_PN_SWAP;
        return 0;

    case eBacklightHigh:
#ifdef BACKLIGHT_HIGH_PWM
        *pu4Data = BACKLIGHT_HIGH_PWM;
        return 0;
#else
        return -1;
#endif /* BACKLIGHT_HIGH_PWM */

    case eBacklightMiddle:
#ifdef BACKLIGHT_MIDDLE_PWM
        *pu4Data = BACKLIGHT_MIDDLE_PWM;
        return 0;
#else
        return -1;
#endif /* BACKLIGHT_MIDDLE_PWM */

    case eBacklightLow:
#ifdef BACKLIGHT_LOW_PWM
        *pu4Data = BACKLIGHT_LOW_PWM;
        return 0;
#else
        return -1;
#endif /* BACKLIGHT_LOW_PWM */

    case eBackLightGpio2:
#ifdef BACKLIGHT_GPIO2
        *pu4Data = BACKLIGHT_GPIO2;
        return 0;
#else
        return -1;
#endif /* BACKLIGHT_GPIO2 */

    case eSpreadSpectrumPermillage:
#ifdef SPREAD_SPECTRUM_PERMILLAGE
        *pu4Data = SPREAD_SPECTRUM_PERMILLAGE;
        return 0;
#else
        return -1;
#endif /* SPREAD_SPECTRUM_PERMILLAGE */

    case eTimerDimmingFreq:
#ifdef TIMER_DIMMING_FREQ
        *pu4Data = TIMER_DIMMING_FREQ;
        return 0;
#else
        return -1;
#endif /* TIMER_DIMMING_FREQ */

    case eDimmingFreq60:
        *pu4Data = DIMMING_FREQ_60;
        return 0;

    case eDimmingFreq50:
        *pu4Data = DIMMING_FREQ_50;
        return 0;

    case ePanelTTLOutput:
        *pu4Data = PANEL_TTL_OUTPUT;
        return 0;

    case ePanelFlashTableSize:
        *pu4Data = PANEL_FLASH_TABLE_SIZE;
        return 0;

    case ePanelFlashTableAddress:
        *pu4Data = PANEL_FLASH_TABLE_ADDRESS;
        return 0;

    case ePanelEepromToFlashIndexMap:
        *pu4Data = (UINT32)PANEL_EEPROM_TO_FLASH_INDEX_MAP;
        return 0;

    case eLvdsDrivingCurrent:
#ifdef LVDS_DRIVING_CURRENT
        *pu4Data = LVDS_DRIVING_CURRENT;
        return 0;
#else
        return -1;
#endif /* LVDS_DRIVING_CURRENT */

    case ePanelBacklightPwmPort:
#ifdef PANEL_BACKLIGHT_PWM_PORT
		*pu4Data = PANEL_BACKLIGHT_PWM_PORT;
#else
		*pu4Data = 2; // Default using PWM2 as backlight control.
#endif /* PANEL_BACKLIGHT_PWM_PORT */
		return 0;

    case ePanelBacklightGPIO:
#ifdef PANEL_BACKLIGHT_PWM_GPIO
#ifdef CC_BACKLIGHT_PWM_BY_MODEL
        if (i4ModelIdx == -1)
        {
            i4ModelIdx = GetModelIndexFromEEPROM ();
        }
        *pu4Data = GetModelIndexBacklightPwmPort (i4ModelIdx);
#else
        *pu4Data = PANEL_BACKLIGHT_PWM_PORT;
#endif        
#else
		*pu4Data = 0; // Default using PWM output
#endif /* PANEL_BACKLIGHT_PWM_GPIO */
    return 0;

	case eFlagBacklightBoostCtrl:
#ifdef SUPPORT_BACKLIGHT_BOOST_CTRL
		*pu4Data = (UINT32)SUPPORT_BACKLIGHT_BOOST_CTRL;
#else
		*pu4Data = (UINT32)0;
#endif // #ifdef SUPPORT_BACKLIGHT_BOOST_CTRL
		return 0;
	case eFlagBacklightBoostPort:
#ifdef PANEL_BACKLIGHT_BOOST_PORT
		*pu4Data = (UINT32)PANEL_BACKLIGHT_BOOST_PORT;
#else
		*pu4Data = (UINT32)1;
#endif // #ifdef PANEL_BACKLIGHT_BOOST_PORT
		return 0;
	case eFlagBacklightBoostInvert:
#ifdef SUPPORT_BACKLIGHT_BOOST_INVERT
		*pu4Data = (UINT32)SUPPORT_BACKLIGHT_BOOST_INVERT;
#else
		*pu4Data = (UINT32)0;
#endif // #ifdef SUPPORT_BACKLIGHT_BOOST_INVERT
		return 0;
	case eFlagBacklightDimmingLock:
#ifdef SUPPORT_BACKLIGHT_DIMMING_LOCK
		*pu4Data = (UINT32)SUPPORT_BACKLIGHT_DIMMING_LOCK;
#else
        // default set pwm lock ON. No, default is OFF, if needed, you can set PWM lock via vDrvSetLock().
		*pu4Data = (UINT32)0;
#endif // #ifdef SUPPORT_BACKLIGHT_DIMMING_LOCK
		return 0;
	case eFlagBacklightDimmingInvert:
#ifdef SUPPORT_BACKLIGHT_DIMMING_INVERT
		*pu4Data = (UINT32)SUPPORT_BACKLIGHT_DIMMING_INVERT;
#else
		*pu4Data = (UINT32)0;
#endif // #ifdef SUPPORT_BACKLIGHT_DIMMING_INVERT
		return 0;
    case ePanelPixelShiftHMax:
        *pu4Data = PANEL_PIXEL_SHIFT_H_MAX;
        return 0;
    case ePanelPixelShiftVMax:
        *pu4Data = PANEL_PIXEL_SHIFT_V_MAX;
        return 0;

    case ePanelBitSelectCtrlGpio:
#ifdef PANEL_BIT_SELECT_CTRL_GPIO
        *pu4Data = PANEL_BIT_SELECT_CTRL_GPIO;
        return 0;
#else
        *pu4Data = (UINT32)-1;
        return -1;
#endif

        case ePanelLvdsSelectCtrlGpio:
#ifdef PANEL_LVDS_SELECT_CTRL_GPIO
            *pu4Data = PANEL_LVDS_SELECT_CTRL_GPIO;
            return 0;
#else
            *pu4Data = (UINT32)-1;
            return -1;
#endif

        case ePanel242530pOutput:
            #ifdef PANEL_SUPPORT_24_25_30_OUTPUT           
            *pu4Data = (UINT32)PANEL_SUPPORT_24_25_30_OUTPUT;
            #else
            *pu4Data = (UINT32)0;
            #endif
            return 0;


        case eDDDSErrorLimit:
            #ifdef DDDS_ERROR_LIMIT
            *pu4Data = DDDS_ERROR_LIMIT;
            return 0;
            #else
            *pu4Data = (UINT32)-1;
            return -1;
            #endif

        case eDDDSLockOnOthers:
            #ifdef DDDS_LOCK_OTHERS
            *pu4Data = DDDS_LOCK_OTHERS;
            #else
            *pu4Data = (UINT32)1;
            #endif
            return 0;

		case eDISP_RLVDSEncodeLink:
		#ifdef DISP_RLVDSENCODE_PORT
			*pu4Data = DISP_RLVDSENCODE_LINK;
			return 0;
		#else
			*pu4Data = (UINT32)-1;
			return -1;
		#endif
		
#ifdef CC_ENABLE_MTK_MODEL_INDEX
        case eModelIndexOffset1:
#ifdef MODEL_INDEX_OFFSET1
            *pu4Data = MODEL_INDEX_OFFSET1;
            return 0;
#else
            return -1;
#endif /* MODEL_INDEX_OFFSET1 */
        
        case eModelIndexOffset2:
#ifdef MODEL_INDEX_OFFSET2
            *pu4Data = MODEL_INDEX_OFFSET2;
            return 0;
#else
            return -1;
#endif /* MODEL_INDEX_OFFSET2 */
    
        case eModelIndexOffset3:
#ifdef MODEL_INDEX_OFFSET3
            *pu4Data = MODEL_INDEX_OFFSET3;
            return 0;
#else
            return -1;
#endif /* MODEL_INDEX_OFFSET3 */
#endif /* #ifdef CC_ENABLE_MTK_MODEL_INDEX */

    default:
        return -1;
    }
}

UINT32 DRVCUST_PanelGet(QUERY_TYPE_T eQryType)
{
    UINT32 u4Ret = 0xffffffff;

    UNUSED(DRVCUST_PanelQuery(eQryType, &u4Ret));
    return u4Ret;
}


//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------

UINT32 IsLvdsMsbSwOn(void)
{
    UINT32 u4Value;
    if (DRVCUST_PanelQuery(eLvdsMsbSw, &u4Value) == 0)
    {
        return u4Value;
    }
    if (IS_COMPANION_CHIP_ON())
    {
        return LVDS_JEIDA;
    }
   return (PANEL_GetControlWord() & (LVDS_MSB_SW_ON | LVDS_SPECIAL_NS));
}


/**
* @brief Check if support 120Hz Panel
* @retval SV_TURE/SV_FALSE
*/
UINT8 fgIsSupport120HzPanel(void)
{
    if (PANEL_GetVClkMax() > 100)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}


//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------

UINT32 IsPwmLowPanelBright(void)
{
    UINT32 u4Value;
    if (DRVCUST_PanelQuery(ePanelPwmPolarity, &u4Value) == 0)
    {
        return u4Value;
    }
    return PANEL_GetControlWord() & PWM_LOW_PANEL_BRIGHT;
}


//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------

void SelectPanel(UINT32 u4Index)
{
	#ifdef SUPPORT_PANEL_CUSTOMER_SPEC
	UINT32 u4ExtIdx = EXT_PANEL_INDEX_INVALID;
	#endif
    // read panel index from customer define
    if (u4Index == PANEL_INVALID)
    {
#if defined(CC_LGE_PROTO_PCBA) && defined(CC_UBOOT)
	
		UINT32 u4ControlWord ;
		printf("#resolution: %d\n",gPanelInfo.panel_resolution);	// 1
		printf("#interface: %d\n",gPanelInfo.panel_interface);// 1
		printf("#bSupport_frc: %d\n",gPanelInfo.bSupport_frc);// 0
		printf("#eModelModuleType: %d\n",gPanelInfo.eModelModuleType);// 0
		printf("#nLVDSBit: %d\n",gPanelInfo.nLVDSBit);// 0

		if(gPanelInfo.panel_resolution)
		{
			if(gPanelInfo.bSupport_frc==0)
				_u4PanelIndex=PANEL_LG_37_WU1;
			else if(gPanelInfo.bSupport_frc==1)
				_u4PanelIndex=PANEL_LG_42_WUD_SAC1_10BIT_NS;
		
		}
		else
		{
			if(gPanelInfo.bSupport_frc==0)
				_u4PanelIndex=PANEL_LG_26_WX2;
			else if(gPanelInfo.bSupport_frc==1)
		   		_u4PanelIndex=PANEL_LG_42_WX4_SLB1_8280;

		}
		u4ControlWord = PANEL_GetControlWord();
		if(gPanelInfo.panel_interface ==MODELOPT_PANEL_INTERFACE_EPI) 
			u4ControlWord |= DISP_TYPE_EPI;
	    PANEL_SetControlWord(u4ControlWord);

		 _prPanelAttribute = &_arPanelAttribute[_u4PanelIndex];

#else 
#ifdef  CC_LGE_PROTO_PCBA
						u4Index = PANEL_LG_37_WU1;
#else

	u4Index = DRVCUST_PanelGet(eDefaultPanelSelect);
#endif
#endif
    }
    if (u4Index != PANEL_INVALID)
    {
        _u4PanelIndex = u4Index;

#ifndef CC_MULTI_PANEL
        _u4PanelIndex = 0;
#endif

        _prPanelAttribute = &_arPanelAttribute[_u4PanelIndex];
    }
	#ifdef SUPPORT_PANEL_CUSTOMER_SPEC
	u4ExtIdx = PANEL_GetExtAttributeIndex();
    if ((u4ExtIdx != EXT_PANEL_INDEX_INVALID))
    {
		_prPanelAttributeExt =	& _arPanelAttributeExt[u4ExtIdx];
    }
	#endif
}


//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------

CHAR* GetCurrentPanelName(void)
{
    if (DRVCUST_PanelGet(ePanelFlashTableSize) > 0)
    {
        return GetPanelName(PANEL_GetEepromPanelIndex());
    }
    else
    {
        return GetPanelName(GetCurrentPanelIndex());
    }
}


//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------

UINT32 GetCurrentPanelIndex(void)
{
    if (DRVCUST_PanelGet(ePanelFlashTableSize) > 0)
    {
        return PANEL_GetEepromPanelIndex();
    }
    else
    {
	    return _u4PanelIndex;
    }
}

UINT32 GetFlagPanelIdInEeprom(void)
{
    return _fgPanelIdInEeprom;
}

//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------

void DumpPanelAttribute(UINT32 u4Flag)
{
    UINT32 u4Size = sizeof(_arPanelAttribute) / sizeof(PANEL_ATTRIBUTE_T);
    UINT32 i = 0;
    PANEL_ATTRIBUTE_T* prTbl;

    if ((u4Flag & PANEL_DUMP_ALL) == 0)
    {
        i = _u4PanelIndex;
        u4Size = _u4PanelIndex + 1;
    }
    for (; i < u4Size; i++)
    {
        prTbl = &_arPanelAttribute[i];
        if ((DRVCUST_PanelGet(ePanelFlashTableSize) > 0) && (i == 0))
        {
            Printf("---------- Panel[%3d] %s %dx%d ----------\n",
                   PANEL_GetEepromPanelIndex(), GetPanelName(PANEL_GetEepromPanelIndex()), prTbl->u2PanelWidth, prTbl->u2PanelHeight);
        }
        else
        {
            Printf("---------- Panel[%3d] %s %dx%d ----------\n",
                   i, GetPanelName(i), prTbl->u2PanelWidth, prTbl->u2PanelHeight);
        }
        if ((u4Flag & PANEL_DUMP_TITLE) == PANEL_DUMP_TITLE)
        {
            continue;
        }
        Printf("PixelClk[Max=%d, 60Hz=%d, 50Hz=%d, Min=%d]\n",
               prTbl->u4PixelClkMax, prTbl->u4PixelClk60Hz,
               prTbl->u4PixelClk50Hz, prTbl->u4PixelClkMin);
        Printf("HTotal[Max=%d, 60Hz=%d, 50Hz=%d, Min=%d] HSyncLen[60Hz=%d, 50Hz=%d]\n",
               prTbl->u2HTotalMax, prTbl->u2HTotal60Hz,
               prTbl->u2HTotal50Hz, prTbl->u2HTotalMin,
               PANEL_GetHSyncLen60Hz(), PANEL_GetHSyncLen50Hz());
        Printf("VTotal[Max=%d, 60Hz=%d, 50Hz=%d, Min=%d] VClk[Max=%d, Min=%d]\n",
               prTbl->u2VTotalMax, prTbl->u2VTotal60Hz,
               prTbl->u2VTotal50Hz, prTbl->u2VTotalMin,
               prTbl->u1VClkMax, prTbl->u1VClkMin);
        #ifdef SUPPORT_PANEL_CUSTOMER_SPEC
        Printf("HSyncWidth60Hz=%d, ", PANEL_GetHSyncWidthByTiming(SV_DCLK_60HZ));
        Printf("HSyncWidth50Hz=%d, ", PANEL_GetHSyncWidthByTiming(SV_DCLK_50HZ));
        Printf("VSyncWidth60Hz=%d, ", PANEL_GetVSyncWidthByTiming(SV_DCLK_60HZ));
        Printf("VSyncWidth50Hz=%d\n", PANEL_GetVSyncWidthByTiming(SV_DCLK_50HZ));
        #else
        Printf("HSyncWidth=%d, ", prTbl->u1HSyncWidth);
        Printf("VSyncWidth=%d, ", prTbl->u1VSyncWidth);
        #endif
        Printf("HPosition60Hz=0x%X, ", H_POS_60HZ);
        Printf("HPosition50Hz=0x%X, ", H_POS_50HZ);
        #ifdef SUPPORT_PANEL_CUSTOMER_SPEC
        Printf("VPosition60Hz=%d, ", PANEL_GetVPositionByTiming(SV_DCLK_60HZ));
        Printf("VPosition50Hz=%d\n", PANEL_GetVPositionByTiming(SV_DCLK_50HZ));
        #else
        Printf("VPosition=%d\n", prTbl->u2VPosition);
        #endif        

        if (SUPPORT_PANEL_48HZ)
        {
	        Printf("-SUPPORT_48HZ ExtId[%d]- Pclk=%d, HTot48Hz=%d, VTot48Hz=%d\n"
	               "HsWid48Hz=%d, VsWid48Hz=%d, HPos48Hz=0x%X, VPos48Hz=0x%X\n",
	                PANEL_GetExtAttributeIndex(),
	                PANEL_GetPixelClk48Hz(), PANEL_GetHTotal48Hz(), PANEL_GetVTotal48Hz(),
	        #ifdef SUPPORT_PANEL_CUSTOMER_SPEC
	                PANEL_GetHSyncWidthByTiming(SV_DCLK_48HZ), 
	                PANEL_GetVSyncWidthByTiming(SV_DCLK_48HZ),
	                PANEL_GetHPositionByTiming(SV_DCLK_48HZ),
	                PANEL_GetVPositionByTiming(SV_DCLK_48HZ) 
	        #else
	                prTbl->u1HSyncWidth, prTbl->u1VSyncWidth,
	                H_POS_60HZ, prTbl->u2VPosition
	        #endif
	                );
        } 
		
        if (prTbl->u4ControlWord & LVDS_MSB_SW_ON)
        {
            Printf("LVDS_NS, ");
        }
        else
        {
            Printf("LVDS_JEIDA, ");
        }
        if (prTbl->u4ControlWord & LVDS_ODD_SW_ON)
        {
            Printf("LVDS_ODD_SW_ON, ");
        }
        else
        {
            Printf("LVDS_ODD_SW_OFF, ");
        }
        if (prTbl->u4ControlWord & DUAL_PORT)
        {
            Printf("DUAL_PORT, ");
        }
        else if (prTbl->u4ControlWord & FOUR_PORT)
        {
            Printf("FOUR_PORT, ");
        }
        else if (prTbl->u4ControlWord & OCTA_PORT)
        {
            Printf("OCTA_PORT, ");
        }		
        else
        {
            Printf("SINGLE_PORT, ");
        }

        if ((prTbl->u4ControlWord & DISP_BIT_MSK) == DISP_30BIT)
        {
            Printf("DISP_30BIT, ");
        }
        else if ((prTbl->u4ControlWord & DISP_BIT_MSK) == DISP_24BIT)
        {
            Printf("DISP_24BIT\n");
        }
        else
        {
            Printf("DISP_18BIT\n");
        }
        if (prTbl->u4ControlWord & WFB_PANEL_RESET_ON)
        {
            Printf("WFB_PANEL_RESET_ON, ");
        }
        else
        {
            Printf("WFB_PANEL_RESET_OFF, ");
        }
        if (prTbl->u4ControlWord & PWM_LOW_PANEL_BRIGHT)
        {
            Printf("PWM_LOW_PANEL_BRIGHT\n");
        }
        else
        {
            Printf("PWM_HIGH_PANEL_BRIGHT\n");
        }

        if(prTbl->u4ControlWord & LVDS_USE_INDEPENDENT_SETTING)
        {
		if (LVDS_A_INDEPENDENT_SETTING == 3)
        {
            Printf("LVDS_A_FROM_BO,");
        }
		else if (LVDS_A_INDEPENDENT_SETTING == 2)
        {
            Printf("LVDS_A_FROM_BE,");
        }
        else if (LVDS_A_INDEPENDENT_SETTING == 1)
        {
            Printf("LVDS_A_FROM_AO,");
        }
		else if (LVDS_A_INDEPENDENT_SETTING == 0)
		{
			Printf("LVDS_A_FROM_AE,");
		}

		if (LVDS_B_INDEPENDENT_SETTING == 3)
        {
            Printf("LVDS_B_FROM_BO,");
        }
        else if (LVDS_B_INDEPENDENT_SETTING == 2)
        {
            Printf("LVDS_B_FROM_BE,");
        }
        else if (LVDS_B_INDEPENDENT_SETTING == 1)
        {
            Printf("LVDS_B_FROM_AO,");
        }
		else if (LVDS_B_INDEPENDENT_SETTING == 0)
		{
			Printf("LVDS_B_FROM_AE,");
		}

		if (LVDS_C_INDEPENDENT_SETTING == 3)
        {
            Printf("LVDS_C_FROM_BO,");
        }
        else if (LVDS_C_INDEPENDENT_SETTING == 2)
        {
            Printf("LVDS_C_FROM_BE,");
        }
        else if (LVDS_C_INDEPENDENT_SETTING == 1)
        {
            Printf("LVDS_C_FROM_AO,");
        }
		else if (LVDS_C_INDEPENDENT_SETTING == 0)
		{
			Printf("LVDS_C_FROM_AE,");
		}

		if (LVDS_D_INDEPENDENT_SETTING == 3)
        {
            Printf("LVDS_D_FROM_BO,\n");
        }
        else if (LVDS_D_INDEPENDENT_SETTING == 2)
        {
            Printf("LVDS_D_FROM_BE,\n");
        }
        else if (LVDS_D_INDEPENDENT_SETTING == 1)
        {
            Printf("LVDS_D_FROM_AO,\n");
        }
		else if (LVDS_D_INDEPENDENT_SETTING == 0)
		{
			Printf("LVDS_D_FROM_AE,\n");
		}		
        }
		
        if(LVDS_DISP_3D==LVDS_DISP_3D_POLARIZED_LRLR)
        {
            Printf("3D_POLARIZED_LRLR, ");
        }
        else if(LVDS_DISP_3D==LVDS_DISP_3D_POLARIZED_RLRL)
        {
            Printf("3D_POLARIZED_RLRL, ");
        }
        else if(LVDS_DISP_3D==LVDS_DISP_3D_SHUTTER)
        {
            Printf("3D_SHUTTER, ");
        }
        else
        {
            Printf("3D_NA, ");
        }

        if(LVDS_DISP_SPLIT)
        {
            Printf("SPLIT_ON, ");
        }
        else
        {
            Printf("SPLIT_OFF, ");
        }

        if(IS_VBYONE)
        {
            Printf("VB1");
        }
        if(IS_MLVDS)
        {
            Printf("MLVDS");
        }
        if(IS_EPI)
        {
            Printf("EPI");
        }
        Printf("\n");

        if(IS_LVDS_HIGHSPEED)
        {
            Printf("LVDS_HIGHSPEED,");
        }

        if(IS_PANEL_L12R12)
        {
            Printf("PANEL_L12R12,");
        }		

        if(IS_PANEL_ASPECT_RATIO_21_9)
        {
            Printf("PANEL_ASPECT_RATIO_21_9,");
        }	

        if(IS_OD_ENABLE)
        {
            Printf("OD_ENABLE,");
        }	

        if(IS_FB_ENABLE)
        {
            Printf("FB_ENABLE,");
        }	

		Printf("\n");

		// Panel Table Control Word 2 START
		Printf("==CW2==\n");
		if (IS_DISPR_ENCODEL4)
            Printf("DISPR_ENCODEL4,");
		
		if (IS_DISABLE_PANEL_HVSYNC)
					Printf("DISABLE_PANEL_HVSYNC,");			

		if (IS_SUPPORT_VB1_GRAPH_MODE)
					Printf("SUPPORT_VB1_GRAPH_MODE,");		

		if (IS_FORCE_LVDS_ANA_2CH_ON)
					Printf("FORCE_LVDS_ANA_2CH_ON,");

		if (IS_FORCE_LVDS_ANA_4CH_ON)
					Printf("FORCE_LVDS_ANA_4CH_ON,");		

		if (IS_SEPARATE_OSD_AND_DATA)
					Printf("SEPARATE_OSD_AND_DATA,");	

		if (IS_DISPLAY_FREERUN)
					Printf("DISPLAY_FREERUN,");			

		if (IS_FORCE_LVDS_DIG_4CH_ON)
					Printf("FORCE_LVDS_DIG_4CH_ON,");		

		if (IS_PANEL_2D_N_3D_L12R12)
					Printf("PANEL_2D_N_3D_L12R12,");	

		if (IS_DISABLE_LVDS_LINEBUFFER)
					Printf("DISABLE_LVDS_LINEBUFFER,");		

		if (IS_DISP_4K2K_TYPE1)
					Printf("DISP_4K2K_TYPE_1,");	

		if (IS_DISP_4K2K_TYPE2)
					Printf("DISP_4K2K_TYPE_2,");

		if (IS_SUPPORT_OSD_INDEX)
					Printf("SUPPORT_OSD_INDEX,");	

		if (IS_EPI_4LANE)
					Printf("EPI_4LANE,");			

		if (IS_EPI_8LANE)
					Printf("EPI_8LANE,");			

		if (IS_STEP_CLOCK_CHG)
		   			Printf("STEP_CLOCK_CHG,"); 

		if (IS_DISPR_INVERSE)
		   			Printf("DISPR_INVERSE,"); 			

		if (IS_VB1_ALL_LANE_ON)
		   			Printf("VB1_ALL_LANE_ON,"); 			
		
		Printf("\n");		
		// Panel Table Control Word 2 End

        Printf("Backlight[High=0x%x, Middle=0x%x, Low=0x%x]\n",
               prTbl->u1BacklightHigh, prTbl->u1BacklightMiddle,
               prTbl->u1BacklightLow);
        Printf("DimFreq60Hz=%d, DimFreq50Hz=%d"
               "\n",
               prTbl->u2DimmingFrequency60Hz, prTbl->u2DimmingFrequency50Hz
               );
		if (SUPPORT_PANEL_48HZ)
		{
			Printf("DimFreq48Hz=%d\n", PANEL_GetDimmingFrequency48Hz());
		}
        Printf("Delay[LvdsOn=%d0, BacklightOn=%d0, BacklightOff=%d0, LvdsOff=%d0]\n",
               prTbl->u1LvdsOnDalay, prTbl->u1BacklightOnDelay,
               prTbl->u1BacklightOffDelay, prTbl->u1LvdsOffDalay);
        Printf("MaxOverscan=%d\n", prTbl->u2MaxOverscan);
    }
    Printf("\nCurrent panel setting is %s\n", GetCurrentPanelName());
    Printf("Backlight gpio=%d, turn on value=%d, ",
           DRVCUST_PanelGet(eBackLightGpio), DRVCUST_PanelGet(eBackLightOnVal));
    Printf("LVDS gpio=%d, turn on value=%d\n",
           DRVCUST_PanelGet(ePanelPowerGpio), DRVCUST_PanelGet(ePanelPowerOnVal));
    Printf("Pixel Shift=%d (%d x %d)\n", _fgPixelShiftEnable,
           PANEL_GetPanelWidth(), PANEL_GetPanelHeight());

}


UINT32 PANEL_TranslateEepromToFlashIndex(UINT32 u4EepromIndex)
{
    UINT32 u4Size, u4FlashIndex;
    UINT32* pu4Array;

    u4Size = DRVCUST_PanelGet(ePanelFlashTableSize);
    if ((pu4Array = (UINT32*)DRVCUST_PanelGet(ePanelEepromToFlashIndexMap)) !=  NULL)
    {
        for (u4FlashIndex = 0; u4FlashIndex < u4Size; u4FlashIndex++)
        {
            if (pu4Array[u4FlashIndex] == u4EepromIndex)
            {
                return u4FlashIndex;
            }
        }
        if (u4FlashIndex == u4Size)
        {
            return 0;
        }
    }
    return u4EepromIndex;
}

UINT32 PANEL_GetEepromPanelIndex(void)
{
    return _u4EepromPanelIndex;
}

#ifdef CC_ENABLE_MTK_MODEL_INDEX
// static variables 
static UINT32 _u4PanelIdFromModelIndex=0xFFFFFFFF;
static UINT16 _u4AQ_tableIndex=0;
static UINT16 _u4PQ_tableIndex=0;
static BOOL bIsmodelindex_init = FALSE;

void WriteModelIndex(UINT32 u4ModelIndex)
{
    UINT32 u4Value, i;
    QUERY_TYPE_T arKey[] = {eModelIndexOffset1, eModelIndexOffset2,
                            eModelIndexOffset3};
    UINT32 u4Size = sizeof(arKey) / sizeof(QUERY_TYPE_T);

    for (i = 0; i < u4Size; i++)
    {
        if (DRVCUST_PanelQuery(arKey[i], &u4Value) == 0)
        {
            if (EEPROM_Write((UINT64)u4Value, (UINT32)&u4ModelIndex, 1) != 0)
            {
                Printf("WriteModelIndexToEeprom fail, eeprom addr=0x%x\n", u4Value);
            }
        }
    }

    bIsmodelindex_init = FALSE;
}

#define IS_MODEL_INDEX_VALID(x) ((x) != 0xFF)
#define MODEL_INVALID (0xFF)
static UINT32 u4FinalIndex = MODEL_INVALID;

UINT32 GetModelIndexFromEEPROM(void)
{
    UINT8 au1Index[] = {MODEL_INVALID, MODEL_INVALID, MODEL_INVALID};
    QUERY_TYPE_T arKey[] = {eModelIndexOffset1, eModelIndexOffset2,
                            eModelIndexOffset3};
    UINT32 u4Size = sizeof(arKey) / sizeof(QUERY_TYPE_T);
    UINT32 u4Value, u4ValidCounter = 0, i, j;
    INT32 i4Ret;

#ifdef NOT_USE_EEP
    if (!bIsmodelindex_init)
    {
        // read configuration from eeprom
        for (i = 0; i < u4Size; i++)
        {
            if (DRVCUST_PanelQuery(arKey[i], &u4Value) == 0)
            {
                Printf("GetModelIndexFromEEPROM(): i=%d, EEPROM read offset 0x%x\n", i, u4Value);
                i4Ret = EEPROM_Read((UINT64)u4Value, (UINT32)&au1Index[i], 1);
                if ((i4Ret != 0) || IS_MODEL_INDEX_VALID(au1Index[i]))
                {
                    u4ValidCounter++;
                    Printf("GetModelIndexFromEEPROM(): Got value 0x%x\n", au1Index[i]);
                }
                else
                {
                    au1Index[i] = MODEL_INVALID;            
                }
            }
        }

        // decide valid model index
        if (u4ValidCounter == 1)
        {
            for (i = 0; i < u4Size; i++)
            {
                if (au1Index[i] != MODEL_INVALID)
                {
                    u4FinalIndex = au1Index[i];
                    break;
                }
            }
        }
        else if (u4ValidCounter >= 2)
        {
            for (i = 0; i < u4Size; i++)
            {
                if (au1Index[i] == MODEL_INVALID)
                {
                    continue;
                }
                for (j = 0; j < u4Size; j++)
                {
                    if ((i != j) && (au1Index[i] == au1Index[j]))
                    {
                        u4FinalIndex = au1Index[i];
                        break;
                    }
                }
                if (u4FinalIndex != MODEL_INVALID)
                {
                    break;
                }
            }
        }
        else if (u4ValidCounter == 0)
        {
            u4FinalIndex = 0;
            WriteModelIndex (u4FinalIndex);
        }

        // Here, u4FinalIndex is the EEPROM panel id value.
        if (u4FinalIndex == MODEL_INVALID)
        {
            Printf("Here, Model index is invalid, change to default 0\n");
            u4FinalIndex = 0;
        }    

        Printf("GetModelIndexFromEEPROM(): return final index %d\n", u4FinalIndex);
        bIsmodelindex_init = TRUE;
        return u4FinalIndex;
    }
    else  // init ok , return modelindex directly
#endif
    {
        return u4FinalIndex;
    }
}

extern void SRMFBM_SetConf(UINT32 conf);
#ifdef CC_SET_TUNER_TYPE_BY_MODEL_INDEX
extern void TunerSelectType(UINT8 TunerType);
#endif
extern void vSetSuperFlipMirrorConfig(UINT8 u1SuperCfgOnOff, UINT8 u1SuperFlip, UINT8 u1SuperMirror);

void GetModelIndexSetDriverTypes(void)
{
    UINT32 u4ModelIndex;

    // Set driver types before they are initialized
    u4ModelIndex = GetModelIndexFromEEPROM();
    
#if !defined(CC_UBOOT) && !defined(CC_MTK_LOADER) && !defined(CC_MTK_PRELOADER)
    // Set FBM config
    Printf("Set FBM config 0x%x\n", GetModelIndexFbmId(u4ModelIndex));
    SRMFBM_SetConf(GetModelIndexFbmId(u4ModelIndex));

    // Set Tuner type
    Printf("Set tuner type 0x%x\n", GetModelIndexTunerId(u4ModelIndex));
    #ifdef CC_SET_TUNER_TYPE_BY_MODEL_INDEX
    TunerSelectType((UINT8)GetModelIndexTunerId(u4ModelIndex));
    #endif
#endif // !defined(CC_UBOOT) && !defined(CC_MTK_LOADER) && !defined(CC_MTK_PRELOADER)

#if !defined(CC_UBOOT) && !defined(CC_MTK_PRELOADER)

    _u4AQ_tableIndex = GetModelIndexAQId(u4ModelIndex);
    _u4PQ_tableIndex = GetModelIndexPQId(u4ModelIndex);

    // Set Panel
    Printf("Set panel id %d\n", GetModelIndexPanelId(u4ModelIndex));
    _u4PanelIdFromModelIndex = GetModelIndexPanelId(u4ModelIndex);

    // Set Mirror/Flip  
    Printf("Set mirror/flip %d\n", GetModelIndexMirrorFlipOpt(u4ModelIndex));
    vSetSuperFlipMirrorConfig(1, GetModelIndexMirrorFlipOpt(u4ModelIndex) & 0x1, 
                              (GetModelIndexMirrorFlipOpt(u4ModelIndex) & 0x2) >> 1);
#endif // !defined(CC_UBOOT) && !defined(CC_MTK_PRELOADER)

    return ;
}

UINT32 GetCurrentAQIndex(void)
{
    return _u4AQ_tableIndex;
}

UINT32 GetCurrentPQIndex(void)
{
    return _u4PQ_tableIndex;
}
#else // CC_ENABLE_MTK_MODEL_INDEX not defined

void GetModelIndexSetDriverTypes(void)
{
}
#endif // CC_ENABLE_MTK_MODEL_INDEX

//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
 
void LoadPanelIndex(void)
{
#ifdef NOT_USE_EEP  //for a5lr remove eep function
static UINT8 _fgInit = 0 ;
UINT32 u4Value,i;
UINT32 u4FinalIndex = PANEL_INVALID;

#else

	UINT8 au1Index[] = {PANEL_INVALID, PANEL_INVALID, PANEL_INVALID};
    QUERY_TYPE_T arKey[] = {ePanelIndexOffset1, ePanelIndexOffset2,
                            ePanelIndexOffset3};
    UINT32 u4Size = sizeof(arKey) / sizeof(QUERY_TYPE_T);
    UINT32 u4Value, u4ValidCounter = 0, i, j;
    UINT32 u4FinalIndex = PANEL_INVALID, i4Ret;
    UINT32 u4WriteIdxToEeprom = 0;
    
#endif



    if(_fgInit)
    {
//        Printf("skip LoadPanelIndex\n");
        return  ;
    }
    _fgInit = 1 ;

#ifdef CC_ENABLE_MTK_MODEL_INDEX
    GetModelIndexSetDriverTypes();  // Get table for model index from EEPROM before selecting Panel
#endif

#ifdef NOT_USE_EEP  //for a5lr remove eep function
#else
    if ((DRVCUST_PanelQuery(ePanelIndexFromEeprom, &u4Value) == 0) &&
        (u4Value == 1))
    {
        _fgPanelIdInEeprom = TRUE;

        // hack u4TotalNumber to pass IS_PANEL_INDEX_INVALID
        if (DRVCUST_PanelGet(ePanelFlashTableSize) > 0)
        {
            u4TotalNumber = 0xff;
        }
        
#ifdef CC_ENABLE_MTK_MODEL_INDEX
        // check whether panel id is set by model index
        if (_u4PanelIdFromModelIndex != 0xFFFFFFFF)
        {
            u4ValidCounter = u4Size;
            for (i = 0; i < u4Size; i++)
            {
                au1Index[i] = _u4PanelIdFromModelIndex;
            }
        }
        else
        {
#endif
            // read configuration from eeprom
            for (i = 0; i < u4Size; i++)
            {
                if (DRVCUST_PanelQuery(arKey[i], &u4Value) == 0)
                {
                    i4Ret = EEPROM_Read((UINT64)u4Value, (UINT32)&au1Index[i], 1);
                    if ((i4Ret != 0) || IS_PANEL_INDEX_INVALID(au1Index[i]))
                    {
                        au1Index[i] = PANEL_INVALID;
                    }
                    else
                    {
                        u4ValidCounter++;
                    }
                }
            }
#ifdef CC_ENABLE_MTK_MODEL_INDEX
        }
#endif // CC_ENABLE_MTK_MODEL_INDEX

        // decide valid panel index
        if (u4ValidCounter == 1)
        {
            for (i = 0; i < u4Size; i++)
            {
                if (au1Index[i] != PANEL_INVALID)
                {
                    u4FinalIndex = au1Index[i];
                    break;
                }
            }
        }
        else if (u4ValidCounter >= 2)
        {
            for (i = 0; i < u4Size; i++)
            {
                if (au1Index[i] == PANEL_INVALID)
                {
                    continue;
                }
                for (j = 0; j < u4Size; j++)
                {
                    if ((i != j) && (au1Index[i] == au1Index[j]))
                    {
                        u4FinalIndex = au1Index[i];
                        break;
                    }
                }
                if (u4FinalIndex != PANEL_INVALID)
                {
                    break;
                }
            }
        }

        // Here, u4FinalIndex is the EEPROM panel id value.
        // Check this is use gpio number.
#ifdef PANEL_INDEX_FLAG_OFFSET
        u4Value = 0;
        i4Ret = EEPROM_Read((UINT64)PANEL_INDEX_MAGIC, (UINT32)&u4Value, 1);
        if (i4Ret == 0)
        {
            if ((u4Value & 0xff) != 0x5a)
            {
                u4Value = 0;
                EEPROM_Write((UINT64)PANEL_INDEX_FLAG_OFFSET, (UINT32)&u4Value, 1);
                u4Value = 0x5a;
                EEPROM_Write((UINT64)PANEL_INDEX_MAGIC, (UINT32)&u4Value, 1);
            }
        }

        u4Value = 0;    // default flag is not in eeprom (gpio).
        i4Ret = EEPROM_Read((UINT64)PANEL_INDEX_FLAG_OFFSET, (UINT32)&u4Value, 1);
        if (i4Ret != 0) { u4FinalIndex = 0; } // if eeprom failed, force gpio directly.
        if (u4Value != 1) { u4FinalIndex = 0; } // if eeprom flag is not 1, force gpio directly.
#endif /* PANEL_INDEX_FLAG_OFFSET */
        {
            _u4EepromPanelIndex = u4FinalIndex;
            if (u4FinalIndex != PANEL_INVALID)
            {
                Printf("Find panel index %d from EEPROM\n", u4FinalIndex);

                // check if we need to write panel idx back to EEPROM
                for (i = 0; i < u4Size; i++)
                {
                    if (DRVCUST_PanelQuery(arKey[i], &u4Value) == 0)
                    {
                        if (au1Index[i] != (UINT8)u4FinalIndex)
                        {
                            u4WriteIdxToEeprom = 1;
                            Printf("Recover panel index in EEPROM\n");
                        }
                    }
                }

                // 1. check if we need to load panel table from flash 
                // 2. check if we need to map EEPROM index to flash index
                // 3. Load panel table from flash
                if (DRVCUST_PanelGet(ePanelFlashTableSize) > 0)
                {
                    // if there is no mapping table, return the same index
                    // if can't find flash index from mapping table, return 0
                    u4FinalIndex = PANEL_TranslateEepromToFlashIndex(u4FinalIndex);
                    Printf("Translate panel index to %d\n", u4FinalIndex);

                    #ifdef LOAD_PANEL_FROM_FLASH_CUSTOM
                    if (!LOAD_PANEL_FROM_FLASH_CUSTOM(LOAD_PANEL_FROM_FLASH_BY_ID, &u4FinalIndex))
                    {
                            Printf("Load panel table from EEPROM failed\n");

                            // FIXME!
                            // FIXME!
                            // start self-diag.
                            SelectPanel(0);    

                    }
                    #else  // turnkey load panel table from flash
                    {
                        UINT32 offset = DRVCUST_PanelGet(ePanelFlashTableAddress)+
                        (u4FinalIndex * DRVCUST_PanelGet(ePanelFlashTableSize));
                        if (!_PanelLoadTableFromFlash(offset))
                        {
                            Printf("Load panel table from EEPROM failed\n");
                            SelectPanel(PANEL_INVALID);
                        }
                    }
                    #endif
                }
                else
                {
                    if (u4FinalIndex >= PANEL_TOTAL_NUMBER)
                    {
                        Printf("Panel index loaded from EEPROM is out of panel table size\n");
                        u4FinalIndex = PANEL_INVALID;
                    }
                    SelectPanel(u4FinalIndex);
                }
            }
            else
            {
                Printf("No valid index found from EEPROM\n");
                SelectPanel(PANEL_INVALID);
            }
            if (u4WriteIdxToEeprom)
            {
                WritePanelIndexToEeprom(_u4EepromPanelIndex);
            }
            _fgPanelIdInEeprom = TRUE;
        }
    }
    else
 #endif 
    if ((u4Value = DRVCUST_PanelGet(ePanelIndexFromGpioNum)) > 0)
    {
        UINT32* pu4Array = (UINT32*)DRVCUST_PanelGet(ePanelIndexFromGpioIdx);
        u4FinalIndex = 0;
        for (i = 0; i < u4Value; i++)
        {
            u4FinalIndex <<= 1;
            u4FinalIndex |= GPIO_Input(pu4Array[i]);
            //Printf("gpio[%d]=%d, u4FinalIndex=%d\n", i, pu4Array[i], u4FinalIndex);
        }
        pu4Array = (UINT32*)DRVCUST_PanelGet(ePanelIndexFromGpioMap);
        u4FinalIndex = pu4Array[u4FinalIndex];
        if (IS_PANEL_INDEX_INVALID(u4FinalIndex))
        {
            Printf("No valid index found from GPIO\n");
            SelectPanel(PANEL_INVALID);
        }
        else
        {
            SelectPanel(u4FinalIndex);
            Printf("Find panel index %d(%s) from GPIO\n",
                   u4FinalIndex, GetCurrentPanelName());
        }
        _fgPanelIdInEeprom = FALSE;
    }
    else
    {
        // use compile time defined panel index
        SelectPanel(PANEL_INVALID);
        _fgPanelIdInEeprom = FALSE;
    }
}


//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------

void WritePanelIndexToEeprom(UINT32 u4PanelIndex)
{
    UINT32 u4Value, i;
    QUERY_TYPE_T arKey[] = {ePanelIndexOffset1, ePanelIndexOffset2,
                            ePanelIndexOffset3};
    UINT32 u4Size = sizeof(arKey) / sizeof(QUERY_TYPE_T);

    if ((DRVCUST_PanelQuery(ePanelIndexFromEeprom, &u4Value) == 0) &&
        (u4Value == 1))
    {
        for (i = 0; i < u4Size; i++)
        {
            if (DRVCUST_PanelQuery(arKey[i], &u4Value) == 0)
            {
                if (EEPROM_Write((UINT64)u4Value, (UINT32)&u4PanelIndex, 1) != 0)
                {
                    Printf("WritePanelIndexToEeprom fail, eeprom addr=0x%x\n",
                           u4Value);
                }
            }
        }
    }
}


//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------

UINT32 PANEL_GetBacklightHigh(void)
{
    UINT32 value;
    if (DRVCUST_PanelQuery(eBacklightHigh, &value) == 0)
    {
        return value;
    }

    return _prPanelAttribute->u1BacklightHigh;
}


//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------

UINT32 PANEL_GetBacklightMiddle(void)
{
    UINT32 value;
    if (DRVCUST_PanelQuery(eBacklightMiddle, &value) == 0)
    {
        return value;
    }
    return _prPanelAttribute->u1BacklightMiddle;
}


//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------

UINT32 PANEL_GetBacklightLow(void)
{
    UINT32 value;
    if (DRVCUST_PanelQuery(eBacklightLow, &value) == 0)
    {
        return value;
    }
    return _prPanelAttribute->u1BacklightLow;
}

#if SCALER_PSCAN_DISPMODE_MIN_VTOTAL_OFFSET 
UINT32 PANEL_GetVTotal60Hz(void)
{
    if(_prPanelAttribute->u2VTotal60Hz 
        < (PANEL_GetPanelHeight() + SCALER_PSCAN_DISPMODE_MIN_VTOTAL_OFFSET))
    {
        return (PANEL_GetPanelHeight() + SCALER_PSCAN_DISPMODE_MIN_VTOTAL_OFFSET);
    }
    else
    {
        return _prPanelAttribute->u2VTotal60Hz;        
    }
}

UINT32 PANEL_GetPixelClk60Hz(void)
{
    UINT32 u4Clk;
    UINT32 u4VTotal;
    UINT32 u4HTotal;
    
    u4VTotal = PANEL_GetVTotal60Hz();
    u4HTotal = PANEL_GetHTotal60Hz();
    
    if((_prPanelAttribute->u2VTotal60Hz) < u4VTotal)
    {
        u4Clk = u4HTotal * u4VTotal * 60;
        if (fgIsSupport120HzPanel())
        {
            u4Clk <<= 1;
        }
    }
    else
    {
        u4Clk = _prPanelAttribute->u4PixelClk60Hz;
    }
    
    return u4Clk;
}

UINT32 PANEL_GetVTotal50Hz(void)
{
    if(_prPanelAttribute->u2VTotal50Hz 
        < (PANEL_GetPanelHeight() + SCALER_PSCAN_DISPMODE_MIN_VTOTAL_OFFSET))
    {
        return (PANEL_GetPanelHeight() + SCALER_PSCAN_DISPMODE_MIN_VTOTAL_OFFSET);
    }
    else
    {
        return _prPanelAttribute->u2VTotal50Hz;        
    }
}

UINT32 PANEL_GetPixelClk50Hz(void)
{
    UINT32 u4Clk;
    UINT32 u4VTotal;
    UINT32 u4HTotal;
    
    u4VTotal = PANEL_GetVTotal50Hz();
    u4HTotal = PANEL_GetHTotal50Hz();
    
    if((_prPanelAttribute->u2VTotal50Hz) < u4VTotal)
    {
        u4Clk = u4HTotal * u4VTotal * 50;
        if (fgIsSupport120HzPanel())
        {
            u4Clk <<= 1;
        }
    }
    else
    {
        u4Clk = _prPanelAttribute->u4PixelClk50Hz;
    }
    
    return u4Clk;
}
#endif

UINT32 PANEL_GetHSyncLen60Hz(void)
{
    UINT32 u4Len = 0;

#if 0
    if (_fgPixelShiftEnable)
    {
        // leave right mask as (PIXEL_SHIFT_H_MAX + 1) - _i4HPixelShift
        u4Len = (PANEL_GetHTotal60Hz() - PANEL_GetPanelWidth()) -
                ((PIXEL_SHIFT_H_MAX + 1) - _i4HPixelShift);
    }
    else
    {
        // leave right mask as PIXEL_SHIFT_H_MAX*2 + 1
        u4Len = (PANEL_GetHTotal60Hz() - PANEL_GetPanelWidth()) -
                (PIXEL_SHIFT_H_MAX * 2 + 1);
    }
#else
//    u4Len = (PANEL_GetHTotal60Hz() - _prPanelAttribute->u2PanelWidth) - (PIXEL_SHIFT_H_MAX * 2 + RIGHT_MASK_TYPI);
    u4Len = (PANEL_GetHTotal60Hz() - _prPanelAttribute->u2PanelWidth) - (PIXEL_SHIFT_H_MAX * 2);
#endif

    // protect overflow
    return (u4Len > 2047)?2047:u4Len; // 5360B is 511.
//    return (u4Len > 511)?511:u4Len;
}

UINT32 PANEL_GetHSyncLen50Hz(void)
{
    UINT32 u4Len = 0;

#if 0
    if (_fgPixelShiftEnable)
    {
        // leave right mask as (PIXEL_SHIFT_H_MAX + 1) - _i4HPixelShift
        u4Len = (PANEL_GetHTotal50Hz() - PANEL_GetPanelWidth()) -
                ((PIXEL_SHIFT_H_MAX + 1) - _i4HPixelShift);
    }
    else
    {
        // leave right mask as PIXEL_SHIFT_H_MAX*2 + 1
        u4Len = (PANEL_GetHTotal50Hz() - PANEL_GetPanelWidth()) -
                (PIXEL_SHIFT_H_MAX * 2 + 1);
    }
#else
//    u4Len = (PANEL_GetHTotal50Hz() - _prPanelAttribute->u2PanelWidth) - (PIXEL_SHIFT_H_MAX * 2 + RIGHT_MASK_50HZ);
    u4Len = (PANEL_GetHTotal50Hz() - _prPanelAttribute->u2PanelWidth) - (PIXEL_SHIFT_H_MAX * 2);
#endif

    // protect overflow
//    return (u4Len > 2047)?2047:u4Len; // 5360B is 511.
    //return (u4Len > 511)?511:u4Len;
    return (u4Len > 2047)?2047:u4Len; //5387 is 11 bits
}

//#ifndef SUPPORT_PANEL_CUSTOMER_SPEC
UINT32 PANEL_GetHPosition60Hz(void)
{
    if (_fgPixelShiftEnable)
    {
        return (UINT32)((INT32)(_prPanelAttribute->u2HPosition + PIXEL_SHIFT_H_MAX) + (_i4HPixelShift));
    }
    else
    {
        return _prPanelAttribute->u2HPosition;
    }
}

UINT32 PANEL_GetHPosition50Hz(void)
{
    return ((PANEL_GetHPosition60Hz() + PANEL_GetHTotal50Hz()) - PANEL_GetHTotal60Hz());
}

UINT32 PANEL_GetVPosition(void)
{
    if (_fgPixelShiftEnable)
    {
        return (UINT32)((INT32)(_prPanelAttribute->u2VPosition + PIXEL_SHIFT_V_MAX) + _i4VPixelShift);
    }
    else
    {
        return _prPanelAttribute->u2VPosition;
    }
}
//#endif  //SUPPORT_PANEL_CUSTOMER_SPEC

UINT32 PANEL_GetPanelWidth(void)
{
#if 1 // customers' spec
    return _prPanelAttribute->u2PanelWidth + (PIXEL_SHIFT_H_MAX * 2);
#else
    if (_fgPixelShiftEnable)
    {
        return _prPanelAttribute->u2PanelWidth + (PIXEL_SHIFT_H_MAX * 2);
    }
    else
    {
        return _prPanelAttribute->u2PanelWidth;
    }
#endif
}

UINT32 PANEL_GetPanelHeight(void)
{
#if 1 // customers' spec
    return _prPanelAttribute->u2PanelHeight + (PIXEL_SHIFT_V_MAX * 2);
#else
    if (_fgPixelShiftEnable)
    {
        return _prPanelAttribute->u2PanelHeight + (PIXEL_SHIFT_V_MAX * 2);
    }
    else
    {
        return _prPanelAttribute->u2PanelHeight;
    }
#endif
}

UINT32 PANEL_GetPanelPhysicalWidth(void)
{
	if(IS_SUPPORT_4K2K_DISPLAY)
    return 3840;
	else
    return _prPanelAttribute->u2PanelWidth;
}

UINT32 PANEL_GetPanelPhysicalHeight(void)
{
	if(IS_SUPPORT_4K2K_DISPLAY)
    return 2160;
	else
    return _prPanelAttribute->u2PanelHeight;
}

UINT32 PANEL_GetPixelShiftHMax(void)
{
    UINT32 u4Value;
    if (DRVCUST_PanelQuery(ePanelPixelShiftHMax, &u4Value) != 0)
    {
        u4Value = 0;
    }
    return u4Value;
}

UINT32 PANEL_GetPixelShiftVMax(void)
{
    UINT32 u4Value;
    if (DRVCUST_PanelQuery(ePanelPixelShiftVMax, &u4Value) != 0)
    {
        u4Value = 0;
    }
    return u4Value;
}

void PANEL_SetPixelShift(BOOL fgEnable, INT32 i4HPixelShift, INT32 i4VPixelShift)
{
    _fgPixelShiftEnable = fgEnable;
    _i4HPixelShift = i4HPixelShift;
    _i4VPixelShift = i4VPixelShift;
}

BOOL PANEL_IsPixelShift(void)
{
    return _fgPixelShiftEnable;
}

INT32 PANEL_GetPixelShift_H(void)
{
    return _i4HPixelShift;
}

INT32 PANEL_GetPixelShift_V(void)
{
    return _i4VPixelShift;
}

CHAR * PANEL_GetVersion(void)
{
    #ifdef LOAD_PANEL_FROM_FLASH_CUSTOM
    return FlashPanel_GetVersion();
    #else
    return "Unknow";
    #endif
}

UINT32 PANEL_GetSpreadSpectrum(UINT32 *pu4Freq, UINT32 *pu4Permillage)
{
    if ((pu4Freq == NULL) || (pu4Permillage == NULL))
    {
        return 1;
    }
    #ifdef SUPPORT_PANEL_SS
    {
        UINT32 u4ExtIdx = PANEL_GetExtAttributeIndex();
        if (u4ExtIdx >= EXT_PANEL_INDEX_INVALID)
        {           
            *pu4Freq = 30000;
            *pu4Permillage = 0; 
            return 1;
        }
        *pu4Freq = _arPanelAttributeExt[u4ExtIdx].u4SSFreq;
        *pu4Permillage = (UINT32)_arPanelAttributeExt[u4ExtIdx].u1SSPermillage;
    }
    #else
    *pu4Freq = 30000;
    #ifdef CC_MTK_LOADER
    *pu4Permillage = 0;
    #else
    *pu4Permillage = DRVCUST_OptGet(eSpreadSpectrumPermillage);
    #endif
    #endif
    return 0;
}

UINT32 PANEL_GetDither(void)
{
    #ifdef SUPPORT_PANEL_DITHER
    {
        UINT32 u4ExtIdx = PANEL_GetExtAttributeIndex();
        if (u4ExtIdx >= EXT_PANEL_INDEX_INVALID)
        {   
            return 0xffffffff;
        }
        return (UINT32)_arPanelAttributeExt[u4ExtIdx].u4Dither;
    }
    #else
    return 0xffffffff;
    #endif
}

UINT32 PANEL_GetDrivingCurrent(void)
{
    UINT32 u4Value;
    #ifdef SUPPORT_PANEL_DRIVING
    UINT32 u4ExtIdx = PANEL_GetExtAttributeIndex();
    if (u4ExtIdx < EXT_PANEL_INDEX_INVALID)
    {   
        u4Value = (UINT32)_arPanelAttributeExt[u4ExtIdx].u1DrivingCurrent;
    }
    else
    #endif
    if (DRVCUST_PanelQuery(eLvdsDrivingCurrent, &u4Value) == -1)
    {
        u4Value = 0xffffffff;
    }
    return u4Value;
}

UINT16 PANEL_Get3DPixelSize(void)
{
    UINT32 u4Value;
    #ifdef SUPPORT_PANEL_3D_PIXEL_SIZE
    UINT32 u4ExtIdx = PANEL_GetExtAttributeIndex();
    if (u4ExtIdx < EXT_PANEL_INDEX_INVALID)
    {   
        u4Value = (UINT32)_arPanelAttributeExt[u4ExtIdx].u2PixelSize;
    }
    else
    #endif
    {
        u4Value = 320; // Pixel Size = 0.320 mm = 320 um
    }
    return u4Value;
}

UINT32 Panel_GetAQIndex(UINT32 u4Idx)
{
    #ifdef LOAD_PANEL_FROM_FLASH_CUSTOM
    return FlashPanel_GetAQIndex(u4Idx);
    #else
    return 0;
    #endif
}

UINT32 Panel_GetPQIndex(UINT32 u4Idx)
{
    #ifdef LOAD_PANEL_FROM_FLASH_CUSTOM
    return FlashPanel_GetPQIndex(u4Idx);
    #else
    return 0;
    #endif
}


// ------------------------------------------------------------------
//  Optional function
// ------------------------------------------------------------------
#if defined(SUPPORT_PANEL_CUSTOMER_SPEC) || defined(SUPPORT_PANEL_SS) || defined(SUPPORT_PANEL_DITHER) || defined(SUPPORT_PANEL_ERRORLIMIT_TABLE) || defined(SUPPORT_PANEL_3D_PIXEL_SIZE) || defined(SUPPORT_PANEL_SCAN_PWM) || defined(SUPPORT_PANEL_PRE_EMPHASIS)
static UINT32 PANEL_GetExtAttributeIndex(void)
{
    UINT32 u4ExtIdx = 0;
	UINT32 i ;

    if (DRVCUST_PanelGet(ePanelFlashTableSize) > 0)
    {   // load panel table from flash
        u4ExtIdx = 0;
    }
    else
    {
        u4ExtIdx = EXT_PANEL_INDEX_INVALID;
        for(i=0 ; i < sizeof(_arPanelAttributeExt)/sizeof(PANEL_ATTRIBUTE_EXT_T); i++)
        {
            if(GetCurrentPanelIndex() == _arPanelAttributeExt[i].u4PanelIndex)
            {
                u4ExtIdx = i;
                break;
            }
        }
    }
    return u4ExtIdx;
}


UINT32 PANEL_IsSupportExtAttribute(void)
{
    if(PANEL_GetExtAttributeIndex() >= EXT_PANEL_INDEX_INVALID)
    {
        return 0; 
    }
    return 1;
}

#endif

UINT32 PANEL_GetPixelClk48Hz(void)
{
    UINT32 u4ExtIdx = PANEL_GetExtAttributeIndex();
    UINT32 u4Clk;
#if SCALER_PSCAN_DISPMODE_MIN_VTOTAL_OFFSET 
    UINT32 u4VTotal;
#endif    
    if (u4ExtIdx >= EXT_PANEL_INDEX_INVALID)
    {
        return PANEL_GetPixelClk60Hz();
    }

#if SCALER_PSCAN_DISPMODE_MIN_VTOTAL_OFFSET 
    u4VTotal = PANEL_GetVTotal48Hz();
    
    if(_arPanelAttributeExt[u4ExtIdx].u2VTotal48Hz < u4VTotal)
    {
        u4Clk = PANEL_GetHTotal48Hz() * u4VTotal * 48;
        if (fgIsSupport120HzPanel())
        {
            u4Clk <<= 1;
        }
    }
    else
#endif
    {
        u4Clk = _arPanelAttributeExt[u4ExtIdx].u4PixelClk48Hz;
    }
    
    return u4Clk;
}

UINT32 PANEL_GetHTotal48Hz(void)
{
    UINT32 u4ExtIdx = PANEL_GetExtAttributeIndex();
    if (u4ExtIdx >= EXT_PANEL_INDEX_INVALID)
    {
        return PANEL_GetHTotal60Hz();
    }
    return _arPanelAttributeExt[u4ExtIdx].u2HTotal48Hz;
}

UINT32 PANEL_GetVTotal48Hz(void)
{
    UINT32 u4ExtIdx = PANEL_GetExtAttributeIndex();
    if (u4ExtIdx >= EXT_PANEL_INDEX_INVALID)
    {
        return PANEL_GetVTotal60Hz();
    }
#if SCALER_PSCAN_DISPMODE_MIN_VTOTAL_OFFSET 
    if(_arPanelAttributeExt[u4ExtIdx].u2VTotal48Hz 
        < (PANEL_GetPanelHeight() + SCALER_PSCAN_DISPMODE_MIN_VTOTAL_OFFSET))
    {
        return (PANEL_GetPanelHeight() + SCALER_PSCAN_DISPMODE_MIN_VTOTAL_OFFSET);
    }
    else
#endif
    {
        return _arPanelAttributeExt[u4ExtIdx].u2VTotal48Hz;
    }
}

UINT32 PANEL_GetHSyncLen48Hz(void)
{
    UINT32 u4Len;
    u4Len = (PANEL_GetHTotal48Hz() - PANEL_GetPanelWidth());

    // protect overflow
    return (u4Len > 2047)?2047:u4Len;
}

UINT32 PANEL_GetDimmingFrequency48Hz(void)
{
    UINT32 u4ExtIdx = PANEL_GetExtAttributeIndex();
    if (u4ExtIdx >= EXT_PANEL_INDEX_INVALID)
    {
        return PANEL_GetDimmingFrequency60Hz();
    }
    return (UINT32)_arPanelAttributeExt[u4ExtIdx].u2DimmingFrequency48Hz;
}

void PANEL_SetPixelClk48Hz(UINT32 u4Value)
{
    UINT32 u4ExtIdx = PANEL_GetExtAttributeIndex();
    if (u4ExtIdx >= EXT_PANEL_INDEX_INVALID)
    {
        return ;
    }
    _arPanelAttributeExt[u4ExtIdx].u4PixelClk48Hz = u4Value;
    return;
}

void PANEL_SetHTotal48Hz(UINT32 u4Value)
{
    UINT32 u4ExtIdx = PANEL_GetExtAttributeIndex();
    if (u4ExtIdx >= EXT_PANEL_INDEX_INVALID)
    {
        return;
    }
    _arPanelAttributeExt[u4ExtIdx].u2HTotal48Hz = (UINT16)u4Value;
    return;
}

void PANEL_SetVTotal48Hz(UINT32 u4Value)
{
    UINT32 u4ExtIdx = PANEL_GetExtAttributeIndex();
    if (u4ExtIdx >= EXT_PANEL_INDEX_INVALID)
    {
        return;
    }
    _arPanelAttributeExt[u4ExtIdx].u2VTotal48Hz = (UINT16)u4Value;
    return;
}
UINT32 PANEL_GetHPosition48Hz(void)
{
    return ((PANEL_GetHPosition60Hz() + PANEL_GetHTotal48Hz()) - PANEL_GetHTotal60Hz());
}

#ifdef SUPPORT_PANEL_CUSTOMER_SPEC
INT32 PANEL_GetHPositionByTiming(UINT8 bDclkType)
{
    UINT32 u4ExtIdx = PANEL_GetExtAttributeIndex();
    if ((u4ExtIdx >= EXT_PANEL_INDEX_INVALID) || (bDclkType >= 3))
    {
    	if (bDclkType==0 && SUPPORT_PANEL_48HZ)
			return PANEL_GetHPosition48Hz();
    	else if (bDclkType==1)
			return PANEL_GetHPosition50Hz();
		else	
        	return PANEL_GetHPosition60Hz();
    }
    return (_arPanelAttributeExt[u4ExtIdx].u2HFrontPorch[bDclkType]+0x800);
}

INT32 PANEL_GetVPositionByTiming(UINT8 bDclkType)
{
    UINT32 u4ExtIdx = PANEL_GetExtAttributeIndex();
    UINT16 u2VSyncWidth;
    UINT16 u2VBackPorch;
    UINT16 u2VSyncDelSel;

    if ((u4ExtIdx >= EXT_PANEL_INDEX_INVALID) || (bDclkType >= 3))
    {
        return PANEL_GetVPosition();
    }
    
    u2VSyncWidth = _arPanelAttributeExt[u4ExtIdx].u1VSyncWidth[bDclkType];
    u2VBackPorch = _arPanelAttributeExt[u4ExtIdx].u2VbackPorch[bDclkType];
    u2VSyncDelSel =5;

#if 0//CC_SCPOS_BOTTOM_MASK_BIT_WA
    #ifndef CC_5391_LOADER
    return (INT32)(0x400 -u2VSyncWidth -u2VBackPorch + u2BottomMaskToTop +u2VSyncDelSel);
    #else
    return (INT32)(0x400 -u2VSyncWidth -u2VBackPorch +u2VSyncDelSel);
    #endif
#else
    return (INT32)(0x400 -u2VSyncWidth -u2VBackPorch +u2VSyncDelSel);
#endif
}

UINT32 PANEL_GetHSyncWidthByTiming(UINT8 bDclkType)
{
    UINT32 u4ExtIdx = PANEL_GetExtAttributeIndex();
    if ((u4ExtIdx >= EXT_PANEL_INDEX_INVALID) || (bDclkType >= 3))
    {
        return PANEL_GetHSyncWidth();
    }
    return _arPanelAttributeExt[u4ExtIdx].u1HSyncWidth[bDclkType];
}

UINT32 PANEL_GetVSyncWidthByTiming(UINT8 bDclkType)
{
    UINT32 u4ExtIdx = PANEL_GetExtAttributeIndex();
    if ((u4ExtIdx >= EXT_PANEL_INDEX_INVALID) || (bDclkType >= 3))
    {
        return PANEL_GetVSyncWidth();
    }
    return _arPanelAttributeExt[u4ExtIdx].u1VSyncWidth[bDclkType];
}

void PANEL_SetVSyncWidthByTiming(UINT8 bDclkType, UINT16 u2VSyncWd)
{
    UINT32 u4ExtIdx = PANEL_GetExtAttributeIndex();
    if ((u4ExtIdx >= EXT_PANEL_INDEX_INVALID) || (bDclkType >= 3))
    {
        return;
    }
    _arPanelAttributeExt[u4ExtIdx].u1VSyncWidth[bDclkType]=u2VSyncWd;
}

void PANEL_SetHSyncWidthByTiming(UINT8 bDclkType, UINT16 u2HSyncWd)
{
    UINT32 u4ExtIdx = PANEL_GetExtAttributeIndex();
    if ((u4ExtIdx >= EXT_PANEL_INDEX_INVALID) || (bDclkType >= 3))
    {
        return;
    }
    _arPanelAttributeExt[u4ExtIdx].u1HSyncWidth[bDclkType]=u2HSyncWd;
}

UINT32 PANEL_GetVSyncBpByTiming(UINT8 bDclkType)
{
    UINT32 u4ExtIdx = PANEL_GetExtAttributeIndex();
    if ((u4ExtIdx >= EXT_PANEL_INDEX_INVALID) || (bDclkType >= 3))
    {
        return (V_POS < (0x400 - VS_WIDTH + 6))?((0x400 - V_POS - VS_WIDTH + 6)):(0);
    }
    return _arPanelAttributeExt[u4ExtIdx].u2VbackPorch[bDclkType];
}

void PANEL_SetVSyncBpByTiming(UINT8 bDclkType, UINT16 u2VSyncFP)
{
    UINT32 u4ExtIdx = PANEL_GetExtAttributeIndex();
    if ((u4ExtIdx >= EXT_PANEL_INDEX_INVALID) || (bDclkType >= 3))
    {
        return;
    }
    _arPanelAttributeExt[u4ExtIdx].u2VbackPorch[bDclkType]=u2VSyncFP;
}

UINT32 PANEL_GetHSyncFpByTiming(UINT8 bDclkType)
{
    UINT32 u4ExtIdx = PANEL_GetExtAttributeIndex();
    if ((u4ExtIdx >= EXT_PANEL_INDEX_INVALID) || (bDclkType >= 3))
    {
		return (H_POS_60HZ > 0x800)?(H_POS_60HZ-0x800):(0);
    }
    return _arPanelAttributeExt[u4ExtIdx].u2HFrontPorch[bDclkType];
}

void PANEL_SetHSyncFpByTiming(UINT8 bDclkType, UINT16 u2HSyncFP)
{
    UINT32 u4ExtIdx = PANEL_GetExtAttributeIndex();
    if ((u4ExtIdx >= EXT_PANEL_INDEX_INVALID) || (bDclkType >= 3))
    {
        return;
    }
    _arPanelAttributeExt[u4ExtIdx].u2HFrontPorch[bDclkType]=u2HSyncFP;
}

UINT32 PANEL_GetHSyncPolarity(void)
{
    UINT32 u4ExtIdx = PANEL_GetExtAttributeIndex();
    if (u4ExtIdx >= EXT_PANEL_INDEX_INVALID)
    {
        return HSYNC_LOW;
    }
    return _arPanelAttributeExt[u4ExtIdx].u1HSyncPolarity;
}

UINT32 PANEL_GetVSyncPolarity(void)
{
    UINT32 u4ExtIdx = PANEL_GetExtAttributeIndex();
    if (u4ExtIdx >= EXT_PANEL_INDEX_INVALID)
    {
        return VSYNC_LOW;
    }
    return _arPanelAttributeExt[u4ExtIdx].u1VSyncPolarity;
}

void PANEL_SetHSyncPolarity(UINT8 u1HsyncPol)
{
    UINT32 u4ExtIdx = PANEL_GetExtAttributeIndex();
    if (u4ExtIdx >= EXT_PANEL_INDEX_INVALID)
    {
        return;
    }
    _arPanelAttributeExt[u4ExtIdx].u1HSyncPolarity=u1HsyncPol;
}

void PANEL_SetVSyncPolarity(UINT8 u1VsyncPol)
{
    UINT32 u4ExtIdx = PANEL_GetExtAttributeIndex();
    if (u4ExtIdx >= EXT_PANEL_INDEX_INVALID)
    {
        return;
    }
    _arPanelAttributeExt[u4ExtIdx].u1VSyncPolarity=u1VsyncPol;
}

/*
//return num*10; for 120Hz panel
UINT32 PANEL_GetFrameDelay(void)
{    
    return 0;
}*/
#endif //#SUPPORT_PANEL_CUSTOMER_SPEC

#ifdef SUPPORT_PANEL_ERRORLIMIT_TABLE
UINT32 PANEL_GetErrorLimit(void)
{
    UINT32 u4ExtIdx = PANEL_GetExtAttributeIndex();

    if (u4ExtIdx >= EXT_PANEL_INDEX_INVALID)
    {
        return 0x3F;
    }    
    return (UINT32)_arPanelAttributeExt[u4ExtIdx].u4ErrorLimit;
}
#endif

#ifdef SUPPORT_PANEL_SCAN_PWM
UINT32 PANEL_GetScanPWMStart(void)
{
	UINT32 u4ExtIdx = PANEL_GetExtAttributeIndex();
    if (u4ExtIdx >= EXT_PANEL_INDEX_INVALID)
    {
        return (DEFAULT_SCAN_PWM_START);
    }
    return _arPanelAttributeExt[u4ExtIdx].u4ScanPWMStart;
}

UINT32 PANEL_GetScanPWMDuty(void)
{
	UINT32 u4ExtIdx = PANEL_GetExtAttributeIndex();
    if (u4ExtIdx >= EXT_PANEL_INDEX_INVALID)
    {
        return (DEFAULT_SCAN_PWM_DUTY);
    }
    return _arPanelAttributeExt[u4ExtIdx].u4ScanPWMDuty;
}
#endif

// ====== Local Dimming ============================

UINT32 PANEL_IsSupportLocalDimming(void)
{
    UINT32 u4PanelIdx;
    UINT32 i;

    u4PanelIdx = GetCurrentPanelIndex();

    for (i = 0; i < sizeof(_arPanelLDAttribute)/sizeof(PANEL_LOCAL_DIMMING_ATTRIBUTE_T); i++)
    {
        if (_arPanelLDAttribute[i].u4PanelIndex == u4PanelIdx)
        {
            return SV_TRUE;
        }
    }
    return SV_FALSE;
}


UINT32 PANEL_GetLocalDimmingParam(PANEL_LOCAL_DIMMING_ATTRIBUTE_T **pParam)
{
    UINT32 u4PanelIdx;
    UINT32 i;

    u4PanelIdx = GetCurrentPanelIndex();

    for (i = 0; i < sizeof(_arPanelLDAttribute)/sizeof(PANEL_LOCAL_DIMMING_ATTRIBUTE_T); i++)
    {
        if (_arPanelLDAttribute[i].u4PanelIndex == u4PanelIdx)
        {
            *pParam = &_arPanelLDAttribute[i];
            return SV_TRUE;
        }
    }
    *pParam = NULL;
    return SV_FALSE;
}

// ====== miniLVDS ============================
UINT32 PANEL_IsSupportMlvds(void)
{
    return ((PANEL_GetControlWord() & DISP_TYPE_MASK) == DISP_TYPE_MLVDS);
}


UINT32 PANEL_GetMlvdsParam(PANEL_MINILVDS_ATTRIBUTE_T **pParam)
{
    UINT32 u4PanelIdx;
    UINT32 i;

    if (DRVCUST_PanelGet(ePanelFlashTableSize) > 0)
    {   
        if(PANEL_IsSupportMlvds()) //Adam check
//        if((PANEL_GetControlWord() & DISP_TYPE_VB1) != 0) //Adam check
        {
            *pParam = &_arPanelMiniLVDSAttribute[0];
            return  SV_TRUE;
        }

    }
    else
    {
    u4PanelIdx = GetCurrentPanelIndex();

    for (i = 0; i < sizeof(_arPanelMiniLVDSAttribute)/sizeof(PANEL_MINILVDS_ATTRIBUTE_T); i++)
    {
        if (_arPanelMiniLVDSAttribute[i].u4PanelIndex == u4PanelIdx)
        {
            *pParam = &_arPanelMiniLVDSAttribute[i];
            return SV_TRUE;
        }
    }
    }
    *pParam = NULL;
    return SV_FALSE;
}

static UINT32 _u4ProtDisplayTypeMsk = 
#ifdef PANEL_DISPLAY_TYPE_ALLOWED_MASK
    PANEL_DISPLAY_TYPE_ALLOWED_MASK;
#else
    // defaultly set to lvds only mode
    (1<<PANEL_DISPLAY_TYPE_LVDS);
#endif

UINT32 PANEL_SetProtDisplayTypeMsk(UINT32 *u4ProtDisplayTypeMsk)
{
    if(u4ProtDisplayTypeMsk != NULL)
    {
        _u4ProtDisplayTypeMsk = *u4ProtDisplayTypeMsk; 
    }
    return _u4ProtDisplayTypeMsk;
}

// -- for multiple panel interface configuration --
UINT8 PANEL_IsSupportVb1(void)
{
    return ((PANEL_GetControlWord() & DISP_TYPE_MASK) == DISP_TYPE_VB1); 
}
UINT8 PANEL_IsSupportEpi(void)
{
    return ((PANEL_GetControlWord() & DISP_TYPE_MASK) == DISP_TYPE_EPI); 
}

UINT32 PANEL_GetDisplayType(void)
{
    UINT32 u4PanelDisplayType = PANEL_DISPLAY_TYPE_LVDS;

    if( PANEL_IsSupportMlvds() )
    {
        u4PanelDisplayType = PANEL_DISPLAY_TYPE_MLVDS;
    }
    if( PANEL_IsSupportVb1() )
    {
        // check if multiple panel interface is defined
        ASSERT(u4PanelDisplayType == PANEL_DISPLAY_TYPE_LVDS);

        u4PanelDisplayType = PANEL_DISPLAY_TYPE_VB1;
    }

//#if 0
    if( PANEL_IsSupportEpi() )
    {
        // check if multiple panel interface is defined
        ASSERT(u4PanelDisplayType == PANEL_DISPLAY_TYPE_LVDS);

        u4PanelDisplayType = PANEL_DISPLAY_TYPE_EPI;
    }
//#endif

    // set to allowed mode only if confirmed 
    if(0 == (_u4ProtDisplayTypeMsk & (1<<u4PanelDisplayType)))
    {
        return PANEL_DISPLAY_TYPE_MAX;
    } 

    return u4PanelDisplayType;
}

void PANEL_SetColorDepth(UINT8 u1BitRes)
{
    UINT32 u4ControlWord = PANEL_GetControlWord();

    /* clear the bits we used */
    u4ControlWord = u4ControlWord & (~DISP_BIT_MSK);

    if (u1BitRes == 0) // 8bit
    {
        u4ControlWord |= DISP_24BIT;   
    }
    else
    {
        u4ControlWord |= DISP_30BIT;   
    }
    if(PANEL_GetDisplayType()== PANEL_DISPLAY_TYPE_EPI)
    {
        u4ControlWord = u4ControlWord & (~DISP_BIT_MSK);
        u4ControlWord |= DISP_24BIT;   
    }
    PANEL_SetControlWord(u4ControlWord);
}

void PANEL_SetLvdsSelect(UINT8 u1Format)
{
    UINT32 u4ControlWord = PANEL_GetControlWord();

    if (u1Format == 0) // ns
    {
        u4ControlWord = (u4ControlWord & 0xFFFFFFFC) | LVDS_NS;
    }
    else if (u1Format == 1) // jeida
    {
        u4ControlWord = (u4ControlWord & 0xFFFFFFFC) | LVDS_JEIDA;
    }
	else
	{
        u4ControlWord = (u4ControlWord & 0xFFFFFFFC) | LVDS_SPECIAL_NS;
	}
	
    PANEL_SetControlWord(u4ControlWord);
//    vDrvLVDSConfig();
}

void PANEL_SetLvdsSwap(UINT8 u1Inverse)
{

    UINT32 u4ControlWord = PANEL_GetControlWord();

    u4ControlWord = u4ControlWord & (~LVDS_ODD_SW_ON);

    if (u1Inverse == 0)
    {
        u4ControlWord |= LVDS_ODD_SW_OFF;
    }
    else
    {
        u4ControlWord |= LVDS_ODD_SW_ON;
    }

    PANEL_SetControlWord(u4ControlWord);
	
}

void PANEL_SetLvdsPCBSwap(UINT8 u1Inverse)
{
	if(u1Inverse)
	fgLVDSPCBSwap = 1;	
	else
	fgLVDSPCBSwap = 0;			
}

void PANEL_SetLvdsPixel(UINT8 u1Mode)
{
    UINT32 u4ControlWord = PANEL_GetControlWord();
    
    /* clear the bits we used */
    u4ControlWord = u4ControlWord & (~(DUAL_PORT|FOUR_PORT));

    if (u1Mode == 0) // single
    {
        u4ControlWord = u4ControlWord | SINGLE_PORT;
        PANEL_SetVClkMax(78);
        PANEL_SetVClkMin(46);
    }
    else if(u1Mode == 1)// dual
    {
        u4ControlWord= u4ControlWord | DUAL_PORT;
		if (IS_PANEL_L12R12)
		{
        	PANEL_SetVClkMax(123);
        	PANEL_SetVClkMin(46);		
		}
		else
		{
			PANEL_SetVClkMax(78);
			PANEL_SetVClkMin(46);
		}
    }
    else
    {
        u4ControlWord= u4ControlWord | FOUR_PORT;
        PANEL_SetVClkMax(123);
        PANEL_SetVClkMin(46);
    }

    PANEL_SetControlWord(u4ControlWord);
}

void PANEL_SetResolution(UINT16 u2Width, UINT16 u2Height)
{
    PANEL_SetPanelWidth(u2Width);
    PANEL_SetPanelHeight(u2Height);
#ifndef CC_MTK_LOADER  
	if (IS_PANEL_L12R12)
	{             
    	vDrvODInit();
	}
#endif
}

void PANEL_SetHTotalTyp(UINT16 u2HTotal)
{
   if(PANEL_GetDisplayType()!= PANEL_DISPLAY_TYPE_EPI)
    {
    PANEL_SetHTotal60Hz(u2HTotal);
    PANEL_SetHTotal50Hz(u2HTotal);
	if (SUPPORT_PANEL_48HZ)
	{
		PANEL_SetHTotal48Hz(u2HTotal);
	}
    PANEL_SetHTotalMax(u2HTotal/10*12);
    PANEL_SetHTotalMin(u2HTotal/10*8);
    }
}

void PANEL_SetVTotal(UINT16 u2VTotal48Hz, UINT16 u2VTotal50Hz, UINT16 u2VTotal60Hz)
{
    UINT8 u1MultiFrameRate;

   if(PANEL_GetDisplayType()!= PANEL_DISPLAY_TYPE_EPI)
    {
		if (SUPPORT_PANEL_48HZ)
		{
    		_prPanelAttributeExt->u2VTotal48Hz = u2VTotal48Hz;
		}
	    PANEL_SetVTotal60Hz(u2VTotal60Hz);
	    PANEL_SetVTotal50Hz(u2VTotal50Hz);
	    PANEL_SetVTotalMax(u2VTotal48Hz/20*21);
	    PANEL_SetVTotalMin(u2VTotal60Hz/10*8);
    
        if (IS_PANEL_L12R12)
        {
            u1MultiFrameRate = 1;
        }
        else
        {
    		u1MultiFrameRate = ((PANEL_GetControlWord() & FOUR_PORT)!=0)?(2):(1); 
        }

	    // update pixel clock
	    PANEL_SetPixelClk50Hz(PANEL_GetHTotal60Hz() * u2VTotal50Hz * 50 *u1MultiFrameRate);
	    PANEL_SetPixelClk60Hz(PANEL_GetHTotal60Hz() * u2VTotal60Hz * 60 *u1MultiFrameRate);
		if (SUPPORT_PANEL_48HZ)
		{
	    	PANEL_SetPixelClk48Hz(PANEL_GetHTotal60Hz() * u2VTotal48Hz * 48 *u1MultiFrameRate);
		}
		//PANEL_SetPixelClkMax(PANEL_GetHTotal60Hz() * PANEL_GetVTotalMax()*60*u1MultiFrameRate);
	    PANEL_SetPixelClkMax(PANEL_GetHTotal60Hz() * PANEL_GetVTotalMax()*48*u1MultiFrameRate);
	    PANEL_SetPixelClkMin(PANEL_GetHTotal60Hz() * PANEL_GetVTotalMin()*48*u1MultiFrameRate);
    }
}

void PANEL_SetVertSyncWidth(UINT16 u2Vsync48Hz, UINT16 u2Vsync50Hz, UINT16 u2Vsync60Hz)
{
   if(PANEL_GetDisplayType()!= PANEL_DISPLAY_TYPE_EPI)
   {
#ifdef SUPPORT_PANEL_CUSTOMER_SPEC
    _prPanelAttributeExt->u1VSyncWidth[0] = u2Vsync48Hz;
    _prPanelAttributeExt->u1VSyncWidth[1] = u2Vsync50Hz;
    _prPanelAttributeExt->u1VSyncWidth[2] = u2Vsync60Hz;
#endif
    }
}

void PANEL_SetVertBackPorch(UINT16 u2VbackPorch48Hz, UINT16 u2VbackPorch50Hz, UINT16 u2VbackPorch60Hz)
{
   if(PANEL_GetDisplayType()!= PANEL_DISPLAY_TYPE_EPI)
   {
#ifdef SUPPORT_PANEL_CUSTOMER_SPEC
    _prPanelAttributeExt->u2VbackPorch[0] = u2VbackPorch48Hz;
    _prPanelAttributeExt->u2VbackPorch[1] = u2VbackPorch50Hz;
    _prPanelAttributeExt->u2VbackPorch[2] = u2VbackPorch60Hz;
#endif
    }
}

void PANEL_SetHorizSyncWidth(UINT16 u2Hsync48Hz, UINT16 u2Hsync50Hz, UINT16 u2Hsync60Hz)
{
   if(PANEL_GetDisplayType()!= PANEL_DISPLAY_TYPE_EPI)
   {
#ifdef SUPPORT_PANEL_CUSTOMER_SPEC
    _prPanelAttributeExt->u1HSyncWidth[0] = u2Hsync48Hz;
    _prPanelAttributeExt->u1HSyncWidth[1] = u2Hsync50Hz;
    _prPanelAttributeExt->u1HSyncWidth[2] = u2Hsync60Hz;
#endif
    }
}

void PANEL_SetHorizBackPorch(UINT16 u2HbackPorch48Hz, UINT16 u2HbackPorch50Hz, UINT16 u2HbackPorch60Hz)
{
   if(PANEL_GetDisplayType()!= PANEL_DISPLAY_TYPE_EPI)
   {
	#ifdef SUPPORT_PANEL_CUSTOMER_SPEC
		if (SUPPORT_PANEL_48HZ)
		{
			_prPanelAttributeExt->u2HFrontPorch[0] = HLEN_TOTAL_48HZ- wDISPLAY_WIDTH - u2HbackPorch48Hz - _prPanelAttributeExt->u1HSyncWidth[0];
		}
    	_prPanelAttributeExt->u2HFrontPorch[1] = HLEN_TOTAL_50HZ- wDISPLAY_WIDTH - u2HbackPorch50Hz - _prPanelAttributeExt->u1HSyncWidth[1];
    	_prPanelAttributeExt->u2HFrontPorch[2] = HLEN_TOTAL_TYPI- wDISPLAY_WIDTH - u2HbackPorch60Hz - _prPanelAttributeExt->u1HSyncWidth[2];
	#endif
    }
}

#if defined(SUPPORT_PANEL_GRAPH_MODE)
#define GRAPH_MODE_PANEL_INDEX_INVALID 0xffff
static GRAPHMODE_ATTRIBUTE_T* _prGraphModeAttribute = & _arGraphModeAttribute[0];

static UINT32 GraphMode_GetAttributeIndex(void)
{
    UINT32 u4GMIdx = 0;
	UINT32 i ;

    if (DRVCUST_PanelGet(ePanelFlashTableSize) > 0)
    {   // load panel table from flash
        u4GMIdx = 0;
    }
    else
    {
        u4GMIdx = GRAPH_MODE_PANEL_INDEX_INVALID;
        for(i=0 ; i < sizeof(_arGraphModeAttribute)/sizeof(GRAPHMODE_ATTRIBUTE_T); i++)
        {
            if(GetCurrentPanelIndex() == _arGraphModeAttribute[i].u4PanelIndex)
            {
                u4GMIdx = i;
                break;
            }
        }
    }
    return u4GMIdx;
}

UINT32 GraphMode_GetControlWord(void)
{
    UINT32 u4GMIdx = GraphMode_GetAttributeIndex();
    if (u4GMIdx >= GRAPH_MODE_PANEL_INDEX_INVALID)
    {
        return 0;
    }
    return _arGraphModeAttribute[u4GMIdx].u4GControlWord;
}

void GraphMode_SetControlWord(UINT32 u4ControlWord)
{
    UINT32 u4GMIdx = GraphMode_GetAttributeIndex();
    if (u4GMIdx >= GRAPH_MODE_PANEL_INDEX_INVALID)
    {
        return;
    }
    _arGraphModeAttribute[u4GMIdx].u4GControlWord=u4ControlWord;
}

UINT8 GraphMode_IsSupportAttribute(void)
{
    if(GraphMode_GetAttributeIndex() >= GRAPH_MODE_PANEL_INDEX_INVALID)
    {
        return 0; 
    }
    return 1;
}

UINT32 GRAPHMODE_GetPixelClkByTiming(UINT8 bDclkType)
{
    UINT32 u4GMIdx = GraphMode_GetAttributeIndex();
    if ((u4GMIdx >= GRAPH_MODE_PANEL_INDEX_INVALID) || (bDclkType >= 3))
    {
        return PANEL_GetPixelClk60Hz();
    }
    return _arGraphModeAttribute[u4GMIdx].au4PixelClk[bDclkType];
}

void GRAPHMODE_SetPixelClkByTiming(UINT8 bDclkType, UINT32 u4PixelClk)
{
	UINT32 u4GMIdx = GraphMode_GetAttributeIndex();
	Printf("u4GMIdx=%d\n", u4GMIdx);	
    if ((u4GMIdx >= GRAPH_MODE_PANEL_INDEX_INVALID) || (bDclkType >= 3))
    {
        return;
    }
	Printf("bDclkType=%d, u4PixelClk = %d \n", bDclkType, u4PixelClk);
    _arGraphModeAttribute[u4GMIdx].au4PixelClk[bDclkType] = u4PixelClk;
}

void GRAPHMODE_SetPixelClk(UINT32 u4PixelClk48Hz, UINT16 u4PixelClk50Hz, UINT16 u4PixelClk60Hz)
{
    GRAPHMODE_SetPixelClkByTiming(0,u4PixelClk48Hz);
    GRAPHMODE_SetPixelClkByTiming(1,u4PixelClk50Hz);
    GRAPHMODE_SetPixelClkByTiming(2,u4PixelClk60Hz);	
}

UINT16 GRAPHMODE_GetHtotalByTiming(UINT8 bDclkType)
{
    UINT32 u4GMIdx = GraphMode_GetAttributeIndex();
    if ((u4GMIdx >= GRAPH_MODE_PANEL_INDEX_INVALID) || (bDclkType >= 3))
    {
        return PANEL_GetHTotal60Hz();
    }
    return _arGraphModeAttribute[u4GMIdx].au2HTotal[bDclkType];
}

void GRAPHMODE_SetHtotalByTiming(UINT8 bDclkType, UINT16 u2Htotal)
{
	UINT32 u4GMIdx = GraphMode_GetAttributeIndex();
    if ((u4GMIdx >= GRAPH_MODE_PANEL_INDEX_INVALID) || (bDclkType >= 3))
    {
        return;
    }
    _arGraphModeAttribute[u4GMIdx].au2HTotal[bDclkType] = u2Htotal;
}

void GRAPHMODE_SetHtotal(UINT16 u2Htotal48Hz, UINT16 u2Htotal50Hz, UINT16 u2Htotal60Hz)
{
    GRAPHMODE_SetHtotalByTiming(0,u2Htotal48Hz);
    GRAPHMODE_SetHtotalByTiming(1,u2Htotal50Hz);
    GRAPHMODE_SetHtotalByTiming(2,u2Htotal60Hz);	
}

UINT32 GRAPHMODE_GetPanelWidth(void)
{
    return _prGraphModeAttribute->u2GraphModeWidth;
}

void GRAPHMODE_SetPanelWidth(UINT16 u2PanelWidth)
{
    _prGraphModeAttribute->u2GraphModeWidth = u2PanelWidth;
}

UINT16 GRAPHMODE_GetHSyncFpByTiming(UINT8 bDclkType)
{
    UINT32 u4GMIdx = GraphMode_GetAttributeIndex();
    if ((u4GMIdx >= GRAPH_MODE_PANEL_INDEX_INVALID) || (bDclkType >= 3))
    {
        return 0;
    }
    return _arGraphModeAttribute[u4GMIdx].au2HFrontPorch[bDclkType];
}

void GRAPHMODE_SetHSyncFpByTiming(UINT8 bDclkType, UINT16 u2HFp)
{
	UINT32 u4GMIdx = GraphMode_GetAttributeIndex();
    if ((u4GMIdx >= GRAPH_MODE_PANEL_INDEX_INVALID) || (bDclkType >= 3))
    {
        return;
    }
    _arGraphModeAttribute[u4GMIdx].au2HFrontPorch[bDclkType] = u2HFp;
}

void GRAPHMODE_SetHSyncFp(UINT16 u2HFp48Hz, UINT16 u2HFp50Hz, UINT16 u2HFp60Hz)
{
    GRAPHMODE_SetHSyncFpByTiming(0,u2HFp48Hz);
    GRAPHMODE_SetHSyncFpByTiming(1,u2HFp50Hz);
    GRAPHMODE_SetHSyncFpByTiming(2,u2HFp60Hz);
}

UINT8 GRAPHMODE_GetHSyncWidthByTiming(UINT8 bDclkType)
{
    UINT32 u4GMIdx = GraphMode_GetAttributeIndex();
    if ((u4GMIdx >= GRAPH_MODE_PANEL_INDEX_INVALID) || (bDclkType >= 3))
    {
        return PANEL_GetHSyncWidth();
    }
    return _arGraphModeAttribute[u4GMIdx].au1HSyncWidth[bDclkType];
}

void GRAPHMODE_SetHSyncWidthByTiming(UINT8 bDclkType, UINT16 u2HWidth)
{
	UINT32 u4GMIdx = GraphMode_GetAttributeIndex();
    if ((u4GMIdx >= GRAPH_MODE_PANEL_INDEX_INVALID) || (bDclkType >= 3))
    {
        return;
    }
    _arGraphModeAttribute[u4GMIdx].au1HSyncWidth[bDclkType] = u2HWidth;
}

void GRAPHMODE_SetHSyncWidth(UINT16 u2HWidth48Hz, UINT16 u2HWidth50Hz, UINT16 u2HWidth60Hz)
{
    GRAPHMODE_SetHSyncFpByTiming(0,u2HWidth48Hz);
    GRAPHMODE_SetHSyncFpByTiming(1,u2HWidth50Hz);
    GRAPHMODE_SetHSyncFpByTiming(2,u2HWidth60Hz);
}

UINT16 GRAPHMODE_GetHSyncBpByTiming(UINT8 bDclkType)
{
    UINT32 u4GMIdx = GraphMode_GetAttributeIndex();
    if ((u4GMIdx >= GRAPH_MODE_PANEL_INDEX_INVALID) || (bDclkType >= 3))
    {
        return 0;
    }
    return _arGraphModeAttribute[u4GMIdx].au2HBackPorch[bDclkType];
}

void GRAPHMODE_SetHSyncBpByTiming(UINT8 bDclkType, UINT16 u2HBp)
{
	UINT32 u4GMIdx = GraphMode_GetAttributeIndex();
    if ((u4GMIdx >= GRAPH_MODE_PANEL_INDEX_INVALID) || (bDclkType >= 3))
    {
        return;
    }
    _arGraphModeAttribute[u4GMIdx].au2HBackPorch[bDclkType] = u2HBp;
}

void GRAPHMODE_SetHSyncBp(UINT16 u2HBp48Hz, UINT16 u2HBp50Hz, UINT16 u2HBp60Hz)
{
    GRAPHMODE_SetHSyncFpByTiming(0,u2HBp48Hz);
    GRAPHMODE_SetHSyncFpByTiming(1,u2HBp50Hz);
    GRAPHMODE_SetHSyncFpByTiming(2,u2HBp60Hz);
}

UINT16 GRAPHMODE_GetVtotalByTiming(UINT8 bDclkType)
{
    UINT32 u4GMIdx = GraphMode_GetAttributeIndex();
    if ((u4GMIdx >= GRAPH_MODE_PANEL_INDEX_INVALID) || (bDclkType >= 3))
    {
        return PANEL_GetVTotal60Hz();
    }
    return _arGraphModeAttribute[u4GMIdx].au2VTotal[bDclkType];
}

void GRAPHMODE_SetVtotalByTiming(UINT8 bDclkType, UINT16 u2Vtotal)
{
	UINT32 u4GMIdx = GraphMode_GetAttributeIndex();
    if ((u4GMIdx >= GRAPH_MODE_PANEL_INDEX_INVALID) || (bDclkType >= 3))
    {
        return;
    }
    _arGraphModeAttribute[u4GMIdx].au2VTotal[bDclkType] = u2Vtotal;
}

void GRAPHMODE_SetVtotal(UINT16 u2Vtotal48Hz, UINT16 u2Vtotal50Hz, UINT16 u2Vtotal60Hz)
{
    GRAPHMODE_SetVtotalByTiming(0,u2Vtotal48Hz);
    GRAPHMODE_SetVtotalByTiming(1,u2Vtotal50Hz);
    GRAPHMODE_SetVtotalByTiming(2,u2Vtotal60Hz);
}

UINT32 GRAPHMODE_GetPanelHeight(void)
{
    return _prGraphModeAttribute->u2GraphModeHight;
}

void GRAPHMODE_SetPanelHeight(UINT16 u2PanelHeight)
{
    _prGraphModeAttribute->u2GraphModeHight = u2PanelHeight;
}

UINT16 GRAPHMODE_GetVSyncFpByTiming(UINT8 bDclkType)
{
    UINT32 u4GMIdx = GraphMode_GetAttributeIndex();
    if ((u4GMIdx >= GRAPH_MODE_PANEL_INDEX_INVALID) || (bDclkType >= 3))
    {
        return 0;
    }
    return _arGraphModeAttribute[u4GMIdx].au2VFrontPorch[bDclkType];
}

void GRAPHMODE_SetVSyncFpByTiming(UINT8 bDclkType, UINT16 u2VFp)
{
	UINT32 u4GMIdx = GraphMode_GetAttributeIndex();
    if ((u4GMIdx >= GRAPH_MODE_PANEL_INDEX_INVALID) || (bDclkType >= 3))
    {
        return;
    }
    _arGraphModeAttribute[u4GMIdx].au2VFrontPorch[bDclkType] = u2VFp;
}

void GRAPHMODE_SetVSyncFp(UINT16 u2VFp48Hz, UINT16 u2VFp50Hz, UINT16 u2VFp60Hz)
{
    GRAPHMODE_SetVSyncFpByTiming(0,u2VFp48Hz);
    GRAPHMODE_SetVSyncFpByTiming(1,u2VFp50Hz);
    GRAPHMODE_SetVSyncFpByTiming(2,u2VFp60Hz);
}

UINT8 GRAPHMODE_GetVSyncWidthByTiming(UINT8 bDclkType)
{
    UINT32 u4GMIdx = GraphMode_GetAttributeIndex();
    if ((u4GMIdx >= GRAPH_MODE_PANEL_INDEX_INVALID) || (bDclkType >= 3))
    {
        return PANEL_GetVSyncWidth();
    }
    return _arGraphModeAttribute[u4GMIdx].au1VSyncWidth[bDclkType];
}

void GRAPHMODE_SetVSyncWidthByTiming(UINT8 bDclkType, UINT16 u2VWidth)
{
	UINT32 u4GMIdx = GraphMode_GetAttributeIndex();
    if ((u4GMIdx >= GRAPH_MODE_PANEL_INDEX_INVALID) || (bDclkType >= 3))
    {
        return;
    }
    _arGraphModeAttribute[u4GMIdx].au1VSyncWidth[bDclkType] = u2VWidth;
}

void GRAPHMODE_SetVSyncWidth(UINT8 u1VWidth48Hz, UINT8 u1VWidth50Hz, UINT8 u1VWidth60Hz)
{
    GRAPHMODE_SetVSyncWidthByTiming(0,u1VWidth48Hz);
    GRAPHMODE_SetVSyncWidthByTiming(1,u1VWidth50Hz);
    GRAPHMODE_SetVSyncWidthByTiming(2,u1VWidth60Hz);
}

UINT16 GRAPHMODE_GetVSyncBpByTiming(UINT8 bDclkType)
{
    UINT32 u4GMIdx = GraphMode_GetAttributeIndex();
    if ((u4GMIdx >= GRAPH_MODE_PANEL_INDEX_INVALID) || (bDclkType >= 3))
    {
        return 0;
    }
    return _arGraphModeAttribute[u4GMIdx].au2VBackPorch[bDclkType];
}

void GRAPHMODE_SetVSyncBpByTiming(UINT8 bDclkType, UINT16 u2VBp)
{
	UINT32 u4GMIdx = GraphMode_GetAttributeIndex();
    if ((u4GMIdx >= GRAPH_MODE_PANEL_INDEX_INVALID) || (bDclkType >= 3))
    {
        return;
    }
    _arGraphModeAttribute[u4GMIdx].au2VBackPorch[bDclkType] = u2VBp;
}

void GRAPHMODE_SetVSyncBp(UINT16 u2VBp48Hz, UINT16 u2VBp50Hz, UINT16 u2VBp60Hz)
{
    GRAPHMODE_SetVSyncBpByTiming(0,u2VBp48Hz);
    GRAPHMODE_SetVSyncBpByTiming(1,u2VBp50Hz);
    GRAPHMODE_SetVSyncBpByTiming(2,u2VBp60Hz);
}

void DumpGraphModeAttribute(void)
{
	UINT8 u1Cnt = 0;
	
	if(PANEL_GetDisplayType() == PANEL_DISPLAY_TYPE_VB1)
	{

		Printf("Pixel Clock,HTotal, HActive, HFp, HWidth, HBp\n"); 
		for(u1Cnt=SV_DCLK_48HZ;u1Cnt<SV_DCLK_OTHERS;u1Cnt++)
		{
			if(u1Cnt==SV_DCLK_48HZ)
			Printf("[48Hz]");	
			else if(u1Cnt==SV_DCLK_50HZ)
			Printf("[50Hz]");	
			else if(u1Cnt==SV_DCLK_60HZ)
			Printf("[60Hz]");	

			Printf(" %9d,%4d,%4d,%4d,%4d,%4d\n",GRAPHMODE_GetPixelClkByTiming(u1Cnt),GRAPHMODE_GetHtotalByTiming(u1Cnt),GRAPHMODE_GetPanelWidth(),
						GRAPHMODE_GetHSyncFpByTiming(u1Cnt),GRAPHMODE_GetHSyncWidthByTiming(u1Cnt), GRAPHMODE_GetHSyncBpByTiming(u1Cnt));
		}

		Printf("VTotal, VActive, VFp, VWidth, VBp\n"); 
		for(u1Cnt=SV_DCLK_48HZ;u1Cnt<SV_DCLK_OTHERS;u1Cnt++)
		{
			if(u1Cnt==SV_DCLK_48HZ)
			Printf("[48Hz]");	
			else if(u1Cnt==SV_DCLK_50HZ)
			Printf("[50Hz]");	
			else if(u1Cnt==SV_DCLK_60HZ)
			Printf("[60Hz]");	

			Printf(" %4d,%4d,%4d,%4d,%4d\n",GRAPHMODE_GetVtotalByTiming(u1Cnt),GRAPHMODE_GetPanelHeight(),
						GRAPHMODE_GetVSyncFpByTiming(u1Cnt),GRAPHMODE_GetVSyncWidthByTiming(u1Cnt), GRAPHMODE_GetVSyncBpByTiming(u1Cnt));
		}
		
		if (GraphMode_FORMAT == GM_FORMAT_CASE0_SMODE1)
		Printf("GM_FORMAT_CASE0_SMODE1,");
		else if (GraphMode_FORMAT == GM_FORMAT_CASE1_SMODE0)
		Printf("GM_FORMAT_CASE1_SMODE0,");
		else if (GraphMode_FORMAT == GM_FORMAT_CASE2_MMODE0)
		Printf("GM_FORMAT_CASE2_MMODE0,");	
		else if (GraphMode_FORMAT == GM_FORMAT_CASE3_INHOUSE)
		Printf("GM_FORMAT_CASE3_INHOUSE,");			

		if (GM_LANE_NUM == GM_LANE_1)
		Printf("GM_LANE_1,");	
		else if (GM_LANE_NUM == GM_LANE_2)
		Printf("GM_LANE_2,");	
		else if (GM_LANE_NUM == GM_LANE_4)
		Printf("GM_LANE_4,");		

		Printf("\n");		
	}
	else
	{
		Printf("Not Support Graph Mode\n");
	}

}
#endif

#endif /* CC_MTK_PRELOADER */

