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
 * $RCSfile: fbm_fb.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file sample.h
 *  Brief of file sample.h.
 *  Details of file sample.h (optional).
 */

#ifndef FBM_FB_H
#define FBM_FB_H


//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------

#include "fbm_drvif.h"
#include "fbm_cs.h"
#include "x_typedef.h"


//---------------------------------------------------------------------------
// Configurations
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Constant definitions
//---------------------------------------------------------------------------

// Maximum frame buffer number per frame buffer group
#ifdef DEBUG_MAX_FB
#define FBM_MAX_FB_NS_PER_GROUP		64//32
#else
//for H264
//#define FBM_MAX_FB_NS_PER_GROUP     10
//#define FBM_MAX_FB_NS_PER_GROUP     18
#define FBM_MAX_FB_NS_PER_GROUP     44//22
#endif

// Reference buffer number per frame buffer group
#define FBM_REF_FB_NS_PER_GROUP		2

// cabac info buffer per frame buffer group for HEVC and H264
#define FBM_MAX_CABAC_BUF_NS_PER_GROUP 2

//---------------------------------------------------------------------------
// Type definitions
//---------------------------------------------------------------------------

/** Brief of FBM_FBQ_T
 *  Frame Buffer Queue
 */
typedef struct
{
    UCHAR ucReadIdx;
    UCHAR ucCount;
    BOOL fgTrueDispQ;
    UCHAR aucQueue[FBM_MAX_FB_NS_PER_GROUP];
} FBM_FBQ_T;

typedef enum
{
    FBM_DYNAMIC_MODE_NONE,
    FBM_DYNAMIC_DIFF,    //From Two Pools,  
    FBM_DYNAMIC_SAME,    //From One Pool,   
    FBM_DYNAMIC_MODE_MAX,
} FBM_SEAMLESS_FB_MODE_T;

typedef struct
{
    UINT32 u4Addr;
    UINT32 u4Size;
    BOOL fgInUse;
} FBM_SEAMLESS_POOL_T;


typedef enum
{
    FBM_RESOLUTION_CHANGE_MODE_NONE,
    FBM_RESOLUTION_CHANGE_MODE_FHD,
    FBM_RESOLUTION_CHANGE_MODE_4K2K,
    FBM_RESOLUTION_CHANGE_MODE_NS,
} FBM_RESOLUTION_CHANGE_MODE_T;


/** Brief of FBM_FBG_T
 *  Frame Buffer Group
 */
typedef struct
{
    BOOL  fgEnableNewSeamless;
    BOOL  fgRPRMode;
    BOOL  fgAutoSeamless;
    UCHAR ucFbgType;                        // Frame Buffer Group Type
    UCHAR ucFbgId;                          // Frame Buffer Group ID
    UCHAR ucFbgCm;                          // Frame Buffer Color Mode
    UCHAR ucFbNs;                           // Frame Buffer Number
    UCHAR ucFbNsBase;                       // Frame Buffer Number
    UCHAR ucFbNsOld;                        // Frame Buffer Number
    UCHAR ucFbNsOldBase;                    // Frame Buffer Number
    UCHAR ucMvBufNs;                        // MV Buffer Number
    UCHAR ucSeamlessFbNs;
    UCHAR ucSeamless2FbNs;	
    UCHAR ucCurSeamlessTaskId;
    UCHAR ucSeamlessRzId;
    UCHAR ucSeamlessRzId2;
    UCHAR ucSeamlessRzMask;
    UCHAR ucFbDecode;                       // Current Decoding FB
    UCHAR ucFbFRef;                         // Forward Reference FB
    UCHAR ucFbBRef;                         // Backward Reference FB
    UCHAR ucFbResize;                       // Current Resizing FB
    UCHAR ucFbResize2;                       // Current Resizing FB
    UCHAR ucFbFRefValid;
    UCHAR ucFbBRefValid;

    UCHAR ucStcSrc;                         // AV Sync Source
    UCHAR ucAvSyncMode;                     // AV Sync Mode

    UCHAR ucPlayMode;                       // DTV or MM or ...
    UCHAR ucRecordNextFbId;
    UCHAR ucNewFbId;
    UCHAR ucSeqChg;
    UINT32 u4SeqChgFlag;
    BOOL  fgGotFirstSeqChg;
    UCHAR ucLastDecodeReference;            // Last Decode Picture is Reference
    UCHAR aucResizeBlockFb;     //Sema lock block FB for wait empty RzFb
    UCHAR aucResize2BlockFb;     //Sema lock block FB for wait empty RzFb
    UCHAR eDynamicMode;         //Dynamic change FBs..
    UCHAR ucFbOldEmptyCnt;      //Dynamic mode: Old  FBs empty record.
    UCHAR auc2FBsConflictListCnt[FBM_MAX_FB_NS_PER_GROUP];
    UCHAR auc2FBsConflictList[FBM_MAX_FB_NS_PER_GROUP][(FBM_MAX_FB_NS_PER_GROUP/2-1)/7+1+1]; // +1 for MV, +1 for
    UCHAR aucFbStatus[FBM_MAX_FB_NS_PER_GROUP];
    UCHAR aucResizeFbStatus[FBM_MAX_FB_NS_PER_GROUP];
    UCHAR aucResizeTaskId[FBM_MAX_FB_NS_PER_GROUP];
    UCHAR aucResizeSrcFbId[FBM_MAX_FB_NS_PER_GROUP];
    UCHAR aucResize2FbStatus[FBM_MAX_FB_NS_PER_GROUP];
    UCHAR aucResize2TaskId[FBM_MAX_FB_NS_PER_GROUP];
    UCHAR aucResize2SrcFbId[FBM_MAX_FB_NS_PER_GROUP];
    UCHAR aucPlayBackNs[FBM_MAX_FB_NS_PER_GROUP];
    UCHAR aucChangeFieldNs[FBM_MAX_FB_NS_PER_GROUP];
    UCHAR aucChgFldPlayBackNs[FBM_MAX_FB_NS_PER_GROUP];
    BOOL   afgRefList[FBM_MAX_FB_NS_PER_GROUP];    //for H264 to maintain reference list
    BOOL   afgIsOwner[FBM_MAX_FB_NS_PER_GROUP]; // Is fb owned by FBM or by others, like OMX
    BOOL   fgCreateFromInst;                 // Init fbg from external FBM inst
    BOOL   fgSendEOS;

    UINT8 u1DecoderSrcId;
    UINT8 u1FbgAppMode;
    BOOL  fgEnableH264V5Support;

    HANDLE_T hChgFieldSema;

    UINT32 u4VDecFmt;                       //for mt539x various video decoder
    UINT32 u4FbgFlag;                       // Flag
    UINT32 u4FbWidth;                       // Frame Buffer Width, According to Color Mode
    UINT32 u4FbHeight;                      // Frame Buffer Height, According to Color Mode
    UINT32 u4WfdFlag;
    UINT32 u4_es_cnt;
    UINT32 u4ResidualFb;                    // Indicate whether residual FB is used
    UINT32 u4ResidualFree;                  // Indicate whether residual FB is pending to free
    UINT32 u4WorkBufSize;                   // The size for working buffer

    UINT32 u4FbMemoryPool;                  //Y Addr
    UINT32 u4FbMemoryPoolC;                 //C Addr support two dram channel.
    UINT32 u4FbMemoryPoolSize;
    UINT32 u4FbMemoryPool2;                 //2nd part for SBS/TNB pool type.
    UINT32 u4FbMemoryPool2C;                //2nd part for SBS/TNB pool type and two dram channel.
    UINT32 u4FbMemoryPool2Size;             //2nd part for SBS/TNB pool type and two dram channel.
    UINT32 u4FbResizeMemoryPool;
    UINT32 u4FbResizeMemoryPoolSize;
    UINT32 u4FbResizeMemoryPool2;
    UINT32 u4FbResizeMemoryPool2Size;
	/* 4K Seamless use*/
	BOOL fg4k2KPoolAlocate; 
	FBM_RESOLUTION_CHANGE_MODE_T eResolutionChangeMode; 
	UINT32 u4FbActiveFHDPool;
	UINT32 u4FbActiveFHDPoolC;
	UINT32 u4FbActiveFHDPool2;
	UINT32 u4FbActiveFHDPool2C;
	UINT32 u4FbActive4K2KPool;
	UINT32 u4FbActive4K2KPoolC;
	UINT32 u4FbActive4K2KPool2;
	UINT32 u4FbActive4K2KPool2C;
	/* ----------------------------*/
	/* check Frame buffer addr overlap */
	/* --------------------------*/
    UINT32 u4FbUsingPoolYSize;
    UINT32 u4FbUsingPool2YSize;	
    UINT32 u4FbUsingPoolCSize;
    UINT32 u4FbUsingPool2CSize;	
	
	UINT32 u4FbStartPoolYAddr;
	UINT32 u4FbEndPoolYAddr;
	UINT32 u4FbStartPool2YAddr;
	UINT32 u4FbEndPool2YAddr;
	
	UINT32 u4FbStartPoolCAddr;
	UINT32 u4FbEndPoolCAddr;
	UINT32 u4FbStartPool2CAddr;
	UINT32 u4FbEndPool2CAddr;
	/* --------------------------*/
    UINT32 u4AvSyncStatus;                  // AV sync status, repeat or drop
#ifdef CC_B2R_RES_SUPPORT
    B2R_HW_RES_T* pt_res;
#endif
#ifdef FBM_VIRTUAL_MPEG
    UINT32 u4VmStatus;
#endif

    void* pMallocAddr;                      // Memory allocation addr
    void* pWorkBufVirAddr;                  // WorkBuf Virtual Address
	BOOL fg10Bit;
	BOOL fgUFO;
    UINT32 au4AddrY[FBM_MAX_FB_NS_PER_GROUP];
    UINT32 au4AddrY_Ext[FBM_MAX_FB_NS_PER_GROUP];
    UINT32 au4AddrC[FBM_MAX_FB_NS_PER_GROUP];
    UINT32 au4AddrC_Ext[FBM_MAX_FB_NS_PER_GROUP];
    UINT32 au4AddrMv[FBM_MAX_FB_NS_PER_GROUP-1];
	UINT32 au4AddrCabac[FBM_MAX_CABAC_BUF_NS_PER_GROUP];
    UINT32 au4GfxMemsetId[FBM_MAX_FB_NS_PER_GROUP*2];  //0 means memset done. other means memset ing
    UINT32 au4GdmaMemsetYId[FBM_MAX_FB_NS_PER_GROUP];  //0 means memset done. other means memset ing    
    UINT32 au4GdmaMemsetCId[FBM_MAX_FB_NS_PER_GROUP];  //0 means memset done. other means memset ing    
    UINT32 au4ResizeAddrY[FBM_MAX_FB_NS_PER_GROUP];
    UINT32 au4ResizeAddrC[FBM_MAX_FB_NS_PER_GROUP];
	
    UINT32 au4Resize2AddrY[FBM_MAX_FB_NS_PER_GROUP];
    UINT32 au4Resize2AddrC[FBM_MAX_FB_NS_PER_GROUP];	

    UINT32 au4DbkOfstCnt[FBM_MAX_FB_NS_PER_GROUP];
    UINT32 au4Dbk4pCnt[FBM_MAX_FB_NS_PER_GROUP];
    UINT32 u4DispTag[FBM_MAX_FB_NS_PER_GROUP];
    UINT32 eSeamlessMode;
	UINT32 au4MiscTblStartAddr; //VP9 just use,segld tife count prob total size
    UINT32 u4ExtraYBuffer;
    UINT32 u4ExtraCBuffer;
    UINT32 u4RPRSeamlessBuffer;
    FBM_SEQ_HDR_T rSeqHdr;
    FBM_PIC_HDR_T* prPicHdr;
    FBM_FBQ_T rEmptyQ;
    FBM_FBQ_T rDisplayQ;
    FBM_FBQ_T rResizeEmptyQ;
    FBM_FBQ_T rResize2EmptyQ;	//two image:rz0 using ResizeEmptyQ,Rz1 using ResizeEmptyQ2 ,in order to fix two prescale trhead!!
    ENUM_CODEC_INFO_T eCodecType;

    COUNTING_SEMAPHORE_T hEmptyQSemaphore;  // Empty Queue Counting Semaphore
    COUNTING_SEMAPHORE_T hEmptyBQSemaphore; // Empty B Queue Counting Semaphore
    COUNTING_SEMAPHORE_T hEmptyResizeQSemaphore;
    COUNTING_SEMAPHORE_T hEmptyResize2QSemaphore; 
    HAL_TIME_T arRlResizeFb[FBM_MAX_FB_NS_PER_GROUP];

    HANDLE_T hMutex;                        // Thread safe protection
    CRIT_STATE_T* prState;                  // Thread/Isr safe protection
} FBM_FBG_T;


//---------------------------------------------------------------------------
// Macro definitions
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Inter-file functions
//---------------------------------------------------------------------------

extern void _FBM_FbInit(FBM_FBG_T* prFbg);

extern void _FBM_PutFrameBufferToEmptyQ(UCHAR ucFbgId, UCHAR ucFbId);

extern VOID _FBM_2FBsConflictList(UCHAR ucFbgId);

extern void _FBM_PutFrameBufferToEmptyResizeQ(UCHAR ucFbgId, UCHAR ucFbId);
extern void _FBM_PutFrameBufferToEmptyResize2Q(UCHAR ucFbgId, UCHAR ucFbId);
extern UCHAR FBM_GetLatestEmptyFrameBuffer(UCHAR ucFbgId, UINT32 u4Delay);

extern BOOL _FBM_2FBsChkDynamicDone(UCHAR ucFbgId);


//---------------------------------------------------------------------------
// Public functions
//---------------------------------------------------------------------------


#endif	// FBM_FB_H
