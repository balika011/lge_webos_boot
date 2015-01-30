/*
 * vm_linux/chiling/uboot/board/mt5398/mt5398_panel.c
 *
 * Copyright (c) 2010-2012 MediaTek Inc.
 *	This file is based  ARM Realview platform
 *  Copyright (C) 2002 ARM Ltd.
 *  All Rights Reserved
 * $Author: p4admin $
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 * http://www.gnu.org/licenses/old-licenses/gpl-2.0.html
 *
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include <common.h>

#include "pmx_drvif.h"
#include "osd_drvif.h"
#include "panel.h"
#include "drv_video.h"
#include "drv_vdoclk.h"
#include "drv_display.h"
#include "drv_lvds.h"
#include "sv_const.h"
#include "hw_scpos.h"
#include "hw_psc.h"
#include "hw_lvds.h"
#include "vdp_if.h"
#include "nptv_if.h"
#include "x_printf.h"
#include "panel.h"
#include "c_model.h"
#include "ostg_if.h"
#include "hw_ospe.h"
#ifdef CFG_LG_CHG
#include "drv_tcon.h"
#endif
#ifdef CC_ONE_PKG_MULTI_DRAM_SIZE_SUPPORT
#include "x_dram.h"
#include "x_bim.h"
#endif
//#define LOGO_DEBUG
#ifndef LOGO_DEBUG
#define Printf(...)
#endif
#define LOADER_LOGO_COLOR_MODE 14
#define LOADER_LOGO_WIDTH 1280
#define LOADER_LOGO_HEIGHT 720

#define LOADER_LOGO_MAINTAIN 

static BOOL fgBootLogoInit = 0;
#define UBOOT_LOGO_WIDTH 1920
#define UBOOT_LOGO_HEIGHT 1080

#ifdef CFG_LG_CHG
/**
 * Named size of panel, comes from nvm_ddi.h
 */
typedef enum
{
	INCH_29=0,
	INCH_32,
	INCH_37,
	INCH_39,
	INCH_42,
	INCH_47,
	INCH_50,
	INCH_55,
	INCH_60,
	INCH_70,
	INCH_75,
	INCH_84,
	INCH_95,
	INCH_100,

	INCH_22,
	INCH_23,
	INCH_24,
	INCH_26,
	INCH_27, // Smart Monitor TV
	INCH_BASE
} INCH_TYPE_T;
#endif

typedef enum 
{
    CM_Reserved0 = 0,
    CM_Reserved1,
    CM_YCbCr_CLUT8,
    CM_Reserved2,
    CM_CbYCrY422_DIRECT16,
    CM_YCbYCr422_DIRECT16,
    CM_AYCbCr8888_DIRECT32,
    CM_Reserved3,
    CM_Reserved4,
    CM_Reserved5,
    CM_RGB_CLUT8,
    CM_RGB565_DIRECT16,
    CM_ARGB1555_DIRECT16,
    CM_ARGB4444_DIRECT16,
    CM_ARGB8888_DIRECT32,
    CM_Reserved6
} GFX_COLOR_MODE_T;


#ifndef CFG_LG_CHG
typedef enum
{
    VIDEO_TCON_GST,		/**< refer to panel cas */
    VIDEO_TCON_MCLK,	/**< refer to panel cas */
    VIDEO_TCON_GCLK,	/**< refer to panel cas */
    VIDEO_TCON_EO,		/**< refer to panel cas */
    VIDEO_TCON_VCOM_DYN,/**< refer to panel cas */
    VIDEO_TCON_EPISIGNAL,/**< refer to panel cas */

};
#endif
//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------

//---------------------------------------------------------------------
// Constant definitions
//---------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------
static const UINT32 g_au4PlaneArray[PMX_MAX_INPORT_NS] = { PMX_OSD2, PMX_MAX_INPORT_NS, PMX_MAX_INPORT_NS, PMX_MAX_INPORT_NS, PMX_MAIN};

//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

#if 1 // def LOADER_LOGO_FLASHOFFSET

#ifdef  PANEL_LG_32_EUD_SDA1
void PANEL_SetDisplayType(DISPLAY_TYPE_T display_type);

#endif
#ifndef  UBOOT_HACK   
//hongjun.chu marked
void PANEL_SetDisplayType_BootPDP(BOOL isFHD);
extern UINT8 check_vb1_panel_from_eep(void);
#endif



//-----------------------------------------------------------------------------
/** PmxDisplay(): Setup Panel Pmx/LVDS driver.
 *  This function is going to setup panel pmx and lvds driver with background
 *  color.
 *
 *  @param  u4Background    This is background color setting. 0xRRGGBB.
 *  @retval 0               Success.
 *  @retval otherwise       Failed.
 */
//-----------------------------------------------------------------------------
UINT32 PmxDisplay(UINT32 u4Background, UINT8 colordepth, UINT8 epi,UINT8 frctype, UINT8 inch, UINT8 paneltype, UINT8 panelRes)
{
    static UINT32 _fgInit = 0;
    UINT32 i;

    if (_fgInit)
    {
        vRegWriteFldAlign(MUTE_04, u4Background & 0xff, B_BACKGROUND_MJC);
        vRegWriteFldAlign(MUTE_04, (u4Background>>8)&0xff, G_BACKGROUND_MJC);
        vRegWriteFldAlign(MUTE_04, (u4Background>>16)&0xff, R_BACKGROUND_MJC);
        PMX_SetBg(u4Background, FALSE);
        return 0;
    }
    _fgInit = 1;

    PMX_Init();
    LoadPanelIndex();
#ifdef  PANEL_LG_32_EUD_SDA1
    PANEL_SetDisplayType(LCD_DISPLAY);
#endif
#ifdef CFG_LG_CHG	
	if( epi == 1) //EPI PANEL
	{
		printf("\n ==EPI TCON SETTING=== [Panel type: %d][Frc type:%d][inch:%d]\n",paneltype,frctype,inch);
		if(paneltype)	// V13
		{
			if(frctype == 0)
			{	
				if(inch == INCH_32)
					vDrvTCONSel(EPIV13FHD60_32);
				else if(inch == INCH_42)
					vDrvTCONSel(EPIV13FHD60_42);
				else if(inch == INCH_47)
					vDrvTCONSel(EPIV13FHD60_47);
				else if(inch == INCH_55)
					vDrvTCONSel(EPIV13FHD60_55);
				else
					vDrvTCONSel(EPIV13FHD60_47);
			}
			else
			{
				if(inch == INCH_42)
					vDrvTCONSel(EPIV13FHD120_42);
				else if(inch == INCH_47)
					vDrvTCONSel(EPIV13FHD120_47);
				else if(inch == INCH_55)
					vDrvTCONSel(EPIV13FHD120_55);
				else
					vDrvTCONSel(EPIV13FHD120_47);
			}
		}
		else // V12
		{
			if(frctype == 0)
				vDrvTCONSel(EPIV12FHD60_47);
			else
				vDrvTCONSel(EPIV12FHD120_47);
		}
	}
#endif

    // power off panel
    vApiPanelPowerSequence(FALSE);
    vApiTCONPower(VIDEO_TCON_GST, FALSE);
    vApiTCONPower(VIDEO_TCON_MCLK, FALSE);
    vApiTCONPower(VIDEO_TCON_GCLK, FALSE);
    vApiTCONPower(VIDEO_TCON_EO, FALSE);
    vApiTCONPower(VIDEO_TCON_VCOM_DYN, FALSE);
    vApiTCONPower(VIDEO_TCON_EPISIGNAL, FALSE);
#if 1//defined(CC_MT5368)|| defined(CC_MT5396)|| defined(CC_MT5389) || defined(CC_MT5398) || defined(CC_MT5399) || defined(CC_MT5890)
    vDrvLVDSInit();
    vErrorHandleInit();
#endif


    //vDrvOutputStageInit();
    vDrvDisplayInit();
    //vDrvLVDSInit();
    //vDrvVOPLLSet(PANEL_GetPixelClk60Hz());
    //vDrvSetLCDTiming();
    vDrvCalPanelTiming(SV_DCLK_60HZ, (PANEL_IsSupport120Hz()?120 : 60));


    // set background color
    vRegWriteFldAlign(MUTE_04, u4Background & 0xff, B_BACKGROUND_MJC);
    vRegWriteFldAlign(MUTE_04, (u4Background>>8)&0xff, G_BACKGROUND_MJC);
    vRegWriteFldAlign(MUTE_04, (u4Background>>16)&0xff, R_BACKGROUND_MJC);
    //PMX_SetBg(u4Background, FALSE);
    // remove vdp plane
    PMX_SetPlaneOrderArray(g_au4PlaneArray);

    for (i=0; i<=PMX_UPDATE_DELAY; i++)
    {
        // Use OutputVSync to update PlaneOrder.
        PMX_OnOutputVSync();
    }

    // disable main video window
    vIO32WriteFldAlign(PSCCTRL_11, 0, PSC_OUTPUT_HEIGHT);
    vIO32WriteFldAlign(PSCCTRL_0A, 1, PSC_SET_RES_TOGGLE);
    vIO32WriteFldAlign(PSCCTRL_0A, 0, PSC_SET_RES_TOGGLE);
	
	//Set OSD Timing from Front Scaler
	vIO32WriteFldAlign(SCPIP_SCSYS_14, 0, SCSYS_14_SC_OSD_SRC_SEL);

	//set vesa type => jeida
#ifndef UBOOT_HACK    
	//hongjun.chu marked for build pass 
    if (check_vb1_panel_from_eep())
    {
        PANEL_SetLvdsSelect(1); //0); //added by LG
    }
    else
    {
        PANEL_SetLvdsSelect(0); //0); //added by LG
    }
	#endif 
	//set color depth 
	PANEL_SetColorDepth(colordepth); //added by LG

   // power on panel
    vApiPanelPowerSequence(TRUE);

	if(inch == INCH_22 || inch == INCH_23 || inch == INCH_24 || inch == INCH_26 || inch == INCH_27) // MTV model
	{
		Splash_MICOM_PanelOn();
		udelay(20*1000);	//20msec delay
		//printf("MTV Tool. LVDS even/odd swap!!\n");
		if(panelRes == 1)
			PANEL_SetLvdsPCBSwap(TRUE);
	}
    // just enable the output for TCON here.
    switch(PANEL_GetDisplayType())
    {
        default:
        case PANEL_DISPLAY_TYPE_MAX:
            // error display type, defaultly do nothing
            break;

        case PANEL_DISPLAY_TYPE_LVDS:
            vDrvLVDSOff();
            vDrvLVDSConfig();
            vDrvLVDSOn();
            break;

        case PANEL_DISPLAY_TYPE_MLVDS:
            vDrvLVDSConfig();
            u1DrvTconInit();
            break;

#if defined(CC_MT5398) || defined(CC_MT5399) || defined(CC_MT5890)
        case PANEL_DISPLAY_TYPE_VB1:
            vDrvLVDSOff();
            vDrvLVDSConfig();
            vDrvVByOneOn();
            break;
#endif
    }

    if( epi == 1)
        udelay(30*1000);
    
    vApiTCONPower(VIDEO_TCON_GST, TRUE);
    vApiTCONPower(VIDEO_TCON_MCLK, TRUE);
    
    if( epi == 1)
        udelay(30*1000);
    
    vApiTCONPower(VIDEO_TCON_GCLK, TRUE);
    vApiTCONPower(VIDEO_TCON_EO, TRUE);
    vApiTCONPower(VIDEO_TCON_VCOM_DYN, TRUE);
    vApiTCONPower(VIDEO_TCON_EPISIGNAL, TRUE);
    //DumpPanelAttribute(PANEL_DUMP_CURRENT);

        PMX_SetBg(u4Background, FALSE);
        return 0;
    }

UINT32 PmxDisplay_PDP(UINT32 u4Background, UINT8 colordepth, UINT8 fhd)
{
	static UINT32 _fgInit = 0;
	UINT32 i;

	if (_fgInit)
	{
		vRegWriteFldAlign(MUTE_04, u4Background & 0xff, B_BACKGROUND_MJC);
		vRegWriteFldAlign(MUTE_04, (u4Background>>8)&0xff, G_BACKGROUND_MJC);
		vRegWriteFldAlign(MUTE_04, (u4Background>>16)&0xff, R_BACKGROUND_MJC);
		//PMX_SetBg(u4Background, FALSE);
		return 0;
	}

    _fgInit = 1;

#ifdef CC_MT5363
    vDrvVOPLLAutoKVCOBand();
#endif /* CC_MT5391 */

    PMX_Init();
    LoadPanelIndex();
//hongjun.chu marked 
#ifndef UBOOT_HACK	
	if(fhd == 1) // PDP
	{
		PANEL_SetDisplayType_BootPDP(TRUE);
	}
	else if(fhd == 0)  // HD
	{
		PANEL_SetDisplayType_BootPDP(FALSE);
	}
	else
	{
		printf("HW option error!\n");
		return -1;
	}
#endif
    // power off panel
    vApiPanelPowerSequence(FALSE);

#if defined(CC_MT5882)||defined(CC_MT5368)|| defined(CC_MT5396)|| defined(CC_MT5389) || defined(CC_MT5398)  || defined(CC_MT5399) || defined(CC_MT5890)
	vDrvLVDSInit();
	vErrorHandleInit();
#endif


    vDrvDisplayInit();


    // set background color
    vRegWriteFldAlign(MUTE_04, u4Background & 0xff, B_BACKGROUND_MJC);
    vRegWriteFldAlign(MUTE_04, (u4Background>>8)&0xff, G_BACKGROUND_MJC);
    vRegWriteFldAlign(MUTE_04, (u4Background>>16)&0xff, R_BACKGROUND_MJC);
	//PMX_SetBg(u4Background, FALSE);
    // remove vdp plane
    PMX_SetPlaneOrderArray(g_au4PlaneArray);

    for (i=0; i<=PMX_UPDATE_DELAY; i++)
    {
        // Use OutputVSync to update PlaneOrder.
        PMX_OnOutputVSync();
    }

    // disable main video window
    vIO32WriteFldAlign(PSCCTRL_11, 0, PSC_OUTPUT_HEIGHT);
    vIO32WriteFldAlign(PSCCTRL_0A, 1, PSC_SET_RES_TOGGLE);
    vIO32WriteFldAlign(PSCCTRL_0A, 0, PSC_SET_RES_TOGGLE);

	//set vesa type
	PANEL_SetLvdsSelect(1); //jeida for PDP

	//set color depth 
	PANEL_SetColorDepth(colordepth); //added by LG

    // power on panel
    vApiPanelPowerSequence(TRUE);

	// just enable the output for TCON here.
    switch(PANEL_GetDisplayType())
    {
        default:
        case PANEL_DISPLAY_TYPE_MAX:
            // error display type, defaultly do nothing
            break;

        case PANEL_DISPLAY_TYPE_LVDS:
            vDrvLVDSOff();
            vDrvLVDSConfig();
            vDrvLVDSOn();
            break;

        case PANEL_DISPLAY_TYPE_MLVDS:
            vDrvLVDSConfig();
            u1DrvTconInit();
            break;

#if defined(CC_MT5398) || defined(CC_MT5399) || defined(CC_MT5890)
        case PANEL_DISPLAY_TYPE_VB1:
            vDrvLVDSOff();
            vDrvLVDSConfig();
            vDrvVByOneOn();
            break;
#endif
    }

    return 0;
}


typedef enum _FORMAT_TYPE
{
    BMP_WINDOWS_V3 = 0,
    BMP_OS2_V1 = 1,
    /*
    BMP_OS2_V2 = 2,
    BMP_WINDOWS_V4 = 3,
    BMP_WINDOWS_V5 = 4,
    */
    BMP_NOT_SUPPORT = 3
} FORMAT_TYPE;

typedef enum _COMPRESSION_TYPE
{
    BI_RGB  = 0,
    BI_RLE8 = 1,
    BI_RLE4 = 2,
    BI_BITFIELDS = 3
} COMPRESSION_TYPE;

typedef struct _BMP_INFO_T
{
	UINT32                  ui4_file_size;
    UINT32                  ui4_read_byte;
	
	UINT16              	ui2_image_width;
    UINT16              	ui2_image_height;
	
    FORMAT_TYPE          	e_bmp_type;
    UINT32                  ui4_data_ofst;
    UINT8                   ui1_bit_count;
    COMPRESSION_TYPE    	e_comprees_type;
    UINT32                  ui4_color_num;
    UINT8*                  pui1_rgb_quad;
    UINT32                  aui4_rgb_mask[3];
    UINT8                   aui1_right_shift[3];
    UINT8                   aui1_left_shift[3];

    BOOL                     b_bottom_to_top;
} BMP_INFO_T;

typedef enum
{
	BMPR_OK,
	BMPR_INV_FILE,
	BMPR_NOT_SUPPORT,
	BMPR_OUT_OF_MEMORY,
	BMPR_OUT_OF_FILE_LEN
}BMP_ERROR;

UINT32 get4bytes(UCHAR* buf, UINT32* pu4ReadPos)
{
	UINT32 u4Value = 0;

	u4Value  = buf[(*pu4ReadPos)++];
    u4Value |= buf[(*pu4ReadPos)++] << 8;
    u4Value |= buf[(*pu4ReadPos)++] << 16;
    u4Value |= buf[(*pu4ReadPos)++] << 24;

	return u4Value;
}

UINT32 get2bytes(UCHAR* buf, UINT32* pu4ReadPos)
{
	UINT32 u4Value = 0;

	u4Value  = buf[(*pu4ReadPos)++];
    u4Value |= buf[(*pu4ReadPos)++] << 8;

	return u4Value;
}

void skipBytes(UINT32* pu4ReadPos, UINT32 u4Bytes)
{
	*pu4ReadPos += u4Bytes;
}

void setPos(UINT32* pu4ReadPos, UINT32 u4Pos)
{
	*pu4ReadPos = u4Pos;
}

INT32 copyBytes(UCHAR* fromBuf, UCHAR* toBuf, UINT32 u4CopyBytes, UINT32* pu4ReadPos, UINT32 file_len)
{
	if (*pu4ReadPos + u4CopyBytes > file_len)
	{
		return BMPR_OUT_OF_FILE_LEN;
	}
	memcpy((VOID*)toBuf, (VOID*)&fromBuf[*pu4ReadPos], u4CopyBytes);

	*pu4ReadPos += u4CopyBytes;

	return BMPR_OK;
}

INT32 parse_bmp_header(UCHAR* BmpBuf, BMP_INFO_T* pt_bmp_info)
{
	UINT32 u4HeaderSize;
	UINT32 u4ReadPos = pt_bmp_info->ui4_read_byte;
	UINT16 u2BitCount;
	INT32 i;
	UINT32 u4Mask;
	
	//bmp type
	if (BmpBuf[u4ReadPos++] != 'B' || BmpBuf[u4ReadPos++] !='M') 
	{
		printf("u4ReadPos = %d, BmpBuf[0] =%d, BmpBuf[1] = %d \n", 
			u4ReadPos, BmpBuf[0], BmpBuf[1]);
		pt_bmp_info->ui4_read_byte = u4ReadPos;
		printf("[uboot bmp decoder]it is not bmp file\n");
		return -(UINT32)BMPR_INV_FILE;
	}

	//file size
	pt_bmp_info->ui4_file_size = get4bytes(BmpBuf, &u4ReadPos);
	if (pt_bmp_info->ui4_file_size == 0)
	{
//		pt_bmp_info->ui4_file_size = sizeof(bg_graphic);
	}
	//reserved
	skipBytes(&u4ReadPos, 4);

	//get the offset from beginning of the file to the beginning of the bitmap data
	pt_bmp_info->ui4_data_ofst = get4bytes(BmpBuf, &u4ReadPos);

	//get info header length
	u4HeaderSize = get4bytes(BmpBuf, &u4ReadPos);
	Printf("[uboot bmp decoder]HeaderSize = %d \n", u4HeaderSize);
	if (u4HeaderSize == 0x0c)
	{
		pt_bmp_info->e_bmp_type = BMP_OS2_V1;

		pt_bmp_info->ui2_image_width = get2bytes(BmpBuf, &u4ReadPos);
		pt_bmp_info->ui2_image_height = get2bytes(BmpBuf, &u4ReadPos);

		skipBytes(&u4ReadPos, 2);

		u2BitCount = get2bytes(BmpBuf, &u4ReadPos);
		/* OS/2 V1 bitmaps cannot be compressed and cannot be 16 or 32 bits/pixel */
		if (u2BitCount == 32 || u2BitCount == 16)
		{
			pt_bmp_info->ui4_read_byte = u4ReadPos;
			printf("[uboot bmp decoder]OS/2 V1 bitmaps cannot be 16 or 32 bits/pixel, bitcount=%d \n", u2BitCount);
			return -(UINT32)BMPR_INV_FILE;
		}
		pt_bmp_info->ui1_bit_count = u2BitCount;
		pt_bmp_info->e_comprees_type = BI_RGB;

		pt_bmp_info->ui4_color_num = (pt_bmp_info->ui4_data_ofst - 0x1A) / 3;

		if (pt_bmp_info->ui4_color_num > 0)
		{
			setPos(&u4ReadPos, 0x1A);

			if (pt_bmp_info->ui1_bit_count == 1 ||
				pt_bmp_info->ui1_bit_count == 4 ||
				pt_bmp_info->ui1_bit_count == 8)
			{
				pt_bmp_info->pui1_rgb_quad = (UINT8*)malloc(3 * pt_bmp_info->ui4_color_num);
				if (NULL == pt_bmp_info->pui1_rgb_quad)
				{
					pt_bmp_info->ui4_read_byte = u4ReadPos;
					return BMPR_OUT_OF_MEMORY;
				}
				copyBytes(BmpBuf, pt_bmp_info->pui1_rgb_quad, 3 * pt_bmp_info->ui4_color_num, &u4ReadPos, pt_bmp_info->ui4_file_size);
			}
			else
			{
				pt_bmp_info->ui4_read_byte = u4ReadPos;
				return BMPR_INV_FILE;
			}
		}
	}
	else if (u4HeaderSize >= 0x28)
	{
		pt_bmp_info->e_bmp_type = BMP_WINDOWS_V3;

		pt_bmp_info->ui2_image_width = get4bytes(BmpBuf, &u4ReadPos);
		pt_bmp_info->ui2_image_height = get4bytes(BmpBuf, &u4ReadPos);

		skipBytes(&u4ReadPos, 2);

		pt_bmp_info->ui1_bit_count = get2bytes(BmpBuf, &u4ReadPos);

		pt_bmp_info->e_comprees_type = get4bytes(BmpBuf, &u4ReadPos);

		pt_bmp_info->ui4_color_num = (pt_bmp_info->ui4_data_ofst - 0x36) / 4;

		if (pt_bmp_info->ui4_color_num > 0)
		{
			setPos(&u4ReadPos, 0x36);

			if (pt_bmp_info->ui1_bit_count == 1 ||
				pt_bmp_info->ui1_bit_count == 4 ||
				pt_bmp_info->ui1_bit_count == 8)
			{
				pt_bmp_info->pui1_rgb_quad = (UINT8*)malloc(4 * pt_bmp_info->ui4_color_num);
				if (NULL == pt_bmp_info->pui1_rgb_quad)
				{
					pt_bmp_info->ui4_read_byte = u4ReadPos;
					return BMPR_OUT_OF_MEMORY;
				}

				copyBytes(BmpBuf, pt_bmp_info->pui1_rgb_quad, 4 * pt_bmp_info->ui4_color_num, &u4ReadPos, pt_bmp_info->ui4_file_size);			
			}
			else if (pt_bmp_info->ui1_bit_count == 16)
			{
				if (pt_bmp_info->e_comprees_type == BI_BITFIELDS)
				{
					pt_bmp_info->ui4_color_num = 0;
					for (i = 0; i < 3; i++)
                    {
                    	pt_bmp_info->aui4_rgb_mask[i] = get4bytes(BmpBuf, &u4ReadPos);      
                
                        pt_bmp_info->aui1_right_shift[i] = 0;
                        pt_bmp_info->aui1_left_shift[i] = 0;
                        while (!((pt_bmp_info->aui4_rgb_mask[i] >> 
                            pt_bmp_info->aui1_right_shift[i]) & 0x00000001))
                        {
                            pt_bmp_info->aui1_right_shift[i]++;
                        }
                        while (!(((UINT8)((pt_bmp_info->aui4_rgb_mask[i] >> 
                            pt_bmp_info->aui1_right_shift[i]) <<
                            pt_bmp_info->aui1_left_shift[i])) & 0x80))
                        {
                            pt_bmp_info->aui1_left_shift[i]++;
                        }  
                    }
				}
				else
				{
					pt_bmp_info->e_bmp_type = BMP_NOT_SUPPORT;
					pt_bmp_info->ui4_read_byte = u4ReadPos;
            		return BMPR_NOT_SUPPORT;
				}
			}
			else if (pt_bmp_info->ui1_bit_count == 24)
            {
                /* 24bpp does not contain any palette entries */
                //BMP_ASSERT(0);
                pt_bmp_info->ui4_color_num = 0;
                setPos(&u4ReadPos, pt_bmp_info->ui4_data_ofst);
				pt_bmp_info->ui4_read_byte = u4ReadPos;
                return BMPR_OK;
            }
			else /*32bpp*/
            {
                /* When the compression field of 32bpp is set to BI_RGB, the 
                palette field does not contain any entries */
                if (BI_RGB == pt_bmp_info->e_comprees_type)
                {
                    pt_bmp_info->e_bmp_type = BMP_NOT_SUPPORT;
					pt_bmp_info->ui4_read_byte = u4ReadPos;
        			return BMPR_NOT_SUPPORT;
                }
                else if (BI_BITFIELDS == pt_bmp_info->e_comprees_type)
                {
                    /* When the compression field is set to BI_BITFIELDS, the 
                    palettle field contains three dword color masks */
                    pt_bmp_info->ui4_color_num = 0;
                    for (i = 0; i < 3; i++)
                    {
                        pt_bmp_info->aui4_rgb_mask[i] = get4bytes(BmpBuf, &u4ReadPos);
                        pt_bmp_info->aui1_right_shift[i] = 0;
                        pt_bmp_info->aui1_left_shift[i] = 0;
                        while (!((pt_bmp_info->aui4_rgb_mask[i] >> 
                            pt_bmp_info->aui1_right_shift[i]) & 0x00000001))
                        {
                            pt_bmp_info->aui1_right_shift[i]++;
                        }
                        while (!(((UINT8)((pt_bmp_info->aui4_rgb_mask[i] >> 
                            pt_bmp_info->aui1_right_shift[i]) <<
                            pt_bmp_info->aui1_left_shift[i])) & 0x80))
                        {
                            pt_bmp_info->aui1_left_shift[i]++;
                        }
                    }
                }
                else
                {
                    pt_bmp_info->e_bmp_type = BMP_NOT_SUPPORT;
					pt_bmp_info->ui4_read_byte = u4ReadPos;
        			return BMPR_NOT_SUPPORT;
                }
            } 
		}
		else
		{
			if ((pt_bmp_info->ui1_bit_count == 16)&&
				(BI_RGB == pt_bmp_info->e_comprees_type))
            {
                /*555*/
                pt_bmp_info->aui4_rgb_mask[0] = 0x7C00;
                pt_bmp_info->aui4_rgb_mask[1] = 0x03E0;
                pt_bmp_info->aui4_rgb_mask[2] = 0x001F;
                pt_bmp_info->aui1_right_shift[0] = 10;
                pt_bmp_info->aui1_right_shift[1] = 5;
                pt_bmp_info->aui1_right_shift[2] = 0;
                pt_bmp_info->aui1_left_shift[0] = 3;
                pt_bmp_info->aui1_left_shift[1] = 3;
                pt_bmp_info->aui1_left_shift[2] = 3;

            }
			if ((pt_bmp_info->ui1_bit_count == 32)&&
					(BI_RGB == pt_bmp_info->e_comprees_type))
                {
                    /*888*/
                    pt_bmp_info->aui4_rgb_mask[0] = 0x00FF0000;
                    pt_bmp_info->aui4_rgb_mask[1] = 0x0000FF00;
                    pt_bmp_info->aui4_rgb_mask[2] = 0x000000FF;
                    pt_bmp_info->aui1_right_shift[0] = 16;
                    pt_bmp_info->aui1_right_shift[1] = 8;
                    pt_bmp_info->aui1_right_shift[2] = 0;
                    pt_bmp_info->aui1_left_shift[0] = 0;
                    pt_bmp_info->aui1_left_shift[1] = 0;
                    pt_bmp_info->aui1_left_shift[2] = 0;

                }
		}
	}
	else
    {
        /* Other BMP types are not supported now */
        pt_bmp_info->e_bmp_type = BMP_NOT_SUPPORT;
		pt_bmp_info->ui4_read_byte = u4ReadPos;
        return BMPR_NOT_SUPPORT;
    }

	pt_bmp_info->ui4_read_byte = u4ReadPos;
	return BMPR_OK;
}

#define DST_BYTE_COUNT 4 //ARGB8888
INT32 load_bmp_content(BMP_INFO_T* pt_bmp_info, UCHAR* src_buf, UCHAR** dst_buf)
{
	UINT32 	ui4_h, ui4_w;
	UINT32 	ui4_readPos = pt_bmp_info->ui4_read_byte;
	UINT32 	ui4_row_bytes;
	UINT8  	*pui1_row_data = NULL, *pui1_raw_data = NULL, *pui1_raw_data_tmp;
	UINT8  	ui1_bit_count = pt_bmp_info->ui1_bit_count;
	UINT8 	ui1_bytes = (UINT8)((pt_bmp_info->e_bmp_type == BMP_OS2_V1) ? 3 : 4);;
        static UINT32 u4DstAddr = 0;
	UINT32 u4BmpDecAddr;

        if(u4DstAddr == 0)
        {
            u4DstAddr =  TOTAL_MEM_SIZE - FBM_MEM_CFG_SIZE;
        }
        if(*dst_buf == NULL)
        {
            u4BmpDecAddr = u4DstAddr;
        }
        else
        {
            u4BmpDecAddr = (UINT32)*dst_buf;
        }
	
	//just support bitfiled and RGB
	if (pt_bmp_info->e_comprees_type == BI_RLE4 ||
		pt_bmp_info->e_comprees_type == BI_RLE8)
	{
		return BMPR_NOT_SUPPORT;
	}

	ui4_row_bytes = (((pt_bmp_info->ui2_image_width * pt_bmp_info->ui1_bit_count + 31) >> 5) << 2);
	pui1_row_data = (UINT8*)malloc(ui4_row_bytes);
	if (pui1_row_data == NULL)
	{
		printf("pui1_row_data alloc fail.\n");
		return BMPR_OUT_OF_MEMORY;
	}
	
	pui1_raw_data_tmp = (UINT8*)u4BmpDecAddr;
	
	pui1_raw_data = pui1_raw_data_tmp + pt_bmp_info->ui2_image_width * DST_BYTE_COUNT * (pt_bmp_info->ui2_image_height - 1);
	
	setPos(&ui4_readPos, pt_bmp_info->ui4_data_ofst);

	for (ui4_h = 0; ui4_h < pt_bmp_info->ui2_image_height; ui4_h++)
	{

		if (copyBytes(src_buf, pui1_row_data, ui4_row_bytes, &ui4_readPos, pt_bmp_info->ui4_file_size) != BMPR_OK)
		{
			free(pui1_row_data);
			return BMPR_OUT_OF_FILE_LEN;
		}

		for (ui4_w = 0; ui4_w < pt_bmp_info->ui2_image_width; ui4_w++)
		{
			if (ui1_bit_count == 1 ||
		        ui1_bit_count == 4 ||
		        ui1_bit_count == 8)
		    {
		        UINT32 l, j;
		        UINT32 ui4_idx;
		        
		        j = (ui4_w * ui1_bit_count) / 8;
		        l = (ui4_w * ui1_bit_count) % 8;
		        ui4_idx = ((pui1_row_data[j] << l) & 0xFF) >> (8 - ui1_bit_count);

		        if (ui4_idx < pt_bmp_info->ui4_color_num)
		        {
		            pui1_raw_data[4*ui4_w]   = 0xFF;
		            pui1_raw_data[4*ui4_w+1] = pt_bmp_info->pui1_rgb_quad[ui1_bytes*ui4_idx+2];
		            pui1_raw_data[4*ui4_w+2] = pt_bmp_info->pui1_rgb_quad[ui1_bytes*ui4_idx+1];
		            pui1_raw_data[4*ui4_w+3] = pt_bmp_info->pui1_rgb_quad[ui1_bytes*ui4_idx];
		        }
		        else
		        {
		            pui1_raw_data[4*ui4_w]   = 0xFF; 
		            pui1_raw_data[4*ui4_w+1] = 0xFF;
		            pui1_raw_data[4*ui4_w+2] = 0xFF;
		            pui1_raw_data[4*ui4_w+3] = 0xFF;
		        }
		    }
		    else if (ui1_bit_count == 16)
		    {
		        UINT16* pui2_src = (UINT16*)pui1_row_data;
		        pui1_raw_data[4*ui4_w] = 0xFF;
		        pui1_raw_data[4*ui4_w+1] = (UINT8)(((UINT8)((pui2_src[ui4_w] & 
					(UINT16)pt_bmp_info->aui4_rgb_mask[0])>>pt_bmp_info->aui1_right_shift[0]))<<pt_bmp_info->aui1_left_shift[0]);
		        pui1_raw_data[4*ui4_w+2] = (UINT8)(((UINT8)((pui2_src[ui4_w] & 
					(UINT16)pt_bmp_info->aui4_rgb_mask[1])>>pt_bmp_info->aui1_right_shift[1]))<<pt_bmp_info->aui1_left_shift[1]);
		        pui1_raw_data[4*ui4_w+3] = (UINT8)(((UINT8)((pui2_src[ui4_w] & 
					(UINT16)pt_bmp_info->aui4_rgb_mask[2])>>pt_bmp_info->aui1_right_shift[2]))<<pt_bmp_info->aui1_left_shift[2]);
		        
		    }
		    else if (ui1_bit_count == 24)
		    {
		        pui1_raw_data[4*ui4_w]   = 0xFF;
		        pui1_raw_data[4*ui4_w+1] = pui1_row_data[3*ui4_w+2];
		        pui1_raw_data[4*ui4_w+2] = pui1_row_data[3*ui4_w+1];
		        pui1_raw_data[4*ui4_w+3] = pui1_row_data[3*ui4_w];
		    }
		    else
		    {
		        UINT32* pui4_src = (UINT32*)pui1_row_data;

		        pui1_raw_data[4*ui4_w] = 0xFF;
		        pui1_raw_data[4*ui4_w+1] = (UINT8)(((UINT8)((pui4_src[ui4_w] & 
					pt_bmp_info->aui4_rgb_mask[0])>>pt_bmp_info->aui1_right_shift[0]))<<pt_bmp_info->aui1_left_shift[0]);
		        pui1_raw_data[4*ui4_w+2] = (UINT8)(((UINT8)((pui4_src[ui4_w] & 
					pt_bmp_info->aui4_rgb_mask[1])>>pt_bmp_info->aui1_right_shift[1]))<<pt_bmp_info->aui1_left_shift[0]);
		        pui1_raw_data[4*ui4_w+3] = (UINT8)(((UINT8)((pui4_src[ui4_w] & 
					pt_bmp_info->aui4_rgb_mask[2])>>pt_bmp_info->aui1_right_shift[2]))<<pt_bmp_info->aui1_left_shift[0]);
		        
		    }
		}
		pui1_raw_data -= pt_bmp_info->ui2_image_width * DST_BYTE_COUNT;
	}

	pt_bmp_info->ui4_read_byte = ui4_readPos;

        if(*dst_buf == NULL)
        {
	    u4DstAddr += pt_bmp_info->ui2_image_width * DST_BYTE_COUNT * pt_bmp_info->ui2_image_height;
            *dst_buf = (UCHAR*)u4BmpDecAddr;
        }

	free(pui1_row_data);
	free(pt_bmp_info->pui1_rgb_quad);
	return BMPR_OK;
}

INT32 decode_bmp(unsigned int u4BmpAddr, unsigned int *u4BmpWidth, unsigned int *u4BmpHeight, unsigned char **ppu1DstRawAddr)
{
	char* BmpBuf = (char*)u4BmpAddr;
	char* tmp = BmpBuf;
	BMP_INFO_T t_bmp;
	INT32 i4_ret;
	
	memset(&t_bmp, 0, sizeof(BMP_INFO_T));
	
	/*parse header file */
	i4_ret = parse_bmp_header(tmp, &t_bmp);
	if (i4_ret != BMPR_OK)
	{
		Printf("[uboot bmp decoder]parse_bmp_header fail (%d) \n", i4_ret);
		return i4_ret;
	}

	Printf("e_bmp_type=%d, e_compress_type=%d, bit_count=%d \nimage_h=%d, image_w=%d, file_size=%d \n", 
		t_bmp.e_bmp_type, t_bmp.e_comprees_type, t_bmp.ui1_bit_count, 
		t_bmp.ui2_image_height, t_bmp.ui2_image_width, t_bmp.ui4_file_size);
	
        *u4BmpWidth = t_bmp.ui2_image_width;
        *u4BmpHeight = t_bmp.ui2_image_height;

	//decode
	i4_ret = load_bmp_content(&t_bmp, BmpBuf, ppu1DstRawAddr);
	if (i4_ret != BMPR_OK)
	{
		Printf("[uboot bmp decoder]load_bmp_content fail (%d) \n", i4_ret);
		return i4_ret;
	}

	return BMPR_OK;
}

static UINT32 Loder_ColorExpansion(UINT32 u4Color, UINT32 u4ColorMode)
{
    switch (u4ColorMode)
    {
    case 1:
        u4Color = (u4Color << 16) | (u4Color & 0xffff);
        break;

    case 2:
        u4Color = (u4Color <<  8) | (u4Color & 0xff);
        u4Color = (u4Color << 16) | (u4Color & 0xffff);
        break;

    case 3:
        u4Color = (u4Color <<  4) | (u4Color & 0xf);
        u4Color = (u4Color <<  8) | (u4Color & 0xff);
        u4Color = (u4Color << 16) | (u4Color & 0xffff);
        break;

    case 4:
        u4Color = (u4Color <<  2) | (u4Color & 0x3);
        u4Color = (u4Color <<  4) | (u4Color & 0xf);
        u4Color = (u4Color <<  8) | (u4Color & 0xff);
        u4Color = (u4Color << 16) | (u4Color & 0xffff);
        break;

    default:
        break;
    }

    return u4Color;
}

void Loader_SwGetColorComponent(UINT8 **ppu1DestFb, UINT32 u4CM, 
    INT32 *pi4ColorComponent)
{
    // get color components from frame buffer, advance frame buffer point for 1 pixel
    INT32  i4Q, i4W, i4E, i4R;
    UINT32 u4Color;
    UINT16 u2Color;
    UINT8  u1Color;

    if ((ppu1DestFb == NULL) || (pi4ColorComponent == NULL))
    {
        return;
    }

    switch (u4CM)
    {
    case CM_RGB_CLUT8:
        u1Color = *((UINT8 *)*ppu1DestFb);
        i4Q = u1Color & 0xff;
        i4W = u1Color & 0xff;
        i4E = u1Color & 0xff;
        i4R = u1Color & 0xff;
        *ppu1DestFb += 1;
        break;

    case CM_RGB565_DIRECT16:
        u2Color = *((UINT16 *)*ppu1DestFb);
        i4Q = 0xff;
        i4W = (u2Color >> 8) & 0xf8;
        i4E = (u2Color >> 3) & 0xfc;
        i4R = (u2Color << 3) & 0xf8;

        i4W = i4W | (i4W >> 5);
        i4E = i4E | (i4E >> 6);
        i4R = i4R | (i4R >> 5);

        *ppu1DestFb += 2;
        break;

    case CM_ARGB1555_DIRECT16:
        u2Color = *((UINT16 *)*ppu1DestFb);
        i4Q = (u2Color & 0x8000) ? 0xff : 0;
        i4W = (u2Color >> 7) & 0xf8;
        i4E = (u2Color >> 2) & 0xf8;
        i4R = (u2Color << 3) & 0xf8;

        i4W = i4W | (i4W >> 5);
        i4E = i4E | (i4E >> 5);
        i4R = i4R | (i4R >> 5);

        *ppu1DestFb += 2;
        break;

    case CM_ARGB4444_DIRECT16:
        u2Color = *((UINT16 *)*ppu1DestFb);
        i4Q = (u2Color >> 8) & 0xf0;
        i4W = (u2Color >> 4) & 0xf0;
        i4E = u2Color & 0xf0;
        i4R = (u2Color << 4) & 0xf0;

        i4Q = i4Q | (i4Q >> 4);
        i4W = i4W | (i4W >> 4);
        i4E = i4E | (i4E >> 4);
        i4R = i4R | (i4R >> 4);

        *ppu1DestFb += 2;
        break;

    case CM_ARGB8888_DIRECT32:
        u4Color = *((UINT32 *)*ppu1DestFb);
        i4Q = u4Color >> 24;
        i4W = (u4Color >> 16) & 0xff;
        i4E = (u4Color >>  8) & 0xff;
        i4R = u4Color & 0xff;
        *ppu1DestFb += 4;
        break;

    default:
        return;
    }

    pi4ColorComponent[0] = i4Q;
    pi4ColorComponent[1] = i4W;
    pi4ColorComponent[2] = i4E;
    pi4ColorComponent[3] = i4R;

    return;
}

void Loader_SwSetColorComponent(UINT8 **ppu1DestFb, UINT32 u4CM, 
    const INT32 *pi4sColorComponent)
{
    INT32  i4Q, i4W, i4E, i4R;
    UINT32 u4Color;
    UINT16 u2Color;
    UINT8  u1Color;

    if ((ppu1DestFb == NULL) || (pi4sColorComponent == NULL))
    {
        return;
    }

    i4Q = pi4sColorComponent[0];
    i4W = pi4sColorComponent[1];
    i4E = pi4sColorComponent[2];
    i4R = pi4sColorComponent[3];

    i4Q = (i4Q > 255) ? 255 :
          (i4Q <   0) ?   0 : i4Q;

    i4W = (i4W > 255) ? 255 :
          (i4W <   0) ?   0 : i4W;

    i4E = (i4E > 255) ? 255 :
          (i4E <   0) ?   0 : i4E;

    i4R = (i4R > 255) ? 255 :
          (i4R <   0) ?   0 : i4R;
          
    switch (u4CM)
    {
    case CM_RGB_CLUT8:
        u1Color = i4R & 0xff;
        *((UINT8 *)*ppu1DestFb) = u1Color;
        *ppu1DestFb += 1;
        break;
        
    case CM_RGB565_DIRECT16:
        u2Color = ((i4W & 0xf8) << 8)   |
                  ((i4E & 0xfc) << 3)   |
                  (i4R >> 3);
        *((UINT16 *)*ppu1DestFb) = u2Color;
        *ppu1DestFb += 2;
        break;

    case CM_ARGB1555_DIRECT16:
        u2Color = (i4Q ? 0x8000 : 0)     |
                  ((i4W & 0xf8) << 7)    |
                  ((i4E & 0xf8) << 2)    |
                  (i4R >> 3);
        *((UINT16 *)*ppu1DestFb) = u2Color;
        *ppu1DestFb += 2;
        break;

    case CM_ARGB4444_DIRECT16:
        u2Color = ((i4Q & 0xf0) << 8)   |
                  ((i4W & 0xf0) << 4)   |
                  (i4E & 0xf0)          |
                  (i4R >> 4);
        *((UINT16 *)*ppu1DestFb) = u2Color;
        *ppu1DestFb += 2;
        break;

    case CM_ARGB8888_DIRECT32:
        u4Color = (i4Q << 24) |
                  (i4W << 16) |
                  (i4E <<  8) |
                  i4R;
        *((UINT32 *)*ppu1DestFb) = u4Color;
        *ppu1DestFb += 4;
        break;

    default:
        return;
    }

    return;
}



INT32 Loader_SwBitBlt(UINT8 *pu1SrcBase, UINT8 *pu1DstBase, UINT32 u4SrcPitch, 
    UINT32 u4DstPitch, UINT32 u4SrcCM, UINT32 u4DstCM, UINT32 u4Width, 
    UINT32 u4Height, UINT32 u4ColorMin, UINT32 u4ColorMax, UINT32 u4TransEn,
    UINT32 u4KeynotEn, UINT32 u4ColchgEn, UINT32 u4KeySdSel, UINT32 u4Color)
{
    INT32 ai4ColorCompMin[4] = {0};
    INT32 ai4ColorCompMax[4] = {0};
    INT32 ai4SrcColorComp[4] = {0};
    INT32 ai4DstColorComp[4] = {0};
    INT32 ai4RectColor[4] = {0};
    UINT32 *pu4Color;
    UINT32 x, y;
    UINT8 *pu1SrcLine, *pu1DstLine;
    UINT8 *pu1SrcRead, *pu1DstRead, *pu1DstWrite;

    // protection
    if (u4TransEn && u4ColchgEn)
    {
        return 0;
    }

    pu1SrcLine  = pu1SrcBase;
    pu1DstLine  = pu1DstBase;
		
    if (u4TransEn || u4ColchgEn)
    {
        ai4ColorCompMin[0] = (INT32) (u4ColorMin >> 24);
        ai4ColorCompMin[1] = (INT32)((u4ColorMin >> 16) & 0xFF);
        ai4ColorCompMin[2] = (INT32)((u4ColorMin >>  8) & 0xFF);
        ai4ColorCompMin[3] = (INT32)( u4ColorMin        & 0xFF);

        ai4ColorCompMax[0] = (INT32) (u4ColorMax >> 24);
        ai4ColorCompMax[1] = (INT32)((u4ColorMax >> 16) & 0xFF);
        ai4ColorCompMax[2] = (INT32)((u4ColorMax >>  8) & 0xFF);
        ai4ColorCompMax[3] = (INT32)( u4ColorMax        & 0xFF);

        pu4Color = &u4Color;
        u4Color  = Loder_ColorExpansion(u4Color, u4DstCM);

        Loader_SwGetColorComponent((UINT8 **)&pu4Color, u4DstCM, ai4RectColor);
    }

    for (y = 0; y < u4Height; y++)
    {
        pu1SrcRead = pu1SrcLine;

        pu1DstRead = pu1DstLine;
        pu1DstWrite = pu1DstLine;

        for (x = 0; x < u4Width; x++)
        {
            Loader_SwGetColorComponent(&pu1SrcRead, u4SrcCM, ai4SrcColorComp);
            Loader_SwGetColorComponent(&pu1DstRead, u4DstCM, ai4DstColorComp);

            // the document says COLCHA_ENA and TRANS_ENA should not
            // set at the same time, but if we set them both, which
            // one would process by hardware ? Check it.
            // 
            // hardware can NOT work, software must keep it off
            //
                            //---------------------------------------------------
            if (u4KeySdSel) // key compared with destination pixel (dst color key)
            {               //---------------------------------------------------
                if ((u4SrcCM == u4DstCM) && ((UINT32)CM_RGB_CLUT8 == u4SrcCM))
                {
                    if (u4TransEn)
                    {
                        if (u4KeynotEn)
                        {
                            // exactly equal color_key_min[7:0]
                            if (ai4DstColorComp[0] == ai4ColorCompMin[3])
                            {
                                // write src color to dst
                                Loader_SwSetColorComponent(&pu1DstWrite, u4DstCM, 
                                    ai4SrcColorComp);
                            }
                            else
                            {
                                // keep dst color
                                pu1DstWrite += 1;
                            }
                        }
                        else    // u4KeynotEn = 0
                        {
                            // exactly equal color_key_min[7:0]
                            if (ai4DstColorComp[0] == ai4ColorCompMin[3])
                            {
                                // keep dst color
                                pu1DstWrite += 1;
                            }
                            else
                            {
                                // write src color to dst
                                Loader_SwSetColorComponent(&pu1DstWrite, u4DstCM, 
                                    ai4SrcColorComp);
                            }
                        }
                    }
                    else if (u4ColchgEn)
                    {
                        if (u4KeynotEn)
                        {
                            // exactly equal color_key_min[7:0]
                            if (ai4DstColorComp[0] == ai4ColorCompMin[3])
                            {
                                // write src color to dst
                                Loader_SwSetColorComponent(&pu1DstWrite, u4DstCM, 
                                    ai4SrcColorComp);
                            }
                            else
                            {
                                // write rect color to dst
                                Loader_SwSetColorComponent(&pu1DstWrite, u4DstCM, 
                                    ai4RectColor);
                            }
                        }
                        else    // u4KeynotEn = 0
                        {
                            // exactly equal color_key_min[7:0]
                            if (ai4DstColorComp[0] == ai4ColorCompMin[3])
                            {
                                // write rect color to dst
                                Loader_SwSetColorComponent(&pu1DstWrite, u4DstCM, 
                                    ai4RectColor);
                            }
                            else
                            {
                                // write src color to dst
                                Loader_SwSetColorComponent(&pu1DstWrite, u4DstCM, 
                                    ai4SrcColorComp);
                            }
                        }
                    }
                    else        // u4TransEn = 0 and u4ColchgEn = 0
                    {           // write src color to dst
                        Loader_SwSetColorComponent(&pu1DstWrite, u4DstCM, ai4SrcColorComp);
                    }

                }
                else    // 32bpp and 16bpp
                {
                    if (u4TransEn)
                    {
                        if (u4KeynotEn)
                        {
                            if ((ai4DstColorComp[0] >= ai4ColorCompMin[0]) && 
                                (ai4DstColorComp[0] <= ai4ColorCompMax[0]) &&
                                (ai4DstColorComp[1] >= ai4ColorCompMin[1]) && 
                                (ai4DstColorComp[1] <= ai4ColorCompMax[1]) &&
                                (ai4DstColorComp[2] >= ai4ColorCompMin[2]) && 
                                (ai4DstColorComp[2] <= ai4ColorCompMax[2]) &&
                                (ai4DstColorComp[3] >= ai4ColorCompMin[3]) && 
                                (ai4DstColorComp[3] <= ai4ColorCompMax[3]))
                            {
                                // write src color to dst
                                Loader_SwSetColorComponent(&pu1DstWrite, u4DstCM, 
                                    ai4SrcColorComp);
                            }
                            else
                            {
                                // keep dst color
                                if ((UINT32)CM_ARGB8888_DIRECT32 == u4DstCM)
                                {
                                    pu1DstWrite += 4;
                                }
                                else if (((UINT32)CM_ARGB4444_DIRECT16 == u4DstCM) ||
                                         ((UINT32)CM_ARGB1555_DIRECT16 == u4DstCM) ||
                                         ((UINT32)CM_RGB565_DIRECT16   == u4DstCM))
                                {
                                    pu1DstWrite += 2;
                                }
                                else    // for lint happy
                                {
                                    ;
                                }
                            }
                        }
                        else    // u4KeynotEn = 0
                        {
                            if ((ai4DstColorComp[0] >= ai4ColorCompMin[0]) && 
                                (ai4DstColorComp[0] <= ai4ColorCompMax[0]) &&
                                (ai4DstColorComp[1] >= ai4ColorCompMin[1]) && 
                                (ai4DstColorComp[1] <= ai4ColorCompMax[1]) &&
                                (ai4DstColorComp[2] >= ai4ColorCompMin[2]) && 
                                (ai4DstColorComp[2] <= ai4ColorCompMax[2]) &&
                                (ai4DstColorComp[3] >= ai4ColorCompMin[3]) && 
                                (ai4DstColorComp[3] <= ai4ColorCompMax[3]))
                            {
                                // keep dst color
                                if ((UINT32)CM_ARGB8888_DIRECT32 == u4DstCM)
                                {
                                    pu1DstWrite += 4;
                                }
                                else if (((UINT32)CM_ARGB4444_DIRECT16 == u4DstCM) ||
                                         ((UINT32)CM_ARGB1555_DIRECT16 == u4DstCM) ||
                                         ((UINT32)CM_RGB565_DIRECT16   == u4DstCM))
                                {
                                    pu1DstWrite += 2;
                                }
                                else    // for lint happy
                                {
                                    ;
                                }
                            }
                            else
                            {
                                // write src color to dst
                                Loader_SwSetColorComponent(&pu1DstWrite, u4DstCM, 
                                    ai4SrcColorComp);
                            }
                        }
                    }
                    else if (u4ColchgEn)
                    {
                        if (u4KeynotEn)
                        {
                            if ((ai4DstColorComp[0] >= ai4ColorCompMin[0]) && 
                                (ai4DstColorComp[0] <= ai4ColorCompMax[0]) &&
                                (ai4DstColorComp[1] >= ai4ColorCompMin[1]) && 
                                (ai4DstColorComp[1] <= ai4ColorCompMax[1]) &&
                                (ai4DstColorComp[2] >= ai4ColorCompMin[2]) && 
                                (ai4DstColorComp[2] <= ai4ColorCompMax[2]) &&
                                (ai4DstColorComp[3] >= ai4ColorCompMin[3]) && 
                                (ai4DstColorComp[3] <= ai4ColorCompMax[3]))
                            {
                                // write src color to dst
                                Loader_SwSetColorComponent(&pu1DstWrite, u4DstCM, 
                                    ai4SrcColorComp);
                            }
                            else
                            {
                                // write rect color to dst
                                Loader_SwSetColorComponent(&pu1DstWrite, u4DstCM, 
                                    ai4RectColor);
                            }
                        }
                        else    // u4KeynotEn = 0
                        {
                            if ((ai4DstColorComp[0] >= ai4ColorCompMin[0]) && 
                                (ai4DstColorComp[0] <= ai4ColorCompMax[0]) &&
                                (ai4DstColorComp[1] >= ai4ColorCompMin[1]) && 
                                (ai4DstColorComp[1] <= ai4ColorCompMax[1]) &&
                                (ai4DstColorComp[2] >= ai4ColorCompMin[2]) && 
                                (ai4DstColorComp[2] <= ai4ColorCompMax[2]) &&
                                (ai4DstColorComp[3] >= ai4ColorCompMin[3]) && 
                                (ai4DstColorComp[3] <= ai4ColorCompMax[3]))
                            {
                                // write rect color to dst
                                Loader_SwSetColorComponent(&pu1DstWrite, u4DstCM, 
                                    ai4RectColor);
                            }
                            else
                            {
                                // write src color to dst
                                Loader_SwSetColorComponent(&pu1DstWrite, u4DstCM, 
                                    ai4SrcColorComp);
                            }
                        }
                    }
                    else        // u4TransEn = 0 and u4ColchgEn = 0
                    {           // write src color to dst
                        Loader_SwSetColorComponent(&pu1DstWrite, u4DstCM, ai4SrcColorComp);
                    }
                }
            }       //---------------------------------------------------
            else    // key compared with source pixel (src color key)
            {       //---------------------------------------------------
                if ((u4SrcCM == u4DstCM) && ((UINT32)CM_RGB_CLUT8 == u4SrcCM))
                {
                    if (u4TransEn)
                    {
                        if (u4KeynotEn)
                        {
                            // exactly equal color_key_min[7:0]
                            if (ai4SrcColorComp[0] == ai4ColorCompMin[3])
                            {
                                // write src color to dst
                                Loader_SwSetColorComponent(&pu1DstWrite, u4DstCM, 
                                    ai4SrcColorComp);
                            }
                            else
                            {
                                // keep dst color
                                pu1DstWrite += 1;
                            }
                        }
                        else    // u4KeynotEn = 0
                        {
                            // exactly equal color_key_min[7:0]
                            if (ai4SrcColorComp[0] == ai4ColorCompMin[3])
                            {
                                // keep dst color
                                pu1DstWrite += 1;
                            }
                            else
                            {
                                // write src color to dst
                                Loader_SwSetColorComponent(&pu1DstWrite, u4DstCM, 
                                    ai4SrcColorComp);
                            }
                        }
                    }
                    else if (u4ColchgEn)
                    {
                        if (u4KeynotEn)
                        {
                            // exactly equal color_key_min[7:0]
                            if (ai4SrcColorComp[0] == ai4ColorCompMin[3])
                            {
                                // write src color to dst
                                Loader_SwSetColorComponent(&pu1DstWrite, u4DstCM, 
                                    ai4SrcColorComp);
                            }
                            else
                            {
                                // write rect color to dst
                                Loader_SwSetColorComponent(&pu1DstWrite, u4DstCM, 
                                    ai4RectColor);
                            }
                        }
                        else    // u4KeynotEn = 0
                        {
                            // exactly equal color_key_min[7:0]
                            if (ai4SrcColorComp[0] == ai4ColorCompMin[3])
                            {
                                // write rect color to dst
                                Loader_SwSetColorComponent(&pu1DstWrite, u4DstCM, 
                                    ai4RectColor);
                            }
                            else
                            {
                                // write src color to dst
                                Loader_SwSetColorComponent(&pu1DstWrite, u4DstCM, 
                                    ai4SrcColorComp);
                            }
                        }
                    }
                    else        // u4TransEn = 0 and u4ColchgEn = 0
                    {           // write src color to dst
                        Loader_SwSetColorComponent(&pu1DstWrite, u4DstCM, ai4SrcColorComp);
                    }

                }
                else    // 32bpp and 16bpp
                {
                    if (u4TransEn)
                    {
                        if (u4KeynotEn)
                        {
                            if ((ai4SrcColorComp[0] >= ai4ColorCompMin[0]) && 
                                (ai4SrcColorComp[0] <= ai4ColorCompMax[0]) &&
                                (ai4SrcColorComp[1] >= ai4ColorCompMin[1]) && 
                                (ai4SrcColorComp[1] <= ai4ColorCompMax[1]) &&
                                (ai4SrcColorComp[2] >= ai4ColorCompMin[2]) && 
                                (ai4SrcColorComp[2] <= ai4ColorCompMax[2]) &&
                                (ai4SrcColorComp[3] >= ai4ColorCompMin[3]) && 
                                (ai4SrcColorComp[3] <= ai4ColorCompMax[3]))
                            {
                                // write src color to dst
                                Loader_SwSetColorComponent(&pu1DstWrite, u4DstCM, 
                                    ai4SrcColorComp);
                            }
                            else
                            {
                                // keep dst color
                                if ((UINT32)CM_ARGB8888_DIRECT32 == u4DstCM)
                                {
                                    pu1DstWrite += 4;
                                }
                                else if (((UINT32)CM_ARGB4444_DIRECT16 == u4DstCM) ||
                                         ((UINT32)CM_ARGB1555_DIRECT16 == u4DstCM) ||
                                         ((UINT32)CM_RGB565_DIRECT16   == u4DstCM))
                                {
                                    pu1DstWrite += 2;
                                }
                                else    // for lint happy
                                {
                                    ;
                                }
                            }
                        }
                        else    // u4KeynotEn = 0
                        {
                            if ((ai4SrcColorComp[0] >= ai4ColorCompMin[0]) && 
                                (ai4SrcColorComp[0] <= ai4ColorCompMax[0]) &&
                                (ai4SrcColorComp[1] >= ai4ColorCompMin[1]) && 
                                (ai4SrcColorComp[1] <= ai4ColorCompMax[1]) &&
                                (ai4SrcColorComp[2] >= ai4ColorCompMin[2]) && 
                                (ai4SrcColorComp[2] <= ai4ColorCompMax[2]) &&
                                (ai4SrcColorComp[3] >= ai4ColorCompMin[3]) && 
                                (ai4SrcColorComp[3] <= ai4ColorCompMax[3]))
                            {
                                // keep dst color
                                if ((UINT32)CM_ARGB8888_DIRECT32 == u4DstCM)
                                {
                                    pu1DstWrite += 4;
                                }
                                else if (((UINT32)CM_ARGB4444_DIRECT16 == u4DstCM) ||
                                         ((UINT32)CM_ARGB1555_DIRECT16 == u4DstCM) ||
                                         ((UINT32)CM_RGB565_DIRECT16   == u4DstCM))
                                {
                                    pu1DstWrite += 2;
                                }
                                else    // for lint happy
                                {
                                    ;
                                }
                            }
                            else
                            {
                                // write src color to dst
                                Loader_SwSetColorComponent(&pu1DstWrite, u4DstCM, 
                                    ai4SrcColorComp);
                            }
                        }
                    }
                    else if (u4ColchgEn)
                    {
                        if (u4KeynotEn)
                        {
                            if ((ai4SrcColorComp[0] >= ai4ColorCompMin[0]) && 
                                (ai4SrcColorComp[0] <= ai4ColorCompMax[0]) &&
                                (ai4SrcColorComp[1] >= ai4ColorCompMin[1]) && 
                                (ai4SrcColorComp[1] <= ai4ColorCompMax[1]) &&
                                (ai4SrcColorComp[2] >= ai4ColorCompMin[2]) && 
                                (ai4SrcColorComp[2] <= ai4ColorCompMax[2]) &&
                                (ai4SrcColorComp[3] >= ai4ColorCompMin[3]) && 
                                (ai4SrcColorComp[3] <= ai4ColorCompMax[3]))
                            {
                                // write src color to dst
                                Loader_SwSetColorComponent(&pu1DstWrite, u4DstCM, 
                                    ai4SrcColorComp);
                            }
                            else
                            {
                                // write rect color to dst
                                Loader_SwSetColorComponent(&pu1DstWrite, u4DstCM, 
                                    ai4RectColor);
                            }
                        }
                        else    // u4KeynotEn = 0
                        {
                            if ((ai4SrcColorComp[0] >= ai4ColorCompMin[0]) && 
                                (ai4SrcColorComp[0] <= ai4ColorCompMax[0]) &&
                                (ai4SrcColorComp[1] >= ai4ColorCompMin[1]) && 
                                (ai4SrcColorComp[1] <= ai4ColorCompMax[1]) &&
                                (ai4SrcColorComp[2] >= ai4ColorCompMin[2]) && 
                                (ai4SrcColorComp[2] <= ai4ColorCompMax[2]) &&
                                (ai4SrcColorComp[3] >= ai4ColorCompMin[3]) && 
                                (ai4SrcColorComp[3] <= ai4ColorCompMax[3]))
                            {
                                // write rect color to dst
                                Loader_SwSetColorComponent(&pu1DstWrite, u4DstCM, 
                                    ai4RectColor);
                            }
                            else
                            {
                                // write src color to dst
                                Loader_SwSetColorComponent(&pu1DstWrite, u4DstCM, 
                                    ai4SrcColorComp);
                            }
                        }
                    }
                    else        // u4TransEn = 0 and u4ColchgEn = 0
                    {           // write src color to dst
                        Loader_SwSetColorComponent(&pu1DstWrite, u4DstCM, ai4SrcColorComp);
                    }
                }
            }
        } // ~for
        
        pu1SrcLine += u4SrcPitch;
        pu1DstLine += u4DstPitch;

    } // ~for
    
    return (INT32)0;
}


void Splash_DrawImage(unsigned int x, unsigned int y, unsigned int u4Width, 
	unsigned int u4Height, unsigned int u4BmpAddr, int panelRes)
{
    UINT32 u4RegionList, u4Region, u4FBPitch = 0, u4BmpPitch = 0;
    UINT32 u4PanelWidth, u4PanelHeight;
    INT32 ret;
    UINT32 u4ColorMode = 14; // ARGB8888
    UINT32 u4FBBuffer = 0, u4HeaderBuffer = 0, u4FirstRegionAddr = 0;
    UINT8 *pu1TmpBuf = NULL;
	
    u4FBBuffer = TOTAL_MEM_SIZE - 0xB00000;
    u4HeaderBuffer = TOTAL_MEM_SIZE - 0xC00000;


    Printf("Color:%d BmpAddr:0x%08x Width:%d Height:%d\n", (int)u4ColorMode, u4BmpAddr, (int)u4Width, (int)u4Height);
#if 1
    ret = decode_bmp(u4BmpAddr, &u4Width, &u4Height, &pu1TmpBuf);
    if (ret != BMPR_OK)
    {
	return 1;
    }
    u4BmpAddr = pu1TmpBuf;
#endif

  //printf("test debug Splash_DrawImage 2222222222222 u4FBBuffer =0x%x  x= %d, y=%d\n",u4FBBuffer, x, y);

    if(!fgBootLogoInit)
    {
        fgBootLogoInit = 1;

        OSD_Init();
		
		//_OSD_BASE_SetScrnHStartOsd2(wDrvGetHsyncBp() - 2);
        //_OSD_BASE_UpdateHwReg();
		vDrvFireImportPortection();
		
        //GFX_Reset();
        u4PanelWidth = PANEL_GetPanelWidth();
        u4PanelHeight = PANEL_GetPanelHeight();

        Printf("Panel %d x %d \n", (int)u4PanelWidth, (int)u4PanelHeight);
        memset((void *)u4FBBuffer, 0 , 1920*1080*4);
	memset((void *)u4HeaderBuffer, 0 , 32);
	
        ret = OSD_RGN_LIST_Create(&u4RegionList);
        if (ret != OSD_RET_OK) return 1;
        // to set u4BmpPitch by u4ColorMode and u4Width.
        OSD_GET_PITCH_SIZE(u4ColorMode, UBOOT_LOGO_WIDTH, u4FBPitch); 
       //  hongjun marked 
#ifndef UBOOT_HACK	
		if(GetCurrentPanelIndex() == PANEL_PDP_HD_60HZ)   
#else
		if(0) //GetCurrentPanelIndex() == PANEL_PDP_HD_60HZ)   
#endif
		{
#if 1	
			ret = OSD_RGN_Create(&u4Region, 1920, 450, (void *)u4FBBuffer,
                                u4ColorMode, u4FBPitch, 0, 206, 1024, 320); 
#else	
			ret = OSD_RGN_Create(&u4Region, 1920, 1080, (void *)u4FBBuffer,
                                u4ColorMode, u4FBPitch, 0, 0, 1024, 768);
#endif								
		}
		else
		{
#if 1
			if(panelRes == 1)//FHD
			{
				ret = OSD_RGN_Create(&u4Region, 1920, 450, (void *)u4FBBuffer,
                                u4ColorMode, u4FBPitch, 0, 290, 1920, 450);
			}
			else
			{
				ret = OSD_RGN_Create(&u4Region, 1920, 450, (void *)u4FBBuffer,
              	                u4ColorMode, u4FBPitch, 0, 206, 1366, 320);
			}				
#else		
			ret = OSD_RGN_Create(&u4Region, UBOOT_LOGO_WIDTH, UBOOT_LOGO_HEIGHT, (void *)u4FBBuffer,
                                u4ColorMode, u4FBPitch, 0, 0, UBOOT_LOGO_WIDTH, UBOOT_LOGO_HEIGHT);
#endif								

		}
        if (ret != OSD_RET_OK) return 2;
        ret = OSD_RGN_Insert(u4Region, u4RegionList);
        if (ret != OSD_RET_OK) return 3;
        ret = OSD_PLA_FlipTo(OSD_PLANE_2, u4RegionList);
        if (ret != OSD_RET_OK) return 4;
/*
        ret = OSD_RGN_Set(u4Region, OSD_RGN_POS_X, x);
        if (ret != OSD_RET_OK) return 5;
        ret = OSD_RGN_Set(u4Region, OSD_RGN_POS_Y, y);
        if (ret != OSD_RET_OK) return 6;
*/
        ret = OSD_RGN_SetBigEndian(u4Region, TRUE);
        if (ret != OSD_RET_OK) return 8;

	_OSD_RGN_GetAddress((UINT32)u4Region, &u4FirstRegionAddr);

    memcpy((void*)u4HeaderBuffer, (void*)u4FirstRegionAddr, sizeof(OSD_RGN_UNION_T));

	_OSD_PLA_SetHeaderAddr(OSD_PLANE_2, u4HeaderBuffer);

	u4FBBuffer += u4FBPitch*y + x*4;
	
	//printf("test debug Splash_DrawImage 44444444444444444 u4FBBuffer =0x%x	x= %d, y=%d\n",u4FBBuffer, x, y);
	OSD_GET_PITCH_SIZE(u4ColorMode, u4Width, u4BmpPitch);
        Loader_SwBitBlt((UINT8 *)u4BmpAddr, (UINT8 *)u4FBBuffer, u4BmpPitch, u4FBPitch, u4ColorMode, u4ColorMode, u4Width,
   	u4Height, 0, 0, 0, 0, 0, 0, 0);
	
        HalFlushDCache();

#if 0 //def OSD_4K2K_SUPPORT
		if (u4PanelWidth == 1920*2 && u4PanelHeight == 1080*2)
		{
			vScpipOsdTgSrcSel(1);
			vPscSetData(1);
		}
#endif

		OSD_SC_SetRegion(OSD_SCALER_2, u4Region);
		OSD_SC_Scale(OSD_SCALER_2, 1, 1920, 1080, u4PanelWidth, u4PanelHeight);
		OSD_PLA_Enable(OSD_PLANE_1, FALSE);
		OSD_PLA_Enable(OSD_PLANE_3, FALSE);
		ret = OSD_PLA_Enable(OSD_PLANE_2, TRUE);
		if (ret != OSD_RET_OK) return 9;
    }
    else
    {
        OSD_GET_PITCH_SIZE(u4ColorMode, u4Width, u4BmpPitch);
        OSD_GET_PITCH_SIZE(u4ColorMode, UBOOT_LOGO_WIDTH, u4FBPitch); 
	
        u4FBBuffer += u4FBPitch*y + x*4;
	
        //printf("test debug Splash_DrawImage 555555555 u4FBBuffer =0x%x	x= %d, y=%d\n",u4FBBuffer, x, y);
        Loader_SwBitBlt((UINT8 *)u4BmpAddr, (UINT8 *)u4FBBuffer, u4BmpPitch, u4FBPitch, u4ColorMode, u4ColorMode, u4Width,
   	                 u4Height, 0, 0, 0, 0, 0, 0, 0);
        HalFlushDCache();

    }
    return 0;
}

#if 0
//-----------------------------------------------------------------------------
/** OsdDisplay(): Setup OSD driver.
 *  This function is going to setup osd driver with bmp info.
 *
 *  @param  u4BmpAddr       The bmp data pointer.
 *  @param  u4Width         The width info of the bmp data.
 *  @param  u4Height        The height info of the bmp data.
 *  @retval 0               Success.
 *  @retval otherwise       Failed.
 */
//-----------------------------------------------------------------------------
UINT32 OsdDisplay(UINT32 u4ColorMode, UINT32 u4BmpAddr, UINT32 u4Width, UINT32 u4Height)
{
    UINT32 u4RegionList, u4Region, u4BmpPitch;
    UINT32 u4PanelWidth, u4PanelHeight;
    INT32 ret;

    Printf("Color:%d BmpAddr:0x%08x Width:%d Height:%d\n", (int)u4ColorMode, u4BmpAddr, (int)u4Width, (int)u4Height);

    OSD_Init();

    u4PanelWidth = PANEL_GetPanelWidth();
    u4PanelHeight = PANEL_GetPanelHeight();

    Printf("Panel %d x %d \n", (int)u4PanelWidth, (int)u4PanelHeight);

    ret = OSD_RGN_LIST_Create(&u4RegionList);
    if (ret != OSD_RET_OK) return 1;
    u4BmpPitch = 0;
    OSD_GET_PITCH_SIZE(u4ColorMode, u4Width, u4BmpPitch); // to set u4BmpPitch by u4ColorMode and u4Width.
    ret = OSD_RGN_Create(&u4Region, u4Width, u4Height, (void *)u4BmpAddr,
                            u4ColorMode, u4BmpPitch, 0, 0, u4Width, u4Height);
    if (ret != OSD_RET_OK) return 2;
    ret = OSD_RGN_Insert(u4Region, u4RegionList);
    if (ret != OSD_RET_OK) return 3;
    ret = OSD_PLA_FlipTo(OSD_PLANE_2, u4RegionList);
    if (ret != OSD_RET_OK) return 4;

#if 1   // original bmp size.
    ret = OSD_RGN_Set(u4Region, OSD_RGN_POS_X, (u4PanelWidth - u4Width) >> 1);
    if (ret != OSD_RET_OK) return 5;
    ret = OSD_RGN_Set(u4Region, OSD_RGN_POS_Y, (u4PanelHeight - u4Height) >> 1);
    if (ret != OSD_RET_OK) return 6;
#else   // scale size.
    ret = OSD_SC_Scale(OSD_SCALER_2, 1, u4Width, u4Height, u4PanelWidth, u4PanelHeight);
    if (ret != OSD_RET_OK) return 7;
#endif

    ret = OSD_RGN_SetBigEndian(u4Region, TRUE);
    if (ret != OSD_RET_OK) return 8;
    
    ret = OSD_PLA_Enable(OSD_PLANE_2, TRUE);
    if (ret != OSD_RET_OK) return 9;

    return 0;
}

//-----------------------------------------------------------------------------
/** PanelLogo(): display logo on panel.
 *  This function is going to find out the logo location and call lvds/pmx/osd 
 *  low level driver to display logo on panel.
 *
 *  @param  u4BmpAddr       The memory offset to store logo file.
 *  @retval 0               Success.
 *  @retval otherwise       Failed.
 */
//-----------------------------------------------------------------------------
UINT32 PanelLogo(UINT32 u4BmpAddr)
{
    UINT32 u4Ret;

    Printf("Display background:0x%08x\n", (unsigned int)DRVCUST_InitGet(eLoaderLogoBackground));
    PmxDisplay(DRVCUST_InitGet(eLoaderLogoBackground));
    Printf("OsdDisplay(%d, 0x%08x, %d, %d)\n",
                    LOADER_LOGO_COLOR_MODE,
                    (unsigned int)u4BmpAddr,
                    LOADER_LOGO_WIDTH,
                    LOADER_LOGO_HEIGHT);
    u4Ret = OsdDisplay(LOADER_LOGO_COLOR_MODE,
                    u4BmpAddr,
                    LOADER_LOGO_WIDTH,
                    LOADER_LOGO_HEIGHT);

    return u4Ret;
}
#endif

static void ClearToColor(UINT32* addr, UINT32 width, UINT32 height, UINT32 color)
{
    UINT32 size = width*height, i;

    for (i=0; i<size; i+=1)
    {
        *(addr+i) = color;
    }
}

UINT32 LogoDemo(void)
{
    UINT32 u4Ret, u4BmpAddr;

    //u4BmpAddr = DRVCUST_InitGet(eFbmMemAddr) - DRVCUST_InitGet(eDirectFBMemSize);
    u4BmpAddr = TOTAL_MEM_SIZE - FBM_MEM_CFG_SIZE;

    Printf("(yjdbg) dFB start address: 0x%x\n", u4BmpAddr);
    //ClearToColor(u4BmpAddr, LOADER_LOGO_WIDTH, LOADER_LOGO_HEIGHT, 0xff0000ff);
//    Printf("bg_graphic[0] = %d.\n", bg_graphic[0]);
//    memcpy((UINT8*)u4BmpAddr, bg_graphic, sizeof(bg_graphic));
    #if 1
    Printf("(yjdbg) vDrvMLVDSPatternGen\n");
    vDrvMLVDSPatternGen(1);
    x_thread_delay(10*100);
    vDrvMLVDSPatternGen(0);
    #endif
    //Printf("(yjdbg) Splash_DrawImage again\n");
    Splash_DrawImage(0, 0, 1280, 200, u4BmpAddr,1);
    x_thread_delay(10*100);

    Printf("(yjdbg) dFB start address: 0x%x\n", u4BmpAddr);
    Splash_DrawImage(0, 800, 1280, 200, u4BmpAddr,1);
    x_thread_delay(10*100);

    return u4Ret;
}

#ifdef CFG_LG_CHG
u32 Get_DrawAddr(void)
{
    return TOTAL_MEM_SIZE - 0x2000000;
}
#endif

#endif // LOADER_LOGO_FLASHOFFSET

