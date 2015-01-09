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
 * $RCSfile: fvr_if.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file fvr_if.h
 *  FVR driver - FVR Interface Header
 */

#ifndef _FVR_IF_H
#define _FVR_IF_H

#include "dmx_mm_if.h"

#define FVR_SCRAMBLE_PID_INDEX              (4)
#define FVR_KEY_COUNT                       (4)

#define FVR_SINGLE_STREAM_GBL_COUNT         (4)
#define FVR_GBL_COUNT                       (5) //add new record region for ci+1.4.

#define FVR_FIFO_ALIGNMENT                  32

#define FVR_PIC_I_TYPE                      0x1
#define FVR_PIC_P_TYPE                      0x2
#define FVR_PIC_B_TYPE                      0x4
#define FVR_PIC_UNKNOWN                     0xFF

#define FVR_PIC_I_MASK                      0x1
#define FVR_PIC_P_MASK                      0x2
#define FVR_PIC_B_MASK                      0x4

#define FVR_PIC_H264_IDR_TYPE       0x10
#define FVR_PIC_H264_NIDR_TYPE      0x20

#define FVR_CIPLUS14_REGION_IDX          4
//------------------------------------------------------------
// TimeStamp
//------------------------------------------------------------
#define FVR_TIMESTAMP_FLAG_ENABLE           0x00000001
#define FVR_TIMESTAMP_FLAG_CLKDIV           0x00000002
#define FVR_TIMESTAMP_FLAG_MASK             0x00000004
#define FVR_TIMESTAMP_FLAG_COUNTER          0x00000008
#define FVR_TIMESTAMP_FLAG_NONE             0x00000000
#define FVR_TIMESTAMP_FLAG_ALL              0xFFFFFFFF

typedef struct
{
    BOOL fgEnable;
    UINT16 u2ClkDiv;
    UINT32 u4Mask;
    UINT32 u4Counter;
} FVR_TIMESTAMP_T;

//------------------------------------------------------------
// GBL Callback
//------------------------------------------------------------
typedef enum
{
    FVR_VIDEO_MPEG = 0,
    FVR_VIDEO_H264,
    FVR_VIDEO_AVS,
    FVR_VIDEO_NONE,
} FVR_VIDEO_TYPE_T;

typedef enum
{
    FVR_GBL_NFY_TIME,
    FVR_GBL_NFY_PKTCOUNT,
    FVR_GBL_NFY_OVERFLOW
} FVR_GBL_NOTIFY_CODE_T;

typedef enum
{
    FVR_CRYPTO_KEY_AES = 0,
    FVR_CRYPTO_KEY_AES_IV,
    FVR_CRYPTO_KEY_SECURE_PVR_AES_0 = 0x30,  ///< secure key
    FVR_CRYPTO_KEY_SECURE_PVR_AES_IV_0,
    FVR_CRYPTO_KEY_MAX
} FVR_CRYPTO_KEY_TYPE_T;

typedef struct
{
    UINT8  u1TsIndex;
    UINT32 u4Addr;
    UINT32 u4Size;
    UINT32 u4PktCount;
} FVR_GBL_PKTNOTIFY_INFO_T;

typedef struct
{
    UINT8  u1TsIndex;
    UINT32 u4Addr;
    UINT32 u4Size;
    UINT32 u4PktCount;
} FVR_GBL_TIMENOTIFY_INFO_T;

typedef struct
{
    UINT8  u1Pidx;
    UINT16 u2PicType;
    UINT32 u4PktCount;
    UINT64 u8Pts33;
} FVR_PID_PICNOTIFY_INFO_T;

typedef struct
{
    UINT8 u1TsIndex;
    UINT8 u1SitPktCnt;
    UINT32 u4SitWp;
} FVR_SIT_PKTREPLACE_INFO_T;

typedef union {
    struct {
        UINT32 au4Key[8];
    } rAesKey;

    struct {
        UINT32 au4Iv[4];
    } rAesIv;
} FVR_CRYPTO_KEY_UNION_T;

typedef struct
{
    BOOL fgEven;
    UINT8 u1KeyIdx;
    UINT32 u4KeyLen;
    FVR_CRYPTO_KEY_TYPE_T eKeyType;
    FVR_CRYPTO_KEY_UNION_T uKey;
} FVR_CRYPTO_KEY_INFO_T;

typedef void (*PFN_FVR_GBL_NOTIFY)(FVR_GBL_NOTIFY_CODE_T eCode,
                                   UINT32 u4Data, const void* pvTag);

//------------------------------------------------------------
// PID Callback
//------------------------------------------------------------
typedef void (*PFN_FVR_PID_PKTSWAP)(UINT8 u1Pidx, UINT32 u4Addr);

typedef void (*PFN_FVR_PID_PICSEARCH)(UINT8 u1Pidx, UINT16 u2Type, UINT32 u4PktCount, 
                                    UINT64 u8Pts33, const void* pvTag);

/// Scrambling state
typedef enum
{
    FVR_SCRAMBLE_STATE_CLEAR,           ///< Clear data
    FVR_SCRAMBLE_STATE_SCRAMBLED,       ///< Scrambled data
    FVR_SCRAMBLE_STATE_UNKNOWN,          ///< Unknown data
    FVR_SCRAMBLE_STATE_SWAP_ONCE_COMPLETE = 0x10
} FVR_SCRAMBLE_STATE_T;


typedef BOOL (*PFN_FVR_PID_DETECTSCRAMBLE)(UINT8 u1Pidx, FVR_SCRAMBLE_STATE_T eState);

//-----------------------------------------------------------------------------
// Function bitmap of PID operations
//-----------------------------------------------------------------------------
#define FVR_GBL_FLAG_BUFFER             0x00000001
#define FVR_GBL_FLAG_TARGET_PKT         0x00000002
#define FVR_GBL_FLAG_NOTIFY             0x00000004
#define FVR_GBL_FLAG_TIME_INFO          0x00000008
#define FVR_GBL_FLAG_SYNC_BYTE          0x00000010
#define FVR_GBL_FLAG_SYNC_MASK          0x00000020
#define FVR_GBL_FLAG_CIPUS_TS           0x00000040
#define FVR_GBL_FLAG_NONE               0x00000000
#define FVR_GBL_FLAG_ALL                0xFFFFFFFF

#define FVR_PID_FLAG_ENABLE             0x00000001
#define FVR_PID_FLAG_TSINDEX            0x00000002
#define FVR_PID_FLAG_PID                0x00000004
#define FVR_PID_FLAG_PICSEARCH          0x00000008
#define FVR_PID_FLAG_PKTSWAP            0x00000010
#define FVR_PID_FLAG_SCRAMBLE_ENABLE    0x00000020  // Record encryption
#define FVR_PID_FLAG_CBC_MODE           0x00000040  // Record encryption
#define FVR_PID_FLAG_KEY_TYPE           0x00000080  // Record encryption
#define FVR_PID_FLAG_KEY_LEN            0x00000100  // Record encryption
#define FVR_PID_FLAG_KEY_INDEX          0x00000200  // Record encryption
#define FVR_PID_FLAG_DETECT_SCRAMBLE    0x00000400  // Input TS: Detect descramble or not
#define FVR_PID_FLAG_FREEZE             0x00000800
#define FVR_PID_FLAG_SOURCE             0x00001000
#define FVR_PID_FLAG_TYPE               0x00002000
#define FVR_PID_FLAG_SECNOTIFY          0x00004000  // PSI section filter
#define FVR_PID_FLAG_INSTANCE_TAG       0x00008000  // PSI section filter
#define FVR_PID_FLAG_DESC               0x00010000  // Input TS: Desc mode (CI+, multi-2)
#define FVR_PID_FLAG_LOCAL_TSINDEX      0x00020000 //input ts: for ci+1.4 remux ts local index.
#define FVR_PID_FLAG_NONE               0x00000000
#define FVR_PID_FLAG_ALL                0xFFFFFFFF


//-----------------------------------------------------------------------------
// Time-shift Tick Count
//-----------------------------------------------------------------------------
#define TIMESHIFT_TIME_SLICE               250
#define TIMESHIFT_MAX_TICKCOUNT            40//20

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------
typedef enum
{
    FVR_KEY_EVEN,
    FVR_KEY_ODD,
    FVR_KEY_BOTH
} FVR_KEY_TYPE_T;

typedef struct
{
    UINT16 u2TargetPktCount;
    BOOL fgAllocateBuffer;
    UINT32 u4BufStart;
    UINT32 u4BufSize;
    UINT32 u4Wp;
    UINT32 u4Rp;

    BOOL fgSetAlign;
    UINT32 u4BufAlign;

    PFN_FVR_GBL_NOTIFY pfnNotifyFunc;
    void* pvNotifyTag;
   
    UINT32 u4TimeToNotify;
    /*the following item used for ciplus1.4 record.*/
	UINT32 u4SyncPattern; /*ts3 bit[31:24], ts2 bit[23:16], ts1 bit[15:8], ts0bit[7:0]*/
    UINT8  u1CiTsidx; /*for ci input ts index settings*/
    UINT8  u1SyncMask; /*use bit[3:0] */
} FVR_GBL_T;

typedef struct
{
    BOOL fgEnable;
    UINT8 u1TsIndex;
	UINT8 u1LocalTsIndex;
    UINT16 u2Pid;

    BOOL fgPktSwap;
    PFN_FVR_PID_PKTSWAP pfnSwapNotify;
    void *pvSwapNotifyTag;
    BOOL fgPktSwapOnce;

    BOOL fgPicSearch;
    UINT8 u1PicSearchMask;
    PFN_FVR_PID_PICSEARCH pfnPicSearchNotify;
    void *pvPicSearchNotifyTag;

    BOOL fgScramble;
    BOOL fgCBCMode;

    FVR_KEY_TYPE_T eKeyType;
    UINT8 u1KeyIdx;
    UINT32 u4KeyLen;

    BOOL fgDetectScramble;
    PFN_FVR_PID_DETECTSCRAMBLE pfnDetectScrambleNotify;
    PFN_FVR_PID_DETECTSCRAMBLE pfnDetectRawScrambleNotify;
     
    BOOL fgFreeze;
    DMX_SOURCE_T eSource;     // Logical channel: DTV0, DTV1
    DMX_PID_TYPE_T ePidType;

    //----------------------------------
    // Below is for section filter
    BOOL fgDoSFilter;
    BOOL fgAllocateBuffer;      // Allocate buffer or not
    UINT32 u4BufAddr;          // Buffer start
    UINT32 u4BufSize;            // Buffer length
    UINT32 u4Rp;                // Read pointer
    UINT32 u4SectionRp;         // Section read pointer
    UINT32 u4Wp;                // Write pointer
    PFN_DMX_NOTIFY pfnNotify;   // Demux callback function
    void* pvNotifyTag;          // Tag value of callback function
    void *pvInstanceTag;
    //----------------------------------

    BOOL  fgDesc;               // Decrypt TS (CI+, multi-2)
    UINT8 u1DescKeyidx;           // Decrypt TS (CI+, multi-2)
    DMX_DESC_MODE_T eDescMode;  // Descramble mode

} FVR_PID_T;

typedef struct
{
    UINT32 u4BufStart;
    UINT32 u4BufEnd;
    UINT32 u4Wp;
    UINT32 u4Rp;
    UINT32 u4PktCount;
    UINT32 u4OverflowCount;
    UINT32 u4BufPeakSize;
} FVR_QUERY_INFO_T;

//------------------------------------------------------------
// Macro
//------------------------------------------------------------
#define GET_BYTE(arg, n)                ((UINT8*)&(arg))[n]

//------------------------------------------------------------
// Function
//------------------------------------------------------------
EXTERN BOOL FVR_Init(void);
EXTERN BOOL FVR_Start(UINT8 u1Idx);
EXTERN BOOL FVR_Stop(UINT8 u1Idx);
EXTERN BOOL FVR_GetResidual(UINT8 u1Idx,UINT32 *pu4Addr, UINT32 *pu4Size, UINT32 *pu4Index);
EXTERN BOOL FVR_IsRecording(UINT8 u1Idx);
EXTERN void FVR_SetScrambleScheme(BOOL fgOrg);

EXTERN BOOL FVR_SetGBL(UINT8 u1Idx,UINT32 u4Flags, const FVR_GBL_T* prPid);
EXTERN BOOL FVR_GetGBL(UINT8 u1Idx,UINT32 u4Flags, FVR_GBL_T* prPid);
EXTERN BOOL FVR_FreeGBL(UINT8 u1Idx);
EXTERN BOOL FVR_DropScrambledPacket(UINT8 u1Idx, BOOL fgEnalbe);

EXTERN BOOL FVR_SetPid(UINT8 u1Pidx, UINT32 u4Flags, const FVR_PID_T* prPid);
EXTERN BOOL FVR_GetPid(UINT8 u1Pidx, UINT32 u4Flags, FVR_PID_T* prPid);
EXTERN BOOL FVR_FreePid(UINT8 u1Pidx);

EXTERN void FVR_SetTimestamp(UINT32 u4Flags, const FVR_TIMESTAMP_T* prTimeStamp);
EXTERN void FVR_GetTimestamp(UINT32 u4Flags, FVR_TIMESTAMP_T* prTimeStamp);

EXTERN BOOL FVR_FlushBuffer(UINT8 u1Idx);
EXTERN BOOL FVR_GetBuffer(UINT8 u1Idx,UINT32 u4Addr, UINT32 u4Size, UINT8 *pu1Buf, UINT32 *pu4RetSize);
EXTERN BOOL FVR_UnlockBuffer(UINT8 u1Idx,UINT32 u4Addr, UINT32 u4Size);
EXTERN BOOL FVR_FreeBuffer(UINT8 u1Idx);

EXTERN void FVR_SetIsOverflow(UINT8 u1Idx, BOOL fgIsOverflow);
EXTERN void FVR_QueryStatus(UINT8 u1Idx, FVR_QUERY_INFO_T *prInfo);

EXTERN void FVR_SetByPass(UINT8 u1TsIndex, UINT32 u4Val, BOOL fgEnable);

EXTERN BOOL FVR_SwapSetPattern(UINT8 u1Pidx, UINT8* pu1Pattern, UINT16 u2Len);
EXTERN BOOL FVR_SwapSetNullPattern(UINT8 u1Pidx);
EXTERN BOOL FVR_SwapFreePattern(UINT8 u1Pidx);

EXTERN BOOL FVR_SetVideoTypeEx(UINT8 u1TsIdx, FVR_VIDEO_TYPE_T eVideoType);
EXTERN FVR_VIDEO_TYPE_T FVR_GetVideoTypeEx(UINT8 u1TsIdx);
EXTERN BOOL FVR_SetVideoTypeByPidx(UINT8 u1Pidx, FVR_VIDEO_TYPE_T eVideoType);
EXTERN FVR_VIDEO_TYPE_T FVR_GetVideoTypeByPidx(UINT8 u1Pidx);

EXTERN BOOL FVR_SetAesKey(UINT8 u1KeyIdx, BOOL fgEven, UINT8 *pu1Key);
EXTERN BOOL FVR_SetAesEvenOddIV(UINT8 u1Idx, BOOL fgEven, UINT8 *pu1IV);
EXTERN BOOL FVR_SetCryptoKey(UINT8 u1KeyIdx, BOOL fgEven, FVR_CRYPTO_KEY_TYPE_T eKeyType,
                            const UINT8 *pu1Key, UINT32 u4KeyLen);
EXTERN BOOL FVR_UpdateWritePointer(UINT8 u1Idx, UINT32 u4Wp);

EXTERN BOOL FVR_SitSetPattern(UINT8 u1TsIdx, UINT16 u2Pid, const UINT8 *pu1Data, UINT16 u2Len);
EXTERN BOOL FVR_SitFreePattern(UINT8 u1TsIdx);
EXTERN BOOL FVR_SitInsertPattern(UINT8 u1TsIdx);
EXTERN BOOL FVR_SitInsertPatternByTimer(UINT8 u1TsIdx, UINT32 u4MsDelay);
EXTERN BOOL FVR_SitStopTimer(UINT8 u1TsIdx);

EXTERN BOOL FVR_StartInsertDIT(UINT8 u1Pidx, UINT32 u4PcrDiffMs);
EXTERN BOOL FVR_StopInsertDIT(UINT8 u1Pidx);

EXTERN FVR_SCRAMBLE_STATE_T FVR_GetScrambleState(UINT8 u1Pidx);
EXTERN FVR_SCRAMBLE_STATE_T FVR_GetRawScrambleState(UINT8 u1Pidx);
//
// PVR loopback
//
EXTERN BOOL PVRM_Loopback_Init(UINT32 u4BufSize, UINT32 u4RecPktCount, UINT32 u4PlayThreshold);
EXTERN BOOL PVRM_Loopback_SetPid(UINT8 u1Pidx, UINT16 u2PidNum, DMX_PID_TYPE_T eType);
EXTERN BOOL PVRM_Loopback_Start(UINT32 u4Distance);
EXTERN BOOL PVRM_Loopback_Stop(VOID);
EXTERN void PVRM_Loopback_GetDataSize(UINT32 *pu4Size, UINT32 *pu4Max, UINT32 *pu4Min);

EXTERN BOOL PVRM_Loopback_Live(UINT8 u1TsIdx);
EXTERN BOOL PVRM_Loopback_Record(UINT32 u4BufSize);
EXTERN BOOL PVRM_Loopback_SinglePlay(UINT32 u4BufAddr, UINT32 u4BufSize);
EXTERN BOOL PVRM_Loopback_StreamPlay(UINT32 u4BufAddr, UINT32 u4BufSize);
EXTERN BOOL PVRM_Loopback_ErrorTest(void);

#endif  // FVR_IF_H

