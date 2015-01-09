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
 * $RCSfile: dmx_emu.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file dmx_emu.h
 *  Demux driver - emulation header
 */

#ifndef DMX_EMU_H
#define DMX_EMU_H


//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#include "x_typedef.h"
#include "dmx_mm_if.h"

//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------

//#define EMU_5391


//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------

#if defined(CC_MT5890)
#define MAX_TS_INDEX                    4
#else //MT5882
#define MAX_TS_INDEX                    3
#endif

#define DEF_SRC_BUF_SIZE                0x100000

#define DEF_DST_BUF_SIZE                0x120000

#define DEF_BUF_FULL_GAP                0x30

#define TS_RP_BOUND                     (188 * 14)

#define TS_PACKET_SIZE                  188

#define MAX_KEY_NO                      DMX_DMEM_CA_KEY_NUM

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------

typedef enum
{
    KEY_SELECT_EVEN = 0,
    KEY_SELECT_ODD,
    KEY_SELECT_RANDOM
} KEY_SELECT_T;

typedef struct
{
    UINT8 au1Even[32];
    UINT8 au1Odd[32];
} DMX_KEY_T;

typedef struct
{
    BOOL fgValid;
    UINT8 u1KeyIndex;
    UINT16 u2Pid;
} DMX_KEY_MAP_T;

typedef struct
{
    DMX_KEY_T arKey[DMX_DMEM_CA_KEY_NUM];
    DMX_KEY_MAP_T arKeyMap[DMX_NUM_PID_INDEX];
    KEY_SELECT_T eKeySelect;
    UINT16 u2KeyLen;
    UINT8 au1InitVector[16];
    DMX_CA_FEEDBACK_MODE_T eFeedbackMode;
    DMX_DESC_RTB_MODE_T eRtb;
    UINT8 au1SampleId[8];
    BOOL fgCTRResidual;
} AES_PARAM_T;

typedef struct
{
    UINT16 u2KeyLen;
    UINT8 au1Key[32];
    UINT8 au1InitVector[16];
    UINT8 au1SampleId[8];
    BOOL fgCbc;
    DMX_DRM_MODE_T eMode;
} DRM_PARAM_T;

typedef struct
{
    UINT16 u2Pid;
	UINT16 u2PidEx[4]; 
    UINT8 au1EvenKey[24];
    UINT8 au1OddKey[24];
    BOOL fgValid;
} ENPID_T;

typedef struct
{
    ENPID_T arEnPid[MAX_KEY_NO];
    KEY_SELECT_T eKeySelect;
    UINT8 au1InitVector[8];
    BOOL fg3Des;
    BOOL fgCbc;
    BOOL fgSCTE;
    DMX_CA_FEEDBACK_MODE_T eFeedbackMode;
    DMX_DESC_RTB_MODE_T eRtb;
    UINT8 au1SampleId[8];
    BOOL fgCTRResidual;
} DES_PARAM_T;

typedef struct
{
    UINT16 u2Pid;
    KEY_SELECT_T eKeySelect;
    UINT8 au1InitVector[16];
    UINT8 au1OddKey[16];
    UINT8 au1EvenKey[16];
    UINT8 u1KeyIdx;
    DMX_CA_FEEDBACK_MODE_T eFeedbackMode;
    DMX_DESC_RTB_MODE_T eRtb;
} SMS4_PARAM_T;
//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Function prototypes
//-----------------------------------------------------------------------------
EXTERN BOOL _DMX_EMU_DDI_Init(UINT8 u1TsIndex);
EXTERN BOOL _DMX_EMU_DDI_130byteEnable(BOOL fgEnable, UINT8 u1PktSize);
EXTERN BOOL _DMX_EMU_PalyBack_130byteEnable(BOOL fgEnable, UINT8 u1PktSize);
EXTERN BOOL _DMX_EMU_Framer_130byteEnable(UINT8 u1Framer, BOOL fgDemod130byteTs, BOOL fgInputEnable, UINT8 u1InputPktSize ,BOOL fgOutputEnable, UINT8 u1OutputPktSize);
EXTERN BOOL _DMX_EMU_DDI_SetBuffer(UINT32 u4Addr, UINT32 u4Size);
EXTERN BOOL _DMX_EMU_DDI_Transfer(UINT8 u1Type, UINT32 u4Addr, UINT32 u4Size);

EXTERN BOOL _DMX_EMU_SyncToggleBitInBypassCtrlWord(UINT8 u1TsIndex, UINT8 u1Pidx);

EXTERN BOOL _DMX_EMU_SetPktSize(UINT8 u1TsIndex, UINT16 u2PktSize, UINT16 u2ValidSize);

EXTERN BOOL _DDI_RECPULL_Transfer(UINT8 u1TsIndex, UINT32 u4Addr, UINT32 u4Size);
EXTERN BOOL _DDI_PULLTransfer(UINT8 u1Pidx, UINT32 u4Addr, UINT32 u4Size);

EXTERN BOOL _DMX_EncryptAesPacket(UINT8* pu1Buf, AES_PARAM_T* prParam, UINT8 u1ForceKeyId);
EXTERN BOOL _DMX_EncryptDesPacket(UINT8* pu1Buf, DES_PARAM_T* prParam);

#endif  // DMX_EMU_H



