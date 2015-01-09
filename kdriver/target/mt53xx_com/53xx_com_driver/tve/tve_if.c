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
*  TVE: Interface
*****************************************************************************/

#ifndef _TVE_IF_C_
#define _TVE_IF_C_

#include "x_assert.h"
#include "tve_if.h"
#include "mpv_drvif.h" 
#include "tve_hal.h"
#include "tve_debug.h"
#include "tve_hw.h"

#ifdef TVE_CS4954
#include "tve_cs4954.h"
#endif
#include "sv_const.h"
#include "source_table.h"
#include "drv_scaler.h"
#include "vdp_if.h"
#include "vdp_drvif.h"
#include "drv_display.h"
#include "hw_scpos.h"
#include "vdec_if.h"
#include "drvcust_if.h"
#include "hw_di.h"
#include "drv_di.h"
#include "vdo_misc.h"
#include "util.h"
#include "video_def.h"
#include "mute_if.h"
#include "x_bim.h"
#include "drv_scaler.h"
#include "feature.h"
#if (defined(CC_MT5881) || defined(CC_MT5399) || defined(CC_MT5890)|| defined(CC_MT5882))
#include "drv_mon_out.h"
#include "hw_pdwnc.h"
#endif
#define TVE_OUT_CC_VCHIP    // cc and v-chip test

/******************************************************************************
* local definition, TVE configuration
******************************************************************************/
/* TVE configuration */
typedef struct _TVE_CONF_T
{
    UCHAR       ucTveEnable;
    UCHAR       ucMode;
    UCHAR       ucFmt;
    UCHAR       ucImpedence;
    UCHAR       ucCcEnable;
    //UCHAR       ucCcBufferExist[MPV_MAX_ES];
    //UCHAR       ucCcPlay[MPV_MAX_ES];
    UCHAR       ucTTXEnable;
    //UCHAR       ucMvEnable;
    //UCHAR       ucMvTestNum;
    UCHAR       ucWssEnable;
#ifdef CC_TVE_INSERT_VPS//  for vps
	UCHAR		ucVPSEnable;
#endif		
    UCHAR       ucAspectRatio;
    UCHAR       ucCgmsaInfo;
    UCHAR       ucDAC1Enable;
    UCHAR       ucDAC2Enable;
} TVE_CONF_T;

#if (defined(CC_MT5881)||defined(CC_MT5399)|| defined(CC_MT5890)|| defined(CC_MT5882))
#if CC_TVE_SUPPORT_VDAC_SOFTWARE_CAL
extern UINT8 _bTveSoftCalState;
#endif
#endif

//#include "tve_if_conf.h" 
/******************************************************************************
* Function prototype
******************************************************************************/
static void _TveIfStatus(UCHAR ucTveId);
#if 0
static void _TveMainCcNfyFct(void *pvTag, MPV_CC_COND_T rCcCond, void *pvArg);
static void _TveAuxCcNfyFct(void *pvTag, MPV_CC_COND_T rCcCond, void *pvArg);
#endif

/******************************************************************************
* local variable
******************************************************************************/
#ifdef CC_SUPPORT_STR
static BOOL _fgTveResumeFlow=FALSE;
#endif
static UCHAR _TveInitiated = 0;
static TVE_CONF_T _arTveConf[TVE_MAX_NS];
static BOOL _bBypassFromMonOut = FALSE;
#if 0
static UCHAR _ucEsId = 0;
#endif

#ifdef CC_TVE_SUPPORT_CGMSA   //this setting is just for sony!!!
/* Wss 625-Line Aspect Ratio Table */   //for sony request CGMS-A
static UCHAR _uc625LAsrTable2Wss[2][16] ={
{// 4:3
    0x1, 0x1, 0x2, 0x4, 0xB, 0x1, 0x1, 0x1,
    0x1, 0x1, 0xD, 0x8, 0x1, 0x7, 0xD, 0xD
},
{// 16:9
    0xE, 0xE, 0xE, 0xE, 0xE, 0xE, 0xE, 0xE,
    0xE, 0xE, 0xE, 0xE, 0xE, 0xE, 0xE, 0xE
}
};
#else
/* Wss 625-Line Aspect Ratio Table */
static UCHAR _uc625LAsrTable2Wss[2][16] ={
{// 4:3
    0x1, 0x1, 0x2, 0x4, 0xB, 0x1, 0x1, 0x1,
    0x1, 0x1, 0xD, 0x8, 0x1, 0x7, 0xD, 0xD
},
{// 16:9
    0xE, 0xE, 0xE, 0xE, 0xE, 0xE, 0xE, 0xE,
    0xE, 0x1, 0xE, 0xE, 0xE, 0x7, 0xE, 0xE
}
};
#endif


/******************************************************************************
* local macro
******************************************************************************/
#define VERIFY_TVE_ID(id)                                   \
          do {                                              \
            if ((id) >= TVE_MAX_NS) { return TVE_SET_ERROR; }     \
          } while (0)
#define VERIFY_DAC_ID(id)                                   \
          do {                                              \
            if ((id) >= DAC_MAX) { return TVE_SET_ERROR; }     \
          } while (0)

#define NTSC_CC_VALID           0x4
#define NTSC_CC_TYPE_MASK       0x3
#define NTSC_CC_TYPE_FIELD1     0x0
#define NTSC_CC_TYPE_FIELD2     0x1

#define NTSC_CC_PACKET_SIZE     2       // 2 bytes per packet
#define NTSC_CC_BUF_SIZE        2560    // 2560 bytes

//#define TTX_PACKET_SIZE     45       // 45 bytes per packet: field parity+line offset+framing_code+ttx_data_block
//#define TTX_PACKET_FIELD_PARITY     0x80
//#define TTX_PACKET_LINE_OFFSET     0x1f

/*-----------------------------------------------------------------
// CC Testing Path 03: Cooperate with YT demo program
// Imitate the MiddleWare behavior: set callback function for MPV
//-----------------------------------------------------------------*/
#if 0
static void _TveMainCcNfyFct(void *pvTag, MPV_CC_COND_T rCcCond, void *pvArg)
{
    INT32 i, idx;
    UINT32 u4ValidPackets, u4TotalPackets;
    UINT32 u4RetVal = TVE_SET_OK;
    UCHAR *pucTag = NULL;
    MPV_CC_DATA rCcData;
    UCHAR ucCcData[NTSC_CC_BUF_SIZE];
    UCHAR ucAtscCc[2];
    UCHAR ucCcType;
    PTS_T u8Pts;

    rCcData.u4Size = NTSC_CC_BUF_SIZE;
    rCcData.pucCcData = ucCcData;
        
    if (rCcCond == MPV_CC_COND_DATA_ARRIVAL)
    {
        if (pvTag != NULL)
        {
            pucTag = (UCHAR*) pvTag;
//            VERIFY(MPV_GetAnalogCc(*pucTag, &rCcData) == MPV_DRV_OK);
        }

        u4ValidPackets = 0;
        u4TotalPackets = rCcData.u4Size / NTSC_CC_PACKET_SIZE;
        for (i = 0; i < u4TotalPackets; i++)
        {
            idx = (i * NTSC_CC_PACKET_SIZE);
            if (rCcData.pucCcData[idx + 1] & NTSC_CC_VALID)
            {
                u4ValidPackets++;

                ucCcType = (UCHAR)(rCcData.pucCcData[idx + 1] & NTSC_CC_TYPE_MASK);

                if (ucCcType == NTSC_CC_TYPE_FIELD1)    // line 21 (22)
                {
                    LOG(8, "(Main) NTSC CC1, CC2, T1, T2 Valid\n");
                    
                    ucAtscCc[0] = rCcData.pucCcData[idx + 2];
                    ucAtscCc[1] = rCcData.pucCcData[idx + 3];
                    u8Pts = rCcData.u8Pts;
                    
                    u4RetVal = TVE_SendCc(TVE_1, TVE_FIELD_ODD, ucAtscCc, u8Pts);
                    if (u4RetVal != TVE_SET_OK)
                    {
                        LOG(8, "(Main) TVE_SendCc Invalid\n");
                    }
                }

                if (ucCcType == NTSC_CC_TYPE_FIELD2)    // line 284 (335)
                {
                    LOG(8, "(Main) NTSC CC3, CC4, T3, T4, XDS Valid\n");
                    
                    ucAtscCc[0] = rCcData.pucCcData[idx + 2];
                    ucAtscCc[1] = rCcData.pucCcData[idx + 3];
                    u8Pts = rCcData.u8Pts;

                    u4RetVal = TVE_SendCc(TVE_1, TVE_FIELD_EVEN, ucAtscCc, u8Pts);
                    if (u4RetVal != TVE_SET_OK)
                    {
                        LOG(8, "(Main) TVE_SendCc Invalid\n");
                    }
                }
            }
        } // ~for

        LOG(8, "(Main) NTSC CC Valid Packet = %u\n", u4ValidPackets);
    }

    return;
}
#endif
/*-----------------------------------------------------------------
// CC Testing Path 03: Cooperate with YT demo program
// Imitate the MiddleWare behavior: set callback function for MPV
//-----------------------------------------------------------------*/
#if 0
static void _TveAuxCcNfyFct(void *pvTag, MPV_CC_COND_T rCcCond, void *pvArg)
{
    UINT32 u4RetVal = TVE_SET_OK;
    UCHAR *pucTag = 0;
    MPV_CC_DATA rCcData;
    UCHAR ucCcData[512], ucAtscCc[2], ucCcType;
    rCcData.u4Size = 512;
    rCcData.pucCcData = ucCcData;
        
    if (rCcCond == MPV_CC_COND_DATA_ARRIVAL)
    {
        if (pvTag != NULL)
        {
            pucTag = (UCHAR*) pvTag;
            u4RetVal = (UINT32) MPV_GetCc(*pucTag, &rCcData);
        }
        
        // Parse the cc_type of pucCcData[1] 
        // pucCcData[0] = reserved (refer to MW Video Decoder Document) 
        if (rCcData.pucCcData[1] & NTSC_CC_VALID)  // cc_type == 1 ?
        {
            ucCcType = (UCHAR)(rCcData.pucCcData[1] & NTSC_CC_TYPE_MASK);
            if (ucCcType == NTSC_CC_TYPE_FIELD1)
            {
                LOG(5, "(Aux) NTSC CC1, CC2, T1, T2 Valid\n");
                ucAtscCc[0] = rCcData.pucCcData[2];
                ucAtscCc[1] = rCcData.pucCcData[3];
            
                u4RetVal = TVE_SendCc(TVE_2, TVE_FIELD_ODD, ucAtscCc, sizeof(ucAtscCc));
                if (u4RetVal != TVE_SET_OK)
                {
                    LOG(5, "(Aux) TVE_SendCc Invalid\n");
                }
            }
            else if (ucCcType == NTSC_CC_TYPE_FIELD2)
            {
                LOG(5, "(Aux) NTSC CC3, CC4, T3, T4, XDS Valid\n");
                ucAtscCc[0] = rCcData.pucCcData[2];
                ucAtscCc[1] = rCcData.pucCcData[3];
            
                u4RetVal = TVE_SendCc(TVE_2, TVE_FIELD_EVEN, ucAtscCc, sizeof(ucAtscCc));
                if (u4RetVal != TVE_SET_OK)
                {
                    LOG(5, "(Aux) TVE_SendCc Invalid\n");
                }
            }
        }        
        else
        {
            LOG(5, "(Aux) CC Invalid\n");
        }
    }

    return;
}
#endif

/*----------------------------------------------------------------------------
 * Function: _TVEISR
 *
 * Description:
 *      ISR of TVE
 * Inputs:
 *      u2Vector: The IRQ vector, must be VECTOR_TVE
 *
 * Outputs: -
 *
 * Returns: -
 *---------------------------------------------------------------------------*/
#if defined(CC_MT5363) || defined(CC_TVE6595)
static VOID _TVEISR(UINT16 u2Vector)
{
#ifdef TVE_WA
    UINT8 ucEnable;
#endif

    // Must be this IRQ
    VERIFY(u2Vector == VECTOR_TVE);
    #ifdef CC_SCPOS_3DTV_SUPPORT
    vScpipOnTVEInputVSync();
    #endif
    
#ifdef TVE_WA
    //TVE ISR function  
    TVE_GetEnable(TVE_1, &ucEnable);
    if(ucEnable)
    {
        TVE_OnOutputVSync();
    }
#endif
    
#if 0
#ifdef CC_SUPPORT_TVE
        // call tve isr, for VBI CC and TTX
        TVE_GetCcEnable(TVE_1, &ucEnable);
        if (ucEnable)
        {
            TVE_HalSendCcIsr(TVE_1, TVE_GetFieldInfo(TVE_1), 0);
        }
        TVE_GetTTXEnable(TVE_1, &ucEnable);
        if (ucEnable)
        {
            TVE_HalSendTTXIsr(TVE_1, TVE_GetFieldInfo(TVE_1), 0);
        }
#endif
#endif
    // Clear Parser interrupt
    VERIFY(BIM_ClearIrq(VECTOR_TVE));
}
#endif
/*--------------------------------------------
// Init TVE
//--------------------------------------------*/
UINT32 TVE_Init(void)
{
#if defined(CC_MT5363) || defined(CC_TVE6595)
    x_os_isr_fct pfnOldIsr;
#endif
    UINT32 u4RetVal = TVE_SET_OK;
    //INT32 i;
#if defined(CC_MT5396) || defined(CC_MT5368) || defined(CC_MT5389)
    vRegWriteFldAlign(SCPIP_PIP_OA_14, 1, PIP_OA_14_TVE_FIELD_REPEAT_MODE);
#endif

    if (_TveInitiated == 0)
    {
        LOG(3, "TVE Init\n");
        x_memset(_arTveConf, 0, sizeof(_arTveConf));
        
        /* Reset TVE HW */
        u4RetVal = TVE_Reset(TVE_1);
        //u4RetVal = TVE_Reset(TVE_2);
        
        /* Close Caption init */
        TVE_HalCcInit(TVE_1);
        //TVE_HalCcInit(TVE_2);
        #ifdef LINUX_TURNKEY_SOLUTION
        TVE_HalTTXInit();
        #endif

        #ifdef CC_TVE_INSERT_VPS
        TVE_SetVPSEnable(TVE_1, FALSE);
        #endif
        // TVE_2 no mpv cc buffers
        //for (i = 0; i < MPV_MAX_ES; i++)
        //{
        //    _arTveConf[TVE_2].ucCcBufferExist[i] = (UCHAR)TRUE;
        //}

        /* Set Low Impedence for Demo Board */
        TVE_HalSetImpedence(TVE_1, TVE_LOW_IMPEDENCE);
        
        /* reg to mv's rm */

#ifdef TVE_CS4954
        TVE_CS4954_Init();
#endif

#if defined(CC_SUPPORT_TVE)
        // Register mute change notify function for mute/unmute TVE
        UNUSED(_vDrvVideoRegisterMuteNotify(vApiTVEVideoMuteChangeNotify,
                                            EVENT_VIDEO_MUTE_CHANGE, 
                                            SV_TVE_NOTIFY_VIDEO_MODE_CHANGE));
#endif
#if defined(CC_MT5363) || defined(CC_TVE6595)
        // Register TVE ISR
        #ifdef CC_SUPPORT_STR
        if(_fgTveResumeFlow!=TRUE)
        #endif
        {
            LOG(3, "Register _TVEISR\n");
            if (x_reg_isr(VECTOR_TVE, _TVEISR, &pfnOldIsr) != OSR_OK)
            {
                return FALSE;
            }
        }
#endif
        _TveInitiated = 1;
    }

    return u4RetVal;
}

/*--------------------------------------------
// Un-init TVE
//--------------------------------------------*/
UINT32 TVE_UnInit(void)
{
    if (_TveInitiated == 1)
    {
        /* enable ISR, thread */
        
        /* dereg to mv's rm */

        _TveInitiated = 0;
    }

#ifdef LINUX_TURNKEY_SOLUTION
    TVE_HalTTXUnInit();
#endif
    return TVE_SET_OK;
}

/*--------------------------------------------
// Send reset signal to TVE HW
//--------------------------------------------*/
UINT32 TVE_Reset(UCHAR ucTveId)
{
    UINT32 u4RetVal = TVE_SET_OK;

    TVE_HalReset(ucTveId);

#ifdef TVE_CS4954
    if (ucTveId == TVE_2)
    {
        return u4RetVal;    
    }    
#endif

    u4RetVal = TVE_SetEnable(ucTveId, 0);

    return u4RetVal;
}

/*--------------------------------------------
// Query TVE Main & Aux status
//--------------------------------------------*/
UINT32 TVE_QueryStatus(void)
{
    UCHAR ucTveId;

    for (ucTveId = 0; ucTveId < TVE_MAX_NS; ucTveId++)
    {
        _TveIfStatus(ucTveId);
    }
    
    return TVE_SET_OK;
}

/*--------------------------------------------
// Enable/disable TVE
//--------------------------------------------*/
UINT32 TVE_SetEnable(UCHAR ucTveId, UCHAR ucEnable)
{
    VERIFY_TVE_ID(ucTveId);

    _arTveConf[ucTveId].ucTveEnable = ucEnable;

#ifdef TVE_CS4954
    if (ucTveId == TVE_2)
    {
        TVE_CS4954_Enable(ucEnable);
        return TVE_SET_OK;    
    }    
#endif

    if (ucEnable)
    {
        TVE_HalEnable(ucTveId);
    }
    else
    {
    /* fixme: the bypass & tve setting use the same enable bit
     *        not to turn-off the tve in case bypass failed.   */
        TVE_HalDisable(ucTveId);
    }

    return TVE_SET_OK;
}

/*--------------------------------------------
// Get TVE enable/disable status
//--------------------------------------------*/
UINT32 TVE_GetEnable(UCHAR ucTveId, UCHAR* pucEnable)
{
    VERIFY_TVE_ID(ucTveId);

    if (pucEnable != NULL)
    {
        *pucEnable = _arTveConf[ucTveId].ucTveEnable;
    }

    return TVE_SET_OK;    
}

/*--------------------------------------------
// Set TVE output mode
//--------------------------------------------*/
UINT32 TVE_SetMode(UCHAR ucTveId, UCHAR ucMode)
{
    VERIFY_TVE_ID(ucTveId);

    _arTveConf[ucTveId].ucMode = ucMode;

#ifdef TVE_CS4954
    if (ucTveId == TVE_2)
    {
        /* only composite mode */
        return TVE_SET_OK;
    }
#endif

    TVE_HalSetMode(ucTveId, _arTveConf[ucTveId].ucFmt, ucMode);

    return TVE_SET_OK;    
}

/*--------------------------------------------
// Get TVE current output mode
//--------------------------------------------*/
UINT32 TVE_GetMode(UCHAR ucTveId, UCHAR* pucMode)
{
    VERIFY_TVE_ID(ucTveId);

    if (pucMode != NULL)
    {
        *pucMode = _arTveConf[ucTveId].ucMode;
    }

    return TVE_SET_OK;    
}

/*--------------------------------------------
// Set TVE format
//--------------------------------------------*/
UINT32 TVE_SetFmt(UCHAR ucTveId, UCHAR ucFmt)
{
    INT32 i4RetVal = TVE_SET_OK;
    UINT8 bSrc;
    VERIFY_TVE_ID(ucTveId);

    _arTveConf[ucTveId].ucFmt= ucFmt;   // Record the current TVE format
    if ((ucFmt != TVE_FMT_480I) && (ucFmt != TVE_FMT_576I))
    {
        // Others TVE Fmt (480p/576p/720p/1080i)
        // disable CC signal output
        //TVE_SetCcEnable(ucTveId, (UCHAR)FALSE);//TODO??
        #if defined(CC_MT5881)
        vMonOutNormProc(SV_OFF,TVE_FMT_480I);
        #endif
        return TVE_SET_ERROR;
    }
    #if defined(CC_MT5881)
    vMonOutNormProc(SV_ON,ucFmt);
	#elif (defined(CC_MT5399)|| defined(CC_MT5890)|| defined(CC_MT5882))
	if (TVE_GetEnableBypassMonOut(TVE_1))
	{		
		vMonOutSetOutput(E_MON_OUT_TVE);
		vMonOutSetTveFormat(ucFmt);
	}
	else
	{
		vScpipTveImportEn(1);
	    vScpipTveImportTg(0);
		if(ucFmt == TVE_FMT_480I)
		{
			vScpipSetTveTiming(0x2d0, 0x35a, 0x1e0, 0x20d);
		}
		else
		{
			vScpipSetTveTiming(0x2d0, 0x360, 0x240, 0x271);
		}

	    vScpipTveImportTg(1);
	}
    #elif (defined(CC_MT5398) || defined(CC_MT5880) || defined(CC_MT5860))
	vScpipTveImportEn(1);
    vScpipTveImportTg(0);
	if(ucFmt == TVE_FMT_480I)
	{
    #if 0
		vRegWriteFldAlign(SCPIP_SCSYS_16, 0x20d, SCSYS_16_SC_TVE_VTOTAL);
		vRegWriteFldAlign(SCPIP_SCSYS_16, 0x35a, SCSYS_16_SC_TVE_HTOTAL);
		vRegWriteFldAlign(SCPIP_SCSYS_17, 0x1e0, SCSYS_17_SC_TVE_HEIGHT);
		vRegWriteFldAlign(SCPIP_SCSYS_17, 0x2d0, SCSYS_17_SC_TVE_WIDTH);
	#endif
		vScpipSetTveTiming(0x2d0, 0x35a, 0x1e0, 0x20d);
	}
	else
	{
    #if 0
		vRegWriteFldAlign(SCPIP_SCSYS_16, 0x271, SCSYS_16_SC_TVE_VTOTAL);
		vRegWriteFldAlign(SCPIP_SCSYS_16, 0x360, SCSYS_16_SC_TVE_HTOTAL);
		vRegWriteFldAlign(SCPIP_SCSYS_17, 0x240, SCSYS_17_SC_TVE_HEIGHT);
		vRegWriteFldAlign(SCPIP_SCSYS_17, 0x2d0, SCSYS_17_SC_TVE_WIDTH);
	#endif
		vScpipSetTveTiming(0x2d0, 0x360, 0x240, 0x271);
	}

	//vRegWriteFldAlign(SCPIP_SCSYS_01, 0,SCSYS_01_SC_TVE_SET_TOGGLE);//import toggle
    //vRegWriteFldAlign(SCPIP_SCSYS_01, 1,SCSYS_01_SC_TVE_SET_TOGGLE);
    vScpipTveImportTg(1);
    #elif defined(CC_MT5387) && !defined(CC_MT5363)
/*    if(ucFmt == TVE_FMT_480I)
    {
        vRegWriteFldAlign(SCPIP_PIP_OA_02, 0, PIP_OA_02_TVE_PAL);//PAL
    }
    else
    {
        vRegWriteFldAlign(SCPIP_PIP_OA_02, 1, PIP_OA_02_TVE_PAL);//0x20025048[9];//PAL
    }    
*/
    #else //CC_MT5363 or TVE6595
    if(ucFmt == TVE_FMT_480I)
    {
        vRegWriteFldAlign(SCPIP_PIP_OA_02, 0, PIP_OA_02_TVE_PAL);//PAL
    }
    else
    {
        vRegWriteFldAlign(SCPIP_PIP_OA_02, 1, PIP_OA_02_TVE_PAL);//0x20025048[9];//PAL
    }    
    #endif

    TVE_HalSetInputSrc(&bSrc);
    Bypass_HalTVESyncTime(bSrc, ucFmt);    

#ifdef TVE_CS4954
    if (ucTveId == TVE_2)
    {
        TVE_CS4954_SetFmt(ucFmt);
        return TVE_SET_OK;    
    }    
#endif

    TVE_HalSetFmt(ucTveId, ucFmt, _arTveConf[ucTveId].ucMode);    

    i4RetVal = TVE_SET_OK;
    
    return (UINT32)i4RetVal;
}

/*--------------------------------------------
// Get TVE current format
//--------------------------------------------*/
UINT32 TVE_GetFmt(UCHAR ucTveId, UCHAR* pucFmt)
{
    VERIFY_TVE_ID(ucTveId);

    if (pucFmt != NULL)
    {
        *pucFmt = _arTveConf[ucTveId].ucFmt;
    }

    return TVE_SET_OK;    
}

/*--------------------------------------------
// Set TVE Impedence
//--------------------------------------------*/
UINT32 TVE_SetImpedence(UCHAR ucTveId, UCHAR ucImpedence)
{
    VERIFY_TVE_ID(ucTveId);

    _arTveConf[ucTveId].ucImpedence = ucImpedence;

    TVE_HalSetImpedence(ucTveId, ucImpedence);

    return TVE_SET_OK;
}

/*--------------------------------------------
// Get TVE Impedence
//--------------------------------------------*/
UINT32 TVE_GetImpedence(UCHAR ucTveId, UCHAR* pucImpedence)
{
    VERIFY_TVE_ID(ucTveId);

    if (pucImpedence != NULL)
    {
        *pucImpedence = _arTveConf[ucTveId].ucImpedence;
    }

    return TVE_SET_OK;
}

/*--------------------------------------------
// Set VDAC Current
//--------------------------------------------*/
UINT32 TVE_SetCurrent(UCHAR ucDacId, UCHAR ucCurrent)
{
    VERIFY_DAC_ID(ucDacId);

    TVE_HalSetCurrent(ucDacId, ucCurrent);

    return TVE_SET_OK;
}


/*--------------------------------------------
// Turn on/off color bar
//--------------------------------------------*/
UINT32 TVE_SetColorBar(UCHAR ucTveId, UCHAR ucOn)
{
    VERIFY_TVE_ID(ucTveId);

#ifdef TVE_CS4954
    if (ucTveId == TVE_2)
    {
        TVE_CS4954_SetColorBar(ucOn);
        return TVE_SET_OK;    
    }    
#endif

    TVE_HalColorBar(ucTveId, ucOn);
    
    return TVE_SET_OK;    
}

/*--------------------------------------------
// Set CC status
//--------------------------------------------*/
UINT32 TVE_SetCcEnable(UCHAR ucTveId, UCHAR ucEnable)
{        
    VERIFY_TVE_ID(ucTveId);

    _arTveConf[ucTveId].ucCcEnable = ucEnable;

#ifdef TVE_CS4954
    if (ucTveId == TVE_2)
    {
        return TVE_SET_ERROR;    
    }    
#endif

    TVE_HalEnableCc(ucTveId, ucEnable);
    
    return TVE_SET_OK;    
}

/*--------------------------------------------
// Get CC status
//--------------------------------------------*/
UINT32 TVE_GetCcEnable(UCHAR ucTveId, UCHAR* pucCcEnable)
{
    VERIFY_TVE_ID(ucTveId);
    
    if (pucCcEnable != NULL)
    {
        *pucCcEnable = _arTveConf[ucTveId].ucCcEnable;
    }
    
    return TVE_SET_OK;
}

/*--------------------------------------------
// Send Closed Caption
//--------------------------------------------*/
UINT32 TVE_SendCc(UCHAR ucTveId, UCHAR ucField, const UCHAR *pucData, PTS_T u8Pts)
{
    UINT32 u4RetVal = TVE_SET_OK;
    
    VERIFY_TVE_ID(ucTveId);

#ifdef TVE_CS4954
    if (ucTveId == TVE_2)
    {
        return TVE_SET_ERROR;    
    }    
#endif

    if (pucData != NULL)
    {
        u4RetVal = TVE_HalSend2CcQue(ucTveId, ucField, pucData, u8Pts);
    }

    return u4RetVal;
}

UINT32 TVE_SendCcIsr(UCHAR ucTveId, UCHAR ucIsOddField, UINT32 u4PmxPts)
{
    UINT32 u4RetVal = TVE_SET_OK;
    
    VERIFY_TVE_ID(ucTveId);
    TVE_HalSendCcIsr(ucTveId, ucIsOddField, u4PmxPts);

    return u4RetVal;
}

/*--------------------------------------------
// Play Closed Caption
//--------------------------------------------*/
#if 0
UINT32 TVE_PlayCc(UCHAR ucTveId, UCHAR ucEsId)
{
    MPV_CC_NFY_INFO_T mpv_cc_nfy_info;
    UINT32 u4RetVal;

    VERIFY_TVE_ID(ucTveId);


#ifdef TVE_CS4954
    if (ucTveId == TVE_2)
    {
        return TVE_SET_ERROR;    
    }    
#endif

    // if no CC buffer exist, then prepare CC buffers for all EsID
    if (_arTveConf[ucTveId].ucCcBufferExist[ucEsId] != (UCHAR)TRUE)
    {
        // Allocate CC buffers
//CCTEMP        u4RetVal = (UINT32) MPV_AllocAnalogCcBuf(ucEsId, NTSC_CC_BUF_SIZE);
        VERIFY(u4RetVal == (UINT32)MPV_DRV_OK);
            
        _arTveConf[ucTveId].ucCcBufferExist[ucEsId] = (UCHAR)TRUE;
    }

    // Reg callback func to MPV and call MPV to get CC data
    if (ucTveId == TVE_1)   // Main
    {
       mpv_cc_nfy_info.pfCcNfyFct = _TveMainCcNfyFct;
    }
    else                    // Aux
    {
        mpv_cc_nfy_info.pfCcNfyFct = _TveAuxCcNfyFct;
    }

    _ucEsId = ucEsId;
    mpv_cc_nfy_info.pvTag = (void *)(&_ucEsId);
//CCTEMP    VERIFY(MPV_SetAnalogCcNfyFct(_ucEsId, &mpv_cc_nfy_info) == MPV_DRV_OK);

    TVE_HalResetVChip();    // reset v-chip setting

    _arTveConf[ucTveId].ucCcPlay[_ucEsId] = (UCHAR)TRUE;
//CCTEMP    MPV_PlayAnalogCc(_ucEsId);

    return TVE_SET_OK;
}
#endif

/*--------------------------------------------
// Stop Closed Caption
//--------------------------------------------*/
UINT32 TVE_StopCc(UCHAR ucTveId, UCHAR ucEsId)
{
    // disable CC signal output
    TVE_SetCcEnable(ucTveId, (UCHAR)FALSE);

//    MPV_StopAnalogCc(ucEsId);
    //_arTveConf[ucTveId].ucCcPlay[ucEsId] = (UCHAR)FALSE;

//    MPV_FlushAnalogCcBuf(ucEsId);

    // reset TVE CC fifo
    TVE_HalResetCcFifo(ucTveId);

    return TVE_SET_OK;
}

/*--------------------------------------------
// Set TTX status
//--------------------------------------------*/
UINT32 TVE_SetTTXEnable(UCHAR ucTveId, UCHAR ucEnable)
{      
    //UINT32 i;
    VERIFY_TVE_ID(ucTveId);

    _arTveConf[ucTveId].ucTTXEnable = ucEnable;
    
    TVE_HalEnableTTX(ucTveId, ucEnable);
    
    return TVE_SET_OK;    
}

/*--------------------------------------------
// Get TTX status
//--------------------------------------------*/
UINT32 TVE_GetTTXEnable(UCHAR ucTveId, UCHAR* pucTTXEnable)
{
    VERIFY_TVE_ID(ucTveId);
    
    if (pucTTXEnable != NULL)
    {
        *pucTTXEnable = _arTveConf[ucTveId].ucTTXEnable;
    }
    
    return TVE_SET_OK;
}

/*--------------------------------------------
// Send TTX
//--------------------------------------------*/
UINT32 TVE_SendTTX(UCHAR ucTveId,const UCHAR *pu1Buf, UINT32 length)
{
    UINT32 u4RetVal = TVE_SET_OK;
    
    VERIFY_TVE_ID(ucTveId);

    if (pu1Buf != NULL)
    {
        CRIT_STATE_T csState;
        csState = x_crit_start();
        u4RetVal = TVE_HalSend2TTXQue(ucTveId, pu1Buf, length);
        x_crit_end(csState);
    }

    return u4RetVal;
}


/*--------------------------------------------
// Set WSS on/off
//--------------------------------------------*/
UINT32 TVE_SetWssEnable(UCHAR ucTveId, UCHAR ucEnable)
{
    UCHAR ucTveFmt = 0;
    UINT32 u4RetVal = TVE_SET_OK;
    
    VERIFY_TVE_ID(ucTveId);

    _arTveConf[ucTveId].ucWssEnable = ucEnable;
    ucTveFmt = _arTveConf[ucTveId].ucFmt;

    if (ucEnable)
    {
        u4RetVal = TVE_HalEnableWss(ucTveId, ucTveFmt);
    }
    else
    {
        u4RetVal = TVE_HalDisableWss(ucTveId, ucTveFmt);
    }

    return u4RetVal;
}

/*--------------------------------------------
// Get Wss on/off
//--------------------------------------------*/
UINT32 TVE_GetWssEnable(UCHAR ucTveId, UCHAR* pucWssEnable)
{
    VERIFY_TVE_ID(ucTveId);

    if (pucWssEnable != NULL)
    {
        *pucWssEnable = _arTveConf[ucTveId].ucWssEnable;
    }
    
    return TVE_SET_OK;
}

#ifdef CC_TVE_INSERT_VPS//  for vps
/*-------------------------------------------
//Set vps on/off
--------------------------------------------*/
UINT32 TVE_SetVPSEnable(UCHAR ucTveId, UCHAR ucEnable)
{
	UINT32 u4RetVal = TVE_SET_OK;

	VERIFY_TVE_ID(ucTveId);
	_arTveConf[ucTveId].ucVPSEnable = ucEnable;
	u4RetVal = TVE_HalEnableVPS(ucTveId, ucEnable);
	return u4RetVal;
}

UINT32 TVE_GetVPSEnable(UCHAR ucTveId, UCHAR* pucVPSEnable)
{
	VERIFY_TVE_ID(ucTveId);
	if(pucVPSEnable != NULL)
	{
		*pucVPSEnable = _arTveConf[ucTveId].ucVPSEnable;
	}
	return TVE_SET_OK;
}

UINT32 TVE_SendVPS(UCHAR ucTveId, const UCHAR *pucData)
{
    UINT32 u4RetVal = TVE_SET_OK;    
    VERIFY_TVE_ID(ucTveId);
	if(pucData != NULL)
	{
		u4RetVal = TVE_HalSendVPSValue(ucTveId, pucData);
	}
	return u4RetVal;
}

#endif
/*--------------------------------------------
// Set WSS Aspect Ratio
//--------------------------------------------*/
UINT32 TVE_SetAspectRatio(UCHAR ucTveId, UCHAR ucAspectRatio)
{
    UCHAR ucTveFmt = 0;
    UINT32 u4RetVal = TVE_SET_OK;
    
    VERIFY_TVE_ID(ucTveId);

    if (_arTveConf[ucTveId].ucWssEnable)
    {
        ucTveFmt = _arTveConf[ucTveId].ucFmt;
        _arTveConf[ucTveId].ucAspectRatio = ucAspectRatio;
        u4RetVal = TVE_HalSetAspectRatio(ucTveId, ucTveFmt, ucAspectRatio);
    }
    else
    {
        u4RetVal = TVE_SET_ERROR;
    }

    return u4RetVal;
}

/*--------------------------------------------
// Get WSS Aspect Ratio
//--------------------------------------------*/
UINT32 TVE_GetAspectRatio(UCHAR ucTveId, UCHAR* pucAspectRatio)
{
    VERIFY_TVE_ID(ucTveId);

    if (pucAspectRatio != NULL)
    {
        *pucAspectRatio = _arTveConf[ucTveId].ucAspectRatio;
    }
    
    return TVE_SET_OK;
}

/*--------------------------------------------
// Set CGMS-A Copy Control on/off
//--------------------------------------------*/
UINT32 TVE_SetCgmsaValue(UCHAR ucTveId, UCHAR ucValue)
{
    UCHAR ucTveFmt = 0;
    UINT32 u4RetVal = TVE_SET_OK;
    
    VERIFY_TVE_ID(ucTveId);

    _arTveConf[ucTveId].ucCgmsaInfo = ucValue;
    ucTveFmt = _arTveConf[ucTveId].ucFmt;

    u4RetVal = TVE_HalEnableWss(ucTveId, ucTveFmt);
    u4RetVal = TVE_HalSetCgmsaValue(ucTveId, ucTveFmt, ucValue);

    return u4RetVal;
}

/*--------------------------------------------
// Get CGMS-A Copy Control on/off
//--------------------------------------------*/
UINT32 TVE_GetCgmsaValue(UCHAR ucTveId, UCHAR* pucValue)
{
    VERIFY_TVE_ID(ucTveId);

    if (pucValue != NULL)
    {
        *pucValue = _arTveConf[ucTveId].ucCgmsaInfo;
    }
    
    return TVE_SET_OK;
}

/*--------------------------------------------
// Send WSS
//--------------------------------------------*/
UINT32 TVE_SendWss(UCHAR ucTveId,const UCHAR *pu1Buf, UINT32 length)
{
    UCHAR ucTveFmt = 0;
    
    ucTveFmt = _arTveConf[TVE_1].ucFmt;

    TVE_HalSetWSSValue(TVE_1, ucTveFmt, pu1Buf);  

    return TVE_SET_OK;
}

/*--------------------------------------------
// Enable/disable DAC
//--------------------------------------------*/
UINT32 TVE_DACPower(UCHAR ucTveId, UCHAR ucDAC, UCHAR ucEnable)
{
    VERIFY_TVE_ID(ucTveId);
    VERIFY_DAC_ID(ucDAC);

    if(ucDAC == DAC_1)
    {
        _arTveConf[ucTveId].ucDAC1Enable = ucEnable;
    }
    else
    {
        _arTveConf[ucTveId].ucDAC2Enable = ucEnable;
    }

#ifdef TVE_CS4954
    if (ucTveId == TVE_2)
    {
        TVE_CS4954_Enable(ucEnable);
        return TVE_SET_OK;    
    }    
#endif

    TVE_HalDACPowerOn(ucTveId, ucDAC, ucEnable);
    
    return TVE_SET_OK;
}
#if defined(CC_MT5363) || defined(CC_TVE6595)
/*--------------------------------------------
// Enable/disable PIP Mixer
//--------------------------------------------*/
UINT32 TVE_PIPMixer(UCHAR ucTveId, UCHAR ucEnable)
{
    VERIFY_TVE_ID(ucTveId);

    TVE_HalPIPMixerOn(ucTveId, ucEnable);
    
    return TVE_SET_OK;
}
#endif

/*--------------------------------------------
// Select TVE input source
//--------------------------------------------*/
UINT32 TVE_SelectInput(UCHAR ucTveSrc)
{
//    VERIFY_TVE_ID(ucTveId);

//    _arTveConf[ucTveId].ucDACEnable = ucEnable;

    TVE_HalSrcSelect(ucTveSrc);
    
    return TVE_SET_OK;
}

#ifdef TVE_WA
/**
* @brief Check if PSCAN Y bob mode
* @param void
* @param  
*/
UINT32 TVE_ChkYBobMode(void)
{
#if ((!defined(CC_MT5368))&&(!defined(CC_MT5396))&&(!defined(CC_MT5389))&&(!defined(CC_MT5398))&&(!defined(CC_MT5880))&&(!defined(CC_MT5881))&&(!defined(CC_MT5399))&&(!defined(CC_MT5890))&&(!defined(CC_MT5882)))
    switch (_arMDDiPrm[VDP_2].u1DataMode)
    {
        case E_DI_YCBOB_0W0R:
        case E_DI_YCBOB_1W1R:
        case E_DI_FLIP_MIRROR_YCBOB_1W1R:
            return TVE_SET_OK;
        default:
            return TVE_SET_ERROR;
    }
#else
    return IS_DI_BOB(VDP_2) ? TVE_SET_OK : TVE_SET_ERROR;
#endif
}

static UINT8 TVE_SCPIP_W_POS(void)
{
#if (defined(CC_MT5398) || defined(CC_MT5880) || defined(CC_MT5860) || defined(CC_MT5881) || defined(CC_MT5399)|| defined(CC_MT5890)|| defined(CC_MT5882))
    return u4ScpipGetDramWStatus(SV_VP_PIP);
#elif (defined(CC_MT5368) || defined(CC_MT5396) || defined(CC_MT5389))
    return RegReadFldAlign(SCPIP_PIP_OA_RO_0A,PIP_OA_RO_0A_RD_W_POS_2);
#elif defined(CC_MT5363)
	return RegReadFldAlign(SCPIP_PIP2_10, PIP2_10_W_POS_2);
#else
	return RegReadFldAlign(SCPIP_PIP2_RO_00, PIP2_RO_00_W_POS_2);
#endif
}

static UINT8 TVE_SCPIP_R_POS(void)
{
#if (defined(CC_MT5398) || defined(CC_MT5880) || defined(CC_MT5860) || defined(CC_MT5881) || defined(CC_MT5399)|| defined(CC_MT5890)|| defined(CC_MT5882))
    return u4ScpipGetDramRStatus(SV_VP_PIP);
#elif (defined(CC_MT5368) || defined(CC_MT5396) || defined(CC_MT5389))
    return RegReadFldAlign(SCPIP_PIP_OA_RO_0A,PIP_OA_RO_0A_RD_R_POS_2);
#elif defined(CC_MT5363)
    return RegReadFldAlign(SCPIP_PIP2_10, PIP2_10_R_POS_2);
#else
	return RegReadFldAlign(SCPIP_PIP2_RO_00, PIP2_RO_00_R_POS_2);
#endif
}


static UINT16 TVE_SCPIP_WSTARTP_OFST_2(void)
{
    #if (defined(CC_MT5398) || defined(CC_MT5880) || defined(CC_MT5860) || defined(CC_MT5881) || defined(CC_MT5399)|| defined(CC_MT5890)|| defined(CC_MT5882))
    return 0;
    #else
	return RegReadFldAlign(SCPIP_PIP2_04, PIP2_04_WSTARTP_OFST_2);
    #endif
}

static UINT8 TVE_DI_CF_BOTTOM(void)
{
#ifdef CC_MT5363
	return MDDI_READ_FLD(SV_VP_PIP, MDDI_STA_00, CF_BOTTOM);
	
#elif (defined(CC_MT5368) || defined(CC_MT5396) || defined(CC_MT5389) || defined(CC_MT5398) || defined(CC_MT5880) || defined(CC_MT5881) || defined(CC_MT5399)|| defined(CC_MT5890)|| defined(CC_MT5882))
	return MDDI_READ_FLD(SV_VP_PIP, MCVP_KC_19, CF_BOTTOM);
	
#else
	return MDDI_READ_FLD(SV_VP_PIP, MDDI_KC_04, CF_BOTTOM);
#endif
}

UINT8 TVE_Field_Info[4], bTVECounter = 0, TVE_FirstReset = 20;
UINT8 bTVEFieldDeglitch = 0;
UINT8 bTVEFieldInvert = 0;
UINT16 u2TVEWStartP = 0;
UINT8 TVE_FirstReset2 = 10,bTVECounter2=0;
UINT8 bPrevTVEField = 0;

static void TVE_SCPIP_W_STARTP(void)
{
	u2TVEWStartP = u2TVEWStartP & 0x01 ? u2TVEWStartP-1 : u2TVEWStartP+1;
#if (defined(CC_MT5398) || defined(CC_MT5880) || defined(CC_MT5860) || defined(CC_MT5881) || defined(CC_MT5399)|| defined(CC_MT5890)|| defined(CC_MT5882))
//todo
#elif defined(CC_MT5363)
	vRegWriteFldAlign(SCPIP_PIP2_04, u2TVEWStartP, PIP2_04_WSTARTP_OFST_2);
	vRegWriteFldAlign(SCPIP_PIP2_05, (u2TVEWStartP >> 8), PIP2_05_WSTARTP_OF1ST_2); 
	vRegWriteFldAlign(SCPIP_PIP2_06, (u2TVEWStartP & 0xff), PIP2_06_WSTARTP_OF1ST_2); 
#else
	vRegWriteFldAlign(SCPIP_PIP2_04, u2TVEWStartP, PIP2_04_WSTARTP_OFST_2);
#endif
#if defined(CC_MT5396) || defined(CC_MT5368) || defined(CC_MT5389)
    vRegWriteFldAlign(SCPIP_SYSTEM_12, 1, SYSTEM_12_IMPORT_OFF_2);
    vRegWriteFldAlign(SCPIP_SYSTEM_12, 0, SYSTEM_12_IMPORT_OFF_2);
#endif

}

/*
UINT32 TVE_OnInputVSync(void)
{
    UINT8 bFlag, bFlag1, bFrame,u1VdoIsInterlace,bSrc;
    UINT8 ucEnable;
    UINT16 u2VdoInputHeight;
    
    TVE_HalSetInputSrc(&bSrc);
    if(!fgIsVdoIntPipDet()) return TVE_SET_ERROR;
    
    if(bDrvVideoSignalStatus(SV_VP_PIP) != SV_VDO_STABLE)
    {
        TVE_FirstReset = 20;
        return TVE_SET_ERROR;
    }

    if(!TVE_ChkYBobMode()) return TVE_SET_ERROR;//if Y is not bob, not need WA

    TVE_GetEnable(TVE_1, &ucEnable);
    if (ucEnable == 0) return TVE_SET_ERROR;

    u1VdoIsInterlace = bDrvVideoIsSrcInterlace(SV_VP_PIP);
    u2VdoInputHeight = wDrvVideoInputHeight(SV_VP_PIP);
    if (u1VdoIsInterlace && ((u2VdoInputHeight == 576) || (u2VdoInputHeight == 480)))
    {
        bFrame = TVE_SCPIP_W_POS();
        LOG(9, "Scaler_W=%d\n", bFrame);
        if(bFrame > 3) // Fix Klocwork warning 
        {
            LOG(9, "Scaler w frame error\n");
            return TVE_SET_ERROR;
        }
        bFlag = TVE_DI_CF_BOTTOM();
        LOG(9, "DI=%d\n", bFlag);
        //TVE_Field_Info[bFrame] = (bSrc == SV_VD_YPBPR) ? !bFlag : bFlag;
        TVE_Field_Info[bFrame] = bFlag;
        
        bFlag1 = TVE_GetFieldInfo(TVE_1);
        LOG(9, "TVE=%d\n", bFlag1);
        bFrame = TVE_SCPIP_R_POS();
        LOG(9, "Scaler_R=%d\n", bFrame);
        if(bFrame > 3) // Fix Klocwork warning 
        {
            LOG(9, "Scaler r frame error\n");
            return TVE_SET_ERROR;
        }
        
        if(TVE_FirstReset)
        {
            if((bFlag1 != TVE_Field_Info[bFrame]))
	     {
	         bTVECounter++;
	         if(bTVECounter > 5)
	         {
                    TVE_HalResetSyncEn(TVE_1);//reset TVE sync
                    bTVECounter = 0;
                    LOG(6, "reset TVE sync\n");
                }   
	     }
	     else
	     {
                bTVECounter = 0;
                TVE_FirstReset = (TVE_FirstReset == 0) ? 0 : TVE_FirstReset-1;   
	     }
	     if(TVE_FirstReset == 0)
	     {
	         bTVEFieldInvert = 0;
		  u2TVEWStartP = TVE_SCPIP_WSTARTP_OFST_2();
		  if((u2TVEWStartP&0x01) == 0x01)
		  {
			TVE_SCPIP_W_STARTP();
		  }
		  LOG(6, "reset u2TVEWStartP=%d\n", u2TVEWStartP);
                #if defined(CC_FLIP_MIRROR_SUPPORT)
                if(u4GetFlipMirrorModule(VDP_2)==FLIP_BY_SCALER)
                {
                    u2TVEWStartP = TVE_SCPIP_WSTARTP_OFST_2();
                    TVE_SCPIP_W_STARTP();    
                }
                #endif
            }
	 }
        #if 1
	 else
	 {
            if((u2TVEWStartP&0x01) != (TVE_SCPIP_WSTARTP_OFST_2()&0x01))
            {
              u2TVEWStartP = TVE_SCPIP_WSTARTP_OFST_2();
		TVE_SCPIP_W_STARTP();
              LOG(6, "check even/odd u2TVEWStartP=%d\n", u2TVEWStartP);
              bTVEFieldDeglitch=0;
            }
            if(bTVEFieldInvert == 0)
            {
                if(bFlag1 != TVE_Field_Info[bFrame])
                {
                    if((bFlag1 != TVE_Field_Info[bFrame])&&(bTVEFieldDeglitch==1))
                    {
                        bTVEFieldInvert = 1;
                        u2TVEWStartP = TVE_SCPIP_WSTARTP_OFST_2();
                        TVE_SCPIP_W_STARTP();
                        LOG(6, "TVE detect field invert\n");
                        LOG(6, "bTVEFieldInvert1 u2TVEWStartP=%d\n", u2TVEWStartP);
                        bTVEFieldDeglitch=0;
                    }
                    else
                    {
                    	   bTVEFieldDeglitch=1;
                    }
                }
                else
                {
                    bTVEFieldDeglitch=0;
                }
            }
            else
            {
                if(bFlag1 == TVE_Field_Info[bFrame])
                {
                    if((bFlag1 == TVE_Field_Info[bFrame])&&(bTVEFieldDeglitch==1))
                    {
                        bTVEFieldInvert = 0;
                        u2TVEWStartP = TVE_SCPIP_WSTARTP_OFST_2();
                        TVE_SCPIP_W_STARTP();
                        LOG(6, "TVE detect field invert2\n");
                        LOG(6, "bTVEFieldInvert2 u2TVEWStartP=%d\n", u2TVEWStartP);
                        bTVEFieldDeglitch=0;
                    }
                    else
                    {
                        bTVEFieldDeglitch=1;
                    }
                }
                else
                {
                    bTVEFieldDeglitch=0;
                }
            }
        }
        #endif
    }
    return TVE_SET_OK;
}
*/

UINT32 TVE_OnInputVSync(void)
{
    UINT8 bFlag, /*bFlag1,*/ bFrame,u1VdoIsInterlace,bSrc;
    UINT8 ucEnable;
    UINT16 u2VdoInputHeight;
    CRIT_STATE_T csState;
    
    TVE_HalSetInputSrc(&bSrc);
    if(!fgIsVdoIntPipDet()) return TVE_SET_ERROR;
    
    if(bDrvVideoSignalStatus(SV_VP_PIP) != SV_VDO_STABLE)
    {
        TVE_FirstReset = 20;
        return TVE_SET_ERROR;
    }

    //if(!TVE_ChkYBobMode()) return TVE_SET_ERROR;//if Y is not bob, not need WA

    TVE_GetEnable(TVE_1, &ucEnable);
    if (ucEnable == 0) return TVE_SET_ERROR;

    u1VdoIsInterlace = bDrvVideoIsSrcInterlace(SV_VP_PIP);
    u2VdoInputHeight = wDrvVideoInputHeight(SV_VP_PIP);
    if (u1VdoIsInterlace && ((u2VdoInputHeight == 576) || (u2VdoInputHeight == 480)))
    {
        bFrame = TVE_SCPIP_W_POS();
        LOG(9, "Scaler_W=%d\n", bFrame);
        if(bFrame > 3) // Fix Klocwork warning 
        {
            LOG(9, "Scaler w frame error\n");
            return TVE_SET_ERROR;
        }
        bFlag = TVE_DI_CF_BOTTOM();
        LOG(9, "DI=%d\n", bFlag);
        //TVE_Field_Info[bFrame] = (bSrc == SV_VD_YPBPR) ? !bFlag : bFlag;
        csState = x_crit_start();
        TVE_Field_Info[bFrame] = bFlag;        
        x_crit_end(csState);
        
    }
    return TVE_SET_OK;
}

UINT32 TVE_OnOutputVSync(void)
{
    UINT8 bFlagDI, bFlag1, bFrame,u1VdoIsInterlace,bSrc;
    UINT8 ucEnable;
    UINT16 u2VdoInputHeight;
    CRIT_STATE_T csState;
    
    TVE_HalSetInputSrc(&bSrc);
    //if(!fgIsVdoIntPipDet()) return TVE_SET_ERROR;
    
    if(bDrvVideoSignalStatus(SV_VP_PIP) != SV_VDO_STABLE)
    {
        TVE_FirstReset = 20;
        return TVE_SET_ERROR;
    }

    //if(!TVE_ChkYBobMode()) return TVE_SET_ERROR;//if Y is not bob, not need WA

    TVE_GetEnable(TVE_1, &ucEnable);
    if (ucEnable == 0) return TVE_SET_ERROR;

    u1VdoIsInterlace = bDrvVideoIsSrcInterlace(SV_VP_PIP);
    u2VdoInputHeight = wDrvVideoInputHeight(SV_VP_PIP);
    if (u1VdoIsInterlace && ((u2VdoInputHeight == 576) || (u2VdoInputHeight == 480)))
    {
        if(TVE_FirstReset)
        {
            bFlag1 = TVE_GetFieldInfo(TVE_1);
            bPrevTVEField = bFlag1;
        }
        else
        {
            bFlag1 = !bPrevTVEField;
            bPrevTVEField = bFlag1;
        }    
        LOG(9, "TVE=%d\n", bFlag1);
        bFrame = TVE_SCPIP_R_POS();
        LOG(9, "Scaler_R=%d\n", bFrame);
        if(bFrame > 3) // Fix Klocwork warning 
        {
            LOG(9, "Scaler r frame error\n");
            return TVE_SET_ERROR;
        }
        csState = x_crit_start();
        bFlagDI = TVE_Field_Info[bFrame];
        x_crit_end(csState);
        
        if(TVE_FirstReset)
        {
            if((bFlag1 != bFlagDI))
	     {
	         bTVECounter++;
	         if(bTVECounter > 5)
	         {
                    TVE_HalResetSyncEn(TVE_1);//reset TVE sync
                    bTVECounter = 0;
                    LOG(2, "reset TVE sync\n");
                }   
	     }
	     else
	     {
                bTVECounter = 0;
                TVE_FirstReset = (TVE_FirstReset == 0) ? 0 : TVE_FirstReset-1;   
	     }
	     if(TVE_FirstReset == 0)
	     {
	         bTVECounter2=0;
	         TVE_FirstReset2 = 20;
	         bTVEFieldInvert = 0;
		  u2TVEWStartP = TVE_SCPIP_WSTARTP_OFST_2();
		  if((u2TVEWStartP&0x01) == 0x01)
		  {
			TVE_SCPIP_W_STARTP();
		  }
		  LOG(2, "reset u2TVEWStartP=%d\n", u2TVEWStartP);
                #if defined(CC_FLIP_MIRROR_SUPPORT)
                if(u4GetFlipMirrorModule(VDP_2)==FLIP_BY_SCALER)
                {
                    u2TVEWStartP = TVE_SCPIP_WSTARTP_OFST_2();
                    TVE_SCPIP_W_STARTP();    
                }
                #endif
            }
	 }
    #if 1
	 else
	 {
            if(TVE_FirstReset2)
            {
                #if defined(CC_MT5396) || defined(CC_MT5389) || defined(CC_MT5368)
                if(TVE_HalGetFieldInvStatus()==1)
                #else
                if(TVE_HalGetFieldInvStatus()==0)
                #endif
                {	        
                    bTVECounter2++;
                    if(bTVECounter2 > 5)
                    {
                        
                        u2TVEWStartP = TVE_SCPIP_WSTARTP_OFST_2();
                        #ifdef CC_TVE6595
                        TVE_SCPIP_W_STARTP();
                        #endif
                        LOG(2, "TVE detect field invert (Method2)\n");
                        LOG(6, "bTVEFieldInvert1 u2TVEWStartP=%d\n", u2TVEWStartP);
                        bTVECounter2 = TVE_FirstReset2 = 0;
                        //bTVECounter2 = 0xFF;
                    }   
                }
                else
                {
                    bTVECounter2 = 0;
                }
                TVE_FirstReset2 = (TVE_FirstReset2 == 0) ? 0 : TVE_FirstReset2-1;
            }
	 
            if((u2TVEWStartP&0x01) != (TVE_SCPIP_WSTARTP_OFST_2()&0x01))
            {
              u2TVEWStartP = TVE_SCPIP_WSTARTP_OFST_2();
		TVE_SCPIP_W_STARTP();
              LOG(6, "check even/odd u2TVEWStartP=%d\n", u2TVEWStartP);
              bTVEFieldDeglitch=0;
            }
            if(bTVEFieldInvert == 0)
            {
                if(bFlag1 != bFlagDI)
                {
                    if((bFlag1 != bFlagDI)&&(bTVEFieldDeglitch==3))
                    {
                        bTVEFieldInvert = 1;
                        u2TVEWStartP = TVE_SCPIP_WSTARTP_OFST_2();
                        TVE_SCPIP_W_STARTP();
                        LOG(2, "TVE detect field invert\n");
                        LOG(6, "bTVEFieldInvert1 u2TVEWStartP=%d\n", u2TVEWStartP);
                        bTVEFieldDeglitch=0;
                    }
                    else
                    {
                    	   bTVEFieldDeglitch++;
                    }
                }
                else
                {
                    bTVEFieldDeglitch=0;
                }
            }
            else
            {
                if(bFlag1 == bFlagDI)
                {
                    if((bFlag1 == bFlagDI)&&(bTVEFieldDeglitch==3))
                    {
                        bTVEFieldInvert = 0;
                        u2TVEWStartP = TVE_SCPIP_WSTARTP_OFST_2();
                        TVE_SCPIP_W_STARTP();
                        LOG(2, "TVE detect field invert2\n");
                        LOG(6, "bTVEFieldInvert2 u2TVEWStartP=%d\n", u2TVEWStartP);
                        bTVEFieldDeglitch=0;
                    }
                    else
                    {
                        bTVEFieldDeglitch++;
                    }
                }
                else
                {
                    bTVEFieldDeglitch=0;
                }
            }
        }
    #endif
    }
    return TVE_SET_OK;
}

#endif
/*--------------------------------------------
// Get TVE Main & Aux current status
//--------------------------------------------*/
static void _TveIfStatus(UCHAR ucTveId)
{
    //LOG(5, "\nTVE(%d) E(%d) F(%d) M(%d) CC(%d) MV(%d) DAC1(%d) DAC2(%d)",
    LOG(5, "\nTVE(%d) E(%d) F(%d) M(%d) CC(%d) DAC1(%d) DAC2(%d) Asr(%d)",
        ucTveId,
        _arTveConf[ucTveId].ucTveEnable,
        _arTveConf[ucTveId].ucFmt,
        _arTveConf[ucTveId].ucMode,
        _arTveConf[ucTveId].ucCcEnable,
        //_arTveConf[ucTveId].ucMvEnable,
        _arTveConf[ucTveId].ucDAC1Enable,
        _arTveConf[ucTveId].ucDAC2Enable,
        _arTveConf[ucTveId].ucAspectRatio);
    return;
}

UINT32 TVE_GetFieldInfo(UCHAR ucTveId)
{
    if(TVE_HalGetFieldInfo(ucTveId))
    {
    	return TVE_FIELD_EVEN;
    }
    else
    {
    	return TVE_FIELD_ODD;
    }
}

static UINT8 TVE_ML_Counter = 0;
void  _TVE_Mainloop(void)
{
    VDEC_HDR_INFO_T rHdrInfo;
    UINT8 idx, ucAspectRatio,ucWssEnable;

    TVE_ML_Counter ++;
    if(TVE_ML_Counter < 20)
    {
        return;
    }
    else
    {
        TVE_ML_Counter = 0;
    }

    if(DRVCUST_OptGet(eTVESupportAFD) == 0) return;

    TVE_GetWssEnable(TVE_1, &ucWssEnable);   
    if(ucWssEnable == 0) return;

#if 1 
    VDEC_QueryInfo(0, &rHdrInfo);
#endif
    if(rHdrInfo.fgHdrInvalid || (rHdrInfo.ucAfd > 15))
    {
        return;
    }
    else
    {
        if(rHdrInfo.eAspectRatio == VDEC_ASPECT_RATIO_4_3)
        {
            idx = 0;
        }
        else
        {
            idx = 1;
        }  
        TVE_GetAspectRatio(TVE_1, &ucAspectRatio);
        if(ucAspectRatio != _uc625LAsrTable2Wss[idx][rHdrInfo.ucAfd])
        {
            TVE_SetAspectRatio(TVE_1, _uc625LAsrTable2Wss[idx][rHdrInfo.ucAfd]);
        }
    }    
}

void Bypass_Init(void)
{
    TVE_Init();
#if (defined(CC_MT5890))
    TVE_HalSetCurrent(DAC_1, DAC_FULL_CURRENT);
	vIO32WriteFldAlign(REG_VDAC_CFG0, 0, RG_VDAC_REV);  //Before calibration need to make VADC=1.57V default
#endif
#if (defined(CC_MT5881)||defined(CC_MT5399)|| defined(CC_MT5890)|| defined(CC_MT5882))
#if CC_TVE_SUPPORT_VDAC_SOFTWARE_CAL
	TVE_HalVdacCalInit();
#else
    TVE_HalLoadDftTrimVal();
#endif
#endif
#if defined(CC_MT5881)
	vIO32WriteFldAlign(PDWNC_VGACFG0, 0, FLD_RG_VDACIB_PWD);
	vIO32WriteFldAlign(REG_VDAC_CFG0, 0, RG_BGREF_PWD);
#endif
    Bypass_HalClear2Default();
    Bypass_HalSetCommon();
    TVE_SetMode(TVE_1, TVE_MOD_COMPOSITE);
#if (!defined(CC_MT5881))
    TVE_SetFmt(TVE_1, TVE_FMT_576I);
#endif
}

#ifdef CC_SUPPORT_STR
void TVE_pm_suspend(void)
{
    LOG(3, "TVE suspend\n");
	TVE_HalSuspendSaveAddr();
	TVE_HalTVEClkPowerOn(0);
	TVE_HalVDACClkPowerOn(0);
}

void TVE_pm_resume(void)
{
    LOG(3,"TVE_Resume\n");
    Bypass_HalSetCommon();
	TVE_HalTVEClkPowerOn(1);
	TVE_HalVDACClkPowerOn(1);
	TVE_HalResumeSaveAddr();
}
#endif

#if defined(CC_SUPPORT_TVE) && defined(CC_SUPPORT_TVE_VIDEO_CLIP_BY_MTAL)
/*--------------------------------------------
// Set H Display Area, and center the image
//--------------------------------------------*/
UINT32 TVE_SetHDispArea(UCHAR ucFmt, UCHAR bSrc, UINT16 u2BgnValue, UINT16 u2EndValue)
{
    UINT32 u4RetVal = TVE_SET_OK;
    UINT16 u2Vsync, u2Hsync, u2NewHsync;

    if(u2EndValue < u2BgnValue)
        return TVE_SET_ERROR;
    
    //change the H-sync to center the image
    TVE_HalGetSyncTime1(TVE_1, ucFmt, bSrc, &u2Vsync, &u2Hsync);
    u2NewHsync = u2Hsync - u2BgnValue;
    if(u2NewHsync < ((ucFmt == TVE_FMT_480I) ? 429 : 434))
    {
        TVE_HalSetSyncTime1(TVE_1, u2Vsync - 1, u2NewHsync); 
    }
    else
    {
        TVE_HalSetSyncTime1(TVE_1, u2Vsync, u2NewHsync); 
    }
    //set DispArea to clip grabage
    if(ucFmt == TVE_FMT_480I)
    {
        u4RetVal = TVE_HalSetDispArea(TVE_1, SV_ON, 0x6b + u2BgnValue, 0x6b + u2EndValue);//0x6B -> 0x33B
    }
    else
    {
        u4RetVal = TVE_HalSetDispArea(TVE_1, SV_ON, 0x77 + u2BgnValue, 0x77 + u2EndValue);//0x77->0x347
    }
    return u4RetVal;
}

/*--------------------------------------------
// Set V Display Area 
//--------------------------------------------*/
UINT32 TVE_SetVDispArea(UCHAR ucFmt, UINT16 u2BgnValue, UINT16 u2EndValue)
{
    UINT32 u4RetVal = TVE_SET_OK;

    if(u2EndValue < u2BgnValue)
        return TVE_SET_ERROR;
#if (defined(CC_MT5396) || defined(CC_MT5368) || defined(CC_MT5389) || defined(CC_MT5398) || defined(CC_MT5880) || defined(CC_MT5881) || defined(CC_MT5399)|| defined(CC_MT5890)|| defined(CC_MT5882))
    if(ucFmt == TVE_FMT_480I)
    {
        u4RetVal = TVE_HalSetSyncTime2(TVE_1, SV_ON, 0x59 + u2BgnValue * 2,  0x58 + u2EndValue * 2);//0x50~0x410
    }
    else
    {
        u4RetVal = TVE_HalSetSyncTime2(TVE_1, SV_ON, 0x59 + u2BgnValue * 2,  0x58 + u2EndValue * 2);//0x51~0x4D0
    }
#else
    if(ucFmt == TVE_FMT_480I)
    {
        u4RetVal = TVE_HalSetSyncTime2(TVE_1, SV_ON, 0x51 + u2BgnValue * 2,  0x50 + u2EndValue * 2);//0x50~0x410
    }
    else
    {
        u4RetVal = TVE_HalSetSyncTime2(TVE_1, SV_ON, 0x51 + u2BgnValue * 2,  0x50 + u2EndValue * 2);//0x51~0x4D0
    }
#endif
    return u4RetVal;
}
#endif

#if (defined(CC_MT5881)||defined(CC_MT5399)|| defined(CC_MT5890)|| defined(CC_MT5882))
#if CC_TVE_SUPPORT_VDAC_SOFTWARE_CAL
UINT32 TveSetAbistBypass(UINT8 bPattern)
{
    if(bPattern > 3)
    {
		return TVE_SET_ERROR;
    }
	TVE_HalSetAbistBypss(bPattern);
	
	return TVE_SET_OK;
	
}

#if (defined(CC_MT5890))
UINT32 TveSetAbistDcWaveFullCmd(void)
{
    #if (defined(CC_MT5890))
    TVE_HalSetAbistBypssFullZero(1,0xfff);
	#endif
	return TVE_SET_OK;
}

UINT32 TveSetAbistDcWaveZeroCmd(void)
{
    #if (defined(CC_MT5890))
    TVE_HalSetAbistBypssFullZero(1,0x0);
	#endif
	return TVE_SET_OK;
}

UINT32 TveReadAdcValueCmd(void)
{
   UINT16 _adcvalue=0;
   //UINT32 REG_CON_DCMETERSEL_CFG011=0;
   #if (defined(CC_MT5890))
   vIO32WriteFldAlign(REG_CON_DCMETERSEL_CFG0, 0x3, RG_DCMMETER_DATA);
   //REG_CON_DCMETERSEL_CFG011=IO32ReadFldAlign(REG_CON_DCMETERSEL_CFG0, RG_DCMMETER_DATA);
   //Printf("REG_CON_DCMETERSEL_CFG011:REG_CON_DCMETERSEL_CFG011=%d.\n",REG_CON_DCMETERSEL_CFG011);
   #endif
   vIO32WriteFldAlign(REG_VDAC_CFG0, 0x1, RG_VDAC_CAL_EN); 
   vIO32WriteFldAlign(REG_VDAC_CFG0, 0x1, RG_VDAC_ABIST_EN);
   TVE_HalEnServoAD(1);
   #if (defined(CC_MT5890))
   _adcvalue= IO32ReadFldAlign(PDWNC_ADOUTA, FLD_ADOUTA); 
    vUtDelay2us(200);
   _adcvalue= IO32ReadFldAlign(PDWNC_ADOUTA, FLD_ADOUTA); 
   #else  //remy and MT5399
   _adcvalue= IO32ReadFldAlign(PDWNC_ADOUT8, FLD_ADOUT8); 
    vUtDelay2us(200);
   _adcvalue= IO32ReadFldAlign(PDWNC_ADOUT8, FLD_ADOUT8); 
   #endif
   Printf("_adcvalue:_adcvalue=%d.\n",_adcvalue);
   return TVE_SET_OK;
}

#endif

UINT32 TveVdacStartSoftCalibration(void)
{
    _bTveSoftCalState = VDO_TVE_CAL_START;
	if(TVE_SET_OK == TVE_HalDoSoftCal())
	{
		TVE_HalLoadSoftCalVal();  //load trim value
		Printf("CLI set Vdac software calibration success\n");
		TVE_HalSetCurrent(DAC_1, DAC_FULL_CURRENT);
		TVE_HalLoadCLICalVal();
        return TVE_SET_OK;
	}
	else
	{
		TVE_HalLoadDftTrimVal();
		Printf("CLI set Vdac software calibration fail\n");
		TVE_HalSetCurrent(DAC_1, DAC_FULL_CURRENT);
		return TVE_SET_ERROR;
	}	
}

#endif
#endif

/*--------------------------------------------
// Enable/disable TVE bypass monitor out
//--------------------------------------------*/
BOOL TVE_SetEnableBypassMonOut(UCHAR ucTveId, BOOL ucEnable)
{
	VERIFY_TVE_ID(ucTveId);

	_bBypassFromMonOut = ucEnable;
	
	return TRUE;
}

/*--------------------------------------------
// Get TVE enable/disable bypass monitor out
//--------------------------------------------*/
BOOL TVE_GetEnableBypassMonOut(UCHAR ucTveId)
{
	VERIFY_TVE_ID(ucTveId);

	return _bBypassFromMonOut;	  
}

#if 0//def CC_SUPPORT_TVE
const UINT8 TVESrcInternalMap[]=
{SV_VD_TVD3D, 
SV_VD_TVD3D, 
SV_VD_TVD3D, 
SV_VD_TVD3D, 
SV_VD_YPBPR, 
SV_VD_YPBPR, 
SV_VD_VGA,   
SV_VD_DVI,   
SV_VD_DVI,   
SV_VD_MPEGHD,
SV_VD_MPEGHD,
SV_VD_TVD3D
};

UINT32 AuxOutConnect(UCHAR ucDACid, UCHAR ucDACSrc)
{
    UINT8 bDec=0, ucFmt;
    
    VERIFY_DAC_ID(ucDACid);
    
    LOG(3, "vAuxOutConnect: DAC%x, Source%x\n", ucDACid, ucDACSrc);
    
    if (VSS_MAJOR(ucDACSrc)<VSS_RESERVE)
    {
        bDec=TVESrcInternalMap[VSS_MAJOR(ucDACSrc)];
    }
    
    if(bDec == SV_VD_TVD3D)
    {
        Bypass_HalSrcBypass(ucDACid, bDec);
    }
    else
    {
        Bypass_HalSrcPIP(ucDACid,bDec);
        TVE_GetFmt(TVE_1, &ucFmt);
        if (ucFmt == TVE_FMT_480I)
        {
            _SCPIP_SetTVEEnable(VDP_2, TRUE, 720, 480);	         	    
        }
        else
        {
            _SCPIP_SetTVEEnable(VDP_2, TRUE, 720, 576);	         	    
        }

         VDP_SendSCPOSOutsizeSRMEvent(VDP_2);   
    }

    return TVE_SET_OK;
}

UINT32 AuxOutDisconnect(UCHAR ucDACid, UCHAR ucDACSrc)
{
    VERIFY_DAC_ID(ucDACid);

    LOG(3, "vAuxOutDisconnect: DAC%x, Source%x\n", ucDACid, ucDACSrc);

    //TODO
    //Bypass_Init();
    _SCPIP_SetTVEEnable(VDP_2, FALSE, 720, 576);	         	    

    return TVE_SET_OK;
}

UINT32 AuxOut_VideoStart(UCHAR ucDACid)
{
    VERIFY_DAC_ID(ucDACid);
   
    LOG(3, "vAuxOut_VideoStart:DAC%x\n", ucDACid);

    TVE_DACPower(TVE_1, ucDACid, 1);
    
    return TVE_SET_OK;
}

UINT32 AuxOut_VideoStop(UCHAR ucDACid)
{
    VERIFY_DAC_ID(ucDACid);

    LOG(3, "vAuxOut_VideoStop: DAC%x\n", ucDACid);

    TVE_DACPower(TVE_1, ucDACid, 0);  
    
    return TVE_SET_OK;
}

UINT32 AuxOut_VideoMute(UCHAR ucDACid, UCHAR ucOn)
{
    VERIFY_DAC_ID(ucDACid);

    LOG(3, "vAuxOut_VideoMute: DAC%x\n", ucDACid);

    if(ucOn)
    {
        AuxOut_VideoStop(ucDACid);
    }
    else
    {
        AuxOut_VideoStart(ucDACid);
    }
    
    return TVE_SET_OK;
}

UINT32 AuxOut_VideoFmt(UCHAR ucDACid, UCHAR ucFmt)
{
    VERIFY_DAC_ID(ucDACid);

    LOG(3, "vAuxOut_VideoFmt: DAC%x Fmt = %x\n", ucDACid, ucFmt);

    _arTveConf[TVE_1].ucMode = TVE_MOD_COMPOSITE;
    TVE_SetFmt(TVE_1, ucFmt); //only support Compisite mode

    return TVE_SET_OK;
}


const UINT8 TVEVbiInternalMap[]=
{
	TVE_VBI_DATA_CC,
	TVE_VBI_DATA_XDS, ///< Extended Data Service
	TVE_VBI_DATA_TTX, ///< Analog SdUserData is Teletext
	TVE_VBI_DATA_WSS, ///< Analog SdUserData is WSS	
	TVE_VBI_DATA_VPS, ///< Analog SdUserData is VPS - included in NTSC line 21 XDS
	TVE_VBI_DATA_PDC, ///< Analog SdUserData is PDC
	TVE_VBI_DATA_MAX
};

UINT32 AuxOut_VbiStart(UCHAR ucDACid, UCHAR eType, UCHAR ucOn)
{
    UINT8 bVbi_type;
    UINT32 u4RetVal = TVE_SET_OK;

    VERIFY_DAC_ID(ucDACid);

    LOG(3, "vAuxOut_VbiEnable: DAC%x\n", ucDACid);

    bVbi_type = TVEVbiInternalMap[eType];
    switch(bVbi_type)
    {
         case TVE_VBI_DATA_CC:
         case TVE_VBI_DATA_XDS:
             u4RetVal = TVE_SetCcEnable(TVE_1, (UCHAR)ucOn);
         break;
         case TVE_VBI_DATA_TTX:
             u4RetVal = TVE_SetTTXEnable(TVE_1, (UCHAR)ucOn);
         break;
         case TVE_VBI_DATA_WSS:
             TVE_SetWssEnable(TVE_1, (UCHAR)ucOn);
         break;
         default:
             u4RetVal = TVE_SET_ERROR;
         break;
    }
    
    return u4RetVal;
}

UINT32 AuxOut_SendCCData(UCHAR eType, UCHAR *pu1Buf, UINT32 length)
{
    INT32 i, idx;
    UINT32 u4ValidPackets, u4TotalPackets;
    UINT32 u4RetVal = TVE_SET_OK;
    //UCHAR *pucTag = NULL;
    //MPV_CC_DATA rCcData;
    //UCHAR ucCcData[NTSC_CC_BUF_SIZE];
    UCHAR ucAtscCc[2];
    //UCHAR ucCcType;
    PTS_T u8Pts;
    UCHAR* ucCcData;

    ucCcData = pu1Buf;

        u4ValidPackets = 0;
        u4TotalPackets = length / NTSC_CC_PACKET_SIZE;
        for (i = 0; i < u4TotalPackets; i++)
        {
            idx = (i * NTSC_CC_PACKET_SIZE);
                u4ValidPackets++;

                if (eType == TVE_VBI_DATA_CC)    // line 21 (22)
                {
                    LOG(8, "NTSC CC1, CC2, T1, T2 Valid\n");
                    
                    ucAtscCc[0] = ucCcData[idx];
                    ucAtscCc[1] = ucCcData[idx + 1];
                    u8Pts = 0;//rCcData.u8Pts;
                    
                    u4RetVal = TVE_SendCc(TVE_1, TVE_FIELD_ODD, ucAtscCc, u8Pts);
                    if (u4RetVal != TVE_SET_OK)
                    {
                        LOG(8, "TVE_SendCc Invalid\n");
                    }
                }

                if (eType == TVE_VBI_DATA_XDS)    // line 284 (335)
                {
                    LOG(8, "NTSC CC3, CC4, T3, T4, XDS Valid\n");
                    
                    ucAtscCc[0] = ucCcData[idx];
                    ucAtscCc[1] = ucCcData[idx + 1];
                    u8Pts = 0;//rCcData.u8Pts;

                    u4RetVal = TVE_SendCc(TVE_1, TVE_FIELD_EVEN, ucAtscCc, u8Pts);
                    if (u4RetVal != TVE_SET_OK)
                    {
                        LOG(8, "TVE_SendCc Invalid\n");
                    }
                }
        } // ~for

        LOG(8, "NTSC CC Valid Packet = %u\n", u4ValidPackets);
    return u4RetVal;
}

UINT32 AuxOut_SendTTXData(UCHAR *pu1Buf, UINT32 length)
{
    UINT32 u4RetVal = TVE_SET_OK;
        u4RetVal = TVE_SendTTX(TVE_1, pu1Buf, length);
        if (u4RetVal != TVE_SET_OK)
        {
            LOG(8, "TVE_SendTTx Invalid\n");
        }
    return u4RetVal;
}

UINT32 AuxOut_SendWSSData(UCHAR *pu1Buf, UINT32 length)
{
    TVE_SendWss(TVE_1, pu1Buf, length);  

    return TVE_SET_OK;
}

UINT32 AuxOut_SendVbiData(UCHAR ucDACid, UCHAR eType, UCHAR *pu1Buf, UINT32 length)
{
    UINT32 i;
    UINT8 bVbi_type;
    UINT32 u4RetVal = TVE_SET_OK;
    
    VERIFY_DAC_ID(ucDACid);

    LOG(8, "vAuxOut_SendVbiData: DAC%x, Type=%x\n", ucDACid, eType);
    for(i = 0; i < length; i++)
    {
        LOG(8, "DATA%x=%x\n", i, *(pu1Buf+i));
    }

    bVbi_type = TVEVbiInternalMap[eType];
    switch(bVbi_type)
    {
         case TVE_VBI_DATA_CC:
         case TVE_VBI_DATA_XDS:
             u4RetVal = AuxOut_SendCCData(bVbi_type, pu1Buf, length);
         break;
         case TVE_VBI_DATA_TTX:
             u4RetVal = AuxOut_SendTTXData(pu1Buf, length);
         break;
         case TVE_VBI_DATA_WSS:
             u4RetVal = AuxOut_SendWSSData(pu1Buf, length);
         break;
         default:
             u4RetVal = TVE_SET_ERROR;
         break;
    }
    
    return u4RetVal;
}
#endif

#endif /* _TVE_IF_C_ */

