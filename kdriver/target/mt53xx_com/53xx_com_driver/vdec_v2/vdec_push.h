/*----------------------------------------------------------------------------*
 * No Warranty                                                                *
 * Except as may be otherwise agreed to in writing, no warranties of any      *
 * kind, whether express or implied, are given by MTK with respect to any MTK *
 * Deliverables or any use thereof, and MTK Deliverables are provided on an   *
 * "AS IS" basis.  MTK hereby expressly disclaims all such warranties,        *
 * including any implied warranties of merchantability, non-infringement and  *
 * fitness for a particular purpose and any warranties arising out of course  *
 * of performance, course of dealing or usage of trade.  Parties further      *
 * acknowledge that Company may, either presently and/or in the future,       *
 * instruct MTK to assist it in the development and the implementation, in    *
 * accordance with Company's designs, of certain softwares relating to        *
 * Company's product(s) (the "Services").  Except as may be otherwise agreed  *
 * to in writing, no warranties of any kind, whether express or implied, are  *
 * given by MTK with respect to the Services provided, and the Services are   *
 * provided on an "AS IS" basis.  Company further acknowledges that the       *
 * Services may contain errors, that testing is important and Company is      *
 * solely responsible for fully testing the Services and/or derivatives       *
 * thereof before they are used, sublicensed or distributed.  Should there be *
 * any third party action brought against MTK, arising out of or relating to  *
 * the Services, Company agree to fully indemnify and hold MTK harmless.      *
 * If the parties mutually agree to enter into or continue a business         *
 * relationship or other arrangement, the terms and conditions set forth      *
 * hereunder shall remain effective and, unless explicitly stated otherwise,  *
 * shall prevail in the event of a conflict in the terms in any agreements    *
 * entered into between the parties.                                          *
 *---------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * Copyright(c) 2005, MediaTek, Inc.
 * All rights reserved.
 *
 * Unauthorized use, practice, perform, copy, distribution, reproduction,
 * or disclosure of this information in whole or in part is prohibited.
 *-----------------------------------------------------------------------------
 * $Author: p4admin $
 * $Date: 2015/02/28 $
 * $RCSfile: vdec_type.h,v $
 * $Revision: #4 $
 *---------------------------------------------------------------------------*/

/** @file mpv_push.h
 *  This header file contains common constant and type definitions of MPV.
 */

#ifndef VDEC_PUSH_H
#define VDEC_PUSH_H


//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "vdec_drvif.h"
#include "vdec_push.h"
LINT_EXT_HEADER_BEGIN
#include "drv_dbase.h"
#include "x_typedef.h"
#include "x_csemaphore.h"
LINT_EXT_HEADER_END


//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
#if !defined(CC_ENABLE_VOMX) && !defined(CC_SUPPORT_TV_STAGEFRIGHT) 
#define VDEC_INPUT_DATA_QUEUE_NUMBER (10)
#define VDEC_INPUT_AU_QUEUE_NUMBER (100)
#define VDEC_BUF_COUNT      (10)
#else
#define VDEC_INPUT_DATA_QUEUE_NUMBER (3)
#define VDEC_INPUT_AU_QUEUE_NUMBER (100)
#ifdef CC_DTV_SUPPORT_LG
#define VDEC_BUF_COUNT      (10)
#else
#ifdef CC_SUPPORT_TV_STAGEFRIGHT
#define VDEC_BUF_COUNT      (2)
#else
#define VDEC_BUF_COUNT      (10)
#endif
#endif
#endif

#if defined(CC_FBM_FOUR_INST_SUPPORT)
#define VDEC_PUSH_MAX_DECODER         4
#elif defined(CC_FBM_TWO_FBP)
#define VDEC_PUSH_MAX_DECODER         2
#else
#define VDEC_PUSH_MAX_DECODER         1
#endif

#define VDEC_PUSH_ES_START           (0)
#define VDEC_PUSH_CHANNEL_START      (1)
#ifdef CC_SUPPORT_TV_STAGEFRIGHT
#define VDEC_PUSH_MSG_Q_SIZE       (VDEC_INPUT_DATA_QUEUE_NUMBER + 240)
#else
#define VDEC_PUSH_MSG_Q_SIZE       (VDEC_INPUT_DATA_QUEUE_NUMBER + 240)
#endif
#define VDEC_PUSH_STACK_SIZE       (8172)
#ifdef MPV_SEMI_HOST // for FPGA, lower to cli thread. We could key in command
#define VDEC_PUSH_THREAD_PRIORITY            255
#else
#ifdef CC_ENABLE_VDECOMX
// This is block mode decoder. It should be same with VDEC.
#define VDEC_PUSH_THREAD_PRIORITY            60
#else
#define VDEC_PUSH_THREAD_PRIORITY            100
#endif
#endif
#define VDEC_PUSH_STOP_SYNC_WAIT_TIME       (5000)
#define VDEC_PUSH_PUTDATA_SYNC_WAIT_TIME    (5000)
#define VDEC_PUSH_MOVEESMQ_SYNC_WAIT_TIME   (5000)
#define VDEC_PUSH_FLUSH_SYNC_WAIT_TIME      (5000)
#define VDEC_PUSH_VDEC_IO_WAIT_TIME         (5000)
#define VDEC_DECRYPT_UNIT_LEN (16)
#define VDEC_DECRYPT_BUFLEN (4*1024)
#define VDEC_PUSH_BIG_FEEDER_SIZE         (0xA00000)

#define RM_V_PUSH_SLICE_SZ_BUF_NS   20000 
typedef enum VDEC_CONTAINER_METADATA
{
    VIDEO_MKVCodecData          = 0x01,         /** Have MKV codec data (sequence header)*/
    VIDEO_VariableFrameRate     = 0x02,         /** Variable frame rate */
    VIDEO_ContainerMetaDataMax  = 0x7FFFFFFF
} VDEC_CONTAINER_METADATA;

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------

typedef struct {
    BOOL   fgDataSeqHdr;      // contain sequence header
    UCHAR  ucDataPicType;     // picture type
    UINT32 u4DataPTS;         // PTS
    UINT32 u4DataTag;         // caller's tag
    UINT32 u4DataAddr;        // data address
    UINT32 u4DataSize;        // data size
} VDEC_DATA_INTO_T;


typedef struct {
    BOOL fgInited;
    BOOL fgIsRing;
    BOOL fgIsPartial;   // not complete decode data

    // fifo
    UINT32 u4FifoStart;     // ring buffer start address
    UINT32 u4FifoEnd;       // ring buffer end address
    UINT32 u4FifoRpt;           // ring buffer read pointer
    UINT32 u4FifoWpt;           // ring buffer write pointer
    UINT32 u4FifoRptParse;      // ring buffer read pointer for parsing
    UINT32 u4FifoRptParseStart; // ring buffer read pointer for parsing
    UINT32 u4FifoRptValue;      // ring buffer read pointer value
    UINT32 u4FifoRptValueLast;      // ring buffer read pointer value
    UCHAR  ucFifoRptPicType;    // ring buffer read pointer value
    UCHAR  ucFifoRptPicTypeNext;    // ring buffer read pointer value
    BOOL   fgFifoRptSeqHdr;     // ring buffer read pointer value
    BOOL   fgFirst;             // ring buffer read pointer value
    BOOL   fgPicture;           // ring buffer read pointer value

    // fifo2

    // input data
    VDEC_DATA_INTO_T arData[VDEC_INPUT_DATA_QUEUE_NUMBER];
    UINT32 u4DataLastPTS;
    UINT32 u4DataLast2PTS;
    UINT32 u4DataReadIdx;
    UINT32 u4DataWriteIdx;
    UINT32 u4DataNum;
    BOOL   fgEos;

    // AU
    BOOL fgAUBuf;
    VDEC_AU_INTO_T rAUBuffer;
    VDEC_AU_INTO_T arAU[VDEC_INPUT_AU_QUEUE_NUMBER];
    UINT32 u4AUReadIdx;
    UINT32 u4AUWriteIdx;
    UINT32 u4AUNum;

    // for callback
    VDEC_PUSH_CB_T fnCb;
} VDEC_INPUT_STREAM_T;
typedef struct
{
   UINT32 u4TempBufWp;
   UINT8 *arTempUnitBuf;//[VDEC_DECRYPT_BUFLEN];
   BOOL fgReseted;
   BOOL fgDecryptPlayback;
}VDEC_DECRYPT_INFO_T;

typedef struct
{
    BOOL        fgUsed;
    UINT32      u4BufAddr;
    UINT32      u4BufSize;
} VDEC_BUF_INFO_T;

typedef struct {
    //BOOL fgGoDead;
    BOOL fgBytesReady;
    BOOL fgAUReady;
    BOOL fgFrameReady;
    BOOL fgEnDmx;
    BOOL fgNonFirst;
    BOOL fgIsRv8;
    BOOL fgDataTimerStart;
    BOOL fgFirstVideoChunk;
    BOOL fgInsertStartcode;
    BOOL fgGotEos;
    BOOL fgNotCalPTS;
    BOOL fgVariableFrameRate;
    BOOL fgDecoderCalPts;
	BOOL fgIsSecureInput;    
    BOOL fgFifoFull;
    BOOL fgLowLatencyMode;
	BOOL fgGstPlay;
    UINT8 u1DmxId;
    UINT8 u1DmxPid;
    UINT8 ucVPushId;
    UINT8 ucVdecId;
    UINT8 ucChannelId;
    UINT16 u2MstCount;
    UINT32 u1StartCodeLenMinus1;
    UINT32 u4CurStopId;
    UINT32 u4PreviousStopId;
    UINT32 u4FifoStart;
    UINT32 u4FifoEnd;
    UINT32 u4Width;
    UINT32 u4Height;
    UINT32 u4AllocBufCnt;
    UINT32 u4UnmoveCnt;
    UINT32 au4RprSizeArray[16];
    UINT32 u4Rate;
    UINT32 u4PutDataPicCnt;
    UINT32 u4PutDataPicDoneCnt;
    UINT32 u4ContainerMetadata;
    UINT32 u4FordwardRefPts;
    UINT32 u4BackwardRefPts;
    UINT32 u4FordwardRefTr;
    UINT32 u4BackwardRefTr;
    UINT32 u4SecureInputStartAddr;
    UINT32 u4SecureInputEndAddr;
    UINT64 u8PrePts;
    ENUM_VDEC_FRAME_RATE_T eFrameRate;
    UINT64 u8TotalPushSize;
    UINT64 u8TotalPushSizeBak;
    UINT64 u8LastUndecodeSize;
    UINT64 u8FordwardRefPts;
    UINT64 u8BackwardRefPts;
    
    HANDLE_T hThread;
    HANDLE_T hMsgQ;
    HANDLE_T hDataTimer;
    HANDLE_T hStopSema;
    HANDLE_T hPutDataSema;
    HANDLE_T hMoveEsmQSema;
    HANDLE_T hFlushSema;
    HANDLE_T hIoQSema;
    //VPUSH_RV_SLICE_BUF
    HANDLE_T hRvMoveSema;
    VDEC_MSG_INTO_T rMsg;
    ENUM_VPUSH_STATE_T eCurStateSync;
    ENUM_VPUSH_STATE_T eCurState;
    ENUM_VPUSH_STATE_T ePreviousState;
    ENUM_VDEC_FMT_T eFmt;
    ENUM_VDEC_PUSH_FMT_T ePushFmt;
    VDEC_INPUT_STREAM_T rInpStrm;
    VDEC_BUF_INFO_T arBufInfo[VDEC_BUF_COUNT];
    VDEC_DECRYPT_INFO_T rDecryptInfo;
    BOOL fgInputBufReady;   //JPEG only
    UINT32 u4NaluSizeRemain;    //nalu data remain size
    UINT32 u4NaluSizePending;   //nalu data size value, start code position at push data boundary
    UINT8 u1StartCodeLenMinus1Remain; //nalu data size remain byte
    BOOL fgPacketAppend;
    UINT32 u4PacketSize;
     //VPUSH_RV_SLICE_BUF
    UINT32 *pu4VPSliceSzBuf ;
    UINT32 dwCrntFrmSliceSzWp; // the current frame 1st slice sz wp! to sendES!
    UINT8   u1CrntFrmSliceNum;
    UINT32 dwVPSliceSzBufWp;
    UINT32 dwVPSliceSzBufWpBak; // to record the latest frame last slice sz wp!for divided frames!
    UINT32 dwFirst4CC;
    FBM_POOL_T* prFeederPool;
    FBM_POOL_T* prDmxPool;  
    UINT32 u4VFifoAddr;
    UINT32 u4VFifoSize;
    VDEC_PES_INFO_T arPesInfo[10];
    UINT8 ucPesIndex;
    VDEC_BYTES_INTO_T* pCurrentInputFrame;
    UINT32 u4BufCnt;
    INT32 ErrorID;
} VDEC_T;

typedef struct {
    BOOL fgInited;
    BOOL fgOccupied[VDEC_PUSH_MAX_DECODER];
    VDEC_T arDec[VDEC_PUSH_MAX_DECODER];
} VDEC_DECODER_T;

typedef struct{
    UINT32 biSize;
    UINT32 biWidth;
    UINT32 biHeight;
    UINT16 biPlanes;
    UINT16 biBitCount;
    UINT32 biCompression;
    UINT32 biSizeImage;
    UINT32 biXPelsPerMeter;
    UINT32 biYPelsPerMeter;
    UINT32 biClrUsed;
    UINT32 biClrImportant;
}asf_bitmapinfoheader_s;

/********************************************************************
  EXTERN VARIABLES & FUNCTION PROTOTYPES DECLARATIONS
********************************************************************/
extern BOOL _VPUSH_SetupFifoBuffer(VOID* prdec);
extern BOOL _VPUSH_PutDataDone(VOID* prdec, UINT32 u4Tag);
extern BOOL _VPUSH_Decode(VOID* prdec);
#endif // VDEC_PUSH_H

