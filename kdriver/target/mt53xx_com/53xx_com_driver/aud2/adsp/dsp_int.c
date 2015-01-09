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
/***************    MTK CONFIDENTIAL & COPYRIGHTED     ****************/
/***************                                       ****************/
/***************  $Modtime:: 04/12/06 4:57p    $       ****************/
/***************  $Revision: #1 $       ****************/
/***************                                       ****************/
/***************   Description : DSP Control routines  ****************/
/***************                                       ****************/
/***************       Company : MediaTek Inc.         ****************/
/***************    Programmer : Bowen Ku              ****************/
/***************                 Jerry Shieh           ****************/
/***************                 Harrison Hsieh        ****************/
/***************                 Peichen Chang         ****************/
/***************                 SM Lee                ****************/
/**********************************************************************/
#define _DSP_INT_C

#include "dsp_reg.h"
#include "dsp_intf.h"
#include "d2rc_int.h"    // only use in this files
#include "dsp_func.h"
#include "dsp_table.h"
#include "dsp_uop.h"
#include "dsp_shm.h"
#include "dsp_data.h"
#include "dsp_rg_ctl.h"
#include "d2rc_shm.h"
#include "dsp_intf.h"

#include "aud_if.h"
#include "aud_debug.h"
#include "aud_drv.h"
#include "aud_dsp_cfg.h"

#include "x_assert.h"

extern UINT8 _au1MWSetSoundModeIdx;
#ifdef CC_SONY_2K14_FLASH_AQ
extern UINT8 _au1MWSetPositionIdx;
#endif

void vDspSetTable (UINT8 u1DecId, UINT32 u4FreqIdx);
void vDspSetSamplingRate(UINT8 u1DspId, UINT8 u1DecId);

#ifdef CC_AUD_BBE_SUPPORT
static void vSetBbeTable (UINT8 u1DecId);
#endif

#ifdef CC_AUD_DSP_SUPPORT_CDNOTCH
static void vSetCDNotchTable(void);
#endif

#if defined(DSP_SUPPORT_SRS_PEQ)
static void vSetPeqTable(UINT32 u4FreqIdx, BOOL fgFirstDecoder);
#endif
#if defined(DSP_SUPPORT_SRS_HPF)
static void vSetSrsHpfTable(UINT32 u4FreqIdx, BOOL fgFirstDecoder);
#endif

#ifdef DSP_SUPPORT_SRS_AEQ
static void vSetAeqTable(UINT32 u4FreqIdx, BOOL fgFirstDecoder);
#endif
#ifdef DSP_SUPPORT_SRS_LMT
static void vSetLmtTable(UINT32 u4FreqIdx, BOOL fgFirstDecoder);
#endif

#ifdef DSP_SUPPORT_SRS_SSHD
static void vSetSSHDTable(UINT32 u4FreqIdx, BOOL fgFirstDecoder);
#endif
#ifdef DSP_SUPPORT_SRS_PURESOUND
static void vSetPuresoundTable(UINT32 u4FreqIdx, BOOL fgFirstDecoder);
#endif
#ifdef DSP_SUPPORT_SRS_SS3D
static void vSetSS3DTable(UINT32 u4FreqIdx, BOOL fgFirstDecoder);
#endif

#ifdef DSP_SUPPORT_TCL_VBASS
static void vSetTclVbass (UINT8 u1DecId);
#endif

#ifdef DSP_SUPPORT_SRSVIQ
//sunman_viq
static void vSetViqTable(UINT32 u4FreqIdx, BOOL fgFirstDecoder);
extern UINT32 dwDspCalOfst (UINT32 dwOfst);
#endif

static void vSetBassManageTable (UINT8 u1DspId, UINT8 u1DecId);
static void vSetPostProcTable (UINT8 u1DecId, UINT32 u4FreqIdx);
static void vSetupUpMix (UINT8 u1DspId, UINT8 u1DecId, UINT32 u4FreqIdx);
static void vSetupDownMix (UINT8 u1DecId, UINT32 u4FreqIdx);
#ifdef CC_AUD_DSP_SUPPORT_AUDYSSEY
static void vSetABXTable(void);
#endif
static BOOL vSetIECRawDelay(UINT32 u4FreqIdx, UINT8 u1DecId);

#ifdef	CC_SONY_2K14_FLASH_AQ
extern void _AUD_DspSetTvPosition(UINT8 u1DecId,UINT8 u1Position,BOOL fgWithMute);
#endif
extern void _AUD_DspPostSoundMode(UINT8 u1DecId, UINT8 u1ModeIdx, BOOL fgWithMute);
#ifndef CC_AUD_NEW_POST_MULTI_VS_SUPPORT
extern void _AUD_DspVirtualSurroundEnalbe(UINT8 u1DecId, BOOL fgEnable);
#else
extern void _AUD_DspVirtualSurroundEnalbe(UINT8 u1DecId, UINT8 u1ModeIdx);
#endif


//extern void _AUD_DspSetDDCOEnable(BOOL fgEnable);
//extern AUD_IEC_T _GetIECFlag(UINT8 u1DecId);
#ifdef CC_AUD_HPF_SUPPORT
UINT8 FC_TO_TABLE_INDEX(UINT16 u2Fc)
{
    // Check support range
    u2Fc = (u2Fc > HPF_FC_END) ? HPF_FC_END : u2Fc;
    u2Fc = (u2Fc < HPF_FC_STR) ? HPF_FC_STR : u2Fc;

    // Transfer to table index
    return ((u2Fc - HPF_FC_STR) / HPF_FC_STEP);
}
#endif

#if 1//def CC_AUD_PEQ_SUPPORT
UINT32 dwGetPEQFsIdx(UINT8 u1Set)
{
    UINT32 u4FsIdx = 0;
    UINT32 u4FsIdx1 = 0;
    UNUSED(u4FsIdx);

    switch (_rDspVars[AUD_DSP0][AUD_DEC_MAIN].dwDspFreq)
    {
    /*
    case SFREQ_16K:
        u4FsIdx=16000;
        break;
    case SFREQ_32K:
        u4FsIdx=0x20c49;
        break;
    case SFREQ_8K:
        u4FsIdx=8000;
        break;
    case SFREQ_11K:
        u4FsIdx=11000;
        break;
    case SFREQ_12K:
        u4FsIdx=12000;
        break;
    case SFREQ_22K:
        u4FsIdx=22000;
        break;
    case SFREQ_44K:
        u4FsIdx=0x17c6f;
        break;
    case SFREQ_88K:
        u4FsIdx=88000;
        break;
    case SFREQ_176K:
        u4FsIdx=176000;
        break;
    case SFREQ_24K:
        u4FsIdx=24000;
        break;
        */
    case SFREQ_48K:
        u4FsIdx=0x15d86;// (1/48000)*(180/45)*(8192*2)
        break;
        /*
    case SFREQ_96K:
        u4FsIdx=96000;
        break;
    case SFREQ_192K:
        u4FsIdx=192000;
        break;
        */
    default:
        u4FsIdx=0x15d86;
        break;
    }

#if 0
    u4FsIdx1 = u2ReadDspShmWORD(W_PEQ_SET0_BW + 8*u1Set) * u4FsIdx;
    u4FsIdx1 = u4FsIdx1>>16;
    u4FsIdx1 = u4FsIdx1+0x08;
    u4FsIdx1 = u4FsIdx1>>1;
#else
    u4FsIdx1 = u2ReadDspShmWORD(AUD_DSP0, W_PEQ_SET0_BW + 8*u1Set);
    if(u4FsIdx1 > 23000)
        {
        u4FsIdx1 = 23000;
        }

    if(u4FsIdx1 > 2000)
        {
        u4FsIdx1 = (u4FsIdx1 - 2000 + 1)/2 +2000;
        }
#endif

    LOG(7, "dwDspFreq = %x\n",_rDspVars[AUD_DSP0][AUD_DEC_MAIN].dwDspFreq);
    LOG(7, "PEQu4FsIdx = %x\n",u4FsIdx1);

    // Check index range
    if (u4FsIdx1 > sizeof(MTK_TAN_TABLE)/4)
    {
        u4FsIdx1 = sizeof(MTK_TAN_TABLE)/4 - 1;
        LOG(7, "Error PEQ BW idx\n");
    }

    return u4FsIdx1;
}

UINT32 dwGetPEQCosIdx(UINT8 u1Set)
{
    UINT32 u4FsIdx = 0;

    u4FsIdx = u2ReadDspShmWORD(AUD_DSP0, W_PEQ_SET0_FC + 8*u1Set);
    if(u4FsIdx > 20000) // Check index range,FC less than 20K
    {
        u4FsIdx = 20000;
    }

    if(u4FsIdx > 2000)
    {
        u4FsIdx = (u4FsIdx - 2000 + 1)/2 +2000;
    }


    LOG(7, "u4FsIdx = %x\n",u4FsIdx);

    // Check index range
    if (u4FsIdx > sizeof(COS_TABLE)/4)
    {
        u4FsIdx = sizeof(COS_TABLE)/4 - 1;
        LOG(7, "Error PEQ FC idx\n");
    }

    return u4FsIdx;
}

void vLoadPEQTable(void)
{
    UINT32 i;
    UINT32 u4Index;
    UINT32 u4DramBase;

    u4DramBase = dReadDspCommDram(AUD_DSP0, ADDR_RC2D_PEQ_BASE);

    for (i=0; i<9; i++)
    {
        //UINT32 u4Index;
        u4Index = dwGetPEQFsIdx(i);
        VERIFY(u4Index < sizeof(MTK_TAN_TABLE)/4);
        WriteDspCommDram (AUD_DSP0, (u4DramBase + ADDR_PEQ1_BW + i*3), MTK_TAN_TABLE[u4Index] );
        u4Index = dwGetPEQCosIdx(i) ;
        WriteDspCommDram (AUD_DSP0, (u4DramBase + ADDR_PEQ1_FC + i*3), COS_TABLE[u4Index]);
    }
}
#ifdef CC_AUD_PEQ_LR
UINT32 dwGetPEQFsIdx_R(UINT8 u1Set)
{
    UINT32 u4FsIdx = 0;
    UINT32 u4FsIdx1 = 0;
    UNUSED(u4FsIdx);

    switch (_rDspVars[AUD_DEC_MAIN].dwDspFreq)
    {
    /*
    case SFREQ_16K:
        u4FsIdx=16000;
        break;
    case SFREQ_32K:
        u4FsIdx=0x20c49;
        break;
    case SFREQ_8K:
        u4FsIdx=8000;
        break;
    case SFREQ_11K:
        u4FsIdx=11000;
        break;
    case SFREQ_12K:
        u4FsIdx=12000;
        break;
    case SFREQ_22K:
        u4FsIdx=22000;
        break;
    case SFREQ_44K:
        u4FsIdx=0x17c6f;
        break;
    case SFREQ_88K:
        u4FsIdx=88000;
        break;
    case SFREQ_176K:
        u4FsIdx=176000;
        break;
    case SFREQ_24K:
        u4FsIdx=24000;
        break;
        */
    case SFREQ_48K:
        u4FsIdx=0x15d86;// (1/48000)*(180/45)*(8192*2)
        break;
        /*
    case SFREQ_96K:
        u4FsIdx=96000;
        break;
    case SFREQ_192K:
        u4FsIdx=192000;
        break;
        */
    default:
        u4FsIdx=0x15d86;
        break;
    }

#if 0
    u4FsIdx1 = u2ReadDspShmWORD(W_PEQ_SET0_BW + 8*u1Set) * u4FsIdx;
    u4FsIdx1 = u4FsIdx1>>16;
    u4FsIdx1 = u4FsIdx1+0x08;
    u4FsIdx1 = u4FsIdx1>>1;
#else
    u4FsIdx1 = u2ReadDspShmWORD(W_PEQ_SET0_BW_R + 8*u1Set);
    if(u4FsIdx1 > 23000)
        {
        u4FsIdx1 = 23000;
        }

    if(u4FsIdx1 > 2000)
        {
        u4FsIdx1 = (u4FsIdx1 - 2000 + 1)/2 +2000;
        }
#endif

    LOG(7, "dwDspFreq = %x\n",_rDspVars[AUD_DEC_MAIN].dwDspFreq);
    LOG(7, "PEQu4FsIdx = %x\n",u4FsIdx1);

    // Check index range
    if (u4FsIdx1 > sizeof(MTK_TAN_TABLE)/4)
    {
        u4FsIdx1 = sizeof(MTK_TAN_TABLE)/4 - 1;
        LOG(7, "Error PEQ BW idx\n");
    }

    return u4FsIdx1;
}

UINT32 dwGetPEQCosIdx_R(UINT8 u1Set)
{
    UINT32 u4FsIdx = 0;

    u4FsIdx = u2ReadDspShmWORD(W_PEQ_SET0_FC_R + 8*u1Set);
    if(u4FsIdx > 20000) // Check index range,FC less than 20K
    {
        u4FsIdx = 20000;
    }

    if(u4FsIdx > 2000)
    {
        u4FsIdx = (u4FsIdx - 2000 + 1)/2 +2000;
    }

    
    LOG(7, "u4FsIdx = %x\n",u4FsIdx);

    // Check index range
    if (u4FsIdx > sizeof(COS_TABLE)/4)
    {
        u4FsIdx = sizeof(COS_TABLE)/4 - 1;
        LOG(7, "Error PEQ FC idx\n");
    }

    return u4FsIdx;
}

void vLoadPEQTable_R(void)
{
    UINT32 i;
    UINT32 u4Index;
    UINT32 u4DramBase;

    u4DramBase = dReadDspCommDram(ADDR_RC2D_PEQ_BASE);

    for (i=0; i<9; i++)
    {
        //UINT32 u4Index;
        u4Index = dwGetPEQFsIdx_R(i);
        VERIFY(u4Index < sizeof(MTK_TAN_TABLE)/4);
        WriteDspCommDram ((u4DramBase + ADDR_PEQ1_BW_R + i*3), MTK_TAN_TABLE[u4Index] );
        u4Index = dwGetPEQCosIdx_R(i) ;
        if(u4Index < sizeof(COS_TABLE)/sizeof(COS_TABLE[0]))
        {
            WriteDspCommDram ((u4DramBase + ADDR_PEQ1_FC_R + i*3), COS_TABLE[u4Index]);
        }
    }
}
#endif
#endif

#ifdef CC_AUD_SPEAKER_HEIGHT_MODE
void vLoadSPHTable(void)
{
     UINT8 u1ModeIdx;
     UINT32 iLoopVar;
     UINT32 *pdwTable = NULL;
     UINT32 u4DramBase;

    u1ModeIdx = uReadDspShmBYTE(AUD_DSP0, B_SPH_MODE);
    u4DramBase = dReadDspCommDram(AUD_DSP0, ADDR_RC2D_SPKHEIGHT_BASE);
	
    if (u1ModeIdx ==1)
    {
        pdwTable = (UINT32 *)&MTK_SHELF_1DB_TABLE[0];
    }
    else if (u1ModeIdx == 2)
    {
        pdwTable = (UINT32 *)&MTK_SHELF_2DB_TABLE[0];
    }
    else if (u1ModeIdx == 3)
    {
        pdwTable = (UINT32 *)&MTK_SHELF_3DB_TABLE[0];
    }
    else if (u1ModeIdx == 4)
    {
        pdwTable = (UINT32 *)&MTK_SHELF_4DB_TABLE[0];
    }
    else if (u1ModeIdx == 5)
    {
        pdwTable = (UINT32 *)&MTK_SHELF_5DB_TABLE[0];
    }
    else if (u1ModeIdx == 6)
    {
        pdwTable = (UINT32 *)&MTK_SHELF_6DB_TABLE[0];
    }


    if (pdwTable == NULL)
    {
        return;
    }

    for (iLoopVar = 0; iLoopVar < 5; iLoopVar++)
    {
        WriteDspCommDram (AUD_DSP0, (u4DramBase + ADDR_SHELF_R_COEF + iLoopVar),
                          pdwTable[iLoopVar]);
        WriteDspCommDram (AUD_DSP0, (u4DramBase + ADDR_SHELF_L_COEF + iLoopVar),
                          pdwTable[iLoopVar]);
    }
}
#endif

#ifdef CC_AUD_FIR_SUPPORT
void vSetFIRTable (UINT8 u1DecId, UINT32 u4FreqIdx)
{
	UINT8 u1ModeIdx;
    UINT32 iLoopVar;
    UINT32 *pdwTable = NULL;
    UINT32 u4DramAdrIdx;

    UNUSED(u4FreqIdx);
    UNUSED(u1ModeIdx);

#ifdef CC_AUD_USE_FLASH_AQ
#ifdef	CC_SONY_2K14_FLASH_AQ
    if(_au1MWSetPositionIdx==1)
    {
        Aud_FlashAQQuery(AQ_TYPE_FIR_TABLE, (void*)FIR_TABLE);
        pdwTable = (UINT32 *)&FIR_TABLE[0];
    }
	else
	{
        Aud_FlashAQQuery(AQ_TYPE_FIR_WALL, (void*)FIR_TABLE);
        pdwTable = (UINT32 *)&FIR_TABLE[0];

	}
#else
    Aud_FlashAQQuery(AQ_TYPE_FIR, (void*)FIR_TABLE);
    pdwTable = (UINT32 *)&FIR_TABLE[0];
#endif
#else
    u1ModeIdx = uReadDspShmBYTE(B_FIR_ENABLE);
    if (u1ModeIdx ==1)
    {
        pdwTable = (UINT32 *)&FIR_TABLE_1[0];
    }
    else if (u1ModeIdx == 2)
    {
        pdwTable = (UINT32 *)&FIR_TABLE_2[0];
    }
    else if (u1ModeIdx == 3)
    {
        pdwTable = (UINT32 *)&FIR_TABLE_3[0];
    }
#endif

    if (pdwTable == NULL)
    {
        return;
    }

    if (u1DecId == AUD_DEC_MAIN)
    {
        u4DramAdrIdx = dReadDspCommDram(ADDR_RC2D_FIR_BASE) + ADDR_RC2D_FIR_FILTER_COEFF;

        for (iLoopVar = 0; iLoopVar < DSP_FIR_SZ; iLoopVar++)
        {
            WriteDspCommDram ((u4DramAdrIdx + iLoopVar), pdwTable[iLoopVar]);
        }
    }
    else                        // secondary decoder
    {
        /*
        for (iLoopVar = 0; iLoopVar < DSP_FIR_SZ; iLoopVar++)
        {
            WriteDspCommDram (ADDR_RC2D_FIR_FILTER_COEFF +
                              iLoopVar, pdwTable[iLoopVar]);
        }
        */
        // No dec2 now.
    }

}
#endif


#ifdef CC_AUD_NEW_POST_MULTI_VS_SUPPORT
void vLoadVirtualSurroundTable(void)
{
    UINT16 u2ModeIdx;
    UINT32 iLoopVar;
    UINT32 *pdwTable;
    UINT32 u4DramBase;

    u2ModeIdx = u2ReadDspShmWORD(AUD_DSP0, W_VSURRFLAG);
    u4DramBase = dReadDspCommDram(AUD_DSP0, ADDR_RC2D_M2S_BASE);

    if (u2ModeIdx == VSS_MTK)
    {
#ifndef DSP_SUPPORT_SRSWOW
#ifndef DSP_SUPPORT_SRSTSXT
        pdwTable = (UINT32 *)&MTK_VSURR_TABLE[0];
        for (iLoopVar = 0; iLoopVar < 20; iLoopVar++)
        {
            WriteDspCommDram (AUD_DSP0, (ADDR_VSURR_IIR_COEF + iLoopVar),
                pdwTable[iLoopVar]);
        }

        // MTK Vsurr Crosstalk Filter Coefficient
        pdwTable = (UINT32 *)&MTK_VSURR_CROS_TABLE[0];
        for (iLoopVar = 0; iLoopVar < 15; iLoopVar++)
        {
            WriteDspCommDram (AUD_DSP0, (ADDR_VSURR_CROS_COEF + iLoopVar),
                pdwTable[iLoopVar]);
        }

        pdwTable = (UINT32 *)&MTK_VSURR_BASS_TABLE[0];
        for (iLoopVar = 0; iLoopVar < 5; iLoopVar++)
        {
            WriteDspCommDram (AUD_DSP0, (ADDR_VSURR_BASS_COEF + iLoopVar),
                pdwTable[iLoopVar]);
        }

#endif //DSP_SUPPORT_SRSTSXT
#endif //DSP_SUPPORT_SRSWOW
    }

    if (u2ModeIdx == VSS_M2S)
    {
        // mono 2 stereo
        pdwTable = (UINT32 *)&MTK_COMB_R_TABLE[0];
        for (iLoopVar = 0; iLoopVar < 70; iLoopVar++)
        {
            WriteDspCommDram (AUD_DSP0, (u4DramBase + ADDR_M2S_COMB_R_COEF + iLoopVar),
                              pdwTable[iLoopVar]);
        }

        pdwTable = (UINT32 *)&MTK_COMB_L_TABLE[0];
        for (iLoopVar = 0; iLoopVar < 70; iLoopVar++)
        {
            WriteDspCommDram (AUD_DSP0, (u4DramBase + ADDR_M2S_COMB_L_COEF + iLoopVar),
                              pdwTable[iLoopVar]);
        }

        pdwTable = (UINT32 *)&MTK_M2S_ALLPASS_TABLE[0];
        for (iLoopVar = 0; iLoopVar < 20; iLoopVar++)
        {
            WriteDspCommDram (AUD_DSP0, (u4DramBase +ADDR_M2S_ALLPASS_COEF + iLoopVar),
                              pdwTable[iLoopVar]);
        }
    }
    else
    {
        WriteDspCommDram (AUD_DSP0, ADDR_VSURR_CROS_PARA1,u4ReadDspShmDWRD(AUD_DSP0, D_VSURR_CROS_TALK)); //0x8400
        WriteDspCommDram (AUD_DSP0, ADDR_VSURR_CROS_PARA2,u4ReadDspShmDWRD(AUD_DSP0, D_VSURR_WIDTH));  //0x8401
        WriteDspCommDram (AUD_DSP0, ADDR_VSURR_CROS_PARA3,u4ReadDspShmDWRD(AUD_DSP0, D_VSURR_LR_GAIN));  //0x8402
        WriteDspCommDram (AUD_DSP0, ADDR_VSURR_GAIN_MPY,u4ReadDspShmDWRD(AUD_DSP0, D_VSURR_CLARITY));  //0x8406
        WriteDspCommDram (AUD_DSP0, (ADDR_VSURR_BASS_COEF + 3),u4ReadDspShmDWRD(AUD_DSP0, D_VSURR_FO)); //0x843c
        WriteDspCommDram (AUD_DSP0, ADDR_VSURR_CROS_PARA4,u4ReadDspShmDWRD(AUD_DSP0, D_VSURR_BASS_GAIN));  //0x843e
        WriteDspCommDram (AUD_DSP0, ADDR_VSURR_CROS_PARA5,u4ReadDspShmDWRD(AUD_DSP0, D_VSURR_OUTPUT_GAIN)); //0x843f
    }
}

#endif

/***************************************************************************
     Function : vDspSetSamplingRate
  Description : setup sampling rate related configuration
                Init Src speaker config
    Parameter :
    Return    :
***************************************************************************/
void vDspSetSamplingRate(UINT8 u1DspId, UINT8 u1DecId)
{
    UINT32 u4FreqIdx = SFREQ_48K;
    BOOL fgValidFreq;
    UINT16 u2SpkCfg;
    UINT32 u4DataRate = 0;
    BOOL fgFreqChange = FALSE;

    AUD_DEC_ID_VALIDATE(u1DecId);

    fgValidFreq = TRUE;

#if defined(CC_MT5365) || defined(CC_MT5395) //5368/96 supports 4th decoder
    if (u1DecId != AUD_DEC_4TH)
#endif
    {
        if (_rDspStatus[u1DspId][u1DecId].fgDspAoutMuted)
        {
            if (u1DecId == AUD_DEC_MAIN)
            {
                vDspCmd(u1DspId, UOP_DSP_MASTER_VOLUME);
            }
            else if (u1DecId == AUD_DEC_AUX)
            {
                vDspCmd(u1DspId, UOP_DSP_MASTER_VOLUME_DEC2);
            }
        #ifdef CC_MT5391_AUD_3_DECODER
            else if (u1DecId == AUD_DEC_THIRD)
            {
                vDspCmd(u1DspId, UOP_DSP_MASTER_VOLUME_DEC3);
            }
        #endif
        #if !defined(CC_MT5365) && !defined(CC_MT5395) //5368/96 supports 4th decoder
            #ifdef CC_AUD_4_DECODER_SUPPORT
            else if (u1DecId == AUD_DEC_4TH)
            {
                vDspCmd(u1DspId, UOP_DSP_MASTER_VOLUME_DEC4);
            }
            #endif
        #endif

            _rDspStatus[u1DspId][u1DecId].fgDspAoutMuted = FALSE;
        }
    }

    // For mp3 get mpeg type
    // if it is mp3 -> it will go here
    //  we stop dsp here
    // else if it is mp2
    //  we stop dsp at dsp interrupt
    if ((u1DecId == AUD_DEC_MAIN) && (_rDspVars[u1DspId][u1DecId].bDspStrTyp == MPEG3_STREAM))
    {
        if (_rDspStatus[u1DspId][u1DecId].fgDspGetMpgTyp)
        {
            _rDspStatus[u1DspId][u1DecId].fgDspMpgTypGot = TRUE;
            _rDspStatus[u1DspId][u1DecId].fgDspGetMpgTyp = FALSE;
            if (u1DecId == AUD_DEC_MAIN)
            {
                vDspCmd (u1DspId, DSP_STOP);
            }
            else if (u1DecId == AUD_DEC_AUX)
            {
                vDspCmd(u1DspId, DSP_STOP_DEC2);
            }
            else if (u1DecId == AUD_DEC_THIRD)
            {
                vDspCmd(u1DspId, DSP_STOP_DEC3); 
            }
            return;
        }
    }



    //Extract SFreq
     u4FreqIdx = u4DspGetSampleRate(u1DspId, u1DecId);
    // Move the check to AUD_SetSampleFreq() function, since in HDMI mode,
    // driver may modify sampling frequency, so checking _dwDspFreq only is not okay
    if ((u1DecId == AUD_DEC_MAIN) || (u4FreqIdx != _rDspVars[u1DspId][u1DecId].dwDspFreq))
    {
        _rDspVars[u1DspId][u1DecId].bCode3D = CODE3D_NONE;
        _rDspVars[u1DspId][u1DecId].bCode3DUpmix = CODE3D_NONE;
        _rDspVars[u1DspId][u1DecId].dwDspFreq = u4FreqIdx;
        fgValidFreq = TRUE;

        switch (u4FreqIdx)
        {
        case SFREQ_8K:        // 8K
            _rDspVars[u1DspId][u1DecId].dwDacFreq = (UINT32)FS_8K;
            _rDspVars[u1DspId][u1DecId].dwTableFreq = SAMPLE_32K;
            break;
        case SFREQ_16K:        // 16K
            _rDspVars[u1DspId][u1DecId].dwDacFreq = (UINT32)FS_16K;
            _rDspVars[u1DspId][u1DecId].dwTableFreq = SAMPLE_32K;
            break;
        case SFREQ_22K:        // 22K
            _rDspVars[u1DspId][u1DecId].dwDacFreq = (UINT32)FS_22K;
            _rDspVars[u1DspId][u1DecId].dwTableFreq = SAMPLE_32K;
            break;
        case SFREQ_24K:        // 24K
            _rDspVars[u1DspId][u1DecId].dwDacFreq = (UINT32)FS_24K;
            _rDspVars[u1DspId][u1DecId].dwTableFreq = SAMPLE_32K;
            break;
        case SFREQ_32K:        // 32K
            _rDspVars[u1DspId][u1DecId].dwDacFreq = (UINT32)FS_32K;
            _rDspVars[u1DspId][u1DecId].dwTableFreq = SAMPLE_32K;
            break;
        case SFREQ_44K:        // 44K
            _rDspVars[u1DspId][u1DecId].dwDacFreq = (UINT32)FS_44K;
            _rDspVars[u1DspId][u1DecId].dwTableFreq = SAMPLE_44K;
            break;
        case SFREQ_48K:        // 48K
        case SFREQ_52K:        //for line in
        case SFREQ_56K:        //for line in
        case SFREQ_62K:
        case SFREQ_64K:
            _rDspVars[u1DspId][u1DecId].dwDacFreq = (UINT32)FS_48K;
            _rDspVars[u1DspId][u1DecId].dwTableFreq = SAMPLE_48K;
            break;
        case SFREQ_88K:        // 88K
            _rDspVars[u1DspId][u1DecId].dwDacFreq = (UINT32)FS_88K;
            _rDspVars[u1DspId][u1DecId].dwTableFreq = SAMPLE_96K;
            break;
        case SFREQ_96K:        // 96K
            _rDspVars[u1DspId][u1DecId].dwDacFreq = (UINT32)FS_96K;
            _rDspVars[u1DspId][u1DecId].dwTableFreq = SAMPLE_96K;
            break;
        case SFREQ_176K:       // 176K
            _rDspVars[u1DspId][u1DecId].dwDacFreq = (UINT32)FS_176K;
            _rDspVars[u1DspId][u1DecId].dwTableFreq = SAMPLE_192K;
            break;
        case SFREQ_192K:       // 192K
            _rDspVars[u1DspId][u1DecId].dwDacFreq = (UINT32)FS_192K;
            _rDspVars[u1DspId][u1DecId].dwTableFreq = SAMPLE_192K;
            break;
        default:
            fgValidFreq = FALSE;
            LOG(1,"DspSetSamplingRate:u4FreqIdx(0x%x)\n",u4FreqIdx);
            break;
        }
    }

    fgFreqChange = TRUE;

    if (((u1DecId == AUD_DEC_MAIN) || (u1DecId == AUD_DEC_AUX) || (u1DecId == AUD_DEC_THIRD)) && fgValidFreq)            // && _bDspStrTyp != SACD_STREAM)
    {
        vDspSetFreq (u1DecId, _rDspVars[u1DspId][u1DecId].dwDacFreq, fgFreqChange);
        //Work around for DDP sampling rate change test fail issue.
#ifdef CC_MT5391_AUD_3_DECODER
        if((u1DecId == AUD_DEC_AUX) || (AUD_IsSourceFromWFD(u1DecId)))
#else
        if (u1DecId == AUD_DEC_MAIN)
#endif
        {
            LOG(5, "====>Set dec0 Asrc input frequency when set dec1 sampling rate in TV source\n");
            _AUD_DspAsrcSetBaseFreq(0, _rDspVars[u1DspId][u1DecId].dwDacFreq);
        }
        // IEC RAW delay
        vSetIECRawDelay (_rDspVars[u1DspId][u1DecId].dwDacFreq,u1DecId);
    }

    DSP_FlushInvalidateDCache(u1DspId, CACHE_FLUSH_TYPE_DATA, TYPE_COMM_DRAM_IDX);

    if (u1DecId == AUD_DEC_MAIN)
    {
        u2SpkCfg = wReadDspWORD (u1DspId, ADDR_D2RC_RISC_INFO_INPUT_CH_CFG);
        vTrnsltSpkCfg (u2SpkCfg, &_rDspVars[u1DspId][u1DecId].tSrcSpkCfg);
    }
    else if (u1DecId == AUD_DEC_AUX)
    {
        u2SpkCfg = wReadDspWORD (u1DspId, ADDR_D2RC_RISC_INFO_INPUT_CH_CFG_DEC2);
        vTrnsltSpkCfg (u2SpkCfg, &_rDspVars[u1DspId][u1DecId].tSrcSpkCfg);
    }

    // Set DMX interrupt threshold according to bitrate
    if (_rDspVars[u1DspId][u1DecId].bDspStrTyp == MPEG12_STREAM)
    {
        if (u1DecId == AUD_DEC_MAIN)
        {
            u4DataRate = dReadDspCommDram(u1DspId, ADDR_D2RC_MPEG_BITRATE);
        }
        else if (u1DecId == AUD_DEC_AUX)
        {
            u4DataRate = dReadDspCommDram(u1DspId, ADDR_D2RC_MPEG_BITRATE_DEC2);
        }
#ifdef CC_MT5391_AUD_3_DECODER
        else if (u1DecId == AUD_DEC_THIRD)
        {
            u4DataRate = dReadDspCommDram(u1DspId, ADDR_D2RC_MPEG_BITRATE_DEC3);
        }
#endif

        if (u4DataRate >= 256000)
        {
            AUD_DrvDmxIntThreshold(u1DecId, 1);          // Set to 188*1
        }
    }

#ifdef CC_SONY_2K14_FLASH_AQ
	if (!(_au1MWSetPositionIdx == 0xff))
	{
		_AUD_DspSetTvPosition(AUD_DEC_MAIN, _au1MWSetPositionIdx, FALSE); // Mute/Unmute is controlled by AP Start End
	}
#endif

    if (!(_au1MWSetSoundModeIdx == 0xff))
    {
        _AUD_DspPostSoundMode(AUD_DEC_MAIN, _au1MWSetSoundModeIdx, FALSE); // Mute/Unmute is controlled by AP Start End
    }

    if (u1DecId == AUD_DEC_MAIN)
    {
        // move table to DSP working area
        vDspSetTable(AUD_DEC_MAIN, _rDspVars[u1DspId][u1DecId].dwTableFreq);
    }
    else if (u1DecId == AUD_DEC_AUX)
    {
        // move table to DSP working area
        vDspSetTable(AUD_DEC_AUX, _rDspVars[u1DspId][u1DecId].dwTableFreq);
    }

    // Load ramcode for PostProcessing
    // Let Dsp continue to play

    vDspSendSample(u1DspId, u1DecId);
}

void vDspUpdatePostEffect(void)
{
    //srs headroom
    WriteDspCommDram32bit (AUD_DSP0, ADDR_SRS_HEADROOM, u4ReadDspShmDWRD (AUD_DSP0, D_SRS_HEADROOM) << 16);
    WriteDspCommDram32bit (AUD_DSP0, ADDR_SRS_HEADROOM + 1, u4ReadDspShmDWRD (AUD_DSP0, D_SRS_COMPENSATE) << 16);

    vSetPostProcTable (AUD_DEC_MAIN, _rDspVars[AUD_DSP0][AUD_DEC_MAIN].dwTableFreq);
    vSetupUpMix       (AUD_DSP0, AUD_DEC_MAIN, _rDspVars[AUD_DSP0][AUD_DEC_MAIN].dwTableFreq);
    vSetupDownMix     (AUD_DEC_MAIN, _rDspVars[AUD_DSP0][AUD_DEC_MAIN].dwTableFreq);
#ifdef CC_AUD_BBE_SUPPORT
    vSetBbeTable(AUD_DEC_MAIN);
#endif
#ifdef DSP_SUPPORT_SRS_SSHD
    vSetSSHDTable(2, TRUE);
#endif
#ifdef DSP_SUPPORT_SRS_PURESOUND
    vSetPuresoundTable(2, TRUE);
#endif
#ifdef DSP_SUPPORT_SRS_SS3D
    vSetSS3DTable(2, TRUE);
#endif
#if defined(DSP_SUPPORT_SRS_PEQ)
    vSetPeqTable(2, TRUE);
#endif
#if defined(DSP_SUPPORT_SRS_HPF)
    vSetSrsHpfTable(2, TRUE);
#endif
#ifdef DSP_SUPPORT_SRS_AEQ
    vSetAeqTable(2, TRUE);
#endif
#ifdef DSP_SUPPORT_SRS_LMT
    vSetLmtTable(2, TRUE);
#endif
#ifdef DSP_SUPPORT_SRSVIQ
    vSetViqTable(2, TRUE);//sunman_viq
#endif
#ifdef DSP_SUPPORT_TCL_VBASS	
	vSetTclVbass (AUD_DEC_MAIN);
#endif
    vSetBassManageTable (AUD_DSP0, AUD_DEC_MAIN);
#ifdef CC_AUD_FIR_SUPPORT
    vSetFIRTable (AUD_DEC_MAIN, _rDspVars[AUD_DEC_MAIN].dwTableFreq);
#endif
    DspIntOrSram(AUD_DSP0, SRAM_EFFECT_CONTROL,0x01000);
}

#ifdef CC_AUD_EFFECT_MUTE_PROTECT
void vDspEffectReInitUnMuteSurround(void)
{
    _AUD_DspSurroundMute(AUD_DEC_MAIN, AUD_CH_FRONT_LEFT, FALSE);
    _AUD_DspSurroundMute(AUD_DEC_MAIN, AUD_CH_FRONT_RIGHT, FALSE);
}

void vDspEffectReInitUnMuteVBass(void)
{
    _AUD_DspVBassMute(AUD_CH_FRONT_LEFT, FALSE);
    _AUD_DspVBassMute(AUD_CH_FRONT_RIGHT, FALSE);
}
#endif

/***************************************************************************
     Function :
  Description : Set table ...
    Parameter :
    Return    :
***************************************************************************/
void vDspSetTable (UINT8 u1DecId, UINT32 u4FreqIdx)
{
    AUD_DEC_ID_VALIDATE_2(u1DecId);

    //srs headroom
    WriteDspCommDram32bit (AUD_DSP0, ADDR_SRS_HEADROOM, u4ReadDspShmDWRD (AUD_DSP0, D_SRS_HEADROOM) << 16);
    WriteDspCommDram32bit (AUD_DSP0, ADDR_SRS_HEADROOM + 1, u4ReadDspShmDWRD (AUD_DSP0, D_SRS_COMPENSATE) << 16);

    vSetPostProcTable (u1DecId, u4FreqIdx);
    vSetupUpMix       (AUD_DSP0, u1DecId, u4FreqIdx);
    vSetupDownMix     (u1DecId, u4FreqIdx);
#ifdef CC_AUD_BBE_SUPPORT
    vSetBbeTable(u1DecId);
#endif
    vSetBassManageTable(AUD_DSP0, u1DecId);
#ifdef CC_AUD_DSP_SUPPORT_AUDYSSEY
    vSetABXTable ();
#endif

#ifdef CC_AUD_FIR_SUPPORT
    vSetFIRTable (u1DecId, u4FreqIdx);
#endif
#ifdef CC_AUD_DSP_SUPPORT_CDNOTCH
    vSetCDNotchTable();
#endif
#ifdef DSP_SUPPORT_SRS_SSHD
    vSetSSHDTable(2, TRUE);
#endif
#ifdef DSP_SUPPORT_SRS_PURESOUND
    vSetPuresoundTable(2, TRUE);
#endif
#ifdef DSP_SUPPORT_SRS_SS3D
    vSetSS3DTable(2, TRUE);
#endif
#if defined(DSP_SUPPORT_SRS_PEQ)
    vSetPeqTable(u4FreqIdx, TRUE);
#endif
#if defined(DSP_SUPPORT_SRS_HPF)
    vSetSrsHpfTable(2, TRUE);
#endif
#ifdef DSP_SUPPORT_SRS_AEQ
    vSetAeqTable(u4FreqIdx, TRUE);
#endif
#ifdef DSP_SUPPORT_SRS_LMT
    vSetLmtTable(2, TRUE);
#endif

#ifdef DSP_SUPPORT_SRSVIQ
    vSetViqTable(u4FreqIdx, TRUE);//sunman_viq
#endif

#ifdef DSP_SUPPORT_TCL_VBASS	
	vSetTclVbass (AUD_DEC_MAIN);
#endif


    //Check if DRC Auto and Set DRC Mode
    if (u1DecId == AUD_DEC_MAIN)
    {
        if (u2ReadDspShmWORD (AUD_DSP0, W_PROCMOD) & 0x8)
        {
            vDspCmd (AUD_DSP0, UOP_DSP_PROCESSING_MODE);
        }
    }
    else
    {
        if (u2ReadDspShmWORD (AUD_DSP0, W_PROCMOD_DEC2) & 0x8)
        {
            vDspCmd (AUD_DSP0, UOP_DSP_PROCESSING_MODE_DEC2);
        }
    }
}

#ifdef CC_AUD_HPF_SUPPORT
void vLoadHPFTable(void)
{
    const UINT32 * pu4TableAddr;
    UINT32   u4FcIndx;
    INT32    i;

    // Find out table address
    u4FcIndx = FC_TO_TABLE_INDEX(u2ReadDspShmWORD(W_HPF_FC));
    // LOG(0, "Fc Idx = %d\n", u4FcIndx);
    pu4TableAddr = &ORDER_6_FILTER_TABLE[u4FcIndx][0];

#if 1
    // Perform loading coefficient table
    for (i = 0; i < HPF_TABLE_SET_SZ; i++)
    {
        WriteDspCommDram ((DRAM_HPF_BUFFER + i), pu4TableAddr[i]);
        WriteDspCommDram ((DRAM_HPF_BUFFER + i + 5), pu4TableAddr[i]);
        WriteDspCommDram ((DRAM_HPF_BUFFER + i + 10), pu4TableAddr[i]);
    }

    // Re-initialize DSP working buffer
    for (i = HPF_TABLE_SET_SZ*3; i < 39; i++)
    {
        WriteDspCommDram (DRAM_HPF_BUFFER + i, 0x00000000);
    }


    // Find out table address
    u4FcIndx = FC_TO_TABLE_INDEX(u2ReadDspShmWORD(W_HPF2_FC));
    // LOG(0, "Fc Idx = %d\n", u4FcIndx);
    pu4TableAddr = &ORDER_6_FILTER_TABLE[u4FcIndx][0];

    // Perform loading coefficient table HPF2
    for (i = 0; i < HPF_TABLE_SET_SZ; i++)
    {
        WriteDspCommDram ((DRAM_HPF2_BUFFER + i), pu4TableAddr[i]);
        WriteDspCommDram ((DRAM_HPF2_BUFFER + i + 5), pu4TableAddr[i]);
        WriteDspCommDram ((DRAM_HPF2_BUFFER + i + 10), pu4TableAddr[i]);
    }

    // Re-initialize DSP working buffer HPF2
    for (i = HPF_TABLE_SET_SZ*3; i < 39; i++)
    {
        WriteDspCommDram (DRAM_HPF2_BUFFER + i, 0x00000000);
    }

#else

    /*b2*/ WriteDspCommDram (DRAM_HPF_BUFFER + 0, 0x003f6978);
    /*b1*/ WriteDspCommDram (DRAM_HPF_BUFFER + 1, 0xff812d12);
    /*a2*/ WriteDspCommDram (DRAM_HPF_BUFFER + 2, 0xffc12bac);
    /*a1*/ WriteDspCommDram (DRAM_HPF_BUFFER + 3, 0x007ed18a);
    /*b0*/ WriteDspCommDram (DRAM_HPF_BUFFER + 4, 0x003f6978);

    /*b2*/ WriteDspCommDram (DRAM_HPF_BUFFER + 5, 0x003f6978);
    /*b1*/ WriteDspCommDram (DRAM_HPF_BUFFER + 6, 0xff812d12);
    /*a2*/ WriteDspCommDram (DRAM_HPF_BUFFER + 7, 0xffc12bac);
    /*a1*/ WriteDspCommDram (DRAM_HPF_BUFFER + 8, 0x007ed18a);
    /*b0*/ WriteDspCommDram (DRAM_HPF_BUFFER + 9, 0x003f6978);

    /*b2*/ WriteDspCommDram (DRAM_HPF_BUFFER + 10, 0x003f6978);
    /*b1*/ WriteDspCommDram (DRAM_HPF_BUFFER + 11, 0xff812d12);
    /*a2*/ WriteDspCommDram (DRAM_HPF_BUFFER + 12, 0xffc12bac);
    /*a1*/ WriteDspCommDram (DRAM_HPF_BUFFER + 13, 0x007ed18a);
    /*b0*/ WriteDspCommDram (DRAM_HPF_BUFFER + 14, 0x003f6978);

    // Re-initialize DSP working buffer
    for (i = HPF_TABLE_SET_SZ; i < 39; i++)
    {
        WriteDspCommDram (DRAM_HPF_BUFFER + i, 0x00000000);
    }

#endif

}
#endif

#ifdef CC_AUD_VBASS_SUPPORT
void vDspVBassCutOffCoeff(void)
{
    UINT32 iLoopVar;
    UINT32 *pdwTable;
    UINT8 u1CutOffFreq;
    UINT32 u4DramBase;

    UNUSED(vDspVBassCutOffCoeff);

    u1CutOffFreq = AUD_GetVBassCutOff();
    if (u1CutOffFreq>=5) u1CutOffFreq = 5; //gallen 20110808, Fix Clockwork check
    u4DramBase = dReadDspCommDram(AUD_DSP0, ADDR_RC2D_VBASS_BASE);
    LOG(7, "VBASSS: u1CutOffFreq = %d\n", u1CutOffFreq);
    pdwTable = (UINT32 *)&MTK_VBASS_CUTOFF_TABLE[u1CutOffFreq][0];
    for (iLoopVar = 25; iLoopVar < 30; iLoopVar++)
    {
        WriteDspCommDram (AUD_DSP0, (u4DramBase + ADDR_VBASS_R_COEF + iLoopVar), pdwTable[iLoopVar-25]);
        //LOG(7,"R: %d, 0x%x\n", iLoopVar, pdwTable[iLoopVar-25]);
    }
    pdwTable = (UINT32 *)&MTK_VBASS_CUTOFF_TABLE[u1CutOffFreq][0];
    for (iLoopVar = 25; iLoopVar < 30; iLoopVar++)
    {
       WriteDspCommDram (AUD_DSP0, (u4DramBase + ADDR_VBASS_L_COEF + iLoopVar), pdwTable[iLoopVar-25]);
        //LOG(7,"L: %d, 0x%x\n", iLoopVar, pdwTable[iLoopVar-25]);
    }
}
#endif //CC_AUD_VBASS_SUPPORT

void vSetPostProcTable (UINT8 u1DecId, UINT32 u4FreqIdx)
{
    UINT32 iLoopVar;
    UINT32 *pdwTable;
    UINT32 u4DramBase;

#ifdef CC_VOCAL_MUTE
    UINT8 u1Index;
#endif

    UNUSED(u4DramBase);
    UNUSED(pdwTable);

    if (u1DecId == AUD_DEC_MAIN) // First decoder
    {
        //================ Delay Factor ================
        WriteDspCommDram (AUD_DSP0, ADDR_RC2D_CH_DELAY_FACTOR, DELAY_FACTOR[SAMPLE_48K]);
        //Channel Time Delay
        for (iLoopVar = 0; iLoopVar < 10; iLoopVar++)
        {
            WriteDspCommDram (AUD_DSP0, (ADDR_RC2D_CH_DELAY_C + iLoopVar),
                u2ReadDspShmWORD (AUD_DSP0, (W_CHDELAY_C + (iLoopVar * 2))));
        }
        //================ Equalizer IIR Coefficient ================
        // 20050929 Jack move the "Set all Channel gain" from UOP to here to prevent the pop noise
        // Dry Gain
        WriteDspCommDram (AUD_DSP0, ADDR_RC2D_EQUALIZER_DRY_LEVEL,
            (u4ReadDspShmDWRD (AUD_DSP0, D_EQ_DRY)));
        vWriteDspWORD(AUD_DSP0, ADDR_RC2D_EQUALIZER_BAND_NUM_1, (AUD_EQ_IIR_BAND_NUM-1));
        vEQBandGainFlush(AUD_DEC_MAIN);

        // 0x1 (2.22), 0x02 (3.21), 0x03 (4.20), 0x04 (5.19)
#ifdef CC_AUD_7BAND_EQUALIZER
        vWriteDspWORD (AUD_DSP0, ADDR_RC2D_EQUALIZER_BAND1_SHIFT_BIT,0x1);
        vWriteDspWORD (AUD_DSP0, ADDR_RC2D_EQUALIZER_BAND2_SHIFT_BIT,0x3);
#else
        vWriteDspWORD (AUD_DSP0, ADDR_RC2D_EQUALIZER_BAND1_SHIFT_BIT,0x1);
        vWriteDspWORD (AUD_DSP0, ADDR_RC2D_EQUALIZER_BAND2_SHIFT_BIT,0x3);
#endif

        // Enable update flag
        vWriteDspWORD(AUD_DSP0, ADDR_RC2D_EQUALIZER_TABLE_CHANGE, 0x1);

        //================ Super Bass Pipe ================
        vEQ2SbassBandGainFlush(AUD_DEC_MAIN);
        // Enable update flag
        vWriteDspWORD(AUD_DSP0, ADDR_RC2D_SUPER_BASS_TABLE_CHANGE, 0x1);

        //================ AVC table ================
#ifndef DSP_SUPPORT_SRSVIQ //sunman_viq
        pdwTable = (UINT32 *)&AVC_PARA_TAB[0];
        for (iLoopVar=0; iLoopVar<38; iLoopVar++)
        {
            WriteDspCommDram (AUD_DSP0, (ADDR_RC2D_AVC_AVERAGE_FACTOR_TABLE + iLoopVar),
                              pdwTable[iLoopVar]);
        }
#endif

#ifdef CC_AUD_SUPPORT_MS10
//================ MXM table ================
        pdwTable = (UINT32 *) &xmxchsclgainstolineartab[0];
        for (iLoopVar = 0; iLoopVar < 16; iLoopVar ++)
        {
            WriteDspCommDram (AUD_DSP0, (ADDR_RC2D_XMX_TABLE + iLoopVar), pdwTable[iLoopVar]);
        }

        pdwTable = (UINT32 *) &xmxpgmscltolineartab[0];
        for (iLoopVar = 0; iLoopVar < 64; iLoopVar ++)
        {
            WriteDspCommDram (AUD_DSP0, (ADDR_RC2D_XMX_TABLE + 16 + iLoopVar), pdwTable[iLoopVar]);
        }

        pdwTable = (UINT32 *) &xmxgaininterptab[0];
        for (iLoopVar = 0; iLoopVar < 245; iLoopVar ++)
        {
            WriteDspCommDram (AUD_DSP0, (ADDR_RC2D_XMX_TABLE + 16 + 64 + iLoopVar), pdwTable[iLoopVar]);
        }

        pdwTable = (UINT32 *) &xmxpanmean2chlineargaintab[0];
        for (iLoopVar = 0; iLoopVar < 42; iLoopVar ++)
        {
            WriteDspCommDram (AUD_DSP0, (ADDR_RC2D_XMX_TABLE + 16 + 64 + 245 + iLoopVar), pdwTable[iLoopVar]);
        }
#endif

#ifdef CC_AUD_DRC_V20
        //================ DRC v2.0 table ================
        // common tables
        pdwTable = (UINT32 *) &drc2_common_tab[0];
        for (iLoopVar = 0; iLoopVar < 21; iLoopVar ++)
        {
            WriteDspCommDram (AUD_DSP0, (DRAM_DRC2_COMMON_COEF_BASE + iLoopVar), pdwTable[iLoopVar]);
        }
        // 1 band AVC iir coefficients
        pdwTable = (UINT32 *) &drc2_1_band_tab[0];
        for (iLoopVar = 0; iLoopVar < 5; iLoopVar ++)
        {
            WriteDspCommDram (AUD_DSP0, (DRAM_DRC2_IIR_COEF_AVC_BASE + iLoopVar), pdwTable[iLoopVar]);
        }
        // 2 band AVC iir coefficients
        pdwTable = (UINT32 *) &drc2_2_band_tab[0];
        for (iLoopVar = 0; iLoopVar < 2*5*2; iLoopVar ++)
        {
            WriteDspCommDram (AUD_DSP0, (DRAM_DRC2_IIR_COEF_AVC_BAND0_BASE + iLoopVar), pdwTable[iLoopVar]);
        }
        // 2 band DRC iir coefficients
        pdwTable = (UINT32 *) &drc2_2_band_tab[0];
        for (iLoopVar = 0; iLoopVar < 2*5*2; iLoopVar ++)
        {
            WriteDspCommDram (AUD_DSP0, (DRAM_DRC2_IIR_COEF_DRC_BAND0_BASE + iLoopVar), pdwTable[iLoopVar]);
        }
#endif

#ifdef CC_AUD_VBASS_SUPPORT
        //================ VBASS ================
        pdwTable = (UINT32 *)&MTK_VBASS_R_TABLE[0];
        u4DramBase = dReadDspCommDram(AUD_DSP0, ADDR_RC2D_VBASS_BASE);
        for (iLoopVar = 0; iLoopVar < 70; iLoopVar++)
        {
            WriteDspCommDram (AUD_DSP0, ((u4DramBase+ADDR_VBASS_R_COEF) + iLoopVar),
                              pdwTable[iLoopVar]);
        }

        pdwTable = (UINT32 *)&MTK_VBASS_L_TABLE[0];
        for (iLoopVar = 0; iLoopVar < 70; iLoopVar++)
        {
            WriteDspCommDram (AUD_DSP0, ((u4DramBase+ADDR_VBASS_L_COEF) + iLoopVar),
                              pdwTable[iLoopVar]);
        }
        vDspVBassCutOffCoeff();

        pdwTable = (UINT32 *)&MTK_VBASS_HIGHPASS_TABLE[0];
        for (iLoopVar = 0; iLoopVar < 20; iLoopVar++)
        {
            WriteDspCommDram (AUD_DSP0, ((u4DramBase+ADDR_VBASS_HIGHPASS_COEF) + iLoopVar),
                              pdwTable[iLoopVar]);
        }
#endif
#ifndef CC_AUD_NEW_POST_MULTI_VS_SUPPORT
        //================ Vsurr Surround ================
#ifndef DSP_SUPPORT_SRSWOW
#ifndef DSP_SUPPORT_SRSTSXT
#ifndef DSP_SUPPORT_SRSTSHD //sunman_tshd
#ifndef DSP_SUPPORT_SRSWOWHD
#ifndef DSP_SUPPORT_SRSCC3D
        pdwTable = (UINT32 *)&MTK_VSURR_TABLE[0];
        for (iLoopVar = 0; iLoopVar < 20; iLoopVar++)
        {
            WriteDspCommDram (AUD_DSP0, (ADDR_VSURR_IIR_COEF + iLoopVar),
                              pdwTable[iLoopVar]);
        }

        //MTK Vsurr Crosstalk Filter Coefficient
        pdwTable = (UINT32 *)&MTK_VSURR_CROS_TABLE[0];
        for (iLoopVar = 0; iLoopVar < 15; iLoopVar++)
        {
            WriteDspCommDram (AUD_DSP0, (ADDR_VSURR_CROS_COEF + iLoopVar),
                              pdwTable[iLoopVar]);
        }


        pdwTable = (UINT32 *)&MTK_VSURR_BASS_TABLE[0];
        for (iLoopVar = 0; iLoopVar < 5; iLoopVar++)
        {
            WriteDspCommDram (AUD_DSP0, (ADDR_VSURR_BASS_COEF + iLoopVar),
                              pdwTable[iLoopVar]);
        }

        pdwTable = (UINT32 *)&MTK_VSURR_1STOP_TABLE[0];
        for (iLoopVar = 0; iLoopVar < 5; iLoopVar++)
        {
            //WriteDspCommDram ((ADDR_VSURR_1STOP_COEF + iLoopVar),pdwTable[iLoopVar]);
        }
         pdwTable = (UINT32 *)&MTK_VSURR_2STOP_TABLE[0];
        for (iLoopVar = 0; iLoopVar < 5; iLoopVar++)
        {
            //WriteDspCommDram ((ADDR_VSURR_2STOP_COEF + iLoopVar),pdwTable[iLoopVar]);
        }
#endif
#endif //DSP_SUPPORT_SRSWOWHD
#endif //DSP_SUPPORT_SRSTSHD //sunman_tshd
#endif //DSP_SUPPORT_SRSTSXT
#endif //DSP_SUPPORT_SRSWOW

#else
        vLoadVirtualSurroundTable();
#endif

#ifdef CC_AUD_SPEAKER_HEIGHT
        pdwTable = (UINT32 *)&MTK_SHELF_R_TABLE[0];
		u4DramBase = dReadDspCommDram(AUD_DSP0, ADDR_RC2D_SPKHEIGHT_BASE);
        for (iLoopVar = 0; iLoopVar < 15; iLoopVar++)
        {
            WriteDspCommDram (AUD_DSP0, (u4DramBase + ADDR_SHELF_R_COEF + iLoopVar),
                              pdwTable[iLoopVar]);
        }


        pdwTable = (UINT32 *)&MTK_SHELF_L_TABLE[0];
        for (iLoopVar = 0; iLoopVar < 15; iLoopVar++)
        {
            WriteDspCommDram (AUD_DSP0, (u4DramBase + ADDR_SHELF_L_COEF + iLoopVar),
                              pdwTable[iLoopVar]);
        }

        pdwTable = (UINT32 *)&MTK_ALLPASS1_TABLE[0];
        for (iLoopVar = 0; iLoopVar < 10; iLoopVar++)
        {
            WriteDspCommDram (AUD_DSP0, (u4DramBase + ADDR_ALLPASS1_COEF + iLoopVar),
                              pdwTable[iLoopVar]);
        }

#ifdef CC_AUD_SPEAKER_HEIGHT_MODE
        vLoadSPHTable();
#endif
#endif

#ifdef CC_AUD_FIR_SUPPORT
        vSetFIRTable (AUD_DEC_MAIN, u4FreqIdx);
#endif

#ifdef CC_AUD_HPF_SUPPORT
        // ================ Load HPF table ================
        vLoadHPFTable();
#endif

#if 1//def CC_AUD_PEQ_SUPPORT
        // ================ Load PEQ table ================

        vLoadPEQTable();
#ifdef CC_AUD_PEQ_LR
        vLoadPEQTable_R();
#endif
        pdwTable = (UINT32 *)&MTK_SPKCAL_TABLE[0];
        u4DramBase = dReadDspCommDram(AUD_DSP0, ADDR_RC2D_PEQ_BASE);
        for (iLoopVar = 0; iLoopVar < 40; iLoopVar++)
        {
            WriteDspCommDram (AUD_DSP0, (u4DramBase + ADDR_PEQ_COEF + iLoopVar),
                              pdwTable[iLoopVar]);
        }
#endif

#ifdef KARAOKE_SUPPORT
        pdwTable = (UINT32 *) & KEYSHIFT_TABLE[0];
        for (iLoopVar =0; iLoopVar < 132;iLoopVar++)
        {
            WriteDspCommDram (AUD_DSP0, (ADDR_R2CD_KEYSHIFT_TABLE + iLoopVar), pdwTable[iLoopVar]);
        }
#endif

#ifdef CC_VOCAL_MUTE
        // Set Vocal Reducer BSF Coeff
        u1Index = AUD_GetVocaleMuteFilter1();
        for(iLoopVar=0; iLoopVar<5; iLoopVar++)
        {
            WriteDspCommDram ((ADDR_RC2D_KARA_VOICE_MUTE_FILTER_COEFF+iLoopVar), VM_FILTER1[u1Index][iLoopVar]);
        } 
        // Set Vocal Reducer BPF Coeff
        u1Index = AUD_GetVocaleMuteFilter2();
        for(iLoopVar=0; iLoopVar<5; iLoopVar++)
        {
            WriteDspCommDram ((ADDR_RC2D_KARA_VOICE_MUTE_FILTER_COEFF+5+iLoopVar), VM_FILTER2[u1Index][iLoopVar]);
        }  
#endif
    }
    else // Second decoder
    {
        //================ Delay Factor ================
        WriteDspCommDram (AUD_DSP0, ADDR_RC2D_CH_DELAY_FACTOR_DEC2,
                          DELAY_FACTOR[SAMPLE_48K]);
        //Channel Time Delay
        for (iLoopVar = 0; iLoopVar < 3; iLoopVar++)
        {
            WriteDspCommDram (AUD_DSP0, (ADDR_RC2D_CH_DELAY_C_DEC2 + iLoopVar),
                              u2ReadDspShmWORD (AUD_DSP0, (W_CHDELAY_C + (iLoopVar * 2))));
        }
    }
}

#ifdef CC_AUD_BBE_SUPPORT
void vSetBbeTable(UINT8 u1DecId)
{
    UINT32 iLoopVar;
    UINT32 *pdwTable;
    UINT32 u4DramAdrIdx;

    if (u1DecId == AUD_DEC_MAIN)
    {
        // BBE VIVA 4 bbe_f Coefficients
        pdwTable = (UINT32 *)&BBEV4_BBE_F_TABLE[0];
        u4DramAdrIdx = dReadDspCommDram(ADDR_RC2D_BBE_BASE) + ADDR_BBEV4_BBE_F;
        WriteDspCommDram (u4DramAdrIdx, pdwTable[0]);

        //BBE VIVA 4 Surround Coefficient
        pdwTable = (UINT32 *)&BBEV4_IIR_COEF_TABLE[0];
        u4DramAdrIdx = dReadDspCommDram(ADDR_RC2D_BBE_BASE) + ADDR_BBEV4_IIR_COEF;
        for (iLoopVar = 0; iLoopVar < 5; iLoopVar++)
        {
            WriteDspCommDram ((u4DramAdrIdx + iLoopVar), pdwTable[iLoopVar]);
        }
    }
}
#endif // CC_AUD_BBE_SUPPORT

#ifdef CC_AUD_DSP_SUPPORT_AUDYSSEY//ABX_SUPPORT
void vSetABXTable(void)
{
    UINT32 i;

    // Setup ABX enable bit
    WriteDspCommDram (ADDR_RC2D_AUDYSSEY_FLAG,
                ((dReadDspCommDram(ADDR_RC2D_AUDYSSEY_FLAG)) & ~ADDR_RC2D_ABX_ENABLE_BIT) |
                ((uReadDspShmBYTE(B_ABX_ENABLE) > 0) ? ADDR_RC2D_ABX_ENABLE_BIT : 0));

    //Default write 48KHz coef.
    for (i=0;i<98;i++)
    {
        WriteDspCommDram (ADDR_RC2D_ABX_IIR_COEF_ADDR_START+i, ABX_COEF_TABLE[uReadDspShmBYTE(B_ABX_FILTER_SET)][i]);
    }

    WriteDspCommDram (ADDR_RC2D_ABX_IIR_COEF_ADDR_START+98, ABX_GAIN_TABLE[uReadDspShmBYTE(B_ABX_DRY_GAIN_INDX)]);
    WriteDspCommDram (ADDR_RC2D_ABX_IIR_COEF_ADDR_START+99, ABX_GAIN_TABLE[uReadDspShmBYTE(B_ABX_WET_GAIN_INDX)]);
}
#endif

#ifdef CC_AUD_DSP_SUPPORT_CDNOTCH
void vSetCDNotchTable(void)
{
    UINT32 *pu32Table;
    UINT32 u32DramAdr;
    UINT8 u1QIdx;

	u32DramAdr = dReadDspCommDram(AUD_DSP0, ADDR_RC2D_CDNOTCH_BASE);
	u1QIdx = uReadDspShmBYTE(AUD_DSP0, B_CDNOTCH_USER_Q);

    // CDNotch alpha_halfH table
    pu32Table = (UINT32 *)&CDNOTCH_ALPHA_HALFH_TABLE[(u1QIdx<<1)];
    WriteDspCommDram(AUD_DSP0, (u32DramAdr+ADDR_CDNOTCH_ALPHA_IDX), pu32Table[0]);
    WriteDspCommDram(AUD_DSP0, (u32DramAdr+ADDR_CDNOTCH_HALF_H_IDX), pu32Table[1]);
    LOG(7, "set CDNotch table done\n");
}
#endif

#ifdef DSP_SUPPORT_SRS_SSHD
void vSetSSHDTable (UINT32 u4FreqIdx, BOOL fgFirstDecoder){

    WriteDspCommDram (ADDR_SRS_FLAG, u2ReadDspShmWORD (W_SRS_FLAG));
}
#endif
#ifdef DSP_SUPPORT_SRS_PURESOUND
void vSetPuresoundTable (UINT32 u4FreqIdx, BOOL fgFirstDecoder){

    WriteDspCommDram (ADDR_SRS_FLAG, u2ReadDspShmWORD (W_SRS_FLAG));
}
#endif
#ifdef DSP_SUPPORT_SRS_SS3D
void vSetSS3DTable (UINT32 u4FreqIdx, BOOL fgFirstDecoder){

    WriteDspCommDram (ADDR_SRS_FLAG, u2ReadDspShmWORD (W_SRS_FLAG));
    //Printf("fubofeng ADDR_SRS_FLAG 0x%x \n",dReadDspCommDram32(ADDR_SRS_FLAG)); 
}
#endif

#if defined(DSP_SUPPORT_SRS_PEQ)
void vSetPeqTable (UINT32 u4FreqIdx, BOOL fgFirstDecoder){//sunman_peq

    UINT32 i, idx, u4Ptr;
    UINT32 u32DramAdr;
    static INT32  srs_peq_hpf_coef[3][40+10+10+10];
    u4FreqIdx=SAMPLE_48K;

    VERIFY(DRVCUST_OptQuery(eAudioSRSPeqHpfFactory, (UINT32 *) &u4Ptr)==0);
    x_memcpy((VOID *)srs_peq_hpf_coef, (const VOID *)u4Ptr, sizeof(srs_peq_hpf_coef));

    u32DramAdr = dReadDspCommDram(ADDR_RC2D_SRS_PEQ_BASE);
    WriteDspCommDram (u32DramAdr+ADDR_SRS_PEQ_INPUT_GAIN, u4ReadDspShmDWRD (D_SRS_PEQ_INPUT_GAIN));
    WriteDspCommDram (u32DramAdr+ADDR_SRS_PEQ_OUTPUT_GAIN, u4ReadDspShmDWRD (D_SRS_PEQ_OUTPUT_GAIN));
    WriteDspCommDram (u32DramAdr+ADDR_SRS_PEQ_BANDS_1, u2ReadDspShmWORD (W_SRS_PEQ_BANDS_1));

    // fixed data


    idx = 0;
    for (i=0;i<40;i++)
    {
        WriteDspCommDram (u32DramAdr+ADDR_SRS_PEQ_COEF + i,  srs_peq_hpf_coef[u4FreqIdx][idx++]);
        //printf("write peq %x -> %x\n",u32DramAdr+ADDR_SRS_PEQ_COEF + i,srs_peq_hpf_coef[u4FreqIdx][idx-1] );
    }
    for (i=0;i<10;i++)
    {
        WriteDspCommDram (u32DramAdr+ADDR_SRS_HPF_COEF + i,  srs_peq_hpf_coef[u4FreqIdx][idx++]);
       //printf("write 1st hpf %x -> %x\n",u32DramAdr+ADDR_SRS_HPF_COEF + i,srs_peq_hpf_coef[u4FreqIdx][idx-1] );
    }
    for (i=0;i<10;i++)
    {
        WriteDspCommDram (u32DramAdr+ADDR_SRS_HPF2_COEF + i,  srs_peq_hpf_coef[u4FreqIdx][idx++]);
       //printf("write 1st hpf %x -> %x\n",u32DramAdr+ADDR_SRS_HPF_COEF + i,srs_peq_hpf_coef[u4FreqIdx][idx-1] );
    }
    for (i=0;i<10;i++)
    {
        WriteDspCommDram (u32DramAdr+ADDR_SRS_PEQ_HPF_COEF + i,  srs_peq_hpf_coef[u4FreqIdx][idx++]);
        //printf("write 2nd hpf %x -> %x\n",u32DramAdr+ADDR_SRS_PEQ_HPF_COEF + i,srs_peq_hpf_coef[u4FreqIdx][idx-1] );
    }

}

#endif
#if defined(DSP_SUPPORT_SRS_HPF)

void vSetSrsHpfTable (UINT32 u4FreqIdx, BOOL fgFirstDecoder){//sunman_peq

    UINT32 i;

    WriteDspCommDram32bit (ADDR_SRS_HPF_IIR_SECTION,  srs_hpf[0]<<16);

    WriteDspCommDram32bit (ADDR_SRS_HPF_IIR_SECTION1_IWL,  srs_hpf[1]<<16);
    for (i=2;i<(2+5);i++)
    {
        WriteDspCommDram32bit (ADDR_SRS_HPF_IIR_COEF + i - 2,  srs_hpf[i]);
    }
    WriteDspCommDram32bit (ADDR_SRS_HPF_IIR_SECTION2_IWL,  srs_hpf[7]<<16);
    for (i=8;i<(8+5);i++)
    {
        WriteDspCommDram32bit (ADDR_SRS_HPF_IIR_COEF + i - 8 + 5,  srs_hpf[i]);
    }
    WriteDspCommDram32bit (ADDR_SRS_HPF_IIR_SECTION3_IWL,  srs_hpf[13]<<16);
    for (i=14;i<(14+5);i++)
    {
        WriteDspCommDram32bit (ADDR_SRS_HPF_IIR_COEF + i - 14 + 10,  srs_hpf[i]);
    }
    WriteDspCommDram32bit (ADDR_SRS_HPF_IIR_SECTION4_IWL,  srs_hpf[19]<<16);
    for (i=20;i<(20+5);i++)
    {
        WriteDspCommDram32bit (ADDR_SRS_HPF_IIR_COEF + i - 20 + 15,  srs_hpf[i]);
    }
    WriteDspCommDram32bit (ADDR_SRS_HPF_IIR_SECTION5_IWL,  srs_hpf[25]<<16);
    for (i=26;i<(26+5);i++)
    {
        WriteDspCommDram32bit (ADDR_SRS_HPF_IIR_COEF + i - 26 + 20,  srs_hpf[i]);
    }
    WriteDspCommDram32bit (ADDR_SRS_HPF_IIR_SECTION6_IWL,  srs_hpf[31]<<16);
    for (i=32;i<(32+5);i++)
    {
        WriteDspCommDram32bit (ADDR_SRS_HPF_IIR_COEF + i - 32 + 25,  srs_hpf[i]);
    }
    WriteDspCommDram32bit (ADDR_SRS_HPF_IIR_LASTGAIN,  srs_hpf[38]);
    WriteDspCommDram32bit (ADDR_SRS_HPF_IIR_LASTGAIN_IWL,  srs_hpf[37]<<16);


    WriteDspCommDram32bit (ADDR_SRS_HPF2_IIR_SECTION,  srs_hpf[0 + 39]<<16);

    WriteDspCommDram32bit (ADDR_SRS_HPF2_IIR_SECTION1_IWL,  srs_hpf[1 + 39]<<16);
    for (i=2;i<(2+5);i++)
    {
        WriteDspCommDram32bit (ADDR_SRS_HPF2_IIR_COEF + i - 2,  srs_hpf[i + 39]);
    }
    WriteDspCommDram32bit (ADDR_SRS_HPF2_IIR_SECTION2_IWL,  srs_hpf[7 + 39]<<16);
    for (i=8;i<(8+5);i++)
    {
        WriteDspCommDram32bit (ADDR_SRS_HPF2_IIR_COEF + i - 8 + 5,  srs_hpf[i + 39]);
    }
    WriteDspCommDram32bit (ADDR_SRS_HPF2_IIR_SECTION3_IWL,  srs_hpf[13 + 39]<<16);
    for (i=14;i<(14+5);i++)
    {
        WriteDspCommDram32bit (ADDR_SRS_HPF2_IIR_COEF + i - 14 + 10,  srs_hpf[i + 39]);
    }
    WriteDspCommDram32bit (ADDR_SRS_HPF2_IIR_SECTION4_IWL,  srs_hpf[19 + 39]<<16);
    for (i=20;i<(20+5);i++)
    {
        WriteDspCommDram32bit (ADDR_SRS_HPF2_IIR_COEF + i - 20 + 15,  srs_hpf[i + 39]);
    }
    WriteDspCommDram32bit (ADDR_SRS_HPF2_IIR_SECTION5_IWL,  srs_hpf[25 + 39]<<16);
    for (i=26;i<(26+5);i++)
    {
        WriteDspCommDram32bit (ADDR_SRS_HPF2_IIR_COEF + i - 26 + 20,  srs_hpf[i + 39]);
    }
    WriteDspCommDram32bit (ADDR_SRS_HPF2_IIR_SECTION6_IWL,  srs_hpf[31 + 39]<<16);
    for (i=32;i<(32+5);i++)
    {
        WriteDspCommDram32bit (ADDR_SRS_HPF2_IIR_COEF + i - 32 + 25,  srs_hpf[i + 39]);
    }

    WriteDspCommDram32bit (ADDR_SRS_HPF2_IIR_LASTGAIN,  srs_hpf[38 + 39]);
    WriteDspCommDram32bit (ADDR_SRS_HPF2_IIR_LASTGAIN_IWL,  srs_hpf[37 + 39]<<16);

}

#endif

#ifdef DSP_SUPPORT_SRS_LMT
static UINT8 useCustom1 = 0;
void vSetLmtTable (UINT32 u4FreqIdx, BOOL fgFirstDecoder){//sunman_peq

    UINT32 i;
    UINT32 u32DramAdr;

    // fixed data
    if(useCustom1==0)
    {
        useCustom1 = 1;
        //sunman_lmt
        u32DramAdr = dReadDspCommDram(ADDR_RC2D_SRS_HL_BASE);
        for (i=0;i<(256);i++)
        {
            WriteDspCommDram32bit (u32DramAdr+ADDR_SRS_LMT_TABLE + i,  srs_lmt[i]);
        }
        for (i=0;i<(256);i++)
        {
            WriteDspCommDram (u32DramAdr+ADDR_SRS_LMT_LZTBL + i,  srs_lmt_lzTbl[i]<<8);
        }

    }
    u32DramAdr = dReadDspCommDram(ADDR_RC2D_SRS_HL_BASE);
    //WriteDspCommDram32bit (u32DramAdr+ADDR_SRS_LMT_BACKUP+0, 0);           //leftInOut
    //WriteDspCommDram32bit (u32DramAdr+ADDR_SRS_LMT_BACKUP+1, 0);           //rightInOut
    //WriteDspCommDram32bit (u32DramAdr+ADDR_SRS_LMT_BACKUP+2, 0);           //input_dlybuf_l_48_ptr
    //WriteDspCommDram32bit (u32DramAdr+ADDR_SRS_LMT_BACKUP+3, 0);           //input_dlybuf_r_48_ptr
    //WriteDspCommDram32bit (u32DramAdr+ADDR_SRS_LMT_BACKUP+4, 0);           //temp_buffer_304_ptr
    WriteDspCommDram32bit (u32DramAdr+ADDR_SRS_LMT_BACKUP+5, u4ReadDspShmDWRD (D_SRS_LMT_INPUT_GAIN));      //iputgain
    WriteDspCommDram32bit (u32DramAdr+ADDR_SRS_LMT_BACKUP+6, u4ReadDspShmDWRD (D_SRS_LMT_OUTPUT_GAIN));     //outputgain
    WriteDspCommDram32bit (u32DramAdr+ADDR_SRS_LMT_BACKUP+7, u4ReadDspShmDWRD (D_SRS_LMT_DELAY));           //Delaylen
    WriteDspCommDram32bit (u32DramAdr+ADDR_SRS_LMT_BACKUP+8, u4ReadDspShmDWRD (D_SRS_LMT_BOOST));           //Limiterboost
    WriteDspCommDram32bit (u32DramAdr+ADDR_SRS_LMT_BACKUP+9, u4ReadDspShmDWRD (D_SRS_LMT_LEVEL));           //Hardlimit
    WriteDspCommDram32bit (u32DramAdr+ADDR_SRS_LMT_BACKUP+10, u4ReadDspShmDWRD (D_SRS_LMT_COEFF));          //LimiterCoeff
    WriteDspCommDram32bit (u32DramAdr+ADDR_SRS_LMT_BACKUP+11, u4ReadDspShmDWRD (D_SRS_LMT_THRESH));          //HLThresh
    WriteDspCommDram32bit (u32DramAdr+ADDR_SRS_LMT_BACKUP+12, 1);          //DecaySmoothEn
    WriteDspCommDram32bit (u32DramAdr+ADDR_SRS_LMT_BACKUP+13, 0);          //MaxValL
    WriteDspCommDram32bit (u32DramAdr+ADDR_SRS_LMT_BACKUP+14, 0);          //MaxValR
    WriteDspCommDram32bit (u32DramAdr+ADDR_SRS_LMT_BACKUP+15, 0x04000000);          //PrevLastGainL
    WriteDspCommDram32bit (u32DramAdr+ADDR_SRS_LMT_BACKUP+16, 0x04000000);          //PrevLastGainR
    WriteDspCommDram32bit (u32DramAdr+ADDR_SRS_LMT_BACKUP+17, 0);          //SmoothedGainL
    WriteDspCommDram32bit (u32DramAdr+ADDR_SRS_LMT_BACKUP+18, 0);          //SmoothedGainR
    WriteDspCommDram32bit (u32DramAdr+ADDR_SRS_LMT_BACKUP+19, 0);          //CBufIndex
    WriteDspCommDram32bit (u32DramAdr+ADDR_SRS_LMT_BACKUP+20, 0x04000000);          //TheRatio
    WriteDspCommDram32bit (u32DramAdr+ADDR_SRS_LMT_BACKUP+21, 0);          //LookAhead0
    WriteDspCommDram32bit (u32DramAdr+ADDR_SRS_LMT_BACKUP+22, 0);          //LookAhead1
    WriteDspCommDram32bit (u32DramAdr+ADDR_SRS_LMT_BACKUP+23, 0);          //LAIndex0
    WriteDspCommDram32bit (u32DramAdr+ADDR_SRS_LMT_BACKUP+24, 0);          //LAIndex1
    WriteDspCommDram32bit (u32DramAdr+ADDR_SRS_LMT_BACKUP+25, 0x03d70000);  //K_const0_03f_1        SRS_FXP16(0.03f, 1)
    WriteDspCommDram32bit (u32DramAdr+ADDR_SRS_LMT_BACKUP+26, 0x7fffffff);  //K_const1_0f           0x7fffffff
    WriteDspCommDram32bit (u32DramAdr+ADDR_SRS_LMT_BACKUP+27, 0x4147ae00);  //K_const1_02f_2       SRS_FXP32(1.02f, 2)
    WriteDspCommDram32bit (u32DramAdr+ADDR_SRS_LMT_BACKUP+28, 0x43330000);  //K_const1_05f_2       SRS_FXP16(1.05, 2)
    WriteDspCommDram32bit (u32DramAdr+ADDR_SRS_LMT_BACKUP+29, 0x79e79e7a);  //K_constrec1_05f_1    SRS_FXP32(1.0/1.05, 1)
    WriteDspCommDram32bit (u32DramAdr+ADDR_SRS_LMT_BACKUP+30, 0x7fffffff);  //K_const1_0f_1        0x7FFFFFFF
    WriteDspCommDram32bit (u32DramAdr+ADDR_SRS_LMT_BACKUP+31, 0x20000000);  //K_const1_0f_3        SRS_FXP32(1.0f,3)
    WriteDspCommDram32bit (u32DramAdr+ADDR_SRS_LMT_BACKUP+32, 0x04000000);  //K_const1_0f_6        SRS_FXP32(1.0, SRS_HDLMT_BOOST_IWL)
    WriteDspCommDram32bit (u32DramAdr+ADDR_SRS_LMT_BACKUP+33, 0x7c290000);  //K_const0_97f_1         SRS_FXP16(0.97f, 1)
    WriteDspCommDram32bit (u32DramAdr+ADDR_SRS_LMT_BACKUP+34, 0x7eb80000);  //K_const0_99f_1       SRS_FXP16(0.99, 1)
    WriteDspCommDram32bit (u32DramAdr+ADDR_SRS_LMT_BACKUP+35, 0x0147ae14);  //K_const0_01f_1       SRS_FXP32(0.01, 1)
    WriteDspCommDram32bit (u32DramAdr+ADDR_SRS_LMT_BACKUP+36, 0x0010624e);  //kSRSHLLimiterCoeff   kSRSHLLimiterCoeff   SRS_FXP32(5.0e-4, 1)
    WriteDspCommDram32bit (u32DramAdr+ADDR_SRS_LMT_BACKUP+37, 0x7fc50480);  //kSRSHLLookAheadCoef   kSRSHLLookAheadCoef  SRS_FXP32(0.9982f, 1)
    WriteDspCommDram32bit (u32DramAdr+ADDR_SRS_LMT_BACKUP+38, 0x7e147b00);  //kSRSHLRefConst       kSRSHLRefConst  SRS_FXP32(0.985, 1)
    WriteDspCommDram32bit (u32DramAdr+ADDR_SRS_LMT_BACKUP+39, 0x04444444);  //K_recimaxamount      SRS_FXP32(1.0/kMaxHistoryAmount, 1)
    for (i=0;i<(40);i++)
    {
        WriteDspCommDram32bit (u32DramAdr+ADDR_SRS_LMT_BACKUP + 40 + i,  0); //MaxHistory
    }


}
#endif

#ifdef DSP_SUPPORT_SRS_AEQ

void vSetAeqTable (UINT32 u4FreqIdx, BOOL fgFirstDecoder){//AEQ

    UINT32 i;
    UINT32 u32DramAdr;
    UINT32 u32AeqSize;

    //bofeng_aeq
    u32DramAdr = dReadDspCommDram(ADDR_RC2D_SRS_AEQ_BASE);
    WriteDspCommDram32bit (u32DramAdr+ADDR_SRS_AEQ_INPUT_GAIN, u4ReadDspShmDWRD (D_SRS_AEQ_INPUT_GAIN));
    WriteDspCommDram32bit (u32DramAdr+ADDR_SRS_AEQ_OUTPUT_GAIN, u4ReadDspShmDWRD (D_SRS_AEQ_OUTPUT_GAIN));
    u32AeqSize = u4ReadDspShmDWRD (D_SRS_AEQ_FILTER_SIZE);
    u32AeqSize = 0;
    if(u32AeqSize == 0)
    {
        WriteDspCommDram32bit (u32DramAdr+ADDR_SRS_AEQ_FIR_ORDER, (srs_aeq[0] << 16));
        WriteDspCommDram32bit (u32DramAdr+ADDR_SRS_AEQ_FIR_ORDER_IWL, srs_aeq[1] << 16);
        //aeq fir
        for (i=2;i<(2+64);i++)
        {
            WriteDspCommDram32bit (u32DramAdr+ADDR_SRS_AEQ_FIR_FILTER + i - 2,  srs_aeq[i]);
        }
        WriteDspCommDram32bit (u32DramAdr+ADDR_SRS_AEQ_FIR_LASTGAIN_IWL, srs_aeq[66] << 16);
        WriteDspCommDram32bit (u32DramAdr+ADDR_SRS_AEQ_FIR_LASTGAIN, srs_aeq[67]);

        WriteDspCommDram32bit (u32DramAdr+ADDR_SRS_AEQ_IIR_SECTION, srs_aeq[68] << 16);//4
        WriteDspCommDram32bit (u32DramAdr+ADDR_SRS_AEQ_IIR_SECTION1_IWL, srs_aeq[69] << 16);//4
        for (i=70;i<(70+5);i++)
        {
            WriteDspCommDram32bit (u32DramAdr+ADDR_SRS_AEQ_IIR_FILTER + i - 70,  srs_aeq[i]);
        }
        WriteDspCommDram32bit (u32DramAdr+ADDR_SRS_AEQ_IIR_SECTION2_IWL, srs_aeq[75] << 16);//4
        for (i=76;i<(76+5);i++)
        {
            WriteDspCommDram32bit (u32DramAdr+ADDR_SRS_AEQ_IIR_FILTER + i - 76 + 5,  srs_aeq[i]);
        }
        WriteDspCommDram32bit (u32DramAdr+ADDR_SRS_AEQ_IIR_SECTION3_IWL, srs_aeq[81] << 16);//4
        for (i=82;i<(82+5);i++)
        {
            WriteDspCommDram32bit (u32DramAdr+ADDR_SRS_AEQ_IIR_FILTER + i - 82 + 10,  srs_aeq[i]);
        }
        WriteDspCommDram32bit (u32DramAdr+ADDR_SRS_AEQ_IIR_SECTION4_IWL, srs_aeq[87] << 16);//4
        for (i=88;i<(88+5);i++)
        {
            WriteDspCommDram32bit (u32DramAdr+ADDR_SRS_AEQ_IIR_FILTER + i - 88 + 15,  srs_aeq[i]);
        }
        WriteDspCommDram32bit (u32DramAdr+ADDR_SRS_AEQ_IIR_SECTION5_IWL, srs_aeq[93] << 16);//4
        for (i=94;i<(94+5);i++)
        {
            WriteDspCommDram32bit (u32DramAdr+ADDR_SRS_AEQ_IIR_FILTER + i - 94 + 20,  srs_aeq[i]);
        }
        WriteDspCommDram32bit (u32DramAdr+ADDR_SRS_AEQ_IIR_SECTION6_IWL, srs_aeq[99] << 16);//4
        for (i=100;i<(100+5);i++)
        {
            WriteDspCommDram32bit (u32DramAdr+ADDR_SRS_AEQ_IIR_FILTER + i - 100 + 25,  srs_aeq[i]);
        }
        WriteDspCommDram32bit (u32DramAdr+ADDR_SRS_AEQ_IIR_SECTION7_IWL, srs_aeq[105] << 16);//4
        for (i=106;i<(106+5);i++)
        {
            WriteDspCommDram32bit (u32DramAdr+ADDR_SRS_AEQ_IIR_FILTER + i - 106 + 30,  srs_aeq[i]);
        }
        WriteDspCommDram32bit (u32DramAdr+ADDR_SRS_AEQ_IIR_SECTION8_IWL, srs_aeq[111] << 16);//4
        for (i=112;i<(112+5);i++)
        {
            WriteDspCommDram32bit (u32DramAdr+ADDR_SRS_AEQ_IIR_FILTER + i - 112 + 35,  srs_aeq[i]);
        }
        WriteDspCommDram32bit (u32DramAdr+ADDR_SRS_AEQ_IIR_LASTGAIN_IWL, srs_aeq[117] << 16);
        WriteDspCommDram32bit (u32DramAdr+ADDR_SRS_AEQ_IIR_LASTGAIN, srs_aeq[118]);


    }




}
#endif


#ifdef DSP_SUPPORT_TCL_VBASS

void vSetTclVbass (UINT8 u1DecId)
{

    UINT32 u4TblDstAdr;
    UINT32 *u4TblSrcAdr;
	UINT32 iLoopVar;
	UINT32 u4a0_peak;	      
	UINT32 u4a1_peak;	      
	UINT32 u4a2_peak;	      
	UINT32 u4b0_peak;	      
	UINT32 u4b1_peak;	      
	UINT32 u4b2_peak;	      
	UINT32 u4at;	          
	UINT32 u4rt;	          
	UINT32 u4a_peak;	      
	UINT32 u4gain_hamo;	    
	UINT32 u4g_out;  	      
	UINT32 u4gain_back;	    
	UINT32 u4gain_out;	    
	UINT32 u4gainhamonicout;
	UINT32 u4coeff_ham;	    
                          
	UINT32 u4x1peak;	      
	UINT32 u4x2peak;	      
	UINT32 u4c1peak;	      
	UINT32 u4c2peak;	      
	UINT32 u4coutpeak;	    
	UINT32 u4y_out;	     

	if (u1DecId != AUD_DEC_MAIN)
	{
	    printf("error decode ID\n");
		return;
	}

	u4a0_peak	        = u4ReadDspShmDWRD(D_TCL_AO_PEAK);	
	u4a1_peak	        = u4ReadDspShmDWRD(D_TCL_A1_PEAK);	
	u4a2_peak	        = u4ReadDspShmDWRD(D_TCL_A2_PEAK);	
	u4b0_peak	        = u4ReadDspShmDWRD(D_TCL_BO_PEAK);	
	u4b1_peak	        = u4ReadDspShmDWRD(D_TCL_B1_PEAK);	
	u4b2_peak	        = u4ReadDspShmDWRD(D_TCL_B2_PEAK);	
	u4at	            = u4ReadDspShmDWRD(D_TCL_AT);	
	u4rt	            = u4ReadDspShmDWRD(D_TCL_RT);	
	u4a_peak	        = u4ReadDspShmDWRD(D_TCL_A_PEAK);	
	u4gain_hamo	        = u4ReadDspShmDWRD(D_TCL_GAIN_HAMONICGEN);	
	u4g_out  	        = u4ReadDspShmDWRD(D_TCL_G_OUT);
	u4gain_back	        = u4ReadDspShmDWRD(D_TCL_GAIN_BACK);	
	u4gain_out	        = u4ReadDspShmDWRD(D_TCL_GAIN_OUT);	
	u4gainhamonicout	= u4ReadDspShmDWRD(D_TCL_GAINHAMONICOUT);	
	u4coeff_ham	        = u4ReadDspShmDWRD(D_TCL_COEFF_HAM);

	u4x1peak	        = u4ReadDspShmDWRD(D_TCL_X1PEAK);	
	u4x2peak	        = u4ReadDspShmDWRD(D_TCL_X2PEAK);
	u4c1peak	        = u4ReadDspShmDWRD(D_TCL_C1PEAK);	
	u4c2peak	        = u4ReadDspShmDWRD(D_TCL_C2PEAK);	
	u4coutpeak	        = u4ReadDspShmDWRD(D_TCL_COUT_PEAK);	
	u4y_out	            = u4ReadDspShmDWRD(D_TCL_Y_OUT);

    printf("u4a0_peak %d\n", u4a0_peak);
    WriteDspCommDram32bit (ADDR_TCL_VBASS_DATA + 4,  u4a0_peak	      );  //tvb_a0_peak    
  	WriteDspCommDram32bit (ADDR_TCL_VBASS_DATA + 5,  u4a1_peak	      );  //tvb_a1_peak    
  	WriteDspCommDram32bit (ADDR_TCL_VBASS_DATA + 6,  u4a2_peak	      );  //tvb_a2_peak    
  	WriteDspCommDram32bit (ADDR_TCL_VBASS_DATA + 7,  u4b0_peak	      );  //tvb_b0_peak    
  	WriteDspCommDram32bit (ADDR_TCL_VBASS_DATA + 8,  u4b1_peak	      );  //tvb_b1_peak    
  	WriteDspCommDram32bit (ADDR_TCL_VBASS_DATA + 9,  u4b2_peak	      );  //tvb_b2_peak    
  	WriteDspCommDram32bit (ADDR_TCL_VBASS_DATA + 10, u4at	            );  //tvb_AT         
  	WriteDspCommDram32bit (ADDR_TCL_VBASS_DATA + 11, u4rt	            );  //tvb_RT         
  	WriteDspCommDram32bit (ADDR_TCL_VBASS_DATA + 12, u4a_peak	        );  //tvb_A_PEAK     
  	WriteDspCommDram32bit (ADDR_TCL_VBASS_DATA + 13, u4gain_hamo	    );  //tvb_GAIN_HAM   
  	WriteDspCommDram32bit (ADDR_TCL_VBASS_DATA + 14, u4g_out  	      );  //tvb_G_OUT      
  	WriteDspCommDram32bit (ADDR_TCL_VBASS_DATA + 15, u4gain_back	    );  //tvb_GAIN_BACK  
  	WriteDspCommDram32bit (ADDR_TCL_VBASS_DATA + 16, u4gain_out	      );  //tvb_GAIN_OUT   
  	WriteDspCommDram32bit (ADDR_TCL_VBASS_DATA + 17, u4gainhamonicout );  //tvb_GAIN_HAMOUT
  	WriteDspCommDram32bit (ADDR_TCL_VBASS_DATA + 18, u4coeff_ham	    );  //tvb_coeff_ham  
  	WriteDspCommDram32bit (ADDR_TCL_VBASS_DATA + 19, u4x1peak	        );  //tvb_xpeak1     
  	WriteDspCommDram32bit (ADDR_TCL_VBASS_DATA + 20, u4x2peak	        );  //tvb_xpeak2     
  	WriteDspCommDram32bit (ADDR_TCL_VBASS_DATA + 21, u4c1peak	        );  //tvb_c1_peak    
  	WriteDspCommDram32bit (ADDR_TCL_VBASS_DATA + 22, u4c2peak	        );  //tvb_c2_peak    
  	WriteDspCommDram32bit (ADDR_TCL_VBASS_DATA + 23, u4coutpeak	      );  //tvb_cout_peak  
  	WriteDspCommDram32bit (ADDR_TCL_VBASS_DATA + 24, u4y_out	        );  //tvb_y_out2
  	WriteDspCommDram32bit (ADDR_TCL_VBASS_DATA + 25, 0x7fffffff	        );  //tvb_const
    printf("u4a0_peak %d\n", dReadDspCommDram32(ADDR_TCL_VBASS_DATA + 4));

    /**** LPF ****/
	u4TblSrcAdr = (UINT32 *)&TCL_VBASS_TBL[0];
	u4TblDstAdr  = ADDR_TCL_VBASS_LPF_COEF;
	for (iLoopVar = 0; iLoopVar < 10; iLoopVar++)
	{
		WriteDspCommDram32bit (u4TblDstAdr+iLoopVar, u4TblSrcAdr[iLoopVar]);
	}
	u4TblSrcAdr += 10;
	u4TblDstAdr  = ADDR_TCL_VBASS_LPF_GAIN;
	WriteDspCommDram32bit (u4TblDstAdr, u4TblSrcAdr[0]);

	/**** HPF ****/
	u4TblSrcAdr += 1;
	u4TblDstAdr  = ADDR_TCL_VBASS_HPF_COEF;
	for (iLoopVar = 0; iLoopVar < 15; iLoopVar++)
	{
		WriteDspCommDram32bit (u4TblDstAdr+iLoopVar, u4TblSrcAdr[iLoopVar]);
	}
	u4TblSrcAdr += 15;
    u4TblDstAdr  = ADDR_TCL_VBASS_HPF_GAIN;
    WriteDspCommDram32bit (u4TblDstAdr, u4TblSrcAdr[0]);

    /**** FB ****/
	u4TblSrcAdr += 1;
	u4TblDstAdr  = ADDR_TCL_VBASS_FB_COEF;
	for (iLoopVar = 0; iLoopVar < 10; iLoopVar++)
	{
		WriteDspCommDram32bit (u4TblDstAdr+iLoopVar, u4TblSrcAdr[iLoopVar]);
	}
	u4TblSrcAdr += 10;
    u4TblDstAdr  = ADDR_TCL_VBASS_FB_GAIN;
    WriteDspCommDram32bit (u4TblDstAdr, u4TblSrcAdr[0]);

    /**** FOUT ****/
	u4TblSrcAdr += 1;
	u4TblDstAdr  = ADDR_TCL_VBASS_FOUT_COEF;
	for (iLoopVar = 0; iLoopVar < 15; iLoopVar++)
	{
		WriteDspCommDram32bit (u4TblDstAdr+iLoopVar, u4TblSrcAdr[iLoopVar]);
	}
	u4TblSrcAdr += 15;
    u4TblDstAdr  = ADDR_TCL_VBASS_FOUT_GAIN;
    WriteDspCommDram32bit (u4TblDstAdr, u4TblSrcAdr[0]);

	printf("u4a0_peak %d\n", dReadDspCommDram32(ADDR_TCL_VBASS_FOUT_GAIN));
	
}

#endif


#ifdef DSP_SUPPORT_SRSVIQ
//sunman_viq
void vSetViqTable (UINT32 u4FreqIdx, BOOL fgFirstDecoder)
{
    UINT32 dwOfst;
    UINT32 dwSzConst, dwSzSamp;

    UINT32 dwTabAddr = CMPT_VIQ2_BUFFER * 4;  // Byte address
    /*UINT32 dwDspBuf;*/
    UINT32 dwDspCmptBuf;
    UINT32 dwFlashOfst;
    UINT32 dwTmp;

    u4FreqIdx=SAMPLE_48K;
    // copy Table to DRAM from flash
    //dwDspCmptBuf = dwGetDSPCSADR (TYPE_AUD_CMM);
    dwDspCmptBuf = dwGetDSPNSADR (TYPE_AUD_CMM);
    dwFlashOfst = u4GetDspImgAddr ();

    dwSzConst = 470;    //sunman 472 DWORD independent with sampling rate   //sunman_viq
    dwSzSamp = 167 ;
    dwOfst = u4GetFlashData (dwFlashOfst + (DSPF_VIQ_TBL_SA << 2));
    dwOfst = dwDspCalOfst (dwOfst);
    //dwSzAll = u4GetFlashData (dwFlashOfst + (DSPF_VIQ_TBL_SZ << 2));
    // ASSERT(dwSzAll != (dwSzConst+dwSzSamp*3));
    vFlash2DramDma (dwDspCmptBuf + dwTabAddr, dwOfst, dwSzConst * 4);

    u4FreqIdx = SAMPLE_48K;
    dwOfst = dwOfst + dwSzConst*4 + u4FreqIdx*dwSzSamp*4;   //sunman 96 DWORD data dependent with sampling rate
    vFlash2DramDma (dwDspCmptBuf + dwTabAddr + dwSzConst * 4, dwOfst, dwSzSamp * 4);

     WriteDspCommDram (ADDR_RC2D_AVC_REFERENCE_LEVEL, u4ReadDspShmDWRD (D_VIQ_REF_LVL));
     WriteDspCommDram (ADDR_RC2D_AVC_MODE, u4ReadDspShmDWRD (D_VIQ_MODE));
     WriteDspCommDram (ADDR_RC2D_AVC_MAX_GAIN, u4ReadDspShmDWRD (D_VIQ_MAX_GAIN));
     WriteDspCommDram (ADDR_RC2D_AVC_INPUT_GAIN, u4ReadDspShmDWRD (D_VIQ_INPUT_GAIN));
     WriteDspCommDram (ADDR_RC2D_AVC_OUTPUT_GAIN, u4ReadDspShmDWRD (D_VIQ_OUTPUT_GAIN));
     WriteDspCommDram (ADDR_RC2D_AVC_NOISE_TH, u4ReadDspShmDWRD (D_VIQ_NOISE_TH));

    dwTmp = dReadDspCommDram(ADDR_RC2D_AVC_MODE);
    switch( (dwTmp & 0x3000)>>12 )
    {
    case 0://Light
        //printf("\n\n L dwCoefPnt=%x, dwFacPnt=%x\n\n ", ADDR_SRS_VIQ_COEF1, ADDR_SRS_VIQ_G_FACTOR);
        WriteDspCommDram (ADDR_RC2D_AVC_MAX_GAIN,0x10000 );
        WriteDspCommDram (ADDR_SRS_VIQ_COEF1, 0x4);  //kSRSVolumeControlHLCoeff1Light
        WriteDspCommDram (ADDR_SRS_VIQ_COEF2, 0x2);   //kSRSVolumeControlHLCoeff2Light
        WriteDspCommDram (ADDR_SRS_VIQ_COEF3, 0x3);   //kSRSVolumeControlHLCoeff3Light
        WriteDspCommDram (ADDR_SRS_VIQ_G_FACTOR,0x6 );   //gfactor
         break;
    case 1://Normal
        //printf("\n\n N dwCoefPnt=%x, dwFacPnt=%x\n\n ", ADDR_SRS_VIQ_COEF1, ADDR_SRS_VIQ_G_FACTOR);
        WriteDspCommDram (ADDR_RC2D_AVC_MAX_GAIN,0x20000 );
        WriteDspCommDram (ADDR_SRS_VIQ_COEF1, 0x8);
        WriteDspCommDram (ADDR_SRS_VIQ_COEF2, 0x4);
        WriteDspCommDram (ADDR_SRS_VIQ_COEF3, 0x6);
        WriteDspCommDram (ADDR_SRS_VIQ_G_FACTOR, 0x9);
        break;
    case 2://Heavy
        //printf("\n\n H dwCoefPnt=%x, dwFacPnt=%x\n\n ", ADDR_SRS_VIQ_COEF1, ADDR_SRS_VIQ_G_FACTOR);
        WriteDspCommDram (ADDR_RC2D_AVC_MAX_GAIN, 0x40000);
        WriteDspCommDram (ADDR_SRS_VIQ_COEF1, 0xe);
        WriteDspCommDram (ADDR_SRS_VIQ_COEF2, 0x7);
        WriteDspCommDram (ADDR_SRS_VIQ_COEF3, 0xb);
        WriteDspCommDram (ADDR_SRS_VIQ_G_FACTOR,0xd );
        break;
    default :// Set as Normal
        //printf("\n\n N dwCoefPnt=%x, dwFacPnt=%x\n\n ", ADDR_SRS_VIQ_COEF1, ADDR_SRS_VIQ_G_FACTOR);
        WriteDspCommDram (ADDR_RC2D_AVC_MAX_GAIN,0x20000 );
        WriteDspCommDram (ADDR_SRS_VIQ_COEF1, 0x8);
        WriteDspCommDram (ADDR_SRS_VIQ_COEF2, 0x4);
        WriteDspCommDram (ADDR_SRS_VIQ_COEF3, 0x6);
        WriteDspCommDram (ADDR_SRS_VIQ_G_FACTOR, 0x9);
        break;
    }
}
#endif


void vSetBassManageTable(UINT8 u1DspId, UINT8 u1DecId)
{
    UINT32 iLoopVar;
    UINT32 *pdwTable;

    UINT32 iCutFreq;

    iCutFreq = u4ReadDspShmDWRD (u1DspId, D_CUTOFF_FREQ);

    //Bass Management LPF and HPF Coeff.
    if (iCutFreq == 50)
    {
        //pdwTable = (UINT32 *) (BASSM_TABLE_50) + (u4FreqIdx * DSP_BASSM_SZ);
        pdwTable = (UINT32 *)&BASSM_TABLE_50[0];
    }
    else if (iCutFreq == 60)
    {
        //pdwTable = (UINT32 *) (BASSM_TABLE_60) + (u4FreqIdx * DSP_BASSM_SZ);
        pdwTable = (UINT32 *)&BASSM_TABLE_60[0];
    }
    else if (iCutFreq == 80)
    {
        //pdwTable = (UINT32 *) (BASSM_TABLE_80) + (u4FreqIdx * DSP_BASSM_SZ);
        pdwTable = (UINT32 *)&BASSM_TABLE_80[0];
    }
    else if (iCutFreq == 90)
    {
        //pdwTable = (UINT32 *) (BASSM_TABLE_90) + (u4FreqIdx * DSP_BASSM_SZ);
        pdwTable = (UINT32 *)&BASSM_TABLE_90[0];
    }
    else if (iCutFreq == 110)
    {
        //pdwTable = (UINT32 *) (BASSM_TABLE_110) + (u4FreqIdx * DSP_BASSM_SZ);
        pdwTable = (UINT32 *)&BASSM_TABLE_110[0];
    }
    else if (iCutFreq == 120)
    {
        //pdwTable = (UINT32 *) (BASSM_TABLE_120) + (u4FreqIdx * DSP_BASSM_SZ);
        pdwTable = (UINT32 *)&BASSM_TABLE_120[0];
    }
    else if (iCutFreq == 130)
    {
        //pdwTable = (UINT32 *) (BASSM_TABLE_130) + (u4FreqIdx * DSP_BASSM_SZ);
        pdwTable = (UINT32 *)&BASSM_TABLE_130[0];
    }
    else if (iCutFreq == 140)
    {
        //pdwTable = (UINT32 *) (BASSM_TABLE_140) + (u4FreqIdx * DSP_BASSM_SZ);
        pdwTable = (UINT32 *)&BASSM_TABLE_140[0];
    }
    else if (iCutFreq == 150)
    {
        //pdwTable = (UINT32 *) (BASSM_TABLE_150) + (u4FreqIdx * DSP_BASSM_SZ);
        pdwTable = (UINT32 *)&BASSM_TABLE_150[0];
    }
    else if (iCutFreq == 160)
    {
        //pdwTable = (UINT32 *) (BASSM_TABLE_160) + (u4FreqIdx * DSP_BASSM_SZ);
        pdwTable = (UINT32 *)&BASSM_TABLE_160[0];
    }
    else if (iCutFreq == 170)
    {
        //pdwTable = (UINT32 *) (BASSM_TABLE_170) + (u4FreqIdx * DSP_BASSM_SZ);
        pdwTable = (UINT32 *)&BASSM_TABLE_170[0];
    }
    else if (iCutFreq == 180)
    {
        //pdwTable = (UINT32 *) (BASSM_TABLE_180) + (u4FreqIdx * DSP_BASSM_SZ);
        pdwTable = (UINT32 *)&BASSM_TABLE_180[0];
    }
    else if (iCutFreq == 190)
    {
        //pdwTable = (UINT32 *) (BASSM_TABLE_190) + (u4FreqIdx * DSP_BASSM_SZ);
        pdwTable = (UINT32 *)&BASSM_TABLE_190[0];
    }
    else if (iCutFreq == 70)
    {
        //pdwTable = (UINT32 *) (BASSM_TABLE_70) + (u4FreqIdx * DSP_BASSM_SZ);
        pdwTable = (UINT32 *)&BASSM_TABLE_70[0];
    }
    else if (iCutFreq == 100)
    {
        //pdwTable = (UINT32 *) (BASSM_TABLE_100) + (u4FreqIdx * DSP_BASSM_SZ);
        pdwTable = (UINT32 *)&BASSM_TABLE_100[0];
    }
    else if (iCutFreq == 200)
    {
        //pdwTable = (UINT32 *) (BASSM_TABLE_200) + (u4FreqIdx * DSP_BASSM_SZ);
        pdwTable = (UINT32 *)&BASSM_TABLE_200[0];
    }
    else // 300Hz
    {
        pdwTable = (UINT32 *)&BASSM_TABLE_300[0];
    }

    if (u1DecId == AUD_DEC_MAIN)
    {
        for (iLoopVar = 0; iLoopVar < DSP_BASSM_SZ; iLoopVar++)
        {
            WriteDspCommDram (u1DspId, ADDR_RC2D_BASS_MANAGEMENT_FILTER_COEFF +
                              iLoopVar, pdwTable[iLoopVar]);
        }
    }
    else                        // secondary decoder
    {
        for (iLoopVar = 0; iLoopVar < DSP_BASSM_SZ; iLoopVar++)
        {
            WriteDspCommDram (u1DspId, ADDR_RC2D_BASS_MANAGEMENT_FILTER_COEFF_DEC2 +
                              iLoopVar, pdwTable[iLoopVar]);
        }
    }

#ifdef CC_S_SUBWOFFER_SUPPORT  ///CC_BASS_MANGER_INDEP_LPF
    if (u1DecId == AUD_DEC_MAIN)
    {
        iCutFreq = u4ReadDspShmDWRD (u1DspId, D_CUTOFF_FREQ_LPF);
    }
    else
    {
        iCutFreq = u4ReadDspShmDWRD (u1DspId, D_CUTOFF_FREQ_LPF_DEC2);
    }

    /// if iCutFreq == 0, it mean LPF is not independent.
    if(iCutFreq == 0)
    {
        return;
    }

    //Bass Management LPF Only.
    if (iCutFreq == 50)
    {
        pdwTable = (UINT32 *)&BASSM_TABLE_50[0];
    }
    else if (iCutFreq == 60)
    {
        pdwTable = (UINT32 *)&BASSM_TABLE_60[0];
    }
    else if (iCutFreq == 80)
    {
        pdwTable = (UINT32 *)&BASSM_TABLE_80[0];
    }
    else if (iCutFreq == 90)
    {
        pdwTable = (UINT32 *)&BASSM_TABLE_90[0];
    }
    else if (iCutFreq == 110)
    {
        pdwTable = (UINT32 *)&BASSM_TABLE_110[0];
    }
    else if (iCutFreq == 120)
    {
        pdwTable = (UINT32 *)&BASSM_TABLE_120[0];
    }
    else if (iCutFreq == 130)
    {
        pdwTable = (UINT32 *)&BASSM_TABLE_130[0];
    }
    else if (iCutFreq == 140)
    {
        pdwTable = (UINT32 *)&BASSM_TABLE_140[0];
    }
    else if (iCutFreq == 150)
    {
        pdwTable = (UINT32 *)&BASSM_TABLE_150[0];
    }
    else if (iCutFreq == 160)
    {
        pdwTable = (UINT32 *)&BASSM_TABLE_160[0];
    }
    else if (iCutFreq == 170)
    {
        pdwTable = (UINT32 *)&BASSM_TABLE_170[0];
    }
    else if (iCutFreq == 180)
    {
        pdwTable = (UINT32 *)&BASSM_TABLE_180[0];
    }
    else if (iCutFreq == 190)
    {
        pdwTable = (UINT32 *)&BASSM_TABLE_190[0];
    }
    else if (iCutFreq == 70)
    {
        pdwTable = (UINT32 *)&BASSM_TABLE_70[0];
    }
    else if (iCutFreq == 100)
    {
        pdwTable = (UINT32 *)&BASSM_TABLE_100[0];
    }
    else if (iCutFreq == 200)
    {
        pdwTable = (UINT32 *)&BASSM_TABLE_200[0];
    }
    else // 300Hz
    {
        pdwTable = (UINT32 *)&BASSM_TABLE_300[0];
    }

    if (u1DecId == AUD_DEC_MAIN)
    {
        for (iLoopVar = 0; iLoopVar < DSP_BASSM_SZ/2; iLoopVar++)   /// /2 for LPF independend only 
        {
            WriteDspCommDram (u1DspId, ADDR_RC2D_BASS_MANAGEMENT_FILTER_COEFF +
                              iLoopVar, pdwTable[iLoopVar]);
        }
    }
    else                        // secondary decoder
    {
        for (iLoopVar = 0; iLoopVar < DSP_BASSM_SZ/2; iLoopVar++)   /// /2 for LPF independend only 
        {
            WriteDspCommDram (u1DspId, ADDR_RC2D_BASS_MANAGEMENT_FILTER_COEFF_DEC2 +
                              iLoopVar, pdwTable[iLoopVar]);
        }
    }
#endif

}


void vSetupUpMix(UINT8 u1DspId, UINT8 u1DecId, UINT32 u4FreqIdx)
{
#ifndef UPMIX_SUPPORT
    vWriteDspWORD (u1DspId, ADDR_RC2D_PRO_LOGIC_II_CONFIG, 0);
   // vWriteDspWORD (ADDR_RC2D_NEO6_FLAG, 0);
#else // UPMIX_SUPPORT
    UINT32 iLoopVar;
    UINT32 dDspData;
    UINT32 *pdwShareInfo;
    UINT32 dwOfst, dwSz;
    UINT32 dwCodeDestAdr, dwCodeSrcAdr;
    UINT32 dwFlashOfst;
    UINT32 dwFlashOfst1;

    if (u1DecId != AUD_DEC_MAIN)
    {
        return;
    }

    dwFlashOfst = u4GetDspImgAddr (u1DspId);
#ifndef PROLOGICII_SUPPORT
    vWriteDspWORD (u1DspId, ADDR_RC2D_PRO_LOGIC_II_CONFIG, 0);
#else
    //Prologic II
    //vWriteDspWORD(ADDR_RC2D_PRO_LOGIC_II_CONFIG, u2DspData);
    DSP_FlushInvalidateDCache(u1DspId, CACHE_FLUSH_TYPE_DATA, TYPE_COMM_DRAM_IDX);

    dDspData = wReadDspWORD (u1DspId, ADDR_RC2D_PRO_LOGIC_II_CONFIG);
    //Check if Pro Logic II
    if (dDspData & 0x8001)
    {
        //Prologic II
        vWriteDspWORD (u1DspId, ADDR_RC2D_PRO_LOGIC_II_MODE,
                       u2ReadDspShmWORD (u1DspId, W_PLIIMODE));
        if (_rDspVars[u1DspId][AUD_DEC_MAIN].bCode3DUpmix != CODE3D_PROLOGIC_II)
        {
            //Set _bCode3DUpmix = CODE3D_PROLOGIC_II
            _rDspVars[u1DspId][AUD_DEC_MAIN].bCode3DUpmix = CODE3D_PROLOGIC_II;
            //PL2 Buffer Address
            WriteDspCommDram (u1DspId, ADDR_RC2D_3D_BUFFER_ADDR,
                              u4ReadDspShmDWRD (u1DspId, D_3D_BUF_ADDR));
            //Prologic II Table
            dwFlashOfst1 = u4GetDspImgAddr (u1DspId);
            pdwShareInfo = (UINT32 *) u4GetDspImgAddr (u1DspId);
            pdwShareInfo +=
                u4GetFlashData (dwFlashOfst1 + (DSPF_PL2_TABLE_SA << 2));
            pdwShareInfo += (u4FreqIdx * DSP_PL2_SZ);
            for (iLoopVar = 0; iLoopVar < DSP_PL2_SZ; iLoopVar++)
            {
                WriteDspCommDram (u1DspId, ADDR_PL2_TABLE + iLoopVar,
                                  pdwShareInfo[iLoopVar]);
            }
            /*lint -e{506}*/
            /*lint -e{774}*/
            if (_fgPostRamFromFlash)
            {
                // Download program code
                dwOfst = u4GetFlashData (dwFlashOfst + (DSPF_PL2_POST_SA << 2));
                dwSz = u4GetFlashData (dwFlashOfst + (DSPF_PL2_POST_SZ << 2));
                dwCodeDestAdr =
                    (dwGetDspIcacheAdr (u1DspId, TYPE_POST_ICACHE) +
                     MULTI_SURROUND_PRAM_ADR) * 4;
                // Rice: check if exceed to next section @20060804
                ASSERT((dwCodeDestAdr + ((dwSz - 1) * 4)) <
                       ((dwGetDspIcacheAdr (u1DspId, TYPE_POST_ICACHE)+VIRTUAL_SURROUND_PRAM_ADR) * 4));
                dwCodeSrcAdr = u4GetDspImgAddr (u1DspId) + (dwOfst * 4);
                vFlash2DramDma (dwCodeDestAdr, dwCodeSrcAdr, (dwSz - 1) * 4);
            }
        }
    }
#endif

    //NEO:6
 //   vWriteDspWORD (ADDR_RC2D_NEO6_FLAG, 0);
#endif // UPMIX_SUPPORT
}

void vSetupDownMix (UINT8 u1DecId, UINT32 u4FreqIdx)
{
#ifndef DOWNMIX_SUPPORT
    //Virtual Surround
    //Be careful of multi-flag on
    //Virtual Surround & Spatializer
    vWriteDspWORD (ADDR_RC2D_VIRTUAL_SURROUND, 0);
#else
    UINT32 iLoopVar;
    UINT32 dwOfst = 0;
    UINT32 dwSz = 0;
    UINT32 dVSCfg;

    // 20030912 modified
    if (u1DecId == AUD_DEC_MAIN)
    {   // fisrt decoder
        DSP_FlushInvalidateDCache(AUD_DSP0, CACHE_FLUSH_TYPE_DATA, TYPE_COMM_DRAM_IDX);

        dVSCfg = wReadDspWORD (AUD_DSP0, ADDR_RC2D_VIRTUAL_SURROUND) & 0xff;

        if (VSS_MTK == dVSCfg)
        {
            if ((_rDspVars[AUD_DSP0][AUD_DEC_MAIN].bCode3D != CODE3D_VIRTUAL_SURROUND))
            {
                _rDspVars[AUD_DSP0][AUD_DEC_MAIN].bCode3D = CODE3D_VIRTUAL_SURROUND;
            }
        }
#ifdef DSP_SUPPORT_SRSTSXT
        else if (VSS_SRS_TSXT == (dVSCfg & 0xFFFFFFFE))
        {
            UINT32 u4TblDstAdr;
            UINT32 *u4TblSrcAdr;
            UINT32 u4FSid;
            UINT32 u4tsxt_fg;
            UINT32 u4fc_elev;
            UINT32 u4tb_ctrl;
            UINT32 u4input_gain;
            UINT32 u4spksize;
            UINT32 u4BIMPROT;

            u4FSid       = u4FreqIdx;
            u4tsxt_fg    = u4ReadDspShmDWRD(D_SRS_TSXT_FLAGS);
            u4fc_elev    = u4ReadDspShmDWRD(D_SRS_TSXT_FC_Elevation);
            u4tb_ctrl    = u4ReadDspShmDWRD(D_SRS_TSXT_TB_TBCtrl);
            u4input_gain = u4ReadDspShmDWRD(D_SRS_TSXT_Input_gain);
            u4spksize    = (u4tsxt_fg>>24) & 0x0000000F;

            // Chk Input Fs
            if (u4FSid > SAMPLE_48K) u4FSid = SAMPLE_48K;

            // Init. TSXT Data Section: Control Paremeters
            {
                // User adjustable
                WriteDspCommDram (ADDR_SRS_TSXT_DATA + 0, ((u4tsxt_fg >> 16) & 0x0000000F) << 8);   //m_fc.m_Enable; 0x00010, false or true
                WriteDspCommDram (ADDR_SRS_TSXT_DATA + 1, u4fc_elev);                               //m_Elevation ; 0x40000
                WriteDspCommDram (ADDR_SRS_TSXT_DATA + 2, ((u4tsxt_fg >> 20) & 0x0000000F) << 8);   //m_tbc.m_Enable; 0x00010, false or true
                WriteDspCommDram (ADDR_SRS_TSXT_DATA + 3, ((u4tsxt_fg >>  4) & 0x0000000F) << 8);   //m_TruSurround; 0x00000, false or true
                WriteDspCommDram (ADDR_SRS_TSXT_DATA + 4, ((u4tsxt_fg) & 0x0000000F) << 8);         //Bypass
                WriteDspCommDram (ADDR_SRS_TSXT_DATA + 6, ((u4tsxt_fg >> 12) & 0x0000000F) << 8);   //Channel mode
                WriteDspCommDram (ADDR_SRS_TSXT_DATA + 7, ((u4tsxt_fg >>  8) & 0x0000000F) << 8);   //m_Headphone; 0x00000, false or true
                WriteDspCommDram (ADDR_SRS_TSXT_DATA + 8, u4tb_ctrl);                               //m_TruBassControl; 0x4cccd

                //user un-adjustable parameters
                #if 0
                WriteDspCommDram (ADDR_SRS_TSXT_DATA +  5, 0x00400000);  //m_InputGain; 0x40000 = -6dB
                #else

                //WriteDspCommDram (ADDR_SRS_TSXT_DATA +  5, 0x005ae140);    //-3dB
                WriteDspCommDram (ADDR_SRS_TSXT_DATA +  5, u4input_gain);    //-3dB
                #endif
                WriteDspCommDram (ADDR_SRS_TSXT_DATA +  9, 0x005a3d71);  //m_RearSpaceControl
                WriteDspCommDram (ADDR_SRS_TSXT_DATA + 10, 0x0050a3d7);  //m_FrontSpaceControl
                WriteDspCommDram (ADDR_SRS_TSXT_DATA + 11, 0x002d70a0);  //m_FrontCenterControl
                WriteDspCommDram (ADDR_SRS_TSXT_DATA + 12, 0x00090620);  //m_RearCenterControl
                WriteDspCommDram (ADDR_SRS_TSXT_DATA + 13, 0x00000000);  //m_ExpandorLevel
                WriteDspCommDram (ADDR_SRS_TSXT_DATA + 14, 0x00120670);  //m_ExpandorGain, varies with input gain
            }

            // Rice: only load RODATA, TSXT2CH's DATA section shall be set in API.
            u4TblDstAdr  = ADDR_SRS_TSXT_RODATA;
            u4TblSrcAdr = (UINT32 *)&SRSTSXT2CH_TBL[u4FSid][0];
            // Init. TSXT Data Section: D/L Table
            //SRS TSXT Tables Step 1.: 18 dwords (M2S)
            for (iLoopVar = 0; iLoopVar < 18; iLoopVar++)
            {
                WriteDspCommDram (u4TblDstAdr+iLoopVar, u4TblSrcAdr[iLoopVar]);
            }
            u4TblDstAdr += 18;
            u4TblSrcAdr += 18;
            //SRS TSXT Tables Step 2.: 11 dwords (TB)
            for (iLoopVar = 0; iLoopVar < 11; iLoopVar++)
            {
                WriteDspCommDram (u4TblDstAdr+iLoopVar,
                    u4TblSrcAdr[u4spksize/* SpeakerSize */*11 + iLoopVar]);
            }
            u4TblDstAdr += 11;
            u4TblSrcAdr += 11*8; /* 8 sets of 11 words */

            //SRS TSXT Tables Step 3.: 2 dwords ("Attack" & "Decay") (TB)
            for (iLoopVar = 0; iLoopVar < 2; iLoopVar++)
            {
                WriteDspCommDram (u4TblDstAdr+iLoopVar, u4TblSrcAdr[iLoopVar]);
            }
            u4TblDstAdr += 2;
            u4TblSrcAdr += 2;

            //SRS TSXT Tables Step 4.: 9 dwords (TS 2-ch), skip 9 dwords (for future TS multi-ch)
            for (iLoopVar = 0; iLoopVar < 9; iLoopVar++)
            {
                WriteDspCommDram (u4TblDstAdr+iLoopVar, u4TblSrcAdr[iLoopVar]);
            }
            u4TblDstAdr += 9;
            u4TblSrcAdr += 18;  //skip 9 dwords

            //SRS TSXT Tables Step 6.: 8 dwords (FOCUS)
            for (iLoopVar = 0; iLoopVar < 8; iLoopVar++)
            {
                WriteDspCommDram (u4TblDstAdr+iLoopVar, u4TblSrcAdr[iLoopVar]);
            }
            u4TblDstAdr += 8;
            u4TblSrcAdr += 8;

            //SRS TSXT Tables Step 7.: 9 dwords (constants)
            for (iLoopVar = 0; iLoopVar < 9; iLoopVar++)
            {
                WriteDspCommDram (u4TblDstAdr+iLoopVar, u4TblSrcAdr[iLoopVar]);
            }


            IO_WRITE32(BIM_BASE, 0x660, 0x00000001);
            IO_WRITE32(BIM_BASE, 0x670, 0x00002000);
            IO_WRITE32(BIM_BASE, 0x674, 0x00500000);
            IO_WRITE32(BIM_BASE, 0x678, 0x00000060);
            IO_WRITE32(BIM_BASE, 0x67c, 0x90000000);
            u4BIMPROT = IO_READ32(BIM_BASE, 0x650);
            //LOG( 9,"u4BIMPROT (0x%8x)\n",u4BIMPROT);

#ifndef __KERNEL__
            if ((u4BIMPROT&0x0000FF00)!= 0xFF00){
                WriteDspCommDram(ADDR_SRS_TSXT_RODATA+57, 0);
            }else{

                WriteDspCommDram(ADDR_SRS_TSXT_RODATA+57, 0x200);
            }
#else
            WriteDspCommDram(ADDR_SRS_TSXT_RODATA+57, 0x200);
#endif

            IO_WRITE32(BIM_BASE, 0x670, 0x00000000);
            u4BIMPROT = IO_READ32(BIM_BASE, 0x650);

            // D/L DSP Code
            if(_rDspVars[u1DecId].bCode3D != CODE3D_SRS_TSXT)
            {
                #if 0
                UINT32 u4CodeDestAdr,
                UINT32 u4CodeSrcAdr;
                UINT32 u4StrAddr = u4GetDspImgAddr ();

                dwOfst = u4GetFlashData (u4StrAddr + (DSPF_SRS_TSXT_SA << 2));
                dwSz   = u4GetFlashData (u4StrAddr + (DSPF_SRS_TSXT_SZ << 2));

                u4CodeDestAdr =
                    (dwGetDspIcacheAdr (TYPE_POST_ICACHE) + DOWNMIX_PRAM_ADR) * 4;
                u4CodeSrcAdr = u4GetDspImgAddr () + (dwOfst * 4);
                vFlash2DramDma (u4CodeDestAdr, u4CodeSrcAdr, (dwSz - 1) * 4);
                #endif
                _rDspVars[u1DecId].bCode3D = CODE3D_SRS_TSXT;
            }
        }
#endif //DSP_SUPPORT_SRSTSXT

#if  (defined(DSP_SUPPORT_SRSTSHD) || defined(DSP_SUPPORT_SRSWOWHD))
        else if (VSS_SRS_TSXT == (dVSCfg & 0xFFFFFFFE))
        {
            UINT32 u4TblDstAdr;
            UINT32 *u4TblSrcAdr;
            UINT32 u4FSid;
            UINT32 u4tsxt_fg;
            UINT32 u4fc_elev;
            UINT32 u4tb_ctrl;
            UINT32 u4compr_ctrl;
            UINT32 u4input_gain;
            UINT32 u4spksize;
            UINT32 u4anasize;
            UINT32 u4trubassflag;
            //UINT32 u4BIMPROT;
            //UINT32 u4temp,u4temp1,u4temp2,u4temp3,u4temp4,u4temp5,u4temp6,u4temp7,u4temp8;
            UINT32 u4headphone;
            UINT32 u4def_ctrl;
            UINT32 u4limiter_ctrl;
            UINT32 u4sub_spksize;
            UINT32 u4co_freq;
            UINT32 u4inp_type;
            UINT32 u4ch_dest;
            UINT32 u4wow_fc_ctrl;
            UINT32 u4center_ctrl, u4space_ctrl;
            UINT32 u4surr_lvl;
            UINT32 u4output_gain;
            UINT32 u4hpfsize;

            u4FSid           = u4FreqIdx;
            if (u4FSid > SAMPLE_48K)
            {
                u4FSid = SAMPLE_48K;
            }
            u4FSid=SAMPLE_48K;

            u4input_gain    = u4ReadDspShmDWRD(D_SRS_TSHD_INPUT_GAIN);      //0x00333333    ;//-8 dB
            u4output_gain  = u4ReadDspShmDWRD(D_SRS_TSHD_OUTPUT_GAIN); //0x007FFFFF    ;//0 dB
            u4tsxt_fg         = u4ReadDspShmDWRD(D_SRS_TSHD_FLAGS);
            u4headphone   = (u4tsxt_fg & 0x00000080) >> 7;
            u4wow_fc_ctrl  = u4ReadDspShmDWRD(D_SRS_TSHD_FC_Elevation);
            u4fc_elev         = u4ReadDspShmDWRD(D_SRS_TSHD_FC_Elevation);
            u4tb_ctrl          = u4ReadDspShmDWRD(D_SRS_TSHD_TB_TBCtrl);
            u4compr_ctrl       = u4ReadDspShmDWRD(D_SRS_TSHD_TB_COMPRCtrl);
            u4def_ctrl        = u4ReadDspShmDWRD(D_SRS_TSHD_DefCtrl);
            u4limiter_ctrl    = u4ReadDspShmDWRD(D_SRS_TSHD_LimiterCtrl);
            u4spksize        = (UINT32)uReadDspShmBYTE(B_SRS_TSHD_SPKSIZE);
            u4sub_spksize = (UINT32)uReadDspShmBYTE(B_SRS_TSHD_SUBSPKSIZE);
            u4anasize        = (UINT32)uReadDspShmBYTE(B_SRS_TSHD_ANASIZE);
            //u4leveldep = (UINT32)uReadDspShmBYTE(B_SRS_TSHD_LEVELDEP);
            u4trubassflag = (UINT32)uReadDspShmBYTE(B_SRS_TSHD_TBFLAG);
            u4co_freq        = (UINT32)uReadDspShmBYTE(B_SRS_TSHD_COFREQ);
            u4inp_type      = (UINT32)uReadDspShmBYTE(B_SRS_TSHD_INP_TYPE);
            u4ch_dest       = (UINT32)uReadDspShmBYTE(B_SRS_TSHD_CH_DEST);
            u4center_ctrl   = u4ReadDspShmDWRD(D_SRS_TSHD_Center_Ctrl);
            u4space_ctrl    = u4ReadDspShmDWRD(D_SRS_TSHD_Space_Ctrl);
            u4surr_lvl        = u4ReadDspShmDWRD(D_SRS_TSHD_Surr_Level);
            u4hpfsize        = (UINT32)uReadDspShmBYTE(B_SRS_TSHD_HPFSIZE);

            WriteDspCommDram (ADDR_SRS_TSHD_DATA + 0, u4tsxt_fg << 8);
            WriteDspCommDram (ADDR_SRS_TSHD_DATA +  1,u4fc_elev);         //m_Elevation ; 0x40000
            WriteDspCommDram (ADDR_SRS_TSHD_DATA +  2, 0    << 8);         //ts_mode
            WriteDspCommDram32bit (ADDR_SRS_TSHD_DATA +  3, u4tb_ctrl);          //m_TruBassControl; 0.3  for test (u4tb_ctrl=0.6)
            //WriteDspCommDram32bit (ADDR_SRS_TSHD_DATA +  3, 0x7fffffff);          //m_TruBassControl; 0.3  for test (u4tb_ctrl=0.6)
            WriteDspCommDram32bit (ADDR_SRS_TSHD_DATA +  4, u4input_gain);     //m_InputGain; 0x40000 = 0.5
            //user un-adjustable parameters
            WriteDspCommDram (ADDR_SRS_TSHD_DATA +  5, 0x002d1eb8);     //m_RearSpaceControl
            WriteDspCommDram (ADDR_SRS_TSHD_DATA +  6, 0x00090625);     //m_RearCenterControl

   #if (0)
            //TSHD v1.2
            WriteDspCommDram (ADDR_SRS_TSHD_DATA +  5, 0x00000000);     //m_RearSpaceControl
            WriteDspCommDram (ADDR_SRS_TSHD_DATA +  6, 0x00000000);     //m_RearCenterControl
    #endif
            WriteDspCommDram (ADDR_SRS_TSHD_DATA +  7, u4space_ctrl);     //m_FrontSpaceControl
            WriteDspCommDram (ADDR_SRS_TSHD_DATA +  8, u4center_ctrl);    //m_FrontCenterControl
            WriteDspCommDram (ADDR_SRS_TSHD_DATA +  9, u4def_ctrl   );      //m_definitionControl    0.3 scale 0
            WriteDspCommDram (ADDR_SRS_TSHD_DATA + 10, u4surr_lvl);        //SurroundLevel         0.6
            WriteDspCommDram (ADDR_SRS_TSHD_DATA + 11, 0x007e2bcf);      //tweak_gain              0.9857119009006160
            //WriteDspCommDram (ADDR_SRS_TSHD_DATA + 12, 0x005ae148);     //ts_bypass_gain          1
            WriteDspCommDram32bit (ADDR_SRS_TSHD_DATA + 12, u4output_gain);   //ts_bypass_gain used as output gain
            WriteDspCommDram (ADDR_SRS_TSHD_DATA + 13, u4wow_fc_ctrl);   //ts_wow_focus_ctrl   0.4  scale 0
            WriteDspCommDram (ADDR_SRS_TSHD_DATA + 14, u4limiter_ctrl);     //ts_limiter_ctrl         0.75
            WriteDspCommDram (ADDR_SRS_TSHD_DATA + 15, 0x00400000);      //ts_LowPassMixGain    0.5
            //Constants
            //WriteDspCommDram (ADDR_SRS_TSHD_DATA + 17, 0x00028f5c);  //ts_FeedbackConstant           0.02 scale 0
            WriteDspCommDram32bit (ADDR_SRS_TSHD_DATA + 16, 0x02d70a3d);  //ts_IntegrationConstant    0.71     scale 5
            WriteDspCommDram32bit (ADDR_SRS_TSHD_DATA + 17, 0x028f5c29);  //ts_FeedbackConstant           0.02 scale 0
            WriteDspCommDram32bit (ADDR_SRS_TSHD_DATA + 18, u4compr_ctrl);  //ts_IntegrationLimit        sacle  5 /* Set integrationLimit = 2 + truBassControl * 6 */
            WriteDspCommDram (ADDR_SRS_TSHD_DATA + 19, 0x007fffff);  //kWOWOne
            WriteDspCommDram (ADDR_SRS_TSHD_DATA + 20, 0x00200000);  //kWOWLimiterMinAdjustLevel   0.25 scale 0
            WriteDspCommDram (ADDR_SRS_TSHD_DATA + 21, 0x0019999a);  //kWOWLimiterLevelRatio         0.2 scale  0
            WriteDspCommDram (ADDR_SRS_TSHD_DATA + 22, 0x007fffff);  //kWOWLimiterMaxMultiplier        1  scale 0
            WriteDspCommDram (ADDR_SRS_TSHD_DATA + 23, 0x00400000);  //kWOWLimiterLevelMax        1 scale 1
            WriteDspCommDram (ADDR_SRS_TSHD_DATA + 24, 0xffd00000);  //kWOWLimiterLevelMin_s_Max    (0.25 -1) scale 1
            WriteDspCommDram (ADDR_SRS_TSHD_DATA + 25, 0x00400000);  //kWOWLimiterLevelDefault        1 scale 1
            //need backup data
            WriteDspCommDram (ADDR_SRS_TSHD_DATA + 26, 0x00000000);  //ts_RefLevel
            WriteDspCommDram (ADDR_SRS_TSHD_DATA + 27, 0x00000000);  //ts_FasdLevel
            WriteDspCommDram (ADDR_SRS_TSHD_DATA + 28, 0x00000000);  //ts_Integrator
            WriteDspCommDram (ADDR_SRS_TSHD_DATA + 29, 0x00200000);  //ts_limiter_lv_msb
            WriteDspCommDram (ADDR_SRS_TSHD_DATA + 30, 0x00000000);  //ts_limiter_lv_lsb
            WriteDspCommDram (ADDR_SRS_TSHD_DATA + 31, 0x00000000);  //ts_sub_FasdLevel
            WriteDspCommDram (ADDR_SRS_TSHD_DATA + 32, 0x00000000);  //ts_sub_Integrator
            //ts_Attack, ts_Decay, kWOWLimiterDecay
            //WriteDspCommDram (ADDR_SRS_TSHD_DATA + 49, SRSTSHD_M_TBL[u4FSid][0]);  //ts_Attack
            //WriteDspCommDram (ADDR_SRS_TSHD_DATA + 50, SRSTSHD_M_TBL[u4FSid][1]);  //ts_Decay
            WriteDspCommDram32bit (ADDR_SRS_TSHD_DATA + 49, SRSTSHD_M_TBL[u4FSid][0]);  //ts_Attack
            WriteDspCommDram32bit (ADDR_SRS_TSHD_DATA + 50, SRSTSHD_M_TBL[u4FSid][1]);  //ts_Decay
            WriteDspCommDram (ADDR_SRS_TSHD_DATA + 51, SRSTSHD_M_TBL[u4FSid][2]);  //kWOWLimiterDecay
            //peak tracking for level independence
            WriteDspCommDram (ADDR_SRS_TSHD_DATA + 52, 0x00000000);   //ts_mPeakHold
            WriteDspCommDram (ADDR_SRS_TSHD_DATA + 53, 0x00000000);   //ts_mPeakHoldCount
            WriteDspCommDram (ADDR_SRS_TSHD_DATA + 54, 0x00000000);   //ts_mNextPeak
            //WriteDspCommDram (ADDR_SRS_TSHD_DATA + 55, 0x00040cf6);   //ts_mPeak
            WriteDspCommDram (ADDR_SRS_TSHD_DATA + 56, 0x00000000);   //ts_mLastMoveUp
            //WriteDspCommDram (ADDR_SRS_TSHD_DATA + 57, 0x007fffff);   //ts_mValley,24bit
            //WriteDspCommDram (ADDR_SRS_TSHD_DATA + 57, 0x00000000);   //ts_mValley,32bit
            WriteDspCommDram32bit (ADDR_SRS_TSHD_DATA + 58, 0x00000960);   //ts_mPeakSampleCount
            WriteDspCommDram32bit (ADDR_SRS_TSHD_DATA + 59, 0x00fccccd);   //ts_mMultiplier
            //WriteDspCommDram (ADDR_SRS_TSHD_DATA + 60, u4leveldep<<8);   //ts_mLevelIndependent
            //WriteDspCommDram (ADDR_SRS_TSHD_DATA + 60, 0x00000001);   //ts_mLevelIndependent
            //WriteDspCommDram (ADDR_SRS_TSHD_DATA + 61, 0x00200000);   //ts_mCompressorControl
            WriteDspCommDram (ADDR_SRS_TSHD_DATA + 60, u4trubassflag<<8);  //     cc_trubassflag
            WriteDspCommDram32bit (ADDR_SRS_TSHD_DATA + 61, u4compr_ctrl);   //ts_mCompressorControl
            WriteDspCommDram32bit (ADDR_SRS_TSHD_DATA + 62, 0x7fffffff);   //ts_mInputGain

            WriteDspCommDram32bit (ADDR_SRS_TSHD_DATA + 63, 0x040cf647);   //kSrsTruBassInvHalfMinDynRng
            //WriteDspCommDram (ADDR_SRS_TSHD_DATA + 62, 0x007fffff);   //kSrsTruBassMinValley,24bit
            WriteDspCommDram32bit (ADDR_SRS_TSHD_DATA + 64, 0x00000015);   //kSrsTruBassMinValley,32bit
            WriteDspCommDram32bit (ADDR_SRS_TSHD_DATA + 65, 0x7e666666);   //kSrsTruBassHalfMinDynRng
            WriteDspCommDram32bit (ADDR_SRS_TSHD_DATA + 66, 0x7fffffff);   //kSrsTruBassMultiplierMax
            WriteDspCommDram32bit (ADDR_SRS_TSHD_DATA + 67, 0x00080000);   //kSrsTruBassMultiplierMin
            WriteDspCommDram32bit (ADDR_SRS_TSHD_DATA + 68, 0x00000960);   //kPeakSampleCount
            WriteDspCommDram32bit (ADDR_SRS_TSHD_DATA + 69, 0xc169fbe7);   //K_threshold

            u4TblSrcAdr = (UINT32 *)&SRSTSHD_M_TBL[u4FSid][3];
            //M2S (6)
            u4TblDstAdr  = ADDR_SRS_TSHD_M2S_COEF;
            for (iLoopVar = 0; iLoopVar < 30; iLoopVar++)
            {
                WriteDspCommDram (u4TblDstAdr+iLoopVar, u4TblSrcAdr[iLoopVar]);
            }
            u4TblSrcAdr += 30;
            //persp front
            u4TblDstAdr  = ADDR_SRS_TSHD_PERSP_FR_COEF;
            for (iLoopVar = 0; iLoopVar < 15; iLoopVar++)
            {
                WriteDspCommDram (u4TblDstAdr+iLoopVar, u4TblSrcAdr[iLoopVar]);
            }
            u4TblSrcAdr += 15;
            //FOCUS
            u4TblDstAdr  = ADDR_SRS_TSHD_FOCUS_COEF;
            for (iLoopVar = 0; iLoopVar < 5; iLoopVar++)
            {
                WriteDspCommDram (u4TblDstAdr+iLoopVar, u4TblSrcAdr[iLoopVar]);
            }
            u4TblSrcAdr += 5;
            //persp rear
            u4TblDstAdr  = ADDR_SRS_TSHD_PERSP_RR_COEF;
            for (iLoopVar = 0; iLoopVar < 15; iLoopVar++)
            {
                WriteDspCommDram (u4TblDstAdr+iLoopVar, u4TblSrcAdr[iLoopVar]);
            }
            u4TblSrcAdr += 15;

            #if 0
            if(u4spksize < 10)//if it is customsize,it will not write here
            {
                //TB, sub TB
                u4TblDstAdr  = ADDR_SRS_TSHD_TB_COEF;
                for (iLoopVar = 0; iLoopVar < 15; iLoopVar++)
                {
                    //WriteDspCommDram (u4TblDstAdr+iLoopVar, u4TblSrcAdr[u4spksize*15 + iLoopVar]);
                    WriteDspCommDram32bit (u4TblDstAdr+iLoopVar, u4TblSrcAdr[u4spksize*15 + iLoopVar]);
                }

                #if 0
                //Analysis Filter coeff
                u4TblDstAdr  = ADDR_SRS_TSHD_TB_COEF + 15;
                for (iLoopVar = 5; iLoopVar < 15; iLoopVar++)
                {
                    //WriteDspCommDram (u4TblDstAdr+iLoopVar-5, u4TblSrcAdr[u4anasize*15 + iLoopVar]);
                    WriteDspCommDram32bit (u4TblDstAdr+iLoopVar-5, u4TblSrcAdr[u4anasize*15 + iLoopVar]);
                }
                #endif
            }
            else if(u4spksize == 10)//custom speakersize
            {
                //TB, sub TB
                u4TblDstAdr  = ADDR_SRS_TSHD_TB_COEF;
                for (iLoopVar = 0; iLoopVar < 15; iLoopVar++)
                {
                    //WriteDspCommDram (u4TblDstAdr+iLoopVar, u4TblSrcAdr[u4spksize*15 + iLoopVar]);
                    WriteDspCommDram32bit (u4TblDstAdr+iLoopVar, SRSTB_CUSTOM_SPEAKER_TBL[iLoopVar]);
                }

            }
            else
            {
                ;
            }

            if(u4anasize < 10)//if it is customsize,it will not write here
            {
                //Analysis Filter coeff
                u4TblDstAdr  = ADDR_SRS_TSHD_TB_COEF + 15;

                #if 1
                for (iLoopVar = 5; iLoopVar < 15; iLoopVar++)
                {
                    //WriteDspCommDram (u4TblDstAdr+iLoopVar-5, u4TblSrcAdr[u4anasize*15 + iLoopVar]);
                    WriteDspCommDram32bit (u4TblDstAdr+iLoopVar-5, u4TblSrcAdr[u4anasize*15 + iLoopVar]);
                }
                #endif
            }
            else if(u4anasize == 10)//custom ana speakersize
            {
                u4TblDstAdr  = ADDR_SRS_TSHD_TB_COEF + 15;
                for (iLoopVar = 0; iLoopVar < 10; iLoopVar++)
                {
                    WriteDspCommDram32bit (u4TblDstAdr+iLoopVar, SRSTB_CUSTOM_SPEAKER_TBL[iLoopVar + 15]);
                }

            }
            else
            {
                ;
            }
            #endif

            if(u4anasize < 10)//if it is customsize,it will not write here
            {
                //TB, sub TB
                u4TblDstAdr  = ADDR_SRS_TSHD_TB_COEF;
                //WriteDspCommDram (u4TblDstAdr+iLoopVar, u4TblSrcAdr[u4spksize*15 + iLoopVar]);
                WriteDspCommDram32bit (u4TblDstAdr+0, u4TblSrcAdr[u4anasize*15 + 1]);
                WriteDspCommDram32bit (u4TblDstAdr+1, u4TblSrcAdr[u4anasize*15 + 3]);
                WriteDspCommDram32bit (u4TblDstAdr+2, u4TblSrcAdr[u4anasize*15 + 9]);
                WriteDspCommDram32bit (u4TblDstAdr+3, u4TblSrcAdr[u4anasize*15 + 8]);
                WriteDspCommDram32bit (u4TblDstAdr+4, u4TblSrcAdr[u4anasize*15 + 7]);
                WriteDspCommDram32bit (u4TblDstAdr+5, u4TblSrcAdr[u4anasize*15 + 14]);
                WriteDspCommDram32bit (u4TblDstAdr+6, u4TblSrcAdr[u4anasize*15 + 13]);
                WriteDspCommDram32bit (u4TblDstAdr+7, u4TblSrcAdr[u4anasize*15 + 12]);

            }
            else if(u4anasize == 10)//custom speakersize
            {
                //TB, sub TB
                u4TblDstAdr  = ADDR_SRS_TSHD_TB_COEF;
                WriteDspCommDram32bit (u4TblDstAdr+0, SRSTB_CUSTOM_SPEAKER_TBL[1]);
                WriteDspCommDram32bit (u4TblDstAdr+1, SRSTB_CUSTOM_SPEAKER_TBL[3]);
                WriteDspCommDram32bit (u4TblDstAdr+2, SRSTB_CUSTOM_SPEAKER_TBL[9]);
                WriteDspCommDram32bit (u4TblDstAdr+3, SRSTB_CUSTOM_SPEAKER_TBL[8]);
                WriteDspCommDram32bit (u4TblDstAdr+4, SRSTB_CUSTOM_SPEAKER_TBL[7]);
                WriteDspCommDram32bit (u4TblDstAdr+5, SRSTB_CUSTOM_SPEAKER_TBL[14]);
                WriteDspCommDram32bit (u4TblDstAdr+6, SRSTB_CUSTOM_SPEAKER_TBL[13]);
                WriteDspCommDram32bit (u4TblDstAdr+7, SRSTB_CUSTOM_SPEAKER_TBL[12]);

            }
            else
            {
                ;
            }

            if(u4spksize < 10)//if it is customsize,it will not write here
            {
                u4TblDstAdr  = ADDR_SRS_TSHD_TB_COEF;
                WriteDspCommDram32bit (u4TblDstAdr+8, u4TblSrcAdr[u4spksize*15 + 9]);
                WriteDspCommDram32bit (u4TblDstAdr+9, u4TblSrcAdr[u4spksize*15 + 8]);
                WriteDspCommDram32bit (u4TblDstAdr+10, u4TblSrcAdr[u4spksize*15 + 7]);
                WriteDspCommDram32bit (u4TblDstAdr+11, u4TblSrcAdr[u4spksize*15 + 14]);
                WriteDspCommDram32bit (u4TblDstAdr+12, u4TblSrcAdr[u4spksize*15 + 13]);
                WriteDspCommDram32bit (u4TblDstAdr+13, u4TblSrcAdr[u4spksize*15 + 12]);

            }
            else if(u4spksize == 10)//custom ana speakersize
            {
                u4TblDstAdr  = ADDR_SRS_TSHD_TB_COEF;
                WriteDspCommDram32bit (u4TblDstAdr+8, SRSTB_CUSTOM_SPEAKER_TBL[9]);
                WriteDspCommDram32bit (u4TblDstAdr+9, SRSTB_CUSTOM_SPEAKER_TBL[8]);
                WriteDspCommDram32bit (u4TblDstAdr+10, SRSTB_CUSTOM_SPEAKER_TBL[7]);
                WriteDspCommDram32bit (u4TblDstAdr+11, SRSTB_CUSTOM_SPEAKER_TBL[14]);
                WriteDspCommDram32bit (u4TblDstAdr+12, SRSTB_CUSTOM_SPEAKER_TBL[13]);
                WriteDspCommDram32bit (u4TblDstAdr+13, SRSTB_CUSTOM_SPEAKER_TBL[12]);
            }
            else
            {
                ;
            }

            u4TblDstAdr  = ADDR_SRS_TSHD_SUB_TB_COEF;
            for (iLoopVar = 0; iLoopVar < 15; iLoopVar++)
            {
                WriteDspCommDram (u4TblDstAdr+iLoopVar, u4TblSrcAdr[u4sub_spksize*15 + iLoopVar]);
            }
            u4TblSrcAdr += 165;// 15*11
            u4TblDstAdr  = ADDR_SRS_TSHD_HPF_COEF;
            for (iLoopVar = 0; iLoopVar < 5; iLoopVar++)
            {
                WriteDspCommDram32bit (u4TblDstAdr+iLoopVar, u4TblSrcAdr[u4hpfsize*5 + iLoopVar]);
            }

            u4TblSrcAdr += 45;// 5*9
            //Def
            u4TblDstAdr  = ADDR_SRS_TSHD_DEF_COEF;
            for (iLoopVar = 0; iLoopVar < 5; iLoopVar++)
            {
                WriteDspCommDram (u4TblDstAdr+iLoopVar, u4TblSrcAdr[iLoopVar]);
            }
            u4TblSrcAdr += 5;
            //cross over
            u4TblDstAdr  = ADDR_SRS_TSHD_CROSS_COEF;
            for (iLoopVar = 0; iLoopVar < 10; iLoopVar++)
            {
                WriteDspCommDram (u4TblDstAdr+iLoopVar, u4TblSrcAdr[u4co_freq*10 + iLoopVar]);
            }
            u4TblSrcAdr += 40; //10*4
            //SRS3D
            u4TblDstAdr  = ADDR_SRS_TSHD_SRS3D_COEF;
            for (iLoopVar = 0; iLoopVar < 5; iLoopVar++)
            {
                WriteDspCommDram (u4TblDstAdr+iLoopVar, u4TblSrcAdr[u4headphone*5 + iLoopVar]);
            }
            for (iLoopVar = 5; iLoopVar < 10; iLoopVar++)
            {
                WriteDspCommDram (u4TblDstAdr+iLoopVar, u4TblSrcAdr[iLoopVar+5]);
            }
            u4TblSrcAdr += 15; //5*2+5

            WriteDspCommDram (ADDR_SRS_TSHD_BANK2_CONST +  0, 0x00400000 );    //kPlus12dB    scale 3
            WriteDspCommDram (ADDR_SRS_TSHD_BANK2_CONST +  1, 0x005a3d71 );    //kPlus9dB    scale 2
            //WriteDspCommDram (ADDR_SRS_TSHD_BANK2_CONST +  2, 0x005a3d71 );    //kPlus3dB    scale 1
            WriteDspCommDram (ADDR_SRS_TSHD_BANK2_CONST +  2, 0x0071eb85 );    //kPlus5dB    scale 1
            WriteDspCommDram (ADDR_SRS_TSHD_BANK2_CONST +  3, 0x005ae148 );    //kMinus3dB    scale 0
            WriteDspCommDram (ADDR_SRS_TSHD_BANK2_CONST +  4, 0x0050a3d7 );    //kMinus4dB    scale 0
            WriteDspCommDram (ADDR_SRS_TSHD_BANK2_CONST +  5, 0x0047ae14 );     //kMinus5dB    scale 0
            //WriteDspCommDram (ADDR_SRS_TSHD_BANK2_CONST +  7, 0x00400000 );    //kMinus6dB    scale 0
            //WriteDspCommDram (ADDR_SRS_TSHD_BANK2_CONST +  8, 0x005ae148 );    //kMinus9dB    scale -1
            WriteDspCommDram (ADDR_SRS_TSHD_BANK2_CONST +  6, 0x00090625 );    //kMinus11dB_scale2
            WriteDspCommDram (ADDR_SRS_TSHD_BANK2_CONST +  7, 0x00404189 );    //kMinus12dB    scale -1
            WriteDspCommDram (ADDR_SRS_TSHD_BANK2_CONST +  8, 0x0050e560 );    //kMinus16dB    scale -2
            WriteDspCommDram (ADDR_SRS_TSHD_BANK2_CONST +  9, 0x00408312 );    //kMinus18dB    scale -2
            WriteDspCommDram (ADDR_SRS_TSHD_BANK2_CONST + 10, 0x00666666 );    //kMinus20dB    scale -3
            WriteDspCommDram (ADDR_SRS_TSHD_BANK2_CONST + 11, 0x00551eb8 );    //kPoint665    scale  0
            WriteDspCommDram (ADDR_SRS_TSHD_BANK2_CONST + 12, 0x00570a3d );    //kPoint34        scale -1
            WriteDspCommDram (ADDR_SRS_TSHD_BANK2_CONST + 13, u4inp_type <<8);    //kPoint665    scale  0
            WriteDspCommDram (ADDR_SRS_TSHD_BANK2_CONST + 14, u4ch_dest <<8);

            if (_rDspVars[AUD_DEC_MAIN].bCode3D != CODE3D_SRS_TSXT)
            {
                _rDspVars[AUD_DEC_MAIN].bCode3D = CODE3D_SRS_TSXT;
            }
        }
#endif //DSP_SUPPORT_SRSTSHD

#if  defined(DSP_SUPPORT_SRSCC3D)
            else if (VSS_SRS_TSXT == (dVSCfg & 0xFFFFFFFE))
            {
                UINT32 u4TblDstAdr;
                UINT32 *u4TblSrcAdr;
                UINT32 u4FSid;

                UINT32 u4tb_ctrl;
                UINT32 u4compr_ctrl;
                UINT32 u4input_gain;
                UINT32 u4spksize;
                UINT32 u4anasize;
                UINT32 u4trubassflag;
                UINT32 u4output_gain;
                UINT32 u4flag;
                UINT32 u4mode;
                UINT32 u4inmode;
                UINT32 u43dmode;
                UINT32 u4hpfsize;
                UINT32 u4inisize;
                UINT32 u4inidepth;
                UINT32 u4spacectrltune;
                UINT32 u4mixfadectrl;
                UINT32 u4frontcenctrl;
                UINT32 u4srs3dcenterctrl;
                UINT32 u4srs3dspacectrl;

                UINT32 u4frontspacectrl;
                UINT32 u4rearspacectrl;
                UINT32 u4rearcenterctrl;
                UINT32 u4trusurlevel;
                UINT32 u4mixfadectrlext;

                UINT32 u4focuslevel;
                UINT32 u4lfocuslevel;
                UINT32 u4rfocuslevel;
                UINT32 u4ldeflevel;
                UINT32 u4rdeflevel;

                UINT32 u4reflevel;
                UINT32 u4tbq;
                UINT32 u4dculevel;
    
                u4FSid           = u4FreqIdx;
                if (u4FSid > SAMPLE_48K)
                {
                    u4FSid = SAMPLE_48K;
                }
                u4FSid=SAMPLE_48K;


                u4flag  = (UINT32)u2ReadDspShmWORD(W_SRS_CC3D_FLAG);
                u4mode  = (UINT32)uReadDspShmBYTE(B_SRS_CC3D_MODE);//cc_mode

                u4input_gain    = (UINT32)u2ReadDspShmWORD(W_SRS_CC3D_INPUT_GAIN);      //
                u4output_gain  = u4ReadDspShmDWRD(D_SRS_CC3D_OUTPUT_GAIN); //

                u4inmode  = (UINT32)uReadDspShmBYTE(B_SRS_CC3D_INMODE);  //cc_inmode
                u43dmode  = (UINT32)uReadDspShmBYTE(B_SRS_CC3D_3DMODE);  //cc_srs3dmode

                u4hpfsize        = (UINT32)uReadDspShmBYTE(B_SRS_CC3D_HPFSIZE);
                u4inisize        = (UINT32)uReadDspShmBYTE(B_SRS_CC3D_INISIZE);
                u4inidepth        = (UINT32)uReadDspShmBYTE(B_SRS_CC3D_INIDEPTH);

                u4tb_ctrl          = u4ReadDspShmDWRD(D_SRS_CC3D_TB_TBCtrl);
                u4compr_ctrl       = u4ReadDspShmDWRD(D_SRS_CC3D_TB_COMPRCtrl);

                u4spksize        = (UINT32)uReadDspShmBYTE(B_SRS_CC3D_SPKSIZE);
                u4anasize        = (UINT32)uReadDspShmBYTE(B_SRS_CC3D_ANASIZE);
                u4trubassflag = (UINT32)uReadDspShmBYTE(B_SRS_CC3D_TBFLAG);

                u4spacectrltune  = (UINT32)u2ReadDspShmWORD(W_SRS_SPACE_CTRLTUNE);
                u4mixfadectrl  = (UINT32)u2ReadDspShmWORD(W_SRS_MIX_FADECTRL);
                u4frontcenctrl  = (UINT32)u2ReadDspShmWORD(W_SRS_FRONT_CENTERCTRL);

                u4frontspacectrl  = (UINT32)u2ReadDspShmWORD(W_SRS_FRONT_SPACECTRL);
                u4rearspacectrl  = (UINT32)u2ReadDspShmWORD(W_SRS_REAR_SPACECTRL);
                u4rearcenterctrl  = (UINT32)u2ReadDspShmWORD(W_SRS_REAR_CENTERCTRL);
                u4trusurlevel  = (UINT32)u2ReadDspShmWORD(W_SRS_TRUSUR_LEVEL);
                u4mixfadectrlext  = (UINT32)u4ReadDspShmDWRD(D_SRS_MIX_FADECTRLEXT);


                u4srs3dcenterctrl  = (UINT32)u2ReadDspShmWORD(W_SRS_SRS3D_CENTERCTRL);
                u4srs3dspacectrl  = (UINT32)u2ReadDspShmWORD(W_SRS_SRS3D_SPACECTRL);

                u4focuslevel  = (UINT32)u2ReadDspShmWORD(W_SRS_FOCUS_LEVEL);
                u4lfocuslevel  = (UINT32)u2ReadDspShmWORD(W_SRS_LFOCUS_LEVEL);
                u4rfocuslevel  = (UINT32)u2ReadDspShmWORD(W_SRS_RFOCUS_LEVEL);
                u4ldeflevel  = (UINT32)u2ReadDspShmWORD(W_SRS_LDEF_LEVEL);
                u4rdeflevel  = (UINT32)u2ReadDspShmWORD(W_SRS_RDEF_LEVEL);

                u4reflevel  = (UINT32)u2ReadDspShmWORD(W_SRS_CC3D_REF_LEV);
                u4tbq  = (UINT32)u2ReadDspShmWORD(W_SRS_CC3D_TBQ);
                u4dculevel  = (UINT32)u2ReadDspShmWORD(W_SRS_CC3D_DUCLEV);
                

                #if 0
                WriteDspCommDram (ADDR_SRS_COMMON_DATA + 0, 0x00000400);  //     cc_cnter0
                WriteDspCommDram (ADDR_SRS_COMMON_DATA + 1, 0x00000000);  //     leftInOut
                WriteDspCommDram (ADDR_SRS_COMMON_DATA + 2, 0x00000000);  //     rightInOut
                WriteDspCommDram (ADDR_SRS_COMMON_DATA + 3, 0x00000000);  //     cc_temp1
                WriteDspCommDram (ADDR_SRS_COMMON_DATA + 4, 0x00000000);  //     cc_temp2
                WriteDspCommDram (ADDR_SRS_COMMON_DATA + 5, 0x00000000);  //     cc_temp3
                WriteDspCommDram (ADDR_SRS_COMMON_DATA + 6, 0x00000000);  //     cc_temp4
                WriteDspCommDram (ADDR_SRS_COMMON_DATA + 7, 0x00000000);  //     cc_temp5
                WriteDspCommDram (ADDR_SRS_COMMON_DATA + 8, 0x00000000);  //     cc_temp6
                WriteDspCommDram (ADDR_SRS_COMMON_DATA + 9, 0x00000000);  //     cc_temp7
                #endif

                WriteDspCommDram (ADDR_SRS_COMMON_DATA + 10, u4flag<<8);  //   cc_flag   cc_suren
                WriteDspCommDram (ADDR_SRS_COMMON_DATA + 11, u4mode<<8);  //    cc_mode
                WriteDspCommDram (ADDR_SRS_COMMON_DATA + 12, 0x005a7f00);  //     cc_bypass_gain  = 0.707
                WriteDspCommDram32bit (ADDR_SRS_COMMON_DATA + 13, u4input_gain<<16);  //     cc_input_gain  = 0.5                                                                                   
                WriteDspCommDram32bit (ADDR_SRS_COMMON_DATA + 14, u4output_gain);  //     cc_output_gain =
                WriteDspCommDram (ADDR_SRS_COMMON_DATA + 15, 0x00200000);  //    cc_compressorscalecoeff
                //WriteDspCommDram (ADDR_SRS_COMMON_DATA + 16, 0x00200000);  //    cc_spacectrltune
                WriteDspCommDram (ADDR_SRS_COMMON_DATA + 16, u4spacectrltune<<8);  //    cc_spacectrltune
                Printf("fubofeng SRS_COMMON_DATA 0x%x\n",ADDR_SRS_COMMON_DATA);
                //Printf("fubofeng SRS_CC3D_DATA 0x%x\n",ADDR_SRS_CC3D_DATA);
                //WriteDspCommDram (ADDR_SRS_CC3D_DATA + 0, 0x00000100);  //       cc_enable
                //WriteDspCommDram (ADDR_SRS_CC3D_DATA + 0, 0x005a7f00);  //       cc_bypass_gain  = 0.707
                //WriteDspCommDram (ADDR_SRS_CC3D_DATA + 1, u4input_gain);  //       cc_input_gain  = 0.5
                //WriteDspCommDram32bit (ADDR_SRS_CC3D_DATA + 2, u4output_gain);  //       cc_output_gain =
                //WriteDspCommDram (ADDR_SRS_CC3D_DATA + 4, 0x00000000);  //       cc_lrenhenable      false
                WriteDspCommDram32bit (ADDR_SRS_CC3D_DATA + 0, 0x33333333);  //       cc_kSRSCC3DMode3DInputGain=0.40
                WriteDspCommDram (ADDR_SRS_CC3D_DATA + 1, 0x00000000);  //       cc_depthsumfront
                WriteDspCommDram (ADDR_SRS_CC3D_DATA + 2, 0x00000000);  //       cc_depthdifffront
                WriteDspCommDram (ADDR_SRS_CC3D_DATA + 3, 0x00000000);  //       cc_depthsumleft
                WriteDspCommDram (ADDR_SRS_CC3D_DATA + 4, 0x00000000);  //       cc_depthsumright
                WriteDspCommDram32bit (ADDR_SRS_CC3D_DATA + 5, ((0x8000+ADDR_FFDELAYLINE_LEFT)<<16));  //      cc_delaylineleftwptr
                WriteDspCommDram32bit (ADDR_SRS_CC3D_DATA + 6, ((0x8000+ADDR_FFDELAYLINE_RIGHT)<<16));  //      cc_delaylinerightwptr
                WriteDspCommDram (ADDR_SRS_CC3D_DATA + 7, 0x00000000);  //      cc_bcnt
                WriteDspCommDram (ADDR_SRS_CC3D_DATA + 8, 0x00006000);  //      cc_depthestimationblocklength           96
                //WriteDspCommDram (ADDR_SRS_CC3D_DATA + 9, 0x00002000);  //      cc_delaylineinitialdepth        //32
                WriteDspCommDram32bit (ADDR_SRS_CC3D_DATA + 9, u4inidepth<<16);  //      cc_delaylineinitialdepth        //32
                WriteDspCommDram32bit (ADDR_SRS_CC3D_DATA + 10, ((0x8000+ADDR_FFDELAYLINE_RIGHT+2000-128)<<16));  //      cc_delaylinerightrptr     2000-(96+32)
                WriteDspCommDram32bit (ADDR_SRS_CC3D_DATA + 11, ((0x8000+ADDR_FFDELAYLINE_RIGHT+2000-128)<<16));  //      cc_delaylinerightprevrptr
                WriteDspCommDram32bit (ADDR_SRS_CC3D_DATA + 12, ((0x8000+ADDR_FFDELAYLINE_LEFT+2000-128)<<16));  //      cc_delaylineleftrptr
                WriteDspCommDram32bit (ADDR_SRS_CC3D_DATA + 13, ((0x8000+ADDR_FFDELAYLINE_LEFT+2000-128)<<16));  //      cc_delaylineleftprevrptr
                WriteDspCommDram32bit (ADDR_SRS_CC3D_DATA + 14, ((0x8000+ADDR_FFDELAYLINE_LEFT+2000-96)<<16));  //      cc_delaylineleftblkrptr
                WriteDspCommDram32bit (ADDR_SRS_CC3D_DATA + 15, (((0x8000+ADDR_FBDELAYLINE_LEFT)<<16)));  //      cc_fbdelaylineleftwptr
                WriteDspCommDram32bit (ADDR_SRS_CC3D_DATA + 16, ((0x8000+ADDR_FFDELAYLINE_RIGHT+2000-96)<<16));  //      cc_delaylinerightblkrptr
                WriteDspCommDram32bit (ADDR_SRS_CC3D_DATA + 17, (((0x8000+ADDR_FBDELAYLINE_RIGHT)<<16))); //      cc_fbdelaylinerightwptr
                WriteDspCommDram32bit (ADDR_SRS_CC3D_DATA + 18, ((0x8000+ADDR_FBDELAYLINE_RIGHT+2000-32)<<16));  //      cc_fbdelaylinerightrptr
                WriteDspCommDram32bit (ADDR_SRS_CC3D_DATA + 19, ((0x8000+ADDR_FBDELAYLINE_RIGHT+2000-32)<<16));  //      cc_fbdelaylinerightprevrptr
                WriteDspCommDram32bit (ADDR_SRS_CC3D_DATA + 20, ((0x8000+ADDR_FBDELAYLINE_LEFT+2000-32)<<16));  //      cc_fbdelaylineleftrptr
                WriteDspCommDram32bit (ADDR_SRS_CC3D_DATA + 21, ((0x8000+ADDR_FBDELAYLINE_LEFT+2000-32)<<16));  //      cc_fbdelaylineleftprevrptr
                WriteDspCommDram (ADDR_SRS_CC3D_DATA + 22, 0x00000000);  //      cc_crossfadecnt        0
                WriteDspCommDram (ADDR_SRS_CC3D_DATA + 23, 0x00100000);  //      cc_frontright              0x00100000
                WriteDspCommDram32bit (ADDR_SRS_CC3D_DATA + 24, 0x30000000);  //      cc_delaylineaperture          (3.0,4)
                WriteDspCommDram32bit (ADDR_SRS_CC3D_DATA + 25, 0x7fffffff);  //      cc_frontleftc1
                WriteDspCommDram32bit (ADDR_SRS_CC3D_DATA + 26, 0x7fffffff);  //      cc_frontleftc2
                WriteDspCommDram32bit (ADDR_SRS_CC3D_DATA + 27, 0x7edb8b80);  //      cc_attacksmoothingcoeff         0.991075f,1
                WriteDspCommDram32bit (ADDR_SRS_CC3D_DATA + 28, 0x13333340);  //      cc_delaylinefarfielddepth
                WriteDspCommDram (ADDR_SRS_CC3D_DATA + 29, 0x00400000);  //      cc_heightleftscale                   0x00400000
                WriteDspCommDram (ADDR_SRS_CC3D_DATA + 30, 0x00000000);  //      cc_depthsumfrontprev             0
                WriteDspCommDram32bit (ADDR_SRS_CC3D_DATA + 31, 0x4ccccd00);  //      cc_delaylinenearfielddepth
                WriteDspCommDram32bit (ADDR_SRS_CC3D_DATA + 32, 0x01555555);  //      cc_invblksize
                WriteDspCommDram32bit (ADDR_SRS_CC3D_DATA + 33, 0x7fffffff);  //      cc_normlefta1
                WriteDspCommDram (ADDR_SRS_CC3D_DATA + 34, 0x00000100);  //      cc_frontleftciwl     0x00000100
                WriteDspCommDram (ADDR_SRS_CC3D_DATA + 35, 0x00000100);  //      cc_leftscaleiwl      0x00000100
                //WriteDspCommDram (ADDR_SRS_CC3D_DATA + 41, 0x00000100);  //      cc_depthchannelfadeenable    0x00000100
                WriteDspCommDram32bit (ADDR_SRS_CC3D_DATA + 36, 0x7ff49f80);  //      cc_decaysmoothingcoeff         0.9996528f,1
                WriteDspCommDram32bit (ADDR_SRS_CC3D_DATA + 37, 0x6fffffff);  //      cc_delaylinemaxgainlimit
                WriteDspCommDram (ADDR_SRS_CC3D_DATA + 38, 0x00400000);  //      cc_heightrightscale        0x00400000
                WriteDspCommDram32bit (ADDR_SRS_CC3D_DATA + 39, 0x00000000);  //      cc_leftscale          0
                WriteDspCommDram32bit (ADDR_SRS_CC3D_DATA + 40, 0x00100000);  //      cc_frontleft            0x00100000
                WriteDspCommDram32bit (ADDR_SRS_CC3D_DATA + 41, 0x00000000);  //      cc_rightscale        0
                WriteDspCommDram32bit (ADDR_SRS_CC3D_DATA + 42, 0x00000000);  //      cc_heightleft        0
                WriteDspCommDram32bit (ADDR_SRS_CC3D_DATA + 43, 0x00000000);  //      cc_heightright        0
                WriteDspCommDram32bit (ADDR_SRS_CC3D_DATA + 44, 0x0007d000);  //      k_cc3d_delaylinelen    2000
                WriteDspCommDram32bit (ADDR_SRS_CC3D_DATA + 45, 0x50000000);  //      K_const2_5f_3         SRS_FXP32(2.5f, 3)
                WriteDspCommDram32bit (ADDR_SRS_CC3D_DATA + 46, 0x0cccccd0);  //      K_const0_1f             SRS_FXP32(0.1f, 1)
                WriteDspCommDram32bit (ADDR_SRS_CC3D_DATA + 47, 0x199999a0);  //      K_const0_2f             SRS_FXP32(0.2f, 1)
                WriteDspCommDram32bit (ADDR_SRS_CC3D_DATA + 48, 0x26666680);  //      K_const0_3f             SRS_FXP32(0.3f, 1)
                WriteDspCommDram32bit (ADDR_SRS_CC3D_DATA + 49, 0x7fffffff);  //      K_const1_0f             0x7fffffff
                WriteDspCommDram32bit (ADDR_SRS_CC3D_DATA + 50, 0x59999980);  //      K_const0_7f             SRS_FXP32(0.7f, 1)
                WriteDspCommDram32bit (ADDR_SRS_CC3D_DATA + 51, 0x66666680);  //      K_const0_8f             SRS_FXP32(0.8f, 1)
                WriteDspCommDram32bit (ADDR_SRS_CC3D_DATA + 52, 0x73333300);  //      k_const0_9f_1        SRS_FXP32(0.9f, 1)
                WriteDspCommDram32bit (ADDR_SRS_CC3D_DATA + 53, 0x000029f1);  //      K_const1_0_5f          SRS_FXP32(1.0e-5f, 2)
                WriteDspCommDram32bit (ADDR_SRS_CC3D_DATA + 54, 0x14000000);  //      K_const1_25f_4       SRS_FXP32(1.25, 4)
                WriteDspCommDram32bit (ADDR_SRS_CC3D_DATA + 55, 0x03333333);  //      K_const0_2f_4           SRS_FXP32(0.2, 4)
                WriteDspCommDram32bit (ADDR_SRS_CC3D_DATA + 56, 0x00400000);  //      K_const1_0f_10       SRS_FXP32(1.0, 10)
                WriteDspCommDram32bit (ADDR_SRS_CC3D_DATA + 57, 0x311a587e);  //      K_const_a1
                WriteDspCommDram32bit (ADDR_SRS_CC3D_DATA + 58, 0x27277fc7);  //      K_const_a2
                WriteDspCommDram32bit (ADDR_SRS_CC3D_DATA + 59, 0x20000000);  //      K_const1_0f_3           0x20000000
                WriteDspCommDram32bit (ADDR_SRS_CC3D_DATA + 60, 0x10000000);  //      K_const1_0f_4           0x10000000
                WriteDspCommDram32bit (ADDR_SRS_CC3D_DATA + 61, 0x02000000);  //      K_const1_0f_7           0x02000000
                WriteDspCommDram32bit (ADDR_SRS_CC3D_DATA + 62, 0x20000000);  //      K_const0_5f_2           SRS_FXP32(0.5, 2)
                WriteDspCommDram32bit (ADDR_SRS_CC3D_DATA + 63, 0x0ccccccd);  //      K_const0_8f_4           SRS_FXP32(0.8, 4)
                WriteDspCommDram32bit (ADDR_SRS_CC3D_DATA + 64, 0x28f5c29a);  //      K_const0_32f_1           SRS_FXP32(0.8*0.4f, 1)
                WriteDspCommDram32bit (ADDR_SRS_CC3D_DATA + 65, 0x1eb851f3);  //      K_const0_24f_1           SRS_FXP32(1.2*0.4f*0.5f, 1)

                //WriteDspCommDram (ADDR_SRS_STTSHD_DATA + 0, 0x00000100);  //      cc_rdefen          1
                //WriteDspCommDram (ADDR_SRS_STTSHD_DATA + 1, 0x00000100);  //      cc_ldefen            1
                //WriteDspCommDram32bit (ADDR_SRS_STTSHD_DATA + 0, 0x33333333);  //      cc_lfoclevel               0x33333333
                WriteDspCommDram32bit (ADDR_SRS_STTSHD_DATA + 0, u4lfocuslevel<<16);  //      cc_lfoclevel               0x33333333
                //WriteDspCommDram32bit (ADDR_SRS_STTSHD_DATA + 1, 0x10002000);  //      cc_mixfadectrlext        0x10002000
                WriteDspCommDram32bit (ADDR_SRS_STTSHD_DATA + 1, u4mixfadectrlext);  //      cc_mixfadectrlext        0x10002000
                //WriteDspCommDram (ADDR_SRS_STTSHD_DATA + 4, 0x00000100);  //      cc_focus_enable          1
                //WriteDspCommDram32bit (ADDR_SRS_STTSHD_DATA + 2, 0x50a40000);  //      cc_frontspacectrl           50a4
                WriteDspCommDram32bit (ADDR_SRS_STTSHD_DATA + 2, u4frontspacectrl<<16);  //      cc_frontspacectrl           50a4
                //WriteDspCommDram32bit (ADDR_SRS_STTSHD_DATA + 3, 0x40000000);  //      cc_srs3dcenterctrl       SRS_FXP32(0.5, 1)
                WriteDspCommDram32bit (ADDR_SRS_STTSHD_DATA + 3, u4srs3dcenterctrl<<16);  //      cc_srs3dcenterctrl       SRS_FXP32(0.5, 1)
                //WriteDspCommDram32bit (ADDR_SRS_STTSHD_DATA + 4, 0x4ccd0000);  //      cc_trusurlevel        4ccd
                WriteDspCommDram32bit (ADDR_SRS_STTSHD_DATA + 4, u4trusurlevel<<16);  //      cc_trusurlevel        4ccd
                //WriteDspCommDram32bit (ADDR_SRS_STTSHD_DATA + 5, 0x0ccccccd);  //      cc_ldeflevel            0x0ccccccd
                WriteDspCommDram32bit (ADDR_SRS_STTSHD_DATA + 5, u4ldeflevel<<16);  //      cc_ldeflevel            0x0ccccccd
                WriteDspCommDram (ADDR_SRS_STTSHD_DATA + 6, u43dmode<<8);  //      cc_srs3dmode         SRS_SRS3D_STEREO
                //WriteDspCommDram32bit (ADDR_SRS_STTSHD_DATA + 7, 0x33333333);  //     cc_rfoclevel            0x33333333
                WriteDspCommDram32bit (ADDR_SRS_STTSHD_DATA + 7, u4rfocuslevel<<16);  //     cc_rfoclevel            0x33333333
                //WriteDspCommDram32bit (ADDR_SRS_STTSHD_DATA + 8, 0x2d70a3d7);  //     cc_frontcenterctrl               SRS_FXP32(0.355, 1)
                WriteDspCommDram32bit (ADDR_SRS_STTSHD_DATA + 8, u4frontcenctrl<<16);  //     cc_frontcenterctrl
                //WriteDspCommDram (ADDR_SRS_STTSHD_DATA + 12, 0x00000100);  //     cc_rfocen               1
                //WriteDspCommDram32bit (ADDR_SRS_STTSHD_DATA + 9, 0x20000000);  //     cc_mixfadectrl                SRS_FXP32(0.25, 1)
                WriteDspCommDram32bit (ADDR_SRS_STTSHD_DATA + 9, u4mixfadectrl<<16);  //     cc_mixfadectrl
                WriteDspCommDram (ADDR_SRS_STTSHD_DATA + 10, 0x00000100);  //     cc_rearctrliwl0            1
                //WriteDspCommDram (ADDR_SRS_STTSHD_DATA + 15, 0x00000100);  //     cc_lfocen                   1
                //WriteDspCommDram32bit (ADDR_SRS_STTSHD_DATA + 11, 0x33333333);  //     cc_focuslevel       0x33333333
                WriteDspCommDram32bit (ADDR_SRS_STTSHD_DATA + 11, u4focuslevel<<16);  //     cc_focuslevel       0x33333333
                //WriteDspCommDram (ADDR_SRS_STTSHD_DATA + 17, 0x00000100);  //     cc_surenable               1
                //WriteDspCommDram (ADDR_SRS_STTSHD_DATA + 18, 0x00000000);  //     cc_srs3dheadphone        0
                //WriteDspCommDram32bit (ADDR_SRS_STTSHD_DATA + 12, 0x66666680);  //     cc_srs3dspacectrl           SRS_FXP32(0.8f, 1)
                WriteDspCommDram32bit (ADDR_SRS_STTSHD_DATA + 12, u4srs3dspacectrl<<16);  //     cc_srs3dspacectrl           SRS_FXP32(0.8f, 1)
                //WriteDspCommDram32bit (ADDR_SRS_STTSHD_DATA + 13, 0x0ccccccd);  //     cc_rdeflevel                    0x0ccccccd
                WriteDspCommDram32bit (ADDR_SRS_STTSHD_DATA + 13, u4rdeflevel<<16);  //     cc_rdeflevel                    0x0ccccccd
                //WriteDspCommDram (ADDR_SRS_STTSHD_DATA + 21, 0x00000100);  //     cc_hpen                                   1
                WriteDspCommDram32bit (ADDR_SRS_STTSHD_DATA + 14, 0x40000000);  //     cc_frontrendergain1           0x40000000
                WriteDspCommDram32bit (ADDR_SRS_STTSHD_DATA + 15, 0x5a3d70a4);  //     cc_frontrendergain2           SRS_FXP32(0.63, 1)
                WriteDspCommDram (ADDR_SRS_STTSHD_DATA + 16, u4inmode<<8);  //     cc_inmode                             SRS_IO_RtLt   //SRS_IO_2_0_0
                //WriteDspCommDram (ADDR_SRS_STTSHD_DATA + 25, 0x00000100);  //     cc_mixenable                          1
                //WriteDspCommDram32bit (ADDR_SRS_STTSHD_DATA + 17, 0x2d70a3d7);  //     cc_rearspacectrl           0x2d70a3d7
                WriteDspCommDram32bit (ADDR_SRS_STTSHD_DATA + 17, u4rearspacectrl<<16);  //     cc_rearspacectrl           0x2d70a3d7
                //WriteDspCommDram (ADDR_SRS_STTSHD_DATA + 27, 0x00000100);  //     cc_tbhdenable                       1
                WriteDspCommDram32bit (ADDR_SRS_STTSHD_DATA + 18, u4tb_ctrl);  //     cc_tb_ctrl
                WriteDspCommDram (ADDR_SRS_STTSHD_DATA + 19, 0x00400000);  //     cc_LowPassMixGain
                WriteDspCommDram32bit (ADDR_SRS_STTSHD_DATA + 20, 0x02d70a3d);  //     cc_IntegrationConstant
                WriteDspCommDram32bit (ADDR_SRS_STTSHD_DATA + 21, 0x028f5c29);  //     cc_FeedbackConstant
                WriteDspCommDram32bit (ADDR_SRS_STTSHD_DATA + 22, u4compr_ctrl);  //     cc_IntegrationLimit
                //WriteDspCommDram (ADDR_SRS_STTSHD_DATA + 23, 0x00000000);  //     cc_RefLevel     
                WriteDspCommDram32bit (ADDR_SRS_STTSHD_DATA + 23, u4reflevel<<16);  //     cc_RefLevel   
                WriteDspCommDram (ADDR_SRS_STTSHD_DATA + 24, 0x00000000);  //     cc_FasdLevel_L
                WriteDspCommDram (ADDR_SRS_STTSHD_DATA + 25, 0x00000000);  //     cc_Integrator
                WriteDspCommDram (ADDR_SRS_STTSHD_DATA + 26, 0x00000000);  //     cc_FasdLevel_R
                WriteDspCommDram32bit (ADDR_SRS_STTSHD_DATA + 27, SRSCC3D_M_TBL[u4FSid][0]);  //     cc_Attack                       ?
                WriteDspCommDram32bit (ADDR_SRS_STTSHD_DATA + 28, SRSCC3D_M_TBL[u4FSid][1]);  //     cc_Decay                       ?
                WriteDspCommDram (ADDR_SRS_STTSHD_DATA + 29, 0x00000000);  //     cc_mPeakHold
                WriteDspCommDram (ADDR_SRS_STTSHD_DATA + 30, 0x00000000);  //     cc_mPeakHoldCount
                WriteDspCommDram (ADDR_SRS_STTSHD_DATA + 31, 0x00000000);  //     cc_mNextPeak
                WriteDspCommDram (ADDR_SRS_STTSHD_DATA + 32, 0x00040cf6);  //     cc_mPeak
                WriteDspCommDram (ADDR_SRS_STTSHD_DATA + 33, 0x00000000);  //     cc_mLastMoveUp
                WriteDspCommDram (ADDR_SRS_STTSHD_DATA + 34, 0x00000000);  //     cc_mValley
                WriteDspCommDram32bit (ADDR_SRS_STTSHD_DATA + 35, 0x00000960);  //     cc_mPeakSampleCount
                WriteDspCommDram32bit (ADDR_SRS_STTSHD_DATA + 36, 0x00fccccd);  //     cc_mMultiplier
                WriteDspCommDram (ADDR_SRS_STTSHD_DATA + 37, u4trubassflag<<8);  //     cc_trubassflag
                WriteDspCommDram32bit (ADDR_SRS_STTSHD_DATA + 38, u4compr_ctrl);  //     cc_mCompressorControl
                WriteDspCommDram32bit (ADDR_SRS_STTSHD_DATA + 39, 0x40000000);  //     cc_mInputGain
                WriteDspCommDram32bit (ADDR_SRS_STTSHD_DATA + 40, u4tbq<<16);  //     cc_tbq                                                                              
                WriteDspCommDram32bit (ADDR_SRS_STTSHD_DATA + 41, u4dculevel<<16);  //     cc_dculev                   
                
                WriteDspCommDram32bit (ADDR_SRS_STTSHD_DATA + 42, 0x040cf647);  //     kSrsTruBassInvHalfMinDynRng                                                                              
                WriteDspCommDram32bit (ADDR_SRS_STTSHD_DATA + 43, 0x00000015);  //     kSrsTruBassMinValley                                                                                    
                WriteDspCommDram32bit (ADDR_SRS_STTSHD_DATA + 44, 0x7e666666);  //     kSrsTruBassHalfMinDynRng                                                                                
                WriteDspCommDram32bit (ADDR_SRS_STTSHD_DATA + 45, 0x7fffffff);  //     kSrsTruBassMultiplierMax                                                                            
                WriteDspCommDram32bit (ADDR_SRS_STTSHD_DATA + 46, 0x00080000);  //     kSrsTruBassMultiplierMin                                                                            
                WriteDspCommDram32bit (ADDR_SRS_STTSHD_DATA + 47, 0x00000960);  //     kPeakSampleCount                                                                                     
                WriteDspCommDram32bit (ADDR_SRS_STTSHD_DATA + 48, 0x2b851ec0);  //     K_const0_34_f              SRS_FXP32(0.34f, 1)   
                WriteDspCommDram32bit (ADDR_SRS_STTSHD_DATA + 49, 0x23d70a40);  //     K_const0_56f_2             SRS_FXP32(0.56f, 2)                                                                              
                WriteDspCommDram32bit (ADDR_SRS_STTSHD_DATA + 50, 0x551eb880);  //     K_const0_665_f           SRS_FXP32(0.665f, 1)


                u4TblSrcAdr = (UINT32 *)&SRSCC3D_M_TBL[u4FSid][2];
                u4TblDstAdr  = ADDR_SRS_CC3D_LRENH_COEF;
                for (iLoopVar = 0; iLoopVar < 10; iLoopVar++)
                {
                    WriteDspCommDram32bit (u4TblDstAdr+iLoopVar, u4TblSrcAdr[iLoopVar]);
                }
                u4TblSrcAdr += 10;
                //
                u4TblDstAdr  = ADDR_SRS_CC3D_PERSPECTIVE_COEF;
                for (iLoopVar = 0; iLoopVar < 5; iLoopVar++)
                {
                    WriteDspCommDram32bit (u4TblDstAdr+iLoopVar, u4TblSrcAdr[iLoopVar]);
                }
                u4TblSrcAdr += 5;
                //
                u4TblDstAdr  = ADDR_SRS_CC3D_HP48_COEF;
                for (iLoopVar = 0; iLoopVar < 5; iLoopVar++)
                {
                    WriteDspCommDram32bit (u4TblDstAdr+iLoopVar, u4TblSrcAdr[iLoopVar]);
                }
                u4TblSrcAdr += 5;
                //
                u4TblDstAdr  = ADDR_SRS_CC3D_LP200_COEF;
                for (iLoopVar = 0; iLoopVar < 5; iLoopVar++)
                {
                    WriteDspCommDram32bit (u4TblDstAdr+iLoopVar, u4TblSrcAdr[iLoopVar]);
                }
                u4TblSrcAdr += 5;
                //
                u4TblDstAdr  = ADDR_SRS_CC3D_HP7K_COEF;
                for (iLoopVar = 0; iLoopVar < 5; iLoopVar++)
                {
                    WriteDspCommDram32bit (u4TblDstAdr+iLoopVar, u4TblSrcAdr[iLoopVar]);
                }
                u4TblSrcAdr += 5;
                //
                u4TblDstAdr  = ADDR_SRS_CC3D_FOCUS_COEF;
                for (iLoopVar = 0; iLoopVar < 5; iLoopVar++)
                {
                    WriteDspCommDram32bit (u4TblDstAdr+iLoopVar, u4TblSrcAdr[iLoopVar]);
                }
                u4TblSrcAdr += 5;
                //
                u4TblDstAdr  = ADDR_SRS_CC3D_HP13K_COEF;
                for (iLoopVar = 0; iLoopVar < 5; iLoopVar++)
                {
                    WriteDspCommDram32bit (u4TblDstAdr+iLoopVar, u4TblSrcAdr[iLoopVar]);
                }
                u4TblSrcAdr += 5;
                //
                u4TblDstAdr  = ADDR_SRS_CC3D_LP8K_COEF;
                for (iLoopVar = 0; iLoopVar < 5; iLoopVar++)
                {
                    WriteDspCommDram32bit (u4TblDstAdr+iLoopVar, u4TblSrcAdr[iLoopVar]);
                }
                u4TblSrcAdr += 5;
                //
                u4TblDstAdr  = ADDR_SRS_CC3D_LP950_COEF;
                for (iLoopVar = 0; iLoopVar < 5; iLoopVar++)
                {
                    WriteDspCommDram32bit (u4TblDstAdr+iLoopVar, u4TblSrcAdr[iLoopVar]);
                }
                u4TblSrcAdr += 5;
                //
                u4TblDstAdr  = ADDR_SRS_CC3D_SPACEHPF_COEF;
                for (iLoopVar = 0; iLoopVar < 5; iLoopVar++)
                {
                    WriteDspCommDram32bit (u4TblDstAdr+iLoopVar, u4TblSrcAdr[u4hpfsize*5 + iLoopVar]);
                }
                u4TblSrcAdr += 45;
                //
                u4TblDstAdr  = ADDR_SRS_CC3D_SPACEHPF_GAIN;
                WriteDspCommDram32bit (u4TblDstAdr, u4TblSrcAdr[u4hpfsize]);
                u4TblSrcAdr += 9;
                //
                u4TblDstAdr  = ADDR_SRS_CC3D_INIT_COEF;
                for (iLoopVar = 0; iLoopVar < 5; iLoopVar++)
                {
                    WriteDspCommDram32bit (u4TblDstAdr+iLoopVar, u4TblSrcAdr[u4inisize*5 + iLoopVar]);
                }
                u4TblSrcAdr += 20;
                //
                u4TblDstAdr  = ADDR_SRS_CC3D_FINAL_COEF;
                for (iLoopVar = 0; iLoopVar < 5; iLoopVar++)
                {
                    WriteDspCommDram32bit (u4TblDstAdr+iLoopVar, u4TblSrcAdr[iLoopVar]);
                }
                u4TblSrcAdr += 5;
                //
                u4TblDstAdr  = ADDR_SRS_CC3D_SIGNALALLPASS_COEF;
                for (iLoopVar = 0; iLoopVar < 5; iLoopVar++)
                {
                    WriteDspCommDram32bit (u4TblDstAdr+iLoopVar, u4TblSrcAdr[iLoopVar]);
                }
                u4TblSrcAdr += 5;
                //
                u4TblDstAdr  = ADDR_SRS_CC3D_SIGNALFIRST_COEF;
                for (iLoopVar = 0; iLoopVar < 5; iLoopVar++)
                {
                    WriteDspCommDram32bit (u4TblDstAdr+iLoopVar, u4TblSrcAdr[iLoopVar]);
                }
                u4TblSrcAdr += 5;
                //
                u4TblDstAdr  = ADDR_SRS_CC3D_SIGNALSECOND_COEF;
                for (iLoopVar = 0; iLoopVar < 5; iLoopVar++)
                {
                    WriteDspCommDram32bit (u4TblDstAdr+iLoopVar, u4TblSrcAdr[iLoopVar]);
                }
                u4TblSrcAdr += 5;
                //
                u4TblDstAdr  = ADDR_SRS_CC3D_SIGNALTHIRD_COEF;
                for (iLoopVar = 0; iLoopVar < 5; iLoopVar++)
                {
                    WriteDspCommDram32bit (u4TblDstAdr+iLoopVar, u4TblSrcAdr[iLoopVar]);
                }
                u4TblSrcAdr += 5;
                //
                u4TblDstAdr  = ADDR_SRS_CC3D_PHASE200_COEF;
                for (iLoopVar = 0; iLoopVar < 5; iLoopVar++)
                {
                    WriteDspCommDram32bit (u4TblDstAdr+iLoopVar, u4TblSrcAdr[iLoopVar]);
                }
                u4TblSrcAdr += 5;
                //
                u4TblDstAdr  = ADDR_SRS_CC3D_PHASE1675_COEF;
                for (iLoopVar = 0; iLoopVar < 5; iLoopVar++)
                {
                    WriteDspCommDram32bit (u4TblDstAdr+iLoopVar, u4TblSrcAdr[iLoopVar]);
                }
                u4TblSrcAdr += 5;
                //
                u4TblDstAdr  = ADDR_SRS_CC3D_PHASE18K_COEF;
                for (iLoopVar = 0; iLoopVar < 5; iLoopVar++)
                {
                    WriteDspCommDram32bit (u4TblDstAdr+iLoopVar, u4TblSrcAdr[iLoopVar]);
                }
                u4TblSrcAdr += 5;
                //
                u4TblDstAdr  = ADDR_SRS_CC3D_PHASE50_COEF;
                for (iLoopVar = 0; iLoopVar < 5; iLoopVar++)
                {
                    WriteDspCommDram32bit (u4TblDstAdr+iLoopVar, u4TblSrcAdr[iLoopVar]);
                }
                u4TblSrcAdr += 5;
                //
                u4TblDstAdr  = ADDR_SRS_CC3D_PHASE600_COEF;
                for (iLoopVar = 0; iLoopVar < 5; iLoopVar++)
                {
                    WriteDspCommDram32bit (u4TblDstAdr+iLoopVar, u4TblSrcAdr[iLoopVar]);
                }
                u4TblSrcAdr += 5;
                //
                u4TblDstAdr  = ADDR_SRS_CC3D_PHASE4850_COEF;
                for (iLoopVar = 0; iLoopVar < 5; iLoopVar++)
                {
                    WriteDspCommDram32bit (u4TblDstAdr+iLoopVar, u4TblSrcAdr[iLoopVar]);
                }
                u4TblSrcAdr += 5;
                //
                u4TblDstAdr  = ADDR_SRS_CC3D_LDEF_COEF;
                for (iLoopVar = 0; iLoopVar < 5; iLoopVar++)
                {
                    WriteDspCommDram32bit (u4TblDstAdr+iLoopVar, u4TblSrcAdr[iLoopVar]);
                }
                u4TblSrcAdr += 5;
                //
                u4TblDstAdr  = ADDR_SRS_CC3D_RDEF_COEF;
                for (iLoopVar = 0; iLoopVar < 5; iLoopVar++)
                {
                    WriteDspCommDram32bit (u4TblDstAdr+iLoopVar, u4TblSrcAdr[iLoopVar]);
                }
                u4TblSrcAdr += 5;
                //
                u4TblDstAdr  = ADDR_SRS_CC3D_LFOC_COEF;
                for (iLoopVar = 0; iLoopVar < 5; iLoopVar++)
                {
                    WriteDspCommDram32bit (u4TblDstAdr+iLoopVar, u4TblSrcAdr[iLoopVar]);
                }
                u4TblSrcAdr += 5;
                //
                u4TblDstAdr  = ADDR_SRS_CC3D_RFOC_COEF;
                for (iLoopVar = 0; iLoopVar < 5; iLoopVar++)
                {
                    WriteDspCommDram32bit (u4TblDstAdr+iLoopVar, u4TblSrcAdr[iLoopVar]);
                }
                u4TblSrcAdr += 5;

                #if 0
                if(u4anasize < 10)//if it is customsize,it will not write here
                {
                    //TB, sub TB
                    u4TblDstAdr  = ADDR_SRS_CC3D_TB_COEF;
                    for (iLoopVar = 0; iLoopVar < 15; iLoopVar++)
                    {
                        //WriteDspCommDram (u4TblDstAdr+iLoopVar, u4TblSrcAdr[u4spksize*15 + iLoopVar]);
                        WriteDspCommDram32bit (u4TblDstAdr+iLoopVar, u4TblSrcAdr[u4anasize*15 + iLoopVar]);
                    }

                #if 0
                    //Analysis Filter coeff
                    u4TblDstAdr  = ADDR_SRS_TSHD_TB_COEF + 15;
                    for (iLoopVar = 5; iLoopVar < 15; iLoopVar++)
                    {
                        //WriteDspCommDram (u4TblDstAdr+iLoopVar-5, u4TblSrcAdr[u4anasize*15 + iLoopVar]);
                        WriteDspCommDram32bit (u4TblDstAdr+iLoopVar-5, u4TblSrcAdr[u4anasize*15 + iLoopVar]);
                    }
                #endif
                }
                else if(u4anasize == 10)//custom speakersize
                {
                    //TB, sub TB
                    u4TblDstAdr  = ADDR_SRS_CC3D_TB_COEF;
                    for (iLoopVar = 0; iLoopVar < 15; iLoopVar++)
                    {
                        //WriteDspCommDram (u4TblDstAdr+iLoopVar, u4TblSrcAdr[u4spksize*15 + iLoopVar]);
                        WriteDspCommDram32bit (u4TblDstAdr+iLoopVar, SRSTB_CUSTOM_SPEAKER_TBL[iLoopVar]);
                    }

                }
                else
                {
                    ;
                }

                if(u4spksize < 10)//if it is customsize,it will not write here
                {
                    //Analysis Filter coeff
                    u4TblDstAdr  = ADDR_SRS_CC3D_TB_COEF + 15;

                #if 1
                    for (iLoopVar = 5; iLoopVar < 15; iLoopVar++)
                    {
                        //WriteDspCommDram (u4TblDstAdr+iLoopVar-5, u4TblSrcAdr[u4anasize*15 + iLoopVar]);
                        WriteDspCommDram32bit (u4TblDstAdr+iLoopVar-5, u4TblSrcAdr[u4spksize*15 + iLoopVar]);
                    }
                #endif
                }
                else if(u4spksize == 10)//custom ana speakersize
                {
                    u4TblDstAdr  = ADDR_SRS_CC3D_TB_COEF + 15;
                    for (iLoopVar = 0; iLoopVar < 10; iLoopVar++)
                    {
                        WriteDspCommDram32bit (u4TblDstAdr+iLoopVar, SRSTB_CUSTOM_SPEAKER_TBL[iLoopVar + 15]);
                    }

                }
                else
                {
                    ;
                }
                #endif

                if(u4anasize < 10)//if it is customsize,it will not write here
                {
                    //TB, sub TB
                    u4TblDstAdr  = ADDR_SRS_CC3D_TB_COEF;
                    //WriteDspCommDram (u4TblDstAdr+iLoopVar, u4TblSrcAdr[u4spksize*15 + iLoopVar]);
                    WriteDspCommDram32bit (u4TblDstAdr+0, u4TblSrcAdr[u4anasize*15 + 1]);
                    WriteDspCommDram32bit (u4TblDstAdr+1, u4TblSrcAdr[u4anasize*15 + 3]);
                    WriteDspCommDram32bit (u4TblDstAdr+2, u4TblSrcAdr[u4anasize*15 + 9]);
                    WriteDspCommDram32bit (u4TblDstAdr+3, u4TblSrcAdr[u4anasize*15 + 8]);
                    WriteDspCommDram32bit (u4TblDstAdr+4, u4TblSrcAdr[u4anasize*15 + 7]);
                    WriteDspCommDram32bit (u4TblDstAdr+5, u4TblSrcAdr[u4anasize*15 + 14]);
                    WriteDspCommDram32bit (u4TblDstAdr+6, u4TblSrcAdr[u4anasize*15 + 13]);
                    WriteDspCommDram32bit (u4TblDstAdr+7, u4TblSrcAdr[u4anasize*15 + 12]);

                }
                else if(u4anasize == 10)//custom speakersize
                {
                    //TB, sub TB
                    u4TblDstAdr  = ADDR_SRS_CC3D_TB_COEF;
                    WriteDspCommDram32bit (u4TblDstAdr+0, SRSTB_CUSTOM_SPEAKER_TBL[1]);
                    WriteDspCommDram32bit (u4TblDstAdr+1, SRSTB_CUSTOM_SPEAKER_TBL[3]);
                    WriteDspCommDram32bit (u4TblDstAdr+2, SRSTB_CUSTOM_SPEAKER_TBL[9]);
                    WriteDspCommDram32bit (u4TblDstAdr+3, SRSTB_CUSTOM_SPEAKER_TBL[8]);
                    WriteDspCommDram32bit (u4TblDstAdr+4, SRSTB_CUSTOM_SPEAKER_TBL[7]);
                    WriteDspCommDram32bit (u4TblDstAdr+5, SRSTB_CUSTOM_SPEAKER_TBL[14]);
                    WriteDspCommDram32bit (u4TblDstAdr+6, SRSTB_CUSTOM_SPEAKER_TBL[13]);
                    WriteDspCommDram32bit (u4TblDstAdr+7, SRSTB_CUSTOM_SPEAKER_TBL[12]);

                }
                else
                {
                    ;
                }

                if(u4spksize < 10)//if it is customsize,it will not write here
                {
                    u4TblDstAdr  = ADDR_SRS_CC3D_TB_COEF;
                    WriteDspCommDram32bit (u4TblDstAdr+8, u4TblSrcAdr[u4spksize*15 + 9]);
                    WriteDspCommDram32bit (u4TblDstAdr+9, u4TblSrcAdr[u4spksize*15 + 8]);
                    WriteDspCommDram32bit (u4TblDstAdr+10, u4TblSrcAdr[u4spksize*15 + 7]);
                    WriteDspCommDram32bit (u4TblDstAdr+11, u4TblSrcAdr[u4spksize*15 + 14]);
                    WriteDspCommDram32bit (u4TblDstAdr+12, u4TblSrcAdr[u4spksize*15 + 13]);
                    WriteDspCommDram32bit (u4TblDstAdr+13, u4TblSrcAdr[u4spksize*15 + 12]);

                }
                else if(u4spksize == 10)//custom ana speakersize
                {
                    u4TblDstAdr  = ADDR_SRS_CC3D_TB_COEF;
                    WriteDspCommDram32bit (u4TblDstAdr+8, SRSTB_CUSTOM_SPEAKER_TBL[9]);
                    WriteDspCommDram32bit (u4TblDstAdr+9, SRSTB_CUSTOM_SPEAKER_TBL[8]);
                    WriteDspCommDram32bit (u4TblDstAdr+10, SRSTB_CUSTOM_SPEAKER_TBL[7]);
                    WriteDspCommDram32bit (u4TblDstAdr+11, SRSTB_CUSTOM_SPEAKER_TBL[14]);
                    WriteDspCommDram32bit (u4TblDstAdr+12, SRSTB_CUSTOM_SPEAKER_TBL[13]);
                    WriteDspCommDram32bit (u4TblDstAdr+13, SRSTB_CUSTOM_SPEAKER_TBL[12]);
                }
                else
                {
                    ;
                }

                u4TblSrcAdr += 165;
                u4TblDstAdr  = ADDR_SRS_CC3D_DCU_COEF;
                for (iLoopVar = 0; iLoopVar < 5; iLoopVar++)
                {
                    if(u4spksize < 10)
                    {
                        WriteDspCommDram32bit (u4TblDstAdr+iLoopVar, u4TblSrcAdr[u4spksize*5 + iLoopVar]);
                    }
                    if(u4spksize == 10)
                    {
                        WriteDspCommDram32bit (u4TblDstAdr+iLoopVar, u4TblSrcAdr[iLoopVar]);
                    }
                }
                u4TblSrcAdr += 50;
                u4TblDstAdr  = ADDR_SRS_CC3D_DCU_GAIN;
                for (iLoopVar = 0; iLoopVar < 1; iLoopVar++)
                {
                    if(u4spksize < 10)
                    {
                        WriteDspCommDram32bit (u4TblDstAdr+iLoopVar, u4TblSrcAdr[u4spksize + iLoopVar]);
                    }
                    if(u4spksize == 10)
                    {
                        WriteDspCommDram32bit (u4TblDstAdr+iLoopVar, u4TblSrcAdr[iLoopVar]);
                    }
                }
            
                //init iir state
                for (iLoopVar=0;iLoopVar<(252);iLoopVar++)
                {
                    WriteDspCommDram (ADDR_SRS_CC3D_STATE + iLoopVar, 0);
                }
                //init ADDR_LMT_LZTBL
                for (iLoopVar=0;iLoopVar<(256);iLoopVar++)
                {
                    WriteDspCommDram (ADDR_LMT_LZTBL + iLoopVar,  srs_lzTbl[iLoopVar]<<8);
                }
                //init ADDR_POW2_TBL
                for (iLoopVar = 0; iLoopVar < 120; iLoopVar++)
                {
                    WriteDspCommDram32bit (ADDR_POW2_TBL+iLoopVar, srs_Pow2Tbl[0][iLoopVar]);

                }
                //init delay line
                for (iLoopVar = 0; iLoopVar < 2000*4; iLoopVar++)
                {
                    WriteDspCommDram32bit (ADDR_FFDELAYLINE_LEFT+iLoopVar, 0);
                }


                if (_rDspVars[AUD_DEC_MAIN].bCode3D != CODE3D_SRS_TSXT)
                {
                    _rDspVars[AUD_DEC_MAIN].bCode3D = CODE3D_SRS_TSXT;
                }
                //Printf("fubofeng cc3d init \n");    
            }
#endif //DSP_SUPPORT_SRSCC3D

    }
    else
    {
        // second decoder
    }

    // Just fix warning message for -O2
    UNUSED(dwOfst);
    UNUSED(dwSz);
    UNUSED(u4FreqIdx);
    UNUSED(iLoopVar);
#endif //DOWNMIX_SUPPORT

}

#if 1///def CC_VORBIS_SUPPORT

/* DSP Dram Space Definition */
/* decoder page 0x0000 ~ 0x0400 */
#define ADDR_VBSD2RC_DRAM_GENERAL_INFO                   0x0000        // 0x8000
    #define ADDR_VBSD2RC_DRAM_VORBIS_CODEBOOK_COUNT      (ADDR_VBSD2RC_DRAM_GENERAL_INFO+0x00)
#define ADDR_VBSD2RC_DRAM_ID_HEADER                      0x0020        // 0x8020
#define ADDR_VBSD2RC_DRAM_SETUP_HEADER                   0x0030        // 0x8030
#define ADDR_VBSD2RC_DRAM_CODEBOOK_PTR_ADR               0x0040        // 0x8040
#define ADDR_VBSD2RC_DRAM_COMMENT_HEADER                 0x0100        // 0x8100
#define ADDR_VBSD2RC_DRAM_TEMP_CODEBOOK_STACK            0x0180        // 0x8180 (0x0020)
/// Compact Dram, borrow dram for RISC calculate.
//#define ADDR_VBSD2RC_DRAM_TEMP_CODEBOOK_CODE             0x4000        // 0xc000 (0x2000)
//#define ADDR_VBSD2RC_DRAM_TEMP_CODEBOOK_VALUE            0x6000        // 0xe000 (0x2000)
/// Use OUTPUT_BUF_ADR dram. (016B00-8000 = EB00)
#define ADDR_VBSD2RC_DRAM_TEMP_CODEBOOK_CODE             0xEB00        // 0x16B00 (0x2000)
#define ADDR_VBSD2RC_DRAM_TEMP_CODEBOOK_VALUE            0x10B00       // 0x18B00 (0x2000)

/* For Function Declaration */
BOOL fgConstructHuffmanTree(UINT32 dwBookAdr, UINT32 dwBookLen) ;
BOOL fgMakeHuffmanCode(UINT32 dwBookAdr, UINT32 dwBookLen);
void vVorbisQuickSort(UINT32 dwValueAdr, UINT32 dwCodeAdr, UINT32 dwBookLen) ;

/* For Huffman Tree Construction */
#define LEFT_TREE   0
#define RIGHT_TREE  1

/* For Quick Sort Data */
#define BOOK_STACK_DEPTH  32

typedef struct QuickSortStack
{
  INT32 iBookStackTop ;
  UINT32 dwBookStackAdr ;
} QuickSortStack ;

/* For Time Consuming Computation */
#define VORBIS_HUFF_DEBUG   0
#if (VORBIS_HUFF_DEBUG)
UINT32  dwStcCount1, dwStcCount2 ;
#endif

#define FEW_DRAM_FLUSH
#define DUAL_DEC_DRAM_ACCESS
#ifdef DUAL_DEC_DRAM_ACCESS
UINT32 VorbisReadDecDram(UINT32 u4Addr)
{
    UINT32 u4Data = 0;
    if(_uVorbisIntDec == 1)
    {
        #ifdef FEW_DRAM_FLUSH
        u4Data = DspDecCacheDramData(AUD_DSP0, u4Addr);
        #else
        ReadDspDecCacheDram(AUD_DSP0, u4Addr, u4Data);
        #endif
    }
    else if(_uVorbisIntDec == 2)
    {
        #ifdef FEW_DRAM_FLUSH
        u4Data = DspDec2CacheDramData(AUD_DSP0, u4Addr);
        #else
        ReadDspDec2CacheDram(AUD_DSP0, u4Addr, u4Data);
        #endif
    }
    else if(_uVorbisIntDec == 3)
    {
        #ifdef FEW_DRAM_FLUSH
        u4Data = DspDec3CacheDramData(AUD_DSP0, u4Addr);
        #else
        ReadDspDec3CacheDram(AUD_DSP0, u4Addr, u4Data);
        #endif
    }
    else
    {
        LOG(0, "Read Dec Dram - _uVorbisIntDec = 0 !!!!!! \n");
        ASSERT(0);
    }
    return (u4Data);
}

VOID VorbisWriteDecDram(UINT32 u4Addr, UINT32 u4Data)
{
    if(_uVorbisIntDec == 1)
    {
        #ifdef FEW_DRAM_FLUSH
        DspDecCacheDramWriteData(AUD_DSP0, u4Addr, u4Data);
        #else
        WriteDspDecCacheDram(AUD_DSP0, u4Addr, u4Data);
        #endif
    }
    else if(_uVorbisIntDec == 2)
    {
        #ifdef FEW_DRAM_FLUSH
        DspDec2CacheDramWriteData(AUD_DSP0, u4Addr, u4Data);
        #else
        WriteDspDec2CacheDram(AUD_DSP0, u4Addr, u4Data);
        #endif
    }
    else if(_uVorbisIntDec == 3)
    {
        #ifdef FEW_DRAM_FLUSH
        DspDec3CacheDramWriteData(AUD_DSP0, u4Addr, u4Data);
        #else
        WriteDspDec3CacheDram(AUD_DSP0, u4Addr, u4Data);
        #endif
    }
    else
    {
        LOG(0, "Write Dec Dram - _uVorbisIntDec = 0 !!!!!! \n");
        ASSERT(0);
    }
}

#if 0   // Unused
///Without DRAM Flush
UINT32 VorbisDecDramData(UINT32 u4Addr)
{
    UINT32 u4Data = 0;
    if(_uVorbisIntDec == 1)
    {
        u4Data = DspDecCacheDramData(AUD_DSP0, u4Addr);
    }
    else if(_uVorbisIntDec == 2)
    {
        u4Data = DspDec2CacheDramData(AUD_DSP0, u4Addr);
    }
    else if(_uVorbisIntDec == 3)
    {
        u4Data = DspDec3CacheDramData(AUD_DSP0, u4Addr);
    }
    else
    {
        LOG(0, "Read2 Dec Dram Data- _uVorbisIntDec = 0 !!!!!! \n");
        ASSERT(0);
    }
    return (u4Data);
}
#endif

///for MT5368/96 use
///Some case dram= 0012 0034
///DRAM read  = 00 001200 (already >>8)
///DRAM write = 0012 0000 (already <<8)
UINT32 VorbisReadDecDram32(UINT32 u4Addr)
{
    UINT32 u4Data = 0;
    if(_uVorbisIntDec == 1)
    {
        #ifdef FEW_DRAM_FLUSH
        u4Data = DspDecCacheDramData32(AUD_DSP0, u4Addr);
        #else
        ReadDspDecCacheDram32(AUD_DSP0, u4Addr, u4Data);
        #endif
    }
    else if(_uVorbisIntDec == 2)
    {
        #ifdef FEW_DRAM_FLUSH
        u4Data = DspDec2CacheDramData32(AUD_DSP0, u4Addr);
        #else
        ReadDspDec2CacheDram32(AUD_DSP0, u4Addr, u4Data);
        #endif
    }
    else if(_uVorbisIntDec == 3)
    {
        #ifdef FEW_DRAM_FLUSH
        u4Data = DspDec3CacheDramData32(AUD_DSP0, u4Addr);
        #else
        ReadDspDec3CacheDram32(AUD_DSP0, u4Addr, u4Data);
        #endif
    }
    else
    {
        LOG(0, "Read Dec Dram32 - _uVorbisIntDec = 0 !!!!!! \n");
        ASSERT(0);
    }
    return (u4Data);
}

VOID VorbisWriteDecDram32(UINT32 u4Addr, UINT32 u4Data)
{
    if(_uVorbisIntDec == 1)
    {
        #ifdef FEW_DRAM_FLUSH
        DspDecCacheDramWriteData32(AUD_DSP0, u4Addr, u4Data);
        #else
        WriteDspDecCacheDram32(AUD_DSP0, u4Addr, u4Data);
        #endif
    }
    else if(_uVorbisIntDec == 2)
    {
        #ifdef FEW_DRAM_FLUSH
        DspDec2CacheDramWriteData32(AUD_DSP0, u4Addr, u4Data);
        #else
        WriteDspDec2CacheDram32(AUD_DSP0, u4Addr, u4Data);
        #endif
    }
    else if(_uVorbisIntDec == 3)
    {
        #ifdef FEW_DRAM_FLUSH
        DspDec3CacheDramWriteData32(AUD_DSP0, u4Addr, u4Data);
        #else
        WriteDspDec3CacheDram32(AUD_DSP0, u4Addr, u4Data);
        #endif
    }
    else
    {
        LOG(0, "Write Dec Dram32 - _uVorbisIntDec = 0 !!!!!! \n");
        ASSERT(0);
    }
}

///Without DRAM Flush
UINT32 VorbisDecDramData32(UINT32 u4Addr)
{
    UINT32 u4Data = 0;
    if(_uVorbisIntDec == 1)
    {
        u4Data = DspDecCacheDramData32(AUD_DSP0, u4Addr);
    }
    else if(_uVorbisIntDec == 2)
    {
        u4Data = DspDec2CacheDramData32(AUD_DSP0, u4Addr);
    }
    else if(_uVorbisIntDec == 3)
    {
        u4Data = DspDec3CacheDramData32(AUD_DSP0, u4Addr);
    }
    else
    {
        LOG(0, "Read2 Dec Dram32 Data- _uVorbisIntDec = 0 !!!!!! \n");
        ASSERT(0);
    }
    return (u4Data);
}
#endif  ///DUAL_DEC_DRAM_ACCESS

/***************************************************************************
     Function : fgMakeVorbisCodebook
  Description : Construct Vorbis Codebook (Huffman Tree)
    Parameter : None
    Return    : TRUE/FALSE
***************************************************************************/
BOOL fgMakeVorbisCodebook(void)
{
  BOOL result = FALSE;
  UINT16 wBookNum ;
  UINT32 dwBookAdr ;
  UINT32 dwBookLen ;
  UINT16 iLoopVar ;
#ifdef VORBIS_HUFF_DEBUG
  UINT32 dwStcStart ;
  UINT32 dwStcEnd ;
  UNUSED(dwStcStart);
  UNUSED(dwStcEnd);
#endif

#ifdef FEW_DRAM_FLUSH   ///Before read dram from dsp
 DSP_FlushInvalidateDCache(AUD_DSP0, CACHE_FLUSH_TYPE_DATA, TYPE_DEC1_DRAM_IDX);
 DSP_FlushInvalidateDCache(AUD_DSP0, CACHE_FLUSH_TYPE_DATA, TYPE_DEC2_DRAM_IDX);
 DSP_FlushInvalidateDCache(AUD_DSP0, CACHE_FLUSH_TYPE_DATA, TYPE_COMM_DRAM_IDX);
#endif

#ifdef DUAL_DEC_DRAM_ACCESS
  wBookNum = VorbisReadDecDram((UINT32)ADDR_VBSD2RC_DRAM_VORBIS_CODEBOOK_COUNT);
#else
  ReadDspDecCacheDram((AUD_DSP0, UINT32)ADDR_VBSD2RC_DRAM_VORBIS_CODEBOOK_COUNT, wBookNum) ;
#endif

  wBookNum = (wBookNum >> 8) + 1 ;

  for( iLoopVar = 0 ; iLoopVar < wBookNum ; iLoopVar ++ )
  {
    /// Dsp set compact write address and length
    /// Risc make huffman code and write to dram.
#ifdef DUAL_DEC_DRAM_ACCESS
    dwBookAdr = VorbisReadDecDram((UINT32)(ADDR_VBSD2RC_DRAM_CODEBOOK_PTR_ADR+iLoopVar*2)) ;
    dwBookLen = VorbisReadDecDram((UINT32)(ADDR_VBSD2RC_DRAM_CODEBOOK_PTR_ADR+iLoopVar*2+1)) ;
#else
    ReadDspDecCacheDram(AUD_DSP0, (UINT32)(ADDR_VBSD2RC_DRAM_CODEBOOK_PTR_ADR+iLoopVar*2), dwBookAdr) ;
    ReadDspDecCacheDram(AUD_DSP0, (UINT32)(ADDR_VBSD2RC_DRAM_CODEBOOK_PTR_ADR+iLoopVar*2+1), dwBookLen) ;
#endif
    dwBookLen = dwBookLen >> 8 ;
    result = fgMakeHuffmanCode(dwBookAdr, dwBookLen) ;
    if(result)
    {
      if( dwBookLen )
      {
#if (VORBIS_HUFF_DEBUG==2)
        dwStcCount1 = 0 ;
        dwStcCount2 = 0 ;
#endif
        result = fgConstructHuffmanTree(dwBookAdr, dwBookLen) ;
#if (VORBIS_HUFF_DEBUG==2)
#ifdef DUAL_DEC_DRAM_ACCESS
        VorbisWriteDecDram((UINT32)(ADDR_VBSD2RC_DRAM_COMMENT_HEADER+0+iLoopVar), dwStcCount1) ;
        VorbisWriteDecDram((UINT32)(ADDR_VBSD2RC_DRAM_COMMENT_HEADER+64+iLoopVar), dwStcCount2) ;
#else
        WriteDspDecCacheDram(AUD_DSP0, (UINT32)(ADDR_VBSD2RC_DRAM_COMMENT_HEADER+0+iLoopVar), dwStcCount1) ;
        WriteDspDecCacheDram(AUD_DSP0, (UINT32)(ADDR_VBSD2RC_DRAM_COMMENT_HEADER+64+iLoopVar), dwStcCount2) ;
#endif
#endif
        if(!result)
          break ;
      }
    }
    else
    {
      break ;
    }
  }

#ifdef FEW_DRAM_FLUSH   ///After write dram to dsp
 DSP_FlushInvalidateDCache(AUD_DSP0, CACHE_FLUSH_TYPE_DATA, TYPE_DEC1_DRAM_IDX);
 DSP_FlushInvalidateDCache(AUD_DSP0, CACHE_FLUSH_TYPE_DATA, TYPE_DEC2_DRAM_IDX);
 DSP_FlushInvalidateDCache(AUD_DSP0, CACHE_FLUSH_TYPE_DATA, TYPE_COMM_DRAM_IDX);
#endif

  return(result);
}

/***************************************************************************
     Function : fgMakeHuffmanCode
  Description : Make Huffman Codeword
    Parameter : Huffman Data Address, Huffman Data Length
    Return    : TRUE/FALSE
***************************************************************************/
BOOL fgMakeHuffmanCode(UINT32 dwBookAdr, UINT32 dwBookLen)
{
  UINT32 dwData ;
  UINT32 dwEntry ;
  UINT32 dwMarker[33] ;
  UINT32 iLoopVar, jLoopVar ;
  UINT32 dwValueAdr ;
  UINT32 dwCodeAdr ;
  UINT32 dwCodeLen ;
  UINT32 dwCodeCount ;
  dwCodeCount = 0 ;

  /* initial dwMarkder to zero, clean dwMarker[33]=0 */
  for( iLoopVar = 0 ; iLoopVar < 33 ; iLoopVar ++ )
  {
    *(dwMarker+iLoopVar) = 0 ;
  }

  /* copy the codebook code length & value to temp DRAM value */
  for( iLoopVar = 0 ; iLoopVar < dwBookLen ; iLoopVar ++ )
  {
#ifdef DUAL_DEC_DRAM_ACCESS
    dwData = VorbisReadDecDram32(dwBookAdr+iLoopVar) ;
    VorbisWriteDecDram32((UINT32)ADDR_VBSD2RC_DRAM_TEMP_CODEBOOK_VALUE+iLoopVar, dwData) ;
#else
    ReadDspDecCacheDram(AUD_DSP0, dwBookAdr+iLoopVar, dwData) ;
    WriteDspDecCacheDram(AUD_DSP0, (UINT32)ADDR_VBSD2RC_DRAM_TEMP_CODEBOOK_VALUE+iLoopVar, dwData) ;
#endif
  }

  dwValueAdr = (UINT32)ADDR_VBSD2RC_DRAM_TEMP_CODEBOOK_VALUE ;
  dwCodeAdr = (UINT32)ADDR_VBSD2RC_DRAM_TEMP_CODEBOOK_CODE ;

  for( iLoopVar = 0 ; iLoopVar < dwBookLen ; iLoopVar ++ )
  {
#ifdef DUAL_DEC_DRAM_ACCESS
    dwCodeLen = VorbisReadDecDram32(dwValueAdr+iLoopVar) ;
#else
    ReadDspDecCacheDram(AUD_DSP0, dwValueAdr+iLoopVar, dwCodeLen) ;
#endif
    // hi-16 = value, lo-16 = code length
    dwCodeLen = dwCodeLen & 0xffff ;
    if(( dwCodeLen > 0 ) && ( dwCodeLen < 33 ))
    {
        dwEntry = *(dwMarker+dwCodeLen) ;
        /* update ourself */
        if( dwCodeLen < 32 && (dwEntry >> dwCodeLen) )
        {
            /* error condition; the lengths must specify an overpopulated tree */
            return(FALSE);
        }

        /* bitreverse the words because our bitwise packer/unpacker is LSb
           endian */
#ifdef DUAL_DEC_DRAM_ACCESS
        VorbisWriteDecDram32(dwCodeAdr+dwCodeCount, dwEntry << (32-dwCodeLen)) ;
#else
        WriteDspDecCacheDram(AUD_DSP0, dwCodeAdr+dwCodeCount, dwEntry << (32-dwCodeLen)) ;
#endif
        dwCodeCount ++ ;

        /* Look to see if the next shorter marker points to the node
        above. if so, update it and repeat.  */
        for( jLoopVar = dwCodeLen ; jLoopVar > 0 ; jLoopVar -- )
        {
            if( *(dwMarker+jLoopVar) & 1 )
            {
                /* have to jump branches */
                if( jLoopVar == 1 )
                    *(dwMarker+1) += 1 ;
                else
                    *(dwMarker+jLoopVar) = *(dwMarker+jLoopVar-1) << 1 ;
                break; /* invariant says next upper marker would already
                          have been moved if it was on the same path */
            }
            *(dwMarker+jLoopVar) += 1 ;
        }

        /* prune the tree; the implicit invariant says all the longer
           markers were dangling from our just-taken node.  Dangle them
           from our *new* node. */
        for( jLoopVar = dwCodeLen + 1 ; jLoopVar < 33 ; jLoopVar ++ )
        {
            if( (*(dwMarker+jLoopVar) >> 1) == dwEntry)
            {
                dwEntry = *(dwMarker+jLoopVar);
                *(dwMarker+jLoopVar) = *(dwMarker+jLoopVar-1) << 1 ;
            }
            else
            {
                break;
            }
        }
    }
  }

  return(TRUE);
}

/***************************************************************************
     Function : vWriteHuffTree
  Description : Write the Huffman Tree Data to Specified DRAM
    Parameter :
    Return    : None
***************************************************************************/
void vWriteHuffTree(UINT32 dwOutAdr, UINT32 *dwOutData, UINT32 dwInData, UINT32 *dwOutIndx)
{
  if( (*dwOutIndx)&1 )                          // second word in UINT32
  {
#ifdef DUAL_DEC_DRAM_ACCESS
    VorbisWriteDecDram32(dwOutAdr+(*dwOutIndx>>1), (*dwOutData | ( dwInData << 16))) ;
#else
    WriteDspDecCacheDram(AUD_DSP0, dwOutAdr+(*dwOutIndx>>1), (*dwOutData | ( dwInData << 16))) ;
#endif
  }
  else                                          // first word in UINT32
  {
    *dwOutData = dwInData & 0xffff ;
  }
  (*dwOutIndx) ++ ;
}

#define LEFT_TREE   0
#define RIGHT_TREE  1

/***************************************************************************
     Function : fgConstructHuffmanTree
  Description : Huffman Tree Construction
    Parameter : Huffman Data Address, Huffman Data Length
    Return    : TRUE/FALSE
***************************************************************************/
BOOL fgConstructHuffmanTree(UINT32 dwBookAdr, UINT32 dwBookLen)
{
  UINT32 dwTreeIndx=0, dwIndx ;
  UINT32 jLoopVar ;
  UINT8 bBranch ;
  UINT32 dwLeaf, dwMask, dwTmpCode; ///[Joel] no use, dwCodeIndx ;
  UINT32 dwLenNow, dwCodeNow ;
  UINT32 dwValueAdr, dwCodeAdr  ;
  UINT32 dwCodeLen, dwCodeValue, dwCode ;
  UINT32 dwTreeData, dwData ;
#if (VORBIS_HUFF_DEBUG==2)
  UINT32  dwStcStart, dwStcEnd ;
#endif

  bBranch = LEFT_TREE ;
  dwLenNow = 1 ;
  dwCodeNow = 0 ;
  dwValueAdr = (UINT32)ADDR_VBSD2RC_DRAM_TEMP_CODEBOOK_VALUE ;
  dwCodeAdr = (UINT32)ADDR_VBSD2RC_DRAM_TEMP_CODEBOOK_CODE ;
  if( dwBookLen > 1 )
  {

#if (VORBIS_HUFF_DEBUG==2)
    dwStcStart = dPRGetStcS() ;
#endif
    // sort the huffman code first
    vVorbisQuickSort(dwValueAdr, dwCodeAdr, dwBookLen) ;
#if (VORBIS_HUFF_DEBUG==2)
    dwStcEnd = dPRGetStcS() ;
    dwStcCount1 += (dwStcEnd-dwStcStart) ;
#endif

    dwTreeIndx = 0 ;
    dwIndx = 0 ;
#ifdef DUAL_DEC_DRAM_ACCESS
    dwCode = VorbisReadDecDram32(dwCodeAdr+0) ;
    dwCodeLen = VorbisReadDecDram32(dwValueAdr+0) ;
#else
    ReadDspDecCacheDram(AUD_DSP0, dwCodeAdr+0, dwCode) ;
    ReadDspDecCacheDram(AUD_DSP0, dwValueAdr+0, dwCodeLen) ;
#endif
    dwCodeLen &= 0xffff ;
#if (VORBIS_HUFF_DEBUG==2)
    dwStcStart = dPRGetStcS() ;
#endif
    while( dwIndx < dwBookLen )
    {
      if( bBranch == LEFT_TREE )
      {   // branch to LEFT_TREE
        // check if code match & check if length match
        if( (dwCodeNow == dwCode) && (dwLenNow == dwCodeLen) )
        {
          // match --> leaf
          bBranch = RIGHT_TREE ;
          dwTmpCode = 1 << (32-dwLenNow) ;
          // huff_tree[tree_index++] = 1 ;
          vWriteHuffTree(dwBookAdr, &dwTreeData, (UINT32)1, &dwTreeIndx) ;
          // huff_tree[tree_index++] = -(ogg_int32_t)huff_value[code_index] ;
    #ifdef DUAL_DEC_DRAM_ACCESS
          dwCodeValue = VorbisReadDecDram32(dwValueAdr+dwIndx) ;
    #else
          ReadDspDecCacheDram(AUD_DSP0, dwValueAdr+dwIndx, dwCodeValue) ;
    #endif
          dwCodeValue = -(dwCodeValue >> 16) & 0xffff ;
          vWriteHuffTree(dwBookAdr, &dwTreeData, dwCodeValue, &dwTreeIndx) ;
          dwCodeNow += dwTmpCode ;
          dwIndx ++ ;
    #ifdef DUAL_DEC_DRAM_ACCESS
          dwCode = VorbisReadDecDram32(dwCodeAdr+dwIndx) ;
          dwCodeLen = VorbisReadDecDram32(dwValueAdr+dwIndx) ;
    #else
          ReadDspDecCacheDram(AUD_DSP0, dwCodeAdr+dwIndx, dwCode) ;
          ReadDspDecCacheDram(AUD_DSP0, dwValueAdr+dwIndx, dwCodeLen) ;
    #endif
          dwCodeLen &= 0xffff ;
        }
        else
        {
          // internal node --> count the leafs below this node
          bBranch = LEFT_TREE ;
          dwMask = ((1<<dwLenNow) - 1) << (32-dwLenNow) ;
          dwLeaf = 0 ;
          jLoopVar = dwIndx ;
          do
          {
            dwLeaf ++ ;
            jLoopVar ++ ;
    #ifdef DUAL_DEC_DRAM_ACCESS
            dwData = VorbisReadDecDram32(dwCodeAdr+jLoopVar) ;
    #else
            ReadDspDecCacheDram(AUD_DSP0, dwCodeAdr+jLoopVar, dwData) ;
    #endif
          } while( (dwData&dwMask) == dwCodeNow ) ;
          //huff_tree[tree_index++] = (leaf<<1)-1 ;
          vWriteHuffTree(dwBookAdr, &dwTreeData, (dwLeaf<<1)-1, &dwTreeIndx) ;
          dwLenNow ++ ;
        }
      }
      else  ///RIGHT_TREE
      {                  // branch to RIGHT_TREE
        if( (dwCodeNow == dwCode) && (dwLenNow == dwCodeLen) )
        {
          // match --> leaf
          bBranch = RIGHT_TREE ;            // next branch to right
          //huff_tree[tree_index++] = -((Vbook[i].dwValLen>>16)&0xffff) ;
    #ifdef DUAL_DEC_DRAM_ACCESS
          dwCodeValue = VorbisReadDecDram32(dwValueAdr+dwIndx) ;
    #else
          ReadDspDecCacheDram(AUD_DSP0, dwValueAdr+dwIndx, dwCodeValue) ;
    #endif
          dwCodeValue = -(dwCodeValue >> 16) & 0xffff ;
          vWriteHuffTree(dwBookAdr, &dwTreeData, dwCodeValue, &dwTreeIndx) ;
          dwTmpCode = dwCodeNow >> (32-dwLenNow) ;
          while( dwTmpCode & 0x1 )
          {
            dwTmpCode >>= 1 ;
            dwLenNow -- ;
          }
          dwTmpCode ++ ;
          dwCodeNow = dwTmpCode << (32-dwLenNow) ;
          dwIndx ++ ;
    #ifdef DUAL_DEC_DRAM_ACCESS
          dwCode = VorbisReadDecDram32(dwCodeAdr+dwIndx) ;
          dwCodeLen = VorbisReadDecDram32(dwValueAdr+dwIndx) ;
    #else
          ReadDspDecCacheDram(AUD_DSP0, dwCodeAdr+dwIndx, dwCode) ;
          ReadDspDecCacheDram(AUD_DSP0, dwValueAdr+dwIndx, dwCodeLen) ;
    #endif
          dwCodeLen &= 0xffff ;
        }
        else
        {
          bBranch = LEFT_TREE ;
          dwLenNow ++ ;
        }
      }
    } // end of while( dwIndx < dwBookLen )
    if( dwTreeIndx & 0x1 )
    {
      // make a UINT32 to write --> hufftree entries = 2*n-1
      vWriteHuffTree(dwBookAdr, &dwTreeData, (UINT32)0, &dwTreeIndx) ;
    }
    else
    {
      /* there is a error condition */
      return(FALSE) ;
    }
  }   // end of if( dwBookLen > 1 )
  else
  {
    // huff_tree[tree_index] = 0 ;
    vWriteHuffTree(dwBookAdr, &dwTreeData, (UINT32)0, &dwTreeIndx) ;
  }
#if (VORBIS_HUFF_DEBUG==2)
    dwStcEnd = dPRGetStcS() ;
    dwStcCount2 += (dwStcEnd-dwStcStart) ;
#endif

  return(TRUE) ;
}


/***************************************************************************
     Function : vVorbisExchange
  Description : exchange Value & Code data of index i & j
    Parameter :
    Return    :
***************************************************************************/
void vVorbisExchange(UINT32 dwValueAdr, UINT32 dwCodeAdr, UINT32 i, UINT32 j)
{
    UINT32 temp ;
#ifdef DUAL_DEC_DRAM_ACCESS
    // exchange code
    temp = VorbisReadDecDram32(dwCodeAdr+i) ;
    VorbisWriteDecDram32(dwCodeAdr+i, VorbisDecDramData32(dwCodeAdr + j)) ;
    VorbisWriteDecDram32(dwCodeAdr+j, temp) ;
    // exchange value
    temp = VorbisReadDecDram32(dwValueAdr+i) ;
    VorbisWriteDecDram32(dwValueAdr+i, VorbisDecDramData32(dwValueAdr + j)) ;
    VorbisWriteDecDram32(dwValueAdr+j, temp) ;
#else
    // exchange code
    ReadDspDecCacheDram(AUD_DSP0, dwCodeAdr+i, temp) ;
    WriteDspDecCacheDram(AUD_DSP0, dwCodeAdr+i, DspDecCacheDramData(dwCodeAdr + j)) ;
    WriteDspDecCacheDram(AUD_DSP0, dwCodeAdr+j, temp) ;
    // exchange value
    ReadDspDecCacheDram(AUD_DSP0, dwValueAdr+i, temp) ;
    WriteDspDecCacheDram(AUD_DSP0, dwValueAdr+i, DspDecCacheDramData(dwValueAdr + j)) ;
    WriteDspDecCacheDram(AUD_DSP0, dwValueAdr+j, temp) ;
#endif
}

/***************************************************************************
     Function : dwVorbisQuickSortPush
  Description : codebook quick sort stack push command
    Parameter : dwData
    Return    :
***************************************************************************/
void vVorbisQuickSortPush( QuickSortStack *sStack, UINT32 dwData )
{
/*  if( sStack->iBookStackTop >= (BOOK_STACK_DEPTH-1) ) {
    // ... overflow
  }
*/
  (sStack->iBookStackTop)++ ;
#ifdef DUAL_DEC_DRAM_ACCESS
  VorbisWriteDecDram(sStack->dwBookStackAdr+sStack->iBookStackTop, dwData) ;
#else
  WriteDspDecCacheDram(AUD_DSP0, sStack->dwBookStackAdr+sStack->iBookStackTop, dwData) ;
#endif
}

/***************************************************************************
     Function : dwVorbisQuickSortPop
  Description : codebook quick sort stack pop command
    Parameter :
    Return    : top of stack
***************************************************************************/
UINT32 dwVorbisQuickSortPop( QuickSortStack *sStack )
{
  UINT32 dwData ;

/*  if( stack->top_ptr < 0 ) {
    // ... underflow
  }
*/
#ifdef DUAL_DEC_DRAM_ACCESS
  dwData = VorbisReadDecDram(sStack->dwBookStackAdr+sStack->iBookStackTop) ;
#else
  ReadDspDecCacheDram(AUD_DSP0, sStack->dwBookStackAdr+sStack->iBookStackTop, dwData) ;
#endif
  (sStack->iBookStackTop)-- ;
  return( dwData ) ;
}

/***************************************************************************
     Function : dwVorbisQuickSortPartition
  Description : quick sort partition operation
    Parameter : codebook value adr, codebook code adr, left, right
    Return    : i
***************************************************************************/
UINT32 dwVorbisQuickSortPartition(UINT32 dwValueAdr, UINT32 dwCodeAdr, UINT32 dwLeft, UINT32 dwRight)
{
  UINT32 i, j ;
  UINT32 dwKey; ///[Joel]no use, dwCode ;
                ///[Joel]no use,UINT32 temp ;

#ifdef DUAL_DEC_DRAM_ACCESS
    dwKey = VorbisReadDecDram32(dwCodeAdr+dwRight) ;
#else
  ReadDspDecCacheDram(AUD_DSP0, dwCodeAdr+dwRight, dwKey) ;
#endif
  i = dwLeft ;
  j = dwRight-1 ;
  while( 1 )
  {
#ifdef DUAL_DEC_DRAM_ACCESS
    while( VorbisDecDramData32(dwCodeAdr + i) < dwKey )
    {
        i ++ ;
    }
    while( VorbisDecDramData32(dwCodeAdr + j) > dwKey )
    {
        j -- ;
    }
#else
    while( DspDecCacheDramData(AUD_DSP0, dwCodeAdr + i) < dwKey ) {
      i ++ ;
    }
    while( DspDecCacheDramData(AUD_DSP0, dwCodeAdr + j) > dwKey ) {
      j -- ;
    }
#endif
    if( i >= j )
      break ;
    vVorbisExchange(dwValueAdr, dwCodeAdr, i, j) ;
  }
  vVorbisExchange(dwValueAdr, dwCodeAdr, i, dwRight) ;

  return i ;
}

/***************************************************************************
     Function : vVorbisInsertSort
  Description : insert sort for huffman code when data is fewer
    Parameter : codebook value adr, codebook code adr, left, right
    Return    :
***************************************************************************/
void vVorbisInsertSort(UINT32 dwValueAdr, UINT32 dwCodeAdr, UINT32 dwLeft, UINT32 dwRight)
{
  UINT32 i, j, dwCode, dwValue; ///[Joel]no use, dwTemp ;

  for( i = dwLeft+1 ; i <= dwRight ; i ++ )
  {
#ifdef DUAL_DEC_DRAM_ACCESS
    dwCode = VorbisReadDecDram32(dwCodeAdr+i) ;
    dwValue = VorbisReadDecDram32(dwValueAdr+i) ;
#else
    ReadDspDecCacheDram(AUD_DSP0, dwCodeAdr+i, dwCode) ;
    ReadDspDecCacheDram(AUD_DSP0, dwValueAdr+i, dwValue) ;
#endif
    j = i ;

#ifdef DUAL_DEC_DRAM_ACCESS
    while( (j > dwLeft) && (VorbisDecDramData32(dwCodeAdr + j-1) > dwCode) )
    {
        VorbisWriteDecDram32(dwCodeAdr+j, VorbisDecDramData32(dwCodeAdr + j-1)) ;
        VorbisWriteDecDram32(dwValueAdr+j, VorbisDecDramData32(dwValueAdr + j-1)) ;
        j -- ;
    }
#else
    while( (j > dwLeft) && (DspDecCacheDramData(dwCodeAdr + j-1) > dwCode) )
    {
        WriteDspDecCacheDram(AUD_DSP0, dwCodeAdr+j, DspDecCacheDramData(AUD_DSP0, dwCodeAdr + j-1)) ;
        WriteDspDecCacheDram(AUD_DSP0, dwValueAdr+j, DspDecCacheDramData(AUD_DSP0, dwValueAdr + j-1)) ;
        j -- ;
    }
#endif

#ifdef DUAL_DEC_DRAM_ACCESS
    VorbisWriteDecDram32(dwCodeAdr+j, dwCode) ;
    VorbisWriteDecDram32(dwValueAdr+j, dwValue) ;
#else
    WriteDspDecCacheDram(AUD_DSP0, dwCodeAdr+j, dwCode) ;
    WriteDspDecCacheDram(AUD_DSP0, dwValueAdr+j, dwValue) ;
#endif
  }
}

/***************************************************************************
     Function : vVorbisSort3
  Description : sort the 3 data indexed by i, j, k
    Parameter : codebook value adr, codebook code adr, i, j, k
    Return    :
***************************************************************************/
void vVorbisSort3(UINT32 dwValueAdr, UINT32 dwCodeAdr, UINT32 i, UINT32 j, UINT32 k)
{
  ///[Joel]no use, UINT32 dwCode1, dwCode2 ;
#ifdef DUAL_DEC_DRAM_ACCESS
    if( VorbisDecDramData32(dwCodeAdr + i) > VorbisDecDramData32(dwCodeAdr + j) )
        vVorbisExchange(dwValueAdr, dwCodeAdr, i, j) ;

    if( VorbisDecDramData32(dwCodeAdr + i) > VorbisDecDramData32(dwCodeAdr + k) )
        vVorbisExchange(dwValueAdr, dwCodeAdr, i, k) ;

    if( VorbisDecDramData32(dwCodeAdr + j) > VorbisDecDramData32(dwCodeAdr + k) )
        vVorbisExchange(dwValueAdr, dwCodeAdr, j, k) ;
#else
  if( DspDecCacheDramData(dwCodeAdr + i) > DspDecCacheDramData(dwCodeAdr + j) )
    vVorbisExchange(dwValueAdr, dwCodeAdr, i, j) ;

  if( DspDecCacheDramData(dwCodeAdr + i) > DspDecCacheDramData(dwCodeAdr + k) )
    vVorbisExchange(dwValueAdr, dwCodeAdr, i, k) ;

  if( DspDecCacheDramData(dwCodeAdr + j) > DspDecCacheDramData(dwCodeAdr + k) )
    vVorbisExchange(dwValueAdr, dwCodeAdr, j, k) ;
#endif
}

/***************************************************************************
     Function : vVorbisQuickSort
  Description : quick sort for huffman code
    Parameter : codebook value adr, codebook code adr, codebook length
    Return    :
***************************************************************************/
void vVorbisQuickSort(UINT32 dwValueAdr, UINT32 dwCodeAdr, UINT32 dwBookLen)
{
  QuickSortStack sBookStack ;
  UINT32 dwLeft, dwRight, dwIndex ;

  // stack init
  sBookStack.dwBookStackAdr = (UINT32)ADDR_VBSD2RC_DRAM_TEMP_CODEBOOK_STACK ;
  sBookStack.iBookStackTop = -1 ;

  dwLeft = 0 ;
  dwRight = dwBookLen-1 ;
  vVorbisQuickSortPush(&sBookStack, dwLeft) ;
  vVorbisQuickSortPush(&sBookStack, dwRight) ;
  do
  {
    if( (dwRight - dwLeft) > 15 )
    {
        vVorbisSort3(dwValueAdr, dwCodeAdr, dwLeft, (dwLeft+dwRight)>>1, dwRight) ;
        vVorbisExchange(dwValueAdr, dwCodeAdr, (dwLeft+dwRight)>>1, dwRight-1) ;
        dwIndex = dwVorbisQuickSortPartition(dwValueAdr, dwCodeAdr, dwLeft, dwRight-1) ;
        if( (dwIndex-dwLeft) > (dwRight-dwIndex) )
        {
            vVorbisQuickSortPush(&sBookStack, dwLeft) ;
            vVorbisQuickSortPush(&sBookStack, dwIndex-1) ;
            dwLeft = dwIndex + 1 ;
        }
        else
        {
            vVorbisQuickSortPush(&sBookStack, dwIndex+1) ;
            vVorbisQuickSortPush(&sBookStack, dwRight) ;
            dwRight = dwIndex - 1 ;
        }
    }
    else
    {
        vVorbisInsertSort(dwValueAdr, dwCodeAdr, dwLeft, dwRight) ;
        dwRight = dwVorbisQuickSortPop(&sBookStack) ;
        dwLeft = dwVorbisQuickSortPop(&sBookStack) ;
    }
  }while( sBookStack.iBookStackTop >= 0 ) ;
}
#endif                               // end of CC_VORBIS_SUPPORT

//RAW_DELAY
#define IECRAW_SAMPLES_PER_BANK 256

BOOL vSetIECRawDelay(UINT32 u4FreqIdx,UINT8 u1DecId)
{
    UINT32 u4Factor = 48;
    UINT32 u4ChDelay;
    UINT32 u4DelayBank = 0;
    UINT32 u4MaxNum;
    INT32 i4Sample, i4Delay;

    UNUSED(vSetIECRawDelay);
    if ( u1DecId == AUD_DEC_MAIN)
    {
        switch (u4FreqIdx)
        {
        case FS_192K:
            u4Factor = 192;
            break;
        case FS_96K:
            u4Factor = 96;
            break;
        case FS_48K:
            u4Factor = 48;
            break;
        case FS_44K:
            u4Factor = 44;
            break;
        case FS_32K:
            u4Factor = 32;
            break;
        default:
            break;
        }
    }

    u4ChDelay= 0;
    // if IEC RAW delay is synced to channel delay?
    if (uReadDspShmBYTE(AUD_DSP0, B_IECRAW_DELAY_FLAG) != 0)
    {
        // the IEC RAW is modified accroding to "7/8" channel delay
#ifdef CC_AUD_DDI
        u4ChDelay = (UINT32)(u2ReadDspShmWORD(AUD_DSP0, W_CHDELAY_CH7) * u4Factor * 5) / 34; // samples
#else
        u4ChDelay = (UINT32)(u2ReadDspShmWORD(AUD_DSP0, W_CHDELAY_L) * u4Factor * 5) / 34; // samples
#endif
    }

    i4Delay = (INT32)(u2ReadDspShmWORD(AUD_DSP0, W_IEC_RAW_OFFSET) << 16) >> 16; // signed extenstion
    i4Sample = u4ChDelay +  i4Delay * u4Factor; // samples
    if (i4Sample < 0)
    {
        i4Sample = 0;
    }
    u4DelayBank = (UINT32)((i4Sample + (IECRAW_SAMPLES_PER_BANK/2)) /
        IECRAW_SAMPLES_PER_BANK); // fix me!!! Current usage, only 256 bank supports IEC RAW output

    LOG(3, "IEC RAW delay = (%d) bank\n", u4DelayBank);

    // protection for invalid value
    u4MaxNum = uReadDspShmBYTE(AUD_DSP0, B_BANK256NUM) - 10; // PCM = 6 + 4, fix me!!!!
     if (u4DelayBank > u4MaxNum)
    {
        LOG (1, "IEC RAW delay  %d is invalid! (MAX number is %d)\n", u4DelayBank, u4MaxNum);
        u4DelayBank = u4MaxNum;
    }

    WriteDspCommDram (AUD_DSP0, ADDR_RC2D_IEC_DELAY, u4DelayBank);

    return TRUE;
}



