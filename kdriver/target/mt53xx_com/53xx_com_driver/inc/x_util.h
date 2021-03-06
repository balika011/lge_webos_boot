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
#ifndef __X_UTIL_H__
#define __X_UTIL_H__

#include "x_typedef.h"

#ifndef CC_MT5301A
#define NEW_COUNTRY_TYPE
#endif


#ifdef NEW_COUNTRY_TYPE

#define COUNTRY_AUDIO_POS    5
#define COUNTRY_VBI_POS     4 
#define COUNTRY_ATV_POS    3
#define COUNTRY_DTV_POS    0

#define COUNTRY_AUDIO_MASK      ((UINT32)(7 << COUNTRY_AUDIO_POS))
#define COUNTRY_VBI_MASK  ((UINT32)(1 << COUNTRY_VBI_POS))
#define COUNTRY_ATV_MASK   ((UINT32)(1 << COUNTRY_ATV_POS))
#define COUNTRY_DTV_MASK   ((UINT32)(7 << COUNTRY_DTV_POS))


typedef enum {
    COUNTRY_AUD_US = 0,
    COUNTRY_AUD_EU,
    COUNTRY_AUD_BZ,
    COUNTRY_AUD_AU,
    COUNTRY_AUD_CN,
    COUNTRY_AUD_JP,
    COUNTRY_AUD_TW,
    COUNTRY_AUD_AP
} COUNTRY_AUD;

typedef enum {
    COUNTRY_VBI_CC = 0,
    COUNTRY_VBI_TELETEXT
} COUNTRY_VBI; 

typedef enum {
    COUNTRY_ATV_US = 0,
    COUNTRY_ATV_EU
} COUNTRY_ATV;

typedef enum {
    COUNTRY_DTV_ATSC = 0,
    COUNTRY_DTV_DVBT,
    COUNTRY_DTV_DTMB,
    COUNTRY_DTV_ISDBT,
    COUNTRY_DTV_DUMMY
} COUNTRY_DTV;

#define MAKE_TARGET_COUNTRY(aud,vbi,atv,dtv) ((aud<<COUNTRY_AUDIO_POS) |(vbi<<COUNTRY_VBI_POS)|(atv<<COUNTRY_ATV_POS)|(dtv<<COUNTRY_DTV_POS)) 

#define COUNTRY_US  MAKE_TARGET_COUNTRY(COUNTRY_AUD_US, COUNTRY_VBI_CC, COUNTRY_ATV_US, COUNTRY_DTV_ATSC)
#define COUNTRY_EU  MAKE_TARGET_COUNTRY(COUNTRY_AUD_EU, COUNTRY_VBI_TELETEXT, COUNTRY_ATV_EU, COUNTRY_DTV_DVBT)
#define COUNTRY_JP  MAKE_TARGET_COUNTRY(COUNTRY_AUD_JP, COUNTRY_VBI_CC, COUNTRY_ATV_US, COUNTRY_DTV_ISDBT)
#define COUNTRY_TW  MAKE_TARGET_COUNTRY(COUNTRY_AUD_TW, COUNTRY_VBI_CC, COUNTRY_ATV_US, COUNTRY_DTV_DVBT)
#define COUNTRY_BZ  MAKE_TARGET_COUNTRY(COUNTRY_AUD_BZ, COUNTRY_VBI_CC, COUNTRY_ATV_US, COUNTRY_DTV_ATSC)
#define COUNTRY_PAA MAKE_TARGET_COUNTRY(COUNTRY_AUD_AP, COUNTRY_VBI_TELETEXT, COUNTRY_ATV_EU, COUNTRY_DTV_DUMMY)
#define COUNTRY_PAD MAKE_TARGET_COUNTRY(COUNTRY_AUD_EU, COUNTRY_VBI_TELETEXT, COUNTRY_ATV_EU, COUNTRY_DTV_DVBT)

#else

#define SCAN_ATV_POS    3
#define SCAN_DTV_POS    0

#define COUNTRY_CC_ENABLE_MASK  (UINT32)(1 << 7)
#define COUNTRY_AUDIO_MASK      (UINT32)(3 << 5)
#define COUNTRY_SCAN_ATV_MASK   (UINT32)(3 << SCAN_ATV_POS)
#define COUNTRY_SCAN_DTV_MASK   (UINT32)(7 << SCAN_DTV_POS)

typedef enum {
    COUNTRY_US = 0,
    COUNTRY_EU,
    COUNTRY_DUMMY,
    COUNTRY_JP,
    COUNTRY_TW,    
    COUNTRY_BZ,    
} TARGET_COUNTRY;
#endif


typedef enum {
    eNO_TYPE,
    eRD_ONLY,
    eWR_ONLY,
    eRD_WR
} REGTYPE;


typedef struct strucRegTest {
    UINT32     u4Addr;		// the register offset
    REGTYPE    eRegType;
    INT32      iRegLen;		// 1, 2, or 4; number of byte
	UINT32     u4Mask;		// the valid bit mask
	INT32      fgWDfV;		// set 0 if without reset value.
	UINT32     u4DfVal;		// the reset value.
} REG_TEST_T;

/* Log export function. */
extern INT32 UTIL_LogThreadInit(void);
extern INT32 UTIL_Log(const CHAR *szFmt, ...);
extern void UTIL_LogFlush(void);

/* Cli util functions. */
extern INT32 UTIL_MemOrder(UINT32 u4Addr, UINT32 u4ByteUnit, UINT32 u4Len, UINT32 u4Step);
extern INT32 UTIL_MemCmp(UINT32 u4Addr1, UINT32 u4Addr2, UINT32 u4Len);

/* Reg test functions. */
INT32 UTIL_AllSpaceRWTest(UINT32 u4BaseAddr, UINT32 u4Length);
INT32 UTIL_RegDefChk(UINT32 u4BaseAddr, REG_TEST_T *psrgtRegList);
INT32 UTIL_RegRWTest(UINT32 u4BaseAddr, REG_TEST_T *psrgtRegList);

#ifndef CC_MTK_LOADER
/*
 * Do u8Dividend / u8Divider.
 * u8Divider value should be < max 32 bit integer number.
 * If pu8Remainder is not NULL, remainder will be returned.
 * The function returns quotient.
 */
extern UINT64 u8Div6432(UINT64 u8Dividend, UINT64 u8Divider, UINT64 *pu8Remainder);
extern UINT64 u8Div6464(UINT64 u8Dividend, UINT64 u8Divider, UINT64 *pu8Remainder);
#endif

/**
 * Set country.
 *
 * @param u4Country country selection
 * @return void
 */
void SetTargetCountry(UINT32 u4Country);

/**
 * Get country setting.
 *
 * @param void
 * @return country selection
 */
UINT32 GetTargetCountry(void);

/**
 * Set HID KB country.
 *
 * @param u4Country country selection
 * @return void
 */
void SetHidKbCountry(UINT32 u4Country);

/**
 * Get HID KB country setting.
 *
 * @param void
 * @return country selection
 */
UINT32 GetHidKbCountry(void);
#ifdef NEW_COUNTRY_TYPE
/**
 * Get country setting of sub-module.
 *
 * @param u4Mask sub-module mask
 * @return country selection
 */
UINT32 GetTargetCountryMask(UINT32 u4Mask);
#endif

#endif /* __X_UTIL_H__ */

