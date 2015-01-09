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
 * $RCSfile: fvr.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file fvr.h
 *  FVR driver - FVR header
 */

#ifndef _FVR_H
#define _FVR_H

#include "dmx.h"
#include "fvr_if.h"
#include "x_hal_5381.h"

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------

//
// Base addresses
//
#define FVR_GBL_SIZE                        (16)   // 64 bytes
#define FVR_GBL_COMMON_SIZE                 (31)   // 124 bytes
#define FVR_CI14_GBL_SIZE                   (16)  //64bytes for per one ,ci+1.4

#if defined(CC_MT5890)
#define FVR_PID_SIZE                        (9)    // 36 bype Record PID data structure,total 128share
#else  //MT5882
#define FVR_PID_SIZE                        (7)    // 28 bype Record PID data structure,total 128share
#endif

#define FVR_PID_TABLE_BASE                  (DEMUX0_BASE + 0xE00) //0x17E00

#if defined(CC_MT5890)
#define FVR_GBL_PID_BASE                    (DEMUX7_BASE + 0x1F4) //0x541f4
#define FVR_GBL_PID_BASE1                   (DEMUX7_BASE + 0x234) //0x54234
#define FVR_GBL_PID_BASE2                   (DEMUX7_BASE + 0x274) //0x54274
#define FVR_GBL_PID_BASE3                   (DEMUX7_BASE + 0x2B4) //0x542B4
#define FVR_GBL_CI14_PID_BASE               (DEMUX7_BASE + 0x2F4) //0x542F4

#define FVR_GBL_COMMON_REGION               (DEMUX7_BASE + 0x334) //0x54334

#define FVR_PER_PID_OFFSET                  (0x2D00)
#define FVR_PER_PID_BASE                    (DEMUX2_BASE + FVR_PER_PID_OFFSET) //0xf001bd00

#define FVR_KEY_BASE                        (DEMUX9_BASE + 0xC80) //0xf006AC80

#else  //MT5882

#define FVR_GBL_PID_BASE                    (DEMUX7_BASE + 0xA4)  //0x540A4
#define FVR_GBL_PID_BASE1                   (DEMUX7_BASE + 0xE4)  //0x540E4
#define FVR_GBL_PID_BASE2                   (DEMUX7_BASE + 0x124) //0x54124
#define FVR_GBL_PID_BASE3                   (DEMUX7_BASE + 0x164) //0x54164
#define FVR_GBL_CI14_PID_BASE               (DEMUX7_BASE + 0x1A4) //0x541A4

#define FVR_GBL_COMMON_REGION               (DEMUX7_BASE + 0x1E4) //0x541E4

#define FVR_PER_PID_OFFSET                  (0x1E00)
#define FVR_PER_PID_BASE                    (DEMUX2_BASE + FVR_PER_PID_OFFSET) //0x1AE00

#define FVR_KEY_BASE                        (DEMUX7_BASE + 0xD24) //0x54D24
#endif

#define FVR_SIT_BUFFER_OFFSET               (0x4A34)
#define FVR_SIT_BUFFER_OFFSET1              (0x4AE8)
#define FVR_SIT_BUFFER_OFFSET2              (0x4B9C)
#define FVR_SIT_BUFFER_OFFSET3              (0x4C50)


#define FVR_SIT_BUFFER_BASE                 (DEMUX7_BASE + FVR_SIT_BUFFER_OFFSET - 0x4000) 
#define FVR_SIT_BUFFER_BASE1                (DEMUX7_BASE + FVR_SIT_BUFFER_OFFSET1 - 0x4000)
#define FVR_SIT_BUFFER_BASE2                (DEMUX7_BASE + FVR_SIT_BUFFER_OFFSET2 - 0x4000)
#define FVR_SIT_BUFFER_BASE3                (DEMUX7_BASE + FVR_SIT_BUFFER_OFFSET3 - 0x4000)

#if defined(CC_MT5890)
#define FVR_CLOCK                           (432000000) //oryx/gazelle  demux clock
#else  //MT5882
#define FVR_CLOCK                           (324000000) //capri demux clock
#endif

//
// Interrupt
//
#define FVR_INT_ERR_MASK                   0xFFFF
#define FVR_INT_ERR_REC_DBM                (1 << 0)
#define FVR_INT_ERR_uP                     (1 << 2)

#define FVR_INT_STATUS_MASK                0xFFFF0000
#define FVR_INT_STATUS_REC_DBM             (1 << 16)
#define FVR_INT_STATUS_uP                  (1 << 18)

//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------

#define FVR_SIT_BASE_W(base, word)        ((volatile UINT32*)(base))[(word)]

#define FVR_SIT_ARY_W(idx, word)         \
            FVR_SIT_BASE_W((idx==0) ? FVR_SIT_BUFFER_BASE : \
            (idx==1) ? FVR_SIT_BUFFER_BASE1 : \
            (idx==2) ? FVR_SIT_BUFFER_BASE2 : \
            FVR_SIT_BUFFER_BASE3 , word)

///
/// FVR PID data structure access commands
///
#define FVR_GBL_BASE_W(base, word)          ((volatile UINT32*)(base))[(word)]

#define FVR_GBL_ARY_W(idx, word)         \
            FVR_GBL_BASE_W((idx==0) ? FVR_GBL_PID_BASE : \
            (idx==1) ? FVR_GBL_PID_BASE1 : \
            (idx==2) ? FVR_GBL_PID_BASE2 : \
            (idx==3) ? FVR_GBL_PID_BASE3 : \
            FVR_GBL_CI14_PID_BASE, word)

/*the following two item used for ciplus1.4*/
#define FVR_GBL_CI14_REPLACE_BYTE_CONTROL    FVR_GBL_BASE_W(FVR_GBL_CI14_PID_BASE,9)
#define FVR_GBL_CI14_TS_SYNCBYTE_SETTING     FVR_GBL_BASE_W(FVR_GBL_CI14_PID_BASE,10)

#define FVR_GBL_TIMEINFO                     FVR_GBL_BASE_W(FVR_GBL_COMMON_REGION, 0)
#define FVR_GBL_TIMEINFO_UPDATE              FVR_GBL_BASE_W(FVR_GBL_COMMON_REGION, 1)
#define FVR_GBL_SERIALNUM_ADDR               FVR_GBL_BASE_W(FVR_GBL_COMMON_REGION, 2)

#define FVR_GBL_DMEMENDADDR_TS0              FVR_GBL_BASE_W(FVR_GBL_COMMON_REGION, 3)
#define FVR_GBL_PKTCOUNTER_TS0               FVR_GBL_BASE_W(FVR_GBL_COMMON_REGION, 4)

#define FVR_GBL_DMEMENDADDR_TS1              FVR_GBL_BASE_W(FVR_GBL_COMMON_REGION, 5)
#define FVR_GBL_PKTCOUNTER_TS1               FVR_GBL_BASE_W(FVR_GBL_COMMON_REGION, 6)

#define FVR_GBL_DMEMENDADDR_TS2              FVR_GBL_BASE_W(FVR_GBL_COMMON_REGION, 7)
#define FVR_GBL_PKTCOUNTER_TS2               FVR_GBL_BASE_W(FVR_GBL_COMMON_REGION, 8)

#define FVR_GBL_DMEMENDADDR_TS3              FVR_GBL_BASE_W(FVR_GBL_COMMON_REGION, 9)
#define FVR_GBL_PKTCOUNTER_TS3               FVR_GBL_BASE_W(FVR_GBL_COMMON_REGION, 10)

#define FVR_GBL_DMEMENDADDR_CI14              FVR_GBL_BASE_W(FVR_GBL_COMMON_REGION, 11)
#define FVR_GBL_PKTCOUNTER_CI14               FVR_GBL_BASE_W(FVR_GBL_COMMON_REGION, 12)



#define FVR_PER_PID_W(word)                 ((volatile UINT32*)(FVR_PER_PID_BASE))[(word)]
#define FVR_PER_PID_S(pidx)                 FVR_PER_PID_W(FVR_PID_SIZE * (pidx))
#define FVR_PER_PID_S_W(pidx, word)         FVR_PER_PID_W((FVR_PID_SIZE * (pidx)) + (word))

/// PID index table access command
#define FVR_PID_INDEX_TABLE(pidx)           ((volatile UINT32*)(FVR_PID_TABLE_BASE))[(pidx)]

#define FVR_KEY_EVEN_KEY(idx, word)         ((volatile UINT32*)(FVR_KEY_BASE + (idx * 96)))[(word)]
#define FVR_KEY_ODD_KEY(idx, word)          ((volatile UINT32*)(FVR_KEY_BASE + ((idx * 96) + 48)))[(word)]
#define FVR_KEY_EVEN_IV(idx, word)          ((volatile UINT32*)(FVR_KEY_BASE + ((idx * 96) + 32)))[(word)]
#define FVR_KEY_ODD_IV(idx, word)           ((volatile UINT32*)(FVR_KEY_BASE + ((idx * 96) + 80)))[(word)]

#define DATASIZE(rp, wp, size)      \
    (((rp) <= (wp)) ? ((wp) - (rp)) : (((wp) + (size)) - (rp)))

//-----------------------------------------------------------------------------
// Type def
//-----------------------------------------------------------------------------
typedef struct
{
    UINT32 u4BufStart;
    UINT32 u4BufEnd;
    UINT32 u4Rp;
    UINT32 u4Wp;
    UINT16 u2CurPktCount;
    UINT16 u2TargetPktCount;
    UINT32 u4LongPktCount;
    UINT32 u4PktRp;

    BOOL fgAllocateBuffer;
    UINT32 u4BufSize;

    BOOL fgSetAlign;
    UINT32 u4BufAlign;

    PFN_FVR_GBL_NOTIFY pfnNotifyFunc;
    void* pvNotifyTag;

    UINT32 u4TimeToNotify;

    UINT32 u4PicWorkingBuf;
	/*add new item for ciplus1.4 */
	UINT32 u4SyncPattern; /*ts3 bit[31:24], ts2 bit[23:16], ts1 bit[15:8], ts0bit[7:0]*/
	UINT8  u1CiTsidx; /*for ci input ts index settings*/
	UINT8  u1SyncMask; /*use bit[3:0] */
} FVR_GBL_STRUCT_T;

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

    FVR_KEY_TYPE_T eKeyType;    // Record encryption key type
    UINT8 u1KeyIdx;             // Record encryption key index
    UINT32 u4KeyLen;            // Record encryption key length

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
    UINT32 u4BufStart;          // Buffer start
    UINT32 u4BufEnd;            // Buffer end
    UINT32 u4BufLen;            // Buffer length
    UINT32 u4Rp;                // Read pointer
    UINT32 u4SectionRp;         // Section read pointer
    UINT32 u4Wp;                // Write pointer
    UINT32 u1SerialNumber;
    PFN_DMX_NOTIFY pfnNotify;   // Demux callback function
    void* pvNotifyTag;          // Tag value of callback function
    void *pvInstanceTag;

#ifdef CC_DMX_MULTIPLE_SECTION
    //UINT32 u4SecMask;
    UINT32 au4SecMask[SEC_CNT];
    UINT8  u1SecCount;
    UINT8  u1SecIdx;            // If u1SecIdx means first secion filter index
#endif  // CC_DMX_MULTIPLE_SECTION
    //----------------------------------

    BOOL  fgDesc;               // Decrypt TS (CI+, multi-2)
    UINT8 u1DescKeyidx;         // Decrypt TS (CI+, multi-2)
    DMX_DESC_MODE_T eDescMode;  // Descramble mode

    BOOL fgOddKey;
} FVR_PID_STRUCT_T;

typedef enum
{
    FVR_MSG_PKTCOUNT,
    FVR_MSG_PKTSWAP,
    FVR_MSG_PICSEARCH,
    FVR_MSG_TIMERPKT,
    FVR_MSG_SITCOMPLETE
} FVR_MSG_TYPE_T;

typedef struct {
    FVR_MSG_TYPE_T eMsgType;    ///< Message type
    union {
        FVR_SIT_PKTREPLACE_INFO_T rSitInfo;
        FVR_PID_PICNOTIFY_INFO_T rPicInfo;
        FVR_GBL_TIMENOTIFY_INFO_T rTimeInfo;
    } uMsgData;
} FVR_MSG_T;



//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

EXTERN void _FVR_ApiLock(void);
EXTERN void _FVR_ApiUnlock(void);

//
// fvr_init.c
//
EXTERN BOOL _FVR_Init(void);

EXTERN BOOL _FVR_Start(UINT8 u1Idx);
EXTERN BOOL _FVR_Stop(UINT8 u1Idx);
EXTERN BOOL _FVR_IsRecording(UINT8 u1Idx);

EXTERN BOOL _FVR_GetResidual(UINT8 u1Idx, UINT32 *pu4Addr, UINT32 *pu4Size, UINT32 *pu4Index);
EXTERN void _FVR_SetScrambleScheme(BOOL fgOrg);

//
// fvr_swap.c
//
EXTERN BOOL _FVR_SwapInit(void);
EXTERN BOOL _FVR_SwapSetPattern(UINT8 u1Pidx, UINT8* pu1Pattern, UINT16 u2Len);
EXTERN BOOL _FVR_SwapSetNullPattern(UINT8 u1Pidx);
EXTERN BOOL _FVR_SwapFreePattern(UINT8 u1Pidx);
EXTERN BOOL _FVR_DoSwap(UINT8 u1Pidx, UINT32 u4Addr);
EXTERN BOOL _FVR_DoExchangeKey(UINT8 u1Idx);
EXTERN BOOL _FVR_DoPacketSwapOnce(UINT8 u1Pidx);


//
// fvr_isr.c
//
EXTERN BOOL _FVR_InitIsr(void);
EXTERN BOOL _FVR_DispatchuPInterrupt(UINT32 u4Status, UINT32 u4Status2);
EXTERN void _FVR_uPErrorInt(UINT8 u1Idx);

//
// fvr_api.c
//
#ifdef CC_DEFINE_DMX_LOCK
extern CRIT_STATE_T _rGFvrLock;
#define _FVR_Lock() do { _rGFvrLock = x_crit_start(); } while (0)
#define _FVR_Unlock() do {  x_crit_end(_rGFvrLock); } while (0)
#else /* CC_DEFINE_DMX_LOCK */
EXTERN void _FVR_Lock(void);
EXTERN void _FVR_Unlock(void);
#endif /* CC_DEFINE_DMX_LOCK */

EXTERN void _FVR_InitAPI(void);

EXTERN BOOL _FVR_SetGBL(UINT8 u1Idx, UINT32 u4Flags, const FVR_GBL_T* prGBL);
EXTERN BOOL _FVR_GetGBL(UINT8 u1Idx, UINT32 u4Flags, FVR_GBL_T *prGBL);
EXTERN BOOL _FVR_FreeGBL(UINT8 u1Idx);
EXTERN FVR_GBL_STRUCT_T* _FVR_GetGBLStruct(UINT8 u1Idx);
EXTERN BOOL _FVR_DropScrambledPacket(UINT8 u1Idx, BOOL fgEnalbe);

EXTERN BOOL _FVR_TogglePidKey(void);
EXTERN void _FVR_SetIsToggleKey(UINT8 u1Idx);
EXTERN BOOL _FVR_GetIsToggleKey(UINT8 u1Idx);

EXTERN BOOL _FVR_SetPid(UINT8 u1Pidx, UINT32 u4Flags, const FVR_PID_T* prPid);
EXTERN BOOL _FVR_GetPid(UINT8 u1Pidx, UINT32 u4Flags, FVR_PID_T *prPid);
EXTERN FVR_PID_STRUCT_T* _FVR_GetPidStruct(UINT8 u1Pidx);
EXTERN BOOL _FVR_FreePid(UINT8 u1Pidx);
EXTERN BOOL _FVR_FlushPidBuffer(UINT8 u1Idx);

EXTERN void _FVR_SetTimestamp(UINT32 u4Flags, const FVR_TIMESTAMP_T* prTimeStamp);
EXTERN void _FVR_GetTimestamp(UINT32 u4Flags, FVR_TIMESTAMP_T* prTimeStamp);

EXTERN BOOL _FVR_FlushBuffer(UINT8 u1Idx);
EXTERN BOOL _FVR_GetBuffer(UINT8 u1Idx, UINT32 u4Addr, UINT32 u4Size, UINT8 *pu1Buf, UINT32 *pu4RetAddr);
EXTERN BOOL _FVR_UnlockBuffer(UINT8 u1Idx, UINT32 u4Addr, UINT32 u4Size);
EXTERN BOOL _FVR_FreeBuffer(UINT8 u1Idx);

EXTERN void _FVR_QueryStatus(UINT8 u1Idx, FVR_QUERY_INFO_T *prInfo);
EXTERN void _FVR_SetIsOverflow(UINT8 u1Idx, BOOL fgOverflow);

EXTERN void _FVR_SetByPass(UINT8 u1TsIndex, UINT32 u4Val, BOOL fgEnable);

EXTERN BOOL _FVR_SetVideoTypeByPidx(UINT8 u1Pidx, FVR_VIDEO_TYPE_T eVideoType);
EXTERN FVR_VIDEO_TYPE_T _FVR_GetVideoTypeByPidx(UINT8 u1Pidx);

EXTERN BOOL _FVR_SetAesIV(UINT32 *pu4IV);
EXTERN BOOL _FVR_SetAesKey(UINT8 u1KeyIdx, BOOL fgEven, UINT8 *pu1Key);
EXTERN BOOL _FVR_SetAesEvenOddIV(UINT8 u1Idx, BOOL fgEven, UINT32 *pu4IV);
EXTERN BOOL _FVR_SetCryptoKey(UINT8 u1KeyIdx, BOOL fgEven, FVR_CRYPTO_KEY_TYPE_T eKeyType,
                                const UINT8 *pu1Key, UINT32 u4KeyLen);

EXTERN BOOL _FVR_IsPidEnabled(UINT8 u1Pidx);

EXTERN BOOL _FVR_PSI_UnlockBuffer(UINT8 u1Pidx, UINT8 u1SerialNumber, UINT32 u4Size, UINT32 u4FrameAddr);
EXTERN BOOL _FVR_PSI_FlushBuffer(UINT8 u1Pidx);
EXTERN BOOL _FVR_PSI_GetBuffer(UINT8 u1Pidx, UINT8 u1SerialNumber, UINT32 u4FrameAddr,
                               UINT32 u4SkipSize, UINT32 u4CopySize, UCHAR* pu1Dest);

EXTERN BOOL _FVR_UpdateWritePointer(UINT8 u1Idx, UINT32 u4Wp);
EXTERN BOOL _FVR_GetIsTSEnabled(UINT8 u1TsIdx);

EXTERN BOOL _FVR_SendTimerPktEvent(FVR_GBL_TIMENOTIFY_INFO_T *prTimeInfo);
EXTERN BOOL _FVR_SendPictureEvent(FVR_PID_PICNOTIFY_INFO_T *prPicInfo);
EXTERN BOOL _FVR_SendSitCompleteEvent(FVR_SIT_PKTREPLACE_INFO_T *prSitInfo);

EXTERN BOOL _FVR_ReplaceDITPacket(UINT8 u1Pidx, UINT32 u4PktIdx);

EXTERN UINT32 _FVR_SitQueryInsertedPacketCount(UINT8 u1TsIdx);
EXTERN BOOL _FVR_SitReplacePacket(UINT8 u1TsIdx, UINT8 u1SitPktCnt, BOOL fgComplete);
EXTERN BOOL _FVR_SitSetPattern(UINT8 u1TsIdx, UINT16 u2Pid, const UINT8 *pu1Data, UINT16 u2Len);
EXTERN BOOL _FVR_SitFreePattern(UINT8 u1TsIdx);
EXTERN BOOL _FVR_SitInsertPattern(UINT8 u1TsIdx, BOOL fgSyncCmd);
EXTERN BOOL _FVR_SitInsertPatternByTimer(UINT8 u1TsIdx, UINT32 u4MsDelay);
EXTERN BOOL _FVR_SitStopTimer(UINT8 u1TsIdx);
EXTERN BOOL _FVR_SitInit(void);

EXTERN BOOL _FVR_StartInsertDIT(UINT8 u1Pidx, UINT32 u4PcrDiffMs);
EXTERN BOOL _FVR_StopInsertDIT(UINT8 u1Pidx);

EXTERN FVR_SCRAMBLE_STATE_T _FVR_GetScrambleState(UINT8 u1Pidx);
EXTERN FVR_SCRAMBLE_STATE_T _FVR_GetRawScrambleState(UINT8 u1Pidx);

EXTERN UINT8 _FVR_GetRecordIdx(UINT8 u1Pidx);


#endif  // _FVR_H

