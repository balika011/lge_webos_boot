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
 * $RCSfile: aud_slt.c,v $
 * $Revision: #1 $
 * $Revision: #1 $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file aud_slt.c
 *  Brief of file aud_slt.c.
 *  Details of file aud_slt.c (optional).
 */
//#define __MODEL_slt__
//#define AUD_EXTERN_FILE

//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------
#ifdef __MODEL_slt__

#include "x_lint.h"

LINT_EXT_HEADER_BEGIN

#include "x_typedef.h"
#include "x_os.h"
#include "x_bim.h"
#include "x_assert.h"

#include "aud_debug.h"
#include "aud_slt.h"
#include "aud_if.h"
#include "aud_cfg.h"
#include "aud_drv.h"
#include "aud_hw.h"
#include "dsp_intf.h"
//#include "drv_slt.h"
#include "x_pinmux.h"
#include "x_drv_cli.h"
#include "codec_mt5365.h"
#include "nor_if.h"

#include "drv_adsp.h"
//#include "dsp_slt.c"
//#include "dsp_intf.c"

// Rocky Huang 070607 for compile
#include "x_timer.h"
//#include "dsp_reg.h"
#define PTR_SEL_MAINDEC_SPDIF_LINE_IN   (0x1 << 8)

#ifdef CC_5380_SLT
#define IAD_FROM_AIN1 0
#else
#define IAD_FROM_AIN1 1
#endif
LINT_EXT_HEADER_END

/****************************************************************************
** Local definitions
****************************************************************************/

// SLT common define
#define SLT_UNKNOWN_MODE                0
#define SLT_SORTING_MODE                1
#define SLT_FLASHING_MODE               2
#define SLT_TYPE_AUD 1

// Audio SLT sorting result
#define AUD_SORTING_OK                           0
#define AUD_SORTING_DTV_FAIL                 (-1)
#define AUD_SORTING_ATV_FAIL                 (-2)
#define AUD_SORTING_FAIL                        (-3)
#define AUD_SORTING_ATV_CAPTURE_FAIL (-4)
#define AUD_SORTING_ATV_DATA_FAIL       (-5)
#define AUD_SORTING_IAD_FAIL            (-6)
#define AUD_SORTING_ROM_FAIL       (-6)

// Remapping non-cache address
#define NOR_FLASH_BASE_ADDR                        (0xf8000000)
#define NOR_FLASH_BASE_ADDR_NON_CACHE        (0xf8000000)       /* Remapping non-cache address */
//#define NOR_SLT_START_ADDR                   (0x770000)     //code size should < 7M
#define NOR_SLT_START_ADDR                   (0x670000)     //oryx : 0x770000-> 670000
#if defined(CC_MT5398)
#define NOR_SLT_AUD_ADDR_OFFSET          (0x010000)
#else
#define NOR_SLT_AUD_ADDR_OFFSET                (0x000000)            // 0M
#endif

// DTV SLT setting
#define AUD_BITSTREAM_LEN                               (0x80000)           // 0.5M
#define AUD_SLT_CAPTURE_PERIOD                     (3000)   // 3 second
#define AUD_SLT_TIMEOUT                                   (AUD_SLT_CAPTURE_PERIOD * 2)

#define VALID_STREAM_LENGTH                           (0x1800 * 3)
#define LINE_IN_BUF_SA(DspId)                           (u4GetAFIFOStart(DspId, AUD_DEC_AUX))
#define LINE_IN_BUF_SZ(DspId)                           (u4GetAFIFOEnd(DspId, AUD_DEC_AUX) - u4GetAFIFOStart(DspId, AUD_DEC_AUX))
#define INT_CNT                                                  ((LINE_IN_BUF_SZ(AUD_DSP0) / (256 * 4)) - 100)

// FFT configuration
#define N 4096						//FFT data length
#define BITNUM 32						//FFT data bit
#define FFT_PIONT_NUM                     (4096)

// FFT result and threshold
//#define BITTYPE int
#define OVS 889516850
#define DOVS 1779033700
#define WNS 0x7FFFFFFF
#define SHN  31
#define FREQ_POINT                          14   // 1k/288k*4096
#define DC_VALUE_MAX_THRESHOLD  5
#define DC_VALUE_MIN_THRESHOLD   (0.5)
#define THD_PLUS_N_THRESHOLD       70000
#define MAG_MIN_THRESHOLD            (10)
#define MAG_MAX_THRESHOLD           (400)

#define SLT_CH_NUM                           2
#define FREQ_POINT_AD                       85  // 1K/48K*4096
#define THD_PLUS_N_THRESHOLD_AD            150000//60000

#define FREQ_POINT_ATD0                       85//128  // 1K/32K*4096
#define FREQ_POINT_ATD1                       86//129  // 1K/32K*4096

#define int64 long long

struct Complex
{                           
    int real;
    int imag;
};
struct Complex atd[SLT_CH_NUM][FFT_PIONT_NUM],ad[SLT_CH_NUM][FFT_PIONT_NUM];
typedef struct Complex COMPLEX;
COMPLEX wn[N/2],XkWn;
/****************************************************************************
** Local structures and enumerations
****************************************************************************/

/****************************************************************************
** Function prototypes
****************************************************************************/
void SLT_CaptureReady(UINT32 u4Type, UINT32 u4Pts);
extern void AUD_StopMemStr(void);
extern void Fixed_fft(COMPLEX * x);

extern UINT32 AUD_DspProbePause(UINT8 u1DspId);
extern UINT32 AUD_DspProbeGo(UINT8 u1DspId);
extern void vResetDSP(UINT8 u1DspId);
extern void DSP_SetAenvSemaCtrl (UINT8 u1DspId, UINT16 u2Ctrl);
extern void DSP_SetAenvSemaCtrlToDSP (UINT8 u1DspId);

extern void AUD_pm_resume(void);
extern void AUD_pm_suspend(void);

extern UINT32 u4AUD_DspProbeRead(UINT8 u1DspId, UINT32 u4Addr);
extern void AUD_DspPatternTest(UINT8 u1DspId, UINT8 u1PatId,UINT8 u1ShowHelp);
/****************************************************************************
** Local variable
****************************************************************************/

// For sampling rate 48 KHz
static UINT32 _u4SltStreamLen = (AUD_SLT_CAPTURE_PERIOD*48*2*2);
static INT32 _i4AudSltResult = 0;
//static HANDLE_T _hAudCaptureTimer = NULL;

static HANDLE_T _hSltAud;
static HANDLE_T _hSltAudFinish = NULL;
static HANDLE_T _hSltAudThread;

static UINT32 _u4SltAudMode = SLT_UNKNOWN_MODE;
static HAL_TIME_T _rTime;

#define AUD_SLT_COMPARED_LEN                  (_u4SltStreamLen * 3/5)

static INT32 Magnitued[SLT_CH_NUM][FFT_PIONT_NUM];

static INT32 _i4AudDspbDtvFlashingFlag = 0;

/****************************************************************************
** Local functions
****************************************************************************/

extern const UINT8 _au1Stream[];

/***********************************************************
* Function      : build_win
* Description   : build the window table for inplace_fft
* Parameter     : argu 1: (i): length
                  argu 2: (o): buffer address window real part
                  argu 3: (o): buffer address window image part
* Return        : None
***********************************************************/

/***********************************************************
*Find the inverse subscript of current subscript
***********************************************************/
 static const int Cos_PAR[] =
{
    
    0x7fffffff, 0x7ffff620, 0x7fffd884, 0x7fffa72b, 0x7fff6215, 0x7fff0941, 0x7ffe9cb1, 0x7ffe1c63,
    0x7ffd8859, 0x7ffce092, 0x7ffc250e, 0x7ffb55cd, 0x7ffa72d0, 0x7ff97c16, 0x7ff871a0, 0x7ff7536e,
    0x7ff62181, 0x7ff4dbd7, 0x7ff38272, 0x7ff21552, 0x7ff09476, 0x7feeffe0, 0x7fed578f, 0x7feb9b84,
    0x7fe9cbbe, 0x7fe7e83f, 0x7fe5f107, 0x7fe3e615, 0x7fe1c76a, 0x7fdf9507, 0x7fdd4eeb, 0x7fdaf517,
    0x7fd8878c, 0x7fd6064a, 0x7fd37151, 0x7fd0c8a2, 0x7fce0c3d, 0x7fcb3c22, 0x7fc85852, 0x7fc560cd,
    0x7fc25595, 0x7fbf36a8, 0x7fbc0409, 0x7fb8bdb6, 0x7fb563b1, 0x7fb1f5fb, 0x7fae7493, 0x7faadf7b,
    0x7fa736b3, 0x7fa37a3b, 0x7f9faa14, 0x7f9bc63e, 0x7f97cebb, 0x7f93c38b, 0x7f8fa4ae, 0x7f8b7225,
    0x7f872bf1, 0x7f82d213, 0x7f7e648a, 0x7f79e359, 0x7f754e7e, 0x7f70a5fc, 0x7f6be9d3, 0x7f671a03,
    0x7f62368e, 0x7f5d3f74, 0x7f5834b5, 0x7f531653, 0x7f4de44f, 0x7f489ea9, 0x7f434562, 0x7f3dd87b,
    0x7f3857f4, 0x7f32c3cf, 0x7f2d1c0d, 0x7f2760ae, 0x7f2191b2, 0x7f1baf1c, 0x7f15b8ec, 0x7f0faf23,
    0x7f0991c2, 0x7f0360c9, 0x7efd1c3b, 0x7ef6c417, 0x7ef0585e, 0x7ee9d912, 0x7ee34634, 0x7edc9fc5,
    0x7ed5e5c5, 0x7ecf1836, 0x7ec83719, 0x7ec1426e, 0x7eba3a38, 0x7eb31e76, 0x7eabef2b, 0x7ea4ac57,
    0x7e9d55fb, 0x7e95ec18, 0x7e8e6eb0, 0x7e86ddc4, 0x7e7f3955, 0x7e778164, 0x7e6fb5f2, 0x7e67d701,
    0x7e5fe492, 0x7e57dea5, 0x7e4fc53d, 0x7e47985a, 0x7e3f57fd, 0x7e370429, 0x7e2e9cde, 0x7e26221d,
    0x7e1d93e8, 0x7e14f241, 0x7e0c3d28, 0x7e03749e, 0x7dfa98a6, 0x7df1a941, 0x7de8a66f, 0x7ddf9033,
    0x7dd6668d, 0x7dcd2980, 0x7dc3d90c, 0x7dba7533, 0x7db0fdf6, 0x7da77358, 0x7d9dd559, 0x7d9423fa,
    0x7d8a5f3e, 0x7d808726, 0x7d769bb4, 0x7d6c9ce8, 0x7d628ac4, 0x7d58654b, 0x7d4e2c7d, 0x7d43e05d,
    0x7d3980eb, 0x7d2f0e29, 0x7d248819, 0x7d19eebd, 0x7d0f4217, 0x7d048226, 0x7cf9aeef, 0x7ceec872,
    0x7ce3ceb0, 0x7cd8c1ac, 0x7ccda167, 0x7cc26de4, 0x7cb72723, 0x7cabcd26, 0x7ca05ff0, 0x7c94df81,
    0x7c894bdc, 0x7c7da503, 0x7c71eaf7, 0x7c661dba, 0x7c5a3d4e, 0x7c4e49b5, 0x7c4242f1, 0x7c362903,
    0x7c29fbed, 0x7c1dbbb1, 0x7c116852, 0x7c0501d0, 0x7bf8882f, 0x7bebfb6f, 0x7bdf5b93, 0x7bd2a89d,
    0x7bc5e28e, 0x7bb90969, 0x7bac1d30, 0x7b9f1de4, 0x7b920b88, 0x7b84e61d, 0x7b77ada7, 0x7b6a6226,
    0x7b5d039c, 0x7b4f920d, 0x7b420d79, 0x7b3475e3, 0x7b26cb4e, 0x7b190dba, 0x7b0b3d2b, 0x7afd59a2,
    0x7aef6322, 0x7ae159ad, 0x7ad33d44, 0x7ac50dea, 0x7ab6cba2, 0x7aa8766d, 0x7a9a0e4e, 0x7a8b9347,
    0x7a7d055a, 0x7a6e6489, 0x7a5fb0d7, 0x7a50ea45, 0x7a4210d7, 0x7a33248e, 0x7a24256d, 0x7a151376,
    0x7a05eeac, 0x79f6b710, 0x79e76ca5, 0x79d80f6e, 0x79c89f6c, 0x79b91ca3, 0x79a98714, 0x7999dec2,
    0x798a23b0, 0x797a55df, 0x796a7553, 0x795a820d, 0x794a7c10, 0x793a635f, 0x792a37fd, 0x7919f9ea,
    0x7909a92b, 0x78f945c2, 0x78e8cfb0, 0x78d846fa, 0x78c7aba0, 0x78b6fda7, 0x78a63d0f, 0x789569dd,
    0x78848412, 0x78738bb2, 0x786280be, 0x7851633a, 0x78403327, 0x782ef08a, 0x781d9b63, 0x780c33b7,
    0x77fab987, 0x77e92cd7, 0x77d78da9, 0x77c5dc00, 0x77b417de, 0x77a24147, 0x7790583c, 0x777e5cc2,
    0x776c4eda, 0x775a2e87, 0x7747fbcd, 0x7735b6ad, 0x77235f2c, 0x7710f54b, 0x76fe790d, 0x76ebea76,
    0x76d94987, 0x76c69645, 0x76b3d0b2, 0x76a0f8d1, 0x768e0ea4, 0x767b122f, 0x76680375, 0x7654e278,
    0x7641af3b, 0x762e69c2, 0x761b1210, 0x7607a826, 0x75f42c09, 0x75e09dbc, 0x75ccfd41, 0x75b94a9b,
    0x75a585ce, 0x7591aedc, 0x757dc5c9, 0x7569ca97, 0x7555bd4a, 0x75419de5, 0x752d6c6b, 0x751928df,
    0x7504d344, 0x74f06b9d, 0x74dbf1ee, 0x74c76639, 0x74b2c882, 0x749e18cc, 0x7489571a, 0x74748370,
    0x745f9dd0, 0x744aa63d, 0x74359cbc, 0x7420814f, 0x740b53f9, 0x73f614bf, 0x73e0c3a2, 0x73cb60a6,
    0x73b5ebd0, 0x73a06521, 0x738acc9d, 0x73752248, 0x735f6625, 0x73499837, 0x7333b882, 0x731dc708,
    0x7307c3cf, 0x72f1aed7, 0x72db8827, 0x72c54fbf, 0x72af05a5, 0x7298a9db, 0x72823c66, 0x726bbd47,
    0x72552c83, 0x723e8a1e, 0x7227d61b, 0x7211107c, 0x71fa3947, 0x71e3507e, 0x71cc5625, 0x71b54a3f,
    0x719e2cd1, 0x7186fddd, 0x716fbd67, 0x71586b72, 0x71410803, 0x7129931d, 0x71120cc4, 0x70fa74fa,
    0x70e2cbc5, 0x70cb1126, 0x70b34523, 0x709b67bf, 0x708378fd, 0x706b78e2, 0x70536770, 0x703b44ab,
    0x70231098, 0x700acb3a, 0x6ff27495, 0x6fda0cac, 0x6fc19384, 0x6fa90920, 0x6f906d83, 0x6f77c0b2,
    0x6f5f02b0, 0x6f463382, 0x6f2d532b, 0x6f1461af, 0x6efb5f11, 0x6ee24b56, 0x6ec92681, 0x6eaff097,
    0x6e96a99b, 0x6e7d5192, 0x6e63e87e, 0x6e4a6e64, 0x6e30e348, 0x6e17472e, 0x6dfd9a1a, 0x6de3dc10,
    0x6dca0d13, 0x6db02d28, 0x6d963c53, 0x6d7c3a97, 0x6d6227f9, 0x6d48047d, 0x6d2dd026, 0x6d138af9,
    0x6cf934fa, 0x6cdece2d, 0x6cc45696, 0x6ca9ce39, 0x6c8f351b, 0x6c748b3e, 0x6c59d0a8, 0x6c3f055c,
    0x6c24295f, 0x6c093cb5, 0x6bee3f61, 0x6bd33169, 0x6bb812d0, 0x6b9ce39a, 0x6b81a3cc, 0x6b665369,
    0x6b4af277, 0x6b2f80f9, 0x6b13fef4, 0x6af86c6b, 0x6adcc963, 0x6ac115e0, 0x6aa551e7, 0x6a897d7c,
    0x6a6d98a3, 0x6a51a360, 0x6a359db8, 0x6a1987af, 0x69fd6149, 0x69e12a8b, 0x69c4e379, 0x69a88c18,
    0x698c246b, 0x696fac77, 0x69532441, 0x69368bcd, 0x6919e31f, 0x68fd2a3c, 0x68e06128, 0x68c387e8,
    0x68a69e80, 0x6889a4f4, 0x686c9b4a, 0x684f8185, 0x683257aa, 0x68151dbd, 0x67f7d3c3, 0x67da79c1,
    0x67bd0fbb, 0x679f95b6, 0x67820bb6, 0x676471bf, 0x6746c7d6, 0x67290e01, 0x670b4442, 0x66ed6aa0,
    0x66cf811f, 0x66b187c2, 0x66937e8f, 0x6675658b, 0x66573cba, 0x66390421, 0x661abbc4, 0x65fc63a8,
    0x65ddfbd2, 0x65bf8446, 0x65a0fd0a, 0x65826621, 0x6563bf91, 0x6545095e, 0x6526438d, 0x65076e23,
    0x64e88925, 0x64c99497, 0x64aa907e, 0x648b7cdf, 0x646c59be, 0x644d2721, 0x642de50c, 0x640e9384,
    0x63ef328e, 0x63cfc22f, 0x63b0426c, 0x6390b349, 0x637114cb, 0x635166f8, 0x6331a9d3, 0x6311dd63,
    0x62f201ab, 0x62d216b1, 0x62b21c7a, 0x6292130b, 0x6271fa68, 0x6251d297, 0x62319b9c, 0x6211557d,
    0x61f1003e, 0x61d09be4, 0x61b02875, 0x618fa5f6, 0x616f146a, 0x614e73d9, 0x612dc445, 0x610d05b6,
    0x60ec382f, 0x60cb5bb5, 0x60aa704f, 0x60897600, 0x60686cce, 0x604754bd, 0x60262dd4, 0x6004f818,
    0x5fe3b38c, 0x5fc26037, 0x5fa0fe1e, 0x5f7f8d45, 0x5f5e0db2, 0x5f3c7f6a, 0x5f1ae272, 0x5ef936d0,
    0x5ed77c88, 0x5eb5b3a1, 0x5e93dc1e, 0x5e71f605, 0x5e50015c, 0x5e2dfe28, 0x5e0bec6d, 0x5de9cc32,
    0x5dc79d7b, 0x5da5604e, 0x5d8314b0, 0x5d60baa6, 0x5d3e5235, 0x5d1bdb64, 0x5cf95637, 0x5cd6c2b3,
    0x5cb420df, 0x5c9170be, 0x5c6eb257, 0x5c4be5af, 0x5c290acb, 0x5c0621b1, 0x5be32a66, 0x5bc024ef,
    0x5b9d1152, 0x5b79ef95, 0x5b56bfbc, 0x5b3381cd, 0x5b1035ce, 0x5aecdbc3, 0x5ac973b4, 0x5aa5fda3,
    0x5a827999, 0x5a5ee799, 0x5a3b47a9, 0x5a1799d0, 0x59f3de11, 0x59d01473, 0x59ac3cfc, 0x598857b1,
    0x59646497, 0x594063b3, 0x591c550d, 0x58f838a8, 0x58d40e8b, 0x58afd6bb, 0x588b913e, 0x58673e1a,
    0x5842dd53, 0x581e6ef0, 0x57f9f2f6, 0x57d5696b, 0x57b0d255, 0x578c2db8, 0x57677b9c, 0x5742bc05,
    0x571deef8, 0x56f9147d, 0x56d42c98, 0x56af374f, 0x568a34a8, 0x566524a9, 0x56400757, 0x561adcb8,
    0x55f5a4d1, 0x55d05fa9, 0x55ab0d45, 0x5585adab, 0x556040e1, 0x553ac6ed, 0x55153fd3, 0x54efab9b,
    0x54ca0a49, 0x54a45be5, 0x547ea072, 0x5458d7f8, 0x5433027c, 0x540d2004, 0x53e73096, 0x53c13437,
    0x539b2aee, 0x537514c1, 0x534ef1b4, 0x5328c1cf, 0x53028517, 0x52dc3b91, 0x52b5e544, 0x528f8237,
    0x5269126d, 0x524295ef, 0x521c0cc0, 0x51f576e9, 0x51ced46d, 0x51a82554, 0x518169a4, 0x515aa161,
    0x5133cc93, 0x510ceb3f, 0x50e5fd6c, 0x50bf031e, 0x5097fc5d, 0x5070e92e, 0x5049c998, 0x50229da0,
    0x4ffb654c, 0x4fd420a3, 0x4faccfaa, 0x4f857268, 0x4f5e08e2, 0x4f36931f, 0x4f0f1125, 0x4ee782fa,
    0x4ebfe8a3, 0x4e984228, 0x4e708f8e, 0x4e48d0dc, 0x4e210616, 0x4df92f45, 0x4dd14c6d, 0x4da95d95,
    0x4d8162c3, 0x4d595bfd, 0x4d31494a, 0x4d092aaf, 0x4ce10033, 0x4cb8c9dc, 0x4c9087b0, 0x4c6839b5,
    0x4c3fdff2, 0x4c177a6d, 0x4bef092c, 0x4bc68c35, 0x4b9e038f, 0x4b756f3f, 0x4b4ccf4c, 0x4b2423bd,
    0x4afb6c97, 0x4ad2a9e1, 0x4aa9dba1, 0x4a8101dd, 0x4a581c9c, 0x4a2f2be5, 0x4a062fbc, 0x49dd2829,
    0x49b41532, 0x498af6de, 0x4961cd32, 0x49389835, 0x490f57ed, 0x48e60c61, 0x48bcb598, 0x48935396,
    0x4869e664, 0x48406e06, 0x4816ea85, 0x47ed5be5, 0x47c3c22e, 0x479a1d66, 0x47706d92, 0x4746b2bb,
    0x471cece6, 0x46f31c19, 0x46c9405b, 0x469f59b3, 0x46756827, 0x464b6bbd, 0x4621647c, 0x45f7526a,
    0x45cd358e, 0x45a30def, 0x4578db92, 0x454e9e7f, 0x452456bc, 0x44fa044f, 0x44cfa73f, 0x44a53f92,
    0x447acd4f, 0x4450507d, 0x4425c922, 0x43fb3745, 0x43d09aec, 0x43a5f41d, 0x437b42e0, 0x4350873b,
    0x4325c134, 0x42faf0d3, 0x42d0161d, 0x42a5311a, 0x427a41cf, 0x424f4844, 0x42244480, 0x41f93688,
    0x41ce1e64, 0x41a2fc19, 0x4177cfb0, 0x414c992e, 0x4121589a, 0x40f60dfa, 0x40cab957, 0x409f5ab5,
    0x4073f21c, 0x40487f93, 0x401d0320, 0x3ff17cc9, 0x3fc5ec97, 0x3f9a528f, 0x3f6eaeb7, 0x3f430118,
    0x3f1749b7, 0x3eeb889b, 0x3ebfbdcc, 0x3e93e94f, 0x3e680b2c, 0x3e3c2368, 0x3e10320d, 0x3de4371e,
    0x3db832a5, 0x3d8c24a7, 0x3d600d2b, 0x3d33ec38, 0x3d07c1d5, 0x3cdb8e09, 0x3caf50da, 0x3c830a4f,
    0x3c56ba6f, 0x3c2a6141, 0x3bfdfecc, 0x3bd19317, 0x3ba51e28, 0x3b78a006, 0x3b4c18b9, 0x3b1f8847,
    0x3af2eeb6, 0x3ac64c0e, 0x3a99a056, 0x3a6ceb95, 0x3a402dd1, 0x3a136711, 0x39e6975d, 0x39b9bebb,
    0x398cdd31, 0x395ff2c8, 0x3932ff86, 0x39060372, 0x38d8fe92, 0x38abf0ee, 0x387eda8d, 0x3851bb76,
    0x382493af, 0x37f76340, 0x37ca2a2f, 0x379ce884, 0x376f9e45, 0x37424b7a, 0x3714f029, 0x36e78c5a,
    0x36ba2013, 0x368cab5b, 0x365f2e3a, 0x3631a8b7, 0x36041ad8, 0x35d684a5, 0x35a8e624, 0x357b3f5d,
    0x354d9056, 0x351fd917, 0x34f219a7, 0x34c4520c, 0x3496824f, 0x3468aa75, 0x343aca87, 0x340ce28a,
    0x33def286, 0x33b0fa83, 0x3382fa87, 0x3354f29a, 0x3326e2c2, 0x32f8cb06, 0x32caab6e, 0x329c8402,
    0x326e54c7, 0x32401dc5, 0x3211df03, 0x31e39889, 0x31b54a5d, 0x3186f486, 0x3158970d, 0x312a31f7,
    0x30fbc54c, 0x30cd5114, 0x309ed555, 0x30705217, 0x3041c760, 0x30133538, 0x2fe49ba6, 0x2fb5fab1,
    0x2f875261, 0x2f58a2bd, 0x2f29ebcb, 0x2efb2d94, 0x2ecc681d, 0x2e9d9b70, 0x2e6ec791, 0x2e3fec8a,
    0x2e110a61, 0x2de2211d, 0x2db330c6, 0x2d843963, 0x2d553afb, 0x2d263595, 0x2cf72939, 0x2cc815ed,
    0x2c98fbba, 0x2c69daa6, 0x2c3ab2b8, 0x2c0b83f9, 0x2bdc4e6e, 0x2bad1221, 0x2b7dcf16, 0x2b4e8557,
    0x2b1f34eb, 0x2aefddd7, 0x2ac08025, 0x2a911bdb, 0x2a61b101, 0x2a323f9d, 0x2a02c7b7, 0x29d34957,
    0x29a3c484, 0x29743945, 0x2944a7a1, 0x29150fa1, 0x28e5714a, 0x28b5cca5, 0x288621b8, 0x2856708c,
    0x2826b927, 0x27f6fb92, 0x27c737d2, 0x27976df0, 0x27679df3, 0x2737c7e3, 0x2707ebc6, 0x26d809a4,
    0x26a82185, 0x26783370, 0x26483f6c, 0x26184580, 0x25e845b5, 0x25b84011, 0x2588349c, 0x2558235e,
    0x25280c5d, 0x24f7efa1, 0x24c7cd32, 0x2497a517, 0x24677757, 0x243743fa, 0x24070b07, 0x23d6cc86,
    0x23a6887e, 0x23763ef6, 0x2345eff7, 0x23159b87, 0x22e541ae, 0x22b4e274, 0x22847ddf, 0x225413f7,
    0x2223a4c5, 0x21f3304e, 0x21c2b69b, 0x219237b4, 0x2161b39f, 0x21312a64, 0x21009c0b, 0x20d0089b,
    0x209f701c, 0x206ed294, 0x203e300d, 0x200d888c, 0x1fdcdc1a, 0x1fac2abe, 0x1f7b7480, 0x1f4ab967,
    0x1f19f97a, 0x1ee934c2, 0x1eb86b45, 0x1e879d0c, 0x1e56ca1d, 0x1e25f281, 0x1df5163e, 0x1dc4355d,
    0x1d934fe5, 0x1d6265dd, 0x1d31774c, 0x1d00843c, 0x1ccf8cb2, 0x1c9e90b7, 0x1c6d9053, 0x1c3c8b8c,
    0x1c0b826a, 0x1bda74f5, 0x1ba96334, 0x1b784d30, 0x1b4732ef, 0x1b161479, 0x1ae4f1d5, 0x1ab3cb0c,
    0x1a82a025, 0x1a517127, 0x1a203e1a, 0x19ef0706, 0x19bdcbf2, 0x198c8ce6, 0x195b49e9, 0x192a0303,
    0x18f8b83c, 0x18c7699b, 0x18961727, 0x1864c0e9, 0x183366e8, 0x1802092b, 0x17d0a7bb, 0x179f429f,
    0x176dd9de, 0x173c6d80, 0x170afd8c, 0x16d98a0b, 0x16a81304, 0x1676987f, 0x16451a82, 0x16139917,
    0x15e21444, 0x15b08c11, 0x157f0086, 0x154d71aa, 0x151bdf85, 0x14ea4a1f, 0x14b8b17f, 0x148715ad,
    0x145576b1, 0x1423d491, 0x13f22f57, 0x13c0870a, 0x138edbb0, 0x135d2d53, 0x132b7bf9, 0x12f9c7aa,
    0x12c8106e, 0x1296564d, 0x1264994d, 0x1232d978, 0x120116d4, 0x11cf516a, 0x119d8940, 0x116bbe5f,
    0x1139f0ce, 0x11082095, 0x10d64dbc, 0x10a4784a, 0x1072a047, 0x1040c5bb, 0x100ee8ad, 0x0fdd0925,
    0x0fab272b, 0x0f7942c6, 0x0f475bfe, 0x0f1572dc, 0x0ee38765, 0x0eb199a3, 0x0e7fa99d, 0x0e4db75a,
    0x0e1bc2e3, 0x0de9cc3f, 0x0db7d376, 0x0d85d88e, 0x0d53db92, 0x0d21dc86, 0x0cefdb75, 0x0cbdd865,
    0x0c8bd35d, 0x0c59cc67, 0x0c27c389, 0x0bf5b8cb, 0x0bc3ac35, 0x0b919dce, 0x0b5f8d9f, 0x0b2d7bae,
    0x0afb6805, 0x0ac952aa, 0x0a973ba5, 0x0a6522fd, 0x0a3308bc, 0x0a00ece8, 0x09cecf89, 0x099cb0a7,
    0x096a9049, 0x09386e77, 0x09064b3a, 0x08d42698, 0x08a2009a, 0x086fd947, 0x083db0a7, 0x080b86c1,
    0x07d95b9e, 0x07a72f45, 0x077501be, 0x0742d310, 0x0710a344, 0x06de7261, 0x06ac406f, 0x067a0d75,
    0x0647d97c, 0x0615a48a, 0x05e36ea9, 0x05b137df, 0x057f0034, 0x054cc7b0, 0x051a8e5b, 0x04e8543d,
    0x04b6195d, 0x0483ddc3, 0x0451a176, 0x041f647f, 0x03ed26e6, 0x03bae8b1, 0x0388a9e9, 0x03566a96,
    0x03242abe, 0x02f1ea6b, 0x02bfa9a4, 0x028d6870, 0x025b26d7, 0x0228e4e1, 0x01f6a296, 0x01c45ffe,
    0x01921d1f, 0x015fda03, 0x012d96b0, 0x00fb532f, 0x00c90f87, 0x0096cbc1, 0x006487e2, 0x003243f5,
    0x00000000, 0xffcdbc0b, 0xff9b781e, 0xff69343f, 0xff36f079, 0xff04acd1, 0xfed26950, 0xfea025fd,
    0xfe6de2e1, 0xfe3ba002, 0xfe095d6a, 0xfdd71b1f, 0xfda4d929, 0xfd729790, 0xfd40565c, 0xfd0e1595,
    0xfcdbd542, 0xfca9956a, 0xfc775617, 0xfc45174f, 0xfc12d91a, 0xfbe09b81, 0xfbae5e8a, 0xfb7c223d,
    0xfb49e6a3, 0xfb17abc3, 0xfae571a5, 0xfab33850, 0xfa80ffcc, 0xfa4ec821, 0xfa1c9157, 0xf9ea5b76,
    0xf9b82684, 0xf985f28b, 0xf953bf91, 0xf9218d9f, 0xf8ef5cbc, 0xf8bd2cf0, 0xf88afe42, 0xf858d0bb,
    0xf826a462, 0xf7f4793f, 0xf7c24f59, 0xf79026b9, 0xf75dff66, 0xf72bd968, 0xf6f9b4c6, 0xf6c79189,
    0xf6956fb7, 0xf6634f59, 0xf6313077, 0xf5ff1318, 0xf5ccf744, 0xf59add03, 0xf568c45b, 0xf536ad56,
    0xf50497fb, 0xf4d28452, 0xf4a07261, 0xf46e6232, 0xf43c53cb, 0xf40a4735, 0xf3d83c77, 0xf3a63399,
    0xf3742ca3, 0xf342279b, 0xf310248b, 0xf2de237a, 0xf2ac246e, 0xf27a2772, 0xf2482c8a, 0xf21633c1,
    0xf1e43d1d, 0xf1b248a6, 0xf1805663, 0xf14e665d, 0xf11c789b, 0xf0ea8d24, 0xf0b8a402, 0xf086bd3a,
    0xf054d8d5, 0xf022f6db, 0xeff11753, 0xefbf3a45, 0xef8d5fb9, 0xef5b87b6, 0xef29b244, 0xeef7df6b,
    0xeec60f32, 0xee9441a1, 0xee6276c0, 0xee30ae96, 0xedfee92c, 0xedcd2688, 0xed9b66b3, 0xed69a9b3,
    0xed37ef92, 0xed063856, 0xecd48407, 0xeca2d2ad, 0xec712450, 0xec3f78f6, 0xec0dd0a9, 0xebdc2b6f,
    0xebaa894f, 0xeb78ea53, 0xeb474e81, 0xeb15b5e1, 0xeae4207b, 0xeab28e56, 0xea80ff7a, 0xea4f73ef,
    0xea1debbc, 0xe9ec66e9, 0xe9bae57e, 0xe9896781, 0xe957ecfc, 0xe92675f5, 0xe8f50274, 0xe8c39280,
    0xe8922622, 0xe860bd61, 0xe82f5845, 0xe7fdf6d5, 0xe7cc9918, 0xe79b3f17, 0xe769e8d9, 0xe7389665,
    0xe70747c4, 0xe6d5fcfd, 0xe6a4b617, 0xe673731a, 0xe642340e, 0xe610f8fa, 0xe5dfc1e6, 0xe5ae8ed9,
    0xe57d5fdb, 0xe54c34f4, 0xe51b0e2b, 0xe4e9eb87, 0xe4b8cd11, 0xe487b2d0, 0xe4569ccc, 0xe4258b0b,
    0xe3f47d96, 0xe3c37474, 0xe3926fad, 0xe3616f49, 0xe330734e, 0xe2ff7bc4, 0xe2ce88b4, 0xe29d9a23,
    0xe26cb01b, 0xe23bcaa3, 0xe20ae9c2, 0xe1da0d7f, 0xe1a935e3, 0xe17862f4, 0xe14794bb, 0xe116cb3e,
    0xe0e60686, 0xe0b54699, 0xe0848b80, 0xe053d542, 0xe02323e6, 0xdff27774, 0xdfc1cff3, 0xdf912d6c,
    0xdf608fe4, 0xdf2ff765, 0xdeff63f5, 0xdeced59c, 0xde9e4c61, 0xde6dc84c, 0xde3d4965, 0xde0ccfb2,
    0xdddc5b3b, 0xddabec09, 0xdd7b8221, 0xdd4b1d8c, 0xdd1abe52, 0xdcea6479, 0xdcba1009, 0xdc89c10a,
    0xdc597782, 0xdc29337a, 0xdbf8f4f9, 0xdbc8bc06, 0xdb9888a9, 0xdb685ae9, 0xdb3832ce, 0xdb08105f,
    0xdad7f3a3, 0xdaa7dca2, 0xda77cb64, 0xda47bfef, 0xda17ba4b, 0xd9e7ba80, 0xd9b7c094, 0xd987cc90,
    0xd957de7b, 0xd927f65c, 0xd8f8143a, 0xd8c8381d, 0xd898620d, 0xd8689210, 0xd838c82e, 0xd809046e,
    0xd7d946d9, 0xd7a98f74, 0xd779de48, 0xd74a335b, 0xd71a8eb6, 0xd6eaf05f, 0xd6bb585f, 0xd68bc6bb,
    0xd65c3b7c, 0xd62cb6a9, 0xd5fd3849, 0xd5cdc063, 0xd59e4eff, 0xd56ee425, 0xd53f7fdb, 0xd5102229,
    0xd4e0cb15, 0xd4b17aa9, 0xd48230ea, 0xd452eddf, 0xd423b192, 0xd3f47c07, 0xd3c54d48, 0xd396255a,
    0xd3670446, 0xd337ea13, 0xd308d6c7, 0xd2d9ca6b, 0xd2aac505, 0xd27bc69d, 0xd24ccf3a, 0xd21ddee3,
    0xd1eef59f, 0xd1c01376, 0xd191386f, 0xd1626490, 0xd13397e3, 0xd104d26c, 0xd0d61435, 0xd0a75d43,
    0xd078ad9f, 0xd04a054f, 0xd01b645a, 0xcfeccac8, 0xcfbe38a0, 0xcf8fade9, 0xcf612aab, 0xcf32aeec,
    0xcf043ab4, 0xced5ce09, 0xcea768f3, 0xce790b7a, 0xce4ab5a3, 0xce1c6777, 0xcdee20fd, 0xcdbfe23b,
    0xcd91ab39, 0xcd637bfe, 0xcd355492, 0xcd0734fa, 0xccd91d3e, 0xccab0d66, 0xcc7d0579, 0xcc4f057d,
    0xcc210d7a, 0xcbf31d76, 0xcbc53579, 0xcb97558b, 0xcb697db1, 0xcb3badf4, 0xcb0de659, 0xcae026e9,
    0xcab26faa, 0xca84c0a3, 0xca5719dc, 0xca297b5b, 0xc9fbe528, 0xc9ce5749, 0xc9a0d1c6, 0xc97354a5,
    0xc945dfed, 0xc91873a6, 0xc8eb0fd7, 0xc8bdb486, 0xc89061bb, 0xc863177c, 0xc835d5d1, 0xc8089cc0,
    0xc7db6c51, 0xc7ae448a, 0xc7812573, 0xc7540f12, 0xc727016e, 0xc6f9fc8e, 0xc6cd007a, 0xc6a00d38,
    0xc67322cf, 0xc6464145, 0xc61968a3, 0xc5ec98ef, 0xc5bfd22f, 0xc593146b, 0xc5665faa, 0xc539b3f2,
    0xc50d114a, 0xc4e077b9, 0xc4b3e747, 0xc4875ffa, 0xc45ae1d8, 0xc42e6ce9, 0xc4020134, 0xc3d59ebf,
    0xc3a94591, 0xc37cf5b1, 0xc350af26, 0xc32471f7, 0xc2f83e2b, 0xc2cc13c8, 0xc29ff2d5, 0xc273db59,
    0xc247cd5b, 0xc21bc8e2, 0xc1efcdf3, 0xc1c3dc98, 0xc197f4d4, 0xc16c16b1, 0xc1404234, 0xc1147765,
    0xc0e8b649, 0xc0bcfee8, 0xc0915149, 0xc065ad71, 0xc03a1369, 0xc00e8337, 0xbfe2fce0, 0xbfb7806d,
    0xbf8c0de4, 0xbf60a54b, 0xbf3546a9, 0xbf09f206, 0xbedea766, 0xbeb366d2, 0xbe883050, 0xbe5d03e7,
    0xbe31e19c, 0xbe06c978, 0xbddbbb80, 0xbdb0b7bc, 0xbd85be31, 0xbd5acee6, 0xbd2fe9e3, 0xbd050f2d,
    0xbcda3ecc, 0xbcaf78c5, 0xbc84bd20, 0xbc5a0be3, 0xbc2f6514, 0xbc04c8bb, 0xbbda36de, 0xbbafaf83,
    0xbb8532b1, 0xbb5ac06e, 0xbb3058c1, 0xbb05fbb1, 0xbadba944, 0xbab16181, 0xba87246e, 0xba5cf211,
    0xba32ca72, 0xba08ad96, 0xb9de9b84, 0xb9b49443, 0xb98a97d9, 0xb960a64d, 0xb936bfa5, 0xb90ce3e7,
    0xb8e3131a, 0xb8b94d45, 0xb88f926e, 0xb865e29a, 0xb83c3dd2, 0xb812a41b, 0xb7e9157b, 0xb7bf91fa,
    0xb796199c, 0xb76cac6a, 0xb7434a68, 0xb719f39f, 0xb6f0a813, 0xb6c767cb, 0xb69e32ce, 0xb6750922,
    0xb64beace, 0xb622d7d7, 0xb5f9d044, 0xb5d0d41b, 0xb5a7e364, 0xb57efe23, 0xb556245f, 0xb52d561f,
    0xb5049369, 0xb4dbdc43, 0xb4b330b4, 0xb48a90c1, 0xb461fc71, 0xb43973cb, 0xb410f6d4, 0xb3e88593,
    0xb3c0200e, 0xb397c64b, 0xb36f7850, 0xb3473624, 0xb31effcd, 0xb2f6d551, 0xb2ceb6b6, 0xb2a6a403,
    0xb27e9d3d, 0xb256a26b, 0xb22eb393, 0xb206d0bb, 0xb1def9ea, 0xb1b72f24, 0xb18f7072, 0xb167bdd8,
    0xb140175d, 0xb1187d06, 0xb0f0eedb, 0xb0c96ce1, 0xb0a1f71e, 0xb07a8d98, 0xb0533056, 0xb02bdf5d,
    0xb0049ab4, 0xafdd6260, 0xafb63668, 0xaf8f16d2, 0xaf6803a3, 0xaf40fce2, 0xaf1a0294, 0xaef314c1,
    0xaecc336d, 0xaea55e9f, 0xae7e965c, 0xae57daac, 0xae312b93, 0xae0a8917, 0xade3f340, 0xadbd6a11,
    0xad96ed93, 0xad707dc9, 0xad4a1abc, 0xad23c46f, 0xacfd7ae9, 0xacd73e31, 0xacb10e4c, 0xac8aeb3f,
    0xac64d512, 0xac3ecbc9, 0xac18cf6a, 0xabf2dffc, 0xabccfd84, 0xaba72808, 0xab815f8e, 0xab5ba41b,
    0xab35f5b7, 0xab105465, 0xaaeac02d, 0xaac53913, 0xaa9fbf1f, 0xaa7a5255, 0xaa54f2bb, 0xaa2fa057,
    0xaa0a5b2f, 0xa9e52348, 0xa9bff8a9, 0xa99adb57, 0xa975cb58, 0xa950c8b1, 0xa92bd368, 0xa906eb83,
    0xa8e21108, 0xa8bd43fb, 0xa8988464, 0xa873d248, 0xa84f2dab, 0xa82a9695, 0xa8060d0a, 0xa7e19110,
    0xa7bd22ad, 0xa798c1e6, 0xa7746ec2, 0xa7502945, 0xa72bf175, 0xa707c758, 0xa6e3aaf3, 0xa6bf9c4d,
    0xa69b9b69, 0xa677a84f, 0xa653c304, 0xa62feb8d, 0xa60c21ef, 0xa5e86630, 0xa5c4b857, 0xa5a11867,
    0xa57d8667, 0xa55a025d, 0xa5368c4c, 0xa513243d, 0xa4efca32, 0xa4cc7e33, 0xa4a94044, 0xa486106b,
    0xa462eeae, 0xa43fdb11, 0xa41cd59a, 0xa3f9de4f, 0xa3d6f535, 0xa3b41a51, 0xa3914da9, 0xa36e8f42,
    0xa34bdf21, 0xa3293d4d, 0xa306a9c9, 0xa2e4249c, 0xa2c1adcb, 0xa29f455a, 0xa27ceb50, 0xa25a9fb2,
    0xa2386285, 0xa21633ce, 0xa1f41393, 0xa1d201d8, 0xa1affea4, 0xa18e09fb, 0xa16c23e2, 0xa14a4c5f,
    0xa1288378, 0xa106c930, 0xa0e51d8e, 0xa0c38096, 0xa0a1f24e, 0xa08072bb, 0xa05f01e2, 0xa03d9fc9,
    0xa01c4c74, 0x9ffb07e8, 0x9fd9d22c, 0x9fb8ab43, 0x9f979332, 0x9f768a00, 0x9f558fb1, 0x9f34a44b,
    0x9f13c7d1, 0x9ef2fa4a, 0x9ed23bbb, 0x9eb18c27, 0x9e90eb96, 0x9e705a0a, 0x9e4fd78b, 0x9e2f641c,
    0x9e0effc2, 0x9deeaa83, 0x9dce6464, 0x9dae2d69, 0x9d8e0598, 0x9d6decf5, 0x9d4de386, 0x9d2de94f,
    0x9d0dfe55, 0x9cee229d, 0x9cce562d, 0x9cae9908, 0x9c8eeb35, 0x9c6f4cb7, 0x9c4fbd94, 0x9c303dd1,
    0x9c10cd72, 0x9bf16c7c, 0x9bd21af4, 0x9bb2d8df, 0x9b93a642, 0x9b748321, 0x9b556f82, 0x9b366b69,
    0x9b1776db, 0x9af891dd, 0x9ad9bc73, 0x9abaf6a2, 0x9a9c406f, 0x9a7d99df, 0x9a5f02f6, 0x9a407bba,
    0x9a22042e, 0x9a039c58, 0x99e5443c, 0x99c6fbdf, 0x99a8c346, 0x998a9a75, 0x996c8171, 0x994e783e,
    0x99307ee1, 0x99129560, 0x98f4bbbe, 0x98d6f1ff, 0x98b9382a, 0x989b8e41, 0x987df44a, 0x98606a4a,
    0x9842f045, 0x9825863f, 0x98082c3d, 0x97eae243, 0x97cda856, 0x97b07e7b, 0x979364b6, 0x97765b0c,
    0x97596180, 0x973c7818, 0x971f9ed8, 0x9702d5c4, 0x96e61ce1, 0x96c97433, 0x96acdbbf, 0x96905389,
    0x9673db95, 0x965773e8, 0x963b1c87, 0x961ed575, 0x96029eb7, 0x95e67851, 0x95ca6248, 0x95ae5ca0,
    0x9592675d, 0x95768284, 0x955aae19, 0x953eea20, 0x9523369d, 0x95079395, 0x94ec010c, 0x94d07f07,
    0x94b50d89, 0x9499ac97, 0x947e5c34, 0x94631c66, 0x9447ed30, 0x942cce97, 0x9411c09f, 0x93f6c34b,
    0x93dbd6a1, 0x93c0faa4, 0x93a62f58, 0x938b74c2, 0x9370cae5, 0x935631c7, 0x933ba96a, 0x932131d3,
    0x9306cb06, 0x92ec7507, 0x92d22fda, 0x92b7fb83, 0x929dd807, 0x9283c569, 0x9269c3ad, 0x924fd2d8,
    0x9235f2ed, 0x921c23f0, 0x920265e6, 0x91e8b8d2, 0x91cf1cb8, 0x91b5919c, 0x919c1782, 0x9182ae6e,
    0x91695665, 0x91500f69, 0x9136d97f, 0x911db4aa, 0x9104a0ef, 0x90eb9e51, 0x90d2acd5, 0x90b9cc7e,
    0x90a0fd50, 0x90883f4e, 0x906f927d, 0x9056f6e0, 0x903e6c7c, 0x9025f354, 0x900d8b6b, 0x8ff534c6,
    0x8fdcef68, 0x8fc4bb55, 0x8fac9890, 0x8f94871e, 0x8f7c8703, 0x8f649841, 0x8f4cbadd, 0x8f34eeda,
    0x8f1d343b, 0x8f058b06, 0x8eedf33c, 0x8ed66ce3, 0x8ebef7fd, 0x8ea7948e, 0x8e904299, 0x8e790223,
    0x8e61d32f, 0x8e4ab5c1, 0x8e33a9db, 0x8e1caf82, 0x8e05c6b9, 0x8deeef84, 0x8dd829e5, 0x8dc175e2,
    0x8daad37d, 0x8d9442b9, 0x8d7dc39a, 0x8d675625, 0x8d50fa5b, 0x8d3ab041, 0x8d2477d9, 0x8d0e5129,
    0x8cf83c31, 0x8ce238f8, 0x8ccc477e, 0x8cb667c9, 0x8ca099db, 0x8c8addb8, 0x8c753363, 0x8c5f9adf,
    0x8c4a1430, 0x8c349f5a, 0x8c1f3c5e, 0x8c09eb41, 0x8bf4ac07, 0x8bdf7eb1, 0x8bca6344, 0x8bb559c3,
    0x8ba06230, 0x8b8b7c90, 0x8b76a8e6, 0x8b61e734, 0x8b4d377e, 0x8b3899c7, 0x8b240e12, 0x8b0f9463,
    0x8afb2cbc, 0x8ae6d721, 0x8ad29395, 0x8abe621b, 0x8aaa42b6, 0x8a963569, 0x8a823a37, 0x8a6e5124,
    0x8a5a7a32, 0x8a46b565, 0x8a3302bf, 0x8a1f6244, 0x8a0bd3f7, 0x89f857da, 0x89e4edf0, 0x89d1963e,
    0x89be50c5, 0x89ab1d88, 0x8997fc8b, 0x8984edd1, 0x8971f15c, 0x895f072f, 0x894c2f4e, 0x893969bb,
    0x8926b679, 0x8914158a, 0x890186f3, 0x88ef0ab5, 0x88dca0d4, 0x88ca4953, 0x88b80433, 0x88a5d179,
    0x8893b126, 0x8881a33e, 0x886fa7c4, 0x885dbeb9, 0x884be822, 0x883a2400, 0x88287257, 0x8816d329,
    0x88054679, 0x87f3cc49, 0x87e2649d, 0x87d10f76, 0x87bfccd9, 0x87ae9cc6, 0x879d7f42, 0x878c744e,
    0x877b7bee, 0x876a9623, 0x8759c2f1, 0x87490259, 0x87385460, 0x8727b906, 0x87173050, 0x8706ba3e,
    0x86f656d5, 0x86e60616, 0x86d5c803, 0x86c59ca1, 0x86b583f0, 0x86a57df3, 0x86958aad, 0x8685aa21,
    0x8675dc50, 0x8666213e, 0x865678ec, 0x8646e35d, 0x86376094, 0x8627f092, 0x8618935b, 0x860948f0,
    0x85fa1154, 0x85eaec8a, 0x85dbda93, 0x85ccdb72, 0x85bdef29, 0x85af15bb, 0x85a04f29, 0x85919b77,
    0x8582faa6, 0x85746cb9, 0x8565f1b2, 0x85578993, 0x8549345e, 0x853af216, 0x852cc2bc, 0x851ea653,
    0x85109cde, 0x8502a65e, 0x84f4c2d5, 0x84e6f246, 0x84d934b2, 0x84cb8a1d, 0x84bdf287, 0x84b06df3,
    0x84a2fc64, 0x84959dda, 0x84885259, 0x847b19e3, 0x846df478, 0x8460e21c, 0x8453e2d0, 0x8446f697,
    0x843a1d72, 0x842d5763, 0x8420a46d, 0x84140491, 0x840777d1, 0x83fafe30, 0x83ee97ae, 0x83e2444f,
    0x83d60413, 0x83c9d6fd, 0x83bdbd0f, 0x83b1b64b, 0x83a5c2b2, 0x8399e246, 0x838e1509, 0x83825afd,
    0x8376b424, 0x836b207f, 0x835fa010, 0x835432da, 0x8348d8dd, 0x833d921c, 0x83325e99, 0x83273e54,
    0x831c3150, 0x8311378e, 0x83065111, 0x82fb7dda, 0x82f0bde9, 0x82e61143, 0x82db77e7, 0x82d0f1d7,
    0x82c67f15, 0x82bc1fa3, 0x82b1d383, 0x82a79ab5, 0x829d753c, 0x82936318, 0x8289644c, 0x827f78da,
    0x8275a0c2, 0x826bdc06, 0x82622aa7, 0x82588ca8, 0x824f020a, 0x82458acd, 0x823c26f4, 0x8232d680,
    0x82299973, 0x82206fcd, 0x82175991, 0x820e56bf, 0x8205675a, 0x81fc8b62, 0x81f3c2d8, 0x81eb0dbf,
    0x81e26c18, 0x81d9dde3, 0x81d16322, 0x81c8fbd7, 0x81c0a803, 0x81b867a6, 0x81b03ac3, 0x81a8215b,
    0x81a01b6e, 0x819828ff, 0x81904a0e, 0x81887e9c, 0x8180c6ab, 0x8179223c, 0x81719150, 0x816a13e8,
    0x8162aa05, 0x815b53a9, 0x815410d5, 0x814ce18a, 0x8145c5c8, 0x813ebd92, 0x8137c8e7, 0x8130e7ca,
    0x812a1a3b, 0x8123603b, 0x811cb9cc, 0x811626ee, 0x810fa7a2, 0x81093be9, 0x8102e3c5, 0x80fc9f37,
    0x80f66e3e, 0x80f050dd, 0x80ea4714, 0x80e450e4, 0x80de6e4e, 0x80d89f52, 0x80d2e3f3, 0x80cd3c31,
    0x80c7a80c, 0x80c22785, 0x80bcba9e, 0x80b76157, 0x80b21bb1, 0x80ace9ad, 0x80a7cb4b, 0x80a2c08c,
    0x809dc972, 0x8098e5fd, 0x8094162d, 0x808f5a04, 0x808ab182, 0x80861ca7, 0x80819b76, 0x807d2ded,
    0x8078d40f, 0x80748ddb, 0x80705b52, 0x806c3c75, 0x80683145, 0x806439c2, 0x806055ec, 0x805c85c5,
    0x8058c94d, 0x80552085, 0x80518b6d, 0x804e0a05, 0x804a9c4f, 0x8047424a, 0x8043fbf7, 0x8040c958,
    0x803daa6b, 0x803a9f33, 0x8037a7ae, 0x8034c3de, 0x8031f3c3, 0x802f375e, 0x802c8eaf, 0x8029f9b6,
    0x80277874, 0x80250ae9, 0x8022b115, 0x80206af9, 0x801e3896, 0x801c19eb, 0x801a0ef9, 0x801817c1,
    0x80163442, 0x8014647c, 0x8012a871, 0x80110020, 0x800f6b8a, 0x800deaae, 0x800c7d8e, 0x800b2429,
    0x8009de7f, 0x8008ac92, 0x80078e60, 0x800683ea, 0x80058d30, 0x8004aa33, 0x8003daf2, 0x80031f6e,
    0x800277a7, 0x8001e39d, 0x8001634f, 0x8000f6bf, 0x80009deb, 0x800058d5, 0x8000277c, 0x800009e0,  	
};

static const int Sin_PAR[] =
{
    
    0x00000000, 0x003243f5, 0x006487e2, 0x0096cbc1, 0x00c90f87, 0x00fb532f, 0x012d96b0, 0x015fda03,
    0x01921d1f, 0x01c45ffe, 0x01f6a296, 0x0228e4e1, 0x025b26d7, 0x028d6870, 0x02bfa9a4, 0x02f1ea6b,
    0x03242abe, 0x03566a96, 0x0388a9e9, 0x03bae8b1, 0x03ed26e6, 0x041f647f, 0x0451a176, 0x0483ddc3,
    0x04b6195d, 0x04e8543d, 0x051a8e5b, 0x054cc7b0, 0x057f0034, 0x05b137df, 0x05e36ea9, 0x0615a48a,
    0x0647d97c, 0x067a0d75, 0x06ac406f, 0x06de7261, 0x0710a344, 0x0742d310, 0x077501be, 0x07a72f45,
    0x07d95b9e, 0x080b86c1, 0x083db0a7, 0x086fd947, 0x08a2009a, 0x08d42698, 0x09064b3a, 0x09386e77,
    0x096a9049, 0x099cb0a7, 0x09cecf89, 0x0a00ece8, 0x0a3308bc, 0x0a6522fd, 0x0a973ba5, 0x0ac952aa,
    0x0afb6805, 0x0b2d7bae, 0x0b5f8d9f, 0x0b919dce, 0x0bc3ac35, 0x0bf5b8cb, 0x0c27c389, 0x0c59cc67,
    0x0c8bd35d, 0x0cbdd865, 0x0cefdb75, 0x0d21dc86, 0x0d53db92, 0x0d85d88e, 0x0db7d376, 0x0de9cc3f,
    0x0e1bc2e3, 0x0e4db75a, 0x0e7fa99d, 0x0eb199a3, 0x0ee38765, 0x0f1572dc, 0x0f475bfe, 0x0f7942c6,
    0x0fab272b, 0x0fdd0925, 0x100ee8ad, 0x1040c5bb, 0x1072a047, 0x10a4784a, 0x10d64dbc, 0x11082095,
    0x1139f0ce, 0x116bbe5f, 0x119d8940, 0x11cf516a, 0x120116d4, 0x1232d978, 0x1264994d, 0x1296564d,
    0x12c8106e, 0x12f9c7aa, 0x132b7bf9, 0x135d2d53, 0x138edbb0, 0x13c0870a, 0x13f22f57, 0x1423d491,
    0x145576b1, 0x148715ad, 0x14b8b17f, 0x14ea4a1f, 0x151bdf85, 0x154d71aa, 0x157f0086, 0x15b08c11,
    0x15e21444, 0x16139917, 0x16451a82, 0x1676987f, 0x16a81304, 0x16d98a0b, 0x170afd8c, 0x173c6d80,
    0x176dd9de, 0x179f429f, 0x17d0a7bb, 0x1802092b, 0x183366e8, 0x1864c0e9, 0x18961727, 0x18c7699b,
    0x18f8b83c, 0x192a0303, 0x195b49e9, 0x198c8ce6, 0x19bdcbf2, 0x19ef0706, 0x1a203e1a, 0x1a517127,
    0x1a82a025, 0x1ab3cb0c, 0x1ae4f1d5, 0x1b161479, 0x1b4732ef, 0x1b784d30, 0x1ba96334, 0x1bda74f5,
    0x1c0b826a, 0x1c3c8b8c, 0x1c6d9053, 0x1c9e90b7, 0x1ccf8cb2, 0x1d00843c, 0x1d31774c, 0x1d6265dd,
    0x1d934fe5, 0x1dc4355d, 0x1df5163e, 0x1e25f281, 0x1e56ca1d, 0x1e879d0c, 0x1eb86b45, 0x1ee934c2,
    0x1f19f97a, 0x1f4ab967, 0x1f7b7480, 0x1fac2abe, 0x1fdcdc1a, 0x200d888c, 0x203e300d, 0x206ed294,
    0x209f701c, 0x20d0089b, 0x21009c0b, 0x21312a64, 0x2161b39f, 0x219237b4, 0x21c2b69b, 0x21f3304e,
    0x2223a4c5, 0x225413f7, 0x22847ddf, 0x22b4e274, 0x22e541ae, 0x23159b87, 0x2345eff7, 0x23763ef6,
    0x23a6887e, 0x23d6cc86, 0x24070b07, 0x243743fa, 0x24677757, 0x2497a517, 0x24c7cd32, 0x24f7efa1,
    0x25280c5d, 0x2558235e, 0x2588349c, 0x25b84011, 0x25e845b5, 0x26184580, 0x26483f6c, 0x26783370,
    0x26a82185, 0x26d809a4, 0x2707ebc6, 0x2737c7e3, 0x27679df3, 0x27976df0, 0x27c737d2, 0x27f6fb92,
    0x2826b927, 0x2856708c, 0x288621b8, 0x28b5cca5, 0x28e5714a, 0x29150fa1, 0x2944a7a1, 0x29743945,
    0x29a3c484, 0x29d34957, 0x2a02c7b7, 0x2a323f9d, 0x2a61b101, 0x2a911bdb, 0x2ac08025, 0x2aefddd7,
    0x2b1f34eb, 0x2b4e8557, 0x2b7dcf16, 0x2bad1221, 0x2bdc4e6e, 0x2c0b83f9, 0x2c3ab2b8, 0x2c69daa6,
    0x2c98fbba, 0x2cc815ed, 0x2cf72939, 0x2d263595, 0x2d553afb, 0x2d843963, 0x2db330c6, 0x2de2211d,
    0x2e110a61, 0x2e3fec8a, 0x2e6ec791, 0x2e9d9b70, 0x2ecc681d, 0x2efb2d94, 0x2f29ebcb, 0x2f58a2bd,
    0x2f875261, 0x2fb5fab1, 0x2fe49ba6, 0x30133538, 0x3041c760, 0x30705217, 0x309ed555, 0x30cd5114,
    0x30fbc54c, 0x312a31f7, 0x3158970d, 0x3186f486, 0x31b54a5d, 0x31e39889, 0x3211df03, 0x32401dc5,
    0x326e54c7, 0x329c8402, 0x32caab6e, 0x32f8cb06, 0x3326e2c2, 0x3354f29a, 0x3382fa87, 0x33b0fa83,
    0x33def286, 0x340ce28a, 0x343aca87, 0x3468aa75, 0x3496824f, 0x34c4520c, 0x34f219a7, 0x351fd917,
    0x354d9056, 0x357b3f5d, 0x35a8e624, 0x35d684a5, 0x36041ad8, 0x3631a8b7, 0x365f2e3a, 0x368cab5b,
    0x36ba2013, 0x36e78c5a, 0x3714f029, 0x37424b7a, 0x376f9e45, 0x379ce884, 0x37ca2a2f, 0x37f76340,
    0x382493af, 0x3851bb76, 0x387eda8d, 0x38abf0ee, 0x38d8fe92, 0x39060372, 0x3932ff86, 0x395ff2c8,
    0x398cdd31, 0x39b9bebb, 0x39e6975d, 0x3a136711, 0x3a402dd1, 0x3a6ceb95, 0x3a99a056, 0x3ac64c0e,
    0x3af2eeb6, 0x3b1f8847, 0x3b4c18b9, 0x3b78a006, 0x3ba51e28, 0x3bd19317, 0x3bfdfecc, 0x3c2a6141,
    0x3c56ba6f, 0x3c830a4f, 0x3caf50da, 0x3cdb8e09, 0x3d07c1d5, 0x3d33ec38, 0x3d600d2b, 0x3d8c24a7,
    0x3db832a5, 0x3de4371e, 0x3e10320d, 0x3e3c2368, 0x3e680b2c, 0x3e93e94f, 0x3ebfbdcc, 0x3eeb889b,
    0x3f1749b7, 0x3f430118, 0x3f6eaeb7, 0x3f9a528f, 0x3fc5ec97, 0x3ff17cc9, 0x401d0320, 0x40487f93,
    0x4073f21c, 0x409f5ab5, 0x40cab957, 0x40f60dfa, 0x4121589a, 0x414c992e, 0x4177cfb0, 0x41a2fc19,
    0x41ce1e64, 0x41f93688, 0x42244480, 0x424f4844, 0x427a41cf, 0x42a5311a, 0x42d0161d, 0x42faf0d3,
    0x4325c134, 0x4350873b, 0x437b42e0, 0x43a5f41d, 0x43d09aec, 0x43fb3745, 0x4425c922, 0x4450507d,
    0x447acd4f, 0x44a53f92, 0x44cfa73f, 0x44fa044f, 0x452456bc, 0x454e9e7f, 0x4578db92, 0x45a30def,
    0x45cd358e, 0x45f7526a, 0x4621647c, 0x464b6bbd, 0x46756827, 0x469f59b3, 0x46c9405b, 0x46f31c19,
    0x471cece6, 0x4746b2bb, 0x47706d92, 0x479a1d66, 0x47c3c22e, 0x47ed5be5, 0x4816ea85, 0x48406e06,
    0x4869e664, 0x48935396, 0x48bcb598, 0x48e60c61, 0x490f57ed, 0x49389835, 0x4961cd32, 0x498af6de,
    0x49b41532, 0x49dd2829, 0x4a062fbc, 0x4a2f2be5, 0x4a581c9c, 0x4a8101dd, 0x4aa9dba1, 0x4ad2a9e1,
    0x4afb6c97, 0x4b2423bd, 0x4b4ccf4c, 0x4b756f3f, 0x4b9e038f, 0x4bc68c35, 0x4bef092c, 0x4c177a6d,
    0x4c3fdff2, 0x4c6839b5, 0x4c9087b0, 0x4cb8c9dc, 0x4ce10033, 0x4d092aaf, 0x4d31494a, 0x4d595bfd,
    0x4d8162c3, 0x4da95d95, 0x4dd14c6d, 0x4df92f45, 0x4e210616, 0x4e48d0dc, 0x4e708f8e, 0x4e984228,
    0x4ebfe8a3, 0x4ee782fa, 0x4f0f1125, 0x4f36931f, 0x4f5e08e2, 0x4f857268, 0x4faccfaa, 0x4fd420a3,
    0x4ffb654c, 0x50229da0, 0x5049c998, 0x5070e92e, 0x5097fc5d, 0x50bf031e, 0x50e5fd6c, 0x510ceb3f,
    0x5133cc93, 0x515aa161, 0x518169a4, 0x51a82554, 0x51ced46d, 0x51f576e9, 0x521c0cc0, 0x524295ef,
    0x5269126d, 0x528f8237, 0x52b5e544, 0x52dc3b91, 0x53028517, 0x5328c1cf, 0x534ef1b4, 0x537514c1,
    0x539b2aee, 0x53c13437, 0x53e73096, 0x540d2004, 0x5433027c, 0x5458d7f8, 0x547ea072, 0x54a45be5,
    0x54ca0a49, 0x54efab9b, 0x55153fd3, 0x553ac6ed, 0x556040e1, 0x5585adab, 0x55ab0d45, 0x55d05fa9,
    0x55f5a4d1, 0x561adcb8, 0x56400757, 0x566524a9, 0x568a34a8, 0x56af374f, 0x56d42c98, 0x56f9147d,
    0x571deef8, 0x5742bc05, 0x57677b9c, 0x578c2db8, 0x57b0d255, 0x57d5696b, 0x57f9f2f6, 0x581e6ef0,
    0x5842dd53, 0x58673e1a, 0x588b913e, 0x58afd6bb, 0x58d40e8b, 0x58f838a8, 0x591c550d, 0x594063b3,
    0x59646497, 0x598857b1, 0x59ac3cfc, 0x59d01473, 0x59f3de11, 0x5a1799d0, 0x5a3b47a9, 0x5a5ee799,
    0x5a827999, 0x5aa5fda3, 0x5ac973b4, 0x5aecdbc3, 0x5b1035ce, 0x5b3381cd, 0x5b56bfbc, 0x5b79ef95,
    0x5b9d1152, 0x5bc024ef, 0x5be32a66, 0x5c0621b1, 0x5c290acb, 0x5c4be5af, 0x5c6eb257, 0x5c9170be,
    0x5cb420df, 0x5cd6c2b3, 0x5cf95637, 0x5d1bdb64, 0x5d3e5235, 0x5d60baa6, 0x5d8314b0, 0x5da5604e,
    0x5dc79d7b, 0x5de9cc32, 0x5e0bec6d, 0x5e2dfe28, 0x5e50015c, 0x5e71f605, 0x5e93dc1e, 0x5eb5b3a1,
    0x5ed77c88, 0x5ef936d0, 0x5f1ae272, 0x5f3c7f6a, 0x5f5e0db2, 0x5f7f8d45, 0x5fa0fe1e, 0x5fc26037,
    0x5fe3b38c, 0x6004f818, 0x60262dd4, 0x604754bd, 0x60686cce, 0x60897600, 0x60aa704f, 0x60cb5bb5,
    0x60ec382f, 0x610d05b6, 0x612dc445, 0x614e73d9, 0x616f146a, 0x618fa5f6, 0x61b02875, 0x61d09be4,
    0x61f1003e, 0x6211557d, 0x62319b9c, 0x6251d297, 0x6271fa68, 0x6292130b, 0x62b21c7a, 0x62d216b1,
    0x62f201ab, 0x6311dd63, 0x6331a9d3, 0x635166f8, 0x637114cb, 0x6390b349, 0x63b0426c, 0x63cfc22f,
    0x63ef328e, 0x640e9384, 0x642de50c, 0x644d2721, 0x646c59be, 0x648b7cdf, 0x64aa907e, 0x64c99497,
    0x64e88925, 0x65076e23, 0x6526438d, 0x6545095e, 0x6563bf91, 0x65826621, 0x65a0fd0a, 0x65bf8446,
    0x65ddfbd2, 0x65fc63a8, 0x661abbc4, 0x66390421, 0x66573cba, 0x6675658b, 0x66937e8f, 0x66b187c2,
    0x66cf811f, 0x66ed6aa0, 0x670b4442, 0x67290e01, 0x6746c7d6, 0x676471bf, 0x67820bb6, 0x679f95b6,
    0x67bd0fbb, 0x67da79c1, 0x67f7d3c3, 0x68151dbd, 0x683257aa, 0x684f8185, 0x686c9b4a, 0x6889a4f4,
    0x68a69e80, 0x68c387e8, 0x68e06128, 0x68fd2a3c, 0x6919e31f, 0x69368bcd, 0x69532441, 0x696fac77,
    0x698c246b, 0x69a88c18, 0x69c4e379, 0x69e12a8b, 0x69fd6149, 0x6a1987af, 0x6a359db8, 0x6a51a360,
    0x6a6d98a3, 0x6a897d7c, 0x6aa551e7, 0x6ac115e0, 0x6adcc963, 0x6af86c6b, 0x6b13fef4, 0x6b2f80f9,
    0x6b4af277, 0x6b665369, 0x6b81a3cc, 0x6b9ce39a, 0x6bb812d0, 0x6bd33169, 0x6bee3f61, 0x6c093cb5,
    0x6c24295f, 0x6c3f055c, 0x6c59d0a8, 0x6c748b3e, 0x6c8f351b, 0x6ca9ce39, 0x6cc45696, 0x6cdece2d,
    0x6cf934fa, 0x6d138af9, 0x6d2dd026, 0x6d48047d, 0x6d6227f9, 0x6d7c3a97, 0x6d963c53, 0x6db02d28,
    0x6dca0d13, 0x6de3dc10, 0x6dfd9a1a, 0x6e17472e, 0x6e30e348, 0x6e4a6e64, 0x6e63e87e, 0x6e7d5192,
    0x6e96a99b, 0x6eaff097, 0x6ec92681, 0x6ee24b56, 0x6efb5f11, 0x6f1461af, 0x6f2d532b, 0x6f463382,
    0x6f5f02b0, 0x6f77c0b2, 0x6f906d83, 0x6fa90920, 0x6fc19384, 0x6fda0cac, 0x6ff27495, 0x700acb3a,
    0x70231098, 0x703b44ab, 0x70536770, 0x706b78e2, 0x708378fd, 0x709b67bf, 0x70b34523, 0x70cb1126,
    0x70e2cbc5, 0x70fa74fa, 0x71120cc4, 0x7129931d, 0x71410803, 0x71586b72, 0x716fbd67, 0x7186fddd,
    0x719e2cd1, 0x71b54a3f, 0x71cc5625, 0x71e3507e, 0x71fa3947, 0x7211107c, 0x7227d61b, 0x723e8a1e,
    0x72552c83, 0x726bbd47, 0x72823c66, 0x7298a9db, 0x72af05a5, 0x72c54fbf, 0x72db8827, 0x72f1aed7,
    0x7307c3cf, 0x731dc708, 0x7333b882, 0x73499837, 0x735f6625, 0x73752248, 0x738acc9d, 0x73a06521,
    0x73b5ebd0, 0x73cb60a6, 0x73e0c3a2, 0x73f614bf, 0x740b53f9, 0x7420814f, 0x74359cbc, 0x744aa63d,
    0x745f9dd0, 0x74748370, 0x7489571a, 0x749e18cc, 0x74b2c882, 0x74c76639, 0x74dbf1ee, 0x74f06b9d,
    0x7504d344, 0x751928df, 0x752d6c6b, 0x75419de5, 0x7555bd4a, 0x7569ca97, 0x757dc5c9, 0x7591aedc,
    0x75a585ce, 0x75b94a9b, 0x75ccfd41, 0x75e09dbc, 0x75f42c09, 0x7607a826, 0x761b1210, 0x762e69c2,
    0x7641af3b, 0x7654e278, 0x76680375, 0x767b122f, 0x768e0ea4, 0x76a0f8d1, 0x76b3d0b2, 0x76c69645,
    0x76d94987, 0x76ebea76, 0x76fe790d, 0x7710f54b, 0x77235f2c, 0x7735b6ad, 0x7747fbcd, 0x775a2e87,
    0x776c4eda, 0x777e5cc2, 0x7790583c, 0x77a24147, 0x77b417de, 0x77c5dc00, 0x77d78da9, 0x77e92cd7,
    0x77fab987, 0x780c33b7, 0x781d9b63, 0x782ef08a, 0x78403327, 0x7851633a, 0x786280be, 0x78738bb2,
    0x78848412, 0x789569dd, 0x78a63d0f, 0x78b6fda7, 0x78c7aba0, 0x78d846fa, 0x78e8cfb0, 0x78f945c2,
    0x7909a92b, 0x7919f9ea, 0x792a37fd, 0x793a635f, 0x794a7c10, 0x795a820d, 0x796a7553, 0x797a55df,
    0x798a23b0, 0x7999dec2, 0x79a98714, 0x79b91ca3, 0x79c89f6c, 0x79d80f6e, 0x79e76ca5, 0x79f6b710,
    0x7a05eeac, 0x7a151376, 0x7a24256d, 0x7a33248e, 0x7a4210d7, 0x7a50ea45, 0x7a5fb0d7, 0x7a6e6489,
    0x7a7d055a, 0x7a8b9347, 0x7a9a0e4e, 0x7aa8766d, 0x7ab6cba2, 0x7ac50dea, 0x7ad33d44, 0x7ae159ad,
    0x7aef6322, 0x7afd59a2, 0x7b0b3d2b, 0x7b190dba, 0x7b26cb4e, 0x7b3475e3, 0x7b420d79, 0x7b4f920d,
    0x7b5d039c, 0x7b6a6226, 0x7b77ada7, 0x7b84e61d, 0x7b920b88, 0x7b9f1de4, 0x7bac1d30, 0x7bb90969,
    0x7bc5e28e, 0x7bd2a89d, 0x7bdf5b93, 0x7bebfb6f, 0x7bf8882f, 0x7c0501d0, 0x7c116852, 0x7c1dbbb1,
    0x7c29fbed, 0x7c362903, 0x7c4242f1, 0x7c4e49b5, 0x7c5a3d4e, 0x7c661dba, 0x7c71eaf7, 0x7c7da503,
    0x7c894bdc, 0x7c94df81, 0x7ca05ff0, 0x7cabcd26, 0x7cb72723, 0x7cc26de4, 0x7ccda167, 0x7cd8c1ac,
    0x7ce3ceb0, 0x7ceec872, 0x7cf9aeef, 0x7d048226, 0x7d0f4217, 0x7d19eebd, 0x7d248819, 0x7d2f0e29,
    0x7d3980eb, 0x7d43e05d, 0x7d4e2c7d, 0x7d58654b, 0x7d628ac4, 0x7d6c9ce8, 0x7d769bb4, 0x7d808726,
    0x7d8a5f3e, 0x7d9423fa, 0x7d9dd559, 0x7da77358, 0x7db0fdf6, 0x7dba7533, 0x7dc3d90c, 0x7dcd2980,
    0x7dd6668d, 0x7ddf9033, 0x7de8a66f, 0x7df1a941, 0x7dfa98a6, 0x7e03749e, 0x7e0c3d28, 0x7e14f241,
    0x7e1d93e8, 0x7e26221d, 0x7e2e9cde, 0x7e370429, 0x7e3f57fd, 0x7e47985a, 0x7e4fc53d, 0x7e57dea5,
    0x7e5fe492, 0x7e67d701, 0x7e6fb5f2, 0x7e778164, 0x7e7f3955, 0x7e86ddc4, 0x7e8e6eb0, 0x7e95ec18,
    0x7e9d55fb, 0x7ea4ac57, 0x7eabef2b, 0x7eb31e76, 0x7eba3a38, 0x7ec1426e, 0x7ec83719, 0x7ecf1836,
    0x7ed5e5c5, 0x7edc9fc5, 0x7ee34634, 0x7ee9d912, 0x7ef0585e, 0x7ef6c417, 0x7efd1c3b, 0x7f0360c9,
    0x7f0991c2, 0x7f0faf23, 0x7f15b8ec, 0x7f1baf1c, 0x7f2191b2, 0x7f2760ae, 0x7f2d1c0d, 0x7f32c3cf,
    0x7f3857f4, 0x7f3dd87b, 0x7f434562, 0x7f489ea9, 0x7f4de44f, 0x7f531653, 0x7f5834b5, 0x7f5d3f74,
    0x7f62368e, 0x7f671a03, 0x7f6be9d3, 0x7f70a5fc, 0x7f754e7e, 0x7f79e359, 0x7f7e648a, 0x7f82d213,
    0x7f872bf1, 0x7f8b7225, 0x7f8fa4ae, 0x7f93c38b, 0x7f97cebb, 0x7f9bc63e, 0x7f9faa14, 0x7fa37a3b,
    0x7fa736b3, 0x7faadf7b, 0x7fae7493, 0x7fb1f5fb, 0x7fb563b1, 0x7fb8bdb6, 0x7fbc0409, 0x7fbf36a8,
    0x7fc25595, 0x7fc560cd, 0x7fc85852, 0x7fcb3c22, 0x7fce0c3d, 0x7fd0c8a2, 0x7fd37151, 0x7fd6064a,
    0x7fd8878c, 0x7fdaf517, 0x7fdd4eeb, 0x7fdf9507, 0x7fe1c76a, 0x7fe3e615, 0x7fe5f107, 0x7fe7e83f,
    0x7fe9cbbe, 0x7feb9b84, 0x7fed578f, 0x7feeffe0, 0x7ff09476, 0x7ff21552, 0x7ff38272, 0x7ff4dbd7,
    0x7ff62181, 0x7ff7536e, 0x7ff871a0, 0x7ff97c16, 0x7ffa72d0, 0x7ffb55cd, 0x7ffc250e, 0x7ffce092,
    0x7ffd8859, 0x7ffe1c63, 0x7ffe9cb1, 0x7fff0941, 0x7fff6215, 0x7fffa72b, 0x7fffd884, 0x7ffff620,
    0x7fffffff, 0x7ffff620, 0x7fffd884, 0x7fffa72b, 0x7fff6215, 0x7fff0941, 0x7ffe9cb1, 0x7ffe1c63,
    0x7ffd8859, 0x7ffce092, 0x7ffc250e, 0x7ffb55cd, 0x7ffa72d0, 0x7ff97c16, 0x7ff871a0, 0x7ff7536e,
    0x7ff62181, 0x7ff4dbd7, 0x7ff38272, 0x7ff21552, 0x7ff09476, 0x7feeffe0, 0x7fed578f, 0x7feb9b84,
    0x7fe9cbbe, 0x7fe7e83f, 0x7fe5f107, 0x7fe3e615, 0x7fe1c76a, 0x7fdf9507, 0x7fdd4eeb, 0x7fdaf517,
    0x7fd8878c, 0x7fd6064a, 0x7fd37151, 0x7fd0c8a2, 0x7fce0c3d, 0x7fcb3c22, 0x7fc85852, 0x7fc560cd,
    0x7fc25595, 0x7fbf36a8, 0x7fbc0409, 0x7fb8bdb6, 0x7fb563b1, 0x7fb1f5fb, 0x7fae7493, 0x7faadf7b,
    0x7fa736b3, 0x7fa37a3b, 0x7f9faa14, 0x7f9bc63e, 0x7f97cebb, 0x7f93c38b, 0x7f8fa4ae, 0x7f8b7225,
    0x7f872bf1, 0x7f82d213, 0x7f7e648a, 0x7f79e359, 0x7f754e7e, 0x7f70a5fc, 0x7f6be9d3, 0x7f671a03,
    0x7f62368e, 0x7f5d3f74, 0x7f5834b5, 0x7f531653, 0x7f4de44f, 0x7f489ea9, 0x7f434562, 0x7f3dd87b,
    0x7f3857f4, 0x7f32c3cf, 0x7f2d1c0d, 0x7f2760ae, 0x7f2191b2, 0x7f1baf1c, 0x7f15b8ec, 0x7f0faf23,
    0x7f0991c2, 0x7f0360c9, 0x7efd1c3b, 0x7ef6c417, 0x7ef0585e, 0x7ee9d912, 0x7ee34634, 0x7edc9fc5,
    0x7ed5e5c5, 0x7ecf1836, 0x7ec83719, 0x7ec1426e, 0x7eba3a38, 0x7eb31e76, 0x7eabef2b, 0x7ea4ac57,
    0x7e9d55fb, 0x7e95ec18, 0x7e8e6eb0, 0x7e86ddc4, 0x7e7f3955, 0x7e778164, 0x7e6fb5f2, 0x7e67d701,
    0x7e5fe492, 0x7e57dea5, 0x7e4fc53d, 0x7e47985a, 0x7e3f57fd, 0x7e370429, 0x7e2e9cde, 0x7e26221d,
    0x7e1d93e8, 0x7e14f241, 0x7e0c3d28, 0x7e03749e, 0x7dfa98a6, 0x7df1a941, 0x7de8a66f, 0x7ddf9033,
    0x7dd6668d, 0x7dcd2980, 0x7dc3d90c, 0x7dba7533, 0x7db0fdf6, 0x7da77358, 0x7d9dd559, 0x7d9423fa,
    0x7d8a5f3e, 0x7d808726, 0x7d769bb4, 0x7d6c9ce8, 0x7d628ac4, 0x7d58654b, 0x7d4e2c7d, 0x7d43e05d,
    0x7d3980eb, 0x7d2f0e29, 0x7d248819, 0x7d19eebd, 0x7d0f4217, 0x7d048226, 0x7cf9aeef, 0x7ceec872,
    0x7ce3ceb0, 0x7cd8c1ac, 0x7ccda167, 0x7cc26de4, 0x7cb72723, 0x7cabcd26, 0x7ca05ff0, 0x7c94df81,
    0x7c894bdc, 0x7c7da503, 0x7c71eaf7, 0x7c661dba, 0x7c5a3d4e, 0x7c4e49b5, 0x7c4242f1, 0x7c362903,
    0x7c29fbed, 0x7c1dbbb1, 0x7c116852, 0x7c0501d0, 0x7bf8882f, 0x7bebfb6f, 0x7bdf5b93, 0x7bd2a89d,
    0x7bc5e28e, 0x7bb90969, 0x7bac1d30, 0x7b9f1de4, 0x7b920b88, 0x7b84e61d, 0x7b77ada7, 0x7b6a6226,
    0x7b5d039c, 0x7b4f920d, 0x7b420d79, 0x7b3475e3, 0x7b26cb4e, 0x7b190dba, 0x7b0b3d2b, 0x7afd59a2,
    0x7aef6322, 0x7ae159ad, 0x7ad33d44, 0x7ac50dea, 0x7ab6cba2, 0x7aa8766d, 0x7a9a0e4e, 0x7a8b9347,
    0x7a7d055a, 0x7a6e6489, 0x7a5fb0d7, 0x7a50ea45, 0x7a4210d7, 0x7a33248e, 0x7a24256d, 0x7a151376,
    0x7a05eeac, 0x79f6b710, 0x79e76ca5, 0x79d80f6e, 0x79c89f6c, 0x79b91ca3, 0x79a98714, 0x7999dec2,
    0x798a23b0, 0x797a55df, 0x796a7553, 0x795a820d, 0x794a7c10, 0x793a635f, 0x792a37fd, 0x7919f9ea,
    0x7909a92b, 0x78f945c2, 0x78e8cfb0, 0x78d846fa, 0x78c7aba0, 0x78b6fda7, 0x78a63d0f, 0x789569dd,
    0x78848412, 0x78738bb2, 0x786280be, 0x7851633a, 0x78403327, 0x782ef08a, 0x781d9b63, 0x780c33b7,
    0x77fab987, 0x77e92cd7, 0x77d78da9, 0x77c5dc00, 0x77b417de, 0x77a24147, 0x7790583c, 0x777e5cc2,
    0x776c4eda, 0x775a2e87, 0x7747fbcd, 0x7735b6ad, 0x77235f2c, 0x7710f54b, 0x76fe790d, 0x76ebea76,
    0x76d94987, 0x76c69645, 0x76b3d0b2, 0x76a0f8d1, 0x768e0ea4, 0x767b122f, 0x76680375, 0x7654e278,
    0x7641af3b, 0x762e69c2, 0x761b1210, 0x7607a826, 0x75f42c09, 0x75e09dbc, 0x75ccfd41, 0x75b94a9b,
    0x75a585ce, 0x7591aedc, 0x757dc5c9, 0x7569ca97, 0x7555bd4a, 0x75419de5, 0x752d6c6b, 0x751928df,
    0x7504d344, 0x74f06b9d, 0x74dbf1ee, 0x74c76639, 0x74b2c882, 0x749e18cc, 0x7489571a, 0x74748370,
    0x745f9dd0, 0x744aa63d, 0x74359cbc, 0x7420814f, 0x740b53f9, 0x73f614bf, 0x73e0c3a2, 0x73cb60a6,
    0x73b5ebd0, 0x73a06521, 0x738acc9d, 0x73752248, 0x735f6625, 0x73499837, 0x7333b882, 0x731dc708,
    0x7307c3cf, 0x72f1aed7, 0x72db8827, 0x72c54fbf, 0x72af05a5, 0x7298a9db, 0x72823c66, 0x726bbd47,
    0x72552c83, 0x723e8a1e, 0x7227d61b, 0x7211107c, 0x71fa3947, 0x71e3507e, 0x71cc5625, 0x71b54a3f,
    0x719e2cd1, 0x7186fddd, 0x716fbd67, 0x71586b72, 0x71410803, 0x7129931d, 0x71120cc4, 0x70fa74fa,
    0x70e2cbc5, 0x70cb1126, 0x70b34523, 0x709b67bf, 0x708378fd, 0x706b78e2, 0x70536770, 0x703b44ab,
    0x70231098, 0x700acb3a, 0x6ff27495, 0x6fda0cac, 0x6fc19384, 0x6fa90920, 0x6f906d83, 0x6f77c0b2,
    0x6f5f02b0, 0x6f463382, 0x6f2d532b, 0x6f1461af, 0x6efb5f11, 0x6ee24b56, 0x6ec92681, 0x6eaff097,
    0x6e96a99b, 0x6e7d5192, 0x6e63e87e, 0x6e4a6e64, 0x6e30e348, 0x6e17472e, 0x6dfd9a1a, 0x6de3dc10,
    0x6dca0d13, 0x6db02d28, 0x6d963c53, 0x6d7c3a97, 0x6d6227f9, 0x6d48047d, 0x6d2dd026, 0x6d138af9,
    0x6cf934fa, 0x6cdece2d, 0x6cc45696, 0x6ca9ce39, 0x6c8f351b, 0x6c748b3e, 0x6c59d0a8, 0x6c3f055c,
    0x6c24295f, 0x6c093cb5, 0x6bee3f61, 0x6bd33169, 0x6bb812d0, 0x6b9ce39a, 0x6b81a3cc, 0x6b665369,
    0x6b4af277, 0x6b2f80f9, 0x6b13fef4, 0x6af86c6b, 0x6adcc963, 0x6ac115e0, 0x6aa551e7, 0x6a897d7c,
    0x6a6d98a3, 0x6a51a360, 0x6a359db8, 0x6a1987af, 0x69fd6149, 0x69e12a8b, 0x69c4e379, 0x69a88c18,
    0x698c246b, 0x696fac77, 0x69532441, 0x69368bcd, 0x6919e31f, 0x68fd2a3c, 0x68e06128, 0x68c387e8,
    0x68a69e80, 0x6889a4f4, 0x686c9b4a, 0x684f8185, 0x683257aa, 0x68151dbd, 0x67f7d3c3, 0x67da79c1,
    0x67bd0fbb, 0x679f95b6, 0x67820bb6, 0x676471bf, 0x6746c7d6, 0x67290e01, 0x670b4442, 0x66ed6aa0,
    0x66cf811f, 0x66b187c2, 0x66937e8f, 0x6675658b, 0x66573cba, 0x66390421, 0x661abbc4, 0x65fc63a8,
    0x65ddfbd2, 0x65bf8446, 0x65a0fd0a, 0x65826621, 0x6563bf91, 0x6545095e, 0x6526438d, 0x65076e23,
    0x64e88925, 0x64c99497, 0x64aa907e, 0x648b7cdf, 0x646c59be, 0x644d2721, 0x642de50c, 0x640e9384,
    0x63ef328e, 0x63cfc22f, 0x63b0426c, 0x6390b349, 0x637114cb, 0x635166f8, 0x6331a9d3, 0x6311dd63,
    0x62f201ab, 0x62d216b1, 0x62b21c7a, 0x6292130b, 0x6271fa68, 0x6251d297, 0x62319b9c, 0x6211557d,
    0x61f1003e, 0x61d09be4, 0x61b02875, 0x618fa5f6, 0x616f146a, 0x614e73d9, 0x612dc445, 0x610d05b6,
    0x60ec382f, 0x60cb5bb5, 0x60aa704f, 0x60897600, 0x60686cce, 0x604754bd, 0x60262dd4, 0x6004f818,
    0x5fe3b38c, 0x5fc26037, 0x5fa0fe1e, 0x5f7f8d45, 0x5f5e0db2, 0x5f3c7f6a, 0x5f1ae272, 0x5ef936d0,
    0x5ed77c88, 0x5eb5b3a1, 0x5e93dc1e, 0x5e71f605, 0x5e50015c, 0x5e2dfe28, 0x5e0bec6d, 0x5de9cc32,
    0x5dc79d7b, 0x5da5604e, 0x5d8314b0, 0x5d60baa6, 0x5d3e5235, 0x5d1bdb64, 0x5cf95637, 0x5cd6c2b3,
    0x5cb420df, 0x5c9170be, 0x5c6eb257, 0x5c4be5af, 0x5c290acb, 0x5c0621b1, 0x5be32a66, 0x5bc024ef,
    0x5b9d1152, 0x5b79ef95, 0x5b56bfbc, 0x5b3381cd, 0x5b1035ce, 0x5aecdbc3, 0x5ac973b4, 0x5aa5fda3,
    0x5a827999, 0x5a5ee799, 0x5a3b47a9, 0x5a1799d0, 0x59f3de11, 0x59d01473, 0x59ac3cfc, 0x598857b1,
    0x59646497, 0x594063b3, 0x591c550d, 0x58f838a8, 0x58d40e8b, 0x58afd6bb, 0x588b913e, 0x58673e1a,
    0x5842dd53, 0x581e6ef0, 0x57f9f2f6, 0x57d5696b, 0x57b0d255, 0x578c2db8, 0x57677b9c, 0x5742bc05,
    0x571deef8, 0x56f9147d, 0x56d42c98, 0x56af374f, 0x568a34a8, 0x566524a9, 0x56400757, 0x561adcb8,
    0x55f5a4d1, 0x55d05fa9, 0x55ab0d45, 0x5585adab, 0x556040e1, 0x553ac6ed, 0x55153fd3, 0x54efab9b,
    0x54ca0a49, 0x54a45be5, 0x547ea072, 0x5458d7f8, 0x5433027c, 0x540d2004, 0x53e73096, 0x53c13437,
    0x539b2aee, 0x537514c1, 0x534ef1b4, 0x5328c1cf, 0x53028517, 0x52dc3b91, 0x52b5e544, 0x528f8237,
    0x5269126d, 0x524295ef, 0x521c0cc0, 0x51f576e9, 0x51ced46d, 0x51a82554, 0x518169a4, 0x515aa161,
    0x5133cc93, 0x510ceb3f, 0x50e5fd6c, 0x50bf031e, 0x5097fc5d, 0x5070e92e, 0x5049c998, 0x50229da0,
    0x4ffb654c, 0x4fd420a3, 0x4faccfaa, 0x4f857268, 0x4f5e08e2, 0x4f36931f, 0x4f0f1125, 0x4ee782fa,
    0x4ebfe8a3, 0x4e984228, 0x4e708f8e, 0x4e48d0dc, 0x4e210616, 0x4df92f45, 0x4dd14c6d, 0x4da95d95,
    0x4d8162c3, 0x4d595bfd, 0x4d31494a, 0x4d092aaf, 0x4ce10033, 0x4cb8c9dc, 0x4c9087b0, 0x4c6839b5,
    0x4c3fdff2, 0x4c177a6d, 0x4bef092c, 0x4bc68c35, 0x4b9e038f, 0x4b756f3f, 0x4b4ccf4c, 0x4b2423bd,
    0x4afb6c97, 0x4ad2a9e1, 0x4aa9dba1, 0x4a8101dd, 0x4a581c9c, 0x4a2f2be5, 0x4a062fbc, 0x49dd2829,
    0x49b41532, 0x498af6de, 0x4961cd32, 0x49389835, 0x490f57ed, 0x48e60c61, 0x48bcb598, 0x48935396,
    0x4869e664, 0x48406e06, 0x4816ea85, 0x47ed5be5, 0x47c3c22e, 0x479a1d66, 0x47706d92, 0x4746b2bb,
    0x471cece6, 0x46f31c19, 0x46c9405b, 0x469f59b3, 0x46756827, 0x464b6bbd, 0x4621647c, 0x45f7526a,
    0x45cd358e, 0x45a30def, 0x4578db92, 0x454e9e7f, 0x452456bc, 0x44fa044f, 0x44cfa73f, 0x44a53f92,
    0x447acd4f, 0x4450507d, 0x4425c922, 0x43fb3745, 0x43d09aec, 0x43a5f41d, 0x437b42e0, 0x4350873b,
    0x4325c134, 0x42faf0d3, 0x42d0161d, 0x42a5311a, 0x427a41cf, 0x424f4844, 0x42244480, 0x41f93688,
    0x41ce1e64, 0x41a2fc19, 0x4177cfb0, 0x414c992e, 0x4121589a, 0x40f60dfa, 0x40cab957, 0x409f5ab5,
    0x4073f21c, 0x40487f93, 0x401d0320, 0x3ff17cc9, 0x3fc5ec97, 0x3f9a528f, 0x3f6eaeb7, 0x3f430118,
    0x3f1749b7, 0x3eeb889b, 0x3ebfbdcc, 0x3e93e94f, 0x3e680b2c, 0x3e3c2368, 0x3e10320d, 0x3de4371e,
    0x3db832a5, 0x3d8c24a7, 0x3d600d2b, 0x3d33ec38, 0x3d07c1d5, 0x3cdb8e09, 0x3caf50da, 0x3c830a4f,
    0x3c56ba6f, 0x3c2a6141, 0x3bfdfecc, 0x3bd19317, 0x3ba51e28, 0x3b78a006, 0x3b4c18b9, 0x3b1f8847,
    0x3af2eeb6, 0x3ac64c0e, 0x3a99a056, 0x3a6ceb95, 0x3a402dd1, 0x3a136711, 0x39e6975d, 0x39b9bebb,
    0x398cdd31, 0x395ff2c8, 0x3932ff86, 0x39060372, 0x38d8fe92, 0x38abf0ee, 0x387eda8d, 0x3851bb76,
    0x382493af, 0x37f76340, 0x37ca2a2f, 0x379ce884, 0x376f9e45, 0x37424b7a, 0x3714f029, 0x36e78c5a,
    0x36ba2013, 0x368cab5b, 0x365f2e3a, 0x3631a8b7, 0x36041ad8, 0x35d684a5, 0x35a8e624, 0x357b3f5d,
    0x354d9056, 0x351fd917, 0x34f219a7, 0x34c4520c, 0x3496824f, 0x3468aa75, 0x343aca87, 0x340ce28a,
    0x33def286, 0x33b0fa83, 0x3382fa87, 0x3354f29a, 0x3326e2c2, 0x32f8cb06, 0x32caab6e, 0x329c8402,
    0x326e54c7, 0x32401dc5, 0x3211df03, 0x31e39889, 0x31b54a5d, 0x3186f486, 0x3158970d, 0x312a31f7,
    0x30fbc54c, 0x30cd5114, 0x309ed555, 0x30705217, 0x3041c760, 0x30133538, 0x2fe49ba6, 0x2fb5fab1,
    0x2f875261, 0x2f58a2bd, 0x2f29ebcb, 0x2efb2d94, 0x2ecc681d, 0x2e9d9b70, 0x2e6ec791, 0x2e3fec8a,
    0x2e110a61, 0x2de2211d, 0x2db330c6, 0x2d843963, 0x2d553afb, 0x2d263595, 0x2cf72939, 0x2cc815ed,
    0x2c98fbba, 0x2c69daa6, 0x2c3ab2b8, 0x2c0b83f9, 0x2bdc4e6e, 0x2bad1221, 0x2b7dcf16, 0x2b4e8557,
    0x2b1f34eb, 0x2aefddd7, 0x2ac08025, 0x2a911bdb, 0x2a61b101, 0x2a323f9d, 0x2a02c7b7, 0x29d34957,
    0x29a3c484, 0x29743945, 0x2944a7a1, 0x29150fa1, 0x28e5714a, 0x28b5cca5, 0x288621b8, 0x2856708c,
    0x2826b927, 0x27f6fb92, 0x27c737d2, 0x27976df0, 0x27679df3, 0x2737c7e3, 0x2707ebc6, 0x26d809a4,
    0x26a82185, 0x26783370, 0x26483f6c, 0x26184580, 0x25e845b5, 0x25b84011, 0x2588349c, 0x2558235e,
    0x25280c5d, 0x24f7efa1, 0x24c7cd32, 0x2497a517, 0x24677757, 0x243743fa, 0x24070b07, 0x23d6cc86,
    0x23a6887e, 0x23763ef6, 0x2345eff7, 0x23159b87, 0x22e541ae, 0x22b4e274, 0x22847ddf, 0x225413f7,
    0x2223a4c5, 0x21f3304e, 0x21c2b69b, 0x219237b4, 0x2161b39f, 0x21312a64, 0x21009c0b, 0x20d0089b,
    0x209f701c, 0x206ed294, 0x203e300d, 0x200d888c, 0x1fdcdc1a, 0x1fac2abe, 0x1f7b7480, 0x1f4ab967,
    0x1f19f97a, 0x1ee934c2, 0x1eb86b45, 0x1e879d0c, 0x1e56ca1d, 0x1e25f281, 0x1df5163e, 0x1dc4355d,
    0x1d934fe5, 0x1d6265dd, 0x1d31774c, 0x1d00843c, 0x1ccf8cb2, 0x1c9e90b7, 0x1c6d9053, 0x1c3c8b8c,
    0x1c0b826a, 0x1bda74f5, 0x1ba96334, 0x1b784d30, 0x1b4732ef, 0x1b161479, 0x1ae4f1d5, 0x1ab3cb0c,
    0x1a82a025, 0x1a517127, 0x1a203e1a, 0x19ef0706, 0x19bdcbf2, 0x198c8ce6, 0x195b49e9, 0x192a0303,
    0x18f8b83c, 0x18c7699b, 0x18961727, 0x1864c0e9, 0x183366e8, 0x1802092b, 0x17d0a7bb, 0x179f429f,
    0x176dd9de, 0x173c6d80, 0x170afd8c, 0x16d98a0b, 0x16a81304, 0x1676987f, 0x16451a82, 0x16139917,
    0x15e21444, 0x15b08c11, 0x157f0086, 0x154d71aa, 0x151bdf85, 0x14ea4a1f, 0x14b8b17f, 0x148715ad,
    0x145576b1, 0x1423d491, 0x13f22f57, 0x13c0870a, 0x138edbb0, 0x135d2d53, 0x132b7bf9, 0x12f9c7aa,
    0x12c8106e, 0x1296564d, 0x1264994d, 0x1232d978, 0x120116d4, 0x11cf516a, 0x119d8940, 0x116bbe5f,
    0x1139f0ce, 0x11082095, 0x10d64dbc, 0x10a4784a, 0x1072a047, 0x1040c5bb, 0x100ee8ad, 0x0fdd0925,
    0x0fab272b, 0x0f7942c6, 0x0f475bfe, 0x0f1572dc, 0x0ee38765, 0x0eb199a3, 0x0e7fa99d, 0x0e4db75a,
    0x0e1bc2e3, 0x0de9cc3f, 0x0db7d376, 0x0d85d88e, 0x0d53db92, 0x0d21dc86, 0x0cefdb75, 0x0cbdd865,
    0x0c8bd35d, 0x0c59cc67, 0x0c27c389, 0x0bf5b8cb, 0x0bc3ac35, 0x0b919dce, 0x0b5f8d9f, 0x0b2d7bae,
    0x0afb6805, 0x0ac952aa, 0x0a973ba5, 0x0a6522fd, 0x0a3308bc, 0x0a00ece8, 0x09cecf89, 0x099cb0a7,
    0x096a9049, 0x09386e77, 0x09064b3a, 0x08d42698, 0x08a2009a, 0x086fd947, 0x083db0a7, 0x080b86c1,
    0x07d95b9e, 0x07a72f45, 0x077501be, 0x0742d310, 0x0710a344, 0x06de7261, 0x06ac406f, 0x067a0d75,
    0x0647d97c, 0x0615a48a, 0x05e36ea9, 0x05b137df, 0x057f0034, 0x054cc7b0, 0x051a8e5b, 0x04e8543d,
    0x04b6195d, 0x0483ddc3, 0x0451a176, 0x041f647f, 0x03ed26e6, 0x03bae8b1, 0x0388a9e9, 0x03566a96,
    0x03242abe, 0x02f1ea6b, 0x02bfa9a4, 0x028d6870, 0x025b26d7, 0x0228e4e1, 0x01f6a296, 0x01c45ffe,
    0x01921d1f, 0x015fda03, 0x012d96b0, 0x00fb532f, 0x00c90f87, 0x0096cbc1, 0x006487e2, 0x003243f5,    	
};

int FindIndex12bit(int k)
{
	int j;
	k=k&(0xfff);
	j=((k&0x001)<<11)+((k&0x002)<<9)+((k&0x004)<<7)+((k&0x008)<<5)+((k&0x010)<<3)+((k&0x020)<<1)+((k&0x040)>>1)+((k&0x080)>>3)+((k&0x100)>>5)+((k&0x200)>>7)+((k&0x400)>>9)+((k&0x800)>>11);
	return j;
}

/***********************************************************
* inverse subscript 
***********************************************************/
void ReverseIndex(COMPLEX *x){
int i,j;
COMPLEX tmp;
for (i=0;i<N;i++){
	j=FindIndex12bit(i);
	if (j>i){
		tmp.real=x[i].real;
		tmp.imag=x[i].imag;
		x[i].real =x[j].real;
		x[i].imag =x[j].imag;
		x[j].real=tmp.real;
		x[j].imag=tmp.imag;
		}
	}
}

/***********************************************************
* Function      : fft
* Description   : This is a in-place radix 2 FFT, i.e. output will directly
                  overwrite the input.
* Parameter     : argu 1: (i/o): buffer address of the real part
                  argu 2: (i/o): buffer address of the image part
                  argu 3: (i): FFT length in power of 2
* Return        : None
***********************************************************/
void Fixed_fft(COMPLEX *x)
{				
	int i,j,k,u=0,l=0,wi=0;				
	int SubBlockNum,SubBlockStep=1;		
	bool ov,ovd;							
	//COMPLEX wn[N/2],XkWn;
	
	for (k=0;k<N/2;k++)
        {               
			//printf("40 wn[k].real = 0x%x, wn[k].imag = 0x%x \n",wn[k].real, wn[k].imag);
			wn[k].real=Cos_PAR[k];
			wn[k].imag=Sin_PAR[k];
		}
	ReverseIndex(x);					
	for(k=N;k>1;k=(k>>1))
        {               
		    ov=0;ovd=0;						
		    for(i=0;i<N;i++)
		    {
			    if (x[i].real>DOVS || x[i].imag>DOVS || x[i].real<-DOVS || x[i].imag<-DOVS)				
			    {
				    ovd=1;
				    break;
			    }
				else if (x[i].real>OVS || x[i].imag>OVS || x[i].real<-OVS || x[i].imag<-OVS)			
    			{
    				ov=1;
    			}
		    }
    		if (ovd==1)
    		{
    			for(i=0;i<N;i++)
    			{
    				x[i].real=x[i].real>>2;
    				x[i].imag=x[i].imag>>2;
    			}
    		}
			else if(ov==1)
			{
				for(i=0;i<N;i++)
    			{
    				x[i].real=x[i].real>>1;
    				x[i].imag=x[i].imag>>1;
    			}
			}
			
    		SubBlockNum=k>>1;				
    		SubBlockStep=SubBlockStep<<1;	
    		wi=0;							
    		for(j=0;j<SubBlockStep>>1;j++)
                {   
    			    for(u=j;u<N;u+=SubBlockStep)
                    {           
        				l=u+(SubBlockStep>>1);	
        				#if (BITNUM==32)			
						XkWn.real=(((int64)x[l].real*wn[wi].real)>>SHN)-(((int64)x[l].imag*wn[wi].imag)>>SHN);
						XkWn.imag=(((int64)x[l].imag*wn[wi].real)>>SHN)+(((int64)x[l].real*wn[wi].imag)>>SHN);
        				#else
        				XkWn.real=((x[l].real*wn[wi].real)>>SHN)-((x[l].imag*wn[wi].imag)>>SHN);
        				XkWn.imag=((x[l].imag*wn[wi].real)>>SHN)+((x[l].real*wn[wi].imag)>>SHN);
        				#endif
        				x[l].real=x[u].real-XkWn.real;
        				x[l].imag=x[u].imag-XkWn.imag;
        				x[u].real=x[u].real+XkWn.real;
        				x[u].imag=x[u].imag+XkWn.imag;
    			    }
    			    wi+=SubBlockNum;			
    		    }
	}

}

static void _sltAudInit(UINT8 u1DspId)
{
    MEM_BUFFER_INFO_T rDecMemBuf;

    rDecMemBuf.pData = (UINT8 *)_au1Stream;
    rDecMemBuf.u4Length = AUD_BITSTREAM_LEN;

    LOG(5, "Audio stream addr = 0x%0x, len = %d\n", rDecMemBuf.pData, rDecMemBuf.u4Length);

    // Setup memory buffer address
    AUD_SetDecMemBuffer(u1DspId, AUD_DEC_MAIN, (const MEM_BUFFER_INFO_T *) &rDecMemBuf);
    // Feed audio bitstream from memory
    //CLI_Parser("aud.s 0 6 2");
    //AUD_SetDecType(AUD_DEC_MAIN,6, 2);
    AUD_SetDecType(u1DspId, AUD_DEC_MAIN,AUD_STREAM_FROM_MEMORY, AUD_FMT_AC3);
    AUD_SltLineInInit(u1DspId);
}
/*
static VOID _sltAudCaptureReady(HANDLE_T  pt_tm_handle, VOID *pv_tag)
{
    UNUSED(pt_tm_handle);
    UNUSED(pv_tag);

    // Turn off line-in buffer control
    AUD_LineInCtrlEnable(AUD_DEC_MAIN, FALSE);

    // Stop stream source
    AUD_StopMemStr();

    SLT_CaptureReady(SLT_TYPE_AUD, 0);
}
*/
static VOID _sltAudCaptureReady1(void)
{

    // Turn off line-in buffer control
    AUD_LineInCtrlEnable(AUD_DEC_MAIN, FALSE);

    // Stop stream source
    AUD_StopMemStr();
//    SLT_CaptureReady(SLT_TYPE_AUD, 0);
}

extern const unsigned char _au1SltPcm[];

static INT32 _sltAudCompare(UINT8 u1DspId)
{
    UINT8 * pu1ValidStr;
    UINT8 * pu1StrStart;
    UINT8 * pu1GoldenStart;
    INT32  i4Idx = 0;
    INT32  i4Ret = 0;

    pu1StrStart = (UINT8 *)VIRTUAL(u4GetAFIFOStart(u1DspId, AUD_DEC_AUX));

    DSP_InvalidateDCache(u4GetAFIFOStart(u1DspId, AUD_DEC_AUX), _u4SltStreamLen);

    // Search none zero byte as string start
    for (i4Idx = 0; i4Idx < _u4SltStreamLen; i4Idx++)
    {
        if ( *(pu1StrStart + i4Idx) != 0)
        {
            pu1ValidStr = (pu1StrStart + i4Idx);
            LOG(5, "Find stream start at addr = 0x%x , length = %d\n", pu1ValidStr, i4Idx);
            break;
        }
        else if (i4Idx == (_u4SltStreamLen - 1))
        {
            Printf("All stream are zero\n");
        }
    }
    pu1GoldenStart = (void*)x_mem_alloc(AUD_SLT_COMPARED_LEN);
	
    if (u1DspId == AUD_DSP1)
    {
	      NOR_Read((UINT64) ((NOR_SLT_START_ADDR + NOR_SLT_AUD_ADDR_OFFSET+ _u4SltStreamLen)),(const VOID *)pu1GoldenStart, AUD_SLT_COMPARED_LEN);    
    }
    else
    {	
	NOR_Read((UINT64) ((NOR_SLT_START_ADDR + NOR_SLT_AUD_ADDR_OFFSET)),(const VOID *)pu1GoldenStart, AUD_SLT_COMPARED_LEN);
	  }

    i4Ret = x_memcmp((const VOID *) (pu1ValidStr),
    	             (const VOID *)(pu1GoldenStart),
                     AUD_SLT_COMPARED_LEN);
/*
    i4Ret = x_memcmp((const VOID *) (pu1ValidStr),
                     (const VOID *) (_au1SltPcm),
                     AUD_SLT_COMPARED_LEN);
*/ 

    //Printf("SLT compared len = 0x%x, Ret = %d\n", AUD_SLT_COMPARED_LEN, i4Ret);
    return i4Ret;
}

static INT32 _sltAudFlash(UINT8 u1DspId)
{
    UINT8 * pu1ValidStr;
    UINT8 * pu1StrStart;
    INT32 i4Idx;
    INT32 i4Ret = 0;
    pu1StrStart = (UINT8 *)VIRTUAL(u4GetAFIFOStart(u1DspId, AUD_DEC_AUX));

    DSP_InvalidateDCache(u4GetAFIFOStart(u1DspId, AUD_DEC_AUX), _u4SltStreamLen);

    if ((u1DspId== AUD_DSP1)&&(_i4AudDspbDtvFlashingFlag == 0))
    {
        return i4Ret;
    }

    // Search none zero byte as string start
    for (i4Idx = 0; i4Idx < _u4SltStreamLen; i4Idx++)
    {
        if ( *(pu1StrStart + i4Idx) != 0)
        {
            pu1ValidStr = (pu1StrStart + i4Idx);
            LOG(5, "Find addr = 0x%x , length = %d\n", pu1ValidStr, i4Idx);
            break;
        }
        else if (i4Idx == (_u4SltStreamLen - 1))
        {
            LOG(5, "All stream are zero\n");
            i4Ret = -1;
        }
    }
#ifndef __KERNEL__
    // Write golden to flash
    CLI_Parser_Arg("nor.e 0x%x 0x%x", NOR_SLT_START_ADDR + NOR_SLT_AUD_ADDR_OFFSET, _u4SltStreamLen);
    CLI_Parser_Arg("nor.wr 0x%x 0x%x 0x%x", NOR_SLT_START_ADDR + NOR_SLT_AUD_ADDR_OFFSET, pu1ValidStr, _u4SltStreamLen);
#else	
    if (u1DspId == AUD_DSP1)
    {
	      NOR_EraseAddr((UINT32)(NOR_SLT_START_ADDR + NOR_SLT_AUD_ADDR_OFFSET + _u4SltStreamLen), _u4SltStreamLen);
	      NOR_Write((UINT64)(NOR_SLT_START_ADDR + NOR_SLT_AUD_ADDR_OFFSET+ _u4SltStreamLen), pu1ValidStr, _u4SltStreamLen);    
	      _i4AudDspbDtvFlashingFlag = 0;
    }
    else
    {
	NOR_EraseAddr((UINT32)(NOR_SLT_START_ADDR + NOR_SLT_AUD_ADDR_OFFSET), _u4SltStreamLen);
	NOR_Write((UINT64)(NOR_SLT_START_ADDR + NOR_SLT_AUD_ADDR_OFFSET), pu1ValidStr, _u4SltStreamLen);
	  }
#endif

    LOG(0, "Audio dsp(%d) golden flashing completely\n", u1DspId);

    return i4Ret;
}

static void _sltAudThread(void *pvArgv)
{
    UINT8 u1DspId = AUD_DSP0;

    if (pvArgv != NULL)
    {
        u1DspId = *(UINT8 *)pvArgv;
    }
    UNUSED(pvArgv);


    _i4AudSltResult = 0;

    // Audio slt init
    _sltAudInit(u1DspId);
    AUD_SET_BIT(REG_TEST_CFG, DTVSLT);
    AUD_CLR_BIT(REG_ACK2_CFG, LIN_CLK_AUD3_SEL);
    AUD_DspBypassPostProcess(u1DspId, AUD_DEC_MAIN,TRUE);
    VERIFY(x_sema_create(&_hSltAud, X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK) == OSR_OK);

    //CLI_Parser("aud.io.cfg.isw 0");
    //AUD_AvMuxSel(0);
    AUD_AvMuxSel(AUD_INPUT_ID_COMP_VID_0);
    AUD_WRITE32(REG_EXTADC_CFG, 0x172e);
    // Start playing audio
    //CLI_Parser("aud.play");
     AUD_DSPCmdPlay(u1DspId, AUD_DEC_MAIN);

#if 1
    // Work around for error data generated at the first decoder play
    AUD_LineInCtrlEnable(AUD_DEC_MAIN, TRUE);
    x_thread_delay(20);
    AUD_DSPCmdStop(u1DspId, AUD_DEC_MAIN);
    x_thread_delay(30);
    AUD_LineInCtrlEnable(AUD_DEC_MAIN, FALSE);
    //CLI_Parser("aud.play");
    AUD_DSPCmdPlay(u1DspId, AUD_DEC_MAIN);
#endif

    // Open line-in and start capturing data
    //AUD_LineInCtrlEnable(AUD_DEC_MAIN, TRUE);
    AUD_LineInCtrlEnable(AUD_DEC_MAIN, FALSE);
    AUD_LineInCtrlEnable(AUD_DEC_MAIN, TRUE);
    AUD_SET_BIT(REG_INTADC_SDADC0, RST_FILTER | 0x40);
    AUD_CLR_BIT(REG_INTADC_SDADC0, RST_FILTER);


#if 0
    HAL_GetTime(&_rTime);
    LOG(5, "Audio start capturing .. Time (%d, %d)\n", _rTime.u4Seconds, _rTime.u4Micros);

    VERIFY(x_timer_create(&_hAudCaptureTimer) == OSR_OK);
    VERIFY(x_timer_start(_hAudCaptureTimer, AUD_SLT_CAPTURE_PERIOD, X_TIMER_FLAG_ONCE,
                         _sltAudCaptureReady, NULL) == OSR_OK);

    // Waiting for audio capture ready
    VERIFY(x_sema_lock(_hSltAud, X_SEMA_OPTION_WAIT) == OSR_OK);

    HAL_GetTime(&_rTime);
    LOG(5, "Audio stop capturing .. Time (%d, %d)\n", _rTime.u4Seconds, _rTime.u4Micros);
#else
    x_thread_delay(AUD_SLT_CAPTURE_PERIOD);
    _sltAudCaptureReady1();
#endif
    // Notes: use async stop instread of sync stop for fail IC, DSP won't ack flush done ..
    AUD_DSPCmdStop(u1DspId, AUD_DEC_MAIN);

    // Delay to insure DSP has finished stop command
    x_thread_delay(20);
    //AUD_UopCommandDone(AUD_DEC_MAIN, AUD_CMD_STOP);
    //AUD_CLR_BIT_SPECIAL(REG_TEST_CFG, DTVSLT);

    if (_u4SltAudMode == SLT_SORTING_MODE)
    {
        // Audio comparition with golden pattern
        if (_sltAudCompare(u1DspId) == 0)
        {
            // pattern match
            _i4AudSltResult = 0;
            LOG(5, "Aud compare match (ok) \n");
        }
        else
        {
            // pattern unmatch
            _i4AudSltResult = -1;
            LOG(5, "Aud compare unmatch (fail) \n");
        }

        if (_hSltAudFinish != NULL)
        {
            //Printf("Realse aud semaphore\n");
            VERIFY(x_sema_unlock(_hSltAudFinish) == OSR_OK);
        }
    }
    else if (_u4SltAudMode == SLT_FLASHING_MODE)
    {
        if (_sltAudFlash(u1DspId) != 0)
        {
            _i4AudSltResult = -1;
            Printf("Flash audio golden fail!\n");
        }
        else
        {
            _i4AudSltResult = 0;
        }
    }
    else
    {
    }

   // VERIFY(x_timer_delete(_hAudCaptureTimer) == OSR_OK);
   // VERIFY(x_sema_delete(_hSltAud) == OSR_OK);
   // AUD_DspBypassPostProcess(AUD_DEC_MAIN,FALSE);
    // destroy thread
    x_thread_exit();
}

void _sltDTV(UINT8 u1DspId)
{
    _i4AudSltResult = 0;

    //if (u1DspId == AUD_DSP0) AUD_WRITE32(REG_MODULE_CTRL, 0x0000); //semaphore switch to DSP0
    //else AUD_WRITE32(REG_MODULE_CTRL, 0x1555); //semaphore switch to DSP1

    // Audio slt init
    _sltAudInit(u1DspId);
    AUD_SET_BIT(REG_TEST_CFG, DTVSLT);
    AUD_CLR_BIT(REG_ACK2_CFG, LIN_CLK_AUD3_SEL);    
    AUD_DspBypassPostProcess(u1DspId, AUD_DEC_MAIN,TRUE);
    //VERIFY(x_sema_create(&_hSltAud, X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK) == OSR_OK);

    //CLI_Parser("aud.io.cfg.isw 0");
    //AUD_AvMuxSel(0);
    AUD_AvMuxSel(AUD_INPUT_ID_COMP_VID_0);
    AUD_WRITE32(REG_EXTADC_CFG, 0x172e);
    // Start playing audio
    //CLI_Parser("aud.play");
     AUD_DSPCmdPlay(u1DspId, AUD_DEC_MAIN);

#if 1
    // Work around for error data generated at the first decoder play
    AUD_LineInCtrlEnable(AUD_DEC_MAIN, TRUE);
    x_thread_delay(20);
    AUD_DSPCmdStop(u1DspId, AUD_DEC_MAIN);
    x_thread_delay(30);
    AUD_LineInCtrlEnable(AUD_DEC_MAIN, FALSE);
    //CLI_Parser("aud.play");
    AUD_DSPCmdPlay(u1DspId, AUD_DEC_MAIN);
#endif

    // Open line-in and start capturing data
    //AUD_LineInCtrlEnable(AUD_DEC_MAIN, TRUE);
    AUD_LineInCtrlEnable(AUD_DEC_MAIN, FALSE);
    AUD_LineInCtrlEnable(AUD_DEC_MAIN, TRUE);
    AUD_SET_BIT(REG_INTADC_SDADC0, RST_FILTER | 0x40);
    AUD_CLR_BIT(REG_INTADC_SDADC0, RST_FILTER);

#if 0
    HAL_GetTime(&_rTime);
    LOG(5, "Audio start capturing .. Time (%d, %d)\n", _rTime.u4Seconds, _rTime.u4Micros);

    VERIFY(x_timer_create(&_hAudCaptureTimer) == OSR_OK);
    VERIFY(x_timer_start(_hAudCaptureTimer, AUD_SLT_CAPTURE_PERIOD, X_TIMER_FLAG_ONCE,
                         _sltAudCaptureReady, NULL) == OSR_OK);

    // Waiting for audio capture ready
    VERIFY(x_sema_lock(_hSltAud, X_SEMA_OPTION_WAIT) == OSR_OK);

    HAL_GetTime(&_rTime);
    LOG(5, "Audio stop capturing .. Time (%d, %d)\n", _rTime.u4Seconds, _rTime.u4Micros);
#else
    x_thread_delay(AUD_SLT_CAPTURE_PERIOD);
    _sltAudCaptureReady1();
#endif
    // Notes: use async stop instread of sync stop for fail IC, DSP won't ack flush done ..
    AUD_DSPCmdStop(u1DspId, AUD_DEC_MAIN);

    // Delay to insure DSP has finished stop command
    x_thread_delay(180);
    //AUD_UopCommandDone(AUD_DEC_MAIN, AUD_CMD_STOP);
    //AUD_CLR_BIT_SPECIAL(REG_TEST_CFG, DTVSLT);
    // Disable Line-In  interrupt
    //VERIFY(BIM_DisableIrq(VECTOR_SPDIF));

    //AUD_WRITE32(REG_MODULE_CTRL, 0x0000); //semaphore switch back to DSP0

    if (_u4SltAudMode == SLT_SORTING_MODE)
    {
        // Audio comparition with golden pattern
        if (_sltAudCompare(u1DspId) == 0)
        {
            // pattern match
            _i4AudSltResult = 0;
            LOG(0, "Aud dsp(%d) compare match (ok) \n", u1DspId);
        }
        else
        {
            // pattern unmatch
            _i4AudSltResult = -1;
            LOG(0, "Aud dsp(%d) compare unmatch (fail) \n", u1DspId);
        }
#if 0
        if (_hSltAudFinish != NULL)
        {
            //Printf("Realse aud semaphore\n");
            VERIFY(x_sema_unlock(_hSltAudFinish) == OSR_OK);
        }
#endif
    }
    else if (_u4SltAudMode == SLT_FLASHING_MODE)
    {
        if (_sltAudFlash(u1DspId) != 0)
        {
            _i4AudSltResult = -1;
            Printf("Flash audio golden fail!\n");
        }
        else
        {
            _i4AudSltResult = 0;
        }
    }
    else
    {
    }

   // VERIFY(x_timer_delete(_hAudCaptureTimer) == OSR_OK);
   // VERIFY(x_sema_delete(_hSltAud) == OSR_OK);
   // AUD_DspBypassPostProcess(AUD_DEC_MAIN,FALSE);
    // destroy thread
    //x_thread_exit();
}


INT32 _sltGetDtvResult(void)
{
    UINT32 i4RValue;

    if (_u4SltAudMode == SLT_SORTING_MODE)
    {
        VERIFY(x_sema_create(&_hSltAudFinish, X_SEMA_TYPE_BINARY,
               X_SEMA_STATE_LOCK) == OSR_OK);

        //Printf("Aud create audio semaphore\n");

        if ((x_sema_lock_timeout(_hSltAudFinish, AUD_SLT_TIMEOUT)) != OSR_OK)
        {
            LOG(5, "Audio SLT get result time out!\n");
                  // fail
                  _i4AudSltResult = AUD_SORTING_DTV_FAIL;
            return AUD_SORTING_DTV_FAIL;
        }
        else
        {
                  return _i4AudSltResult;
        }
    }
    else
    {
            LOG(5, "Only sorting mode can get result\n");
    }

    VERIFY(x_sema_delete(_hSltAudFinish) == OSR_OK);
    _hSltAudFinish = NULL;

    return i4RValue;
}


UINT32 _sltGetAoutStrAddr(UINT32 * pu4StrLen)
{
    UINT32 u4CompactAddr;
    u4CompactAddr =((((AUD_READ32(REG_ADSP_CFG) & ADSP_AUDIO_BLOCK_MASK))>>ADSP_AUDIO_BLOCK_POS) <<24)|
        ((AUD_READ32(REG_DSP_CMPT_DRAM_SBLK01(AUD_DSP0)) & 0x0000ffff)  << 8);
    if (pu4StrLen)
    {
        *pu4StrLen = (0xd80*4);
    }
    return (u4CompactAddr + (0x1000)*4);
}

BOOL _sltAtdPathVerify(VOID)
{
    UINT8 * pu1Buf;
    UINT8   u1MsbH, u1MsbL, u1Lsb;
    BOOL    fgSign;
    INT32   i4Value, i;
    UINT32  u4StrLen;
    UINT32  u4FFTNum;
    //FLOAT   fMagSum = 0;
    int   fMagSum = 0;
    //DOUBLE dThdPlusN;
    int64 dThdPlusN;
    UINT32  u4MaxFreqPoint = 0;
    //DOUBLE dfTemp = 0;
    int64 dfTemp = 0;
    CHAR cBuf[32];
    UNUSED(cBuf);
    pu1Buf = (UINT8 *)VIRTUAL(_sltGetAoutStrAddr(&u4StrLen));
    u4FFTNum = u4StrLen/3;
    LOG(5,"buf address %d \n", pu1Buf);
    LOG(5,"input samples %d \n", u4FFTNum);

    // Load data
    for (i=0 ; i<u4FFTNum; i++)
    {
#if 0
        u1MsbH = *pu1Buf;
        u1MsbL = *(pu1Buf+1);
        u1Lsb  = *(pu1Buf+2);
#else
        u1Lsb = *pu1Buf;
        u1MsbL = *(pu1Buf+1);
        u1MsbH = *(pu1Buf+2);
#endif

        fgSign = ((u1MsbH & 0x80) == 0x80) ? TRUE: FALSE;
        i4Value = ((u1MsbH & 0x7f) << 16) + (u1MsbL << 8) + u1Lsb;
        //afRealPart[0][i] = (FLOAT)((fgSign == TRUE) ? (i4Value - 0x800000) : i4Value);
        //afRealPart[0][i] = afRealPart[0][i]/80000;
        //afImagPart[0][i] = 0;
        //pu1Buf += 3;

        atd[0][i].real = ((fgSign == TRUE) ? (i4Value - 0x800000) : i4Value);
        atd[0][i].real = atd[0][i].real/80000;
        atd[0][i].imag = 0;
        pu1Buf += 3;
    }

    LOG(5,"atd.real[0][i] %d \n", atd[0][1].real);
    // FFT
    //inplace_fft(afRealPart[0], afImagPart[0], FFT_LEN_IN_POW2);
    Fixed_fft(atd[0]);
    // Compute magnitued
    for (i=0 ; i<FFT_PIONT_NUM; i++)
    {
        Magnitued[0][i] = atd[0][i].real*atd[0][i].real + atd[0][i].imag*atd[0][i].imag;
    }

    // Normalized
    for (i=0 ; i<FFT_PIONT_NUM; i++)
    {
        //adfMagnitued[0][i] = adfMagnitued[0][i] / 237000;
        //adfMagnitued[0][i] = adfMagnitued[0][i] / 6370000;
        Magnitued[0][i] = Magnitued[0][i] / 3000000;//1250000; // Tune for mt5388
    }

#if 0 // linking error
    LOG(5,"DC value = %s\n", ftoa(cBuf, (FLOAT)adfMagnitued[0], 6));
#endif

/*
    // Check DC value
    if ((adfMagnitued[0] > DC_VALUE_MAX_THRESHOLD) ||
         (adfMagnitued[0] < DC_VALUE_MIN_THRESHOLD))
    {
        LOG(5,"invalid DC value\n");
        //return FALSE;
    }
*/

    // Check maximum magnitude
    for (i=0; i<((FFT_PIONT_NUM/2) - 1) ; i++)
    {
        if (Magnitued[0][i] > dfTemp)
        {
            u4MaxFreqPoint = i;
            dfTemp = Magnitued[0][i];
        }
    }

    // Compute THD+N
    for (i=0 ; i<FFT_PIONT_NUM; i++)
    {
        fMagSum += Magnitued[0][i]*Magnitued[0][i];
    }

    dThdPlusN =  (UINT32)(fMagSum - Magnitued[0][u4MaxFreqPoint]*Magnitued[0][u4MaxFreqPoint]);

#if 0 // linking error
    LOG(5,"THD+N = %s\n",ftoa(cBuf, (FLOAT)dThdPlusN, 6));
#endif

    LOG(1,"ATV THD+N = %d \n", (INT32)dThdPlusN);
    if (dThdPlusN > THD_PLUS_N_THRESHOLD)
    {
        LOG(5,"Invalid THD+N value \n");
        return FALSE;
    }

#if 0 // linking error
    LOG(5,"u4MaxMag[%d] = %s\n", u4MaxFreqPoint, ftoa(cBuf, (FLOAT)adfMagnitued[u4MaxFreqPoint], 6));
#endif

    LOG(1,"ATV Magnitued = %d, %d \n",u4MaxFreqPoint, (INT32)Magnitued[0][u4MaxFreqPoint]);
    printf("\n\nu4MaxFreqPoint=%d\n\n",u4MaxFreqPoint);
    if ((u4MaxFreqPoint < FREQ_POINT_ATD0) ||
        (u4MaxFreqPoint > FREQ_POINT_ATD1) ||
    	 (Magnitued[0][u4MaxFreqPoint] < MAG_MIN_THRESHOLD) ||
    	 (Magnitued[0][u4MaxFreqPoint] > MAG_MAX_THRESHOLD))
    {
        LOG(5,"Invalid Max Freq\n");
        return FALSE;
    }
    return TRUE;
}


INT32 _sltAtdSorting(void)
{
    UINT32 u4Data;

    // Set AUDIN_CFG
    u4Data = (AUD_READ32(REG_AUDIN_CFG) & (~REG_AUDIN_CFG_MASK));
    u4Data = u4Data | APGA_GAIN_0DB | APGA_OFFSET_0V | APGA_GAIN_BY_DSP | ADCK_INV_RISING | ADC_SEL_DEMOD;
    AUD_WRITE32(REG_AUDIN_CFG, u4Data);
    DSP_ChangeSourceMode(1);

    //x_thread_delay(2000);
    // Power on DAC1 R channel only
    AUD_WRITE32(REG_IDACHCG, 0);
    AUD_DspBypassPostProcess(AUD_DSP0, AUD_DEC_MAIN,TRUE);

    //DSP_ChangeSourceMode(1);
#if 0
    CLI_Parser("aud.s 0 0 2 11");
    CLI_Parser("aud.play0 0");
    CLI_Parser("aud.uop.v 0 50");
    CLI_Parser("aud.t.s 0");
#else
    AUD_SetDecType(AUD_DSP0, AUD_DEC_MAIN, AUD_STREAM_FROM_ANALOG_TUNER, AUD_FMT_MTS);
    AUD_DSPCmdPlay(AUD_DSP0, AUD_DEC_MAIN);
    AUD_DspMasterVolume(AUD_DEC_MAIN, 50);
    AUD_DspSoundEffectFlag(AUD_DEC_MAIN, 0);
#endif
    x_thread_delay(1200);

    AUD_DSPCmdStopAsyn(AUD_DSP0, AUD_DEC_MAIN);
    // Delay to insure DSP has finished stop command
    x_thread_delay(200);
    AUD_UopCommandDone(AUD_DSP0, AUD_DEC_MAIN, AUD_CMD_STOP);
    AUD_DspBypassPostProcess(AUD_DSP0, AUD_DEC_MAIN,FALSE);

    if (_sltAtdPathVerify() != TRUE)
    {
        return AUD_SORTING_ATV_FAIL;
    }

    return AUD_SORTING_OK;
}

INT32 _sltAtdEuSorting(void)
{
    return AUD_SORTING_OK;
}

// check 1 or 2 channels
BOOL _sltIntAdPathVerify(UINT8 u1ChNum)
{
    UINT8 * pu1Buf;
    UINT8   u1MsbH, u1MsbL, u1Lsb;
    BOOL    fgSign;
    INT32   i4Value, i, q;
    int   fDcValue[2] = {0};
    int   fMagSum[2] = {0};
    int64 dThdPlusN[2];
    UINT32  u4MaxFreqPoint = 0;
    int64 dfTemp = 0;
    BOOL fgResult = TRUE;

    pu1Buf = (UINT8 *)VIRTUAL(LINE_IN_BUF_SA(AUD_DSP0)) + 6*5000;  //ignore the first 11000 samples
    //pu1Buf = (UINT8 *)LINE_IN_BUF_SA;
    // Load data
    for (i=0 ; i<FFT_PIONT_NUM; i++)
    {
        for (q=0 ; q<2; q++)     // Jessica: Fix to 2 channels for Line in
        {
            u1MsbH = *pu1Buf;
            u1MsbL = *(pu1Buf+1);
            u1Lsb  = *(pu1Buf+2);

            fgSign = ((u1MsbH & 0x80) == 0x80) ? TRUE: FALSE;
            i4Value = ((u1MsbH & 0x7f) << 16) + (u1MsbL << 8) + u1Lsb;
            ad[q][i].real = ((fgSign == TRUE) ? (i4Value - 0x800000) : i4Value);
            ad[q][i].imag = 0;
            pu1Buf += 3;
        }
    }

    for (q=0 ; q<u1ChNum; q++)
    {
        // Remove DC value and normalize data
        for (i=0 ; i<FFT_PIONT_NUM; i++)
        {
            fDcValue[q] += ad[q][i].real / FFT_PIONT_NUM;
        }

        for (i=0 ; i<FFT_PIONT_NUM; i++)
        {
            ad[q][i].real = ad[q][i].real - fDcValue[q];
            ad[q][i].real = ad[q][i].real / 0x07ffff;
        }

        // FFT
        //inplace_fft(afRealPart[q], afImagPart[q], FFT_LEN_IN_POW2);
        Fixed_fft(ad[q]);
        for (i=0 ; i<FFT_PIONT_NUM; i++)
        {
            // Compute magnitude
            Magnitued[q][i] = ad[q][i].real*ad[q][i].real + ad[q][i].imag*ad[q][i].imag;
            // Normalized
            Magnitued[q][i] = Magnitued[q][i] / 156000;/// adjust to let adfMagnitued[q][u4MaxFreqPoint] within 50~400
            // Compute THD+N
            fMagSum[q] += Magnitued[q][i]*Magnitued[q][i];
        }

        dThdPlusN[q] =  (UINT32)(fMagSum[q] - Magnitued[q][FREQ_POINT_AD]*Magnitued[q][FREQ_POINT_AD]);

        LOG(0, "In-AD THD+N = %d \n", (INT32)dThdPlusN[q]);
        if ((dThdPlusN[q] > THD_PLUS_N_THRESHOLD_AD))
        {
            LOG(0,"Internal AD SLT failed at THD+N of ch %d \n", q);
            fgResult = FALSE;
       }

        // Check maximum magnitude of channel 1
        for (i=0; i<((FFT_PIONT_NUM/2) - 1) ; i++)
        {
            if (Magnitued[q][i] > dfTemp)
            {
                u4MaxFreqPoint = i;
                dfTemp = Magnitued[q][i];
            }
        }

        LOG(0,"In-AD Magnitued = %d \n", (INT32)Magnitued[q][u4MaxFreqPoint]);
        //LOG(5,"u4MaxMag[%d] = %s\n", u4MaxFreqPoint, ftoa(cBuf, (FLOAT)adfMagnitued[u4MaxFreqPoint], 6));
        printf("\n\nu4MaxFreqPoint=%d\n\n",u4MaxFreqPoint);
        if ((u4MaxFreqPoint != FREQ_POINT_AD) ||
        	 (Magnitued[q][u4MaxFreqPoint] < MAG_MIN_THRESHOLD) ||
        	 (Magnitued[q][u4MaxFreqPoint] > MAG_MAX_THRESHOLD))
        {
            LOG(0,"Invalid Max Freq of ch %d \n", q);
            fgResult = FALSE;
        }
    }

    return fgResult;
}

INT32 _sltIntAdSorting(UINT8 u1DacId, UINT8 u1ChNum)
{
    UINT32 u4DacPwdReg;
    UNUSED(u4DacPwdReg);
    // Power on only 1 DAC
//    u4DacPwdReg = (0x77 & ~(0x11 << u1DacId)) << 4;
    #if defined(CC_MT5389)
         ANA_WRITE32(REG_AADC_CFG0, (ANA_READ32(REG_AADC_CFG0) & ~ADC_INPUT_MUX_MASK) | ADC_IN2L2R);
    #elif defined(CC_MT5398)
        ANA_WRITE32(REG_AADC_CFG0, (ANA_READ32(REG_AADC_CFG0) & ~ADC_INPUT_MUX_MASK) | ADC_IN4L4R);
    #elif defined(CC_MT5880)
        ANA_WRITE32(REG_AADC_CFG0, (ANA_READ32(REG_AADC_CFG0) & ~ADC_INPUT_MUX_MASK) | ADC_IN1L1R);
    #elif defined(CC_MT5881) // may be fixed, paul
        ANA_WRITE32(REG_AADC_CFG0, (ANA_READ32(REG_AADC_CFG0) & ~ADC_INPUT_MUX_MASK) | ADC_IN3L3R);
    #elif defined(CC_MT5890)
        ANA_WRITE32(REG_AADC_CFG0, (ANA_READ32(REG_AADC_CFG0) & ~ADC_INPUT_MUX_MASK) | ADC_IN3L3R);
    #else
        ANA_WRITE32(REG_AADC_CFG0, (ANA_READ32(REG_AADC_CFG0) & ~ADC_INPUT_MUX_MASK) | ADC_IN3L3R);
    #endif
    ANA_CLR_BIT(REG_AADC_CFG0, (ADC_L_PD | ADC_R_PD)); // power on
    AUD_WRITE32(REG_ACK2_CFG, 0x22);
    vSetSPDIFInFIFO(AUD_DEC_MAIN, LINE_IN_BUF_SA(0), LINE_IN_BUF_SA(0) + LINE_IN_BUF_SZ(0));
    DSP_FlushInvalidateDCacheFree(LINE_IN_BUF_SA(0), LINE_IN_BUF_SZ(0));  //test 0916
    AUD_WRITE32(REG_LIN_CFG0, SPDIF_LINE_IN_DISABLE |
                               DATA_PCM_24_BITS |
                               RISC_INT_PERIOD_256 |
                               PCM_CH_NUM_2_CH |
                               INT_ADC_INTLIN);
    AUD_WRITE32(REG_EXTADC_CFG, 0x172e);

    AUD_WRITE32(REG_IDACHCG, 0);

    // play test tone from first decoder
#if defined(CC_MT5389)
    CLI_Parser("aud.t.t 0");
#else
    CLI_Parser("aud.t.t 1 0 0");
#endif
   #if 0
   MEM_BUFFER_INFO_T  rMemBuf;
   AUD_PCM_SETTING_T  rPcmSetting;
   if (AUD_IsDecoderPlay(AUD_DEC_MAIN))
    {
        AUD_StopMemStr();
        AUD_DSPCmdStop(AUD_DEC_MAIN);
    }
    rMemBuf.u4LoopCount = 0; // Infinite loop
    rPcmSetting.eSampleFreq = FS_48K;
    rPcmSetting.ePcmDataInfo = PCM_24_BITS;
    AUD_DspProcMode(AUD_DEC_MAIN, 0xdc00);
    AUD_SetDecType(AUD_DEC_MAIN, AUD_STREAM_FROM_MEMORY, AUD_FMT_PCM);
    AUD_PcmSetting(AUD_DEC_MAIN, (const AUD_PCM_SETTING_T *)&rPcmSetting);
    AUD_StrSelect(AUD_DEC_MAIN, 0, &rMemBuf);
    AUD_SetDecMemBuffer(AUD_DEC_MAIN, &rMemBuf);
    AUD_DSPCmdPlay(AUD_DEC_MAIN);
    #endif
#if defined(CC_MT5389)
        CLI_Parser("aud.t.b 1");
#else
        //CLI_Parser("aud.t.s 0");
        AUD_DspSoundEffectFlag(AUD_DEC_MAIN, FALSE);

        //CLI_Parser("aud.uop.v 0 100");
        AUD_DspMasterVolume(AUD_DEC_MAIN, 100);
#endif




    // Turn on line-in buffer control
    //AUD_LineInCtrlEnable(AUD_DEC_MAIN, TRUE);
    AUD_LineInCtrlEnable(AUD_DEC_MAIN, FALSE);
    AUD_LineInCtrlEnable(AUD_DEC_MAIN, TRUE);
    AUD_SET_BIT(REG_INTADC_SDADC0, RST_FILTER | 0x40);
    AUD_CLR_BIT(REG_INTADC_SDADC0, RST_FILTER);

    x_thread_delay(1200);

    // Turn off line-in buffer control
    AUD_LineInCtrlEnable(AUD_DEC_MAIN, FALSE);

    AUD_StopMemStr();
    // Notes: use async stop instread of sync stop for fail IC, DSP won't ack flush done ..
    AUD_DSPCmdStopAsyn(AUD_DSP0, AUD_DEC_MAIN);

    //CLI_Parser("aud.stop");
    // Delay to insure DSP has finished stop command
    x_thread_delay(200);
    AUD_UopCommandDone(AUD_DSP0, AUD_DEC_MAIN, AUD_CMD_STOP);

    // Power on all DAC

    DSP_InvalidateDCache(LINE_IN_BUF_SA(0), LINE_IN_BUF_SZ(0));  //test 0916

    if (_sltIntAdPathVerify(u1ChNum) != TRUE)
    {
        LOG(0, "Internal DAC%d fail\n", u1DacId);
        return AUD_SORTING_IAD_FAIL;
    }

    LOG(1, "Internal DAC%d pass\n", u1DacId);
    return AUD_SORTING_OK;
}

void AUD_SltLineInInit(UINT8 u1DspId)
{

    vSetSPDIFInFIFO(AUD_DEC_MAIN, LINE_IN_BUF_SA(u1DspId), LINE_IN_BUF_SA(u1DspId) + LINE_IN_BUF_SZ(u1DspId) );
    AUD_WRITE32(REG_LIN_CFG0, SPDIF_LINE_IN_DISABLE |
                               DATA_PCM_24_BITS |
                               RISC_INT_PERIOD_256 |
                               PCM_CH_NUM_2_CH);
    AUD_SET_BIT(REG_BS_CFG(u1DspId), PTR_SEL_BANK0_SPDIF_LINE_IN);

    // Clear line in buffer

    x_memset((VOID *)VIRTUAL(LINE_IN_BUF_SA(u1DspId)), 0x80, LINE_IN_BUF_SZ(u1DspId));
    
    DSP_FlushInvalidateDCacheFree(LINE_IN_BUF_SA(u1DspId), LINE_IN_BUF_SZ(u1DspId));

    // Line in format config
    // AUD_LineInCfg(); // TODO

    // Enable Line-In  interrupt
    //VERIFY(BIM_EnableIrq(VECTOR_SPDIF));
}

void _sltAudReInitDSPB(void)
{
    AUD_DspProbePause(AUD_DSP0);
    AUD_DspProbePause(AUD_DSP1);
    DSP_SetAenvSemaCtrl(AUD_DSP0, 0x0);
    DSP_SetAenvSemaCtrl(AUD_DSP1, 0xF);
    AUD_WRITE32(REG_MODULE_CTRL, 0x1555); 
    DSP_SetAenvSemaCtrlToDSP(AUD_DSP0);
    DSP_SetAenvSemaCtrlToDSP(AUD_DSP1);   
    vResetDSP(AUD_DSP0);
    vResetDSP(AUD_DSP1);
    AUD_DspProbeGo(AUD_DSP0);
    AUD_DspProbeGo(AUD_DSP1);

    AUD_DrvThreadWakeup(AUD_DSP0, AUD_DEC_MAIN);
    AUD_DrvThreadWakeup(AUD_DSP0, AUD_DEC_AUX);
    AUD_DrvThreadWakeup(AUD_DSP1, AUD_DEC_MAIN);
    AUD_DrvThreadWakeup(AUD_DSP1, AUD_DEC_AUX);

}

void _sltAudReInitDSPA(void)
{
    AUD_DspProbePause(AUD_DSP0);
    AUD_DspProbePause(AUD_DSP1);
    DSP_SetAenvSemaCtrl(AUD_DSP0, 0xF);
    DSP_SetAenvSemaCtrl(AUD_DSP1, 0x0);
    AUD_WRITE32(REG_MODULE_CTRL, 0x0000); 
    DSP_SetAenvSemaCtrlToDSP(AUD_DSP0);
    DSP_SetAenvSemaCtrlToDSP(AUD_DSP1);   
    vResetDSP(AUD_DSP0);
    vResetDSP(AUD_DSP1);
    AUD_DspProbeGo(AUD_DSP0);
    AUD_DspProbeGo(AUD_DSP1);

    AUD_DrvThreadWakeup(AUD_DSP0, AUD_DEC_MAIN);
    AUD_DrvThreadWakeup(AUD_DSP0, AUD_DEC_AUX);
    AUD_DrvThreadWakeup(AUD_DSP1, AUD_DEC_MAIN);
    AUD_DrvThreadWakeup(AUD_DSP1, AUD_DEC_AUX);

}

void SLT_FlashingDSPBAudInit(void)
{    
    UINT8 u1DspId = AUD_DSP1;
    
    // ** part I, reboot 2 DSPs, switch aenv to DSPB **//
    //x_thread_delay(15000); => no need
    _sltAudReInitDSPB();
    
    //*** part II, flashing ***//

    // Audio slt init
    x_thread_delay(50);  //=> 5s/1s/100 is OK
    _sltAudInit(u1DspId);

    _u4SltAudMode = SLT_FLASHING_MODE;

    if (x_thread_create(&_hSltAudThread, "SLT_AUD", 2048, CLI_THREAD1_PRIORITY,
                        _sltAudThread, sizeof(UINT8), (void *)&u1DspId) != OSR_OK)
    {
        Printf("Create DSP(%d) SLT_AUD thead fail!\n", u1DspId);
    }

    x_thread_delay(9500);  // 9000 is necessary, 8500 will fail
    //*** part III, sorting ***//

    INT32 i4AudResult;

    _u4SltAudMode = SLT_SORTING_MODE;

    _sltDTV(AUD_DSP1);
    i4AudResult = _i4AudSltResult;
    
    
    // ** part IV, reboot 2 DSPs, switch aenv back to DSPA **//
    //_sltAudReInitDSPA();
    AUD_pm_suspend();
    x_thread_delay(50);
    AUD_pm_resume();
    
    if (i4AudResult == -1) return i4AudResult;
    //x_thread_delay(200);
    return i4AudResult;
}

INT32 SLT_DtvSorting(void)
{
    INT32 i4AudResult;
    // DTV part sorting
    ADAC_Mute(FALSE);

    _u4SltAudMode = SLT_SORTING_MODE;
#if 0
    if (x_thread_create(&_hSltAudThread, "SLT_AUD", 2048, CLI_THREAD1_PRIORITY, _sltAudThread, 0, NULL) != OSR_OK)
    {
         LOG(5,"Create SLT_AUD thead fail!\n");
    }
    i4AudResult = _sltGetDtvResult();
#else
    _sltDTV(AUD_DSP0);
     //x_thread_delay(AUD_SLT_TIMEOUT);
    i4AudResult = _i4AudSltResult;
    if (i4AudResult == -1) return i4AudResult;
#endif
#if 0 
    // DSPB SLT : DTV_AC3 -> instruction pattern

#ifndef CC_AUD_DISABLE_2ND_DSP
    //_sltDTV(AUD_DSP1);
    SLT_FlashingDSPBAudInit();
    //x_thread_delay(200);
    i4AudResult = _i4AudSltResult;
#endif    
#endif    
    return i4AudResult;
}

INT32 SLT_AtvSorting(void)
{
    return TRUE;
}

INT32 SLT_AtdSorting(void)
{
    return _sltAtdSorting();
}

INT32 SLT_AtdEuSorting(void)
{
    return _sltAtdEuSorting();
}

INT32 SLT_IntAdSorting(void)
{
    INT32 i4AudResult;
    ADAC_Mute(FALSE);

    i4AudResult = _sltIntAdSorting(INTERNAL_DAC_ID_MAIN, 2);
    if (i4AudResult != AUD_SORTING_OK)
    {
        return i4AudResult;
    }

    i4AudResult = _sltIntAdSorting(INTERNAL_DAC_ID_AUX, 1);
    if (i4AudResult != AUD_SORTING_OK)
    {
        return i4AudResult;
    }

    //sunman  PWM only 2 outputs
    //i4AudResult = _sltIntAdSorting(INTERNAL_DAC_ID_THIRD, 2);
    return i4AudResult;
}

#define AUD_ROMCOD        0xFC

INT32 _sltRomSorting(void)
{
    UINT32 u4Data;
    BOOL    fgResult = FALSE ;
    UINT32 u4Counter1=0;
    UINT32 u4Counter2=0;
    UNUSED(fgResult);
    x_thread_delay(300);
    //disable DSP
    u4Data = (AUD_READ32(REG_ADSP_CFG) & (~ADSP_CFG_RST_B) );
    AUD_WRITE32(REG_ADSP_CFG, u4Data);

    // Load Rom Code
    DSP_LoadRamCode (AUD_DSP0, AUD_ROMCOD);
    //DSP_LoadRamCode (AUD_COMMRAM);
    //DSP_LoadRamCode (AUD_AININTRAM);
    // Enable Ram Boot
    vSetDspBootFromRam (AUD_DSP0);

    // enable DSP
    vTriggerDSP(AUD_DSP0);

    x_thread_delay(5);
    DSP_ReadDecDram(0x20, &u4Counter1);
    x_thread_delay(10);
    DSP_ReadDecDram(0x20, &u4Counter2);
    LOG(5, "u4Counter1,  u4Counter2= (%d, %d)\n", u4Counter1, u4Counter2);

    if (u4Counter1==u4Counter2)
    {
        return AUD_SORTING_ROM_FAIL;
    }

    return AUD_SORTING_OK;


}
INT32 SLT_ROMSorting(void)
{
    BOOL fgFail = FALSE;
    UINT32 u4Data;

    fgFail =  _sltRomSorting();

    x_thread_delay(300);
    //disable DSP & set to rom boot
    u4Data = AUD_READ32(REG_ADSP_CFG);
    AUD_WRITE32(REG_ADSP_CFG, ((u4Data & (~ADSP_BOOT_FROM_RAM)) & (~ADSP_CFG_RST_B)));
    vTriggerDSP(AUD_DSP0);

    if (fgFail)
    {
        return AUD_SORTING_ROM_FAIL;
    }

    return AUD_SORTING_OK;
}

#ifndef CC_AUD_DISABLE_2ND_DSP
INT32 SLT_DSPBSorting(void)
{
    BOOL fgSuccess = FALSE;
    UINT32 u4Data1 = 0;
    UINT32 u4Data2 = 0;
    UINT32 u4Data3 = 0;

    AUD_DspPatternTest(AUD_DSP1, 13, FALSE); // INST_TST_PAT = 13

    x_thread_delay(100);
    
    AUD_DspProbePause(AUD_DSP1);
    u4Data1 = u4AUD_DspProbeRead(AUD_DSP1, 0x1453);
    AUD_DspProbeGo(AUD_DSP1);
    
    x_thread_delay(20);

    AUD_DspProbePause(AUD_DSP1);    
    u4Data2 = u4AUD_DspProbeRead(AUD_DSP1, 0x1453);
    AUD_DspProbeGo(AUD_DSP1);
    
    x_thread_delay(20);    
    
    AUD_DspProbePause(AUD_DSP1);
    u4Data3 = u4AUD_DspProbeRead(AUD_DSP1, 0x1453);
    AUD_DspProbeGo(AUD_DSP1);

    if ((u4Data1 == 0) || (u4Data2 == 0) || (u4Data3 == 0))
    {
        fgSuccess = FALSE; 
    } 
    else 
    {
        if ((u4Data2 > u4Data1) && (u4Data3 > u4Data2))
        {
            fgSuccess = TRUE;        
        }
        else
        {
            fgSuccess = FALSE;        
        }
    }
    LOG(5, "u4Data1 = %x\n", u4Data1);
    LOG(5, "u4Data2 = %x\n", u4Data2);
    LOG(5, "u4Data3 = %x\n", u4Data3);
    LOG(5, "fgSuccess = %d\n", fgSuccess);

    if (!fgSuccess)
    {
        return AUD_SORTING_ROM_FAIL;
    }

    return AUD_SORTING_OK;
}
#endif

void SLT_CaptureReady(UINT32 u4Type, UINT32 u4Pts)
{
    if (u4Type == SLT_TYPE_AUD)
    {
        HAL_GetTime(&_rTime);
        LOG(5, "Audio capture ready = (%d, %d)\n", _rTime.u4Seconds, _rTime.u4Micros);
        VERIFY(x_sema_unlock(_hSltAud) == OSR_OK);
    }
}

void SLT_FlashingAudInit(void)
{
    UINT8 u1DspId = AUD_DSP0;
    // Audio slt init
    _sltAudInit(u1DspId);

    _u4SltAudMode = SLT_FLASHING_MODE;

    if (x_thread_create(&_hSltAudThread, "SLT_AUD", 2048, CLI_THREAD1_PRIORITY,
                        _sltAudThread, sizeof(UINT8), (void *)&u1DspId) != OSR_OK)
    {
        Printf("Create DSP(%d) SLT_AUD thead fail!\n", u1DspId);
    }
#ifndef CC_AUD_DISABLE_2ND_DSP    
    _i4AudDspbDtvFlashingFlag = 1;
    //set this flag to be 1 for nex-time,SLT_FlashingDSPBAudInit(), DSPB to perform flashing 
#endif    
}


INT32 SLT_SortingAud(void)
{
    INT32 i4AudResult = AUD_SORTING_OK;

    i4AudResult = SLT_DtvSorting();
    LOG(5,"SLT_DtvSorting = %d\n",i4AudResult);

    if (i4AudResult != AUD_SORTING_OK)
    {
        return AUD_SORTING_DTV_FAIL;
    }

    i4AudResult = SLT_AtvSorting();
    LOG(5,"SLT_AtvSorting = %d\n",i4AudResult);
    if (i4AudResult != AUD_SORTING_OK)
    {
        return AUD_SORTING_ATV_FAIL;
    }


    i4AudResult = SLT_IntAdSorting();
    LOG(5,"SLT_IntAdSorting = %d\n",i4AudResult);


    return i4AudResult;

}

#endif // __MODEL_slt__

