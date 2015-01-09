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
 * $RCSfile: b2r_hal.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file b2r_hal.h
 *  This header file of b2r hal 
 */


#ifndef _B2R_HAL_H_
#define _B2R_HAL_H_
#include "x_typedef.h"
#include "fbm_drvif.h"
#include "vdp_hal.h"

typedef enum
{
    B2R_ISR_VEI,
    B2R_ISR_ADI,
    B2R_ISR_FI,
} B2R_HAL_ISR_TYPE_T;


typedef enum
{
    B2R_HAL_3DTYPE,
    B2R_HAL_TIMING,
    B2R_HAL_DRAM_LAYOUT,
    B2R_HAL_COLOR_FMT,
    B2R_HAL_ADDR,
    B2R_HAL_ADDR_LEN,
    B2R_HAL_MIRROR_FLIP,
    B2R_HAL_OMUX,
    B2R_HAL_VB1,
    B2R_HAL_SMART_PCR,
    B2R_HAL_FRC,
    B2R_HAL_PATTERN,
    B2R_HAL_LR_STATE,
    B2R_HAL_TB_STATE,    
    B2R_HAL_BURST_READ,
    B2R_HAL_PITCH,
    B2R_HAL_SET_RESOLUTION,
    B2R_HAL_COLOR_FMT_CHG,
    B2R_HAL_TYPE_MAX
} B2R_HAL_TYPE_T;


typedef enum
{
    B2R_HAL_STATUS,
    B2R_HAL_DISP_INFO,
    B2R_HAL_GET_TIMING,
    B2R_HAL_GET_COLOR_FMT,
    B2R_HAL_GTYPE_MAX
} B2R_HAL_GET_TYPE_T;

typedef enum
{
    B2R_HAL_IN_BLANK_REGION,
    B2R_HAL_ENABLE_ST,
    B2R_HAL_ISTYPE_MAX
} B2R_HAL_IS_TYPE_T;


typedef enum
{
    B2R_CM_420,
    B2R_CM_422,
    B2R_CM_444,
} B2R_COLOR_MODE_T;

typedef enum
{
    B2R_AM_BLOCK4 = 0,
    B2R_AM_BLOCK1 = 1,
    B2R_AM_RASTER = 2,
} B2R_ADDR_MODE_T;

typedef enum
{
    B2R_SM_SWAP0   = 1,
    B2R_SM_NO_SWAP = 2,
    B2R_SM_SWAP1   = 5,
    B2R_SM_SWAP2   = 6,
} B2R_SWAP_MODE_T;

typedef enum
{
    B2R_PM_NORMAL  = 0x00,
    B2R_PM_16bit   = 0x01,
    B2R_PM_COMPACT = 0x10,
    B2R_PM_REORDER = 0x11,
    B2R_PM_COMPACT_TILE = 0x20,
} B2R_PACK_MODE_T;


typedef struct 
{
    VOID * pvParent;
} B2R_HAL_CFG_T;


typedef struct 
{
    UINT32 u4SrcW;
    UINT32 u4SrcH;
    UINT32 u4HTotal;
    UINT32 u4VTotal;
    UINT32 u4Clock;
    UINT32 u4Rate;
    UINT32 u4Scale;
    BOOL   fgProg;
} B2R_HAL_TIMING_T;

typedef struct 
{
    BOOL   fg3D;
    UINT32 u4Layout;
    UINT32 u4Data;
} B2R_HAL_3D_T;

typedef struct 
{
    B2R_ADDR_MODE_T eAddrMode;
    B2R_SWAP_MODE_T eSwapMode;
    B2R_PACK_MODE_T ePackMode;    
    UINT32 u4Pitch;
} B2R_HAL_DRAM_LAYOUT_T;

typedef struct 
{
    UINT8    u1Depth;
    BOOL     fgUfoEn;
    B2R_COLOR_MODE_T eMode;
} B2R_HAL_COLOR_FMT_T;

typedef struct 
{
    BOOL fgMirror;
    BOOL fgFlip;
} B2R_HAL_MF_T;

typedef struct
{
    UCHAR ucPath;
    BOOL  fgScartOut;
} B2R_HAL_OMUX_T;

typedef struct 
{
    UINT32  u4Y0;
    UINT32  u4C0;
    UINT32  u4Y1;
    UINT32  u4C1;
} B2R_HAL_ADDR_T;

typedef struct 
{
    UINT32  u4YLen0;
    UINT32  u4CLen0;
    UINT32  u4YLen1;
    UINT32  u4CLen1;
} B2R_HAL_ADDR_LEN_T;


typedef struct 
{
    UINT32  u4CrC;
    UINT32  u4LineCnt;
    BOOL    fgLeft;
    BOOL    fgTop;
    UINT32  u4Status;
    UINT32  u4UnderFlow;
    UINT32  u4CurrentField;
    UINT32  u4CurrentLR;
    UINT32  u4CurrentOutputField;
    UINT32  u4DramChkSum;
    UINT32  u4PSCANSrcCrc;
    UINT32  u4PSCANOutCrc;
    UINT32  u4MJCSrcCrc;
    UINT32  u4MJCOutCrc;
    B2R_HAL_ISR_TYPE_T eIsrType;  
} B2R_HAL_STATUS_T;

typedef enum
{
    B2R_FRC_FIELD_T = 0,
    B2R_FRC_FIELD_B,
    B2R_FRC_FIELD_MAX
} B2R_HAL_FRC_FIELD_T;

typedef struct
{
    UCHAR u2TargetNs;
    UCHAR u2ChangeFieldNs;
    UCHAR ucMode;
} B2R_HAL_FRC_ARG_T;

typedef struct
{
    UCHAR ucFrcEn;
    UCHAR u2TargetNs;
    UCHAR u2ChangeFieldNs;
    UCHAR ucCurrentNs;
    UCHAR ucMode;
    B2R_HAL_FRC_FIELD_T eRepeatField;
} B2R_HAL_FRC_T;


typedef enum
{
    B2R_TP_NO_GRID = 0,
    B2R_TP_H_BAR,
    B2R_TP_V_BAR,
    B2R_TP_H_OR_V,
    B2R_TP_H_XOR_V,
    B2R_TP_H_AND_V,
    B2R_TP_H_XNOR_V,
    B2R_TP_H_NOR_V
} B2R_HAL_TP_MODE_T;


typedef struct
{
    BOOL fgEnable;
    B2R_HAL_TP_MODE_T eMode;
    UINT32 u4Color;
    UINT32 u4Reserve;
} B2R_HAL_TP_T;

typedef struct
{
    UINT32 u4YAddr;
    UINT32 u4CAddr;
    UINT32 u4With;             // active  width
    UINT32 u4Height;           // active  height
    UINT32 u4Pitch;            // Line Size of buffer
    B2R_ADDR_MODE_T eAddrMode;
} B2R_HAL_DISP_INFO_T;

typedef struct _B2R_HAL_OBJECT_T
{
    //common
    CHAR    acName[12];
    UINT8   u1Id;
    UINT8   u1Used;
    BOOL    fgEnable;
    VOID*   pvHost;
   
    //function
    UINT32      (*Reset)     (struct _B2R_HAL_OBJECT_T *);
    UINT32      (*Enable)    (struct _B2R_HAL_OBJECT_T *);
    UINT32      (*Disable)   (struct _B2R_HAL_OBJECT_T *);
    UINT32      (*Config)    (struct _B2R_HAL_OBJECT_T *);
    UINT32      (*Set)       (struct _B2R_HAL_OBJECT_T *, B2R_HAL_TYPE_T, VOID *);
    UINT32      (*Get)       (struct _B2R_HAL_OBJECT_T *, B2R_HAL_GET_TYPE_T, VOID *);
    UINT32      (*Is)        (struct _B2R_HAL_OBJECT_T *, B2R_HAL_IS_TYPE_T, VOID *);
    BOOL        (*IsSupport) (struct _B2R_HAL_OBJECT_T *, VOID *);
    VOID        (*Isr)       (UINT16);

    UINT8 begin;
    //local 
    BOOL  fgMirror;
    BOOL  fgFlip;
    BOOL  fgVB1Enable;//VB1 enable
    UCHAR ucDmxPcrId;
    UINT32 u4InsideVActRegion;
    B2R_HAL_3D_T t3D;
    B2R_HAL_TIMING_T tTiming;
    B2R_HAL_DRAM_LAYOUT_T tDramLayout;
    B2R_HAL_COLOR_FMT_T tColorFmt;
    B2R_HAL_STATUS_T tStatus;

    //for frc
    B2R_HAL_FRC_T tFrc;

    UINT8 end;
}B2R_HAL_OBJECT_T;

#define B2R_HAL_COMM \
    VOID* pvData; \
    VOID  (*IsrCb)(VOID *, VOID *); \


typedef struct 
{
    B2R_HAL_COMM;
} B2R_HAL_COMM_T;


typedef struct 
{
    UINT32 u4Data;
    VOID * pvData;
} B2R_ICfG_T;

typedef struct 
{
    UINT32 u4Hactive;
    UINT32 u4Vactive;
} B2R_IC_SPEC_T;

//return value
#define B2R_HAL_OK                     (0)
#define B2R_HAL_FAIL                   (1)
#define B2R_HAL_INV_ARG                (2)
#define B2R_HAL_INTERNAL_ERR           (3)
#define B2R_HAL_NOT_IMPL               (4)

#define B2R_HAL_TRACE(fmt...)          LOG(11, fmt)
#define B2R_MK_RET(ret)                ((__LINE__<<8)|(ret))     
#define B2R_RET(ret)                   ((ret)&0x000000FF)     
#define B2R_MK_REALD_DATA(tag,offset)  (((tag)<<16)|(offset))

#define B2R_NULL_HANDLE                ((void *)0)
typedef VOID * B2R_HANDLE_T;

//#define B2R_EMU
#define B2R_SUPPORT_V2HW


#define B2R_FRC_NORMAL                      0
#define B2R_FRC_NONE                        1   // no FRC
#define B2R_FRC_STOP_AUTO                   2   // stop auto at last field
#define B2R_FRC_TRICK_PLAY                  3   // Trick Play at 20081001
#define B2R_FRC_CANCEL                      4   // vdp cancel for 4 frame buffer at 20090630

//#define NEW_B2R_SINGLE_TEST

#define VALIGN_SIZE    32
#define ALIGN_WITH32(u4_length) (((u4_length)%32) == 0)

#define HD_HW_ID    0
#define SD_HW_ID    1

#define HDE_RATIO_MIN     512
//-----------------------------------------------------------------------------
// Prototype  of Inter-file functions
//-----------------------------------------------------------------------------
UINT32 _B2R_HAL_SetFrc(B2R_HAL_OBJECT_T *this, UCHAR u2TargetNs, UCHAR u2ChangeFieldNs, UCHAR ucMode);

UINT32 _B2R_HW_Select( UINT8 id);
UINT32 _B2R_VB1_Setting( UINT32 u4Pitch,B2R_PACK_MODE_T ePackMode,UINT8 u1Depth,UINT32 u4SrcW,UINT32 u4SrcH,UINT32 u4Y0,UINT32 u4C0,UINT32 u4YLen,UINT32 u4CLen);
void B2R_HW_Enable(BOOL fgEnable);///For VB1 bypass used


//-----------------------------------------------------------------------------
// Prototype  of public functions
//-----------------------------------------------------------------------------
VOID B2R_HAL_Init(VOID);
B2R_HANDLE_T B2R_HAL_Open(VOID* pvArg);
UINT32 B2R_HAL_Close(B2R_HANDLE_T hB2R);
UINT32 B2R_HAL_Get(B2R_HANDLE_T hB2R, B2R_HAL_GET_TYPE_T eType, VOID *pvArg);
UINT32 B2R_HAL_Set(B2R_HANDLE_T hB2R, B2R_HAL_TYPE_T eType, VOID *pvArg);
BOOL   B2R_HAL_Is(B2R_HANDLE_T hB2R, B2R_HAL_IS_TYPE_T eType, VOID *pvArg);
UINT32 B2R_HAL_Disable(B2R_HANDLE_T hB2R);
UINT32 B2R_HAL_Enable(B2R_HANDLE_T hB2R);
UINT32 B2R_HAL_Reset(B2R_HANDLE_T hB2R);
UINT32 B2R_HAL_Config(B2R_HANDLE_T hB2R);

#endif /*_B2R_HAL_H_*/
